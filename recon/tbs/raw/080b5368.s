.syntax unified
	.thumb
	.set sub_08077008, 0x08077008
	.set sub_08077028, 0x08077028
	.set sub_08077050, 0x08077050
	.set sub_08077058, 0x08077058
	.set sub_08077088, 0x08077088
	.set sub_08077150, 0x08077150
	.set sub_08077168, 0x08077168
	.set sub_080771a8, 0x080771a8
	.set sub_080771b0, 0x080771b0
	.set sub_080771f0, 0x080771f0
	.global Func_080b5368
	.thumb_func
Func_080b5368:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #44
	str	r0, [sp, #24]
	movs	r1, #0
	movs	r0, #0
	str	r1, [sp, #20]
	bl	sub_08077168
	movs	r0, #1
	bl	sub_08077168
	movs	r0, #2
	bl	sub_08077168
	movs	r0, #3
	bl	sub_08077168
	movs	r0, #5
	bl	sub_08077168
	mov	r3, sp
	adds	r3, #28
	str	r3, [sp, #4]
	movs	r2, #0
	ldr	r5, [sp, #4]
	str	r2, [sp, #12]
	add	r3, sp, #40
	mov	ip, r5
.L_080b53ac:
	str	r2, [r3, #0]
	subs	r3, #4
	cmp	r3, ip
	bge.n	.L_080b53ac
	movs	r1, #0
	str	r1, [sp, #16]
	b.n	.L_080b54e8
.L_080b53ba:
	ldr	r2, [sp, #12]
	adds	r2, #1
	str	r2, [sp, #12]
	b.n	.L_080b54e2
.L_080b53c2:
	ldr	r3, [sp, #12]
	ldr	r1, [sp, #24]
	cmp	r3, r1
	beq.n	.L_080b53cc
	b.n	.L_080b54e2
.L_080b53cc:
	mov	r0, sl
	bl	sub_08077150
	ldr	r2, [pc, #108]
	adds	r3, r5, r2
	ldrb	r1, [r3, #1]
	mov	r0, sl
	bl	sub_080771f0
	mov	r0, sl
	bl	sub_08077008
	movs	r5, #140
	adds	r1, r0, #0
	adds	r2, r1, #0
	lsls	r5, r5, #1
	movs	r4, #0
	movs	r0, #0
	adds	r2, #248
	adds	r3, r1, r5
	movs	r7, #3
.L_080b53f6:
	subs	r7, #1
	strb	r4, [r3, #0]
	strb	r4, [r3, #4]
	str	r0, [r2, #0]
	str	r0, [r2, #16]
	adds	r3, #1
	adds	r2, #4
	cmp	r7, #0
	bge.n	.L_080b53f6
	adds	r0, r1, #0
	ldr	r3, [pc, #48]
	movs	r7, #31
	adds	r0, #212
.L_080b5410:
	subs	r7, #1
	strh	r3, [r0, #0]
	subs	r0, #4
	cmp	r7, #0
	bge.n	.L_080b5410
	ldr	r1, [sp, #8]
	ldr	r2, [sp, #16]
	ldr	r5, [pc, #32]
	adds	r3, r1, r2
	lsls	r3, r3, #2
	adds	r3, r5, r3
	adds	r5, r3, #0
	adds	r5, #14
	movs	r7, #1
.L_080b542c:
	ldrh	r3, [r5, #0]
	cmp	r3, #0
	beq.n	.L_080b5444
	adds	r1, r3, #0
	mov	r0, sl
	bl	sub_08077088
	b.n	.L_080b5444
	.4byte 0x00000000
	.2byte 0x3f34
	.2byte 0x080c
.L_080b5444:
	subs	r7, #1
	adds	r5, #2
	cmp	r7, #0
	bge.n	.L_080b542c
	ldr	r1, [sp, #8]
	ldr	r2, [sp, #16]
	adds	r3, r1, r2
	lsls	r4, r3, #2
	movs	r3, #0
	movs	r7, #0
	mov	r9, r3
.L_080b545a:
	ldr	r2, [pc, #212]
	adds	r2, #2
	movs	r5, #0
	ldrsb	r3, [r2, r4]
	mov	r8, r5
	cmp	r8, r3
	bge.n	.L_080b5498
	ldr	r6, [sp, #4]
	mov	r5, r9
	mov	fp, r2
.L_080b546e:
	ldr	r2, [r6, r5]
	adds	r1, r7, #0
	mov	r0, sl
	str	r4, [sp, #0]
	bl	sub_080771a8
	ldr	r2, [r6, r5]
	adds	r1, r7, #0
	mov	r0, sl
	bl	sub_080771b0
	ldr	r3, [r6, r5]
	ldr	r4, [sp, #0]
	adds	r3, #1
	mov	r2, fp
	str	r3, [r6, r5]
	movs	r1, #1
	ldrsb	r3, [r2, r4]
	add	r8, r1
	cmp	r8, r3
	blt.n	.L_080b546e
.L_080b5498:
	movs	r3, #4
	adds	r7, #1
	adds	r4, #1
	add	r9, r3
	cmp	r7, #3
	ble.n	.L_080b545a
	movs	r7, #15
.L_080b54a6:
	mov	r0, sl
	movs	r1, #0
	subs	r7, #1
	bl	sub_08077058
	cmp	r7, #0
	bge.n	.L_080b54a6
	ldr	r5, [sp, #8]
	ldr	r1, [sp, #16]
	ldr	r2, [pc, #116]
	adds	r3, r5, r1
	lsls	r3, r3, #2
	adds	r3, r2, r3
	adds	r5, r3, #6
	movs	r7, #3
.L_080b54c4:
	ldrh	r3, [r5, #0]
	cmp	r3, #0
	beq.n	.L_080b54da
	adds	r1, r3, #0
	mov	r0, sl
	bl	sub_08077028
	adds	r1, r0, #0
	mov	r0, sl
	bl	sub_08077050
.L_080b54da:
	subs	r7, #1
	adds	r5, #2
	cmp	r7, #0
	bge.n	.L_080b54c4
.L_080b54e2:
	ldr	r3, [sp, #16]
	adds	r3, #1
	str	r3, [sp, #16]
.L_080b54e8:
	ldr	r5, [sp, #16]
	ldr	r1, [sp, #16]
	lsls	r5, r5, #2
	str	r5, [sp, #8]
	adds	r3, r5, r1
	ldr	r2, [pc, #60]
	lsls	r5, r3, #2
	ldrsb	r2, [r2, r5]
	movs	r3, #166
	lsls	r3, r3, #1
	mov	sl, r2
	cmp	r1, r3
	bls.n	.L_080b5508
	movs	r5, #1
	str	r5, [sp, #20]
	b.n	.L_080b551c
.L_080b5508:
	movs	r1, #1
	negs	r1, r1
	cmp	sl, r1
	beq.n	.L_080b5512
	b.n	.L_080b53c2
.L_080b5512:
	ldr	r2, [sp, #12]
	ldr	r3, [sp, #24]
	cmp	r2, r3
	beq.n	.L_080b551c
	b.n	.L_080b53ba
.L_080b551c:
	ldr	r0, [sp, #20]
	add	sp, #44
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x080c3f34
