@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080bd3e4
	.thumb_func
Func_080bd3e4:
	push	{r5, lr}
	mov	r5, r9
	push	{r5}
	mov	r3, r9
	sub	sp, #4
	str	r3, [sp, #0]
	adds	r5, r0, #0
	bl	Func_080771a0
	movs	r3, #255
	ldrb	r2, [r5, #0]
	ands	r0, r3
	movs	r4, #0
	movs	r1, #0
	cmp	r0, r2
	blt.n	.L0
.L1:
	adds	r1, #1
	cmp	r1, #7
	bgt.n	.L0
	ldrb	r3, [r5, r1]
	adds	r2, r2, r3
	cmp	r0, r2
	bge.n	.L1
	adds	r4, r1, #0
.L0:
	adds	r0, r4, #0
	add	sp, #4
	pop	{r3}
	mov	r9, r3
	pop	{r5}
	pop	{r1}
	bx	r1
