.syntax unified
	.thumb
	.global BattleFx_RunMemberBeam
	.thumb_func
BattleFx_RunMemberBeam:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, .L_080cf3cc
	adds r6, r0, #0
	ldmia r3!, {r0}
	ldr r5, .L_080cf3d0
	ldr r3, [r3]
	mov r11, r0
	sub sp, #116
	add r5, r11
	str r3, [sp, #48]
	movs r0, #0
	str r6, [r5]
	adds r7, r1, #0
	bl BattleFx_BeginCanvasLayer
	ldr r5, [r5]
	ldr r3, [r5, #28]
	cmp r3, #1
	bne .L_080cf300
	ldr r2, [r5, #4]
	eors r2, r3
	add r3, sp, #64
	str r3, [sp, #0]
	add r3, sp, #60
	str r3, [sp, #4]
	adds r0, r6, #0
	adds r1, r7, #0
	movs r3, #0
	bl BattleFx_PrepareCanvasEffect
.L_080cf300:
	ldr r0, .L_080cf3d4
	bl Resource_GetTableEntry
	adds r6, r0, #0
	movs r0, #160
	movs r2, #128
	adds r1, r6, #0
	ldr r5, .L_080cf3d8
	adds r6, #128
	lsls r0, r0, #19
	bl _call_via_r5
	mov r1, r11
	adds r0, r6, #0
	bl Resource_DecodeType01
	ldr r0, .L_080cf3dc
	bl Resource_GetTableEntry
	adds r6, r0, #0
	movs r0, #160
	adds r1, r6, #0
	lsls r0, r0, #19
	movs r2, #128
	bl _call_via_r5
	ldr r1, .L_080cf3e0
	adds r6, #128
	add r1, r11
	adds r0, r6, #0
	bl Resource_DecodeType01
	cmp r7, #0
	bne .L_080cf348
	ldr r0, .L_080cf3e4
	b .L_080cf34a
.L_080cf348:
	ldr r0, .L_080cf3e8
.L_080cf34a:
	bl Resource_GetTableEntry
	adds r6, r0, #0
	movs r0, #160
	adds r1, r6, #0
	ldr r3, .L_080cf3d8
	movs r2, #128
	lsls r0, r0, #19
	bl _call_via_r3
	ldr r1, .L_080cf3ec
	adds r6, #128
	add r1, r11
	adds r0, r6, #0
	bl Resource_DecodeType01
	ldr r5, .L_080cf3f0
	movs r1, #0
	mov r9, r1
.L_080cf370:
	movs r3, #0
	str r3, [r5]
	movs r3, #160
	lsls r3, r3, #15
	str r3, [r5, #4]
	bl Random16
	movs r3, #32
	negs r3, r3
	orrs r3, r0
	lsls r3, r3, #14
	str r3, [r5, #8]
	bl Random16
	movs r3, #255
	ands r3, r0
	str r3, [r5, #24]
	movs r2, #1
	movs r3, #128
	add r9, r2
	lsls r3, r3, #2
	adds r5, #28
	cmp r9, r3
	bne .L_080cf370
	ldr r2, .L_080cf3f4
	ldr r3, .L_080cf3c8
	strh r3, [r2]
	ldr r3, .L_080cf3d0
	add r3, r11
	ldr r2, [r3]
	ldr r3, [r2, #20]
	cmp r3, #1
	bne .L_080cf3f8
	movs r5, #36
	ldrsh r0, [r2, r5]
	add r5, sp, #104
	adds r1, r5, #0
	bl EffectPosition_ApplyStepAndYOffset
	ldr r2, [r5]
	movs r3, #64
	subs r3, r3, r2
	str r3, [sp, #40]
	b .L_080cf408
.L_080cf3c8:
	.4byte 0x00000100
.L_080cf3cc:
	.4byte gBattleFxWork
.L_080cf3d0:
	.4byte 0x00007828
.L_080cf3d4:
	.4byte 0x0000007b
.L_080cf3d8:
	.4byte IwramCopyWords
.L_080cf3dc:
	.4byte 0x000000b1
.L_080cf3e0:
	.4byte 0x00002710
.L_080cf3e4:
	.4byte 0x00000093
.L_080cf3e8:
	.4byte 0x00000091
.L_080cf3ec:
	.4byte 0x000065c0
.L_080cf3f0:
	.4byte gMapCellBuffer
.L_080cf3f4:
	.4byte 0x04000020
.L_080cf3f8:
	ldr r3, [r2, #4]
	movs r0, #112
	negs r0, r0
	str r0, [sp, #40]
	cmp r3, #1
	beq .L_080cf408
	movs r1, #0
	str r1, [sp, #40]
.L_080cf408:
	ldr r5, [sp, #40]
	ldr r2, .L_080cf5dc
	lsls r3, r5, #8
	str r3, [r2]
	movs r2, #239
	lsls r2, r2, #7
	add r2, r11
	movs r3, #2
	str r3, [r2]
	ldr r2, .L_080cf5e0
	movs r3, #50
	add r2, r11
	movs r1, #144
	str r3, [r2]
	lsls r1, r1, #3
	ldr r0, .L_080cf5e4
	bl Scheduler_AddOrUpdateCallback
	movs r0, #0
	ldr r3, .L_080cf5e8
	str r0, [sp, #44]
	add r3, r11
	ldr r3, [r3]
	ldr r3, [r3, #20]
	movs r1, #116
	lsls r3, r3, #4
	negs r1, r1
	cmp r3, r1
	bne .L_080cf444
	b .L_080cf84c
.L_080cf444:
	mov r2, sp
	adds r2, #52
	str r2, [sp, #28]
.L_080cf44a:
	ldr r3, .L_080cf5ec
	ldr r3, [r3]
	str r3, [sp, #36]
	ldr r3, [sp, #44]
	cmp r3, #64
	bne .L_080cf45c
	movs r0, #212
	bl AudioCommand_PlayFar
.L_080cf45c:
	ldr r5, [sp, #44]
	cmp r5, #80
	bne .L_080cf468
	movs r0, #0
	bl BattleEventRuntime_BeginPhaseFar
.L_080cf468:
	ldr r7, .L_080cf5e8
	add r7, r11
	ldr r3, [r7]
	ldr r3, [r3, #28]
	mov r8, r3
	cmp r3, #1
	bne .L_080cf504
	ldr r0, [sp, #44]
	lsls r5, r0, #11
	adds r0, r5, #0
	bl Trig_Sin
	negs r0, r0
	lsls r3, r0, #2
	adds r3, r3, r0
	ldr r2, [sp, #64]
	lsls r3, r3, #2
	ldr r1, [sp, #40]
	asrs r3, r3, #16
	adds r3, r3, r2
	adds r3, r3, r1
	subs r3, #20
	adds r0, r5, #0
	mov r10, r3
	bl Trig_Cos
	ldr r3, [sp, #60]
	lsls r0, r0, #2
	asrs r0, r0, #16
	adds r0, r0, r3
	ldr r3, [r7]
	adds r6, r0, #0
	ldr r0, [r3, #4]
	mov r2, r8
	eors r0, r2
	ldr r1, [sp, #28]
	bl BattleFx_FetchRectangleBlitters
	ldr r3, [sp, #44]
	subs r6, #24
	cmp r3, #32
	ble .L_080cf4c4
	lsls r3, r3, #1
	subs r3, r6, r3
	adds r6, r3, #0
	adds r6, #64
.L_080cf4c4:
	movs r5, #40
	ldr r7, .L_080cf5f0
	ldr r0, [sp, #28]
	str r5, [sp, #0]
	str r5, [sp, #4]
	add r7, r11
	ldr r4, [r0, #4]
	adds r1, r7, #0
	ldr r0, [sp, #48]
	mov r2, r10
	adds r3, r6, #0
	bl _call_via_r4
	ldr r1, [sp, #44]
	cmp r1, #3
	bgt .L_080cf4f8
	ldr r2, [sp, #28]
	str r5, [sp, #0]
	str r5, [sp, #4]
	ldr r0, [sp, #48]
	ldr r4, [r2, #4]
	adds r1, r7, #0
	mov r2, r10
	adds r3, r6, #0
	bl _call_via_r4
.L_080cf4f8:
	movs r0, #47
	bl Runtime_ReleaseHeapBlock
	movs r0, #46
	bl Runtime_ReleaseHeapBlock
.L_080cf504:
	movs r6, #2
	movs r1, #7
	movs r2, #7
	movs r3, #3
	movs r0, #46
	str r6, [sp, #0]
	bl Unnamed_080ed408
	ldr r5, .L_080cf5f4
	adds r3, r5, #0
	adds r3, #184
	ldr r3, [r3]
	movs r1, #7
	str r3, [sp, #52]
	movs r2, #7
	movs r3, #7
	movs r0, #47
	str r6, [sp, #0]
	bl Unnamed_080ed408
	adds r5, #188
	ldr r3, [r5]
	ldr r2, .L_080cf5e8
	ldr r5, [sp, #28]
	mov r1, r11
	str r3, [r5, #4]
	ldr r3, [r1, r2]
	ldr r3, [r3, #20]
	movs r0, #0
	mov r8, r0
	cmp r3, #0
	bne .L_080cf546
	b .L_080cf678
.L_080cf546:
	ldr r3, [sp, #36]
	adds r3, #12
	str r3, [sp, #32]
	str r0, [sp, #16]
	add r7, sp, #68
.L_080cf550:
	mov r5, r11
	mov r0, r8
	ldr r2, [r5, r2]
	lsls r3, r0, #1
	adds r3, #36
	ldrsh r0, [r2, r3]
	bl GetBattleObjectSlotFar
	mov r2, r8
	lsls r2, r2, #4
	ldr r5, [r0]
	mov r10, r2
	bl Render_ResetTransformState
	ldr r0, [sp, #36]
	ldr r1, [sp, #32]
	bl Graphics_PrepareTransferInIwramWork
	ldr r3, [r5, #8]
	str r3, [r7]
	movs r3, #0
	str r3, [r7, #4]
	ldr r3, [r5, #16]
	adds r0, r7, #0
	str r3, [r7, #8]
	bl SceneTransform_ApplyPosition
	ldr r3, [sp, #44]
	cmp r3, r10
	ble .L_080cf65c
	ldr r0, [sp, #16]
	ldr r1, .L_080cf5f8
	movs r5, #0
	mov r9, r5
	adds r6, r0, r1
.L_080cf596:
	mov r2, r9
	lsls r3, r2, #3
	ldr r5, [sp, #44]
	add r3, r10
	cmp r5, r3
	ble .L_080cf650
	movs r0, #128
	ldr r3, [r6, #4]
	lsls r0, r0, #12
	cmp r3, r0
	ble .L_080cf650
	add r5, sp, #92
	adds r1, r5, #0
	adds r0, r6, #0
	bl EffectPosition_ApplyBaseAndYOffset
	ldr r3, [r5]
	ldr r1, [sp, #40]
	adds r3, r3, r1
	str r3, [r5]
	ldr r0, [r6, #24]
	lsls r0, r0, #10
	bl Trig_Sin
	movs r3, #1
	mov r2, r9
	lsls r0, r0, #4
	ands r3, r2
	asrs r0, r0, #16
	cmp r3, #0
	beq .L_080cf5fc
	ldr r3, [r5]
	subs r3, r3, r0
	b .L_080cf600
	.2byte 0x0000
.L_080cf5dc:
	.4byte 0x04000028
.L_080cf5e0:
	.4byte 0x00007784
.L_080cf5e4:
	.4byte BattlePresentation_ProcessPendingGraphicsTransfer
.L_080cf5e8:
	.4byte 0x00007828
.L_080cf5ec:
	.4byte gCameraWork
.L_080cf5f0:
	.4byte 0x000065c0
.L_080cf5f4:
	.4byte gWorkSlot
.L_080cf5f8:
	.4byte gMapCellBuffer
.L_080cf5fc:
	ldr r3, [r5]
	adds r3, r3, r0
.L_080cf600:
	str r3, [r5]
	movs r3, #1
	mov r0, r9
	ldr r1, [sp, #28]
	ands r3, r0
	ldr r2, [r6, #24]
	lsls r3, r3, #2
	adds r4, r3, r1
	ldr r1, .L_080cf868
	cmp r2, #0
	bge .L_080cf618
	adds r2, #7
.L_080cf618:
	movs r3, #7
	asrs r2, r2, #3
	ands r2, r3
	ldrb r3, [r1, r2]
	lsls r1, r3, #3
	adds r1, r1, r3
	ldr r2, .L_080cf86c
	lsls r1, r1, #6
	movs r0, #24
	add r1, r11
	ldr r3, [r5, #4]
	adds r1, r1, r2
	ldr r2, [r5]
	str r0, [sp, #0]
	str r0, [sp, #4]
	subs r3, #12
	subs r2, #12
	ldr r4, [r4]
	ldr r0, [sp, #48]
	bl _call_via_r4
	ldr r5, .L_080cf870
	ldr r3, [r6, #4]
	adds r3, r3, r5
	str r3, [r6, #4]
	ldr r3, [r6, #24]
	adds r3, #1
	str r3, [r6, #24]
.L_080cf650:
	movs r0, #1
	add r9, r0
	mov r1, r9
	adds r6, #28
	cmp r1, #8
	bne .L_080cf596
.L_080cf65c:
	ldr r2, [sp, #16]
	movs r3, #224
	lsls r3, r3, #3
	adds r2, r2, r3
	str r2, [sp, #16]
	ldr r2, .L_080cf874
	mov r0, r11
	ldr r3, [r0, r2]
	movs r5, #1
	ldr r3, [r3, #20]
	add r8, r5
	cmp r8, r3
	beq .L_080cf678
	b .L_080cf550
.L_080cf678:
	ldr r2, .L_080cf874
	mov r5, r11
	ldr r3, [r5, r2]
	ldr r3, [r3, #20]
	movs r1, #0
	mov r9, r1
	cmp r3, #0
	bne .L_080cf68a
	b .L_080cf81a
.L_080cf68a:
	movs r0, #36
	movs r1, #72
	str r0, [sp, #20]
	str r1, [sp, #24]
.L_080cf692:
	mov r3, r9
	ldr r5, [sp, #44]
	ldr r0, [sp, #24]
	lsls r3, r3, #4
	mov r8, r3
	cmp r5, r0
	bge .L_080cf6a2
	b .L_080cf7fc
.L_080cf6a2:
	mov r1, r11
	adds r5, r1, r2
	ldr r3, [r5]
	ldr r2, [sp, #20]
	ldrsh r0, [r3, r2]
	bl GetBattleObjectSlotFar
	ldr r6, [r0]
	bl Render_ResetTransformState
	ldr r0, [sp, #36]
	adds r1, r0, #0
	adds r1, #12
	bl Graphics_PrepareTransferInIwramWork
	ldr r2, [sp, #44]
	ldr r3, [sp, #24]
	cmp r2, r3
	bne .L_080cf6de
	ldr r3, [r5]
	ldr r1, [sp, #20]
	ldrsh r0, [r3, r1]
	movs r3, #0
	movs r2, #1
	str r3, [sp, #0]
	movs r1, #1
	negs r2, r2
	subs r3, #1
	bl ObjectGroup_UpdateMembers
.L_080cf6de:
	ldr r4, [sp, #24]
	ldr r3, [sp, #44]
	adds r4, #16
	cmp r3, r4
	bne .L_080cf702
	ldr r3, [r5]
	ldr r5, [sp, #20]
	movs r2, #1
	ldrsh r0, [r3, r5]
	movs r3, #0
	str r3, [sp, #0]
	movs r1, #0
	negs r2, r2
	subs r3, #1
	str r4, [sp, #8]
	bl ObjectGroup_UpdateMembers
	ldr r4, [sp, #8]
.L_080cf702:
	ldr r3, [r6, #8]
	add r0, sp, #80
	str r3, [r0]
	movs r3, #0
	str r3, [r0, #4]
	ldr r3, [r6, #16]
	add r7, sp, #92
	str r3, [r0, #8]
	adds r1, r7, #0
	str r4, [sp, #8]
	bl EffectPosition_ApplyBaseAndYOffset
	ldr r3, [r7]
	ldr r5, [sp, #40]
	adds r2, r3, r5
	str r2, [r7]
	mov r3, r8
	ldr r0, [sp, #44]
	adds r3, #104
	ldr r4, [sp, #8]
	cmp r0, r3
	bge .L_080cf7fc
	adds r3, r0, #0
	cmp r0, #0
	bge .L_080cf736
	adds r3, #3
.L_080cf736:
	asrs r6, r3, #2
	ldr r3, [sp, #44]
	movs r1, #6
	mov r10, r1
	cmp r3, r4
	ble .L_080cf758
	mov r5, r8
	subs r0, r3, r5
	movs r1, #3
	subs r0, #88
	str r2, [sp, #12]
	bl __divsi3
	mov r1, r10
	subs r1, r1, r0
	ldr r2, [sp, #12]
	mov r10, r1
.L_080cf758:
	cmp r6, #2
	ble .L_080cf762
	movs r3, #1
	ands r3, r6
	adds r6, r3, #1
.L_080cf762:
	mov r3, r8
	ldr r5, [sp, #44]
	adds r3, #100
	cmp r5, r3
	bge .L_080cf7b4
	ldr r3, .L_080cf878
	ldr r0, .L_080cf87c
	ldrb r4, [r3, r6]
	ldr r3, .L_080cf880
	lsls r5, r6, #1
	ldrh r1, [r0, r5]
	ldrb r0, [r3, r6]
	ldr r3, [r7, #4]
	subs r3, r3, r0
	subs r2, r2, r4
	add r1, r11
	adds r3, #8
	str r4, [sp, #0]
	str r0, [sp, #4]
	ldr r4, [sp, #52]
	ldr r0, [sp, #48]
	bl _call_via_r4
	ldr r0, .L_080cf87c
	ldr r3, .L_080cf880
	ldrh r1, [r0, r5]
	ldr r5, .L_080cf878
	ldrb r4, [r3, r6]
	ldrb r0, [r5, r6]
	ldr r3, [r7, #4]
	ldr r2, [r7]
	str r0, [sp, #0]
	str r4, [sp, #4]
	ldr r0, [sp, #28]
	subs r3, r3, r4
	add r1, r11
	ldr r4, [r0, #4]
	adds r3, #8
	ldr r0, [sp, #48]
	bl _call_via_r4
.L_080cf7b4:
	ldr r3, [r7, #4]
	movs r1, #0
	mov r8, r1
	cmp r3, #0
	beq .L_080cf7fc
	mov r5, r11
	adds r6, r7, #0
	adds r5, #5
	movs r7, #1
.L_080cf7c6:
	ldr r2, [r6]
	mov r3, r10
	subs r2, r2, r3
	str r3, [sp, #0]
	ldr r4, [sp, #52]
	mov r3, r8
	str r7, [sp, #4]
	ldr r0, [sp, #48]
	adds r1, r5, #0
	bl _call_via_r4
	mov r0, r10
	ldr r2, [r6]
	ldr r1, [sp, #28]
	str r0, [sp, #0]
	str r7, [sp, #4]
	mov r3, r8
	ldr r4, [r1, #4]
	ldr r0, [sp, #48]
	adds r1, r5, #0
	bl _call_via_r4
	movs r2, #1
	ldr r3, [r6, #4]
	add r8, r2
	cmp r8, r3
	bne .L_080cf7c6
.L_080cf7fc:
	ldr r3, [sp, #20]
	ldr r5, [sp, #24]
	adds r3, #2
	adds r5, #16
	str r3, [sp, #20]
	str r5, [sp, #24]
	ldr r2, .L_080cf874
	mov r1, r11
	ldr r3, [r1, r2]
	movs r0, #1
	ldr r3, [r3, #20]
	add r9, r0
	cmp r9, r3
	beq .L_080cf81a
	b .L_080cf692
.L_080cf81a:
	movs r0, #47
	bl Runtime_ReleaseHeapBlock
	movs r0, #46
	bl Runtime_ReleaseHeapBlock
	ldr r2, .L_080cf884
	movs r3, #1
	add r2, r11
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r2, [sp, #44]
	ldr r3, .L_080cf874
	adds r2, #1
	str r2, [sp, #44]
	add r3, r11
	ldr r3, [r3]
	ldr r3, [r3, #20]
	lsls r3, r3, #4
	adds r3, #116
	cmp r2, r3
	beq .L_080cf84c
	b .L_080cf44a
.L_080cf84c:
	ldr r0, .L_080cf888
	bl Scheduler_RemoveCallback
	bl BattleFx_EndCanvasLayer
	add sp, #116
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
.L_080cf868:
	.4byte Data_080ee0a2
.L_080cf86c:
	.4byte 0x00002710
.L_080cf870:
	.4byte 0xffff0000
.L_080cf874:
	.4byte 0x00007828
.L_080cf878:
	.4byte Data_080ee0b0
.L_080cf87c:
	.4byte Data_080ee0aa
.L_080cf880:
	.4byte Data_080ee0b3
.L_080cf884:
	.4byte 0x00007824
.L_080cf888:
	.4byte BattlePresentation_ProcessPendingGraphicsTransfer
