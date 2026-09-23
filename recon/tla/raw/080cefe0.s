.syntax unified
	.thumb
	.set sub_08013560, 0x08013560
	.set sub_08020090, 0x08020090
	.set sub_080cad84, 0x080cad84
	.set sub_081c0010, 0x081c0010
	.global Func_080cefe0
	.thumb_func
Func_080cefe0:
	push	{lr}
	bl	sub_080cad84
	movs	r1, #6
	bl	sub_08020090
	movs	r0, #124
	bl	sub_081c0010
	movs	r0, #12
	bl	sub_08013560
	pop	{pc}
	.2byte 0x0000
