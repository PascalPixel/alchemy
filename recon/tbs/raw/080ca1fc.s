.syntax unified
	.thumb
	.global BattleFx_RunParticlePool
	.thumb_func
BattleFx_RunParticlePool:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #68
	ldr r2, .L_080ca534
	str r1, [sp, #40]
	adds r3, r2, #0
	ldmia r3!, {r1}
	str r1, [sp, #36]
	ldr r3, [r3]
	str r3, [sp, #32]
	adds r3, r2, #0
	subs r3, #108
	ldr r3, [r3]
	str r3, [sp, #20]
	ldr r2, [r2, #8]
	str r2, [sp, #16]
	ldr r2, .L_080ca538
	adds r3, r1, r2
	str r0, [r3]
	ldr r3, [sp, #40]
	cmp r3, #0
	bne .L_080ca23a
	movs r0, #0
	bl BattleFx_BeginCanvasLayer
	b .L_080ca240
.L_080ca23a:
	movs r0, #1
	bl BattleFx_BeginCanvasLayer
.L_080ca240:
	movs r6, #2
	movs r1, #7
	movs r2, #7
	movs r3, #3
	movs r0, #46
	str r6, [sp, #0]
	bl BattleEffect_LoadWork
	ldr r5, .L_080ca53c
	adds r3, r5, #0
	adds r3, #184
	ldr r3, [r3]
	movs r1, #7
	str r3, [sp, #24]
	movs r2, #7
	movs r3, #11
	movs r0, #47
	str r6, [sp, #0]
	bl BattleEffect_LoadWork
	adds r5, #188
	ldr r5, [r5]
	ldr r0, .L_080ca540
	ldr r1, [sp, #16]
	movs r2, #0
	movs r3, #0
	str r5, [sp, #28]
	bl Resource_LoadAndDecompress
	ldr r4, [sp, #40]
	cmp r4, #0
	bne .L_080ca284
	ldr r0, .L_080ca544
	b .L_080ca286
.L_080ca284:
	ldr r0, .L_080ca548
.L_080ca286:
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_080ca54c
	movs r2, #128
	lsls r0, r0, #19
	bl _call_via_r3
	movs r3, #239
	ldr r1, [sp, #36]
	ldr r4, .L_080ca550
	lsls r3, r3, #7
	adds r2, r1, r3
	movs r3, #2
	str r3, [r2]
	adds r2, r1, r4
	movs r3, #75
	movs r1, #144
	str r3, [r2]
	lsls r1, r1, #3
	ldr r0, .L_080ca554
	bl Scheduler_AddOrUpdateCallback
	ldr r2, .L_080ca538
	ldr r1, [sp, #36]
	adds r3, r1, r2
	ldr r3, [r3]
	ldr r0, [r3, #8]
	bl GetBattleObjectSlotFar
	ldr r0, [r0]
	movs r3, #0
	str r0, [sp, #12]
	ldr r7, .L_080ca558
	mov r8, r3
.L_080ca2ce:
	bl Random16
	ldr r5, .L_080ca55c
	ands r5, r0
	bl Random16
	ldr r3, .L_080ca560
	ldr r4, [sp, #12]
	adds r6, r0, #0
	ands r6, r3
	ldr r3, [r4, #8]
	str r3, [r7]
	movs r1, #160
	ldr r3, [r4, #12]
	lsls r1, r1, #11
	adds r3, r3, r1
	str r3, [r7, #4]
	ldr r3, [r4, #16]
	adds r0, r6, #0
	str r3, [r7, #8]
	bl Trig_Sin
	adds r5, #32
	adds r3, r5, #0
	muls r3, r0
	asrs r3, r3, #8
	str r3, [r7, #12]
	bl Random16
	movs r3, #255
	ands r3, r0
	subs r3, #32
	lsls r3, r3, #9
	str r3, [r7, #16]
	adds r0, r6, #0
	bl Trig_Cos
	adds r3, r5, #0
	muls r3, r0
	lsls r3, r3, #1
	negs r3, r3
	asrs r3, r3, #8
	str r3, [r7, #20]
	bl Random16
	movs r3, #31
	ands r3, r0
	adds r3, #48
	str r3, [r7, #24]
	ldr r2, [sp, #40]
	cmp r2, #0
	bne .L_080ca34a
	ldr r3, [r7, #12]
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	str r3, [r7, #12]
	ldr r3, [r7, #20]
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	str r3, [r7, #20]
.L_080ca34a:
	movs r3, #1
	movs r4, #128
	add r8, r3
	lsls r4, r4, #1
	adds r7, #28
	cmp r8, r4
	bne .L_080ca2ce
	ldr r2, [sp, #20]
	movs r3, #56
	adds r2, #12
	movs r1, #0
	add r3, sp
	str r2, [sp, #8]
	mov r11, r1
	mov r9, r3
.L_080ca368:
	bl Render_ResetTransformState
	ldr r0, [sp, #20]
	ldr r1, [sp, #8]
	bl Graphics_PrepareTransferInIwramWork
	ldr r6, .L_080ca558
	movs r4, #0
	mov r8, r4
.L_080ca37a:
	mov r3, r8
	cmp r3, #0
	bge .L_080ca382
	adds r3, #31
.L_080ca382:
	asrs r3, r3, #5
	lsls r3, r3, #3
	cmp r11, r3
	blt .L_080ca440
	ldr r3, [r6, #24]
	cmp r3, #0
	blt .L_080ca440
	mov r1, r8
	lsls r0, r1, #2
	adds r0, r0, r3
	lsls r0, r0, #10
	bl Trig_Sin
	ldr r3, [r6]
	lsls r0, r0, #4
	mov r2, r9
	adds r3, r3, r0
	str r3, [r2]
	ldr r3, [r6, #4]
	str r3, [r2, #4]
	ldr r3, [r6, #8]
	add r5, sp, #44
	str r3, [r2, #8]
	mov r0, r9
	adds r1, r5, #0
	bl EffectPosition_ApplyBaseAndYOffset
	ldr r3, [r5]
	asrs r3, r3, #1
	str r3, [r5]
	ldr r4, .L_080ca564
	ldr r3, [r5, #8]
	cmp r3, r4
	bgt .L_080ca3cc
	movs r3, #157
	lsls r3, r3, #1
	str r3, [r5, #8]
.L_080ca3cc:
	ldr r2, .L_080ca568
	cmp r3, r2
	ble .L_080ca3d6
	str r2, [r5, #8]
	adds r3, r2, #0
.L_080ca3d6:
	ldr r1, .L_080ca56c
	adds r2, r3, r1
	cmp r2, #0
	bge .L_080ca3e2
	adds r2, r3, #0
	subs r2, #251
.L_080ca3e2:
	asrs r3, r2, #6
	movs r0, #6
	subs r0, r0, r3
	lsls r4, r0, #1
	ldr r2, .L_080ca570
	subs r3, r4, #2
	ldrh r1, [r2, r3]
	ldr r2, [sp, #16]
	lsrs r3, r0, #31
	adds r1, r2, r1
	adds r3, r0, r3
	ldr r2, [r5]
	asrs r3, r3, #1
	subs r2, r2, r3
	ldr r3, [r5, #4]
	str r0, [sp, #0]
	subs r3, r3, r0
	str r4, [sp, #4]
	ldr r0, [sp, #32]
	ldr r4, [sp, #24]
	bl _call_via_r4
	movs r2, #128
	movs r1, #62
	adds r0, r6, #0
	lsls r2, r2, #3
	bl EffectStep_AdvanceWithGravity3D
	ldr r1, [sp, #40]
	cmp r1, #1
	bne .L_080ca43a
	ldr r2, [sp, #12]
	ldr r3, [r2, #8]
	cmp r3, #0
	bge .L_080ca432
	ldr r3, [r6, #12]
	movs r4, #128
	lsls r4, r4, #6
	adds r3, r3, r4
	b .L_080ca438
.L_080ca432:
	ldr r3, [r6, #12]
	ldr r1, .L_080ca574
	adds r3, r3, r1
.L_080ca438:
	str r3, [r6, #12]
.L_080ca43a:
	ldr r3, [r6, #24]
	subs r3, #1
	str r3, [r6, #24]
.L_080ca440:
	movs r2, #1
	add r8, r2
	mov r3, r8
	adds r6, #28
	cmp r3, #128
	bne .L_080ca37a
	ldr r4, [sp, #40]
	cmp r4, #1
	bne .L_080ca49a
	ldr r3, [sp, #36]
	ldr r4, .L_080ca538
	adds r2, r3, r4
	ldr r3, [r2]
	ldr r3, [r3, #20]
	movs r1, #0
	mov r8, r1
	cmp r3, #0
	beq .L_080ca4ea
	adds r5, r2, #0
	movs r7, #36
	movs r6, #48
.L_080ca46a:
	cmp r11, r6
	bne .L_080ca488
	movs r0, #1
	negs r0, r0
	bl BattleEventRuntime_BeginPhaseFar
	ldr r3, [r5]
	ldrsh r0, [r3, r7]
	movs r3, #8
	str r3, [sp, #0]
	movs r1, #7
	movs r2, #5
	mov r3, r8
	bl ObjectGroup_UpdateMembers
.L_080ca488:
	ldr r3, [r5]
	movs r2, #1
	ldr r3, [r3, #20]
	add r8, r2
	adds r7, #2
	adds r6, #8
	cmp r8, r3
	bne .L_080ca46a
	b .L_080ca4ea
.L_080ca49a:
	ldr r4, [sp, #36]
	ldr r1, .L_080ca538
	movs r3, #0
	adds r2, r4, r1
	mov r8, r3
	ldr r3, [r2]
	ldr r3, [r3, #20]
	cmp r3, #0
	beq .L_080ca4ea
	movs r3, #1
	negs r3, r3
	mov r10, r3
	adds r5, r2, #0
	movs r7, #36
	movs r6, #48
.L_080ca4b8:
	cmp r11, r6
	bne .L_080ca4da
	movs r0, #126
	bl AudioCommand_PlayFar
	mov r0, r10
	bl BattleEventRuntime_BeginPhaseFar
	ldr r3, [r5]
	ldrsh r0, [r3, r7]
	movs r3, #8
	str r3, [sp, #0]
	movs r1, #7
	mov r2, r10
	mov r3, r8
	bl ObjectGroup_UpdateMembers
.L_080ca4da:
	ldr r3, [r5]
	movs r1, #1
	ldr r3, [r3, #20]
	add r8, r1
	adds r7, #2
	adds r6, #8
	cmp r8, r3
	bne .L_080ca4b8
.L_080ca4ea:
	bl ObjectGroup_TickMemberTimers
	ldr r4, .L_080ca578
	ldr r3, [sp, #36]
	adds r2, r3, r4
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	movs r1, #1
	add r11, r1
	mov r2, r11
	cmp r2, #128
	beq .L_080ca50a
	b .L_080ca368
.L_080ca50a:
	ldr r0, .L_080ca554
	bl Scheduler_RemoveCallback
	movs r0, #47
	bl Runtime_ReleaseHeapBlock
	movs r0, #46
	bl Runtime_ReleaseHeapBlock
	bl BattleFx_EndCanvasLayer
	add sp, #68
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
.L_080ca534:
	.4byte gBattleFxWork
.L_080ca538:
	.4byte 0x00007828
.L_080ca53c:
	.4byte gWorkSlot
.L_080ca540:
	.4byte 0x00000073
.L_080ca544:
	.4byte 0x0000007c
.L_080ca548:
	.4byte 0x0000007b
.L_080ca54c:
	.4byte IwramCopyWords
.L_080ca550:
	.4byte 0x00007784
.L_080ca554:
	.4byte BattlePresentation_ProcessPendingGraphicsTransfer
.L_080ca558:
	.4byte gMapCellBuffer
.L_080ca55c:
	.4byte 0x000003ff
.L_080ca560:
	.4byte 0x0000ffff
.L_080ca564:
	.4byte 0x00000139
.L_080ca568:
	.4byte 0x0000027a
.L_080ca56c:
	.4byte 0xfffffec6
.L_080ca570:
	.4byte ParticleStreams_CellOffsets
.L_080ca574:
	.4byte 0xffffe000
.L_080ca578:
	.4byte 0x00007824
