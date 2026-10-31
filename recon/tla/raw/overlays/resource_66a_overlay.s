.syntax unified
	.thumb
	.section .text.x02008818,"ax",%progbits
	.balign 4
	.global Func_02000818
	.thumb_func
Func_02000818:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #171
	sub sp, #12
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008832
	bl .L_02009206
.L_02008832:
	movs r0, #245
	lsls r0, r0, #3
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #1
	bne .L_02008844
	bl .L_02009206
.L_02008844:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #189
	bl GameFlag_SetBit
	bl Func_02001430
	movs r0, #0
	bl Func_02001528
	movs r1, #128
	movs r2, #128
	movs r0, #22
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl Actor_SetSpeedParameters
	movs r1, #128
	movs r2, #128
	movs r0, #21
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl Actor_SetSpeedParameters
	movs r1, #128
	movs r2, #128
	movs r0, #17
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl Actor_SetSpeedParameters
	movs r1, #128
	movs r2, #128
	movs r0, #19
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl Actor_SetSpeedParameters
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #9
	lsls r2, r2, #8
	movs r0, #20
	bl Actor_SetSpeedParameters
	ldr r0, .L_02008c9c
	bl Func_020014b8
	movs r0, #24
	movs r1, #0
	movs r2, #10
	bl Func_020014c8
	movs r1, #200
	movs r2, #144
	movs r0, #4
	lsls r1, r1, #2
	lsls r2, r2, #1
	bl Actor_SetPositionAndReset
	movs r1, #128
	movs r0, #4
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r0, #186
	movs r1, #1
	movs r2, #132
	movs r3, #1
	lsls r2, r2, #17
	negs r1, r1
	lsls r0, r0, #18
	bl Camera_MoveWithinMapBounds
	bl Func_02001508
	movs r0, #20
	bl FieldEvent_Wait
	movs r1, #3
	movs r0, #18
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl FieldEvent_Wait
	movs r2, #10
	movs r0, #18
	movs r1, #0
	bl Func_020014c8
	movs r1, #3
	movs r0, #22
	bl Motion_SetModeAndWaitAnimation
	movs r0, #30
	bl FieldEvent_Wait
	movs r1, #160
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #18
	bl Actor_ArmCallback
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #18
	movs r1, #0
	movs r2, #10
	bl Func_020014c8
	movs r0, #21
	movs r1, #6
	movs r2, #15
	bl Actor_Jump
	movs r0, #21
	movs r1, #6
	movs r2, #23
	bl Actor_Jump
	movs r0, #21
	movs r1, #0
	movs r2, #10
	bl Func_020014c8
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #22
	bl Func_020014e8
	movs r0, #128
	lsls r0, r0, #6
	movs r2, #10
	adds r0, #22
	movs r1, #0
	bl Func_020014c8
	movs r1, #2
	movs r0, #16
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl FieldEvent_Wait
	movs r0, #16
	movs r1, #0
	movs r2, #10
	bl Func_020014c8
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #22
	bl Func_020014e8
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #22
	movs r1, #0
	movs r2, #10
	bl Func_020014c8
	movs r1, #192
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #22
	bl Actor_ArmCallback
	movs r0, #30
	bl FieldEvent_Wait
	movs r1, #8
	adds r1, #255
	movs r2, #50
	movs r0, #22
	bl Func_020014e8
	movs r0, #186
	movs r1, #1
	movs r2, #156
	movs r3, #1
	lsls r2, r2, #17
	negs r1, r1
	lsls r0, r0, #18
	bl Camera_MoveWithinMapBounds
	bl Func_02001508
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #21
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #24
	movs r1, #3
	bl Actor_StartRepeatedMotion
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #24
	bl Func_020014f0
	movs r0, #40
	bl FieldEvent_Wait
	movs r0, #24
	movs r1, #1
	bl Camera_FollowActor
	bl Func_02001508
	movs r1, #192
	movs r2, #192
	lsls r2, r2, #8
	movs r0, #24
	lsls r1, r1, #9
	bl Actor_SetSpeedParameters
	ldr r1, .L_02008ca0
	movs r0, #24
	bl Actor_SetMovementTypeById
	movs r0, #20
	bl FieldEvent_Wait
	movs r1, #3
	movs r0, #21
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl FieldEvent_Wait
	movs r1, #192
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #21
	bl Actor_ArmCallback
	movs r0, #24
	bl Actor_RefreshSelectorById
	movs r0, #22
	movs r1, #0
	movs r2, #0
	bl Actor_ArmCallback
	movs r0, #186
	movs r1, #1
	movs r2, #136
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #17
	lsls r0, r0, #18
	bl Camera_MoveWithinMapBounds
	bl Func_02001508
	movs r0, #10
	bl FieldEvent_Wait
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #24
	bl Func_020014e8
	movs r2, #10
	movs r0, #24
	movs r1, #0
	bl Func_020014c8
	movs r1, #4
	movs r0, #24
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl FieldEvent_Wait
	movs r0, #24
	movs r1, #0
	movs r2, #10
	bl Func_020014c8
	movs r1, #176
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #24
	bl Actor_ArmCallback
	movs r0, #20
	bl FieldEvent_Wait
	movs r1, #4
	adds r1, #255
	movs r2, #30
	movs r0, #24
	bl Func_020014e8
	movs r2, #10
	movs r1, #0
	movs r0, #24
	bl Func_020014c8
	movs r0, #21
	bl Actor_GetById
	adds r5, r0, #0
	movs r0, #15
	bl Actor_GetById
	movs r1, #144
	lsls r1, r1, #5
	adds r6, r0, #0
	adds r1, #16
	movs r0, #8
	bl Func_020015a0
	bl Func_020015b0
	movs r0, #21
	bl Actor_GetById
	movs r1, #2
	bl Func_020015c8
	movs r0, #201
	bl Func_02001600
	movs r0, #40
	bl FieldEvent_Wait
	adds r0, r5, #0
	bl BattleEffect_StartItemBreak
	ldr r3, [r6, #8]
	mov r5, sp
	str r3, [r5]
	mov r8, r0
	ldr r3, [r6, #12]
	str r6, [r0, #104]
	movs r0, #128
	lsls r0, r0, #13
	adds r3, r3, r0
	str r3, [r5, #4]
	movs r1, #192
	ldr r3, [r6, #16]
	adds r2, r5, #0
	str r3, [r5, #8]
	lsls r1, r1, #8
	bl Vector_AddPolarOffsetFar
	ldr r2, [r5, #4]
	ldr r3, [r5, #8]
	ldr r1, [r5]
	mov r0, r8
	bl Func_020013f8
	mov r0, r8
	bl BattleEffect_SnapScaleToFull
	mov r0, r8
	movs r1, #4
	bl Func_020013f0
	ldr r3, .L_02008ca4
	movs r1, #192
	str r3, [r6, #108]
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #4
	bl Actor_ArmCallback
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #17
	movs r1, #0
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #128
	movs r0, #19
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #1
	movs r0, #4
	bl Func_020014e8
	movs r0, #10
	bl FieldEvent_Wait
	movs r1, #3
	movs r0, #17
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl FieldEvent_Wait
	movs r0, #19
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r1, #192
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #20
	bl Actor_ArmCallback
	movs r0, #30
	bl FieldEvent_Wait
	movs r0, #202
	movs r1, #1
	movs r2, #144
	movs r3, #1
	lsls r2, r2, #17
	lsls r0, r0, #18
	negs r1, r1
	bl Camera_MoveWithinMapBounds
	bl Func_02001508
	movs r0, #4
	movs r1, #3
	bl Actor_StartRepeatedMotion
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #4
	bl Func_020014f0
	movs r0, #50
	bl FieldEvent_Wait
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #60
	movs r0, #20
	bl Func_020014e8
	movs r0, #186
	movs r1, #1
	movs r2, #136
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #17
	lsls r0, r0, #18
	bl Camera_MoveWithinMapBounds
	bl Func_02001508
	movs r0, #20
	bl FieldEvent_Wait
	movs r1, #16
	negs r1, r1
	movs r2, #0
	movs r0, #20
	bl Actor_CommitPositionAndActivate
	movs r0, #20
	bl FieldEvent_Wait
	movs r1, #6
	adds r1, #255
	movs r2, #60
	movs r0, #20
	bl Func_020014e8
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #6
	movs r0, #18
	bl Actor_ArmCallback
	movs r0, #40
	bl FieldEvent_Wait
	movs r0, #18
	movs r1, #3
	bl Actor_StartRepeatedMotion
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #18
	bl Func_020014f0
	movs r0, #40
	bl FieldEvent_Wait
	movs r1, #160
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #18
	bl Actor_ArmCallback
	movs r0, #30
	bl FieldEvent_Wait
	movs r0, #18
	movs r1, #0
	movs r2, #20
	bl Func_020014c8
	movs r1, #128
	movs r2, #30
	movs r5, #0
	lsls r1, r1, #1
	movs r0, #21
	bl Func_020014e8
	movs r1, #0
	str r5, [r6, #108]
	adds r0, r6, #0
	bl Animation_ApplyChildValues
	mov r0, r8
	bl UpdateRisingParticleBurst
	movs r0, #21
	bl Actor_GetById
	movs r1, #0
	bl Func_020015c8
	bl Func_020015c0
	bl Func_020015b8
	movs r0, #8
	.ifdef TLA_EDITION_ES
	bl Field_EndActorSpriteEffect
	.else
	.ifdef TLA_EDITION_IT
	bl Field_EndActorSpriteEffect
	.else
	bl Field_StartPaletteTransition
	.endif
	.endif
	movs r0, #30
	bl FieldEvent_Wait
	movs r1, #128
	movs r0, #4
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r0, #16
	movs r1, #0
	movs r2, #0
	b .L_02008ca8
	.2byte 0x0000
.L_02008c9c:
	.4byte MsgYouHereTo
.L_02008ca0:
	.4byte gData_02001610
.L_02008ca4:
	.4byte Func_02001228
.L_02008ca8:
	bl Actor_ArmCallback
	movs r1, #192
	movs r0, #17
	lsls r1, r1, #6
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #160
	movs r0, #19
	lsls r1, r1, #7
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #128
	movs r0, #20
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #176
	movs r0, #24
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #176
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #22
	bl Actor_ArmCallback
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #21
	movs r1, #3
	bl Actor_StartRepeatedMotion
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #21
	bl Func_020014f0
	movs r0, #40
	bl FieldEvent_Wait
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #21
	bl Actor_ArmCallback
	movs r0, #30
	bl FieldEvent_Wait
	movs r1, #4
	movs r0, #18
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl FieldEvent_Wait
	movs r1, #192
	movs r0, #16
	lsls r1, r1, #6
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #192
	movs r0, #17
	lsls r1, r1, #6
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #160
	movs r0, #19
	lsls r1, r1, #7
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #160
	movs r0, #20
	lsls r1, r1, #7
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #160
	movs r0, #18
	lsls r1, r1, #7
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #160
	movs r0, #22
	lsls r1, r1, #7
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #24
	bl Actor_ArmCallback
	movs r0, #40
	bl FieldEvent_Wait
	movs r1, #128
	movs r2, #40
	lsls r1, r1, #1
	movs r0, #21
	bl Func_020014e8
	movs r1, #4
	movs r0, #21
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl FieldEvent_Wait
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #6
	movs r0, #21
	bl Actor_ArmCallback
	movs r0, #20
	bl FieldEvent_Wait
	movs r1, #144
	lsls r1, r1, #5
	adds r1, #16
	movs r0, #8
	bl Func_020015a0
	bl Func_020015b0
	movs r0, #21
	bl Actor_GetById
	movs r1, #2
	bl Func_020015c8
	movs r0, #201
	bl Func_02001600
	movs r0, #40
	bl FieldEvent_Wait
	movs r2, #10
	movs r1, #0
	movs r0, #18
	bl Func_020014c8
	movs r0, #21
	bl Actor_GetById
	movs r1, #0
	bl Func_020015c8
	bl Func_020015c0
	bl Func_020015b8
	movs r0, #8
	.ifdef TLA_EDITION_ES
	bl Field_EndActorSpriteEffect
	.else
	.ifdef TLA_EDITION_IT
	bl Field_EndActorSpriteEffect
	.else
	bl Field_StartPaletteTransition
	.endif
	.endif
	movs r0, #16
	movs r1, #0
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #192
	movs r0, #17
	lsls r1, r1, #6
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #160
	movs r0, #19
	lsls r1, r1, #7
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #128
	movs r0, #20
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #176
	movs r0, #24
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #176
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #22
	bl Actor_ArmCallback
	movs r0, #40
	bl FieldEvent_Wait
	movs r1, #208
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #21
	bl Actor_ArmCallback
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #21
	movs r1, #0
	movs r2, #10
	bl Func_020014c8
	movs r1, #10
	adds r1, #255
	movs r2, #50
	movs r0, #21
	bl Func_020014e8
	movs r1, #160
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #22
	bl Actor_ArmCallback
	movs r0, #30
	bl FieldEvent_Wait
	movs r1, #129
	movs r2, #50
	lsls r1, r1, #1
	movs r0, #22
	bl Func_020014e8
	movs r1, #2
	movs r0, #21
	bl Motion_SetVarCbAndRefresh
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #21
	movs r1, #0
	movs r2, #10
	bl Func_020014c8
	movs r1, #192
	movs r0, #16
	lsls r1, r1, #6
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #192
	movs r0, #17
	lsls r1, r1, #6
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #160
	movs r0, #19
	lsls r1, r1, #7
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #160
	movs r0, #20
	lsls r1, r1, #7
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #160
	movs r0, #18
	lsls r1, r1, #7
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #160
	movs r0, #22
	lsls r1, r1, #7
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #24
	bl Actor_ArmCallback
	movs r0, #20
	bl FieldEvent_Wait
	movs r1, #2
	movs r0, #21
	adds r1, #255
	bl Func_020014f0
	movs r2, #20
	movs r0, #21
	movs r1, #0
	bl Func_020014c8
	ldr r3, .L_02009210
	movs r1, #144
	str r5, [r3]
	lsls r1, r1, #3
	ldr r0, .L_02009214
	bl Task_Add
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #24
	bl Func_020014e8
	movs r0, #24
	movs r1, #0
	movs r2, #10
	bl Func_020014c8
	movs r1, #176
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #22
	bl Actor_ArmCallback
	movs r0, #30
	bl FieldEvent_Wait
	movs r1, #3
	movs r0, #18
	bl Motion_SetModeAndWaitAnimation
	movs r0, #30
	bl FieldEvent_Wait
	movs r1, #3
	movs r0, #22
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl FieldEvent_Wait
	movs r1, #160
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #22
	bl Actor_ArmCallback
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #22
	movs r1, #0
	movs r2, #8
	bl Actor_CommitPositionAndActivate
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #22
	bl Actor_ArmCallback
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #128
	lsls r0, r0, #6
	movs r2, #10
	adds r0, #22
	movs r1, #0
	bl Func_020014c8
	movs r1, #2
	movs r0, #21
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl FieldEvent_Wait
	movs r1, #128
	movs r0, #21
	lsls r1, r1, #1
	bl Func_020014f0
	movs r2, #10
	movs r0, #21
	movs r1, #0
	bl Func_020014c8
	movs r1, #3
	movs r0, #22
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl FieldEvent_Wait
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #22
	movs r1, #0
	movs r2, #10
	bl Func_020014c8
	movs r2, #0
	movs r1, #0
	movs r0, #21
	bl Actor_ArmCallback
	movs r0, #20
	bl FieldEvent_Wait
	movs r1, #3
	movs r0, #21
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl FieldEvent_Wait
	movs r2, #10
	movs r0, #21
	movs r1, #0
	bl Func_020014c8
	movs r1, #3
	movs r0, #22
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl FieldEvent_Wait
	movs r0, #128
	lsls r0, r0, #6
	movs r2, #10
	adds r0, #22
	movs r1, #0
	bl Func_020014c8
	movs r1, #3
	movs r0, #21
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl FieldEvent_Wait
	movs r0, #21
	movs r1, #0
	movs r2, #10
	bl Func_020014c8
	movs r1, #240
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #22
	bl Actor_ArmCallback
	movs r0, #30
	bl FieldEvent_Wait
	movs r0, #128
	lsls r0, r0, #6
	movs r2, #10
	adds r0, #22
	movs r1, #0
	bl Func_020014c8
	movs r1, #3
	movs r0, #24
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl FieldEvent_Wait
	movs r0, #24
	movs r1, #0
	movs r2, #10
	bl Func_020014c8
	movs r1, #176
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #22
	bl Actor_ArmCallback
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #128
	lsls r0, r0, #6
	movs r2, #10
	adds r0, #22
	movs r1, #0
	bl Func_020014c8
	movs r1, #3
	movs r0, #18
	bl Motion_SetModeAndWaitAnimation
	movs r0, #30
	bl FieldEvent_Wait
	movs r1, #3
	movs r0, #22
	bl Motion_SetModeAndWaitAnimation
	movs r0, #30
	bl FieldEvent_Wait
	movs r1, #128
	movs r0, #22
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r2, #0
	movs r1, #0
	movs r0, #21
	bl Actor_ArmCallback
	movs r0, #30
	bl FieldEvent_Wait
	movs r1, #3
	movs r0, #22
	bl Actor_SetModeById
	movs r0, #30
	bl FieldEvent_Wait
	movs r1, #3
	movs r0, #21
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #24
	movs r1, #21
	bl Actor_WatchAndSetScript
	movs r2, #16
	movs r0, #22
	movs r1, #0
	negs r2, r2
	bl Actor_OffsetPositionAndReset
	movs r2, #16
	movs r0, #21
	movs r1, #0
	negs r2, r2
	bl Actor_CommitPositionAndActivate
	movs r0, #22
	movs r1, #52
	movs r2, #0
	bl Actor_OffsetPositionAndReset
	movs r2, #0
	movs r1, #52
	movs r0, #21
	bl Actor_CommitPositionAndActivate
	movs r0, #15
	bl FieldEvent_Wait
	movs r0, #22
	movs r1, #3
	bl Actor_StartRepeatedMotion
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #22
	bl Func_020014f0
	movs r0, #40
	bl FieldEvent_Wait
	movs r0, #4
	movs r1, #21
	bl Actor_WatchAndSetScript
	movs r0, #22
	movs r1, #2
	bl Actor_SetSpritePriority
	movs r0, #21
	movs r1, #2
	bl Actor_SetSpritePriority
	ldr r5, .L_02009218
	movs r0, #22
	adds r1, r5, #0
	bl Actor_SetMovementTypeById
	movs r0, #10
	bl FieldEvent_Wait
	adds r1, r5, #0
	movs r0, #21
	bl Actor_SetMovementTypeById
	movs r0, #10
	bl FieldEvent_Wait
	movs r0, #20
	movs r1, #16
	movs r2, #0
	bl Actor_CommitPositionAndActivate
	movs r1, #192
	movs r0, #20
	lsls r1, r1, #6
	movs r2, #0
	bl Actor_ArmCallback
	movs r0, #17
	movs r1, #0
	movs r2, #16
	bl Actor_OffsetPositionAndReset
	movs r1, #0
	movs r2, #16
	movs r0, #19
	bl Actor_CommitPositionAndActivate
	movs r0, #5
	bl FieldEvent_Wait
	movs r1, #192
	movs r0, #17
	lsls r1, r1, #6
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #192
	movs r0, #19
	lsls r1, r1, #6
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #192
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #18
	bl Actor_ArmCallback
	movs r0, #21
	bl Actor_RefreshSelectorById
	movs r0, #24
	bl Actor_Stop
	movs r0, #4
	bl Actor_Stop
	movs r0, #22
	movs r1, #0
	movs r2, #0
	bl Func_02001488
	movs r2, #0
	movs r1, #0
	movs r0, #21
	bl Func_02001488
	movs r0, #24
	bl Actor_GetById
	movs r1, #9
	bl Actor_SetActionCallback
	ldr r3, .L_0200921c
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	movs r1, #1
	bl Camera_FollowActor
	bl Func_02001438
.L_02009206:
	add sp, #12
	pop {r3}
	mov r8, r3
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02009210:
	.4byte gData_02001c08
.L_02009214:
	.4byte Func_02000814
.L_02009218:
	.4byte gData_02001674
.L_0200921c:
	.4byte gMainRoutineWork
	.section .text.x0200929c,"ax",%progbits
	.global Func_0200129c
	.thumb_func
Func_0200129c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #224
	adds r5, r0, #0
	mov r9, r1
	mov r10, r2
	movs r1, #255
	ldr r2, [r3]
	b .L_02009308
.L_020092b8:
	ldrh r3, [r5]
	movs r1, #26
	ldrsh r7, [r2, r1]
	cmp r7, r3
	bne .L_02009304
	adds r0, r7, #0
	bl Actor_GetById
	adds r5, #2
	ldrh r2, [r5]
	mov r3, r10
	adds r6, r0, #0
	mov r8, r2
	ldrh r5, [r5, #2]
	cmp r3, #7
	bgt .L_020092e0
	ldr r3, [r6, #28]
	ldr r1, .L_0200931c
	adds r3, r3, r1
	str r3, [r6, #28]
.L_020092e0:
	mov r2, r9
	cmp r2, #1
	bne .L_02009312
	adds r0, r5, #0
	bl GameFlag_SetBit
	adds r3, r6, #0
	adds r3, #34
	ldrb r3, [r3]
	adds r0, r7, #0
	mov r1, r8
	adds r2, r5, #0
	bl Func_02001320
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r6, #28]
	b .L_02009312
.L_02009304:
	adds r5, #6
	movs r1, #255
.L_02009308:
	ldrh r3, [r5]
	lsls r1, r1, #8
	adds r1, #255
	cmp r3, r1
	bne .L_020092b8
.L_02009312:
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_0200931c:
	.4byte 0xffffe100
	.section .bss,"aw",%nobits
	.balign 16
	.global gData_02001be0
gData_02001be0:
	.space 0x00000028
	.global gData_02001c08
gData_02001c08:
