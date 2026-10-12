.syntax unified
	.thumb
	.section .text.x020082f8,"ax",%progbits
	.global Func_020002f8
	.thumb_func
Func_020002f8:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #32]
	ldr r3, .L_02008510
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	sub sp, #8
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #32
	orrs r3, r2
	ldrb r2, [r5, #23]
	strb r3, [r0]
	movs r3, #13
	negs r3, r3
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	movs r1, #144
	strb r3, [r5, #23]
	ldr r0, .L_02008514
	lsls r1, r1, #3
	bl Scheduler_AddOrUpdateCallback
	movs r0, #129
	lsls r0, r0, #2
	movs r6, #0
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008344
	bl Func_020000fc
.L_02008344:
	movs r0, #8
	bl Object_GetById
	adds r3, r0, #0
	adds r2, r3, #0
	movs r5, #192
	lsls r5, r5, #12
	adds r2, #85
	strb r6, [r2]
	movs r0, #9
	str r5, [r3, #12]
	bl Object_GetById
	adds r3, r0, #0
	adds r2, r3, #0
	adds r2, #85
	strb r6, [r2]
	movs r0, #10
	str r5, [r3, #12]
	bl Object_GetById
	adds r3, r0, #0
	adds r2, r3, #0
	adds r2, #85
	strb r6, [r2]
	movs r0, #8
	str r5, [r3, #12]
	bl Object_GetById
	movs r1, #6
	bl Animation_ApplyChildValues
	movs r0, #9
	bl Object_GetById
	movs r1, #6
	bl Animation_ApplyChildValues
	movs r0, #10
	bl Object_GetById
	movs r1, #6
	bl Animation_ApplyChildValues
	movs r0, #12
	bl Object_GetById
	adds r3, r0, #0
	adds r2, r3, #0
	movs r5, #128
	adds r2, #85
	lsls r5, r5, #13
	strb r6, [r2]
	movs r0, #11
	str r5, [r3, #12]
	bl Object_GetById
	adds r3, r0, #0
	adds r2, r3, #0
	adds r2, #85
	movs r1, #128
	strb r6, [r2]
	movs r0, #13
	str r5, [r3, #12]
	lsls r1, r1, #5
	bl Func_02001598
	movs r0, #11
	movs r1, #2
	bl Object_SetModeById
	movs r0, #12
	movs r1, #3
	bl Object_SetModeById
	movs r0, #13
	movs r1, #2
	bl Object_SetModeById
	movs r0, #11
	movs r1, #2
	bl ObjectMotion_SetActionVariant
	movs r0, #12
	movs r1, #2
	bl ObjectMotion_SetActionVariant
	movs r1, #3
	movs r0, #13
	bl ObjectMotion_SetActionVariant
	movs r0, #11
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r5, #2
	orrs r3, r5
	strb r3, [r0]
	movs r0, #12
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
	movs r0, #13
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
	movs r0, #11
	bl Object_GetById
	movs r1, #6
	bl Animation_ApplyChildValues
	movs r0, #12
	bl Object_GetById
	movs r1, #6
	bl Animation_ApplyChildValues
	movs r0, #13
	bl Object_GetById
	movs r1, #6
	bl Animation_ApplyChildValues
	movs r0, #245
	lsls r0, r0, #3
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008474
	movs r1, #200
	movs r2, #136
	movs r0, #14
	lsls r1, r1, #16
	lsls r2, r2, #16
	bl Func_02001488
	movs r1, #196
	movs r2, #180
	movs r0, #15
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_02001488
.L_02008474:
	movs r0, #14
	bl Object_GetById
	adds r3, r0, #0
	adds r2, r3, #0
	adds r2, #85
	strb r6, [r2]
	movs r2, #0
	ldr r1, [r3, #16]
	ldr r0, [r3, #8]
	str r6, [r3, #20]
	str r6, [r3, #12]
	movs r3, #4
	bl Func_02001420
	movs r0, #15
	bl Object_GetById
	adds r3, r0, #0
	adds r2, r3, #0
	adds r2, #85
	strb r6, [r2]
	movs r2, #0
	ldr r1, [r3, #16]
	ldr r0, [r3, #8]
	str r6, [r3, #20]
	str r6, [r3, #12]
	movs r3, #4
	bl Func_02001420
	ldr r0, .L_02008518
	bl Func_02001578
	movs r0, #16
	bl Object_GetById
	movs r1, #9
	bl Object_SetActionCallback
	movs r0, #245
	lsls r0, r0, #3
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200850c
	movs r3, #13
	movs r2, #58
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #58
	movs r2, #12
	movs r3, #7
	bl Func_02001408
	movs r3, #25
	movs r2, #66
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #58
	movs r2, #2
	movs r3, #9
	bl Func_02001408
	movs r3, #15
	movs r2, #74
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #58
	movs r2, #10
	movs r3, #5
	bl Func_02001408
.L_0200850c:
	add sp, #8
	pop {r5, r6, pc}
.L_02008510:
	.4byte gPartyState
.L_02008514:
	.4byte Func_02000220
.L_02008518:
	.4byte Data_02001608
	.section .text.x0200851c,"ax",%progbits
	.global Func_0200051c
	.thumb_func
Func_0200051c:
	push {r5, r6, r7, lr}
	ldr r7, .L_02008790
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r7, r2
	ldr r0, [r3]
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r6, #32
	orrs r3, r6
	strb r3, [r0]
	movs r0, #8
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	orrs r3, r6
	strb r3, [r0]
	movs r0, #0
	bl Func_02001520
	ldr r5, .L_02008794
	movs r0, #0
	bl Func_020015e0
	movs r3, #128
	movs r2, #15
	lsls r3, r3, #23
	adds r0, r5, #0
	movs r1, #8
	bl Func_020015e8
	movs r1, #144
	ldr r0, .L_02008798
	lsls r1, r1, #3
	bl Scheduler_AddOrUpdateCallback
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r7, r2
	ldrh r3, [r3]
	movs r2, #128
	subs r3, #8
	lsls r3, r3, #16
	lsls r2, r2, #9
	cmp r3, r2
	bhi .L_02008590
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #208
	lsls r2, r2, #4
	adds r2, #76
	adds r3, r3, r2
	movs r2, #3
	strh r2, [r3]
.L_02008590:
	bl Func_02001568
	movs r1, #128
	movs r2, #10
	lsls r1, r1, #2
	movs r3, #11
	movs r0, #0
	bl Func_02001570
	movs r0, #10
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r5, #2
	orrs r3, r6
	strb r3, [r0]
	movs r0, #11
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	orrs r3, r6
	strb r3, [r0]
	ldr r0, .L_0200879c
	bl Func_02001230
	movs r0, #9
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	movs r1, #128
	movs r0, #14
	lsls r1, r1, #5
	bl Func_02001598
	movs r0, #12
	movs r1, #0
	bl Object_SetModeById
	movs r0, #13
	movs r1, #1
	bl Object_SetModeById
	movs r0, #14
	movs r1, #2
	bl Object_SetModeById
	movs r0, #12
	movs r1, #1
	bl ObjectMotion_SetActionVariant
	movs r1, #1
	movs r0, #13
	bl ObjectMotion_SetActionVariant
	movs r0, #12
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
	movs r0, #13
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
	movs r0, #14
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
	movs r0, #12
	bl Object_GetById
	movs r1, #6
	bl Animation_ApplyChildValues
	movs r0, #13
	bl Object_GetById
	movs r1, #6
	bl Animation_ApplyChildValues
	movs r0, #14
	bl Object_GetById
	movs r1, #6
	bl Animation_ApplyChildValues
	movs r0, #245
	lsls r0, r0, #3
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200866c
	movs r0, #15
	movs r1, #0
	movs r2, #0
	bl Func_02001488
	b .L_020086ee
.L_0200866c:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #171
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020086ee
	movs r2, #0
	movs r0, #23
	movs r1, #0
	bl Func_02001488
	movs r0, #18
	movs r1, #2
	bl ObjectMotion_SetActionVariant
	movs r0, #16
	movs r1, #2
	bl ObjectMotion_SetActionVariant
	movs r0, #17
	movs r1, #2
	bl ObjectMotion_SetActionVariant
	movs r0, #19
	movs r1, #2
	bl ObjectMotion_SetActionVariant
	movs r0, #20
	movs r1, #2
	bl ObjectMotion_SetActionVariant
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #189
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020086d8
	movs r1, #178
	movs r2, #216
	movs r0, #17
	lsls r1, r1, #18
	lsls r2, r2, #16
	bl Func_02001488
	movs r1, #186
	movs r2, #216
	movs r0, #19
	lsls r1, r1, #18
	lsls r2, r2, #16
	bl Func_02001488
	b .L_02008748
.L_020086d8:
	movs r0, #22
	movs r1, #0
	movs r2, #0
	bl Func_02001488
	movs r0, #21
	movs r1, #0
	movs r2, #0
	bl Func_02001488
	b .L_02008748
.L_020086ee:
	movs r0, #18
	movs r1, #0
	movs r2, #0
	bl Func_02001488
	movs r0, #16
	movs r1, #0
	movs r2, #0
	bl Func_02001488
	movs r0, #17
	movs r1, #0
	movs r2, #0
	bl Func_02001488
	movs r0, #19
	movs r1, #0
	movs r2, #0
	bl Func_02001488
	movs r0, #20
	movs r1, #0
	movs r2, #0
	bl Func_02001488
	movs r0, #21
	movs r1, #0
	movs r2, #0
	bl Func_02001488
	movs r0, #22
	movs r1, #0
	movs r2, #0
	bl Func_02001488
	movs r0, #24
	movs r1, #0
	movs r2, #0
	bl Func_02001488
	movs r0, #23
	bl Object_GetById
	ldr r3, .L_020087a0
	str r3, [r0, #108]
.L_02008748:
	ldr r3, .L_02008790
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #9
	bne .L_0200878e
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200878e
	bl Event_StartScreenTransition
	bl Event_WaitTransitionFrames
	movs r1, #152
	movs r2, #152
	lsls r1, r1, #7
	lsls r2, r2, #6
	movs r0, #8
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r0, #8
	movs r1, #0
	movs r2, #16
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r0, #8
	bl ObjectMotion_CommitCurrentPositionAndActivate
.L_0200878e:
	pop {r5, r6, r7, pc}
.L_02008790:
	.4byte gPartyState
.L_02008794:
	.4byte Data_02001be0
.L_02008798:
	.4byte Func_02000258
.L_0200879c:
	.4byte Data_020016e4
.L_020087a0:
	.4byte Func_02001220
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
	bl ObjectMotion_SetSpeedParameters
	movs r1, #128
	movs r2, #128
	movs r0, #21
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r1, #128
	movs r2, #128
	movs r0, #17
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r1, #128
	movs r2, #128
	movs r0, #19
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #9
	lsls r2, r2, #8
	movs r0, #20
	bl ObjectMotion_SetSpeedParameters
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
	bl ObjectMotion_SetPositionAndReset
	movs r1, #128
	movs r0, #4
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #186
	movs r1, #1
	movs r2, #132
	movs r3, #1
	lsls r2, r2, #17
	negs r1, r1
	lsls r0, r0, #18
	bl Motion_CamBounds
	bl Func_02001508
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #18
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #18
	movs r1, #0
	bl Func_020014c8
	movs r1, #3
	movs r0, #22
	bl Motion_SetModeAndWaitAnimation
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #160
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #18
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #18
	movs r1, #0
	movs r2, #10
	bl Func_020014c8
	movs r0, #21
	movs r1, #6
	movs r2, #15
	bl ObjectMotion_Launch
	movs r0, #21
	movs r1, #6
	movs r2, #23
	bl ObjectMotion_Launch
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
	bl Battle_WaitMode0
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
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
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
	bl Motion_CamBounds
	bl Func_02001508
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #21
	movs r1, #3
	bl Object_SetModeById
	movs r0, #24
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #24
	bl Func_020014f0
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #24
	movs r1, #1
	bl Object_AttachWorkTargetToObject
	bl Func_02001508
	movs r1, #192
	movs r2, #192
	lsls r2, r2, #8
	movs r0, #24
	lsls r1, r1, #9
	bl ObjectMotion_SetSpeedParameters
	ldr r1, .L_02008ca0
	movs r0, #24
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #21
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #192
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #21
	bl ObjectMotion_ArmCallback
	movs r0, #24
	bl Object_RefreshSelectorById
	movs r0, #22
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #186
	movs r1, #1
	movs r2, #136
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #17
	lsls r0, r0, #18
	bl Motion_CamBounds
	bl Func_02001508
	movs r0, #10
	bl Battle_WaitMode0
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
	bl Battle_WaitMode0
	movs r0, #24
	movs r1, #0
	movs r2, #10
	bl Func_020014c8
	movs r1, #176
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #24
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
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
	bl Object_GetById
	adds r5, r0, #0
	movs r0, #15
	bl Object_GetById
	movs r1, #144
	lsls r1, r1, #5
	adds r6, r0, #0
	adds r1, #16
	movs r0, #8
	bl Func_020015a0
	bl Func_020015b0
	movs r0, #21
	bl Object_GetById
	movs r1, #2
	bl Func_020015c8
	movs r0, #201
	bl Func_02001600
	movs r0, #40
	bl Battle_WaitMode0
	adds r0, r5, #0
	bl BattleFx_StartItemBreak
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
	bl BattleFx_SnapScaleToFull
	mov r0, r8
	movs r1, #4
	bl Func_020013f0
	ldr r3, .L_02008ca4
	movs r1, #192
	str r3, [r6, #108]
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #4
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #17
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #19
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #1
	movs r0, #4
	bl Func_020014e8
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #17
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #19
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r1, #192
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #20
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #202
	movs r1, #1
	movs r2, #144
	movs r3, #1
	lsls r2, r2, #17
	lsls r0, r0, #18
	negs r1, r1
	bl Motion_CamBounds
	bl Func_02001508
	movs r0, #4
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #4
	bl Func_020014f0
	movs r0, #50
	bl Battle_WaitMode0
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
	bl Motion_CamBounds
	bl Func_02001508
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #16
	negs r1, r1
	movs r2, #0
	movs r0, #20
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #6
	adds r1, #255
	movs r2, #60
	movs r0, #20
	bl Func_020014e8
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #6
	movs r0, #18
	bl ObjectMotion_ArmCallback
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #18
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #18
	bl Func_020014f0
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #160
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #18
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
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
	bl Object_GetById
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
	bl Field_BeginPaletteTransition
	.endif
	.endif
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #128
	movs r0, #4
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #16
	movs r1, #0
	movs r2, #0
	b .L_02008ca8
	.2byte 0x0000
.L_02008c9c:
	.4byte MsgYouHereTo
.L_02008ca0:
	.4byte Data_02001610
.L_02008ca4:
	.4byte Func_02001228
.L_02008ca8:
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #17
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	movs r0, #19
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #20
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #176
	movs r0, #24
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #176
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #22
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #21
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #21
	bl Func_020014f0
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #21
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #4
	movs r0, #18
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #192
	movs r0, #16
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #17
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	movs r0, #19
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	movs r0, #20
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	movs r0, #18
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	movs r0, #22
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #24
	bl ObjectMotion_ArmCallback
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #128
	movs r2, #40
	lsls r1, r1, #1
	movs r0, #21
	bl Func_020014e8
	movs r1, #4
	movs r0, #21
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #6
	movs r0, #21
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #144
	lsls r1, r1, #5
	adds r1, #16
	movs r0, #8
	bl Func_020015a0
	bl Func_020015b0
	movs r0, #21
	bl Object_GetById
	movs r1, #2
	bl Func_020015c8
	movs r0, #201
	bl Func_02001600
	movs r0, #40
	bl Battle_WaitMode0
	movs r2, #10
	movs r1, #0
	movs r0, #18
	bl Func_020014c8
	movs r0, #21
	bl Object_GetById
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
	bl Field_BeginPaletteTransition
	.endif
	.endif
	movs r0, #16
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #17
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	movs r0, #19
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #20
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #176
	movs r0, #24
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #176
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #22
	bl ObjectMotion_ArmCallback
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #208
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #21
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
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
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #129
	movs r2, #50
	lsls r1, r1, #1
	movs r0, #22
	bl Func_020014e8
	movs r1, #2
	movs r0, #21
	bl Motion_SetVarCbAndRefresh
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #21
	movs r1, #0
	movs r2, #10
	bl Func_020014c8
	movs r1, #192
	movs r0, #16
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #17
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	movs r0, #19
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	movs r0, #20
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	movs r0, #18
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	movs r0, #22
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #24
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
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
	bl Scheduler_AddOrUpdateCallback
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
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #18
	bl Motion_SetModeAndWaitAnimation
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #22
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #160
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #22
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #22
	movs r1, #0
	movs r2, #8
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #22
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
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
	bl Battle_WaitMode0
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
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #22
	movs r1, #0
	movs r2, #10
	bl Func_020014c8
	movs r2, #0
	movs r1, #0
	movs r0, #21
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #21
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #21
	movs r1, #0
	bl Func_020014c8
	movs r1, #3
	movs r0, #22
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
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
	bl Battle_WaitMode0
	movs r0, #21
	movs r1, #0
	movs r2, #10
	bl Func_020014c8
	movs r1, #240
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #22
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
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
	bl Battle_WaitMode0
	movs r0, #24
	movs r1, #0
	movs r2, #10
	bl Func_020014c8
	movs r1, #176
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #22
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
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
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #22
	bl Motion_SetModeAndWaitAnimation
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #128
	movs r0, #22
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r2, #0
	movs r1, #0
	movs r0, #21
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #22
	bl Object_SetModeById
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #21
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #24
	movs r1, #21
	bl Object_LinkObjectAndSetCallback
	movs r2, #16
	movs r0, #22
	movs r1, #0
	negs r2, r2
	bl ObjectMotion_OffsetPositionAndReset
	movs r2, #16
	movs r0, #21
	movs r1, #0
	negs r2, r2
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #22
	movs r1, #52
	movs r2, #0
	bl ObjectMotion_OffsetPositionAndReset
	movs r2, #0
	movs r1, #52
	movs r0, #21
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #15
	bl Battle_WaitMode0
	movs r0, #22
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #22
	bl Func_020014f0
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #4
	movs r1, #21
	bl Object_LinkObjectAndSetCallback
	movs r0, #22
	movs r1, #2
	bl ObjectMotion_SetActionVariant
	movs r0, #21
	movs r1, #2
	bl ObjectMotion_SetActionVariant
	ldr r5, .L_02009218
	movs r0, #22
	adds r1, r5, #0
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #10
	bl Battle_WaitMode0
	adds r1, r5, #0
	movs r0, #21
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #20
	movs r1, #16
	movs r2, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #192
	movs r0, #20
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #17
	movs r1, #0
	movs r2, #16
	bl ObjectMotion_OffsetPositionAndReset
	movs r1, #0
	movs r2, #16
	movs r0, #19
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #5
	bl Battle_WaitMode0
	movs r1, #192
	movs r0, #17
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #19
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #18
	bl ObjectMotion_ArmCallback
	movs r0, #21
	bl Object_RefreshSelectorById
	movs r0, #24
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #4
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #22
	movs r1, #0
	movs r2, #0
	bl Func_02001488
	movs r2, #0
	movs r1, #0
	movs r0, #21
	bl Func_02001488
	movs r0, #24
	bl Object_GetById
	movs r1, #9
	bl Object_SetActionCallback
	ldr r3, .L_0200921c
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	movs r1, #1
	bl Object_AttachWorkTargetToObject
	bl Func_02001438
.L_02009206:
	add sp, #12
	pop {r3}
	mov r8, r3
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02009210:
	.4byte Data_02001c08
.L_02009214:
	.4byte Func_02000814
.L_02009218:
	.4byte Data_02001674
.L_0200921c:
	.4byte gPartyState
	.section .text.x02009230,"ax",%progbits
	.global Func_02001230
	.thumb_func
Func_02001230:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r2, #255
	ldrh r3, [r0]
	lsls r2, r2, #8
	adds r2, #255
	sub sp, #4
	cmp r3, r2
	beq .L_02009294
	adds r7, r0, #0
.L_02009246:
	ldrh r3, [r7]
	mov r8, r3
	mov r0, r8
	bl Object_GetById
	adds r5, r0, #0
	adds r1, r5, #0
	adds r1, #89
	movs r2, #2
	ldrsh r6, [r7, r2]
	ldrb r2, [r1]
	movs r3, #4
	ldrsh r4, [r7, r3]
	movs r3, #254
	ands r3, r2
	strb r3, [r1]
	movs r1, #0
	str r4, [sp, #0]
	bl ObjectDispatch_SetSingleChildFlags
	ldr r4, [sp, #0]
	lsls r6, r6, #16
	lsls r4, r4, #16
	lsrs r4, r4, #16
	lsrs r6, r6, #16
	adds r5, #34
	ldrb r3, [r5]
	adds r2, r4, #0
	mov r0, r8
	adds r1, r6, #0
	adds r7, #6
	bl Func_02001320
	movs r2, #255
	ldrh r3, [r7]
	lsls r2, r2, #8
	adds r2, #255
	cmp r3, r2
	bne .L_02009246
.L_02009294:
	add sp, #4
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
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
	bl Object_GetById
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
	.section .text.x02009320,"ax",%progbits
	.global Func_02001320
	.thumb_func
Func_02001320:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r5, r3, #0
	mov r8, r2
	adds r6, r1, #0
	bl Object_GetById
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #32]
	lsls r3, r5, #3
	subs r3, r3, r5
	movs r1, #156
	lsls r1, r1, #1
	lsls r3, r3, #3
	adds r3, r3, r1
	ldr r5, [r2, r3]
	adds r7, r0, #0
	bl Func_02001588
	lsls r0, r0, #2
	adds r5, r5, r0
	mov r0, r8
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009394
	adds r1, r7, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #0
	strb r3, [r5, #2]
	movs r3, #2
	orrs r3, r2
	strb r3, [r1]
	cmp r6, #1
	beq .L_02009380
	cmp r6, #1
	bcc .L_02009376
	cmp r6, #2
	beq .L_0200938a
	b .L_020093c2
.L_02009376:
	adds r0, r7, #0
	movs r1, #2
	bl Func_020013f0
	b .L_020093c2
.L_02009380:
	adds r0, r7, #0
	movs r1, #4
	bl Func_020013f0
	b .L_020093c2
.L_0200938a:
	adds r0, r7, #0
	movs r1, #6
	bl Func_020013f0
	b .L_020093c2
.L_02009394:
	movs r3, #255
	strb r3, [r5, #2]
	cmp r6, #1
	beq .L_020093b0
	cmp r6, #1
	bcc .L_020093a6
	cmp r6, #2
	beq .L_020093ba
	b .L_020093c2
.L_020093a6:
	adds r0, r7, #0
	movs r1, #1
	bl Func_020013f0
	b .L_020093c2
.L_020093b0:
	adds r0, r7, #0
	movs r1, #3
	bl Func_020013f0
	b .L_020093c2
.L_020093ba:
	adds r0, r7, #0
	movs r1, #5
	bl Func_020013f0
.L_020093c2:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.section .bss,"aw",%nobits
	.global Data_02001be0
Data_02001be0:
	.space 0x00000028
	.global Data_02001c08
Data_02001c08:
