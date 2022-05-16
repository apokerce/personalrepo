;***************************************************************	
; Program section		
			AREA    	subroutine, READONLY, CODE
			THUMB
			EXPORT  	start_adc	; Make available
;***************************************************************
;LABEL			DIRECTIVE	VALUE		COMMENT
; ADC Registers
RCGCADC EQU 0x400FE638 ; ADC clock register ; ADC0 base address EQU 0x40038000
ADC0_ACTSS EQU 0x40038000 ; Sample sequencer (ADC0 base address)

ADC0_IM EQU 0x40038008 ; Interrupt select
ADC0_EMUX EQU 0x40038014 ; Trigger select

ADC0_SSMUX3 EQU 0x400380A0 ; Input channel select
ADC0_SSCTL3 EQU 0x400380A4 ; Sample sequence control

ADC0_PC EQU 0x40038FC4 ; Sample rate
; GPIO Registers
RCGCGPIO EQU 0x400FE108 ; GPIO clock register ;PORT E base address EQU 0x40024000
PORTE_DEN EQU 0x4002451C ; Digital Enable
;PORTE_PCTL EQU 0x4002452C ; Alternate function select
PORTE_AFSEL EQU 0x40024420 ; Enable Alt functions
PORTE_AMSEL EQU 0x40024528 ; Enable analog
PORTE_DIR	EQU	0x40024400
;***************************************************************
;LABEL			DIRECTIVE	VALUE		COMMENT
start_adc		PROC
				LDR R1, =RCGCADC ; Turn on ADC clock
				LDR R0, [R1]
				ORR R0, R0, #0x01 ; set bit 0 to enable ADC0 clock
				STR R0, [R1]
				NOP
				NOP
				NOP ; Let clock stabilize
				LDR R1, =RCGCGPIO ; Turn on GPIO clock
				LDR R0, [R1]
				ORR R0, R0, #0x10 ; set bit 4 to enable port E clock
				STR R0, [R1]
				NOP
				NOP
				NOP ; Let clock stabilize
				; Setup GPIO to make PE3 input for ADC0
				LDR			R0,=PORTE_DIR
				LDR			R1,[R0]
				BIC			R1,#0x08
				STR			R1,[R0]
				; Enable alternate functions
				LDR R1, =PORTE_AFSEL
				LDR R0, [R1]
				ORR R0, R0, #0x08 ; set bit 3 to enable alt functions on PE3
				STR R0, [R1]
				; PCTL does not have to be configured
				; since ADC0 is automatically selected when
				; port pin is set to analog.
				; Disable digital on PE3
				LDR R1, =PORTE_DEN
				LDR R0, [R1]
				BIC R0, R0, #0x08 ; clear bit 3 to disable digital on PE3
				STR R0, [R1]
				; Enable analog on PE3
				LDR R1, =PORTE_AMSEL
				LDR R0, [R1]
				ORR R0, R0, #0x08 ; set bit 3 to enable analog on PE3
				STR R0, [R1]
				; Disable sequencer while ADC setup
				LDR R1, =ADC0_ACTSS
				LDR R0, [R1]
				BIC R0, R0, #0x08 ; clear bit 3 to disable seq 3
				STR R0, [R1]
				; Select trigger source
				LDR R1, =ADC0_EMUX
				LDR R0, [R1]
				BIC R0, R0, #0xF000 ; clear bits 15:12 to select SOFTWARE
				STR R0, [R1] ; trigger
				; Select input channel
				LDR R1, =ADC0_SSMUX3
				LDR R0, [R1]
				BIC R0, R0, #0x000F ; clear bits 3:0 to select AIN0
				STR R0, [R1]
				; Config sample sequence
				LDR R1, =ADC0_SSCTL3
				LDR R0, [R1]
				ORR R0, R0, #0x06 ; set bits 2:1 (IE0, END0) IE0 is set since we want RIS to be set
				STR R0, [R1]
				; Set sample rate
				LDR R1, =ADC0_PC
				LDR R0, [R1]
				ORR R0, R0, #0x01 ; set bits 3:0 to 2 for 125k sps
				STR R0, [R1]
				; Done with setup, enable sequencer
				LDR R1, =ADC0_ACTSS
				LDR R0, [R1]
				ORR R0, R0, #0x08 ; set bit 3 to enable seq 3
				STR R0, [R1] ; sampling enabled but not initiated yet;Disable
				;Interrupt
				LDR R1, =ADC0_IM
				LDR R0, [R1]
				BIC R0, R0, #0x08 ; disable interrupt
				STR R0, [R1]
				BX 	LR
				ENDP
;***************************************************************
; End of the program  section
;***************************************************************
;LABEL      DIRECTIVE       VALUE                           COMMENT
			ALIGN
			END

