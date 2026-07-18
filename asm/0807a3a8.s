@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0807a3a8
	.thumb_func
Func_0807a3a8:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	mov	r8, r1
	movs	r1, #0
	mov	sl, r2
	mov	r9, r1
	movs	r3, #0
	cmp	r0, #7
	bls.n	.L0
	movs	r3, #1
.L0:
	adds	r0, r3, #0
	bl	Func_08077330
	movs	r1, #132
	adds	r3, r0, #0
	lsls	r1, r1, #1
	movs	r2, #8
	adds	r2, r2, r3
	adds	r7, r3, r1
	mov	ip, r2
	ldr	r2, [r7, #0]
	movs	r4, #0
	adds	r0, #9
	movs	r5, #0
	mov	r1, ip
	cmp	r9, r2
	bge.n	.L1
	ldrb	r3, [r1, #0]
	mov	lr, r3
	mov	r6, r9
	cmp	r8, lr
	bne.n	.L2
	ldrb	r3, [r0, #0]
	cmp	sl, r3
	bne.n	.L2
	subs	r3, r2, #1
	movs	r1, #1
	str	r3, [r7, #0]
	mov	r9, r1
	b.n	.L3
.L2:
	ldr	r2, [r7, #0]
	adds	r4, #1
	adds	r0, #4
	adds	r1, #4
	adds	r5, #4
	cmp	r4, r2
	bge.n	.L1
	ldrb	r3, [r1, #0]
	adds	r6, r5, #0
	cmp	r8, r3
	bne.n	.L2
	ldrb	r3, [r0, #0]
	cmp	sl, r3
	bne.n	.L2
	subs	r3, r2, #1
	str	r3, [r7, #0]
	movs	r2, #1
	mov	r9, r2
.L3:
	movs	r3, #128
	lsls	r3, r3, #1
	add	r3, ip
	ldr	r3, [r3, #0]
	cmp	r4, r3
	bge.n	.L1
	movs	r2, #128
	lsls	r2, r2, #1
	add	r2, ip
	b.n	.L4
.L5:
	lsls	r6, r4, #2
.L4:
	mov	r1, ip
	adds	r3, r6, #4
	ldr	r3, [r1, r3]
	str	r3, [r1, r6]
	ldr	r3, [r2, #0]
	adds	r4, #1
	cmp	r4, r3
	blt.n	.L5
.L1:
	mov	r0, r9
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
