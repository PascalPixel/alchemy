.syntax unified
	.thumb
	.set sub_080022ec, 0x080022ec
	.set sub_0800231c, 0x0800231c
	.set sub_08002322, 0x08002322
	.set sub_08002dd8, 0x08002dd8
	.set sub_08002f40, 0x08002f40
	.set sub_080030f8, 0x080030f8
	.set sub_080041d8, 0x080041d8
	.set sub_08004278, 0x08004278
	.set sub_08004458, 0x08004458
	.set sub_080049ac, 0x080049ac
	.set sub_080049e8, 0x080049e8
	.set sub_08004a5c, 0x08004a5c
	.set sub_08004bd4, 0x08004bd4
	.set sub_08004c1c, 0x08004c1c
	.set sub_08004c6c, 0x08004c6c
	.set sub_08004cb4, 0x08004cb4
	.set sub_080051d8, 0x080051d8
	.set sub_08005340, 0x08005340
	.set sub_080072f0, 0x080072f0
	.set sub_080072f4, 0x080072f4
	.set sub_080072f8, 0x080072f8
	.set sub_080b5098, 0x080b5098
	.set sub_080cd594, 0x080cd594
	.set sub_080d6888, 0x080d6888
	.set sub_080e0524, 0x080e0524
	.set sub_080e38b8, 0x080e38b8
	.set sub_080e3908, 0x080e3908
	.set sub_080e3944, 0x080e3944
	.set sub_080ed408, 0x080ed408
	.set sub_080f9010, 0x080f9010
	.global BattleFx_PrepareCanvasEffect
	.global Func_080de2f8
	.thumb_func
