.syntax unified
	.thumb
	.global Overlay_08020210
Overlay_08020210:
	ldr	r4, [pc, #0]
	bx	r4
	.2byte 0xd87d
	.2byte 0x0802
	ldr	r4, [pc, #0]
	bx	r4
	.4byte 0x08023665
