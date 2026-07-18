@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0800e280
	.thumb_func
Func_0800e280:
	push	{lr}
	adds	r4, r2, #0
	cmp	r1, #0
	bne.n	.L0
	strh	r4, [r0, #6]
	b.n	.L1
.L0:
	cmp	r1, #1
	bne.n	.L2
	ldrh	r3, [r0, #6]
	adds	r3, r3, r4
	strh	r3, [r0, #6]
	b.n	.L1
.L2:
	lsls	r3, r4, #16
	ldrh	r2, [r0, #6]
	lsrs	r3, r3, #16
	movs	r1, #0
	cmp	r2, r3
	bne.n	.L3
	movs	r1, #1
.L3:
	adds	r3, r0, #0
	adds	r3, #87
	strb	r1, [r3, #0]
.L1:
	pop	{r0}
	bx	r0
