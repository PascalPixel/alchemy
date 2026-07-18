@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080fb518
	.thumb_func
Func_080fb518:
	push	{r4, r5, r6, lr}
	adds	r4, r0, #0
	adds	r6, r1, #0
	ldr	r1, [r6, #64]
	ldrb	r5, [r1, #0]
	adds	r2, r1, #1
	str	r2, [r6, #64]
	ldr	r0, [r4, #24]
	ldrb	r1, [r1, #1]
	adds	r3, r1, r0
	adds	r0, r2, #1
	str	r0, [r6, #64]
	ldrb	r2, [r2, #1]
	adds	r0, #1
	str	r0, [r6, #64]
	cmp	r5, #17
	bls.n	.L0
	b.n	.L1
.L0:
	lsls	r0, r5, #2
	ldr	r1, [pc, #8]
	adds	r0, r0, r1
	ldr	r0, [r0, #0]
	mov	pc, r0
	movs	r0, r0
	.4byte 0x080fb54c
	.4byte 0x080fb594
	.4byte 0x080fb598
	.4byte 0x080fb5a0
	.4byte 0x080fb5a8
	.4byte 0x080fb5b2
	.4byte 0x080fb5c0
	.4byte 0x080fb5ce
	.4byte 0x080fb5d6
	.4byte 0x080fb5de
	.4byte 0x080fb5e6
	.4byte 0x080fb5ee
	.4byte 0x080fb5f6
	.4byte 0x080fb5fe
	.4byte 0x080fb60c
	.4byte 0x080fb61a
	.4byte 0x080fb628
	.4byte 0x080fb636
	.4byte 0x080fb644
	strb	r2, [r3, #0]
	b.n	.L1
	ldrb	r1, [r3, #0]
	adds	r0, r1, r2
	strb	r0, [r3, #0]
	b.n	.L1
	ldrb	r1, [r3, #0]
	subs	r0, r1, r2
	strb	r0, [r3, #0]
	b.n	.L1
	ldr	r0, [r4, #24]
	adds	r0, r0, r2
	ldrb	r0, [r0, #0]
	strb	r0, [r3, #0]
	b.n	.L1
	ldr	r0, [r4, #24]
	adds	r0, r0, r2
	ldrb	r1, [r3, #0]
	ldrb	r0, [r0, #0]
	adds	r0, r1, r0
	strb	r0, [r3, #0]
	b.n	.L1
	ldr	r0, [r4, #24]
	adds	r0, r0, r2
	ldrb	r1, [r3, #0]
	ldrb	r0, [r0, #0]
	subs	r0, r1, r0
	strb	r0, [r3, #0]
	b.n	.L1
	ldrb	r3, [r3, #0]
	cmp	r3, r2
	beq.n	.L2
	b.n	.L3
	ldrb	r3, [r3, #0]
	cmp	r3, r2
	bne.n	.L2
	b.n	.L3
	ldrb	r3, [r3, #0]
	cmp	r3, r2
	bhi.n	.L2
	b.n	.L3
	ldrb	r3, [r3, #0]
	cmp	r3, r2
	bcs.n	.L2
	b.n	.L3
	ldrb	r3, [r3, #0]
	cmp	r3, r2
	bls.n	.L2
	b.n	.L3
	ldrb	r3, [r3, #0]
	cmp	r3, r2
	bcc.n	.L2
	b.n	.L3
	ldr	r0, [r4, #24]
	adds	r0, r0, r2
	ldrb	r3, [r3, #0]
	ldrb	r0, [r0, #0]
	cmp	r3, r0
	beq.n	.L2
	b.n	.L3
	ldr	r0, [r4, #24]
	adds	r0, r0, r2
	ldrb	r3, [r3, #0]
	ldrb	r0, [r0, #0]
	cmp	r3, r0
	bne.n	.L2
	b.n	.L3
	ldr	r0, [r4, #24]
	adds	r0, r0, r2
	ldrb	r3, [r3, #0]
	ldrb	r0, [r0, #0]
	cmp	r3, r0
	bhi.n	.L2
	b.n	.L3
	ldr	r0, [r4, #24]
	adds	r0, r0, r2
	ldrb	r3, [r3, #0]
	ldrb	r0, [r0, #0]
	cmp	r3, r0
	bcs.n	.L2
	b.n	.L3
	ldr	r0, [r4, #24]
	adds	r0, r0, r2
	ldrb	r3, [r3, #0]
	ldrb	r0, [r0, #0]
	cmp	r3, r0
	bls.n	.L2
	b.n	.L3
	ldr	r0, [r4, #24]
	adds	r0, r0, r2
	ldrb	r3, [r3, #0]
	ldrb	r0, [r0, #0]
	cmp	r3, r0
	bcs.n	.L3
.L2:
	ldr	r0, [pc, #12]
	ldr	r2, [r0, #0]
	adds	r0, r4, #0
	adds	r1, r6, #0
	bl	Func_080072ec
	b.n	.L1
	movs	r0, r0
	.4byte 0x02004004
.L3:
	ldr	r0, [r6, #64]
	adds	r0, #4
	str	r0, [r6, #64]
.L1:
	pop	{r4, r5, r6}
	pop	{r0}
	bx	r0
