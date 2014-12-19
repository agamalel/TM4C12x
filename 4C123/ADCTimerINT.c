// TemperatureSensor.c
// Runs on LM4F120
// Provide a function that initializes Timer0A to trigger ADC
// SS3 conversions and request an interrupt when the conversion is complete.
// Mo'meN Ali
// July 31, 2014


#include "TemperatureSensor.h"
#include "tm4c123gh6pm.h"
#include "SSI_Nokia5110.h"

#define ADC_SSCLT3_TS					0x0008 // Enable Temperature sensor														 
#define ADC_SSCTL3_IM	   			0x0004 
#define ADC_SSCTL3_END				0x0002 
#define SYSCTL_RCGC0_ADC0			0x00010000
#define SYSCTL_RCGC1_TIMER0		0x00010000

void EnableInterrupts(void);
void DisableInterrupts(void);

// This initialization function sets up the ADC
// Max sample rate: <=125,000 samples/second
// SS3 triggering event: software trigger
// SS3 1st sample source:  channel 1
// SS3 interrupts: enabled but not promoted to controller
// Timer0 32-bit is the trigger Source
// Sample  timer = Bus Period * (Prescale + 1) * (Reload value + 1)
void Init_TemperatureSensor(unsigned long SamplePeriod) 
{
	/*** GPIO_PORTE Init ***/
	DisableInterrupts();
	SYSCTL_RCGC2_R|= SYSCTL_RCGC2_GPIOE; 
	SYSCTL_RCGC0_R|= SYSCTL_RCGC0_ADC0;   // ADC Clock enable (legacy mode)
	SYSCTL_RCGC1_R|= SYSCTL_RCGC1_TIMER0; // Enable Clock for Timer0 (legacy mode)
	GPIO_PORTE_DEN_R&= ~0x20;  // Disable analog on PE5
	GPIO_PORTE_AMSEL_R|= 0x20; // Analog on PE5
	GPIO_PORTE_AFSEL_R|= 0x20; 
	GPIO_PORTE_DIR_R&= ~0x20; // PE5 input 
	/*** Timer0 init ***/
	TIMER0_CTL_R&= ~0x00000001; // Disable Timer0 while configuring
	TIMER0_CTL_R|= 0x00000020;  // Enable Timer0 to trigger ADC
	TIMER0_CFG_R&= 0; // 32-bit Timer0
	TIMER0_TAMR_R|= 0x0000002; // Timer0 Periodic Mode, defualt count down
	TIMER0_TAPR_R= 0;					 // Bus clock resolution
	TIMER0_TAILR_R= SamplePeriod;	// reload value
	TIMER0_TAPR_R = 0;     // prescale value for trigger
	TIMER0_IMR_R&= ~0x00000001;   // Disable rollover / interrupt
	TIMER0_CTL_R|= 0x00000001;    //  Start/Enable Timer0, 32-bit, Periodic mode, no interrupts
	/*** ADC init ***/
	ADC0_ACTSS_R&= ~0x00000008; // Disable SS3 
	ADC0_SSPRI_R= (ADC0_SSPRI_R & 0) | 0x0123; // SS3 priority 0 (higher)
	ADC0_PP_R= (ADC0_PP_R & 0) | 0x00000010 | 0x00000001; // Resolution is 2, 125k sample/sec
	ADC0_EMUX_R|= 0x5000; // Timer is the trigger source
	ADC0_SSCTL3_R= (ADC0_SSCTL3_R & 0) | ADC_SSCTL3_IM | ADC_SSCTL3_END;  // Interrupts and END0. No differential sampling mode 
	ADC0_SSMUX3_R= (ADC0_SSCTL3_R & 0x0) | 0x8;  // AIN8 is  the sample destination
	ADC0_IM_R= (ADC0_IM_R & ~0x0000000F) | 0x00000008; // Promote SS2 interrupt to the controller	
	ADC0_ACTSS_R|= 0x00000008; // Enable SS3
	ADC0_ISC_R= 0x00000001; 	 // clear interrupt flag
	NVIC_PRI4_R= (NVIC_PRI4_R & 0xFFFF00FF) | 0x00002000; // bits 13-15
	NVIC_EN0_R|= 1 << 17; // Enable interrupt 17 in NVIC
}

unsigned int ADCdata;
void ADC0Seq3_Handler(void)
{
	ADCdata= ADC0_FIFO3_R & 0xFF;
	ADC0_ISC_R= 0x00000008; // ack INT SS3
}
