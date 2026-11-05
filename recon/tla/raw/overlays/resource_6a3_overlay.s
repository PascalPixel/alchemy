.syntax unified
	.thumb
	.section .text.x02009ed0,"ax",%progbits
	.ifdef TLA_EDITION_JA
	.global Func_02001ed0
	.thumb_func
Func_02001ed0:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	bl Func_020047d4
	movs r0, #0
	bl Func_0200494c
	movs r2, #209
	movs r1, #0
	ldr r0, .L_ja_0200a2e0
	lsls r2, r2, #16
	movs r3, #0
	bl Camera_MoveWithinMapBounds
	ldr r3, .L_ja_0200a2e4
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	movs r2, #4
	str r2, [r3]
	movs r0, #0
	bl Party_RemoveActiveOwner
	movs r0, #2
	bl Party_RemoveActiveOwner
	movs r0, #5
	bl Party_RemoveActiveOwner
	movs r0, #6
	bl Party_RemoveActiveOwner
	movs r0, #7
	bl Party_RemoveActiveOwner
	movs r0, #3
	bl Party_RemoveActiveOwner
	movs r0, #1
	bl Party_RemoveActiveOwner
	movs r0, #4
	bl Party_AddActiveOwner
	movs r1, #128
	movs r2, #128
	movs r0, #4
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl Actor_SetSpeedParameters
	movs r3, #128
	movs r1, #166
	movs r2, #176
	lsls r3, r3, #7
	movs r0, #4
	lsls r1, r1, #18
	lsls r2, r2, #15
	movs r6, #128
	mov r8, r3
	lsls r6, r6, #6
	bl Func_0200484c
	movs r2, #208
	adds r3, r6, #0
	movs r0, #27
	ldr r1, .L_ja_0200a2e8
	lsls r2, r2, #16
	bl Func_0200484c
	movs r2, #195
	adds r3, r6, #0
	movs r0, #26
	ldr r1, .L_ja_0200a2e0
	lsls r2, r2, #16
	movs r5, #160
	bl Func_0200484c
	lsls r5, r5, #8
	movs r2, #219
	adds r3, r5, #0
	movs r0, #0
	ldr r1, .L_ja_0200a2ec
	lsls r2, r2, #16
	bl Func_0200484c
	movs r2, #232
	adds r3, r5, #0
	lsls r2, r2, #16
	movs r0, #2
	ldr r1, .L_ja_0200a2f0
	bl Func_0200484c
	ldr r1, .L_ja_0200a2f4
	movs r0, #3
	bl Actor_SetMovementTypeById
	movs r0, #0
	bl Actor_GetById
	movs r1, #0
	bl Actor_SetSingleChildFlags
	movs r1, #39
	movs r0, #0
	bl Actor_SetModeById
	movs r0, #2
	bl Actor_GetById
	movs r1, #0
	bl Actor_SetSingleChildFlags
	movs r1, #32
	movs r0, #2
	bl Actor_SetModeById
	bl Event_StartScreenTransition
	bl Event_WaitTransitionFrames
	ldr r0, .L_ja_0200a2f8
	bl Func_0200488c
	movs r2, #5
	movs r0, #27
	movs r1, #0
	bl Func_0200489c
	movs r1, #8
	movs r0, #26
	bl Actor_SetModeById
	movs r0, #40
	bl FieldEvent_Wait
	movs r2, #5
	movs r0, #26
	movs r1, #0
	bl Func_0200489c
	movs r1, #0
	movs r0, #27
	bl Func_020048b4
	movs r0, #10
	bl FieldEvent_Wait
	movs r2, #5
	movs r0, #27
	movs r1, #0
	bl Func_0200489c
	movs r0, #0
	movs r1, #2
	bl Actor_StartRepeatedMotion
	movs r1, #2
	movs r0, #2
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl FieldEvent_Wait
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #26
	bl Func_020048cc
	movs r2, #5
	movs r0, #26
	movs r1, #0
	bl Func_0200489c
	movs r1, #224
	lsls r1, r1, #8
	movs r0, #27
	bl Func_020048b4
	movs r0, #10
	bl FieldEvent_Wait
	movs r2, #5
	movs r0, #27
	movs r1, #0
	bl Func_0200489c
	movs r1, #1
	movs r0, #26
	bl Actor_SetModeById
	movs r0, #30
	bl FieldEvent_Wait
	movs r1, #160
	lsls r1, r1, #7
	movs r0, #26
	bl Func_020048b4
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #26
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #10
	bl FieldEvent_Wait
	bl Func_020048fc
	movs r3, #0
	adds r0, #85
	strb r3, [r0]
	movs r1, #1
	movs r0, #4
	bl Camera_FollowActor
	movs r0, #20
	bl FieldEvent_Wait
	mov r3, r8
	movs r2, #0
	movs r0, #6
	movs r1, #0
	bl Func_02004964
	movs r0, #6
	movs r1, #1
	bl Camera_FollowActor
	mov r3, r8
	movs r0, #7
	movs r1, #16
	movs r2, #0
	bl Func_02004964
	movs r1, #16
	adds r3, r6, #0
	movs r0, #28
	negs r1, r1
	movs r2, #24
	bl Func_02004964
	movs r1, #16
	adds r3, r6, #0
	movs r0, #5
	negs r1, r1
	movs r2, #0
	bl Func_02004964
	movs r0, #4
	movs r1, #16
	movs r2, #24
	bl Actor_CommitPositionAndActivate
	mov r1, r8
	movs r0, #4
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #28
	bl Func_020048cc
	movs r2, #5
	movs r0, #28
	movs r1, #0
	bl Func_0200489c
	movs r1, #0
	movs r0, #28
	bl Func_020048b4
	movs r0, #10
	bl FieldEvent_Wait
	movs r1, #128
	movs r0, #4
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r2, #35
	movs r0, #28
	movs r1, #0
	bl Func_0200489c
	movs r0, #4
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #28
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #10
	bl FieldEvent_Wait
	movs r1, #214
	lsls r1, r1, #1
	movs r0, #4
	adds r1, #255
	movs r2, #186
	bl Actor_ResetAndSetPositionInMode2
	movs r1, #206
	lsls r1, r1, #1
	movs r2, #202
	movs r0, #28
	adds r1, #255
	bl Actor_ResetAndSetPositionInMode2
	ldr r1, .L_ja_0200a2fc
	movs r0, #4
	bl Actor_SetMovementTypeById
	ldr r1, .L_ja_0200a300
	movs r0, #28
	bl Actor_SetMovementTypeById
	movs r1, #128
	movs r0, #26
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #128
	movs r0, #27
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #6
	bl Func_020048cc
	movs r0, #6
	movs r1, #0
	movs r2, #5
	bl Func_0200489c
	movs r0, #5
	movs r1, #4
	bl Actor_SetModeById
	movs r0, #33
	bl FieldEvent_Wait
	movs r0, #5
	movs r1, #0
	movs r2, #5
	bl Func_0200489c
	movs r0, #7
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #10
	bl FieldEvent_Wait
	movs r0, #5
	movs r1, #7
	movs r2, #0
	bl Actor_SetAngleToward
	movs r0, #6
	movs r1, #7
	movs r2, #0
	bl Actor_SetAngleToward
	movs r0, #7
	movs r1, #5
	movs r2, #0
	bl Actor_SetAngleToward
	movs r2, #5
	movs r0, #7
	movs r1, #0
	bl Func_0200489c
	movs r0, #6
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #5
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #7
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #7
	movs r1, #0
	movs r2, #40
	bl Actor_OffsetPositionAndReset
	movs r0, #6
	movs r1, #0
	movs r2, #40
	bl Actor_OffsetPositionAndReset
	movs r2, #40
	movs r0, #5
	movs r1, #0
	bl Actor_OffsetPositionAndReset
	ldr r1, .L_ja_0200a304
	movs r0, #7
	bl Actor_SetMovementTypeById
	ldr r1, .L_ja_0200a308
	movs r0, #6
	bl Actor_SetMovementTypeById
	ldr r1, .L_ja_0200a30c
	movs r0, #5
	bl Actor_SetMovementTypeById
	movs r1, #1
	movs r0, #4
	bl Camera_FollowActor
	bl Func_020048f4
	movs r0, #20
	bl FieldEvent_Wait
	movs r1, #8
	adds r1, #255
	movs r2, #30
	movs r0, #26
	bl Func_020048cc
	movs r0, #26
	movs r1, #0
	movs r2, #5
	bl Func_0200489c
	movs r1, #8
	adds r1, #255
	movs r2, #30
	movs r0, #27
	bl Func_020048cc
	movs r2, #5
	movs r0, #27
	movs r1, #0
	bl Func_0200489c
	movs r1, #192
	lsls r1, r1, #8
	movs r0, #26
	bl Func_020048b4
	movs r0, #10
	bl FieldEvent_Wait
	movs r1, #128
	lsls r1, r1, #8
	movs r0, #26
	bl Func_020048b4
	movs r0, #10
	bl FieldEvent_Wait
	mov r1, r8
	movs r0, #26
	bl Func_020048b4
	movs r0, #10
	bl FieldEvent_Wait
	movs r1, #128
	lsls r1, r1, #8
	movs r0, #26
	bl Func_020048b4
	movs r0, #10
	bl FieldEvent_Wait
	movs r1, #192
	lsls r1, r1, #8
	movs r0, #26
	bl Func_020048b4
	movs r0, #10
	bl FieldEvent_Wait
	movs r1, #128
	movs r0, #26
	lsls r1, r1, #8
	bl Func_020048b4
	movs r0, #26
	movs r1, #0
	movs r2, #5
	b .L_ja_0200a310
