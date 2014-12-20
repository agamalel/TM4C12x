// DC Motor Control using PWM
// The motor is responding to the average of the pulses
// The 1 pulse takes 1ms
// SysTick_Handler will be the PWM producer
// PortF_Handler will be the PWM controller using edge triggered interrupts

#include "tm4c123gh6pm.h"
#include "PLL.h"
#define SW1	(GPIO_PORTF_DATA_R & 0x10)
#define SW2 (GPIO_PORTF_DATA_R & 0x01)
#define PA5 (GPIO_PORTA_DATA_R & 0x20)
#define FullWave	80000

unsigned long H_Period, L_Period;

extern void EnableInterrupts(void);
extern void WaitForInterrupt(void);
void Delay(unsigned int);
void Init_SW(void); 
void Init_SysTick(unsigned long);
void PortF_Handler(void);
void SysTick_Handler(void);
void Init_Motor(void);


int main(void)
{
	PLL_Init();   // 80 MHz
	EnableInterrupts(); // Enable global interrupt register flag 
	Init_SW(); 
	Init_Motor(); 
	while(1)
	{
		WaitForInterrupt();
	}
}

void Init_SysTick(unsigned long pValue)
{
	NVIC_ST_CTRL_R&= 0; // Disable SysTick timer while setup
	NVIC_ST_RELOAD_R= pValue - 1; 
	NVIC_ST_CURRENT_R= 0; 
	NVIC_ST_CTRL_R|= 0x00000007; // Enable Clock, Enable SysTick and Enable Interrupts
}

void Init_SW(void)
{
	volatile unsigned long delay= 0;
// PortF Configuration	
	SYSCTL_RCGC2_R|= SYSCTL_RCGC2_GPIOF;
	delay= SYSCTL_RCGC2_R;  // 3 cycles delay
	GPIO_PORTF_LOCK_R= 0x4C4F434B; // unlock GPIOF
	GPIO_PORTF_CR_R|= 0x11; // Enable for PF0 & PF4
	GPIO_PORTF_PCTL_R&= 0;  // will use digital
	GPIO_PORTF_AMSEL_R&= 0; // no analog
	GPIO_PORTF_AFSEL_R&= 0; // no AF
	GPIO_PORTF_DEN_R|= 0x11; // 2 switches
	GPIO_PORTF_PUR_R|= 0x11; // pull-up the 2 switches
	GPIO_PORTF_DIR_R|= 0x11; // output switches
// PortF Interrupt Configuration
	GPIO_PORTF_IS_R&= 0; // Edge-trigger Interrupt
	GPIO_PORTF_IBE_R&= 0; // not both-edges
	GPIO_PORTF_IEV_R&= 0x00; // falling edges of PF0 & PF4
  GPIO_PORTF_ICR_R&= 0;  // clear the interrupt flags before starting
  GPIO_PORTF_IM_R|= 0x11; // Enable external interrupts on PF4, PF4
  // setup PortF interrupt on NVIC  
	NVIC_PRI7_R= (NVIC_PRI7_R & 0xFF0FFFFF) + 0x00800000; // Priority 4
	NVIC_EN0_R|= 0x40000000; // Enable PortF interrupt on NVIC	
	// PortF IRQ is 30 so we set bit 30 in the NVIC Enable register
}

void Delay(unsigned int t)
{
	int i;

	for(; t != 0; --t)
			for(i= 133333; i != 0; --i) {}
}

void Init_Motor(void)
{
	volatile unsigned long delay= 0;
// init PA5 as digital output pin with 8mA drive strength
	SYSCTL_RCGC2_R|= SYSCTL_RCGC2_GPIOA;
	delay= SYSCTL_RCGC2_R;
	GPIO_PORTA_AMSEL_R&= 0;
	GPIO_PORTA_AFSEL_R&= 0;
	GPIO_PORTA_PCTL_R&= 0;
	GPIO_PORTA_DEN_R|= 0x20; // PA5 as digital
	GPIO_PORTA_DR8R_R|= 0x20; // Drive 8mA on PA5
	GPIO_PORTA_DIR_R|= 0x20; // PA5 output
  GPIO_PORTA_DATA_R= 0; // Low output
	H_Period= L_Period= 8000; // set the initial period to 10%
	Init_SysTick(H_Period); // it does not matter what period to start with
}
// Toglle PA5 with PWM
void SysTick_Handler(void)
{
	if(!PA5) // if PA5 is low
	{
		GPIO_PORTA_DATA_R^= 0x20; // toggle PA5 to high
		// Configure the high Period by making SysTick wait for amout of High Pulse till next round
		NVIC_ST_RELOAD_R= FullWave - L_Period; 	
	}
	else // else should be low
	{
		GPIO_PORTA_DATA_R^= 0x20; // make it low
		// wait the an amount of the low period 
		NVIC_ST_RELOAD_R= FullWave - H_Period; 
	}
}

void PortF_Handler(void)
{
  if(SW1) // faster by 10%
	{
		if(L_Period > 8000) // make sure it wont go less than 10%
			L_Period= L_Period - 8000; // increse the speed by increasing the High Period
		
		GPIO_PORTA_ICR_R|= 0x01; // Acknowledge the interrupt for PF0
	}
	else if(SW2) // slower by 10%
	{
		if(H_Period < 72000)
			L_Period= L_Period + 8000; // go slower
		
		GPIO_PORTA_ICR_R|= 0x10; // Acknowledge the interrupt for PF4
	}
	H_Period= FullWave - L_Period; // calculate the High Period
}
