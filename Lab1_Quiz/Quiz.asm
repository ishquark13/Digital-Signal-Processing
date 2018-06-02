	.ref Quiz_Values, Quiz_Values_Length, Min_Value, Max_Value


		.include	"GPIO_registers.asm"
		.global		_c_int00	;This assembler directive allows _c_int00 to be a
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

_c_int00:

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

		mov ar0, #0x0000
		mov ar1, #GPAMUX1
		mov *ar1, ar0		;write the GPAMUX to have GPIO functionality

		mov ar0, #0xFF
		mov ar1, #GPADIR
		mov *ar1,ar0		; write 1's to GPIO[0:7] as outputs

		mov ar0, #0x0FF
		mov ar1, #GPAPUD
		mov *ar1,ar0		; write 1's to GPIO[0:7] as outputs

;;***************INIT GPIO***************************************************
			;mov xar4,#Min_Value
CHECK:
			MOV AR2,#Quiz_Values_Length
			;MOVL XAR5, *XAR2
  			MOVL XAR0,#Quiz_Values		;load the address of vector of values in ar0
  			MOVL ACC, *XAR0++
  			MOVL ACC, *XAR0++
loop:		MOVL XAR1, *XAR0++			; put the value at the address into a

  			MOVL XAR3, *XAR0			; put the next value in the vector list
  			dec AR2						; decrement the counter
  			BF store, EQ			; store when the counter == 0
  			CMPL ACC,XAR3				; compare both values



  			BF change_num, LO	; if al < ah , put ah into al;; try GT
  			BF loop, UNC		; else iterate

change_num:
			MOVL XAR1,XAR3			; put ah to al
			BF loop,UNC			; go back to load new value into ah and compare with lowest value currently stored


store:
		MOVL XAR4,#Max_Value
		nop
		MOVL *XAR4,ACC 			; write to the min_addr the min number
		nop
		;BF CHECK, UNC



			MOV AR2,#Quiz_Values_Length
			;MOVL XAR5, *XAR2
  			MOVL XAR0,#Quiz_Values		;load the address of vector of values in ar0
loop1:		MOVL XAR1, *XAR0++			; put the value at the address into al

			MOVL ACC, XAR1
  			MOVL XAR3, *XAR0			; put the next value in the vector list
  			dec AR2						; decrement the counter
  			BF store1, EQ			; store when the counter == 0
  			CMPL ACC,XAR3				; compare both values



  			BF change_num1, HI	; if al < ah , put ah into al;; try GT
  			BF loop1, UNC		; else iterate

change_num1:
			MOVL XAR1,XAR3			; put ah to al
			BF loop1,UNC			; go back to load new value into ah and compare with lowest value currently stored


store1:
		MOVL XAR5,#Min_Value
		nop
		MOVL *XAR5,ACC 			; write to the min_addr the min number
		nop
		;BF DEBUG,UNC

TOP:
		mov ar2, #GPADAT	; read in the value of the gpio data
		nop
		nop
		;BF TOP, UNC
		mov al, *ar2		; make the accumulator hold the value from the gpio data
		mov ar1, #0x0F00	; mask the gpio[8:11]
		and al,ar1
		LSR al, 3
		cmp al, #0x0100
		BF LCD_WRITE_MAXIMUM, EQ

LCD_WRITE_MINIMUM:

		LC INIT_LCD
		LC Delay
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
;**********************WRITE M******************************************
;**********************
;***********************************************************************
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
;***********i************************8
;******************************8
;*************************************
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
		LC S_E
		LC S_D
		LC S_Bit0

		LC Delay
		LC Delay
		LC Delay

		;LC S_Bit0
		LC S_E
		LC S_9
		LC S_Bit0
		LC STOP
;*********************************************
;*******************************************
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
;*********************************************
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
;***********************************8
;****************************************************
;**************************************************8
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
;*******************************************************8
;********************************************************
;********************************************************
;******************************************************
;********************************************************
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
		BF  TOP, UNC
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;




LCD_WRITE_MAXIMUM:

	LC INIT_LCD
	LC Delay
;*********************************************
;*****************************************
;***********************************************
;***********************************************
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
;*********************************************
;*****************************************
;***********************************************
;***********************************************
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

;*********************************************
;*****************************************
;***********************************************
;***********************************************
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

		;*********************************************
;*****************************************
;***********************************************
;***********************************************
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

		;*********************************************
;*****************************************
;***********************************************
;***********************************************
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

		;*********************************************
;*****************************************
;***********************************************
;***********************************************
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


		;*********************************************
;*****************************************
;***********************************************
;***********************************************
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
		LC Delay
		BF  TOP, UNC



























;************************************************************************************************
INIT_LCD:
loop2:

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

;
;*******DUNZO SENDING 1st init***************************
		LC Delay
		LC Delay
		LC Delay
		LC Delay
		LC Delay
		LC Delay


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
		LRET

;;;;;;***********DUNNZO INITIALIZING THE LCD**********************************
;******************************************************************************
;****************************************************************************88
;*******************************************************************************
;*********************************************************************
;*****************************************************************************
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
;**********THIS IS SENDING ONE PACKET**************888888
;******************************************************8
;***************************************************8
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
