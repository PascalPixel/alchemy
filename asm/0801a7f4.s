@ 呼出しグラフで未到達だったコード間隙関数の再構築サム逆アセンブル。
@ 探索では未到達。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0801a7f4
	.thumb_func
Func_0801a7f4:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #76]
	ldr	r3, [r3, #0]
	mov	r9, r3
	movs	r3, #229
	lsls	r3, r3, #2
	add	r3, r9
	ldrh	r3, [r3, #0]
	sub	sp, #8
	str	r3, [sp, #4]
	movs	r3, #231
	lsls	r3, r3, #2
	add	r3, r9
	ldrh	r3, [r3, #0]
	movs	r2, #213
	mov	sl, r3
	lsls	r3, r3, #1
	movs	r6, #0
	add	r3, r9
	lsls	r2, r2, #2
	mov	fp, r6
	adds	r4, r3, r2
	b.n	.L0
.L4:
	adds	r2, r5, #0
	movs	r3, #0
	adds	r0, r7, #0
	mov	r1, r8
	str	r4, [sp, #0]
	bl	Func_0801bd98
	movs	r3, #210
	lsls	r3, r3, #2
	add	r3, r9
	ldr	r2, [r3, #0]
	ldr	r4, [sp, #0]
	cmp	r2, #0
	bne.n	.L1
	str	r5, [r3, #0]
	str	r2, [r5, #0]
	b.n	.L2
	.4byte 0x03001e98
.L1:
	str	r5, [r6, #4]
	str	r6, [r5, #0]
.L2:
	movs	r3, #0
	str	r3, [r5, #4]
	movs	r3, #1
	add	fp, r3
	mov	r2, fp
	adds	r6, r5, #0
	cmp	r2, #5
	beq.n	.L3
	adds	r4, #2
	add	sl, r3
.L0:
	ldr	r3, [sp, #4]
	cmp	sl, r3
	bcs.n	.L3
	ldrh	r2, [r4, #32]
	movs	r0, #0
	ldrh	r7, [r4, #0]
	str	r4, [sp, #0]
	mov	r8, r2
	bl	Func_0801a910
	adds	r5, r0, #0
	ldr	r4, [sp, #0]
	cmp	r5, #0
	bne.n	.L4
.L3:
	mov	r3, fp
	lsls	r2, r3, #3
	ldr	r3, [pc, #48]
	ldr	r1, [pc, #52]
	subs	r3, r3, r2
	movs	r2, #230
	add	r1, r9
	lsls	r2, r2, #2
	strh	r3, [r1, #0]
	add	r2, r9
	movs	r3, #140
	strh	r3, [r2, #0]
	movs	r3, #210
	lsls	r3, r3, #2
	add	r3, r9
	ldr	r6, [r3, #0]
	movs	r3, #0
	mov	fp, r3
	cmp	r6, #0
	beq.n	.L5
	movs	r0, #238
	lsls	r0, r0, #2
	adds	r5, r1, #0
	adds	r4, r2, #0
	add	r0, r9
	movs	r1, #0
	b.n	.L6
	movs	r0, r0
	.4byte 0x00000064
	.4byte 0x00000396
.L6:
	ldrh	r3, [r5, #0]
	add	r3, fp
	strh	r3, [r6, #16]
	ldrh	r2, [r4, #0]
	strh	r2, [r6, #18]
	strh	r2, [r6, #26]
	ldrh	r2, [r6, #10]
	strh	r3, [r6, #24]
	cmp	r2, #6
	bne.n	.L7
	ldrh	r3, [r0, #0]
	cmp	r3, #0
	bne.n	.L7
	strh	r2, [r6, #18]
	strh	r2, [r6, #26]
.L7:
	strh	r1, [r6, #20]
	strh	r1, [r6, #22]
	ldr	r6, [r6, #4]
	movs	r2, #16
	add	fp, r2
	cmp	r6, #0
	bne.n	.L6
.L5:
	bl	Func_0801c188
	add	sp, #8
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	bx	lr
