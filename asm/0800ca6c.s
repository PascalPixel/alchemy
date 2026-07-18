@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0800ca6c
	.thumb_func
Func_0800ca6c:
	push	{r5, r6, lr}
	adds	r6, r0, #0
	movs	r5, #0
	b.n	.L0
.L2:
	movs	r0, #1
	bl	Func_080030f8
	adds	r5, #1
.L0:
	ldr	r3, [pc, #20]
	cmp	r5, r3
	bgt.n	.L1
	adds	r0, r6, #0
	bl	Func_0800ca98
	cmp	r0, #0
	beq.n	.L2
.L1:
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x00000257
