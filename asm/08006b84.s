@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08006b84
	.thumb_func
Func_08006b84:
	push	{r4, lr}
	adds	r4, r0, #0
	subs	r3, r2, #1
	cmp	r2, #0
	beq.n	.L0
	movs	r2, #1
	negs	r2, r2
.L1:
	ldrb	r0, [r4, #0]
	strb	r0, [r1, #0]
	adds	r4, #1
	adds	r1, #1
	subs	r3, #1
	cmp	r3, r2
	bne.n	.L1
.L0:
	pop	{r4}
	pop	{r0}
	bx	r0
