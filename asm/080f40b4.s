@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080f40b4
	.thumb_func
Func_080f40b4:
	push	{lr}
	lsls	r0, r0, #16
	lsls	r1, r1, #16
	asrs	r1, r1, #16
	asrs	r0, r0, #16
	muls	r0, r1
	adds	r3, r0, #0
	cmp	r0, #0
	bge.n	.L0
	adds	r3, #255
.L0:
	lsls	r0, r3, #8
	asrs	r0, r0, #16
	pop	{r1}
	bx	r1
