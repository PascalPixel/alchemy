.syntax unified
	.thumb
	.section .text.x02008430,"ax",%progbits
	.ifdef TLA_EDITION_JA
	.balign 4
	.global Func_02000430
	.thumb_func
Func_02000430:
	push {r5, lr}
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #33
	bl GameFlag_Test
	cmp r0, #0
	bne .L_ja_02008442
	b .L_ja_0200882c
.L_ja_02008442:
	movs r0, #0
	bl GameFlag_Test
	cmp r0, #0
	bne .L_ja_0200844e
	b .L_ja_0200882c
.L_ja_0200844e:
	bl Func_02001080
	movs r0, #0
	bl Func_020011e0
	movs r0, #167
	lsls r0, r0, #4
	bl GameFlag_SetBit
	ldr r0, .L_ja_02008830
	bl Func_02001138
	movs r0, #4
	movs r1, #1
	bl Object_SetModeById
	movs r0, #19
	movs r1, #0
	movs r2, #1
	bl Func_02001148
	movs r2, #221
	movs r0, #4
	movs r1, #200
	bl ObjectMotion_SetPositionAndReset
	movs r1, #2
	movs r0, #4
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl FieldEvent_WaitFrames
	movs r1, #16
	movs r0, #4
	negs r1, r1
	movs r2, #8
	bl ObjectMotion_OffsetPositionAndReset
	movs r3, #192
	movs r0, #0
	movs r1, #0
	movs r2, #0
	lsls r3, r3, #8
	bl Func_020011f0
	movs r3, #192
	lsls r3, r3, #8
	movs r1, #8
	movs r2, #16
	movs r0, #18
	bl Func_020011f0
	movs r0, #0
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #18
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #0
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #4
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #18
	bl ObjectMotion_ArmCallback
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #0
	bl Func_02001160
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #18
	bl Func_02001160
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #40
	movs r0, #4
	bl Func_02001160
	movs r0, #199
	movs r1, #1
	movs r2, #177
	lsls r0, r0, #16
	negs r1, r1
	lsls r2, r2, #16
	movs r3, #1
	movs r5, #128
	bl Camera_MoveWithinMapBounds
	lsls r5, r5, #7
	bl Func_02001180
	movs r1, #199
	movs r2, #130
	adds r3, r5, #0
	movs r0, #19
	lsls r1, r1, #16
	lsls r2, r2, #16
	bl Func_020010f0
	movs r1, #231
	movs r2, #199
	movs r0, #19
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r1, #199
	movs r2, #130
	adds r3, r5, #0
	movs r0, #20
	lsls r1, r1, #16
	lsls r2, r2, #16
	bl Func_020010f0
	movs r1, #220
	movs r2, #191
	movs r0, #20
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r1, #199
	movs r2, #130
	adds r3, r5, #0
	movs r0, #21
	lsls r1, r1, #16
	lsls r2, r2, #16
	bl Func_020010f0
	movs r0, #21
	movs r1, #204
	movs r2, #191
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r0, #199
	movs r1, #1
	movs r2, #225
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #16
	lsls r0, r0, #16
	bl Camera_MoveWithinMapBounds
	bl Func_02001180
	movs r0, #19
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r1, #160
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #19
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r1, #160
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #20
	bl ObjectMotion_ArmCallback
	movs r0, #21
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r1, #160
	movs r0, #21
	lsls r1, r1, #7
	bl Func_02001158
	movs r0, #21
	movs r1, #2
	bl Motion_SetVarCbAndRefresh
	movs r0, #21
	movs r1, #0
	movs r2, #5
	bl Func_02001148
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #18
	bl Func_02001160
	movs r0, #18
	movs r1, #0
	movs r2, #5
	bl Func_02001148
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #19
	bl Func_02001160
	movs r2, #5
	movs r0, #19
	movs r1, #0
	bl Func_02001148
	movs r1, #2
	movs r0, #20
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl FieldEvent_WaitFrames
	movs r0, #20
	movs r1, #0
	movs r2, #5
	bl Func_02001148
	movs r1, #8
	adds r1, #255
	movs r2, #30
	movs r0, #21
	bl Func_02001160
	movs r2, #5
	movs r0, #21
	movs r1, #0
	bl Func_02001148
	movs r1, #224
	movs r0, #0
	lsls r1, r1, #8
	bl Func_02001158
	movs r1, #3
	movs r0, #0
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r2, #5
	movs r0, #0
	movs r1, #0
	bl Func_02001148
	movs r1, #2
	movs r0, #19
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl FieldEvent_WaitFrames
	movs r0, #19
	movs r1, #0
	movs r2, #5
	bl Func_02001148
	movs r1, #4
	movs r2, #0
	movs r0, #18
	bl Object_LinkPair
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r1, #2
	adds r1, #255
	movs r2, #30
	movs r0, #0
	bl Func_02001160
	movs r0, #0
	movs r1, #0
	movs r2, #5
	bl Func_02001148
	movs r1, #8
	adds r1, #255
	movs r2, #30
	movs r0, #20
	bl Func_02001160
	movs r0, #20
	movs r1, #0
	movs r2, #5
	bl Func_02001148
	movs r1, #224
	movs r2, #0
	movs r0, #4
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r1, #224
	movs r0, #18
	lsls r1, r1, #8
	bl Func_02001158
	movs r0, #18
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r0, #21
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #21
	movs r1, #0
	movs r2, #5
	bl Func_02001148
	movs r1, #10
	adds r1, #255
	movs r2, #30
	movs r0, #18
	bl Func_02001160
	movs r2, #5
	movs r0, #18
	movs r1, #0
	bl Func_02001148
	movs r0, #21
	movs r1, #4
	bl Object_SetModeById
	movs r0, #19
	movs r1, #4
	bl Object_SetModeById
	movs r0, #20
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #19
	movs r1, #0
	movs r2, #5
	bl Func_02001148
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #4
	bl Func_02001160
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #18
	bl Func_02001160
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #0
	bl Func_02001160
	movs r0, #0
	movs r1, #0
	movs r2, #5
	bl Func_02001148
	movs r1, #21
	movs r2, #0
	movs r0, #20
	bl Object_LinkPair
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r1, #8
	adds r1, #255
	movs r2, #30
	movs r0, #20
	bl Func_02001160
	movs r0, #20
	movs r1, #0
	movs r2, #5
	bl Func_02001148
	movs r1, #160
	movs r0, #20
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	movs r2, #0
	movs r0, #21
	lsls r1, r1, #7
	bl ObjectMotion_ArmCallback
	movs r1, #224
	lsls r1, r1, #8
	movs r0, #18
	bl Func_02001158
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r0, #18
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r2, #5
	movs r0, #18
	movs r1, #0
	bl Func_02001148
	movs r0, #21
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #21
	movs r1, #0
	movs r2, #5
	bl Func_02001148
	movs r1, #16
	movs r0, #20
	negs r1, r1
	movs r2, #24
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r1, #16
	movs r0, #21
	negs r1, r1
	movs r2, #24
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r1, #16
	negs r1, r1
	movs r2, #24
	movs r0, #19
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r0, #19
	bl ObjectMotion_CommitCurrentPositionAndActivate
	ldr r3, .L_ja_02008834
	movs r2, #166
	lsls r2, r2, #1
	adds r2, #255
	adds r3, r3, r2
	movs r2, #2
	strb r2, [r3]
	ldr r5, .L_ja_02008838
	movs r1, #20
	adds r0, r5, #0
	bl Party_SetFields1eeAnd1f0
	adds r0, r5, #0
	movs r1, #21
	bl Party_SetFields1f2And1f4
	movs r0, #102
	movs r1, #2
	bl Func_020011a0
