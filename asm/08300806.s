@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08300806
	.thumb_func
Func_08300806:
	push	{r0, r1, r4, lr}
	movs	r1, #191
	pop	{r4, pc}
