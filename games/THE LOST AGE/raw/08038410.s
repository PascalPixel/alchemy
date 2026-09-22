.syntax unified
	.thumb
	.global Overlay_08038410
Overlay_08038410:
	ldr	r4, [pc, #0]
	bx	r4
	.2byte 0xd8ad
	.2byte 0x0803
	ldr	r4, [pc, #0]
	bx	r4
	.4byte 0x0803d021
