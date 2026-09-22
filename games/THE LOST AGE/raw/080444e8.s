.syntax unified
	.thumb
	.set sub_080420a8, 0x080420a8
	.global Overlay_080444e8
Overlay_080444e8:
	push	{r5, r6, lr}
	ldrb	r2, [r1, #0]
	sub	sp, #20
	adds	r3, r2, #0
	adds	r6, r0, #0
	movs	r4, #0
	cmp	r3, #0
	beq.n	.L_08044510
	mov	r0, sp
	adds	r5, r0, #0
.L_080444fc:
	strb	r2, [r5, #0]
	adds	r1, #1
	ldrb	r3, [r1, #0]
	adds	r5, #1
	adds	r2, r3, #0
	adds	r3, r2, #0
	adds	r4, #1
	cmp	r3, #0
	bne.n	.L_080444fc
	b.n	.L_08044512
.L_08044510:
	mov	r0, sp
.L_08044512:
	movs	r3, #8
	strb	r3, [r0, r4]
	adds	r4, #1
	movs	r3, #2
	strb	r3, [r0, r4]
	adds	r4, #1
	cmp	r4, #6
	bgt.n	.L_08044536
	movs	r3, #7
	adds	r2, r4, r0
	movs	r1, #95
	subs	r4, r3, r4
.L_0804452a:
	subs	r4, #1
	strb	r1, [r2, #0]
	adds	r2, #1
	cmp	r4, #0
	bne.n	.L_0804452a
	movs	r4, #7
.L_08044536:
	movs	r3, #8
	strb	r3, [r0, r4]
	adds	r4, #1
	movs	r3, #15
	strb	r3, [r0, r4]
	adds	r4, #1
	movs	r3, #0
	strb	r3, [r0, r4]
	movs	r3, #2
	negs	r3, r3
	adds	r1, r6, #0
	movs	r2, #0
	bl	sub_080420a8
	add	sp, #20
	pop	{r5, r6, pc}
