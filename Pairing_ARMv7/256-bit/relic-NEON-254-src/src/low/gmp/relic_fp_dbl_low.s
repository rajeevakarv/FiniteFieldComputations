#include "relic_fp_low.h"

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Eight word or (256-bit) substraction
@ Usage:
@    void fp_dbl_low(uint32_t *B, uint32_t *A)
@ Operation:
@    B = B - A
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@	
	.text
	.align	2
	.global	fp_dbl_low
	.type	fp_dbl_low, %function
fp_dbl_low:
	vldm 		r1,{q0, q1}
	vtrn.32	q0, q3
	vshl.u64 	q4, q0, #1
	vshl.u64 	q5, q3, #1
	vtrn.32 	q4, q5
	vshl.u64 	q6, q5, #32
	vorr		q4, q6
	vshr.u64 	q6, q5, #32
	vorr		d9, d12

	veor		q3,q3,q3
	vtrn.32	q1, q3
	vshl.u64 	q5, q1, #1
	vshl.u64 	q8, q3, #1
	vtrn.32 	q5, q8
	vshl.u64 	q9, q8, #32
	vorr		q5, q9
	vshr.u64 	q9, q8, #32
	vorr		d11, d18
	vorr		d10, d13
	vstm		r0!, {q4, q5}
	vstm		r0, {d19}

	bx	lr
	.size	fp_dbl_low, .-fp_dbl_low