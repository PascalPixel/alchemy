@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080270ac
	.thumb_func
Func_080270ac:
	push	{r5, lr}
	mov	r5, r9
	push	{r5}
	sub	sp, #8
	mov	r5, sp
	mov	r3, r9
	str	r3, [sp, #4]
	adds	r0, r5, #0
	movs	r3, #255
	strh	r3, [r5, #0]
	bl	Func_0802281c
	adds	r0, r5, #0
	movs	r1, #1
	bl	Func_080b50e0
	add	sp, #8
	pop	{r3}
	mov	r9, r3
	pop	{r5}
	pop	{r0}
	bx	r0
