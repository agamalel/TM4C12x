// This code runs on TM4C123
// using Timer0PWM 
// Timer0APWM for right wheel PB6
// Timer0BPWM for Left Wheel
// By Mo'meN Ali

// PWM0	PB6
// PWM1	PB7

#include "Timer0PWM.h"

#define GPIO_PORTB_AFSEL_R      (*((volatile unsigned long *)0x40005420))
#define GPIO_PORTB_DEN_R        (*((volatile unsigned long *)0x4000551C))
#define GPIO_PORTB_AMSEL_R      (*((volatile unsigned long *)0x40005528))
#define GPIO_PORTB_PCTL_R       (*((volatile unsigned long *)0x4000552C))
#define SYSCTL_RCGC1_R          (*((volatile unsigned long *)0x400FE104))
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))
#define SYSCTL_RCGC1_TIMER0     0x00010000  // timer 0 Clock Gating Control
#define SYSCTL_RCGC2_GPIOB      0x00000002  // port B Clock Gating Control
#define TIMER0_CFG_R            (*((volatile unsigned long *)0x40030000))
#define TIMER0_TAMR_R           (*((volatile unsigned long *)0x40030004))
#define TIMER0_TBMR_R           (*((volatile unsigned long *)0x40030008))	
#define TIMER0_CTL_R            (*((volatile unsigned long *)0x4003000C))
#define TIMER0_TAILR_R          (*((volatile unsigned long *)0x40030028))
#define TIMER0_TBILR_R          (*((volatile unsigned long *)0x4003002C))
#define TIMER0_TAMATCHR_R       (*((volatile unsigned long *)0x40030030))
#define TIMER0_TAMBTCHR_R       (*((volatile unsigned long *)0x40030034))
#define TIMER_CTL_TAEN          0x00000001  // GPTM TimerA Enable
#define TIMER_TnMR_PERIODIC			0x00000002  // Priodic mode
#define TIMER_CFG_16_BIT        0x00000004  // 16-bit timer configuration
#define TIMER_TnMR_TnCMR				0x0					// Edge Count Mode
#define TIMER_TnMR_TnAMS				0x00000008  // PWM mode enable
#define TIMER_CTL_TBPWML				0x0					// PWM output is unaffected
#define TIMER_CTL_TBEN					0x00000100  // TimerB enable

void Timer0APWM_Init(unsigned short Period, unsigned short High)
{
	SYSCTL_RCGC2_R|= SYSCTL_RCGC2_GPIOB; // PortB clock
	SYSCTL_RCGC1_R|= SYSCTL_RCGC1_TIMER0; // Timer0 clock
	// PortB Init
	GPIO_PORTB_AMSEL_R&= ~0x40; 	// Disable Analog on PB6
	GPIO_PORTB_AFSEL_R|= 0x40;  	// Enable Alternate function
	GPIO_PORTB_PCTL_R= (GPIO_PORTB_PCTL_R & ~0x0F000000) | 0x07000000;
	GPIO_PORTB_DEN_R|= 0x40;    	// Enable Digital
	//Timer0A 16-Bit mode Init
	TIMER0_CTL_R&= ~TIMER_CTL_TAEN;   // Disalbe Timer0A while configuring
	TIMER0_CFG_R|= TIMER_CFG_16_BIT;  // 16-Bit Mode
	TIMER0_TAMR_R|= TIMER_TnMR_TnCMR | TIMER_TnMR_TnAMS | TIMER_TnMR_PERIODIC; // Periodic, PWM Enabled, Edge Count mode
	TIMER0_TAILR_R= Period - 1; 			// Square wave Period
	TIMER0_TAMATCHR_R= Period - High - 1;  // Duty Cycle, the match Register works as the low period of the pulse
	TIMER0_CTL_R|= TIMER_CTL_TAEN;  // Enable/Start Timer0A
}

void Timer0APWM_DutyCycle(unsigned short High)
{
	TIMER0_TAMATCHR_R= TIMER0_TAILR_R - High;   // change the low period of the square wave 
}

void Timer0BPWM_Init(unsigned short Period, unsigned short High)
{
	// PB7 Init
	// PORTB already has clock from first wheel init
	GPIO_PORTB_AMSEL_R&= 0x80; 		//Disable analog on PB7
	GPIO_PORTB_AFSEL_R|= 0X80;    // Enable alternate function
	GPIO_PORTB_PCTL_R= (GPIO_PORTB_PCTL_R & ~0xF0000000) | 0x70000000;
	GPIO_PORTB_DEN_R|= 0x80; 			// Enable Digital on PB7
	// Timer Init
	// Timer alreayd has clock from the Timer0APWM_LeftWheelInit(Period, High)
	TIMER0_CTL_R&= ~TIMER_CTL_TBEN;  // Disable Timer0B while configuring
	TIMER0_CFG_R|= TIMER_CFG_16_BIT; // 16 Bit
	TIMER0_TBMR_R|= TIMER_TnMR_TnCMR | TIMER_TnMR_PERIODIC | TIMER_TnMR_TnAMS; // Periodic, PWM Enabled, Edge Count mode
	TIMER0_TBILR_R= Period;    // Square wave width
	TIMER0_TAMBTCHR_R= Period - High - 1;   // Duty Cycl, value in match register is considered as the low period
	TIMER0_CTL_R|= TIMER_CTL_TBEN; // Timer0B Enable
}

void Timer0BPWM_DutyCycle(unsigned short High)
{
	TIMER0_TAMBTCHR_R= TIMER0_TBILR_R - High;  
}
