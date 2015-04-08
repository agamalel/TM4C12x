#include "tm4c123gh6pm.h"
#include "PLL.h"
#include "UART0.h"
#define SW1	(GPIO_PORTF_DATA_R & 0x10)
#define SW2 (GPIO_PORTF_DATA_R & 0x01)

void PortF_Init(void); 
void Delay(unsigned int);

//  red, yellow, green, cyan, blue, violet,  white,  dark
const long RGB_LED[8] = {0x02, 0x0A, 0x08, 0x0C, 0x04, 0x06, 0x0E, 0x00};

int main(void)
{
	unsigned long ColorNum= 0;
	char ChosenColor= '\0';
	
	PLL_Init();   // 80 MHz
	UART0_Init();  // UART2, PORTD(6, 7), 9600 bit/s
	PortF_Init();
	while(1)
	{
			UART0_TxString("Enter a color to be displayed, please !");
			UART0_NewLine();	
			UART0_TxString("r, g, b, y, v, c, w, d:  ");
			while (!ChosenColor)   								  // wait until there's a command
						UART0_RxChar(&ChosenColor);				// recieve a character
			
			UART0_NewLine();			
		  UART0_TxString("received ");
		  UART0_TxChar(ChosenColor);
		  UART0_NewLine();
			
			switch (ChosenColor)
			{
					case 'r':		
						ColorNum= 0;
						break;
					case 'y':
						ColorNum= 1;
						break;
					case 'g':
						ColorNum= 2;				
						break;
					case 'c':		
						ColorNum= 3;
						break;	
					case 'b':
						ColorNum= 4;
						break;	
					case 'v':		
						ColorNum= 5;
						break;
					case 'w':		
						ColorNum= 6;
						break;
					case 'd':		
						ColorNum= 7;
						break;				
			}
			GPIO_PORTF_DATA_R= RGB_LED[ColorNum];					// Display the command
			ChosenColor= '\0';														// Clear the Input variable
	}
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
	GPIO_PORTF_DEN_R|= 0x1F;					// Enable Digital on PF0-PF4
	GPIO_PORTF_DIR_R|= 0x0E;					// PF0, PF4 input (switches), PF1-PF3 output (RGB LED)
	GPIO_PORTF_PUR_R|= 0x11; // Enable PUll up resistor for the 2 switches  PF0, PF4
}
