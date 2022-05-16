
;***************************************************************	
; Program section		
			AREA    	subroutine, READONLY, CODE
			THUMB
			EXPORT  	changespeed	; Make available
;***************************************************************
;LABEL				DIRECTIVE	VALUE		COMMENT
TIMER0_TAILR		EQU 0x40030028
CONSTANT			EQU	0x1F40
SLOPE				EQU	0x6
;***************************************************************
;LABEL			DIRECTIVE	VALUE		COMMENT
changespeed		PROC
				PUSH	{R0,R2,R3,R4}
				CMP		R3,#1000
				BHI		MAXSPEED
				CMP		R3,#200
				BLT		ENDDD
				LDR		R0,=CONSTANT
				LDR		R4,=SLOPE
				MUL		R2,R4,R3
				SUB		R2,R0,R2
CHANGE			LDR 	R1, =TIMER0_TAILR ; initialize match clocks
				STR 	R2, [R1]
				B		ENDDD
MAXSPEED		MOV		R2,#0x7D0
				B		CHANGE
ENDDD			POP		{R0,R2,R3,R4}
				BX		LR
				ENDP
;***************************************************************
; End of the program  section
;***************************************************************
;LABEL      DIRECTIVE       VALUE                           COMMENT
			ALIGN
			END