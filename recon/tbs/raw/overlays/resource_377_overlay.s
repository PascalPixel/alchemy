.syntax unified
	.thumb
	.section .text.x02008578,"ax",%progbits
	.global FieldScene_RunComplexActorSequence
	.thumb_func
FieldScene_RunComplexActorSequence:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	ldr r2, .L_02008640
	mov r9, r2
	ldr r3, [r2]
	subs r2, #76
	ldr r7, [r2]
	movs r2, #240
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r0, #17
	ldr r6, [r3]
	bl Object_GetById
	ldr r0, [r0, #80]
	mov r8, r0
	bl Engine_EventBegin
	movs r0, #11
	movs r1, #0
	movs r2, #0
	bl Engine_ActorSetPosition
	movs r0, #12
	movs r1, #0
	movs r2, #0
	bl Engine_ActorSetPosition
	movs r0, #13
	movs r1, #0
	movs r2, #0
	bl Engine_ActorSetPosition
	movs r0, #14
	movs r1, #0
	movs r2, #0
	bl Engine_ActorSetPosition
	movs r0, #15
	movs r1, #0
	movs r2, #0
	bl Engine_ActorSetPosition
	movs r2, #0
	movs r1, #0
	movs r0, #16
	bl Engine_ActorSetPosition
	movs r0, #0
	bl Object_GetById
	movs r1, #0
	bl Engine_ActorSetSpriteFlags
	movs r1, #18
	movs r0, #0
	bl Engine_ActorSetAnimation
	movs r3, #0
	mov r10, r3
	ldr r3, .L_02008644
	mov r2, r8
	strh r3, [r2, #30]
	movs r0, #17
	bl Object_GetById
	ldr r5, .L_0200863c
	adds r0, #85
	strb r5, [r0]
	movs r0, #17
	bl Object_GetById
	movs r1, #0
	bl Engine_ActorSetSpriteFlags
	movs r1, #144
	lsls r1, r1, #18
	ldr r2, .L_02008648
	movs r0, #17
	bl Engine_ActorSetPosition
	movs r0, #7
	bl Map_ClearLayerEntryFlag
	movs r2, #172
	ldr r1, .L_0200864c
	lsls r2, r2, #18
	movs r0, #8
	bl Engine_ActorSetPosition
	bl Graphics_EnableObjLayerAndCallbacks
	movs r0, #8
	b .L_02008650
	.2byte 0x0000
.L_0200863c:
	.4byte 0x00000000
.L_02008640:
	.4byte Data_03001ebc
.L_02008644:
	.4byte 0x00000555
.L_02008648:
	.4byte 0x028a0000
.L_0200864c:
	.4byte 0x02160000
.L_02008650:
	bl Ui_SetRenderResultFromObject
	ldr r5, .L_020089c8
	movs r1, #1
	adds r0, r5, #0
	movs r2, #0
	bl UiText_ShowCenteredMessage
	movs r0, #40
	bl Engine_EventWait
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #9
	lsls r2, r2, #9
	lsls r0, r0, #9
	bl Runtime_SetWorkTripleIfNonNegative
	movs r0, #8
	bl Ui_SetRenderResultFromObject
	movs r1, #1
	adds r0, r5, #1
	movs r2, #0
	bl UiText_ShowCenteredMessage
	bl ObjectDispatch_StopCallbacksAndHideLayers
	movs r0, #40
	bl Engine_EventWait
	adds r2, r7, #0
	movs r3, #164
	adds r2, #236
	lsls r3, r3, #17
	str r3, [r2]
	movs r3, #150
	adds r2, #4
	lsls r3, r3, #18
	str r3, [r2]
	movs r3, #156
	adds r2, #4
	lsls r3, r3, #18
	str r3, [r2]
	movs r3, #204
	adds r2, #4
	lsls r3, r3, #18
	str r3, [r2]
	movs r3, #141
	lsls r3, r3, #18
	str r3, [r6, #8]
	mov r3, r10
	str r3, [r6, #12]
	ldr r3, .L_020089cc
	str r3, [r6, #16]
	bl Engine_MapRedraw
	movs r0, #1
	bl Engine_TaskWait
	mov r2, r9
	ldr r1, [r2]
	movs r3, #224
	lsls r3, r3, #1
	adds r2, r1, r3
	adds r3, #73
	str r3, [r2]
	subs r3, #65
	adds r2, r1, r3
	movs r3, #64
	str r3, [r2]
	bl BattleFx_StartTwelveFrameBlend
	mov r2, r9
	ldr r3, [r2, #12]
	ldr r2, .L_020089d0
	adds r3, r3, r2
	movs r2, #1
	strh r2, [r3]
	bl BattleFx_SetBlock30Values12Zero
	movs r0, #30
	bl Engine_TaskWait
	adds r5, #2
	bl Engine_EventOpenScreen
	bl Engine_EventWaitForScreen
	bl BattleFx_SetBlock30Values128One
	movs r1, #4
	movs r0, #8
	bl Engine_ActorSetAnimationAndWait
	adds r0, r5, #0
	bl Engine_EventSetMessage
	movs r2, #60
	ldr r0, .L_020089d4
	movs r1, #0
	bl Engine_EventShowMessageAndWait
	movs r1, #2
	movs r0, #0
	bl Engine_ActorRunRepeatedMotion
	movs r0, #40
	bl Engine_EventWait
	movs r1, #1
	movs r0, #8
	bl Engine_ActorRunRepeatedMotion
	movs r0, #40
	bl Engine_EventWait
	movs r2, #20
	ldr r0, .L_020089d4
	movs r1, #0
	bl Engine_EventShowMessageAndWait
	movs r1, #2
	movs r0, #0
	bl Engine_ActorRunRepeatedMotion
	movs r0, #7
	bl Map_SetLayerEntryFlag
	movs r0, #20
	bl Engine_EventWait
	movs r0, #8
	bl Map_ClearLayerEntryFlag
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #8
	movs r0, #0
	lsls r1, r1, #9
	bl Engine_ActorSetSpeed
	movs r0, #0
	movs r1, #19
	bl Engine_ActorSetAnimation
	ldr r1, .L_020089d8
	ldr r2, .L_020089dc
	movs r0, #0
	bl Engine_ObjectMotionSetPositionAndCommit
	movs r0, #8
	bl Map_SetLayerEntryFlag
	movs r0, #9
	bl Map_ClearLayerEntryFlag
	movs r2, #170
	ldr r1, .L_020089e0
	lsls r2, r2, #2
	movs r0, #0
	bl Engine_ObjectMotionSetPositionAndCommit
	movs r0, #30
	bl Engine_EventWait
	movs r1, #208
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #8
	bl Engine_ActorFaceDirection
	movs r0, #0
	bl Object_GetById
	movs r1, #1
	bl Engine_ActorSetSpriteFlags
	movs r0, #0
	movs r1, #4
	movs r2, #0
	bl Engine_ActorJump
	ldr r2, .L_020089e4
	movs r0, #0
	ldr r1, .L_020089e8
	bl Engine_ActorWalkToAndWait
	movs r0, #0
	movs r1, #3
	bl Engine_ActorSetSpritePriority
	movs r1, #128
	movs r2, #40
	movs r0, #0
	lsls r1, r1, #7
	bl Engine_ActorFaceDirection
	movs r1, #4
	movs r0, #8
	bl Engine_ActorSetAnimationAndWait
	movs r0, #20
	bl Engine_EventWait
	ldr r0, .L_020089d4
	movs r1, #0
	bl Engine_EventShowMessage
	bl FieldScene_RunSixStepSequence17e4
	movs r0, #8
	movs r1, #2
	bl Engine_ActorStartRepeatedMotion
	movs r1, #0
	movs r2, #20
	ldr r0, .L_020089d4
	bl Engine_EventShowMessageAndWait
	movs r0, #8
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r5, #254
	adds r3, r5, #0
	ands r3, r2
	movs r2, #170
	lsls r2, r2, #2
	strb r3, [r0]
	ldr r1, .L_020089ec
	movs r0, #8
	bl Engine_ActorWalkToAndWait
	movs r0, #1
	bl Engine_EventWait
	movs r0, #8
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	movs r6, #1
	orrs r3, r6
	strb r3, [r0]
	movs r0, #10
	bl Engine_EventWait
	movs r1, #2
	movs r0, #8
	bl Engine_ActorRunRepeatedMotion
	movs r0, #0
	bl Object_GetById
	movs r1, #226
	bl ObjectDispatch_RegisterChildMetadata
	movs r0, #33
	bl Engine_GameFlagSet
	movs r0, #126
	bl Engine_AudioPlayCue
	movs r1, #7
	movs r0, #0
	bl ObjectGroup_ConfigureChildValue
	movs r0, #10
	bl Engine_EventWait
	movs r1, #0
	movs r0, #0
	bl ObjectGroup_ConfigureChildValue
	movs r0, #20
	bl Engine_EventWait
	movs r0, #8
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	movs r2, #172
	ands r5, r3
	ldr r1, .L_020089f0
	lsls r2, r2, #2
	strb r5, [r0]
	movs r0, #8
	bl Engine_ActorWalkToAndWait
	movs r0, #1
	bl Engine_EventWait
	movs r0, #8
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	orrs r3, r6
	strb r3, [r0]
	movs r0, #20
	bl Engine_EventWait
	movs r1, #192
	movs r2, #192
	movs r0, #8
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl Engine_ActorSetSpeed
	movs r1, #192
	movs r2, #192
	lsls r2, r2, #8
	movs r0, #0
	lsls r1, r1, #9
	bl Engine_ActorSetSpeed
	movs r1, #1
	movs r0, #8
	bl Engine_CameraFollowActor
	movs r0, #0
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	ldr r5, .L_020089f4
	orrs r6, r3
	adds r1, r5, #0
	strb r6, [r0]
	movs r0, #8
	bl Engine_ActorEnableActionCallback
	movs r0, #20
	bl Engine_EventWait
	adds r1, r5, #0
	movs r0, #0
	bl Engine_ActorEnableActionCallback
	movs r0, #8
	bl Object_RefreshSelectorById
	movs r0, #8
	ldr r1, .L_020089f8
	ldr r2, .L_020089fc
	bl Engine_ActorWalkToAndWait
	movs r1, #204
	ldr r2, .L_020089fc
	movs r0, #8
	lsls r1, r1, #1
	bl Engine_ActorWalkToAndWait
	movs r0, #8
	movs r1, #1
	bl Engine_ActorSetAnimation
	movs r0, #0
	movs r1, #1
	bl Engine_ActorSetAnimation
	movs r1, #128
	movs r0, #8
	lsls r1, r1, #7
	movs r2, #10
	bl Engine_ActorFaceDirection
	movs r1, #0
	ldr r0, .L_02008a00
	bl Engine_EventOpenMessage
	movs r0, #0
	movs r1, #0
	bl Engine_EventChooseYesNo
	cmp r0, #0
	bne .L_0200895a
	mov r3, r9
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
.L_0200895a:
	movs r0, #20
	bl Engine_EventWait
	movs r2, #20
	ldr r0, .L_02008a00
	movs r1, #0
	bl Engine_EventShowMessageAndWait
	movs r0, #0
	movs r1, #3
	bl Engine_ActorSetAnimation
	movs r1, #3
	movs r0, #8
	bl Engine_ActorSetAnimationAndWait
	movs r0, #20
	bl Engine_EventWait
	ldr r1, .L_02008a04
	movs r0, #8
	bl Engine_ActorEnableActionCallback
	ldr r1, .L_02008a08
	movs r0, #0
	bl Engine_ActorEnableActionCallback
	movs r0, #20
	bl Engine_EventWait
	mov r2, r9
	ldr r1, [r2]
	movs r3, #224
	lsls r3, r3, #1
	adds r2, r1, r3
	adds r3, #65
	str r3, [r2]
	subs r3, #57
	adds r2, r1, r3
	movs r3, #16
	str r3, [r2]
	bl Engine_EventCloseScreen
	bl Engine_EventWaitForScreen
	movs r0, #20
	bl Engine_EventRequestExit
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7}
	pop {r0}
	bx r0
.L_020089c8:
	.4byte 0x00000e52
.L_020089cc:
	.4byte 0x02b30000
.L_020089d0:
	.4byte 0x00001f84
.L_020089d4:
	.4byte 0x00009008
.L_020089d8:
	.4byte 0x0000022d
.L_020089dc:
	.4byte 0x000002a7
.L_020089e0:
	.4byte 0x0000022b
.L_020089e4:
	.4byte 0x000002a2
.L_020089e8:
	.4byte 0x0000021f
.L_020089ec:
	.4byte 0x0000021e
.L_020089f0:
	.4byte 0x00000216
.L_020089f4:
	.4byte Data_02001ab4
.L_020089f8:
	.4byte 0x000001a3
.L_020089fc:
	.4byte 0x00000295
.L_02008a00:
	.4byte 0x00008008
.L_02008a04:
	.4byte Data_02001b04
.L_02008a08:
	.4byte Data_02001b34
	.section .rodata.x02009ab4,"a",%progbits
	.global Data_02001ab4
Data_02001ab4:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x020a0000
	.4byte 0x00000000
	.4byte 0x02ec0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x01a30000
	.4byte 0x00000000
	.4byte 0x02ec0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x01a30000
	.4byte 0x00000000
	.4byte 0x02b30000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.global Data_02001b04
Data_02001b04:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x01830000
	.4byte 0x00000000
	.4byte 0x02950000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000010
	.global Data_02001b34
Data_02001b34:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x02950000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x01830000
	.4byte 0x00000000
	.4byte 0x02950000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000010
	.global gHaidiaBabiRampActor8Action
gHaidiaBabiRampActor8Action:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x01d40000
	.4byte 0x00000000
	.4byte 0x01860000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x01fc0000
	.4byte 0x00000000
	.4byte 0x01920000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.global gHaidiaBabiRampLeaderAction
gHaidiaBabiRampLeaderAction:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x01af0000
	.4byte 0x00000000
	.4byte 0x01870000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x01d40000
	.4byte 0x00000000
	.4byte 0x01860000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x01e60000
	.4byte 0x00000000
	.4byte 0x01890000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.global gHaidiaBabiRampActor8ActionB
gHaidiaBabiRampActor8ActionB:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x020d0000
	.4byte 0x00000000
	.4byte 0x019e0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x01fd0000
	.4byte 0x00000000
	.4byte 0x01ba0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x01c70000
	.4byte 0x00000000
	.4byte 0x01c20000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.global gHaidiaBabiRampLeaderActionB
gHaidiaBabiRampLeaderActionB:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x020d0000
	.4byte 0x00000000
	.4byte 0x019e0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x01fd0000
	.4byte 0x00000000
	.4byte 0x01ba0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x01e10000
	.4byte 0x00000000
	.4byte 0x01bb0000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.global gHaidiaBabiRampFinalAction
gHaidiaBabiRampFinalAction:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x01ac0000
	.4byte 0x00000000
	.4byte 0x01cc0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x019f0000
	.4byte 0x00000000
	.4byte 0x02010000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x019f0000
	.4byte 0x00000000
	.4byte 0x024d0000
	.4byte 0x00000001
	.4byte 0x00000010
	.global gHaidiaBabiRampActor10Action
gHaidiaBabiRampActor10Action:
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x0000e666
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00010000
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x0000cccc
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x0000cccc
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0x00000000
	.global gHaidiaBabiRampActor10ActionB
gHaidiaBabiRampActor10ActionB:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00010000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00013333
	.4byte 0x00000000
	.4byte 0x00000003
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x0000cccc
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x0000cccc
	.4byte 0x80010000
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x0000051e
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x0000051e
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x00000008
	.4byte 0xc0010000
	.4byte 0x80020000
	.4byte 0x00000016
	.4byte 0x00000006
	.4byte 0xffffe667
	.4byte 0x00000016
	.4byte 0x00000012
	.4byte 0xffffe667
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0xfffffd71
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x00000a3d
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x0000000e
	.4byte 0xc0020000
	.4byte 0x00000015
	.4byte 0x00000006
	.4byte 0x00080000
	.4byte 0x00000015
	.4byte 0x00000012
	.4byte 0x00080000
	.4byte 0x80030000
	.4byte 0x00000016
	.4byte 0x00000006
	.4byte 0x00007333
	.4byte 0x00000016
	.4byte 0x00000012
	.4byte 0x00007333
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00000041
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffffae2
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x00000028
	.4byte 0xc0030000
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000010
	.global gHaidiaBabiActor8Departure
gHaidiaBabiActor8Departure:
	.4byte 0x00000022
	.4byte SceneActor_UpdateFacingTowardTarget
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000003
	.4byte 0x02370000
	.4byte 0x00000000
	.4byte 0x02b20000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x021f0000
	.4byte 0x00000000
	.4byte 0x02a20000
	.4byte 0x00000001
	.4byte 0x00000010
	.global gHaidiaBabiBagLiftScript
gHaidiaBabiBagLiftScript:
	.4byte 0x00000016
	.4byte 0x00000006
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x00000010
	.4byte 0x00000000
	.global gHaidiaBabiBagShowScript
gHaidiaBabiBagShowScript:
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0xffffc000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00002000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffff000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0xffffc000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00002000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffff000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x00000010
	.global gHaidiaBabiEntrances
gHaidiaBabiEntrances:
	.4byte 0xffff0000
	.4byte 0x00000078
	.4byte 0x40000064
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x000000a0
	.4byte 0xc00000e4
	.4byte 0x00180000
	.4byte 0x00f80000
	.4byte 0x00000100
	.4byte 0xffff0002
	.4byte 0x000001e0
	.4byte 0xc00000b0
	.4byte 0x01080000
	.4byte 0x02300008
	.4byte 0x000000f0
	.4byte 0xffff0003
	.4byte 0x000002d1
	.4byte 0xc00000c0
	.4byte 0x02480000
	.4byte 0x03500000
	.4byte 0x000000e0
	.4byte 0xffff0004
	.4byte 0x00000090
	.4byte 0xc00001ee
	.4byte 0x00080000
	.4byte 0x01400100
	.4byte 0x00000210
	.4byte 0xffff0005
	.4byte 0x00000077
	.4byte 0x40000154
	.4byte 0x00080000
	.4byte 0x01400100
	.4byte 0x00000210
	.4byte 0xffff0006
	.4byte 0x000001a0
	.4byte 0xc0000210
	.4byte 0x01580000
	.4byte 0x02800110
	.4byte 0x00000230
	.4byte 0xffff0007
	.4byte 0x00000198
	.4byte 0x40000173
	.4byte 0x01580000
	.4byte 0x02800110
	.4byte 0x00000230
	.4byte 0xffff0008
	.4byte 0x000000b8
	.4byte 0x00000279
	.4byte 0x00080000
	.4byte 0x01400240
	.4byte 0x000002e0
	.4byte 0xffff0009
	.4byte 0x00000198
	.4byte 0x00000299
	.4byte 0x01480000
	.4byte 0x02700258
	.4byte 0x00000330
	.4byte 0xffff000a
	.4byte 0x00000210
	.4byte 0x400002ab
	.4byte 0x01480000
	.4byte 0x02700258
	.4byte 0x00000330
	.4byte 0xffff000b
	.4byte 0x000002c1
	.4byte 0xc0000331
	.4byte 0x02800000
	.4byte 0x03b80228
	.4byte 0x00000360
	.4byte 0xffff000c
	.4byte 0x0000036f
	.4byte 0xc00002fd
	.4byte 0x02800000
	.4byte 0x03b80228
	.4byte 0x00000360
	.4byte 0xffff0010
	.4byte 0x0000021a
	.4byte 0x400002ab
	.4byte 0x01480000
	.4byte 0x02700258
	.4byte 0x00000330
	.4byte 0xffff0013
	.4byte 0x000002f8
	.4byte 0x40000148
	.4byte 0x02a00000
	.4byte 0x03c00110
	.4byte 0x000001e8
	.4byte 0xffff0014
	.4byte 0x00000237
	.4byte 0x000002a4
	.4byte 0x04000000
	.4byte 0x04000240
	.4byte 0x00000240
	.4byte 0xffff0015
	.4byte 0x00000198
	.4byte 0x40000173
	.4byte 0x01580000
	.4byte 0x02800110
	.4byte 0x00000230
	.4byte 0xffff0016
	.4byte 0x000001e0
	.4byte 0xc00000b0
	.4byte 0x01080000
	.4byte 0x02300008
	.4byte 0x000000f0
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gHaidiaBabiExits
gHaidiaBabiExits:
	.4byte 0x00000008
	.4byte 0x00106005
	.4byte 0x00203006
	.4byte 0x00302005
	.4byte 0x00407005
	.4byte 0x00508008
	.4byte 0x00608004
	.4byte 0x00709008
	.4byte 0x00805008
	.4byte 0x00907008
	.4byte 0x00a0c003
	.4byte 0x00b0d003
	.4byte 0x0130b08a
	.4byte 0x01415008
	.4byte 0x0150f003
	.4byte 0x0160a006
	.4byte 0x000001ff
	.global gHaidiaBabiExits2
gHaidiaBabiExits2:
	.4byte 0x00000008
	.4byte 0x00106005
	.4byte 0x00203006
	.4byte 0x00302005
	.4byte 0x00407005
	.4byte 0x00508008
	.4byte 0x00608003
	.4byte 0x00709008
	.4byte 0x00805008
	.4byte 0x00907008
	.4byte 0x00a0c003
	.4byte 0x00b0d003
	.4byte 0x01415008
	.4byte 0x0150f003
	.4byte 0x000001ff
	.global gHaidiaBabiPlacements
gHaidiaBabiPlacements:
	.4byte 0xffff001f
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0035
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000c000
	.4byte 0xffff00d0
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000c000
	.4byte 0x0000006c
	.4byte 0x00000001
	.4byte 0x00970000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00004000
	.4byte 0x00000078
	.4byte 0x00000002
	.4byte 0x00570000
	.4byte 0x00000000
	.4byte 0x00430000
	.4byte 0x00008000
	.4byte 0x00000080
	.4byte 0x00000001
	.4byte 0x00f60000
	.4byte 0x00000000
	.4byte 0x01870000
	.4byte 0x00018000
	.4byte 0x0000006c
	.4byte 0x00000002
	.4byte 0x009f0000
	.4byte 0x00000000
	.4byte 0x01c40000
	.4byte 0x0000b000
	.4byte 0x0000006a
	.4byte 0x00000001
	.4byte 0x00da0000
	.4byte 0x00000000
	.4byte 0x01750000
	.4byte 0x00011000
	.4byte 0x00000022
	.4byte 0x00000001
	.4byte 0x00360000
	.4byte 0x00000000
	.4byte 0x029d0000
	.4byte 0x00018000
	.4byte 0xffff00e2
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0002e000
	.4byte 0xffff001e
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gHaidiaBabiPlacements2
gHaidiaBabiPlacements2:
	.4byte 0xffff001f
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000035
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000c000
	.4byte 0x000000d0
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000c000
	.4byte 0x0000006c
	.4byte 0x00000001
	.4byte 0x00970000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00004000
	.4byte 0x00000078
	.4byte 0x00000002
	.4byte 0x00570000
	.4byte 0x00000000
	.4byte 0x00430000
	.4byte 0x00008000
	.4byte 0x00000080
	.4byte 0x00000001
	.4byte 0x00f60000
	.4byte 0x00000000
	.4byte 0x01870000
	.4byte 0x00008000
	.4byte 0x0000006c
	.4byte 0x00000002
	.4byte 0x009f0000
	.4byte 0x00000000
	.4byte 0x01c40000
	.4byte 0x0000b000
	.4byte 0x0000006a
	.4byte 0x00000001
	.4byte 0x00da0000
	.4byte 0x00000000
	.4byte 0x01750000
	.4byte 0x00011000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gHaidiaBabiPlacements3
gHaidiaBabiPlacements3:
	.4byte 0xffff001f
	.4byte 0x00000001
	.4byte 0x024a0000
	.4byte 0x00000000
	.4byte 0x01960000
	.4byte 0x00020000
	.4byte 0x00000035
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000c000
	.4byte 0x000000d0
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000c000
	.4byte 0x0000006c
	.4byte 0x00000001
	.4byte 0x00970000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00004000
	.4byte 0x00000078
	.4byte 0x00000002
	.4byte 0x00570000
	.4byte 0x00000000
	.4byte 0x00430000
	.4byte 0x00008000
	.4byte 0x00000080
	.4byte 0x00000001
	.4byte 0x00f60000
	.4byte 0x00000000
	.4byte 0x01870000
	.4byte 0x00008000
	.4byte 0x0000006c
	.4byte 0x00000002
	.4byte 0x009f0000
	.4byte 0x00000000
	.4byte 0x01c40000
	.4byte 0x0000b000
	.4byte 0x0000006a
	.4byte 0x00000001
	.4byte 0x00da0000
	.4byte 0x00000000
	.4byte 0x01750000
	.4byte 0x00011000
	.4byte 0x00000065
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x01750000
	.4byte 0x0001b000
	.4byte 0x00000016
	.4byte 0x00000001
	.4byte 0x00fc0000
	.4byte 0x00000000
	.4byte 0x017a0000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gHaidiaBabiPlacements4
gHaidiaBabiPlacements4:
	.4byte 0xffff0070
	.4byte 0x00000001
	.4byte 0x03180000
	.4byte 0x00000000
	.4byte 0x01500000
	.4byte 0x0000c000
	.4byte 0xffff0071
	.4byte 0x00000001
	.4byte 0x03600000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00016000
	.4byte 0xffff0070
	.4byte 0x00000001
	.4byte 0x03880000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x00010000
	.4byte 0xffff0071
	.4byte 0x00000001
	.4byte 0x02d80000
	.4byte 0x00000000
	.4byte 0x01b80000
	.4byte 0x00004000
	.4byte 0xffff0067
	.4byte 0x00000002
	.4byte 0x03400000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gHaidiaBabiEvents4
gHaidiaBabiEvents4:
	.4byte 0x00000002
	.4byte 0xffff0001
	.4byte SceneState_SetValue123Mode1
	.4byte 0x00000002
	.4byte 0xffff0002
	.4byte FieldScene_RunStep7BThen2
	.4byte 0x00000002
	.4byte 0xffff0003
	.4byte SceneState_SetValue123Mode3
	.4byte 0x00000002
	.4byte 0xffff0004
	.4byte FieldScene_RunStep7BThen4
	.4byte 0x00000002
	.4byte 0xffff0005
	.4byte FieldScene_RunStep80Then5
	.4byte 0x00000002
	.4byte 0xffff0006
	.4byte FieldScene_RunStep7BThen6
	.4byte 0x00000002
	.4byte 0xffff0007
	.4byte FieldScene_RunStep80Then7
	.4byte 0x00000002
	.4byte 0xffff0008
	.4byte SceneState_SetValue129Mode8
	.4byte 0x00000002
	.4byte 0xffff0009
	.4byte SceneState_SetValue129Mode9
	.4byte 0x00000002
	.4byte 0xffff000a
	.4byte FieldScene_RunStep7BThen10
	.4byte 0x00000002
	.4byte 0xffff000b
	.4byte SceneState_ApplyValues123And11
	.4byte 0x000000d3
	.4byte 0xffff0064
	.4byte 0x00400955
	.4byte 0x00000023
	.4byte 0xffff0065
	.4byte 0x0040094a
	.4byte 0x00000033
	.4byte 0xffff0066
	.4byte 0x0040094b
	.4byte 0x000000f3
	.4byte 0xffff00ce
	.4byte 0x004029ca
	.4byte 0x000000f3
	.4byte 0xffff00cf
	.4byte 0x004029cb
	.4byte 0x000000f3
	.4byte 0xffff00d0
	.4byte 0x004029cc
	.4byte 0x0000c4f3
	.4byte 0xffff00d1
	.4byte 0x004029cd
	.4byte 0x0000c4f3
	.4byte 0xffff00d2
	.4byte 0x004029ce
	.4byte 0x000000f3
	.4byte 0xffff00d3
	.4byte 0x004029cf
	.4byte 0x0000c4f3
	.4byte 0xffff00d4
	.4byte 0x004029d0
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global gHaidiaBabiEvents
gHaidiaBabiEvents:
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x00000f56
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x00000f57
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x00000f59
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x00000f5a
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte HaidiaBabi_RunHeyBoyScene
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte HaidiaBabi_RunInnkeeperTalk
	.4byte 0x00000002
	.4byte 0xffff0001
	.4byte SceneState_SetValue123Mode1
	.4byte 0x00000002
	.4byte 0xffff0002
	.4byte FieldScene_RunStep7BThen2
	.4byte 0x00000002
	.4byte 0xffff0003
	.4byte SceneState_SetValue123Mode3
	.4byte 0x00000002
	.4byte 0xffff0004
	.4byte FieldScene_RunStep7BThen4
	.4byte 0x00000002
	.4byte 0xffff0005
	.4byte FieldScene_RunStep80Then5
	.4byte 0x00000002
	.4byte 0xffff0006
	.4byte FieldScene_RunStep7BThen6
	.4byte 0x00000002
	.4byte 0xffff0007
	.4byte FieldScene_RunStep80Then7
	.4byte 0x00000002
	.4byte 0xffff0008
	.4byte SceneState_SetValue129Mode8
	.4byte 0x00000002
	.4byte 0xffff0009
	.4byte SceneState_SetValue129Mode9
	.4byte 0x00000002
	.4byte 0xffff000a
	.4byte FieldScene_RunStep7BThen10
	.4byte 0x00000002
	.4byte 0xffff000b
	.4byte SceneState_ApplyValues123And11
	.4byte 0x000000d3
	.4byte 0x0f470064
	.4byte 0x001000b4
	.4byte 0x00000023
	.4byte 0x0f480065
	.4byte 0x00200005
	.4byte 0x00000033
	.4byte 0x0f490066
	.4byte 0x00200001
	.4byte 0x000000f3
	.4byte 0xffff00ce
	.4byte 0x004029ca
	.4byte 0x000000f3
	.4byte 0xffff00cf
	.4byte 0x004029cb
	.4byte 0x000000f3
	.4byte 0xffff00d0
	.4byte 0x004029cc
	.4byte 0x0000c4f3
	.4byte 0xffff00d1
	.4byte 0x004029cd
	.4byte 0x0000c4f3
	.4byte 0xffff00d2
	.4byte 0x004029ce
	.4byte 0x000000f3
	.4byte 0xffff00d3
	.4byte 0x004029cf
	.4byte 0x0000c4f3
	.4byte 0xffff00d4
	.4byte 0x004029d0
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global gHaidiaBabiEvents2
gHaidiaBabiEvents2:
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x000011a7
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x000011a8
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte SceneDialogue_RunActorFourteenDialogue11AA
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x000011af
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte HaidiaBabi_RunInnkeeperTalk
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x000011de
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x000011df
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x000011e1
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x000011e2
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x000011e0
	.4byte 0x00000002
	.4byte 0xffff0001
	.4byte SceneState_SetValue123Mode1
	.4byte 0x00000002
	.4byte 0xffff0002
	.4byte FieldScene_RunStep7BThen2
	.4byte 0x00000002
	.4byte 0xffff0003
	.4byte SceneState_SetValue123Mode3
	.4byte 0x00000002
	.4byte 0xffff0004
	.4byte FieldScene_RunStep7BThen4
	.4byte 0x00000002
	.4byte 0xffff0005
	.4byte FieldScene_RunStep80Then5
	.4byte 0x00000002
	.4byte 0xffff0006
	.4byte FieldScene_RunStep7BThen6
	.4byte 0x00000002
	.4byte 0xffff0007
	.4byte FieldScene_RunStep80Then7
	.4byte 0x00000002
	.4byte 0xffff0008
	.4byte SceneState_SetValue129Mode8
	.4byte 0x00000002
	.4byte 0xffff0009
	.4byte SceneState_SetValue129Mode9
	.4byte 0x00000002
	.4byte 0xffff000a
	.4byte FieldScene_RunStep7BThen10
	.4byte 0x00000002
	.4byte 0xffff000b
	.4byte SceneState_ApplyValues123And11
	.4byte 0x000000d3
	.4byte 0x0f470064
	.4byte 0x001000b4
	.4byte 0x00000023
	.4byte 0x0f480065
	.4byte 0x00200005
	.4byte 0x00000033
	.4byte 0x0f490066
	.4byte 0x00200001
	.4byte 0x000000f3
	.4byte 0xffff00ce
	.4byte 0x004029ca
	.4byte 0x000000f3
	.4byte 0xffff00cf
	.4byte 0x004029cb
	.4byte 0x000000f3
	.4byte 0xffff00d0
	.4byte 0x004029cc
	.4byte 0x0000c4f3
	.4byte 0xffff00d1
	.4byte 0x004029cd
	.4byte 0x0000c4f3
	.4byte 0xffff00d2
	.4byte 0x004029ce
	.4byte 0x000000f3
	.4byte 0xffff00d3
	.4byte 0x004029cf
	.4byte 0x0000c4f3
	.4byte 0xffff00d4
	.4byte 0x004029d0
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global gHaidiaBabiEvents3
gHaidiaBabiEvents3:
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x00001c0b
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x00001c0c
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x00001c18
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x00001c19
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte HaidiaBabi_RunInnkeeperTalk
	.4byte 0x00000000
	.4byte 0x03000010
	.4byte SceneDialogue_ShowLine1C13WithActor16Steps
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x00001c13
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00001c10
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00001c11
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x00001c1c
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x00001c1d
	.4byte 0x00008d15
	.4byte 0x0300040d
	.4byte HaidiaBabi_RunInnkeeperTalk
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte SceneDialogue_RunActorThirteenDialogue
	.4byte 0x00008d15
	.4byte 0x03010410
	.4byte SceneDialogue_ShowLine1C13WithActor16Steps
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte SceneDialogue_RunActor16LineAndFlag81c
	.4byte 0x00000000
	.4byte 0x081e0008
	.4byte HaidiaBabi_RunSickbedVisit
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte HaidiaBabi_RunActorEightMessageScene
	.4byte 0x00008d15
	.4byte 0x081e0408
	.4byte HaidiaBabi_RunSickbedVisit
	.4byte 0x00008d15
	.4byte 0x02030008
	.4byte 0x00001c7b
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001c78
	.4byte 0x00000002
	.4byte 0xffff0001
	.4byte SceneState_SetValue123Mode1
	.4byte 0x00000002
	.4byte 0xffff0002
	.4byte FieldScene_RunStep7BThen2
	.4byte 0x00000002
	.4byte 0xffff0003
	.4byte SceneState_SetValue123Mode3
	.4byte 0x00000002
	.4byte 0xffff0004
	.4byte FieldScene_RunStep7BThen4
	.4byte 0x00000002
	.4byte 0xffff0005
	.4byte FieldScene_RunStep80Then5
	.4byte 0x00000002
	.4byte 0xffff0006
	.4byte FieldScene_RunStep7BThen6
	.4byte 0x00000002
	.4byte 0xffff0007
	.4byte FieldScene_RunStep80Then7
	.4byte 0x00000002
	.4byte 0xffff0008
	.4byte SceneState_SetValue129Mode8
	.4byte 0x00000002
	.4byte 0xffff0009
	.4byte SceneState_SetValue129Mode9
	.4byte 0x00000002
	.4byte 0xffff000a
	.4byte FieldScene_RunStep7BThen10
	.4byte 0x00000002
	.4byte 0xffff000b
	.4byte SceneState_ApplyValues123And11
	.4byte 0x000000d3
	.4byte 0x0f470064
	.4byte 0x001000b4
	.4byte 0x00000023
	.4byte 0x0f480065
	.4byte 0x00200005
	.4byte 0x00000033
	.4byte 0x0f490066
	.4byte 0x00200001
	.4byte 0x000000f3
	.4byte 0xffff00ce
	.4byte 0x004029ca
	.4byte 0x000000f3
	.4byte 0xffff00cf
	.4byte 0x004029cb
	.4byte 0x000000f3
	.4byte 0xffff00d0
	.4byte 0x004029cc
	.4byte 0x0000c4f3
	.4byte 0xffff00d1
	.4byte 0x004029cd
	.4byte 0x0000c4f3
	.4byte 0xffff00d2
	.4byte 0x004029ce
	.4byte 0x000000f3
	.4byte 0xffff00d3
	.4byte 0x004029cf
	.4byte 0x0000c4f3
	.4byte 0xffff00d4
	.4byte 0x004029d0
	.4byte 0x00000023
	.4byte 0x08ad006c
	.4byte 0x001000b6
	.4byte 0x00000033
	.4byte 0x08ae006b
	.4byte 0x0020007b
	.4byte 0x00000003
	.4byte 0x08af006d
	.4byte 0x00300000
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global gHaidiaBabiEvents5
gHaidiaBabiEvents5:
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00002017
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00002018
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x00002019
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x0000201a
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x0000201b
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x0000201c
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x0000201d
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x0000201e
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x0000201f
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00002020
	.4byte 0x00000021
	.4byte 0xffff0014
	.4byte 0x00000013
	.4byte 0x00000023
	.4byte 0x0f9e0067
	.4byte 0x001000c2
	.4byte 0x000000f3
	.4byte 0xffff00cf
	.4byte 0x004029b7
	.4byte 0x000000f3
	.4byte 0xffff00d0
	.4byte 0x004029b8
	.4byte 0x000000f3
	.4byte 0xffff00d1
	.4byte 0x004029b9
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global gHaidiaBabiEvents6
gHaidiaBabiEvents6:
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x000022b8
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte HaidiaBabi_AskAboutKraden
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x000022bc
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x000022bd
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x000022be
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x000022bf
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x000022c0
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x000022c1
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x000022c2
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x000022c3
	.4byte 0x00000021
	.4byte 0xffff0014
	.4byte 0x00000013
	.4byte 0x00000023
	.4byte 0x0f9e0067
	.4byte 0x001000c2
	.4byte 0x000000f3
	.4byte 0xffff00cf
	.4byte 0x004029b7
	.4byte 0x000000f3
	.4byte 0xffff00d0
	.4byte 0x004029b8
	.4byte 0x000000f3
	.4byte 0xffff00d1
	.4byte 0x004029b9
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
