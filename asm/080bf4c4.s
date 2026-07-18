@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080bf4c4
	.thumb_func
Func_080bf4c4:
	push	{r5, r6, lr}
	adds	r6, r0, #0
	bl	Func_08077008
	ldr	r3, [pc, #80]
	adds	r5, r0, r3
	ldrb	r2, [r5, #0]
	adds	r3, r2, #0
	cmp	r3, #0
	beq.n	.L0
	cmp	r3, #7
	bls.n	.L1
	adds	r3, #248
	strb	r3, [r5, #0]
	adds	r2, r3, #0
.L1:
	movs	r3, #7
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L2
	adds	r3, r2, #0
	adds	r3, #255
	strb	r3, [r5, #0]
	adds	r2, r3, #0
.L2:
	lsls	r3, r2, #24
	lsrs	r3, r3, #24
	movs	r0, #1
	cmp	r3, #0
	beq.n	.L3
	cmp	r3, #7
	bhi.n	.L0
	ldrb	r1, [r5, #0]
	adds	r0, r6, #0
	movs	r2, #30
	bl	Func_080bf208
	cmp	r0, #0
	beq.n	.L0
	movs	r3, #0
	strb	r3, [r5, #0]
	movs	r0, #1
	b.n	.L3
.L0:
	movs	r0, #0
.L3:
	pop	{r5, r6}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x0000013d
