@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080c0edc
	.thumb_func
Func_080c0edc:
	push	{lr}
	cmp	r0, #0
	bge.n	.L0
	adds	r0, #15
.L0:
	asrs	r0, r0, #4
	pop	{r1}
	bx	r1
