@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080a1cb0
	.thumb_func
Func_080a1cb0:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r3, [pc, #72]
	movs	r2, #56
	sub	sp, #4
	ldr	r3, [r3, #0]
	mov	r8, r2
	cmp	r0, #1
	beq.n	.L0
	movs	r2, #40
	mov	r8, r2
.L0:
	adds	r5, r3, #0
	adds	r5, #72
	movs	r3, #5
	movs	r6, #0
	adds	r7, r5, #0
	mov	sl, r3
.L2:
	ldmia	r7!, {r3}
	cmp	r3, #0
	beq.n	.L1
	mov	r2, sl
	str	r2, [sp, #0]
	adds	r0, r5, #0
	adds	r1, r6, #0
	movs	r2, #116
	mov	r3, r8
	bl	Func_080a1c6c
.L1:
	adds	r6, #1
	adds	r5, #4
	cmp	r6, #14
	ble.n	.L2
	add	sp, #4
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001f2c
