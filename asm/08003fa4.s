@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08003fa4
	.thumb_func
Func_08003fa4:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r5, r0, #0
	ldr	r3, [pc, #124]
	mov	r8, r2
	lsls	r2, r5, #2
	adds	r6, r1, #0
	adds	r7, r2, r3
	movs	r0, #0
	cmp	r5, #95
	bhi.n	.L0
	movs	r2, #128
	lsls	r2, r2, #6
	cmp	r6, r2
	bhi.n	.L0
	ldrh	r3, [r7, #0]
	cmp	r3, #16
	bls.n	.L1
	cmp	r3, r6
	beq.n	.L2
	adds	r0, r5, #0
	bl	Func_08003f3c
	b.n	.L1
.L2:
	ldrh	r5, [r7, #2]
	b.n	.L3
.L1:
	adds	r0, r5, #0
	adds	r1, r6, #0
	bl	Func_08003e58
	adds	r5, r0, #0
.L3:
	movs	r3, #1
	negs	r3, r3
	cmp	r5, r3
	beq.n	.L4
	ldr	r2, [pc, #64]
	adds	r1, r5, r2
	mov	r2, r8
	strh	r6, [r7, #0]
	strh	r5, [r7, #2]
	cmp	r2, #0
	beq.n	.L5
	cmp	r8, r3
	bne.n	.L6
	adds	r0, r1, #0
	ldr	r3, [pc, #48]
	adds	r1, r6, #0
	bl	Func_080072f0
	b.n	.L5
.L6:
	movs	r4, #132
	lsrs	r2, r6, #2
	lsls	r4, r4, #24
	ldr	r3, [pc, #36]
	mov	r0, r8
	orrs	r2, r4
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
.L5:
	lsrs	r0, r5, #5
	b.n	.L0
.L4:
	movs	r0, #0
.L0:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x03001b10
	.4byte 0x06010000
	.4byte 0x03000164
	.4byte 0x040000d4
