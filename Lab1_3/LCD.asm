
; File = LCD.asm
; we will use the I2C protocol to serially write the bits into the LCD display and get cancer
;
;
; Ishmael Contreras: 09/19/2016
;
		.include	"GPIO_registers.asm"
		;.global		_c_int00	;This assembler directive allows _c_int00 to be a
								;global variable. This tells the linker where your
								;program (.text code) begins and where to boot from.
;


		.sect ".cinit"		;data section, see the command linker file, this puts the
							;following data defined below in a block of internal SRAM						;starting at 0x8000.
buffer  .byte "Work?"

	    					;.BSS SECTION is used to reserve space in SRAM for run-time results.
	    					; See the command linker file, the starting address is 0xC000

results .usect 		".ebss", 3	;reserves three words at label 'results' in the .ebss section
sum		.usect    	".ebss", 1	;reserves one word at label 'sum' in the .ebss section

								;.global directive lets you to see the assigned addresses in map file.

;****************************************************************************************


;****************** Start of LCD program and subroutines ***********************

	.text

		;_c_int00:

		;disable the watchdog
		EALLOW
		mov		ar0, #0x0068
		mov		ar1, #WDCR
		mov		*ar1, ar0	;write a #0068 to the #WDCR address to allow the GPA registers

		;set GPBMUX gpio
		mov ar0, #0x0000	; need the GPIO 32,33 to be input
		mov ar1, #GPBMUX1
		mov *ar1, ar0		;write the GPAMUX to have GPIO functionality

;loop
		mov ar0, #0x0000	; note change this back to 0x0000 to output the SDA,SCL
		mov ar1, #GPBDIR
		mov *ar1,ar0		; write 1 to GPIO[32:33] as inputs;; check the GPIO32 clk as output to DAD


		;******INIT_LCD*******************************************************
		;Function: sends the right 7bits to initialize the LCD so that it can
		;be ready to recieve inputs
		;
		;*********************************************************************
		; send address to SDA 0x3F

		; make a delay and check the timing


		;TODO; check the timing of the delay and try to get withing the range of 100khz-150khz



; :
loop:

		mov ar2,#0x0001
		mov ar1,#GPBDIR
		mov *ar1,ar2	; make SDA low

		LC Delay
		LC Delay
		nop
		nop
		nop				; small delay

		mov ar2,#0x0003
		mov ar1,#GPBDIR
		mov *ar1,ar2	; make SCL low



;******Start B O Y Z******************

		LC Delay
		LC Delay

;******CLOCK in 0x33*********************
;****************************************
;*****************************************
;*****************************************
;***************************************

		LC S_3
		LC S_F
		;ACK
		LC S_Bit0
		;ACK


		LC Delay



; Try to send a 0x3
		;Start Condition again for new Byte*********
		LC START
		LC S_Bit0
		;Needed to Start the second Byte************
		LC S_3
		LC S_C

		; ACK
		LC S_Bit0
		; ACK
		;LC Delay
		;TODO: changed this STOP (1)
		;LC STOP
		;mov ar2,#0x0001
		;mov ar1,#GPBDIR
		;mov *ar1,ar2	; high for

		;LC Delay
		;LC Delay

		;mov ar2,#0x0000
		;mov ar1,#GPBDIR
		;mov *ar1,ar2	; high to end transfer
;
;*******DUNZO SENDING 1st init***************************
		LC Delay
		LC Delay
		LC Delay
		LC Delay
		LC Delay
		LC Delay

;******DELAY this long after sending a stop condition*******

		;mov ar2,#0x0001
		;mov ar1,#GPBDIR
		;mov *ar1,ar2	; make SDA low

		;LC Delay
		;LC Delay
		;LC Delay

		;mov ar2,#0x0003
		;mov ar1,#GPBDIR
		;mov *ar1,ar2	; make SCL low

		; prototype to send a 2 addr and cmd byte

; TODO: I changed this START and STOP (1) and the 5 lines below
		;LC START
		;LC S_3
		;LC S_F
		;LC S_Bit0
		;LC S_Bit0
		LC Delay
		LC S_Bit0
		LC S_3
		LC S_8
		LC S_Bit0
		;LC STOP
		;BF loop, unc
;*********Send third time*****

		LC Delay
		LC Delay
		LC Delay
		;LC START
		;LC S_3
		;LC S_F
		;LC S_Bit0
		;LC S_Bit0
		LC Delay
		LC S_Bit0
		LC S_3
		LC S_C
		LC S_Bit0
		;LC STOP
		;BF loop, unc
;*********Send Fourth Time**********

		LC Delay
		LC Delay
		LC Delay
		;LC START
		;LC S_3
		;LC S_F
		;LC S_Bit0
		;LC S_Bit0
		;LC Delay
		LC S_Bit0
		LC S_3
		LC S_8
		LC S_Bit0
		LC STOP
		;BF loop, unc

;******SEND the 0x32**************
;********************************
;*********************************
;*********************************
;*********************************
		LC Delay
		mov ar2,#0x0001
		mov ar1,#GPBDIR
		mov *ar1,ar2	; make SDA low

		LC Delay
		LC Delay
		nop
		nop
		nop				; small delay

		mov ar2,#0x0003
		mov ar1,#GPBDIR
		mov *ar1,ar2	; make SCL low



;******Start B O Y Z******************

		LC Delay
		LC Delay

