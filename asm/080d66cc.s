@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080d66cc
	.thumb_func
Func_080d66cc:
	push	{r5, r6, lr}
	ldr	r6, [pc, #44]
	ldr	r5, [pc, #36]
	ldr	r1, [pc, #44]
	ldr	r4, [pc, #44]
	movs	r0, #0
.L4:
	adds	r3, r0, #0
	subs	r3, #8
	cmp	r3, #127
	bhi.n	.L0
	ldrh	r2, [r6, #0]
	ldrb	r3, [r4, #0]
	subs	r2, r2, r3
	cmp	r2, #0
	bge.n	.L1
	movs	r2, #0
.L1:
	cmp	r2, #240
	ble.n	.L2
	movs	r2, #240
.L2:
	strh	r2, [r1, #0]
	b.n	.L3
	movs	r0, r0
	.4byte 0x0000fff1
	.4byte 0x02010000
	.4byte 0x02010082
	.4byte 0x0200fffa
.L0:
	strh	r5, [r1, #0]
.L3:
	adds	r0, #1
	adds	r1, #2
	adds	r4, #1
	cmp	r0, #160
	bne.n	.L4
	ldr	r3, [pc, #32]
	ldr	r2, [pc, #36]
	ldrh	r1, [r3, #10]
	ands	r2, r1
	strh	r2, [r3, #10]
	ldr	r2, [pc, #32]
	ldrh	r1, [r3, #10]
	ands	r2, r1
	strh	r2, [r3, #10]
	ldr	r0, [pc, #28]
	ldrh	r2, [r3, #10]
	ldr	r1, [pc, #28]
	ldr	r2, [pc, #28]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	.4byte 0x040000b0
	.4byte 0x0000c5ff
	.4byte 0x00007fff
	.4byte 0x02010082
	.4byte 0x04000040
	.4byte 0xa2600001
