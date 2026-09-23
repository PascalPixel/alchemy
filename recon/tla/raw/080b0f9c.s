.syntax unified
	.thumb
	.set sub_08016ca4, 0x08016ca4
	.set sub_080afe78, 0x080afe78
	.global Party_SumDjinnCounts
	.global Func_080b0f9c
	.thumb_func
Party_SumDjinnCounts:
Func_080b0f9c:
	push	{r5, r6, r7, lr}
	sub	sp, #36
	add	r6, sp, #4
	adds	r7, r0, #0
	adds	r0, r6, #0
	movs	r5, #0
	bl	sub_080afe78
	cmp	r5, r0
	bge.n	.L_080b0ffc
	adds	r1, r6, #0
	adds	r6, r0, #0
.L_080b0fb4:
	ldrh	r0, [r1, #0]
	adds	r1, #2
	str	r1, [sp, #0]
	bl	sub_08016ca4
	adds	r2, r0, #0
	movs	r0, #1
	negs	r0, r0
	ldr	r1, [sp, #0]
	cmp	r7, r0
	bne.n	.L_080b0fec
	movs	r0, #140
	lsls	r0, r0, #1
	adds	r3, r2, r0
	ldrb	r3, [r3, #0]
	adds	r0, #1
	adds	r5, r5, r3
	adds	r3, r2, r0
	ldrb	r3, [r3, #0]
	adds	r0, #1
	adds	r5, r5, r3
	adds	r3, r2, r0
	ldrb	r3, [r3, #0]
	adds	r0, #1
	adds	r5, r5, r3
	adds	r3, r2, r0
	ldrb	r3, [r3, #0]
	b.n	.L_080b0ff4
.L_080b0fec:
	movs	r0, #140
	lsls	r0, r0, #1
	adds	r3, r7, r0
	ldrb	r3, [r2, r3]
.L_080b0ff4:
	adds	r5, r5, r3
	subs	r6, #1
	cmp	r6, #0
	bne.n	.L_080b0fb4
.L_080b0ffc:
	adds	r0, r5, #0
	add	sp, #36
	pop	{r5, r6, r7, pc}
	.align 2, 0
