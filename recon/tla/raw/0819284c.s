.syntax unified
	.thumb
	.global Func_0819284c
	.thumb_func
Func_0819284c:
	push	{r5, lr}
	mov	r5, r9
	push	{r5}
	sub	sp, #4
	mov	r3, sp
	mov	r2, r9
	str	r2, [r3, #0]
	adds	r3, r2, #0
	subs	r4, r3, #4
	ldr	r3, [r4, #0]
	movs	r0, #36
	movs	r5, #0
	ldrsh	r3, [r3, r5]
	movs	r1, #0
	cmp	r3, #0
	bne.n	.L_08192870
	movs	r0, #0
	b.n	.L_08192882
.L_08192870:
	adds	r1, #1
	cmp	r1, #36
	beq.n	.L_08192882
	ldr	r3, [r4, #0]
	lsls	r2, r1, #1
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	bne.n	.L_08192870
	adds	r0, r1, #0
.L_08192882:
	cmp	r0, #36
	bne.n	.L_0819288a
	movs	r0, #1
	negs	r0, r0
.L_0819288a:
	add	sp, #4
	pop	{r3}
	mov	r9, r3
	pop	{r5, pc}
	.align 2, 0
