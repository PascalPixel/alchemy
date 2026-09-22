.syntax unified
	.thumb
	.set sub_080f9ee8, 0x080f9ee8
	.global Overlay_080f9ef8
Overlay_080f9ef8:
	push	{r4, r5, r6, lr}
	adds	r5, r1, #0
	ldrb	r1, [r5, #0]
	movs	r0, #128
	tst	r0, r1
	beq.n	.L_080f9f30
	ldr	r4, [r5, #32]
	cmp	r4, #0
	beq.n	.L_080f9f2e
	movs	r6, #0
.L_080f9f0c:
	ldrb	r0, [r4, #0]
	cmp	r0, #0
	beq.n	.L_080f9f26
	ldrb	r0, [r4, #1]
	movs	r3, #7
	ands	r0, r3
	beq.n	.L_080f9f24
	ldr	r3, [pc, #28]
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #44]
	bl	sub_080f9ee8
.L_080f9f24:
	strb	r6, [r4, #0]
.L_080f9f26:
	str	r6, [r4, #44]
	ldr	r4, [r4, #52]
	cmp	r4, #0
	bne.n	.L_080f9f0c
.L_080f9f2e:
	str	r4, [r5, #32]
.L_080f9f30:
	pop	{r4, r5, r6}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03007ff0
