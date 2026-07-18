@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0801868c
	.thumb_func
Func_0801868c:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #4
	str	r2, [sp, #0]
	adds	r5, r3, #0
	ldr	r3, [pc, #228]
	ldr	r2, [sp, #36]
	ldr	r3, [r3, #0]
	mov	r8, r2
	mov	r9, r3
	movs	r2, #30
	ldr	r3, [sp, #0]
	mov	sl, r2
	ldr	r2, [sp, #44]
	ldr	r7, [r3, #0]
	movs	r3, #2
	ands	r3, r2
	mov	fp, r1
	ldr	r4, [sp, #40]
	ldr	r6, [r1, #0]
	cmp	r3, #0
	bne.n	.L0
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L1
	adds	r1, r5, #0
	mov	r2, r8
	adds	r3, r4, #0
	bl	Func_08018a50
	b.n	.L0
.L1:
	adds	r1, r5, #0
	mov	r2, r8
	adds	r3, r4, #0
	bl	Func_08018850
.L0:
	ldr	r1, [r5, #0]
	cmp	r1, #0
	bne.n	.L2
	mov	r0, r8
	ldr	r3, [r0, #0]
	cmp	r3, #0
	beq.n	.L3
.L2:
	ldr	r2, [sp, #44]
	movs	r3, #2
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L4
	adds	r3, r1, #0
	adds	r3, #19
	lsrs	r3, r3, #3
	str	r3, [r5, #0]
	mov	r0, r8
	ldr	r3, [r0, #0]
	adds	r3, #15
	lsrs	r3, r3, #3
	str	r3, [r0, #0]
	ldr	r3, [pc, #128]
	add	r3, r9
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L5
	ldr	r3, [r5, #0]
	movs	r2, #29
	adds	r3, #2
	str	r3, [r5, #0]
	mov	sl, r2
	adds	r1, r3, #0
	b.n	.L4
.L5:
	ldr	r1, [r5, #0]
.L4:
	adds	r3, r6, r1
	cmp	r3, sl
	ble.n	.L6
	mov	r0, sl
	subs	r3, r3, r0
	subs	r3, r6, r3
	adds	r6, r3, #0
	cmp	r3, #0
	bge.n	.L6
	movs	r6, #0
.L6:
	mov	r3, r8
	ldr	r2, [r3, #0]
	adds	r3, r7, r2
	cmp	r3, #20
	ble.n	.L7
	subs	r3, #20
	subs	r3, r7, r3
	adds	r7, r3, #0
	cmp	r3, #0
	bge.n	.L7
	movs	r7, #0
.L7:
	cmp	r6, #0
	bge.n	.L8
	movs	r6, #0
.L8:
	cmp	r7, #0
	bge.n	.L9
	movs	r7, #0
.L9:
	mov	r0, sl
	subs	r3, r0, r1
	cmp	r6, r3
	bls.n	.L10
	adds	r6, r3, #0
.L10:
	movs	r3, #20
	subs	r2, r3, r2
	cmp	r7, r2
	bls.n	.L11
	adds	r7, r2, #0
.L11:
	mov	r2, fp
	str	r6, [r2, #0]
	ldr	r3, [sp, #0]
	str	r7, [r3, #0]
.L3:
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001e8c
	.4byte 0x00000ea4
