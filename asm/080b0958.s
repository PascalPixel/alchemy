@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080b0958
	.thumb_func
Func_080b0958:
	push	{r5, lr}
	adds	r5, r0, #0
	ldr	r4, [r5, #0]
	cmp	r4, #0
	beq.n	.L0
	movs	r1, #8
	ldrsh	r3, [r5, r1]
	ldrh	r2, [r4, #6]
	subs	r0, r2, r3
	adds	r3, r0, #0
	cmp	r0, #0
	bge.n	.L1
	adds	r3, r0, #3
.L1:
	asrs	r3, r3, #2
	cmp	r3, #0
	bge.n	.L2
	negs	r3, r3
.L2:
	cmp	r0, #0
	ble.n	.L3
	cmp	r3, #0
	beq.n	.L4
	subs	r3, r2, r3
	b.n	.L5
.L4:
	ldr	r1, [pc, #104]
	adds	r3, r2, r1
	b.n	.L5
.L3:
	cmp	r0, #0
	bge.n	.L6
	cmp	r3, #0
	beq.n	.L7
	adds	r3, r2, r3
	b.n	.L5
.L7:
	adds	r3, r2, #1
.L5:
	strh	r3, [r4, #6]
	ldrh	r3, [r4, #6]
	ldr	r2, [pc, #84]
	ldrh	r1, [r4, #22]
	ands	r2, r3
	ldr	r3, [pc, #80]
	ands	r3, r1
	orrs	r3, r2
	strh	r3, [r4, #22]
.L6:
	movs	r1, #10
	ldrsh	r3, [r5, r1]
	ldrh	r2, [r4, #8]
	subs	r0, r2, r3
	adds	r3, r0, #0
	cmp	r0, #0
	bge.n	.L8
	adds	r3, r0, #3
.L8:
	asrs	r3, r3, #2
	cmp	r3, #0
	bge.n	.L9
	negs	r3, r3
.L9:
	cmp	r0, #0
	ble.n	.L10
	cmp	r3, #0
	beq.n	.L11
	subs	r3, r2, r3
	b.n	.L12
.L11:
	ldr	r1, [pc, #28]
	adds	r3, r2, r1
	b.n	.L12
.L10:
	cmp	r0, #0
	bge.n	.L0
	cmp	r3, #0
	beq.n	.L13
	adds	r3, r2, r3
	b.n	.L12
.L13:
	adds	r3, r2, #1
.L12:
	strh	r3, [r4, #8]
	ldrh	r3, [r4, #8]
	strb	r3, [r4, #20]
.L0:
	pop	{r5}
	pop	{r0}
	bx	r0
	.4byte 0x0000ffff
	.4byte 0x000001ff
	.4byte 0xfffffe00
