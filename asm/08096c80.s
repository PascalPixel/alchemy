@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08096c80
	.thumb_func
Func_08096c80:
	push	{r5, r6, lr}
	ldr	r4, [pc, #84]
	ldr	r6, [r4, #0]
	bl	Func_080090c8
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L0
	adds	r3, r5, #0
	adds	r3, #84
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L1
	bl	Func_080090d0
	movs	r0, #0
	b.n	.L2
.L1:
	ldr	r3, [r6, #16]
	ldr	r3, [r3, #20]
	adds	r2, r5, #0
	str	r3, [r5, #20]
	adds	r2, #85
	movs	r3, #4
	strb	r3, [r2, #0]
	subs	r2, #50
	strb	r3, [r2, #0]
	ldr	r1, [r5, #80]
	ldrb	r2, [r1, #9]
	subs	r3, #17
	ands	r3, r2
	strb	r3, [r1, #9]
	adds	r0, r5, #0
	movs	r1, #0
	bl	Func_080091e0
	adds	r0, r5, #0
	movs	r1, #1
	bl	Func_08009080
.L0:
	adds	r0, r5, #0
.L2:
	pop	{r5, r6}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x03001f30
