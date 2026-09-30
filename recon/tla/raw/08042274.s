.syntax unified
	.thumb
	.global UiText_DrawPrefixedNumberAtOffset
	.global Func_08042274
	.thumb_func
UiText_DrawPrefixedNumberAtOffset:
Func_08042274:
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
	bl	0x0803ae14
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
	bl	Func_080416cc
.L_08042302:
	add	sp, #32
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.4byte 0x0000f01e
	.2byte 0x2000
	.2byte 0x0600
	.global RenderOutput_Create
	.thumb_func
RenderOutput_Create:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	adds	r7, r0, #0
	mov	sl, r1
	mov	r8, r2
	adds	r6, r3, #0
	bl	Func_08038eb0
	adds	r5, r0, #0
	cmp	r5, #0
	bne.n	.L_08042338
	adds	r0, r7, #0
	bl	0x08014274
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
	bl	RenderOutput_AppendToList
	adds	r0, r5, #0
.L_08042390:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x36e0
	.2byte 0x0200
