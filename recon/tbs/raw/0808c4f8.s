.syntax unified
	.thumb
	.global Func_0808c4f8
	.thumb_func
Func_0808c4f8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r1, .L_0808c7f0
	movs r0, #27
	sub sp, #16
	bl Runtime_AllocateBlock
	movs r7, #0
	mov r8, r0
	add r0, sp, #12
	str r7, [r0]
	ldr r3, .L_0808c7f4
	mov r1, r8
	ldr r2, .L_0808c7f8
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r0, .L_0808c7fc
	bl GameFlag_ClearBitFar
	ldr r5, .L_0808c800
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r5, r1
	movs r4, #228
	ldrh r2, [r3]
	lsls r4, r4, #1
	adds r3, r5, r4
	strh r2, [r3]
	adds r1, #2
	adds r3, r5, r1
	ldrh r3, [r3]
	adds r4, #2
	adds r2, r5, r4
	strh r3, [r2]
	adds r1, #12
	ldr r3, .L_0808c804
	adds r2, r5, r1
	strh r3, [r2]
	movs r3, #232
	lsls r3, r3, #1
	adds r2, r5, r3
	movs r3, #1
	negs r3, r3
	adds r4, #8
	strh r3, [r2]
	adds r1, #6
	adds r2, r5, r4
	strh r3, [r2]
	adds r2, r5, r1
	strh r3, [r2]
	bl Scheduler_ResetTaskTable
	movs r0, #0
	bl Djinn_ResolvePendingEvent
	movs r2, #237
	lsls r2, r2, #1
	adds r3, r5, r2
	movs r4, #0
	ldrsh r0, [r3, r4]
	ldr r3, .L_0808c808
	cmp r0, r3
	bne .L_0808c598
	bl Map_InitializePerspectiveSceneFar
	movs r1, #137
	lsls r1, r1, #2
	adds r2, r5, r1
	movs r3, #1
	strb r3, [r2]
	movs r0, #0
	movs r6, #3
	bl BattleFx_SelectLocationRule
	b .L_0808c5a4
.L_0808c598:
	bl Map_LoadLayeredSceneFar
	movs r0, #1
	movs r6, #2
	bl BattleFx_SelectLocationRule
