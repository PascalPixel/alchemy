.syntax unified
.text
	.thumb
	.global Fragment_080b7eb4
	.thumb_func
Fragment_080b7eb4:
	ldr	r3, [pc, #28]
	ldr	r4, [r3, #0]
	movs	r3, #44
	adds	r2, r0, #0
	muls	r2, r3
	adds	r3, r2, #0
	adds	r3, #128
	ldr	r3, [r4, r3]
	str	r3, [r1, #0]
	movs	r3, #0
	str	r3, [r1, #4]
	adds	r2, #132
	ldr	r3, [r4, r2]
	movs	r0, #0
	str	r3, [r1, #8]
	bx	lr
	.4byte 0x03001e74
