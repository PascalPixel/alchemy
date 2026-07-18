@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0800b93c
	.thumb_func
Func_0800b93c:
	push	{r5, r6, lr}
	adds	r6, r0, #0
	adds	r5, r1, #0
	cmp	r6, #0
	beq.n	.L0
	cmp	r5, #0
	beq.n	.L0
	adds	r0, r5, #0
	bl	Func_0800bc48
	ldr	r3, [r6, #40]
	movs	r0, #0
	cmp	r5, r3
	beq.n	.L1
	adds	r2, r6, #0
	adds	r2, #40
.L2:
	adds	r0, #1
	cmp	r0, #3
	bhi.n	.L1
	adds	r2, #4
	ldr	r3, [r2, #0]
	cmp	r5, r3
	bne.n	.L2
.L1:
	cmp	r0, #4
	beq.n	.L0
	lsls	r3, r0, #2
	movs	r2, #0
	adds	r3, #40
	str	r2, [r6, r3]
	adds	r2, r0, #1
	movs	r4, #0
	cmp	r2, #3
	bhi.n	.L3
	lsls	r3, r2, #2
	adds	r3, r3, r6
	adds	r1, r3, #0
	adds	r1, #40
.L5:
	ldmia	r1!, {r3}
	cmp	r3, #0
	beq.n	.L4
	adds	r4, #1
.L4:
	adds	r2, #1
	cmp	r2, #3
	bls.n	.L5
.L3:
	cmp	r4, #0
	bne.n	.L0
	adds	r3, r6, #0
	adds	r3, #39
	strb	r0, [r3, #0]
.L0:
	pop	{r5, r6}
	pop	{r0}
	bx	r0
