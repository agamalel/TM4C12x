
#include "Joystick.h"
#include "tm4c123gh6pm.h"

char String[10];
volatile unsigned long Flag;     // 1 means valid Distance, 0 means Distance is empty

// This initialization function sets up the ADC
// Max sample rate: <=125,000 samples/second
// SS3 triggering event: software trigger
// SS3 1st sample source:  channel 1
// SS3 interrupts: enabled but not promoted to controller
void ADC0_Init(void)
{
	unsigned long delay;

	// GPIOE Init
	SYSCTL_RCGC2_R|= SYSCTL_RCGC2_GPIOE;
	delay= SYSCTL_RCGC2_R;
	GPIO_PORTE_DEN_R|= 0x04;
	GPIO_PORTE_PCTL_R&= 0xFFFF0000; // Mask the lower nibble to use PE2 as AIN1
	GPIO_PORTE_DIR_R&= ~0x04;  // PE2 input
	GPIO_PORTE_AMSEL_R|= 0x04; // PE2-analog
	GPIO_PORTE_AFSEL_R|= 0x04; // PE2 Alternate function (ADC)
	// ADC Init, PE2
	SYSCTL_RCGC0_R|= 0x00010000;
	delay= SYSCTL_RCGC0_R;
	SYSCTL_RCGC0_R&= ~0x00000300;  // Clear bits 8-9 for 125k sampling rate
	ADC0_SSPRI_R= (ADC0_SSPRI_R & 0) | 0x0123;   // Set sequencer 3 as the highest priority
	ADC0_ACTSS_R&= ~0x08;    // Disable SS3 while configuration
	ADC0_EMUX_R&= ~0xF000;   // Software trigger
	ADC0_SSMUX3_R&= ~0x000F; // Clear the SS3 field
	ADC0_SSMUX3_R|= 0x0001;  // Select AIN1 as the input pin for SS3
	ADC0_SSCTL3_R|= 0x0006;  // Set END0, IE0; Clear D0, TS0
  ADC0_ACTSS_R|= 0x08;     // Activate Analog-to-digital SS3
}

//------------ADC0_In------------
// Busy-wait Analog to digital conversion
// Input: none
// Output: 12-bit result of ADC conversion

unsigned long ADC0_Sample(void)
{
	unsigned long ADC_Data;

	ADC0_PSSI_R|= 0x08; 							  // Initiate a sampling sequence on SS3
	while( 0 == (ADC0_RIS_R & 0x08 ) ){} // Wait until SS3 is done
	ADC_Data= ADC0_SSFIFO3_R; 				  // Save 12-bit result
	ADC0_ISC_R|= 0x08; 								  // Acknowledge the interrupt bit

  return ADC_Data;
}

unsigned long ADC_Calibration(unsigned long Sample)
{
  return ( ((1690 * Sample) >> 10) );
}
