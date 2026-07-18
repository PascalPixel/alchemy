@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080b1470
	.thumb_func
Func_080b1470:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	adds	r5, r1, #0
	mov	sl, r0
	adds	r0, r5, #0
	sub	sp, #4
	mov	r9, r2
	bl	Func_08077008
	mov	r1, sl
	mov	r8, r0
	movs	r6, #8
	movs	r7, #8
	cmp	r1, #0
	beq.n	.L0
	mov	r0, sl
	bl	Func_08015060
	adds	r0, r5, #0
	mov	r1, r9
	bl	Func_08077038
	movs	r2, #1
	negs	r2, r2
	cmp	r0, r2
	beq.n	.L1
	lsls	r3, r0, #1
	adds	r3, #216
	mov	r1, r8
	ldrh	r0, [r1, r3]
	lsrs	r0, r0, #11
	adds	r0, #1
	movs	r1, #5
	bl	Func_08015120
	ldr	r0, [pc, #116]
	mov	r1, sl
	movs	r2, #0
	movs	r3, #0
	bl	Func_08015080
	b.n	.L2
.L1:
	ldr	r0, [pc, #108]
	mov	r1, sl
	movs	r2, #0
	movs	r3, #0
	bl	Func_08015080
.L2:
	movs	r3, #216
	mov	r2, r8
	ldrh	r3, [r2, r3]
	movs	r5, #0
	cmp	r3, #0
	beq.n	.L0
	movs	r2, #0
.L5:
	adds	r3, r2, #0
	adds	r3, #216
	mov	r1, r8
	ldrh	r0, [r1, r3]
	mov	r2, sl
	adds	r3, r6, #0
	movs	r1, #27
	str	r7, [sp, #0]
	bl	Func_080152d0
	movs	r3, #252
	strb	r3, [r0, #15]
	adds	r6, #16
	cmp	r5, #4
	bne.n	.L3
	movs	r6, #8
	adds	r7, #16
.L3:
	cmp	r5, #9
	bne.n	.L4
	movs	r6, #8
	adds	r7, #16
.L4:
	adds	r5, #1
	cmp	r5, #14
	bgt.n	.L0
	lsls	r3, r5, #1
	adds	r2, r3, #0
	mov	r1, r8
	adds	r3, #216
	ldrh	r3, [r1, r3]
	cmp	r3, #0
	bne.n	.L5
.L0:
	add	sp, #4
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x00000c90
	.4byte 0x00000c8f
