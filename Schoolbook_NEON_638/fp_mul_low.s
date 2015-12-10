@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ Eight word or (256-bit) addition
@ Usage:
@    void ADD638_3ADDC(uint32_t *C, uint32_t *B, uint32_t *A)
@ Operation:
@    B = B + A
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	.align	2
	.global	ADD638_3ADDC
	.type	ADD638_3ADDC, %function
ADD638_3ADDC:
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
	
	LDM r0, {r3-r7}
	LDM r1!, {r8-r11}
	ADDS r3,r12
	ADDS r3,r8
	ADCS r4,r9
	ADCS r5,r10
	ADCS r6,r11
	ADC  r7,#0
	LDM r2!, {r8-r11}
	ADDS r3,r8
	ADCS r4,r9
	ADCS r5,r10
	ADCS r6,r11
	ADC  r7,#0
	STM r0!, {r3-r7}

	pop {r4-r12}
	bx	lr
	.size	ADD638_3ADDC, .-ADD638_3ADDC
	.end

