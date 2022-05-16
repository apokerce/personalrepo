;*************************************************************** 
; Clock_wise_subroutine Q1.s   
;***************************************************************	
; Program section		
			AREA    	subroutine, READONLY, CODE
			THUMB
			IMPORT		FINDMAX
			IMPORT		onoffleds
			EXTERN		arm_cfft_q15
			EXTERN		arm_cfft_sR_q15_len256
			EXPORT  	read_mic	; Make available
;***************************************************************
;LABEL				DIRECTIVE	VALUE		COMMENT
ADC0_RIS		EQU 		0x40038004 ; Interrupt status
ADC0_PSSI 		EQU 		0x40038028 ; Initiate sample
ADC0_SSFIFO3 	EQU			0x400380A8 ; Channel 3 results
ADC0_ISC		EQU			0x4003800C
FFT_ADRESS 		EQU			0x20000400
NVIC_ST_CTRL 	EQU 		0xE000E010
;***************************************************************
;LABEL			DIRECTIVE	VALUE		COMMENT
read_mic		PROC
				PUSH		{R0,R2,R3,R4,R6,R7,R8,R9,LR}
				LDR 		R3, =ADC0_RIS ; interrupt address
				LDR 		R4, =ADC0_SSFIFO3 ; result address
				LDR 		R2, =ADC0_PSSI ; sample sequence initiate address
				LDR 		R6,= ADC0_ISC
				; initiate sampling by enabling sequencer 3 in ADC0_PSSI

				LDR 		R0, [R2]
				ORR 		R0, R0, #0x08 ; set bit 3 for SS3
				STR 		R0, [R2]
				; check for sample complete (bit 3 of ADC0_RIS set)
Cont 			LDR 		R0, [R3]
				CMP			R0,#8
				BNE 		Cont
				
				LDR			R0,[R2]
				BIC			R0,R0,#0x8		; set SS3 bit to STOP sampling
				STR			R0,[R2]
				LDR 		R8,[R4] ;read dat
				
				MOV 		R0, #8
				STR			R0, [R6] ; clear flag
				
				MOV			R7,#0x610

				SUB			R8,R8,R7
				LSL			R8,#4
				STR			R8,[R5],#4
     	    	
				SUBS		R10,#1
				BEQ			FFT
				B			EXIT
				
FFT				LDR			R3,=NVIC_ST_CTRL
				MOV			R2,#0
				STR			R2,[R3]
				LDR			R0,=arm_cfft_sR_q15_len256
				LDR			R1,=FFT_ADRESS
				MOV			R2,#0
				MOV			R3,#1
				BL			arm_cfft_q15
				BL			FINDMAX	
				BL			onoffleds
EXIT			POP			{R0,R2,R3,R4,R6,R7,R8,R9,LR}
				BX			LR
				ENDP
;***************************************************************
; End of the program  section
;***************************************************************
;LABEL      DIRECTIVE       VALUE                           COMMENT
			ALIGN
			END