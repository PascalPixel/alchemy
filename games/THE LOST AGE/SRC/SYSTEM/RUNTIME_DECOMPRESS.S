.syntax unified
	.arm
	.thumb
	.global IwramDecompress
IwramDecompress:
	bx	pc
AlchemyUncredited_08000c4e:
	.2byte	0x0200
	.arm
AlchemyUncreditedEnd_08000c4e:
	ldrb	r2, [r0], #1
	ldrb	r3, [r0], #1
	orrs	r2, r2, r3, lsl #8
	bxeq	lr
	push	{r5, r6, r7}
	mov	r6, r0
	add	r2, r2, r0
	sub	r2, r2, #2
.L_030006b8:
	ldrb	r3, [r2], #1
	orr	r3, r3, #0x100
.L_030006c0:
	lsrs	r3, r3, #1
	bcc	.L_030006e0
	beq	.L_030006b8
	ldrb	r4, [r0], #1
	subs	r5, r4, #223
	addhi	r1, r1, r5
	strbls	r4, [r1], #1
	b	.L_030006c0
.L_030006e0:
	ldrb	r4, [r2], #1
	ldrb	ip, [r2], #1
	orrs	r4, ip, r4, lsl #8
	beq	.L_03000724
	bic	ip, r4, #0xf000
	sub	ip, r6, ip
	lsrs	r4, r4, #12
	ldrbeq	r4, [r2], #1
	addeq	r4, r4, #16
	add	r4, r4, #1
.L_03000708:
	ldrb	r5, [ip], #1
	subs	r7, r5, #223
	addhi	r1, r1, r7
	strbls	r5, [r1], #1
	subs	r4, r4, #1
	bpl	.L_03000708
	b	.L_030006c0
.L_03000724:
	pop	{r5, r6, r7}
	mov	r0, #0
	bx	lr
