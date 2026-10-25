.syntax unified
	.thumb
	.section .text.x0200861c,"ax",%progbits
	.balign 4
	.global Func_0200061c
	.thumb_func
Func_0200061c:
	push {r5, r6, r7, lr}
	adds r7, r0, #0
	bl Func_020021c4
	movs r0, #0
	bl Func_020022bc
	movs r0, #143
	movs r1, #1
	movs r2, #140
	movs r3, #1
	lsls r0, r0, #16
	negs r1, r1
	lsls r2, r2, #18
	bl Camera_MoveWithinMapBounds
	ldr r3, .L_02008930
	cmp r7, #0
	bne .L_02008654
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	movs r1, #184
	adds r2, #36
	bl ObjectMotion_SetPositionAndReset
	b .L_02008664
.L_02008654:
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	movs r1, #104
	adds r2, #36
	bl ObjectMotion_SetPositionAndReset
.L_02008664:
	movs r0, #15
	bl FieldEvent_WaitFrames
	ldr r0, .L_02008934
	bl Func_02002244
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #15
	movs r1, #0
	bl Func_02002254
	movs r0, #22
	movs r1, #3
	bl Object_SetModeById
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #22
	movs r1, #0
	bl Func_02002254
	movs r1, #128
	movs r2, #30
	lsls r1, r1, #1
	movs r0, #15
	bl Func_0200226c
	movs r0, #128
	lsls r0, r0, #8
	movs r1, #0
	adds r0, #15
	bl Func_02002254
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r1, #3
	movs r0, #22
	bl Object_SetModeById
	movs r0, #15
	bl FieldEvent_WaitFrames
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #22
	movs r1, #0
	bl Func_02002254
	movs r1, #4
	movs r2, #30
	adds r1, #255
	movs r0, #15
	bl Func_0200226c
	movs r0, #22
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #22
	bl Func_02002274
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #22
	movs r1, #0
	bl Func_02002254
	movs r2, #30
	movs r0, #20
	movs r1, #0
	bl ObjectMotion_ArmCallback
	movs r0, #20
	movs r1, #4
	bl Object_SetModeById
	movs r0, #128
	lsls r0, r0, #8
	movs r1, #0
	adds r0, #20
	bl Func_02002254
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r1, #128
	movs r2, #30
	movs r0, #15
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r1, #3
	movs r0, #15
	bl Object_SetModeById
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #15
	movs r1, #0
	bl Func_02002254
	movs r1, #3
	movs r0, #22
	bl Object_SetModeById
	movs r0, #15
	bl FieldEvent_WaitFrames
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #22
	movs r1, #0
	bl Func_02002254
	movs r1, #2
	movs r0, #20
	bl ObjectMotion_SetVariantCallback
	movs r0, #15
	bl FieldEvent_WaitFrames
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #20
	movs r1, #0
	bl Func_02002254
	ldr r3, .L_02008930
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r1, [r3]
	movs r2, #0
	movs r0, #15
	bl ObjectMotion_SetAngleToward
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r1, #0
	movs r2, #16
	movs r0, #15
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r0, #15
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #15
	bl FieldEvent_WaitFrames
	movs r1, #128
	movs r0, #15
	lsls r1, r1, #1
	movs r2, #30
	bl Func_0200226c
	cmp r7, #0
	bne .L_020087c2
	movs r2, #134
	movs r0, #15
	movs r1, #168
	lsls r2, r2, #2
	bl ObjectMotion_SetPositionAndReset
	b .L_020087ce
.L_020087c2:
	movs r2, #134
	movs r0, #15
	movs r1, #120
	lsls r2, r2, #2
	bl ObjectMotion_SetPositionAndReset
.L_020087ce:
	movs r1, #128
	movs r0, #15
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	ldr r5, .L_02008930
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	movs r2, #0
	ldr r1, [r5]
	movs r0, #20
	bl ObjectMotion_SetAngleToward
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #15
	movs r1, #0
	bl Func_02002254
	movs r1, #8
	movs r2, #0
	adds r1, #255
	movs r0, #15
	bl Func_0200226c
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #15
	movs r1, #0
	bl Func_02002254
	movs r2, #0
	movs r0, #22
	movs r1, #4
	bl ObjectMotion_Launch
	movs r1, #2
	adds r1, #255
	movs r0, #22
	bl Func_02002274
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r1, #128
	movs r0, #20
	lsls r1, r1, #7
	movs r2, #30
	bl ObjectMotion_ArmCallback
	movs r1, #2
	movs r2, #30
	adds r1, #255
	movs r0, #20
	bl Func_0200226c
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #20
	movs r1, #0
	bl Func_02002254
	movs r2, #0
	movs r0, #22
	movs r1, #20
	bl ObjectMotion_SetAngleToward
	movs r1, #128
	movs r0, #22
	lsls r1, r1, #1
	bl Func_02002274
	movs r1, #0
	movs r0, #22
	bl Func_02002254
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #15
	movs r1, #0
	bl Func_02002254
	movs r1, #2
	movs r0, #20
	bl ObjectMotion_SetVariantCallback
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r2, #0
	ldr r1, [r5]
	movs r0, #20
	bl ObjectMotion_SetAngleToward
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #20
	movs r1, #0
	bl Func_02002254
	movs r1, #132
	movs r2, #30
	lsls r1, r1, #1
	movs r0, #15
	bl Func_0200226c
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #15
	movs r1, #0
	bl Func_02002254
	movs r2, #130
	movs r1, #144
	lsls r2, r2, #2
	movs r0, #15
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r0, #15
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r1, #128
	movs r0, #15
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #20
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	ldr r0, [r5]
	movs r1, #3
	bl Object_SetModeById
	movs r0, #30
	bl FieldEvent_WaitFrames
	cmp r7, #0
	bne .L_0200890e
	movs r2, #142
	movs r0, #22
	movs r1, #104
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPositionInMode2
	b .L_0200891a
