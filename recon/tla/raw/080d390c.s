.syntax unified
	.thumb
	.balign 4
	.global Func_080d390c
	.thumb_func
Func_080d390c:
	push	{lr}
	bl	Object_GetById
	adds	r2, r0, #0
	movs	r3, #0
	adds	r2, #85
	strb	r3, [r2, #0]
	adds	r2, #4
	strb	r3, [r2, #0]
	movs	r1, #0
	bl	ObjectDispatch_SetSingleChildField26Far
	pop	{pc}
	.2byte 0x0000