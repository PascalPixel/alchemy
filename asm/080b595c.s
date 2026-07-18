@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080b595c
	.thumb_func
Func_080b595c:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r3, [pc, #144]
	ldr	r3, [r3, #0]
	sub	sp, #16
	adds	r7, r0, #0
	mov	sl, r3
	bl	Func_08015118
	mov	r6, sp
	adds	r0, r6, #0
	bl	Func_080b6ae0
	movs	r5, #0
	cmp	r7, #0
	beq.n	.L0
	subs	r3, r7, #1
	mov	r8, r3
.L3:
	ldrh	r0, [r6, #0]
	movs	r1, #1
	adds	r6, #2
	bl	Func_08015120
	cmp	r5, r8
	bne.n	.L1
	ldr	r0, [pc, #104]
	bl	Func_080151c8
	b.n	.L2
.L1:
	ldr	r0, [pc, #100]
	bl	Func_080151c8
.L2:
	adds	r5, #1
	bl	Func_080bb65c
	cmp	r5, r7
	bne.n	.L3
.L0:
	bl	Func_08015218
	mov	r3, sl
	adds	r3, #69
	ldrb	r3, [r3, #0]
	cmp	r3, #1
	bne.n	.L4
	bl	Func_08015118
	movs	r0, #0
	movs	r1, #1
	bl	Func_08015120
	ldr	r0, [pc, #60]
	bl	Func_080151c8
	bl	Func_080bb65c
	b.n	.L5
.L4:
	cmp	r3, #2
	bne.n	.L5
	bl	Func_08015118
	movs	r0, #0
	movs	r1, #1
	bl	Func_08015120
	ldr	r0, [pc, #36]
	bl	Func_080151c8
	bl	Func_080bb65c
.L5:
	add	sp, #16
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001e74
	.4byte 0x00000811
	.4byte 0x00000810
	.4byte 0x00000812
	.4byte 0x00000813
