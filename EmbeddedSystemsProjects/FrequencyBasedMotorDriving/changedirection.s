
;***************************************************************	
; Program section		
			AREA    	subroutine, READONLY, CODE
			THUMB
			EXPORT  	changedirection1	; Make available
;***************************************************************
;LABEL				DIRECTIVE	VALUE		COMMENT
ROTATION_POSITION	EQU		0x20001000
GPIO_PORTF_MIS 		EQU 	0x40025418
GPIO_PORTF_ICR 		EQU 	0x4002541C	
;***************************************************************
;LABEL				DIRECTIVE	VALUE		COMMENT
changedirection1	PROC
					PUSH		{R5,R10,R0}
					LDR			R5,=GPIO_PORTF_MIS
					LDR			R10,[R5]
					LDR			R5,=ROTATION_POSITION
					CMP			R10,#0x10
					BEQ			CCW1
					CMP			R10,#0x01
					BEQ			CCW
					B			ENDPP
CCW1				mov			r10,#1
					STR			R10,[R5,#4]
					B			ENDPP
CCW					mov			r10,#2
					STR			R10,[R5,#4]
ENDPP				LDR			R0,=GPIO_PORTF_ICR
					MOV			R1,#0x11
					STR			R1,[R0]
					POP			{R5,R10,R0}
					BX			LR
					ENDP
;***************************************************************
; End of the program  section
;***************************************************************
;LABEL      DIRECTIVE       VALUE                           COMMENT
			ALIGN
			END