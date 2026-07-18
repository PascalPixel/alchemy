@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0801aeec
	.thumb_func
Func_0801aeec:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #88]
	ldr	r3, [r3, #0]
	lsrs	r4, r3, #2
	movs	r3, #7
	mov	r8, r1
	ands	r4, r3
	movs	r3, #52
	mov	r2, r8
	muls	r2, r3
	adds	r6, r0, #0
	adds	r3, r2, r6
	ldrh	r3, [r3, #10]
	cmp	r3, #0
	bne.n	.L0
	b.n	.L1
.L0:
	ldr	r0, [pc, #60]
	adds	r3, r6, r2
	adds	r2, #16
	ldrh	r1, [r6, r2]
	adds	r5, r3, #0
	mov	ip, r0
	adds	r5, #40
	mov	r3, ip
	ldr	r7, [pc, #48]
	ands	r3, r1
	ldrh	r1, [r5, #6]
	adds	r0, r7, #0
	ands	r0, r1
	orrs	r0, r3
	strh	r0, [r5, #6]
	adds	r2, r6, r2
	ldrh	r3, [r2, #2]
	mov	r1, r8
	strb	r3, [r5, #4]
	cmp	r1, #0
	beq.n	.L2
	ldrh	r2, [r6, #60]
	adds	r3, r2, #0
	ldr	r1, [pc, #24]
	cmp	r3, #0
	beq.n	.L3
	lsls	r3, r0, #23
	lsrs	r3, r3, #23
	adds	r3, r3, r2
	b.n	.L4
	.4byte 0x03001800
	.4byte 0x000001ff
	.4byte 0xfffffe00
	.4byte 0x080342f8
.L2:
	ldrh	r2, [r6, #8]
	adds	r3, r2, #0
	ldr	r1, [pc, #80]
	cmp	r3, #0
	beq.n	.L3
	lsls	r3, r0, #23
	lsrs	r3, r3, #23
	subs	r3, r3, r2
.L4:
	mov	r2, ip
	ands	r3, r2
	ands	r0, r7
	orrs	r0, r3
	strh	r0, [r5, #6]
.L3:
	movs	r3, #52
	mov	r0, r8
	muls	r0, r3
	adds	r3, r0, #0
	adds	r3, #12
	lsls	r2, r4, #7
	adds	r2, r1, r2
	ldrh	r0, [r6, r3]
	movs	r1, #128
	bl	Func_08003fa4
	ldr	r3, [pc, #32]
	ldrh	r2, [r5, #8]
	ands	r0, r3
	ldr	r3, [pc, #36]
	ands	r3, r2
	orrs	r3, r0
	strh	r3, [r5, #8]
	ldr	r0, [pc, #32]
	bl	Func_080770c0
	cmp	r0, #0
	beq.n	.L5
	ldr	r1, [pc, #24]
	adds	r3, r6, r1
	ldrh	r3, [r3, #0]
	cmp	r3, #1
	bne.n	.L6
	b.n	.L7
	.4byte 0x000003ff
	.4byte 0x08033ef8
	.4byte 0xfffffc00
	.4byte 0x00000103
	.4byte 0x000002e2
.L7:
	ldrb	r3, [r5, #5]
	movs	r2, #13
	negs	r2, r2
	ands	r2, r3
	movs	r3, #4
	orrs	r2, r3
	strb	r2, [r5, #5]
	b.n	.L5
.L6:
	ldrb	r2, [r5, #5]
	movs	r3, #13
	negs	r3, r3
	ands	r3, r2
	strb	r3, [r5, #5]
.L5:
	movs	r1, #238
	adds	r0, r5, #0
	bl	Func_08003dec
	movs	r3, #52
	mov	r2, r8
	muls	r2, r3
	adds	r3, r2, #0
	adds	r1, r3, #0
	adds	r1, #8
	ldrh	r2, [r6, r1]
	adds	r3, r2, #0
	cmp	r3, #0
	beq.n	.L1
	ldr	r0, [pc, #16]
	adds	r3, r2, r0
	strh	r3, [r6, r1]
.L1:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x0000ffff
