.syntax unified
	.thumb
	.balign 4
	.global VramBlock_LoadCached
	.thumb_func
VramBlock_LoadCached:
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
	bl	Func_08014274
	b.n	.L_0801430a
.L_08014306:
	ldrh	r5, [r7, #2]
	b.n	.L_08014314
.L_0801430a:
	adds	r0, r5, #0
	adds	r1, r6, #0
	bl	0x08014174
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
	.global Func_08014368
	.thumb_func
Func_08014368:
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
	.2byte 0x0000