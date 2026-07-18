@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0801a32c
	.thumb_func
Func_0801a32c:
	push	{lr}
	adds	r3, r2, #0
	cmp	r0, #1
	beq.n	.L0
	cmp	r0, #1
	bcc.n	.L1
	cmp	r0, #2
	beq.n	.L2
	cmp	r0, #3
	beq.n	.L3
	b.n	.L1
.L0:
	ldr	r2, [pc, #28]
	b.n	.L4
.L2:
	ldr	r2, [pc, #28]
	b.n	.L4
.L3:
	ldr	r2, [pc, #28]
	b.n	.L4
.L1:
	ldr	r2, [pc, #28]
.L4:
	adds	r0, r3, #0
	movs	r1, #32
	bl	Func_08003fa4
	movs	r0, #1
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x08031864
	.4byte 0x08031864
	.4byte 0x08031864
	.4byte 0x08031864
