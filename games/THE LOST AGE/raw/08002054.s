.syntax unified
	.thumb
	.global Overlay_08002054
Overlay_08002054:
	ldr	r3, [pc, #0]
	bx	r3
	.2byte 0x0528
	.2byte 0x0300
	ldr	r3, [pc, #0]
	bx	r3
	.2byte 0x0534
	.2byte 0x0300
	ldr	r3, [pc, #0]
	bx	r3
	.2byte 0x0508
	.2byte 0x0300
	ldr	r3, [pc, #0]
	bx	r3
	.2byte 0x0514
	.2byte 0x0300
	bx	pc
