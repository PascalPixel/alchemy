@ 呼出しグラフで未到達だったコード間隙関数の再構築サム逆アセンブル。
@ 探索では未到達。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0817b764
	.thumb_func
Func_0817b764:
	push	{r1, r3, r4, r6, r7, lr}
	lsrs	r7, r2, #32
	pop	{r0, r4, r5, r7}
	pop	{pc}
