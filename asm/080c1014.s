@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080c1014
	.thumb_func
Func_080c1014:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	sub	sp, #28
	mov	r5, sp
	adds	r7, r0, #0
	adds	r1, r5, #0
	movs	r0, #3
	bl	Func_080b6c08
	cmp	r0, #0
	ble.n	.L0
	mov	r8, r5
	movs	r6, #0
	adds	r5, r0, #0
.L2:
	mov	r2, r8
	ldrsh	r0, [r6, r2]
	cmp	r0, r7
	beq.n	.L1
	movs	r1, #1
	bl	Func_080c0f98
.L1:
	subs	r5, #1
	adds	r6, #2
	cmp	r5, #0
	bne.n	.L2
.L0:
	add	sp, #28
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
