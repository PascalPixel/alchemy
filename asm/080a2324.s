@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080a2324
	.thumb_func
Func_080a2324:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	mov	r9, r3
	ldr	r3, [pc, #136]
	ldr	r3, [r3, #0]
	sub	sp, #4
	mov	r8, r3
	movs	r2, #13
	adds	r3, #72
	movs	r6, #31
.L1:
	ldmia	r3!, {r5}
	cmp	r5, #0
	beq.n	.L0
	strb	r2, [r5, #5]
.L0:
	subs	r6, #1
	cmp	r6, #0
	bge.n	.L1
	adds	r6, r1, #0
	adds	r0, r0, r6
	cmp	r6, r0
	bge.n	.L2
	lsls	r2, r6, #2
	adds	r3, r2, #0
	adds	r3, #72
	mov	r1, r8
	ldr	r5, [r1, r3]
	cmp	r5, #0
	beq.n	.L2
	movs	r3, #134
	lsls	r3, r3, #2
	add	r3, r8
	ldrb	r3, [r3, #0]
	subs	r3, #1
	cmp	r6, r3
	bgt.n	.L2
	adds	r3, r2, r1
	adds	r2, r3, #0
	ldr	r7, [sp, #32]
	mov	sl, r0
	adds	r2, #72
.L3:
	mov	r3, r9
	strh	r3, [r5, #6]
	strh	r7, [r5, #8]
	adds	r0, r5, #0
	str	r2, [sp, #0]
	bl	Func_080a17c4
	adds	r6, #1
	movs	r3, #1
	strb	r3, [r5, #5]
	adds	r7, #16
	ldr	r2, [sp, #0]
	cmp	r6, sl
	bge.n	.L2
	adds	r2, #4
	ldr	r5, [r2, #0]
	cmp	r5, #0
	beq.n	.L2
	movs	r3, #134
	lsls	r3, r3, #2
	add	r3, r8
	ldrb	r3, [r3, #0]
	subs	r3, #1
	cmp	r6, r3
	ble.n	.L3
.L2:
	add	sp, #4
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001f2c
