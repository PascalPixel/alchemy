.syntax unified
	.thumb
	.set sub_08002304, 0x08002304
	.set sub_0800231c, 0x0800231c
	.set sub_08002322, 0x08002322
	.set sub_08003d28, 0x08003d28
	.set sub_08003dec, 0x08003dec
	.set sub_08003fa4, 0x08003fa4
	.set sub_08004458, 0x08004458
	.global RenderOutput_UpdateScaleAnimation
	.global Func_0801908c
	.thumb_func
RenderOutput_UpdateScaleAnimation:
Func_0801908c:
.L_0801908c:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r6, r0, #0
	ldrb	r3, [r6, #5]
	movs	r7, #128
	adds	r5, r6, #0
	sub	sp, #8
	lsls	r7, r7, #1
	adds	r5, #16
	cmp	r3, #9
	beq.n	.L_080190b6
	cmp	r3, #9
	blt.n	.L_08019106
	cmp	r3, #10
	beq.n	.L_080190c8
	cmp	r3, #11
	beq.n	.L_080190da
	cmp	r3, #12
	beq.n	.L_080190f0
	b.n	.L_08019106
.L_080190b6:
	ldrh	r2, [r6, #12]
	adds	r3, r2, #1
	strh	r3, [r6, #12]
	movs	r3, #31
	ldr	r1, [pc, #192]
	ands	r3, r2
	lsls	r3, r3, #1
	ldrh	r7, [r1, r3]
	b.n	.L_08019106
.L_080190c8:
	ldrh	r2, [r6, #12]
	adds	r3, r2, #1
	strh	r3, [r6, #12]
	movs	r3, #31
	ldr	r1, [pc, #172]
	ands	r3, r2
	lsls	r3, r3, #1
	ldrh	r3, [r1, r3]
	b.n	.L_08019104
.L_080190da:
	ldrh	r3, [r6, #12]
	adds	r0, r3, #0
	cmp	r0, #7
	bhi.n	.L_08019106
	adds	r3, #1
	ldr	r2, [pc, #152]
	strh	r3, [r6, #12]
	lsls	r3, r0, #2
	adds	r3, #32
	ldrh	r7, [r2, r3]
	b.n	.L_08019106
.L_080190f0:
	ldrh	r3, [r6, #12]
	adds	r1, r3, #0
	cmp	r1, #7
	bhi.n	.L_08019106
	adds	r3, #1
	ldr	r2, [pc, #132]
	strh	r3, [r6, #12]
	lsls	r3, r1, #2
	adds	r3, #32
	ldrh	r3, [r2, r3]
.L_08019104:
	lsrs	r7, r3, #1
.L_08019106:
	movs	r3, #128
	lsls	r3, r3, #1
	mov	r8, r3
	cmp	r7, r8
	bne.n	.L_08019124
	ldrb	r2, [r5, #7]
	movs	r3, #63
	negs	r3, r3
	ands	r3, r2
	strb	r3, [r5, #7]
	ldrb	r2, [r5, #5]
	movs	r3, #4
	negs	r3, r3
	ands	r3, r2
	b.n	.L_080191a0
.L_08019124:
	ldr	r3, [sp, #0]
	ldr	r4, [pc, #92]
	ldr	r2, [pc, #92]
	adds	r1, r7, #0
	ands	r3, r4
	orrs	r3, r1
	ands	r3, r2
	lsls	r1, r1, #16
	orrs	r3, r1
	str	r3, [sp, #0]
	mov	r0, sp
	ldr	r3, [r0, #4]
	ands	r3, r4
	str	r3, [r0, #4]
	bl	sub_08003d28
	movs	r3, #31
	ands	r0, r3
	ldrb	r2, [r5, #7]
	movs	r3, #63
	negs	r3, r3
	lsls	r0, r0, #1
	ands	r3, r2
	orrs	r3, r0
	strb	r3, [r5, #7]
	cmp	r7, r8
	ble.n	.L_08019194
	ldrb	r3, [r5, #5]
	movs	r2, #3
	orrs	r3, r2
	strb	r3, [r5, #5]
	ldr	r3, [pc, #40]
	ldrh	r2, [r6, #6]
	adds	r2, r2, r3
	ldr	r3, [pc, #16]
	ldrh	r1, [r5, #6]
	ands	r2, r3
	ldr	r3, [pc, #32]
	ands	r3, r1
	orrs	r3, r2
	strh	r3, [r5, #6]
	ldrb	r3, [r6, #8]
	adds	r3, #248
	b.n	.L_080191b4
	.4byte 0x000001ff
	.4byte 0x080366f8
	.4byte 0xffff0000
	.4byte 0x0000ffff
	.4byte 0x0000fff8
	.2byte 0xfe00
	.2byte 0xffff
.L_08019194:
	ldrb	r2, [r5, #5]
	movs	r3, #4
	negs	r3, r3
	ands	r3, r2
	movs	r2, #1
	orrs	r3, r2
.L_080191a0:
	strb	r3, [r5, #5]
	ldr	r2, [pc, #32]
	ldrh	r3, [r6, #6]
	ldrh	r1, [r5, #6]
	ands	r2, r3
	ldr	r3, [pc, #28]
	ands	r3, r1
	orrs	r3, r2
	strh	r3, [r5, #6]
	ldrh	r3, [r6, #8]
.L_080191b4:
	strb	r3, [r5, #4]
	add	sp, #8
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x000001ff
	.2byte 0xfe00
	.2byte 0xffff
