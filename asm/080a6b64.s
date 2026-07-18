@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080a6b64
	.thumb_func
Func_080a6b64:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #316]
	ldr	r3, [r3, #0]
	sub	sp, #4
	mov	sl, r3
	mov	r8, r0
	adds	r6, r2, #0
	bl	Func_08015270
	movs	r3, #11
	str	r3, [sp, #0]
	movs	r2, #11
	movs	r3, #16
	mov	r0, r8
	movs	r1, #0
	bl	Func_08015070
	movs	r3, #136
	lsls	r3, r3, #2
	add	r3, sl
	ldrh	r2, [r3, #0]
	movs	r3, #2
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L0
	ldr	r0, [pc, #272]
	mov	r1, r8
	movs	r2, #0
	movs	r3, #88
	bl	Func_08015080
	b.n	.L1
.L0:
	ldr	r0, [pc, #260]
	mov	r1, r8
	movs	r2, #0
	movs	r3, #88
	bl	Func_08015080
.L1:
	ldr	r2, [r6, #8]
	lsls	r3, r2, #2
	adds	r5, r3, r2
	ldr	r3, [r6, #20]
	subs	r3, r3, r5
	lsls	r3, r3, #24
	lsrs	r3, r3, #24
	mov	fp, r3
	cmp	r3, #5
	bls.n	.L2
	movs	r1, #5
	mov	fp, r1
.L2:
	movs	r3, #34
	str	r3, [sp, #0]
	movs	r0, #5
	adds	r1, r5, #0
	mov	r2, r8
	movs	r3, #112
	bl	Func_080a2324
	movs	r2, #15
	ldr	r1, [r6, #20]
	ldr	r3, [r6, #8]
	mov	r0, r8
	str	r2, [sp, #0]
	movs	r2, #5
	bl	Func_080a21b0
	movs	r2, #96
	movs	r3, #0
	ldr	r0, [pc, #192]
	mov	r1, r8
	bl	Func_08015080
	movs	r2, #0
	mov	r3, fp
	mov	r9, r2
	cmp	r3, #0
	bls.n	.L3
	movs	r1, #228
	lsls	r3, r5, #1
	lsls	r1, r1, #1
	adds	r6, r3, r1
.L7:
	ldr	r3, [pc, #172]
	add	r3, sl
	ldrb	r0, [r3, #0]
	bl	Func_08077008
	mov	r2, sl
	ldrh	r3, [r6, r2]
	adds	r5, r0, #0
	ldr	r0, [pc, #160]
	ands	r0, r3
	bl	Func_08077080
	adds	r7, r0, #0
	ldrb	r2, [r7, #9]
	movs	r1, #58
	ldrsh	r3, [r5, r1]
	cmp	r2, r3
	ble.n	.L4
	movs	r0, #2
	bl	Func_080150b8
	b.n	.L5
.L4:
	mov	r2, sl
	ldrh	r3, [r6, r2]
	ldr	r0, [pc, #128]
	ands	r0, r3
	bl	Func_080a735c
	cmp	r0, #0
	beq.n	.L6
	movs	r0, #4
	bl	Func_080150b8
	b.n	.L5
.L6:
	movs	r0, #15
	bl	Func_080150b8
.L5:
	mov	r1, sl
	ldrh	r3, [r6, r1]
	ldr	r0, [pc, #96]
	mov	r2, r9
	ands	r0, r3
	lsls	r5, r2, #4
	ldr	r3, [pc, #92]
	adds	r5, #8
	adds	r0, r0, r3
	mov	r1, r8
	movs	r2, #16
	adds	r3, r5, #0
	bl	Func_08015080
	ldrb	r0, [r7, #9]
	movs	r3, #104
	movs	r1, #2
	mov	r2, r8
	str	r5, [sp, #0]
	bl	Func_080150a8
	movs	r0, #15
	bl	Func_080150b8
	mov	r3, r9
	adds	r3, #1
	lsls	r3, r3, #24
	lsrs	r3, r3, #24
	mov	r9, r3
	adds	r6, #2
	cmp	fp, r9
	bhi.n	.L7
.L3:
	movs	r0, #1
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x03001f2c
	.4byte 0x00000ae1
	.4byte 0x00000b89
	.4byte 0x00000aed
	.4byte 0x0000021a
	.4byte 0x00003fff
	.4byte 0x00000333
