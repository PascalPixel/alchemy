.syntax unified
	.thumb
	.set sub_08020030, 0x08020030
	.global Overlay_080ebea8
Overlay_080ebea8:
	push	{lr}
	ldr	r0, [r0, #0]
	bl	sub_08020030
	pop	{pc}