;******CLOCK in 3F*********************

		LC S_3
		LC S_F
		LC S_Bit0
		LC Delay
		LC START
		LC S_Bit0
		LC S_3
		LC S_C
		LC S_Bit0
		;;;;;;;;;
		LC Delay
		LC Delay
		LC Delay
		LC Delay
		LC Delay
		LC Delay
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
		LC Delay
		LC S_Bit0
		LC S_3
		LC S_8
		LC S_Bit0

		LC Delay
		LC Delay
		LC Delay

		LC Delay
		LC S_Bit0
		LC S_2
		LC S_C
		LC S_Bit0

		LC Delay
		LC Delay
		LC Delay

		LC S_Bit0
		LC S_2
		LC S_8
		LC S_Bit0
		LC STOP
		;BF loop, unc

;******SEND 0x28**************************
;***************************************
;**************************************
;****************************************
;*****************************************
		LC Delay
		mov ar2,#0x0001
		mov ar1,#GPBDIR
		mov *ar1,ar2	; make SDA low

		LC Delay
		LC Delay
		nop
		nop
		nop				; small delay

		mov ar2,#0x0003
		mov ar1,#GPBDIR
		mov *ar1,ar2	; make SCL low



;******Start B O Y Z******************

		LC Delay
		LC Delay
;*********************************

		LC S_3
		LC S_F
		LC S_Bit0
		LC Delay
		LC START
		LC S_Bit0
		LC S_2
		LC S_C
		LC S_Bit0
		;;;;;;;;;
		LC Delay
		LC Delay
		LC Delay
		LC Delay
		LC Delay
		LC Delay
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
		LC Delay
		LC S_Bit0
		LC S_2
		LC S_8
		LC S_Bit0

		LC Delay
		LC Delay
		LC Delay

		LC Delay
		;BUFFER not needed for the 4-bit numbers
		LC S_8
		LC S_C
		LC S_Bit0

		LC Delay
		LC Delay
		LC Delay

		LC S_8
		LC S_8
		LC S_Bit0
		LC STOP
		;BF loop, unc

;****SEND 0x0F*********************************
;***********************************************
;**********************************************
;**********************************************
;*********************************************
;********************************************

		LC Delay
		mov ar2,#0x0001
		mov ar1,#GPBDIR
		mov *ar1,ar2	; make SDA low

		LC Delay
		LC Delay
		nop
		nop
		nop				; small delay

		mov ar2,#0x0003
		mov ar1,#GPBDIR
		mov *ar1,ar2	; make SCL low



;******Start B O Y Z******************

		LC Delay
		LC Delay
;*********************************

		LC S_3
		LC S_F
		LC S_Bit0
		LC Delay
		LC START
		LC S_Bit0
		LC S_0
		LC S_C
		LC S_Bit0
		;;;;;;;;;
		LC Delay
		LC Delay
		LC Delay
		LC Delay
		LC Delay
		LC Delay
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
		LC Delay
		LC S_Bit0
		LC S_0
		LC S_8
		LC S_Bit0

		LC Delay
		LC Delay
		LC Delay

		LC Delay
		;LC S_Bit0
		LC S_F
		LC S_C
		LC S_Bit0

		LC Delay
		LC Delay
		LC Delay

		;LC S_Bit0
		LC S_F
		LC S_8
		LC S_Bit0
		LC STOP
		;BF loop, unc

;********Below SEND 0x01*******************
;******************************************
;****************************************
;***************************************
;******************************************
;**************************************

		LC Delay
		mov ar2,#0x0001
		mov ar1,#GPBDIR
		mov *ar1,ar2	; make SDA low

		LC Delay
		LC Delay
		nop
		nop
		nop				; small delay

		mov ar2,#0x0003
		mov ar1,#GPBDIR
		mov *ar1,ar2	; make SCL low



;******Start B O Y Z******************

		LC Delay
		LC Delay
;*********************************

		LC S_3
		LC S_F
		LC S_Bit0
		LC Delay
		LC START
		LC S_Bit0
		LC S_0
		LC S_C
		LC S_Bit0
		;;;;;;;;;
		LC Delay
		LC Delay
		LC Delay
		LC Delay
		LC Delay
		LC Delay
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
		LC Delay
		LC S_Bit0
		LC S_0
		LC S_8
		LC S_Bit0

		LC Delay
		LC Delay
		LC Delay

		LC Delay
		LC S_Bit0
		LC S_1
		LC S_C
		LC S_Bit0

		LC Delay
		LC Delay
		LC Delay

		LC S_Bit0
		LC S_1
		LC S_8
		LC S_Bit0
		LC STOP
		;BF loop, unc

;;;;;;***********DUNNZO INITIALIZING THE LCD**********************************
;******************************************************************************
;****************************************************************************88
;*******************************************************************************
;*********************************************************************
;*****************************************************************************
;******SEND AN "I"*********************************************************
		LC Delay
		mov ar2,#0x0001
		mov ar1,#GPBDIR
		mov *ar1,ar2	; make SDA low

		LC Delay
		LC Delay
		nop
		nop
		nop				; small delay

		mov ar2,#0x0003
		mov ar1,#GPBDIR
		mov *ar1,ar2	; make SCL low



;******Start B O Y Z******************

		LC Delay
		LC Delay
