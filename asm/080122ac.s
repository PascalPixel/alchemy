@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080122ac
	.thumb_func
Func_080122ac:
	push	{lr}
	adds	r0, r1, #0
	bl	Func_08012204
	subs	r0, #5
	cmp	r0, #7
	bhi.n	.L0
	movs	r0, #0
	b.n	.L1
.L0:
	movs	r0, #1
	negs	r0, r0
.L1:
	pop	{r1}
	bx	r1
