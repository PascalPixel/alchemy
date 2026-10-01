.syntax unified
	.thumb
	.global Func_080ed2a4
	.thumb_func
Func_080ed2a4:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #56
	str r0, [sp, #28]
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #32]
	ldr r4, .L_080ed36c
	str r1, [sp, #20]
	add r6, sp, #28
	ldr r2, [r3, #108]
	mov r11, r4
	str r2, [sp, #16]
	ldr r3, [r3, #24]
	str r3, [sp, #12]
	movs r3, #0
	str r3, [sp, #4]
	str r3, [sp, #0]
	mov r9, r3
	movs r3, #192
	lsls r3, r3, #3
	ldrh r6, [r6]
	adds r3, #66
	add r3, r11
	strh r6, [r3]
	ldr r1, [sp, #28]
	cmp r1, #0
	bne .L_080ed2f6
	movs r4, #197
	lsls r4, r4, #1
	adds r3, r2, r4
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #3
	beq .L_080ed30c
.L_080ed2f6:
	bl Func_080cad9c
	ldr r6, [sp, #16]
	movs r1, #197
	lsls r1, r1, #1
	adds r3, r6, r1
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #3
	bne .L_080ed370
.L_080ed30c:
	ldr r3, [sp, #16]
	movs r4, #218
	lsls r4, r4, #1
	adds r2, r3, r4
	ldr r6, [r2]
	movs r3, #6
	str r6, [sp, #24]
	str r3, [r2]
	bl Func_080d2a64
	bl Func_080d2a8c
	ldr r1, [sp, #20]
	mov r0, sp
	adds r1, #24
	movs r4, #1
	add r2, sp, #40
	adds r0, #55
.L_080ed330:
	ldrh r3, [r1, #10]
	strh r4, [r1, #10]
	strb r3, [r2]
	adds r2, #1
	adds r1, #12
	cmp r2, r0
	ble .L_080ed330
	ldr r1, [sp, #12]
	movs r3, #1
	strh r3, [r1, #4]
	movs r0, #1
	bl WaitFrames
	movs r4, #144
	ldr r2, [sp, #20]
	ldr r5, .L_080ed368
	lsls r4, r4, #4
	adds r4, #114
	adds r3, r2, r4
	strb r5, [r3]
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #82
	ldrh r3, [r3]
	lsls r3, r3, #16
	asrs r3, r3, #16
	str r3, [sp, #8]
	b .L_080ed436
.L_080ed368:
	.4byte 0x00000001
.L_080ed36c:
	.4byte Data_0202a000
.L_080ed370:
	ldr r6, [sp, #16]
	movs r1, #230
	lsls r1, r1, #1
	adds r3, r6, r1
	ldr r3, [r3]
	movs r5, #1
	adds r3, #91
	strb r5, [r3]
	movs r3, #218
	lsls r3, r3, #1
	adds r2, r6, r3
	ldr r4, [r2]
	subs r1, #32
	movs r3, #6
	str r4, [sp, #24]
	str r3, [r2]
	adds r2, r6, r1
	movs r3, #0
	str r3, [r2]
	bl Func_080d2a64
	bl Func_080d2a8c
	bl Scheduler_ResetTaskTable
	bl UiWork_InitializeWithResourceCountersFar
	ldr r2, [sp, #12]
	movs r0, #1
	strh r5, [r2, #4]
	bl WaitFrames
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #82
	ldrh r3, [r3]
	movs r4, #128
	lsls r3, r3, #16
	asrs r3, r3, #16
	str r3, [sp, #8]
	ldr r0, .L_080ed694
	ldr r2, .L_080ed698
	lsls r4, r4, #10
	movs r1, #0
	adds r4, #2
.L_080ed3ca:
	movs r3, #15
.L_080ed3cc:
	subs r3, #1
	stmia r0!, {r2}
	adds r2, r2, r4
	cmp r3, #0
	bge .L_080ed3cc
	adds r1, #1
	cmp r1, #19
	ble .L_080ed3ca
	movs r4, #128
	ldr r2, .L_080ed69c
	lsls r4, r4, #10
	movs r1, #0
	adds r4, #2
.L_080ed3e6:
	movs r3, #15
.L_080ed3e8:
	subs r3, #1
	stmia r0!, {r2}
	adds r2, r2, r4
	cmp r3, #0
	bge .L_080ed3e8
	adds r1, #1
	cmp r1, #23
	ble .L_080ed3e6
	movs r4, #128
	ldr r2, .L_080ed69c
	lsls r4, r4, #10
	movs r1, #0
	adds r4, #2
.L_080ed402:
	movs r3, #15
.L_080ed404:
	subs r3, #1
	stmia r0!, {r2}
	adds r2, r2, r4
	cmp r3, #0
	bge .L_080ed404
	adds r1, #1
	cmp r1, #3
	ble .L_080ed402
	ldr r4, [sp, #20]
	ldr r2, .L_080ed6a0
	movs r3, #0
	str r3, [r4]
	strh r3, [r2, #4]
	strh r3, [r2, #6]
	movs r2, #192
	movs r3, #128
	lsls r2, r2, #3
	lsls r3, r3, #19
	adds r2, #130
	adds r3, #10
	strh r2, [r3]
	movs r2, #154
	lsls r2, r2, #5
	subs r3, #10
	strh r2, [r3]
.L_080ed436:
	bl Func_080ebf94
.L_080ed43a:
	mov r6, sp
	ldr r3, .L_080ed6a4
	ldrh r6, [r6]
	strh r6, [r3]
	bl Func_080ece20
	movs r0, #1
	bl WaitFrames
	movs r2, #134
	movs r3, #128
	lsls r2, r2, #8
	lsls r3, r3, #19
	adds r2, #130
	adds r3, #10
	strh r2, [r3]
	movs r2, #154
	lsls r2, r2, #5
	subs r3, #10
	strh r2, [r3]
	movs r0, #142
	lsls r0, r0, #1
	bl GameFlag_Test
	cmp r0, #0
	bne .L_080ed47a
	movs r0, #143
	lsls r0, r0, #1
	bl GameFlag_Test
	cmp r0, #0
	beq .L_080ed48c
.L_080ed47a:
	ldr r1, [sp, #4]
	cmp r1, #0
	bne .L_080ed48c
	ldr r0, .L_080ed6a8
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWaitFar
	movs r2, #1
	str r2, [sp, #4]
.L_080ed48c:
	ldr r3, .L_080ed6ac
	ldr r4, .L_080ed6b0
	ldr r6, .L_080ed6b4
	ldr r7, .L_080ed6b8
	mov r10, r3
	mov r8, r4
.L_080ed498:
	bl Func_080ed0a0
	mov r5, r10
	bl Func_080ec4d4
	movs r0, #1
	bl WaitFrames
	ldr r2, [r5, #12]
	movs r3, #8
	ands r2, r3
	cmp r2, #0
	beq .L_080ed4d2
	mov r1, r9
	cmp r1, #0
	bne .L_080ed4d2
	add r0, sp, #36
	add r1, sp, #32
	bl Func_080ec1d0
	ldr r3, [sp, #36]
	ldr r5, .L_080ed6ac
	lsls r3, r3, #16
	str r3, [r6]
	ldr r3, [sp, #32]
	movs r2, #1
	lsls r3, r3, #16
	str r3, [r7]
	mov r9, r2
.L_080ed4d2:
	mov r3, r10
	ldr r2, [r3]
	movs r3, #8
	ands r2, r3
	cmp r2, #0
	bne .L_080ed4f6
	mov r4, r9
	cmp r4, #1
	bne .L_080ed4f6
	mov r1, r8
	ldr r3, [r1]
	ldr r5, .L_080ed6ac
	str r3, [r6]
	ldr r3, .L_080ed6bc
	movs r2, #0
	ldr r3, [r3]
	mov r9, r2
	str r3, [r7]
.L_080ed4f6:
	mov r3, r10
	ldr r2, [r3, #12]
	movs r3, #128
	lsls r3, r3, #1
	ands r2, r3
	cmp r2, #0
	beq .L_080ed526
	movs r2, #1
	add r0, sp, #36
	add r1, sp, #32
	bl Func_080ec2b8
	ldr r3, [sp, #36]
	mov r4, r8
	lsls r3, r3, #16
	str r3, [r6]
	ldr r3, [sp, #32]
	ldr r2, .L_080ed6bc
	lsls r3, r3, #16
	str r3, [r7]
	ldr r3, [r6]
	str r3, [r4]
	ldr r3, [r7]
	str r3, [r2]
.L_080ed526:
	ldr r2, [r5, #12]
	movs r3, #128
	lsls r3, r3, #2
	ands r2, r3
	cmp r2, #0
	beq .L_080ed556
	movs r2, #1
	add r1, sp, #32
	negs r2, r2
	add r0, sp, #36
	bl Func_080ec2b8
	ldr r3, [sp, #36]
	mov r1, r8
	lsls r3, r3, #16
	str r3, [r6]
	ldr r3, [sp, #32]
	ldr r2, .L_080ed6bc
	lsls r3, r3, #16
	str r3, [r7]
	ldr r3, [r6]
	str r3, [r1]
	ldr r3, [r7]
	str r3, [r2]
.L_080ed556:
	ldr r2, [r5, #12]
	movs r3, #4
	ands r2, r3
	cmp r2, #0
	beq .L_080ed5d2
	movs r3, #255
	lsls r3, r3, #8
	mov r2, r11
	adds r3, #255
	strh r3, [r2, #18]
	ldr r0, [r2, #28]
	bl RenderOutput_PrepareForRedrawFar
	movs r2, #64
	movs r3, #128
	lsls r3, r3, #19
	strh r2, [r3]
	movs r3, #1
	ldr r4, [sp, #0]
	eors r4, r3
	str r4, [sp, #0]
	ldr r2, [r6]
	cmp r2, #0
	bne .L_080ed588
	b .L_080ed43a
.L_080ed588:
	mov r6, r11
	str r2, [r6, #4]
	ldr r3, [r7]
	ldr r1, .L_080ed6c0
	str r3, [r6, #8]
	ldr r6, .L_080ed6c4
	movs r0, #136
	adds r3, r2, r6
	str r3, [r1]
	mov r6, r11
	ldr r3, [r6, #8]
	ldr r6, .L_080ed6c8
	ldr r2, .L_080ed6cc
	adds r3, r3, r6
	str r3, [r2]
	movs r5, #224
	ldr r3, [r1]
	movs r4, #0
	lsls r0, r0, #17
	lsls r5, r5, #16
	cmp r3, #0
	bge .L_080ed5b8
	str r4, [r1]
	movs r3, #0
.L_080ed5b8:
	cmp r3, r0
	ble .L_080ed5be
	str r0, [r1]
.L_080ed5be:
	ldr r3, [r2]
	cmp r3, #0
	bge .L_080ed5c8
	str r4, [r2]
	movs r3, #0
.L_080ed5c8:
	cmp r3, r5
	bgt .L_080ed5ce
	b .L_080ed43a
.L_080ed5ce:
	str r5, [r2]
	b .L_080ed43a
.L_080ed5d2:
	ldr r2, [r5, #12]
	movs r3, #1
	ands r2, r3
	cmp r2, #0
	beq .L_080ed628
	ldr r1, [sp, #28]
	cmp r1, #0
	beq .L_080ed654
	mov r4, r11
	movs r2, #18
	ldrsh r3, [r4, r2]
	movs r1, #1
	negs r1, r1
	cmp r3, r1
	bne .L_080ed5f2
	b .L_080ed498
.L_080ed5f2:
	bl Func_080ed804
	mov r4, r11
	movs r3, #18
	ldrsh r2, [r4, r3]
	ldr r6, .L_080ed6d0
	lsls r3, r2, #2
	movs r1, #128
	adds r3, r3, r2
	lsls r1, r1, #2
	ldr r2, .L_080ed6d4
	lsls r3, r3, #2
	adds r1, #190
	adds r0, r0, r3
	adds r3, r6, r1
	strh r2, [r3]
	ldrb r3, [r0]
	ldr r2, .L_080ed6d8
	movs r6, #18
	ldrsh r1, [r0, r6]
	strh r3, [r2]
	movs r4, #16
	ldrsh r3, [r0, r4]
	adds r0, r3, #0
	bl Func_080ca368
	b .L_080ed654
.L_080ed628:
	ldr r2, [r5, #12]
	movs r3, #2
	ands r2, r3
	cmp r2, #0
	bne .L_080ed634
	b .L_080ed498
.L_080ed634:
	ldr r1, [sp, #28]
	cmp r1, #0
	beq .L_080ed654
	ldr r2, .L_080ed6d0
	movs r4, #128
	lsls r4, r4, #2
	adds r4, #190
	adds r3, r2, r4
	movs r2, #255
	lsls r2, r2, #8
	adds r2, #255
	ldr r6, .L_080ed6d8
	strh r2, [r3]
	movs r3, #1
	negs r3, r3
	strh r3, [r6]
.L_080ed654:
	bl Func_080ec14c
	movs r2, #64
	movs r3, #128
	lsls r3, r3, #19
	strh r2, [r3]
	ldr r1, [sp, #28]
	cmp r1, #0
	bne .L_080ed678
	ldr r2, [sp, #16]
	movs r4, #197
	lsls r4, r4, #1
	adds r3, r2, r4
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #3
	beq .L_080ed6dc
.L_080ed678:
	ldr r6, [sp, #16]
	movs r1, #172
	movs r2, #186
	lsls r1, r1, #1
	lsls r2, r2, #2
	adds r3, r6, r1
	adds r2, #255
	movs r0, #10
	strh r2, [r3]
	adds r0, #255
	bl GameFlag_SetBit
	b .L_080ed778
	.2byte 0x0000
.L_080ed694:
	.4byte 0x06003000
.L_080ed698:
	.4byte 0x01810180
.L_080ed69c:
	.4byte 0x01010100
.L_080ed6a0:
	.4byte Data_03001120
.L_080ed6a4:
	.4byte Data_0202a640
.L_080ed6a8:
	.4byte 0x00000e2a
.L_080ed6ac:
	.4byte gInput
.L_080ed6b0:
	.4byte Data_0202a64c
.L_080ed6b4:
	.4byte Data_0202a644
.L_080ed6b8:
	.4byte Data_0202a648
.L_080ed6bc:
	.4byte Data_0202a650
.L_080ed6c0:
	.4byte Data_0202a630
.L_080ed6c4:
	.4byte 0xff880000
.L_080ed6c8:
	.4byte 0xffb00000
.L_080ed6cc:
	.4byte Data_0202a634
.L_080ed6d0:
	.4byte gPartyState
.L_080ed6d4:
	.4byte 0x00000002
.L_080ed6d8:
	.4byte Data_02000500
.L_080ed6dc:
	bl Func_08020270
	ldr r1, .L_080ed76c
	ldr r0, .L_080ed770
	ldrh r3, [r0]
	adds r4, r3, #0
	strh r0, [r0]
	ldrh r3, [r1]
	cmp r3, #31
	bgt .L_080ed714
	lsls r2, r3, #1
	adds r2, r2, r3
	adds r3, #1
	strh r3, [r1]
	ldr r6, [sp, #8]
	lsls r2, r2, #2
	adds r2, r2, r1
	lsls r3, r6, #16
	adds r2, #4
	lsrs r3, r3, #16
	stmia r2!, {r3}
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #82
	stmia r2!, {r3}
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r2]
.L_080ed714:
	strh r4, [r0]
	ldr r1, [sp, #20]
	mov r0, sp
	adds r1, #24
	add r2, sp, #40
	adds r0, #55
.L_080ed720:
	ldrb r3, [r2]
	adds r2, #1
	strh r3, [r1, #10]
	adds r1, #12
	cmp r2, r0
	ble .L_080ed720
	ldr r1, [sp, #12]
	ldr r2, .L_080ed774
	movs r3, #0
	strh r3, [r1, #4]
	strh r3, [r2, #4]
	strh r3, [r2, #6]
	ldr r2, [sp, #20]
	movs r4, #144
	ldr r5, .L_080ed768
	lsls r4, r4, #4
	adds r4, #114
	adds r3, r2, r4
	strb r5, [r3]
	bl Func_080d2a3c
	bl Func_080d2a8c
	ldr r6, [sp, #16]
	ldr r2, [sp, #24]
	movs r1, #218
	lsls r1, r1, #1
	movs r4, #230
	adds r3, r6, r1
	lsls r4, r4, #1
	str r2, [r3]
	adds r3, r6, r4
	ldr r3, [r3]
	adds r3, #91
	strb r5, [r3]
	b .L_080ed778
.L_080ed768:
	.4byte 0x00000000
.L_080ed76c:
	.4byte gIoWriteQueue
.L_080ed770:
	.4byte 0x04000208
.L_080ed774:
	.4byte Data_03001120
.L_080ed778:
	movs r0, #0
	add sp, #56
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
