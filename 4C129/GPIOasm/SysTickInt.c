// SysTick Device Driver 
// Runs on TM4C129
// By Mo'meN Ali
// September 3, 2014

#include <stdint.h>
#include "tm4c1294ncpdt.h"
#include "SysTickInt.h"


void SysTick_Init(uint32_t period)
{
	NVIC_ST_CTRL_R&= 0;  // Disable systick while configuring
	NVIC_ST_CURRENT_R= 0;  // any write to current clears it
	NVIC_ST_RELOAD_R= period;
	NVIC_PRI3_R= 0;					// priority 0
}
	
// This function is various amount of delay based of t0, t1
// t0 is the time of cycles of delay the maxmimum period for t0 is (ClockFreq / delayFreq)  
// at 120 MHz BusClockPeriod = 8.33 ns, with the restriction of 10^24 which is the wide of the register
// t1 is the how many times you want to repeat t0 amount of delay
// mixing them gives various amount of time delays;
/******** Example ********/
// delay for 30 seconds
// t0 = (ClkFreq / delayFreq) & 0x00FFFFFF or t0 * 8.33 ns = 1 sec, t0= 120,000,000 for 1 second delay
// for 30 sec delay, t1 = 30. 
void SysTick_Delay(uint32_t t0, uint32_t t1)
{	
	if(0 == t1)
	{
		NVIC_ST_RELOAD_R= (t0- 1) & 0x00FFFFFF;  // make sure it does not overflow
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

void SysTick_Start(void)
{
	NVIC_ST_CTRL_R|= 0x7; // Enable SysTick , main clock is the clock source , interrupts enable
}

void SysTick_Stop(void)
{
	NVIC_ST_CTRL_R&= 0;
}
