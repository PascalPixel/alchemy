@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08006088
	.thumb_func
Func_08006088:
	push	{r5, r6, r7, lr}
	ldr	r3, [pc, #84]
	ldr	r5, [pc, #84]
	ldr	r7, [r3, #0]
	ldrb	r3, [r5, #1]
	adds	r6, r0, #0
	adds	r0, r1, #0
	cmp	r3, #1
	bne.n	.L0
	bl	Func_0800615c
	adds	r0, r6, #0
	bl	Func_080060e8
	ldrb	r3, [r5, #11]
	adds	r3, #1
	strb	r3, [r5, #11]
.L0:
	ldrb	r3, [r5, #2]
	ldrb	r2, [r5, #3]
	lsls	r3, r3, #8
	orrs	r2, r3
	ldrb	r3, [r5, #0]
	cmp	r3, #8
	bne.n	.L1
	movs	r3, #128
	orrs	r2, r3
.L1:
	ldrb	r3, [r5, #9]
	adds	r0, r2, #0
	cmp	r3, #0
	beq.n	.L2
	movs	r3, #128
	lsls	r3, r3, #5
	orrs	r0, r3
.L2:
	lsls	r3, r7, #26
	lsrs	r3, r3, #30
	cmp	r3, #1
	bls.n	.L3
	movs	r3, #128
	lsls	r3, r3, #6
	orrs	r0, r3
.L3:
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x04000128
	.4byte 0x02002240
