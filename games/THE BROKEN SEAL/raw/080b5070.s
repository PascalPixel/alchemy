.syntax unified
	.thumb
	.global Overlay_080b5070
Overlay_080b5070:
	ldr	r4, [pc, #0]
	bx	r4
	.2byte 0x8531
	.2byte 0x080b
	ldr	r4, [pc, #0]
	bx	r4
	.4byte 0x080b82c5
