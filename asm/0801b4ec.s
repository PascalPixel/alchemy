@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0801b4ec
	.thumb_func
Func_0801b4ec:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	adds	r5, r0, #0
	ldr	r1, [pc, #192]
	movs	r0, #231
	lsls	r0, r0, #2
	adds	r7, r5, r0
	adds	r6, r5, r1
	ldrh	r3, [r7, #0]
	ldrh	r1, [r6, #0]
	movs	r2, #229
	adds	r3, r3, r1
	lsls	r2, r2, #2
	adds	r3, #1
	adds	r2, r2, r5
	mov	sl, r3
	ldrh	r3, [r2, #0]
	mov	r8, r2
	cmp	sl, r3
	beq.n	.L0
	adds	r0, r5, #0
	bl	Func_0801b9a8
	ldr	r3, [pc, #156]
	adds	r2, r5, r3
	movs	r3, #33
	strh	r3, [r2, #0]
	movs	r0, #1
	bl	Func_080030f8
	ldrh	r1, [r6, #0]
	movs	r0, #128
	adds	r3, r1, #1
	strh	r3, [r6, #0]
	lsls	r0, r0, #11
	lsls	r3, r3, #16
	cmp	r3, r0
	bne.n	.L1
	mov	r0, r8
	mov	r2, sl
	ldrh	r3, [r0, #0]
	adds	r2, #1
	cmp	r2, r3
	bcs.n	.L1
	movs	r2, #128
	lsls	r2, r2, #9
	adds	r3, r1, r2
	strh	r3, [r6, #0]
	movs	r3, #8
	strh	r3, [r5, #60]
	ldrh	r3, [r7, #0]
	adds	r3, #1
	strh	r3, [r7, #0]
	adds	r0, r5, #0
	movs	r1, #1
	bl	Func_0801ba68
	ldrh	r2, [r6, #0]
	ldrh	r3, [r7, #0]
	mov	r0, r8
	adds	r3, r3, r2
	ldrh	r2, [r0, #0]
	adds	r3, #2
	cmp	r3, r2
	bne.n	.L2
	movs	r3, #0
	strh	r3, [r5, #62]
.L2:
	movs	r3, #1
	strh	r3, [r5, #10]
.L1:
	ldr	r1, [pc, #64]
	movs	r3, #1
	adds	r2, r5, r1
	strh	r3, [r2, #0]
	ldr	r2, [pc, #52]
	adds	r3, r5, r2
	ldrh	r1, [r3, #0]
	adds	r0, r5, #0
	bl	Func_0801b9ec
	movs	r0, #1
	bl	Func_080030f8
	movs	r0, #210
	lsls	r0, r0, #2
	adds	r3, r5, r0
	ldr	r3, [r3, #0]
	movs	r1, #0
	ldrh	r0, [r3, #10]
	bl	Func_0801b010
	movs	r0, #1
	bl	Func_080030f8
.L0:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x0000039e
	.4byte 0x000003a2
