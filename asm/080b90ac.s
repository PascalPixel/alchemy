@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080b90ac
	.thumb_func
Func_080b90ac:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	sub	sp, #28
	mov	r5, sp
	movs	r0, #3
	adds	r1, r5, #0
	bl	Func_080b6c08
	cmp	r0, #0
	ble.n	.L0
	movs	r6, #0
	adds	r7, r5, #0
	mov	r8, r6
	adds	r5, r0, #0
.L1:
	ldrh	r0, [r6, r7]
	bl	Func_08077008
	ldr	r2, [pc, #32]
	adds	r3, r0, r2
	mov	r2, r8
	ldrh	r0, [r6, r7]
	strb	r2, [r3, #0]
	subs	r5, #1
	bl	Func_08077010
	adds	r6, #2
	cmp	r5, #0
	bne.n	.L1
.L0:
	add	sp, #28
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x0000012b
