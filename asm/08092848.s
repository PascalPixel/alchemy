@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08092848
	.thumb_func
Func_08092848:
	push	{r5, r6, r7, lr}
	adds	r5, r1, #0
	adds	r7, r2, #0
	bl	Func_0808ba1c
	adds	r6, r0, #0
	adds	r0, r5, #0
	bl	Func_0808ba1c
	adds	r1, r0, #0
	cmp	r6, #0
	beq.n	.L0
	cmp	r1, #0
	beq.n	.L0
	adds	r0, r6, #0
	bl	Func_08092878
	adds	r0, r7, #0
	bl	Func_0809163c
.L0:
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
