@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080c9c9c
	.thumb_func
Func_080c9c9c:
	push	{lr}
	movs	r1, #1
	bl	Func_080c9ca8
	pop	{r0}
	bx	r0
