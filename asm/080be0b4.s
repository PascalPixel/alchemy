@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080be0b4
	.thumb_func
Func_080be0b4:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	adds	r5, r0, #0
	adds	r6, r1, #0
	movs	r1, #0
	sub	sp, #20
	mov	fp, r1
	movs	r0, #1
	cmp	r5, #7
	bls.n	.L0
	movs	r0, #2
.L0:
	add	r2, sp, #4
	mov	r9, r2
	mov	r1, r9
	bl	Func_080b6c08
	adds	r7, r0, #0
	movs	r0, #0
	cmp	r5, #7
	bls.n	.L1
	movs	r0, #1
.L1:
	bl	Func_08077000
	adds	r0, #8
	mov	r8, r0
	cmp	r6, #0
	beq.n	.L2
	movs	r2, #0
	adds	r3, r6, #3
	mov	ip, r6
.L3:
	strb	r2, [r3, #0]
	subs	r3, #1
	cmp	r3, ip
	bge.n	.L3
.L2:
	movs	r2, #128
	lsls	r2, r2, #1
	movs	r3, #0
	add	r2, r8
	mov	sl, r3
	ldr	r3, [r2, #0]
	cmp	r3, #0
	beq.n	.L4
	movs	r1, #0
	str	r2, [sp, #0]
	mov	ip, r9
	mov	r5, r8
	mov	lr, r1
.L9:
	movs	r2, #1
	movs	r3, #3
	ldrsb	r3, [r5, r3]
	negs	r2, r2
	cmp	r3, r2
	bne.n	.L5
	movs	r4, #0
	cmp	r4, r7
	bge.n	.L6
	mov	r3, ip
	ldrh	r2, [r3, #0]
	ldrb	r3, [r5, #2]
	cmp	r2, r3
	beq.n	.L6
	adds	r1, r5, #0
	mov	r0, r9
.L7:
	adds	r4, #1
	cmp	r4, r7
	bge.n	.L6
	adds	r0, #2
	ldrh	r2, [r0, #0]
	ldrb	r3, [r1, #2]
	cmp	r2, r3
	bne.n	.L7
.L6:
	cmp	r4, r7
	beq.n	.L5
	cmp	r6, #0
	beq.n	.L8
	mov	r3, r8
	mov	r1, lr
	ldrb	r2, [r1, r3]
	ldrb	r3, [r6, r2]
	adds	r3, #1
	strb	r3, [r6, r2]
.L8:
	movs	r1, #1
	add	fp, r1
.L5:
	ldr	r1, [sp, #0]
	movs	r3, #1
	add	sl, r3
	ldr	r3, [r1, #0]
	movs	r2, #4
	adds	r5, #4
	add	lr, r2
	cmp	sl, r3
	bne.n	.L9
.L4:
	mov	r0, fp
	add	sp, #20
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