.L_0200890e:
	movs r2, #142
	movs r0, #22
	movs r1, #184
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPositionInMode2
.L_0200891a:
	movs r0, #22
	bl ObjectMotion_CommitCurrentPositionAndActivate
	cmp r7, #0
	bne .L_02008938
	movs r0, #22
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	b .L_02008944
.L_02008930:
	.4byte gPartyState
.L_02008934:
	.4byte MsgWarriorsAppearedFrom
.L_02008938:
	movs r1, #128
	movs r0, #22
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
.L_02008944:
	ldr r3, .L_02008d3c
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r3, r2
	movs r1, #144
	adds r2, #28
	ldr r0, [r5]
	bl ObjectMotion_ResetAndSetPositionInMode2
	ldr r0, [r5]
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r1, #192
	ldr r0, [r5]
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r3, #192
	movs r0, #5
	movs r1, #8
	movs r2, #24
	lsls r3, r3, #8
	bl Func_020022cc
	movs r3, #192
	movs r0, #6
	movs r1, #24
	movs r2, #24
	lsls r3, r3, #8
	bl Func_020022cc
	movs r1, #8
	movs r3, #192
	movs r0, #7
	negs r1, r1
	movs r2, #24
	lsls r3, r3, #8
	bl Func_020022cc
	movs r1, #24
	movs r3, #192
	lsls r3, r3, #8
	movs r2, #24
	negs r1, r1
	movs r0, #23
	bl Func_020022cc
	movs r0, #60
	bl FieldEvent_WaitFrames
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #20
	movs r1, #0
	bl Func_02002254
	movs r1, #3
	movs r0, #23
	bl Object_SetModeById
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r0, #23
	movs r1, #0
	bl Func_02002254
	movs r1, #3
	ldr r0, [r5]
	bl Object_SetModeById
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r0, #15
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #20
	bl Object_SetModeById
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r2, #0
	movs r1, #7
	movs r0, #23
	bl ObjectMotion_SetAngleToward
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r0, #23
	movs r1, #0
	bl Func_02002254
	movs r1, #3
	movs r0, #7
	bl Object_SetModeById
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r0, #7
	movs r1, #0
	bl Func_02002254
	movs r0, #15
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #20
	bl Object_SetModeById
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r1, #3
	movs r0, #5
	bl Object_SetModeById
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r0, #5
	movs r1, #0
	bl Func_02002254
	movs r1, #3
	movs r0, #6
	bl Object_SetModeById
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r0, #15
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #20
	bl Object_SetModeById
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r2, #0
	movs r0, #5
	movs r1, #23
	bl ObjectMotion_SetAngleToward
	movs r1, #3
	movs r0, #23
	bl Object_SetModeById
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r0, #23
	movs r1, #0
	bl Func_02002254
	movs r1, #3
	movs r0, #20
	bl Object_SetModeById
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #20
	movs r1, #0
	bl Func_02002254
	movs r1, #3
	movs r0, #6
	bl Object_SetModeById
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r0, #6
	movs r1, #0
	bl Func_02002254
	movs r1, #2
	movs r2, #30
	adds r1, #255
	movs r0, #15
	bl Func_0200226c
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #15
	movs r1, #0
	bl Func_02002254
	movs r1, #20
	movs r2, #0
	movs r0, #15
	bl Object_LinkPair
	movs r0, #60
	bl FieldEvent_WaitFrames
	ldr r1, [r5]
	movs r0, #15
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r2, #0
	ldr r1, [r5]
	movs r0, #20
	bl ObjectMotion_SetAngleToward
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r1, #4
	movs r0, #20
	bl Object_SetModeById
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #20
	movs r1, #0
	bl Func_02002254
	ldr r0, [r5]
	movs r1, #23
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #5
	movs r1, #23
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #6
	movs r1, #23
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r1, #23
	movs r2, #0
	movs r0, #7
	bl ObjectMotion_SetAngleToward
	movs r0, #30
	bl FieldEvent_WaitFrames
	ldr r1, [r5]
	movs r2, #0
	movs r0, #23
	bl ObjectMotion_SetAngleToward
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r2, #0
	movs r1, #15
	movs r0, #23
	bl ObjectMotion_SetAngleToward
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r0, #23
	movs r1, #0
	bl Func_02002254
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #15
	bl Func_0200226c
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #1
	movs r0, #20
	bl Func_0200226c
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #15
	movs r1, #0
	bl Func_02002254
	ldr r0, [r5]
	movs r1, #15
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #5
	movs r1, #15
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #6
	movs r1, #15
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r2, #0
	movs r1, #15
	movs r0, #7
	bl ObjectMotion_SetAngleToward
	movs r0, #30
	bl FieldEvent_WaitFrames
	ldr r0, [r5]
	movs r1, #3
	bl Object_SetModeById
	movs r0, #5
	movs r1, #3
	bl Object_SetModeById
	movs r0, #6
	movs r1, #3
	bl Object_SetModeById
	movs r0, #7
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #23
	bl Object_SetModeById
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r1, #129
	movs r2, #30
	lsls r1, r1, #1
	movs r0, #15
	bl Func_0200226c
	movs r0, #128
	lsls r0, r0, #8
	movs r1, #0
	adds r0, #15
	bl Func_02002254
	movs r0, #60
	bl FieldEvent_WaitFrames
	movs r1, #0
	movs r2, #0
	movs r0, #15
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl FieldEvent_WaitFrames
	bl Func_02001d8c
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #15
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #15
	movs r1, #0
	bl Func_02002254
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	ldr r0, [r5]
	bl Func_0200226c
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #5
	bl Func_0200226c
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #6
	bl Func_0200226c
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #7
	bl Func_0200226c
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #1
	movs r0, #23
	bl Func_0200226c
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r1, #1
	movs r0, #5
	bl ObjectMotion_SetVariantCallback
	movs r0, #15
	bl FieldEvent_WaitFrames
	movs r0, #5
	movs r1, #0
	bl Func_02002254
	movs r1, #20
	movs r2, #0
	movs r0, #15
	bl ObjectMotion_SetAngleToward
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r2, #0
	ldr r1, [r5]
	movs r0, #15
	bl ObjectMotion_SetAngleToward
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #15
	movs r1, #0
	bl Func_02002254
	movs r1, #2
	adds r1, #255
	movs r2, #30
	movs r0, #20
	bl Func_0200226c
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r0, #128
	lsls r0, r0, #8
	movs r1, #0
	adds r0, #20
	bl UiText_OpenMessageAtObject
	ldr r0, [r5]
	movs r1, #0
	bl Event_AskYesNoWithActorAnimation
	cmp r0, #1
	bne .L_02008d5e
	ldr r0, .L_02008d40
	bl Func_02002244
	movs r1, #128
	movs r2, #30
	lsls r1, r1, #1
	movs r0, #20
	bl Func_0200226c
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #20
	movs r1, #0
	bl Func_02002254
	movs r0, #6
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #5
	bl Object_SetModeById
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r2, #0
	ldr r1, [r5]
	movs r0, #7
	bl ObjectMotion_SetAngleToward
	movs r0, #15
	b .L_02008d44
