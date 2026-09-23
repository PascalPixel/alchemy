.syntax unified
	.thumb
	.set sub_08013164, 0x08013164
	.set sub_08014d78, 0x08014d78
	.set sub_080ad0f0, 0x080ad0f0
	.set sub_081180c8, 0x081180c8
	.global Func_0804257c
	.thumb_func
Func_0804257c:
	push	{lr}
	cmp	r0, #0
	beq.n	.L_08042586
	mvns	r3, r1
	strb	r3, [r0, #15]
.L_08042586:
	pop	{pc}
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r7, [r3, #60]
	sub	sp, #4
	mov	r8, r0
	cmp	r7, #0
	bne.n	.L_080425bc
	ldr	r3, [pc, #24]
	mov	r0, sp
	adds	r0, #2
	strh	r3, [r0, #0]
	movs	r2, #129
	movs	r3, #128
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	mov	r1, r8
	adds	r2, #160
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	b.n	.L_080425ec
	.2byte 0xe0e0
	.2byte 0x0000
.L_080425bc:
	ldr	r5, [pc, #52]
	adds	r0, r5, #0
	bl	sub_08014d78
	movs	r2, #132
	movs	r3, #128
	adds	r6, r0, #0
	lsrs	r5, r5, #2
	lsls	r2, r2, #24
	lsls	r3, r3, #19
	adds	r3, #212
	ldr	r0, [pc, #36]
	adds	r1, r6, #0
	orrs	r2, r5
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r1, r7, #0
	adds	r1, #8
	mov	r0, r8
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x1c30
	bl	sub_08013164
.L_080425ec:
	add	sp, #4
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.4byte 0x00000218
	.2byte 0x88fc
	.2byte 0x0803
	push	{r5, r6, lr}
	adds	r5, r3, #0
	movs	r4, #0
	ldr	r6, [sp, #12]
	ldr	r0, [pc, #36]
	cmp	r4, r5
	bcs.n	.L_08042628
	movs	r3, #32
	subs	r3, r3, r2
	lsls	r3, r3, #1
.L_08042610:
	movs	r1, #0
	cmp	r1, r2
	bcs.n	.L_08042620
.L_08042616:
	adds	r1, #1
	strh	r6, [r0, #0]
	adds	r0, #2
	cmp	r1, r2
	bcc.n	.L_08042616
.L_08042620:
	adds	r4, #1
	adds	r0, r0, r3
	cmp	r4, r5
	bcc.n	.L_08042610
.L_08042628:
	pop	{r5, r6, pc}
	movs	r0, r0
	.2byte 0x2000
	.2byte 0x0600
	push	{r5, r6, r7, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #64]
	ldr	r3, [r3, #60]
	adds	r6, r0, #0
	ldrb	r3, [r3, #5]
	movs	r7, #4
	cmp	r3, #0
	beq.n	.L_0804264e
	movs	r0, #0
	bl	sub_081180c8
	movs	r7, #3
	b.n	.L_08042658
.L_0804264e:
	bl	sub_080ad0f0
	cmp	r0, #4
	bls.n	.L_08042658
	movs	r0, #4
.L_08042658:
	movs	r3, #1
	ands	r3, r6
	cmp	r3, #0
	beq.n	.L_08042664
	adds	r7, #1
	b.n	.L_0804266a
.L_08042664:
	movs	r3, #3
	negs	r3, r3
	ands	r6, r3
.L_0804266a:
	lsls	r3, r0, #1
	adds	r3, r3, r0
	lsls	r0, r3, #1
	movs	r3, #2
	ands	r3, r6
	adds	r1, r0, #1
	cmp	r3, #0
	beq.n	.L_0804267c
	adds	r1, r0, #6
.L_0804267c:
	movs	r3, #30
	subs	r3, r3, r1
	movs	r2, #0
	strh	r3, [r5, #4]
	strh	r2, [r5, #6]
	strh	r1, [r5, #8]
	strh	r7, [r5, #10]
	strh	r6, [r5, #12]
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
