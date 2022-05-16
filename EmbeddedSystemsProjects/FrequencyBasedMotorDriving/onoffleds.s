;***************************************************************	
; Program section		
			AREA    	subroutine, READONLY, CODE
			THUMB
			IMPORT		CONVRT
			IMPORT		changespeed
			EXPORT  	onoffleds	; Make available
;***************************************************************
GPIO_PORTF_DATA_GREEN			EQU 	0x40025020
GPIO_PORTF_DATA_RED				EQU 	0x40025008
GPIO_PORTF_DATA_BLUE			EQU 	0x40025010

NVIC_ST_CTRL 					EQU 	0xE000E010
FREQLOCATION					EQU		0x20001900
AMPLOCATION						EQU		0x2000190A
AMPTHRESH						EQU		0x20001914
HIGHTHRESH						EQU		0x2000191E
LOWTHRESH						EQU		0x20001928
;***************************************************************
;LABEL		DIRECTIVE	VALUE		COMMENT
onoffleds	PROC
			PUSH		{R1,R2,R4,R9,R7,LR}	
			PUSH		{R7}
			LDR			R4,=AMPLOCATION
			MOV32		R1,#200000
			UDIV		R7,R7,R1
			STR			R7,[R4]
			BL			CONVRT
			LDR			R4,=AMPTHRESH
			MOV			R7,#0x28
			STR			R7,[R4]
			BL			CONVRT
			;;
			LDR			R4,=LOWTHRESH
			MOV			R7,#0x190
			STR			R7,[R4]
			BL			CONVRT
			;;
			;;
			LDR			R4,=HIGHTHRESH
			MOV			R7,#0x320
			STR			R7,[R4]
			BL			CONVRT
			POP			{R7}
			LDR			R1,=0x0700000
			CMP			R7,R1
			BLT			OFFF
			MOV			R3,#2000
			MOV			R4,#256
			MUL			R3,R9,R3
			UDIV		R3,R3,R4
			LDR			R4,=FREQLOCATION
			STR			R3,[R4]
			BL			CONVRT
;			LDR			R4,=AMPLOCATION
;			MOV32		R1,#200000
;			UDIV		R7,R7,R1
;			STR			R7,[R4]
;			BL			CONVRT
			;;
			
			;;
			BL			changespeed
			MOV			R4,#0x320
			CMP			R3,R4
			BHI			BLUE
			MOV			R4,#0x190
			CMP			R3,R4
			BLT			RED
			B			GREEN
BLUE		LDR			R1,=GPIO_PORTF_DATA_RED
			MOV			R2,#0
			STR			R2,[R1]
			LDR			R1,=GPIO_PORTF_DATA_GREEN
			MOV			R2,#0
			STR			R2,[R1]
			LDR			R1,=GPIO_PORTF_DATA_BLUE
			MOV			R2,#0xF
			STR			R2,[R1]
			B			DONE
RED			LDR			R1,=GPIO_PORTF_DATA_BLUE
			MOV			R2,#0
			STR			R2,[R1]
			LDR			R1,=GPIO_PORTF_DATA_GREEN
			MOV			R2,#0
			STR			R2,[R1]
			LDR			R1,=GPIO_PORTF_DATA_RED
			MOV			R2,#0xF
			STR			R2,[R1]
			B			DONE
GREEN		LDR			R1,=GPIO_PORTF_DATA_BLUE
			MOV			R2,#0
			STR			R2,[R1]
			LDR			R1,=GPIO_PORTF_DATA_RED
			MOV			R2,#0
			STR			R2,[R1]
			LDR			R1,=GPIO_PORTF_DATA_GREEN
			MOV			R2,#0xF
			STR			R2,[R1]
			B			DONE
OFFF		LDR			R1,=GPIO_PORTF_DATA_BLUE
			MOV			R2,#0
			STR			R2,[R1]
			LDR			R1,=GPIO_PORTF_DATA_RED
			MOV			R2,#0
			STR			R2,[R1]
			LDR			R1,=GPIO_PORTF_DATA_GREEN
			MOV			R2,#0x0
			STR			R2,[R1]
DONE		POP			{R1,R2,R4,R9,R7,LR}
			BX			LR
			ENDP
;***************************************************************
; End of the program  section
;***************************************************************
;LABEL      DIRECTIVE       VALUE                           COMMENT
			ALIGN
			END