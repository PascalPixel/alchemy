@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0807a550
	.thumb_func
Func_0807a550:
	push	{r5, r6, r7, lr}
	adds	r5, r0, #0
	movs	r0, #0
	bl	Func_08077330
	adds	r2, r0, #0
	adds	r1, r2, #0
	movs	r6, #0
	adds	r1, #8
	cmp	r5, #0
	beq.n	.L0
	adds	r3, r5, #3
	strb	r6, [r3, #0]
	adds	r3, r5, #2
	strb	r6, [r3, #0]
	adds	r3, r5, #1
	strb	r6, [r3, #0]
	strb	r6, [r5, #0]
.L0:
	movs	r3, #132
	lsls	r3, r3, #1
	adds	r0, r2, r3
	ldr	r3, [r0, #0]
	movs	r4, #0
	cmp	r3, #0
	beq.n	.L1
	movs	r7, #1
	negs	r7, r7
.L4:
	movs	r3, #3
	ldrsb	r3, [r1, r3]
	cmp	r3, r7
	bne.n	.L2
	cmp	r5, #0
	beq.n	.L3
	ldrb	r2, [r1, #0]
	ldrb	r3, [r5, r2]
	adds	r3, #1
	strb	r3, [r5, r2]
.L3:
	adds	r6, #1
.L2:
	ldr	r3, [r0, #0]
	adds	r4, #1
	adds	r1, #4
	cmp	r4, r3
	bne.n	.L4
.L1:
	adds	r0, r6, #0
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
