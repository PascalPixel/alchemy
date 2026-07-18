@ 呼出しグラフで未到達だったコード間隙関数の再構築サム逆アセンブル。
@ 探索では未到達。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0803ff3e
	.thumb_func
Func_0803ff3e:
	push	{r0, r3, r7, lr}
	bmi.n	Func_0803ffd0
	pop	{r0, r2, r6, pc}
