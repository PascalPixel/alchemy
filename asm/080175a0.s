@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080175a0
	.thumb_func
Func_080175a0:
	push	{lr}
	bl	Func_080174f8
	b.n	.L0
.L1:
	movs	r0, #1
	bl	Func_080030f8
.L0:
	bl	Func_08017364
	cmp	r0, #0
	beq.n	.L1
	movs	r0, #1
	bl	Func_080030f8
	pop	{r0}
	bx	r0
