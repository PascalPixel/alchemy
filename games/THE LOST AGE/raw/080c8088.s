.syntax unified
	.thumb
	.global Overlay_080c8088
Overlay_080c8088:
	ldr	r4, [pc, #0]
	bx	r4
	.4byte 0x080d2d85
