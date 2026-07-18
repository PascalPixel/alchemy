@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080b606c
	.thumb_func
Func_080b606c:
	push	{r5, r6, lr}
	sub	sp, #8
	mov	r5, sp
	movs	r6, #95
	adds	r4, r5, #0
	adds	r0, r5, #0
	movs	r1, #3
.L1:
	ldrh	r3, [r2, #0]
	strb	r3, [r0, #0]
	lsls	r3, r3, #24
	adds	r2, #2
	adds	r0, #1
	cmp	r3, #0
	bne.n	.L0
	strb	r6, [r4, #0]
.L0:
	subs	r1, #1
	adds	r4, #1
	cmp	r1, #0
	bge.n	.L1
	movs	r3, #0
	strb	r3, [r5, #4]
	add	sp, #8
	pop	{r5, r6}
	pop	{r1}
	bx	r1