.L_02008d3c:
	.4byte gPartyState
.L_02008d40:
	.4byte MsgIThoughtSo
.L_02008d44:
	bl FieldEvent_WaitFrames
	movs r1, #4
	movs r0, #7
	bl Object_SetModeById
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r0, #7
	movs r1, #0
	bl Func_02002254
.L_02008d5e:
	ldr r0, .L_020090c8
	bl Func_02002244
	movs r1, #3
	movs r0, #20
	bl Object_SetModeById
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #20
	movs r1, #0
	bl Func_02002254
	ldr r0, [r5]
	movs r1, #3
	bl Object_SetModeById
	movs r0, #5
	movs r1, #3
	bl Object_SetModeById
	movs r0, #6
	movs r1, #3
	bl Object_SetModeById
	movs r0, #7
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #23
	bl Object_SetModeById
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r0, #15
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #20
	bl Object_SetModeById
	movs r0, #60
	bl FieldEvent_WaitFrames
	movs r1, #2
	adds r1, #255
	movs r2, #30
	movs r0, #20
	bl Func_0200226c
	movs r2, #0
	ldr r1, [r5]
	movs r0, #20
	bl ObjectMotion_SetAngleToward
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r0, #128
	lsls r0, r0, #8
	movs r1, #0
	adds r0, #20
	bl Func_02002254
	movs r0, #90
	bl FieldEvent_WaitFrames
	movs r0, #21
	movs r1, #0
	bl Func_02002254
	movs r1, #144
	movs r2, #170
	movs r0, #21
	lsls r1, r1, #16
	lsls r2, r2, #18
	bl Func_02002214
	ldr r0, [r5]
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #5
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #6
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #7
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #23
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r2, #0
	movs r1, #21
	movs r0, #22
	bl ObjectMotion_SetAngleToward
	movs r0, #120
	bl FieldEvent_WaitFrames
	movs r0, #128
	lsls r0, r0, #8
	movs r1, #0
	adds r0, #15
	bl Func_02002254
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r1, #1
	movs r0, #21
	bl Camera_FollowActor
	movs r0, #120
	bl FieldEvent_WaitFrames
	movs r1, #153
	movs r2, #152
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #21
	adds r1, #153
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r1, #153
	movs r2, #152
	lsls r1, r1, #8
	lsls r2, r2, #7
	adds r1, #153
	adds r2, #204
	movs r0, #21
	bl ObjectMotion_SetSpeedParameters
	movs r0, #21
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r6, #254
	adds r3, r6, #0
	ands r3, r2
	movs r2, #162
	strb r3, [r0]
	movs r1, #144
	movs r0, #21
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPositionInMode2
	cmp r7, #0
	beq .L_02008eb0
	b .L_020090cc
