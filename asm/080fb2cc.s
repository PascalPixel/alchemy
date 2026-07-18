@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080fb2cc
	.thumb_func
Func_080fb2cc:
	push	{r4, r5, r6, r7, lr}
	mov	r7, r9
	mov	r6, r8
	push	{r6, r7}
	adds	r4, r0, #0
	lsls	r1, r1, #16
	lsrs	r7, r1, #16
	lsls	r6, r2, #16
	ldr	r3, [r4, #52]
	ldr	r0, [pc, #80]
	cmp	r3, r0
	bne.n	.L0
	adds	r0, r3, #1
	str	r0, [r4, #52]
	ldrb	r2, [r4, #8]
	ldr	r1, [r4, #44]
	movs	r5, #1
	cmp	r2, #0
	ble.n	.L1
	movs	r0, #128
	mov	r8, r0
	lsrs	r6, r6, #18
	movs	r0, #3
	mov	ip, r0
.L3:
	adds	r0, r7, #0
	ands	r0, r5
	cmp	r0, #0
	beq.n	.L2
	ldrb	r3, [r1, #0]
	mov	r0, r8
	ands	r0, r3
	cmp	r0, #0
	beq.n	.L2
	strb	r6, [r1, #19]
	mov	r0, ip
	orrs	r0, r3
	strb	r0, [r1, #0]
.L2:
	subs	r2, #1
	adds	r1, #80
	lsls	r5, r5, #1
	cmp	r2, #0
	bgt.n	.L3
.L1:
	ldr	r0, [pc, #12]
	str	r0, [r4, #52]
.L0:
	pop	{r3, r4}
	mov	r8, r3
	mov	r9, r4
	pop	{r4, r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x68736d53
