.syntax unified
	.thumb
	.section .text.x0200b1ac,"ax",%progbits
	.global Scene_RunActorGroupDepartureSequence
	.thumb_func
Scene_RunActorGroupDepartureSequence:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r0, #19
	sub sp, #4
	bl Object_GetById
	adds r7, r0, #0
	movs r0, #27
	bl Object_GetById
	adds r6, r0, #0
	ldr r1, [r6, #80]
	ldr r0, [r7, #80]
	mov r11, r1
	mov r10, r0
	movs r1, #128
	movs r0, #128
	lsls r0, r0, #9
	lsls r1, r1, #6
	bl Engine_CameraSetSpeed
	movs r0, #220
	movs r1, #1
	movs r3, #1
	lsls r0, r0, #15
	negs r1, r1
	ldr r2, .L_0200b398
	bl Engine_CameraMoveTo
	movs r0, #8
	ldr r1, .L_0200b39c
	ldr r2, .L_0200b3a0
	bl Engine_ActorSetSpeed
	movs r0, #26
	ldr r1, .L_0200b39c
	ldr r2, .L_0200b3a0
	bl Engine_ActorSetSpeed
	movs r0, #0
	ldr r1, .L_0200b39c
	ldr r2, .L_0200b3a0
	bl Engine_ActorSetSpeed
	ldr r2, .L_0200b3a0
	movs r0, #22
	ldr r1, .L_0200b39c
	bl Engine_ActorSetSpeed
	ldr r5, .L_0200b3a4
	movs r0, #8
	adds r1, r5, #0
	bl Engine_ActorEnableActionCallback
	movs r0, #10
	bl Engine_EventWait
	adds r1, r5, #0
	movs r0, #26
	bl Engine_ActorEnableActionCallback
	bl BattleFx_SetBlock30Values12Zero
	movs r0, #10
	bl Engine_EventWait
	adds r1, r5, #0
	movs r0, #0
	bl Engine_ActorEnableActionCallback
	movs r0, #10
	bl Engine_EventWait
	bl BattleFx_SetBlock30ValuesMaxZero
	adds r1, r5, #0
	movs r0, #22
	bl Engine_ActorEnableActionCallback
	movs r0, #128
	bl Engine_EventWait
	bl SceneState_SetFlag210AndConfigureRegion40_89
	movs r0, #174
	movs r1, #1
	negs r1, r1
	ldr r2, .L_0200b3a8
	movs r3, #1
	lsls r0, r0, #16
	bl Engine_CameraMoveTo
	movs r0, #104
	bl Engine_EventWait
	movs r0, #153
	movs r1, #1
	movs r3, #1
	lsls r0, r0, #16
	negs r1, r1
	ldr r2, .L_0200b3ac
	bl Engine_CameraMoveTo
	movs r2, #159
	movs r0, #9
	movs r1, #158
	lsls r2, r2, #3
	bl Engine_ActorWalkToAndWait
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #6
	movs r0, #9
	bl Engine_ActorFaceDirection
	movs r0, #8
	bl Object_RefreshSelectorById
	ldr r1, .L_0200b3b0
	movs r0, #8
	bl Engine_ActorEnableActionCallback
	ldr r1, .L_0200b3b4
	movs r0, #26
	bl Engine_ActorEnableActionCallback
	ldr r1, .L_0200b3b8
	movs r0, #0
	bl Engine_ActorEnableActionCallback
	ldr r1, .L_0200b3bc
	movs r0, #22
	bl Object_SetActionCallbackAndRefreshById
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #11
	lsls r2, r2, #9
	lsls r0, r0, #11
	bl Engine_WorkSetValuesIfNonNegative
	movs r0, #145
	bl Engine_AudioPlayCue
	movs r0, #20
	bl Engine_EventWait
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #9
	lsls r2, r2, #9
	lsls r0, r0, #9
	bl Engine_WorkSetValuesIfNonNegative
	movs r0, #60
	bl Engine_EventWait
	movs r0, #0
	ldr r1, .L_0200b3c0
	movs r2, #0
	bl Engine_ActorShowEmote
	movs r0, #26
	ldr r1, .L_0200b3c0
	movs r2, #0
	bl Engine_ActorShowEmote
	movs r0, #22
	ldr r1, .L_0200b3c0
	movs r2, #0
	bl Engine_ActorShowEmote
	movs r0, #8
	ldr r1, .L_0200b3c0
	movs r2, #0
	bl Engine_ActorShowEmote
	movs r0, #9
	ldr r1, .L_0200b3c0
	movs r2, #60
	bl Engine_ActorShowEmote
	movs r0, #26
	movs r1, #8
	movs r2, #0
	bl Engine_ActorFaceEachOther
	movs r2, #0
	movs r1, #0
	movs r0, #22
	bl Engine_ActorFaceEachOther
	movs r0, #20
	bl Engine_EventWait
	movs r0, #0
	bl Object_GetById
	adds r5, r0, #0
	bl Engine_RandomNext
	movs r1, #20
	bl IwramUnsignedRemainder
	adds r5, #100
	ldr r2, .L_0200b394
	adds r0, #20
	movs r3, #0
	strh r0, [r5]
	movs r0, #22
	mov r9, r3
	mov r8, r2
	bl Object_GetById
	adds r5, r0, #0
	bl Engine_RandomNext
	movs r1, #20
	bl IwramUnsignedRemainder
	adds r5, #100
	adds r0, #20
	strh r0, [r5]
	movs r0, #26
	bl Object_GetById
	adds r5, r0, #0
	bl Engine_RandomNext
	movs r1, #20
	bl IwramUnsignedRemainder
	adds r5, #100
	adds r0, #20
	strh r0, [r5]
	b .L_0200b3c4
	.2byte 0x0000
.L_0200b394:
	.4byte 0x00000000
.L_0200b398:
	.4byte 0x058b0000
.L_0200b39c:
	.4byte 0x00013333
.L_0200b3a0:
	.4byte 0x00009999
.L_0200b3a4:
	.4byte Data_02004d6c
.L_0200b3a8:
	.4byte 0x05940000
.L_0200b3ac:
	.4byte 0x052d0000
.L_0200b3b0:
	.4byte Data_02004e04
.L_0200b3b4:
	.4byte Data_02004e30
.L_0200b3b8:
	.4byte Data_02004e5c
.L_0200b3bc:
	.4byte Data_02004e88
.L_0200b3c0:
	.4byte 0x00000101
.L_0200b3c4:
	movs r0, #8
	bl Object_GetById
	adds r5, r0, #0
	bl Engine_RandomNext
	movs r1, #20
	bl IwramUnsignedRemainder
	adds r5, #100
	adds r0, #20
	strh r0, [r5]
	movs r0, #9
	bl Object_GetById
	adds r5, r0, #0
	bl Engine_RandomNext
	movs r1, #20
	bl IwramUnsignedRemainder
	adds r5, #100
	adds r0, #20
	strh r0, [r5]
	ldr r5, .L_0200b7ec
	movs r0, #9
	adds r1, r5, #0
	bl Engine_ActorEnableActionCallback
	movs r0, #30
	bl Engine_EventWait
	adds r1, r5, #0
	movs r0, #0
	bl Engine_ActorEnableActionCallback
	adds r1, r5, #0
	movs r0, #26
	bl Engine_ActorEnableActionCallback
	adds r1, r5, #0
	movs r0, #22
	bl Engine_ActorEnableActionCallback
	adds r1, r5, #0
	movs r0, #8
	bl Engine_ActorEnableActionCallback
	movs r0, #10
	bl Engine_EventWait
	movs r0, #17
	bl Engine_AudioPlayCue
	movs r0, #192
	movs r1, #192
	movs r2, #128
	lsls r1, r1, #10
	lsls r2, r2, #9
	lsls r0, r0, #10
	bl Engine_WorkSetValuesIfNonNegative
	movs r0, #145
	bl Engine_AudioPlayCue
	movs r0, #30
	bl Engine_EventWait
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #9
	lsls r2, r2, #9
	lsls r0, r0, #9
	bl Engine_WorkSetValuesIfNonNegative
	movs r0, #120
	bl Engine_EventWait
	movs r0, #192
	movs r1, #192
	movs r2, #128
	lsls r1, r1, #10
	lsls r2, r2, #9
	lsls r0, r0, #10
	bl Engine_WorkSetValuesIfNonNegative
	movs r0, #145
	bl Engine_AudioPlayCue
	movs r0, #40
	bl Engine_EventWait
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #10
	lsls r2, r2, #9
	lsls r0, r0, #10
	bl Engine_WorkSetValuesIfNonNegative
	movs r0, #60
	bl Engine_EventWait
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #11
	lsls r2, r2, #9
	lsls r0, r0, #11
	bl Engine_WorkSetValuesIfNonNegative
	movs r0, #145
	bl Engine_AudioPlayCue
	movs r0, #20
	bl Engine_EventWait
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #9
	lsls r2, r2, #9
	lsls r0, r0, #9
	bl Engine_WorkSetValuesIfNonNegative
	movs r0, #60
	bl Engine_EventWait
	movs r0, #192
	movs r1, #192
	movs r2, #128
	lsls r1, r1, #10
	lsls r2, r2, #9
	lsls r0, r0, #10
	bl Engine_WorkSetValuesIfNonNegative
	movs r0, #145
	bl Engine_AudioPlayCue
	movs r0, #40
	bl Engine_EventWait
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #9
	lsls r2, r2, #9
	lsls r0, r0, #9
	bl Engine_WorkSetValuesIfNonNegative
	movs r0, #60
	bl Engine_EventWait
	bl BattleFx_SetBlock30ValuesMaxZero
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #9
	lsls r2, r2, #9
	lsls r0, r0, #9
	bl Engine_WorkSetValuesIfNonNegative
	movs r0, #1
	bl Engine_EventWait
	movs r0, #1
	movs r1, #1
	ldr r2, .L_0200b7f0
	negs r0, r0
	negs r1, r1
	bl Engine_WorkSetValuesIfNonNegative
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #12
	lsls r1, r1, #12
	bl Engine_CameraSetSpeed
	movs r0, #217
	movs r1, #1
	ldr r2, .L_0200b7f4
	movs r3, #1
	lsls r0, r0, #16
	negs r1, r1
	bl Engine_CameraMoveTo
	movs r1, #0
	movs r0, #0
	bl Engine_ColorBufferApplyTarget
	movs r0, #40
	bl Engine_ColorBufferInterpolate
	movs r0, #40
	bl Engine_TaskWait
	movs r1, #0
	movs r0, #19
	bl Engine_ActorSetChildValue
	movs r0, #19
	bl Object_GetById
	movs r1, #0
	bl Engine_ActorSetSpriteFlags
	movs r0, #27
	bl Object_GetById
	movs r1, #0
	bl Engine_ActorSetSpriteFlags
	ldr r3, .L_0200b7f8
	adds r1, r6, #0
	str r3, [r6, #24]
	str r3, [r6, #28]
	adds r1, #35
	ldrb r2, [r1]
	movs r0, #254
	adds r3, r0, #0
	ands r3, r2
	strb r3, [r1]
	mov r1, r11
	ldrb r2, [r1, #9]
	movs r1, #13
	negs r1, r1
	adds r3, r1, #0
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	mov r2, r11
	strb r3, [r2, #9]
	movs r3, #200
	lsls r3, r3, #16
	ldr r2, .L_0200b7fc
	str r3, [r7, #8]
	str r3, [r7, #12]
	str r3, [r7, #56]
	str r3, [r7, #60]
	adds r3, r7, #0
	str r2, [r7, #16]
	str r2, [r7, #64]
	adds r3, #85
	mov r2, r8
	str r3, [sp, #0]
	strb r2, [r3]
	adds r2, r7, #0
	adds r2, #35
	ldrb r3, [r2]
	ands r0, r3
	strb r0, [r2]
	mov r0, r10
	ldrb r3, [r0, #9]
	ands r1, r3
	strb r1, [r0, #9]
	bl Battle_GetWorkObject1e0
	movs r5, #128
	lsls r5, r5, #24
	str r5, [r0, #56]
	bl Battle_GetWorkObject1e0
	str r5, [r0, #60]
	bl Battle_GetWorkObject1e0
	str r5, [r0, #64]
	bl Battle_GetWorkObject1e0
	mov r1, r9
	str r1, [r0, #36]
	bl Battle_GetWorkObject1e0
	mov r2, r9
	str r2, [r0, #40]
	bl Battle_GetWorkObject1e0
	mov r3, r9
	str r3, [r0, #44]
	movs r0, #1
	bl Engine_TaskWait
	movs r0, #247
	movs r1, #128
	ldr r2, .L_0200b800
	movs r3, #0
	lsls r1, r1, #16
	lsls r0, r0, #16
	bl Engine_CameraMoveTo
	bl Engine_MapRedraw
	movs r0, #1
	bl Engine_TaskWait
	ldr r0, .L_0200b804
	movs r1, #1
	bl Engine_ColorBufferApplyTarget
	movs r0, #128
	movs r1, #2
	lsls r0, r0, #9
	bl Engine_ColorBufferApplyTarget
	movs r0, #30
	bl Engine_ColorBufferInterpolate
	movs r0, #30
	bl Engine_TaskWait
	movs r1, #200
	lsls r1, r1, #4
	ldr r0, .L_0200b808
	bl Scheduler_AddOrUpdateCallback
	ldr r1, .L_0200b80c
	movs r0, #19
	bl Engine_ActorEnableActionCallback
	movs r0, #128
	lsls r0, r0, #10
	ldr r1, .L_0200b810
	bl Engine_CameraSetSpeed
	movs r0, #175
	movs r1, #192
	lsls r0, r0, #16
	lsls r1, r1, #15
	ldr r2, .L_0200b814
	movs r3, #1
	bl Engine_CameraMoveTo
	adds r5, r7, #0
	adds r5, #102
.L_0200b662:
	movs r0, #1
	bl Engine_TaskWait
	movs r0, #0
	ldrsh r3, [r5, r0]
	cmp r3, #8
	bne .L_0200b662
	movs r1, #0
	movs r0, #0
	bl Engine_ColorBufferApplyTarget
	movs r0, #60
	bl Engine_ColorBufferInterpolate
	movs r0, #60
	bl Engine_TaskWait
	bl Engine_MapWaitWorkValuesBelow256
	bl Battle_GetWorkObject1e0
	movs r1, #128
	lsls r1, r1, #24
	str r1, [r0, #56]
	mov r8, r1
	bl Battle_GetWorkObject1e0
	mov r2, r8
	str r2, [r0, #60]
	bl Battle_GetWorkObject1e0
	mov r3, r8
	str r3, [r0, #64]
	bl Battle_GetWorkObject1e0
	movs r5, #0
	str r5, [r0, #36]
	bl Battle_GetWorkObject1e0
	str r5, [r0, #40]
	bl Battle_GetWorkObject1e0
	str r5, [r0, #44]
	ldr r0, .L_0200b808
	bl Scheduler_RemoveCallback
	movs r0, #19
	bl Engine_ActorStop
	movs r0, #1
	bl Engine_TaskWait
	movs r0, #19
	movs r1, #0
	bl Engine_ActorSetAnimation
	mov r2, r11
	movs r3, #160
	lsls r3, r3, #9
	adds r2, #35
	movs r0, #2
	str r3, [r6, #24]
	str r3, [r6, #28]
	strb r0, [r2]
	movs r2, #128
	lsls r2, r2, #10
	mov r1, r11
	str r3, [r1, #24]
	movs r0, #1
	str r2, [r7, #24]
	str r2, [r7, #28]
	str r5, [r7, #8]
	str r5, [r7, #16]
	str r5, [r7, #56]
	str r5, [r7, #64]
	mov r10, r2
	bl Engine_TaskWait
	movs r0, #23
	movs r1, #8
	bl Engine_ActorSetAnimation
	movs r1, #169
	movs r2, #158
	movs r0, #9
	lsls r1, r1, #16
	lsls r2, r2, #19
	bl Engine_ActorSetPosition
	movs r1, #192
	movs r2, #0
	movs r0, #9
	lsls r1, r1, #8
	bl Engine_ActorFaceDirection
	movs r0, #9
	movs r1, #9
	bl Engine_ActorSetAnimation
	movs r1, #151
	movs r0, #26
	lsls r1, r1, #16
	ldr r2, .L_0200b818
	bl Engine_ActorSetPosition
	movs r1, #128
	movs r2, #0
	movs r0, #26
	lsls r1, r1, #8
	bl Engine_ActorFaceDirection
	movs r0, #26
	movs r1, #5
	bl Engine_ActorSetAnimation
	movs r1, #170
	movs r0, #8
	lsls r1, r1, #16
	ldr r2, .L_0200b81c
	bl Engine_ActorSetPosition
	movs r1, #192
	movs r2, #0
	movs r0, #8
	lsls r1, r1, #7
	bl Engine_ActorFaceDirection
	movs r0, #8
	movs r1, #5
	bl Engine_ActorSetAnimation
	movs r1, #185
	movs r0, #0
	lsls r1, r1, #16
	ldr r2, .L_0200b820
	bl Engine_ActorSetPosition
	movs r1, #128
	movs r2, #0
	movs r0, #0
	lsls r1, r1, #6
	bl Engine_ActorFaceDirection
	movs r0, #0
	movs r1, #17
	bl Engine_ActorSetAnimation
	movs r1, #169
	movs r2, #173
	movs r0, #22
	lsls r1, r1, #16
	lsls r2, r2, #19
	bl Engine_ActorSetPosition
	movs r1, #128
	movs r2, #0
	movs r0, #22
	lsls r1, r1, #7
	bl Engine_ActorFaceDirection
	movs r0, #22
	movs r1, #0
	bl Engine_ActorSetAnimation
	movs r0, #166
	movs r1, #0
	ldr r2, .L_0200b824
	lsls r0, r0, #16
	movs r3, #0
	bl Engine_CameraMoveTo
	bl Engine_MapRedraw
	ldr r3, [sp, #0]
	mov r0, r8
	strb r5, [r3]
	str r0, [r7, #56]
	str r0, [r7, #60]
	str r0, [r7, #64]
	bl HaidiaArashi_FlashLightning
	movs r1, #218
	movs r2, #147
	movs r0, #27
	lsls r1, r1, #16
	lsls r2, r2, #19
	bl Engine_ActorSetPosition
	movs r0, #210
	ldr r2, .L_0200b828
	movs r3, #0
	lsls r0, r0, #16
	movs r1, #0
	bl Engine_CameraMoveTo
	b .L_0200b82c
	.2byte 0x0000
.L_0200b7ec:
	.4byte Data_02004eb4
.L_0200b7f0:
	.4byte 0x0000e666
.L_0200b7f4:
	.4byte 0x043c0000
.L_0200b7f8:
	.4byte 0x0000cccc
.L_0200b7fc:
	.4byte 0x03820000
.L_0200b800:
	.4byte 0x03950000
.L_0200b804:
	.4byte 0x00010003
.L_0200b808:
	.4byte HaidiaArashi_UpdatePulsingGlow
.L_0200b80c:
	.4byte Data_02004edc
.L_0200b810:
	.4byte 0x000007ae
.L_0200b814:
	.4byte 0x043e0000
.L_0200b818:
	.4byte 0x050c0000
.L_0200b81c:
	.4byte 0x05210000
.L_0200b820:
	.4byte 0x05350000
.L_0200b824:
	.4byte 0x05390000
.L_0200b828:
	.4byte 0x04ac0000
.L_0200b82c:
	bl Engine_MapRedraw
	mov r1, r10
	str r1, [r6, #24]
	str r1, [r6, #28]
	movs r1, #200
	lsls r1, r1, #4
	ldr r0, .L_0200bac0
	bl Scheduler_AddOrUpdateCallback
	movs r0, #10
	bl Engine_ActorStop
	movs r0, #24
	bl Engine_ActorStop
	movs r0, #25
	bl Engine_ActorStop
	movs r0, #1
	bl Engine_TaskWait
	movs r0, #10
	bl Object_GetById
	adds r6, r0, #0
	ldr r2, [r6, #80]
	adds r1, r6, #0
	adds r1, #35
	mov r11, r2
	ldrb r2, [r1]
	movs r3, #254
	movs r0, #128
	mov r10, r3
	lsls r0, r0, #9
	ands r3, r2
	strb r3, [r1]
	str r0, [r6, #24]
	str r0, [r6, #28]
	mov r1, r11
	movs r3, #208
	ldrb r2, [r1, #9]
	subs r5, #13
	lsls r3, r3, #8
	strh r3, [r6, #6]
	adds r3, r5, #0
	ands r3, r2
	strb r3, [r1, #9]
	movs r0, #10
	movs r1, #0
	bl Engine_ActorSetAnimation
	movs r0, #24
	bl Object_GetById
	adds r6, r0, #0
	ldr r2, [r6, #80]
	adds r1, r6, #0
	adds r1, #35
	mov r11, r2
	ldrb r2, [r1]
	mov r3, r10
	ands r3, r2
	strb r3, [r1]
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r6, #24]
	str r3, [r6, #28]
	movs r0, #176
	mov r3, r11
	ldrb r2, [r3, #9]
	lsls r0, r0, #8
	mov r9, r0
	adds r3, r5, #0
	ands r3, r2
	mov r1, r9
	mov r0, r11
	strb r3, [r0, #9]
	strh r1, [r6, #6]
	movs r0, #24
	movs r1, #5
	bl Engine_ActorSetAnimation
	movs r0, #25
	bl Object_GetById
	adds r6, r0, #0
	ldr r1, [r6, #80]
	mov r11, r1
	adds r1, r6, #0
	adds r1, #35
	ldrb r2, [r1]
	mov r3, r10
	ands r3, r2
	movs r2, #128
	lsls r2, r2, #9
	strb r3, [r1]
	str r2, [r6, #24]
	str r2, [r6, #28]
	mov r0, r11
	ldrb r2, [r0, #9]
	mov r3, r9
	strh r3, [r6, #6]
	adds r3, r5, #0
	ands r3, r2
	strb r3, [r0, #9]
	movs r1, #5
	movs r0, #25
	bl Engine_ActorSetAnimation
	movs r0, #27
	bl Object_GetById
	adds r6, r0, #0
	ldr r1, [r6, #80]
	mov r11, r1
	bl HaidiaArashi_FlashLightning
	movs r3, #192
	lsls r3, r3, #14
	movs r1, #214
	movs r2, #152
	str r3, [r7, #12]
	lsls r1, r1, #16
	mov r3, r8
	lsls r2, r2, #19
	str r1, [r7, #8]
	str r2, [r7, #16]
	str r3, [r7, #56]
	str r3, [r7, #60]
	str r3, [r7, #64]
	mov r0, r11
	ldrb r3, [r0, #9]
	ands r5, r3
	movs r3, #4
	orrs r5, r3
	strb r5, [r0, #9]
	movs r0, #27
	bl Engine_ActorSetPosition
	movs r1, #192
	movs r0, #24
	lsls r1, r1, #8
	movs r2, #0
	bl Engine_ActorFaceDirection
	movs r1, #192
	movs r2, #20
	lsls r1, r1, #8
	movs r0, #25
	bl Engine_ActorFaceDirection
	movs r0, #179
	lsls r0, r0, #1
	bl Engine_GameFlagSet
	movs r0, #0
	bl Map_SetLayerEntryFlag
	movs r0, #1
	bl Map_SetLayerEntryFlag
	movs r0, #2
	bl Map_SetLayerEntryFlag
	movs r0, #3
	bl Map_SetLayerEntryFlag
	movs r0, #4
	bl Map_SetLayerEntryFlag
	movs r0, #5
	bl Map_SetLayerEntryFlag
	ldr r0, .L_0200bac4
	movs r1, #1
	bl Engine_ColorBufferApplyTarget
	movs r0, #128
	movs r1, #2
	lsls r0, r0, #9
	bl Engine_ColorBufferApplyTarget
	movs r0, #120
	bl Engine_ColorBufferInterpolate
	movs r0, #160
	bl Engine_TaskWait
	ldr r0, .L_0200bac8
	movs r1, #1
	bl Engine_ColorBufferApplyTarget
	movs r1, #2
	ldr r0, .L_0200bac8
	bl Engine_ColorBufferApplyTarget
	movs r0, #80
	bl Engine_ColorBufferInterpolate
	movs r0, #80
	bl Engine_EventWait
	movs r0, #100
	bl Engine_EventWait
	ldr r0, .L_0200bac0
	bl Scheduler_RemoveCallback
	ldr r3, [r6, #24]
	mov r1, r11
	movs r0, #179
	str r3, [r1, #24]
	lsls r0, r0, #1
	bl Engine_GameFlagClear
	movs r0, #0
	bl Map_ClearLayerEntryFlag
	movs r0, #1
	bl Map_ClearLayerEntryFlag
	movs r0, #2
	bl Map_ClearLayerEntryFlag
	movs r0, #3
	bl Map_ClearLayerEntryFlag
	movs r0, #4
	bl Map_ClearLayerEntryFlag
	movs r0, #5
	bl Map_ClearLayerEntryFlag
	bl FieldScene_BuildPlacementGrid
	movs r1, #165
	ldr r2, .L_0200bacc
	movs r0, #9
	lsls r1, r1, #16
	bl Engine_ActorSetPosition
	movs r1, #1
	movs r0, #9
	bl Engine_ActorSetAnimation
	movs r0, #9
	bl Object_GetById
	movs r2, #224
	lsls r2, r2, #8
	mov r8, r2
	adds r7, r0, #0
	mov r3, r8
	strh r3, [r7, #6]
	bl Engine_RandomNext
	movs r1, #90
	bl IwramUnsignedRemainder
	adds r3, r7, #0
	ldr r5, .L_0200bad0
	adds r0, #60
	adds r3, #100
	adds r2, r7, #0
	strh r0, [r3]
	adds r2, #102
	movs r3, #1
	strh r3, [r2]
	adds r1, r5, #0
	movs r0, #9
	bl Engine_ActorEnableActionCallback
	movs r1, #165
	ldr r2, .L_0200bad4
	movs r0, #26
	lsls r1, r1, #16
	bl Engine_ActorSetPosition
	movs r1, #1
	movs r0, #26
	bl Engine_ActorSetAnimation
	movs r0, #26
	bl Object_GetById
	adds r7, r0, #0
	mov r0, r8
	strh r0, [r7, #6]
	bl Engine_RandomNext
	movs r1, #90
	bl IwramUnsignedRemainder
	adds r3, r7, #0
	adds r0, #60
	adds r3, #100
	ldr r1, .L_0200babc
	strh r0, [r3]
	adds r3, #2
	strh r1, [r3]
	movs r0, #26
	adds r1, r5, #0
	bl Engine_ActorEnableActionCallback
	movs r1, #152
	ldr r2, .L_0200bad8
	movs r0, #22
	lsls r1, r1, #16
	bl Engine_ActorSetPosition
	movs r1, #1
	movs r0, #22
	bl Engine_ActorSetAnimation
	movs r0, #22
	bl Object_GetById
	mov r2, r8
	adds r7, r0, #0
	strh r2, [r7, #6]
	bl Engine_RandomNext
	movs r1, #90
	bl IwramUnsignedRemainder
	adds r3, r7, #0
	b .L_0200badc
.L_0200babc:
	.4byte 0x00000002
.L_0200bac0:
	.4byte ActorPresentation_SelectActorTwentySevenState
.L_0200bac4:
	.4byte 0x00010003
.L_0200bac8:
	.4byte 0x00007fff
.L_0200bacc:
	.4byte 0x04cd0000
.L_0200bad0:
	.4byte HaidiaArashi_ActorEightScript
.L_0200bad4:
	.4byte 0x04e60000
.L_0200bad8:
	.4byte 0x05050000
.L_0200badc:
	adds r0, #60
	adds r3, #100
	adds r2, r7, #0
	strh r0, [r3]
	adds r2, #102
	movs r3, #3
	strh r3, [r2]
	adds r1, r5, #0
	movs r0, #22
	bl Engine_ActorEnableActionCallback
	movs r1, #180
	ldr r2, .L_0200bc30
	lsls r1, r1, #16
	movs r0, #8
	bl Engine_ActorSetPosition
	movs r0, #8
	bl Object_GetById
	mov r3, r8
	adds r7, r0, #0
	strh r3, [r7, #6]
	bl Engine_RandomNext
	movs r1, #90
	bl IwramUnsignedRemainder
	adds r3, r7, #0
	adds r0, #60
	adds r3, #100
	adds r2, r7, #0
	adds r2, #102
	strh r0, [r3]
	movs r3, #4
	strh r3, [r2]
	adds r1, r5, #0
	movs r0, #8
	bl Engine_ActorEnableActionCallback
	movs r1, #6
	movs r0, #8
	bl Engine_ActorSetAnimation
	movs r0, #22
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	mov r3, r10
	ands r3, r2
	strb r3, [r0]
	movs r0, #8
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	mov r1, r10
	ands r1, r3
	strb r1, [r0]
	mov r10, r1
	movs r1, #200
	lsls r1, r1, #4
	ldr r0, .L_0200bc34
	bl Scheduler_AddOrUpdateCallback
	movs r1, #181
	lsls r1, r1, #16
	ldr r2, .L_0200bc38
	movs r0, #0
	bl Engine_ActorSetPosition
	movs r0, #0
	bl Object_GetById
	mov r2, r8
	strh r2, [r0, #6]
	movs r1, #1
	movs r0, #0
	bl Engine_ActorSetAnimation
	movs r0, #181
	movs r3, #0
	lsls r0, r0, #16
	movs r1, #0
	ldr r2, .L_0200bc38
	bl Engine_CameraMoveTo
	bl Engine_MapRedraw
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl Engine_ActorSetPosition
	movs r0, #19
	movs r1, #0
	movs r2, #0
	bl Engine_ActorSetPosition
	movs r0, #24
	movs r1, #0
	movs r2, #0
	bl Engine_ActorSetPosition
	movs r0, #25
	movs r1, #0
	movs r2, #0
	bl Engine_ActorSetPosition
	movs r0, #23
	movs r1, #0
	movs r2, #0
	bl Engine_ActorSetPosition
	movs r0, #27
	movs r1, #0
	movs r2, #0
	bl Engine_ActorSetPosition
	movs r1, #144
	movs r0, #17
	lsls r1, r1, #16
	ldr r2, .L_0200bc3c
	bl Engine_ActorSetPosition
	movs r1, #138
	ldr r2, .L_0200bc40
	lsls r1, r1, #17
	movs r0, #18
	bl Engine_ActorSetPosition
	movs r0, #60
	bl Engine_TaskWait
	ldr r0, .L_0200bc44
	movs r1, #1
	bl Engine_ColorBufferApplyTarget
	movs r0, #128
	movs r1, #2
	lsls r0, r0, #9
	bl Engine_ColorBufferApplyTarget
	movs r0, #80
	bl Engine_ColorBufferInterpolate
	movs r0, #60
	bl Engine_EventWait
	bl BattleFx_PlayQueuedSound
	movs r0, #60
	bl Engine_EventWait
	movs r0, #1
	bl Party_RemoveOwnerRestored
	bl BattleFx_SetBlock30Values128One
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
.L_0200bc30:
	.4byte 0x051f0000
.L_0200bc34:
	.4byte OverlayObject_CopyRecordField1ToSlots22And8
.L_0200bc38:
	.4byte 0x04f90000
.L_0200bc3c:
	.4byte 0x042e0000
.L_0200bc40:
	.4byte 0x04f60000
.L_0200bc44:
	.4byte 0x00010003
	.section .rodata.x0200c8bc,"a",%progbits
	.global HaidiaArashi_FrameModes
HaidiaArashi_FrameModes:
	.4byte 0x00000007
	.global HaidiaArashi_ActorTwentyScript
HaidiaArashi_ActorTwentyScript:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00b90000
	.4byte 0x00000000
	.4byte 0x03380000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00b30000
	.4byte 0x00000000
	.4byte 0x03440000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00e20000
	.4byte 0x00000000
	.4byte 0x03480000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00e20000
	.4byte 0x00000000
	.4byte 0x03b00000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.global HaidiaArashi_ActorTwentyTwoScriptA
HaidiaArashi_ActorTwentyTwoScriptA:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x01590000
	.4byte 0x00000000
	.4byte 0x02640000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x01370000
	.4byte 0x00000000
	.4byte 0x027e0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x01000000
	.4byte 0x00000000
	.4byte 0x02600000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.global HaidiaArashi_ActorTwentyTwoScriptB
HaidiaArashi_ActorTwentyTwoScriptB:
	.4byte 0x80010000
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x000000c0
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x00000180
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x00000018
	.4byte 0xc0010000
	.4byte 0x80020000
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0xfffffe80
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffffd00
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x0000000c
	.4byte 0xc0020000
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000010
	.global HaidiaArashi_ActorNineteenScript
HaidiaArashi_ActorNineteenScript:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000010
	.global HaidiaArashi_LeaderScript
HaidiaArashi_LeaderScript:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x005e0000
	.4byte 0x00000000
	.4byte 0x02340000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x00000010
	.global HaidiaArashi_ActorTwentyTwoScript
HaidiaArashi_ActorTwentyTwoScript:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x004c0000
	.4byte 0x00000000
	.4byte 0x02340000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x00000010
	.global HaidiaArashi_ActorNineScriptA
HaidiaArashi_ActorNineScriptA:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00b20000
	.4byte 0x00000000
	.4byte 0x04a60000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00950000
	.4byte 0x00000000
	.4byte 0x04aa0000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.global HaidiaArashi_ActorTwentySixScriptA
HaidiaArashi_ActorTwentySixScriptA:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00c60000
	.4byte 0x00000000
	.4byte 0x04980000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00aa0000
	.4byte 0x00000000
	.4byte 0x04ab0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00950000
	.4byte 0x00000000
	.4byte 0x04aa0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00950000
	.4byte 0x00000000
	.4byte 0x04970000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.global HaidiaArashi_ActorNineScriptB
HaidiaArashi_ActorNineScriptB:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x00000002
	.4byte 0x00660000
	.4byte 0x00000000
	.4byte 0x04a50000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00660000
	.4byte 0x00000000
	.4byte 0x04c20000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00540000
	.4byte 0x00000000
	.4byte 0x04cc0000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000010
	.global HaidiaArashi_ActorTwentySixScriptB
HaidiaArashi_ActorTwentySixScriptB:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x00000002
	.4byte 0x00660000
	.4byte 0x00000000
	.4byte 0x04a50000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00660000
	.4byte 0x00000000
	.4byte 0x04c20000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x04cc0000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000010
	.global HaidiaArashi_ActorNineScriptC
HaidiaArashi_ActorNineScriptC:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00320000
	.4byte 0x00000000
	.4byte 0x04be0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x002e0000
	.4byte 0x00000000
	.4byte 0x046a0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00430000
	.4byte 0x00000000
	.4byte 0x044c0000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.global HaidiaArashi_ActorTwentySixScriptC
HaidiaArashi_ActorTwentySixScriptC:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00990000
	.4byte 0x00000000
	.4byte 0x04e30000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00b10000
	.4byte 0x00000000
	.4byte 0x054d0000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.global HaidiaArashi_ActorNineScriptD
HaidiaArashi_ActorNineScriptD:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x007b0000
	.4byte 0x00000000
	.4byte 0x04360000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00940000
	.4byte 0x00000000
	.4byte 0x04360000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00940000
	.4byte 0x00000000
	.4byte 0x04280000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00230000
	.4byte 0x00000000
	.4byte 0x04280000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.global HaidiaArashi_StormRunActions
HaidiaArashi_StormRunActions:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00660000
	.4byte 0x00000000
	.4byte 0x04d40000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00940000
	.4byte 0x00000000
	.4byte 0x04e80000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00b10000
	.4byte 0x00000000
	.4byte 0x05170000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.global Data_02004d6c
Data_02004d6c:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00890000
	.4byte 0x00000000
	.4byte 0x05630000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00720000
	.4byte 0x00000000
	.4byte 0x05880000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x005f0000
	.4byte 0x00000000
	.4byte 0x05880000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00530000
	.4byte 0x00000000
	.4byte 0x05930000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x009a0000
	.4byte 0x00000000
	.4byte 0x05960000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00b30000
	.4byte 0x00000000
	.4byte 0x05760000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00b30000
	.4byte 0x00000000
	.4byte 0x05510000
	.4byte 0x00000001
	.4byte 0x00000010
	.global Data_02004e04
Data_02004e04:
	.4byte 0x00000003
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x051d0000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x0000b000
	.4byte 0x00000010
	.global Data_02004e30
Data_02004e30:
	.4byte 0x00000003
	.4byte 0x00a50000
	.4byte 0x00000000
	.4byte 0x051a0000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x0000a000
	.4byte 0x00000010
	.global Data_02004e5c
Data_02004e5c:
	.4byte 0x00000003
	.4byte 0x00b70000
	.4byte 0x00000000
	.4byte 0x052d0000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x0000a000
	.4byte 0x00000010
	.global Data_02004e88
Data_02004e88:
	.4byte 0x00000003
	.4byte 0x00a60000
	.4byte 0x00000000
	.4byte 0x052d0000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x0000b000
	.4byte 0x00000010
	.global Data_02004eb4
Data_02004eb4:
	.4byte 0x00000022
	.4byte OverlayObject_SetField6OnCountdown
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0x00000000
	.global HaidiaArashi_ActorEightScript
HaidiaArashi_ActorEightScript:
	.4byte 0x00000022
	.4byte UpdateFixedPointCountdown
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0x00000000
	.global Data_02004edc
Data_02004edc:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x0000000f
	.4byte 0x00030000
	.4byte 0x00000015
	.4byte 0x00000010
	.4byte 0x00010000
	.4byte 0x00000015
	.4byte 0x00000023
	.4byte 0x00000000
	.4byte 0x00000015
	.4byte 0x00000024
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00020000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00020000
	.4byte 0x00000003
	.4byte 0x01000000
	.4byte 0x00800000
	.4byte 0x03a30000
	.4byte 0x00000001
	.4byte 0x0000001e
	.4byte 0x00000091
	.4byte 0x00000015
	.4byte 0x00000023
	.4byte 0x0000003c
	.4byte 0x00000015
	.4byte 0x00000024
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00e40000
	.4byte 0x00950000
	.4byte 0x03bd0000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000024
	.4byte 0x00000003
	.4byte 0x00000003
	.4byte 0x00c90000
	.4byte 0x00a00000
	.4byte 0x03d80000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000024
	.4byte 0x00000004
	.4byte 0x00000003
	.4byte 0x00ad0000
	.4byte 0x008a0000
	.4byte 0x03f30000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000024
	.4byte 0x00000005
	.4byte 0x00000003
	.4byte 0x00920000
	.4byte 0x00600000
	.4byte 0x040e0000
	.4byte 0x00000001
	.4byte 0x0000001e
	.4byte 0x00000091
	.4byte 0x00000015
	.4byte 0x0000000f
	.4byte 0x00018000
	.4byte 0x00000015
	.4byte 0x00000010
	.4byte 0x00008000
	.4byte 0x00000015
	.4byte 0x00000023
	.4byte 0x0000003c
	.4byte 0x00000015
	.4byte 0x00000024
	.4byte 0x00000006
	.4byte 0x00000003
	.4byte 0x00a10000
	.4byte 0x006b0000
	.4byte 0x041a0000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x0000000f
	.4byte 0x00020000
	.4byte 0x00000015
	.4byte 0x00000010
	.4byte 0x00009999
	.4byte 0x00000015
	.4byte 0x00000024
	.4byte 0x00000007
	.4byte 0x00000003
	.4byte 0x00b10000
	.4byte 0x00600000
	.4byte 0x04260000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x0000000f
	.4byte 0x00028000
	.4byte 0x00000015
	.4byte 0x00000010
	.4byte 0x0000cccc
	.4byte 0x00000015
	.4byte 0x00000024
	.4byte 0x00000008
	.4byte 0x00000003
	.4byte 0x00c90000
	.4byte 0x004a0000
	.4byte 0x04320000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x0000000f
	.4byte 0x00030000
	.4byte 0x00000015
	.4byte 0x00000010
	.4byte 0x00010000
	.4byte 0x00000015
	.4byte 0x00000024
	.4byte 0x00000009
	.4byte 0x00000003
	.4byte 0x00e10000
	.4byte 0xffc00000
	.4byte 0x043e0000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000023
	.4byte 0x00000000
	.4byte 0x00000015
	.4byte 0x00000024
	.4byte 0x00000000
	.4byte 0x00000010
	.global HaidiaArashi_SceneTable0
HaidiaArashi_SceneTable0:
	.4byte 0xffff0000
	.4byte 0x000000a7
	.4byte 0x40000501
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000101
	.4byte 0x400001c8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0002
	.4byte 0x00000071
	.4byte 0x4000012f
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0003
	.4byte 0x0000001b
	.4byte 0x0000026d
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0004
	.4byte 0x0000001d
	.4byte 0x00000318
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0005
	.4byte 0x000001ca
	.4byte 0x80000571
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0006
	.4byte 0x00000196
	.4byte 0x400002e7
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0007
	.4byte 0x00000106
	.4byte 0x40000335
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0008
	.4byte 0x00000154
	.4byte 0x40000388
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0009
	.4byte 0x00000146
	.4byte 0x40000476
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000a
	.4byte 0x00000176
	.4byte 0x400004e6
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000b
	.4byte 0x00000066
	.4byte 0x400004c6
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000c
	.4byte 0x00000065
	.4byte 0x400004c0
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000d
	.4byte 0x00000092
	.4byte 0x400004a9
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000f
	.4byte 0x0000014f
	.4byte 0xc000038c
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0010
	.4byte 0x000000b5
	.4byte 0xe00004f9
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global HaidiaArashi_SceneTable1
HaidiaArashi_SceneTable1:
	.4byte 0x00000003
	.4byte 0x00208005
	.4byte 0x00301006
	.4byte 0x00402006
	.4byte 0x00506007
	.4byte 0x00605007
	.4byte 0x00706008
	.4byte 0x00802007
	.4byte 0x00901007
	.4byte 0x00a01007
	.4byte 0x00b01007
	.4byte 0x00c0b008
	.4byte 0x00d0c008
	.4byte 0x01410003
	.4byte 0x000001ff
	.global HaidiaArashi_SceneTable2
HaidiaArashi_SceneTable2:
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000c000
	.4byte 0xffff001f
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0035
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff00c8
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff00c8
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff00c8
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff00c8
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff00c8
	.4byte 0x00000001
	.4byte 0x01b50000
	.4byte 0x00000000
	.4byte 0x02fa0000
	.4byte 0x00024000
	.4byte 0xffff00c8
	.4byte 0x00000001
	.4byte 0x01420000
	.4byte 0x00000000
	.4byte 0x05990000
	.4byte 0x01024000
	.4byte 0xffff00c8
	.4byte 0x00000001
	.4byte 0x00900000
	.4byte 0x00000000
	.4byte 0x042e0000
	.4byte 0x01024000
	.4byte 0xffff00c8
	.4byte 0x00000001
	.4byte 0x01140000
	.4byte 0x00000000
	.4byte 0x04f60000
	.4byte 0x01024000
	.4byte 0xffff00c8
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff006d
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff00d2
	.4byte 0x00000001
	.4byte 0x01a40000
	.4byte 0x00000000
	.4byte 0x026b0000
	.4byte 0x00004000
	.4byte 0xffff0013
	.4byte 0x00000001
	.4byte 0x018c0000
	.4byte 0x00000000
	.4byte 0x026b0000
	.4byte 0x00000000
	.4byte 0xffff0024
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff003b
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff003c
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0030
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0120
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global HaidiaArashi_LastObjectCall
HaidiaArashi_LastObjectCall:
	.4byte 0xffff0016
	.4byte 0x00000007
	.4byte 0x007c0000
	.4byte 0x00000000
	.4byte 0x01770000
	.4byte 0x0002b000
	.4byte 0xffff006c
	.4byte 0x00000001
	.4byte 0x00550000
	.4byte 0x00000000
	.4byte 0x01690000
	.4byte 0x0002d000
	.4byte 0xffff006c
	.4byte 0x00000001
	.4byte 0x00870000
	.4byte 0x00000000
	.4byte 0x016b0000
	.4byte 0x0002b000
	.4byte 0xffff0016
	.4byte 0x00000007
	.4byte 0x00640000
	.4byte 0x00000000
	.4byte 0x01770000
	.4byte 0x0002d000
	.4byte 0xffff007a
	.4byte 0x00000001
	.4byte 0x007c0000
	.4byte 0x00000000
	.4byte 0x01770000
	.4byte 0x0002b000
	.4byte 0xffff007a
	.4byte 0x00000001
	.4byte 0x00640000
	.4byte 0x00000000
	.4byte 0x01770000
	.4byte 0x0002d000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global HaidiaArashi_SceneTable3
HaidiaArashi_SceneTable3:
	.4byte 0x00000002
	.4byte 0xffff0002
	.4byte SceneState_SetValue123Mode1
	.4byte 0x00000002
	.4byte 0xffff0003
	.4byte SceneState_ApplyValues123And3
	.4byte 0x00000002
	.4byte 0xffff0004
	.4byte SceneState_SetValue123Mode4
	.4byte 0x00000002
	.4byte 0xffff0005
	.4byte FieldScene_RunStep7BAndCheckFlags841And842
	.4byte 0x0000c602
	.4byte 0xffff0006
	.4byte FieldScene_SetupDescriptorD78a
	.4byte 0x0000c602
	.4byte 0xffff0007
	.4byte FieldScene_RunScene372_02000278
	.4byte 0x0000c602
	.4byte 0xffff0008
	.4byte FieldScene_SetupDescriptorD78aIfFlag205Clear
	.4byte 0x0000c602
	.4byte 0xffff0009
	.4byte FieldScene_SetupWithDescriptorD7A0
	.4byte 0x0000c602
	.4byte 0xffff000a
	.4byte FieldScene_SetupDescriptorD7b6
	.4byte 0x0000c602
	.4byte 0xffff000b
	.4byte FieldScene_RunScene372_02000398
	.4byte 0x0000c602
	.4byte 0xffff000c
	.4byte FieldScene_RunScene372_020003cc
	.4byte 0x0000c602
	.4byte 0xffff000d
	.4byte FieldScene_RunScene372_02000400
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte FieldScene_RunScene372SequenceC
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte SceneDialogue_RunActorTenFlag30dDialogue
	.4byte 0x00000000
	.4byte 0xffff0018
	.4byte 0x00000ea6
	.4byte 0x00000000
	.4byte 0xffff0019
	.4byte 0x00000ea7
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00000ece
	.4byte 0x00000000
	.4byte 0xffff001a
	.4byte 0x00000ecf
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte FieldScene_RunScene372_02003e48
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x00001121
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x00001121
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x00001121
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x00001121
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x00001121
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x00001121
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x00001121
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x00001121
	.4byte 0x00000002
	.4byte 0xffff000f
	.4byte SceneState_SetFlag210AndConfigureRegion40_84
	.4byte 0x00000002
	.4byte 0xffff000e
	.4byte SceneState_SetFlag210AndConfigureRegion40_89
	.4byte 0x00000002
	.4byte 0xffff0029
	.4byte HaidiaArashi_RunScene00D5C
	.4byte 0x00000002
	.4byte 0xffff002a
	.4byte HaidiaArashi_RunEventSequence
	.4byte 0x00000002
	.4byte 0xffff002b
	.4byte FieldScene_RunOpeningAuxiliarySequence
	.4byte 0x00000002
	.4byte 0xffff002c
	.4byte HaidiaArashi_RunSecondEventSequence
	.4byte 0x00000002
	.4byte 0xffff001e
	.4byte FieldScene_RunScene372SequenceB
	.4byte 0x00000002
	.4byte 0xffff001f
	.4byte FieldScene_RunActor22SceneWhenFlag836Only
	.4byte 0x00000002
	.4byte 0xffff0020
	.4byte FieldScene_RunScene372SequenceE
	.4byte 0x00000002
	.4byte 0xffff0021
	.4byte FieldScene_RunScene372SequenceD
	.4byte 0x00000002
	.4byte 0xffff0022
	.4byte Scene_BoulderFalls
	.4byte 0x00000002
	.4byte 0xffff0023
	.4byte FieldScene_RunFlagGatedActorSequence
	.4byte 0x00000002
	.4byte 0xffff0024
	.4byte HaidiaArashi_RunScene02DEC
	.4byte 0x00000002
	.4byte 0x087c0010
	.4byte SceneState_SetWords1c0And1c8AndRun
	.4byte 0x00000002
	.4byte 0x087f0011
	.4byte SceneState_SetWorkWordsAndFlag87f
	.4byte 0x00000003
	.4byte 0xffff0018
	.4byte SceneState_SetValueEe4
	.4byte 0x00000003
	.4byte 0xffff0013
	.4byte FieldScene_ShowChestValuables
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global HaidiaArashi_CellSteps0
HaidiaArashi_CellSteps0:
	.4byte 0x00620000
	.4byte 0x00020002
	.4byte 0x00020002
	.4byte 0x00020062
	.4byte 0x00020002
	.2byte 0xffff
	.global HaidiaArashi_CellSteps1
HaidiaArashi_CellSteps1:
	.2byte 0x0000
	.4byte 0x00020060
	.4byte 0x00020002
	.4byte 0x00600002
	.4byte 0x00020002
	.4byte 0xffff0002
	.global HaidiaArashi_CellSteps2
HaidiaArashi_CellSteps2:
	.4byte 0x00620004
	.4byte 0x00020002
	.4byte 0x00060002
	.4byte 0x00020062
	.4byte 0x00020002
	.2byte 0xffff
	.global HaidiaArashi_CellSteps3
HaidiaArashi_CellSteps3:
	.2byte 0x0004
	.4byte 0x00020060
	.4byte 0x00020002
	.4byte 0x00600006
	.4byte 0x00020002
	.4byte 0xffff0002
	.global HaidiaArashi_CellSteps4
HaidiaArashi_CellSteps4:
	.4byte 0x00600000
	.4byte 0x00020002
	.4byte 0x00320002
	.4byte 0x0002002c
	.4byte 0x00020002
	.2byte 0xffff
	.global HaidiaArashi_CellSteps5
HaidiaArashi_CellSteps5:
	.2byte 0x0004
	.4byte 0x00020062
	.4byte 0x00020002
	.4byte 0x006c002c
	.4byte 0x00020002
	.4byte 0xffff0002
