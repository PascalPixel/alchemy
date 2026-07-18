@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080a9b94
	.thumb_func
Func_080a9b94:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r3, [pc, #52]
	ldr	r3, [r3, #0]
	adds	r6, r3, #0
	sub	sp, #4
	mov	sl, r0
	mov	r8, r1
	adds	r7, r2, #0
	movs	r5, #0
	adds	r6, #72
.L1:
	ldmia	r6!, {r0}
	cmp	r0, #0
	beq.n	.L0
	adds	r1, r5, #0
	mov	r2, sl
	mov	r3, r8
	str	r7, [sp, #0]
	bl	Func_080a9bd8
.L0:
	adds	r5, #1
	cmp	r5, #31
	ble.n	.L1
	add	sp, #4
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001f2c