.L_ja_0200882c:
	pop {r5, pc}
	.2byte 0x0000
.L_ja_02008830:
	.4byte 0x00003092
.L_ja_02008834:
	.4byte gPartyState
.L_ja_02008838:
	.4byte 0x000000f0
	.endif
	.section .text.x0200887c,"ax",%progbits
	.ifdef TLA_EDITION_JA
	.global Func_0200087c
	.thumb_func
Func_0200087c:
	push {r5, lr}
	bl Func_02001080
	movs r0, #0
	bl Func_020011e0
	movs r0, #199
	movs r1, #1
	negs r1, r1
	ldr r2, .L_ja_020089fc
	movs r3, #0
	lsls r0, r0, #16
	bl Camera_MoveWithinMapBounds
	movs r5, #160
	ldr r0, .L_ja_02008a00
	bl Func_02001138
	lsls r5, r5, #8
	movs r1, #184
	movs r2, #229
	movs r0, #4
	lsls r1, r1, #16
	lsls r2, r2, #16
	adds r3, r5, #0
	bl Func_020010f0
	movs r1, #200
	movs r2, #221
	movs r0, #0
	lsls r1, r1, #16
	lsls r2, r2, #16
	adds r3, r5, #0
	bl Func_020010f0
	movs r3, #192
	movs r1, #218
	movs r2, #160
	lsls r3, r3, #8
	lsls r2, r2, #17
	movs r0, #18
	lsls r1, r1, #16
	bl Func_020010f0
	movs r0, #4
	movs r1, #19
	bl Object_SetModeById
	movs r0, #0
	movs r1, #39
	bl Object_SetModeById
	bl Event_StartScreenTransition
	bl Event_WaitTransitionFrames
	movs r1, #2
	movs r0, #21
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl FieldEvent_WaitFrames
	movs r1, #8
	adds r1, #255
	movs r2, #30
	movs r0, #21
	bl Func_02001160
	movs r0, #21
	movs r1, #0
	movs r2, #5
	bl Func_02001148
	movs r1, #8
	adds r1, #255
	movs r2, #30
	movs r0, #19
	bl Func_02001160
	movs r0, #19
	movs r1, #0
	bl UiText_OpenMessageAtObject
	movs r0, #1
	bl Func_020011e8
	cmp r0, #0
	bne .L_ja_02008950
	bl UiWork_FinalizePendingCore
	movs r1, #0
	movs r2, #5
	movs r0, #20
	bl Func_02001148
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r1, #226
	lsls r1, r1, #1
	adds r2, r2, r1
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_ja_02008970
.L_ja_02008950:
	bl UiWork_FinalizePendingCore
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #20
	adds r3, #1
	strh r3, [r2]
	movs r1, #0
	movs r2, #5
	bl Func_02001148
