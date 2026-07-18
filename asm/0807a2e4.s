@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0807a2e4
	.thumb_func
Func_0807a2e4:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	adds	r7, r0, #0
	adds	r6, r1, #0
	mov	r8, r2
	bl	Func_08077394
	adds	r1, r6, #0
	adds	r5, r0, #0
	mov	r2, r8
	adds	r0, r7, #0
	bl	Func_0807a1f8
	mov	sl, r0
	cmp	r0, #0
	beq.n	.L0
	lsls	r2, r6, #2
	adds	r3, r2, #0
	adds	r3, #248
	movs	r1, #1
	mov	r0, r8
	ldr	r3, [r5, r3]
	lsls	r1, r0
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L1
	movs	r3, #132
	lsls	r3, r3, #1
	adds	r2, r2, r3
	ldr	r3, [r5, r2]
	orrs	r3, r1
	str	r3, [r5, r2]
	b.n	.L2
.L1:
	movs	r0, #0
	b.n	.L3
.L2:
	movs	r0, #142
	lsls	r0, r0, #1
	adds	r2, r6, r0
	ldrb	r3, [r5, r2]
	adds	r3, #1
	strb	r3, [r5, r2]
	adds	r0, r7, #0
	bl	Func_08079ae8
.L0:
	mov	r0, sl
.L3:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
