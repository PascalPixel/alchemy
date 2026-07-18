@ 呼出しグラフで未到達だったコード間隙関数の再構築サム逆アセンブル。
@ 探索では未到達。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0828f382
	.thumb_func
Func_0828f382:
	push	{r1, r4, lr}
	ldmia	r1!, {r2, r3, r4}
	add	r1, sp, #72
	subs	r2, r5, #3
	adds	r0, r7, #3
	adds	r4, r2, #2
	pop	{r1, r4, r5, r6, pc}
