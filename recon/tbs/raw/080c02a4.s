.syntax unified
	.thumb
	.global Func_080c02a4
	.thumb_func
Func_080c02a4:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #148
	str r0, [sp, #0]
	ldr r6, .L_080c0380
	adds r5, r1, #0
	movs r0, #42
	movs r1, #4
	ldr r7, [r6]
	bl Runtime_AllocateHeapBlock
	ldr r1, .L_080c0384
	mov r11, r0
	cmp r5, r1
	bne .L_080c02ce
	b .L_080c04c8
.L_080c02ce:
	ldr r2, .L_080c0388
	mov r12, r2
	ldr r3, .L_080c038c
	mov r0, r12
	ldr r1, .L_080c0390
	ldr r2, .L_080c0394
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	mov r0, r12
	adds r0, #32
	adds r1, #32
	ldr r2, .L_080c0394
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	mov r0, r12
	adds r0, #64
	adds r1, #32
	ldr r2, .L_080c0394
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	mov r0, r12
	adds r0, #96
	adds r1, #32
	ldr r2, .L_080c0394
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	mov r0, r12
	adds r0, #128
	adds r1, #32
	ldr r2, .L_080c0394
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	mov r0, r12
	adds r0, #160
	adds r1, #32
	ldr r2, .L_080c0394
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	mov r0, r12
	adds r0, #192
	adds r1, #32
	ldr r2, .L_080c0394
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	subs r3, #212
	movs r2, #1
	strh r2, [r3]
	ldr r3, .L_080c0398
	add r4, sp, #144
	movs r5, #0
	str r2, [r7, #12]
	str r2, [r7, #8]
	str r5, [r7, #16]
	adds r0, r4, #0
	str r3, [r4]
	subs r1, #224
	ldr r3, .L_080c038c
	ldr r2, .L_080c039c
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	str r5, [r4]
	adds r0, r4, #0
	ldr r1, .L_080c03a0
	ldr r2, .L_080c039c
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r2, .L_080c03a4
	ldr r3, .L_080c0378
	ldr r1, .L_080c03a8
	strh r3, [r2]
	ldr r2, .L_080c037c
	ldrh r3, [r1]
	orrs r3, r2
	strh r3, [r1]
	movs r3, #2
	str r3, [r7, #8]
	ldr r2, .L_080c03ac
	movs r6, #0
.L_080c036a:
	ldr r1, .L_080c03b0
	cmp r6, #20
	bls .L_080c0372
	ldr r1, .L_080c03b4
.L_080c0372:
	movs r3, #0
	b .L_080c03b8
	.2byte 0x0000
.L_080c0378:
	.4byte 0x00000c04
.L_080c037c:
	.4byte 0x00000002
.L_080c0380:
	.4byte gTransitionWork
.L_080c0384:
	.4byte 0x0000015b
.L_080c0388:
	.4byte Data_080c5b30
.L_080c038c:
	.4byte 0x040000d4
.L_080c0390:
	.4byte 0x06005020
.L_080c0394:
	.4byte 0x84000008
.L_080c0398:
	.4byte 0x33333333
.L_080c039c:
	.4byte 0x85000008
.L_080c03a0:
	.4byte 0x06005100
.L_080c03a4:
	.4byte 0x0400000a
.L_080c03a8:
	.4byte 0x04000008
.L_080c03ac:
	.4byte 0x06006000
.L_080c03b0:
	.4byte 0x0000f080
.L_080c03b4:
	.4byte 0x0000f088
.L_080c03b8:
	adds r3, #1
	strh r1, [r2]
	adds r2, #2
	cmp r3, #31
	bls .L_080c03b8
	adds r6, #1
	cmp r6, #31
	bls .L_080c036a
	ldr r6, .L_080c0424
	movs r3, #0
	movs r5, #32
	mov r9, r3
	movs r3, #8
	strh r5, [r6, #2]
	strh r5, [r6, #6]
	strh r3, [r6, #4]
	movs r0, #1
	bl WaitFrames
	ldr r1, .L_080c0414
	ldr r3, .L_080c0428
	ldr r2, .L_080c0418
	strh r1, [r3]
	adds r3, #4
	strh r2, [r3]
	subs r3, #2
	strh r1, [r3]
	adds r3, #4
	strh r2, [r3]
	ldr r2, .L_080c042c
	ldr r3, .L_080c041c
	strh r3, [r2]
	ldr r3, .L_080c0420
	adds r2, #2
	movs r0, #128
	strh r3, [r2]
	lsls r0, r0, #19
	ldr r1, .L_080c0430
	bl QueueIoWriteDelay2
	movs r0, #0
	movs r1, #0
	movs r2, #0
	movs r3, #180
	b .L_080c0434
	.2byte 0x0000
.L_080c0414:
	.4byte 0x000000f0
.L_080c0418:
	.4byte 0x00000088
.L_080c041c:
	.4byte 0x00003537
.L_080c0420:
	.4byte 0x00003f21
.L_080c0424:
	.4byte gBgScroll
.L_080c0428:
	.4byte 0x04000040
.L_080c042c:
	.4byte 0x04000048
.L_080c0430:
	.4byte 0x00007741
.L_080c0434:
	bl BattlePres_SetupTransitionScene
	ldr r3, .L_080c04b4
	mov r2, r11
	mov r1, r9
	mov r10, r3
	str r1, [r2]
	movs r1, #200
	lsls r1, r1, #4
	mov r0, r10
	bl Scheduler_AddOrUpdateCallback
	ldr r1, .L_080c04b8
	mov r8, r1
	movs r1, #144
	lsls r1, r1, #3
	mov r0, r8
	bl Scheduler_AddOrUpdateCallback
	ldr r2, .L_080c04bc
	movs r1, #32
	movs r0, #2
	bl Runtime_SetIrqHandler
	strh r5, [r6, #2]
	movs r0, #1
	bl WaitFrames
	ldr r3, .L_080c04c0
	ldr r3, [r3]
	adds r3, #65
	ldrb r0, [r3]
	ldr r5, .L_080c04c4
	bl UiWindow_CreateWithLayoutBoundsFar
	movs r0, #20
	bl WaitFrames
	adds r0, r5, #0
	movs r1, #2
	bl QueueIoWriteDelay10
	movs r1, #0
	adds r0, r5, #0
	bl QueueIoWriteDelay6
	ldr r0, [sp, #0]
	bl BattleIntro_AnnounceEncounter
	mov r0, r10
	bl Scheduler_RemoveCallback
	mov r0, r8
	bl Scheduler_RemoveCallback
	mov r2, r9
	strh r2, [r6, #2]
	movs r0, #2
	movs r1, #0
	movs r2, #0
	bl Runtime_SetIrqHandler
	b .L_080c069c
	.2byte 0x0000
.L_080c04b4:
	.4byte BattlePres_AdvanceTransitionTimer
.L_080c04b8:
	.4byte BattlePres_DrawTransitionRows
.L_080c04bc:
	.4byte Graphics_ClearBg0Vofs
.L_080c04c0:
	.4byte gBattleWork
.L_080c04c4:
	.4byte 0x04000008
.L_080c04c8:
	adds r3, r6, #0
	subs r3, #140
	ldr r3, [r3]
	mov r10, r3
	movs r3, #1
	str r3, [r7, #12]
	movs r3, #0
	str r3, [r7, #16]
	add r1, sp, #32
	movs r0, #3
	bl BattleParty_ListActorIds
	movs r6, #0
	mov r8, r0
	cmp r0, #0
	beq .L_080c0516
.L_080c04e8:
	adds r5, r6, #0
	adds r5, #120
	cmp r6, #7
	bgt .L_080c04f2
	adds r5, r6, #0
.L_080c04f2:
	adds r0, r5, #0
	bl GetBattleObjectSlot
	adds r7, r0, #0
	adds r0, r5, #0
	bl Owner_GetStateFar
	movs r3, #148
	lsls r3, r3, #1
	adds r0, r0, r3
	ldrb r3, [r0]
	cmp r3, #148
	beq .L_080c0510
	ldr r3, .L_080c05a8
	str r3, [r7, #24]
.L_080c0510:
	adds r6, #1
	cmp r6, r8
	bne .L_080c04e8
.L_080c0516:
	ldr r1, .L_080c05ac
	ldr r0, .L_080c05b0
	ldrh r3, [r0]
	adds r4, r3, #0
	strh r0, [r0]
	ldrh r2, [r1]
	cmp r2, #31
	bgt .L_080c0544
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	adds r2, #1
	adds r3, r3, r1
	strh r2, [r1]
	ldr r2, .L_080c05b4
	adds r3, #4
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #19
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #10
	str r2, [r3]
.L_080c0544:
	strh r4, [r0]
	movs r0, #1
	bl WaitFrames
	movs r3, #201
	ldr r2, .L_080c05b8
	lsls r3, r3, #3
	add r6, sp, #96
	add r3, r10
	movs r1, #0
	mov r8, r1
	strh r2, [r3]
	adds r1, r6, #0
	movs r0, #2
	add r5, sp, #60
	bl BattleParty_ListActorIds
	ldr r2, .L_080c05a4
	str r0, [r5, #20]
	mov r10, r2
	lsls r0, r0, #1
	mov r3, r10
	adds r0, #36
	strh r3, [r5, r0]
	movs r1, #0
	adds r0, r6, #0
	bl BattleActor_SpawnObjectsForList
	adds r0, r5, #0
	bl BattleEffect_RunTileAndPaletteAnimationFar
	movs r3, #100
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl BattlePres_SetupTransitionScene
	mov r1, r8
	mov r2, r11
	str r1, [r2]
	movs r0, #2
	ldr r2, .L_080c05bc
	movs r1, #32
	bl Runtime_SetIrqHandler
	movs r0, #1
	b .L_080c05c0
	.2byte 0x0000
.L_080c05a4:
	.4byte 0x000000ff
.L_080c05a8:
	.4byte 0x0000b333
.L_080c05ac:
	.4byte gIoWriteQueue
.L_080c05b0:
	.4byte 0x04000208
.L_080c05b4:
	.4byte 0x00006041
.L_080c05b8:
	.4byte 0x00000021
.L_080c05bc:
	.4byte Graphics_ClearBg0Vofs
.L_080c05c0:
	bl WaitFrames
	movs r0, #20
	bl WaitFrames
	ldr r3, .L_080c0624
	ldr r3, [r3]
	ldr r5, .L_080c0628
	adds r3, #65
	ldrb r0, [r3]
	bl UiWindow_CreateWithLayoutBoundsFar
	adds r0, r5, #0
	movs r1, #2
	bl QueueIoWriteDelay10
	adds r0, r5, #0
	movs r1, #0
	bl QueueIoWriteDelay6
	ldr r2, .L_080c062c
	ldr r3, .L_080c0620
	strh r3, [r2]
	add r3, sp, #4
	mov r8, r3
	mov r1, r8
	movs r0, #3
	bl BattleParty_ListActorIds
	adds r7, r0, #0
	lsls r3, r7, #1
	mov r2, r10
	mov r1, r8
	strh r2, [r1, r3]
	mov r0, r8
	movs r1, #0
	bl BattleActor_SpawnObjectsForList
	movs r0, #1
	mov r1, r8
	bl BattleParty_ListActorIds
	adds r7, r0, #0
	movs r6, #0
	cmp r7, #0
	beq .L_080c0640
	mov r5, r8
	b .L_080c0630
.L_080c0620:
	.4byte 0x00003f40
.L_080c0624:
	.4byte gBattleWork
.L_080c0628:
	.4byte 0x04000008
.L_080c062c:
	.4byte 0x04000050
.L_080c0630:
	ldrh r0, [r5]
	movs r1, #1
	adds r6, #1
	adds r5, #2
	bl BattlePres_SetActorRecordMode
	cmp r6, r7
	bne .L_080c0630
.L_080c0640:
	ldr r3, .L_080c066c
	ldr r5, .L_080c0668
	movs r6, #0
	mov r10, r3
.L_080c0648:
	adds r3, r6, #0
	orrs r3, r5
	mov r1, r10
	strh r3, [r1]
	movs r0, #1
	adds r6, #1
	bl WaitFrames
	cmp r6, #16
	bne .L_080c0648
	movs r6, #0
	cmp r7, #0
	beq .L_080c0680
	mov r5, r8
	b .L_080c0670
	.2byte 0x0000
.L_080c0668:
	.4byte 0x00001000
.L_080c066c:
	.4byte 0x04000052
.L_080c0670:
	ldrh r0, [r5]
	movs r1, #0
	adds r6, #1
	adds r5, #2
	bl BattlePres_SetActorRecordMode
	cmp r6, r7
	bne .L_080c0670
.L_080c0680:
	ldr r0, [sp, #0]
	bl BattleIntro_AnnounceEncounter
	ldr r2, .L_080c06e0
	movs r3, #0
	strh r3, [r2, #2]
	movs r0, #1
	bl WaitFrames
	movs r0, #2
	movs r1, #0
	movs r2, #0
	bl Runtime_SetIrqHandler
.L_080c069c:
	ldr r6, .L_080c06e4
	ldr r5, .L_080c06d8
	movs r1, #0
	movs r2, #0
	movs r0, #2
	bl Runtime_SetIrqHandler
	strh r5, [r6]
	movs r0, #1
	bl WaitFrames
	strh r5, [r6]
	ldr r1, .L_080c06e8
	ldr r3, .L_080c06ec
	ldrh r2, [r1]
	ands r3, r2
	ldr r2, .L_080c06e0
	strh r3, [r1]
	movs r3, #8
	strh r3, [r2, #4]
	ldr r3, .L_080c06dc
	movs r2, #128
	lsls r2, r2, #19
	strh r3, [r2]
	movs r0, #42
	bl Runtime_ReleaseHeapBlock
	add sp, #148
	b .L_080c06f0
	.2byte 0x0000
.L_080c06d8:
	.4byte 0x00001f83
.L_080c06dc:
	.4byte 0x00001541
.L_080c06e0:
	.4byte gBgScroll
.L_080c06e4:
	.4byte 0x0400000a
.L_080c06e8:
	.4byte 0x04000008
.L_080c06ec:
	.4byte 0x0000fffd
.L_080c06f0:
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
