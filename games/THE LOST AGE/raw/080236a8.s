.syntax unified
	.thumb
	.global Overlay_080236a8
Overlay_080236a8:
	push	{lr}
	cmp	r0, #0
	beq.n	.L_080236cc
	adds	r3, r0, #0
	adds	r3, #84
	ldrb	r3, [r3, #0]
	cmp	r3, #1
	bne.n	.L_080236cc
	ldr	r0, [r0, #80]
	movs	r3, #1
	ldrb	r2, [r0, #17]
	ands	r1, r3
	movs	r3, #3
	negs	r3, r3
	lsls	r1, r1, #1
	ands	r3, r2
	orrs	r3, r1
	strb	r3, [r0, #17]
.L_080236cc:
	pop	{pc}
