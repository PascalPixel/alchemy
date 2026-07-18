@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0801b398
	.thumb_func
Func_0801b398:
	push	{r5, r6, r7, lr}
	ldr	r3, [pc, #120]
	ldr	r5, [r3, #0]
	adds	r6, r0, #0
	movs	r1, #0
	adds	r0, r5, #0
	bl	Func_0801b9ec
	ldr	r7, [pc, #108]
.L0:
	movs	r0, #1
	bl	Func_080030f8
	movs	r2, #232
	lsls	r2, r2, #2
	adds	r3, r5, r2
	ldrh	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L0
	ldr	r3, [pc, #92]
	cmp	r6, r3
	beq.n	.L1
	ldr	r1, [pc, #92]
	ldr	r3, [r1, #0]
	movs	r2, #16
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L2
	adds	r0, r5, #0
	bl	Func_0801b664
	b.n	.L1
.L2:
	ldr	r3, [r1, #0]
	movs	r2, #32
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L3
	adds	r0, r5, #0
	bl	Func_0801b810
	b.n	.L1
.L3:
	ldr	r3, [r7, #0]
	movs	r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L1
	adds	r0, r5, #0
	bl	Func_0801be80
	b.n	.L4
.L1:
	cmp	r6, #0
	beq.n	.L0
	ldr	r3, [r7, #0]
	movs	r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L0
	movs	r0, #1
	negs	r0, r0
.L4:
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x03001e98
	.4byte 0x03001c94
	.4byte 0x000003e7
	.4byte 0x03001b04
