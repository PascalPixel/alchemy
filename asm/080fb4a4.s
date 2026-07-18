@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080fb4a4
	.thumb_func
Func_080fb4a4:
	push	{r4, r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	adds	r6, r0, #0
	lsls	r1, r1, #16
	lsrs	r1, r1, #16
	mov	sl, r1
	lsls	r2, r2, #24
	lsrs	r2, r2, #24
	mov	r8, r2
	ldr	r1, [r6, #52]
	ldr	r0, [pc, #84]
	cmp	r1, r0
	bne.n	.L0
	adds	r0, r1, #1
	str	r0, [r6, #52]
	ldrb	r5, [r6, #8]
	ldr	r4, [r6, #44]
	movs	r7, #1
	cmp	r5, #0
	ble.n	.L1
	mov	r9, r8
.L3:
	mov	r0, sl
	ands	r0, r7
	cmp	r0, #0
	beq.n	.L2
	movs	r0, #128
	ldrb	r1, [r4, #0]
	ands	r0, r1
	cmp	r0, #0
	beq.n	.L2
	mov	r0, r8
	strb	r0, [r4, #25]
	mov	r1, r9
	cmp	r1, #0
	bne.n	.L2
	adds	r0, r4, #0
	bl	Func_080fb410
.L2:
	subs	r5, #1
	adds	r4, #80
	lsls	r7, r7, #1
	cmp	r5, #0
	bgt.n	.L3
.L1:
	ldr	r0, [pc, #16]
	str	r0, [r6, #52]
.L0:
	pop	{r3, r4, r5}
	mov	r8, r3
	mov	r9, r4
	mov	sl, r5
	pop	{r4, r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x68736d53
