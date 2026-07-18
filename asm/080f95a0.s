@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080f95a0
	.thumb_func
Func_080f95a0:
	push	{r5, r6, lr}
	ldr	r6, [pc, #28]
	movs	r5, #0
.L1:
	ldrb	r3, [r6, #0]
	cmp	r3, #0
	beq.n	.L0
	movs	r0, #1
	bl	Func_080030f8
	ldr	r3, [pc, #16]
	adds	r5, #1
	cmp	r5, r3
	ble.n	.L1
.L0:
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	.4byte 0x02003000
	.4byte 0x0000012b
