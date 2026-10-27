.syntax unified
	.thumb
	.section .text.x020080c6,"ax",%progbits
	.2byte 0x0000
	.section .text.x02008510,"ax",%progbits
	.global Func_02000510
	.thumb_func
Func_02000510:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r0, #137
	lsls r0, r0, #4
	bl GameFlag_SetBit
	bl Func_020028b4
	movs r0, #0
	bl Func_020029cc
	ldr r0, .L_0200892c
	bl Func_0200293c
	movs r1, #128
	movs r2, #128
	movs r0, #12
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r1, #128
	movs r2, #128
	movs r0, #13
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r0, #12
	movs r1, #0
	movs r2, #0
	bl Func_0200294c
	movs r1, #188
	movs r2, #140
	lsls r1, r1, #1
	lsls r2, r2, #1
	movs r0, #5
	bl ObjectMotion_SetPositionAndReset
	movs r0, #10
	bl FieldEvent_WaitTicks
	movs r1, #2
	adds r1, #255
	movs r2, #40
	movs r0, #5
	bl Func_02002964
	movs r1, #192
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #5
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl FieldEvent_WaitTicks
	movs r1, #128
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #5
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl FieldEvent_WaitTicks
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #5
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl FieldEvent_WaitTicks
	movs r1, #2
	movs r0, #5
	bl Motion_SetVarCbAndRefresh
	movs r0, #20
	bl FieldEvent_WaitTicks
	movs r1, #188
	movs r2, #180
	lsls r1, r1, #1
	lsls r2, r2, #1
	movs r0, #5
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r0, #30
	bl FieldEvent_WaitTicks
	movs r0, #188
	movs r1, #1
	movs r2, #212
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #17
	lsls r0, r0, #17
	bl Camera_MoveWithinMapBounds
	bl Func_0200298c
	movs r0, #5
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #10
	bl FieldEvent_WaitTicks
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #40
	movs r0, #12
	bl Func_02002964
	movs r2, #10
	movs r0, #12
	movs r1, #0
	bl Func_0200294c
	movs r1, #2
	movs r0, #13
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl FieldEvent_WaitTicks
	movs r0, #13
	movs r1, #0
	movs r2, #10
	bl Func_0200294c
	movs r1, #8
	adds r1, #255
	movs r2, #30
	movs r0, #17
	bl Func_02002964
	movs r0, #17
	movs r1, #0
	movs r2, #10
	bl Func_0200294c
	movs r0, #18
	movs r1, #6
	movs r2, #15
	bl ObjectMotion_Launch
	movs r0, #18
	movs r1, #6
	movs r2, #25
	bl ObjectMotion_Launch
	movs r0, #18
	movs r1, #0
	movs r2, #10
	bl Func_0200294c
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #14
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl FieldEvent_WaitTicks
	movs r1, #160
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #14
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl FieldEvent_WaitTicks
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #14
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl FieldEvent_WaitTicks
	movs r1, #128
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #14
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl FieldEvent_WaitTicks
	movs r2, #10
	movs r0, #14
	movs r1, #0
	bl Func_0200294c
	movs r1, #2
	movs r0, #15
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl FieldEvent_WaitTicks
	movs r0, #15
	movs r1, #0
	movs r2, #10
	bl Func_0200294c
	movs r1, #160
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #19
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl FieldEvent_WaitTicks
	movs r2, #10
	movs r0, #19
	movs r1, #0
	bl Func_0200294c
	movs r1, #3
	movs r0, #20
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl FieldEvent_WaitTicks
	movs r2, #10
	movs r0, #20
	movs r1, #0
	bl Func_0200294c
	movs r1, #3
	movs r0, #19
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl FieldEvent_WaitTicks
	movs r1, #224
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #15
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl FieldEvent_WaitTicks
	movs r1, #160
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #16
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl FieldEvent_WaitTicks
	movs r2, #10
	movs r0, #15
	movs r1, #0
	bl Func_0200294c
	movs r1, #3
	movs r0, #16
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl FieldEvent_WaitTicks
	movs r0, #16
	movs r1, #0
	movs r2, #10
	bl Func_0200294c
	movs r0, #16
	movs r1, #64
	movs r2, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #0
	movs r2, #0
	movs r0, #16
	bl Func_0200290c
	movs r0, #10
	bl FieldEvent_WaitTicks
	movs r1, #160
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #20
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl FieldEvent_WaitTicks
	movs r1, #128
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #21
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl FieldEvent_WaitTicks
	movs r2, #10
	movs r0, #20
	movs r1, #0
	bl Func_0200294c
	movs r1, #3
	movs r0, #21
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl FieldEvent_WaitTicks
	movs r0, #21
	movs r1, #0
	movs r2, #10
	bl Func_0200294c
	movs r1, #64
	movs r0, #21
	negs r1, r1
	movs r2, #64
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #0
	movs r2, #0
	movs r0, #21
	bl Func_0200290c
	movs r0, #10
	bl FieldEvent_WaitTicks
	movs r1, #160
	movs r0, #14
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	movs r0, #15
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #224
	movs r0, #19
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #224
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #20
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl FieldEvent_WaitTicks
	movs r0, #188
	movs r1, #1
	movs r2, #180
	negs r1, r1
	lsls r2, r2, #17
	movs r3, #1
	lsls r0, r0, #17
	bl Camera_MoveWithinMapBounds
	bl Func_0200298c
	movs r0, #10
	bl FieldEvent_WaitTicks
	movs r1, #16
	movs r2, #16
	movs r3, #128
	movs r0, #8
	negs r1, r1
	negs r2, r2
	lsls r3, r3, #7
	bl Func_020029d4
	movs r2, #16
	movs r3, #128
	lsls r3, r3, #7
	movs r1, #16
	negs r2, r2
	movs r0, #9
	bl Func_020029d4
	movs r0, #8
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #20
	bl FieldEvent_WaitTicks
	movs r1, #128
	movs r2, #128
	movs r0, #9
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #9
	lsls r2, r2, #8
	movs r0, #8
	bl ObjectMotion_SetSpeedParameters
	movs r0, #20
	bl FieldEvent_WaitTicks
	movs r2, #16
	movs r1, #0
	movs r0, #8
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #20
	bl FieldEvent_WaitTicks
	movs r1, #2
	movs r0, #8
	bl Motion_SetVarCbAndRefresh
	movs r0, #20
	bl FieldEvent_WaitTicks
	movs r1, #0
	movs r2, #0
	movs r0, #8
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl FieldEvent_WaitTicks
	movs r0, #8
	movs r1, #0
	movs r2, #10
	bl Func_0200294c
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #5
	bl ObjectMotion_ArmCallback
	movs r0, #40
	bl FieldEvent_WaitTicks
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #5
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl FieldEvent_WaitTicks
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #5
	movs r1, #0
	movs r2, #10
	bl Func_0200294c
	movs r1, #2
	adds r1, #255
	movs r2, #30
	movs r0, #9
	bl Func_02002964
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_0200294c
	movs r1, #0
	movs r2, #16
	movs r0, #9
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #10
	bl FieldEvent_WaitTicks
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #9
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl FieldEvent_WaitTicks
	movs r0, #9
	movs r1, #0
	movs r2, #10
	bl Func_0200294c
	movs r1, #0
	movs r2, #0
	movs r0, #5
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl FieldEvent_WaitTicks
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #60
	b .L_02008930
.L_0200892c:
	.4byte MsgThereTheyAre
