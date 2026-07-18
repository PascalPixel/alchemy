@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08090488
	.thumb_func
Func_08090488:
	push	{r5, r6, r7, lr}
	ldr	r3, [pc, #184]
	ldr	r1, [pc, #184]
	ldr	r6, [r3, #0]
	adds	r4, r6, r1
	movs	r2, #0
	ldrsb	r2, [r4, r2]
	subs	r3, #92
	ldr	r7, [r3, #0]
	cmp	r2, #0
	beq.n	.L0
	ldr	r3, [pc, #172]
	adds	r1, r6, r3
	movs	r3, #0
	ldrsb	r3, [r1, r3]
	ldrb	r0, [r1, #0]
	cmp	r3, r2
	blt.n	.L1
	movs	r3, #0
	strb	r3, [r4, #0]
	ldr	r0, [pc, #156]
	bl	Func_08004278
	movs	r0, #1
	movs	r1, #0
	movs	r2, #0
	bl	Func_0800307c
	b.n	.L2
.L1:
	ldr	r2, [pc, #144]
	adds	r3, r6, r2
	movs	r2, #0
	ldrsb	r2, [r3, r2]
	ldr	r3, [pc, #140]
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
	ldr	r3, [pc, #116]
	bl	Func_080072f0
	movs	r3, #0
	ldrsb	r3, [r5, r3]
	ldr	r1, [pc, #112]
	adds	r3, r3, r0
	adds	r2, r6, r1
	strh	r3, [r2, #0]
.L0:
	ldr	r2, [pc, #104]
	adds	r3, r6, r2
	ldrh	r5, [r3, #0]
	cmp	r5, #79
	bls.n	.L3
	movs	r1, #128
	lsls	r1, r1, #1
	adds	r3, r7, r1
	movs	r2, #200
	strh	r2, [r3, #0]
	movs	r3, #129
	lsls	r3, r3, #1
	adds	r2, r7, r3
	movs	r3, #250
	b.n	.L4
.L3:
	cmp	r5, #0
	beq.n	.L5
	ldr	r3, [pc, #72]
	ldr	r3, [r3, #0]
	movs	r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L5
	movs	r1, #128
	lsls	r1, r1, #1
	adds	r2, r5, #0
	adds	r3, r7, r1
	adds	r2, #80
	strh	r2, [r3, #0]
	ldr	r3, [pc, #12]
	movs	r2, #129
	lsls	r2, r2, #1
	subs	r3, r3, r5
	adds	r1, r7, r2
	strh	r3, [r1, #0]
	b.n	.L2
	movs	r0, r0
	.4byte 0x00000050
	.4byte 0x03001ecc
	.4byte 0x0000053c
	.4byte 0x0000053d
	.4byte 0x08090489
	.4byte 0x0000053b
	.4byte 0x0000053a
	.4byte 0x03000380
	.4byte 0x0000052a
	.4byte 0x03001e40
.L5:
	movs	r1, #128
	lsls	r1, r1, #1
	adds	r3, r7, r1
	movs	r2, #0
	strh	r2, [r3, #0]
	movs	r3, #129
	lsls	r3, r3, #1
	adds	r2, r7, r3
	movs	r3, #159
.L4:
	strh	r3, [r2, #0]
.L2:
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
