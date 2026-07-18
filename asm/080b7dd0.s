@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080b7dd0
	.thumb_func
Func_080b7dd0:
	push	{lr}
	ldr	r3, [pc, #44]
	ldr	r2, [r3, #0]
	adds	r1, r2, #0
	adds	r1, #116
	cmp	r0, #7
	ble.n	.L0
	subs	r0, #120
.L0:
	movs	r3, #183
	lsls	r3, r3, #2
	adds	r0, r0, r3
	ldrb	r3, [r2, r0]
	cmp	r3, #255
	bne.n	.L1
	movs	r0, #0
	b.n	.L2
.L1:
	ldrb	r2, [r2, r0]
	movs	r3, #44
	adds	r0, r2, #0
	muls	r0, r3
	adds	r0, r1, r0
.L2:
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x03001e74
