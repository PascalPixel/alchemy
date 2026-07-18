@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08079754
	.thumb_func
Func_08079754:
	push	{lr}
	ldr	r3, [pc, #32]
	movs	r2, #142
	lsls	r2, r2, #1
	adds	r3, r3, r2
	movs	r2, #0
	ldrsb	r2, [r3, r2]
	adds	r2, r2, r0
	cmp	r2, #28
	ble.n	.L0
	movs	r2, #28
.L0:
	cmp	r2, #0
	bge.n	.L1
	movs	r2, #0
.L1:
	strb	r2, [r3, #0]
	adds	r0, r2, #0
	pop	{r1}
	bx	r1
	.4byte 0x02000240
