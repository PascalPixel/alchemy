@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080b75dc
	.thumb_func
Func_080b75dc:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #80
	ldr	r3, [pc, #272]
	mov	r2, sp
	ldr	r3, [r3, #0]
	adds	r2, #52
	movs	r1, #0
	adds	r0, r2, #0
	mov	sl, r3
	str	r2, [sp, #0]
	mov	r9, r1
	bl	Func_080b6a60
	ldr	r3, [pc, #256]
	adds	r5, r0, #0
	movs	r2, #255
	movs	r7, #13
	add	r3, sl
.L0:
	subs	r7, #1
	strb	r2, [r3, #0]
	subs	r3, #1
	cmp	r7, #0
	bge.n	.L0
	ldr	r3, [pc, #236]
	movs	r1, #5
	add	r3, sl
	movs	r2, #13
.L1:
	subs	r1, #1
	strb	r2, [r3, #0]
	subs	r3, #1
	subs	r2, #1
	cmp	r1, #0
	bge.n	.L1
	cmp	r5, #0
	ble.n	.L2
	ldr	r3, [pc, #216]
	ldr	r1, [sp, #0]
	mov	r2, r9
	mov	fp, r3
	mov	r8, r1
	lsls	r6, r2, #1
	adds	r7, r5, #0
.L3:
	mov	r3, r8
	ldrh	r5, [r3, #0]
	movs	r2, #183
	movs	r1, #2
	lsls	r2, r2, #2
	adds	r3, r5, r2
	add	r8, r1
	mov	r2, r9
	mov	r1, sl
	strb	r2, [r1, r3]
	adds	r0, r5, #0
	bl	Func_080b7dd0
	mov	r3, fp
	ldrsb	r2, [r6, r3]
	mov	r1, fp
	adds	r3, r6, #1
	ldrsb	r3, [r3, r1]
	adds	r1, r5, #0
	bl	Func_080b6f44
	subs	r7, #1
	movs	r2, #1
	adds	r6, #2
	add	r9, r2
	cmp	r7, #0
	bne.n	.L3
.L2:
	movs	r5, #2
	add	r5, sl
	movs	r3, #100
	ldrsh	r3, [r5, r3]
	movs	r7, #0
	mov	fp, r5
	cmp	r3, #255
	beq.n	.L4
	ldr	r4, [sp, #0]
	movs	r0, #0
	movs	r2, #100
	mov	r1, fp
.L5:
	ldrh	r3, [r1, r2]
	adds	r7, #1
	strh	r3, [r0, r4]
	adds	r2, #2
	adds	r0, #2
	cmp	r7, #5
	bgt.n	.L4
	ldrsh	r3, [r1, r2]
	cmp	r3, #255
	bne.n	.L5
.L4:
	movs	r1, #28
	movs	r2, #4
	add	r1, sp
	add	r2, sp
	adds	r5, r7, #0
	mov	r9, r1
	mov	sl, r2
	ldr	r0, [sp, #0]
	adds	r1, r5, #0
	mov	r2, r9
	mov	r3, sl
	bl	Func_080b7424
	cmp	r5, #0
	ble.n	.L6
	movs	r3, #0
	lsls	r6, r3, #2
	movs	r3, #100
	mov	r8, r3
.L8:
	mov	r1, fp
	mov	r3, r8
	ldrsh	r5, [r1, r3]
	cmp	r5, #254
	beq.n	.L7
	adds	r0, r5, #0
	bl	Func_080b7dd0
	mov	r1, r9
	ldr	r2, [r6, r1]
	mov	r1, sl
	ldr	r3, [r6, r1]
	adds	r1, r5, #0
	bl	Func_080b6f44
.L7:
	movs	r2, #2
	subs	r7, #1
	add	r8, r2
	adds	r6, #4
	cmp	r7, #0
	bne.n	.L8
.L6:
	add	sp, #80
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001e74
	.4byte 0x000002e9
	.4byte 0x080c2a62
