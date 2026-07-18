@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08021e48
	.thumb_func
Func_08021e48:
	push	{r5, lr}
	movs	r3, #1
	bl	Func_08017658
	adds	r5, r0, #0
	b.n	.L0
.L1:
	movs	r0, #1
	bl	Func_080030f8
.L0:
	bl	Func_08017364
	cmp	r0, #0
	beq.n	.L1
	adds	r0, r5, #0
	pop	{r5}
	pop	{r1}
	bx	r1