.L_ja_02008970:
	movs r0, #21
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r2, #5
	movs r0, #21
	movs r1, #0
	bl Func_02001148
	movs r1, #3
	movs r0, #19
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r2, #5
	movs r0, #19
	movs r1, #0
	bl Func_02001148
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #8
	lsls r1, r1, #5
	bl Func_02001170
	movs r0, #199
	movs r1, #1
	movs r3, #1
	negs r1, r1
	ldr r2, .L_ja_02008a04
	lsls r0, r0, #16
	bl Camera_MoveWithinMapBounds
	movs r0, #100
	bl FieldEvent_WaitFrames
	movs r1, #6
	adds r1, #255
	movs r2, #1
	movs r0, #18
	bl Func_02001160
	movs r0, #20
	bl FieldEvent_WaitFrames
	ldr r2, .L_ja_02008a08
	movs r1, #242
	lsls r1, r1, #1
	adds r3, r2, r1
	movs r1, #0
	ldrsh r0, [r3, r1]
	movs r1, #243
	lsls r1, r1, #1
	adds r3, r2, r1
	movs r2, #0
	ldrsh r1, [r3, r2]
	bl Func_02001190
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r1, #214
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r2, #0
	str r2, [r3]
	pop {r5, pc}
	.2byte 0x0000
.L_ja_020089fc:
	.4byte 0x01110000
.L_ja_02008a00:
	.4byte 0x000030a3
.L_ja_02008a04:
	.4byte 0x01750000
.L_ja_02008a08:
	.4byte gPartyState
	.endif
	.section .text.x02008a24,"ax",%progbits
	.ifdef TLA_EDITION_JA
	.global Func_02000a24
	.thumb_func
Func_02000a24:
	push {r5, lr}
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #113
	bl GameFlag_ClearBit
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #114
	bl GameFlag_SetBit
	bl Func_02001080
	movs r0, #0
	bl Func_020011e0
	movs r0, #199
	movs r1, #1
	negs r1, r1
	ldr r2, .L_ja_02008d5c
	movs r3, #0
	lsls r0, r0, #16
	bl Camera_MoveWithinMapBounds
	movs r5, #224
	ldr r0, .L_ja_02008d60
	bl Func_02001138
	lsls r5, r5, #8
	movs r1, #184
	movs r2, #229
	adds r3, r5, #0
	movs r0, #4
	lsls r1, r1, #16
	lsls r2, r2, #16
	bl Func_020010f0
	movs r1, #200
	movs r2, #221
	adds r3, r5, #0
	movs r0, #0
	lsls r1, r1, #16
	lsls r2, r2, #16
	bl Func_020010f0
	movs r1, #208
	movs r2, #237
	adds r3, r5, #0
	lsls r2, r2, #16
	movs r0, #18
	lsls r1, r1, #16
	bl Func_020010f0
	movs r0, #20
	movs r1, #8
	bl Object_SetModeById
	movs r0, #19
	movs r1, #8
	bl Object_SetModeById
	movs r0, #21
	movs r1, #8
	bl Object_SetModeById
	bl Event_StartScreenTransition
	bl Event_WaitTransitionFrames
	movs r1, #2
	movs r0, #20
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl FieldEvent_WaitFrames
	movs r0, #21
	movs r1, #0
	movs r2, #5
	bl Func_02001148
	movs r0, #19
	movs r1, #0
	movs r2, #5
	bl Func_02001148
	movs r0, #20
	movs r1, #0
	movs r2, #5
	bl Func_02001148
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #19
	bl Func_02001160
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #21
	bl Func_02001160
	movs r1, #8
	adds r1, #255
	movs r2, #30
	movs r0, #0
	bl Func_02001160
	movs r2, #5
	movs r0, #0
	movs r1, #0
	bl Func_02001148
	movs r1, #3
	movs r0, #21
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r0, #21
	movs r1, #0
	movs r2, #5
	bl Func_02001148
	movs r1, #10
	adds r1, #255
	movs r2, #30
	movs r0, #19
	bl Func_02001160
	movs r1, #0
	movs r0, #19
	bl UiText_OpenMessageAtObject
	b .L_ja_02008b44