.L_02008eb0:
	movs r2, #162
	movs r0, #21
	movs r1, #88
	lsls r2, r2, #2
	bl ObjectMotion_SetPositionAndReset
	ldr r0, [r5]
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #5
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #6
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #7
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #23
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #15
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #20
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #22
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r2, #154
	movs r0, #21
	movs r1, #56
	lsls r2, r2, #2
	bl ObjectMotion_SetPositionAndReset
	ldr r0, [r5]
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #5
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #6
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #7
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #23
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #15
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #20
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #22
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r2, #146
	movs r1, #56
	lsls r2, r2, #2
	movs r0, #21
	bl ObjectMotion_SetPositionAndReset
	movs r0, #22
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	adds r3, r6, #0
	ands r3, r2
	movs r2, #142
	strb r3, [r0]
	movs r1, #88
	lsls r2, r2, #2
	movs r0, #22
	bl ObjectMotion_SetPositionAndReset
	movs r0, #1
	bl FieldEvent_WaitFrames
	movs r0, #22
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	movs r2, #1
	orrs r3, r2
	strb r3, [r0]
	movs r2, #0
	movs r0, #22
	movs r1, #0
	bl ObjectMotion_ArmCallback
	movs r0, #21
	movs r1, #1
	bl ObjectMotion_SetActionVariant
	movs r2, #142
	movs r0, #21
	movs r1, #104
	lsls r2, r2, #2
	bl ObjectMotion_SetPositionAndReset
	ldr r0, [r5]
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #5
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #6
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #7
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #23
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #15
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #20
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r2, #0
	movs r0, #22
	movs r1, #21
	bl ObjectMotion_SetAngleToward
	movs r0, #23
	movs r1, #1
	bl ObjectMotion_SetActionVariant
	movs r0, #21
	movs r1, #2
	bl ObjectMotion_SetActionVariant
	movs r2, #142
	lsls r2, r2, #2
	movs r0, #21
	movs r1, #120
	bl ObjectMotion_SetPositionAndReset
	movs r0, #21
	movs r1, #3
	bl ObjectMotion_SetActionVariant
	movs r0, #23
	movs r1, #3
	bl ObjectMotion_SetActionVariant
	movs r2, #134
	movs r0, #21
	movs r1, #120
	lsls r2, r2, #2
	bl ObjectMotion_SetPositionAndReset
	ldr r0, [r5]
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #5
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #6
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #7
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #23
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #15
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #20
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r2, #0
	movs r0, #22
	movs r1, #21
	bl ObjectMotion_SetAngleToward
	movs r0, #22
	movs r1, #3
	bl Object_SetModeById
	movs r2, #146
	movs r0, #22
	movs r1, #56
	lsls r2, r2, #2
	bl ObjectMotion_SetPositionAndReset
	movs r2, #162
	movs r0, #22
	movs r1, #88
	lsls r2, r2, #2
	bl ObjectMotion_SetPositionAndReset
	movs r0, #22
	movs r1, #0
	movs r2, #0
	bl Func_02002214
	b .L_020092e4
.L_020090c8:
	.4byte MsgSoYouCall
.L_020090cc:
	movs r2, #162
	movs r0, #21
	movs r1, #200
	lsls r2, r2, #2
	bl ObjectMotion_SetPositionAndReset
	ldr r0, [r5]
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #5
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #6
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #7
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #23
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #15
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #20
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #22
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r2, #154
	movs r0, #21
	movs r1, #232
	lsls r2, r2, #2
	bl ObjectMotion_SetPositionAndReset
	ldr r0, [r5]
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #5
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #6
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #7
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #23
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #15
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #20
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #22
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r2, #146
	movs r1, #232
	lsls r2, r2, #2
	movs r0, #21
	bl ObjectMotion_SetPositionAndReset
	movs r0, #22
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	adds r3, r6, #0
	ands r3, r2
	movs r2, #142
	strb r3, [r0]
	movs r1, #200
	lsls r2, r2, #2
	movs r0, #22
	bl ObjectMotion_SetPositionAndReset
	movs r0, #1
	bl FieldEvent_WaitFrames
	movs r0, #22
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	movs r2, #1
	orrs r3, r2
	movs r1, #128
	strb r3, [r0]
	movs r2, #0
	movs r0, #22
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r0, #21
	movs r1, #1
	bl ObjectMotion_SetActionVariant
	movs r2, #142
	movs r0, #21
	movs r1, #184
	lsls r2, r2, #2
	bl ObjectMotion_SetPositionAndReset
	ldr r0, [r5]
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #5
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #6
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #7
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #23
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #15
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #20
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r2, #0
	movs r0, #22
	movs r1, #21
	bl ObjectMotion_SetAngleToward
	movs r0, #21
	movs r1, #2
	bl ObjectMotion_SetActionVariant
	movs r0, #6
	movs r1, #1
	bl ObjectMotion_SetActionVariant
	movs r2, #142
	lsls r2, r2, #2
	movs r0, #21
	movs r1, #168
	bl ObjectMotion_SetPositionAndReset
	movs r0, #21
	movs r1, #3
	bl ObjectMotion_SetActionVariant
	movs r0, #6
	movs r1, #3
	bl ObjectMotion_SetActionVariant
	movs r2, #134
	movs r0, #21
	movs r1, #168
	lsls r2, r2, #2
	bl ObjectMotion_SetPositionAndReset
	ldr r0, [r5]
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #5
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #6
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #7
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #23
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #15
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #20
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r2, #0
	movs r0, #22
	movs r1, #21
	bl ObjectMotion_SetAngleToward
	movs r0, #22
	movs r1, #3
	bl Object_SetModeById
	movs r2, #146
	movs r0, #22
	movs r1, #232
	lsls r2, r2, #2
	bl ObjectMotion_SetPositionAndReset
	movs r2, #162
	movs r0, #22
	movs r1, #200
	lsls r2, r2, #2
	bl ObjectMotion_SetPositionAndReset
	movs r0, #22
	movs r1, #0
	movs r2, #0
	bl Func_02002214