;*********************************

		LC S_3
		LC S_F
		LC S_Bit0
		LC Delay
		LC START
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
		LC S_Bit0
		LC S_4
		LC S_D
		LC S_Bit0
		;;;;;;;;;
		LC Delay
		LC Delay
		LC Delay
		LC Delay
		LC Delay
		LC Delay

		LC Delay

		LC S_Bit0
		LC S_4
		LC S_9
		LC S_Bit0

		LC Delay
		LC Delay
		LC Delay
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;UPPER NIBBLE
		LC Delay
		;LC S_Bit0
		LC S_9
		LC S_D
		LC S_Bit0

		LC Delay
		LC Delay
		LC Delay

		;LC S_Bit0
		LC S_9
		LC S_9
		LC S_Bit0
		LC STOP
		;BF loop, unc

;*******SEND an "s"***********
		LC Delay
		mov ar2,#0x0001
		mov ar1,#GPBDIR
		mov *ar1,ar2	; make SDA low

		LC Delay
		LC Delay
		nop
		nop
		nop				; small delay

		mov ar2,#0x0003
		mov ar1,#GPBDIR
		mov *ar1,ar2	; make SCL low



;******Start B O Y Z******************

		LC Delay
		LC Delay
;*********************************

		LC S_3
		LC S_F
		LC S_Bit0
		LC Delay
		LC START
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
		LC S_Bit0
		LC S_7
		LC S_D
		LC S_Bit0
		;;;;;;;;;
		LC Delay
		LC Delay
		LC Delay
		LC Delay
		LC Delay
		LC Delay

		LC Delay

		LC S_Bit0
		LC S_7
		LC S_9
		LC S_Bit0

		LC Delay
		LC Delay
		LC Delay
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;UPPER NIBBLE
		LC Delay
		LC S_Bit0
		LC S_3
		LC S_D
		LC S_Bit0

		LC Delay
		LC Delay
		LC Delay

		LC S_Bit0
		LC S_3
		LC S_9
		LC S_Bit0
		LC STOP
;***SEND AN "h"******************
		LC Delay
		mov ar2,#0x0001
		mov ar1,#GPBDIR
		mov *ar1,ar2	; make SDA low

		LC Delay
		LC Delay
		nop
		nop
		nop				; small delay

		mov ar2,#0x0003
		mov ar1,#GPBDIR
		mov *ar1,ar2	; make SCL low



;******Start B O Y Z******************

		LC Delay
		LC Delay
;*********************************

		LC S_3
		LC S_F
		LC S_Bit0
		LC Delay
		LC START
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
		LC S_Bit0
		LC S_6
		LC S_D
		LC S_Bit0
		;;;;;;;;;
		LC Delay
		LC Delay
		LC Delay
		LC Delay
		LC Delay
		LC Delay

		LC Delay

		LC S_Bit0
		LC S_6
		LC S_9
		LC S_Bit0

		LC Delay
		LC Delay
		LC Delay
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;UPPER NIBBLE
		LC Delay
		;LC S_Bit0
		LC S_8
		LC S_D
		LC S_Bit0

		LC Delay
		LC Delay
		LC Delay

		;LC S_Bit0
		LC S_8
		LC S_9
		LC S_Bit0
		LC STOP

;*****SEND AN "m"*******************
		LC Delay
		mov ar2,#0x0001
		mov ar1,#GPBDIR
		mov *ar1,ar2	; make SDA low

		LC Delay
		LC Delay
		nop
		nop
		nop				; small delay

		mov ar2,#0x0003
		mov ar1,#GPBDIR
		mov *ar1,ar2	; make SCL low


		LC Delay
		LC Delay

		LC S_3
		LC S_F
		LC S_Bit0
		LC Delay
		LC START
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
		LC S_Bit0
		LC S_6
		LC S_D
		LC S_Bit0
		;;;;;;;;;
		LC Delay
		LC Delay
		LC Delay
		LC Delay
		LC Delay
		LC Delay

		LC Delay

		LC S_Bit0
		LC S_6
		LC S_9
		LC S_Bit0

		LC Delay
		LC Delay
		LC Delay
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;UPPER NIBBLE
		LC Delay
		;LC S_Bit0
		LC S_D
		LC S_D
		LC S_Bit0

		LC Delay
		LC Delay
		LC Delay

		;LC S_Bit0
		LC S_D
		LC S_9
		LC S_Bit0
		LC STOP

;*****SEND AN "a"**************************
		LC Delay
		mov ar2,#0x0001
		mov ar1,#GPBDIR
		mov *ar1,ar2	; make SDA low

		LC Delay
		LC Delay
		nop
		nop
		nop				; small delay

		mov ar2,#0x0003
		mov ar1,#GPBDIR
		mov *ar1,ar2	; make SCL low



;******Start B O Y Z******************

		LC Delay
		LC Delay
;*********************************

		LC S_3
		LC S_F
		LC S_Bit0
		LC Delay
		LC START
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
		LC S_Bit0
		LC S_6
		LC S_D
		LC S_Bit0
		;;;;;;;;;
		LC Delay
		LC Delay
		LC Delay
		LC Delay
		LC Delay
		LC Delay

		LC Delay

		LC S_Bit0
		LC S_6
		LC S_9
		LC S_Bit0

		LC Delay
		LC Delay
		LC Delay
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;UPPER NIBBLE
		LC Delay
		LC S_Bit0
		LC S_1
		LC S_D
		LC S_Bit0

		LC Delay
		LC Delay
		LC Delay

		LC S_Bit0
		LC S_1
		LC S_9
		LC S_Bit0
		LC STOP


