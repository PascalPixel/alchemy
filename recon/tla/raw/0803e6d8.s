.syntax unified
	.thumb
	.balign 4
	.global Func_0803e6d8
	.thumb_func
Func_0803e6d8:
	push	{r5, r6, r7, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r6, [r3, #72]
	bl	Func_0803df14
	movs	r2, #212
	lsls	r2, r2, #2
	adds	r3, r6, r2
	ldr	r0, [r3, #0]
	movs	r1, #2
	bl	UiWork_Finalize
	movs	r0, #1
	bl	WaitFrames
	movs	r2, #210
	lsls	r2, r2, #2
	adds	r3, r6, r2
	ldr	r5, [r3, #0]
	cmp	r5, #0
	beq.n	.L_0803e71a
	movs	r7, #0
.L_0803e706:
	ldrh	r3, [r5, #10]
	cmp	r3, #0
	beq.n	.L_0803e714
	ldrh	r0, [r5, #12]
	bl	Func_08014274
	strh	r7, [r5, #10]
.L_0803e714:
	ldr	r5, [r5, #4]
	cmp	r5, #0
	bne.n	.L_0803e706
.L_0803e71a:
	movs	r2, #211
	lsls	r2, r2, #2
	adds	r3, r6, r2
	ldr	r5, [r3, #0]
	cmp	r5, #0
	beq.n	.L_0803e73c
	movs	r7, #0
.L_0803e728:
	ldrh	r3, [r5, #10]
	cmp	r3, #0
	beq.n	.L_0803e736
	ldrh	r0, [r5, #12]
	bl	Func_08014274
	strh	r7, [r5, #10]
.L_0803e736:
	ldr	r5, [r5, #4]
	cmp	r5, #0
	bne.n	.L_0803e728
.L_0803e73c:
	bl	0x0803f758
	movs	r2, #18
	ldrsh	r3, [r6, r2]
	cmp	r3, #0
	beq.n	.L_0803e760
	ldrh	r0, [r6, #12]
	bl	Func_08014274
	movs	r2, #18
	ldrsh	r3, [r6, r2]
	cmp	r3, #0
	beq.n	.L_0803e760
	adds	r3, r6, #0
	adds	r3, #64
	ldrh	r0, [r3, #0]
	bl	Func_08014274
.L_0803e760:
	movs	r2, #185
	lsls	r2, r2, #2
	adds	r3, r6, r2
	ldrh	r0, [r3, #0]
	bl	Func_08014274
	movs	r0, #72
	bl	Runtime_ReleaseHeapBlock
	pop	{r5, r6, r7, pc}
	.global Func_0803e774
	.thumb_func
Func_0803e774:
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #72]
	movs	r4, #192
	lsls	r4, r4, #2
	adds	r4, #150
	adds	r2, r3, r4
	adds	r4, #2
	strh	r0, [r2, #0]
	adds	r2, r3, r4
	strh	r1, [r2, #0]
	movs	r2, #210
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_0803e7a8
.L_0803e798:
	strh	r0, [r3, #16]
	strh	r0, [r3, #24]
	strh	r1, [r3, #18]
	strh	r1, [r3, #26]
	ldr	r3, [r3, #4]
	adds	r0, #16
	cmp	r3, #0
	bne.n	.L_0803e798
.L_0803e7a8:
	pop	{pc}
	.2byte 0x0000
