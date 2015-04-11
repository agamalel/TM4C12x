// This code runs on TM4C123
// using Timer0PWM 
// Timer0APWM for right wheel PB6
// Timer0BPWM for Left Wheel
// By Mo'meN Ali

// PWM0	PB6
// PWM1	PB7


// Timer0A, 16-bit
// initialize the PWM0 for the right wheel
// PB6(7)
// Period is 1/clockFrequency 
// you must callthe this function in the begining
// because it contains the clock enable for both the timer and the GPIO
void Timer0APWM_Init(unsigned short Period, unsigned short High);

// Change Duty cycle on Timer0A
// Timer0A, 16-Bit
// maximum Speed is Clock Speed / Period  
// for 50MHz Timer speed
// Duty Cycle ==> (Period * (High / 100))
// Example: 75% duty cycle ==> (Period * (75/100))
void Timer0APWM_DutyCycle(unsigned short High);

// Timer0B, 16-bit
// Initialize the PWM1 for the left wheel
// PB7(7)
// Period is 1/clockFrequency 
// you must call Timer0APWM_Init(unsigned Period, unsigned short High) first
void Timer0BPWM_Init(unsigned short Period, unsigned short High);

// Change Duty cycle on Timer0B
// Timer0B, 16-Bit
// maximum Speed is Clock Speed / Period 
void Timer0BPWM_DutyCycle(unsigned short High);

