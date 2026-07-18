@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0801ba68
	.thumb_func
Func_0801ba68:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	adds	r7, r0, #0
	mov	sl, r1
	cmp	r1, #0
	beq.n	.L0
	movs	r0, #231
	lsls	r0, r0, #2
	adds	r3, r7, r0
	ldrh	r3, [r3, #0]
	movs	r1, #221
	adds	r3, #4
	lsls	r2, r3, #1
	lsls	r1, r1, #2
	adds	r3, r2, r1
	ldrh	r3, [r7, r3]
	movs	r4, #213
	lsls	r4, r4, #2
	mov	r8, r3
	movs	r0, #0
	adds	r3, r2, r4
	ldrh	r6, [r7, r3]
	bl	Func_0801a910
	adds	r5, r0, #0
	cmp	r5, #0
	bne.n	.L1
	b.n	.L2
.L1:
	adds	r2, r5, #0
	adds	r0, r6, #0
	mov	r1, r8
	movs	r3, #0
	bl	Func_0801bd98
	ldr	r0, [pc, #368]
	adds	r3, r7, r0
	ldrh	r2, [r3, #0]
	adds	r3, r2, #0
	adds	r3, #80
	movs	r4, #230
	strh	r3, [r5, #16]
	lsls	r4, r4, #2
	adds	r3, r7, r4
	ldrh	r3, [r3, #0]
	adds	r2, #64
	strh	r2, [r5, #24]
	strh	r3, [r5, #18]
	strh	r3, [r5, #26]
	ldr	r2, [pc, #344]
	movs	r3, #32
	strh	r3, [r5, #36]
	strh	r3, [r5, #34]
	adds	r3, #224
	strh	r3, [r5, #38]
	strh	r2, [r5, #20]
	subs	r4, #80
	adds	r3, r7, r4
	adds	r0, r5, #0
	ldr	r5, [r3, #0]
	ldr	r3, [pc, #328]
	strh	r3, [r5, #36]
	ldrh	r3, [r5, #16]
	subs	r3, #16
	strh	r3, [r5, #24]
	ldr	r3, [r5, #4]
	movs	r1, #0
	strh	r1, [r5, #38]
	strh	r2, [r5, #20]
	cmp	r3, #0
	beq.n	.L3
.L4:
	adds	r5, r3, #0
	ldrh	r3, [r5, #16]
	subs	r3, #16
	strh	r3, [r5, #24]
	ldr	r3, [r5, #4]
	strh	r2, [r5, #20]
	cmp	r3, #0
	bne.n	.L4
.L3:
	movs	r3, #0
	str	r0, [r5, #4]
	str	r3, [r0, #4]
	str	r5, [r0, #0]
	movs	r0, #210
	lsls	r0, r0, #2
	adds	r3, r7, r0
	ldr	r5, [r3, #0]
.L5:
	movs	r0, #1
	bl	Func_080030f8
	movs	r1, #34
	ldrsh	r6, [r5, r1]
	cmp	r6, #0
	bne.n	.L5
	movs	r2, #210
	lsls	r2, r2, #2
	adds	r3, r7, r2
	ldr	r2, [r5, #4]
	str	r2, [r3, #0]
	ldrh	r0, [r5, #12]
	bl	Func_08003f3c
	strh	r6, [r5, #10]
	ldr	r5, [r5, #4]
	str	r6, [r5, #0]
	b.n	.L2
.L0:
	movs	r4, #231
	lsls	r4, r4, #2
	adds	r3, r7, r4
	ldrh	r3, [r3, #0]
	movs	r0, #221
	lsls	r2, r3, #1
	lsls	r0, r0, #2
	adds	r3, r2, r0
	ldrh	r3, [r7, r3]
	movs	r1, #213
	lsls	r1, r1, #2
	mov	r8, r3
	movs	r0, #0
	adds	r3, r2, r1
	ldrh	r6, [r7, r3]
	bl	Func_0801a910
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L2
	adds	r2, r5, #0
	adds	r0, r6, #0
	mov	r1, r8
	movs	r3, #0
	bl	Func_0801bd98
	ldr	r2, [pc, #176]
	adds	r3, r7, r2
	ldrh	r2, [r3, #0]
	ldr	r4, [pc, #180]
	movs	r0, #230
	adds	r3, r2, r4
	strh	r3, [r5, #16]
	lsls	r0, r0, #2
	adds	r3, r7, r0
	ldrh	r3, [r3, #0]
	movs	r1, #128
	strh	r3, [r5, #18]
	strh	r3, [r5, #26]
	movs	r3, #2
	strh	r3, [r5, #20]
	lsls	r1, r1, #9
	movs	r3, #32
	strh	r3, [r5, #34]
	strh	r3, [r5, #36]
	adds	r2, r2, r1
	adds	r3, #224
	movs	r4, #210
	strh	r2, [r5, #24]
	strh	r3, [r5, #38]
	lsls	r4, r4, #2
	adds	r2, r7, r4
	adds	r3, r5, #0
	ldr	r5, [r2, #0]
	mov	r0, sl
	str	r3, [r5, #0]
	str	r5, [r3, #4]
	str	r0, [r3, #0]
	str	r3, [r2, #0]
	adds	r5, r3, #0
	ldrh	r3, [r5, #16]
	adds	r3, #16
	strh	r3, [r5, #24]
	ldr	r3, [r5, #4]
	movs	r2, #2
	strh	r2, [r5, #20]
	cmp	r3, #0
	beq.n	.L6
.L7:
	adds	r5, r3, #0
	ldrh	r3, [r5, #16]
	adds	r3, #16
	strh	r3, [r5, #24]
	ldr	r3, [r5, #4]
	strh	r2, [r5, #20]
	cmp	r3, #0
	bne.n	.L7
.L6:
	movs	r3, #0
	strh	r3, [r5, #38]
	ldr	r3, [pc, #80]
	movs	r1, #210
	strh	r3, [r5, #36]
	lsls	r1, r1, #2
	adds	r3, r7, r1
	movs	r6, #128
	ldr	r5, [r3, #0]
	lsls	r6, r6, #1
.L8:
	movs	r0, #1
	bl	Func_080030f8
	movs	r2, #34
	ldrsh	r3, [r5, r2]
	cmp	r3, r6
	bne.n	.L8
	ldr	r2, [r5, #4]
	cmp	r2, #0
	beq.n	.L9
.L10:
	adds	r5, r2, #0
	ldr	r3, [r5, #4]
	adds	r2, r3, #0
	cmp	r3, #0
	bne.n	.L10
.L9:
	ldrh	r0, [r5, #12]
	bl	Func_08003f3c
	ldr	r3, [r5, #0]
	movs	r2, #0
	strh	r2, [r5, #10]
	str	r2, [r3, #4]
.L2:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x00000396
	.4byte 0x0000fffe
	.4byte 0x0000ffe0
	.4byte 0x0000fff0
