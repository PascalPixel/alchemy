@ 呼出しグラフで未到達だったコード間隙関数の再構築サム逆アセンブル。
@ 探索では未到達。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08064278
	.thumb_func
Func_08064278:
	push	{r1, r2, r3, r5, lr}
	strb	r7, [r6, #12]
	strh	r1, [r1, #54]
	add	r2, sp, #808
	ldr	r2, [r6, #116]
	pop	{r0, r1, r2, r3, r4, r5, r7, pc}
