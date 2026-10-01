.syntax unified
	.thumb
	.global Unnamed_080d5e54
	.thumb_func
Unnamed_080d5e54:
	.global BattleEffect_RunSparkTravel
BattleEffect_RunSparkTravel:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r2, .L_080d60a4
	adds r3, r2, #0
	ldmia r3!, {r1}
	sub sp, #172
	str r1, [sp, #76]
	ldr r3, [r3]
	str r3, [sp, #72]
	ldr r2, [r2, #8]
	str r2, [sp, #64]
	ldr r2, .L_080d60a8
	adds r5, r1, r2
	str r0, [r5]
	movs r0, #0
	bl BattleFx_BeginCanvasLayer
	ldr r0, .L_080d60ac
	ldr r1, [sp, #76]
	movs r2, #1
	movs r3, #1
	bl Resource_LoadAndDecompress
	movs r2, #0
	ldr r1, [sp, #64]
	ldr r0, .L_080d60b0
	movs r3, #0
	bl Resource_LoadAndDecompress
	ldr r3, [r5]
	ldr r0, [r3, #4]
	movs r3, #1
	eors r0, r3
	mov r3, sp
	adds r3, #80
	adds r1, r3, #0
	str r3, [sp, #60]
	bl BattleFx_FetchRectangleBlitters
	ldr r3, [r5]
	ldr r0, [r3, #8]
	bl GetBattleObjectSlotFar
	ldr r3, [r5]
	ldr r0, [r0]
	mov r9, r0
	movs r5, #36
	ldrsh r0, [r3, r5]
	bl GetBattleObjectSlotFar
	ldr r0, [r0]
	ldr r7, .L_080d60b4
	str r0, [sp, #56]
	movs r0, #0
	mov r10, r0
	mov r8, r0
.L_080d5ece:
	bl Random16
	ldr r3, .L_080d60b8
	adds r6, r0, #0
	ands r6, r3
	bl Random16
	mov r1, r8
	str r1, [r7]
	movs r5, #255
	ands r5, r0
	bl Random16
	movs r3, #31
	ands r3, r0
	adds r3, #20
	lsls r3, r3, #16
	mov r2, r8
	str r3, [r7, #4]
	str r2, [r7, #8]
	adds r0, r6, #0
	bl Trig_Sin
	adds r5, #128
	adds r3, r5, #0
	muls r3, r0
	asrs r3, r3, #5
	str r3, [r7, #12]
	mov r3, r8
	str r3, [r7, #16]
	adds r0, r6, #0
	bl Trig_Cos
	adds r3, r5, #0
	muls r3, r0
	movs r0, #1
	add r10, r0
	asrs r3, r3, #5
	mov r5, r8
	mov r1, r10
	str r3, [r7, #20]
	str r5, [r7, #24]
	adds r7, #28
	cmp r1, #64
	bne .L_080d5ece
	ldr r3, [sp, #76]
	movs r5, #239
	lsls r5, r5, #7
	adds r2, r3, r5
	movs r3, #2
	str r3, [r2]
	ldr r0, [sp, #76]
	ldr r1, .L_080d60bc
	movs r3, #75
	adds r2, r0, r1
	movs r1, #144
	str r3, [r2]
	lsls r1, r1, #3
	ldr r0, .L_080d60c0
	bl Scheduler_AddOrUpdateCallback
	mov r5, r9
	movs r2, #160
	ldr r3, [r5, #8]
	add r2, sp
	mov r11, r2
	str r3, [r2]
	mov r0, r11
	movs r2, #0
	str r2, [r0, #4]
	ldr r3, [r5, #16]
	mov r1, sp
	str r3, [r0, #8]
	adds r1, #148
	ldr r3, [sp, #56]
	str r1, [sp, #52]
	mov r5, sp
	ldr r0, [r3, #8]
	movs r3, #180
	adds r5, #136
	lsls r3, r3, #15
	str r0, [r1]
	str r3, [r1, #4]
	str r2, [r1, #8]
	str r5, [sp, #48]
	mov r1, r11
	ldr r3, [r1]
	movs r1, #40
	subs r0, r0, r3
	bl __divsi3
	str r0, [r5]
	ldr r2, [sp, #52]
	mov r5, r11
	ldr r3, [r5, #4]
	ldr r0, [r2, #4]
	movs r1, #40
	subs r0, r0, r3
	bl __divsi3
	ldr r1, [sp, #48]
	str r0, [r1, #4]
	ldr r2, [sp, #52]
	ldr r3, [r5, #8]
	ldr r0, [r2, #8]
	movs r1, #40
	subs r0, r0, r3
	bl __divsi3
	ldr r3, [sp, #48]
	movs r5, #0
	str r0, [r3, #8]
	mov r9, r5
.L_080d5fb0:
	ldr r3, .L_080d60c4
	mov r0, r9
	ldr r5, [r3]
	cmp r0, #8
	bne .L_080d5fc0
	movs r0, #212
	bl AudioCommand_PlayFar
.L_080d5fc0:
	mov r1, r9
	cmp r1, #80
	bne .L_080d5fcc
	movs r0, #142
	bl AudioCommand_PlayFar
.L_080d5fcc:
	bl Render_ResetTransformState
	adds r1, r5, #0
	adds r1, #12
	adds r0, r5, #0
	bl Graphics_PrepareTransferInIwramWork
	mov r3, r9
	subs r3, #30
	cmp r3, #39
	bhi .L_080d6000
	ldr r5, [sp, #48]
	mov r2, r11
	ldr r3, [r2]
	ldr r2, [r5]
	mov r0, r11
	adds r3, r3, r2
	str r3, [r0]
	ldr r3, [r0, #4]
	ldr r2, [r5, #4]
	adds r3, r3, r2
	str r3, [r0, #4]
	ldr r3, [r0, #8]
	ldr r2, [r5, #8]
	adds r3, r3, r2
	str r3, [r0, #8]
.L_080d6000:
	mov r0, r11
	bl SceneTransform_ApplyPosition
	mov r1, r9
	cmp r1, #0
	bne .L_080d6026
	ldr r2, [sp, #76]
	ldr r5, .L_080d60a8
	adds r3, r2, r5
	ldr r3, [r3]
	movs r2, #1
	ldr r0, [r3, #8]
	movs r3, #1
	str r1, [sp, #0]
	negs r2, r2
	movs r1, #7
	negs r3, r3
	bl ObjectGroup_UpdateMembers
.L_080d6026:
	mov r0, r9
	cmp r0, #24
	bne .L_080d6046
	ldr r1, [sp, #76]
	ldr r2, .L_080d60a8
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
.L_080d6046:
	mov r0, r9
	negs r0, r0
	mov r1, r9
	movs r5, #0
	lsls r1, r1, #8
	str r0, [sp, #44]
	ldr r6, .L_080d60b4
	mov r10, r5
	lsls r7, r0, #8
	mov r8, r1
.L_080d605a:
	cmp r9, r10
	bgt .L_080d6060
	b .L_080d616a
.L_080d6060:
	ldr r3, [r6, #24]
	cmp r3, #0
	beq .L_080d6068
	b .L_080d616a
.L_080d6068:
	bl Graphics_SaveTransferWorkOnce
	movs r3, #3
	mov r2, r10
	ands r3, r2
	cmp r3, #1
	beq .L_080d6092
	cmp r3, #1
	bgt .L_080d6080
	cmp r3, #0
	beq .L_080d608a
	b .L_080d60d4
.L_080d6080:
	cmp r3, #2
	beq .L_080d609a
	cmp r3, #3
	beq .L_080d60c8
	b .L_080d60d4
.L_080d608a:
	mov r0, r8
	bl SceneTransform_ApplyYaw
	b .L_080d60d4
.L_080d6092:
	adds r0, r7, #0
	bl SceneTransform_ApplyPitch
	b .L_080d60d4
.L_080d609a:
	adds r0, r7, #0
	bl SceneTransform_ApplyRoll
	b .L_080d60d4
	.2byte 0x0000
.L_080d60a4:
	.4byte gBattleFxWork
.L_080d60a8:
	.4byte 0x00007828
.L_080d60ac:
	.4byte 0x00000092
.L_080d60b0:
	.4byte 0x00000073
.L_080d60b4:
	.4byte gMapCellBuffer
.L_080d60b8:
	.4byte 0x0000ffff
.L_080d60bc:
	.4byte 0x00007784
.L_080d60c0:
	.4byte BattlePresentation_ProcessPendingGraphicsTransfer
.L_080d60c4:
	.4byte gCameraWork
.L_080d60c8:
	adds r0, r7, #0
	bl SceneTransform_ApplyPitch
	adds r0, r7, #0
	bl SceneTransform_ApplyRoll
.L_080d60d4:
	add r5, sp, #112
	adds r1, r5, #0
	adds r0, r6, #0
	bl EffectPosition_ApplyBaseAndYOffset
	ldr r3, [r5]
	asrs r3, r3, #1
	str r3, [r5]
	bl Graphics_RestoreTransferWork
	ldr r2, [r5, #8]
	cmp r2, #249
	bgt .L_080d60f4
	movs r3, #250
	str r3, [r5, #8]
	movs r2, #250
.L_080d60f4:
	ldr r3, .L_080d64a0
	cmp r2, r3
	ble .L_080d60fe
	str r3, [r5, #8]
	adds r2, r3, #0
.L_080d60fe:
	adds r3, r2, #0
	subs r3, #250
	cmp r3, #0
	bge .L_080d6108
	adds r3, #63
.L_080d6108:
	asrs r3, r3, #6
	movs r0, #9
	subs r0, r0, r3
	lsls r4, r0, #1
	ldr r2, .L_080d64a4
	subs r3, r4, #2
	ldrh r1, [r2, r3]
	ldr r3, [sp, #64]
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
	ldr r0, [sp, #72]
	ldr r4, [sp, #80]
	bl _call_via_r4
	adds r0, r6, #0
	movs r1, #60
	movs r2, #0
	bl EffectStep_AdvanceWithGravity3D
	mov r3, r10
	adds r3, #30
	cmp r9, r3
	ble .L_080d616a
	ldr r2, [r6]
	ldr r3, [r6, #12]
	negs r2, r2
	asrs r2, r2, #8
	ldr r1, [r6, #4]
	adds r3, r3, r2
	negs r1, r1
	str r3, [r6, #12]
	ldr r3, [r6, #16]
	asrs r1, r1, #8
	ldr r0, [r6, #8]
	adds r3, r3, r1
	negs r0, r0
	str r3, [r6, #16]
	ldr r3, [r6, #20]
	asrs r0, r0, #8
	adds r3, r3, r0
	str r3, [r6, #20]
.L_080d616a:
	ldr r5, [sp, #44]
	movs r1, #1
	lsls r3, r5, #5
	mov r0, r9
	add r10, r1
	adds r7, r7, r3
	mov r2, r10
	lsls r3, r0, #5
	add r8, r3
	adds r6, #28
	cmp r2, #32
	beq .L_080d6184
	b .L_080d605a
.L_080d6184:
	cmp r0, #82
	ble .L_080d61c6
	add r6, sp, #124
	movs r3, #0
	str r3, [r6]
	lsls r0, r0, #10
	bl Trig_Sin
	movs r5, #0
	lsls r0, r0, #2
	str r5, [r6, #8]
	add r5, sp, #112
	str r0, [r6, #4]
	adds r1, r5, #0
	adds r0, r6, #0
	bl EffectPosition_ApplyBaseAndYOffset
	ldr r2, [r5]
	movs r1, #20
	asrs r2, r2, #1
	str r2, [r5]
	ldr r3, [r5, #4]
	str r1, [sp, #0]
	movs r1, #34
	str r1, [sp, #4]
	ldr r0, [sp, #60]
	subs r2, #10
	ldr r4, [r0, #4]
	subs r3, #17
	ldr r0, [sp, #72]
	ldr r1, [sp, #76]
	bl _call_via_r4
.L_080d61c6:
	ldr r1, [sp, #76]
	ldr r3, .L_080d64a8
	movs r5, #1
	adds r2, r1, r3
	movs r3, #1
	movs r0, #1
	str r3, [r2]
	add r9, r5
	bl WaitFrames
	mov r0, r9
	cmp r0, #98
	beq .L_080d61e2
	b .L_080d5fb0
.L_080d61e2:
	movs r1, #0
	ldr r5, .L_080d64ac
	mov r10, r1
	movs r6, #255
.L_080d61ea:
	bl Random16
	ands r0, r6
	subs r0, #127
	lsls r0, r0, #15
	str r0, [r5]
	bl Random16
	movs r3, #127
	ands r3, r0
	adds r3, #64
	lsls r3, r3, #15
	str r3, [r5, #4]
	bl Random16
	movs r3, #0
	ands r0, r6
	str r3, [r5, #24]
	subs r0, #127
	movs r2, #1
	movs r3, #128
	lsls r0, r0, #15
	add r10, r2
	lsls r3, r3, #2
	str r0, [r5, #8]
	adds r5, #28
	cmp r10, r3
	bne .L_080d61ea
	ldr r0, .L_080d64b0
	ldr r1, [sp, #64]
	movs r2, #0
	movs r3, #0
	bl Resource_LoadAndDecompress
	ldr r1, .L_080d64b4
	ldr r0, [sp, #76]
	adds r3, r0, r1
	ldr r3, [r3]
	ldr r3, [r3, #20]
	movs r2, #72
	movs r5, #0
	lsls r3, r3, #3
	negs r2, r2
	mov r9, r5
	cmp r3, r2
	bne .L_080d6248
	b .L_080d64d6
.L_080d6248:
	mov r3, sp
	mov r5, sp
	ldr r2, [sp, #52]
	adds r3, #100
	adds r5, #88
	adds r1, r0, r1
	str r3, [sp, #16]
	str r5, [sp, #20]
	str r1, [sp, #32]
	str r2, [sp, #28]
.L_080d625c:
	ldr r3, .L_080d64b8
	ldr r3, [r3]
	str r3, [sp, #40]
	bl Render_ResetTransformState
	ldr r3, [sp, #40]
	adds r3, #12
	adds r1, r3, #0
	ldr r0, [sp, #40]
	str r3, [sp, #36]
	bl Graphics_PrepareTransferInIwramWork
	ldr r5, [sp, #32]
	ldr r3, [r5]
	ldr r3, [r3, #20]
	lsls r3, r3, #3
	adds r3, #40
	cmp r9, r3
	blt .L_080d628e
	ldr r0, [sp, #28]
	movs r1, #128
	ldr r3, [r0, #4]
	lsls r1, r1, #11
	adds r3, r3, r1
	str r3, [r0, #4]
.L_080d628e:
	ldr r2, [sp, #28]
	ldr r5, [sp, #16]
	ldr r3, [r2]
	str r3, [r5]
	ldr r3, [r2, #4]
	mov r1, r9
	str r3, [r5, #4]
	lsls r0, r1, #11
	bl Trig_Sin
	ldr r5, [sp, #28]
	lsls r2, r0, #2
	ldr r3, [r5, #8]
	adds r2, r2, r0
	lsls r2, r2, #3
	ldr r0, [sp, #16]
	adds r3, r3, r2
	str r3, [r0, #8]
	add r2, sp, #88
	adds r1, r2, #0
	ldr r0, [sp, #16]
	bl EffectPosition_ApplyBaseAndYOffset
	ldr r2, [sp, #88]
	ldr r3, [sp, #92]
	movs r1, #20
	asrs r2, r2, #1
	str r1, [sp, #0]
	movs r1, #34
	str r2, [sp, #88]
	subs r3, #17
	str r1, [sp, #4]
	subs r2, #10
	ldr r4, [sp, #80]
	ldr r0, [sp, #72]
	ldr r1, [sp, #76]
	bl _call_via_r4
	movs r3, #0
	str r3, [sp, #68]
	ldr r5, [sp, #32]
	ldr r3, [r5]
	ldr r3, [r3, #20]
	cmp r3, #0
	bne .L_080d62ea
	b .L_080d6484
.L_080d62ea:
	ldr r1, .L_080d64b4
	ldr r0, [sp, #76]
	movs r2, #36
	adds r1, r0, r1
	movs r3, #0
	str r1, [sp, #24]
	str r2, [sp, #12]
	str r3, [sp, #8]
.L_080d62fa:
	ldr r5, [sp, #24]
	ldr r1, [sp, #12]
	ldr r3, [r5]
	ldrsh r0, [r3, r1]
	bl GetBattleObjectSlotFar
	ldr r3, [sp, #68]
	lsls r3, r3, #3
	ldr r5, [r0]
	mov r8, r3
	bl Render_ResetTransformState
	ldr r0, [sp, #40]
	ldr r1, [sp, #36]
	bl Graphics_PrepareTransferInIwramWork
	ldr r3, [r5, #8]
	mov r0, r11
	str r3, [r0]
	movs r3, #160
	lsls r3, r3, #14
	str r3, [r0, #4]
	ldr r3, [r5, #16]
	str r3, [r0, #8]
	bl SceneTransform_ApplyPosition
	mov r3, r8
	adds r3, #30
	cmp r9, r3
	bne .L_080d633c
	movs r0, #126
	bl AudioCommand_PlayFar
.L_080d633c:
	mov r3, r8
	adds r3, #40
	cmp r9, r3
	bne .L_080d635c
	ldr r1, [sp, #24]
	ldr r2, [sp, #12]
	ldr r3, [r1]
	ldrsh r0, [r3, r2]
	movs r3, #0
	movs r2, #1
	str r3, [sp, #0]
	movs r1, #7
	negs r2, r2
	subs r3, #1
	bl ObjectGroup_UpdateMembers
.L_080d635c:
	mov r3, r8
	adds r3, #64
	cmp r9, r3
	bne .L_080d637c
	ldr r0, [sp, #24]
	ldr r1, [sp, #12]
	ldr r3, [r0]
	ldrsh r0, [r3, r1]
	movs r3, #0
	movs r2, #1
	str r3, [sp, #0]
	movs r1, #0
	negs r2, r2
	subs r3, #1
	bl ObjectGroup_UpdateMembers
.L_080d637c:
	cmp r9, r8
	ble .L_080d6462
	mov r3, r9
	mov r5, r8
	subs r0, r3, r5
	lsls r0, r0, #9
	bl SceneTransform_ApplyYaw
	ldr r2, .L_080d64ac
	ldr r1, [sp, #8]
	movs r0, #0
	mov r10, r0
	adds r6, r1, r2
.L_080d6396:
	mov r5, r10
	lsrs r3, r5, #31
	add r3, r10
	asrs r3, r3, #1
	add r3, r8
	cmp r9, r3
	ble .L_080d6456
	ldr r3, [r6]
	asrs r3, r3, #8
	adds r0, r3, #0
	muls r0, r3
	ldr r3, [r6, #4]
	asrs r3, r3, #8
	adds r2, r3, #0
	muls r2, r3
	ldr r3, [r6, #8]
	asrs r3, r3, #8
	adds r1, r3, #0
	muls r1, r3
	adds r0, r0, r2
	adds r3, r1, #0
	adds r0, r0, r3
	ldr r3, .L_080d64bc
	bl _call_via_r3
	asrs r7, r0, #9
	cmp r7, #0
	beq .L_080d6456
	add r2, sp, #88
	adds r0, r6, #0
	adds r1, r2, #0
	bl EffectPosition_ApplyBaseAndYOffset
	ldr r3, [sp, #88]
	asrs r3, r3, #1
	str r3, [sp, #88]
	ldr r5, .L_080d64c0
	ldr r3, [sp, #96]
	cmp r3, r5
	bgt .L_080d63ee
	movs r3, #157
	ldr r0, [sp, #20]
	lsls r3, r3, #1
	str r3, [r0, #8]
.L_080d63ee:
	ldr r2, .L_080d64a0
	cmp r3, r2
	ble .L_080d63f8
	ldr r1, [sp, #20]
	str r2, [r1, #8]
.L_080d63f8:
	mov r3, r10
	lsls r0, r3, #2
	movs r1, #9
	add r0, r9
	bl __modsi3
	ldr r2, .L_080d64c4
	lsls r3, r0, #1
	ldrh r1, [r2, r3]
	ldr r3, .L_080d64c8
	ldr r5, [sp, #64]
	ldrb r0, [r3, r0]
	ldr r3, [sp, #20]
	adds r1, r5, r1
	ldr r2, [r3]
	ldr r5, [sp, #60]
	ldr r3, [r3, #4]
	str r0, [sp, #0]
	str r0, [sp, #4]
	lsrs r4, r0, #1
	subs r2, r2, r4
	subs r3, r3, r4
	ldr r0, [sp, #72]
	ldr r4, [r5, #4]
	bl _call_via_r4
	ldr r5, [r6]
	adds r1, r7, #0
	adds r0, r5, #0
	bl __divsi3
	subs r5, r5, r0
	str r5, [r6]
	ldr r5, [r6, #4]
	adds r1, r7, #0
	adds r0, r5, #0
	bl __divsi3
	subs r5, r5, r0
	str r5, [r6, #4]
	ldr r5, [r6, #8]
	adds r1, r7, #0
	adds r0, r5, #0
	bl __divsi3
	subs r5, r5, r0
	str r5, [r6, #8]
.L_080d6456:
	movs r0, #1
	add r10, r0
	mov r1, r10
	adds r6, #28
	cmp r1, #64
	bne .L_080d6396
.L_080d6462:
	ldr r2, [sp, #12]
	ldr r3, [sp, #8]
	ldr r0, [sp, #68]
	movs r5, #224
	lsls r5, r5, #3
	adds r3, r3, r5
	adds r2, #2
	adds r0, #1
	str r2, [sp, #12]
	str r3, [sp, #8]
	str r0, [sp, #68]
	ldr r1, [sp, #24]
	ldr r3, [r1]
	ldr r3, [r3, #20]
	cmp r0, r3
	beq .L_080d6484
	b .L_080d62fa
.L_080d6484:
	ldr r3, [sp, #76]
	ldr r5, .L_080d64a8
	adds r2, r3, r5
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r1, [sp, #32]
	ldr r3, [r1]
	ldr r3, [r3, #20]
	movs r0, #1
	lsls r3, r3, #3
	b .L_080d64cc
.L_080d64a0:
	.4byte 0x0000027a
.L_080d64a4:
	.4byte ParticleStreams_CellOffsets
.L_080d64a8:
	.4byte 0x00007824
.L_080d64ac:
	.4byte gMapCellBuffer
.L_080d64b0:
	.4byte 0x000000ba
.L_080d64b4:
	.4byte 0x00007828
.L_080d64b8:
	.4byte gCameraWork
.L_080d64bc:
	.4byte IwramSqrt
.L_080d64c0:
	.4byte 0x00000139
.L_080d64c4:
	.4byte BattleFx_PuffCells
.L_080d64c8:
	.4byte BattleFx_PuffSizes
.L_080d64cc:
	add r9, r0
	adds r3, #72
	cmp r9, r3
	beq .L_080d64d6
	b .L_080d625c
.L_080d64d6:
	ldr r0, .L_080d6500
	bl Scheduler_RemoveCallback
	movs r0, #47
	bl Runtime_ReleaseHeapBlock
	movs r0, #46
	bl Runtime_ReleaseHeapBlock
	bl BattleFx_EndCanvasLayer
	add sp, #172
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
.L_080d6500:
	.4byte BattlePresentation_ProcessPendingGraphicsTransfer
