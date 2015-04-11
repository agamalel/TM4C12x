// UART0 Device Driver 
// Runs on TM4C
// By Mo'meN Ali
// January 1, 2015

/* Copyright by Mo'meN M. Ali, momen.1993@live.com
You may use, edit, run or distribute this file
as long as the above copyright notice remains
THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. */

#include "stdint.h"

// this connection occurs in the USB debugging cable
// U0Rx 17 PA0 (1) I TTL UART module 0 receive. 
// U0Tx 18 PA1 (1) O TTL UART module 0 transmit.
// Ground connected ground in the USB cable

// standard ASCII symbols
#define CR   0x0D    // <enter>
#define LF   0x0A		 // NEW LINE
#define BS   0x08    // <backspace>
#define ESC  0x1B    // <Escape>
#define SP   0x20    // 
#define DEL  0x7F    // <Delete>


// initialize the UART0 for 115200 baud rate (assuming 16 Mhz PIOSC Clock)
// 8 bit frame length, no parity bits, one stop bit, FIFOs enabled
// Interrupt Enabled
void UART0_Init(void);

// Output String (NULL termination)
// Input: pointer to a NULL-terminated string to be transferred
void UART0_TxString(char *Ch);

//------------UART0_RxStr------------
// Accepts ASCII characters from the serial port
//    and adds them to a string until <enter> is typed
//    or until max length of the string is reached.
// It echoes each character as it is inputted.
// If a backspace is inputted, the string is modified
//    and the backspace is echoed
// terminates the string with a null character
// uses busy-waiting synchronization on RDRF
// Input: pointer to empty buffer, size of buffer
// Output: Null terminated string
// -- Modified by Agustinus Darmawan + Mingjie Qiu --
void UART0_RxString(char *string, uint16_t maxLen);

// Output 8-bit to serial port
// Input: letter is an 8-bit ASCII character to be transferred
void UART0_TxChar(char Ch);

// Accepts ASCII input in unsigned char format
// Input: none
// Output: 8-bit unsigned number
// if you enter any number above 255, it will overflow
// if FIFOs empty, it waits until you receive at least 1 character
void UART0_RxChar(char *data);

// Accepts ASCII input in unsigned char format
// Input: none
// Output: 8-bit unsigned number
// if you enter any number above 255, it will overflow
// it picks up one character from the FIFO then return
// if the FIFO is full it returns 0
char UART0_GetOneChar(void);
//-----------------------UART_TxUDec-----------------------
// Output a 32-bit number in unsigned decimal format
// Input: 32-bit number to be transferred
// Output: none
// Variable format 1-10 digits with no space before or after
void UART0_TxUNum(uint32_t UDecNum);



//------------UART0_RxUDec------------
// RxUDec() accepts ASCII input in unsigned decimal format
//     and converts to a 32-bit unsigned number
//     valid range is 0 to 4294967295 (2^32-1)
// Input: none
// Output: 32-bit unsigned number
// If you enter a number above 4294967295, it will return an incorrect value
// Backspace will remove last digit typed
uint32_t UART0_RxUNum(void);

// Make a newline
void UART0_NewLine(void);

