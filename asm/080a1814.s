@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080a1814
	.thumb_func
Func_080a1814:
	push	{r5, r6, lr}
	mov	r6, r8
	push	{r6}
	adds	r5, r0, #0
	movs	r3, #0
	sub	sp, #8
	mov	r8, r3
	str	r3, [r5, #16]
	adds	r6, r5, #0
	movs	r3, #5
	str	r3, [sp, #0]
	adds	r6, #16
	movs	r3, #2
	str	r3, [sp, #4]
	adds	r0, r6, #0
	movs	r3, #13
	movs	r1, #0
	movs	r2, #0
	bl	Func_080a10d0
	ldr	r6, [r6, #0]
	movs	r1, #8
	negs	r1, r1
	adds	r0, r6, #0
	movs	r2, #11
	bl	Func_080a1778
	movs	r3, #13
	strb	r3, [r0, #5]
	movs	r3, #255
	strb	r3, [r5, #28]
	mov	r3, r8
	strb	r3, [r5, #29]
	movs	r3, #254
	str	r0, [r5, #20]
	strb	r3, [r0, #15]
	ldr	r2, [r5, #24]
	subs	r3, #255
	adds	r0, r6, #0
	strb	r3, [r2, #15]
	add	sp, #8
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6}
	pop	{r1}
	bx	r1
