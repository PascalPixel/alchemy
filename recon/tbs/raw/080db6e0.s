.syntax unified
	.thumb
	.global RunParticleFieldEffect
	.thumb_func
RunParticleFieldEffect:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #56
	str r1, [sp, #40]
	ldr r3, .L_080dba34
	ldmia r3!, {r1}
	ldr r5, .L_080dba38
	mov r10, r1
	ldr r3, [r3]
	add r5, r10
	str r3, [sp, #36]
	str r0, [r5]
	movs r0, #1
	bl BattleFx_BeginCanvasLayer
	movs r2, #1
	ldr r0, .L_080dba3c
	mov r1, r10
	movs r3, #0
	bl Resource_LoadAndDecompress
	ldr r2, [sp, #40]
	cmp r2, #1
	bne .L_080db744
	movs r3, #0
	movs r0, #160
	mov r8, r3
	lsls r0, r0, #19
.L_080db722:
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
	bne .L_080db722
	str r1, [sp, #24]
	b .L_080db75e
.L_080db744:
	ldr r0, .L_080dba40
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_080dba44
	lsls r0, r0, #19
	movs r2, #128
	bl _call_via_r3
	ldr r3, [r5]
	ldr r3, [r3, #24]
	str r3, [sp, #24]
.L_080db75e:
	movs r5, #225
	movs r3, #0
	lsls r5, r5, #7
	mov r8, r3
	movs r7, #0
	movs r6, #63
	add r5, r10
.L_080db76c:
	ldr r3, .L_080dba38
	add r3, r10
	ldr r3, [r3]
	ldr r3, [r3, #4]
	cmp r3, #1
	bne .L_080db77e
	movs r3, #200
	lsls r3, r3, #14
	b .L_080db780
.L_080db77e:
	ldr r3, .L_080dba48
.L_080db780:
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
	bne .L_080db76c
	movs r1, #0
	ldr r5, .L_080dba4c
	mov r8, r1
	movs r6, #0
	movs r7, #63
.L_080db7c2:
	ldr r3, .L_080dba38
	add r3, r10
	ldr r3, [r3]
	ldr r3, [r3, #4]
	cmp r3, #1
	bne .L_080db7d4
	movs r3, #200
	lsls r3, r3, #14
	b .L_080db7d6
.L_080db7d4:
	ldr r3, .L_080dba48
.L_080db7d6:
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
	bne .L_080db7c2
	movs r5, #2
	movs r1, #7
	movs r2, #7
	movs r3, #3
	movs r0, #46
	str r5, [sp, #0]
	bl Unnamed_080ed408
	ldr r3, .L_080dba50
	adds r3, #184
	ldr r3, [r3]
	str r3, [sp, #28]
	movs r3, #239
	lsls r3, r3, #7
	ldr r2, .L_080dba54
	add r3, r10
	str r5, [r3]
	add r2, r10
	movs r3, #75
	movs r1, #144
	str r3, [r2]
	lsls r1, r1, #3
	ldr r0, .L_080dba58
	bl Scheduler_AddOrUpdateCallback
	ldr r1, [sp, #24]
	ldr r3, [sp, #24]
	lsls r1, r1, #1
	str r1, [sp, #12]
	adds r2, r1, r3
	ldr r0, .L_080dba5c
	adds r1, r2, #2
	ldrb r3, [r0, r1]
	movs r4, #0
	mov r9, r4
	cmp r3, #0
	bne .L_080db85e
	b .L_080dbaf0
.L_080db85e:
	adds r4, r2, #0
	adds r4, #1
	str r2, [sp, #16]
	str r4, [sp, #8]
	str r1, [sp, #20]
	mov r11, r0
.L_080db86a:
	ldr r3, .L_080dba60
	ldr r5, [r3]
	bl Render_ResetTransformState
	adds r1, r5, #0
	adds r0, r5, #0
	adds r1, #12
	bl Graphics_PrepareTransferInIwramWork
	mov r0, r9
	cmp r0, #2
	bne .L_080db888
	movs r0, #144
	bl AudioCommand_PlayFar
.L_080db888:
	ldr r1, [sp, #12]
	ldr r2, [sp, #24]
	ldr r6, .L_080dba5c
	adds r5, r1, r2
	adds r3, r5, #2
	ldrb r3, [r6, r3]
	subs r3, #48
	cmp r9, r3
	bne .L_080db8a0
	movs r0, #133
	bl BattleEventRuntime_BeginPhaseFar
.L_080db8a0:
	movs r3, #0
	mov r8, r3
	ldrb r3, [r6, r5]
	cmp r3, #0
	beq .L_080db94e
	ldr r6, .L_080dba4c
.L_080db8ac:
	ldr r3, [r6, #4]
	cmp r3, #0
	blt .L_080db93e
	add r5, sp, #44
	adds r0, r6, #0
	adds r1, r5, #0
	bl EffectPosition_ApplyBaseAndYOffset
	ldr r2, [r5]
	ldr r3, .L_080dba38
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
	bgt .L_080db8de
	movs r3, #160
	str r3, [r5, #8]
	movs r2, #160
.L_080db8de:
	ldr r3, .L_080dba64
	cmp r2, r3
	ble .L_080db8e8
	str r3, [r5, #8]
	adds r2, r3, #0
.L_080db8e8:
	adds r3, r2, #0
	subs r3, #160
	cmp r3, #0
	bge .L_080db8f2
	adds r3, #63
.L_080db8f2:
	asrs r3, r3, #6
	movs r0, #9
	subs r0, r0, r3
	lsls r4, r0, #1
	ldr r2, .L_080dba68
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
	ldr r2, .L_080dba6c
	bl EffectStep_AdvanceWithGravity3D
.L_080db93e:
	ldr r2, [sp, #16]
	mov r1, r11
	movs r0, #1
	ldrb r3, [r1, r2]
	add r8, r0
	adds r6, #28
	cmp r8, r3
	bne .L_080db8ac
.L_080db94e:
	mov r3, r9
	cmp r3, #2
	ble .L_080db9e4
	ldr r0, [sp, #8]
	mov r2, r11
	ldrb r3, [r2, r0]
	movs r4, #0
	mov r8, r4
	cmp r3, #0
	beq .L_080db9e4
	movs r6, #225
	lsls r6, r6, #7
	add r6, r10
.L_080db968:
	cmp r8, r9
	bge .L_080db9d6
	ldr r3, [r6, #4]
	cmp r3, #0
	blt .L_080db9d6
	add r5, sp, #44
	adds r0, r6, #0
	adds r1, r5, #0
	bl EffectPosition_ApplyBaseAndYOffset
	ldr r2, [r5]
	ldr r3, .L_080dba38
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
	bhi .L_080db9c2
	movs r1, #3
	bl __divsi3
	ldr r3, .L_080dba70
	lsls r0, r0, #1
	ldrh r1, [r3, r0]
	ldr r3, .L_080dba74
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
.L_080db9c2:
	cmp r0, #20
	bgt .L_080db9ca
	adds r3, r0, #1
	str r3, [r6, #24]
.L_080db9ca:
	ldr r2, .L_080dba6c
	adds r0, r6, #0
	movs r1, #64
	bl EffectStep_AdvanceWithGravity3D
	ldr r2, .L_080dba5c
.L_080db9d6:
	ldr r1, [sp, #8]
	movs r0, #1
	ldrb r3, [r2, r1]
	add r8, r0
	adds r6, #28
	cmp r8, r3
	bne .L_080db968
.L_080db9e4:
	ldr r2, [sp, #40]
	cmp r2, #0
	bne .L_080dba78
	ldr r2, .L_080dba38
	movs r3, #0
	add r2, r10
	mov r8, r3
	ldr r3, [r2]
	ldr r3, [r3, #20]
	cmp r3, #0
	beq .L_080dbab8
	adds r5, r2, #0
	movs r6, #36
.L_080db9fe:
	mov r3, r8
	adds r3, #6
	cmp r9, r3
	bne .L_080dba22
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
.L_080dba22:
	ldr r3, [r5]
	movs r2, #1
	ldr r3, [r3, #20]
	add r8, r2
	adds r6, #2
	cmp r8, r3
	bne .L_080db9fe
	b .L_080dbab8
	.2byte 0x0000
.L_080dba34:
	.4byte gBattleFxWork
.L_080dba38:
	.4byte 0x00007828
.L_080dba3c:
	.4byte 0x000000c0
.L_080dba40:
	.4byte 0x00000096
.L_080dba44:
	.4byte IwramCopyWords
.L_080dba48:
	.4byte 0xffce0000
.L_080dba4c:
	.4byte gMapCellBuffer
.L_080dba50:
	.4byte gWorkSlot
.L_080dba54:
	.4byte 0x00007784
.L_080dba58:
	.4byte BattlePresentation_ProcessPendingGraphicsTransfer
.L_080dba5c:
	.4byte Data_080eeae2
.L_080dba60:
	.4byte gCameraWork
.L_080dba64:
	.4byte 0x0000031f
.L_080dba68:
	.4byte ParticleStreams_CellOffsets
.L_080dba6c:
	.4byte 0xffffe000
.L_080dba70:
	.4byte Data_080eeaec
.L_080dba74:
	.4byte Data_080eeafa
.L_080dba78:
	ldr r2, .L_080dbb14
	movs r3, #0
	mov r4, r10
	mov r8, r3
	ldr r3, [r4, r2]
	ldr r3, [r3, #20]
	cmp r3, #0
	beq .L_080dbab8
	movs r5, #36
.L_080dba8a:
	mov r3, r8
	adds r3, #6
	cmp r9, r3
	bne .L_080dbaa6
	mov r0, r10
	ldr r3, [r0, r2]
	ldrsh r0, [r3, r5]
	movs r3, #10
	str r3, [sp, #0]
	movs r1, #7
	movs r2, #5
	mov r3, r8
	bl ObjectGroup_UpdateMembers
.L_080dbaa6:
	movs r2, #1
	add r8, r2
	ldr r2, .L_080dbb14
	mov r4, r10
	ldr r3, [r4, r2]
	ldr r3, [r3, #20]
	adds r5, #2
	cmp r8, r3
	bne .L_080dba8a
.L_080dbab8:
	mov r0, r9
	cmp r0, #2
	bne .L_080dbac6
	ldr r2, .L_080dbb18
	movs r3, #6
	add r2, r10
	str r3, [r2]
.L_080dbac6:
	movs r1, #16
	movs r0, #16
	bl Camera_ApplyShake
	bl ObjectGroup_TickMemberTimers
	ldr r3, .L_080dbb1c
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
	beq .L_080dbaf0
	b .L_080db86a
.L_080dbaf0:
	ldr r0, .L_080dbb20
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
	.2byte 0x0000
.L_080dbb14:
	.4byte 0x00007828
.L_080dbb18:
	.4byte 0x000077a8
.L_080dbb1c:
	.4byte 0x00007824
.L_080dbb20:
	.4byte BattlePresentation_ProcessPendingGraphicsTransfer
