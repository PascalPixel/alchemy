@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08028920
	.thumb_func
Func_08028920:
	push	{r5, r6, r7, lr}
	adds	r5, r0, #0
	movs	r0, #1
	negs	r0, r0
	movs	r6, #0
	bl	Func_08077290
	cmp	r0, #0
	bne.n	.L0
	movs	r6, #1
.L0:
	lsls	r3, r6, #1
	adds	r3, r3, r6
	ldr	r2, [pc, #88]
	lsls	r7, r3, #1
	adds	r3, r5, r7
	ldrsb	r3, [r2, r3]
	subs	r5, r3, #1
	cmp	r5, #0
	bge.n	.L1
	movs	r5, #0
.L1:
	bl	Func_080284dc
	movs	r0, #1
	bl	Func_080287a8
	cmp	r6, #0
	bne.n	.L2
	movs	r0, #15
	bl	Func_080287a8
.L2:
	movs	r0, #2
	bl	Func_080287a8
	movs	r0, #7
	bl	Func_080287a8
	movs	r0, #17
	movs	r1, #7
	movs	r2, #0
	bl	Func_08028808
	adds	r0, r5, #0
	bl	Func_08028574
	adds	r5, r0, #0
	bl	Func_0802851c
	cmp	r5, #0
	blt.n	.L3
	adds	r3, r5, r7
	ldr	r2, [pc, #16]
	adds	r3, #1
	ldrsb	r5, [r2, r3]
.L3:
	adds	r0, r5, #0
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x08037403
	.4byte 0x080373f7