.L_ja_0200a2e0:
	.4byte 0x02e10000
.L_ja_0200a2e4:
	.4byte gMainRoutineWork
.L_ja_0200a2e8:
	.4byte 0x02cd0000
.L_ja_0200a2ec:
	.4byte 0x02f90000
.L_ja_0200a2f0:
	.4byte 0x02e50000
.L_ja_0200a2f4:
	.4byte gData_02005548
.L_ja_0200a2f8:
	.4byte 0x00002bb8
.L_ja_0200a2fc:
	.4byte gData_02004a6c
.L_ja_0200a300:
	.4byte gData_02004a90
.L_ja_0200a304:
	.4byte gData_02004ab4
.L_ja_0200a308:
	.4byte gData_02004ae8
.L_ja_0200a30c:
	.4byte gData_02004b1c
.L_ja_0200a310:
	bl Func_0200489c
	movs r0, #27
	movs r1, #4
	bl Actor_SetModeById
	movs r0, #33
	bl FieldEvent_Wait
	movs r0, #27
	movs r2, #5
	movs r1, #0
	bl Func_0200489c
	movs r1, #2
	movs r0, #28
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl FieldEvent_Wait
	movs r0, #28
	movs r1, #0
	movs r2, #5
	bl Func_0200489c
	movs r1, #4
	adds r1, #255
	movs r2, #30
	movs r0, #26
	bl Func_020048cc
	movs r0, #26
	movs r1, #0
	movs r2, #5
	bl Func_0200489c
	movs r0, #6
	movs r1, #4
	bl Actor_SetModeById
	movs r0, #33
	bl FieldEvent_Wait
	movs r0, #6
	movs r1, #0
	movs r2, #5
	bl Func_0200489c
	movs r1, #8
	movs r2, #30
	adds r1, #255
	movs r0, #27
	bl Func_020048cc
	movs r1, #0
	movs r0, #27
	bl FieldMessage_ShowAtActor
	movs r0, #28
	movs r1, #4
	movs r2, #0
	bl Actor_SetAngleToward
	movs r0, #5
	movs r1, #4
	movs r2, #0
	bl Actor_SetAngleToward
	movs r0, #6
	movs r1, #4
	movs r2, #0
	bl Actor_SetAngleToward
	movs r1, #4
	movs r0, #7
	movs r2, #0
	bl Actor_SetAngleToward
	movs r0, #10
	bl FieldEvent_Wait
	movs r0, #4
	movs r1, #0
	bl Event_AskYesNoWithActorAnimation
	cmp r0, #0
	bne .L_ja_0200a3ea
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #27
	bl Func_020048cc
	movs r2, #5
	movs r0, #27
	movs r1, #0
	bl Func_0200489c
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_ja_0200a412
.L_ja_0200a3ea:
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #27
	bl Func_020048cc
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #27
	adds r3, #1
	strh r3, [r2]
	movs r1, #0
	movs r2, #5
	bl Func_0200489c
