.syntax unified
	.thumb
	.global Overlay_080f8028
Overlay_080f8028:
	ldr	r4, [pc, #0]
	bx	r4
	.2byte 0x8889
	.2byte 0x080f
	ldr	r4, [pc, #0]
	bx	r4
	.2byte 0x0e7d
	.2byte 0x0810
	ldr	r4, [pc, #0]
	bx	r4
	.2byte 0xb8ad
	.2byte 0x080f
	ldr	r4, [pc, #0]
	bx	r4
	.2byte 0xb8b9
	.2byte 0x080f
	ldr	r4, [pc, #0]
	bx	r4
	.4byte 0x080f92ad
