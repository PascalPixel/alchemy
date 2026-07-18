@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08019f98
	.thumb_func
Func_08019f98:
	push	{r5, r6, lr}
	mov	r6, sl
	mov	r5, r8
	push	{r5, r6}
	sub	sp, #4
	ldr	r5, [sp, #24]
	adds	r6, r1, #0
	mov	r8, r2
	mov	sl, r3
	bl	Func_08077080
	adds	r1, r6, #0
	ldrb	r0, [r0, #4]
	mov	r2, r8
	mov	r3, sl
	str	r5, [sp, #0]
	bl	Func_08019fcc
	add	sp, #4
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6}
	pop	{r0}
	bx	r0
