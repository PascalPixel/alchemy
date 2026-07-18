@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0800d6d8
	.thumb_func
Func_0800d6d8:
	push	{lr}
	adds	r2, r0, #0
	adds	r2, #94
	movs	r3, #0
	strh	r3, [r2, #0]
	cmp	r1, #0
	bne.n	.L0
	b.n	.L1
.L3:
	adds	r0, #1
	b.n	.L2
.L0:
	ldr	r3, [pc, #24]
	ldr	r4, [pc, #28]
	ldr	r2, [r0, #0]
	ands	r1, r3
	movs	r0, #0
.L4:
	ldmia	r2!, {r3}
	cmp	r3, r1
	beq.n	.L3
	adds	r0, #1
	cmp	r0, r4
	ble.n	.L4
.L1:
	movs	r0, #0
.L2:
	pop	{r1}
	bx	r1
	.4byte 0xbfffffff
	.4byte 0x000003ff
