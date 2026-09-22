.syntax unified
	.thumb
	.global Overlay_08045768
Overlay_08045768:
	movs	r1, #128
	lsls	r1, r1, #19
	adds	r1, #10
	ldrh	r2, [r1, #0]
	movs	r3, #4
	negs	r3, r3
	ands	r3, r2
	movs	r2, #3
	orrs	r3, r2
	strh	r3, [r1, #0]
	bx	lr
