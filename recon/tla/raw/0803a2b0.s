.syntax unified
	.thumb
	.global Func_0803a2b0
	.thumb_func
Func_0803a2b0:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	mov	lr, r3
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #60]
	lsls	r1, r1, #6
	sub	sp, #4
	adds	r6, r2, #0
	adds	r1, r3, r1
	lsls	r0, r0, #1
	ldr	r2, [sp, #36]
	str	r3, [sp, #0]
	adds	r1, r1, r0
	cmp	r6, #1
	bls.n	.L_0803a3aa
	mov	r3, lr
	cmp	r3, #1
	bls.n	.L_0803a3aa
	cmp	r6, #30
	bhi.n	.L_0803a3aa
	cmp	r3, #30
	bhi.n	.L_0803a3aa
	adds	r1, #72
	cmp	r2, #0
	bne.n	.L_0803a34c
	movs	r2, #1
	negs	r2, r2
	add	r2, lr
	movs	r5, #1
	mov	r8, r2
	cmp	r5, r8
	bcs.n	.L_0803a3a4
	subs	r3, r6, #1
	mov	ip, r3
	movs	r3, #32
	subs	r3, r3, r6
	movs	r2, #40
	lsls	r3, r3, #1
	adds	r2, #255
	mov	r9, ip
	mov	sl, r3
	mov	fp, r2
.L_0803a310:
	movs	r4, #1
	adds	r1, #2
	cmp	r4, r9
	bcs.n	.L_0803a338
	mov	r3, lr
	subs	r3, #2
	ldr	r7, [pc, #36]
	ldr	r6, [pc, #40]
	adds	r2, r3, #0
	mov	r3, fp
	adds	r0, r3, r5
.L_0803a326:
	adds	r3, r0, #0
	ands	r3, r7
	orrs	r3, r6
	adds	r4, #1
	strh	r3, [r1, #0]
	adds	r0, r0, r2
	adds	r1, #2
	cmp	r4, ip
	bcc.n	.L_0803a326
.L_0803a338:
	adds	r1, #2
	adds	r5, #1
	add	r1, sl
	cmp	r5, r8
	bcc.n	.L_0803a310
	b.n	.L_0803a3a4
	.4byte 0x00000fff
	.2byte 0xf000
	.2byte 0x0000
.L_0803a34c:
	mov	r2, lr
	movs	r5, #1
	subs	r2, #1
	cmp	r5, r2
	bcs.n	.L_0803a3a4
	movs	r3, #32
	subs	r3, r3, r6
	lsls	r3, r3, #1
	mov	sl, r3
	movs	r3, #40
	adds	r3, #255
	mov	r9, r3
	mov	r8, r2
.L_0803a366:
	movs	r4, #0
	cmp	r4, r6
	bcs.n	.L_0803a396
	movs	r2, #2
	negs	r2, r2
	movs	r3, #240
	add	r2, lr
	lsls	r3, r3, #4
	ldr	r7, [pc, #40]
	mov	fp, r2
	adds	r3, #255
	mov	r2, r9
	mov	ip, r3
	adds	r0, r5, r2
.L_0803a382:
	adds	r3, r0, #0
	mov	r2, ip
	ands	r3, r2
	orrs	r3, r7
	adds	r4, #1
	strh	r3, [r1, #0]
	add	r0, fp
	adds	r1, #2
	cmp	r4, r6
	bcc.n	.L_0803a382
.L_0803a396:
	adds	r5, #1
	add	r1, sl
	cmp	r5, r8
	bcc.n	.L_0803a366
	b.n	.L_0803a3a4
	.2byte 0xf000
	.2byte 0x0000
.L_0803a3a4:
	ldr	r2, [sp, #0]
	movs	r3, #1
	strb	r3, [r2, #3]
.L_0803a3aa:
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #60]
	movs	r1, #215
	lsls	r1, r1, #3
	adds	r2, r3, r1
	movs	r1, #0
.L_0803a3c8:
	ldr	r3, [r2, #0]
	cmp	r3, #0
	beq.n	.L_0803a3d6
	ldrh	r3, [r3, #20]
	movs	r0, #0
	cmp	r3, #0
	beq.n	.L_0803a3e0
.L_0803a3d6:
	adds	r1, #1
	adds	r2, #40
	cmp	r1, #3
	bne.n	.L_0803a3c8
	movs	r0, #1
.L_0803a3e0:
	pop	{pc}
	.align 2, 0
