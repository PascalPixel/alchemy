.syntax unified
	.thumb
	.set sub_08014274, 0x08014274
	.set sub_080143ac, 0x080143ac
	.set sub_08038eb0, 0x08038eb0
	.set sub_08039500, 0x08039500
	.set sub_0803ae14, 0x0803ae14
	.set sub_0803d2f0, 0x0803d2f0
	.set sub_0803d4e4, 0x0803d4e4
	.set sub_0803d8ac, 0x0803d8ac
	.set sub_0803dab0, 0x0803dab0
	.set sub_0803f1d4, 0x0803f1d4
	.set sub_080416cc, 0x080416cc
	.global Overlay_08042274
Overlay_08042274:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	mov	r8, r3
	movs	r3, #192
	sub	sp, #32
	lsls	r3, r3, #18
	ldr	r5, [sp, #56]
	ldr	r3, [r3, #60]
	adds	r4, r0, #0
	adds	r6, r1, #0
	adds	r7, r2, #0
	add	r0, sp, #16
	adds	r1, r4, #0
	movs	r2, #4
	mov	sl, r3
	bl	sub_0803ae14
	cmp	r5, #0
	bne.n	.L_080422a8
	movs	r3, #240
	lsls	r3, r3, #8
	mov	r4, sp
	adds	r3, #29
	b.n	.L_080422b0
.L_080422a8:
	movs	r3, #240
	lsls	r3, r3, #8
	mov	r4, sp
	adds	r3, #31
.L_080422b0:
	strh	r3, [r4, #0]
	ldr	r3, [pc, #88]
	strh	r3, [r4, #2]
	adds	r2, r4, #4
	movs	r1, #4
.L_080422ba:
	ldrb	r3, [r0, #0]
	subs	r1, #1
	strh	r3, [r2, #0]
	adds	r0, #1
	adds	r2, #2
	cmp	r1, #0
	bge.n	.L_080422ba
	movs	r3, #0
	strh	r3, [r4, #12]
	movs	r1, #14
	ldrsh	r3, [r6, r1]
	mov	r1, r8
	lsrs	r2, r1, #3
	adds	r3, r3, r2
	movs	r1, #12
	ldrsh	r2, [r6, r1]
	adds	r3, #1
	lsrs	r1, r7, #3
	adds	r2, r2, r1
	lsls	r3, r3, #5
	adds	r3, r3, r2
	movs	r2, #160
	adds	r1, r3, #1
	lsls	r2, r2, #2
	cmp	r1, r2
	bcs.n	.L_08042302
	ldr	r3, [pc, #32]
	lsls	r1, r1, #1
	adds	r2, r1, r3
	add	r1, sl
	movs	r3, #7
	adds	r1, #8
	ands	r3, r7
	adds	r0, r4, #0
	bl	sub_080416cc
.L_08042302:
	add	sp, #32
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.4byte 0x0000f01e
	.2byte 0x2000
	.2byte 0x0600
.L_08042314:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	adds	r7, r0, #0
	mov	sl, r1
	mov	r8, r2
	adds	r6, r3, #0
	bl	sub_08038eb0
	adds	r5, r0, #0
	cmp	r5, #0
	bne.n	.L_08042338
	adds	r0, r7, #0
	bl	sub_08014274
	movs	r0, #0
	b.n	.L_08042390
.L_08042338:
	mov	r2, r8
	movs	r0, #14
	ldrsh	r3, [r2, r0]
	movs	r0, #12
	ldrsh	r1, [r2, r0]
	ldr	r2, [sp, #24]
	lsls	r3, r3, #3
	adds	r2, r2, r3
	lsls	r1, r1, #3
	movs	r3, #128
	adds	r1, r6, r1
	lsls	r3, r3, #1
	adds	r3, #255
	adds	r1, #8
	ands	r1, r3
	adds	r2, #8
	movs	r3, #255
	ands	r2, r3
	lsls	r3, r1, #16
	orrs	r3, r2
	mov	r0, sl
	orrs	r3, r0
	ldr	r0, [pc, #48]
	str	r3, [r5, #20]
	lsls	r3, r7, #2
	adds	r3, r3, r0
	ldrh	r3, [r3, #2]
	movs	r0, #0
	lsrs	r3, r3, #5
	str	r3, [r5, #24]
	movs	r3, #254
	strb	r3, [r5, #15]
	movs	r3, #1
	strh	r1, [r5, #6]
	str	r0, [r5, #0]
	strh	r2, [r5, #8]
	strb	r7, [r5, #14]
	strb	r3, [r5, #4]
	strb	r3, [r5, #5]
	mov	r0, r8
	adds	r1, r5, #0
	bl	sub_08039500
	adds	r0, r5, #0
.L_08042390:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x36e0
	.2byte 0x0200
	push	{r5, r6, lr}
	sub	sp, #4
	adds	r5, r2, #0
	adds	r6, r3, #0
	bl	sub_0803f1d4
	cmp	r0, #0
	bge.n	.L_080423b0
	movs	r0, #0
	b.n	.L_080423c0
.L_080423b0:
	ldr	r3, [sp, #16]
	movs	r1, #128
	str	r3, [sp, #0]
	lsls	r1, r1, #23
	adds	r2, r5, #0
	adds	r3, r6, #0
	bl	.L_08042314
.L_080423c0:
	add	sp, #4
	pop	{r5, r6, pc}
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	sub	sp, #4
	adds	r6, r0, #0
	adds	r7, r1, #0
	mov	r8, r2
	mov	sl, r3
	bl	sub_080143ac
	adds	r5, r0, #0
	movs	r0, #0
	cmp	r5, #96
	beq.n	.L_080423fe
	adds	r1, r7, #0
	adds	r2, r5, #0
	adds	r0, r6, #0
	bl	sub_0803d8ac
	ldr	r3, [sp, #28]
	movs	r1, #128
	str	r3, [sp, #0]
	lsls	r1, r1, #23
	adds	r0, r5, #0
	mov	r2, r8
	mov	r3, sl
	bl	.L_08042314
.L_080423fe:
	add	sp, #4
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	sub	sp, #12
	adds	r5, r0, #0
	adds	r7, r1, #0
	mov	r8, r2
	adds	r6, r3, #0
	bl	sub_080143ac
	str	r0, [sp, #8]
	cmp	r0, #96
	bne.n	.L_08042426
	movs	r0, #0
	b.n	.L_08042448
.L_08042426:
	add	r2, sp, #8
	add	r3, sp, #4
	movs	r1, #1
	adds	r0, r5, #0
	str	r1, [sp, #0]
	bl	sub_0803d4e4
	movs	r1, #128
	mov	r3, r8
	ldr	r0, [sp, #8]
	lsls	r1, r1, #23
	adds	r2, r7, #0
	str	r6, [sp, #0]
	bl	.L_08042314
	movs	r3, #251
	strb	r3, [r0, #15]
.L_08042448:
	add	sp, #12
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
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
	bl	sub_0803d2f0
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
	bl	sub_0803dab0
	ldr	r3, [sp, #48]
	movs	r1, #128
	str	r3, [sp, #0]
	ldr	r0, [sp, #12]
	lsls	r1, r1, #24
	mov	r2, r9
	ldr	r3, [sp, #44]
	bl	.L_08042314
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
