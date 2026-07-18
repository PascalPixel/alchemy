@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080f9a50
	.thumb_func
Func_080f9a50:
	push	{r4, r5, lr}
	adds	r5, r1, #0
	ldr	r4, [r5, #32]
	cmp	r4, #0
	beq.n	.L0
.L2:
	ldrb	r1, [r4, #0]
	movs	r0, #199
	tst	r0, r1
	beq.n	.L1
	movs	r0, #64
	orrs	r1, r0
	strb	r1, [r4, #0]
.L1:
	adds	r0, r4, #0
	bl	Func_080f9a30
	ldr	r4, [r4, #52]
	cmp	r4, #0
	bne.n	.L2
.L0:
	movs	r0, #0
	strb	r0, [r5, #0]
	pop	{r4, r5}
	pop	{r0}
	bx	r0
