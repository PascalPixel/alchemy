@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0808b320
	.thumb_func
Func_0808b320:
	push	{r5, r6, r7, lr}
	lsls	r0, r0, #4
	adds	r0, r0, r1
	lsls	r0, r0, #16
	asrs	r7, r0, #16
	movs	r0, #182
	lsls	r0, r0, #1
	ldr	r5, [pc, #92]
	bl	Func_080770c0
	cmp	r0, #0
	beq.n	.L0
	movs	r6, #18
	b.n	.L1
.L0:
	movs	r2, #0
	ldrsh	r3, [r5, r2]
	lsls	r2, r3, #16
	lsrs	r1, r2, #16
	adds	r5, #2
	cmp	r1, #0
	beq.n	.L1
	lsls	r3, r7, #16
	lsrs	r0, r3, #16
	cmp	r1, r0
	beq.n	.L1
	movs	r4, #128
	ldr	r1, [pc, #56]
	lsls	r4, r4, #8
	mov	ip, r0
.L3:
	lsrs	r2, r2, #16
	adds	r3, r2, #0
	ands	r3, r4
	cmp	r3, #0
	beq.n	.L2
	adds	r6, r2, #0
	ands	r6, r1
.L2:
	movs	r2, #0
	ldrsh	r3, [r5, r2]
	lsls	r2, r3, #16
	lsrs	r3, r2, #16
	adds	r5, #2
	cmp	r3, #0
	beq.n	.L1
	cmp	r3, ip
	bne.n	.L3
.L1:
	ldr	r3, [pc, #24]
	movs	r2, #247
	lsls	r2, r2, #1
	adds	r3, r3, r2
	strh	r6, [r3, #0]
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x0809e488
	.4byte 0x00000fff
	.4byte 0x02000240
