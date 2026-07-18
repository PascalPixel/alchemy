@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0801b810
	.thumb_func
Func_0801b810:
	push	{r5, r6, r7, lr}
	ldr	r1, [pc, #384]
	adds	r7, r0, #0
	adds	r6, r7, r1
	ldrh	r1, [r6, #0]
	bl	Func_0801b9a8
	ldr	r3, [pc, #376]
	adds	r2, r7, r3
	movs	r3, #33
	strh	r3, [r2, #0]
	movs	r0, #1
	bl	Func_080030f8
	movs	r1, #229
	lsls	r1, r1, #2
	adds	r3, r7, r1
	ldrh	r1, [r3, #0]
	adds	r3, r1, #0
	cmp	r3, #5
	bhi.n	.L0
	b.n	.L1
.L0:
	movs	r2, #231
	lsls	r2, r2, #2
	adds	r5, r7, r2
	ldrh	r1, [r5, #0]
	ldrh	r2, [r6, #0]
	adds	r3, r1, #0
	orrs	r3, r2
	cmp	r3, #0
	beq.n	.L2
	adds	r6, r2, #0
	cmp	r6, #1
	bne.n	.L3
	adds	r3, r1, #0
	cmp	r3, #0
	beq.n	.L3
	movs	r3, #8
	strh	r3, [r7, #8]
	ldr	r1, [pc, #316]
	ldrh	r3, [r5, #0]
	adds	r3, r3, r1
	strh	r3, [r5, #0]
	adds	r0, r7, #0
	movs	r1, #0
	bl	Func_0801ba68
	ldrh	r3, [r5, #0]
	cmp	r3, #0
	bne.n	.L4
	strh	r3, [r7, #10]
.L4:
	strh	r6, [r7, #62]
	b.n	.L5
.L3:
	ldr	r3, [pc, #280]
	adds	r2, r7, r3
	ldrh	r3, [r2, #0]
	ldr	r1, [pc, #280]
	adds	r3, r3, r1
	strh	r3, [r2, #0]
	b.n	.L5
.L2:
	movs	r2, #210
	lsls	r2, r2, #2
	adds	r3, r7, r2
	movs	r0, #0
	ldr	r5, [r3, #0]
	strh	r0, [r7, #62]
	ldr	r3, [r5, #4]
	movs	r1, #64
	cmp	r3, #0
	beq.n	.L6
	movs	r2, #12
.L7:
	ldrh	r3, [r5, #16]
	adds	r3, r3, r1
	strh	r3, [r5, #24]
	strh	r2, [r5, #20]
	ldr	r5, [r5, #4]
	ldr	r3, [r5, #4]
	subs	r1, #16
	cmp	r3, #0
	bne.n	.L7
.L6:
	movs	r1, #210
	lsls	r1, r1, #2
	adds	r3, r7, r1
	ldr	r5, [r3, #0]
	b.n	.L8
.L9:
	movs	r0, #1
	bl	Func_080030f8
.L8:
	movs	r3, #16
	ldrsh	r2, [r5, r3]
	movs	r1, #24
	ldrsh	r3, [r5, r1]
	cmp	r2, r3
	bne.n	.L9
	movs	r2, #229
	lsls	r2, r2, #2
	adds	r3, r7, r2
	ldrh	r3, [r3, #0]
	movs	r1, #0
	cmp	r3, #5
	beq.n	.L10
	adds	r3, r7, r2
	ldrh	r3, [r3, #0]
	subs	r3, #5
.L11:
	adds	r1, #1
	cmp	r1, r3
	bne.n	.L11
.L10:
	movs	r2, #210
	lsls	r2, r2, #2
	adds	r3, r7, r2
	adds	r2, #84
	ldr	r5, [r3, #0]
	adds	r3, r7, r2
	strh	r1, [r3, #0]
	ldr	r3, [pc, #156]
	adds	r2, r7, r3
	movs	r3, #4
	strh	r3, [r2, #0]
	cmp	r5, #0
	beq.n	.L12
	lsls	r3, r1, #1
	movs	r1, #213
	adds	r3, r3, r7
	lsls	r1, r1, #2
	adds	r6, r3, r1
.L13:
	ldrh	r0, [r6, #0]
	ldrh	r1, [r6, #32]
	adds	r2, r5, #0
	movs	r3, #1
	bl	Func_0801bd98
	ldr	r5, [r5, #4]
	adds	r6, #2
	cmp	r5, #0
	bne.n	.L13
.L12:
	movs	r2, #210
	lsls	r2, r2, #2
	adds	r3, r7, r2
	ldr	r1, [pc, #120]
	ldr	r5, [r3, #0]
	adds	r3, r7, r1
	ldrh	r1, [r3, #0]
	ldr	r3, [r5, #4]
	cmp	r3, #0
	beq.n	.L14
	ldr	r2, [pc, #112]
.L15:
	strh	r1, [r5, #24]
	strh	r2, [r5, #20]
	ldr	r5, [r5, #4]
	ldr	r3, [r5, #4]
	adds	r1, #16
	cmp	r3, #0
	bne.n	.L15
.L14:
	movs	r3, #1
	strh	r3, [r7, #10]
	b.n	.L5
.L1:
	ldrh	r2, [r6, #0]
	adds	r3, r2, #0
	cmp	r3, #0
	beq.n	.L16
	ldr	r1, [pc, #72]
	adds	r3, r2, r1
	b.n	.L17
.L16:
	ldr	r2, [pc, #68]
	adds	r3, r1, r2
.L17:
	strh	r3, [r6, #0]
.L5:
	ldr	r3, [pc, #56]
	ldr	r1, [pc, #52]
	adds	r2, r7, r3
	movs	r3, #1
	strh	r3, [r2, #0]
	adds	r3, r7, r1
	ldrh	r1, [r3, #0]
	adds	r0, r7, #0
	bl	Func_0801b9ec
	movs	r0, #1
	bl	Func_080030f8
	movs	r2, #210
	lsls	r2, r2, #2
	adds	r3, r7, r2
	ldr	r3, [r3, #0]
	movs	r1, #0
	ldrh	r0, [r3, #10]
	bl	Func_0801b010
	movs	r0, #1
	bl	Func_080030f8
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x0000039e
	.4byte 0x000003a2
	.4byte 0x0000ffff
	.4byte 0x00000396
	.4byte 0x0000fff4
