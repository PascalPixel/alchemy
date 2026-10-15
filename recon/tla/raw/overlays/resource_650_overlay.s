.syntax unified
	.thumb
	.section .text.x020095a0,"ax",%progbits
	.balign 4
	.global Func_020015a0
	.thumb_func
Func_020015a0:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	movs r1, #132
	lsls r1, r1, #1
	adds r1, r1, r3
	ldr r3, .L_02009610
	mov r8, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	movs r6, #0
	lsls r3, r2, #2
	adds r3, r3, r2
	ldr r2, .L_02009614
	lsls r3, r3, #6
	adds r5, r3, r2
.L_020095c6:
	ldr r7, .L_02009618
	mov r1, r8
	ldrb r0, [r7]
	movs r2, #6
	ldrsh r3, [r1, r2]
	subs r0, r0, r6
	subs r0, r0, r3
	adds r0, #160
	lsls r0, r0, #9
	bl Math_Sine
	movs r2, #128
	lsls r2, r2, #9
	adds r0, r0, r2
	mov r2, r8
	movs r1, #6
	ldrsh r3, [r2, r1]
	asrs r0, r0, #15
	adds r3, r3, r0
	adds r6, #1
	strh r3, [r5]
	adds r5, #2
	cmp r6, #160
	bne .L_020095c6
	ldr r1, .L_02009610
	ldr r2, .L_0200960c
	ldrh r3, [r1]
	eors r3, r2
	strh r3, [r1]
	ldr r3, [r7]
	adds r3, #1
	str r3, [r7]
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_0200960c:
	.4byte 0x00000001
.L_02009610:
	.4byte Data_02003c14
.L_02009614:
	.4byte Data_02003990
.L_02009618:
	.4byte Data_02003c10
	.section .text.x020096d8,"ax",%progbits
	.global Func_020016d8
	.thumb_func
