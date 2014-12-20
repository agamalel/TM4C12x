// Systick timer device driver
// by Mo'meN Ali
// September 8, 2014

// note: that for percise time delays, Enable the PLL & set the Main Clock Frequency to 80 MHz 

#include "stdint.h"

// Initialize SysTick Timer, main clock source, Interrupts Enable
void SysTick_Init(uint32_t period);

// This function is various amount of delay based of t0, t1
// t0 is the time of cycles of delay the maxmimum period for t0 is (ClockFreq / delayFreq)  
// at 80 MHz BusClockPeriod = 12.5 ns, with the restriction of 10^24 which is the wide of the register
// t1 is the how many times you want to repeat t0 amount of delay
// mixing them gives various amount of time delays;
/******** Example ********/
// delay for 30 seconds
// t0 = (ClkFreq / delayFreq) & 0x00FFFFFF or t0 * 12.5 ns = 1 sec, t0= 80,000,000 for 1 second delay
// for 30 sec delay, t1 = 30. 
void SysTick_Delay(uint32_t t0, uint32_t t1);
