@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08078870
	.thumb_func
Func_08078870:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r2, #128
	ldr	r3, [pc, #68]
	lsls	r2, r2, #2
	adds	r5, r0, #0
	adds	r7, r1, #0
	movs	r6, #0
	mov	r8, r2
	mov	sl, r3
	adds	r5, #216
.L2:
	ldrh	r3, [r5, #0]
	mov	r2, r8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L0
	ldrh	r0, [r5, #0]
	bl	Func_08078414
	ldrb	r3, [r0, #2]
	cmp	r3, r7
	bne.n	.L0
	ldrh	r3, [r5, #0]
	mov	r0, sl
	ands	r0, r3
	b.n	.L1
.L0:
	adds	r6, #1
	adds	r5, #2
	cmp	r6, #14
	ble.n	.L2
	movs	r0, #0
.L1:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x000001ff
