.syntax unified
	.thumb
	.global BattleFx_PrepareCanvasEffect
	.thumb_func
BattleFx_PrepareCanvasEffect:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #108
	adds r6, r2, #0
	ldr r2, .L_080de518
	str r3, [sp, #44]
	adds r3, r2, #0
	adds r5, r1, #0
	ldmia r3!, {r1}
	str r1, [sp, #40]
	ldr r3, [r3]
	str r3, [sp, #36]
	ldr r2, [r2, #8]
	str r2, [sp, #24]
	ldr r2, .L_080de51c
	adds r3, r1, r2
	str r0, [r3]
	movs r0, #0
	bl BattleFx_BeginCanvasLayer
	ldr r4, .L_080de520
	ldr r3, [sp, #40]
	adds r2, r3, r4
	movs r3, #24
	str r3, [r2]
	ldr r7, [sp, #40]
	ldr r1, .L_080de524
	movs r3, #0
	adds r2, r7, r1
	str r3, [r2]
	cmp r5, #3
	ble .L_080de34a
	movs r2, #84
	subs r5, #4
	str r2, [sp, #20]
	b .L_080de34e
.L_080de34a:
	movs r3, #64
	str r3, [sp, #20]
.L_080de34e:
	cmp r5, #1
	beq .L_080de366
	cmp r5, #1
	bgt .L_080de35c
	cmp r5, #0
	beq .L_080de362
	b .L_080de36e
.L_080de35c:
	cmp r5, #2
	beq .L_080de36a
	b .L_080de36e
.L_080de362:
	ldr r0, .L_080de528
	b .L_080de370
.L_080de366:
	ldr r0, .L_080de52c
	b .L_080de370
.L_080de36a:
	ldr r0, .L_080de530
	b .L_080de370
.L_080de36e:
	ldr r0, .L_080de534
.L_080de370:
	bl Resource_GetTableEntry
	adds r5, r0, #0
	movs r0, #160
	ldr r3, .L_080de538
	adds r1, r5, #0
	movs r2, #128
	lsls r0, r0, #19
	bl _call_via_r3
	adds r5, #128
	ldr r1, [sp, #40]
	adds r0, r5, #0
	bl Resource_DecodeType01
	ldr r0, .L_080de53c
	ldr r1, [sp, #24]
	movs r2, #0
	movs r3, #0
	bl Resource_LoadAndDecompress
	cmp r6, #1
	bne .L_080de3c0
	movs r3, #3
	str r3, [sp, #0]
	movs r1, #7
	movs r2, #7
	movs r3, #7
	movs r0, #46
	bl Unnamed_080ed408
	movs r3, #2
	str r3, [sp, #0]
	movs r0, #47
	movs r1, #7
	movs r2, #7
	movs r3, #7
	bl Unnamed_080ed408
	b .L_080de3de
.L_080de3c0:
	movs r3, #3
	movs r1, #7
	movs r2, #7
	movs r0, #46
	str r3, [sp, #0]
	bl Unnamed_080ed408
	movs r3, #2
	str r3, [sp, #0]
	movs r0, #47
	movs r1, #7
	movs r2, #7
	movs r3, #3
	bl Unnamed_080ed408
.L_080de3de:
	ldr r3, .L_080de540
	ldr r4, [sp, #40]
	adds r2, r3, #0
	ldr r7, .L_080de51c
	adds r3, #188
	ldr r3, [r3]
	adds r5, r4, r7
	str r3, [sp, #32]
	adds r2, #184
	ldr r3, [r5]
	ldr r2, [r2]
	ldr r0, [r3, #8]
	str r2, [sp, #28]
	bl GetBattleObjectSlotFar
	ldr r3, [r5]
	ldr r0, [r0]
	mov r9, r0
	movs r1, #36
	ldrsh r0, [r3, r1]
	bl GetBattleObjectSlotFar
	ldr r0, [r0]
	movs r2, #0
	str r0, [sp, #16]
	ldr r7, .L_080de544
	mov r8, r2
	mov r10, r2
.L_080de416:
	bl Random16
	ldr r3, .L_080de548
	adds r6, r0, #0
	ands r6, r3
	bl Random16
	mov r3, r10
	str r3, [r7]
	movs r5, #255
	ands r5, r0
	bl Random16
	movs r3, #31
	ands r3, r0
	adds r3, #20
	mov r4, r10
	lsls r3, r3, #16
	str r4, [r7, #8]
	str r3, [r7, #4]
	adds r0, r6, #0
	bl Trig_Sin
	adds r5, #128
	adds r3, r5, #0
	muls r3, r0
	mov r1, r10
	asrs r3, r3, #5
	str r3, [r7, #12]
	str r1, [r7, #16]
	adds r0, r6, #0
	bl Trig_Cos
	adds r3, r5, #0
	muls r3, r0
	asrs r3, r3, #5
	str r3, [r7, #20]
	movs r3, #1
	add r8, r3
	mov r2, r10
	mov r4, r8
	str r2, [r7, #24]
	adds r7, #28
	cmp r4, #64
	bne .L_080de416
	ldr r7, [sp, #40]
	movs r1, #239
	lsls r1, r1, #7
	adds r2, r7, r1
	movs r3, #2
	str r3, [r2]
	ldr r3, .L_080de54c
	movs r1, #144
	adds r2, r7, r3
	movs r3, #75
	str r3, [r2]
	lsls r1, r1, #3
	ldr r0, .L_080de550
	bl Scheduler_AddOrUpdateCallback
	mov r7, r9
	ldr r3, [r7, #8]
	add r4, sp, #96
	str r3, [r4]
	movs r3, #0
	str r3, [r4, #4]
	ldr r3, [r7, #16]
	str r3, [r4, #8]
	ldr r1, [sp, #44]
	mov r11, r4
	cmp r1, #4
	bhi .L_080de558
	ldr r2, .L_080de554
	lsls r3, r1, #2
	ldr r3, [r3, r2]
	mov pc, r3
	.2byte 0x0000
.L_080de4b0:
	.4byte .L_080de4c4
	.4byte .L_080de4d8
	.4byte .L_080de4de
	.4byte .L_080de4f2
	.4byte .L_080de506
.L_080de4c4:
	ldr r2, [sp, #16]
	ldr r3, [r2, #8]
	add r5, sp, #84
	str r3, [r5]
	movs r3, #240
	lsls r3, r3, #14
	str r3, [r5, #4]
	ldr r3, [r2, #16]
	str r3, [r5, #8]
	b .L_080de55a
.L_080de4d8:
	ldr r4, [sp, #16]
	ldr r3, [r4, #8]
	b .L_080de4f6
.L_080de4de:
	mov r7, r9
	ldr r3, [r7, #8]
	add r5, sp, #84
	str r3, [r5]
	movs r3, #240
	lsls r3, r3, #14
	str r3, [r5, #4]
	ldr r3, [r7, #16]
	str r3, [r5, #8]
	b .L_080de55a
.L_080de4f2:
	mov r1, r9
	ldr r3, [r1, #8]
.L_080de4f6:
	add r5, sp, #84
	str r3, [r5]
	movs r3, #240
	lsls r3, r3, #14
	str r3, [r5, #4]
	movs r3, #0
	str r3, [r5, #8]
	b .L_080de55a
.L_080de506:
	movs r3, #240
	add r5, sp, #84
	movs r2, #0
	lsls r3, r3, #14
	str r2, [r5]
	str r3, [r5, #4]
	str r2, [r5, #8]
	b .L_080de55a
	.2byte 0x0000
.L_080de518:
	.4byte gBattleFxWork
.L_080de51c:
	.4byte 0x00007828
.L_080de520:
	.4byte 0x000077b4
.L_080de524:
	.4byte 0x000077b8
.L_080de528:
	.4byte 0x00000094
.L_080de52c:
	.4byte 0x00000092
.L_080de530:
	.4byte 0x0000008e
.L_080de534:
	.4byte 0x00000090
.L_080de538:
	.4byte IwramCopyWords
.L_080de53c:
	.4byte 0x00000073
.L_080de540:
	.4byte gWorkSlot
.L_080de544:
	.4byte gMapCellBuffer
.L_080de548:
	.4byte 0x0000ffff
.L_080de54c:
	.4byte 0x00007784
.L_080de550:
	.4byte BattlePresentation_ProcessPendingGraphicsTransfer
.L_080de554:
	.4byte .L_080de4b0
.L_080de558:
	add r5, sp, #84
.L_080de55a:
	mov r2, sp
	adds r2, #72
	str r2, [sp, #12]
	mov r4, r11
	ldr r3, [r4]
	ldr r0, [r5]
	movs r1, #40
	subs r0, r0, r3
	bl FixedPoint_Ratio
	ldr r7, [sp, #12]
	str r0, [r7]
	mov r1, r11
	ldr r3, [r1, #4]
	ldr r0, [r5, #4]
	movs r1, #40
	subs r0, r0, r3
	bl FixedPoint_Ratio
	str r0, [r7, #4]
	mov r2, r11
	ldr r3, [r2, #8]
	ldr r0, [r5, #8]
	movs r1, #40
	subs r0, r0, r3
	bl FixedPoint_Ratio
	str r0, [r7, #8]
	ldr r4, [sp, #20]
	movs r3, #0
	mov r10, r3
	cmp r4, #0
	bne .L_080de59e
	b .L_080de8f4
.L_080de59e:
	ldr r3, .L_080de5cc
	mov r7, r10
	ldr r5, [r3]
	cmp r7, #75
	ble .L_080de5b6
	ldr r3, .L_080de5c4
	lsls r2, r7, #1
	subs r3, r3, r2
	ldr r2, .L_080de5c8
	ldr r1, .L_080de5d0
	orrs r3, r2
	strh r3, [r1]
.L_080de5b6:
	mov r1, r10
	cmp r1, #8
	bne .L_080de5d4
	movs r0, #212
	bl AudioCommand_PlayFar
	b .L_080de5d4
.L_080de5c4:
	.4byte 0x000000a8
.L_080de5c8:
	.4byte 0x00001000
.L_080de5cc:
	.4byte gCameraWork
.L_080de5d0:
	.4byte 0x04000052
.L_080de5d4:
	bl Render_ResetTransformState
	adds r1, r5, #0
	adds r1, #12
	adds r0, r5, #0
	bl Graphics_PrepareTransferInIwramWork
	mov r3, r10
	subs r3, #6
	cmp r3, #39
	bhi .L_080de608
	ldr r4, [sp, #12]
	mov r2, r11
	ldr r3, [r2]
	ldr r2, [r4]
	mov r7, r11
	adds r3, r3, r2
	str r3, [r7]
	ldr r3, [r7, #4]
	ldr r2, [r4, #4]
	adds r3, r3, r2
	str r3, [r7, #4]
	ldr r3, [r7, #8]
	ldr r2, [r4, #8]
	adds r3, r3, r2
	str r3, [r7, #8]
.L_080de608:
	mov r0, r11
	bl SceneTransform_ApplyPosition
	mov r1, r10
	cmp r1, #0
	bne .L_080de62e
	ldr r2, [sp, #40]
	ldr r4, .L_080de940
	adds r3, r2, r4
	ldr r3, [r3]
	movs r2, #1
	ldr r0, [r3, #8]
	movs r3, #1
	str r1, [sp, #0]
	negs r2, r2
	movs r1, #7
	negs r3, r3
	bl ObjectGroup_UpdateMembers
.L_080de62e:
	mov r7, r10
	cmp r7, #24
	bne .L_080de64e
	ldr r1, [sp, #40]
	ldr r2, .L_080de940
	adds r3, r1, r2
	ldr r3, [r3]
	movs r2, #1
	ldr r0, [r3, #8]
	movs r3, #0
	str r3, [sp, #0]
	movs r1, #0
	negs r2, r2
	subs r3, #1
	bl ObjectGroup_UpdateMembers
.L_080de64e:
	mov r4, r10
	negs r4, r4
	mov r1, r10
	movs r3, #0
	lsls r1, r1, #8
	str r4, [sp, #8]
	ldr r6, .L_080de944
	mov r8, r3
	lsls r7, r4, #8
	mov r9, r1
.L_080de662:
	mov r3, r8
	cmp r3, #0
	bge .L_080de66a
	adds r3, #7
.L_080de66a:
	asrs r3, r3, #3
	cmp r10, r3
	bge .L_080de672
	b .L_080de7a4
.L_080de672:
	ldr r3, [r6, #24]
	cmp r3, #0
	beq .L_080de67a
	b .L_080de7a4
.L_080de67a:
	bl Graphics_SaveTransferWorkOnce
	movs r3, #3
	mov r2, r8
	ands r3, r2
	cmp r3, #1
	beq .L_080de6a4
	cmp r3, #1
	bgt .L_080de692
	cmp r3, #0
	beq .L_080de69c
	b .L_080de6c0
.L_080de692:
	cmp r3, #2
	beq .L_080de6ac
	cmp r3, #3
	beq .L_080de6b4
	b .L_080de6c0
.L_080de69c:
	mov r0, r9
	bl SceneTransform_ApplyYaw
	b .L_080de6c0
.L_080de6a4:
	adds r0, r7, #0
	bl SceneTransform_ApplyPitch
	b .L_080de6c0
.L_080de6ac:
	adds r0, r7, #0
	bl SceneTransform_ApplyRoll
	b .L_080de6c0
.L_080de6b4:
	adds r0, r7, #0
	bl SceneTransform_ApplyPitch
	adds r0, r7, #0
	bl SceneTransform_ApplyRoll
.L_080de6c0:
	add r5, sp, #48
	adds r1, r5, #0
	adds r0, r6, #0
	bl EffectPosition_ApplyBaseAndYOffset
	ldr r3, [r5]
	asrs r3, r3, #1
	str r3, [r5]
	bl Graphics_RestoreTransferWork
	ldr r2, [r5, #8]
	cmp r2, #249
	bgt .L_080de6e0
	movs r3, #250
	str r3, [r5, #8]
	movs r2, #250
.L_080de6e0:
	ldr r3, .L_080de948
	cmp r2, r3
	ble .L_080de6ea
	str r3, [r5, #8]
	adds r2, r3, #0
.L_080de6ea:
	adds r3, r2, #0
	subs r3, #250
	cmp r3, #0
	bge .L_080de6f4
	adds r3, #63
.L_080de6f4:
	asrs r3, r3, #6
	movs r0, #8
	subs r0, r0, r3
	lsls r4, r0, #1
	ldr r2, .L_080de94c
	subs r3, r4, #2
	ldrh r1, [r2, r3]
	ldr r3, [sp, #24]
	adds r1, r3, r1
	lsrs r3, r0, #31
	ldr r2, [r5]
	adds r3, r0, r3
	asrs r3, r3, #1
	subs r2, r2, r3
	ldr r3, [r5, #4]
	str r0, [sp, #0]
	subs r3, r3, r0
	str r4, [sp, #4]
	ldr r0, [sp, #36]
	ldr r4, [sp, #32]
	bl _call_via_r4
	adds r0, r6, #0
	movs r1, #60
	movs r2, #0
	bl EffectStep_AdvanceWithGravity3D
	mov r3, r8
	cmp r3, #0
	bge .L_080de732
	adds r3, #7
.L_080de732:
	asrs r3, r3, #3
	adds r3, #24
	cmp r10, r3
	blt .L_080de7a4
	ldr r3, [r6]
	negs r3, r3
	asrs r5, r3, #7
	ldr r3, [r6, #8]
	ldr r2, [r6, #4]
	negs r3, r3
	asrs r4, r3, #7
	negs r2, r2
	ldr r3, [r6, #16]
	ldr r1, [r6, #12]
	asrs r2, r2, #7
	adds r2, r3, r2
	ldr r3, [r6, #20]
	adds r1, r1, r5
	adds r0, r3, r4
	lsls r3, r1, #5
	subs r3, r3, r1
	lsls r3, r3, #1
	str r1, [r6, #12]
	str r2, [r6, #16]
	str r0, [r6, #20]
	cmp r3, #0
	bge .L_080de76a
	adds r3, #63
.L_080de76a:
	asrs r3, r3, #6
	str r3, [r6, #12]
	lsls r3, r2, #5
	subs r3, r3, r2
	lsls r3, r3, #1
	cmp r3, #0
	bge .L_080de77a
	adds r3, #63
.L_080de77a:
	asrs r3, r3, #6
	str r3, [r6, #16]
	lsls r3, r0, #5
	subs r3, r3, r0
	lsls r3, r3, #1
	cmp r3, #0
	bge .L_080de78a
	adds r3, #63
.L_080de78a:
	ldr r1, .L_080de950
	asrs r3, r3, #6
	ldr r2, .L_080de954
	str r3, [r6, #20]
	adds r3, r5, r1
	cmp r3, r2
	bhi .L_080de7a4
	adds r3, r4, r1
	cmp r3, r2
	bhi .L_080de7a4
	movs r2, #1
	negs r2, r2
	str r2, [r6, #24]
.L_080de7a4:
	ldr r4, [sp, #8]
	mov r1, r10
	lsls r3, r4, #5
	movs r2, #1
	adds r7, r7, r3
	add r8, r2
	lsls r3, r1, #5
	add r9, r3
	mov r3, r8
	adds r6, #28
	cmp r3, #32
	beq .L_080de7be
	b .L_080de662
.L_080de7be:
	mov r3, r10
	subs r3, #54
	cmp r3, #15
	bhi .L_080de80c
	lsls r0, r1, #10
	bl Trig_Sin
	movs r3, #0
	add r5, sp, #48
	add r2, sp, #60
	lsls r0, r0, #2
	str r0, [r2]
	str r3, [r2, #4]
	str r3, [r2, #8]
	adds r0, r2, #0
	adds r1, r5, #0
	bl EffectPosition_ApplyBaseAndYOffset
	ldr r3, [r5]
	ldr r4, [sp, #140]
	str r3, [r4]
	ldr r7, [sp, #144]
	ldr r3, [r5, #4]
	str r3, [r7]
	ldr r2, [r5]
	ldr r3, [r5, #4]
	asrs r2, r2, #1
	movs r1, #20
	str r2, [r5]
	str r1, [sp, #0]
	movs r1, #40
	str r1, [sp, #4]
	subs r2, #10
	subs r3, #20
	ldr r0, [sp, #36]
	ldr r1, [sp, #40]
	ldr r4, [sp, #28]
	bl _call_via_r4
.L_080de80c:
	mov r7, r10
	cmp r7, #64
	bne .L_080de876
	ldr r2, [sp, #40]
	movs r3, #225
	movs r1, #0
	lsls r3, r3, #7
	mov r8, r1
	adds r7, r2, r3
.L_080de81e:
	bl Random16
	ldr r3, .L_080de958
	adds r6, r0, #0
	ands r6, r3
	bl Random16
	ldr r4, [sp, #140]
	ldr r3, [r4]
	lsls r3, r3, #15
	str r3, [r7]
	ldr r1, [sp, #144]
	ldr r3, [r1]
	movs r5, #255
	lsls r3, r3, #16
	str r3, [r7, #4]
	ands r5, r0
	adds r0, r6, #0
	bl Trig_Sin
	adds r5, #128
	adds r3, r5, #0
	muls r3, r0
	asrs r3, r3, #6
	str r3, [r7, #12]
	adds r0, r6, #0
	bl Trig_Cos
	adds r3, r5, #0
	muls r3, r0
	asrs r3, r3, #5
	str r3, [r7, #16]
	bl Random16
	movs r3, #15
	ands r3, r0
	movs r2, #1
	adds r3, #8
	add r8, r2
	str r3, [r7, #24]
	mov r3, r8
	adds r7, #28
	cmp r3, #64
	bne .L_080de81e
.L_080de876:
	mov r4, r10
	cmp r4, #63
	ble .L_080de8d8
	ldr r1, [sp, #40]
	movs r2, #225
	movs r7, #0
	lsls r2, r2, #7
	ldr r6, .L_080de94c
	mov r8, r7
	adds r5, r1, r2
.L_080de88a:
	ldr r0, [r5, #24]
	cmp r0, #0
	blt .L_080de8cc
	asrs r0, r0, #3
	adds r0, #2
	lsls r4, r0, #1
	subs r3, r4, #2
	ldrh r1, [r6, r3]
	ldr r3, [sp, #24]
	adds r1, r3, r1
	lsrs r3, r0, #31
	movs r7, #2
	ldrsh r2, [r5, r7]
	adds r3, r0, r3
	asrs r3, r3, #1
	subs r2, r2, r3
	movs r7, #6
	ldrsh r3, [r5, r7]
	str r0, [sp, #0]
	subs r3, r3, r0
	str r4, [sp, #4]
	ldr r0, [sp, #36]
	ldr r4, [sp, #28]
	bl _call_via_r4
	adds r0, r5, #0
	movs r1, #60
	movs r2, #0
	bl EffectStep_AdvanceWithGravity2D
	ldr r3, [r5, #24]
	subs r3, #1
	str r3, [r5, #24]
.L_080de8cc:
	movs r7, #1
	add r8, r7
	mov r1, r8
	adds r5, #28
	cmp r1, #64
	bne .L_080de88a
.L_080de8d8:
	ldr r3, [sp, #40]
	ldr r4, .L_080de95c
	adds r2, r3, r4
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	movs r7, #1
	ldr r1, [sp, #20]
	add r10, r7
	cmp r10, r1
	beq .L_080de8f4
	b .L_080de59e
.L_080de8f4:
	ldr r0, .L_080de960
	bl Scheduler_RemoveCallback
	movs r0, #47
	bl Runtime_ReleaseHeapBlock
	movs r0, #46
	bl Runtime_ReleaseHeapBlock
	ldr r0, .L_080de964
	bl Scheduler_RemoveCallback
	movs r1, #128
	ldr r5, .L_080de968
	lsls r1, r1, #7
	ldr r0, .L_080de96c
	bl _call_via_r5
	movs r1, #128
	ldr r0, [sp, #36]
	lsls r1, r1, #7
	bl _call_via_r5
	ldr r2, .L_080de970
	ldr r3, .L_080de93c
	add sp, #108
	strh r3, [r2]
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
.L_080de93c:
	.4byte 0x00001010
.L_080de940:
	.4byte 0x00007828
.L_080de944:
	.4byte gMapCellBuffer
.L_080de948:
	.4byte 0x0000027a
.L_080de94c:
	.4byte ParticleStreams_CellOffsets
.L_080de950:
	.4byte 0x000007ff
.L_080de954:
	.4byte 0x00000ffe
.L_080de958:
	.4byte 0x0000ffff
.L_080de95c:
	.4byte 0x00007824
.L_080de960:
	.4byte BattlePresentation_ProcessPendingGraphicsTransfer
.L_080de964:
	.4byte Palette_StepFadeTransfer
.L_080de968:
	.4byte IwramClearWords
.L_080de96c:
	.4byte 0x06004000
.L_080de970:
	.4byte 0x04000052
