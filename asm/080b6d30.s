@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080b6d30
	.thumb_func
Func_080b6d30:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #224]
	sub	sp, #4
	ldr	r6, [r3, #0]
	mov	fp, r0
	bl	Func_08077008
	movs	r5, #148
	mov	r9, r0
	lsls	r5, r5, #1
	add	r5, r9
	ldrb	r0, [r5, #0]
	bl	Func_080c23c0
	movs	r1, #0
	mov	r8, r0
	ldrb	r0, [r5, #0]
	mov	sl, r1
	bl	Func_080c2384
	mov	r4, sl
	adds	r7, r0, #0
.L8:
	ldr	r3, [pc, #184]
	add	r3, r9
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L0
	movs	r2, #4
	ldrsh	r3, [r6, r2]
	movs	r5, #0
	cmp	r3, #0
	bne.n	.L1
	mov	r3, r8
	cmp	r3, #0
	bne.n	.L2
	movs	r1, #6
	ldrsh	r3, [r6, r1]
	cmp	r3, #0
	beq.n	.L2
.L1:
	adds	r5, #1
	cmp	r5, #5
	bgt.n	.L2
	lsls	r2, r5, #1
	adds	r3, r2, #4
	ldrsh	r3, [r6, r3]
	cmp	r3, #0
	bne.n	.L1
	mov	r3, r8
	cmp	r3, #0
	bne.n	.L2
	cmp	r5, #4
	bgt.n	.L1
	adds	r3, r2, #6
	ldrsh	r3, [r6, r3]
	cmp	r3, #0
	bne.n	.L1
.L2:
	cmp	r5, #6
	beq.n	.L3
	movs	r3, #148
	lsls	r3, r3, #1
	add	r3, r9
	ldrb	r0, [r3, #0]
	str	r4, [sp, #0]
	bl	Func_080c23a0
	ldr	r2, [pc, #104]
	ldr	r4, [sp, #0]
	lsls	r1, r5, #14
	adds	r3, r0, #0
	adds	r1, r1, r2
	adds	r0, r5, #0
	adds	r2, r7, r4
	bl	Func_08009270
	ldr	r4, [sp, #0]
	cmp	r0, #0
	bne.n	.L4
	movs	r0, #0
	b.n	.L5
.L4:
	cmp	r4, #0
	bne.n	.L6
	lsls	r3, r5, #12
	orrs	r3, r7
	mov	sl, r3
.L6:
	lsls	r0, r5, #1
	adds	r3, r0, #4
	mov	r1, fp
	mov	r2, r8
	strh	r1, [r6, r3]
	cmp	r2, #0
	bne.n	.L7
	adds	r3, r0, #6
	strh	r1, [r6, r3]
.L7:
	movs	r2, #238
	lsls	r2, r2, #1
	cmp	r7, r2
	beq.n	.L0
	ldr	r3, [pc, #44]
	cmp	r7, r3
	bne.n	.L3
.L0:
	adds	r4, #1
	cmp	r4, #1
	ble.n	.L8
.L3:
	mov	r0, sl
.L5:
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x03001e74
	.4byte 0x00000129
	.4byte 0x02018000
	.4byte 0x000001e3
