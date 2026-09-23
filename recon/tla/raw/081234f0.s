.syntax unified
	.thumb
	.set sub_08013560, 0x08013560
	.set sub_08014644, 0x08014644
	.set sub_080ad000, 0x080ad000
	.set sub_0811a31c, 0x0811a31c
	.set sub_081234a4, 0x081234a4
	.global Func_081234f0
	.thumb_func
Func_081234f0:
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #36]
	movs	r3, #128
	lsls	r3, r3, #4
	adds	r3, #44
	adds	r2, r1, r3
	ldr	r3, [r2, #0]
	cmp	r3, #0
	bne.n	.L_0812350a
	movs	r3, #1
	str	r3, [r2, #0]
.L_0812350a:
	cmp	r3, #4
	beq.n	.L_08123522
	movs	r3, #128
	lsls	r3, r3, #4
	adds	r3, #44
	adds	r5, r1, r3
.L_08123516:
	movs	r0, #1
	bl	sub_08013560
	ldr	r3, [r5, #0]
	cmp	r3, #4
	bne.n	.L_08123516
.L_08123522:
	ldr	r0, [pc, #12]
	bl	sub_08014644
	bl	sub_081234a4
	pop	{r5, pc}
	movs	r0, r0
	.2byte 0x2d11
	.2byte 0x0812
	push	{r5, r6, lr}
	adds	r6, r0, #0
	sub	sp, #16
	movs	r0, #1
	cmp	r6, #7
	bls.n	.L_08123542
	movs	r0, #2
.L_08123542:
	mov	r5, sp
	adds	r1, r5, #0
	bl	sub_0811a31c
	movs	r2, #0
	cmp	r2, r0
	bge.n	.L_08123566
	ldrh	r3, [r5, #0]
	cmp	r3, r6
	beq.n	.L_08123566
	adds	r1, r5, #0
.L_08123558:
	adds	r2, #1
	cmp	r2, r0
	bge.n	.L_08123566
	adds	r1, #2
	ldrh	r3, [r1, #0]
	cmp	r3, r6
	bne.n	.L_08123558
.L_08123566:
	adds	r3, r2, #0
	eors	r3, r0
	negs	r0, r3
	orrs	r0, r3
	lsrs	r0, r0, #31
	add	sp, #16
	pop	{r5, r6, pc}
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	adds	r5, r0, #0
	adds	r6, r1, #0
	movs	r1, #0
	sub	sp, #20
	mov	fp, r1
	movs	r0, #1
	cmp	r5, #7
	bls.n	.L_08123594
	movs	r0, #2
.L_08123594:
	add	r2, sp, #4
	mov	r9, r2
	mov	r1, r9
	bl	sub_0811a31c
	adds	r7, r0, #0
	movs	r0, #0
	cmp	r5, #7
	bls.n	.L_081235a8
	movs	r0, #1
.L_081235a8:
	bl	sub_080ad000
	adds	r0, #8
	mov	r8, r0
	cmp	r6, #0
	beq.n	.L_081235c2
	movs	r2, #0
	adds	r3, r6, #3
	mov	ip, r6
.L_081235ba:
	strb	r2, [r3, #0]
	subs	r3, #1
	cmp	r3, ip
	bge.n	.L_081235ba
.L_081235c2:
	movs	r2, #144
	lsls	r2, r2, #1
	movs	r3, #0
	add	r2, r8
	mov	sl, r3
	ldr	r3, [r2, #0]
	cmp	r3, #0
	beq.n	.L_08123636
	str	r2, [sp, #0]
	movs	r1, #0
	mov	ip, r9
	mov	r5, r8
	mov	lr, r1
.L_081235dc:
	movs	r3, #3
	ldrsb	r3, [r5, r3]
	movs	r2, #1
	negs	r2, r2
	cmp	r3, r2
	bne.n	.L_08123624
	movs	r4, #0
	cmp	r4, r7
	bge.n	.L_0812360c
	mov	r3, ip
	ldrh	r2, [r3, #0]
	ldrb	r3, [r5, #2]
	cmp	r2, r3
	beq.n	.L_0812360c
	adds	r1, r5, #0
	mov	r0, r9
.L_081235fc:
	adds	r4, #1
	cmp	r4, r7
	bge.n	.L_0812360c
	adds	r0, #2
	ldrh	r2, [r0, #0]
	ldrb	r3, [r1, #2]
	cmp	r2, r3
	bne.n	.L_081235fc
.L_0812360c:
	cmp	r4, r7
	beq.n	.L_08123624
	cmp	r6, #0
	beq.n	.L_08123620
	mov	r3, r8
	mov	r1, lr
	ldrb	r2, [r1, r3]
	ldrb	r3, [r6, r2]
	adds	r3, #1
	strb	r3, [r6, r2]
.L_08123620:
	movs	r1, #1
	add	fp, r1
.L_08123624:
	ldr	r1, [sp, #0]
	movs	r3, #1
	add	sl, r3
	ldr	r3, [r1, #0]
	movs	r2, #4
	adds	r5, #4
	add	lr, r2
	cmp	sl, r3
	bne.n	.L_081235dc
.L_08123636:
	mov	r0, fp
	add	sp, #20
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.align 2, 0
