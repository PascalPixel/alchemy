.syntax unified
	.thumb
	.set sub_08166b10, 0x08166b10
	.global Overlay_08166b04
Overlay_08166b04:
	push	{lr}
	movs	r1, #1
	bl	sub_08166b10
	pop	{pc}
