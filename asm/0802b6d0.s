@ 呼出しグラフで未到達だったコード間隙関数の再構築サム逆アセンブル。
@ 探索では未到達。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0802b6d0
	.thumb_func
Func_0802b6d0:
	push	{lr}
	pop	{r0, r1, r3, r4, r5, r7, pc}
