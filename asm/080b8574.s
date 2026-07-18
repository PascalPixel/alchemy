@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080b8574
	.thumb_func
Func_080b8574:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #60
	add	r2, sp, #16
	movs	r1, #0
	mov	sl, r2
	str	r0, [sp, #12]
	mov	r9, r1
	movs	r0, #1
	mov	r1, sl
	bl	Func_080b6b40
	movs	r7, #0
	mov	r8, r0
.L0:
	adds	r0, r7, #0
	adds	r7, #1
	bl	Func_08077008
	cmp	r7, #4
	bne.n	.L0
	movs	r7, #0
	cmp	r7, r8
	bge.n	.L1
	ldr	r6, [sp, #12]
	movs	r1, #0
	mov	r2, sl
.L2:
	ldrh	r5, [r2, #0]
	adds	r2, #2
	adds	r0, r5, #0
	str	r1, [sp, #4]
	str	r2, [sp, #0]
	bl	Func_08077008
	adds	r3, r0, #0
	adds	r3, #64
	ldrh	r3, [r3, #0]
	ldr	r1, [sp, #4]
	strh	r3, [r6, #4]
	movs	r3, #128
	strh	r3, [r6, #10]
	adds	r7, #1
	movs	r3, #1
	strh	r5, [r6, #0]
	strh	r1, [r6, #6]
	strh	r1, [r6, #8]
	add	r9, r3
	adds	r6, #16
	ldr	r2, [sp, #0]
	cmp	r7, r8
	blt.n	.L2
.L1:
	mov	r1, sl
	movs	r0, #2
	bl	Func_080b6b40
	mov	r1, r8
	ldr	r2, [sp, #12]
	lsls	r3, r1, #4
	adds	r5, r0, #0
	movs	r1, #0
	movs	r0, #1
	adds	r6, r2, r3
	bl	Func_080b6b40
	str	r0, [sp, #8]
	cmp	r5, #0
	ble.n	.L3
	movs	r3, #0
	mov	fp, r3
	mov	r8, sl
	adds	r7, r5, #0
.L5:
	mov	r1, r8
	ldrh	r5, [r1, #0]
	movs	r2, #2
	adds	r0, r5, #0
	add	r8, r2
	bl	Func_08077008
	strh	r5, [r6, #0]
	adds	r5, r0, #0
	adds	r5, #64
	ldrh	r3, [r5, #0]
	lsrs	r3, r3, #1
	strh	r3, [r6, #4]
	cmp	r3, #0
	beq.n	.L4
	bl	Func_08004458
	ldrh	r3, [r5, #0]
	adds	r2, r3, #0
	muls	r2, r0
	ldrh	r3, [r6, #4]
	lsrs	r2, r2, #16
	adds	r3, r3, r2
	strh	r3, [r6, #4]
.L4:
	mov	r3, fp
	mov	r1, fp
	strh	r3, [r6, #6]
	strh	r1, [r6, #8]
	bl	Func_08004458
	ldr	r2, [sp, #8]
	adds	r3, r2, #0
	muls	r3, r0
	lsrs	r3, r3, #16
	strh	r3, [r6, #10]
	subs	r7, #1
	movs	r3, #1
	add	r9, r3
	adds	r6, #16
	cmp	r7, #0
	bne.n	.L5
.L3:
	mov	r7, r9
	subs	r7, #2
	cmp	r7, #0
	ble.n	.L6
	movs	r1, #1
	negs	r1, r1
	add	r1, r9
	mov	r8, r1
	ldr	r2, [sp, #12]
	lsls	r1, r1, #4
	mov	lr, r1
	add	r2, lr
	mov	sl, r2
.L10:
	movs	r3, #0
	mov	r6, r8
	mov	ip, r3
	cmp	r6, #0
	ble.n	.L7
	ldr	r5, [sp, #12]
	mov	r4, sl
	subs	r4, #16
	add	r5, lr
.L9:
	movs	r1, #20
	ldrsh	r2, [r4, r1]
	movs	r1, #4
	ldrsh	r3, [r4, r1]
	cmp	r2, r3
	ble.n	.L8
	ldr	r3, [pc, #76]
	adds	r0, r5, #0
	add	r1, sp, #44
	ldr	r2, [pc, #76]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	adds	r0, r4, #0
	adds	r1, r5, #0
	ldr	r2, [pc, #64]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	add	r0, sp, #44
	adds	r1, r4, #0
	ldr	r2, [pc, #56]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r2, #1
	add	ip, r2
.L8:
	subs	r6, #1
	subs	r4, #16
	subs	r5, #16
	cmp	r6, #0
	bgt.n	.L9
.L7:
	mov	r3, ip
	cmp	r3, #0
	beq.n	.L6
	subs	r7, #1
	cmp	r7, #0
	bgt.n	.L10
.L6:
	mov	r0, r9
	add	sp, #60
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x040000d4
	.4byte 0x84000004
