@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080f61e8
	.thumb_func
Func_080f61e8:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r1, #160
	lsls	r1, r1, #19
	sub	sp, #128
	mov	r8, r1
	bl	Func_08002f40
	ldr	r3, [pc, #28]
	mov	r1, sp
	ldr	r2, [pc, #28]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r2, #0
	movs	r3, #31
	ldr	r7, [pc, #12]
	mov	lr, r2
	mov	sl, r3
	mov	ip, sp
	movs	r6, #0
	b.n	.L0
	movs	r0, r0
	.4byte 0x0000001f
	.4byte 0x040000d4
	.4byte 0x84000020
.L0:
	mov	r1, r8
	ldrh	r3, [r1, #0]
	mov	r5, sl
	mov	r1, ip
	ands	r5, r3
	lsls	r3, r3, #16
	lsrs	r2, r3, #21
	lsrs	r0, r3, #26
	ldrh	r3, [r6, r1]
	mov	r1, sl
	ands	r1, r3
	lsls	r3, r3, #16
	lsrs	r4, r3, #21
	lsrs	r3, r3, #26
	ands	r2, r7
	ands	r0, r7
	ands	r4, r7
	ands	r3, r7
	cmp	r5, r1
	bge.n	.L1
	adds	r5, #1
	b.n	.L2
.L1:
	cmp	r5, r1
	ble.n	.L2
	subs	r5, #1
.L2:
	cmp	r2, r4
	bge.n	.L3
	adds	r2, #1
	b.n	.L4
.L3:
	cmp	r2, r4
	ble.n	.L4
	subs	r2, #1
.L4:
	cmp	r0, r3
	bge.n	.L5
	adds	r0, #1
	b.n	.L6
.L5:
	cmp	r0, r3
	ble.n	.L6
	subs	r0, #1
.L6:
	lsls	r2, r2, #5
	lsls	r3, r0, #10
	orrs	r3, r2
	orrs	r3, r5
	mov	r2, ip
	strh	r3, [r6, r2]
	movs	r3, #1
	add	lr, r3
	movs	r1, #2
	mov	r2, lr
	adds	r6, #2
	add	r8, r1
	cmp	r2, #64
	bne.n	.L0
	mov	r0, sp
	ldr	r3, [pc, #24]
	adds	r0, #2
	ldr	r1, [pc, #24]
	ldr	r2, [pc, #28]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	add	sp, #128
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x040000d4
	.4byte 0x05000002
	.4byte 0x8000003f
