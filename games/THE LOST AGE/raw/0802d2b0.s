.syntax unified
	.thumb
	.global Overlay_0802d2b0
Overlay_0802d2b0:
	push	{r5, lr}
	movs	r3, #0
	ldrsb	r3, [r0, r3]
	adds	r5, r2, #0
	lsls	r4, r3, #19
	movs	r3, #1
	ldrsb	r3, [r0, r3]
	adds	r2, r4, #0
	lsls	r0, r3, #19
	cmp	r0, r4
	ble.n	.L_0802d2c8
	adds	r2, r0, #0
.L_0802d2c8:
	subs	r3, r5, r1
	adds	r1, r3, #0
	adds	r1, #15
	cmp	r1, #15
	bne.n	.L_0802d2d6
	adds	r0, r2, #0
	b.n	.L_0802d2dc
.L_0802d2d6:
	cmp	r1, #14
	bhi.n	.L_0802d2dc
	adds	r0, r4, #0
.L_0802d2dc:
	pop	{r5, pc}
