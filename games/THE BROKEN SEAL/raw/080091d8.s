.syntax unified
	.thumb
	.global Overlay_080091d8
Overlay_080091d8:
	ldr	r4, [pc, #0]
	bx	r4
	.2byte 0x20dd
	.2byte 0x0801
	ldr	r4, [pc, #0]
	bx	r4
	.4byte 0x0800c529
