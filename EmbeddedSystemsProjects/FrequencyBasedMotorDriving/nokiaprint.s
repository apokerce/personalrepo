;***************************************************************	
; Program section		
			AREA    	subroutine, READONLY, CODE
			THUMB
			IMPORT		delay1s	
			IMPORT		CONVRT
			EXPORT  	nokiaprint	; Make available
;***************************************************************
;LABEL				DIRECTIVE	VALUE		COMMENT
SYSCTL_RCGCSSI 		EQU 		0x400FE61C
SYSCTL_PRSSI		EQU 		0x400FEA1C
SSICR1 				EQU 		0x40008004
SSICPSR				EQU 		0x40008010
SSICONTROL0			EQU			0x40008000
SSI0_DR				EQU			0x40008008
SSISR				EQU			0x4000800C		; status register

TIMER1_ICR			EQU 		0x40031024 

GPIO_PORTA_DIR 		EQU 		0x40004400 ; Port direction
GPIO_PORTA_AFSEL	EQU 		0x40004420 ; Alternate function select
GPIO_PORTA_DEN 		EQU 		0x4000451C ; Enable digital
GPIO_PORTA_AMSEL 	EQU 		0x40004528 ; Analog mode select
GPIO_PORTA_PCTL 	EQU			0x4000452C ; Port Control
GPIO_PORTA_PUR 		EQU 		0x40004510
GPIO_PORTA_LOCK		EQU			0x40004520		
GPIO_PORTA_CR		EQU			0x40004524	
GPIO_PORTA_DATA		EQU			0x400043FC

FREQLOCATION		EQU			0x20001900
	
NUMBER_ZERO			DCB			0x00
					DCB 		0x3e
					DCB 		0x51
					DCB 		0x49
					DCB 		0x45
					DCB			0x3e
					DCB			0x00
					
NUMBER_ONE			DCB			0x00
					DCB			0x00
					DCB			0x42
					DCB			0x7f
					DCB			0x40
					DCB			0x00
					DCB			0x00
					
NUMBER_TWO			DCB			0x00
					DCB			0x42
					DCB			0x61
					DCB			0x51
					DCB			0x49
					DCB			0x46
					DCB			0x00
					
NUMBER_THREE		DCB			0x00
					DCB			0x21
					DCB			0x41
					DCB			0x45
					DCB			0x4B
					DCB			0x31
					DCB			0x00
					
NUMBER_FOUR			DCB			0x00
					DCB			0x18
					DCB			0x14
					DCB			0x12
					DCB			0x7F
					DCB			0x10
					DCB			0x00
					
NUMBER_FIVE			DCB			0x00
					DCB			0x27
					DCB			0x45
					DCB			0x45
					DCB			0x45
					DCB			0x39
					DCB			0x00
					
NUMBER_SIX			DCB			0x00
					DCB			0x3C
					DCB			0x4A
					DCB			0x49
					DCB			0x49
					DCB			0x30
					DCB			0x00	
			
NUMBER_SEVEN		DCB			0x00
					DCB			0x01
					DCB			0x71
					DCB			0x09
					DCB			0x05
					DCB			0x03
					DCB			0x00	
					
NUMBER_EIGHT		DCB			0x00
					DCB			0x36
					DCB			0x49
					DCB			0x49
					DCB			0x49
					DCB			0x36
					DCB			0x00	
					
NUMBER_NINE			DCB			0x00
					DCB			0x06
					DCB			0x49
					DCB			0x49
					DCB			0x29
					DCB			0x1e
					DCB			0x00
;***************************************************************
;LABEL			DIRECTIVE	VALUE		COMMENT
nokiaprint		PROC
				PUSH	{R0,R1,R2,R3,R4,R5,R6,LR}
				
; D/C = 1 sending data
				LDR 	R1,=GPIO_PORTA_DATA 
				LDR 	R0,[R1]
				ORR 	R0,R0,#0x40
				STR 	R0,[R1]
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;				
				MOV		R6,#3
				MOV		R2,#7
				MOV		R8,#5
				MOV		R9,#0
				LDR 	R1,=SSI0_DR 
				LDR		R5,=FREQLOCATION
PRINT			MOV		R6,#3
				MOV		R2,#7
LOOP2			LDR		R0,=NUMBER_ZERO
				LDRB	R4,[R5],#1
				CMP		R4,#0xFF
				BEQ		DONE
				ADD		R9,#1
				MUL		R4,R4,R2
				ADD		R0,R0,R4
				
LOOP			LDRB	R3,[R0],#1
				STR		R3,[R1]
				SUBS	R2,#1
				BNE		LOOP
CHECKDONE0		LDR		R7,=SSISR
				LDR		R4,[R7]
				CMP		R4,#0x0F	; transmitter FIFO is empty or not
				BNE		CHECKDONE0
				MOV		R2,#7
				SUBS	R6,#1
				BNE		LOOP2
DONE				
				MOV		R3,#0
				MOV		R6,#12
				MOV		R2,#7
				SUB		R6,R6,R9
				MOV		R9,#0
			
RETURN			MOV		R3,#0	
LOOP78			STR		R3,[R1]
				SUBS	R2,#1
				BNE		LOOP78
CHECKDONE1		LDR		R7,=SSISR
				LDR		R4,[R7]
				CMP		R4,#0x0F	; transmitter FIFO is empty or not
				BNE		CHECKDONE1
				SUBS	R6,#1
				MOV		R2,#7
				BNE		RETURN
				ADD		R5,#7
				SUBS	R8,#1
				BNE		PRINT
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;				

				
							
						
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
				MOV		R3,#0
				MOV		R6,#12
				MOV		R2,#7
				
LOOP1			STR		R3,[R1]
				SUBS	R2,#1
				BNE		LOOP1

CHECKDONE2		LDR		R7,=SSISR
				LDR		R4,[R7]
				CMP		R4,#0x0F	; transmitter FIFO is empty or not
				BNE		CHECKDONE2
				
				MOV		R2,#7
				SUBS	R6,#1
				BNE		LOOP1
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;				
				
;				STR		R3,[R1]
CHECKDONE3		LDR		R7,=SSISR
				LDR		R4,[R7]
				CMP		R4,#0x0F	; transmitter FIFO is empty or not
				BNE		CHECKDONE3
				
; stop transmittin ( D/C = 0)
				LDR 	R1,=GPIO_PORTA_DATA 
				LDR 	R0,[R1]
				BIC 	R0,R0,#0x40
				STR 	R0,[R1]
				LDR			R1,=TIMER1_ICR
				MOV			R0,#0x1
				STR			R0,[R1]
				POP		{R0,R1,R2,R3,R4,R5,R6,LR}
				BX		LR
				ENDP				
				
;***************************************************************
; End of the program  section
;***************************************************************
;LABEL      DIRECTIVE       VALUE                           COMMENT
			ALIGN
			END					