.syntax unified
	.thumb
	.global Unnamed_080d33c0
	.thumb_func
Unnamed_080d33c0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r6, .L_080d3464
	adds r3, r6, #0
	ldmia r3!, {r7}
	ldr r1, .L_080d3468
	ldr r3, [r3]
	sub sp, #36
	adds r1, r1, r7
	str r3, [sp, #20]
	str r0, [r1]
	movs r0, #1
	mov r8, r1
	bl BattleFx_BeginCanvasLayer
	ldr r0, .L_080d346c
	bl Resource_GetTableEntry
	adds r5, r0, #0
	movs r0, #160
	ldr r3, .L_080d3470
	adds r1, r5, #0
	movs r2, #128
	adds r5, #128
	lsls r0, r0, #19
	bl _call_via_r3
	adds r0, r5, #0
	adds r1, r7, #0
	bl Resource_DecodeType01
	movs r5, #2
	movs r1, #7
	movs r2, #7
	movs r3, #3
	movs r0, #46
	str r5, [sp, #0]
	bl BattleEffect_LoadWork
	ldr r2, [r6, #28]
	movs r1, #7
	str r2, [sp, #12]
	movs r3, #15
	movs r2, #7
	movs r0, #47
	str r5, [sp, #0]
	bl BattleEffect_LoadWork
	ldr r2, .L_080d3474
	ldr r3, .L_080d3460
	mov r4, r8
	strh r3, [r2]
	ldr r3, [r4]
	ldr r6, [r6, #32]
	ldr r0, [r3, #8]
	str r6, [sp, #16]
	bl GetBattleObjectSlotFar
	ldr r6, [r0]
	mov r0, r8
	ldr r3, [r0]
	ldr r0, [r3, #8]
	bl Battle_GetObjectTableValueFar
	ldr r3, [r6, #12]
	adds r3, r3, r0
	mov r9, r3
	movs r3, #225
	movs r1, #0
	movs r2, #255
	lsls r3, r3, #7
	mov r8, r1
	mov r10, r2
	adds r5, r7, r3
	b .L_080d3478
.L_080d3460:
	.4byte 0x00000f0f
.L_080d3464:
	.4byte gBattleFxWork
.L_080d3468:
	.4byte 0x00007828
.L_080d346c:
	.4byte 0x000000cd
.L_080d3470:
	.4byte IwramCopyWords
.L_080d3474:
	.4byte 0x04000052
.L_080d3478:
	ldr r3, [r6, #8]
	mov r4, r9
	str r4, [r5, #4]
	str r3, [r5]
	ldr r3, [r6, #16]
	str r3, [r5, #8]
	bl Random16
	mov r1, r10
	ands r0, r1
	subs r0, #127
	lsls r0, r0, #16
	asrs r0, r0, #5
	str r0, [r5, #12]
	bl Random16
	movs r3, #127
	ands r3, r0
	subs r3, #16
	lsls r3, r3, #16
	asrs r3, r3, #6
	str r3, [r5, #16]
	bl Random16
	mov r2, r10
	ands r0, r2
	subs r0, #127
	lsls r0, r0, #16
	asrs r0, r0, #5
	movs r3, #1
	movs r1, #1
	str r0, [r5, #20]
	negs r3, r3
	mov r0, r8
	ldr r4, .L_080d3824
	add r8, r1
	str r3, [r5, #24]
	mov r2, r8
	movs r3, #0
	strb r3, [r4, r0]
	adds r5, #28
	cmp r2, #30
	bne .L_080d3478
	ldr r4, .L_080d3828
	movs r3, #0
	adds r2, r7, r4
	mov r8, r3
	ldr r3, [r2]
	ldr r3, [r3, #20]
	cmp r3, #0
	beq .L_080d3512
	movs r0, #232
	lsls r0, r0, #7
	adds r6, r2, #0
	movs r1, #36
	adds r5, r7, r0
.L_080d34e8:
	ldr r3, [r6]
	ldrsh r0, [r3, r1]
	str r1, [sp, #8]
	bl GetBattleObjectSlotFar
	ldr r2, [r0]
	ldr r3, [r2, #8]
	str r3, [r5]
	movs r3, #0
	str r3, [r5, #4]
	ldr r3, [r2, #16]
	str r3, [r5, #8]
	movs r3, #1
	add r8, r3
	ldr r3, [r6]
	ldr r1, [sp, #8]
	ldr r3, [r3, #20]
	adds r1, #2
	adds r5, #28
	cmp r8, r3
	bne .L_080d34e8
.L_080d3512:
	ldr r4, .L_080d382c
	ldr r0, .L_080d3830
	adds r3, r7, r4
	movs r2, #0
	movs r5, #144
	str r2, [r3]
	lsls r5, r5, #3
	adds r3, r7, r0
	str r2, [r3]
	adds r1, r5, #0
	ldr r0, .L_080d3834
	bl Scheduler_AddOrUpdateCallback
	movs r1, #239
	lsls r1, r1, #7
	adds r2, r7, r1
	movs r3, #2
	str r3, [r2]
	ldr r3, .L_080d3838
	adds r2, r7, r3
	movs r3, #75
	str r3, [r2]
	adds r1, r5, #0
	ldr r0, .L_080d383c
	bl Scheduler_AddOrUpdateCallback
	movs r0, #164
	bl AudioCommand_PlayFar
	ldr r0, .L_080d3828
	adds r3, r7, r0
	ldr r3, [r3]
	ldr r3, [r3, #24]
	ldr r2, .L_080d3840
	lsls r3, r3, #1
	adds r3, #1
	ldrb r3, [r2, r3]
	movs r4, #0
	mov r11, r4
	cmp r3, #0
	bne .L_080d3566
	b .L_080d37f4
.L_080d3566:
	ldr r3, .L_080d3844
	ldr r6, [r3]
	mov r3, r11
	subs r3, #17
	cmp r3, #46
	bhi .L_080d357c
	ldr r1, .L_080d382c
	movs r3, #192
	adds r2, r7, r1
	lsls r3, r3, #1
	b .L_080d3582
.L_080d357c:
	ldr r3, .L_080d382c
	adds r2, r7, r3
	movs r3, #0
.L_080d3582:
	str r3, [r2]
	ldr r0, .L_080d3828
	adds r5, r7, r0
	ldr r3, [r5]
	ldr r3, [r3, #24]
	ldr r4, .L_080d3840
	lsls r3, r3, #1
	adds r3, #1
	ldrb r3, [r4, r3]
	subs r3, #16
	mov r10, r4
	cmp r11, r3
	bne .L_080d35a2
	movs r0, #132
	bl BattleEventRuntime_BeginPhaseFar
.L_080d35a2:
	bl Render_ResetTransformState
	adds r1, r6, #0
	adds r1, #12
	adds r0, r6, #0
	bl Graphics_PrepareTransferInIwramWork
	ldr r3, [r5]
	ldr r3, [r3, #24]
	mov r2, r10
	lsls r3, r3, #1
	ldrb r3, [r2, r3]
	movs r1, #0
	mov r8, r1
	cmp r3, #0
	bne .L_080d35c4
	b .L_080d37ac
.L_080d35c4:
	movs r3, #225
	lsls r3, r3, #7
	mov r10, r5
	mov r9, r1
	adds r6, r7, r3
.L_080d35ce:
	cmp r11, r9
	ble .L_080d3634
	ldr r4, .L_080d3824
	mov r0, r8
	ldrsb r3, [r4, r0]
	cmp r3, #0
	bne .L_080d3634
	add r5, sp, #24
	adds r0, r6, #0
	adds r1, r5, #0
	bl EffectPosition_ApplyBaseAndYOffset
	ldr r3, [r5]
	asrs r3, r3, #1
	str r3, [r5]
	ldr r3, [r5, #8]
	cmp r3, #159
	bgt .L_080d35f6
	movs r3, #160
	str r3, [r5, #8]
.L_080d35f6:
	ldr r2, .L_080d3848
	cmp r3, r2
	ble .L_080d35fe
	str r2, [r5, #8]
.L_080d35fe:
	ldr r2, [r5]
	ldr r3, [r5, #4]
	movs r1, #12
	movs r4, #192
	str r1, [sp, #0]
	lsls r4, r4, #4
	movs r1, #24
	subs r2, #6
	subs r3, #12
	str r1, [sp, #4]
	ldr r0, [sp, #20]
	adds r1, r7, r4
	ldr r4, [sp, #12]
	bl _call_via_r4
	ldr r3, [r6]
	ldr r2, [r6, #12]
	adds r3, r3, r2
	str r3, [r6]
	ldr r2, [r6, #16]
	ldr r3, [r6, #4]
	adds r3, r3, r2
	str r3, [r6, #4]
	ldr r2, [r6, #20]
	ldr r3, [r6, #8]
	adds r3, r3, r2
	str r3, [r6, #8]
.L_080d3634:
	mov r3, r9
	adds r3, #48
	cmp r11, r3
	ble .L_080d3730
	ldr r0, .L_080d3824
	mov r1, r8
	ldrsb r3, [r0, r1]
	cmp r3, #0
	bne .L_080d3730
	mov r2, r10
	ldr r3, [r2]
	mov r0, r8
	ldr r1, [r3, #20]
	bl __modsi3
	lsls r1, r0, #3
	subs r1, r1, r0
	lsls r1, r1, #2
	movs r3, #232
	lsls r3, r3, #7
	adds r1, r7, r1
	adds r1, r1, r3
	ldr r3, [r1]
	ldr r2, [r6]
	subs r3, r3, r2
	ldr r2, [r6, #12]
	asrs r3, r3, #9
	adds r0, r2, r3
	str r0, [r6, #12]
	ldr r2, [r6, #4]
	ldr r3, [r1, #4]
	subs r3, r3, r2
	ldr r2, [r6, #16]
	asrs r3, r3, #9
	adds r4, r2, r3
	str r4, [r6, #16]
	ldr r2, [r6, #8]
	ldr r3, [r1, #8]
	subs r3, r3, r2
	ldr r2, [r6, #20]
	asrs r3, r3, #9
	adds r1, r2, r3
	mov r3, r9
	adds r3, #85
	str r1, [r6, #20]
	cmp r11, r3
	bge .L_080d36c2
	lsls r3, r0, #4
	subs r3, r3, r0
	lsls r2, r3, #2
	cmp r2, #0
	bge .L_080d369e
	adds r2, #63
.L_080d369e:
	asrs r3, r2, #6
	str r3, [r6, #12]
	lsls r3, r4, #4
	subs r3, r3, r4
	lsls r2, r3, #2
	cmp r2, #0
	bge .L_080d36ae
	adds r2, #63
.L_080d36ae:
	asrs r3, r2, #6
	str r3, [r6, #16]
	lsls r3, r1, #4
	subs r3, r3, r1
	lsls r2, r3, #2
	cmp r2, #0
	bge .L_080d36be
	adds r2, #63
.L_080d36be:
	asrs r3, r2, #6
	str r3, [r6, #20]
.L_080d36c2:
	ldr r3, [r6, #4]
	cmp r3, #0
	bge .L_080d3730
	ldr r4, .L_080d3824
	movs r3, #1
	mov r0, r8
	strb r3, [r4, r0]
	movs r3, #0
	str r3, [r6, #24]
	add r5, sp, #24
	ldr r3, [r5]
	str r3, [r6]
	bl Random16
	ldr r3, [r5, #4]
	movs r2, #31
	ands r2, r0
	adds r3, r3, r2
	subs r3, #16
	mov r1, r10
	ldr r5, [r1]
	str r3, [r6, #4]
	mov r0, r8
	ldr r1, [r5, #20]
	bl __modsi3
	adds r3, r0, #0
	lsls r2, r3, #1
	adds r2, #36
	movs r1, #4
	ldrsh r0, [r5, r2]
	str r1, [sp, #0]
	movs r2, #5
	movs r1, #7
	bl ObjectGroup_UpdateMembers
	mov r2, r10
	ldr r5, [r2]
	mov r0, r8
	ldr r1, [r5, #20]
	bl __modsi3
	lsls r0, r0, #1
	adds r0, #36
	ldrsh r0, [r5, r0]
	movs r1, #0
	bl BattleMotion_ApplyVariantMotionFar
	ldr r4, .L_080d384c
	movs r0, #4
	adds r3, r7, r4
	str r0, [r3]
	movs r0, #132
	bl AudioCommand_PlayFar
.L_080d3730:
	ldr r3, [r6, #24]
	cmp r3, #15
	bhi .L_080d3790
	lsrs r0, r3, #31
	adds r0, r3, r0
	movs r1, #3
	asrs r0, r0, #1
	bl __modsi3
	ldr r2, [r6]
	ldr r3, [r6, #4]
	adds r1, r0, #0
	movs r4, #16
	movs r0, #64
	lsls r1, r1, #10
	str r4, [sp, #0]
	str r0, [sp, #4]
	ldr r4, [sp, #12]
	subs r2, #16
	ldr r0, [sp, #20]
	adds r1, r7, r1
	subs r3, #56
	bl _call_via_r4
	ldr r0, [r6, #24]
	lsrs r3, r0, #31
	adds r0, r0, r3
	movs r1, #3
	asrs r0, r0, #1
	bl __modsi3
	ldr r3, [r6, #4]
	adds r1, r0, #0
	movs r4, #64
	movs r0, #16
	lsls r1, r1, #10
	ldr r2, [r6]
	subs r3, #56
	str r0, [sp, #0]
	str r4, [sp, #4]
	adds r1, r7, r1
	ldr r0, [sp, #20]
	ldr r4, [sp, #16]
	bl _call_via_r4
	ldr r3, [r6, #24]
	adds r3, #1
	str r3, [r6, #24]
.L_080d3790:
	mov r4, r10
	ldr r3, [r4]
	ldr r3, [r3, #24]
	ldr r2, .L_080d3840
	lsls r3, r3, #1
	movs r1, #1
	ldrb r3, [r2, r3]
	movs r0, #2
	add r8, r1
	add r9, r0
	adds r6, #28
	cmp r8, r3
	beq .L_080d37ac
	b .L_080d35ce
.L_080d37ac:
	ldr r0, .L_080d3828
	adds r5, r7, r0
	ldr r3, [r5]
	ldr r1, [r3, #24]
	lsls r1, r1, #1
	adds r1, #2
	adds r0, r1, #0
	bl Camera_ApplyShake
	ldr r1, .L_080d3830
	adds r2, r7, r1
	ldr r3, [r2]
	cmp r3, #0
	bne .L_080d37cc
	movs r3, #1
	str r3, [r2]
.L_080d37cc:
	bl ObjectGroup_TickMemberTimers
	ldr r3, .L_080d3850
	adds r2, r7, r3
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r3, [r5]
	ldr r3, [r3, #24]
	ldr r2, .L_080d3840
	lsls r3, r3, #1
	adds r3, #1
	movs r4, #1
	ldrb r3, [r2, r3]
	add r11, r4
	cmp r11, r3
	beq .L_080d37f4
	b .L_080d3566
.L_080d37f4:
	ldr r0, .L_080d3834
	bl Scheduler_RemoveCallback
	ldr r0, .L_080d383c
	bl Scheduler_RemoveCallback
	movs r0, #47
	bl Runtime_ReleaseHeapBlock
	movs r0, #46
	bl Runtime_ReleaseHeapBlock
	bl BattleFx_EndCanvasLayer
	add sp, #36
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
.L_080d3824:
	.4byte gMapCellBuffer
.L_080d3828:
	.4byte 0x00007828
.L_080d382c:
	.4byte 0x000077ac
.L_080d3830:
	.4byte 0x000077b0
.L_080d3834:
	.4byte Camera_ApplyPhasedDelta
.L_080d3838:
	.4byte 0x00007784
.L_080d383c:
	.4byte BattlePresentation_ProcessPendingGraphicsTransfer
.L_080d3840:
	.4byte Data_080ee1b4 + 0x10
.L_080d3844:
	.4byte gCameraWork
.L_080d3848:
	.4byte 0x0000031f
.L_080d384c:
	.4byte 0x000077a8
.L_080d3850:
	.4byte 0x00007824
