@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080f40d0
	.thumb_func
Func_080f40d0:
	push	{lr}
	lsls	r0, r0, #16
	lsls	r1, r1, #16
	asrs	r1, r1, #16
	asrs	r0, r0, #8
	bl	Func_080022ec
	lsls	r0, r0, #16
	asrs	r0, r0, #16
	pop	{r1}
	bx	r1
