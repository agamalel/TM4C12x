// PLL configuration
// TM4C1294NCPDT
// By Mo'meN Ali
// August 31, 2014
// modified January, 4, 2014

#include <stdint.h>
#include "PLL.h"

#define SYSCTL_RIS_R            (*((volatile uint32_t *)0x400FE050))
#define SYSCTL_RIS_MOSCPUPRIS   0x00000100  // MOSC Power Up Raw Interrupt
                                            // Status
#define SYSCTL_MOSCCTL_R        (*((volatile uint32_t *)0x400FE07C))
#define SYSCTL_MOSCCTL_PWRDN    0x00000008  // Power Down
#define SYSCTL_MOSCCTL_NOXTAL   0x00000004  // No Crystal Connected
#define SYSCTL_RSCLKCFG_R       (*((volatile uint32_t *)0x400FE0B0))
#define SYSCTL_RSCLKCFG_MEMTIMU 0x80000000  // Memory Timing Register Update
#define SYSCTL_RSCLKCFG_NEWFREQ 0x40000000  // New PLLFREQ Accept
#define SYSCTL_RSCLKCFG_USEPLL  0x10000000  // Use PLL
#define SYSCTL_RSCLKCFG_PLLSRC_M                                              \
                                0x0F000000  // PLL Source
#define SYSCTL_RSCLKCFG_PLLSRC_MOSC                                           \
                                0x03000000  // MOSC is the PLL input clock
                                            // source
#define SYSCTL_RSCLKCFG_OSCSRC_M                                              \
                                0x00F00000  // Oscillator Source
#define SYSCTL_RSCLKCFG_OSCSRC_MOSC                                           \
                                0x00300000  // MOSC is oscillator source
#define SYSCTL_RSCLKCFG_PSYSDIV_M                                             \
                                0x000003FF  // PLL System Clock Divisor
#define SYSCTL_DSCLKCFG_R       (*((volatile uint32_t *)0x400FE144))
#define SYSCTL_DSCLKCFG_DSOSCSRC_M                                            \
                                0x00F00000  // Deep Sleep Oscillator Source
#define SYSCTL_DSCLKCFG_DSOSCSRC_MOSC                                         \
                                0x00300000  // MOSC
#define SYSCTL_PLLFREQ0_R       (*((volatile uint32_t *)0x400FE160))
#define SYSCTL_PLLFREQ0_PLLPWR  0x00800000  // PLL Power
#define SYSCTL_PLLFREQ0_MFRAC_M 0x000FFC00  // PLL M Fractional Value
#define SYSCTL_PLLFREQ0_MINT_M  0x000003FF  // PLL M Integer Value
#define SYSCTL_PLLFREQ0_MFRAC_S 10
#define SYSCTL_PLLFREQ0_MINT_S  0
#define SYSCTL_PLLFREQ1_R       (*((volatile uint32_t *)0x400FE164))
#define SYSCTL_PLLFREQ1_Q_M     0x00001F00  // PLL Q Value
#define SYSCTL_PLLFREQ1_N_M     0x0000001F  // PLL N Value
#define SYSCTL_PLLFREQ1_Q_S     8
#define SYSCTL_PLLFREQ1_N_S     0
//#define SYSCTL_PLLSTAT_R        (*((volatile uint32_t *)0x400FE168))
#define SYSCTL_PLLSTAT_LOCK     0x00000001  // PLL Lock
#define SYSCTL_RSCLKCFG_USEPLL	0x10000000
#define SYSCTL_PLLFREQ0_PLLPWR	0x00800000
#define SYSCTL_MEMTIM0_R				( *((volatile uint32_t *)0x400FE0C0) )
#define SYSCTL_PLLSTAT_R				( *((volatile uint32_t *)0x400FE168) )
#define SYSCTL_PLLSTAT_LOCK			0x00000001
#define SYSCTL_ALTCLKCFG_ALTCLK_PIOSC		0

// SysClk = fVCO/(PSYSDIV + 1)
// fVCO = (fIN*MDIV)
// fIN = fXTAL/((Q+1)(N+1)) or fPIOSC/((Q+1)(N+1))
// MDIV = MINT + (MFRAC/1024)
// The Q and N values are programmed in the PLLFREQ1 register. Note that to reduce jitter, MFRAC
// should be programmed to 0x0.

