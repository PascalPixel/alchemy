@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08011d60
	.thumb_func
Func_08011d60:
	push	{r5, lr}
	movs	r3, #0
	ldrsb	r3, [r0, r3]
	lsls	r4, r3, #19
	movs	r3, #1
	ldrsb	r3, [r0, r3]
	lsls	r0, r3, #19
	adds	r5, r2, #0
	adds	r2, r4, #0
	cmp	r0, r4
	ble.n	.L0
	adds	r2, r0, #0
.L0:
	subs	r3, r5, r1
	adds	r1, r3, #0
	adds	r1, #15
	cmp	r1, #15
	bne.n	.L1
	adds	r0, r2, #0
	b.n	.L2
.L1:
	cmp	r1, #14
	bhi.n	.L2
	adds	r0, r4, #0
.L2:
	pop	{r5}
	pop	{r1}
	bx	r1
