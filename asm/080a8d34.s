@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080a8d34
	.thumb_func
Func_080a8d34:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #484]
	mov	fp, r2
	ldr	r3, [r3, #0]
	ldr	r2, [r2, #8]
	mov	r1, fp
	mov	r8, r3
	lsls	r3, r2, #2
	adds	r3, r3, r2
	ldr	r2, [r1, #16]
	adds	r3, r3, r2
	mov	r2, r8
	str	r3, [r1, #24]
	adds	r7, r0, #0
	ldr	r0, [r2, #44]
	sub	sp, #8
	bl	Func_08015270
	movs	r0, #1
	bl	Func_080030f8
	mov	r1, fp
	ldr	r3, [r1, #24]
	movs	r2, #228
	lsls	r2, r2, #1
	lsls	r3, r3, #1
	adds	r3, r3, r2
	mov	r1, r8
	ldrh	r2, [r1, r3]
	adds	r3, r2, #0
	cmp	r3, #0
	beq.n	.L0
	ldr	r5, [pc, #424]
	ldr	r3, [pc, #428]
	adds	r0, r5, #0
	ands	r0, r2
	adds	r0, r0, r3
	ldr	r1, [r1, #44]
	movs	r2, #0
	movs	r3, #0
	bl	Func_08015080
	mov	r2, fp
	ldr	r3, [r2, #24]
	movs	r1, #228
	lsls	r1, r1, #1
	lsls	r3, r3, #1
	adds	r3, r3, r1
	mov	r2, r8
	ldrh	r3, [r2, r3]
	ands	r5, r3
	adds	r0, r5, #0
	bl	Func_08077080
	movs	r3, #104
	adds	r5, r0, #0
	str	r3, [sp, #0]
	adds	r0, r7, #0
	movs	r3, #224
	movs	r1, #0
	movs	r2, #96
	bl	Func_08015068
	movs	r3, #0
	mov	sl, r3
	ldrb	r3, [r5, #12]
	cmp	r3, #0
	bne.n	.L1
	ldrb	r0, [r5, #1]
	movs	r3, #64
	ands	r3, r0
	cmp	r3, #0
	beq.n	.L2
	b.n	.L3
.L1:
	ldrb	r0, [r5, #1]
.L3:
	movs	r1, #2
	mov	sl, r1
.L2:
	movs	r3, #128
	ands	r3, r0
	cmp	r3, #0
	beq.n	.L4
	mov	r2, sl
	movs	r3, #1
	orrs	r2, r3
	mov	sl, r2
.L4:
	mov	r3, sl
	cmp	r3, #3
	bne.n	.L5
	ldr	r0, [pc, #320]
	b.n	.L6
.L5:
	mov	r1, sl
	cmp	r1, #2
	bne.n	.L7
	ldr	r0, [pc, #316]
.L6:
	adds	r1, r7, #0
	movs	r2, #0
	movs	r3, #96
	bl	Func_08015080
	b.n	.L0
.L7:
	mov	r2, sl
	cmp	r2, #1
	bne.n	.L0
	ldr	r0, [pc, #300]
	adds	r1, r7, #0
	movs	r2, #0
	movs	r3, #96
	bl	Func_08015080
.L0:
	mov	r3, fp
	ldr	r2, [r3, #8]
	lsls	r3, r2, #2
	adds	r3, r3, r2
	movs	r1, #0
	mov	sl, r1
	lsls	r3, r3, #1
	movs	r1, #228
	add	r3, r8
	lsls	r1, r1, #1
	movs	r2, #1
	adds	r1, r1, r3
	mov	r9, r2
	movs	r6, #2
	mov	r8, r1
.L13:
	mov	r2, fp
	ldr	r3, [r2, #16]
	cmp	sl, r3
	bne.n	.L8
	mov	r1, r8
	ldrh	r3, [r1, #0]
	ldr	r0, [pc, #228]
	ands	r0, r3
	bl	Func_08077080
	adds	r5, r0, #0
	ldrb	r3, [r5, #2]
	cmp	r3, #4
	beq.n	.L9
	adds	r1, r3, #0
	movs	r3, #0
	str	r3, [sp, #0]
	adds	r1, #1
	adds	r0, r7, #0
	movs	r2, #24
	adds	r3, r6, #0
	bl	Func_08015280
	mov	r2, r9
	movs	r3, #14
	str	r2, [sp, #0]
	str	r3, [sp, #4]
	adds	r0, r7, #0
	movs	r1, #9
	adds	r2, r6, #0
	movs	r3, #15
	bl	Func_080a2268
	mov	r1, r9
	movs	r2, #14
	str	r1, [sp, #0]
	str	r2, [sp, #4]
	b.n	.L10
.L9:
	mov	r3, r9
	movs	r1, #14
	str	r3, [sp, #0]
	str	r1, [sp, #4]
	adds	r0, r7, #0
	movs	r1, #9
	adds	r2, r6, #0
	movs	r3, #19
	bl	Func_080a2268
	b.n	.L11
.L8:
	mov	r2, r8
	ldrh	r3, [r2, #0]
	ldr	r0, [pc, #140]
	ands	r0, r3
	bl	Func_08077080
	adds	r5, r0, #0
	ldrb	r3, [r5, #2]
	cmp	r3, #4
	beq.n	.L12
	adds	r1, r3, #0
	movs	r3, #4
	str	r3, [sp, #0]
	adds	r1, #1
	adds	r0, r7, #0
	movs	r2, #24
	adds	r3, r6, #0
	bl	Func_08015280
	mov	r3, r9
	movs	r1, #15
	str	r3, [sp, #0]
	str	r1, [sp, #4]
	adds	r0, r7, #0
	movs	r1, #9
	adds	r2, r6, #0
	movs	r3, #15
	bl	Func_080a2268
	mov	r2, r9
	movs	r3, #15
	str	r2, [sp, #0]
	str	r3, [sp, #4]
.L10:
	adds	r0, r7, #0
	movs	r1, #25
	adds	r2, r6, #0
	movs	r3, #3
	bl	Func_080a2268
	b.n	.L11
.L12:
	mov	r1, r9
	movs	r2, #15
	str	r1, [sp, #0]
	str	r2, [sp, #4]
	adds	r0, r7, #0
	movs	r1, #9
	adds	r2, r6, #0
	movs	r3, #19
	bl	Func_080a2268
.L11:
	movs	r1, #1
	add	sl, r1
	movs	r3, #2
	mov	r2, sl
	adds	r6, #2
	add	r8, r3
	cmp	r2, #4
	ble.n	.L13
	movs	r0, #1
	bl	Func_080030f8
	movs	r0, #1
	add	sp, #8
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x03001f2c
	.4byte 0x00003fff
	.4byte 0x0000053a
	.4byte 0x00000b15
	.4byte 0x00000b14
	.4byte 0x00000b13
