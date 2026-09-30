.syntax unified
	.thumb
	.section .text.x020083cc,"ax",%progbits
	.global Func_020003cc
	.thumb_func
Func_020003cc:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r5, .L_020086b4
	movs r0, #0
	add sp, r5
	str r0, [sp, #20]
	bl Clear_LoadBackground
	movs r1, #200
	lsls r1, r1, #4
	ldr r0, .L_020086b8
	bl Scheduler_AddOrUpdateCallback
	ldr r3, .L_020086bc
	movs r1, #224
	ldr r3, [r3]
	lsls r1, r1, #1
	ldr r2, [sp, #20]
	adds r3, r3, r1
	str r2, [r3]
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	ldr r3, .L_020086c0
	movs r0, #225
	lsls r0, r0, #1
	adds r3, r3, r0
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #2
	bne .L_020084a0
.L_02008416:
	ldr r5, .L_020086c4
	movs r1, #5
	adds r0, r5, #0
	bl UiText_ShowPositionedMessageAndWait
	movs r0, #1
	bl Object_CallSpawnRoutineAtOrigin
	adds r7, r0, #0
	bl UiWork_FinalizePendingCore
	cmp r7, #0
	bne .L_02008450
	adds r0, r5, #1
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
	ldr r3, .L_020086c0
	ldr r2, .L_020086c8
	adds r3, r3, r2
	movs r2, #1
	strb r2, [r3]
	bl Save_WriteSelectedSlot
	movs r3, #1
	adds r7, r0, #0
	negs r3, r3
	cmp r7, r3
	beq .L_02008416
.L_02008450:
	movs r0, #60
	bl Battle_WaitMode0
	bl Event_ClearStatus1c6
	movs r0, #17
	bl Engine_AudioPlayCue
	movs r0, #150
	lsls r0, r0, #1
	bl Battle_WaitMode0
	ldr r0, .L_020086cc
	movs r1, #72
	bl Event_SetPairWork1c0
	bl .L_02008cbe
.L_02008474:
	movs r0, #112
	bl Engine_AudioPlayCue
	mov r0, r10
	bl RenderOutput_PrepareForRedraw
	mov r0, r10
	movs r1, #2
	bl UiWork_Finalize
	movs r1, #2
	ldr r0, [sp, #16]
	bl UiWork_Finalize
	movs r1, #2
	ldr r0, [sp, #12]
	bl UiWork_Finalize
	movs r0, #1
	bl WaitFrames
	b .L_020084a8
.L_020084a0:
	add r0, sp, #20
	ldr r3, .L_020086d0
	ldrb r0, [r0]
	strb r0, [r3]
.L_020084a8:
	bl SaveState_ScanRecordFlags
	adds r6, r0, #0
	cmp r6, #0
	bge .L_020084d2
	ldr r3, .L_020086d4
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_020084d2
	ldr r1, .L_020086d8
	ldr r3, .L_020086c0
	movs r2, #1
	adds r3, r3, r1
	strb r2, [r3]
	ldr r3, .L_020086dc
	ldr r0, .L_020086e0
	strb r2, [r3]
	movs r1, #1
	movs r2, #8
	bl UiText_ShowCenteredMessage
.L_020084d2:
	cmp r6, #0
	bne .L_020084f6
	ldr r2, [sp, #20]
	cmp r2, #0
	bne .L_020084f6
	movs r0, #30
	bl WaitFrames
	movs r1, #200
	lsls r1, r1, #4
	ldr r0, .L_020086e4
	bl Scheduler_AddOrUpdateCallback
	movs r0, #1
	bl WaitFrames
	movs r3, #1
	str r3, [sp, #20]
.L_020084f6:
	cmp r6, #0
	ble .L_02008502
	bl Menu_SelectSaveSlotAction
	adds r6, r0, #0
	b .L_02008504
.L_02008502:
	movs r6, #0
.L_02008504:
	cmp r6, #0
	bne .L_0200857a
	bl GameState_InitDefaults
	ldr r2, .L_020086c0
	ldr r0, .L_020086e8
	ldr r1, .L_020086ec
	adds r3, r2, r0
	adds r2, r2, r1
	ldrb r0, [r3]
	ldrb r1, [r2]
	bl PaletteGlow_Update
	movs r5, #1
	movs r7, #0
.L_02008522:
	movs r0, #6
	bl WaitFrames
	adds r0, r7, #0
	bl NameEntry_EditOwnerName
	movs r2, #1
	adds r6, r0, #0
	negs r2, r2
	cmp r6, r2
	bne .L_02008540
	cmp r7, #0
	beq .L_020084a8
	subs r7, #1
	b .L_02008522
.L_02008540:
	ldr r3, .L_020086f0
	movs r0, #0
	ldrsh r3, [r3, r0]
	cmp r3, #0
	beq .L_0200854c
	movs r5, #4
.L_0200854c:
	ldr r3, .L_020086f4
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	beq .L_02008558
	movs r5, #7
.L_02008558:
	adds r7, #1
	cmp r7, r5
	blt .L_02008522
	bl Party_SetFlag32AndRefreshMembers
	ldr r1, .L_020086c0
	movs r0, #224
	ldr r3, .L_020086f8
	lsls r0, r0, #1
	adds r2, r1, r0
	strh r3, [r2]
	movs r3, #225
	lsls r3, r3, #1
	adds r2, r1, r3
	movs r3, #20
	strh r3, [r2]
	b .L_02008c96
.L_0200857a:
	cmp r6, #1
	beq .L_02008580
	b .L_020086a0
.L_02008580:
	movs r0, #1
	bl SaveState_LoadRecordIntoWork
	adds r6, r0, #0
	movs r0, #1
	negs r0, r0
	cmp r6, r0
	beq .L_020084a8
	ldr r0, .L_020086fc
	bl GameFlag_SetBit
	ldr r5, .L_020086c0
	ldr r1, .L_020086e8
	ldr r2, .L_020086ec
	adds r3, r5, r1
	ldrb r0, [r3]
	adds r3, r5, r2
	ldrb r1, [r3]
	bl PaletteGlow_Update
	bl Runtime_GetBuildStampTime
	ldr r3, [r5]
	cmp r3, r0
	beq .L_020085d8
	movs r0, #226
	lsls r0, r0, #1
	adds r3, r5, r0
	movs r1, #224
	ldrh r2, [r3]
	lsls r1, r1, #1
	adds r3, r5, r1
	strh r2, [r3]
	movs r2, #227
	lsls r2, r2, #1
	adds r3, r5, r2
	subs r0, #2
	ldrh r3, [r3]
	adds r2, r5, r0
	strh r3, [r2]
	subs r0, #185
	bl GameFlag_ClearBit
	b .L_02008696
.L_020085d8:
	ldr r3, .L_02008700
	movs r2, #130
	ldr r3, [r3]
	lsls r2, r2, #2
	ands r3, r2
	cmp r3, r2
	bne .L_0200861e
	bl Scene_GetModeMask
	cmp r0, #0
	beq .L_020085f2
	ldr r0, .L_02008704
	b .L_02008664
.L_020085f2:
	movs r1, #226
	lsls r1, r1, #1
	adds r3, r5, r1
	movs r0, #224
	ldrh r2, [r3]
	lsls r0, r0, #1
	adds r3, r5, r0
	strh r2, [r3]
	adds r1, #2
	adds r3, r5, r1
	ldrh r3, [r3]
	adds r0, #2
	adds r2, r5, r0
	strh r3, [r2]
	subs r0, #185
	bl GameFlag_ClearBit
	movs r0, #159
	lsls r0, r0, #1
	bl GameFlag_SetBit
	b .L_02008696
.L_0200861e:
	ldr r3, .L_02008708
	movs r1, #128
	lsls r1, r1, #1
	adds r3, r3, r1
	ldr r2, [r5, #4]
	ldr r3, [r3]
	cmp r2, r3
	beq .L_02008696
	ldr r6, .L_0200870c
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
	beq .L_02008656
	bl UiWork_FinalizePendingCore
	b .L_020084a8
.L_02008656:
	bl UiWork_FinalizePendingCore
	bl Scene_GetModeMask
	cmp r0, #0
	beq .L_0200866c
	adds r0, r6, #2
.L_02008664:
	movs r1, #9
	bl UiText_ShowPositionedMessageAndWait
	b .L_020084a8
.L_0200866c:
	movs r2, #226
	lsls r2, r2, #1
	adds r3, r5, r2
	movs r0, #224
	ldrh r2, [r3]
	lsls r0, r0, #1
	adds r3, r5, r0
	movs r1, #227
	strh r2, [r3]
	lsls r1, r1, #1
	adds r3, r5, r1
	ldrh r3, [r3]
	adds r0, #2
	adds r2, r5, r0
	strh r3, [r2]
	subs r0, #185
	bl GameFlag_ClearBit
	ldr r0, .L_02008710
	bl GameFlag_SetBit
.L_02008696:
	movs r0, #131
	lsls r0, r0, #1
	bl GameFlag_ClearBit
	b .L_02008c96
.L_020086a0:
	cmp r6, #2
	bne .L_020086aa
	bl SaveState_CopySlotPair
	b .L_020084a8
.L_020086aa:
	cmp r6, #3
	bne .L_02008714
	bl SaveState_DeleteSelectedSlot
	b .L_020084a8
.L_020086b4:
	.4byte 0xfffffddc
.L_020086b8:
	.4byte Clear_CheckButtonCodes
.L_020086bc:
	.4byte Data_03001ebc
.L_020086c0:
	.4byte gCell
.L_020086c4:
	.4byte 0x00000007
.L_020086c8:
	.4byte 0x0000020f
.L_020086cc:
	.4byte 0x00000002
.L_020086d0:
	.4byte Data_03001ca0
.L_020086d4:
	.4byte gDebugMode
.L_020086d8:
	.4byte 0x0000022a
.L_020086dc:
	.4byte gOptionMirror
.L_020086e0:
	.4byte 0x0000000a
.L_020086e4:
	.4byte Clear_UpdateBlend
.L_020086e8:
	.4byte 0x00000205
.L_020086ec:
	.4byte 0x00000206
.L_020086f0:
	.4byte Clear_CodeUnlocked
.L_020086f4:
	.4byte Clear_ExtraCodeUnlocked
.L_020086f8:
	.4byte 0x00000008
.L_020086fc:
	.4byte 0x00000109
.L_02008700:
	.4byte Data_03001ae8
.L_02008704:
	.4byte 0x00000006
.L_02008708:
	.4byte gSceneState
.L_0200870c:
	.4byte 0x00000004
.L_02008710:
	.4byte 0x0000013f
.L_02008714:
	cmp r6, #4
	bne .L_020087b2
	movs r0, #4
	bl SaveState_LoadRecordIntoWork
	movs r1, #1
	adds r6, r0, #0
	negs r1, r1
	cmp r6, r1
	bne .L_0200872a
	b .L_020084a8
.L_0200872a:
	ldr r5, .L_02008864
	movs r2, #250
	lsls r2, r2, #1
	adds r3, r5, r2
	movs r2, #0
	str r2, [r3]
	ldr r0, .L_02008868
	bl Engine_GameFlagIsSet
	cmp r0, #0
	beq .L_02008774
	bl InventorySnapshot_Restore
	movs r0, #0
	bl Party_RemoveActiveOwner
	movs r0, #1
	bl Party_RemoveActiveOwner
	movs r0, #2
	bl Party_RemoveActiveOwner
	movs r0, #3
	bl Party_RemoveActiveOwner
	movs r0, #0
	bl Party_AddActiveOwner
	movs r0, #1
	bl Party_AddActiveOwner
	movs r0, #2
	bl Party_AddActiveOwner
	movs r0, #3
	bl Party_AddActiveOwner
.L_02008774:
	ldr r0, .L_0200886c
	ldr r1, .L_02008870
	adds r3, r5, r0
	ldrb r0, [r3]
	adds r3, r5, r1
	ldrb r1, [r3]
	bl PaletteGlow_Update
	ldr r0, .L_02008874
	bl GameFlag_ClearBit
	movs r0, #131
	lsls r0, r0, #1
	bl GameFlag_ClearBit
	movs r0, #191
	lsls r0, r0, #1
	bl GameFlag_SetBit
	ldr r2, .L_02008878
	movs r3, #1
	strb r3, [r2]
	ldr r0, .L_0200887c
	movs r1, #1
	bl Event_SetPairWork1c0
	b .L_02008c96
.L_020087aa:
	movs r7, #1
	b .L_020088e0
.L_020087ae:
	movs r4, #0
	b .L_0200895e
.L_020087b2:
	cmp r6, #5
	beq .L_020087b8
	b .L_020084a8
.L_020087b8:
	movs r0, #5
	bl SaveState_LoadRecordIntoWork
	movs r2, #1
	adds r6, r0, #0
	negs r2, r2
	cmp r6, r2
	bne .L_020087ca
	b .L_020084a8
.L_020087ca:
	ldr r2, .L_02008864
	ldr r0, .L_0200886c
	ldr r1, .L_02008870
	adds r3, r2, r0
	adds r2, r2, r1
	ldrb r0, [r3]
	ldrb r1, [r2]
	bl PaletteGlow_Update
.L_020087dc:
	movs r0, #0
	bl Menu_SelectResourceLayout
	movs r2, #1
	adds r6, r0, #0
	negs r2, r2
	cmp r6, r2
	beq .L_020087b8
	cmp r6, #1
	beq .L_020087f2
	b .L_020089cc
.L_020087f2:
	movs r3, #2
	str r3, [sp, #0]
	movs r1, #5
	movs r2, #18
	movs r3, #8
	movs r0, #6
	bl UiWindow_Create
	ldr r5, .L_02008880
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
	bl SerialRuntime_Initialize
	movs r0, #10
	bl WaitFrames
	ldr r2, .L_02008884
	ldr r3, .L_02008860
	strh r3, [r2]
	strh r3, [r2, #2]
	strh r3, [r2, #4]
	strh r3, [r2, #6]
	ldr r2, .L_02008860
	ldr r3, .L_02008888
	movs r7, #0
	movs r5, #3
	movs r1, #0
.L_0200884c:
	adds r1, #1
	strh r2, [r3]
	strh r2, [r3, #2]
	strh r2, [r3, #4]
	strh r2, [r3, #6]
	adds r3, #24
	cmp r1, #4
	bne .L_0200884c
	b .L_020088ce
	.2byte 0x0000
.L_02008860:
	.4byte 0x00000030
.L_02008864:
	.4byte gCell
.L_02008868:
	.4byte 0x00000952
.L_0200886c:
	.4byte 0x00000205
.L_02008870:
	.4byte 0x00000206
.L_02008874:
	.4byte 0x00000109
.L_02008878:
	.4byte Data_03001ca0
.L_0200887c:
	.4byte 0x000000be
.L_02008880:
	.4byte 0x00000c83
.L_02008884:
	.4byte Data_02002224
.L_02008888:
	.4byte gLinkPeerSignatures
.L_0200888c:
	ldr r3, .L_02008afc
	ldrh r2, [r3]
	adds r3, r5, #0
	ands r3, r2
	cmp r3, r5
	bne .L_020088c8
	ldr r3, .L_02008b00
	ldr r3, [r3]
	lsls r3, r3, #26
	movs r2, #1
	lsrs r3, r3, #30
	eors r3, r2
	lsls r2, r3, #1
	adds r2, r2, r3
	ldr r3, .L_02008b04
	lsls r2, r2, #3
	adds r2, r2, r3
	ldrh r3, [r2]
	cmp r3, #85
	bne .L_020088c8
	ldrh r3, [r2, #2]
	cmp r3, #86
	bne .L_020088c8
	ldrh r3, [r2, #4]
	cmp r3, #84
	bne .L_020088c8
	ldrh r3, [r2, #6]
	cmp r3, #83
	bne .L_020088c8
	b .L_020087aa
.L_020088c8:
	movs r0, #1
	bl WaitFrames
.L_020088ce:
	ldr r3, .L_02008b08
	ldr r2, [r3]
	movs r3, #2
	ands r2, r3
	cmp r2, #0
	beq .L_0200888c
	movs r0, #113
	bl Engine_AudioPlayCue
.L_020088e0:
	adds r0, r6, #0
	movs r1, #2
	bl UiWork_Finalize
	cmp r7, #0
	bne .L_020088ee
	b .L_020087dc
.L_020088ee:
	movs r3, #2
	str r3, [sp, #0]
	movs r1, #10
	movs r2, #20
	movs r3, #4
	movs r0, #5
	bl UiWindow_Create
	adds r6, r0, #0
	movs r2, #0
	movs r3, #4
	adds r1, r6, #0
	ldr r0, .L_02008b0c
	bl UiText_DrawResource
	movs r0, #10
	bl WaitFrames
	ldr r1, .L_02008b10
	ldr r0, .L_02008b14
	bl SerialRuntime_BeginTransferA
	movs r0, #10
	bl WaitFrames
	movs r5, #0
	movs r1, #3
	movs r4, #1
	movs r7, #0
	b .L_0200893a
.L_0200892a:
	movs r0, #1
	str r1, [sp, #8]
	str r4, [sp, #4]
	bl WaitFrames
	ldr r4, [sp, #4]
	ldr r1, [sp, #8]
	adds r7, #1
.L_0200893a:
	ldr r3, .L_02008b18
	cmp r7, r3
	bgt .L_0200895e
	ldr r3, .L_02008afc
	ldrh r2, [r3]
	adds r3, r1, #0
	ands r3, r2
	adds r5, #1
	cmp r3, r1
	bne .L_02008950
	movs r5, #0
.L_02008950:
	cmp r5, #10
	bne .L_02008956
	b .L_020087ae
.L_02008956:
	ldr r3, .L_02008b1c
	ldr r3, [r3]
	cmp r3, #0
	bne .L_0200892a
.L_0200895e:
	cmp r4, #0
	bne .L_02008986
	adds r0, r6, #0
	bl RenderOutput_PrepareForRedraw
	ldr r0, .L_02008b20
	adds r1, r6, #0
	movs r2, #0
	movs r3, #4
	bl UiText_DrawResource
	ldr r7, .L_02008b08
	movs r5, #1
.L_02008978:
	movs r0, #1
	bl WaitFrames
	ldr r3, [r7]
	ands r3, r5
	cmp r3, #0
	beq .L_02008978
.L_02008986:
	movs r0, #10
	bl WaitFrames
	bl SerialRuntime_RemoveIrqHandlers
	movs r0, #10
	bl WaitFrames
	adds r0, r6, #0
	bl RenderOutput_PrepareForRedraw
	adds r0, r6, #0
	movs r1, #2
	bl UiWork_Finalize
	b .L_020084a8
.L_020089a6:
	movs r0, #113
	bl Engine_AudioPlayCue
	mov r0, r10
	bl RenderOutput_PrepareForRedraw
	mov r0, r10
	movs r1, #2
	bl UiWork_Finalize
	movs r1, #2
	ldr r0, [sp, #16]
	bl UiWork_Finalize
	movs r1, #2
	ldr r0, [sp, #12]
	bl UiWork_Finalize
	b .L_020089d2
.L_020089cc:
	cmp r6, #0
	beq .L_020089d2
	b .L_02008c8a
.L_020089d2:
	movs r0, #1
	bl Menu_SelectResourceLayout
	adds r6, r0, #0
	movs r0, #1
	negs r0, r0
	cmp r6, r0
	bne .L_020089e4
	b .L_020087dc
.L_020089e4:
	add r5, sp, #348
	adds r2, r5, #0
	adds r1, r6, #0
	movs r0, #0
	bl Func_02000de4
	adds r1, r5, #0
	mov r9, r0
	bl SceneData_GetBufferCrc16
	mov r3, r9
	lsls r0, r0, #16
	adds r3, #1
	asrs r2, r0, #16
	mov r1, r9
	lsrs r0, r0, #24
	strb r0, [r5, r1]
	strb r2, [r5, r3]
	movs r2, #2
	add r9, r2
	mov r1, r9
	add r2, sp, #28
	adds r0, r5, #0
	bl Clear_EncodePassword
	mov r9, r0
	bl Ui_LoadWindowGraphics
	movs r6, #2
	movs r2, #20
	movs r1, #4
	movs r3, #12
	movs r0, #5
	str r6, [sp, #0]
	bl UiWindow_Create
	movs r3, #0
	movs r1, #50
	mov r10, r0
	mov r0, r9
	mov r11, r3
	bl Engine_MathDivide
	adds r0, #1
	mov r8, r0
	movs r1, #0
	movs r2, #10
	movs r3, #4
	movs r0, #10
	str r6, [sp, #0]
	bl UiWindow_Create
	ldr r5, .L_02008b24
	str r0, [sp, #12]
	ldr r1, [sp, #12]
	adds r0, r5, #0
	movs r2, #6
	movs r3, #4
	bl UiText_DrawResource
	mov r0, r8
	movs r7, #1
	cmp r0, #1
	bne .L_02008a82
	movs r1, #16
	movs r2, #20
	movs r3, #3
	movs r0, #5
	str r6, [sp, #0]
	bl UiWindow_Create
	str r0, [sp, #16]
	ldr r1, [sp, #16]
	subs r0, r5, #2
	movs r2, #80
	movs r3, #0
	bl UiText_DrawCharacterAtOffset
	b .L_02008a9e
.L_02008a82:
	movs r1, #16
	movs r2, #28
	movs r3, #3
	movs r0, #1
	str r6, [sp, #0]
	bl UiWindow_Create
	str r0, [sp, #16]
	ldr r1, [sp, #16]
	subs r0, r5, #1
	movs r2, #0
	movs r3, #0
	bl UiText_DrawCharacterAtOffset
.L_02008a9e:
	ldr r0, .L_02008b28
	bl Graphics_ExpandVramTilesByColorTable
	mov r0, r10
	bl RenderOutput_PrepareForRedraw
.L_02008aaa:
	ldr r0, .L_02008b2c
	bl Link_DrawShiftedTilePair
	ldr r1, .L_02008b08
	ldr r2, [r1]
	movs r3, #2
	ands r2, r3
	cmp r2, #0
	beq .L_02008abe
	b .L_020089a6
.L_02008abe:
	ldr r2, [r1]
	movs r3, #1
	ands r2, r3
	cmp r2, #0
	beq .L_02008ada
	add r11, r3
	movs r7, #1
	cmp r11, r8
	bne .L_02008ad2
	b .L_02008474
.L_02008ad2:
	movs r0, #111
	bl Engine_AudioPlayCue
	b .L_02008b58
.L_02008ada:
	ldr r3, .L_02008b08
	ldr r2, [r3]
	movs r3, #32
	ands r2, r3
	cmp r2, #0
	beq .L_02008b30
	mov r0, r8
	cmp r0, #1
	ble .L_02008b30
	movs r0, #111
	bl Engine_AudioPlayCue
	mov r0, r11
	add r0, r8
	subs r0, #1
	b .L_02008b4e
	.2byte 0x0000
.L_02008afc:
	.4byte gLinkStatus
.L_02008b00:
	.4byte 0x04000128
.L_02008b04:
	.4byte gLinkPeerSignatures
.L_02008b08:
	.4byte gKeyState
.L_02008b0c:
	.4byte 0x00000c85
.L_02008b10:
	.4byte 0x00001004
.L_02008b14:
	.4byte gSaveBuffer
.L_02008b18:
	.4byte 0x000927bf
.L_02008b1c:
	.4byte gSerialSendSource
.L_02008b20:
	.4byte 0x00000c87
.L_02008b24:
	.4byte 0x00000c82
.L_02008b28:
	.4byte 0x06006000
.L_02008b2c:
	.4byte 0x06002500
.L_02008b30:
	ldr r1, .L_02008cd8
	ldr r2, [r1]
	movs r3, #16
	ands r2, r3
	cmp r2, #0
	beq .L_02008b58
	mov r2, r8
	cmp r2, #1
	ble .L_02008b58
	movs r0, #111
	bl Engine_AudioPlayCue
	mov r0, r11
	add r0, r8
	adds r0, #1
.L_02008b4e:
	mov r1, r8
	bl Engine_MathRemainder
	movs r7, #1
	mov r11, r0
.L_02008b58:
	cmp r7, #1
	beq .L_02008b5e
	b .L_02008c82
.L_02008b5e:
	mov r0, r10
	bl RenderOutput_PrepareForRedraw
	movs r7, #0
	movs r5, #2
.L_02008b68:
	adds r2, r5, #0
	mov r0, r10
	movs r1, #0
	movs r3, #18
	adds r7, #1
	str r5, [sp, #0]
	bl UiWindow_DrawDividerLine
	adds r5, #2
	cmp r7, #4
	bne .L_02008b68
	mov r3, r8
	cmp r3, #1
	ble .L_02008bf2
	movs r7, #0
	cmp r3, #0
	beq .L_02008bb4
	negs r3, r3
	adds r5, r3, #0
	movs r6, #0
	adds r5, #18
.L_02008b92:
	ldr r0, .L_02008cdc
	adds r1, r7, r0
	cmp r7, r11
	bne .L_02008b9e
	ldr r2, .L_02008ce0
	adds r1, r7, r2
.L_02008b9e:
	movs r3, #1
	adds r2, r5, #0
	mov r0, r10
	negs r3, r3
	adds r7, #1
	str r6, [sp, #0]
	adds r5, #1
	bl UiWindow_SetTilemapEntry
	cmp r7, r8
	bne .L_02008b92
.L_02008bb4:
	mov r3, r8
	movs r2, #17
	subs r2, r2, r3
	movs r3, #1
	movs r5, #0
	mov r0, r10
	ldr r1, .L_02008ce4
	negs r3, r3
	str r5, [sp, #0]
	bl UiWindow_SetTilemapEntry
	movs r3, #1
	mov r0, r10
	ldr r1, .L_02008ce8
	movs r2, #18
	negs r3, r3
	str r5, [sp, #0]
	bl UiWindow_SetTilemapEntry
	ldr r3, .L_02008cec
	mov r2, r10
	ldr r1, [r3]
	ldr r0, .L_02008cf0
	ldrh r3, [r2, #14]
	adds r1, r1, r0
	lsrs r3, r3, #2
	movs r2, #2
	lsls r2, r3
	ldrb r3, [r1]
	orrs r2, r3
	strb r2, [r1]
.L_02008bf2:
	movs r3, #50
	mov r0, r11
	muls r0, r3
	adds r6, r0, #0
	adds r6, #50
	cmp r6, r9
	ble .L_02008c02
	mov r6, r9
.L_02008c02:
	adds r7, r0, #0
	movs r4, #0
	cmp r7, r6
	beq .L_02008c80
.L_02008c0a:
	add r3, sp, #28
	ldrb r3, [r3, r7]
	movs r0, #63
	ands r0, r3
	add r1, sp, #24
	str r4, [sp, #4]
	bl Clear_NameEntryCharacter
	adds r0, r7, #0
	movs r1, #10
	bl Engine_MathRemainder
	ldr r4, [sp, #4]
	cmp r0, #4
	ble .L_02008c4a
	adds r0, r4, #0
	movs r1, #10
	bl Engine_MathRemainder
	ldr r4, [sp, #4]
	adds r5, r0, #0
	movs r1, #10
	adds r0, r4, #0
	bl Engine_MathDivide
	lsls r2, r5, #1
	adds r3, r0, #0
	adds r2, r2, r5
	lsls r2, r2, #2
	lsls r3, r3, #4
	adds r2, #18
	b .L_02008c6c
.L_02008c4a:
	adds r0, r4, #0
	movs r1, #10
	str r4, [sp, #4]
	bl Engine_MathRemainder
	ldr r4, [sp, #4]
	adds r5, r0, #0
	movs r1, #10
	adds r0, r4, #0
	bl Engine_MathDivide
	lsls r2, r5, #1
	adds r3, r0, #0
	adds r2, r2, r5
	lsls r2, r2, #2
	lsls r3, r3, #4
	adds r2, #8
.L_02008c6c:
	adds r3, #2
	add r0, sp, #24
	mov r1, r10
	bl UiText_DrawString
	ldr r4, [sp, #4]
	adds r7, #1
	adds r4, #1
	cmp r7, r6
	bne .L_02008c0a
.L_02008c80:
	movs r7, #0
.L_02008c82:
	movs r0, #1
	bl WaitFrames
	b .L_02008aaa
.L_02008c8a:
	movs r0, #150
	lsls r0, r0, #1
	bl WaitFrames
	bl .L_020084a8
.L_02008c96:
	ldr r3, .L_02008cf4
	movs r0, #184
	ldr r3, [r3]
	ldr r2, .L_02008cf8
	lsls r0, r0, #1
	adds r3, r3, r0
	strh r2, [r3]
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #17
	bl Engine_AudioPlayCue
	bl Event_ClearStatus1c6
	bl Event_WaitValue1c8Frames
	movs r0, #60
	bl Battle_WaitMode0
.L_02008cbe:
	movs r0, #0
	movs r3, #137
	lsls r3, r3, #2
	add sp, r3
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r1}
	bx r1
	.2byte 0x0000
.L_02008cd8:
	.4byte gKeyState
.L_02008cdc:
	.4byte 0x0000f301
.L_02008ce0:
	.4byte 0x0000f30b
.L_02008ce4:
	.4byte 0x0000f128
.L_02008ce8:
	.4byte 0x0000f129
.L_02008cec:
	.4byte Data_03001e8c
.L_02008cf0:
	.4byte 0x00000ea3
.L_02008cf4:
	.4byte Data_03001ebc
.L_02008cf8:
	.4byte 0x000003e7
	.section .text.x02008de2,"ax",%progbits
	.2byte 0x0000
	.section .text.x02008de4,"ax",%progbits
	.global Func_02000de4
	.thumb_func
Func_02000de4:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #64
	movs r0, #11
	str r1, [sp, #28]
	mov r11, r2
	str r0, [sp, #24]
	cmp r1, #1
	beq .L_02008e18
	cmp r1, #1
	bgt .L_02008e0a
	cmp r1, #0
	beq .L_02008e12
	b .L_02008e22
.L_02008e0a:
	ldr r1, [sp, #28]
	cmp r1, #2
	beq .L_02008e1e
	b .L_02008e22
.L_02008e12:
	movs r2, #173
	str r2, [sp, #24]
	b .L_02008e22
.L_02008e18:
	movs r3, #39
	str r3, [sp, #24]
	b .L_02008e22
.L_02008e1e:
	movs r4, #9
	str r4, [sp, #24]
.L_02008e22:
	ldr r0, [sp, #24]
	movs r6, #0
	mov r9, r6
	cmp r0, #0
	beq .L_02008e3e
	movs r2, #0
	mov r3, r11
.L_02008e30:
	strb r2, [r3]
	movs r1, #1
	ldr r4, [sp, #24]
	add r9, r1
	adds r3, #1
	cmp r9, r4
	bne .L_02008e30
.L_02008e3e:
	mov r0, sp
	movs r6, #0
	adds r0, #32
	str r6, [sp, #20]
	str r6, [sp, #16]
	str r6, [sp, #12]
	str r6, [sp, #8]
	str r0, [sp, #4]
	mov r9, r6
	movs r2, #0
	adds r3, r0, #0
.L_02008e54:
	movs r1, #1
	add r9, r1
	mov r4, r9
	stmia r3!, {r2}
	cmp r4, #8
	bne .L_02008e54
	movs r6, #0
	ldr r5, .L_02009028
	mov r9, r6
	movs r6, #1
.L_02008e68:
	ldrh r0, [r5]
	adds r5, #2
	bl Engine_GameFlagIsSet
	cmp r0, #0
	beq .L_02008e84
	ldr r1, [sp, #8]
	adds r3, r6, #0
	mov r0, r9
	lsls r3, r0
	orrs r1, r3
	lsls r3, r1, #24
	lsrs r3, r3, #24
	str r3, [sp, #8]
.L_02008e84:
	movs r2, #1
	add r9, r2
	mov r3, r9
	cmp r3, #6
	bne .L_02008e68
	movs r4, #0
	ldr r7, [sp, #4]
	mov r9, r4
.L_02008e94:
	ldr r2, .L_0200902c
	mov r6, r9
	lsls r3, r6, #2
	ldr r0, [r2, r3]
	bl Owner_GetState
	mov r12, r0
	mov r1, r12
	adds r1, #16
	movs r0, #0
	ldrsh r3, [r1, r0]
	ldr r0, .L_02009030
	ldrh r2, [r1]
	cmp r3, r0
	ble .L_02008eb6
	strh r0, [r1]
	adds r2, r0, #0
.L_02008eb6:
	lsls r3, r2, #16
	cmp r3, #0
	bge .L_02008ec0
	movs r3, #0
	strh r3, [r1]
.L_02008ec0:
	movs r4, #2
	ldrsh r3, [r1, r4]
	ldrh r2, [r1, #2]
	cmp r3, r0
	ble .L_02008ece
	strh r0, [r1, #2]
	adds r2, r0, #0
.L_02008ece:
	lsls r3, r2, #16
	cmp r3, #0
	bge .L_02008ed8
	movs r3, #0
	strh r3, [r1, #2]
.L_02008ed8:
	ldrh r3, [r1, #8]
	ldr r2, .L_02009034
	cmp r3, r2
	bls .L_02008ee2
	strh r2, [r1, #8]
.L_02008ee2:
	ldrh r3, [r1, #10]
	cmp r3, r2
	bls .L_02008eea
	strh r2, [r1, #10]
.L_02008eea:
	ldrh r3, [r1, #12]
	cmp r3, r2
	bls .L_02008ef2
	strh r2, [r1, #12]
.L_02008ef2:
	ldrb r3, [r1, #14]
	cmp r3, #99
	bls .L_02008efc
	movs r3, #99
	strb r3, [r1, #14]
.L_02008efc:
	movs r3, #0
	ldrsh r2, [r1, r3]
	movs r4, #2
	ldrsh r3, [r1, r4]
	lsls r2, r2, #21
	lsls r3, r3, #10
	orrs r2, r3
	ldrh r3, [r1, #8]
	orrs r2, r3
	str r2, [r7]
	ldrh r3, [r1, #12]
	ldrh r2, [r1, #10]
	lsls r3, r3, #12
	lsls r2, r2, #22
	orrs r2, r3
	ldrb r3, [r1, #14]
	lsls r3, r3, #4
	orrs r2, r3
	mov r6, r9
	str r2, [r7, #4]
	lsls r0, r6, #3
	mov r6, r12
	ldrb r2, [r6, #15]
	adds r3, r2, #0
	cmp r3, #99
	bls .L_02008f36
	movs r3, #99
	strb r3, [r6, #15]
	movs r2, #99
.L_02008f36:
	adds r3, r2, #0
	cmp r3, #0
	bne .L_02008f42
	movs r3, #1
	mov r1, r12
	strb r3, [r1, #15]
.L_02008f42:
	mov r2, r12
	ldrb r3, [r2, #15]
	mov r4, r9
	subs r2, r0, r4
	ldr r6, [sp, #20]
	lsls r3, r2
	orrs r6, r3
	mov r2, r12
	str r6, [sp, #20]
	movs r5, #0
	adds r2, #248
	movs r1, #0
.L_02008f5a:
	ldmia r2!, {r3}
	ldr r0, [sp, #16]
	lsls r3, r1
	adds r0, r0, r3
	adds r5, #1
	str r0, [sp, #16]
	adds r1, #7
	cmp r5, #4
	bne .L_02008f5a
	ldr r1, .L_02009038
	ldr r2, .L_0200903c
	movs r3, #1
	mov r0, r12
	movs r5, #0
	mov r8, r1
	mov lr, r2
	mov r10, r3
	adds r0, #216
.L_02008f7e:
	ldrh r3, [r0]
	mov r1, r8
	movs r4, #0
	ands r1, r3
	mov r2, lr
.L_02008f88:
	ldrh r3, [r2]
	adds r2, #2
	cmp r1, r3
	bne .L_02008f9e
	ldr r6, [sp, #12]
	mov r3, r10
	lsls r3, r4
	orrs r6, r3
	lsls r3, r6, #24
	lsrs r3, r3, #24
	str r3, [sp, #12]
.L_02008f9e:
	adds r4, #1
	cmp r4, #8
	bne .L_02008f88
	adds r5, #1
	adds r0, #2
	cmp r5, #15
	bne .L_02008f7e
	movs r0, #1
	add r9, r0
	mov r1, r9
	adds r7, #8
	cmp r1, #4
	beq .L_02008fba
	b .L_02008e94
.L_02008fba:
	ldr r2, [sp, #28]
	cmp r2, #0
	beq .L_02008fc2
	b .L_02009108
.L_02008fc2:
	movs r3, #39
	movs r6, #0
	mov r10, r3
	mov r9, r6
.L_02008fca:
	mov r4, r9
	ldr r3, .L_0200902c
	lsls r2, r4, #2
	ldr r0, [r3, r2]
	bl Owner_GetState
	mov r5, r10
	adds r4, r0, #0
	movs r0, #216
	movs r7, #0
	mov r8, r0
	add r5, r11
.L_02008fe2:
	mov r1, r8
	ldrh r0, [r1, r4]
	str r4, [sp, #0]
	bl Func_02001444
	ldr r4, [sp, #0]
	mov r2, r8
	ldrh r1, [r2, r4]
	ldr r3, .L_02009024
	ands r1, r3
	adds r0, r6, #1
	ldrb r3, [r5]
	adds r2, r1, #0
	asrs r2, r0
	adds r3, r3, r2
	strb r3, [r5]
	movs r3, #7
	subs r3, r3, r6
	lsls r1, r3
	ldrb r3, [r5, #1]
	adds r3, r3, r1
	strb r3, [r5, #1]
	adds r6, r0, #0
	movs r3, #1
	adds r5, #1
	add r10, r3
	cmp r6, #7
	bne .L_02009040
	movs r6, #0
	adds r5, #1
	add r10, r3
	b .L_02009040
	.2byte 0x0000
.L_02009024:
	.4byte 0x000001ff
.L_02009028:
	.4byte Data_020016d0
.L_0200902c:
	.4byte Data_020016c0
.L_02009030:
	.4byte 0x000007cf
.L_02009034:
	.4byte 0x000003e7
.L_02009038:
	.4byte 0x000001ff
.L_0200903c:
	.4byte Data_020016dc
.L_02009040:
	movs r0, #2
	adds r7, #1
	add r8, r0
	cmp r7, #15
	bne .L_02008fe2
	movs r1, #1
	add r9, r1
	mov r2, r9
	cmp r2, #4
	bne .L_02008fca
	movs r3, #107
	movs r6, #1
	movs r4, #0
	mov r10, r3
	negs r6, r6
	mov r9, r4
.L_02009060:
	ldr r3, .L_020091d4
	mov r0, r9
	lsls r2, r0, #2
	ldr r0, [r3, r2]
	bl Owner_GetState
	ldr r2, .L_020091d8
	mov r8, r0
	movs r1, #0
	mov r0, r10
	mov lr, r1
	mov r12, r2
	add r0, r11
.L_0200907a:
	mov r3, r12
	mov r1, r8
	ldrh r4, [r3]
	movs r5, #0
	movs r7, #0
	adds r1, #216
.L_02009086:
	ldrh r2, [r1]
	ldr r3, .L_020091dc
	ands r3, r2
	adds r1, #2
	cmp r3, r4
	bne .L_0200909a
	movs r3, #248
	lsls r3, r3, #8
	ands r3, r2
	lsrs r5, r3, #11
.L_0200909a:
	adds r7, #1
	cmp r7, #15
	bne .L_02009086
	lsls r2, r5, #16
	cmp r6, #0
	bge .L_020090be
	lsrs r1, r2, #16
	negs r3, r6
	adds r2, r1, #0
	asrs r2, r3
	ldrb r3, [r0]
	movs r4, #1
	adds r3, r3, r2
	strb r3, [r0]
	add r10, r4
	adds r0, #1
	adds r6, #8
	b .L_020090c0
.L_020090be:
	lsrs r1, r2, #16
.L_020090c0:
	ldrb r3, [r0]
	lsls r1, r6
	adds r3, r3, r1
	movs r1, #5
	subs r6, #5
	negs r1, r1
	strb r3, [r0]
	cmp r6, r1
	bne .L_020090da
	movs r2, #1
	adds r0, #1
	add r10, r2
	movs r6, #3
.L_020090da:
	movs r4, #1
	add lr, r4
	movs r3, #2
	mov r1, lr
	add r12, r3
	cmp r1, #23
	bne .L_0200907a
	add r9, r4
	mov r2, r9
	cmp r2, #4
	bne .L_02009060
	ldr r2, .L_020091e0
	mov r1, r11
	ldrh r3, [r2, #18]
	adds r1, #165
	strb r3, [r1]
	ldr r3, [r2, #16]
	adds r1, #1
	lsrs r3, r3, #8
	strb r3, [r1]
	ldr r3, [r2, #16]
	adds r1, #1
	strb r3, [r1]
.L_02009108:
	ldr r3, [sp, #28]
	cmp r3, #2
	beq .L_0200917c
	ldr r4, [sp, #28]
	negs r3, r3
	orrs r3, r4
	lsrs r3, r3, #31
	adds r3, #8
	movs r6, #0
	mov r1, r11
	ldr r4, [sp, #4]
	mov r9, r6
	adds r0, r3, r1
.L_02009122:
	ldr r2, [r4]
	lsrs r3, r2, #24
	strb r3, [r0]
	lsrs r3, r2, #16
	strb r3, [r0, #1]
	lsrs r3, r2, #8
	strb r3, [r0, #2]
	strb r2, [r0, #3]
	ldr r1, [r4, #4]
	lsrs r3, r1, #24
	strb r3, [r0, #4]
	lsrs r3, r1, #16
	strb r3, [r0, #5]
	lsrs r3, r1, #8
	strb r1, [r0, #7]
	strb r3, [r0, #6]
	ldr r2, [r4, #8]
	lsrs r3, r2, #28
	orrs r1, r3
	lsrs r3, r2, #20
	strb r3, [r0, #8]
	lsrs r3, r2, #12
	strb r3, [r0, #9]
	lsrs r3, r2, #4
	lsls r2, r2, #4
	strb r2, [r0, #11]
	strb r3, [r0, #10]
	strb r1, [r0, #7]
	ldr r1, [r4, #12]
	lsrs r3, r1, #28
	orrs r2, r3
	strb r2, [r0, #11]
	lsrs r3, r1, #20
	movs r2, #1
	strb r3, [r0, #12]
	add r9, r2
	lsrs r3, r1, #12
	strb r3, [r0, #13]
	lsrs r1, r1, #4
	mov r3, r9
	strb r1, [r0, #14]
	adds r4, #16
	adds r0, #15
	cmp r3, #2
	bne .L_02009122
.L_0200917c:
	add r4, sp, #20
	ldrb r4, [r4]
	mov r6, r11
	strb r4, [r6]
	ldr r6, [sp, #20]
	mov r0, r11
	lsrs r3, r6, #8
	strb r3, [r0, #1]
	lsrs r3, r6, #16
	strb r3, [r0, #2]
	lsrs r2, r6, #20
	movs r3, #240
	ands r2, r3
	ldr r3, [sp, #16]
	movs r1, #15
	ands r3, r1
	orrs r2, r3
	strb r2, [r0, #3]
	ldr r1, [sp, #16]
	lsrs r3, r1, #4
	strb r3, [r0, #4]
	lsrs r3, r1, #12
	strb r3, [r0, #5]
	lsrs r3, r1, #20
	strb r3, [r0, #6]
	add r2, sp, #8
	ldrb r2, [r2]
	strb r2, [r0, #7]
	ldr r3, [sp, #28]
	cmp r3, #0
	beq .L_020091c0
	add r4, sp, #12
	ldrb r4, [r4]
	strb r4, [r0, #8]
.L_020091c0:
	ldr r0, [sp, #24]
	add sp, #64
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r1}
	bx r1
.L_020091d4:
	.4byte Data_020016c0
.L_020091d8:
	.4byte Data_020016ec
.L_020091dc:
	.4byte 0x000001ff
.L_020091e0:
	.4byte gCell
	.section .rodata.x020094d4,"a",%progbits
	.global Clear_CodeSequence
Clear_CodeSequence:
	.4byte 0x00040004
	.4byte 0x00000004
	.global Clear_ExtraCodeSequence
Clear_ExtraCodeSequence:
	.4byte 0x00800040
	.4byte 0x00800040
	.4byte 0x00100020
	.4byte 0x00100020
	.4byte 0x00100040
	.4byte 0x00200080
	.4byte 0x00040040
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
	.4byte 0x00000000
	.global Clear_ScriptTable
Clear_ScriptTable:
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
	.global Clear_MessageTable
Clear_MessageTable:
	.4byte 0x000001ff
	.global Clear_ActorTable
Clear_ActorTable:
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Clear_EffectTable
Clear_EffectTable:
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Clear_BlendFrame
Clear_BlendFrame:
	.2byte 0x0000
	.global Clear_CodeUnlocked
Clear_CodeUnlocked:
	.2byte 0x0000
	.global Clear_ExtraCodeUnlocked
Clear_ExtraCodeUnlocked:
	.2byte 0x0000
	.global Clear_CodeProgress
Clear_CodeProgress:
	.2byte 0x0000
	.global Clear_ExtraCodeProgress
Clear_ExtraCodeProgress:
	.2byte 0x0000
	.global Clear_CodeHeld
Clear_CodeHeld:
	.2byte 0x0000
	.global Clear_ExtraCodeHeld
Clear_ExtraCodeHeld:
	.4byte 0x00000000
	.global Data_020016c0
Data_020016c0:
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000003
	.global Data_020016d0
Data_020016d0:
	.4byte 0x09510941
	.4byte 0x08d108b3
	.4byte 0x0868081e
	.global Data_020016dc
Data_020016dc:
	.4byte 0x00c900c8
	.4byte 0x00cb00ca
	.4byte 0x00cd00cc
	.4byte 0x00cf00ce
	.global Data_020016ec
Data_020016ec:
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
