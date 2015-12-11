@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Eight word or (256-bit) addition
@ Usage:
@    void ADD448_3ADDC(uint32_t *C, uint32_t *B, uint32_t *A)
@ Operation:
@    B = B + A
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	.align	2
	.global	ADD448_3ADDC
	.type	ADD448_3ADDC, %function
ADD448_3ADDC:
	push {r4-r12}

	LDM r0, {r3-r6}
	LDM r1!, {r8-r11}
	ADDS r3,r8
	ADCS r4,r9
	ADCS r5,r10
	ADCS r6,r11
	MOV  r12, #0
	ADC  r12,#0
	LDM r2!, {r8-r11}
	ADDS r3,r8
	ADCS r4,r9
	ADCS r5,r10
	ADCS r6,r11
	ADC  r12,#0
	STM r0!, {r3-r6}

	LDM r0, {r3-r6}
	LDM r1!, {r8-r11}
	ADDS r3,r12
	ADDS r3,r8
	ADCS r4,r9
	ADCS r5,r10
	ADCS r6,r11
	MOV  r12, #0
	ADC  r12,#0
	LDM r2!, {r8-r11}
	ADDS r3,r8
	ADCS r4,r9
	ADCS r5,r10
	ADCS r6,r11
	ADC  r12,#0
	STM r0!, {r3-r6}
	
	LDM r0, {r3-r6}
	LDM r1!, {r8-r11}
	ADDS r3,r12
	ADDS r3,r8
	ADCS r4,r9
	ADCS r5,r10
	ADCS r6,r11
	MOV  r12, #0
	ADC  r12,#0
	LDM r2!, {r8-r11}
	ADDS r3,r8
	ADCS r4,r9
	ADCS r5,r10
	ADCS r6,r11
	ADC  r12,#0
	STM r0!, {r3-r6}
	
	
	LDM r0, {r3-r5}
	LDM r1!, {r8-r9}
	ADDS r3,r12
	ADDS r3,r8
	ADCS r4,r9
	ADC  r5,#0
	LDM r2!, {r8-r9}
	ADDS r3,r8
	ADCS r4,r9
	ADC  r5,#0
	STM r0!, {r3-r5}

	pop {r4-r12}
	bx	lr
	.size	ADD448_3ADDC, .-ADD448_3ADDC

.align	2
.global fp_muln_low_asm_comba_446   /* Coma multiplication (optimized for FP_PRIME = 256)*/
.type	fp_muln_low_asm_comba_446  , %function

fp_muln_low_asm_comba_446:

push	{r0-r12}
push	{r14}

