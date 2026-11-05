.syntax unified
	.thumb
	.section .text.x02009f10,"ax",%progbits
	.balign 4
	.global Func_02001f10
	.thumb_func
Func_02001f10:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r0, #17
	sub sp, #8
	bl Actor_GetById
	adds r7, r0, #0
	movs r0, #30
	bl Actor_GetById
	mov r9, r0
	movs r0, #31
	bl Actor_GetById
	mov r11, r0
	movs r0, #32
	bl Actor_GetById
	str r0, [sp, #4]
	bl Func_0200560c
	movs r0, #0
	bl Func_02005784
	movs r0, #17
	movs r1, #2
	bl Actor_SetModeById
	movs r2, #0
	mov r8, r2
	adds r3, r7, #0
	adds r3, #85
	mov r2, r8
	strb r2, [r3]
	ldr r3, [r7, #8]
	ldr r2, .L_0200a35c
	movs r0, #28
	adds r3, r3, r2
	str r3, [r7, #8]
	ldr r3, [r7, #16]
	movs r2, #128
	lsls r2, r2, #13
	adds r3, r3, r2
	str r3, [r7, #16]
	movs r1, #2
	bl Actor_SetModeById
	movs r0, #29
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r2, r2
	movs r3, #0
	negs r1, r1
	negs r0, r0
	bl Camera_MoveWithinMapBounds
	movs r0, #1
	bl wait
	movs r0, #30
	movs r1, #7
	bl Actor_SetModeById
	movs r0, #31
	movs r1, #7
	bl Actor_SetModeById
	movs r0, #32
	movs r1, #6
	bl Actor_SetModeById
	ldr r3, .L_0200a360
	movs r2, #133
	mov r10, r3
	lsls r2, r2, #2
	add r10, r2
	mov r3, r10
	ldr r0, [r3]
	movs r1, #252
	movs r3, #192
	movs r2, #160
	lsls r3, r3, #8
	lsls r2, r2, #17
	lsls r1, r1, #17
	bl Func_02005684
	movs r0, #1
	bl wait
	movs r0, #0
	movs r1, #0
	bl Func_02005764
	movs r1, #0
	movs r0, #0
	bl Func_0200575c
	movs r0, #1
	bl Func_0200576c
	movs r0, #30
	bl Actor_GetById
	movs r1, #15
	bl Actor_SetPartAttribute
	movs r0, #31
	bl Actor_GetById
	movs r1, #15
	bl Actor_SetPartAttribute
	movs r0, #32
	bl Actor_GetById
	movs r1, #15
	bl Actor_SetPartAttribute
	movs r0, #1
	bl wait
	movs r0, #21
	movs r1, #1
	bl Actor_SetSpritePriority
	movs r0, #20
	movs r1, #1
	bl Actor_SetSpritePriority
	movs r0, #19
	movs r1, #1
	bl Actor_SetSpritePriority
	movs r0, #26
	movs r1, #1
	bl Actor_SetSpritePriority
	movs r0, #25
	movs r1, #1
	bl Actor_SetSpritePriority
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #108]
	movs r3, #214
	lsls r3, r3, #1
	adds r2, r1, r3
	adds r3, #88
	str r3, [r2]
	subs r3, #80
	adds r2, r1, r3
	movs r3, #1
	str r3, [r2]
	bl Event_StartScreenTransition
	bl Event_WaitTransitionFrames
	movs r0, #195
	lsls r0, r0, #1
	bl Func_020057dc
	movs r0, #128
	lsls r0, r0, #9
	adds r0, #3
	movs r1, #1
	bl Func_0200575c
	movs r0, #128
	movs r1, #2
	lsls r0, r0, #9
	bl Func_0200575c
	movs r0, #16
	bl Func_0200576c
	movs r0, #16
	bl wait
	movs r0, #80
	bl FieldEvent_Wait
	ldr r3, .L_0200a364
	ldr r6, .L_0200a368
	mov r2, r8
	str r2, [r3]
	movs r3, #4
	str r3, [r6]
	movs r0, #17
	bl Actor_GetById
	ldr r5, .L_0200a36c
	str r5, [r0, #108]
	movs r0, #27
	bl Actor_GetById
	str r5, [r0, #108]
	movs r0, #28
	bl Actor_GetById
	str r5, [r0, #108]
	movs r0, #29
	bl Actor_GetById
	ldr r3, .L_0200a370
	mov r2, r9
	str r5, [r0, #108]
	str r3, [r2, #108]
	mov r2, r11
	str r3, [r2, #108]
	ldr r2, [sp, #4]
	movs r0, #170
	lsls r0, r0, #1
	str r3, [r2, #108]
	adds r0, #255
	bl Func_020057dc
	movs r0, #10
	bl wait
	movs r0, #170
	lsls r0, r0, #1
	adds r0, #255
	bl Func_020057dc
	movs r0, #10
	bl wait
	movs r0, #170
	lsls r0, r0, #1
	adds r0, #255
	bl Func_020057dc
	movs r0, #10
	bl wait
	movs r0, #170
	lsls r0, r0, #1
	adds r0, #255
	bl Func_020057dc
	movs r0, #10
	bl wait
	movs r0, #170
	lsls r0, r0, #1
	adds r0, #255
	bl Func_020057dc
	movs r0, #40
	bl wait
	movs r3, #3
	str r3, [r6]
	movs r0, #30
	bl wait
	movs r3, #2
	str r3, [r6]
	movs r0, #20
	bl wait
	movs r0, #17
	movs r1, #0
	movs r2, #0
	bl Func_0200567c
	movs r0, #27
	movs r1, #0
	movs r2, #0
	bl Func_0200567c
	movs r0, #28
	movs r1, #0
	movs r2, #0
	bl Func_0200567c
	movs r1, #0
	movs r2, #0
	movs r0, #29
	bl Func_0200567c
	movs r0, #17
	bl Actor_GetById
	mov r3, r8
	str r3, [r0, #108]
	movs r0, #27
	bl Actor_GetById
	mov r2, r8
	str r2, [r0, #108]
	movs r0, #28
	bl Actor_GetById
	mov r3, r8
	str r3, [r0, #108]
	movs r0, #29
	bl Actor_GetById
	mov r2, r8
	mov r3, r9
	str r2, [r0, #108]
	str r2, [r3, #108]
	mov r3, r11
	str r2, [r3, #108]
	ldr r3, [sp, #4]
	movs r0, #1
	str r2, [r3, #108]
	bl wait
	movs r0, #103
	bl Func_020057dc
	movs r0, #30
	bl Actor_GetById
	movs r1, #9
	bl Actor_SetPartAttribute
	movs r0, #31
	bl Actor_GetById
	movs r1, #9
	bl Actor_SetPartAttribute
	movs r0, #32
	bl Actor_GetById
	movs r1, #9
	bl Actor_SetPartAttribute
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #30
	bl Actor_GetById
	movs r1, #0
	bl Actor_SetPartAttribute
	movs r0, #31
	bl Actor_GetById
	movs r1, #0
	bl Actor_SetPartAttribute
	movs r0, #32
	bl Actor_GetById
	movs r1, #0
	bl Actor_SetPartAttribute
	movs r0, #20
	bl FieldEvent_Wait
	bl Random16Far
	lsls r3, r0, #2
	adds r3, r3, r0
	lsls r3, r3, #4
	lsrs r3, r3, #16
	mov r2, r9
	adds r3, #40
	adds r2, #98
	strb r3, [r2]
	bl Random16Far
	lsls r3, r0, #2
	adds r3, r3, r0
	lsls r3, r3, #4
	lsrs r3, r3, #16
	mov r2, r11
	adds r3, #40
	adds r2, #98
	strb r3, [r2]
	bl Random16Far
	lsls r3, r0, #2
	ldr r2, [sp, #4]
	adds r3, r3, r0
	lsls r3, r3, #4
	lsrs r3, r3, #16
	adds r3, #40
	adds r2, #98
	strb r3, [r2]
	ldr r3, .L_0200a374
	mov r2, r9
	str r3, [r2, #108]
	mov r2, r11
	str r3, [r2, #108]
	ldr r2, [sp, #4]
	movs r0, #20
	str r3, [r2, #108]
	bl FieldEvent_Wait
	movs r0, #128
	movs r1, #1
	lsls r0, r0, #9
	bl Func_0200575c
	movs r0, #40
	bl Func_0200576c
	movs r0, #80
	bl wait
	ldr r0, .L_0200a378
	bl Func_020056dc
	movs r1, #128
	movs r0, #26
	lsls r1, r1, #8
	bl Func_02005704
	movs r0, #26
	movs r1, #0
	bl Func_020056f4
	movs r0, #19
	movs r1, #0
	bl Func_020056f4
	movs r1, #128
	movs r0, #22
	lsls r1, r1, #6
	movs r2, #0
	bl Actor_ArmCallback
	mov r3, r10
	movs r1, #128
	ldr r0, [r3]
	lsls r1, r1, #6
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #128
	movs r0, #23
	lsls r1, r1, #7
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #128
	movs r0, #24
	lsls r1, r1, #7
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #128
	movs r0, #21
	lsls r1, r1, #6
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #128
	movs r0, #20
	lsls r1, r1, #6
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #128
	movs r0, #26
	lsls r1, r1, #7
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #128
	movs r2, #0
	movs r0, #25
	lsls r1, r1, #7
	bl Actor_ArmCallback
	movs r1, #204
	lsls r1, r1, #6
	adds r1, #51
	ldr r0, .L_0200a37c
	bl Func_02005724
	bl Func_0200573c
	mov r2, r8
	adds r0, #85
	strb r2, [r0]
	movs r1, #160
	movs r0, #128
	movs r2, #170
	movs r3, #1
	lsls r0, r0, #18
	lsls r1, r1, #15
	lsls r2, r2, #17
	bl Camera_MoveWithinMapBounds
	bl Func_02005734
	movs r1, #10
	movs r2, #20
	adds r1, #255
	movs r0, #19
	bl Func_02005714
	movs r1, #0
	movs r0, #19
	bl Func_020056f4
	movs r0, #8
	bl Func_020057dc
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #24
	bl Func_0200571c
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #24
	movs r1, #0
	bl Func_020056f4
	movs r0, #25
	movs r1, #0
	bl Func_020056f4
	movs r0, #19
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #19
	movs r1, #0
	bl Func_020056f4
	movs r1, #129
	movs r0, #26
	lsls r1, r1, #1
	bl Func_0200571c
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #26
	movs r1, #0
	b .L_0200a380
	.2byte 0x0000
.L_0200a35c:
	.4byte 0xfffc0000
.L_0200a360:
	.4byte gMainRoutineWork
.L_0200a364:
	.4byte gData_02006b64
.L_0200a368:
	.4byte gData_02006b6c
.L_0200a36c:
	.4byte Func_02001eb0
.L_0200a370:
	.4byte Func_02001ee4
.L_0200a374:
	.4byte Func_0200008c
.L_0200a378:
	.4byte MsgWhoAreThey
.L_0200a37c:
	.4byte 0x00019999
.L_0200a380:
	bl Func_020056f4
	movs r0, #19
	movs r1, #4
	bl Actor_SetModeById
	movs r0, #19
	movs r1, #0
	bl Func_020056f4
	movs r1, #4
	adds r1, #255
	movs r2, #0
	movs r0, #23
	bl Func_02005714
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #23
	ldr r1, .L_0200a7a0
	adds r2, #204
	bl Actor_SetSpeedParameters
	movs r1, #128
	movs r2, #164
	lsls r2, r2, #1
	movs r0, #23
	lsls r1, r1, #2
	bl Actor_SetPositionAndReset
	movs r1, #128
	movs r0, #23
	lsls r1, r1, #6
	bl Func_02005704
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #23
	movs r1, #0
	bl Func_020056f4
	movs r1, #128
	movs r0, #20
	lsls r1, r1, #8
	bl Func_02005704
	movs r0, #160
	lsls r0, r0, #8
	adds r0, #20
	movs r1, #0
	bl Func_020056f4
	movs r1, #128
	movs r2, #20
	lsls r1, r1, #1
	movs r0, #19
	bl Func_02005714
	movs r1, #2
	movs r0, #21
	bl Actor_SetSpritePriority
	movs r0, #20
	bl Actor_GetById
	mov r3, r8
	adds r0, #98
	strb r3, [r0]
	movs r0, #21
	bl Actor_GetById
	mov r2, r8
	adds r0, #98
	strb r2, [r0]
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #21
	adds r1, #204
	adds r2, #102
	bl Actor_SetSpeedParameters
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	adds r2, #102
	movs r0, #20
	adds r1, #204
	bl Actor_SetSpeedParameters
	ldr r1, .L_0200a7a4
	movs r0, #21
	bl Actor_SetMovementTypeById
	ldr r1, .L_0200a7a8
	movs r0, #20
	bl Actor_SetMovementTypeById
	movs r0, #19
	ldr r1, .L_0200a7ac
	ldr r2, .L_0200a7b0
	bl Actor_SetSpeedParameters
	movs r1, #135
	movs r2, #170
	movs r0, #19
	lsls r1, r1, #2
	lsls r2, r2, #1
	bl Actor_SetPositionAndReset
	movs r2, #10
	movs r0, #19
	movs r1, #4
	bl Actor_Jump
	movs r0, #19
	movs r1, #0
	bl Func_020056f4
	movs r0, #21
	bl Actor_GetById
	adds r7, r0, #0
.L_0200a47a:
	movs r0, #1
	bl wait
	adds r3, r7, #0
	adds r3, #98
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_0200a47a
	movs r1, #128
	movs r2, #0
	movs r0, #20
	lsls r1, r1, #6
	bl Actor_ArmCallback
	movs r1, #128
	movs r0, #21
	lsls r1, r1, #6
	bl Func_02005704
	movs r0, #21
	movs r1, #0
	bl Func_020056f4
	movs r0, #20
	movs r1, #2
	bl Actor_SetSpritePriority
	movs r0, #22
	movs r1, #2
	bl Actor_SetSpritePriority
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #20
	bl Func_02005714
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #20
	ldr r1, .L_0200a7a0
	adds r2, #204
	bl Actor_SetSpeedParameters
	movs r1, #245
	movs r2, #148
	movs r0, #20
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl Actor_SetPositionAndReset
	movs r1, #224
	movs r0, #21
	lsls r1, r1, #8
	movs r2, #20
	bl Actor_ArmCallback
	movs r1, #160
	movs r0, #20
	lsls r1, r1, #8
	movs r2, #20
	bl Actor_ArmCallback
	movs r1, #224
	movs r0, #20
	lsls r1, r1, #8
	movs r2, #20
	bl Actor_ArmCallback
	movs r1, #204
	movs r2, #200
	lsls r1, r1, #6
	lsls r2, r2, #5
	adds r1, #51
	adds r2, #153
	movs r0, #20
	bl Actor_SetSpeedParameters
	movs r0, #20
	bl Actor_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	mov r8, r3
	ands r3, r2
	movs r2, #0
	mov r10, r2
	movs r1, #244
	movs r2, #150
	strb r3, [r0]
	lsls r1, r1, #1
	lsls r2, r2, #1
	movs r0, #20
	bl Actor_SetPositionAndReset
	movs r0, #1
	bl FieldEvent_Wait
	movs r0, #20
	bl Actor_GetById
	adds r0, #90
	ldrb r3, [r0]
	movs r2, #1
	mov r11, r2
	mov r2, r11
	orrs r3, r2
	strb r3, [r0]
	movs r0, #20
	bl FieldEvent_Wait
	movs r1, #2
	movs r0, #20
	bl Motion_SetVarCbAndRefresh
	movs r0, #20
	bl Actor_GetById
	adds r0, #90
	ldrb r2, [r0]
	mov r3, r8
	ands r3, r2
	movs r1, #243
	movs r2, #154
	strb r3, [r0]
	lsls r1, r1, #1
	lsls r2, r2, #1
	movs r0, #20
	bl Actor_SetPositionAndReset
	movs r0, #1
	bl FieldEvent_Wait
	movs r0, #20
	bl Actor_GetById
	adds r0, #90
	ldrb r3, [r0]
	mov r2, r11
	orrs r3, r2
	strb r3, [r0]
	movs r1, #200
	movs r0, #204
	lsls r0, r0, #8
	lsls r1, r1, #5
	adds r0, #204
	adds r1, #153
	bl Func_02005724
	movs r0, #128
	movs r1, #192
	movs r2, #160
	movs r3, #1
	lsls r0, r0, #18
	lsls r1, r1, #15
	lsls r2, r2, #17
	bl Camera_MoveWithinMapBounds
	bl Func_02005734
	movs r0, #21
	movs r1, #0
	movs r2, #40
	bl Actor_ArmCallback
	movs r1, #224
	movs r2, #20
	movs r0, #21
	lsls r1, r1, #8
	bl Actor_ArmCallback
	movs r0, #21
	movs r1, #0
	bl Func_02005704
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #21
	movs r1, #0
	bl Func_020056f4
	movs r1, #192
	movs r0, #22
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	ldr r6, .L_0200a7b4
	movs r3, #133
	lsls r3, r3, #2
	adds r6, r6, r3
	movs r1, #160
	ldr r0, [r6]
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #160
	movs r0, #23
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #160
	movs r0, #24
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #160
	movs r0, #26
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #160
	movs r2, #0
	movs r0, #25
	lsls r1, r1, #8
	bl Actor_ArmCallback
	movs r0, #128
	lsls r0, r0, #8
	movs r1, #0
	adds r0, #20
	bl Func_020056f4
	movs r0, #32
	bl Actor_GetById
	movs r5, #128
	mov r2, r10
	adds r7, r0, #0
	lsls r5, r5, #9
	str r2, [r7, #108]
	movs r0, #1
	bl wait
	str r5, [r7, #24]
	str r5, [r7, #28]
	movs r0, #32
	movs r1, #2
	bl Motion_SetVarCbAndRefresh
	movs r0, #32
	movs r1, #0
	bl Func_020056f4
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #23
	bl Func_02005714
	movs r1, #224
	movs r0, #23
	lsls r1, r1, #8
	movs r2, #40
	bl Actor_ArmCallback
	movs r1, #128
	movs r0, #25
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #2
	movs r2, #20
	adds r1, #255
	movs r0, #25
	bl Func_02005714
	movs r0, #25
	movs r1, #0
	bl Func_020056f4
	movs r2, #40
	movs r0, #23
	movs r1, #0
	bl Actor_ArmCallback
	movs r1, #192
	movs r0, #23
	lsls r1, r1, #8
	bl Func_02005704
	movs r0, #32
	movs r1, #3
	bl Actor_StartRepeatedMotion
	movs r0, #32
	movs r1, #0
	bl Func_020056f4
	movs r1, #192
	movs r2, #40
	movs r0, #24
	lsls r1, r1, #8
	bl Actor_ArmCallback
	movs r1, #192
	movs r0, #24
	lsls r1, r1, #7
	bl Func_02005704
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #24
	movs r1, #0
	bl Func_020056f4
	movs r1, #128
	movs r0, #21
	lsls r1, r1, #6
	bl Func_02005704
	movs r1, #129
	movs r0, #21
	lsls r1, r1, #1
	bl Func_0200571c
	movs r0, #21
	movs r1, #0
	bl Func_020056f4
	movs r1, #192
	ldr r0, [r6]
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #192
	movs r0, #24
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #192
	movs r0, #26
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #25
	bl Actor_ArmCallback
	movs r0, #30
	bl Actor_GetById
	mov r3, r10
	adds r7, r0, #0
	str r3, [r7, #108]
	movs r0, #1
	bl wait
	str r5, [r7, #24]
	str r5, [r7, #28]
	movs r0, #31
	bl Actor_GetById
	mov r2, r10
	adds r7, r0, #0
	str r2, [r7, #108]
	movs r0, #1
	bl wait
	str r5, [r7, #24]
	str r5, [r7, #28]
	movs r0, #10
	bl FieldEvent_Wait
	movs r0, #31
	movs r1, #1
	bl Motion_SetVarCbAndRefresh
	movs r0, #249
	lsls r0, r0, #5
	adds r0, #255
	movs r1, #0
	bl Func_020056f4
	movs r0, #30
	movs r1, #2
	bl Motion_SetVarCbAndRefresh
	movs r0, #30
	movs r1, #0
	bl Func_020056f4
	movs r0, #20
	movs r1, #2
	movs r2, #10
	bl Actor_Jump
	movs r2, #10
	movs r0, #20
	movs r1, #4
	b .L_0200a7b8
	.2byte 0x0000
.L_0200a7a0:
	.4byte 0x00019999
.L_0200a7a4:
	.4byte gData_02005f08
.L_0200a7a8:
	.4byte gData_02005f50
.L_0200a7ac:
	.4byte 0x00026666
.L_0200a7b0:
	.4byte 0x00013333
.L_0200a7b4:
	.4byte gMainRoutineWork
.L_0200a7b8:
	bl Actor_Jump
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #20
	movs r1, #0
	bl Func_020056f4
	movs r0, #20
	ldr r1, .L_0200a964
	ldr r2, .L_0200a968
	bl Actor_SetSpeedParameters
	ldr r0, [r6]
	ldr r1, .L_0200a964
	ldr r2, .L_0200a968
	bl Actor_SetSpeedParameters
	movs r0, #23
	ldr r1, .L_0200a964
	ldr r2, .L_0200a968
	bl Actor_SetSpeedParameters
	ldr r2, .L_0200a968
	movs r0, #24
	ldr r1, .L_0200a964
	bl Actor_SetSpeedParameters
	ldr r1, .L_0200a96c
	movs r0, #20
	bl Actor_SetMovementTypeById
	ldr r0, [r6]
	ldr r1, .L_0200a970
	bl Actor_SetMovementTypeById
	ldr r1, .L_0200a974
	movs r0, #23
	bl Actor_SetMovementTypeById
	ldr r1, .L_0200a978
	movs r0, #24
	bl Actor_SetMovementTypeAndWaitById
	movs r0, #20
	bl FieldEvent_Wait
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #22
	ldr r1, .L_0200a97c
	adds r2, #204
	bl Actor_SetSpeedParameters
	movs r1, #132
	movs r2, #160
	movs r0, #22
	lsls r1, r1, #2
	lsls r2, r2, #1
	bl Actor_SetPositionAndReset
	movs r1, #128
	movs r2, #0
	movs r0, #22
	lsls r1, r1, #6
	bl Actor_ArmCallback
	movs r1, #160
	movs r0, #25
	lsls r1, r1, #8
	bl Func_02005704
	movs r0, #160
	lsls r0, r0, #7
	adds r0, #22
	movs r1, #0
	bl Func_020056f4
	movs r2, #204
	lsls r2, r2, #8
	adds r2, #204
	movs r0, #25
	ldr r1, .L_0200a97c
	bl Actor_SetSpeedParameters
	ldr r1, .L_0200a980
	movs r0, #25
	bl Actor_SetMovementTypeById
	movs r1, #243
	movs r2, #154
	lsls r2, r2, #1
	movs r0, #22
	lsls r1, r1, #1
	bl Actor_SetPositionAndReset
	movs r1, #160
	movs r0, #22
	lsls r1, r1, #8
	bl Func_02005704
	ldr r1, .L_0200a964
	ldr r2, .L_0200a968
	movs r0, #21
	bl Actor_SetSpeedParameters
	movs r0, #21
	bl Actor_GetById
	adds r0, #90
	ldrb r3, [r0]
	mov r2, r8
	ands r2, r3
	strb r2, [r0]
	mov r8, r2
	movs r1, #237
	movs r2, #158
	lsls r1, r1, #1
	lsls r2, r2, #1
	movs r0, #21
	bl Actor_SetPositionAndReset
	movs r0, #1
	bl FieldEvent_Wait
	movs r0, #21
	bl Actor_GetById
	adds r0, #90
	ldrb r3, [r0]
	mov r2, r11
	orrs r2, r3
	movs r1, #224
	strb r2, [r0]
	lsls r1, r1, #8
	movs r0, #21
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #231
	movs r2, #140
	lsls r2, r2, #1
	movs r0, #22
	lsls r1, r1, #1
	bl Actor_SetPositionAndReset
	movs r0, #22
	movs r1, #0
	bl Func_02005704
	movs r1, #1
	movs r0, #22
	bl Actor_SetSpritePriority
	movs r0, #22
	bl Actor_GetById
	ldr r1, .L_0200a984
	bl Func_020053e4
	movs r0, #25
	bl Actor_GetById
	ldr r1, .L_0200a988
	bl Func_020053e4
	movs r0, #40
	bl FieldEvent_Wait
	movs r0, #19
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r1, #0
	movs r0, #19
	bl Func_020056f4
	movs r0, #78
	bl Func_020057dc
	movs r1, #0
	movs r0, #0
	bl Func_0200575c
	movs r0, #120
	bl Func_0200576c
	movs r0, #120
	bl wait
	movs r0, #22
	bl Actor_GetById
	bl Func_020054e4
	movs r0, #25
	bl Actor_GetById
	bl Func_020054e4
	movs r0, #80
	bl FieldEvent_Wait
	movs r0, #2
	bl Func_02005744
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200a964:
	.4byte 0x00026666
.L_0200a968:
	.4byte 0x00013333
.L_0200a96c:
	.4byte gData_02005f98
.L_0200a970:
	.4byte gData_02005fdc
.L_0200a974:
	.4byte gData_02006004
.L_0200a978:
	.4byte gData_0200602c
.L_0200a97c:
	.4byte 0x00019999
.L_0200a980:
	.4byte gData_02006070
.L_0200a984:
	.4byte gData_02006c20
.L_0200a988:
	.4byte gData_02006b70
	.section .text.x0200cd50,"ax",%progbits
	.ifdef TLA_EDITION_JA
	.else
	.balign 4
	.global Func_02004d50
	.thumb_func
Func_02004d50:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #60]
	movs r4, #128
	ldr r3, .L_0200cd98
	lsls r4, r4, #2
	adds r4, #106
	adds r2, r3, r4
	movs r3, #0
	ldrsh r6, [r2, r3]
	ldrh r4, [r2]
	cmp r6, r0
	bne .L_0200cd9c
	movs r0, #152
	lsls r0, r0, #5
	adds r0, #138
	adds r3, r1, r0
	movs r5, #0
	strb r5, [r3]
	ldr r3, .L_0200cd94
	adds r0, r6, #0
	orrs r3, r4
	strh r3, [r2]
	bl Actor_GetById
	str r5, [r0, #108]
	adds r0, r6, #0
	bl Actor_GetById
	movs r1, #0
	bl Actor_SetPartAttribute
	b .L_0200cd9c
.L_0200cd94:
	.4byte 0x0000ffff
.L_0200cd98:
	.4byte gMainRoutineWork
.L_0200cd9c:
	pop {r5, r6, pc}
	.2byte 0x0000
	.endif
	.section .bss,"aw",%nobits
	.balign 16
	.global Data_0200eb60
Data_0200eb60:
	.space 0x00000004
	.global gData_02006b64
gData_02006b64:
	.space 0x00000004
	.global Data_02006b68
Data_02006b68:
	.space 0x00000004
	.global gData_02006b6c
gData_02006b6c:
	.space 0x00000004
	.global gData_02006b70
gData_02006b70:
	.space 0x000000b0
	.global gData_02006c20
gData_02006c20:
	.space 0x000000ac
	.global Data_02006ccc
Data_02006ccc:
	.space 0x00000004
	.global gData_02006cd0
gData_02006cd0:
	.space 0x00000004
	.global Data_02006cd4
Data_02006cd4:
	.space 0x00000004
	.global gData_02006cd8
gData_02006cd8:
	.space 0x00000004
	.global gData_02006cdc
gData_02006cdc:
	.space 0x00000004
	.global gData_02006ce0
gData_02006ce0:
	.space 0x00000004
	.global Data_02006ce4
Data_02006ce4:
	.space 0x00000004
	.global Data_02006ce8
Data_02006ce8:
	.space 0x00000004
	.global gData_02006cec
gData_02006cec:
	.space 0x00000004
	.global gData_02006cf0
gData_02006cf0:
