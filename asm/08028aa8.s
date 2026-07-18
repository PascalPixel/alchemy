@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08028aa8
	.thumb_func
Func_08028aa8:
	push	{r5, r6, lr}
	ldr	r3, [pc, #204]
	ldr	r6, [r3, #0]
	adds	r0, r6, #0
	adds	r5, r6, #0
	adds	r5, #140
	adds	r0, #150
	movs	r3, #0
	ldrsh	r2, [r0, r3]
	movs	r4, #0
	ldrsh	r3, [r5, r4]
	sub	sp, #4
	ldrh	r1, [r5, #0]
	cmp	r2, r3
	beq.n	.L0
	strh	r1, [r0, #0]
	movs	r3, #80
	ldr	r0, [r6, #124]
	movs	r2, #40
	str	r3, [sp, #0]
	movs	r1, #8
	movs	r3, #144
	bl	Func_080164d4
	movs	r2, #0
	ldrsh	r3, [r5, r2]
	cmp	r3, #1
	beq.n	.L1
	cmp	r3, #1
	bgt.n	.L2
	cmp	r3, #0
	bne.n	.L2
	ldr	r5, [pc, #144]
	ldr	r1, [r6, #124]
	adds	r0, r5, #0
	movs	r2, #18
	movs	r3, #40
	bl	Func_0801e7c0
	adds	r0, r5, #1
	ldr	r1, [r6, #124]
	movs	r2, #18
	movs	r3, #48
	bl	Func_0801e7c0
	adds	r0, r5, #2
	ldr	r1, [r6, #124]
	movs	r2, #18
	movs	r3, #56
	bl	Func_0801e7c0
	adds	r0, r5, #3
	ldr	r1, [r6, #124]
	movs	r2, #18
	movs	r3, #64
	adds	r5, #4
	bl	Func_0801e7c0
	ldr	r1, [r6, #124]
	adds	r0, r5, #0
	movs	r2, #18
	movs	r3, #72
	bl	Func_0801e7c0
	b.n	.L0
.L1:
	ldr	r5, [pc, #80]
	ldr	r1, [r6, #124]
	adds	r0, r5, #0
	movs	r2, #18
	movs	r3, #40
	bl	Func_0801e7c0
	adds	r0, r5, #1
	ldr	r1, [r6, #124]
	movs	r2, #18
	movs	r3, #48
	adds	r5, #2
	bl	Func_0801e7c0
	ldr	r1, [r6, #124]
	adds	r0, r5, #0
	movs	r2, #18
	movs	r3, #56
	bl	Func_0801e7c0
	b.n	.L0
.L2:
	ldr	r5, [pc, #36]
	ldr	r1, [r6, #124]
	adds	r0, r5, #0
	movs	r2, #18
	movs	r3, #40
	adds	r5, #1
	bl	Func_0801e7c0
	ldr	r1, [r6, #124]
	adds	r0, r5, #0
	movs	r2, #18
	movs	r3, #48
	bl	Func_0801e7c0
.L0:
	add	sp, #4
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	.4byte 0x03001f38
	.4byte 0x00000c7b
