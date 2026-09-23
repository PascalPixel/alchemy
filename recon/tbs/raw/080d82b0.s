.syntax unified
	.thumb
	.set sub_080022fc, 0x080022fc
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
	.set sub_080b5070, 0x080b5070
	.set sub_080b5098, 0x080b5098
	.set sub_080b50e8, 0x080b50e8
	.set sub_080cd52c, 0x080cd52c
	.set sub_080cd594, 0x080cd594
	.set sub_080cdbc0, 0x080cdbc0
	.set sub_080d6888, 0x080d6888
	.set sub_080e0524, 0x080e0524
	.set sub_080e38b8, 0x080e38b8
	.set sub_080e3908, 0x080e3908
	.set sub_080e3944, 0x080e3944
	.set sub_080ed408, 0x080ed408
	.set sub_080f9010, 0x080f9010
	.global Func_080d82b0
	.thumb_func
Func_080d82b0:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r5, [pc, #732]
	adds	r3, r5, #0
	ldmia	r3!, {r1}
	sub	sp, #56
	str	r1, [sp, #28]
	ldr	r3, [r3, #0]
	str	r3, [sp, #24]
	ldr	r2, [r5, #8]
	ldr	r4, [pc, #720]
	str	r2, [sp, #16]
	adds	r3, r5, #0
	subs	r3, #108
	ldr	r6, [r3, #0]
	adds	r3, r1, r4
	str	r0, [r3, #0]
	movs	r0, #1
	bl	sub_080cd594
	movs	r2, #0
	ldr	r1, [sp, #16]
	movs	r3, #0
	ldr	r0, [pc, #696]
	bl	sub_080e0524
	ldr	r0, [pc, #696]
	bl	sub_08002f40
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #688]
	movs	r2, #128
	lsls	r0, r0, #19
	bl	sub_080072f0
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #7
	movs	r3, #3
	movs	r0, #46
	bl	sub_080ed408
	ldr	r5, [r5, #28]
	movs	r0, #0
	movs	r1, #1
	movs	r2, #128
	str	r5, [sp, #20]
	ldr	r3, [pc, #656]
	mov	r8, r0
	negs	r1, r1
	lsls	r2, r2, #3
.L_080d8324:
	movs	r4, #1
	add	r8, r4
	str	r1, [r3, #0]
	adds	r3, #28
	cmp	r8, r2
	bne.n	.L_080d8324
	bl	sub_080049ac
	adds	r1, r6, #0
	adds	r1, #12
	adds	r0, r6, #0
	bl	sub_080051d8
	ldr	r2, [pc, #608]
	ldr	r1, [sp, #28]
	ldr	r3, [r1, r2]
	ldr	r3, [r3, #20]
	movs	r0, #0
	mov	fp, r0
	cmp	r3, #0
	beq.n	.L_080d8436
	movs	r3, #44
	movs	r4, #32
	movs	r0, #36
	movs	r1, #0
	add	r3, sp
	add	r4, sp
	str	r0, [sp, #12]
	str	r1, [sp, #8]
	mov	r9, r3
	mov	sl, r4
.L_080d8362:
	ldr	r3, [sp, #28]
	adds	r5, r3, r2
	ldr	r4, [sp, #12]
	ldr	r3, [r5, #0]
	ldrsh	r0, [r3, r4]
	bl	sub_080b5098
	ldr	r3, [r5, #0]
	ldr	r2, [sp, #12]
	ldr	r6, [r0, #0]
	ldrsh	r0, [r3, r2]
	bl	sub_080b5070
	lsrs	r3, r0, #31
	adds	r0, r0, r3
	ldr	r3, [r6, #8]
	mov	r1, r9
	asrs	r0, r0, #1
	str	r0, [r1, #4]
	str	r3, [r1, #0]
	ldr	r3, [r6, #16]
	mov	r0, r9
	str	r3, [r1, #8]
	mov	r1, sl
	bl	sub_080e3944
	mov	r2, sl
	ldr	r3, [r2, #0]
	asrs	r3, r3, #1
	str	r3, [r2, #0]
	ldr	r4, [sp, #8]
	ldr	r0, [pc, #528]
	movs	r3, #0
	mov	r8, r3
	adds	r7, r4, r0
.L_080d83a8:
	bl	sub_08004458
	movs	r1, #255
	adds	r6, r0, #0
	ands	r6, r1
	bl	sub_08004458
	ldr	r3, [pc, #512]
	adds	r5, r0, #0
	ands	r5, r3
	adds	r0, r5, #0
	bl	sub_08002322
	adds	r2, r6, #0
	muls	r2, r0
	mov	r4, sl
	ldr	r3, [r4, #0]
	asrs	r2, r2, #7
	lsls	r3, r3, #16
	adds	r2, r2, r3
	str	r2, [r7, #0]
	adds	r0, r5, #0
	bl	sub_0800231c
	adds	r2, r6, #0
	muls	r2, r0
	mov	r0, sl
	ldr	r3, [r0, #4]
	asrs	r2, r2, #3
	lsls	r3, r3, #16
	adds	r2, r2, r3
	str	r2, [r7, #4]
	bl	sub_08004458
	movs	r1, #255
	ands	r0, r1
	movs	r3, #128
	subs	r3, r3, r0
	lsls	r3, r3, #9
	str	r3, [r7, #12]
	bl	sub_08004458
	movs	r2, #255
	movs	r3, #0
	ands	r0, r2
	str	r3, [r7, #24]
	negs	r0, r0
	movs	r3, #1
	subs	r0, #128
	add	r8, r3
	lsls	r0, r0, #10
	mov	r4, r8
	str	r0, [r7, #16]
	adds	r7, #28
	cmp	r4, #128
	bne.n	.L_080d83a8
	ldr	r0, [sp, #12]
	ldr	r1, [sp, #8]
	movs	r2, #224
	lsls	r2, r2, #4
	adds	r1, r1, r2
	adds	r0, #2
	str	r0, [sp, #12]
	str	r1, [sp, #8]
	ldr	r2, [pc, #372]
	ldr	r4, [sp, #28]
	add	fp, r3
	ldr	r3, [r4, r2]
	ldr	r3, [r3, #20]
	cmp	fp, r3
	bne.n	.L_080d8362
.L_080d8436:
	ldr	r0, [sp, #28]
	movs	r1, #239
	lsls	r1, r1, #7
	adds	r2, r0, r1
	movs	r3, #2
	str	r3, [r2, #0]
	ldr	r3, [pc, #376]
	movs	r1, #144
	adds	r2, r0, r3
	movs	r3, #50
	str	r3, [r2, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #368]
	bl	sub_080041d8
	ldr	r1, [pc, #328]
	ldr	r0, [sp, #28]
	adds	r3, r0, r1
	ldr	r3, [r3, #0]
	ldr	r2, [r3, #20]
	lsls	r3, r2, #2
	adds	r3, r3, r2
	movs	r2, #56
	movs	r4, #0
	lsls	r3, r3, #2
	negs	r2, r2
	mov	r9, r4
	cmp	r3, r2
	bne.n	.L_080d8472
	b.n	.L_080d8578
.L_080d8472:
	adds	r6, r0, r1
.L_080d8474:
	mov	r3, r9
	cmp	r3, #32
	bne.n	.L_080d8480
	movs	r0, #0
	bl	sub_080b50e8
.L_080d8480:
	ldr	r3, [r6, #0]
	ldr	r3, [r3, #20]
	movs	r4, #0
	mov	fp, r4
	cmp	r3, #0
	beq.n	.L_080d854e
	movs	r0, #0
	movs	r7, #0
	mov	sl, r0
.L_080d8492:
	cmp	r9, r7
	bne.n	.L_080d84b6
	movs	r0, #143
	bl	sub_080f9010
	mov	r1, fp
	ldr	r2, [r6, #0]
	lsls	r3, r1, #1
	adds	r3, #36
	ldrsh	r0, [r2, r3]
	movs	r3, #20
	movs	r2, #1
	str	r3, [sp, #0]
	movs	r1, #7
	negs	r2, r2
	mov	r3, fp
	bl	sub_080d6888
.L_080d84b6:
	cmp	r9, r7
	ble.n	.L_080d853a
	ldr	r5, [pc, #248]
	movs	r0, #0
	mov	r8, r0
	add	r5, sl
.L_080d84c2:
	ldr	r3, [r5, #24]
	cmp	r3, #0
	blt.n	.L_080d852e
	movs	r1, #3
	mov	r0, r8
	bl	sub_080022fc
	adds	r0, #1
	lsls	r4, r0, #1
	ldr	r2, [pc, #236]
	subs	r3, r4, #2
	ldrh	r1, [r2, r3]
	ldr	r2, [sp, #16]
	adds	r1, r2, r1
	movs	r3, #2
	ldrsh	r2, [r5, r3]
	lsrs	r3, r0, #31
	adds	r3, r0, r3
	asrs	r3, r3, #1
	subs	r2, r2, r3
	mov	lr, r2
	movs	r3, #6
	ldrsh	r2, [r5, r3]
	str	r0, [sp, #0]
	subs	r3, r2, r0
	str	r4, [sp, #4]
	mov	r2, lr
	ldr	r0, [sp, #24]
	ldr	r4, [sp, #20]
	bl	sub_080072f4
	movs	r0, #3
	mov	r3, r8
	ldr	r2, [pc, #192]
	ands	r3, r0
	lsls	r3, r3, #2
	ldr	r2, [r2, r3]
	adds	r0, r5, #0
	movs	r1, #62
	bl	sub_080e3908
	ldr	r3, [r5, #24]
	adds	r3, #1
	str	r3, [r5, #24]
	ldr	r3, [r5, #16]
	cmp	r3, #0
	ble.n	.L_080d852e
	movs	r1, #6
	ldrsh	r3, [r5, r1]
	cmp	r3, #112
	ble.n	.L_080d852e
	movs	r3, #1
	negs	r3, r3
	str	r3, [r5, #24]
.L_080d852e:
	movs	r2, #1
	add	r8, r2
	mov	r3, r8
	adds	r5, #28
	cmp	r3, #128
	bne.n	.L_080d84c2
.L_080d853a:
	ldr	r3, [r6, #0]
	movs	r4, #224
	movs	r0, #1
	ldr	r3, [r3, #20]
	lsls	r4, r4, #4
	add	fp, r0
	adds	r7, #20
	add	sl, r4
	cmp	fp, r3
	bne.n	.L_080d8492
.L_080d854e:
	bl	sub_080cd52c
	ldr	r3, [pc, #120]
	ldr	r1, [sp, #28]
	adds	r2, r1, r3
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_080030f8
	ldr	r3, [r6, #0]
	ldr	r2, [r3, #20]
	lsls	r3, r2, #2
	adds	r3, r3, r2
	movs	r4, #1
	lsls	r3, r3, #2
	add	r9, r4
	adds	r3, #56
	cmp	r9, r3
	beq.n	.L_080d8578
	b.n	.L_080d8474
.L_080d8578:
	ldr	r0, [pc, #68]
	bl	sub_08004278
	movs	r0, #46
	bl	sub_08002dd8
	bl	sub_080cdbc0
	add	sp, #56
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
	.4byte 0x00000073
	.4byte 0x000000b9
	.4byte 0x03001388
	.4byte 0x02010018
	.4byte 0x02010000
	.4byte 0x0000ffff
	.4byte 0x00007784
	.4byte 0x080cd261
	.4byte 0x080ede48
	.4byte 0x080ee9f8
	.2byte 0x7824
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r2, [pc, #808]
	adds	r3, r2, #0
	ldmia	r3!, {r1}
	ldr	r3, [r3, #0]
	sub	sp, #64
	str	r3, [sp, #48]
	ldr	r3, [r2, #8]
	str	r3, [sp, #40]
	subs	r2, #108
	ldr	r2, [r2, #0]
	str	r2, [sp, #36]
	ldr	r3, [r0, #24]
	negs	r5, r3
	orrs	r5, r3
	ldr	r3, [pc, #784]
	mov	r9, r1
	add	r3, r9
	str	r0, [r3, #0]
	movs	r0, #1
	bl	sub_080cd594
	lsrs	r5, r5, #31
	ldr	r0, [pc, #772]
	ldr	r1, [sp, #40]
	movs	r2, #0
	movs	r3, #0
	bl	sub_080e0524
	cmp	r5, #0
	bne.n	.L_080d861e
	ldr	r0, [pc, #760]
	b.n	.L_080d8620
.L_080d861e:
	ldr	r0, [pc, #760]
.L_080d8620:
	bl	sub_08002f40
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #752]
	movs	r2, #128
	lsls	r0, r0, #19
	bl	sub_080072f0
	movs	r1, #1
	movs	r2, #128
	ldr	r3, [pc, #744]
	movs	r7, #0
	negs	r1, r1
	lsls	r2, r2, #3
.L_080d863e:
	adds	r7, #1
	str	r1, [r3, #0]
	adds	r3, #28
	cmp	r7, r2
	bne.n	.L_080d863e
	ldr	r2, [pc, #704]
	mov	r0, r9
	ldr	r3, [r0, r2]
	ldr	r3, [r3, #20]
	movs	r4, #0
	mov	r8, r4
	cmp	r3, #0
	beq.n	.L_080d86f0
	movs	r3, #36
	movs	r1, #255
	str	r3, [sp, #24]
	str	r4, [sp, #16]
	mov	fp, r1
.L_080d8662:
	mov	r4, r9
	adds	r5, r4, r2
	ldr	r3, [r5, #0]
	ldr	r1, [sp, #24]
	ldrsh	r0, [r3, r1]
	bl	sub_080b5098
	ldr	r3, [r5, #0]
	ldr	r4, [sp, #24]
	ldr	r6, [r0, #0]
	ldrsh	r0, [r3, r4]
	bl	sub_080b5070
	lsrs	r3, r0, #31
	adds	r0, r0, r3
	ldr	r2, [sp, #16]
	ldr	r3, [pc, #672]
	asrs	r0, r0, #1
	mov	sl, r0
	movs	r7, #0
	adds	r5, r2, r3
.L_080d868c:
	ldr	r3, [r6, #8]
	str	r3, [r5, #0]
	ldr	r3, [r6, #12]
	add	r3, sl
	str	r3, [r5, #4]
	ldr	r3, [r6, #16]
	str	r3, [r5, #8]
	bl	sub_08004458
	mov	r4, fp
	ands	r0, r4
	subs	r0, #128
	lsls	r0, r0, #10
	str	r0, [r5, #12]
	bl	sub_08004458
	mov	r1, fp
	ands	r0, r1
	subs	r0, #128
	lsls	r0, r0, #10
	str	r0, [r5, #16]
	bl	sub_08004458
	mov	r2, fp
	ands	r0, r2
	subs	r0, #128
	lsls	r0, r0, #10
	movs	r3, #0
	adds	r7, #1
	str	r0, [r5, #20]
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	r7, #128
	bne.n	.L_080d868c
	ldr	r3, [sp, #24]
	ldr	r4, [sp, #16]
	movs	r0, #224
	lsls	r0, r0, #4
	adds	r3, #2
	adds	r4, r4, r0
	str	r4, [sp, #16]
	str	r3, [sp, #24]
	ldr	r2, [pc, #552]
	mov	r4, r9
	ldr	r3, [r4, r2]
	movs	r1, #1
	ldr	r3, [r3, #20]
	add	r8, r1
	cmp	r8, r3
	bne.n	.L_080d8662
.L_080d86f0:
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #7
	movs	r3, #3
	movs	r0, #46
	bl	sub_080ed408
	ldr	r3, [pc, #548]
	adds	r3, #184
	ldr	r3, [r3, #0]
	movs	r2, #239
	lsls	r2, r2, #7
	str	r3, [sp, #44]
	add	r2, r9
	movs	r3, #3
	str	r3, [r2, #0]
	ldr	r2, [pc, #536]
	ldr	r3, [pc, #536]
	add	r2, r9
	movs	r1, #144
	str	r3, [r2, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #532]
	bl	sub_080041d8
	movs	r0, #142
	bl	sub_080f9010
	ldr	r1, [pc, #480]
	mov	r2, r9
	ldr	r3, [r2, r1]
	ldr	r2, [r3, #20]
	lsls	r3, r2, #2
	adds	r3, r3, r2
	movs	r4, #72
	movs	r0, #0
	lsls	r3, r3, #2
	negs	r4, r4
	mov	fp, r0
	cmp	r3, r4
	bne.n	.L_080d8746
	b.n	.L_080d88e4
.L_080d8746:
	ldr	r0, [sp, #36]
	adds	r0, #12
	str	r0, [sp, #28]
.L_080d874c:
	mov	r2, r9
	adds	r5, r2, r1
	ldr	r3, [r5, #0]
	ldr	r0, [r3, #8]
	bl	sub_080b5098
	ldr	r3, [r5, #0]
	ldr	r0, [r0, #0]
	mov	sl, r0
	ldr	r0, [r3, #8]
	bl	sub_080b5070
	lsrs	r3, r0, #31
	adds	r0, r0, r3
	asrs	r0, r0, #1
	mov	r3, fp
	str	r0, [sp, #32]
	cmp	r3, #64
	bne.n	.L_080d8778
	movs	r0, #133
	bl	sub_080b50e8
.L_080d8778:
	bl	sub_080049ac
	ldr	r0, [sp, #36]
	ldr	r1, [sp, #28]
	bl	sub_080051d8
	mov	r4, fp
	cmp	r4, #40
	bne.n	.L_080d879e
	ldr	r3, [r5, #0]
	movs	r2, #1
	ldr	r0, [r3, #8]
	movs	r3, #0
	str	r3, [sp, #0]
	movs	r1, #7
	negs	r2, r2
	subs	r3, #1
	bl	sub_080d6888
.L_080d879e:
	ldr	r1, [r5, #0]
	ldr	r2, [r1, #20]
	lsls	r3, r2, #2
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r3, #52
	cmp	fp, r3
	bne.n	.L_080d87c2
	movs	r3, #0
	movs	r2, #1
	ldr	r0, [r1, #8]
	str	r3, [sp, #0]
	movs	r1, #0
	negs	r2, r2
	subs	r3, #1
	bl	sub_080d6888
	ldr	r1, [r5, #0]
.L_080d87c2:
	ldr	r3, [r1, #20]
	movs	r0, #0
	mov	r8, r0
	cmp	r3, #0
	beq.n	.L_080d88b8
	str	r0, [sp, #8]
	str	r0, [sp, #20]
.L_080d87d0:
	ldr	r1, [sp, #8]
	cmp	fp, r1
	bne.n	.L_080d87f2
	ldr	r3, [pc, #308]
	mov	r4, r8
	add	r3, r9
	ldr	r2, [r3, #0]
	lsls	r3, r4, #1
	adds	r3, #36
	ldrsh	r0, [r2, r3]
	movs	r3, #42
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #5
	mov	r3, r8
	bl	sub_080d6888
.L_080d87f2:
	ldr	r2, [sp, #8]
	cmp	fp, r2
	ble.n	.L_080d8898
	ldr	r3, [sp, #20]
	ldr	r4, [pc, #296]
	movs	r7, #0
	add	r6, sp, #52
	adds	r5, r3, r4
.L_080d8802:
	ldr	r3, [r5, #24]
	cmp	r3, #0
	blt.n	.L_080d8890
	adds	r1, r6, #0
	adds	r0, r5, #0
	bl	sub_080e3944
	ldr	r2, [r6, #0]
	ldr	r3, [pc, #292]
	asrs	r2, r2, #1
	str	r2, [r6, #0]
	ldrh	r1, [r3, #10]
	ldr	r0, [sp, #40]
	ldr	r3, [r6, #4]
	adds	r1, r0, r1
	movs	r4, #6
	movs	r0, #12
	subs	r3, #6
	str	r4, [sp, #0]
	str	r0, [sp, #4]
	subs	r2, #3
	ldr	r0, [sp, #48]
	ldr	r4, [sp, #44]
	bl	sub_080072f4
	adds	r0, r5, #0
	movs	r1, #62
	movs	r2, #0
	bl	sub_080e38b8
	ldr	r0, [sp, #8]
	adds	r3, r0, r7
	adds	r3, #10
	cmp	fp, r3
	ble.n	.L_080d8890
	mov	r1, sl
	ldr	r0, [r1, #8]
	ldr	r3, [r5, #0]
	ldr	r2, [sp, #32]
	ldr	r1, [r1, #12]
	subs	r0, r0, r3
	ldr	r3, [r5, #4]
	adds	r1, r1, r2
	subs	r1, r1, r3
	mov	r3, sl
	ldr	r2, [r3, #16]
	ldr	r3, [r5, #8]
	subs	r2, r2, r3
	ldr	r3, [r5, #12]
	asrs	r0, r0, #8
	adds	r3, r3, r0
	str	r3, [r5, #12]
	ldr	r3, [r5, #16]
	asrs	r1, r1, #8
	adds	r3, r3, r1
	str	r3, [r5, #16]
	ldr	r4, [pc, #200]
	ldr	r3, [r5, #20]
	asrs	r2, r2, #8
	ldr	r1, [pc, #196]
	adds	r3, r3, r2
	adds	r0, r0, r4
	str	r3, [r5, #20]
	cmp	r0, r1
	bhi.n	.L_080d8890
	adds	r3, r2, r4
	cmp	r3, r1
	bhi.n	.L_080d8890
	movs	r0, #1
	negs	r0, r0
	str	r0, [r5, #24]
.L_080d8890:
	adds	r7, #1
	adds	r5, #28
	cmp	r7, #32
	bne.n	.L_080d8802
.L_080d8898:
	ldr	r1, [sp, #8]
	ldr	r2, [sp, #20]
	movs	r3, #224
	lsls	r3, r3, #4
	adds	r2, r2, r3
	adds	r1, #20
	ldr	r3, [pc, #100]
	str	r1, [sp, #8]
	str	r2, [sp, #20]
	add	r3, r9
	ldr	r3, [r3, #0]
	movs	r4, #1
	ldr	r3, [r3, #20]
	add	r8, r4
	cmp	r8, r3
	bne.n	.L_080d87d0
.L_080d88b8:
	bl	sub_080cd52c
	ldr	r2, [pc, #132]
	movs	r3, #1
	add	r2, r9
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_080030f8
	ldr	r1, [pc, #64]
	mov	r2, r9
	ldr	r3, [r2, r1]
	ldr	r2, [r3, #20]
	lsls	r3, r2, #2
	adds	r3, r3, r2
	movs	r0, #1
	lsls	r3, r3, #2
	add	fp, r0
	adds	r3, #72
	cmp	fp, r3
	beq.n	.L_080d88e4
	b.n	.L_080d874c
.L_080d88e4:
	ldr	r0, [pc, #76]
	bl	sub_08004278
	movs	r0, #46
	bl	sub_08002dd8
	bl	sub_080cdbc0
	add	sp, #64
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
	.4byte 0x00000073
	.4byte 0x000000b9
	.4byte 0x000000c0
	.4byte 0x03001388
	.4byte 0x02010018
	.4byte 0x02010000
	.4byte 0x03001e50
	.4byte 0x00007784
	.4byte 0x04040404
	.4byte 0x080cd261
	.4byte 0x080ede48
	.4byte 0x00000fff
	.4byte 0x00001ffe
	.4byte 0x00007824
