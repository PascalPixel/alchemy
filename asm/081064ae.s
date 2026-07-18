@ 呼出しグラフで未到達だったコード間隙関数の再構築サム逆アセンブル。
@ 探索では未到達。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_081064ae
	.thumb_func
Func_081064ae:
	push	{r0, r1, r3, r5, r7, lr}
	stmia	r2!, {r0, r2, r3, r4, r5, r7}
	stmia	r0!, {r1, r6, r7}
	hlt	0x003d
	pop	{r1, r3, r4, r5, r7, pc}
