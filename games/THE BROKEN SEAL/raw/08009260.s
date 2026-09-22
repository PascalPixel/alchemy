.syntax unified
	.thumb
	.global Overlay_08009260
Overlay_08009260:
	ldr	r4, [pc, #0]
	bx	r4
	.2byte 0xbe21
	.2byte 0x0800
	ldr	r4, [pc, #0]
	bx	r4
	.2byte 0x22c9
	.2byte 0x0801
	ldr	r4, [pc, #0]
	bx	r4
	.4byte 0x0800b6b9
