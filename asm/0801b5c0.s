@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0801b5c0
	.thumb_func
Func_0801b5c0:
	push	{r5, r6, r7, lr}
	movs	r1, #231
	adds	r6, r0, #0
	lsls	r1, r1, #2
	adds	r7, r6, r1
	ldr	r3, [r7, #0]
	cmp	r3, #0
	beq.n	.L0
	ldr	r2, [pc, #132]
	adds	r5, r6, r2
	ldrh	r1, [r5, #0]
	bl	Func_0801b9a8
	ldr	r3, [pc, #128]
	adds	r2, r6, r3
	movs	r3, #33
	strh	r3, [r2, #0]
	movs	r0, #1
	bl	Func_080030f8
	ldrh	r5, [r5, #0]
	cmp	r5, #1
	bne.n	.L1
	ldrh	r3, [r7, #0]
	cmp	r3, #0
	beq.n	.L1
	movs	r3, #8
	strh	r3, [r6, #8]
	ldrh	r3, [r7, #0]
	ldr	r1, [pc, #100]
	adds	r3, r3, r1
	strh	r3, [r7, #0]
	adds	r0, r6, #0
	movs	r1, #0
	bl	Func_0801ba68
	ldrh	r3, [r7, #0]
	cmp	r3, #0
	bne.n	.L2
	strh	r3, [r6, #10]
.L2:
	strh	r5, [r6, #62]
	b.n	.L3
.L1:
	ldr	r3, [pc, #64]
	adds	r2, r6, r3
	ldrh	r3, [r2, #0]
	ldr	r1, [pc, #68]
	adds	r3, r3, r1
	strh	r3, [r2, #0]
.L3:
	ldr	r3, [pc, #56]
	ldr	r1, [pc, #52]
	adds	r2, r6, r3
	movs	r3, #1
	strh	r3, [r2, #0]
	adds	r3, r6, r1
	ldrh	r1, [r3, #0]
	adds	r0, r6, #0
	bl	Func_0801b9ec
	movs	r0, #1
	bl	Func_080030f8
	movs	r2, #210
	lsls	r2, r2, #2
	adds	r3, r6, r2
	ldr	r3, [r3, #0]
	movs	r1, #0
	ldrh	r0, [r3, #10]
	bl	Func_0801b010
	movs	r0, #1
	bl	Func_080030f8
.L0:
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x0000039e
	.4byte 0x000003a2
	.4byte 0x0000ffff
