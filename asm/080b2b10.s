@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080b2b10
	.thumb_func
Func_080b2b10:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #616]
	sub	sp, #12
	ldr	r7, [r3, #0]
	movs	r1, #1
	ldr	r2, [pc, #612]
	movs	r0, #0
	str	r0, [sp, #8]
	str	r1, [sp, #4]
	adds	r3, r7, r2
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	ldr	r0, [pc, #600]
	mov	r9, r3
	bl	Func_080b28d4
	movs	r5, #2
	movs	r1, #12
	movs	r2, #13
	movs	r3, #3
	movs	r0, #1
	str	r5, [sp, #0]
	bl	Func_08015010
	movs	r4, #224
	lsls	r4, r4, #2
	adds	r3, r7, r4
	ldr	r2, [r3, #0]
	add	r1, sp, #4
	mov	fp, r0
	movs	r0, #234
	ldrb	r1, [r1, #0]
	movs	r3, #4
	lsls	r0, r0, #2
	strb	r3, [r2, #5]
	adds	r3, r7, r0
	strb	r1, [r3, #0]
	ldr	r2, [sp, #8]
	mov	r0, fp
	str	r2, [sp, #0]
	movs	r1, #2
	movs	r2, #0
	movs	r3, #8
	bl	Func_080a1028
	movs	r0, #1
	movs	r1, #16
	movs	r2, #23
	movs	r3, #3
	str	r5, [sp, #0]
	bl	Func_08015010
	movs	r6, #0
	movs	r5, #219
	mov	sl, r6
	mov	r8, r6
	lsls	r5, r5, #2
	str	r0, [sp, #8]
	b.n	.L0
.L2:
	adds	r5, #2
	adds	r6, #1
.L0:
	ldr	r4, [pc, #504]
	adds	r3, r7, r4
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r6, r3
	bge.n	.L1
	adds	r3, r7, #2
	ldrsh	r0, [r3, r5]
	mov	r1, r9
	mov	sl, r0
	bl	Func_080b27b0
	cmp	r0, #0
	beq.n	.L2
.L1:
	movs	r2, #1
	str	r2, [sp, #4]
.L9:
	mov	r3, r8
	cmp	r3, #0
	beq.n	.L3
	movs	r4, #0
	ldr	r0, [pc, #460]
	mov	r8, r4
	bl	Func_080b28d4
	movs	r5, #219
	movs	r0, #1
	movs	r6, #0
	lsls	r5, r5, #2
	str	r0, [sp, #4]
	b.n	.L4
.L5:
	adds	r5, #2
	adds	r6, #1
.L4:
	ldr	r1, [pc, #440]
	adds	r3, r7, r1
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r6, r3
	bge.n	.L3
	adds	r3, r7, #2
	ldrsh	r2, [r3, r5]
	mov	sl, r2
	mov	r0, sl
	mov	r1, r9
	bl	Func_080b27b0
	cmp	r0, #0
	beq.n	.L5
.L3:
	ldr	r0, [sp, #4]
	cmp	r0, #0
	beq.n	.L6
	ldr	r2, [pc, #400]
	movs	r1, #0
	str	r1, [sp, #4]
	adds	r3, r7, r2
	movs	r1, #0
	ldrsb	r1, [r3, r1]
	adds	r0, r6, r1
	bl	Func_080022fc
	movs	r3, #219
	adds	r6, r0, #0
	lsls	r1, r6, #1
	lsls	r3, r3, #2
	adds	r2, r1, r3
	adds	r3, r7, #2
	adds	r1, r1, r6
	ldrsh	r4, [r3, r2]
	lsls	r1, r1, #3
	subs	r1, #12
	mov	r0, fp
	movs	r2, #0
	mov	sl, r4
	bl	Func_080b0a6c
	movs	r1, #234
	lsls	r1, r1, #2
	adds	r2, r7, r1
	movs	r3, #3
	mov	r0, fp
	adds	r1, r6, #0
	strb	r3, [r2, #0]
	bl	Func_080b2e30
	mov	r1, sl
	ldr	r0, [sp, #8]
	bl	Func_080b2ed8
.L6:
	ldr	r1, [pc, #332]
	ldr	r2, [r1, #0]
	movs	r3, #1
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L7
	movs	r0, #1
	bl	Func_080030f8
	mov	r1, r9
	mov	r0, sl
	bl	Func_080b2778
	mov	r1, r9
	adds	r5, r0, #0
	mov	r0, sl
	bl	Func_080b27b0
	cmp	r0, #0
	bne.n	.L8
	movs	r0, #113
	bl	Func_080f9010
	b.n	.L9
.L8:
	mov	r0, sl
	movs	r1, #1
	bl	Func_08015120
	adds	r0, r5, #0
	movs	r1, #5
	bl	Func_08015120
	ldr	r2, [pc, #272]
	mov	r8, r2
	mov	r0, r8
	bl	Func_080b28d4
	movs	r0, #0
	bl	Func_080b0664
	cmp	r0, #0
	beq.n	.L10
	mov	r0, r8
	adds	r0, #2
	bl	Func_080b2928
	movs	r3, #1
	mov	r8, r3
	b.n	.L9
.L10:
	ldr	r3, [pc, #240]
	ldr	r3, [r3, #16]
	cmp	r5, r3
	bls.n	.L11
	movs	r0, #113
	bl	Func_080f9010
	mov	r0, r8
	adds	r0, #1
	bl	Func_080b2928
	movs	r4, #1
	mov	r8, r4
	b.n	.L9
.L11:
	movs	r1, #1
	mov	r0, sl
	bl	Func_08015120
	mov	r0, r8
	adds	r0, #3
	bl	Func_080b28d4
	bl	Func_08015140
	mov	r1, r9
	mov	r0, sl
	bl	Func_080b2da8
	adds	r0, r6, #0
	bl	Func_080b3050
	negs	r0, r5
	bl	Func_08077230
	bl	Func_080b10cc
	mov	r0, sl
	movs	r1, #1
	bl	Func_08015120
	mov	r0, r8
	adds	r0, #4
	bl	Func_080b28d4
	bl	Func_080b280c
	cmp	r0, #0
	beq.n	.L12
	movs	r0, #1
	mov	r8, r0
	b.n	.L9
.L7:
	ldr	r3, [r1, #0]
	movs	r2, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L13
	movs	r0, #113
	bl	Func_080f9010
	b.n	.L12
.L13:
	ldr	r5, [pc, #124]
	ldr	r3, [r5, #0]
	movs	r2, #32
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L14
	movs	r0, #111
	bl	Func_080f9010
	movs	r1, #1
	str	r1, [sp, #4]
	subs	r6, #1
.L14:
	ldr	r3, [r5, #0]
	movs	r2, #16
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L15
	movs	r0, #111
	bl	Func_080f9010
	movs	r2, #1
	str	r2, [sp, #4]
	adds	r6, #1
.L15:
	movs	r0, #1
	bl	Func_080030f8
	b.n	.L9
.L12:
	bl	Func_080a1030
	movs	r1, #2
	ldr	r0, [sp, #8]
	bl	Func_08015018
	mov	r0, fp
	movs	r1, #2
	bl	Func_08015018
	movs	r0, #1
	bl	Func_080030f8
	movs	r0, #0
	add	sp, #12
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x03001f2c
	.4byte 0x000003aa
	.4byte 0x00000d26
	.4byte 0x000003a7
	.4byte 0x03001c94
	.4byte 0x00000d27
	.4byte 0x02000240
	.4byte 0x03001b04
