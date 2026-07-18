@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080fa484
	.thumb_func
Func_080fa484:
	push	{lr}
	bl	Func_080fa264
	pop	{r0}
	bx	r0
