.syntax unified
.text
	.thumb
	.global Fragment_08095348
	.thumb_func
Fragment_08095348:
	ldr	r4, [r0, #104]
	ldr	r1, [r0, #8]
	ldr	r3, [r4, #8]
	subs	r3, r3, r1
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	adds	r1, r1, r3
	str	r1, [r0, #8]
	ldr	r1, [r0, #12]
	ldr	r3, [r4, #12]
	subs	r3, r3, r1
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	adds	r1, r1, r3
	str	r1, [r0, #12]
	ldr	r1, [r0, #16]
	ldr	r3, [r4, #16]
	subs	r3, r3, r1
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	adds	r1, r1, r3
	str	r1, [r0, #16]
	bx	lr
