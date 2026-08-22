@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080b7548
	.thumb_func
Func_080b7548:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	ldr	r3, [pc, #132]
	ldr	r2, [r3, #0]
	movs	r3, #100
	adds	r2, #2
	ldrsh	r3, [r2, r3]
	sub	sp, #76
	movs	r6, #0
	mov	r9, r2
	cmp	r3, #255
	beq.n	.L0
	add	r0, sp, #48
	adds	r5, r0, #0
	movs	r4, #0
	movs	r2, #100
	mov	r1, r9
.L2:
	ldrh	r3, [r1, r2]
	adds	r6, #1
	strh	r3, [r4, r5]
	adds	r2, #2
	adds	r4, #2
	cmp	r6, #5
	bgt.n	.L1
	ldrsh	r3, [r1, r2]
	cmp	r3, #255
	bne.n	.L2
	b.n	.L1
.L0:
	add	r0, sp, #48
.L1:
	add	r1, sp, #24
	mov	sl, r1
	mov	r8, sp
	adds	r1, r6, #0
	mov	r2, sl
	mov	r3, r8
	bl	Func_080b7424
	cmp	r6, #0
	ble.n	.L3
	movs	r5, #0
	movs	r7, #100
.L5:
	mov	r2, r9
	ldrsh	r0, [r2, r7]
	cmp	r0, #254
	beq.n	.L4
	bl	Func_080b7dd0
	mov	r1, sl
	ldr	r3, [r5, r1]
	lsls	r3, r3, #16
	str	r3, [r0, #12]
	mov	r2, r8
	ldr	r3, [r5, r2]
	lsls	r3, r3, #16
	str	r3, [r0, #16]
.L4:
	subs	r6, #1
	adds	r5, #4
	adds	r7, #2
	cmp	r6, #0
	bne.n	.L5
.L3:
	add	sp, #76
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x03001e74
