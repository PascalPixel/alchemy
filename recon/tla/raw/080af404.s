.syntax unified
	.thumb
	.set sub_080af3a0, 0x080af3a0
	.set sub_080af4b8, 0x080af4b8
	.set sub_080afe78, 0x080afe78
	.global Func_080af404
	.thumb_func
Func_080af404:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	sub	sp, #32
	mov	r5, sp
	mov	r8, r0
	adds	r0, r5, #0
	movs	r7, #0
	bl	sub_080afe78
	cmp	r7, r0
	bge.n	.L_080af432
	adds	r6, r5, #0
	adds	r5, r0, #0
.L_080af420:
	ldrh	r0, [r6, #0]
	mov	r1, r8
	bl	sub_080af3a0
	subs	r5, #1
	adds	r6, #2
	adds	r7, r7, r0
	cmp	r5, #0
	bne.n	.L_080af420
.L_080af432:
	adds	r0, r7, #0
	add	sp, #32
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	push	{lr}
	movs	r3, #252
	lsls	r3, r3, #6
	adds	r3, #255
	adds	r2, r0, #0
	ands	r2, r3
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r3, #222
	cmp	r2, r3
	bls.n	.L_080af454
	movs	r2, #0
.L_080af454:
	lsls	r0, r2, #1
	ldr	r3, [pc, #8]
	adds	r0, r0, r2
	lsls	r0, r0, #2
	adds	r0, r0, r3
	pop	{pc}
	.2byte 0x7c14
	.2byte 0x080b
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	sub	sp, #20
	mov	r8, sp
	mov	sl, r0
	mov	r0, r8
	bl	sub_080afe78
	movs	r5, #0
	adds	r7, r0, #0
	mov	r9, r8
	movs	r6, #0
	b.n	.L_080af488
.L_080af484:
	adds	r6, #2
	adds	r5, #1
.L_080af488:
	cmp	r5, r7
	bge.n	.L_080af49a
	mov	r1, r9
	ldrsh	r0, [r6, r1]
	mov	r1, sl
	bl	sub_080af4b8
	cmp	r0, #0
	beq.n	.L_080af484
.L_080af49a:
	cmp	r5, r7
	bne.n	.L_080af4a4
	movs	r0, #1
	negs	r0, r0
	b.n	.L_080af4aa
.L_080af4a4:
	lsls	r3, r5, #1
	mov	r1, r8
	ldrsh	r0, [r1, r3]
.L_080af4aa:
	add	sp, #20
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.align 2, 0