BattleFx_PrepareCanvasEffect:
Func_080de2f8:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #108
	adds	r6, r2, #0
	ldr	r2, [pc, #524]
	str	r3, [sp, #44]
	adds	r3, r2, #0
	adds	r5, r1, #0
	ldmia	r3!, {r1}
	str	r1, [sp, #40]
	ldr	r3, [r3, #0]
	str	r3, [sp, #36]
	ldr	r2, [r2, #8]
	str	r2, [sp, #24]
	ldr	r2, [pc, #508]
	adds	r3, r1, r2
	str	r0, [r3, #0]
	movs	r0, #0
	bl	sub_080cd594
	ldr	r4, [pc, #500]
	ldr	r3, [sp, #40]
	adds	r2, r3, r4
	movs	r3, #24
	str	r3, [r2, #0]
	ldr	r7, [sp, #40]
	ldr	r1, [pc, #492]
	movs	r3, #0
	adds	r2, r7, r1
	str	r3, [r2, #0]
	cmp	r5, #3
	ble.n	.L_080de34a
	movs	r2, #84
	subs	r5, #4
	str	r2, [sp, #20]
	b.n	.L_080de34e
.L_080de34a:
	movs	r3, #64
	str	r3, [sp, #20]
.L_080de34e:
	cmp	r5, #1
	beq.n	.L_080de366
	cmp	r5, #1
	bgt.n	.L_080de35c
	cmp	r5, #0
	beq.n	.L_080de362
	b.n	.L_080de36e
.L_080de35c:
	cmp	r5, #2
	beq.n	.L_080de36a
	b.n	.L_080de36e
.L_080de362:
	ldr	r0, [pc, #452]
	b.n	.L_080de370
.L_080de366:
	ldr	r0, [pc, #452]
	b.n	.L_080de370
.L_080de36a:
	ldr	r0, [pc, #452]
	b.n	.L_080de370
.L_080de36e:
	ldr	r0, [pc, #452]
.L_080de370:
	bl	sub_08002f40
	adds	r5, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #444]
	adds	r1, r5, #0
	movs	r2, #128
	lsls	r0, r0, #19
	bl	sub_080072f0
	adds	r5, #128
	ldr	r1, [sp, #40]
	adds	r0, r5, #0
	bl	sub_08005340
	ldr	r0, [pc, #428]
	ldr	r1, [sp, #24]
	movs	r2, #0
	movs	r3, #0
	bl	sub_080e0524
	cmp	r6, #1
	bne.n	.L_080de3c0
	movs	r3, #3
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #7
	movs	r3, #7
	movs	r0, #46
	bl	sub_080ed408
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r0, #47
	movs	r1, #7
	movs	r2, #7
	movs	r3, #7
	bl	sub_080ed408
	b.n	.L_080de3de
.L_080de3c0:
	movs	r3, #3
	movs	r1, #7
	movs	r2, #7
	movs	r0, #46
	str	r3, [sp, #0]
	bl	sub_080ed408
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r0, #47
	movs	r1, #7
	movs	r2, #7
	movs	r3, #3
	bl	sub_080ed408
.L_080de3de:
	ldr	r3, [pc, #352]
	ldr	r4, [sp, #40]
	adds	r2, r3, #0
	ldr	r7, [pc, #308]
	adds	r3, #188
	ldr	r3, [r3, #0]
	adds	r5, r4, r7
	str	r3, [sp, #32]
	adds	r2, #184
	ldr	r3, [r5, #0]
	ldr	r2, [r2, #0]
	ldr	r0, [r3, #8]
	str	r2, [sp, #28]
	bl	sub_080b5098
	ldr	r3, [r5, #0]
	ldr	r0, [r0, #0]
	mov	r9, r0
	movs	r1, #36
	ldrsh	r0, [r3, r1]
	bl	sub_080b5098
	ldr	r0, [r0, #0]
	movs	r2, #0
	str	r0, [sp, #16]
	ldr	r7, [pc, #304]
	mov	r8, r2
	mov	sl, r2
.L_080de416:
	bl	sub_08004458
	ldr	r3, [pc, #300]
	adds	r6, r0, #0
	ands	r6, r3
	bl	sub_08004458
	mov	r3, sl
	str	r3, [r7, #0]
	movs	r5, #255
	ands	r5, r0
	bl	sub_08004458
	movs	r3, #31
	ands	r3, r0
	adds	r3, #20
	mov	r4, sl
	lsls	r3, r3, #16
	str	r4, [r7, #8]
	str	r3, [r7, #4]
	adds	r0, r6, #0
	bl	sub_08002322
	adds	r5, #128
	adds	r3, r5, #0
	muls	r3, r0
	mov	r1, sl
	asrs	r3, r3, #5
	str	r3, [r7, #12]
	str	r1, [r7, #16]
	adds	r0, r6, #0
	bl	sub_0800231c
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #5
	str	r3, [r7, #20]
	movs	r3, #1
	add	r8, r3
	mov	r2, sl
	mov	r4, r8
	str	r2, [r7, #24]
	adds	r7, #28
	cmp	r4, #64
	bne.n	.L_080de416
	ldr	r7, [sp, #40]
	movs	r1, #239
	lsls	r1, r1, #7
	adds	r2, r7, r1
	movs	r3, #2
	str	r3, [r2, #0]
	ldr	r3, [pc, #204]
	movs	r1, #144
	adds	r2, r7, r3
	movs	r3, #75
	str	r3, [r2, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #196]
	bl	sub_080041d8
	mov	r7, r9
	ldr	r3, [r7, #8]
	add	r4, sp, #96
	str	r3, [r4, #0]
	movs	r3, #0
	str	r3, [r4, #4]
	ldr	r3, [r7, #16]
	str	r3, [r4, #8]
	ldr	r1, [sp, #44]
	mov	fp, r4
	cmp	r1, #4
	bhi.n	.L_080de558
	ldr	r2, [pc, #172]
	lsls	r3, r1, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	movs	r0, r0
	.4byte 0x080de4c4
	.4byte 0x080de4d8
	.4byte 0x080de4de
	.4byte 0x080de4f2
	.2byte 0xe506
	.2byte 0x080d
	ldr	r2, [sp, #16]
	ldr	r3, [r2, #8]
	add	r5, sp, #84
	str	r3, [r5, #0]
	movs	r3, #240
	lsls	r3, r3, #14
	str	r3, [r5, #4]
	ldr	r3, [r2, #16]
	str	r3, [r5, #8]
	b.n	.L_080de55a
	ldr	r4, [sp, #16]
	ldr	r3, [r4, #8]
	b.n	.L_080de4f6
	mov	r7, r9
	ldr	r3, [r7, #8]
	add	r5, sp, #84
	str	r3, [r5, #0]
	movs	r3, #240
	lsls	r3, r3, #14
	str	r3, [r5, #4]
	ldr	r3, [r7, #16]
	str	r3, [r5, #8]
	b.n	.L_080de55a
	mov	r1, r9
	ldr	r3, [r1, #8]
.L_080de4f6:
	add	r5, sp, #84
	str	r3, [r5, #0]
	movs	r3, #240
	lsls	r3, r3, #14
	str	r3, [r5, #4]
	movs	r3, #0
	str	r3, [r5, #8]
	b.n	.L_080de55a
	movs	r3, #240
	add	r5, sp, #84
	movs	r2, #0
	lsls	r3, r3, #14
	str	r2, [r5, #0]
	str	r3, [r5, #4]
	str	r2, [r5, #8]
	b.n	.L_080de55a
	movs	r0, r0
	.4byte 0x03001eec
	.4byte 0x00007828
	.4byte 0x000077b4
	.4byte 0x000077b8
	.4byte 0x00000094
	.4byte 0x00000092
	.4byte 0x0000008e
	.4byte 0x00000090
	.4byte 0x03001388
	.4byte 0x00000073
	.4byte 0x03001e50
	.4byte 0x02010000
	.4byte 0x0000ffff
	.4byte 0x00007784
	.4byte 0x080cd261
	.2byte 0xe4b0
	.2byte 0x080d
.L_080de558:
	add	r5, sp, #84
.L_080de55a:
	mov	r2, sp
	adds	r2, #72
	str	r2, [sp, #12]
	mov	r4, fp
	ldr	r3, [r4, #0]
	ldr	r0, [r5, #0]
	movs	r1, #40
	subs	r0, r0, r3
	bl	sub_080022ec
	ldr	r7, [sp, #12]
	str	r0, [r7, #0]
	mov	r1, fp
	ldr	r3, [r1, #4]
	ldr	r0, [r5, #4]
	movs	r1, #40
	subs	r0, r0, r3
	bl	sub_080022ec
	str	r0, [r7, #4]
	mov	r2, fp
	ldr	r3, [r2, #8]
	ldr	r0, [r5, #8]
	movs	r1, #40
	subs	r0, r0, r3
	bl	sub_080022ec
	str	r0, [r7, #8]
	ldr	r4, [sp, #20]
	movs	r3, #0
	mov	sl, r3
	cmp	r4, #0
	bne.n	.L_080de59e
	b.n	.L_080de8f4
.L_080de59e:
	ldr	r3, [pc, #44]
	mov	r7, sl
	ldr	r5, [r3, #0]
	cmp	r7, #75
	ble.n	.L_080de5b6
	ldr	r3, [pc, #24]
	lsls	r2, r7, #1
	subs	r3, r3, r2
	ldr	r2, [pc, #24]
	ldr	r1, [pc, #28]
	orrs	r3, r2
	strh	r3, [r1, #0]
.L_080de5b6:
	mov	r1, sl
	cmp	r1, #8
	bne.n	.L_080de5d4
	movs	r0, #212
	bl	sub_080f9010
	b.n	.L_080de5d4
	.4byte 0x000000a8
	.4byte 0x00001000
	.4byte 0x03001e80
	.2byte 0x0052
	.2byte 0x0400
.L_080de5d4:
	bl	sub_080049ac
	adds	r1, r5, #0
	adds	r1, #12
	adds	r0, r5, #0
	bl	sub_080051d8
	mov	r3, sl
	subs	r3, #6
	cmp	r3, #39
	bhi.n	.L_080de608
	ldr	r4, [sp, #12]
	mov	r2, fp
	ldr	r3, [r2, #0]
	ldr	r2, [r4, #0]
	mov	r7, fp
	adds	r3, r3, r2
	str	r3, [r7, #0]
	ldr	r3, [r7, #4]
	ldr	r2, [r4, #4]
	adds	r3, r3, r2
	str	r3, [r7, #4]
	ldr	r3, [r7, #8]
	ldr	r2, [r4, #8]
	adds	r3, r3, r2
	str	r3, [r7, #8]
.L_080de608:
	mov	r0, fp
	bl	sub_08004cb4
	mov	r1, sl
	cmp	r1, #0
	bne.n	.L_080de62e
	ldr	r2, [sp, #40]
	ldr	r4, [pc, #808]
	adds	r3, r2, r4
	ldr	r3, [r3, #0]
	movs	r2, #1
	ldr	r0, [r3, #8]
	movs	r3, #1
	str	r1, [sp, #0]
	negs	r2, r2
	movs	r1, #7
	negs	r3, r3
	bl	sub_080d6888
.L_080de62e:
	mov	r7, sl
	cmp	r7, #24
	bne.n	.L_080de64e
	ldr	r1, [sp, #40]
	ldr	r2, [pc, #776]
	adds	r3, r1, r2
	ldr	r3, [r3, #0]
	movs	r2, #1
	ldr	r0, [r3, #8]
	movs	r3, #0
	str	r3, [sp, #0]
	movs	r1, #0
	negs	r2, r2
	subs	r3, #1
	bl	sub_080d6888
.L_080de64e:
	mov	r4, sl
	negs	r4, r4
	mov	r1, sl
	movs	r3, #0
	lsls	r1, r1, #8
	str	r4, [sp, #8]
	ldr	r6, [pc, #744]
	mov	r8, r3
	lsls	r7, r4, #8
	mov	r9, r1
.L_080de662:
	mov	r3, r8
	cmp	r3, #0
	bge.n	.L_080de66a
	adds	r3, #7
.L_080de66a:
	asrs	r3, r3, #3
	cmp	sl, r3
	bge.n	.L_080de672
	b.n	.L_080de7a4
.L_080de672:
	ldr	r3, [r6, #24]
	cmp	r3, #0
	beq.n	.L_080de67a
	b.n	.L_080de7a4
.L_080de67a:
	bl	sub_080049e8
	movs	r3, #3
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #1
	beq.n	.L_080de6a4
	cmp	r3, #1
	bgt.n	.L_080de692
	cmp	r3, #0
	beq.n	.L_080de69c
	b.n	.L_080de6c0
.L_080de692:
	cmp	r3, #2
	beq.n	.L_080de6ac
	cmp	r3, #3
	beq.n	.L_080de6b4
	b.n	.L_080de6c0
.L_080de69c:
	mov	r0, r9
	bl	sub_08004c1c
	b.n	.L_080de6c0
.L_080de6a4:
	adds	r0, r7, #0
	bl	sub_08004bd4
	b.n	.L_080de6c0
.L_080de6ac:
	adds	r0, r7, #0
	bl	sub_08004c6c
	b.n	.L_080de6c0
.L_080de6b4:
	adds	r0, r7, #0
	bl	sub_08004bd4
	adds	r0, r7, #0
	bl	sub_08004c6c
.L_080de6c0:
	add	r5, sp, #48
	adds	r1, r5, #0
	adds	r0, r6, #0
	bl	sub_080e3944
	ldr	r3, [r5, #0]
	asrs	r3, r3, #1
	str	r3, [r5, #0]
	bl	sub_08004a5c
	ldr	r2, [r5, #8]
	cmp	r2, #249
	bgt.n	.L_080de6e0
	movs	r3, #250
	str	r3, [r5, #8]
	movs	r2, #250
.L_080de6e0:
	ldr	r3, [pc, #612]
	cmp	r2, r3
	ble.n	.L_080de6ea
	str	r3, [r5, #8]
	adds	r2, r3, #0
.L_080de6ea:
	adds	r3, r2, #0
	subs	r3, #250
	cmp	r3, #0
	bge.n	.L_080de6f4
	adds	r3, #63
.L_080de6f4:
	asrs	r3, r3, #6
	movs	r0, #8
	subs	r0, r0, r3
	lsls	r4, r0, #1
	ldr	r2, [pc, #588]
	subs	r3, r4, #2
	ldrh	r1, [r2, r3]
	ldr	r3, [sp, #24]
	adds	r1, r3, r1
	lsrs	r3, r0, #31
	ldr	r2, [r5, #0]
	adds	r3, r0, r3
	asrs	r3, r3, #1
	subs	r2, r2, r3
	ldr	r3, [r5, #4]
	str	r0, [sp, #0]
	subs	r3, r3, r0
	str	r4, [sp, #4]
	ldr	r0, [sp, #36]
	ldr	r4, [sp, #32]
	bl	sub_080072f4
	adds	r0, r6, #0
	movs	r1, #60
	movs	r2, #0
	bl	sub_080e38b8
	mov	r3, r8
	cmp	r3, #0
	bge.n	.L_080de732
	adds	r3, #7
.L_080de732:
	asrs	r3, r3, #3
	adds	r3, #24
	cmp	sl, r3
	blt.n	.L_080de7a4
	ldr	r3, [r6, #0]
	negs	r3, r3
	asrs	r5, r3, #7
	ldr	r3, [r6, #8]
	ldr	r2, [r6, #4]
	negs	r3, r3
	asrs	r4, r3, #7
	negs	r2, r2
	ldr	r3, [r6, #16]
	ldr	r1, [r6, #12]
	asrs	r2, r2, #7
	adds	r2, r3, r2
	ldr	r3, [r6, #20]
	adds	r1, r1, r5
	adds	r0, r3, r4
	lsls	r3, r1, #5
	subs	r3, r3, r1
	lsls	r3, r3, #1
	str	r1, [r6, #12]
	str	r2, [r6, #16]
	str	r0, [r6, #20]
	cmp	r3, #0
	bge.n	.L_080de76a
	adds	r3, #63
.L_080de76a:
	asrs	r3, r3, #6
	str	r3, [r6, #12]
	lsls	r3, r2, #5
	subs	r3, r3, r2
	lsls	r3, r3, #1
	cmp	r3, #0
	bge.n	.L_080de77a
	adds	r3, #63
.L_080de77a:
	asrs	r3, r3, #6
	str	r3, [r6, #16]
	lsls	r3, r0, #5
	subs	r3, r3, r0
	lsls	r3, r3, #1
	cmp	r3, #0
	bge.n	.L_080de78a
	adds	r3, #63
.L_080de78a:
	ldr	r1, [pc, #452]
	asrs	r3, r3, #6
	ldr	r2, [pc, #452]
	str	r3, [r6, #20]
	adds	r3, r5, r1
	cmp	r3, r2
	bhi.n	.L_080de7a4
	adds	r3, r4, r1
	cmp	r3, r2
	bhi.n	.L_080de7a4
	movs	r2, #1
	negs	r2, r2
	str	r2, [r6, #24]
.L_080de7a4:
	ldr	r4, [sp, #8]
	mov	r1, sl
	lsls	r3, r4, #5
	movs	r2, #1
	adds	r7, r7, r3
	add	r8, r2
	lsls	r3, r1, #5
	add	r9, r3
	mov	r3, r8
	adds	r6, #28
	cmp	r3, #32
	beq.n	.L_080de7be
	b.n	.L_080de662
.L_080de7be:
	mov	r3, sl
	subs	r3, #54
	cmp	r3, #15
	bhi.n	.L_080de80c
	lsls	r0, r1, #10
	bl	sub_08002322
	movs	r3, #0
	add	r5, sp, #48
	add	r2, sp, #60
	lsls	r0, r0, #2
	str	r0, [r2, #0]
	str	r3, [r2, #4]
	str	r3, [r2, #8]
	adds	r0, r2, #0
	adds	r1, r5, #0
	bl	sub_080e3944
	ldr	r3, [r5, #0]
	ldr	r4, [sp, #140]
	str	r3, [r4, #0]
	ldr	r7, [sp, #144]
	ldr	r3, [r5, #4]
	str	r3, [r7, #0]
	ldr	r2, [r5, #0]
	ldr	r3, [r5, #4]
	asrs	r2, r2, #1
	movs	r1, #20
	str	r2, [r5, #0]
	str	r1, [sp, #0]
	movs	r1, #40
	str	r1, [sp, #4]
	subs	r2, #10
	subs	r3, #20
	ldr	r0, [sp, #36]
	ldr	r1, [sp, #40]
	ldr	r4, [sp, #28]
	bl	sub_080072f4
.L_080de80c:
	mov	r7, sl
	cmp	r7, #64
	bne.n	.L_080de876
	ldr	r2, [sp, #40]
	movs	r3, #225
	movs	r1, #0
	lsls	r3, r3, #7
	mov	r8, r1
	adds	r7, r2, r3
.L_080de81e:
	bl	sub_08004458
	ldr	r3, [pc, #308]
	adds	r6, r0, #0
	ands	r6, r3
	bl	sub_08004458
	ldr	r4, [sp, #140]
	ldr	r3, [r4, #0]
	lsls	r3, r3, #15
	str	r3, [r7, #0]
	ldr	r1, [sp, #144]
	ldr	r3, [r1, #0]
	movs	r5, #255
	lsls	r3, r3, #16
	str	r3, [r7, #4]
	ands	r5, r0
	adds	r0, r6, #0
	bl	sub_08002322
	adds	r5, #128
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #6
	str	r3, [r7, #12]
	adds	r0, r6, #0
	bl	sub_0800231c
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #5
	str	r3, [r7, #16]
	bl	sub_08004458
	movs	r3, #15
	ands	r3, r0
	movs	r2, #1
	adds	r3, #8
	add	r8, r2
	str	r3, [r7, #24]
	mov	r3, r8
	adds	r7, #28
	cmp	r3, #64
	bne.n	.L_080de81e
.L_080de876:
	mov	r4, sl
	cmp	r4, #63
	ble.n	.L_080de8d8
	ldr	r1, [sp, #40]
	movs	r2, #225
	movs	r7, #0
	lsls	r2, r2, #7
	ldr	r6, [pc, #196]
	mov	r8, r7
	adds	r5, r1, r2
.L_080de88a:
	ldr	r0, [r5, #24]
	cmp	r0, #0
	blt.n	.L_080de8cc
	asrs	r0, r0, #3
	adds	r0, #2
	lsls	r4, r0, #1
	subs	r3, r4, #2
	ldrh	r1, [r6, r3]
	ldr	r3, [sp, #24]
	adds	r1, r3, r1
	lsrs	r3, r0, #31
	movs	r7, #2
	ldrsh	r2, [r5, r7]
	adds	r3, r0, r3
	asrs	r3, r3, #1
	subs	r2, r2, r3
	movs	r7, #6
	ldrsh	r3, [r5, r7]
	str	r0, [sp, #0]
	subs	r3, r3, r0
	str	r4, [sp, #4]
	ldr	r0, [sp, #36]
	ldr	r4, [sp, #28]
	bl	sub_080072f4
	adds	r0, r5, #0
	movs	r1, #60
	movs	r2, #0
	bl	sub_080e3908
	ldr	r3, [r5, #24]
	subs	r3, #1
	str	r3, [r5, #24]
.L_080de8cc:
	movs	r7, #1
	add	r8, r7
	mov	r1, r8
	adds	r5, #28
	cmp	r1, #64
	bne.n	.L_080de88a
.L_080de8d8:
	ldr	r3, [sp, #40]
	ldr	r4, [pc, #128]
	adds	r2, r3, r4
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_080030f8
	movs	r7, #1
	ldr	r1, [sp, #20]
	add	sl, r7
	cmp	sl, r1
	beq.n	.L_080de8f4
	b.n	.L_080de59e
.L_080de8f4:
	ldr	r0, [pc, #104]
	bl	sub_08004278
	movs	r0, #47
	bl	sub_08002dd8
	movs	r0, #46
	bl	sub_08002dd8
	ldr	r0, [pc, #92]
	bl	sub_08004278
	movs	r1, #128
	ldr	r5, [pc, #88]
	lsls	r1, r1, #7
	ldr	r0, [pc, #88]
	bl	sub_080072f8
	movs	r1, #128
	ldr	r0, [sp, #36]
	lsls	r1, r1, #7
	bl	sub_080072f8
	ldr	r2, [pc, #76]
	ldr	r3, [pc, #20]
	add	sp, #108
	strh	r3, [r2, #0]
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x00001010
	.4byte 0x00007828
	.4byte 0x02010000
	.4byte 0x0000027a
	.4byte 0x080ede48
	.4byte 0x000007ff
	.4byte 0x00000ffe
	.4byte 0x0000ffff
	.4byte 0x00007824
	.4byte 0x080cd261
	.4byte 0x080cd4b5
	.4byte 0x03000164
	.4byte 0x06004000
	.4byte 0x04000052
