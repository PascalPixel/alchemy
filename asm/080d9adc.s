@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080d9adc
	.thumb_func
Func_080d9adc:
	push	{lr}
	movs	r1, #3
	bl	Func_080d9ae8
	pop	{r0}
	bx	r0
