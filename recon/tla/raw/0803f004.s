.syntax unified
	.thumb
	.global Menu_ScrollSelectionList
	.thumb_func
Menu_ScrollSelectionList:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	adds r7, r0, #0
	mov r10, r1
	cmp r1, #0
	beq .L_0803f0e6
	movs r0, #231
	lsls r0, r0, #2
	adds r3, r7, r0
	ldrh r3, [r3]
	movs r1, #221
	adds r3, #4
	lsls r2, r3, #1
	lsls r1, r1, #2
	adds r3, r2, r1
	ldrh r3, [r7, r3]
	movs r4, #213
	lsls r4, r4, #2
	mov r8, r3
	movs r0, #0
	adds r3, r2, r4
	ldrh r6, [r7, r3]
	bl Resource_FindFreeTransferEntry
	adds r5, r0, #0
	cmp r5, #0
	bne .L_0803f040
	b .L_0803f1ca
.L_0803f040:
	adds r2, r5, #0
	adds r0, r6, #0
	mov r1, r8
	movs r3, #0
	bl MenuSelection_SetupEntry
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #150
	adds r3, r7, r0
	ldrh r2, [r3]
	movs r4, #230
	adds r3, r2, #0
	adds r3, #80
	strh r3, [r5, #16]
	lsls r4, r4, #2
	adds r3, r7, r4
	ldrh r3, [r3]
	adds r2, #64
	strh r2, [r5, #24]
	movs r2, #255
	strh r3, [r5, #18]
	strh r3, [r5, #26]
	lsls r2, r2, #8
	movs r3, #32
	strh r3, [r5, #36]
	adds r2, #254
	strh r3, [r5, #34]
	adds r3, #224
	strh r3, [r5, #38]
	strh r2, [r5, #20]
	subs r4, #80
	adds r3, r7, r4
	adds r0, r5, #0
	ldr r5, [r3]
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #224
	strh r3, [r5, #36]
	ldrh r3, [r5, #16]
	movs r1, #0
	subs r3, #16
	strh r3, [r5, #24]
	ldr r3, [r5, #4]
	strh r1, [r5, #38]
	strh r2, [r5, #20]
	cmp r3, #0
	beq .L_0803f0b0
.L_0803f0a0:
	adds r5, r3, #0
	ldrh r3, [r5, #16]
	strh r2, [r5, #20]
	subs r3, #16
	strh r3, [r5, #24]
	ldr r3, [r5, #4]
	cmp r3, #0
	bne .L_0803f0a0
.L_0803f0b0:
	movs r3, #0
	str r0, [r5, #4]
	str r3, [r0, #4]
	str r5, [r0]
	movs r0, #210
	lsls r0, r0, #2
	adds r3, r7, r0
	ldr r5, [r3]
.L_0803f0c0:
	movs r0, #1
	bl WaitFrames
	movs r1, #34
	ldrsh r6, [r5, r1]
	cmp r6, #0
	bne .L_0803f0c0
	movs r2, #210
	lsls r2, r2, #2
	adds r3, r7, r2
	ldr r2, [r5, #4]
	str r2, [r3]
	ldrh r0, [r5, #12]
	bl Resource_ResetEntry
	strh r6, [r5, #10]
	ldr r5, [r5, #4]
	str r6, [r5]
	b .L_0803f1ca
.L_0803f0e6:
	movs r4, #231
	lsls r4, r4, #2
	adds r3, r7, r4
	ldrh r3, [r3]
	movs r0, #221
	lsls r2, r3, #1
	lsls r0, r0, #2
	adds r3, r2, r0
	ldrh r3, [r7, r3]
	movs r1, #213
	lsls r1, r1, #2
	mov r8, r3
	movs r0, #0
	adds r3, r2, r1
	ldrh r6, [r7, r3]
	bl Resource_FindFreeTransferEntry
	adds r5, r0, #0
	cmp r5, #0
	beq .L_0803f1ca
	adds r2, r5, #0
	adds r0, r6, #0
	mov r1, r8
	movs r3, #0
	bl MenuSelection_SetupEntry
	movs r2, #192
	lsls r2, r2, #2
	adds r2, #150
	adds r3, r7, r2
	ldrh r2, [r3]
	movs r4, #255
	lsls r4, r4, #8
	adds r4, #240
	adds r3, r2, r4
	strh r3, [r5, #16]
	movs r0, #230
	lsls r0, r0, #2
	adds r3, r7, r0
	ldrh r3, [r3]
	movs r1, #128
	strh r3, [r5, #18]
	strh r3, [r5, #26]
	movs r3, #2
	strh r3, [r5, #20]
	lsls r1, r1, #9
	movs r3, #32
	strh r3, [r5, #34]
	strh r3, [r5, #36]
	adds r2, r2, r1
	adds r3, #224
	strh r2, [r5, #24]
	strh r3, [r5, #38]
	movs r4, #210
	lsls r4, r4, #2
	adds r2, r7, r4
	adds r3, r5, #0
	ldr r5, [r2]
	mov r0, r10
	str r3, [r5]
	str r5, [r3, #4]
	str r0, [r3]
	str r3, [r2]
	adds r5, r3, #0
	ldrh r3, [r5, #16]
	movs r2, #2
	adds r3, #16
	strh r3, [r5, #24]
	ldr r3, [r5, #4]
	strh r2, [r5, #20]
	cmp r3, #0
	beq .L_0803f186
.L_0803f176:
	adds r5, r3, #0
	ldrh r3, [r5, #16]
	strh r2, [r5, #20]
	adds r3, #16
	strh r3, [r5, #24]
	ldr r3, [r5, #4]
	cmp r3, #0
	bne .L_0803f176
.L_0803f186:
	movs r3, #0
	strh r3, [r5, #38]
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #224
	strh r3, [r5, #36]
	movs r1, #210
	lsls r1, r1, #2
	adds r3, r7, r1
	ldr r5, [r3]
	movs r6, #128
	lsls r6, r6, #1
.L_0803f19e:
	movs r0, #1
	bl WaitFrames
	movs r2, #34
	ldrsh r3, [r5, r2]
	cmp r3, r6
	bne .L_0803f19e
	ldr r2, [r5, #4]
	cmp r2, #0
	beq .L_0803f1bc
.L_0803f1b2:
	adds r5, r2, #0
	ldr r3, [r5, #4]
	adds r2, r3, #0
	cmp r3, #0
	bne .L_0803f1b2
.L_0803f1bc:
	ldrh r0, [r5, #12]
	bl Resource_ResetEntry
	ldr r3, [r5]
	movs r2, #0
	strh r2, [r5, #10]
	str r2, [r3, #4]
.L_0803f1ca:
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
