@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080b7e04
	.thumb_func
Func_080b7e04:
	push	{lr}
	cmp	r0, #0
	beq.n	.L0
	movs	r1, #0
	adds	r0, #40
	movs	r2, #3
.L2:
	ldmia	r0!, {r3}
	cmp	r3, #0
	beq.n	.L1
	str	r1, [r3, #16]
.L1:
	subs	r2, #1
	cmp	r2, #0
	bge.n	.L2
.L0:
	pop	{r0}
	bx	r0
