@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080a3ce4
	.thumb_func
Func_080a3ce4:
	push	{lr}
	cmp	r0, #196
	bgt.n	.L0
	cmp	r0, #193
	blt.n	.L0
	movs	r0, #1
	b.n	.L1
.L0:
	movs	r0, #0
.L1:
	pop	{r1}
	bx	r1
