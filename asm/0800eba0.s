@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0800eba0
	.thumb_func
Func_0800eba0:
	push	{r5, r6, lr}
	ldmia	r0!, {r4}
	ldmia	r2!, {r5}
	subs	r4, r4, r5
	asrs	r6, r4, #16
	ldmia	r2!, {r5}
	ldmia	r0!, {r4}
	ldr	r2, [r2, #0]
	ldr	r0, [r0, #0]
	adds	r1, r1, r3
	movs	r3, #128
	subs	r4, r4, r5
	subs	r0, r0, r2
	lsls	r3, r3, #15
	asrs	r4, r4, #16
	asrs	r0, r0, #16
	cmp	r6, r3
	bgt.n	.L0
	cmp	r0, r3
	bgt.n	.L0
	adds	r2, r4, #0
	muls	r2, r4
	adds	r3, r6, #0
	muls	r3, r6
	adds	r3, r3, r2
	adds	r2, r0, #0
	muls	r2, r0
	adds	r3, r3, r2
	adds	r2, r1, #0
	muls	r2, r1
	movs	r0, #0
	cmp	r3, r2
	blt.n	.L1
.L0:
	movs	r0, #1
	negs	r0, r0
.L1:
	pop	{r5, r6}
	pop	{r1}
	bx	r1
