@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0808b824
	.thumb_func
Func_0808b824:
	push	{lr}
	ldr	r3, [pc, #40]
	ldr	r3, [r3, #0]
	adds	r1, r3, #0
	movs	r0, #7
	movs	r2, #8
	adds	r1, #52
.L1:
	ldmia	r1!, {r3}
	cmp	r3, #0
	beq.n	.L0
	adds	r0, r2, #0
.L0:
	adds	r2, #1
	cmp	r2, #65
	ble.n	.L1
	adds	r0, #1
	cmp	r0, #66
	bne.n	.L2
	movs	r0, #1
	negs	r0, r0
.L2:
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x03001ebc
