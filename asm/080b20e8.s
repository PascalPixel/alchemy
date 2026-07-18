@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080b20e8
	.thumb_func
Func_080b20e8:
	push	{r5, lr}
	adds	r5, r0, #0
	bl	Func_08077018
	movs	r3, #0
	ldrsh	r0, [r0, r3]
	cmp	r0, #0
	bge.n	.L0
	adds	r0, #3
.L0:
	movs	r3, #128
	lsls	r3, r3, #3
	ands	r3, r5
	asrs	r0, r0, #2
	cmp	r3, #0
	bne.n	.L1
	movs	r0, #0
.L1:
	pop	{r5}
	pop	{r1}
	bx	r1