// to set the SysClk to 120 MHz, you need to set the fVCO to 480 MHz and the PSYSDIV to 4  


void PLL_Init(void)
{	
	 uint32_t timeOut= 0;    // time out variable
	 // after POR the syste is clocked from the PIOSC
	 // but if it's previously called then changing the PLL VCO frequency must be done as a sequence
	 //from PLL to PIOSC/MOSC and then PIOSC/MOSC to new PLL.
	 SYSCTL_RSCLKCFG_R&= ~SYSCTL_RSCLKCFG_USEPLL;
	 SYSCTL_MOSCCTL_R&= ~SYSCTL_MOSCCTL_NOXTAL;
	 // since crystal oscillator is required, clearn the PWRDN bit
	 SYSCTL_MOSCCTL_R&= ~SYSCTL_MOSCCTL_PWRDN;
	 // wait until affects to take place
	 while ( 0 == (SYSCTL_RIS_MOSCPUPRIS & SYSCTL_RIS_R) ) { }
	 // Temporarily get run/sleep clock from 25 MHz main oscillator.
	 SYSCTL_RSCLKCFG_R= (SYSCTL_RSCLKCFG_R & ~SYSCTL_RSCLKCFG_OSCSRC_M) | SYSCTL_RSCLKCFG_OSCSRC_MOSC;
	 // PLL clock source is the MOSC clock source
	 SYSCTL_RSCLKCFG_R= (SYSCTL_RSCLKCFG_R & ~SYSCTL_RSCLKCFG_PLLSRC_M) | SYSCTL_RSCLKCFG_PLLSRC_MOSC;
	 // if the application needs the deep sleep mode clock source to be the MOSC clock source
	 SYSCTL_DSCLKCFG_R= (SYSCTL_DSCLKCFG_R & ~SYSCTL_DSCLKCFG_DSOSCSRC_M) | SYSCTL_DSCLKCFG_DSOSCSRC_MOSC;
	 // set the Q, N, MINT & MFRAC values to set the f-vco
	 // f-vco = f-IN * MDIV
	 // f-IN = (f-XTAL/((Q+1)*(N+1))) || (f-PIOSC/((Q+1)*(N+1)))
   // MDIV = MINT + (MFRAC/1024)
#define FXTAL										 25000000
#define MINT													 96
#define MFRAC														0
#define Q																0
#define N																4
#define Q            										0
#define N            										4  // chosen for reference frequency within 4 to 30 MHz
#define MINT        									 96  // 480,000,000 = (25,000,000/(0 + 1)/(4 + 1))*(96 + (0/1,024))
#define MFRAC       									  0  // zero to reduce jitter
#define SYSCLK       ( ((FXTAL / ((Q + 1) * (N + 1))) * (MINT + MFRAC/1024)) / (PSYSDIV+1) )
	 SYSCTL_PLLFREQ0_R= (SYSCTL_PLLFREQ0_R & ~(SYSCTL_PLLFREQ0_MFRAC_M | SYSCTL_PLLFREQ0_MINT_M)) |  // clear fields
											(MFRAC << SYSCTL_PLLFREQ0_MFRAC_S) | (MINT << SYSCTL_PLLFREQ0_MINT_S);			 // MINT = 96, MFRAC = 0
	 SYSCTL_PLLFREQ1_R= (SYSCTL_PLLFREQ1_R & ~(SYSCTL_PLLFREQ1_Q_M | SYSCTL_PLLFREQ1_N_M)) |				 // clear fields
											(Q << SYSCTL_PLLFREQ1_Q_S) | (N << SYSCTL_PLLFREQ1_N_S);										 // Q = 0, N = 4 
	 SYSCTL_PLLFREQ0_R|= SYSCTL_PLLFREQ0_PLLPWR;		 // Power-on the PLL 
	 SYSCTL_RSCLKCFG_R|= SYSCTL_RSCLKCFG_NEWFREQ;    // Lock in the new calculations changes 	 
	 // set the MEMTIM0 for FLASH memory and EEPROM
	 // CPU Frequency range (f) in MHz : Time Period Range (t) in ns : FBCHT/EBCHT : FBCE/EBCE : FWS/EWS

	 // 			     16 											  62.5													0x0 				1 					0x0
	 //   16  < f <= 40						 		  62.5 > t = 25 											0x2 				0	 					0x1
	 // 	40  < f <= 60						 	 			25 > t = 16.67 										0x3 				0 					0x2
	 // 	60  < f <= 80							   16.67 > t = 12.5 										0x4 				0 					0x3
	 // 	80  < f <=100						 			12.5 > t = 10 											0x5 				0 					0x4
	 // 	100 < f <=120							 			10 > t = 8.33 										0x6 				0 					0x5
	 
#define FBCHT_60MHZ			0x3					
#define EBCHT_60MHZ			0x3
#define FWS_60MHZ				0x2
#define EWS_60MHZ				0x2

#define FBCHT_80MHZ			0x4					
#define EBCHT_80MHZ			0x4
#define FWS_80MHZ				0x3
#define EWS_80MHZ				0x3

#define FBCHT_100MHZ		0x5					
#define EBCHT_100MHZ		0x5
#define FWS_100MHZ			0x4
#define EWS_100MHZ			0x4

#define FBCHT_120MHZ		0x6					
#define EBCHT_120MHZ		0x6
#define FWS_120MHZ			0x5
#define EWS_120MHZ			0x5

#define FBCE							0
#define EBCE							0

// i intended to change frequency only in those ranges (60-80-120)MHz 
	if(SYSCLK <= 40000000)
	{
    // FBCHT/EBCHT = 2, FBCE/EBCE = 0, FWS/EWS = 1
    SYSCTL_MEMTIM0_R = (SYSCTL_MEMTIM0_R&~0x03EF03EF) | ((0x2 << 22) | 
																												(0x0 << 21) | (0x1 << 16) | 
																												(0x2 << 6)  | (0x0 << 5)  | 
																												(0x1));
  } 
	else if(SYSCLK <= 60000000)
	{
    // FBCHT/EBCHT = 3, FBCE/EBCE = 0, FWS/EWS = 2
       SYSCTL_MEMTIM0_R = (SYSCTL_MEMTIM0_R&~0x03EF03EF) | ((0x2 << 22) | 
																												(0x0 << 21) | (0x1 << 16) | 
																												(0x2 << 6)  | (0x0 << 5)  | 
																												(0x2));
  } 
	else if(SYSCLK <= 80000000)
	{
    // FBCHT/EBCHT = 4, FBCE/EBCE = 0, FWS/EWS = 3
       SYSCTL_MEMTIM0_R = (SYSCTL_MEMTIM0_R&~0x03EF03EF) | ((0x2 << 22) | 
																												(0x0 << 21) | (0x1 << 16) | 
																												(0x2 << 6)  | (0x0 << 5)  | 
																												(0x3));
  } 
	else if(SYSCLK <= 100000000)
	{
    // FBCHT/EBCHT = 5, FBCE/EBCE = 0, FWS/EWS = 4
       SYSCTL_MEMTIM0_R = (SYSCTL_MEMTIM0_R&~0x03EF03EF) | ((0x2 << 22) | 
																												(0x0 << 21) | (0x1 << 16) | 
																												(0x2 << 6)  | (0x0 << 5)  | 
																												(0x4));
  } else if(SYSCLK <= 120000000)
	{
    // FBCHT/EBCHT = 6, FBCE/EBCE = 0, FWS/EWS = 5
       SYSCTL_MEMTIM0_R = (SYSCTL_MEMTIM0_R&~0x03EF03EF) | ((0x2 << 22) | 
																												(0x0 << 21) | (0x1 << 16) | 
																												(0x2 << 6)  | (0x0 << 5)  | 
																												(0x5));
  }
	 // wait until the PLL unlocked or time out reached 
	 // (time out check is a must to avoid an infinite loop in case failed)
	 while ( 0 == (SYSCTL_PLLSTAT_R & SYSCTL_PLLSTAT_LOCK) && (timeOut++ < 0xFFFF) ) { }		 
   if (0xFFFF == timeOut)  // if time out reached
			 return; // do not Power the system clock source from PLL
	 
	 SYSCTL_RSCLKCFG_R= (SYSCTL_RSCLKCFG_R & ~SYSCTL_RSCLKCFG_PSYSDIV_M) | PSYSDIV;   // set the PSYSDIV
	 SYSCTL_RSCLKCFG_R|= SYSCTL_RSCLKCFG_USEPLL | 	// change the system clock source for Run/sleep modes to the PLL 
											 SYSCTL_RSCLKCFG_MEMTIMU;		// Update the memory timing with the new changes  
}
