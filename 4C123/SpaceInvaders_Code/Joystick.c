// Slide pot pin 3 connected to +3.3V
// Slide pot pin 2 connected to PE1 and PD3
// Slide pot pin 1 connected to ground

#include "Joystick.h"
#include "..//tm4c123gh6pm.h"

void EnableInterrupts(void);  // Enable interrupts

unsigned char String[10];
unsigned long Distance; // units 0.001 cm
unsigned long ADCdata;  // 12-bit 0 to 4095 sample
volatile unsigned long Flag;     // 1 means valid Distance, 0 means Distance is empty

//********Convert****************
// Convert a 12-bit binary ADC sample into a 32-bit unsigned
// fixed-point distance (resolution 0.001 cm).  Calibration
// data is gathered using known distances and reading the
// ADC value measured on PE1.
// Overflow and dropout should be considered
// Input: sample  12-bit ADC sample
// Output: 32-bit distance (resolution 0.001cm)
unsigned long Convert(unsigned long Sample)
{
  return ( ((500 * Sample) >> 10) + 1 );
}
//-----------------------UART_ConvertDistance-----------------------
// Converts a 32-bit distance into an ASCII string
// Input: 32-bit number to be converted (resolution 0.001cm)
// Output: store the conversion in global variable String[10]
// Fixed format 1 digit, point, 3 digits, space, units, null termination
// Examples
//    4 to "0.004 cm"
//   31 to "0.031 cm"
//  102 to "0.102 cm"
// 2210 to "2.210 cm"
//10000 to "*.*** cm"  any value larger than 9999 converted to "*.*** cm"
void UART_ConvertDistance(unsigned long Distance)
{
	long i;

	for(i= 4; i > -1; --i)
	{
		if(Distance < 10000)
		{
			if(1 != i )
			{
				String[i]= Distance % 10;
				String[i]|= 0x30;
				Distance/= 10;
			}
			else
				String[i]= '.';
		}
		else
		{
			if(1 != i)
				String[i]= '*';
			else
				String[i]= '.';
		}
	}
	i= 5;
	String[i++]= ' ';
	String[i++]= 'c';
	String[i++]= 'm';
}

// executes every 25 ms, collects a sample, converts and stores in mailbox
void SysTick_Handler(void)
{
  ADCdata= ADC0_Sample();
	Distance= Convert(ADCdata);
	UART_ConvertDistance(Distance);
	Flag= 1; // There's new data
}

int main(void)
{
  while(1)
	{
// read mailbox
		while(0 == Flag){} // wait data
		Flag= 0;
  }
}
