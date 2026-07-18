@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0801bd98
	.thumb_func
Func_0801bd98:
	push	{r5, r6, r7, lr}
	adds	r7, r0, #0
	adds	r6, r1, #0
	sub	sp, #12
	adds	r5, r2, #0
	adds	r1, r3, #0
	cmp	r7, #2
	beq.n	.L0
	cmp	r7, #2
	bhi.n	.L1
	cmp	r7, #1
	beq.n	.L2
	b.n	.L3
.L1:
	cmp	r7, #4
	beq.n	.L4
	cmp	r7, #6
	bne.n	.L3
.L2:
	cmp	r1, #0
	beq.n	.L5
	ldrh	r3, [r5, #12]
	str	r3, [sp, #8]
.L5:
	add	r3, sp, #4
	str	r1, [sp, #0]
	add	r2, sp, #8
	adds	r0, r6, #0
	movs	r1, #0
	bl	Func_08019ee4
	ldr	r3, [pc, #152]
	b.n	.L6
.L0:
	cmp	r1, #0
	beq.n	.L7
	ldrh	r3, [r5, #12]
	str	r3, [sp, #8]
.L7:
	add	r3, sp, #4
	str	r1, [sp, #0]
	add	r2, sp, #8
	adds	r0, r6, #0
	movs	r1, #1
	bl	Func_08019fcc
	ldr	r3, [pc, #132]
	b.n	.L6
.L4:
	cmp	r1, #0
	beq.n	.L8
	ldrh	r3, [r5, #12]
	str	r3, [sp, #8]
.L8:
	add	r3, sp, #4
	str	r1, [sp, #0]
	add	r2, sp, #8
	adds	r0, r6, #0
	movs	r1, #1
	bl	Func_0801a3d0
	ldr	r3, [pc, #108]
.L6:
	adds	r3, r6, r3
	strh	r3, [r5, #32]
.L3:
	ldr	r3, [sp, #8]
	strh	r6, [r5, #8]
	strh	r3, [r5, #12]
	ldr	r6, [sp, #4]
	movs	r3, #128
	lsls	r3, r3, #1
	strh	r6, [r5, #14]
	strh	r7, [r5, #10]
	strh	r3, [r5, #34]
	strh	r3, [r5, #38]
	adds	r0, r5, #0
	adds	r0, #40
	movs	r5, #13
	ldrb	r3, [r0, #5]
	negs	r5, r5
	adds	r2, r5, #0
	ands	r2, r3
	movs	r3, #33
	negs	r3, r3
	ldrb	r1, [r0, #7]
	movs	r4, #63
	ands	r2, r3
	adds	r3, #16
	ands	r2, r3
	adds	r3, r4, #0
	ands	r2, r4
	ands	r3, r1
	movs	r1, #64
	orrs	r3, r1
	strb	r2, [r0, #5]
	ldrb	r2, [r0, #9]
	strb	r3, [r0, #7]
	movs	r3, #15
	ands	r3, r2
	strb	r3, [r0, #9]
	ldr	r3, [pc, #36]
	ldrh	r2, [r0, #8]
	ands	r6, r3
	ldr	r3, [pc, #36]
	ands	r3, r2
	orrs	r3, r6
	strh	r3, [r0, #8]
	ldrb	r3, [r0, #9]
	ands	r5, r3
	strb	r5, [r0, #9]
	add	sp, #12
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x0000001f
	.4byte 0x00000182
	.4byte 0x00000333
	.4byte 0x000003ff
	.4byte 0xfffffc00
