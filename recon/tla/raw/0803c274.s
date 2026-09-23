.syntax unified
	.thumb
	.set sub_080143ac, 0x080143ac
	.set sub_08038eb0, 0x08038eb0
	.set sub_08039500, 0x08039500
	.global Func_0803c274
	.thumb_func
Func_0803c274:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r4, r3, #0
	movs	r3, #192
	adds	r7, r0, #0
	lsls	r3, r3, #18
	ldr	r6, [r3, #60]
	ldrh	r3, [r7, #10]
	adds	r5, r6, #0
	subs	r3, #2
	sub	sp, #4
	adds	r0, r2, #0
	adds	r5, #8
	cmp	r4, r3
	bhi.n	.L_0803c36c
	ldrh	r3, [r7, #8]
	subs	r3, #2
	cmp	r0, r3
	bhi.n	.L_0803c36c
	ldr	r2, [sp, #24]
	cmp	r2, #1
	bne.n	.L_0803c344
	bl	sub_08038eb0
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_0803c36c
	ldr	r4, [pc, #116]
	ldr	r2, [pc, #120]
	subs	r3, r5, r6
	adds	r3, r3, r4
	adds	r1, r3, #0
	muls	r1, r2
	movs	r3, #2
	movs	r2, #152
	strb	r3, [r5, #5]
	lsls	r2, r2, #5
	adds	r2, #70
	adds	r6, r6, r2
	ldrh	r3, [r6, #0]
	adds	r4, r5, #0
	mov	r8, r1
	adds	r4, #16
	cmp	r3, #99
	bne.n	.L_0803c2da
	str	r4, [sp, #0]
	bl	sub_080143ac
	strh	r0, [r6, #0]
	ldr	r4, [sp, #0]
.L_0803c2da:
	ldrh	r3, [r7, #8]
	movs	r1, #255
	ldrh	r2, [r7, #12]
	lsls	r1, r1, #8
	adds	r1, #254
	adds	r3, r3, r1
	adds	r2, r2, r3
	ldr	r3, [pc, #52]
	lsls	r2, r2, #3
	adds	r2, #4
	ands	r2, r3
	ldr	r1, [pc, #56]
	ldrh	r3, [r4, #6]
	ands	r1, r3
	orrs	r1, r2
	ldrb	r2, [r7, #10]
	ldrb	r3, [r7, #14]
	adds	r2, #254
	adds	r3, r3, r2
	strh	r1, [r4, #6]
	lsls	r3, r3, #3
	lsls	r1, r1, #23
	subs	r3, #1
	lsrs	r1, r1, #23
	strb	r3, [r4, #4]
	strh	r1, [r5, #6]
	mov	r2, r8
	ldrb	r3, [r4, #4]
	strb	r2, [r5, #14]
	strh	r3, [r5, #8]
	movs	r3, #0
	str	r3, [r5, #0]
	ldrb	r3, [r5, #5]
	b.n	.L_0803c330
	movs	r0, r0
	.4byte 0x000001ff
	.4byte 0xfffff8d0
	.4byte 0xb6db6db7
	.2byte 0xfe00
	.2byte 0xffff
.L_0803c330:
	cmp	r3, #0
	bne.n	.L_0803c33a
	add	r3, sp, #24
	ldrb	r3, [r3, #0]
	strb	r3, [r5, #5]
.L_0803c33a:
	adds	r0, r7, #0
	adds	r1, r5, #0
	bl	sub_08039500
	b.n	.L_0803c36c
.L_0803c344:
	cmp	r1, #255
	bhi.n	.L_0803c36c
	movs	r3, #14
	ldrsh	r2, [r7, r3]
	adds	r4, #1
	adds	r2, r2, r4
	movs	r4, #12
	ldrsh	r3, [r7, r4]
	adds	r0, #1
	lsls	r2, r2, #5
	adds	r3, r3, r0
	adds	r0, r2, r3
	movs	r2, #160
	lsls	r2, r2, #2
	cmp	r0, r2
	bcs.n	.L_0803c36c
	ldr	r3, [pc, #12]
	lsls	r2, r0, #1
	orrs	r1, r3
	strh	r1, [r5, r2]
.L_0803c36c:
	add	sp, #4
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0xf000
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	adds	r4, r3, #0
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #60]
	adds	r5, r2, #0
	adds	r6, r3, #0
	ldrh	r3, [r0, #10]
	adds	r4, #1
	subs	r3, #1
	adds	r7, r1, #0
	ldr	r2, [sp, #16]
	adds	r6, #8
	adds	r5, #1
	cmp	r4, r3
	bhi.n	.L_0803c408
	ldrh	r3, [r0, #8]
	subs	r3, #1
	cmp	r5, r3
	bhi.n	.L_0803c408
	movs	r1, #240
	lsls	r1, r1, #8
	cmp	r2, #3
	beq.n	.L_0803c3c0
	cmp	r2, #3
	bhi.n	.L_0803c3b6
	movs	r1, #224
	lsls	r1, r1, #8
	cmp	r2, #2
	beq.n	.L_0803c3c0
	b.n	.L_0803c3be
.L_0803c3b6:
	movs	r1, #128
	lsls	r1, r1, #5
	cmp	r2, #4
	beq.n	.L_0803c3c0
.L_0803c3be:
	movs	r1, #0
.L_0803c3c0:
	cmp	r2, #1
	beq.n	.L_0803c408
	cmp	r2, #1
	bcc.n	.L_0803c3ec
	cmp	r2, #4
	bhi.n	.L_0803c3ec
	movs	r3, #14
	ldrsh	r2, [r0, r3]
	adds	r2, r2, r4
	movs	r4, #12
	ldrsh	r3, [r0, r4]
	lsls	r2, r2, #5
	adds	r3, r3, r5
	adds	r0, r2, r3
	movs	r3, #160
	lsls	r3, r3, #2
	cmp	r0, r3
	bcs.n	.L_0803c408
	lsls	r3, r0, #1
	orrs	r1, r7
	strh	r1, [r6, r3]
	b.n	.L_0803c408
.L_0803c3ec:
	movs	r1, #14
	ldrsh	r2, [r0, r1]
	movs	r1, #160
	adds	r2, r2, r4
	movs	r4, #12
	ldrsh	r3, [r0, r4]
	lsls	r2, r2, #5
	adds	r3, r3, r5
	adds	r0, r2, r3
	lsls	r1, r1, #2
	cmp	r0, r1
	bcs.n	.L_0803c408
	lsls	r3, r0, #1
	strh	r7, [r6, r3]
.L_0803c408:
	pop	{r5, r6, r7, pc}
	.align 2, 0
