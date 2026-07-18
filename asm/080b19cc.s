@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080b19cc
	.thumb_func
Func_080b19cc:
	push	{r5, r6, r7, lr}
	adds	r6, r0, #0
	bl	Func_08077018
	ldrh	r3, [r0, #0]
	adds	r0, r6, #0
	lsls	r7, r3, #16
	bl	Func_08077018
	ldrb	r2, [r0, #3]
	movs	r3, #8
	ands	r3, r2
	asrs	r5, r7, #16
	cmp	r3, #0
	beq.n	.L0
	movs	r5, #0
	b.n	.L1
.L0:
	movs	r3, #128
	lsls	r3, r3, #3
	ands	r3, r6
	cmp	r3, #0
	beq.n	.L2
	lsrs	r3, r7, #31
	adds	r3, r5, r3
	asrs	r5, r3, #1
	b.n	.L1
.L2:
	lsls	r3, r5, #1
	adds	r0, r3, r5
	cmp	r0, #0
	bge.n	.L3
	adds	r0, #3
.L3:
	asrs	r5, r0, #2
.L1:
	adds	r0, r5, #0
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
