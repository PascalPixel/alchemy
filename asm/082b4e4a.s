@ 呼出しグラフで未到達だったコード間隙関数の再構築サム逆アセンブル。
@ 探索では未到達。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_082b4e4a
	.thumb_func
Func_082b4e4a:
	push	{r0, r3, r4, r6, r7, lr}
	b.n	Func_082b4d2e
	pop	{r1, r2, r3, r5, r6, r7, pc}
