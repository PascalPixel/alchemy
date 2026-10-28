.syntax unified
	.thumb
	.section .text.x02008328,"ax",%progbits
	.ifdef TLA_EDITION_JA
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
	ldr r0, .L_ja_020086a8
	bl Task_Add
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	mov r1, r10
	str r1, [r3]
	bl Event_StartScreenTransition
	bl Event_WaitTransitionFrames
	ldr r3, .L_ja_020086ac
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #2
	bne .L_ja_020083ee
.L_ja_0200836a:
	ldr r5, .L_ja_020086b0
	movs r1, #5
	adds r0, r5, #0
	bl UiText_ShowPositionedMessageAndWait
	movs r0, #1
	bl Func_020023d8
	adds r6, r0, #0
	bl UiWork_FinalizePendingCore
	cmp r6, #0
	bne .L_ja_020083cc
	movs r1, #1
	adds r0, r5, #1
	bl UiText_ShowPositionedMessageAndWait
	ldr r3, .L_ja_020086ac
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
	beq .L_ja_0200836a
.L_ja_020083cc:
	movs r0, #60
	bl FieldEvent_WaitFrames
	bl Event_FinishScreenTransition
	movs r0, #78
	bl Func_02002408
	movs r0, #150
	lsls r0, r0, #1
	bl FieldEvent_WaitFrames
	ldr r0, .L_ja_020086b4
	movs r1, #1
	bl Func_020023b8
	b .L_ja_02008a24
.L_ja_020083ee:
	ldr r3, .L_ja_020086b8
	mov r1, r10
	strb r1, [r3]
.L_ja_020083f4:
	bl Func_020022f0
	adds r5, r0, #0
	cmp r5, #0
	bge .L_ja_02008422
	ldr r3, .L_ja_020086bc
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_ja_02008422
	ldr r3, .L_ja_020086ac
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #74
	adds r3, r3, r2
	movs r2, #1
	strb r2, [r3]
	ldr r3, .L_ja_020086c0
	ldr r0, .L_ja_020086c4
	strb r2, [r3]
	movs r1, #1
	movs r2, #8
	bl Func_020022b8
.L_ja_02008422:
	cmp r5, #0
	bne .L_ja_02008446
	mov r3, r10
	cmp r3, #0
	bne .L_ja_02008446
	movs r0, #30
	bl WaitFrames
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_ja_020086c8
	bl Task_Add
	movs r0, #1
	bl WaitFrames
	movs r1, #1
	mov r10, r1
.L_ja_02008446:
	cmp r5, #0
	ble .L_ja_02008452
	bl Menu_SelectSaveSlotAction
	adds r5, r0, #0
	b .L_ja_02008454
.L_ja_02008452:
	movs r5, #0
.L_ja_02008454:
	cmp r5, #0
	beq .L_ja_0200845a
	b .L_ja_020086fc
.L_ja_0200845a:
	bl Func_02002350
	ldr r3, .L_ja_020086ac
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
.L_ja_0200847c:
	movs r0, #6
	bl WaitFrames
	adds r0, r6, #0
	bl Func_020022f8
	movs r3, #1
	adds r5, r0, #0
	negs r3, r3
	cmp r5, r3
	bne .L_ja_0200849a
	cmp r6, #4
	beq .L_ja_020083f4
	subs r6, #1
	b .L_ja_0200847c
.L_ja_0200849a:
	ldr r3, .L_ja_020086cc
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	bne .L_ja_020084ae
	ldr r3, .L_ja_020086d0
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	beq .L_ja_020084b0
.L_ja_020084ae:
	movs r7, #8
.L_ja_020084b0:
	adds r6, #1
	cmp r6, r7
	blt .L_ja_0200847c
.L_ja_020084b6:
	ldr r0, .L_ja_020086d4
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
	bne .L_ja_02008570
.L_ja_020084d4:
	movs r0, #0
	bl Func_020022b0
	movs r3, #1
	negs r3, r3
	adds r5, r0, #0
	mov r8, r3
	cmp r5, r8
	beq .L_ja_020083f4
	cmp r5, #1
	bne .L_ja_0200852a
	ldr r5, .L_ja_020086d8
	movs r2, #192
	adds r1, r5, #0
	lsls r2, r2, #6
	ldr r6, .L_ja_020086dc
	ldr r0, .L_ja_020086e0
	mov lr, r6
	.2byte 0xf800
	bl Func_02000b24
	adds r7, r0, #0
	cmp r7, r8
	beq .L_ja_02008508
	bl Func_020023e8
.L_ja_02008508:
	movs r2, #192
	adds r1, r5, #0
	lsls r2, r2, #6
	ldr r0, .L_ja_020086e4
	mov lr, r6
	.2byte 0xf800
	movs r2, #192
	adds r0, r5, #0
	ldr r1, .L_ja_020086e0
	lsls r2, r2, #6
	mov lr, r6
	.2byte 0xf800
	cmp r7, r8
	beq .L_ja_020084d4
	ldr r0, .L_ja_020086e4
	movs r1, #0
	b .L_ja_02008568
.L_ja_0200852a:
	cmp r5, #0
	bne .L_ja_02008574
	ldr r5, .L_ja_020086d8
	movs r2, #192
	adds r1, r5, #0
	lsls r2, r2, #6
	ldr r6, .L_ja_020086dc
	ldr r0, .L_ja_020086e0
	mov lr, r6
	.2byte 0xf800
	bl Func_02002350
	bl Func_02000e44
	movs r2, #192
	adds r7, r0, #0
	adds r1, r5, #0
	lsls r2, r2, #6
	ldr r0, .L_ja_020086e4
	mov lr, r6
	.2byte 0xf800
	movs r2, #192
	adds r0, r5, #0
	ldr r1, .L_ja_020086e0
	lsls r2, r2, #6
	mov lr, r6
	.2byte 0xf800
	cmp r7, r8
	beq .L_ja_020084d4
	ldr r0, .L_ja_020086e4
	movs r1, #1
.L_ja_02008568:
	movs r2, #0
	bl Func_02002370
	b .L_ja_02008574
.L_ja_02008570:
	bl Func_02002398
.L_ja_02008574:
	movs r6, #0
	movs r7, #1
.L_ja_02008578:
	movs r0, #6
	bl WaitFrames
	adds r0, r6, #0
	bl Func_020022f8
	movs r1, #1
	adds r5, r0, #0
	negs r1, r1
	cmp r5, r1
	bne .L_ja_02008596
	cmp r6, #0
	beq .L_ja_020084b6
	subs r6, #1
	b .L_ja_02008578
.L_ja_02008596:
	ldr r3, .L_ja_020086d0
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	beq .L_ja_020085a2
	movs r7, #4
.L_ja_020085a2:
	adds r6, #1
	cmp r6, r7
	blt .L_ja_02008578
	ldr r3, .L_ja_020086e8
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	beq .L_ja_0200869c
.L_ja_020085b2:
	ldr r5, .L_ja_020086ec
	movs r1, #13
	adds r0, r5, #0
	bl UiText_ShowPositionedMessageAndWait
	ldr r1, .L_ja_020086f0
	movs r0, #1
	movs r2, #6
	movs r3, #0
	bl Menu_RunConfirmSelection
	adds r6, r0, #0
	bl UiWork_FinalizePendingCore
	cmp r6, #0
	bne .L_ja_02008648
	ldr r3, .L_ja_020086e8
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	bne .L_ja_020085e6
	adds r0, r5, #1
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
	b .L_ja_02008648
.L_ja_020085e6:
	ldr r6, .L_ja_020086d8
	movs r2, #192
	adds r1, r6, #0
	lsls r2, r2, #6
	ldr r7, .L_ja_020086dc
	ldr r0, .L_ja_020086e0
	mov lr, r7
	.2byte 0xf800
	movs r0, #5
	bl Func_020022d8
	adds r5, r0, #0
	cmp r5, #0
	bne .L_ja_020085b2
	ldr r5, .L_ja_020086ac
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
	ldr r0, .L_ja_020086e4
	mov lr, r7
	.2byte 0xf800
	movs r2, #192
	ldr r1, .L_ja_020086e0
	lsls r2, r2, #6
	adds r0, r6, #0
	mov lr, r7
	.2byte 0xf800
	ldr r0, .L_ja_020086e4
	bl Func_020023a0
	movs r3, #196
	lsls r3, r3, #1
	adds r3, #255
	adds r5, r5, r3
	movs r3, #1
	strb r3, [r5]
	b .L_ja_0200869c
.L_ja_02008648:
	ldr r0, .L_ja_020086f4
	movs r1, #13
	bl UiText_ShowPositionedMessageAndWait
	ldr r5, .L_ja_020086f0
	movs r0, #1
	adds r1, r5, #0
	movs r2, #6
	movs r3, #0
	bl Menu_RunConfirmSelection
	adds r6, r0, #0
	bl UiWork_FinalizePendingCore
	cmp r6, #0
	bne .L_ja_0200867e
	movs r0, #46
	bl GameFlag_SetBit
	ldr r3, .L_ja_020086ac
	movs r1, #196
	lsls r1, r1, #1
	adds r1, #255
	adds r3, r3, r1
	movs r2, #2
	strb r2, [r3]
	b .L_ja_0200869c
.L_ja_0200867e:
	ldr r0, .L_ja_020086f8
	movs r1, #13
	bl UiText_ShowPositionedMessageAndWait
	movs r0, #1
	adds r1, r5, #0
	movs r2, #6
	movs r3, #0
	bl Menu_RunConfirmSelection
	adds r6, r0, #0
	bl UiWork_FinalizePendingCore
	cmp r6, #0
	bne .L_ja_020085b2
.L_ja_0200869c:
	ldr r0, .L_ja_020086b4
	movs r1, #8
	bl Func_020023b8
	b .L_ja_020089f6
	.2byte 0x0000
.L_ja_020086a8:
	.4byte Func_020001f4
.L_ja_020086ac:
	.4byte gAdventure
.L_ja_020086b0:
	.4byte 0x00000008
.L_ja_020086b4:
	.4byte 0x00000000
.L_ja_020086b8:
	.4byte gSleepDisabled
.L_ja_020086bc:
	.4byte gDebugMode
.L_ja_020086c0:
	.4byte gAutoSleepEnabled
.L_ja_020086c4:
	.4byte 0x0000000b
.L_ja_020086c8:
	.4byte Func_0200015c
.L_ja_020086cc:
	.4byte gData_020024f6
.L_ja_020086d0:
	.4byte gData_020024f8
.L_ja_020086d4:
	.4byte 0x00000024
.L_ja_020086d8:
	.4byte gData_02000000
.L_ja_020086dc:
	.4byte IwramCopyWords
.L_ja_020086e0:
	.4byte gMapCellBuffer
.L_ja_020086e4:
	.4byte Data_02012f94 + 0x6c
.L_ja_020086e8:
	.4byte gData_02003860
.L_ja_020086ec:
	.4byte 0x00000025
.L_ja_020086f0:
	.4byte 0x00000029
.L_ja_020086f4:
	.4byte 0x00000027
.L_ja_020086f8:
	.4byte 0x00000028
.L_ja_020086fc:
	cmp r5, #1
	beq .L_ja_02008702
	b .L_ja_02008850
.L_ja_02008702:
	movs r0, #1
	bl Func_020022d8
	movs r2, #1
	adds r5, r0, #0
	negs r2, r2
	cmp r5, r2
	bne .L_ja_02008714
	b .L_ja_020083f4
.L_ja_02008714:
	movs r0, #10
	adds r0, #255
	bl GameFlag_SetBit
	ldr r5, .L_ja_02008a30
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
	beq .L_ja_02008772
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
	b .L_ja_02008846
.L_ja_02008772:
	ldr r3, .L_ja_02008a34
	movs r2, #130
	ldr r3, [r3]
	lsls r2, r2, #2
	ands r3, r2
	cmp r3, r2
	bne .L_ja_020087c6
	bl Func_020002e0
	cmp r0, #0
	beq .L_ja_0200878c
	ldr r0, .L_ja_02008a38
	b .L_ja_02008806
.L_ja_0200878c:
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
	b .L_ja_02008846
