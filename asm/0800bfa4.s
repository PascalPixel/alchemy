@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0800bfa4
	.thumb_func
Func_0800bfa4:
	push	{r5, lr}
	ldr	r3, [pc, #76]
	ldr	r3, [r3, #0]
	adds	r3, #228
	ldr	r4, [pc, #72]
	adds	r5, r1, #0
	ldr	r1, [r3, #0]
	ldr	r2, [r3, #4]
	ldr	r3, [r0, #8]
	ands	r1, r4
	subs	r1, r3, r1
	ldr	r3, [r0, #16]
	ldr	r0, [pc, #60]
	ands	r2, r4
	subs	r2, r3, r2
	adds	r3, r1, r0
	ldr	r0, [pc, #56]
	cmp	r3, r0
	bhi.n	.L0
	cmp	r2, #0
	ble.n	.L0
	movs	r3, #224
	lsls	r3, r3, #16
	cmp	r2, r3
	bge.n	.L0
	asrs	r3, r1, #16
	stmia	r5!, {r3}
	asrs	r3, r2, #16
	str	r3, [r5, #0]
	movs	r0, #0
	b.n	.L1
.L0:
	movs	r3, #0
	stmia	r5!, {r3}
	movs	r0, #1
	str	r3, [r5, #0]
	negs	r0, r0
.L1:
	pop	{r5}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x03001e70
	.4byte 0xffff0000
	.4byte 0x001fffff
	.4byte 0x012ffffe
