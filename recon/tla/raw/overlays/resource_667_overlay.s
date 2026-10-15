.syntax unified
	.thumb
	.section .text.x02008514,"ax",%progbits
	.balign 4
	.global Func_02000514
	.thumb_func
Func_02000514:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #93
	str r2, [r3]
	movs r0, #15
	bl Object_GetById
	ldr r3, .L_02008640
	ldr r5, .L_02008644
	str r3, [r0, #108]
	movs r3, #241
	lsls r3, r3, #1
	adds r6, r5, r3
	movs r2, #0
	ldrsh r3, [r6, r2]
	cmp r3, #80
	bne .L_0200857c
	movs r3, #245
	lsls r3, r3, #1
	adds r2, r5, r3
	movs r1, #160
	movs r3, #14
	strh r3, [r2]
	lsls r1, r1, #7
	strh r3, [r6]
	movs r2, #0
	movs r0, #14
	bl ObjectMotion_ArmCallback
	bl Event_StartScreenTransition
	bl Event_WaitTransitionFrames
	movs r0, #10
	bl Battle_WaitMode0
	ldr r0, .L_02008648
	bl Func_02001538
	movs r0, #14
	movs r1, #0
	bl Func_02001550
	movs r0, #48
	adds r0, #255
	bl GameFlag_ClearBit
.L_0200857c:
	movs r2, #0
	ldrsh r3, [r6, r2]
	cmp r3, #81
	bne .L_02008594
	movs r3, #245
	lsls r3, r3, #1
	adds r2, r5, r3
	movs r3, #8
	strh r3, [r2]
	strh r3, [r6]
	bl Func_02000658
.L_02008594:
	movs r0, #245
	lsls r0, r0, #3
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020085c4
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020085c4
	movs r0, #18
	movs r1, #0
	movs r2, #0
	bl Func_02001500
	movs r0, #19
	movs r1, #0
	movs r2, #0
	bl Func_02001500
.L_020085c4:
	ldr r3, .L_02008644
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #99
	bne .L_0200863a
	movs r0, #152
	movs r1, #1
	movs r2, #160
	negs r1, r1
	lsls r0, r0, #17
	lsls r2, r2, #17
	movs r3, #0
	bl Motion_CamBounds
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	subs r2, #172
	str r2, [r3]
	bl Event_StartScreenTransition
	bl Event_WaitTransitionFrames
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #10
	lsls r1, r1, #7
	bl Func_02001578
	movs r0, #144
	movs r1, #1
	movs r2, #160
	movs r3, #1
	lsls r0, r0, #16
	negs r1, r1
	lsls r2, r2, #17
	bl Motion_CamBounds
	b .L_0200862a
.L_02008624:
	movs r0, #1
	bl WaitFrames
.L_0200862a:
	ldr r3, .L_0200864c
	ldr r3, [r3]
	cmp r3, #0
	beq .L_02008624
	ldr r0, .L_02008650
	movs r1, #80
	bl Func_02001588
.L_0200863a:
	movs r0, #0
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02008640:
	.4byte Func_02001488
.L_02008644:
	.4byte gPartyState
.L_02008648:
	.4byte MsgAlhafraManor10
.L_0200864c:
	.4byte gInput
.L_02008650:
	.4byte 0x00000061
	.section .text.x02008658,"ax",%progbits
	.global Func_02000658
	.thumb_func
Func_02000658:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	bl Func_020014b8
	movs r0, #0
	bl Func_020015b8
	ldr r0, .L_02008a64
	bl Func_02001538
	movs r1, #172
	movs r2, #192
	movs r0, #9
	lsls r1, r1, #17
	lsls r2, r2, #15
	bl Func_02001500
	movs r1, #204
	movs r2, #136
	movs r0, #8
	lsls r1, r1, #17
	lsls r2, r2, #16
	bl Func_02001500
	movs r1, #156
	movs r2, #208
	movs r0, #13
	lsls r1, r1, #17
	lsls r2, r2, #15
	bl Func_02001500
	movs r1, #148
	movs r2, #240
	movs r0, #7
	lsls r1, r1, #17
	lsls r2, r2, #15
	bl Func_02001500
	movs r1, #148
	movs r2, #136
	movs r0, #6
	lsls r1, r1, #17
	lsls r2, r2, #16
	bl Func_02001500
	movs r1, #156
	movs r2, #136
	movs r0, #5
	lsls r1, r1, #17
	lsls r2, r2, #16
	bl Func_02001500
	movs r1, #0
	movs r2, #0
	movs r0, #7
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #172
	movs r1, #1
	movs r2, #240
	lsls r0, r0, #17
	negs r1, r1
	lsls r2, r2, #15
	movs r3, #0
	bl Motion_CamBounds
	movs r2, #192
	lsls r2, r2, #18
	ldr r1, [r2, #108]
	movs r3, #218
	lsls r3, r3, #1
	movs r6, #214
	movs r5, #128
	lsls r5, r5, #1
	mov r8, r2
	lsls r6, r6, #1
	adds r2, r1, r3
	movs r3, #60
	str r3, [r2]
	str r5, [r1, r6]
	bl Event_StartScreenTransition
	bl Event_WaitTransitionFrames
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #13
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #13
	movs r1, #0
	bl Func_02001548
	movs r0, #9
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #9
	movs r1, #0
	movs r2, #10
	bl Func_02001548
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #9
	bl Func_02001560
	movs r0, #128
	lsls r0, r0, #6
	movs r2, #10
	adds r0, #9
	movs r1, #0
	bl Func_02001548
	movs r1, #2
	movs r0, #6
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #6
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #6
	movs r1, #0
	movs r2, #10
	bl Func_02001548
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #5
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #5
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #5
	movs r1, #0
	movs r2, #10
	bl Func_02001548
	movs r1, #8
	adds r1, #255
	movs r2, #30
	movs r0, #9
	bl Func_02001560
	movs r1, #160
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #9
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	movs r2, #10
	adds r0, #9
	movs r1, #0
	bl Func_02001548
	movs r0, #6
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	movs r0, #6
	lsls r1, r1, #1
	bl Func_02001568
	movs r0, #5
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #5
	bl Func_02001568
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #224
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #224
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #5
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #6
	movs r1, #4
	bl Object_SetModeById
	movs r1, #4
	movs r0, #5
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #9
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #9
	movs r1, #0
	movs r2, #10
	bl Func_02001548
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #9
	bl ObjectMotion_ArmCallback
	movs r0, #40
	bl Battle_WaitMode0
	mov r2, r8
	ldr r3, [r2, #108]
	movs r0, #128
	lsls r0, r0, #9
	str r5, [r3, r6]
	movs r1, #0
	adds r0, #2
	bl Func_02001590
	movs r0, #40
	bl Func_020015a0
	bl Event_WaitTransitionFrames
	movs r1, #3
	movs r0, #9
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #2
	adds r1, #255
	movs r2, #50
	movs r0, #13
	bl Func_02001560
	movs r0, #6
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	movs r0, #4
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #224
	movs r0, #7
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #13
	bl ObjectMotion_ArmCallback
	movs r0, #50
	bl Battle_WaitMode0
	movs r1, #6
	adds r1, #255
	movs r2, #0
	movs r0, #13
	bl Func_02001560
	movs r1, #6
	adds r1, #255
	movs r2, #0
	movs r0, #4
	bl Func_02001560
	movs r1, #6
	adds r1, #255
	movs r2, #0
	movs r0, #7
	bl Func_02001560
	movs r1, #6
	adds r1, #255
	movs r2, #0
	movs r0, #5
	bl Func_02001560
	movs r1, #6
	movs r2, #50
	adds r1, #255
	movs r0, #6
	bl Func_02001560
	movs r0, #9
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #9
	bl Func_02001568
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #224
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #224
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #224
	movs r0, #4
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #224
	movs r0, #7
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #0
	movs r2, #0
	movs r0, #13
	bl ObjectMotion_ArmCallback
	movs r0, #35
	bl Battle_WaitMode0
	movs r1, #160
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #9
	bl ObjectMotion_ArmCallback
	movs r0, #35
	bl Battle_WaitMode0
	movs r1, #132
	movs r2, #50
	lsls r1, r1, #1
	movs r0, #9
	bl Func_02001560
	movs r0, #6
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	movs r0, #6
	lsls r1, r1, #1
	bl Func_02001568
	movs r0, #5
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #5
	bl Func_02001568
	movs r0, #60
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #9
	bl Motion_SetModeAndWaitAnimation
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #130
	movs r2, #50
	lsls r1, r1, #1
	movs r0, #9
	bl Func_02001560
	movs r1, #4
	movs r0, #5
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #8
	adds r1, #255
	movs r2, #60
	movs r0, #6
	bl Func_02001560
	movs r0, #4
	movs r1, #6
	movs r2, #15
	bl ObjectMotion_Launch
	movs r1, #6
	movs r2, #23
	movs r0, #4
	bl ObjectMotion_Launch
	movs r0, #15
	bl Battle_WaitMode0
	movs r1, #10
	movs r2, #70
	adds r1, #255
	movs r0, #9
	bl Func_02001560
	movs r0, #78
	bl Func_020015f8
	movs r0, #128
	lsls r0, r0, #9
	adds r0, #2
	movs r1, #0
	bl Func_02001598
	movs r1, #0
	movs r0, #0
	bl Func_02001590
	movs r0, #60
	bl Func_020015a0
	bl Event_WaitTransitionFrames
	movs r0, #60
	bl Battle_WaitMode0
	movs r0, #128
	movs r1, #0
	lsls r0, r0, #9
	bl Func_02001590
	bl Event_StartScreenTransition
	bl Event_WaitTransitionFrames
	movs r0, #20
	bl Battle_WaitMode0
	bl Func_020015c0
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #9
	b .L_02008a68
.L_02008a64:
	.4byte MsgAlhafraManor29
.L_02008a68:
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #8
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #11
	ldr r1, .L_02008e84
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #12
	ldr r1, .L_02008e84
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r0, #9
	movs r1, #16
	movs r2, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #128
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #9
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #4
	adds r1, #255
	movs r2, #30
	movs r0, #9
	bl Func_02001560
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #9
	movs r1, #0
	movs r2, #10
	bl Func_02001548
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #45
	movs r0, #8
	bl Func_02001560
	movs r1, #16
	movs r0, #8
	negs r1, r1
	movs r2, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #160
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #8
	bl ObjectMotion_ArmCallback
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #0
	movs r2, #20
	movs r0, #8
	bl Func_02001548
	movs r0, #78
	bl Func_020015f8
	movs r1, #172
	movs r2, #176
	movs r0, #11
	lsls r1, r1, #17
	lsls r2, r2, #16
	bl Func_02001500
	movs r2, #16
	movs r0, #11
	movs r1, #0
	negs r2, r2
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #11
	movs r1, #0
	movs r2, #10
	bl Func_02001548
	movs r1, #2
	adds r1, #255
	movs r2, #30
	movs r0, #9
	bl Func_02001560
	movs r1, #192
	movs r0, #13
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #4
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #7
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #6
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	movs r0, #9
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #8
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #9
	movs r1, #0
	movs r2, #10
	bl Func_02001548
	movs r1, #230
	movs r2, #230
	lsls r1, r1, #9
	lsls r2, r2, #8
	movs r0, #11
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r2, #24
	movs r0, #11
	movs r1, #16
	negs r2, r2
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #128
	movs r0, #4
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #7
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #6
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #208
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #11
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #4
	adds r1, #255
	movs r2, #30
	movs r0, #11
	bl Func_02001560
	movs r1, #0
	movs r2, #10
	movs r0, #11
	bl Func_02001548
	movs r0, #49
	bl Func_020015f8
	adds r1, r5, #0
	movs r2, #0
	movs r0, #13
	bl Func_02001560
	adds r1, r5, #0
	movs r2, #0
	movs r0, #4
	bl Func_02001560
	adds r1, r5, #0
	movs r2, #0
	movs r0, #7
	bl Func_02001560
	adds r1, r5, #0
	movs r2, #0
	movs r0, #5
	bl Func_02001560
	adds r1, r5, #0
	movs r2, #0
	movs r0, #6
	bl Func_02001560
	adds r1, r5, #0
	movs r2, #0
	movs r0, #8
	bl Func_02001560
	adds r1, r5, #0
	movs r2, #60
	movs r0, #9
	bl Func_02001560
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #9
	bl Func_02001560
	movs r2, #10
	movs r0, #9
	movs r1, #0
	bl Func_02001548
	movs r0, #11
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #11
	movs r1, #0
	movs r2, #10
	bl Func_02001548
	movs r1, #8
	adds r1, #255
	movs r2, #30
	movs r0, #8
	bl Func_02001560
	movs r0, #8
	movs r1, #0
	movs r2, #10
	bl Func_02001548
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #50
	movs r0, #11
	bl Func_02001560
	movs r1, #0
	movs r2, #0
	movs r0, #11
	bl ObjectMotion_ArmCallback
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #11
	movs r1, #0
	movs r2, #20
	bl Func_02001548
	movs r1, #172
	movs r2, #176
	movs r0, #12
	lsls r1, r1, #17
	lsls r2, r2, #16
	bl Func_02001500
	movs r2, #16
	movs r1, #0
	negs r2, r2
	movs r0, #12
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #12
	bl Func_02001560
	movs r0, #12
	movs r1, #0
	movs r2, #10
	bl Func_02001548
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #6
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #6
	adds r1, #255
	movs r2, #0
	movs r0, #13
	bl Func_02001560
	movs r1, #6
	adds r1, #255
	movs r2, #0
	movs r0, #4
	bl Func_02001560
	movs r1, #6
	adds r1, #255
	movs r2, #0
	movs r0, #7
	bl Func_02001560
	movs r1, #6
	adds r1, #255
	movs r2, #0
	movs r0, #5
	bl Func_02001560
	movs r1, #6
	adds r1, #255
	movs r2, #0
	movs r0, #6
	bl Func_02001560
	movs r1, #6
	adds r1, #255
	movs r2, #0
	movs r0, #8
	bl Func_02001560
	movs r1, #6
	adds r1, #255
	movs r2, #60
	movs r0, #9
	bl Func_02001560
	movs r1, #176
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #12
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #208
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #12
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #176
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #12
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #208
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #12
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #2
	adds r1, #255
	movs r2, #30
	movs r0, #12
	bl Func_02001560
	movs r0, #12
	movs r1, #0
	movs r2, #10
	bl Func_02001548
	movs r1, #160
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #11
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	adds r1, r5, #0
	movs r2, #50
	movs r0, #12
	bl Func_02001560
	movs r2, #8
	movs r1, #0
	negs r2, r2
	movs r0, #12
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #12
	movs r1, #0
	movs r2, #10
	bl Func_02001548
	movs r1, #4
	adds r1, #255
	movs r2, #30
	movs r0, #9
	bl Func_02001560
	movs r0, #9
	movs r1, #0
	movs r2, #10
	bl Func_02001548
	adds r1, r5, #0
	movs r2, #40
	movs r0, #12
	bl Func_02001560
	movs r2, #10
	movs r1, #0
	movs r0, #12
	bl Func_02001548
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #8
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #12
	movs r1, #0
	movs r2, #10
	bl Func_02001548
	movs r0, #11
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r2, #0
	movs r1, #0
	movs r0, #12
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #11
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #11
	movs r1, #0
	movs r2, #10
	bl Func_02001548
	movs r1, #160
	movs r0, #11
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	b .L_02008e88
.L_02008e84:
	.4byte 0x00019999
.L_02008e88:
	movs r1, #208
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #12
	bl ObjectMotion_ArmCallback
	movs r0, #35
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #12
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #12
	movs r1, #0
	bl Func_02001548
	movs r1, #3
	movs r0, #11
	bl Motion_SetModeAndWaitAnimation
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #12
	bl Motion_SetModeAndWaitAnimation
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #4
	adds r1, #255
	movs r2, #30
	movs r0, #8
	bl Func_02001560
	movs r0, #8
	movs r1, #0
	movs r2, #10
	bl Func_02001548
	movs r0, #12
	movs r1, #6
	movs r2, #0
	bl ObjectMotion_Launch
	movs r0, #11
	movs r1, #6
	movs r2, #25
	bl ObjectMotion_Launch
	movs r0, #12
	movs r1, #0
	movs r2, #24
	bl ObjectMotion_OffsetPositionAndReset
	movs r1, #16
	movs r0, #11
	negs r1, r1
	movs r2, #16
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #0
	movs r2, #24
	movs r0, #11
	bl ObjectMotion_OffsetPositionAndReset
	movs r0, #12
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r1, #0
	movs r2, #0
	movs r0, #12
	bl Func_02001500
	movs r0, #11
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r2, #0
	movs r1, #0
	movs r0, #11
	bl Func_02001500
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #4
	movs r0, #9
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #224
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #224
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #224
	movs r0, #4
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #224
	movs r0, #7
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #0
	movs r2, #0
	movs r0, #13
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #9
	movs r1, #0
	movs r2, #10
	bl Func_02001548
	movs r1, #128
	movs r2, #128
	movs r0, #9
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #8
	movs r0, #8
	lsls r1, r1, #9
	bl ObjectMotion_SetSpeedParameters
	movs r0, #4
	movs r1, #9
	bl Object_LinkObjectAndSetCallback
	movs r0, #13
	movs r1, #9
	bl Object_LinkObjectAndSetCallback
	movs r0, #7
	movs r1, #9
	bl Object_LinkObjectAndSetCallback
	movs r0, #5
	movs r1, #9
	bl Object_LinkObjectAndSetCallback
	movs r0, #6
	movs r1, #9
	bl Object_LinkObjectAndSetCallback
	movs r0, #9
	movs r1, #16
	movs r2, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #9
	movs r1, #0
	movs r2, #50
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #32
	negs r1, r1
	movs r2, #4
	movs r0, #9
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #4
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #13
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #7
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #5
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #6
	bl ObjectMotion_EnableActionAndResetMotion
	movs r1, #131
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #9
	bl Func_02001560
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #9
	movs r1, #0
	movs r2, #10
	bl Func_02001548
	movs r1, #176
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #9
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #9
	movs r1, #0
	movs r2, #10
	bl Func_02001548
	movs r0, #6
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	movs r0, #4
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #224
	movs r0, #7
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #13
	bl ObjectMotion_ArmCallback
	movs r0, #50
	bl Battle_WaitMode0
	movs r1, #8
	adds r1, #255
	movs r2, #30
	movs r0, #9
	bl Func_02001560
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #9
	movs r1, #0
	movs r2, #10
	bl Func_02001548
	movs r1, #128
	movs r0, #6
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #4
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #7
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #6
	movs r0, #13
	bl ObjectMotion_ArmCallback
	movs r0, #35
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #9
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #9
	movs r1, #0
	movs r2, #10
	bl Func_02001548
	movs r1, #32
	negs r1, r1
	movs r2, #0
	movs r0, #8
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #8
	movs r1, #6
	movs r2, #15
	bl ObjectMotion_Launch
	movs r0, #8
	movs r1, #6
	movs r2, #23
	bl ObjectMotion_Launch
	movs r0, #8
	movs r1, #0
	movs r2, #10
	bl Func_02001548
	movs r1, #208
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #9
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #0
	movs r2, #10
	movs r0, #9
	bl Func_02001548
	movs r0, #78
	bl Func_020015f8
	movs r1, #160
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #8
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #8
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #9
	movs r1, #0
	movs r2, #24
	bl ObjectMotion_OffsetPositionAndReset
	movs r1, #16
	movs r0, #8
	negs r1, r1
	movs r2, #16
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #0
	movs r2, #24
	movs r0, #8
	bl ObjectMotion_OffsetPositionAndReset
	movs r0, #9
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r1, #0
	movs r2, #0
	movs r0, #9
	bl Func_02001500
	movs r0, #8
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r1, #0
	movs r2, #0
	movs r0, #8
	bl Func_02001500
	movs r0, #30
	bl Battle_WaitMode0
	bl Func_020015c0
	movs r1, #10
	adds r1, #255
	movs r2, #30
	movs r0, #7
	bl Func_02001560
	movs r0, #7
	movs r1, #0
	movs r2, #10
	bl Func_02001548
	movs r1, #8
	adds r1, #255
	movs r2, #30
	movs r0, #6
	bl Func_02001560
	movs r0, #6
	movs r1, #0
	movs r2, #10
	bl Func_02001548
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #5
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #128
	movs r0, #4
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #224
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #7
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #13
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	movs r1, #0
	adds r0, #5
	bl UiText_OpenMessageAtObject
	movs r0, #4
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_02009286
	bl Func_020015f0
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #6
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r2, #10
	movs r0, #6
	movs r1, #0
	bl Func_02001548
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_020092b6
.L_02009286:
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #6
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #6
	adds r3, #1
	strh r3, [r2]
	movs r1, #0
	movs r2, #10
	bl Func_02001548
.L_020092b6:
	movs r1, #3
	movs r0, #13
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #13
	movs r1, #0
	movs r2, #10
	bl Func_02001548
	movs r1, #224
	movs r0, #7
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #4
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #13
	bl Func_02001560
	movs r0, #128
	lsls r0, r0, #6
	movs r2, #10
	adds r0, #13
	movs r1, #0
	bl Func_02001548
	movs r1, #3
	movs r0, #13
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	movs r2, #10
	adds r0, #13
	movs r1, #0
	bl Func_02001548
	movs r0, #4
	movs r1, #3
	bl Object_SetModeById
	movs r0, #6
	movs r1, #3
	bl Object_SetModeById
	movs r0, #5
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #7
	bl Motion_SetModeAndWaitAnimation
	movs r0, #25
	bl Battle_WaitMode0
	movs r2, #153
	lsls r2, r2, #8
	adds r2, #153
	movs r0, #7
	ldr r1, .L_02009480
	bl ObjectMotion_SetSpeedParameters
	movs r0, #7
	movs r1, #2
	bl Object_SetModeById
	ldr r3, .L_02009484
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r3, r2
	ldr r0, [r5]
	bl Object_GetById
	cmp r0, #0
	beq .L_02009382
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #7
	bl ObjectMotion_ResetAndSetPosition
.L_02009382:
	movs r0, #7
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #7
	movs r1, #0
	movs r2, #0
	bl Func_02001500
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #5
	ldr r1, .L_02009480
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r0, #5
	movs r1, #2
	bl Object_SetModeById
	ldr r0, [r5]
	bl Object_GetById
	cmp r0, #0
	beq .L_020093c0
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #5
	bl ObjectMotion_ResetAndSetPosition
.L_020093c0:
	movs r0, #5
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl Func_02001500
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #6
	ldr r1, .L_02009480
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r0, #6
	movs r1, #2
	bl Object_SetModeById
	ldr r0, [r5]
	bl Object_GetById
	cmp r0, #0
	beq .L_020093fe
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #6
	bl ObjectMotion_ResetAndSetPosition
.L_020093fe:
	movs r0, #6
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #6
	movs r1, #0
	movs r2, #0
	bl Func_02001500
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #13
	ldr r1, .L_02009480
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r0, #13
	movs r1, #2
	bl Object_SetModeById
	ldr r0, [r5]
	bl Object_GetById
	cmp r0, #0
	beq .L_0200943c
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #13
	bl ObjectMotion_ResetAndSetPosition
.L_0200943c:
	movs r0, #13
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r2, #0
	movs r0, #13
	movs r1, #0
	bl Func_02001500
	ldr r0, [r5]
	movs r1, #1
	bl Object_AttachWorkTargetToObject
	bl Func_020014c0
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #108]
	movs r3, #218
	lsls r3, r3, #1
	adds r2, r1, r3
	movs r3, #16
	str r3, [r2]
	movs r3, #214
	lsls r3, r3, #1
	adds r2, r1, r3
	movs r0, #48
	adds r3, #93
	str r3, [r2]
	adds r0, #255
	bl GameFlag_ClearBit
	pop {r3}
	mov r8, r3
	pop {r5, r6, pc}
.L_02009480:
	.4byte 0x00013333
.L_02009484:
	.4byte gPartyState
