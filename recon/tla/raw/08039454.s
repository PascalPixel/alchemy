.syntax unified
	.thumb
	.set sub_08041abc, 0x08041abc
	.global Func_08039454
	.thumb_func
Func_08039454:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r7, r2, #0
	movs	r2, #192
	lsls	r2, r2, #18
	sub	sp, #4
	ldr	r2, [r2, #60]
	adds	r5, r1, #0
	ldr	r1, [sp, #24]
	mov	r8, r2
	lsrs	r4, r5, #3
	movs	r2, #12
	ldrsh	r5, [r0, r2]
	movs	r6, #14
	ldrsh	r0, [r0, r6]
	adds	r3, #7
	adds	r1, #7
	lsrs	r2, r7, #3
	lsrs	r3, r3, #3
	lsrs	r1, r1, #3
	adds	r2, r2, r0
	adds	r4, r4, r5
	adds	r3, r3, r5
	adds	r1, r1, r0
	adds	r5, r4, #1
	adds	r7, r2, #1
	subs	r6, r3, r4
	subs	r4, r1, r2
	adds	r3, r4, #0
	adds	r1, r7, #0
	adds	r2, r6, #0
	adds	r0, r5, #0
	str	r4, [sp, #0]
	bl	sub_08041abc
	lsls	r3, r7, #6
	add	r3, r8
	lsls	r2, r5, #1
	ldr	r4, [sp, #0]
	adds	r3, r3, r2
	adds	r0, r3, #0
	movs	r1, #0
	adds	r0, #8
	cmp	r1, r4
	bcs.n	.L_080394d8
	movs	r3, #32
	subs	r3, r3, r6
	lsls	r3, r3, #1
.L_080394b6:
	movs	r5, #0
	cmp	r5, r6
	bcs.n	.L_080394c8
	ldr	r2, [pc, #20]
.L_080394be:
	adds	r5, #1
	strh	r2, [r0, #0]
	adds	r0, #2
	cmp	r5, r6
	bcc.n	.L_080394be
.L_080394c8:
	adds	r1, #1
	adds	r0, r0, r3
	cmp	r1, r4
	bcc.n	.L_080394b6
	b.n	.L_080394d8
	movs	r0, r0
	.2byte 0xf020
	.2byte 0x0000
.L_080394d8:
	movs	r3, #1
	mov	r2, r8
	strb	r3, [r2, #3]
	add	sp, #4
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.align 2, 0
