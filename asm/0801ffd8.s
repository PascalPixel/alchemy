@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0801ffd8
	.thumb_func
Func_0801ffd8:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #148]
	sub	sp, #4
	mov	r8, r0
	mov	fp, r1
	mov	r9, r2
	ldr	r3, [r3, #0]
	cmp	r0, #0
	beq.n	.L0
	movs	r1, #141
	lsls	r1, r1, #2
	adds	r6, r3, r1
	subs	r1, #16
	adds	r1, r3, r1
	movs	r2, #0
	str	r1, [sp, #0]
	movs	r7, #0
	mov	sl, r2
.L2:
	ldr	r3, [pc, #116]
	lsls	r2, r7, #2
	ldr	r0, [r3, r2]
	bl	Func_08009030
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L1
	movs	r1, #2
	bl	Func_08009020
	adds	r2, r5, #0
	adds	r2, #38
	movs	r3, #0
	strb	r3, [r2, #0]
	movs	r1, #13
	ldrb	r3, [r5, #9]
	negs	r1, r1
	adds	r2, r1, #0
	ands	r3, r2
	strb	r3, [r5, #9]
.L1:
	ldr	r3, [sp, #0]
	stmia	r3!, {r5}
	adds	r2, r3, #0
	str	r2, [sp, #0]
	mov	r1, r8
	ldrh	r3, [r1, #12]
	add	r3, fp
	add	r3, sl
	lsls	r3, r3, #3
	adds	r3, #16
	strh	r3, [r6, #0]
	ldrh	r3, [r1, #14]
	add	r3, r9
	lsls	r3, r3, #3
	adds	r3, #16
	movs	r2, #3
	adds	r7, #1
	strh	r3, [r6, #8]
	add	sl, r2
	adds	r6, #2
	cmp	r7, #3
	ble.n	.L2
	movs	r1, #200
	ldr	r0, [pc, #32]
	lsls	r1, r1, #4
	bl	Func_080041d8
.L0:
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001f2c
	.4byte 0x08073854
	.4byte 0x080200cd
