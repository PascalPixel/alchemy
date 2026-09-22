.syntax unified
	.thumb
	.global Overlay_08020328
Overlay_08020328:
	ldr	r4, [pc, #0]
	bx	r4
	.2byte 0xb7e1
	.2byte 0x0802
	ldr	r4, [pc, #0]
	bx	r4
	.4byte 0x080237c9
