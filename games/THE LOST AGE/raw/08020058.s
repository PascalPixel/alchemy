.syntax unified
	.thumb
	.global Overlay_08020058
Overlay_08020058:
	ldr	r4, [pc, #0]
	bx	r4
	.2byte 0x29ad
	.2byte 0x0802
	ldr	r4, [pc, #0]
	bx	r4
	.2byte 0x2a25
	.2byte 0x0802
	ldr	r4, [pc, #0]
	bx	r4
	.2byte 0x2a85
	.2byte 0x0802
	ldr	r4, [pc, #0]
	bx	r4
	.4byte 0x08022d1d
