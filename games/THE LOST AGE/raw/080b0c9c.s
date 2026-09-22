.syntax unified
	.thumb
	.set sub_08016ca4, 0x08016ca4
	.set sub_080b0298, 0x080b0298
	.set sub_080b0bb8, 0x080b0bb8
	.global Overlay_080b0c9c
Overlay_080b0c9c:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	adds	r7, r0, #0
	adds	r6, r1, #0
	mov	r8, r2
	bl	sub_08016ca4
	adds	r1, r6, #0
	adds	r5, r0, #0
	mov	r2, r8
	adds	r0, r7, #0
	bl	sub_080b0bb8
	mov	sl, r0
	cmp	r0, #0
	beq.n	.L_080b0cf8
	lsls	r2, r6, #2
	adds	r3, r2, #0
	adds	r3, #248
	ldr	r3, [r5, r3]
	movs	r1, #1
	mov	r0, r8
	lsls	r1, r0
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_080b0ce2
	movs	r3, #132
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldr	r3, [r5, r2]
	orrs	r3, r1
	str	r3, [r5, r2]
	b.n	.L_080b0ce6
.L_080b0ce2:
	movs	r0, #0
	b.n	.L_080b0cfa
.L_080b0ce6:
	movs	r0, #142
	lsls	r0, r0, #1
	adds	r2, r6, r0
	ldrb	r3, [r5, r2]
	adds	r0, r7, #0
	adds	r3, #1
	strb	r3, [r5, r2]
	bl	sub_080b0298
.L_080b0cf8:
	mov	r0, sl
.L_080b0cfa:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
