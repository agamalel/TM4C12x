// 4x4 Keypad interface module
// by Mo'meN Ali
// 23 August, 2014
// it uses the mazidi's KeyPad itnerfacing approach

// RB0-RB3 cols(output), RB4-RB7 rows (input) 

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
void MatrixKeyPad_Int(void);
void DisableInterrupts(void);
void EnableInterrupts(void);
void PortB_Handler(void);
void SysTick_Handler(void);
void WaitForInterrupt(void);

int main(void)
{
	unsigned int period= 1600000;   	
	
	DisableInterrupts();
	PLL_Init();
	Nokia5110_Init();
	MatrixKeyPad_Int();
	SysTick_Init(period);
	EnableInterrupts();
	Nokia5110_OutString("Press a Key!");

	while(1) 
	{	
		WaitForInterrupt();			
	}
}

// First rows and columns are assumption based 
// rows and columns (keys) are not connected only the pushed button connects a joint between a row and a column
// Here low nibble is rows, High nibble is columns
// the concept is to set all bits to the same value (0xF) and check which bit changed which indicates to the rows(here)
// then change each column and see which row got effected
void SysTick_Handler(void)
{
	unsigned char savePress= 0;   // save the port states when push
	unsigned char row= 0;      
		
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
		//key= keyPad[0][column];
		Nokia5110_SetCursor(1, 2);
		Nokia5110_OutChar(keyPad[row - 1][0]);
	}
	GPIO_PORTB_DATA_R= 0xD0;
	if( 0x0F != (GPIO_PORTB_DATA_R & 0x0F) )
	{
		Nokia5110_SetCursor(1, 2);
		Nokia5110_OutChar(keyPad[row - 1][1]);
	}
	GPIO_PORTB_DATA_R= 0xB0;
	if( 0x0F != (GPIO_PORTB_DATA_R & 0x0F) )
	{
		Nokia5110_SetCursor(1, 2);
		Nokia5110_OutChar(keyPad[row - 1][2]);
	}
	GPIO_PORTB_DATA_R= 0x70;
	if( 0x0F != (GPIO_PORTB_DATA_R & 0x0F) )
	{
	  Nokia5110_SetCursor(1, 2);
		Nokia5110_OutChar(keyPad[row - 1][3]);
	}
	GPIO_PORTB_DATA_R= 0x0F; // rows default input is high, columns defualt output low
}

void MatrixKeyPad_Int(void)
{
	unsigned long delay;
	
	SYSCTL_RCGC2_R|= SYSCTL_RCGC2_GPIOB;
  delay= SYSCTL_RCGC2_R;
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
