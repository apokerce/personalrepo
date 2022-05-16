;***************************************************************	
; Program section		
			AREA    	subroutine, READONLY, CODE
			THUMB
			IMPORT		delay1s
			EXPORT  	SSI	; Make available
;***************************************************************
;LABEL				DIRECTIVE	VALUE		COMMENT
SYSCTL_RCGCSSI 		EQU 		0x400FE61C
SYSCTL_PRSSI		EQU 		0x400FEA1C
SSICR1 				EQU 		0x40008004
SSICPSR				EQU 		0x40008010
SSICONTROL0			EQU			0x40008000
SSI0_DR				EQU			0x40008008
SSISR				EQU			0x4000800C		; status register

GPIO_PORTA_DIR 		EQU 0x40004400 ; Port direction
GPIO_PORTA_AFSEL	EQU 0x40004420 ; Alternate function select
GPIO_PORTA_DEN 		EQU 0x4000451C ; Enable digital
GPIO_PORTA_AMSEL 	EQU 0x40004528 ; Analog mode select
GPIO_PORTA_PCTL 	EQU 0x4000452C ; Port Control
GPIO_PORTA_PUR 		EQU 0x40004510
GPIO_PORTA_LOCK		EQU	0x40004520		
GPIO_PORTA_CR		EQU	0x40004524	
GPIO_PORTA_DATA		EQU	0x400043FC
;***************************************************************
;LABEL			DIRECTIVE	VALUE		COMMENT
SSI				PROC
				PUSH	{LR}	
				LDR 	R1,=GPIO_PORTA_LOCK			
				LDR 	R0,=0x4C4F434B				
				STR 	R0,[R1]
			
				LDR 	R1,=GPIO_PORTA_CR			
				MOV 	R0,#0xFF			
				STR 	R0,[R1]
					
					
				LDR 	R1,=GPIO_PORTA_DIR 
				LDR 	R0,[R1]
				ORR 	R0,R0,#0xEF
				STR 	R0,[R1]
					
				LDR 	R1,=GPIO_PORTA_AFSEL 
				MOV 	R0,#0x3C
				STR 	R0,[R1]
					
				LDR 	R1,=GPIO_PORTA_DEN 
				MOV 	R0,#0xFF
				STR 	R0,[R1]
					
				LDR 	R1,=GPIO_PORTA_PCTL 
				MOV32 	R0,#0x00202200
				STR 	R0,[R1]	
				
				
				LDR			R0,=SYSCTL_RCGCSSI
				LDR			R1,[R0]
				ORR			R1,#0x1
				STR			R1,[R0]
				
				LDR 		R1,=SYSCTL_PRSSI
wait			LDR 		R0,[R1]
				AND 		R0,R0,#0x01
				CMP 		R0,#0x01
				BNE			wait				
				;DISABLE SSI
				LDR			R0,=SSICR1
				LDR			R1,[R0]
				ORR			R1,#0
				STR			R1,[R0]
				;arrrange prescale value
				LDR			R0,=SSICPSR
				MOV			R1,#10
				STR			R1,[R0]
				;ARRANGE BIT SIZE
				LDR			R0,=SSICONTROL0
				MOV			R1,#0x31C7
				STR			R1,[R0]
				;ENABLE SSI
				LDR 		R1,=SSICR1 
				LDR 		R0,[R1]
				ORR 		R0,R0,#0x02
				STR 		R0,[R1]
				LDR 	R1,=GPIO_PORTA_DATA 
				MOV	 	R0,#0x10
				STR 	R0,[R1]
; reset 
				LDR 	R0,[R1]
				ORR 	R0,R0,#0x80
				STR 	R0,[R1]
				BL		delay1s
				BIC 	R0,R0,#0x80
				STR 	R0,[R1]
				BL		delay1s
				ORR 	R0,R0,#0x80
				STR 	R0,[R1]
				BL		delay1s
					
; D/C = 0 sending command
				BIC 	R0,R0,#0x40
				STR 	R0,[R1]
				
; serial data				
				LDR 	R1,=SSI0_DR 
				MOV 	R0,#0x21
				STR 	R0,[R1] 
				NOP
				NOP
				MOV 	R0,#0xC0		;  	
				STR 	R0,[R1] 
				NOP
				NOP
				MOV 	R0,#0x07		; temperaturen control
				STR 	R0,[R1] 
				NOP
				NOP
				MOV 	R0,#0x13		; biassiing
				STR 	R0,[R1] 
				NOP
				NOP
				MOV 	R0,#0x20		; H=0
				STR 	R0,[R1] 
				NOP
				NOP
				MOV 	R0,#0x0C
				STR 	R0,[R1] 
				NOP
				NOP
				MOV 	R0,#0x40
				STR 	R0,[R1] 
				NOP
				NOP
				MOV 	R0,#0x80
				STR 	R0,[R1] 
				NOP
				NOP
				LDR		R5,=SSISR					
; sending command control
CMD_END			LDR		R4,[R5]
				BIC		R4,#0x1E
				CMP		R4,#0x01	; transmitter FIFO is empty or not
				BNE		CMD_END
					
				POP		{LR}
				BX		LR
				ENDP				
				
;***************************************************************
; End of the program  section
;***************************************************************
;LABEL      DIRECTIVE       VALUE                           COMMENT
			ALIGN
			END					