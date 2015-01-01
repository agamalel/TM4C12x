// UART0 Device Driver 
// Runs on TM4C/LM4F
// By Mo'meN Ali
// August 27, 2014

#include "stdint.h"

// this connection occurs in the USB debugging cable
// U0Rx 17 PA0 (1) I TTL UART module 0 receive. 
// U0Tx 18 PA1 (1) O TTL UART module 0 transmit.
// Ground connected ground in the USB cable

// standard ASCII symbols
#define CR   0x0D
#define LF   0x0A
#define BS   0x08
#define ESC  0x1B
#define SP   0x20
#define DEL  0x7F


// initialize the UART0 for 115200 baud rate (assuming 16 Mhz PIOSC Clock)
// 8 bit frame length, no parity bits, one stop bit, FIFOs enabled
// Interrupt Enabled
void UART0_Init(void);

// Output String (NULL termination)
// Input: pointer to a NULL-terminated string to be transferred
void UART0_TxString(char *Ch);

// Output 8-bit to serial port
// Input: letter is an 8-bit ASCII character to be transferred
void UART0_TxChar(char Ch);

//-----------------------UART_TxUDec-----------------------
// Output a 32-bit number in unsigned decimal format
// Input: 32-bit number to be transferred
// Output: none
// Variable format 1-10 digits with no space before or after
void UART0_TxUDec(uint32_t UDecNum);

// Accepts ASCII input in unsigned char format
// Input: none
// Output: 8-bit unsigned number
// if you enter any number above 255, it will overflow
void UART0_RxChar(char *data);

// Accepts ASCII input in unsigned decimal format
// and converts it to a 32-bit unsigned number
// valid range is 0 to 4294967295 (2^32-1)
// Input: none
// Output: 32-bit unsigned number
// If you enter a number above 4294967295, it will return an incorrect value
// Backspace will remove last digit typed
void UART0_RxDecNum(uint32_t *UDecNum);

// Make a newline
void UART0_NewLine(void);
