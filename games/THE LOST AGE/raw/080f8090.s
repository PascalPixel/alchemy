.syntax unified
	.thumb
	.global Overlay_080f8090
Overlay_080f8090:
	ldr	r4, [pc, #0]
	bx	r4
	.4byte 0x08105301
