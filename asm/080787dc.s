@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080787dc
	.thumb_func
Func_080787dc:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	mov	r8, r1
	bl	Func_08077394
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r7, r0, #0
	movs	r6, #0
	movs	r5, #216
	mov	sl, r2
	b.n	.L0
.L2:
	adds	r5, #2
	adds	r6, #1
.L0:
	cmp	r6, #14
	bgt.n	.L1
	ldrh	r3, [r5, r7]
	mov	r2, sl
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L2
	ldrh	r0, [r5, r7]
	bl	Func_08078414
	ldrb	r3, [r0, #2]
	cmp	r3, r8
	bne.n	.L2
.L1:
	cmp	r6, #15
	bne.n	.L3
	movs	r6, #1
	negs	r6, r6
.L3:
	adds	r0, r6, #0
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
