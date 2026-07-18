@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0800711c
	.thumb_func
Func_0800711c:
	push	{r4, r5, r6, lr}
	sub	sp, #64
	lsls	r0, r0, #16
	lsrs	r4, r0, #16
	cmp	r4, #15
	bls.n	.L0
	ldr	r0, [pc, #0]
	b.n	.L1
	.4byte 0x000080ff
.L0:
	mov	r0, sp
	bl	Func_08006ac0
	ldr	r2, [pc, #24]
	ldrh	r0, [r2, #0]
	ldr	r1, [pc, #24]
	ands	r0, r1
	ldr	r1, [pc, #24]
	ldrh	r1, [r1, #36]
	orrs	r0, r1
	strh	r0, [r2, #0]
	lsls	r0, r4, #21
	lsrs	r5, r0, #16
	movs	r6, #0
.L6:
	movs	r4, #2
	b.n	.L2
	.4byte 0x04000204
	.4byte 0x0000fffc
	.4byte 0x08007c10
.L4:
	subs	r0, r4, #1
	lsls	r0, r0, #16
	lsrs	r4, r0, #16
	cmp	r4, #0
	beq.n	.L3
.L2:
	adds	r0, r5, #0
	bl	Func_08007098
	lsls	r0, r0, #16
	lsrs	r3, r0, #16
	cmp	r3, #0
	bne.n	.L4
.L3:
	adds	r0, r5, #1
	lsls	r0, r0, #16
	lsrs	r5, r0, #16
	cmp	r3, #0
	bne.n	.L5
	adds	r0, r6, #1
	lsls	r0, r0, #16
	lsrs	r6, r0, #16
	cmp	r6, #31
	bls.n	.L6
.L5:
	ldr	r2, [pc, #20]
	ldrh	r0, [r2, #0]
	ldr	r1, [pc, #20]
	ands	r0, r1
	movs	r1, #3
	orrs	r0, r1
	strh	r0, [r2, #0]
	adds	r0, r3, #0
.L1:
	add	sp, #64
	pop	{r4, r5, r6}
	pop	{r1}
	bx	r1
	.4byte 0x04000204
	.4byte 0x0000fffc
