.syntax unified
	.thumb
	.set sub_08020090, 0x08020090
	.set sub_080cad84, 0x080cad84
	.global Func_080cefd0
	.thumb_func
Func_080cefd0:
	push	{lr}
	bl	sub_080cad84
	movs	r1, #2
	bl	sub_08020090
	pop	{pc}
	.align 2, 0
