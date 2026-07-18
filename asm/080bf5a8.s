@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080bf5a8
	.thumb_func
Func_080bf5a8:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r0, #0
	sub	sp, #4
	bl	Func_08077000
	movs	r1, #132
	adds	r3, r0, #0
	lsls	r1, r1, #1
	adds	r7, r3, #0
	adds	r3, r3, r1
	ldr	r3, [r3, #0]
	movs	r2, #0
	adds	r7, #8
	mov	r8, r2
	cmp	r2, r3
	bge.n	.L0
	adds	r5, r7, #0
.L2:
	movs	r3, #3
	ldrsb	r3, [r5, r3]
	cmp	r3, #0
	ble.n	.L1
	ldrb	r0, [r5, #2]
	str	r2, [sp, #0]
	bl	Func_08077008
	movs	r1, #56
	ldrsh	r3, [r0, r1]
	ldr	r2, [sp, #0]
	cmp	r3, #0
	beq.n	.L1
	ldrb	r3, [r5, #3]
	subs	r3, #1
	strb	r3, [r5, #3]
.L1:
	movs	r1, #128
	movs	r3, #1
	lsls	r1, r1, #1
	add	r8, r3
	adds	r3, r7, r1
	ldr	r3, [r3, #0]
	adds	r5, #4
	cmp	r8, r3
	blt.n	.L2
.L0:
	movs	r1, #128
	movs	r3, #0
	lsls	r1, r1, #1
	mov	r8, r3
	adds	r3, r7, r1
	ldr	r3, [r3, #0]
	cmp	r8, r3
	bge.n	.L3
	adds	r6, r7, #0
.L6:
	movs	r3, #3
	ldrsb	r3, [r6, r3]
	cmp	r3, #0
	bne.n	.L4
	ldrb	r5, [r6, #2]
	ldrb	r1, [r6, #0]
	ldrb	r2, [r6, #1]
	adds	r0, r5, #0
	bl	Func_080771b0
	ldrb	r2, [r6, #1]
	ldrb	r1, [r6, #0]
	adds	r0, r5, #0
	bl	Func_080771c0
	adds	r0, r5, #0
	bl	Func_08077010
	movs	r2, #1
	b.n	.L5
.L4:
	movs	r3, #1
	adds	r6, #4
	add	r8, r3
.L5:
	movs	r1, #128
	lsls	r1, r1, #1
	adds	r3, r7, r1
	ldr	r3, [r3, #0]
	cmp	r8, r3
	blt.n	.L6
.L3:
	adds	r0, r2, #0
	add	sp, #4
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
