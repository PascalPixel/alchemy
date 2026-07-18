@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080b2778
	.thumb_func
Func_080b2778:
	push	{r5, lr}
	adds	r5, r1, #0
	bl	Func_08077008
	ldrb	r2, [r0, #15]
	movs	r0, #0
	cmp	r5, #0
	bne.n	.L0
	lsls	r3, r2, #2
	adds	r3, r3, r2
	lsls	r0, r3, #2
	b.n	.L1
.L0:
	cmp	r5, #1
	bne.n	.L2
	movs	r0, #10
	b.n	.L1
.L2:
	cmp	r5, #2
	bne.n	.L3
	movs	r0, #50
	b.n	.L1
.L3:
	cmp	r5, #3
	bne.n	.L1
	lsls	r3, r2, #2
	adds	r3, r3, r2
	lsls	r0, r3, #1
.L1:
	pop	{r5}
	pop	{r1}
	bx	r1
