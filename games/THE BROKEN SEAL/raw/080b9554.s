@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080b9554
	.thumb_func
Func_080b9554:
	push	{r5, r6, r7, lr}
	mov	r7, r9
	push	{r7}
	sub	sp, #4
	mov	r3, sp
	mov	r2, r9
	str	r2, [r3, #0]
	adds	r7, r2, #0
	subs	r3, r7, #4
	ldr	r0, [r3, #0]
	movs	r1, #20
	bl	Func_080063bc
	movs	r3, #1
	movs	r5, #150
	negs	r3, r3
	movs	r6, #0
	lsls	r5, r5, #1
	cmp	r0, r3
	bne.n	.L0
	b.n	.L1
.L4:
	movs	r0, #1
	subs	r5, #1
	bl	Func_080030f8
	cmp	r5, #0
	blt.n	.L2
	ldr	r3, [pc, #116]
	ldrh	r2, [r3, #0]
	movs	r3, #3
	ands	r3, r2
	cmp	r3, #3
	beq.n	.L3
	adds	r6, #1
	cmp	r6, #24
	ble.n	.L0
	b.n	.L2
.L3:
	movs	r6, #0
.L0:
	bl	Func_080064f4
	cmp	r0, #0
	bne.n	.L4
	adds	r3, r7, #0
	subs	r3, #8
	ldr	r1, [r3, #0]
	cmp	r1, #0
	beq.n	.L5
	subs	r3, #4
	ldr	r0, [r3, #0]
	bl	Func_080063bc
	movs	r2, #1
	negs	r2, r2
	cmp	r0, r2
	bne.n	.L6
	b.n	.L1
.L8:
	movs	r0, #1
	subs	r5, #1
	bl	Func_080030f8
	cmp	r5, #0
	blt.n	.L2
	ldr	r3, [pc, #44]
	ldrh	r2, [r3, #0]
	movs	r3, #3
	ands	r3, r2
	cmp	r3, #3
	beq.n	.L7
	adds	r6, #1
	cmp	r6, #24
	ble.n	.L6
.L2:
	movs	r0, #1
	negs	r0, r0
	b.n	.L1
.L7:
	movs	r6, #0
.L6:
	bl	Func_080064f4
	cmp	r0, #0
	bne.n	.L8
.L5:
	movs	r0, #0
.L1:
	add	sp, #4
	pop	{r3}
	mov	r9, r3
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x03001f64