.L_020092e4:
	movs r0, #21
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	ldr r5, .L_02009678
	strb r3, [r0]
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	movs r1, #192
	ldr r0, [r5]
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #7
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #23
	bl ObjectMotion_ArmCallback
	movs r0, #15
	bl FieldEvent_WaitFrames
	movs r1, #0
	movs r0, #20
	bl Func_02002254
	movs r0, #15
	bl FieldEvent_WaitFrames
	movs r1, #132
	movs r2, #30
	lsls r1, r1, #1
	movs r0, #20
	bl Func_0200226c
	movs r0, #20
	movs r1, #0
	bl Func_02002254
	movs r2, #0
	movs r1, #20
	movs r0, #21
	bl ObjectMotion_SetAngleToward
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r0, #21
	movs r1, #0
	bl Func_02002254
	ldr r1, [r5]
	movs r2, #0
	movs r0, #21
	bl ObjectMotion_SetAngleToward
	movs r0, #60
	bl FieldEvent_WaitFrames
	movs r2, #0
	movs r1, #20
	movs r0, #21
	bl ObjectMotion_SetAngleToward
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r0, #21
	movs r1, #0
	bl Func_02002254
	movs r1, #20
	movs r2, #0
	movs r0, #15
	bl Object_LinkPair
	movs r0, #30
	bl FieldEvent_WaitFrames
	ldr r1, [r5]
	movs r0, #15
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r2, #0
	ldr r1, [r5]
	movs r0, #20
	bl ObjectMotion_SetAngleToward
	movs r0, #15
	movs r1, #0
	bl Func_02002254
	movs r0, #15
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #20
	bl Object_SetModeById
	movs r0, #30
	bl FieldEvent_WaitFrames
	ldr r0, [r5]
	movs r1, #3
	bl Object_SetModeById
	movs r0, #5
	movs r1, #3
	bl Object_SetModeById
	movs r0, #6
	movs r1, #3
	bl Object_SetModeById
	movs r0, #7
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #23
	bl Object_SetModeById
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r1, #2
	movs r2, #30
	adds r1, #255
	movs r0, #15
	bl Func_0200226c
	movs r0, #15
	movs r1, #0
	bl Func_02002254
	movs r2, #0
	ldr r1, [r5]
	movs r0, #21
	bl ObjectMotion_SetAngleToward
	movs r0, #21
	movs r1, #0
	bl Func_02002254
	ldr r1, [r5]
	movs r0, #15
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	ldr r1, [r5]
	movs r2, #0
	movs r0, #20
	bl ObjectMotion_SetAngleToward
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r1, #129
	movs r2, #30
	lsls r1, r1, #1
	ldr r0, [r5]
	bl Func_0200226c
	movs r1, #3
	movs r0, #23
	bl Object_SetModeById
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r0, #23
	movs r1, #0
	bl Func_02002254
	movs r0, #15
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #20
	bl Object_SetModeById
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r0, #23
	movs r1, #3
	bl Object_SetModeById
	movs r0, #23
	movs r1, #0
	bl Func_02002254
	movs r1, #2
	adds r1, #255
	movs r2, #0
	movs r0, #15
	bl Func_0200226c
	movs r1, #2
	adds r1, #255
	movs r2, #0
	movs r0, #20
	bl Func_0200226c
	movs r1, #2
	adds r1, #255
	movs r2, #0
	movs r0, #21
	bl Func_0200226c
	movs r1, #2
	adds r1, #255
	movs r2, #0
	movs r0, #22
	bl Func_0200226c
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r1, #128
	movs r2, #30
	lsls r1, r1, #1
	movs r0, #23
	bl Func_0200226c
	movs r0, #23
	movs r1, #0
	bl Func_02002254
	movs r1, #2
	movs r2, #30
	adds r1, #255
	movs r0, #21
	bl Func_0200226c
	movs r0, #21
	movs r1, #0
	bl Func_02002254
	movs r2, #0
	movs r1, #21
	movs r0, #23
	bl ObjectMotion_SetAngleToward
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r0, #23
	movs r1, #0
	bl Func_02002254
	movs r1, #131
	movs r2, #30
	lsls r1, r1, #1
	movs r0, #20
	bl Func_0200226c
	movs r0, #20
	movs r1, #0
	bl Func_02002254
	movs r1, #132
	movs r2, #30
	lsls r1, r1, #1
	movs r0, #23
	bl Func_0200226c
	movs r0, #23
	movs r1, #0
	bl Func_02002254
	movs r1, #3
	movs r0, #23
	bl Object_SetModeById
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r0, #23
	movs r1, #0
	bl Func_02002254
	movs r1, #2
	movs r2, #30
	adds r1, #255
	movs r0, #15
	bl Func_0200226c
	movs r0, #15
	movs r1, #0
	bl Func_02002254
	movs r2, #0
	movs r1, #23
	movs r0, #7
	bl ObjectMotion_SetAngleToward
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r0, #7
	movs r1, #0
	bl Func_02002254
	movs r2, #0
	movs r0, #23
	movs r1, #7
	bl ObjectMotion_SetAngleToward
	movs r1, #3
	movs r0, #23
	bl Object_SetModeById
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r1, #2
	adds r1, #255
	movs r2, #30
	movs r0, #15
	bl Func_0200226c
	movs r0, #23
	movs r1, #15
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r2, #0
	movs r1, #15
	movs r0, #7
	bl ObjectMotion_SetAngleToward
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r0, #15
	movs r1, #0
	bl Func_02002254
	movs r1, #132
	movs r2, #30
	lsls r1, r1, #1
	movs r0, #6
	bl Func_0200226c
	movs r0, #6
	movs r1, #0
	bl Func_02002254
	movs r1, #128
	movs r2, #30
	lsls r1, r1, #1
	movs r0, #21
	bl Func_0200226c
	movs r0, #21
	movs r1, #0
	bl Func_02002254
	movs r1, #3
	movs r0, #5
	bl Object_SetModeById
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r0, #5
	movs r1, #0
	bl Func_02002254
	movs r1, #3
	movs r0, #20
	bl Object_SetModeById
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r0, #20
	movs r1, #0
	bl Func_02002254
	ldr r0, [r5]
	movs r1, #3
	bl Object_SetModeById
	movs r0, #5
	movs r1, #3
	bl Object_SetModeById
	movs r0, #6
	movs r1, #3
	bl Object_SetModeById
	movs r0, #7
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #23
	bl Object_SetModeById
	movs r0, #90
	bl FieldEvent_WaitFrames
	movs r0, #15
	movs r1, #3
	bl Object_SetModeById
	movs r1, #0
	movs r0, #15
	bl UiText_OpenMessageAtObject
	ldr r0, [r5]
	movs r1, #0
	bl Event_AskYesNoWithActorAnimation
	cmp r0, #0
	bne .L_02009680
	ldr r0, .L_0200967c
	bl Func_02002244
	movs r1, #132
	lsls r1, r1, #1
	movs r0, #15
	movs r2, #30
	bl Func_0200226c
	movs r0, #15
	movs r1, #0
	bl Func_02002254
	b .L_0200968e
