@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080783dc
	.thumb_func
Func_080783dc:
	push	{r5, r6, r7, lr}
	adds	r5, r1, #0
	adds	r7, r0, #0
	bl	Func_08077394
	adds	r6, r0, #0
	movs	r1, #58
	ldrsh	r3, [r6, r1]
	movs	r1, #54
	ldrsh	r2, [r6, r1]
	adds	r3, r3, r5
	adds	r1, r2, #0
	cmp	r3, r2
	bgt.n	.L0
	movs	r1, #0
	cmp	r3, #0
	blt.n	.L0
	adds	r1, r3, #0
.L0:
	adds	r0, r7, #0
	strh	r1, [r6, #58]
	bl	Func_0807822c
	movs	r2, #58
	ldrsh	r0, [r6, r2]
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