;*******************SEND AN "e"****************

		LC Delay
		mov ar2,#0x0001
		mov ar1,#GPBDIR
		mov *ar1,ar2	; make SDA low

		LC Delay
		LC Delay
		nop
		nop
		nop				; small delay

		mov ar2,#0x0003
		mov ar1,#GPBDIR
		mov *ar1,ar2	; make SCL low



;******Start B O Y Z******************

		LC Delay
		LC Delay
;*********************************

		LC S_3
		LC S_F
		LC S_Bit0
		LC Delay
		LC START
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
		LC S_Bit0
		LC S_6
		LC S_D
		LC S_Bit0
		;;;;;;;;;
		LC Delay
		LC Delay
		LC Delay
		LC Delay
		LC Delay
		LC Delay

		LC Delay

		LC S_Bit0
		LC S_6
		LC S_9
		LC S_Bit0

		LC Delay
		LC Delay
		LC Delay
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;UPPER NIBBLE
		LC Delay
		LC S_Bit0
		LC S_5
		LC S_D
		LC S_Bit0

		LC Delay
		LC Delay
		LC Delay

		LC S_Bit0
		LC S_5
		LC S_9
		LC S_Bit0
		LC STOP

;*****SEND AN "l"***************************
		LC Delay
		mov ar2,#0x0001
		mov ar1,#GPBDIR
		mov *ar1,ar2	; make SDA low

		LC Delay
		LC Delay
		nop
		nop
		nop				; small delay

		mov ar2,#0x0003
		mov ar1,#GPBDIR
		mov *ar1,ar2	; make SCL low



;******Start B O Y Z******************

		LC Delay
		LC Delay
;*********************************

		LC S_3
		LC S_F
		LC S_Bit0
		LC Delay
		LC START
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
		LC S_Bit0
		LC S_6
		LC S_D
		LC S_Bit0
		;;;;;;;;;
		LC Delay
		LC Delay
		LC Delay
		LC Delay
		LC Delay
		LC Delay

		LC Delay

		LC S_Bit0
		LC S_6
		LC S_9
		LC S_Bit0

		LC Delay
		LC Delay
		LC Delay
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;UPPER NIBBLE
		LC Delay
		;LC S_Bit0
		LC S_C
		LC S_D
		LC S_Bit0

		LC Delay
		LC Delay
		LC Delay

		;LC S_Bit0
		LC S_C
		LC S_9
		LC S_Bit0
		LC STOP

;***************SEND AN "space"********************
		LC Delay
		mov ar2,#0x0001
		mov ar1,#GPBDIR
		mov *ar1,ar2	; make SDA low

		LC Delay
		LC Delay
		nop
		nop
		nop				; small delay

		mov ar2,#0x0003
		mov ar1,#GPBDIR
		mov *ar1,ar2	; make SCL low



;******Start B O Y Z******************

		LC Delay
		LC Delay
;*********************************

		LC S_3
		LC S_F
		LC S_Bit0
		LC Delay
		LC START
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
		LC S_Bit0
		LC S_2
		LC S_D
		LC S_Bit0
		;;;;;;;;;
		LC Delay
		LC Delay
		LC Delay
		LC Delay
		LC Delay
		LC Delay

		LC Delay

		LC S_Bit0
		LC S_2
		LC S_9
		LC S_Bit0

		LC Delay
		LC Delay
		LC Delay
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;UPPER NIBBLE
		LC Delay
		LC S_Bit0
		LC S_0
		LC S_D
		LC S_Bit0

		LC Delay
		LC Delay
		LC Delay

		LC S_Bit0
		LC S_0
		LC S_9
		LC S_Bit0
		LC STOP

;**********SEND AN "P"*****************
		LC Delay
		mov ar2,#0x0001
		mov ar1,#GPBDIR
		mov *ar1,ar2	; make SDA low

		LC Delay
		LC Delay
		nop
		nop
		nop				; small delay

		mov ar2,#0x0003
		mov ar1,#GPBDIR
		mov *ar1,ar2	; make SCL low



;******Start B O Y Z******************

		LC Delay
		LC Delay
;*********************************

		LC S_3
		LC S_F
		LC S_Bit0
		LC Delay
		LC START
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
		LC S_Bit0
		LC S_5
		LC S_D
		LC S_Bit0
		;;;;;;;;;
		LC Delay
		LC Delay
		LC Delay
		LC Delay
		LC Delay
		LC Delay

		LC Delay

		LC S_Bit0
		LC S_5
		LC S_9
		LC S_Bit0

		LC Delay
		LC Delay
		LC Delay
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;UPPER NIBBLE
		LC Delay
		LC S_Bit0
		LC S_0
		LC S_D
		LC S_Bit0

		LC Delay
		LC Delay
		LC Delay

		LC S_Bit0
		LC S_0
		LC S_9
		LC S_Bit0
		LC STOP
		;BF loop,unc

;******SEND AN "e"************************

		LC Delay
		mov ar2,#0x0001
		mov ar1,#GPBDIR
		mov *ar1,ar2	; make SDA low

		LC Delay
		LC Delay
		nop
		nop
		nop				; small delay

		mov ar2,#0x0003
		mov ar1,#GPBDIR
		mov *ar1,ar2	; make SCL low



