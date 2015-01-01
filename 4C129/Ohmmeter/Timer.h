// Mo'meN Ali
// 24 December, 2014

#include <stdint.h>

// Timer0A 16-bit 
// capture mode / input edge time mode
// You must enable interrupts in your application
void T0A_InputEdgeInit(void);

void T0A_PeriodicInit(uint32_t Period);

// T0CCP1
// Pin-PL5
void T0A_FreqMeasurement(void);
