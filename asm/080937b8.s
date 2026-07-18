@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080937b8
	.thumb_func
Func_080937b8:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	adds	r7, r1, #0
	movs	r3, #255
	ands	r3, r7
	mov	r8, r0
	mov	sl, r2
	cmp	r3, #6
	bne.n	.L0
	movs	r0, #110
	bl	Func_080f9010
.L0:
	mov	r0, r8
	bl	Func_0808ba1c
	adds	r6, r0, #0
	cmp	r6, #0
	beq.n	.L1
	ldr	r1, [r6, #8]
	ldr	r2, [r6, #12]
	ldr	r3, [r6, #16]
	movs	r0, #21
	bl	Func_080090c8
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L2
	ldr	r1, [pc, #80]
	bl	Func_08009098
	movs	r1, #15
	ands	r1, r7
	adds	r0, r5, #0
	bl	Func_08009080
	adds	r3, r5, #0
	movs	r2, #0
	adds	r3, #85
	strb	r2, [r3, #0]
	adds	r3, #15
	strh	r2, [r3, #0]
	adds	r3, #2
	mov	r2, r8
	strh	r2, [r3, #0]
	ldr	r3, [pc, #48]
	ldr	r0, [r5, #80]
	ldr	r1, [pc, #36]
	str	r3, [r5, #108]
	adds	r3, r0, #0
	adds	r3, #38
	strb	r1, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #1
	ands	r3, r7
	str	r6, [r5, #104]
	cmp	r3, #0
	beq.n	.L3
	ldrb	r3, [r0, #9]
	movs	r2, #13
	negs	r2, r2
	ands	r2, r3
	movs	r3, #4
	orrs	r2, r3
	strb	r2, [r0, #9]
	b.n	.L2
	movs	r0, r0
	.4byte 0x00000000
	.4byte 0x0809fc2c
	.4byte 0x0809376d
.L3:
	ldr	r3, [r6, #80]
	ldrb	r3, [r3, #9]
	movs	r2, #12
	ands	r2, r3
	ldrb	r1, [r0, #9]
	movs	r3, #13
	negs	r3, r3
	ands	r3, r1
	orrs	r3, r2
	strb	r3, [r0, #9]
.L2:
	mov	r0, sl
	bl	Func_0809163c
.L1:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
