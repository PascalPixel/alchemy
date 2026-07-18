@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0801cf48
	.thumb_func
Func_0801cf48:
	push	{r5, r6, lr}
	ldr	r3, [pc, #164]
	ldr	r1, [pc, #164]
	ldr	r5, [r3, #0]
	ldr	r2, [pc, #164]
	adds	r3, r5, r1
	adds	r0, r5, r2
	ldrh	r6, [r3, #0]
	bl	Func_080b0020
	ldr	r3, [pc, #156]
	adds	r0, r5, r3
	bl	Func_080b0028
	ldr	r1, [pc, #152]
	adds	r0, r5, r1
	bl	Func_080b0028
	cmp	r6, #0
	bne.n	.L0
	ldr	r2, [pc, #144]
	adds	r3, r5, r2
	ldrh	r2, [r3, #0]
	movs	r3, #7
	ands	r3, r2
	ldr	r2, [pc, #140]
	adds	r0, r3, r2
	b.n	.L1
.L0:
	ldr	r0, [pc, #132]
.L1:
	bl	Func_08002f40
	ldr	r1, [pc, #116]
	adds	r3, r5, r1
	ldr	r3, [r3, #0]
	movs	r1, #128
	adds	r2, r0, #0
	lsls	r1, r1, #1
	ldrb	r0, [r3, #14]
	bl	Func_08003fa4
	cmp	r6, #1
	bne.n	.L2
	ldr	r2, [pc, #100]
	adds	r3, r5, r2
	ldrh	r2, [r3, #0]
	movs	r3, #7
	ands	r3, r2
	ldr	r2, [pc, #96]
	adds	r0, r3, r2
	b.n	.L3
.L2:
	ldr	r0, [pc, #88]
.L3:
	bl	Func_08002f40
	ldr	r1, [pc, #76]
	adds	r3, r5, r1
	ldr	r3, [r3, #0]
	movs	r1, #128
	adds	r2, r0, #0
	lsls	r1, r1, #1
	ldrb	r0, [r3, #14]
	bl	Func_08003fa4
	cmp	r6, #1
	ble.n	.L4
	ldr	r1, [pc, #64]
	adds	r2, r6, r1
	ldrsb	r2, [r5, r2]
	lsls	r3, r6, #1
	adds	r3, r3, r6
	adds	r3, r3, r2
	ldr	r2, [pc, #56]
	lsls	r3, r3, #2
	adds	r3, r3, r2
	ldr	r0, [r5, r3]
	bl	Func_080217a4
.L4:
	ldr	r3, [pc, #32]
	adds	r2, r5, r3
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	.4byte 0x03001ea0
	.4byte 0x00000574
	.4byte 0x000005a4
	.4byte 0x000005b4
	.4byte 0x000005c4
	.4byte 0x0000057c
	.4byte 0x000000e8
	.4byte 0x00000594
	.4byte 0x000005d4
