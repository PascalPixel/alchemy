.syntax unified
	.thumb
	.global Overlay_08020380
Overlay_08020380:
	ldr	r4, [pc, #0]
	bx	r4
	.2byte 0x2c79
	.2byte 0x0802
	ldr	r4, [pc, #0]
	bx	r4
	.4byte 0x08022c85
