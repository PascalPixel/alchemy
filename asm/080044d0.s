@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080044d0
	.thumb_func
Func_080044d0:
	push	{r5, r6, lr}
	adds	r6, r0, #0
	adds	r5, r1, #0
	movs	r4, #0
	cmp	r6, #0
	beq.n	.L0
	movs	r4, #128
	lsls	r4, r4, #7
	cmp	r5, #0
	beq.n	.L0
	cmp	r5, #0
	bge.n	.L1
	negs	r1, r5
.L1:
	adds	r0, r6, #0
	cmp	r6, #0
	bge.n	.L2
	negs	r0, r6
.L2:
	lsls	r0, r0, #8
	bl	Func_080022ec
	ldr	r3, [pc, #160]
	movs	r4, #128
	adds	r1, r0, #0
	lsls	r4, r4, #7
	cmp	r1, r3
	bgt.n	.L0
	ldr	r2, [pc, #152]
	ldrh	r0, [r2, #0]
	movs	r4, #0
	subs	r2, #128
	cmp	r1, r0
	ble.n	.L3
	movs	r3, #128
	movs	r4, #128
	lsls	r3, r3, #1
	lsls	r4, r4, #6
	adds	r2, r2, r3
.L3:
	ldrh	r0, [r2, #0]
	subs	r2, #64
	cmp	r1, r0
	ble.n	.L4
	movs	r3, #128
	lsls	r3, r3, #5
	orrs	r4, r3
	adds	r2, #128
.L4:
	ldrh	r0, [r2, #0]
	subs	r2, #32
	cmp	r1, r0
	ble.n	.L5
	movs	r3, #128
	lsls	r3, r3, #4
	orrs	r4, r3
	adds	r2, #64
.L5:
	ldrh	r0, [r2, #0]
	subs	r2, #16
	cmp	r1, r0
	ble.n	.L6
	movs	r3, #128
	lsls	r3, r3, #3
	orrs	r4, r3
	adds	r2, #32
.L6:
	ldrh	r0, [r2, #0]
	subs	r2, #8
	cmp	r1, r0
	ble.n	.L7
	movs	r3, #128
	lsls	r3, r3, #2
	orrs	r4, r3
	adds	r2, #16
.L7:
	ldrh	r0, [r2, #0]
	subs	r2, #4
	cmp	r1, r0
	ble.n	.L8
	movs	r3, #128
	lsls	r3, r3, #1
	orrs	r4, r3
	adds	r2, #8
.L8:
	ldrh	r0, [r2, #0]
	subs	r2, #2
	cmp	r1, r0
	ble.n	.L9
	movs	r3, #128
	orrs	r4, r3
	adds	r2, #4
.L9:
	ldrh	r0, [r2, #0]
	cmp	r1, r0
	ble.n	.L0
	movs	r3, #64
	orrs	r4, r3
.L0:
	cmp	r5, #0
	bge.n	.L10
	movs	r3, #128
	lsls	r3, r3, #8
	subs	r4, r3, r4
.L10:
	cmp	r6, #0
	bge.n	.L11
	negs	r4, r4
.L11:
	lsls	r0, r4, #16
	lsrs	r0, r0, #16
	pop	{r5, r6}
	pop	{r1}
	bx	r1
	.4byte 0x0000fb6a
	.4byte 0x08007676
