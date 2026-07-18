@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080b7424
	.thumb_func
Func_080b7424:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #4
	mov	fp, r2
	mov	r9, r1
	str	r3, [sp, #0]
	movs	r2, #30
	cmp	r1, #4
	ble.n	.L0
	movs	r2, #27
.L0:
	mov	r3, r9
	subs	r3, #1
	muls	r3, r2
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	movs	r1, #0
	mov	r2, r9
	asrs	r7, r3, #1
	mov	sl, r1
	cmp	r2, #0
	beq.n	.L1
	adds	r6, r0, #0
	mov	r8, r1
.L6:
	movs	r3, #80
	negs	r3, r3
	mov	r0, r8
	mov	r1, fp
	str	r3, [r0, r1]
	mov	r3, sl
	movs	r2, #0
	cmp	r3, #0
	beq.n	.L2
	ldrh	r3, [r6, #0]
	ldr	r0, [pc, #156]
	movs	r1, #128
	adds	r3, r3, r0
	lsls	r3, r3, #16
	lsls	r1, r1, #9
	movs	r2, #25
	cmp	r3, r1
	bls.n	.L2
	ldrh	r0, [r6, #0]
	bl	Func_08077008
	movs	r2, #148
	lsls	r2, r2, #1
	adds	r5, r0, #0
	adds	r3, r5, r2
	ldrb	r0, [r3, #0]
	bl	Func_080c23c0
	movs	r2, #27
	cmp	r0, #0
	bne.n	.L3
	movs	r2, #38
.L3:
	movs	r0, #148
	lsls	r0, r0, #1
	adds	r3, r5, r0
	ldrb	r0, [r3, #0]
	cmp	r0, #148
	beq.n	.L4
	cmp	r0, #121
	bne.n	.L2
.L4:
	movs	r3, #50
	negs	r3, r3
	mov	r1, r8
	mov	r0, fp
	str	r3, [r1, r0]
.L2:
	lsrs	r3, r2, #1
	subs	r7, r7, r3
	ldr	r2, [sp, #0]
	ldrh	r3, [r6, #0]
	ldr	r0, [pc, #80]
	mov	r1, r8
	str	r7, [r1, r2]
	adds	r3, r3, r0
	movs	r1, #128
	lsls	r3, r3, #16
	lsls	r1, r1, #9
	movs	r2, #25
	cmp	r3, r1
	bls.n	.L5
	ldrh	r0, [r6, #0]
	bl	Func_08077008
	movs	r2, #148
	lsls	r2, r2, #1
	adds	r0, r0, r2
	ldrb	r0, [r0, #0]
	bl	Func_080c23c0
	movs	r2, #27
	cmp	r0, #0
	bne.n	.L5
	movs	r2, #38
.L5:
	lsrs	r3, r2, #1
	movs	r0, #1
	subs	r7, r7, r3
	add	sl, r0
	movs	r3, #4
	adds	r6, #2
	add	r8, r3
	cmp	sl, r9
	bne.n	.L6
.L1:
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x0000ff02
