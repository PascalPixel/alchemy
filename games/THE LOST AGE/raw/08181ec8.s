.syntax unified
	.thumb
	.set sub_08181ed4, 0x08181ed4
	.global Overlay_08181ec8
Overlay_08181ec8:
	push	{lr}
	movs	r1, #1
	bl	sub_08181ed4
	pop	{pc}
