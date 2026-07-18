@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080bbae8
	.thumb_func
Func_080bbae8:
	push	{lr}
	cmp	r0, #60
	beq.n	.L0
	cmp	r0, #60
	bgt.n	.L1
	cmp	r0, #32
	bgt.n	.L2
	cmp	r0, #31
	blt.n	.L2
	b.n	.L0
.L1:
	cmp	r0, #69
	bne.n	.L2
.L0:
	movs	r0, #1
	b.n	.L3
.L2:
	movs	r0, #0
.L3:
	pop	{r1}
	bx	r1
