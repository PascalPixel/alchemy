.syntax unified
	.thumb
	.global Overlay_080200c0
Overlay_080200c0:
	ldr	r4, [pc, #0]
	bx	r4
	.2byte 0x3221
	.2byte 0x0802
	ldr	r4, [pc, #0]
	bx	r4
	.4byte 0x080231c9
