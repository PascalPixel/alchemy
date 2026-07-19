.syntax unified
.text
	.thumb
	.global Fragment_080ec0f0
	.thumb_func
Fragment_080ec0f0:
	ldr	r2, [pc, #8]
	ldr	r3, [pc, #4]
	strh	r3, [r2, #0]
	bx	lr
	.4byte 0x00000080
	.4byte 0x04000020
