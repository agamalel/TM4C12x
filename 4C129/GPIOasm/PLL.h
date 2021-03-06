// PLL configuration
// TM4C129
// By Mo'meN Ali
// August 31, 2014

/* Copyright by Mo'meN M. Ali, momen.1993@live.com
You may use, edit, run or distribute this file
as long as the above copyright notice remains
THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. */

#include <stdint.h>

// SysClk = fVCO/ (PSYSDIV + 1
// The following are macro to set the PLL Frequency as required, assuming Fvco = 480 MHz
// change the PSYSDIV according to the following table to set a different frequency
#define PSYSDIV			3


/*
System Divisor Factors
------------------------------|---------------------------------|
|	fVCO (MHz)= 480 MHz             |
------------------------------|---------------------------------|
System Clock (SYSCLK) (MHz)	|	System Divisors (PSYSDIV +1)a   |
------------------------------|---------------------------------|
120 						|					4												|
60	 						|				  8												|
48 							|					10											|
30 							|					16											|
24 							|					20											|
12	 						|					40											|
6 							|					80											|
------------------------------|---------------------------------|
*/


/*
-------------|----------------|
PSYSDIV  | SysClk (Hz) 		|
-------------|----------------|
3      |   120,000,000	|
4      |	 96,000,000		|
5      |	 80,000,000		|
7      |	 60,000,000		|
9      |	 48,000,000		|
15     |  30,000,000		|
19     |	 24,000,000		|
29     |	 16,000,000		|
39     |	 12,000,000		|
79     | 	 6,000,000		|
------------------------------|
*/

void PLL_Init(void);
