.syntax unified
	.thumb
	.global Overlay_08020010
Overlay_08020010:
	ldr	r4, [pc, #0]
	bx	r4
	.2byte 0x20f1
	.2byte 0x0802
	ldr	r4, [pc, #0]
	bx	r4
	.4byte 0x08022319
