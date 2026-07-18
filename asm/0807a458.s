@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0807a458
	.thumb_func
Func_0807a458:
	push	{r5, r6, r7, lr}
	adds	r5, r0, #0
	adds	r6, r1, #0
	adds	r7, r2, #0
	bl	Func_0807a3a8
	movs	r0, #0
	cmp	r5, #7
	bls.n	.L0
	movs	r0, #1
.L0:
	bl	Func_08077330
	movs	r1, #132
	adds	r3, r0, #0
	lsls	r1, r1, #1
	adds	r0, r3, r1
	ldr	r1, [r0, #0]
	adds	r2, r3, #0
	adds	r2, #8
	lsls	r3, r1, #2
	strb	r6, [r2, r3]
	adds	r1, #1
	adds	r2, r2, r3
	movs	r3, #255
	strb	r7, [r2, #1]
	strb	r5, [r2, #2]
	strb	r3, [r2, #3]
	str	r1, [r0, #0]
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
