@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080a24ac
	.thumb_func
Func_080a24ac:
	push	{lr}
	movs	r0, #15
	bl	Func_080150b8
	pop	{r0}
	bx	r0
