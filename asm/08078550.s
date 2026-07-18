@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08078550
	.thumb_func
Func_08078550:
	push	{r5, r6, r7, lr}
	sub	sp, #20
	mov	r5, sp
	adds	r0, r5, #0
	bl	Func_080796c4
	movs	r7, #0
	adds	r6, r5, #0
	cmp	r7, r0
	bge.n	.L0
	adds	r5, r0, #0
.L1:
	movs	r3, #0
	ldrsh	r0, [r6, r3]
	bl	Func_080784d8
	subs	r0, r7, r0
	adds	r7, r0, #0
	subs	r5, #1
	adds	r6, #2
	adds	r7, #15
	cmp	r5, #0
	bne.n	.L1
.L0:
	adds	r0, r7, #0
	add	sp, #20
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