Func_020016d8:
	push {r5, r6, lr}
	mov r6, r11
	mov r5, r10
	push {r5, r6}
	mov r6, r9
	mov r5, r8
	push {r5, r6}
	bl Engine_EventBegin
	movs r0, #0
	bl Func_02003310
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r1, r1
	negs r2, r2
	movs r3, #0
	negs r0, r0
	bl Motion_CamBounds
	movs r0, #1
	bl Task_Wait
	movs r0, #252
	movs r1, #1
	movs r2, #216
	negs r1, r1
	lsls r2, r2, #16
	movs r3, #0
	lsls r0, r0, #16
	bl Motion_CamBounds
	movs r0, #1
	bl Task_Wait
	bl Func_02003180
	ldr r3, .L_02009b20
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	movs r1, #0
	movs r2, #0
	bl Func_02003228
	movs r3, #128
	movs r1, #184
	movs r2, #168
	lsls r3, r3, #7
	lsls r1, r1, #16
	lsls r2, r2, #16
	movs r0, #5
	mov r10, r3
	bl Func_02003230
	movs r0, #8
	bl Object_GetById
	movs r2, #128
	lsls r2, r2, #8
	mov r9, r2
	mov r3, r9
	movs r1, #186
	movs r2, #164
	strh r3, [r0, #6]
	lsls r2, r2, #16
	lsls r1, r1, #16
	movs r0, #10
	bl Func_02003228
	movs r0, #10
	bl Object_GetById
	movs r1, #15
	bl Object_SetPartAttribute
	movs r0, #10
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	movs r0, #1
	bl Task_Wait
	bl Event_StartScreenTransition
	movs r0, #153
	movs r1, #152
	lsls r0, r0, #8
	lsls r1, r1, #5
	adds r0, #153
	adds r1, #51
	bl Engine_CameraSetSpeed
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #5
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r0, #5
	movs r1, #204
	movs r2, #196
	bl ObjectMotion_SetPositionAndReset
	movs r0, #5
	movs r1, #208
	movs r2, #218
	bl ObjectMotion_SetPositionAndReset
	movs r1, #192
	movs r2, #0
	movs r0, #5
	lsls r1, r1, #7
	bl ObjectMotion_ArmCallback
	movs r1, #5
	movs r0, #8
	bl Engine_ActorPlaceAtActor
	movs r0, #1
	bl Task_Wait
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #8
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r0, #8
	movs r1, #186
	movs r2, #216
	bl ObjectMotion_SetPositionAndReset
	movs r1, #192
	movs r0, #8
	lsls r1, r1, #6
	movs r2, #60
	bl ObjectMotion_ArmCallback
	mov r1, r9
	movs r0, #8
	movs r2, #40
	bl ObjectMotion_ArmCallback
	movs r1, #160
	movs r0, #8
	lsls r1, r1, #7
	movs r2, #20
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #8
	lsls r1, r1, #6
	movs r2, #40
	bl ObjectMotion_ArmCallback
	movs r2, #0
	movs r1, #0
	movs r0, #8
	bl ObjectMotion_ArmCallback
	ldr r0, .L_02009b24
	bl Engine_EventSetMessage
	movs r0, #128
	lsls r0, r0, #5
	movs r1, #0
	adds r0, #8
	bl UiText_OpenMessageAtObject
	movs r2, #0
	mov r1, r9
	movs r0, #5
	bl ObjectMotion_ArmCallback
	movs r1, #0
	movs r0, #5
	bl Inventory_PromptAndSetObjectMode
	movs r0, #5
	bl ObjectMotion_WaitForAnimationChange
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #8
	movs r1, #0
	bl Engine_EventShowMessage
	movs r0, #5
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r1, #208
	movs r0, #8
	lsls r1, r1, #8
	movs r2, #80
	bl ObjectMotion_ArmCallback
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #8
	movs r1, #0
	movs r2, #40
	bl Func_02003298
	movs r1, #131
	movs r2, #40
	lsls r1, r1, #1
	movs r0, #8
	bl Engine_ActorShowEmote
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #8
	movs r1, #0
	bl Engine_EventShowMessage
	movs r0, #5
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r2, #0
	movs r0, #8
	movs r1, #4
	bl ObjectMotion_Launch
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #8
	movs r1, #0
	bl Engine_EventShowMessage
	movs r0, #5
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #8
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #5
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	adds r2, #102
	movs r0, #8
	adds r1, #204
	bl ObjectMotion_SetSpeedParameters
	ldr r1, .L_02009b28
	movs r0, #8
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #40
	bl Battle_WaitMode0
	ldr r1, .L_02009b2c
	movs r0, #5
	bl Object_SetActionCallbackAndRefreshById
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #20
	movs r0, #8
	bl Engine_ActorShowEmote
	mov r1, r9
	movs r0, #8
	movs r2, #40
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #8
	ldr r1, .L_02009b30
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r1, #129
	movs r2, #172
	movs r0, #8
	lsls r1, r1, #1
	bl ObjectMotion_SetPositionAndReset
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #8
	movs r1, #0
	bl Engine_EventShowMessage
	movs r0, #5
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #7
	lsls r2, r2, #6
	movs r0, #5
	adds r1, #102
	adds r2, #51
	bl ObjectMotion_SetSpeedParameters
	movs r0, #5
	movs r1, #242
	movs r2, #172
	bl ObjectMotion_SetPositionAndReset
	movs r0, #8
	movs r1, #248
	movs r2, #156
	bl ObjectMotion_SetPositionAndReset
	movs r0, #8
	movs r1, #226
	movs r2, #164
	bl ObjectMotion_SetPositionAndReset
	movs r0, #8
	movs r1, #228
	movs r2, #180
	bl ObjectMotion_SetPositionAndReset
	movs r1, #192
	movs r2, #40
	movs r0, #8
	lsls r1, r1, #6
	bl ObjectMotion_ArmCallback
	movs r0, #8
	movs r1, #0
	bl Engine_EventShowMessage
	movs r0, #8
	movs r1, #226
	movs r2, #164
	bl ObjectMotion_SetPositionAndReset
	movs r0, #8
	movs r1, #248
	movs r2, #156
	bl ObjectMotion_SetPositionAndReset
	movs r1, #129
	movs r0, #8
	lsls r1, r1, #1
	movs r2, #172
	bl ObjectMotion_SetPositionAndReset
	movs r1, #160
	movs r0, #8
	lsls r1, r1, #7
	movs r2, #20
	bl ObjectMotion_ArmCallback
	movs r1, #6
	movs r2, #20
	adds r1, #255
	movs r0, #8
	bl Engine_ActorShowEmote
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #8
	movs r1, #0
	bl Engine_EventShowMessage
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #8
	movs r1, #6
	movs r2, #0
	bl ObjectMotion_Launch
	movs r1, #128
	movs r0, #8
	lsls r1, r1, #1
	movs r2, #198
	bl ObjectMotion_SetPositionAndReset
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl Func_02003228
	mov r1, r10
	movs r0, #5
	movs r2, #20
	bl ObjectMotion_ArmCallback
	movs r1, #6
	adds r1, #255
	movs r2, #40
	movs r0, #5
	bl Engine_ActorShowEmote
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #5
	movs r1, #0
	movs r2, #20
	bl Func_02003298
	movs r0, #5
	movs r1, #224
	movs r2, #182
	bl ObjectMotion_SetPositionAndReset
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #6
	movs r2, #40
	bl ObjectMotion_ArmCallback
	movs r1, #6
	adds r1, #255
	movs r2, #80
	movs r0, #5
	bl Engine_ActorShowEmote
	movs r0, #5
	movs r1, #242
	movs r2, #172
	bl ObjectMotion_SetPositionAndReset
	mov r1, r10
	movs r0, #5
	movs r2, #40
	bl ObjectMotion_ArmCallback
	movs r1, #6
	adds r1, #255
	movs r2, #120
	movs r0, #5
	bl Engine_ActorShowEmote
	movs r1, #8
	adds r1, #255
	movs r2, #40
	movs r0, #5
	bl Engine_ActorShowEmote
	movs r0, #5
	movs r1, #2
	movs r2, #10
	bl ObjectMotion_Launch
	movs r0, #5
	movs r1, #4
	movs r2, #20
	bl ObjectMotion_Launch
	movs r0, #128
	lsls r0, r0, #8
	movs r2, #40
	adds r0, #5
	movs r1, #0
	bl Func_02003298
	movs r0, #153
	movs r1, #152
	lsls r0, r0, #8
	lsls r1, r1, #5
	adds r0, #153
	adds r1, #51
	bl Engine_CameraSetSpeed
	movs r0, #131
	movs r1, #1
	movs r2, #180
	movs r3, #1
	lsls r0, r0, #17
	negs r1, r1
	lsls r2, r2, #16
	bl Motion_CamBounds
	movs r1, #128
	movs r2, #198
	lsls r1, r1, #17
	lsls r2, r2, #16
	movs r0, #8
	bl Func_02003228
	movs r0, #1
	bl Task_Wait
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #5
	bl Engine_ActorShowEmote
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #8
	movs r1, #6
	movs r2, #0
	bl ObjectMotion_Launch
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #172
	movs r0, #8
	bl ObjectMotion_SetPositionAndReset
	b .L_02009b34
.L_02009b20:
	.4byte gPartyState
.L_02009b24:
	.4byte MsgIdejimaIsThisThePlace
.L_02009b28:
	.4byte Data_02003330
.L_02009b2c:
	.4byte Data_0200336c
.L_02009b30:
	.4byte 0x00019999
.L_02009b34:
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #6
	movs r2, #80
	adds r1, #255
	movs r0, #8
	bl Engine_ActorShowEmote
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #5
	movs r1, #0
	bl Engine_EventShowMessage
	movs r0, #8
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #160
	lsls r0, r0, #8
	adds r0, #8
	movs r1, #0
	bl Engine_EventShowMessage
	movs r1, #2
	adds r1, #255
	movs r2, #0
	movs r0, #5
	bl Engine_ActorShowEmote
	movs r0, #128
	lsls r0, r0, #8
	movs r2, #20
	adds r0, #5
	movs r1, #0
	bl Func_02003298
	movs r0, #160
	lsls r0, r0, #8
	adds r0, #8
	movs r1, #0
	bl Engine_EventShowMessage
	movs r2, #224
	lsls r2, r2, #8
	movs r5, #192
	movs r6, #172
	mov r8, r2
	lsls r5, r5, #14
	lsls r6, r6, #16
	movs r0, #238
	adds r1, r5, #0
	adds r2, r6, #0
	mov r3, r8
	lsls r0, r0, #16
	bl DriftScene_SpawnObject
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #246
	movs r3, #128
	lsls r3, r3, #6
	adds r2, r6, #0
	adds r1, r5, #0
	lsls r0, r0, #16
	mov r11, r3
	bl DriftScene_SpawnObject
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #5
	movs r1, #0
	bl Engine_EventShowMessage
	movs r1, #160
	movs r0, #8
	lsls r1, r1, #7
	movs r2, #40
	bl ObjectMotion_ArmCallback
	movs r0, #192
	lsls r0, r0, #7
	adds r0, #8
	movs r1, #0
	movs r2, #20
	bl Func_02003298
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #5
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r2, #148
	movs r0, #5
	movs r1, #248
	bl ObjectMotion_SetPositionAndReset
	movs r0, #5
	movs r1, #0
	bl Engine_EventShowMessage
	movs r1, #176
	movs r0, #8
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #2
	movs r2, #20
	adds r1, #255
	movs r0, #8
	bl Engine_ActorShowEmote
	movs r0, #160
	lsls r0, r0, #8
	adds r0, #8
	movs r1, #0
	bl Engine_EventShowMessage
	mov r1, r10
	movs r0, #5
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r2, #20
	movs r0, #5
	movs r1, #0
	bl Func_02003298
	movs r0, #8
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #160
	lsls r0, r0, #8
	adds r0, #8
	movs r1, #0
	bl Engine_EventShowMessage
	movs r1, #128
	movs r2, #20
	lsls r1, r1, #1
	movs r0, #5
	bl Engine_ActorShowEmote
	movs r0, #5
	movs r1, #0
	bl Engine_EventShowMessage
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #8
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #244
	movs r2, #170
	movs r0, #8
	bl ObjectMotion_SetPositionAndReset
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #170
	movs r0, #8
	bl ObjectMotion_SetPositionAndReset
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #137
	lsls r1, r1, #1
	movs r2, #170
	movs r0, #8
	bl ObjectMotion_SetPositionAndReset
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #170
	movs r0, #8
	bl ObjectMotion_SetPositionAndReset
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #244
	movs r2, #170
	movs r0, #8
	bl ObjectMotion_SetPositionAndReset
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #170
	movs r0, #8
	bl ObjectMotion_SetPositionAndReset
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #137
	lsls r1, r1, #1
	movs r2, #170
	movs r0, #8
	bl ObjectMotion_SetPositionAndReset
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #170
	movs r0, #8
	bl ObjectMotion_SetPositionAndReset
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #131
	movs r2, #40
	lsls r1, r1, #1
	movs r0, #8
	bl Engine_ActorShowEmote
	movs r0, #160
	lsls r0, r0, #8
	adds r0, #8
	movs r1, #0
	bl Engine_EventShowMessage
	movs r1, #2
	movs r2, #0
	adds r1, #255
	movs r0, #5
	bl Engine_ActorShowEmote
	movs r0, #5
	movs r1, #0
	bl Engine_EventShowMessage
	movs r1, #176
	movs r2, #20
	movs r0, #8
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r0, #8
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #160
	lsls r0, r0, #8
	adds r0, #8
	movs r1, #0
	bl Engine_EventShowMessage
	movs r0, #5
	movs r1, #3
	bl Engine_ActorSetAnimation
	movs r0, #5
	movs r1, #0
	bl Engine_EventShowMessage
	movs r0, #160
	lsls r0, r0, #8
	adds r0, #8
	movs r1, #0
	bl Engine_EventShowMessage
	movs r0, #144
	lsls r0, r0, #8
	adds r0, #10
	movs r1, #0
	bl Engine_EventShowMessage
	movs r1, #192
	movs r0, #5
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r2, #0
	mov r1, r9
	movs r0, #8
	bl ObjectMotion_ArmCallback
	movs r0, #10
	bl Object_GetById
	movs r1, #1
	bl ObjectDispatch_SetSingleChildFlags
	movs r0, #10
	bl Object_GetById
	movs r1, #0
	bl Object_SetPartAttribute
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #40
	movs r0, #10
	movs r1, #0
	bl ObjectMotion_ArmCallback
	movs r0, #10
	movs r1, #3
	bl Engine_ActorSetAnimation
	movs r0, #144
	lsls r0, r0, #8
	adds r0, #10
	movs r1, #0
	bl Engine_EventShowMessage
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #10
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r0, #10
	movs r1, #216
	movs r2, #164
	bl ObjectMotion_SetPositionAndReset
	movs r1, #234
	movs r2, #160
	movs r0, #10
	bl ObjectMotion_SetPositionAndReset
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #192
	movs r2, #20
	movs r0, #10
	lsls r1, r1, #6
	bl ObjectMotion_ArmCallback
	movs r0, #10
	movs r1, #0
	bl Engine_EventShowMessage
	movs r1, #208
	movs r2, #0
	movs r0, #10
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #10
	movs r1, #0
	bl Engine_EventShowMessage
	mov r1, r8
	movs r0, #5
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #208
	movs r2, #20
	movs r0, #8
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r0, #5
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r1, #2
	movs r2, #40
	adds r1, #255
	movs r0, #8
	bl Engine_ActorShowEmote
	movs r0, #160
	lsls r0, r0, #8
	adds r0, #8
	movs r1, #0
	bl Engine_EventShowMessage
	mov r1, r11
	movs r0, #5
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r2, #0
	movs r0, #10
	movs r1, #0
	bl ObjectMotion_ArmCallback
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #10
	movs r1, #0
	bl Engine_EventShowMessage
	movs r0, #160
	lsls r0, r0, #8
	adds r0, #8
	movs r1, #0
	bl Engine_EventShowMessage
	movs r1, #208
	movs r2, #40
	movs r0, #10
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #10
	movs r1, #0
	bl Engine_EventShowMessage
	movs r1, #192
	movs r2, #0
	movs r0, #5
	lsls r1, r1, #7
	bl ObjectMotion_ArmCallback
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #5
	movs r1, #0
	bl Engine_EventShowMessage
	movs r0, #5
	mov r1, r8
	movs r2, #80
	bl ObjectMotion_ArmCallback
	bl Event_FinishScreenTransition
	bl Event_WaitTransitionFrames
	movs r0, #11
	bl Func_020032e8
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r3}
	mov r11, r3
	pop {r5, r6, pc}
	.section .text.x02009ee0,"ax",%progbits
	.global Func_02001ee0
	.thumb_func
