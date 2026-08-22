@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080b60a0
	.thumb_func
Func_080b60a0:
	push	{r5, r6, r7, lr}
	ldr	r3, [pc, #96]
	ldr	r1, [r3, #0]
	adds	r3, r1, #0
	adds	r3, #68
	ldrb	r3, [r3, #0]
	sub	sp, #4
	movs	r4, #0
	cmp	r3, #0
	beq.n	.L0
	adds	r3, r1, #0
	adds	r3, #80
	ldrb	r2, [r3, #0]
	movs	r3, #1
	eors	r3, r2
	lsls	r2, r3, #1
	adds	r2, r2, r3
	ldr	r3, [pc, #68]
	lsls	r2, r2, #3
	adds	r7, r2, r3
	adds	r3, r1, #0
	adds	r3, #82
	ldrb	r3, [r3, #0]
	ldr	r5, [pc, #60]
	cmp	r3, #0
	bne.n	.L1
	ldr	r3, [pc, #32]
	ldr	r2, [pc, #36]
	strh	r3, [r5, #0]
	strh	r3, [r5, #4]
	ldr	r3, [pc, #32]
	strh	r2, [r5, #2]
	strh	r3, [r5, #6]
	movs	r6, #0
.L4:
	ldr	r3, [pc, #40]
	ldrh	r2, [r3, #0]
	movs	r3, #3
	ands	r3, r2
	cmp	r3, #3
	beq.n	.L2
	adds	r4, #1
	cmp	r4, #24
	ble.n	.L3
	b.n	.L1
	.4byte 0x00000045
	.4byte 0x00000058
	.4byte 0x00000043
	.4byte 0x03001e74
	.4byte 0x02002024
	.4byte 0x02002224
	.4byte 0x03001f64
.L2:
	ldrh	r2, [r5, #4]
	ldrh	r3, [r7, #4]
	movs	r4, #0
	cmp	r2, r3
	bne.n	.L3
	ldrh	r2, [r5, #6]
	ldrh	r3, [r7, #6]
	cmp	r2, r3
	beq.n	.L0
.L3:
	movs	r0, #1
	str	r4, [sp, #0]
	bl	Func_080030f8
	adds	r6, #1
	ldr	r4, [sp, #0]
	cmp	r6, #29
	ble.n	.L4
.L1:
	movs	r0, #1
	negs	r0, r0
	b.n	.L5
.L0:
	movs	r0, #0
.L5:
	add	sp, #4
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
