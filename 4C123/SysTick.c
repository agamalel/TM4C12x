// Systick timer device driver
// by Mo'meN Ali
// September 8, 2014

// note: that for percise time delays, Enable the PLL & set the Main Clock Frequency to 80 MHz 

#include "tm4c123gh6pm.h"
#include "stdint.h"
#include "SysTick.h"


// Initialize SysTick Timer, main clock source, Interrupts Enable
void SysTick_Init(uint32_t period)
{
	NVIC_ST_CTRL_R&= 0; 
	NVIC_ST_CURRENT_R= 0;
	NVIC_ST_RELOAD_R= period - 1;
	NVIC_PRI3_R= (NVIC_PRI3_R & 0x00FFFFFF) | 0x20000000;		// Priority 1
	NVIC_ST_CTRL_R|= 0x00000007; // Enable Clock, Interrupts, start SysTick Timer
}


// This function is various amount of delay based of t0, t1
// t0 is the time of cycles of delay the maxmimum period for t0 is (ClockFreq / delayFreq)  
// at 80 MHz BusClockPeriod = 12.5 ns, with the restriction of 10^24 which is the wide of the register
// t1 is the how many times you want to repeat t0 amount of delay
// mixing them gives various amount of time delays;
/******** Example ********/
// delay for 30 seconds
// t0 = (ClkFreq / delayFreq) & 0x00FFFFFF or t0 * 12.5 ns = 1 sec, t0= 80,000,000 for 1 second delay
// for 30 sec delay, t1 = 30. 
void SysTick_Delay(uint32_t t0, uint32_t t1)
{
	uint32_t i;
		
	if(0 == t1)
	{
		i= 1;
	}
	else
	{
		i= t1;
	}
		
	for( ; 0 != i; --i)
	{
		NVIC_ST_RELOAD_R= (t0- 1) & 0x00FFFFFF;  // make sure it does not overflow
		while( 0 == (NVIC_ST_CTRL_R & NVIC_ST_CTRL_COUNT) ) {}  // check count flag if set then SysTick reached 0
	}
}
