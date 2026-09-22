.syntax unified
	.thumb
	.global Overlay_08013164
Overlay_08013164:
	movs	r4, #3
	lsls	r4, r4, #24
	movs	r1, #4
	lsrs	r2, r0, #22
	ands	r2, r1
	str	r0, [r2, r4]
	bx	lr
