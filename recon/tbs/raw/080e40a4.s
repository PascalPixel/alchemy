.syntax unified
	.thumb
	.global BattlePres_RunRingAndSparkScene
BattlePres_RunRingAndSparkScene:
	.global Unnamed_080e40a4
	.thumb_func
Unnamed_080e40a4:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, .L_080e4124
	adds r2, r3, #0
	adds r1, r0, #0
	ldmia r2!, {r0}
	ldr r2, [r2]
	sub sp, #112
	str r2, [sp, #52]
	ldr r2, [r3, #8]
	str r2, [sp, #44]
	subs r3, #108
	ldr r3, [r3]
	str r3, [sp, #40]
	ldr r3, [r1]
	mov r11, r0
	movs r6, #1
	cmp r3, #199
	bgt .L_080e40d6
	movs r6, #0
.L_080e40d6:
	ldr r5, .L_080e4128
	add r5, r11
	movs r2, #130
	ldr r0, [r1, #8]
	str r1, [r5]
	ldr r1, [r1, #12]
	bl BattlePres_SetupTransitionAtPairMidpointFar
	movs r0, #1
	bl WaitFrames
	bl BattleFx_SetupCanvasTileMap
	ldr r3, .L_080e4120
	ldr r2, .L_080e412c
	strh r3, [r2]
	ldr r3, [r5]
	ldr r3, [r3, #4]
	cmp r3, #0
	bne .L_080e4130
	movs r3, #3
	movs r1, #7
	movs r2, #7
	movs r0, #46
	str r3, [sp, #0]
	bl BattleEffect_LoadWork
	movs r3, #2
	str r3, [sp, #0]
	movs r0, #47
	movs r1, #7
	movs r2, #7
	movs r3, #3
	bl BattleEffect_LoadWork
	b .L_080e4150
	.2byte 0x0000
.L_080e4120:
	.4byte 0x00001f80
.L_080e4124:
	.4byte gBattleFxWork
.L_080e4128:
	.4byte 0x00007828
.L_080e412c:
	.4byte 0x0400000a
.L_080e4130:
	movs r3, #3
	str r3, [sp, #0]
	movs r1, #7
	movs r2, #7
	movs r3, #7
	movs r0, #46
	bl BattleEffect_LoadWork
	movs r3, #2
	str r3, [sp, #0]
	movs r0, #47
	movs r1, #7
	movs r2, #7
	movs r3, #7
	bl BattleEffect_LoadWork
.L_080e4150:
	ldr r3, .L_080e4424
	adds r2, r3, #0
	adds r2, #184
	ldr r2, [r2]
	str r2, [sp, #56]
	adds r3, #188
	mov r5, sp
	ldr r3, [r3]
	adds r5, #56
	str r5, [sp, #16]
	str r3, [r5, #4]
	ldr r5, .L_080e4428
	add r5, r11
	ldr r3, [r5]
	movs r2, #130
	ldr r0, [r3, #8]
	ldr r1, [r3, #12]
	bl BattlePres_SetupTransitionAtPairMidpointFar
	movs r0, #1
	bl WaitFrames
	ldr r0, .L_080e442c
	mov r1, r11
	movs r2, #1
	movs r3, #0
	bl Resource_LoadAndDecompress
	ldr r3, [r5]
	movs r2, #130
	ldr r0, [r3, #8]
	ldr r1, [r3, #12]
	bl BattlePres_SetupTransitionAtPairMidpointFar
	movs r0, #1
	bl WaitFrames
	movs r2, #1
	movs r3, #1
	ldr r0, .L_080e4430
	ldr r1, .L_080e4434
	bl Resource_LoadAndDecompress
	ldr r2, [r5]
	ldr r3, [r2, #8]
	cmp r3, #7
	ble .L_080e41c4
	ldr r0, .L_080e4438
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	movs r2, #128
	ldr r3, .L_080e443c
	lsls r0, r0, #19
	bl _call_via_r3
	ldr r2, [r5]
.L_080e41c4:
	ldr r1, [r2, #12]
	ldr r0, [r2, #8]
	movs r2, #130
	bl BattlePres_SetupTransitionAtPairMidpointFar
	movs r0, #1
	bl WaitFrames
	ldr r0, .L_080e4440
	ldr r1, [sp, #44]
	movs r2, #0
	movs r3, #0
	bl Resource_LoadAndDecompress
	ldr r3, [r5]
	movs r2, #130
	ldr r0, [r3, #8]
	ldr r1, [r3, #12]
	bl BattlePres_SetupTransitionAtPairMidpointFar
	movs r0, #1
	bl WaitFrames
	movs r2, #239
	lsls r2, r2, #7
	add r2, r11
	movs r3, #1
	str r3, [r2]
	ldr r2, .L_080e4444
	movs r3, #0
	add r2, r11
	movs r1, #144
	str r3, [r2]
	ldr r0, .L_080e4448
	lsls r1, r1, #3
	bl Scheduler_AddOrUpdateCallback
	cmp r6, #1
	beq .L_080e4214
	b .L_080e43cc
.L_080e4214:
	ldr r3, [r5]
	ldr r0, [r3, #8]
	bl GetBattleObjectSlotFar
	ldr r0, [r0]
	movs r5, #225
	mov r8, r0
	lsls r5, r5, #7
	movs r0, #0
	ldr r6, .L_080e444c
	mov r10, r0
	movs r7, #0
	add r5, r11
.L_080e422e:
	bl Random16
	movs r3, #63
	ands r3, r0
	adds r3, #16
	str r3, [r5]
	str r7, [r5, #4]
	str r7, [r5, #8]
	bl Random16
	ands r0, r6
	str r0, [r5, #12]
	bl Random16
	ands r0, r6
	str r0, [r5, #16]
	bl Random16
	movs r1, #1
	add r10, r1
	ands r0, r6
	mov r2, r10
	str r0, [r5, #20]
	adds r5, #28
	cmp r2, #64
	bne .L_080e422e
	mov r0, r8
	movs r1, #0
	bl ObjectDispatch_ApplyValueToChildrenFar
	mov r3, r8
	ldr r3, [r3, #36]
	str r3, [sp, #36]
	mov r5, r8
	ldr r5, [r5, #40]
	str r5, [sp, #32]
	mov r0, r8
	ldr r0, [r0, #44]
	str r0, [sp, #28]
	mov r1, r8
	ldr r1, [r1, #72]
	str r1, [sp, #20]
	mov r2, r8
	ldr r2, [r2, #52]
	movs r5, #0
	mov r3, r8
	str r2, [sp, #24]
	str r5, [r3, #36]
	str r5, [r3, #40]
	str r5, [r3, #44]
	str r5, [r3, #52]
	str r5, [r3, #72]
	ldr r3, .L_080e4428
	add r3, r11
	ldr r3, [r3]
	mov r1, sp
	ldr r0, [r3, #8]
	adds r1, #100
	str r1, [sp, #12]
	bl EffectPosition_ApplyStepAndYOffset
	ldr r2, [sp, #12]
	ldr r3, [r2]
	mov r0, r10
	subs r0, r0, r3
	str r0, [sp, #48]
	add r1, sp, #48
	ldr r2, .L_080e4450
	ldrh r1, [r1]
	movs r3, #80
	strh r1, [r2, #4]
	strh r3, [r2, #6]
	ldr r2, .L_080e4454
	movs r3, #24
	add r2, r11
	str r3, [r2]
	ldr r3, .L_080e4458
	movs r1, #200
	add r3, r11
	str r5, [r3]
	lsls r1, r1, #4
	ldr r0, .L_080e445c
	bl Scheduler_AddOrUpdateCallback
	movs r0, #212
	bl AudioCommand_PlayFar
	mov r9, r5
.L_080e42de:
	ldr r3, .L_080e4428
	add r3, r11
	ldr r3, [r3]
	movs r2, #130
	ldr r0, [r3, #8]
	ldr r1, [r3, #12]
	movs r6, #225
	bl BattlePres_SetupTransitionAtPairMidpointFar
	lsls r6, r6, #7
	movs r2, #0
	mov r10, r2
	add r6, r11
.L_080e42f8:
	ldr r3, [r6]
	cmp r3, #0
	blt .L_080e4384
	mov r3, r10
	cmp r3, #0
	bge .L_080e4306
	adds r3, #3
.L_080e4306:
	asrs r3, r3, #2
	cmp r9, r3
	blt .L_080e4384
	mov r5, r10
	movs r3, #1
	ands r3, r5
	adds r7, r3, #5
	bl Render_ResetTransformState
	ldr r0, [r6, #20]
	bl SceneTransform_ApplyRoll
	ldr r0, [r6, #12]
	bl SceneTransform_ApplyPitch
	ldr r0, [r6, #16]
	bl SceneTransform_ApplyYaw
	add r5, sp, #76
	adds r0, r6, #0
	adds r1, r5, #0
	bl EffectPosition_ApplyBaseAndYOffset
	ldr r3, [r5]
	adds r3, #64
	str r3, [r5]
	ldr r2, [sp, #104]
	ldr r3, [r5, #4]
	adds r3, r3, r2
	adds r3, #24
	str r3, [r5, #4]
	movs r2, #60
	ldr r3, [r5, #8]
	negs r2, r2
	cmp r3, r2
	bge .L_080e4352
	str r2, [r5, #8]
	adds r3, r2, #0
.L_080e4352:
	cmp r3, #60
	ble .L_080e435a
	movs r3, #60
	str r3, [r5, #8]
.L_080e435a:
	lsls r0, r7, #1
	ldr r2, .L_080e4460
	adds r3, #60
	str r3, [r5, #8]
	subs r3, r0, #2
	ldrh r1, [r2, r3]
	ldr r2, [sp, #44]
	ldr r3, [r5, #4]
	adds r1, r2, r1
	ldr r2, [r5]
	subs r3, r3, r7
	str r0, [sp, #0]
	str r0, [sp, #4]
	subs r2, r2, r7
	ldr r4, [sp, #56]
	ldr r0, [sp, #52]
	bl _call_via_r4
	ldr r3, [r6]
	subs r3, #4
	str r3, [r6]
.L_080e4384:
	movs r3, #1
	add r10, r3
	mov r5, r10
	adds r6, #28
	cmp r5, #64
	bne .L_080e42f8
	ldr r2, .L_080e4464
	add r2, r11
	movs r0, #1
	str r3, [r2]
	bl WaitFrames
	movs r0, #1
	add r9, r0
	mov r1, r9
	cmp r1, #32
	bne .L_080e42de
	ldr r0, .L_080e445c
	bl Scheduler_RemoveCallback
	mov r0, r8
	movs r1, #16
	bl ObjectDispatch_ApplyValueToChildrenFar
	ldr r2, [sp, #36]
	mov r3, r8
	str r2, [r3, #36]
	ldr r5, [sp, #32]
	str r5, [r3, #40]
	ldr r0, [sp, #28]
	str r0, [r3, #44]
	ldr r1, [sp, #24]
	str r1, [r3, #52]
	ldr r2, [sp, #20]
	str r2, [r3, #72]
	b .L_080e43d2
.L_080e43cc:
	mov r3, sp
	adds r3, #100
	str r3, [sp, #12]
.L_080e43d2:
	movs r1, #128
	ldr r5, .L_080e4468
	ldr r0, [sp, #52]
	lsls r1, r1, #7
	bl _call_via_r5
	movs r1, #128
	lsls r1, r1, #7
	ldr r0, .L_080e446c
	bl _call_via_r5
	movs r2, #239
	lsls r2, r2, #7
	add r2, r11
	movs r3, #2
	str r3, [r2]
	ldr r2, .L_080e4444
	movs r3, #75
	add r2, r11
	ldr r5, .L_080e4428
	str r3, [r2]
	ldr r2, .L_080e4470
	ldr r3, .L_080e4420
	add r5, r11
	strh r3, [r2]
	ldr r3, [r5]
	add r6, sp, #88
	movs r1, #36
	ldrsh r0, [r3, r1]
	adds r1, r6, #0
	bl EffectPosition_ApplyStepAndYOffset
	ldr r3, [r5]
	ldr r3, [r3, #4]
	cmp r3, #0
	bne .L_080e4474
	ldr r2, [r6]
	movs r3, #32
	b .L_080e4478
.L_080e4420:
	.4byte 0x00001f81
.L_080e4424:
	.4byte gWorkSlot
.L_080e4428:
	.4byte 0x00007828
.L_080e442c:
	.4byte 0x00000049
.L_080e4430:
	.4byte 0x0000004a
.L_080e4434:
	.4byte gMapCellBuffer
.L_080e4438:
	.4byte 0x0000008e
.L_080e443c:
	.4byte IwramCopyWords
.L_080e4440:
	.4byte 0x00000076
.L_080e4444:
	.4byte 0x00007784
.L_080e4448:
	.4byte BattlePresentation_ProcessPendingGraphicsTransfer
.L_080e444c:
	.4byte 0x0000ffff
.L_080e4450:
	.4byte gBgScroll
.L_080e4454:
	.4byte 0x000077b4
.L_080e4458:
	.4byte 0x000077b8
.L_080e445c:
	.4byte Palette_StepFadeTransfer
.L_080e4460:
	.4byte BattleFx6_FlareCells
.L_080e4464:
	.4byte 0x00007824
.L_080e4468:
	.4byte IwramClearWords
.L_080e446c:
	.4byte 0x06004000
.L_080e4470:
	.4byte 0x0400000a
.L_080e4474:
	ldr r2, [r6]
	movs r3, #96
.L_080e4478:
	subs r3, r3, r2
	str r3, [sp, #48]
	ldr r2, [sp, #48]
	cmp r2, #0
	ble .L_080e4486
	movs r3, #0
	str r3, [sp, #48]
.L_080e4486:
	movs r3, #128
	ldr r5, [sp, #48]
	negs r3, r3
	cmp r5, r3
	bge .L_080e4492
	str r3, [sp, #48]
.L_080e4492:
	ldr r0, [sp, #48]
	ldr r3, [r6]
	ldr r5, .L_080e46cc
	ldr r2, .L_080e46d0
	adds r3, r3, r0
	str r3, [r6]
	add r5, r11
	add r1, sp, #48
	movs r3, #80
	strh r3, [r2, #6]
	ldrh r1, [r1]
	ldr r3, [r5]
	strh r1, [r2, #4]
	movs r2, #36
	ldrsh r0, [r3, r2]
	bl GetBattleObjectSlotFar
	ldr r3, [r5]
	ldr r6, [r0]
	movs r5, #36
	ldrsh r0, [r3, r5]
	bl Battle_GetObjectTableValueFar
	lsrs r3, r0, #31
	adds r0, r0, r3
	asrs r0, r0, #1
	movs r5, #225
	mov r8, r0
	lsls r5, r5, #7
	movs r0, #0
	mov r10, r0
	movs r7, #255
	add r5, r11
.L_080e44d4:
	ldr r3, [r6, #8]
	str r3, [r5]
	ldr r3, [r6, #12]
	add r3, r8
	str r3, [r5, #4]
	ldr r3, [r6, #16]
	str r3, [r5, #8]
	bl Random16
	ands r0, r7
	lsls r0, r0, #10
	str r0, [r5, #12]
	bl Random16
	ands r0, r7
	lsls r0, r0, #10
	str r0, [r5, #16]
	bl Random16
	ands r0, r7
	subs r0, #127
	ldr r3, [r5]
	lsls r0, r0, #10
	str r0, [r5, #20]
	cmp r3, #0
	ble .L_080e450e
	ldr r3, [r5, #12]
	negs r3, r3
	str r3, [r5, #12]
.L_080e450e:
	movs r1, #1
	mov r3, r10
	add r10, r1
	adds r3, #16
	mov r2, r10
	str r3, [r5, #24]
	adds r5, #28
	cmp r2, #64
	bne .L_080e44d4
	ldr r5, [sp, #40]
	adds r5, #12
	movs r3, #0
	str r5, [sp, #8]
	mov r9, r3
.L_080e452a:
	mov r0, r9
	cmp r0, #5
	bne .L_080e4536
	movs r0, #134
	bl BattleEventRuntime_BeginPhaseFar
.L_080e4536:
	mov r1, r9
	cmp r1, #4
	bne .L_080e454c
	ldr r3, .L_080e46cc
	add r3, r11
	ldr r3, [r3]
	movs r1, #0
	movs r2, #36
	ldrsh r0, [r3, r2]
	bl BattleMotion_ApplyVariantMotionFar
.L_080e454c:
	ldr r3, .L_080e46cc
	add r3, r11
	ldr r3, [r3]
	ldr r1, [sp, #12]
	ldr r0, [r3, #8]
	bl EffectPosition_ApplyStepAndYOffset
	ldr r5, [sp, #12]
	ldr r3, [r5, #4]
	mov r0, r9
	adds r3, #16
	str r3, [r5, #4]
	cmp r0, #1
	bgt .L_080e4576
	movs r1, #120
	str r1, [sp, #0]
	str r1, [sp, #4]
	ldr r4, [sp, #56]
	ldr r0, [sp, #52]
	mov r1, r11
	b .L_080e45a0
.L_080e4576:
	mov r2, r9
	cmp r2, #3
	bgt .L_080e458e
	movs r1, #225
	movs r3, #120
	lsls r1, r1, #6
	str r3, [sp, #0]
	str r3, [sp, #4]
	ldr r4, [sp, #56]
	ldr r0, [sp, #52]
	add r1, r11
	b .L_080e45a0
.L_080e458e:
	mov r5, r9
	cmp r5, #5
	bgt .L_080e45aa
	movs r0, #120
	str r0, [sp, #0]
	str r0, [sp, #4]
	ldr r4, [sp, #56]
	ldr r0, [sp, #52]
	ldr r1, .L_080e46d4
.L_080e45a0:
	movs r2, #0
	movs r3, #0
	bl _call_via_r4
	b .L_080e45c4
.L_080e45aa:
	mov r1, r9
	cmp r1, #7
	bgt .L_080e45c4
	movs r2, #120
	str r2, [sp, #0]
	str r2, [sp, #4]
	ldr r4, [sp, #56]
	ldr r0, [sp, #52]
	ldr r1, .L_080e46d8
	movs r2, #0
	movs r3, #0
	bl _call_via_r4
.L_080e45c4:
	bl Render_ResetTransformState
	ldr r0, [sp, #40]
	ldr r1, [sp, #8]
	bl Graphics_PrepareTransferInIwramWork
	mov r3, r9
	subs r3, #4
	cmp r3, #27
	bhi .L_080e465a
	movs r5, #64
	movs r3, #0
	add r5, sp
	mov r10, r3
	mov r8, r5
.L_080e45e2:
	mov r0, r10
	lsrs r3, r0, #31
	add r3, r10
	asrs r5, r3, #1
	lsls r3, r5, #3
	subs r3, r3, r5
	lsls r3, r3, #2
	movs r1, #225
	add r3, r11
	lsls r1, r1, #7
	adds r7, r3, r1
	ldr r6, [r7, #24]
	cmp r6, #0
	ble .L_080e4650
	mov r1, r8
	adds r0, r7, #0
	bl EffectPosition_ApplyBaseAndYOffset
	mov r3, r8
	ldr r2, [r3]
	ldr r0, [sp, #48]
	adds r2, r2, r0
	str r2, [r3]
	asrs r6, r6, #3
	ldr r3, [r3, #4]
	adds r6, #2
	movs r0, #1
	lsls r4, r6, #1
	adds r3, #16
	mov r1, r8
	ands r0, r5
	ldr r5, .L_080e46dc
	str r3, [r1, #4]
	subs r1, r4, #2
	ldrh r1, [r5, r1]
	ldr r5, [sp, #44]
	str r4, [sp, #0]
	adds r1, r5, r1
	str r4, [sp, #4]
	ldr r5, [sp, #16]
	lsls r0, r0, #2
	subs r3, r3, r6
	ldr r4, [r0, r5]
	subs r2, r2, r6
	ldr r0, [sp, #52]
	bl _call_via_r4
	adds r0, r7, #0
	movs r1, #60
	ldr r2, .L_080e46e0
	bl EffectStep_AdvanceWithGravity3D
	ldr r3, [r7, #24]
	subs r3, #1
	str r3, [r7, #24]
.L_080e4650:
	movs r0, #1
	add r10, r0
	mov r1, r10
	cmp r1, #64
	bne .L_080e45e2
.L_080e465a:
	ldr r2, .L_080e46e4
	movs r3, #1
	add r2, r11
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	movs r2, #1
	add r9, r2
	mov r3, r9
	cmp r3, #32
	beq .L_080e4674
	b .L_080e452a
.L_080e4674:
	ldr r0, .L_080e46e8
	bl Scheduler_RemoveCallback
	movs r0, #47
	bl Runtime_ReleaseHeapBlock
	movs r0, #46
	bl Runtime_ReleaseHeapBlock
	ldr r3, .L_080e46d0
	mov r5, r9
	strh r5, [r3, #6]
	ldr r3, .L_080e46ec
	movs r1, #201
	ldr r3, [r3]
	movs r0, #0
	lsls r1, r1, #3
	mov r9, r0
	adds r5, r3, r1
	movs r6, #6
.L_080e469c:
	mov r2, r9
	ldrh r0, [r5]
	subs r1, r6, r2
	bl BattlePresentation_SetPaletteLevelFar
	movs r0, #1
	bl WaitFrames
	movs r3, #1
	add r9, r3
	mov r0, r9
	cmp r0, #7
	bne .L_080e469c
	bl BattleFx_SetTransitionFlagAndDisplay
	add sp, #112
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
.L_080e46cc:
	.4byte 0x00007828
.L_080e46d0:
	.4byte gBgScroll
.L_080e46d4:
	.4byte gMapCellBuffer
.L_080e46d8:
	.4byte gMapCellBuffer + 0x3840
.L_080e46dc:
	.4byte BattleFx6_FlareCells
.L_080e46e0:
	.4byte 0xfffffc00
.L_080e46e4:
	.4byte 0x00007824
.L_080e46e8:
	.4byte BattlePresentation_ProcessPendingGraphicsTransfer
.L_080e46ec:
	.4byte gBattleWork
