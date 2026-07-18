@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0801c0dc
	.thumb_func
Func_0801c0dc:
	push	{r5, r6, lr}
	mov	r6, r8
	push	{r6}
	ldr	r3, [pc, #84]
	adds	r5, r0, #0
	mov	r8, r3
	adds	r6, r1, #0
	bl	Func_08004080
	mov	r2, r8
	str	r0, [r6, #0]
	movs	r1, #128
	bl	Func_08003fa4
	ldr	r3, [pc, #56]
	ldrh	r2, [r5, #8]
	ands	r0, r3
	ldr	r3, [pc, #60]
	ands	r3, r2
	orrs	r3, r0
	movs	r0, #13
	strh	r3, [r5, #8]
	negs	r0, r0
	ldrb	r3, [r5, #5]
	adds	r2, r0, #0
	ands	r2, r3
	movs	r3, #17
	negs	r3, r3
	ands	r2, r3
	movs	r3, #32
	orrs	r2, r3
	movs	r3, #4
	ldrb	r1, [r5, #7]
	negs	r3, r3
	ands	r2, r3
	subs	r3, #59
	ands	r3, r1
	movs	r1, #63
	ands	r3, r1
	strb	r3, [r5, #7]
	ands	r2, r1
	movs	r3, #128
	orrs	r2, r3
	b.n	.L0
	.4byte 0x000003ff
	.4byte 0x080342f8
	.4byte 0xfffffc00
.L0:
	ldrb	r3, [r5, #9]
	ands	r0, r3
	strb	r2, [r5, #5]
	strb	r0, [r5, #9]
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6}
	pop	{r0}
	bx	r0
