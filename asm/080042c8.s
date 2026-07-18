@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080042c8
	.thumb_func
Func_080042c8:
	push	{r5, r6, lr}
	movs	r5, #1
	ldr	r4, [pc, #52]
	negs	r5, r5
	ldr	r3, [pc, #52]
	ldrh	r2, [r3, #0]
	adds	r6, r2, #0
	strh	r3, [r3, #0]
	movs	r1, #0
	movs	r2, #1
.L2:
	cmp	r0, #0
	beq.n	.L0
	ldr	r3, [r4, #0]
	cmp	r3, r0
	bne.n	.L1
.L0:
	ldrb	r3, [r4, #5]
	orrs	r3, r2
	strb	r3, [r4, #5]
	adds	r5, r1, #0
.L1:
	adds	r1, #1
	adds	r4, #8
	cmp	r1, #19
	ble.n	.L2
	ldr	r3, [pc, #16]
	strh	r6, [r3, #0]
	adds	r0, r5, #0
	pop	{r5, r6}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x03001a20
	.4byte 0x04000208
