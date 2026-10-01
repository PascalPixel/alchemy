.syntax unified
	.thumb
	.global BattleFx_RunFiveMode
	.thumb_func
BattleFx_RunFiveMode:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #84
	ldr r6, .L_080c9d64
	str r1, [sp, #68]
	adds r3, r6, #0
	ldmia r3!, {r1}
	str r1, [sp, #64]
	ldr r3, [r3]
	str r3, [sp, #60]
	adds r3, r6, #0
	ldr r2, .L_080c9d68
	subs r3, #108
	ldr r3, [r3]
	adds r5, r1, r2
	str r3, [sp, #44]
	str r0, [r5]
	movs r0, #1
	bl BattleFx_BeginCanvasLayer
	ldr r3, [r5]
	ldr r3, [r3, #4]
	cmp r3, #0
	bne .L_080c9d00
	movs r5, #2
	movs r1, #7
	movs r2, #7
	movs r3, #11
	movs r0, #46
	str r5, [sp, #0]
	bl Unnamed_080ed408
	ldr r3, [r6, #28]
	movs r0, #47
	str r3, [sp, #48]
	movs r1, #7
	movs r2, #7
	movs r3, #3
	b .L_080c9d1c
.L_080c9d00:
	movs r5, #2
	movs r1, #7
	movs r2, #7
	movs r3, #15
	movs r0, #46
	str r5, [sp, #0]
	bl Unnamed_080ed408
	ldr r4, [r6, #28]
	movs r0, #47
	movs r1, #7
	movs r2, #7
	movs r3, #7
	str r4, [sp, #48]
.L_080c9d1c:
	str r5, [sp, #0]
	bl Unnamed_080ed408
	ldr r6, [r6, #32]
	str r6, [sp, #52]
	ldr r0, .L_080c9d6c
	ldr r1, [sp, #64]
	movs r2, #0
	movs r3, #0
	bl Resource_LoadAndDecompress
	ldr r0, [sp, #68]
	cmp r0, #4
	bhi .L_080c9d84
	ldr r3, .L_080c9d70
	lsls r1, r0, #2
	ldr r3, [r1, r3]
	mov pc, r3
.L_080c9d40:
	.4byte .L_080c9d54
	.4byte .L_080c9d58
	.4byte .L_080c9d5c
	.4byte .L_080c9d60
	.4byte .L_080c9d84
.L_080c9d54:
	ldr r0, .L_080c9d74
	b .L_080c9d86
.L_080c9d58:
	ldr r0, .L_080c9d78
	b .L_080c9d86
.L_080c9d5c:
	ldr r0, .L_080c9d7c
	b .L_080c9d86
.L_080c9d60:
	ldr r0, .L_080c9d80
	b .L_080c9d86
.L_080c9d64:
	.4byte gBattleFxWork
.L_080c9d68:
	.4byte 0x00007828
.L_080c9d6c:
	.4byte 0x00000058
.L_080c9d70:
	.4byte .L_080c9d40
.L_080c9d74:
	.4byte 0x000000b4
.L_080c9d78:
	.4byte 0x000000a0
.L_080c9d7c:
	.4byte 0x000000cb
.L_080c9d80:
	.4byte 0x00000086
.L_080c9d84:
	ldr r0, .L_080c9f08
.L_080c9d86:
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_080c9f0c
	movs r2, #128
	lsls r0, r0, #19
	bl _call_via_r3
	movs r4, #239
	ldr r3, [sp, #64]
	lsls r4, r4, #7
	adds r2, r3, r4
	movs r3, #2
	str r3, [r2]
	ldr r0, [sp, #64]
	ldr r1, .L_080c9f10
	movs r3, #50
	adds r2, r0, r1
	movs r1, #144
	str r3, [r2]
	lsls r1, r1, #3
	ldr r0, .L_080c9f14
	bl Scheduler_AddOrUpdateCallback
	ldr r3, .L_080c9f18
	ldr r2, [sp, #64]
	adds r5, r2, r3
	ldr r3, [r5]
	ldr r0, [r3, #8]
	bl GetBattleObjectSlotFar
	ldr r3, [r5]
	ldr r1, [r3, #20]
	movs r4, #3
	adds r2, r1, #0
	muls r2, r4
	lsls r3, r2, #1
	ldr r0, [r0]
	adds r3, r3, r2
	lsls r3, r3, #1
	mov r10, r0
	adds r3, #48
	movs r0, #0
	str r3, [sp, #40]
	str r0, [sp, #56]
	cmp r1, #0
	beq .L_080c9e84
	mov r9, r0
.L_080c9de8:
	ldr r1, [sp, #64]
	ldr r2, .L_080c9f18
	ldr r4, [sp, #56]
	adds r3, r1, r2
	ldr r2, [r3]
	lsls r3, r4, #1
	adds r3, #36
	ldrsh r0, [r2, r3]
	bl GetBattleObjectSlotFar
	ldr r0, [r0]
	mov r4, r9
	mov r8, r0
	ldr r0, [sp, #64]
	lsls r3, r4, #2
	movs r1, #225
	movs r2, #0
	adds r3, r3, r0
	lsls r1, r1, #7
	mov r11, r2
	adds r7, r3, r1
.L_080c9e12:
	mov r2, r10
	ldr r3, [r2, #8]
	str r3, [r7]
	movs r4, #160
	ldr r5, [r2, #12]
	lsls r4, r4, #13
	adds r5, r5, r4
	str r5, [r7, #4]
	ldr r6, [r2, #16]
	str r6, [r7, #8]
	mov r1, r8
	ldr r0, [r1, #8]
	movs r1, #24
	subs r0, r0, r3
	bl __divsi3
	str r0, [r7, #12]
	mov r2, r8
	ldr r0, [r2, #12]
	movs r3, #160
	lsls r3, r3, #13
	adds r0, r0, r3
	movs r1, #24
	subs r0, r0, r5
	bl __divsi3
	str r0, [r7, #16]
	mov r4, r8
	ldr r0, [r4, #16]
	movs r1, #24
	subs r0, r0, r6
	bl __divsi3
	str r0, [r7, #20]
	movs r0, #1
	add r11, r0
	movs r3, #0
	mov r1, r11
	str r3, [r7, #24]
	adds r7, #28
	cmp r1, #3
	bne .L_080c9e12
	movs r2, #3
	lsls r3, r2, #3
	subs r3, r3, r2
	add r9, r3
	ldr r3, [sp, #56]
	ldr r4, [sp, #64]
	adds r3, #1
	ldr r0, .L_080c9f18
	str r3, [sp, #56]
	adds r3, r4, r0
	ldr r3, [r3]
	ldr r1, [sp, #56]
	ldr r3, [r3, #20]
	cmp r1, r3
	bne .L_080c9de8
.L_080c9e84:
	ldr r3, [sp, #40]
	movs r2, #0
	mov r10, r2
	cmp r3, #0
	bne .L_080c9e90
	b .L_080ca1a4
.L_080c9e90:
	ldr r4, [sp, #44]
	subs r3, #16
	adds r4, #12
	str r3, [sp, #24]
	str r4, [sp, #28]
.L_080c9e9a:
	ldr r0, [sp, #64]
	movs r1, #211
	movs r2, #0
	lsls r1, r1, #7
	movs r7, #128
	mov r3, r10
	str r2, [sp, #56]
	adds r6, r0, r1
	lsls r7, r7, #12
	lsls r5, r3, #12
.L_080c9eae:
	adds r0, r5, #0
	bl Trig_Sin
	lsls r0, r0, #1
	subs r0, r7, r0
	asrs r0, r0, #10
	stmia r6!, {r0}
	ldr r0, [sp, #56]
	movs r4, #128
	lsls r4, r4, #5
	adds r0, #1
	adds r5, r5, r4
	str r0, [sp, #56]
	cmp r0, #160
	bne .L_080c9eae
	ldr r1, [sp, #24]
	cmp r10, r1
	ble .L_080c9ee0
	ldr r4, [sp, #40]
	mov r0, r10
	ldr r1, .L_080c9f04
	ldr r3, .L_080c9f1c
	subs r2, r4, r0
	orrs r2, r1
	strh r2, [r3]
.L_080c9ee0:
	bl Render_ResetTransformState
	ldr r1, [sp, #28]
	ldr r0, [sp, #44]
	bl Graphics_PrepareTransferInIwramWork
	movs r1, #0
	ldr r3, [sp, #64]
	ldr r4, .L_080c9f18
	str r1, [sp, #56]
	adds r2, r3, r4
	ldr r3, [r2]
	ldr r3, [r3, #20]
	cmp r3, #0
	bne .L_080c9f00
	b .L_080ca17c
.L_080c9f00:
	b .L_080c9f20
	.2byte 0x0000
.L_080c9f04:
	.4byte 0x00001000
.L_080c9f08:
	.4byte 0x000000a3
.L_080c9f0c:
	.4byte IwramCopyWords
.L_080c9f10:
	.4byte 0x00007784
.L_080c9f14:
	.4byte BattlePresentation_ProcessPendingGraphicsTransfer
.L_080c9f18:
	.4byte 0x00007828
.L_080c9f1c:
	.4byte 0x04000052
.L_080c9f20:
	mov r1, r10
	str r2, [sp, #36]
	movs r0, #36
	movs r2, #0
	subs r1, #30
	str r0, [sp, #20]
	str r1, [sp, #16]
	str r2, [sp, #12]
	str r2, [sp, #8]
.L_080c9f32:
	ldr r3, [sp, #12]
	cmp r10, r3
	bge .L_080c9f3a
	b .L_080ca152
.L_080c9f3a:
	ldr r0, [sp, #68]
	lsls r0, r0, #2
	movs r4, #0
	str r0, [sp, #32]
	mov r11, r4
	mov r9, r3
.L_080c9f46:
	cmp r10, r9
	blt .L_080ca040
	ldr r3, [sp, #8]
	add r3, r11
	lsls r2, r3, #3
	ldr r1, [sp, #64]
	subs r2, r2, r3
	lsls r2, r2, #2
	movs r3, #225
	adds r2, r1, r2
	lsls r3, r3, #7
	adds r3, r3, r2
	mov r8, r3
	add r6, sp, #72
	mov r0, r8
	adds r1, r6, #0
	bl EffectPosition_ApplyBaseAndYOffset
	ldr r3, [r6]
	asrs r7, r3, #1
	str r7, [r6]
	mov r4, r8
	ldr r5, [r4, #24]
	cmp r5, #0
	bge .L_080c9f7a
	adds r5, #7
.L_080c9f7a:
	asrs r2, r5, #3
	cmp r2, #5
	ble .L_080c9f82
	movs r2, #5
.L_080c9f82:
	ldr r3, .L_080ca1cc
	ldr r0, [sp, #32]
	ldrsb r3, [r3, r0]
	cmp r3, #0
	beq .L_080c9fe8
	mov r1, r10
	lsls r5, r2, #1
	lsrs r0, r1, #31
	adds r5, r5, r2
	add r0, r10
	movs r1, #3
	lsls r5, r5, #3
	asrs r0, r0, #1
	adds r5, r5, r2
	bl __modsi3
	lsls r2, r0, #2
	adds r2, r2, r0
	lsls r3, r2, #4
	subs r3, r3, r2
	lsls r3, r3, #6
	ldr r2, [sp, #64]
	lsls r5, r5, #5
	adds r5, r5, r3
	ldr r3, [r6, #4]
	adds r5, r2, r5
	movs r4, #20
	movs r0, #40
	adds r2, r7, #0
	subs r2, #10
	subs r3, #40
	str r4, [sp, #0]
	str r0, [sp, #4]
	adds r1, r5, #0
	ldr r4, [sp, #48]
	ldr r0, [sp, #60]
	bl _call_via_r4
	ldr r2, [r6]
	movs r0, #20
	movs r1, #40
	ldr r3, [r6, #4]
	subs r2, #10
	str r0, [sp, #0]
	str r1, [sp, #4]
	ldr r0, [sp, #60]
	adds r1, r5, #0
	ldr r4, [sp, #52]
	bl _call_via_r4
	b .L_080ca02e
.L_080c9fe8:
	lsls r5, r2, #1
	adds r5, r5, r2
	lsls r5, r5, #3
	ldr r0, [sp, #64]
	adds r5, r5, r2
	lsls r5, r5, #5
	movs r1, #150
	ldr r3, [r6, #4]
	adds r5, r0, r5
	lsls r1, r1, #6
	adds r5, r5, r1
	movs r4, #20
	movs r0, #40
	adds r2, r7, #0
	subs r2, #10
	subs r3, #40
	str r4, [sp, #0]
	str r0, [sp, #4]
	adds r1, r5, #0
	ldr r4, [sp, #48]
	ldr r0, [sp, #60]
	bl _call_via_r4
	ldr r2, [r6]
	movs r0, #20
	movs r1, #40
	ldr r3, [r6, #4]
	subs r2, #10
	str r0, [sp, #0]
	str r1, [sp, #4]
	ldr r0, [sp, #60]
	adds r1, r5, #0
	ldr r4, [sp, #52]
	bl _call_via_r4
.L_080ca02e:
	mov r0, r8
	movs r1, #64
	movs r2, #0
	bl EffectStep_AdvanceWithGravity3D
	mov r0, r8
	ldr r3, [r0, #24]
	adds r3, #1
	str r3, [r0, #24]
.L_080ca040:
	movs r2, #1
	add r11, r2
	movs r1, #6
	mov r3, r11
	add r9, r1
	cmp r3, #3
	beq .L_080ca050
	b .L_080c9f46
.L_080ca050:
	ldr r3, [sp, #32]
	ldr r1, .L_080ca1cc
	adds r3, #3
	ldrsb r3, [r1, r3]
	cmp r3, #0
	beq .L_080ca0ca
	ldr r3, [sp, #12]
	adds r3, #30
	cmp r10, r3
	blt .L_080ca0ca
	ldr r3, [sp, #12]
	adds r3, #62
	cmp r10, r3
	bge .L_080ca0ca
	ldr r4, [sp, #36]
	ldr r1, [sp, #20]
	ldr r3, [r4]
	ldrsh r0, [r3, r1]
	bl GetBattleObjectSlotFar
	ldr r4, [sp, #12]
	mov r3, r10
	subs r2, r3, r4
	ldr r3, [sp, #16]
	ldr r0, [r0]
	ldr r1, .L_080ca1d0
	cmp r3, #0
	bge .L_080ca08c
	adds r3, r2, #0
	subs r3, #23
.L_080ca08c:
	ldr r2, [sp, #16]
	asrs r3, r3, #3
	lsls r3, r3, #3
	subs r3, r2, r3
	ldrsb r3, [r1, r3]
	ldr r2, [r0, #8]
	lsls r3, r3, #16
	adds r2, r2, r3
	str r2, [r0, #8]
	cmp r2, #0
	ble .L_080ca0aa
	movs r4, #128
	lsls r4, r4, #8
	adds r3, r2, r4
	b .L_080ca0ae
.L_080ca0aa:
	ldr r1, .L_080ca1d4
	adds r3, r2, r1
.L_080ca0ae:
	str r3, [r0, #8]
	ldr r2, [sp, #36]
	ldr r3, [r2]
	ldr r4, [sp, #20]
	ldrsh r0, [r3, r4]
	movs r3, #0
	movs r1, #1
	str r3, [sp, #0]
	negs r1, r1
	movs r2, #5
	subs r3, #1
	bl ObjectGroup_UpdateMembers
	ldr r1, .L_080ca1cc
.L_080ca0ca:
	ldr r3, [sp, #32]
	adds r3, #1
	ldrsb r3, [r1, r3]
	cmp r3, #0
	beq .L_080ca126
	ldr r3, [sp, #12]
	adds r3, #24
	cmp r10, r3
	bne .L_080ca106
	movs r0, #133
	bl AudioCommand_PlayFar
	ldr r2, [sp, #56]
	cmp r2, #0
	bne .L_080ca0f0
	movs r0, #1
	negs r0, r0
	bl BattleEventRuntime_BeginPhaseFar
.L_080ca0f0:
	ldr r4, [sp, #36]
	ldr r1, [sp, #20]
	ldr r3, [r4]
	ldrsh r0, [r3, r1]
	movs r3, #8
	str r3, [sp, #0]
	movs r1, #7
	movs r2, #5
	ldr r3, [sp, #56]
	bl ObjectGroup_UpdateMembers
.L_080ca106:
	ldr r3, [sp, #12]
	adds r3, #40
	cmp r10, r3
	bne .L_080ca124
	ldr r4, [sp, #36]
	ldr r3, [r4]
	ldr r1, [sp, #20]
	ldrsh r0, [r3, r1]
	movs r3, #8
	str r3, [sp, #0]
	movs r1, #7
	movs r2, #5
	ldr r3, [sp, #56]
	bl ObjectGroup_UpdateMembers
.L_080ca124:
	ldr r1, .L_080ca1cc
.L_080ca126:
	ldr r3, [sp, #32]
	adds r3, #2
	ldrsb r1, [r1, r3]
	movs r3, #1
	negs r3, r3
	cmp r1, r3
	beq .L_080ca152
	ldr r3, [sp, #12]
	adds r3, #24
	cmp r10, r3
	bne .L_080ca152
	ldr r4, [sp, #64]
	ldr r0, .L_080ca1d8
	movs r3, #4
	adds r2, r4, r0
	str r3, [r2]
	ldr r2, [sp, #36]
	ldr r4, [sp, #20]
	ldr r3, [r2]
	ldrsh r0, [r3, r4]
	bl BattleMotion_ApplyVariantMotionFar
.L_080ca152:
	ldr r3, [sp, #20]
	ldr r4, [sp, #16]
	ldr r0, [sp, #12]
	ldr r1, [sp, #8]
	ldr r2, [sp, #56]
	adds r3, #2
	subs r4, #32
	adds r0, #32
	adds r1, #3
	adds r2, #1
	str r4, [sp, #16]
	str r3, [sp, #20]
	str r0, [sp, #12]
	str r1, [sp, #8]
	str r2, [sp, #56]
	ldr r4, [sp, #36]
	ldr r3, [r4]
	ldr r3, [r3, #20]
	cmp r2, r3
	beq .L_080ca17c
	b .L_080c9f32
.L_080ca17c:
	movs r0, #8
	movs r1, #8
	bl Camera_ApplyShake
	bl ObjectGroup_TickMemberTimers
	ldr r1, .L_080ca1dc
	ldr r0, [sp, #64]
	movs r3, #1
	adds r2, r0, r1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	movs r2, #1
	ldr r3, [sp, #40]
	add r10, r2
	cmp r10, r3
	beq .L_080ca1a4
	b .L_080c9e9a
.L_080ca1a4:
	ldr r0, .L_080ca1e0
	bl Scheduler_RemoveCallback
	movs r0, #47
	bl Runtime_ReleaseHeapBlock
	movs r0, #46
	bl Runtime_ReleaseHeapBlock
	bl BattleFx_EndCanvasLayer
	add sp, #84
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
.L_080ca1cc:
	.4byte Data_080ededc + 0xc
.L_080ca1d0:
	.4byte Data_080ededc + 0x20
.L_080ca1d4:
	.4byte 0xffff8000
.L_080ca1d8:
	.4byte 0x000077a8
.L_080ca1dc:
	.4byte 0x00007824
.L_080ca1e0:
	.4byte BattlePresentation_ProcessPendingGraphicsTransfer
