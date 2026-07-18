@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080a2144
	.thumb_func
Func_080a2144:
	push	{r5, lr}
	movs	r3, #160
	lsls	r0, r0, #5
	lsls	r3, r3, #19
	adds	r5, r0, r3
	adds	r1, r5, #0
	ldr	r3, [pc, #60]
	ldr	r0, [pc, #64]
	ldr	r2, [pc, #64]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r0, [pc, #56]
	ldr	r2, [pc, #60]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldrh	r2, [r5, #8]
	lsls	r3, r2, #16
	lsrs	r4, r3, #26
	lsrs	r1, r3, #21
	ldr	r3, [pc, #32]
	movs	r0, #31
	adds	r4, #9
	ands	r1, r3
	ands	r0, r2
	cmp	r4, #31
	bls.n	.L0
	movs	r4, #31
.L0:
	adds	r1, #9
	cmp	r1, #31
	bls.n	.L1
	movs	r1, #31
.L1:
	adds	r0, #9
	cmp	r0, #31
	bls.n	.L2
	movs	r0, #31
	b.n	.L2
	.4byte 0x0000001f
	.4byte 0x040000d4
	.4byte 0x050001e0
	.4byte 0x80000010
	.4byte 0x84000008
.L2:
	lsls	r3, r4, #10
	lsls	r2, r1, #5
	orrs	r3, r2
	orrs	r3, r0
	strh	r3, [r5, #8]
	pop	{r5}
	pop	{r0}
	bx	r0
