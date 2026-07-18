@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
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
	bl	Func_080cd594
	movs	r2, #0
	ldr	r1, [sp, #16]
	movs	r3, #0
	ldr	r0, [pc, #696]
	bl	Func_080e0524
	ldr	r0, [pc, #696]
	bl	Func_08002f40
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #688]
	movs	r2, #128
	lsls	r0, r0, #19
	bl	Func_080072f0
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #7
	movs	r3, #3
	movs	r0, #46
	bl	Func_080ed408
	ldr	r5, [r5, #28]
	movs	r0, #0
	movs	r1, #1
	movs	r2, #128
	str	r5, [sp, #20]
	ldr	r3, [pc, #656]
	mov	r8, r0
	negs	r1, r1
	lsls	r2, r2, #3
.L0:
	movs	r4, #1
	add	r8, r4
	str	r1, [r3, #0]
	adds	r3, #28
	cmp	r8, r2
	bne.n	.L0
	bl	Func_080049ac
	adds	r1, r6, #0
	adds	r1, #12
	adds	r0, r6, #0
	bl	Func_080051d8
	ldr	r2, [pc, #608]
	ldr	r1, [sp, #28]
	ldr	r3, [r1, r2]
	ldr	r3, [r3, #20]
	movs	r0, #0
	mov	fp, r0
	cmp	r3, #0
	beq.n	.L1
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
.L3:
	ldr	r3, [sp, #28]
	adds	r5, r3, r2
	ldr	r4, [sp, #12]
	ldr	r3, [r5, #0]
	ldrsh	r0, [r3, r4]
	bl	Func_080b5098
	ldr	r3, [r5, #0]
	ldr	r2, [sp, #12]
	ldr	r6, [r0, #0]
	ldrsh	r0, [r3, r2]
	bl	Func_080b5070
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
	bl	Func_080e3944
	mov	r2, sl
	ldr	r3, [r2, #0]
	asrs	r3, r3, #1
	str	r3, [r2, #0]
	ldr	r4, [sp, #8]
	ldr	r0, [pc, #528]
	movs	r3, #0
	mov	r8, r3
	adds	r7, r4, r0
.L2:
	bl	Func_08004458
	movs	r1, #255
	adds	r6, r0, #0
	ands	r6, r1
	bl	Func_08004458
	ldr	r3, [pc, #512]
	adds	r5, r0, #0
	ands	r5, r3
	adds	r0, r5, #0
	bl	Func_08002322
	adds	r2, r6, #0
	muls	r2, r0
	mov	r4, sl
	ldr	r3, [r4, #0]
	asrs	r2, r2, #7
	lsls	r3, r3, #16
	adds	r2, r2, r3
	str	r2, [r7, #0]
	adds	r0, r5, #0
	bl	Func_0800231c
	adds	r2, r6, #0
	muls	r2, r0
	mov	r0, sl
	ldr	r3, [r0, #4]
	asrs	r2, r2, #3
	lsls	r3, r3, #16
	adds	r2, r2, r3
	str	r2, [r7, #4]
	bl	Func_08004458
	movs	r1, #255
	ands	r0, r1
	movs	r3, #128
	subs	r3, r3, r0
	lsls	r3, r3, #9
	str	r3, [r7, #12]
	bl	Func_08004458
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
	bne.n	.L2
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
	bne.n	.L3
.L1:
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
	bl	Func_080041d8
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
	bne.n	.L4
	b.n	.L5
.L4:
	adds	r6, r0, r1
.L13:
	mov	r3, r9
	cmp	r3, #32
	bne.n	.L6
	movs	r0, #0
	bl	Func_080b50e8
.L6:
	ldr	r3, [r6, #0]
	ldr	r3, [r3, #20]
	movs	r4, #0
	mov	fp, r4
	cmp	r3, #0
	beq.n	.L7
	movs	r0, #0
	movs	r7, #0
	mov	sl, r0
.L12:
	cmp	r9, r7
	bne.n	.L8
	movs	r0, #143
	bl	Func_080f9010
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
	bl	Func_080d6888
.L8:
	cmp	r9, r7
	ble.n	.L9
	ldr	r5, [pc, #248]
	movs	r0, #0
	mov	r8, r0
	add	r5, sl
.L11:
	ldr	r3, [r5, #24]
	cmp	r3, #0
	blt.n	.L10
	movs	r1, #3
	mov	r0, r8
	bl	Func_080022fc
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
	bl	Func_080072f4
	movs	r0, #3
	mov	r3, r8
	ldr	r2, [pc, #192]
	ands	r3, r0
	lsls	r3, r3, #2
	ldr	r2, [r2, r3]
	adds	r0, r5, #0
	movs	r1, #62
	bl	Func_080e3908
	ldr	r3, [r5, #24]
	adds	r3, #1
	str	r3, [r5, #24]
	ldr	r3, [r5, #16]
	cmp	r3, #0
	ble.n	.L10
	movs	r1, #6
	ldrsh	r3, [r5, r1]
	cmp	r3, #112
	ble.n	.L10
	movs	r3, #1
	negs	r3, r3
	str	r3, [r5, #24]
.L10:
	movs	r2, #1
	add	r8, r2
	mov	r3, r8
	adds	r5, #28
	cmp	r3, #128
	bne.n	.L11
.L9:
	ldr	r3, [r6, #0]
	movs	r4, #224
	movs	r0, #1
	ldr	r3, [r3, #20]
	lsls	r4, r4, #4
	add	fp, r0
	adds	r7, #20
	add	sl, r4
	cmp	fp, r3
	bne.n	.L12
.L7:
	bl	Func_080cd52c
	ldr	r3, [pc, #120]
	ldr	r1, [sp, #28]
	adds	r2, r1, r3
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	Func_080030f8
	ldr	r3, [r6, #0]
	ldr	r2, [r3, #20]
	lsls	r3, r2, #2
	adds	r3, r3, r2
	movs	r4, #1
	lsls	r3, r3, #2
	add	r9, r4
	adds	r3, #56
	cmp	r9, r3
	beq.n	.L5
	b.n	.L13
.L5:
	ldr	r0, [pc, #68]
	bl	Func_08004278
	movs	r0, #46
	bl	Func_08002dd8
	bl	Func_080cdbc0
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
	.4byte 0x00007824
