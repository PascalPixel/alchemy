.syntax unified
	.thumb
	.set sub_08038080, 0x08038080
	.set sub_080380b8, 0x080380b8
	.set sub_080ad010, 0x080ad010
	.set sub_080ad078, 0x080ad078
	.set sub_080ad1c0, 0x080ad1c0
	.set sub_080c8510, 0x080c8510
	.global Func_080fb554
	.thumb_func
Func_080fb554:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r5, r0, #0
	movs	r0, #15
	adds	r6, r1, #0
	bl	sub_080380b8
	movs	r2, #1
	movs	r3, #0
	ldrsb	r3, [r5, r3]
	negs	r2, r2
	mov	r8, r2
	cmp	r3, r8
	bne.n	.L_080fb578
	movs	r0, #14
	bl	sub_080380b8
.L_080fb578:
	ldr	r7, [pc, #184]
	movs	r3, #24
	adds	r0, r7, #0
	adds	r1, r6, #0
	movs	r2, #0
	bl	sub_08038080
	movs	r0, #15
	bl	sub_080380b8
	movs	r3, #1
	ldrsb	r3, [r5, r3]
	cmp	r3, r8
	bne.n	.L_080fb59a
	movs	r0, #14
	bl	sub_080380b8
.L_080fb59a:
	movs	r3, #24
	adds	r0, r7, #1
	adds	r1, r6, #0
	movs	r2, #40
	bl	sub_08038080
	movs	r0, #15
	bl	sub_080380b8
	movs	r3, #3
	ldrsb	r3, [r5, r3]
	cmp	r3, r8
	bne.n	.L_080fb5ba
	movs	r0, #14
	bl	sub_080380b8
.L_080fb5ba:
	movs	r3, #32
	adds	r0, r7, #2
	adds	r1, r6, #0
	movs	r2, #0
	bl	sub_08038080
	movs	r0, #15
	bl	sub_080380b8
	movs	r3, #5
	ldrsb	r3, [r5, r3]
	cmp	r3, r8
	bne.n	.L_080fb5da
	movs	r0, #14
	bl	sub_080380b8
.L_080fb5da:
	movs	r3, #32
	adds	r0, r7, #3
	adds	r1, r6, #0
	movs	r2, #80
	bl	sub_08038080
	movs	r0, #15
	bl	sub_080380b8
	movs	r3, #2
	ldrsb	r3, [r5, r3]
	cmp	r3, r8
	bne.n	.L_080fb5fa
	movs	r0, #14
	bl	sub_080380b8
.L_080fb5fa:
	movs	r3, #24
	adds	r0, r7, #4
	adds	r1, r6, #0
	movs	r2, #80
	bl	sub_08038080
	movs	r0, #15
	bl	sub_080380b8
	movs	r3, #4
	ldrsb	r3, [r5, r3]
	cmp	r3, r8
	bne.n	.L_080fb61a
	movs	r0, #14
	bl	sub_080380b8
.L_080fb61a:
	adds	r0, r7, #5
	adds	r1, r6, #0
	movs	r2, #40
	movs	r3, #32
	bl	sub_08038080
	movs	r0, #15
	bl	sub_080380b8
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0x1063
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r3, #128
	lsls	r3, r3, #1
	adds	r6, r1, #0
	adds	r3, #255
	ands	r6, r3
	mov	sl, r0
	adds	r0, r6, #0
	bl	sub_080ad010
	movs	r7, #1
	adds	r5, r0, #0
	adds	r0, r6, #0
	negs	r7, r7
	bl	sub_080c8510
	cmp	r0, #0
	beq.n	.L_080fb666
	movs	r0, #0
	b.n	.L_080fb6ca
.L_080fb666:
	ldrh	r3, [r5, #40]
	movs	r0, #252
	lsls	r0, r0, #6
	adds	r0, #255
	ands	r0, r3
	bl	sub_080ad078
	ldrh	r3, [r5, #40]
	mov	r8, r0
	cmp	r3, #0
	beq.n	.L_080fb6c8
	ldrb	r3, [r5, #2]
	cmp	r3, #0
	beq.n	.L_080fb694
	ldrb	r3, [r5, #12]
	cmp	r3, #3
	beq.n	.L_080fb696
	mov	r0, sl
	adds	r1, r6, #0
	bl	sub_080ad1c0
	cmp	r0, #0
	beq.n	.L_080fb696
.L_080fb694:
	movs	r7, #1
.L_080fb696:
	cmp	r7, #1
	bne.n	.L_080fb6c8
	mov	r3, r8
	ldrb	r2, [r3, #1]
	movs	r3, #64
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080fb6b8
	mov	r3, r8
	ldrb	r2, [r3, #8]
	movs	r3, #255
	eors	r2, r3
	negs	r3, r2
	orrs	r3, r2
	lsrs	r7, r3, #31
	movs	r3, #2
	b.n	.L_080fb6c6
.L_080fb6b8:
	movs	r3, #128
	ands	r3, r2
	negs	r2, r3
	orrs	r2, r3
	lsrs	r2, r2, #31
	adds	r7, r2, #0
	movs	r3, #0
.L_080fb6c6:
	subs	r7, r3, r7
.L_080fb6c8:
	adds	r0, r7, #0
.L_080fb6ca:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.align 2, 0
