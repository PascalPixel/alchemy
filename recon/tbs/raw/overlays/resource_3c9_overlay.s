.syntax unified
	.thumb
	.section .text.x0200a360,"ax",%progbits
	.global Scene_RunPairedActorEffectSequence
	.thumb_func
Scene_RunPairedActorEffectSequence:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #136
	bl Engine_EventBegin
	movs r3, #17
	movs r2, #8
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #10
	movs r2, #4
	movs r3, #2
	movs r0, #17
	bl Engine_MapCopyCellAttributes
	movs r0, #1
	bl Object_GetById
	movs r3, #128
	lsls r3, r3, #8
	ldr r2, .L_0200a3d0
	mov r8, r3
	mov r4, r8
	mov r9, r2
	movs r1, #164
	movs r2, #168
	strh r4, [r0, #6]
	lsls r1, r1, #17
	lsls r2, r2, #16
	movs r0, #1
	bl Engine_ActorSetPosition
	movs r0, #2
	bl Object_GetById
	mov r2, r8
	strh r2, [r0, #6]
	movs r1, #170
	movs r2, #196
	lsls r1, r1, #17
	lsls r2, r2, #16
	movs r0, #2
	bl Engine_ActorSetPosition
	movs r0, #3
	bl Object_GetById
	mov r3, r8
	movs r1, #163
	movs r2, #204
	b .L_0200a3d4
.L_0200a3d0:
	.4byte 0x00000000
.L_0200a3d4:
	strh r3, [r0, #6]
	lsls r1, r1, #17
	lsls r2, r2, #16
	movs r0, #3
	bl Engine_ActorSetPosition
	movs r0, #6
	bl Object_GetById
	movs r4, #192
	lsls r4, r4, #6
	mov r10, r4
	mov r2, r10
	strh r2, [r0, #6]
	movs r1, #134
	movs r2, #154
	lsls r1, r1, #17
	lsls r2, r2, #16
	movs r0, #6
	bl Engine_ActorSetPosition
	movs r0, #21
	bl Object_GetById
	mov r3, r10
	movs r1, #134
	movs r2, #164
	strh r3, [r0, #6]
	lsls r1, r1, #17
	lsls r2, r2, #16
	movs r0, #21
	bl Engine_ActorSetPosition
	movs r0, #20
	bl Object_GetById
	adds r7, r0, #0
	adds r3, r7, #0
	movs r6, #10
	adds r3, #100
	strh r6, [r3]
	movs r3, #208
	lsls r3, r3, #8
	movs r1, #147
	movs r2, #212
	strh r3, [r7, #6]
	lsls r2, r2, #16
	movs r0, #20
	lsls r1, r1, #17
	bl Engine_ActorSetPosition
	movs r0, #20
	movs r1, #9
	bl Engine_ActorSetAnimation
	ldr r4, .L_0200a830
	mov r11, r4
	mov r1, r11
	movs r0, #20
	bl Engine_ActorEnableActionCallback
	movs r0, #20
	bl Object_GetById
	movs r1, #0
	bl Engine_ActorSetSpriteFlags
	movs r0, #19
	bl Object_GetById
	adds r7, r0, #0
	adds r3, r7, #0
	adds r3, #100
	movs r5, #0
	movs r1, #143
	movs r2, #192
	strh r6, [r3]
	lsls r2, r2, #16
	strh r5, [r7, #6]
	movs r0, #19
	lsls r1, r1, #17
	bl Engine_ActorSetPosition
	movs r0, #19
	movs r1, #7
	bl Engine_ActorSetAnimation
	mov r1, r11
	movs r0, #19
	bl Engine_ActorEnableActionCallback
	movs r0, #19
	bl Object_GetById
	movs r1, #0
	bl Engine_ActorSetSpriteFlags
	bl Engine_EventGetViewCenter
	mov r2, r9
	adds r0, #85
	strb r2, [r0]
	movs r1, #128
	movs r0, #152
	movs r2, #180
	movs r3, #0
	lsls r1, r1, #14
	lsls r2, r2, #16
	lsls r0, r0, #17
	bl Engine_CameraMoveTo
	movs r0, #1
	bl Engine_TaskWait
	bl Engine_MapRedraw
	movs r0, #1
	bl Engine_TaskWait
	movs r6, #128
	bl Engine_EventOpenScreen
	bl Engine_EventWaitForScreen
	movs r0, #80
	bl Engine_EventWait
	lsls r6, r6, #6
	movs r2, #20
	movs r0, #1
	movs r1, #2
	bl Engine_ActorJump
	adds r1, r6, #0
	movs r0, #1
	bl VinasuChojo_FaceActor
	movs r7, #160
	ldr r0, .L_0200a834
	bl Engine_EventSetMessage
	lsls r7, r7, #8
	ldr r0, .L_0200a838
	bl VinasuChojo_ShowMessage
	adds r1, r7, #0
	movs r0, #3
	bl VinasuChojo_FaceActor
	movs r0, #3
	bl VinasuChojo_ShowMessage
	movs r2, #0
	movs r0, #1
	mov r1, r8
	bl Engine_ActorFaceDirection
	adds r1, r7, #0
	movs r0, #2
	bl VinasuChojo_FaceActor
	movs r0, #21
	movs r1, #2
	bl Engine_ActorRunRepeatedMotion
	ldr r1, .L_0200a83c
	ldr r2, .L_0200a840
	movs r0, #21
	bl Engine_ActorSetSpeed
	movs r0, #21
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	movs r1, #140
	strb r3, [r0]
	lsls r1, r1, #1
	movs r2, #164
	movs r0, #21
	bl Engine_ActorWalkToAndWait
	movs r0, #1
	bl Engine_EventWait
	movs r0, #21
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	strb r3, [r0]
	movs r0, #20
	bl Engine_EventWait
	movs r1, #129
	movs r2, #40
	lsls r1, r1, #1
	movs r0, #2
	bl Engine_ActorShowEmote
	movs r0, #2
	bl VinasuChojo_ShowMessage
	movs r1, #4
	movs r0, #21
	bl Engine_ActorSetAnimationAndWait
	movs r0, #21
	bl VinasuChojo_ShowMessage
	movs r2, #20
	movs r0, #1
	ldr r1, .L_0200a844
	bl Engine_ActorShowEmote
	movs r1, #1
	movs r0, #1
	bl Engine_ActorRunRepeatedMotion
	movs r0, #1
	bl VinasuChojo_ShowMessage
	movs r0, #21
	movs r1, #1
	bl Engine_ActorRunRepeatedMotion
	movs r1, #0
	movs r0, #21
	bl VinasuChojo_FaceActor
	movs r0, #21
	bl VinasuChojo_ShowMessage
	movs r1, #2
	movs r0, #3
	bl Engine_ActorStartRepeatedMotion
	movs r0, #3
	bl VinasuChojo_ShowMessage
	movs r1, #4
	movs r0, #3
	bl Engine_ActorSetAnimation
	movs r0, #3
	bl VinasuChojo_ShowMessage
	movs r2, #40
	movs r0, #21
	ldr r1, .L_0200a848
	bl Engine_ActorShowEmote
	mov r1, r10
	movs r0, #21
	bl VinasuChojo_FaceActor
	movs r0, #21
	bl VinasuChojo_ShowMessage
	movs r2, #20
	movs r1, #2
	movs r0, #2
	bl Engine_ActorJump
	movs r0, #2
	bl VinasuChojo_ShowMessage
	movs r1, #3
	movs r0, #21
	bl Engine_ActorSetAnimationAndWait
	movs r0, #21
	bl VinasuChojo_ShowMessage
	movs r0, #1
	movs r1, #1
	bl Engine_ActorRunRepeatedMotion
	movs r0, #1
	adds r1, r6, #0
	bl VinasuChojo_FaceActor
	movs r1, #0
	movs r0, #1
	bl Engine_EventOpenMessage
	movs r1, #224
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #0
	bl Engine_ActorFaceDirection
	movs r1, #224
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #0
	bl Engine_ActorFaceDirection
	movs r0, #0
	movs r1, #0
	bl Engine_EventChooseYesNo
	movs r7, #1
	cmp r0, #0
	beq .L_0200a650
	ldr r3, .L_0200a84c
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	movs r7, #0
.L_0200a650:
	movs r0, #20
	bl Engine_TaskWait
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl Engine_ActorFaceDirection
	movs r1, #160
	movs r2, #0
	movs r0, #2
	lsls r1, r1, #8
	bl Engine_ActorFaceDirection
	movs r4, #160
	lsls r4, r4, #8
	mov r10, r4
	movs r0, #3
	mov r1, r10
	bl VinasuChojo_FaceActor
	movs r0, #21
	movs r1, #0
	bl VinasuChojo_FaceActor
	movs r0, #21
	movs r1, #0
	bl Engine_EventShowMessage
	movs r0, #17
	bl Engine_AudioPlayCue
	movs r0, #40
	bl Engine_EventWait
	cmp r7, #0
	beq .L_0200a6ac
	ldr r3, .L_0200a84c
	movs r0, #236
	ldr r2, [r3]
	lsls r0, r0, #1
	adds r2, r2, r0
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
.L_0200a6ac:
	movs r0, #20
	bl Engine_ActorStop
	movs r0, #19
	bl Engine_ActorStop
	movs r0, #1
	bl Engine_TaskWait
	movs r0, #20
	bl Object_GetById
	movs r2, #128
	lsls r2, r2, #9
	adds r7, r0, #0
	str r2, [r7, #24]
	str r2, [r7, #28]
	movs r0, #19
	mov r8, r2
	bl Object_GetById
	mov r3, r8
	adds r7, r0, #0
	str r3, [r7, #24]
	str r3, [r7, #28]
	movs r0, #1
	bl Engine_TaskWait
	movs r0, #20
	bl VinasuChojo_ShowMessage
	movs r1, #192
	movs r0, #21
	lsls r1, r1, #6
	movs r2, #0
	bl Engine_ActorFaceDirection
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #7
	movs r2, #0
	bl Engine_ActorFaceDirection
	movs r1, #192
	movs r0, #1
	lsls r1, r1, #7
	movs r2, #0
	bl Engine_ActorFaceDirection
	movs r1, #128
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #0
	bl Engine_ActorFaceDirection
	movs r1, #128
	movs r2, #40
	movs r0, #3
	lsls r1, r1, #8
	bl Engine_ActorFaceDirection
	movs r1, #1
	movs r0, #20
	bl Engine_ActorSetAnimation
	movs r0, #20
	bl Object_GetById
	movs r1, #1
	bl Engine_ActorSetSpriteFlags
	movs r0, #20
	bl Engine_EventWait
	movs r0, #20
	ldr r1, .L_0200a850
	ldr r2, .L_0200a854
	bl Engine_ActorSetSpeed
	movs r1, #150
	movs r2, #206
	lsls r1, r1, #1
	movs r0, #20
	bl Engine_ActorWalkToAndWait
	movs r0, #20
	bl Engine_EventWait
	movs r1, #2
	movs r0, #20
	bl Engine_ActorRunRepeatedMotion
	movs r0, #20
	bl Engine_EventWait
	movs r0, #20
	bl Object_GetById
	ldr r6, .L_0200a858
	str r6, [r0, #24]
	movs r0, #161
	bl Engine_AudioPlayCue
	movs r1, #8
	movs r0, #20
	bl Engine_ActorSetAnimation
	movs r0, #20
	bl Engine_EventWait
	movs r1, #1
	movs r0, #19
	bl Engine_ActorSetAnimation
	movs r0, #19
	bl Object_GetById
	movs r1, #1
	bl Engine_ActorSetSpriteFlags
	movs r0, #19
	movs r1, #4
	movs r2, #40
	bl Engine_ActorJump
	ldr r1, .L_0200a850
	ldr r2, .L_0200a854
	movs r0, #19
	bl Engine_ActorSetSpeed
	movs r0, #19
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	movs r1, #148
	strb r3, [r0]
	lsls r1, r1, #1
	movs r0, #19
	movs r2, #186
	bl Engine_ActorWalkToAndWait
	movs r0, #19
	ldr r1, .L_0200a854
	ldr r2, .L_0200a85c
	bl Engine_ActorSetSpeed
	movs r1, #146
	movs r2, #186
	lsls r1, r1, #1
	movs r0, #19
	bl Engine_ActorWalkToAndWait
	movs r0, #19
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	movs r4, #1
	orrs r3, r4
	strb r3, [r0]
	movs r0, #19
	bl Object_GetById
	str r6, [r0, #24]
	movs r0, #161
	bl Engine_AudioPlayCue
	movs r1, #5
	movs r0, #19
	bl Engine_ActorSetAnimation
	movs r0, #20
	bl Engine_EventWait
	movs r1, #2
	movs r0, #19
	bl Engine_ActorRunRepeatedMotion
	movs r0, #20
	bl Engine_EventWait
	movs r1, #1
	movs r0, #20
	bl Engine_ActorRunRepeatedMotion
	movs r0, #80
	bl Engine_EventWait
	movs r0, #3
	movs r1, #4
	b .L_0200a860
.L_0200a830:
	.4byte Data_0200e074
.L_0200a834:
	.4byte MsgVinasuBeatEm
.L_0200a838:
	.4byte 0x00001001
.L_0200a83c:
	.4byte 0x0000cccc
.L_0200a840:
	.4byte 0x00006666
.L_0200a844:
	.4byte 0x00000103
.L_0200a848:
	.4byte 0x00000105
.L_0200a84c:
	.4byte gEventWork
.L_0200a850:
	.4byte 0x00003333
.L_0200a854:
	.4byte 0x00001999
.L_0200a858:
	.4byte 0xffff0000
.L_0200a85c:
	.4byte 0x00000ccc
.L_0200a860:
	bl Engine_ActorSetAnimationAndWait
	movs r2, #20
	movs r0, #3
	movs r1, #0
	bl Engine_EventShowMessageAndWait
	movs r1, #2
	movs r0, #19
	bl Engine_ActorRunRepeatedMotion
	movs r0, #40
	bl Engine_EventWait
	movs r0, #19
	bl VinasuChojo_ShowMessage
	movs r0, #0
	mov r1, r10
	movs r2, #0
	bl Engine_ActorFaceDirection
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #6
	movs r2, #0
	bl Engine_ActorFaceDirection
	movs r1, #192
	movs r0, #2
	lsls r1, r1, #7
	movs r2, #0
	bl Engine_ActorFaceDirection
	movs r1, #224
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #40
	bl Engine_ActorFaceDirection
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #7
	movs r2, #0
	bl Engine_ActorFaceDirection
	movs r1, #192
	movs r0, #1
	lsls r1, r1, #7
	movs r2, #0
	bl Engine_ActorFaceDirection
	movs r1, #128
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #0
	bl Engine_ActorFaceDirection
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #20
	movs r0, #3
	bl Engine_ActorFaceDirection
	movs r0, #20
	bl Object_GetById
	movs r2, #192
	lsls r2, r2, #6
	adds r7, r0, #0
	mov r11, r2
	ldr r0, .L_0200a92c
	mov r4, r8
	mov r3, r11
	strh r3, [r7, #6]
	str r4, [r7, #24]
	movs r1, #1
	mov r9, r0
	movs r6, #208
	movs r0, #20
	bl Engine_ActorSetAnimation
	lsls r6, r6, #8
	movs r0, #10
	bl Engine_EventWait
	movs r0, #20
	adds r1, r6, #0
	bl VinasuChojo_FaceActor
	movs r1, #6
	movs r2, #0
	movs r0, #20
	bl Engine_ActorJump
	movs r0, #10
	bl Engine_EventWait
	movs r0, #0
	mov r1, r10
	movs r2, #0
	b .L_0200a930
.L_0200a92c:
	.4byte 0x00000000
.L_0200a930:
	bl Engine_ActorFaceDirection
	movs r0, #1
	mov r1, r10
	movs r2, #0
	bl Engine_ActorFaceDirection
	movs r0, #2
	mov r1, r10
	movs r2, #0
	bl Engine_ActorFaceDirection
	movs r0, #3
	mov r1, r10
	movs r2, #0
	bl Engine_ActorFaceDirection
	movs r0, #21
	adds r1, r6, #0
	movs r2, #0
	bl Engine_ActorFaceDirection
	movs r0, #6
	movs r1, #0
	movs r2, #0
	bl Engine_ActorFaceDirection
	movs r0, #128
	ldr r2, [r7, #12]
	lsls r0, r0, #12
	mov r8, r0
	ldr r1, [r7, #8]
	add r2, r8
	ldr r3, [r7, #16]
	movs r0, #22
	bl Engine_ObjectCreate
	adds r5, r0, #0
	cmp r5, #0
	beq .L_0200a9f2
	ldr r6, [r5, #80]
	adds r3, r6, #0
	adds r3, #39
	mov r2, r9
	strb r2, [r3]
	movs r3, #33
	ldrb r2, [r6, #5]
	negs r3, r3
	ands r3, r2
	ldrb r2, [r6, #9]
	strb r3, [r6, #5]
	movs r3, #15
	ands r3, r2
	adds r1, r5, #0
	movs r2, #13
	adds r1, #35
	negs r2, r2
	ands r3, r2
	ldrb r2, [r1]
	strb r3, [r6, #9]
	movs r3, #254
	ands r3, r2
	strb r3, [r1]
	adds r3, r5, #0
	mov r4, r9
	adds r3, #85
	adds r2, r5, #0
	strb r4, [r3]
	adds r2, #92
	movs r3, #1
	strb r3, [r2]
	ldr r3, .L_0200ac58
	str r3, [r5, #48]
	ldr r3, .L_0200ac5c
	movs r1, #193
	str r3, [r5, #52]
	lsls r1, r1, #3
	movs r0, #17
	bl Runtime_AllocateHeapBlock
	str r0, [sp, #40]
	movs r0, #220
	bl ItemIcon_LoadTiles
	movs r3, #128
	ldr r2, [sp, #40]
	lsls r3, r3, #3
	adds r3, r2, r3
	ldrb r0, [r6, #28]
	movs r1, #128
	adds r2, r3, #0
	str r3, [sp, #36]
	bl VramBlock_LoadCached
	movs r0, #17
	bl Runtime_ReleaseHeapBlock
.L_0200a9f2:
	movs r1, #1
	movs r0, #22
	bl Engine_ActorSetSpritePriority
	movs r0, #22
	bl Object_GetById
	movs r6, #128
	ldr r3, [r7, #8]
	lsls r6, r6, #14
	str r3, [r0, #8]
	str r6, [r0, #12]
	ldr r3, [r7, #16]
	str r3, [r0, #16]
	adds r3, r0, #0
	adds r3, #85
	movs r1, #3
	strb r1, [r3]
	ldr r3, .L_0200ac58
	ldr r2, .L_0200ac5c
	str r3, [r0, #48]
	movs r3, #192
	lsls r3, r3, #8
	str r2, [r0, #52]
	str r3, [r0, #24]
	str r3, [r0, #28]
	cmp r5, #0
	beq .L_0200aa4a
	adds r3, r5, #0
	adds r3, #85
	strb r1, [r3]
	ldr r3, .L_0200ac60
	mov r4, r8
	str r3, [r5, #72]
	movs r1, #154
	movs r3, #164
	str r2, [r5, #68]
	str r4, [r5, #40]
	adds r0, r5, #0
	lsls r1, r1, #17
	adds r2, r6, #0
	lsls r3, r3, #16
	bl Object_SetMoveTarget
.L_0200aa4a:
	movs r1, #154
	movs r0, #22
	lsls r1, r1, #1
	movs r2, #164
	bl Engine_ActorMoveToAndWait
	movs r0, #22
	movs r1, #0
	movs r2, #0
	bl Engine_ActorSetPosition
	movs r0, #21
	movs r1, #0
	bl Engine_ActorSetSpritePriority
	cmp r5, #0
	beq .L_0200aafc
	ldr r0, .L_0200ac64
	bl Engine_AudioPlayCue
	adds r0, r5, #0
	movs r1, #0
	bl Engine_ActorSetSpriteFlags
	movs r3, #128
	lsls r3, r3, #11
	str r3, [r5, #40]
	movs r1, #157
	movs r3, #137
	lsls r1, r1, #17
	adds r2, r6, #0
	lsls r3, r3, #16
	adds r0, r5, #0
	bl Object_SetMoveTarget
	adds r0, r5, #0
	bl Script_WaitForEventTimeout
	ldr r0, .L_0200ac64
	bl Engine_AudioPlayCue
	ldr r1, [r5, #80]
	movs r3, #13
	ldrb r2, [r1, #9]
	negs r3, r3
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	strb r3, [r1, #9]
	movs r3, #192
	lsls r3, r3, #11
	str r3, [r5, #40]
	movs r3, #146
	ldr r1, .L_0200ac68
	adds r2, r6, #0
	lsls r3, r3, #16
	adds r0, r5, #0
	bl Object_SetMoveTarget
	adds r0, r5, #0
	bl Script_WaitForEventTimeout
	ldr r0, .L_0200ac64
	bl Engine_AudioPlayCue
	movs r3, #160
	lsls r3, r3, #11
	str r3, [r5, #40]
	movs r1, #150
	movs r3, #154
	lsls r1, r1, #17
	adds r2, r6, #0
	lsls r3, r3, #16
	adds r0, r5, #0
	bl Object_SetMoveTarget
	adds r0, r5, #0
	bl Script_WaitForEventTimeout
	movs r0, #6
	bl Engine_TaskWait
	movs r0, #0
	str r0, [r5, #8]
	str r0, [r5, #12]
	str r0, [r5, #16]
	adds r0, r5, #0
	bl SceneActor_ParkRecord
.L_0200aafc:
	movs r1, #128
	movs r0, #21
	lsls r1, r1, #1
	movs r2, #0
	bl Engine_ActorShowEmote
	movs r1, #128
	movs r0, #6
	lsls r1, r1, #1
	movs r2, #0
	bl Engine_ActorShowEmote
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #0
	bl Engine_ActorShowEmote
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #1
	movs r2, #0
	bl Engine_ActorShowEmote
	movs r1, #128
	movs r0, #2
	lsls r1, r1, #1
	movs r2, #0
	bl Engine_ActorShowEmote
	movs r1, #128
	movs r2, #30
	movs r0, #3
	lsls r1, r1, #1
	bl Engine_ActorShowEmote
	movs r1, #1
	movs r0, #21
	bl Engine_ActorSetSpritePriority
	movs r0, #21
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r2, #1
	orrs r3, r2
	strb r3, [r0]
	movs r1, #2
	movs r0, #2
	bl Engine_ActorStartRepeatedMotion
	movs r0, #2
	bl VinasuChojo_ShowMessage
	movs r0, #3
	movs r1, #1
	bl Engine_ActorRunRepeatedMotion
	movs r2, #40
	movs r0, #3
	movs r1, #0
	bl Engine_EventShowMessageAndWait
	movs r1, #1
	movs r0, #20
	bl Engine_ActorRunRepeatedMotion
	movs r0, #20
	bl Engine_EventWait
	movs r0, #20
	bl VinasuChojo_ShowMessage
	movs r1, #160
	movs r0, #0
	lsls r1, r1, #7
	movs r2, #0
	bl Engine_ActorFaceDirection
	movs r1, #160
	movs r0, #1
	lsls r1, r1, #7
	movs r2, #0
	bl Engine_ActorFaceDirection
	movs r1, #128
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #0
	bl Engine_ActorFaceDirection
	movs r1, #128
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #0
	bl Engine_ActorFaceDirection
	movs r2, #0
	movs r0, #6
	mov r1, r11
	bl Engine_ActorFaceDirection
	movs r0, #21
	mov r1, r11
	bl VinasuChojo_FaceActor
	movs r0, #21
	ldr r1, .L_0200ac6c
	movs r2, #0
	bl Engine_ActorShowEmote
	movs r0, #6
	ldr r1, .L_0200ac6c
	movs r2, #0
	bl Engine_ActorShowEmote
	movs r0, #0
	ldr r1, .L_0200ac6c
	movs r2, #0
	bl Engine_ActorShowEmote
	movs r0, #1
	ldr r1, .L_0200ac6c
	movs r2, #0
	bl Engine_ActorShowEmote
	movs r0, #2
	ldr r1, .L_0200ac6c
	movs r2, #0
	bl Engine_ActorShowEmote
	movs r0, #3
	ldr r1, .L_0200ac6c
	movs r2, #60
	bl Engine_ActorShowEmote
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #40
	movs r0, #20
	bl Engine_ActorShowEmote
	movs r0, #20
	bl VinasuChojo_ShowMessage
	movs r0, #0
	mov r1, r10
	movs r2, #0
	bl Engine_ActorFaceDirection
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #6
	movs r2, #40
	bl Engine_ActorFaceDirection
	movs r1, #0
	movs r0, #1
	bl Engine_EventOpenMessage
	movs r0, #0
	movs r1, #0
	bl Engine_EventChooseYesNo
	cmp r0, #0
	bne .L_0200ac70
	movs r0, #1
	movs r1, #3
	bl Engine_ActorSetAnimation
	movs r7, #1
	b .L_0200ac8a
	.2byte 0x0000
.L_0200ac58:
	.4byte 0x00019999
.L_0200ac5c:
	.4byte 0x0000cccc
.L_0200ac60:
	.4byte 0x00009999
.L_0200ac64:
	.4byte 0x00000135
.L_0200ac68:
	.4byte 0x011d0000
.L_0200ac6c:
	.4byte 0x00000101
.L_0200ac70:
	ldr r3, .L_0200b064
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	movs r0, #1
	movs r1, #4
	bl Engine_ActorSetAnimation
	movs r7, #0
.L_0200ac8a:
	movs r0, #1
	bl VinasuChojo_ShowMessage
	cmp r7, #0
	beq .L_0200aca4
	ldr r3, .L_0200b064
	movs r4, #236
	ldr r2, [r3]
	lsls r4, r4, #1
	adds r2, r2, r4
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
.L_0200aca4:
	movs r0, #20
	bl Engine_EventWait
	movs r0, #24
	bl Object_GetById
	movs r1, #0
	bl Engine_ActorSetSpriteFlags
	movs r1, #7
	movs r0, #24
	bl Engine_ActorSetChildValue
	movs r0, #24
	bl Object_GetById
	movs r4, #0
	adds r7, r0, #0
	ldr r0, .L_0200b068
	mov r8, r4
	ldr r2, .L_0200b06c
	adds r3, r7, #0
	str r0, [r7, #28]
	mov r9, r0
	adds r3, #85
	mov r0, r8
	str r2, [r7, #24]
	mov r10, r2
	strb r0, [r3]
	movs r6, #152
	movs r3, #128
	movs r2, #158
	lsls r2, r2, #16
	lsls r3, r3, #15
	lsls r6, r6, #17
	str r3, [r7, #12]
	str r2, [r7, #16]
	str r6, [r7, #8]
	movs r0, #25
	mov r11, r2
	bl Object_GetById
	movs r1, #0
	bl Engine_ActorSetSpriteFlags
	movs r1, #7
	movs r0, #25
	bl Engine_ActorSetChildValue
	movs r0, #25
	bl Object_GetById
	mov r3, r9
	adds r7, r0, #0
	str r3, [r7, #28]
	adds r3, r7, #0
	mov r4, r10
	mov r0, r8
	adds r3, #85
	str r4, [r7, #24]
	strb r0, [r3]
	movs r3, #192
	lsls r3, r3, #15
	mov r2, r11
	movs r1, #128
	str r3, [r7, #12]
	str r6, [r7, #8]
	str r2, [r7, #16]
	movs r0, #21
	lsls r1, r1, #1
	movs r2, #0
	bl Engine_ActorShowEmote
	movs r1, #128
	movs r0, #6
	lsls r1, r1, #1
	movs r2, #0
	bl Engine_ActorShowEmote
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #0
	bl Engine_ActorShowEmote
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #1
	movs r2, #0
	bl Engine_ActorShowEmote
	movs r1, #128
	movs r0, #2
	lsls r1, r1, #1
	movs r2, #0
	bl Engine_ActorShowEmote
	movs r1, #128
	movs r0, #3
	lsls r1, r1, #1
	movs r2, #0
	bl Engine_ActorShowEmote
	movs r1, #160
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl Engine_ActorFaceDirection
	movs r1, #160
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #0
	bl Engine_ActorFaceDirection
	movs r1, #160
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #0
	bl Engine_ActorFaceDirection
	movs r1, #208
	movs r0, #21
	lsls r1, r1, #8
	movs r2, #0
	bl Engine_ActorFaceDirection
	movs r2, #0
	movs r0, #6
	movs r1, #0
	bl Engine_ActorFaceDirection
	ldr r6, .L_0200b070
	movs r0, #24
	adds r1, r6, #0
	bl Engine_ActorEnableActionCallback
	adds r1, r6, #0
	movs r0, #25
	bl Engine_ActorEnableActionCallback
	movs r0, #145
	bl Engine_AudioPlayCue
	movs r0, #192
	movs r1, #192
	movs r2, #128
	lsls r2, r2, #9
	lsls r0, r0, #11
	lsls r1, r1, #11
	bl Engine_WorkSetValuesIfNonNegative
	movs r1, #0
	ldr r0, .L_0200b074
	bl Engine_ColorBufferApplyTarget
	movs r0, #16
	bl Engine_ColorBufferInterpolate
	movs r0, #20
	bl Engine_TaskWait
	movs r1, #0
	ldr r0, .L_0200b078
	bl Engine_ColorBufferApplyTarget
	movs r0, #24
	bl Engine_ColorBufferInterpolate
	movs r0, #60
	bl Engine_TaskWait
	movs r1, #200
	lsls r1, r1, #4
	ldr r0, .L_0200b07c
	bl Engine_TaskAddCallback
	movs r0, #141
	bl Engine_AudioPlayCue
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #9
	lsls r0, r0, #11
	lsls r1, r1, #11
	bl Engine_WorkSetValuesIfNonNegative
	movs r1, #0
	ldr r0, .L_0200b074
	bl Engine_ColorBufferApplyTarget
	movs r0, #120
	bl Engine_ColorBufferInterpolate
	ldr r6, .L_0200b080
	movs r0, #24
	adds r1, r6, #0
	bl Engine_ActorEnableActionCallback
	adds r1, r6, #0
	movs r0, #25
	bl Engine_ActorEnableActionCallback
	movs r0, #120
	bl Engine_TaskWait
	movs r0, #192
	movs r1, #192
	movs r2, #128
	lsls r2, r2, #9
	lsls r0, r0, #10
	lsls r1, r1, #10
	bl Engine_WorkSetValuesIfNonNegative
	movs r1, #0
	ldr r0, .L_0200b084
	bl Engine_ColorBufferApplyTarget
	movs r0, #120
	bl Engine_ColorBufferInterpolate
	movs r0, #120
	bl Engine_TaskWait
	movs r0, #63
	bl Engine_AudioPlayCue
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #9
	lsls r0, r0, #10
	lsls r1, r1, #10
	bl Engine_WorkSetValuesIfNonNegative
	movs r0, #128
	movs r1, #0
	lsls r0, r0, #9
	bl Engine_ColorBufferApplyTarget
	movs r0, #120
	bl Engine_ColorBufferInterpolate
	movs r0, #120
	bl Engine_TaskWait
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #9
	lsls r0, r0, #9
	lsls r1, r1, #9
	bl Engine_WorkSetValuesIfNonNegative
	movs r0, #19
	movs r1, #1
	bl Engine_ActorSetAnimation
	movs r1, #0
	movs r2, #0
	movs r0, #19
	bl Engine_ActorFaceDirection
	movs r0, #19
	bl Object_GetById
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r0, #24]
	movs r2, #40
	movs r0, #19
	movs r1, #4
	bl Engine_ActorJump
	movs r0, #19
	movs r1, #1
	bl Engine_ActorRunRepeatedMotion
	movs r0, #19
	movs r1, #0
	movs r2, #20
	bl Engine_EventShowMessageAndWait
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #7
	movs r2, #0
	bl Engine_ActorFaceDirection
	movs r1, #192
	movs r0, #1
	lsls r1, r1, #7
	movs r2, #0
	bl Engine_ActorFaceDirection
	movs r1, #128
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #0
	bl Engine_ActorFaceDirection
	movs r1, #128
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #0
	bl Engine_ActorFaceDirection
	movs r1, #192
	movs r0, #21
	lsls r1, r1, #6
	movs r2, #0
	bl Engine_ActorFaceDirection
	movs r1, #192
	movs r2, #20
	movs r0, #6
	lsls r1, r1, #6
	bl Engine_ActorFaceDirection
	movs r1, #1
	movs r0, #20
	bl Engine_ActorRunRepeatedMotion
	movs r6, #128
	movs r0, #20
	lsls r6, r6, #8
	bl VinasuChojo_ShowMessage
	movs r0, #3
	movs r1, #1
	bl Engine_ActorRunRepeatedMotion
	adds r1, r6, #0
	movs r0, #3
	bl VinasuChojo_FaceActor
	movs r0, #3
	bl VinasuChojo_ShowMessage
	movs r0, #20
	movs r1, #0
	bl VinasuChojo_FaceActor
	movs r1, #4
	movs r0, #20
	bl Engine_ActorSetAnimationAndWait
	movs r0, #20
	bl VinasuChojo_ShowMessage
	movs r2, #20
	movs r0, #19
	ldr r1, .L_0200b088
	bl Engine_ActorShowEmote
	movs r1, #2
	movs r0, #19
	bl Engine_ActorStartRepeatedMotion
	movs r0, #19
	bl VinasuChojo_ShowMessage
	movs r1, #128
	movs r2, #20
	movs r0, #1
	lsls r1, r1, #1
	bl Engine_ActorShowEmote
	movs r4, #192
	lsls r4, r4, #7
	mov r11, r4
	mov r1, r11
	movs r0, #1
	bl VinasuChojo_FaceActor
	movs r0, #1
	bl VinasuChojo_ShowMessage
	movs r1, #208
	movs r0, #20
	lsls r1, r1, #8
	bl VinasuChojo_FaceActor
	movs r0, #20
	movs r1, #3
	bl Engine_ActorSetAnimationAndWait
	movs r2, #20
	movs r0, #20
	movs r1, #0
	bl Engine_EventShowMessageAndWait
	movs r1, #1
	movs r0, #1
	bl Engine_ActorRunRepeatedMotion
	movs r0, #1
	bl VinasuChojo_ShowMessage
	movs r1, #176
	movs r2, #20
	movs r0, #19
	lsls r1, r1, #8
	bl Engine_ActorFaceDirection
	movs r1, #1
	movs r0, #19
	bl Engine_ActorRunRepeatedMotion
	ldr r0, .L_0200b08c
	mov r8, r0
	bl VinasuChojo_ShowMessage
	movs r0, #0
	movs r1, #1
	bl Engine_ActorStartRepeatedMotion
	movs r0, #1
	movs r1, #1
	bl Engine_ActorStartRepeatedMotion
	movs r0, #2
	movs r1, #1
	bl Engine_ActorStartRepeatedMotion
	movs r0, #3
	movs r1, #1
	bl Engine_ActorRunRepeatedMotion
	adds r1, r6, #0
	movs r0, #0
	movs r2, #0
	bl Engine_ActorFaceDirection
	adds r1, r6, #0
	movs r0, #1
	movs r2, #0
	bl Engine_ActorFaceDirection
	movs r1, #160
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #0
	bl Engine_ActorFaceDirection
	movs r2, #160
	lsls r2, r2, #8
	mov r9, r2
	mov r1, r9
	movs r0, #3
	bl VinasuChojo_FaceActor
	ldr r1, .L_0200b090
	movs r2, #80
	movs r0, #21
	bl Engine_ActorShowEmote
	movs r0, #21
	bl VinasuChojo_ShowMessage
	mov r1, r9
	movs r0, #0
	movs r2, #0
	bl Engine_ActorFaceDirection
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #6
	movs r2, #0
	bl Engine_ActorFaceDirection
	movs r0, #2
	mov r1, r11
	movs r2, #0
	bl Engine_ActorFaceDirection
	movs r1, #224
	movs r2, #40
	b .L_0200b094
.L_0200b064:
	.4byte gEventWork
.L_0200b068:
	.4byte 0xffff0000
.L_0200b06c:
	.4byte 0x00001999
.L_0200b070:
	.4byte Data_0200e088
.L_0200b074:
	.4byte 0x004063ff
.L_0200b078:
	.4byte 0x00007fff
.L_0200b07c:
	.4byte SceneEffect_SpawnParticlesAboveActor
.L_0200b080:
	.4byte Data_020060ac
.L_0200b084:
	.4byte 0x00203210
.L_0200b088:
	.4byte 0x00000103
.L_0200b08c:
	.4byte 0x00002013
.L_0200b090:
	.4byte 0x00000101
.L_0200b094:
	movs r0, #3
	lsls r1, r1, #8
	bl Engine_ActorFaceDirection
	movs r1, #1
	movs r0, #20
	bl Engine_ActorRunRepeatedMotion
	movs r0, #20
	bl VinasuChojo_ShowMessage
	adds r1, r6, #0
	movs r0, #0
	movs r2, #0
	bl Engine_ActorFaceDirection
	adds r1, r6, #0
	movs r0, #1
	movs r2, #0
	bl Engine_ActorFaceDirection
	movs r7, #176
	adds r1, r6, #0
	movs r0, #2
	movs r2, #0
	bl Engine_ActorFaceDirection
	lsls r7, r7, #8
	movs r2, #20
	adds r1, r6, #0
	movs r0, #3
	bl Engine_ActorFaceDirection
	adds r1, r7, #0
	movs r0, #20
	bl VinasuChojo_FaceActor
	ldr r3, .L_0200b4d0
	mov r10, r3
	mov r0, r10
	bl VinasuChojo_ShowMessage
	movs r0, #21
	movs r1, #3
	bl Engine_ActorSetAnimationAndWait
	adds r1, r7, #0
	movs r0, #21
	movs r2, #60
	bl Engine_ActorFaceDirection
	movs r1, #192
	movs r2, #40
	movs r0, #21
	lsls r1, r1, #6
	bl Engine_ActorFaceDirection
	movs r1, #1
	movs r0, #19
	bl Engine_ActorRunRepeatedMotion
	mov r0, r8
	bl VinasuChojo_ShowMessage
	movs r1, #0
	movs r2, #40
	movs r0, #21
	bl Engine_ActorFaceDirection
	movs r0, #21
	bl VinasuChojo_ShowMessage
	movs r1, #208
	movs r2, #40
	movs r0, #20
	lsls r1, r1, #8
	bl Engine_ActorFaceDirection
	adds r1, r7, #0
	movs r0, #20
	bl VinasuChojo_FaceActor
	mov r0, r10
	bl VinasuChojo_ShowMessage
	movs r1, #192
	movs r0, #21
	lsls r1, r1, #6
	movs r2, #40
	bl Engine_ActorFaceDirection
	movs r1, #192
	movs r2, #20
	movs r0, #21
	lsls r1, r1, #6
	bl Engine_ActorFaceDirection
	movs r0, #21
	movs r1, #3
	bl Engine_ActorSetAnimationAndWait
	movs r0, #19
	movs r1, #0
	movs r2, #40
	bl Engine_ActorFaceDirection
	adds r1, r7, #0
	movs r0, #19
	movs r2, #20
	bl Engine_ActorFaceDirection
	movs r2, #40
	ldr r1, .L_0200b4d4
	movs r0, #19
	bl Engine_ActorShowEmote
	mov r0, r8
	bl VinasuChojo_ShowMessage
	movs r1, #192
	movs r0, #21
	lsls r1, r1, #6
	bl VinasuChojo_FaceActor
	ldr r1, .L_0200b4d4
	movs r2, #20
	movs r0, #21
	bl Engine_ActorShowEmote
	movs r0, #21
	bl VinasuChojo_ShowMessage
	movs r2, #20
	adds r1, r7, #0
	movs r0, #20
	bl Engine_ActorFaceDirection
	movs r1, #4
	movs r0, #19
	bl Engine_ActorSetAnimation
	mov r0, r8
	bl VinasuChojo_ShowMessage
	movs r2, #80
	ldr r1, .L_0200b4d8
	movs r0, #21
	bl Engine_ActorShowEmote
	movs r0, #21
	bl VinasuChojo_ShowMessage
	movs r1, #3
	movs r0, #19
	bl Engine_ActorSetAnimationAndWait
	mov r0, r8
	bl VinasuChojo_ShowMessage
	movs r2, #20
	movs r0, #21
	ldr r1, .L_0200b4d8
	bl Engine_ActorShowEmote
	movs r1, #2
	movs r0, #21
	bl Engine_ActorStartRepeatedMotion
	movs r0, #21
	bl VinasuChojo_ShowMessage
	movs r1, #4
	movs r0, #19
	bl Engine_ActorSetAnimation
	mov r0, r8
	bl VinasuChojo_ShowMessage
	movs r1, #129
	movs r2, #60
	movs r0, #21
	lsls r1, r1, #1
	bl Engine_ActorShowEmote
	movs r1, #1
	movs r0, #21
	bl Engine_ActorRunRepeatedMotion
	movs r0, #21
	bl VinasuChojo_ShowMessage
	movs r1, #132
	movs r2, #40
	movs r0, #20
	lsls r1, r1, #1
	bl Engine_ActorShowEmote
	movs r0, #20
	movs r1, #4
	bl Engine_ActorSetAnimation
	movs r2, #40
	mov r0, r10
	movs r1, #0
	bl Engine_EventShowMessageAndWait
	movs r1, #1
	movs r0, #21
	bl Engine_ActorRunRepeatedMotion
	movs r0, #20
	bl Engine_EventWait
	movs r0, #21
	bl VinasuChojo_ShowMessage
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #1
	movs r2, #0
	bl Engine_ActorShowEmote
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #1
	movs r2, #0
	bl Engine_ActorShowEmote
	movs r1, #128
	movs r0, #2
	lsls r1, r1, #1
	movs r2, #0
	bl Engine_ActorShowEmote
	movs r1, #128
	movs r0, #3
	lsls r1, r1, #1
	movs r2, #0
	bl Engine_ActorShowEmote
	mov r1, r9
	movs r0, #2
	movs r2, #0
	bl Engine_ActorFaceDirection
	mov r1, r9
	movs r0, #3
	movs r2, #20
	bl Engine_ActorFaceDirection
	movs r1, #192
	movs r2, #20
	movs r0, #19
	lsls r1, r1, #6
	bl Engine_ActorFaceDirection
	movs r0, #19
	movs r1, #3
	bl Engine_ActorSetAnimation
	movs r0, #20
	movs r1, #3
	bl Engine_ActorSetAnimationAndWait
	adds r1, r7, #0
	movs r2, #20
	movs r0, #21
	bl Engine_ActorFaceDirection
	ldr r0, .L_0200b4dc
	bl VinasuChojo_ShowMessage
	movs r1, #128
	adds r2, r6, #0
	movs r0, #6
	lsls r1, r1, #9
	bl Engine_ActorSetSpeed
	movs r1, #128
	adds r2, r6, #0
	movs r0, #21
	lsls r1, r1, #9
	bl Engine_ActorSetSpeed
	ldr r7, .L_0200b4e0
	movs r0, #21
	adds r1, r7, #0
	bl Engine_ActorEnableActionCallback
	movs r0, #20
	bl Engine_EventWait
	adds r1, r7, #0
	movs r0, #6
	bl Engine_ActorEnableActionCallback
	movs r0, #1
	mov r1, r11
	bl VinasuChojo_FaceActor
	movs r1, #2
	movs r0, #1
	bl Engine_ActorStartRepeatedMotion
	movs r0, #1
	bl VinasuChojo_ShowMessage
	movs r0, #17
	bl Engine_AudioPlayCue
	movs r1, #1
	ldr r0, .L_0200b4e4
	bl Engine_ColorBufferApplyTarget
	movs r0, #40
	bl Engine_ColorBufferInterpolate
	movs r1, #208
	lsls r1, r1, #8
	movs r0, #20
	bl VinasuChojo_FaceActor
	movs r0, #20
	bl VinasuChojo_ShowMessage
	movs r0, #0
	mov r1, r11
	movs r2, #0
	bl Engine_ActorFaceDirection
	adds r1, r6, #0
	movs r0, #2
	movs r2, #0
	bl Engine_ActorFaceDirection
	movs r2, #0
	adds r1, r6, #0
	movs r0, #3
	bl Engine_ActorFaceDirection
	bl VinasuChojo_FlashScreen
	movs r0, #20
	movs r1, #7
	bl Engine_ActorSetChildValue
	movs r1, #7
	movs r0, #19
	bl Engine_ActorSetChildValue
	movs r0, #20
	bl Engine_EventWait
	movs r1, #128
	movs r0, #20
	lsls r1, r1, #1
	bl Engine_ActorSetChildValue
	movs r1, #128
	lsls r1, r1, #1
	movs r0, #19
	bl Engine_ActorSetChildValue
	movs r0, #20
	bl Engine_EventWait
	bl VinasuChojo_FlashScreen
	movs r2, #20
	movs r1, #2
	movs r0, #3
	bl Engine_ActorJump
	movs r0, #3
	bl VinasuChojo_ShowMessage
	movs r1, #0
	movs r0, #19
	bl VinasuChojo_FaceActor
	movs r0, #19
	bl VinasuChojo_ShowMessage
	bl VinasuChojo_FlashScreen
	movs r0, #19
	bl Object_GetById
	add r4, sp, #96
	movs r3, #7
	mov r10, r0
	str r3, [r4, #4]
	movs r0, #128
	ldr r3, .L_0200b4e8
	movs r2, #84
	lsls r0, r0, #9
	add r2, sp
	mov r9, r2
	str r3, [r4, #36]
	str r0, [r4, #8]
	str r0, [r4, #12]
	mov r8, r4
	movs r7, #0
	mov r6, r9
.L_0200b3bc:
	lsls r3, r7, #12
	adds r0, r3, #0
	str r3, [sp, #32]
	bl Engine_MathCos
	movs r3, #0
	str r3, [r6, #4]
	str r0, [r6]
	ldr r0, [sp, #32]
	bl Engine_MathSin
	ldr r3, [r6]
	lsls r2, r3, #1
	adds r3, r3, r2
	lsls r0, r0, #1
	str r0, [r6, #8]
	str r3, [r6]
	mov r4, r10
	ldr r4, [r4, #8]
	str r4, [sp, #28]
	mov r2, r10
	ldr r1, [r2, #12]
	ldr r4, [r6, #4]
	ldr r2, [r2, #16]
	str r0, [sp, #4]
	ldr r0, .L_0200b4ec
	str r4, [sp, #0]
	str r0, [sp, #8]
	mov r4, r8
	ldr r0, [sp, #28]
	adds r7, #1
	str r4, [sp, #12]
	bl Effect_Spawn
	cmp r7, #16
	bls .L_0200b3bc
	movs r0, #212
	bl Engine_AudioPlayCue
	movs r0, #6
	bl Engine_TaskWait
	bl VinasuChojo_FlashScreen
	movs r0, #20
	bl Object_GetById
	mov r10, r0
	add r0, sp, #44
	movs r3, #7
	str r3, [r0, #4]
	ldr r3, .L_0200b4e8
	str r3, [r0, #36]
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r0, #8]
	str r3, [r0, #12]
	mov r8, r0
	movs r7, #0
	mov r6, r9
.L_0200b434:
	lsls r2, r7, #12
	adds r0, r2, #0
	str r2, [sp, #24]
	bl Engine_MathCos
	movs r3, #0
	str r3, [r6, #4]
	str r0, [r6]
	ldr r0, [sp, #24]
	bl Engine_MathSin
	ldr r3, [r6]
	lsls r2, r3, #1
	adds r3, r3, r2
	lsls r0, r0, #1
	str r0, [r6, #8]
	str r3, [r6]
	mov r4, r10
	ldr r4, [r4, #8]
	str r4, [sp, #20]
	mov r2, r10
	ldr r1, [r2, #12]
	ldr r4, [r6, #4]
	ldr r2, [r2, #16]
	str r0, [sp, #4]
	ldr r0, .L_0200b4ec
	str r4, [sp, #0]
	str r0, [sp, #8]
	mov r4, r8
	ldr r0, [sp, #20]
	adds r7, #1
	str r4, [sp, #12]
	bl Effect_Spawn
	cmp r7, #16
	bls .L_0200b434
	movs r0, #212
	bl Engine_AudioPlayCue
	movs r2, #20
	movs r1, #6
	movs r0, #2
	bl Engine_ActorJump
	movs r0, #54
	bl Engine_AudioPlayCue
	movs r0, #2
	bl VinasuChojo_ShowMessage
	movs r1, #4
	movs r0, #20
	bl Engine_ActorSetAnimation
	movs r0, #20
	bl VinasuChojo_ShowMessage
	bl VinasuChojo_FlashScreen
	movs r0, #20
	ldr r1, .L_0200b4f0
	ldr r2, .L_0200b4f4
	bl Engine_ActorSetSpeed
	ldr r1, .L_0200b4f0
	ldr r2, .L_0200b4f4
	movs r0, #19
	bl Engine_ActorSetSpeed
	movs r0, #20
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r6, #254
	adds r3, r6, #0
	b .L_0200b4f8
	.2byte 0x0000
.L_0200b4d0:
	.4byte 0x00002014
.L_0200b4d4:
	.4byte 0x00000101
.L_0200b4d8:
	.4byte 0x00000103
.L_0200b4dc:
	.4byte 0x0000a015
.L_0200b4e0:
	.4byte Data_0200622c
.L_0200b4e4:
	.4byte 0x0040250d
.L_0200b4e8:
	.4byte Effect_MoveWithDrag
.L_0200b4ec:
	.4byte 0x01090000
.L_0200b4f0:
	.4byte 0x00003333
.L_0200b4f4:
	.4byte 0x00001999
.L_0200b4f8:
	ands r3, r2
	strb r3, [r0]
	movs r0, #19
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	movs r2, #254
	ands r2, r3
	movs r1, #147
	str r6, [sp, #16]
	lsls r1, r1, #1
	strb r2, [r0]
	movs r0, #20
	movs r2, #196
	bl Engine_ActorSetDestination
	movs r1, #147
	movs r2, #196
	movs r0, #19
	lsls r1, r1, #1
	bl Engine_ActorSetDestination
	movs r1, #200
	lsls r1, r1, #4
	ldr r0, .L_0200b5ac
	bl Engine_TaskAddCallback
	movs r0, #1
	movs r1, #2
	bl Engine_ActorStartRepeatedMotion
	movs r1, #0
	movs r0, #1
	bl Engine_EventShowMessage
	movs r0, #141
	lsls r0, r0, #2
	bl Engine_GameFlagSet
	movs r1, #0
	movs r0, #2
	bl Engine_EventShowMessage
	ldr r0, .L_0200b5b0
	bl Engine_GameFlagSet
	bl VinasuChojo_FlashScreen
	movs r0, #20
	bl Engine_EventWait
	bl VinasuChojo_FlashScreen
	movs r0, #20
	bl Engine_EventWait
	ldr r3, .L_0200b5b4
	ldr r4, .L_0200b5b8
	movs r2, #3
	adds r3, r3, r4
	strb r2, [r3]
	ldr r6, .L_0200b5bc
	movs r1, #3
	adds r0, r6, #0
	bl Party_SetFields1ceAnd1d0
	adds r0, r6, #0
	movs r1, #9
	bl Event_SetPair1d4
	movs r1, #0
	movs r0, #98
	bl BattleFx_SetWeightedResult
	bl Owner_RefreshRatiosOnFlag
	ldr r0, .L_0200b5c0
	bl Engine_GameFlagSet
	add sp, #136
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
.L_0200b5ac:
	.4byte VinasuChojo_UpdateBeamActors
.L_0200b5b0:
	.4byte 0x00000235
.L_0200b5b4:
	.4byte gCell
.L_0200b5b8:
	.4byte 0x0000022b
.L_0200b5bc:
	.4byte 0x000000bb
.L_0200b5c0:
	.4byte 0x00000351
	.section .text.x0200b6d0,"ax",%progbits
	.global SceneEffect_SpawnParticlesAboveActor
	.thumb_func
SceneEffect_SpawnParticlesAboveActor:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r0, .L_0200b70c
	bl Engine_GameFlagIsSet
	cmp r0, #0
	bne .L_0200b6ee
	ldr r3, .L_0200b710
	movs r1, #3
	ldr r0, [r3]
	bl Math_RemainderUnsigned
	cmp r0, #0
	bne .L_0200b7b8
.L_0200b6ee:
	movs r0, #24
	bl Object_GetById
	adds r5, r0, #0
	ldr r0, .L_0200b70c
	bl Engine_GameFlagIsSet
	cmp r0, #0
	beq .L_0200b714
	bl Engine_RandomNext
	adds r2, r0, #0
	lsls r2, r2, #8
	b .L_0200b71c
	.2byte 0x0000
.L_0200b70c:
	.4byte 0x00000236
.L_0200b710:
	.4byte gFrameCount
.L_0200b714:
	bl Engine_RandomNext
	adds r2, r0, #0
	lsls r2, r2, #6
.L_0200b71c:
	ldr r3, [r5, #12]
	lsrs r2, r2, #16
	lsls r2, r2, #16
	adds r2, r2, r3
	ldr r3, .L_0200b7a8
	movs r0, #142
	adds r2, r2, r3
	ldr r1, [r5, #8]
	ldr r3, [r5, #16]
	lsls r0, r0, #1
	bl Engine_ObjectCreate
	adds r7, r0, #0
	cmp r7, #0
	beq .L_0200b7b8
	ldr r1, .L_0200b7ac
	adds r0, r7, #0
	ldr r6, [r7, #80]
	bl Engine_ObjectSetScript
	movs r1, #1
	adds r0, r7, #0
	bl ObjectGroup_SetChildValue
	adds r3, r7, #0
	adds r3, #85
	movs r5, #0
	strb r5, [r3]
	bl Engine_RandomNext
	ldr r3, .L_0200b7b0
	adds r2, r7, #0
	adds r2, #100
	ands r3, r0
	strh r3, [r2]
	adds r3, r7, #0
	adds r3, #102
	strh r5, [r3]
	ldr r3, .L_0200b7b4
	ldr r1, .L_0200b7a4
	str r3, [r7, #108]
	mov r8, r1
	bl Engine_RandomNext
	adds r3, r0, #0
	lsls r0, r3, #16
	subs r0, r0, r3
	lsrs r0, r0, #20
	bl Engine_MathSin
	lsls r3, r0, #1
	adds r3, r3, r0
	lsls r3, r3, #3
	asrs r3, r3, #16
	str r3, [r7, #48]
	adds r3, r6, #0
	adds r3, #38
	mov r2, r8
	strb r2, [r3]
	movs r3, #13
	ldrb r2, [r6, #9]
	negs r3, r3
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	strb r3, [r6, #9]
	b .L_0200b7b8
	.2byte 0x0000
.L_0200b7a4:
	.4byte 0x00000000
.L_0200b7a8:
	.4byte 0xffe40000
.L_0200b7ac:
	.4byte VinasuChojo_OrbitParticleScript
.L_0200b7b0:
	.4byte 0x0ffff000
.L_0200b7b4:
	.4byte SceneEffect_UpdateOrbitAroundActor
.L_0200b7b8:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.section .rodata.x0200df10,"a",%progbits
.L_0200df10:
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x00000016
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000001b
.L_0200df48:
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x0000002c
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000001b
.L_0200df80:
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x0000007e
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000001b
	.global gEffectScripts
gEffectScripts:
	.4byte .L_0200df10
	.4byte .L_0200df48
	.4byte .L_0200df80
	.global VinasuChojo_PairActionScript
VinasuChojo_PairActionScript:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x01ec0000
	.4byte 0x00000000
	.4byte 0x00a40000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000010
	.global SceneAction_EntryGroup
SceneAction_EntryGroup:
	.4byte 0x00000015
	.4byte 0x0000000f
	.4byte 0x00003333
	.4byte 0x00000015
	.4byte 0x00000010
	.4byte 0x00001999
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0xfff80000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000010
	.global SceneAction_EntryPair
SceneAction_EntryPair:
	.4byte 0x00000015
	.4byte 0x0000000f
	.4byte 0x00003333
	.4byte 0x00000015
	.4byte 0x00000010
	.4byte 0x00001999
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000010
	.global Data_0200e074
Data_0200e074:
	.4byte 0x00000022
	.4byte OverlayObject_StepScaleByCounter
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0x00000000
	.global Data_0200e088
Data_0200e088:
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x00000020
	.4byte 0x00000000
	.4byte 0x00000010
	.global Data_020060ac
Data_020060ac:
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0xfffffc80
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x0000008c
	.4byte 0x00000000
	.4byte 0x00000010
	.global Data_0200e0d0
Data_0200e0d0:
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0xfffffc80
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x000000a0
	.4byte 0x00000000
	.4byte 0x00000010
	.global Data_0200e0f4
Data_0200e0f4:
	.4byte 0x00000016
	.4byte 0x00000006
	.4byte 0x00040000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x0000001e
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000010
	.global Data_0200e130
Data_0200e130:
	.4byte 0x00000016
	.4byte 0x00000006
	.4byte 0x00020000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x000000a0
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000010
	.global VinasuChojo_OrbitParticleScript
VinasuChojo_OrbitParticleScript:
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x0000cccc
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x0000cccc
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x80010000
	.4byte 0x00000016
	.4byte 0x00000006
	.4byte 0x00010000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x00000010
	.4byte 0xc0010000
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000001b
	.global VinasuChojo_RiseParticleScript
VinasuChojo_RiseParticleScript:
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x0000cccc
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x0000cccc
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x80010000
	.4byte 0x00000016
	.4byte 0x00000006
	.4byte 0xffff0000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x00000010
	.4byte 0xc0010000
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000001b
	.global Data_0200622c
Data_0200622c:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x010c0000
	.4byte 0x00000000
	.4byte 0x00ba0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x010c0000
	.4byte 0x00000000
	.4byte 0x00fc0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x00de0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000010
	.global VinasuChojo_RiseActionScript
VinasuChojo_RiseActionScript:
	.4byte 0x80010000
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0xfffffa00
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x00000004
	.4byte 0xc0010000
	.4byte 0x80020000
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00000600
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x00000004
	.4byte 0xc0020000
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0x00000000
	.global Data_0200e324
Data_0200e324:
	.4byte 0x00000016
	.4byte 0x00000006
	.4byte 0xffff4000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x00000078
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000010
	.global Data_0200e360
Data_0200e360:
	.4byte 0x00000016
	.4byte 0x00000006
	.4byte 0xfffb8000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x00000050
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000010
	.global Data_0200e39c
Data_0200e39c:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x014a0000
	.4byte 0x00000000
	.4byte 0x00ad0000
	.4byte 0x00000001
	.4byte 0x0000001b
	.4byte 0x00000010
	.global Data_0200e3c0
Data_0200e3c0:
	.4byte 0x00000022
	.4byte OverlayObject_UpdateHeadingTimer
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0x00000000
	.global Data_0200e3d4
Data_0200e3d4:
	.4byte 0xffff0000
	.4byte 0x00000130
	.4byte 0x300000d8
	.4byte 0x00000000
	.4byte 0x02580000
	.4byte 0x000001a4
	.4byte 0xffff0001
	.4byte 0x000001f8
	.4byte 0x300000a8
	.4byte 0x00000000
	.4byte 0x02580000
	.4byte 0x000001a4
	.4byte 0xffff0002
	.4byte 0x00000154
	.4byte 0x800000b8
	.4byte 0x00000000
	.4byte 0x02580000
	.4byte 0x000001a4
	.4byte 0xffff0003
	.4byte 0x00000154
	.4byte 0x800000b8
	.4byte 0x00000000
	.4byte 0x02580000
	.4byte 0x000001a4
	.4byte 0xffff0009
	.4byte 0x00000000
	.4byte 0x80000000
	.4byte 0x00000000
	.4byte 0x02580000
	.4byte 0x000001a4
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_0200e464
Data_0200e464:
	.4byte 0x000000bb
	.4byte 0x00149002
	.4byte 0x002630b4
	.4byte 0x0090c0b3
	.4byte 0x000001ff
	.global Data_0200e478
Data_0200e478:
	.4byte 0xffff0102
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00024000
	.4byte 0xffff0102
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x01024000
	.4byte 0xffff0102
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x01024000
	.4byte 0xffff0102
	.4byte 0x00000001
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x01024000
	.4byte 0xffff00f2
	.4byte 0x00000007
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x00fc0000
	.4byte 0x00024000
	.4byte 0xffff00f2
	.4byte 0x00000007
	.4byte 0x01d80000
	.4byte 0x00000000
	.4byte 0x00fc0000
	.4byte 0x01024000
	.4byte 0xffff00f2
	.4byte 0x00000007
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x00fc0000
	.4byte 0x01024000
	.4byte 0xffff00f2
	.4byte 0x00000007
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x00fc0000
	.4byte 0x01024000
	.4byte 0xffff00f2
	.4byte 0x00000007
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x00fc0000
	.4byte 0x01024000
	.4byte 0xffff00f2
	.4byte 0x00000007
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x00fc0000
	.4byte 0x01024000
	.4byte 0xffff00f2
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x00940000
	.4byte 0x00024000
	.4byte 0xffff0002
	.4byte 0x00000001
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x00940000
	.4byte 0x00024000
	.4byte 0xffff0003
	.4byte 0x00000001
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x00940000
	.4byte 0x00024000
	.4byte 0xffff0021
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x00a40000
	.4byte 0x0002d000
	.4byte 0xffff0020
	.4byte 0x00000001
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x00b40000
	.4byte 0x0002b000
	.4byte 0xffff0006
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x00ac0000
	.4byte 0x00020000
	.4byte 0xffff0023
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x00fc0000
	.4byte 0x0002d000
	.4byte 0xffff0016
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff00f4
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff00e5
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff00e5
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff00e5
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff00e5
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff00e5
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_0200e6e8
Data_0200e6e8:
	.4byte 0x00000000
	.global Data_0200e6ec
Data_0200e6ec:
	.4byte 0x00000202
	.4byte 0xffff000b
	.4byte InitializeActorZeroMotion
	.4byte 0x00000002
	.4byte 0xffff000b
	.4byte SceneActor_SetByte55ForActorZeroAnd12To17
	.4byte 0x00000202
	.4byte 0xffff000a
	.4byte InitializeActorZeroMotion
	.4byte 0x00000002
	.4byte 0x0250000a
	.4byte SceneEffect_SpawnAndBobWithActorZero
	.4byte 0x00000002
	.4byte 0xffff0005
	.4byte Scene_RunActorEntrySequence
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global VinasuChojo_BesideParticleScript
VinasuChojo_BesideParticleScript:
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00016666
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00016666
	.4byte 0x0000001c
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x0000001b
	.section .bss,"aw",%nobits
	.global VinasuChojo_TransitionTimer
VinasuChojo_TransitionTimer:
	.space 4
	.global VinasuChojo_TransitionStep
VinasuChojo_TransitionStep:
	.space 4
