@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08016418
	.thumb_func
Func_08016418:
	push	{r5, r6, r7, lr}
	adds	r5, r0, #0
	adds	r7, r1, #0
	cmp	r5, #0
	beq.n	.L0
	bl	Func_08016478
	ldrh	r3, [r5, #12]
	strh	r3, [r5, #28]
	ldrh	r3, [r5, #14]
	strh	r3, [r5, #30]
	ldrh	r3, [r5, #8]
	strh	r3, [r5, #32]
	ldrh	r3, [r5, #10]
	movs	r6, #0
	strh	r6, [r5, #22]
	strh	r3, [r5, #34]
	cmp	r7, #0
	beq.n	.L1
	ldrh	r0, [r5, #12]
	ldrh	r1, [r5, #14]
	ldrh	r2, [r5, #8]
	ldrh	r3, [r5, #10]
	bl	Func_08016178
	str	r6, [r5, #0]
	str	r6, [r5, #4]
	strh	r6, [r5, #8]
	strh	r6, [r5, #10]
	strh	r6, [r5, #12]
	strh	r6, [r5, #14]
	strh	r6, [r5, #16]
	strh	r6, [r5, #18]
	strh	r6, [r5, #20]
	strh	r6, [r5, #22]
	strh	r6, [r5, #24]
	strh	r6, [r5, #26]
	strh	r6, [r5, #28]
	strh	r6, [r5, #30]
	strh	r6, [r5, #32]
	strh	r6, [r5, #34]
	b.n	.L0
.L1:
	movs	r3, #4
	strh	r7, [r5, #24]
	strh	r3, [r5, #26]
.L0:
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
