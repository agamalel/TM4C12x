
#include "tm4c123gh6pm.h"
#include "SSI_Nokia5110.h"

#define RES   						(*( (volatile unsigned long *) 0x40004200 )) // PA7
#define DC    						(*( (volatile unsigned long *) 0x40004100 )) // PA6
#define BSY								0x00000010 // Busy bit
#define TNF								0x00000002
#define NO_OF_LINES			( ( 84 * 48) / 8 )
#define MAX_X             84
#define MAX_Y             48
#define DC_COMMAND				0
#define DC_DATA						0x40

	void Init_SSI(void)
{
// PortA initialization
	SYSCTL_RCGC2_R|= SYSCTL_RCGC2_GPIOA;
	SYSCTL_RCGC1_R|= SYSCTL_RCGC1_SSI0; // Enable clock for SSI0
	GPIO_PORTA_AMSEL_R&= 0x00;
  GPIO_PORTA_AFSEL_R|= 0x2C; // PA2-PA5 SSI
	GPIO_PORTA_PCTL_R= (GPIO_PORTA_PCTL_R & ~0x00FFFF00) + 0x00222200;
	GPIO_PORTA_DEN_R|= 0xEC;
	GPIO_PORTA_DIR_R= (GPIO_PORTA_DIR_R & ~0x30) + 0xE0; // PA6, PA7 output, PA4 receive, PA5 Transmit
// SSI Initialization
	SSI0_CR1_R&= ~0x02; // Disable SSI during configuration
	SSI0_CR1_R&= 0; // Master mode
	SSI0_CC_R&= 0; // SSICLK source is the SYSCLCK
	// configure Baud Rate bits to 4 MHz
	SSI0_CPSR_R= 2; // CPSDVSR = 2
  SSI0_CR0_R= 0x0900; // SCR = 9 for 4 MHz SSI Baud Rate
	SSI0_CR0_R|= 0x0007; // 8-bit data size
	SSI0_CR0_R&= ~0x00F0; // Freescale SPI, SPO = SPH = 0
	SSI0_CR1_R|= 0x02; // Enable the SSI
}

void Nokia5110_Init(void)
{
	unsigned long delay;

	Init_SSI();
	RES= 0; // reset LCD to a known state
	for(delay= 10; 0 != delay; --delay) {}; // about 100ns delay (Minimum)
	RES= 0x80; // negative logic (Active low)
  Nokia5110_Write(command, 0x21);  // chip active PD = 0, V = 0 (horizontal addressing mode), H = 1 (extended instruction set)
	Nokia5110_Write(command, 0xB1);  // 3.3 Volt red SparkFun
  Nokia5110_Write(command, 0x04);  // set temp Coefficient
	Nokia5110_Write(command, 0x14);  // set 1:48 Bias	 		
	Nokia5110_Write(command, 0x20);  // we must send 0x20 before change the display control mode, H= 0 (normal instruction set)
  Nokia5110_Write(command, 0x0C);  // Normal Mode	
	Nokia5110_Clear();
}


void Nokia5110_Write(enum writetype type, unsigned char byte)
{
	if(command == type)
	{
		while( BSY == (SSI0_SR_R & BSY) ) {}; 
		DC= DC_COMMAND;
		SSI0_DR_R= byte;
		while( BSY == (SSI0_SR_R & BSY) ) {}; 
	}
	else
	{
		while( !TNF == (SSI0_SR_R & TNF) ) {}; // wait until transmit FIFO have a place
		DC= DC_DATA;
		SSI0_DR_R= byte;
	}
}	

void Nokia5110_SetCursor(unsigned long X, unsigned long Y)
{
	if(X > 11 || Y > 5)   // if bad input
		return;
	
	else
	{
		Nokia5110_Write(command, (X | 0x80)); // Set DB7
		Nokia5110_Write(command, (Y | 0x40)); // Set DB6
	}
}

void Nokia5110_PrintChar(unsigned char Ch)
{
	long i;
	
	Nokia5110_Write(data, 0x00); // leave a blank column
	for(i= 0; i < 5; ++i) 
		Nokia5110_Write(data, ASCII[Ch - 0x20][i]);
	
	Nokia5110_Write(data, 0x00);
}

void Nokia5110_Clear(void)
{
	long i;
// Write 0s in all pixels
	for (i = 0; i < NO_OF_LINES; ++i)
		Nokia5110_Write(data, 0x00); 
	
	Nokia5110_SetCursor(0, 0); // reset the Cursor
}

void Nokia5110_PrintString(char *pSTR)
{
	while(*pSTR) 
	{
		Nokia5110_PrintChar((unsigned char)*pSTR);
	  ++pSTR;
	}
}

// Prints a number as long as (2^31) - 1 in both negative and positive forms
// i have develpoed the mode technique to take the advantage of positive variable ranges
// calculations is the same with negative and positive
// when using Nokia5110_PrintNum(mode, Number);
// You must use the NEGATIVENUM macro in order to print a negative number
// POSITIVENUM in order to print a positive number
// other wise the function will not print anything
void Nokia5110_PrintNum(unsigned long mode, unsigned long number)
{
	  static unsigned long flag= 0;
		
		if(POSITIVENUM == mode)
		{
				while (number > 10)
				{
						Nokia5110_PrintNum(POSITIVENUM, (number / 10));
						number%= 10;
				}
				Nokia5110_PrintChar((number | 0x30));
		}
		else if (NEGATIVENUM == mode)
		{
				if (!flag)
				{
						Nokia5110_PrintChar('-');
						flag= 1;
				}
				while (number > 10)
				{
						Nokia5110_PrintNum(NEGATIVENUM, (number / 10));
						number%= 10;
				}
				Nokia5110_PrintChar((number | 0x30));
		}
}