.L_ja_0200a412:
	movs r0, #7
	movs r1, #26
	movs r2, #0
	bl Actor_SetAngleToward
	movs r1, #8
	adds r1, #255
	movs r2, #30
	movs r0, #7
	bl Func_020048cc
	movs r0, #7
	movs r1, #0
	movs r2, #5
	bl Func_0200489c
	movs r0, #26
	movs r1, #4
	bl Actor_SetModeById
	movs r0, #33
	bl FieldEvent_Wait
	movs r0, #26
	movs r1, #0
	movs r2, #5
	bl Func_0200489c
	movs r0, #27
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #10
	bl FieldEvent_Wait
	movs r0, #27
	movs r1, #0
	movs r2, #5
	bl Func_0200489c
	movs r0, #5
	movs r1, #4
	bl Actor_SetModeById
	movs r0, #33
	bl FieldEvent_Wait
	movs r0, #5
	movs r1, #0
	movs r2, #5
	bl Func_0200489c
	movs r1, #27
	movs r2, #0
	movs r0, #26
	bl Actor_TurnToFaceEachOther
	movs r0, #26
	bl Actor_RefreshSelectorById
	movs r0, #27
	bl Actor_RefreshSelectorById
	movs r1, #6
	adds r1, #255
	movs r2, #0
	movs r0, #26
	bl Func_020048cc
	movs r1, #6
	adds r1, #255
	movs r2, #30
	movs r0, #27
	bl Func_020048cc
	movs r0, #28
	movs r1, #27
	movs r2, #0
	bl Actor_SetAngleToward
	movs r0, #28
	movs r1, #4
	bl Actor_SetModeById
	movs r0, #33
	bl FieldEvent_Wait
	movs r0, #26
	movs r1, #28
	movs r2, #0
	bl Actor_SetAngleToward
	movs r0, #27
	movs r1, #28
	movs r2, #0
	bl Actor_SetAngleToward
	movs r1, #0
	movs r2, #5
	movs r0, #28
	bl Func_0200489c
	movs r0, #60
	bl FieldEvent_Wait
	movs r0, #27
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #10
	bl FieldEvent_Wait
	movs r2, #5
	movs r0, #27
	movs r1, #0
	bl Func_0200489c
	movs r1, #2
	movs r0, #26
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl FieldEvent_Wait
	movs r0, #26
	movs r1, #0
	movs r2, #5
	bl Func_0200489c
	movs r0, #27
	movs r1, #4
	bl Actor_SetModeById
	movs r0, #33
	bl FieldEvent_Wait
	movs r0, #27
	movs r1, #0
	movs r2, #5
	bl Func_0200489c
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #26
	bl Func_020048cc
	movs r0, #26
	movs r1, #0
	movs r2, #5
	bl Func_0200489c
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #27
	bl Func_020048cc
	movs r2, #5
	movs r1, #0
	movs r0, #27
	bl Func_0200489c
	movs r0, #20
	bl FieldEvent_Wait
	movs r1, #2
	movs r0, #26
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl FieldEvent_Wait
	movs r2, #5
	movs r0, #26
	movs r1, #0
	bl Func_0200489c
	movs r1, #128
	lsls r1, r1, #8
	movs r0, #26
	bl Func_020048b4
	movs r0, #10
	bl FieldEvent_Wait
	movs r0, #26
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #10
	bl FieldEvent_Wait
	movs r2, #5
	movs r0, #26
	movs r1, #0
	bl Func_0200489c
	movs r1, #160
	lsls r1, r1, #8
	movs r0, #27
	bl Func_020048b4
	movs r0, #10
	bl FieldEvent_Wait
	movs r0, #27
	movs r1, #0
	movs r2, #5
	bl Func_0200489c
	movs r0, #7
	movs r1, #4
	movs r2, #0
	bl Actor_SetAngleToward
	movs r0, #6
	movs r1, #5
	movs r2, #0
	bl Actor_TurnToFaceEachOther
	movs r1, #28
	movs r2, #0
	movs r0, #4
	bl Actor_TurnToFaceEachOther
	movs r0, #4
	bl Actor_RefreshSelectorById
	movs r0, #28
	bl Actor_RefreshSelectorById
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #4
	bl Func_020048cc
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #6
	bl Func_020048cc
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #5
	bl Func_020048cc
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #7
	bl Func_020048cc
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #40
	movs r0, #28
	bl Func_020048cc
	movs r0, #4
	movs r1, #26
	movs r2, #0
	bl Actor_SetAngleToward
	movs r0, #5
	movs r1, #26
	movs r2, #0
	bl Actor_SetAngleToward
	movs r0, #6
	movs r1, #26
	movs r2, #0
	bl Actor_SetAngleToward
	movs r0, #7
	movs r1, #26
	movs r2, #0
	bl Actor_SetAngleToward
	movs r0, #28
	movs r1, #26
	movs r2, #0
	bl Actor_SetAngleToward
	movs r0, #26
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #10
	bl FieldEvent_Wait
	movs r1, #0
	movs r0, #26
	bl FieldMessage_ShowAtActor
	movs r0, #4
	movs r1, #0
	bl Event_AskYesNoWithActorAnimation
	cmp r0, #0
	bne .L_ja_0200a6b6
	movs r1, #4
	movs r0, #26
	bl Actor_SetModeById
	movs r0, #33
	bl FieldEvent_Wait
	movs r0, #26
	movs r2, #5
	movs r1, #0
	bl Func_0200489c
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_ja_0200a6e0
.L_ja_0200a6b6:
	movs r1, #4
	movs r0, #26
	bl Actor_SetModeById
	movs r0, #33
	bl FieldEvent_Wait
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #26
	adds r3, #1
	strh r3, [r2]
	movs r1, #0
	movs r2, #5
	bl Func_0200489c
