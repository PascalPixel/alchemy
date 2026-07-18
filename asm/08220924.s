@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08220924
	.thumb_func
Func_08220924:
	push	{r2, r5, r6, lr}
	str	r5, [r7, #72]
	add	pc, r4
