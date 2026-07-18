@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080b9470
	.thumb_func
Func_080b9470:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	adds	r7, r1, #0
	sub	sp, #16
	mov	fp, r0
	cmp	r7, #0
	ble.n	.L0
	mov	r5, fp
	adds	r6, r7, #0
.L3:
	movs	r1, #6
	ldrsh	r3, [r5, r1]
	cmp	r3, #5
	bne.n	.L1
	movs	r2, #0
	ldrsh	r0, [r5, r2]
	bl	Func_08077008
	ldrh	r2, [r5, #8]
	ldr	r3, [pc, #52]
	lsls	r0, r2, #16
	asrs	r0, r0, #24
	movs	r1, #255
	ands	r1, r2
	ands	r0, r3
	bl	Func_080771e8
	bl	Func_08077080
	ldrb	r2, [r0, #3]
	adds	r3, r2, #0
	adds	r3, #210
	movs	r1, #128
	lsls	r3, r3, #24
	lsls	r1, r1, #17
	cmp	r3, r1
	bls.n	.L2
	adds	r3, r2, #0
	cmp	r3, #53
	bne.n	.L1
.L2:
	ldrh	r3, [r5, #4]
	ldr	r2, [pc, #12]
	adds	r3, r3, r2
	strh	r3, [r5, #4]
	b.n	.L1
	movs	r0, r0
	.4byte 0x0000000f
	.4byte 0x00002710
.L1:
	subs	r6, #1
	adds	r5, #16
	cmp	r6, #0
	bne.n	.L3
.L0:
	subs	r7, #1
	mov	r9, r7
.L7:
	movs	r3, #0
	mov	r7, r9
	mov	sl, r3
	cmp	r7, #0
	ble.n	.L4
	lsls	r3, r7, #4
	add	r3, fp
	ldr	r1, [pc, #88]
	adds	r5, r3, #0
	mov	r8, r1
	subs	r5, #16
	adds	r6, r3, #0
.L6:
	movs	r3, #20
	ldrsh	r2, [r5, r3]
	movs	r1, #4
	ldrsh	r3, [r5, r1]
	cmp	r2, r3
	ble.n	.L5
	mov	r0, sp
	adds	r1, r6, #0
	movs	r2, #16
	bl	Func_08007304
	adds	r1, r5, #0
	movs	r2, #16
	adds	r0, r6, #0
	bl	Func_08007304
	movs	r2, #16
	adds	r0, r5, #0
	mov	r1, sp
	bl	Func_08007304
	movs	r2, #1
	add	sl, r2
.L5:
	subs	r7, #1
	subs	r5, #16
	subs	r6, #16
	cmp	r7, #0
	bgt.n	.L6
.L4:
	mov	r3, sl
	cmp	r3, #0
	bne.n	.L7
	add	sp, #16
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001388
