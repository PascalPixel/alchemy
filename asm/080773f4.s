@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080773f4
	.thumb_func
Func_080773f4:
	push	{lr}
	cmp	r3, #0
	beq.n	.L0
	movs	r4, #0
	cmp	r4, r2
	bge.n	.L1
.L2:
	ldrb	r3, [r0, #0]
	adds	r4, #1
	strb	r3, [r1, #0]
	adds	r0, #1
	adds	r1, #1
	cmp	r4, r2
	blt.n	.L2
	b.n	.L1
.L0:
	cmp	r2, #0
	ble.n	.L1
	adds	r4, r2, #0
.L3:
	ldrb	r3, [r1, #0]
	subs	r4, #1
	strb	r3, [r0, #0]
	adds	r1, #1
	adds	r0, #1
	cmp	r4, #0
	bne.n	.L3
.L1:
	pop	{r0}
	bx	r0
