@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0819a56a
	.thumb_func
Func_0819a56a:
	push	{r6, r7, lr}
	pop	{r1, r2, r4}
	stmia	r0!, {r1, r2, r4}
	adds	r2, #18
	asrs	r7, r3, #27
	ldrb	r2, [r6, #26]
	adds	r5, #18
	orrs	r1, r4
	asrs	r7, r7, #21
	pop	{r0, r1, r6, r7, pc}
