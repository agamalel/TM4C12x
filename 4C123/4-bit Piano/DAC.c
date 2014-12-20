#include "DAC.h"
#include "tm4c123gh6pm.h"

#define SOUND_OUTPUT  (*((volatile unsigned long *) 0x400063C0))

// **************DAC_Init*********************
// Initialize 4-bit DAC 
// Input: digital array contains sound
// Output: Sound
void DAC_Init(void)
{
	unsigned long delay;
	
	SYSCTL_RCGC2_R|= SYSCTL_RCGC2_GPIOB;
	delay= SYSCTL_RCGC2_R;
	GPIO_PORTB_PCTL_R&= 0xFFFF0000;
	GPIO_PORTB_AMSEL_R&= 0xF0;  
	GPIO_PORTB_AFSEL_R&= 0xF0; 	 
	GPIO_PORTB_DR8R_R|= 0x0F;  
	GPIO_PORTB_DIR_R|= 0x0F; 	  
	GPIO_PORTB_DEN_R|= 0x0F;	  
}

// **************DAC_Out*********************
// output to DAC
// Input: 4-bit data, 0 to 15 
// Output: none
void DAC_Out(unsigned long data)
{
   GPIO_PORTB_DATA_R= data;
}