;******Start B O Y Z******************

		LC Delay
		LC Delay
;*********************************

		LC S_3
		LC S_F
		LC S_Bit0
		LC Delay
		LC START
		LC S_Bit0
		LC S_6
		LC S_D
		LC S_Bit0
		;;;;;;;;;
		LC Delay
		LC Delay
		LC Delay
		LC Delay
		LC Delay
		LC Delay
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
		LC Delay
		LC S_Bit0
		LC S_6
		LC S_9
		LC S_Bit0

		LC Delay
		LC Delay
		LC Delay

		LC Delay
		LC S_Bit0
		LC S_5
		LC S_D
		LC S_Bit0

		LC Delay
		LC Delay
		LC Delay

		LC S_Bit0
		LC S_5
		LC S_9
		LC S_Bit0
		LC STOP

;*******SEND AN "l"***********************
		LC Delay
		mov ar2,#0x0001
		mov ar1,#GPBDIR
		mov *ar1,ar2	; make SDA low

		LC Delay
		LC Delay
		nop
		nop
		nop				; small delay

		mov ar2,#0x0003
		mov ar1,#GPBDIR
		mov *ar1,ar2	; make SCL low



;******Start B O Y Z******************

		LC Delay
		LC Delay
;*********************************

		LC S_3
		LC S_F
		LC S_Bit0
		LC Delay
		LC START
		LC S_Bit0
		LC S_6
		LC S_D
		LC S_Bit0
		;;;;;;;;;
		LC Delay
		LC Delay
		LC Delay
		LC Delay
		LC Delay
		LC Delay
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
		LC Delay
		LC S_Bit0
		LC S_6
		LC S_9
		LC S_Bit0

		LC Delay
		LC Delay
		LC Delay

		LC Delay
		;LC S_Bit0
		LC S_C
		LC S_D
		LC S_Bit0

		LC Delay
		LC Delay
		LC Delay

		;LC S_Bit0
		LC S_C
		LC S_9
		LC S_Bit0
		LC STOP

;*******SEND AN "a"********************8*****

		LC Delay
		mov ar2,#0x0001
		mov ar1,#GPBDIR
		mov *ar1,ar2	; make SDA low

		LC Delay
		LC Delay
		nop
		nop
		nop				; small delay

		mov ar2,#0x0003
		mov ar1,#GPBDIR
		mov *ar1,ar2	; make SCL low



;******Start B O Y Z******************

		LC Delay
		LC Delay
;*********************************

		LC S_3
		LC S_F
		LC S_Bit0
		LC Delay
		LC START
		LC S_Bit0
		LC S_6
		LC S_D
		LC S_Bit0
		;;;;;;;;;
		LC Delay
		LC Delay
		LC Delay
		LC Delay
		LC Delay
		LC Delay
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
		LC Delay
		LC S_Bit0
		LC S_6
		LC S_9
		LC S_Bit0

		LC Delay
		LC Delay
		LC Delay

		LC Delay
		LC S_Bit0
		LC S_1
		LC S_D
		LC S_Bit0

		LC Delay
		LC Delay
		LC Delay

		LC S_Bit0
		LC S_1
		LC S_9
		LC S_Bit0
		LC STOP

;*******SEND AN "y"**************************
		LC Delay
		mov ar2,#0x0001
		mov ar1,#GPBDIR
		mov *ar1,ar2	; make SDA low

		LC Delay
		LC Delay
		nop
		nop
		nop				; small delay

		mov ar2,#0x0003
		mov ar1,#GPBDIR
		mov *ar1,ar2	; make SCL low



;******Start B O Y Z******************

		LC Delay
		LC Delay
;*********************************

		LC S_3
		LC S_F
		LC S_Bit0
		LC Delay
		LC START
		LC S_Bit0
		LC S_7
		LC S_D
		LC S_Bit0
		;;;;;;;;;
		LC Delay
		LC Delay
		LC Delay
		LC Delay
		LC Delay
		LC Delay
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
		LC Delay
		LC S_Bit0
		LC S_7
		LC S_9
		LC S_Bit0

		LC Delay
		LC Delay
		LC Delay

		LC Delay
		;LC S_Bit0
		LC S_9
		LC S_D
		LC S_Bit0

		LC Delay
		LC Delay
		LC Delay

		;LC S_Bit0
		LC S_9
		LC S_9
		LC S_Bit0
		LC STOP

;************SEND AN "o"*****************

		LC Delay
		mov ar2,#0x0001
		mov ar1,#GPBDIR
		mov *ar1,ar2	; make SDA low

		LC Delay
		LC Delay
		nop
		nop
		nop				; small delay

		mov ar2,#0x0003
		mov ar1,#GPBDIR
		mov *ar1,ar2	; make SCL low



;******Start B O Y Z******************

		LC Delay
		LC Delay
;*********************************

		LC S_3
		LC S_F
		LC S_Bit0
		LC Delay
		LC START
		LC S_Bit0
		LC S_6
		LC S_D
		LC S_Bit0
		;;;;;;;;;
		LC Delay
		LC Delay
		LC Delay
		LC Delay
		LC Delay
		LC Delay
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
		LC Delay
		LC S_Bit0
		LC S_6
		LC S_9
		LC S_Bit0

		LC Delay
		LC Delay
		LC Delay

		LC Delay
		;LC S_Bit0
		LC S_F
		LC S_D
		LC S_Bit0

		LC Delay
		LC Delay
		LC Delay

		;LC S_Bit0
		LC S_F
		LC S_9
		LC S_Bit0
		LC STOP
		;BF loop, unc
