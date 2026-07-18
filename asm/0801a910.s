@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0801a910
	.thumb_func
Func_0801a910:
	push	{lr}
	ldr	r3, [pc, #80]
	ldr	r4, [r3, #0]
	cmp	r0, #0
	beq.n	.L0
	movs	r3, #239
	lsls	r3, r3, #1
	movs	r1, #0
	adds	r2, r4, r3
	movs	r0, #0
.L3:
	ldrh	r3, [r2, #0]
	cmp	r3, #0
	bne.n	.L1
	movs	r3, #234
	adds	r0, r4, r0
	lsls	r3, r3, #1
	adds	r0, r0, r3
	b.n	.L2
.L1:
	adds	r1, #1
	adds	r2, #52
	adds	r0, #52
	cmp	r1, #5
	bne.n	.L3
	b.n	.L4
.L0:
	adds	r2, r4, #0
	adds	r0, r4, #0
	movs	r1, #0
	adds	r2, #104
	adds	r0, #114
.L6:
	ldrh	r3, [r0, #0]
	adds	r0, #52
	cmp	r3, #0
	bne.n	.L5
	adds	r0, r2, #0
	b.n	.L2
.L5:
	adds	r1, #1
	adds	r2, #52
	cmp	r1, #7
	bne.n	.L6
.L4:
	movs	r0, #0
.L2:
	pop	{r1}
	bx	r1
	.4byte 0x03001e98
