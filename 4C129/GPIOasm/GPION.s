; GPIO.s
; Runs on TM4C1294
; Mo'meN Ali
; September 1, 2014

; PF0 is an output to positive logic LED4
; PF4 is an output to positive logic LED3
; PN0 is an output to positive logic LED2
; PN1 is an output to positive logic LED1

GPIO_PORTN_DATA_R             EQU 0x4006403C
GPIO_PORTN_DIR_R              EQU 0x40064400
GPIO_PORTN_AFSEL_R            EQU 0x40064420
GPIO_PORTN_DEN_R              EQU 0x4006451C
GPIO_PORTN_AMSEL_R            EQU 0x40064528
GPIO_PORTN_PCTL_R             EQU 0x4006452C
SYSCTL_RCGCGPIO_R             EQU 0x400FE608
SYSCTL_RCGCGPIO_R12           EQU 0x00001000  ; GPIO Port N Run Mode Clock Gating Control
SYSCTL_PRGPIO_R               EQU 0x400FEA08
SYSCTL_PRGPION                EQU 0x00001000  ; GPIO Port N Peripheral Ready
ONEHZDELAY                    EQU 20000000    ; delay ~0.5 sec at 120 MHz (3.75 sec at 16 MHz; 0.78125 sec at 76.8 MHz)
TENHZDELAY                    EQU 2000000     ; delay ~0.05 sec at 120 MHz (0.375 sec at 16 MHz; 0.078125 sec at 76.8 MHz)	
	
				AREA    |.text|, CODE, READONLY, ALIGN=2
				THUMB
				EXPORT  Start
				IMPORT	PLL_Init
					
Start
			;BL PLL_Init
			BL GPIO_Init
			LDR R0, =GPIO_PORTN_DATA_R
mainLoop			
			MOV	R1, #1
			STR	R1, [R0]
			LDR R2, =TENHZDELAY
			BL	Delay
			MOV R1, #2
			STR R1, [R0]
			LDR R2, =TENHZDELAY
			BL	Delay
			MOV	R1, #3
			STR	R1, [R0]
			LDR R2, =TENHZDELAY
			BL	Delay
			B	mainLoop
					
GPIO_Init
			LDR R0, =SYSCTL_RCGCGPIO_R   	; r0= &SYSCTL_RCGCGPIO_R
			MOV	R1, #SYSCTL_RCGCGPIO_R12	; PORTN
			STR	R1, [R0]					; SYSCTL_RCGCGPIO_R= SYSCTL_RCGCGPIO_R12
			; make sure the clock has stablizied
			LDR	R0, =SYSCTL_PRGPIO_R			
			STR	R1, [R0]					; r1= SYSCTL_RCGCGGPIO_R			
			; while( 0 == (SYSCTL_PRGPION & SYSCTL_RCGCGPIO_R) ) {}
loop			
			ANDS R1, #SYSCTL_PRGPION     	
			BEQ	loop
			; GPIO_PORTN_PCTL_R&= ~0x0000FFFF
			LDR R0, =GPIO_PORTN_PCTL_R
			LDR R1, [R0]
			BIC R1, #0x000000FF
			BIC R1, #0x0000FF00        ; R1 = R1 & ~0x0000FF
			STR R1, [R0]
			; GPIO_PORTN_AMSEL_R&= 0
			LDR	R0, =GPIO_PORTN_AMSEL_R	
			MOV R1, #0
			STR R1, [R0]	
			; GPIO_PORTN_AFSEL_R&= 0
			LDR R0, =GPIO_PORTN_AFSEL_R
			STR R1, [R0]
			; GPIO_PORTN_DIR_R|= 0x0F
			LDR R0, =GPIO_PORTN_DIR_R
			LDR R1, [R0]
			ORR R1, #0x0F
			STR R1, [R0]
			; GPIO_PORTN_DEN_R|= 0x0F
			LDR R0, =GPIO_PORTN_DEN_R
			LDR R1, [R0]
			ORR R1, #0x0F
			STR R1, [R0]
			BX	LR
			
Delay		
			SUBS    R2, #1
			BHS     Delay
			BX      LR
			
				ALIGN
				END	