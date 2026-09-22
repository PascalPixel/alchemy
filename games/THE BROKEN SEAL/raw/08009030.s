.syntax unified
	.thumb
	.global Overlay_08009030
Overlay_08009030:
	ldr	r4, [pc, #0]
	bx	r4
	.2byte 0xbc71
	.2byte 0x0800
	ldr	r4, [pc, #0]
	bx	r4
	.4byte 0x0800bdd5
