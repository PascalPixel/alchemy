@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0800f9f4
	.thumb_func
Func_0800f9f4:
	push	{r5, r6, r7, lr}
	subs	r3, r0, #1
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r4, r3, #1
	movs	r3, #1
	ands	r3, r0
	ldr	r6, [pc, #124]
	ldr	r5, [pc, #124]
	cmp	r3, #0
	beq.n	.L0
	ldr	r3, [pc, #124]
	ldrb	r3, [r3, #0]
	cmp	r3, #1
	beq.n	.L1
	cmp	r3, #1
	bgt.n	.L2
	cmp	r3, #0
	beq.n	.L3
	b.n	.L0
.L2:
	cmp	r3, #2
	beq.n	.L4
	b.n	.L0
.L3:
	movs	r1, #0
	cmp	r1, r4
	bge.n	.L0
.L5:
	ldrh	r3, [r6, #0]
	adds	r1, #1
	strh	r3, [r5, #0]
	adds	r6, #2
	adds	r5, #2
	cmp	r1, r4
	blt.n	.L5
	b.n	.L0
.L1:
	ldr	r6, [pc, #68]
	movs	r1, #0
	movs	r7, #0
	adds	r0, r4, r6
	cmp	r1, r4
	bge.n	.L0
.L6:
	ldrb	r3, [r6, #0]
	ldrb	r2, [r0, #0]
	lsls	r3, r3, #8
	orrs	r3, r2
	eors	r3, r7
	adds	r1, #1
	strh	r3, [r5, #0]
	adds	r0, #1
	adds	r6, #1
	adds	r5, #2
	adds	r7, r3, #0
	cmp	r1, r4
	blt.n	.L6
	b.n	.L0
.L4:
	movs	r2, #0
	cmp	r4, #0
	ble.n	.L0
	adds	r1, r4, #0
.L7:
	ldrh	r3, [r6, #0]
	subs	r1, #1
	eors	r3, r2
	strh	r3, [r5, #0]
	adds	r6, #2
	adds	r5, #2
	adds	r2, r3, #0
	cmp	r1, #0
	bne.n	.L7
.L0:
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x02010002
	.4byte 0x02020000
	.4byte 0x02010001
