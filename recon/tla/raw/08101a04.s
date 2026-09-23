.syntax unified
	.thumb
	.set sub_080ad1b0, 0x080ad1b0
	.set sub_080ad1b8, 0x080ad1b8
	.global Func_08101a04
	.thumb_func
Func_08101a04:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r3, #240
	adds	r5, r0, #0
	lsls	r3, r3, #4
	ands	r3, r5
	lsrs	r3, r3, #8
	mov	r8, r3
	movs	r3, #224
	ands	r3, r5
	movs	r6, #31
	lsrs	r7, r3, #5
	ands	r6, r5
	mov	r0, r8
	adds	r1, r7, #0
	adds	r2, r6, #0
	bl	sub_080ad1b8
	cmp	r0, #0
	bne.n	.L_08101a3c
	mov	r0, r8
	adds	r1, r7, #0
	adds	r2, r6, #0
	bl	sub_080ad1b0
	cmp	r0, #0
	beq.n	.L_08101a4a
.L_08101a3c:
	movs	r2, #128
	lsls	r2, r2, #8
	ands	r2, r5
	negs	r3, r2
	orrs	r3, r2
	lsrs	r0, r3, #31
	b.n	.L_08101a4c
.L_08101a4a:
	movs	r0, #2
.L_08101a4c:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.align 2, 0
