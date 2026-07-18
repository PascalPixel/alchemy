@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080b6c90
	.thumb_func
Func_080b6c90:
	push	{r5, lr}
	sub	sp, #28
	mov	r5, sp
	adds	r1, r5, #0
	movs	r0, #3
	bl	Func_080b6c08
	adds	r0, r5, #0
	movs	r1, #1
	bl	Func_080b7b6c
	add	sp, #28
	pop	{r5}
	pop	{r0}
	bx	r0