;********ENABLE 2-line MODE*******************8

		LC Delay
		mov ar2,#0x0001
		mov ar1,#GPBDIR
		mov *ar1,ar2	; make SDA low

		LC Delay
		LC Delay
		nop
		nop
		nop				; small delay

		mov ar2,#0x0003
		mov ar1,#GPBDIR
		mov *ar1,ar2	; make SCL low



;******Start B O Y Z******************

		LC Delay
		LC Delay
;*********************************

		LC S_3
		LC S_F
		LC S_Bit0
		LC Delay
		LC START
		;LC S_Bit0
		LC S_C
		LC S_C
		LC S_Bit0
		;;;;;;;;;
		LC Delay
		LC Delay
		LC Delay
		LC Delay
		LC Delay
		LC Delay
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
		LC Delay
		;LC S_Bit0
		LC S_C
		LC S_8
		LC S_Bit0

		LC Delay
		LC Delay
		LC Delay

		LC Delay
		LC S_Bit0
		LC S_0
		LC S_C
		LC S_Bit0

		LC Delay
		LC Delay
		LC Delay

		LC S_Bit0
		LC S_0
		LC S_8
		LC S_Bit0
		LC STOP
;*************************************
;****SEND AN"E"**********************

		LC Delay
		mov ar2,#0x0001
		mov ar1,#GPBDIR
		mov *ar1,ar2	; make SDA low

		LC Delay
		LC Delay
		nop
		nop
		nop				; small delay

		mov ar2,#0x0003
		mov ar1,#GPBDIR
		mov *ar1,ar2	; make SCL low



;******Start B O Y Z******************

		LC Delay
		LC Delay
;*********************************

		LC S_3
		LC S_F
		LC S_Bit0
		LC Delay
		LC START
		LC S_Bit0
		LC S_4
		LC S_D
		LC S_Bit0
		;;;;;;;;;
		LC Delay
		LC Delay
		LC Delay
		LC Delay
		LC Delay
		LC Delay
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
		LC Delay
		LC S_Bit0
		LC S_4
		LC S_9
		LC S_Bit0

		LC Delay
		LC Delay
		LC Delay

		LC Delay
		LC S_Bit0
		LC S_5
		LC S_D
		LC S_Bit0

		LC Delay
		LC Delay
		LC Delay

		LC S_Bit0
		LC S_5
		LC S_9
		LC S_Bit0
		LC STOP
		;BF loop, unc
;;;;;;;;;;;;;;;;*****SEND AN "E"*******************
		LC Delay
		mov ar2,#0x0001
		mov ar1,#GPBDIR
		mov *ar1,ar2	; make SDA low

		LC Delay
		LC Delay
		nop
		nop
		nop				; small delay

		mov ar2,#0x0003
		mov ar1,#GPBDIR
		mov *ar1,ar2	; make SCL low



;******Start B O Y Z******************

		LC Delay
		LC Delay
;*********************************

		LC S_3
		LC S_F
		LC S_Bit0
		LC Delay
		LC START
		LC S_Bit0
		LC S_4
		LC S_D
		LC S_Bit0
		;;;;;;;;;
		LC Delay
		LC Delay
		LC Delay
		LC Delay
		LC Delay
		LC Delay
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
		LC Delay
		LC S_Bit0
		LC S_4
		LC S_9
		LC S_Bit0

		LC Delay
		LC Delay
		LC Delay

		LC Delay
		LC S_Bit0
		LC S_5
		LC S_D
		LC S_Bit0

		LC Delay
		LC Delay
		LC Delay

		LC S_Bit0
		LC S_5
		LC S_9
		LC S_Bit0
		LC STOP

;****SEND AN "L"********************************
		LC Delay
		mov ar2,#0x0001
		mov ar1,#GPBDIR
		mov *ar1,ar2	; make SDA low

		LC Delay
		LC Delay
		nop
		nop
		nop				; small delay

		mov ar2,#0x0003
		mov ar1,#GPBDIR
		mov *ar1,ar2	; make SCL low



;******Start B O Y Z******************

		LC Delay
		LC Delay
;*********************************

		LC S_3
		LC S_F
		LC S_Bit0
		LC Delay
		LC START
		LC S_Bit0
		LC S_4
		LC S_D
		LC S_Bit0
		;;;;;;;;;
		LC Delay
		LC Delay
		LC Delay
		LC Delay
		LC Delay
		LC Delay
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
		LC Delay
		LC S_Bit0
		LC S_4
		LC S_9
		LC S_Bit0

		LC Delay
		LC Delay
		LC Delay

		LC Delay
		;LC S_Bit0
		LC S_C
		LC S_D
		LC S_Bit0

		LC Delay
		LC Delay
		LC Delay

		;LC S_Bit0
		LC S_C
		LC S_9
		LC S_Bit0
		LC STOP
		;BF loop, unc
;;;***********SEND "space"*****************
		LC Delay
		mov ar2,#0x0001
		mov ar1,#GPBDIR
		mov *ar1,ar2	; make SDA low

		LC Delay
		LC Delay
		nop
		nop
		nop				; small delay

		mov ar2,#0x0003
		mov ar1,#GPBDIR
		mov *ar1,ar2	; make SCL low



