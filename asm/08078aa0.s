@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08078aa0
	.thumb_func
Func_08078aa0:
	push	{lr}
	adds	r2, r0, #0
	ldr	r4, [pc, #36]
	movs	r0, #0
	cmp	r2, #127
	bgt.n	.L0
	ldrb	r3, [r4, r2]
	adds	r3, r3, r1
	cmp	r3, #0
	bge.n	.L1
	movs	r3, #0
	b.n	.L2
.L1:
	cmp	r3, #99
	ble.n	.L3
	movs	r3, #99
	movs	r0, #99
	b.n	.L2
.L3:
	adds	r0, r3, #0
.L2:
	strb	r3, [r4, r2]
.L0:
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x02000380
