.syntax unified
	.thumb
	.set sub_080dbb78, 0x080dbb78
	.global Func_080dbde8
	.thumb_func
Func_080dbde8:
	push	{lr}
	bl	sub_080dbb78
	ldrb	r0, [r0, #3]
	pop	{pc}
	.align 2, 0
