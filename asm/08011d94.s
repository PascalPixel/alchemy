@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08011d94
	.thumb_func
Func_08011d94:
	push	{r5, r6, lr}
	movs	r3, #0
	ldrsb	r3, [r0, r3]
	adds	r0, #1
	lsls	r6, r3, #19
	movs	r3, #0
	ldrsb	r3, [r0, r3]
	lsls	r5, r3, #19
	movs	r3, #1
	ldrsb	r3, [r0, r3]
	adds	r1, r1, r2
	lsls	r3, r3, #19
	adds	r0, r5, #0
	cmp	r1, #15
	beq.n	.L0
	cmp	r1, #14
	bhi.n	.L1
	subs	r3, r5, r6
	adds	r0, r1, #0
	muls	r0, r3
	movs	r1, #15
	bl	Func_080022ec
	adds	r0, r6, r0
	b.n	.L0
.L1:
	subs	r1, #15
	subs	r3, r3, r5
	adds	r0, r1, #0
	muls	r0, r3
	movs	r1, #15
	bl	Func_080022ec
	adds	r0, r5, r0
.L0:
	pop	{r5, r6}
	pop	{r1}
	bx	r1
