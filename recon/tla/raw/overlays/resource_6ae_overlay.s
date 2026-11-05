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
	.section .text.x0200b658,"ax",%progbits
	.balign 4
	.global Func_02003658
	.thumb_func
Func_02003658:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r0, #1
	movs r1, #1
	movs r2, #1
	movs r3, #0
	negs r1, r1
	negs r2, r2
	negs r0, r0
	sub sp, #8
	bl Camera_MoveWithinMapBounds
	movs r0, #1
	bl wait
	movs r0, #28
	movs r1, #0
	movs r2, #0
	bl Func_0200567c
	movs r0, #29
	movs r1, #0
	movs r2, #0
	bl Func_0200567c
	movs r2, #0
	movs r0, #30
	movs r1, #0
	bl Func_0200567c
	movs r1, #204
	lsls r1, r1, #6
	adds r1, #51
	ldr r0, .L_0200b718
	bl Func_02005724
	bl Func_0200573c
	movs r1, #0
	mov r10, r1
	adds r0, #85
	mov r2, r10
	strb r2, [r0]
	movs r1, #192
	movs r0, #128
	movs r2, #156
	lsls r1, r1, #15
	lsls r2, r2, #17
	movs r3, #1
	lsls r0, r0, #18
	bl Camera_MoveWithinMapBounds
	bl Func_02005734
	movs r0, #1
	bl wait
	ldr r3, .L_0200b710
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	ldr r3, .L_0200b714
	subs r2, #2
	strh r3, [r2]
	movs r1, #10
	movs r3, #23
	str r3, [sp, #0]
	str r1, [sp, #4]
	movs r2, #18
	movs r1, #40
	movs r0, #18
	mov r11, r3
	movs r3, #16
	bl Func_020055ac
	movs r0, #7
	bl Func_02003648
	movs r0, #5
	bl Func_02003648
	movs r0, #6
	bl Func_02003648
	movs r0, #19
	b .L_0200b71c
.L_0200b710:
	.4byte 0x00000c08
.L_0200b714:
	.4byte 0x00003f10
.L_0200b718:
	.4byte 0x00019999
.L_0200b71c:
	bl Func_02003648
	movs r0, #0
	bl Func_02003648
	movs r0, #3
	bl Func_02003648
	movs r0, #2
	bl Func_02003648
	movs r0, #1
	bl Func_02003648
	movs r0, #0
	movs r1, #0
	bl Event_PrepareObjectAndApplyValue
	ldr r3, .L_0200bb38
	movs r2, #133
	lsls r2, r2, #2
	adds r2, r2, r3
	mov r9, r2
	ldr r0, [r2]
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	adds r1, #204
	adds r2, #102
	bl Actor_SetSpeedParameters
	mov r3, r9
	movs r1, #128
	movs r2, #140
	ldr r0, [r3]
	lsls r2, r2, #1
	lsls r1, r1, #2
	bl Actor_SetPositionAndReset
	mov r1, r9
	ldr r0, [r1]
	movs r1, #192
	lsls r1, r1, #8
	bl Func_02005704
	mov r2, r9
	ldr r0, [r2]
	movs r1, #2
	bl Motion_SetVarCbAndRefresh
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #222
	bl PartyInventory_Remove
	movs r0, #4
	bl Actor_GetById
	ldr r2, [r0, #12]
	movs r3, #128
	lsls r3, r3, #12
	ldr r1, [r0, #8]
	adds r2, r2, r3
	ldr r3, [r0, #16]
	movs r0, #234
	adds r0, #255
	bl Func_02005574
	adds r7, r0, #0
	cmp r7, #0
	beq .L_0200b8ac
	ldr r6, [r7, #80]
	movs r3, #33
	ldrb r2, [r6, #5]
	negs r3, r3
	ands r3, r2
	ldrb r2, [r6, #9]
	strb r3, [r6, #5]
	movs r3, #15
	mov r1, r10
	ands r3, r2
	movs r2, #13
	strb r1, [r6, #27]
	negs r2, r2
	adds r1, r7, #0
	adds r1, #35
	ands r3, r2
	movs r2, #8
	orrs r3, r2
	ldrb r2, [r1]
	strb r3, [r6, #9]
	movs r3, #254
	ands r3, r2
	movs r2, #85
	strb r3, [r1]
	adds r2, r2, r7
	mov r3, r10
	strb r3, [r2]
	mov r8, r2
	adds r2, r7, #0
	adds r2, #92
	movs r3, #1
	strb r3, [r2]
	movs r2, #2
	ldrb r3, [r1]
	orrs r3, r2
	strb r3, [r1]
	movs r1, #0
	bl Actor_SetSingleChildFlags
	movs r3, #204
	lsls r3, r3, #7
	adds r3, #102
	str r3, [r7, #48]
	movs r3, #192
	lsls r3, r3, #4
	adds r3, #204
	str r3, [r7, #52]
	movs r3, #128
	lsls r3, r3, #8
	movs r1, #193
	str r3, [r7, #72]
	lsls r1, r1, #3
	movs r0, #68
	bl HeapSlot_AllocFar
	adds r5, r0, #0
	movs r0, #247
	bl Func_020055ec
	movs r1, #128
	lsls r1, r1, #3
	adds r5, r5, r1
	adds r2, r5, #0
	movs r1, #128
	ldrb r0, [r6, #16]
	bl VramCache_Load
	movs r0, #68
	bl HeapSlot_Release
	mov r2, r10
	mov r3, r8
	strb r2, [r3]
	movs r1, #128
	movs r2, #128
	movs r3, #140
	lsls r2, r2, #16
	lsls r1, r1, #18
	lsls r3, r3, #17
	adds r0, r7, #0
	bl Func_02005594
	adds r0, r7, #0
	bl Func_0200559c
	movs r3, #4
	mov r1, r8
	strb r3, [r1]
	movs r0, #60
	bl FieldEvent_Wait
	mov r2, r10
	mov r3, r8
	strb r2, [r3]
	movs r1, #128
	movs r2, #128
	movs r3, #132
	lsls r1, r1, #18
	lsls r2, r2, #16
	lsls r3, r3, #17
	adds r0, r7, #0
	bl Func_02005594
	adds r0, r7, #0
	bl Func_0200559c
	movs r0, #20
	bl FieldEvent_Wait
	movs r1, #128
	movs r2, #160
	movs r3, #252
	lsls r1, r1, #18
	lsls r2, r2, #15
	lsls r3, r3, #16
	adds r0, r7, #0
	bl Func_02005594
	adds r0, r7, #0
	bl Func_0200559c
	movs r0, #20
	bl FieldEvent_Wait
	adds r0, r7, #0
	bl Func_0200557c
.L_0200b8ac:
	movs r0, #40
	bl FieldEvent_Wait
	movs r0, #23
	movs r1, #2
	bl Actor_SetSpritePriority
	movs r0, #24
	movs r1, #2
	bl Actor_SetSpritePriority
	movs r0, #25
	movs r1, #2
	bl Actor_SetSpritePriority
	movs r0, #26
	movs r1, #2
	bl Actor_SetSpritePriority
	movs r1, #2
	movs r0, #27
	bl Actor_SetSpritePriority
	movs r0, #141
	bl Func_020057dc
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #9
	lsls r1, r1, #9
	lsls r0, r0, #9
	bl Func_020055c4
	movs r0, #23
	bl Actor_GetById
	movs r1, #7
	bl Actor_SetPartAttribute
	movs r0, #23
	bl Actor_GetById
	ldr r6, .L_0200bb3c
	movs r5, #200
	adds r3, r0, #0
	lsls r5, r5, #5
	adds r5, #153
	adds r3, #85
	mov r1, r10
	str r5, [r0, #24]
	str r6, [r0, #28]
	movs r2, #128
	strb r1, [r3]
	movs r1, #128
	movs r3, #204
	lsls r2, r2, #15
	lsls r3, r3, #16
	lsls r1, r1, #18
	bl Actor_SetPositionAndResetMotion
	movs r0, #24
	bl Actor_GetById
	movs r1, #7
	bl Actor_SetPartAttribute
	movs r0, #24
	bl Actor_GetById
	adds r3, r0, #0
	adds r3, #85
	mov r2, r10
	str r5, [r0, #24]
	str r6, [r0, #28]
	movs r1, #128
	strb r2, [r3]
	movs r2, #192
	movs r3, #204
	lsls r2, r2, #15
	lsls r1, r1, #18
	lsls r3, r3, #16
	bl Actor_SetPositionAndResetMotion
	ldr r3, .L_0200bb40
	movs r1, #144
	mov r8, r3
	lsls r1, r1, #3
	mov r0, r8
	bl Task_Add
	movs r1, #224
	movs r0, #7
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #224
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #224
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #208
	movs r0, #19
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #160
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #160
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #160
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #160
	movs r2, #80
	movs r0, #1
	lsls r1, r1, #8
	bl Actor_ArmCallback
	ldr r5, .L_0200bb44
	movs r0, #23
	adds r1, r5, #0
	bl Actor_SetMovementTypeById
	adds r1, r5, #0
	movs r0, #24
	bl Actor_SetMovementTypeById
	movs r0, #145
	bl Func_020057dc
	movs r0, #192
	movs r1, #192
	movs r2, #128
	lsls r2, r2, #9
	lsls r0, r0, #11
	lsls r1, r1, #11
	bl Func_020055c4
	movs r1, #0
	ldr r0, .L_0200bb48
	bl Func_0200575c
	movs r0, #16
	bl Func_0200576c
	movs r0, #20
	bl wait
	movs r0, #254
	lsls r0, r0, #7
	movs r1, #0
	adds r0, #255
	bl Func_0200575c
	movs r0, #24
	bl Func_0200576c
	movs r0, #60
	bl wait
	movs r0, #141
	bl Func_020057dc
	movs r0, #128
	lsls r0, r0, #2
	bl GameFlag_SetBit
	movs r0, #23
	bl Actor_GetById
	ldr r3, .L_0200bb4c
	str r3, [r0, #12]
	movs r0, #24
	bl Actor_GetById
	ldr r3, .L_0200bb50
	str r3, [r0, #12]
	movs r0, #25
	bl Actor_GetById
	movs r1, #7
	bl Actor_SetPartAttribute
	movs r0, #25
	bl Actor_GetById
	adds r5, r0, #0
	str r6, [r5, #28]
	movs r0, #23
	bl Actor_GetById
	ldr r3, [r0, #24]
	mov r1, r10
	str r3, [r5, #24]
	adds r3, r5, #0
	adds r3, #85
	strb r1, [r3]
	movs r2, #128
	movs r1, #128
	movs r3, #204
	lsls r2, r2, #12
	lsls r3, r3, #16
	adds r0, r5, #0
	lsls r1, r1, #18
	bl Actor_SetPositionAndResetMotion
	movs r0, #26
	bl Actor_GetById
	movs r1, #7
	bl Actor_SetPartAttribute
	movs r0, #26
	bl Actor_GetById
	adds r5, r0, #0
	str r6, [r5, #28]
	movs r0, #23
	bl Actor_GetById
	ldr r3, [r0, #24]
	mov r2, r10
	str r3, [r5, #24]
	adds r3, r5, #0
	adds r3, #85
	strb r2, [r3]
	movs r1, #128
	movs r2, #160
	movs r3, #204
	lsls r2, r2, #14
	lsls r3, r3, #16
	adds r0, r5, #0
	lsls r1, r1, #18
	bl Actor_SetPositionAndResetMotion
	movs r0, #27
	bl Actor_GetById
	movs r1, #7
	bl Actor_SetPartAttribute
	movs r0, #27
	bl Actor_GetById
	adds r5, r0, #0
	str r6, [r5, #28]
	movs r0, #23
	bl Actor_GetById
	ldr r3, [r0, #24]
	mov r1, r10
	str r3, [r5, #24]
	adds r3, r5, #0
	adds r3, #85
	strb r1, [r3]
	movs r2, #144
	movs r1, #128
	movs r3, #204
	adds r0, r5, #0
	lsls r1, r1, #18
	lsls r2, r2, #15
	lsls r3, r3, #16
	bl Actor_SetPositionAndResetMotion
	mov r2, r11
	str r2, [sp, #4]
	movs r5, #17
	movs r0, #106
	movs r1, #36
	movs r2, #87
	movs r3, #7
	str r5, [sp, #0]
	bl Func_020055a4
	mov r3, r11
	str r3, [sp, #4]
	movs r0, #42
	movs r1, #98
	movs r2, #23
	movs r3, #69
	str r5, [sp, #0]
	bl Func_020055a4
	mov r1, r11
	movs r2, #10
	str r1, [sp, #0]
	str r2, [sp, #4]
	movs r1, #40
	movs r2, #18
	movs r3, #20
	movs r0, #0
	bl Func_020055ac
	movs r0, #7
	bl Func_020035ec
	movs r0, #5
	bl Func_020035ec
	movs r0, #6
	bl Func_020035ec
	movs r0, #20
	b .L_0200bb54
	.2byte 0x0000
.L_0200bb38:
	.4byte gMainRoutineWork
.L_0200bb3c:
	.4byte 0xffff0000
.L_0200bb40:
	.4byte Func_0200326c
.L_0200bb44:
	.4byte gData_020060b4
.L_0200bb48:
	.4byte 0x004063ff
.L_0200bb4c:
	.4byte 0xffc80000
.L_0200bb50:
	.4byte 0xffe80000
.L_0200bb54:
	bl Func_020035ec
	movs r0, #21
	bl Func_020035ec
	movs r0, #19
	bl Func_020035ec
	movs r0, #2
	bl Func_0200361c
	movs r0, #1
	bl Func_0200361c
	movs r0, #22
	bl Func_0200361c
	movs r0, #0
	bl Func_0200361c
	movs r0, #3
	bl Func_0200361c
	mov r3, r9
	ldr r0, [r3]
	movs r1, #242
	movs r3, #224
	movs r2, #156
	lsls r3, r3, #8
	lsls r2, r2, #17
	lsls r1, r1, #17
	bl Func_02005684
	mov r1, r9
	ldr r0, [r1]
	movs r1, #1
	bl Actor_SetSpritePriority
	movs r0, #1
	bl wait
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #9
	lsls r0, r0, #11
	lsls r1, r1, #11
	bl Func_020055c4
	movs r1, #0
	ldr r0, .L_0200bfac
	bl Func_0200575c
	movs r0, #120
	bl Func_0200576c
	ldr r5, .L_0200bfb0
	movs r0, #23
	adds r1, r5, #0
	bl Actor_SetMovementTypeById
	adds r1, r5, #0
	movs r0, #24
	bl Actor_SetMovementTypeById
	adds r1, r5, #0
	movs r0, #25
	bl Actor_SetMovementTypeById
	adds r1, r5, #0
	movs r0, #26
	bl Actor_SetMovementTypeById
	adds r1, r5, #0
	movs r0, #27
	bl Actor_SetMovementTypeById
	movs r0, #120
	bl wait
	movs r0, #192
	movs r1, #192
	movs r2, #128
	lsls r2, r2, #9
	lsls r0, r0, #10
	lsls r1, r1, #10
	bl Func_020055c4
	movs r1, #0
	ldr r0, .L_0200bfb4
	bl Func_0200575c
	movs r0, #120
	bl Func_0200576c
	movs r0, #120
	bl wait
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #9
	lsls r0, r0, #10
	lsls r1, r1, #10
	bl Func_020055c4
	movs r0, #128
	movs r1, #0
	lsls r0, r0, #9
	bl Func_0200575c
	movs r0, #120
	bl Func_0200576c
	movs r0, #120
	bl wait
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #9
	lsls r1, r1, #9
	lsls r0, r0, #9
	bl Func_020055c4
	movs r0, #78
	bl Func_020057dc
	movs r0, #23
	bl Actor_GetById
	ldr r5, .L_0200bfb8
	movs r3, #160
	lsls r3, r3, #3
	adds r3, #30
	str r3, [r0, #28]
	adds r1, r5, #0
	movs r0, #24
	bl Actor_SetMovementTypeById
	movs r0, #25
	adds r1, r5, #0
	bl Actor_SetMovementTypeById
	movs r0, #26
	adds r1, r5, #0
	bl Actor_SetMovementTypeById
	adds r1, r5, #0
	movs r0, #27
	bl Actor_SetMovementTypeAndWaitById
	movs r0, #195
	lsls r0, r0, #1
	bl Func_020057dc
	movs r0, #23
	bl Actor_GetById
	movs r1, #15
	bl Actor_SetPartAttribute
	movs r0, #20
	bl FieldEvent_Wait
	ldr r1, .L_0200bfbc
	movs r0, #23
	bl Actor_SetMovementTypeAndWaitById
	mov r0, r8
	bl Task_RemoveFar
	movs r0, #8
	bl Actor_GetById
	adds r3, r0, #0
	adds r3, #85
	mov r2, r10
	strb r2, [r3]
	movs r1, #1
	bl Animation_SetStateFlags
	movs r0, #8
	bl Actor_GetById
	movs r1, #7
	bl Actor_SetPartAttribute
	movs r0, #8
	movs r1, #2
	bl Actor_SetSpritePriority
	ldr r3, .L_0200bfc0
	ldr r2, .L_0200bfc4
	mov r1, r10
	str r1, [r3]
	movs r3, #240
	movs r1, #144
	str r3, [r2]
	ldr r0, .L_0200bfc8
	lsls r1, r1, #3
	bl Task_Add
.L_0200bcea:
	movs r0, #1
	bl wait
	ldr r3, .L_0200bfc0
	ldr r3, [r3]
	cmp r3, #100
	bls .L_0200bcea
	movs r0, #72
	bl Func_020057dc
	movs r0, #120
	bl FieldEvent_Wait
	movs r1, #10
	movs r2, #40
	adds r1, #255
	movs r0, #3
	bl Func_02005714
	ldr r6, .L_0200bfcc
	adds r0, r6, #0
	bl Func_020056dc
	movs r0, #3
	movs r1, #0
	bl Func_020056f4
	movs r1, #6
	movs r2, #40
	adds r1, #255
	movs r0, #2
	bl Func_02005714
	movs r0, #2
	movs r1, #0
	bl Func_020056f4
	movs r0, #6
	movs r1, #0
	bl Func_020056f4
	movs r1, #10
	movs r2, #20
	adds r1, #255
	movs r0, #7
	bl Func_02005714
	movs r0, #7
	movs r1, #0
	bl Func_020056f4
	movs r1, #4
	movs r2, #40
	adds r1, #255
	movs r0, #1
	bl Func_02005714
	movs r0, #1
	movs r1, #0
	bl Func_020056f4
	movs r0, #7
	movs r1, #0
	movs r2, #0
	bl Actor_ArmCallback
	ldr r5, .L_0200bfd0
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
	ldr r0, [r5]
	movs r1, #0
	movs r2, #0
	bl Actor_ArmCallback
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl Actor_ArmCallback
	movs r0, #6
	movs r1, #0
	movs r2, #0
	bl Actor_ArmCallback
	movs r0, #19
	movs r1, #0
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #128
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #128
	movs r2, #20
	movs r0, #2
	lsls r1, r1, #8
	bl Actor_ArmCallback
	movs r0, #7
	movs r1, #3
	bl Actor_SetModeById
	ldr r0, [r5]
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #5
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #6
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #19
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #3
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #0
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #1
	movs r1, #3
	bl Actor_SetModeById
	movs r1, #3
	movs r0, #2
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl FieldEvent_Wait
	movs r1, #128
	movs r0, #7
	lsls r1, r1, #7
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #128
	ldr r0, [r5]
	lsls r1, r1, #7
	movs r2, #0
	bl Actor_ArmCallback
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #192
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #176
	movs r0, #19
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #128
	movs r0, #3
	lsls r1, r1, #7
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #224
	movs r2, #0
	movs r0, #1
	lsls r1, r1, #8
	bl Actor_ArmCallback
	movs r0, #0
	movs r1, #0
	bl Func_02005704
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #5
	movs r1, #0
	movs r2, #10
	bl Func_020056ec
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	adds r2, #102
	negs r0, r0
	negs r1, r1
	bl Func_020055c4
	bl Func_020055cc
	movs r0, #23
	movs r1, #24
	bl Func_0200578c
	adds r0, r6, #6
	movs r1, #1
	bl FieldMessage_Show
	bl Func_02005794
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #23
	movs r1, #24
	bl Func_0200578c
	adds r0, r6, #7
	movs r1, #1
	bl FieldMessage_Show
	bl Func_02005794
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r1, #226
	lsls r1, r1, #1
	mov r8, r1
	add r2, r8
	mov r10, r3
	ldrh r3, [r2]
	movs r1, #0
	adds r3, #2
	strh r3, [r2]
	mov r9, r1
	ldr r0, [r5]
	bl Func_02000610
	movs r0, #7
	bl Func_02000610
	movs r0, #5
	bl Func_02000610
	movs r0, #6
	bl Func_02000610
	movs r0, #19
	bl Func_02000610
	movs r0, #1
	bl Func_02000610
	movs r0, #2
	bl Func_02000610
	movs r0, #3
	bl Func_02000610
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #40
	bl Actor_ArmCallback
	movs r1, #128
	movs r2, #40
	movs r0, #0
	lsls r1, r1, #7
	bl Actor_ArmCallback
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #7
	bl Func_02005704
	movs r0, #192
	movs r2, #10
	lsls r0, r0, #7
	movs r1, #0
	bl Func_020056ec
	adds r6, #9
	movs r0, #23
	movs r1, #24
	bl Func_0200578c
	adds r0, r6, #0
	movs r1, #1
	bl FieldMessage_Show
	bl Func_02005794
	movs r0, #3
	bl Func_02000640
	movs r0, #192
	lsls r0, r0, #7
	movs r1, #128
	lsls r1, r1, #7
	adds r0, #3
	bl Func_02005704
	movs r0, #3
	bl Actor_GetById
	mov r2, r9
	str r2, [r0, #108]
	mov r3, r10
	ldr r2, [r3, #108]
	movs r1, #128
	add r2, r8
	ldrh r3, [r2]
	lsls r1, r1, #1
	adds r3, #1
	strh r3, [r2]
	movs r0, #3
	movs r2, #20
	bl Func_02005714
	movs r2, #20
	movs r1, #0
	movs r0, #3
	bl Func_020056ec
	movs r0, #201
	bl Func_020057dc
	movs r0, #254
	lsls r0, r0, #7
	movs r1, #0
	adds r0, #255
	bl Func_0200575c
	movs r0, #20
	b .L_0200bfd4
.L_0200bfac:
	.4byte 0x004063ff
.L_0200bfb0:
	.4byte gData_020060fc
.L_0200bfb4:
	.4byte 0x00203210
.L_0200bfb8:
	.4byte gData_02006120
.L_0200bfbc:
	.4byte gData_02006150
.L_0200bfc0:
	.4byte Data_02006ccc
.L_0200bfc4:
	.4byte Data_02006b68
.L_0200bfc8:
	.4byte Func_02003370
.L_0200bfcc:
	.4byte MsgTheBeaconIs
.L_0200bfd0:
	.4byte gMainRoutineWork
.L_0200bfd4:
	bl Func_0200576c
	movs r0, #20
	bl wait
	movs r0, #3
	bl Func_02005744
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
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
	.section .text.x0200d0b0,"ax",%progbits
	.balign 4
	.global Func_020050b0
	.thumb_func
Func_020050b0:
	push {r5, r6, r7, lr}
	ldr r1, .L_0200d160
	movs r4, #0
	ldrh r2, [r1]
	adds r3, r2, #0
	cmp r3, #0
	beq .L_0200d0ea
	movs r0, #255
	lsls r0, r0, #8
	adds r0, #255
	adds r3, r2, r0
	strh r3, [r1]
	lsls r3, r3, #16
	cmp r3, #0
	bne .L_0200d0ea
	ldr r0, .L_0200d164
	movs r4, #1
	ldrh r2, [r0]
	strh r2, [r1]
	movs r1, #128
	lsls r3, r2, #16
	lsls r1, r1, #10
	cmp r3, r1
	bls .L_0200d0ea
	movs r1, #255
	lsls r1, r1, #8
	adds r1, #255
	adds r3, r2, r1
	strh r3, [r0]
.L_0200d0ea:
	cmp r4, #0
	bne .L_0200d0f0
	b .L_0200d1dc
.L_0200d0f0:
	ldr r3, .L_0200d168
	ldr r6, .L_0200d16c
	ldr r1, [r3]
	ldrh r3, [r6]
	movs r5, #0
	cmp r5, r3
	bcs .L_0200d13e
	ldr r3, .L_0200d170
	ldr r2, .L_0200d174
	ldr r7, [r3]
	mov lr, r2
	mov r12, r6
.L_0200d108:
	mov r3, lr
	ldrh r2, [r3]
	ldrh r3, [r6]
	movs r0, #160
	muls r3, r2
	adds r3, r3, r5
	lsls r3, r3, #1
	ldrh r3, [r3, r7]
	lsls r0, r0, #19
	lsls r3, r3, #1
	adds r4, r3, r0
	ldrh r0, [r1]
	adds r1, #2
	ldrh r2, [r1]
	adds r1, #2
	ldrh r3, [r1]
	adds r1, #2
	lsls r3, r3, #10
	lsls r2, r2, #5
	orrs r3, r2
	orrs r3, r0
	strh r3, [r4]
	adds r5, #1
	mov r2, r12
	ldrh r3, [r2]
	cmp r5, r3
	bcc .L_0200d108
.L_0200d13e:
	ldr r3, .L_0200d16c
	movs r0, #160
	ldrh r1, [r3]
	ldr r3, .L_0200d178
	lsls r2, r1, #1
	ldr r3, [r3]
	lsls r0, r0, #19
	ldrh r3, [r2, r3]
	adds r2, r2, r1
	lsls r3, r3, #1
	adds r4, r3, r0
	ldr r3, .L_0200d17c
	ldrh r3, [r3]
	cmp r3, #0
	beq .L_0200d184
	ldr r3, .L_0200d180
	b .L_0200d186
.L_0200d160:
	.4byte gData_02006cd8
.L_0200d164:
	.4byte gData_02006cdc
.L_0200d168:
	.4byte Data_02006ce8
.L_0200d16c:
	.4byte gData_02006cec
.L_0200d170:
	.4byte Data_02006cd4
.L_0200d174:
	.4byte gData_02006cf0
.L_0200d178:
	.4byte gData_02006ce0
.L_0200d17c:
	.4byte gData_02006cd0
.L_0200d180:
	.4byte Data_02006ce4
.L_0200d184:
	ldr r3, .L_0200d1cc
.L_0200d186:
	lsls r2, r2, #1
	ldr r3, [r3]
	adds r1, r3, r2
	ldrh r0, [r1]
	adds r1, #2
	ldrh r2, [r1]
	ldrh r3, [r1, #2]
	lsls r3, r3, #10
	lsls r2, r2, #5
	orrs r3, r2
	orrs r3, r0
	strh r3, [r4]
	ldr r1, .L_0200d1d0
	ldr r2, .L_0200d1c4
	ldrh r3, [r1]
	eors r3, r2
	strh r3, [r1]
	ldr r1, .L_0200d1d4
	ldr r2, .L_0200d1d8
	ldrh r3, [r1]
	adds r3, #1
	strh r3, [r1]
	lsls r3, r3, #16
	ldrh r2, [r2]
	lsrs r3, r3, #16
	cmp r3, r2
	bne .L_0200d1dc
	ldr r3, .L_0200d1c8
	strh r3, [r1]
	b .L_0200d1dc
	.2byte 0x0000
.L_0200d1c4:
	.4byte 0x00000001
.L_0200d1c8:
	.4byte 0x00000000
.L_0200d1cc:
	.4byte Data_02006ce8
.L_0200d1d0:
	.4byte gData_02006cd0
.L_0200d1d4:
	.4byte gData_02006cf0
.L_0200d1d8:
	.4byte gData_02006cec
.L_0200d1dc:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
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
