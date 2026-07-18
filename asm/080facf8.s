@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080facf8
	.thumb_func
Func_080facf8:
	push	{r4, r5, r6, r7, lr}
	lsls	r0, r0, #24
	lsrs	r0, r0, #24
	lsls	r1, r1, #24
	lsrs	r5, r1, #24
	lsls	r2, r2, #24
	lsrs	r2, r2, #24
	mov	ip, r2
	cmp	r0, #4
	bne.n	.L0
	cmp	r5, #20
	bhi.n	.L1
	movs	r5, #0
	b.n	.L2
.L1:
	adds	r0, r5, #0
	subs	r0, #21
	lsls	r0, r0, #24
	lsrs	r5, r0, #24
	cmp	r5, #59
	bls.n	.L2
	movs	r5, #59
.L2:
	ldr	r0, [pc, #8]
	adds	r0, r5, r0
	ldrb	r0, [r0, #0]
	b.n	.L3
	movs	r0, r0
	.4byte 0x080fb9c8
.L0:
	cmp	r5, #35
	bhi.n	.L4
	movs	r0, #0
	mov	ip, r0
	movs	r5, #0
	b.n	.L5
.L4:
	adds	r0, r5, #0
	subs	r0, #36
	lsls	r0, r0, #24
	lsrs	r5, r0, #24
	cmp	r5, #130
	bls.n	.L5
	movs	r5, #130
	movs	r1, #255
	mov	ip, r1
.L5:
	ldr	r3, [pc, #72]
	adds	r0, r5, r3
	ldrb	r6, [r0, #0]
	ldr	r4, [pc, #68]
	movs	r2, #15
	adds	r0, r6, #0
	ands	r0, r2
	lsls	r0, r0, #1
	adds	r0, r0, r4
	movs	r7, #0
	ldrsh	r1, [r0, r7]
	asrs	r0, r6, #4
	adds	r6, r1, #0
	asrs	r6, r0
	adds	r0, r5, #1
	adds	r0, r0, r3
	ldrb	r1, [r0, #0]
	adds	r0, r1, #0
	ands	r0, r2
	lsls	r0, r0, #1
	adds	r0, r0, r4
	movs	r2, #0
	ldrsh	r0, [r0, r2]
	asrs	r1, r1, #4
	asrs	r0, r1
	subs	r0, r0, r6
	mov	r7, ip
	muls	r7, r0
	adds	r0, r7, #0
	asrs	r0, r0, #8
	adds	r0, r6, r0
	movs	r1, #128
	lsls	r1, r1, #4
	adds	r0, r0, r1
.L3:
	pop	{r4, r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x080fb92c
	.4byte 0x080fb9b0
