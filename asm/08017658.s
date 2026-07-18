@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08017658
	.thumb_func
Func_08017658:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	adds	r5, r3, #0
	ldr	r3, [pc, #236]
	sub	sp, #36
	ldr	r3, [r3, #0]
	str	r2, [sp, #20]
	ldr	r2, [pc, #232]
	mov	r8, r3
	lsls	r3, r5, #4
	add	r2, r8
	lsrs	r3, r3, #20
	str	r1, [sp, #24]
	strh	r3, [r2, #0]
	ldr	r3, [pc, #224]
	movs	r6, #0
	add	r3, r8
	strh	r6, [r3, #0]
	ldr	r3, [pc, #220]
	movs	r1, #1
	ands	r5, r3
	bl	Func_08018038
	lsls	r3, r0, #1
	mov	sl, r0
	movs	r0, #235
	lsls	r0, r0, #4
	adds	r3, r3, r0
	mov	r2, r8
	ldrh	r3, [r2, r3]
	movs	r7, #0
	movs	r0, #0
	cmp	r3, #0
	beq.n	.L0
	add	r0, sp, #12
	str	r0, [sp, #0]
	add	r0, sp, #28
	add	r2, sp, #20
	str	r0, [sp, #4]
	add	r1, sp, #24
	add	r3, sp, #16
	mov	r9, r0
	mov	r0, sl
	str	r6, [sp, #8]
	bl	Func_0801868c
	ldr	r2, [sp, #16]
	cmp	r2, #0
	bne.n	.L1
	ldr	r4, [sp, #12]
	cmp	r4, #0
	bne.n	.L2
	movs	r0, #0
	b.n	.L0
.L1:
	ldr	r4, [sp, #12]
.L2:
	movs	r3, #1
	ands	r3, r5
	cmp	r3, #0
	bne.n	.L3
	movs	r3, #2
	orrs	r7, r3
.L3:
	movs	r1, #8
	adds	r3, r5, #0
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L4
	orrs	r7, r1
.L4:
	movs	r3, #16
	ands	r3, r5
	cmp	r3, #0
	beq.n	.L5
	movs	r3, #128
	orrs	r7, r3
.L5:
	movs	r3, #32
	ands	r3, r5
	cmp	r3, #0
	beq.n	.L6
	movs	r3, #128
	lsls	r3, r3, #1
	orrs	r7, r3
.L6:
	ldr	r0, [sp, #24]
	ldr	r1, [sp, #20]
	adds	r3, r4, #0
	str	r7, [sp, #0]
	bl	Func_080162d4
	adds	r6, r0, #0
	movs	r0, #0
	cmp	r6, #0
	beq.n	.L0
	mov	r2, r9
	str	r2, [sp, #0]
	movs	r5, #0
	adds	r0, r6, #0
	mov	r1, sl
	movs	r2, #0
	movs	r3, #0
	str	r5, [sp, #4]
	bl	Func_080165d8
	cmp	r0, #0
	bne.n	.L7
	adds	r0, r6, #0
	movs	r1, #1
	bl	Func_08016418
	movs	r0, #0
	b.n	.L0
.L7:
	ldr	r3, [pc, #44]
	add	r3, r8
	strb	r5, [r3, #0]
	ldr	r3, [pc, #40]
	add	r3, r8
	strb	r5, [r3, #0]
	adds	r0, r6, #0
.L0:
	add	sp, #36
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x03001e8c
	.4byte 0x000012f4
	.4byte 0x000012f6
	.4byte 0x0000ffff
	.4byte 0x000012fa
	.4byte 0x000012fb
