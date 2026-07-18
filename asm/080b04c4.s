@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080b04c4
	.thumb_func
Func_080b04c4:
	push	{lr}
	b.n	.L0
.L1:
	movs	r0, #1
	bl	Func_080030f8
.L0:
	bl	Func_080f9048
	cmp	r0, #0
	bne.n	.L1
	pop	{r0}
	bx	r0
