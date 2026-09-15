@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080dfe2c
	.thumb_func
Func_080dfe2c:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r2, [pc, #872]
	adds	r3, r2, #0
	ldmia	r3!, {r1}
	ldr	r3, [r3, #0]
	sub	sp, #36
	str	r3, [sp, #24]
	ldr	r3, [pc, #864]
	mov	r9, r1
	ldr	r2, [r2, #8]
	add	r3, r9
	str	r2, [sp, #16]
	str	r0, [r3, #0]
	movs	r0, #0
	bl	Func_080cd594
	mov	r2, sp
	adds	r2, #28
	adds	r1, r2, #0
	movs	r0, #0
	str	r2, [sp, #12]
	bl	Func_080cef64
	ldr	r5, [pc, #836]
	ldr	r1, [sp, #16]
	ldr	r0, [pc, #836]
	movs	r2, #0
	movs	r3, #0
	bl	Func_080e0524
	ldr	r0, [pc, #828]
	mov	r1, r9
	movs	r2, #1
	movs	r3, #0
	bl	Func_080e0524
	adds	r1, r5, #0
	ldr	r0, [pc, #820]
	movs	r2, #1
	movs	r3, #1
	bl	Func_080e0524
	movs	r1, #170
	lsls	r1, r1, #2
	adds	r0, r5, #0
	add	r1, r9
	movs	r2, #17
	movs	r3, #104
	bl	Func_080dfddc
	movs	r3, #221
	lsls	r3, r3, #3
	movs	r1, #153
	adds	r5, r5, r3
	lsls	r1, r1, #4
	add	r1, r9
	adds	r0, r5, #0
	movs	r2, #34
	movs	r3, #65
	bl	Func_080dfddc
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, r9
	movs	r3, #2
	str	r3, [r2, #0]
	ldr	r2, [pc, #764]
	movs	r3, #75
	add	r2, r9
	movs	r1, #144
	str	r3, [r2, #0]
	ldr	r0, [pc, #760]
	lsls	r1, r1, #3
	bl	Func_080041d8
	ldr	r3, [pc, #756]
	movs	r7, #0
	movs	r2, #1
	mov	sl, r7
	negs	r2, r2
	add	r3, r9
.L0:
	movs	r0, #1
	add	sl, r0
	mov	r1, sl
	str	r2, [r3, #0]
	adds	r3, #28
	cmp	r1, #8
	bne.n	.L0
	movs	r2, #0
	mov	sl, r2
	movs	r1, #1
	movs	r2, #128
	ldr	r3, [pc, #724]
	negs	r1, r1
	lsls	r2, r2, #2
.L1:
	movs	r7, #1
	add	sl, r7
	str	r1, [r3, #0]
	adds	r3, #28
	cmp	sl, r2
	bne.n	.L1
	movs	r0, #162
	bl	Func_080f9010
	movs	r0, #0
	str	r0, [sp, #20]
.L15:
	ldr	r1, [sp, #20]
	cmp	r1, #56
	bne.n	.L2
	movs	r0, #133
	bl	Func_080b50e8
.L2:
	movs	r3, #225
	lsls	r3, r3, #7
	movs	r2, #0
	add	r3, r9
	mov	sl, r2
	mov	r8, r3
	mov	fp, r2
.L5:
	mov	r7, r8
	movs	r0, #1
	ldr	r3, [r7, #24]
	negs	r0, r0
	cmp	r3, r0
	beq.n	.L3
	movs	r1, #65
	ldr	r2, [r7, #0]
	ldr	r3, [r7, #4]
	str	r1, [sp, #0]
	movs	r1, #34
	str	r1, [sp, #4]
	movs	r1, #153
	lsls	r1, r1, #4
	subs	r3, #17
	subs	r2, #16
	ldr	r4, [sp, #28]
	ldr	r0, [sp, #24]
	add	r1, r9
	bl	Func_080072f4
	ldr	r3, [r7, #0]
	subs	r3, #12
	str	r3, [r7, #0]
	ldr	r3, [r7, #24]
	adds	r3, #1
	str	r3, [r7, #24]
	cmp	r3, #5
	bne.n	.L3
	movs	r0, #133
	bl	Func_080f9010
	ldr	r2, [pc, #612]
	movs	r3, #4
	add	r2, r9
	ldr	r7, [pc, #572]
	str	r3, [r2, #0]
	movs	r4, #0
	add	r7, fp
.L4:
	str	r4, [sp, #8]
	bl	Func_08004458
	ldr	r3, [pc, #596]
	adds	r6, r0, #0
	ands	r6, r3
	bl	Func_08004458
	mov	r2, r8
	ldr	r3, [r2, #0]
	lsls	r3, r3, #16
	str	r3, [r7, #0]
	ldr	r3, [r2, #4]
	ldr	r5, [pc, #580]
	lsls	r3, r3, #16
	movs	r1, #128
	lsls	r1, r1, #1
	str	r3, [r7, #4]
	ands	r5, r0
	adds	r0, r6, #0
	adds	r5, r5, r1
	bl	Func_08002322
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #8
	str	r3, [r7, #12]
	adds	r0, r6, #0
	bl	Func_0800231c
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #7
	str	r3, [r7, #16]
	bl	Func_08004458
	movs	r3, #15
	ldr	r4, [sp, #8]
	ands	r3, r0
	adds	r3, #32
	adds	r4, #1
	str	r3, [r7, #24]
	adds	r7, #28
	cmp	r4, #32
	bne.n	.L4
.L3:
	movs	r0, #1
	movs	r7, #224
	add	sl, r0
	movs	r3, #28
	lsls	r7, r7, #2
	mov	r1, sl
	add	r8, r3
	add	fp, r7
	cmp	r1, #5
	bne.n	.L5
	ldr	r2, [sp, #20]
	cmp	r2, #95
	ble.n	.L6
	b.n	.L7
.L6:
	lsls	r6, r2, #11
	adds	r0, r6, #0
	bl	Func_08002322
	ldr	r7, [sp, #20]
	movs	r5, #64
	lsls	r3, r7, #1
	subs	r5, r5, r3
	adds	r3, r5, #0
	muls	r3, r0
	movs	r0, #96
	asrs	r7, r3, #17
	adds	r0, r0, r7
	mov	r8, r0
	adds	r0, r6, #0
	bl	Func_0800231c
	adds	r3, r5, #0
	muls	r3, r0
	movs	r1, #60
	asrs	r3, r3, #16
	adds	r1, r1, r3
	mov	fp, r1
	movs	r1, #20
	str	r1, [sp, #0]
	movs	r1, #34
	str	r1, [sp, #4]
	ldr	r0, [sp, #12]
	adds	r2, r7, #0
	adds	r2, #86
	adds	r3, #43
	ldr	r4, [r0, #4]
	mov	r1, r9
	ldr	r0, [sp, #24]
	bl	Func_080072f4
	ldr	r0, [pc, #420]
	movs	r1, #0
	ldrb	r3, [r0, r1]
	ldr	r2, [sp, #20]
	mov	sl, r1
	cmp	r2, r3
	bne.n	.L8
	movs	r1, #225
	lsls	r1, r1, #7
	add	r1, r9
	movs	r2, #1
	ldr	r3, [r1, #24]
	negs	r2, r2
	cmp	r3, r2
	bne.n	.L8
	adds	r3, r7, #0
	adds	r3, #88
	str	r3, [r1, #0]
	mov	r7, fp
	mov	r3, r8
	mov	r0, sl
	str	r3, [r1, #12]
	str	r7, [r1, #4]
	str	r0, [r1, #24]
	b.n	.L7
.L8:
	mov	r1, sl
	ldrb	r3, [r0, r1]
	ldr	r2, [sp, #20]
	adds	r3, #6
	cmp	r2, r3
	bne.n	.L9
	ldr	r3, [pc, #308]
	mov	r7, r9
	ldr	r3, [r7, r3]
	ldr	r3, [r3, #20]
	movs	r4, #0
	cmp	r3, #0
	beq.n	.L9
	ldr	r5, [pc, #296]
	movs	r6, #36
	add	r5, r9
.L10:
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
	bne.n	.L10
.L9:
	movs	r3, #1
	add	sl, r3
	mov	r7, sl
	cmp	r7, #5
	beq.n	.L7
	ldr	r3, [pc, #284]
	adds	r0, r3, #0
	ldrb	r3, [r0, r7]
	ldr	r1, [sp, #20]
	cmp	r1, r3
	bne.n	.L8
	lsls	r3, r7, #3
	subs	r3, r3, r7
	lsls	r3, r3, #2
	movs	r7, #225
	add	r3, r9
	lsls	r7, r7, #7
	adds	r2, r3, r7
	movs	r1, #1
	ldr	r3, [r2, #24]
	negs	r1, r1
	cmp	r3, r1
	bne.n	.L8
	mov	r3, r8
	subs	r3, #8
	str	r3, [r2, #0]
	mov	r3, r8
	str	r3, [r2, #12]
	mov	r7, fp
	movs	r3, #0
	str	r7, [r2, #4]
	str	r3, [r2, #24]
.L7:
	movs	r0, #0
	ldr	r5, [pc, #184]
	ldr	r6, [pc, #228]
	mov	sl, r0
.L13:
	movs	r1, #1
	ldr	r0, [r5, #24]
	negs	r1, r1
	cmp	r0, r1
	beq.n	.L11
	cmp	r0, #0
	bge.n	.L12
	adds	r0, #15
.L12:
	asrs	r0, r0, #4
	adds	r0, #2
	lsls	r4, r0, #1
	subs	r3, r4, #2
	ldrh	r1, [r6, r3]
	ldr	r2, [sp, #16]
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
	ldr	r0, [sp, #12]
	ldr	r4, [r0, #4]
	ldr	r0, [sp, #24]
	bl	Func_080072f4
	movs	r2, #128
	adds	r0, r5, #0
	movs	r1, #62
	lsls	r2, r2, #6
	bl	Func_080e3908
	ldr	r3, [r5, #24]
	subs	r3, #1
	str	r3, [r5, #24]
.L11:
	movs	r1, #1
	movs	r2, #128
	add	sl, r1
	lsls	r2, r2, #1
	adds	r5, #28
	cmp	sl, r2
	bne.n	.L13
	movs	r0, #4
	movs	r1, #4
	bl	Func_080e155c
	bl	Func_080cd52c
	ldr	r2, [pc, #124]
	movs	r3, #1
	add	r2, r9
	str	r3, [r2, #0]
	movs	r0, #1
	bl	Func_080030f8
	ldr	r3, [sp, #20]
	adds	r3, #1
	str	r3, [sp, #20]
	cmp	r3, #96
	beq.n	.L14
	b.n	.L15
.L14:
	ldr	r0, [pc, #64]
	bl	Func_08004278
	movs	r0, #47
	bl	Func_08002dd8
	movs	r0, #46
	bl	Func_08002dd8
	bl	Func_080cdbc0
	add	sp, #36
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001eec
	.4byte 0x00007828
	.4byte 0x02010000
	.4byte 0x00000073
	.4byte 0x00000092
	.4byte 0x0000006f
	.4byte 0x00007784
	.4byte 0x080cd261
	.4byte 0x00007098
	.4byte 0x02010018
	.4byte 0x000077a8
	.4byte 0x0000ffff
	.4byte 0x000001ff
	.4byte 0x080eec5a
	.4byte 0x080ede48
	.4byte 0x00007824
