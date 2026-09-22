.syntax unified
	.thumb
	.global Overlay_080f8000
Overlay_080f8000:
	ldr	r4, [pc, #0]
	bx	r4
	.2byte 0x94a5
	.2byte 0x080f
	ldr	r4, [pc, #0]
	bx	r4
	.2byte 0xcab9
	.2byte 0x080f
	ldr	r4, [pc, #0]
	bx	r4
	.4byte 0x080fe275
