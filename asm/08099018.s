@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08099018
	.thumb_func
Func_08099018:
	push	{lr}
	ldr	r3, [pc, #32]
	ldr	r2, [r3, #0]
	movs	r3, #7
	ands	r2, r3
	cmp	r2, #0
	bne.n	.L0
	movs	r1, #2
	bl	Func_08009240
	b.n	.L1
.L0:
	cmp	r2, #2
	bne.n	.L1
	movs	r1, #0
	bl	Func_08009240
.L1:
	pop	{r0}
	bx	r0
	.4byte 0x03001e40
