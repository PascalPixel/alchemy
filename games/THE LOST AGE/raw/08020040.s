.syntax unified
	.thumb
	.global Overlay_08020040
Overlay_08020040:
	ldr	r4, [pc, #0]
	bx	r4
	.2byte 0x2d41
	.2byte 0x0802
	ldr	r4, [pc, #0]
	bx	r4
	.4byte 0x08022e91
