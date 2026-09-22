.syntax unified
	.thumb
	.set sub_08014174, 0x08014174
	.global Overlay_08014220
Overlay_08014220:
	push	{lr}
	ldr	r1, [pc, #24]
	movs	r2, #128
	movs	r0, #0
	lsls	r2, r2, #2
.L_0801422a:
	ldrb	r3, [r1, #0]
	adds	r1, #1
	cmp	r3, #255
	bne.n	.L_08014234
	adds	r0, #1
.L_08014234:
	subs	r2, #1
	cmp	r2, #0
	bne.n	.L_0801422a
	pop	{pc}
	.2byte 0x3410
	.2byte 0x0200
.L_08014240:
	push	{r5, lr}
	movs	r4, #0
	cmp	r0, #95
	bhi.n	.L_08014266
	ldr	r2, [pc, #36]
	movs	r1, #128
	movs	r5, #255
	lsls	r1, r1, #2
.L_08014250:
	ldrb	r3, [r2, #0]
	cmp	r3, r0
	bne.n	.L_0801425a
	strb	r5, [r2, #0]
	adds	r4, #1
.L_0801425a:
	subs	r1, #1
	adds	r2, #1
	cmp	r1, #0
	bne.n	.L_08014250
	cmp	r4, #0
	beq.n	.L_0801426c
.L_08014266:
	movs	r0, #1
	negs	r0, r0
	b.n	.L_0801426e
.L_0801426c:
	movs	r0, #0
.L_0801426e:
	pop	{r5, pc}
	.2byte 0x3410
	.2byte 0x0200
.L_08014274:
	push	{r5, r6, lr}
	ldr	r3, [pc, #48]
	lsls	r2, r0, #2
	adds	r5, r2, r3
	cmp	r0, #95
	bls.n	.L_08014286
	movs	r0, #1
	negs	r0, r0
	b.n	.L_080142a4
.L_08014286:
	movs	r6, #255
	ldrh	r3, [r5, #2]
	lsls	r6, r6, #8
	adds	r6, #255
	cmp	r3, r6
	beq.n	.L_080142a2
	bl	.L_08014240
	ldrh	r3, [r5, #2]
	adds	r2, r6, #0
	orrs	r2, r3
	movs	r3, #0
	strh	r2, [r5, #2]
	strh	r3, [r5, #0]
.L_080142a2:
	movs	r0, #0
.L_080142a4:
	pop	{r5, r6, pc}
	movs	r0, r0
	.2byte 0x36e0
	.2byte 0x0200
	push	{r5, lr}
	ldr	r3, [pc, #32]
	lsls	r2, r0, #2
	adds	r5, r2, r3
	cmp	r0, #95
	bls.n	.L_080142be
	movs	r0, #1
	negs	r0, r0
	b.n	.L_080142ce
.L_080142be:
	ldrh	r3, [r5, #0]
	cmp	r3, #16
	bls.n	.L_080142cc
	bl	.L_08014240
	movs	r3, #1
	strh	r3, [r5, #0]
.L_080142cc:
	movs	r0, #0
.L_080142ce:
	pop	{r5, pc}
	.2byte 0x36e0
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #128]
	adds	r5, r0, #0
	mov	r8, r2
	lsls	r2, r5, #2
	adds	r6, r1, #0
	adds	r7, r2, r3
	movs	r0, #0
	cmp	r5, #95
	bhi.n	.L_08014354
	movs	r2, #128
	lsls	r2, r2, #6
	cmp	r6, r2
	bhi.n	.L_08014354
	ldrh	r3, [r7, #0]
	cmp	r3, #16
	bls.n	.L_0801430a
	cmp	r3, r6
	beq.n	.L_08014306
	adds	r0, r5, #0
	bl	.L_08014274
	b.n	.L_0801430a
.L_08014306:
	ldrh	r5, [r7, #2]
	b.n	.L_08014314
.L_0801430a:
	adds	r0, r5, #0
	adds	r1, r6, #0
	bl	sub_08014174
	adds	r5, r0, #0
.L_08014314:
	movs	r3, #1
	negs	r3, r3
	cmp	r5, r3
	beq.n	.L_08014352
	ldr	r2, [pc, #64]
	strh	r6, [r7, #0]
	adds	r1, r5, r2
	mov	r2, r8
	strh	r5, [r7, #2]
	cmp	r2, #0
	beq.n	.L_0801434e
	cmp	r8, r3
	bne.n	.L_0801433a
	adds	r0, r1, #0
	ldr	r3, [pc, #48]
	adds	r1, r6, #0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0xe009
.L_0801433a:
	movs	r4, #132
	movs	r3, #128
	lsrs	r2, r6, #2
	lsls	r4, r4, #24
	lsls	r3, r3, #19
	adds	r3, #212
	mov	r0, r8
	orrs	r2, r4
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
.L_0801434e:
	lsrs	r0, r5, #5
	b.n	.L_08014354
.L_08014352:
	movs	r0, #0
.L_08014354:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x020036e0
	.4byte 0x06010000
	.2byte 0x0258
	.2byte 0x0300
	push	{lr}
	movs	r0, #128
	ldr	r3, [pc, #32]
	lsls	r0, r0, #1
	movs	r1, #0
	adds	r0, #255
	movs	r2, #255
.L_08014376:
	adds	r1, #1
	strb	r2, [r3, #0]
	adds	r3, #1
	cmp	r1, r0
	bls.n	.L_08014376
	ldr	r2, [pc, #16]
	ldr	r4, [pc, #8]
	movs	r1, #0
	movs	r0, #0
	b.n	.L_08014398
	movs	r0, r0
	.4byte 0x0000ffff
	.4byte 0x02003410
	.2byte 0x36e0
	.2byte 0x0200
.L_08014398:
	ldrh	r3, [r2, #2]
	adds	r1, #1
	orrs	r3, r4
	strh	r3, [r2, #2]
	strh	r0, [r2, #0]
	adds	r2, #4
	cmp	r1, #95
	bls.n	.L_08014398
	pop	{pc}
