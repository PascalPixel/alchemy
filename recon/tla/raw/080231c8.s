.syntax unified
	.thumb
	.set sub_08022e90, 0x08022e90
	.global Func_080231c8
	.thumb_func
Func_080231c8:
	push	{r5, r6, r7, lr}
	adds	r7, r0, #0
	sub	sp, #4
	cmp	r7, #0
	beq.n	.L_0802321a
	adds	r3, r7, #0
	adds	r3, #84
	ldrb	r3, [r3, #0]
	movs	r2, #15
	ands	r2, r3
	cmp	r2, #1
	beq.n	.L_080231e6
	cmp	r2, #2
	beq.n	.L_080231ee
	b.n	.L_08023202
.L_080231e6:
	ldr	r0, [r7, #80]
	bl	sub_08022e90
	b.n	.L_08023202
.L_080231ee:
	ldr	r5, [r7, #80]
	movs	r6, #3
.L_080231f2:
	ldmia	r5!, {r0}
	cmp	r0, #0
	beq.n	.L_080231fc
	bl	sub_08022e90
.L_080231fc:
	subs	r6, #1
	cmp	r6, #0
	bge.n	.L_080231f2
.L_08023202:
	mov	r0, sp
	movs	r3, #0
	str	r3, [r0, #0]
	movs	r2, #133
	movs	r3, #128
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	adds	r1, r7, #0
	adds	r2, #32
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
.L_0802321a:
	add	sp, #4
	pop	{r5, r6, r7, pc}
	.align 2, 0