pld		[r1, #32]
pld		[r2, #32]


/* col 0 */
ldr     r12, [r1]
ldr		r14, [r2]
mov		r5, #0
umull	r3, r4, r12, r14
mov		r11, #0
mov		r6, #0

/* col 1 */
ldr		r14, [r2, #4]
mov		r10, #0
umlal	r4, r5, r12, r14
mov		r7, #0

ldr     r12, [r1, #4]
ldr		r14, [r2]
mov		r9, #0
umlal	r4, r11, r12, r14

mov		r8, #0
ldr     r12, [r1, #8]

adds	r5, r5, r11
adc		r6, r6, #0

/* col 2 */
umlal	r5, r10, r12, r14

mov		r11, #0
ldr     r12, [r1, #4]
ldr		r14, [r2, #4]

adds	r6, r6, r10
adc		r7, r7, #0

umlal	r5, r9, r12, r14

mov		r10, #0
ldr     r12, [r1]
ldr		r14, [r2, #8]

adds	r6, r6, r9
adc		r7, r7, #0

umlal	r5, r8, r12, r14

mov		r9, #0
ldr		r14, [r2, #12]

adds	r6, r6, r8
adcs	r7, r7, #0


/* col 3 */
umlal	r6, r11, r12, r14

mov		r8, #0
ldr     r12, [r1, #4]
ldr		r14, [r2, #8]

adds	r7, r7, r11
adc		r8, r8, #0

umlal	r6, r10, r12, r14

mov		r11, #0
ldr     r12, [r1, #8]
ldr		r14, [r2, #4]

adds	r7, r7, r10
adc		r8, r8, #0

umlal	r6, r11, r12, r14

mov		r10, #0
ldr     r12, [r1, #12]
ldr		r14, [r2]

adds	r7, r7, r11
adc		r8, r8, #0

umlal	r6, r10, r12, r14

mov		r11, #0
ldr     r12, [r1, #16]

stmia	r0!, {r3-r6}

adds	r7, r7, r10
adc		r8, r8, #0

mov		r3, r7




/* col 4 */
umlal	r3, r11, r12, r14

mov		r4, r8
mov		r5, #0
ldr     r12, [r1, #12]
ldr		r14, [r2, #4]
mov		r10, #0

adds	r4, r4, r11
adc		r5, r5, #0

umlal	r3, r10, r12, r14

mov		r6, #0
mov		r11, #0
ldr     r12, [r1, #8]
ldr		r14, [r2, #8]

adds	r4, r4, r10
adc		r5, r5, #0

umlal	r3, r11, r12, r14

mov		r7, #0
mov		r10, #0
ldr     r12, [r1, #4]
ldr		r14, [r2, #12]

adds	r4, r4, r11
adc		r5, r5, #0

umlal	r3, r10, r12, r14

mov		r11, #0
ldr     r12, [r1]
ldr		r14, [r2, #16]

adds	r4, r4, r10
adc		r5, r5, #0

umlal	r3, r11, r12, r14

mov		r8, #0
mov		r10, #0
ldr		r14, [r2, #20]

adds	r4, r4, r11
adc		r5, r5, #0


/* col 5 */
umlal	r4, r10, r12, r14

mov		r11, #0
ldr     r12, [r1, #4]
ldr		r14, [r2, #16]

adds	r5, r5, r10
adc		r6, r6, #0

umlal	r4, r11, r12, r14

mov		r10, #0
ldr     r12, [r1, #8]
ldr		r14, [r2, #12]

adds	r5, r5, r11
adc		r6, r6, #0

umlal	r4, r10, r12, r14

mov		r11, #0
ldr     r12, [r1, #12]
ldr		r14, [r2, #8]

adds	r5, r5, r10
adc		r6, r6, #0

umlal	r4, r11, r12, r14

mov		r10, #0
ldr     r12, [r1, #16]
ldr		r14, [r2, #4]

adds	r5, r5, r11
adc		r6, r6, #0

umlal	r4, r10, r12, r14

mov		r11, #0
ldr     r12, [r1, #20]
ldr		r14, [r2]

adds	r5, r5, r10
adc		r6, r6, #0


umlal	r4, r11, r12, r14

mov		r10, #0
ldr     r12, [r1, #24]

adds	r5, r5, r11
adc		r6, r6, #0


/* col 6 */
umlal	r5, r10, r12, r14

mov		r11, #0
ldr     r12, [r1, #20]
ldr		r14, [r2, #4]

adds	r6, r6, r10
adc		r7, r7, #0

umlal	r5, r11, r12, r14

mov		r10, #0
ldr     r12, [r1, #16]
ldr		r14, [r2, #8]

adds	r6, r6, r11
adc		r7, r7, #0

umlal	r5, r10, r12, r14

mov		r11, #0
ldr     r12, [r1, #12]
ldr		r14, [r2, #12]

adds	r6, r6, r10
adc		r7, r7, #0

umlal	r5, r11, r12, r14

mov		r10, #0
ldr     r12, [r1, #8]
ldr		r14, [r2, #16]

adds	r6, r6, r11
adc		r7, r7, #0

umlal	r5, r10, r12, r14

mov		r11, #0
ldr     r12, [r1, #4]
ldr		r14, [r2, #20]

adds	r6, r6, r10
adc		r7, r7, #0

umlal	r5, r11, r12, r14

mov		r10, #0
ldr     r12, [r1]
ldr		r14, [r2, #24]

adds	r6, r6, r11
adc		r7, r7, #0

umlal	r5, r10, r12, r14

mov		r11, #0
ldr		r14, [r2, #28]

adds	r6, r6, r10
adc		r7, r7, #0


/* col 7 */
umlal	r6, r11, r12, r14

mov		r10, #0
ldr     r12, [r1, #4]
ldr		r14, [r2, #24]

adds	r7, r7, r11
adc		r8, r8, #0

umlal	r6, r10, r12, r14

mov		r11, #0
ldr     r12, [r1, #8]
ldr		r14, [r2, #20]

adds	r7, r7, r10
adc		r8, r8, #0

umlal	r6, r11, r12, r14

mov		r10, #0
ldr     r12, [r1, #12]
ldr		r14, [r2, #16]

adds	r7, r7, r11
adc		r8, r8, #0

umlal	r6, r10, r12, r14

mov		r11, #0
ldr     r12, [r1, #16]
ldr		r14, [r2, #12]

adds	r7, r7, r10
adc		r8, r8, #0

umlal	r6, r11, r12, r14

mov		r10, #0
ldr     r12, [r1, #20]
ldr		r14, [r2, #8]

adds	r7, r7, r11
adc		r8, r8, #0

umlal	r6, r10, r12, r14

mov		r11, #0
ldr     r12, [r1, #24]
ldr		r14, [r2, #4]

adds	r7, r7, r10
adc		r8, r8, #0

umlal	r6, r11, r12, r14

mov		r10, #0
ldr     r12, [r1, #28]
ldr		r14, [r2]

adds	r7, r7, r11
adc		r8, r8, #0

umlal	r6, r10, r12, r14

mov		r11, #0
ldr		r12, [r1, #32]

stmia	r0!, {r3-r6}

adds	r7, r7, r10
adc		r8, r8, #0

mov		r3, r7




/* col 8 */
umlal	r3, r11, r12, r14 /* a8*b0 */

mov		r4, r8
mov		r5, #0
ldr     r12, [r1, #28] /* a7*b1 */
ldr		r14, [r2, #4]
mov		r10, #0

adds	r4, r4, r11
adc		r5, r5, #0

umlal	r3, r10, r12, r14

mov		r6, #0
mov		r11, #0
ldr     r12, [r1, #24] /* a6*b2 */
ldr		r14, [r2, #8]

adds	r4, r4, r10
adc		r5, r5, #0

umlal	r3, r11, r12, r14

mov		r7, #0
mov		r10, #0
ldr     r12, [r1, #20]
ldr		r14, [r2, #12]

adds	r4, r4, r11
adc		r5, r5, #0

umlal	r3, r10, r12, r14  /* a5*b3 */

mov		r11, #0
ldr     r12, [r1, #16]
ldr		r14, [r2, #16]

adds	r4, r4, r10
adc		r5, r5, #0

umlal	r3, r11, r12, r14  /* a4*b4*/

mov		r10, #0
ldr     r12, [r1, #12]
ldr		r14, [r2, #20]

adds	r4, r4, r11
adc		r5, r5, #0

umlal	r3, r10, r12, r14  /* a3*b5*/

mov		r11, #0
ldr     r12, [r1, #8]
ldr		r14, [r2, #24]

adds	r4, r4, r10
adc		r5, r5, #0

umlal	r3, r11, r12, r14  /* a2*b6*/

mov		r10, #0
ldr     r12, [r1, #4]
ldr		r14, [r2, #28]

adds	r4, r4, r11
adc		r5, r5, #0

umlal	r3, r10, r12, r14  /* a1*b7*/

mov		r11, #0
ldr     r12, [r1, #0]
ldr		r14, [r2, #32]

adds	r4, r4, r10
adc		r5, r5, #0

umlal	r3, r11, r12, r14  /* a0*b8*/

mov		r10, #0
ldr     r14, [r2, #36]

adds	r4, r4, r11
adc		r5, r5, #0

/* col 9 */
umlal	r4, r10, r12, r14 /* a0*b9*/

mov		r11, #0
ldr     r12, [r1, #4]
ldr		r14, [r2, #32]

adds	r5, r5, r10
adc		r6, r6, #0

umlal	r4, r11, r12, r14 /* a1*b8*/

mov		r10, #0
ldr     r12, [r1, #8]
ldr		r14, [r2, #28]

adds	r5, r5, r11
adc		r6, r6, #0

umlal	r4, r10, r12, r14 /* a2*b7*/

mov		r11, #0
ldr     r12, [r1, #12]
ldr		r14, [r2, #24]

adds	r5, r5, r10
adc		r6, r6, #0

umlal	r4, r11, r12, r14 /* a3*b6*/

mov		r10, #0
ldr     r12, [r1, #16]
ldr		r14, [r2, #20]

adds	r5, r5, r11
adc		r6, r6, #0

umlal	r4, r10, r12, r14 /* a4*b5*/

mov		r11, #0
ldr     r12, [r1, #20]
ldr		r14, [r2, #16]

adds	r5, r5, r10
adc		r6, r6, #0

umlal	r4, r11, r12, r14 /* a5*b4*/

mov		r10, #0
ldr     r12, [r1, #24]
ldr		r14, [r2, #12]

adds	r5, r5, r11
adc		r6, r6, #0

umlal	r4, r10, r12, r14 /* a6*b3*/

mov		r11, #0
ldr     r12, [r1, #28]
ldr		r14, [r2, #8]

adds	r5, r5, r10
adc		r6, r6, #0

umlal	r4, r11, r12, r14 /* a7*b2*/

mov		r10, #0
ldr     r12, [r1, #32]
ldr		r14, [r2, #4]

adds	r5, r5, r11
adc		r6, r6, #0

umlal	r4, r10, r12, r14 /* a8*b1*/

mov		r11, #0
ldr     r12, [r1, #36]
ldr		r14, [r2, #0]

adds	r5, r5, r10
adc		r6, r6, #0

umlal	r4, r11, r12, r14 /* a9*b0*/

mov		r10, #0
ldr		r12, [r1, #40]

adds	r5, r5, r11
adc		r6, r6, #0

/* col 10 */
umlal	r5, r10, r12, r14 /* a10*b0*/

mov		r11, #0
ldr     r12, [r1, #36]
ldr		r14, [r2, #4]

adds	r6, r6, r10
adc		r7, r7, #0

umlal	r5, r11, r12, r14  /* a9*b1*/

mov		r10, #0
ldr     r12, [r1, #32]
ldr		r14, [r2, #8]

adds	r6, r6, r11
adc		r7, r7, #0

umlal	r5, r10, r12, r14  /* a8*b2*/

mov		r11, #0
ldr     r12, [r1, #28]
ldr		r14, [r2, #12]

adds	r6, r6, r10
adc		r7, r7, #0

umlal	r5, r11, r12, r14  /* a7*b3*/

mov		r10, #0
ldr     r12, [r1, #24]
ldr		r14, [r2, #16]

adds	r6, r6, r11
adc		r7, r7, #0

umlal	r5, r10, r12, r14  /* a6*b4*/

mov		r11, #0
ldr     r12, [r1, #20]
ldr		r14, [r2, #20]

adds	r6, r6, r10
adc		r7, r7, #0

umlal	r5, r11, r12, r14  /* a5*b5*/

mov		r10, #0
ldr     r12, [r1, #16]
ldr		r14, [r2, #24]

adds	r6, r6, r11
adc		r7, r7, #0

umlal	r5, r10, r12, r14  /* a4*b6*/

mov		r11, #0
ldr     r12, [r1, #12]
ldr		r14, [r2, #28]

adds	r6, r6, r10
adc		r7, r7, #0

umlal	r5, r11, r12, r14  /* a3*b7*/

mov		r10, #0
ldr     r12, [r1, #8]
ldr		r14, [r2, #32]

adds	r6, r6, r11
adc		r7, r7, #0

umlal	r5, r10, r12, r14  /* a2*b8*/

mov		r11, #0
ldr     r12, [r1, #4]
ldr		r14, [r2, #36]

adds	r6, r6, r10
adc		r7, r7, #0

umlal	r5, r11, r12, r14  /* a1*b9*/

mov		r10, #0
ldr     r12, [r1, #0]
ldr		r14, [r2, #40]

adds	r6, r6, r11
adc		r7, r7, #0

umlal	r5, r10, r12, r14  /* a0*b10*/

mov		r11, #0
ldr		r14, [r2, #44]

adds	r6, r6, r10
adc		r7, r7, #0

/* col 11 */

umlal	r6, r11, r12, r14 /* a0*b11*/

mov		r10, #0
mov		r8, #0
ldr     r12, [r1, #4]
ldr		r14, [r2, #40]

adds	r7, r7, r11
adc		r8, r8, #0

umlal	r6, r10, r12, r14 /* a1*b10*/

mov		r11, #0
ldr     r12, [r1, #8]
ldr		r14, [r2, #36]

adds	r7, r7, r10
adc		r8, r8, #0

umlal	r6, r11, r12, r14 /* a2*b9*/

mov		r10, #0
ldr     r12, [r1, #12]
ldr		r14, [r2, #32]

adds	r7, r7, r11
adc		r8, r8, #0

umlal	r6, r10, r12, r14 /* a3*b8*/

mov		r11, #0
ldr     r12, [r1, #16]
ldr		r14, [r2, #28]

adds	r7, r7, r10
adc		r8, r8, #0

umlal	r6, r11, r12, r14 /* a4*b7*/

mov		r10, #0
ldr     r12, [r1, #20]
ldr		r14, [r2, #24]

adds	r7, r7, r11
adc		r8, r8, #0

umlal	r6, r10, r12, r14 /* a5*b6*/

mov		r11, #0
ldr     r12, [r1, #24]
ldr		r14, [r2, #20]

adds	r7, r7, r10
adc		r8, r8, #0

umlal	r6, r11, r12, r14 /* a6*b5*/

mov		r10, #0
ldr     r12, [r1, #28]
ldr		r14, [r2, #16]

adds	r7, r7, r11
adc		r8, r8, #0

umlal	r6, r10, r12, r14 /* a7*b4*/

mov		r11, #0
ldr     r12, [r1, #32]
ldr		r14, [r2, #12]

adds	r7, r7, r10
adc		r8, r8, #0

umlal	r6, r11, r12, r14 /* a8*b3*/

mov		r10, #0
ldr     r12, [r1, #36]
ldr		r14, [r2, #8]

adds	r7, r7, r11
adc		r8, r8, #0

umlal	r6, r10, r12, r14 /* a9*b2*/

mov		r11, #0
ldr     r12, [r1, #40]
ldr		r14, [r2, #4]

adds	r7, r7, r10
adc		r8, r8, #0

umlal	r6, r11, r12, r14 /* a10*b1*/

mov		r10, #0
ldr     r12, [r1, #44]
ldr		r14, [r2, #0]

adds	r7, r7, r11
adc		r8, r8, #0

umlal	r6, r10, r12, r14 /* a11*b0*/

mov		r11, #0
ldr		r12, [r1, #48]

stmia	r0!, {r3-r6}

adds	r7, r7, r10
adc		r8, r8, #0

mov		r3, r7

/* col 12 */
umlal	r3, r11, r12, r14 /* a12*b0*/

mov		r4, r8
mov		r5, #0
mov		r10, #0
ldr     r12, [r1, #44]
ldr		r14, [r2, #4]

adds	r4, r4, r11
adc		r5, r5, #0

umlal	r3, r10, r12, r14 /* a11*b1*/

mov		r11, #0
ldr     r12, [r1, #40]
ldr		r14, [r2, #8]

adds	r4, r4, r10
adc		r5, r5, #0

umlal	r3, r11, r12, r14 /* a10*b2*/

mov		r10, #0
ldr     r12, [r1, #36]
ldr		r14, [r2, #12]

adds	r4, r4, r11
adc		r5, r5, #0

umlal	r3, r10, r12, r14 /* a9*b3*/

mov		r11, #0
ldr     r12, [r1, #32]
ldr		r14, [r2, #16]

adds	r4, r4, r10
adc		r5, r5, #0

umlal	r3, r11, r12, r14 /* a8*b4*/

mov		r10, #0
ldr     r12, [r1, #28]
ldr		r14, [r2, #20]

adds	r4, r4, r11
adc		r5, r5, #0

umlal	r3, r10, r12, r14 /* a7*b5*/

mov		r11, #0
ldr     r12, [r1, #24]
ldr		r14, [r2, #24]

adds	r4, r4, r10
adc		r5, r5, #0

umlal	r3, r11, r12, r14 /* a6*b6*/

mov		r10, #0
ldr     r12, [r1, #20]
ldr		r14, [r2, #28]

adds	r4, r4, r11
adc		r5, r5, #0

umlal	r3, r10, r12, r14 /* a5*b7*/

mov		r11, #0
ldr     r12, [r1, #16]
ldr		r14, [r2, #32]

adds	r4, r4, r10
adc		r5, r5, #0

umlal	r3, r11, r12, r14 /* a4*b8*/

mov		r10, #0
ldr     r12, [r1, #12]
ldr		r14, [r2, #36]

adds	r4, r4, r11
adc		r5, r5, #0

umlal	r3, r10, r12, r14 /* a3*b9*/

mov		r11, #0
ldr     r12, [r1, #8]
ldr		r14, [r2, #40]

adds	r4, r4, r10
adc		r5, r5, #0

umlal	r3, r11, r12, r14 /* a2*b10*/

mov		r10, #0
ldr     r12, [r1, #4]
ldr		r14, [r2, #44]

adds	r4, r4, r11
adc		r5, r5, #0

umlal	r3, r10, r12, r14 /* a1*b11*/

mov		r11, #0
ldr     r12, [r1, #0]
ldr		r14, [r2, #48]

adds	r4, r4, r10
adc		r5, r5, #0

umlal	r3, r11, r12, r14 /* a0*b12*/

mov		r6, #0
mov		r10, #0
ldr     r14, [r2, #52]

adds	r4, r4, r11
adc		r5, r5, #0


/* col 13 */

umlal	r4, r10, r12, r14 /* a0*b13*/

mov		r11, #0
ldr     r12, [r1, #4]
ldr		r14, [r2, #48]

adds	r5, r5, r10
adc		r6, r6, #0

umlal	r4, r11, r12, r14 /* a1*b12*/

mov		r10, #0
ldr     r12, [r1, #8]
ldr		r14, [r2, #44]

adds	r5, r5, r11
adc		r6, r6, #0

umlal	r4, r10, r12, r14 /* a2*b11*/

mov		r11, #0
ldr     r12, [r1, #12]
ldr		r14, [r2, #40]

adds	r5, r5, r10
adc		r6, r6, #0

umlal	r4, r11, r12, r14 /* a3*b10*/

mov		r10, #0
ldr     r12, [r1, #16]
ldr		r14, [r2, #36]

adds	r5, r5, r11
adc		r6, r6, #0

umlal	r4, r10, r12, r14 /* a4*b9*/

mov		r11, #0
ldr     r12, [r1, #20]
ldr		r14, [r2, #32]

adds	r5, r5, r10
adc		r6, r6, #0

umlal	r4, r11, r12, r14 /* a5*b8*/

mov		r10, #0
ldr     r12, [r1, #24]
ldr		r14, [r2, #28]

adds	r5, r5, r11
adc		r6, r6, #0

umlal	r4, r10, r12, r14 /* a6*b7*/

mov		r11, #0
ldr     r12, [r1, #28]
ldr		r14, [r2, #24]

adds	r5, r5, r10
adc		r6, r6, #0

umlal	r4, r11, r12, r14 /* a7*b6*/

mov		r10, #0
ldr     r12, [r1, #32]
ldr		r14, [r2, #20]

adds	r5, r5, r11
adc		r6, r6, #0

umlal	r4, r10, r12, r14 /* a8*b5*/

mov		r11, #0
ldr     r12, [r1, #36]
ldr		r14, [r2, #16]

adds	r5, r5, r10
adc		r6, r6, #0

umlal	r4, r11, r12, r14 /* a9*b4*/

mov		r10, #0
ldr     r12, [r1, #40]
ldr		r14, [r2, #12]

adds	r5, r5, r11
adc		r6, r6, #0

umlal	r4, r10, r12, r14 /* a10*b3*/

mov		r11, #0
ldr     r12, [r1, #44]
ldr		r14, [r2, #8]

adds	r5, r5, r10
adc		r6, r6, #0

umlal	r4, r11, r12, r14 /* a11*b2*/

mov		r10, #0
ldr     r12, [r1, #48]
ldr		r14, [r2, #4]

adds	r5, r5, r11
adc		r6, r6, #0

umlal	r4, r10, r12, r14 /* a12*b1*/

mov		r11, #0
ldr     r12, [r1, #52]
ldr		r14, [r2, #0]

adds	r5, r5, r10
adc		r6, r6, #0

umlal	r4, r11, r12, r14 /* a13*b0*/

mov		r10, #0
ldr		r14, [r2, #4]

adds	r5, r5, r11
adc		r6, r6, #0

/* col 14 */
umlal	r5, r10, r12, r14  /* a13*b1*/

mov		r7, #0
mov		r11, #0
ldr     r12, [r1, #48]
ldr		r14, [r2, #8]

adds	r6, r6, r10
adc		r7, r7, #0

umlal	r5, r11, r12, r14 /* a12*b2*/

mov		r10, #0
ldr     r12, [r1, #44]
ldr		r14, [r2, #12]

adds	r6, r6, r11
adc		r7, r7, #0

umlal	r5, r10, r12, r14 /* a11*b3*/

mov		r11, #0
ldr     r12, [r1, #40]
ldr		r14, [r2, #16]

adds	r6, r6, r10
adc		r7, r7, #0

umlal	r5, r11, r12, r14 /* a10*b4*/

mov		r10, #0
ldr     r12, [r1, #36]
ldr		r14, [r2, #20]

adds	r6, r6, r11
adc		r7, r7, #0

umlal	r5, r10, r12, r14 /* a9*b5*/

mov		r11, #0
ldr     r12, [r1, #32]
ldr		r14, [r2, #24]

adds	r6, r6, r10
adc		r7, r7, #0

umlal	r5, r11, r12, r14 /* a8*b6*/

mov		r10, #0
ldr     r12, [r1, #28]
ldr		r14, [r2, #28]

adds	r6, r6, r11
adc		r7, r7, #0

umlal	r5, r10, r12, r14 /* a7*b7*/

mov		r11, #0
ldr     r12, [r1, #24]
ldr		r14, [r2, #32]

adds	r6, r6, r10
adc		r7, r7, #0

umlal	r5, r11, r12, r14 /* a6*b8*/

mov		r10, #0
ldr     r12, [r1, #20]
ldr		r14, [r2, #36]

adds	r6, r6, r11
adc		r7, r7, #0

umlal	r5, r10, r12, r14 /* a5*b9*/

mov		r11, #0
ldr     r12, [r1, #16]
ldr		r14, [r2, #40]

adds	r6, r6, r10
adc		r7, r7, #0

umlal	r5, r11, r12, r14 /* a4*b10*/

mov		r10, #0
ldr     r12, [r1, #12]
ldr		r14, [r2, #44]

adds	r6, r6, r11
adc		r7, r7, #0

umlal	r5, r10, r12, r14 /* a3*b11*/

mov		r11, #0
ldr     r12, [r1, #8]
ldr		r14, [r2, #48]

adds	r6, r6, r10
adc		r7, r7, #0

umlal	r5, r11, r12, r14 /* a2*b12*/

mov		r10, #0
ldr     r12, [r1, #4]
ldr		r14, [r2, #52]

adds	r6, r6, r11
adc		r7, r7, #0

umlal	r5, r10, r12, r14 /* a1*b13*/

mov		r11, #0
ldr     r12, [r1, #8]

adds	r6, r6, r10
adc		r7, r7, #0

/* col 15 */

umlal	r6, r11, r12, r14 /* a2*b13*/

mov		r10, #0
mov		r8, #0
ldr     r12, [r1, #12]
ldr		r14, [r2, #48]

adds	r7, r7, r11
adc		r8, r8, #0

umlal	r6, r10, r12, r14 /* a3*b12*/

mov		r11, #0
ldr     r12, [r1, #16]
ldr		r14, [r2, #44]

adds	r7, r7, r10
adc		r8, r8, #0

umlal	r6, r11, r12, r14 /* a4*b11*/

mov		r10, #0
ldr     r12, [r1, #20]
ldr		r14, [r2, #40]

adds	r7, r7, r11
adc		r8, r8, #0

umlal	r6, r10, r12, r14 /* a5*b10*/

mov		r11, #0
ldr     r12, [r1, #24]
ldr		r14, [r2, #36]

adds	r7, r7, r10
adc		r8, r8, #0

umlal	r6, r11, r12, r14 /* a6*b9*/

mov		r10, #0
ldr     r12, [r1, #28]
ldr		r14, [r2, #32]

adds	r7, r7, r11
adc		r8, r8, #0

umlal	r6, r10, r12, r14 /* a7*b8*/

mov		r11, #0
ldr     r12, [r1, #32]
ldr		r14, [r2, #28]

adds	r7, r7, r10
adc		r8, r8, #0

umlal	r6, r11, r12, r14 /* a8*b7*/

mov		r10, #0
ldr     r12, [r1, #36]
ldr		r14, [r2, #24]

adds	r7, r7, r11
adc		r8, r8, #0

umlal	r6, r10, r12, r14 /* a9*b6*/

mov		r11, #0
ldr     r12, [r1, #40]
ldr		r14, [r2, #20]

adds	r7, r7, r10
adc		r8, r8, #0

umlal	r6, r11, r12, r14 /* a10*b5*/

mov		r10, #0
ldr     r12, [r1, #44]
ldr		r14, [r2, #16]

adds	r7, r7, r11
adc		r8, r8, #0

umlal	r6, r10, r12, r14 /* a11*b4*/

mov		r11, #0
ldr     r12, [r1, #48]
ldr		r14, [r2, #12]

adds	r7, r7, r10
adc		r8, r8, #0

umlal	r6, r11, r12, r14 /* a12*b3*/

mov		r10, #0
ldr     r12, [r1, #52]
ldr		r14, [r2, #8]

adds	r7, r7, r11
adc		r8, r8, #0

umlal	r6, r10, r12, r14 /* a13*b2*/

mov		r11, #0
ldr		r14, [r2, #12]

stmia	r0!, {r3-r6}

adds	r7, r7, r10
adc		r8, r8, #0

mov		r3, r7

/* col 16 */

umlal	r3, r11, r12, r14 /* a13*b3*/

mov		r4, r8
mov		r5, #0
mov		r10, #0
ldr     r12, [r1, #48]
ldr		r14, [r2, #16]

adds	r4, r4, r11
adc		r5, r5, #0

umlal	r3, r10, r12, r14 /* a12*b4*/

mov		r11, #0
ldr     r12, [r1, #44]
ldr		r14, [r2, #20]

adds	r4, r4, r10
adc		r5, r5, #0

umlal	r3, r11, r12, r14 /* a11*b5*/

mov		r10, #0
ldr     r12, [r1, #40]
ldr		r14, [r2, #24]

adds	r4, r4, r11
adc		r5, r5, #0

umlal	r3, r10, r12, r14 /* a10*b6*/

mov		r11, #0
ldr     r12, [r1, #36]
ldr		r14, [r2, #28]

adds	r4, r4, r10
adc		r5, r5, #0

umlal	r3, r11, r12, r14 /* a9*b7*/

mov		r10, #0
ldr     r12, [r1, #32]
ldr		r14, [r2, #32]

adds	r4, r4, r11
adc		r5, r5, #0

umlal	r3, r10, r12, r14 /* a8*b8*/

mov		r11, #0
ldr     r12, [r1, #28]
ldr		r14, [r2, #36]

adds	r4, r4, r10
adc		r5, r5, #0

umlal	r3, r11, r12, r14 /* a7*b9*/

mov		r10, #0
ldr     r12, [r1, #24]
ldr		r14, [r2, #40]

adds	r4, r4, r11
adc		r5, r5, #0

umlal	r3, r10, r12, r14 /* a6*b10*/

mov		r11, #0
ldr     r12, [r1, #20]
ldr		r14, [r2, #44]

adds	r4, r4, r10
adc		r5, r5, #0

umlal	r3, r11, r12, r14 /* a5*b11*/

mov		r10, #0
ldr     r12, [r1, #16]
ldr		r14, [r2, #48]

adds	r4, r4, r11
adc		r5, r5, #0

umlal	r3, r10, r12, r14 /* a4*b12*/

mov		r11, #0
ldr     r12, [r1, #12]
ldr		r14, [r2, #52]

adds	r4, r4, r10
adc		r5, r5, #0

umlal	r3, r11, r12, r14 /* a3*b13*/

mov		r10, #0
mov		r6, #0
ldr		r12, [r1, #16]

adds	r4, r4, r11
adc		r5, r5, #0

/*col 17 */

umlal	r4, r10, r12, r14 /* a4*b13*/

mov		r11, #0
ldr     r12, [r1, #20]
ldr		r14, [r2, #48]

adds	r5, r5, r10
adc		r6, r6, #0

umlal	r4, r11, r12, r14 /* a5*b12*/

mov		r10, #0
ldr     r12, [r1, #24]
ldr		r14, [r2, #44]

adds	r5, r5, r11
adc		r6, r6, #0

umlal	r4, r10, r12, r14 /* a6*b11*/

mov		r11, #0
ldr     r12, [r1, #28]
ldr		r14, [r2, #40]

adds	r5, r5, r10
adc		r6, r6, #0

umlal	r4, r11, r12, r14 /* a7*b10*/

mov		r10, #0
ldr     r12, [r1, #32]
ldr		r14, [r2, #36]

adds	r5, r5, r11
adc		r6, r6, #0

umlal	r4, r10, r12, r14 /* a8*b9*/

mov		r11, #0
ldr     r12, [r1, #36]
ldr		r14, [r2, #32]

adds	r5, r5, r10
adc		r6, r6, #0

umlal	r4, r11, r12, r14 /* a9*b8*/

mov		r10, #0
ldr     r12, [r1, #40]
ldr		r14, [r2, #28]

adds	r5, r5, r11
adc		r6, r6, #0

umlal	r4, r10, r12, r14 /* a10*b7*/

mov		r11, #0
ldr     r12, [r1, #44]
ldr		r14, [r2, #24]

adds	r5, r5, r10
adc		r6, r6, #0

umlal	r4, r11, r12, r14 /* a11*b6*/

mov		r10, #0
ldr     r12, [r1, #48]
ldr		r14, [r2, #20]

adds	r5, r5, r11
adc		r6, r6, #0

umlal	r4, r10, r12, r14 /* a12*b5*/

mov		r11, #0
ldr     r12, [r1, #52]
ldr		r14, [r2, #16]

adds	r5, r5, r10
adc		r6, r6, #0

umlal	r4, r11, r12, r14 /* a13*b4*/

mov		r10, #0
mov		r7, #0
ldr     r14, [r2, #20]

adds	r5, r5, r11
adc		r6, r6, #0


/* col 18 */

umlal	r5, r10, r12, r14 /* a13*b5*/

mov		r11, #0
ldr     r12, [r1, #48]
ldr		r14, [r2, #24]

adds	r6, r6, r10
adc		r7, r7, #0

umlal	r5, r11, r12, r14 /* a12*b6*/

mov		r10, #0
ldr     r12, [r1, #44]
ldr		r14, [r2, #28]

adds	r6, r6, r11
adc		r7, r7, #0

umlal	r5, r10, r12, r14 /* a11*b7*/

mov		r11, #0
ldr     r12, [r1, #40]
ldr		r14, [r2, #32]

adds	r6, r6, r10
adc		r7, r7, #0

umlal	r5, r11, r12, r14 /* a10*b8*/

mov		r10, #0
ldr     r12, [r1, #36]
ldr		r14, [r2, #36]

adds	r6, r6, r11
adc		r7, r7, #0

umlal	r5, r10, r12, r14 /* a9*b9*/

mov		r11, #0
ldr     r12, [r1, #32]
ldr		r14, [r2, #40]

adds	r6, r6, r10
adc		r7, r7, #0

umlal	r5, r11, r12, r14 /* a8*b10*/

mov		r10, #0
ldr     r12, [r1, #28]
ldr		r14, [r2, #44]

adds	r6, r6, r11
adc		r7, r7, #0

umlal	r5, r10, r12, r14 /* a7*b11*/

mov		r11, #0
ldr     r12, [r1, #24]
ldr		r14, [r2, #48]

adds	r6, r6, r10
adc		r7, r7, #0

umlal	r5, r11, r12, r14 /* a6*b12*/

mov		r10, #0
ldr     r12, [r1, #20]
ldr		r14, [r2, #52]

adds	r6, r6, r11
adc		r7, r7, #0

umlal	r5, r10, r12, r14 /* a5*b13*/

mov		r11, #0
/*mov		r8, #0*/
ldr		r12, [r1, #24]

adds	r6, r6, r10
adc		r7, r7, #0


/* col 19 */

umlal	r6, r11, r12, r14 /* a6*b13*/

mov		r10, #0
mov		r8, #0
ldr     r12, [r1, #28]
ldr		r14, [r2, #48]

adds	r7, r7, r11
adc		r8, r8, #0

umlal	r6, r10, r12, r14 /* a7*b12*/

mov		r11, #0
ldr     r12, [r1, #32]
ldr		r14, [r2, #44]

adds	r7, r7, r10
adc		r8, r8, #0

umlal	r6, r11, r12, r14 /* a8*b11*/

mov		r10, #0
ldr     r12, [r1, #36]
ldr		r14, [r2, #40]

adds	r7, r7, r11
adc		r8, r8, #0

umlal	r6, r10, r12, r14 /* a9*b10*/

mov		r11, #0
ldr     r12, [r1, #40]
ldr		r14, [r2, #36]

adds	r7, r7, r10
adc		r8, r8, #0

umlal	r6, r11, r12, r14 /* a10*b9*/

mov		r10, #0
ldr     r12, [r1, #44]
ldr		r14, [r2, #32]

adds	r7, r7, r11
adc		r8, r8, #0

umlal	r6, r10, r12, r14 /* a11*b8*/

mov		r11, #0
ldr     r12, [r1, #48]
ldr		r14, [r2, #28]

adds	r7, r7, r10
adc		r8, r8, #0

umlal	r6, r11, r12, r14 /* a12*b7*/

mov		r10, #0
ldr     r12, [r1, #52]
ldr		r14, [r2, #24]

adds	r7, r7, r11
adc		r8, r8, #0

umlal	r6, r10, r12, r14 /* a13*b6 */


mov		r11, #0
ldr		r14, [r2, #28]

stmia	r0!, {r3-r6}

adds	r7, r7, r10
adc		r8, r8, #0

mov		r3, r7

/* col 20 */
umlal	r3, r11, r12, r14 /* a13*b7 */

mov		r4, r8
mov		r5, #0
mov		r10, #0
ldr     r12, [r1, #48]
ldr		r14, [r2, #32]

adds	r4, r4, r11
adc		r5, r5, #0

umlal	r3, r10, r12, r14 /* a12*b8 */

mov		r11, #0
ldr     r12, [r1, #44]
ldr		r14, [r2, #36]

adds	r4, r4, r10
adc		r5, r5, #0

umlal	r3, r11, r12, r14 /* a11*b9 */

mov		r10, #0
ldr     r12, [r1, #40]
ldr		r14, [r2, #40]

adds	r4, r4, r11
adc		r5, r5, #0

umlal	r3, r10, r12, r14 /* a10*b10 */

mov		r11, #0
ldr     r12, [r1, #36]
ldr		r14, [r2, #44]

adds	r4, r4, r10
adc		r5, r5, #0

umlal	r3, r11, r12, r14 /* a9*b11 */

mov		r10, #0
ldr     r12, [r1, #32]
ldr		r14, [r2, #48]

adds	r4, r4, r11
adc		r5, r5, #0

umlal	r3, r10, r12, r14 /* a8*b12 */

mov		r11, #0
ldr     r12, [r1, #28]
ldr		r14, [r2, #52]

adds	r4, r4, r10
adc		r5, r5, #0

umlal	r3, r11, r12, r14 /* a7*b13 */

mov		r8, #0
mov		r10, #0
ldr     r12, [r1, #32]

adds	r4, r4, r11
adc		r5, r5, #0


/* col 21 */
umlal	r4, r10, r12, r14 /* a8*b13 */

mov		r11, #0
mov		r6, #0
ldr     r12, [r1, #36]
ldr		r14, [r2, #48]

adds	r5, r5, r10
adc		r6, r6, #0

umlal	r4, r11, r12, r14 /* a9*b12 */

mov		r10, #0
ldr     r12, [r1, #40]
ldr		r14, [r2, #44]

adds	r5, r5, r11
adc		r6, r6, #0

umlal	r4, r10, r12, r14 /* a10*b11 */

mov		r11, #0
ldr     r12, [r1, #44]
ldr		r14, [r2, #40]

adds	r5, r5, r10
adc		r6, r6, #0

umlal	r4, r11, r12, r14 /* a11*b10 */

mov		r10, #0
ldr     r12, [r1, #48]
ldr		r14, [r2, #36]

adds	r5, r5, r11
adc		r6, r6, #0

umlal	r4, r10, r12, r14 /* a12*b9 */

mov		r11, #0
ldr     r12, [r1, #52]
ldr		r14, [r2, #32]

adds	r5, r5, r10
adc		r6, r6, #0

umlal	r4, r11, r12, r14 /* a13*b8 */

mov		r10, #0
mov		r7, #0
ldr		r14, [r2, #36]

adds	r5, r5, r11
adc		r6, r6, #0

/* col 22 */
umlal	r5, r10, r12, r14 /* a13*b9 */

mov		r11, #0
ldr     r12, [r1, #48]
ldr		r14, [r2, #40]

adds	r6, r6, r10
adc		r7, r7, #0

umlal	r5, r11, r12, r14 /* a12*b10 */

mov		r10, #0
ldr     r12, [r1, #44]
ldr		r14, [r2, #44]

adds	r6, r6, r11
adc		r7, r7, #0

umlal	r5, r10, r12, r14 /* a11*b11 */

mov		r11, #0
ldr     r12, [r1, #40]
ldr		r14, [r2, #48]

adds	r6, r6, r10
adc		r7, r7, #0

umlal	r5, r11, r12, r14 /* a10*b12 */

mov		r10, #0
ldr     r12, [r1, #36]
ldr		r14, [r2, #52]

adds	r6, r6, r11
adc		r7, r7, #0

umlal	r5, r10, r12, r14 /* a9*b13 */

mov		r11, #0
ldr     r12, [r1, #40]

adds	r6, r6, r10
adc		r7, r7, #0

/* col 23 */

umlal	r6, r11, r12, r14 /* a10*b13 */

mov		r10, #0
ldr     r12, [r1, #44]
ldr		r14, [r2, #48]

adds	r7, r7, r11
adc		r8, r8, #0

umlal	r6, r10, r12, r14 /* a11*b12 */

mov		r11, #0
ldr     r12, [r1, #48]
ldr		r14, [r2, #44]

adds	r7, r7, r10
adc		r8, r8, #0

umlal	r6, r11, r12, r14 /* a12*b11 */

mov		r10, #0
ldr     r12, [r1, #52]
ldr		r14, [r2, #40]

adds	r7, r7, r11
adc		r8, r8, #0

umlal	r6, r10, r12, r14 /* a13*b10 */

mov		r11, #0
ldr		r14, [r2, #44]

stmia	r0!, {r3-r6}

adds	r7, r7, r10
adc		r8, r8, #0

mov		r3, r7

/* col 24 */
umlal	r3, r11, r12, r14 /* a13*b11 */

mov		r4, r8
mov		r5, #0
mov		r10, #0
ldr     r12, [r1, #48]
ldr		r14, [r2, #48]

adds	r4, r4, r11
adc		r5, r5, #0

umlal	r3, r10, r12, r14 /* a12*b12 */

mov		r11, #0
ldr     r12, [r1, #44]
ldr		r14, [r2, #52]

adds	r4, r4, r10
adc		r5, r5, #0

umlal	r3, r11, r12, r14 /* a11*b13 */

mov		r10, #0
ldr     r12, [r1, #48]

adds	r4, r4, r11
adc		r5, r5, #0

/* col 25 */

umlal	r4, r10, r12, r14 /* a12*b13 */

mov		r11, #0
mov		r6, #0
ldr     r12, [r1, #52]
ldr		r14, [r2, #48]

adds	r5, r5, r10
adc		r6, r6, #0

umlal	r4, r11, r12, r14 /* a13*b12 */

/*		mov		r10, #0 */
ldr		r14, [r2, #52]

adds	r5, r5, r11
adc		r6, r6, #0

/* col 26 */
umlal	r5, r6, r12, r14  /* a13*b13 */

stmia	r0, {r3-r6}

pop		{r14}
pop		{r0-r12}
mov		pc, lr



/*
 * Function: fp_muln_low_asm_comba_256
 * Inputs: r1 = a, r2 = b
 * Output: r0 = c = a*b
 */

.align	2
.global fp_muln_low_asm_comba_256   /*// Coma multiplication (optimized for FP_PRIME = 256)*/
.type	fp_muln_low_asm_comba_256  , %function

fp_muln_low_asm_comba_256:

	push	{r0-r12}
	push	{r14}

	pld		[r1, #32]
	pld		[r2, #32]


	/* col 0 */
	ldr     r12, [r1]
	ldr		r14, [r2]
		mov		r5, #0
	umull	r3, r4, r12, r14
		mov		r11, #0
		mov		r6, #0

	/* col 1 */
	ldr		r14, [r2, #4]
		mov		r10, #0
	umlal	r4, r5, r12, r14
		mov		r7, #0

	ldr     r12, [r1, #4]
	ldr		r14, [r2]
		mov		r9, #0
	umlal	r4, r11, r12, r14

		mov		r8, #0
		ldr     r12, [r1, #8]

	adds	r5, r5, r11
	adc		r6, r6, #0
	
	/* col 2 */
	umlal	r5, r10, r12, r14
	
		mov		r11, #0
		ldr     r12, [r1, #4]
		ldr		r14, [r2, #4]
	
	adds	r6, r6, r10
	adc		r7, r7, #0

	umlal	r5, r9, r12, r14

		mov		r10, #0
		ldr     r12, [r1]
		ldr		r14, [r2, #8]

	adds	r6, r6, r9
	adc		r7, r7, #0
	
	umlal	r5, r8, r12, r14

		mov		r9, #0
		ldr		r14, [r2, #12]

	adds	r6, r6, r8
	adcs	r7, r7, #0


	/* col 3 */
	umlal	r6, r11, r12, r14

		mov		r8, #0
		ldr     r12, [r1, #4]
		ldr		r14, [r2, #8]

	adds	r7, r7, r11
	adc		r8, r8, #0

	umlal	r6, r10, r12, r14

		mov		r11, #0
		ldr     r12, [r1, #8]
		ldr		r14, [r2, #4]

	adds	r7, r7, r10
	adc		r8, r8, #0
	
	umlal	r6, r11, r12, r14

		mov		r10, #0
		ldr     r12, [r1, #12]
		ldr		r14, [r2]

	adds	r7, r7, r11
	adc		r8, r8, #0

	umlal	r6, r10, r12, r14

		mov		r11, #0
		ldr     r12, [r1, #16]

			stmia	r0!, {r3-r6}

	adds	r7, r7, r10
	adc		r8, r8, #0

			mov		r3, r7

			
			
	
	/* col 4 */
	umlal	r3, r11, r12, r14
		
			mov		r4, r8
			mov		r5, #0
		ldr     r12, [r1, #12]
		ldr		r14, [r2, #4]
		mov		r10, #0

	adds	r4, r4, r11
	adc		r5, r5, #0

	umlal	r3, r10, r12, r14
		
			mov		r6, #0
		mov		r11, #0
		ldr     r12, [r1, #8]
		ldr		r14, [r2, #8]
	
	adds	r4, r4, r10
	adc		r5, r5, #0

	umlal	r3, r11, r12, r14

			mov		r7, #0
		mov		r10, #0
		ldr     r12, [r1, #4]
		ldr		r14, [r2, #12]

	adds	r4, r4, r11
	adc		r5, r5, #0

	umlal	r3, r10, r12, r14
	
		mov		r11, #0
		ldr     r12, [r1]
		ldr		r14, [r2, #16]
		
	adds	r4, r4, r10
	adc		r5, r5, #0

	umlal	r3, r11, r12, r14
		
			mov		r8, #0
		mov		r10, #0
		ldr		r14, [r2, #20]
		
	adds	r4, r4, r11
	adc		r5, r5, #0


	/* col 5 */
	umlal	r4, r10, r12, r14

		mov		r11, #0
		ldr     r12, [r1, #4]
		ldr		r14, [r2, #16]

	adds	r5, r5, r10
	adc		r6, r6, #0

	umlal	r4, r11, r12, r14

		mov		r10, #0
		ldr     r12, [r1, #8]
		ldr		r14, [r2, #12]

	adds	r5, r5, r11
	adc		r6, r6, #0
	
	umlal	r4, r10, r12, r14

		mov		r11, #0
		ldr     r12, [r1, #12]
		ldr		r14, [r2, #8]

	adds	r5, r5, r10
	adc		r6, r6, #0
	
	umlal	r4, r11, r12, r14

		mov		r10, #0
		ldr     r12, [r1, #16]
		ldr		r14, [r2, #4]

	adds	r5, r5, r11
	adc		r6, r6, #0

	umlal	r4, r10, r12, r14

		mov		r11, #0
		ldr     r12, [r1, #20]
		ldr		r14, [r2]

	adds	r5, r5, r10
	adc		r6, r6, #0

	
	umlal	r4, r11, r12, r14

		mov		r10, #0
		ldr     r12, [r1, #24]

	adds	r5, r5, r11
	adc		r6, r6, #0

	
	/* col 6 */
	umlal	r5, r10, r12, r14

		mov		r11, #0
		ldr     r12, [r1, #20]
		ldr		r14, [r2, #4]

	adds	r6, r6, r10
	adc		r7, r7, #0

	umlal	r5, r11, r12, r14

		mov		r10, #0
		ldr     r12, [r1, #16]
		ldr		r14, [r2, #8]

	adds	r6, r6, r11
	adc		r7, r7, #0

	umlal	r5, r10, r12, r14

		mov		r11, #0
		ldr     r12, [r1, #12]
		ldr		r14, [r2, #12]

	adds	r6, r6, r10
	adc		r7, r7, #0

	umlal	r5, r11, r12, r14

		mov		r10, #0
		ldr     r12, [r1, #8]
		ldr		r14, [r2, #16]

	adds	r6, r6, r11
	adc		r7, r7, #0

	umlal	r5, r10, r12, r14
	
		mov		r11, #0
		ldr     r12, [r1, #4]
		ldr		r14, [r2, #20]

	adds	r6, r6, r10
	adc		r7, r7, #0
	
	umlal	r5, r11, r12, r14

		mov		r10, #0
		ldr     r12, [r1]
		ldr		r14, [r2, #24]

	adds	r6, r6, r11
	adc		r7, r7, #0

	umlal	r5, r10, r12, r14

		mov		r11, #0
		ldr		r14, [r2, #28]

	adds	r6, r6, r10
	adc		r7, r7, #0


	/* col 7 */
	umlal	r6, r11, r12, r14

		mov		r10, #0
		ldr     r12, [r1, #4]
		ldr		r14, [r2, #24]

	adds	r7, r7, r11
	adc		r8, r8, #0

	umlal	r6, r10, r12, r14
	
		mov		r11, #0
		ldr     r12, [r1, #8]
		ldr		r14, [r2, #20]

	adds	r7, r7, r10
	adc		r8, r8, #0

	umlal	r6, r11, r12, r14

		mov		r10, #0
		ldr     r12, [r1, #12]
		ldr		r14, [r2, #16]

	adds	r7, r7, r11
	adc		r8, r8, #0

	umlal	r6, r10, r12, r14
	
		mov		r11, #0
		ldr     r12, [r1, #16]
		ldr		r14, [r2, #12]

	adds	r7, r7, r10
	adc		r8, r8, #0

	umlal	r6, r11, r12, r14
	
		mov		r10, #0
		ldr     r12, [r1, #20]
		ldr		r14, [r2, #8]

	adds	r7, r7, r11
	adc		r8, r8, #0

	umlal	r6, r10, r12, r14
	
		mov		r11, #0
		ldr     r12, [r1, #24]
		ldr		r14, [r2, #4]
	
	adds	r7, r7, r10
	adc		r8, r8, #0
	
	umlal	r6, r11, r12, r14
	
		mov		r10, #0
		ldr     r12, [r1, #28]
		ldr		r14, [r2]

	adds	r7, r7, r11
	adc		r8, r8, #0
	
	umlal	r6, r10, r12, r14
	
		mov		r11, #0
		ldr		r14, [r2, #4]
			
			stmia	r0!, {r3-r6}
	
	adds	r7, r7, r10
	adc		r8, r8, #0

			mov		r3, r7
	

	/* col 8 */
	umlal	r3, r11, r12, r14
	
			mov		r4, r8
			mov		r5, #0
		mov		r10, #0
		ldr     r12, [r1, #24]
		ldr		r14, [r2, #8]

	adds	r4, r4, r11
	adc		r5, r5, #0

	umlal	r3, r10, r12, r14

			mov		r6, #0
		mov		r11, #0
		ldr     r12, [r1, #20]
		ldr		r14, [r2, #12]

	adds	r4, r4, r10
	adc		r5, r5, #0

	umlal	r3, r11, r12, r14

			mov		r7, #0
		mov		r10, #0
		ldr     r12, [r1, #16]
		ldr		r14, [r2, #16]

	adds	r4, r4, r11
	adc		r5, r5, #0

	umlal	r3, r10, r12, r14
	
		mov		r11, #0
		ldr     r12, [r1, #12]
		ldr		r14, [r2, #20]

	adds	r4, r4, r10
	adc		r5, r5, #0

	umlal	r3, r11, r12, r14

		mov		r10, #0
		ldr     r12, [r1, #8]
		ldr		r14, [r2, #24]

	adds	r4, r4, r11
	adc		r5, r5, #0

	umlal	r3, r10, r12, r14

		mov		r11, #0
		ldr     r12, [r1, #4]
		ldr		r14, [r2, #28]

	adds	r4, r4, r10
	adc		r5, r5, #0

	umlal	r3, r11, r12, r14

			mov		r8, #0
		mov		r10, #0
		ldr     r12, [r1, #8]

	adds	r4, r4, r11
	adc		r5, r5, #0


	/* col 9 */
	umlal	r4, r10, r12, r14

		mov		r11, #0
		ldr     r12, [r1, #12]
		ldr		r14, [r2, #24]

	adds	r5, r5, r10
	adc		r6, r6, #0

	umlal	r4, r11, r12, r14
	
		mov		r10, #0
		ldr     r12, [r1, #16]
		ldr		r14, [r2, #20]

	adds	r5, r5, r11
	adc		r6, r6, #0

	umlal	r4, r10, r12, r14

		mov		r11, #0
		ldr     r12, [r1, #20]
		ldr		r14, [r2, #16]

	adds	r5, r5, r10
	adc		r6, r6, #0

	umlal	r4, r11, r12, r14
	
		mov		r10, #0
		ldr     r12, [r1, #24]
		ldr		r14, [r2, #12]

	adds	r5, r5, r11
	adc		r6, r6, #0

	umlal	r4, r10, r12, r14
	
		mov		r11, #0
		ldr     r12, [r1, #28]
		ldr		r14, [r2, #8]

	adds	r5, r5, r10
	adc		r6, r6, #0

	umlal	r4, r11, r12, r14
	
		mov		r10, #0
		ldr		r14, [r2, #12]

	adds	r5, r5, r11
	adc		r6, r6, #0

	
	/* col 10 */
	umlal	r5, r10, r12, r14

		mov		r11, #0
		ldr     r12, [r1, #24]
		ldr		r14, [r2, #16]

	adds	r6, r6, r10
	adc		r7, r7, #0

	umlal	r5, r11, r12, r14
	
		mov		r10, #0
		ldr     r12, [r1, #20]
		ldr		r14, [r2, #20]

	adds	r6, r6, r11
	adc		r7, r7, #0

	umlal	r5, r10, r12, r14
	
		mov		r11, #0
		ldr     r12, [r1, #16]
		ldr		r14, [r2, #24]

	adds	r6, r6, r10
	adc		r7, r7, #0

	umlal	r5, r11, r12, r14
	
		mov		r10, #0
		ldr     r12, [r1, #12]
		ldr		r14, [r2, #28]

	adds	r6, r6, r11
	adc		r7, r7, #0

	umlal	r5, r10, r12, r14
	
		mov		r11, #0
		ldr     r12, [r1, #16]

	adds	r6, r6, r10
	adc		r7, r7, #0


	/* col 11 */
	umlal	r6, r11, r12, r14
	
		mov		r10, #0
		ldr     r12, [r1, #20]
		ldr		r14, [r2, #24]

	adds	r7, r7, r11
	adc		r8, r8, #0

	umlal	r6, r10, r12, r14
	
		mov		r11, #0
		ldr     r12, [r1, #24]
		ldr		r14, [r2, #20]

	adds	r7, r7, r10
	adc		r8, r8, #0

	umlal	r6, r11, r12, r14
	
		mov		r10, #0
		ldr     r12, [r1, #28]
		ldr		r14, [r2, #16]

	adds	r7, r7, r11
	adc		r8, r8, #0

	umlal	r6, r10, r12, r14
	
		mov		r11, #0
		ldr		r14, [r2, #20]

			stmia	r0!, {r3-r6}

	adds	r7, r7, r10
	adc		r8, r8, #0

			mov		r3, r7

/*	mov		r7, #0
	mov		r8, #0 */
	
	/* col 12 */
	umlal	r3, r11, r12, r14

			mov		r4, r8
			mov		r5, #0
		mov		r10, #0
		ldr     r12, [r1, #24]
		ldr		r14, [r2, #24]

	adds	r4, r4, r11
	adc		r5, r5, #0

	umlal	r3, r10, r12, r14
	
			mov		r6, #0
		mov		r11, #0
		ldr     r12, [r1, #20]
		ldr		r14, [r2, #28]

	adds	r4, r4, r10
	adc		r5, r5, #0

	umlal	r3, r11, r12, r14
	
		mov		r10, #0
		ldr     r12, [r1, #24]

	adds	r4, r4, r11
	adc		r5, r5, #0

	/* col 13 */
	umlal	r4, r10, r12, r14
	
		mov		r11, #0
		ldr     r12, [r1, #28]
		ldr		r14, [r2, #24]

	adds	r5, r5, r10
	adc		r6, r6, #0

	umlal	r4, r11, r12, r14
	
/*		mov		r10, #0 */
		ldr		r14, [r2, #28]

	adds	r5, r5, r11
	adc		r6, r6, #0

	
	/* col 14 */
	umlal	r5, r6, r12, r14

/*	mov		r11, #0 */
	
	stmia	r0, {r3-r6}

	pop		{r14}
	pop		{r0-r12}
	mov		pc, lr

.end

