
		.def score_addr
		.def score_vector_len
		.def min_addr

		.sect ".cinit"
		;.data
score_vector_len:	.set	9

score_addr: 		.word   0xC34E,0x1101,0x2156,0x7FD1,0x8912,0x5F00,0x0431,0x6ED3,0x94F1,0x4C01

;min_addr:		.usect    	".ebss", 1
min_addr:		.set 0xFACC
		.global min_addr

