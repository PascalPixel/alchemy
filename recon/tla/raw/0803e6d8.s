.syntax unified
	.thumb
	.global Func_0803e6d8
	.thumb_func
Func_0803e6d8:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r6, [r3, #72]
	bl Func_0803df14
	movs r2, #212
	lsls r2, r2, #2
	adds r3, r6, r2
	ldr r0, [r3]
	movs r1, #2
	bl UiWork_Finalize
	movs r0, #1
	bl WaitFrames
	movs r2, #210
	lsls r2, r2, #2
	adds r3, r6, r2
	ldr r5, [r3]
	cmp r5, #0
	beq .L_0803e71a
	movs r7, #0
.L_0803e706:
	ldrh r3, [r5, #10]
	cmp r3, #0
	beq .L_0803e714
	ldrh r0, [r5, #12]
	bl Func_08014274
	strh r7, [r5, #10]
.L_0803e714:
	ldr r5, [r5, #4]
	cmp r5, #0
	bne .L_0803e706
.L_0803e71a:
	movs r2, #211
	lsls r2, r2, #2
	adds r3, r6, r2
	ldr r5, [r3]
	cmp r5, #0
	beq .L_0803e73c
	movs r7, #0
.L_0803e728:
	ldrh r3, [r5, #10]
	cmp r3, #0
	beq .L_0803e736
	ldrh r0, [r5, #12]
	bl Func_08014274
	strh r7, [r5, #10]
.L_0803e736:
	ldr r5, [r5, #4]
	cmp r5, #0
	bne .L_0803e728
.L_0803e73c:
	bl Func_0803f758
	movs r2, #18
	ldrsh r3, [r6, r2]
	cmp r3, #0
	beq .L_0803e760
	ldrh r0, [r6, #12]
	bl Func_08014274
	movs r2, #18
	ldrsh r3, [r6, r2]
	cmp r3, #0
	beq .L_0803e760
	adds r3, r6, #0
	adds r3, #64
	ldrh r0, [r3]
	bl Func_08014274
.L_0803e760:
	movs r2, #185
	lsls r2, r2, #2
	adds r3, r6, r2
	ldrh r0, [r3]
	bl Func_08014274
	movs r0, #72
	bl Runtime_ReleaseHeapBlock
	pop {r5, r6, r7, pc}
