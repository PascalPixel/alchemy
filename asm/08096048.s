@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08096048
	.thumb_func
Func_08096048:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r3, [pc, #228]
	movs	r2, #250
	lsls	r2, r2, #1
	adds	r3, r3, r2
	adds	r7, r0, #0
	ldr	r0, [r3, #0]
	sub	sp, #12
	bl	Func_08092054
	movs	r3, #64
	adds	r3, r3, r7
	movs	r2, #0
	ldrsb	r2, [r3, r2]
	mov	sl, r3
	mov	r8, r2
	cmp	r2, #0
	bne.n	.L0
	ldr	r3, [r0, #8]
	mov	r6, sp
	str	r3, [r6, #0]
	ldr	r3, [r0, #12]
	str	r3, [r6, #4]
	ldr	r3, [r0, #16]
	str	r3, [r6, #8]
	bl	Func_08004458
	lsls	r5, r0, #2
	adds	r5, r5, r0
	movs	r3, #160
	lsls	r3, r3, #12
	lsls	r5, r5, #1
	adds	r5, r5, r3
	bl	Func_08004458
	adds	r2, r6, #0
	adds	r1, r0, #0
	adds	r0, r5, #0
	bl	Func_0800447c
	adds	r0, r6, #0
	bl	Func_080974d8
	ldr	r2, [r6, #0]
	str	r2, [r7, #20]
	ldr	r3, [r6, #8]
	movs	r0, #240
	movs	r1, #192
	str	r3, [r7, #24]
	str	r2, [r7, #4]
	str	r3, [r7, #8]
	lsls	r0, r0, #15
	str	r2, [r6, #0]
	lsls	r1, r1, #8
	adds	r2, r6, #0
	str	r3, [r6, #8]
	bl	Func_0800447c
	ldr	r3, [r6, #0]
	str	r3, [r7, #12]
	ldr	r3, [r6, #8]
	str	r3, [r7, #16]
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r7, #36]
	movs	r3, #160
	lsls	r3, r3, #11
	str	r3, [r7, #32]
	adds	r3, r7, #0
	adds	r3, #66
	mov	r2, r8
	strb	r2, [r3, #0]
	mov	r2, sl
	ldrb	r3, [r2, #0]
	adds	r3, #1
	strb	r3, [r2, #0]
	ldr	r3, [pc, #84]
	ldr	r3, [r3, #0]
	movs	r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L1
	movs	r0, #144
	bl	Func_080f9010
	b.n	.L1
.L0:
	mov	r3, r8
	cmp	r3, #1
	bne.n	.L2
	adds	r0, r7, #0
	bl	Func_0809ba34
	cmp	r0, #0
	bne.n	.L1
	mov	r2, sl
	ldrb	r3, [r2, #0]
	subs	r3, #1
	strb	r3, [r2, #0]
	b.n	.L1
.L2:
	mov	r3, r8
	cmp	r3, #2
	bne.n	.L1
	adds	r0, r7, #0
	bl	Func_0809ba34
	cmp	r0, #0
	bne.n	.L1
	adds	r0, r7, #0
	bl	Func_0809bb34
.L1:
	add	sp, #12
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x02000240
	.4byte 0x03001800
