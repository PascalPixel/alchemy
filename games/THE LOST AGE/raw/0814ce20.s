.syntax unified
	.thumb
	.set sub_08143a88, 0x08143a88
	.set sub_08143bb8, 0x08143bb8
	.global Overlay_0814ce20
Overlay_0814ce20:
	push	{lr}
	movs	r0, #1
	bl	sub_08143a88
	bl	sub_08143bb8
	pop	{pc}
