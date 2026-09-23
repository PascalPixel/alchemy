.syntax unified
	.thumb
	.set sub_0800206c, 0x0800206c
	.set sub_08002090, 0x08002090
	.set sub_08002096, 0x08002096
	.set sub_0801314c, 0x0801314c
	.set sub_0801401c, 0x0801401c
	.set sub_08014128, 0x08014128
	.set sub_080142d4, 0x080142d4
	.set sub_08014878, 0x08014878
	.set sub_08014cc0, 0x08014cc0
	.set sub_0803b094, 0x0803b094
	.set sub_0803d178, 0x0803d178
	.global Func_0803c40c
	.thumb_func
Func_0803c40c:
.L_0803c40c:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r6, r0, #0
	ldrb	r3, [r6, #5]
	movs	r7, #128
	adds	r5, r6, #0
	sub	sp, #8
	lsls	r7, r7, #1
	adds	r5, #16
	cmp	r3, #9
	beq.n	.L_0803c436
	cmp	r3, #9
	blt.n	.L_0803c486
	cmp	r3, #10
	beq.n	.L_0803c448
	cmp	r3, #11
	beq.n	.L_0803c45a
	cmp	r3, #12
	beq.n	.L_0803c470
	b.n	.L_0803c486
.L_0803c436:
	ldrh	r2, [r6, #12]
	ldr	r1, [pc, #204]
	adds	r3, r2, #1
	strh	r3, [r6, #12]
	movs	r3, #31
	ands	r3, r2
	lsls	r3, r3, #1
	ldrh	r7, [r1, r3]
	b.n	.L_0803c486
.L_0803c448:
	ldrh	r2, [r6, #12]
	ldr	r1, [pc, #188]
	adds	r3, r2, #1
	strh	r3, [r6, #12]
	movs	r3, #31
	ands	r3, r2
	lsls	r3, r3, #1
	ldrh	r3, [r1, r3]
	b.n	.L_0803c484
.L_0803c45a:
	ldrh	r3, [r6, #12]
	adds	r0, r3, #0
	cmp	r0, #7
	bhi.n	.L_0803c486
	adds	r3, #1
	ldr	r2, [pc, #160]
	strh	r3, [r6, #12]
	lsls	r3, r0, #2
	adds	r3, #32
	ldrh	r7, [r2, r3]
	b.n	.L_0803c486
.L_0803c470:
	ldrh	r3, [r6, #12]
	adds	r1, r3, #0
	cmp	r1, #7
	bhi.n	.L_0803c486
	adds	r3, #1
	ldr	r2, [pc, #140]
	strh	r3, [r6, #12]
	lsls	r3, r1, #2
	adds	r3, #32
	ldrh	r3, [r2, r3]
.L_0803c484:
	lsrs	r7, r3, #1
.L_0803c486:
	movs	r3, #128
	lsls	r3, r3, #1
	mov	r8, r3
	cmp	r7, r8
	bne.n	.L_0803c4a4
	ldrb	r2, [r5, #7]
	movs	r3, #63
	negs	r3, r3
	ands	r3, r2
	ldrb	r2, [r5, #5]
	strb	r3, [r5, #7]
	movs	r3, #4
	negs	r3, r3
	ands	r3, r2
	b.n	.L_0803c520
.L_0803c4a4:
	ldr	r3, [sp, #0]
	ldr	r4, [pc, #100]
	movs	r2, #255
	adds	r1, r7, #0
	ands	r3, r4
	lsls	r2, r2, #8
	adds	r2, #255
	orrs	r3, r1
	ands	r3, r2
	lsls	r1, r1, #16
	orrs	r3, r1
	str	r3, [sp, #0]
	mov	r0, sp
	ldr	r3, [r0, #4]
	ands	r3, r4
	str	r3, [r0, #4]
	bl	sub_0801401c
	movs	r3, #31
	ldrb	r2, [r5, #7]
	ands	r0, r3
	movs	r3, #63
	negs	r3, r3
	lsls	r0, r0, #1
	ands	r3, r2
	orrs	r3, r0
	strb	r3, [r5, #7]
	cmp	r7, r8
	ble.n	.L_0803c514
	ldrb	r3, [r5, #5]
	movs	r2, #3
	orrs	r3, r2
	strb	r3, [r5, #5]
	movs	r3, #255
	ldrh	r2, [r6, #6]
	lsls	r3, r3, #8
	adds	r3, #248
	adds	r2, r2, r3
	ldr	r3, [pc, #16]
	ldrh	r1, [r5, #6]
	ands	r2, r3
	ldr	r3, [pc, #24]
	ands	r3, r1
	orrs	r3, r2
	strh	r3, [r5, #6]
	ldrb	r3, [r6, #8]
	adds	r3, #248
	b.n	.L_0803c538
	.4byte 0x000001ff
	.4byte 0x0805e9c4
	.4byte 0xffff0000
	.2byte 0xfe00
	.2byte 0xffff
.L_0803c514:
	.2byte 0x796a
	movs	r3, #4
	negs	r3, r3
	ands	r3, r2
	movs	r2, #1
	orrs	r3, r2
.L_0803c520:
	strb	r3, [r5, #5]
	movs	r2, #128
	ldrh	r3, [r6, #6]
	lsls	r2, r2, #1
	adds	r2, #255
	ands	r2, r3
	ldrh	r1, [r5, #6]
	ldr	r3, [pc, #20]
	ands	r3, r1
	orrs	r3, r2
	strh	r3, [r5, #6]
	ldrh	r3, [r6, #8]
.L_0803c538:
	strb	r3, [r5, #4]
	add	sp, #8
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0xfe00
	.2byte 0xffff
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #60]
	movs	r1, #161
	mov	r9, r3
	lsls	r1, r1, #3
	add	r1, r9
	movs	r2, #0
	sub	sp, #24
	mov	sl, r1
	mov	fp, r2
.L_0803c56c:
	mov	r3, sl
	ldrh	r2, [r3, #22]
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_0803c57a
	b.n	.L_0803c988
.L_0803c57a:
	mov	r1, sl
	ldr	r6, [r1, #0]
	ldr	r1, [pc, #92]
	b.n	.L_0803c97a
.L_0803c582:
	ldrb	r3, [r6, #5]
	adds	r7, r6, #0
	subs	r3, #2
	adds	r7, #16
	cmp	r3, #16
	bls.n	.L_0803c590
	b.n	.L_0803c922
.L_0803c590:
	ldr	r2, [pc, #76]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x0803c5e4
	.4byte 0x0803c922
	.4byte 0x0803c810
	.4byte 0x0803c67c
	.4byte 0x0803c6e8
	.4byte 0x0803c792
	.4byte 0x0803c922
	.4byte 0x0803c91c
	.4byte 0x0803c91c
	.4byte 0x0803c91c
	.4byte 0x0803c91c
	.4byte 0x0803c922
	.4byte 0x0803c8b2
	.4byte 0x0803c886
	.4byte 0x0803c89c
	.4byte 0x0803c870
	.4byte 0x0803c8e0
	.4byte 0x03001100
	.2byte 0xc598
	.2byte 0x0803
	movs	r1, #152
	lsls	r1, r1, #5
	adds	r1, #70
	add	r1, r9
	ldrh	r3, [r1, #0]
	cmp	r3, #96
	bne.n	.L_0803c5f4
	b.n	.L_0803c922
.L_0803c5f4:
	ldr	r3, [pc, #76]
	lsls	r2, r0, #7
	adds	r2, r2, r3
	ldrh	r0, [r1, #0]
	movs	r1, #128
	bl	sub_080142d4
	ldr	r3, [pc, #60]
	ldrh	r2, [r7, #8]
	ands	r0, r3
	ldr	r3, [pc, #60]
	movs	r5, #13
	ands	r3, r2
	orrs	r3, r0
	strh	r3, [r7, #8]
	strb	r3, [r6, #14]
	ldrb	r3, [r7, #5]
	negs	r5, r5
	ands	r5, r3
	movs	r3, #17
	negs	r3, r3
	ldrb	r2, [r7, #7]
	ands	r5, r3
	movs	r3, #32
	orrs	r5, r3
	movs	r3, #63
	adds	r4, r3, #0
	ands	r5, r3
	movs	r3, #128
	ands	r4, r2
	orrs	r5, r3
	strb	r4, [r7, #7]
	strb	r5, [r7, #5]
	ldr	r3, [pc, #20]
	ldrb	r2, [r6, #8]
	ldr	r0, [r3, #0]
	b.n	.L_0803c650
	movs	r0, r0
	.4byte 0x000003ff
	.4byte 0x0805ec84
	.4byte 0xfffffc00
	.2byte 0x1100
	.2byte 0x0300
.L_0803c650:
	mov	r8, r2
	ldr	r2, [pc, #140]
	movs	r1, #80
	str	r2, [sp, #4]
	str	r4, [sp, #0]
	bl	sub_0800206c
	ldr	r2, [sp, #4]
	mov	r1, r8
	ldrb	r3, [r2, r0]
	ldr	r4, [sp, #0]
	adds	r3, r1, r3
	adds	r3, #2
	strb	r3, [r7, #4]
	movs	r3, #4
	negs	r3, r3
	ands	r5, r3
	subs	r3, #59
	ands	r3, r4
	strb	r5, [r7, #5]
	strb	r3, [r7, #7]
	b.n	.L_0803c922
	ldr	r3, [r1, #0]
	movs	r2, #1
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_0803c688
	b.n	.L_0803c922
.L_0803c688:
	bl	sub_08014878
	adds	r5, r0, #0
	bl	sub_08014878
	lsls	r2, r5, #1
	lsls	r3, r0, #1
	adds	r2, r2, r5
	adds	r3, r3, r0
	ldrh	r1, [r6, #6]
	lsrs	r3, r3, #16
	lsrs	r2, r2, #16
	adds	r2, r2, r3
	lsrs	r2, r2, #1
	ldr	r3, [pc, #52]
	adds	r1, r1, r2
	subs	r1, #1
	ands	r1, r3
	ldrh	r2, [r7, #6]
	ldr	r3, [pc, #52]
	ands	r3, r2
	orrs	r3, r1
	strh	r3, [r7, #6]
	bl	sub_08014878
	adds	r5, r0, #0
	bl	sub_08014878
	lsls	r2, r5, #1
	lsls	r3, r0, #1
	adds	r2, r2, r5
	adds	r3, r3, r0
	ldrb	r1, [r6, #8]
	lsrs	r2, r2, #16
	lsrs	r3, r3, #16
	adds	r2, r2, r3
	lsrs	r2, r2, #1
	adds	r1, r1, r2
	subs	r1, #1
	strb	r1, [r7, #4]
	b.n	.L_0803c922
	movs	r0, r0
	.4byte 0x000001ff
	.4byte 0x0805c12c
	.2byte 0xfe00
	.2byte 0xffff
	ldrh	r3, [r6, #12]
	cmp	r3, #0
	beq.n	.L_0803c764
	ldr	r1, [pc, #108]
	ldr	r3, [sp, #16]
	movs	r2, #128
	movs	r5, #255
	ands	r3, r1
	lsls	r2, r2, #2
	lsls	r5, r5, #8
	orrs	r3, r2
	adds	r5, #255
	movs	r2, #128
	lsls	r2, r2, #18
	ands	r3, r5
	orrs	r3, r2
	str	r3, [sp, #16]
	add	r0, sp, #16
	ldr	r3, [r0, #4]
	ands	r3, r1
	str	r3, [r0, #4]
	bl	sub_0801401c
	ldrb	r2, [r7, #7]
	movs	r3, #31
	ands	r0, r3
	movs	r3, #63
	negs	r3, r3
	ands	r3, r2
	lsls	r0, r0, #1
	orrs	r3, r0
	strb	r3, [r7, #7]
	ldrb	r3, [r7, #5]
	movs	r2, #3
	orrs	r3, r2
	strb	r3, [r7, #5]
	movs	r3, #255
	ldrh	r2, [r6, #6]
	lsls	r3, r3, #8
	adds	r3, #251
	adds	r2, r2, r3
	ldr	r3, [pc, #28]
	ldrh	r1, [r7, #6]
	ands	r2, r3
	ldr	r3, [pc, #28]
	ands	r3, r1
	orrs	r3, r2
	strh	r3, [r7, #6]
	ldrb	r3, [r6, #8]
	adds	r3, #251
	strb	r3, [r7, #4]
	ldrh	r3, [r6, #12]
	adds	r3, r3, r5
	strh	r3, [r6, #12]
	b.n	.L_0803c922
	movs	r0, r0
	.4byte 0x000001ff
	.4byte 0xffff0000
	.2byte 0xfe00
	.2byte 0xffff
.L_0803c764:
	.2byte 0x79fa
	movs	r3, #63
	negs	r3, r3
	ands	r3, r2
	ldrb	r2, [r7, #5]
	strb	r3, [r7, #7]
	movs	r3, #4
	negs	r3, r3
	ands	r3, r2
	strb	r3, [r7, #5]
	movs	r2, #128
	ldrh	r3, [r6, #6]
	lsls	r2, r2, #1
	adds	r2, #255
	ands	r2, r3
	ldrh	r1, [r7, #6]
	ldr	r3, [pc, #132]
	ands	r3, r1
	orrs	r3, r2
	strh	r3, [r7, #6]
	ldrh	r3, [r6, #8]
	strb	r3, [r7, #4]
	b.n	.L_0803c922
	movs	r3, #128
	add	r5, sp, #16
	lsls	r3, r3, #1
	strh	r3, [r5, #0]
	strh	r3, [r5, #2]
	ldrh	r3, [r6, #12]
	movs	r1, #192
	lsls	r1, r1, #2
	adds	r3, r3, r1
	strh	r3, [r6, #12]
	strh	r3, [r5, #4]
	adds	r0, r5, #0
	bl	sub_0801401c
	ldrb	r2, [r7, #7]
	movs	r3, #31
	ands	r0, r3
	movs	r3, #63
	negs	r3, r3
	lsls	r0, r0, #1
	ands	r3, r2
	orrs	r3, r0
	ldrb	r2, [r7, #5]
	strb	r3, [r7, #7]
	movs	r3, #4
	negs	r3, r3
	ands	r3, r2
	ldrh	r0, [r5, #4]
	movs	r2, #1
	orrs	r3, r2
	movs	r2, #232
	lsls	r2, r2, #8
	strb	r3, [r7, #5]
	adds	r0, r0, r2
	bl	sub_08002096
	ldrh	r2, [r6, #6]
	asrs	r0, r0, #14
	ldr	r3, [pc, #40]
	subs	r2, r2, r0
	subs	r2, #2
	ands	r2, r3
	ldrh	r1, [r7, #6]
	ldr	r3, [pc, #32]
	ldrh	r0, [r5, #4]
	ands	r3, r1
	orrs	r3, r2
	strh	r3, [r7, #6]
	movs	r3, #208
	lsls	r3, r3, #7
	adds	r0, r0, r3
	bl	sub_08002090
	ldrb	r3, [r6, #8]
	asrs	r0, r0, #14
	subs	r3, r3, r0
	subs	r3, #2
	strb	r3, [r7, #4]
	b.n	.L_0803c922
	.4byte 0x000001ff
	.2byte 0xfe00
	.2byte 0xffff
	ldr	r3, [r1, #0]
	movs	r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0803c820
	ldrh	r3, [r6, #12]
	adds	r3, #1
	strh	r3, [r6, #12]
.L_0803c820:
	ldr	r4, [pc, #68]
	ldrh	r0, [r6, #12]
	movs	r1, #20
	str	r4, [sp, #0]
	bl	sub_0800206c
	ldr	r4, [sp, #0]
	lsls	r0, r0, #16
	lsrs	r0, r0, #15
	ldrsb	r3, [r4, r0]
	ldrh	r2, [r6, #6]
	ldrh	r1, [r7, #6]
	adds	r2, r2, r3
	ldr	r3, [pc, #40]
	ands	r2, r3
	ldr	r3, [pc, #44]
	ands	r3, r1
	orrs	r3, r2
	strh	r3, [r7, #6]
	movs	r1, #20
	ldrh	r0, [r6, #12]
	bl	sub_0800206c
	lsls	r0, r0, #16
	ldr	r4, [sp, #0]
	lsrs	r0, r0, #15
	adds	r0, #1
	ldrb	r5, [r6, #8]
	ldrb	r3, [r4, r0]
	adds	r5, r5, r3
	subs	r5, #2
	strb	r5, [r7, #4]
	b.n	.L_0803c922
	movs	r0, r0
	.4byte 0x000001ff
	.4byte 0x0805c17c
	.2byte 0xfe00
	.2byte 0xffff
	ldrh	r3, [r6, #12]
	ldr	r0, [pc, #100]
	adds	r3, #1
	movs	r2, #15
	strh	r3, [r6, #12]
	ands	r3, r2
	ldrb	r1, [r6, #8]
	ldrb	r3, [r0, r3]
	subs	r1, r1, r3
	strb	r1, [r7, #4]
	b.n	.L_0803c922
	ldrh	r3, [r6, #12]
	ldr	r0, [pc, #76]
	adds	r3, #1
	movs	r2, #15
	strh	r3, [r6, #12]
	ands	r3, r2
	ldrb	r1, [r6, #8]
	ldrb	r3, [r0, r3]
	adds	r1, r1, r3
	strb	r1, [r7, #4]
	b.n	.L_0803c922
	ldrh	r3, [r6, #12]
	ldr	r1, [pc, #56]
	adds	r3, #1
	movs	r2, #15
	strh	r3, [r6, #12]
	ands	r3, r2
	ldrsb	r3, [r1, r3]
	ldrh	r2, [r6, #6]
	ldrh	r1, [r7, #6]
	subs	r2, r2, r3
	b.n	.L_0803c8c6
	ldrh	r3, [r6, #12]
	ldr	r1, [pc, #32]
	adds	r3, #1
	movs	r2, #15
	strh	r3, [r6, #12]
	ands	r3, r2
	ldrsb	r3, [r1, r3]
	ldrh	r2, [r6, #6]
	ldrh	r1, [r7, #6]
	adds	r2, r2, r3
.L_0803c8c6:
	ldr	r3, [pc, #12]
	ands	r2, r3
	ldr	r3, [pc, #16]
	ands	r3, r1
	orrs	r3, r2
	strh	r3, [r7, #6]
	b.n	.L_0803c922
	.4byte 0x000001ff
	.4byte 0x0805c1b4
	.2byte 0xfe00
	.2byte 0xffff
	ldrh	r3, [r6, #12]
	ldr	r4, [pc, #48]
	adds	r3, #1
	movs	r0, #15
	strh	r3, [r6, #12]
	ands	r3, r0
	ldrh	r2, [r6, #6]
	ldrsb	r3, [r4, r3]
	ldrh	r1, [r7, #6]
	subs	r2, r2, r3
	ldr	r3, [pc, #24]
	ands	r2, r3
	ldr	r3, [pc, #28]
	ands	r3, r1
	orrs	r3, r2
	strh	r3, [r7, #6]
	ldrh	r3, [r6, #12]
	ldrb	r2, [r6, #8]
	ands	r0, r3
	ldrb	r3, [r4, r0]
	adds	r2, r2, r3
	strb	r2, [r7, #4]
	b.n	.L_0803c922
	movs	r0, r0
	.4byte 0x000001ff
	.4byte 0x0805c1b4
	.2byte 0xfe00
	.2byte 0xffff
	adds	r0, r6, #0
	bl	.L_0803c40c
.L_0803c922:
	ldrb	r3, [r6, #5]
	cmp	r3, #2
	bne.n	.L_0803c940
	movs	r3, #152
	lsls	r3, r3, #5
	adds	r3, #70
	add	r3, r9
	ldrh	r3, [r3, #0]
	cmp	r3, #96
	beq.n	.L_0803c974
	adds	r0, r7, #0
	movs	r1, #255
	bl	sub_08014128
	b.n	.L_0803c974
.L_0803c940:
	cmp	r3, #19
	bne.n	.L_0803c968
	movs	r1, #6
	ldrsh	r3, [r6, r1]
	movs	r1, #8
	ldrsh	r2, [r6, r1]
	movs	r1, #152
	adds	r3, #64
	lsls	r1, r1, #1
	cmp	r3, r1
	bcs.n	.L_0803c974
	adds	r3, r2, #0
	adds	r3, #64
	cmp	r3, #223
	bhi.n	.L_0803c974
	ldrb	r1, [r6, #15]
	adds	r0, r7, #0
	bl	sub_08014128
	b.n	.L_0803c974
.L_0803c968:
	cmp	r3, #13
	beq.n	.L_0803c974
	ldrb	r1, [r6, #15]
	adds	r0, r7, #0
	bl	sub_08014128
.L_0803c974:
	ldr	r3, [pc, #48]
	ldr	r6, [r6, #0]
	adds	r1, r3, #0
.L_0803c97a:
	ldr	r3, [r1, #0]
	lsrs	r0, r3, #2
	movs	r3, #7
	ands	r0, r3
	cmp	r6, #0
	beq.n	.L_0803c988
	b.n	.L_0803c582
.L_0803c988:
	movs	r3, #1
	add	fp, r3
	movs	r2, #36
	mov	r1, fp
	add	sl, r2
	cmp	r1, #12
	beq.n	.L_0803c998
	b.n	.L_0803c56c
.L_0803c998:
	add	sp, #24
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0x1100
	.2byte 0x0300
	push	{lr}
	ldr	r2, [pc, #8]
	bl	.L_0803ca20
	pop	{pc}
	movs	r0, r0
	.2byte 0xffff
	.2byte 0x7fff
	push	{r5, r6, r7, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r6, [r3, #60]
	movs	r3, #152
	lsls	r3, r3, #5
	adds	r3, #66
	adds	r5, r2, #0
	adds	r2, r6, r3
	movs	r3, #0
	adds	r7, r1, #0
	strh	r3, [r2, #0]
	movs	r1, #1
	bl	sub_0803b094
	subs	r5, #1
	movs	r0, #0
	cmp	r0, r5
	bcs.n	.L_0803ca0e
	movs	r2, #244
	lsls	r2, r2, #4
	ldrh	r3, [r6, r2]
	strh	r3, [r7, #0]
	lsls	r3, r3, #16
	cmp	r3, #0
	beq.n	.L_0803ca0e
	mov	ip, r5
	adds	r2, r6, r2
	movs	r4, #0
.L_0803c9f6:
	adds	r0, #1
	adds	r4, #2
	cmp	r0, ip
	bcs.n	.L_0803ca12
	adds	r2, #2
	ldrh	r3, [r2, #0]
	adds	r1, r4, #0
	strh	r3, [r1, r7]
	lsls	r3, r3, #16
	cmp	r3, #0
	bne.n	.L_0803c9f6
	b.n	.L_0803ca14
.L_0803ca0e:
	movs	r1, #0
	b.n	.L_0803ca14
.L_0803ca12:
	lsls	r1, r0, #1
.L_0803ca14:
	ldr	r3, [pc, #4]
	strh	r3, [r1, r7]
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0x0000
	.2byte 0x0000
.L_0803ca20:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	adds	r7, r2, #0
	movs	r2, #192
	lsls	r2, r2, #18
	adds	r2, #200
	ldr	r3, [r2, #0]
	sub	sp, #12
	mov	r9, r0
	adds	r6, r1, #0
	mov	r8, r2
	mov	sl, r3
	cmp	r3, #0
	bne.n	.L_0803ca66
	ldr	r5, [pc, #192]
	movs	r0, #200
	adds	r1, r5, #0
	bl	sub_08014cc0
	movs	r2, #132
	movs	r3, #128
	lsrs	r5, r5, #2
	lsls	r2, r2, #24
	lsls	r3, r3, #19
	adds	r1, r0, #0
	adds	r3, #212
	ldr	r0, [pc, #172]
	orrs	r2, r5
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	mov	r2, r8
	ldr	r3, [r2, #0]
.L_0803ca66:
	mov	r5, sp
	mov	r1, r9
	adds	r0, r5, #0
	mov	r8, r3
	bl	sub_0803d178
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	mov	r9, r3
	b.n	.L_0803cad8
.L_0803ca7c:
	cmp	r0, #14
	beq.n	.L_0803ca94
	cmp	r0, #14
	bhi.n	.L_0803ca8e
	cmp	r0, #12
	bhi.n	.L_0803cace
	cmp	r0, #8
	bcc.n	.L_0803cace
	b.n	.L_0803cab4
.L_0803ca8e:
	cmp	r0, #15
	beq.n	.L_0803cab4
	b.n	.L_0803cace
.L_0803ca94:
	subs	r7, #3
	cmp	r7, #0
	ble.n	.L_0803cae2
	strh	r0, [r6, #0]
	adds	r0, r5, #0
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x3602
	add	r0, r9
	strh	r0, [r6, #0]
	adds	r0, r5, #0
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x3602
	add	r0, r9
	b.n	.L_0803cad4
.L_0803cab4:
	subs	r7, #1
	cmp	r7, #0
	ble.n	.L_0803cae2
	strh	r0, [r6, #0]
	adds	r0, r5, #0
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x22ff
	lsls	r2, r2, #8
	adds	r2, #255
	adds	r6, #2
	adds	r0, r0, r2
	b.n	.L_0803cad4
.L_0803cace:
	subs	r7, #1
	cmp	r7, #0
	ble.n	.L_0803cae2
.L_0803cad4:
	strh	r0, [r6, #0]
	adds	r6, #2
.L_0803cad8:
	adds	r0, r5, #0
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x2800
	bne.n	.L_0803ca7c
.L_0803cae2:
	mov	r3, sl
	cmp	r3, #0
	bne.n	.L_0803caee
	movs	r0, #200
	bl	sub_0801314c
.L_0803caee:
	ldr	r3, [pc, #16]
	add	sp, #12
	strh	r3, [r6, #0]
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x00000000
	.4byte 0x00000144
	.4byte 0x08038438
