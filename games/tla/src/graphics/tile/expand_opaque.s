.syntax unified
	.thumb
	.global Tile_ExpandOpaqueCode
Tile_ExpandOpaqueCode:
	.global Tile_ExpandOpaque
	.thumb_func
Tile_ExpandOpaque:
	bx	pc
AlchemyUncredited_08038e32:
	.2byte	0x0200
	.arm
AlchemyUncreditedEnd_08038e32:
	push	{r5, r6, r7, r8, r9, sl}
	add	sl, pc, #100
	ldm	sl, {r4, r6, r7}
.L_08038e40:
	add	sl, r0, r2, lsl #3
.L_08038e44:
	add	r9, r1, #32
.L_08038e48:
	ldm	r0, {r5, ip}
	orr	r5, r5, r5, lsr #4
	and	r5, r5, r4
	orr	r5, r5, r5, lsr #8
	and	r5, r5, r6
	orr	ip, ip, ip, lsr #4
	and	ip, ip, r4
	orr	ip, ip, ip, lsr #8
	orr	ip, r5, ip, lsl #16
	str	ip, [r1], #4
	add	r0, r0, r2, lsl #3
	cmp	r1, r9
	bne	.L_08038e48
	sub	r0, r0, r2, lsl #6
	add	r0, r0, #8
	cmp	r0, sl
	bne	.L_08038e44
	add	r0, r0, r2, lsl #6
	sub	r0, r0, r2, lsl #3
	subs	r3, r3, #1
	bne	.L_08038e40
	pop	{r5, r6, r7, r8, r9, sl}
	bx	lr
