;***************************************************************	
; Program section		
			AREA    	subroutine, READONLY, CODE
			THUMB
			EXPORT  	open_port	; Make available

;***************************************************************
;LABEL				DIRECTIVE	VALUE		COMMENT
GPIO_PORTF_DIR 		EQU 0x40025400
GPIO_PORTF_AFSEL 	EQU 0x40025420
GPIO_PORTF_DEN 		EQU 0x4002551C
SYSCTL_RCGCGPIO 	EQU 0x400FE608
GPIO_PORTF_LOCK 	EQU 0x40025520
GPIO_PORTF_CR 		EQU 0x40025524
GPIO_PORTF_PUR 		EQU 0x40025510
GPIO_PORTF_PDR 		EQU 0x40025514
GPIO_PORTF_IS 		EQU 0x40025404
GPIO_PORTF_IBE		EQU 0x40025408
GPIO_PORTF_IEV		EQU 0x4002540C
GPIO_PORTF_IM		EQU 0x40025410
GPIO_PORTB_DIR 		EQU 0x40005400
GPIO_PORTB_AFSEL 	EQU 0x40005420
GPIO_PORTB_DEN 		EQU 0x4000551C
GPIO_PORTB_PUR 		EQU 0x40005510 

NVIC_EN0			EQU 0xE000E100 
NVIC_PRI7			EQU 0xE000E41C 
;***************************************************************
;LABEL		DIRECTIVE	VALUE		COMMENT
open_port	PROC
			LDR			R1,=SYSCTL_RCGCGPIO
			LDR			R0,[R1]
			ORR			R0,R0,#0x2F
			STR			R0,[R1]
			NOP
			NOP
			NOP
			LDR			R0,=GPIO_PORTF_LOCK
			LDR			R1,=0x4C4F434B
			STR			R1,[R0]
			LDR			R0,=GPIO_PORTF_CR
			MOV			R1,#0x01
			STR			R1,[R0]		
			NOP
			NOP
			NOP
			LDR			R1,=GPIO_PORTF_DIR
			LDR			R0,[R1]
			BIC			R0,#0x11
			ORR			R0,R0,#0x0E
			STR			R0,[R1]
			LDR			R1,=GPIO_PORTF_AFSEL
			LDR			R0,[R1]
			BIC			R0,#0xFF
			STR			R0,[R1]
			LDR 		R1,=GPIO_PORTF_DEN
			LDR 		R0,[R1]
			ORR 		R0,#0xFF
			STR 		R0,[R1]
			LDR			R0,=GPIO_PORTF_PUR
			MOV			R1,#0x11
			STR			R1,[R0]
			LDR			R0,=GPIO_PORTF_PDR
			MOV			R1,#0x0E
			STR			R1,[R0]			
			;Make buttons edge sensitive
			LDR			R1,=GPIO_PORTF_IS
			LDR			R0,[R1]
			BIC			R0,#0x11
			STR			R0,[R1]
			;LET buttons to be controlled by IEV
			LDR			R1,=GPIO_PORTF_IBE
			LDR			R0,[R1]
			BIC			R0,#0x11
			STR			R0,[R1]
			;Make to be falling edge sensitive
			LDR			R1,=GPIO_PORTF_IEV
			LDR			R0,[R1]
			BIC			R0,#0x11
			STR			R0,[R1]
			;The interrupt from the corresponding pin is sent to the interrupt controller
			LDR			R1,=GPIO_PORTF_IM
			LDR			R0,[R1]
			ORR			R0,#0x11
			STR			R0,[R1]
			
			;set priority
			LDR 		 R0, =NVIC_PRI7
			LDR 		 R1, [R0]
			AND			 R1, R1, #0xFF0FFFFF 
			ORR 		 R1, R1, #0x00200000 
			STR			 R1, [R0]
						
			;Enable interrupt
			LDR			R0, =NVIC_EN0
			MOV32 		R1, #0x40000000 
			STR 		R1, [R0]
			
			LDR			R1,=GPIO_PORTB_DIR
			LDR			R0,[R1]
			BIC			R0,#0xFF
			ORR			R0,R0,#0xF0
			STR			R0,[R1]
			LDR			R1,=GPIO_PORTB_AFSEL
			LDR			R0,[R1]
			BIC			R0,#0xFF
			STR			R0,[R1]
			LDR 		R1,=GPIO_PORTB_DEN
			LDR 		R0,[R1]
			ORR 		R0,#0xFF
			STR 		R0,[R1]
			LDR			R0,=GPIO_PORTB_PUR
			MOV			R1,#0xFF
			STR			R1,[R0]
			
			BX			LR
			ENDP
;***************************************************************
; End of the program  section
;***************************************************************
;LABEL      DIRECTIVE       VALUE                           COMMENT
			ALIGN
			END