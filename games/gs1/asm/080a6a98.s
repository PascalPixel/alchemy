@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080a6a98
	.thumb_func
Func_080a6a98:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	mov	r8, r2
	ldr	r3, [pc, #172]
	ldr	r2, [r2, #8]
	mov	r1, r8
	ldr	r7, [r3, #0]
	lsls	r3, r2, #2
	adds	r3, r3, r2
	ldr	r2, [r1, #16]
	adds	r3, r3, r2
	str	r3, [r1, #24]
	ldr	r0, [pc, #156]
	sub	sp, #8
	bl	Func_080770c0
	cmp	r0, #0
	bne.n	.L0
	ldr	r0, [r7, #44]
	bl	Func_08015270
	movs	r0, #1
	bl	Func_080030f8
	mov	r2, r8
	ldr	r3, [r2, #24]
	movs	r1, #228
	lsls	r3, r3, #1
	lsls	r1, r1, #1
	adds	r3, r3, r1
	ldrh	r2, [r7, r3]
	adds	r3, r2, #0
	cmp	r3, #0
	beq.n	.L1
	ldr	r0, [pc, #116]
	ldr	r3, [pc, #120]
	ands	r0, r2
	adds	r0, r0, r3
	ldr	r1, [r7, #44]
	movs	r2, #0
	movs	r3, #0
	bl	Func_08015080
	b.n	.L1
.L0:
	ldr	r0, [pc, #104]
	bl	Func_080770d0
.L1:
	movs	r2, #1
	movs	r6, #0
	mov	sl, r2
	movs	r5, #1
.L4:
	mov	r1, r8
	ldr	r3, [r1, #16]
	cmp	r6, r3
	bne.n	.L2
	mov	r2, sl
	movs	r3, #14
	ldr	r0, [r7, #32]
	movs	r1, #0
	str	r2, [sp, #0]
	str	r3, [sp, #4]
	adds	r2, r5, #0
	movs	r3, #15
	bl	Func_080a2268
	b.n	.L3
.L2:
	mov	r3, sl
	ldr	r0, [r7, #32]
	movs	r1, #0
	str	r3, [sp, #0]
	adds	r2, r5, #0
	movs	r3, #15
	str	r3, [sp, #4]
	bl	Func_080a2268
.L3:
	adds	r6, #1
	adds	r5, #2
	cmp	r6, #4
	ble.n	.L4
	movs	r0, #1
	bl	Func_080030f8
	movs	r0, #1
	add	sp, #8
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x03001f2c
	.4byte 0x00000151
	.4byte 0x000001ff
	.4byte 0x0000053a
	.4byte 0x000002ff
