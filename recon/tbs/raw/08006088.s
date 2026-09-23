.syntax unified
	.thumb
	.set sub_080060e8, 0x080060e8
	.set sub_0800615c, 0x0800615c
	.global SerialRuntime_ExchangePayloads
	.global Func_08006088
	.thumb_func
SerialRuntime_ExchangePayloads:
Func_08006088:
	push	{r5, r6, r7, lr}
	ldr	r3, [pc, #84]
	ldr	r5, [pc, #84]
	ldr	r7, [r3, #0]
	ldrb	r3, [r5, #1]
	adds	r6, r0, #0
	adds	r0, r1, #0
	cmp	r3, #1
	bne.n	.L_080060aa
	bl	sub_0800615c
	adds	r0, r6, #0
	bl	sub_080060e8
	ldrb	r3, [r5, #11]
	adds	r3, #1
	strb	r3, [r5, #11]
.L_080060aa:
	ldrb	r3, [r5, #2]
	ldrb	r2, [r5, #3]
	lsls	r3, r3, #8
	orrs	r2, r3
	ldrb	r3, [r5, #0]
	cmp	r3, #8
	bne.n	.L_080060bc
	movs	r3, #128
	orrs	r2, r3
.L_080060bc:
	ldrb	r3, [r5, #9]
	adds	r0, r2, #0
	cmp	r3, #0
	beq.n	.L_080060ca
	movs	r3, #128
	lsls	r3, r3, #5
	orrs	r0, r3
.L_080060ca:
	lsls	r3, r7, #26
	lsrs	r3, r3, #30
	cmp	r3, #1
	bls.n	.L_080060d8
	movs	r3, #128
	lsls	r3, r3, #6
	orrs	r0, r3
.L_080060d8:
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x04000128
	.4byte 0x02002240
