@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080270d8
	.thumb_func
Func_080270d8:
	push	{r5, r6, lr}
	mov	r6, r9
	push	{r6}
	sub	sp, #132
	mov	r2, r9
	mov	r6, sp
	add	r3, sp, #128
	adds	r5, r2, #0
	str	r2, [r3, #0]
	adds	r1, r6, #0
	movs	r2, #52
	subs	r5, #8
	ldr	r0, [pc, #28]
	bl	Func_0801965c
	ldr	r3, [r5, #0]
	adds	r0, r6, #0
	ldr	r1, [r3, #68]
	movs	r2, #0
	movs	r3, #4
	bl	Func_08017aa4
	add	sp, #132
	pop	{r3}
	mov	r9, r3
	pop	{r5, r6}
	pop	{r1}
	bx	r1
	.4byte 0x0000080d