;******Start B O Y Z******************

		LC Delay
		LC Delay
;*********************************

		LC S_3
		LC S_F
		LC S_Bit0
		LC Delay
		LC START
		LC S_Bit0
		LC S_2
		LC S_D
		LC S_Bit0
		;;;;;;;;;
		LC Delay
		LC Delay
		LC Delay
		LC Delay
		LC Delay
		LC Delay
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
		LC Delay
		LC S_Bit0
		LC S_2
		LC S_9
		LC S_Bit0

		LC Delay
		LC Delay
		LC Delay

		LC Delay
		LC S_Bit0
		LC S_0
		LC S_D
		LC S_Bit0

		LC Delay
		LC Delay
		LC Delay

		LC S_Bit0
		LC S_0
		LC S_9
		LC S_Bit0
		LC STOP
		;BF loop, unc
;;******SEND A 4********************
		LC Delay
		mov ar2,#0x0001
		mov ar1,#GPBDIR
		mov *ar1,ar2	; make SDA low

		LC Delay
		LC Delay
		nop
		nop
		nop				; small delay

		mov ar2,#0x0003
		mov ar1,#GPBDIR
		mov *ar1,ar2	; make SCL low



;******Start B O Y Z******************

		LC Delay
		LC Delay
;*********************************

		LC S_3
		LC S_F
		LC S_Bit0
		LC Delay
		LC START
		LC S_Bit0
		LC S_3
		LC S_D
		LC S_Bit0
		;;;;;;;;;
		LC Delay
		LC Delay
		LC Delay
		LC Delay
		LC Delay
		LC Delay
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
		LC Delay
		LC S_Bit0
		LC S_3
		LC S_9
		LC S_Bit0

		LC Delay
		LC Delay
		LC Delay

		LC Delay
		LC S_Bit0
		LC S_4
		LC S_D
		LC S_Bit0

		LC Delay
		LC Delay
		LC Delay

		LC S_Bit0
		LC S_4
		LC S_9
		LC S_Bit0
		LC STOP
;*****SEND A 5********************
;**********************************
		LC Delay
		mov ar2,#0x0001
		mov ar1,#GPBDIR
		mov *ar1,ar2	; make SDA low

		LC Delay
		LC Delay
		nop
		nop
		nop				; small delay

		mov ar2,#0x0003
		mov ar1,#GPBDIR
		mov *ar1,ar2	; make SCL low



;******Start B O Y Z******************

		LC Delay
		LC Delay
;*********************************

		LC S_3
		LC S_F
		LC S_Bit0
		LC Delay
		LC START
		LC S_Bit0
		LC S_3
		LC S_D
		LC S_Bit0
		;;;;;;;;;
		LC Delay
		LC Delay
		LC Delay
		LC Delay
		LC Delay
		LC Delay
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
		LC Delay
		LC S_Bit0
		LC S_3
		LC S_9
		LC S_Bit0

		LC Delay
		LC Delay
		LC Delay

		LC Delay
		LC S_Bit0
		LC S_5
		LC S_D
		LC S_Bit0

		LC Delay
		LC Delay
		LC Delay

		LC S_Bit0
		LC S_5
		LC S_9
		LC S_Bit0
		LC STOP

;;SEND A 1*********************************
		LC Delay
		mov ar2,#0x0001
		mov ar1,#GPBDIR
		mov *ar1,ar2	; make SDA low

		LC Delay
		LC Delay
		nop
		nop
		nop				; small delay

		mov ar2,#0x0003
		mov ar1,#GPBDIR
		mov *ar1,ar2	; make SCL low



;******Start B O Y Z******************

		LC Delay
		LC Delay
;*********************************

		LC S_3
		LC S_F
		LC S_Bit0
		LC Delay
		LC START
		LC S_Bit0
		LC S_3
		LC S_D
		LC S_Bit0
		;;;;;;;;;
		LC Delay
		LC Delay
		LC Delay
		LC Delay
		LC Delay
		LC Delay
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
		LC Delay
		LC S_Bit0
		LC S_3
		LC S_9
		LC S_Bit0

		LC Delay
		LC Delay
		LC Delay

		LC Delay
		LC S_Bit0
		LC S_1
		LC S_D
		LC S_Bit0

		LC Delay
		LC Delay
		LC Delay

		LC S_Bit0
		LC S_1
		LC S_9
		LC S_Bit0
		LC STOP
;************SEND A 1*********************************
		LC Delay
		mov ar2,#0x0001
		mov ar1,#GPBDIR
		mov *ar1,ar2	; make SDA low

		LC Delay
		LC Delay
		nop
		nop
		nop				; small delay

		mov ar2,#0x0003
		mov ar1,#GPBDIR
		mov *ar1,ar2	; make SCL low



;******Start B O Y Z******************

		LC Delay
		LC Delay
