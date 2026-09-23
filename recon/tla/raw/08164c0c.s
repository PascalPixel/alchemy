.syntax unified
	.thumb
	.global Func_08164c0c
	.thumb_func
Func_08164c0c:
	push	{r5, r6, r7, lr}
	mov	r7, r9
	push	{r7}
	sub	sp, #4
	mov	r4, r9
	mov	r3, sp
	str	r4, [r3, #0]
	adds	r3, r4, #0
	adds	r5, r1, #0
	adds	r1, r3, #0
	subs	r1, #132
	ldr	r3, [r1, #0]
	movs	r7, #224
	lsls	r7, r7, #2
	adds	r6, r2, #0
	adds	r2, r3, r7
	ldr	r3, [r2, #24]
	movs	r7, #1
	negs	r7, r7
	movs	r4, #0
	cmp	r3, r7
	bne.n	.L_08164c3c
	str	r4, [r2, #24]
	b.n	.L_08164c60
.L_08164c3c:
	adds	r4, #1
	cmp	r4, #16
	beq.n	.L_08164c66
	lsls	r3, r4, #3
	ldr	r2, [r1, #0]
	subs	r3, r3, r4
	lsls	r3, r3, #2
	adds	r2, r2, r3
	movs	r3, #224
	lsls	r3, r3, #2
	adds	r2, r2, r3
	ldr	r3, [r2, #24]
	movs	r7, #1
	negs	r7, r7
	cmp	r3, r7
	bne.n	.L_08164c3c
	movs	r3, #0
	str	r3, [r2, #24]
.L_08164c60:
	str	r0, [r2, #0]
	str	r5, [r2, #4]
	str	r6, [r2, #12]
.L_08164c66:
	add	sp, #4
	pop	{r3}
	mov	r9, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
