@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080b7994
	.thumb_func
Func_080b7994:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r5, r0, #0
	movs	r3, #31
	ldrsb	r3, [r5, r3]
	movs	r2, #0
	mov	r8, r2
	ldrb	r2, [r5, #31]
	cmp	r3, #0
	blt.n	.L0
	subs	r3, r2, #1
	strb	r3, [r5, #31]
.L0:
	ldr	r4, [r5, #32]
	cmp	r4, #0
	bne.n	.L1
	movs	r2, #28
	ldrsh	r3, [r5, r2]
	ldrh	r1, [r5, #28]
	cmp	r3, #0
	bne.n	.L2
	b.n	.L3
.L1:
	movs	r2, #28
	ldrsh	r3, [r5, r2]
	ldrb	r2, [r5, #30]
	asrs	r3, r2
	movs	r2, #1
	ands	r3, r2
	ldrh	r1, [r5, #28]
	cmp	r3, #0
	beq.n	.L2
.L3:
	movs	r3, #31
	ldrsb	r3, [r5, r3]
	cmp	r3, #0
	bne.n	.L4
.L2:
	lsls	r3, r1, #16
	movs	r6, #1
	asrs	r3, r3, #16
	negs	r6, r6
	ldr	r0, [r5, #0]
	cmp	r3, #0
	beq.n	.L5
	ldrb	r2, [r5, #30]
	mov	ip, r3
	adds	r6, r2, #1
	movs	r1, #1
	b.n	.L6
.L8:
	adds	r6, #1
.L6:
	cmp	r6, #13
	ble.n	.L7
	movs	r6, #0
.L7:
	mov	r3, ip
	asrs	r3, r6
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L8
	cmp	r2, r6
	bne.n	.L9
	cmp	r4, #0
	bne.n	.L10
.L9:
	movs	r3, #1
	strb	r6, [r5, #30]
	mov	r8, r3
.L10:
	movs	r3, #80
	strb	r3, [r5, #31]
	b.n	.L11
.L5:
	movs	r2, #1
	mov	r8, r2
.L11:
	movs	r1, #0
	bl	Func_080b7f70
	adds	r7, r0, #0
	cmp	r7, #0
	beq.n	.L4
	cmp	r6, #0
	blt.n	.L12
	adds	r3, r7, #0
	adds	r3, #32
	ldrb	r3, [r3, #0]
	cmp	r3, #32
	bne.n	.L13
	movs	r3, #170
	lsls	r3, r3, #1
	adds	r6, r6, r3
	b.n	.L12
.L13:
	ldr	r2, [pc, #104]
	adds	r6, r6, r2
.L12:
	ldr	r1, [r5, #32]
	cmp	r1, #0
	beq.n	.L14
	mov	r3, r8
	cmp	r3, #0
	beq.n	.L14
	adds	r0, r7, #0
	bl	Func_08009050
	movs	r3, #0
	str	r3, [r5, #32]
.L14:
	cmp	r6, #0
	blt.n	.L15
	mov	r2, r8
	cmp	r2, #0
	beq.n	.L15
	adds	r0, r7, #0
	adds	r1, r6, #0
	bl	Func_08009048
	movs	r3, #1
	negs	r3, r3
	str	r0, [r5, #32]
	cmp	r0, r3
	bne.n	.L16
	movs	r3, #0
	str	r3, [r5, #32]
.L16:
	ldr	r0, [r5, #32]
	cmp	r0, #0
	beq.n	.L15
	movs	r3, #3
	strb	r3, [r0, #6]
	movs	r1, #0
	bl	Func_08009070
.L15:
	adds	r2, r7, #0
	adds	r2, #37
	movs	r3, #1
	strb	r3, [r2, #0]
	cmp	r6, #0
	blt.n	.L17
	strh	r6, [r5, #8]
	b.n	.L4
.L17:
	movs	r3, #0
	strh	r3, [r5, #8]
.L4:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x00000163