.L_ja_0200a6e0:
	movs r1, #4
	movs r2, #0
	movs r0, #28
	bl Actor_TurnToFaceEachOther
	movs r0, #28
	bl Actor_RefreshSelectorById
	movs r0, #10
	bl FieldEvent_Wait
	movs r0, #28
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #10
	bl FieldEvent_Wait
	movs r0, #28
	movs r1, #0
	movs r2, #5
	bl Func_0200489c
	movs r1, #28
	movs r2, #0
	movs r0, #4
	bl Actor_TurnToFaceEachOther
	movs r0, #4
	bl Actor_RefreshSelectorById
	movs r0, #10
	bl FieldEvent_Wait
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #4
	bl Func_020048cc
	movs r0, #28
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #40
	bl FieldEvent_Wait
	movs r0, #4
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #30
	bl FieldEvent_Wait
	movs r1, #27
	movs r2, #0
	movs r0, #26
	bl Actor_TurnToFaceEachOther
	movs r0, #26
	bl Actor_RefreshSelectorById
	movs r0, #27
	bl Actor_RefreshSelectorById
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #27
	bl Func_020048cc
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #26
	bl Func_020048cc
	movs r0, #26
	movs r1, #4
	movs r2, #0
	bl Actor_SetAngleToward
	movs r0, #4
	movs r1, #27
	movs r2, #0
	bl Actor_SetAngleToward
	movs r1, #222
	lsls r1, r1, #1
	movs r0, #27
	adds r1, #255
	movs r2, #202
	bl Actor_SetPositionAndReset
	movs r0, #27
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #10
	bl FieldEvent_Wait
	movs r0, #27
	movs r1, #0
	movs r2, #5
	bl Func_0200489c
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #0
	bl Func_020048cc
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #4
	bl Func_020048cc
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #2
	bl Func_020048cc
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #5
	bl Func_020048cc
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #28
	bl Func_020048cc
	movs r1, #10
	adds r1, #255
	movs r2, #30
	movs r0, #28
	bl Func_020048cc
	movs r0, #28
	movs r1, #0
	movs r2, #0
	bl Actor_ArmCallback
	movs r0, #28
	movs r1, #0
	movs r2, #5
	bl Func_0200489c
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #26
	bl Func_020048cc
	movs r0, #26
	movs r1, #0
	movs r2, #5
	bl Func_0200489c
	movs r0, #4
	movs r1, #7
	movs r2, #0
	bl Actor_TurnToFaceEachOther
	movs r1, #5
	movs r2, #0
	movs r0, #6
	bl Actor_TurnToFaceEachOther
	movs r0, #6
	bl Actor_RefreshSelectorById
	movs r0, #5
	bl Actor_RefreshSelectorById
	movs r0, #40
	bl FieldEvent_Wait
	movs r0, #4
	movs r1, #26
	movs r2, #0
	bl Actor_SetAngleToward
	movs r0, #7
	movs r1, #26
	movs r2, #0
	bl Actor_SetAngleToward
	movs r0, #6
	movs r1, #26
	movs r2, #0
	bl Actor_SetAngleToward
	movs r0, #5
	movs r1, #26
	movs r2, #0
	bl Actor_SetAngleToward
	movs r1, #8
	adds r1, #255
	movs r2, #30
	movs r0, #26
	bl Func_020048cc
	movs r0, #27
	movs r1, #0
	movs r2, #0
	bl Actor_ArmCallback
	movs r0, #27
	movs r1, #0
	movs r2, #5
	bl Func_0200489c
	movs r1, #128
	movs r2, #128
	movs r0, #7
	lsls r1, r1, #11
	lsls r2, r2, #10
	bl Actor_SetSpeedParameters
	movs r1, #128
	movs r2, #128
	movs r0, #27
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl Actor_SetSpeedParameters
	movs r0, #7
	movs r1, #8
	movs r2, #24
	bl Actor_OffsetPositionAndReset
	movs r1, #8
	movs r2, #4
	movs r0, #27
	bl Actor_CommitPositionAndActivate
	movs r0, #7
	bl Actor_WaitForMoveById
	movs r0, #5
	bl FieldEvent_Wait
	movs r1, #4
	adds r1, #255
	movs r2, #30
	movs r0, #26
	bl Func_020048cc
	movs r0, #26
	movs r1, #7
	movs r2, #0
	bl Actor_SetAngleToward
	movs r0, #27
	movs r1, #7
	movs r2, #0
	bl Actor_SetAngleToward
	movs r2, #5
	movs r0, #26
	movs r1, #0
	bl Func_0200489c
	movs r1, #2
	movs r0, #0
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl FieldEvent_Wait
	movs r0, #0
	movs r1, #0
	movs r2, #5
	bl Func_0200489c
	movs r0, #26
	movs r1, #0
	movs r2, #0
	bl Actor_SetAngleToward
	movs r2, #0
	movs r0, #27
	movs r1, #0
	bl Actor_SetAngleToward
	movs r1, #2
	movs r0, #0
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl FieldEvent_Wait
	movs r2, #5
	movs r1, #0
	movs r0, #0
	bl Func_0200489c
	movs r0, #40
	bl FieldEvent_Wait
	movs r1, #128
	lsls r1, r1, #6
	movs r0, #4
	bl Func_020048b4
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #4
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #10
	bl FieldEvent_Wait
	movs r0, #26
	movs r1, #4
	bl Actor_WatchAndSetScript
	movs r0, #27
	movs r1, #4
	bl Actor_WatchAndSetScript
	movs r0, #4
	movs r1, #5
	movs r2, #32
	bl Actor_CommitPositionAndActivate
	movs r2, #2
	negs r2, r2
	movs r1, #56
	movs r0, #4
	bl Actor_CommitPositionAndActivate
	movs r0, #30
	bl FieldEvent_Wait
	movs r1, #2
	movs r0, #0
	bl Actor_StartRepeatedMotion
	movs r0, #4
	bl FieldEvent_Wait
	movs r1, #2
	movs r0, #4
	bl Motion_SetVarCbAndRefresh
	movs r0, #223
	bl PartyInventory_Remove
	movs r0, #245
	bl PartyInventory_Add
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #226
	lsls r2, r2, #1
	adds r3, r3, r2
	ldrh r0, [r3]
	movs r1, #5
	adds r2, r0, #1
	lsls r0, r0, #16
	strh r2, [r3]
	asrs r0, r0, #16
	bl FieldMessage_Show
	movs r5, #0
	movs r6, #216
	b .L_ja_0200a9f4
