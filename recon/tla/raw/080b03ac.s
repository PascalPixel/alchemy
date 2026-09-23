.syntax unified
	.thumb
	.set sub_080b02dc, 0x080b02dc
	.global Func_080b03ac
	.thumb_func
Func_080b03ac:
	push	{r5, r6, lr}
	adds	r5, r0, #0
	subs	r5, r5, r1
	adds	r6, r2, #0
	adds	r0, r3, #0
	cmp	r5, #0
	bge.n	.L_080b03bc
	movs	r5, #0
.L_080b03bc:
	movs	r1, #1
	bl	sub_080b02dc
	lsls	r3, r6, #1
	adds	r3, r5, r3
	muls	r0, r3
	cmp	r0, #0
	bge.n	.L_080b03d4
	movs	r3, #128
	lsls	r3, r3, #1
	adds	r3, #255
	adds	r0, r0, r3
.L_080b03d4:
	asrs	r0, r0, #9
	cmp	r0, #0
	bge.n	.L_080b03dc
	movs	r0, #0
.L_080b03dc:
	pop	{r5, r6, pc}
	.align 2, 0
