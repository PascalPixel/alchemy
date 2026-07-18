@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08093874
	.thumb_func
Func_08093874:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	mov	sl, r0
	mov	r8, r1
	bl	Func_0808ba1c
	adds	r7, r0, #0
	movs	r5, #0
	movs	r6, #0
	cmp	r7, #0
	beq.n	.L0
	movs	r3, #3
	mov	r1, r8
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L1
	cmp	r3, #2
	beq.n	.L2
	ldr	r3, [r7, #104]
	cmp	r3, #0
	bne.n	.L3
.L2:
	ldr	r1, [r7, #8]
	ldr	r2, [r7, #12]
	ldr	r3, [r7, #16]
	movs	r0, #209
	bl	Func_080090c8
	adds	r5, r0, #0
	b.n	.L3
.L1:
	ldr	r5, [r7, #104]
	cmp	r5, #0
	beq.n	.L0
	adds	r0, r5, #0
	bl	Func_080090d0
	str	r6, [r7, #104]
	b.n	.L0
.L3:
	cmp	r5, #0
	beq.n	.L0
	movs	r6, #3
	mov	r2, r8
	ands	r6, r2
	cmp	r6, #1
	beq.n	.L4
	cmp	r6, #2
	beq.n	.L5
	b.n	.L6
.L4:
	adds	r0, r5, #0
	movs	r1, #1
	bl	Func_08009080
	adds	r3, r5, #0
	adds	r3, #100
	str	r5, [r7, #104]
	strh	r6, [r3, #0]
	b.n	.L6
.L5:
	adds	r0, r5, #0
	movs	r1, #2
	bl	Func_08009080
	ldr	r1, [pc, #72]
	adds	r0, r5, #0
	bl	Func_08009098
	adds	r2, r5, #0
	adds	r2, #100
	movs	r3, #1
	strh	r3, [r2, #0]
.L6:
	adds	r3, r5, #0
	ldr	r2, [pc, #52]
	adds	r3, #102
	mov	r1, sl
	strh	r1, [r3, #0]
	subs	r3, #17
	strb	r2, [r3, #0]
	ldr	r3, [pc, #48]
	ldr	r6, [r5, #80]
	str	r3, [r5, #108]
	adds	r3, r6, #0
	adds	r3, #38
	strb	r2, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #1
	mov	r2, r8
	ands	r3, r2
	str	r7, [r5, #104]
	cmp	r3, #0
	beq.n	.L7
	ldrb	r3, [r6, #9]
	movs	r2, #13
	negs	r2, r2
	ands	r2, r3
	movs	r3, #4
	orrs	r2, r3
	strb	r2, [r6, #9]
	b.n	.L0
	.4byte 0x00000000
	.4byte 0x0809fd38
	.4byte 0x0809376d
.L7:
	ldr	r3, [r7, #80]
	ldrb	r3, [r3, #9]
	movs	r2, #12
	ands	r2, r3
	ldrb	r1, [r6, #9]
	movs	r3, #13
	negs	r3, r3
	ands	r3, r1
	orrs	r3, r2
	strb	r3, [r6, #9]
.L0:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
