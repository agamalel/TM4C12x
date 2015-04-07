// Tuning Fork
// This Project is Tunning Fork sine wave generator
// It uses PLL and SysTick Timer to produce a precise frequency on PA4
// Runs on TM4C123

#include "tm4c123gh6pm.h"
#include "PLL.h"
#include "SysTick.h"


void GPIOPortA_Init(void); 

int main(void)
{	
	PLL_Init();   				// 80 MHz
	GPIOPortA_Init();
	SysTick_Init();			
	while(1)
	{
		  GPIO_PORTA_DATA_R^= 0x80;
	    SysTick_Delay(90909, 0);			// Frequency = 880 Hz, Periodic Time = 2.27 ms 
	}
}


// PA5, PA6, PA7   Output Pins
void GPIOPortA_Init(void)
{
	volatile unsigned long delay;
	
	SYSCTL_RCGC2_R|= SYSCTL_RCGC2_GPIOA;
	while ( 0 == (SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_R0) ) { } 
	GPIO_PORTA_PCTL_R&= ~0xF0000000;
	GPIO_PORTA_AMSEL_R&= ~0x80;
  GPIO_PORTA_AFSEL_R&= ~0x80;
	GPIO_PORTA_DEN_R|= 0x80;					
	GPIO_PORTA_DIR_R|= 0x80;
}
