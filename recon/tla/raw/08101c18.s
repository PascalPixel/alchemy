.syntax unified
	.thumb
	.set sub_080383a8, 0x080383a8
	.global Func_08101c18
	.thumb_func
Func_08101c18:
.L_08101c18:
	push	{r5, r6, r7, lr}
	adds	r4, r0, #0
	movs	r7, #12
	ldrsh	r0, [r4, r7]
	sub	sp, #4
	adds	r0, r0, r1
	movs	r7, #14
	ldrsh	r1, [r4, r7]
	ldr	r5, [sp, #24]
	adds	r6, r3, #0
	adds	r1, r1, r2
	ldr	r3, [sp, #20]
	adds	r0, #1
	adds	r1, #1
	adds	r2, r6, #0
	str	r5, [sp, #0]
	bl	sub_080383a8
	add	sp, #4
	pop	{r5, r6, r7, pc}
	push	{r5, lr}
	adds	r5, r0, #0
	adds	r0, r2, #0
	negs	r2, r3
	orrs	r2, r3
	lsrs	r4, r2, #31
	movs	r3, #15
	subs	r4, r3, r4
	sub	sp, #8
	adds	r3, r1, #0
	cmp	r1, #0
	bge.n	.L_08101c5a
	adds	r3, r1, #3
.L_08101c5a:
	asrs	r3, r3, #2
	lsls	r3, r3, #2
	subs	r3, r1, r3
	lsls	r1, r3, #3
	subs	r1, r1, r3
	movs	r3, #1
	adds	r2, r0, #2
	str	r3, [sp, #0]
	adds	r1, #1
	adds	r0, r5, #0
	movs	r3, #6
	str	r4, [sp, #4]
	bl	.L_08101c18
	add	sp, #8
	pop	{r5, pc}
	.align 2, 0
