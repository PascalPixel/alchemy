@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080fac44
	.thumb_func
Func_080fac44:
	push	{r4, lr}
	adds	r2, r1, #0
	movs	r0, #1
	ldrb	r1, [r2, #0]
	ands	r0, r1
	cmp	r0, #0
	beq.n	.L0
	ldrb	r3, [r2, #19]
	ldrb	r1, [r2, #18]
	adds	r0, r3, #0
	muls	r0, r1
	lsrs	r3, r0, #5
	ldrb	r4, [r2, #24]
	cmp	r4, #1
	bne.n	.L1
	movs	r0, #22
	ldrsb	r0, [r2, r0]
	adds	r0, #128
	muls	r0, r3
	lsrs	r3, r0, #7
.L1:
	movs	r0, #20
	ldrsb	r0, [r2, r0]
	lsls	r0, r0, #1
	movs	r1, #21
	ldrsb	r1, [r2, r1]
	adds	r1, r0, r1
	cmp	r4, #2
	bne.n	.L2
	movs	r0, #22
	ldrsb	r0, [r2, r0]
	adds	r1, r1, r0
.L2:
	movs	r0, #128
	negs	r0, r0
	cmp	r1, r0
	bge.n	.L3
	adds	r1, r0, #0
	b.n	.L4
.L3:
	cmp	r1, #127
	ble.n	.L4
	movs	r1, #127
.L4:
	adds	r0, r1, #0
	adds	r0, #128
	muls	r0, r3
	lsrs	r0, r0, #8
	strb	r0, [r2, #16]
	movs	r0, #127
	subs	r0, r0, r1
	muls	r0, r3
	lsrs	r0, r0, #8
	strb	r0, [r2, #17]
.L0:
	ldrb	r1, [r2, #0]
	movs	r0, #4
	ands	r0, r1
	adds	r3, r1, #0
	cmp	r0, #0
	beq.n	.L5
	movs	r0, #14
	ldrsb	r0, [r2, r0]
	ldrb	r1, [r2, #15]
	muls	r0, r1
	movs	r1, #12
	ldrsb	r1, [r2, r1]
	adds	r1, r1, r0
	lsls	r1, r1, #2
	movs	r0, #10
	ldrsb	r0, [r2, r0]
	lsls	r0, r0, #8
	adds	r1, r1, r0
	movs	r0, #11
	ldrsb	r0, [r2, r0]
	lsls	r0, r0, #8
	adds	r1, r1, r0
	ldrb	r0, [r2, #13]
	adds	r1, r0, r1
	ldrb	r0, [r2, #24]
	cmp	r0, #0
	bne.n	.L6
	movs	r0, #22
	ldrsb	r0, [r2, r0]
	lsls	r0, r0, #4
	adds	r1, r1, r0
.L6:
	asrs	r0, r1, #8
	strb	r0, [r2, #8]
	strb	r1, [r2, #9]
.L5:
	movs	r0, #250
	ands	r0, r3
	strb	r0, [r2, #0]
	pop	{r4}
	pop	{r0}
	bx	r0
