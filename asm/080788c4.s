@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080788c4
	.thumb_func
Func_080788c4:
	push	{r5, r6, r7, lr}
	adds	r5, r1, #0
	adds	r7, r0, #0
	bl	Func_08077394
	lsls	r5, r5, #1
	adds	r5, #216
	ldrh	r3, [r0, r5]
	movs	r6, #1
	negs	r6, r6
	cmp	r3, #0
	beq.n	.L0
	movs	r2, #248
	lsls	r2, r2, #8
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L1
	ldr	r2, [pc, #8]
	adds	r3, r3, r2
	strh	r3, [r0, r5]
	movs	r6, #1
	b.n	.L0
	.4byte 0xfffff800
.L1:
	adds	r6, r0, #0
	adds	r6, #216
	strh	r2, [r0, r5]
	adds	r4, r6, #0
	movs	r5, #0
	adds	r1, r6, #0
	movs	r0, #14
.L3:
	ldrh	r2, [r4, #0]
	lsls	r3, r2, #16
	adds	r4, #2
	cmp	r3, #0
	beq.n	.L2
	strh	r2, [r1, #0]
	adds	r5, #1
	adds	r1, #2
.L2:
	subs	r0, #1
	cmp	r0, #0
	bge.n	.L3
	cmp	r5, #14
	bgt.n	.L4
	lsls	r3, r5, #1
	adds	r0, r3, r6
	ldr	r2, [pc, #16]
	movs	r3, #15
	subs	r5, r3, r5
.L5:
	subs	r5, #1
	strh	r2, [r0, #0]
	adds	r0, #2
	cmp	r5, #0
	bne.n	.L5
	b.n	.L4
	movs	r0, r0
	.4byte 0x00000000
.L4:
	movs	r6, #2
.L0:
	adds	r0, r7, #0
	bl	Func_08077428
	adds	r0, r6, #0
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
