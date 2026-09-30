.syntax unified
	.thumb
	.balign 4
	.global Func_080d5e64
	.thumb_func
Func_080d5e64:
	push	{lr}
	movs	r0, #25
	bl	ObjectEffect_PrepareContextEffect
	movs	r0, #34
	adds	r0, #255
	bl	GameFlag_SetBit
	pop	{pc}
	.2byte 0x0000
