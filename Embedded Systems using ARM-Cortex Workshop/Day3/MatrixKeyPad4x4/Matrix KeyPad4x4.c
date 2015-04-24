// 4x4 Keypad interface module
// by Mo'meN Ali
// 23 August, 2014
// it uses the mazidi's KeyPad searching algorithm
// with enhancements 
// it maintains rollovers

// RB0-RB3 cols(output), RB4-RB7 rows (input)
// RB0-3 should be connected to the 4 pins of the Matrix from left to right (keypad's face up)
// RB4-7 should be connected the remaining pins starting from the left too (keypad's face up)

#include "stdint.h"
#include "PLL.h"
#include "SSI_Nokia5110.h"
#include "tm4c123gh6pm.h"

#define ROWS			4
#define COLS			4
#define LOCK_KEY	0x4C4F434B	

const char keyPad[ROWS][COLS]= {
	{'1', '2', '3', 'A'},
	{'4', '5', '6', 'B'},
	{'7', '8', '9', 'C'},
	{'*', '0', '#', 'D'}
};

void SysTick_Init(unsigned int period);
void MatrixKeyPad_Init(void);
void DisableInterrupts(void);
void EnableInterrupts(void);
void PortB_Handler(void);
void SysTick_Handler(void);
void WaitForInterrupt(void);
char MatrixScan(uint32_t *);

typedef struct MAILBOX {
		uint32_t Semaphore;
	  unsigned char	KeyPressed;
} mailbox;

mailbox MailBox;
char static lastKey;
int main(void)
{
	unsigned int period= 1440000;   	
	
	DisableInterrupts();
	PLL_Init();
	Nokia5110_Init();
	MatrixKeyPad_Init();
	SysTick_Init(period);   // 18ms 
	EnableInterrupts();
	
	while(1) 
	{	
			WaitForInterrupt();	
			while (0 == MailBox.Semaphore) { }	// no key pressed yet
			MailBox.Semaphore= 0;
			if (0 != MailBox.KeyPressed)		// block the flogs
					Nokia5110_OutChar(MailBox.KeyPressed);
	}
}

// First rows and columns are assumption based 
// rows and columns (keys) are not connected only the pushed button connects a joint between a row and a column
// Here low nibble is rows, High nibble is columns
// the concept is to set all bits to the same value (0xF) and check which bit changed which indicates to the rows(here)
// then change each column and see which row got effected
// The keys will be reported starting from the second Interrupt
void SysTick_Handler(void)
{
	  uint32_t thisKey= 0, keyFound= 0;
	
		thisKey= MatrixScan(&keyFound);
		if (lastKey)		// This will be the first Pressed Key
		{
				MailBox.Semaphore= 1;
				MailBox.KeyPressed= lastKey;
				lastKey= 0;
		}
		if ( (thisKey != lastKey) && (1 == keyFound) )    // The rollover key
		{
				MailBox.Semaphore= 1;
				MailBox.KeyPressed= lastKey;
				lastKey = thisKey;
		}
}

char MatrixScan(uint32_t *keyFound)
{
		unsigned char row, Key, savePress;   // save the port states when push
		*keyFound= row= Key= savePress= 0;    
		
		savePress= (GPIO_PORTB_DATA_R & 0x0F) ^ 0x0F; // save the pressed button state and flip it
	
		// search the effected column
		while(0 != savePress)
		{
				++row;
				savePress>>= 1;
		}
		// search for each columm
		GPIO_PORTB_DATA_R= 0xE0;
		if( 0x0F != (GPIO_PORTB_DATA_R & 0x0F) ) 
		{
				Key= keyPad[row - 1][0];
				++(*keyFound);
		}
		GPIO_PORTB_DATA_R= 0xD0;
		if( 0x0F != (GPIO_PORTB_DATA_R & 0x0F) )
		{
				Key= keyPad[row - 1][1];
				++(*keyFound);
		}
		GPIO_PORTB_DATA_R= 0xB0;
		if( 0x0F != (GPIO_PORTB_DATA_R & 0x0F) ) 
		{
				Key= keyPad[row - 1][2];
				++(*keyFound);
		}
		GPIO_PORTB_DATA_R= 0x70;
		if( 0x0F != (GPIO_PORTB_DATA_R & 0x0F) ) 
		{
				Key= keyPad[row - 1][3];
				++(*keyFound);
		}
		GPIO_PORTB_DATA_R= 0x0F; // rows default input is high, columns defualt output low
		
		return (Key);
}

void MatrixKeyPad_Init(void)
{
	SYSCTL_RCGC2_R|= SYSCTL_RCGC2_GPIOB;
  while ( 0 == (SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_R1) ) { }
  // columns (RB0-RB3), rows (RB4-RB7)
	GPIO_PORTB_CR_R= 0x0F;      // commit to open I2C0
	GPIO_PORTB_PUR_R= 0x0F;			// pull-up on the input pins (rows)
	GPIO_PORTB_AMSEL_R&= 0x00;			// no analog
	GPIO_PORTB_AFSEL_R&= 0x00;			// no alternate function
	GPIO_PORTB_PCTL_R&= 0x00000000;
	GPIO_PORTB_DEN_R|= 0xFF;  
	GPIO_PORTB_DIR_R|= 0xF0;  // RB0-RB3 cols(output), RB4-RB7 rows (input) 
	GPIO_PORTB_DATA_R= 0x0F;  // rows default output is high
}

void SysTick_Init(unsigned int period)
{
	NVIC_ST_CTRL_R&= 0; 
	NVIC_ST_CURRENT_R= 0;
	NVIC_ST_RELOAD_R= period - 1;
	NVIC_PRI3_R= (NVIC_PRI3_R & 0x00FFFFFF) | 0x20000000;		// Priority 1
	NVIC_ST_CTRL_R|= 0x00000007; // Enable Clock, Interrupts, start SysTick Timer
}
