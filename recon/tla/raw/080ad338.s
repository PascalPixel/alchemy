.syntax unified
	.thumb
	.set sub_080addf0, 0x080addf0
	.set sub_080c8008, 0x080c8008
	.global Func_080ad338
	.thumb_func
Func_080ad338:
	push	{lr}
	bl	sub_080addf0
	movs	r0, #0
	bl	sub_080c8008
	pop	{pc}
	.align 2, 0
