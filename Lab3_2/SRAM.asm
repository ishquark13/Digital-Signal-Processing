	 ;SRAM.asm:
	;Author: Ishmael Pelayo
	;interface a 256K x 16 SRAM
	;Make tests to write and read to the addresses
	;trigger the tests on the switches 0,1,2

	.global _c_int00
	.include "GPIO_registers_v_3.asm"
	.text

SRAM_addr_begin		.set	0x100000; At 1MB extends to 13FFFF because 256K
SRAM_addr_end		.set	0x1FFFFF;
SRAM_err_addr		.set    0x8500;
SRAM_val_addr		.set    0x8550;

_c_int00:
	.global SRAM_addr_begin, SRAM_addr_end,SRAM_err_addr, SRAM_val_addr
;****** Initializations for the specific registers of GPIO ******************
		EALLOW
		mov	ar0, #0x0068
		mov	ar1, #WDCR
		mov	*ar1, ar0	;write a #0068 to the #WDCR address to allow the GPA registers

		mov ar0, #0x0000
		mov ar1, #GPAMUX1
		mov *ar1, ar0		;write the GPAMUX to have GPIO functionality

		mov ar0, #0xFF
		mov ar1, #GPADIR
		mov *ar1,ar0		; write 1's to GPIO[0:7] as outputs

		mov ar0, #0x0FF
		mov ar1, #GPAPUD
		mov *ar1,ar0		; write 1's to GPIO[0:7] as outputs
;*********************************************************************************

;********Initializations for the SRAM Chip Select*********************************;
;*********************************************************************************;
		mov ar0, #0xFF00
		mov ar1, #GPAMUX2 + 1
		mov *ar1, ar0

		mov ar0, #0xFFFF
		mov ar1, #GPBMUX1 + 1
		mov *ar1, ar0

		mov ar0, #0xF0A0	; FFC0
		mov ar1, #GPBMUX1
		mov *ar1, ar0

		mov ar0, #0xFFFF
		mov ar1, #GPCMUX1
		mov *ar1, ar0

		mov ar0, #0xFFFF
		mov ar1, #GPCMUX1 + 1
		mov *ar1, ar0

		mov ar0, #0xFFFF
		mov ar1, #GPCMUX2
		mov *ar1, ar0

		; debug this ;
		mov al, #0x0100
		mov ar1, #PCLKCR3
		OR  al, ar1
		mov *ar1, al
		; debug this ;

;***************DONE INIT THE SRAM*************************************************;

;***************TRY WRITING AND READING TO IT**************************************;
		;MOVL XAR0, #SRAM_addr_begin
CHECK:
		;MOV AR1, #0xAA
  		;MOV *XAR0++, AR1
  		 LC TEST_3_POPULATE
		 LC TEST_3_SEARCH
		 BF	CHECK, UNC

;***********************************************************************************



;*******************TEST SUBROUTINES************************************************

TEST_1:

		MOVL XAR0, #SRAM_addr_begin;	ADDRESS TO CHANGE
		MOV   AR1, #0XAA			;		VALUE TO WRITE
		MOVL XAR6, #0XFFFF		; 		STOPPING COND

		MOVL XAR2,#SRAM_addr_end;		END ADDRESS TO PLACE STOP COND
		MOVL *XAR2, XAR6

WR_AA:

		MOV *XAR0++, AR1
		MOVL ACC, XAR2
		CMPL ACC, XAR0
		BF   WR_AA, NEQ

RD_AA:
		MOVL XAR0, #SRAM_addr_begin
		MOVL ACC, *XAR0
		CMPL ACC, XAR1
		BF   LED_$EE, NEQ
		MOV  AR1, #0X02
		MOVL ACC, XAR0	; CHANGE 3 TO 0
		ADDL ACC, XAR1	; INCREMENT
		CMPL ACC, XAR2
		BF   LED_$AA, EQ
		MOVL XAR0, ACC	; CHANGE 3 TO 0
		BF   RD_AA, UNC




LED_$EE:
		MOVL XAR0, #0X00000000
		MOV AR0, #GPADAT;	ERROR
		MOV AR1, #0XEE
		MOV *AR0, AR1 ; 	WRITE 0XEE TO LEDS

		LRET

LED_$AA:
		MOVL XAR0, #0X00000000
		MOV AR0, #GPADAT;	ERROR
		MOV AR1, #0XAA
		MOV *AR0, AR1 ; 	WRITE 0XEE TO LEDS

		LRET




