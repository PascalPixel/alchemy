.syntax unified
	.thumb
	.global Overlay_08038090
Overlay_08038090:
	ldr	r4, [pc, #0]
	bx	r4
	.2byte 0x20fd
	.2byte 0x0804
	ldr	r4, [pc, #0]
	bx	r4
	.4byte 0x08042189
