.syntax unified
	.thumb
	.set sub_080770c0, 0x080770c0
	.set sub_080770c8, 0x080770c8
	.set sub_080770d0, 0x080770d0
	.global Overlay_0802938c
Overlay_0802938c:
	push	{r5, r6, lr}
	ldr	r6, [pc, #312]
	ldr	r3, [r6, #0]
	adds	r5, r2, #0
	movs	r2, #1
	ands	r3, r2
	adds	r4, r5, #4
	cmp	r3, #0
	beq.n	.L_080293c6
	ldr	r3, [r1, #0]
	ldr	r2, [r4, #0]
	lsls	r3, r3, #4
	adds	r3, r3, r2
	ldr	r2, [r5, #0]
	lsls	r3, r3, #4
	adds	r5, r3, r2
	adds	r0, r5, #0
	bl	sub_080770c0
	cmp	r0, #0
	beq.n	.L_080293be
	adds	r0, r5, #0
	bl	sub_080770d0
	b.n	.L_080294bc
.L_080293be:
	adds	r0, r5, #0
	bl	sub_080770c8
	b.n	.L_080294bc
.L_080293c6:
	ldr	r3, [pc, #260]
	ldr	r3, [r3, #0]
	movs	r2, #2
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_080293dc
	ldr	r3, [r6, #0]
	movs	r2, #4
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080293e2
.L_080293dc:
	movs	r0, #1
	negs	r0, r0
	b.n	.L_080294c2
.L_080293e2:
	ldr	r0, [r6, #0]
	movs	r3, #64
	ands	r0, r3
	cmp	r0, #0
	beq.n	.L_080293fc
	ldr	r3, [r4, #0]
	subs	r3, #1
	str	r3, [r4, #0]
	cmp	r3, #0
	bge.n	.L_080294c0
	movs	r3, #15
	str	r3, [r4, #0]
	b.n	.L_080294c0
.L_080293fc:
	ldr	r3, [r6, #0]
	movs	r2, #128
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08029414
	ldr	r3, [r4, #0]
	adds	r3, #1
	str	r3, [r4, #0]
	cmp	r3, #15
	ble.n	.L_080294c0
	str	r0, [r4, #0]
	b.n	.L_080294c0
.L_08029414:
	ldr	r0, [r6, #0]
	movs	r3, #32
	ands	r0, r3
	cmp	r0, #0
	beq.n	.L_0802942e
	ldr	r3, [r5, #0]
	subs	r3, #1
	str	r3, [r5, #0]
	cmp	r3, #0
	bge.n	.L_080294c0
	movs	r3, #15
	str	r3, [r5, #0]
	b.n	.L_080294c0
.L_0802942e:
	ldr	r3, [r6, #0]
	movs	r2, #16
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08029446
	ldr	r3, [r5, #0]
	adds	r3, #1
	str	r3, [r5, #0]
	cmp	r3, #15
	ble.n	.L_080294c0
	str	r0, [r5, #0]
	b.n	.L_080294c0
.L_08029446:
	ldr	r3, [r6, #0]
	movs	r2, #128
	lsls	r2, r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08029462
	ldr	r3, [r6, #0]
	movs	r2, #8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08029462
	ldr	r3, [r1, #0]
	subs	r3, #10
	b.n	.L_08029498
.L_08029462:
	ldr	r3, [r6, #0]
	movs	r2, #128
	lsls	r2, r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08029488
	ldr	r3, [r6, #0]
	movs	r2, #8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08029488
	ldr	r3, [r1, #0]
	adds	r3, #10
	str	r3, [r1, #0]
	cmp	r3, #15
	ble.n	.L_080294bc
	movs	r3, #0
	str	r3, [r1, #0]
	b.n	.L_080294bc
.L_08029488:
	ldr	r0, [r6, #0]
	movs	r3, #128
	lsls	r3, r3, #2
	ands	r0, r3
	cmp	r0, #0
	beq.n	.L_080294a4
	ldr	r3, [r1, #0]
	subs	r3, #1
.L_08029498:
	str	r3, [r1, #0]
	cmp	r3, #0
	bge.n	.L_080294bc
	movs	r3, #15
	str	r3, [r1, #0]
	b.n	.L_080294bc
.L_080294a4:
	ldr	r3, [r6, #0]
	movs	r2, #128
	lsls	r2, r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080294c0
	ldr	r3, [r1, #0]
	adds	r3, #1
	str	r3, [r1, #0]
	cmp	r3, #15
	ble.n	.L_080294bc
	str	r0, [r1, #0]
.L_080294bc:
	movs	r0, #1
	b.n	.L_080294c2
.L_080294c0:
	movs	r0, #0
.L_080294c2:
	pop	{r5, r6}
	pop	{r1}
	bx	r1
	.4byte 0x03001b04
	.4byte 0x03001c94
