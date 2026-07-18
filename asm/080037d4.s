@ 呼出しグラフで未到達だったコード間隙関数の再構築サム逆アセンブル。
@ 探索では未到達。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080037d4
	.thumb_func
Func_080037d4:
	push	{lr}
	cmp	r0, #4
	bls.n	.L0
	movs	r0, #0
.L0:
	movs	r3, #152
	adds	r2, r0, #0
	muls	r2, r3
	ldr	r0, [pc, #20]
	ldr	r3, [pc, #20]
	adds	r0, r2, r0
	ldr	r1, [pc, #20]
	ldr	r2, [pc, #24]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r0, #0
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x08000404
	.4byte 0x040000d4
	.4byte 0x03000bd8
	.4byte 0x84000026
	bx	lr
