// Simple Line Follower
// By Mo'meN Ali

#include "tm4c123gh6pm.h"
#include "PLL.h"
#include "Timer0PWM.h"
#include "UART0.h"
#include "SysTick.h"

#define PF4											(GPIO_PORTF_RIS_R & 0x10)		 // read SW1 state
#define PF0											(GPIO_PORTF_RIS_R & 0x01)		 // read SW1 state

void WaitForInterrupt(void);
void EnableInterrupts(void);
void DisableInterrupts(void);
void GPIOPortF_EdgeTriggerSW(void);
	
unsigned long High= 500, Low; // this is the high cycle variable 

int main(void)
{		
	PLL_Init();			 //	Enable the PLL Block and set the main clock frequency to 80 MHz
	GPIOPortF_EdgeTriggerSW();  // Init Edge Interrupt on high Edge on two switches PF0, PF4 
	UART0_Init();
	SysTickInts_Init(8000000);		// 100ms
	// Timer Period = Clock / Period
	// Period = 1 / ClockFrequency
	// for 80Khz, Period= 80MHz / 80KHz = 1000
	// start with 50% DutyCycle on both wheels
	Timer0APWM_Init(1000, High); // 80 KHz, 50% Duty Cycle 
	
	EnableInterrupts();
	
	UART0_TxString("Ready, Sir");
	UART0_NewLine();
	while(1) 
	{
			WaitForInterrupt(); // low power mode
	}
}

void SysTick_Handler(void)
{	
		UART0_TxString("Motor has ");
		UART0_TxUNum(High / 10);
		UART0_TxString("% DutyCycle");
	  UART0_NewLine();
}

// initialize 2 swithces PF0, PF4 
// using Edge Interrupts, on high Pulse
void GPIOPortF_EdgeTriggerSW(void)
{
		// PortF Configuration	
		SYSCTL_RCGC2_R|= SYSCTL_RCGC2_GPIOF;
		while ( 0 == (SYSCTL_PRGPIO_R & SYSCTL_PRGPIO_R5) ) { }
		GPIO_PORTF_LOCK_R= 0x4C4F434B; // unlock GPIOF
		GPIO_PORTF_CR_R|= 0x1F; // Enable for PF0 & PF4
		GPIO_PORTF_PCTL_R&= ~0x000FFFFF;  // will use digital
		GPIO_PORTF_AMSEL_R&= ~0x17; // no analog
		GPIO_PORTF_AFSEL_R&= ~0x17; // no AF
		GPIO_PORTF_DEN_R|= 0x17; // 2 switches, RGB LED
		GPIO_PORTF_PUR_R|= 0x11; // pull-up the 2 switches (negative logic)
		GPIO_PORTF_DIR_R|= ~0x11; // input switches, output RGB LED
		// PortF Interrupt Configuration
		GPIO_PORTF_IS_R&= ~0x11; // Edge-trigger Interrupt
		GPIO_PORTF_IBE_R&= ~0x11; // not both-edges
		GPIO_PORTF_IEV_R&= ~0x11; // falling edges of PF0 & PF4
		GPIO_PORTF_ICR_R= 0x11;  // clear the interrupt flags before starting
		GPIO_PORTF_IM_R= 0x11; // Enable external interrupts on PF0, PF4
		// setup PortF interrupt on NVIC  
		NVIC_PRI7_R= (NVIC_PRI7_R & 0xFF0FFFFF) | 0x00800000; // Priority 4
		NVIC_EN0_R|= 0x40000000; // Enable PortF interrupt on NVIC	
		// PortF IRQ is 30 so we set bit 30 in the NVIC Enable register
}

// PF0 & PF4 are the PWM modifiers
// modification rate is 10% per push 
// PF0 +10%
// PF4 -10%
void GPIOPortF_Handler(void)
{
		if(PF4) 
		{
				GPIO_PORTF_ICR_R= 0x10; // ack PF4
				GPIO_PORTF_DATA_R= (GPIO_PORTF_DATA_R & ~0x04) ^ 0x02;
				if(High < 900)
				{
						High+= 100;
						Timer0APWM_DutyCycle(High);  // +10%
				}
		}
		else if(PF0)
		{
				GPIO_PORTF_ICR_R= 0x01; // ack PF0
				GPIO_PORTF_DATA_R= (GPIO_PORTF_DATA_R & ~0x02) ^ 0x04;
				if(High > 200)
				{
						High-= 100;
						Timer0APWM_DutyCycle(High); // -10%
				}
		}
}
