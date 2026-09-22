.syntax unified
	.thumb
	.global Overlay_080090c8
Overlay_080090c8:
	ldr	r4, [pc, #0]
	bx	r4
	.2byte 0xc151
	.2byte 0x0800
	ldr	r4, [pc, #0]
	bx	r4
	.4byte 0x0800c0f5