.L_ja_0200a9f0:
	adds r6, #2
	adds r5, #1
.L_ja_0200a9f4:
	cmp r5, #14
	bgt .L_ja_0200aa18
	movs r0, #4
	bl Unit_Get
	ldr r3, .L_ja_0200aa14
	ldrh r2, [r0, r6]
	ands r3, r2
	cmp r3, #245
	bne .L_ja_0200a9f0
	movs r0, #4
	adds r1, r5, #0
	bl Func_020047a4
	b .L_ja_0200aa18
	.2byte 0x0000
.L_ja_0200aa14:
	.4byte 0x000001ff
.L_ja_0200aa18:
	movs r1, #3
	movs r0, #27
	bl Actor_SetModeById
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #10
	bl FieldEvent_Wait
	movs r0, #27
	movs r1, #0
	movs r2, #5
	bl Func_0200489c
	movs r1, #27
	movs r2, #0
	movs r0, #4
	bl Actor_SetAngleToward
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #4
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #20
	bl FieldEvent_Wait
	movs r1, #128
	movs r2, #128
	movs r0, #7
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl Actor_SetSpeedParameters
	movs r1, #128
	movs r2, #128
	movs r0, #26
	lsls r1, r1, #8
	lsls r2, r2, #7
	bl Actor_SetSpeedParameters
	movs r1, #128
	movs r2, #128
	movs r0, #27
	lsls r1, r1, #8
	lsls r2, r2, #7
	bl Actor_SetSpeedParameters
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #7
	movs r0, #4
	lsls r1, r1, #8
	bl Actor_SetSpeedParameters
	ldr r6, .L_ja_0200ac48
	movs r0, #26
	adds r1, r6, #0
	bl Actor_SetMovementTypeById
	movs r0, #40
	bl FieldEvent_Wait
	movs r1, #16
	movs r2, #0
	negs r1, r1
	movs r0, #7
	bl Actor_OffsetPositionAndResetMotion
	movs r0, #10
	bl FieldEvent_Wait
	adds r1, r6, #0
	movs r0, #27
	bl Actor_SetMovementTypeById
	movs r0, #110
	bl FieldEvent_Wait
	adds r1, r6, #0
	movs r0, #4
	bl Actor_SetMovementTypeById
	movs r0, #26
	bl Actor_RefreshSelectorById
	ldr r5, .L_ja_0200ac4c
	movs r0, #26
	adds r1, r5, #0
	bl Actor_SetMovementTypeById
	movs r0, #27
	bl Actor_RefreshSelectorById
	adds r1, r5, #0
	movs r0, #27
	bl Actor_SetMovementTypeById
	movs r0, #4
	bl Actor_RefreshSelectorById
	movs r0, #4
	movs r1, #1
	bl Actor_SetModeById
	movs r0, #7
	movs r1, #2
	bl Actor_StartRepeatedMotion
	movs r1, #192
	movs r0, #7
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #192
	movs r0, #28
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r2, #5
	movs r0, #7
	movs r1, #0
	bl Func_0200489c
	movs r0, #6
	movs r1, #1
	bl Camera_FollowActor
	movs r1, #128
	lsls r1, r1, #7
	movs r0, #4
	bl Func_020048b4
	movs r0, #10
	bl FieldEvent_Wait
	movs r1, #2
	adds r1, #255
	movs r2, #30
	movs r0, #4
	bl Func_020048cc
	movs r1, #10
	adds r1, #255
	movs r2, #30
	movs r0, #7
	bl Func_020048cc
	movs r0, #7
	movs r1, #0
	movs r2, #5
	bl Func_0200489c
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #4
	bl Func_020048cc
	movs r1, #131
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #6
	bl Func_020048cc
	movs r2, #5
	movs r0, #6
	movs r1, #0
	bl Func_0200489c
	movs r1, #128
	lsls r1, r1, #7
	movs r0, #4
	bl Func_020048b4
	movs r0, #10
	bl FieldEvent_Wait
	movs r0, #5
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #10
	bl FieldEvent_Wait
	movs r1, #192
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r2, #5
	movs r0, #5
	movs r1, #0
	bl Func_0200489c
	movs r1, #128
	lsls r1, r1, #7
	movs r0, #4
	bl Func_020048b4
	movs r0, #10
	bl FieldEvent_Wait
	movs r0, #28
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #10
	bl FieldEvent_Wait
	movs r0, #28
	movs r1, #0
	movs r2, #5
	bl Func_0200489c
	movs r0, #4
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #10
	bl FieldEvent_Wait
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #8
	movs r0, #7
	lsls r1, r1, #9
	bl Actor_SetSpeedParameters
	adds r1, r6, #0
	movs r0, #7
	bl Actor_SetMovementTypeById
	movs r0, #7
	bl Actor_RefreshSelectorById
	movs r0, #7
	bl Party_AddActiveOwner
	movs r0, #4
	movs r1, #1
	bl Camera_FollowActor
	movs r1, #0
	movs r2, #0
	movs r0, #7
	bl Func_02004844
	movs r0, #20
	bl FieldEvent_Wait
	movs r1, #166
	movs r0, #4
	lsls r1, r1, #2
	movs r2, #70
	bl Actor_SetPositionAndReset
	bl Func_020047dc
	pop {r3}
	mov r8, r3
	pop {r5, r6, pc}
