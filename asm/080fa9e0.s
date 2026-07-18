@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080fa9e0
	.thumb_func
Func_080fa9e0:
	push	{r4, r5, r6, r7, lr}
	adds	r7, r0, #0
	adds	r6, r1, #0
	lsls	r2, r2, #24
	lsrs	r4, r2, #24
	cmp	r4, #0
	beq.n	.L0
	cmp	r4, #16
	bls.n	.L1
	movs	r4, #16
.L1:
	ldr	r0, [pc, #84]
	ldr	r5, [r0, #0]
	ldr	r1, [r5, #0]
	ldr	r0, [pc, #84]
	cmp	r1, r0
	bne.n	.L0
	adds	r0, r1, #1
	str	r0, [r5, #0]
	adds	r0, r7, #0
	bl	Func_080fa68c
	str	r6, [r7, #44]
	strb	r4, [r7, #8]
	movs	r0, #128
	lsls	r0, r0, #24
	str	r0, [r7, #4]
	cmp	r4, #0
	beq.n	.L2
	movs	r1, #0
.L3:
	strb	r1, [r6, #0]
	subs	r0, r4, #1
	lsls	r0, r0, #24
	lsrs	r4, r0, #24
	adds	r6, #80
	cmp	r4, #0
	bne.n	.L3
.L2:
	ldr	r0, [r5, #32]
	cmp	r0, #0
	beq.n	.L4
	str	r0, [r7, #56]
	ldr	r0, [r5, #36]
	str	r0, [r7, #60]
	movs	r0, #0
	str	r0, [r5, #32]
.L4:
	str	r7, [r5, #36]
	ldr	r0, [pc, #24]
	str	r0, [r5, #32]
	ldr	r0, [pc, #16]
	str	r0, [r5, #0]
	str	r0, [r7, #52]
.L0:
	pop	{r4, r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03007ff0
	.4byte 0x68736d53
	.4byte 0x080f9c91
