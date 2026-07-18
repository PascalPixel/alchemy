@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08019d2c
	.thumb_func
Func_08019d2c:
	push	{r5, lr}
	movs	r1, #1
	negs	r1, r1
	movs	r2, #0
	cmp	r0, #19
	bhi.n	.L0
	ldr	r4, [pc, #100]
	movs	r5, #0
	ldrsh	r3, [r4, r5]
	cmp	r3, r1
	beq.n	.L1
	cmp	r3, r0
	bne.n	.L2
	movs	r2, #2
	ldrsh	r1, [r4, r2]
	b.n	.L1
.L2:
	adds	r2, #2
	lsls	r3, r2, #1
	ldrsh	r3, [r4, r3]
	movs	r5, #1
	negs	r5, r5
	cmp	r3, r5
	beq.n	.L1
	cmp	r3, r0
	bne.n	.L2
	adds	r2, #1
	lsls	r3, r2, #1
	ldrsh	r1, [r4, r3]
	b.n	.L1
.L0:
	ldr	r4, [pc, #60]
	movs	r5, #0
	ldrsh	r3, [r4, r5]
	movs	r5, #1
	negs	r5, r5
	cmp	r3, r5
	beq.n	.L1
	cmp	r3, r0
	bne.n	.L3
	movs	r2, #2
	ldrsh	r1, [r4, r2]
	b.n	.L4
.L3:
	adds	r2, #2
	lsls	r3, r2, #1
	ldrsh	r3, [r4, r3]
	movs	r5, #1
	negs	r5, r5
	cmp	r3, r5
	beq.n	.L1
	cmp	r3, r0
	bne.n	.L3
	adds	r2, #1
	lsls	r3, r2, #1
	ldrsh	r1, [r4, r3]
.L4:
	adds	r1, #128
.L1:
	adds	r0, r1, #0
	pop	{r5}
	pop	{r1}
	bx	r1
	.4byte 0x080367e4
	.4byte 0x0803680c
