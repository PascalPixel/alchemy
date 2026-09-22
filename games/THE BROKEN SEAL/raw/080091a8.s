.syntax unified
	.thumb
	.global Overlay_080091a8
Overlay_080091a8:
	ldr	r4, [pc, #0]
	bx	r4
	.2byte 0x1f55
	.2byte 0x0801
	ldr	r4, [pc, #0]
	bx	r4
	.4byte 0x08012039
