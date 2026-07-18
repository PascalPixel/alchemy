@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08029504
	.thumb_func
Func_08029504:
	push	{r5, r6, lr}
.L3:
	bl	Func_0801db70
	movs	r6, #1
	adds	r5, r0, #0
	negs	r6, r6
	adds	r0, r6, #0
	cmp	r5, r6
	beq.n	.L0
	cmp	r5, #0
	bne.n	.L1
	bl	Func_080207c4
	cmp	r0, r6
	bne.n	.L2
	b.n	.L3
.L1:
	cmp	r5, #1
	bne.n	.L4
	ldr	r0, [pc, #32]
	movs	r1, #1
	bl	Func_0801776c
	ldr	r3, [pc, #28]
	strb	r5, [r3, #0]
	b.n	.L2
.L4:
	cmp	r5, #2
	bne.n	.L2
	bl	Func_0801d4cc
	cmp	r0, r6
	beq.n	.L3
.L2:
	movs	r0, #0
.L0:
	pop	{r5, r6}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x00000c2a
	.4byte 0x03001cc8
