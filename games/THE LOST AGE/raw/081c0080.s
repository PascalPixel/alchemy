.syntax unified
	.thumb
	.global Overlay_081c0080
Overlay_081c0080:
	ldr	r4, [pc, #0]
	bx	r4
	.2byte 0x1001
	.2byte 0x081c
	ldr	r4, [pc, #0]
	bx	r4
	.4byte 0x081c0e31
