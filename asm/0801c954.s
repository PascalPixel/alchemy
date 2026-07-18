@ 呼出しグラフで未到達だったコード間隙関数の再構築サム逆アセンブル。
@ 探索では未到達。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0801c954
	.thumb_func
Func_0801c954:
	push	{r5, r6, lr}
	ldr	r3, [pc, #88]
	ldr	r2, [pc, #88]
	ldr	r5, [r3, #0]
	adds	r3, r5, r2
	ldr	r0, [r3, #0]
	movs	r1, #0
	bl	Func_08016418
	ldr	r3, [pc, #76]
	adds	r6, r5, r3
	b.n	.L0
.L1:
	movs	r0, #1
	bl	Func_080030f8
.L0:
	ldr	r0, [r6, #0]
	bl	Func_08017394
	cmp	r0, #0
	beq.n	.L1
	adds	r3, r5, #0
	adds	r3, #70
	ldrh	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L2
	adds	r3, r5, #0
	adds	r3, #72
	ldrh	r0, [r3, #0]
	bl	Func_08003f3c
.L2:
	ldr	r2, [pc, #36]
	adds	r3, r5, r2
	ldrh	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L3
	adds	r2, #2
	adds	r3, r5, r2
	ldrh	r0, [r3, #0]
	bl	Func_08003f3c
.L3:
	movs	r0, #19
	bl	Func_08002dd8
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	.4byte 0x03001e9c
	.4byte 0x00000ff4
	.4byte 0x00000352
	bx	lr
