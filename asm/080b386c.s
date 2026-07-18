@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080b386c
	.thumb_func
Func_080b386c:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	mov	sl, r1
	adds	r7, r0, #0
	mov	r0, sl
	mov	r9, r2
	sub	sp, #4
	bl	Func_08077008
	mov	r2, r9
	lsls	r3, r2, #1
	adds	r6, r3, #0
	adds	r6, #216
	ldrh	r3, [r0, r6]
	ldr	r5, [pc, #152]
	ands	r5, r3
	ldrh	r3, [r0, r6]
	lsrs	r3, r3, #11
	adds	r3, #1
	mov	r8, r0
	mov	fp, r3
	cmp	r7, #0
	beq.n	.L0
	adds	r0, r7, #0
	bl	Func_08015270
	ldr	r0, [pc, #132]
	movs	r3, #0
	adds	r0, r5, r0
	adds	r1, r7, #0
	movs	r2, #0
	bl	Func_08015080
	mov	r0, sl
	mov	r1, r9
	bl	Func_080772a8
	movs	r3, #4
	negs	r3, r3
	cmp	r0, r3
	bne.n	.L1
	ldr	r0, [pc, #104]
	b.n	.L2
.L1:
	movs	r2, #3
	negs	r2, r2
	cmp	r0, r2
	bne.n	.L3
	ldr	r0, [pc, #96]
.L2:
	adds	r1, r7, #0
	movs	r2, #0
	movs	r3, #8
	bl	Func_08015080
	b.n	.L0
.L3:
	mov	r3, r8
	ldrh	r0, [r3, r6]
	bl	Func_080b19cc
	ldr	r5, [pc, #80]
	mov	r6, fp
	muls	r6, r0
	adds	r1, r7, #0
	adds	r0, r5, #0
	movs	r2, #8
	movs	r3, #8
	bl	Func_08015080
	movs	r3, #8
	str	r3, [sp, #0]
	adds	r0, r6, #0
	movs	r1, #5
	adds	r2, r7, #0
	movs	r3, #40
	subs	r5, #5
	bl	Func_080150b0
	adds	r0, r5, #0
	adds	r1, r7, #0
	movs	r2, #80
	movs	r3, #8
	bl	Func_08015080
.L0:
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x000001ff
	.4byte 0x00000182
	.4byte 0x00000c94
	.4byte 0x00000c95
	.4byte 0x00000c8d
