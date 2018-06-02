			.ref score_addr
			.ref score_vector_len
			.ref min_addr

	.global		_c_int00

	.text

_c_int00:
			mov ar2,#score_vector_len
  			mov ar0,#score_addr	;load the address of vector of values in ar0
  			mov al, *ar0		; put the value at the address into al
loop:		inc ar0
  			mov ah, *ar0		; put the next value in the vector list
  			dec ar2				; decrement the counter
  			BF store, EQ		; store when the counter == 0
  			cmp al,ah			; compare both values

  			BF change_num, LT	; if al < ah , put ah into al;; try GT
  			BF loop, UNC		; else iterate

change_num:
			mov al,ah			; put ah to al
			BF loop,UNC			; go back to load new value into ah and compare with lowest value currently stored


store:
		mov AR1,#min_addr
		nop
		mov *AR1, al			; write to the min_addr the min number
		nop

JOHNS:	BF JOHNS, unc