.L_02008930:
	movs r0, #5
	bl Func_02002964
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #5
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl FieldEvent_WaitTicks
	movs r1, #3
	movs r0, #5
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl FieldEvent_WaitTicks
	movs r2, #10
	movs r0, #5
	movs r1, #0
	bl Func_0200294c
	movs r1, #2
	movs r0, #9
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl FieldEvent_WaitTicks
	movs r0, #9
	movs r1, #0
	movs r2, #10
	bl Func_0200294c
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #9
	bl ObjectMotion_SetSpeedParameters
	movs r0, #10
	bl FieldEvent_WaitTicks
	movs r0, #9
	movs r1, #0
	movs r2, #16
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #192
	movs r0, #9
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #8
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl FieldEvent_WaitTicks
	movs r1, #2
	adds r1, #255
	movs r2, #30
	movs r0, #8
	bl Func_02002964
	movs r0, #8
	movs r1, #0
	movs r2, #10
	bl Func_0200294c
	movs r1, #160
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #9
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl FieldEvent_WaitTicks
	movs r1, #0
	movs r0, #9
	bl UiText_OpenMessageAtObject
	movs r0, #10
	bl FieldEvent_WaitTicks
	movs r0, #5
	movs r1, #0
	bl Event_AskYesNoWithActorAnimation
	cmp r0, #0
	bne .L_02008a7c
	movs r0, #20
	bl FieldEvent_WaitTicks
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r2, #0
	movs r1, #0
	movs r0, #8
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl FieldEvent_WaitTicks
	movs r1, #3
	movs r0, #8
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl FieldEvent_WaitTicks
	movs r1, #3
	movs r0, #5
	bl Motion_SetModeAndWaitAnimation
	movs r0, #30
	bl FieldEvent_WaitTicks
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #8
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl FieldEvent_WaitTicks
	movs r2, #10
	movs r0, #8
	movs r1, #0
	bl Func_0200294c
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_02008ac2
.L_02008a7c:
	movs r0, #30
	bl FieldEvent_WaitTicks
	movs r2, #0
	movs r1, #0
	movs r0, #8
	bl ObjectMotion_ArmCallback
	movs r0, #10
	bl FieldEvent_WaitTicks
	movs r0, #8
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #8
	adds r3, #1
	strh r3, [r2]
	movs r1, #0
	movs r2, #10
	bl Func_0200294c
	movs r1, #128
	movs r0, #8
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
.L_02008ac2:
	movs r1, #3
	movs r0, #9
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl FieldEvent_WaitTicks
	movs r2, #10
	movs r0, #9
	movs r1, #0
	bl Func_0200294c
	movs r1, #2
	movs r0, #9
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl FieldEvent_WaitTicks
	movs r2, #10
	movs r0, #9
	movs r1, #0
	bl Func_0200294c
	movs r0, #5
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #8
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl FieldEvent_WaitTicks
	movs r1, #160
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #9
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl FieldEvent_WaitTicks
	movs r1, #192
	movs r0, #5
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #8
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl FieldEvent_WaitTicks
	movs r0, #156
	movs r1, #1
	movs r2, #212
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #17
	lsls r0, r0, #17
	bl Camera_MoveWithinMapBounds
	bl Func_0200298c
	movs r0, #20
	bl FieldEvent_WaitTicks
	movs r0, #9
	movs r1, #0
	movs r2, #10
	bl Func_0200294c
	movs r0, #5
	movs r1, #0
	movs r2, #10
	bl Func_0200294c
	movs r0, #8
	movs r1, #0
	movs r2, #10
	bl Func_0200294c
	movs r1, #128
	movs r0, #17
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #0
	movs r2, #0
	movs r0, #18
	bl ObjectMotion_ArmCallback
	movs r0, #40
	bl FieldEvent_WaitTicks
	movs r1, #224
	movs r0, #17
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #224
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #18
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl FieldEvent_WaitTicks
	movs r0, #17
	movs r1, #0
	movs r2, #10
	bl Func_0200294c
	movs r1, #8
	adds r1, #255
	movs r2, #30
	movs r0, #18
	bl Func_02002964
	movs r0, #18
	movs r1, #0
	movs r2, #10
	bl Func_0200294c
	movs r0, #188
	movs r1, #1
	movs r2, #212
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #17
	lsls r0, r0, #17
	bl Camera_MoveWithinMapBounds
	bl Func_0200298c
	movs r0, #20
	bl FieldEvent_WaitTicks
	movs r1, #128
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #9
	bl ObjectMotion_ArmCallback
	movs r0, #25
	bl FieldEvent_WaitTicks
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #9
	movs r1, #0
	movs r2, #10
	bl Func_0200294c
	movs r1, #128
	movs r0, #8
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #5
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl FieldEvent_WaitTicks
	movs r0, #5
	movs r1, #0
	movs r2, #10
	bl Func_0200294c
	movs r0, #188
	movs r1, #1
	movs r2, #180
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #17
	lsls r0, r0, #17
	bl Camera_MoveWithinMapBounds
	bl Func_0200298c
	movs r0, #10
	bl FieldEvent_WaitTicks
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #9
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl FieldEvent_WaitTicks
	movs r1, #3
	movs r0, #9
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl FieldEvent_WaitTicks
	movs r0, #9
	movs r1, #0
	movs r2, #10
	bl Func_0200294c
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #40
	movs r0, #5
	bl Func_02002964
	movs r2, #10
	movs r0, #5
	movs r1, #0
	bl Func_0200294c
	movs r1, #2
	movs r0, #9
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl FieldEvent_WaitTicks
	movs r2, #10
	movs r0, #9
	movs r1, #0
	bl Func_0200294c
	movs r0, #5
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #8
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl FieldEvent_WaitTicks
	movs r1, #3
	movs r0, #9
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl FieldEvent_WaitTicks
	movs r0, #9
	movs r1, #0
	movs r2, #10
	bl Func_0200294c
	movs r0, #188
	movs r1, #1
	movs r2, #204
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #17
	lsls r0, r0, #17
	bl Camera_MoveWithinMapBounds
	bl Func_0200298c
	movs r0, #20
	bl FieldEvent_WaitTicks
	movs r1, #192
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #9
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl FieldEvent_WaitTicks
	movs r1, #128
	movs r2, #128
	movs r0, #9
	lsls r1, r1, #8
	lsls r2, r2, #7
	bl ObjectMotion_SetSpeedParameters
	movs r0, #9
	movs r1, #0
	movs r2, #16
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #128
	movs r0, #9
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #240
	movs r0, #17
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #240
	movs r0, #18
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #240
	movs r0, #19
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #240
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #20
	bl ObjectMotion_ArmCallback
	movs r0, #10
	bl FieldEvent_WaitTicks
	movs r1, #2
	movs r0, #12
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl FieldEvent_WaitTicks
	movs r2, #10
	movs r0, #12
	movs r1, #0
	bl Func_0200294c
	movs r0, #9
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #128
	lsls r0, r0, #5
	movs r2, #10
	adds r0, #9
	movs r1, #0
	bl Func_0200294c
	movs r1, #2
	movs r0, #13
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl FieldEvent_WaitTicks
	movs r0, #13
	movs r1, #0
	movs r2, #10
	bl Func_0200294c
	movs r1, #144
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #9
	bl ObjectMotion_ArmCallback
	movs r0, #25
	bl FieldEvent_WaitTicks
	movs r0, #9
	movs r1, #0
	movs r2, #10
	bl Func_0200294c
	movs r1, #128
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #9
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl FieldEvent_WaitTicks
	movs r0, #12
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #13
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl FieldEvent_WaitTicks
	movs r0, #12
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r1, #3
	movs r0, #13
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl FieldEvent_WaitTicks
	movs r1, #160
	movs r0, #12
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #13
	bl ObjectMotion_ArmCallback
	movs r0, #10
	bl FieldEvent_WaitTicks
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #12
	ldr r1, .L_02009220
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #13
	ldr r1, .L_02009220
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r1, #26
	movs r0, #12
	negs r1, r1
	movs r2, #8
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #176
	movs r0, #12
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #16
	movs r2, #12
	movs r0, #13
	negs r1, r1
	negs r2, r2
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #13
	bl ObjectMotion_ArmCallback
	movs r0, #10
	bl FieldEvent_WaitTicks
	movs r1, #2
	movs r0, #9
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl FieldEvent_WaitTicks
	movs r0, #128
	lsls r0, r0, #5
	movs r2, #10
	movs r1, #0
	adds r0, #9
	bl Func_0200294c
	movs r0, #10
	bl FieldEvent_WaitTicks
	movs r1, #8
	movs r0, #9
	bl Object_SetModeById
	movs r0, #50
	bl FieldEvent_WaitTicks
	bl Func_02002a44
	movs r0, #9
	bl Object_GetById
	movs r1, #2
	bl Func_02002a5c
	movs r0, #178
	bl Engine_AudioPlayCue
	movs r0, #20
	bl FieldEvent_WaitTicks
	movs r1, #128
	lsls r1, r1, #7
	adds r1, #132
	movs r0, #8
	bl Func_02002a34
	movs r0, #60
	bl FieldEvent_WaitTicks
	movs r0, #195
	lsls r0, r0, #1
	bl Engine_AudioPlayCue
	movs r1, #0
	movs r0, #12
	bl ObjectMotion_SetActionVariant
	movs r0, #12
	bl Func_020023a4
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #17
	bl Func_02002964
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #18
	bl Func_02002964
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #19
	bl Func_02002964
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #20
	bl Func_02002964
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #13
	bl Func_02002964
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #14
	bl Func_02002964
	movs r1, #128
	movs r2, #50
	lsls r1, r1, #1
	movs r0, #15
	bl Func_02002964
	movs r1, #1
	movs r0, #13
	bl ObjectMotion_SetActionVariant
	movs r0, #13
	bl Func_020023a4
	movs r0, #30
	bl FieldEvent_WaitTicks
	movs r0, #9
	bl Object_GetById
	movs r1, #0
	bl Func_02002a5c
	bl Func_02002a54
	bl Func_02002a4c
	movs r0, #16
	bl Field_StartPaletteTransition
	movs r0, #10
	bl FieldEvent_WaitTicks
	movs r1, #9
	movs r0, #9
	bl Object_SetModeById
	movs r0, #30
	bl FieldEvent_WaitTicks
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #40
	movs r0, #14
	bl Func_02002964
	movs r2, #10
	movs r0, #14
	movs r1, #0
	bl Func_0200294c
	movs r1, #2
	movs r0, #15
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl FieldEvent_WaitTicks
	movs r2, #10
	movs r0, #15
	movs r1, #0
	bl Func_0200294c
	movs r0, #14
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	movs r0, #14
	lsls r1, r1, #1
	bl Func_0200296c
	movs r0, #15
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #15
	bl Func_0200296c
	movs r0, #50
	bl FieldEvent_WaitTicks
	movs r1, #128
	movs r0, #14
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #15
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #14
	movs r1, #6
	movs r2, #15
	bl ObjectMotion_Launch
	movs r0, #14
	movs r1, #6
	movs r2, #25
	bl ObjectMotion_Launch
	movs r0, #14
	movs r1, #0
	movs r2, #10
	bl Func_0200294c
	movs r1, #192
	movs r0, #5
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	movs r0, #8
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #9
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl FieldEvent_WaitTicks
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #9
	bl Func_02002964
	movs r0, #9
	movs r1, #0
	movs r2, #10
	bl Func_0200294c
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #9
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #16
	movs r0, #9
	negs r1, r1
	movs r2, #16
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #192
	movs r0, #9
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #180
	movs r1, #1
	movs r2, #204
	movs r3, #1
	lsls r2, r2, #17
	lsls r0, r0, #17
	negs r1, r1
	bl Camera_MoveWithinMapBounds
	bl Func_0200298c
	movs r0, #17
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	movs r0, #17
	lsls r1, r1, #1
	bl Func_0200296c
	movs r0, #18
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	movs r0, #18
	lsls r1, r1, #1
	bl Func_0200296c
	movs r0, #19
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	movs r0, #19
	lsls r1, r1, #1
	bl Func_0200296c
	movs r0, #20
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	movs r0, #20
	lsls r1, r1, #1
	bl Func_0200296c
	movs r0, #21
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #21
	bl Func_0200296c
	movs r0, #50
	bl FieldEvent_WaitTicks
	movs r0, #17
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #17
	movs r1, #0
	movs r2, #10
	bl Func_0200294c
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #40
	movs r0, #18
	bl Func_02002964
	movs r2, #10
	movs r0, #18
	movs r1, #0
	bl Func_0200294c
	movs r0, #19
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r2, #10
	movs r0, #19
	movs r1, #0
	bl Func_0200294c
	movs r0, #20
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #20
	bl Func_0200296c
	movs r0, #55
	bl FieldEvent_WaitTicks
	movs r1, #128
	movs r0, #17
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #18
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	movs r0, #19
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #224
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #20
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl FieldEvent_WaitTicks
	movs r1, #2
	movs r0, #20
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl FieldEvent_WaitTicks
	movs r0, #20
	movs r1, #0
	movs r2, #10
	bl Func_0200294c
	movs r0, #18
	movs r1, #6
	movs r2, #0
	bl ObjectMotion_Launch
	movs r0, #19
	movs r1, #6
	movs r2, #0
	bl ObjectMotion_Launch
	movs r0, #20
	movs r1, #6
	movs r2, #25
	bl ObjectMotion_Launch
	bl Func_020026f4
	movs r1, #230
	movs r2, #230
	lsls r1, r1, #9
	lsls r2, r2, #8
	movs r0, #20
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #230
	movs r2, #230
	lsls r1, r1, #9
	lsls r2, r2, #8
	movs r0, #19
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #230
	movs r2, #230
	lsls r1, r1, #9
	lsls r2, r2, #8
	movs r0, #18
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #230
	movs r2, #230
	lsls r1, r1, #9
	lsls r2, r2, #8
	adds r2, #102
	adds r1, #204
	movs r0, #17
	bl ObjectMotion_SetSpeedParameters
	movs r0, #20
	bl Object_GetById
	ldr r6, .L_02009224
	adds r7, r0, #0
	adds r3, r7, #0
	movs r5, #0
	adds r3, #100
	strh r5, [r3]
	movs r0, #19
	str r6, [r7, #108]
	bl Object_GetById
	adds r7, r0, #0
	adds r3, r7, #0
	adds r3, #100
	strh r5, [r3]
	movs r0, #18
	str r6, [r7, #108]
	bl Object_GetById
	adds r7, r0, #0
	b .L_02009228
.L_02009220:
	.4byte 0x00013333
.L_02009224:
	.4byte VenusLighthouseEntrance_UpdateSparkTrail
.L_02009228:
	adds r3, r7, #0
	adds r3, #100
	strh r5, [r3]
	movs r0, #20
	str r6, [r7, #108]
	movs r1, #10
	bl Object_SetModeById
	movs r0, #19
	movs r1, #10
	bl Object_SetModeById
	movs r0, #18
	movs r1, #10
	bl Object_SetModeById
	movs r1, #24
	movs r0, #20
	negs r1, r1
	movs r2, #24
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r1, #24
	movs r0, #19
	negs r1, r1
	movs r2, #24
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r1, #24
	negs r1, r1
	movs r2, #24
	movs r0, #18
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r0, #18
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r2, #240
	movs r0, #20
	movs r1, #224
	lsls r2, r2, #1
	bl ObjectMotion_ResetAndSetPosition
	movs r2, #240
	movs r0, #19
	movs r1, #224
	lsls r2, r2, #1
	bl ObjectMotion_ResetAndSetPosition
	movs r2, #240
	movs r1, #224
	lsls r2, r2, #1
	movs r0, #18
	bl ObjectMotion_SetPositionAndCommit
	movs r0, #20
	bl Object_GetById
	adds r7, r0, #0
	str r5, [r7, #108]
	movs r0, #19
	bl Object_GetById
	adds r7, r0, #0
	str r5, [r7, #108]
	movs r0, #18
	bl Object_GetById
	adds r7, r0, #0
	str r5, [r7, #108]
	movs r0, #1
	bl FieldEvent_WaitTicks
	movs r0, #17
	movs r1, #6
	movs r2, #15
	bl ObjectMotion_Launch
	movs r1, #6
	movs r2, #25
	movs r0, #17
	bl ObjectMotion_Launch
	movs r0, #17
	bl Object_GetById
	movs r2, #100
	adds r7, r0, #0
	adds r2, r2, r7
	mov r10, r2
	mov r3, r10
	strh r5, [r3]
	movs r0, #17
	str r6, [r7, #108]
	movs r1, #10
	bl Object_SetModeById
	movs r1, #20
	negs r1, r1
	movs r2, #20
	movs r0, #17
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r0, #17
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #17
	movs r1, #4
	movs r2, #0
	bl ObjectMotion_Launch
	movs r1, #20
	movs r2, #20
	negs r1, r1
	movs r0, #17
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r0, #17
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r1, #1
	movs r0, #17
	bl Object_SetModeById
	movs r0, #133
	bl Engine_AudioPlayCue
	ldr r3, [r7, #8]
	movs r2, #128
	lsls r2, r2, #12
	adds r3, r3, r2
	str r3, [r7, #8]
	ldr r2, .L_0200972c
	ldr r3, [r7, #16]
	str r5, [r7, #108]
	adds r3, r3, r2
	str r3, [r7, #16]
	movs r3, #192
	lsls r3, r3, #8
	mov r8, r3
	mov r2, r8
	strh r2, [r7, #6]
	movs r2, #160
	ldr r3, [r7, #80]
	lsls r2, r2, #8
	mov r11, r2
	mov r2, r11
	strh r2, [r3, #18]
	movs r0, #55
	bl FieldEvent_WaitTicks
	ldr r3, [r7, #8]
	ldr r2, .L_02009730
	movs r0, #17
	adds r3, r3, r2
	str r3, [r7, #8]
	ldr r3, [r7, #16]
	movs r2, #128
	lsls r2, r2, #11
	adds r3, r3, r2
	str r3, [r7, #16]
	movs r3, #128
	lsls r3, r3, #7
	mov r9, r3
	ldr r3, [r7, #80]
	mov r2, r9
	strh r5, [r3, #18]
	strh r2, [r7, #6]
	movs r1, #6
	movs r2, #40
	bl ObjectMotion_Launch
	movs r1, #11
	movs r0, #17
	bl Object_SetModeById
	movs r0, #135
	bl Engine_AudioPlayCue
	movs r0, #14
	bl FieldEvent_WaitTicks
	movs r0, #135
	bl Engine_AudioPlayCue
	movs r0, #14
	bl FieldEvent_WaitTicks
	movs r0, #135
	bl Engine_AudioPlayCue
	movs r0, #14
	bl FieldEvent_WaitTicks
	movs r0, #135
	bl Engine_AudioPlayCue
	movs r0, #14
	bl FieldEvent_WaitTicks
	movs r0, #20
	bl FieldEvent_WaitTicks
	mov r3, r10
	strh r5, [r3]
	movs r0, #17
	str r6, [r7, #108]
	movs r1, #10
	bl Object_SetModeById
	movs r2, #236
	movs r1, #224
	lsls r2, r2, #1
	movs r0, #17
	bl ObjectMotion_SetPositionAndCommit
	movs r0, #30
	bl FieldEvent_WaitTicks
	str r5, [r7, #108]
	movs r0, #20
	movs r1, #0
	movs r2, #0
	bl Func_0200290c
	movs r0, #19
	movs r1, #0
	movs r2, #0
	bl Func_0200290c
	movs r0, #18
	movs r1, #0
	movs r2, #0
	bl Func_0200290c
	movs r2, #0
	movs r1, #0
	movs r0, #17
	bl Func_0200290c
	bl VenusLighthouseEntrance_StopSparks
	movs r0, #20
	bl FieldEvent_WaitTicks
	movs r1, #4
	movs r0, #9
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl FieldEvent_WaitTicks
	movs r0, #9
	movs r1, #0
	movs r2, #10
	bl Func_0200294c
	movs r0, #188
	movs r1, #1
	movs r2, #204
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #17
	lsls r0, r0, #17
	bl Camera_MoveWithinMapBounds
	bl Func_0200298c
	movs r0, #10
	bl FieldEvent_WaitTicks
	movs r1, #176
	movs r0, #9
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #5
	mov r1, r9
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #8
	bl ObjectMotion_ArmCallback
	movs r0, #15
	bl FieldEvent_WaitTicks
	movs r0, #9
	movs r1, #0
	movs r2, #10
	bl Func_0200294c
	movs r1, #128
	movs r0, #14
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	mov r1, r11
	movs r2, #0
	movs r0, #15
	bl ObjectMotion_ArmCallback
	movs r0, #40
	bl FieldEvent_WaitTicks
	movs r1, #128
	movs r0, #14
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #15
	bl ObjectMotion_ArmCallback
	movs r0, #10
	bl FieldEvent_WaitTicks
	movs r1, #192
	mov r2, r8
	movs r0, #14
	lsls r1, r1, #9
	bl ObjectMotion_SetSpeedParameters
	movs r1, #32
	negs r1, r1
	movs r2, #0
	movs r0, #14
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #9
	bl Object_GetById
	movs r6, #128
	lsls r6, r6, #6
	strh r6, [r0, #6]
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #9
	movs r1, #0
	movs r2, #10
	bl Func_0200294c
	movs r1, #192
	mov r2, r8
	movs r0, #9
	lsls r1, r1, #9
	bl ObjectMotion_SetSpeedParameters
	movs r0, #9
	movs r1, #0
	movs r2, #16
	bl ObjectMotion_CommitPositionAndActivate
	movs r2, #0
	movs r1, #0
	movs r0, #9
	bl ObjectMotion_ArmCallback
	movs r0, #10
	bl FieldEvent_WaitTicks
	movs r1, #2
	movs r0, #14
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl FieldEvent_WaitTicks
	movs r0, #14
	movs r1, #3
	bl ObjectMotion_SetActionVariant
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #10
	lsls r2, r2, #9
	movs r0, #14
	bl ObjectMotion_SetSpeedParameters
	movs r0, #14
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r5, #254
	adds r3, r5, #0
	ands r3, r2
	movs r1, #228
	movs r2, #212
	strb r3, [r0]
	lsls r1, r1, #1
	lsls r2, r2, #1
	movs r0, #14
	bl ObjectMotion_SetPositionAndReset
	movs r0, #1
	bl FieldEvent_WaitTicks
	movs r0, #14
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	movs r2, #1
	mov r8, r2
	mov r2, r8
	orrs r3, r2
	movs r1, #192
	strb r3, [r0]
	lsls r1, r1, #7
	movs r0, #14
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #8
	lsls r1, r1, #9
	movs r0, #14
	bl ObjectMotion_SetSpeedParameters
	movs r0, #20
	bl FieldEvent_WaitTicks
	movs r1, #3
	movs r0, #9
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl FieldEvent_WaitTicks
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #9
	movs r1, #0
	movs r2, #10
	bl Func_0200294c
	movs r1, #179
	movs r2, #178
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #9
	adds r1, #51
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r0, #9
	movs r1, #48
	movs r2, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #128
	movs r0, #9
	lsls r1, r1, #5
	movs r2, #0
	bl ObjectMotion_ArmCallback
	adds r1, r6, #0
	movs r0, #5
	movs r2, #0
	bl ObjectMotion_ArmCallback
	adds r1, r6, #0
	movs r0, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #9
	lsls r2, r2, #8
	movs r0, #15
	bl ObjectMotion_SetSpeedParameters
	movs r0, #15
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	movs r1, #228
	ands r5, r3
	movs r2, #220
	lsls r1, r1, #1
	lsls r2, r2, #1
	strb r5, [r0]
	movs r0, #15
	bl ObjectMotion_SetPositionAndReset
	movs r0, #1
	bl FieldEvent_WaitTicks
	movs r0, #15
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	mov r2, r8
	orrs r3, r2
	strb r3, [r0]
	movs r0, #10
	bl FieldEvent_WaitTicks
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #9
	bl Func_02002964
	movs r0, #128
	lsls r0, r0, #5
	movs r2, #10
	adds r0, #9
	movs r1, #0
	bl Func_0200294c
	movs r1, #2
	movs r0, #14
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl FieldEvent_WaitTicks
	movs r0, #14
	movs r1, #0
	movs r2, #10
	bl Func_0200294c
	movs r1, #16
	movs r2, #0
	movs r0, #9
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #10
	bl FieldEvent_WaitTicks
	movs r1, #8
	adds r1, #255
	movs r2, #30
	movs r0, #9
	bl Func_02002964
	movs r0, #128
	lsls r0, r0, #5
	movs r2, #10
	adds r0, #9
	movs r1, #0
	bl Func_0200294c
	movs r0, #14
	movs r1, #2
	bl ObjectMotion_SetVariantCallback
	movs r1, #2
	movs r0, #15
	bl Motion_SetVarCbAndRefresh
	movs r0, #20
	bl FieldEvent_WaitTicks
	movs r1, #128
	movs r2, #128
	movs r0, #14
	lsls r1, r1, #8
	lsls r2, r2, #7
	bl ObjectMotion_SetSpeedParameters
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #15
	bl ObjectMotion_SetSpeedParameters
	movs r0, #14
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r6, #254
	adds r3, r6, #0
	ands r3, r2
	movs r1, #244
	movs r2, #212
	strb r3, [r0]
	lsls r1, r1, #1
	lsls r2, r2, #1
	movs r0, #14
	bl ObjectMotion_SetPositionAndReset
	movs r0, #1
	bl FieldEvent_WaitTicks
	movs r0, #14
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	mov r2, r8
	orrs r3, r2
	strb r3, [r0]
	movs r0, #15
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	adds r3, r6, #0
	ands r3, r2
	movs r1, #244
	movs r2, #220
	strb r3, [r0]
	lsls r1, r1, #1
	lsls r2, r2, #1
	movs r0, #15
	bl ObjectMotion_SetPositionAndReset
	movs r0, #1
	bl FieldEvent_WaitTicks
	movs r0, #15
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	mov r2, r8
	orrs r3, r2
	movs r1, #228
	movs r2, #216
	strb r3, [r0]
	lsls r1, r1, #1
	movs r0, #9
	lsls r2, r2, #1
	b .L_02009734
	.2byte 0x0000
.L_0200972c:
	.4byte 0xfff80000
.L_02009730:
	.4byte 0xfffc0000
.L_02009734:
	bl ObjectMotion_SetPositionAndReset
	movs r2, #0
	movs r1, #0
	movs r0, #9
	bl ObjectMotion_ArmCallback
	movs r0, #10
	bl FieldEvent_WaitTicks
	movs r1, #3
	movs r0, #9
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl FieldEvent_WaitTicks
	movs r0, #128
	lsls r0, r0, #5
	movs r2, #10
	adds r0, #9
	movs r1, #0
	bl Func_0200294c
	movs r0, #14
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	movs r0, #14
	lsls r1, r1, #1
	bl Func_0200296c
	movs r0, #15
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #15
	bl Func_0200296c
	movs r0, #50
	bl FieldEvent_WaitTicks
	movs r0, #14
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	adds r3, r6, #0
	ands r3, r2
	movs r1, #252
	movs r2, #212
	strb r3, [r0]
	lsls r1, r1, #1
	lsls r2, r2, #1
	movs r0, #14
	bl ObjectMotion_SetPositionAndReset
	movs r0, #1
	bl FieldEvent_WaitTicks
	movs r0, #14
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	mov r2, r8
	orrs r3, r2
	strb r3, [r0]
	movs r1, #0
	movs r2, #0
	movs r0, #14
	bl Func_0200290c
	movs r0, #15
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	movs r1, #252
	ands r6, r3
	movs r2, #220
	lsls r1, r1, #1
	lsls r2, r2, #1
	strb r6, [r0]
	movs r0, #15
	bl ObjectMotion_SetPositionAndReset
	movs r0, #1
	bl FieldEvent_WaitTicks
	movs r0, #15
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	mov r2, r8
	orrs r2, r3
	strb r2, [r0]
	movs r1, #0
	movs r0, #15
	mov r8, r2
	movs r2, #0
	bl Func_0200290c
	movs r0, #10
	bl FieldEvent_WaitTicks
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #9
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r0, #9
	movs r1, #64
	movs r2, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r2, #0
	movs r1, #0
	movs r0, #9
	bl Func_0200290c
	movs r0, #30
	bl FieldEvent_WaitTicks
	movs r1, #1
	movs r0, #5
	bl Func_02002994
	bl Func_0200298c
	movs r0, #10
	bl FieldEvent_WaitTicks
	movs r1, #6
	adds r1, #255
	movs r2, #30
	movs r0, #8
	bl Func_02002964
	movs r2, #10
	movs r0, #8
	movs r1, #0
	bl Func_0200294c
	movs r1, #3
	movs r0, #5
	bl Motion_SetModeAndWaitAnimation
	movs r0, #30
	bl FieldEvent_WaitTicks
	movs r1, #2
	movs r0, #8
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl FieldEvent_WaitTicks
	movs r1, #0
	movs r2, #10
	movs r0, #8
	bl Func_0200294c
	movs r0, #10
	bl FieldEvent_WaitTicks
	movs r0, #8
	movs r1, #4
	movs r2, #0
	bl ObjectMotion_Launch
	movs r1, #0
	movs r2, #0
	movs r0, #8
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl FieldEvent_WaitTicks
	movs r0, #8
	movs r1, #0
	movs r2, #10
	bl Func_0200294c
	movs r1, #6
	adds r1, #255
	movs r2, #50
	movs r0, #5
	bl Func_02002964
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #5
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl FieldEvent_WaitTicks
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #5
	movs r1, #0
	movs r2, #10
	bl Func_0200294c
	movs r1, #2
	adds r1, #255
	movs r2, #30
	movs r0, #8
	bl Func_02002964
	movs r0, #8
	movs r1, #0
	movs r2, #10
	bl Func_0200294c
	movs r1, #10
	adds r1, #255
	movs r2, #80
	movs r0, #5
	bl Func_02002964
	movs r0, #10
	bl FieldEvent_WaitTicks
	movs r1, #131
	lsls r1, r1, #1
	movs r2, #40
	movs r0, #8
	bl Func_02002964
	movs r1, #0
	movs r0, #8
	bl UiText_OpenMessageAtObject
	movs r0, #5
	movs r1, #0
	bl Event_AskYesNoWithActorAnimation
	cmp r0, #0
	bne .L_0200994c
	movs r0, #20
	bl FieldEvent_WaitTicks
	movs r2, #10
	movs r0, #8
	movs r1, #0
	bl Func_0200294c
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_0200996e
.L_0200994c:
	movs r0, #40
	bl FieldEvent_WaitTicks
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #8
	adds r3, #1
	strh r3, [r2]
	movs r1, #0
	movs r2, #10
	bl Func_0200294c
.L_0200996e:
	movs r1, #3
	movs r0, #5
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl FieldEvent_WaitTicks
	movs r1, #160
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #8
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl FieldEvent_WaitTicks
	movs r1, #3
	movs r0, #8
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl FieldEvent_WaitTicks
	movs r0, #8
	movs r1, #0
	movs r2, #10
	bl Func_0200294c
	movs r2, #0
	movs r1, #0
	movs r0, #8
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl FieldEvent_WaitTicks
	movs r1, #3
	movs r0, #5
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl FieldEvent_WaitTicks
	movs r1, #3
	movs r0, #8
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl FieldEvent_WaitTicks
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #8
	ldr r1, .L_02009a28
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r0, #8
	movs r1, #2
	bl Object_SetModeById
	movs r0, #5
	bl Object_GetById
	cmp r0, #0
	beq .L_02009a00
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #8
	bl ObjectMotion_ResetAndSetPosition
.L_02009a00:
	movs r0, #8
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r2, #0
	movs r0, #8
	movs r1, #0
	bl Func_0200290c
	movs r0, #5
	movs r1, #1
	bl Camera_FollowActor
	bl Func_020028bc
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_02009a28:
	.4byte 0x00013333
	.section .text.x02009fd0,"ax",%progbits
	.global Func_02001fd0
	.thumb_func
Func_02001fd0:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r6, [r3]
	movs r0, #132
	lsls r0, r0, #4
	adds r3, r6, r0
	ldr r3, [r3]
	movs r2, #128
	lsls r2, r2, #4
	adds r2, #68
	mov r10, r3
	adds r0, #20
	adds r3, r6, r2
	ldr r1, [r3]
	adds r3, r6, r0
	movs r2, #0
	ldrsh r3, [r3, r2]
	sub sp, #4
	cmp r3, #7
	bls .L_0200a004
	b .L_0200a1fc
.L_0200a004:
	ldr r2, .L_0200a194
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
.L_0200a00c:
	.4byte .L_0200a02c
	.4byte .L_0200a040
	.4byte .L_0200a076
	.4byte .L_0200a08c
	.4byte .L_0200a0e4
	.4byte .L_0200a11e
	.4byte .L_0200a152
	.4byte .L_0200a19c
.L_0200a02c:
	movs r3, #128
	lsls r3, r3, #4
	adds r3, #86
	adds r1, r6, r3
	movs r0, #0
	ldrsh r3, [r1, r0]
	cmp r3, #0
	beq .L_0200a03e
	b .L_0200a1fc
.L_0200a03e:
	b .L_0200a106
.L_0200a040:
	movs r3, #128
	lsls r3, r3, #4
	adds r3, #92
	adds r2, r6, r3
	ldrh r3, [r2]
	movs r0, #248
	adds r3, #4
	strh r3, [r2]
	lsls r0, r0, #13
	lsls r3, r3, #16
	cmp r3, r0
	bgt .L_0200a05a
	b .L_0200a1fc
.L_0200a05a:
	movs r3, #128
	lsls r3, r3, #4
	adds r3, #84
	adds r2, r6, r3
	ldrh r3, [r2]
	movs r0, #128
	adds r3, #1
	strh r3, [r2]
	lsls r0, r0, #4
	movs r3, #255
	adds r0, #86
	lsls r3, r3, #8
	adds r2, r6, r0
	b .L_0200a14c
.L_0200a076:
	movs r2, #128
	lsls r2, r2, #4
	adds r2, #86
	adds r1, r6, r2
	movs r0, #0
	ldrsh r3, [r1, r0]
	cmp r3, #90
	beq .L_0200a088
	b .L_0200a1fc
.L_0200a088:
	subs r2, #2
	b .L_0200a10c
.L_0200a08c:
	movs r3, #128
	lsls r3, r3, #4
	adds r3, #86
	adds r5, r6, r3
	movs r0, #0
	ldrsh r1, [r5, r0]
	cmp r1, #0
	bne .L_0200a0bc
	mov r2, r10
	adds r2, #85
	movs r3, #3
	movs r0, #128
	strb r3, [r2]
	lsls r0, r0, #4
	movs r3, #192
	lsls r3, r3, #13
	adds r0, #94
	mov r2, r10
	str r3, [r2, #40]
	adds r3, r6, r0
	strb r1, [r3]
	movs r0, #134
	bl Engine_AudioPlayCue
.L_0200a0bc:
	movs r3, #128
	lsls r3, r3, #4
	adds r3, #92
	adds r2, r6, r3
	ldrh r3, [r2]
	movs r0, #158
	adds r3, #8
	strh r3, [r2]
	lsls r0, r0, #15
	lsls r3, r3, #16
	cmp r3, r0
	bgt .L_0200a0d6
	b .L_0200a1fc
.L_0200a0d6:
	movs r2, #128
	lsls r2, r2, #4
	adds r2, #84
	adds r3, r6, r2
	ldrh r2, [r3]
	adds r2, #1
	b .L_0200a1f2
.L_0200a0e4:
	movs r3, #128
	lsls r3, r3, #4
	adds r3, #86
	adds r1, r6, r3
	movs r0, #0
	ldrsh r3, [r1, r0]
	cmp r3, #10
	bne .L_0200a0fc
	movs r3, #0
	mov r2, r10
	str r3, [r2, #16]
	str r3, [r2, #8]
.L_0200a0fc:
	movs r0, #0
	ldrsh r3, [r1, r0]
	cmp r3, #30
	beq .L_0200a106
	b .L_0200a1fc
.L_0200a106:
	movs r2, #128
	lsls r2, r2, #4
	adds r2, #84
.L_0200a10c:
	adds r3, r6, r2
	ldrh r2, [r3]
	adds r2, #1
	strh r2, [r3]
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	strh r3, [r1]
	b .L_0200a1fc
.L_0200a11e:
	movs r3, #128
	lsls r3, r3, #4
	adds r3, #92
	adds r2, r6, r3
	ldrh r3, [r2]
	subs r3, #8
	strh r3, [r2]
	lsls r3, r3, #16
	cmp r3, #0
	bgt .L_0200a1fc
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #84
	adds r2, r6, r0
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	movs r3, #128
	lsls r3, r3, #4
	adds r3, #86
	adds r2, r6, r3
	movs r3, #255
	lsls r3, r3, #8
.L_0200a14c:
	adds r3, #255
	strh r3, [r2]
	b .L_0200a1fc
.L_0200a152:
	ldr r3, [r1, #24]
	cmp r3, #0
	ble .L_0200a15e
	ldr r0, .L_0200a198
	adds r3, r3, r0
	b .L_0200a160
.L_0200a15e:
	movs r3, #0
.L_0200a160:
	str r3, [r1, #24]
	str r3, [r1, #28]
	movs r2, #128
	lsls r2, r2, #4
	adds r2, #86
	adds r5, r6, r2
	movs r0, #0
	ldrsh r3, [r5, r0]
	cmp r3, #10
	bne .L_0200a17c
	movs r0, #155
	lsls r0, r0, #1
	bl Engine_AudioPlayCue
.L_0200a17c:
	movs r2, #0
	ldrsh r3, [r5, r2]
	cmp r3, #90
	bne .L_0200a1fc
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #84
	adds r3, r6, r0
	ldrh r2, [r3]
	adds r2, #1
	b .L_0200a1f2
	.2byte 0x0000
.L_0200a194:
	.4byte .L_0200a00c
.L_0200a198:
	.4byte 0xfffffae2
.L_0200a19c:
	movs r2, #128
	lsls r2, r2, #4
	adds r2, #86
	adds r5, r6, r2
	movs r0, #0
	ldrsh r3, [r5, r0]
	cmp r3, #0
	bne .L_0200a1c2
	movs r0, #144
	bl Engine_AudioPlayCue
	movs r0, #192
	movs r1, #192
	movs r2, #128
	lsls r0, r0, #10
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl Func_02002894
.L_0200a1c2:
	movs r2, #0
	ldrsh r3, [r5, r2]
	cmp r3, #30
	bne .L_0200a1fc
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	negs r0, r0
	negs r1, r1
	adds r2, #102
	bl Func_02002894
	movs r0, #0
	ldrsh r3, [r5, r0]
	cmp r3, #30
	bne .L_0200a1fc
	movs r2, #128
	lsls r2, r2, #4
	adds r2, #84
	adds r3, r6, r2
	movs r2, #186
	lsls r2, r2, #2
	adds r2, #255
.L_0200a1f2:
	strh r2, [r3]
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	strh r3, [r5]
.L_0200a1fc:
	mov r0, r10
	ldrh r3, [r0, #6]
	movs r2, #128
	lsls r2, r2, #5
	adds r3, r3, r2
	movs r2, #128
	lsls r2, r2, #4
	adds r2, #92
	strh r3, [r0, #6]
	adds r3, r6, r2
	movs r0, #0
	ldrsh r3, [r3, r0]
	cmp r3, #0
	bgt .L_0200a21a
	b .L_0200a38c
.L_0200a21a:
	movs r2, #0
	mov r8, r2
	subs r3, #8
	movs r4, #0
	cmp r8, r3
	bge .L_0200a2c2
	movs r3, #128
	movs r2, #208
	lsls r3, r3, #3
	lsls r2, r2, #3
	adds r0, r6, r3
	adds r1, r6, r2
.L_0200a232:
	movs r2, #128
	lsls r2, r2, #4
	adds r2, #72
	adds r3, r6, r2
	ldr r3, [r3]
	adds r5, r1, #0
	str r3, [r5]
	adds r2, #4
	adds r3, r6, r2
	ldr r3, [r3]
	lsls r2, r4, #16
	adds r3, r3, r2
	movs r2, #128
	lsls r2, r2, #12
	adds r3, r3, r2
	str r3, [r5, #4]
	movs r2, #133
	lsls r2, r2, #4
	adds r3, r6, r2
	ldr r3, [r3]
	adds r7, r0, #0
	str r3, [r5, #8]
	ldr r3, [r5, #24]
	movs r0, #128
	adds r3, #1
	str r3, [r5, #24]
	lsls r0, r0, #4
	adds r0, #88
	adds r1, r6, r0
	ldrh r1, [r1]
	movs r2, #3
	asrs r3, r3, #2
	ands r3, r2
	lsls r3, r3, #3
	adds r1, r1, r3
	ldr r3, .L_0200a2a8
	ldr r2, .L_0200a2ac
	ands r1, r3
	ldrh r3, [r7, #8]
	adds r0, r7, #0
	ands r3, r2
	orrs r3, r1
	strh r3, [r7, #8]
	adds r1, r5, #0
	str r4, [sp, #0]
	bl Func_02002a14
	movs r2, #1
	ldr r4, [sp, #0]
	add r8, r2
	adds r0, r7, #0
	adds r1, r5, #0
	mov r3, r8
	adds r0, #40
	adds r1, #28
	adds r4, #16
	cmp r3, #15
	bgt .L_0200a38c
	b .L_0200a2b0
.L_0200a2a8:
	.4byte 0x000003ff
.L_0200a2ac:
	.4byte 0xfffffc00
.L_0200a2b0:
	movs r2, #128
	lsls r2, r2, #4
	adds r2, #92
	adds r3, r6, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	subs r3, #8
	cmp r4, r3
	blt .L_0200a232
.L_0200a2c2:
	mov r3, r8
	cmp r3, #15
	bgt .L_0200a38c
	mov r0, r8
	lsls r3, r3, #3
	subs r3, r3, r0
	movs r0, #128
	lsls r3, r3, #2
	movs r2, #208
	lsls r0, r0, #4
	adds r3, r6, r3
	lsls r2, r2, #3
	adds r0, #72
	adds r5, r3, r2
	adds r3, r6, r0
	ldr r3, [r3]
	movs r2, #128
	str r3, [r5]
	adds r0, #20
	lsls r2, r2, #4
	adds r3, r6, r0
	adds r2, #76
	adds r1, r6, r2
	movs r0, #0
	ldrsh r2, [r3, r0]
	ldr r3, [r1]
	lsls r2, r2, #16
	adds r3, r3, r2
	str r3, [r5, #4]
	movs r2, #133
	lsls r2, r2, #4
	adds r3, r6, r2
	ldr r0, [r5, #24]
	ldr r3, [r3]
	adds r0, #1
	str r3, [r5, #8]
	str r0, [r5, #24]
	lsls r0, r0, #12
	bl Math_Sine
	ldr r3, .L_0200a380
	adds r1, r0, #0
	ldr r0, .L_0200a384
	mov lr, r3
	.2byte 0xf800
	ldr r3, [r5, #4]
	mov r2, r8
	adds r3, r3, r0
	ldr r0, .L_0200a384
	adds r3, r3, r0
	str r3, [r5, #4]
	lsls r3, r2, #2
	add r3, r8
	lsls r3, r3, #3
	movs r0, #128
	adds r3, r6, r3
	lsls r0, r0, #3
	adds r7, r3, r0
	movs r2, #128
	ldr r3, [r5, #24]
	lsls r2, r2, #4
	adds r2, #90
	adds r1, r6, r2
	asrs r3, r3, #2
	movs r2, #3
	ands r3, r2
	ldrh r2, [r1]
	lsls r3, r3, #3
	adds r2, r2, r3
	ldr r3, .L_0200a37c
	ldrh r1, [r7, #8]
	ands r2, r3
	ldr r3, .L_0200a388
	adds r0, r7, #0
	ands r3, r1
	orrs r3, r2
	strh r3, [r7, #8]
	adds r1, r5, #0
	bl Func_02002a14
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #94
	adds r3, r6, r0
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	beq .L_0200a38c
	ldr r3, [r5, #4]
	mov r2, r10
	str r3, [r2, #12]
	b .L_0200a38c
.L_0200a37c:
	.4byte 0x000003ff
.L_0200a380:
	.4byte IwramMulQ16
.L_0200a384:
	.4byte 0xfffe0000
.L_0200a388:
	.4byte 0xfffffc00
.L_0200a38c:
	movs r3, #128
	lsls r3, r3, #4
	adds r3, #86
	adds r2, r6, r3
	ldrh r3, [r2]
	add sp, #4
	adds r3, #1
	strh r3, [r2]
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.section .text.x0200a3a4,"ax",%progbits
	.global Func_020023a4
	.thumb_func
Func_020023a4:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r1, #134
	adds r5, r0, #0
	lsls r1, r1, #4
	movs r0, #220
	sub sp, #4
	bl Runtime_AllocateHeapBlockFar
	adds r7, r0, #0
	adds r0, r5, #0
	bl Object_GetById
	adds r6, r0, #0
	adds r2, r6, #0
	movs r1, #132
	movs r3, #0
	adds r2, #91
	lsls r1, r1, #4
	strb r3, [r2]
	adds r3, r7, r1
	str r6, [r3]
	movs r3, #128
	lsls r3, r3, #4
	adds r3, #72
	adds r2, r7, r3
	ldr r3, [r6, #8]
	adds r1, #12
	str r3, [r2]
	adds r2, r7, r1
	ldr r3, [r6, #12]
	movs r1, #128
	str r3, [r2]
	movs r3, #133
	lsls r3, r3, #4
	adds r2, r7, r3
	ldr r3, [r6, #16]
	lsls r1, r1, #10
	adds r3, r3, r1
	str r3, [r2]
	adds r1, r7, #0
	ldr r0, .L_0200a520
	bl Func_02002824
	bl Resource_FindFreeEntry
	movs r1, #128
	lsls r1, r1, #4
	adds r2, r7, #0
	mov r11, r0
	bl VramCache_Load
	movs r2, #128
	lsls r2, r2, #4
	mov r10, r0
	adds r2, #90
	adds r3, r7, r2
	mov r1, r10
	strh r1, [r3]
	movs r1, #128
	lsls r1, r1, #4
	adds r1, #88
	mov r2, r10
	adds r3, r7, r1
	adds r2, #32
	strh r2, [r3]
	movs r2, #208
	lsls r2, r2, #3
	movs r3, #128
	adds r2, r2, r7
	lsls r3, r3, #3
	movs r1, #15
	mov r8, r2
	adds r5, r7, r3
	mov r9, r1
.L_0200a444:
	mov r2, r10
	movs r3, #128
	str r2, [sp, #0]
	movs r1, #8
	movs r2, #8
	lsls r3, r3, #23
	adds r0, r5, #0
	bl Func_02002a0c
	adds r0, r6, #0
	bl Func_02002a2c
	subs r0, #2
	strh r0, [r5, #30]
	adds r0, r6, #0
	bl Func_02002a24
	ldrb r2, [r5, #9]
	movs r1, #13
	movs r3, #3
	negs r1, r1
	ands r0, r3
	adds r3, r1, #0
	ands r2, r3
	ldrb r3, [r5, #5]
	movs r1, #32
	orrs r3, r1
	lsls r0, r0, #2
	strb r3, [r5, #5]
	orrs r2, r0
	movs r3, #15
	ands r2, r3
	subs r3, #16
	strb r2, [r5, #9]
	add r9, r3
	mov r2, r8
	str r3, [r2, #24]
	mov r1, r9
	movs r3, #28
	adds r5, #40
	add r8, r3
	cmp r1, #0
	bge .L_0200a444
	movs r0, #145
	ldr r1, [r6, #8]
	ldr r2, [r6, #12]
	ldr r3, [r6, #16]
	lsls r0, r0, #1
	bl Func_0200286c
	movs r2, #128
	lsls r2, r2, #4
	adds r2, #68
	adds r3, r7, r2
	str r0, [r3]
	mov r8, r0
	movs r1, #1
	bl Func_0200285c
	mov r0, r8
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	mov r2, r8
	adds r2, #35
	movs r3, #2
	strb r3, [r2]
	adds r0, r6, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	ldr r2, [r6, #80]
	movs r3, #128
	lsls r3, r3, #7
	strh r3, [r2, #18]
	ldr r2, .L_0200a524
	ldr r3, [r6, #8]
	ldr r1, .L_0200a51c
	adds r3, r3, r2
	str r3, [r6, #8]
	adds r3, r6, #0
	adds r3, #85
	strb r1, [r3]
	movs r3, #128
	movs r1, #128
	lsls r3, r3, #4
	lsls r1, r1, #4
	movs r2, #128
	adds r1, #86
	adds r3, #84
	lsls r2, r2, #4
	movs r5, #0
	adds r6, r7, r3
	adds r2, #92
	adds r3, r7, r1
	strh r5, [r6]
	strh r5, [r3]
	adds r3, r7, r2
	strh r5, [r3]
	movs r3, #128
	lsls r3, r3, #4
	adds r3, #94
	adds r2, r7, r3
	movs r3, #1
	strb r3, [r2]
	movs r0, #221
	b .L_0200a528
	.2byte 0x0000
.L_0200a51c:
	.4byte 0x00000000
.L_0200a520:
	.4byte gField_Tiles64D
.L_0200a524:
	.4byte 0xfff40000
.L_0200a528:
	bl Engine_AudioPlayCue
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_0200a588
	bl Scheduler_AddOrUpdateCallback
	movs r2, #186
	movs r1, #0
	ldrsh r3, [r6, r1]
	lsls r2, r2, #2
	adds r2, #255
	cmp r3, r2
	beq .L_0200a560
.L_0200a544:
	movs r0, #1
	bl WaitTicks
	movs r1, #128
	lsls r1, r1, #4
	adds r1, #84
	adds r3, r7, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	movs r1, #186
	lsls r1, r1, #2
	adds r1, #255
	cmp r3, r1
	bne .L_0200a544
.L_0200a560:
	ldr r0, .L_0200a588
	bl Scheduler_RemoveCallbackFar
	mov r0, r8
	bl Func_02002874
	mov r0, r11
	bl Resource_ResetEntry
	movs r0, #220
	bl Runtime_ReleaseHeapBlock
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200a588:
	.4byte Func_02001fd0
	.section .text.x0200a58c,"ax",%progbits
	.global Func_0200258c
	.thumb_func
Func_0200258c:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r7, [r3]
	movs r2, #128
	lsls r2, r2, #4
	movs r3, #148
	adds r5, r7, r2
	lsls r3, r3, #5
	movs r2, #95
	adds r6, r7, r3
	mov r8, r2
.L_0200a5aa:
	ldr r3, [r5, #24]
	cmp r3, #15
	bhi .L_0200a5f4
	movs r2, #132
	lsls r2, r2, #6
	adds r2, #130
	adds r1, r7, r2
	ldrh r1, [r1]
	movs r2, #7
	asrs r3, r3, #1
	ands r3, r2
	lsls r3, r3, #3
	adds r1, r1, r3
	ldr r3, .L_0200a5ec
	ldr r2, .L_0200a5f0
	ands r1, r3
	ldrh r3, [r6, #8]
	adds r0, r6, #0
	ands r3, r2
	orrs r3, r1
	strh r3, [r6, #8]
	adds r1, r5, #0
	bl Func_02002a14
	adds r0, r5, #0
	movs r1, #64
	movs r2, #0
	bl Func_02002a1c
	ldr r3, [r5, #24]
	adds r3, #1
	str r3, [r5, #24]
	b .L_0200a5f4
.L_0200a5ec:
	.4byte 0x000003ff
.L_0200a5f0:
	.4byte 0xfffffc00
.L_0200a5f4:
	movs r3, #1
	negs r3, r3
	add r8, r3
	mov r2, r8
	adds r6, #40
	adds r5, #28
	cmp r2, #0
	bge .L_0200a5aa
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x0200a60c,"ax",%progbits
	.global Func_0200260c
	.thumb_func
Func_0200260c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #8
	str r2, [sp, #4]
	str r3, [sp, #0]
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r3, [r3]
	mov r10, r0
	mov r8, r3
	adds r7, r1, #0
	bl Random16Far
	movs r1, #134
	lsls r1, r1, #6
	add r1, r8
	movs r3, #0
	ldrsh r2, [r1, r3]
	mov r9, r1
	lsls r5, r2, #2
	adds r5, r5, r2
	lsls r3, r2, #3
	subs r3, r3, r2
	lsls r5, r5, #3
	movs r1, #148
	mov r11, r0
	lsls r1, r1, #5
	lsls r3, r3, #2
	add r5, r8
	mov r0, r10
	adds r5, r5, r1
	add r8, r3
	bl Func_02002a24
	ldrb r2, [r5, #9]
	movs r3, #3
	ands r0, r3
	movs r3, #13
	negs r3, r3
	ands r3, r2
	lsls r0, r0, #2
	orrs r3, r0
	strb r3, [r5, #9]
	mov r0, r10
	bl Func_02002a2c
	movs r6, #128
	lsls r6, r6, #4
	add r6, r8
	movs r3, #0
	strh r0, [r5, #30]
	str r3, [r6, #24]
	str r7, [r6]
	ldr r1, [sp, #4]
	mov r10, r3
	str r1, [r6, #4]
	ldr r3, [sp, #0]
	str r3, [r6, #8]
	bl Random16Far
	movs r1, #128
	lsls r1, r1, #10
	lsls r0, r0, #1
	adds r2, r6, #0
	adds r0, r0, r1
	mov r1, r11
	bl Vector_AddPolarOffsetFar
	mov r3, r10
	str r3, [r6, #12]
	bl Random16Far
	movs r1, #192
	lsls r1, r1, #8
	lsrs r0, r0, #1
	mov r3, r10
	adds r0, r0, r1
	str r3, [r6, #20]
	str r0, [r6, #16]
	bl Random16Far
	movs r1, #128
	lsls r1, r1, #4
	adds r1, #12
	add r8, r1
	mov r2, r8
	mov r1, r11
	lsrs r0, r0, #2
	bl Vector_AddPolarOffsetFar
	mov r3, r9
	ldrh r0, [r3]
	mov r1, r9
	adds r0, #1
	strh r0, [r1]
	lsls r0, r0, #16
	movs r1, #96
	asrs r0, r0, #16
	bl __modsi3
	mov r3, r9
	strh r0, [r3]
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x0200a6f4,"ax",%progbits
	.global Func_020026f4
	.thumb_func
Func_020026f4:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r1, #132
	lsls r1, r1, #6
	adds r1, #136
	movs r0, #220
	sub sp, #4
	bl Runtime_AllocateHeapBlockFar
	adds r6, r0, #0
	ldr r0, .L_0200a7a8
	bl Resource_GetTableEntry
	adds r1, r6, #0
	bl Func_02002824
	bl Resource_FindFreeEntry
	movs r1, #128
	lsls r1, r1, #4
	adds r2, r6, #0
	adds r5, r0, #0
	bl VramCache_Load
	movs r1, #132
	lsls r1, r1, #6
	adds r1, #130
	mov r10, r0
	adds r3, r6, r1
	mov r2, r10
	adds r1, #2
	strh r2, [r3]
	adds r3, r6, r1
	strh r5, [r3]
	movs r2, #128
	movs r3, #148
	lsls r2, r2, #4
	lsls r3, r3, #5
	movs r1, #95
	adds r7, r6, r2
	adds r5, r6, r3
	mov r8, r1
.L_0200a74c:
	mov r2, r10
	movs r3, #128
	str r2, [sp, #0]
	adds r0, r5, #0
	movs r1, #8
	movs r2, #8
	lsls r3, r3, #23
	bl Func_02002a0c
	ldrb r3, [r5, #5]
	movs r2, #32
	orrs r3, r2
	ldrb r2, [r5, #9]
	movs r1, #13
	strb r3, [r5, #5]
	negs r1, r1
	movs r3, #15
	ands r3, r2
	adds r2, r1, #0
	ands r3, r2
	strb r3, [r5, #9]
	movs r3, #240
	strh r3, [r5, #30]
	subs r3, #241
	add r8, r3
	mov r2, r8
	str r3, [r7, #24]
	adds r5, #40
	adds r7, #28
	cmp r2, #0
	bge .L_0200a74c
	movs r1, #134
	lsls r1, r1, #6
	adds r2, r6, r1
	movs r3, #0
	movs r1, #144
	strh r3, [r2]
	lsls r1, r1, #3
	ldr r0, .L_0200a7ac
	bl Scheduler_AddOrUpdateCallback
	add sp, #4
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_0200a7a8:
	.4byte 0x000001e1
.L_0200a7ac:
	.4byte Func_0200258c
