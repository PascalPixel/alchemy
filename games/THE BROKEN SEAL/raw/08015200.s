.syntax unified
	.thumb
	.global Overlay_08015200
Overlay_08015200:
	ldr	r4, [pc, #0]
	bx	r4
	.2byte 0xc34d
	.2byte 0x0801
	ldr	r4, [pc, #0]
	bx	r4
	.4byte 0x0801c429
