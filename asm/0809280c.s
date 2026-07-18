@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0809280c
	.thumb_func
Func_0809280c:
	push	{r5, r6, r7, lr}
	adds	r5, r1, #0
	adds	r7, r2, #0
	bl	Func_0808ba1c
	adds	r6, r0, #0
	adds	r0, r5, #0
	bl	Func_0808ba1c
	adds	r2, r0, #0
	cmp	r6, #0
	beq.n	.L0
	cmp	r2, #0
	beq.n	.L0
	ldr	r3, [r6, #16]
	ldr	r0, [r2, #16]
	ldr	r1, [r2, #8]
	subs	r0, r0, r3
	ldr	r3, [r6, #8]
	subs	r1, r1, r3
	bl	Func_080044d0
	strh	r0, [r6, #6]
	adds	r0, r7, #0
	bl	Func_0809163c
.L0:
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
