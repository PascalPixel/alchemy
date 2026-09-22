.syntax unified
	.thumb
	.global Overlay_080383d0
Overlay_080383d0:
	ldr	r4, [pc, #0]
	bx	r4
	.2byte 0x5551
	.2byte 0x0804
	ldr	r4, [pc, #0]
	bx	r4
	.4byte 0x0803a60d
