	.cdecls C, NOLIST, "Lab6_FilterAssembly.h"
		.ref _LPFilter
		.def _FirFilter
	.text
	; 3057812428
_FirFilter:
	;FirFilter(&LPFilter.coefficientsOfTaps[0],&LPFilter.circularBuffer[iterator],LPFilter.TapsMax,LPFilter.circularBufferMax);
	;Put order of function parameteres as follows:
	;float pointer: coefficientsBufferTaps --> XAR4
	;float pointer: circularBuffer         --> XAR5
	;Uint32 TapsMax						   --> ACC
	;Uint32 CircularbufferMax			   --> Stack

	;grab value before pushing the rest of the saved registers
	;POP  XAR5
	; preserve after grabbing the Uint32 circularBuffer
	PUSH XAR1
	PUSH XAR2
	PUSH XAR3
	MOV32 XAR1,R7H
	PUSH XAR1
	;Save these used on entry registers because they will be used in the subroutine

	;MOV32 XAR1,R4H
	;PUSH XAR1
	;MOV32 XAR1,R5H
	;PUSH XAR1			; these were done with a function call w/o struct*
	;MOV32 XAR1,R6H
	;PUSH XAR1
	;MOV32 XAR1,R7H
	;PUSH XAR1


	;Load into the assembly registers
	;MOVL ACC, *+XAR4[1]
	;LSL ACC, 16
	;MOVL ACC, *+XAR4[0]							; grab the first value the pointer to the buffer in SRAM	; grab the second value, the pointer to the tap array											;*+XAR4[FIRFilter.circularBufferMax]  ; grab the current value of the iterator in the array for the adc circular buffer
	;MOVL XAR5, #TAPSMAX	- 1
	DEC AH
	MOV AR3, AH
	MOV AH,#CIRCULARBUFFERMAX
	;MOVL XAR7, *+XAR4[6]							; grab the second value, the pointer to the taps; grab the max value of the circularBuffer to do comparison
	;ADDB XAR4, #2													; or use *+XAR4[FIRFilter.TapsMax]
	;MOVL XAR3, *+XAR4[6]												; load a zero into the auxillary register for comparison
	;; put the values into ACC and shift L by 1
	;; load into XAR1 for amode1
	;; ADDB of 2 to the smallest
	LSL ACC, 1
	MOVL XAR1, ACC


	; do the rest here
	MOVL XAR0, #0
	MOV32 R3H,XAR0									; in the FPU the comparison will need to be done also

	;recall that the math operations need to be done in the ACCumulator

	;MOVL ACC, XAR2 										; AL = order*2 (for float)							; load into the ACC the current value of the iterator in "circularBuffer"
	;LSL	 ACC, 1
	;ADDL ACC, XAR6
	MOVL XAR6, XAR4
	MOVL XAR7, XAR5													; Add the sram current value to the one inside the ACC;
														; to get to the right place in memory

	;MOVL XAR3, ACC										;put this is XAR3 for the MACF32 instruction calls for it
	;ADD XAR3, #2										;this addition of two will be able to account for the predecrement of the pointer in next instruction

	SETC AMODE 											; AMODE = 1, sets the addressing mode from the c28x to c2xlp which allows greater than 128 taps 8 bits

														;if this instruction does not work use: ADDB for a 7-bit constant to minimize memory usage
BEGIN:
	NOP
	RPT AR3
	||MACF32 R7H, R3H, *+XAR6[AR1%++], *XAR7++ 			; TODO:!!!!performs the following (index of xar1 0:16) READ AMODE = 1
	ADDF32 R7H,R7H,R3H								; Add both the parts of the needed value because they are placed in different registers
	CLRC AMODE
	NOP
	MOV32 R0H,R7H						; put the value in the right register to be placed into the return stack for the C-file

	; save these registers because they will be needed in the FIR algorithm
	;
	;
	; Perform the Algorithm below:
	; Current values are in
	; float pointer: coefficientsBufferTaps --> XAR4
	; float pointer: circularBuffer         --> XAR5
	; Uint32 TapsMax						--> ACC
	; Uint32 CircularbufferMax			    --> XAR1
	;


	; pop the r3h and r7h
	POP XAR1
	MOV32 R7H,XAR1
	POP XAR3
	POP XAR2
	POP XAR1
	LRETR
