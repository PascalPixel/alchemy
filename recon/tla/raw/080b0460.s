.syntax unified
	.thumb
	.set sub_080aec04, 0x080aec04
	.global Func_080b0460
	.thumb_func
Func_080b0460:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r2, #128
	lsls	r2, r2, #2
	sub	sp, #4
	adds	r7, r0, #0
	movs	r6, #0
	movs	r5, #216
	mov	r8, r2
	movs	r1, #14
.L_080b0476:
	ldrh	r3, [r5, r7]
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080b04a2
	ldrh	r0, [r5, r7]
	str	r1, [sp, #0]
	bl	sub_080aec04
	ldr	r1, [sp, #0]
	adds	r0, #24
	movs	r2, #3
.L_080b048e:
	ldrb	r3, [r0, #0]
	cmp	r3, #23
	bne.n	.L_080b049a
	movs	r3, #1
	ldrsb	r3, [r0, r3]
	adds	r6, r6, r3
.L_080b049a:
	subs	r2, #1
	adds	r0, #4
	cmp	r2, #0
	bge.n	.L_080b048e
.L_080b04a2:
	subs	r1, #1
	adds	r5, #2
	cmp	r1, #0
	bge.n	.L_080b0476
	cmp	r6, #0
	bge.n	.L_080b04b0
	movs	r6, #0
.L_080b04b0:
	adds	r0, r6, #0
	add	sp, #4
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
