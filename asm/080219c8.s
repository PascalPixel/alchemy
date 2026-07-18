@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080219c8
	.thumb_func
Func_080219c8:
	push	{r5, r6, r7, lr}
	ldr	r3, [pc, #60]
	ldr	r3, [r3, #0]
	lsrs	r6, r3, #2
	movs	r3, #3
	ands	r6, r3
	adds	r7, r0, #0
	cmp	r6, #2
	ble.n	.L0
	movs	r6, #2
.L0:
	cmp	r6, #0
	bgt.n	.L1
	movs	r6, #1
.L1:
	ldr	r5, [pc, #40]
	adds	r6, #1
	negs	r3, r6
	adds	r1, r5, #0
	adds	r2, r7, #0
	ldr	r0, [pc, #32]
	bl	Func_08021950
	adds	r5, #32
	adds	r2, r7, #0
	ldr	r0, [pc, #28]
	adds	r2, #32
	adds	r1, r5, #0
	adds	r3, r6, #0
	bl	Func_08021950
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001e40
	.4byte 0x08037280
	.4byte 0x06000220
	.4byte 0x06000240
