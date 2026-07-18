@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0801e9d4
	.thumb_func
Func_0801e9d4:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	sub	sp, #16
	adds	r4, r0, #0
	ldr	r7, [sp, #36]
	adds	r5, r1, #0
	mov	r8, r3
	adds	r1, r4, #0
	adds	r6, r2, #0
	mov	r0, sp
	adds	r2, r5, #0
	bl	Func_08017dd4
	adds	r1, r6, #0
	mov	r2, r8
	adds	r3, r7, #0
	bl	Func_0801e8b0
	add	sp, #16
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
