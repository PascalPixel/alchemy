.syntax unified
.text
	.thumb
	.global Fragment_080b7410
	.thumb_func
Fragment_080b7410:
	ldr	r4, [pc, #12]
	lsls	r0, r0, #1
	ldrsb	r3, [r4, r0]
	adds	r0, #1
	str	r3, [r1, #0]
	ldrsb	r3, [r4, r0]
	str	r3, [r2, #0]
	bx	lr
	.4byte 0x080c2a62
