#include "tm4c123gh6pm.h"
#include "PLL.h"
#include "UART.h"
#define SW1	(GPIO_PORTF_DATA_R & 0x10)
#define SW2 (GPIO_PORTF_DATA_R & 0x01)

void PortF_Init(void); 
void Delay(unsigned int);

//  red, yellow, green, light blue, blue, purple,  white,  dark
const long ColorWheel[8] = {0x02, 0x0A, 0x08, 0x0C, 0x04, 0x06, 0x0E, 0x00};

int main(void)
{
	volatile unsigned long SW1_PrevState, SW2_PrevState;
	unsigned long DisplayColor= 0, ChosenColor= 0;
	
	PLL_Init();   // 80 MHz
	UART_Init();  // UART2, PORTD(6, 7), 9600 bit/s
	PortF_Init();
	while(1)
	{
		if(!SW1 && SW1_PrevState)  // make sure it's not a debounce, PF switches are negative logic
		{
			ChosenColor= (ChosenColor + 1) & 0x07;
		}
		SW1_PrevState= SW1; // save SW1 state
		if(!SW2 && SW2_PrevState)
		{
			TX_UART( (ChosenColor | 0x30) ); // transmit the chosen color in character form 
		}
		SW2_PrevState= SW2;
		DisplayColor= (getChar_UART() & 0x07); // get the received char, decode it to a color
		if(DisplayColor)
		{
			ChosenColor= DisplayColor;
		}
		GPIO_PORTF_DATA_R= 0;
		GPIO_PORTF_DATA_R|= ColorWheel[ (ChosenColor & 0x07) ];
		Delay(10);	// avoid debounce
	}
}

void Delay(unsigned int t)
{
	int i;

	for(; t != 0; --t)
			for(i= 133333; i != 0; --i) {}
}

void PortF_Init(void)
{
	volatile unsigned long delay;
	
	SYSCTL_RCGC2_R|= 0x00000020;
	delay= SYSCTL_RCGC2_R;  
	GPIO_PORTF_LOCK_R = 0x4C4F434B;   // 2) unlock GPIO Port F
  GPIO_PORTF_CR_R = 0x1F;           // allow changes to PF4-0
	GPIO_PORTF_AMSEL_R&= 0x00;
  GPIO_PORTF_AFSEL_R&= 0x00;
	GPIO_PORTF_DEN_R|= 0x1F;
	GPIO_PORTF_DIR_R|= 0x0E;
	GPIO_PORTF_PUR_R|= 0x11; // Enable PUll up resistor for the 2 switches  PF0, PF4
}
