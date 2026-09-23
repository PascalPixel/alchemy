.syntax unified
	.thumb
	.global Func_081c16cc
	.thumb_func
Func_081c16cc:
.L_081c16cc:
	ldr	r3, [r0, #44]
	cmp	r3, #0
	beq.n	.L_081c16ea
	ldr	r1, [r0, #52]
	ldr	r2, [r0, #48]
	cmp	r2, #0
	beq.n	.L_081c16de
	str	r1, [r2, #52]
	b.n	.L_081c16e0
.L_081c16de:
	str	r1, [r3, #32]
.L_081c16e0:
	cmp	r1, #0
	beq.n	.L_081c16e6
	str	r2, [r1, #48]
.L_081c16e6:
	movs	r1, #0
	str	r1, [r0, #44]
.L_081c16ea:
	bx	lr
	push	{r4, r5, lr}
	adds	r5, r1, #0
	ldr	r4, [r5, #32]
	cmp	r4, #0
	beq.n	.L_081c1710
.L_081c16f6:
	ldrb	r1, [r4, #0]
	movs	r0, #199
	tst	r0, r1
	beq.n	.L_081c1704
	movs	r0, #64
	orrs	r1, r0
	strb	r1, [r4, #0]
.L_081c1704:
	adds	r0, r4, #0
	bl	.L_081c16cc
	ldr	r4, [r4, #52]
	cmp	r4, #0
	bne.n	.L_081c16f6
.L_081c1710:
	movs	r0, #0
	strb	r0, [r5, #0]
	pop	{r4, r5}
	pop	{r0}
	bx	r0
	.align 2, 0