TEST_2:

		MOVL XAR3, #SRAM_addr_begin;	ADDRESS TO CHANGE
		MOVL XAR1, #0X55			;		VALUE TO WRITE
		MOVL XAR6, #0XFFFF		; 		STOPPING COND

		MOVL XAR2,#SRAM_addr_end;		END ADDRESS TO PLACE STOP COND
		MOVL *XAR2, XAR6

WR_55:

		MOVL *XAR3++, XAR1
		MOVL ACC, XAR2
		CMPL ACC, XAR3
		BF   WR_55, NEQ

RD_55:
		MOVL XAR0, #SRAM_addr_begin
		MOVL ACC, *XAR0
		CMPL ACC, XAR1
		BF   LED_$EE2, NEQ
		MOV  AR1, #0X02
		MOVL ACC, XAR0	; CHANGE 3 TO 0
		ADDL ACC, XAR1	; INCREMENT
		CMPL ACC, XAR2
		BF   LED_$55, EQ
		MOVL XAR0, ACC	; CHANGE 3 TO 0
		BF   RD_55, UNC

LED_$EE2:
		MOVL XAR0, #0X00000000
		MOV AR0, #GPADAT;	ERROR
		MOV AR1, #0XEE
		MOV *AR0, AR1 ; 	WRITE 0XEE TO LEDS

		LRET

LED_$55:
		MOVL XAR0, #0X00000000
		MOV AR0, #GPADAT;	ERROR
		MOV AR1, #0XAA
		MOV *AR0, AR1 ; 	WRITE 0XEE TO LEDS

		LRET




TEST_3_POPULATE:

			MOVL  XAR0, #SRAM_addr_begin;		ADDRESS TO START AT
			MOVL  XAR3,#SRAM_addr_end
			MOV   AR1, #0X0000			;		START WRITING A 0X0000 AND INCREMENT AT EACH ADDRESS
STORE:		MOV  *XAR0, AR1
			INC   AR1							;INCREMENT AR1 VALUE

												;ADD THE 0X01 TO THE 32-BIT NAVIGATE TO THE HIGHER 16-BIT

			MOV  AR2, #0X01
			MOVL ACC, XAR0	;CH
			ADD  ACC, AR2	; 					INCREMENT THE POINTER
			CMPL ACC, XAR3						;CHECK IF THE
			MOVL XAR0, ACC
			BF   DONE, EQ
			BF   STORE, UNC
			;;;;;;;;;;;;;;;;;;;;;
DONE:
			mov ar7,#0x420
			LRET


;;;;;;;;;;;;;*DEBUG RUBBISH BELOW;;;;;;;;;;;;;;;;;;;;;;;;;;



TEST_3_SEARCH:

			MOVL  XAR0, #SRAM_addr_begin;		ADDRESS TO START AT
			MOVL  XAR3,#SRAM_addr_end
			MOV   AR1, #0X0000			;		START WRITING A 0X0000 AND INCREMENT AT EACH ADDRESS
SEARCH:

		    MOVL XAR6,#0X00000000
			MOVL ACC, XAR6
			MOV AL, *AR0
			CMP AL, AR1
			BF   PRINT_EE, NEQ
			INC AR1


			MOV  *XAR0, AR1
			;INC   AR1							;INCREMENT AR1 VALUE

			MOV  AR2, #0X01
			MOVL ACC, XAR0						;CH
			ADD  ACC, AR2	 					;INCREMENT THE POINTER
			CMPL ACC, XAR3						;CHECK IF THE
			MOVL XAR0, ACC						;ADD THE 0X01 TO THE 32-BIT NAVIGATE TO THE HIGHER 16-BIT

			BF   EXIT, EQ
			BF   SEARCH, UNC
			;;;;;;;;;;;;;;;;;;;;;

PRINT_EE:
			MOVL XAR3, #SRAM_err_addr
			MOVL *XAR3, XAR0

			MOVL XAR2, #SRAM_val_addr
			MOV *XAR2, AR1

			MOVL XAR0, #0X00000000
			MOV AR0, #GPADAT;	ERROR
			MOV AR1, #0XEE
			MOV *AR0, AR1 ; 	WRITE 0XEE TO LEDS
			; WRITE TO THE ERROR
			;


		LRET

EXIT:
			mov ar7,#0x420
			LRET





