@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08135260
	.thumb_func
Func_08135260:
	push	{r0, r1, r4, r5, r7, lr}
	stmia	r3!, {r0, r1, r3, r4, r5, r7}
	pop	{r0, r1, r2, r6, r7, pc}
