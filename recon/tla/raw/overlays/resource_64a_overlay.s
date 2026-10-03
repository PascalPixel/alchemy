.syntax unified
	.thumb
	.section .text.x0200805c,"ax",%progbits
	.global Func_0200005c
	.thumb_func
Func_0200005c:
	push {r5, r6, lr}
	movs r0, #0
	ldr r5, .L_020080a4
	bl Blend_SetDarkenTarget16
	ldr r3, .L_020080a0
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #12
	strh r3, [r2]
	ldr r2, .L_020080a8
	movs r3, #0
	strh r3, [r2, #10]
	adds r0, r5, #0
	bl Resource_GetTableEntry
	movs r6, #128
	movs r3, #128
	movs r2, #132
	lsls r6, r6, #1
	lsls r3, r3, #19
	movs r1, #160
	lsls r2, r2, #24
	adds r6, #255
	adds r4, r0, #0
	adds r3, #212
	lsls r1, r1, #19
	adds r2, #112
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r2, #224
	lsls r2, r2, #1
	adds r4, r4, r2
	b .L_020080ac
.L_020080a0:
	.4byte 0x00000681
.L_020080a4:
	.4byte 0x00000022
.L_020080a8:
	.4byte Data_03001120
.L_020080ac:
	adds r0, r4, #0
	ldr r1, .L_02008144
	bl Func_020021f8
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #212
	ldr r0, .L_02008144
	ldr r1, .L_02008148
	ldr r2, .L_0200814c
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r1, .L_02008150
	movs r3, #208
	lsls r3, r3, #1
	movs r0, #0
.L_020080cc:
	movs r4, #0
.L_020080ce:
	adds r2, r3, #0
	movs r5, #128
	lsls r3, r2, #16
	lsls r5, r5, #9
	adds r3, r3, r5
	adds r4, #1
	strh r2, [r1]
	asrs r3, r3, #16
	adds r1, #2
	cmp r4, #29
	bls .L_020080ce
	strh r6, [r1]
	adds r0, #1
	adds r1, #2
	strh r6, [r1]
	adds r1, #2
	cmp r0, #19
	bls .L_020080cc
	ldr r2, .L_02008154
	movs r0, #0
.L_020080f6:
	movs r3, #0
	adds r0, #1
	strh r3, [r2, #2]
	strh r3, [r2]
	adds r2, #4
	cmp r0, #3
	bls .L_020080f6
	movs r3, #128
	movs r1, #128
	movs r2, #132
	lsls r3, r3, #19
	lsls r1, r1, #19
	lsls r2, r2, #24
	adds r3, #212
	ldr r0, .L_02008154
	adds r1, #16
	adds r2, #4
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #32]
	movs r3, #160
	lsls r3, r3, #5
	strh r3, [r2, #20]
	ldr r3, .L_02008158
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	ldr r5, .L_02008140
	adds r0, #85
	strb r5, [r0]
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02008140:
	.4byte 0x00000000
.L_02008144:
	.4byte gMapCellBuffer
.L_02008148:
	.4byte 0x06006800
.L_0200814c:
	.4byte 0x84002580
.L_02008150:
	.4byte 0x06003000
.L_02008154:
	.4byte Data_03001120
.L_02008158:
	.4byte gPartyState
	.section .text.x0200815c,"ax",%progbits
	.global Func_0200015c
	.thumb_func
Func_0200015c:
	push {r5, lr}
	ldr r3, .L_020081e4
	ldr r1, .L_020081e8
	ldrh r2, [r3]
	adds r2, #1
	strh r2, [r3]
	lsls r2, r2, #16
	lsrs r5, r2, #17
	ldr r0, .L_020081ec
	ldrh r3, [r0]
	adds r4, r3, #0
	strh r0, [r0]
	ldrh r2, [r1]
	cmp r2, #31
	bgt .L_0200819e
	lsls r3, r2, #1
	adds r3, r3, r2
	adds r2, #1
	lsls r3, r3, #2
	strh r2, [r1]
	movs r2, #184
	adds r3, r3, r1
	lsls r2, r2, #6
	adds r3, #4
	adds r2, #81
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #80
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #10
	str r2, [r3]
.L_0200819e:
	strh r4, [r0]
	ldrh r3, [r0]
	adds r4, r3, #0
	strh r0, [r0]
	ldrh r3, [r1]
	cmp r3, #31
	bgt .L_020081d4
	lsls r2, r3, #1
	adds r2, r2, r3
	lsls r2, r2, #2
	adds r3, #1
	adds r2, r2, r1
	strh r3, [r1]
	adds r1, r5, #0
	movs r3, #16
	subs r3, r3, r1
	lsls r3, r3, #8
	adds r2, #4
	orrs r3, r1
	stmia r2!, {r3}
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #82
	stmia r2!, {r3}
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r2]
.L_020081d4:
	strh r4, [r0]
	cmp r5, #15
	bls .L_020081e0
	ldr r0, .L_020081f0
	bl Scheduler_RemoveCallbackFar
.L_020081e0:
	pop {r5, pc}
	.2byte 0x0000
.L_020081e4:
	.4byte Data_020024f4
.L_020081e8:
	.4byte gIoWriteQueue
.L_020081ec:
	.4byte 0x04000208
.L_020081f0:
	.4byte Func_0200015c
	.section .text.x020081f4,"ax",%progbits
	.global Func_020001f4
	.thumb_func
Func_020001f4:
	push {r5, r6, r7, lr}
	ldr r1, .L_02008254
	movs r2, #0
	ldrsh r3, [r1, r2]
	mov r12, r1
	cmp r3, #0
	bne .L_0200826a
	ldr r6, .L_02008258
	ldr r4, .L_0200825c
	movs r3, #0
	ldrsh r5, [r6, r3]
	cmp r5, #0
	beq .L_02008218
	ldr r3, [r4]
	cmp r3, #0
	bne .L_0200826a
	strh r3, [r6]
	b .L_0200826a
.L_02008218:
	ldr r3, [r4]
	cmp r3, #0
	beq .L_0200826a
	ldr r1, .L_02008260
	ldr r7, .L_02008264
	movs r2, #0
	ldrsh r3, [r1, r2]
	ldrh r0, [r1]
	lsls r3, r3, #1
	ldrh r2, [r7, r3]
	ldr r3, [r4]
	cmp r3, r2
	bne .L_02008268
	ldr r2, .L_02008250
	adds r3, r0, #1
	strh r3, [r1]
	strh r2, [r6]
	lsls r3, r3, #16
	asrs r3, r3, #15
	ldrh r3, [r7, r3]
	cmp r3, #0
	bne .L_0200826a
	mov r3, r12
	strh r2, [r3]
	movs r0, #110
	bl Func_02002408
	b .L_0200826a
.L_02008250:
	.4byte 0x00000001
.L_02008254:
	.4byte Data_020024f6
.L_02008258:
	.4byte Data_020024fe
.L_0200825c:
	.4byte gInput
.L_02008260:
	.4byte Data_020024fa
.L_02008264:
	.4byte Data_02002410
.L_02008268:
	strh r5, [r1]
.L_0200826a:
	ldr r1, .L_020082c8
	movs r2, #0
	ldrsh r3, [r1, r2]
	mov r12, r1
	cmp r3, #0
	bne .L_020082de
	ldr r6, .L_020082cc
	ldr r4, .L_020082d0
	movs r3, #0
	ldrsh r5, [r6, r3]
	cmp r5, #0
	beq .L_0200828c
	ldr r3, [r4]
	cmp r3, #0
	bne .L_020082de
	strh r3, [r6]
	b .L_020082de
.L_0200828c:
	ldr r3, [r4]
	cmp r3, #0
	beq .L_020082de
	ldr r1, .L_020082d4
	ldr r7, .L_020082d8
	movs r2, #0
	ldrsh r3, [r1, r2]
	ldrh r0, [r1]
	lsls r3, r3, #1
	ldrh r2, [r7, r3]
	ldr r3, [r4]
	cmp r3, r2
	bne .L_020082dc
	ldr r2, .L_020082c4
	adds r3, r0, #1
	strh r3, [r1]
	strh r2, [r6]
	lsls r3, r3, #16
	asrs r3, r3, #15
	ldrh r3, [r7, r3]
	cmp r3, #0
	bne .L_020082de
	mov r3, r12
	strh r2, [r3]
	movs r0, #110
	bl Func_02002408
	b .L_020082de
.L_020082c4:
	.4byte 0x00000001
.L_020082c8:
	.4byte Data_020024f8
.L_020082cc:
	.4byte Data_02002500
.L_020082d0:
	.4byte gInput
.L_020082d4:
	.4byte Data_020024fc
.L_020082d8:
	.4byte Data_02002418
.L_020082dc:
	strh r5, [r1]
.L_020082de:
	pop {r5, r6, r7, pc}
	.section .text.x020082e0,"ax",%progbits
	.global Func_020002e0
	.thumb_func
Func_020002e0:
	push {lr}
	movs r0, #162
	lsls r0, r0, #1
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020082f2
	movs r0, #0
	b .L_0200831c
.L_020082f2:
	ldr r2, .L_02008320
	movs r1, #128
	lsls r1, r1, #2
	adds r1, #94
	adds r3, r2, r1
	movs r1, #0
	ldrsh r3, [r3, r1]
	movs r0, #0
	cmp r3, #2
	beq .L_0200831c
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r2, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	ldr r2, .L_02008324
	eors r3, r2
	negs r0, r3
	orrs r0, r3
	lsrs r0, r0, #31
	negs r0, r0
.L_0200831c:
	pop {pc}
	.2byte 0x0000
.L_02008320:
	.4byte gPartyState
.L_02008324:
	.4byte 0x00000002
	.section .text.x02008328,"ax",%progbits
	.global Func_02000328
	.thumb_func
Func_02000328:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r1, #0
	mov r10, r1
	bl Func_0200005c
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_020086a8
	bl Scheduler_AddOrUpdateCallback
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	mov r1, r10
	str r1, [r3]
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	ldr r3, .L_020086ac
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #2
	bne .L_020083ee
.L_0200836a:
	ldr r5, .L_020086b0
	movs r1, #5
	adds r0, r5, #0
	bl UiText_ShowPositionedMessageAndWait
	movs r0, #1
	bl Func_020023d8
	adds r6, r0, #0
	bl UiWork_FinalizePendingCore
	cmp r6, #0
	bne .L_020083cc
	movs r1, #1
	adds r0, r5, #1
	bl UiText_ShowPositionedMessageAndWait
	ldr r3, .L_020086ac
	movs r2, #152
	lsls r2, r2, #1
	adds r2, #255
	adds r3, r3, r2
	movs r2, #1
	strb r2, [r3]
	movs r0, #5
	bl Party_AddActiveOwner
	movs r0, #6
	bl Party_AddActiveOwner
	movs r0, #7
	bl Party_AddActiveOwner
	movs r0, #1
	bl Party_AddActiveOwner
	movs r0, #2
	bl Party_AddActiveOwner
	movs r0, #3
	bl Party_AddActiveOwner
	bl Func_020022d0
	movs r3, #1
	adds r6, r0, #0
	negs r3, r3
	cmp r6, r3
	beq .L_0200836a
.L_020083cc:
	movs r0, #60
	bl Battle_WaitMode0
	bl Event_ClearStatus1c6
	movs r0, #78
	bl Func_02002408
	movs r0, #150
	lsls r0, r0, #1
	bl Battle_WaitMode0
	ldr r0, .L_020086b4
	movs r1, #1
	bl Func_020023b8
	b .L_02008a2c
.L_020083ee:
	ldr r3, .L_020086b8
	mov r1, r10
	strb r1, [r3]
.L_020083f4:
	bl Func_020022f0
	adds r5, r0, #0
	cmp r5, #0
	bge .L_02008422
	ldr r3, .L_020086bc
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_02008422
	ldr r3, .L_020086ac
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #74
	adds r3, r3, r2
	movs r2, #1
	strb r2, [r3]
	ldr r3, .L_020086c0
	ldr r0, .L_020086c4
	strb r2, [r3]
	movs r1, #1
	movs r2, #8
	bl Func_020022b8
.L_02008422:
	cmp r5, #0
	bne .L_02008446
	mov r3, r10
	cmp r3, #0
	bne .L_02008446
	movs r0, #30
	bl WaitFrames
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_020086c8
	bl Scheduler_AddOrUpdateCallback
	movs r0, #1
	bl WaitFrames
	movs r1, #1
	mov r10, r1
.L_02008446:
	cmp r5, #0
	ble .L_02008452
	bl Menu_SelectSaveSlotAction
	adds r5, r0, #0
	b .L_02008454
.L_02008452:
	movs r5, #0
.L_02008454:
	cmp r5, #0
	beq .L_0200845a
	b .L_020086fc
.L_0200845a:
	bl Func_02002350
	ldr r3, .L_020086ac
	movs r1, #147
	lsls r1, r1, #1
	adds r1, #255
	adds r2, r3, r1
	ldrb r0, [r2]
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #38
	adds r3, r3, r2
	ldrb r1, [r3]
	bl Func_02002310
	movs r6, #4
	movs r7, #5
.L_0200847c:
	movs r0, #6
	bl WaitFrames
	adds r0, r6, #0
	bl Func_020022f8
	movs r3, #1
	adds r5, r0, #0
	negs r3, r3
	cmp r5, r3
	bne .L_0200849a
	cmp r6, #4
	beq .L_020083f4
	subs r6, #1
	b .L_0200847c
.L_0200849a:
	ldr r3, .L_020086cc
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	bne .L_020084ae
	ldr r3, .L_020086d0
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	beq .L_020084b0
.L_020084ae:
	movs r7, #8
.L_020084b0:
	adds r6, #1
	cmp r6, r7
	blt .L_0200847c
.L_020084b6:
	ldr r0, .L_020086d4
	movs r1, #13
	bl UiText_ShowPositionedMessageAndWait
	movs r0, #1
	movs r1, #0
	movs r2, #0
	movs r3, #0
	bl Menu_RunConfirmSelection
	adds r5, r0, #0
	bl UiWork_FinalizePendingCore
	cmp r5, #0
	bne .L_02008570
.L_020084d4:
	movs r0, #0
	bl Func_020022b0
	movs r3, #1
	negs r3, r3
	adds r5, r0, #0
	mov r8, r3
	cmp r5, r8
	beq .L_020083f4
	cmp r5, #1
	bne .L_0200852a
	ldr r5, .L_020086d8
	movs r2, #192
	adds r1, r5, #0
	lsls r2, r2, #6
	ldr r6, .L_020086dc
	ldr r0, .L_020086e0
	mov lr, r6
	.2byte 0xf800
	bl Func_02000b24
	adds r7, r0, #0
	cmp r7, r8
	beq .L_02008508
	bl Func_020023e8
.L_02008508:
	movs r2, #192
	adds r1, r5, #0
	lsls r2, r2, #6
	ldr r0, .L_020086e4
	mov lr, r6
	.2byte 0xf800
	movs r2, #192
	adds r0, r5, #0
	ldr r1, .L_020086e0
	lsls r2, r2, #6
	mov lr, r6
	.2byte 0xf800
	cmp r7, r8
	beq .L_020084d4
	ldr r0, .L_020086e4
	movs r1, #0
	b .L_02008568
.L_0200852a:
	cmp r5, #0
	bne .L_02008574
	ldr r5, .L_020086d8
	movs r2, #192
	adds r1, r5, #0
	lsls r2, r2, #6
	ldr r6, .L_020086dc
	ldr r0, .L_020086e0
	mov lr, r6
	.2byte 0xf800
	bl Func_02002350
	bl Func_02000e44
	movs r2, #192
	adds r7, r0, #0
	adds r1, r5, #0
	lsls r2, r2, #6
	ldr r0, .L_020086e4
	mov lr, r6
	.2byte 0xf800
	movs r2, #192
	adds r0, r5, #0
	ldr r1, .L_020086e0
	lsls r2, r2, #6
	mov lr, r6
	.2byte 0xf800
	cmp r7, r8
	beq .L_020084d4
	ldr r0, .L_020086e4
	movs r1, #1
.L_02008568:
	movs r2, #0
	bl Func_02002370
	b .L_02008574
.L_02008570:
	bl Func_02002398
.L_02008574:
	movs r6, #0
	movs r7, #1
.L_02008578:
	movs r0, #6
	bl WaitFrames
	adds r0, r6, #0
	bl Func_020022f8
	movs r1, #1
	adds r5, r0, #0
	negs r1, r1
	cmp r5, r1
	bne .L_02008596
	cmp r6, #0
	beq .L_020084b6
	subs r6, #1
	b .L_02008578
.L_02008596:
	ldr r3, .L_020086d0
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	beq .L_020085a2
	movs r7, #4
.L_020085a2:
	adds r6, #1
	cmp r6, r7
	blt .L_02008578
	ldr r3, .L_020086e8
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	beq .L_0200869c
.L_020085b2:
	ldr r5, .L_020086ec
	movs r1, #13
	adds r0, r5, #0
	bl UiText_ShowPositionedMessageAndWait
	ldr r1, .L_020086f0
	movs r0, #1
	movs r2, #8
	movs r3, #0
	bl Menu_RunConfirmSelection
	adds r6, r0, #0
	bl UiWork_FinalizePendingCore
	cmp r6, #0
	bne .L_02008648
	ldr r3, .L_020086e8
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	bne .L_020085e6
	adds r0, r5, #1
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
	b .L_02008648
.L_020085e6:
	ldr r6, .L_020086d8
	movs r2, #192
	adds r1, r6, #0
	lsls r2, r2, #6
	ldr r7, .L_020086dc
	ldr r0, .L_020086e0
	mov lr, r7
	.2byte 0xf800
	movs r0, #5
	bl Func_020022d8
	adds r5, r0, #0
	cmp r5, #0
	bne .L_020085b2
	ldr r5, .L_020086ac
	movs r1, #147
	lsls r1, r1, #1
	movs r2, #128
	adds r1, #255
	lsls r2, r2, #2
	adds r3, r5, r1
	adds r2, #38
	ldrb r0, [r3]
	adds r3, r5, r2
	ldrb r1, [r3]
	bl Func_02002310
	movs r2, #192
	adds r1, r6, #0
	lsls r2, r2, #6
	ldr r0, .L_020086e4
	mov lr, r7
	.2byte 0xf800
	movs r2, #192
	ldr r1, .L_020086e0
	lsls r2, r2, #6
	adds r0, r6, #0
	mov lr, r7
	.2byte 0xf800
	ldr r0, .L_020086e4
	bl Func_020023a0
	movs r3, #196
	lsls r3, r3, #1
	adds r3, #255
	adds r5, r5, r3
	movs r3, #1
	strb r3, [r5]
	b .L_0200869c
.L_02008648:
	ldr r0, .L_020086f4
	movs r1, #13
	bl UiText_ShowPositionedMessageAndWait
	ldr r5, .L_020086f0
	movs r0, #1
	adds r1, r5, #0
	movs r2, #8
	movs r3, #0
	bl Menu_RunConfirmSelection
	adds r6, r0, #0
	bl UiWork_FinalizePendingCore
	cmp r6, #0
	bne .L_0200867e
	movs r0, #46
	bl GameFlag_SetBit
	ldr r3, .L_020086ac
	movs r1, #196
	lsls r1, r1, #1
	adds r1, #255
	adds r3, r3, r1
	movs r2, #2
	strb r2, [r3]
	b .L_0200869c
.L_0200867e:
	ldr r0, .L_020086f8
	movs r1, #13
	bl UiText_ShowPositionedMessageAndWait
	movs r0, #1
	adds r1, r5, #0
	movs r2, #8
	movs r3, #0
	bl Menu_RunConfirmSelection
	adds r6, r0, #0
	bl UiWork_FinalizePendingCore
	cmp r6, #0
	bne .L_020085b2
.L_0200869c:
	ldr r0, .L_020086b4
	movs r1, #8
	bl Func_020023b8
	b .L_020089fe
	.2byte 0x0000
.L_020086a8:
	.4byte Func_020001f4
.L_020086ac:
	.4byte gPartyState
.L_020086b0:
	.4byte 0x00000008
.L_020086b4:
	.4byte 0x00000000
.L_020086b8:
	.4byte gSleepDisabled
.L_020086bc:
	.4byte gDebugMode
.L_020086c0:
	.4byte gAutoSleepEnabled
.L_020086c4:
	.4byte 0x0000000b
.L_020086c8:
	.4byte Func_0200015c
.L_020086cc:
	.4byte Data_020024f6
.L_020086d0:
	.4byte Data_020024f8
.L_020086d4:
	.4byte 0x00000024
.L_020086d8:
	.4byte Data_02000000
.L_020086dc:
	.4byte IwramCopyWords
.L_020086e0:
	.4byte gMapCellBuffer
.L_020086e4:
	.4byte Data_02012f94 + 0x6c
.L_020086e8:
	.4byte Data_02003860
.L_020086ec:
	.4byte 0x00000025
.L_020086f0:
	.4byte 0x00000029
.L_020086f4:
	.4byte 0x00000027
.L_020086f8:
	.4byte 0x00000028
.L_020086fc:
	cmp r5, #1
	beq .L_02008702
	b .L_02008850
.L_02008702:
	movs r0, #1
	bl Func_020022d8
	movs r2, #1
	adds r5, r0, #0
	negs r2, r2
	cmp r5, r2
	bne .L_02008714
	b .L_020083f4
.L_02008714:
	movs r0, #10
	adds r0, #255
	bl GameFlag_SetBit
	ldr r5, .L_02008a38
	movs r1, #147
	lsls r1, r1, #1
	movs r2, #128
	adds r1, #255
	lsls r2, r2, #2
	adds r3, r5, r1
	adds r2, #38
	ldrb r0, [r3]
	adds r3, r5, r2
	ldrb r1, [r3]
	bl Func_02002310
	bl Func_02002388
	ldr r3, [r5]
	cmp r3, r0
	beq .L_02008772
	movs r1, #242
	lsls r1, r1, #1
	adds r3, r5, r1
	ldrh r0, [r3]
	movs r2, #240
	lsls r2, r2, #1
	adds r3, r5, r2
	strh r0, [r3]
	adds r1, #2
	adds r3, r5, r1
	ldrh r1, [r3]
	adds r2, #2
	adds r3, r5, r2
	strh r1, [r3]
	lsls r0, r0, #16
	lsls r1, r1, #16
	asrs r0, r0, #16
	asrs r1, r1, #16
	bl Func_020023e0
	movs r0, #10
	adds r0, #255
	bl GameFlag_ClearBit
	b .L_02008846
.L_02008772:
	ldr r3, .L_02008a3c
	movs r2, #130
	ldr r3, [r3]
	lsls r2, r2, #2
	ands r3, r2
	cmp r3, r2
	bne .L_020087c6
	bl Func_020002e0
	cmp r0, #0
	beq .L_0200878c
	ldr r0, .L_02008a40
	b .L_02008806
.L_0200878c:
	movs r1, #242
	lsls r1, r1, #1
	adds r3, r5, r1
	ldrh r0, [r3]
	movs r2, #240
	lsls r2, r2, #1
	adds r3, r5, r2
	strh r0, [r3]
	adds r1, #2
	adds r3, r5, r1
	ldrh r1, [r3]
	adds r2, #2
	adds r3, r5, r2
	strh r1, [r3]
	lsls r0, r0, #16
	lsls r1, r1, #16
	asrs r0, r0, #16
	asrs r1, r1, #16
	bl Func_020023e0
	movs r0, #10
	adds r0, #255
	bl GameFlag_ClearBit
	movs r0, #159
	lsls r0, r0, #1
	bl GameFlag_SetBit
	b .L_02008846
.L_020087c6:
	ldr r3, .L_02008a44
	ldr r2, [r5, #4]
	ldr r3, [r3]
	cmp r2, r3
	beq .L_02008846
	ldr r6, .L_02008a48
	movs r1, #9
	adds r0, r6, #0
	bl UiText_ShowPositionedMessageAndWait
	adds r0, r6, #1
	movs r1, #13
	bl UiText_ShowPositionedMessageAndWait
	movs r0, #1
	movs r1, #0
	movs r2, #0
	movs r3, #0
	bl Menu_RunConfirmSelection
	cmp r0, #0
	beq .L_020087f8
	bl UiWork_FinalizePendingCore
	b .L_020083f4
.L_020087f8:
	bl UiWork_FinalizePendingCore
	bl Func_020002e0
	cmp r0, #0
	beq .L_0200880e
	adds r0, r6, #2
.L_02008806:
	movs r1, #9
	bl UiText_ShowPositionedMessageAndWait
	b .L_020083f4
.L_0200880e:
	movs r1, #242
	lsls r1, r1, #1
	adds r3, r5, r1
	ldrh r0, [r3]
	movs r2, #240
	lsls r2, r2, #1
	adds r3, r5, r2
	strh r0, [r3]
	adds r1, #2
	adds r3, r5, r1
	ldrh r1, [r3]
	adds r2, #2
	adds r3, r5, r2
	strh r1, [r3]
	lsls r0, r0, #16
	lsls r1, r1, #16
	asrs r0, r0, #16
	asrs r1, r1, #16
	bl Func_020023e0
	movs r0, #10
	adds r0, #255
	bl GameFlag_ClearBit
	movs r0, #64
	adds r0, #255
	bl GameFlag_SetBit
.L_02008846:
	movs r0, #131
	lsls r0, r0, #1
	bl GameFlag_ClearBit
	b .L_020089fe
.L_02008850:
	cmp r5, #2
	bne .L_0200885a
	bl Func_020022e0
	b .L_020083f4
.L_0200885a:
	cmp r5, #3
	bne .L_02008864
	bl Func_020022e8
	b .L_020083f4
.L_02008864:
	cmp r5, #4
	bne .L_020088dc
	movs r0, #4
	bl Func_020022d8
	movs r3, #1
	adds r5, r0, #0
	negs r3, r3
	cmp r5, r3
	bne .L_0200887a
	b .L_020083f4
.L_0200887a:
	ldr r5, .L_02008a38
	movs r1, #133
	lsls r1, r1, #2
	movs r0, #144
	adds r3, r5, r1
	movs r2, #4
	lsls r0, r0, #4
	str r2, [r3]
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008898
	bl Func_02002390
.L_02008898:
	movs r2, #147
	lsls r2, r2, #1
	movs r1, #128
	adds r2, #255
	lsls r1, r1, #2
	adds r3, r5, r2
	adds r1, #38
	ldrb r0, [r3]
	adds r3, r5, r1
	ldrb r1, [r3]
	bl Func_02002310
	movs r0, #10
	adds r0, #255
	bl GameFlag_ClearBit
	movs r0, #131
	lsls r0, r0, #1
	bl GameFlag_ClearBit
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #118
	adds r2, r5, r3
	movs r3, #0
	strh r3, [r2]
	ldr r2, .L_02008a4c
	movs r3, #1
	strb r3, [r2]
	ldr r0, .L_02008a50
	movs r1, #1
	bl Func_020023b8
	b .L_020089fe
.L_020088dc:
	cmp r5, #5
	beq .L_020088e2
	b .L_020083f4
.L_020088e2:
	movs r0, #6
	bl Func_020022d8
	movs r1, #1
	adds r5, r0, #0
	negs r1, r1
	cmp r5, r1
	bne .L_020088f4
	b .L_020083f4
.L_020088f4:
	ldr r3, .L_02008a38
	movs r1, #147
	lsls r1, r1, #1
	adds r1, #255
	adds r2, r3, r1
	ldrb r0, [r2]
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #38
	adds r3, r3, r2
	ldrb r1, [r3]
	bl Func_02002310
.L_0200890e:
	movs r0, #2
	bl Func_020022b0
	movs r3, #1
	negs r3, r3
	adds r5, r0, #0
	mov r8, r3
	cmp r5, r8
	beq .L_020088e2
	cmp r5, #1
	bne .L_0200896a
	ldr r5, .L_02008a54
	movs r2, #192
	adds r1, r5, #0
	lsls r2, r2, #6
	ldr r6, .L_02008a58
	ldr r0, .L_02008a5c
	mov lr, r6
	.2byte 0xf800
	bl Func_02000b24
	adds r7, r0, #0
	cmp r7, r8
	beq .L_02008942
	bl Func_020023e8
.L_02008942:
	movs r2, #192
	adds r1, r5, #0
	lsls r2, r2, #6
	ldr r0, .L_02008a60
	mov lr, r6
	.2byte 0xf800
	movs r2, #192
	adds r0, r5, #0
	ldr r1, .L_02008a5c
	lsls r2, r2, #6
	mov lr, r6
	.2byte 0xf800
	cmp r7, r8
	beq .L_0200890e
	ldr r0, .L_02008a60
	movs r1, #0
	movs r2, #1
	bl Func_02002370
	b .L_020089ae
.L_0200896a:
	cmp r5, #0
	bne .L_020089ae
	ldr r5, .L_02008a54
	movs r2, #192
	adds r1, r5, #0
	lsls r2, r2, #6
	ldr r6, .L_02008a58
	ldr r0, .L_02008a5c
	mov lr, r6
	.2byte 0xf800
	bl Func_02002350
	bl Func_02000e44
	movs r2, #192
	adds r7, r0, #0
	adds r1, r5, #0
	lsls r2, r2, #6
	ldr r0, .L_02008a60
	mov lr, r6
	.2byte 0xf800
	movs r2, #192
	adds r0, r5, #0
	ldr r1, .L_02008a5c
	lsls r2, r2, #6
	mov lr, r6
	.2byte 0xf800
	cmp r7, r8
	beq .L_0200890e
	ldr r0, .L_02008a60
	movs r1, #1
	movs r2, #1
	bl Func_02002370
.L_020089ae:
	ldr r3, .L_02008a64
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	bne .L_020089c2
	ldr r3, .L_02008a68
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	beq .L_020089f6
.L_020089c2:
	movs r6, #0
	movs r7, #1
.L_020089c6:
	movs r0, #6
	bl WaitFrames
	adds r0, r6, #0
	bl Func_020022f8
	movs r3, #1
	adds r5, r0, #0
	negs r3, r3
	cmp r5, r3
	bne .L_020089e4
	cmp r6, #0
	beq .L_020088e2
	subs r6, #1
	b .L_020089c6
.L_020089e4:
	ldr r3, .L_02008a68
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	beq .L_020089f0
	movs r7, #4
.L_020089f0:
	adds r6, #1
	cmp r6, r7
	blt .L_020089c6
.L_020089f6:
	movs r0, #10
	adds r0, #255
	bl GameFlag_SetBit
.L_020089fe:
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #172
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #186
	lsls r2, r2, #2
	adds r2, #255
	strh r2, [r3]
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #78
	bl Func_02002408
	bl Event_ClearStatus1c6
	bl Event_WaitValue1c8Frames
	movs r0, #60
	bl Battle_WaitMode0
.L_02008a2c:
	movs r0, #0
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008a38:
	.4byte gPartyState
.L_02008a3c:
	.4byte gInput
.L_02008a40:
	.4byte 0x00000007
.L_02008a44:
	.4byte Data_02001000
.L_02008a48:
	.4byte 0x00000005
.L_02008a4c:
	.4byte gSleepDisabled
.L_02008a50:
	.4byte 0x00000138
.L_02008a54:
	.4byte Data_02000000
.L_02008a58:
	.4byte IwramCopyWords
.L_02008a5c:
	.4byte gMapCellBuffer
.L_02008a60:
	.4byte Data_02012f94 + 0x6c
.L_02008a64:
	.4byte Data_020024f6
.L_02008a68:
	.4byte Data_020024f8
	.section .text.x02008a70,"ax",%progbits
	.global Func_02000a70
	.thumb_func
Func_02000a70:
	push {lr}
	ldr r3, .L_02008a98
	strh r3, [r1, #2]
	strh r3, [r1, #4]
	cmp r0, #7
	bgt .L_02008a82
	adds r3, r0, #0
	adds r3, #65
	b .L_02008b1a
.L_02008a82:
	cmp r0, #12
	bgt .L_02008a8c
	adds r3, r0, #0
	adds r3, #66
	b .L_02008b1a
.L_02008a8c:
	cmp r0, #23
	bgt .L_02008a9c
	adds r3, r0, #0
	adds r3, #67
	b .L_02008b1a
	.2byte 0x0000
.L_02008a98:
	.4byte 0x00000000
.L_02008a9c:
	cmp r0, #31
	bgt .L_02008aa6
	adds r3, r0, #0
	adds r3, #26
	b .L_02008b1a
.L_02008aa6:
	cmp r0, #42
	bgt .L_02008ab0
	adds r3, r0, #0
	adds r3, #65
	b .L_02008b1a
.L_02008ab0:
	cmp r0, #44
	bgt .L_02008aba
	adds r3, r0, #0
	adds r3, #66
	b .L_02008b1a
.L_02008aba:
	cmp r0, #55
	bgt .L_02008ac4
	adds r3, r0, #0
	adds r3, #67
	b .L_02008b1a
.L_02008ac4:
	cmp r0, #56
	bne .L_02008acc
	ldr r3, .L_02008ae4
	b .L_02008b1a
.L_02008acc:
	cmp r0, #57
	bne .L_02008ad4
	ldr r3, .L_02008ae8
	b .L_02008b1a
.L_02008ad4:
	cmp r0, #58
	bne .L_02008adc
	ldr r3, .L_02008aec
	b .L_02008b1a
.L_02008adc:
	cmp r0, #59
	bne .L_02008af4
	ldr r3, .L_02008af0
	b .L_02008b1a
.L_02008ae4:
	.4byte 0x00000021
.L_02008ae8:
	.4byte 0x0000003f
.L_02008aec:
	.4byte 0x00000023
.L_02008af0:
	.4byte 0x00000026
.L_02008af4:
	cmp r0, #60
	bne .L_02008afc
	ldr r3, .L_02008b0c
	b .L_02008b1a
.L_02008afc:
	cmp r0, #61
	bne .L_02008b04
	ldr r3, .L_02008b10
	b .L_02008b1a
.L_02008b04:
	cmp r0, #62
	bne .L_02008b18
	ldr r3, .L_02008b14
	b .L_02008b1a
.L_02008b0c:
	.4byte 0x00000024
.L_02008b10:
	.4byte 0x00000025
.L_02008b14:
	.4byte 0x0000002b
.L_02008b18:
	ldr r3, .L_02008b20
.L_02008b1a:
	strh r3, [r1]
	pop {pc}
	.2byte 0x0000
.L_02008b20:
	.4byte 0x0000003d
	.section .text.x02008b24,"ax",%progbits
	.global Func_02000b24
	.thumb_func
Func_02000b24:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #4
	movs r3, #2
	str r3, [sp, #0]
	movs r1, #5
	movs r2, #18
	movs r3, #8
	movs r0, #6
	bl UiWindow_Create
	ldr r5, .L_02008bf0
	adds r6, r0, #0
	adds r1, r6, #0
	adds r0, r5, #0
	movs r2, #0
	movs r3, #4
	bl UiText_DrawResource
	adds r0, r5, #1
	adds r1, r6, #0
	movs r2, #0
	movs r3, #16
	adds r5, #3
	bl UiText_DrawResource
	adds r0, r5, #0
	adds r1, r6, #0
	movs r2, #0
	movs r3, #36
	bl UiText_DrawResource
	bl Func_02002220
	movs r0, #10
	bl WaitFrames
.L_02008b74:
	movs r5, #3
	b .L_02008bcc
.L_02008b78:
	ldr r3, .L_02008bf4
	ldrh r2, [r3]
	adds r3, r5, #0
	ands r3, r2
	cmp r3, r5
	bne .L_02008bc6
	ldr r3, .L_02008bf8
	movs r2, #1
	ldr r3, [r3]
	lsls r3, r3, #26
	lsrs r3, r3, #30
	eors r3, r2
	lsls r2, r3, #1
	adds r2, r2, r3
	ldr r3, .L_02008bfc
	lsls r2, r2, #3
	adds r2, r2, r3
	ldrh r3, [r2]
	cmp r3, #48
	bne .L_02008bc6
	ldrh r3, [r2, #2]
	cmp r3, #48
	bne .L_02008bc6
	ldrh r3, [r2, #4]
	cmp r3, #48
	bne .L_02008bc6
	ldrh r3, [r2, #6]
	cmp r3, #48
	bne .L_02008bc6
	ldr r2, .L_02008c00
	ldr r3, .L_02008be0
	strh r3, [r2]
	ldr r3, .L_02008be4
	strh r3, [r2, #2]
	ldr r3, .L_02008be8
	strh r3, [r2, #4]
	ldr r3, .L_02008bec
	strh r3, [r2, #6]
	b .L_02008c08
.L_02008bc6:
	movs r0, #1
	bl WaitFrames
.L_02008bcc:
	ldr r3, .L_02008c04
	movs r2, #2
	ldr r3, [r3, #4]
	ands r3, r2
	cmp r3, #0
	beq .L_02008b78
	movs r0, #113
	bl Func_02002408
	b .L_02008ce8
.L_02008be0:
	.4byte 0x00000055
.L_02008be4:
	.4byte 0x00000056
.L_02008be8:
	.4byte 0x00000054
.L_02008bec:
	.4byte 0x00000053
.L_02008bf0:
	.4byte 0x00001184
.L_02008bf4:
	.4byte gLinkStatus
.L_02008bf8:
	.4byte 0x04000128
.L_02008bfc:
	.4byte Data_02003874
.L_02008c00:
	.4byte Data_02003a74
.L_02008c04:
	.4byte gInput
.L_02008c08:
	movs r0, #10
	bl WaitFrames
	ldr r0, .L_02008d7c
	bl Party_Check
	movs r0, #10
	bl WaitFrames
	movs r1, #3
	movs r2, #1
	movs r3, #0
	mov r10, r1
	movs r7, #0
	mov r9, r2
	mov r8, r3
	movs r5, #0
.L_02008c2a:
	mov r1, r8
	cmp r1, #0
	bne .L_02008c62
	ldr r3, .L_02008d80
	ldrh r3, [r3]
	cmp r3, #0
	beq .L_02008c62
	movs r2, #1
	adds r0, r6, #0
	movs r1, #2
	mov r8, r2
	bl UiWork_Finalize
	movs r3, #2
	str r3, [sp, #0]
	movs r1, #10
	movs r2, #20
	movs r3, #4
	movs r0, #5
	bl UiWindow_Create
	adds r6, r0, #0
	adds r1, r6, #0
	ldr r0, .L_02008d84
	movs r2, #0
	movs r3, #4
	bl UiText_DrawResource
.L_02008c62:
	cmp r5, #9
	bne .L_02008c8e
	ldr r3, .L_02008d80
	ldrh r3, [r3]
	cmp r3, #0
	bne .L_02008c8e
	ldr r2, .L_02008d88
	strh r3, [r2]
	strh r3, [r2, #2]
	strh r3, [r2, #4]
	strh r3, [r2, #6]
	bl Func_02002228
	movs r0, #10
	bl WaitFrames
	bl Func_02002220
	movs r0, #10
	bl WaitFrames
	b .L_02008b74
.L_02008c8e:
	ldr r3, .L_02008d8c
	adds r7, #1
	ldrh r2, [r3]
	mov r3, r10
	ands r3, r2
	cmp r3, r10
	bne .L_02008c9e
	movs r7, #0
.L_02008c9e:
	cmp r7, #10
	bne .L_02008ca8
	movs r3, #0
	mov r9, r3
	b .L_02008cbe
.L_02008ca8:
	ldr r3, .L_02008d90
	ldr r3, [r3]
	cmp r3, #0
	beq .L_02008cbe
	movs r0, #1
	bl WaitFrames
	ldr r1, .L_02008d94
	adds r5, #1
	cmp r5, r1
	ble .L_02008c2a
.L_02008cbe:
	mov r2, r9
	cmp r2, #0
	bne .L_02008cf6
	adds r0, r6, #0
	bl RenderOutput_PrepareForRedraw
	ldr r0, .L_02008d98
	adds r1, r6, #0
	movs r2, #0
	movs r3, #4
	bl UiText_DrawResource
.L_02008cd6:
	movs r0, #1
	bl WaitFrames
	ldr r3, .L_02008d9c
	movs r2, #1
	ldr r3, [r3, #4]
	ands r3, r2
	cmp r3, #0
	beq .L_02008cd6
.L_02008ce8:
	adds r0, r6, #0
	movs r1, #2
	bl UiWork_Finalize
	movs r0, #1
	negs r0, r0
	b .L_02008d6e
.L_02008cf6:
	movs r0, #10
	bl WaitFrames
	bl Func_02002228
	movs r0, #10
	bl WaitFrames
	adds r0, r6, #0
	movs r1, #2
	bl UiWork_Finalize
	ldr r5, .L_02008da0
	movs r2, #133
	ldr r6, .L_02008da4
	ldr r1, .L_02008da8
	lsls r2, r2, #2
	adds r0, r5, #0
	mov lr, r6
	.2byte 0xf800
	movs r3, #136
	lsls r3, r3, #2
	movs r2, #210
	adds r0, r5, r3
	ldr r1, .L_02008dac
	lsls r2, r2, #4
	mov lr, r6
	.2byte 0xf800
	movs r0, #0
	bl Owner_RecalculateStats
	movs r0, #1
	bl Owner_RecalculateStats
	movs r0, #2
	bl Owner_RecalculateStats
	movs r0, #3
	bl Owner_RecalculateStats
	movs r2, #244
	adds r1, r5, #0
	lsls r2, r2, #4
	ldr r0, .L_02008da8
	mov lr, r6
	.2byte 0xf800
	ldr r3, .L_02008db0
	movs r1, #147
	lsls r1, r1, #1
	adds r1, #255
	adds r2, r3, r1
	ldrb r0, [r2]
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #38
	adds r3, r3, r2
	ldrb r1, [r3]
	bl Func_02002310
	movs r0, #1
.L_02008d6e:
	add sp, #4
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008d7c:
	.4byte Data_02012f94 + 0x6c
.L_02008d80:
	.4byte gSerialReceivedSize
.L_02008d84:
	.4byte 0x00001186
.L_02008d88:
	.4byte Data_02003a74
.L_02008d8c:
	.4byte gLinkStatus
.L_02008d90:
	.4byte gSerialReceiveDest
.L_02008d94:
	.4byte 0x000927bf
.L_02008d98:
	.4byte 0x00001188
.L_02008d9c:
	.4byte gInput
.L_02008da0:
	.4byte GameFlagBytes
.L_02008da4:
	.4byte IwramCopyWords
.L_02008da8:
	.4byte Data_02012f94 + 0xac
.L_02008dac:
	.4byte Data_02012f94 + 0x2ac
.L_02008db0:
	.4byte gPartyState
	.section .text.x02008db4,"ax",%progbits
	.global Func_02000db4
	.thumb_func
Func_02000db4:
	push {r5, r6, r7, lr}
	adds r7, r3, #0
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #60]
	adds r5, r1, #0
	mov r12, r3
	movs r4, #12
	ldrsh r3, [r0, r4]
	ldr r1, [sp, #16]
	adds r3, r5, r3
	adds r5, r3, #1
	movs r4, #14
	ldrsh r3, [r0, r4]
	adds r3, r2, r3
	adds r2, r3, #1
	ldr r3, [sp, #20]
	lsls r3, r3, #12
	str r3, [sp, #20]
	cmp r5, #0
	bge .L_02008de2
	adds r7, r7, r5
	movs r5, #0
.L_02008de2:
	adds r3, r5, r7
	cmp r3, #29
	ble .L_02008dec
	movs r3, #30
	subs r7, r3, r5
.L_02008dec:
	cmp r2, #0
	bge .L_02008df4
	adds r1, r1, r2
	movs r2, #0
.L_02008df4:
	adds r3, r2, r1
	cmp r3, #29
	ble .L_02008dfe
	movs r3, #20
	subs r1, r3, r2
.L_02008dfe:
	cmp r7, #0
	ble .L_02008e3c
	cmp r1, #0
	ble .L_02008e3c
	lsls r3, r2, #6
	mov r4, r12
	adds r6, r3, r4
.L_02008e0c:
	lsls r3, r5, #1
	adds r3, r6, r3
	adds r4, r3, #0
	adds r0, r7, #0
	adds r4, #8
	cmp r0, #0
	beq .L_02008e2e
.L_02008e1a:
	ldrh r3, [r4]
	ldr r2, .L_02008e40
	subs r0, #1
	ands r3, r2
	ldr r2, [sp, #20]
	orrs r3, r2
	strh r3, [r4]
	adds r4, #2
	cmp r0, #0
	bne .L_02008e1a
.L_02008e2e:
	subs r1, #1
	adds r6, #64
	cmp r1, #0
	bne .L_02008e0c
	movs r3, #1
	mov r4, r12
	strb r3, [r4, #3]
.L_02008e3c:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008e40:
	.4byte 0xffffefff
	.section .text.x02008e44,"ax",%progbits
	.global Func_02000e44
	.thumb_func
Func_02000e44:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #124
	bl Func_02002208
	ldr r3, .L_02008ecc
	movs r0, #147
	movs r1, #128
	lsls r0, r0, #1
	lsls r1, r1, #2
	adds r0, #255
	adds r1, #38
	adds r2, r3, r0
	adds r3, r3, r1
	ldrb r0, [r2]
	ldrb r1, [r3]
	bl Func_02002310
	ldr r4, .L_02008ed0
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	ldr r0, .L_02008ed4
	adds r1, r4, #0
	adds r2, #8
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r3, .L_02008ec8
	movs r0, #1
	strh r3, [r4, #8]
	bl Func_020022b0
	movs r2, #1
	negs r2, r2
	str r0, [sp, #64]
	cmp r0, r2
	bne .L_02008ea0
	bl .L_020097f0
.L_02008ea0:
	movs r0, #71
	bl Func_02002408
	movs r0, #238
	movs r3, #0
	lsls r0, r0, #1
	str r3, [sp, #60]
	str r3, [sp, #52]
	str r3, [sp, #48]
	bl Runtime_BumpAllocateAlternatePool
	movs r4, #0
	str r0, [sp, #36]
	ldr r0, .L_02008ed8
	str r4, [sp, #8]
	bl Func_02002320
	ldr r1, [sp, #64]
	b .L_02008edc
	.2byte 0x0000
.L_02008ec8:
	.4byte 0x00006318
.L_02008ecc:
	.4byte gPartyState
.L_02008ed0:
	.4byte 0x050001c0
.L_02008ed4:
	.4byte 0x050001e0
.L_02008ed8:
	.4byte 0x06006000
.L_02008edc:
	movs r0, #130
	lsls r0, r0, #1
	str r0, [sp, #44]
	cmp r1, #0
	beq .L_02008ef4
	ldr r3, [sp, #64]
	movs r2, #61
	str r2, [sp, #44]
	cmp r3, #1
	beq .L_02008ef4
	movs r4, #16
	str r4, [sp, #44]
.L_02008ef4:
	ldr r0, [sp, #44]
	movs r1, #50
	bl __divsi3
	adds r0, #1
	str r0, [sp, #40]
	movs r5, #2
	movs r1, #13
	movs r2, #29
	movs r3, #6
	movs r0, #1
	str r5, [sp, #0]
	bl UiWindow_Create
	mov r9, r0
	mov r1, r9
	ldr r0, .L_02009160
	movs r2, #0
	movs r3, #0
	bl UiText_DrawStringInWindow
	ldr r0, .L_02009164
	mov r1, r9
	movs r2, #0
	movs r3, #8
	bl UiText_DrawStringInWindow
	ldr r0, .L_02009168
	mov r1, r9
	movs r2, #0
	movs r3, #16
	bl UiText_DrawStringInWindow
	ldr r0, .L_0200916c
	mov r1, r9
	movs r2, #0
	movs r3, #24
	bl UiText_DrawStringInWindow
	ldr r6, .L_02009170
	movs r3, #5
	str r3, [sp, #0]
	mov r0, r9
	movs r1, #20
	movs r2, #0
	movs r3, #20
	bl UiWindow_DrawDividerLine
	adds r0, r6, #0
	mov r1, r9
	adds r6, #1
	movs r2, #160
	movs r3, #0
	bl UiText_DrawCharacterAtOffset
	adds r0, r6, #0
	mov r1, r9
	adds r6, #1
	movs r2, #160
	movs r3, #8
	bl UiText_DrawCharacterAtOffset
	adds r0, r6, #0
	mov r1, r9
	movs r2, #160
	movs r3, #24
	bl UiText_DrawCharacterAtOffset
	movs r0, #5
	movs r1, #1
	movs r2, #20
	movs r3, #12
	str r5, [sp, #0]
	bl UiWindow_Create
	str r0, [sp, #56]
	movs r6, #0
.L_02008f8e:
	ldr r2, [sp, #36]
	movs r0, #224
	lsls r0, r0, #1
	adds r1, r6, r0
	adds r2, #2
	movs r3, #0
	adds r6, #1
	strb r3, [r2, r1]
	cmp r6, #26
	bne .L_02008f8e
	ldr r2, .L_02009174
	movs r6, #0
.L_02008fa6:
	movs r1, #130
	movs r3, #99
	adds r6, #1
	lsls r1, r1, #1
	strb r3, [r2]
	adds r2, #1
	cmp r6, r1
	bne .L_02008fa6
	ldr r2, [sp, #36]
	movs r3, #200
	lsls r3, r3, #1
	movs r6, #0
	adds r2, r2, r3
	mov r10, r2
	mov r11, r6
.L_02008fc4:
	movs r1, #10
	adds r0, r6, #0
	bl __modsi3
	adds r5, r0, #0
	lsls r3, r5, #1
	adds r3, r3, r5
	lsls r7, r3, #2
	movs r4, #8
	adds r4, r4, r7
	adds r0, r6, #0
	movs r1, #10
	mov r8, r4
	bl __divsi3
	lsls r0, r0, #4
	adds r3, r0, #2
	cmp r5, #4
	ble .L_02008fee
	adds r7, #18
	mov r8, r7
.L_02008fee:
	ldr r2, [sp, #36]
	mov r0, r11
	ldr r1, [r0, r2]
	ldr r0, [sp, #56]
	mov r2, r8
	bl Func_02002330
	ldr r3, [sp, #36]
	mov r2, r11
	adds r2, #200
	str r0, [r2, r3]
	mov r4, r10
	movs r3, #99
	strb r3, [r4]
	ldr r1, [sp, #36]
	movs r0, #1
	add r10, r0
	ldr r3, .L_02009178
	ldr r0, [r2, r1]
	movs r1, #128
	mov lr, r3
	.2byte 0xf800
	adds r6, #1
	movs r2, #4
	add r11, r2
	cmp r6, #50
	bne .L_02008fc4
	bl Resource_FindFreeEntry
	adds r5, r0, #0
	cmp r5, #95
	bgt .L_0200905e
	bl Func_02002300
	movs r1, #128
	movs r3, #0
	lsls r1, r1, #23
	mov r2, r9
	adds r0, r5, #0
	str r3, [sp, #0]
	bl RenderOutput_Create
	adds r1, r0, #0
	add r0, sp, #108
	str r1, [r0]
	mov r4, r9
	movs r3, #12
	ldrsh r1, [r4, r3]
	movs r3, #14
	ldrsh r2, [r4, r3]
	lsls r1, r1, #3
	lsls r2, r2, #3
	subs r1, #2
	adds r2, #12
	bl Func_02002400
.L_0200905e:
	bl Resource_FindFreeEntry
	adds r5, r0, #0
	cmp r5, #95
	bgt .L_020090a6
	bl Func_02002308
	movs r1, #128
	movs r3, #0
	lsls r1, r1, #23
	mov r2, r9
	adds r0, r5, #0
	str r3, [sp, #0]
	bl RenderOutput_Create
	adds r1, r0, #0
	add r0, sp, #92
	str r1, [r0]
	movs r3, #255
	ldrb r2, [r1, #25]
	strb r3, [r1, #15]
	movs r3, #13
	negs r3, r3
	ands r3, r2
	strb r3, [r1, #25]
	ldr r2, [sp, #56]
	movs r4, #12
	ldrsh r1, [r2, r4]
	movs r3, #14
	ldrsh r2, [r2, r3]
	lsls r1, r1, #3
	lsls r2, r2, #3
	subs r1, #4
	adds r2, #12
	bl Func_02002400
.L_020090a6:
	movs r4, #0
	movs r0, #1
	str r0, [sp, #24]
	str r4, [sp, #32]
	str r4, [sp, #28]
	mov r2, r9
	movs r1, #12
	ldrsh r3, [r2, r1]
	movs r0, #0
	lsls r3, r3, #3
	subs r3, #8
	str r3, [sp, #20]
	movs r4, #14
	ldrsh r3, [r2, r4]
	str r0, [sp, #12]
	lsls r3, r3, #3
	adds r3, #11
	str r3, [sp, #16]
.L_020090ca:
	ldr r2, [sp, #12]
	movs r1, #1
	negs r1, r1
	mov r10, r1
	cmp r2, #2
	bne .L_020090da
	movs r3, #1
	str r3, [sp, #12]
.L_020090da:
	movs r3, #15
	movs r4, #1
	str r3, [sp, #4]
	mov r0, r9
	ldr r1, [sp, #32]
	ldr r2, [sp, #28]
	ldr r3, [sp, #24]
	str r4, [sp, #0]
	bl Func_02000db4
	ldr r0, [sp, #12]
	cmp r0, #0
	beq .L_020090f6
	b .L_020091fc
.L_020090f6:
	ldr r3, .L_0200917c
	movs r2, #16
	ldr r3, [r3, #12]
	ands r3, r2
	cmp r3, #0
	beq .L_0200912e
	movs r0, #111
	bl Func_02002408
	ldr r0, [sp, #52]
	ldr r1, [sp, #48]
	adds r0, #1
	cmp r1, #2
	bne .L_02009126
	adds r3, r0, #0
	cmp r0, #0
	bge .L_0200911c
	ldr r3, [sp, #52]
	adds r3, #16
.L_0200911c:
	asrs r3, r3, #4
	str r3, [sp, #52]
	lsls r3, r3, #4
	subs r0, r0, r3
	b .L_0200912c
.L_02009126:
	movs r1, #17
	bl __modsi3
.L_0200912c:
	str r0, [sp, #52]
.L_0200912e:
	ldr r3, .L_0200917c
	movs r2, #32
	ldr r3, [r3, #12]
	ands r3, r2
	cmp r3, #0
	beq .L_0200918c
	movs r0, #111
	bl Func_02002408
	ldr r2, [sp, #48]
	cmp r2, #2
	bne .L_02009180
	ldr r2, [sp, #52]
	adds r2, #15
	adds r3, r2, #0
	cmp r2, #0
	bge .L_02009154
	ldr r3, [sp, #52]
	adds r3, #30
.L_02009154:
	asrs r3, r3, #4
	str r3, [sp, #52]
	lsls r3, r3, #4
	subs r2, r2, r3
	str r2, [sp, #52]
	b .L_0200918c
.L_02009160:
	.4byte Data_02002440
.L_02009164:
	.4byte Data_02002454
.L_02009168:
	.4byte Data_02002468
.L_0200916c:
	.4byte Data_0200247c
.L_02009170:
	.4byte 0x00000d8d
.L_02009174:
	.4byte Data_0200274a
.L_02009178:
	.4byte IwramClearWords
.L_0200917c:
	.4byte gInput
.L_02009180:
	ldr r0, [sp, #52]
	movs r1, #17
	adds r0, #16
	bl __modsi3
	str r0, [sp, #52]
.L_0200918c:
	ldr r3, .L_02009400
	movs r2, #128
	ldr r3, [r3, #12]
	ands r3, r2
	cmp r3, #0
	beq .L_020091c4
	movs r0, #111
	bl Func_02002408
	ldr r1, [sp, #48]
	adds r1, #1
	adds r3, r1, #0
	cmp r1, #0
	bge .L_020091ac
	ldr r3, [sp, #48]
	adds r3, #4
.L_020091ac:
	asrs r3, r3, #2
	str r3, [sp, #48]
	lsls r3, r3, #2
	subs r1, r1, r3
	ldr r3, [sp, #52]
	str r1, [sp, #48]
	cmp r3, #16
	bne .L_020091c4
	cmp r1, #2
	bne .L_020091c4
	movs r4, #3
	str r4, [sp, #48]
.L_020091c4:
	ldr r3, .L_02009400
	movs r2, #64
	ldr r3, [r3, #12]
	ands r3, r2
	cmp r3, #0
	beq .L_020091fc
	movs r0, #111
	bl Func_02002408
	ldr r1, [sp, #48]
	adds r1, #3
	adds r3, r1, #0
	cmp r1, #0
	bge .L_020091e4
	ldr r3, [sp, #48]
	adds r3, #6
.L_020091e4:
	asrs r3, r3, #2
	ldr r0, [sp, #52]
	str r3, [sp, #48]
	lsls r3, r3, #2
	subs r1, r1, r3
	str r1, [sp, #48]
	cmp r0, #16
	bne .L_02009206
	cmp r1, #2
	bne .L_020091fc
	movs r1, #1
	str r1, [sp, #48]
.L_020091fc:
	ldr r3, [sp, #52]
	movs r2, #6
	str r2, [sp, #24]
	cmp r3, #16
	beq .L_0200920a
.L_02009206:
	movs r4, #1
	str r4, [sp, #24]
.L_0200920a:
	ldr r0, [sp, #52]
	ldr r1, [sp, #48]
	str r0, [sp, #32]
	str r1, [sp, #28]
	cmp r0, #4
	ble .L_0200921c
	adds r2, r0, #0
	adds r2, #1
	str r2, [sp, #32]
.L_0200921c:
	ldr r3, [sp, #52]
	cmp r3, #9
	ble .L_02009228
	ldr r4, [sp, #32]
	adds r4, #1
	str r4, [sp, #32]
.L_02009228:
	ldr r0, [sp, #52]
	cmp r0, #14
	ble .L_02009234
	ldr r1, [sp, #32]
	adds r1, #1
	str r1, [sp, #32]
.L_02009234:
	ldr r2, [sp, #52]
	cmp r2, #15
	ble .L_02009240
	ldr r3, [sp, #32]
	adds r3, #1
	str r3, [sp, #32]
.L_02009240:
	movs r3, #14
	movs r4, #1
	mov r0, r9
	str r3, [sp, #4]
	ldr r1, [sp, #32]
	ldr r2, [sp, #48]
	ldr r3, [sp, #24]
	str r4, [sp, #0]
	bl Func_02000db4
	mov r1, r9
	movs r0, #12
	ldrsh r3, [r1, r0]
	ldr r2, [sp, #52]
	mov r0, r9
	adds r3, r3, r2
	lsls r1, r3, #3
	movs r4, #14
	ldrsh r3, [r0, r4]
	ldr r4, [sp, #48]
	adds r2, r1, #0
	adds r3, r3, r4
	lsls r3, r3, #3
	adds r0, r3, #0
	ldr r3, [sp, #52]
	subs r2, #8
	adds r0, #11
	cmp r3, #4
	ble .L_0200927c
	adds r2, r1, #0
.L_0200927c:
	ldr r4, [sp, #52]
	cmp r4, #9
	ble .L_02009284
	adds r2, #8
.L_02009284:
	ldr r1, [sp, #52]
	cmp r1, #14
	ble .L_0200928c
	adds r2, #8
.L_0200928c:
	ldr r3, [sp, #52]
	cmp r3, #15
	ble .L_02009294
	adds r2, #8
.L_02009294:
	ldr r4, [sp, #20]
	ldr r1, [sp, #16]
	subs r3, r2, r4
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	adds r4, r4, r3
	subs r3, r0, r1
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	adds r1, r1, r3
	str r1, [sp, #16]
	ldr r2, [sp, #16]
	add r0, sp, #108
	adds r1, r4, #0
	movs r3, #1
	str r4, [sp, #20]
	bl Func_020023f8
	add r0, sp, #108
	bl Func_020023f0
	movs r1, #50
	ldr r0, [sp, #60]
	bl __modsi3
	movs r1, #10
	adds r5, r0, #0
	bl __divsi3
	ldr r4, [sp, #56]
	lsls r0, r0, #1
	movs r2, #14
	ldrsh r3, [r4, r2]
	movs r1, #10
	adds r0, r0, r3
	ldr r3, .L_02009404
	lsls r0, r0, #3
	ldr r3, [r3]
	subs r7, r0, #2
	lsrs r6, r3, #1
	movs r0, #7
	mov r8, r0
	ands r6, r0
	adds r0, r5, #0
	bl __modsi3
	ldr r4, [sp, #56]
	lsls r2, r0, #1
	movs r1, #12
	ldrsh r3, [r4, r1]
	adds r2, r2, r0
	lsls r2, r2, #2
	lsls r3, r3, #3
	adds r2, r2, r3
	adds r1, r2, #0
	adds r1, #16
	cmp r0, #4
	ble .L_0200930e
	adds r1, #10
.L_0200930e:
	ldr r2, .L_02009408
	mov r0, r8
	ldrsb r3, [r2, r6]
	adds r1, r1, r3
	adds r3, r6, #5
	ands r3, r0
	ldrsb r3, [r2, r3]
	add r0, sp, #92
	adds r7, r7, r3
	adds r2, r7, #0
	movs r3, #1
	bl Func_020023f8
	add r0, sp, #92
	bl Func_020023f0
	ldr r1, [sp, #8]
	cmp r1, #0
	ble .L_02009346
	adds r1, #1
	str r1, [sp, #8]
	cmp r1, #8
	bne .L_02009346
	ldr r3, [sp, #60]
	movs r2, #0
	adds r3, #1
	str r2, [sp, #8]
	str r3, [sp, #60]
.L_02009346:
	ldr r4, [sp, #8]
	cmp r4, #0
	beq .L_0200934e
	b .L_02009692
.L_0200934e:
	ldr r0, [sp, #52]
	ldr r3, .L_02009400
	cmp r0, #16
	bne .L_0200936a
	ldr r1, [sp, #48]
	cmp r1, #0
	bne .L_02009360
	ldr r3, [r3, #12]
	b .L_0200936c
.L_02009360:
	ldr r2, [sp, #48]
	cmp r2, #1
	bne .L_0200936a
	ldr r3, [r3, #12]
	b .L_0200936c
.L_0200936a:
	ldr r3, [r3, #4]
.L_0200936c:
	lsls r3, r3, #16
	asrs r2, r3, #16
	movs r3, #128
	lsls r2, r2, #16
	lsls r3, r3, #9
	ands r3, r2
	cmp r3, #0
	beq .L_02009418
	ldr r3, [sp, #12]
	cmp r3, #0
	bne .L_02009418
	ldr r0, [sp, #52]
	movs r4, #0
	mov r10, r4
	cmp r0, #16
	bne .L_020093b0
	ldr r1, [sp, #48]
	cmp r1, #0
	bne .L_02009398
	movs r2, #1
	mov r10, r2
	b .L_02009418
.L_02009398:
	ldr r3, [sp, #48]
	cmp r3, #1
	bne .L_020093a4
	movs r4, #2
	mov r10, r4
	b .L_02009418
.L_020093a4:
	ldr r0, [sp, #48]
	cmp r0, #3
	bne .L_02009418
	movs r1, #3
	mov r10, r1
	b .L_02009418
.L_020093b0:
	ldr r2, [sp, #60]
	ldr r3, [sp, #44]
	cmp r2, r3
	bge .L_02009418
	ldr r4, [sp, #48]
	ldr r0, [sp, #52]
	ldr r2, .L_0200940c
	lsls r3, r4, #4
	adds r3, r0, r3
	ldrb r5, [r2, r3]
	cmp r5, #63
	bgt .L_02009418
	movs r0, #112
	bl Func_02002408
	ldr r3, .L_02009410
	ldr r1, [sp, #60]
	strb r5, [r3, r1]
	ldr r0, [sp, #60]
	movs r1, #10
	bl __modsi3
	ldr r3, [sp, #44]
	ldr r2, [sp, #60]
	subs r3, #1
	cmp r2, r3
	bge .L_02009414
	adds r0, r2, #0
	movs r1, #50
	bl __modsi3
	cmp r0, #49
	bne .L_020093f8
	movs r3, #1
	str r3, [sp, #8]
	b .L_02009418
.L_020093f8:
	ldr r4, [sp, #60]
	adds r4, #1
	str r4, [sp, #60]
	b .L_02009418
.L_02009400:
	.4byte gInput
.L_02009404:
	.4byte gFrameTick
.L_02009408:
	.4byte Data_02002438
.L_0200940c:
	.4byte Data_0200265e
.L_02009410:
	.4byte Data_0200274a
.L_02009414:
	movs r0, #2
	str r0, [sp, #12]
.L_02009418:
	mov r1, r10
	cmp r1, #0
	bne .L_02009426
	ldr r2, [sp, #12]
	cmp r2, #1
	beq .L_02009426
	b .L_02009692
.L_02009426:
	ldr r1, .L_02009758
	movs r2, #8
	ldr r3, [r1, #4]
	ands r3, r2
	cmp r3, #0
	bne .L_02009440
	mov r3, r10
	cmp r3, #3
	beq .L_02009440
	ldr r4, [sp, #12]
	cmp r4, #1
	beq .L_02009440
	b .L_02009602
.L_02009440:
	ldr r7, .L_0200975c
	ldr r1, [sp, #44]
	adds r2, r7, #0
	ldr r0, .L_02009760
	bl Func_02002068
	mov r10, r0
	mov r6, r10
	subs r6, #2
	adds r1, r7, #0
	adds r0, r6, #0
	bl Func_0200217c
	movs r3, #2
	adds r5, r0, #0
	movs r0, #0
	str r0, [sp, #12]
	str r3, [sp, #0]
	movs r1, #12
	movs r3, #3
	movs r0, #5
	movs r2, #20
	bl UiWindow_Create
	lsls r5, r5, #16
	asrs r5, r5, #16
	ldrb r3, [r7, r6]
	lsls r5, r5, #16
	lsrs r1, r5, #16
	lsrs r5, r5, #24
	mov r8, r0
	cmp r3, r5
	bne .L_0200955a
	mov r3, r10
	subs r3, #1
	ldrb r3, [r7, r3]
	movs r2, #255
	ands r2, r1
	cmp r3, r2
	bne .L_0200955a
	ldr r0, .L_02009764
	mov r1, r8
	movs r2, #0
	movs r3, #0
	bl UiText_DrawCharacterAtOffset
	movs r0, #112
	bl Func_02002408
.L_020094a2:
	movs r0, #1
	bl WaitFrames
	ldr r3, .L_02009758
	movs r2, #1
	ldr r3, [r3, #4]
	ands r3, r2
	cmp r3, #0
	beq .L_020094a2
	movs r0, #112
	bl Func_02002408
	bl Func_02002350
	ldr r5, .L_02009768
	movs r1, #133
	lsls r1, r1, #2
	adds r2, r5, r1
	movs r3, #0
	str r3, [r2]
	movs r0, #4
	bl Party_RemoveActiveOwner
	movs r0, #5
	bl Party_RemoveActiveOwner
	movs r0, #6
	bl Party_RemoveActiveOwner
	movs r0, #7
	bl Party_RemoveActiveOwner
	movs r0, #0
	bl Party_AddActiveOwner
	movs r0, #1
	bl Party_AddActiveOwner
	movs r0, #2
	bl Party_AddActiveOwner
	movs r0, #3
	bl Party_AddActiveOwner
	ldr r1, .L_0200975c
	ldr r0, [sp, #64]
	bl Func_020018e0
	mov r0, r8
	movs r1, #2
	bl UiWork_Finalize
	movs r1, #2
	ldr r0, [sp, #56]
	bl UiWork_Finalize
	mov r0, r9
	movs r1, #2
	bl UiWork_Finalize
	movs r2, #147
	lsls r2, r2, #1
	adds r2, #255
	adds r3, r5, r2
	ldrb r0, [r3]
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #38
	adds r5, r5, r3
	ldrb r1, [r5]
	bl Func_02002310
	movs r0, #74
	bl Func_02002408
	movs r0, #6
	bl WaitFrames
	movs r0, #0
	bl Func_020022f8
	bl Func_020023e8
	movs r2, #244
	ldr r3, .L_0200976c
	ldr r0, .L_02009770
	ldr r1, .L_02009774
	lsls r2, r2, #4
	mov lr, r3
	.2byte 0xf800
	movs r0, #0
	b .L_020097f0
.L_0200955a:
	movs r0, #113
	bl Func_02002408
	ldr r0, .L_02009778
	mov r1, r8
	movs r2, #0
	movs r3, #0
	bl UiText_DrawCharacterAtOffset
.L_0200956c:
	movs r0, #1
	bl WaitFrames
	ldr r1, .L_02009758
	movs r2, #1
	ldr r3, [r1, #4]
	ands r3, r2
	cmp r3, #0
	bne .L_02009588
	ldr r3, [r1, #4]
	movs r2, #2
	ands r3, r2
	cmp r3, #0
	beq .L_0200956c
.L_02009588:
	movs r1, #2
	mov r0, r8
	bl UiWork_Finalize
	mov r0, r9
	bl RenderOutput_RedrawSavedRect
	ldr r0, .L_0200977c
	mov r1, r9
	movs r2, #0
	movs r3, #0
	bl UiText_DrawStringInWindow
	ldr r0, .L_02009780
	mov r1, r9
	movs r2, #0
	movs r3, #8
	bl UiText_DrawStringInWindow
	ldr r0, .L_02009784
	mov r1, r9
	movs r2, #0
	movs r3, #16
	bl UiText_DrawStringInWindow
	ldr r0, .L_02009788
	mov r1, r9
	movs r2, #0
	movs r3, #24
	bl UiText_DrawStringInWindow
	ldr r6, .L_0200978c
	movs r3, #5
	str r3, [sp, #0]
	mov r0, r9
	movs r1, #20
	movs r2, #0
	movs r3, #20
	bl UiWindow_DrawDividerLine
	adds r0, r6, #0
	mov r1, r9
	adds r6, #1
	movs r2, #160
	movs r3, #0
	bl UiText_DrawCharacterAtOffset
	adds r0, r6, #0
	mov r1, r9
	movs r2, #160
	movs r3, #8
	adds r6, #1
	bl UiText_DrawCharacterAtOffset
	adds r0, r6, #0
	mov r1, r9
	movs r2, #160
	movs r3, #24
	bl UiText_DrawCharacterAtOffset
	b .L_02009692
.L_02009602:
	ldr r3, [r1, #12]
	movs r2, #128
	lsls r2, r2, #2
	ands r3, r2
	cmp r3, #0
	bne .L_02009614
	mov r4, r10
	cmp r4, #2
	bne .L_02009628
.L_02009614:
	ldr r0, [sp, #60]
	cmp r0, #0
	ble .L_02009692
	movs r0, #111
	bl Func_02002408
	ldr r1, [sp, #60]
	subs r1, #1
	str r1, [sp, #60]
	b .L_02009692
.L_02009628:
	ldr r3, [r1, #12]
	movs r2, #128
	lsls r2, r2, #1
	ands r3, r2
	cmp r3, #0
	bne .L_0200963a
	mov r2, r10
	cmp r2, #1
	bne .L_02009652
.L_0200963a:
	ldr r3, [sp, #44]
	ldr r4, [sp, #60]
	subs r3, #1
	cmp r4, r3
	bge .L_02009692
	movs r0, #111
	bl Func_02002408
	ldr r0, [sp, #60]
	adds r0, #1
	str r0, [sp, #60]
	b .L_02009692
.L_02009652:
	ldr r3, [r1, #4]
	movs r2, #2
	ands r3, r2
	cmp r3, #0
	beq .L_02009692
	ldr r1, [sp, #60]
	cmp r1, #0
	ble .L_02009670
	movs r0, #111
	bl Func_02002408
	ldr r2, [sp, #60]
	subs r2, #1
	str r2, [sp, #60]
	b .L_02009692
.L_02009670:
	movs r0, #113
	bl Func_02002408
	movs r1, #2
	ldr r0, [sp, #56]
	bl UiWork_Finalize
	mov r0, r9
	movs r1, #2
	bl UiWork_Finalize
	movs r0, #74
	bl Func_02002408
	movs r0, #1
	negs r0, r0
	b .L_020097f0
.L_02009692:
	ldr r0, [sp, #60]
	movs r1, #50
	bl __divsi3
	mov r11, r0
	ldr r0, [sp, #56]
	bl RenderOutput_RedrawSavedRect
	movs r6, #0
	movs r5, #2
.L_020096a6:
	adds r2, r5, #0
	ldr r0, [sp, #56]
	movs r1, #0
	movs r3, #18
	adds r6, #1
	str r5, [sp, #0]
	bl UiWindow_DrawDividerLine
	adds r5, #2
	cmp r6, #4
	bne .L_020096a6
	ldr r3, [sp, #40]
	movs r6, #0
	cmp r3, #0
	beq .L_020096f6
	negs r3, r3
	adds r5, r3, #0
	adds r5, #18
.L_020096ca:
	movs r4, #243
	lsls r4, r4, #8
	adds r4, #1
	adds r1, r6, r4
	cmp r6, r11
	bne .L_020096de
	movs r0, #243
	lsls r0, r0, #8
	adds r0, #11
	adds r1, r6, r0
.L_020096de:
	movs r3, #0
	str r3, [sp, #0]
	adds r2, r5, #0
	ldr r0, [sp, #56]
	movs r3, #10
	bl UiWindow_SetTilemapEntry
	ldr r1, [sp, #40]
	adds r6, #1
	adds r5, #1
	cmp r6, r1
	bne .L_020096ca
.L_020096f6:
	ldr r2, [sp, #44]
	mov r10, r2
	cmp r2, #49
	ble .L_02009716
	ldr r3, [sp, #40]
	subs r3, #1
	cmp r3, r11
	bne .L_02009712
	ldr r0, [sp, #44]
	movs r1, #50
	bl __modsi3
	mov r10, r0
	b .L_02009716
.L_02009712:
	movs r3, #50
	mov r10, r3
.L_02009716:
	mov r4, r10
	movs r6, #0
	cmp r4, #0
	beq .L_020097b4
	ldr r0, [sp, #36]
	ldr r1, [sp, #36]
	movs r2, #200
	adds r0, #200
	lsls r2, r2, #1
	mov r8, r0
	adds r7, r1, r2
.L_0200972c:
	movs r3, #50
	mov r0, r11
	muls r0, r3
	ldr r4, .L_02009760
	adds r3, r0, #0
	adds r0, r6, r3
	ldrb r2, [r4, r0]
	ldrb r3, [r7]
	adds r1, r2, #0
	cmp r1, r3
	beq .L_020097a8
	strb r2, [r7]
	cmp r1, #99
	bne .L_02009794
	mov r1, r8
	ldr r0, [r1]
	ldr r3, .L_02009790
	movs r1, #128
	mov lr, r3
	.2byte 0xf800
	b .L_020097a8
	.2byte 0x0000
.L_02009758:
	.4byte gInput
.L_0200975c:
	.4byte Data_0200288a
.L_02009760:
	.4byte Data_0200274a
.L_02009764:
	.4byte 0x00000d91
.L_02009768:
	.4byte gPartyState
.L_0200976c:
	.4byte IwramCopyWords
.L_02009770:
	.4byte Data_02012f94 + 0xac
.L_02009774:
	.4byte GameFlagBytes
.L_02009778:
	.4byte 0x00000d90
.L_0200977c:
	.4byte Data_02002440
.L_02009780:
	.4byte Data_02002454
.L_02009784:
	.4byte Data_02002468
.L_02009788:
	.4byte Data_0200247c
.L_0200978c:
	.4byte 0x00000d8d
.L_02009790:
	.4byte IwramClearWords
.L_02009794:
	add r5, sp, #68
	ldrb r0, [r4, r0]
	adds r1, r5, #0
	bl Func_02000a70
	mov r2, r8
	ldrh r0, [r5]
	ldr r1, [r2]
	bl Func_02002328
.L_020097a8:
	movs r3, #4
	adds r6, #1
	add r8, r3
	adds r7, #1
	cmp r6, r10
	bne .L_0200972c
.L_020097b4:
	cmp r6, #50
	beq .L_020097e8
	ldr r4, [sp, #36]
	lsls r3, r6, #2
	movs r0, #200
	adds r3, r3, r4
	lsls r0, r0, #1
	adds r7, r3, #0
	adds r3, r6, r0
	adds r7, #200
	adds r5, r3, r4
.L_020097ca:
	ldrb r3, [r5]
	cmp r3, #99
	beq .L_020097de
	movs r3, #99
	strb r3, [r5]
	ldr r0, [r7]
	ldr r3, .L_02009800
	movs r1, #128
	mov lr, r3
	.2byte 0xf800
.L_020097de:
	adds r6, #1
	adds r7, #4
	adds r5, #1
	cmp r6, #50
	bne .L_020097ca
.L_020097e8:
	movs r0, #1
	bl WaitFrames
	b .L_020090ca
.L_020097f0:
	add sp, #124
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009800:
	.4byte IwramClearWords
	.section .text.x02009804,"ax",%progbits
	.global Func_02001804
	.thumb_func
Func_02001804:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r7, r0, #0
	movs r3, #192
	movs r0, #192
	lsls r3, r3, #18
	lsls r0, r0, #2
	ldr r5, [r3, #60]
	adds r6, r1, #0
	bl Runtime_BumpAllocateAlternatePool
	mov r8, r0
	mov r1, r8
	adds r0, r6, #0
	bl Func_020021f8
	movs r1, #14
	ldrsh r3, [r7, r1]
	movs r1, #12
	ldrsh r2, [r7, r1]
	lsls r3, r3, #5
	adds r3, r3, r2
	ldr r2, .L_0200986c
	lsls r3, r3, #1
	adds r5, r5, r3
	adds r1, r3, r2
	adds r5, #8
	movs r4, #0
.L_0200983e:
	movs r0, #0
.L_02009840:
	ldrh r3, [r7, #8]
	ldr r2, .L_02009868
	muls r3, r4
	adds r3, r3, r0
	orrs r3, r2
	lsls r3, r3, #16
	asrs r3, r3, #16
	adds r0, #1
	strh r3, [r1]
	strh r3, [r5]
	adds r1, #2
	adds r5, #2
	cmp r0, #15
	ble .L_02009840
	adds r4, #1
	adds r1, #32
	adds r5, #32
	cmp r4, #7
	ble .L_0200983e
	b .L_02009870
.L_02009868:
	.4byte 0xfffff000
.L_0200986c:
	.4byte 0x06002000
.L_02009870:
	mov r0, r8
	bl Sys_Free
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.section .text.x0200987c,"ax",%progbits
	.global Func_0200187c
	.thumb_func
Func_0200187c:
	push {r5, r6, lr}
	adds r6, r0, #0
	movs r3, #192
	movs r0, #192
	lsls r3, r3, #18
	lsls r0, r0, #2
	ldr r5, [r3, #60]
	bl Runtime_BumpAllocateAlternatePool
	movs r1, #14
	ldrsh r3, [r6, r1]
	movs r1, #12
	ldrsh r2, [r6, r1]
	lsls r3, r3, #5
	adds r3, r3, r2
	ldr r2, .L_020098d4
	lsls r3, r3, #1
	adds r5, r5, r3
	adds r1, r3, r2
	adds r5, #8
	movs r6, #0
.L_020098a6:
	movs r4, #0
.L_020098a8:
	lsls r3, r6, #4
	ldr r2, .L_020098d0
	adds r3, r3, r4
	adds r3, #32
	orrs r3, r2
	lsls r3, r3, #16
	asrs r3, r3, #16
	adds r4, #1
	strh r3, [r1]
	strh r3, [r5]
	adds r1, #2
	adds r5, #2
	cmp r4, #15
	ble .L_020098a8
	adds r6, #1
	adds r1, #32
	adds r5, #32
	cmp r6, #7
	ble .L_020098a6
	b .L_020098d8
.L_020098d0:
	.4byte 0xfffff000
.L_020098d4:
	.4byte 0x06002000
.L_020098d8:
	bl Sys_Free
	pop {r5, r6, pc}
	.2byte 0x0000
	.section .text.x020098e0,"ax",%progbits
	.global Func_020018e0
	.thumb_func
Func_020018e0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	mov r10, r1
	mov r2, r10
	ldrb r3, [r2, #1]
	ldrb r1, [r1]
	lsls r3, r3, #8
	orrs r1, r3
	ldrb r3, [r2, #2]
	ldrb r2, [r2, #3]
	lsls r3, r3, #16
	orrs r1, r3
	movs r3, #240
	ands r3, r2
	lsls r3, r3, #20
	orrs r1, r3
	movs r3, #15
	mov r9, r3
	mov r8, r1
	mov r1, r9
	ands r1, r2
	mov r2, r10
	ldrb r3, [r2, #4]
	sub sp, #40
	lsls r3, r3, #4
	orrs r1, r3
	ldrb r3, [r2, #5]
	ldrb r6, [r2, #7]
	lsls r3, r3, #12
	orrs r1, r3
	ldrb r3, [r2, #6]
	mov r11, r0
	lsls r3, r3, #20
	orrs r1, r3
	ldrb r3, [r2, #8]
	mov r9, r1
	str r3, [sp, #4]
	cmp r0, #2
	beq .L_020099fe
	negs r3, r0
	orrs r3, r0
	lsrs r3, r3, #31
	adds r3, #8
	movs r7, #0
	movs r4, #4
	adds r0, r3, r2
.L_02009946:
	ldrb r2, [r0]
	ldrb r3, [r0, #1]
	lsls r2, r2, #24
	lsls r3, r3, #16
	orrs r2, r3
	ldrb r3, [r0, #2]
	add r5, sp, #8
	lsls r3, r3, #8
	orrs r2, r3
	ldrb r3, [r0, #3]
	lsls r1, r7, #4
	orrs r2, r3
	str r2, [r5, r1]
	adds r7, #1
	ldrb r2, [r0, #4]
	ldrb r3, [r0, #5]
	lsls r2, r2, #24
	lsls r3, r3, #16
	orrs r2, r3
	ldrb r3, [r0, #6]
	ldrb r1, [r0, #7]
	lsls r3, r3, #8
	orrs r2, r3
	orrs r2, r1
	str r2, [r5, r4]
	lsls r1, r1, #28
	ldrb r3, [r0, #8]
	adds r2, r4, #4
	lsls r3, r3, #20
	orrs r1, r3
	ldrb r3, [r0, #9]
	lsls r3, r3, #12
	orrs r1, r3
	ldrb r3, [r0, #10]
	lsls r3, r3, #4
	orrs r1, r3
	ldrb r3, [r0, #11]
	lsrs r3, r3, #4
	orrs r1, r3
	str r1, [r5, r2]
	adds r1, r4, #0
	ldrb r2, [r0, #11]
	ldrb r3, [r0, #12]
	lsls r2, r2, #28
	lsls r3, r3, #20
	orrs r2, r3
	ldrb r3, [r0, #13]
	adds r1, #8
	lsls r3, r3, #12
	orrs r2, r3
	ldrb r3, [r0, #14]
	adds r4, #16
	lsls r3, r3, #4
	orrs r2, r3
	str r2, [r5, r1]
	adds r0, #15
	cmp r7, #2
	bne .L_02009946
	movs r7, #0
.L_020099bc:
	ldr r2, .L_02009a1c
	lsls r3, r7, #2
	ldr r0, [r2, r3]
	bl Owner_GetState
	lsls r1, r7, #3
	ldr r2, [r5, r1]
	movs r4, #128
	lsls r4, r4, #1
	adds r0, #16
	lsrs r3, r2, #21
	adds r4, #255
	strh r3, [r0]
	adds r1, #4
	lsls r3, r2, #11
	ands r2, r4
	strh r2, [r0, #8]
	ldr r2, [r5, r1]
	lsrs r3, r3, #21
	strh r3, [r0, #2]
	lsrs r3, r2, #22
	ands r3, r4
	strh r3, [r0, #10]
	lsrs r3, r2, #12
	ands r3, r4
	strh r3, [r0, #12]
	lsrs r2, r2, #4
	movs r3, #127
	ands r2, r3
	adds r7, #1
	strb r2, [r0, #14]
	cmp r7, #4
	bne .L_020099bc
.L_020099fe:
	movs r7, #0
.L_02009a00:
	adds r2, r6, #0
	movs r3, #1
	asrs r2, r7
	ands r2, r3
	adds r5, r7, #1
	ldr r3, .L_02009a20
	lsls r0, r7, #1
	cmp r2, #0
	beq .L_02009a24
	ldrh r0, [r3, r0]
	bl GameFlag_SetBit
	b .L_02009a2a
	.2byte 0x0000
.L_02009a1c:
	.4byte Data_020026a0
.L_02009a20:
	.4byte Data_02002490
.L_02009a24:
	ldrh r0, [r3, r0]
	bl GameFlag_ClearBit
.L_02009a2a:
	adds r7, r5, #0
	cmp r7, #6
	bne .L_02009a00
	movs r7, #0
	movs r6, #0
.L_02009a34:
	ldr r2, .L_02009b78
	lsls r3, r7, #2
	ldr r0, [r2, r3]
	bl Owner_GetState
	mov r3, r8
	lsrs r3, r6
	movs r2, #127
	adds r5, r0, #0
	ands r3, r2
	strb r3, [r5, #15]
	adds r0, r7, #0
	ldrb r1, [r5, #15]
	bl Owner_GetLevelThreshold
	movs r1, #146
	lsls r1, r1, #1
	adds r5, r5, r1
	adds r7, #1
	str r0, [r5]
	adds r6, #7
	cmp r7, #4
	bne .L_02009a34
	movs r0, #0
	bl Trade_GetOfferState
	movs r2, #148
	lsls r2, r2, #1
	adds r0, r0, r2
	movs r3, #0
	str r3, [r0]
	movs r7, #0
.L_02009a74:
	ldr r3, .L_02009b78
	lsls r2, r7, #2
	ldr r0, [r3, r2]
	bl Owner_GetState
	movs r6, #0
	adds r5, r0, #0
.L_02009a82:
	adds r0, r5, #0
	ldr r3, .L_02009b7c
	adds r0, #248
	movs r1, #40
	adds r6, #1
	mov lr, r3
	.2byte 0xf800
	cmp r6, #4
	bne .L_02009a82
	adds r7, #1
	cmp r7, #4
	bne .L_02009a74
	movs r7, #0
.L_02009a9c:
	adds r0, r7, #0
	movs r1, #7
	bl __divsi3
	movs r1, #7
	adds r5, r0, #0
	adds r0, r7, #0
	bl __modsi3
	adds r3, r0, #0
	lsls r0, r5, #2
	adds r0, r0, r5
	lsls r0, r0, #2
	adds r0, r0, r3
	adds r0, #48
	adds r7, #1
	bl GameFlag_ClearBit
	cmp r7, #28
	bne .L_02009a9c
	movs r7, #0
.L_02009ac6:
	mov r3, r9
	lsrs r3, r7
	movs r2, #1
	ands r3, r2
	cmp r3, #0
	beq .L_02009aec
	movs r1, #7
	adds r0, r7, #0
	bl __divsi3
	movs r1, #7
	adds r5, r0, #0
	adds r0, r7, #0
	bl __modsi3
	adds r1, r0, #0
	adds r0, r5, #0
	bl Djinn_AddToLeastLoadedOwner
.L_02009aec:
	adds r7, #1
	cmp r7, #28
	bne .L_02009ac6
	mov r3, r11
	cmp r3, #0
	beq .L_02009b26
	ldr r3, .L_02009b78
	movs r7, #0
	ldr r0, [r3]
	bl Owner_GetState
.L_02009b02:
	ldr r3, [sp, #4]
	movs r2, #1
	asrs r3, r7
	ands r3, r2
	cmp r3, #0
	beq .L_02009b18
	ldr r3, .L_02009b80
	lsls r2, r7, #1
	ldrh r0, [r3, r2]
	bl PartyInventory_Add
.L_02009b18:
	adds r7, #1
	cmp r7, #8
	bne .L_02009b02
	mov r1, r11
	cmp r1, #0
	beq .L_02009b26
	b .L_02009c4a
.L_02009b26:
	movs r2, #39
	mov r8, r2
	movs r5, #0
	movs r7, #0
.L_02009b2e:
	ldr r2, .L_02009b78
	lsls r3, r7, #2
	ldr r0, [r2, r3]
	bl Owner_GetState
	mov r4, r8
	adds r0, #216
	movs r6, #0
	mov r12, r0
	add r4, r10
.L_02009b42:
	ldrb r1, [r4, #1]
	ldrb r2, [r4]
	movs r3, #7
	subs r3, r3, r5
	asrs r1, r3
	adds r0, r5, #1
	ldr r3, .L_02009b74
	lsls r2, r0
	orrs r2, r1
	ands r2, r3
	mov r3, r12
	strh r2, [r3]
	movs r1, #2
	movs r2, #1
	adds r5, r0, #0
	add r12, r1
	adds r4, #1
	add r8, r2
	cmp r5, #7
	bne .L_02009b84
	movs r5, #0
	adds r4, #1
	add r8, r2
	b .L_02009b84
	.2byte 0x0000
.L_02009b74:
	.4byte 0x000001ff
.L_02009b78:
	.4byte Data_020026a0
.L_02009b7c:
	.4byte IwramClearWords
.L_02009b80:
	.4byte Data_020026b0
.L_02009b84:
	adds r6, #1
	cmp r6, #15
	bne .L_02009b42
	adds r7, #1
	cmp r7, #4
	bne .L_02009b2e
	movs r3, #107
	movs r5, #1
	mov r8, r3
	negs r5, r5
	movs r7, #0
.L_02009b9a:
	ldr r3, .L_02009c70
	lsls r2, r7, #2
	ldr r0, [r3, r2]
	bl Owner_GetState
	mov r2, r8
	movs r1, #0
	add r2, r10
	mov r11, r0
	mov r9, r1
	mov r12, r2
.L_02009bb0:
	movs r1, #0
	cmp r5, #0
	bge .L_02009bca
	mov r3, r12
	ldrb r1, [r3]
	negs r3, r5
	lsls r1, r3
	movs r2, #1
	movs r3, #31
	ands r1, r3
	add r12, r2
	add r8, r2
	adds r5, #8
.L_02009bca:
	mov r2, r12
	ldrb r3, [r2]
	movs r2, #31
	asrs r3, r5
	ands r3, r2
	adds r1, r1, r3
	movs r3, #5
	subs r5, #5
	negs r3, r3
	cmp r5, r3
	bne .L_02009be8
	movs r3, #1
	add r12, r3
	add r8, r3
	movs r5, #3
.L_02009be8:
	ands r1, r2
	ldr r2, .L_02009c74
	movs r6, #0
	mov lr, r2
	movs r0, #216
.L_02009bf2:
	mov r3, r11
	ldrh r4, [r0, r3]
	mov r3, r9
	lsls r2, r3, #1
	movs r3, #128
	lsls r3, r3, #1
	adds r3, #255
	ands r3, r4
	str r3, [sp, #0]
	mov r3, lr
	ldrh r2, [r3, r2]
	ldr r3, [sp, #0]
	cmp r3, r2
	bne .L_02009c16
	lsls r3, r1, #11
	orrs r3, r4
	mov r2, r11
	strh r3, [r0, r2]
.L_02009c16:
	adds r6, #1
	adds r0, #2
	cmp r6, #15
	bne .L_02009bf2
	movs r3, #1
	add r9, r3
	mov r1, r9
	cmp r1, #23
	bne .L_02009bb0
	adds r7, #1
	cmp r7, #4
	bne .L_02009b9a
	mov r3, r10
	adds r3, #165
	ldrb r2, [r3]
	adds r3, #1
	ldrb r3, [r3]
	lsls r2, r2, #16
	lsls r3, r3, #8
	orrs r2, r3
	mov r3, r10
	adds r3, #167
	ldrb r3, [r3]
	ldr r1, .L_02009c78
	orrs r2, r3
	str r2, [r1, #16]
.L_02009c4a:
	movs r7, #0
.L_02009c4c:
	ldr r6, .L_02009c70
	lsls r5, r7, #2
	ldr r0, [r6, r5]
	bl Owner_RefreshDerivedData
	adds r7, #1
	ldr r0, [r6, r5]
	bl Owner_RecalculateStats
	cmp r7, #4
	bne .L_02009c4c
	add sp, #40
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_02009c70:
	.4byte Data_020026a0
.L_02009c74:
	.4byte Data_020026c0
.L_02009c78:
	.4byte gPartyState
	.section .text.x02009c7c,"ax",%progbits
	.global Func_02001c7c
	.thumb_func
Func_02001c7c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	mov r11, r1
	sub sp, #60
	mov r8, r2
	movs r1, #11
	mov r2, r11
	str r1, [sp, #24]
	cmp r2, #1
	beq .L_02009cb2
	cmp r2, #1
	bgt .L_02009ca4
	cmp r2, #0
	beq .L_02009cac
	b .L_02009cbc
.L_02009ca4:
	mov r3, r11
	cmp r3, #2
	beq .L_02009cb8
	b .L_02009cbc
.L_02009cac:
	movs r4, #173
	str r4, [sp, #24]
	b .L_02009cbc
.L_02009cb2:
	movs r5, #39
	str r5, [sp, #24]
	b .L_02009cbc
.L_02009cb8:
	movs r1, #9
	str r1, [sp, #24]
.L_02009cbc:
	ldr r2, [sp, #24]
	movs r6, #0
	cmp r2, #0
	beq .L_02009cd4
	mov r2, r8
.L_02009cc6:
	movs r3, #0
	strb r3, [r2]
	ldr r3, [sp, #24]
	adds r6, #1
	adds r2, #1
	cmp r6, r3
	bne .L_02009cc6
.L_02009cd4:
	movs r4, #0
	str r4, [sp, #20]
	str r4, [sp, #16]
	str r4, [sp, #12]
	str r4, [sp, #8]
	movs r6, #0
.L_02009ce0:
	mov r5, sp
	adds r5, #28
	lsls r2, r6, #2
	movs r3, #0
	adds r6, #1
	str r5, [sp, #4]
	str r3, [r5, r2]
	cmp r6, #8
	bne .L_02009ce0
	movs r6, #0
.L_02009cf4:
	ldr r2, .L_02009e44
	lsls r3, r6, #1
	ldrh r0, [r2, r3]
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009d10
	ldr r1, [sp, #8]
	movs r3, #1
	lsls r3, r6
	orrs r1, r3
	lsls r3, r1, #24
	lsrs r3, r3, #24
	str r3, [sp, #8]
.L_02009d10:
	adds r6, #1
	cmp r6, #6
	bne .L_02009cf4
	movs r2, #4
	movs r6, #0
	mov r10, r2
.L_02009d1c:
	ldr r2, .L_02009e48
	lsls r3, r6, #2
	ldr r0, [r2, r3]
	bl Owner_GetState
	adds r1, r0, #0
	adds r1, #16
	movs r2, #218
	movs r4, #0
	ldrsh r3, [r1, r4]
	lsls r2, r2, #3
	adds r2, #255
	cmp r3, r2
	ble .L_02009d3a
	strh r2, [r1]
.L_02009d3a:
	movs r5, #0
	ldrsh r3, [r1, r5]
	cmp r3, #0
	bge .L_02009d46
	movs r3, #0
	strh r3, [r1]
.L_02009d46:
	movs r4, #2
	ldrsh r3, [r1, r4]
	cmp r3, r2
	ble .L_02009d50
	strh r2, [r1, #2]
.L_02009d50:
	movs r5, #2
	ldrsh r3, [r1, r5]
	cmp r3, #0
	bge .L_02009d5c
	movs r3, #0
	strh r3, [r1, #2]
.L_02009d5c:
	movs r2, #186
	ldrh r3, [r1, #8]
	lsls r2, r2, #2
	adds r2, #255
	cmp r3, r2
	bls .L_02009d6a
	strh r2, [r1, #8]
.L_02009d6a:
	ldrh r3, [r1, #8]
	cmp r3, #0
	bge .L_02009d72
	movs r3, #0
.L_02009d72:
	strh r3, [r1, #8]
	ldrh r3, [r1, #10]
	cmp r3, r2
	bls .L_02009d7c
	strh r2, [r1, #10]
.L_02009d7c:
	ldrh r3, [r1, #10]
	cmp r3, #0
	bge .L_02009d84
	movs r3, #0
.L_02009d84:
	strh r3, [r1, #10]
	ldrh r3, [r1, #12]
	cmp r3, r2
	bls .L_02009d8e
	strh r2, [r1, #12]
.L_02009d8e:
	ldrh r3, [r1, #12]
	cmp r3, #0
	bge .L_02009d96
	movs r3, #0
.L_02009d96:
	strh r3, [r1, #12]
	ldrb r3, [r1, #14]
	cmp r3, #99
	bls .L_02009da2
	movs r3, #99
	strb r3, [r1, #14]
.L_02009da2:
	ldrb r3, [r1, #14]
	cmp r3, #0
	bge .L_02009daa
	movs r3, #0
.L_02009daa:
	strb r3, [r1, #14]
	movs r3, #0
	ldrsh r2, [r1, r3]
	movs r5, #2
	ldrsh r3, [r1, r5]
	lsls r2, r2, #21
	lsls r3, r3, #10
	orrs r2, r3
	ldrh r3, [r1, #8]
	lsls r4, r6, #3
	orrs r2, r3
	ldr r3, [sp, #4]
	str r2, [r3, r4]
	ldrh r2, [r1, #10]
	ldrh r3, [r1, #12]
	lsls r2, r2, #22
	lsls r3, r3, #12
	orrs r2, r3
	ldrb r3, [r1, #14]
	ldr r5, [sp, #4]
	lsls r3, r3, #4
	orrs r2, r3
	mov r1, r10
	str r2, [r5, r1]
	ldrb r3, [r0, #15]
	cmp r3, #99
	bls .L_02009de4
	movs r3, #99
	strb r3, [r0, #15]
.L_02009de4:
	ldrb r3, [r0, #15]
	cmp r3, #0
	bne .L_02009dee
	movs r3, #1
	strb r3, [r0, #15]
.L_02009dee:
	ldrb r3, [r0, #15]
	subs r2, r4, r6
	lsls r3, r2
	ldr r2, [sp, #20]
	adds r1, r0, #0
	orrs r2, r3
	str r2, [sp, #20]
	movs r4, #0
	adds r1, #248
	movs r2, #0
.L_02009e02:
	ldmia r1!, {r3}
	ldr r5, [sp, #16]
	lsls r3, r2
	adds r5, r5, r3
	adds r4, #1
	str r5, [sp, #16]
	adds r2, #7
	cmp r4, #4
	bne .L_02009e02
	ldr r7, .L_02009e4c
	movs r4, #0
	adds r0, #216
.L_02009e1a:
	ldr r3, .L_02009e40
	ldrh r2, [r0]
	adds r1, r3, #0
	ands r1, r2
	mov r12, r1
	adds r2, r7, #0
	movs r1, #0
.L_02009e28:
	ldrh r3, [r2]
	adds r2, #2
	cmp r12, r3
	bne .L_02009e50
	ldr r5, [sp, #12]
	movs r3, #1
	lsls r3, r1
	orrs r5, r3
	lsls r3, r5, #24
	lsrs r3, r3, #24
	str r3, [sp, #12]
	b .L_02009e50
.L_02009e40:
	.4byte 0x000001ff
.L_02009e44:
	.4byte Data_02002700
.L_02009e48:
	.4byte Data_020026f0
.L_02009e4c:
	.4byte Data_0200270c
.L_02009e50:
	adds r1, #1
	cmp r1, #8
	bne .L_02009e28
	adds r4, #1
	adds r0, #2
	cmp r4, #15
	bne .L_02009e1a
	movs r1, #8
	adds r6, #1
	add r10, r1
	cmp r6, #4
	beq .L_02009e6a
	b .L_02009d1c
.L_02009e6a:
	mov r2, r11
	cmp r2, #0
	beq .L_02009e72
	b .L_02009f8e
.L_02009e72:
	movs r3, #39
	mov r10, r3
	movs r5, #0
	movs r6, #0
.L_02009e7a:
	ldr r2, .L_02009ecc
	lsls r3, r6, #2
	ldr r0, [r2, r3]
	bl Owner_GetState
	movs r4, #0
	mov r12, r4
	adds r7, r0, #0
	mov r4, r10
	adds r7, #216
	add r4, r8
.L_02009e90:
	ldrh r1, [r7]
	ldr r3, .L_02009ec8
	adds r0, r5, #1
	ands r1, r3
	ldrb r3, [r4]
	adds r2, r1, #0
	asrs r2, r0
	adds r3, r3, r2
	strb r3, [r4]
	movs r3, #7
	subs r3, r3, r5
	lsls r1, r3
	ldrb r3, [r4, #1]
	movs r5, #1
	adds r3, r3, r1
	add r10, r5
	adds r5, r0, #0
	strb r3, [r4, #1]
	adds r7, #2
	adds r4, #1
	cmp r5, #7
	bne .L_02009ed0
	movs r1, #1
	movs r5, #0
	adds r4, #1
	add r10, r1
	b .L_02009ed0
	.2byte 0x0000
.L_02009ec8:
	.4byte 0x000001ff
.L_02009ecc:
	.4byte Data_020026f0
.L_02009ed0:
	movs r2, #1
	add r12, r2
	mov r3, r12
	cmp r3, #15
	bne .L_02009e90
	adds r6, #1
	cmp r6, #4
	bne .L_02009e7a
	movs r4, #107
	movs r5, #1
	mov r10, r4
	negs r5, r5
	movs r6, #0
.L_02009eea:
	ldr r3, .L_0200a05c
	lsls r2, r6, #2
	ldr r0, [r3, r2]
	bl Owner_GetState
	movs r1, #0
	str r0, [sp, #0]
	mov lr, r1
	mov r1, r10
	add r1, r8
.L_02009efe:
	ldr r4, [sp, #0]
	ldr r7, .L_0200a060
	movs r2, #0
	mov r9, r2
	mov r12, r2
	adds r4, #216
.L_02009f0a:
	mov r3, lr
	ldrh r0, [r4]
	lsls r2, r3, #1
	movs r3, #128
	lsls r3, r3, #1
	ldrh r2, [r7, r2]
	adds r3, #255
	ands r3, r0
	adds r4, #2
	cmp r3, r2
	bne .L_02009f24
	lsrs r0, r0, #11
	mov r9, r0
.L_02009f24:
	movs r2, #1
	add r12, r2
	mov r3, r12
	cmp r3, #15
	bne .L_02009f0a
	mov r2, r9
	lsls r4, r2, #16
	cmp r5, #0
	bge .L_02009f4a
	negs r2, r5
	lsrs r3, r4, #16
	asrs r3, r2
	ldrb r2, [r1]
	adds r5, #8
	adds r2, r2, r3
	movs r3, #1
	strb r2, [r1]
	add r10, r3
	adds r1, #1
.L_02009f4a:
	ldrb r3, [r1]
	lsrs r2, r4, #16
	lsls r2, r5
	movs r4, #5
	adds r3, r3, r2
	subs r5, #5
	negs r4, r4
	strb r3, [r1]
	cmp r5, r4
	bne .L_02009f66
	movs r5, #1
	add r10, r5
	adds r1, #1
	movs r5, #3
.L_02009f66:
	movs r2, #1
	add lr, r2
	mov r3, lr
	cmp r3, #23
	bne .L_02009efe
	adds r6, #1
	cmp r6, #4
	bne .L_02009eea
	ldr r2, .L_0200a064
	mov r1, r8
	ldrh r3, [r2, #18]
	adds r1, #165
	strb r3, [r1]
	adds r1, #1
	ldr r3, [r2, #16]
	lsrs r3, r3, #8
	strb r3, [r1]
	adds r1, #1
	ldr r3, [r2, #16]
	strb r3, [r1]
.L_02009f8e:
	mov r4, r11
	cmp r4, #2
	beq .L_0200a006
	negs r3, r4
	orrs r3, r4
	lsrs r3, r3, #31
	adds r3, #8
	mov r5, r8
	movs r6, #0
	movs r4, #4
	adds r0, r3, r5
.L_02009fa4:
	ldr r1, [sp, #4]
	lsls r3, r6, #4
	ldr r2, [r1, r3]
	adds r6, #1
	lsrs r3, r2, #24
	strb r3, [r0]
	lsrs r3, r2, #16
	strb r3, [r0, #1]
	lsrs r3, r2, #8
	strb r3, [r0, #2]
	strb r2, [r0, #3]
	ldr r2, [r1, r4]
	lsrs r3, r2, #24
	strb r3, [r0, #4]
	lsrs r3, r2, #16
	strb r3, [r0, #5]
	lsrs r3, r2, #8
	strb r2, [r0, #7]
	strb r3, [r0, #6]
	adds r3, r4, #4
	ldr r3, [r1, r3]
	lsrs r1, r3, #28
	orrs r2, r1
	strb r2, [r0, #7]
	lsrs r2, r3, #20
	strb r2, [r0, #8]
	lsrs r2, r3, #12
	strb r2, [r0, #9]
	lsrs r2, r3, #4
	lsls r3, r3, #4
	strb r3, [r0, #11]
	strb r2, [r0, #10]
	ldr r5, [sp, #4]
	adds r2, r4, #0
	adds r2, #8
	ldr r1, [r5, r2]
	adds r4, #16
	lsrs r2, r1, #28
	orrs r3, r2
	strb r3, [r0, #11]
	lsrs r3, r1, #20
	strb r3, [r0, #12]
	lsrs r3, r1, #12
	lsrs r1, r1, #4
	strb r3, [r0, #13]
	strb r1, [r0, #14]
	adds r0, #15
	cmp r6, #2
	bne .L_02009fa4
.L_0200a006:
	add r1, sp, #20
	ldrb r1, [r1]
	mov r2, r8
	strb r1, [r2]
	ldr r2, [sp, #20]
	mov r4, r8
	lsrs r3, r2, #8
	strb r3, [r4, #1]
	lsrs r3, r2, #16
	strb r3, [r4, #2]
	ldr r5, [sp, #20]
	movs r3, #240
	lsrs r2, r5, #20
	ands r2, r3
	ldr r3, [sp, #16]
	movs r1, #15
	ands r3, r1
	orrs r2, r3
	strb r2, [r4, #3]
	ldr r1, [sp, #16]
	add r2, sp, #8
	lsrs r3, r1, #4
	strb r3, [r4, #4]
	lsrs r3, r1, #12
	strb r3, [r4, #5]
	lsrs r3, r1, #20
	strb r3, [r4, #6]
	mov r3, r11
	ldrb r2, [r2]
	strb r2, [r4, #7]
	cmp r3, #0
	beq .L_0200a04c
	add r5, sp, #12
	ldrb r5, [r5]
	strb r5, [r4, #8]
.L_0200a04c:
	ldr r0, [sp, #24]
	add sp, #60
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0200a05c:
	.4byte Data_020026f0
.L_0200a060:
	.4byte Data_0200271c
.L_0200a064:
	.4byte gPartyState
	.section .text.x0200a068,"ax",%progbits
	.global Func_02002068
	.thumb_func
Func_02002068:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #268
	mov r10, r2
	movs r3, #11
	movs r2, #0
	adds r7, r1, #0
	mov r11, r0
	movs r4, #2
	str r2, [sp, #4]
	mov r8, r2
	mov r9, r3
	cmp r7, #16
	beq .L_0200a09a
	movs r2, #41
	mov r9, r2
	cmp r7, #61
	beq .L_0200a09a
	movs r3, #175
	mov r9, r3
.L_0200a09a:
	mov r2, r9
	movs r6, #0
	cmp r2, #0
	beq .L_0200a0b0
	mov r2, r10
.L_0200a0a4:
	movs r3, #0
	adds r6, #1
	strb r3, [r2]
	adds r2, #1
	cmp r6, r9
	bne .L_0200a0a4
.L_0200a0b0:
	movs r6, #0
	cmp r7, #0
	beq .L_0200a0ce
	mov r1, r11
.L_0200a0b8:
	ldrb r3, [r1]
	add r2, sp, #8
	strb r3, [r2, r6]
	subs r3, r3, r6
	movs r2, #63
	ands r3, r2
	adds r6, #1
	strb r3, [r1]
	adds r1, #1
	cmp r6, r7
	bne .L_0200a0b8
.L_0200a0ce:
	mov r5, r8
	movs r6, #0
	add r5, r10
	b .L_0200a0fe
.L_0200a0d6:
	adds r4, #8
.L_0200a0d8:
	ldrb r3, [r5]
	lsls r1, r4
	adds r3, r3, r1
	strb r3, [r5]
	movs r3, #6
	subs r4, #6
	negs r3, r3
	cmp r4, r3
	bne .L_0200a0f6
	movs r2, #1
	add r8, r2
	movs r4, #2
	adds r5, #1
	cmp r8, r9
	beq .L_0200a132
.L_0200a0f6:
	ldr r3, [sp, #4]
	adds r6, #1
	adds r3, #1
	str r3, [sp, #4]
.L_0200a0fe:
	cmp r6, r7
	beq .L_0200a132
	adds r0, r6, #0
	movs r1, #10
	str r4, [sp, #0]
	bl __modsi3
	ldr r4, [sp, #0]
	cmp r0, #8
	bgt .L_0200a0f6
	ldr r3, [sp, #4]
	mov r2, r11
	ldrb r1, [r2, r3]
	cmp r4, #0
	bge .L_0200a0d8
	negs r3, r4
	adds r2, r1, #0
	asrs r2, r3
	ldrb r3, [r5]
	adds r3, r3, r2
	movs r2, #1
	add r8, r2
	strb r3, [r5]
	adds r5, #1
	cmp r8, r9
	bne .L_0200a0d6
.L_0200a132:
	mov r3, r10
	add r3, r8
	mov r0, r8
	subs r3, #1
	subs r0, #1
	ldrb r4, [r3]
	movs r6, #0
	cmp r0, #0
	beq .L_0200a156
	mov r1, r10
.L_0200a146:
	ldrb r2, [r1]
	adds r3, r4, #0
	eors r3, r2
	adds r6, #1
	strb r3, [r1]
	adds r1, #1
	cmp r6, r0
	bne .L_0200a146
.L_0200a156:
	movs r6, #0
	cmp r7, #0
	beq .L_0200a16c
	mov r0, r11
.L_0200a15e:
	add r3, sp, #8
	ldrb r3, [r3, r6]
	adds r6, #1
	strb r3, [r0]
	adds r0, #1
	cmp r6, r7
	bne .L_0200a15e
.L_0200a16c:
	mov r0, r8
	add sp, #268
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.section .text.x0200a17c,"ax",%progbits
	.global Func_0200217c
	.thumb_func
Func_0200217c:
	push {r5, r6, r7, lr}
	adds r6, r0, #0
	movs r0, #255
	lsls r0, r0, #8
	adds r0, #255
	movs r5, #0
	cmp r6, #0
	beq .L_0200a1b4
.L_0200a18c:
	ldrb r3, [r1]
	movs r4, #0
	lsls r3, r3, #8
	eors r0, r3
.L_0200a194:
	movs r3, #128
	ldr r7, .L_0200a1bc
	lsls r3, r3, #8
	ands r3, r0
	lsls r2, r0, #1
	adds r4, #1
	adds r0, r2, r7
	cmp r3, #0
	bne .L_0200a1a8
	adds r0, r2, #0
.L_0200a1a8:
	cmp r4, #8
	bne .L_0200a194
	adds r5, #1
	adds r1, #1
	cmp r5, r6
	bne .L_0200a18c
.L_0200a1b4:
	mvns r0, r0
	lsls r0, r0, #16
	lsrs r0, r0, #16
	pop {r5, r6, r7, pc}
.L_0200a1bc:
	.4byte 0xffffefdf
	.section .rodata.x0200a410,"a",%progbits
	.global Data_02002410
Data_02002410:
	.4byte 0x00040004
	.4byte 0x00000004
	.global Data_02002418
Data_02002418:
	.4byte 0x00800040
	.4byte 0x00800040
	.4byte 0x00100020
	.4byte 0x00100020
	.4byte 0x00100040
	.4byte 0x00200080
	.4byte 0x00040040
	.4byte 0x00000000
	.global Data_02002438
Data_02002438:
	.4byte 0x03020100
	.4byte 0x00010203
	.global Data_02002440
Data_02002440:
	.4byte 0x44434241
	.4byte 0x47462045
	.4byte 0x204b4a48
	.4byte 0x504e4d4c
	.4byte 0x00522051
	.global Data_02002454
Data_02002454:
	.4byte 0x56555453
	.4byte 0x59582057
	.4byte 0x2033325a
	.4byte 0x37363534
	.4byte 0x00392038
	.global Data_02002468
Data_02002468:
	.4byte 0x64636261
	.4byte 0x67662065
	.4byte 0x206a6968
	.4byte 0x706e6d6b
	.4byte 0x00722071
	.global Data_0200247c
Data_0200247c:
	.4byte 0x76757473
	.4byte 0x79782077
	.4byte 0x203f217a
	.4byte 0x25242623
	.4byte 0x003d202b
	.global Data_02002490
Data_02002490:
	.4byte 0x09510941
	.4byte 0x08d108b3
	.4byte 0x0868081e
	.global gSceneEntrances
gSceneEntrances:
	.4byte 0xffff0000
	.4byte 0x00000000
	.4byte 0x40000000
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gSceneExits
gSceneExits:
	.4byte 0x000001ff
	.global gScenePlacements
gScenePlacements:
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gSceneEvents
gSceneEvents:
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020024f4
Data_020024f4:
	.2byte 0x0000
	.global Data_020024f6
Data_020024f6:
	.2byte 0x0000
	.global Data_020024f8
Data_020024f8:
	.2byte 0x0000
	.global Data_020024fa
Data_020024fa:
	.2byte 0x0000
	.global Data_020024fc
Data_020024fc:
	.2byte 0x0000
	.global Data_020024fe
Data_020024fe:
	.2byte 0x0000
	.global Data_02002500
Data_02002500:
	.4byte 0x00000000
	.4byte 0x8fc22100
	.4byte 0x250d09f0
	.4byte 0x3f08b7c2
	.4byte 0xf0c97c32
	.4byte 0x0ca7c327
	.4byte 0x907c32bf
	.4byte 0x5fc3d5f8
	.4byte 0xfc3d9f0f
	.4byte 0xb9ddf0f6
	.4byte 0x1f0c3fc2
	.4byte 0xf0c4fc31
	.4byte 0xd45fc315
	.4byte 0xfc7c32f5
	.4byte 0x78652df0
	.4byte 0x6d4b4f21
	.4byte 0x0ccfc331
	.4byte 0xf7ef535f
	.4byte 0xc3e304f0
	.4byte 0x5e7f0f97
	.4byte 0x6df51936
	.4byte 0x0751d1d4
	.4byte 0xc79c1b9b
	.4byte 0xc339f0cd
	.4byte 0x33df0cef
	.4byte 0xe9bd4ffc
	.4byte 0xd1d7ac75
	.4byte 0x863e025e
	.4byte 0x7fa5ab0f
	.4byte 0x1a7efff8
	.4byte 0x87821ebc
	.4byte 0x78be1e1f
	.4byte 0x93e1e3f8
	.4byte 0xb97bc1cb
	.4byte 0x7ebe1f9a
	.4byte 0x031efbf8
	.4byte 0x9f1e98b6
	.4byte 0xe1e59f81
	.4byte 0x1e7f879b
	.4byte 0xe9f87a3e
	.4byte 0x68ebf37a
	.4byte 0x0fbf63a8
	.4byte 0x9f9f83c1
	.4byte 0x7fc1e6f8
	.4byte 0xe60cc1f3
	.4byte 0x65f0d8c1
	.4byte 0x9f0d9fc3
	.4byte 0xc95afc36
	.4byte 0xe5fc3950
	.4byte 0x6fc399f0
	.4byte 0x6a59df0e
	.4byte 0x351f0d3c
	.4byte 0x55f0d4fc
	.4byte 0x09f55fc3
	.4byte 0xb41e6208
	.4byte 0x7c36ff0d
	.4byte 0xc373f0dc
	.4byte 0x39fbd5d7
	.4byte 0xa3f0e87c
	.4byte 0x7f0e97c3
	.4byte 0xd564025a
	.4byte 0x574755b7
	.4byte 0x676f41e7
	.4byte 0x37707982
	.4byte 0x7bf0de7c
	.4byte 0xff0df7c3
	.4byte 0x1d6a6f57
	.4byte 0xd6b475ab
	.4byte 0xe027d5fe
	.4byte 0x1da27e1f
	.4byte 0x083c79ce
	.4byte 0xf8707e1c
	.4byte 0x870fe1c2
	.4byte 0x5e072c4f
	.4byte 0x7e1d8f87
	.4byte 0xe1daf876
	.4byte 0xd410fb6f
	.4byte 0xc1a818f2
	.4byte 0xbe1c583c
	.4byte 0xe1c7f871
	.4byte 0xac9f8723
	.4byte 0xa68eb72f
	.4byte 0x14fb763a
	.4byte 0x90ed8394
	.4byte 0x270fc791
	.4byte 0x09f0a7c2
	.4byte 0xfbc22b0d
	.2byte 0x0000
	.global Data_0200265e
Data_0200265e:
	.2byte 0x0100
	.4byte 0x05040302
	.4byte 0x09080706
	.4byte 0x0d0c0b0a
	.4byte 0x11100f0e
	.4byte 0x15141312
	.4byte 0x19181716
	.4byte 0x1d1c1b1a
	.4byte 0x21201f1e
	.4byte 0x25242322
	.4byte 0x29282726
	.4byte 0x2d2c2b2a
	.4byte 0x31302f2e
	.4byte 0x35343332
	.4byte 0x39383736
	.4byte 0x3d3c3b3a
	.4byte 0x00003f3e
	.global Data_020026a0
Data_020026a0:
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000003
	.global Data_020026b0
Data_020026b0:
	.4byte 0x00c900c8
	.4byte 0x00cb00ca
	.4byte 0x00cd00cc
	.4byte 0x00cf00ce
	.global Data_020026c0
Data_020026c0:
	.4byte 0x00b500b4
	.4byte 0x00b700b6
	.4byte 0x00bb00ba
	.4byte 0x00bd00bc
	.4byte 0x00c000bf
	.4byte 0x00c200c1
	.4byte 0x00c400c3
	.4byte 0x00e300e2
	.4byte 0x00e500e4
	.4byte 0x00ee00ec
	.4byte 0x00f000ef
	.4byte 0x000000f1
	.global Data_020026f0
Data_020026f0:
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000003
	.global Data_02002700
Data_02002700:
	.4byte 0x09510941
	.4byte 0x08d108b3
	.4byte 0x0868081e
	.global Data_0200270c
Data_0200270c:
	.4byte 0x00c900c8
	.4byte 0x00cb00ca
	.4byte 0x00cd00cc
	.4byte 0x00cf00ce
	.global Data_0200271c
Data_0200271c:
	.4byte 0x00b500b4
	.4byte 0x00b700b6
	.4byte 0x00bb00ba
	.4byte 0x00bd00bc
	.4byte 0x00c000bf
	.4byte 0x00c200c1
	.4byte 0x00c400c3
	.4byte 0x00e300e2
	.4byte 0x00e500e4
	.4byte 0x00ee00ec
	.4byte 0x00f000ef
	.2byte 0x00f1
	.section .bss,"aw",%nobits
	.global Data_0200274a
Data_0200274a:
	.space 0x00000140
	.global Data_0200288a
Data_0200288a:
