@ 呼出しグラフで未到達だったコード間隙関数の再構築サム逆アセンブル。
@ 探索では未到達。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080a9e34
	.thumb_func
Func_080a9e34:
	push	{lr}
	bl	Func_080a22f4
	movs	r0, #13
	bl	Func_080a2144
	pop	{r0}
	bx	r0
	bx	lr
