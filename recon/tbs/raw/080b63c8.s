.syntax unified
	.thumb
	.global Battle_RunEncounter
	.thumb_func
Battle_RunEncounter:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #68
	str r0, [sp, #12]
	movs r1, #76
	movs r0, #12
	bl Runtime_AllocateBlock
	ldr r1, .L_080b6610
	mov r10, r0
	movs r0, #9
	bl Runtime_AllocateBlock
	movs r5, #249
	lsls r5, r5, #3
	adds r1, r5, #0
	mov r8, r0
	movs r0, #54
	bl Runtime_AllocateBlock
	movs r1, #32
	adds r6, r0, #0
	movs r0, #44
	bl Runtime_AllocateBlock
	movs r1, #160
	str r0, [sp, #8]
	lsls r1, r1, #2
	movs r0, #11
	bl Runtime_AllocateBlock
	movs r1, #12
	add r1, r10
	ldr r3, .L_080b6614
	mov r9, r1
	adds r0, r6, #0
	adds r1, r5, #0
	bl _call_via_r3
	bl Scheduler_ResetTaskTable
	movs r3, #128
	ldr r2, [sp, #8]
	movs r7, #0
	lsls r3, r3, #6
	str r7, [r2, #4]
	str r3, [r2]
	ldr r3, [sp, #8]
	movs r2, #1
	str r2, [r3, #20]
	str r7, [r3, #24]
	str r7, [r3, #28]
	movs r3, #128
	lsls r3, r3, #19
	strh r2, [r3]
	ldr r0, .L_080b6618
	bl GameFlag_SetBitFar
	ldr r0, .L_080b661c
	bl GameFlag_SetBitFar
	bl Render_ResetTransformState
	add r5, sp, #16
	str r7, [r5]
	ldr r3, .L_080b6620
	adds r0, r5, #0
	mov r1, r10
	ldr r2, .L_080b6624
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	str r7, [r5]
	adds r0, r5, #0
	mov r1, r8
	ldr r2, .L_080b6628
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r3, #1
	negs r3, r3
	str r3, [r1, #84]
	ldr r2, [sp, #12]
	movs r0, #37
	str r2, [r1]
	movs r1, #12
	bl Runtime_AllocateBlock
	str r7, [r5]
	adds r1, r0, #0
	ldr r3, .L_080b6620
	adds r0, r5, #0
	ldr r2, .L_080b662c
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	bl Func_0808a4a0
	movs r3, #201
	lsls r3, r3, #3
	add r3, r8
	movs r1, #224
	strh r0, [r3]
	lsls r1, r1, #4
	movs r0, #4
	bl Runtime_AllocateBlock
	movs r1, #192
	lsls r1, r1, #3
	movs r0, #3
	bl Runtime_AllocateBlock
	movs r0, #4
	bl ObjectSystem_InitializeFar
	movs r0, #183
	lsls r0, r0, #1
	bl Func_080770c0
	cmp r0, #0
	beq .L_080b64c6
	movs r0, #1
	bl UiWork_InitializeFar
	b .L_080b64cc
.L_080b64c6:
	movs r0, #0
	bl UiWork_InitializeFar
.L_080b64cc:
	movs r2, #128
	mov r3, r9
	movs r5, #0
	lsls r2, r2, #15
	str r2, [r3, #4]
	str r5, [r3]
	str r5, [r3, #8]
	movs r3, #180
	mov r1, r10
	lsls r3, r3, #16
	str r3, [r1, #4]
	movs r3, #160
	str r2, [r1, #8]
	lsls r3, r3, #6
	mov r2, r10
	str r5, [r1]
	strh r3, [r2, #54]
	movs r3, #160
	lsls r3, r3, #7
	strh r3, [r1, #52]
	movs r3, #128
	lsls r3, r3, #17
	str r3, [r1, #32]
	mov r2, r8
	ldr r0, [r2]
	bl BattleFormation_BuildEnemyList
	adds r6, r0, #0
	movs r0, #182
	lsls r0, r0, #1
	bl Func_080770c0
	cmp r0, #0
	beq .L_080b6528
	mov r3, r8
	adds r3, #68
	str r3, [sp, #4]
	ldr r1, [sp, #4]
	movs r3, #1
	strb r3, [r1]
	ldr r3, .L_080b6630
	ldr r2, .L_080b6634
	adds r3, r3, r2
	movs r2, #4
	strb r2, [r3]
	b .L_080b652e
.L_080b6528:
	mov r3, r8
	adds r3, #68
	str r3, [sp, #4]
.L_080b652e:
	ldr r1, [sp, #4]
	ldrb r3, [r1]
	cmp r3, #0
	beq .L_080b65a2
	ldr r3, .L_080b6638
	str r5, [r3]
	movs r5, #0
	ldr r2, .L_080b663c
	mov r6, r8
	movs r3, #1
	mov r10, r2
	adds r6, #82
	movs r7, #3
	mov r9, r3
.L_080b654a:
	mov r1, r10
	ldrh r2, [r1]
	adds r3, r7, #0
	ands r3, r2
	cmp r3, #3
	beq .L_080b6566
	movs r0, #1
	adds r5, #1
	bl WaitFrames
	cmp r5, #24
	ble .L_080b654a
	mov r2, r9
	strb r2, [r6]
.L_080b6566:
	ldr r3, .L_080b6640
	ldr r3, [r3]
	mov r2, r8
	lsls r3, r3, #26
	lsrs r3, r3, #30
	adds r2, #80
	strb r3, [r2]
	ldr r3, .L_080b6644
	ldr r4, .L_080b6648
	ldr r2, [r3]
	ldr r1, .L_080b664c
	movs r0, #0
.L_080b657e:
	ldrb r3, [r1]
	adds r0, #1
	strb r3, [r2]
	adds r1, #1
	adds r2, #1
	cmp r0, r4
	bls .L_080b657e
	movs r0, #252
	lsls r0, r0, #2
	bl GameFlag_GetByteFar
	adds r6, r0, #0
	bl BattleParty_AssignMemberSlots
	mov r2, r8
	adds r2, #66
	movs r3, #0
	strb r3, [r2]
.L_080b65a2:
	ldr r1, .L_080b6650
	ldr r0, .L_080b6654
	bl Scheduler_AddOrUpdateCallback
	ldr r3, .L_080b6630
	movs r1, #247
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r2, #0
	ldrsh r0, [r3, r2]
	cmp r0, #0
	beq .L_080b65d8
	bl Func_080f9010
	movs r0, #182
	lsls r0, r0, #1
	bl Func_080770c0
	cmp r0, #0
	beq .L_080b65e4
	movs r0, #55
	bl Func_080f9010
	movs r0, #4
	bl Sound_LoadPresetParameters
	b .L_080b65e4
.L_080b65d8:
	movs r0, #51
	bl Func_080f9010
	movs r0, #76
	bl Func_080f9010
.L_080b65e4:
	bl BattleParty_CollectUnitList
	bl BattleUnit_RefreshPlacement
	bl BattlePlacement_UpdateEntries
	bl BattleSummon_UpdateAvailability
	movs r0, #0
	bl Func_08077000
	ldr r3, [r0]
	cmp r3, #0
	beq .L_080b6658
	movs r3, #65
	add r3, r8
	mov r9, r3
	mov r1, r9
	movs r3, #3
	strb r3, [r1]
	b .L_080b6662
	.2byte 0x0000
.L_080b6610:
	.4byte 0x0000082c
.L_080b6614:
	.4byte IwramClearWords
.L_080b6618:
	.4byte 0x00000103
.L_080b661c:
	.4byte 0x00000169
.L_080b6620:
	.4byte 0x040000d4
.L_080b6624:
	.4byte 0x85000013
.L_080b6628:
	.4byte 0x8500020b
.L_080b662c:
	.4byte 0x85000003
.L_080b6630:
	.4byte gCell
.L_080b6634:
	.4byte 0x0000022b
.L_080b6638:
	.4byte gBattleRandomSeed
.L_080b663c:
	.4byte gLinkStatus
.L_080b6640:
	.4byte 0x04000128
.L_080b6644:
	.4byte gBattleOwnerStates
.L_080b6648:
	.4byte 0x000007c7
.L_080b664c:
	.4byte gActorSpriteSlots
.L_080b6650:
	.4byte 0x00000c7f
.L_080b6654:
	.4byte BattlePresentation_UpdateCamera
.L_080b6658:
	movs r2, #65
	add r2, r8
	movs r3, #1
	strb r3, [r2]
	mov r9, r2
.L_080b6662:
	movs r0, #9
	bl UiWindow_CreateWithLayoutBoundsFar
	bl Camera_InitDefaultTransform
	bl BattleActor_CommitPlacement
	bl BattlePresentation_InitializeWorkAndResetState
	movs r3, #201
	lsls r3, r3, #3
	add r3, r8
	ldrh r1, [r3]
	movs r0, #1
	movs r2, #0
	bl BattleBackground_Load
	movs r3, #128
	lsls r3, r3, #10
	movs r0, #160
	movs r1, #160
	str r3, [sp, #0]
	lsls r0, r0, #16
	lsls r1, r1, #15
	movs r2, #0
	movs r3, #0
	bl BattleCamera_SetRange
	movs r1, #0
	movs r2, #0
	movs r3, #190
	movs r0, #0
	bl BattlePres_SetupTransitionScene
	movs r0, #1
	bl Party_ReservedNoOp5B14
	ldr r5, .L_080b66e4
	ldr r3, .L_080b66e8
	strh r5, [r3]
	bl Summon_ClearWorkFields
	movs r0, #128
	bl Resource_LoadIntoFreeSlot
	mov r3, r8
	movs r1, #69
	str r0, [r3, #84]
	add r1, r8
	movs r0, #183
	strb r5, [r1]
	lsls r0, r0, #1
	mov r11, r1
	bl Func_080770c0
	cmp r0, #0
	bne .L_080b6700
	ldr r3, .L_080b66ec
	ldr r1, .L_080b66f0
	adds r3, r3, r1
	ldrb r3, [r3]
	cmp r3, #0
	bne .L_080b671a
	b .L_080b66f4
	.2byte 0x0000
.L_080b66e4:
	.4byte 0x00000000
.L_080b66e8:
	.4byte 0x04000050
.L_080b66ec:
	.4byte gCell
.L_080b66f0:
	.4byte 0x0000022b
.L_080b66f4:
	bl Func_080771a0
	movs r3, #15
	ands r0, r3
	cmp r0, #0
	bne .L_080b6708
.L_080b6700:
	movs r3, #1
	mov r2, r11
	strb r3, [r2]
	b .L_080b671a
.L_080b6708:
	bl Func_080771a0
	movs r3, #31
	ands r0, r3
	cmp r0, #0
	bne .L_080b671a
	movs r3, #2
	mov r1, r11
	strb r3, [r1]
.L_080b671a:
	adds r0, r6, #0
	ldr r1, [sp, #12]
	bl Func_080c02a4
	ldr r3, [sp, #8]
	movs r2, #0
	str r2, [r3, #20]
	ldr r3, .L_080b6998
	movs r1, #200
	strb r2, [r3]
	ldr r0, .L_080b699c
	lsls r1, r1, #4
	bl Scheduler_AddOrUpdateCallback
.L_080b6736:
	bl Battle_ReservedNoOp9B2C
	bl BattleSummon_UpdateAvailability
	movs r0, #0
	bl Func_08077000
	ldr r3, [r0]
	cmp r3, #0
	beq .L_080b6752
	movs r3, #3
	mov r1, r9
	strb r3, [r1]
	b .L_080b6758
.L_080b6752:
	movs r3, #1
	mov r2, r9
	strb r3, [r2]
.L_080b6758:
	ldr r1, [sp, #8]
	movs r3, #160
	lsls r3, r3, #6
	str r3, [r1]
	movs r3, #60
	str r3, [r1, #4]
	mov r2, r9
	movs r5, #187
	ldrb r0, [r2]
	lsls r5, r5, #2
	bl UiWindow_DrawPartyStatusContentsFar
	add r5, r8
	movs r1, #160
	ldr r3, .L_080b69a0
	lsls r1, r1, #1
	adds r0, r5, #0
	bl _call_via_r3
	mov r3, r8
	ldr r0, [r3, #84]
	bl Resource_ResetEntry
	movs r0, #181
	lsls r0, r0, #1
	bl Func_080770c0
	cmp r0, #0
	bne .L_080b67ac
	bl Runtime_GetRemainingIwram
	bl Runtime_GetRemainingEwram
	adds r0, r5, #0
	bl BattlePresentation_BuildActions
	adds r5, r0, #0
	bl Runtime_GetRemainingIwram
	bl Runtime_GetRemainingEwram
	b .L_080b67b4
.L_080b67ac:
	adds r0, r5, #0
	bl BattlePresentation_BuildSortedUnitEntries
	adds r5, r0, #0
.L_080b67b4:
	movs r0, #128
	bl Resource_LoadIntoFreeSlot
	mov r1, r8
	str r0, [r1, #84]
	mov r2, r9
	ldrb r0, [r2]
	bl UiWindow_DrawPartyStatusContentsFar
	cmp r5, #0
	bge .L_080b67cc
	b .L_080b696e
.L_080b67cc:
	movs r7, #0
	cmp r7, r5
	bge .L_080b6862
	movs r6, #187
	lsls r6, r6, #2
.L_080b67d6:
	mov r3, r8
	ldrsh r3, [r6, r3]
	mov r10, r3
	bl Runtime_GetRemainingIwram
	bl Runtime_GetRemainingEwram
	movs r0, #181
	lsls r0, r0, #1
	bl Func_080770c0
	cmp r0, #0
	bne .L_080b6806
	mov r2, r8
	adds r0, r6, r2
	movs r1, #10
	cmp r7, #0
	beq .L_080b67fc
	movs r1, #0
.L_080b67fc:
	bl BattlePresentation_DispatchAction
	cmp r0, #1
	bne .L_080b6814
	b .L_080b6a00
.L_080b6806:
	mov r3, r8
	adds r0, r6, r3
	bl BattlePres_RunAction
	cmp r0, #1
	bne .L_080b6814
	b .L_080b6a00
.L_080b6814:
	bl Runtime_GetRemainingIwram
	bl Runtime_GetRemainingEwram
	movs r0, #1
	movs r1, #0
	bl BattleParty_ListLivingUnits
	cmp r0, #0
	bne .L_080b682a
	b .L_080b69b0
.L_080b682a:
	movs r0, #2
	movs r1, #0
	bl BattleParty_ListLivingUnits
	cmp r0, #0
	bne .L_080b6850
	mov r1, r10
	cmp r1, #7
	bhi .L_080b68ec
	movs r3, #167
	lsls r3, r3, #3
	add r3, r8
	ldr r3, [r3]
	cmp r3, #1
	bne .L_080b68ec
	movs r3, #3
	mov r2, r8
	strh r3, [r2, #62]
	b .L_080b68ec
.L_080b6850:
	bl BattlePres_SyncTurn
	cmp r0, #0
	bge .L_080b685a
	b .L_080b696e
.L_080b685a:
	adds r7, #1
	adds r6, #16
	cmp r7, r5
	blt .L_080b67d6
.L_080b6862:
	movs r3, #0
	mov r1, r11
	strb r3, [r1]
	bl Battle_ReservedNoOpF674
	bl Battle_ProcessRoundEnd
	bl BattleMotion_DestroyAllSlotObjects
	ldr r2, [sp, #4]
	ldrb r3, [r2]
	cmp r3, #0
	beq .L_080b6886
	bl BattlePres_SyncTurn
	cmp r0, #0
	bge .L_080b688c
	b .L_080b696e
.L_080b6886:
	movs r0, #20
	bl WaitFrames
.L_080b688c:
	movs r0, #183
	lsls r0, r0, #1
	bl Func_080770c0
	cmp r0, #0
	bne .L_080b689a
	b .L_080b6736
.L_080b689a:
	ldr r0, .L_080b69a4
	movs r1, #0
	movs r2, #4
	movs r3, #1
	bl UiText_OpenMessageWindowFar
	adds r5, r0, #0
	b .L_080b68b0
.L_080b68aa:
	movs r0, #1
	bl WaitFrames
.L_080b68b0:
	bl UiWork_IsCompleteFar
	cmp r0, #0
	beq .L_080b68aa
	adds r0, r5, #0
	movs r1, #1
	bl UiWork_FinalizeFar
	movs r0, #1
	bl WaitFrames
	movs r2, #4
	movs r3, #1
	movs r1, #10
	ldr r0, .L_080b69a8
	bl UiText_OpenMessageWindowFar
	movs r1, #24
	adds r5, r0, #0
	movs r0, #92
	bl Unnamed_080bb7c0
	adds r0, r5, #0
	movs r1, #1
	bl UiWork_FinalizeFar
	movs r0, #1
	bl WaitFrames
	b .L_080b6736
.L_080b68ec:
	bl Battle_ApplyValueToWork2224
	movs r0, #183
	lsls r0, r0, #1
	bl Func_080770c0
	cmp r0, #0
	bne .L_080b6954
	ldr r1, [sp, #4]
	ldrb r3, [r1]
	cmp r3, #0
	beq .L_080b690a
	movs r0, #58
	bl Func_080f9010
.L_080b690a:
	movs r3, #167
	lsls r3, r3, #3
	add r3, r8
	ldr r3, [r3]
	cmp r3, #0
	beq .L_080b6950
	movs r0, #58
	bl Func_080f9010
	mov r2, r8
	ldrh r3, [r2, #62]
	cmp r3, #1
	bhi .L_080b6950
	ldrh r3, [r2, #60]
	lsls r3, r3, #1
	adds r3, #16
	ldrh r1, [r2, r3]
	movs r0, #128
	movs r2, #26
	bl BattleUnit_AssignFar
	bl UiWork_ClearValueNameTablesFar
	movs r0, #128
	movs r1, #1
	bl Func_08015120
	mov r3, r8
	ldrh r0, [r3, #62]
	ldr r3, .L_080b69ac
	adds r0, r0, r3
	bl UiText_ShowMessageAndWaitCoreFar
	bl BattlePresentation_WaitForAdvance
.L_080b6950:
	bl Battle_AwardSpoils
.L_080b6954:
	movs r0, #17
	bl Func_080f9010
	movs r0, #30
	bl Blend_SetDarkenTarget16
	bl Blend_WaitForTransition
	movs r3, #167
	lsls r3, r3, #3
	add r3, r8
	ldr r7, [r3]
	b .L_080b6a12
.L_080b696e:
	bl Battle_ApplyValueToWork2224
	movs r0, #0
	bl Scheduler_EnableCallbacks
	ldr r3, .L_080b6994
	movs r2, #128
	lsls r2, r2, #19
	strh r3, [r2]
	movs r3, #167
	lsls r3, r3, #3
	movs r0, #250
	add r3, r8
	lsls r0, r0, #2
	ldr r7, [r3]
	bl GameFlag_SetBitFar
	b .L_080b6a12
	.2byte 0x0000
.L_080b6994:
	.4byte 0x00000001
.L_080b6998:
	.4byte Data_03001f58
.L_080b699c:
	.4byte Func_080b7738
.L_080b69a0:
	.4byte IwramClearWords
.L_080b69a4:
	.4byte 0x00000c47
.L_080b69a8:
	.4byte 0x00000c48
.L_080b69ac:
	.4byte 0x00000838
.L_080b69b0:
	bl Battle_ApplyValueToWork2224
	movs r0, #59
	bl Func_080f9010
	bl UiWork_ClearValueNameTablesFar
	ldr r3, .L_080b6a48
	movs r1, #252
	lsls r1, r1, #1
	adds r3, r3, r1
	ldrb r0, [r3]
	movs r1, #1
	bl Func_08015120
	movs r0, #0
	bl BattleParty_PrepareActiveOwners
	cmp r0, #1
	bne .L_080b69e0
	ldr r0, .L_080b6a4c
	bl UiText_ShowMessageAndWaitCoreFar
	b .L_080b69e6
.L_080b69e0:
	ldr r0, .L_080b6a50
	bl UiText_ShowMessageAndWaitCoreFar
.L_080b69e6:
	bl BattlePresentation_WaitForAdvance
	movs r0, #17
	bl Func_080f9010
	movs r7, #1
	movs r0, #30
	bl Blend_SetDarkenTarget16
	negs r7, r7
	bl Blend_WaitForTransition
	b .L_080b6a12
.L_080b6a00:
	movs r0, #17
	bl Func_080f9010
	movs r0, #30
	bl Blend_SetDarkenTarget16
	bl Blend_WaitForTransition
	ldr r7, .L_080b6a54
.L_080b6a12:
	bl BattleParty_ResetActiveRuntimeFields
	bl Battle_ReservedNoOpF674
	bl BattlePlacement_UpdateTimedEntries
	ldr r3, .L_080b6a48
	ldr r2, .L_080b6a58
	adds r3, r3, r2
	movs r2, #0
	strb r2, [r3]
	ldr r0, .L_080b6a5c
	bl Scheduler_RemoveCallback
	bl Runtime_ReleaseHeapBlock10
	adds r0, r7, #0
	add sp, #68
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r1}
	bx r1
	.2byte 0x0000
.L_080b6a48:
	.4byte gCell
.L_080b6a4c:
	.4byte 0x0000083d
.L_080b6a50:
	.4byte 0x00000837
.L_080b6a54:
	.4byte 0x000003e7
.L_080b6a58:
	.4byte 0x0000022b
.L_080b6a5c:
	.4byte Func_080b7738
