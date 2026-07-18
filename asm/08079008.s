@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08079008
	.thumb_func
Func_08079008:
	push	{r5, lr}
	adds	r5, r1, #0
	bl	Func_08077394
	ldr	r1, [pc, #64]
	adds	r2, r0, #0
	adds	r3, r2, r1
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L0
	movs	r0, #0
	cmp	r5, #0
	ble.n	.L1
	cmp	r5, #99
	bgt.n	.L0
	movs	r3, #148
	lsls	r3, r3, #1
	adds	r0, r2, r3
	ldrb	r3, [r0, #0]
	cmp	r3, #7
	bhi.n	.L0
	adds	r2, r3, #0
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r2, r3, #5
	adds	r3, r3, r2
	adds	r3, r3, r5
	ldr	r1, [pc, #24]
	lsls	r3, r3, #2
	subs	r3, #4
	ldr	r0, [r1, r3]
	b.n	.L1
.L0:
	movs	r0, #1
	negs	r0, r0
.L1:
	pop	{r5}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x00000129
	.4byte 0x0807a830
