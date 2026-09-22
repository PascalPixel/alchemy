.syntax unified
	.thumb
	.global Overlay_080c8508
Overlay_080c8508:
	ldr	r4, [pc, #0]
	bx	r4
	.2byte 0xeb59
	.2byte 0x080c
	ldr	r4, [pc, #0]
	bx	r4
	.4byte 0x080ceb81