.L_02009678:
	.4byte gPartyState
.L_0200967c:
	.4byte MsgRegardlessOfYour
.L_02009680:
	ldr r0, .L_02009948
	bl Func_02002244
	movs r0, #15
	movs r1, #0
	bl Func_02002254
.L_0200968e:
	ldr r6, .L_0200994c
	adds r0, r6, #0
	bl Func_02002244
	movs r1, #1
	movs r0, #23
	bl ObjectMotion_SetVariantCallback
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r0, #23
	movs r1, #0
	bl Func_02002254
	movs r0, #15
	movs r1, #23
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r1, #2
	movs r2, #30
	adds r1, #255
	movs r0, #15
	bl Func_0200226c
	movs r1, #3
	movs r0, #23
	bl Object_SetModeById
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r0, #23
	movs r1, #0
	bl Func_02002254
	movs r2, #0
	movs r1, #21
	movs r0, #23
	bl ObjectMotion_SetAngleToward
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r0, #23
	movs r1, #0
	bl Func_02002254
	movs r2, #0
	movs r1, #20
	movs r0, #15
	bl Object_LinkPair
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r0, #21
	movs r1, #0
	bl Func_02002254
	movs r1, #3
	movs r0, #7
	bl Object_SetModeById
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r0, #7
	movs r1, #0
	bl Func_02002254
	movs r1, #128
	movs r0, #15
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #20
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r1, #3
	movs r0, #6
	bl Object_SetModeById
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r0, #6
	movs r1, #0
	bl Func_02002254
	movs r1, #132
	movs r2, #30
	lsls r1, r1, #1
	movs r0, #5
	bl Func_0200226c
	movs r0, #5
	movs r1, #0
	bl Func_02002254
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #15
	bl Func_0200226c
	movs r0, #15
	bl FieldEvent_WaitFrames
	movs r2, #0
	movs r0, #15
	movs r1, #21
	bl ObjectMotion_SetAngleToward
	movs r0, #15
	movs r1, #0
	bl Func_02002254
	movs r1, #132
	movs r2, #30
	lsls r1, r1, #1
	movs r0, #21
	bl Func_0200226c
	movs r0, #21
	movs r1, #0
	bl Func_02002254
	movs r1, #131
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #6
	bl Func_0200226c
	ldr r5, .L_02009950
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
	ldr r1, [r5]
	movs r2, #0
	movs r0, #6
	bl ObjectMotion_SetAngleToward
	movs r1, #0
	movs r0, #6
	bl UiText_OpenMessageAtObject
	ldr r1, [r5]
	movs r0, #5
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	ldr r1, [r5]
	movs r0, #6
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	ldr r1, [r5]
	movs r0, #7
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	ldr r1, [r5]
	movs r0, #23
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	ldr r1, [r5]
	movs r0, #15
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	ldr r1, [r5]
	movs r0, #20
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	ldr r1, [r5]
	movs r0, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #30
	bl FieldEvent_WaitFrames
	ldr r0, [r5]
	movs r1, #0
	bl Event_AskYesNoWithActorAnimation
	cmp r0, #0
	bne .L_0200983c
	adds r0, r6, #0
	adds r0, #10
	bl Func_02002244
	movs r1, #4
	movs r0, #21
	bl Object_SetModeById
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r0, #21
	movs r1, #0
	bl Func_02002254
	b .L_0200985a
.L_0200983c:
	adds r0, r6, #0
	adds r0, #11
	bl Func_02002244
	movs r1, #3
	movs r0, #21
	bl Object_SetModeById
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r0, #21
	movs r1, #0
	bl Func_02002254
.L_0200985a:
	ldr r6, .L_02009954
	adds r0, r6, #0
	bl Func_02002244
	movs r0, #23
	movs r1, #21
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r1, #2
	movs r2, #30
	adds r1, #255
	movs r0, #23
	bl Func_0200226c
	movs r0, #23
	movs r1, #0
	bl Func_02002254
	movs r1, #3
	movs r0, #23
	bl Object_SetModeById
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r0, #23
	movs r1, #0
	bl Func_02002254
	ldr r5, .L_02009950
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	ldr r0, [r5]
	movs r1, #3
	bl Object_SetModeById
	movs r0, #5
	movs r1, #3
	bl Object_SetModeById
	movs r0, #6
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #7
	bl Object_SetModeById
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r2, #0
	movs r1, #15
	movs r0, #23
	bl ObjectMotion_SetAngleToward
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r0, #23
	movs r1, #0
	bl Func_02002254
	ldr r1, [r5]
	movs r0, #15
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	ldr r1, [r5]
	movs r2, #0
	movs r0, #23
	bl ObjectMotion_SetAngleToward
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r1, #0
	movs r0, #23
	bl UiText_OpenMessageAtObject
	ldr r0, [r5]
	movs r1, #0
	bl Event_AskYesNoWithActorAnimation
	cmp r0, #0
	bne .L_02009958
	adds r0, r6, #4
	bl Func_02002244
	movs r0, #5
	movs r1, #3
	bl Object_SetModeById
	movs r0, #6
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #7
	bl Object_SetModeById
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r0, #5
	movs r1, #0
	bl Func_02002254
	movs r0, #6
	movs r1, #0
	bl Func_02002254
	movs r0, #7
	movs r1, #0
	bl Func_02002254
	b .L_02009994
