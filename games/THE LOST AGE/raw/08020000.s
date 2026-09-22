.syntax unified
	.thumb
	.global Overlay_08020000
Overlay_08020000:
	ldr	r4, [pc, #0]
	bx	r4
	.4byte 0x08021919
