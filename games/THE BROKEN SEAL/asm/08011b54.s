@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08011b54
	.thumb_func
Func_08011b54:
	push	{r5, r6, r7, lr}
	lsls	r2, r2, #16
	lsls	r3, r3, #16
	asrs	r7, r2, #16
	asrs	r2, r3, #16
	ldr	r3, [pc, #96]
	ldr	r4, [r3, #0]
	adds	r5, r4, #0
	lsls	r1, r1, #16
	adds	r5, #176
	asrs	r6, r1, #16
	ldrh	r1, [r5, #0]
	lsls	r0, r0, #16
	asrs	r0, r0, #16
	cmp	r1, #3
	bls.n	.L0
	movs	r0, #1
	negs	r0, r0
	b.n	.L1
.L0:
	movs	r3, #44
	muls	r1, r3
	lsls	r0, r0, #16
	lsls	r3, r6, #16
	lsrs	r3, r3, #16
	lsrs	r0, r0, #12
	adds	r0, r0, r3
	movs	r3, #160
	adds	r1, r4, r1
	lsls	r3, r3, #19
	lsls	r0, r0, #1
	lsls	r2, r2, #16
	movs	r4, #128
	adds	r0, r0, r3
	lsrs	r2, r2, #16
	movs	r3, #0
	lsls	r4, r4, #24
	strh	r3, [r1, #4]
	strh	r3, [r1, #6]
	strh	r2, [r1, #10]
	str	r0, [r1, #0]
	strh	r7, [r1, #8]
	ldr	r3, [pc, #28]
	adds	r1, #12
	orrs	r2, r4
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldrh	r3, [r5, #0]
	adds	r3, #1
	strh	r3, [r5, #0]
	movs	r0, #0
.L1:
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x03001ec0
	.4byte 0x040000d4
