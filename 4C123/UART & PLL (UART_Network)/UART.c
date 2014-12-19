#include "UART.h"
#include "tm4c123gh6pm.h"

unsigned char String[10];
unsigned char flag;

void UART_Init(void)
{
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOD;   // enable clock for GPIO
	SYSCTL_RCGC1_R|= SYSCTL_RCGC1_UART2;   // enable clock for UART
		
	GPIO_PORTD_AFSEL_R|= 0xC0;        // enable alternate function on PA0-PA1 for TX/RX
	GPIO_PORTD_PCTL_R= (GPIO_PORTD_PCTL_R & ~0xFF000000) + 0x11000000;  // enable control for UART0 pins PA0, PA1
	GPIO_PORTD_DEN_R|= 0xC0; // PD7 == TX, PD6 == RX
	GPIO_PORTD_AMSEL_R&= ~0xC0;

	UART2_CTL_R = 0;    // Disable UART while configuration
	UART2_CC_R= 0; // UART Baud Rate source is the main clock source
	UART2_IBRD_R= 520;   // IBRD= Fosc / Baud Rate * CLCKDIV 
	UART2_FBRD_R= 53;   // (FBDR * 64) + 0.5
	UART2_LCRH_R= 0x00000070;  // 8-bit mode, one stop bit, no parity bit, enable FIFOs 
	UART2_CTL_R|= UART_CTL_UARTEN; // Enable UART
}

void TX_UART(unsigned char Char)
{
	while(UART2_FR_R & 0x20) {} // wait until TXFF gets empty space 
	UART2_DR_R= Char;	
}

unsigned char RX_UART(void)
{
	while(UART2_FR_R & 0x10) {} // wait until any data arrives (RXFE)
		
  return ( (unsigned char)(UART2_DR_R & 0xFF) );	// UART0_DR_R is 12-bits only 8 for data and 4 flags	
}
unsigned char getChar_UART(void)
{
	if (0 == (UART_FR_RXFE & UART_FR_R)) // return a character if there is
	{
		return ((unsigned char)(UART2_DR_R & 0xFF));
	}
	else return 0;
}

void UART_OutString(unsigned char buffer[])
{
// as part of Lab 11 implement this function
	unsigned char i= 0;   
	
	while(buffer[i] != '\0')
	{
			TX_UART(buffer[i++]);
	}
}

//-----------------------UART_ConvertUDec-----------------------
// Converts a 32-bit number in unsigned decimal format
// Input: 32-bit number to be transferred
// Output: store the conversion in global variable String[10]
// Fixed format 4 digits, one space after, null termination
// Examples
//    4 to "   4 "  
//   31 to "  31 " 
//  102 to " 102 " 
// 2210 to "2210 "
//10000 to "**** "  any value larger than 9999 converted to "**** "
void UART_ConvertUDec(unsigned long n)
{
	signed int i;
	flag= 1;
	
	for(i= 0; i < 10; ++i)
			String[i]= 0;

  for(i= 3; i > -1; --i)
	{
		  if(0 == n)
			{
				String[i]= '0';
				break;
			}
			else if(n < 10000)
			{
				String[i]= n % 10;
				String[i]|= 0x30;
				n/= 10;
			}
			else if(n > 9999)
				String[i]= '*';
	}
	String[4]= '\0'; 
	if('*' != String[0])
	{
		for(i= 0; i < 3; ++i)
		{
			if ((String[i] <= '9') && (String[i] >= '1')) {
				break;
			}
			else {
				String[i]= SP;
			}
		}
	}
	String[4]= SP;
}

//-----------------------UART_ConvertDistance-----------------------
// Converts a 32-bit distance into an ASCII string
// Input: 32-bit number to be converted (resolution 0.001cm)
// Output: store the conversion in global variable String[10]
// Fixed format 1 digit, point, 3 digits, space, units, null termination
// Examples
//    4 to "0.004 cm"  
//   31 to "0.031 cm" 
//  102 to "0.102 cm" 
// 2210 to "2.210 cm"
//10000 to "*.*** cm"  any value larger than 9999 converted to "*.*** cm"
void UART_ConvertDistance(unsigned long temp)
{
	signed int i;

	for(i= 4; i > -1; --i)
	{
		if(temp < 10000)
		{
			if(1 != i )
			{
				String[i]= temp % 10;
				String[i]|= 0x30;
				temp/= 10;
			}
			else {
				String[i]= '.';
			}
		}
		else
		{		
			if (1 != i)	{
				String[i]= '*';
			}
			else {
				String[i]= '.';
			}
		}
	}
	i= 5;
	String[i++]= ' ';
	String[i++]= 'c';
	String[i++]= 'm';
}

//-----------------------UART_OutDistance-----------------------
// Output a 32-bit number in unsigned decimal fixed-point format
// Input: 32-bit number to be transferred (resolution 0.001cm)
// Output: none
// Fixed format 1 digit, point, 3 digits, space, units, null termination
void UART_OutDistance(unsigned long n)
{
  UART_ConvertDistance(n);      // convert using your function
  UART_OutString(String);       // output using your function
}

//------------UART_InUDec------------
// InUDec accepts ASCII input in unsigned decimal format
//     and converts to a 32-bit unsigned number
//     valid range is 0 to 4294967295 (2^32-1)
// Input: none
// Output: 32-bit unsigned number
// If you enter a number above 4294967295, it will return an incorrect value
// Backspace will remove last digit typed
unsigned long UART_InUDec(void)
{
	unsigned long number=0, length=0;
	char character;
	
  character = RX_UART();
  while(character != CR){ // accepts until <enter> is typed
// The next line checks that the input is a digit, 0-9.
// If the character is not 0-9, it is ignored and not echoed
    if((character>='0') && (character<='9')) 
		{
      number = 10*number+(character-'0');   // this line overflows if above 4294967295
      length++;
      TX_UART(character);
    }
// If the input is a backspace, then the return number is
// changed and a backspace is outputted to the screen
    else if((character==BS) && length)
		{
      number /= 10;
      length--;
      TX_UART(character);
    }
    character = RX_UART();
  }
	
  return number;
}
