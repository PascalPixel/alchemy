.syntax unified
	.thumb
	.set sub_08016ca4, 0x08016ca4
	.set sub_080ad348, 0x080ad348
	.set sub_080b0298, 0x080b0298
	.set sub_080b0c78, 0x080b0c78
	.global Overlay_080b0d04
Overlay_080b0d04:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	adds	r7, r0, #0
	adds	r6, r1, #0
	mov	sl, r2
	bl	sub_08016ca4
	adds	r1, r6, #0
	adds	r5, r0, #0
	mov	r2, sl
	adds	r0, r7, #0
	bl	sub_080b0c78
	mov	r8, r0
	cmp	r0, #0
	beq.n	.L_080b0d4e
	movs	r3, #142
	lsls	r3, r3, #1
	adds	r2, r6, r3
	ldrb	r3, [r5, r2]
	lsls	r1, r6, #2
	adds	r3, #255
	strb	r3, [r5, r2]
	movs	r3, #132
	lsls	r3, r3, #1
	adds	r1, r1, r3
	movs	r2, #1
	mov	r3, sl
	lsls	r2, r3
	ldr	r3, [r5, r1]
	adds	r0, r7, #0
	bics	r3, r2
	str	r3, [r5, r1]
	bl	sub_080b0298
.L_080b0d4e:
	mov	r0, r8
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	mov	r8, r1
	movs	r1, #0
	mov	sl, r2
	mov	r9, r1
	movs	r3, #0
	cmp	r0, #7
	bls.n	.L_080b0d72
	movs	r3, #1
.L_080b0d72:
	adds	r0, r3, #0
	bl	sub_080ad348
	movs	r1, #148
	adds	r3, r0, #0
	lsls	r1, r1, #1
	movs	r2, #8
	adds	r2, r2, r3
	adds	r7, r3, r1
	mov	ip, r2
	ldr	r2, [r7, #0]
	movs	r4, #0
	adds	r0, #9
	movs	r5, #0
	mov	r1, ip
	cmp	r9, r2
	bge.n	.L_080b0df8
	ldrb	r3, [r1, #0]
	mov	r6, r9
	mov	lr, r3
	cmp	r8, lr
	bne.n	.L_080b0dae
	ldrb	r3, [r0, #0]
	cmp	sl, r3
	bne.n	.L_080b0dae
	subs	r3, r2, #1
	movs	r1, #1
	str	r3, [r7, #0]
	mov	r9, r1
	b.n	.L_080b0dd2
.L_080b0dae:
	ldr	r2, [r7, #0]
	adds	r4, #1
	adds	r0, #4
	adds	r1, #4
	adds	r5, #4
	cmp	r4, r2
	bge.n	.L_080b0df8
	ldrb	r3, [r1, #0]
	adds	r6, r5, #0
	cmp	r8, r3
	bne.n	.L_080b0dae
	ldrb	r3, [r0, #0]
	cmp	sl, r3
	bne.n	.L_080b0dae
	subs	r3, r2, #1
	str	r3, [r7, #0]
	movs	r2, #1
	mov	r9, r2
.L_080b0dd2:
	movs	r3, #144
	lsls	r3, r3, #1
	add	r3, ip
	ldr	r3, [r3, #0]
	cmp	r4, r3
	bge.n	.L_080b0df8
	movs	r2, #144
	lsls	r2, r2, #1
	add	r2, ip
	b.n	.L_080b0de8
.L_080b0de6:
	lsls	r6, r4, #2
.L_080b0de8:
	mov	r1, ip
	adds	r3, r6, #4
	ldr	r3, [r1, r3]
	adds	r4, #1
	str	r3, [r1, r6]
	ldr	r3, [r2, #0]
	cmp	r4, r3
	blt.n	.L_080b0de6
.L_080b0df8:
	mov	r0, r9
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
