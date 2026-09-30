.syntax unified
	.thumb
	.global Func_0816c6f8
	.thumb_func
Func_0816c6f8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #152
	str r0, [sp, #84]
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #92]
	movs r0, #2
	str r1, [sp, #80]
	ldr r2, [r3, #96]
	adds r3, #176
	str r2, [sp, #76]
	ldr r3, [r3]
	str r3, [sp, #64]
	bl BattleFx_BeginCanvasLayer
	ldr r3, [sp, #84]
	add r2, sp, #128
	ldr r1, [r3, #4]
	movs r3, #192
	lsls r1, r1, #4
	orrs r1, r3
	ldr r0, [sp, #84]
	add r3, sp, #140
	bl Func_0815585c
	bl Func_0813ba50
	ldr r3, .L_0816c764
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #72
	strh r3, [r2]
	ldr r3, .L_0816c768
	adds r2, #2
	strh r3, [r2]
	ldr r1, [sp, #64]
	movs r3, #1
	str r3, [r1, #16]
	ldr r2, [sp, #84]
	ldr r3, [r2, #4]
	cmp r3, #0
	bne .L_0816c76c
	movs r0, #104
	movs r1, #3
	bl Func_081963ec
	b .L_0816c774
	.2byte 0x0000
.L_0816c764:
	.4byte 0x00003537
.L_0816c768:
	.4byte 0x00003f31
.L_0816c76c:
	movs r0, #104
	movs r1, #7
	bl Func_081963ec
.L_0816c774:
	movs r5, #192
	lsls r5, r5, #18
	ldr r3, [r5, #104]
	movs r2, #128
	str r3, [sp, #68]
	ldr r3, .L_0816c7b8
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	ldr r3, .L_0816c7bc
	subs r2, #2
	strh r3, [r2]
	ldr r1, [sp, #80]
	movs r3, #239
	lsls r3, r3, #7
	adds r2, r1, r3
	movs r3, #1
	str r3, [r2]
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #132
	adds r2, r1, r3
	movs r1, #185
	movs r3, #0
	lsls r1, r1, #4
	str r3, [r2]
	adds r1, #255
	ldr r0, .L_0816c7c0
	bl Scheduler_AddOrUpdateCallback
	movs r1, #192
	lsls r1, r1, #4
	b .L_0816c7c4
	.2byte 0x0000
.L_0816c7b8:
	.4byte 0x00001010
.L_0816c7bc:
	.4byte 0x00000000
.L_0816c7c0:
	.4byte Func_08143000
.L_0816c7c4:
	adds r1, #255
	ldr r0, .L_0816c924
	bl Scheduler_AddOrUpdateCallback
	ldr r1, [sp, #80]
	movs r2, #224
	lsls r2, r2, #3
	adds r2, r1, r2
	movs r3, #240
	str r2, [sp, #60]
	lsls r3, r3, #4
	movs r2, #152
	adds r3, r1, r3
	lsls r2, r2, #5
	str r3, [sp, #56]
	adds r2, r1, r2
	movs r3, #0
	str r2, [sp, #52]
	str r3, [sp, #40]
	ldr r1, [sp, #84]
	ldr r5, [r5, #36]
	movs r7, #0
	str r5, [sp, #36]
	ldr r0, [r1, #8]
	bl GetBattleObjectSlotFar
	ldr r2, .L_0816c928
	ldr r0, [r0]
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #200
	ldr r1, .L_0816c92c
	str r0, [sp, #32]
	strh r3, [r2, #4]
	ldr r3, .L_0816c930
	movs r2, #7
	mov r10, r2
	mov r8, r1
.L_0816c810:
	adds r5, r7, #0
	mov r2, r10
	movs r6, #0
	ands r5, r2
.L_0816c818:
	adds r1, r7, #0
	cmp r7, #0
	bge .L_0816c820
	adds r1, r7, #7
.L_0816c820:
	asrs r1, r1, #3
	lsls r1, r1, #8
	ldr r2, .L_0816c934
	adds r1, r1, r5
	lsls r1, r1, #3
	adds r1, r1, r6
	adds r1, r1, r2
	adds r0, r3, #0
	str r3, [sp, #12]
	movs r2, #8
	mov lr, r8
	.2byte 0xf800
	movs r1, #224
	ldr r3, [sp, #12]
	lsls r1, r1, #3
	adds r6, #64
	adds r1, #255
	adds r3, #8
	cmp r6, r1
	ble .L_0816c818
	adds r7, #1
	cmp r7, #119
	ble .L_0816c810
	ldr r2, [sp, #80]
	movs r3, #216
	lsls r3, r3, #7
	adds r3, #192
	adds r1, r2, r3
	ldr r0, .L_0816c938
	movs r2, #1
	movs r3, #1
	bl Func_08157cf4
	movs r3, #156
	ldr r2, [sp, #80]
	lsls r3, r3, #7
	adds r3, #16
	adds r1, r2, r3
	ldr r0, .L_0816c93c
	movs r2, #0
	movs r3, #0
	bl Func_08157cf4
	ldr r0, .L_0816c940
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_0816c92c
	movs r2, #128
	lsls r0, r0, #19
	mov lr, r3
	.2byte 0xf800
	ldr r1, .L_0816c944
	ldr r5, .L_0816c948
	ldrh r3, [r5]
	adds r0, r3, #0
	strh r5, [r5]
	ldrh r2, [r1]
	cmp r2, #31
	bgt .L_0816c8b8
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	adds r2, #1
	adds r3, r3, r1
	adds r3, #4
	strh r2, [r1]
	movs r2, #128
	stmia r3!, {r2}
	lsls r2, r2, #19
	adds r2, #32
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #10
	str r2, [r3]
.L_0816c8b8:
	strh r0, [r5]
	ldr r0, [sp, #32]
	movs r1, #48
	bl ObjectDispatch_ApplyValueToChildrenFar
	movs r3, #8
	str r3, [sp, #0]
	movs r1, #16
	movs r2, #16
	movs r3, #16
	ldr r0, [sp, #52]
	bl Func_0818de3c
	mov r2, sp
	mov r3, sp
	adds r2, #116
	adds r3, #104
	str r2, [sp, #24]
	str r3, [sp, #28]
	movs r1, #0
	mov r11, r1
.L_0816c8e2:
	ldr r1, [sp, #84]
	ldr r0, [r1, #8]
	ldr r1, [sp, #24]
	bl Func_0815e21c
	ldr r3, [sp, #84]
	ldr r1, [sp, #28]
	movs r2, #36
	ldrsh r0, [r3, r2]
	bl Func_0815e21c
	mov r1, r11
	cmp r1, #11
	bgt .L_0816c9b2
	ldr r2, [sp, #36]
	movs r3, #160
	lsls r3, r3, #3
	adds r3, #108
	adds r2, r2, r3
	mov r12, r2
	movs r1, #160
	ldr r2, .L_0816c920
	lsls r1, r1, #19
	adds r1, #192
	movs r3, #31
	mov r8, r1
	movs r7, #0
	mov lr, r2
	mov r10, r3
	b .L_0816c94c
	.2byte 0x0000
.L_0816c920:
	.4byte 0x0000001f
.L_0816c924:
	.4byte Func_08143424
.L_0816c928:
	.4byte Data_03001120
.L_0816c92c:
	.4byte IwramCopyWords
.L_0816c930:
	.4byte gMapCellBuffer
.L_0816c934:
	.4byte 0x06008000
.L_0816c938:
	.4byte 0x00000157
.L_0816c93c:
	.4byte 0x000000da
.L_0816c940:
	.4byte 0x00000148
.L_0816c944:
	.4byte gIoWriteQueue
.L_0816c948:
	.4byte 0x04000208
.L_0816c94c:
	mov r1, r12
	ldrh r3, [r1]
	mov r4, r10
	ands r4, r3
	mov r1, r8
	lsls r3, r3, #16
	lsrs r5, r3, #21
	lsrs r6, r3, #26
	ldrh r3, [r1]
	mov r0, r10
	mov r2, lr
	ands r0, r3
	lsls r3, r3, #16
	mov r1, lr
	ands r5, r2
	ands r6, r2
	lsrs r2, r3, #21
	ands r2, r1
	lsrs r1, r3, #26
	mov r3, lr
	ands r1, r3
	adds r3, r4, #3
	cmp r0, r3
	ble .L_0816c980
	subs r0, #4
	b .L_0816c982
.L_0816c980:
	adds r0, r4, #0
.L_0816c982:
	adds r3, r5, #3
	cmp r2, r3
	ble .L_0816c98c
	subs r2, #4
	b .L_0816c98e
.L_0816c98c:
	adds r2, r5, #0
.L_0816c98e:
	adds r3, r6, #3
	cmp r1, r3
	ble .L_0816c998
	subs r1, #4
	b .L_0816c99a
.L_0816c998:
	adds r1, r6, #0
.L_0816c99a:
	lsls r3, r1, #10
	lsls r2, r2, #5
	orrs r3, r2
	mov r1, r8
	movs r2, #2
	orrs r3, r0
	adds r7, #1
	strh r3, [r1]
	add r12, r2
	add r8, r2
	cmp r7, #128
	bne .L_0816c94c
.L_0816c9b2:
	mov r3, r11
	cmp r3, #1
	bne .L_0816c9f6
	ldr r1, .L_0816ca98
	ldr r2, .L_0816ca9c
	ldrh r3, [r2]
	adds r0, r3, #0
	movs r3, #130
	lsls r3, r3, #2
	strh r3, [r2]
	ldrh r2, [r1]
	cmp r2, #31
	bgt .L_0816c9ee
	lsls r3, r2, #1
	adds r3, r3, r2
	adds r2, #1
	lsls r3, r3, #2
	strh r2, [r1]
	movs r2, #234
	adds r3, r3, r1
	lsls r2, r2, #7
	adds r3, #4
	adds r2, #65
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #19
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #10
	str r2, [r3]
.L_0816c9ee:
	ldr r3, .L_0816ca9c
	strh r0, [r3]
	bl Func_08143354
.L_0816c9f6:
	mov r1, r11
	cmp r1, #34
	bne .L_0816cab0
	ldr r0, .L_0816caa0
	bl Scheduler_RemoveCallback
	movs r1, #185
	lsls r1, r1, #4
	adds r1, #255
	ldr r0, .L_0816caa4
	bl Scheduler_AddOrUpdateCallback
	ldr r1, .L_0816ca98
	ldr r2, .L_0816ca9c
	ldrh r3, [r2]
	adds r0, r3, #0
	movs r3, #130
	lsls r3, r3, #2
	strh r3, [r2]
	ldrh r2, [r1]
	cmp r2, #31
	bgt .L_0816ca44
	lsls r3, r2, #1
	adds r3, r3, r2
	adds r2, #1
	lsls r3, r3, #2
	strh r2, [r1]
	movs r2, #230
	adds r3, r3, r1
	lsls r2, r2, #7
	adds r3, #4
	adds r2, #65
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #19
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #10
	str r2, [r3]
.L_0816ca44:
	ldr r3, .L_0816ca9c
	strh r0, [r3]
	ldr r3, .L_0816caa8
	movs r1, #240
	lsls r1, r1, #6
	ldr r0, [sp, #76]
	mov lr, r3
	.2byte 0xf800
	ldr r1, .L_0816caac
	movs r3, #78
	strh r3, [r1, #6]
	ldr r3, [sp, #24]
	ldr r2, [r3]
	ldr r3, .L_0816ca88
	subs r3, r3, r2
	strh r3, [r1, #4]
	movs r1, #128
	lsls r1, r1, #19
	adds r1, #10
	ldrh r3, [r1]
	ldr r2, .L_0816ca8c
	ands r3, r2
	strh r3, [r1]
	ldr r2, .L_0816ca90
	ldrh r3, [r1]
	orrs r3, r2
	strh r3, [r1]
	movs r2, #128
	ldr r3, .L_0816ca94
	lsls r2, r2, #19
	adds r2, #12
	strh r3, [r2]
	b .L_0816cab0
	.2byte 0x0000
.L_0816ca88:
	.4byte 0x00000040
.L_0816ca8c:
	.4byte 0x0000fffc
.L_0816ca90:
	.4byte 0x00000001
.L_0816ca94:
	.4byte 0x00000784
.L_0816ca98:
	.4byte gIoWriteQueue
.L_0816ca9c:
	.4byte 0x04000208
.L_0816caa0:
	.4byte Func_08143000
.L_0816caa4:
	.4byte Func_08143264
.L_0816caa8:
	.4byte IwramClearWords
.L_0816caac:
	.4byte Data_03001120
.L_0816cab0:
	mov r1, r11
	cmp r1, #35
	beq .L_0816cab8
	b .L_0816cbaa
.L_0816cab8:
	ldr r0, .L_0816cc38
	bl Scheduler_RemoveCallback
	ldr r0, .L_0816cc3c
	bl Scheduler_RemoveCallback
	movs r1, #185
	lsls r1, r1, #4
	adds r1, #255
	ldr r0, .L_0816cc40
	bl Scheduler_AddOrUpdateCallback
	ldr r1, .L_0816cc44
	ldr r2, .L_0816cc48
	ldrh r3, [r2]
	adds r0, r3, #0
	movs r3, #130
	lsls r3, r3, #2
	strh r3, [r2]
	ldrh r2, [r1]
	cmp r2, #31
	bgt .L_0816cb04
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	adds r2, #1
	adds r3, r3, r1
	adds r3, #4
	strh r2, [r1]
	movs r2, #0
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #40
	stmia r3!, {r2}
	movs r2, #192
	lsls r2, r2, #10
	str r2, [r3]
.L_0816cb04:
	ldr r3, .L_0816cc48
	strh r0, [r3]
	ldrh r3, [r3]
	adds r0, r3, #0
	movs r2, #130
	ldr r3, .L_0816cc48
	lsls r2, r2, #2
	strh r2, [r3]
	ldrh r2, [r1]
	cmp r2, #31
	bgt .L_0816cb3a
	lsls r3, r2, #1
	adds r3, r3, r2
	adds r2, #1
	lsls r3, r3, #2
	strh r2, [r1]
	ldr r2, .L_0816cc4c
	adds r3, r3, r1
	adds r3, #4
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #44
	stmia r3!, {r2}
	movs r2, #192
	lsls r2, r2, #10
	str r2, [r3]
.L_0816cb3a:
	ldr r2, .L_0816cc48
	strh r0, [r2]
	ldrh r3, [r2]
	adds r0, r3, #0
	movs r3, #130
	lsls r3, r3, #2
	strh r3, [r2]
	ldrh r2, [r1]
	cmp r2, #31
	bgt .L_0816cb6c
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	adds r2, #1
	adds r3, r3, r1
	adds r3, #4
	strh r2, [r1]
	movs r2, #128
	stmia r3!, {r2}
	lsls r2, r2, #19
	adds r2, #32
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #10
	str r2, [r3]
.L_0816cb6c:
	ldr r3, .L_0816cc48
	strh r0, [r3]
	ldrh r3, [r3]
	adds r0, r3, #0
	movs r2, #130
	ldr r3, .L_0816cc48
	lsls r2, r2, #2
	strh r2, [r3]
	ldrh r2, [r1]
	cmp r2, #31
	bgt .L_0816cba6
	lsls r3, r2, #1
	adds r3, r3, r2
	adds r2, #1
	lsls r3, r3, #2
	strh r2, [r1]
	movs r2, #252
	adds r3, r3, r1
	lsls r2, r2, #6
	adds r3, #4
	adds r2, #68
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #80
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #10
	str r2, [r3]
.L_0816cba6:
	ldr r1, .L_0816cc48
	strh r0, [r1]
.L_0816cbaa:
	mov r2, r11
	cmp r2, #18
	bne .L_0816cbc2
	ldr r3, [sp, #84]
	movs r1, #1
	ldr r0, [r3, #8]
	negs r1, r1
	movs r2, #2
	movs r3, #0
	str r1, [sp, #0]
	bl Func_0814cd48
.L_0816cbc2:
	mov r1, r11
	cmp r1, #36
	bne .L_0816cc18
	ldr r3, [sp, #80]
	movs r1, #239
	lsls r1, r1, #7
	adds r2, r3, r1
	movs r3, #2
	str r3, [r2]
	ldr r3, [sp, #80]
	adds r1, #4
	adds r2, r3, r1
	ldr r1, .L_0816cc44
	movs r3, #50
	str r3, [r2]
	ldr r2, .L_0816cc48
	ldrh r3, [r2]
	adds r0, r3, #0
	movs r3, #130
	lsls r3, r3, #2
	strh r3, [r2]
	ldrh r2, [r1]
	cmp r2, #31
	bgt .L_0816cc14
	lsls r3, r2, #1
	adds r3, r3, r2
	adds r2, #1
	lsls r3, r3, #2
	strh r2, [r1]
	movs r2, #238
	adds r3, r3, r1
	lsls r2, r2, #7
	adds r3, #4
	adds r2, #65
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #19
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #10
	str r2, [r3]
.L_0816cc14:
	ldr r3, .L_0816cc48
	strh r0, [r3]
.L_0816cc18:
	mov r3, r11
	subs r3, #34
	cmp r3, #5
	bhi .L_0816cc50
	ldr r1, [sp, #84]
	ldr r3, [r1, #4]
	cmp r3, #0
	bne .L_0816cc2c
	ldr r2, .L_0816cc30
	b .L_0816cc58
.L_0816cc2c:
	ldr r2, .L_0816cc34
	b .L_0816cc58
.L_0816cc30:
	.4byte 0x000020f0
.L_0816cc34:
	.4byte 0x000000d0
.L_0816cc38:
	.4byte Func_08143424
.L_0816cc3c:
	.4byte Func_08143264
.L_0816cc40:
	.4byte Func_08143000
.L_0816cc44:
	.4byte gIoWriteQueue
.L_0816cc48:
	.4byte 0x04000208
.L_0816cc4c:
	.4byte 0xfffff000
.L_0816cc50:
	mov r2, r11
	cmp r2, #40
	bne .L_0816cc64
	ldr r2, .L_0816cc78
.L_0816cc58:
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #64
	strh r2, [r3]
	adds r3, #2
	strh r2, [r3]
.L_0816cc64:
	mov r0, r11
	subs r0, #36
	cmp r0, #25
	bhi .L_0816cd22
	adds r6, r0, #0
	cmp r6, #12
	ble .L_0816cc7c
	movs r6, #12
	b .L_0816cc7c
	.2byte 0x0000
.L_0816cc78:
	.4byte 0x000000f0
.L_0816cc7c:
	ldr r1, [sp, #28]
	ldr r2, [sp, #24]
	ldr r3, [r1]
	ldr r5, [r2]
	movs r1, #12
	subs r3, r3, r5
	adds r0, r6, #0
	muls r0, r3
	bl Math_Div
	movs r1, #12
	adds r7, r5, r0
	lsls r0, r6, #6
	bl Math_Div
	adds r0, #16
	mov r9, r0
	lsls r0, r6, #1
	ldr r3, .L_0816ccd0
	adds r0, r0, r6
	ldr r5, .L_0816ccd4
	lsls r0, r0, #3
	subs r0, r0, r6
	subs r3, r3, r7
	lsls r0, r0, #1
	strh r3, [r5, #4]
	movs r1, #12
	negs r0, r0
	bl Math_Div
	adds r0, #78
	strh r0, [r5, #6]
	ldr r1, [sp, #84]
	ldr r3, [r1, #4]
	cmp r3, #0
	bne .L_0816ccd8
	lsrs r3, r7, #31
	adds r3, r7, r3
	asrs r3, r3, #1
	adds r7, r3, #0
	adds r7, #20
	b .L_0816cce2
.L_0816ccd0:
	.4byte 0x00000040
.L_0816ccd4:
	.4byte Data_03001120
.L_0816ccd8:
	lsrs r3, r7, #31
	adds r3, r7, r3
	asrs r3, r3, #1
	adds r7, r3, #0
	subs r7, #20
.L_0816cce2:
	ldr r2, [sp, #80]
	movs r3, #216
	lsls r3, r3, #7
	ldr r4, [sp, #68]
	adds r3, #192
	movs r1, #48
	adds r2, r2, r3
	negs r1, r1
	subs r7, #10
	mov r8, r2
	add r9, r1
	mov r10, r7
	movs r6, #20
	movs r5, #40
	mov r1, r8
	mov r2, r10
	mov r3, r9
	str r4, [sp, #8]
	str r6, [sp, #0]
	str r5, [sp, #4]
	ldr r0, [sp, #76]
	mov lr, r4
	.2byte 0xf800
	mov r1, r8
	str r6, [sp, #0]
	str r5, [sp, #4]
	ldr r0, [sp, #76]
	mov r2, r10
	mov r3, r9
	ldr r4, [sp, #8]
	mov lr, r4
	.2byte 0xf800
.L_0816cd22:
	mov r2, r11
	cmp r2, #34
	bls .L_0816cd2a
	b .L_0816cefa
.L_0816cd2a:
	str r2, [sp, #44]
	cmp r2, #16
	ble .L_0816cd34
	movs r3, #16
	str r3, [sp, #44]
.L_0816cd34:
	ldr r1, [sp, #44]
	movs r3, #64
	lsls r2, r1, #1
	subs r7, r3, r2
	movs r6, #16
	mov r2, r11
	movs r0, #120
	negs r6, r6
	cmp r2, #0
	blt .L_0816cd72
	cmp r2, #16
	ble .L_0816cd4e
	movs r2, #16
.L_0816cd4e:
	ldr r1, [sp, #24]
	ldr r3, [r1]
	subs r3, #120
	muls r3, r2
	cmp r3, #0
	bge .L_0816cd5c
	adds r3, #15
.L_0816cd5c:
	asrs r3, r3, #4
	adds r0, r3, #0
	movs r3, #46
	muls r3, r2
	adds r0, #120
	cmp r3, #0
	bge .L_0816cd6c
	adds r3, #15
.L_0816cd6c:
	asrs r3, r3, #4
	adds r6, r3, #0
	subs r6, #16
.L_0816cd72:
	ldr r2, [sp, #80]
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #208
	adds r1, r7, #0
	lsls r0, r0, #5
	adds r5, r2, r3
	bl Math_Div
	movs r3, #64
	subs r3, r3, r0
	lsls r3, r3, #8
	str r3, [r5]
	ldr r1, [sp, #80]
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #212
	adds r2, r1, r3
	movs r0, #128
	lsls r3, r6, #8
	str r3, [r2]
	adds r1, r7, #0
	lsls r0, r0, #6
	bl Math_Div
	movs r2, #238
	ldr r1, [sp, #80]
	lsls r2, r2, #7
	adds r2, #216
	adds r3, r1, r2
	ldrh r2, [r3]
	strh r0, [r3]
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #218
	adds r2, r1, r3
	ldrh r3, [r2]
	movs r3, #128
	movs r1, #0
	lsls r3, r3, #1
	strh r3, [r2]
	movs r2, #64
	str r1, [sp, #48]
	str r1, [sp, #20]
	negs r2, r2
	mov r9, r2
	mov r10, r1
.L_0816cdd0:
	mov r0, r10
	bl Trig_Cos
	negs r0, r0
	lsls r5, r0, #1
	adds r5, r5, r0
	mov r0, r10
	bl Trig_Sin
	lsls r3, r0, #1
	adds r3, r3, r0
	lsls r3, r3, #4
	lsls r5, r5, #4
	ldr r1, [sp, #44]
	asrs r5, r5, #16
	asrs r2, r3, #16
	mov r3, r9
	subs r5, r5, r3
	adds r3, r1, #0
	muls r3, r5
	cmp r3, #0
	bge .L_0816cdfe
	adds r3, #15
.L_0816cdfe:
	asrs r3, r3, #4
	add r3, r9
	mov r8, r3
	adds r3, r2, #0
	ldr r2, [sp, #44]
	subs r3, #56
	muls r3, r2
	cmp r3, #0
	bge .L_0816ce12
	adds r3, #15
.L_0816ce12:
	ldr r1, [sp, #20]
	asrs r3, r3, #4
	adds r2, r3, #0
	lsls r3, r1, #2
	ldr r1, [sp, #56]
	adds r2, #56
	movs r7, #0
	adds r6, r3, r1
.L_0816ce22:
	lsls r5, r7, #12
	adds r0, r5, #0
	str r2, [sp, #16]
	bl Trig_Sin
	ldr r2, [sp, #16]
	adds r7, #1
	adds r3, r2, #0
	muls r3, r0
	asrs r3, r3, #16
	strb r3, [r6]
	mov r3, r8
	strb r3, [r6, #1]
	adds r0, r5, #0
	bl Trig_Cos
	ldr r2, [sp, #16]
	adds r3, r2, #0
	muls r3, r0
	asrs r3, r3, #16
	strb r3, [r6, #2]
	adds r6, #4
	cmp r7, #16
	bne .L_0816ce22
	movs r1, #8
	ldr r3, [sp, #20]
	add r9, r1
	ldr r1, [sp, #48]
	movs r2, #128
	lsls r2, r2, #4
	adds r2, #136
	adds r3, #16
	adds r1, #1
	add r10, r2
	str r3, [sp, #20]
	str r1, [sp, #48]
	cmp r1, #16
	bne .L_0816cdd0
	movs r0, #1
	bl Func_081969f8
	ldr r2, .L_0816d0fc
	ldr r3, [sp, #96]
	movs r1, #8
	ands r3, r2
	ldr r2, .L_0816d100
	orrs r3, r1
	ands r3, r2
	movs r2, #224
	lsls r2, r2, #3
	orrs r3, r2
	str r3, [sp, #96]
	ldr r3, .L_0816d104
	add r2, sp, #96
	adds r5, r0, #0
	str r3, [r2, #4]
	movs r3, #3
	str r3, [r5, #4]
	str r1, [r5]
	str r2, [r5, #16]
	ldr r1, [sp, #52]
	movs r3, #0
	str r1, [r5, #8]
	ldr r2, [sp, #60]
	strb r3, [r5, #24]
	strb r3, [r5, #25]
	str r2, [r5, #12]
	bl Func_08014de4
	mov r3, r11
	cmp r3, #7
	ble .L_0816ceda
	lsls r0, r3, #3
	subs r0, r0, r3
	lsls r3, r0, #5
	ldr r1, .L_0816d108
	adds r0, r0, r3
	lsls r0, r0, #2
	adds r0, r0, r1
	bl Trig_Sin
	ldr r2, [sp, #40]
	asrs r0, r0, #5
	adds r2, r2, r0
	lsls r0, r2, #1
	adds r0, r0, r2
	lsrs r3, r0, #31
	adds r0, r0, r3
	asrs r0, r0, #1
	str r2, [sp, #40]
	bl SceneTransform_ApplyPitch
.L_0816ceda:
	mov r3, r11
	lsls r0, r3, #9
	bl Func_08015068
	movs r2, #128
	ldr r0, [sp, #56]
	ldr r1, [sp, #60]
	lsls r2, r2, #1
	bl Func_08196958
	adds r0, r5, #0
	bl Func_08196a7c
	adds r0, r5, #0
	bl Sys_Free
.L_0816cefa:
	mov r1, r11
	cmp r1, #48
	bne .L_0816cf14
	movs r0, #145
	bl Audio_PlayCue
	movs r1, #128
	ldr r3, .L_0816d10c
	ldr r0, [sp, #76]
	lsls r1, r1, #7
	ldr r2, .L_0816d110
	mov lr, r3
	.2byte 0xf800
.L_0816cf14:
	mov r2, r11
	cmp r2, #64
	bne .L_0816cf20
	movs r0, #134
	bl Func_081180e8
.L_0816cf20:
	mov r3, r11
	cmp r3, #47
	ble .L_0816d022
	movs r0, #128
	lsls r0, r0, #2
	bl Runtime_BumpAllocateAlternatePool
	mov r8, r0
	movs r0, #1
	bl Func_081969f8
	ldr r2, .L_0816d0fc
	ldr r3, [sp, #88]
	ldr r1, [sp, #80]
	ands r3, r2
	movs r2, #6
	orrs r3, r2
	ldr r2, .L_0816d100
	adds r6, r0, #0
	ands r3, r2
	movs r2, #192
	lsls r2, r2, #3
	orrs r3, r2
	movs r2, #156
	lsls r2, r2, #7
	adds r2, #16
	str r3, [sp, #88]
	adds r3, r1, r2
	add r2, sp, #88
	str r3, [r2, #4]
	movs r3, #7
	str r3, [r6]
	ldr r3, .L_0816d114
	str r2, [r6, #16]
	str r3, [r6, #8]
	mov r3, r8
	str r3, [r6, #12]
	movs r3, #0
	strb r3, [r6, #24]
	strb r3, [r6, #25]
	movs r7, #0
.L_0816cf72:
	ldr r3, .L_0816d118
	ldrb r3, [r3, r7]
	adds r1, r3, #0
	adds r1, #48
	cmp r11, r1
	ble .L_0816d010
	mov r3, r11
	subs r2, r3, r1
	ldr r3, .L_0816d11c
	ldrb r3, [r3, r7]
	muls r2, r3
	lsls r3, r2, #5
	subs r3, r3, r2
	lsls r3, r3, #2
	adds r3, r3, r2
	movs r2, #128
	lsls r3, r3, #4
	lsls r2, r2, #8
	adds r5, r3, r2
	mov r2, r11
	subs r3, r1, r2
	lsls r3, r3, #3
	adds r3, #56
	cmp r3, #0
	ble .L_0816cfa6
	movs r3, #0
.L_0816cfa6:
	movs r1, #64
	negs r1, r1
	cmp r3, r1
	ble .L_0816d010
	str r3, [r6, #20]
	bl Func_08014de4
	ldr r2, [sp, #28]
	ldr r0, [r2]
	movs r2, #0
	lsrs r3, r0, #31
	adds r0, r0, r3
	ldr r3, .L_0816d120
	asrs r0, r0, #1
	ldrsb r1, [r3, r7]
	subs r0, #64
	lsls r0, r0, #16
	lsls r1, r1, #16
	bl Func_08015160
	asrs r2, r5, #1
	adds r0, r2, #0
	lsls r1, r5, #2
	bl Func_080151e4
	movs r0, #250
	lsls r0, r0, #2
	bl SceneTransform_ApplyPitch
	movs r3, #1
	ands r3, r7
	cmp r3, #0
	beq .L_0816cff4
	lsls r0, r7, #2
	add r0, r11
	lsls r0, r0, #11
	bl Func_08015068
	b .L_0816d000
.L_0816cff4:
	lsls r0, r7, #2
	mov r3, r11
	subs r0, r0, r3
	lsls r0, r0, #11
	bl Func_08015068
.L_0816d000:
	ldr r0, .L_0816d124
	mov r1, r8
	movs r2, #32
	bl Func_08196958
	adds r0, r6, #0
	bl Func_08196a7c
.L_0816d010:
	adds r7, #1
	cmp r7, #4
	bne .L_0816cf72
	adds r0, r6, #0
	bl Sys_Free
	mov r0, r8
	bl Sys_Free
.L_0816d022:
	mov r1, r11
	cmp r1, #61
	bne .L_0816d040
	ldr r2, [sp, #64]
	movs r0, #160
	movs r3, #0
	lsls r0, r0, #19
	movs r1, #128
	str r3, [r2, #16]
	adds r0, #192
	ldr r3, .L_0816d10c
	lsls r1, r1, #1
	movs r2, #0
	mov lr, r3
	.2byte 0xf800
.L_0816d040:
	mov r3, r11
	cmp r3, #62
	bne .L_0816d05a
	ldr r1, [sp, #36]
	movs r2, #206
	lsls r2, r2, #3
	adds r3, r1, r2
	movs r2, #1
	ldrh r1, [r3]
	movs r0, #1
	negs r2, r2
	bl Func_08118040
.L_0816d05a:
	mov r3, r11
	subs r3, #63
	cmp r3, #4
	bhi .L_0816d08c
	ldr r1, [sp, #36]
	movs r2, #192
	lsls r2, r2, #3
	adds r2, #108
	adds r3, r1, r2
	mov r1, r11
	lsls r2, r1, #13
	ldr r1, .L_0816d128
	adds r2, r2, r1
	str r2, [r3]
	ldr r3, [sp, #36]
	movs r1, #160
	lsls r1, r1, #3
	adds r1, #108
	adds r0, r3, r1
	movs r1, #160
	lsls r1, r1, #19
	adds r1, #192
	movs r3, #128
	bl ColorBuffer_ScaleFar
.L_0816d08c:
	mov r2, r11
	cmp r2, #48
	bne .L_0816d0a0
	ldr r3, [sp, #80]
	movs r1, #238
	lsls r1, r1, #7
	adds r1, #168
	adds r2, r3, r1
	movs r3, #16
	str r3, [r2]
.L_0816d0a0:
	mov r2, r11
	cmp r2, #47
	ble .L_0816d0ae
	movs r0, #8
	movs r1, #8
	bl Func_08158ce0
.L_0816d0ae:
	bl Func_081434f8
	movs r1, #240
	ldr r3, [sp, #80]
	lsls r1, r1, #7
	adds r1, #232
	adds r2, r3, r1
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	movs r2, #1
	add r11, r2
	mov r3, r11
	cmp r3, #74
	beq .L_0816d0d4
	bl .L_0816c8e2
.L_0816d0d4:
	ldr r0, [sp, #32]
	movs r1, #16
	bl ObjectDispatch_ApplyValueToChildrenFar
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	ldr r0, .L_0816d12c
	bl Scheduler_RemoveCallback
	bl Func_08143bb8
	add sp, #152
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0816d0fc:
	.4byte 0xffffff00
.L_0816d100:
	.4byte 0xffff00ff
.L_0816d104:
	.4byte gMapCellBuffer
.L_0816d108:
	.4byte 0xffffe320
.L_0816d10c:
	.4byte IwramFillWords
.L_0816d110:
	.4byte 0x3f3f3f3f
.L_0816d114:
	.4byte Data_081990d0
.L_0816d118:
	.4byte Data_08198b52
.L_0816d11c:
	.4byte Data_08198b56
.L_0816d120:
	.4byte Data_08198b5a
.L_0816d124:
	.4byte Data_08199050
.L_0816d128:
	.4byte 0xfff84000
.L_0816d12c:
	.4byte Func_08143000
