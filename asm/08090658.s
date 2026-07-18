@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08090658
	.thumb_func
Func_08090658:
	push	{r5, r6, r7, lr}
	ldr	r3, [pc, #276]
	ldr	r1, [pc, #276]
	ldr	r6, [r3, #0]
	adds	r4, r6, r1
	movs	r2, #0
	ldrsb	r2, [r4, r2]
	cmp	r2, #0
	beq.n	.L0
	ldr	r3, [pc, #268]
	adds	r1, r6, r3
	movs	r3, #0
	ldrsb	r3, [r1, r3]
	ldrb	r0, [r1, #0]
	cmp	r3, r2
	blt.n	.L1
	movs	r3, #0
	strb	r3, [r4, #0]
	ldr	r0, [pc, #252]
	bl	Func_08004278
	ldr	r2, [pc, #252]
	ldr	r3, [pc, #252]
	ldrh	r1, [r2, #10]
	ands	r3, r1
	strh	r3, [r2, #10]
	ldr	r3, [pc, #248]
	ldrh	r1, [r2, #10]
	ands	r3, r1
	strh	r3, [r2, #10]
	ldrh	r3, [r2, #10]
	b.n	.L2
.L1:
	ldr	r7, [pc, #240]
	adds	r3, r6, r7
	movs	r2, #0
	ldrsb	r2, [r3, r2]
	ldr	r3, [pc, #236]
	adds	r5, r6, r3
	movs	r3, #0
	ldrsb	r3, [r5, r3]
	subs	r2, r2, r3
	adds	r3, r0, #1
	strb	r3, [r1, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	adds	r0, r3, #0
	muls	r0, r2
	movs	r1, #0
	ldrsb	r1, [r4, r1]
	ldr	r3, [pc, #216]
	bl	Func_080072f0
	movs	r3, #0
	ldrsb	r3, [r5, r3]
	subs	r7, #17
	adds	r3, r3, r0
	adds	r2, r6, r7
	strh	r3, [r2, #0]
.L0:
	ldr	r1, [pc, #200]
	ldr	r2, [pc, #204]
	adds	r3, r6, r1
	ldrh	r3, [r3, #0]
	adds	r1, r6, r2
	subs	r0, r3, #1
	ldrb	r3, [r1, #0]
	movs	r2, #1
	eors	r3, r2
	strb	r3, [r1, #0]
	movs	r3, #32
	ands	r3, r0
	movs	r5, #0
	cmp	r3, #0
	beq.n	.L3
	movs	r5, #15
.L3:
	movs	r3, #31
	ands	r3, r0
	lsls	r0, r3, #1
	ldr	r3, [pc, #172]
	movs	r7, #63
	movs	r4, #0
	mov	ip, r3
	mov	lr, r7
.L6:
	mov	r1, lr
	adds	r3, r0, #0
	ands	r3, r1
	mov	r7, ip
	ldrb	r2, [r7, r3]
	movs	r7, #161
	lsrs	r3, r2, #1
	adds	r3, r6, r3
	lsls	r7, r7, #3
	adds	r1, r3, r7
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L4
	ldrb	r3, [r1, #0]
	movs	r2, #15
	ands	r2, r3
	lsls	r3, r5, #4
	orrs	r2, r3
	strb	r2, [r1, #0]
	b.n	.L5
.L4:
	ldrb	r2, [r1, #0]
	movs	r3, #240
	ands	r3, r2
	orrs	r3, r5
	strb	r3, [r1, #0]
.L5:
	adds	r4, #1
	adds	r0, #1
	cmp	r4, #1
	bls.n	.L6
	ldr	r1, [pc, #104]
	ldr	r0, [pc, #108]
	ldrh	r3, [r0, #0]
	adds	r4, r3, #0
	strh	r0, [r0, #0]
	ldrh	r2, [r1, #0]
	cmp	r2, #31
	bgt.n	.L7
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	adds	r2, #1
	adds	r3, r3, r1
	strh	r2, [r1, #0]
	movs	r1, #161
	lsls	r1, r1, #3
	adds	r3, #4
	adds	r2, r6, r1
	stmia	r3!, {r2}
	movs	r2, #192
	lsls	r2, r2, #19
	stmia	r3!, {r2}
	ldr	r2, [pc, #68]
	str	r2, [r3, #0]
.L7:
	strh	r4, [r0, #0]
.L2:
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001ecc
	.4byte 0x0000053c
	.4byte 0x0000053d
	.4byte 0x08090659
	.4byte 0x040000b0
	.4byte 0x0000c5ff
	.4byte 0x00007fff
	.4byte 0x0000053b
	.4byte 0x0000053a
	.4byte 0x03000380
	.4byte 0x0000052a
	.4byte 0x00000539
	.4byte 0x0809e8ee
	.4byte 0x02002090
	.4byte 0x04000208
	.4byte 0x84000008
