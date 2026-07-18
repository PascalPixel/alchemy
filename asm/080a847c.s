@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080a847c
	.thumb_func
Func_080a847c:
	push	{r5, r6, r7, lr}
	mov	ip, r3
	ldr	r3, [pc, #124]
	ldr	r3, [r3, #0]
	sub	sp, #8
	mov	lr, r3
	cmp	r0, #0
	bne.n	.L0
	lsls	r3, r1, #1
	adds	r5, r3, #5
	ldrb	r3, [r2, #0]
	movs	r7, #0
	movs	r6, #5
	movs	r4, #0
	movs	r0, #0
	cmp	r3, #0
	beq.n	.L1
	cmp	r1, #0
	bne.n	.L2
	ldr	r3, [pc, #96]
	ldrb	r6, [r3, #0]
	b.n	.L3
.L2:
	adds	r4, #1
.L1:
	adds	r0, #1
	cmp	r0, #4
	bgt.n	.L3
	ldrb	r3, [r2, r0]
	cmp	r3, #0
	beq.n	.L1
	cmp	r1, r4
	bne.n	.L2
	ldr	r3, [pc, #72]
	ldrb	r6, [r3, r0]
	b.n	.L3
.L0:
	cmp	r1, #3
	bgt.n	.L4
	adds	r5, r1, #0
	movs	r7, #5
	movs	r6, #13
	b.n	.L3
.L4:
	adds	r5, r1, #4
	movs	r7, #8
	movs	r6, #20
.L3:
	movs	r1, #1
	mov	r2, ip
	adds	r3, r1, #0
	eors	r3, r2
	negs	r2, r3
	orrs	r2, r3
	lsrs	r2, r2, #31
	movs	r3, #15
	subs	r3, r3, r2
	mov	r2, lr
	ldr	r0, [r2, #36]
	str	r1, [sp, #0]
	str	r3, [sp, #4]
	adds	r1, r7, #0
	adds	r2, r5, #0
	adds	r3, r6, #0
	bl	Func_080a2268
	add	sp, #8
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001f2c
	.4byte 0x080af2fc
