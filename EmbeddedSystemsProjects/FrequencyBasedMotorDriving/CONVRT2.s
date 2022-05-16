;*************************************************************** 
; CONVRT.s  
; Take the HEX value in R9 and convert
; to decimal without 0's and store starting adress from R5.              
;***************************************************************	
FIRST	   		EQU	    	0x20001480	
Null			EQU			0x04
;***************************************************************
; Directives - This Data Section is part of the code
; It is in the read only section  so values cannot be changed.
;***************************************************************
;LABEL		DIRECTIVE	VALUE		COMMENT
            AREA        routines, READONLY, CODE
            THUMB
			IMPORT		OutStr
			EXPORT 		CONVRTBCD
;***************************************************************
; Program section					      
;***************************************************************
;LABEL		DIRECTIVE	VALUE		COMMENT
CONVRTBCD	PROC
			PUSH		{R1,R2,R5,R7,R6,R3}
			MOV			R12,LR
			MOV			R6,#10
			MOV			R1,#0
			MOV			R9,R3
			LDR			R5,=FIRST
			CMP			R10,#1
			BEQ			NEGATIVE
			B			DIVISION
NEGATIVE	MOV			R3,#0x2D
			STRB		R3,[R5],#1
DIVISION	UDIV		R2,R9,R6
			CMP			R2,#0
			BEQ			EXIT
			MUL			R7,R2,R6
			SUB			R7,R9,R7
			ADD			R1,R1,#1
			MOV			R9,R2
			PUSH		{R7}
			B			DIVISION
EXIT		PUSH		{R9}
			ADD			R1,R1,#1
			MOV			R2,#0
			CMP			R1,#2
			BEQ			MUL2
			B			DONE
MUL2		PUSH		{R2}
			ADD			R1,R1,#1
DONE		POP			{R9}
			ADD			R9,R9,#48
			STRB		R9,[R5],#1
			ADD			R2,R2,#1
			CMP			R2,#1
			BEQ			ADDDOT
CONT11		SUBS		R1,R1,#1	
			BNE			DONE
			MOV			R3,#0xD
			STRB		R3,[R5],#1
			LDR			R3,=Null	;null character
			STRB		R3,[R5]
			LDR			R5,=FIRST
			BL			OutStr
			POP			{R1,R2,R5,R7,R6,R3}
			MOV			LR,R12
			BX 			LR
ADDDOT		
			MOV			R3,#0x2E
			STRB		R3,[R5],#1
			B			CONT11
			ENDP
;***************************************************************
; End of the program  section
;***************************************************************
;LABEL      DIRECTIVE       VALUE                           COMMENT
			ALIGN
			END