.L_0808c5a4:
	movs r3, #207
	lsls r3, r3, #1
	add r3, r8
	strh r6, [r3]
	adds r0, r6, #0
	bl ObjectSystem_InitializeFar
	bl Func_08015000
	bl BattleFx_ResetCounters
	ldr r5, .L_0808c80c
	ldr r0, [r5, #36]
	bl _call_via_r0
	mov r2, r8
	str r0, [r2, #16]
	bl BattleMap_ApplyEntranceView
	ldr r0, [r5, #28]
	bl _call_via_r0
	bl ObjectTable_ResetForObject
	ldr r0, .L_0808c810
	bl Func_080770c0
	cmp r0, #0
	beq .L_0808c5e2
	bl ObjectTable_Restore
.L_0808c5e2:
	ldr r5, .L_0808c800
	movs r4, #141
	lsls r4, r4, #2
	adds r3, r5, r4
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	beq .L_0808c5f6
	bl FieldObject_PlaceSceneActors
.L_0808c5f6:
	cmp r6, #3
	bne .L_0808c614
	bl Map_UpdateCurrentTileBlockFar
	movs r2, #238
	lsls r2, r2, #1
	movs r4, #242
	adds r3, r5, r2
	lsls r4, r4, #1
	ldr r0, [r3]
	adds r3, r5, r4
	ldr r1, [r3]
	bl WorldMap_LoadGraphicsFar
	b .L_0808c618
.L_0808c614:
	bl Func_08009128
.L_0808c618:
	bl Battle_PlaceMapMarkers
	bl BattleEffect_InitializeBuffers
	movs r0, #128
	movs r1, #0
	lsls r0, r0, #9
	bl BattleFx_ApplyColorToTargetBuffer
	movs r2, #224
	lsls r2, r2, #1
	add r2, r8
	movs r3, #128
	adds r1, r2, #0
	lsls r3, r3, #1
	str r1, [sp, #8]
	str r3, [r2]
	adds r3, #200
	add r3, r8
	movs r4, #16
	movs r7, #227
	movs r2, #216
	str r4, [r3]
	mov r10, r3
	lsls r7, r7, #1
	ldr r3, .L_0808c814
	lsls r2, r2, #1
	movs r6, #0
	add r7, r8
	add r2, r8
	strh r6, [r7]
	str r3, [r2]
	adds r3, #27
	add r3, r8
	ldr r5, .L_0808c800
	ldr r1, .L_0808c818
	str r6, [r3]
	adds r3, r5, r1
	mov r11, r4
	movs r4, #0
	ldrsh r3, [r3, r4]
	cmp r3, #2
	bne .L_0808c678
	movs r0, #162
	str r6, [r2]
	lsls r0, r0, #1
	bl GameFlag_SetBitFar
.L_0808c678:
	movs r3, #222
	ldr r1, .L_0808c810
	lsls r3, r3, #1
	ldr r2, .L_0808c81c
	add r3, r8
	mov r9, r1
	str r2, [r3]
	mov r0, r9
	bl Func_080770c0
	cmp r0, #0
	bne .L_0808c6bc
	bl Party_ResolveTablePair
	movs r2, #139
	lsls r2, r2, #2
	ldr r1, .L_0808c820
	adds r3, r5, r2
	mov r4, r11
	strh r4, [r3]
	adds r3, r5, r1
	strh r6, [r3]
	movs r3, #140
	lsls r3, r3, #2
	adds r2, r5, r3
	ldr r4, .L_0808c824
	movs r3, #1
	strh r3, [r2]
	ldr r3, .L_0808c804
	adds r2, r5, r4
	adds r1, #30
	strh r3, [r2]
	adds r3, r5, r1
	strh r6, [r3]
.L_0808c6bc:
	bl BattleFx_ScheduleCallbackWhenValue24cSet
	ldr r2, .L_0808c828
	ldr r3, .L_0808c804
	add r2, r8
	strh r3, [r2]
	ldr r3, .L_0808c80c
	ldr r0, [r3, #4]
	bl _call_via_r0
	movs r3, #184
	lsls r3, r3, #1
	add r3, r8
	movs r2, #0
	ldrsh r0, [r3, r2]
	cmp r0, #0
	beq .L_0808c6ec
	adds r7, r0, #0
	strh r6, [r3]
	b .L_0808ce1c
.L_0808c6e4:
	movs r3, #0
	adds r7, r5, #0
	strh r3, [r1]
	b .L_0808ce1c
.L_0808c6ec:
	mov r0, r9
	bl GameFlag_ClearBitFar
	bl BattleFx_SumCounters
	cmp r0, #0
	bne .L_0808c794
	ldrh r5, [r7]
	cmp r5, #0
	bne .L_0808c72e
	ldr r3, [sp, #8]
	mov r4, r10
	ldr r0, [r3]
	ldr r1, [r4]
	bl DisplayTransition_Start
	movs r3, #1
	strh r3, [r7]
	movs r3, #160
	lsls r3, r3, #19
	strh r5, [r3]
	bl ObjectEffect_RunPendingFlagEvent
	cmp r0, #0
	bne .L_0808c72e
	mov r1, r10
	ldr r0, [r1]
	adds r0, #1
	lsrs r3, r0, #31
	adds r0, r0, r3
	asrs r0, r0, #1
	bl WaitFrames
.L_0808c72e:
	ldr r5, .L_0808c82c
	adds r0, r5, #0
	bl Func_080770c0
	cmp r0, #0
	beq .L_0808c750
	adds r0, r5, #0
	bl GameFlag_ClearBitFar
	ldr r3, .L_0808c800
	movs r2, #224
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r4, #0
	ldrsh r0, [r3, r4]
	bl UiTimedNotice_CreateFar
.L_0808c750:
	ldr r5, .L_0808c800
	movs r1, #141
	lsls r1, r1, #2
	adds r6, r5, r1
	movs r2, #0
	ldrsh r0, [r6, r2]
	cmp r0, #0
	beq .L_0808c76a
	movs r1, #1
	bl Djinn_ResolvePendingEvent
	movs r3, #0
	strh r3, [r6]
.L_0808c76a:
	movs r3, #143
	lsls r3, r3, #2
	adds r5, r5, r3
	ldrh r3, [r5]
	cmp r3, #0
	beq .L_0808c794
	bl Battle_SetObjectFlag5bWhenMode3
	ldrh r0, [r5]
	bl Party_CheckMemberValueTotal
	cmp r0, #0
	bne .L_0808c78c
	ldrh r0, [r5]
	movs r1, #0
	bl PartyInventory_GiveItem
.L_0808c78c:
	bl Battle_ClearObjectFlag5bWhenMode3
	movs r3, #0
	strh r3, [r5]
.L_0808c794:
	ldr r4, .L_0808c800
	movs r1, #0
	mov r9, r4
	mov r11, r1
.L_0808c79c:
	movs r0, #130
	lsls r0, r0, #1
	bl GameFlag_SetBitFar
	ldr r2, .L_0808c830
	ldr r3, [r2]
	lsls r3, r3, #2
	mov r4, r8
	adds r3, #20
	ldr r3, [r4, r3]
	mov r10, r3
	mov r0, r10
	bl Object_ResetMotion
	mov r1, r10
	movs r2, #238
	ldr r3, [r1, #8]
	lsls r2, r2, #1
	add r2, r9
	str r3, [r2]
	movs r3, #240
	lsls r3, r3, #1
	add r3, r9
	mov r2, r11
	str r2, [r3]
	movs r2, #242
	ldr r3, [r1, #16]
	lsls r2, r2, #1
	add r2, r9
	str r3, [r2]
	movs r2, #244
	ldrh r3, [r1, #6]
	lsls r2, r2, #1
	add r2, r9
	str r3, [r2]
	mov r3, r10
	adds r3, #34
	ldrb r3, [r3]
	ldr r4, .L_0808c834
	strh r3, [r4]
	b .L_0808cd76
	.2byte 0x0000
.L_0808c7f0:
	.4byte 0x00000ccc
.L_0808c7f4:
	.4byte 0x040000d4
.L_0808c7f8:
	.4byte 0x85000333
.L_0808c7fc:
	.4byte 0x00000103
.L_0808c800:
	.4byte gCell
.L_0808c804:
	.4byte 0x0000ffff
.L_0808c808:
	.4byte 0x00000001
.L_0808c80c:
	.4byte gOverlayArea
.L_0808c810:
	.4byte 0x00000109
.L_0808c814:
	.4byte 0x00000199
.L_0808c818:
	.4byte 0x0000023e
.L_0808c81c:
	.4byte gMapCellBuffer
.L_0808c820:
	.4byte 0x0000022e
.L_0808c824:
	.4byte 0x0000024a
.L_0808c828:
	.4byte 0x00000cc8
.L_0808c82c:
	.4byte 0x0000012f
.L_0808c830:
	.4byte Data_02000434
.L_0808c834:
	.4byte Data_0200042c
.L_0808c838:
	movs r1, #184
	lsls r1, r1, #1
	add r1, r8
	movs r2, #0
	ldrsh r5, [r1, r2]
	cmp r5, #0
	beq .L_0808c848
	b .L_0808c6e4
.L_0808c848:
	movs r2, #193
	lsls r2, r2, #1
	add r2, r8
	movs r3, #0
	ldrsh r6, [r2, r3]
	cmp r6, #0
	bne .L_0808c858
	b .L_0808ca0e
.L_0808c858:
	ldr r3, .L_0808c890
	ldr r4, .L_0808c88c
	add r3, r8
	strh r4, [r3]
	movs r1, #0
	ldrsh r0, [r2, r1]
	movs r2, #1
	negs r2, r2
	cmp r0, r2
	beq .L_0808c86e
	b .L_0808c9d4
.L_0808c86e:
	bl Battle_InitializeRenderObject
	bl Battle_SetObjectFlag5bWhenMode3
	movs r3, #194
	lsls r3, r3, #1
	add r3, r8
	movs r4, #0
	ldrsh r3, [r3, r4]
	movs r6, #0
	cmp r5, r3
	bcs .L_0808c8c0
	movs r5, #196
	lsls r5, r5, #1
	b .L_0808c894
.L_0808c88c:
	.4byte 0x00000001
.L_0808c890:
	.4byte 0x00000cb6
.L_0808c894:
	mov r0, r10
	movs r1, #22
	bl Func_08009080
	mov r1, r8
	ldrsh r0, [r5, r1]
	movs r1, #1
	bl Func_08015120
	ldr r0, .L_0808c8f8
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWaitFar
	movs r3, #194
	lsls r3, r3, #1
	add r3, r8
	movs r4, #0
	ldrsh r3, [r3, r4]
	adds r6, #1
	adds r5, #2
	cmp r6, r3
	bcc .L_0808c894
.L_0808c8c0:
	movs r3, #195
	lsls r3, r3, #1
	add r3, r8
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	beq .L_0808c8d0
	b .L_0808c9c0
.L_0808c8d0:
	ldr r2, .L_0808c8fc
	ldr r3, [r2]
	cmp r3, #0
	bne .L_0808c900
	movs r0, #32
	bl Func_080770c0
	cmp r0, #0
	beq .L_0808c8ec
	mov r0, r10
	movs r1, #21
	bl Func_08009080
	b .L_0808c908
.L_0808c8ec:
	mov r0, r10
	movs r1, #37
	bl Func_08009080
	b .L_0808c908
	.2byte 0x0000
.L_0808c8f8:
	.4byte 0x0000091a
.L_0808c8fc:
	.4byte Data_02000434
.L_0808c900:
	mov r0, r10
	movs r1, #19
	bl Func_08009080
.L_0808c908:
	movs r0, #59
	bl Func_080f9010
	movs r1, #1
	ldr r0, .L_0808c948
	bl UiText_ShowPositionedMessageAndWaitFar
	ldr r3, .L_0808c94c
	ldr r0, [r3]
	bl Func_08077008
	ldr r4, .L_0808c944
	adds r6, r0, #0
	strh r4, [r6, #56]
	movs r1, #1
	lsls r5, r1, #14
	adds r0, r5, #0
	movs r2, #52
	ldrsh r1, [r6, r2]
	bl FixedPoint_Ratio
	movs r3, #128
	lsls r3, r3, #7
	cmp r0, r3
	bgt .L_0808c950
	movs r5, #0
	cmp r0, #0
	blt .L_0808c950
	adds r5, r0, #0
	b .L_0808c950
.L_0808c944:
	.4byte 0x00000001
.L_0808c948:
	.4byte 0x0000091b
.L_0808c94c:
	.4byte Data_02000434
.L_0808c950:
	lsls r3, r5, #16
	strh r5, [r6, #20]
	cmp r3, #0
	bne .L_0808c964
	movs r4, #56
	ldrsh r3, [r6, r4]
	cmp r3, #0
	beq .L_0808c964
	ldr r1, .L_0808c984
	strh r1, [r6, #20]
.L_0808c964:
	movs r2, #58
	ldrsh r0, [r6, r2]
	movs r3, #54
	ldrsh r1, [r6, r3]
	lsls r0, r0, #14
	bl FixedPoint_Ratio
	movs r3, #128
	lsls r3, r3, #7
	cmp r0, r3
	bgt .L_0808c988
	movs r3, #0
	cmp r0, #0
	blt .L_0808c988
	adds r3, r0, #0
	b .L_0808c988
.L_0808c984:
	.4byte 0x00000001
.L_0808c988:
	strh r3, [r6, #22]
	lsls r3, r3, #16
	cmp r3, #0
	bne .L_0808c99c
	movs r4, #58
	ldrsh r3, [r6, r4]
	cmp r3, #0
	beq .L_0808c99c
	ldr r1, .L_0808c9c8
	strh r1, [r6, #22]
.L_0808c99c:
	movs r3, #226
	lsls r3, r3, #1
	add r3, r9
	ldrh r2, [r3]
	movs r3, #224
	lsls r3, r3, #1
	add r3, r9
	strh r2, [r3]
	movs r3, #227
	lsls r3, r3, #1
	add r3, r9
	ldrh r3, [r3]
	ldr r2, .L_0808c9cc
	strh r3, [r2]
	bl Battle_ClearObjectFlag5bWhenMode3
	ldr r7, .L_0808c9d0
	b .L_0808ce1c
.L_0808c9c0:
	bl Battle_ClearObjectFlag5bWhenMode3
	b .L_0808c9fa
	.2byte 0x0000
.L_0808c9c8:
	.4byte 0x00000001
.L_0808c9cc:
	.4byte Data_02000402
.L_0808c9d0:
	.4byte 0x000003e7
.L_0808c9d4:
	ldr r3, .L_0808caa0
	cmp r0, r3
	bne .L_0808c9e6
	bl Battle_InitializeRenderObject
	ldr r0, .L_0808caa4
	bl Map_ShowWorldMap
	b .L_0808c9fa
.L_0808c9e6:
	ldr r4, .L_0808caa8
	cmp r0, r4
	bne .L_0808c9f6
	bl Battle_InitializeRenderObject
	bl BattleFx_RunVisibilityTransition
	b .L_0808c9fa
.L_0808c9f6:
	bl BattleFx_RunKind6DescriptorAction
.L_0808c9fa:
	ldr r3, .L_0808caac
	mov r1, r11
	add r3, r8
	strh r1, [r3]
	movs r3, #193
	lsls r3, r3, #1
	add r3, r8
	mov r2, r11
	strh r2, [r3]
	b .L_0808cd76
.L_0808ca0e:
	movs r5, #190
	lsls r5, r5, #1
	add r5, r8
	movs r3, #0
	ldrsh r7, [r5, r3]
	cmp r7, #0
	beq .L_0808ca5e
	str r1, [sp, #4]
	bl Battle_InitializeRenderObject
	bl ObjectTable_Snapshot
	ldr r2, .L_0808cab0
	ldr r3, .L_0808cab4
	add r2, r9
	strh r3, [r2]
	movs r2, #224
	lsls r2, r2, #1
	movs r3, #255
	add r2, r9
	lsls r3, r3, #1
	strh r3, [r2]
	ldr r4, .L_0808cab8
	ldrh r3, [r5]
	ldr r1, [sp, #4]
	strh r3, [r4]
	ldr r3, .L_0808cabc
	strh r3, [r1]
	movs r1, #0
	ldrsh r0, [r5, r1]
	bl Func_080941e0
	movs r3, #212
	lsls r3, r3, #1
	ldr r2, .L_0808cac0
	add r3, r8
	str r6, [r3]
	strh r6, [r5]
	str r6, [r2]
	b .L_0808cd76
.L_0808ca5e:
	movs r3, #182
	lsls r3, r3, #1
	add r3, r8
	movs r4, #0
	ldrsh r6, [r3, r4]
	cmp r6, #0
	beq .L_0808ca86
	ldr r5, .L_0808caac
	ldr r1, .L_0808ca9c
	add r5, r8
	strh r1, [r5]
	movs r2, #0
	ldrsh r0, [r3, r2]
	str r3, [sp, #0]
	bl BattleAction_RunDescriptor
	ldr r3, [sp, #0]
	strh r7, [r5]
	strh r7, [r3]
	b .L_0808cd76
.L_0808ca86:
	movs r5, #183
	lsls r5, r5, #1
	add r5, r8
	movs r3, #0
	ldrsh r0, [r5, r3]
	cmp r0, #0
	beq .L_0808cac4
	bl Battle_DispatchInputEvent
	strh r6, [r5]
	b .L_0808cd76
.L_0808ca9c:
	.4byte 0x00000001
.L_0808caa0:
	.4byte 0xfffffc88
.L_0808caa4:
	.4byte 0x0000001b
.L_0808caa8:
	.4byte 0xfffffc87
.L_0808caac:
	.4byte 0x00000cb6
.L_0808cab0:
	.4byte 0x0000021e
.L_0808cab4:
	.4byte 0x0000ffff
.L_0808cab8:
	.4byte Data_02000402
.L_0808cabc:
	.4byte 0x000003e7
.L_0808cac0:
	.4byte Data_02000478
.L_0808cac4:
	movs r3, #186
	lsls r3, r3, #1
	add r3, r8
	movs r4, #0
	ldrsh r3, [r3, r4]
	cmp r3, #0
	beq .L_0808cb4c
	bl UiTimedNotice_CloseIfActiveFar
	ldr r1, .L_0808cb18
	ldr r0, [r1]
	bl BattleEffect_SelectNearbyObject
	movs r2, #1
	adds r5, r0, #0
	negs r2, r2
	movs r6, #0
	cmp r5, r2
	beq .L_0808caf6
	bl BattleFx_FindDescriptorWithOverride
	cmp r0, #0
	beq .L_0808caf4
	movs r0, #1
.L_0808caf4:
	adds r6, r0, #0
.L_0808caf6:
	cmp r6, #0
	beq .L_0808cb1c
	ldr r3, .L_0808cb14
	orrs r5, r3
	movs r3, #188
	lsls r3, r3, #1
	add r3, r8
	strh r5, [r3]
	movs r3, #185
	lsls r3, r3, #1
	add r3, r8
	mov r4, r11
	strh r4, [r3]
	b .L_0808cb44
	.2byte 0x0000
.L_0808cb14:
	.4byte 0x00001000
.L_0808cb18:
	.4byte Data_02000434
.L_0808cb1c:
	bl Object_GetTriggerTileAheadOfCurrent
	adds r6, r0, #0
	cmp r6, #0
	beq .L_0808cb3a
	movs r3, #189
	lsls r3, r3, #1
	add r3, r8
	strh r6, [r3]
	movs r3, #185
	lsls r3, r3, #1
	add r3, r8
	mov r1, r11
	strh r1, [r3]
	b .L_0808cb44
.L_0808cb3a:
	movs r3, #185
	lsls r3, r3, #1
	ldr r2, .L_0808cb48
	add r3, r8
	strh r2, [r3]
.L_0808cb44:
	movs r3, #186
	b .L_0808cd6e
.L_0808cb48:
	.4byte 0x00000001
.L_0808cb4c:
	movs r3, #185
	lsls r3, r3, #1
	add r3, r8
	movs r1, #0
	ldrsh r7, [r3, r1]
	cmp r7, #0
	beq .L_0808cbe4
	bl UiTimedNotice_CloseIfActiveFar
	bl Battle_InitializeRenderObject
	movs r0, #111
	bl Func_080f9010
	bl Battle_SetObjectFlag5bWhenMode3
	movs r0, #131
	lsls r0, r0, #1
	bl GameFlag_SetBitFar
	ldr r3, .L_0808cbd8
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_0808cb98
	ldr r1, .L_0808cbdc
	ldr r3, [r1]
	movs r2, #2
	ands r3, r2
	cmp r3, #0
	beq .L_0808cb98
	ldr r3, [r1]
	movs r2, #4
	ands r3, r2
	cmp r3, #0
	beq .L_0808cb98
	bl Menu_RunSelectionWithCursorObjectFar
	b .L_0808cbbe
.L_0808cb98:
	ldr r0, .L_0808cbe0
	bl Func_080770c0
	cmp r0, #0
	beq .L_0808cbae
	movs r2, #193
	lsls r2, r2, #1
	add r2, r8
	movs r3, #250
	strh r3, [r2]
	b .L_0808cbbe
.L_0808cbae:
	movs r5, #204
	lsls r5, r5, #4
	add r5, r8
	strh r0, [r5]
	bl Menu_RunTopSelectionFar
	ldr r2, .L_0808cbd4
	strh r2, [r5]
.L_0808cbbe:
	bl Battle_ClearObjectFlag5bWhenMode3
	movs r0, #131
	lsls r0, r0, #1
	bl GameFlag_ClearBitFar
	bl GameFlag_RefreshLureCapFar
	movs r3, #185
	b .L_0808cd6e
	.2byte 0x0000
.L_0808cbd4:
	.4byte 0x00000001
.L_0808cbd8:
	.4byte gDebugMode
.L_0808cbdc:
	.4byte Data_03001ae8
.L_0808cbe0:
	.4byte 0x00000107
.L_0808cbe4:
	movs r5, #188
	lsls r5, r5, #1
	add r5, r8
	movs r1, #0
	ldrsh r6, [r5, r1]
	cmp r6, #0
	beq .L_0808cc02
	bl Battle_SetObjectFlag5bWhenMode3
	ldrh r3, [r5]
	ldr r0, .L_0808cc20
	ands r0, r3
	bl BattleFx_RunDescriptorAction
	b .L_0808cc42
.L_0808cc02:
	movs r5, #189
	lsls r5, r5, #1
	add r5, r8
	movs r2, #0
	ldrsh r7, [r5, r2]
	cmp r7, #0
	beq .L_0808cc24
	bl Battle_SetObjectFlag5bWhenMode3
	movs r3, #0
	ldrsh r0, [r5, r3]
	bl Func_0808d9a4
	b .L_0808cc6e
	.2byte 0x0000
.L_0808cc20:
	.4byte 0x00000fff
.L_0808cc24:
	movs r5, #191
	lsls r5, r5, #1
	add r5, r8
	movs r4, #0
	ldrsh r6, [r5, r4]
	cmp r6, #0
	beq .L_0808cc4a
	bl UiTimedNotice_CloseIfActiveFar
	bl Battle_SetObjectFlag5bWhenMode3
	movs r1, #0
	ldrsh r0, [r5, r1]
	bl BattleCommand_ExecuteSelectedAction
.L_0808cc42:
	bl Battle_ClearObjectFlag5bWhenMode3
	strh r7, [r5]
	b .L_0808cd76
.L_0808cc4a:
	movs r5, #192
	lsls r5, r5, #1
	add r5, r8
	movs r2, #0
	ldrsh r3, [r5, r2]
	cmp r3, #0
	beq .L_0808cc76
	bl Battle_SetObjectFlag5bWhenMode3
	movs r3, #0
	ldrsh r0, [r5, r3]
	movs r3, #205
	lsls r3, r3, #1
	add r3, r8
	movs r4, #0
	ldrsh r1, [r3, r4]
	bl BattleCommand_ExecuteSelectedItem
.L_0808cc6e:
	bl Battle_ClearObjectFlag5bWhenMode3
	strh r6, [r5]
	b .L_0808cd76
.L_0808cc76:
	movs r3, #187
	lsls r3, r3, #1
	add r3, r8
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	beq .L_0808cd76
	movs r0, #111
	bl Func_080f9010
	bl Battle_InitializeRenderObject
	bl Battle_SetObjectFlag5bWhenMode3
	movs r0, #131
	lsls r0, r0, #1
	bl GameFlag_SetBitFar
	ldr r1, .L_0808cd40
	ldrb r3, [r1]
	cmp r3, #0
	beq .L_0808ccb4
	ldr r3, .L_0808cd44
	ldr r3, [r3]
	movs r2, #2
	ands r3, r2
	cmp r3, #0
	beq .L_0808ccb4
	bl Menu_RunSelectionFar
	b .L_0808cd60
.L_0808ccb4:
	ldrb r3, [r1]
	cmp r3, #0
	beq .L_0808ccce
	ldr r3, .L_0808cd44
	movs r2, #128
	ldr r3, [r3]
	lsls r2, r2, #2
	ands r3, r2
	cmp r3, #0
	beq .L_0808ccce
	bl Debug_RunPaletteEditor
	b .L_0808cd60
.L_0808ccce:
	ldr r0, .L_0808cd48
	bl Func_080770c0
	cmp r0, #0
	beq .L_0808cce4
	movs r2, #193
	lsls r2, r2, #1
	add r2, r8
	movs r3, #250
	strh r3, [r2]
	b .L_0808cd60
.L_0808cce4:
	bl Battle_ResetEffectCounter
	bl UiTimedNotice_CloseIfActiveFar
	bl AudioCommand_GetSecondaryStateByteFar
	ldr r3, .L_0808cd4c
	add r3, r9
	strh r0, [r3]
	movs r0, #191
	lsls r0, r0, #1
	bl Func_080770c0
	cmp r0, #0
	bne .L_0808cd58
	ldr r3, .L_0808cd50
	movs r2, #128
	ldr r5, [r3]
	ldr r1, .L_0808cd54
	movs r0, #0
	lsls r2, r2, #2
.L_0808cd0e:
	ldrb r3, [r1]
	adds r1, #1
	cmp r3, #255
	bne .L_0808cd18
	adds r0, #1
.L_0808cd18:
	subs r2, #1
	cmp r2, #0
	bne .L_0808cd0e
	adds r3, r0, #0
	subs r3, #136
	cmp r3, #0
	bge .L_0808cd30
	ldr r2, .L_0808cd3c
	movs r0, #1
	strh r2, [r5, #4]
	bl WaitFrames
.L_0808cd30:
	movs r0, #0
	bl Menu_RunWorkspaceResultLoopFar
	mov r3, r11
	strh r3, [r5, #4]
	b .L_0808cd60
.L_0808cd3c:
	.4byte 0x00000001
.L_0808cd40:
	.4byte gDebugMode
.L_0808cd44:
	.4byte Data_03001ae8
.L_0808cd48:
	.4byte 0x00000107
.L_0808cd4c:
	.4byte 0x0000021e
.L_0808cd50:
	.4byte gMenuCtrlWork
.L_0808cd54:
	.4byte ResourceBlockOwners
.L_0808cd58:
	ldr r0, .L_0808ce60
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWaitFar
.L_0808cd60:
	bl Battle_ClearObjectFlag5bWhenMode3
	movs r0, #131
	lsls r0, r0, #1
	bl GameFlag_ClearBitFar
	movs r3, #187
.L_0808cd6e:
	lsls r3, r3, #1
	add r3, r8
	mov r4, r11
	strh r4, [r3]
.L_0808cd76:
	bl BattleFx_SumCounters
	cmp r0, #0
	beq .L_0808cd80
	b .L_0808c838
.L_0808cd80:
	movs r0, #130
	lsls r0, r0, #1
	bl GameFlag_ClearBitFar
	ldr r1, .L_0808ce64
	ldr r3, [r1]
	lsls r3, r3, #2
	adds r3, #20
	mov r4, r8
	ldr r3, [r4, r3]
	ldr r2, .L_0808ce68
	mov r10, r3
	cmp r3, #0
	beq .L_0808cdd8
	movs r1, #249
	lsls r1, r1, #1
	adds r3, r2, r1
	ldrb r3, [r3]
	cmp r3, #2
	bne .L_0808cdb0
	mov r0, r10
	bl ObjectDispatch_InitFromTable0Far
	b .L_0808cdd8
.L_0808cdb0:
	cmp r3, #1
	bne .L_0808cdbc
	mov r0, r10
	bl ObjectDispatch_InitFromTable1Far
	b .L_0808cdd8
.L_0808cdbc:
	movs r3, #207
	lsls r3, r3, #1
	add r3, r8
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #3
	bne .L_0808cdd2
	mov r0, r10
	bl ObjectDispatch_InitFromTable3Far
	b .L_0808cdd8
.L_0808cdd2:
	mov r0, r10
	bl ObjectDispatch_InitFromTable2Far
.L_0808cdd8:
	ldr r5, .L_0808ce64
.L_0808cdda:
	movs r0, #1
	bl WaitFrames
	ldr r3, [r5]
	lsls r3, r3, #2
	adds r3, #20
	mov r4, r8
	ldr r3, [r4, r3]
	mov r10, r3
	ldr r3, .L_0808ce6c
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_0808cdfe
	ldr r0, .L_0808ce70
	bl Func_080770c0
	cmp r0, #0
	bne .L_0808ce12
.L_0808cdfe:
	mov r3, r10
	adds r3, #34
	mov r2, r10
	mov r4, r10
	ldrb r0, [r3]
	ldr r1, [r2, #8]
	ldr r3, [r4, #16]
	ldr r2, [r2, #12]
	bl Field_ProcessStep
.L_0808ce12:
	bl BattleFx_SumCounters
	cmp r0, #0
	beq .L_0808cdda
	b .L_0808c79c
.L_0808ce1c:
	movs r6, #227
	lsls r6, r6, #1
	add r6, r8
	ldrh r3, [r6]
	cmp r3, #0
	beq .L_0808ce46
	movs r3, #224
	movs r5, #228
	lsls r3, r3, #1
	lsls r5, r5, #1
	add r3, r8
	add r5, r8
	ldr r0, [r3]
	ldr r1, [r5]
	bl DisplayTransition_Finish
	movs r3, #0
	strh r3, [r6]
	ldr r0, [r5]
	bl WaitFrames
.L_0808ce46:
	movs r0, #27
	bl Runtime_ReleaseHeapBlock
	adds r0, r7, #0
	add sp, #16
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r1}
	bx r1
.L_0808ce60:
	.4byte 0x00000c2f
.L_0808ce64:
	.4byte Data_02000434
.L_0808ce68:
	.4byte gCell
.L_0808ce6c:
	.4byte gDebugMode
.L_0808ce70:
	.4byte 0x00000163
