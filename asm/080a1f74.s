@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080a1f74
	.thumb_func
Func_080a1f74:
	push	{lr}
	ldr	r2, [pc, #76]
	cmp	r0, #1
	beq.n	.L0
	cmp	r0, #1
	bgt.n	.L1
	cmp	r0, #0
	beq.n	.L2
	b.n	.L3
.L1:
	cmp	r0, #2
	beq.n	.L4
	b.n	.L3
.L2:
	ldr	r2, [pc, #56]
	b.n	.L3
.L0:
	ldr	r2, [pc, #56]
	b.n	.L3
.L4:
	ldr	r2, [pc, #56]
.L3:
	ldrb	r3, [r2, #0]
	movs	r4, #255
	strb	r3, [r1, #0]
	lsls	r4, r4, #24
	lsls	r3, r3, #24
	movs	r0, #0
	cmp	r3, r4
	beq.n	.L5
.L6:
	adds	r0, #1
	cmp	r0, #31
	bgt.n	.L5
	adds	r2, #1
	ldrb	r3, [r2, #0]
	adds	r1, #1
	movs	r4, #255
	strb	r3, [r1, #0]
	lsls	r4, r4, #24
	lsls	r3, r3, #24
	cmp	r3, r4
	bne.n	.L6
.L5:
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x080af2a6
	.4byte 0x080af2d0
	.4byte 0x080af2bc
	.4byte 0x080af2b1
