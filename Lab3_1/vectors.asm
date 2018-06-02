		.def orig_addr
		.def orig_len
		.def sorted_addr

		.sect ".cinit"
		;.data
orig_len:			.set	10

orig_addr: 		.float   2.1235
				.float	 5.342
				.float	 7.234234
				.float	 9.08234
				.float	 10.2342
				.float	 0.09023
				.float	 2.1234
				.float	 5.32
				.float	 6
				.float   1.743

sorted_addr:	.set 0x9000

					.global min_addr, sorted_addr, orig_len
