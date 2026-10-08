.syntax unified
	.arm
	.global IwramIrqMain
IwramIrqMain:
	mov	r3, #0x4000000
	ldr	r2, [r3, #512]!
	ldrh	r1, [r3, #8]
	mrs	r0, SPSR
	push	{r0, r1, r2, r3, r4, r5, lr}
	mvns	r0, r1, lsl #31
	strhmi	r0, [r3, #8]
	and	r1, r2, r2, lsr #16
	ands	r0, r1, #2
	ldrne	ip, [pc, #188]
	bne	.L_03000188
	ands	r0, r1, #1
	ldrne	ip, [pc, #172]
	bne	.L_03000188
	ands	r0, r1, #4
	ldrne	ip, [pc, #168]
	bne	.L_03000188
	ands	r0, r1, #64
	ldrne	ip, [pc, #172]
	bne	.L_03000188
	ands	r0, r1, #128
	ldrne	ip, [pc, #164]
	bne	.L_03000188
	ands	r0, r1, #16
	ldrne	ip, [pc, #140]
	bne	.L_03000188
	ands	r0, r1, #32
	ldrne	ip, [pc, #132]
	bne	.L_03000188
	ands	r0, r1, #0x1000
	ldrne	ip, [pc, #148]
	bne	.L_03000188
	ands	r0, r1, #0x2000
	ldr	ip, [pc, #140]
.L_03000188:
	bic	r2, r2, r0
	mov	r1, #0x2000
	orr	r1, r1, #196
	and	r1, r1, r2
	orr	r1, r1, r0, lsl #16
	str	r1, [r3]
	mrs	r3, CPSR
	bic	r3, r3, #223
	orr	r3, r3, #31
	msr	CPSR_fc, r3
	mov	r5, lr
	add	lr, pc, #1
	bx	ip
	.thumb
	mov	lr, r5
	bx	pc
	.arm
	mrs	r3, CPSR
	bic	r3, r3, #223
	orr	r3, r3, #146
	msr	CPSR_fc, r3
	pop	{r0, r1, r2, r3, r4, r5, lr}
	strh	r2, [r3]
	strh	r1, [r3, #8]
	msr	SPSR_fc, r0
	bx	lr
