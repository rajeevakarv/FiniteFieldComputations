#include "relic_fp_low.h"

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Eight word or (256-bit) addition
@ Usage:
@    void fp_muln_low128(uint32_t *C, uint32_t *B, uint32_t *A)
@ Operation:
@    C = B X A
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	.align	2
	.global	fp_muln_low128
	.type	fp_muln_low128, %function
fp_muln_low128:

	push	{r0-r12}
	push	{r14}

	pld		[r1, #32]
	pld		[r2, #32]


	/* col 0 */
	ldr     r12, [r1]
	ldr		r14, [r2]
		mov		r5, #0
	umull	r3, r4, r12, r14   /* a0*b0*/
		mov		r11, #0
		mov		r6, #0

	/* col 1 */
	ldr		r14, [r2, #4]
		mov		r10, #0
	umlal	r4, r5, r12, r14    /* a0*b1*/
		mov		r7, #0

	ldr     r12, [r1, #4]
	ldr		r14, [r2]
		mov		r9, #0
	umlal	r4, r11, r12, r14    /* a1*b0*/

		mov		r8, #0
		ldr     r12, [r1, #8]

	adds	r5, r5, r11
/* adc		r6, r6, #0 */

	/* col 2 */
	umlal	r5, r10, r12, r14     /* a2*b0*/

		mov		r11, #0
		ldr     r12, [r1, #4]
		ldr		r14, [r2, #4]

	adcs	r6, r6, r10
	adc		r7, r7, #0

	umlal	r5, r9, r12, r14       /* a1*b1*/

		mov		r10, #0
		ldr     r12, [r1]
		ldr		r14, [r2, #8]

	adds	r6, r6, r9
	adc		r7, r7, #0

	umlal	r5, r8, r12, r14     /* a0*b2*/

		mov		r9, #0
		ldr		r14, [r2, #12]

	adds	r6, r6, r8
	adcs	r7, r7, #0


	/* col 3 */
	umlal	r6, r11, r12, r14      /* a0*b3*/

		mov		r8, #0
		ldr     r12, [r1, #4]
		ldr		r14, [r2, #8]

	adds	r7, r7, r11
	adc		r8, r8, #0

	umlal	r6, r10, r12, r14      /* a1*b2*/

		mov		r11, #0
		ldr     r12, [r1, #8]
		ldr		r14, [r2, #4]

	adds	r7, r7, r10
	adc		r8, r8, #0

	umlal	r6, r11, r12, r14        /* a2*b1*/

		mov		r10, #0
		ldr     r12, [r1, #12]
		ldr		r14, [r2]

	adds	r7, r7, r11
	adc		r8, r8, #0

	umlal	r6, r10, r12, r14       /* a3*b0*/

		mov		r11, #0
		ldr     r14, [r2, #4]

			stmia	r0!, {r3-r6}

	adds	r7, r7, r10
	adc		r8, r8, #0

			mov		r3, r7



	/* col 4 */
	umlal	r3, r11, r12, r14         /* a3*b1*/

		mov		r4, r8
		mov		r5, #0
		mov		r10, #0
		ldr     r12, [r1, #8]
		ldr		r14, [r2, #8]

	adds	r4, r4, r11
	adc		r5, r5, #0

	umlal	r3, r10, r12, r14        /* a2*b2*/

		mov		r6, #0
		mov		r11, #0
		ldr     r12, [r1, #4]
		ldr		r14, [r2, #12]

	adds	r4, r4, r10
	adc		r5, r5, #0
 
	umlal	r3, r11, r12, r14        /* a1*b3*/

		mov		r7, #0
		mov		r10, #0
		ldr     r12, [r1, #8]

	adds	r4, r4, r11
/*	adc		r5, r5, #0  */


	/* col 5 */
	umlal	r4, r10, r12, r14         /* a2*b3*/

		mov		r11, #0
		ldr     r12, [r1, #12]
		ldr		r14, [r2, #8]

	adcs	r5, r5, r10
	adc		r6, r6, #0

	umlal	r4, r11, r12, r14       /* a3*b2*/

		ldr		r14, [r2, #12]

	adds	r5, r5, r11
	adc		r6, r6, #0



	/* col 6 */
	umlal	r5, r6, r12, r14      /* a3*b3*/
	stmia	r0, {r3-r6}

	pop		{r14}
	pop		{r0-r12}
	mov		pc, lr
	
	bx	lr
	.size	fp_muln_low128, .-fp_muln_low128


@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Eight word or (128-bit) addition
@ Usage:
@    void fp_add128(uint32_t *C, uint32_t *B, uint32_t *A)
@ Operation:
@    C = B + A
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	.align	2
	.global	fp_add128
	.type	fp_add128, %function
fp_add128:
	push {r4-r10}

	LDM r1, {r3-r6}
	LDM r2, {r7-r10}
	ADDS r3,r7
	ADCS r4,r8
	ADCS r5,r9
	ADC  r6,r10
	STM r0, {r3-r6}

	pop {r4-r10}
	bx	lr
	.size	fp_add128, .-fp_add128	
	
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Eight word or (128-bit) addition
@ Usage:
@    void fp_add128_carry(uint32_t *B, uint32_t *A)
@ Operation:
@    B = B + A
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	.align	2
	.global	fp_add128_carry
	.type	fp_add128_carry, %function
fp_add128_carry:
	push {r4-r10}

	LDM r0, {r2-r6}
	LDM r1, {r7-r10}
	ADDS r2,r7
	ADCS r3,r8
	ADCS r4,r9
	ADCS r5,r10
	ADC  r6,#0
	STM r0, {r2-r6}

	pop {r4-r10}
	bx	lr
	.size	fp_add128_carry, .-fp_add128_carry
	
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Eight word or (128-bit) addition
@ Usage:
@    void fp_add128_save_carry(uint32_t *C, uint32_t *B, uint32_t *A)
@ Operation:
@    C = B + A
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	.align	2
	.global	fp_add128_save_carry
	.type	fp_add128_save_carry, %function
fp_add128_save_carry:
	push {r4-r10}

	LDM r1, {r3-r6}
	LDM r2, {r7-r10}
	ADDS r3,r7
	ADCS r4,r8
	ADCS r5,r9
	ADCS r6,r10
	MOV r7,#0
	ADC  r7,#0
	STM r0, {r3-r7}

	pop {r4-r10}
	bx	lr
	.size	fp_add128_save_carry, .-fp_add128_save_carry


@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Eight word or (256-bit) addition
@ Usage:
@    void fp_add_asm_low(uint32_t *C, uint32_t *B, uint32_t *A)
@ Operation:
@    C = B + A
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	.align	2
	.global	fp_add_asm_low
	.type	fp_add_asm_low, %function
fp_add_asm_low:
	push {r4-r9}

	LDM r1!, {r3-r6}
	LDM r2!, {r7-r10}
	ADDS r3,r7
	ADCS r4,r8
	ADCS r5,r9
	ADCS r6,r10
	STM r0!, {r3-r6}

	LDM r1, {r3-r6}
	LDM r2, {r7-r10}
	ADCS r3,r7
	ADCS r4,r8
	ADCS r5,r9
	ADC  r6,r10
	STM r0!, {r3-r6}

	pop {r4-r9}
	bx	lr
	.size	fp_add_asm_low, .-fp_add_asm_low

	
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Eight word or (256-bit) substraction
@ Usage:
@    void fp_sub_asm_low(uint32_t *B, uint32_t *A)
@ Operation:
@    B = B - A
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	.align	2
	.global	fp_sub_asm_low
	.type	fp_sub_asm_low, %function
fp_sub_asm_low:
	push {r4-r9}

	LDM r0, {r2-r5}
	LDM r1!, {r6-r9}
	SUBS r2,r6
	SBCS r3,r7
	SBCS r4,r8
	SBCS r5,r9
	STM r0!, {r2-r5}

	LDM r0, {r2-r5}
	LDM r1!, {r6-r9}
	SBCS r2,r6
	SBCS r3,r7
	SBCS r4,r8
	SBC  r5,r9
	STM r0!, {r2-r5}

	pop {r4-r9}
	bx	lr
	.size	fp_sub_asm_low, .-fp_sub_asm_low

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Eight word or (256-bit) addition
@ Usage:
@    void fp_add_asm_carry_low(uint32_t *B, uint32_t *A)
@ Operation:
@    B = B + A
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	.align	2
	.global	fp_add_asm_carry_low
	.type	fp_add_asm_carry_low, %function
fp_add_asm_carry_low:
	push {r4-r10}

	LDM r0, {r2-r5}
	LDM r1!, {r6-r9}
	ADDS r2,r6
	ADCS r3,r7
	ADCS r4,r8
	ADCS r5,r9
	STM r0!, {r2-r5}

	LDM r0, {r2-r6}
	LDM r1!, {r7-r10}
	ADCS r2,r7
	ADCS r3,r8
	ADCS r4,r9
	ADCS r5,r10
	ADC  r6,#0
	STM r0!, {r2-r6}

	pop {r4-r10}
	bx	lr
	.size	fp_add_asm_carry_low, .-fp_add_asm_carry_low
	
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Eight word or (256-bit) addition
@ Usage:
@    void fp_add_asm_carry_low_temp(uint32_t *B, uint32_t *A)
@ Operation:
@    B = B + A
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	.align	2
	.global	fp_add_asm_carry_low_temp
	.type	fp_add_asm_carry_low_temp, %function
fp_add_asm_carry_low_temp:
	push {r4-r11}

	LDM r0, {r2-r5}
	LDM r1!, {r6-r9}
	ADDS r2,r6
	ADCS r3,r7
	ADCS r4,r8
	ADCS r5,r9
	STM r0!, {r2-r5}

	LDM r0, {r2-r6}
	LDM r1!, {r7-r11}
	ADCS r2,r7
	ADCS r3,r8
	ADCS r4,r9
	ADCS r5,r10
	ADC  r6,r11
	STM r0!, {r2-r6}

	pop {r4-r11}
	bx	lr
	.size	fp_add_asm_carry_low_temp, .-fp_add_asm_carry_low_temp

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Eight word or (128-bit) addition
@ Usage:
@    void fp_carry_handler(uint32_t *A, uint32_t B)
@ Operation:
@    A = B^0xFFFFFFFF + 0x01
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	.align	2
	.global	fp_carry_handler
	.type	fp_carry_handler, %function
fp_carry_handler:
	push {r4-r10}

	LDM r0, {r3-r6}
	MOV r9, #0xFFFFFFFF
	EOR r8, r1, r9
	MOV r9, #0x01
	ADDS r7, r8, r9
	AND r3,r3,r7
	AND r4,r4,r7
	AND r5,r5,r7
	AND r6,r6,r7
	STM r0, {r3-r6}

	pop {r4-r10}
	bx	lr
	.size	fp_carry_handler, .-fp_carry_handler

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Eight word or (256-bit) substraction
@ Usage:
@    void fp_sub_asm_low_carry(uint32_t *B, uint32_t *A)
@ Operation:
@    B = B - A
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	.align	2
	.global	fp_sub_asm_low_carry
	.type	fp_sub_asm_low_carry, %function
fp_sub_asm_low_carry:
	push {r4-r10}

	LDM r0, {r2-r5}
	LDM r1!, {r6-r9}
	SUBS r2,r6
	SBCS r3,r7
	SBCS r4,r8
	SBCS r5,r9
	STM r0!, {r2-r5}

	LDM r0, {r2-r6}
	LDM r1!, {r7-r10}
	SBCS r2,r7
	SBCS r3,r8
	SBCS r4,r9
	SBCS r5,r10
	SBC	 r6, #0
	STM r0!, {r2-r6}

	pop {r4-r10}
	bx	lr
	.size	fp_sub_asm_low_carry, .-fp_sub_asm_low_carry
	
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Eight word or (256-bit) substraction
@ Usage:
@    void fp_sub_asm_low_carry_temp(uint32_t *B, uint32_t *A)
@ Operation:
@    B = B - A
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	.align	2
	.global	fp_sub_asm_low_carry_temp
	.type	fp_sub_asm_low_carry_temp, %function
fp_sub_asm_low_carry_temp:
	push {r4-r11}

	LDM r0, {r2-r5}
	LDM r1!, {r6-r9}
	SUBS r2,r6
	SBCS r3,r7
	SBCS r4,r8
	SBCS r5,r9
	STM r0!, {r2-r5}

	LDM r0, {r2-r6}
	LDM r1!, {r7-r11}
	SBCS r2,r7
	SBCS r3,r8
	SBCS r4,r9
	SBCS r5,r10
	SBC	 r6, r11
	STM r0!, {r2-r6}

	pop {r4-r11}
	bx	lr
	.size	fp_sub_asm_low_carry_temp, .-fp_sub_asm_low_carry_temp
.end
