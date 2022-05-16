;*************************************************************** 
; CONVRT.s  
; Take the HEX value in R4 and convert
; to decimal without 0's and store starting adress from R5.              
;***************************************************************	
;*************************************************************** 
; EQU Directives
; These directives do not allocate memory
;***************************************************************
;LABEL		DIRECTIVE	VALUE		COMMENT
Null		EQU			0xFF
;***************************************************************
; Directives - This Data Section is part of the code
; It is in the read only section  so values cannot be changed.
;***************************************************************
;LABEL		DIRECTIVE	VALUE		COMMENT
            AREA        routines, READONLY, CODE
            THUMB
			EXPORT 		CONVRT
;***************************************************************
; Program section	
FREQLOCATION		EQU			0x20000900
;***************************************************************
;LABEL		DIRECTIVE	VALUE		COMMENT
CONVRT		PROC
			PUSH		{R1,R2,R3,R6,R7,R5,R4,LR}
			MOV			R6,#10
			MOV			R1,#0
			MOV			R5,R4
			LDR			R4,[R5]
DIVISION	UDIV		R2,R4,R6
			CMP			R2,#0
			BEQ			EXIT
			MUL			R7,R2,R6
			SUB			R7,R4,R7
			ADD			R1,R1,#1
			MOV			R4,R2
			PUSH		{R7}
			B			DIVISION
EXIT		PUSH		{R4}
			ADD			R1,R1,#1
			B			DONE
DONE		POP			{R4}
			STRB		R4,[R5],#1
			SUBS		R1,R1,#1	
			BNE			DONE
			LDR			R4,=Null
			STRB		R4,[R5]
			POP			{R1,R2,R3,R6,R7,R5,R4,LR}
			BX 			LR
			ENDP
;***************************************************************
; End of the program  section
;***************************************************************
;LABEL      DIRECTIVE       VALUE                           COMMENT
			ALIGN
			END
