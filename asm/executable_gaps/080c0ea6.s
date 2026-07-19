.syntax unified
.text
	.thumb
	.global Fragment_080c0ea6
	.thumb_func
Fragment_080c0ea6:
	movs	r0, r0
	ldr	r2, [pc, #8]
	ldr	r3, [pc, #4]
	strh	r3, [r2, #0]
	bx	lr
	.4byte 0x000000bf
	.4byte 0x04000050
