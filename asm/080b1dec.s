@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080b1dec
	.thumb_func
Func_080b1dec:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	adds	r5, r1, #0
	mov	r9, r0
	adds	r0, r5, #0
	sub	sp, #4
	bl	Func_08077008
	movs	r2, #0
	mov	r3, r9
	mov	sl, r0
	movs	r7, #8
	mov	r8, r2
	cmp	r3, #0
	beq.n	.L0
	mov	r0, r9
	bl	Func_08015060
	adds	r0, r5, #0
	bl	Func_08077248
	cmp	r0, #0
	bne.n	.L1
	ldr	r0, [pc, #88]
	mov	r1, r9
	movs	r2, #8
	movs	r3, #20
	bl	Func_08015078
	b.n	.L0
.L1:
	movs	r5, #0
	movs	r6, #216
.L5:
	mov	r2, sl
	ldrh	r3, [r6, r2]
	cmp	r3, #0
	beq.n	.L2
	adds	r0, r3, #0
	mov	r3, r8
	str	r3, [sp, #0]
	movs	r1, #27
	adds	r3, r7, #0
	mov	r2, r9
	bl	Func_080152d0
	movs	r3, #252
	strb	r3, [r0, #15]
.L2:
	adds	r7, #16
	cmp	r5, #4
	bne.n	.L3
	movs	r2, #16
	movs	r7, #8
	add	r8, r2
.L3:
	cmp	r5, #9
	bne.n	.L4
	movs	r3, #16
	movs	r7, #8
	add	r8, r3
.L4:
	adds	r5, #1
	adds	r6, #2
	cmp	r5, #14
	ble.n	.L5
.L0:
	add	sp, #4
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x00000c91
