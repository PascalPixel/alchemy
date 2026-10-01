.syntax unified
	.thumb
	.global Unnamed_080ea0d8
	.thumb_func
Unnamed_080ea0d8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r5, .L_080ea158
	ldr r1, [r5]
	sub sp, #184
	str r1, [sp, #104]
	subs r3, r5, #4
	ldr r3, [r3]
	str r3, [sp, #100]
	ldr r2, [r5, #16]
	str r2, [sp, #96]
	ldr r3, [r5, #4]
	str r3, [sp, #80]
	adds r3, r5, #0
	ldr r4, [sp, #100]
	ldr r1, .L_080ea15c
	subs r3, #112
	ldr r3, [r3]
	adds r6, r4, r1
	str r3, [sp, #76]
	str r0, [r6]
	movs r0, #128
	lsls r0, r0, #6
	bl BattleFx_BeginCanvasLayer
	ldr r2, .L_080ea160
	ldr r3, .L_080ea150
	mov r8, r2
	mov r4, r8
	strh r3, [r4]
	bl BattlePres_ConfigureEffectDisplay
	ldr r2, .L_080ea154
	movs r3, #160
	lsls r3, r3, #19
	strh r2, [r3]
	adds r3, #2
	strh r2, [r3]
	movs r1, #7
	movs r2, #7
	movs r3, #3
	movs r0, #46
	str r3, [sp, #0]
	bl Unnamed_080ed408
	ldr r5, [r5, #24]
	movs r0, #239
	str r5, [sp, #88]
	ldr r5, [sp, #100]
	lsls r0, r0, #7
	adds r3, r5, r0
	movs r1, #144
	movs r5, #0
	str r5, [r3]
	b .L_080ea164
.L_080ea150:
	.4byte 0x00000100
.L_080ea154:
	.4byte 0x00000000
.L_080ea158:
	.4byte gBattleFxWork + 0x4
.L_080ea15c:
	.4byte 0x00007828
.L_080ea160:
	.4byte 0x04000020
.L_080ea164:
	lsls r1, r1, #3
	ldr r0, .L_080ea1f4
	bl Scheduler_AddOrUpdateCallback
	movs r1, #0
	movs r0, #0
	bl BattleEffect_WipeCanvas
	ldr r0, [r6]
	bl BattleFx_SelectLivingTargets
	movs r1, #191
	lsls r1, r1, #1
	movs r0, #16
	movs r2, #1
	bl BattleFx_SpawnObjects
	ldr r2, .L_080ea1f8
	movs r3, #240
	str r3, [r2, #16]
	movs r0, #1
	bl WaitFrames
	movs r2, #0
	ldr r1, .L_080ea1fc
	movs r0, #1
	bl BattleBackground_LoadFar
	ldr r1, [sp, #96]
	movs r3, #1
	str r3, [r1, #16]
	ldr r3, .L_080ea200
	movs r0, #0
	strh r5, [r3, #4]
	movs r1, #1
	bl BattleEffect_WipeCanvas
	ldr r3, .L_080ea1e4
	movs r2, #128
	lsls r2, r2, #19
	strh r3, [r2]
	ldr r3, .L_080ea1e8
	mov r2, r8
	strh r3, [r2]
	ldr r2, .L_080ea204
	ldr r3, .L_080ea1ec
	strh r3, [r2]
	ldr r3, .L_080ea1f0
	subs r2, #2
	strh r3, [r2]
	ldr r3, .L_080ea208
	ldr r5, .L_080ea20c
	movs r4, #128
	str r3, [sp, #72]
	ldr r1, [sp, #100]
	lsls r4, r4, #12
	movs r6, #0
	ldr r0, .L_080ea210
	movs r2, #1
	movs r3, #1
	str r4, [sp, #68]
	str r5, [sp, #64]
	str r6, [sp, #60]
	b .L_080ea214
.L_080ea1e4:
	.4byte 0x00007741
.L_080ea1e8:
	.4byte 0x00000080
.L_080ea1ec:
	.4byte 0x00001010
.L_080ea1f0:
	.4byte 0x00003f44
.L_080ea1f4:
	.4byte BattlePresentation_ProcessPendingGraphicsTransfer
.L_080ea1f8:
	.4byte gProjection
.L_080ea1fc:
	.4byte 0x0000003b
.L_080ea200:
	.4byte gBgScroll
.L_080ea204:
	.4byte 0x04000052
.L_080ea208:
	.4byte 0xffc00000
.L_080ea20c:
	.4byte 0x0000ffff
.L_080ea210:
	.4byte 0x000000bb
.L_080ea214:
	bl Resource_LoadAndDecompress
	movs r3, #192
	ldr r2, [sp, #100]
	lsls r3, r3, #3
	adds r1, r2, r3
	ldr r0, .L_080ea36c
	movs r2, #0
	movs r3, #0
	bl Resource_LoadAndDecompress
	ldr r5, .L_080ea370
	ldr r4, [sp, #100]
	ldr r0, .L_080ea374
	adds r1, r4, r5
	movs r2, #1
	movs r3, #0
	bl Resource_LoadAndDecompress
	ldr r6, .L_080ea378
	ldr r0, .L_080ea37c
	ldr r1, [sp, #80]
	movs r2, #0
	movs r3, #0
	bl Resource_LoadAndDecompress
	movs r7, #0
	movs r5, #0
	movs r4, #64
.L_080ea24e:
	ldr r2, [sp, #100]
	adds r3, r5, r2
	ldr r2, .L_080ea380
	ldr r1, [sp, #80]
	movs r0, #0
	mov r12, r4
	adds r3, r3, r2
.L_080ea25c:
	ldrb r2, [r1]
	adds r1, #1
	cmp r2, r12
	ble .L_080ea266
	mov r2, r12
.L_080ea266:
	cmp r2, #0
	bge .L_080ea26c
	movs r2, #0
.L_080ea26c:
	adds r0, #1
	strb r2, [r3]
	adds r3, #1
	cmp r0, r6
	bne .L_080ea25c
	adds r7, #1
	adds r5, r5, r6
	subs r4, #7
	cmp r7, #8
	bne .L_080ea24e
	ldr r0, .L_080ea384
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_080ea388
	lsls r0, r0, #19
	movs r2, #128
	bl _call_via_r3
	ldr r6, .L_080ea38c
	ldr r5, .L_080ea390
	movs r7, #0
.L_080ea29a:
	bl Random16
	movs r3, #15
	ands r3, r0
	adds r3, #72
	str r3, [r5]
	bl Random16
	ands r0, r6
	str r0, [r5, #12]
	bl Random16
	ands r0, r6
	str r0, [r5, #16]
	bl Random16
	adds r7, #1
	ands r0, r6
	str r0, [r5, #16]
	adds r5, #28
	cmp r7, #128
	bne .L_080ea29a
	movs r3, #255
	movs r4, #0
	ldr r6, .L_080ea394
	movs r7, #0
	mov r10, r3
	mov r8, r4
.L_080ea2d2:
	bl Random16
	ldr r3, .L_080ea38c
	adds r5, r0, #0
	ands r5, r3
	adds r0, r5, #0
	bl Trig_Sin
	mov r3, r10
	muls r3, r0
	asrs r3, r3, #2
	str r3, [r6]
	adds r0, r5, #0
	bl Trig_Cos
	mov r3, r10
	muls r3, r0
	asrs r3, r3, #2
	str r3, [r6, #8]
	bl Random16
	movs r3, #63
	ands r3, r0
	adds r3, #32
	mov r0, r8
	lsls r3, r3, #16
	adds r7, #1
	str r3, [r6, #4]
	str r0, [r6, #12]
	str r0, [r6, #16]
	str r5, [r6, #24]
	adds r6, #28
	cmp r7, #128
	bne .L_080ea2d2
	ldr r1, [sp, #100]
	movs r3, #239
	lsls r3, r3, #7
	ldr r4, .L_080ea398
	adds r2, r1, r3
	movs r3, #2
	str r3, [r2]
	adds r2, r1, r4
	movs r3, #50
	str r3, [r2]
	ldr r2, .L_080ea39c
	ldr r3, .L_080ea368
	mov r6, sp
	strh r3, [r2]
	mov r0, sp
	mov r1, sp
	ldr r2, .L_080ea3a0
	ldr r3, .L_080ea3a4
	movs r5, #0
	adds r6, #172
	adds r0, #144
	adds r1, #116
	str r5, [sp, #84]
	str r6, [sp, #16]
	str r0, [sp, #20]
	str r1, [sp, #24]
	str r2, [sp, #56]
	str r3, [sp, #12]
.L_080ea34e:
	ldr r4, [sp, #84]
	cmp r4, #143
	bne .L_080ea3b4
	movs r1, #128
	ldr r3, .L_080ea3a8
	ldr r0, [sp, #104]
	lsls r1, r1, #7
	ldr r2, .L_080ea3ac
	bl _call_via_r3
	movs r0, #145
	b .L_080ea3b0
	.2byte 0x0000
.L_080ea368:
	.4byte 0x00000784
.L_080ea36c:
	.4byte 0x00000067
.L_080ea370:
	.4byte 0x0000095c
.L_080ea374:
	.4byte 0x000000ce
.L_080ea378:
	.4byte 0x00000302
.L_080ea37c:
	.4byte 0x00000073
.L_080ea380:
	.4byte 0x00002710
.L_080ea384:
	.4byte 0x00000064
.L_080ea388:
	.4byte IwramCopyWords
.L_080ea38c:
	.4byte 0x0000ffff
.L_080ea390:
	.4byte gMapCellBuffer
.L_080ea394:
	.4byte gMapCellBuffer + 0xe00
.L_080ea398:
	.4byte 0x00007784
.L_080ea39c:
	.4byte 0x0400000c
.L_080ea3a0:
	.4byte gWorkSlot
.L_080ea3a4:
	.4byte 0xfffffa70
.L_080ea3a8:
	.4byte IwramFillWords
.L_080ea3ac:
	.4byte 0x2a2a2a2a
.L_080ea3b0:
	bl AudioCommand_PlayFar
.L_080ea3b4:
	ldr r5, [sp, #84]
	cmp r5, #80
	bne .L_080ea3c0
	movs r0, #142
	bl AudioCommand_PlayFar
.L_080ea3c0:
	ldr r6, [sp, #84]
	cmp r6, #0
	bge .L_080ea496
	bl Render_ResetTransformState
	ldr r0, [sp, #76]
	adds r1, r0, #0
	adds r1, #12
	bl Graphics_PrepareTransferInIwramWork
	add r0, sp, #160
	mov r8, r0
	ldr r6, .L_080ea758
	movs r7, #0
	mov r10, r8
.L_080ea3de:
	adds r0, r6, #0
	movs r1, #60
	ldr r2, .L_080ea75c
	bl EffectStep_AdvanceWithGravity3D
	mov r1, r10
	adds r0, r6, #0
	bl EffectPosition_ApplyBaseAndYOffset
	mov r1, r10
	ldr r3, [r1]
	asrs r3, r3, #1
	str r3, [r1]
	ldr r3, [r1, #4]
	ldr r0, [r1, #8]
	subs r3, #120
	str r3, [r1, #4]
	cmp r0, #99
	bgt .L_080ea40c
	movs r3, #100
	mov r2, r8
	str r3, [r2, #8]
	movs r0, #100
.L_080ea40c:
	movs r3, #225
	lsls r3, r3, #2
	cmp r0, r3
	ble .L_080ea41a
	mov r4, r8
	str r3, [r4, #8]
	adds r0, r3, #0
.L_080ea41a:
	adds r5, r0, #0
	subs r5, #100
	mov r0, r8
	str r5, [r0, #8]
	movs r1, #100
	adds r0, r5, #0
	bl __divsi3
	ldr r1, [sp, #64]
	adds r4, r0, #1
	cmp r1, r5
	ble .L_080ea434
	str r5, [sp, #64]
.L_080ea434:
	ldr r2, [sp, #60]
	cmp r2, r5
	bge .L_080ea43c
	str r5, [sp, #60]
.L_080ea43c:
	lsls r0, r4, #1
	ldr r2, .L_080ea760
	subs r3, r0, #2
	ldrh r1, [r2, r3]
	ldr r3, [sp, #80]
	mov r5, r8
	adds r1, r3, r1
	lsrs r3, r4, #31
	ldr r2, [r5]
	adds r3, r4, r3
	asrs r3, r3, #1
	subs r2, r2, r3
	ldr r3, [r5, #4]
	str r4, [sp, #0]
	subs r3, r3, r4
	str r0, [sp, #4]
	ldr r4, [sp, #88]
	ldr r0, [sp, #104]
	bl _call_via_r4
	ldr r5, [sp, #84]
	cmp r5, #64
	bne .L_080ea48e
	ldr r0, [r6, #24]
	bl Trig_Sin
	lsls r2, r0, #8
	ldr r3, [r6, #12]
	subs r2, r2, r0
	asrs r2, r2, #3
	adds r3, r3, r2
	str r3, [r6, #12]
	ldr r0, [r6, #24]
	bl Trig_Cos
	lsls r2, r0, #8
	ldr r3, [r6, #16]
	subs r2, r2, r0
	asrs r2, r2, #3
	adds r3, r3, r2
	str r3, [r6, #16]
.L_080ea48e:
	adds r7, #1
	adds r6, #28
	cmp r7, #128
	bne .L_080ea3de
.L_080ea496:
	ldr r6, [sp, #84]
	cmp r6, #72
	bne .L_080ea4ae
	ldr r0, [sp, #100]
	ldr r1, .L_080ea764
	movs r2, #24
	adds r3, r0, r1
	str r2, [r3]
	ldr r3, .L_080ea768
	adds r2, r0, r3
	movs r3, #0
	str r3, [r2]
.L_080ea4ae:
	ldr r4, [sp, #16]
	movs r3, #0
	str r3, [r4, #8]
	str r3, [r4, #4]
	ldr r6, .L_080ea76c
	movs r7, #0
.L_080ea4ba:
	adds r3, r7, #0
	cmp r7, #0
	bge .L_080ea4c2
	adds r3, r7, #3
.L_080ea4c2:
	asrs r3, r3, #2
	ldr r5, [sp, #84]
	adds r3, #80
	cmp r5, r3
	ble .L_080ea55c
	ldr r3, [r6]
	cmp r3, #0
	ble .L_080ea55c
	bl Render_ResetTransformState
	ldr r0, [r6, #16]
	bl SceneTransform_ApplyRoll
	ldr r0, [r6, #12]
	bl SceneTransform_ApplyPitch
	ldr r0, [r6, #16]
	lsls r3, r5, #9
	adds r0, r0, r3
	bl SceneTransform_ApplyYaw
	ldr r3, [r6]
	ldr r0, [sp, #16]
	str r3, [r0]
	ldr r3, [r6]
	subs r3, #2
	str r3, [r6]
	cmp r3, #0
	bge .L_080ea500
	movs r3, #0
	str r3, [r6]
.L_080ea500:
	add r5, sp, #160
	ldr r0, [sp, #16]
	adds r1, r5, #0
	bl EffectPosition_ApplyBaseAndYOffset
	movs r3, #60
	ldr r0, [r5, #8]
	negs r3, r3
	cmp r0, r3
	bge .L_080ea518
	str r3, [r5, #8]
	adds r0, r3, #0
.L_080ea518:
	cmp r0, #60
	ble .L_080ea522
	movs r3, #60
	str r3, [r5, #8]
	movs r0, #60
.L_080ea522:
	adds r0, #60
	str r0, [r5, #8]
	movs r1, #20
	bl __divsi3
	ldr r2, [r5]
	ldr r3, [r5, #4]
	adds r0, #2
	adds r2, #60
	adds r3, #80
	str r2, [r5]
	str r3, [r5, #4]
	ldr r4, .L_080ea760
	lsls r5, r0, #1
	subs r1, r5, #2
	ldrh r1, [r4, r1]
	ldr r4, [sp, #80]
	adds r1, r4, r1
	lsrs r4, r0, #31
	adds r4, r0, r4
	asrs r4, r4, #1
	subs r3, r3, r0
	str r0, [sp, #0]
	str r5, [sp, #4]
	subs r2, r2, r4
	ldr r0, [sp, #104]
	ldr r5, [sp, #88]
	bl _call_via_r5
.L_080ea55c:
	adds r7, #1
	adds r6, #28
	cmp r7, #64
	bne .L_080ea4ba
	ldr r3, .L_080ea770
	ldr r4, [r3, #4]
	ldr r3, [r3]
	ldr r6, [sp, #20]
	str r3, [sp, #116]
	str r4, [sp, #120]
	movs r3, #0
	str r3, [r6, #12]
	str r3, [r6, #4]
	ldr r0, [sp, #100]
	ldr r1, .L_080ea774
	movs r7, #0
	adds r5, r0, r1
.L_080ea57e:
	adds r3, r7, #0
	cmp r7, #0
	bge .L_080ea586
	adds r3, r7, #3
.L_080ea586:
	asrs r3, r3, #2
	lsls r2, r3, #2
	subs r2, r7, r2
	movs r4, #152
	ldr r6, [sp, #20]
	lsls r4, r4, #15
	lsls r2, r2, #21
	adds r2, r2, r4
	str r2, [r6]
	ldr r0, [sp, #72]
	lsls r3, r3, #21
	adds r3, r3, r0
	str r3, [r6, #8]
	ldmia r5!, {r0}
	ldr r1, [sp, #20]
	ldr r2, [sp, #24]
	movs r3, #0
	adds r7, #1
	bl Object_ApplyProjectedPlacementFar
	cmp r7, #16
	bne .L_080ea57e
	ldr r1, [sp, #72]
	ldr r2, [sp, #68]
	ldr r3, [sp, #84]
	adds r1, r1, r2
	str r1, [sp, #72]
	cmp r3, #47
	bgt .L_080ea5c6
	ldr r4, .L_080ea778
	adds r2, r2, r4
	str r2, [sp, #68]
.L_080ea5c6:
	ldr r5, [sp, #84]
	cmp r5, #32
	ble .L_080ea5de
	ldr r6, [sp, #68]
	lsls r3, r6, #4
	subs r3, r3, r6
	lsls r3, r3, #2
	cmp r3, #0
	bge .L_080ea5da
	adds r3, #63
.L_080ea5da:
	asrs r3, r3, #6
	str r3, [sp, #68]
.L_080ea5de:
	ldr r0, [sp, #84]
	cmp r0, #144
	bne .L_080ea5e8
	ldr r1, .L_080ea77c
	str r1, [sp, #68]
.L_080ea5e8:
	ldr r2, [sp, #84]
	cmp r2, #146
	bne .L_080ea5f4
	movs r3, #128
	lsls r3, r3, #9
	str r3, [sp, #68]
.L_080ea5f4:
	ldr r4, [sp, #84]
	cmp r4, #72
	bne .L_080ea612
	ldr r6, [sp, #100]
	ldr r0, .L_080ea774
	movs r7, #0
	adds r5, r6, r0
.L_080ea602:
	adds r1, r7, #0
	ldmia r5!, {r0}
	adds r1, #16
	adds r7, #1
	bl Object_InitializeMode
	cmp r7, #16
	bne .L_080ea602
.L_080ea612:
	ldr r1, [sp, #84]
	cmp r1, #76
	bne .L_080ea630
	ldr r2, [sp, #100]
	ldr r3, .L_080ea774
	movs r7, #0
	adds r5, r2, r3
.L_080ea620:
	adds r1, r7, #0
	ldmia r5!, {r0}
	adds r1, #32
	adds r7, #1
	bl Object_InitializeMode
	cmp r7, #16
	bne .L_080ea620
.L_080ea630:
	ldr r3, [sp, #84]
	subs r3, #116
	cmp r3, #27
	bls .L_080ea63a
	b .L_080ea7ce
.L_080ea63a:
	cmp r3, #0
	bge .L_080ea642
	ldr r3, [sp, #84]
	subs r3, #113
.L_080ea642:
	asrs r1, r3, #2
	cmp r1, #6
	ble .L_080ea64a
	movs r1, #6
.L_080ea64a:
	ldr r3, .L_080ea780
	ldrb r3, [r3, r1]
	ldr r2, .L_080ea784
	mov r10, r3
	lsls r3, r1, #1
	ldrh r6, [r2, r3]
	movs r4, #192
	lsls r4, r4, #3
	movs r5, #2
	movs r1, #7
	movs r2, #7
	movs r3, #3
	movs r0, #47
	str r5, [sp, #0]
	adds r6, r6, r4
	bl Unnamed_080ed408
	movs r1, #60
	mov r9, r1
	ldr r0, [sp, #100]
	mov r2, r9
	mov r3, r10
	subs r2, r2, r3
	mov r4, r10
	lsls r3, r3, #1
	str r3, [sp, #4]
	str r4, [sp, #0]
	adds r6, r0, r6
	movs r5, #80
	ldr r0, .L_080ea788
	subs r5, r5, r3
	mov r8, r3
	ldr r4, [r0]
	adds r3, r5, #0
	ldr r0, [sp, #104]
	adds r1, r6, #0
	mov r9, r2
	bl _call_via_r4
	movs r0, #47
	bl Runtime_ReleaseHeapBlock
	movs r1, #2
	str r1, [sp, #0]
	movs r2, #7
	movs r1, #7
	movs r3, #7
	movs r0, #47
	bl Unnamed_080ed408
	mov r3, r8
	mov r2, r10
	str r2, [sp, #0]
	str r3, [sp, #4]
	ldr r0, .L_080ea788
	adds r3, r5, #0
	ldr r4, [r0]
	adds r1, r6, #0
	ldr r0, [sp, #104]
	movs r2, #60
	bl _call_via_r4
	movs r0, #47
	bl Runtime_ReleaseHeapBlock
	movs r1, #2
	str r1, [sp, #0]
	movs r2, #7
	movs r1, #7
	movs r3, #11
	movs r0, #47
	bl Unnamed_080ed408
	mov r3, r8
	mov r2, r10
	str r2, [sp, #0]
	str r3, [sp, #4]
	ldr r5, .L_080ea788
	ldr r0, [sp, #104]
	ldr r4, [r5]
	adds r1, r6, #0
	mov r2, r9
	movs r3, #80
	bl _call_via_r4
	movs r0, #47
	bl Runtime_ReleaseHeapBlock
	movs r0, #2
	str r0, [sp, #0]
	movs r1, #7
	movs r2, #7
	movs r3, #15
	movs r0, #47
	bl Unnamed_080ed408
	mov r2, r8
	mov r1, r10
	str r1, [sp, #0]
	str r2, [sp, #4]
	movs r3, #80
	ldr r4, [r5]
	ldr r0, [sp, #104]
	adds r1, r6, #0
	movs r2, #60
	bl _call_via_r4
	movs r0, #47
	bl Runtime_ReleaseHeapBlock
	ldr r3, .L_080ea78c
	movs r7, #0
	mov r8, r3
.L_080ea72c:
	adds r4, r7, #0
	movs r5, #3
	ands r4, r5
	str r4, [sp, #8]
	bl Random16
	ldr r3, .L_080ea790
	adds r6, r0, #0
	ands r6, r3
	adds r0, r6, #0
	bl Trig_Sin
	ldr r4, [sp, #8]
	adds r5, r0, #0
	mov r0, r8
	ldrb r3, [r0, r4]
	lsls r5, r5, #4
	lsrs r3, r3, #1
	adds r0, r6, #0
	asrs r5, r5, #16
	subs r5, r5, r3
	b .L_080ea794
.L_080ea758:
	.4byte gMapCellBuffer + 0xe00
.L_080ea75c:
	.4byte 0xfffffc00
.L_080ea760:
	.4byte ParticleStreams_CellOffsets
.L_080ea764:
	.4byte 0x000077b4
.L_080ea768:
	.4byte 0x000077b8
.L_080ea76c:
	.4byte gMapCellBuffer
.L_080ea770:
	.4byte Data_080edac8 + 0x8
.L_080ea774:
	.4byte 0x000077d8
.L_080ea778:
	.4byte 0xffffc000
.L_080ea77c:
	.4byte 0xfff80000
.L_080ea780:
	.4byte Data_080eef18 + 0x10
.L_080ea784:
	.4byte Data_080eef18 + 0x18
.L_080ea788:
	.4byte gTransitionWork + 0xc
.L_080ea78c:
	.4byte Data_080eef18 + 0x32
.L_080ea790:
	.4byte 0x0000ffff
.L_080ea794:
	bl Trig_Cos
	ldr r2, .L_080ea9a8
	ldr r4, [sp, #8]
	adds r3, r0, #0
	ldrb r0, [r2, r4]
	lsls r3, r3, #4
	lsrs r2, r0, #1
	ldr r1, .L_080ea9ac
	asrs r3, r3, #16
	subs r3, r3, r2
	lsls r2, r4, #1
	ldrh r1, [r1, r2]
	ldr r2, [sp, #100]
	mov r6, r8
	adds r1, r2, r1
	ldrb r2, [r6, r4]
	adds r5, #60
	str r2, [sp, #0]
	str r0, [sp, #4]
	adds r3, #80
	ldr r0, [sp, #104]
	adds r2, r5, #0
	ldr r4, [sp, #88]
	adds r7, #1
	bl _call_via_r4
	cmp r7, #6
	bne .L_080ea72c
.L_080ea7ce:
	ldr r5, [sp, #84]
	cmp r5, #143
	ble .L_080ea8c2
	ldr r0, .L_080ea9b0
	lsls r6, r5, #4
	movs r1, #3
	str r1, [sp, #0]
	adds r6, r6, r0
	movs r1, #7
	movs r2, #7
	movs r3, #3
	movs r0, #47
	bl Unnamed_080ed408
	ldr r7, [sp, #56]
	movs r5, #64
	movs r2, #24
	str r2, [sp, #0]
	str r5, [sp, #4]
	adds r7, #188
	ldr r1, [sp, #100]
	ldr r0, [sp, #104]
	ldr r4, [r7]
	adds r3, r6, #0
	movs r2, #36
	bl _call_via_r4
	movs r0, #47
	bl Runtime_ReleaseHeapBlock
	movs r3, #3
	str r3, [sp, #0]
	movs r1, #7
	movs r2, #7
	movs r3, #7
	movs r0, #47
	bl Unnamed_080ed408
	movs r4, #24
	str r5, [sp, #4]
	str r4, [sp, #0]
	adds r3, r6, #0
	ldr r1, [sp, #100]
	movs r2, #60
	ldr r4, [r7]
	ldr r0, [sp, #104]
	bl _call_via_r4
	movs r0, #47
	bl Runtime_ReleaseHeapBlock
	ldr r3, [sp, #84]
	subs r3, #144
	lsrs r2, r3, #31
	ldr r6, [sp, #100]
	ldr r0, .L_080ea9b4
	adds r3, r3, r2
	ldr r5, [sp, #12]
	asrs r1, r3, #1
	adds r2, r6, r0
	movs r3, #75
	mov r8, r5
	str r3, [r2]
	cmp r1, #6
	bgt .L_080ea8c2
	lsls r5, r1, #1
	adds r5, r5, r1
	ldr r3, .L_080ea9b8
	lsls r5, r5, #7
	ldrh r3, [r3, #14]
	adds r5, r5, r1
	lsls r5, r5, #1
	ldr r1, .L_080ea9bc
	adds r5, r5, r3
	movs r2, #2
	adds r5, r5, r1
	str r2, [sp, #0]
	movs r3, #3
	movs r0, #47
	movs r1, #7
	movs r2, #7
	bl Unnamed_080ed408
	ldr r7, [r7]
	adds r6, r6, r5
	movs r3, #4
	mov r9, r7
	mov r10, r6
	movs r7, #0
	mov r11, r3
.L_080ea882:
	lsls r6, r7, #9
	adds r0, r6, #0
	bl Trig_Sin
	mov r5, r8
	muls r5, r0
	mov r4, r11
	adds r0, r6, #0
	asrs r5, r5, #16
	subs r5, r5, r4
	bl Trig_Cos
	mov r3, r8
	muls r3, r0
	adds r5, #60
	movs r0, #16
	asrs r3, r3, #17
	movs r6, #8
	str r0, [sp, #4]
	adds r3, #72
	str r6, [sp, #0]
	ldr r0, [sp, #104]
	mov r1, r10
	adds r2, r5, #0
	adds r7, #1
	bl _call_via_r9
	cmp r7, #128
	bne .L_080ea882
	movs r0, #47
	bl Runtime_ReleaseHeapBlock
.L_080ea8c2:
	ldr r1, [sp, #100]
	ldr r3, .L_080ea9c0
	adds r2, r1, r3
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r4, [sp, #12]
	ldr r5, [sp, #84]
	adds r4, #10
	adds r5, #1
	str r4, [sp, #12]
	str r5, [sp, #84]
	cmp r5, #160
	beq .L_080ea8f6
	cmp r5, #4
	bgt .L_080ea8e8
	b .L_080ea34e
.L_080ea8e8:
	ldr r3, .L_080ea9c4
	ldr r3, [r3]
	movs r6, #3
	ands r3, r6
	cmp r3, #0
	bne .L_080ea8f6
	b .L_080ea34e
.L_080ea8f6:
	movs r1, #128
	lsls r1, r1, #7
	ldr r3, .L_080ea9c8
	ldr r0, [sp, #104]
	movs r2, #0
	bl _call_via_r3
	ldr r1, .L_080ea9cc
	ldr r0, [sp, #100]
	movs r7, #0
	adds r5, r0, r1
.L_080ea90c:
	ldmia r5!, {r0}
	adds r7, #1
	bl ResourceObject_ReleaseFar
	cmp r7, #16
	bne .L_080ea90c
	movs r2, #13
	negs r2, r2
	ldr r6, .L_080ea9cc
	movs r7, #0
	mov r8, r2
.L_080ea922:
	movs r0, #195
	lsls r0, r0, #1
	bl GetBattleEffectObject
	ldr r3, [sp, #100]
	adds r5, r0, #0
	str r5, [r6, r3]
	cmp r5, #0
	beq .L_080ea958
	adds r2, r5, #0
	adds r2, #38
	movs r3, #0
	strb r3, [r2]
	movs r1, #3
	adds r0, r7, #0
	bl __modsi3
	adds r1, r0, #0
	adds r0, r5, #0
	bl Object_InitializeMode
	ldr r4, [sp, #100]
	ldr r2, [r6, r4]
	ldrb r3, [r2, #9]
	mov r5, r8
	ands r3, r5
	strb r3, [r2, #9]
.L_080ea958:
	adds r7, #1
	adds r6, #4
	cmp r7, #16
	bne .L_080ea922
	movs r0, #46
	bl Runtime_ReleaseHeapBlock
	movs r3, #2
	str r3, [sp, #0]
	movs r1, #7
	movs r2, #7
	movs r3, #3
	movs r0, #46
	bl Unnamed_080ed408
	ldr r3, .L_080ea9d0
	adds r3, #184
	ldr r3, [r3]
	ldr r6, [sp, #100]
	movs r2, #128
	lsls r2, r2, #7
	adds r1, r6, r2
	str r3, [sp, #88]
	ldr r0, .L_080ea9d4
	movs r2, #1
	movs r3, #1
	bl Resource_LoadAndDecompress
	ldr r0, .L_080ea9d8
	ldr r1, [sp, #80]
	movs r7, #0
	movs r4, #1
.L_080ea998:
	ldrb r3, [r1]
	adds r2, r3, #0
	cmp r2, #32
	bls .L_080ea9dc
	adds r3, #224
	strb r3, [r1]
	b .L_080ea9e2
	.2byte 0x0000
.L_080ea9a8:
	.4byte Data_080eef18 + 0x38
.L_080ea9ac:
	.4byte Data_080eef18 + 0x26
.L_080ea9b0:
	.4byte 0xfffff720
.L_080ea9b4:
	.4byte 0x00007784
.L_080ea9b8:
	.4byte ParticleStreams_CellOffsets
.L_080ea9bc:
	.4byte 0x00002710
.L_080ea9c0:
	.4byte 0x00007824
.L_080ea9c4:
	.4byte gKeysRepeat
.L_080ea9c8:
	.4byte IwramFillWords
.L_080ea9cc:
	.4byte 0x000077d8
.L_080ea9d0:
	.4byte gWorkSlot
.L_080ea9d4:
	.4byte 0x00000064
.L_080ea9d8:
	.4byte 0x00000302
.L_080ea9dc:
	cmp r2, #0
	beq .L_080ea9e2
	strb r4, [r1]
.L_080ea9e2:
	adds r7, #1
	adds r1, #1
	cmp r7, r0
	bne .L_080ea998
	ldr r4, [sp, #100]
	ldr r5, .L_080eaa48
	ldr r6, .L_080eaa4c
	adds r3, r4, r5
	movs r5, #0
	str r5, [r3]
	adds r3, r4, r6
	str r5, [r3]
	ldr r1, .L_080eaa50
	movs r0, #1
	movs r2, #0
	bl BattleBackground_LoadFar
	ldr r2, .L_080eaa54
	ldr r3, .L_080eaa40
	strh r3, [r2]
	ldr r3, .L_080eaa58
	adds r2, #8
	str r3, [r2]
	ldr r3, .L_080eaa44
	add r4, sp, #140
	subs r2, #28
	strh r3, [r2]
	str r5, [r4]
	ldr r3, .L_080eaa5c
	adds r0, r4, #0
	ldr r1, [sp, #104]
	ldr r2, .L_080eaa60
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r0, #60
	ldr r6, [sp, #100]
	str r0, [sp, #52]
	movs r1, #44
	movs r0, #239
	str r1, [sp, #48]
	movs r3, #0
	lsls r0, r0, #7
	ldr r1, .L_080eaa64
	str r3, [sp, #32]
	str r3, [sp, #40]
	b .L_080eaa68
	.2byte 0x0000
.L_080eaa40:
	.4byte 0x00000100
.L_080eaa44:
	.4byte 0x00000784
.L_080eaa48:
	.4byte 0x000077b4
.L_080eaa4c:
	.4byte 0x000077b8
.L_080eaa50:
	.4byte 0x0000003e
.L_080eaa54:
	.4byte 0x04000020
.L_080eaa58:
	.4byte 0xffffc400
.L_080eaa5c:
	.4byte 0x040000d4
.L_080eaa60:
	.4byte 0x85001000
.L_080eaa64:
	.4byte 0x00007784
.L_080eaa68:
	movs r5, #2
	adds r3, r6, r0
	movs r2, #6
	str r2, [sp, #44]
	str r5, [sp, #36]
	adds r2, r6, r1
	str r5, [r3]
	movs r3, #75
	str r3, [r2]
	ldr r2, [sp, #32]
	str r2, [r4]
	ldr r3, .L_080eab7c
	adds r0, r4, #0
	ldr r1, [sp, #100]
	ldr r2, .L_080eab80
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r3, #0
	str r3, [sp, #84]
.L_080eaa8e:
	ldr r4, [sp, #84]
	cmp r4, #66
	bne .L_080eaaa0
	movs r0, #145
	bl AudioCommand_PlayFar
	movs r0, #141
	bl AudioCommand_PlayFar
.L_080eaaa0:
	ldr r5, [sp, #84]
	cmp r5, #155
	bne .L_080eaaac
	movs r0, #162
	bl AudioCommand_PlayFar
.L_080eaaac:
	ldr r6, [sp, #84]
	cmp r6, #217
	bne .L_080eaab8
	movs r0, #156
	bl AudioCommand_PlayFar
.L_080eaab8:
	movs r1, #140
	ldr r0, [sp, #84]
	lsls r1, r1, #1
	cmp r0, r1
	bne .L_080eaac8
	movs r0, #157
	bl AudioCommand_PlayFar
.L_080eaac8:
	movs r3, #150
	ldr r2, [sp, #84]
	lsls r3, r3, #1
	cmp r2, r3
	bne .L_080eaad8
	movs r0, #145
	bl BattleEventRuntime_BeginPhaseFar
.L_080eaad8:
	ldr r3, .L_080eab84
	ldr r3, [r3]
	movs r2, #3
	ands r3, r2
	cmp r3, #0
	beq .L_080eabaa
	ldr r3, [sp, #84]
	subs r3, #5
	cmp r3, #144
	bhi .L_080eab6c
	ldr r1, .L_080eab88
	movs r4, #150
	str r4, [sp, #84]
	ldr r5, .L_080eab8c
	ldrh r3, [r5]
	adds r0, r3, #0
	movs r6, #130
	ldr r2, .L_080eab8c
	lsls r6, r6, #2
	strh r6, [r2]
	ldrh r2, [r1]
	cmp r2, #31
	bgt .L_080eab22
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	adds r2, #1
	adds r3, r3, r1
	adds r3, #4
	strh r2, [r1]
	movs r2, #128
	stmia r3!, {r2}
	ldr r2, .L_080eab90
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #10
	str r2, [r3]
.L_080eab22:
	ldr r3, .L_080eab8c
	strh r0, [r3]
	ldrh r3, [r3]
	adds r0, r3, #0
	movs r4, #130
	ldr r5, .L_080eab8c
	lsls r4, r4, #2
	strh r4, [r5]
	ldrh r2, [r1]
	cmp r2, #31
	bgt .L_080eab54
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	adds r3, r3, r1
	adds r3, #4
	adds r2, #1
	movs r6, #0
	stmia r3!, {r6}
	strh r2, [r1]
	ldr r2, .L_080eab94
	stmia r3!, {r2}
	movs r2, #192
	lsls r2, r2, #10
	str r2, [r3]
.L_080eab54:
	ldr r1, .L_080eab8c
	strh r0, [r1]
	ldr r0, .L_080eab98
	movs r3, #128
	ldr r2, [sp, #100]
	lsls r3, r3, #7
	adds r1, r2, r3
	movs r2, #1
	movs r3, #0
	bl Resource_LoadAndDecompress
	b .L_080eabaa
.L_080eab6c:
	ldr r3, [sp, #84]
	subs r3, #155
	cmp r3, #58
	bhi .L_080eab9c
	movs r4, #214
	str r4, [sp, #84]
	b .L_080eabaa
	.2byte 0x0000
.L_080eab7c:
	.4byte 0x040000d4
.L_080eab80:
	.4byte 0x85000e10
.L_080eab84:
	.4byte gKeysRepeat
.L_080eab88:
	.4byte gIoWriteQueue
.L_080eab8c:
	.4byte 0x04000208
.L_080eab90:
	.4byte 0x04000020
.L_080eab94:
	.4byte 0x04000028
.L_080eab98:
	.4byte 0x00000070
.L_080eab9c:
	ldr r3, [sp, #84]
	subs r3, #219
	cmp r3, #60
	bhi .L_080eabaa
	movs r5, #140
	lsls r5, r5, #1
	str r5, [sp, #84]
.L_080eabaa:
	ldr r6, [sp, #84]
	cmp r6, #64
	bne .L_080eac4c
	ldr r1, .L_080eac84
	ldr r0, .L_080eac88
	ldrh r3, [r0]
	adds r0, r3, #0
	movs r2, #130
	ldr r3, .L_080eac88
	lsls r2, r2, #2
	strh r2, [r3]
	ldrh r2, [r1]
	cmp r2, #31
	bgt .L_080eabe2
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	adds r2, #1
	adds r3, r3, r1
	adds r3, #4
	strh r2, [r1]
	movs r2, #128
	stmia r3!, {r2}
	ldr r2, .L_080eac8c
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #10
	str r2, [r3]
.L_080eabe2:
	ldr r4, .L_080eac88
	strh r0, [r4]
	ldrh r3, [r4]
	adds r0, r3, #0
	movs r5, #130
	ldr r6, .L_080eac88
	lsls r5, r5, #2
	strh r5, [r6]
	ldrh r2, [r1]
	cmp r2, #31
	bgt .L_080eac14
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	adds r3, r3, r1
	adds r2, #1
	adds r3, #4
	strh r2, [r1]
	movs r1, #0
	stmia r3!, {r1}
	ldr r2, .L_080eac90
	stmia r3!, {r2}
	movs r2, #192
	lsls r2, r2, #10
	str r2, [r3]
.L_080eac14:
	ldr r2, .L_080eac88
	strh r0, [r2]
	movs r3, #0
	add r4, sp, #140
	str r3, [sp, #140]
	adds r0, r4, #0
	ldr r3, .L_080eac94
	ldr r1, [sp, #104]
	ldr r2, .L_080eac98
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r5, [sp, #100]
	movs r6, #128
	lsls r6, r6, #7
	ldr r0, .L_080eac9c
	adds r1, r5, r6
	movs r2, #1
	movs r3, #0
	bl Resource_LoadAndDecompress
	movs r2, #192
	lsls r2, r2, #7
	adds r1, r5, r2
	ldr r0, .L_080eaca0
	movs r2, #0
	movs r3, #0
	bl Resource_LoadAndDecompress
.L_080eac4c:
	ldr r3, [sp, #84]
	cmp r3, #66
	bne .L_080eac66
	ldr r2, .L_080eaca4
	ldr r1, .L_080eac80
	movs r7, #0
.L_080eac58:
	ldrh r3, [r2]
	adds r7, #1
	eors r3, r1
	strh r3, [r2]
	adds r2, #2
	cmp r7, #128
	bne .L_080eac58
.L_080eac66:
	ldr r4, [sp, #84]
	cmp r4, #69
	bne .L_080eacac
	ldr r3, .L_080eaca8
	add r5, sp, #140
	str r3, [sp, #140]
	adds r0, r5, #0
	ldr r3, .L_080eac94
	ldr r1, [sp, #104]
	ldr r2, .L_080eac98
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	b .L_080eacac
.L_080eac80:
	.4byte 0x00007fff
.L_080eac84:
	.4byte gIoWriteQueue
.L_080eac88:
	.4byte 0x04000208
.L_080eac8c:
	.4byte 0x04000020
.L_080eac90:
	.4byte 0x04000028
.L_080eac94:
	.4byte 0x040000d4
.L_080eac98:
	.4byte 0x85001000
.L_080eac9c:
	.4byte 0x00000070
.L_080eaca0:
	.4byte 0x00000065
.L_080eaca4:
	.4byte 0x050000c0
.L_080eaca8:
	.4byte 0x3f3f3f3f
.L_080eacac:
	ldr r6, [sp, #84]
	cmp r6, #70
	bne .L_080eacbc
	movs r0, #1
	ldr r1, .L_080eadd0
	movs r2, #7
	bl BattlePresentation_ConfigurePaletteFadeFar
.L_080eacbc:
	ldr r0, [sp, #84]
	cmp r0, #150
	bne .L_080ead02
	movs r3, #0
	movs r1, #112
	movs r2, #32
	movs r4, #4
	movs r5, #8
	add r6, sp, #140
	str r1, [sp, #52]
	str r2, [sp, #48]
	str r3, [sp, #40]
	str r3, [sp, #140]
	str r4, [sp, #36]
	str r5, [sp, #44]
	ldr r3, .L_080eadd4
	adds r0, r6, #0
	ldr r1, [sp, #100]
	ldr r2, .L_080eadd8
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r3, .L_080eaddc
	adds r1, r6, #0
	str r3, [sp, #140]
	adds r0, r1, #0
	ldr r3, .L_080eadd4
	ldr r1, [sp, #104]
	ldr r2, .L_080eade0
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r0, #1
	ldr r1, .L_080eade4
	movs r2, #0
	bl BattleBackground_LoadFar
.L_080ead02:
	ldr r2, [sp, #84]
	cmp r2, #214
	bne .L_080ead82
	movs r3, #0
	add r4, sp, #140
	str r3, [sp, #140]
	adds r0, r4, #0
	ldr r3, .L_080eadd4
	ldr r1, [sp, #100]
	ldr r2, .L_080eadd8
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r3, .L_080eaddc
	adds r5, r4, #0
	str r3, [sp, #140]
	adds r0, r5, #0
	ldr r3, .L_080eadd4
	ldr r1, [sp, #104]
	ldr r2, .L_080eade0
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r0, #1
	ldr r1, .L_080eade8
	movs r2, #0
	bl BattleBackground_LoadFar
	movs r0, #225
	ldr r6, [sp, #100]
	lsls r0, r0, #7
	movs r7, #0
	adds r5, r6, r0
.L_080ead40:
	bl Random16
	movs r3, #63
	ands r3, r0
	adds r3, #32
	lsls r3, r3, #16
	str r3, [r5]
	bl Random16
	movs r3, #31
	ands r3, r0
	adds r3, #96
	lsls r3, r3, #16
	str r3, [r5, #4]
	bl Random16
	movs r3, #15
	ands r3, r0
	adds r3, #16
	lsls r3, r3, #15
	ldr r2, [r5]
	str r3, [r5, #16]
	movs r3, #128
	asrs r2, r2, #7
	lsls r3, r3, #8
	subs r3, r3, r2
	str r3, [r5, #8]
	adds r7, #1
	movs r3, #1
	str r3, [r5, #24]
	adds r5, #28
	cmp r7, #16
	bne .L_080ead40
.L_080ead82:
	movs r2, #140
	ldr r1, [sp, #84]
	lsls r2, r2, #1
	cmp r1, r2
	bne .L_080eae44
	bl BattleEffect_SetupBlendedDisplay
	movs r3, #0
	add r4, sp, #140
	str r3, [sp, #140]
	adds r0, r4, #0
	ldr r3, .L_080eadd4
	ldr r1, [sp, #100]
	ldr r2, .L_080eadd8
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r3, .L_080eadec
	adds r5, r4, #0
	str r3, [sp, #140]
	adds r0, r5, #0
	ldr r3, .L_080eadd4
	ldr r1, [sp, #104]
	ldr r2, .L_080eade0
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r0, [sp, #96]
	ldr r2, .L_080eadf0
	ldr r3, .L_080eadcc
	movs r6, #0
	strh r3, [r2]
	str r6, [r0, #16]
	ldr r1, [sp, #100]
	movs r2, #225
	lsls r2, r2, #7
	movs r7, #0
	adds r5, r1, r2
	b .L_080eadf4
.L_080eadcc:
	.4byte 0x00001010
.L_080eadd0:
	.4byte 0x0000003e
.L_080eadd4:
	.4byte 0x040000d4
.L_080eadd8:
	.4byte 0x85000e10
.L_080eaddc:
	.4byte 0x3f3f3f3f
.L_080eade0:
	.4byte 0x85001000
.L_080eade4:
	.4byte 0x00000036
.L_080eade8:
	.4byte 0x0000003a
.L_080eadec:
	.4byte 0x01010101
.L_080eadf0:
	.4byte 0x04000052
.L_080eadf4:
	bl Random16
	movs r3, #63
	movs r4, #128
	lsls r4, r4, #1
	ands r3, r0
	adds r3, r3, r4
	lsls r3, r3, #16
	str r3, [r5]
	bl Random16
	movs r3, #31
	ands r3, r0
	adds r3, #96
	lsls r3, r3, #16
	str r3, [r5, #4]
	bl Random16
	movs r3, #15
	ands r3, r0
	adds r3, #16
	lsls r3, r3, #15
	movs r6, #0
	adds r7, #1
	str r3, [r5, #16]
	str r6, [r5, #8]
	adds r5, #28
	cmp r7, #16
	bne .L_080eadf4
	ldr r1, [sp, #100]
	movs r2, #239
	movs r0, #136
	lsls r2, r2, #7
	ldr r4, .L_080eae80
	adds r3, r1, r2
	lsls r0, r0, #1
	str r0, [sp, #32]
	str r6, [r3]
	adds r3, r1, r4
	str r6, [r3]
.L_080eae44:
	ldr r5, [sp, #84]
	ldr r6, .L_080eae84
	cmp r5, r6
	ble .L_080eae9e
	ldr r5, .L_080eae88
	ldr r6, .L_080eae7c
	movs r7, #0
.L_080eae52:
	ldrh r2, [r5]
	lsls r3, r2, #16
	lsrs r0, r3, #26
	ands r0, r6
	lsrs r1, r3, #21
	movs r4, #31
	ands r1, r6
	ands r4, r2
	adds r0, #1
	adds r1, #1
	adds r4, #1
	cmp r0, #31
	ble .L_080eae6e
	movs r0, #31
.L_080eae6e:
	cmp r1, #31
	ble .L_080eae74
	movs r1, #31
.L_080eae74:
	cmp r4, #31
	ble .L_080eae8c
	movs r4, #31
	b .L_080eae8c
.L_080eae7c:
	.4byte 0x0000001f
.L_080eae80:
	.4byte 0x00007784
.L_080eae84:
	.4byte 0x00000117
.L_080eae88:
	.4byte 0x05000002
.L_080eae8c:
	lsls r3, r0, #10
	lsls r2, r1, #5
	orrs r3, r2
	orrs r3, r4
	adds r7, #1
	strh r3, [r5]
	adds r5, #2
	cmp r7, #63
	bne .L_080eae52
.L_080eae9e:
	ldr r0, [sp, #84]
	cmp r0, #182
	bne .L_080eaeb6
	ldr r3, .L_080eb1bc
	add r1, sp, #140
	str r3, [sp, #140]
	adds r0, r1, #0
	ldr r3, .L_080eb1c0
	ldr r1, [sp, #104]
	ldr r2, .L_080eb1c4
	stmia r3!, {r0, r1, r2}
	subs r3, #12
.L_080eaeb6:
	ldr r2, [sp, #84]
	cmp r2, #63
	bgt .L_080eaf88
	ldr r4, [sp, #84]
	movs r3, #7
	ldr r5, [sp, #100]
	movs r6, #128
	subs r2, #4
	lsls r6, r6, #7
	ands r3, r4
	mov r8, r2
	adds r7, r5, r6
	cmp r3, #3
	ble .L_080eaed8
	ldr r0, [sp, #100]
	ldr r1, .L_080eb1c8
	adds r7, r0, r1
.L_080eaed8:
	movs r5, #2
	movs r1, #7
	movs r2, #7
	movs r3, #3
	movs r0, #47
	str r5, [sp, #0]
	bl Unnamed_080ed408
	movs r2, #24
	str r2, [sp, #0]
	str r2, [sp, #4]
	ldr r3, .L_080eb1cc
	mov r6, r8
	subs r6, #24
	ldr r0, [sp, #104]
	ldr r4, [r3]
	adds r1, r7, #0
	adds r3, r6, #0
	movs r2, #36
	bl _call_via_r4
	movs r0, #47
	bl Runtime_ReleaseHeapBlock
	movs r1, #7
	movs r2, #7
	movs r3, #7
	movs r0, #47
	str r5, [sp, #0]
	bl Unnamed_080ed408
	movs r4, #24
	str r4, [sp, #0]
	str r4, [sp, #4]
	ldr r0, .L_080eb1cc
	adds r3, r6, #0
	ldr r4, [r0]
	adds r1, r7, #0
	ldr r0, [sp, #104]
	movs r2, #59
	bl _call_via_r4
	movs r0, #47
	bl Runtime_ReleaseHeapBlock
	movs r1, #7
	movs r2, #7
	movs r3, #11
	movs r0, #47
	str r5, [sp, #0]
	bl Unnamed_080ed408
	movs r1, #24
	str r1, [sp, #0]
	str r1, [sp, #4]
	ldr r2, .L_080eb1cc
	adds r6, #23
	ldr r4, [r2]
	ldr r0, [sp, #104]
	adds r1, r7, #0
	movs r2, #36
	adds r3, r6, #0
	bl _call_via_r4
	movs r0, #47
	bl Runtime_ReleaseHeapBlock
	movs r1, #7
	movs r2, #7
	movs r3, #15
	movs r0, #47
	str r5, [sp, #0]
	bl Unnamed_080ed408
	movs r3, #24
	str r3, [sp, #0]
	str r3, [sp, #4]
	ldr r5, .L_080eb1cc
	ldr r0, [sp, #104]
	ldr r4, [r5]
	adds r1, r7, #0
	movs r2, #59
	adds r3, r6, #0
	bl _call_via_r4
	movs r0, #47
	bl Runtime_ReleaseHeapBlock
.L_080eaf88:
	ldr r3, [sp, #84]
	subs r3, #64
	cmp r3, #1
	bhi .L_080eafac
	movs r3, #16
	ldr r6, [sp, #100]
	movs r2, #192
	lsls r2, r2, #7
	str r3, [sp, #0]
	movs r3, #17
	str r3, [sp, #4]
	adds r1, r6, r2
	ldr r0, [sp, #104]
	movs r2, #52
	movs r3, #51
	ldr r4, [sp, #88]
	bl _call_via_r4
.L_080eafac:
	ldr r3, [sp, #84]
	subs r3, #66
	cmp r3, #1
	bhi .L_080eafce
	ldr r2, .L_080eb1d0
	ldr r6, [sp, #100]
	movs r3, #41
	movs r5, #24
	str r3, [sp, #4]
	adds r1, r6, r2
	str r5, [sp, #0]
	ldr r0, [sp, #104]
	movs r2, #48
	movs r3, #40
	ldr r4, [sp, #88]
	bl _call_via_r4
.L_080eafce:
	ldr r3, [sp, #84]
	subs r3, #68
	cmp r3, #7
	bhi .L_080eb054
	ldr r5, [sp, #84]
	movs r6, #76
	subs r6, r6, r5
	ldr r0, [sp, #100]
	ldr r1, .L_080eb1d4
	lsrs r5, r6, #31
	adds r5, r6, r5
	movs r2, #38
	adds r0, r0, r1
	mov r8, r2
	asrs r5, r5, #1
	ldr r2, [sp, #88]
	movs r4, #49
	mov r11, r0
	subs r4, r4, r5
	movs r1, #44
	mov r3, r8
	movs r0, #22
	subs r3, r3, r6
	str r0, [sp, #0]
	mov r10, r1
	str r1, [sp, #4]
	mov r9, r2
	str r4, [sp, #8]
	ldr r0, [sp, #104]
	adds r2, r4, #0
	mov r1, r11
	mov r8, r3
	bl _call_via_r9
	ldr r4, [sp, #8]
	adds r6, #38
	movs r3, #22
	mov r0, r10
	adds r2, r4, #0
	str r3, [sp, #0]
	str r0, [sp, #4]
	mov r1, r11
	ldr r0, [sp, #104]
	adds r3, r6, #0
	bl _call_via_r9
	adds r5, #49
	movs r1, #22
	mov r2, r10
	str r1, [sp, #0]
	str r2, [sp, #4]
	mov r1, r11
	adds r2, r5, #0
	mov r3, r8
	ldr r0, [sp, #104]
	bl _call_via_r9
	movs r3, #22
	mov r4, r10
	str r3, [sp, #0]
	str r4, [sp, #4]
	ldr r0, [sp, #104]
	mov r1, r11
	adds r2, r5, #0
	adds r3, r6, #0
	bl _call_via_r9
.L_080eb054:
	ldr r3, [sp, #84]
	subs r3, #78
	cmp r3, #1
	bhi .L_080eb078
	movs r3, #16
	ldr r5, [sp, #100]
	movs r6, #192
	str r3, [sp, #0]
	lsls r6, r6, #7
	movs r3, #17
	str r3, [sp, #4]
	ldr r0, [sp, #104]
	adds r1, r5, r6
	movs r2, #52
	movs r3, #51
	ldr r4, [sp, #88]
	bl _call_via_r4
.L_080eb078:
	ldr r3, [sp, #84]
	subs r3, #80
	cmp r3, #1
	bhi .L_080eb09a
	ldr r2, .L_080eb1d0
	ldr r6, [sp, #100]
	movs r3, #41
	movs r5, #24
	str r3, [sp, #4]
	adds r1, r6, r2
	str r5, [sp, #0]
	ldr r0, [sp, #104]
	movs r2, #48
	movs r3, #40
	ldr r4, [sp, #88]
	bl _call_via_r4
.L_080eb09a:
	ldr r3, [sp, #84]
	subs r3, #82
	cmp r3, #3
	bhi .L_080eb126
	ldr r5, [sp, #84]
	ldr r0, .L_080eb1d4
	lsls r7, r5, #1
	ldr r6, [sp, #100]
	adds r2, r7, #0
	subs r2, #164
	adds r0, r6, r0
	str r0, [sp, #28]
	adds r3, r2, #0
	cmp r2, #0
	bge .L_080eb0bc
	adds r3, r7, #0
	subs r3, #161
.L_080eb0bc:
	asrs r3, r3, #2
	movs r5, #49
	subs r5, r5, r3
	mov r9, r3
	movs r6, #38
	ldr r3, [sp, #88]
	subs r6, r6, r2
	movs r1, #22
	movs r2, #44
	str r1, [sp, #0]
	mov r8, r2
	str r2, [sp, #4]
	mov r10, r3
	ldr r1, [sp, #28]
	ldr r0, [sp, #104]
	adds r2, r5, #0
	adds r3, r6, #0
	bl _call_via_sl
	subs r7, #126
	movs r4, #22
	mov r11, r7
	mov r0, r8
	str r4, [sp, #0]
	str r0, [sp, #4]
	ldr r1, [sp, #28]
	adds r2, r5, #0
	ldr r0, [sp, #104]
	mov r3, r11
	bl _call_via_sl
	movs r1, #49
	add r9, r1
	movs r2, #22
	mov r3, r8
	str r2, [sp, #0]
	str r3, [sp, #4]
	ldr r1, [sp, #28]
	mov r2, r9
	adds r3, r6, #0
	ldr r0, [sp, #104]
	bl _call_via_sl
	mov r5, r8
	movs r4, #22
	str r4, [sp, #0]
	str r5, [sp, #4]
	ldr r0, [sp, #104]
	ldr r1, [sp, #28]
	mov r2, r9
	mov r3, r11
	bl _call_via_sl
.L_080eb126:
	ldr r3, [sp, #84]
	subs r3, #72
	cmp r3, #3
	bhi .L_080eb190
	ldr r6, [sp, #84]
	ldr r0, .L_080eb1d8
	lsls r3, r6, #4
	ldr r2, .L_080eb1dc
	adds r0, r0, r3
	movs r1, #5
	movs r3, #20
	mov r8, r0
	movs r7, #0
	mov r11, r1
	mov r9, r2
	mov r10, r3
.L_080eb146:
	lsls r6, r7, #8
	adds r0, r6, #0
	bl Trig_Sin
	mov r5, r8
	muls r5, r0
	mov r4, r11
	adds r0, r6, #0
	asrs r5, r5, #16
	subs r5, r5, r4
	bl Trig_Cos
	mov r3, r8
	muls r3, r0
	mov r2, r10
	subs r2, #2
	mov r6, r9
	ldrh r1, [r6, r2]
	ldr r0, [sp, #80]
	adds r5, #60
	movs r2, #10
	mov r4, r10
	asrs r3, r3, #17
	movs r6, #128
	adds r1, r0, r1
	str r2, [sp, #0]
	adds r3, #50
	adds r2, r5, #0
	str r4, [sp, #4]
	ldr r0, [sp, #104]
	ldr r5, [sp, #88]
	adds r7, #1
	lsls r6, r6, #1
	bl _call_via_r5
	cmp r7, r6
	bne .L_080eb146
.L_080eb190:
	ldr r0, [sp, #84]
	cmp r0, #85
	bgt .L_080eb198
	b .L_080eb364
.L_080eb198:
	cmp r0, #213
	ble .L_080eb19e
	b .L_080eb364
.L_080eb19e:
	ldr r1, [sp, #36]
	movs r7, #0
	cmp r1, #0
	bne .L_080eb1a8
	b .L_080eb364
.L_080eb1a8:
	ldr r2, [sp, #40]
	movs r3, #0
	mov lr, r2
	mov r10, r3
	mov r8, r2
	cmp r2, #0
	bge .L_080eb1b8
	b .L_080eb354
.L_080eb1b8:
	b .L_080eb1e0
	.2byte 0x0000
.L_080eb1bc:
	.4byte 0x3f3f3f3f
.L_080eb1c0:
	.4byte 0x040000d4
.L_080eb1c4:
	.4byte 0x85001000
.L_080eb1c8:
	.4byte 0x00004240
.L_080eb1cc:
	.4byte gTransitionWork + 0xc
.L_080eb1d0:
	.4byte 0x00006110
.L_080eb1d4:
	.4byte 0x000064e8
.L_080eb1d8:
	.4byte 0xfffffb90
.L_080eb1dc:
	.4byte ParticleStreams_CellOffsets
.L_080eb1e0:
	mov r4, lr
	lsrs r3, r4, #31
	add r3, lr
	asrs r3, r3, #1
	mov r9, r3
	lsls r3, r3, #3
	cmp r3, #0
	bge .L_080eb1f2
	adds r3, #7
.L_080eb1f2:
	ldr r5, [sp, #52]
	ldr r6, [sp, #52]
	ldr r1, [sp, #44]
	asrs r3, r3, #3
	adds r0, r5, r3
	subs r4, r6, r3
	mov r3, r10
	muls r3, r1
	cmp r3, #0
	bge .L_080eb208
	adds r3, #7
.L_080eb208:
	ldr r5, [sp, #48]
	asrs r3, r3, #3
	mov r6, r10
	adds r2, r5, r3
	lsls r3, r6, #3
	cmp r3, #0
	bge .L_080eb218
	adds r3, #7
.L_080eb218:
	ldr r5, [sp, #48]
	asrs r3, r3, #3
	subs r1, r5, r3
	cmp r1, #0
	bge .L_080eb224
	movs r1, #0
.L_080eb224:
	cmp r2, #119
	ble .L_080eb22a
	movs r2, #119
.L_080eb22a:
	cmp r4, #0
	bge .L_080eb230
	movs r4, #0
.L_080eb230:
	cmp r0, #119
	ble .L_080eb236
	movs r0, #119
.L_080eb236:
	lsls r3, r2, #4
	subs r3, r3, r2
	lsls r5, r3, #3
	ldr r3, [sp, #100]
	movs r2, #20
	adds r6, r5, r0
	strb r2, [r3, r6]
	lsls r3, r1, #4
	subs r3, r3, r1
	ldr r6, [sp, #100]
	lsls r1, r3, #3
	adds r3, r1, r0
	strb r2, [r6, r3]
	adds r3, r5, r4
	strb r2, [r6, r3]
	adds r3, r1, r4
	strb r2, [r6, r3]
	ldr r3, [sp, #52]
	ldr r2, [sp, #52]
	add r3, r9
	mov r4, r9
	adds r0, r3, #1
	subs r3, r2, r4
	adds r4, r3, #1
	cmp r4, #0
	bge .L_080eb26c
	movs r4, #0
.L_080eb26c:
	cmp r0, #119
	ble .L_080eb272
	movs r0, #119
.L_080eb272:
	ldr r6, [sp, #100]
	movs r2, #20
	adds r3, r5, r0
	strb r2, [r6, r3]
	adds r3, r1, r0
	strb r2, [r6, r3]
	adds r3, r5, r4
	strb r2, [r6, r3]
	mov r0, r10
	adds r3, r1, r4
	strb r2, [r6, r3]
	lsrs r3, r0, #31
	add r3, r10
	asrs r3, r3, #1
	mov r12, r3
	lsls r3, r3, #3
	cmp r3, #0
	bge .L_080eb298
	adds r3, #7
.L_080eb298:
	ldr r1, [sp, #52]
	ldr r2, [sp, #52]
	ldr r5, [sp, #44]
	asrs r3, r3, #3
	adds r0, r1, r3
	subs r4, r2, r3
	mov r3, lr
	muls r3, r5
	cmp r3, #0
	bge .L_080eb2ae
	adds r3, #7
.L_080eb2ae:
	ldr r6, [sp, #48]
	asrs r3, r3, #3
	mov r1, lr
	adds r2, r6, r3
	lsls r3, r1, #3
	cmp r3, #0
	bge .L_080eb2be
	adds r3, #7
.L_080eb2be:
	ldr r5, [sp, #48]
	asrs r3, r3, #3
	subs r1, r5, r3
	cmp r4, #0
	bge .L_080eb2ca
	movs r4, #0
.L_080eb2ca:
	cmp r0, #119
	ble .L_080eb2d0
	movs r0, #119
.L_080eb2d0:
	cmp r1, #0
	bge .L_080eb2d6
	movs r1, #0
.L_080eb2d6:
	cmp r2, #119
	ble .L_080eb2dc
	movs r2, #119
.L_080eb2dc:
	lsls r3, r2, #4
	subs r3, r3, r2
	lsls r6, r3, #3
	ldr r2, [sp, #100]
	movs r5, #20
	adds r3, r6, r0
	strb r5, [r2, r3]
	lsls r3, r1, #4
	subs r3, r3, r1
	lsls r2, r3, #3
	adds r3, r2, r0
	ldr r0, [sp, #100]
	strb r5, [r0, r3]
	adds r3, r6, r4
	strb r5, [r0, r3]
	adds r3, r2, r4
	strb r5, [r0, r3]
	ldr r3, [sp, #52]
	ldr r1, [sp, #52]
	add r3, r12
	mov r4, r12
	adds r0, r3, #1
	subs r3, r1, r4
	adds r4, r3, #1
	cmp r4, #0
	bge .L_080eb312
	movs r4, #0
.L_080eb312:
	cmp r0, #119
	ble .L_080eb318
	movs r0, #119
.L_080eb318:
	ldr r1, [sp, #100]
	adds r3, r6, r0
	strb r5, [r1, r3]
	adds r3, r2, r0
	strb r5, [r1, r3]
	adds r3, r6, r4
	strb r5, [r1, r3]
	adds r3, r2, r4
	mov r2, r10
	mov r4, r8
	strb r5, [r1, r3]
	lsls r3, r2, #1
	subs r3, r4, r3
	subs r3, #1
	mov r8, r3
	cmp r3, #0
	bge .L_080eb34a
	mov r5, lr
	lsls r3, r5, #1
	add r3, r8
	movs r6, #1
	subs r3, #2
	negs r6, r6
	mov r8, r3
	add lr, r6
.L_080eb34a:
	movs r0, #1
	add r10, r0
	cmp lr, r10
	blt .L_080eb354
	b .L_080eb1e0
.L_080eb354:
	ldr r1, [sp, #40]
	ldr r2, [sp, #36]
	adds r1, #1
	adds r7, #1
	str r1, [sp, #40]
	cmp r7, r2
	beq .L_080eb364
	b .L_080eb1a8
.L_080eb364:
	ldr r3, [sp, #84]
	subs r3, #86
	mov r8, r3
	cmp r3, #63
	bhi .L_080eb3e4
	ldr r5, [sp, #84]
	ldr r4, [sp, #84]
	lsrs r3, r5, #31
	adds r3, r5, r3
	subs r4, #70
	asrs r3, r3, #1
	ldr r6, .L_080eb60c
	movs r7, #0
	mov r9, r4
	mov r10, r3
.L_080eb382:
	adds r0, r6, #0
	bl Trig_Sin
	mov r3, r8
	muls r3, r0
	adds r0, r6, #0
	asrs r5, r3, #16
	bl Trig_Cos
	mov r3, r9
	muls r3, r0
	mov r1, r10
	adds r2, r1, r7
	asrs r0, r3, #16
	adds r3, r2, #0
	cmp r2, #0
	bge .L_080eb3a6
	adds r3, r2, #3
.L_080eb3a6:
	asrs r3, r3, #2
	lsls r3, r3, #2
	subs r3, r2, r3
	lsls r1, r3, #3
	subs r1, r1, r3
	lsls r1, r1, #2
	ldr r2, [sp, #100]
	subs r1, r1, r3
	lsls r1, r1, #6
	movs r3, #128
	adds r1, r2, r1
	lsls r3, r3, #7
	adds r1, r1, r3
	adds r3, r0, #0
	movs r0, #32
	adds r2, r5, #0
	str r0, [sp, #0]
	movs r5, #128
	movs r0, #54
	str r0, [sp, #4]
	adds r2, #44
	adds r3, #17
	ldr r0, [sp, #104]
	ldr r4, [sp, #88]
	lsls r5, r5, #5
	adds r7, #1
	bl _call_via_r4
	adds r6, r6, r5
	cmp r7, #9
	bne .L_080eb382
.L_080eb3e4:
	movs r3, #3
	movs r0, #47
	movs r1, #7
	movs r2, #7
	str r3, [sp, #0]
	bl Unnamed_080ed408
	ldr r6, .L_080eb610
	ldr r0, [sp, #84]
	ldr r4, [r6]
	str r4, [sp, #92]
	cmp r0, #85
	ble .L_080eb410
	movs r3, #120
	str r3, [sp, #0]
	str r3, [sp, #4]
	ldr r0, [sp, #104]
	ldr r1, [sp, #100]
	movs r2, #0
	movs r3, #0
	bl _call_via_r4
.L_080eb410:
	ldr r1, [sp, #84]
	ldr r2, .L_080eb614
	cmp r1, r2
	ble .L_080eb41e
	ldr r3, [sp, #32]
	subs r3, #8
	str r3, [sp, #32]
.L_080eb41e:
	ldr r4, [sp, #84]
	ldr r5, .L_080eb618
	cmp r4, r5
	ble .L_080eb45a
	ldr r3, .L_080eb61c
	ldr r6, [sp, #100]
	ldr r3, [r6, r3]
	ldr r3, [r3, #20]
	movs r7, #0
	cmp r3, #0
	beq .L_080eb45a
	ldr r0, .L_080eb61c
	adds r5, r6, r0
	movs r6, #36
.L_080eb43a:
	ldr r3, [r5]
	movs r2, #0
	ldrsh r0, [r3, r6]
	movs r3, #1
	str r2, [sp, #0]
	negs r3, r3
	movs r1, #14
	movs r2, #5
	bl ObjectGroup_UpdateMembers
	ldr r3, [r5]
	ldr r3, [r3, #20]
	adds r7, #1
	adds r6, #2
	cmp r7, r3
	bne .L_080eb43a
.L_080eb45a:
	ldr r3, [sp, #84]
	cmp r3, #238
	bne .L_080eb472
	ldr r3, .L_080eb620
	add r4, sp, #140
	str r3, [sp, #140]
	adds r0, r4, #0
	ldr r3, .L_080eb624
	ldr r1, [sp, #104]
	ldr r2, .L_080eb628
	stmia r3!, {r0, r1, r2}
	subs r3, #12
.L_080eb472:
	ldr r3, [sp, #84]
	subs r3, #214
	cmp r3, #65
	bhi .L_080eb482
	ldr r2, .L_080eb62c
	ldrh r3, [r2, #4]
	adds r3, #8
	strh r3, [r2, #4]
.L_080eb482:
	ldr r3, [sp, #84]
	subs r3, #246
	cmp r3, #33
	bhi .L_080eb4a4
	ldr r5, [sp, #32]
	ldr r6, [sp, #100]
	movs r0, #239
	lsls r0, r0, #7
	adds r3, r6, r0
	adds r5, #8
	movs r1, #0
	str r5, [sp, #32]
	str r1, [r3]
	ldr r3, .L_080eb630
	adds r2, r6, r3
	movs r3, #75
	str r3, [r2]
.L_080eb4a4:
	ldr r4, [sp, #84]
	cmp r4, #213
	bgt .L_080eb4ac
	b .L_080eb6d2
.L_080eb4ac:
	add r3, sp, #124
	movs r5, #0
	str r5, [r3, #12]
	str r5, [r3, #4]
	ldr r0, [sp, #100]
	movs r6, #108
	ldr r1, .L_080eb634
	movs r2, #225
	add r6, sp
	lsls r2, r2, #7
	mov r8, r6
	movs r7, #0
	adds r4, r3, #0
	adds r6, r0, r1
	adds r5, r0, r2
.L_080eb4ca:
	ldr r3, [r5, #24]
	cmp r3, #0
	beq .L_080eb56a
	ldr r2, [r5, #4]
	ldr r3, [r5, #8]
	asrs r2, r2, #7
	ldr r0, .L_080eb638
	adds r3, r3, r2
	str r3, [sp, #108]
	cmp r3, r0
	bgt .L_080eb4e6
	movs r3, #128
	lsls r3, r3, #4
	str r3, [sp, #108]
.L_080eb4e6:
	mov r1, r8
	str r3, [r1, #4]
	movs r2, #255
	ldr r3, [r5]
	lsls r2, r2, #16
	str r2, [r4, #4]
	str r3, [r4]
	ldr r3, [r5, #4]
	adds r3, r3, r2
	str r3, [r4, #8]
	adds r1, r4, #0
	mov r2, r8
	movs r3, #0
	ldr r0, [r6]
	str r4, [sp, #8]
	bl Object_ApplyProjectedPlacementFar
	ldr r2, [r5, #4]
	ldr r3, [r5, #16]
	subs r2, r2, r3
	ldr r3, .L_080eb63c
	str r2, [r5, #4]
	ldr r4, [sp, #8]
	cmp r2, r3
	bgt .L_080eb56a
	ldr r0, [sp, #84]
	ldr r1, .L_080eb614
	cmp r0, r1
	bgt .L_080eb530
	bl Random16
	movs r3, #63
	ldr r2, [sp, #32]
	ands r3, r0
	adds r3, r3, r2
	adds r3, #32
	b .L_080eb540
.L_080eb530:
	str r4, [sp, #8]
	bl Random16
	movs r3, #63
	ands r3, r0
	ldr r0, [sp, #32]
	adds r3, r3, r0
	subs r3, #32
.L_080eb540:
	lsls r3, r3, #16
	str r3, [r5]
	ldr r4, [sp, #8]
	ldr r1, [sp, #84]
	ldr r2, .L_080eb640
	cmp r1, r2
	ble .L_080eb554
	movs r3, #0
	str r3, [r5, #24]
	b .L_080eb56a
.L_080eb554:
	ldr r0, [sp, #84]
	cmp r0, #245
	ble .L_080eb560
	movs r3, #192
	lsls r3, r3, #15
	b .L_080eb568
.L_080eb560:
	ldr r2, [r5]
	movs r3, #192
	lsls r3, r3, #16
	subs r3, r3, r2
.L_080eb568:
	str r3, [r5, #4]
.L_080eb56a:
	adds r7, #1
	adds r6, #4
	adds r5, #28
	cmp r7, #16
	bne .L_080eb4ca
	ldr r1, [sp, #84]
	ldr r2, .L_080eb614
	cmp r1, r2
	bgt .L_080eb644
	ldr r4, [sp, #32]
	movs r3, #15
	mov r10, r3
	lsrs r3, r4, #31
	adds r3, r4, r3
	asrs r1, r1, #31
	asrs r3, r3, #1
	movs r7, #0
	mov r8, r1
	mov r9, r3
.L_080eb590:
	bl Random16
	movs r2, #1
	ands r2, r7
	mov r5, r10
	ands r0, r5
	lsls r3, r2, #2
	lsrs r5, r7, #31
	adds r3, r3, r2
	adds r5, r7, r5
	asrs r5, r5, #1
	lsls r3, r3, #2
	lsls r2, r5, #2
	subs r3, r3, r0
	adds r3, r3, r2
	add r3, r9
	adds r6, r3, #0
	bl Random16
	mov r2, r8
	ldr r4, [sp, #84]
	lsrs r3, r2, #31
	adds r3, r4, r3
	asrs r3, r3, #1
	mov r1, r10
	lsls r5, r5, #5
	ands r0, r1
	adds r2, r3, r7
	subs r6, #16
	subs r5, r5, r0
	adds r3, r2, #0
	cmp r2, #0
	bge .L_080eb5d4
	adds r3, r2, #3
.L_080eb5d4:
	asrs r3, r3, #2
	lsls r3, r3, #2
	subs r3, r2, r3
	lsls r1, r3, #3
	subs r1, r1, r3
	lsls r1, r1, #2
	ldr r0, [sp, #100]
	subs r1, r1, r3
	lsls r1, r1, #6
	movs r3, #32
	movs r2, #128
	adds r1, r0, r1
	lsls r2, r2, #7
	str r3, [sp, #0]
	movs r3, #54
	adds r1, r1, r2
	str r3, [sp, #4]
	ldr r0, [sp, #104]
	adds r2, r6, #0
	adds r3, r5, #0
	ldr r4, [sp, #92]
	adds r7, #1
	bl _call_via_r4
	cmp r7, #8
	bne .L_080eb590
	b .L_080eb6d2
	.2byte 0x0000
.L_080eb60c:
	.4byte 0xffffc000
.L_080eb610:
	.4byte gTransitionWork + 0xc
.L_080eb614:
	.4byte 0x00000117
.L_080eb618:
	.4byte 0x0000013f
.L_080eb61c:
	.4byte 0x00007828
.L_080eb620:
	.4byte 0x3f3f3f3f
.L_080eb624:
	.4byte 0x040000d4
.L_080eb628:
	.4byte 0x85001000
.L_080eb62c:
	.4byte gBgScroll
.L_080eb630:
	.4byte 0x00007784
.L_080eb634:
	.4byte 0x000077d8
.L_080eb638:
	.4byte 0x000007ff
.L_080eb63c:
	.4byte 0x000fffff
.L_080eb640:
	.4byte 0x0000012d
.L_080eb644:
	ldr r0, [sp, #32]
	ldr r5, [sp, #84]
	lsrs r3, r0, #31
	adds r3, r0, r3
	asrs r5, r5, #31
	movs r6, #15
	asrs r3, r3, #1
	movs r7, #0
	mov r8, r5
	mov r10, r6
	mov r9, r3
.L_080eb65a:
	bl Random16
	movs r2, #1
	ands r2, r7
	lsls r3, r2, #2
	lsrs r5, r7, #31
	adds r3, r3, r2
	mov r1, r10
	adds r5, r7, r5
	ands r0, r1
	asrs r5, r5, #1
	lsls r3, r3, #2
	lsls r2, r5, #2
	adds r3, r3, r0
	subs r3, r3, r2
	add r3, r9
	adds r6, r3, #0
	bl Random16
	mov r2, r10
	ands r0, r2
	lsls r5, r5, #5
	subs r5, r5, r0
	mov r4, r8
	ldr r0, [sp, #84]
	lsrs r3, r4, #31
	adds r3, r0, r3
	asrs r3, r3, #1
	adds r2, r3, r7
	subs r6, #16
	adds r3, r2, #0
	cmp r2, #0
	bge .L_080eb69e
	adds r3, r2, #3
.L_080eb69e:
	asrs r3, r3, #2
	lsls r3, r3, #2
	subs r3, r2, r3
	lsls r1, r3, #3
	subs r1, r1, r3
	lsls r1, r1, #2
	ldr r2, [sp, #100]
	subs r1, r1, r3
	lsls r1, r1, #6
	movs r3, #128
	adds r1, r2, r1
	lsls r3, r3, #7
	adds r1, r1, r3
	movs r3, #32
	str r3, [sp, #0]
	movs r3, #54
	str r3, [sp, #4]
	ldr r0, [sp, #104]
	adds r2, r6, #0
	adds r3, r5, #0
	ldr r4, [sp, #92]
	adds r7, #1
	bl _call_via_r4
	cmp r7, #8
	bne .L_080eb65a
.L_080eb6d2:
	movs r0, #47
	bl Runtime_ReleaseHeapBlock
	ldr r5, [sp, #84]
	cmp r5, #63
	ble .L_080eb6ec
	bl Random16
	movs r3, #3
	ands r3, r0
	ldr r2, .L_080eb744
	adds r3, #30
	strh r3, [r2, #6]
.L_080eb6ec:
	ldr r6, [sp, #100]
	ldr r0, .L_080eb748
	movs r3, #1
	adds r2, r6, r0
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r1, [sp, #84]
	movs r2, #160
	adds r1, #1
	lsls r2, r2, #1
	str r1, [sp, #84]
	cmp r1, r2
	beq .L_080eb70e
	bl .L_080eaa8e
.L_080eb70e:
	ldr r3, [sp, #100]
	ldr r4, .L_080eb74c
	movs r7, #0
	adds r6, r3, r4
.L_080eb716:
	ldmia r6!, {r0}
	adds r7, #1
	bl ResourceObject_ReleaseFar
	cmp r7, #16
	bne .L_080eb716
	ldr r0, .L_080eb750
	bl Scheduler_RemoveCallback
	movs r0, #46
	bl Runtime_ReleaseHeapBlock
	bl BattleFx_EndCanvasLayer
	add sp, #184
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
.L_080eb744:
	.4byte gBgScroll
.L_080eb748:
	.4byte 0x00007824
.L_080eb74c:
	.4byte 0x000077d8
.L_080eb750:
	.4byte BattlePresentation_ProcessPendingGraphicsTransfer
