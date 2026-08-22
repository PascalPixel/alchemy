@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080a68ec
	.thumb_func
Func_080a68ec:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	mov	sl, r1
	adds	r1, r2, #0
	movs	r2, #2
	eors	r2, r1
	negs	r3, r2
	orrs	r3, r2
	lsrs	r3, r3, #31
	mov	r8, r3
	movs	r2, #3
	add	r8, r2
	mov	r3, sl
	ldr	r2, [pc, #32]
	sub	sp, #8
	mov	r9, r0
	adds	r3, #62
	mov	ip, sl
.L0:
	strh	r2, [r3, #0]
	subs	r3, #2
	cmp	r3, ip
	bge.n	.L0
	movs	r4, #0
	cmp	r1, #1
	bne.n	.L1
	ldr	r7, [pc, #12]
	movs	r1, #0
	movs	r6, #88
	mov	r5, sl
	b.n	.L2
	movs	r0, r0
	.4byte 0x00000000
	.4byte 0x00003fff
.L2:
	mov	r3, r9
	ldrh	r2, [r6, r3]
	adds	r3, r2, #0
	cmp	r3, #0
	beq.n	.L3
	adds	r0, r7, #0
	ands	r0, r2
	str	r1, [sp, #4]
	str	r4, [sp, #0]
	bl	Func_08077080
	ldrb	r3, [r0, #12]
	ldr	r1, [sp, #4]
	ldr	r4, [sp, #0]
	cmp	r3, #0
	beq.n	.L3
	mov	r2, r9
	ldrh	r3, [r2, r6]
	adds	r4, #1
	strh	r3, [r5, #0]
	adds	r5, #2
.L3:
	adds	r1, #1
	adds	r6, #4
	cmp	r1, #31
	ble.n	.L2
	b.n	.L4
.L1:
	movs	r7, #0
	cmp	r7, r8
	bge.n	.L4
	movs	r3, #64
	mov	fp, r3
.L9:
	lsls	r3, r4, #1
	mov	r2, sl
	mov	r6, r9
	adds	r5, r3, r2
	movs	r1, #31
	adds	r6, #88
.L8:
	ldrh	r2, [r6, #0]
	adds	r3, r2, #0
	cmp	r3, #0
	beq.n	.L5
	ldr	r0, [pc, #36]
	ands	r0, r2
	str	r1, [sp, #4]
	str	r4, [sp, #0]
	bl	Func_08077080
	ldr	r1, [sp, #4]
	ldr	r4, [sp, #0]
	cmp	r7, #0
	bne.n	.L6
	ldrb	r3, [r0, #12]
	cmp	r3, #0
	bne.n	.L7
	ldrb	r2, [r0, #1]
	mov	r3, fp
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L7
	b.n	.L6
	.4byte 0x00003fff
.L6:
	cmp	r7, #1
	beq.n	.L5
	cmp	r7, #2
	beq.n	.L5
	cmp	r7, #3
	bne.n	.L5
	ldrb	r3, [r0, #12]
	cmp	r3, #0
	bne.n	.L5
	ldrb	r2, [r0, #1]
	mov	r3, fp
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L5
.L7:
	ldrh	r3, [r6, #0]
	adds	r4, #1
	strh	r3, [r5, #0]
	adds	r5, #2
.L5:
	subs	r1, #1
	adds	r6, #4
	cmp	r1, #0
	bge.n	.L8
	adds	r7, #1
	cmp	r7, r8
	blt.n	.L9
.L4:
	adds	r0, r4, #0
	add	sp, #8
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
