@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080e0564
	.thumb_func
Func_080e0564:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r5, [pc, #96]
	adds	r3, r5, #0
	ldmia	r3!, {r1}
	ldr	r3, [r3, #0]
	sub	sp, #40
	str	r3, [sp, #36]
	ldr	r3, [pc, #88]
	mov	r9, r1
	ldr	r2, [r5, #8]
	add	r3, r9
	str	r2, [sp, #20]
	str	r0, [r3, #0]
	movs	r0, #0
	bl	Func_080cd594
	ldr	r2, [pc, #72]
	ldr	r3, [pc, #60]
	movs	r6, #2
	strh	r3, [r2, #0]
	movs	r1, #7
	movs	r2, #7
	movs	r3, #11
	movs	r0, #46
	str	r6, [sp, #0]
	bl	Func_080ed408
	movs	r3, #3
	movs	r1, #7
	movs	r2, #7
	movs	r0, #47
	str	r3, [sp, #0]
	bl	Func_080ed408
	ldr	r3, [r5, #28]
	ldr	r5, [r5, #32]
	str	r3, [sp, #24]
	ldr	r1, [sp, #20]
	ldr	r0, [pc, #32]
	movs	r2, #0
	movs	r3, #0
	str	r5, [sp, #28]
	bl	Func_080e0524
	ldr	r0, [pc, #24]
	mov	r1, r9
	movs	r2, #1
	b.n	.L0
	.4byte 0x00001010
	.4byte 0x03001eec
	.4byte 0x00007828
	.4byte 0x04000052
	.4byte 0x00000073
	.4byte 0x00000094
.L0:
	movs	r3, #1
	bl	Func_080e0524
	movs	r1, #190
	lsls	r1, r1, #2
	ldr	r0, [pc, #668]
	add	r1, r9
	movs	r2, #1
	movs	r3, #0
	bl	Func_080e0524
	movs	r3, #239
	lsls	r3, r3, #7
	ldr	r2, [pc, #656]
	add	r3, r9
	str	r6, [r3, #0]
	add	r2, r9
	movs	r3, #75
	movs	r1, #144
	str	r3, [r2, #0]
	ldr	r0, [pc, #644]
	lsls	r1, r1, #3
	movs	r5, #225
	bl	Func_080041d8
	lsls	r5, r5, #7
	movs	r4, #0
	mov	sl, r4
	movs	r7, #63
	add	r5, r9
	movs	r6, #104
.L1:
	bl	Func_08004458
	ands	r0, r7
	str	r0, [r5, #0]
	movs	r0, #1
	add	sl, r0
	mov	r1, sl
	str	r6, [r5, #4]
	adds	r5, #28
	cmp	r1, #32
	bne.n	.L1
	movs	r2, #0
	mov	sl, r2
	movs	r1, #1
	movs	r2, #128
	ldr	r3, [pc, #596]
	negs	r1, r1
	lsls	r2, r2, #2
.L2:
	movs	r4, #1
	add	sl, r4
	str	r1, [r3, #0]
	adds	r3, #28
	cmp	sl, r2
	bne.n	.L2
	movs	r0, #141
	bl	Func_080f9010
	movs	r0, #128
	movs	r7, #0
	lsls	r0, r0, #8
	str	r7, [sp, #32]
	str	r0, [sp, #16]
.L16:
	ldr	r1, [sp, #32]
	cmp	r1, #79
	bgt.n	.L3
	ldr	r0, [sp, #16]
	bl	Func_08002322
	lsls	r5, r0, #1
	adds	r5, r5, r0
	ldr	r0, [sp, #16]
	bl	Func_0800231c
	ldr	r3, [sp, #32]
	lsls	r2, r3, #1
	movs	r3, #64
	subs	r3, r3, r2
	muls	r3, r0
	lsls	r5, r5, #3
	movs	r2, #20
	asrs	r5, r5, #16
	asrs	r3, r3, #16
	adds	r5, #22
	str	r2, [sp, #0]
	movs	r2, #38
	str	r2, [sp, #4]
	adds	r3, #29
	ldr	r0, [sp, #36]
	mov	r1, r9
	adds	r2, r5, #0
	ldr	r4, [sp, #28]
	bl	Func_080072f4
.L3:
	ldr	r7, [sp, #32]
	cmp	r7, #56
	bne.n	.L4
	movs	r0, #133
	bl	Func_080b50e8
.L4:
	movs	r2, #225
	movs	r0, #0
	lsls	r2, r2, #7
	movs	r1, #16
	add	r2, r9
	str	r0, [sp, #12]
	mov	sl, r0
	mov	fp, r1
	mov	r8, r2
.L11:
	ldr	r3, [sp, #32]
	cmp	r3, fp
	blt.n	.L5
	mov	r4, r8
	movs	r1, #34
	ldr	r2, [r4, #0]
	ldr	r3, [r4, #4]
	str	r1, [sp, #0]
	movs	r1, #65
	str	r1, [sp, #4]
	movs	r1, #158
	lsls	r1, r1, #4
	subs	r2, #17
	subs	r3, #32
	ldr	r0, [sp, #36]
	add	r1, r9
	ldr	r7, [sp, #24]
	bl	Func_08007300
	ldr	r0, [sp, #32]
	cmp	r0, fp
	bne.n	.L6
	ldr	r1, [sp, #12]
	ldr	r2, [pc, #428]
	movs	r4, #0
	adds	r7, r1, r2
.L7:
	str	r4, [sp, #8]
	bl	Func_08004458
	ldr	r6, [pc, #420]
	movs	r3, #128
	lsls	r3, r3, #7
	ands	r6, r0
	adds	r6, r6, r3
	bl	Func_08004458
	mov	r1, r8
	ldr	r3, [r1, #0]
	lsls	r3, r3, #16
	str	r3, [r7, #0]
	ldr	r5, [pc, #404]
	ldr	r3, [r1, #4]
	ands	r5, r0
	adds	r3, #16
	movs	r0, #128
	lsls	r3, r3, #16
	lsls	r0, r0, #1
	adds	r5, r5, r0
	str	r3, [r7, #4]
	adds	r0, r6, #0
	bl	Func_08002322
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #7
	str	r3, [r7, #12]
	adds	r0, r6, #0
	bl	Func_0800231c
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #6
	str	r3, [r7, #16]
	bl	Func_08004458
	movs	r3, #15
	ldr	r4, [sp, #8]
	ands	r3, r0
	adds	r3, #32
	adds	r4, #1
	str	r3, [r7, #24]
	adds	r7, #28
	cmp	r4, #16
	bne.n	.L7
	movs	r3, #1
	mov	r2, sl
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L8
	movs	r0, #133
	bl	Func_080f9010
.L8:
	ldr	r2, [pc, #324]
	movs	r3, #4
	add	r2, r9
	str	r3, [r2, #0]
	ldr	r3, [pc, #320]
	mov	r7, r9
	ldr	r3, [r7, r3]
	ldr	r3, [r3, #20]
	movs	r4, #0
	cmp	r3, #0
	beq.n	.L6
	ldr	r5, [pc, #304]
	movs	r6, #36
	add	r5, r9
.L9:
	ldr	r3, [r5, #0]
	ldrsh	r0, [r3, r6]
	movs	r3, #6
	str	r3, [sp, #0]
	movs	r2, #5
	adds	r3, r4, #0
	movs	r1, #7
	str	r4, [sp, #8]
	bl	Func_080d6888
	ldr	r3, [r5, #0]
	movs	r1, #6
	ldrsh	r0, [r3, r6]
	bl	Func_080b5088
	ldr	r3, [r5, #0]
	ldr	r4, [sp, #8]
	ldr	r3, [r3, #20]
	adds	r4, #1
	adds	r6, #2
	cmp	r4, r3
	bne.n	.L9
.L6:
	mov	r4, r8
	ldr	r3, [r4, #4]
	subs	r3, #12
	str	r3, [r4, #4]
.L5:
	ldr	r1, [sp, #12]
	movs	r2, #224
	movs	r3, #1
	lsls	r2, r2, #2
	add	sl, r3
	movs	r7, #4
	movs	r0, #28
	adds	r1, r1, r2
	mov	r4, sl
	add	fp, r7
	add	r8, r0
	str	r1, [sp, #12]
	cmp	r4, #10
	beq.n	.L10
	b.n	.L11
.L10:
	movs	r7, #0
	ldr	r5, [pc, #200]
	ldr	r6, [pc, #216]
	mov	sl, r7
.L14:
	movs	r1, #1
	ldr	r0, [r5, #24]
	negs	r1, r1
	cmp	r0, r1
	beq.n	.L12
	cmp	r0, #0
	bge.n	.L13
	adds	r0, #15
.L13:
	asrs	r0, r0, #4
	adds	r0, #2
	lsls	r4, r0, #1
	subs	r3, r4, #2
	ldrh	r1, [r6, r3]
	ldr	r2, [sp, #20]
	adds	r1, r2, r1
	movs	r3, #2
	ldrsh	r2, [r5, r3]
	lsrs	r3, r0, #31
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
	bl	Func_080072f4
	movs	r2, #128
	adds	r0, r5, #0
	movs	r1, #62
	lsls	r2, r2, #6
	bl	Func_080e3908
	ldr	r3, [r5, #24]
	subs	r3, #1
	str	r3, [r5, #24]
.L12:
	movs	r7, #1
	movs	r0, #128
	add	sl, r7
	lsls	r0, r0, #2
	adds	r5, #28
	cmp	sl, r0
	bne.n	.L14
	movs	r1, #4
	movs	r0, #4
	bl	Func_080e155c
	bl	Func_080cd52c
	ldr	r2, [pc, #112]
	movs	r3, #1
	add	r2, r9
	str	r3, [r2, #0]
	movs	r0, #1
	bl	Func_080030f8
	ldr	r2, [pc, #104]
	ldr	r1, [sp, #16]
	ldr	r3, [sp, #32]
	adds	r1, r1, r2
	adds	r3, #1
	str	r1, [sp, #16]
	str	r3, [sp, #32]
	cmp	r3, #96
	beq.n	.L15
	b.n	.L16
.L15:
	ldr	r0, [pc, #48]
	bl	Func_08004278
	movs	r0, #47
	bl	Func_08002dd8
	movs	r0, #46
	bl	Func_08002dd8
	bl	Func_080cdbc0
	add	sp, #40
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x0000006f
	.4byte 0x00007784
	.4byte 0x080cd261
	.4byte 0x02010018
	.4byte 0x02010000
	.4byte 0x00007fff
	.4byte 0x000001ff
	.4byte 0x000077a8
	.4byte 0x00007828
	.4byte 0x080ede48
	.4byte 0x00007824
	.4byte 0xfffff800
