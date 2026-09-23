.syntax unified
	.thumb
	.set sub_08016ca4, 0x08016ca4
	.set sub_08020058, 0x08020058
	.set sub_08020060, 0x08020060
	.set sub_08020080, 0x08020080
	.set sub_0811bdb0, 0x0811bdb0
	.global Func_0811b4d8
	.thumb_func
Func_0811b4d8:
	push	{r5, r6, lr}
	adds	r6, r1, #0
	adds	r3, r6, #0
	adds	r3, #43
	ldrb	r3, [r3, #0]
	movs	r5, #0
	cmp	r3, #0
	bne.n	.L_0811b596
	bl	sub_08016ca4
	movs	r1, #50
	adds	r2, r0, #0
	adds	r1, #255
	adds	r3, r2, r1
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #1
	bne.n	.L_0811b500
	movs	r5, #1
.L_0811b500:
	cmp	r3, #2
	bne.n	.L_0811b506
	orrs	r5, r3
.L_0811b506:
	movs	r1, #156
	lsls	r1, r1, #1
	adds	r3, r2, r1
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_0811b516
	movs	r3, #32
	orrs	r5, r3
.L_0811b516:
	movs	r1, #60
	adds	r1, #255
	adds	r3, r2, r1
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_0811b54e
	movs	r3, #4
	adds	r1, #15
	orrs	r5, r3
	adds	r3, r2, r1
	ldrh	r3, [r3, #0]
	adds	r1, #18
	cmp	r3, r1
	beq.n	.L_0811b548
	cmp	r3, r1
	bgt.n	.L_0811b540
	cmp	r3, #97
	bgt.n	.L_0811b54e
	cmp	r3, #91
	blt.n	.L_0811b54e
	b.n	.L_0811b548
.L_0811b540:
	movs	r1, #102
	adds	r1, #255
	cmp	r3, r1
	bne.n	.L_0811b54e
.L_0811b548:
	movs	r3, #5
	negs	r3, r3
	ands	r5, r3
.L_0811b54e:
	movs	r1, #62
	adds	r1, #255
	adds	r3, r2, r1
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_0811b55e
	movs	r3, #8
	orrs	r5, r3
.L_0811b55e:
	movs	r1, #160
	lsls	r1, r1, #1
	adds	r3, r2, r1
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_0811b56e
	movs	r3, #64
	orrs	r5, r3
.L_0811b56e:
	movs	r1, #158
	lsls	r1, r1, #1
	adds	r3, r2, r1
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_0811b57e
	movs	r3, #16
	orrs	r5, r3
.L_0811b57e:
	movs	r3, #66
	adds	r3, #255
	adds	r2, r2, r3
	ldrb	r3, [r2, #0]
	cmp	r3, #0
	beq.n	.L_0811b594
	adds	r2, r3, #0
	adds	r2, #6
	movs	r3, #1
	lsls	r3, r2
	orrs	r5, r3
.L_0811b594:
	strh	r5, [r6, #28]
.L_0811b596:
	pop	{r5, r6, pc}
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r5, r0, #0
	movs	r3, #0
	mov	r8, r3
	movs	r3, #31
	ldrsb	r3, [r5, r3]
	ldrb	r2, [r5, #31]
	cmp	r3, #0
	blt.n	.L_0811b5b2
	subs	r3, r2, #1
	strb	r3, [r5, #31]
.L_0811b5b2:
	movs	r3, #28
	ldrsh	r2, [r5, r3]
	ldrh	r4, [r5, #28]
	cmp	r2, #0
	beq.n	.L_0811b5c4
	ldr	r1, [r5, #32]
	cmp	r1, #0
	beq.n	.L_0811b5e0
	b.n	.L_0811b5c6
.L_0811b5c4:
	ldr	r1, [r5, #32]
.L_0811b5c6:
	cmp	r1, #0
	beq.n	.L_0811b5d8
	ldrb	r3, [r5, #30]
	asrs	r2, r3
	adds	r3, r2, #0
	movs	r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0811b5e0
.L_0811b5d8:
	movs	r3, #31
	ldrsb	r3, [r5, r3]
	cmp	r3, #0
	bne.n	.L_0811b69c
.L_0811b5e0:
	lsls	r3, r4, #16
	movs	r6, #1
	asrs	r3, r3, #16
	negs	r6, r6
	ldr	r0, [r5, #0]
	cmp	r3, #0
	beq.n	.L_0811b61e
	ldrb	r2, [r5, #30]
	mov	ip, r3
	adds	r6, r2, #1
	movs	r4, #1
	b.n	.L_0811b5fa
.L_0811b5f8:
	adds	r6, #1
.L_0811b5fa:
	cmp	r6, #13
	ble.n	.L_0811b600
	movs	r6, #0
.L_0811b600:
	mov	r3, ip
	asrs	r3, r6
	ands	r3, r4
	cmp	r3, #0
	beq.n	.L_0811b5f8
	cmp	r2, r6
	bne.n	.L_0811b612
	cmp	r1, #0
	bne.n	.L_0811b618
.L_0811b612:
	strb	r6, [r5, #30]
	movs	r3, #1
	mov	r8, r3
.L_0811b618:
	movs	r3, #80
	strb	r3, [r5, #31]
	b.n	.L_0811b622
.L_0811b61e:
	movs	r3, #1
	mov	r8, r3
.L_0811b622:
	movs	r1, #0
	bl	sub_0811bdb0
	adds	r7, r0, #0
	cmp	r7, #0
	beq.n	.L_0811b69c
	cmp	r6, #0
	blt.n	.L_0811b646
	ldrb	r3, [r7, #20]
	cmp	r3, #32
	bne.n	.L_0811b640
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r3, #190
	b.n	.L_0811b644
.L_0811b640:
	movs	r3, #172
	lsls	r3, r3, #2
.L_0811b644:
	adds	r6, r6, r3
.L_0811b646:
	ldr	r1, [r5, #32]
	cmp	r1, #0
	beq.n	.L_0811b65c
	mov	r3, r8
	cmp	r3, #0
	beq.n	.L_0811b65c
	adds	r0, r7, #0
	bl	sub_08020060
	movs	r3, #0
	str	r3, [r5, #32]
.L_0811b65c:
	cmp	r6, #0
	blt.n	.L_0811b68c
	mov	r3, r8
	cmp	r3, #0
	beq.n	.L_0811b68c
	adds	r0, r7, #0
	adds	r1, r6, #0
	bl	sub_08020058
	movs	r3, #1
	negs	r3, r3
	str	r0, [r5, #32]
	cmp	r0, r3
	bne.n	.L_0811b67c
	movs	r3, #0
	str	r3, [r5, #32]
.L_0811b67c:
	ldr	r0, [r5, #32]
	cmp	r0, #0
	beq.n	.L_0811b68c
	movs	r3, #3
	strb	r3, [r0, #6]
	movs	r1, #0
	bl	sub_08020080
.L_0811b68c:
	movs	r3, #1
	strb	r3, [r7, #25]
	cmp	r6, #0
	blt.n	.L_0811b698
	strh	r6, [r5, #8]
	b.n	.L_0811b69c
.L_0811b698:
	movs	r3, #0
	strh	r3, [r5, #8]
.L_0811b69c:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.align 2, 0
