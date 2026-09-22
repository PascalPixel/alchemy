.syntax unified
	.thumb
	.global Overlay_08038200
Overlay_08038200:
	ldr	r4, [pc, #0]
	bx	r4
	.2byte 0xf86d
	.2byte 0x0803
	ldr	r4, [pc, #0]
	bx	r4
	.4byte 0x0803f93d
