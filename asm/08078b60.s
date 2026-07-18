@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08078b60
	.thumb_func
Func_08078b60:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	sub	sp, #32
	mov	r5, sp
	mov	r8, r0
	adds	r0, r5, #0
	movs	r7, #0
	bl	Func_080796c4
	cmp	r7, r0
	bge.n	.L0
	adds	r6, r5, #0
	adds	r5, r0, #0
.L1:
	ldrh	r0, [r6, #0]
	mov	r1, r8
	bl	Func_08078af8
	subs	r5, #1
	adds	r6, #2
	adds	r7, r7, r0
	cmp	r5, #0
	bne.n	.L1
.L0:
	adds	r0, r7, #0
	add	sp, #32
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
