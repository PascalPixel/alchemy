.syntax unified
	.thumb
	.set sub_0803d2f0, 0x0803d2f0
	.set sub_0803dab0, 0x0803dab0
	.global Overlay_08042508
Overlay_08042508:
	push	{r5, r6, r7, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r5, r0, #0
	adds	r0, r1, #0
	adds	r7, r2, #0
	sub	sp, #16
	ldr	r6, [r3, #60]
	bl	sub_0803d2f0
	movs	r2, #1
	negs	r2, r2
	cmp	r0, r2
	beq.n	.L_08042568
	cmp	r5, #1
	bls.n	.L_08042548
	movs	r2, #152
	lsls	r2, r2, #5
	adds	r2, #126
	adds	r3, r6, r2
	ldrh	r3, [r3, #0]
	movs	r5, #1
	cmp	r3, r0
	beq.n	.L_08042548
	movs	r2, #152
	lsls	r2, r2, #5
	adds	r2, #124
	adds	r3, r6, r2
	ldrh	r3, [r3, #0]
	cmp	r3, r0
	bne.n	.L_08042568
	movs	r5, #0
.L_08042548:
	movs	r2, #156
	lsls	r2, r2, #5
	lsls	r3, r5, #1
	adds	r3, r3, r2
	ldrh	r3, [r6, r3]
	adds	r1, r5, #0
	adds	r1, #14
	str	r1, [sp, #0]
	movs	r1, #1
	str	r3, [sp, #12]
	str	r1, [sp, #4]
	add	r2, sp, #12
	add	r3, sp, #8
	adds	r1, r7, #0
	bl	sub_0803dab0
.L_08042568:
	add	sp, #16
	pop	{r5, r6, r7, pc}
	push	{lr}
	cmp	r0, #0
	beq.n	.L_08042578
	movs	r3, #0
	strb	r1, [r0, #5]
	strh	r3, [r0, #12]
.L_08042578:
	pop	{pc}
