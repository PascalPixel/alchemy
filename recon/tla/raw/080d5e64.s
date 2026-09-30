.syntax unified
	.thumb
	.global Func_080d5e64
	.thumb_func
Func_080d5e64:
	push	{lr}
	movs	r0, #25
	bl	0x080d5de0
	movs	r0, #34
	adds	r0, #255
	bl	GameFlag_SetBitFar
	pop	{pc}
	.2byte 0x0000
