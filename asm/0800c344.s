@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0800c344
	.thumb_func
Func_0800c344:
	push	{r5, r6, r7, lr}
	adds	r7, r1, #0
	cmp	r0, #0
	beq.n	.L0
	adds	r3, r0, #0
	adds	r3, #84
	ldrb	r3, [r3, #0]
	movs	r2, #15
	ands	r2, r3
	cmp	r2, #1
	beq.n	.L1
	cmp	r2, #2
	beq.n	.L2
	b.n	.L0
.L1:
	ldr	r0, [r0, #80]
	adds	r1, r7, #0
	bl	Func_0800baf8
	b.n	.L0
.L2:
	ldr	r5, [r0, #80]
	movs	r6, #3
.L4:
	ldmia	r5!, {r0}
	cmp	r0, #0
	beq.n	.L3
	adds	r1, r7, #0
	bl	Func_0800baf8
.L3:
	subs	r6, #1
	cmp	r6, #0
	bge.n	.L4
.L0:
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
