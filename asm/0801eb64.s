@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0801eb64
	.thumb_func
Func_0801eb64:
	push	{r5, r6, lr}
	sub	sp, #4
	adds	r5, r2, #0
	adds	r6, r3, #0
	bl	Func_0801bc34
	cmp	r0, #0
	bge.n	.L0
	movs	r0, #0
	b.n	.L1
.L0:
	ldr	r3, [sp, #16]
	movs	r1, #128
	str	r3, [sp, #0]
	lsls	r1, r1, #23
	adds	r2, r5, #0
	adds	r3, r6, #0
	bl	Func_0801eadc
.L1:
	add	sp, #4
	pop	{r5, r6}
	pop	{r1}
	bx	r1
