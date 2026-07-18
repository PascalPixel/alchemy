@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0803e976
	.thumb_func
Func_0803e976:
	push	{r0, r1, r2, r4, r5, r6, lr}
	stmia	r1!, {r0, r3, r4, r6}
	pop	{r3, r7, pc}
