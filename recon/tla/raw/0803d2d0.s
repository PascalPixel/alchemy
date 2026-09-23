.syntax unified
	.thumb
	.set sub_08039260, 0x08039260
	.set sub_0803939c, 0x0803939c
	.set sub_08042450, 0x08042450
	.global Func_0803d2d0
	.thumb_func
Func_0803d2d0:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #60]
	movs	r0, #152
	ldr	r2, [pc, #16]
	lsls	r0, r0, #5
	adds	r0, #124
	adds	r1, r3, r0
	adds	r0, #2
	strh	r2, [r1, #0]
	adds	r1, r3, r0
	strh	r2, [r1, #0]
	bx	lr
	movs	r0, r0
	.2byte 0x03e7
	.2byte 0x0000
.L_0803d2f0:
	push	{r5, lr}
	adds	r3, r0, #0
	movs	r4, #1
	subs	r3, #47
	negs	r4, r4
	movs	r1, #0
	cmp	r3, #7
	bhi.n	.L_0803d346
	ldr	r2, [pc, #176]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x0803d328
	.4byte 0x0803d330
	.4byte 0x0803d32c
	.4byte 0x0803d334
	.4byte 0x0803d338
	.4byte 0x0803d33c
	.4byte 0x0803d340
	.2byte 0xd344
	.2byte 0x0803
	movs	r0, #4
	b.n	.L_0803d346
	movs	r0, #6
	b.n	.L_0803d346
	movs	r0, #5
	b.n	.L_0803d346
	movs	r0, #7
	b.n	.L_0803d346
	movs	r0, #0
	b.n	.L_0803d346
	movs	r0, #1
	b.n	.L_0803d346
	movs	r0, #2
	b.n	.L_0803d346
	movs	r0, #3
.L_0803d346:
	cmp	r0, #15
	bhi.n	.L_0803d37c
	ldr	r2, [pc, #108]
	movs	r5, #0
	ldrsh	r3, [r2, r5]
	movs	r5, #1
	negs	r5, r5
	cmp	r3, r5
	beq.n	.L_0803d3ae
	cmp	r3, r0
	bne.n	.L_0803d362
	movs	r1, #2
	ldrsh	r4, [r2, r1]
	b.n	.L_0803d3ae
.L_0803d362:
	adds	r1, #2
	lsls	r3, r1, #1
	ldrsh	r3, [r2, r3]
	movs	r5, #1
	negs	r5, r5
	cmp	r3, r5
	beq.n	.L_0803d3ae
	cmp	r3, r0
	bne.n	.L_0803d362
	adds	r1, #1
	lsls	r3, r1, #1
	ldrsh	r4, [r2, r3]
	b.n	.L_0803d3ae
.L_0803d37c:
	ldr	r2, [pc, #60]
	movs	r5, #0
	ldrsh	r3, [r2, r5]
	movs	r5, #1
	negs	r5, r5
	cmp	r3, r5
	beq.n	.L_0803d3ae
	cmp	r3, r0
	bne.n	.L_0803d394
	movs	r1, #2
	ldrsh	r4, [r2, r1]
	b.n	.L_0803d3ac
.L_0803d394:
	adds	r1, #2
	lsls	r3, r1, #1
	ldrsh	r3, [r2, r3]
	movs	r5, #1
	negs	r5, r5
	cmp	r3, r5
	beq.n	.L_0803d3ae
	cmp	r3, r0
	bne.n	.L_0803d394
	adds	r1, #1
	lsls	r3, r1, #1
	ldrsh	r4, [r2, r3]
.L_0803d3ac:
	adds	r4, #128
.L_0803d3ae:
	adds	r0, r4, #0
	pop	{r5, pc}
	movs	r0, r0
	.4byte 0x0803d308
	.4byte 0x0805eb58
	.2byte 0xeb7c
	.2byte 0x0805
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	adds	r7, r3, #0
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #60]
	sub	sp, #8
	mov	sl, r0
	mov	fp, r1
	adds	r6, r2, #0
	bl	.L_0803d2f0
	movs	r3, #1
	negs	r3, r3
	cmp	r0, r3
	bne.n	.L_0803d3ee
	movs	r0, #0
	b.n	.L_0803d442
.L_0803d3ee:
	movs	r3, #4
	negs	r3, r3
	mov	r9, r3
	ldrb	r3, [r5, #4]
	mov	r8, r9
	cmp	r3, #0
	beq.n	.L_0803d414
	movs	r3, #2
	str	r3, [sp, #0]
	adds	r0, r6, #0
	movs	r3, #5
	adds	r1, r7, #0
	movs	r2, #6
	bl	sub_08039260
	movs	r3, #0
	adds	r5, r0, #0
	mov	r8, r3
	b.n	.L_0803d426
.L_0803d414:
	movs	r3, #2
	str	r3, [sp, #0]
	adds	r0, r6, #0
	adds	r1, r7, #0
	movs	r2, #5
	movs	r3, #5
	bl	sub_08039260
	adds	r5, r0, #0
.L_0803d426:
	cmp	r5, #0
	beq.n	.L_0803d440
	mov	r3, r8
	movs	r2, #1
	str	r3, [sp, #0]
	mov	r3, r9
	str	r3, [sp, #4]
	negs	r2, r2
	mov	r0, sl
	mov	r1, fp
	adds	r3, r5, #0
	bl	sub_08042450
.L_0803d440:
	adds	r0, r5, #0
.L_0803d442:
	add	sp, #8
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	push	{r5, r6, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r6, [r3, #60]
	movs	r2, #161
	lsls	r2, r2, #3
	adds	r5, r6, r2
	bl	.L_0803d2f0
	movs	r3, #1
	negs	r3, r3
	cmp	r0, r3
	beq.n	.L_0803d4b8
	movs	r2, #152
	lsls	r2, r2, #5
	adds	r2, #126
	adds	r3, r6, r2
	ldrh	r3, [r3, #0]
	cmp	r3, r0
	bne.n	.L_0803d486
	movs	r3, #1
	b.n	.L_0803d496
.L_0803d47c:
	adds	r0, r5, #0
	movs	r1, #2
	bl	sub_0803939c
	b.n	.L_0803d4b8
.L_0803d486:
	movs	r2, #152
	lsls	r2, r2, #5
	adds	r2, #124
	adds	r3, r6, r2
	ldrh	r3, [r3, #0]
	cmp	r3, r0
	bne.n	.L_0803d4b8
	movs	r3, #0
.L_0803d496:
	movs	r2, #156
	lsls	r3, r3, #1
	lsls	r2, r2, #5
	adds	r3, r3, r2
	ldrh	r0, [r6, r3]
	movs	r1, #0
.L_0803d4a2:
	ldr	r2, [r5, #0]
	ldrb	r3, [r2, #4]
	cmp	r3, #2
	bne.n	.L_0803d4b0
	ldrb	r3, [r2, #14]
	cmp	r3, r0
	beq.n	.L_0803d47c
.L_0803d4b0:
	adds	r1, #1
	adds	r5, #36
	cmp	r1, #12
	bne.n	.L_0803d4a2
.L_0803d4b8:
	pop	{r5, r6, pc}
	.2byte 0x0000
