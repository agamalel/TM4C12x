// This code runs on TM4C123
// using Timer0PWM 
// Timer0APWM for right wheel PB6
// Timer0BPWM for Left  wheel PB7
// By Mo'meN Ali

// PWM0	PB6
// PWM1	PB7

#include "tm4c123gh6pm.h"
#include "Timer0PWM.h"
#include "UART0.h"

void Timer0APWM_Init(unsigned short Period, unsigned short High)
{
	SYSCTL_RCGC2_R|= SYSCTL_RCGC2_GPIOB; // PortB clock
	SYSCTL_RCGCTIMER_R|= SYSCTL_RCGCTIMER_R0; // Timer0 clock
	// PortB Init
	GPIO_PORTB_AMSEL_R&= ~0x40; 	// Disable Analog on PB6
	GPIO_PORTB_AFSEL_R|= 0x40;  	// Enable Alternate function
	GPIO_PORTB_PCTL_R= (GPIO_PORTB_PCTL_R & ~0x0F000000) | 0x07000000;
	GPIO_PORTB_DEN_R|= 0x40;    	// Enable Digital
	//Timer0A 16-Bit mode Init
	TIMER0_CTL_R&= ~TIMER_CTL_TAEN;   // Disalbe Timer0A while configuring
	TIMER0_CFG_R|= TIMER_CFG_16_BIT;  // 16-Bit Mode
	TIMER0_TAMR_R|= 0x0000000A; 			// Periodic, PWM Enabled
	TIMER0_TAILR_R= Period - 1; 			// Square wave Period
	TIMER0_TAMATCHR_R= Period - High - 1;  // Duty Cycle, the match Register works as the low period of the pulse
	TIMER0_CTL_R|= TIMER_CTL_TAEN;  // Enable/Start Timer0A
}

void Timer0APWM_DutyCycle(unsigned short High)
{
	TIMER0_TAMATCHR_R= TIMER0_TAILR_R - High;   // change the low period of the square wave 
}
