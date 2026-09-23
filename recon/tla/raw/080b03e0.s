.syntax unified
	.thumb
	.set sub_080b02dc, 0x080b02dc
	.global Func_080b03e0
	.thumb_func
Func_080b03e0:
	push	{r5, r6, lr}
	adds	r6, r0, #0
	adds	r0, r1, #0
	movs	r1, #0
	adds	r5, r2, #0
	bl	sub_080b02dc
	adds	r3, r6, #0
	muls	r3, r0
	adds	r0, r5, #0
	muls	r0, r3
	cmp	r0, #0
	bge.n	.L_080b0402
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	adds	r0, r0, r3
.L_080b0402:
	asrs	r0, r0, #16
	pop	{r5, r6, pc}
	.align 2, 0
