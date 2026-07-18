@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080a21b0
	.thumb_func
Func_080a21b0:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	adds	r5, r1, #0
	adds	r6, r2, #0
	sub	sp, #4
	adds	r1, r6, #0
	mov	r9, r0
	mov	fp, r3
	adds	r0, r5, #0
	movs	r3, #49
	ldr	r7, [sp, #36]
	mov	sl, r3
	bl	Func_080022ec
	adds	r1, r6, #0
	mov	r8, r0
	adds	r0, r5, #0
	bl	Func_080022fc
	cmp	r0, #0
	beq.n	.L0
	movs	r3, #1
	add	r8, r3
.L0:
	mov	r3, r8
	subs	r7, r7, r3
	cmp	r3, #1
	ble.n	.L1
	movs	r0, #0
	movs	r3, #1
	str	r0, [sp, #0]
	ldr	r1, [pc, #104]
	subs	r2, r7, #1
	negs	r3, r3
	mov	r0, r9
	movs	r5, #0
	bl	Func_08015280
	cmp	r5, r8
	bge.n	.L2
.L5:
	cmp	r5, fp
	bne.n	.L3
	movs	r3, #2
	str	r3, [sp, #0]
	mov	r0, r9
	mov	r1, sl
	adds	r2, r7, #0
	subs	r3, #3
	bl	Func_08015280
	b.n	.L4
.L3:
	movs	r3, #3
	str	r3, [sp, #0]
	mov	r0, r9
	mov	r1, sl
	adds	r2, r7, #0
	subs	r3, #4
	bl	Func_08015280
.L4:
	movs	r3, #1
	adds	r5, #1
	add	sl, r3
	adds	r7, #1
	cmp	r5, r8
	blt.n	.L5
.L2:
	movs	r2, #0
	movs	r3, #1
	str	r2, [sp, #0]
	ldr	r1, [pc, #32]
	negs	r3, r3
	mov	r0, r9
	adds	r2, r7, #0
	bl	Func_08015280
.L1:
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x0000f128
	.4byte 0x0000f129
