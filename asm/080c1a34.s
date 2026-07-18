@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080c1a34
	.thumb_func
Func_080c1a34:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #180]
	lsls	r0, r0, #4
	adds	r0, r0, r3
	ldrb	r3, [r0, #6]
	movs	r2, #0
	sub	sp, #28
	mov	r9, r2
	mov	fp, r2
	mov	sl, r0
	movs	r7, #0
	cmp	r3, #0
	bne.n	.L0
	mov	r2, sl
	adds	r2, #6
.L1:
	adds	r7, #1
	cmp	r7, #4
	bhi.n	.L0
	adds	r2, #1
	ldrb	r3, [r2, #0]
	cmp	r3, #0
	beq.n	.L1
.L0:
	cmp	r7, #5
	bne.n	.L2
	movs	r0, #1
	negs	r0, r0
	b.n	.L3
.L2:
	movs	r3, #11
	add	r3, sl
	movs	r7, #0
	mov	r8, r3
.L7:
	mov	r2, r8
	ldrb	r3, [r2, #0]
	movs	r2, #1
	add	r8, r2
	cmp	r3, #0
	beq.n	.L4
	mov	r3, sl
	adds	r3, #1
	ldrb	r6, [r3, r7]
	adds	r0, r6, #0
	adds	r0, #8
	bl	Func_08077198
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L4
	ldrb	r3, [r5, #15]
	cmp	r3, #3
	bls.n	.L5
	movs	r0, #186
	lsls	r0, r0, #1
	bl	Func_080770c0
	cmp	r0, #0
	bne.n	.L5
	movs	r3, #193
	lsls	r3, r3, #3
	adds	r0, r6, r3
	bl	Func_080770c0
	cmp	r0, #0
	beq.n	.L6
.L5:
	ldrb	r3, [r5, #15]
	movs	r2, #1
	add	fp, r3
	add	r9, r2
.L4:
	adds	r7, #1
	cmp	r7, #4
	bls.n	.L7
	mov	r3, r9
	cmp	r3, #0
	bne.n	.L8
	movs	r0, #3
	negs	r0, r0
	b.n	.L3
.L6:
	movs	r0, #2
	negs	r0, r0
	b.n	.L3
.L8:
	mov	r0, fp
	mov	r1, r9
	bl	Func_080022ec
.L3:
	add	sp, #28
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x080c5c38
