@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080b98b4
	.thumb_func
Func_080b98b4:
	push	{r5, r6, r7, lr}
	movs	r1, #0
	adds	r5, r0, #0
	movs	r3, #15
	mov	lr, r1
	movs	r7, #31
.L7:
	lsls	r3, r3, #4
	movs	r6, #0
	mov	ip, r3
.L6:
	mov	r2, ip
	adds	r3, r2, r6
	movs	r1, #160
	lsls	r1, r1, #19
	lsls	r0, r3, #1
	adds	r3, r0, r1
	ldrh	r3, [r3, #0]
	lsrs	r4, r3, #10
	ands	r4, r7
	lsrs	r2, r3, #5
	adds	r1, r7, #0
	ands	r2, r7
	ands	r1, r3
	adds	r4, r4, r5
	adds	r2, r2, r5
	adds	r1, r1, r5
	cmp	r4, #31
	ble.n	.L0
	movs	r4, #31
.L0:
	cmp	r2, #31
	ble.n	.L1
	movs	r2, #31
.L1:
	cmp	r1, #31
	ble.n	.L2
	movs	r1, #31
.L2:
	cmp	r4, #0
	bge.n	.L3
	movs	r4, #0
.L3:
	cmp	r2, #0
	bge.n	.L4
	movs	r2, #0
.L4:
	cmp	r1, #0
	bge.n	.L5
	movs	r1, #0
.L5:
	lsls	r2, r2, #5
	lsls	r3, r4, #10
	orrs	r3, r2
	orrs	r3, r1
	ldr	r1, [pc, #28]
	adds	r6, #1
	adds	r2, r0, r1
	strh	r3, [r2, #0]
	cmp	r6, #15
	ble.n	.L6
	movs	r2, #1
	add	lr, r2
	mov	r1, lr
	movs	r3, #5
	cmp	r1, #1
	ble.n	.L7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x04ffffe0
