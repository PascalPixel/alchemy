@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08028df4
	.thumb_func
Func_08028df4:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	adds	r5, r2, #0
	adds	r6, r3, #0
	movs	r3, #0
	adds	r7, r0, #0
	mov	sl, r1
	mov	r8, r3
	bl	Func_080284dc
	cmp	r5, #0
	bne.n	.L0
	movs	r5, #3
.L0:
	cmp	r7, #0
	beq.n	.L1
	movs	r3, #17
	mov	r8, r3
.L1:
	movs	r0, #5
	bl	Func_080287a8
	movs	r0, #6
	bl	Func_080287a8
	adds	r1, r5, #0
	mov	r2, sl
	mov	r0, r8
	bl	Func_08028808
	adds	r0, r6, #0
	bl	Func_08028574
	adds	r6, r0, #0
	bl	Func_0802851c
	movs	r3, #1
	negs	r3, r3
	cmp	r6, r3
	bne.n	.L2
	movs	r6, #1
.L2:
	adds	r0, r6, #0
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
