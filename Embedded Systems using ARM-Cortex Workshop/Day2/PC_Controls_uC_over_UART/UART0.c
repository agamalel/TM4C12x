// UART0 Device Driver 
// Runs on TM4C123
// By Mo'meN Ali
// August 27, 2014

/* Copyright by Mo'meN M. Ali, momen.1993@live.com
You may use, edit, run or distribute this file
as long as the above copyright notice remains
THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. */

// this connection occurs in the USB debugging cable
// U0Rx 17 PA0 (1) I TTL UART module 0 receive. 
// U0Tx 18 PA1 (1) O TTL UART module 0 transmit.
// Ground connected ground in the USB cable

#include "UART0.h"
#include "stdint.h"
#include "tm4c123gh6pm.h"


// UART0 connected on Debug USB
// Baude Rate = 115200
// initialize the UART0 for 115200 baud rate (assuming 16 Mhz PIOSC Clock)
// 8 bit frame length, parity bits, one stop bit, FIFOs enabled
// Interrupt Enabled
// PA0, PA1 
// the clock system divisor is 16 in low speed mode, 8 in high speed mode
// BRD = SysClk / (SysDiv * Desired Baudrate)
void UART0_Init(void)
{
		SYSCTL_RCGCGPIO_R|= SYSCTL_RCGCGPIO_R0;
		SYSCTL_RCGCUART_R|= SYSCTL_RCGCUART_R0;
		
		GPIO_PORTA_AMSEL_R&= ~0x03;
		GPIO_PORTA_PCTL_R= (GPIO_PORTA_PCTL_R & ~0x000000FF) | 0x00000011;
		GPIO_PORTA_AFSEL_R|= 0x03;
		GPIO_PORTA_DEN_R|= 0x03;
  
		UART0_CTL_R&= ~UART_CTL_UARTEN;	
		UART0_IBRD_R= 43;										// Baud Rate = 115200
		UART0_FBRD_R= 26;										
		UART0_LCRH_R|= UART_LCRH_WLEN_8 | UART_LCRH_FEN;		  // 8 bit word length, FIFOs Enabled, one stop bit, no parity bits
		UART0_IFLS_R|= UART_IFLS_TX1_8  | UART_IFLS_RX4_8;    // trigger Transmit when 1/8 of Tx FIFO is ready
																												  // trigger receive when 1/4 or less of Rx FIFO is empty
		UART0_CC_R&= ~0xF;  // Alternate Clock source, as define by the ALTCLKCFG register
		UART0_CTL_R|= UART_CTL_UARTEN;
	  UART0_CTL_R&= ~UART_CTL_HSE;												 // low speed disabled, divide clock by 16	
}


void UART0_TxChar(char Ch)
{
		while (UART0_FR_R & UART_FR_TXFF) { } // wait untill transmitter is not full
		UART0_DR_R= Ch;		
}

void UART0_RxChar(char *data)
{
		while (UART0_FR_R & UART_FR_RXFE) { }  // wait until the receiver receives data
		*data= UART0_DR_R; 		
}

char UART0_GetOneChar(void)
{
		if (0 != (UART0_FR_R & UART_FR_RXFE))			// if there's a character in FIFO, get it
				return ((unsigned char)UART0_DR_R);

		else 
			return 0;
}

void UART0_TxString(char *String)
{
		while (*String)
		{
				UART0_TxChar(*String);
				++String;
		}
}

void UART0_RxString(char *string, uint16_t strLen)
{
		char Ch= 0;
		
    UART0_RxChar(&Ch);	
		while (CR != Ch)
		{		
				if ( (0 != strLen) && BS != Ch ) // if normal character save it and count it
				{
						*string= Ch;
					  ++string;
						--strLen;
				}
				else														// else should be a backspace, so delete it and de-count it
				{
						++strLen;		
					  --string;   // delete the character
				}
				UART0_TxChar(Ch);
				UART0_RxChar(&Ch);  // get next character
		}
		*string= '\0';  // string null terminator
}

//-----------------------UART_TxUDec-----------------------
// Output a 32-bit number in unsigned decimal format
// Input: 32-bit number to be transferred
// Output: none
// Variable format 1-10 digits with no space before or after
void UART0_TxUNum(uint32_t UDecNum)
{	
	  while (UDecNum > 10) 
		{
				UART0_TxUNum(UDecNum / 10);
			  UDecNum= UDecNum % 10;
		}
		UART0_TxChar(UDecNum | 0x30);
}

//------------UART0_RxUDec------------
// RxUDec() accepts ASCII input in unsigned decimal format
//     and converts to a 32-bit unsigned number
//     valid range is 0 to 4294967295 (2^32-1)
// Input: none
// Output: 32-bit unsigned number
// If you enter a number above 4294967295, it will return an incorrect value
// Backspace will remove last digit typed
uint32_t UART0_RxUNum(void)
{
	  uint32_t decNum= 0;
	  char Ch= 0;
	  
	  UART0_RxChar(&Ch);
		while (CR != Ch) 
		{
				if (Ch >= '0' && Ch <= '9')
						decNum= decNum * 10 + (Ch ^ 0x30);  
				
				else if ( (BS == Ch) && decNum )
						decNum/= 10;   //	delete this digit
						
				UART0_TxChar(Ch);		// echo the current character
				UART0_RxChar(&Ch);  // receive the next character
		}
		return (decNum);
}
	
// make new line
void UART0_NewLine(void)
{
		UART0_TxChar(CR);
		UART0_TxChar(LF);
}
