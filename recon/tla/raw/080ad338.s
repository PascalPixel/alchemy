.syntax unified
	.thumb
	.global Func_080ad338
	.thumb_func
Func_080ad338:
	push	{lr}
	bl	0x080addf0
	movs	r0, #0
	bl	Func_080c8008
	pop	{pc}
	.2byte 0x0000
