
// **************DAC_Init*********************
// Initialize 4-bit DAC 
// Input: none
// Output: none
void DAC_Init(void);

// **************DAC_Out*********************
// output to DAC
// Input: 4-bit data, 0 to 15 
// Output: none
void DAC_Out(unsigned long data);
void Sound_Play(void);
void Sound_Init(void);
void Sound_Set(const unsigned char *ptrTOSound, unsigned long elements);
void Sound_Highpitch(void);
void Sound_Fastinvader1(void);
void Sound_Fastinvader2(void);
void Sound_Fastinvader3(void);
void Sound_Fastinvader4(void);
void Sound_Explosion(void);
void Sound_Killed(void);
void Sound_Shoot(void);
