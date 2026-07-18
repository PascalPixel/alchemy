@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080b5e14
	.thumb_func
Func_080b5e14:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r0, #170
	lsls	r0, r0, #1
	sub	sp, #48
	bl	Func_08004970
	movs	r2, #0
	mov	r8, r0
	mov	sl, r2
	movs	r7, #0
	b.n	.L0
.L9:
	bl	Func_08006488
	movs	r2, #149
	lsls	r2, r2, #1
	adds	r3, r6, r2
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L1
	movs	r3, #1
	add	sl, r3
.L1:
	movs	r0, #2
	bl	Func_080030f8
	mov	r5, sp
	ldr	r0, [pc, #184]
	adds	r1, r5, #0
	bl	Func_08015020
	movs	r0, #0
	ldrh	r3, [r5, r0]
	cmp	r3, #0
	beq.n	.L2
	adds	r2, r5, #0
.L3:
	adds	r0, #1
	cmp	r0, #4
	bgt.n	.L2
	adds	r2, #2
	ldrh	r3, [r2, #0]
	cmp	r3, #0
	bne.n	.L3
.L2:
	adds	r4, r0, #0
	movs	r0, #14
	cmp	r0, r4
	blt.n	.L4
	subs	r3, r6, r4
	adds	r1, r6, #0
	adds	r2, r3, #0
	adds	r1, #14
	adds	r2, #14
.L5:
	ldrb	r3, [r2, #0]
	subs	r0, #1
	strb	r3, [r1, #0]
	subs	r2, #1
	subs	r1, #1
	cmp	r0, r4
	bge.n	.L5
.L4:
	cmp	r4, #0
	ble.n	.L6
	adds	r2, r6, #0
	adds	r1, r5, #0
	adds	r0, r4, #0
.L7:
	ldrh	r3, [r1, #0]
	subs	r0, #1
	strb	r3, [r2, #0]
	adds	r1, #2
	adds	r2, #1
	cmp	r0, #0
	bne.n	.L7
.L6:
	movs	r3, #0
	strb	r3, [r6, #14]
	adds	r7, #1
.L0:
	cmp	r7, #2
	bgt.n	.L8
	adds	r0, r7, #0
	adds	r0, #128
	bl	Func_08077008
	adds	r6, r0, #0
	bl	Func_08006408
	movs	r2, #1
	negs	r2, r2
	cmp	r0, r2
	bne.n	.L9
.L8:
	mov	r0, r8
	bl	Func_08002df0
	movs	r0, #160
	lsls	r0, r0, #1
	bl	Func_08004970
	mov	r8, r0
	movs	r0, #1
	bl	Func_08077000
	bl	Func_08006408
	movs	r3, #1
	negs	r3, r3
	cmp	r0, r3
	beq.n	.L10
	bl	Func_08006488
	movs	r0, #2
	bl	Func_080030f8
.L10:
	mov	r0, r8
	bl	Func_08002df0
	mov	r0, sl
	add	sp, #48
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x0000080c
