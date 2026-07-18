@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0808b98c
	.thumb_func
Func_0808b98c:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r3, [pc, #92]
	ldr	r5, [r3, #0]
	movs	r3, #2
	movs	r1, #0
	negs	r3, r3
	mov	sl, r1
	movs	r6, #52
	mov	r8, r3
	movs	r7, #57
.L1:
	ldr	r0, [r6, r5]
	cmp	r0, #0
	beq.n	.L0
	adds	r2, r0, #0
	adds	r2, #84
	movs	r3, #1
	strb	r3, [r2, #0]
	ldr	r2, [r0, #80]
	ldrb	r3, [r2, #29]
	mov	r1, r8
	ands	r3, r1
	strb	r3, [r2, #29]
	bl	Func_080090d0
	mov	r3, sl
	str	r3, [r6, r5]
.L0:
	subs	r7, #1
	adds	r6, #4
	cmp	r7, #0
	bge.n	.L1
	ldr	r6, [r5, #4]
	movs	r3, #0
	str	r3, [r5, #4]
	str	r3, [r5, #8]
	str	r3, [r5, #12]
	cmp	r6, #0
	beq.n	.L2
	bl	Func_0808b824
	adds	r1, r0, #0
	adds	r0, r6, #0
	bl	Func_0808b3ec
.L2:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001ebc
