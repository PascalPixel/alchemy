@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08079e9c
	.thumb_func
Func_08079e9c:
	push	{r5, lr}
	adds	r5, r1, #0
	ldr	r1, [pc, #80]
	adds	r2, r0, #0
	adds	r3, r2, r1
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L0
	subs	r1, #1
	adds	r3, r2, r1
	ldrb	r0, [r3, #0]
	bl	Func_080773d8
	movs	r2, #0
	adds	r0, #72
.L2:
	ldrb	r3, [r0, #0]
	cmp	r3, r5
	beq.n	.L1
	adds	r2, #1
	adds	r0, #1
	cmp	r2, #2
	ble.n	.L2
	b.n	.L3
.L0:
	ldr	r1, [pc, #40]
	adds	r3, r2, r1
	ldrb	r0, [r3, #0]
	bl	Func_08079ad8
	movs	r2, #0
	adds	r0, #80
.L6:
	ldrb	r3, [r0, #0]
	adds	r0, #1
	cmp	r3, r5
	bne.n	.L4
.L1:
	movs	r0, #1
	b.n	.L5
.L4:
	adds	r2, #1
	cmp	r2, #2
	ble.n	.L6
.L3:
	movs	r0, #0
.L5:
	pop	{r5}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x00000129
