@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080b110c
	.thumb_func
Func_080b110c:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r6, r0, #0
	sub	sp, #4
	adds	r5, r1, #0
	mov	r8, r2
	adds	r7, r3, #0
	cmp	r6, #0
	bne.n	.L0
	b.n	.L1
.L4:
	ldr	r0, [pc, #112]
	adds	r1, r6, #0
	movs	r2, #0
	b.n	.L2
.L3:
	ldr	r5, [pc, #108]
	adds	r1, r6, #0
	adds	r0, r5, #0
	movs	r2, #0
	movs	r3, #8
	bl	Func_08015080
	movs	r3, #8
	str	r3, [sp, #0]
	mov	r0, r8
	movs	r1, #5
	adds	r2, r6, #0
	movs	r3, #32
	subs	r5, #3
	bl	Func_080150b0
	adds	r0, r5, #0
	adds	r1, r6, #0
	movs	r2, #72
.L2:
	movs	r3, #8
	bl	Func_08015080
	b.n	.L1
.L0:
	adds	r0, r6, #0
	bl	Func_08015270
	ldr	r0, [pc, #60]
	movs	r3, #0
	adds	r0, r5, r0
	adds	r1, r6, #0
	movs	r2, #0
	bl	Func_08015080
	mov	r3, r8
	cmp	r3, #0
	bne.n	.L3
	cmp	r7, #1
	beq.n	.L4
	cmp	r7, #2
	bne.n	.L3
	ldr	r0, [pc, #36]
	adds	r1, r6, #0
	movs	r2, #0
	movs	r3, #8
	bl	Func_08015080
.L1:
	add	sp, #4
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x00000c92
	.4byte 0x00000c8b
	.4byte 0x00000182
	.4byte 0x00000c93
