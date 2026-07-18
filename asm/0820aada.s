@ 呼出しグラフで未到達だったコード間隙関数の再構築サム逆アセンブル。
@ 探索では未到達。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0820aada
	.thumb_func
Func_0820aada:
	push	{r0, r1, r3, r4, r6, lr}
	pop	{r0, r1, r3, r4, pc}
