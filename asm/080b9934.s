@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080b9934
	.thumb_func
Func_080b9934:
	push	{r5, r6, r7, lr}
	ldr	r3, [pc, #252]
	movs	r4, #187
	ldr	r5, [r3, #0]
	movs	r1, #128
	lsls	r4, r4, #2
	adds	r7, r0, #0
	movs	r2, #0
	movs	r0, #255
	lsls	r1, r1, #8
	adds	r3, r5, r4
.L0:
	adds	r2, #1
	strh	r0, [r3, #0]
	strh	r1, [r3, #4]
	adds	r3, #16
	cmp	r2, #19
	bls.n	.L0
	bl	Func_080b90ac
	movs	r0, #8
	bl	Func_080b98b4
	ldr	r0, [pc, #212]
	bl	Func_080770c8
	adds	r5, #69
	movs	r0, #0
	bl	Func_080b8fd4
	bl	Func_08015218
	ldrb	r3, [r5, #0]
	cmp	r3, #2
	beq.n	.L1
	adds	r0, r7, #0
	bl	Func_080b920c
	adds	r6, r0, #0
	cmp	r6, #0
	blt.n	.L2
	cmp	r6, #0
	beq.n	.L3
	movs	r1, #6
	ldrsh	r3, [r7, r1]
	cmp	r3, #99
	bne.n	.L3
	bl	Func_080b90f8
	cmp	r0, #0
	bne.n	.L3
	movs	r3, #2
	strb	r3, [r5, #0]
	b.n	.L3
.L1:
	movs	r6, #0
.L3:
	ldr	r3, [pc, #144]
	ldr	r3, [r3, #0]
	adds	r3, #68
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L4
	adds	r0, r7, #0
	adds	r1, r6, #0
	bl	Func_080b9724
	adds	r5, r0, #0
	bl	Func_080b60a0
	cmp	r0, #0
	blt.n	.L5
	adds	r6, r6, r5
	cmp	r5, #0
	bge.n	.L6
.L5:
	movs	r6, #1
	negs	r6, r6
	b.n	.L2
.L4:
	lsls	r0, r6, #4
	adds	r0, r7, r0
	bl	Func_080b9324
	adds	r6, r6, r0
.L6:
	adds	r0, r7, #0
	adds	r1, r6, #0
	bl	Func_080b9470
	cmp	r6, #0
	ble.n	.L2
	adds	r5, r7, #0
	adds	r7, r6, #0
.L9:
	movs	r2, #6
	ldrsh	r3, [r5, r2]
	cmp	r3, #3
	beq.n	.L7
	cmp	r3, #7
	bne.n	.L8
.L7:
	movs	r3, #0
	ldrsh	r0, [r5, r3]
	bl	Func_08077008
	movs	r4, #6
	ldrsh	r2, [r5, r4]
	movs	r3, #3
	eors	r2, r3
	negs	r3, r2
	orrs	r3, r2
	ldr	r1, [pc, #52]
	lsrs	r3, r3, #31
	adds	r3, #1
	adds	r2, r0, r1
	strb	r3, [r2, #0]
.L8:
	subs	r7, #1
	adds	r5, #16
	cmp	r7, #0
	bne.n	.L9
.L2:
	ldr	r0, [pc, #32]
	bl	Func_080770d0
	bl	Func_080b7f9c
	ldr	r3, [pc, #28]
	ldr	r2, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #6
	str	r3, [r2, #0]
	adds	r0, r6, #0
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x03001e74
	.4byte 0x0000016b
	.4byte 0x0000012b
	.4byte 0x03001f00
