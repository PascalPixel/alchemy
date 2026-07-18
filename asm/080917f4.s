@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080917f4
	.thumb_func
Func_080917f4:
	push	{r5, r6, lr}
	adds	r6, r1, #0
	adds	r5, r0, #0
	bl	Func_08077150
	adds	r0, r6, #0
	bl	Func_08077150
	adds	r0, r5, #0
	adds	r1, r6, #0
	bl	Func_08015428
	pop	{r5, r6}
	pop	{r0}
	bx	r0
