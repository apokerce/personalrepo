;*************************************************************** 
; Clock_wise_subroutine Q1.s   
;***************************************************************	
; Program section		
			AREA    	subroutine, READONLY, CODE
			THUMB
			EXPORT  	rotation	; Make available
;***************************************************************
;LABEL				DIRECTIVE	VALUE		COMMENT
PB_OUT 				EQU 		0x400053C0
ROTATION_POSITION	EQU			0x20001000
TIMER0_ICR			EQU 		0x40030024 ; Timer Interrupt Clear
;***************************************************************
;LABEL			DIRECTIVE	VALUE		COMMENT
rotation		PROC
				PUSH		{R10,R8,R9}
				LDR			R8,=ROTATION_POSITION
				LDR			R10,[R8,#4]
				CMP			R10,#1
				BEQ			CW
				CMP			R10,#2
				BEQ			CCW
CW				LDR			R8,=ROTATION_POSITION
				LDR			R9,[R8]
				LDR			R8,=PB_OUT
				STR			R9,[R8]
				LSL			R9,#1
				LDR			R8,=ROTATION_POSITION
				STR			R9,[R8]
				CMP			R9,#0x100
				BEQ			STARTOVER1
				LDR			R8,=TIMER0_ICR
				MOV			R9,#0x1
				STR			R9,[R8]
				POP			{R10,R8,R9}
				BX			LR
CCW				LDR			R8,=ROTATION_POSITION
				LDR			R9,[R8]
				LDR			R8,=PB_OUT
				STR			R9,[R8]
				LSR			R9,#1
				LDR			R8,=ROTATION_POSITION
				STR			R9,[R8]
				CMP			R9,#0x8
				BEQ			STARTOVER2
				LDR			R8,=TIMER0_ICR
				MOV			R9,#0x1
				STR			R9,[R8]
				POP			{R10,R8,R9}
				BX			LR
STARTOVER1		MOV			R9,#0x10
				LDR			R8,=ROTATION_POSITION
				STR			R9,[R8]
				LDR			R8,=TIMER0_ICR
				MOV			R9,#0x1
				STR			R9,[R8]
				POP			{R10,R8,R9}
				BX			LR
STARTOVER2		MOV			R9,#0x80
				LDR			R8,=ROTATION_POSITION
				STR			R9,[R8]
				LDR			R8,=TIMER0_ICR
				MOV			R9,#0x1
				STR			R9,[R8]
				POP			{R10,R8,R9}
				BX			LR
				ENDP
;***************************************************************
; End of the program  section
;***************************************************************
;LABEL      DIRECTIVE       VALUE                           COMMENT
			ALIGN
			END