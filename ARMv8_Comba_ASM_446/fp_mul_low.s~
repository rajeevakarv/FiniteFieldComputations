.align 2
.global fp_muln_low_asm_comba_446	// Modular addition
.type fp_muln_low_asm_comba_446, %function

fp_muln_low_asm_comba_446:
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
	umull		x3, w12, w16   /* a0*b0*/
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
	mov 	x7,#0
	mov	x6,#0
	mov	x5,#0
	mov	x4,#0


	/* col 8 */
	ldr w8,[x1, #32]
	umull	x11, w8, w16 	/* a8*b0 */
	adds	x3,x3,x11
	adc	x4,x4,x28

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

	ldr w8,[x2, #32]
	umull	x10, w12, w8  /* a0*b8*/
	adds	x3,x3,x10
	adc	x4,x4,x28

	lsr	x7,x3,#32
	AND	x3,x3,#0xFFFFFFFF
	lsl	x4,x4,#32
	add	x4,x4,x7


	/* col 9 */
	ldr w8,[x2, #36]
	umull	x10, w12, w8 /* a0*b9*/
	adds	x4,x4,x10
	adc	x5,x5,x28

	ldr w8,[x2, #32]
	umull	x11, w13, w8 /* a1*b8*/
	adds	x4,x4,x11
	adc	x5,x5,x28

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

	ldr w8,[x1, #32]
	umull	x10, w8, w17 /* a8*b1*/
	adds	x4,x4,x10
	adc	x5,x5,x28
	
	ldr w8,[x1, #36]
	umull	x11, w8, w16 /* a9*b0*/
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
	
	ldr w8,[x1, #40]
	umull	x10, w8, w16 /* a10*b0*/
	adds	x4,x4,x10
	adc	x5,x5,x28
	
	ldr w8,[x1, #36]
	umull	x11, w8, w17  /* a9*b1*/
	adds	x4,x4,x11
	adc	x5,x5,x28

	ldr w8,[x1, #32]
	umull	x10, w8, w18  /* a8*b2*/
	adds	x4,x4,x10
	adc	x5,x5,x28

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

	ldr w8,[x2, #32]
	umull	x10, w14, w8  /* a2*b8*/
	adds	x4,x4,x10
	adc	x5,x5,x28

	ldr w8,[x2, #36]
	umull	x11, w13, w8  /* a1*b9*/
	adds	x4,x4,x11
	adc	x5,x5,x28

	ldr w8,[x2, #40]
	umull	x10, w12, w8  /* a0*b10*/
	adds	x4,x4,x10
	adc	x5,x5,x28

	lsr	x7,x4, #32
	AND	x4,x4,#0xFFFFFFFF
	lsl	x5,x5, #32
	add	x5,x5,x7


	/* col 11 */

	ldr w8,[x2, #44]
	umull	x11, w12, w8 /* a0*b11*/
	adds	x5,x5,x11
	adc	x6,x6,x28

	ldr w8,[x2, #40]
	umull	x10, w13, w8 /* a1*b10*/
	adds	x5,x5,x10
	adc	x6,x6,x28

	ldr w8,[x2, #36]
	umull	x11, w14, w8 /* a2*b9*/
	adds	x5,x5,x11
	adc	x6,x6,x28

	ldr w8,[x2, #32]
	umull	x10, w15, w8 /* a3*b8*/
	adds	x5,x5,x10
	adc	x6,x6,x28

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

	ldr w8,[x1, #32]	
	umull	x11, w8, w19 /* a8*b3*/
	adds	x5,x5,x11
	adc	x6,x6,x28

	ldr w8,[x1, #36]
	umull	x10, w8, w18 /* a9*b2*/
	adds	x5,x5,x10
	adc	x6,x6,x28

	ldr w8,[x1, #40]
	umull	x11, w8, w17 /* a10*b1*/
	adds	x5,x5,x11
	adc	x6,x6,x28

	ldr w8,[x1, #44]
	umull	x10, w8, w16 /* a11*b0*/
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
	mov 	x7,#0
	mov	x6,#0
	mov	x5,#0
	mov	x4,#0


	/* col 12 */
	ldr w8,[x1, #48]
	umull	x11, w8, w16 /* a12*b0*/
	adds	x3,x3,x11
	adc	x4,x4,x28

	ldr w8,[x1, #44]
	umull	x10, w8, w17 /* a11*b1*/
	adds	x3,x3,x10
	adc	x4,x4,x28

	ldr w8,[x1, #40]
	umull	x11, w8, w18 /* a10*b2*/
	adds	x3,x3,x11
	adc	x4,x4,x28

	ldr w8,[x1, #36]
	umull	x10, w8, w19 /* a9*b3*/
	adds	x3,x3,x10
	adc	x4,x4,x28

	ldr w8,[x1, #32]
	umull	x11, w8, w24 /* a8*b4*/
	adds	x3,x3,x11
	adc	x4,x4,x28

	umull	x11, w23, w25	/*a7 b5 */
	adds	x3,x3,x11
	adc	x4,x4,x28

	umull	x10, w22, w26	/*a6 b6 */
	adds	x3,x3,x10
	adc	x4,x4,x28

	umull	x11, w21, w27	/*a5 b7 */
	adds	x3,x3,x11
	adc	x4,x4,x28

	ldr w8,[x2, #32]
	umull	x11, w20, w8 /* a4*b8*/
	adds	x3,x3,x11
	adc	x4,x4,x28

	ldr w8,[x2, #36]
	umull	x10, w15, w8 /* a3*b9*/
	adds	x3,x3,x10
	adc	x4,x4,x28

	ldr w8,[x2, #40]
	umull	x11, w14, w8 /* a2*b10*/
	adds	x3,x3,x11
	adc	x4,x4,x28

	ldr w8,[x2, #44]
	umull	x10, w13, w8 /* a1*b11*/
	adds	x3,x3,x10
	adc	x4,x4,x28

	ldr w8,[x2, #48]
	umull	x11, w12, w8 /* a0*b12*/
	adds	x3,x3,x11
	adc	x4,x4,x28

	lsr	x7,x3,#32
	AND	x3,x3,#0xFFFFFFFF
	lsl	x4,x4,#32
	add	x4,x4,x7


	/* col 13 */

	ldr w8,[x2, #52]
	umull	x10, w12, w8 /* a0*b13*/
	adds	x4,x4,x10
	adc	x5,x5,x28

	ldr w8,[x2, #48]
	umull	x11, w13, w8 /* a1*b12*/
	adds	x4,x4,x11
	adc	x5,x5,x28

	ldr w8,[x2, #44]
	umull	x10, w14, w8 /* a2*b11*/
	adds	x4,x4,x10
	adc	x5,x5,x28

	ldr w8,[x2, #40]
	umull	x11, w15, w8 /* a3*b10*/
	adds	x4,x4,x11
	adc	x5,x5,x28

	ldr w8,[x2, #36]
	umull	x10, w20, w8 /* a4*b9*/
	adds	x4,x4,x10
	adc	x5,x5,x28

	ldr w8,[x2, #32]
	umull	x11, w21, w8 /* a5*b8*/
	adds	x4,x4,x11
	adc	x5,x5,x28

	umull	x10, w22, w27	/*a6 b7 */
	adds	x4,x4,x10
	adc	x5,x5,x28

	umull	x11, w23, w26	/*a7 b6 */
	adds	x4,x4,x11
	adc	x5,x5,x28

	ldr w8,[x1, #32]
	umull	x10, w8, w25 /* a8*b5*/
	adds	x4,x4,x10
	adc	x5,x5,x28

	ldr w8,[x1, #36]
	umull	x11, w8, w24 /* a9*b4*/
	adds	x4,x4,x11
	adc	x5,x5,x28

	ldr w8,[x1, #40]
	umull	x10, w8, w19 /* a10*b3*/
	adds	x4,x4,x10
	adc	x5,x5,x28

	ldr w8,[x1, #44]
	umull	x11, w8, w18 /* a11*b2*/
	adds	x4,x4,x11
	adc	x5,x5,x28

	ldr w8,[x1, #48]
	umull	x10, w8, w17 /* a12*b1*/
	adds	x4,x4,x10
	adc	x5,x5,x28

	ldr w8,[x1, #52]
	umull	x11, w8, w16 /* a13*b0*/
	adds	x4,x4,x11
	adc	x5,x5,x28


	mov	x7,#0
	lsl	x7, x4, #32
	add	x3,x7,w3,UXTX #0

	lsr	x4,x4,#32
	lsl	x5,x5, #32
	add	x4,x4,x5


	/* col 14 */
	umull	x10, w8, w17  /* a13*b1*/
	adds	x4,x4,x10
	adc	x5,x5,x28

	ldr w8,[x1, #48]
	umull	x11, w8, w18 /* a12*b2*/
	adds	x4,x4,x11
	adc	x5,x5,x28

	ldr w8,[x1, #44]
	umull	x10, w8, w19 /* a11*b3*/
	adds	x4,x4,x10
	adc	x5,x5,x28

	ldr w8,[x1, #40]
	umull	x11, w8, w24 /* a10*b4*/
	adds	x4,x4,x11
	adc	x5,x5,x28

	ldr w8,[x1, #36]
	umull	x10, w8, w25 /* a9*b5*/
	adds	x4,x4,x10
	adc	x5,x5,x28

	ldr w8,[x1, #32]
	umull	x11, w8, w26 /* a8*b6*/
	adds	x4,x4,x11
	adc	x5,x5,x28

	umull	x10, w23, w27 /* a7*b7*/
	adds	x4,x4,x10
	adc	x5,x5,x28

	ldr w8,[x2, #32]
	umull	x11, w22, w8 /* a6*b8*/
	adds	x4,x4,x11
	adc	x5,x5,x28

	ldr w8,[x2, #36]
	umull	x10, w21, w8 /* a5*b9*/
	adds	x4,x4,x10
	adc	x5,x5,x28

	ldr w8,[x2, #40]
	umull	x11, w20, w8 /* a4*b10*/
	adds	x4,x4,x11
	adc	x5,x5,x28

	ldr w8,[x2, #44]
	umull	x10, w15, w8 /* a3*b11*/
	adds	x4,x4,x10
	adc	x5,x5,x28

	ldr w8,[x2, #48]
	umull	x11, w14, w8 /* a2*b12*/
	adds	x4,x4,x11
	adc	x5,x5,x28

	ldr w8,[x2, #52]
	umull	x10, w13, w8 /* a1*b13*/
	adds	x4,x4,x10
	adc	x5,x5,x28

	lsr	x7,x4, #32
	AND	x4,x4,#0xFFFFFFFF
	lsl	x5,x5, #32
	add	x5,x5,x7


	/* col 15 */

	umull	x11, w14, w8 /* a2*b13*/
	adds	x5,x5,x11
	adc	x6,x6,x28

	ldr w8,[x2, #48]
	umull	x10, w15, w8 /* a3*b12*/
	adds	x5,x5,x10
	adc	x6,x6,x28

	ldr w8,[x2, #44]
	umull	x11, w20, w8 /* a4*b11*/
	adds	x5,x5,x11
	adc	x6,x6,x28

	ldr w8,[x2, #40]
	umull	x10, w21, w8 /* a5*b10*/
	adds	x5,x5,x10
	adc	x6,x6,x28

	ldr w8,[x2, #36]
	umull	x11, w22, w8 /* a6*b9*/
	adds	x5,x5,x11
	adc	x6,x6,x28

	ldr w8,[x2, #32]
	umull	x10, w23, w8 /* a7*b8*/
	adds	x5,x5,x10
	adc	x6,x6,x28

	ldr w8,[x1, #32]
	umull	x11, w8, w27 /* a8*b7*/
	adds	x5,x5,x11
	adc	x6,x6,x28

	ldr w8,[x1, #36]
	umull	x10, w8, w26 /* a9*b6*/
	adds	x5,x5,x10
	adc	x6,x6,x28

	ldr w8,[x1, #40]
	umull	x11, w8, w25 /* a10*b5*/
	adds	x5,x5,x11
	adc	x6,x6,x28

	ldr w8,[x1, #44]
	umull	x10, w8, w24 /* a11*b4*/
	adds	x5,x5,x10
	adc	x6,x6,x28

	ldr w8,[x1, #48]
	umull	x11, w8, w19 /* a12*b3*/
	adds	x5,x5,x11
	adc	x6,x6,x28

	ldr w8,[x1, #52]
	umull	x10, w8, w18 /* a13*b2*/
	adds	x5,x5,x10
	adc	x6,x6,x28

	mov 	x7, #0
	lsl	x7,x5,#32
	add	x4,x7,w4,UXTX #0

	STP x3,x4,[X0, #48]
	
	lsr	x5,x5,#32
	lsl	x6,x6,#32
	add	x5,x5,x6

	mov	x3,x5
	mov 	x7,#0
	mov	x6,#0
	mov	x5,#0
	mov	x4,#0


	LDP		x12,x14, [x1, #16]
	LDP		x20,x22, [x1, #32]
	LDP		x16,x18, [x2, #16]
	LDP		x24,x26, [x2, #32]
	
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
	


	/* col 16 */

	ldr w9,[x2, #12]
	umull	x11, w8, w9 /* a13*b3*/
	adds	x3,x3,x11
	adc	x4,x4,x28

	ldr w8,[x1, #48]
	umull	x10, w8, w16 /* a12*b4*/
	adds	x3,x3,x10
	adc	x4,x4,x28

	umull	x11, w23, w17 /* a11*b5*/
	adds	x3,x3,x11
	adc	x4,x4,x28

	umull 	x10, w22, w18 /* a10*b6*/
	adds	x3,x3,x10
	adc	x4,x4,x28

	umull	x11, w21, w19 /* a9*b7*/
	adds	x3,x3,x11
	adc	x4,x4,x28

	umull	x10, w20, w24 /* a8*b8*/
	adds	x3,x3,x10
	adc	x4,x4,x28

	umull	x11, w15, w25 /* a7*b9*/
	adds	x3,x3,x11
	adc	x4,x4,x28

	umull	x10, w14, w26 /* a6*b10*/
	adds	x3,x3,x10
	adc	x4,x4,x28

	umull	x11, w13, w27 /* a5*b11*/
	adds	x3,x3,x11
	adc	x4,x4,x28

	ldr w9,[x2, #48]
	umull	x10, w12, w9 /* a4*b12*/
	adds	x3,x3,x10
	adc	x4,x4,x28

	ldr w8,[x1, #12]
	ldr w9,[x2, #52]
	umull	x11, w8, w9 /* a3*b13*/
	adds	x3,x3,x11
	adc	x4,x4,x28

	lsr	x7,x3,#32
	AND	x3,x3,#0xFFFFFFFF
	lsl	x4,x4,#32
	add	x4,x4,x7


	/*col 17 */

	umull	x10, w12, w9 /* a4*b13*/
	adds	x4,x4,x10
	adc	x5,x5,x28

	ldr w8,[x2, #48]
	umull	x11, w13, w8 /* a5*b12*/
	adds	x4,x4,x11
	adc	x5,x5,x28

	umull	x10, w14, w27 /* a6*b11*/
	adds	x4,x4,x10
	adc	x5,x5,x28

	umull	x11, w15, w26 /* a7*b10*/
	adds	x4,x4,x11
	adc	x5,x5,x28

	umull	x10, w20, w25 /* a8*b9*/
	adds	x4,x4,x10
	adc	x5,x5,x28

	umull	x11, w21, w24 /* a9*b8*/
	adds	x4,x4,x11
	adc	x5,x5,x28

	umull	x10, w22, w19 /* a10*b7*/
	adds	x4,x4,x10
	adc	x5,x5,x28

	umull	x11, w23, w18 /* a11*b6*/
	adds	x4,x4,x11
	adc	x5,x5,x28

	ldr w8,[x1, #48]
	umull	x10, w8, w17 /* a12*b5*/
	adds	x4,x4,x10
	adc	x5,x5,x28

	ldr w8,[x1, #52]
	umull	x11, w8, w16 /* a13*b4*/
	adds	x4,x4,x11
	adc	x5,x5,x28

	mov	x7,#0
	lsl	x7, x4, #32
	add	x3,x7,w3,UXTX #0

	lsr	x4,x4,#32
	lsl	x5,x5, #32
	add	x4,x4,x5


	/* col 18 */

	umull	x10, w8, w17 /* a13*b5*/
	adds	x4,x4,x10
	adc	x5,x5,x28

	ldr w8,[x1, #48]
	umull	x11, w8, w18 /* a12*b6*/
	adds	x4,x4,x11
	adc	x5,x5,x28

	umull	x10, w23, w19 /* a11*b7*/
	adds	x4,x4,x10
	adc	x5,x5,x28

	umull	x11, w22, w24 /* a10*b8*/
	adds	x4,x4,x11
	adc	x5,x5,x28

	umull	x10, w21, w25 /* a9*b9*/
	adds	x4,x4,x10
	adc	x5,x5,x28

	umull	x11, w20, w26 /* a8*b10*/
	adds	x4,x4,x11
	adc	x5,x5,x28

	umull	x10, w15, w27 /* a7*b11*/
	adds	x4,x4,x10
	adc	x5,x5,x28

	ldr w8,[x2, #48]
	umull	x11, w14, w8 /* a6*b12*/
	adds	x4,x4,x11
	adc	x5,x5,x28

	ldr w8,[x2, #52]
	umull	x10, w13, w8 /* a5*b13*/
	adds	x4,x4,x10
	adc	x5,x5,x28

	lsr	x7,x4, #32
	AND	x4,x4,#0xFFFFFFFF
	lsl	x5,x5, #32
	add	x5,x5,x7



	/* col 19 */

	umull	x11, w14, w8 /* a6*b13*/
	adds	x5,x5,x11
	adc	x6,x6,x28

	ldr w9,[x2, #48]
	umull	x10, w15, w9 /* a7*b12*/
	adds	x5,x5,x10
	adc	x6,x6,x28

	umull	x11, w20, w27 /* a8*b11*/
	adds	x5,x5,x11
	adc	x6,x6,x28

	umull	x10, w21, w26 /* a9*b10*/
	adds	x5,x5,x10
	adc	x6,x6,x28

	umull	x11, w22, w25 /* a10*b9*/
	adds	x5,x5,x11
	adc	x6,x6,x28

	umull	x10, w23, w24 /* a11*b8*/
	adds	x5,x5,x10
	adc	x6,x6,x28

	ldr w8,[x1, #48]
	umull	x11, w8, w19 /* a12*b7*/
	adds	x5,x5,x11
	adc	x6,x6,x28

	ldr w8,[x1, #52]
	umull	x10, w8, w18 /* a13*b6 */
	adds	x5,x5,x10
	adc	x6,x6,x28

	mov 	x7, #0
	lsl	x7,x5,#32
	add	x4,x7,w4,UXTX #0

	STP x3,x4,[X0, #64]
	
	lsr	x5,x5,#32
	lsl	x6,x6,#32
	add	x5,x5,x6

	mov	x3,x5
	mov 	x7,#0
	mov	x6,#0
	mov	x5,#0
	mov	x4,#0

	LDP		x12,x14, [x1, #24]
	LDP		x20,x22, [x1, #40]
	LDP		x16,x18, [x2, #24]
	LDP		x24,x26, [x2, #40]
	
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


	/* col 20 */
	
	umull	x11, w23, w17 /* a13*b7 */
	adds	x3,x3,x11
	adc	x4,x4,x28

	umull	x10, w22, w18 /* a12*b8 */
	adds	x3,x3,x10
	adc	x4,x4,x28

	umull	x11, w21, w19 /* a11*b9 */
	adds	x3,x3,x11
	adc	x4,x4,x28

	umull	x10, w20, w24 /* a10*b10 */
	adds	x3,x3,x10
	adc	x4,x4,x28

	umull	x11, w15, w25 /* a9*b11 */
	adds	x3,x3,x11
	adc	x4,x4,x28

	umull	x10, w14, w26 /* a8*b12 */
	adds	x3,x3,x10
	adc	x4,x4,x28

	umull	x11, w13, w27 /* a7*b13 */
	adds	x3,x3,x11
	adc	x4,x4,x28

	lsr	x7,x3,#32
	AND	x3,x3,#0xFFFFFFFF
	lsl	x4,x4,#32
	add	x4,x4,x7


	/* col 21 */
	umull	x10, w14, w27 /* a8*b13 */
	adds	x4,x4,x10
	adc	x5,x5,x28

	umull	x11, w15, w26 /* a9*b12 */
	adds	x4,x4,x11
	adc	x5,x5,x28

	umull	x10, w20, w25 /* a10*b11 */
	adds	x4,x4,x10
	adc	x5,x5,x28

	umull	x11, w21, w24 /* a11*b10 */
	adds	x4,x4,x11
	adc	x5,x5,x28

	umull	x10, w22, w19 /* a12*b9 */
	adds	x4,x4,x10
	adc	x5,x5,x28

	umull	x11, w23, w18 /* a13*b8 */
	adds	x4,x4,x11
	adc	x5,x5,x28

	mov	x7,#0
	lsl	x7, x4, #32
	add	x3,x7,w3,UXTX #0

	lsr	x4,x4,#32
	lsl	x5,x5, #32
	add	x4,x4,x5


	/* col 22 */
	umull	x10, w23, w19 /* a13*b9 */
	adds	x4,x4,x10
	adc	x5,x5,x28

	umull	x11, w22, w24 /* a12*b10 */
	adds	x4,x4,x11
	adc	x5,x5,x28

	umull	x10, w21, w25 /* a11*b11 */
	adds	x4,x4,x10
	adc	x5,x5,x28

	umull	x11, w20, w26 /* a10*b12 */
	adds	x4,x4,x11
	adc	x5,x5,x28

	umull	x10, w15, w27 /* a9*b13 */
	adds	x4,x4,x10
	adc	x5,x5,x28

	lsr	x7,x4, #32
	AND	x4,x4,#0xFFFFFFFF
	lsl	x5,x5, #32
	add	x5,x5,x7


	/* col 23 */

	umull	x11, w20, w27 /* a10*b13 */
	adds	x5,x5,x11
	adc	x6,x6,x28

	umull	x10, w21, w26 /* a11*b12 */
	adds	x5,x5,x10
	adc	x6,x6,x28

	umull	x11, w22, w25 /* a12*b11 */
	adds	x5,x5,x11
	adc	x6,x6,x28

	umull	x10, w23, w24 /* a13*b10 */
	adds	x5,x5,x10
	adc	x6,x6,x28

	mov 	x7, #0
	lsl	x7,x5,#32
	add	x4,x7,w4,UXTX #0

	STP x3,x4,[X0, #80]
	
	lsr	x5,x5,#32
	lsl	x6,x6,#32
	add	x5,x5,x6

	mov	x3,x5
	mov 	x7,#0
	mov	x6,#0
	mov	x5,#0
	mov	x4,#0


	/* col 24 */
	umull	x11, w23, w25 /* a13*b11 */
	adds	x3,x3,x11
	adc	x4,x4,x28

	umull	x10, w22, w26 /* a12*b12 */
	adds	x3,x3,x10
	adc	x4,x4,x28

	umull	x11, w21, w27 /* a11*b13 */
	adds	x3,x3,x11
	adc	x4,x4,x28

	lsr	x7,x3,#32
	AND	x3,x3,#0xFFFFFFFF
	lsl	x4,x4,#32
	add	x4,x4,x7

	/* col 25 */

	umull	x10, w22, w27 /* a12*b13 */
	adds	x4,x4,x10
	adc	x5,x5,x28

	umull	x11, w23, w26 /* a13*b12 */
	adds	x4,x4,x11
	adc	x5,x5,x28

	mov	x7,#0
	lsl	x7, x4, #32
	add	x3,x7,w3,UXTX #0

	lsr	x4,x4,#32
	lsl	x5,x5, #32
	add	x4,x4,x5

	/* col 26 */
	umull	x10, w23, w27  /* a13*b13 */
	add	x4,x4,x10

	STP x3,x4,[x0,#96]



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
	.size	fp_muln_low_asm_comba_446, .-fp_muln_low_asm_comba_446
.end
	