.L_ja_020087c6:
	ldr r3, .L_ja_02008a3c
	ldr r2, [r5, #4]
	ldr r3, [r3]
	cmp r2, r3
	beq .L_ja_02008846
	ldr r6, .L_ja_02008a40
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
	beq .L_ja_020087f8
	bl UiWork_FinalizePendingCore
	b .L_ja_020083f4
.L_ja_020087f8:
	bl UiWork_FinalizePendingCore
	bl Func_020002e0
	cmp r0, #0
	beq .L_ja_0200880e
	adds r0, r6, #2
.L_ja_02008806:
	movs r1, #9
	bl UiText_ShowPositionedMessageAndWait
	b .L_ja_020083f4
.L_ja_0200880e:
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
.L_ja_02008846:
	movs r0, #131
	lsls r0, r0, #1
	bl GameFlag_ClearBit
	b .L_ja_020089f6
.L_ja_02008850:
	cmp r5, #2
	bne .L_ja_0200885a
	bl Func_020022e0
	b .L_ja_020083f4
.L_ja_0200885a:
	cmp r5, #3
	bne .L_ja_02008864
	bl Func_020022e8
	b .L_ja_020083f4
.L_ja_02008864:
	cmp r5, #4
	bne .L_ja_020088dc
	movs r0, #4
	bl Func_020022d8
	movs r3, #1
	adds r5, r0, #0
	negs r3, r3
	cmp r5, r3
	bne .L_ja_0200887a
	b .L_ja_020083f4
.L_ja_0200887a:
	ldr r5, .L_ja_02008a30
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
	beq .L_ja_02008898
	bl Func_02002390
.L_ja_02008898:
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
	ldr r2, .L_ja_02008a44
	movs r3, #1
	strb r3, [r2]
	ldr r0, .L_ja_02008a48
	movs r1, #1
	bl Func_020023b8
	b .L_ja_020089f6
.L_ja_020088dc:
	cmp r5, #5
	beq .L_ja_020088e2
	b .L_ja_020083f4
.L_ja_020088e2:
	movs r0, #6
	bl Func_020022d8
	movs r1, #1
	adds r5, r0, #0
	negs r1, r1
	cmp r5, r1
	bne .L_ja_020088f4
	b .L_ja_020083f4
.L_ja_020088f4:
	ldr r3, .L_ja_02008a30
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
.L_ja_0200890e:
	movs r0, #2
	bl Func_020022b0
	movs r3, #1
	negs r3, r3
	adds r5, r0, #0
	mov r8, r3
	cmp r5, r8
	beq .L_ja_020088e2
	cmp r5, #1
	bne .L_ja_0200896a
	ldr r5, .L_ja_02008a4c
	movs r2, #192
	adds r1, r5, #0
	lsls r2, r2, #6
	ldr r6, .L_ja_02008a50
	ldr r0, .L_ja_02008a54
	mov lr, r6
	.2byte 0xf800
	bl Func_02000b24
	adds r7, r0, #0
	cmp r7, r8
	beq .L_ja_02008942
	bl Func_020023e8
.L_ja_02008942:
	movs r2, #192
	adds r1, r5, #0
	lsls r2, r2, #6
	ldr r0, .L_ja_02008a58
	mov lr, r6
	.2byte 0xf800
	movs r2, #192
	adds r0, r5, #0
	ldr r1, .L_ja_02008a54
	lsls r2, r2, #6
	mov lr, r6
	.2byte 0xf800
	cmp r7, r8
	beq .L_ja_0200890e
	ldr r0, .L_ja_02008a58
	movs r1, #0
	movs r2, #1
	bl Func_02002370
	b .L_ja_020089ae
.L_ja_0200896a:
	cmp r5, #0
	bne .L_ja_020089ae
	ldr r5, .L_ja_02008a4c
	movs r2, #192
	adds r1, r5, #0
	lsls r2, r2, #6
	ldr r6, .L_ja_02008a50
	ldr r0, .L_ja_02008a54
	mov lr, r6
	.2byte 0xf800
	bl Func_02002350
	bl Func_02000e44
	movs r2, #192
	adds r7, r0, #0
	adds r1, r5, #0
	lsls r2, r2, #6
	ldr r0, .L_ja_02008a58
	mov lr, r6
	.2byte 0xf800
	movs r2, #192
	adds r0, r5, #0
	ldr r1, .L_ja_02008a54
	lsls r2, r2, #6
	mov lr, r6
	.2byte 0xf800
	cmp r7, r8
	beq .L_ja_0200890e
	ldr r0, .L_ja_02008a58
	movs r1, #1
	movs r2, #1
	bl Func_02002370
.L_ja_020089ae:
	ldr r3, .L_ja_02008a5c
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	bne .L_ja_020089c2
	ldr r3, .L_ja_02008a60
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	beq .L_ja_020089f6
.L_ja_020089c2:
	movs r6, #0
	movs r7, #1
.L_ja_020089c6:
	movs r0, #6
	bl WaitFrames
	adds r0, r6, #0
	bl Func_020022f8
	movs r3, #1
	adds r5, r0, #0
	negs r3, r3
	cmp r5, r3
	bne .L_ja_020089e4
	cmp r6, #0
	beq .L_ja_020088e2
	subs r6, #1
	b .L_ja_020089c6
.L_ja_020089e4:
	ldr r3, .L_ja_02008a60
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	beq .L_ja_020089f0
	movs r7, #4
.L_ja_020089f0:
	adds r6, #1
	cmp r6, r7
	blt .L_ja_020089c6
.L_ja_020089f6:
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
	bl FieldEvent_WaitFrames
	movs r0, #78
	bl Func_02002408
	bl Event_FinishScreenTransition
	bl Event_WaitTransitionFrames
	movs r0, #60
	bl FieldEvent_WaitFrames
.L_ja_02008a24:
	movs r0, #0
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_ja_02008a30:
	.4byte gAdventure
.L_ja_02008a34:
	.4byte gInput
.L_ja_02008a38:
	.4byte 0x00000007
.L_ja_02008a3c:
	.4byte gData_02001000
.L_ja_02008a40:
	.4byte 0x00000005
.L_ja_02008a44:
	.4byte gSleepDisabled
.L_ja_02008a48:
	.4byte 0x00000138
.L_ja_02008a4c:
	.4byte gData_02000000
.L_ja_02008a50:
	.4byte IwramCopyWords
.L_ja_02008a54:
	.4byte gMapCellBuffer
.L_ja_02008a58:
	.4byte Data_02012f94 + 0x6c
.L_ja_02008a5c:
	.4byte gData_020024f6
.L_ja_02008a60:
	.4byte gData_020024f8
	.else
	.ifdef TLA_EDITION_FR
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
	ldr r0, .L_fr_020086a0
	bl Task_Add
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	mov r1, r10
	str r1, [r3]
	bl Event_StartScreenTransition
	bl Event_WaitTransitionFrames
	ldr r3, .L_fr_020086a4
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #2
	bne .L_fr_020083ee
.L_fr_0200836a:
	ldr r5, .L_fr_020086a8
	movs r1, #5
	adds r0, r5, #0
	bl UiText_ShowPositionedMessageAndWait
	movs r0, #1
	bl Func_020023d8
	adds r6, r0, #0
	bl UiWork_FinalizePendingCore
	cmp r6, #0
	bne .L_fr_020083cc
	movs r1, #1
	adds r0, r5, #1
	bl UiText_ShowPositionedMessageAndWait
	ldr r3, .L_fr_020086a4
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
	beq .L_fr_0200836a
.L_fr_020083cc:
	movs r0, #60
	bl FieldEvent_WaitFrames
	bl Event_FinishScreenTransition
	movs r0, #78
	bl Func_02002408
	movs r0, #150
	lsls r0, r0, #1
	bl FieldEvent_WaitFrames
	ldr r0, .L_fr_020086ac
	movs r1, #1
	bl Func_020023b8
	b .L_fr_02008a24
.L_fr_020083ee:
	ldr r3, .L_fr_020086b0
	mov r1, r10
	strb r1, [r3]
.L_fr_020083f4:
	bl Func_020022f0
	adds r5, r0, #0
	cmp r5, #0
	bge .L_fr_02008422
	ldr r3, .L_fr_020086b4
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_fr_02008422
	ldr r3, .L_fr_020086a4
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #74
	adds r3, r3, r2
	movs r2, #1
	strb r2, [r3]
	ldr r3, .L_fr_020086b8
	ldr r0, .L_fr_020086bc
	strb r2, [r3]
	movs r1, #1
	movs r2, #8
	bl Func_020022b8
.L_fr_02008422:
	cmp r5, #0
	bne .L_fr_02008446
	mov r3, r10
	cmp r3, #0
	bne .L_fr_02008446
	movs r0, #30
	bl WaitFrames
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_fr_020086c0
	bl Task_Add
	movs r0, #1
	bl WaitFrames
	movs r1, #1
	mov r10, r1
.L_fr_02008446:
	cmp r5, #0
	ble .L_fr_02008452
	bl Menu_SelectSaveSlotAction
	adds r5, r0, #0
	b .L_fr_02008454
.L_fr_02008452:
	movs r5, #0
.L_fr_02008454:
	cmp r5, #0
	beq .L_fr_0200845a
	b .L_fr_020086f4
.L_fr_0200845a:
	bl Func_02002350
	ldr r3, .L_fr_020086a4
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
.L_fr_0200847c:
	movs r0, #6
	bl WaitFrames
	adds r0, r6, #0
	bl Func_020022f8
	movs r3, #1
	adds r5, r0, #0
	negs r3, r3
	cmp r5, r3
	bne .L_fr_0200849a
	cmp r6, #4
	beq .L_fr_020083f4
	subs r6, #1
	b .L_fr_0200847c
.L_fr_0200849a:
	ldr r3, .L_fr_020086c4
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	bne .L_fr_020084ae
	ldr r3, .L_fr_020086c8
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	beq .L_fr_020084b0
.L_fr_020084ae:
	movs r7, #8
.L_fr_020084b0:
	adds r6, #1
	cmp r6, r7
	blt .L_fr_0200847c
.L_fr_020084b6:
	ldr r0, .L_fr_020086cc
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
	bne .L_fr_0200856a
.L_fr_020084d4:
	movs r0, #0
	bl Func_020022b0
	movs r3, #1
	negs r3, r3
	adds r5, r0, #0
	mov r8, r3
	cmp r5, r8
	beq .L_fr_020083f4
	cmp r5, #1
	bne .L_fr_0200852a
	ldr r5, .L_fr_020086d0
	movs r2, #192
	adds r1, r5, #0
	lsls r2, r2, #6
	ldr r6, .L_fr_020086d4
	ldr r0, .L_fr_020086d8
	mov lr, r6
	.2byte 0xf800
	bl Func_02000b24
	adds r7, r0, #0
	cmp r7, r8
	beq .L_fr_02008508
	bl Func_020023e8
.L_fr_02008508:
	movs r2, #192
	adds r1, r5, #0
	lsls r2, r2, #6
	ldr r0, .L_fr_020086dc
	mov lr, r6
	.2byte 0xf800
	movs r2, #192
	adds r0, r5, #0
	ldr r1, .L_fr_020086d8
	lsls r2, r2, #6
	mov lr, r6
	.2byte 0xf800
	cmp r7, r8
	beq .L_fr_020084d4
	ldr r0, .L_fr_020086dc
	movs r1, #0
	b .L_fr_02008562
.L_fr_0200852a:
	cmp r5, #0
	bne .L_fr_0200856e
	ldr r5, .L_fr_020086d0
	movs r2, #192
	adds r1, r5, #0
	lsls r2, r2, #6
	ldr r6, .L_fr_020086d4
	ldr r0, .L_fr_020086d8
	mov lr, r6
	.2byte 0xf800
	bl Func_02002350
	bl Func_02000e44
	movs r2, #192
	adds r1, r5, #0
	lsls r2, r2, #6
	ldr r0, .L_fr_020086dc
	mov lr, r6
	.2byte 0xf800
	movs r2, #192
	ldr r1, .L_fr_020086d8
	lsls r2, r2, #6
	adds r0, r5, #0
	mov lr, r6
	.2byte 0xf800
	ldr r0, .L_fr_020086dc
	movs r1, #1
.L_fr_02008562:
	movs r2, #0
	bl Func_02002370
	b .L_fr_0200856e
.L_fr_0200856a:
	bl Func_02002398
.L_fr_0200856e:
	movs r6, #0
	movs r7, #1
.L_fr_02008572:
	movs r0, #6
	bl WaitFrames
	adds r0, r6, #0
	bl Func_020022f8
	movs r1, #1
	adds r5, r0, #0
	negs r1, r1
	cmp r5, r1
	bne .L_fr_02008590
	cmp r6, #0
	beq .L_fr_020084b6
	subs r6, #1
	b .L_fr_02008572
.L_fr_02008590:
	ldr r3, .L_fr_020086c8
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	beq .L_fr_0200859c
	movs r7, #4
.L_fr_0200859c:
	adds r6, #1
	cmp r6, r7
	blt .L_fr_02008572
	ldr r3, .L_fr_020086e0
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	beq .L_fr_02008696
.L_fr_020085ac:
	ldr r5, .L_fr_020086e4
	movs r1, #13
	adds r0, r5, #0
	bl UiText_ShowPositionedMessageAndWait
	ldr r1, .L_fr_020086e8
	movs r0, #1
	movs r2, #8
	movs r3, #0
	bl Menu_RunConfirmSelection
	adds r6, r0, #0
	bl UiWork_FinalizePendingCore
	cmp r6, #0
	bne .L_fr_02008642
	ldr r3, .L_fr_020086e0
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	bne .L_fr_020085e0
	adds r0, r5, #1
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
	b .L_fr_02008642
.L_fr_020085e0:
	ldr r6, .L_fr_020086d0
	movs r2, #192
	adds r1, r6, #0
	lsls r2, r2, #6
	ldr r7, .L_fr_020086d4
	ldr r0, .L_fr_020086d8
	mov lr, r7
	.2byte 0xf800
	movs r0, #5
	bl Func_020022d8
	adds r5, r0, #0
	cmp r5, #0
	bne .L_fr_020085ac
	ldr r5, .L_fr_020086a4
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
	ldr r0, .L_fr_020086dc
	mov lr, r7
	.2byte 0xf800
	movs r2, #192
	ldr r1, .L_fr_020086d8
	lsls r2, r2, #6
	adds r0, r6, #0
	mov lr, r7
	.2byte 0xf800
	ldr r0, .L_fr_020086dc
	bl Func_020023a0
	movs r3, #196
	lsls r3, r3, #1
	adds r3, #255
	adds r5, r5, r3
	movs r3, #1
	strb r3, [r5]
	b .L_fr_02008696
.L_fr_02008642:
	ldr r0, .L_fr_020086ec
	movs r1, #13
	bl UiText_ShowPositionedMessageAndWait
	ldr r5, .L_fr_020086e8
	movs r0, #1
	adds r1, r5, #0
	movs r2, #8
	movs r3, #0
	bl Menu_RunConfirmSelection
	adds r6, r0, #0
	bl UiWork_FinalizePendingCore
	cmp r6, #0
	bne .L_fr_02008678
	movs r0, #46
	bl GameFlag_SetBit
	ldr r3, .L_fr_020086a4
	movs r1, #196
	lsls r1, r1, #1
	adds r1, #255
	adds r3, r3, r1
	movs r2, #2
	strb r2, [r3]
	b .L_fr_02008696
.L_fr_02008678:
	ldr r0, .L_fr_020086f0
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
	bne .L_fr_020085ac
.L_fr_02008696:
	ldr r0, .L_fr_020086ac
	movs r1, #8
	bl Func_020023b8
	b .L_fr_020089f6
.L_fr_020086a0:
	.4byte Func_020001f4
.L_fr_020086a4:
	.4byte gAdventure
.L_fr_020086a8:
	.4byte 0x00000008
.L_fr_020086ac:
	.4byte 0x00000000
.L_fr_020086b0:
	.4byte gSleepDisabled
.L_fr_020086b4:
	.4byte gDebugMode
.L_fr_020086b8:
	.4byte gAutoSleepEnabled
.L_fr_020086bc:
	.4byte 0x0000000b
.L_fr_020086c0:
	.4byte Func_0200015c
.L_fr_020086c4:
	.4byte gData_020024f6
.L_fr_020086c8:
	.4byte gData_020024f8
.L_fr_020086cc:
	.4byte 0x00000024
.L_fr_020086d0:
	.4byte gData_02000000
.L_fr_020086d4:
	.4byte IwramCopyWords
.L_fr_020086d8:
	.4byte gMapCellBuffer
.L_fr_020086dc:
	.4byte 0x02013000
.L_fr_020086e0:
	.4byte gData_02003860
.L_fr_020086e4:
	.4byte 0x00000025
.L_fr_020086e8:
	.4byte 0x00000029
.L_fr_020086ec:
	.4byte 0x00000027
.L_fr_020086f0:
	.4byte 0x00000028
.L_fr_020086f4:
	cmp r5, #1
	beq .L_fr_020086fa
	b .L_fr_02008848
.L_fr_020086fa:
	movs r0, #1
	bl Func_020022d8
	movs r2, #1
	adds r5, r0, #0
	negs r2, r2
	cmp r5, r2
	bne .L_fr_0200870c
	b .L_fr_020083f4
.L_fr_0200870c:
	movs r0, #10
	adds r0, #255
	bl GameFlag_SetBit
	ldr r5, .L_fr_02008a30
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
	beq .L_fr_0200876a
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
	b .L_fr_0200883e
.L_fr_0200876a:
	ldr r3, .L_fr_02008a34
	movs r2, #130
	ldr r3, [r3]
	lsls r2, r2, #2
	ands r3, r2
	cmp r3, r2
	bne .L_fr_020087be
	bl Func_020002e0
	cmp r0, #0
	beq .L_fr_02008784
	ldr r0, .L_fr_02008a38
	b .L_fr_020087fe
.L_fr_02008784:
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
	b .L_fr_0200883e
.L_fr_020087be:
	ldr r3, .L_fr_02008a3c
	ldr r2, [r5, #4]
	ldr r3, [r3]
	cmp r2, r3
	beq .L_fr_0200883e
	ldr r6, .L_fr_02008a40
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
	beq .L_fr_020087f0
	bl UiWork_FinalizePendingCore
	b .L_fr_020083f4
.L_fr_020087f0:
	bl UiWork_FinalizePendingCore
	bl Func_020002e0
	cmp r0, #0
	beq .L_fr_02008806
	adds r0, r6, #2
.L_fr_020087fe:
	movs r1, #9
	bl UiText_ShowPositionedMessageAndWait
	b .L_fr_020083f4
.L_fr_02008806:
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
.L_fr_0200883e:
	movs r0, #131
	lsls r0, r0, #1
	bl GameFlag_ClearBit
	b .L_fr_020089f6
.L_fr_02008848:
	cmp r5, #2
	bne .L_fr_02008852
	bl Func_020022e0
	b .L_fr_020083f4
.L_fr_02008852:
	cmp r5, #3
	bne .L_fr_0200885c
	bl Func_020022e8
	b .L_fr_020083f4
.L_fr_0200885c:
	cmp r5, #4
	bne .L_fr_020088d4
	movs r0, #4
	bl Func_020022d8
	movs r3, #1
	adds r5, r0, #0
	negs r3, r3
	cmp r5, r3
	bne .L_fr_02008872
	b .L_fr_020083f4
.L_fr_02008872:
	ldr r5, .L_fr_02008a30
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
	beq .L_fr_02008890
	bl Func_02002390
.L_fr_02008890:
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
	ldr r2, .L_fr_02008a44
	movs r3, #1
	strb r3, [r2]
	ldr r0, .L_fr_02008a48
	movs r1, #1
	bl Func_020023b8
	b .L_fr_020089f6
.L_fr_020088d4:
	cmp r5, #5
	beq .L_fr_020088da
	b .L_fr_020083f4
.L_fr_020088da:
	movs r0, #6
	bl Func_020022d8
	movs r1, #1
	adds r5, r0, #0
	negs r1, r1
	cmp r5, r1
	bne .L_fr_020088ec
	b .L_fr_020083f4
.L_fr_020088ec:
	ldr r3, .L_fr_02008a30
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
.L_fr_02008906:
	movs r0, #2
	bl Func_020022b0
	movs r3, #1
	negs r3, r3
	adds r5, r0, #0
	mov r8, r3
	cmp r5, r8
	beq .L_fr_020088da
	cmp r5, #1
	bne .L_fr_02008962
	ldr r5, .L_fr_02008a4c
	movs r2, #192
	adds r1, r5, #0
	lsls r2, r2, #6
	ldr r6, .L_fr_02008a50
	ldr r0, .L_fr_02008a54
	mov lr, r6
	.2byte 0xf800
	bl Func_02000b24
	adds r7, r0, #0
	cmp r7, r8
	beq .L_fr_0200893a
	bl Func_020023e8
.L_fr_0200893a:
	movs r2, #192
	adds r1, r5, #0
	lsls r2, r2, #6
	ldr r0, .L_fr_02008a58
	mov lr, r6
	.2byte 0xf800
	movs r2, #192
	adds r0, r5, #0
	ldr r1, .L_fr_02008a54
	lsls r2, r2, #6
	mov lr, r6
	.2byte 0xf800
	cmp r7, r8
	beq .L_fr_02008906
	ldr r0, .L_fr_02008a58
	movs r1, #0
	movs r2, #1
	bl Func_02002370
	b .L_fr_020089a6
.L_fr_02008962:
	cmp r5, #0
	bne .L_fr_020089a6
	ldr r5, .L_fr_02008a4c
	movs r2, #192
	adds r1, r5, #0
	lsls r2, r2, #6
	ldr r6, .L_fr_02008a50
	ldr r0, .L_fr_02008a54
	mov lr, r6
	.2byte 0xf800
	bl Func_02002350
	bl Func_02000e44
	movs r2, #192
	adds r7, r0, #0
	adds r1, r5, #0
	lsls r2, r2, #6
	ldr r0, .L_fr_02008a58
	mov lr, r6
	.2byte 0xf800
	movs r2, #192
	adds r0, r5, #0
	ldr r1, .L_fr_02008a54
	lsls r2, r2, #6
	mov lr, r6
	.2byte 0xf800
	cmp r7, r8
	beq .L_fr_02008906
	ldr r0, .L_fr_02008a58
	movs r1, #1
	movs r2, #1
	bl Func_02002370
.L_fr_020089a6:
	ldr r3, .L_fr_02008a5c
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	bne .L_fr_020089ba
	ldr r3, .L_fr_02008a60
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	beq .L_fr_020089ee
.L_fr_020089ba:
	movs r6, #0
	movs r7, #1
.L_fr_020089be:
	movs r0, #6
	bl WaitFrames
	adds r0, r6, #0
	bl Func_020022f8
	movs r3, #1
	adds r5, r0, #0
	negs r3, r3
	cmp r5, r3
	bne .L_fr_020089dc
	cmp r6, #0
	beq .L_fr_020088da
	subs r6, #1
	b .L_fr_020089be
.L_fr_020089dc:
	ldr r3, .L_fr_02008a60
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	beq .L_fr_020089e8
	movs r7, #4
.L_fr_020089e8:
	adds r6, #1
	cmp r6, r7
	blt .L_fr_020089be
.L_fr_020089ee:
	movs r0, #10
	adds r0, #255
	bl GameFlag_SetBit
.L_fr_020089f6:
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
	bl FieldEvent_WaitFrames
	movs r0, #78
	bl Func_02002408
	bl Event_FinishScreenTransition
	bl Event_WaitTransitionFrames
	movs r0, #60
	bl FieldEvent_WaitFrames
.L_fr_02008a24:
	movs r0, #0
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_fr_02008a30:
	.4byte gAdventure
.L_fr_02008a34:
	.4byte gInput
.L_fr_02008a38:
	.4byte 0x00000007
.L_fr_02008a3c:
	.4byte gData_02001000
.L_fr_02008a40:
	.4byte 0x00000005
.L_fr_02008a44:
	.4byte gSleepDisabled
.L_fr_02008a48:
	.4byte 0x00000138
.L_fr_02008a4c:
	.4byte gData_02000000
.L_fr_02008a50:
	.4byte IwramCopyWords
.L_fr_02008a54:
	.4byte gMapCellBuffer
.L_fr_02008a58:
	.4byte 0x02013000
.L_fr_02008a5c:
	.4byte gData_020024f6
.L_fr_02008a60:
	.4byte gData_020024f8
	.else
	.global Func_02000328
	.thumb_func
Func_02000328:
	.ifdef TLA_EDITION_DE
	push {r5, r6, r7, lr}
	mov r7, sl
	mov r6, r8
	push {r6, r7}
	movs r1, #0
	mov sl, r1
	bl Func_0200005c
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_de_020086a0
	bl Task_Add
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	mov r1, sl
	str r1, [r3, #0]
	bl Event_StartScreenTransition
	bl Event_WaitTransitionFrames
	ldr r3, .L_de_020086a4
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #2
	bne .L_de_020083ee
.L_de_0200836a:
	ldr r5, .L_de_020086a8
	movs r1, #5
	adds r0, r5, #0
	bl UiText_ShowPositionedMessageAndWait
	movs r0, #1
	bl Func_020023d8
	adds r6, r0, #0
	bl UiWork_FinalizePendingCore
	cmp r6, #0
	bne .L_de_020083cc
	movs r1, #1
	adds r0, r5, #1
	bl UiText_ShowPositionedMessageAndWait
	ldr r3, .L_de_020086a4
	movs r2, #152
	lsls r2, r2, #1
	adds r2, #255
	adds r3, r3, r2
	movs r2, #1
	strb r2, [r3, #0]
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
	beq .L_de_0200836a
.L_de_020083cc:
	movs r0, #60
	bl FieldEvent_WaitFrames
	bl Event_FinishScreenTransition
	movs r0, #78
	bl Func_02002408
	movs r0, #150
	lsls r0, r0, #1
	bl FieldEvent_WaitFrames
	ldr r0, .L_de_020086ac
	movs r1, #1
	bl Func_020023b8
	b .L_de_02008a24
.L_de_020083ee:
	ldr r3, .L_de_020086b0
	mov r1, sl
	strb r1, [r3, #0]
.L_de_020083f4:
	bl Func_020022f0
	adds r5, r0, #0
	cmp r5, #0
	bge .L_de_02008422
	ldr r3, .L_de_020086b4
	ldrb r3, [r3, #0]
	cmp r3, #0
	beq .L_de_02008422
	ldr r3, .L_de_020086a4
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #74
	adds r3, r3, r2
	movs r2, #1
	strb r2, [r3, #0]
	ldr r3, .L_de_020086b8
	ldr r0, .L_de_020086bc
	strb r2, [r3, #0]
	movs r1, #1
	movs r2, #8
	bl Func_020022b8
.L_de_02008422:
	cmp r5, #0
	bne .L_de_02008446
	mov r3, sl
	cmp r3, #0
	bne .L_de_02008446
	movs r0, #30
	bl WaitFrames
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_de_020086c0
	bl Task_Add
	movs r0, #1
	bl WaitFrames
	movs r1, #1
	mov sl, r1
.L_de_02008446:
	cmp r5, #0
	ble .L_de_02008452
	bl Menu_SelectSaveSlotAction
	adds r5, r0, #0
	b .L_de_02008454
.L_de_02008452:
	movs r5, #0
.L_de_02008454:
	cmp r5, #0
	beq .L_de_0200845a
	b .L_de_020086f4
.L_de_0200845a:
	bl Func_02002350
	ldr r3, .L_de_020086a4
	movs r1, #147
	lsls r1, r1, #1
	adds r1, #255
	adds r2, r3, r1
	ldrb r0, [r2, #0]
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #38
	adds r3, r3, r2
	ldrb r1, [r3, #0]
	bl Func_02002310
	movs r6, #4
	movs r7, #5
.L_de_0200847c:
	movs r0, #6
	bl WaitFrames
	adds r0, r6, #0
	bl Func_020022f8
	movs r3, #1
	adds r5, r0, #0
	negs r3, r3
	cmp r5, r3
	bne .L_de_0200849a
	cmp r6, #4
	beq .L_de_020083f4
	subs r6, #1
	b .L_de_0200847c
.L_de_0200849a:
	ldr r3, .L_de_020086c4
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	bne .L_de_020084ae
	ldr r3, .L_de_020086c8
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	beq .L_de_020084b0
.L_de_020084ae:
	movs r7, #8
.L_de_020084b0:
	adds r6, #1
	cmp r6, r7
	blt .L_de_0200847c
.L_de_020084b6:
	ldr r0, .L_de_020086cc
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
	bne .L_de_0200856a
.L_de_020084d4:
	movs r0, #0
	bl Func_020022b0
	movs r3, #1
	negs r3, r3
	adds r5, r0, #0
	mov r8, r3
	cmp r5, r8
	beq .L_de_020083f4
	cmp r5, #1
	bne .L_de_0200852a
	ldr r5, .L_de_020086d0
	movs r2, #192
	adds r1, r5, #0
	lsls r2, r2, #6
	ldr r6, .L_de_020086d4
	ldr r0, .L_de_020086d8
	mov lr, r6
	.2byte 0xf800
	bl Func_02000b24
	adds r7, r0, #0
	cmp r7, r8
	beq .L_de_02008508
	bl Func_020023e8
.L_de_02008508:
	movs r2, #192
	adds r1, r5, #0
	lsls r2, r2, #6
	ldr r0, .L_de_020086dc
	mov lr, r6
	.2byte 0xf800
	movs r2, #192
	adds r0, r5, #0
	ldr r1, .L_de_020086d8
	lsls r2, r2, #6
	mov lr, r6
	.2byte 0xf800
	cmp r7, r8
	beq .L_de_020084d4
	ldr r0, .L_de_020086dc
	movs r1, #0
	b .L_de_02008562
.L_de_0200852a:
	cmp r5, #0
	bne .L_de_0200856e
	ldr r5, .L_de_020086d0
	movs r2, #192
	adds r1, r5, #0
	lsls r2, r2, #6
	ldr r6, .L_de_020086d4
	ldr r0, .L_de_020086d8
	mov lr, r6
	.2byte 0xf800
	bl Func_02002350
	bl Func_02000e44
	movs r2, #192
	adds r1, r5, #0
	lsls r2, r2, #6
	ldr r0, .L_de_020086dc
	mov lr, r6
	.2byte 0xf800
	movs r2, #192
	ldr r1, .L_de_020086d8
	lsls r2, r2, #6
	adds r0, r5, #0
	mov lr, r6
	.2byte 0xf800
	ldr r0, .L_de_020086dc
	movs r1, #1
.L_de_02008562:
	movs r2, #0
	bl Func_02002370
	b .L_de_0200856e
.L_de_0200856a:
	bl Func_02002398
.L_de_0200856e:
	movs r6, #0
	movs r7, #1
.L_de_02008572:
	movs r0, #6
	bl WaitFrames
	adds r0, r6, #0
	bl Func_020022f8
	movs r1, #1
	adds r5, r0, #0
	negs r1, r1
	cmp r5, r1
	bne .L_de_02008590
	cmp r6, #0
	beq .L_de_020084b6
	subs r6, #1
	b .L_de_02008572
.L_de_02008590:
	ldr r3, .L_de_020086c8
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	beq .L_de_0200859c
	movs r7, #4
.L_de_0200859c:
	adds r6, #1
	cmp r6, r7
	blt .L_de_02008572
	ldr r3, .L_de_020086e0
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	beq .L_de_02008696
.L_de_020085ac:
	ldr r5, .L_de_020086e4
	movs r1, #13
	adds r0, r5, #0
	bl UiText_ShowPositionedMessageAndWait
	ldr r1, .L_de_020086e8
	movs r0, #1
	movs r2, #8
	movs r3, #0
	bl Menu_RunConfirmSelection
	adds r6, r0, #0
	bl UiWork_FinalizePendingCore
	cmp r6, #0
	bne .L_de_02008642
	ldr r3, .L_de_020086e0
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	bne .L_de_020085e0
	adds r0, r5, #1
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
	b .L_de_02008642
.L_de_020085e0:
	ldr r6, .L_de_020086d0
	movs r2, #192
	adds r1, r6, #0
	lsls r2, r2, #6
	ldr r7, .L_de_020086d4
	ldr r0, .L_de_020086d8
	mov lr, r7
	.2byte 0xf800
	movs r0, #5
	bl Func_020022d8
	adds r5, r0, #0
	cmp r5, #0
	bne .L_de_020085ac
	ldr r5, .L_de_020086a4
	movs r1, #147
	lsls r1, r1, #1
	movs r2, #128
	adds r1, #255
	lsls r2, r2, #2
	adds r3, r5, r1
	adds r2, #38
	ldrb r0, [r3, #0]
	adds r3, r5, r2
	ldrb r1, [r3, #0]
	bl Func_02002310
	movs r2, #192
	adds r1, r6, #0
	lsls r2, r2, #6
	ldr r0, .L_de_020086dc
	mov lr, r7
	.2byte 0xf800
	movs r2, #192
	ldr r1, .L_de_020086d8
	lsls r2, r2, #6
	adds r0, r6, #0
	mov lr, r7
	.2byte 0xf800
	ldr r0, .L_de_020086dc
	bl Func_020023a0
	movs r3, #196
	lsls r3, r3, #1
	adds r3, #255
	adds r5, r5, r3
	movs r3, #1
	strb r3, [r5, #0]
	b .L_de_02008696
.L_de_02008642:
	ldr r0, .L_de_020086ec
	movs r1, #13
	bl UiText_ShowPositionedMessageAndWait
	ldr r5, .L_de_020086e8
	movs r0, #1
	adds r1, r5, #0
	movs r2, #8
	movs r3, #0
	bl Menu_RunConfirmSelection
	adds r6, r0, #0
	bl UiWork_FinalizePendingCore
	cmp r6, #0
	bne .L_de_02008678
	movs r0, #46
	bl GameFlag_SetBit
	ldr r3, .L_de_020086a4
	movs r1, #196
	lsls r1, r1, #1
	adds r1, #255
	adds r3, r3, r1
	movs r2, #2
	strb r2, [r3, #0]
	b .L_de_02008696
.L_de_02008678:
	ldr r0, .L_de_020086f0
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
	bne .L_de_020085ac
.L_de_02008696:
	ldr r0, .L_de_020086ac
	movs r1, #8
	bl Func_020023b8
	b .L_de_020089f6
.L_de_020086a0:
	.4byte Func_020001f4
.L_de_020086a4:
	.4byte gAdventure
.L_de_020086a8:
	.4byte 0x00000008
.L_de_020086ac:
	.4byte 0x00000000
.L_de_020086b0:
	.4byte gSleepDisabled
.L_de_020086b4:
	.4byte gDebugMode
.L_de_020086b8:
	.4byte gAutoSleepEnabled
.L_de_020086bc:
	.4byte 0x0000000b
.L_de_020086c0:
	.4byte Func_0200015c
.L_de_020086c4:
	.4byte gData_020024f6
.L_de_020086c8:
	.4byte gData_020024f8
.L_de_020086cc:
	.4byte 0x00000024
.L_de_020086d0:
	.4byte gData_02000000
.L_de_020086d4:
	.4byte IwramCopyWords
.L_de_020086d8:
	.4byte gMapCellBuffer
.L_de_020086dc:
	.4byte 0x02013000
.L_de_020086e0:
	.4byte gData_02003860
.L_de_020086e4:
	.4byte 0x00000025
.L_de_020086e8:
	.4byte 0x00000029
.L_de_020086ec:
	.4byte 0x00000027
.L_de_020086f0:
	.4byte 0x00000028
.L_de_020086f4:
	cmp r5, #1
	beq .L_de_020086fa
	b .L_de_02008848
.L_de_020086fa:
	movs r0, #1
	bl Func_020022d8
	movs r2, #1
	adds r5, r0, #0
	negs r2, r2
	cmp r5, r2
	bne .L_de_0200870c
	b .L_de_020083f4
.L_de_0200870c:
	movs r0, #10
	adds r0, #255
	bl GameFlag_SetBit
	ldr r5, .L_de_02008a30
	movs r1, #147
	lsls r1, r1, #1
	movs r2, #128
	adds r1, #255
	lsls r2, r2, #2
	adds r3, r5, r1
	adds r2, #38
	ldrb r0, [r3, #0]
	adds r3, r5, r2
	ldrb r1, [r3, #0]
	bl Func_02002310
	bl Func_02002388
	ldr r3, [r5, #0]
	cmp r3, r0
	beq .L_de_0200876a
	movs r1, #242
	lsls r1, r1, #1
	adds r3, r5, r1
	ldrh r0, [r3, #0]
	movs r2, #240
	lsls r2, r2, #1
	adds r3, r5, r2
	strh r0, [r3, #0]
	adds r1, #2
	adds r3, r5, r1
	ldrh r1, [r3, #0]
	adds r2, #2
	adds r3, r5, r2
	strh r1, [r3, #0]
	lsls r0, r0, #16
	lsls r1, r1, #16
	asrs r0, r0, #16
	asrs r1, r1, #16
	bl Func_020023e0
	movs r0, #10
	adds r0, #255
	bl GameFlag_ClearBit
	b .L_de_0200883e
.L_de_0200876a:
	ldr r3, .L_de_02008a34
	movs r2, #130
	ldr r3, [r3, #0]
	lsls r2, r2, #2
	ands r3, r2
	cmp r3, r2
	bne .L_de_020087be
	bl Func_020002e0
	cmp r0, #0
	beq .L_de_02008784
	ldr r0, .L_de_02008a38
	b .L_de_020087fe
.L_de_02008784:
	movs r1, #242
	lsls r1, r1, #1
	adds r3, r5, r1
	ldrh r0, [r3, #0]
	movs r2, #240
	lsls r2, r2, #1
	adds r3, r5, r2
	strh r0, [r3, #0]
	adds r1, #2
	adds r3, r5, r1
	ldrh r1, [r3, #0]
	adds r2, #2
	adds r3, r5, r2
	strh r1, [r3, #0]
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
	b .L_de_0200883e
.L_de_020087be:
	ldr r3, .L_de_02008a3c
	ldr r2, [r5, #4]
	ldr r3, [r3, #0]
	cmp r2, r3
	beq .L_de_0200883e
	ldr r6, .L_de_02008a40
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
	beq .L_de_020087f0
	bl UiWork_FinalizePendingCore
	b .L_de_020083f4
.L_de_020087f0:
	bl UiWork_FinalizePendingCore
	bl Func_020002e0
	cmp r0, #0
	beq .L_de_02008806
	adds r0, r6, #2
.L_de_020087fe:
	movs r1, #9
	bl UiText_ShowPositionedMessageAndWait
	b .L_de_020083f4
.L_de_02008806:
	movs r1, #242
	lsls r1, r1, #1
	adds r3, r5, r1
	ldrh r0, [r3, #0]
	movs r2, #240
	lsls r2, r2, #1
	adds r3, r5, r2
	strh r0, [r3, #0]
	adds r1, #2
	adds r3, r5, r1
	ldrh r1, [r3, #0]
	adds r2, #2
	adds r3, r5, r2
	strh r1, [r3, #0]
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
.L_de_0200883e:
	movs r0, #131
	lsls r0, r0, #1
	bl GameFlag_ClearBit
	b .L_de_020089f6
.L_de_02008848:
	cmp r5, #2
	bne .L_de_02008852
	bl Func_020022e0
	b .L_de_020083f4
.L_de_02008852:
	cmp r5, #3
	bne .L_de_0200885c
	bl Func_020022e8
	b .L_de_020083f4
.L_de_0200885c:
	cmp r5, #4
	bne .L_de_020088d4
	movs r0, #4
	bl Func_020022d8
	movs r3, #1
	adds r5, r0, #0
	negs r3, r3
	cmp r5, r3
	bne .L_de_02008872
	b .L_de_020083f4
.L_de_02008872:
	ldr r5, .L_de_02008a30
	movs r1, #133
	lsls r1, r1, #2
	movs r0, #144
	adds r3, r5, r1
	movs r2, #4
	lsls r0, r0, #4
	str r2, [r3, #0]
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_de_02008890
	bl Func_02002390
.L_de_02008890:
	movs r2, #147
	lsls r2, r2, #1
	movs r1, #128
	adds r2, #255
	lsls r1, r1, #2
	adds r3, r5, r2
	adds r1, #38
	ldrb r0, [r3, #0]
	adds r3, r5, r1
	ldrb r1, [r3, #0]
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
	strh r3, [r2, #0]
	ldr r2, .L_de_02008a44
	movs r3, #1
	strb r3, [r2, #0]
	ldr r0, .L_de_02008a48
	movs r1, #1
	bl Func_020023b8
	b .L_de_020089f6
.L_de_020088d4:
	cmp r5, #5
	beq .L_de_020088da
	b .L_de_020083f4
.L_de_020088da:
	movs r0, #6
	bl Func_020022d8
	movs r1, #1
	adds r5, r0, #0
	negs r1, r1
	cmp r5, r1
	bne .L_de_020088ec
	b .L_de_020083f4
.L_de_020088ec:
	ldr r3, .L_de_02008a30
	movs r1, #147
	lsls r1, r1, #1
	adds r1, #255
	adds r2, r3, r1
	ldrb r0, [r2, #0]
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #38
	adds r3, r3, r2
	ldrb r1, [r3, #0]
	bl Func_02002310
.L_de_02008906:
	movs r0, #2
	bl Func_020022b0
	movs r3, #1
	negs r3, r3
	adds r5, r0, #0
	mov r8, r3
	cmp r5, r8
	beq .L_de_020088da
	cmp r5, #1
	bne .L_de_02008962
	ldr r5, .L_de_02008a4c
	movs r2, #192
	adds r1, r5, #0
	lsls r2, r2, #6
	ldr r6, .L_de_02008a50
	ldr r0, .L_de_02008a54
	mov lr, r6
	.2byte 0xf800
	bl Func_02000b24
	adds r7, r0, #0
	cmp r7, r8
	beq .L_de_0200893a
	bl Func_020023e8
.L_de_0200893a:
	movs r2, #192
	adds r1, r5, #0
	lsls r2, r2, #6
	ldr r0, .L_de_02008a58
	mov lr, r6
	.2byte 0xf800
	movs r2, #192
	adds r0, r5, #0
	ldr r1, .L_de_02008a54
	lsls r2, r2, #6
	mov lr, r6
	.2byte 0xf800
	cmp r7, r8
	beq .L_de_02008906
	ldr r0, .L_de_02008a58
	movs r1, #0
	movs r2, #1
	bl Func_02002370
	b .L_de_020089a6
.L_de_02008962:
	cmp r5, #0
	bne .L_de_020089a6
	ldr r5, .L_de_02008a4c
	movs r2, #192
	adds r1, r5, #0
	lsls r2, r2, #6
	ldr r6, .L_de_02008a50
	ldr r0, .L_de_02008a54
	mov lr, r6
	.2byte 0xf800
	bl Func_02002350
	bl Func_02000e44
	movs r2, #192
	adds r7, r0, #0
	adds r1, r5, #0
	lsls r2, r2, #6
	ldr r0, .L_de_02008a58
	mov lr, r6
	.2byte 0xf800
	movs r2, #192
	adds r0, r5, #0
	ldr r1, .L_de_02008a54
	lsls r2, r2, #6
	mov lr, r6
	.2byte 0xf800
	cmp r7, r8
	beq .L_de_02008906
	ldr r0, .L_de_02008a58
	movs r1, #1
	movs r2, #1
	bl Func_02002370
.L_de_020089a6:
	ldr r3, .L_de_02008a5c
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	bne .L_de_020089ba
	ldr r3, .L_de_02008a60
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	beq .L_de_020089ee
.L_de_020089ba:
	movs r6, #0
	movs r7, #1
.L_de_020089be:
	movs r0, #6
	bl WaitFrames
	adds r0, r6, #0
	bl Func_020022f8
	movs r3, #1
	adds r5, r0, #0
	negs r3, r3
	cmp r5, r3
	bne .L_de_020089dc
	cmp r6, #0
	beq .L_de_020088da
	subs r6, #1
	b .L_de_020089be
.L_de_020089dc:
	ldr r3, .L_de_02008a60
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	beq .L_de_020089e8
	movs r7, #4
.L_de_020089e8:
	adds r6, #1
	cmp r6, r7
	blt .L_de_020089be
.L_de_020089ee:
	movs r0, #10
	adds r0, #255
	bl GameFlag_SetBit
.L_de_020089f6:
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #172
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #186
	lsls r2, r2, #2
	adds r2, #255
	strh r2, [r3, #0]
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r0, #78
	bl Func_02002408
	bl Event_FinishScreenTransition
	bl Event_WaitTransitionFrames
	movs r0, #60
	bl FieldEvent_WaitFrames
.L_de_02008a24:
	movs r0, #0
	pop {r3, r5}
	mov r8, r3
	mov sl, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_de_02008a30:
	.4byte gAdventure
.L_de_02008a34:
	.4byte gInput
.L_de_02008a38:
	.4byte 0x00000007
.L_de_02008a3c:
	.4byte gData_02001000
.L_de_02008a40:
	.4byte 0x00000005
.L_de_02008a44:
	.4byte gSleepDisabled
.L_de_02008a48:
	.4byte 0x00000138
.L_de_02008a4c:
	.4byte gData_02000000
.L_de_02008a50:
	.4byte IwramCopyWords
.L_de_02008a54:
	.4byte gMapCellBuffer
.L_de_02008a58:
	.4byte 0x02013000
.L_de_02008a5c:
	.4byte gData_020024f6
.L_de_02008a60:
	.4byte gData_020024f8
	.else
	.ifdef TLA_EDITION_ES
	push {r5, r6, r7, lr}
	mov r7, sl
	mov r6, r8
	push {r6, r7}
	movs r1, #0
	mov sl, r1
	bl Func_0200005c
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_es_020086a0
	bl Task_Add
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	mov r1, sl
	str r1, [r3, #0]
	bl Event_StartScreenTransition
	bl Event_WaitTransitionFrames
	ldr r3, .L_es_020086a4
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #2
	bne .L_es_020083ee
.L_es_0200836a:
	ldr r5, .L_es_020086a8
	movs r1, #5
	adds r0, r5, #0
	bl UiText_ShowPositionedMessageAndWait
	movs r0, #1
	bl Func_020023d8
	adds r6, r0, #0
	bl UiWork_FinalizePendingCore
	cmp r6, #0
	bne .L_es_020083cc
	movs r1, #1
	adds r0, r5, #1
	bl UiText_ShowPositionedMessageAndWait
	ldr r3, .L_es_020086a4
	movs r2, #152
	lsls r2, r2, #1
	adds r2, #255
	adds r3, r3, r2
	movs r2, #1
	strb r2, [r3, #0]
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
	beq .L_es_0200836a
.L_es_020083cc:
	movs r0, #60
	bl FieldEvent_WaitFrames
	bl Event_FinishScreenTransition
	movs r0, #78
	bl Func_02002408
	movs r0, #150
	lsls r0, r0, #1
	bl FieldEvent_WaitFrames
	ldr r0, .L_es_020086ac
	movs r1, #1
	bl Func_020023b8
	b .L_es_02008a24
.L_es_020083ee:
	ldr r3, .L_es_020086b0
	mov r1, sl
	strb r1, [r3, #0]
.L_es_020083f4:
	bl Func_020022f0
	adds r5, r0, #0
	cmp r5, #0
	bge .L_es_02008422
	ldr r3, .L_es_020086b4
	ldrb r3, [r3, #0]
	cmp r3, #0
	beq .L_es_02008422
	ldr r3, .L_es_020086a4
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #74
	adds r3, r3, r2
	movs r2, #1
	strb r2, [r3, #0]
	ldr r3, .L_es_020086b8
	ldr r0, .L_es_020086bc
	strb r2, [r3, #0]
	movs r1, #1
	movs r2, #8
	bl Func_020022b8
.L_es_02008422:
	cmp r5, #0
	bne .L_es_02008446
	mov r3, sl
	cmp r3, #0
	bne .L_es_02008446
	movs r0, #30
	bl WaitFrames
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_es_020086c0
	bl Task_Add
	movs r0, #1
	bl WaitFrames
	movs r1, #1
	mov sl, r1
.L_es_02008446:
	cmp r5, #0
	ble .L_es_02008452
	bl Menu_SelectSaveSlotAction
	adds r5, r0, #0
	b .L_es_02008454
.L_es_02008452:
	movs r5, #0
.L_es_02008454:
	cmp r5, #0
	beq .L_es_0200845a
	b .L_es_020086f4
.L_es_0200845a:
	bl Func_02002350
	ldr r3, .L_es_020086a4
	movs r1, #147
	lsls r1, r1, #1
	adds r1, #255
	adds r2, r3, r1
	ldrb r0, [r2, #0]
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #38
	adds r3, r3, r2
	ldrb r1, [r3, #0]
	bl Func_02002310
	movs r6, #4
	movs r7, #5
.L_es_0200847c:
	movs r0, #6
	bl WaitFrames
	adds r0, r6, #0
	bl Func_020022f8
	movs r3, #1
	adds r5, r0, #0
	negs r3, r3
	cmp r5, r3
	bne .L_es_0200849a
	cmp r6, #4
	beq .L_es_020083f4
	subs r6, #1
	b .L_es_0200847c
.L_es_0200849a:
	ldr r3, .L_es_020086c4
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	bne .L_es_020084ae
	ldr r3, .L_es_020086c8
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	beq .L_es_020084b0
.L_es_020084ae:
	movs r7, #8
.L_es_020084b0:
	adds r6, #1
	cmp r6, r7
	blt .L_es_0200847c
.L_es_020084b6:
	ldr r0, .L_es_020086cc
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
	bne .L_es_0200856a
.L_es_020084d4:
	movs r0, #0
	bl Func_020022b0
	movs r3, #1
	negs r3, r3
	adds r5, r0, #0
	mov r8, r3
	cmp r5, r8
	beq .L_es_020083f4
	cmp r5, #1
	bne .L_es_0200852a
	ldr r5, .L_es_020086d0
	movs r2, #192
	adds r1, r5, #0
	lsls r2, r2, #6
	ldr r6, .L_es_020086d4
	ldr r0, .L_es_020086d8
	mov lr, r6
	.2byte 0xf800
	bl Func_02000b24
	adds r7, r0, #0
	cmp r7, r8
	beq .L_es_02008508
	bl Func_020023e8
.L_es_02008508:
	movs r2, #192
	adds r1, r5, #0
	lsls r2, r2, #6
	ldr r0, .L_es_020086dc
	mov lr, r6
	.2byte 0xf800
	movs r2, #192
	adds r0, r5, #0
	ldr r1, .L_es_020086d8
	lsls r2, r2, #6
	mov lr, r6
	.2byte 0xf800
	cmp r7, r8
	beq .L_es_020084d4
	ldr r0, .L_es_020086dc
	movs r1, #0
	b .L_es_02008562
.L_es_0200852a:
	cmp r5, #0
	bne .L_es_0200856e
	ldr r5, .L_es_020086d0
	movs r2, #192
	adds r1, r5, #0
	lsls r2, r2, #6
	ldr r6, .L_es_020086d4
	ldr r0, .L_es_020086d8
	mov lr, r6
	.2byte 0xf800
	bl Func_02002350
	bl Func_02000e44
	movs r2, #192
	adds r1, r5, #0
	lsls r2, r2, #6
	ldr r0, .L_es_020086dc
	mov lr, r6
	.2byte 0xf800
	movs r2, #192
	ldr r1, .L_es_020086d8
	lsls r2, r2, #6
	adds r0, r5, #0
	mov lr, r6
	.2byte 0xf800
	ldr r0, .L_es_020086dc
	movs r1, #1
.L_es_02008562:
	movs r2, #0
	bl Func_02002370
	b .L_es_0200856e
.L_es_0200856a:
	bl Func_02002398
.L_es_0200856e:
	movs r6, #0
	movs r7, #1
.L_es_02008572:
	movs r0, #6
	bl WaitFrames
	adds r0, r6, #0
	bl Func_020022f8
	movs r1, #1
	adds r5, r0, #0
	negs r1, r1
	cmp r5, r1
	bne .L_es_02008590
	cmp r6, #0
	beq .L_es_020084b6
	subs r6, #1
	b .L_es_02008572
.L_es_02008590:
	ldr r3, .L_es_020086c8
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	beq .L_es_0200859c
	movs r7, #4
.L_es_0200859c:
	adds r6, #1
	cmp r6, r7
	blt .L_es_02008572
	ldr r3, .L_es_020086e0
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	beq .L_es_02008696
.L_es_020085ac:
	ldr r5, .L_es_020086e4
	movs r1, #13
	adds r0, r5, #0
	bl UiText_ShowPositionedMessageAndWait
	ldr r1, .L_es_020086e8
	movs r0, #1
	movs r2, #10
	movs r3, #0
	bl Menu_RunConfirmSelection
	adds r6, r0, #0
	bl UiWork_FinalizePendingCore
	cmp r6, #0
	bne .L_es_02008642
	ldr r3, .L_es_020086e0
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	bne .L_es_020085e0
	adds r0, r5, #1
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
	b .L_es_02008642
.L_es_020085e0:
	ldr r6, .L_es_020086d0
	movs r2, #192
	adds r1, r6, #0
	lsls r2, r2, #6
	ldr r7, .L_es_020086d4
	ldr r0, .L_es_020086d8
	mov lr, r7
	.2byte 0xf800
	movs r0, #5
	bl Func_020022d8
	adds r5, r0, #0
	cmp r5, #0
	bne .L_es_020085ac
	ldr r5, .L_es_020086a4
	movs r1, #147
	lsls r1, r1, #1
	movs r2, #128
	adds r1, #255
	lsls r2, r2, #2
	adds r3, r5, r1
	adds r2, #38
	ldrb r0, [r3, #0]
	adds r3, r5, r2
	ldrb r1, [r3, #0]
	bl Func_02002310
	movs r2, #192
	adds r1, r6, #0
	lsls r2, r2, #6
	ldr r0, .L_es_020086dc
	mov lr, r7
	.2byte 0xf800
	movs r2, #192
	ldr r1, .L_es_020086d8
	lsls r2, r2, #6
	adds r0, r6, #0
	mov lr, r7
	.2byte 0xf800
	ldr r0, .L_es_020086dc
	bl Func_020023a0
	movs r3, #196
	lsls r3, r3, #1
	adds r3, #255
	adds r5, r5, r3
	movs r3, #1
	strb r3, [r5, #0]
	b .L_es_02008696
.L_es_02008642:
	ldr r0, .L_es_020086ec
	movs r1, #13
	bl UiText_ShowPositionedMessageAndWait
	ldr r5, .L_es_020086e8
	movs r0, #1
	adds r1, r5, #0
	movs r2, #10
	movs r3, #0
	bl Menu_RunConfirmSelection
	adds r6, r0, #0
	bl UiWork_FinalizePendingCore
	cmp r6, #0
	bne .L_es_02008678
	movs r0, #46
	bl GameFlag_SetBit
	ldr r3, .L_es_020086a4
	movs r1, #196
	lsls r1, r1, #1
	adds r1, #255
	adds r3, r3, r1
	movs r2, #2
	strb r2, [r3, #0]
	b .L_es_02008696
.L_es_02008678:
	ldr r0, .L_es_020086f0
	movs r1, #13
	bl UiText_ShowPositionedMessageAndWait
	movs r0, #1
	adds r1, r5, #0
	movs r2, #10
	movs r3, #0
	bl Menu_RunConfirmSelection
	adds r6, r0, #0
	bl UiWork_FinalizePendingCore
	cmp r6, #0
	bne .L_es_020085ac
.L_es_02008696:
	ldr r0, .L_es_020086ac
	movs r1, #8
	bl Func_020023b8
	b .L_es_020089f6
.L_es_020086a0:
	.4byte Func_020001f4
.L_es_020086a4:
	.4byte gAdventure
.L_es_020086a8:
	.4byte 0x00000008
.L_es_020086ac:
	.4byte 0x00000000
.L_es_020086b0:
	.4byte gSleepDisabled
.L_es_020086b4:
	.4byte gDebugMode
.L_es_020086b8:
	.4byte gAutoSleepEnabled
.L_es_020086bc:
	.4byte 0x0000000b
.L_es_020086c0:
	.4byte Func_0200015c
.L_es_020086c4:
	.4byte gData_020024f6
.L_es_020086c8:
	.4byte gData_020024f8
.L_es_020086cc:
	.4byte 0x00000024
.L_es_020086d0:
	.4byte gData_02000000
.L_es_020086d4:
	.4byte IwramCopyWords
.L_es_020086d8:
	.4byte gMapCellBuffer
.L_es_020086dc:
	.4byte 0x02013000
.L_es_020086e0:
	.4byte gData_02003860
.L_es_020086e4:
	.4byte 0x00000025
.L_es_020086e8:
	.4byte 0x00000029
.L_es_020086ec:
	.4byte 0x00000027
.L_es_020086f0:
	.4byte 0x00000028
.L_es_020086f4:
	cmp r5, #1
	beq .L_es_020086fa
	b .L_es_02008848
.L_es_020086fa:
	movs r0, #1
	bl Func_020022d8
	movs r2, #1
	adds r5, r0, #0
	negs r2, r2
	cmp r5, r2
	bne .L_es_0200870c
	b .L_es_020083f4
.L_es_0200870c:
	movs r0, #10
	adds r0, #255
	bl GameFlag_SetBit
	ldr r5, .L_es_02008a30
	movs r1, #147
	lsls r1, r1, #1
	movs r2, #128
	adds r1, #255
	lsls r2, r2, #2
	adds r3, r5, r1
	adds r2, #38
	ldrb r0, [r3, #0]
	adds r3, r5, r2
	ldrb r1, [r3, #0]
	bl Func_02002310
	bl Func_02002388
	ldr r3, [r5, #0]
	cmp r3, r0
	beq .L_es_0200876a
	movs r1, #242
	lsls r1, r1, #1
	adds r3, r5, r1
	ldrh r0, [r3, #0]
	movs r2, #240
	lsls r2, r2, #1
	adds r3, r5, r2
	strh r0, [r3, #0]
	adds r1, #2
	adds r3, r5, r1
	ldrh r1, [r3, #0]
	adds r2, #2
	adds r3, r5, r2
	strh r1, [r3, #0]
	lsls r0, r0, #16
	lsls r1, r1, #16
	asrs r0, r0, #16
	asrs r1, r1, #16
	bl Func_020023e0
	movs r0, #10
	adds r0, #255
	bl GameFlag_ClearBit
	b .L_es_0200883e
.L_es_0200876a:
	ldr r3, .L_es_02008a34
	movs r2, #130
	ldr r3, [r3, #0]
	lsls r2, r2, #2
	ands r3, r2
	cmp r3, r2
	bne .L_es_020087be
	bl Func_020002e0
	cmp r0, #0
	beq .L_es_02008784
	ldr r0, .L_es_02008a38
	b .L_es_020087fe
.L_es_02008784:
	movs r1, #242
	lsls r1, r1, #1
	adds r3, r5, r1
	ldrh r0, [r3, #0]
	movs r2, #240
	lsls r2, r2, #1
	adds r3, r5, r2
	strh r0, [r3, #0]
	adds r1, #2
	adds r3, r5, r1
	ldrh r1, [r3, #0]
	adds r2, #2
	adds r3, r5, r2
	strh r1, [r3, #0]
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
	b .L_es_0200883e
.L_es_020087be:
	ldr r3, .L_es_02008a3c
	ldr r2, [r5, #4]
	ldr r3, [r3, #0]
	cmp r2, r3
	beq .L_es_0200883e
	ldr r6, .L_es_02008a40
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
	beq .L_es_020087f0
	bl UiWork_FinalizePendingCore
	b .L_es_020083f4
.L_es_020087f0:
	bl UiWork_FinalizePendingCore
	bl Func_020002e0
	cmp r0, #0
	beq .L_es_02008806
	adds r0, r6, #2
.L_es_020087fe:
	movs r1, #9
	bl UiText_ShowPositionedMessageAndWait
	b .L_es_020083f4
.L_es_02008806:
	movs r1, #242
	lsls r1, r1, #1
	adds r3, r5, r1
	ldrh r0, [r3, #0]
	movs r2, #240
	lsls r2, r2, #1
	adds r3, r5, r2
	strh r0, [r3, #0]
	adds r1, #2
	adds r3, r5, r1
	ldrh r1, [r3, #0]
	adds r2, #2
	adds r3, r5, r2
	strh r1, [r3, #0]
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
.L_es_0200883e:
	movs r0, #131
	lsls r0, r0, #1
	bl GameFlag_ClearBit
	b .L_es_020089f6
.L_es_02008848:
	cmp r5, #2
	bne .L_es_02008852
	bl Func_020022e0
	b .L_es_020083f4
.L_es_02008852:
	cmp r5, #3
	bne .L_es_0200885c
	bl Func_020022e8
	b .L_es_020083f4
.L_es_0200885c:
	cmp r5, #4
	bne .L_es_020088d4
	movs r0, #4
	bl Func_020022d8
	movs r3, #1
	adds r5, r0, #0
	negs r3, r3
	cmp r5, r3
	bne .L_es_02008872
	b .L_es_020083f4
.L_es_02008872:
	ldr r5, .L_es_02008a30
	movs r1, #133
	lsls r1, r1, #2
	movs r0, #144
	adds r3, r5, r1
	movs r2, #4
	lsls r0, r0, #4
	str r2, [r3, #0]
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_es_02008890
	bl Func_02002390
.L_es_02008890:
	movs r2, #147
	lsls r2, r2, #1
	movs r1, #128
	adds r2, #255
	lsls r1, r1, #2
	adds r3, r5, r2
	adds r1, #38
	ldrb r0, [r3, #0]
	adds r3, r5, r1
	ldrb r1, [r3, #0]
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
	strh r3, [r2, #0]
	ldr r2, .L_es_02008a44
	movs r3, #1
	strb r3, [r2, #0]
	ldr r0, .L_es_02008a48
	movs r1, #1
	bl Func_020023b8
	b .L_es_020089f6
.L_es_020088d4:
	cmp r5, #5
	beq .L_es_020088da
	b .L_es_020083f4
.L_es_020088da:
	movs r0, #6
	bl Func_020022d8
	movs r1, #1
	adds r5, r0, #0
	negs r1, r1
	cmp r5, r1
	bne .L_es_020088ec
	b .L_es_020083f4
.L_es_020088ec:
	ldr r3, .L_es_02008a30
	movs r1, #147
	lsls r1, r1, #1
	adds r1, #255
	adds r2, r3, r1
	ldrb r0, [r2, #0]
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #38
	adds r3, r3, r2
	ldrb r1, [r3, #0]
	bl Func_02002310
.L_es_02008906:
	movs r0, #2
	bl Func_020022b0
	movs r3, #1
	negs r3, r3
	adds r5, r0, #0
	mov r8, r3
	cmp r5, r8
	beq .L_es_020088da
	cmp r5, #1
	bne .L_es_02008962
	ldr r5, .L_es_02008a4c
	movs r2, #192
	adds r1, r5, #0
	lsls r2, r2, #6
	ldr r6, .L_es_02008a50
	ldr r0, .L_es_02008a54
	mov lr, r6
	.2byte 0xf800
	bl Func_02000b24
	adds r7, r0, #0
	cmp r7, r8
	beq .L_es_0200893a
	bl Func_020023e8
.L_es_0200893a:
	movs r2, #192
	adds r1, r5, #0
	lsls r2, r2, #6
	ldr r0, .L_es_02008a58
	mov lr, r6
	.2byte 0xf800
	movs r2, #192
	adds r0, r5, #0
	ldr r1, .L_es_02008a54
	lsls r2, r2, #6
	mov lr, r6
	.2byte 0xf800
	cmp r7, r8
	beq .L_es_02008906
	ldr r0, .L_es_02008a58
	movs r1, #0
	movs r2, #1
	bl Func_02002370
	b .L_es_020089a6
.L_es_02008962:
	cmp r5, #0
	bne .L_es_020089a6
	ldr r5, .L_es_02008a4c
	movs r2, #192
	adds r1, r5, #0
	lsls r2, r2, #6
	ldr r6, .L_es_02008a50
	ldr r0, .L_es_02008a54
	mov lr, r6
	.2byte 0xf800
	bl Func_02002350
	bl Func_02000e44
	movs r2, #192
	adds r7, r0, #0
	adds r1, r5, #0
	lsls r2, r2, #6
	ldr r0, .L_es_02008a58
	mov lr, r6
	.2byte 0xf800
	movs r2, #192
	adds r0, r5, #0
	ldr r1, .L_es_02008a54
	lsls r2, r2, #6
	mov lr, r6
	.2byte 0xf800
	cmp r7, r8
	beq .L_es_02008906
	ldr r0, .L_es_02008a58
	movs r1, #1
	movs r2, #1
	bl Func_02002370
.L_es_020089a6:
	ldr r3, .L_es_02008a5c
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	bne .L_es_020089ba
	ldr r3, .L_es_02008a60
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	beq .L_es_020089ee
.L_es_020089ba:
	movs r6, #0
	movs r7, #1
.L_es_020089be:
	movs r0, #6
	bl WaitFrames
	adds r0, r6, #0
	bl Func_020022f8
	movs r3, #1
	adds r5, r0, #0
	negs r3, r3
	cmp r5, r3
	bne .L_es_020089dc
	cmp r6, #0
	beq .L_es_020088da
	subs r6, #1
	b .L_es_020089be
.L_es_020089dc:
	ldr r3, .L_es_02008a60
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	beq .L_es_020089e8
	movs r7, #4
.L_es_020089e8:
	adds r6, #1
	cmp r6, r7
	blt .L_es_020089be
.L_es_020089ee:
	movs r0, #10
	adds r0, #255
	bl GameFlag_SetBit
.L_es_020089f6:
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #172
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #186
	lsls r2, r2, #2
	adds r2, #255
	strh r2, [r3, #0]
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r0, #78
	bl Func_02002408
	bl Event_FinishScreenTransition
	bl Event_WaitTransitionFrames
	movs r0, #60
	bl FieldEvent_WaitFrames
.L_es_02008a24:
	movs r0, #0
	pop {r3, r5}
	mov r8, r3
	mov sl, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_es_02008a30:
	.4byte gAdventure
.L_es_02008a34:
	.4byte gInput
.L_es_02008a38:
	.4byte 0x00000007
.L_es_02008a3c:
	.4byte gData_02001000
.L_es_02008a40:
	.4byte 0x00000005
.L_es_02008a44:
	.4byte gSleepDisabled
.L_es_02008a48:
	.4byte 0x00000138
.L_es_02008a4c:
	.4byte gData_02000000
.L_es_02008a50:
	.4byte IwramCopyWords
.L_es_02008a54:
	.4byte gMapCellBuffer
.L_es_02008a58:
	.4byte 0x02013000
.L_es_02008a5c:
	.4byte gData_020024f6
.L_es_02008a60:
	.4byte gData_020024f8
	.else
	.ifdef TLA_EDITION_IT
	push {r5, r6, r7, lr}
	mov r7, sl
	mov r6, r8
	push {r6, r7}
	movs r1, #0
	mov sl, r1
	bl Func_0200005c
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_it_020086a0
	bl Task_Add
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	mov r1, sl
	str r1, [r3, #0]
	bl Event_StartScreenTransition
	bl Event_WaitTransitionFrames
	ldr r3, .L_it_020086a4
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #2
	bne .L_it_020083ee
.L_it_0200836a:
	ldr r5, .L_it_020086a8
	movs r1, #5
	adds r0, r5, #0
	bl UiText_ShowPositionedMessageAndWait
	movs r0, #1
	bl Func_020023d8
	adds r6, r0, #0
	bl UiWork_FinalizePendingCore
	cmp r6, #0
	bne .L_it_020083cc
	movs r1, #1
	adds r0, r5, #1
	bl UiText_ShowPositionedMessageAndWait
	ldr r3, .L_it_020086a4
	movs r2, #152
	lsls r2, r2, #1
	adds r2, #255
	adds r3, r3, r2
	movs r2, #1
	strb r2, [r3, #0]
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
	beq .L_it_0200836a
.L_it_020083cc:
	movs r0, #60
	bl FieldEvent_WaitFrames
	bl Event_FinishScreenTransition
	movs r0, #78
	bl Func_02002408
	movs r0, #150
	lsls r0, r0, #1
	bl FieldEvent_WaitFrames
	ldr r0, .L_it_020086ac
	movs r1, #1
	bl Func_020023b8
	b .L_it_02008a24
.L_it_020083ee:
	ldr r3, .L_it_020086b0
	mov r1, sl
	strb r1, [r3, #0]
.L_it_020083f4:
	bl Func_020022f0
	adds r5, r0, #0
	cmp r5, #0
	bge .L_it_02008422
	ldr r3, .L_it_020086b4
	ldrb r3, [r3, #0]
	cmp r3, #0
	beq .L_it_02008422
	ldr r3, .L_it_020086a4
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #74
	adds r3, r3, r2
	movs r2, #1
	strb r2, [r3, #0]
	ldr r3, .L_it_020086b8
	ldr r0, .L_it_020086bc
	strb r2, [r3, #0]
	movs r1, #1
	movs r2, #8
	bl Func_020022b8
.L_it_02008422:
	cmp r5, #0
	bne .L_it_02008446
	mov r3, sl
	cmp r3, #0
	bne .L_it_02008446
	movs r0, #30
	bl WaitFrames
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_it_020086c0
	bl Task_Add
	movs r0, #1
	bl WaitFrames
	movs r1, #1
	mov sl, r1
.L_it_02008446:
	cmp r5, #0
	ble .L_it_02008452
	bl Menu_SelectSaveSlotAction
	adds r5, r0, #0
	b .L_it_02008454
.L_it_02008452:
	movs r5, #0
.L_it_02008454:
	cmp r5, #0
	beq .L_it_0200845a
	b .L_it_020086f4
.L_it_0200845a:
	bl Func_02002350
	ldr r3, .L_it_020086a4
	movs r1, #147
	lsls r1, r1, #1
	adds r1, #255
	adds r2, r3, r1
	ldrb r0, [r2, #0]
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #38
	adds r3, r3, r2
	ldrb r1, [r3, #0]
	bl Func_02002310
	movs r6, #4
	movs r7, #5
.L_it_0200847c:
	movs r0, #6
	bl WaitFrames
	adds r0, r6, #0
	bl Func_020022f8
	movs r3, #1
	adds r5, r0, #0
	negs r3, r3
	cmp r5, r3
	bne .L_it_0200849a
	cmp r6, #4
	beq .L_it_020083f4
	subs r6, #1
	b .L_it_0200847c
.L_it_0200849a:
	ldr r3, .L_it_020086c4
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	bne .L_it_020084ae
	ldr r3, .L_it_020086c8
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	beq .L_it_020084b0
.L_it_020084ae:
	movs r7, #8
.L_it_020084b0:
	adds r6, #1
	cmp r6, r7
	blt .L_it_0200847c
.L_it_020084b6:
	ldr r0, .L_it_020086cc
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
	bne .L_it_0200856a
.L_it_020084d4:
	movs r0, #0
	bl Func_020022b0
	movs r3, #1
	negs r3, r3
	adds r5, r0, #0
	mov r8, r3
	cmp r5, r8
	beq .L_it_020083f4
	cmp r5, #1
	bne .L_it_0200852a
	ldr r5, .L_it_020086d0
	movs r2, #192
	adds r1, r5, #0
	lsls r2, r2, #6
	ldr r6, .L_it_020086d4
	ldr r0, .L_it_020086d8
	mov lr, r6
	.2byte 0xf800
	bl Func_02000b24
	adds r7, r0, #0
	cmp r7, r8
	beq .L_it_02008508
	bl Func_020023e8
.L_it_02008508:
	movs r2, #192
	adds r1, r5, #0
	lsls r2, r2, #6
	ldr r0, .L_it_020086dc
	mov lr, r6
	.2byte 0xf800
	movs r2, #192
	adds r0, r5, #0
	ldr r1, .L_it_020086d8
	lsls r2, r2, #6
	mov lr, r6
	.2byte 0xf800
	cmp r7, r8
	beq .L_it_020084d4
	ldr r0, .L_it_020086dc
	movs r1, #0
	b .L_it_02008562
.L_it_0200852a:
	cmp r5, #0
	bne .L_it_0200856e
	ldr r5, .L_it_020086d0
	movs r2, #192
	adds r1, r5, #0
	lsls r2, r2, #6
	ldr r6, .L_it_020086d4
	ldr r0, .L_it_020086d8
	mov lr, r6
	.2byte 0xf800
	bl Func_02002350
	bl Func_02000e44
	movs r2, #192
	adds r1, r5, #0
	lsls r2, r2, #6
	ldr r0, .L_it_020086dc
	mov lr, r6
	.2byte 0xf800
	movs r2, #192
	ldr r1, .L_it_020086d8
	lsls r2, r2, #6
	adds r0, r5, #0
	mov lr, r6
	.2byte 0xf800
	ldr r0, .L_it_020086dc
	movs r1, #1
.L_it_02008562:
	movs r2, #0
	bl Func_02002370
	b .L_it_0200856e
.L_it_0200856a:
	bl Func_02002398
.L_it_0200856e:
	movs r6, #0
	movs r7, #1
.L_it_02008572:
	movs r0, #6
	bl WaitFrames
	adds r0, r6, #0
	bl Func_020022f8
	movs r1, #1
	adds r5, r0, #0
	negs r1, r1
	cmp r5, r1
	bne .L_it_02008590
	cmp r6, #0
	beq .L_it_020084b6
	subs r6, #1
	b .L_it_02008572
.L_it_02008590:
	ldr r3, .L_it_020086c8
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	beq .L_it_0200859c
	movs r7, #4
.L_it_0200859c:
	adds r6, #1
	cmp r6, r7
	blt .L_it_02008572
	ldr r3, .L_it_020086e0
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	beq .L_it_02008696
.L_it_020085ac:
	ldr r5, .L_it_020086e4
	movs r1, #13
	adds r0, r5, #0
	bl UiText_ShowPositionedMessageAndWait
	ldr r1, .L_it_020086e8
	movs r0, #1
	movs r2, #8
	movs r3, #0
	bl Menu_RunConfirmSelection
	adds r6, r0, #0
	bl UiWork_FinalizePendingCore
	cmp r6, #0
	bne .L_it_02008642
	ldr r3, .L_it_020086e0
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	bne .L_it_020085e0
	adds r0, r5, #1
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
	b .L_it_02008642
.L_it_020085e0:
	ldr r6, .L_it_020086d0
	movs r2, #192
	adds r1, r6, #0
	lsls r2, r2, #6
	ldr r7, .L_it_020086d4
	ldr r0, .L_it_020086d8
	mov lr, r7
	.2byte 0xf800
	movs r0, #5
	bl Func_020022d8
	adds r5, r0, #0
	cmp r5, #0
	bne .L_it_020085ac
	ldr r5, .L_it_020086a4
	movs r1, #147
	lsls r1, r1, #1
	movs r2, #128
	adds r1, #255
	lsls r2, r2, #2
	adds r3, r5, r1
	adds r2, #38
	ldrb r0, [r3, #0]
	adds r3, r5, r2
	ldrb r1, [r3, #0]
	bl Func_02002310
	movs r2, #192
	adds r1, r6, #0
	lsls r2, r2, #6
	ldr r0, .L_it_020086dc
	mov lr, r7
	.2byte 0xf800
	movs r2, #192
	ldr r1, .L_it_020086d8
	lsls r2, r2, #6
	adds r0, r6, #0
	mov lr, r7
	.2byte 0xf800
	ldr r0, .L_it_020086dc
	bl Func_020023a0
	movs r3, #196
	lsls r3, r3, #1
	adds r3, #255
	adds r5, r5, r3
	movs r3, #1
	strb r3, [r5, #0]
	b .L_it_02008696
.L_it_02008642:
	ldr r0, .L_it_020086ec
	movs r1, #13
	bl UiText_ShowPositionedMessageAndWait
	ldr r5, .L_it_020086e8
	movs r0, #1
	adds r1, r5, #0
	movs r2, #8
	movs r3, #0
	bl Menu_RunConfirmSelection
	adds r6, r0, #0
	bl UiWork_FinalizePendingCore
	cmp r6, #0
	bne .L_it_02008678
	movs r0, #46
	bl GameFlag_SetBit
	ldr r3, .L_it_020086a4
	movs r1, #196
	lsls r1, r1, #1
	adds r1, #255
	adds r3, r3, r1
	movs r2, #2
	strb r2, [r3, #0]
	b .L_it_02008696
.L_it_02008678:
	ldr r0, .L_it_020086f0
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
	bne .L_it_020085ac
.L_it_02008696:
	ldr r0, .L_it_020086ac
	movs r1, #8
	bl Func_020023b8
	b .L_it_020089f6
.L_it_020086a0:
	.4byte Func_020001f4
.L_it_020086a4:
	.4byte gAdventure
.L_it_020086a8:
	.4byte 0x00000008
.L_it_020086ac:
	.4byte 0x00000000
.L_it_020086b0:
	.4byte gSleepDisabled
.L_it_020086b4:
	.4byte gDebugMode
.L_it_020086b8:
	.4byte gAutoSleepEnabled
.L_it_020086bc:
	.4byte 0x0000000b
.L_it_020086c0:
	.4byte Func_0200015c
.L_it_020086c4:
	.4byte gData_020024f6
.L_it_020086c8:
	.4byte gData_020024f8
.L_it_020086cc:
	.4byte 0x00000024
.L_it_020086d0:
	.4byte gData_02000000
.L_it_020086d4:
	.4byte IwramCopyWords
.L_it_020086d8:
	.4byte gMapCellBuffer
.L_it_020086dc:
	.4byte 0x02013000
.L_it_020086e0:
	.4byte gData_02003860
.L_it_020086e4:
	.4byte 0x00000025
.L_it_020086e8:
	.4byte 0x00000029
.L_it_020086ec:
	.4byte 0x00000027
.L_it_020086f0:
	.4byte 0x00000028
.L_it_020086f4:
	cmp r5, #1
	beq .L_it_020086fa
	b .L_it_02008848
.L_it_020086fa:
	movs r0, #1
	bl Func_020022d8
	movs r2, #1
	adds r5, r0, #0
	negs r2, r2
	cmp r5, r2
	bne .L_it_0200870c
	b .L_it_020083f4
.L_it_0200870c:
	movs r0, #10
	adds r0, #255
	bl GameFlag_SetBit
	ldr r5, .L_it_02008a30
	movs r1, #147
	lsls r1, r1, #1
	movs r2, #128
	adds r1, #255
	lsls r2, r2, #2
	adds r3, r5, r1
	adds r2, #38
	ldrb r0, [r3, #0]
	adds r3, r5, r2
	ldrb r1, [r3, #0]
	bl Func_02002310
	bl Func_02002388
	ldr r3, [r5, #0]
	cmp r3, r0
	beq .L_it_0200876a
	movs r1, #242
	lsls r1, r1, #1
	adds r3, r5, r1
	ldrh r0, [r3, #0]
	movs r2, #240
	lsls r2, r2, #1
	adds r3, r5, r2
	strh r0, [r3, #0]
	adds r1, #2
	adds r3, r5, r1
	ldrh r1, [r3, #0]
	adds r2, #2
	adds r3, r5, r2
	strh r1, [r3, #0]
	lsls r0, r0, #16
	lsls r1, r1, #16
	asrs r0, r0, #16
	asrs r1, r1, #16
	bl Func_020023e0
	movs r0, #10
	adds r0, #255
	bl GameFlag_ClearBit
	b .L_it_0200883e
.L_it_0200876a:
	ldr r3, .L_it_02008a34
	movs r2, #130
	ldr r3, [r3, #0]
	lsls r2, r2, #2
	ands r3, r2
	cmp r3, r2
	bne .L_it_020087be
	bl Func_020002e0
	cmp r0, #0
	beq .L_it_02008784
	ldr r0, .L_it_02008a38
	b .L_it_020087fe
.L_it_02008784:
	movs r1, #242
	lsls r1, r1, #1
	adds r3, r5, r1
	ldrh r0, [r3, #0]
	movs r2, #240
	lsls r2, r2, #1
	adds r3, r5, r2
	strh r0, [r3, #0]
	adds r1, #2
	adds r3, r5, r1
	ldrh r1, [r3, #0]
	adds r2, #2
	adds r3, r5, r2
	strh r1, [r3, #0]
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
	b .L_it_0200883e
.L_it_020087be:
	ldr r3, .L_it_02008a3c
	ldr r2, [r5, #4]
	ldr r3, [r3, #0]
	cmp r2, r3
	beq .L_it_0200883e
	ldr r6, .L_it_02008a40
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
	beq .L_it_020087f0
	bl UiWork_FinalizePendingCore
	b .L_it_020083f4
.L_it_020087f0:
	bl UiWork_FinalizePendingCore
	bl Func_020002e0
	cmp r0, #0
	beq .L_it_02008806
	adds r0, r6, #2
.L_it_020087fe:
	movs r1, #9
	bl UiText_ShowPositionedMessageAndWait
	b .L_it_020083f4
.L_it_02008806:
	movs r1, #242
	lsls r1, r1, #1
	adds r3, r5, r1
	ldrh r0, [r3, #0]
	movs r2, #240
	lsls r2, r2, #1
	adds r3, r5, r2
	strh r0, [r3, #0]
	adds r1, #2
	adds r3, r5, r1
	ldrh r1, [r3, #0]
	adds r2, #2
	adds r3, r5, r2
	strh r1, [r3, #0]
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
.L_it_0200883e:
	movs r0, #131
	lsls r0, r0, #1
	bl GameFlag_ClearBit
	b .L_it_020089f6
.L_it_02008848:
	cmp r5, #2
	bne .L_it_02008852
	bl Func_020022e0
	b .L_it_020083f4
.L_it_02008852:
	cmp r5, #3
	bne .L_it_0200885c
	bl Func_020022e8
	b .L_it_020083f4
.L_it_0200885c:
	cmp r5, #4
	bne .L_it_020088d4
	movs r0, #4
	bl Func_020022d8
	movs r3, #1
	adds r5, r0, #0
	negs r3, r3
	cmp r5, r3
	bne .L_it_02008872
	b .L_it_020083f4
.L_it_02008872:
	ldr r5, .L_it_02008a30
	movs r1, #133
	lsls r1, r1, #2
	movs r0, #144
	adds r3, r5, r1
	movs r2, #4
	lsls r0, r0, #4
	str r2, [r3, #0]
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_it_02008890
	bl Func_02002390
.L_it_02008890:
	movs r2, #147
	lsls r2, r2, #1
	movs r1, #128
	adds r2, #255
	lsls r1, r1, #2
	adds r3, r5, r2
	adds r1, #38
	ldrb r0, [r3, #0]
	adds r3, r5, r1
	ldrb r1, [r3, #0]
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
	strh r3, [r2, #0]
	ldr r2, .L_it_02008a44
	movs r3, #1
	strb r3, [r2, #0]
	ldr r0, .L_it_02008a48
	movs r1, #1
	bl Func_020023b8
	b .L_it_020089f6
.L_it_020088d4:
	cmp r5, #5
	beq .L_it_020088da
	b .L_it_020083f4
.L_it_020088da:
	movs r0, #6
	bl Func_020022d8
	movs r1, #1
	adds r5, r0, #0
	negs r1, r1
	cmp r5, r1
	bne .L_it_020088ec
	b .L_it_020083f4
.L_it_020088ec:
	ldr r3, .L_it_02008a30
	movs r1, #147
	lsls r1, r1, #1
	adds r1, #255
	adds r2, r3, r1
	ldrb r0, [r2, #0]
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #38
	adds r3, r3, r2
	ldrb r1, [r3, #0]
	bl Func_02002310
.L_it_02008906:
	movs r0, #2
	bl Func_020022b0
	movs r3, #1
	negs r3, r3
	adds r5, r0, #0
	mov r8, r3
	cmp r5, r8
	beq .L_it_020088da
	cmp r5, #1
	bne .L_it_02008962
	ldr r5, .L_it_02008a4c
	movs r2, #192
	adds r1, r5, #0
	lsls r2, r2, #6
	ldr r6, .L_it_02008a50
	ldr r0, .L_it_02008a54
	mov lr, r6
	.2byte 0xf800
	bl Func_02000b24
	adds r7, r0, #0
	cmp r7, r8
	beq .L_it_0200893a
	bl Func_020023e8
.L_it_0200893a:
	movs r2, #192
	adds r1, r5, #0
	lsls r2, r2, #6
	ldr r0, .L_it_02008a58
	mov lr, r6
	.2byte 0xf800
	movs r2, #192
	adds r0, r5, #0
	ldr r1, .L_it_02008a54
	lsls r2, r2, #6
	mov lr, r6
	.2byte 0xf800
	cmp r7, r8
	beq .L_it_02008906
	ldr r0, .L_it_02008a58
	movs r1, #0
	movs r2, #1
	bl Func_02002370
	b .L_it_020089a6
.L_it_02008962:
	cmp r5, #0
	bne .L_it_020089a6
	ldr r5, .L_it_02008a4c
	movs r2, #192
	adds r1, r5, #0
	lsls r2, r2, #6
	ldr r6, .L_it_02008a50
	ldr r0, .L_it_02008a54
	mov lr, r6
	.2byte 0xf800
	bl Func_02002350
	bl Func_02000e44
	movs r2, #192
	adds r7, r0, #0
	adds r1, r5, #0
	lsls r2, r2, #6
	ldr r0, .L_it_02008a58
	mov lr, r6
	.2byte 0xf800
	movs r2, #192
	adds r0, r5, #0
	ldr r1, .L_it_02008a54
	lsls r2, r2, #6
	mov lr, r6
	.2byte 0xf800
	cmp r7, r8
	beq .L_it_02008906
	ldr r0, .L_it_02008a58
	movs r1, #1
	movs r2, #1
	bl Func_02002370
.L_it_020089a6:
	ldr r3, .L_it_02008a5c
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	bne .L_it_020089ba
	ldr r3, .L_it_02008a60
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	beq .L_it_020089ee
.L_it_020089ba:
	movs r6, #0
	movs r7, #1
.L_it_020089be:
	movs r0, #6
	bl WaitFrames
	adds r0, r6, #0
	bl Func_020022f8
	movs r3, #1
	adds r5, r0, #0
	negs r3, r3
	cmp r5, r3
	bne .L_it_020089dc
	cmp r6, #0
	beq .L_it_020088da
	subs r6, #1
	b .L_it_020089be
.L_it_020089dc:
	ldr r3, .L_it_02008a60
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	beq .L_it_020089e8
	movs r7, #4
.L_it_020089e8:
	adds r6, #1
	cmp r6, r7
	blt .L_it_020089be
.L_it_020089ee:
	movs r0, #10
	adds r0, #255
	bl GameFlag_SetBit
.L_it_020089f6:
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #172
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #186
	lsls r2, r2, #2
	adds r2, #255
	strh r2, [r3, #0]
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r0, #78
	bl Func_02002408
	bl Event_FinishScreenTransition
	bl Event_WaitTransitionFrames
	movs r0, #60
	bl FieldEvent_WaitFrames
.L_it_02008a24:
	movs r0, #0
	pop {r3, r5}
	mov r8, r3
	mov sl, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_it_02008a30:
	.4byte gAdventure
.L_it_02008a34:
	.4byte gInput
.L_it_02008a38:
	.4byte 0x00000007
.L_it_02008a3c:
	.4byte gData_02001000
.L_it_02008a40:
	.4byte 0x00000005
.L_it_02008a44:
	.4byte gSleepDisabled
.L_it_02008a48:
	.4byte 0x00000138
.L_it_02008a4c:
	.4byte gData_02000000
.L_it_02008a50:
	.4byte IwramCopyWords
.L_it_02008a54:
	.4byte gMapCellBuffer
.L_it_02008a58:
	.4byte 0x02013000
.L_it_02008a5c:
	.4byte gData_020024f6
.L_it_02008a60:
	.4byte gData_020024f8
	.else
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
	bl Task_Add
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	mov r1, r10
	str r1, [r3]
	bl Event_StartScreenTransition
	bl Event_WaitTransitionFrames
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
	bl FieldEvent_WaitFrames
	bl Event_FinishScreenTransition
	movs r0, #78
	bl Func_02002408
	movs r0, #150
	lsls r0, r0, #1
	bl FieldEvent_WaitFrames
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
	bl Task_Add
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
	.4byte gAdventure
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
	.4byte gData_020024f6
.L_020086d0:
	.4byte gData_020024f8
.L_020086d4:
	.4byte 0x00000024
.L_020086d8:
	.4byte gData_02000000
.L_020086dc:
	.4byte IwramCopyWords
.L_020086e0:
	.4byte gMapCellBuffer
.L_020086e4:
	.4byte Data_02012f94 + 0x6c
.L_020086e8:
	.4byte gData_02003860
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
	bl FieldEvent_WaitFrames
	movs r0, #78
	bl Func_02002408
	bl Event_FinishScreenTransition
	bl Event_WaitTransitionFrames
	movs r0, #60
	bl FieldEvent_WaitFrames
.L_02008a2c:
	movs r0, #0
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008a38:
	.4byte gAdventure
.L_02008a3c:
	.4byte gInput
.L_02008a40:
	.4byte 0x00000007
.L_02008a44:
	.4byte gData_02001000
.L_02008a48:
	.4byte 0x00000005
.L_02008a4c:
	.4byte gSleepDisabled
.L_02008a50:
	.4byte 0x00000138
.L_02008a54:
	.4byte gData_02000000
.L_02008a58:
	.4byte IwramCopyWords
.L_02008a5c:
	.4byte gMapCellBuffer
.L_02008a60:
	.4byte Data_02012f94 + 0x6c
.L_02008a64:
	.4byte gData_020024f6
.L_02008a68:
	.4byte gData_020024f8
	.endif
	.endif
	.endif
	.endif
	.endif
	.section .text.x02008b24,"ax",%progbits
	.ifdef TLA_EDITION_FR
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
	movs r2, #26
	movs r3, #8
	movs r0, #2
	bl UiWindow_Create
	ldr r5, .L_fr_02008be8
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
.L_fr_02008b6c:
	movs r5, #3
	b .L_fr_02008bc4
.L_fr_02008b70:
	ldr r3, .L_fr_02008bec
	ldrh r2, [r3]
	adds r3, r5, #0
	ands r3, r2
	cmp r3, r5
	bne .L_fr_02008bbe
	ldr r3, .L_fr_02008bf0
	movs r2, #1
	ldr r3, [r3]
	lsls r3, r3, #26
	lsrs r3, r3, #30
	eors r3, r2
	lsls r2, r3, #1
	adds r2, r2, r3
	ldr r3, .L_fr_02008bf4
	lsls r2, r2, #3
	adds r2, r2, r3
	ldrh r3, [r2]
	cmp r3, #48
	bne .L_fr_02008bbe
	ldrh r3, [r2, #2]
	cmp r3, #48
	bne .L_fr_02008bbe
	ldrh r3, [r2, #4]
	cmp r3, #48
	bne .L_fr_02008bbe
	ldrh r3, [r2, #6]
	cmp r3, #48
	bne .L_fr_02008bbe
	ldr r2, .L_fr_02008bf8
	ldr r3, .L_fr_02008bd8
	strh r3, [r2]
	ldr r3, .L_fr_02008bdc
	strh r3, [r2, #2]
	ldr r3, .L_fr_02008be0
	strh r3, [r2, #4]
	ldr r3, .L_fr_02008be4
	strh r3, [r2, #6]
	b .L_fr_02008c00
.L_fr_02008bbe:
	movs r0, #1
	bl WaitFrames
.L_fr_02008bc4:
	ldr r3, .L_fr_02008bfc
	movs r2, #2
	ldr r3, [r3, #4]
	ands r3, r2
	cmp r3, #0
	beq .L_fr_02008b70
	movs r0, #113
	bl Func_02002408
	b .L_fr_02008ce0
.L_fr_02008bd8:
	.4byte 0x00000046
.L_fr_02008bdc:
	.4byte 0x00000056
.L_fr_02008be0:
	.4byte 0x00000054
.L_fr_02008be4:
	.4byte 0x00000053
.L_fr_02008be8:
	.4byte 0x00001350
.L_fr_02008bec:
	.4byte gLinkStatus
.L_fr_02008bf0:
	.4byte 0x04000128
.L_fr_02008bf4:
	.4byte gLinkPeerSignatures
.L_fr_02008bf8:
	.4byte gData_02003a74
.L_fr_02008bfc:
	.4byte gInput
.L_fr_02008c00:
	movs r0, #10
	bl WaitFrames
	ldr r0, .L_fr_02008d74
	bl Serial_StartReceive
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
.L_fr_02008c22:
	mov r1, r8
	cmp r1, #0
	bne .L_fr_02008c5a
	ldr r3, .L_fr_02008d78
	ldrh r3, [r3]
	cmp r3, #0
	beq .L_fr_02008c5a
	movs r2, #1
	adds r0, r6, #0
	movs r1, #2
	mov r8, r2
	bl UiWork_Finalize
	movs r3, #2
	str r3, [sp, #0]
	movs r1, #10
	movs r2, #26
	movs r3, #4
	movs r0, #2
	bl UiWindow_Create
	adds r6, r0, #0
	adds r1, r6, #0
	ldr r0, .L_fr_02008d7c
	movs r2, #0
	movs r3, #4
	bl UiText_DrawResource
.L_fr_02008c5a:
	cmp r5, #9
	bne .L_fr_02008c86
	ldr r3, .L_fr_02008d78
	ldrh r3, [r3]
	cmp r3, #0
	bne .L_fr_02008c86
	ldr r2, .L_fr_02008d80
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
	b .L_fr_02008b6c
.L_fr_02008c86:
	ldr r3, .L_fr_02008d84
	adds r7, #1
	ldrh r2, [r3]
	mov r3, r10
	ands r3, r2
	cmp r3, r10
	bne .L_fr_02008c96
	movs r7, #0
.L_fr_02008c96:
	cmp r7, #10
	bne .L_fr_02008ca0
	movs r3, #0
	mov r9, r3
	b .L_fr_02008cb6
.L_fr_02008ca0:
	ldr r3, .L_fr_02008d88
	ldr r3, [r3]
	cmp r3, #0
	beq .L_fr_02008cb6
	movs r0, #1
	bl WaitFrames
	ldr r1, .L_fr_02008d8c
	adds r5, #1
	cmp r5, r1
	ble .L_fr_02008c22
.L_fr_02008cb6:
	mov r2, r9
	cmp r2, #0
	bne .L_fr_02008cee
	adds r0, r6, #0
	bl RenderOutput_PrepareForRedraw
	ldr r0, .L_fr_02008d90
	adds r1, r6, #0
	movs r2, #0
	movs r3, #4
	bl UiText_DrawResource
.L_fr_02008cce:
	movs r0, #1
	bl WaitFrames
	ldr r3, .L_fr_02008d94
	movs r2, #1
	ldr r3, [r3, #4]
	ands r3, r2
	cmp r3, #0
	beq .L_fr_02008cce
.L_fr_02008ce0:
	adds r0, r6, #0
	movs r1, #2
	bl UiWork_Finalize
	movs r0, #1
	negs r0, r0
	b .L_fr_02008d66
.L_fr_02008cee:
	movs r0, #10
	bl WaitFrames
	bl Func_02002228
	movs r0, #10
	bl WaitFrames
	adds r0, r6, #0
	movs r1, #2
	bl UiWork_Finalize
	ldr r5, .L_fr_02008d98
	movs r2, #133
	ldr r6, .L_fr_02008d9c
	ldr r1, .L_fr_02008da0
	lsls r2, r2, #2
	adds r0, r5, #0
	mov lr, r6
	.2byte 0xf800
	movs r3, #136
	lsls r3, r3, #2
	movs r2, #210
	adds r0, r5, r3
	ldr r1, .L_fr_02008da4
	lsls r2, r2, #4
	mov lr, r6
	.2byte 0xf800
	movs r0, #0
	bl Unit_Calculate
	movs r0, #1
	bl Unit_Calculate
	movs r0, #2
	bl Unit_Calculate
	movs r0, #3
	bl Unit_Calculate
	movs r2, #244
	adds r1, r5, #0
	lsls r2, r2, #4
	ldr r0, .L_fr_02008da0
	mov lr, r6
	.2byte 0xf800
	ldr r3, .L_fr_02008da8
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
.L_fr_02008d66:
	sub sp, #-4
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_fr_02008d74:
	.4byte 0x02013000
.L_fr_02008d78:
	.4byte gSerialReceivedSize
.L_fr_02008d7c:
	.4byte 0x00001352
.L_fr_02008d80:
	.4byte gData_02003a74
.L_fr_02008d84:
	.4byte gLinkStatus
.L_fr_02008d88:
	.4byte gSerialReceiveDest
.L_fr_02008d8c:
	.4byte 0x000927bf
.L_fr_02008d90:
	.4byte 0x00001354
.L_fr_02008d94:
	.4byte gInput
.L_fr_02008d98:
	.4byte gGameFlagBytes
.L_fr_02008d9c:
	.4byte IwramCopyWords
.L_fr_02008da0:
	.4byte 0x02013040
.L_fr_02008da4:
	.4byte 0x02013240
.L_fr_02008da8:
	.4byte gAdventure
	.else
	.global Func_02000b24
	.thumb_func
Func_02000b24:
	.ifdef TLA_EDITION_ES
	push {r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #4
	movs r3, #2
	str r3, [sp, #0]
	movs r1, #5
	movs r2, #20
	movs r3, #8
	movs r0, #5
	bl UiWindow_Create
	ldr r5, .L_es_02008be4
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
.L_es_02008b6c:
	movs r5, #3
	b .L_es_02008bc2
.L_es_02008b70:
	ldr r3, .L_es_02008be8
	ldrh r2, [r3, #0]
	adds r3, r5, #0
	ands r3, r2
	cmp r3, r5
	bne .L_es_02008bbc
	ldr r3, .L_es_02008bec
	movs r2, #1
	ldr r3, [r3, #0]
	lsls r3, r3, #26
	lsrs r3, r3, #30
	eors r3, r2
	lsls r2, r3, #1
	adds r2, r2, r3
	ldr r3, .L_es_02008bf0
	lsls r2, r2, #3
	adds r2, r2, r3
	ldrh r3, [r2, #0]
	cmp r3, #48
	bne .L_es_02008bbc
	ldrh r3, [r2, #2]
	cmp r3, #48
	bne .L_es_02008bbc
	ldrh r3, [r2, #4]
	cmp r3, #48
	bne .L_es_02008bbc
	ldrh r3, [r2, #6]
	cmp r3, #48
	bne .L_es_02008bbc
	ldr r1, .L_es_02008bf4
	ldr r2, .L_es_02008bd8
	ldr r3, .L_es_02008bdc
	strh r2, [r1, #2]
	ldr r2, .L_es_02008be0
	strh r3, [r1, #0]
	strh r2, [r1, #4]
	strh r3, [r1, #6]
	b .L_es_02008bfc
.L_es_02008bbc:
	movs r0, #1
	bl WaitFrames
.L_es_02008bc2:
	ldr r3, .L_es_02008bf8
	movs r2, #2
	ldr r3, [r3, #4]
	ands r3, r2
	cmp r3, #0
	beq .L_es_02008b70
	movs r0, #113
	bl Func_02002408
	b .L_es_02008cdc
	.2byte 0x0000
.L_es_02008bd8:
	.4byte 0x00000056
.L_es_02008bdc:
	.4byte 0x00000053
.L_es_02008be0:
	.4byte 0x00000054
.L_es_02008be4:
	.4byte 0x00001350
.L_es_02008be8:
	.4byte gLinkStatus
.L_es_02008bec:
	.4byte 0x04000128
.L_es_02008bf0:
	.4byte gLinkPeerSignatures
.L_es_02008bf4:
	.4byte gData_02003a74
.L_es_02008bf8:
	.4byte gInput
.L_es_02008bfc:
	movs r0, #10
	bl WaitFrames
	ldr r0, .L_es_02008d70
	bl Serial_StartReceive
	movs r0, #10
	bl WaitFrames
	movs r1, #3
	movs r2, #1
	movs r3, #0
	mov sl, r1
	movs r7, #0
	mov r9, r2
	mov r8, r3
	movs r5, #0
.L_es_02008c1e:
	mov r1, r8
	cmp r1, #0
	bne .L_es_02008c56
	ldr r3, .L_es_02008d74
	ldrh r3, [r3, #0]
	cmp r3, #0
	beq .L_es_02008c56
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
	ldr r0, .L_es_02008d78
	movs r2, #0
	movs r3, #4
	bl UiText_DrawResource
.L_es_02008c56:
	cmp r5, #9
	bne .L_es_02008c82
	ldr r3, .L_es_02008d74
	ldrh r3, [r3, #0]
	cmp r3, #0
	bne .L_es_02008c82
	ldr r1, .L_es_02008d7c
	strh r3, [r1, #0]
	strh r3, [r1, #2]
	strh r3, [r1, #4]
	strh r3, [r1, #6]
	bl Func_02002228
	movs r0, #10
	bl WaitFrames
	bl Func_02002220
	movs r0, #10
	bl WaitFrames
	b .L_es_02008b6c
.L_es_02008c82:
	ldr r3, .L_es_02008d80
	adds r7, #1
	ldrh r2, [r3, #0]
	mov r3, sl
	ands r3, r2
	cmp r3, sl
	bne .L_es_02008c92
	movs r7, #0
.L_es_02008c92:
	cmp r7, #10
	bne .L_es_02008c9c
	movs r3, #0
	mov r9, r3
	b .L_es_02008cb2
.L_es_02008c9c:
	ldr r3, .L_es_02008d84
	ldr r3, [r3, #0]
	cmp r3, #0
	beq .L_es_02008cb2
	movs r0, #1
	bl WaitFrames
	ldr r1, .L_es_02008d88
	adds r5, #1
	cmp r5, r1
	ble .L_es_02008c1e
.L_es_02008cb2:
	mov r2, r9
	cmp r2, #0
	bne .L_es_02008cea
	adds r0, r6, #0
	bl RenderOutput_PrepareForRedraw
	ldr r0, .L_es_02008d8c
	adds r1, r6, #0
	movs r2, #0
	movs r3, #4
	bl UiText_DrawResource
.L_es_02008cca:
	movs r0, #1
	bl WaitFrames
	ldr r3, .L_es_02008d90
	movs r2, #1
	ldr r3, [r3, #4]
	ands r3, r2
	cmp r3, #0
	beq .L_es_02008cca
.L_es_02008cdc:
	adds r0, r6, #0
	movs r1, #2
	bl UiWork_Finalize
	movs r0, #1
	negs r0, r0
	b .L_es_02008d62
.L_es_02008cea:
	movs r0, #10
	bl WaitFrames
	bl Func_02002228
	movs r0, #10
	bl WaitFrames
	adds r0, r6, #0
	movs r1, #2
	bl UiWork_Finalize
	ldr r5, .L_es_02008d94
	movs r2, #133
	ldr r6, .L_es_02008d98
	ldr r1, .L_es_02008d9c
	lsls r2, r2, #2
	adds r0, r5, #0
	mov lr, r6
	.2byte 0xf800
	movs r3, #136
	lsls r3, r3, #2
	movs r2, #210
	adds r0, r5, r3
	ldr r1, .L_es_02008da0
	lsls r2, r2, #4
	mov lr, r6
	.2byte 0xf800
	movs r0, #0
	bl Unit_Calculate
	movs r0, #1
	bl Unit_Calculate
	movs r0, #2
	bl Unit_Calculate
	movs r0, #3
	bl Unit_Calculate
	movs r2, #244
	adds r1, r5, #0
	lsls r2, r2, #4
	ldr r0, .L_es_02008d9c
	mov lr, r6
	.2byte 0xf800
	ldr r3, .L_es_02008da4
	movs r1, #147
	lsls r1, r1, #1
	adds r1, #255
	adds r2, r3, r1
	ldrb r0, [r2, #0]
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #38
	adds r3, r3, r2
	ldrb r1, [r3, #0]
	bl Func_02002310
	movs r0, #1
.L_es_02008d62:
	add sp, #4
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov sl, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_es_02008d70:
	.4byte 0x02013000
.L_es_02008d74:
	.4byte gSerialReceivedSize
.L_es_02008d78:
	.4byte 0x00001352
.L_es_02008d7c:
	.4byte gData_02003a74
.L_es_02008d80:
	.4byte gLinkStatus
.L_es_02008d84:
	.4byte gSerialReceiveDest
.L_es_02008d88:
	.4byte 0x000927bf
.L_es_02008d8c:
	.4byte 0x00001354
.L_es_02008d90:
	.4byte gInput
.L_es_02008d94:
	.4byte gGameFlagBytes
.L_es_02008d98:
	.4byte IwramCopyWords
.L_es_02008d9c:
	.4byte 0x02013040
.L_es_02008da0:
	.4byte 0x02013240
.L_es_02008da4:
	.4byte gAdventure
	.else
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #4
	movs r3, #2
	str r3, [sp, #0]
	movs r1, #5
	.ifdef TLA_EDITION_DE
	movs r2, #20
	.else
	movs r2, #18
	.endif
	movs r3, #8
	.ifdef TLA_EDITION_DE
	movs r0, #5
	.else
	movs r0, #6
	.endif
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
	.4byte MsgFieldRepelEvil
.L_02008be4:
	.4byte 0x00000056
.L_02008be8:
	.4byte 0x00000054
.L_02008bec:
	.4byte 0x00000053
.L_02008bf0:
	.4byte MsgFieldPreparingToReceive
.L_02008bf4:
	.4byte gLinkStatus
.L_02008bf8:
	.4byte 0x04000128
.L_02008bfc:
	.4byte gLinkPeerSignatures
.L_02008c00:
	.4byte gData_02003a74
.L_02008c04:
	.4byte gInput
.L_02008c08:
	movs r0, #10
	bl WaitFrames
	ldr r0, .L_02008d7c
	bl Serial_StartReceive
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
	bl Unit_Calculate
	movs r0, #1
	bl Unit_Calculate
	movs r0, #2
	bl Unit_Calculate
	movs r0, #3
	bl Unit_Calculate
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
	.4byte MsgFieldTransferringData
.L_02008d88:
	.4byte gData_02003a74
.L_02008d8c:
	.4byte gLinkStatus
.L_02008d90:
	.4byte gSerialReceiveDest
.L_02008d94:
	.4byte 0x000927bf
.L_02008d98:
	.4byte MsgFieldDataTransferWasInterrupted
.L_02008d9c:
	.4byte gInput
.L_02008da0:
	.4byte gGameFlagBytes
.L_02008da4:
	.4byte IwramCopyWords
.L_02008da8:
	.4byte Data_02012f94 + 0xac
.L_02008dac:
	.4byte Data_02012f94 + 0x2ac
.L_02008db0:
	.4byte gAdventure
	.endif
	.endif
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
	.ifdef TLA_EDITION_JA
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
	ldr r3, .L_ja_02008e8c
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
	ldr r4, .L_ja_02008e90
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	ldr r0, .L_ja_02008e94
	adds r1, r4, #0
	adds r2, #8
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r3, .L_ja_02008e88
	movs r0, #1
	strh r3, [r4, #8]
	bl Func_020022b0
	movs r2, #1
	negs r2, r2
	str r0, [sp, #64]
	cmp r0, r2
	bne .L_ja_02008e60
	.2byte 0xf000, 0xfd1a @ bl 0x02009894
.L_ja_02008e60:
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
	ldr r0, .L_ja_02008e98
	str r4, [sp, #8]
	bl Func_02002320
	ldr r1, [sp, #64]
	b .L_ja_02008e9c
	.2byte 0x0000
.L_ja_02008e88:
	.4byte 0x00006318
.L_ja_02008e8c:
	.4byte gAdventure
.L_ja_02008e90:
	.4byte 0x050001c0
.L_ja_02008e94:
	.4byte 0x050001e0
.L_ja_02008e98:
	.4byte 0x06006000
.L_ja_02008e9c:
	movs r0, #130
	lsls r0, r0, #1
	str r0, [sp, #44]
	cmp r1, #0
	beq .L_ja_02008eb4
	ldr r3, [sp, #64]
	movs r2, #61
	str r2, [sp, #44]
	cmp r3, #1
	beq .L_ja_02008eb4
	movs r4, #16
	str r4, [sp, #44]
.L_ja_02008eb4:
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
	ldr r6, .L_ja_02009190
	mov r10, r0
	mov r1, r10
	adds r0, r6, #0
	movs r2, #0
	adds r6, #1
	movs r3, #0
	bl UiText_DrawCharacterAtOffset
	adds r0, r6, #0
	mov r1, r10
	adds r6, #1
	movs r2, #0
	movs r3, #8
	bl UiText_DrawCharacterAtOffset
	adds r0, r6, #0
	mov r1, r10
	adds r6, #1
	movs r2, #0
	movs r3, #16
	bl UiText_DrawCharacterAtOffset
	adds r0, r6, #0
	mov r1, r10
	adds r6, #1
	movs r2, #0
	movs r3, #24
	bl UiText_DrawCharacterAtOffset
	adds r0, r6, #0
	mov r1, r10
	adds r6, #1
	movs r2, #48
	movs r3, #0
	bl UiText_DrawCharacterAtOffset
	adds r0, r6, #0
	mov r1, r10
	adds r6, #1
	movs r2, #48
	movs r3, #8
	bl UiText_DrawCharacterAtOffset
	adds r0, r6, #0
	mov r1, r10
	adds r6, #1
	movs r2, #48
	movs r3, #16
	bl UiText_DrawCharacterAtOffset
	adds r0, r6, #0
	mov r1, r10
	adds r6, #1
	movs r2, #48
	movs r3, #24
	bl UiText_DrawCharacterAtOffset
	adds r0, r6, #0
	mov r1, r10
	adds r6, #1
	movs r2, #96
	movs r3, #0
	bl UiText_DrawCharacterAtOffset
	adds r0, r6, #0
	mov r1, r10
	adds r6, #1
	movs r2, #96
	movs r3, #8
	bl UiText_DrawCharacterAtOffset
	adds r0, r6, #0
	mov r1, r10
	adds r6, #1
	movs r2, #96
	movs r3, #16
	bl UiText_DrawCharacterAtOffset
	adds r0, r6, #0
	mov r1, r10
	adds r6, #1
	movs r2, #96
	movs r3, #24
	bl UiText_DrawCharacterAtOffset
	adds r0, r6, #0
	mov r1, r10
	adds r6, #1
	movs r2, #144
	movs r3, #0
	bl UiText_DrawCharacterAtOffset
	adds r0, r6, #0
	mov r1, r10
	movs r2, #144
	movs r3, #8
	bl UiText_DrawCharacterAtOffset
	movs r3, #5
	str r3, [sp, #0]
	adds r6, #1
	mov r0, r10
	movs r1, #24
	movs r2, #0
	movs r3, #24
	bl UiWindow_DrawDividerLine
	adds r0, r6, #0
	mov r1, r10
	adds r6, #1
	movs r2, #192
	movs r3, #0
	bl UiText_DrawCharacterAtOffset
	adds r0, r6, #0
	mov r1, r10
	adds r6, #1
	movs r2, #192
	movs r3, #8
	bl UiText_DrawCharacterAtOffset
	adds r0, r6, #0
	mov r1, r10
	movs r2, #192
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
.L_ja_02008fe2:
	ldr r2, [sp, #36]
	movs r0, #224
	lsls r0, r0, #1
	adds r1, r6, r0
	adds r2, #2
	movs r3, #0
	adds r6, #1
	strb r3, [r2, r1]
	cmp r6, #26
	bne .L_ja_02008fe2
	ldr r2, .L_ja_02009194
	movs r6, #0
.L_ja_02008ffa:
	movs r1, #130
	movs r3, #99
	adds r6, #1
	lsls r1, r1, #1
	strb r3, [r2]
	adds r2, #1
	cmp r6, r1
	bne .L_ja_02008ffa
	ldr r2, [sp, #36]
	movs r3, #200
	lsls r3, r3, #1
	movs r6, #0
	adds r2, r2, r3
	mov r9, r2
	mov r11, r6
.L_ja_02009018:
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
	ble .L_ja_02009042
	adds r7, #18
	mov r8, r7
.L_ja_02009042:
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
	mov r4, r9
	movs r3, #99
	strb r3, [r4]
	ldr r1, [sp, #36]
	movs r0, #1
	add r9, r0
	ldr r3, .L_ja_02009198
	ldr r0, [r2, r1]
	movs r1, #128
	mov lr, r3
	.2byte 0xf800
	adds r6, #1
	movs r2, #4
	add r11, r2
	cmp r6, #50
	bne .L_ja_02009018
	bl Resource_FindFreeEntry
	adds r5, r0, #0
	cmp r5, #95
	bgt .L_ja_020090b2
	bl Func_02002300
	movs r1, #128
	movs r3, #0
	lsls r1, r1, #23
	mov r2, r10
	adds r0, r5, #0
	str r3, [sp, #0]
	bl RenderOutput_Create
	adds r1, r0, #0
	add r0, sp, #108
	str r1, [r0]
	mov r4, r10
	movs r3, #12
	ldrsh r1, [r4, r3]
	movs r3, #14
	ldrsh r2, [r4, r3]
	lsls r1, r1, #3
	lsls r2, r2, #3
	subs r1, #2
	adds r2, #12
	bl Func_02002400
.L_ja_020090b2:
	bl Resource_FindFreeEntry
	adds r5, r0, #0
	cmp r5, #95
	bgt .L_ja_020090fa
	bl Func_02002308
	movs r1, #128
	movs r3, #0
	lsls r1, r1, #23
	mov r2, r10
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
.L_ja_020090fa:
	movs r4, #0
	movs r0, #1
	str r0, [sp, #24]
	str r4, [sp, #32]
	str r4, [sp, #28]
	mov r2, r10
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
.L_ja_0200911e:
	ldr r2, [sp, #12]
	movs r1, #1
	negs r1, r1
	mov r9, r1
	cmp r2, #2
	bne .L_ja_0200912e
	movs r3, #1
	str r3, [sp, #12]
.L_ja_0200912e:
	movs r3, #15
	movs r4, #1
	str r3, [sp, #4]
	mov r0, r10
	ldr r1, [sp, #32]
	ldr r2, [sp, #28]
	ldr r3, [sp, #24]
	str r4, [sp, #0]
	bl Func_02000db4
	ldr r0, [sp, #12]
	cmp r0, #0
	bne .L_ja_0200921c
	ldr r3, .L_ja_0200919c
	movs r2, #16
	ldr r3, [r3, #12]
	ands r3, r2
	cmp r3, #0
	beq .L_ja_02009170
	movs r0, #111
	bl Func_02002408
	ldr r0, [sp, #52]
	ldr r1, [sp, #48]
	adds r0, #1
	cmp r1, #2
	bne .L_ja_02009168
	movs r1, #20
	b .L_ja_0200916a
.L_ja_02009168:
	movs r1, #21
.L_ja_0200916a:
	bl __modsi3
	str r0, [sp, #52]
.L_ja_02009170:
	ldr r3, .L_ja_0200919c
	movs r2, #32
	ldr r3, [r3, #12]
	ands r3, r2
	cmp r3, #0
	beq .L_ja_020091ac
	movs r0, #111
	bl Func_02002408
	ldr r2, [sp, #48]
	cmp r2, #2
	bne .L_ja_020091a0
	ldr r0, [sp, #52]
	movs r1, #20
	adds r0, #19
	b .L_ja_020091a6
.L_ja_02009190:
	.4byte 0x00000d80
.L_ja_02009194:
	.4byte Data_0200274a
.L_ja_02009198:
	.4byte IwramClearWords
.L_ja_0200919c:
	.4byte gInput
.L_ja_020091a0:
	ldr r0, [sp, #52]
	movs r1, #21
	adds r0, #20
.L_ja_020091a6:
	bl __modsi3
	str r0, [sp, #52]
.L_ja_020091ac:
	ldr r3, .L_ja_02009424
	movs r2, #128
	ldr r3, [r3, #12]
	ands r3, r2
	cmp r3, #0
	beq .L_ja_020091e4
	movs r0, #111
	bl Func_02002408
	ldr r1, [sp, #48]
	adds r1, #1
	adds r3, r1, #0
	cmp r1, #0
	bge .L_ja_020091cc
	ldr r3, [sp, #48]
	adds r3, #4
.L_ja_020091cc:
	asrs r3, r3, #2
	str r3, [sp, #48]
	lsls r3, r3, #2
	subs r1, r1, r3
	ldr r3, [sp, #52]
	str r1, [sp, #48]
	cmp r3, #20
	bne .L_ja_020091e4
	cmp r1, #2
	bne .L_ja_020091e4
	movs r4, #3
	str r4, [sp, #48]
.L_ja_020091e4:
	ldr r3, .L_ja_02009424
	movs r2, #64
	ldr r3, [r3, #12]
	ands r3, r2
	cmp r3, #0
	beq .L_ja_0200921c
	movs r0, #111
	bl Func_02002408
	ldr r1, [sp, #48]
	adds r1, #3
	adds r3, r1, #0
	cmp r1, #0
	bge .L_ja_02009204
	ldr r3, [sp, #48]
	adds r3, #6
.L_ja_02009204:
	asrs r3, r3, #2
	ldr r0, [sp, #52]
	str r3, [sp, #48]
	lsls r3, r3, #2
	subs r1, r1, r3
	str r1, [sp, #48]
	cmp r0, #20
	bne .L_ja_02009226
	cmp r1, #2
	bne .L_ja_0200921c
	movs r1, #1
	str r1, [sp, #48]
.L_ja_0200921c:
	ldr r3, [sp, #52]
	movs r2, #3
	str r2, [sp, #24]
	cmp r3, #20
	beq .L_ja_0200922a
.L_ja_02009226:
	movs r4, #1
	str r4, [sp, #24]
.L_ja_0200922a:
	ldr r0, [sp, #52]
	ldr r1, [sp, #48]
	str r0, [sp, #32]
	str r1, [sp, #28]
	cmp r0, #4
	ble .L_ja_0200923c
	adds r2, r0, #0
	adds r2, #1
	str r2, [sp, #32]
.L_ja_0200923c:
	ldr r3, [sp, #52]
	cmp r3, #9
	ble .L_ja_02009248
	ldr r4, [sp, #32]
	adds r4, #1
	str r4, [sp, #32]
.L_ja_02009248:
	ldr r0, [sp, #52]
	cmp r0, #14
	ble .L_ja_02009254
	ldr r1, [sp, #32]
	adds r1, #1
	str r1, [sp, #32]
.L_ja_02009254:
	ldr r2, [sp, #52]
	cmp r2, #19
	ble .L_ja_02009260
	ldr r3, [sp, #32]
	adds r3, #1
	str r3, [sp, #32]
.L_ja_02009260:
	movs r3, #14
	movs r4, #1
	mov r0, r10
	str r3, [sp, #4]
	ldr r1, [sp, #32]
	ldr r2, [sp, #48]
	ldr r3, [sp, #24]
	str r4, [sp, #0]
	bl Func_02000db4
	mov r1, r10
	movs r0, #12
	ldrsh r3, [r1, r0]
	ldr r2, [sp, #52]
	mov r0, r10
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
	ble .L_ja_0200929c
	adds r2, r1, #0
.L_ja_0200929c:
	ldr r4, [sp, #52]
	cmp r4, #9
	ble .L_ja_020092a4
	adds r2, #8
.L_ja_020092a4:
	ldr r1, [sp, #52]
	cmp r1, #14
	ble .L_ja_020092ac
	adds r2, #8
.L_ja_020092ac:
	ldr r3, [sp, #52]
	cmp r3, #20
	bne .L_ja_020092b4
	adds r2, #8
.L_ja_020092b4:
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
	ldr r3, .L_ja_02009428
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
	ble .L_ja_0200932e
	adds r1, #10
.L_ja_0200932e:
	ldr r2, .L_ja_0200942c
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
	ble .L_ja_02009366
	adds r1, #1
	str r1, [sp, #8]
	cmp r1, #8
	bne .L_ja_02009366
	ldr r3, [sp, #60]
	movs r2, #0
	adds r3, #1
	str r2, [sp, #8]
	str r3, [sp, #60]
.L_ja_02009366:
	ldr r4, [sp, #8]
	cmp r4, #0
	beq .L_ja_0200936e
	b .L_ja_0200974a
.L_ja_0200936e:
	ldr r0, [sp, #52]
	ldr r3, .L_ja_02009424
	cmp r0, #20
	bne .L_ja_0200938a
	ldr r1, [sp, #48]
	cmp r1, #0
	bne .L_ja_02009380
	ldr r3, [r3, #12]
	b .L_ja_0200938c
.L_ja_02009380:
	ldr r2, [sp, #48]
	cmp r2, #1
	bne .L_ja_0200938a
	ldr r3, [r3, #12]
	b .L_ja_0200938c
.L_ja_0200938a:
	ldr r3, [r3, #4]
.L_ja_0200938c:
	lsls r3, r3, #16
	asrs r2, r3, #16
	movs r3, #128
	lsls r2, r2, #16
	lsls r3, r3, #9
	ands r3, r2
	cmp r3, #0
	beq .L_ja_0200943c
	ldr r3, [sp, #12]
	cmp r3, #0
	bne .L_ja_0200943c
	ldr r0, [sp, #52]
	movs r4, #0
	mov r9, r4
	cmp r0, #20
	bne .L_ja_020093d0
	ldr r1, [sp, #48]
	cmp r1, #0
	bne .L_ja_020093b8
	movs r2, #1
	mov r9, r2
	b .L_ja_0200943c
.L_ja_020093b8:
	ldr r3, [sp, #48]
	cmp r3, #1
	bne .L_ja_020093c4
	movs r4, #2
	mov r9, r4
	b .L_ja_0200943c
.L_ja_020093c4:
	ldr r0, [sp, #48]
	cmp r0, #3
	bne .L_ja_0200943c
	movs r1, #3
	mov r9, r1
	b .L_ja_0200943c
.L_ja_020093d0:
	ldr r2, [sp, #60]
	ldr r3, [sp, #44]
	cmp r2, r3
	bge .L_ja_0200943c
	ldr r4, [sp, #48]
	ldr r0, [sp, #52]
	lsls r3, r4, #2
	ldr r2, .L_ja_02009430
	adds r3, r3, r4
	lsls r3, r3, #2
	adds r3, r0, r3
	ldrb r5, [r2, r3]
	cmp r5, #63
	bgt .L_ja_0200943c
	movs r0, #112
	bl Func_02002408
	ldr r3, .L_ja_02009434
	ldr r1, [sp, #60]
	strb r5, [r3, r1]
	ldr r0, [sp, #60]
	movs r1, #10
	bl __modsi3
	ldr r3, [sp, #44]
	ldr r2, [sp, #60]
	subs r3, #1
	cmp r2, r3
	bge .L_ja_02009438
	adds r0, r2, #0
	movs r1, #50
	bl __modsi3
	cmp r0, #49
	bne .L_ja_0200941c
	movs r3, #1
	str r3, [sp, #8]
	b .L_ja_0200943c
.L_ja_0200941c:
	ldr r4, [sp, #60]
	adds r4, #1
	str r4, [sp, #60]
	b .L_ja_0200943c
.L_ja_02009424:
	.4byte gInput
.L_ja_02009428:
	.4byte gVBlankCount
.L_ja_0200942c:
	.4byte gData_02002438
.L_ja_02009430:
	.4byte gData_0200265e
.L_ja_02009434:
	.4byte Data_0200274a
.L_ja_02009438:
	movs r0, #2
	str r0, [sp, #12]
.L_ja_0200943c:
	mov r1, r9
	cmp r1, #0
	bne .L_ja_0200944a
	ldr r2, [sp, #12]
	cmp r2, #1
	beq .L_ja_0200944a
	b .L_ja_0200974a
.L_ja_0200944a:
	ldr r1, .L_ja_020097cc
	movs r2, #8
	ldr r3, [r1, #4]
	ands r3, r2
	cmp r3, #0
	bne .L_ja_02009464
	mov r3, r9
	cmp r3, #3
	beq .L_ja_02009464
	ldr r4, [sp, #12]
	cmp r4, #1
	beq .L_ja_02009464
	b .L_ja_020096ba
.L_ja_02009464:
	ldr r7, .L_ja_020097d0
	ldr r1, [sp, #44]
	adds r2, r7, #0
	ldr r0, .L_ja_020097d4
	bl Func_02002068
	mov r9, r0
	mov r6, r9
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
	movs r0, #8
	movs r2, #14
	bl UiWindow_Create
	lsls r5, r5, #16
	asrs r5, r5, #16
	ldrb r3, [r7, r6]
	lsls r5, r5, #16
	lsrs r1, r5, #16
	lsrs r5, r5, #24
	mov r8, r0
	cmp r3, r5
	bne .L_ja_0200957e
	mov r3, r9
	subs r3, #1
	ldrb r3, [r7, r3]
	movs r2, #255
	ands r2, r1
	cmp r3, r2
	bne .L_ja_0200957e
	ldr r0, .L_ja_020097d8
	mov r1, r8
	movs r2, #0
	movs r3, #0
	bl UiText_DrawCharacterAtOffset
	movs r0, #112
	bl Func_02002408
.L_ja_020094c6:
	movs r0, #1
	bl WaitFrames
	ldr r3, .L_ja_020097cc
	movs r2, #1
	ldr r3, [r3, #4]
	ands r3, r2
	cmp r3, #0
	beq .L_ja_020094c6
	movs r0, #112
	bl Func_02002408
	bl Func_02002350
	ldr r5, .L_ja_020097dc
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
	ldr r1, .L_ja_020097d0
	ldr r0, [sp, #64]
	bl Func_020018e0
	mov r0, r8
	movs r1, #2
	bl UiWork_Finalize
	movs r1, #2
	ldr r0, [sp, #56]
	bl UiWork_Finalize
	mov r0, r10
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
	ldr r3, .L_ja_020097e0
	ldr r0, .L_ja_020097e4
	ldr r1, .L_ja_020097e8
	lsls r2, r2, #4
	mov lr, r3
	.2byte 0xf800
	movs r0, #0
	b .L_ja_02009894
.L_ja_0200957e:
	movs r0, #113
	bl Func_02002408
	ldr r0, .L_ja_020097ec
	mov r1, r8
	movs r2, #0
	movs r3, #0
	bl UiText_DrawCharacterAtOffset
.L_ja_02009590:
	movs r0, #1
	bl WaitFrames
	ldr r1, .L_ja_020097cc
	movs r2, #1
	ldr r3, [r1, #4]
	ands r3, r2
	cmp r3, #0
	bne .L_ja_020095ac
	ldr r3, [r1, #4]
	movs r2, #2
	ands r3, r2
	cmp r3, #0
	beq .L_ja_02009590
.L_ja_020095ac:
	movs r1, #2
	mov r0, r8
	bl UiWork_Finalize
	mov r0, r10
	bl RenderOutput_RedrawSavedRect
	ldr r6, .L_ja_020097f0
	mov r1, r10
	adds r0, r6, #0
	movs r2, #0
	adds r6, #1
	movs r3, #0
	bl UiText_DrawCharacterAtOffset
	adds r0, r6, #0
	mov r1, r10
	adds r6, #1
	movs r2, #0
	movs r3, #8
	bl UiText_DrawCharacterAtOffset
	adds r0, r6, #0
	mov r1, r10
	adds r6, #1
	movs r2, #0
	movs r3, #16
	bl UiText_DrawCharacterAtOffset
	adds r0, r6, #0
	mov r1, r10
	adds r6, #1
	movs r2, #0
	movs r3, #24
	bl UiText_DrawCharacterAtOffset
	adds r0, r6, #0
	mov r1, r10
	adds r6, #1
	movs r2, #48
	movs r3, #0
	bl UiText_DrawCharacterAtOffset
	adds r0, r6, #0
	mov r1, r10
	adds r6, #1
	movs r2, #48
	movs r3, #8
	bl UiText_DrawCharacterAtOffset
	adds r0, r6, #0
	mov r1, r10
	adds r6, #1
	movs r2, #48
	movs r3, #16
	bl UiText_DrawCharacterAtOffset
	adds r0, r6, #0
	mov r1, r10
	adds r6, #1
	movs r2, #48
	movs r3, #24
	bl UiText_DrawCharacterAtOffset
	adds r0, r6, #0
	mov r1, r10
	adds r6, #1
	movs r2, #96
	movs r3, #0
	bl UiText_DrawCharacterAtOffset
	adds r0, r6, #0
	mov r1, r10
	adds r6, #1
	movs r2, #96
	movs r3, #8
	bl UiText_DrawCharacterAtOffset
	adds r0, r6, #0
	mov r1, r10
	adds r6, #1
	movs r2, #96
	movs r3, #16
	bl UiText_DrawCharacterAtOffset
	adds r0, r6, #0
	mov r1, r10
	adds r6, #1
	movs r2, #96
	movs r3, #24
	bl UiText_DrawCharacterAtOffset
	adds r0, r6, #0
	mov r1, r10
	adds r6, #1
	movs r2, #144
	movs r3, #0
	bl UiText_DrawCharacterAtOffset
	adds r0, r6, #0
	mov r1, r10
	movs r2, #144
	movs r3, #8
	bl UiText_DrawCharacterAtOffset
	movs r3, #5
	str r3, [sp, #0]
	adds r6, #1
	mov r0, r10
	movs r1, #24
	movs r2, #0
	movs r3, #24
	bl UiWindow_DrawDividerLine
	adds r0, r6, #0
	mov r1, r10
	adds r6, #1
	movs r2, #192
	movs r3, #0
	bl UiText_DrawCharacterAtOffset
	adds r0, r6, #0
	mov r1, r10
	movs r2, #192
	movs r3, #8
	adds r6, #1
	bl UiText_DrawCharacterAtOffset
	adds r0, r6, #0
	mov r1, r10
	movs r2, #192
	movs r3, #24
	bl UiText_DrawCharacterAtOffset
	b .L_ja_0200974a
.L_ja_020096ba:
	ldr r3, [r1, #12]
	movs r2, #128
	lsls r2, r2, #2
	ands r3, r2
	cmp r3, #0
	bne .L_ja_020096cc
	mov r4, r9
	cmp r4, #2
	bne .L_ja_020096e0
.L_ja_020096cc:
	ldr r0, [sp, #60]
	cmp r0, #0
	ble .L_ja_0200974a
	movs r0, #111
	bl Func_02002408
	ldr r1, [sp, #60]
	subs r1, #1
	str r1, [sp, #60]
	b .L_ja_0200974a
.L_ja_020096e0:
	ldr r3, [r1, #12]
	movs r2, #128
	lsls r2, r2, #1
	ands r3, r2
	cmp r3, #0
	bne .L_ja_020096f2
	mov r2, r9
	cmp r2, #1
	bne .L_ja_0200970a
.L_ja_020096f2:
	ldr r3, [sp, #44]
	ldr r4, [sp, #60]
	subs r3, #1
	cmp r4, r3
	bge .L_ja_0200974a
	movs r0, #111
	bl Func_02002408
	ldr r0, [sp, #60]
	adds r0, #1
	str r0, [sp, #60]
	b .L_ja_0200974a
.L_ja_0200970a:
	ldr r3, [r1, #4]
	movs r2, #2
	ands r3, r2
	cmp r3, #0
	beq .L_ja_0200974a
	ldr r1, [sp, #60]
	cmp r1, #0
	ble .L_ja_02009728
	movs r0, #111
	bl Func_02002408
	ldr r2, [sp, #60]
	subs r2, #1
	str r2, [sp, #60]
	b .L_ja_0200974a
.L_ja_02009728:
	movs r0, #113
	bl Func_02002408
	movs r1, #2
	ldr r0, [sp, #56]
	bl UiWork_Finalize
	mov r0, r10
	movs r1, #2
	bl UiWork_Finalize
	movs r0, #74
	bl Func_02002408
	movs r0, #1
	negs r0, r0
	b .L_ja_02009894
.L_ja_0200974a:
	ldr r0, [sp, #60]
	movs r1, #50
	bl __divsi3
	mov r11, r0
	ldr r0, [sp, #56]
	bl RenderOutput_RedrawSavedRect
	movs r6, #0
	movs r5, #2
.L_ja_0200975e:
	adds r2, r5, #0
	ldr r0, [sp, #56]
	movs r1, #0
	movs r3, #18
	adds r6, #1
	str r5, [sp, #0]
	bl UiWindow_DrawDividerLine
	adds r5, #2
	cmp r6, #4
	bne .L_ja_0200975e
	ldr r3, [sp, #40]
	movs r6, #0
	cmp r3, #0
	beq .L_ja_020097ae
	negs r3, r3
	adds r5, r3, #0
	adds r5, #18
.L_ja_02009782:
	movs r4, #243
	lsls r4, r4, #8
	adds r4, #1
	adds r1, r6, r4
	cmp r6, r11
	bne .L_ja_02009796
	movs r0, #243
	lsls r0, r0, #8
	adds r0, #11
	adds r1, r6, r0
.L_ja_02009796:
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
	bne .L_ja_02009782
.L_ja_020097ae:
	ldr r2, [sp, #44]
	mov r9, r2
	cmp r2, #49
	ble .L_ja_020097f8
	ldr r3, [sp, #40]
	subs r3, #1
	cmp r3, r11
	bne .L_ja_020097f4
	ldr r0, [sp, #44]
	movs r1, #50
	bl __modsi3
	mov r9, r0
	b .L_ja_020097f8
	.2byte 0x0000
.L_ja_020097cc:
	.4byte gInput
.L_ja_020097d0:
	.4byte Data_0200274a + 0x104
.L_ja_020097d4:
	.4byte Data_0200274a
.L_ja_020097d8:
	.4byte 0x00000d92
.L_ja_020097dc:
	.4byte gAdventure
.L_ja_020097e0:
	.4byte IwramCopyWords
.L_ja_020097e4:
	.4byte Data_02012f94 + 0xac
.L_ja_020097e8:
	.4byte gGameFlagBytes
.L_ja_020097ec:
	.4byte 0x00000d91
.L_ja_020097f0:
	.4byte 0x00000d80
.L_ja_020097f4:
	movs r3, #50
	mov r9, r3
.L_ja_020097f8:
	mov r4, r9
	movs r6, #0
	cmp r4, #0
	beq .L_ja_02009858
	ldr r0, [sp, #36]
	ldr r1, [sp, #36]
	movs r2, #200
	adds r0, #200
	lsls r2, r2, #1
	mov r8, r0
	adds r7, r1, r2
.L_ja_0200980e:
	movs r3, #50
	mov r0, r11
	muls r0, r3
	ldr r4, .L_ja_020098a4
	adds r3, r0, #0
	adds r0, r6, r3
	ldrb r2, [r4, r0]
	ldrb r3, [r7]
	adds r1, r2, #0
	cmp r1, r3
	beq .L_ja_0200984c
	strb r2, [r7]
	cmp r1, #99
	bne .L_ja_02009838
	mov r1, r8
	ldr r0, [r1]
	ldr r3, .L_ja_020098a8
	movs r1, #128
	mov lr, r3
	.2byte 0xf800
	b .L_ja_0200984c
.L_ja_02009838:
	add r5, sp, #68
	ldrb r0, [r4, r0]
	adds r1, r5, #0
	bl Func_02000a70
	mov r2, r8
	ldrh r0, [r5]
	ldr r1, [r2]
	bl Func_02002328
.L_ja_0200984c:
	movs r3, #4
	adds r6, #1
	add r8, r3
	adds r7, #1
	cmp r6, r9
	bne .L_ja_0200980e
.L_ja_02009858:
	cmp r6, #50
	beq .L_ja_0200988c
	ldr r4, [sp, #36]
	lsls r3, r6, #2
	movs r0, #200
	adds r3, r3, r4
	lsls r0, r0, #1
	adds r7, r3, #0
	adds r3, r6, r0
	adds r7, #200
	adds r5, r3, r4
.L_ja_0200986e:
	ldrb r3, [r5]
	cmp r3, #99
	beq .L_ja_02009882
	movs r3, #99
	strb r3, [r5]
	ldr r0, [r7]
	ldr r3, .L_ja_020098a8
	movs r1, #128
	mov lr, r3
	.2byte 0xf800
.L_ja_02009882:
	adds r6, #1
	adds r7, #4
	adds r5, #1
	cmp r6, #50
	bne .L_ja_0200986e
.L_ja_0200988c:
	movs r0, #1
	bl WaitFrames
	b .L_ja_0200911e
.L_ja_02009894:
	add sp, #124
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_ja_020098a4:
	.4byte Data_0200274a
.L_ja_020098a8:
	.4byte IwramClearWords
	.else
	.ifdef TLA_EDITION_FR
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
	ldr r3, .L_fr_02008ec4
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
	ldr r4, .L_fr_02008ec8
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	ldr r0, .L_fr_02008ecc
	adds r1, r4, #0
	adds r2, #8
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r3, .L_fr_02008ec0
	movs r0, #1
	strh r3, [r4, #8]
	bl Func_020022b0
	movs r2, #1
	negs r2, r2
	str r0, [sp, #64]
	cmp r0, r2
	bne .L_fr_02008e98
	.2byte 0xf000, 0xfca8 @ bl 0x020097e8
.L_fr_02008e98:
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
	ldr r0, .L_fr_02008ed0
	str r4, [sp, #8]
	bl Func_02002320
	ldr r1, [sp, #64]
	b .L_fr_02008ed4
	.2byte 0x0000
.L_fr_02008ec0:
	.4byte 0x00006318
.L_fr_02008ec4:
	.4byte gAdventure
.L_fr_02008ec8:
	.4byte 0x050001c0
.L_fr_02008ecc:
	.4byte 0x050001e0
.L_fr_02008ed0:
	.4byte 0x06006000
.L_fr_02008ed4:
	movs r0, #130
	lsls r0, r0, #1
	str r0, [sp, #44]
	cmp r1, #0
	beq .L_fr_02008eec
	ldr r3, [sp, #64]
	movs r2, #61
	str r2, [sp, #44]
	cmp r3, #1
	beq .L_fr_02008eec
	movs r4, #16
	str r4, [sp, #44]
.L_fr_02008eec:
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
	ldr r0, .L_fr_02009158
	movs r2, #0
	movs r3, #0
	bl UiText_DrawStringInWindow
	ldr r0, .L_fr_0200915c
	mov r1, r9
	movs r2, #0
	movs r3, #8
	bl UiText_DrawStringInWindow
	ldr r0, .L_fr_02009160
	mov r1, r9
	movs r2, #0
	movs r3, #16
	bl UiText_DrawStringInWindow
	ldr r0, .L_fr_02009164
	mov r1, r9
	movs r2, #0
	movs r3, #24
	bl UiText_DrawStringInWindow
	ldr r6, .L_fr_02009168
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
.L_fr_02008f86:
	ldr r2, [sp, #36]
	movs r0, #224
	lsls r0, r0, #1
	adds r1, r6, r0
	adds r2, #2
	movs r3, #0
	adds r6, #1
	strb r3, [r2, r1]
	cmp r6, #26
	bne .L_fr_02008f86
	ldr r2, .L_fr_0200916c
	movs r6, #0
.L_fr_02008f9e:
	movs r1, #130
	movs r3, #99
	adds r6, #1
	lsls r1, r1, #1
	strb r3, [r2]
	adds r2, #1
	cmp r6, r1
	bne .L_fr_02008f9e
	ldr r2, [sp, #36]
	movs r3, #200
	lsls r3, r3, #1
	movs r6, #0
	adds r2, r2, r3
	mov r10, r2
	mov r11, r6
.L_fr_02008fbc:
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
	ble .L_fr_02008fe6
	adds r7, #18
	mov r8, r7
.L_fr_02008fe6:
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
	ldr r3, .L_fr_02009170
	ldr r0, [r2, r1]
	movs r1, #128
	mov lr, r3
	.2byte 0xf800
	adds r6, #1
	movs r2, #4
	add r11, r2
	cmp r6, #50
	bne .L_fr_02008fbc
	bl Resource_FindFreeEntry
	adds r5, r0, #0
	cmp r5, #95
	bgt .L_fr_02009056
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
.L_fr_02009056:
	bl Resource_FindFreeEntry
	adds r5, r0, #0
	cmp r5, #95
	bgt .L_fr_0200909e
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
.L_fr_0200909e:
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
.L_fr_020090c2:
	ldr r2, [sp, #12]
	movs r1, #1
	negs r1, r1
	mov r10, r1
	cmp r2, #2
	bne .L_fr_020090d2
	movs r3, #1
	str r3, [sp, #12]
.L_fr_020090d2:
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
	beq .L_fr_020090ee
	b .L_fr_020091f4
.L_fr_020090ee:
	ldr r3, .L_fr_02009174
	movs r2, #16
	ldr r3, [r3, #12]
	ands r3, r2
	cmp r3, #0
	beq .L_fr_02009126
	movs r0, #111
	bl Func_02002408
	ldr r0, [sp, #52]
	ldr r1, [sp, #48]
	adds r0, #1
	cmp r1, #2
	bne .L_fr_0200911e
	adds r3, r0, #0
	cmp r0, #0
	bge .L_fr_02009114
	ldr r3, [sp, #52]
	adds r3, #16
.L_fr_02009114:
	asrs r3, r3, #4
	str r3, [sp, #52]
	lsls r3, r3, #4
	subs r0, r0, r3
	b .L_fr_02009124
.L_fr_0200911e:
	movs r1, #17
	bl __modsi3
.L_fr_02009124:
	str r0, [sp, #52]
.L_fr_02009126:
	ldr r3, .L_fr_02009174
	movs r2, #32
	ldr r3, [r3, #12]
	ands r3, r2
	cmp r3, #0
	beq .L_fr_02009184
	movs r0, #111
	bl Func_02002408
	ldr r2, [sp, #48]
	cmp r2, #2
	bne .L_fr_02009178
	ldr r2, [sp, #52]
	adds r2, #15
	adds r3, r2, #0
	cmp r2, #0
	bge .L_fr_0200914c
	ldr r3, [sp, #52]
	adds r3, #30
.L_fr_0200914c:
	asrs r3, r3, #4
	str r3, [sp, #52]
	lsls r3, r3, #4
	subs r2, r2, r3
	str r2, [sp, #52]
	b .L_fr_02009184
.L_fr_02009158:
	.4byte gData_02002440
.L_fr_0200915c:
	.4byte gData_02002454
.L_fr_02009160:
	.4byte gData_02002468
.L_fr_02009164:
	.4byte gData_0200247c
.L_fr_02009168:
	.4byte 0x00000f5a
.L_fr_0200916c:
	.4byte Data_0200274a
.L_fr_02009170:
	.4byte IwramClearWords
.L_fr_02009174:
	.4byte gInput
.L_fr_02009178:
	ldr r0, [sp, #52]
	movs r1, #17
	adds r0, #16
	bl __modsi3
	str r0, [sp, #52]
.L_fr_02009184:
	ldr r3, .L_fr_020093f8
	movs r2, #128
	ldr r3, [r3, #12]
	ands r3, r2
	cmp r3, #0
	beq .L_fr_020091bc
	movs r0, #111
	bl Func_02002408
	ldr r1, [sp, #48]
	adds r1, #1
	adds r3, r1, #0
	cmp r1, #0
	bge .L_fr_020091a4
	ldr r3, [sp, #48]
	adds r3, #4
.L_fr_020091a4:
	asrs r3, r3, #2
	str r3, [sp, #48]
	lsls r3, r3, #2
	subs r1, r1, r3
	ldr r3, [sp, #52]
	str r1, [sp, #48]
	cmp r3, #16
	bne .L_fr_020091bc
	cmp r1, #2
	bne .L_fr_020091bc
	movs r4, #3
	str r4, [sp, #48]
.L_fr_020091bc:
	ldr r3, .L_fr_020093f8
	movs r2, #64
	ldr r3, [r3, #12]
	ands r3, r2
	cmp r3, #0
	beq .L_fr_020091f4
	movs r0, #111
	bl Func_02002408
	ldr r1, [sp, #48]
	adds r1, #3
	adds r3, r1, #0
	cmp r1, #0
	bge .L_fr_020091dc
	ldr r3, [sp, #48]
	adds r3, #6
.L_fr_020091dc:
	asrs r3, r3, #2
	ldr r0, [sp, #52]
	str r3, [sp, #48]
	lsls r3, r3, #2
	subs r1, r1, r3
	str r1, [sp, #48]
	cmp r0, #16
	bne .L_fr_020091fe
	cmp r1, #2
	bne .L_fr_020091f4
	movs r1, #1
	str r1, [sp, #48]
.L_fr_020091f4:
	ldr r3, [sp, #52]
	movs r2, #6
	str r2, [sp, #24]
	cmp r3, #16
	beq .L_fr_02009202
.L_fr_020091fe:
	movs r4, #1
	str r4, [sp, #24]
.L_fr_02009202:
	ldr r0, [sp, #52]
	ldr r1, [sp, #48]
	str r0, [sp, #32]
	str r1, [sp, #28]
	cmp r0, #4
	ble .L_fr_02009214
	adds r2, r0, #0
	adds r2, #1
	str r2, [sp, #32]
.L_fr_02009214:
	ldr r3, [sp, #52]
	cmp r3, #9
	ble .L_fr_02009220
	ldr r4, [sp, #32]
	adds r4, #1
	str r4, [sp, #32]
.L_fr_02009220:
	ldr r0, [sp, #52]
	cmp r0, #14
	ble .L_fr_0200922c
	ldr r1, [sp, #32]
	adds r1, #1
	str r1, [sp, #32]
.L_fr_0200922c:
	ldr r2, [sp, #52]
	cmp r2, #15
	ble .L_fr_02009238
	ldr r3, [sp, #32]
	adds r3, #1
	str r3, [sp, #32]
.L_fr_02009238:
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
	ble .L_fr_02009274
	adds r2, r1, #0
.L_fr_02009274:
	ldr r4, [sp, #52]
	cmp r4, #9
	ble .L_fr_0200927c
	adds r2, #8
.L_fr_0200927c:
	ldr r1, [sp, #52]
	cmp r1, #14
	ble .L_fr_02009284
	adds r2, #8
.L_fr_02009284:
	ldr r3, [sp, #52]
	cmp r3, #15
	ble .L_fr_0200928c
	adds r2, #8
.L_fr_0200928c:
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
	ldr r3, .L_fr_020093fc
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
	ble .L_fr_02009306
	adds r1, #10
.L_fr_02009306:
	ldr r2, .L_fr_02009400
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
	ble .L_fr_0200933e
	adds r1, #1
	str r1, [sp, #8]
	cmp r1, #8
	bne .L_fr_0200933e
	ldr r3, [sp, #60]
	movs r2, #0
	adds r3, #1
	str r2, [sp, #8]
	str r3, [sp, #60]
.L_fr_0200933e:
	ldr r4, [sp, #8]
	cmp r4, #0
	beq .L_fr_02009346
	b .L_fr_0200968a
.L_fr_02009346:
	ldr r0, [sp, #52]
	ldr r3, .L_fr_020093f8
	cmp r0, #16
	bne .L_fr_02009362
	ldr r1, [sp, #48]
	cmp r1, #0
	bne .L_fr_02009358
	ldr r3, [r3, #12]
	b .L_fr_02009364
.L_fr_02009358:
	ldr r2, [sp, #48]
	cmp r2, #1
	bne .L_fr_02009362
	ldr r3, [r3, #12]
	b .L_fr_02009364
.L_fr_02009362:
	ldr r3, [r3, #4]
.L_fr_02009364:
	lsls r3, r3, #16
	asrs r2, r3, #16
	movs r3, #128
	lsls r2, r2, #16
	lsls r3, r3, #9
	ands r3, r2
	cmp r3, #0
	beq .L_fr_02009410
	ldr r3, [sp, #12]
	cmp r3, #0
	bne .L_fr_02009410
	ldr r0, [sp, #52]
	movs r4, #0
	mov r10, r4
	cmp r0, #16
	bne .L_fr_020093a8
	ldr r1, [sp, #48]
	cmp r1, #0
	bne .L_fr_02009390
	movs r2, #1
	mov r10, r2
	b .L_fr_02009410
.L_fr_02009390:
	ldr r3, [sp, #48]
	cmp r3, #1
	bne .L_fr_0200939c
	movs r4, #2
	mov r10, r4
	b .L_fr_02009410
.L_fr_0200939c:
	ldr r0, [sp, #48]
	cmp r0, #3
	bne .L_fr_02009410
	movs r1, #3
	mov r10, r1
	b .L_fr_02009410
.L_fr_020093a8:
	ldr r2, [sp, #60]
	ldr r3, [sp, #44]
	cmp r2, r3
	bge .L_fr_02009410
	ldr r4, [sp, #48]
	ldr r0, [sp, #52]
	ldr r2, .L_fr_02009404
	lsls r3, r4, #4
	adds r3, r0, r3
	ldrb r5, [r2, r3]
	cmp r5, #63
	bgt .L_fr_02009410
	movs r0, #112
	bl Func_02002408
	ldr r3, .L_fr_02009408
	ldr r1, [sp, #60]
	strb r5, [r3, r1]
	ldr r0, [sp, #60]
	movs r1, #10
	bl __modsi3
	ldr r3, [sp, #44]
	ldr r2, [sp, #60]
	subs r3, #1
	cmp r2, r3
	bge .L_fr_0200940c
	adds r0, r2, #0
	movs r1, #50
	bl __modsi3
	cmp r0, #49
	bne .L_fr_020093f0
	movs r3, #1
	str r3, [sp, #8]
	b .L_fr_02009410
.L_fr_020093f0:
	ldr r4, [sp, #60]
	adds r4, #1
	str r4, [sp, #60]
	b .L_fr_02009410
.L_fr_020093f8:
	.4byte gInput
.L_fr_020093fc:
	.4byte gVBlankCount
.L_fr_02009400:
	.4byte gData_02002438
.L_fr_02009404:
	.4byte gData_0200265e
.L_fr_02009408:
	.4byte Data_0200274a
.L_fr_0200940c:
	movs r0, #2
	str r0, [sp, #12]
.L_fr_02009410:
	mov r1, r10
	cmp r1, #0
	bne .L_fr_0200941e
	ldr r2, [sp, #12]
	cmp r2, #1
	beq .L_fr_0200941e
	b .L_fr_0200968a
.L_fr_0200941e:
	ldr r1, .L_fr_02009750
	movs r2, #8
	ldr r3, [r1, #4]
	ands r3, r2
	cmp r3, #0
	bne .L_fr_02009438
	mov r3, r10
	cmp r3, #3
	beq .L_fr_02009438
	ldr r4, [sp, #12]
	cmp r4, #1
	beq .L_fr_02009438
	b .L_fr_020095fa
.L_fr_02009438:
	ldr r7, .L_fr_02009754
	ldr r1, [sp, #44]
	adds r2, r7, #0
	ldr r0, .L_fr_02009758
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
	bne .L_fr_02009552
	mov r3, r10
	subs r3, #1
	ldrb r3, [r7, r3]
	movs r2, #255
	ands r2, r1
	cmp r3, r2
	bne .L_fr_02009552
	ldr r0, .L_fr_0200975c
	mov r1, r8
	movs r2, #0
	movs r3, #0
	bl UiText_DrawCharacterAtOffset
	movs r0, #112
	bl Func_02002408
.L_fr_0200949a:
	movs r0, #1
	bl WaitFrames
	ldr r3, .L_fr_02009750
	movs r2, #1
	ldr r3, [r3, #4]
	ands r3, r2
	cmp r3, #0
	beq .L_fr_0200949a
	movs r0, #112
	bl Func_02002408
	bl Func_02002350
	ldr r5, .L_fr_02009760
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
	ldr r1, .L_fr_02009754
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
	ldr r3, .L_fr_02009764
	ldr r0, .L_fr_02009768
	ldr r1, .L_fr_0200976c
	lsls r2, r2, #4
	mov lr, r3
	.2byte 0xf800
	movs r0, #0
	b .L_fr_020097e8
.L_fr_02009552:
	movs r0, #113
	bl Func_02002408
	ldr r0, .L_fr_02009770
	mov r1, r8
	movs r2, #0
	movs r3, #0
	bl UiText_DrawCharacterAtOffset
.L_fr_02009564:
	movs r0, #1
	bl WaitFrames
	ldr r1, .L_fr_02009750
	movs r2, #1
	ldr r3, [r1, #4]
	ands r3, r2
	cmp r3, #0
	bne .L_fr_02009580
	ldr r3, [r1, #4]
	movs r2, #2
	ands r3, r2
	cmp r3, #0
	beq .L_fr_02009564
.L_fr_02009580:
	movs r1, #2
	mov r0, r8
	bl UiWork_Finalize
	mov r0, r9
	bl RenderOutput_RedrawSavedRect
	ldr r0, .L_fr_02009774
	mov r1, r9
	movs r2, #0
	movs r3, #0
	bl UiText_DrawStringInWindow
	ldr r0, .L_fr_02009778
	mov r1, r9
	movs r2, #0
	movs r3, #8
	bl UiText_DrawStringInWindow
	ldr r0, .L_fr_0200977c
	mov r1, r9
	movs r2, #0
	movs r3, #16
	bl UiText_DrawStringInWindow
	ldr r0, .L_fr_02009780
	mov r1, r9
	movs r2, #0
	movs r3, #24
	bl UiText_DrawStringInWindow
	ldr r6, .L_fr_02009784
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
	b .L_fr_0200968a
.L_fr_020095fa:
	ldr r3, [r1, #12]
	movs r2, #128
	lsls r2, r2, #2
	ands r3, r2
	cmp r3, #0
	bne .L_fr_0200960c
	mov r4, r10
	cmp r4, #2
	bne .L_fr_02009620
.L_fr_0200960c:
	ldr r0, [sp, #60]
	cmp r0, #0
	ble .L_fr_0200968a
	movs r0, #111
	bl Func_02002408
	ldr r1, [sp, #60]
	subs r1, #1
	str r1, [sp, #60]
	b .L_fr_0200968a
.L_fr_02009620:
	ldr r3, [r1, #12]
	movs r2, #128
	lsls r2, r2, #1
	ands r3, r2
	cmp r3, #0
	bne .L_fr_02009632
	mov r2, r10
	cmp r2, #1
	bne .L_fr_0200964a
.L_fr_02009632:
	ldr r3, [sp, #44]
	ldr r4, [sp, #60]
	subs r3, #1
	cmp r4, r3
	bge .L_fr_0200968a
	movs r0, #111
	bl Func_02002408
	ldr r0, [sp, #60]
	adds r0, #1
	str r0, [sp, #60]
	b .L_fr_0200968a
.L_fr_0200964a:
	ldr r3, [r1, #4]
	movs r2, #2
	ands r3, r2
	cmp r3, #0
	beq .L_fr_0200968a
	ldr r1, [sp, #60]
	cmp r1, #0
	ble .L_fr_02009668
	movs r0, #111
	bl Func_02002408
	ldr r2, [sp, #60]
	subs r2, #1
	str r2, [sp, #60]
	b .L_fr_0200968a
.L_fr_02009668:
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
	b .L_fr_020097e8
.L_fr_0200968a:
	ldr r0, [sp, #60]
	movs r1, #50
	bl __divsi3
	mov r11, r0
	ldr r0, [sp, #56]
	bl RenderOutput_RedrawSavedRect
	movs r6, #0
	movs r5, #2
.L_fr_0200969e:
	adds r2, r5, #0
	ldr r0, [sp, #56]
	movs r1, #0
	movs r3, #18
	adds r6, #1
	str r5, [sp, #0]
	bl UiWindow_DrawDividerLine
	adds r5, #2
	cmp r6, #4
	bne .L_fr_0200969e
	ldr r3, [sp, #40]
	movs r6, #0
	cmp r3, #0
	beq .L_fr_020096ee
	negs r3, r3
	adds r5, r3, #0
	adds r5, #18
.L_fr_020096c2:
	movs r4, #243
	lsls r4, r4, #8
	adds r4, #1
	adds r1, r6, r4
	cmp r6, r11
	bne .L_fr_020096d6
	movs r0, #243
	lsls r0, r0, #8
	adds r0, #11
	adds r1, r6, r0
.L_fr_020096d6:
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
	bne .L_fr_020096c2
.L_fr_020096ee:
	ldr r2, [sp, #44]
	mov r10, r2
	cmp r2, #49
	ble .L_fr_0200970e
	ldr r3, [sp, #40]
	subs r3, #1
	cmp r3, r11
	bne .L_fr_0200970a
	ldr r0, [sp, #44]
	movs r1, #50
	bl __modsi3
	mov r10, r0
	b .L_fr_0200970e
.L_fr_0200970a:
	movs r3, #50
	mov r10, r3
.L_fr_0200970e:
	mov r4, r10
	movs r6, #0
	cmp r4, #0
	beq .L_fr_020097ac
	ldr r0, [sp, #36]
	ldr r1, [sp, #36]
	movs r2, #200
	adds r0, #200
	lsls r2, r2, #1
	mov r8, r0
	adds r7, r1, r2
.L_fr_02009724:
	movs r3, #50
	mov r0, r11
	muls r0, r3
	ldr r4, .L_fr_02009758
	adds r3, r0, #0
	adds r0, r6, r3
	ldrb r2, [r4, r0]
	ldrb r3, [r7]
	adds r1, r2, #0
	cmp r1, r3
	beq .L_fr_020097a0
	strb r2, [r7]
	cmp r1, #99
	bne .L_fr_0200978c
	mov r1, r8
	ldr r0, [r1]
	ldr r3, .L_fr_02009788
	movs r1, #128
	mov lr, r3
	.2byte 0xf800
	b .L_fr_020097a0
	.2byte 0x0000
.L_fr_02009750:
	.4byte gInput
.L_fr_02009754:
	.4byte Data_0200288a
.L_fr_02009758:
	.4byte Data_0200274a
.L_fr_0200975c:
	.4byte 0x00000f5e
.L_fr_02009760:
	.4byte gAdventure
.L_fr_02009764:
	.4byte IwramCopyWords
.L_fr_02009768:
	.4byte 0x02013040
.L_fr_0200976c:
	.4byte gGameFlagBytes
.L_fr_02009770:
	.4byte 0x00000f5d
.L_fr_02009774:
	.4byte gData_02002440
.L_fr_02009778:
	.4byte gData_02002454
.L_fr_0200977c:
	.4byte gData_02002468
.L_fr_02009780:
	.4byte gData_0200247c
.L_fr_02009784:
	.4byte 0x00000f5a
.L_fr_02009788:
	.4byte IwramClearWords
.L_fr_0200978c:
	add r5, sp, #68
	ldrb r0, [r4, r0]
	adds r1, r5, #0
	bl Func_02000a70
	mov r2, r8
	ldrh r0, [r5]
	ldr r1, [r2]
	bl Func_02002328
.L_fr_020097a0:
	movs r3, #4
	adds r6, #1
	add r8, r3
	adds r7, #1
	cmp r6, r10
	bne .L_fr_02009724
.L_fr_020097ac:
	cmp r6, #50
	beq .L_fr_020097e0
	ldr r4, [sp, #36]
	lsls r3, r6, #2
	movs r0, #200
	adds r3, r3, r4
	lsls r0, r0, #1
	adds r7, r3, #0
	adds r3, r6, r0
	adds r7, #200
	adds r5, r3, r4
.L_fr_020097c2:
	ldrb r3, [r5]
	cmp r3, #99
	beq .L_fr_020097d6
	movs r3, #99
	strb r3, [r5]
	ldr r0, [r7]
	ldr r3, .L_fr_020097f8
	movs r1, #128
	mov lr, r3
	.2byte 0xf800
.L_fr_020097d6:
	adds r6, #1
	adds r7, #4
	adds r5, #1
	cmp r6, #50
	bne .L_fr_020097c2
.L_fr_020097e0:
	movs r0, #1
	bl WaitFrames
	b .L_fr_020090c2
.L_fr_020097e8:
	sub sp, #-124
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_fr_020097f8:
	.4byte IwramClearWords
	.else
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
	.4byte gAdventure
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
	.4byte gData_02002440
.L_02009164:
	.4byte gData_02002454
.L_02009168:
	.4byte gData_02002468
.L_0200916c:
	.4byte gData_0200247c
.L_02009170:
	.4byte MsgFieldSpace
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
	.4byte gVBlankCount
.L_02009408:
	.4byte gData_02002438
.L_0200940c:
	.4byte gData_0200265e
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
	.4byte MsgFieldPasswordAccepted
.L_02009768:
	.4byte gAdventure
.L_0200976c:
	.4byte IwramCopyWords
.L_02009770:
	.4byte Data_02012f94 + 0xac
.L_02009774:
	.4byte gGameFlagBytes
.L_02009778:
	.4byte MsgFieldInvalidPassword
.L_0200977c:
	.4byte gData_02002440
.L_02009780:
	.4byte gData_02002454
.L_02009784:
	.4byte gData_02002468
.L_02009788:
	.4byte gData_0200247c
.L_0200978c:
	.4byte MsgFieldSpace
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
	.endif
	.endif
	.section .text.x020098e0,"ax",%progbits
	.balign 4
	.ifdef TLA_EDITION_JA
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
	ldrb r3, [r1, #1]
	ldrb r6, [r1]
	lsls r3, r3, #8
	orrs r6, r3
	ldrb r3, [r1, #2]
	ldrb r2, [r1, #3]
	lsls r3, r3, #16
	orrs r6, r3
	movs r3, #240
	ands r3, r2
	lsls r3, r3, #20
	orrs r6, r3
	movs r3, #15
	ands r3, r2
	mov r8, r3
	ldrb r3, [r1, #4]
	mov r2, r8
	lsls r3, r3, #4
	orrs r2, r3
	ldrb r3, [r1, #5]
	sub sp, #40
	lsls r3, r3, #12
	orrs r2, r3
	ldrb r3, [r1, #6]
	mov r9, r0
	lsls r3, r3, #20
	orrs r2, r3
	mov r8, r2
	ldrb r3, [r1, #7]
	ldrb r2, [r1, #8]
	mov r10, r1
	mov r11, r3
	str r2, [sp, #4]
	cmp r0, #2
	beq .L_ja_02009aa2
	negs r3, r0
	orrs r3, r0
	lsrs r3, r3, #31
	adds r3, #8
	movs r7, #0
	movs r4, #4
	adds r0, r3, r1
.L_ja_020099ea:
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
	bne .L_ja_020099ea
	movs r7, #0
.L_ja_02009a60:
	ldr r2, .L_ja_02009ac0
	lsls r3, r7, #2
	ldr r0, [r2, r3]
	bl Unit_Get
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
	bne .L_ja_02009a60
.L_ja_02009aa2:
	movs r7, #0
.L_ja_02009aa4:
	mov r2, r11
	movs r3, #1
	asrs r2, r7
	ands r2, r3
	adds r5, r7, #1
	ldr r3, .L_ja_02009ac4
	lsls r0, r7, #1
	cmp r2, #0
	beq .L_ja_02009ac8
	ldrh r0, [r3, r0]
	bl GameFlag_SetBit
	b .L_ja_02009ace
	.2byte 0x0000
.L_ja_02009ac0:
	.4byte gData_020026a0
.L_ja_02009ac4:
	.4byte gData_02002490
.L_ja_02009ac8:
	ldrh r0, [r3, r0]
	bl GameFlag_ClearBit
.L_ja_02009ace:
	adds r7, r5, #0
	cmp r7, #6
	bne .L_ja_02009aa4
	movs r7, #0
	movs r5, #0
.L_ja_02009ad8:
	ldr r2, .L_ja_02009c08
	lsls r3, r7, #2
	ldr r0, [r2, r3]
	bl Unit_Get
	adds r3, r6, #0
	lsrs r3, r5
	movs r2, #127
	ands r3, r2
	adds r7, #1
	strb r3, [r0, #15]
	adds r5, #7
	cmp r7, #4
	bne .L_ja_02009ad8
	movs r0, #0
	bl Djinn_GetRecoveryTable
	movs r3, #148
	lsls r3, r3, #1
	adds r0, r0, r3
	movs r3, #0
	str r3, [r0]
	movs r7, #0
.L_ja_02009b06:
	ldr r3, .L_ja_02009c08
	lsls r2, r7, #2
	ldr r0, [r3, r2]
	bl Unit_Get
	movs r6, #0
	adds r5, r0, #0
.L_ja_02009b14:
	adds r0, r5, #0
	ldr r3, .L_ja_02009c0c
	adds r0, #248
	movs r1, #40
	adds r6, #1
	mov lr, r3
	.2byte 0xf800
	cmp r6, #4
	bne .L_ja_02009b14
	adds r7, #1
	cmp r7, #4
	bne .L_ja_02009b06
	movs r7, #0
.L_ja_02009b2e:
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
	bne .L_ja_02009b2e
	movs r7, #0
.L_ja_02009b58:
	mov r3, r8
	lsrs r3, r7
	movs r2, #1
	ands r3, r2
	cmp r3, #0
	beq .L_ja_02009b7e
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
.L_ja_02009b7e:
	adds r7, #1
	cmp r7, #28
	bne .L_ja_02009b58
	mov r2, r9
	cmp r2, #0
	beq .L_ja_02009bb8
	ldr r3, .L_ja_02009c08
	movs r7, #0
	ldr r0, [r3]
	bl Unit_Get
.L_ja_02009b94:
	ldr r3, [sp, #4]
	movs r2, #1
	asrs r3, r7
	ands r3, r2
	cmp r3, #0
	beq .L_ja_02009baa
	ldr r3, .L_ja_02009c10
	lsls r2, r7, #1
	ldrh r0, [r3, r2]
	bl PartyInventory_Add
.L_ja_02009baa:
	adds r7, #1
	cmp r7, #8
	bne .L_ja_02009b94
	mov r3, r9
	cmp r3, #0
	beq .L_ja_02009bb8
	b .L_ja_02009cda
.L_ja_02009bb8:
	movs r2, #39
	mov r8, r2
	movs r5, #0
	movs r7, #0
.L_ja_02009bc0:
	ldr r2, .L_ja_02009c08
	lsls r3, r7, #2
	ldr r0, [r2, r3]
	bl Unit_Get
	mov r4, r8
	adds r0, #216
	movs r6, #0
	mov r12, r0
	add r4, r10
.L_ja_02009bd4:
	ldrb r1, [r4, #1]
	ldrb r2, [r4]
	movs r3, #7
	subs r3, r3, r5
	adds r0, r5, #1
	asrs r1, r3
	ldr r3, .L_ja_02009c04
	lsls r2, r0
	orrs r2, r1
	ands r2, r3
	mov r3, r12
	strh r2, [r3]
	adds r5, r0, #0
	movs r2, #2
	movs r3, #1
	add r12, r2
	adds r4, #1
	add r8, r3
	cmp r5, #7
	bne .L_ja_02009c14
	movs r5, #0
	adds r4, #1
	add r8, r3
	b .L_ja_02009c14
.L_ja_02009c04:
	.4byte 0x000001ff
.L_ja_02009c08:
	.4byte gData_020026a0
.L_ja_02009c0c:
	.4byte IwramClearWords
.L_ja_02009c10:
	.4byte gData_020026b0
.L_ja_02009c14:
	adds r6, #1
	cmp r6, #15
	bne .L_ja_02009bd4
	adds r7, #1
	cmp r7, #4
	bne .L_ja_02009bc0
	movs r2, #107
	movs r5, #1
	mov r8, r2
	negs r5, r5
	movs r7, #0
.L_ja_02009c2a:
	ldr r3, .L_ja_02009d00
	lsls r2, r7, #2
	ldr r0, [r3, r2]
	bl Unit_Get
	mov r2, r8
	movs r3, #0
	add r2, r10
	mov r11, r0
	mov r9, r3
	mov r12, r2
.L_ja_02009c40:
	movs r1, #0
	cmp r5, #0
	bge .L_ja_02009c5a
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
.L_ja_02009c5a:
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
	bne .L_ja_02009c78
	movs r3, #1
	add r12, r3
	add r8, r3
	movs r5, #3
.L_ja_02009c78:
	ands r1, r2
	ldr r2, .L_ja_02009d04
	movs r6, #0
	mov lr, r2
	movs r0, #216
.L_ja_02009c82:
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
	bne .L_ja_02009ca6
	lsls r3, r1, #11
	orrs r3, r4
	mov r2, r11
	strh r3, [r0, r2]
.L_ja_02009ca6:
	adds r6, #1
	adds r0, #2
	cmp r6, #15
	bne .L_ja_02009c82
	movs r3, #1
	add r9, r3
	mov r2, r9
	cmp r2, #23
	bne .L_ja_02009c40
	adds r7, #1
	cmp r7, #4
	bne .L_ja_02009c2a
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
	ldr r1, .L_ja_02009d08
	orrs r2, r3
	str r2, [r1, #16]
.L_ja_02009cda:
	movs r7, #0
.L_ja_02009cdc:
	ldr r6, .L_ja_02009d00
	lsls r5, r7, #2
	ldr r0, [r6, r5]
	bl Unit_UpdateClass
	adds r7, #1
	ldr r0, [r6, r5]
	bl Unit_Calculate
	cmp r7, #4
	bne .L_ja_02009cdc
	add sp, #40
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_ja_02009d00:
	.4byte gData_020026a0
.L_ja_02009d04:
	.4byte gData_020026c0
.L_ja_02009d08:
	.4byte gAdventure
	.else
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
	bl Unit_Get
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
	.4byte gData_020026a0
.L_02009a20:
	.4byte gData_02002490
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
	bl Unit_Get
	mov r3, r8
	lsrs r3, r6
	movs r2, #127
	adds r5, r0, #0
	ands r3, r2
	strb r3, [r5, #15]
	adds r0, r7, #0
	ldrb r1, [r5, #15]
	bl Unit_GetExperienceForLevel
	movs r1, #146
	lsls r1, r1, #1
	adds r5, r5, r1
	adds r7, #1
	str r0, [r5]
	adds r6, #7
	cmp r7, #4
	bne .L_02009a34
	movs r0, #0
	bl Djinn_GetRecoveryTable
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
	bl Unit_Get
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
	bl Unit_Get
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
	bl Unit_Get
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
	.4byte gData_020026a0
.L_02009b7c:
	.4byte IwramClearWords
.L_02009b80:
	.4byte gData_020026b0
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
	bl Unit_Get
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
	bl Unit_UpdateClass
	adds r7, #1
	ldr r0, [r6, r5]
	bl Unit_Calculate
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
	.4byte gData_020026a0
.L_02009c74:
	.4byte gData_020026c0
.L_02009c78:
	.4byte gAdventure
	.endif
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
	bl Unit_Get
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
	.4byte gData_02002700
.L_02009e48:
	.4byte gData_020026f0
.L_02009e4c:
	.4byte gData_0200270c
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
	bl Unit_Get
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
	.4byte gData_020026f0
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
	bl Unit_Get
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
	.4byte gData_020026f0
.L_0200a060:
	.4byte gData_0200271c
.L_0200a064:
	.4byte gAdventure
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
	.section .bss,"aw",%nobits
	.global Data_0200274a
Data_0200274a:
	.space 0x00000140
	.global Data_0200288a
Data_0200288a:
