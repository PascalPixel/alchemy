@ Wave, noise and square channel loops of TLA's resident mixer, entered
@ from the mixer. The wave loop saves r2, r3, r9 and ip in the middle of
@ the routine (push {r2, r3, r9, ip}), and every loop leaves with
@ b 0x03000b94 into the channel code of IWRAM_RUNTIME_MIX_CHANNEL.S,
@ sharing that routine's frame. Jumping between routine bodies is beyond
@ both approved compilers.
@ credit: handwritten — TLA IWRAM runtime synthesised channel loops
.syntax unified
	.set	.L_03000b94, 0x03000b94
	.arm
	.set AlchemyLoadAddress, 0x080014a4
	.set AlchemyRunAddress, 0x03000eec
.L_03000eec:
	cmp	ip, #0
	bne	.L_03000fb8
	ldrb	r6, [r3, #2]
	add	r2, r2, r6, lsl #24
	ldrb	r6, [r3, #4]
	adds	r6, r2, r6, lsl #24
	mvnmi	r6, r6
	lsr	sl, r6, #8
	ldrb	r1, [r3, #3]
	ldrb	r0, [r3, #1]
	lsl	r0, r0, #24
	mla	r6, sl, r1, r0
	push	{r2, r3, r9, ip}
.L_03000f20:
	ldm	r5, {r0, r1, r2, r3, r9, sl, ip, lr}
	cmp	r7, r6
	addcc	r0, r0, fp, lsl #6
	subcs	r0, r0, fp, lsl #6
	adds	r7, r7, r4, lsl #3
	cmp	r7, r6
	addcc	r1, r1, fp, lsl #6
	subcs	r1, r1, fp, lsl #6
	adds	r7, r7, r4, lsl #3
	cmp	r7, r6
	addcc	r2, r2, fp, lsl #6
	subcs	r2, r2, fp, lsl #6
	adds	r7, r7, r4, lsl #3
	cmp	r7, r6
	addcc	r3, r3, fp, lsl #6
	subcs	r3, r3, fp, lsl #6
	adds	r7, r7, r4, lsl #3
	cmp	r7, r6
	addcc	r9, r9, fp, lsl #6
	subcs	r9, r9, fp, lsl #6
	adds	r7, r7, r4, lsl #3
	cmp	r7, r6
	addcc	sl, sl, fp, lsl #6
	subcs	sl, sl, fp, lsl #6
	adds	r7, r7, r4, lsl #3
	cmp	r7, r6
	addcc	ip, ip, fp, lsl #6
	subcs	ip, ip, fp, lsl #6
	adds	r7, r7, r4, lsl #3
	cmp	r7, r6
	addcc	lr, lr, fp, lsl #6
	subcs	lr, lr, fp, lsl #6
	adds	r7, r7, r4, lsl #3
	stmia	r5!, {r0, r1, r2, r3, r9, sl, ip, lr}
	subs	r8, r8, #8
	bgt	.L_03000f20
	pop	{r2, r3, r9, ip}
	b	.L_03000b94
.L_03000fb8:
	subs	ip, ip, #1
	bne	.L_03001044
	mov	r6, #0x300
	lsr	fp, fp, #1
	bic	fp, fp, #0xff00
	mov	ip, #112
.L_03000fd0:
	ldm	r5, {r0, r1, sl, lr}
	adds	r7, r7, r4, lsl #3
	rsb	r9, ip, r7, lsr #24
	lsl	r6, r7, #1
	sub	r9, r9, r6, lsr #27
	adds	r2, r9, r2, asr #1
	mlane	r0, fp, r2, r0
	adds	r7, r7, r4, lsl #3
	rsb	r9, ip, r7, lsr #24
	lsl	r6, r7, #1
	sub	r9, r9, r6, lsr #27
	adds	r2, r9, r2, asr #1
	mlane	r1, fp, r2, r1
	adds	r7, r7, r4, lsl #3
	rsb	r9, ip, r7, lsr #24
	lsl	r6, r7, #1
	sub	r9, r9, r6, lsr #27
	adds	r2, r9, r2, asr #1
	mlane	sl, fp, r2, sl
	adds	r7, r7, r4, lsl #3
	rsb	r9, ip, r7, lsr #24
	lsl	r6, r7, #1
	sub	r9, r9, r6, lsr #27
	adds	r2, r9, r2, asr #1
	mlane	lr, fp, r2, lr
	stmia	r5!, {r0, r1, sl, lr}
	subs	r8, r8, #4
	bgt	.L_03000fd0
	b	.L_03000b94
.L_03001044:
	mov	r6, #128
	mov	ip, #0x180
.L_0300104c:
	ldm	r5, {r0, r1, sl, lr}
	adds	r7, r7, r4, lsl #3
	rsbpl	r9, r6, r7, asr #23
	submi	r9, ip, r7, lsr #23
	mla	r0, fp, r9, r0
	adds	r7, r7, r4, lsl #3
	rsbpl	r9, r6, r7, asr #23
	submi	r9, ip, r7, lsr #23
	mla	r1, fp, r9, r1
	adds	r7, r7, r4, lsl #3
	rsbpl	r9, r6, r7, asr #23
	submi	r9, ip, r7, lsr #23
	mla	sl, fp, r9, sl
	adds	r7, r7, r4, lsl #3
	rsbpl	r9, r6, r7, asr #23
	submi	r9, ip, r7, lsr #23
	mla	lr, fp, r9, lr
	stmia	r5!, {r0, r1, sl, lr}
	subs	r8, r8, #4
	bgt	.L_0300104c
	b	.L_03000b94
