.align 2
.global fp_muln_low_asm_comba_256	// Modular addition
.type fp_muln_low_asm_comba_256, %function

fp_muln_low_asm_comba_256:
		SUB  sp, sp, #208
	STP	x3,x4,[sp, #16 * 0]
	STP	x5,x6,[sp, #16 * 1]
	STP	x7,x8,[sp, #16 * 2]
	STP	x9,x10,[sp,#16 * 3]
	STP	x11,x12,[sp,#16 * 4]
	STP	x13,x14,[sp,#16 * 5]
	STP	x15,x16,[sp,#16 * 6]
	STP	x17,x18,[sp,#16 * 7]
	STP	x19,x20,[sp,#16 * 8]
	STP	x21,x22,[sp,#16 * 9]
	STP	x23,x24,[sp,#16 * 10]
	STP	x25,x26,[sp,#16 * 11]
	STP	x27,x28,[sp,#16 * 12]

	/* col 0 */
	LDP		x12,x14, [x1]
	LDP		x20,x22, [x1, #16]
	LDP		x16,x18, [x2]
	LDP		x24,x26, [x2, #16]
	
	LSR		x13,x12, #32
	LSR		x15,x14, #32
	LSR		x21,x20, #32
	LSR		x23,x22, #32
	LSR		x17,x16, #32
	LSR		x19,x18, #32
	LSR		x25,x24, #32
	LSR		x27,x26, #32

	AND	x12,x12,#0xFFFFFFFF
	AND	x14,x14,#0xFFFFFFFF
	AND	x16,x16,#0xFFFFFFFF
	AND	x18,x18,#0xFFFFFFFF
	AND	x20,x20,#0xFFFFFFFF
	AND	x22,x22,#0xFFFFFFFF
	AND	x24,x24,#0xFFFFFFFF
	AND	x26,x26,#0xFFFFFFFF

	mov		x5, #0
	mov		x6, #0
	mov		x7, #0
	mov		x28, #0	
	umull	x3, w12, w16   /* a0*b0*/
	lsr		x4,x3, #32
	AND		x3,x3,#0xFFFFFFFF

	/* col 1 */

	umull	x10, w12, w17    /* a0*b1*/
	add	x4,x4,x10

	umull	x11, w13, w16    /* a1*b0*/
	adds	x4,x4, x11
	adc	x5,x5,x28

	lsl	x7, x4, #32
	add	x3,x7,w3,UXTX #0

	lsr	x4,x4,#32
	lsl	x5,x5,#32
	add	x4,x4,x5

	mov	x7, #0
	mov	x5,#0

	/* col 2 */
	umull	x10, w14, w16     /* a2*b0*/
	adds	x4,x4,x10
	adc	x5,x5,x28
	
	umull	x11, w13, w17       /* a1*b1*/
	adds	x4, x4,x11
	adc	x5, x5, x28

	umull	x10, w12, w18     /* a0*b2*/
	adds	x4, x4,x10
	adc 	x5, x5, x28
	
	lsr	x7,x4, #32
	AND	x4,x4,#0xFFFFFFFF
	lsl	x5,x5, #32
	add	x5,x5,x7
	

	/* col 3 */
	umull	x11, w12, w19      /* a0*b3*/
	adds	x5,x5,x11
	adc	x6, x6, x28


	umull	x10, w13, w18      /* a1*b2*/
	adds	x5,x5,x10
	adc	x6,x6,x28


	umull	x11, w14, w17        /* a2*b1*/
	adds	x5,x5,x11
	adc	x6,x6,x28


	umull	x10, w15, w16       /* a3*b0*/
	adds	x5,x5,x10
	adc	x6,x6,x28
	
	mov 	x7, #0
	lsl	x7,x5,#32
	add	x4,x7,w4,UXTX #0
	mov 	x7, #0

	STP X3,X4,[X0]
	
	lsr	x5,x5,#32
	lsl	x6,x6,#32
	add	x5,x5,x6

	mov	x3,x5
	mov	x6,#0
	mov	x5,#0
	mov	x4,#0

	/* col 4 */
	umull	x11, w20, w16	/*a4 b0 */
	adds	x3,x3,x11
	adc	x4,x4,x28

	umull	x10, w15, w17	/*a3 b1 */
	adds	x3,x3,x10
	adc	x4,x4,x28
		
	umull	x11, w14, w18	/*a2 b2 */
	adds	x3,x3,x11
	adc	x4,x4,x28

	umull	x10, w13, w19	/*a1 b3 */
	adds	x3,x3,x10
	adc	x4,x4,x28
	
	umull	x11, w12, w24	/*a0 b4 */
	adds	x3,x3,x11
	adc	x4,x4,x28

	lsr	x7,x3,#32
	AND	x3,x3,#0xFFFFFFFF
	lsl	x4,x4,#32
	add	x4,x4,x7


	/* col 5 */
	umull	x10, w12, w25	/*a0 b5 */
	adds	x4,x4,x10
	adc	x5,x5,x28

	umull	x11, w13, w24	/*a1 b4 */
	adds	x4,x4,x11
	adc	x5,x5,x28
	
	umull	x10, w14, w19	/*a2 b3 */
	adds	x4,x4,x10
	adc	x5,x5,x28
	
	umull	x11, w15, w18	/*a3 b2 */
	adds	x4,x4,x11
	adc	x5,x5,x28

	umull	x10, w20, w17	/*a4 b1 */
	adds	x4,x4,x10
	adc	x5,x5,x28

	umull	x11, w21, w16	/*a5 b0 */
	adds	x4,x4,x11
	adc	x5,x5,x28

	mov	x7,#0
	lsl	x7, x4, #32
	add	x3,x7,w3,UXTX #0

	lsr	x4,x4,#32
	lsl	x5,x5, #32
	add	x4,x4,x5

	mov	x7,#0
	mov	x5,#0

	
	/* col 6 */
	umull	x10, w22, w16	/*a6 b0 */
	adds	x4,x4,x10
	adc	x5,x5,x28

	umull	x11, w21, w17	/*a5 b1 */
	adds	x4,x4,x11
	adc	x5,x5,x28

	umull	x10, w20, w18	/*a4 b2 */
	adds	x4,x4,x10
	adc	x5,x5,x28

	umull	x11, w15, w19	/*a3 b3 */
	adds	x4,x4,x11
	adc	x5,x5,x28
	
	umull	x10, w14, w24	/*a2 b4 */
	adds	x4,x4,x10
	adc	x5,x5,x28
	
	umull	x11, w13, w25	/*a1 b5 */
	adds	x4,x4,x11
	adc	x5,x5,x28

	umull	x10, w12, w26	/*a0 b6 */
	adds	x4,x4,x10
	adc	x5,x5,x28

	lsr	x7,x4, #32
	AND	x4,x4,#0xFFFFFFFF
	lsl	x5,x5, #32
	add	x5,x5,x7

	/* col 7 */
	umull	x11, w12, w27	/*a0 b7 */
	adds	x5,x5,x11
	adc	x6,x6,x28

	umull	x10, w13, w26	/*a1 b6 */
	adds	x5,x5,x10
	adc	x6,x6,x28

	umull	x11, w14, w25	/*a2 b5 */
	adds	x5,x5,x11
	adc	x6,x6,x28

	umull	x10, w15, w24	/*a3 b4 */
	adds	x5,x5,x10
	adc	x6,x6,x28

	umull	x11, w20, w19	/*a4 b3 */
	adds	x5,x5,x11
	adc	x6,x6,x28

	umull	x10, w21, w18	/*a5 b2 */
	adds	x5,x5,x10
	adc	x6,x6,x28
	
	umull	x11, w22, w17	/*a6 b1 */
	adds	x5,x5,x11
	adc	x6,x6,x28
	
	umull	x10, w23, w16	/*a7 b0 */
	adds	x5,x5,x10
	adc	x6,x6,x28
			
	mov 	x7, #0
	lsl	x7,x5,#32
	add	x4,x7,w4,UXTX #0

	STP X3,X4,[X0, #16]
	
	lsr	x5,x5,#32
	lsl	x6,x6,#32
	add	x5,x5,x6

	mov	x3,x5
	mov x7,#0
	mov	x6,#0
	mov	x5,#0
	mov	x4,#0


	/* col 8 */
	umull	x11, w23, w17	/*a7 b1 */
	adds	x3,x3,x11
	adc	x4,x4,x28

	umull	x10, w22, w18	/*a6 b2 */
	adds	x3,x3,x10
	adc	x4,x4,x28

	umull	x11, w21, w19	/*a5 b3 */
	adds	x3,x3,x11
	adc	x4,x4,x28

	umull	x10, w20, w24	/*a4 b4 */
	adds	x3,x3,x10
	adc	x4,x4,x28

	umull	x11, w15, w25	/*a3 b5 */
	adds	x3,x3,x11
	adc	x4,x4,x28

	umull	x10, w14, w26	/*a2 b6 */
	adds	x3,x3,x10
	adc	x4,x4,x28

	umull	x11, w13, w27	/*a1 b7 */
	adds	x3,x3,x11
	adc	x4,x4,x28

	lsr	x7,x3,#32
	AND	x3,x3,#0xFFFFFFFF
	lsl	x4,x4,#32
	add	x4,x4,x7

	/* col 9 */
	umull	x10, w14, w27	/*a2 b7 */
	adds	x4,x4,x10
	adc	x5,x5,x28

	umull	x11, w15, w26	/*a3 b6 */
	adds	x4,x4,x11
	adc	x5,x5,x28

	umull	x10, w20, w25	/*a4 b5 */
	adds	x4,x4,x10
	adc	x5,x5,x28

	umull	x11, w21, w24	/*a5 b4 */
	adds	x4,x4,x11
	adc	x5,x5,x28

	umull	x10, w22, w19	/*a6 b3 */
	adds	x4,x4,x10
	adc	x5,x5,x28

	umull	x11, w23, w18	/*a7 b2 */
	adds	x4,x4,x11
	adc	x5,x5,x28

	mov	x7,#0
	lsl	x7, x4, #32
	add	x3,x7,w3,UXTX #0

	lsr	x4,x4,#32
	lsl	x5,x5, #32
	add	x4,x4,x5

	mov	x7,#0
	mov	x5,#0
	


	/* col 10 */
	umull	x10, w23, w19	/*a7 b3 */
	adds	x4,x4,x10
	adc	x5,x5,x28

	umull	x11, w22, w24	/*a6 b4 */
	adds	x4,x4,x11
	adc	x5,x5,x28

	umull	x10, w21, w25	/*a5 b5 */
	adds	x4,x4,x10
	adc	x5,x5,x28

	umull	x11, w20, w26	/*a4 b6 */
	adds	x4,x4,x11
	adc	x5,x5,x28

	umull	x10, w15, w27	/*a3 b7 */
	adds	x4,x4,x10
	adc	x5,x5,x28

	lsr	x7,x4, #32
	AND	x4,x4,#0xFFFFFFFF
	lsl	x5,x5, #32
	add	x5,x5,x7

	/* col 11 */
	umull	x11, w20, w27	/*a4 b7 */
	adds	x5,x5,x11
	adc	x6,x6,x28

	umull	x10, w21, w26	/*a5 b6 */
	adds	x5,x5,x10
	adc	x6,x6,x28

	umull	x11, w22, w25	/*a6 b5 */
	adds	x5,x5,x11
	adc	x6,x6,x28

	umull	x10, w23, w24	/*a7 b4 */
	adds	x5,x5,x10
	adc	x6,x6,x28

	mov 	x7, #0
	lsl	x7,x5,#32
	add	x4,x7,w4,UXTX #0

	STP x3,x4,[X0, #32]
	
	lsr	x5,x5,#32
	lsl	x6,x6,#32
	add	x5,x5,x6

	mov	x3,x5
	mov x7,#0
	mov	x6,#0
	mov	x5,#0
	mov	x4,#0
	
	/* col 12 */
	umull	x11, w23, w25	/*a7 b5 */
	adds	x3,x3,x11
	adc	x4,x4,x28

	umull	x10, w22, w26	/*a6 b6 */
	adds	x3,x3,x10
	adc	x4,x4,x28

	umull	x11, w21, w27	/*a5 b7 */
	adds	x3,x3,x11
	adc	x4,x4,x28

	lsr	x7,x3,#32
	AND	x3,x3,#0xFFFFFFFF
	lsl	x4,x4,#32
	add	x4,x4,x7

	
	/* col 13 */
	umull	x10, w22, w27	/*a6 b7 */
	adds	x4,x4,x10
	adc	x5,x5,x28

	umull	x11, w23, w26	/*a7 b6 */
	adds	x4,x4,x11
	adc	x5,x5,x28

	mov	x7,#0
	lsl	x7, x4, #32
	add	x3,x7,w3,UXTX #0

	lsr	x4,x4,#32
	lsl	x5,x5, #32
	add	x4,x4,x5
	
	/* col 14 */
	umull	x10, w23, w27	/*a7 b7 */
	add	x4,x4,x10

	STP X3,X4,[X0,#48]


	LDP	x3,x4,[sp, #16 * 0]
	LDP	x5,x6,[sp, #16 * 1]
	LDP	x7,x8,[sp, #16 * 2]
	LDP	x9,x10,[sp, #16 * 3]
	LDP	x11,x12,[sp,#16 * 4]
	LDP	x13,x14,[sp,#16 * 5]
	LDP	x15,x16,[sp,#16 * 6]
	LDP	x17,x18,[sp,#16 * 7]
	LDP	x19,x20,[sp,#16 * 8]
	LDP	x21,x22,[sp,#16 * 9]
	LDP	x23,x24,[sp,#16 * 10]
	LDP	x25,x26,[sp,#16 * 11]
	LDP	x27,x28,[sp,#16 * 12]

	ADD  sp, sp, #208
	
	RET
	.size	fp_muln_low_asm_comba_256, .-fp_muln_low_asm_comba_256
.end
	
