.syntax unified
	.thumb
	.global Overlay_080200a8
Overlay_080200a8:
	ldr	r4, [pc, #0]
	bx	r4
	.2byte 0x33a9
	.2byte 0x0802
	ldr	r4, [pc, #0]
	bx	r4
	.4byte 0x08023525
