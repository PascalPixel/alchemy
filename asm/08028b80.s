@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08028b80
	.thumb_func
Func_08028b80:
	push	{r5, r6, lr}
	ldr	r3, [pc, #116]
	ldr	r6, [r3, #0]
	adds	r0, r6, #0
	adds	r5, r6, #0
	adds	r5, #140
	adds	r0, #150
	movs	r3, #0
	ldrsh	r2, [r0, r3]
	movs	r4, #0
	ldrsh	r3, [r5, r4]
	ldrh	r1, [r5, #0]
	cmp	r2, r3
	beq.n	.L0
	strh	r1, [r0, #0]
	ldr	r0, [r6, #124]
	bl	Func_08016478
	movs	r2, #0
	ldrsh	r3, [r5, r2]
	cmp	r3, #0
	bne.n	.L1
	ldr	r5, [pc, #76]
	ldr	r1, [r6, #124]
	adds	r0, r5, #0
	movs	r2, #16
	movs	r3, #4
	adds	r5, #1
	bl	Func_0801e74c
	ldr	r1, [r6, #124]
	adds	r0, r5, #0
	movs	r2, #16
	movs	r3, #16
	bl	Func_0801e74c
	b.n	.L0
.L1:
	ldr	r5, [pc, #52]
	ldr	r1, [r6, #124]
	adds	r0, r5, #0
	movs	r2, #0
	movs	r3, #4
	bl	Func_0801e74c
	adds	r0, r5, #1
	ldr	r1, [r6, #124]
	movs	r2, #0
	movs	r3, #16
	adds	r5, #2
	bl	Func_0801e74c
	ldr	r1, [r6, #124]
	adds	r0, r5, #0
	movs	r2, #0
	movs	r3, #28
	bl	Func_0801e74c
.L0:
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	.4byte 0x03001f38
	.4byte 0x00000c71
	.4byte 0x00000c73
