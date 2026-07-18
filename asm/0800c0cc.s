@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0800c0cc
	.thumb_func
Func_0800c0cc:
	push	{lr}
	ldr	r3, [pc, #32]
	ldr	r2, [r3, #0]
	ldr	r3, [r2, #0]
	movs	r0, #0
	movs	r1, #0
	b.n	.L0
.L2:
	adds	r1, #1
	adds	r2, #112
	cmp	r1, #63
	bgt.n	.L1
	ldr	r3, [r2, #0]
.L0:
	cmp	r3, #0
	bne.n	.L2
	adds	r0, r2, #0
.L1:
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x03001e64
