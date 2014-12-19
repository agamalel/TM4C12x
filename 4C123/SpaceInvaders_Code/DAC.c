#include "Sound.h"
#include "tm4c123gh6pm.h"

#define SOUND_OUTPUT  (*((volatile unsigned long *) 0x400063C0))

// **************DAC_Init*********************
// Initialize 4-bit DAC 
// Input: digital array contains sound
// Output: Sound
void DAC_Init(void)
{
	unsigned long delay= 0;
	
	SYSCTL_RCGC2_R|= SYSCTL_RCGC2_GPIOC;
	GPIO_PORTC_LOCK_R= 0x4C4F434B;
	GPIO_PORTC_CR_R= 0xF0;
	GPIO_PORTC_AMSEL_R&= 0x0F;  // Disable analog function on PB0-PB3
	GPIO_PORTC_AFSEL_R&= 0x0F; 	// disable alternate function on PB0-PB3 
	GPIO_PORTC_DR8R_R|= 0xF0;   // Drive 8 mA out of PB0-PB3
	GPIO_PORTC_PCTL_R&= 0x0000FFFF;
	GPIO_PORTC_DIR_R|= 0xF0; 	  // PB0-PB3 out
	GPIO_PORTC_DEN_R|= 0xF0;	  // PB0-PB3 digital
}

// **************DAC_Out*********************
// output to DAC
// Input: 4-bit data, 0 to 15 
// Output: none
void DAC_Out(unsigned long data)
{
   GPIO_PORTC_DATA_R = data;
}
