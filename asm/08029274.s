@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08029274
	.thumb_func
Func_08029274:
	push	{r5, r6, lr}
	sub	sp, #8
	adds	r5, r2, #0
	cmp	r1, #5
	bls.n	.L0
	movs	r1, #5
.L0:
	movs	r2, #0
	cmp	r1, #0
	beq.n	.L1
	movs	r6, #15
	mov	r4, sp
.L4:
	adds	r3, r0, #0
	ands	r3, r6
	cmp	r3, #9
	bhi.n	.L2
	adds	r3, #48
	b.n	.L3
.L2:
	adds	r3, #55
.L3:
	strb	r3, [r4, #0]
	adds	r2, #1
	lsrs	r0, r0, #4
	adds	r4, #1
	cmp	r2, r1
	bne.n	.L4
.L1:
	subs	r2, r1, #1
	cmp	r2, #0
	blt.n	.L5
	mov	r3, sp
	adds	r1, r2, r3
	mov	ip, r3
.L6:
	ldrb	r3, [r1, #0]
	subs	r1, #1
	strb	r3, [r5, #0]
	adds	r5, #1
	cmp	r1, ip
	bge.n	.L6
.L5:
	add	sp, #8
	pop	{r5, r6}
	pop	{r0}
	bx	r0
