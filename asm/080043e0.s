@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080043e0
	.thumb_func
Func_080043e0:
	push	{r5, r6, lr}
	movs	r0, #1
	ldr	r4, [pc, #48]
	negs	r0, r0
	ldr	r3, [pc, #48]
	ldrh	r2, [r3, #0]
	adds	r6, r2, #0
	strh	r3, [r3, #0]
	movs	r1, #0
	movs	r5, #254
.L1:
	ldrb	r3, [r4, #3]
	cmp	r3, #2
	bne.n	.L0
	ldrb	r2, [r4, #5]
	adds	r3, r5, #0
	ands	r3, r2
	strb	r3, [r4, #5]
	adds	r0, r1, #0
.L0:
	adds	r1, #1
	adds	r4, #8
	cmp	r1, #19
	ble.n	.L1
	ldr	r3, [pc, #12]
	strh	r6, [r3, #0]
	pop	{r5, r6}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x03001a20
	.4byte 0x04000208
