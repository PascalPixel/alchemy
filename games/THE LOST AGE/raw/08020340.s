.syntax unified
	.thumb
	.global Overlay_08020340
Overlay_08020340:
	ldr	r4, [pc, #0]
	bx	r4
	.2byte 0x37e9
	.2byte 0x0802
	ldr	r4, [pc, #0]
	bx	r4
	.4byte 0x08026f81
