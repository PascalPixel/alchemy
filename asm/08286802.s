@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08286802
	.thumb_func
Func_08286802:
	push	{r0, r6, r7, lr}
	pop	{r1, r4, r5, pc}