.L_02009948:
	.4byte MsgYouTookPity
.L_0200994c:
	.4byte MsgYouReNotImplying
.L_02009950:
	.4byte gPartyState
.L_02009954:
	.4byte MsgIDisagree2
.L_02009958:
	adds r0, r6, #7
	bl Func_02002244
	movs r0, #5
	movs r1, #4
	bl Object_SetModeById
	movs r0, #6
	movs r1, #4
	bl Object_SetModeById
	movs r1, #4
	movs r0, #7
	bl Object_SetModeById
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r0, #5
	movs r1, #0
	bl Func_02002254
	movs r0, #6
	movs r1, #0
	bl Func_02002254
	movs r0, #7
	movs r1, #0
	bl Func_02002254
.L_02009994:
	ldr r0, .L_02009d54
	bl Func_02002244
	movs r2, #0
	movs r1, #15
	movs r0, #23
	bl ObjectMotion_SetAngleToward
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r0, #23
	movs r1, #0
	bl Func_02002254
	movs r2, #0
	movs r1, #20
	movs r0, #15
	bl Object_LinkPair
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r0, #15
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #20
	bl Object_SetModeById
	movs r0, #30
	bl FieldEvent_WaitFrames
	ldr r3, .L_02009d58
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r3, r2
	ldr r1, [r5]
	movs r0, #15
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r2, #0
	ldr r1, [r5]
	movs r0, #20
	bl ObjectMotion_SetAngleToward
	movs r1, #1
	movs r0, #21
	bl ObjectMotion_SetVariantCallback
	movs r0, #15
	bl FieldEvent_WaitFrames
	movs r1, #6
	adds r1, #255
	movs r2, #30
	movs r0, #21
	bl Func_0200226c
	movs r2, #0
	ldr r1, [r5]
	movs r0, #23
	bl ObjectMotion_SetAngleToward
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r0, #23
	movs r1, #0
	bl Func_02002254
	movs r2, #0
	movs r1, #23
	ldr r0, [r5]
	bl ObjectMotion_SetAngleToward
	movs r0, #15
	bl FieldEvent_WaitFrames
	movs r1, #3
	ldr r0, [r5]
	bl Object_SetModeById
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r1, #3
	movs r0, #23
	bl Object_SetModeById
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r0, #23
	movs r1, #0
	bl Func_02002254
	movs r0, #5
	movs r1, #23
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #6
	movs r1, #23
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r2, #0
	movs r0, #7
	movs r1, #23
	bl ObjectMotion_SetAngleToward
	movs r0, #5
	movs r1, #3
	bl Object_SetModeById
	movs r0, #6
	movs r1, #3
	bl Object_SetModeById
	movs r0, #7
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	ldr r0, [r5]
	bl Object_SetModeById
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #15
	bl Func_0200226c
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #20
	bl Func_0200226c
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #1
	movs r0, #21
	bl Func_0200226c
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r0, #20
	movs r1, #0
	bl Func_02002254
	ldr r0, [r5]
	movs r1, #15
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #5
	movs r1, #15
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #6
	movs r1, #15
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #7
	movs r1, #15
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r2, #0
	movs r1, #15
	movs r0, #23
	bl ObjectMotion_SetAngleToward
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r0, #23
	movs r1, #0
	bl Func_02002254
	movs r1, #10
	movs r2, #30
	adds r1, #255
	movs r0, #15
	bl Func_0200226c
	movs r0, #15
	movs r1, #0
	bl Func_02002254
	movs r1, #3
	movs r0, #5
	bl Object_SetModeById
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r0, #5
	movs r1, #0
	bl Func_02002254
	movs r1, #3
	movs r0, #20
	bl Object_SetModeById
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r0, #20
	movs r1, #0
	bl Func_02002254
	movs r1, #6
	movs r2, #0
	adds r1, #255
	movs r0, #6
	bl Func_0200226c
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r0, #6
	movs r1, #0
	bl Func_02002254
	movs r2, #0
	movs r1, #5
	movs r0, #6
	bl ObjectMotion_SetAngleToward
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r0, #6
	movs r1, #0
	bl Func_02002254
	movs r1, #1
	movs r0, #7
	bl ObjectMotion_SetVariantCallback
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r0, #7
	movs r1, #0
	bl Func_02002254
	movs r2, #0
	ldr r0, [r5]
	movs r1, #7
	bl Object_LinkPair
	ldr r0, [r5]
	movs r1, #3
	bl Object_SetModeById
	movs r0, #7
	movs r1, #3
	bl Object_SetModeById
	movs r2, #0
	movs r0, #5
	movs r1, #6
	bl Object_LinkPair
	movs r0, #5
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #6
	bl Object_SetModeById
	movs r0, #15
	bl FieldEvent_WaitFrames
	ldr r0, [r5]
	movs r1, #5
	movs r2, #0
	bl Object_LinkPair
	movs r2, #0
	movs r0, #7
	movs r1, #6
	bl Object_LinkPair
	ldr r0, [r5]
	movs r1, #3
	bl Object_SetModeById
	movs r0, #7
	movs r1, #3
	bl Object_SetModeById
	movs r0, #5
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #6
	bl Object_SetModeById
	movs r0, #30
	bl FieldEvent_WaitFrames
	ldr r0, [r5]
	movs r1, #15
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #5
	movs r1, #15
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #6
	movs r1, #15
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #7
	movs r1, #15
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r2, #0
	movs r0, #23
	movs r1, #15
	bl ObjectMotion_SetAngleToward
	movs r1, #3
	movs r0, #15
	bl Object_SetModeById
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r0, #15
	movs r1, #0
	bl Func_02002254
	ldr r0, [r5]
	movs r1, #3
	bl Object_SetModeById
	movs r0, #7
	movs r1, #3
	bl Object_SetModeById
	movs r0, #5
	movs r1, #3
	bl Object_SetModeById
	movs r0, #6
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #23
	bl Object_SetModeById
	movs r0, #15
	bl FieldEvent_WaitFrames
	movs r0, #15
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #21
	bl Object_SetModeById
	movs r0, #60
	bl FieldEvent_WaitFrames
	movs r0, #5
	movs r1, #2
	bl Object_SetModeById
	ldr r0, [r5]
	bl Object_GetById
	cmp r0, #0
	beq .L_02009cc0
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #5
	bl ObjectMotion_ResetAndSetPosition
