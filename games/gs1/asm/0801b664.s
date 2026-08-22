@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0801b664
	.thumb_func
Func_0801b664:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	adds	r7, r0, #0
	ldr	r0, [pc, #396]
	adds	r5, r7, r0
	ldrh	r1, [r5, #0]
	adds	r0, r7, #0
	bl	Func_0801b9a8
	ldr	r1, [pc, #388]
	movs	r3, #0
	adds	r2, r7, r1
	mov	sl, r3
	movs	r3, #33
	strh	r3, [r2, #0]
	movs	r0, #1
	bl	Func_080030f8
	ldrh	r3, [r5, #0]
	movs	r0, #229
	adds	r3, #1
	strh	r3, [r5, #0]
	lsls	r0, r0, #2
	adds	r0, r0, r7
	ldrh	r2, [r0, #0]
	mov	r8, r0
	cmp	r2, #5
	bhi.n	.L0
	b.n	.L1
.L0:
	movs	r1, #231
	lsls	r1, r1, #2
	adds	r6, r7, r1
	ldrh	r3, [r6, #0]
	ldrh	r2, [r5, #0]
	ldrh	r1, [r0, #0]
	adds	r3, r3, r2
	cmp	r3, r1
	bne.n	.L2
	movs	r2, #210
	lsls	r2, r2, #2
	adds	r3, r7, r2
	ldr	r5, [r3, #0]
	mov	r3, sl
	strh	r3, [r7, #10]
	ldr	r3, [r5, #4]
	cmp	r3, #0
	beq.n	.L3
	ldr	r0, [pc, #316]
	movs	r3, #230
	adds	r1, r7, r0
	lsls	r3, r3, #2
	ldr	r0, [pc, #312]
	adds	r2, r7, r3
.L4:
	ldrh	r3, [r1, #0]
	strh	r3, [r5, #24]
	ldrh	r3, [r2, #0]
	strh	r0, [r5, #20]
	strh	r3, [r5, #26]
	ldr	r5, [r5, #4]
	ldr	r3, [r5, #4]
	cmp	r3, #0
	bne.n	.L4
.L3:
	ldr	r0, [pc, #284]
	adds	r3, r7, r0
	ldrh	r2, [r3, #0]
	movs	r1, #230
	strh	r2, [r5, #24]
	lsls	r1, r1, #2
	adds	r3, r7, r1
	ldrh	r3, [r3, #0]
	strh	r3, [r5, #26]
	ldr	r3, [pc, #272]
	lsls	r2, r2, #16
	strh	r3, [r5, #20]
	movs	r0, #16
	ldrsh	r3, [r5, r0]
	asrs	r2, r2, #16
	cmp	r2, r3
	beq.n	.L5
.L6:
	movs	r0, #1
	bl	Func_080030f8
	movs	r1, #24
	ldrsh	r2, [r5, r1]
	movs	r0, #16
	ldrsh	r3, [r5, r0]
	cmp	r2, r3
	bne.n	.L6
.L5:
	movs	r1, #210
	lsls	r1, r1, #2
	adds	r3, r7, r1
	ldr	r5, [r3, #0]
	cmp	r5, #0
	beq.n	.L7
	movs	r2, #213
	lsls	r2, r2, #2
	adds	r6, r7, r2
.L8:
	ldrh	r0, [r6, #0]
	ldrh	r1, [r6, #32]
	adds	r2, r5, #0
	movs	r3, #1
	bl	Func_0801bd98
	ldr	r5, [r5, #4]
	adds	r6, #2
	cmp	r5, #0
	bne.n	.L8
.L7:
	ldr	r0, [pc, #188]
	movs	r1, #231
	adds	r3, r7, r0
	movs	r2, #0
	lsls	r1, r1, #2
	strh	r2, [r3, #0]
	adds	r3, r7, r1
	strh	r2, [r3, #0]
	movs	r2, #210
	lsls	r2, r2, #2
	adds	r3, r7, r2
	ldr	r5, [r3, #0]
	movs	r0, #16
	ldrsh	r3, [r5, r0]
	ldr	r5, [r5, #4]
	adds	r3, #16
	cmp	r5, #0
	beq.n	.L9
	movs	r2, #12
.L10:
	strh	r3, [r5, #24]
	strh	r2, [r5, #20]
	ldr	r5, [r5, #4]
	adds	r3, #16
	cmp	r5, #0
	bne.n	.L10
.L9:
	movs	r3, #1
	strh	r3, [r7, #62]
	b.n	.L11
.L2:
	cmp	r2, #4
	bne.n	.L11
	adds	r3, #1
	cmp	r3, r1
	bcs.n	.L11
	ldr	r1, [pc, #136]
	adds	r3, r2, r1
	strh	r3, [r5, #0]
	movs	r3, #8
	strh	r3, [r7, #60]
	ldrh	r3, [r6, #0]
	adds	r3, #1
	strh	r3, [r6, #0]
	adds	r0, r7, #0
	movs	r1, #1
	bl	Func_0801ba68
	ldrh	r2, [r5, #0]
	ldrh	r3, [r6, #0]
	mov	r0, r8
	adds	r3, r3, r2
	ldrh	r2, [r0, #0]
	adds	r3, #2
	cmp	r3, r2
	bne.n	.L12
	mov	r1, sl
	strh	r1, [r7, #62]
.L12:
	movs	r3, #1
	strh	r3, [r7, #10]
	b.n	.L11
.L1:
	lsls	r3, r3, #16
	lsrs	r3, r3, #16
	cmp	r3, r2
	bne.n	.L11
	mov	r2, sl
	strh	r2, [r5, #0]
.L11:
	ldr	r3, [pc, #64]
	ldr	r0, [pc, #56]
	adds	r2, r7, r3
	movs	r3, #1
	strh	r3, [r2, #0]
	adds	r3, r7, r0
	ldrh	r1, [r3, #0]
	adds	r0, r7, #0
	bl	Func_0801b9ec
	movs	r0, #1
	bl	Func_080030f8
	movs	r1, #210
	lsls	r1, r1, #2
	adds	r3, r7, r1
	ldr	r3, [r3, #0]
	movs	r1, #0
	ldrh	r0, [r3, #10]
	bl	Func_0801b010
	movs	r0, #1
	bl	Func_080030f8
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x0000039e
	.4byte 0x000003a2
	.4byte 0x00000396
	.4byte 0x0000fff4
	.4byte 0x0000ffff
