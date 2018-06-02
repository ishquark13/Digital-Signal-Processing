;Echo.asm:
;Author: Ishmael Pelayo
;Take a 4-bit switch circuit and echo it to an 8 bit LED bank,
;duplicating the 4 bits into both the high and low
;sectors of the LED

	.global _c_int00
	.include "GPIO_registers.asm"
	.text

_c_int00:

;****** Initializations for the specific registers of GPIO ******************
		EALLOW
		mov		ar0, #0x0068
		mov		ar1, #WDCR
		mov		*ar1, ar0	;write a #0068 to the #WDCR address to allow the GPA registers

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

; DEBUG LED;

		mov ar0, #0x00FF
		mov ar1, #GPADAT
		mov *ar1, ar0

; works writing to the LED's****************************************************

; Write to the LED's by reading the switch inputs and shifting right by polling

LOOP:

		mov ar0, #GPADAT	; read in the value of the gpio data
		nop
		nop
		mov al, *ar0		; make the accumulator hold the value from the gpio data
		mov ar1, #0x0F00	; mask the gpio[8:11]
		and al,ar1
		LSR al, 4			; shift right 4 bits
		mov ar2, al			; hold these 4bits
		LSR al, 4			; shift right once more
		mov ar3, al			; hold this value in ar3
		OR 	al, ar2
		mov *ar0, al		; print to the led bar
		BF LOOP, UNC		; branch to the top














