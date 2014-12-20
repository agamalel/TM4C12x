// UART0 Device Driver 
// Runs on TM4C129
// By Mo'meN Ali
// August 27, 2014

// this connection occurs in the USB debugging cable
// U0Rx 17 PA0 (1) I TTL UART module 0 receive. 
// U0Tx 18 PA1 (1) O TTL UART module 0 transmit.
// Ground connected ground in the USB cable

#include "UART.h"
#include "stdint.h"
#include "tm4c1294ncpdt.h"

#define NVIC_EN0_INT5						 0x5		
#define SYSCTL_RCGC_GPIOA	       0x1
#define SYSCTL_RCGCUART0				 0x1
#define UART_LCRH_WLEN_8BIT		  0x60
#define SYSCTL_ALTCLKCFG_PIOSC	 0x0
#define UART_CC_PIOSC					 	 0x5
#define UART_IFLS_4RXIE			  	0x10    // 1/4 Full
#define UART_IFLS_2TXIE					 0x2


// initialize the UART0 for 115200 baud rate (assuming 16 Mhz PIOSC Clock)
// 8 bit frame length, parity bits, one stop bit, FIFOs enabled
// Interrupt Enabled
void UART0_Init(void)
{
	SYSCTL_RCGCGPIO_R|= SYSCTL_RCGCGPIO_R0;
	SYSCTL_RCGCUART_R|= SYSCTL_RCGCUART_R0;

	while ( 0 == (SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_R0) ) { }
	GPIO_PORTA_AMSEL_R&= ~0x03;
	GPIO_PORTA_PCTL_R= (GPIO_PORTA_PCTL_R & ~0x000000FF) | 0x00000011;
	GPIO_PORTA_AFSEL_R|= 0x03;
	GPIO_PORTA_DEN_R|= 0x03;
  
  while( 0 == (SYSCTL_PRUART_R & SYSCTL_PRUART_R0) ) { }
  UART0_CTL_R&= ~UART_CTL_UARTEN;	
  UART0_IBRD_R= 8;
  UART0_FBRD_R= 44;
  UART0_LCRH_R|= UART_LCRH_WLEN_8;
  UART0_IFLS_R += (UART_IFLS_TX1_8|UART_IFLS_RX1_8);
	UART0_CC_R= (UART0_CC_R & ~0xF) | UART_CC_CS_PIOSC;
	UART0_CTL_R|= UART_CTL_UARTEN;	
}

void UART0_TxChar(int8_t Ch)
{
	while(UART0_FR_R & UART_FR_TXFF) { } // wait untill transmitter is not full
	
	UART0_DR_R= Ch;	
}

void UART0_RxChar(uint8_t *data)
{
	while(UART0_FR_R & UART_FR_RXFE) { }  // wait until the receiver receives data
	 
 	*data= UART0_DR_R;	
}

// Output String (NULL termination)
// Input: pointer to a string to be transferred
void UART0_TxString(int8_t *String)
{
	while(*String)
	{
		UART0_TxChar(*String);
		++String;
	}
}

//-----------------------UART_TxUDec-----------------------
// Output a 32-bit number in unsigned decimal format
// Input: 32-bit number to be transferred
// Output: none
// Variable format 1-10 digits with no space before or after
void UART_TxUDec(uint32_t UDecNum)
{	
	uint32_t Ch[10]= {0};
	
	if(0 == UDecNum)
	{
		UART0_TxChar('0');
	}
	else 
	{
		int32_t i;
		for(i= 0; (i < 10) && (0 != UDecNum); ++i)
		{
				Ch[i]= (UDecNum % 10) | 0x30;   // take a number and transfer it into a character form
				UDecNum/= 10;		
		}
		for(; -1 < i ; --i)
		{
			UART0_TxChar( (uint8_t)Ch[i] );  
		}
	}
}

//--------------------------UART0_TxUHex----------------------------
// Output a 32-bit number in unsigned hexadecimal format
// Input: 32-bit number to be transferred
// Output: none
// Variable format 1 to 8 digits with no space before or after
void UART0_TxUHex(int32_t UHexNum)
{
	uint32_t UHexCh[8]= {0};
	
	if(0 == UHexNum)
	{
		UART0_TxChar('0');
	}
	else
	{
		int32_t i;
		for(i= 0; (i < 8) && (0 != UHexNum); ++i)
		{
			if(UHexNum < 0xA)
			{
				UHexCh[i]= (UHexNum % 0x10) | 0x30;
				UHexNum/= 10;
			}
			else 
			{
				UHexCh[i]=  (UHexNum % 0xA) + 'A';
				UHexNum/= 0xA;
			}
			for(; -1 < i ; --i)
			{
				UART0_TxChar( (uint8_t)UHexCh[i] );  
			}
		}
	}
}

/*char data;
void UART0_Handler(void)
{
	while(UART0_RIS_R & UART_RIS_TXRIS)
	{
		//data= FIFO_Tx();
		if(-1 == data)  // if no data in the fifo
		{
			UART0_ICR_R= UART_ICR_TXIC;  // clear the flag
			UART0_IM_R&= ~UART_IM_TXIM;  // disable itnerrupts
		}
		UART_TxChar(data);
	}

}*/
