.syntax unified
	.thumb
	.global Overlay_08009070
Overlay_08009070:
	ldr	r4, [pc, #0]
	bx	r4
	.2byte 0xb9f5
	.2byte 0x0800
	ldr	r4, [pc, #0]
	bx	r4
	.2byte 0xc005
	.2byte 0x0800
	ldr	r4, [pc, #0]
	bx	r4
	.2byte 0xc301
	.2byte 0x0800
	ldr	r4, [pc, #0]
	bx	r4
	.4byte 0x0800c345
