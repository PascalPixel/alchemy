@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080e38b8
	.thumb_func
Func_080e38b8:
	push	{r5, r6, lr}
	adds	r4, r0, #0
	ldr	r3, [r4, #0]
	ldr	r0, [r4, #12]
	adds	r3, r3, r0
	adds	r5, r1, #0
	str	r3, [r4, #0]
	ldr	r1, [r4, #16]
	ldr	r3, [r4, #4]
	adds	r3, r3, r1
	str	r3, [r4, #4]
	ldr	r6, [r4, #20]
	ldr	r3, [r4, #8]
	adds	r3, r3, r6
	str	r3, [r4, #8]
	adds	r3, r5, #0
	muls	r3, r0
	adds	r1, r1, r2
	str	r1, [r4, #16]
	cmp	r3, #0
	bge.n	.L0
	adds	r3, #63
.L0:
	muls	r1, r5
	asrs	r3, r3, #6
	str	r3, [r4, #12]
	cmp	r1, #0
	bge.n	.L1
	adds	r1, #63
.L1:
	asrs	r3, r1, #6
	adds	r1, r5, #0
	muls	r1, r6
	str	r3, [r4, #16]
	cmp	r1, #0
	bge.n	.L2
	adds	r1, #63
.L2:
	asrs	r3, r1, #6
	str	r3, [r4, #20]
	pop	{r5, r6}
	pop	{r0}
	bx	r0
