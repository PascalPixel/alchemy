.syntax unified
	.thumb
	.global Menu_ConfirmSelection
	.thumb_func
Menu_ConfirmSelection:
	.global Menu_PushSelectedNode
	.thumb_func
Menu_PushSelectedNode:
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
	ldr r3, .L_0801bffc
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
	ldr r1, .L_0801c000
	movs r3, #33
	adds r2, r5, r1
	strh r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r2, .L_0801c004
	ldr r0, .L_0801c008
	movs r7, #0
	adds r3, r5, r2
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
	beq .L_0801befc
	mov r2, r10
	ldrh r3, [r2]
	cmp r3, #0
	beq .L_0801befc
.L_0801beec:
	ldr r6, [r6, #4]
	movs r3, #1
	add r8, r3
	cmp r6, #0
	beq .L_0801befc
	ldrh r3, [r2]
	cmp r3, r8
	bne .L_0801beec
.L_0801befc:
	ldrh r3, [r6, #16]
	strh r3, [r6, #28]
	ldrh r3, [r6, #18]
	movs r0, #210
	strh r3, [r6, #30]
	lsls r0, r0, #2
	adds r3, r5, r0
	ldr r7, [r3]
	cmp r7, #0
	beq .L_0801bf2c
.L_0801bf10:
	cmp r7, r6
	beq .L_0801bf26
	ldrh r3, [r6, #16]
	strh r3, [r7, #24]
	movs r0, #16
	ldrsh r2, [r7, r0]
	movs r1, #16
	ldrsh r3, [r6, r1]
	subs r3, r3, r2
	asrs r3, r3, #1
	strh r3, [r7, #20]
.L_0801bf26:
	ldr r7, [r7, #4]
	cmp r7, #0
	bne .L_0801bf10
.L_0801bf2c:
	movs r0, #2
	bl WaitFrames
	movs r1, #210
	lsls r1, r1, #2
	adds r3, r5, r1
	ldr r7, [r3]
	cmp r7, #0
	beq .L_0801bf56
	movs r2, #0
	mov r8, r2
.L_0801bf42:
	cmp r7, r6
	beq .L_0801bf50
	ldrh r0, [r7, #12]
	bl Resource_ResetEntry
	mov r3, r8
	strh r3, [r7, #10]
.L_0801bf50:
	ldr r7, [r7, #4]
	cmp r7, #0
	bne .L_0801bf42
.L_0801bf56:
	movs r0, #210
	lsls r0, r0, #2
	adds r3, r5, r0
	str r6, [r3]
	movs r3, #0
	str r3, [r6]
	str r3, [r6, #4]
	movs r1, #211
	movs r3, #4
	strh r3, [r6, #24]
	lsls r1, r1, #2
	adds r3, r5, r1
	ldr r7, [r3]
	movs r2, #0
	mov r8, r2
	cmp r7, #0
	beq .L_0801bf88
.L_0801bf78:
	ldrh r3, [r6, #24]
	adds r3, #16
	strh r3, [r6, #24]
	ldr r7, [r7, #4]
	movs r3, #1
	add r8, r3
	cmp r7, #0
	bne .L_0801bf78
.L_0801bf88:
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
	ldr r3, .L_0801bffc
	movs r2, #235
	lsls r2, r2, #2
	adds r0, r1, r2
	adds r1, r5, r3
	ldrh r3, [r1]
	adds r2, r5, #2
	strh r3, [r2, r0]
	movs r0, #16
	ldrsh r2, [r6, r0]
	movs r0, #24
	ldrsh r3, [r6, r0]
	ldr r0, .L_0801c00c
	subs r3, r3, r2
	movs r2, #0
	mov r8, r2
	asrs r3, r3, #1
	strh r3, [r6, #20]
	mov r2, r8
	adds r3, r5, r0
	strh r2, [r3]
	ldr r3, .L_0801bff8
	ldrh r2, [r1]
	orrs r3, r2
	strh r3, [r1]
	movs r0, #2
	bl WaitFrames
	movs r0, #1
	bl Resource_FindFreeTransferEntry
	ldrh r3, [r6, #10]
	adds r7, r0, #0
	strh r3, [r7, #10]
	ldrh r3, [r6, #32]
	strh r3, [r7, #32]
	ldrh r3, [r6, #8]
	strh r3, [r7, #8]
	ldrh r3, [r6, #12]
	strh r3, [r7, #12]
	ldrh r3, [r6, #14]
	strh r3, [r7, #14]
	ldrh r2, [r6, #16]
	strh r2, [r7, #16]
	b .L_0801c010
	.2byte 0x0000
.L_0801bff8:
	.4byte 0x00000080
.L_0801bffc:
	.4byte 0x0000039e
.L_0801c000:
	.4byte 0x000003a2
.L_0801c004:
	.4byte 0x000002e2
.L_0801c008:
	.4byte 0x000002fa
.L_0801c00c:
	.4byte 0x0000039a
.L_0801c010:
	ldrh r3, [r6, #18]
	strh r2, [r7, #24]
	strh r3, [r7, #18]
	strh r3, [r7, #26]
	ldrh r3, [r6, #28]
	strh r3, [r7, #28]
	ldrh r3, [r6, #30]
	strh r3, [r7, #30]
	mov r3, r8
	strh r3, [r7, #20]
	movs r3, #128
	lsls r3, r3, #1
	mov r0, r8
	strh r0, [r7, #22]
	strh r3, [r7, #34]
	strh r3, [r7, #38]
	adds r0, r7, #0
	adds r0, #40
	ldrb r3, [r0, #5]
	movs r2, #13
	negs r2, r2
	ands r2, r3
	movs r3, #33
	negs r3, r3
	ldrb r1, [r0, #7]
	ands r2, r3
	movs r4, #63
	adds r3, #16
	ands r2, r3
	adds r3, r4, #0
	ands r3, r1
	ands r2, r4
	movs r1, #64
	strb r2, [r0, #5]
	orrs r3, r1
	ldrb r2, [r0, #9]
	strb r3, [r0, #7]
	movs r3, #15
	ands r3, r2
	strb r3, [r0, #9]
	ldrh r3, [r7, #14]
	ldr r2, .L_0801c0bc
	ldrh r1, [r0, #8]
	ands r2, r3
	ldr r3, .L_0801c0c0
	ands r3, r1
	orrs r3, r2
	movs r2, #210
	mov r1, r8
	lsls r2, r2, #2
	strh r3, [r0, #8]
	strh r1, [r6, #10]
	adds r3, r5, r2
	mov r0, r8
	movs r1, #211
	str r0, [r3]
	lsls r1, r1, #2
	adds r0, r5, r1
	ldr r3, [r0]
	cmp r3, #0
	beq .L_0801c0a4
	adds r6, r3, #0
	ldr r2, [r6, #4]
	cmp r2, #0
	beq .L_0801c09c
.L_0801c092:
	adds r6, r2, #0
	ldr r3, [r6, #4]
	adds r2, r3, #0
	cmp r3, #0
	bne .L_0801c092
.L_0801c09c:
	movs r3, #0
	str r7, [r6, #4]
	str r6, [r7]
	b .L_0801c0a8
.L_0801c0a4:
	str r7, [r0]
	str r3, [r7]
.L_0801c0a8:
	str r3, [r7, #4]
	mov r0, r9
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7}
	pop {r1}
	bx r1
	.2byte 0x0000
.L_0801c0bc:
	.4byte 0x000003ff
.L_0801c0c0:
	.4byte 0xfffffc00
