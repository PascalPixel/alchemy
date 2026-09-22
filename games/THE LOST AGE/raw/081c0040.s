.syntax unified
	.thumb
	.global Overlay_081c0040
Overlay_081c0040:
	ldr	r4, [pc, #0]
	bx	r4
	.2byte 0x0fad
	.2byte 0x081c
	ldr	r4, [pc, #0]
	bx	r4
	.4byte 0x081c0fd1
