#include "relic_fp_low.h"	
	
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Eight word or (128-bit) addition  taken
@ Usage:
@    void fp_add128_carry(uint32_t *B, uint32_t *A)
@ Operation:
@    B = B + A
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
	.align	2
	.global	fp_add128_carry
	.type	fp_add128_carry, %function
fp_add128_carry:
	SUB 	sp, sp, #32 
	STP	x3,x4,[sp, #16 * 0]
	STP	x5,x6,[sp, #16 * 1]

	LDP X3, X4, [X0]
	LDP X5, X6, [X1]
	
	ADDS X3,X3,X5
	ADCS X4,X4,X6
	STP  X3,X4,[X0]
	MOV  X3, #0
	MOV  X4, #0
	LDR  W3,[X0,#16]
	ADC  W3,W3,W4
	STR  W3, [x0,#16]

	LDP	x3,x4,[sp, #16 * 0]
	LDP	x5,x6,[sp, #16 * 1]
	ADD  	sp, sp, #32
	
	RET
	.size	fp_add128_carry, .-fp_add128_carry
	
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Eight word or (128-bit) addition taken
@ Usage:
@    void fp_add128_save_carry(uint32_t *C, uint32_t *B, uint32_t *A)
@ Operation:
@    C = B + A
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ */
	.align	2
	.global	fp_add128_save_carry
	.type	fp_add128_save_carry, %function
fp_add128_save_carry:
	SUB 	sp, sp, #48
	STP	x3,x4,[sp, #16 * 0]
	STP	x5,x6,[sp, #16 * 1]
	STP	X7,X8,[sp, #16 * 2]

	LDP X3, X4, [X1]
	LDP X5, X6, [X2]

	ADDS X3,X3,X5
	ADCS X4,X4,X6
	STP  X3,X4,[X0]
	MOV  X7,#0
	MOV  X8, #0
	ADC  W7,W7,W8
	STR  W7, [x0,#16]

	LDP	x3,x4,[sp, #16 * 0]
	LDP	x5,x6,[sp, #16 * 1]
	LDP	X7,X8,[sp, #16 * 2]
	ADD  	sp, sp, #48
	
	RET
	.size	fp_add128_save_carry, .-fp_add128_save_carry
 
	
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Eight word or (256-bit) addition  taken
@ Usage:
@    void fp_add_asm_carry_low_temp(uint32_t *B, uint32_t *A)
@ Operation:
@    B = B + A
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ */
	.align	2
	.global	fp_add_asm_carry_low_temp
	.type	fp_add_asm_carry_low_temp, %function
fp_add_asm_carry_low_temp:
	SUB 	sp, sp, #48
	STP	x3,x4,[sp, #16 * 0]
	STP	x5,x6,[sp, #16 * 1]
	STP	X7,X8,[sp, #16 * 2]

	LDP   X3, X4, [X0]
	LDP   X5, X6, [X1]
	ADDS  X3,X3,X5
	ADCS  X4,X4,X6
	STP   X3,X4,[X0]

	LDP   X3, X4, [X0,#16]
	LDP   X5, X6, [X1,#16]
	ADCS  X3,X3,X5
	ADCS  X4,X4,X6
	STP   X3,X4,[X0,#16]

	MOV   X3, #0
	MOV   X4, #0
	LDR   W3,[X0,#32]
	LDR   W4,[X1,#32]
	ADD   W3,W3,W4
	STR   W3,[X0,#32]

	LDP	x3,x4,[sp, #16 * 0]
	LDP	x5,x6,[sp, #16 * 1]
	LDP	X7,X8,[sp, #16 * 2]
	ADD  	sp, sp, #48
	
	RET
	.size	fp_add_asm_carry_low_temp, .-fp_add_asm_carry_low_temp


/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Eight word or (256-bit) substraction taken
@ Usage:
@    void fp_sub_asm_low_carry_temp(uint32_t *B, uint32_t *A)
@ Operation:
@    B = B - A
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ */
	.align	2
	.global	fp_sub_asm_low_carry_temp
	.type	fp_sub_asm_low_carry_temp, %function
fp_sub_asm_low_carry_temp:
	SUB 	sp, sp, #48
	STP	x3,x4,[sp, #16 * 0]
	STP	x5,x6,[sp, #16 * 1]
	STP	X7,X8,[sp, #16 * 2]

	LDP   X3, X4, [X0]
	LDP   X5, X6, [X1]
	SUBS  X3,X3,X5
	SBCS  X4,X4,X6
	STP   X3,X4,[X0]

	LDP   X3, X4, [X0, #16]
	LDP   X5, X6, [X1, #16]
	SBCS  X3,X3,X5
	SBCS  X4,X4,X6
	STP   X3,X4,[X0, #16]
	
	MOV   X3, #0
	MOV   X4, #0

	LDR   W3,[X0, #32]
	LDR   W4,[X1, #32]

	SBC   W3,W3,W4
  	STR   W3, [X0, #32]

	LDP	x3,x4,[sp, #16 * 0]
	LDP	x5,x6,[sp, #16 * 1]
	LDP	X7,X8,[sp, #16 * 2]
	ADD  	sp, sp, #48
	
	RET
	.size	fp_sub_asm_low_carry_temp, .-fp_sub_asm_low_carry_temp


/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Eight word or (128-bit) addition taken
@ Usage:
@    void fp_carry_handler(uint32_t *A, uint32_t B)
@ Operation:
@    A = B^0xFFFFFFFF + 0x01
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ */
	.align	2
	.global	fp_carry_handler
	.type	fp_carry_handler, %function
fp_carry_handler:
	SUB 	sp, sp, #32 
	STP	x3,x4,[sp, #16 * 0]
	STP	x5,x6,[sp, #16 * 1]
	
	LDP	X3,X4, [X0]
	MOV	X5, #0xFFFFFFFF
	EOR	W6,W1,W5
	MOV	X5, #0x01
	ADD	W6,W6,W5	
	AND	X3,X3,X6
	AND	X4,X4,X6
	STP	X3,X4,[X0]	

	LDP	x3,x4,[sp, #16 * 0]
	LDP	x5,x6,[sp, #16 * 1]
	ADD  	sp, sp, #32
	RET
	.size	fp_carry_handler, .-fp_carry_handler

/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Eight word or (256-bit) addition
@ Usage:
@    void fp_muln_low128(uint32_t *C, uint32_t *B, uint32_t *A)
@ Operation:
@    C = B X A
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/
	.align	2
	.global	fp_muln_low128
	.type	fp_muln_low128, %function
fp_muln_low128:

	SUB  sp, sp, #144
	STP	x3,x4,[sp, #16 * 0]
	STP	x5,x6,[sp, #16 * 1]
	STP	x7,x8,[sp, #16 * 2]
	STP	x9,x10,[sp,#16 * 3]
	STP	x11,x12,[sp,#16 * 4]
	STP	x13,x14,[sp,#16 * 5]
	STP	x15,x16,[sp,#16 * 6]
	STP	x17,x18,[sp,#16 * 7]
	STP	x19,x20,[sp,#16 * 8]

	/* col 0 */
	LDP	x12,x14, [x1]
	LDP	x16,x18, [x2]
	LSR	x13,x12, #32
	LSR	x15,x14, #32
	LSR	x17,x16, #32
	LSR	x19,x18, #32

	AND	x12,x12,#0xFFFFFFFF
	AND	x14,x14,#0xFFFFFFFF
	AND	x16,x16,#0xFFFFFFFF
	AND	x18,x18,#0xFFFFFFFF

	mov		x5, #0x00
	mov		x20, #0
	mov		x6, #0
	mov		x7, #0
	umull		x3, w12, w16   /* a0*b0*/
	lsr		x4,x3, #32

	/* col 1 */

	umull	x10, w12, w17    /* a0*b1*/
	add	x4,x4,x10

	umull	x11, w13, w16    /* a1*b0*/
	adds	x4,x4, x11
	adc	x5,x5,x20

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
	adc	x5,x5,x20
	
	umull	x11, w13, w17       /* a1*b1*/
	adds	x4, x4,x11
	adc	x5, x5, x20

	umull	x10, w12, w18     /* a0*b2*/
	adds	x4, x4,x10
	adc 	x5, x5, x20
	
	lsr	x7,x4, #32
	lsl	x5,x5, #32
	add	x5,x5,x7
	

	/* col 3 */
	umull	x11, w12, w19      /* a0*b3*/
	adds	x5,x5,x11
	adc	x6, x6, x20


	umull	x10, w13, w18      /* a1*b2*/
	adds	x5,x5,x10
	adc	x6,x6,x20


	umull	x11, w14, w17        /* a2*b1*/
	adds	x5,x5,x11
	adc	x6,x6,x20


	umull	x10, w15, w16       /* a3*b0*/
	adds	x5,x5,x10
	adc	x6,x6,x20
	
	mov 	x7, #0
	lsl	x7,x5,#32
	add	x4,x7,w4,UXTX #0
	mov 	x7, #0

	STP X3,X4,[X0]
	
	lsr	x5,x5,#32
	lsl	x6,x6,#32
	add	x5,x5,x6

	mov	x3,x5

	/* col 4 */
	umull	x11, w15, w17         /* a3*b1*/
	adds	x3,x3,x11	
	adc	x4, x4, x20

	umull	x10, w14, w18        /* a2*b2*/
	adds	x3, x3, x10
	adc	x4, x4, x20
 
	umull	x11, w13, w19        /* a1*b3*/
	adds	x3,x3,x11
	adc	x4, x4, x20	

	lsr	x7,x3,#32
	lsl	x4,x4,#32
	add	x4,x4,x7
	
	/* col 5 */
	umull	x10, w14, w19         /* a2*b3*/
	adds	x4,x4,x10
	adc	x5,x5,x20

	umull	x11, w15, w18       /* a3*b2*/
	adds	x4,x4,x11
	adc	x5,x5,x20

	mov	x7,#0
	lsl	x7, x4, #32
	add	x3,x7,w3,UXTX #0

	lsr	x4,x4,#32
	lsl	x5,x5, #32
	add	x4,x4,x5

	/* col 6 */
	umull	x10, w15, w19      /* a3*b3*/
	add	x4,x4,x10

	STP X3,X4,[X0,#16]

	LDP	x3,x4,[sp, #16 * 0]
	LDP	x5,x6,[sp, #16 * 1]
	LDP	x7,x8,[sp, #16 * 2]
	LDP	x9,x10,[sp, #16 * 3]
	LDP	x11,x12,[sp,#16 * 4]
	LDP	x13,x14,[sp,#16 * 5]
	LDP	x15,x16,[sp,#16 * 6]
	LDP	x17,x18,[sp,#16 * 7]
	LDP	x19,x20,[sp,#16 * 8]

	ADD  sp, sp, #144
	
	RET
	.size	fp_muln_low128, .-fp_muln_low128



	.align 2
	.global add256_3numbers
	.type add256_3numbers, %function

add256_3numbers:
	ldp x3, x4, [x0]
	ldp x5, x6, [x1]
	adds x7, x3, x5
	adcs x8, x4, x6
	mov x10, #0
	adc x10, x10, x10
	ldp x5, x6, [x2]
	adds x7, x7, x5
	adcs x8, x8, x6
	mov x11, #0
	adc  x10, x10, x11
	stp x7, x8, [x0]
	
	ldnp x3, x4, [x0, 16]
	ldnp x5, x6, [x1, 16]
	add  x3, x3, x10
	adds x7, x3, x5
	adcs x8, x4, x6
	mov x10, #0
	adc x10, x10, x11

	ldnp x5, x6, [x2, 16]
	adds x7, x7, x5
	adcs x8, x8, x6
 	adc x10, x10, x11
	stnp x7, x8, [x0,16]

	ldr x5, [x0, 32]
	add x5, x5, x10
	str x5, [x0, 32]
	ret
	.size add256_3numbers, .-add256_3numbers
	.align

.end
