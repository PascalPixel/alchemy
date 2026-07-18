@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0820514e
	.thumb_func
Func_0820514e:
	push	{r0, r1, r2, r3, r4, r5, r6, r7, lr}
	cmp	r2, #23
	subs	r7, r2, #4
	ldrsh	r2, [r3, r4]
	subs	r3, r7, #3
	udf	#163
	movs	r6, #39
	str	r7, [r2, #96]
	asrs	r1, r5, #11
	adds	r7, r7, r3
	asrs	r6, r2, #30
	pop	{r2, r3, r4, r5, r6, pc}
