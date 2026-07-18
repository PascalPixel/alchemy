@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080c0098
	.thumb_func
Func_080c0098:
	push	{lr}
	ldr	r2, [pc, #48]
	ldr	r1, [pc, #48]
	movs	r3, #0
.L0:
	adds	r3, #1
	stmia	r0!, {r2}
	adds	r2, r2, r1
	cmp	r3, #63
	bls.n	.L0
	ldr	r2, [pc, #32]
	ldr	r1, [pc, #32]
	movs	r3, #0
.L1:
	adds	r3, #1
	stmia	r0!, {r2}
	adds	r2, r2, r1
	cmp	r3, #55
	bls.n	.L1
	movs	r1, #136
	movs	r2, #1
	ldr	r3, [pc, #20]
	lsls	r1, r1, #2
	negs	r2, r2
	bl	Func_080072f0
	pop	{r0}
	bx	r0
	.4byte 0x03020100
	.4byte 0x04040404
	.4byte 0x03000168
