@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08078480
	.thumb_func
Func_08078480:
	push	{lr}
	bl	Func_08078414
	ldrb	r3, [r0, #2]
	movs	r2, #0
	cmp	r3, #1
	bne.n	.L0
	movs	r2, #1
	b.n	.L1
.L0:
	cmp	r3, #2
	beq.n	.L2
	cmp	r3, #3
	beq.n	.L2
	cmp	r3, #4
	beq.n	.L2
	cmp	r3, #5
	beq.n	.L2
	cmp	r3, #9
	bne.n	.L1
.L2:
	movs	r2, #2
.L1:
	adds	r0, r2, #0
	pop	{r1}
	bx	r1