.L_02009cc0:
	movs r0, #5
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl Func_02002214
	movs r0, #6
	movs r1, #2
	bl Object_SetModeById
	ldr r0, [r5]
	bl Object_GetById
	cmp r0, #0
	beq .L_02009cf0
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #6
	bl ObjectMotion_ResetAndSetPosition
.L_02009cf0:
	movs r0, #6
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #6
	movs r1, #0
	movs r2, #0
	bl Func_02002214
	movs r0, #7
	movs r1, #2
	bl Object_SetModeById
	ldr r0, [r5]
	bl Object_GetById
	cmp r0, #0
	beq .L_02009d20
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #7
	bl ObjectMotion_ResetAndSetPosition
.L_02009d20:
	movs r0, #7
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #7
	movs r1, #0
	movs r2, #0
	bl Func_02002214
	movs r0, #23
	movs r1, #2
	bl Object_SetModeById
	ldr r0, [r5]
	bl Object_GetById
	cmp r0, #0
	beq .L_02009d5c
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #23
	bl ObjectMotion_ResetAndSetPosition
	b .L_02009d5c
	.2byte 0x0000
.L_02009d54:
	.4byte MsgAsYouCanSee
.L_02009d58:
	.4byte gPartyState
.L_02009d5c:
	movs r0, #23
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r1, #0
	movs r2, #0
	movs r0, #23
	bl Func_02002214
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #40
	bl GameFlag_SetBit
	movs r0, #212
	lsls r0, r0, #2
	bl GameFlag_SetBit
	bl Func_020021cc
	pop {r5, r6, r7, pc}
	.section .text.x0200a00c,"ax",%progbits
	.global Func_0200200c
	.thumb_func
Func_0200200c:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #85
	str r2, [r3]
	ldr r3, .L_0200a18c
	subs r2, #31
	adds r5, r3, r2
	movs r2, #0
	ldrsh r3, [r5, r2]
	cmp r3, #7
	beq .L_0200a034
	movs r0, #212
	lsls r0, r0, #2
	bl GameFlag_ClearBit
.L_0200a034:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #39
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a09e
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #40
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a09e
	movs r2, #0
	ldrsh r3, [r5, r2]
	cmp r3, #7
	bne .L_0200a09e
	movs r0, #7
	bl Func_0200231c
	movs r1, #144
	movs r2, #142
	movs r0, #22
	lsls r1, r1, #16
	lsls r2, r2, #18
	bl Func_02002214
	movs r0, #21
	movs r1, #0
	movs r2, #0
	bl Func_02002214
	movs r0, #16
	movs r1, #0
	movs r2, #0
	bl Func_02002214
	movs r0, #17
	movs r1, #0
	movs r2, #0
	bl Func_02002214
	movs r0, #18
	movs r1, #0
	movs r2, #0
	bl Func_02002214
	movs r0, #19
	movs r1, #0
	movs r2, #0
	bl Func_02002214
.L_0200a09e:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #39
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a104
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #40
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a104
	ldr r3, .L_0200a18c
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #7
	bne .L_0200a104
	movs r0, #212
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a104
	movs r0, #7
	bl Func_0200231c
	movs r0, #16
	movs r1, #0
	movs r2, #0
	bl Func_02002214
	movs r0, #17
	movs r1, #0
	movs r2, #0
	bl Func_02002214
	movs r0, #18
	movs r1, #0
	movs r2, #0
	bl Func_02002214
	movs r0, #19
	movs r1, #0
	movs r2, #0
	bl Func_02002214
.L_0200a104:
	ldr r3, .L_0200a18c
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #5
	bne .L_0200a134
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #39
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a134
	movs r0, #17
	movs r1, #5
	bl Object_SetModeById
	movs r1, #2
	movs r0, #17
	adds r1, #255
	bl Func_02002274
.L_0200a134:
	ldr r3, .L_0200a18c
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #7
	bne .L_0200a186
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #39
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a186
	movs r0, #9
	movs r1, #5
	bl Object_SetModeById
	movs r0, #10
	movs r1, #5
	bl Object_SetModeById
	movs r0, #11
	movs r1, #5
	bl Object_SetModeById
	movs r1, #5
	movs r0, #12
	bl Object_SetModeById
	movs r0, #11
	bl Object_GetById
	adds r5, r0, #0
	movs r0, #11
	bl Object_GetById
	ldr r3, [r0, #24]
	negs r3, r3
	str r3, [r5, #24]
.L_0200a186:
	movs r0, #0
	pop {r5, pc}
	.2byte 0x0000
.L_0200a18c:
	.4byte gPartyState
