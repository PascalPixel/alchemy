.syntax unified
	.thumb
	.set sub_08016cfc, 0x08016cfc
	.set sub_080d5de0, 0x080d5de0
	.global Func_080d5e64
	.thumb_func
Func_080d5e64:
	push	{lr}
	movs	r0, #25
	bl	sub_080d5de0
	movs	r0, #34
	adds	r0, #255
	bl	sub_08016cfc
	pop	{pc}
	.align 2, 0
