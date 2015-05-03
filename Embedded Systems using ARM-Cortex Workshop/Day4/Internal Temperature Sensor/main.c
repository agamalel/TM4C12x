// Slide pot pin 3 connected to +3.3V
// Slide pot pin 2 connected to PE1 and PD3
// Slide pot pin 1 connected to ground

#include "PLL.h"
#include "TemperatureSensor.h"
#include "SSI_Nokia5110.h"
#include "tm4c123gh6pm.h"

void EnableInterrupts(void);  // Enable interrupts
void WaitForInterrupt(void);  // low power mode


int main(void)
{
	volatile unsigned long delay;
	PLL_Init();
	SYSCTL_RCGC2_R|= SYSCTL_RCGC2_GPIOF;    // activate port F
	TemperatureSensor_Init(16000000);  			// 5 Hz
  GPIO_PORTF_DIR_R|= 0x0E;                
  GPIO_PORTF_AFSEL_R&= ~0x0E;             // disable alt funct RGB LED
  GPIO_PORTF_DEN_R|= 0x0E;                // enable digital I/O RGB LED
                                          // configure  RGB LED
  GPIO_PORTF_PCTL_R= (GPIO_PORTF_PCTL_R&0xFFFFF0FF)+0x00000000;
  GPIO_PORTF_AMSEL_R= 0;                  // disable analog functionality RGB LED
	GPIO_PORTF_DATA_R&= ~0x0E;              // turn off RGB LED
	Nokia5110_Init();
	EnableInterrupts();
  while(1)
	{
			WaitForInterrupt();   // low power mode
  }
}
