.syntax unified
	.thumb
	.global BattlePres_RunBeamSequence
	.thumb_func
BattlePres_RunBeamSequence:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r2, .L_080e3b08
	adds r3, r2, #0
	ldmia r3!, {r1}
	ldr r3, [r3]
	sub sp, #92
	str r3, [sp, #44]
	ldr r3, [r2, #8]
	str r3, [sp, #36]
	subs r2, #108
	ldr r2, [r2]
	str r2, [sp, #32]
	ldr r5, [r0]
	str r5, [sp, #28]
	ldr r5, .L_080e3b0c
	mov r11, r1
	add r5, r11
	str r0, [r5]
	ldr r0, [r0, #8]
	bl Owner_GetStateFar
	str r0, [sp, #24]
	movs r0, #1
	bl WaitFrames
	bl BattlePres_ConfigureEffectDisplay
	bl BattleFx_SetupCanvasTileMap
	ldr r2, .L_080e3b10
	ldr r3, .L_080e3b04
	movs r0, #1
	strh r3, [r2]
	bl WaitFrames
	ldr r6, [sp, #28]
	cmp r6, #5
	bne .L_080e3b58
	ldr r3, [r5]
	ldr r3, [r3, #4]
	cmp r3, #0
	bne .L_080e3b36
	b .L_080e3b14
	.2byte 0x0000
.L_080e3b04:
	.4byte 0x00001f80
.L_080e3b08:
	.4byte gBattleFxWork
.L_080e3b0c:
	.4byte 0x00007828
.L_080e3b10:
	.4byte 0x0400000a
.L_080e3b14:
	movs r3, #3
	str r3, [sp, #0]
	movs r1, #7
	movs r2, #7
	movs r3, #11
	movs r0, #46
	bl BattleEffect_LoadWork
	movs r3, #2
	str r3, [sp, #0]
	movs r0, #47
	movs r1, #7
	movs r2, #7
	movs r3, #11
	bl BattleEffect_LoadWork
	b .L_080e3ba0
.L_080e3b36:
	movs r3, #3
	str r3, [sp, #0]
	movs r1, #7
	movs r2, #7
	movs r3, #15
	movs r0, #46
	bl BattleEffect_LoadWork
	movs r3, #2
	str r3, [sp, #0]
	movs r0, #47
	movs r1, #7
	movs r2, #7
	movs r3, #15
	bl BattleEffect_LoadWork
	b .L_080e3ba0
.L_080e3b58:
	ldr r3, [r5]
	ldr r3, [r3, #4]
	cmp r3, #0
	bne .L_080e3b80
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
	b .L_080e3ba0
.L_080e3b80:
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
.L_080e3ba0:
	ldr r3, .L_080e3ca8
	adds r2, r3, #0
	adds r2, #184
	ldr r2, [r2]
	str r2, [sp, #48]
	adds r3, #188
	ldr r3, [r3]
	mov r0, sp
	adds r0, #48
	str r0, [sp, #16]
	str r3, [r0, #4]
	movs r0, #1
	bl WaitFrames
	ldr r1, [sp, #28]
	cmp r1, #4
	bne .L_080e3bc6
	ldr r0, .L_080e3cac
	b .L_080e3bf2
.L_080e3bc6:
	ldr r2, [sp, #28]
	cmp r2, #3
	bne .L_080e3bda
	ldr r0, .L_080e3cb0
	mov r1, r11
	movs r2, #0
	movs r3, #0
	bl Resource_LoadAndDecompress
	b .L_080e3c0a
.L_080e3bda:
	ldr r3, [sp, #28]
	cmp r3, #2
	beq .L_080e3bfe
	cmp r3, #2
	bgt .L_080e3bea
	cmp r3, #0
	blt .L_080e3c0a
	b .L_080e3bf0
.L_080e3bea:
	ldr r5, [sp, #28]
	cmp r5, #5
	bne .L_080e3c0a
.L_080e3bf0:
	ldr r0, .L_080e3cb4
.L_080e3bf2:
	mov r1, r11
	movs r2, #1
	movs r3, #1
	bl Resource_LoadAndDecompress
	b .L_080e3c0a
.L_080e3bfe:
	ldr r0, .L_080e3cb8
	mov r1, r11
	movs r2, #1
	movs r3, #1
	bl Resource_LoadAndDecompress
.L_080e3c0a:
	ldr r3, .L_080e3cbc
	add r3, r11
	ldr r3, [r3]
	ldr r3, [r3, #8]
	cmp r3, #7
	ble .L_080e3c2c
	ldr r0, .L_080e3cc0
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_080e3cc4
	lsls r0, r0, #19
	movs r2, #128
	bl _call_via_r3
	b .L_080e3c40
.L_080e3c2c:
	ldr r0, .L_080e3cc8
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_080e3cc4
	lsls r0, r0, #19
	movs r2, #128
	bl _call_via_r3
.L_080e3c40:
	movs r0, #1
	bl WaitFrames
	ldr r0, .L_080e3ccc
	ldr r1, [sp, #36]
	movs r2, #0
	movs r3, #0
	bl Resource_LoadAndDecompress
	ldr r0, .L_080e3cd0
	ldr r1, .L_080e3cd4
	movs r2, #1
	movs r3, #0
	bl Resource_LoadAndDecompress
	movs r2, #239
	lsls r2, r2, #7
	add r2, r11
	movs r3, #2
	str r3, [r2]
	ldr r2, .L_080e3cd8
	movs r3, #50
	add r2, r11
	movs r1, #144
	str r3, [r2]
	lsls r1, r1, #3
	ldr r0, .L_080e3cdc
	bl Scheduler_AddOrUpdateCallback
	ldr r5, .L_080e3cbc
	ldr r2, .L_080e3ce0
	ldr r3, .L_080e3ca4
	add r5, r11
	strh r3, [r2]
	ldr r3, [r5]
	mov r1, sp
	movs r6, #36
	ldrsh r0, [r3, r6]
	adds r1, #68
	str r1, [sp, #20]
	bl EffectPosition_ApplyAnimationAndYOffset
	ldr r3, [r5]
	ldr r3, [r3, #4]
	cmp r3, #0
	bne .L_080e3ce4
	ldr r3, [sp, #20]
	ldr r2, [r3]
	movs r3, #96
	b .L_080e3cea
.L_080e3ca4:
	.4byte 0x00001f81
.L_080e3ca8:
	.4byte gWorkSlot
.L_080e3cac:
	.4byte 0x0000006b
.L_080e3cb0:
	.4byte 0x000000c5
.L_080e3cb4:
	.4byte 0x000000b5
.L_080e3cb8:
	.4byte 0x000000b6
.L_080e3cbc:
	.4byte 0x00007828
.L_080e3cc0:
	.4byte 0x0000008e
.L_080e3cc4:
	.4byte IwramCopyWords
.L_080e3cc8:
	.4byte 0x0000004a
.L_080e3ccc:
	.4byte 0x00000076
.L_080e3cd0:
	.4byte 0x00000099
.L_080e3cd4:
	.4byte gMapCellBuffer
.L_080e3cd8:
	.4byte 0x00007784
.L_080e3cdc:
	.4byte BattlePresentation_ProcessPendingGraphicsTransfer
.L_080e3ce0:
	.4byte 0x0400000a
.L_080e3ce4:
	ldr r5, [sp, #20]
	ldr r2, [r5]
	movs r3, #32
.L_080e3cea:
	subs r3, r3, r2
	str r3, [sp, #40]
	ldr r6, [sp, #40]
	cmp r6, #0
	ble .L_080e3cf8
	movs r0, #0
	str r0, [sp, #40]
.L_080e3cf8:
	movs r3, #128
	ldr r1, [sp, #40]
	negs r3, r3
	cmp r1, r3
	bge .L_080e3d04
	str r3, [sp, #40]
.L_080e3d04:
	ldr r2, [sp, #20]
	ldr r5, [sp, #40]
	ldr r3, [r2]
	add r6, sp, #40
	adds r3, r3, r5
	str r3, [r2]
	ldrh r6, [r6]
	ldr r2, .L_080e4078
	ldr r5, .L_080e407c
	movs r3, #80
	strh r6, [r2, #4]
	strh r3, [r2, #6]
	add r5, r11
	movs r0, #1
	bl WaitFrames
	ldr r3, [r5]
	movs r1, #36
	ldrsh r0, [r3, r1]
	bl GetBattleObjectSlotFar
	ldr r3, [r5]
	ldr r6, [r0]
	movs r2, #36
	ldrsh r0, [r3, r2]
	bl Battle_GetObjectTableValueFar
	lsrs r3, r0, #31
	adds r0, r0, r3
	movs r5, #225
	asrs r0, r0, #1
	movs r3, #0
	lsls r5, r5, #7
	mov r8, r0
	mov r10, r3
	movs r7, #255
	add r5, r11
.L_080e3d4e:
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
	subs r0, #32
	lsls r0, r0, #10
	str r0, [r5, #16]
	bl Random16
	ands r0, r7
	subs r0, #127
	ldr r3, [r5]
	lsls r0, r0, #10
	str r0, [r5, #20]
	cmp r3, #0
	ble .L_080e3d8a
	ldr r3, [r5, #12]
	negs r3, r3
	str r3, [r5, #12]
.L_080e3d8a:
	ldr r3, [r5, #12]
	movs r0, #1
	negs r3, r3
	str r3, [r5, #12]
	mov r3, r10
	add r10, r0
	adds r3, #16
	mov r1, r10
	str r3, [r5, #24]
	adds r5, #28
	cmp r1, #64
	bne .L_080e3d4e
	ldr r5, [sp, #32]
	mov r3, sp
	adds r3, #80
	adds r5, #12
	movs r2, #0
	str r3, [sp, #12]
	str r5, [sp, #8]
	mov r9, r2
.L_080e3db2:
	mov r6, r9
	cmp r6, #5
	bne .L_080e3dd8
	ldr r0, [sp, #24]
	movs r1, #148
	lsls r1, r1, #1
	adds r3, r0, r1
	ldrb r0, [r3]
	bl Summon_IsEntrySecondaryFlaggedFar
	cmp r0, #0
	beq .L_080e3dd2
	movs r0, #134
	bl BattleEventRuntime_BeginPhaseFar
	b .L_080e3dd8
.L_080e3dd2:
	movs r0, #133
	bl BattleEventRuntime_BeginPhaseFar
.L_080e3dd8:
	mov r2, r9
	cmp r2, #4
	bne .L_080e3dee
	ldr r3, .L_080e407c
	add r3, r11
	ldr r3, [r3]
	movs r1, #0
	movs r5, #36
	ldrsh r0, [r3, r5]
	bl BattleMotion_ApplyVariantMotionFar
.L_080e3dee:
	ldr r6, .L_080e407c
	add r6, r11
	ldr r3, [r6]
	ldr r1, [sp, #12]
	ldr r0, [r3, #8]
	bl EffectPosition_ApplyStepAndYOffset
	ldr r1, [sp, #12]
	ldr r0, [r1, #4]
	adds r7, r0, #0
	adds r7, #16
	str r7, [r1, #4]
	ldr r2, [sp, #28]
	cmp r2, #4
	bne .L_080e3e7a
	mov r3, r9
	cmp r3, #11
	ble .L_080e3e14
	b .L_080e3f6e
.L_080e3e14:
	ldr r3, [r6]
	ldr r3, [r3, #4]
	cmp r3, #0
	bne .L_080e3e4c
	mov r5, r9
	lsrs r2, r5, #31
	add r2, r9
	asrs r2, r2, #1
	movs r3, #5
	subs r3, r3, r2
	ldr r6, [sp, #12]
	lsls r1, r3, #1
	ldr r2, [r6]
	adds r1, r1, r3
	ldr r3, [sp, #40]
	movs r5, #48
	adds r2, r2, r3
	adds r3, r0, #0
	movs r0, #16
	str r0, [sp, #4]
	str r5, [sp, #0]
	ldr r6, [sp, #16]
	lsls r1, r1, #8
	add r1, r11
	subs r2, #48
	adds r3, #8
	ldr r4, [r6, #4]
	b .L_080e3f32
.L_080e3e4c:
	mov r1, r9
	lsrs r2, r1, #31
	add r2, r9
	asrs r2, r2, #1
	movs r3, #5
	subs r3, r3, r2
	lsls r1, r3, #1
	adds r1, r1, r3
	ldr r3, [sp, #12]
	movs r6, #48
	ldr r2, [r3]
	adds r3, r0, #0
	movs r0, #16
	str r0, [sp, #4]
	ldr r5, [sp, #40]
	ldr r0, [sp, #16]
	str r6, [sp, #0]
	lsls r1, r1, #8
	ldr r4, [r0, #4]
	add r1, r11
	adds r2, r2, r5
	adds r3, #8
	b .L_080e3f32
.L_080e3e7a:
	ldr r1, [sp, #28]
	cmp r1, #2
	bls .L_080e3e84
	cmp r1, #5
	bne .L_080e3eee
.L_080e3e84:
	mov r2, r9
	cmp r2, #11
	bgt .L_080e3f6e
	ldr r3, [r6]
	ldr r3, [r3, #4]
	cmp r3, #0
	bne .L_080e3ec0
	lsrs r3, r2, #31
	add r3, r9
	asrs r3, r3, #1
	lsls r1, r3, #3
	subs r1, r1, r3
	lsls r1, r1, #2
	subs r1, r1, r3
	ldr r3, [sp, #12]
	ldr r5, [sp, #40]
	ldr r2, [r3]
	movs r6, #48
	adds r3, r0, #0
	movs r0, #72
	str r0, [sp, #4]
	str r6, [sp, #0]
	ldr r0, [sp, #16]
	lsls r1, r1, #7
	adds r2, r2, r5
	ldr r4, [r0, #4]
	add r1, r11
	subs r2, #48
	subs r3, #24
	b .L_080e3f32
.L_080e3ec0:
	mov r1, r9
	lsrs r3, r1, #31
	add r3, r9
	asrs r3, r3, #1
	lsls r1, r3, #3
	subs r1, r1, r3
	lsls r1, r1, #2
	subs r1, r1, r3
	ldr r3, [sp, #12]
	movs r6, #48
	ldr r2, [r3]
	adds r3, r0, #0
	movs r0, #72
	str r0, [sp, #4]
	ldr r5, [sp, #40]
	ldr r0, [sp, #16]
	str r6, [sp, #0]
	lsls r1, r1, #7
	ldr r4, [r0, #4]
	add r1, r11
	adds r2, r2, r5
	subs r3, #24
	b .L_080e3f32
.L_080e3eee:
	mov r1, r9
	cmp r1, #17
	bgt .L_080e3f6e
	mov r0, r9
	movs r1, #3
	bl __divsi3
	ldr r3, [r6]
	ldr r3, [r3, #4]
	adds r5, r0, #0
	cmp r3, #0
	bne .L_080e3f3a
	ldr r2, .L_080e4080
	lsls r3, r5, #1
	ldrh r1, [r2, r3]
	ldr r6, [sp, #12]
	ldr r3, .L_080e4084
	ldr r2, [r6]
	ldrb r3, [r3, r5]
	ldr r0, [sp, #40]
	adds r2, r2, r3
	adds r2, r2, r0
	ldr r3, .L_080e4088
	ldr r0, .L_080e408c
	ldrb r4, [r3, r5]
	ldrb r0, [r0, r5]
	str r4, [sp, #4]
	str r0, [sp, #0]
	ldr r5, [sp, #16]
	lsrs r3, r4, #1
	add r1, r11
	subs r2, #58
	subs r3, r7, r3
	ldr r4, [r5, #4]
.L_080e3f32:
	ldr r0, [sp, #44]
	bl _call_via_r4
	b .L_080e3f6e
.L_080e3f3a:
	ldr r2, .L_080e4080
	lsls r3, r5, #1
	ldrh r1, [r2, r3]
	ldr r6, [sp, #12]
	ldr r3, .L_080e4084
	ldr r2, [r6]
	ldrb r3, [r3, r5]
	subs r2, r2, r3
	ldr r3, .L_080e408c
	ldr r0, [sp, #40]
	ldrb r4, [r3, r5]
	ldr r3, .L_080e4088
	adds r2, r2, r0
	ldrb r0, [r3, r5]
	str r4, [sp, #0]
	str r0, [sp, #4]
	ldr r5, [sp, #16]
	subs r2, r2, r4
	lsrs r3, r0, #1
	add r1, r11
	adds r2, #58
	subs r3, r7, r3
	ldr r4, [r5, #4]
	ldr r0, [sp, #44]
	bl _call_via_r4
.L_080e3f6e:
	mov r5, r9
	subs r5, #4
	cmp r5, #11
	bhi .L_080e3fa0
	lsrs r3, r5, #31
	adds r3, r5, r3
	asrs r3, r3, #1
	ldr r0, [sp, #20]
	lsls r1, r3, #4
	subs r1, r1, r3
	ldr r6, .L_080e4090
	ldr r2, [r0]
	ldr r3, [r0, #4]
	lsls r1, r1, #7
	movs r0, #40
	adds r1, r1, r6
	movs r6, #48
	str r0, [sp, #0]
	subs r2, #16
	subs r3, #24
	str r6, [sp, #4]
	ldr r4, [sp, #48]
	ldr r0, [sp, #44]
	bl _call_via_r4
.L_080e3fa0:
	bl Render_ResetTransformState
	ldr r0, [sp, #32]
	ldr r1, [sp, #8]
	bl Graphics_PrepareTransferInIwramWork
	cmp r5, #27
	bhi .L_080e402e
	movs r1, #56
	movs r0, #0
	add r1, sp
	mov r10, r0
	mov r8, r1
.L_080e3fba:
	mov r2, r10
	lsrs r3, r2, #31
	add r3, r10
	asrs r6, r3, #1
	lsls r3, r6, #3
	subs r3, r3, r6
	lsls r3, r3, #2
	movs r5, #225
	lsls r5, r5, #7
	add r3, r11
	adds r7, r3, r5
	ldr r5, [r7, #24]
	cmp r5, #0
	ble .L_080e4024
	mov r1, r8
	adds r0, r7, #0
	bl Render_ProjectPoint
	mov r0, r8
	ldr r2, [r0]
	ldr r1, [sp, #40]
	asrs r5, r5, #3
	adds r5, #2
	lsls r4, r5, #1
	adds r2, r2, r1
	ldr r1, .L_080e4094
	str r2, [r0]
	subs r3, r4, #2
	ldrh r1, [r1, r3]
	movs r0, #1
	ldr r3, [sp, #36]
	ands r0, r6
	mov r6, r8
	adds r1, r3, r1
	ldr r3, [r6, #4]
	subs r2, r2, r5
	subs r3, r3, r5
	str r4, [sp, #0]
	str r4, [sp, #4]
	ldr r5, [sp, #16]
	lsls r0, r0, #2
	ldr r4, [r0, r5]
	ldr r0, [sp, #44]
	bl _call_via_r4
	adds r0, r7, #0
	movs r1, #60
	ldr r2, .L_080e4098
	bl EffectStep_AdvanceWithGravity3D
	ldr r3, [r7, #24]
	subs r3, #1
	str r3, [r7, #24]
.L_080e4024:
	movs r6, #1
	add r10, r6
	mov r0, r10
	cmp r0, #64
	bne .L_080e3fba
.L_080e402e:
	ldr r2, .L_080e409c
	movs r3, #1
	add r2, r11
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	movs r1, #1
	add r9, r1
	mov r2, r9
	cmp r2, #32
	beq .L_080e4048
	b .L_080e3db2
.L_080e4048:
	ldr r0, .L_080e40a0
	bl Scheduler_RemoveCallback
	movs r0, #47
	bl Runtime_ReleaseHeapBlock
	movs r0, #46
	bl Runtime_ReleaseHeapBlock
	ldr r3, .L_080e4078
	mov r5, r9
	strh r5, [r3, #6]
	bl BattleFx_SetTransitionFlagAndDisplay
	add sp, #92
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
.L_080e4078:
	.4byte gBgScroll
.L_080e407c:
	.4byte 0x00007828
.L_080e4080:
	.4byte Data_080eedbe
.L_080e4084:
	.4byte Data_080eedca
.L_080e4088:
	.4byte Data_080eedb8
.L_080e408c:
	.4byte Data_080eedb2
.L_080e4090:
	.4byte gMapCellBuffer
.L_080e4094:
	.4byte BattleFx6_FlareCells
.L_080e4098:
	.4byte 0xfffffc00
.L_080e409c:
	.4byte 0x00007824
.L_080e40a0:
	.4byte BattlePresentation_ProcessPendingGraphicsTransfer
