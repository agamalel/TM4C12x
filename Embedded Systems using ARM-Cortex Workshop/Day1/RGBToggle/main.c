#include "tm4c123gh6pm.h"

#define SW0			(GPIO_PORTF_DATA_R & 0x10)
#define SW1			(GPIO_PORTF_DATA_R & 0x01)

void GPIOPortF_Init(void); 
void Delay(unsigned int);

//  red, yellow, green, light blue, blue, purple,  white,  dark
const long RGB_LED[8] = {0x02, 0x0A, 0x08, 0x0C, 0x04, 0x06, 0x0E, 0x00};

int main(void)
{
	long Color= 0;
	volatile unsigned long SW0_PrevState, SW1_PrevState;
	SW0_PrevState= SW1_PrevState= 0;
	
	GPIOPortF_Init();
	while(1)
	{
			if (!SW0 && SW0_PrevState)						// SW0 is negative logic  
					GPIO_PORTF_DATA_R= RGB_LED[Color++];
			
			SW0_PrevState= SW0;
			if (!SW1 && SW1_PrevState)			// SW1 is negative logic
					GPIO_PORTF_DATA_R= RGB_LED[--Color];
				
			SW1_PrevState= SW1;
			// reset the color pointer
			if (Color < 0)
					Color= 7;
			
			else if (Color > 7)
					Color= 0;
	}
}

void Delay(unsigned int t)
{
	int i;

	for(; t != 0; --t)
			for(i= 133333; i != 0; --i) {}
}

void GPIOPortF_Init(void)
{	
	SYSCTL_RCGC2_R|= 0x00000020;
	while ( 0 == (SYSCTL_PRGPIO_R & 0x00000020) ) { } 		// Wait for PortF to be present
	GPIO_PORTF_LOCK_R = 0x4C4F434B;   // 2) unlock GPIO Port F
  GPIO_PORTF_CR_R = 0x1F;           // allow changes to PF4-0
	GPIO_PORTF_AMSEL_R&= 0x00;
  GPIO_PORTF_AFSEL_R&= 0x00;
	GPIO_PORTF_DEN_R|= 0x1F;
	GPIO_PORTF_DIR_R|= 0x0E;			// PF0 & PF4, Switches (Input), PF1-PF3 RGB LED (Output)
	GPIO_PORTF_PUR_R|= 0x11; 			// Enable PUll up resistor for the 2 switches  PF0, PF4
}
