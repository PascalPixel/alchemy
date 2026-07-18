@ 呼出しグラフで未到達だったコード間隙関数の再構築サム逆アセンブル。
@ 探索では未到達。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_081a2da0
	.thumb_func
Func_081a2da0:
	push	{r2, r3, r4, lr}
	pop	{r0, r4, pc}
