// TemperatureSensor.h
// Runs on LM4F120/TM4C123
// Provide functions that initialize ADC0 SS3 to be triggered by
// software and trigger a conversion, wait for it to finish,
// and return the result. 
// Mo'meN Ali
// July 8, 2014



// This initialization function sets up the ADC 
// Max sample rate: <=125,000 samples/second
// SS3 triggering event: Timer0 trigger
// SS3 1st sample source: channel 1
// SS3 interrupts: enabled and promoted to controller
void TemperatureSensor_Init(unsigned long SamplePeriod);





