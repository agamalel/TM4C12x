#include "tm4c123gh6pm.h"
#include "PLL.h"
#include "UART0.h"

int main(void)
{
	long Num= 0;
	char StudentName[15]= {'\0'}, TestString[100]= {'\0'};
		
	PLL_Init();    // 80 MHz
	UART0_Init();  // UART2, PORTD(6, 7), 9600 bit/s

	UART0_TxString("Welcome to the Embedded Systems Workshop using ARM Cortex-M4 By Mo'meN Ali");
	UART0_NewLine();
	UART0_TxString("Would you please tell me your name ?");
	UART0_NewLine();
	UART0_RxString(StudentName, 15);
	UART0_NewLine();
	UART0_TxString("Welcome, ");
	UART0_TxString(StudentName);
	UART0_NewLine();
	UART0_TxString("Driver Test starts");
	UART0_NewLine();
	while(1)
	{
			UART0_TxString("Enter a String: ");
			UART0_RxString(TestString, 100);
			UART0_NewLine();
			UART0_TxString("You Entered: "); 
			UART0_TxString(TestString); 
			UART0_NewLine();

			UART0_TxString("Now Enter an unsigned number: ");  
			Num= UART0_RxUNum();
			UART0_TxString("You have Entered:  "); 
			UART0_TxUNum(Num); 
			UART0_NewLine();
	}
}


