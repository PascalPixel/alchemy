@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080289e8
	.thumb_func
Func_080289e8:
	push	{r5, r6, lr}
	movs	r6, #0
	movs	r5, #0
	bl	Func_0801f77c
	cmp	r0, #0
	bge.n	.L0
	movs	r0, #1
	negs	r0, r0
	b.n	.L1
.L0:
	cmp	r0, #0
	bne.n	.L2
	movs	r0, #0
	b.n	.L1
.L2:
	cmp	r0, #3
	bne.n	.L3
	movs	r6, #1
	b.n	.L4
.L3:
	cmp	r0, #103
	bne.n	.L5
	movs	r6, #2
	b.n	.L4
.L5:
	cmp	r0, #100
	ble.n	.L6
	movs	r6, #3
	b.n	.L4
.L6:
	movs	r5, #1
.L4:
	bl	Func_080284dc
	cmp	r6, #0
	beq.n	.L7
	cmp	r6, #3
	bne.n	.L8
.L7:
	movs	r0, #21
	bl	Func_080287a8
.L8:
	cmp	r6, #1
	bhi.n	.L9
	movs	r0, #22
	bl	Func_080287a8
.L9:
	cmp	r6, #0
	beq.n	.L10
	cmp	r6, #3
	bne.n	.L11
.L10:
	movs	r0, #23
	bl	Func_080287a8
.L11:
	movs	r0, #24
	bl	Func_080287a8
	ldr	r3, [pc, #76]
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	beq.n	.L12
	movs	r0, #29
	bl	Func_080287a8
.L12:
	ldr	r3, [pc, #64]
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	beq.n	.L13
	movs	r0, #30
	bl	Func_080287a8
.L13:
	movs	r0, #17
	movs	r1, #7
	movs	r2, #0
	bl	Func_08028808
	adds	r0, r5, #0
	bl	Func_08028574
	adds	r5, r0, #0
	bl	Func_0802851c
	cmp	r5, #0
	blt.n	.L14
	lsls	r3, r6, #1
	adds	r3, r3, r6
	ldr	r2, [pc, #20]
	lsls	r3, r3, #1
	adds	r3, r5, r3
	ldrsb	r5, [r2, r3]
.L14:
	adds	r0, r5, #0
.L1:
	pop	{r5, r6}
	pop	{r1}
	bx	r1
	.4byte 0x0200200c
	.4byte 0x02002010
	.4byte 0x0803740f
