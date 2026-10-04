@ Uncredited owned-ROM disassembly; ordinary draft: ../en/main/080db6e0.c.
.syntax unified
	.text
	.balign 4, 0
	.thumb
	.global BattleFx_RunParticleField
	.type BattleFx_RunParticleField, %function
	.thumb_func
BattleFx_RunParticleField:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #56
	str r1, [sp, #40]
	ldr r3, [pc, #832]
	ldmia r3!, {r1}
	ldr r5, [pc, #832]
	mov r10, r1
	ldr r3, [r3]
	add r5, r10
	str r3, [sp, #36]
	str r0, [r5]
	movs r0, #1
	bl BattleFx_BeginCanvasLayer
	movs r2, #1
	ldr r0, [pc, #816]
	mov r1, r10
	movs r3, #0
	bl Resource_LoadAndDecompress
	ldr r2, [sp, #40]
	cmp r2, #1
	bne .L0
	movs r3, #0
	movs r0, #160
	mov r8, r3
	lsls r0, r0, #19
.L1:
	mov r4, r8
	lsrs r3, r4, #31
	add r3, r8
	asrs r3, r3, #1
	lsls r1, r3, #5
	lsls r2, r3, #10
	orrs r2, r1
	movs r1, #1
	orrs r2, r3
	add r8, r1
	strh r2, [r0]
	mov r2, r8
	adds r0, #2
	cmp r2, #64
	bne .L1
	str r1, [sp, #24]
	b .L2
.L0:
	ldr r0, [pc, #760]
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, [pc, #756]
	lsls r0, r0, #19
	movs r2, #128
	bl _call_via_r3
	ldr r3, [r5]
	ldr r3, [r3, #24]
	str r3, [sp, #24]
.L2:
	movs r5, #225
	movs r3, #0
	lsls r5, r5, #7
	mov r8, r3
	movs r7, #0
	movs r6, #63
	add r5, r10
.L5:
	ldr r3, [pc, #712]
	add r3, r10
	ldr r3, [r3]
	ldr r3, [r3, #4]
	cmp r3, #1
	bne .L3
	movs r3, #200
	lsls r3, r3, #14
	b .L4
.L3:
	ldr r3, [pc, #712]
.L4:
	str r3, [r5]
	str r7, [r5, #4]
	str r7, [r5, #8]
	bl Random16
	ands r0, r6
	subs r0, #32
	lsls r0, r0, #13
	str r0, [r5, #12]
	bl Random16
	ands r0, r6
	adds r0, #16
	lsls r0, r0, #12
	str r0, [r5, #16]
	bl Random16
	ands r0, r6
	subs r0, #32
	movs r4, #1
	lsls r0, r0, #13
	add r8, r4
	str r0, [r5, #20]
	mov r0, r8
	str r7, [r5, #24]
	adds r5, #28
	cmp r0, #32
	bne .L5
	movs r1, #0
	ldr r5, [pc, #656]
	mov r8, r1
	movs r6, #0
	movs r7, #63
.L8:
	ldr r3, [pc, #628]
	add r3, r10
	ldr r3, [r3]
	ldr r3, [r3, #4]
	cmp r3, #1
	bne .L6
	movs r3, #200
	lsls r3, r3, #14
	b .L7
.L6:
	ldr r3, [pc, #624]
.L7:
	str r3, [r5]
	str r6, [r5, #4]
	str r6, [r5, #8]
	bl Random16
	ands r0, r7
	subs r0, #32
	lsls r0, r0, #13
	str r0, [r5, #12]
	bl Random16
	movs r3, #31
	ands r3, r0
	adds r3, #8
	lsls r3, r3, #13
	str r3, [r5, #16]
	bl Random16
	ands r0, r7
	subs r0, #32
	movs r2, #1
	movs r3, #128
	lsls r0, r0, #13
	add r8, r2
	lsls r3, r3, #3
	str r0, [r5, #20]
	str r6, [r5, #24]
	adds r5, #28
	cmp r8, r3
	bne .L8
	movs r5, #2
	movs r1, #7
	movs r2, #7
	movs r3, #3
	movs r0, #46
	str r5, [sp, #0]
	bl BattleEffect_LoadWork
	ldr r3, [pc, #556]
	adds r3, #184
	ldr r3, [r3]
	str r3, [sp, #28]
	movs r3, #239
	lsls r3, r3, #7
	ldr r2, [pc, #548]
	add r3, r10
	str r5, [r3]
	add r2, r10
	movs r3, #75
	movs r1, #144
	str r3, [r2]
	lsls r1, r1, #3
	ldr r0, [pc, #536]
	bl Scheduler_AddOrUpdateCallback
	ldr r1, [sp, #24]
	ldr r3, [sp, #24]
	lsls r1, r1, #1
	str r1, [sp, #12]
	adds r2, r1, r3
	ldr r0, [pc, #524]
	adds r1, r2, #2
	ldrb r3, [r0, r1]
	movs r4, #0
	mov r9, r4
	cmp r3, #0
	bne .L9
	b .L10
.L9:
	adds r4, r2, #0
	adds r4, #1
	str r2, [sp, #16]
	str r4, [sp, #8]
	str r1, [sp, #20]
	mov r11, r0
.L31:
	ldr r3, [pc, #500]
	ldr r5, [r3]
	bl Render_ResetTransformState
	adds r1, r5, #0
	adds r0, r5, #0
	adds r1, #12
	bl Graphics_PrepareTransferInIwramWork
	mov r0, r9
	cmp r0, #2
	bne .L11
	movs r0, #144
	bl AudioCommand_PlayFar
.L11:
	ldr r1, [sp, #12]
	ldr r2, [sp, #24]
	ldr r6, [pc, #460]
	adds r5, r1, r2
	adds r3, r5, #2
	ldrb r3, [r6, r3]
	subs r3, #48
	cmp r9, r3
	bne .L12
	movs r0, #133
	bl BattleEventRuntime_BeginPhaseFar
.L12:
	movs r3, #0
	mov r8, r3
	ldrb r3, [r6, r5]
	cmp r3, #0
	beq .L13
	ldr r6, [pc, #416]
.L18:
	ldr r3, [r6, #4]
	cmp r3, #0
	blt .L14
	add r5, sp, #44
	adds r0, r6, #0
	adds r1, r5, #0
	bl EffectPosition_ApplyBaseAndYOffset
	ldr r2, [r5]
	ldr r3, [pc, #376]
	asrs r2, r2, #1
	add r3, r10
	str r2, [r5]
	ldr r3, [r3]
	ldr r3, [r3, #4]
	lsls r3, r3, #5
	adds r2, r2, r3
	subs r2, #16
	str r2, [r5]
	ldr r2, [r5, #8]
	cmp r2, #159
	bgt .L15
	movs r3, #160
	str r3, [r5, #8]
	movs r2, #160
.L15:
	ldr r3, [pc, #388]
	cmp r2, r3
	ble .L16
	str r3, [r5, #8]
	adds r2, r3, #0
.L16:
	adds r3, r2, #0
	subs r3, #160
	cmp r3, #0
	bge .L17
	adds r3, #63
.L17:
	asrs r3, r3, #6
	movs r0, #9
	subs r0, r0, r3
	lsls r4, r0, #1
	ldr r2, [pc, #364]
	subs r3, r4, #2
	ldrh r1, [r2, r3]
	mov r3, r8
	movs r2, #1
	ands r2, r3
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #7
	adds r3, r3, r2
	lsls r3, r3, #1
	adds r1, r1, r3
	movs r2, #200
	lsls r2, r2, #6
	lsrs r3, r0, #31
	add r1, r10
	adds r1, r1, r2
	adds r3, r0, r3
	ldr r2, [r5]
	asrs r3, r3, #1
	subs r2, r2, r3
	ldr r3, [r5, #4]
	str r0, [sp, #0]
	subs r3, r3, r0
	str r4, [sp, #4]
	ldr r0, [sp, #36]
	ldr r4, [sp, #28]
	bl _call_via_r4
	adds r0, r6, #0
	movs r1, #64
	ldr r2, [pc, #304]
	bl EffectStep_AdvanceWithGravity3D
.L14:
	ldr r2, [sp, #16]
	mov r1, r11
	movs r0, #1
	ldrb r3, [r1, r2]
	add r8, r0
	adds r6, #28
	cmp r8, r3
	bne .L18
.L13:
	mov r3, r9
	cmp r3, #2
	ble .L19
	ldr r0, [sp, #8]
	mov r2, r11
	ldrb r3, [r2, r0]
	movs r4, #0
	mov r8, r4
	cmp r3, #0
	beq .L19
	movs r6, #225
	lsls r6, r6, #7
	add r6, r10
.L23:
	cmp r8, r9
	bge .L20
	ldr r3, [r6, #4]
	cmp r3, #0
	blt .L20
	add r5, sp, #44
	adds r0, r6, #0
	adds r1, r5, #0
	bl EffectPosition_ApplyBaseAndYOffset
	ldr r2, [r5]
	ldr r3, [pc, #184]
	asrs r2, r2, #1
	add r3, r10
	str r2, [r5]
	ldr r3, [r3]
	ldr r3, [r3, #4]
	lsls r3, r3, #5
	adds r2, r2, r3
	adds r7, r2, #0
	subs r7, #16
	str r7, [r5]
	ldr r0, [r6, #24]
	cmp r0, #20
	bhi .L21
	movs r1, #3
	bl __divsi3
	ldr r3, [pc, #204]
	lsls r0, r0, #1
	ldrh r1, [r3, r0]
	ldr r3, [pc, #204]
	ldrh r4, [r3, r0]
	ldr r3, [r5, #4]
	lsrs r0, r4, #1
	subs r2, r7, r0
	subs r3, r3, r0
	str r4, [sp, #0]
	str r4, [sp, #4]
	add r1, r10
	ldr r0, [sp, #36]
	ldr r4, [sp, #28]
	bl _call_via_r4
	ldr r0, [r6, #24]
.L21:
	cmp r0, #20
	bgt .L22
	adds r3, r0, #1
	str r3, [r6, #24]
.L22:
	ldr r2, [pc, #160]
	adds r0, r6, #0
	movs r1, #64
	bl EffectStep_AdvanceWithGravity3D
	ldr r2, [pc, #132]
.L20:
	ldr r1, [sp, #8]
	movs r0, #1
	ldrb r3, [r2, r1]
	add r8, r0
	adds r6, #28
	cmp r8, r3
	bne .L23
.L19:
	ldr r2, [sp, #40]
	cmp r2, #0
	bne .L24
	ldr r2, [pc, #76]
	movs r3, #0
	add r2, r10
	mov r8, r3
	ldr r3, [r2]
	ldr r3, [r3, #20]
	cmp r3, #0
	beq .L25
	adds r5, r2, #0
	movs r6, #36
.L27:
	mov r3, r8
	adds r3, #6
	cmp r9, r3
	bne .L26
	ldr r3, [r5]
	ldrsh r0, [r3, r6]
	movs r3, #10
	movs r1, #7
	str r3, [sp, #0]
	movs r2, #5
	mov r3, r8
	bl ObjectGroup_UpdateMembers
	ldr r3, [r5]
	ldrsh r0, [r3, r6]
	movs r1, #2
	bl BattleMotion_ApplyVariantMotionFar
.L26:
	ldr r3, [r5]
	movs r2, #1
	ldr r3, [r3, #20]
	add r8, r2
	adds r6, #2
	cmp r8, r3
	bne .L27
	b .L25
	.balign 4, 0
	.4byte gBattleFxWork
	.4byte 0x00007828
	.4byte ResourceId_BlastSheet
	.4byte ResourceId_FireStreakSheet
	.4byte IwramCopyWords
	.4byte 0xffce0000
	.4byte gMapCellBuffer
	.4byte gWorkSlot
	.4byte 0x00007784
	.4byte BattlePresentation_ProcessPendingGraphicsTransfer
	.4byte ParticleField_Counts
	.4byte gCameraWork
	.4byte 0x0000031f
	.4byte ParticleStreams_CellOffsets
	.4byte 0xffffe000
	.4byte ParticleField_PuffCells
	.4byte ParticleField_PuffSizes
.L24:
	ldr r2, [pc, #152]
	movs r3, #0
	mov r4, r10
	mov r8, r3
	ldr r3, [r4, r2]
	ldr r3, [r3, #20]
	cmp r3, #0
	beq .L25
	movs r5, #36
.L29:
	mov r3, r8
	adds r3, #6
	cmp r9, r3
	bne .L28
	mov r0, r10
	ldr r3, [r0, r2]
	ldrsh r0, [r3, r5]
	movs r3, #10
	str r3, [sp, #0]
	movs r1, #7
	movs r2, #5
	mov r3, r8
	bl ObjectGroup_UpdateMembers
.L28:
	movs r2, #1
	add r8, r2
	ldr r2, [pc, #104]
	mov r4, r10
	ldr r3, [r4, r2]
	ldr r3, [r3, #20]
	adds r5, #2
	cmp r8, r3
	bne .L29
.L25:
	mov r0, r9
	cmp r0, #2
	bne .L30
	ldr r2, [pc, #88]
	movs r3, #6
	add r2, r10
	str r3, [r2]
.L30:
	movs r1, #16
	movs r0, #16
	bl Camera_ApplyShake
	bl ObjectGroup_TickMemberTimers
	ldr r3, [pc, #72]
	movs r2, #1
	add r3, r10
	str r2, [r3]
	movs r0, #1
	bl WaitFrames
	ldr r4, [sp, #20]
	mov r2, r11
	movs r1, #1
	ldrb r3, [r2, r4]
	add r9, r1
	cmp r9, r3
	beq .L10
	b .L31
.L10:
	ldr r0, [pc, #44]
	bl Scheduler_RemoveCallback
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
	.balign 4, 0
	.4byte 0x00007828
	.4byte 0x000077a8
	.4byte 0x00007824
	.4byte BattlePresentation_ProcessPendingGraphicsTransfer
	.size BattleFx_RunParticleField, .-BattleFx_RunParticleField
