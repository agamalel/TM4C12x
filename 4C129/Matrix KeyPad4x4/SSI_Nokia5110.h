// SSI_Nokia5110.c
// Runs on LM4F120/TM4C123
// Provide a graphical library for Nokia5110 LCD
// Mo'meN Ali

// Red SparkFun Nokia 5110 (LCD-10168)
// -----------------------------------
// Signal        (Nokia 5110) LaunchPad pin
// Reset         (RST, pin 1) connected to PA7
// SSI0Fss       (CE,  pin 2) connected to PA3
// Data/Command  (DC,  pin 3) connected to PA6
// SSI0Tx        (Din, pin 4) connected to PA5
// SSI0Clk       (Clk, pin 5) connected to PA2
// 3.3V          (Vcc, pin 6) power
// back light    (BL,  pin 7) not connected
// Ground        (Gnd, pin 8) ground


// standard ASCII symbols
#define CR   0x0D
#define LF   0x0A
#define BS   0x08
#define ESC  0x1B
#define SP   0x20
#define DEL  0x7F

enum writetype {
	command, 
	data
};

void Nokia5110_Clear(void);
void Nokia5110_OutChar(unsigned char Ch);
void Nokia5110_OutString(unsigned char *pSTR);
void Nokia5110_SetCursor(unsigned long X, unsigned long Y);
void Nokia5110_Write(enum writetype type, unsigned char message);
void Nokia5110_Init(void);
void SSI_Init(void);
void Nokia511_OutString(unsigned char *pSTR);
void ConvertU_TODistance(unsigned long temp);
void Nokia5110_DrawImage(unsigned char *pSTR);
void Nokia5110_OutUDec(unsigned short n);
