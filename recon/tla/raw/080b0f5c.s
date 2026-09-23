.syntax unified
	.thumb
	.set sub_08016ca4, 0x08016ca4
	.global Owner_SumDjinnCounts
	.global Func_080b0f5c
	.thumb_func
Owner_SumDjinnCounts:
Func_080b0f5c:
	push	{r5, lr}
	adds	r5, r1, #0
	bl	sub_08016ca4
	movs	r1, #1
	negs	r1, r1
	adds	r2, r0, #0
	cmp	r5, r1
	bne.n	.L_080b0f90
	movs	r1, #140
	lsls	r1, r1, #1
	adds	r3, r2, r1
	adds	r1, #1
	ldrb	r0, [r3, #0]
	adds	r3, r2, r1
	ldrb	r3, [r3, #0]
	adds	r1, #1
	adds	r0, r0, r3
	adds	r3, r2, r1
	ldrb	r3, [r3, #0]
	adds	r1, #1
	adds	r0, r0, r3
	adds	r3, r2, r1
	ldrb	r3, [r3, #0]
	adds	r0, r0, r3
	b.n	.L_080b0f98
.L_080b0f90:
	movs	r1, #140
	lsls	r1, r1, #1
	adds	r3, r5, r1
	ldrb	r0, [r2, r3]
.L_080b0f98:
	pop	{r5, pc}
	.2byte 0x0000
