@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080c1724
	.thumb_func
Func_080c1724:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r7, r0, #0
	adds	r0, r2, #0
	movs	r2, #128
	lsls	r2, r2, #9
	adds	r6, r1, #0
	cmp	r0, r2
	ble.n	.L0
	adds	r0, r2, #0
.L0:
	cmp	r3, #0
	ble.n	.L1
	movs	r1, #31
	mov	r8, r1
	movs	r2, #248
	movs	r1, #248
	lsls	r2, r2, #2
	lsls	r1, r1, #7
	mov	lr, r2
	mov	ip, r1
	adds	r5, r3, #0
.L2:
	ldrh	r4, [r7, #0]
	mov	r2, r8
	adds	r3, r4, #0
	ands	r3, r2
	mov	r1, lr
	adds	r2, r4, #0
	ands	r2, r1
	muls	r3, r0
	mov	r1, ip
	muls	r2, r0
	ands	r1, r4
	muls	r1, r0
	lsrs	r4, r3, #16
	mov	r3, r8
	ands	r4, r3
	lsrs	r2, r2, #16
	mov	r3, lr
	ands	r2, r3
	orrs	r4, r2
	lsrs	r1, r1, #16
	mov	r2, ip
	ands	r1, r2
	orrs	r4, r1
	subs	r5, #1
	strh	r4, [r6, #0]
	adds	r7, #2
	adds	r6, #2
	cmp	r5, #0
	bne.n	.L2
.L1:
	movs	r0, #0
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