.L_ja_0200ac48:
	.4byte gData_020059a4
.L_ja_0200ac4c:
	.4byte gData_02005a00
	.endif
	.section .text.x0200aefa,"ax",%progbits
	.2byte 0x0000
	.section .text.x0200bb42,"ax",%progbits
	.2byte 0x0000
	.section .text.x0200bb7a,"ax",%progbits
	.2byte 0x0000
	.section .text.x0200c342,"ax",%progbits
	.2byte 0x0000
	.section .bss,"aw",%nobits
	.global gData_02005a3c
gData_02005a3c:
	.space 0x00000004
	.global gData_02005a40
gData_02005a40:
	.space 0x00000020
	.global gData_02005a60
gData_02005a60:
	.space 0x00000004
	.global gData_02005a64
gData_02005a64:
	.space 0x0000001c
	.global gData_02005a80
gData_02005a80:
	.space 0x00000002
	.global gData_02005a82
gData_02005a82:
	.space 0x00000002
	.global gData_02005a84
gData_02005a84:
	.space 0x00000040
	.global Data_02005ac4
Data_02005ac4:
	.space 0x000000c0
	.global gData_02005b84
gData_02005b84:
	.space 0x00000002
	.global gData_02005b86
gData_02005b86:
	.space 0x00000002
	.global Data_02005b88
Data_02005b88:
