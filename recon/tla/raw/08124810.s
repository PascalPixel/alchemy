.syntax unified
	.thumb
	.set sub_08016ca4, 0x08016ca4
	.set sub_080ad148, 0x080ad148
	.global Func_08124810
	.thumb_func
Func_08124810:
	push	{r5, r6, lr}
	adds	r5, r1, #0
	adds	r6, r2, #0
	bl	sub_08016ca4
	cmp	r5, #5
	bgt.n	.L_0812484e
	adds	r3, r0, #0
	adds	r3, #66
	ldrb	r3, [r3, #0]
	lsls	r2, r3, #1
	adds	r2, r2, r3
	lsls	r3, r5, #2
	adds	r3, r3, r5
	subs	r2, r2, r3
	movs	r3, #200
	lsls	r3, r3, #1
	adds	r3, #255
	adds	r2, r2, r6
	adds	r5, r2, #0
	muls	r5, r3
	bl	sub_080ad148
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	ands	r0, r3
	cmp	r5, r0
	blt.n	.L_0812484e
	movs	r0, #1
	b.n	.L_08124850
.L_0812484e:
	movs	r0, #0
.L_08124850:
	pop	{r5, r6, pc}
	.2byte 0x0000
