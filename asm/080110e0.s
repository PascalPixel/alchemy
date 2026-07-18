@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080110e0
	.thumb_func
Func_080110e0:
	push	{r5, lr}
	lsrs	r3, r0, #31
	adds	r3, r0, r3
	movs	r2, #31
	asrs	r3, r3, #1
	ands	r3, r2
	ldr	r2, [pc, #104]
	lsls	r3, r3, #7
	adds	r4, r3, r2
	movs	r3, #62
	ands	r3, r0
	ldr	r5, [pc, #100]
	lsls	r3, r3, #6
	adds	r1, r3, r5
	movs	r0, #0
.L0:
	ldrh	r2, [r4, #0]
	ldr	r5, [pc, #92]
	lsls	r2, r2, #2
	adds	r3, r2, r5
	ldrh	r3, [r3, #0]
	adds	r5, #2
	strh	r3, [r1, #0]
	adds	r3, r2, r5
	ldrh	r3, [r3, #0]
	adds	r2, r1, #0
	adds	r2, #64
	adds	r0, #1
	strh	r3, [r2, #0]
	adds	r1, #2
	adds	r4, #4
	cmp	r0, #31
	bls.n	.L0
	movs	r2, #252
	movs	r3, #248
	lsls	r2, r2, #4
	lsls	r3, r3, #4
	adds	r1, r1, r2
	adds	r4, r4, r3
	movs	r0, #0
.L1:
	ldrh	r2, [r4, #0]
	ldr	r5, [pc, #44]
	lsls	r2, r2, #2
	adds	r3, r2, r5
	ldrh	r3, [r3, #0]
	adds	r5, #2
	strh	r3, [r1, #0]
	adds	r3, r2, r5
	ldrh	r3, [r3, #0]
	adds	r2, r1, #0
	adds	r2, #64
	adds	r0, #1
	strh	r3, [r2, #0]
	adds	r1, #2
	adds	r4, #4
	cmp	r0, #31
	bls.n	.L1
	pop	{r5}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x02020000
	.4byte 0x06004000
	.4byte 0x02010000