.L_ja_02008b1e:
	movs r1, #129
	movs r2, #30
	lsls r1, r1, #1
	movs r0, #19
	bl Func_02001160
	movs r0, #20
	movs r1, #0
	bl UiText_OpenMessageAtObject
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	subs r3, #1
	strh r3, [r2]
.L_ja_02008b44:
	movs r0, #0
	movs r1, #0
	bl Event_AskYesNoWithActorAnimation
	cmp r0, #0
	bne .L_ja_02008b1e
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #20
	adds r3, #1
	strh r3, [r2]
	movs r1, #0
	movs r2, #5
	bl Func_02001148
	movs r0, #20
	movs r1, #1
	bl Object_SetModeById
	movs r0, #19
	movs r1, #1
	bl Object_SetModeById
	movs r1, #1
	movs r0, #21
	bl Object_SetModeById
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r1, #3
	movs r0, #21
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r1, #0
	movs r2, #5
	movs r0, #21
	bl Func_02001148
	bl Func_02000404
	movs r0, #60
	bl FieldEvent_WaitFrames
	movs r1, #2
	movs r2, #30
	adds r1, #255
	movs r0, #0
	bl Func_02001160
	movs r1, #3
	movs r0, #19
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r2, #5
	movs r0, #19
	movs r1, #0
	bl Func_02001148
	movs r1, #128
	lsls r1, r1, #7
	movs r0, #0
	bl Func_02001158
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #20
	bl Func_02001160
	movs r1, #224
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #20
	movs r1, #0
	movs r2, #5
	bl Func_02001148
	movs r1, #10
	adds r1, #255
	movs r2, #30
	movs r0, #21
	bl Func_02001160
	movs r2, #5
	movs r0, #21
	movs r1, #0
	bl Func_02001148
	movs r1, #3
	movs r0, #19
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r2, #5
	movs r0, #19
	movs r1, #0
	bl Func_02001148
	movs r1, #3
	movs r0, #20
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r2, #5
	movs r0, #20
	movs r1, #0
	bl Func_02001148
	movs r0, #0
	movs r1, #3
	bl Object_SetModeById
	movs r0, #4
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #18
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r1, #128
	movs r2, #128
	movs r0, #19
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r1, #128
	movs r2, #128
	movs r0, #20
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #8
	movs r0, #21
	lsls r1, r1, #9
	bl ObjectMotion_SetSpeedParameters
	ldr r1, .L_ja_02008d64
	movs r0, #19
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #20
	bl FieldEvent_WaitFrames
	ldr r5, .L_ja_02008d68
	movs r0, #20
	adds r1, r5, #0
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #20
	bl FieldEvent_WaitFrames
	adds r1, r5, #0
	movs r0, #21
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #100
	bl FieldEvent_WaitFrames
	movs r0, #0
	movs r1, #4
	movs r2, #0
	bl Object_LinkPair
	movs r2, #0
	movs r1, #4
	movs r0, #18
	bl ObjectMotion_SetAngleToward
	movs r0, #40
	bl FieldEvent_WaitFrames
	movs r0, #0
	movs r1, #3
	bl Object_SetModeById
	movs r0, #4
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #18
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r0, #0
	movs r1, #2
	bl Object_SetModeById
	movs r0, #4
	bl Object_GetById
	cmp r0, #0
	beq .L_ja_02008d14
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #0
	bl ObjectMotion_ResetAndSetPosition
.L_ja_02008d14:
	movs r0, #0
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl Func_020010e8
	movs r0, #18
	movs r1, #2
	bl Object_SetModeById
	movs r0, #4
	bl Object_GetById
	cmp r0, #0
	beq .L_ja_02008d44
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #18
	bl ObjectMotion_ResetAndSetPosition
.L_ja_02008d44:
	movs r0, #18
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #18
	movs r1, #0
	movs r2, #0
	bl Func_020010e8
	bl Func_02001088
	pop {r5, pc}
	.2byte 0x0000
.L_ja_02008d5c:
	.4byte 0x01110000
.L_ja_02008d60:
	.4byte 0x000030a9
.L_ja_02008d64:
	.4byte gData_020015e0
.L_ja_02008d68:
	.4byte gData_0200161c
	.endif
