@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0811108c
	.thumb_func
Func_0811108c:
	push	{r4, r5, r7, lr}
	push	{r1, r2, r4, r5, r7, lr}
	pop	{r3, r4, r5, r7, pc}
