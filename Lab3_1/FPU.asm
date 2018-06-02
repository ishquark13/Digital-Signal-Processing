 	;/* FPU.asm
 	;Given a vector of floating points, his program will sort the flp from largest to smallest in a defined area
 	;of memory*/
		.global _c_int00
		.ref orig_addr
		.ref orig_len
		.ref sorted_addr

WDCR	.set 	0x7029


		.text

_c_int00:

	EALLOW

	mov al,#0x0068;
	mov ar0, #WDCR
	mov *ar0, al

	mov SP, #0x0400


	;*this subroutine will order the floating point numbers from  max to min
	;*Registers altered:
	;RH0 = is the left value
	;RH1 = is the right value
	;
	;



BeginSinkingSort:
	;*this subroutine will order the floating point numbers from  max to min
	;*Registers altered:
	;RH0 = is the first value
	;RH1 = is the indexed+1 value
	;AR4 = orig_len inner loop
	;AR5 = (orig_len-1) outer loop
	;Worst-case assumption : n*(n-1)

	MOV AR4,#orig_len
	MOV AR5,#orig_len

	MOVL XAR2, #0x02
	; XAR2 IS AN INCREMENT DUMMY REGISTER

outerLoop:

	DEC AR5
	BF  SEND, EQ
	MOV AR4,#orig_len;			WHEN ITERATING IN THE OUTER LOOP, RESET THE POINTER AND THE FIRST COUNTER
	MOVL XAR0, #orig_addr;		GRAB THE FIRST VALUE IN THE VECTOR
	MOVL XAR1, #orig_addr + 2;	GRAB THE SECOND VALUE IN THE VECTOR

	;BEGIN THE SORT BY MOVING THE VALUES INTO THE REGISTERS AND COMPARE THEM DIRECTLY...

innerLoop:


	MOV32 R0H, *XAR0
	MOV32 R1H, *XAR1


	CMPF32 R0H, R1H
	MOVST0 ZF
	;MOVST0 CF
	MOVST0 NF

	;	THE FLAGS NEED TO BE SENT FROM THE CPU INTO THE FPU, IN ORDER TO BRANCH CORRECTLY

	BF  SWITCH_INDEX, LEQ	;IF THE FIRST VALUE IS LESS SWITCH INDICES TO KEEP COMPARING

	;ELSE, CHECK THE NEXT CONSECUTIVE FLOATS

INC_INDEX:

	MOVL ACC, XAR0
	ADDL ACC, XAR2
	MOVL XAR0, ACC
	MOVL ACC, XAR1
	ADDL ACC, XAR2
	MOVL XAR1, ACC
	DEC AR4
	; GO TO OUTER LOOP
	BF innerLoop, NEQ
	BF outerLoop, EQ

SWITCH_INDEX:

 	INC AR4
	MOV32 *XAR0, R1H
	MOV32 *XAR1, R0H
	DEC AR4
	BF	INC_INDEX, NEQ
	BF  outerLoop, EQ

; OUTSIDE OF SORT, CAN PLACE AT ANOTHER ADDRESS HERE BY ITERATING A DOUBLE POINTER TO BOTH ADDRESSES OR JUST CORRUPT THE VECTOR
;TODO: FORMULATE THIS INTO A SUBROUTINE
SEND:
	MOV  AR4,#orig_len;			KEEP THE COUNT OF VALUES
	MOVL XAR0, #orig_addr;		GRAB THE FIRST VALUE IN THE VECTOR
	MOVL XAR1, #sorted_addr;	POINT TO THE SORTED ADDRESS VECTOR SPACE

SENDING:
	MOV32 R0H, *XAR0++
	nop
	nop
	nop
	nop
	MOV32 *XAR1, R0H;			; NEED TO FIX HERE SEND SORTED TO VECTOR

	MOVL ACC, XAR1
	ADDL ACC, XAR2
	MOVL XAR1, ACC;				INCREMENT TO NEXT SPACE BY 2 BECAUSE FLOAT VALUES

	DEC AR4

	BF SENDING, NEQ

END:
	BF END, UNC
