@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08006f84
	.thumb_func
Func_08006f84:
	push	{r4, r5, r6, r7, lr}
	sub	sp, #64
	adds	r7, r1, #0
	lsls	r0, r0, #16
	lsrs	r4, r0, #16
	cmp	r4, #15
	bls.n	.L0
	ldr	r0, [pc, #4]
	b.n	Func_08007018
	movs	r0, r0
	.4byte 0x000080ff
.L0:
	adds	r0, r4, #0
	bl	Func_08006d50
	lsls	r0, r0, #16
	lsrs	r5, r0, #16
	cmp	r5, #0
	bne.n	.L1
	mov	r0, sp
	bl	Func_08006ac0
	ldr	r3, [pc, #36]
	ldrh	r1, [r3, #0]
	ldr	r0, [pc, #36]
	ands	r1, r0
	ldr	r0, [pc, #36]
	ldr	r2, [r0, #0]
	ldrh	r0, [r2, #16]
	orrs	r0, r1
	strh	r0, [r3, #0]
	ldr	r1, [pc, #32]
	ldr	r0, [r2, #4]
	strh	r0, [r1, #0]
	ldrb	r0, [r2, #8]
	lsls	r4, r0
	movs	r0, #224
	lsls	r0, r0, #20
	adds	r4, r4, r0
	adds	r6, r1, #0
	b.n	.L2
	movs	r0, r0
	.4byte 0x04000204
	.4byte 0x0000fffc
	.4byte 0x02004c08
	.4byte 0x02004c0c
	ldrh	r0, [r6, #0]
	subs	r0, #1
	strh	r0, [r6, #0]
	adds	r7, #1
	adds	r4, #1
.L2:
	ldrh	r0, [r6, #0]
	cmp	r0, #0
	beq.n	.L3
	adds	r0, r7, #0
	adds	r1, r4, #0
	bl	Func_08006dec
	lsls	r0, r0, #16
	lsrs	r5, r0, #16
	.4byte 0xd0ef2d00
.L3:
	ldr	r2, [pc, #20]
	ldrh	r0, [r2, #0]
	ldr	r1, [pc, #20]
	ands	r0, r1
	movs	r1, #3
	orrs	r0, r1
	strh	r0, [r2, #0]
.L1:
	adds	r0, r5, #0
	.4byte 0xbcf0b010
	pop	{r1}
	bx	r1
	.4byte 0x04000204
	.4byte 0x0000fffc
