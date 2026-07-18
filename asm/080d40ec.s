@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080d40ec
	.thumb_func
Func_080d40ec:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	adds	r5, r0, #0
	lsls	r5, r5, #10
	adds	r1, r5, r1
	adds	r0, r1, #0
	sub	sp, #128
	adds	r6, r2, #0
	mov	r8, r3
	bl	Func_08002322
	lsls	r0, r0, #4
	adds	r6, r5, r6
	asrs	r0, r0, #15
	mov	sl, r0
	adds	r0, r6, #0
	bl	Func_08002322
	add	r5, r8
	lsls	r0, r0, #4
	asrs	r7, r0, #15
	adds	r0, r5, #0
	bl	Func_08002322
	movs	r3, #0
	mov	r2, sp
	lsls	r0, r0, #4
	mov	r6, sp
	strh	r3, [r2, #0]
	asrs	r0, r0, #15
	movs	r5, #1
	adds	r6, #2
.L6:
	mov	r2, sl
	adds	r3, r5, r2
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r4, r3, #1
	adds	r3, r5, r7
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r1, r3, #1
	adds	r3, r5, r0
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	cmp	r4, #0
	bge.n	.L0
	movs	r4, #0
.L0:
	cmp	r4, #31
	ble.n	.L1
	movs	r4, #31
.L1:
	cmp	r1, #0
	bge.n	.L2
	movs	r1, #0
.L2:
	cmp	r1, #31
	ble.n	.L3
	movs	r1, #31
.L3:
	cmp	r3, #0
	bge.n	.L4
	movs	r3, #0
.L4:
	cmp	r3, #31
	ble.n	.L5
	movs	r3, #31
.L5:
	lsls	r3, r3, #10
	lsls	r2, r1, #5
	orrs	r3, r2
	orrs	r3, r4
	adds	r5, #1
	strh	r3, [r6, #0]
	adds	r6, #2
	cmp	r5, #64
	bne.n	.L6
	mov	r1, sp
	ldr	r3, [pc, #24]
	movs	r2, #128
	ldr	r0, [pc, #24]
	bl	Func_080072f0
	add	sp, #128
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001388
	.4byte 0x05000002
