@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08078618
	.thumb_func
Func_08078618:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	sub	sp, #24
	add	r5, sp, #4
	mov	r8, r0
	adds	r0, r5, #0
	bl	Func_080796c4
	movs	r6, #0
	adds	r7, r0, #0
	adds	r3, r5, #0
	cmp	r6, r7
	bge.n	.L0
.L3:
	movs	r2, #0
	ldrsh	r5, [r3, r2]
	mov	r1, r8
	adds	r3, #2
	adds	r0, r5, #0
	str	r3, [sp, #0]
	bl	Func_08078588
	ldr	r3, [sp, #0]
	cmp	r0, #0
	blt.n	.L1
	adds	r0, r5, #0
	b.n	.L2
.L1:
	adds	r6, #1
	cmp	r6, r7
	blt.n	.L3
.L0:
	movs	r0, #1
	negs	r0, r0
.L2:
	add	sp, #24
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
