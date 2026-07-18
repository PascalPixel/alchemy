@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080fb334
	.thumb_func
Func_080fb334:
	push	{r4, r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	adds	r4, r0, #0
	lsls	r1, r1, #16
	lsrs	r1, r1, #16
	mov	ip, r1
	lsls	r2, r2, #16
	lsrs	r6, r2, #16
	ldr	r3, [r4, #52]
	ldr	r0, [pc, #84]
	cmp	r3, r0
	bne.n	.L0
	adds	r0, r3, #1
	str	r0, [r4, #52]
	ldrb	r2, [r4, #8]
	ldr	r3, [r4, #44]
	movs	r5, #1
	cmp	r2, #0
	ble.n	.L1
	movs	r0, #128
	mov	r9, r0
	lsls	r0, r6, #16
	asrs	r7, r0, #24
	movs	r0, #12
	mov	r8, r0
.L3:
	mov	r0, ip
	ands	r0, r5
	cmp	r0, #0
	beq.n	.L2
	ldrb	r1, [r3, #0]
	mov	r0, r9
	ands	r0, r1
	cmp	r0, #0
	beq.n	.L2
	strb	r7, [r3, #11]
	strb	r6, [r3, #13]
	mov	r0, r8
	orrs	r0, r1
	strb	r0, [r3, #0]
.L2:
	subs	r2, #1
	adds	r3, #80
	lsls	r5, r5, #1
	cmp	r2, #0
	bgt.n	.L3
.L1:
	ldr	r0, [pc, #16]
	str	r0, [r4, #52]
.L0:
	pop	{r3, r4, r5}
	mov	r8, r3
	mov	r9, r4
	mov	sl, r5
	pop	{r4, r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x68736d53
