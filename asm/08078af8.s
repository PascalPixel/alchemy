@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08078af8
	.thumb_func
Func_08078af8:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	adds	r6, r1, #0
	sub	sp, #4
	bl	Func_08077394
	ldr	r3, [pc, #80]
	mov	r8, r0
	movs	r1, #0
	ands	r6, r3
	movs	r7, #0
	movs	r5, #216
	mov	sl, r3
.L3:
	mov	r2, r8
	ldrh	r3, [r5, r2]
	mov	r2, sl
	ands	r3, r2
	cmp	r3, r6
	bne.n	.L0
	adds	r0, r6, #0
	str	r1, [sp, #0]
	bl	Func_08078414
	ldrb	r2, [r0, #3]
	movs	r3, #16
	ands	r3, r2
	ldr	r1, [sp, #0]
	cmp	r3, #0
	beq.n	.L1
	mov	r2, r8
	ldrh	r3, [r5, r2]
	lsrs	r3, r3, #11
	adds	r1, r3, #1
	b.n	.L2
.L1:
	adds	r1, #1
.L0:
	adds	r7, #1
	adds	r5, #2
	cmp	r7, #14
	ble.n	.L3
.L2:
	adds	r0, r1, #0
	add	sp, #4
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x000001ff
