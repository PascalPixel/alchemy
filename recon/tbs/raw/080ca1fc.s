.syntax unified
	.thumb
	.set sub_0800231c, 0x0800231c
	.set sub_08002322, 0x08002322
	.set sub_08002dd8, 0x08002dd8
	.set sub_08002f40, 0x08002f40
	.set sub_080030f8, 0x080030f8
	.set sub_080041d8, 0x080041d8
	.set sub_08004278, 0x08004278
	.set sub_08004458, 0x08004458
	.set sub_080049ac, 0x080049ac
	.set sub_080051d8, 0x080051d8
	.set sub_080072f0, 0x080072f0
	.set sub_080072f4, 0x080072f4
	.set sub_080b5098, 0x080b5098
	.set sub_080b50e8, 0x080b50e8
	.set sub_080cd52c, 0x080cd52c
	.set sub_080cd594, 0x080cd594
	.set sub_080cdbc0, 0x080cdbc0
	.set sub_080d6888, 0x080d6888
	.set sub_080e0524, 0x080e0524
	.set sub_080e38b8, 0x080e38b8
	.set sub_080e3944, 0x080e3944
	.set sub_080ed408, 0x080ed408
	.set sub_080f9010, 0x080f9010
	.global Func_080ca1fc
	.thumb_func
