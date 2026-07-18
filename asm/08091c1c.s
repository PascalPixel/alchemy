@ 呼出しグラフで未到達だったコード間隙関数の再構築サム逆アセンブル。
@ 探索では未到達。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08091c1c
	.thumb_func
Func_08091c1c:
	push	{r5, lr}
	adds	r5, r2, #0
	adds	r1, r0, #0
	adds	r0, r5, #0
	bl	Func_08077028
	cmp	r0, #0
	blt.n	.L0
	adds	r0, r5, #0
	b.n	.L1
.L0:
	movs	r0, #1
	negs	r0, r0
.L1:
	pop	{r5}
	pop	{r1}
	bx	r1
	movs	r0, r0
	bx	lr
