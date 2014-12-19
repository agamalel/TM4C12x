#include "PLL.h"
#include "tm4c123gh6pm.h"

void PLL_Init(void)
{
	SYSCTL_RCC2_R|= 0x80000000; // use RCC2 because it has more options
	SYSCTL_RCC2_R|= 0x00000800; // set bypass for configuring the PLL
	SYSCTL_RCC_R|= (SYSCTL_RCC_R & ~0x000007C0) + 0x00000540;  // clear XTAL bits, set XTAL to 16 MHz
	// clear PWRDN to enable PLL	select the main oscillator, set DIV400 for 400 MHz
	SYSCTL_RCC2_R= (SYSCTL_RCC2_R & ~0x0FFFFFFF) + 0x40000000; 
	SYSCTL_RCC2_R= (SYSCTL_RCC2_R & ~0x1FC00000) + (4 << 22); // clear SYSDIV, then set it to 5 = (n + 1) for 80 MHz
	while( 0 == (SYSCTL_RIS_R & 0x00000040) ) {}; // wait for PLL to stable	
	SYSCTL_RCC2_R&= ~0x00000800; // clear BYPASS to enable PLL
}