Func_080ca1fc:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #68
	ldr	r2, [pc, #804]
	str	r1, [sp, #40]
	adds	r3, r2, #0
	ldmia	r3!, {r1}
	str	r1, [sp, #36]
	ldr	r3, [r3, #0]
	str	r3, [sp, #32]
	adds	r3, r2, #0
	subs	r3, #108
	ldr	r3, [r3, #0]
	str	r3, [sp, #20]
	ldr	r2, [r2, #8]
	str	r2, [sp, #16]
	ldr	r2, [pc, #784]
	adds	r3, r1, r2
	str	r0, [r3, #0]
	ldr	r3, [sp, #40]
	cmp	r3, #0
	bne.n	.L_080ca23a
	movs	r0, #0
	bl	sub_080cd594
	b.n	.L_080ca240
.L_080ca23a:
	movs	r0, #1
	bl	sub_080cd594
.L_080ca240:
	movs	r6, #2
	movs	r1, #7
	movs	r2, #7
	movs	r3, #3
	movs	r0, #46
	str	r6, [sp, #0]
	bl	sub_080ed408
	ldr	r5, [pc, #744]
	adds	r3, r5, #0
	adds	r3, #184
	ldr	r3, [r3, #0]
	movs	r1, #7
	str	r3, [sp, #24]
	movs	r2, #7
	movs	r3, #11
	movs	r0, #47
	str	r6, [sp, #0]
	bl	sub_080ed408
	adds	r5, #188
	ldr	r5, [r5, #0]
	ldr	r0, [pc, #720]
	ldr	r1, [sp, #16]
	movs	r2, #0
	movs	r3, #0
	str	r5, [sp, #28]
	bl	sub_080e0524
	ldr	r4, [sp, #40]
	cmp	r4, #0
	bne.n	.L_080ca284
	ldr	r0, [pc, #704]
	b.n	.L_080ca286
.L_080ca284:
	ldr	r0, [pc, #704]
.L_080ca286:
	bl	sub_08002f40
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #700]
	movs	r2, #128
	lsls	r0, r0, #19
	bl	sub_080072f0
	movs	r3, #239
	ldr	r1, [sp, #36]
	ldr	r4, [pc, #688]
	lsls	r3, r3, #7
	adds	r2, r1, r3
	movs	r3, #2
	str	r3, [r2, #0]
	adds	r2, r1, r4
	movs	r3, #75
	movs	r1, #144
	str	r3, [r2, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #672]
	bl	sub_080041d8
	ldr	r2, [pc, #640]
	ldr	r1, [sp, #36]
	adds	r3, r1, r2
	ldr	r3, [r3, #0]
	ldr	r0, [r3, #8]
	bl	sub_080b5098
	ldr	r0, [r0, #0]
	movs	r3, #0
	str	r0, [sp, #12]
	ldr	r7, [pc, #652]
	mov	r8, r3
.L_080ca2ce:
	bl	sub_08004458
	ldr	r5, [pc, #648]
	ands	r5, r0
	bl	sub_08004458
	ldr	r3, [pc, #644]
	ldr	r4, [sp, #12]
	adds	r6, r0, #0
	ands	r6, r3
	ldr	r3, [r4, #8]
	str	r3, [r7, #0]
	movs	r1, #160
	ldr	r3, [r4, #12]
	lsls	r1, r1, #11
	adds	r3, r3, r1
	str	r3, [r7, #4]
	ldr	r3, [r4, #16]
	adds	r0, r6, #0
	str	r3, [r7, #8]
	bl	sub_08002322
	adds	r5, #32
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #8
	str	r3, [r7, #12]
	bl	sub_08004458
	movs	r3, #255
	ands	r3, r0
	subs	r3, #32
	lsls	r3, r3, #9
	str	r3, [r7, #16]
	adds	r0, r6, #0
	bl	sub_0800231c
	adds	r3, r5, #0
	muls	r3, r0
	lsls	r3, r3, #1
	negs	r3, r3
	asrs	r3, r3, #8
	str	r3, [r7, #20]
	bl	sub_08004458
	movs	r3, #31
	ands	r3, r0
	adds	r3, #48
	str	r3, [r7, #24]
	ldr	r2, [sp, #40]
	cmp	r2, #0
	bne.n	.L_080ca34a
	ldr	r3, [r7, #12]
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	str	r3, [r7, #12]
	ldr	r3, [r7, #20]
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	str	r3, [r7, #20]
.L_080ca34a:
	movs	r3, #1
	movs	r4, #128
	add	r8, r3
	lsls	r4, r4, #1
	adds	r7, #28
	cmp	r8, r4
	bne.n	.L_080ca2ce
	ldr	r2, [sp, #20]
	movs	r3, #56
	adds	r2, #12
	movs	r1, #0
	add	r3, sp
	str	r2, [sp, #8]
	mov	fp, r1
	mov	r9, r3
.L_080ca368:
	bl	sub_080049ac
	ldr	r0, [sp, #20]
	ldr	r1, [sp, #8]
	bl	sub_080051d8
	ldr	r6, [pc, #480]
	movs	r4, #0
	mov	r8, r4
.L_080ca37a:
	mov	r3, r8
	cmp	r3, #0
	bge.n	.L_080ca382
	adds	r3, #31
.L_080ca382:
	asrs	r3, r3, #5
	lsls	r3, r3, #3
	cmp	fp, r3
	blt.n	.L_080ca440
	ldr	r3, [r6, #24]
	cmp	r3, #0
	blt.n	.L_080ca440
	mov	r1, r8
	lsls	r0, r1, #2
	adds	r0, r0, r3
	lsls	r0, r0, #10
	bl	sub_08002322
	ldr	r3, [r6, #0]
	lsls	r0, r0, #4
	mov	r2, r9
	adds	r3, r3, r0
	str	r3, [r2, #0]
	ldr	r3, [r6, #4]
	str	r3, [r2, #4]
	ldr	r3, [r6, #8]
	add	r5, sp, #44
	str	r3, [r2, #8]
	mov	r0, r9
	adds	r1, r5, #0
	bl	sub_080e3944
	ldr	r3, [r5, #0]
	asrs	r3, r3, #1
	str	r3, [r5, #0]
	ldr	r4, [pc, #420]
	ldr	r3, [r5, #8]
	cmp	r3, r4
	bgt.n	.L_080ca3cc
	movs	r3, #157
	lsls	r3, r3, #1
	str	r3, [r5, #8]
.L_080ca3cc:
	ldr	r2, [pc, #408]
	cmp	r3, r2
	ble.n	.L_080ca3d6
	str	r2, [r5, #8]
	adds	r3, r2, #0
.L_080ca3d6:
	ldr	r1, [pc, #404]
	adds	r2, r3, r1
	cmp	r2, #0
	bge.n	.L_080ca3e2
	adds	r2, r3, #0
	subs	r2, #251
.L_080ca3e2:
	asrs	r3, r2, #6
	movs	r0, #6
	subs	r0, r0, r3
	lsls	r4, r0, #1
	ldr	r2, [pc, #388]
	subs	r3, r4, #2
	ldrh	r1, [r2, r3]
	ldr	r2, [sp, #16]
	lsrs	r3, r0, #31
	adds	r1, r2, r1
	adds	r3, r0, r3
	ldr	r2, [r5, #0]
	asrs	r3, r3, #1
	subs	r2, r2, r3
	ldr	r3, [r5, #4]
	str	r0, [sp, #0]
	subs	r3, r3, r0
	str	r4, [sp, #4]
	ldr	r0, [sp, #32]
	ldr	r4, [sp, #24]
	bl	sub_080072f4
	movs	r2, #128
	movs	r1, #62
	adds	r0, r6, #0
	lsls	r2, r2, #3
	bl	sub_080e38b8
	ldr	r1, [sp, #40]
	cmp	r1, #1
	bne.n	.L_080ca43a
	ldr	r2, [sp, #12]
	ldr	r3, [r2, #8]
	cmp	r3, #0
	bge.n	.L_080ca432
	ldr	r3, [r6, #12]
	movs	r4, #128
	lsls	r4, r4, #6
	adds	r3, r3, r4
	b.n	.L_080ca438
.L_080ca432:
	ldr	r3, [r6, #12]
	ldr	r1, [pc, #316]
	adds	r3, r3, r1
.L_080ca438:
	str	r3, [r6, #12]
.L_080ca43a:
	ldr	r3, [r6, #24]
	subs	r3, #1
	str	r3, [r6, #24]
.L_080ca440:
	movs	r2, #1
	add	r8, r2
	mov	r3, r8
	adds	r6, #28
	cmp	r3, #128
	bne.n	.L_080ca37a
	ldr	r4, [sp, #40]
	cmp	r4, #1
	bne.n	.L_080ca49a
	ldr	r3, [sp, #36]
	ldr	r4, [pc, #224]
	adds	r2, r3, r4
	ldr	r3, [r2, #0]
	ldr	r3, [r3, #20]
	movs	r1, #0
	mov	r8, r1
	cmp	r3, #0
	beq.n	.L_080ca4ea
	adds	r5, r2, #0
	movs	r7, #36
	movs	r6, #48
.L_080ca46a:
	cmp	fp, r6
	bne.n	.L_080ca488
	movs	r0, #1
	negs	r0, r0
	bl	sub_080b50e8
	ldr	r3, [r5, #0]
	ldrsh	r0, [r3, r7]
	movs	r3, #8
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #5
	mov	r3, r8
	bl	sub_080d6888
.L_080ca488:
	ldr	r3, [r5, #0]
	movs	r2, #1
	ldr	r3, [r3, #20]
	add	r8, r2
	adds	r7, #2
	adds	r6, #8
	cmp	r8, r3
	bne.n	.L_080ca46a
	b.n	.L_080ca4ea
.L_080ca49a:
	ldr	r4, [sp, #36]
	ldr	r1, [pc, #152]
	movs	r3, #0
	adds	r2, r4, r1
	mov	r8, r3
	ldr	r3, [r2, #0]
	ldr	r3, [r3, #20]
	cmp	r3, #0
	beq.n	.L_080ca4ea
	movs	r3, #1
	negs	r3, r3
	mov	sl, r3
	adds	r5, r2, #0
	movs	r7, #36
	movs	r6, #48
.L_080ca4b8:
	cmp	fp, r6
	bne.n	.L_080ca4da
	movs	r0, #126
	bl	sub_080f9010
	mov	r0, sl
	bl	sub_080b50e8
	ldr	r3, [r5, #0]
	ldrsh	r0, [r3, r7]
	movs	r3, #8
	str	r3, [sp, #0]
	movs	r1, #7
	mov	r2, sl
	mov	r3, r8
	bl	sub_080d6888
.L_080ca4da:
	ldr	r3, [r5, #0]
	movs	r1, #1
	ldr	r3, [r3, #20]
	add	r8, r1
	adds	r7, #2
	adds	r6, #8
	cmp	r8, r3
	bne.n	.L_080ca4b8
.L_080ca4ea:
	bl	sub_080cd52c
	ldr	r4, [pc, #136]
	ldr	r3, [sp, #36]
	adds	r2, r3, r4
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_080030f8
	movs	r1, #1
	add	fp, r1
	mov	r2, fp
	cmp	r2, #128
	beq.n	.L_080ca50a
	b.n	.L_080ca368
.L_080ca50a:
	ldr	r0, [pc, #72]
	bl	sub_08004278
	movs	r0, #47
	bl	sub_08002dd8
	movs	r0, #46
	bl	sub_08002dd8
	bl	sub_080cdbc0
	add	sp, #68
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001eec
	.4byte 0x00007828
	.4byte 0x03001e50
	.4byte 0x00000073
	.4byte 0x0000007c
	.4byte 0x0000007b
	.4byte 0x03001388
	.4byte 0x00007784
	.4byte 0x080cd261
	.4byte 0x02010000
	.4byte 0x000003ff
	.4byte 0x0000ffff
	.4byte 0x00000139
	.4byte 0x0000027a
	.4byte 0xfffffec6
	.4byte 0x080ede48
	.4byte 0xffffe000
	.4byte 0x00007824