;*********************************

		LC S_3
		LC S_F
		LC S_Bit0
		LC Delay
		LC START
		LC S_Bit0
		LC S_3
		LC S_D
		LC S_Bit0
		;;;;;;;;;
		LC Delay
		LC Delay
		LC Delay
		LC Delay
		LC Delay
		LC Delay
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
		LC Delay
		LC S_Bit0
		LC S_3
		LC S_9
		LC S_Bit0

		LC Delay
		LC Delay
		LC Delay

		LC Delay
		LC S_Bit0
		LC S_1
		LC S_D
		LC S_Bit0

		LC Delay
		LC Delay
		LC Delay

		LC S_Bit0
		LC S_1
		LC S_9
		LC S_Bit0
		LC STOP

		;BF loop, unc

;******SEND A "C"*************************
;
		LC Delay
		mov ar2,#0x0001
		mov ar1,#GPBDIR
		mov *ar1,ar2	; make SDA low

		LC Delay
		LC Delay
		nop
		nop
		nop				; small delay

		mov ar2,#0x0003
		mov ar1,#GPBDIR
		mov *ar1,ar2	; make SCL low



;******Start B O Y Z******************

		LC Delay
		LC Delay
;*********************************

		LC S_3
		LC S_F
		LC S_Bit0
		LC Delay
		LC START
		LC S_Bit0
		LC S_4
		LC S_D
		LC S_Bit0
		;;;;;;;;;
		LC Delay
		LC Delay
		LC Delay
		LC Delay
		LC Delay
		LC Delay
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
		LC Delay
		LC S_Bit0
		LC S_4
		LC S_9
		LC S_Bit0

		LC Delay
		LC Delay
		LC Delay

		LC Delay
		LC S_Bit0
		LC S_3
		LC S_D
		LC S_Bit0

		LC Delay
		LC Delay
		LC Delay

		LC S_Bit0
		LC S_3
		LC S_9
		LC S_Bit0
		LC STOP

		BF loop, unc





; CLEAR SCREEN
;********The subroutines are at the bottom*****
;*****SR: S_Bit0***********************

S_Bit0:
		mov ar2,#0x0003
		mov ar1,#GPBDIR
		mov *ar1,ar2	; make SDA low

		LC Delay

		mov ar2,#0x0001 ; need low SDA and SCL low
		mov ar1,#GPBDIR
		mov *ar1,ar2	;

		LC Delay

		mov ar2,#0x0003 ; keep SDA low
		mov ar1,#GPBDIR
		mov *ar1,ar2	; make SCL low

		LC Delay
		LRET

;*****SR:S_Bit1

S_Bit1:
		mov ar2,#0x0002
		mov ar1,#GPBDIR
		mov *ar1,ar2	; make SDA high while keeping SCL low

		LC Delay

		mov ar2,#0x0000 ; need both SCL and SDA high
		mov ar1,#GPBDIR
		mov *ar1,ar2	; make SCL high

		LC Delay

		mov ar2,#0x0002 ; keep SDA high
		mov ar1,#GPBDIR
		mov *ar1,ar2	; make SCL low

		LC Delay
		LRET

;*********Delay**********************
Delay:
		mov ar0, #0x003F
uls:	dec ar0
		B uls, NEQ
		LRET
;check the DAD board

S_0:
	LC S_Bit0
	LC S_Bit0
	LC S_Bit0
	LRET

S_1:
	LC S_Bit0
	LC S_Bit0
	LC S_Bit1
	LRET


S_2:
	LC S_Bit0
	LC S_Bit1
	LC S_Bit0
	LRET

S_3:
	LC S_Bit0
	LC S_Bit1
	LC S_Bit1
	LRET

S_4:
	LC S_Bit1
	LC S_Bit0
	LC S_Bit0
	LRET

S_5:
	LC S_Bit1
	LC S_Bit0
	LC S_Bit1
	LRET

S_6:
	LC S_Bit1
	LC S_Bit1
	LC S_Bit0
	LRET

S_7:
	LC S_Bit1
	LC S_Bit1
	LC S_Bit1
	LRET

S_8:
	LC S_Bit1
	LC S_Bit0
	LC S_Bit0
	LC S_Bit0
	LRET

S_9:
	LC S_Bit1
	LC S_Bit0
	LC S_Bit0
	LC S_Bit1
	LRET

S_A:
	LC S_Bit1
	LC S_Bit0
	LC S_Bit1
	LC S_Bit0
	LRET

S_B:
	LC S_Bit1
	LC S_Bit0
	LC S_Bit1
	LC S_Bit1
	LRET

S_C:
	LC S_Bit1
	LC S_Bit1
	LC S_Bit0
	LC S_Bit0
	LRET

S_D:
	LC S_Bit1
	LC S_Bit1
	LC S_Bit0
	LC S_Bit1
	LRET

S_E:
	LC S_Bit1
	LC S_Bit1
	LC S_Bit1
	LC S_Bit0
	LRET

S_F:
	LC S_Bit1
	LC S_Bit1
	LC S_Bit1
	LC S_Bit1
	LRET

START:
		mov ar2,#0x0001
		mov ar1,#GPBDIR
		mov *ar1,ar2	; make SDA low

		LC Delay
		LC Delay
		nop
		nop
		nop				; small delay

		mov ar2,#0x0003
		mov ar1,#GPBDIR
		mov *ar1,ar2	; make SCL low

		LC Delay
		LC Delay

	LRET

STOP:
	mov ar2,#0x0001
		mov ar1,#GPBDIR
		mov *ar1,ar2	; high for

		LC Delay
		LC Delay

		mov ar2,#0x0000
		mov ar1,#GPBDIR
		mov *ar1,ar2	; high to end transfer
		LRET

