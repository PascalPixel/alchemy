@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080a5578
	.thumb_func
Func_080a5578:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #136]
	ldr	r3, [r3, #0]
	movs	r2, #134
	mov	r8, r3
	lsls	r2, r2, #2
	adds	r5, r1, #0
	mov	r6, r8
	adds	r5, r5, r2
	adds	r6, #2
	mov	sl, r0
	ldrb	r0, [r6, r5]
	bl	Func_080a3d6c
	adds	r7, r0, #0
	ldrb	r0, [r6, r5]
	bl	Func_08077008
	ldrb	r3, [r6, r5]
	movs	r2, #152
	lsls	r2, r2, #2
	adds	r3, r3, r2
	mov	r2, r8
	ldrsb	r6, [r2, r3]
	adds	r3, r6, #1
	mov	fp, r0
	cmp	r3, r7
	ble.n	.L0
	subs	r6, r7, #1
.L0:
	movs	r1, #5
	adds	r0, r6, #0
	bl	Func_080022ec
	movs	r1, #5
	mov	r9, r0
	adds	r0, r6, #0
	bl	Func_080022fc
	movs	r1, #5
	mov	r8, r0
	adds	r0, r7, #0
	bl	Func_080022ec
	movs	r1, #5
	adds	r5, r0, #0
	adds	r0, r7, #0
	bl	Func_080022fc
	cmp	r0, #0
	beq.n	.L1
	adds	r5, #1
.L1:
	mov	r2, sl
	mov	r3, fp
	str	r3, [r2, #0]
	mov	r3, r9
	str	r3, [r2, #8]
	mov	r3, r8
	str	r5, [r2, #12]
	str	r3, [r2, #16]
	str	r7, [r2, #20]
	str	r6, [r2, #24]
	movs	r0, #1
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x03001f2c
