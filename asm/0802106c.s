@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0802106c
	.thumb_func
Func_0802106c:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #28
	movs	r2, #1
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r1, #13
	movs	r3, #7
	mov	sl, r2
	movs	r0, #7
	movs	r2, #18
	bl	Func_080162d4
	ldr	r5, [pc, #112]
	adds	r6, r0, #0
	adds	r1, r6, #0
	adds	r0, r5, #0
	movs	r2, #8
	movs	r3, #0
	bl	Func_0801e7c0
	adds	r0, r5, #1
	adds	r1, r6, #0
	movs	r2, #8
	movs	r3, #16
	adds	r5, #2
	bl	Func_0801e7c0
	adds	r0, r5, #0
	adds	r1, r6, #0
	movs	r2, #8
	movs	r3, #32
	bl	Func_0801e7c0
	bl	Func_08004080
	movs	r7, #0
	str	r0, [sp, #8]
	cmp	r0, #95
	bgt.n	.L0
	ldr	r2, [pc, #64]
	movs	r1, #128
	bl	Func_08003fa4
	movs	r1, #128
	lsls	r1, r1, #23
	adds	r2, r6, #0
	movs	r3, #0
	ldr	r0, [sp, #8]
	str	r7, [sp, #0]
	bl	Func_0801eadc
	add	r3, sp, #12
	str	r0, [r3, #0]
	ldrh	r1, [r6, #12]
	ldrh	r2, [r6, #14]
	mov	r8, r3
	lsls	r1, r1, #3
	lsls	r2, r2, #3
	subs	r1, #3
	adds	r2, #9
	mov	r0, r8
	bl	Func_080b0038
	b.n	.L1
.L10:
	movs	r0, #113
	movs	r7, #1
	bl	Func_080f9010
	negs	r7, r7
	b.n	.L2
	.4byte 0x00002080
	.4byte 0x080310a4
.L0:
	add	r2, sp, #12
	mov	r8, r2
.L1:
	ldr	r4, [pc, #28]
	ldr	r3, [pc, #32]
	ldr	r0, [pc, #32]
	adds	r1, r4, #0
	ldr	r2, [pc, #32]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r3, [pc, #12]
	ldr	r2, [pc, #28]
	strh	r3, [r4, #8]
	movs	r3, #1
	mov	fp, r3
	mov	r9, r2
	b.n	.L3
	.4byte 0x00006318
	.4byte 0x050001c0
	.4byte 0x040000d4
	.4byte 0x050001e0
	.4byte 0x84000008
	.4byte 0x03001b04
.L3:
	lsls	r5, r7, #1
	mov	r3, fp
	str	r3, [sp, #0]
	movs	r1, #1
	movs	r3, #14
	adds	r2, r5, #0
	adds	r0, r6, #0
	str	r3, [sp, #4]
	bl	Func_08020a60
	movs	r0, #1
	bl	Func_080030f8
	mov	r2, fp
	movs	r3, #15
	str	r2, [sp, #0]
	str	r3, [sp, #4]
	adds	r0, r6, #0
	movs	r3, #14
	movs	r1, #1
	adds	r2, r5, #0
	bl	Func_08020a60
	mov	r3, sl
	cmp	r3, #0
	beq.n	.L4
	movs	r2, #0
	mov	sl, r2
	ldrh	r2, [r6, #14]
	ldrh	r1, [r6, #12]
	adds	r2, r2, r5
	lsls	r1, r1, #3
	lsls	r2, r2, #3
	subs	r1, #3
	adds	r2, #9
	mov	r0, r8
	movs	r3, #3
	bl	Func_080b0030
.L4:
	mov	r0, r8
	bl	Func_080b0020
	mov	r1, r9
	ldr	r3, [r1, #0]
	movs	r2, #64
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L5
	movs	r0, #111
	bl	Func_080f9010
	movs	r2, #1
	movs	r3, #1
	subs	r7, #1
	negs	r2, r2
	mov	sl, r3
	cmp	r7, r2
	bne.n	.L6
	movs	r7, #2
.L6:
	ldr	r1, [pc, #104]
.L5:
	mov	r2, r9
	ldr	r3, [r2, #0]
	movs	r2, #128
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L7
	movs	r0, #111
	bl	Func_080f9010
	adds	r7, #1
	movs	r3, #1
	mov	sl, r3
	cmp	r7, #3
	bne.n	.L8
	movs	r7, #0
.L8:
	ldr	r1, [pc, #72]
.L7:
	mov	r2, r9
	ldr	r3, [r2, #0]
	movs	r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L9
	b.n	.L10
.L9:
	ldr	r3, [r1, #0]
	mov	r2, fp
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L3
	movs	r0, #112
	bl	Func_080f9010
.L2:
	movs	r1, #2
	adds	r0, r6, #0
	bl	Func_08016418
	movs	r0, #1
	bl	Func_080030f8
	ldr	r0, [sp, #8]
	bl	Func_08003f3c
	adds	r0, r7, #0
	add	sp, #28
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x03001b04
