// an ohmmeter with range of 10-260 kilo ohm
// Runs on TM4C129
// By Mo'meN M. Ali

#include <stdint.h>
#include "..//tm4c1294ncpdt.h"
#include "Timer.h"
#include "UART0.h"
#include "PLL.h"

// input Capture using Pin-PL4
// 555 IC circuit - Astable multivibrator
// resolution is 5 ohm

void EnableInterrupts(void);
void WaitForInterrupt(void);

uint32_t Resistance, Period;
uint32_t static  First;

int main(void)
{
		PLL_Init();			// 120MHz
		UART0_Init();
	  T0A_InputEdgeInit();
		UART0_TxString("Ready, Sir!");
		UART0_NewLine();
	  EnableInterrupts();
		while(1)
		{
			  UART0_TxUDec(Resistance);
				UART0_NewLine();
				WaitForInterrupt();
		}
}

void Timer0A_Handler(void)
{
	  TIMER0_ICR_R= TIMER_ICR_CAECINT;   // ack INT 
		Period= (First - TIMER0_TAR_R) & 0x0000FFFF;  //   8.33ns Resolution
		Resistance= ((152 * Period - 939295) >> 16) * 1000;  // Empirical calibration R input= 0.0023 * Period - 14.3325
		First= TIMER0_TAR_R;
}
