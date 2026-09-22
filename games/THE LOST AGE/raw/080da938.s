.syntax unified
	.thumb
	.global Overlay_080da938
Overlay_080da938:
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #160
	lsls	r1, r0, #3
	ldr	r4, [r3, #0]
	subs	r3, r1, r0
	lsls	r3, r3, #2
	adds	r3, #36
	ldr	r3, [r4, r3]
	cmp	r3, #0
	beq.n	.L_080da95a
	movs	r2, #2
.L_080da952:
	strb	r2, [r3, #16]
	ldr	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_080da952
.L_080da95a:
	subs	r3, r1, r0
	lsls	r3, r3, #2
	adds	r3, #32
	movs	r2, #0
	str	r2, [r4, r3]
	pop	{pc}
