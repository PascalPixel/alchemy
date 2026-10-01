.syntax unified
	.thumb
	.global Unnamed_080da6cc
	.thumb_func
Unnamed_080da6cc:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r2, .L_080da708
	adds r3, r2, #0
	ldmia r3!, {r1}
	ldr r3, [r3]
	sub sp, #56
	str r3, [sp, #32]
	adds r3, r2, #0
	subs r3, #108
	ldr r3, [r3]
	str r3, [sp, #24]
	ldr r2, [r2, #8]
	ldr r3, .L_080da70c
	str r2, [sp, #20]
	mov r9, r1
	add r3, r9
	str r0, [r3]
	ldr r3, [r0, #4]
	cmp r3, #1
	bne .L_080da710
	movs r0, #1
	bl BattleFx_BeginCanvasLayer
	b .L_080da716
.L_080da708:
	.4byte gBattleFxWork
.L_080da70c:
	.4byte 0x00007828
.L_080da710:
	movs r0, #0
	bl BattleFx_BeginCanvasLayer
.L_080da716:
	ldr r0, .L_080da798
	mov r1, r9
	movs r2, #1
	movs r3, #1
	bl Resource_LoadAndDecompress
	ldr r0, .L_080da79c
	ldr r1, [sp, #20]
	movs r2, #0
	movs r3, #0
	bl Resource_LoadAndDecompress
	movs r3, #3
	movs r1, #7
	movs r2, #7
	movs r0, #46
	str r3, [sp, #0]
	bl BattleEffect_LoadWork
	ldr r5, .L_080da7a0
	adds r3, r5, #0
	adds r3, #184
	ldr r3, [r3]
	str r3, [sp, #36]
	movs r3, #2
	str r3, [sp, #0]
	movs r1, #7
	movs r2, #7
	movs r3, #3
	movs r0, #47
	bl BattleEffect_LoadWork
	adds r5, #188
	ldr r3, [r5]
	mov r2, sp
	adds r2, #36
	ldr r5, .L_080da7a4
	str r2, [sp, #12]
	str r3, [r2, #4]
	ldr r2, .L_080da7a8
	ldr r3, .L_080da794
	add r5, r9
	strh r3, [r2]
	ldr r3, [r5]
	ldr r0, [r3, #8]
	bl GetBattleObjectSlotFar
	ldr r3, [r5]
	ldr r0, [r0]
	mov r8, r0
	ldr r0, [r3, #8]
	bl Battle_GetObjectTableValueFar
	mov r5, r8
	ldr r3, [r5, #12]
	adds r3, r3, r0
	movs r1, #127
	movs r0, #0
	str r3, [sp, #16]
	ldr r7, .L_080da7ac
	mov r10, r0
	mov r11, r1
	b .L_080da7b0
.L_080da794:
	.4byte 0x00001010
.L_080da798:
	.4byte 0x000000b4
.L_080da79c:
	.4byte 0x00000073
.L_080da7a0:
	.4byte gWorkSlot
.L_080da7a4:
	.4byte 0x00007828
.L_080da7a8:
	.4byte 0x04000052
.L_080da7ac:
	.4byte gMapCellBuffer
.L_080da7b0:
	bl Random16
	adds r6, r0, #0
	bl Random16
	mov r2, r11
	adds r5, r0, #0
	adds r0, r6, #0
	ands r5, r2
	bl Trig_Sin
	adds r5, #127
	adds r3, r5, #0
	muls r3, r0
	asrs r3, r3, #6
	str r3, [r7, #12]
	bl Random16
	mov r3, r11
	ands r0, r3
	subs r0, #16
	lsls r0, r0, #16
	asrs r0, r0, #6
	str r0, [r7, #16]
	adds r0, r6, #0
	bl Trig_Cos
	adds r3, r5, #0
	muls r3, r0
	asrs r3, r3, #6
	str r3, [r7, #20]
	mov r5, r8
	ldr r3, [r5, #8]
	str r3, [r7]
	ldr r0, [sp, #16]
	str r0, [r7, #4]
	ldr r3, [r5, #16]
	movs r1, #1
	str r3, [r7, #8]
	add r10, r1
	movs r3, #1
	negs r3, r3
	mov r2, r10
	str r3, [r7, #24]
	adds r7, #28
	cmp r2, #64
	bne .L_080da7b0
	ldr r3, .L_080dab34
	movs r2, #0
	add r3, r9
	str r2, [r3]
	ldr r3, .L_080dab38
	movs r5, #144
	add r3, r9
	lsls r5, r5, #3
	str r2, [r3]
	adds r1, r5, #0
	ldr r0, .L_080dab3c
	bl Scheduler_AddOrUpdateCallback
	movs r2, #239
	lsls r2, r2, #7
	add r2, r9
	movs r3, #2
	str r3, [r2]
	ldr r2, .L_080dab40
	movs r3, #75
	add r2, r9
	str r3, [r2]
	adds r1, r5, #0
	ldr r0, .L_080dab44
	bl Scheduler_AddOrUpdateCallback
	movs r3, #0
	str r3, [sp, #28]
	ldr r3, .L_080dab48
	add r3, r9
	ldr r3, [r3]
	ldr r2, .L_080dab4c
	ldr r3, [r3, #24]
	ldrb r3, [r2, r3]
	movs r5, #132
	lsrs r3, r3, #1
	negs r5, r5
	cmp r3, r5
	bne .L_080da85e
	b .L_080dab06
.L_080da85e:
	ldr r0, [sp, #24]
	ldr r1, .L_080dab48
	adds r0, #12
	add r1, r9
	str r0, [sp, #8]
	mov r11, r1
.L_080da86a:
	ldr r3, [sp, #28]
	subs r3, #17
	cmp r3, #62
	bhi .L_080da87c
	ldr r2, .L_080dab34
	movs r3, #128
	add r2, r9
	lsls r3, r3, #1
	b .L_080da882
.L_080da87c:
	ldr r2, .L_080dab34
	movs r3, #0
	add r2, r9
.L_080da882:
	str r3, [r2]
	ldr r5, .L_080dab48
	add r5, r9
	ldr r3, [r5]
	ldr r6, .L_080dab4c
	ldr r3, [r3, #24]
	ldrb r3, [r6, r3]
	ldr r2, [sp, #28]
	lsrs r3, r3, #1
	adds r3, #108
	cmp r2, r3
	bne .L_080da8a0
	movs r0, #133
	bl BattleEventRuntime_BeginPhaseFar
.L_080da8a0:
	movs r2, #0
	movs r3, #100
	movs r0, #0
	movs r1, #0
	bl BattlePres_SetupTransitionSceneFar
	bl Render_ResetTransformState
	ldr r0, [sp, #24]
	ldr r1, [sp, #8]
	bl Graphics_PrepareTransferInIwramWork
	movs r3, #0
	mov r10, r3
	ldr r3, [r5]
	ldr r3, [r3, #24]
	ldrb r3, [r6, r3]
	adds r2, r6, #0
	cmp r3, #0
	bne .L_080da8ca
	b .L_080daa5c
.L_080da8ca:
	ldr r6, .L_080dab50
.L_080da8cc:
	mov r5, r10
	lsrs r3, r5, #31
	add r3, r10
	asrs r3, r3, #1
	mov r8, r3
	ldr r0, [sp, #28]
	mov r7, r8
	adds r7, #48
	cmp r0, r8
	ble .L_080da978
	movs r1, #1
	ldr r3, [r6, #24]
	negs r1, r1
	cmp r3, r1
	bne .L_080da974
	add r5, sp, #44
	adds r0, r6, #0
	adds r1, r5, #0
	bl EffectPosition_ApplyBaseAndYOffset
	ldr r3, [r5]
	asrs r3, r3, #1
	str r3, [r5]
	ldr r3, [r5, #8]
	cmp r3, #159
	bgt .L_080da904
	movs r3, #160
	str r3, [r5, #8]
.L_080da904:
	ldr r2, .L_080dab54
	cmp r3, r2
	ble .L_080da90e
	str r2, [r5, #8]
	adds r3, r2, #0
.L_080da90e:
	adds r2, r3, #0
	subs r2, #160
	cmp r2, #0
	bge .L_080da918
	adds r2, #63
.L_080da918:
	asrs r2, r2, #6
	movs r3, #10
	subs r4, r3, r2
	mov r7, r8
	ldr r3, [sp, #28]
	movs r2, #4
	adds r7, #48
	mov r12, r2
	cmp r3, r7
	blt .L_080da930
	movs r0, #0
	mov r12, r0
.L_080da930:
	lsls r0, r4, #1
	ldr r2, .L_080dab58
	subs r3, r0, #2
	ldrh r1, [r2, r3]
	ldr r2, [sp, #20]
	lsrs r3, r4, #31
	adds r1, r2, r1
	adds r3, r4, r3
	ldr r2, [r5]
	asrs r3, r3, #1
	subs r2, r2, r3
	ldr r3, [r5, #4]
	str r0, [sp, #4]
	str r4, [sp, #0]
	ldr r0, [sp, #12]
	mov r5, r12
	subs r3, r3, r4
	ldr r4, [r5, r0]
	ldr r0, [sp, #32]
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
	b .L_080da978
.L_080da974:
	mov r7, r8
	adds r7, #48
.L_080da978:
	ldr r1, [sp, #28]
	cmp r1, r7
	ble .L_080daa46
	movs r2, #1
	ldr r3, [r6, #24]
	negs r2, r2
	cmp r3, r2
	bne .L_080daa46
	mov r3, r11
	ldr r5, [r3]
	mov r0, r10
	ldr r1, [r5, #20]
	bl __modsi3
	lsls r0, r0, #1
	adds r0, #36
	ldrsh r0, [r5, r0]
	bl GetBattleObjectSlotFar
	ldr r1, [r0]
	ldr r2, [r6]
	ldr r3, [r1, #8]
	subs r3, r3, r2
	ldr r2, [r6, #12]
	asrs r3, r3, #9
	adds r0, r2, r3
	str r0, [r6, #12]
	ldr r2, [r6, #4]
	ldr r3, [r1, #12]
	subs r3, r3, r2
	ldr r2, [r6, #16]
	asrs r3, r3, #9
	adds r4, r2, r3
	str r4, [r6, #16]
	ldr r2, [r6, #8]
	ldr r3, [r1, #16]
	subs r3, r3, r2
	ldr r2, [r6, #20]
	asrs r3, r3, #9
	adds r1, r2, r3
	str r1, [r6, #20]
	mov r3, r8
	ldr r2, [sp, #28]
	adds r3, #85
	cmp r2, r3
	bge .L_080daa04
	lsls r3, r0, #4
	subs r3, r3, r0
	lsls r2, r3, #2
	cmp r2, #0
	bge .L_080da9e0
	adds r2, #63
.L_080da9e0:
	asrs r3, r2, #6
	str r3, [r6, #12]
	lsls r3, r4, #4
	subs r3, r3, r4
	lsls r2, r3, #2
	cmp r2, #0
	bge .L_080da9f0
	adds r2, #63
.L_080da9f0:
	asrs r3, r2, #6
	str r3, [r6, #16]
	lsls r3, r1, #4
	subs r3, r3, r1
	lsls r2, r3, #2
	cmp r2, #0
	bge .L_080daa00
	adds r2, #63
.L_080daa00:
	asrs r3, r2, #6
	str r3, [r6, #20]
.L_080daa04:
	ldr r3, [r6, #4]
	cmp r3, #0
	bge .L_080daa46
	movs r3, #0
	str r3, [r6, #24]
	add r2, sp, #44
	ldr r3, [r2]
	str r3, [r6]
	ldr r3, [r2, #4]
	movs r0, #136
	str r3, [r6, #4]
	bl AudioCommand_PlayFar
	mov r3, r11
	ldr r5, [r3]
	mov r0, r10
	ldr r1, [r5, #20]
	bl __modsi3
	adds r3, r0, #0
	lsls r2, r3, #1
	adds r2, #36
	ldrsh r0, [r5, r2]
	movs r2, #4
	str r2, [sp, #0]
	movs r1, #10
	movs r2, #5
	bl ObjectGroup_UpdateMembers
	ldr r2, .L_080dab5c
	movs r3, #2
	add r2, r9
	str r3, [r2]
.L_080daa46:
	mov r5, r11
	ldr r3, [r5]
	movs r2, #1
	add r10, r2
	ldr r3, [r3, #24]
	ldr r2, .L_080dab4c
	ldrb r3, [r2, r3]
	adds r6, #28
	cmp r10, r3
	beq .L_080daa5c
	b .L_080da8cc
.L_080daa5c:
	mov r1, r11
	ldr r3, [r1]
	ldr r3, [r3, #24]
	ldrb r3, [r2, r3]
	movs r0, #0
	mov r10, r0
	cmp r3, #0
	beq .L_080daac2
	ldr r6, .L_080dab50
.L_080daa6e:
	ldr r3, [r6, #24]
	cmp r3, #11
	bhi .L_080daab0
	lsrs r4, r3, #31
	adds r4, r3, r4
	asrs r4, r4, #1
	ldr r2, .L_080dab60
	lsls r3, r4, #1
	ldrh r1, [r2, r3]
	ldr r3, .L_080dab64
	ldrb r5, [r3, r4]
	ldr r2, [r6]
	lsrs r3, r5, #1
	subs r2, r2, r3
	ldr r3, .L_080dab68
	ldrb r0, [r3, r4]
	ldr r3, [r6, #4]
	str r5, [sp, #0]
	adds r3, r3, r0
	ldr r0, .L_080dab6c
	ldrb r0, [r0, r4]
	ldr r5, [sp, #12]
	str r0, [sp, #4]
	subs r3, #56
	add r1, r9
	ldr r4, [r5, #4]
	ldr r0, [sp, #32]
	bl _call_via_r4
	ldr r3, [r6, #24]
	adds r3, #1
	str r3, [r6, #24]
	ldr r2, .L_080dab4c
.L_080daab0:
	mov r1, r11
	ldr r3, [r1]
	ldr r3, [r3, #24]
	movs r0, #1
	ldrb r3, [r2, r3]
	add r10, r0
	adds r6, #28
	cmp r10, r3
	bne .L_080daa6e
.L_080daac2:
	ldr r2, .L_080dab38
	add r2, r9
	ldr r3, [r2]
	cmp r3, #0
	bne .L_080daad0
	movs r3, #1
	str r3, [r2]
.L_080daad0:
	movs r0, #8
	movs r1, #8
	bl Camera_ApplyShake
	bl ObjectGroup_TickMemberTimers
	ldr r2, .L_080dab70
	movs r3, #1
	add r2, r9
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r2, [sp, #28]
	adds r2, #1
	str r2, [sp, #28]
	mov r5, r11
	ldr r3, [r5]
	ldr r2, .L_080dab4c
	ldr r3, [r3, #24]
	ldrb r3, [r2, r3]
	ldr r0, [sp, #28]
	lsrs r3, r3, #1
	adds r3, #132
	cmp r0, r3
	beq .L_080dab06
	b .L_080da86a
.L_080dab06:
	ldr r0, .L_080dab44
	bl Scheduler_RemoveCallback
	ldr r0, .L_080dab3c
	bl Scheduler_RemoveCallback
	movs r0, #47
	bl Runtime_ReleaseHeapBlock
	movs r0, #46
	bl Runtime_ReleaseHeapBlock
	bl BattleFx_EndCanvasLayer
	add sp, #56
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
.L_080dab34:
	.4byte 0x000077ac
.L_080dab38:
	.4byte 0x000077b0
.L_080dab3c:
	.4byte Camera_ApplyPhasedDelta
.L_080dab40:
	.4byte 0x00007784
.L_080dab44:
	.4byte BattlePresentation_ProcessPendingGraphicsTransfer
.L_080dab48:
	.4byte 0x00007828
.L_080dab4c:
	.4byte Data_080eea41
.L_080dab50:
	.4byte gMapCellBuffer
.L_080dab54:
	.4byte 0x0000031f
.L_080dab58:
	.4byte ParticleStreams_CellOffsets
.L_080dab5c:
	.4byte 0x000077a8
.L_080dab60:
	.4byte Data_080eea56
.L_080dab64:
	.4byte Data_080eea44
.L_080dab68:
	.4byte Data_080eea50
.L_080dab6c:
	.4byte Data_080eea4a
.L_080dab70:
	.4byte 0x00007824
