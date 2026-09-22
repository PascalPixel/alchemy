.syntax unified
	.thumb
	.global Overlay_08023680
Overlay_08023680:
	push	{lr}
	cmp	r0, #0
	beq.n	.L_080236a4
	adds	r3, r0, #0
	adds	r3, #84
	ldrb	r3, [r3, #0]
	cmp	r3, #1
	bne.n	.L_080236a4
	ldr	r0, [r0, #80]
	movs	r3, #3
	ldrb	r2, [r0, #5]
	ands	r1, r3
	movs	r3, #13
	negs	r3, r3
	lsls	r1, r1, #2
	ands	r3, r2
	orrs	r3, r1
	strb	r3, [r0, #5]
.L_080236a4:
	pop	{pc}
