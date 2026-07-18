@ 呼出しグラフで未到達だったコード間隙関数の再構築サム逆アセンブル。
@ 探索では未到達。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0811b246
	.thumb_func
Func_0811b246:
	push	{r0, r3, r6, r7, lr}
	add	r4, sp, #688
	pop	{r2, r4, r5, r7, pc}
