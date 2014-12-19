// Sound.c
// Runs on LM4F120 or TM4C123, 
// edX lab 13 
// Use the SysTick timer to request interrupts at a particular period.
// Daniel Valvano, Jonathan Valvano
// March 13, 2014
// This routine calls the 4-bit DAC

#include "Sound.h"
#include "DAC.h"
#include "..//tm4c123gh6pm.h"

unsigned long Pitch= 1;
unsigned char Index;
const unsigned char SineWave[32]= {
	8, 9, 11, 12, 13, 14, 14, 15, 15, 15, 14, 14, 13, 12, 
	11, 9, 8, 7, 5, 4, 3, 2, 2, 1, 1, 1, 2, 2, 3, 4, 5, 7
};

// **************Sound_Init*********************
// Initialize Systick periodic interrupts
// Also calls DAC_Init() to initialize DAC
// Input: none
// Output: none
void Sound_Init(void)
{
	DAC_Init();
	NVIC_ST_CTRL_R= 0;
	NVIC_ST_RELOAD_R= Pitch - 1;
	NVIC_ST_CURRENT_R= 0;
	NVIC_SYS_PRI3_R= (NVIC_SYS_PRI3_R & 0x00FFFFFF) | 0x20000000; // priority 1
	NVIC_ST_CTRL_R|= 0x00000007;
}

// **************Sound_Tone*********************
// Change Systick periodic interrupts to start sound output
// Input: interrupt period
//           Units of period are 12.5ns
//           Maximum is 2^24-1
//           Minimum is determined by length of ISR
// Output: none
void Sound_Tone(unsigned long Period)
{
 // this routine sets the RELOAD and starts SysTick
	Pitch= Period;
	NVIC_ST_RELOAD_R= Pitch - 1;
}

// **************Sound_Off*********************
// stop outputing to DAC
// Output: none
void Sound_Off(void)
{
 // this routine stops the sound output
	NVIC_ST_CURRENT_R= 0; // Sound off
	NVIC_ST_RELOAD_R= 0;
	DAC_Out(0);
}

// Interrupt service routine
// Executed every 12.5ns*(period)
void SysTick_Handler(void)
{
	GPIO_PORTF_DATA_R^= 0x0A; // Blue LED
	DAC_Out(SineWave[Index]);
  Index= (Index + 1) & 0x1F;
}
