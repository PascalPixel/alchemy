@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080c1df4
	.thumb_func
Func_080c1df4:
	push	{r5, r6, r7, lr}
	ldr	r3, [pc, #188]
	ldr	r5, [r3, #0]
	adds	r6, r5, #0
	adds	r6, #64
	ldrb	r4, [r6, #0]
	movs	r1, #0
	sub	sp, #4
	cmp	r1, r4
	bge.n	.L0
	ldrh	r3, [r5, #16]
	cmp	r3, r0
	beq.n	.L0
	adds	r2, r5, #0
	adds	r2, #16
.L1:
	adds	r1, #1
	cmp	r1, r4
	bge.n	.L0
	adds	r2, #2
	ldrh	r3, [r2, #0]
	cmp	r3, r0
	bne.n	.L1
.L0:
	cmp	r1, r4
	beq.n	.L2
	adds	r6, r1, #0
	adds	r6, #52
	ldrsb	r3, [r5, r6]
	movs	r4, #0
	cmp	r3, #0
	bge.n	.L3
	movs	r3, #1
	strb	r3, [r5, r6]
	lsls	r3, r1, #2
	adds	r3, #28
	movs	r2, #3
	str	r2, [r5, r3]
	ldr	r0, [pc, #120]
	b.n	.L4
.L3:
	lsls	r7, r1, #2
	b.n	.L5
.L7:
	adds	r4, #1
.L5:
	cmp	r4, #31
	bgt.n	.L6
	ldrsb	r0, [r5, r6]
	movs	r1, #9
	adds	r0, #1
	str	r4, [sp, #0]
	bl	Func_080022fc
	adds	r3, r7, #0
	strb	r0, [r5, r6]
	adds	r3, #28
	lsls	r0, r0, #24
	asrs	r0, r0, #24
	movs	r2, #1
	ldr	r3, [r5, r3]
	lsls	r2, r0
	ands	r3, r2
	ldr	r4, [sp, #0]
	cmp	r3, #0
	bne.n	.L7
.L6:
	ldrsb	r3, [r5, r6]
	adds	r1, r7, #0
	adds	r1, #28
	movs	r2, #1
	lsls	r2, r3
	ldr	r3, [r5, r1]
	orrs	r3, r2
	str	r3, [r5, r1]
	ldrsb	r0, [r5, r6]
	b.n	.L4
.L2:
	cmp	r4, #4
	bgt.n	.L8
	movs	r1, #1
	adds	r2, r4, #0
	negs	r1, r1
	adds	r2, #52
	adds	r3, r1, #0
	strb	r3, [r5, r2]
	lsls	r3, r4, #1
	adds	r3, #16
	strh	r0, [r5, r3]
	lsls	r3, r4, #2
	adds	r3, #28
	movs	r2, #0
	str	r2, [r5, r3]
	adds	r3, r4, #1
	strb	r3, [r6, #0]
	movs	r0, #9
	b.n	.L4
.L8:
	movs	r0, #1
	negs	r0, r0
.L4:
	add	sp, #4
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x03001e74
	.4byte 0x00008001
