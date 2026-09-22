.syntax unified
	.thumb
	.global Overlay_0802e6d8
Overlay_0802e6d8:
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #16]
	movs	r3, #3
	ands	r3, r0
	lsls	r3, r3, #2
	adds	r4, r3, #0
	adds	r4, #40
	movs	r0, #9
.L_0802e6ec:
	ldr	r3, [r2, r4]
	subs	r0, #1
	strb	r1, [r3, #6]
	adds	r2, #56
	cmp	r0, #0
	bge.n	.L_0802e6ec
	pop	{pc}
