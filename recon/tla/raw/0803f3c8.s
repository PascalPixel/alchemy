.syntax unified
	.thumb
	.global Func_0803f3c8
	.thumb_func
Func_0803f3c8:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r1, #231
	adds r5, r0, #0
	lsls r1, r1, #2
	adds r3, r5, r1
	ldrh r2, [r3]
	movs r3, #192
	lsls r3, r3, #2
	adds r3, #158
	adds r3, r3, r5
	mov r10, r3
	ldrh r3, [r3]
	movs r0, #0
	adds r2, r2, r3
	mov r8, r0
	adds r0, r5, #0
	mov r9, r2
	bl Menu_SendNodeCountList
	mov r0, r10
	ldrh r1, [r0]
	adds r0, r5, #0
	bl Menu_ReloadNodeResource
	movs r1, #192
	lsls r1, r1, #2
	adds r1, #162
	adds r2, r5, r1
	movs r3, #33
	strh r3, [r2]
	movs r0, #1
	bl WaitFrames
	movs r2, #128
	lsls r2, r2, #2
	movs r0, #128
	adds r2, #226
	lsls r0, r0, #2
	movs r7, #0
	adds r3, r5, r2
	adds r0, #250
	strh r7, [r5, #10]
	strh r7, [r5, #62]
	strh r7, [r3]
	adds r3, r5, r0
	strh r7, [r3]
	bl Resource_ResetPendingTransfer
	movs r1, #210
	lsls r1, r1, #2
	adds r3, r5, r1
	ldr r6, [r3]
	cmp r6, #0
	beq .L_0803f454
	mov r2, r10
	ldrh r3, [r2]
	cmp r3, #0
	beq .L_0803f454
.L_0803f444:
	ldr r6, [r6, #4]
	movs r3, #1
	add r8, r3
	cmp r6, #0
	beq .L_0803f454
	ldrh r3, [r2]
	cmp r3, r8
	bne .L_0803f444
.L_0803f454:
	ldrh r3, [r6, #16]
	movs r0, #210
	strh r3, [r6, #28]
	ldrh r3, [r6, #18]
	lsls r0, r0, #2
	strh r3, [r6, #30]
	adds r3, r5, r0
	ldr r7, [r3]
	cmp r7, #0
	beq .L_0803f484
.L_0803f468:
	cmp r7, r6
	beq .L_0803f47e
	ldrh r3, [r6, #16]
	movs r0, #16
	ldrsh r2, [r7, r0]
	strh r3, [r7, #24]
	movs r1, #16
	ldrsh r3, [r6, r1]
	subs r3, r3, r2
	asrs r3, r3, #1
	strh r3, [r7, #20]
.L_0803f47e:
	ldr r7, [r7, #4]
	cmp r7, #0
	bne .L_0803f468
.L_0803f484:
	movs r0, #2
	bl WaitFrames
	movs r1, #210
	lsls r1, r1, #2
	adds r3, r5, r1
	ldr r7, [r3]
	cmp r7, #0
	beq .L_0803f4ae
	movs r2, #0
	mov r8, r2
.L_0803f49a:
	cmp r7, r6
	beq .L_0803f4a8
	ldrh r0, [r7, #12]
	bl Resource_ResetEntry
	mov r3, r8
	strh r3, [r7, #10]
.L_0803f4a8:
	ldr r7, [r7, #4]
	cmp r7, #0
	bne .L_0803f49a
.L_0803f4ae:
	movs r0, #210
	lsls r0, r0, #2
	adds r3, r5, r0
	str r6, [r3]
	movs r3, #0
	str r3, [r6]
	str r3, [r6, #4]
	movs r3, #4
	strh r3, [r6, #24]
	movs r1, #211
	lsls r1, r1, #2
	adds r3, r5, r1
	ldr r7, [r3]
	movs r2, #0
	mov r8, r2
	cmp r7, #0
	beq .L_0803f4e0
.L_0803f4d0:
	ldrh r3, [r6, #24]
	adds r3, #16
	strh r3, [r6, #24]
	ldr r7, [r7, #4]
	movs r3, #1
	add r8, r3
	cmp r7, #0
	bne .L_0803f4d0
.L_0803f4e0:
	mov r0, r8
	lsls r1, r0, #1
	movs r3, #233
	movs r0, #231
	lsls r3, r3, #2
	lsls r0, r0, #2
	adds r2, r1, r3
	adds r3, r5, r0
	ldrh r3, [r3]
	strh r3, [r5, r2]
	movs r3, #192
	movs r2, #235
	lsls r3, r3, #2
	lsls r2, r2, #2
	adds r3, #158
	adds r0, r1, r2
	adds r1, r5, r3
	ldrh r3, [r1]
	adds r2, r5, #2
	strh r3, [r2, r0]
	movs r0, #16
	ldrsh r2, [r6, r0]
	movs r0, #24
	ldrsh r3, [r6, r0]
	movs r0, #192
	subs r3, r3, r2
	lsls r0, r0, #2
	movs r2, #0
	mov r8, r2
	asrs r3, r3, #1
	adds r0, #154
	strh r3, [r6, #20]
	mov r2, r8
	adds r3, r5, r0
	strh r2, [r3]
	ldr r3, .L_0803f558
	ldrh r2, [r1]
	movs r0, #2
	orrs r3, r2
	strh r3, [r1]
	bl WaitFrames
	movs r0, #1
	bl Func_0803deac
	ldrh r3, [r6, #10]
	adds r7, r0, #0
	strh r3, [r7, #10]
	mov r0, r8
	ldrh r3, [r6, #32]
	movs r4, #63
	strh r3, [r7, #32]
	ldrh r3, [r6, #8]
	strh r3, [r7, #8]
	ldrh r3, [r6, #12]
	strh r3, [r7, #12]
	ldrh r3, [r6, #14]
	strh r3, [r7, #14]
	b .L_0803f55c
	.2byte 0x0000
.L_0803f558:
	.4byte 0x00000080
.L_0803f55c:
	ldrh r2, [r6, #16]
	strh r2, [r7, #16]
	ldrh r3, [r6, #18]
	strh r2, [r7, #24]
	strh r3, [r7, #18]
	strh r3, [r7, #26]
	movs r2, #13
	ldrh r3, [r6, #28]
	negs r2, r2
	strh r3, [r7, #28]
	ldrh r3, [r6, #30]
	strh r0, [r7, #22]
	strh r3, [r7, #30]
	mov r3, r8
	strh r3, [r7, #20]
	movs r3, #128
	lsls r3, r3, #1
	strh r3, [r7, #34]
	strh r3, [r7, #38]
	adds r0, r7, #0
	adds r0, #40
	ldrb r3, [r0, #5]
	ldrb r1, [r0, #7]
	ands r2, r3
	movs r3, #33
	negs r3, r3
	ands r2, r3
	adds r3, #16
	ands r2, r3
	ands r2, r4
	adds r3, r4, #0
	strb r2, [r0, #5]
	ands r3, r1
	ldrb r2, [r0, #9]
	movs r1, #64
	orrs r3, r1
	strb r3, [r0, #7]
	movs r3, #15
	ands r3, r2
	strb r3, [r0, #9]
	ldrh r3, [r7, #14]
	movs r2, #192
	lsls r2, r2, #2
	adds r2, #255
	ldrh r1, [r0, #8]
	ands r2, r3
	ldr r3, .L_0803f608
	ands r3, r1
	orrs r3, r2
	movs r2, #210
	lsls r2, r2, #2
	strh r3, [r0, #8]
	mov r1, r8
	adds r3, r5, r2
	mov r0, r8
	strh r1, [r6, #10]
	str r0, [r3]
	movs r1, #211
	lsls r1, r1, #2
	adds r0, r5, r1
	ldr r3, [r0]
	cmp r3, #0
	beq .L_0803f5f4
	adds r6, r3, #0
	ldr r2, [r6, #4]
	cmp r2, #0
	beq .L_0803f5ec
.L_0803f5e2:
	adds r6, r2, #0
	ldr r3, [r6, #4]
	adds r2, r3, #0
	cmp r3, #0
	bne .L_0803f5e2
.L_0803f5ec:
	movs r3, #0
	str r7, [r6, #4]
	str r6, [r7]
	b .L_0803f5f8
.L_0803f5f4:
	str r7, [r0]
	str r3, [r7]
.L_0803f5f8:
	str r3, [r7, #4]
	mov r0, r9
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0803f608:
	.4byte 0xfffffc00
	.4byte 0x00004770
