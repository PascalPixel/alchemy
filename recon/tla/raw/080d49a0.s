.syntax unified
	.thumb
	push	{lr}
	movs	r1, #0
	bl	ObjectDispatch_SetSingleChildField26Far
	movs	r0, #0
	pop	{pc}
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #32
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r0, #0
	bx	lr
	.2byte 0x0000
