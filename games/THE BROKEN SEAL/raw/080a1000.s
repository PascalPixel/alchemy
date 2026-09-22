.syntax unified
	.thumb
	.global Overlay_080a1000
Overlay_080a1000:
	ldr	r4, [pc, #0]
	bx	r4
	.2byte 0x24d1
	.2byte 0x080a
	ldr	r4, [pc, #0]
	bx	r4
	.2byte 0x5b95
	.2byte 0x080a
	ldr	r4, [pc, #0]
	bx	r4
	.4byte 0x080a7479
