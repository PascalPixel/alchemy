.syntax unified
	.thumb
	.global Unnamed_080ce034
	.thumb_func
Unnamed_080ce034:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r6, .L_080ce3f0
	adds r3, r6, #0
	ldmia r3!, {r1}
	sub sp, #60
	str r1, [sp, #36]
	ldr r3, [r3]
	str r3, [sp, #32]
	ldr r2, [r6, #8]
	str r2, [sp, #28]
	ldr r2, .L_080ce3f4
	adds r3, r1, r2
	str r0, [r3]
	movs r0, #0
	bl BattleFx_BeginCanvasLayer
	ldr r0, .L_080ce3f8
	bl Resource_GetTableEntry
	ldr r1, [sp, #28]
	bl Resource_DecodeType01
	ldr r0, .L_080ce3fc
	bl Resource_GetTableEntry
	adds r5, r0, #0
	movs r0, #160
	ldr r3, .L_080ce400
	adds r1, r5, #0
	movs r2, #128
	lsls r0, r0, #19
	bl _call_via_r3
	adds r5, #128
	ldr r1, [sp, #36]
	adds r0, r5, #0
	bl Resource_DecodeType01
	movs r5, #2
	movs r1, #7
	movs r2, #7
	movs r3, #3
	movs r0, #46
	str r5, [sp, #0]
	bl Unnamed_080ed408
	ldr r3, [r6, #28]
	movs r1, #7
	str r3, [sp, #40]
	movs r2, #7
	movs r3, #7
	movs r0, #47
	str r5, [sp, #0]
	bl Unnamed_080ed408
	ldr r3, [r6, #32]
	mov r0, sp
	adds r0, #40
	str r0, [sp, #8]
	str r3, [r0, #4]
	ldr r1, [sp, #36]
	movs r2, #239
	lsls r2, r2, #7
	adds r3, r1, r2
	str r5, [r3]
	ldr r3, .L_080ce404
	adds r2, r1, r3
	movs r3, #75
	movs r1, #144
	str r3, [r2]
	lsls r1, r1, #3
	ldr r0, .L_080ce408
	bl Scheduler_AddOrUpdateCallback
	movs r0, #0
	movs r2, #128
	ldr r3, .L_080ce40c
	mov r8, r0
	movs r1, #0
	lsls r2, r2, #3
.L_080ce0e0:
	movs r0, #1
	add r8, r0
	str r1, [r3]
	adds r3, #28
	cmp r8, r2
	bne .L_080ce0e0
	ldr r1, [sp, #36]
	ldr r2, .L_080ce3f4
	adds r5, r1, r2
	ldr r3, [r5]
	ldr r0, [r3, #8]
	bl GetBattleObjectSlotFar
	ldr r3, [r5]
	ldr r0, [r0]
	mov r10, r0
	movs r1, #36
	ldrsh r0, [r3, r1]
	bl GetBattleObjectSlotFar
	ldr r0, [r0]
	str r0, [sp, #24]
	mov r2, r10
	ldr r3, [r2, #8]
	ldr r0, .L_080ce410
	str r0, [sp, #20]
	cmp r3, #0
	bgt .L_080ce11e
	movs r1, #240
	lsls r1, r1, #12
	str r1, [sp, #20]
.L_080ce11e:
	ldr r0, .L_080ce3f4
	ldr r3, [sp, #36]
	mov r1, sp
	adds r0, r3, r0
	adds r1, #48
	movs r2, #0
	str r0, [sp, #16]
	str r1, [sp, #12]
	mov r11, r2
.L_080ce130:
	ldr r3, .L_080ce414
	ldr r5, [r3]
	bl Render_ResetTransformState
	adds r1, r5, #0
	adds r1, #12
	adds r0, r5, #0
	bl Graphics_PrepareTransferInIwramWork
	mov r2, r11
	cmp r2, #17
	bgt .L_080ce14c
	cmp r2, #0
	bne .L_080ce164
.L_080ce14c:
	ldr r0, [sp, #16]
	ldr r5, [sp, #12]
	ldr r3, [r0]
	adds r1, r5, #0
	ldr r0, [r3, #8]
	bl EffectPosition_ApplyAlternateStepAndYOffset
	ldr r3, [r5]
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	str r3, [r5]
.L_080ce164:
	mov r3, r11
	subs r3, #2
	cmp r3, #1
	bhi .L_080ce188
	ldr r5, [sp, #12]
	movs r1, #32
	ldr r2, [r5]
	ldr r3, [r5, #4]
	str r1, [sp, #0]
	movs r1, #64
	str r1, [sp, #4]
	subs r2, #16
	subs r3, #64
	ldr r4, [sp, #40]
	ldr r0, [sp, #32]
	ldr r1, [sp, #36]
	bl _call_via_r4
.L_080ce188:
	mov r2, r11
	subs r2, #4
	cmp r2, #11
	bhi .L_080ce1ec
	lsrs r3, r2, #31
	adds r3, r2, r3
	asrs r3, r3, #1
	movs r1, #0
	lsls r3, r3, #11
	mov r8, r1
	add r7, sp, #48
	mov r9, r3
.L_080ce1a0:
	mov r2, r8
	lsls r6, r2, #12
	adds r0, r6, #0
	bl Trig_Sin
	mov r3, r11
	muls r3, r0
	ldr r5, [r7]
	asrs r3, r3, #16
	adds r0, r6, #0
	adds r5, r5, r3
	bl Trig_Cos
	mov r2, r11
	muls r2, r0
	ldr r3, [r7, #4]
	asrs r2, r2, #16
	adds r3, r3, r2
	mov r0, r11
	movs r2, #32
	ldr r1, [sp, #36]
	subs r3, r3, r0
	str r2, [sp, #0]
	subs r5, #16
	movs r2, #64
	str r2, [sp, #4]
	add r1, r9
	adds r2, r5, #0
	subs r3, #64
	ldr r4, [sp, #40]
	ldr r0, [sp, #32]
	bl _call_via_r4
	movs r1, #1
	add r8, r1
	mov r2, r8
	cmp r2, #16
	bne .L_080ce1a0
.L_080ce1ec:
	mov r3, r11
	cmp r3, #4
	bne .L_080ce23a
	movs r3, #160
	mov r0, r10
	lsls r3, r3, #13
	str r3, [r0, #40]
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r0, #52]
	movs r3, #192
	lsls r3, r3, #10
	str r3, [r0, #48]
	ldr r3, .L_080ce418
	str r3, [r0, #72]
	mov r3, r10
	movs r2, #0
	adds r3, #90
	strb r2, [r3]
	subs r3, #2
	strb r2, [r3]
	ldr r3, [r0, #8]
	lsls r1, r3, #1
	adds r1, r1, r3
	ldr r3, [r0, #16]
	bl Object_SetMoveTargetFar
	mov r0, r10
	movs r1, #2
	bl Object_SetMode
	ldr r2, .L_080ce41c
	ldr r1, [sp, #36]
	mov r0, r11
	adds r3, r1, r2
	str r0, [r3]
	movs r0, #136
	bl AudioCommand_PlayFar
.L_080ce23a:
	mov r1, r11
	cmp r1, #16
	bne .L_080ce276
	ldr r0, .L_080ce420
	bl Resource_GetTableEntry
	adds r5, r0, #0
	movs r0, #160
	ldr r3, .L_080ce400
	adds r1, r5, #0
	movs r2, #128
	lsls r0, r0, #19
	bl _call_via_r3
	adds r5, #128
	adds r0, r5, #0
	ldr r1, [sp, #36]
	bl Resource_DecodeType01
	movs r3, #0
	mov r2, r10
	str r3, [r2, #72]
	str r3, [r2, #36]
	str r3, [r2, #40]
	ldr r0, [sp, #24]
	ldr r3, [r0, #16]
	mov r0, r10
	str r3, [r2, #16]
	bl Object_ResetMotion
.L_080ce276:
	mov r1, r11
	cmp r1, #17
	bgt .L_080ce27e
	b .L_080ce392
.L_080ce27e:
	mov r3, r10
	ldr r2, [r3, #12]
	cmp r2, #0
	ble .L_080ce2f0
	ldr r0, [sp, #20]
	ldr r3, [r3, #8]
	adds r3, r3, r0
	ldr r0, .L_080ce424
	mov r1, r10
	str r3, [r1, #8]
	adds r3, r2, r0
	str r3, [r1, #12]
	ldr r1, [sp, #16]
	ldr r3, [r1]
	ldr r3, [r3, #4]
	cmp r3, #0
	bne .L_080ce2c8
	ldr r5, [sp, #12]
	movs r1, #40
	ldr r2, [r5]
	ldr r3, [r5, #4]
	str r1, [sp, #0]
	movs r1, #64
	subs r2, #20
	subs r3, #52
	str r1, [sp, #4]
	ldr r4, [sp, #40]
	ldr r0, [sp, #32]
	ldr r1, [sp, #36]
	bl _call_via_r4
	ldr r3, [r5]
	subs r3, #8
	str r3, [r5]
	mov r3, r10
	ldr r2, [r3, #12]
	b .L_080ce2f0
.L_080ce2c8:
	ldr r5, [sp, #12]
	movs r1, #40
	ldr r2, [r5]
	ldr r3, [r5, #4]
	str r1, [sp, #0]
	movs r1, #64
	str r1, [sp, #4]
	ldr r0, [sp, #8]
	subs r2, #26
	subs r3, #52
	ldr r4, [r0, #4]
	ldr r1, [sp, #36]
	ldr r0, [sp, #32]
	bl _call_via_r4
	ldr r3, [r5, #4]
	adds r3, #8
	str r3, [r5, #4]
	mov r1, r10
	ldr r2, [r1, #12]
.L_080ce2f0:
	cmp r2, #0
	bge .L_080ce392
	movs r3, #0
	mov r2, r10
	str r3, [r2, #12]
	mov r8, r3
	add r3, sp, #48
	ldr r7, .L_080ce428
	mov r9, r3
.L_080ce302:
	bl Random16
	ldr r5, .L_080ce42c
	ands r5, r0
	bl Random16
	ldr r3, .L_080ce430
	adds r6, r0, #0
	mov r0, r9
	ands r6, r3
	ldr r3, [r0]
	lsls r3, r3, #16
	str r3, [r7]
	ldr r3, [r0, #4]
	subs r3, #24
	lsls r3, r3, #16
	str r3, [r7, #4]
	adds r0, r6, #0
	bl Trig_Sin
	adds r5, #32
	adds r3, r5, #0
	muls r3, r0
	asrs r3, r3, #6
	str r3, [r7, #8]
	adds r0, r6, #0
	bl Trig_Cos
	adds r3, r5, #0
	muls r3, r0
	lsls r3, r3, #1
	negs r3, r3
	asrs r3, r3, #6
	str r3, [r7, #16]
	bl Random16
	movs r3, #7
	ands r3, r0
	movs r1, #1
	movs r2, #128
	adds r3, #32
	add r8, r1
	lsls r2, r2, #1
	str r3, [r7, #24]
	adds r7, #28
	cmp r8, r2
	bne .L_080ce302
	ldr r0, [sp, #36]
	ldr r1, .L_080ce41c
	movs r5, #8
	adds r3, r0, r1
	str r5, [r3]
	movs r0, #145
	bl BattleEventRuntime_BeginPhaseFar
	ldr r2, [sp, #16]
	ldr r3, [r2]
	movs r1, #36
	ldrsh r0, [r3, r1]
	movs r1, #4
	bl BattleMotion_ApplyVariantMotionFar
	ldr r2, [sp, #16]
	ldr r3, [r2]
	movs r2, #5
	movs r1, #36
	ldrsh r0, [r3, r1]
	movs r1, #7
	movs r3, #0
	str r5, [sp, #0]
	bl ObjectGroup_UpdateMembers
.L_080ce392:
	movs r2, #0
	ldr r6, .L_080ce428
	mov r8, r2
.L_080ce398:
	ldr r4, [r6, #24]
	cmp r4, #0
	ble .L_080ce47a
	ldr r2, [r6, #8]
	ldr r3, [r6]
	adds r3, r3, r2
	mov r12, r3
	str r3, [r6]
	ldr r1, [r6, #16]
	ldr r3, [r6, #4]
	adds r7, r3, r1
	lsls r3, r2, #3
	subs r3, r3, r2
	subs r0, r4, #1
	lsls r3, r3, #3
	str r0, [r6, #24]
	str r7, [r6, #4]
	cmp r3, #0
	bge .L_080ce3c0
	adds r3, #63
.L_080ce3c0:
	asrs r3, r3, #6
	str r3, [r6, #8]
	lsls r3, r1, #3
	subs r3, r3, r1
	lsls r3, r3, #3
	cmp r3, #0
	bge .L_080ce3d0
	adds r3, #63
.L_080ce3d0:
	movs r1, #128
	asrs r3, r3, #6
	lsls r1, r1, #6
	movs r2, #224
	adds r3, r3, r1
	lsls r2, r2, #15
	str r3, [r6, #16]
	cmp r7, r2
	ble .L_080ce434
	negs r3, r3
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	str r3, [r6, #16]
	b .L_080ce47a
	.2byte 0x0000
.L_080ce3f0:
	.4byte gBattleFxWork
.L_080ce3f4:
	.4byte 0x00007828
.L_080ce3f8:
	.4byte 0x00000073
.L_080ce3fc:
	.4byte 0x0000007d
.L_080ce400:
	.4byte IwramCopyWords
.L_080ce404:
	.4byte 0x00007784
.L_080ce408:
	.4byte BattlePresentation_ProcessPendingGraphicsTransfer
.L_080ce40c:
	.4byte gMapCellBuffer + 0x18
.L_080ce410:
	.4byte 0xfff10000
.L_080ce414:
	.4byte gCameraWork
.L_080ce418:
	.4byte 0x0000ab85
.L_080ce41c:
	.4byte 0x000077a8
.L_080ce420:
	.4byte 0x00000089
.L_080ce424:
	.4byte 0xfff80000
.L_080ce428:
	.4byte gMapCellBuffer
.L_080ce42c:
	.4byte 0x000003ff
.L_080ce430:
	.4byte 0x0000ffff
.L_080ce434:
	ldr r3, .L_080ce4d8
	cmp r12, r3
	bhi .L_080ce47a
	cmp r7, #0
	blt .L_080ce47a
	cmp r0, #0
	bge .L_080ce444
	adds r0, r4, #6
.L_080ce444:
	asrs r0, r0, #3
	adds r0, #1
	lsls r5, r0, #1
	ldr r2, .L_080ce4dc
	mov r1, r8
	subs r3, r5, #2
	movs r4, #1
	ands r4, r1
	ldrh r1, [r2, r3]
	ldr r2, [sp, #28]
	mov r3, r12
	adds r1, r2, r1
	asrs r2, r3, #16
	lsrs r3, r0, #31
	adds r3, r0, r3
	asrs r3, r3, #1
	subs r2, r2, r3
	asrs r3, r7, #16
	str r0, [sp, #0]
	subs r3, r3, r0
	str r5, [sp, #4]
	ldr r0, [sp, #8]
	lsls r4, r4, #2
	ldr r4, [r4, r0]
	ldr r0, [sp, #32]
	bl _call_via_r4
.L_080ce47a:
	movs r1, #1
	movs r2, #128
	add r8, r1
	lsls r2, r2, #1
	adds r6, #28
	cmp r8, r2
	bne .L_080ce398
	movs r1, #16
	movs r0, #16
	bl Camera_ApplyShake
	bl ObjectGroup_TickMemberTimers
	ldr r0, .L_080ce4e0
	ldr r3, [sp, #36]
	adds r2, r3, r0
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	movs r1, #1
	add r11, r1
	mov r2, r11
	cmp r2, #88
	beq .L_080ce4b0
	b .L_080ce130
.L_080ce4b0:
	ldr r0, .L_080ce4e4
	bl Scheduler_RemoveCallback
	movs r0, #47
	bl Runtime_ReleaseHeapBlock
	movs r0, #46
	bl Runtime_ReleaseHeapBlock
	bl BattleFx_EndCanvasLayer
	add sp, #60
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
.L_080ce4d8:
	.4byte 0x007effff
.L_080ce4dc:
	.4byte ParticleStreams_CellOffsets
.L_080ce4e0:
	.4byte 0x00007824
.L_080ce4e4:
	.4byte BattlePresentation_ProcessPendingGraphicsTransfer
