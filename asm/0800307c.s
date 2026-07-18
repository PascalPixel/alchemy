@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0800307c
	.thumb_func
Func_0800307c:
	push	{r5, r6, lr}
	adds	r5, r1, #0
	adds	r1, r2, #0
	cmp	r0, #13
	bhi.n	.L0
	ldr	r3, [pc, #92]
	ldrh	r2, [r3, #0]
	adds	r6, r2, #0
	strh	r3, [r3, #0]
	movs	r2, #1
	ldr	r4, [pc, #84]
	ldrh	r3, [r4, #0]
	lsls	r2, r0
	bics	r3, r2
	cmp	r1, #0
	beq.n	.L1
	orrs	r3, r2
.L1:
	strh	r3, [r4, #0]
	cmp	r0, #2
	bhi.n	.L2
	movs	r4, #8
	lsls	r4, r0
	mvns	r2, r4
	cmp	r0, #2
	bne.n	.L3
	lsls	r3, r5, #8
	orrs	r4, r3
	movs	r3, #255
	ands	r2, r3
.L3:
	ldr	r5, [pc, #52]
	ldrh	r3, [r5, #0]
	ands	r3, r2
	cmp	r1, #0
	beq.n	.L4
	orrs	r3, r4
.L4:
	strh	r3, [r5, #0]
.L2:
	cmp	r1, #0
	beq.n	.L5
	ldr	r2, [pc, #36]
	lsls	r3, r0, #2
	str	r1, [r2, r3]
	b.n	.L6
.L5:
	ldr	r1, [pc, #28]
	ldr	r3, [pc, #32]
	lsls	r2, r0, #2
	str	r3, [r1, r2]
.L6:
	ldr	r3, [pc, #8]
	strh	r6, [r3, #0]
.L0:
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x04000208
	.4byte 0x04000200
	.4byte 0x04000004
	.4byte 0x030000e0
	.4byte 0x08003009
