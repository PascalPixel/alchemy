@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080799b0
	.thumb_func
Func_080799b0:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r2, #1
	adds	r5, r0, #0
	negs	r2, r2
	sub	sp, #20
	mov	r8, r2
	movs	r0, #0
	cmp	r5, #7
	bgt.n	.L0
	add	r6, sp, #4
	adds	r0, r5, #0
	adds	r2, r6, #0
	bl	Func_080797fc
	movs	r0, #32
	bl	Func_08079338
	cmp	r0, #0
	beq.n	.L1
	movs	r0, #200
	cmp	r5, #0
	beq.n	.L0
	movs	r0, #201
	cmp	r5, #1
	beq.n	.L0
.L1:
	movs	r0, #202
	cmp	r5, #5
	beq.n	.L0
	movs	r0, #1
	negs	r0, r0
	cmp	r8, r0
	bne.n	.L0
	mov	ip, r8
	mov	r5, r8
	movs	r0, #0
	adds	r2, r6, #0
.L3:
	ldmia	r2!, {r3}
	cmp	ip, r3
	bge.n	.L2
	mov	ip, r3
	adds	r5, r0, #0
.L2:
	adds	r0, #1
	cmp	r0, #3
	ble.n	.L3
	movs	r4, #1
	negs	r4, r4
	mov	ip, r4
	movs	r0, #0
	adds	r2, r6, #0
.L5:
	cmp	r0, r5
	beq.n	.L4
	ldr	r3, [r2, #0]
	cmp	ip, r3
	bge.n	.L4
	mov	ip, r3
	adds	r4, r0, #0
.L4:
	adds	r0, #1
	adds	r2, #4
	cmp	r0, #3
	ble.n	.L5
	lsls	r3, r4, #2
	ldr	r3, [r6, r3]
	adds	r1, r5, #0
	cmp	r3, #9
	ble.n	.L6
	adds	r1, r4, #0
	b.n	.L6
.L11:
	mov	r8, r0
	b.n	.L7
.L6:
	adds	r0, r5, #0
	bl	Func_080797ec
	ldr	r3, [pc, #124]
	ldr	r7, [pc, #128]
	mov	sl, r3
	mov	lr, sl
	str	r6, [sp, #0]
	ldr	r5, [pc, #124]
	mov	fp, r0
	mov	r9, r6
	movs	r0, #202
	add	r7, lr
.L12:
	mov	r6, sl
	ldr	r3, [r5, r6]
	cmp	r3, fp
	bne.n	.L8
	ldrb	r3, [r7, #0]
	ldr	r1, [sp, #0]
	lsls	r2, r3, #2
	adds	r2, r2, r3
	ldr	r3, [r1, #0]
	lsls	r2, r2, #1
	movs	r4, #0
	cmp	r3, r2
	blt.n	.L9
	mov	r2, lr
	adds	r3, r5, r2
	mov	ip, r9
	adds	r1, r3, #4
.L10:
	adds	r4, #1
	cmp	r4, #3
	bgt.n	.L9
	adds	r1, #1
	ldrb	r3, [r1, #0]
	lsls	r2, r3, #2
	adds	r2, r2, r3
	movs	r3, #4
	add	ip, r3
	mov	r6, ip
	ldr	r3, [r6, #0]
	lsls	r2, r2, #1
	cmp	r3, r2
	bge.n	.L10
.L9:
	cmp	r4, #4
	beq.n	.L11
.L8:
	subs	r0, #1
	subs	r7, #84
	subs	r5, #84
	cmp	r0, #0
	bge.n	.L12
.L7:
	movs	r1, #1
	negs	r1, r1
	cmp	r8, r1
	bne.n	.L13
	movs	r2, #0
	mov	r8, r2
.L13:
	mov	r0, r8
.L0:
	add	sp, #20
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x08084b1c
	.4byte 0x0000424c
	.4byte 0x00004248
