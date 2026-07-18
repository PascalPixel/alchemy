@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0807977c
	.thumb_func
Func_0807977c:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r7, [pc, #72]
	movs	r3, #15
	adds	r3, r3, r7
	mov	sl, r3
	movs	r3, #1
	sub	sp, #4
	adds	r5, r0, #0
	movs	r1, #0
	mov	r8, r3
.L1:
	movs	r0, #0
	ldrb	r6, [r7, #0]
	str	r1, [sp, #0]
	bl	Func_08077330
	mov	r2, r8
	ldr	r3, [r0, #0]
	lsls	r2, r6
	ands	r3, r2
	adds	r7, #1
	ldr	r1, [sp, #0]
	cmp	r3, #0
	beq.n	.L0
	strb	r6, [r5, #0]
	adds	r1, #1
	adds	r5, #1
.L0:
	cmp	r7, sl
	bls.n	.L1
	movs	r3, #32
	adds	r0, r1, #0
	strb	r3, [r5, #0]
	add	sp, #4
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x08084a8c
