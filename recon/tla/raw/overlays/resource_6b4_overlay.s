.syntax unified
	.thumb
	push	{r5, lr}
	adds	r5, r0, #0
	adds	r4, r1, #0
	cmp	r5, #0
	beq.n	.L_0200007c
	adds	r3, r5, #0
	adds	r3, #84
	ldrb	r2, [r3, #0]
	movs	r3, #15
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0200007c
	ldr	r1, [r5, #80]
	movs	r2, #13
	ldrb	r0, [r1, #9]
	movs	r3, #3
	negs	r2, r2
	ands	r4, r3
	adds	r3, r2, #0
	lsls	r4, r4, #2
	ands	r3, r0
	orrs	r3, r4
	strb	r3, [r1, #9]
	adds	r1, #37
	ldrb	r3, [r1, #0]
	ands	r2, r3
	orrs	r2, r4
	strb	r2, [r1, #0]
	adds	r1, r5, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #254
	ands	r3, r2
	strb	r3, [r1, #0]
.L_0200007c:
	pop	{r5, pc}
	.2byte 0x0000
	.4byte 0x6c426883
	.4byte 0x189b6d01
	.4byte 0x6c826083
	.4byte 0x189b68c3
	.4byte 0x6cc260c3
	.4byte 0x189b6903
	.4byte 0x6b026103
	.4byte 0x189b6983
	.4byte 0x6b426183
	.4byte 0x189b69c3
	.4byte 0x306461c3
	.4byte 0x88028a4b
	.4byte 0x824b189b
	.2byte 0x4770
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	mov	fp, r3
	ldr	r3, [pc, #420]
	sub	sp, #4
	mov	sl, r2
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r6, r1, #0
	ldr	r1, [sp, #44]
	adds	r3, r3, r2
	adds	r5, r0, #0
	ldr	r0, [r3, #0]
	mov	r8, r1
	ldr	r7, [sp, #48]
	bl 0x02009494
	movs	r3, #128
	lsls	r3, r3, #13
	mov	r1, r8
	ands	r3, r1
	mov	r9, r0
	cmp	r3, #0
	beq.n	.L_02000100
	cmp	r7, #0
	beq.n	.L_02000100
	movs	r2, #24
	ldrsh	r0, [r7, r2]
	adds	r1, r5, #0
	adds	r2, r6, #0
	b.n	.L_02000108
.L_02000100:
	movs	r0, #30
	adds	r2, r6, #0
	adds	r0, #255
	adds	r1, r5, #0
.L_02000108:
	mov	r3, sl
	bl 0x0200946c
	adds	r6, r0, #0
	cmp	r6, #0
	bne.n	.L_02000116
	b.n	.L_02000262
.L_02000116:
	ldr	r3, [r6, #80]
	mov	r1, r8
	movs	r5, #15
	adds	r1, #1
	ands	r1, r5
	adds	r0, r6, #0
	str	r3, [sp, #0]
	bl 0x0200945c
	ldr	r2, [pc, #328]
	mov	r3, r8
	ands	r3, r5
	lsls	r3, r3, #2
	ldr	r1, [r2, r3]
	adds	r0, r6, #0
	mov	sl, r3
	bl 0x02009464
	adds	r3, r6, #0
	movs	r5, #0
	adds	r3, #85
	strb	r5, [r3, #0]
	adds	r0, r6, #0
	movs	r1, #0
	bl 0x02009474
	ldr	r3, [pc, #300]
	mov	r1, r9
	str	r3, [r6, #108]
	mov	r3, fp
	str	r3, [r6, #68]
	ldr	r3, [sp, #36]
	adds	r0, r6, #0
	str	r3, [r6, #72]
	ldr	r3, [sp, #40]
	str	r3, [r6, #76]
	ldr	r3, [r1, #80]
	ldrb	r1, [r3, #9]
	lsls	r1, r1, #28
	lsrs	r1, r1, #30
	bl 0x02008038
	movs	r2, #100
	adds	r2, r2, r6
	mov	r9, r2
	mov	r3, r9
	str	r5, [r6, #48]
	str	r5, [r6, #52]
	strh	r5, [r3, #0]
	ldr	r3, [pc, #256]
	mov	r1, r8
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_02000262
	cmp	r7, #0
	beq.n	.L_02000262
	movs	r3, #128
	lsls	r3, r3, #9
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_02000198
	ldr	r1, [r7, #4]
	adds	r0, r6, #0
	bl 0x020094dc
.L_02000198:
	movs	r3, #128
	lsls	r3, r3, #10
	mov	r2, r8
	ands	r3, r2
.L_020001a0:
	cmp	r3, #0
	beq.n	.L_020001b8
	adds	r1, r6, #0
	adds	r1, #35
	ldrb	r3, [r1, #0]
	movs	r2, #254
	ands	r2, r3
	strb	r2, [r1, #0]
	ldr	r1, [r7, #0]
	adds	r0, r6, #0
	bl 0x02008038
.L_020001b8:
	movs	r2, #128
	lsls	r2, r2, #12
	mov	r3, r8
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_020001cc
	ldr	r3, [r7, #8]
	str	r3, [r6, #24]
	ldr	r3, [r7, #12]
	str	r3, [r6, #28]
.L_020001cc:
	movs	r3, #128
	lsls	r3, r3, #11
	mov	r1, r8
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_02000212
	ldr	r3, [pc, #152]
	mov	r1, sl
	ldr	r5, [r3, r1]
	ldr	r3, [r7, #16]
	ldr	r1, [r5, #12]
	cmp	r2, #0
	beq.n	.L_020001fa
	ldr	r0, [r6, #24]
	subs	r0, r3, r0
	bl 0x0200940c
.L_020001ee:
	str	r0, [r6, #48]
	ldr	r0, [r7, #20]
	ldr	r3, [r6, #28]
	ldr	r1, [r5, #12]
	subs	r0, r0, r3
	b.n	.L_0200020c
.L_020001fa:
	ldr	r2, [pc, #128]
	adds	r0, r3, r2
	bl 0x0200940c
	str	r0, [r6, #48]
	ldr	r0, [r7, #20]
	ldr	r3, [pc, #116]
	ldr	r1, [r5, #12]
	adds	r0, r0, r3
.L_0200020c:
	bl 0x0200940c
	str	r0, [r6, #52]
.L_02000212:
	movs	r3, #128
	lsls	r3, r3, #14
	mov	r1, r8
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_0200022e
	adds	r0, r6, #0
	movs	r1, #1
	bl 0x0200945c
	ldr	r1, [r7, #28]
	adds	r0, r6, #0
	bl 0x02009464
.L_0200022e:
	movs	r3, #128
	lsls	r3, r3, #15
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02000240
	ldrh	r3, [r7, #32]
	ldr	r1, [sp, #0]
	strh	r3, [r1, #18]
.L_02000240:
	movs	r3, #128
	lsls	r3, r3, #16
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02000252
	ldrh	r3, [r7, #34]
	mov	r1, r9
	strh	r3, [r1, #0]
.L_02000252:
	movs	r3, #128
	lsls	r3, r3, #17
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02000262
	ldr	r3, [r7, #36]
	str	r3, [r6, #108]
.L_02000262:
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0x02009650
	.4byte 0x02008081
	.2byte 0x0000
	.2byte 0xffff
	.global Func_02000280
	.thumb_func
Func_02000280:
	.2byte 0x4800
	bx	lr
	.2byte 0x965c
	.2byte 0x0200
	.global Func_02000288
	.thumb_func
Func_02000288:
	movs	r0, #0
	bx	lr
	.global Func_0200028c
	.thumb_func
Func_0200028c:
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x96a4
	.2byte 0x0200
	.global Func_02000294
	.thumb_func
Func_02000294:
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x96a8
	.2byte 0x0200
	.global Func_0200029c
	.thumb_func
Func_0200029c:
	ldr	r0, [pc, #0]
	bx	lr
	.2byte 0x96f0
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	mov	r8, r2
	mov	r3, r8
	movs	r2, #0
	adds	r7, r0, #0
	mov	ip, r1
	mov	lr, r2
	cmp	r3, #0
	beq.n	.L_02000324
.L_020002ba:
	mov	r2, ip
	ldrh	r3, [r2, #0]
	movs	r1, #31
	adds	r0, r1, #0
	ands	r0, r3
	lsls	r3, r3, #16
	lsrs	r5, r3, #21
	lsrs	r6, r3, #26
	ldrh	r3, [r7, #0]
	ldr	r2, [pc, #36]
	ands	r1, r3
	lsls	r3, r3, #16
	lsrs	r4, r3, #21
	lsrs	r3, r3, #26
	ands	r5, r2
	ands	r6, r2
	ands	r4, r2
	ands	r3, r2
	cmp	r1, r0
	bge.n	.L_020002e6
	adds	r1, #1
	b.n	.L_020002ec
.L_020002e6:
	cmp	r1, r0
	ble.n	.L_020002ec
	subs	r1, #1
.L_020002ec:
	cmp	r4, r5
	bge.n	.L_020002f8
	adds	r4, #1
	b.n	.L_020002fe
	.2byte 0x001f
	.2byte 0x0000
.L_020002f8:
	cmp	r4, r5
	ble.n	.L_020002fe
	subs	r4, #1
.L_020002fe:
	cmp	r3, r6
	bge.n	.L_02000306
	adds	r3, #1
	b.n	.L_0200030c
.L_02000306:
	cmp	r3, r6
	ble.n	.L_0200030c
	subs	r3, #1
.L_0200030c:
	lsls	r2, r4, #5
	lsls	r3, r3, #10
	orrs	r3, r2
	orrs	r3, r1
	strh	r3, [r7, #0]
	movs	r3, #1
	movs	r2, #2
	add	lr, r3
	add	ip, r2
	adds	r7, #2
	cmp	lr, r8
	bne.n	.L_020002ba
.L_02000324:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	adds	r5, r0, #0
	mov	ip, r1
	adds	r6, r2, #0
	movs	r7, #0
	cmp	r1, #0
	beq.n	.L_02000390
.L_0200033a:
	ldrh	r2, [r5, #0]
	ldr	r1, [pc, #40]
	movs	r3, #31
	ands	r3, r2
	lsls	r2, r2, #16
	adds	r0, r3, r6
	lsrs	r3, r2, #21
	lsrs	r2, r2, #26
	ands	r3, r1
	ands	r2, r1
	adds	r4, r3, r6
	adds	r2, r2, r6
	cmp	r0, #31
	ble.n	.L_02000358
	movs	r0, #31
.L_02000358:
	cmp	r0, #0
	bge.n	.L_0200035e
	movs	r0, #0
.L_0200035e:
	cmp	r4, #31
	ble.n	.L_0200036c
	movs	r4, #31
	b.n	.L_0200036c
	.2byte 0x0000
	.2byte 0x001f
	.2byte 0x0000
.L_0200036c:
	cmp	r4, #0
	bge.n	.L_02000372
	movs	r4, #0
.L_02000372:
	cmp	r2, #31
	ble.n	.L_02000378
	movs	r2, #31
.L_02000378:
	cmp	r2, #0
	bge.n	.L_0200037e
	movs	r2, #0
.L_0200037e:
	lsls	r3, r2, #10
	lsls	r2, r4, #5
	orrs	r3, r2
	orrs	r3, r0
	adds	r7, #1
	strh	r3, [r5, #0]
	adds	r5, #2
	cmp	r7, ip
	bne.n	.L_0200033a
.L_02000390:
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r3, #160
	lsls	r3, r3, #19
	adds	r3, #194
	mov	r8, r3
	ldr	r3, [pc, #72]
	movs	r7, #160
	ldr	r0, [r3, #0]
	lsls	r7, r7, #19
	lsls	r0, r0, #11
	lsrs	r0, r0, #1
	bl 0x02009434
	lsls	r0, r0, #1
	adds	r7, #130
	asrs	r5, r0, #16
	movs	r6, #0
.L_020003ba:
	mov	r3, r8
	ldrh	r2, [r3, #0]
	ldr	r1, [pc, #40]
	movs	r3, #31
	ands	r3, r2
	lsls	r2, r2, #16
	adds	r0, r3, r5
	lsrs	r3, r2, #21
	lsrs	r2, r2, #26
	ands	r3, r1
	ands	r2, r1
	adds	r4, r3, r5
	adds	r2, r2, r5
	cmp	r0, #31
	ble.n	.L_020003da
	movs	r0, #31
.L_020003da:
	cmp	r0, #0
	bge.n	.L_020003e0
	movs	r0, #0
.L_020003e0:
	cmp	r4, #31
	ble.n	.L_020003f0
	movs	r4, #31
	b.n	.L_020003f0
	.4byte 0x0000001f
	.2byte 0x122c
	.2byte 0x0300
.L_020003f0:
	cmp	r4, #0
	bge.n	.L_020003f6
	movs	r4, #0
.L_020003f6:
	cmp	r2, #31
	ble.n	.L_020003fc
	movs	r2, #31
.L_020003fc:
	cmp	r2, #0
	bge.n	.L_02000402
	movs	r2, #0
.L_02000402:
	lsls	r3, r2, #10
	lsls	r2, r4, #5
	orrs	r3, r2
	orrs	r3, r0
	strh	r3, [r7, #0]
	adds	r6, #1
	movs	r3, #2
	add	r8, r3
	adds	r7, #2
	cmp	r6, #15
	bne.n	.L_020003ba
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	adds	r6, r0, #0
	ldr	r5, [r6, #68]
	ldr	r3, [r6, #8]
	ldr	r2, [r6, #72]
	adds	r3, r3, r5
	str	r3, [r6, #8]
	ldr	r3, [r6, #12]
	ldr	r7, [r6, #76]
	adds	r3, r3, r2
	str	r3, [r6, #12]
	ldr	r3, [r6, #16]
	adds	r0, r5, #0
	adds	r3, r3, r7
	movs	r1, #18
	str	r3, [r6, #16]
	bl 0x0200940c
	subs	r5, r5, r0
	str	r5, [r6, #68]
	adds	r3, r7, #0
	cmp	r7, #0
	bge.n	.L_02000450
	adds	r3, #15
.L_02000450:
	asrs	r3, r3, #4
	subs	r3, r7, r3
	str	r3, [r6, #76]
	ldr	r2, [r6, #48]
	ldr	r3, [r6, #24]
	ldr	r1, [r6, #80]
	adds	r3, r3, r2
	str	r3, [r6, #24]
	ldr	r2, [r6, #52]
	ldr	r3, [r6, #28]
	adds	r3, r3, r2
	str	r3, [r6, #28]
	adds	r2, r6, #0
	adds	r2, #100
	ldrh	r3, [r1, #18]
	ldrh	r2, [r2, #0]
	adds	r3, r3, r2
	strh	r3, [r1, #18]
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	mov	fp, r0
	movs	r0, #8
	sub	sp, #68
	mov	r9, r1
	bl 0x02009494
	ldr	r3, [pc, #196]
	add	r7, sp, #28
	str	r3, [r7, #36]
	movs	r3, #204
	lsls	r3, r3, #8
	adds	r3, #204
	str	r3, [r7, #8]
	str	r3, [r7, #12]
	ldr	r3, [r0, #80]
	movs	r2, #0
	ldrb	r3, [r3, #9]
	mov	r8, r0
	lsls	r3, r3, #28
	lsrs	r3, r3, #30
	str	r3, [r7, #0]
	mov	sl, r2
.L_020004b2:
	movs	r0, #128
	mov	r1, r9
	lsls	r0, r0, #9
	bl 0x0200940c
	mov	r5, sl
	muls	r5, r0
	adds	r0, r5, #0
	bl 0x0200943c
	mov	r3, fp
	muls	r3, r0
	add	r6, sp, #16
	cmp	r3, #0
	bge.n	.L_020004d2
	adds	r3, #255
.L_020004d2:
	asrs	r3, r3, #8
	str	r3, [r6, #0]
	adds	r0, r5, #0
	movs	r3, #0
	str	r3, [r6, #4]
	bl 0x02009434
	mov	r3, fp
	muls	r3, r0
	cmp	r3, #0
	bge.n	.L_020004ea
	adds	r3, #255
.L_020004ea:
	asrs	r3, r3, #8
	str	r3, [r6, #8]
	ldr	r3, [r6, #0]
	asrs	r2, r3, #1
	adds	r3, r3, r2
	str	r3, [r6, #0]
	bl 0x0200942c
	lsls	r3, r0, #1
	ldr	r2, [r6, #0]
	adds	r3, r3, r0
	lsls	r3, r3, #14
	lsrs	r3, r3, #16
	adds	r2, r2, r3
	ldr	r3, [pc, #84]
	adds	r2, r2, r3
	str	r2, [r6, #0]
	bl 0x0200942c
	lsls	r3, r0, #1
	ldr	r5, [r6, #8]
	adds	r3, r3, r0
	ldr	r2, [pc, #72]
	lsls	r3, r3, #13
	lsrs	r3, r3, #16
	adds	r5, r5, r3
	adds	r5, r5, r2
	str	r5, [r6, #8]
	ldr	r4, [r6, #4]
	mov	r3, r8
	ldr	r2, [r3, #16]
	ldr	r0, [r3, #8]
	ldr	r1, [r3, #12]
	ldr	r3, [r6, #0]
	str	r4, [sp, #0]
	movs	r4, #133
	lsls	r4, r4, #17
	adds	r4, #1
	str	r5, [sp, #4]
	str	r4, [sp, #8]
	str	r7, [sp, #12]
	bl 0x020080b8
	movs	r2, #1
	add	sl, r2
	cmp	sl, r9
	bls.n	.L_020004b2
	add	sp, #68
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
	.4byte 0x02008421
	.4byte 0xffffa000
	.2byte 0xd000
	.2byte 0xffff
	.global Func_02000564
	.thumb_func
Func_02000564:
	.2byte 0xb5e0
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	movs	r0, #128
	lsls	r0, r0, #2
	bl 0x02009444
	mov	r9, r0
	bl 0x02009454
	ldr	r5, [pc, #60]
	movs	r1, #147
	lsls	r1, r1, #1
	movs	r2, #128
	adds	r1, #255
	lsls	r2, r2, #2
	adds	r3, r5, r1
	adds	r2, #38
	ldrb	r0, [r3, #0]
	adds	r3, r5, r2
	ldrb	r1, [r3, #0]
	bl 0x02009484
	movs	r1, #0
	movs	r2, #0
	movs	r0, #4
	bl 0x020094b4
	movs	r1, #241
	lsls	r1, r1, #1
	adds	r3, r5, r1
	movs	r2, #0
	ldrsh	r5, [r3, r2]
	cmp	r5, #10
	bne.n	.L_020005c4
	ldr	r0, [pc, #16]
	movs	r1, #72
	bl 0x02009534
	bl 0x020093d6
	movs	r0, r0
	lsls	r0, r0, #9
	lsls	r0, r0, #8
	movs	r3, r0
	movs	r0, r0
.L_020005c4:
	cmp	r5, #2
	beq.n	.L_020005ca
	b.n	.L_020007fc
.L_020005ca:
	movs	r0, #128
	movs	r1, #1
	movs	r2, #128
	lsls	r2, r2, #16
	movs	r3, #0
	lsls	r0, r0, #17
	negs	r1, r1
	bl 0x02009524
	movs	r0, #128
	movs	r1, #128
	lsls	r1, r1, #6
	lsls	r0, r0, #9
	bl 0x0200951c
	movs	r0, #8
	bl 0x02009494
	movs	r1, #0
	bl 0x02009474
	movs	r0, #8
.L_020005f6:
	movs	r1, #12
	bl 0x020094bc
	movs	r0, #8
	movs	r1, #8
	bl 0x020094c4
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r1, #214
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #128
	lsls	r1, r1, #19
	movs	r2, #0
	str	r2, [r3, #0]
	ldrh	r2, [r1, #0]
	movs	r3, #253
	lsls	r3, r3, #8
	adds	r3, #255
	ands	r3, r2
	strh	r3, [r1, #0]
	movs	r0, #255
	adds	r1, #10
	ldrh	r2, [r1, #0]
	lsls	r0, r0, #8
	adds	r0, #252
	adds	r3, r0, #0
	ands	r3, r2
	strh	r3, [r1, #0]
	ldr	r2, [pc, #52]
	ldrh	r3, [r1, #0]
	movs	r5, #0
	orrs	r3, r2
	strh	r3, [r1, #0]
	adds	r1, #4
	ldrh	r3, [r1, #0]
	ldr	r2, [pc, #44]
	ands	r0, r3
	strh	r0, [r1, #0]
	ldrh	r3, [r1, #0]
	orrs	r3, r2
	strh	r3, [r1, #0]
	bl 0x0200954c
	bl 0x0200955c
	movs	r0, #128
	movs	r1, #1
	movs	r2, #159
	lsls	r0, r0, #17
	negs	r1, r1
	lsls	r2, r2, #17
	movs	r3, #1
	bl 0x02009524
	b.n	.L_02000674
	.2byte 0x0000
	.4byte 0x00000003
	.2byte 0x0002
	.2byte 0x0000
.L_02000674:
	bl 0x0200952c
	movs	r0, #158
	movs	r1, #1
	movs	r2, #159
	lsls	r0, r0, #17
	negs	r1, r1
	lsls	r2, r2, #17
	movs	r3, #1
	bl 0x02009524
	bl 0x0200952c
.L_0200068e:
	cmp	r5, #3
	bne.n	.L_020006b4
	movs	r0, #200
	movs	r1, #141
	lsls	r0, r0, #5
	lsls	r1, r1, #2
	adds	r0, #153
	adds	r1, #255
	bl 0x0200951c
	movs	r0, #158
	movs	r1, #1
	movs	r2, #150
	lsls	r0, r0, #17
	negs	r1, r1
	lsls	r2, r2, #17
	movs	r3, #1
	bl 0x02009524
.L_020006b4:
	movs	r0, #8
	bl 0x02009494
	ldr	r2, [pc, #108]
	ldr	r3, [r0, #16]
	adds	r5, #1
	adds	r3, r3, r2
	str	r3, [r0, #16]
	movs	r0, #30
	bl 0x02009414
	cmp	r5, #10
	bne.n	.L_0200068e
	movs	r0, #8
	movs	r1, #0
	bl 0x020094c4
	movs	r1, #160
	lsls	r1, r1, #19
	ldr	r3, [pc, #80]
	mov	r0, r9
	adds	r1, #8
	movs	r2, #14
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2600
.L_020006e8:
	movs	r7, #160
	lsls	r7, r7, #19
	adds	r7, #14
	mov	ip, r7
	movs	r5, #0
.L_020006f2:
	mov	r1, ip
	ldrh	r3, [r1, #0]
	ldr	r2, [pc, #44]
	movs	r4, #31
	ands	r4, r3
	lsls	r3, r3, #16
	lsrs	r0, r3, #21
	lsrs	r1, r3, #26
	ands	r0, r2
	ands	r1, r2
	adds	r4, r4, r6
	adds	r3, r6, #0
	cmp	r6, #0
	bge.n	.L_02000710
	adds	r3, #15
.L_02000710:
	asrs	r3, r3, #4
	adds	r0, r0, r3
	subs	r1, r1, r6
	cmp	r4, #31
	ble.n	.L_0200071c
	movs	r4, #31
.L_0200071c:
	cmp	r0, #31
	ble.n	.L_02000730
	movs	r0, #31
	b.n	.L_02000730
	.4byte 0x0000001f
	.4byte 0xffff0000
	.2byte 0x0730
	.2byte 0x0300
.L_02000730:
	cmp	r1, #7
	bgt.n	.L_02000736
	movs	r1, #8
.L_02000736:
	cmp	r1, #31
	ble.n	.L_0200073c
	movs	r1, #31
.L_0200073c:
	lsls	r2, r0, #5
	lsls	r3, r1, #10
	orrs	r3, r2
	orrs	r3, r4
	movs	r2, #2
	adds	r5, #1
	strh	r3, [r7, #0]
	add	ip, r2
	adds	r7, #2
	cmp	r5, #8
	bne.n	.L_020006f2
	movs	r0, #4
	adds	r6, #1
	bl 0x02009414
	cmp	r6, #10
	bne.n	.L_020006e8
	movs	r1, #129
	movs	r2, #0
	lsls	r1, r1, #1
	movs	r0, #8
	bl 0x0200950c
	movs	r0, #30
	bl 0x02009414
	ldr	r0, [pc, #124]
	bl 0x020094e4
	movs	r0, #128
	lsls	r0, r0, #7
	movs	r1, #0
	adds	r0, #8
	bl 0x020094f4
	movs	r0, #30
	bl 0x02009414
	movs	r0, #8
	movs	r1, #2
	bl 0x020094cc
	movs	r0, #128
	lsls	r0, r0, #7
	adds	r0, #8
	movs	r1, #0
	bl 0x020094f4
	movs	r0, #8
	movs	r1, #32
	bl 0x020094c4
	movs	r5, #0
.L_020007a6:
	cmp	r5, #16
	bne.n	.L_020007c8
	movs	r0, #158
	movs	r1, #1
	movs	r2, #128
	lsls	r0, r0, #17
	negs	r1, r1
	lsls	r2, r2, #16
	movs	r3, #1
	bl 0x02009524
	movs	r0, #128
	movs	r1, #128
	lsls	r0, r0, #10
	lsls	r1, r1, #7
	bl 0x0200951c
.L_020007c8:
	cmp	r5, #19
	bne.n	.L_020007d0
	bl 0x02009554
.L_020007d0:
	movs	r0, #8
	bl 0x02009494
	ldr	r1, [pc, #28]
	ldr	r3, [r0, #16]
	adds	r5, #1
	adds	r3, r3, r1
	str	r3, [r0, #16]
	movs	r0, #8
	bl 0x02009414
	cmp	r5, #20
	bne.n	.L_020007a6
	ldr	r0, [pc, #12]
	bl 0x02009370
	cmp	r7, #62
	movs	r0, r0
	movs	r0, r0
	.2byte 0xfffe
	.2byte 0x0003
	movs	r0, r0
.L_020007fc:
	cmp	r5, #3
	beq.n	.L_02000804
	bl 0x02009378
.L_02000804:
	movs	r2, #192
	lsls	r2, r2, #18
	mov	r8, r2
	mov	r3, r8
	adds	r3, #128
	ldr	r3, [r3, #0]
	movs	r1, #160
	movs	r2, #224
	mov	sl, r3
	mov	r0, r9
	ldr	r3, [pc, #276]
	lsls	r1, r1, #19
	lsls	r2, r2, #1
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2080
	movs	r1, #1
	movs	r2, #144
	lsls	r2, r2, #16
	movs	r3, #0
	lsls	r0, r0, #17
	negs	r1, r1
	bl 0x02009524
	movs	r0, #128
	movs	r1, #128
	lsls	r0, r0, #9
	lsls	r1, r1, #6
	bl 0x0200951c
	movs	r1, #14
	movs	r0, #8
	bl 0x020094bc
	movs	r0, #8
	bl 0x02009494
	movs	r7, #0
	strh	r7, [r0, #6]
	movs	r0, #8
	bl 0x02009494
	movs	r3, #248
	lsls	r3, r3, #16
	str	r3, [r0, #8]
	movs	r0, #8
	bl 0x02009494
	movs	r3, #152
	lsls	r3, r3, #16
	str	r3, [r0, #16]
	ldr	r0, [pc, #200]
	ldr	r1, [pc, #200]
	ldrh	r3, [r1, #0]
	adds	r4, r3, #0
	strh	r1, [r1, #0]
	ldrh	r2, [r0, #0]
	cmp	r2, #31
	bgt.n	.L_0200089e
	lsls	r3, r2, #1
	adds	r3, r3, r2
	adds	r2, #1
	lsls	r3, r3, #2
	strh	r2, [r0, #0]
	movs	r2, #192
	adds	r3, r3, r0
	lsls	r2, r2, #1
	adds	r3, #4
	adds	r2, #255
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #80
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #10
	str	r2, [r3, #0]
.L_0200089e:
	strh	r4, [r1, #0]
	ldrh	r3, [r1, #0]
	adds	r4, r3, #0
	strh	r1, [r1, #0]
	ldrh	r2, [r0, #0]
	cmp	r2, #31
	bgt.n	.L_020008ce
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r2, #1
	adds	r3, r3, r0
	strh	r2, [r0, #0]
	movs	r2, #128
	adds	r3, #4
	lsls	r2, r2, #5
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #82
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #10
	str	r2, [r3, #0]
.L_020008ce:
	strh	r4, [r1, #0]
	movs	r6, #128
	lsls	r6, r6, #19
	movs	r5, #253
	ldrh	r2, [r6, #0]
	lsls	r5, r5, #8
	adds	r5, #255
	adds	r3, r5, #0
	ands	r3, r2
	strh	r3, [r6, #0]
	movs	r1, #128
	lsls	r1, r1, #19
	adds	r1, #10
	movs	r0, #255
	ldrh	r2, [r1, #0]
	lsls	r0, r0, #8
	adds	r0, #252
	adds	r3, r0, #0
	ands	r3, r2
	strh	r3, [r1, #0]
	ldr	r2, [pc, #48]
	ldrh	r3, [r1, #0]
	orrs	r3, r2
	strh	r3, [r1, #0]
	adds	r1, #4
	ldrh	r3, [r1, #0]
	ldr	r2, [pc, #40]
	ands	r0, r3
	strh	r0, [r1, #0]
	movs	r0, #1
	ldrh	r3, [r1, #0]
	orrs	r3, r2
	strh	r3, [r1, #0]
	bl 0x02009414
	mov	r1, r8
	ldr	r3, [r1, #108]
	movs	r2, #214
	lsls	r2, r2, #1
	adds	r3, r3, r2
	str	r7, [r3, #0]
	ldrh	r3, [r6, #0]
	ands	r5, r3
	strh	r5, [r6, #0]
	b.n	.L_0200093c
	.4byte 0x00000003
	.4byte 0x00000002
	.4byte 0x03000730
	.4byte 0x020038e0
	.2byte 0x0208
	.2byte 0x0400
.L_0200093c:
	bl 0x0200954c
	bl 0x0200955c
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r0, r0, #11
	lsls	r1, r1, #11
	lsls	r2, r2, #9
	bl 0x0200947c
	movs	r5, #0
.L_02000956:
	ldr	r2, [pc, #100]
	movs	r0, #5
	ldrh	r3, [r2, #10]
	adds	r5, #1
	adds	r3, #8
	strh	r3, [r2, #10]
	bl 0x0200948c
	cmp	r5, #30
	bne.n	.L_02000956
	movs	r1, #15
	movs	r0, #8
	bl 0x020094bc
	movs	r0, #60
	bl 0x02009414
	movs	r0, #212
	bl 0x02009594
	movs	r1, #128
	lsls	r1, r1, #19
	adds	r1, #10
	ldrh	r2, [r1, #0]
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #252
	ands	r3, r2
	strh	r3, [r1, #0]
	ldr	r2, [pc, #32]
	ldrh	r3, [r1, #0]
	orrs	r3, r2
	strh	r3, [r1, #0]
	movs	r2, #128
	ldr	r3, [pc, #28]
	ldr	r1, [pc, #32]
	lsls	r2, r2, #19
	adds	r2, #80
	strh	r3, [r2, #0]
	ldr	r0, [pc, #28]
	ldrh	r3, [r0, #0]
	adds	r4, r3, #0
	strh	r0, [r0, #0]
	ldrh	r2, [r1, #0]
	cmp	r2, #31
	bgt.n	.L_020009ec
	b.n	.L_020009c8
	.4byte 0x00000001
	.4byte 0x00003f42
	.4byte 0x03001120
	.4byte 0x020038e0
	.2byte 0x0208
	.2byte 0x0400
.L_020009c8:
	lsls	r3, r2, #1
	adds	r3, r3, r2
	adds	r2, #1
	lsls	r3, r3, #2
	strh	r2, [r1, #0]
	movs	r2, #128
	adds	r3, r3, r1
	lsls	r2, r2, #5
	adds	r3, #4
	adds	r2, #2
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #82
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #10
	str	r2, [r3, #0]
.L_020009ec:
	strh	r4, [r0, #0]
	movs	r6, #0
.L_020009f0:
	cmp	r6, #16
	bne.n	.L_02000a02
	movs	r0, #140
	bl 0x02009594
	movs	r0, #8
	movs	r1, #16
	bl 0x020094bc
.L_02000a02:
	cmp	r6, #51
	bne.n	0x02008a2e
	movs	r0, #107
	bl 0x02009594
	movs	r1, #160
	movs	r2, #224
	lsls	r1, r1, #19
	lsls	r2, r2, #1
	ldr	r5, [pc, #968]
	mov	r0, r9
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x20f8
	movs	r1, #130
	lsls	r0, r0, #4
	lsls	r1, r1, #5
	add	r0, sl
	add	r1, sl
	movs	r2, #96
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x1c32
	subs	r2, #16
	cmp	r2, #12
	bhi.n	.L_02000a74
	lsrs	r3, r2, #31
	adds	r3, r2, r3
	ldr	r0, [pc, #936]
	asrs	r3, r3, #1
	adds	r1, r3, #2
	ldr	r4, [pc, #932]
	ldrh	r3, [r4, #0]
	adds	r5, r3, #0
	strh	r4, [r4, #0]
	ldrh	r2, [r0, #0]
	cmp	r2, #31
	bgt.n	.L_02000a72
	lsls	r3, r2, #1
	adds	r3, r3, r2
	adds	r2, #1
	lsls	r3, r3, #2
	strh	r2, [r0, #0]
	movs	r2, #128
	lsls	r2, r2, #5
	adds	r3, r3, r0
	adds	r3, #4
	orrs	r1, r2
	stmia	r3!, {r1}
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #82
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #10
	str	r2, [r3, #0]
.L_02000a72:
	strh	r5, [r4, #0]
.L_02000a74:
	adds	r2, r6, #0
	subs	r2, #34
	cmp	r2, #16
	bhi.n	.L_02000abc
	lsrs	r3, r2, #31
	adds	r3, r2, r3
	asrs	r3, r3, #1
	ldr	r0, [pc, #864]
	adds	r1, r3, #0
	adds	r1, #8
	ldr	r4, [pc, #860]
	ldrh	r3, [r4, #0]
	adds	r5, r3, #0
	strh	r4, [r4, #0]
	ldrh	r2, [r0, #0]
	cmp	r2, #31
	bgt.n	.L_02000aba
	lsls	r3, r2, #1
	adds	r3, r3, r2
	adds	r2, #1
	lsls	r3, r3, #2
	strh	r2, [r0, #0]
	movs	r2, #128
	lsls	r2, r2, #5
	adds	r3, r3, r0
	adds	r3, #4
	orrs	r1, r2
	stmia	r3!, {r1}
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #82
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #10
	str	r2, [r3, #0]
.L_02000aba:
	strh	r5, [r4, #0]
.L_02000abc:
	adds	r3, r6, #0
	subs	r3, #20
	cmp	r3, #31
	bhi.n	.L_02000ad6
	movs	r0, #160
	movs	r1, #160
	lsls	r0, r0, #19
	lsls	r1, r1, #19
	adds	r0, #128
	adds	r1, #192
	movs	r2, #16
	bl 0x020082a4
.L_02000ad6:
	cmp	r6, #49
	ble.n	.L_02000ae6
	movs	r0, #160
	lsls	r0, r0, #19
	movs	r1, #224
	movs	r2, #1
	bl 0x0200832c
.L_02000ae6:
	movs	r0, #4
	adds	r6, #1
	bl 0x02009414
	cmp	r6, #84
	beq.n	.L_02000af4
	b.n	.L_020009f0
.L_02000af4:
	movs	r0, #8
	movs	r1, #3
	bl 0x02009504
	movs	r2, #230
	movs	r0, #1
	movs	r1, #1
	lsls	r2, r2, #8
	negs	r1, r1
	adds	r2, #102
	negs	r0, r0
	bl 0x0200947c
	movs	r0, #195
	lsls	r0, r0, #1
	bl 0x02009594
	movs	r0, #128
	movs	r1, #1
	movs	r2, #159
	lsls	r0, r0, #17
	negs	r1, r1
	lsls	r2, r2, #17
	movs	r3, #0
	bl 0x02009524
	ldr	r1, [pc, #696]
	ldr	r4, [pc, #700]
	ldrh	r3, [r4, #0]
	adds	r5, r3, #0
	strh	r4, [r4, #0]
	ldrh	r3, [r1, #0]
	cmp	r3, #31
	bgt.n	.L_02000b62
	lsls	r2, r3, #1
	adds	r2, r2, r3
	adds	r3, #1
	strh	r3, [r1, #0]
	movs	r0, #128
	lsls	r0, r0, #19
	adds	r0, #10
	lsls	r2, r2, #2
	adds	r2, r2, r1
	ldrh	r1, [r0, #0]
	movs	r3, #4
	negs	r3, r3
	ands	r3, r1
	movs	r1, #3
	adds	r2, #4
	orrs	r3, r1
	stmia	r2!, {r3}
	movs	r3, #128
	stmia	r2!, {r0}
	lsls	r3, r3, #10
	str	r3, [r2, #0]
.L_02000b62:
	strh	r5, [r4, #0]
	movs	r0, #212
	bl 0x02009594
	movs	r0, #1
	bl 0x02009414
	movs	r1, #144
	ldr	r0, [pc, #632]
	lsls	r1, r1, #3
	bl 0x0200941c
	movs	r6, #0
.L_02000b7c:
	movs	r0, #160
	lsls	r0, r0, #19
	mov	r1, r9
	movs	r2, #224
	bl 0x020082a4
	adds	r6, #1
	movs	r0, #4
	bl 0x02009414
	cmp	r6, #32
	bne.n	.L_02000b7c
	movs	r0, #8
	movs	r1, #1
	bl 0x02009504
	movs	r0, #8
	movs	r1, #1
	bl 0x020094bc
	movs	r0, #128
	movs	r1, #1
	movs	r2, #128
	movs	r3, #1
	lsls	r2, r2, #16
	lsls	r0, r0, #17
	negs	r1, r1
	bl 0x02009524
	movs	r0, #128
	movs	r1, #128
	lsls	r0, r0, #9
	lsls	r1, r1, #6
	bl 0x0200951c
	bl 0x0200952c
	movs	r1, #152
	movs	r2, #152
	lsls	r1, r1, #7
	lsls	r2, r2, #6
	movs	r0, #8
	adds	r1, #204
	adds	r2, #102
	bl 0x0200949c
	movs	r2, #128
	movs	r0, #8
	movs	r1, #248
	bl 0x020094ac
	movs	r1, #2
	movs	r0, #8
	bl 0x020094cc
	movs	r0, #5
	bl 0x0200948c
	movs	r0, #78
	bl 0x02009594
	ldr	r5, [pc, #504]
	movs	r6, #0
	adds	r0, r5, #0
	bl 0x020094e4
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #8
	movs	r1, #0
	movs	r2, #5
	bl 0x020094ec
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #7
	movs	r0, #8
	bl 0x020094fc
	movs	r0, #30
	bl 0x0200948c
	movs	r0, #8
	movs	r1, #14
	bl 0x020094bc
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r2, #5
	adds	r0, #8
	movs	r1, #0
	bl 0x020094ec
	movs	r0, #8
	movs	r1, #15
	bl 0x020094bc
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #8
	movs	r1, #0
	movs	r2, #150
	bl 0x020094ec
	movs	r1, #2
	movs	r2, #30
	adds	r1, #255
	movs	r0, #8
	bl 0x0200950c
	movs	r0, #8
	movs	r1, #1
	bl 0x020094bc
	movs	r1, #192
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #8
	bl 0x020094fc
	movs	r0, #10
	bl 0x0200948c
	movs	r1, #128
	lsls	r1, r1, #6
	movs	r2, #0
	movs	r0, #8
	bl 0x020094fc
	movs	r0, #10
	bl 0x0200948c
	movs	r1, #192
	lsls	r1, r1, #7
	movs	r2, #0
	movs	r0, #8
	bl 0x020094fc
	movs	r0, #10
	bl 0x0200948c
	movs	r1, #128
	lsls	r1, r1, #6
	movs	r2, #0
	movs	r0, #8
	bl 0x020094fc
	movs	r0, #10
	bl 0x0200948c
	movs	r1, #128
	movs	r2, #0
	lsls	r1, r1, #7
	movs	r0, #8
	bl 0x020094fc
	movs	r0, #50
	bl 0x0200948c
	movs	r1, #15
	movs	r0, #8
	bl 0x020094bc
	movs	r0, #50
	bl 0x0200948c
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r2, #5
	movs	r1, #0
	adds	r0, #8
	bl 0x020094ec
	movs	r0, #50
	bl 0x0200948c
	movs	r1, #2
	movs	r0, #8
	bl 0x020094cc
	movs	r0, #5
	bl 0x0200948c
	movs	r0, #9
	bl 0x02009494
	movs	r1, #0
	bl 0x02009474
	movs	r0, #9
	movs	r1, #216
	movs	r2, #96
	bl 0x020094ac
	movs	r1, #128
	movs	r0, #8
	lsls	r1, r1, #6
	movs	r2, #0
	bl 0x020094fc
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #8
	bl 0x0200950c
	adds	r0, r5, #4
	bl 0x020094e4
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r1, #0
	movs	r2, #5
	adds	r0, #8
	bl 0x020094ec
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #210
	bl 0x02009594
	movs	r2, #5
	movs	r0, #9
	movs	r1, #0
	bl 0x020094ec
	movs	r0, #8
	movs	r1, #1
	bl 0x020094bc
	movs	r1, #160
	movs	r0, #8
	lsls	r1, r1, #8
	movs	r2, #0
	bl 0x020094fc
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #8
	bl 0x0200950c
	movs	r0, #8
	bl 0x02009494
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #254
	ands	r3, r2
	movs	r1, #128
	strb	r3, [r0, #0]
	lsls	r1, r1, #1
	movs	r0, #8
	movs	r2, #136
.L_02000d78:
	bl 0x020094a4
	movs	r1, #153
	movs	r2, #152
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	adds	r2, #204
	adds	r1, #153
	movs	r0, #8
	bl 0x0200949c
	movs	r0, #20
	bl 0x0200948c
	movs	r0, #8
	movs	r1, #1
	bl 0x020094bc
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r2, #5
	adds	r0, #8
	movs	r1, #0
	bl 0x020094ec
	movs	r0, #9
	movs	r1, #4
	bl 0x020094bc
	movs	r2, #5
	movs	r0, #9
	movs	r1, #0
	bl 0x020094ec
	movs	r1, #2
	movs	r0, #8
	bl 0x020094cc
	movs	r0, #5
	bl 0x0200948c
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r2, #5
	adds	r0, #8
	movs	r1, #0
	bl 0x020094ec
	movs	r0, #9
	movs	r1, #5
	b.n	.L_02000df4
	.2byte 0x0000
	.4byte 0x03000730
	.4byte 0x020038e0
	.4byte 0x04000208
	.4byte 0x02008395
	.2byte 0x2f40
	.2byte 0x0000
.L_02000df4:
	bl 0x020094bc
	movs	r2, #5
	movs	r0, #9
	movs	r1, #0
	bl 0x020094ec
	movs	r1, #129
	lsls	r1, r1, #1
	movs	r0, #8
	bl 0x02009514
	movs	r0, #20
	bl 0x0200948c
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r2, #5
	adds	r0, #8
	movs	r1, #0
	bl 0x020094ec
	movs	r0, #9
	movs	r1, #6
	bl 0x020094bc
	movs	r0, #9
.L_02000e2a:
	movs	r1, #0
	movs	r2, #5
	bl 0x020094ec
	movs	r1, #8
	adds	r1, #255
	movs	r2, #30
	movs	r0, #8
	adds	r5, #12
	bl 0x0200950c
	adds	r0, r5, #0
	bl 0x020094e4
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r2, #5
	adds	r0, #8
	movs	r1, #0
	bl 0x020094ec
	movs	r1, #13
	movs	r0, #8
	bl 0x020094bc
	bl 0x02009574
	movs	r0, #8
	bl 0x02009494
	movs	r1, #2
	bl 0x0200958c
	movs	r0, #178
	bl 0x02009594
	movs	r1, #128
	lsls	r1, r1, #7
	adds	r1, #132
	movs	r0, #8
	bl 0x02009564
	movs	r0, #20
	bl 0x0200948c
	movs	r0, #9
	movs	r1, #3
	bl 0x020094bc
	movs	r1, #200
	movs	r2, #192
	lsls	r1, r1, #5
	lsls	r2, r2, #4
	movs	r0, #9
	adds	r1, #153
	adds	r2, #204
	bl 0x0200949c
.L_02000e9e:
	movs	r3, #1
	ands	r3, r6
	cmp	r3, #0
	beq.n	.L_02000eb2
	movs	r0, #9
	movs	r1, #216
	movs	r2, #96
	bl 0x020094ac
	b.n	.L_02000ecc
.L_02000eb2:
	movs	r0, #9
	bl 0x02009494
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #254
	ands	r3, r2
	strb	r3, [r0, #0]
	movs	r1, #208
	movs	r0, #9
	movs	r2, #88
	bl 0x020094a4
.L_02000ecc:
	movs	r5, #0
.L_02000ece:
	movs	r1, #128
	lsls	r1, r1, #1
	movs	r0, #9
	bl 0x020094d4
	movs	r0, #3
	bl 0x0200948c
	movs	r0, #9
	movs	r1, #0
	bl 0x020094d4
	adds	r5, #1
	movs	r0, #3
	bl 0x0200948c
	cmp	r5, #10
	bne.n	.L_02000ece
	adds	r6, #1
	cmp	r6, #4
	bne.n	.L_02000e9e
	movs	r1, #132
	lsls	r1, r1, #1
	movs	r2, #30
	movs	r0, #8
	bl 0x0200950c
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r2, #5
	movs	r1, #0
	adds	r0, #8
	bl 0x020094ec
	movs	r0, #10
	bl 0x0200948c
	movs	r0, #8
	bl 0x02009494
	movs	r1, #0
	bl 0x0200958c
	bl 0x02009584
	bl 0x0200957c
	movs	r1, #6
	movs	r0, #9
	bl 0x020094bc
	bl 0x02009574
	movs	r0, #9
	bl 0x02009494
	movs	r1, #2
	bl 0x0200958c
	movs	r0, #30
	bl 0x02009414
	movs	r0, #212
	bl 0x02009594
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r0, r0, #13
	lsls	r1, r1, #13
	lsls	r2, r2, #9
	bl 0x0200947c
	movs	r5, #0
.L_02000f62:
	movs	r0, #128
	lsls	r0, r0, #9
	movs	r1, #1
	adds	r0, #5
	bl 0x0200953c
	movs	r0, #1
	bl 0x02009544
	movs	r0, #2
	bl 0x0200948c
	movs	r0, #128
	movs	r1, #1
	lsls	r0, r0, #9
	bl 0x0200953c
	movs	r0, #1
	bl 0x02009544
	adds	r5, #1
	movs	r0, #2
	bl 0x0200948c
	cmp	r5, #4
	bne.n	.L_02000f62
	movs	r2, #0
	movs	r0, #0
	movs	r1, #0
	bl 0x0200947c
	movs	r1, #2
	movs	r0, #8
	bl 0x020094cc
	movs	r0, #1
	bl 0x0200956c
	movs	r1, #136
	lsls	r1, r1, #5
	adds	r1, #16
	movs	r0, #8
	bl 0x02009564
	movs	r0, #20
	bl 0x0200948c
	movs	r1, #128
	movs	r2, #128
	lsls	r2, r2, #11
	movs	r0, #8
	lsls	r1, r1, #12
	bl 0x0200949c
	movs	r1, #8
	movs	r0, #128
	bl 0x02008478
	movs	r0, #8
	bl 0x02009494
	adds	r0, #90
	ldrb	r2, [r0, #0]
	movs	r3, #254
	ands	r3, r2
	movs	r1, #140
	strb	r3, [r0, #0]
	movs	r2, #160
	movs	r0, #8
	lsls	r1, r1, #1
	bl 0x020094a4
	movs	r1, #17
	movs	r0, #8
	bl 0x020094bc
	movs	r0, #134
	bl 0x02009594
	movs	r0, #2
	bl 0x0200948c
	movs	r1, #8
	movs	r0, #128
	bl 0x02008478
	movs	r0, #2
	bl 0x0200948c
	movs	r1, #8
	movs	r0, #128
	bl 0x02008478
	movs	r0, #56
	bl 0x0200948c
	movs	r0, #178
	bl 0x02009594
	movs	r0, #8
	bl 0x02009494
	movs	r5, #0
	adds	r0, #85
	strb	r5, [r0, #0]
.L_02001034:
	cmp	r5, #16
	bne.n	.L_02001040
	movs	r0, #8
	movs	r1, #18
	bl 0x020094bc
.L_02001040:
	movs	r0, #8
	bl 0x02009494
	movs	r1, #152
	ldr	r3, [r0, #12]
	lsls	r1, r1, #6
	adds	r1, #102
	adds	r3, r3, r1
	str	r3, [r0, #12]
	adds	r5, #1
	movs	r0, #1
	bl 0x0200948c
	cmp	r5, #64
	bne.n	.L_02001034
	movs	r1, #2
	movs	r0, #8
	bl 0x020094cc
	movs	r0, #5
	bl 0x0200948c
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #8
	movs	r1, #0
	movs	r2, #5
	bl 0x020094ec
	movs	r5, #0
.L_0200107c:
	movs	r0, #8
	bl 0x02009494
	ldr	r3, [r0, #12]
	movs	r2, #128
	lsls	r2, r2, #9
	adds	r3, r3, r2
	str	r3, [r0, #12]
	adds	r5, #1
	movs	r0, #1
	bl 0x0200948c
	cmp	r5, #32
	bne.n	.L_0200107c
	movs	r0, #8
	bl 0x02009494
	movs	r3, #1
	adds	r0, #85
	strb	r3, [r0, #0]
	movs	r5, #0
.L_020010a6:
	movs	r0, #8
	bl 0x02009494
	ldr	r1, [pc, #824]
	ldr	r3, [r0, #12]
	adds	r5, #1
	adds	r3, r3, r1
	str	r3, [r0, #12]
	movs	r0, #1
	bl 0x0200948c
	cmp	r5, #5
	bne.n	.L_020010a6
	movs	r0, #128
	lsls	r0, r0, #1
	movs	r1, #16
	bl 0x02008478
	movs	r1, #17
	movs	r0, #8
	bl 0x020094bc
	movs	r0, #145
	bl 0x02009594
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r1, r1, #13
	lsls	r2, r2, #9
	lsls	r0, r0, #13
	bl 0x0200947c
	movs	r0, #10
	bl 0x0200948c
	movs	r2, #230
	movs	r0, #1
	movs	r1, #1
	lsls	r2, r2, #8
	adds	r2, #102
	negs	r1, r1
	negs	r0, r0
	bl 0x0200947c
	movs	r0, #9
	bl 0x02009494
	movs	r1, #0
	bl 0x0200958c
	bl 0x02009584
	bl 0x0200957c
	movs	r0, #195
	lsls	r0, r0, #1
	bl 0x02009594
	movs	r1, #4
	movs	r0, #9
	bl 0x020094bc
	movs	r0, #32
	bl 0x0200956c
	movs	r0, #10
	bl 0x0200948c
	movs	r2, #5
	movs	r0, #9
	movs	r1, #0
	bl 0x020094ec
	movs	r1, #2
	movs	r0, #8
	bl 0x020094cc
	movs	r0, #5
	bl 0x0200948c
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r2, #5
	adds	r0, #8
	movs	r1, #0
	bl 0x020094ec
	movs	r0, #9
	movs	r1, #3
	bl 0x020094bc
	movs	r2, #5
	movs	r0, #9
	movs	r1, #0
	bl 0x020094ec
	movs	r1, #18
	movs	r0, #8
	bl 0x020094bc
	movs	r0, #10
	bl 0x0200948c
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r1, #0
	movs	r2, #5
	adds	r0, #8
	bl 0x020094ec
	movs	r0, #120
	bl 0x0200948c
	movs	r0, #107
	bl 0x02009594
	movs	r5, #0
.L_02001192:
	movs	r2, #128
	lsls	r2, r2, #9
	lsls	r1, r5, #16
	adds	r1, r1, r2
	adds	r0, r1, #0
	bl 0x0200947c
	adds	r5, #1
	movs	r0, #8
	bl 0x0200948c
	cmp	r5, #2
	bne.n	.L_02001192
	movs	r1, #8
	movs	r0, #9
	bl 0x020094bc
	movs	r0, #60
	bl 0x0200948c
	movs	r0, #9
	movs	r1, #7
	bl 0x020094bc
	movs	r0, #9
	movs	r1, #0
	movs	r2, #5
	bl 0x020094ec
	movs	r1, #2
	adds	r1, #255
	movs	r2, #30
	movs	r0, #8
	bl 0x0200950c
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r2, #5
	adds	r0, #8
	movs	r1, #0
	bl 0x020094ec
	movs	r0, #9
	movs	r1, #8
	bl 0x020094bc
	movs	r0, #9
	movs	r1, #0
	movs	r2, #5
	bl 0x020094ec
	movs	r6, #128
	movs	r5, #0
	lsls	r6, r6, #10
.L_020011fe:
	movs	r2, #128
	adds	r0, r6, #0
	adds	r1, r6, #0
	lsls	r2, r2, #9
	bl 0x0200947c
	movs	r0, #8
	bl 0x0200948c
	movs	r2, #128
	lsls	r2, r2, #9
	adds	r5, #1
	adds	r6, r6, r2
	cmp	r5, #4
	bne.n	.L_020011fe
	movs	r1, #2
	movs	r0, #8
	bl 0x020094cc
	movs	r0, #5
	bl 0x0200948c
	movs	r0, #128
	lsls	r0, r0, #6
	movs	r2, #5
	adds	r0, #8
	movs	r1, #0
	bl 0x020094ec
	movs	r0, #9
	movs	r1, #3
	bl 0x020094bc
	movs	r2, #5
	movs	r0, #9
	movs	r1, #0
	bl 0x020094ec
	movs	r1, #2
	movs	r0, #8
	bl 0x020094cc
	movs	r0, #5
	bl 0x0200948c
	movs	r0, #9
	movs	r1, #4
	bl 0x020094bc
	movs	r1, #0
	movs	r2, #5
	movs	r0, #9
	bl 0x020094ec
	movs	r0, #10
	bl 0x0200948c
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #32]
	movs	r0, #128
	movs	r1, #128
	movs	r2, #128
	lsls	r0, r0, #13
	lsls	r1, r1, #13
	lsls	r2, r2, #9
	mov	r8, r3
	bl 0x0200947c
	ldr	r7, [pc, #352]
	movs	r6, #0
.L_0200128c:
	cmp	r6, #20
	bne.n	.L_02001298
	movs	r0, #9
	movs	r1, #7
	bl 0x020094bc
.L_02001298:
	adds	r3, r6, #0
	subs	r3, #30
	cmp	r3, #16
	bhi.n	.L_020012c6
	movs	r0, #9
	bl 0x02009494
	movs	r5, #128
	lsls	r5, r5, #9
	subs	r3, r5, r7
	str	r3, [r0, #24]
	movs	r0, #9
	bl 0x02009494
	adds	r5, r7, r5
	str	r5, [r0, #28]
	movs	r0, #9
	bl 0x02009494
	ldr	r1, [pc, #304]
	ldr	r3, [r0, #16]
	adds	r3, r3, r1
	str	r3, [r0, #16]
.L_020012c6:
	cmp	r6, #47
	bne.n	.L_020012d4
	movs	r0, #9
	movs	r1, #0
	movs	r2, #0
	bl 0x020094b4
.L_020012d4:
	cmp	r6, #64
	bne.n	.L_020012ea
	movs	r0, #254
	lsls	r0, r0, #7
	adds	r0, #255
	movs	r1, #0
	bl 0x0200953c
	movs	r0, #64
	bl 0x02009544
.L_020012ea:
	movs	r2, #166
	lsls	r2, r2, #1
	add	r2, r8
	ldr	r3, [r2, #0]
	ldr	r1, [pc, #256]
	movs	r0, #8
	adds	r3, r3, r1
	str	r3, [r2, #0]
	bl 0x02009494
	ldr	r3, [r0, #16]
	movs	r2, #128
	lsls	r2, r2, #9
	adds	r3, r3, r2
	str	r3, [r0, #16]
	movs	r0, #1
	bl 0x02009414
	movs	r3, #128
	lsls	r3, r3, #5
	adds	r6, #1
	adds	r7, r7, r3
	cmp	r6, #128
	bne.n	.L_0200128c
	movs	r0, #78
	bl 0x02009594
	movs	r0, #32
	bl 0x0200948c
	movs	r0, #208
	bl 0x02009594
	movs	r0, #120
	bl 0x0200948c
	ldr	r0, [pc, #196]
	bl 0x02009424
	movs	r0, #254
	lsls	r0, r0, #7
	movs	r1, #0
	adds	r0, #255
	bl 0x0200953c
	movs	r0, #1
	bl 0x02009544
	movs	r0, #1
	bl 0x02009414
	movs	r0, #195
	lsls	r0, r0, #1
	bl 0x02009594
	movs	r1, #0
	movs	r0, #0
	bl 0x0200953c
	movs	r0, #120
	bl 0x02009544
	movs	r0, #150
	lsls	r0, r0, #1
	bl 0x02009414
	ldr	r0, [pc, #140]
	movs	r1, #1
	bl 0x02009534
	b.n	.L_020013d6
	.4byte 0x049b23c0
	.4byte 0x21d66edb
	.4byte 0x185b0049
	.4byte 0x601a2200
	.4byte 0xf8e0f000
	.4byte 0xf8e6f000
	.4byte 0xe00c2600
	.4byte 0xd1092e00
	.4byte 0xf0002008
	.4byte 0x2100f87b
	.4byte 0xf868f000
	.4byte 0x210c2008
	.4byte 0xf888f000
	.4byte 0x22a03601
	.4byte 0x42960052
	.4byte 0x4912d00b
	.4byte 0x680b2202
	.4byte 0x2b004013
	.4byte 0x2600d000
	.4byte 0x2201680b
	.4byte 0x2b004013
	.4byte 0x480dd0e2
	.4byte 0xf0002100
	.2byte 0xf8af
.L_020013d6:
	mov	r0, r9
	bl 0x0200944c
	movs	r0, #0
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.4byte 0xfff7ae15
	.4byte 0xfffe2000
	.4byte 0xfffe0000
	.4byte 0xffff0000
	.4byte 0x02008395
	.4byte 0x00000136
	.4byte 0x03001150
	.2byte 0x0003
	.2byte 0x0000
	.global Func_02001408
	.thumb_func
Func_02001408:
	movs	r0, #0
	bx	lr
	.section .rodata,"a",%progbits
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000016
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.4byte 0x00000011
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000002c
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.4byte 0x00000011
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000007e
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.4byte 0x00000011
	.4byte 0x0200959c
	.4byte 0x020095d8
	.4byte 0x02009614
	.4byte 0xffff0000
	.4byte 0x000000ac
	.4byte 0x40000095
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x000000ac
	.4byte 0x40000095
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x000001ff
	.4byte 0xffff0039
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x0000a000
	.4byte 0xffff0137
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x00180000
	.4byte 0x00002000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
