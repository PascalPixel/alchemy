.syntax unified
	.thumb
	.global Overlay_080f9008
Overlay_080f9008:
	ldr	r4, [pc, #0]
	bx	r4
	.2byte 0x9439
	.2byte 0x080f
	ldr	r4, [pc, #0]
	bx	r4
	.2byte 0x9081
	.2byte 0x080f
	ldr	r4, [pc, #0]
	bx	r4
	.4byte 0x080f91e9
