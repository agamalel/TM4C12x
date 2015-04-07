// SysTick Device Driver 
// Runs on TM4C129
// By Mo'meN Ali
// September 3, 2014

#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "SysTick.h"



// Period = SystemClock Frequency / delay frequency
void SysTick_Init(void)
{	
		NVIC_ST_CTRL_R&= 0;  	 // Disable systick while configuring
		NVIC_ST_CURRENT_R= 0;  // any write to current clears it
		NVIC_ST_RELOAD_R= 0;
		NVIC_ST_CTRL_R|= 0x5;	 // Enable SysTick, main clock source, no interrupts 
}

// this function also disables the interrupts on systick and re-enables it
// This function is various amount of delay based of t0, t1
// t0 is the time of cycles of delay the maxmimum period for t0 is (ClockFreq / delayFreq)  
// at 120 MHz BusClockPeriod = 8.33 ns, with the restriction of 10^24 which is the wide of the register
// t1 is the how many times you want to repeat t0 amount of delay
// mixing them gives various amount of time delays;
// This function does not use interrupts
/******** Example ********/
// delay for 30 seconds
// t0 = (ClkFreq / delayFreq) & 0x00FFFFFF or t0 * 12.5 ns = 1 sec, t0= 80,000,000 for 1 second delay
// for 30 sec delay, t1 = 30. 
void SysTick_Delay(uint32_t t0, uint32_t t1)
{		
		if(0 == t1)
		{
				NVIC_ST_RELOAD_R= (t0 - 1) & 0x00FFFFFF;  
				while( 0 == (NVIC_ST_CTRL_R & NVIC_ST_CTRL_COUNT) ) {}  // check count flag if set then SysTick reached 0
		}
		else
		{
				for( ; 0 != t1; --t1)
				{
						NVIC_ST_RELOAD_R= (t0- 1) & 0x00FFFFFF;  // make sure it does not overflow
						while( 0 == (NVIC_ST_CTRL_R & NVIC_ST_CTRL_COUNT) ) {}  // check count flag if set then SysTick reached 0
				}
		}
}

// starts the timer
void SysTick_Start(void)
{
		NVIC_ST_CTRL_R|= 0x5; // Enable SysTick , main clock is the clock source , interrupts enable
}

// stops the timer
void SysTick_Stop(void)
{
		NVIC_ST_CTRL_R&= 0;
}
