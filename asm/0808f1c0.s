@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0808f1c0
	.thumb_func
Func_0808f1c0:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	ldr	r3, [pc, #184]
	movs	r2, #250
	lsls	r2, r2, #1
	adds	r3, r3, r2
	mov	r9, r0
	ldr	r0, [r3, #0]
	mov	sl, r1
	bl	Func_0808ba1c
	movs	r1, #193
	adds	r7, r0, #0
	lsls	r1, r1, #3
	movs	r0, #17
	bl	Func_080048b0
	ldr	r2, [r7, #12]
	movs	r3, #144
	lsls	r3, r3, #14
	adds	r2, r2, r3
	mov	r8, r0
	ldr	r1, [r7, #8]
	ldr	r3, [r7, #16]
	movs	r0, #22
	bl	Func_080090c8
	adds	r6, r0, #0
	cmp	r6, #0
	beq.n	.L0
	ldr	r5, [r6, #80]
	adds	r2, r5, #0
	movs	r3, #0
	adds	r2, #38
	strb	r3, [r2, #0]
	adds	r2, #1
	strb	r3, [r2, #0]
	ldrb	r2, [r5, #5]
	subs	r3, #33
	ands	r3, r2
	ldrb	r2, [r5, #9]
	strb	r3, [r5, #5]
	movs	r3, #15
	ands	r3, r2
	movs	r2, #13
	negs	r2, r2
	ands	r3, r2
	movs	r2, #4
	orrs	r3, r2
	strb	r3, [r5, #9]
	mov	r0, r9
	bl	Func_08015250
	movs	r2, #128
	lsls	r2, r2, #3
	add	r2, r8
	movs	r1, #128
	ldrb	r0, [r5, #28]
	bl	Func_08003fa4
	movs	r0, #17
	bl	Func_08002dd8
	movs	r3, #1
	mov	r2, sl
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L1
	ldr	r3, [pc, #56]
	str	r3, [r6, #108]
.L1:
	movs	r3, #2
	mov	r2, sl
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L2
	adds	r0, r6, #0
	bl	Func_0808f0d8
.L2:
	movs	r0, #80
	bl	Func_080030f8
	adds	r0, r7, #0
	movs	r1, #1
	bl	Func_08009080
	adds	r0, r6, #0
	bl	Func_080090d0
.L0:
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x02000240
	.4byte 0x0808eee5
