@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08011ddc
	.thumb_func
Func_08011ddc:
	push	{r5, lr}
	movs	r3, #0
	ldrsb	r3, [r0, r3]
	adds	r0, #1
	lsls	r5, r3, #19
	movs	r3, #0
	ldrsb	r3, [r0, r3]
	lsls	r4, r3, #19
	movs	r3, #1
	ldrsb	r3, [r0, r3]
	subs	r0, r2, r1
	adds	r1, r0, #0
	adds	r1, #15
	lsls	r3, r3, #19
	cmp	r1, #15
	bne.n	.L0
	adds	r0, r4, #0
	b.n	.L1
.L0:
	cmp	r1, #14
	bhi.n	.L2
	subs	r3, r4, r5
	adds	r0, r1, #0
	muls	r0, r3
	cmp	r0, #0
	bge.n	.L3
	adds	r0, #15
.L3:
	asrs	r0, r0, #4
	adds	r0, r5, r0
	b.n	.L1
.L2:
	subs	r3, r3, r4
	muls	r0, r3
	cmp	r0, #0
	bge.n	.L4
	adds	r0, #15
.L4:
	asrs	r0, r0, #4
	adds	r0, r4, r0
.L1:
	pop	{r5}
	pop	{r1}
	bx	r1
