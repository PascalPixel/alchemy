.syntax unified
	.thumb
	.global Func_08042450
	.thumb_func
Func_08042450:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	mov	r9, r3
	movs	r3, #192
	lsls	r3, r3, #18
	mov	sl, r1
	sub	sp, #16
	adds	r5, r2, #0
	ldr	r6, [r3, #60]
	bl	Func_0803d2f0
	movs	r1, #1
	adds	r7, r0, #0
	negs	r1, r1
	movs	r0, #0
	cmp	r7, r1
	beq.n	.L_080424fa
	cmp	r5, #1
	bls.n	.L_080424a2
	movs	r2, #152
	lsls	r2, r2, #5
	adds	r2, #126
	adds	r3, r6, r2
	ldrh	r3, [r3, #0]
	movs	r2, #186
	lsls	r2, r2, #2
	adds	r2, #255
	movs	r5, #1
	cmp	r3, r2
	beq.n	.L_080424a2
	movs	r1, #152
	lsls	r1, r1, #5
	adds	r1, #124
	adds	r3, r6, r1
	ldrh	r3, [r3, #0]
	movs	r5, #0
	cmp	r3, r2
	bne.n	.L_080424fa
.L_080424a2:
	movs	r2, #14
	adds	r2, r2, r5
	mov	r8, r2
	mov	r1, r8
	str	r1, [sp, #0]
	movs	r1, #0
	add	r2, sp, #12
	add	r3, sp, #8
	str	r1, [sp, #4]
	adds	r0, r7, #0
	mov	r1, sl
	bl	0x0803dab0
	ldr	r3, [sp, #48]
	movs	r1, #128
	str	r3, [sp, #0]
	ldr	r0, [sp, #12]
	lsls	r1, r1, #24
	mov	r2, r9
	ldr	r3, [sp, #44]
	bl	RenderOutput_Create
	cmp	r0, #0
	beq.n	.L_080424e4
	mov	r2, r8
	lsls	r1, r2, #4
	ldrb	r2, [r0, #25]
	movs	r3, #15
	ands	r3, r2
	orrs	r3, r1
	strb	r3, [r0, #25]
	movs	r3, #2
	strb	r3, [r0, #4]
.L_080424e4:
	movs	r1, #152
	lsls	r1, r1, #5
	lsls	r3, r5, #1
	adds	r1, #124
	adds	r2, r3, r1
	strh	r7, [r6, r2]
	movs	r2, #156
	lsls	r2, r2, #5
	adds	r3, r3, r2
	ldr	r2, [sp, #12]
	strh	r2, [r6, r3]
.L_080424fa:
	add	sp, #16
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
