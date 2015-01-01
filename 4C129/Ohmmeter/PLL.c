// PLL configuration
// TM4C129
// By Mo'meN Ali
// August 31, 2014

#include <stdint.h>
#include "PLL.h"

#define SYSCTL_ALTCLKCFG_PIOSC	 0x0
#define SYSCTL_ALTCLKCFG_R      				( *((volatile uint32_t *)0x400FE138) )	
#define SYSCTL_MOSCCTL_R  							( *((volatile uint32_t *)0x400FE07C) )
#define SYSCTL_MOSCCTL_PWRDN						0x00000008
#define SYSCTL_MOSCCTL_NOXTAL						0x00000004
#define SYSCTL_RSCLKCFG_R								( *((volatile uint32_t *)0x400FE0B0) )
#define SYSCTL_RSCLKCFG_PLLEN						0x10000000	
#define SYSCTL_RIS_R										( *((volatile uint32_t *)0x400FE050) )
#define SYSCTL_RIS_MOSCPUPRIS						0x00000100	
#define SYSCTL_RSCLKCFG_OSCSRC_MOSC			0x00300000
#define SYSCTL_RSCLKCFG_PLLSRC_MOSC     0x03000000  // MOSC is the PLL input clock
                                            // source
#define SYSCTL_RSCLKCFG_MEMTIMU					0x80000000
#define SYSCTL_RSCLKCFG_NEWFREQ 				0x40000000  
#define SYSCTL_DSCLKCFG_R								( *((volatile uint32_t *)0x400FE144) )
#define SYSCTL_DSCLKCFG_DSOSCSRC_MOSC		0x00300000			// MOSC runs and does not disable itself in run,
																												// sleep, and deep-sleep modes regardless ofthe fact that
																												// the PWRDN bit is set.
#define SYSCTL_PLLFREQ0_R								( *((volatile uint32_t *)0x400FE160) )	
#define SYSCTL_PLLFREQ1_R								( *((volatile uint32_t *)0x400FE164) )
#define SYSCTL_PLLFREQ0_PLLPWR					0x00800000
// The SysClk paramters values
#define FXTAL														25
#define MINT														96
#define MFRAC														0
#define Q																0
#define N																4
#define PSYSDIV													3
#define SYSCTL_MEMTIM0_R								( *((volatile uint32_t *)0x400FE0C0) )
// Timing paramter values
#define FBCE														0x00000000
#define FBCHT														0x00000180
#define EBCE														0x00000000
#define EBCHT														0x01800000
#define FWS															0x00000005
#define EWS															0x00050000
#define SYSCTL_PLLSTAT_R								( *((volatile uint32_t *)0x400FE168) )
#define SYSCTL_PLLSTAT_LOCK							0x00000001
#define SYSCTL_ALTCLKCFG_ALTCLK_PIOSC		0

#define Q            0
#define N            4  // chosen for reference frequency within 4 to 30 MHz
#define MINT        96  // 480,000,000 = (25,000,000/(0 + 1)/(4 + 1))*(96 + (0/1,024))
#define MFRAC        0  // zero to reduce jitter

// SysClk = fVCO/(PSYSDIV + 1)
// fVCO = (fIN*MDIV)
// fIN = fXTAL/((Q+1)(N+1)) or fPIOSC/((Q+1)(N+1))
// MDIV = MINT + (MFRAC/1024)
// The Q and N values are programmed in the PLLFREQ1 register. Note that to reduce jitter, MFRAC
// should be programmed to 0x0.

// to set the SysClk to 120 MHz, you need to set the fVCO to 480 MHz and the PSYSDIV to 4  


void PLL_Init(void)
{	
  SYSCTL_ALTCLKCFG_R = (SYSCTL_ALTCLKCFG_R & ~0xF) + SYSCTL_ALTCLKCFG_ALTCLK_PIOSC;  // configure the PIOSC as the alternate clock source
	SYSCTL_RSCLKCFG_R&= ~SYSCTL_RSCLKCFG_PLLEN;     // make sure the is Disabled while configuring
	SYSCTL_MOSCCTL_R&= ~(SYSCTL_MOSCCTL_PWRDN | SYSCTL_MOSCCTL_NOXTAL);
	while( 0 == (SYSCTL_RIS_R & SYSCTL_RIS_MOSCPUPRIS) ) {}
	SYSCTL_RSCLKCFG_R= (SYSCTL_RSCLKCFG_R & ~0x00F00000) | SYSCTL_RSCLKCFG_OSCSRC_MOSC;  // Set the main clock as the clock source
	SYSCTL_PLLFREQ0_R= (SYSCTL_PLLFREQ0_R & ~0x000FFFFF) | (MFRAC << 10) | MINT;				 // Set the SysClk paramter
	SYSCTL_PLLFREQ1_R= (SYSCTL_PLLFREQ1_R & ~0x00001F0F) | (Q << 8) | N;    
	// turn on power to PLL		
  SYSCTL_PLLFREQ0_R |= SYSCTL_PLLFREQ0_PLLPWR;      // THIS BIT IS SO CRITICAL WITHOUT IT PLL WILL TIMEOUT 
  //SYSCTL_RSCLKCFG_R |= SYSCTL_RSCLKCFG_NEWFREQ;      // lock in register changes
  // Set the timing parameters to the main Flash and EEPROM memories, which
  // depend on the system clock frequency.  See Table 5-12 in datasheet.	
	// in this module only set the PLL to 120 MHz.		
	SYSCTL_MEMTIM0_R = (SYSCTL_MEMTIM0_R & ~0x03EF03EF) | FBCE | FBCHT | EBCE | EBCHT | FWS | EWS;	
	while( 0 == (SYSCTL_PLLSTAT_R & SYSCTL_PLLSTAT_LOCK)) {}

	SYSCTL_RSCLKCFG_R= SYSCTL_RSCLKCFG_R | PSYSDIV    // Set the SysClk PSYSDIV= 4
																			 | SYSCTL_RSCLKCFG_PLLEN      // Enable the PLL
																			 | SYSCTL_RSCLKCFG_MEMTIMU;		// Enable the timing register
	
}
