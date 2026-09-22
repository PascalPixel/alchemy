.syntax unified
	.thumb
	.set sub_080142d4, 0x080142d4
	.set sub_080143ac, 0x080143ac
	.global Overlay_0803e7c8
Overlay_0803e7c8:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #52
	muls	r3, r1
	adds	r6, r0, #0
	adds	r2, r6, r3
	movs	r0, #0
	adds	r2, #40
	mov	fp, r0
	adds	r3, #8
	mov	sl, r2
	adds	r4, r6, r3
	mov	r2, fp
	strh	r2, [r4, #2]
	cmp	r1, #0
	beq.n	.L_0803e830
	movs	r0, #229
	lsls	r0, r0, #2
	adds	r3, r6, r0
	ldrh	r2, [r3, #0]
	movs	r3, #231
	lsls	r3, r3, #2
	adds	r0, r6, r3
	ldrh	r3, [r0, #0]
	ldr	r5, [pc, #224]
	cmp	r3, #0
	beq.n	.L_0803e80a
	subs	r2, r2, r3
.L_0803e80a:
	cmp	r2, #5
	bls.n	.L_0803e814
	movs	r3, #1
	strh	r3, [r4, #2]
	movs	r2, #5
.L_0803e814:
	movs	r4, #192
	lsls	r4, r4, #2
	adds	r4, #150
	adds	r3, r6, r4
	ldrh	r3, [r3, #0]
	subs	r2, #1
	lsls	r2, r2, #4
	adds	r3, r3, r2
	adds	r2, r6, #0
	adds	r3, #17
	adds	r2, #68
	mov	fp, r5
	strh	r3, [r2, #0]
	b.n	.L_0803e858
.L_0803e830:
	movs	r2, #192
	lsls	r2, r2, #2
	adds	r2, #150
	adds	r3, r6, r2
	ldrh	r3, [r3, #0]
	movs	r4, #255
	ldr	r0, [pc, #168]
	lsls	r4, r4, #8
	adds	r4, #247
	adds	r3, r3, r4
	strh	r3, [r6, #16]
	mov	fp, r0
	movs	r0, #231
	lsls	r0, r0, #2
	adds	r3, r6, r0
	ldrh	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_0803e858
	movs	r3, #1
	strh	r3, [r6, #10]
.L_0803e858:
	movs	r3, #52
	adds	r7, r1, #0
	muls	r7, r3
	adds	r3, r7, #0
	adds	r3, #16
	adds	r3, r3, r6
	movs	r4, #2
	ldrsh	r2, [r3, r4]
	mov	r8, r3
	mov	r9, r2
	cmp	r2, #0
	bne.n	.L_0803e8d8
	bl	sub_080143ac
	adds	r5, r7, #0
	adds	r5, #12
	strh	r0, [r6, r5]
	movs	r1, #128
	ldrh	r0, [r6, r5]
	mov	r2, fp
	bl	sub_080142d4
	adds	r5, r6, r5
	strh	r0, [r5, #2]
	movs	r0, #230
	lsls	r0, r0, #2
	adds	r3, r6, r0
	ldrh	r3, [r3, #0]
	mov	r2, r8
	strh	r3, [r2, #2]
	adds	r3, r7, #0
	adds	r3, #8
	mov	r4, r9
	strh	r4, [r6, r3]
	mov	r0, sl
	ldrb	r3, [r0, #5]
	movs	r0, #13
	negs	r0, r0
	adds	r2, r0, #0
	ands	r2, r3
	movs	r3, #17
	negs	r3, r3
	ands	r2, r3
	movs	r3, #32
	orrs	r2, r3
	movs	r3, #4
	negs	r3, r3
	ands	r2, r3
	mov	r3, sl
	ldrb	r1, [r3, #7]
	movs	r3, #63
	negs	r3, r3
	ands	r3, r1
	movs	r1, #63
	mov	r4, sl
	ands	r3, r1
	strb	r3, [r4, #7]
	ands	r2, r1
	movs	r3, #128
	orrs	r2, r3
	ldrb	r3, [r4, #9]
	strb	r2, [r4, #5]
	ands	r0, r3
	strb	r0, [r4, #9]
.L_0803e8d8:
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x0805c5c4
	.2byte 0xc1c4
	.2byte 0x0805
	push	{lr}
	movs	r2, #210
	movs	r4, #192
	lsls	r2, r2, #2
	lsls	r4, r4, #2
	adds	r3, r0, r2
	adds	r4, #158
	ldr	r2, [r3, #0]
	adds	r3, r0, r4
	ldrh	r3, [r3, #0]
	movs	r1, #0
	cmp	r3, #0
	beq.n	.L_0803e912
	adds	r3, r0, r4
	ldrh	r0, [r3, #0]
.L_0803e90a:
	adds	r1, #1
	ldr	r2, [r2, #4]
	cmp	r1, r0
	bne.n	.L_0803e90a
.L_0803e912:
	adds	r0, r2, #0
	pop	{pc}
