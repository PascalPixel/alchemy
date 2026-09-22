.syntax unified
	.thumb
	.global Overlay_08015090
Overlay_08015090:
	ldr	r4, [pc, #0]
	bx	r4
	.2byte 0xe8b1
	.2byte 0x0801
	ldr	r4, [pc, #0]
	bx	r4
	.4byte 0x0801e941
