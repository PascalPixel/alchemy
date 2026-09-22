.syntax unified
	.thumb
	.global Overlay_08009048
Overlay_08009048:
	ldr	r4, [pc, #0]
	bx	r4
	.2byte 0xb8ad
	.2byte 0x0800
	ldr	r4, [pc, #0]
	bx	r4
	.4byte 0x0800b93d
