.syntax unified
	.thumb
	.set sub_081280fc, 0x081280fc
	.global Overlay_0811a490
Overlay_0811a490:
	push	{r5, lr}
	bl	sub_081280fc
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r4, [r3, #36]
	movs	r1, #0
	movs	r2, #4
	ldrsh	r3, [r4, r2]
	cmp	r3, #0
	bne.n	.L_0811a4b2
	cmp	r0, #0
	bne.n	.L_0811a4d2
	movs	r5, #6
	ldrsh	r3, [r4, r5]
	cmp	r3, #0
	beq.n	.L_0811a4d2
.L_0811a4b2:
	adds	r1, #1
	cmp	r1, #5
	bgt.n	.L_0811a4d2
	lsls	r2, r1, #1
	adds	r3, r2, #4
	ldrsh	r3, [r4, r3]
	cmp	r3, #0
	bne.n	.L_0811a4b2
	cmp	r0, #0
	bne.n	.L_0811a4d2
	cmp	r1, #4
	bgt.n	.L_0811a4b2
	adds	r3, r2, #6
	ldrsh	r3, [r4, r3]
	cmp	r3, #0
	bne.n	.L_0811a4b2
.L_0811a4d2:
	movs	r3, #6
	eors	r3, r1
	negs	r0, r3
	orrs	r0, r3
	lsrs	r0, r0, #31
	pop	{r5, pc}
