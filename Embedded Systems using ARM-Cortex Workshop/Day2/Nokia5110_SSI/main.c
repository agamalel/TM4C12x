#include "tm4c123gh6pm.h"
#include "SSI_Nokia5110.h"

void Delay(unsigned int);
extern void EnableInterrupts(void);

int main(void)
{
	volatile unsigned long delay;
	
	EnableInterrupts();
	Nokia5110_Init();
	Nokia5110_PrintString("Mo'meN ");
	Nokia5110_PrintNum(POSITIVENUM, 123);
	while(1) {};
}

void Delay(unsigned int t)
{
	int i;

	for(; t != 0; --t)
			for(i= 133333; i != 0; --i) {}
}

