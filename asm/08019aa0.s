@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08019aa0
	.thumb_func
Func_08019aa0:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	ldr	r3, [pc, #232]
	ldr	r3, [r3, #0]
	sub	sp, #24
	mov	r8, r3
	adds	r6, r1, #0
	movs	r3, #8
	movs	r1, #1
	mov	r9, r2
	str	r3, [sp, #20]
	str	r3, [sp, #16]
	adds	r7, r0, #0
	bl	Func_08018038
	movs	r2, #235
	lsls	r2, r2, #4
	lsls	r3, r0, #1
	adds	r3, r3, r2
	mov	r2, r8
	ldrh	r3, [r2, r3]
	movs	r5, #0
	mov	sl, r0
	cmp	r3, #0
	beq.n	.L0
	add	r0, sp, #8
	add	r1, sp, #20
	add	r2, sp, #16
	add	r3, sp, #12
	str	r0, [sp, #0]
	adds	r0, r7, #0
	bl	Func_080187ac
	ldr	r2, [sp, #12]
	movs	r3, #30
	subs	r3, r3, r2
	ldr	r4, [sp, #8]
	asrs	r0, r3, #1
	movs	r3, #15
	subs	r3, r3, r4
	asrs	r3, r3, #1
	mov	r7, r9
	adds	r1, r3, r7
	str	r0, [sp, #20]
	str	r1, [sp, #16]
	cmp	r6, #0
	beq.n	.L1
	adds	r3, r4, #0
	str	r5, [sp, #0]
	bl	Func_080162d4
	adds	r5, r0, #0
	b.n	.L2
.L1:
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r2, #0
	movs	r3, #0
	bl	Func_080162d4
	adds	r5, r0, #0
	strh	r6, [r5, #8]
	strh	r6, [r5, #10]
.L2:
	movs	r3, #0
	adds	r0, r5, #0
	mov	r1, sl
	movs	r2, #0
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	bl	Func_080165d8
	cmp	r0, #0
	bne.n	.L3
	adds	r0, r5, #0
	movs	r1, #1
	bl	Func_08016418
	b.n	.L0
.L4:
	movs	r0, #1
	bl	Func_080030f8
.L3:
	bl	Func_08017364
	cmp	r0, #0
	beq.n	.L4
	cmp	r6, #0
	beq.n	.L5
	adds	r0, r5, #0
	movs	r1, #0
	bl	Func_08016418
	b.n	.L6
.L7:
	movs	r0, #1
	bl	Func_080030f8
.L6:
	adds	r0, r5, #0
	bl	Func_08017394
	cmp	r0, #0
	beq.n	.L7
	b.n	.L8
.L5:
	adds	r0, r5, #0
	movs	r1, #1
	bl	Func_08016418
.L8:
	ldr	r3, [pc, #32]
	movs	r2, #0
	add	r3, r8
	strh	r2, [r3, #0]
	ldr	r3, [pc, #28]
	add	r3, r8
	strh	r2, [r3, #0]
.L0:
	add	sp, #24
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001e8c
	.4byte 0x000012f4
	.4byte 0x000012f6
