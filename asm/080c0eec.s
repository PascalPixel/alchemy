@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080c0eec
	.thumb_func
Func_080c0eec:
	push	{r5, lr}
	ldr	r1, [pc, #144]
	ldr	r3, [r1, #0]
	movs	r2, #8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L0
	ldr	r5, [pc, #136]
.L6:
	ldr	r3, [pc, #136]
	ldr	r1, [r3, #0]
	ldr	r3, [r5, #0]
	movs	r2, #32
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L1
	ldr	r3, [pc, #128]
	adds	r2, r1, r3
	ldr	r3, [r2, #0]
	subs	r3, #1
	str	r3, [r2, #0]
.L1:
	ldr	r3, [r5, #0]
	movs	r2, #16
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L2
	ldr	r3, [pc, #108]
	adds	r2, r1, r3
	ldr	r3, [r2, #0]
	adds	r3, #1
	str	r3, [r2, #0]
.L2:
	ldr	r3, [r5, #0]
	movs	r2, #64
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L3
	ldr	r3, [pc, #88]
	adds	r2, r1, r3
	ldr	r3, [r2, #0]
	subs	r3, #100
	str	r3, [r2, #0]
.L3:
	ldr	r3, [r5, #0]
	movs	r2, #128
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L4
	ldr	r3, [pc, #68]
	adds	r2, r1, r3
	ldr	r3, [r2, #0]
	adds	r3, #100
	str	r3, [r2, #0]
.L4:
	ldr	r3, [pc, #60]
	ldr	r3, [r3, #0]
	movs	r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L5
	ldr	r2, [pc, #44]
	adds	r3, r1, r2
	ldr	r0, [r3, #0]
	ldr	r1, [pc, #28]
	b.n	.L0
.L5:
	movs	r0, #1
	bl	Func_080030f8
	b.n	.L6
.L0:
	ldr	r3, [r1, #0]
	movs	r2, #4
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L7
	ldr	r0, [pc, #24]
.L7:
	pop	{r5}
	pop	{r1}
	bx	r1
	.4byte 0x03001ae8
	.4byte 0x03001b04
	.4byte 0x03001e74
	.4byte 0x00000828
	.4byte 0x03001c94
	.4byte 0x0000018f
