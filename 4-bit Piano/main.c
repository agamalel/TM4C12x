// Lab13.c
// Runs on LM4F120 or TM4C123
// Use SysTick interrupts to implement a 4-key digital piano
// edX Lab 13 
// Daniel Valvano, Jonathan Valvano
// March 13, 2014
// Port B bits 3-0 have the 4-bit DAC
// Port E bits 3-0 have 4 piano keys

#include "..//tm4c123gh6pm.h"
#include "Sound.h"
#include "Piano.h"
#include "PLL.h"
#define C 4780
#define D 4259
#define E 3794
#define G	3193


// basic functions defined at end of startup.s
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void delay(unsigned long msec);
void GPIOPortFInit(void);

int main(void)
{ 
 	unsigned long input= 0;     
  
	PLL_Init();
	Sound_Init(); // initialize SysTick timer and DAC
  Piano_Init();
  GPIOPortFInit();
	EnableInterrupts();  // enable after all initialization are don
  while(1)
	{                
// input from keys to select tone
		input= Piano_In();

		switch (input)
		{
			case 0x01:
				Sound_Tone(C);
				break;
			case 0x2:
				Sound_Tone(D);
				break;
		  case 0x04:
				Sound_Tone(E);
				break;
			case 0x08:
				Sound_Tone(G);
				break;
			default:
				Sound_Off();
				break;
		}	
  }  
            
}

void
GPIOPortFInit(void) 
{
	unsigned long delay;
	
	SYSCTL_RCGC2_R|= 0x00000020;
	delay= SYSCTL_RCGC2_R;  
	GPIO_PORTF_LOCK_R = 0x4C4F434B;   // 2) unlock GPIO Port F
  GPIO_PORTF_CR_R = 0x1F;           // allow changes to PF4-0
	GPIO_PORTF_AMSEL_R&= 0x00;
  GPIO_PORTF_AFSEL_R&= 0x00;
	GPIO_PORTF_DEN_R|= 0x1F;
	GPIO_PORTF_DIR_R|= 0x0E;
	GPIO_PORTF_PUR_R|= 0x11; // Enable PUll up resistor for the 2 switches  PF0, PF4
	GPIO_PORTF_DATA_R|= 0x04;
}		

// Inputs: Number of msec to delay
// Outputs: None
void delay(unsigned long msec){ 
  unsigned long count;
  while(msec > 0 ) {  // repeat while there are still delay
    count = 16000;    // about 1ms
    while (count > 0) { 
      count--;
    } // This while loop takes approximately 3 cycles
    msec--;
  }
}


