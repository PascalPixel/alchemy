@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080f9ef8
	.thumb_func
Func_080f9ef8:
	push	{r4, r5, r6, lr}
	adds	r5, r1, #0
	ldrb	r1, [r5, #0]
	movs	r0, #128
	tst	r0, r1
	beq.n	.L0
	ldr	r4, [r5, #32]
	cmp	r4, #0
	beq.n	.L1
	movs	r6, #0
.L4:
	ldrb	r0, [r4, #0]
	cmp	r0, #0
	beq.n	.L2
	ldrb	r0, [r4, #1]
	movs	r3, #7
	ands	r0, r3
	beq.n	.L3
	ldr	r3, [pc, #28]
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #44]
	bl	Func_080f9ee8
.L3:
	strb	r6, [r4, #0]
.L2:
	str	r6, [r4, #44]
	ldr	r4, [r4, #52]
	cmp	r4, #0
	bne.n	.L4
.L1:
	str	r4, [r5, #32]
.L0:
	pop	{r4, r5, r6}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03007ff0
