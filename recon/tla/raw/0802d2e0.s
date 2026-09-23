.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.global Func_0802d2e0
	.thumb_func
Func_0802d2e0:
	push	{r5, r6, lr}
	movs	r3, #0
	ldrsb	r3, [r0, r3]
	adds	r0, #1
	lsls	r6, r3, #19
	movs	r3, #0
	ldrsb	r3, [r0, r3]
	adds	r1, r1, r2
	lsls	r5, r3, #19
	movs	r3, #1
	ldrsb	r3, [r0, r3]
	adds	r0, r5, #0
	lsls	r3, r3, #19
	cmp	r1, #15
	beq.n	.L_0802d322
	cmp	r1, #14
	bhi.n	.L_0802d312
	subs	r3, r5, r6
	adds	r0, r1, #0
	muls	r0, r3
	movs	r1, #15
	bl	sub_08002054
	adds	r0, r6, r0
	b.n	.L_0802d322
.L_0802d312:
	subs	r1, #15
	subs	r3, r3, r5
	adds	r0, r1, #0
	muls	r0, r3
	movs	r1, #15
	bl	sub_08002054
	adds	r0, r5, r0
.L_0802d322:
	pop	{r5, r6, pc}
	push	{r5, lr}
	movs	r3, #0
	ldrsb	r3, [r0, r3]
	adds	r0, #1
	lsls	r5, r3, #19
	movs	r3, #0
	ldrsb	r3, [r0, r3]
	lsls	r4, r3, #19
	movs	r3, #1
	ldrsb	r3, [r0, r3]
	subs	r0, r2, r1
	adds	r1, r0, #0
	adds	r1, #15
	lsls	r3, r3, #19
	cmp	r1, #15
	bne.n	.L_0802d348
	adds	r0, r4, #0
	b.n	.L_0802d36c
.L_0802d348:
	cmp	r1, #14
	bhi.n	.L_0802d35e
	subs	r3, r4, r5
	adds	r0, r1, #0
	muls	r0, r3
	cmp	r0, #0
	bge.n	.L_0802d358
	adds	r0, #15
.L_0802d358:
	asrs	r0, r0, #4
	adds	r0, r5, r0
	b.n	.L_0802d36c
.L_0802d35e:
	subs	r3, r3, r4
	muls	r0, r3
	cmp	r0, #0
	bge.n	.L_0802d368
	adds	r0, #15
.L_0802d368:
	asrs	r0, r0, #4
	adds	r0, r4, r0
.L_0802d36c:
	pop	{r5, pc}
	.2byte 0x0000