Func_02001ee0:
	push {r5, r6, lr}
	movs r0, #5
	sub sp, #28
	bl Object_GetById
	adds r6, r0, #0
	bl Engine_EventBegin
	movs r0, #0
	bl Func_02003310
	bl Func_0200161c
	bl DriftScene_Prepare
	movs r0, #1
	movs r1, #1
	movs r2, #1
	movs r3, #0
	negs r0, r0
	negs r1, r1
	negs r2, r2
	bl Motion_CamBounds
	movs r1, #222
	movs r2, #212
	lsls r2, r2, #16
	movs r0, #6
	lsls r1, r1, #17
	bl Func_02003228
	movs r1, #19
	movs r0, #6
	bl Engine_ActorSetAnimation
	movs r0, #6
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	ldr r5, .L_0200a2e0
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
	movs r1, #216
	movs r2, #234
	lsls r2, r2, #16
	lsls r1, r1, #17
	ldr r0, [r5]
	bl Func_02003228
	ldr r0, [r5]
	bl Object_GetById
	ldr r3, .L_0200a2e4
	movs r1, #41
	str r3, [r0, #24]
	adds r6, #100
	ldr r0, [r5]
	bl Engine_ActorSetAnimation
	ldr r0, [r5]
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	movs r1, #196
	movs r2, #172
	lsls r1, r1, #17
	lsls r2, r2, #16
	movs r0, #10
	bl Func_02003228
	movs r0, #1
	bl Task_Wait
	movs r0, #192
	movs r1, #1
	movs r2, #214
	movs r3, #0
	negs r1, r1
	lsls r2, r2, #16
	lsls r0, r0, #16
	bl Motion_CamBounds
	bl Func_02003180
	movs r0, #1
	bl Task_Wait
	movs r1, #208
	movs r2, #218
	lsls r2, r2, #16
	movs r0, #5
	lsls r1, r1, #16
	bl Func_02003228
	movs r1, #19
	movs r0, #5
	bl Engine_ActorSetAnimation
	movs r0, #5
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	movs r1, #176
	movs r2, #204
	lsls r2, r2, #16
	movs r0, #8
	lsls r1, r1, #16
	bl Func_02003228
	movs r1, #5
	movs r0, #8
	bl Engine_ActorSetAnimation
	movs r0, #8
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	movs r0, #1
	bl Task_Wait
	bl Event_StartScreenTransition
	bl Event_WaitTransitionFrames
	movs r0, #120
	bl Battle_WaitMode0
	ldr r0, .L_0200a2e8
	bl Engine_EventSetMessage
	movs r2, #40
	movs r0, #5
	movs r1, #0
	bl Func_02003298
	movs r0, #8
	movs r1, #0
	bl Engine_EventShowMessage
	movs r1, #8
	movs r2, #0
	adds r1, #255
	movs r0, #5
	bl Engine_ActorShowEmote
	movs r0, #5
	movs r1, #0
	bl Engine_EventShowMessage
	movs r1, #6
	movs r2, #80
	adds r1, #255
	movs r0, #8
	bl Engine_ActorShowEmote
	movs r0, #8
	movs r1, #0
	bl Engine_EventShowMessage
	movs r0, #5
	movs r1, #0
	bl Engine_EventShowMessage
	movs r1, #6
	adds r1, #255
	movs r2, #40
	movs r0, #8
	bl Engine_ActorShowEmote
	movs r1, #10
	movs r2, #80
	adds r1, #255
	movs r0, #5
	bl Engine_ActorShowEmote
	movs r0, #5
	movs r1, #0
	bl Engine_EventShowMessage
	movs r0, #8
	movs r1, #0
	bl Engine_EventShowMessage
	movs r1, #129
	movs r2, #0
	lsls r1, r1, #1
	movs r0, #5
	bl Engine_ActorShowEmote
	movs r0, #5
	movs r1, #0
	bl Engine_EventShowMessage
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #8
	bl Engine_ActorSetAttachedEffect
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #8
	movs r1, #0
	bl Engine_EventShowMessage
	movs r1, #6
	adds r1, #255
	movs r2, #40
	movs r0, #5
	bl Engine_ActorShowEmote
	movs r0, #8
	movs r1, #0
	movs r2, #40
	bl Func_02003298
	movs r1, #10
	adds r1, #255
	movs r2, #20
	movs r0, #5
	bl Engine_ActorShowEmote
	movs r0, #5
	movs r1, #0
	movs r2, #20
	bl Func_02003298
	movs r0, #8
	movs r1, #0
	movs r2, #40
	bl Func_02003298
	movs r1, #6
	movs r2, #20
	adds r1, #255
	movs r0, #5
	bl Engine_ActorShowEmote
	movs r0, #5
	movs r1, #0
	bl Engine_EventShowMessage
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #10
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #152
	movs r0, #10
	lsls r1, r1, #1
	movs r2, #172
	bl ObjectMotion_SetPositionAndReset
	movs r0, #10
	movs r1, #224
	movs r2, #172
	bl ObjectMotion_SetPositionAndReset
	movs r2, #192
	movs r0, #10
	movs r1, #208
	bl ObjectMotion_SetPositionAndReset
	movs r0, #10
	movs r1, #0
	bl Engine_EventShowMessage
	movs r1, #2
	adds r1, #255
	movs r2, #0
	movs r0, #8
	bl Engine_ActorShowEmote
	movs r1, #2
	movs r2, #80
	adds r1, #255
	movs r0, #5
	bl Engine_ActorShowEmote
	movs r0, #10
	movs r1, #0
	bl Engine_EventShowMessage
	ldr r1, .L_0200a2ec
	movs r0, #10
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #1
	movs r0, #8
	bl Engine_ActorSetAnimation
	movs r0, #8
	bl Object_GetById
	movs r1, #1
	bl ObjectDispatch_SetSingleChildFlags
	movs r2, #20
	movs r0, #8
	movs r1, #4
	bl ObjectMotion_Launch
	movs r1, #1
	movs r0, #5
	bl Engine_ActorSetAnimation
	movs r0, #5
	bl Object_GetById
	movs r1, #1
	bl ObjectDispatch_SetSingleChildFlags
	movs r0, #5
	movs r1, #4
	movs r2, #20
	bl ObjectMotion_Launch
	movs r1, #192
	movs r0, #8
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #40
	bl ObjectMotion_ArmCallback
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #8
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	adds r2, #102
	movs r0, #5
	adds r1, #204
	bl ObjectMotion_SetSpeedParameters
	ldr r1, .L_0200a2f0
	movs r0, #8
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #204
	movs r1, #200
	lsls r0, r0, #8
	lsls r1, r1, #5
	adds r0, #204
	adds r1, #153
	bl Engine_CameraSetSpeed
	movs r0, #184
	movs r1, #1
	movs r2, #210
	lsls r0, r0, #17
	negs r1, r1
	movs r3, #1
	lsls r2, r2, #16
	bl Motion_CamBounds
	movs r3, #0
	strh r3, [r6]
	ldr r1, .L_0200a2f4
	movs r0, #5
	bl ObjectMotion_EnableActionAndSetCallback
.L_0200a1f2:
	movs r0, #1
	bl Task_Wait
	movs r2, #0
	ldrsh r3, [r6, r2]
	cmp r3, #0
	beq .L_0200a1f2
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #129
	movs r0, #8
	lsls r1, r1, #1
	bl Engine_ActorSetAttachedEffect
	movs r1, #129
	movs r0, #5
	lsls r1, r1, #1
	bl Engine_ActorSetAttachedEffect
	movs r3, #12
	movs r2, #8
	movs r1, #9
	movs r0, #2
	movs r4, #4
	str r3, [sp, #0]
	str r2, [sp, #4]
	str r1, [sp, #8]
	str r0, [sp, #12]
	movs r3, #2
	movs r5, #1
	movs r6, #0
	movs r0, #5
	movs r1, #7
	movs r2, #13
	str r4, [sp, #16]
	str r5, [sp, #20]
	str r6, [sp, #24]
	bl Func_020032b8
	movs r1, #192
	movs r0, #10
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #8
	ldr r1, .L_0200a2f8
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r2, #204
	lsls r2, r2, #8
	adds r2, #204
	movs r0, #5
	ldr r1, .L_0200a2f8
	bl ObjectMotion_SetSpeedParameters
	ldr r1, .L_0200a2fc
	movs r0, #8
	bl ObjectMotion_EnableActionAndSetCallback
	ldr r1, .L_0200a300
	movs r0, #5
	bl Object_SetActionCallbackAndRefreshById
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #160
	lsls r0, r0, #7
	adds r0, #8
	movs r1, #0
	bl Engine_EventShowMessage
	movs r2, #10
	movs r0, #5
	movs r1, #4
	bl ObjectMotion_Launch
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #5
	movs r1, #0
	bl Engine_EventShowMessage
	movs r1, #176
	movs r0, #8
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #128
	lsls r0, r0, #8
	movs r1, #0
	adds r0, #8
	movs r2, #40
	bl Func_02003298
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #218
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #120
	str r2, [r3]
	bl Event_FinishScreenTransition
	bl Event_WaitTransitionFrames
	movs r0, #12
	bl Func_020032e8
	add sp, #28
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200a2e0:
	.4byte gPartyState
.L_0200a2e4:
	.4byte 0xffff0000
.L_0200a2e8:
	.4byte MsgIdejimaHowLongWillWeDrift
.L_0200a2ec:
	.4byte Data_020033a8
.L_0200a2f0:
	.4byte Data_02003400
.L_0200a2f4:
	.4byte Data_0200346c
.L_0200a2f8:
	.4byte 0x00019999
.L_0200a2fc:
	.4byte Data_020034e4
.L_0200a300:
	.4byte Data_0200350c
	.section .text.x0200a304,"ax",%progbits
	.global Func_02002304
	.thumb_func
Func_02002304:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r6, .L_0200a70c
	movs r2, #133
	lsls r2, r2, #2
	adds r6, r6, r2
	ldr r0, [r6]
	sub sp, #28
	bl Object_GetById
	adds r7, r0, #0
	bl Engine_EventBegin
	movs r0, #0
	bl Func_02003310
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r0, r0
	negs r1, r1
	negs r2, r2
	movs r3, #0
	bl Motion_CamBounds
	bl DriftScene_Prepare
	movs r3, #192
	movs r1, #157
	movs r2, #200
	lsls r3, r3, #6
	movs r0, #8
	lsls r1, r1, #17
	lsls r2, r2, #16
	movs r5, #208
	mov r9, r3
	lsls r5, r5, #8
	bl Func_02003230
	movs r1, #164
	movs r2, #240
	adds r3, r5, #0
	movs r0, #10
	lsls r1, r1, #17
	lsls r2, r2, #16
	bl Func_02003230
	movs r2, #128
	lsls r2, r2, #7
	mov r11, r2
	movs r1, #170
	movs r2, #200
	movs r0, #5
	lsls r1, r1, #17
	lsls r2, r2, #16
	mov r3, r11
	bl Func_02003230
	movs r1, #171
	movs r2, #230
	lsls r2, r2, #16
	movs r3, #0
	movs r0, #6
	lsls r1, r1, #17
	bl Func_02003230
	movs r1, #19
	movs r0, #6
	bl Engine_ActorSetAnimation
	movs r0, #6
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	movs r3, #128
	movs r1, #156
	movs r2, #230
	lsls r3, r3, #8
	lsls r2, r2, #16
	ldr r0, [r6]
	lsls r1, r1, #17
	mov r8, r3
	bl Func_02003230
	movs r1, #41
	ldr r0, [r6]
	bl Engine_ActorSetAnimation
	ldr r0, [r6]
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	movs r0, #1
	bl Task_Wait
	bl Event_StartScreenTransition
	bl Event_WaitTransitionFrames
	movs r0, #20
	bl Battle_WaitMode0
	ldr r0, .L_0200a710
	bl Engine_EventSetMessage
	movs r0, #8
	movs r1, #0
	bl Engine_EventShowMessage
	movs r1, #176
	movs r2, #0
	movs r0, #10
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #10
	movs r1, #0
	bl Engine_EventShowMessage
	movs r0, #5
	movs r1, #4
	bl Engine_ActorSetAnimation
	movs r0, #5
	movs r1, #0
	bl Engine_EventShowMessage
	movs r0, #8
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #6
	movs r1, #2
	bl Motion_SetVarCbAndRefresh
	adds r1, r5, #0
	movs r0, #10
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #80
	movs r0, #10
	bl Engine_ActorShowEmote
	movs r1, #176
	movs r0, #10
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r2, #10
	str r2, [sp, #4]
	movs r3, #6
	movs r1, #12
	movs r0, #16
	movs r2, #0
	movs r4, #18
	str r3, [sp, #0]
	str r1, [sp, #8]
	str r0, [sp, #12]
	str r2, [sp, #24]
	movs r3, #7
	movs r5, #11
	movs r0, #8
	movs r1, #9
	movs r2, #1
	str r5, [sp, #20]
	str r4, [sp, #16]
	bl Func_020032b8
	movs r1, #2
	movs r2, #40
	adds r1, #255
	movs r0, #8
	bl Engine_ActorShowEmote
	movs r0, #6
	movs r1, #2
	bl Motion_SetVarCbAndRefresh
	movs r1, #129
	movs r0, #5
	lsls r1, r1, #1
	bl Engine_ActorSetAttachedEffect
	movs r2, #20
	movs r0, #5
	movs r1, #0
	bl Func_02003298
	movs r1, #1
	movs r0, #6
	bl Motion_SetVarCbAndRefresh
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #6
	movs r1, #0
	bl Engine_EventShowMessage
	movs r0, #8
	movs r1, #0
	bl Engine_EventShowMessage
	movs r0, #5
	movs r1, #0
	bl Engine_EventShowMessage
	movs r1, #2
	movs r2, #40
	adds r1, #255
	movs r0, #6
	bl Engine_ActorShowEmote
	movs r0, #6
	movs r1, #0
	bl Engine_EventShowMessage
	movs r1, #18
	movs r0, #6
	bl Engine_ActorSetAnimation
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #6
	movs r1, #0
	bl Engine_EventShowMessage
	movs r0, #8
	movs r1, #3
	bl Engine_ActorSetAnimation
	movs r0, #8
	movs r1, #0
	movs r2, #40
	bl Func_02003298
	movs r1, #0
	movs r2, #40
	movs r0, #6
	bl Func_02003298
	movs r0, #6
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r5, #254
	adds r3, r5, #0
	ands r3, r2
	movs r1, #170
	strb r3, [r0]
	lsls r1, r1, #1
	movs r0, #6
	movs r2, #226
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r2, #0
	movs r0, #6
	movs r1, #4
	bl ObjectMotion_Launch
	movs r1, #1
	movs r0, #6
	bl Engine_ActorSetAnimation
	movs r0, #6
	bl Object_GetById
	movs r1, #1
	bl ObjectDispatch_SetSingleChildFlags
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #5
	bl Engine_ActorSetAttachedEffect
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #0
	movs r0, #5
	bl Engine_EventShowMessage
	movs r0, #6
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	movs r2, #1
	mov r10, r2
	mov r2, r10
	orrs r3, r2
	movs r1, #160
	strb r3, [r0]
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #6
	bl ObjectMotion_ArmCallback
	movs r0, #6
	bl Object_RefreshSelectorById
	movs r1, #3
	movs r0, #6
	bl Motion_SetModeAndWaitAnimation
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #8
	movs r1, #6
	bl Object_LinkObjectAndSetCallback
	movs r0, #5
	movs r1, #6
	bl Object_LinkObjectAndSetCallback
	movs r0, #10
	movs r1, #6
	bl Object_LinkObjectAndSetCallback
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #7
	lsls r2, r2, #6
	movs r0, #6
	adds r1, #102
	adds r2, #51
	bl ObjectMotion_SetSpeedParameters
	movs r1, #179
	lsls r1, r1, #1
	movs r2, #222
	movs r0, #6
	bl ObjectMotion_SetPositionAndReset
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #128
	movs r0, #6
	lsls r1, r1, #6
	movs r2, #80
	bl ObjectMotion_ArmCallback
	movs r1, #224
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #40
	bl ObjectMotion_ArmCallback
	movs r0, #6
	mov r1, r8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r2, #20
	movs r0, #6
	movs r1, #0
	bl Func_02003298
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #10
	movs r1, #0
	bl Engine_EventShowMessage
	movs r1, #224
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #40
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #6
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #2
	adds r1, #255
	movs r2, #40
	movs r0, #6
	bl Engine_ActorShowEmote
	movs r1, #192
	movs r2, #0
	movs r0, #6
	lsls r1, r1, #7
	bl ObjectMotion_ArmCallback
	movs r0, #6
	movs r1, #0
	bl Engine_EventShowMessage
	movs r0, #8
	movs r1, #4
	bl Engine_ActorSetAnimation
	movs r0, #8
	movs r1, #0
	bl Engine_EventShowMessage
	movs r2, #0
	movs r0, #5
	movs r1, #4
	bl ObjectMotion_Launch
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #5
	movs r1, #0
	bl Engine_EventShowMessage
	movs r2, #20
	movs r0, #6
	mov r1, r8
	bl ObjectMotion_ArmCallback
	movs r0, #10
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #10
	movs r1, #0
	bl Engine_EventShowMessage
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #6
	ldr r1, .L_0200a714
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r1, #174
	movs r0, #6
	lsls r1, r1, #1
	movs r2, #222
	bl ObjectMotion_SetPositionAndReset
	movs r1, #192
	movs r2, #0
	movs r0, #6
	lsls r1, r1, #7
	bl ObjectMotion_ArmCallback
	movs r0, #128
	lsls r0, r0, #7
	movs r1, #0
	adds r0, #6
	bl Engine_EventShowMessage
	movs r0, #8
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #5
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #10
	bl ObjectMotion_EnableActionAndResetMotion
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #7
	lsls r2, r2, #6
	movs r0, #8
	adds r1, #102
	adds r2, #51
	bl ObjectMotion_SetSpeedParameters
	movs r1, #160
	movs r0, #8
	lsls r1, r1, #1
	movs r2, #200
	bl ObjectMotion_SetPositionAndReset
	movs r0, #8
	movs r1, #0
	movs r2, #40
	bl Func_02003298
	movs r2, #40
	movs r0, #6
	mov r1, r8
	bl ObjectMotion_ArmCallback
	movs r0, #6
	movs r1, #0
	bl Engine_EventShowMessage
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #1
	movs r0, #10
	b .L_0200a718
.L_0200a70c:
	.4byte gPartyState
.L_0200a710:
	.4byte MsgIdejimaThoughtTheyWereInsideLighthouse
.L_0200a714:
	.4byte 0x00019999
.L_0200a718:
	bl Engine_ActorShowEmote
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #10
	movs r1, #0
	bl Engine_EventShowMessage
	movs r0, #6
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r1, #192
	movs r2, #0
	movs r0, #6
	lsls r1, r1, #7
	bl ObjectMotion_ArmCallback
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #6
	movs r1, #0
	bl Engine_EventShowMessage
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r2, #0
	movs r0, #5
	mov r1, r8
	bl ObjectMotion_ArmCallback
	movs r0, #5
	movs r1, #0
	bl Engine_EventShowMessage
	movs r1, #160
	movs r2, #0
	movs r0, #6
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r0, #6
	movs r1, #0
	bl Engine_EventShowMessage
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #10
	movs r1, #0
	bl Engine_EventShowMessage
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #6
	bl ObjectMotion_ArmCallback
	movs r0, #6
	bl Object_RefreshSelectorById
	movs r1, #3
	movs r0, #6
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #10
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #10
	movs r1, #0
	bl Engine_EventShowMessage
	movs r1, #128
	movs r2, #0
	movs r0, #8
	lsls r1, r1, #6
	bl ObjectMotion_ArmCallback
	movs r0, #8
	movs r1, #0
	bl Engine_EventShowMessage
	movs r1, #160
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #6
	bl ObjectMotion_ArmCallback
	movs r0, #6
	bl Object_RefreshSelectorById
	movs r0, #6
	movs r1, #4
	bl Engine_ActorSetAnimation
	movs r2, #20
	movs r0, #6
	movs r1, #0
	bl Func_02003298
	movs r1, #2
	ldr r0, [r6]
	bl Motion_SetVarCbAndRefresh
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #5
	mov r1, r11
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #5
	movs r1, #0
	movs r2, #20
	bl Func_02003298
	movs r1, #6
	adds r1, #255
	movs r2, #0
	ldr r0, [r6]
	bl Engine_ActorShowEmote
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #6
	bl ObjectMotion_ArmCallback
	movs r0, #6
	bl Object_RefreshSelectorById
	movs r0, #6
	movs r1, #3
	bl Engine_ActorSetAnimation
	movs r0, #6
	movs r1, #0
	bl Engine_EventShowMessage
	movs r1, #6
	movs r2, #0
	adds r1, #255
	ldr r0, [r6]
	bl Engine_ActorShowEmote
	ldr r0, [r6]
	movs r1, #2
	bl Motion_SetVarCbAndRefresh
	movs r1, #129
	movs r0, #8
	lsls r1, r1, #1
	bl Engine_ActorSetAttachedEffect
	movs r2, #20
	movs r0, #8
	movs r1, #0
	bl Func_02003298
	ldr r0, [r6]
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #20
	movs r0, #10
	bl Engine_ActorShowEmote
	movs r1, #176
	movs r2, #20
	movs r0, #10
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r0, #10
	movs r1, #0
	bl Engine_EventShowMessage
	movs r1, #42
	ldr r0, [r6]
	bl Engine_ActorSetAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #160
	movs r0, #8
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #5
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r2, #20
	movs r0, #6
	mov r1, r8
	bl ObjectMotion_ArmCallback
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #8
	movs r1, #0
	bl Engine_EventShowMessage
	movs r1, #10
	movs r2, #40
	adds r1, #255
	movs r0, #5
	bl Engine_ActorShowEmote
	movs r1, #0
	movs r0, #5
	bl Engine_EventShowMessage
	ldr r0, [r6]
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	movs r1, #157
	ands r5, r3
	strb r5, [r0]
	lsls r1, r1, #1
	ldr r0, [r6]
	movs r2, #226
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r2, #0
	ldr r0, [r6]
	movs r1, #4
	bl ObjectMotion_Launch
	movs r1, #1
	ldr r0, [r6]
	bl Engine_ActorSetAnimation
	ldr r0, [r6]
	bl Object_GetById
	movs r1, #1
	bl ObjectDispatch_SetSingleChildFlags
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r7, #24]
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #129
	movs r0, #5
	lsls r1, r1, #1
	bl Engine_ActorSetAttachedEffect
	movs r1, #0
	movs r0, #5
	bl Engine_EventShowMessage
	ldr r0, [r6]
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	mov r2, r10
	orrs r2, r3
	strb r2, [r0]
	movs r1, #192
	ldr r0, [r6]
	lsls r1, r1, #7
	mov r10, r2
	movs r2, #40
	bl ObjectMotion_ArmCallback
	movs r1, #128
	ldr r0, [r6]
	lsls r1, r1, #6
	movs r2, #40
	bl ObjectMotion_ArmCallback
	movs r1, #160
	movs r2, #80
	ldr r0, [r6]
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #8
	movs r1, #0
	bl Engine_EventShowMessage
	movs r1, #2
	adds r1, #255
	movs r2, #0
	ldr r0, [r6]
	bl Engine_ActorShowEmote
	ldr r0, [r6]
	movs r1, #0
	movs r2, #20
	bl ObjectMotion_ArmCallback
	movs r1, #176
	movs r2, #0
	movs r0, #10
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r0, #10
	movs r1, #0
	bl Engine_EventShowMessage
	movs r1, #128
	ldr r0, [r6]
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #8
	mov r1, r9
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #6
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r2, #0
	movs r0, #5
	lsls r1, r1, #7
	bl ObjectMotion_ArmCallback
	movs r0, #10
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	movs r1, #0
	bl Engine_EventShowMessage
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #5
	mov r1, r8
	movs r2, #40
	bl ObjectMotion_ArmCallback
	movs r1, #10
	movs r2, #20
	adds r1, #255
	movs r0, #5
	bl Engine_ActorShowEmote
	movs r0, #5
	movs r1, #0
	bl Engine_EventShowMessage
	movs r1, #160
	movs r2, #20
	movs r0, #6
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r0, #6
	movs r1, #0
	bl Engine_EventShowMessage
	movs r2, #0
	movs r0, #8
	mov r1, r9
	bl ObjectMotion_ArmCallback
	movs r0, #8
	movs r1, #0
	bl Engine_EventShowMessage
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #5
	mov r1, r8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	ldr r0, [r6]
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #6
	mov r1, r8
	movs r2, #20
	bl ObjectMotion_ArmCallback
	movs r2, #0
	movs r0, #10
	movs r1, #0
	bl ObjectMotion_ArmCallback
	movs r0, #8
	movs r1, #3
	bl Engine_ActorSetAnimation
	movs r0, #5
	movs r1, #3
	bl Engine_ActorSetAnimation
	ldr r0, [r6]
	movs r1, #3
	bl Engine_ActorSetAnimation
	movs r0, #6
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #10
	ldr r1, .L_0200abc8
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r1, #184
	movs r2, #128
	movs r0, #10
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #6
	bl Engine_ActorShowEmote
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #6
	movs r0, #6
	bl ObjectMotion_ArmCallback
	movs r0, #6
	bl Object_RefreshSelectorById
	movs r0, #160
	lsls r0, r0, #7
	adds r0, #6
	movs r1, #0
	bl Engine_EventShowMessage
	movs r0, #8
	mov r1, r9
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	ldr r0, [r6]
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #200
	movs r2, #132
	movs r0, #10
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r2, #0
	movs r0, #10
	movs r1, #0
	bl ObjectMotion_ArmCallback
	movs r0, #8
	movs r1, #0
	bl Engine_EventShowMessage
	movs r1, #132
	movs r2, #0
	lsls r1, r1, #1
	movs r0, #10
	bl Engine_ActorShowEmote
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #10
	movs r1, #0
	bl Engine_EventShowMessage
	movs r0, #153
	movs r1, #152
	lsls r0, r0, #8
	lsls r1, r1, #5
	adds r0, #153
	adds r1, #51
	bl Engine_CameraSetSpeed
	movs r0, #198
	movs r1, #1
	movs r2, #234
	movs r3, #1
	lsls r0, r0, #17
	negs r1, r1
	lsls r2, r2, #16
	bl Motion_CamBounds
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #5
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #6
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
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	adds r2, #102
	ldr r0, [r6]
	adds r1, #204
	bl ObjectMotion_SetSpeedParameters
	ldr r3, .L_0200abc4
	adds r5, r7, #0
	adds r5, #100
	strh r3, [r5]
	ldr r1, .L_0200abcc
	movs r0, #8
	bl ObjectMotion_EnableActionAndSetCallback
	ldr r1, .L_0200abd0
	movs r0, #5
	bl ObjectMotion_EnableActionAndSetCallback
	ldr r0, [r6]
	ldr r1, .L_0200abd4
	bl ObjectMotion_EnableActionAndSetCallback
	ldr r1, .L_0200abd8
	movs r0, #6
	bl ObjectMotion_EnableActionAndSetCallback
	b .L_0200abdc
	.2byte 0x0000
.L_0200abc4:
	.4byte 0x00000000
.L_0200abc8:
	.4byte 0x00019999
.L_0200abcc:
	.4byte Data_020035bc
.L_0200abd0:
	.4byte Data_02003534
.L_0200abd4:
	.4byte Data_02003600
.L_0200abd8:
	.4byte Data_02003578
.L_0200abdc:
	movs r0, #1
	bl Task_Wait
	movs r2, #0
	ldrsh r3, [r5, r2]
	cmp r3, #0
	beq .L_0200abdc
	bl Func_020032e0
	movs r1, #128
	movs r2, #20
	lsls r1, r1, #1
	movs r0, #5
	bl Engine_ActorShowEmote
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #5
	movs r1, #0
	bl Engine_EventShowMessage
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #6
	movs r1, #0
	bl Engine_EventShowMessage
	movs r1, #0
	movs r0, #8
	bl Engine_EventShowMessage
	movs r0, #13
	bl Func_020032e8
	add sp, #28
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x0200aec8,"ax",%progbits
	.global Func_02002ec8
	.thumb_func
Func_02002ec8:
	push {lr}
	bl Func_0200153c
	movs r1, #200
	lsls r1, r1, #4
	ldr r0, .L_0200af70
	bl Scheduler_AddOrUpdateCallback
	movs r1, #144
	ldr r0, .L_0200af74
	lsls r1, r1, #3
	bl Scheduler_AddOrUpdateCallback
	ldr r3, .L_0200af78
	movs r0, #241
	lsls r0, r0, #1
	adds r3, r3, r0
	movs r2, #0
	ldrsh r3, [r3, r2]
	subs r3, #2
	cmp r3, #11
	bhi .L_0200af6a
	ldr r2, .L_0200af7c
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
.L_0200aefc:
	.4byte .L_0200af2c
	.4byte .L_0200af6a
	.4byte .L_0200af3e
	.4byte .L_0200af6a
	.4byte .L_0200af6a
	.4byte .L_0200af6a
	.4byte .L_0200af6a
	.4byte .L_0200af6a
	.4byte .L_0200af6a
	.4byte .L_0200af5a
	.4byte .L_0200af60
	.4byte .L_0200af66
.L_0200af2c:
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200af3e
	movs r0, #1
	bl Func_020031a8
.L_0200af3e:
	ldr r1, .L_0200af78
	ldr r3, .L_0200af80
	movs r0, #242
	lsls r0, r0, #1
	adds r2, r1, r0
	strh r3, [r2]
	movs r3, #243
	lsls r3, r3, #1
	adds r2, r1, r3
	movs r3, #1
	strh r3, [r2]
	bl Func_02002fd4
	b .L_0200af6a
.L_0200af5a:
	bl Func_020016d8
	b .L_0200af6a
.L_0200af60:
	bl Func_02001ee0
	b .L_0200af6a
.L_0200af66:
	bl Func_02002304
.L_0200af6a:
	movs r0, #0
	pop {pc}
	.2byte 0x0000
.L_0200af70:
	.4byte Func_02001558
.L_0200af74:
	.4byte Func_020015a0
.L_0200af78:
	.4byte gPartyState
.L_0200af7c:
	.4byte .L_0200aefc
.L_0200af80:
	.4byte 0x0000000c
	.section .text.x0200af88,"ax",%progbits
	.global Func_02002f88
	.thumb_func
Func_02002f88:
	push {r5, r6, lr}
	adds r5, r0, #0
	bl Object_GetById
	adds r6, r0, #0
	adds r0, r5, #0
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	movs r3, #128
	ldr r2, .L_0200afcc
	lsls r3, r3, #7
	strh r3, [r6, #6]
	adds r3, r6, #0
	adds r3, #85
	strb r2, [r3]
	movs r1, #192
	ldr r3, [r6, #8]
	lsls r1, r1, #12
	adds r3, r3, r1
	str r3, [r6, #8]
	movs r3, #128
	lsls r3, r3, #13
	str r3, [r6, #12]
	adds r3, r6, #0
	adds r3, #89
	strb r2, [r3]
	movs r3, #192
	ldr r2, [r6, #80]
	lsls r3, r3, #8
	strh r3, [r2, #18]
	b .L_0200afd0
.L_0200afcc:
	.4byte 0x00000000
.L_0200afd0:
	pop {r5, r6, pc}
	.2byte 0x0000
	.section .text.x0200afd4,"ax",%progbits
	.global Func_02002fd4
	.thumb_func
Func_02002fd4:
	push {r5, r6, lr}
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #99
	sub sp, #8
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200b044
	movs r1, #208
	movs r2, #170
	movs r0, #8
	lsls r1, r1, #17
	lsls r2, r2, #18
	bl Func_02003228
	movs r3, #25
	movs r2, #42
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #1
	movs r2, #2
	movs r1, #32
	movs r0, #38
	bl Func_02003190
	movs r0, #8
	bl Object_GetById
	adds r5, r0, #0
	movs r0, #8
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	movs r3, #128
	ldr r2, .L_0200b040
	lsls r3, r3, #7
	strh r3, [r5, #6]
	adds r3, r5, #0
	adds r3, #85
	strb r2, [r3]
	movs r3, #192
	lsls r3, r3, #12
	str r3, [r5, #12]
	adds r5, #89
	strb r2, [r5]
	movs r0, #8
	movs r1, #5
	bl Engine_ActorSetAnimation
	b .L_0200b044
	.2byte 0x0000
.L_0200b040:
	.4byte 0x00000000
.L_0200b044:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #100
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200b0b0
	movs r1, #144
	movs r2, #186
	movs r0, #5
	lsls r1, r1, #17
	lsls r2, r2, #18
	bl Func_02003228
	movs r3, #17
	movs r2, #46
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #1
	movs r2, #2
	movs r1, #33
	movs r0, #38
	bl Func_02003190
	movs r0, #5
	bl Object_GetById
	adds r5, r0, #0
	movs r0, #5
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	movs r3, #128
	ldr r2, .L_0200b0ac
	lsls r3, r3, #7
	strh r3, [r5, #6]
	adds r3, r5, #0
	adds r3, #85
	strb r2, [r3]
	movs r3, #192
	lsls r3, r3, #12
	str r3, [r5, #12]
	adds r5, #89
	strb r2, [r5]
	movs r0, #5
	movs r1, #18
	bl Engine_ActorSetAnimation
	b .L_0200b0b0
	.2byte 0x0000
.L_0200b0ac:
	.4byte 0x00000000
.L_0200b0b0:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #101
	bl GameFlag_Test
	adds r6, r0, #0
	cmp r6, #0
	bne .L_0200b10a
	movs r0, #6
	bl Object_GetById
	movs r1, #240
	movs r2, #162
	adds r5, r0, #0
	lsls r1, r1, #16
	movs r0, #6
	lsls r2, r2, #18
	bl Func_02003228
	movs r3, #14
	movs r2, #40
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #1
	movs r2, #2
	movs r0, #38
	movs r1, #34
	bl Func_02003190
	movs r1, #19
	movs r0, #6
	bl Engine_ActorSetAnimation
	movs r0, #6
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	adds r3, r5, #0
	adds r3, #85
	strb r6, [r3]
	movs r3, #192
	lsls r3, r3, #12
	str r3, [r5, #12]
.L_0200b10a:
	movs r3, #1
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #72
	movs r1, #42
	movs r2, #75
	movs r3, #40
	bl Engine_MapCopyCellsTo
	movs r0, #1
	bl Task_Wait
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #68
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200b134
	bl Func_02002c64
.L_0200b134:
	add sp, #8
	pop {r5, r6, pc}
	.section .bss,"aw",%nobits
	.space 0x00000008
	.global Data_02003990
Data_02003990:
	.space 0x00000280
	.global Data_02003c10
Data_02003c10:
	.space 0x00000004
	.global Data_02003c14
Data_02003c14:
