@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080a8904
	.thumb_func
Func_080a8904:
	push	{lr}
	movs	r3, #255
.L0:
	subs	r3, #1
	cmp	r3, #0
	bge.n	.L0
	pop	{r0}
	bx	r0
