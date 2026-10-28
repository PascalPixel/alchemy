.syntax unified
	.thumb
	.section .text.x0200807e,"ax",%progbits
	.2byte 0x0000
	.section .text.x020080b6,"ax",%progbits
	.2byte 0x0000
	.section .text.x020082a4,"ax",%progbits
	.balign 4
	.global Func_020002a4
	.thumb_func
Func_020002a4:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, .L_02008380
	movs r2, #4
	negs r2, r2
	ands r3, r2
	ldr r3, [r3, #4]
	movs r2, #192
	lsls r2, r2, #18
	mov r11, r3
	ldr r3, [r2, #32]
	movs r0, #160
	lsls r0, r0, #1
	adds r1, r3, r0
	ldr r3, [r2, #108]
	movs r2, #192
	lsls r2, r2, #4
	adds r2, #164
	adds r3, r3, r2
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	ldr r5, .L_02008384
	cmp r3, #0
	bne .L_02008374
	str r3, [r1, #28]
	ldr r3, .L_02008388
	movs r6, #0
	ldrb r2, [r3]
	movs r3, #6
	ldrsh r0, [r1, r3]
	movs r3, #2
	ldrsh r4, [r1, r3]
	movs r3, #204
	lsls r3, r3, #6
	muls r3, r0
	lsls r2, r2, #10
	movs r1, #153
	adds r3, r3, r2
	lsls r1, r1, #5
	mov r10, r3
	adds r3, r0, #0
	muls r3, r1
	mov r9, r4
	subs r7, r3, r2
	cmp r6, #160
	beq .L_02008340
.L_0200830c:
	mov r0, r10
	mov lr, r11
	.2byte 0xf800
	mov r8, r0
	adds r0, r7, #0
	mov lr, r11
	.2byte 0xf800
	mov r4, r8
	lsls r2, r0, #3
	lsls r3, r4, #1
	subs r2, r2, r0
	add r3, r8
	adds r3, r3, r2
	asrs r3, r3, #19
	movs r0, #204
	movs r2, #153
	add r3, r9
	lsls r0, r0, #6
	lsls r2, r2, #5
	adds r6, #1
	strh r3, [r5]
	add r10, r0
	adds r5, #2
	adds r7, r7, r2
	cmp r6, #160
	bne .L_0200830c
.L_02008340:
	ldr r5, .L_02008384
	movs r1, #128
	ldrh r3, [r5]
	lsls r1, r1, #19
	adds r1, #24
	strh r3, [r1]
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #176
	ldrh r0, [r3, #10]
	movs r2, #197
	lsls r2, r2, #8
	adds r2, #255
	ands r2, r0
	strh r2, [r3, #10]
	movs r2, #254
	ldrh r0, [r3, #10]
	lsls r2, r2, #7
	adds r2, #255
	ands r2, r0
	strh r2, [r3, #10]
	adds r0, r5, #2
	ldrh r2, [r3, #10]
	ldr r2, .L_0200838c
	stmia r3!, {r0, r1, r2}
	subs r3, #12
.L_02008374:
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_02008380:
	.4byte Math_Sine
.L_02008384:
	.4byte Data_02006340
.L_02008388:
	.4byte gFrameCount
.L_0200838c:
	.4byte 0xa2600001
	.section .text.x02008390,"ax",%progbits
	.global Func_02000390
	.thumb_func
Func_02000390:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #128
	ldr r3, [r3]
	ldr r7, .L_020083d4
	adds r0, r3, #0
	movs r5, #0
	adds r0, #48
	movs r4, #4
	movs r6, #0
.L_020083a6:
	movs r3, #160
	lsls r3, r3, #19
	lsls r1, r5, #1
	adds r3, #48
	adds r1, r1, r3
	subs r3, r4, #2
	ldrh r2, [r7, r4]
	ldrh r3, [r7, r3]
	lsls r2, r2, #10
	lsls r3, r3, #5
	orrs r2, r3
	ldrh r3, [r7, r6]
	adds r5, #1
	orrs r2, r3
	strh r2, [r1]
	adds r4, #6
	ldrh r3, [r1]
	adds r6, #6
	strh r3, [r0]
	adds r0, #2
	cmp r5, #7
	bls .L_020083a6
	pop {r5, r6, r7, pc}
.L_020083d4:
	.4byte gData_020054ee
	.section .text.x020083d8,"ax",%progbits
	.global Func_020003d8
	.thumb_func
Func_020003d8:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #128
	ldr r3, [r3]
	ldr r7, .L_0200841c
	adds r0, r3, #0
	movs r5, #0
	adds r0, #194
	movs r4, #4
	movs r6, #0
.L_020083ee:
	movs r3, #160
	lsls r3, r3, #19
	lsls r1, r5, #1
	adds r3, #194
	adds r1, r1, r3
	subs r3, r4, #2
	ldrh r2, [r7, r4]
	ldrh r3, [r7, r3]
	lsls r2, r2, #10
	lsls r3, r3, #5
	orrs r2, r3
	ldrh r3, [r7, r6]
	adds r5, #1
	orrs r2, r3
	strh r2, [r1]
	adds r4, #6
	ldrh r3, [r1]
	adds r6, #6
	strh r3, [r0]
	adds r0, #2
	cmp r5, #8
	bls .L_020083ee
	pop {r5, r6, r7, pc}
.L_0200841c:
	.4byte gData_020054b8
	.section .text.x020092a6,"ax",%progbits
	.2byte 0x0000
	.section .text.x020092a8,"ax",%progbits
	.global Func_020012a8
	.thumb_func
Func_020012a8:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #68
	bl Actor_GetById
	ldr r3, .L_02009328
	add r2, sp, #16
	str r3, [r2, #36]
	movs r3, #0
	adds r7, r0, #0
	mov r9, r2
	mov r10, r3
.L_020092c6:
	mov r2, r10
	lsls r6, r2, #12
	adds r0, r6, #0
	bl Math_Cosine
	add r5, sp, #56
	movs r3, #0
	str r0, [r5]
	adds r0, r6, #0
	str r3, [r5, #4]
	bl Math_Sine
	ldr r6, [r5]
	mov r8, r0
	str r0, [r5, #8]
	movs r1, #3
	adds r0, r6, #0
	bl __divsi3
	ldr r3, [r5, #4]
	adds r6, r6, r0
	str r6, [r5]
	ldr r2, [r7, #16]
	ldr r0, [r7, #8]
	ldr r1, [r7, #12]
	str r3, [sp, #0]
	mov r3, r8
	str r3, [sp, #4]
	movs r3, #128
	lsls r3, r3, #17
	adds r3, #1
	str r3, [sp, #8]
	mov r3, r9
	str r3, [sp, #12]
	adds r3, r6, #0
	bl Func_020000b8
	movs r2, #2
	add r10, r2
	mov r3, r10
	cmp r3, #16
	bls .L_020092c6
	add sp, #68
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009328:
	.4byte Effect_AdvanceMotion
	.section .text.x02009490,"ax",%progbits
	.balign 4
	.global Func_02001490
	.thumb_func
Func_02001490:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #28
	sub sp, #12
	bl GameFlag_SetBit
	bl Func_020051ac
	movs r0, #0
	bl Func_02005324
	movs r1, #192
	movs r2, #128
	lsls r1, r1, #15
	lsls r2, r2, #12
	movs r0, #8
	bl Func_0200520c
	ldr r0, .L_0200981c
	bl Func_0200524c
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #40
	movs r0, #4
	bl Func_0200528c
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #4
	adds r1, #204
	adds r2, #102
	bl Actor_SetSpeedParameters
	movs r2, #152
	movs r0, #4
	movs r1, #152
	lsls r2, r2, #1
	bl Actor_SetPositionAndReset
	movs r2, #16
	movs r3, #176
	movs r0, #9
	movs r1, #32
	negs r2, r2
	lsls r3, r3, #8
	bl Func_02005334
	movs r3, #192
	movs r0, #5
	movs r1, #0
	movs r2, #0
	lsls r3, r3, #8
	bl Func_02005334
	movs r3, #176
	movs r0, #6
	movs r1, #16
	movs r2, #0
	lsls r3, r3, #8
	bl Func_02005334
	movs r0, #7
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200953c
	movs r3, #192
	movs r0, #7
	movs r1, #8
	movs r2, #16
	lsls r3, r3, #8
	bl Func_02005334
.L_0200953c:
	movs r1, #16
	movs r2, #16
	movs r0, #4
	negs r1, r1
	negs r2, r2
	bl Actor_CommitPositionAndActivate
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #4
	bl Actor_ArmCallback
	movs r0, #9
	bl Actor_WaitForMoveById
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r0, #4
	movs r1, #0
	movs r2, #0
	bl Actor_ArmCallback
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #128
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #9
	bl Actor_ArmCallback
	movs r0, #25
	bl FieldEvent_Wait
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #2
	adds r1, #255
	movs r2, #0
	movs r0, #6
	bl Func_0200528c
	movs r1, #2
	movs r0, #9
	adds r1, #255
	movs r2, #0
	bl Func_0200528c
	movs r0, #7
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020095d6
	movs r1, #2
	movs r0, #7
	adds r1, #255
	movs r2, #0
	bl Func_0200528c
.L_020095d6:
	movs r1, #2
	adds r1, #255
	movs r2, #50
	movs r0, #4
	bl Func_0200528c
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r0, #7
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020095fe
	movs r0, #7
	movs r1, #2
	bl Actor_StartRepeatedMotion
.L_020095fe:
	movs r0, #6
	movs r1, #2
	bl Actor_StartRepeatedMotion
	movs r0, #9
	movs r1, #2
	bl Actor_StartRepeatedMotion
	movs r0, #4
	movs r1, #2
	bl Actor_StartRepeatedMotion
	movs r1, #2
	movs r0, #5
	bl Motion_SetVarCbAndRefresh
	movs r0, #20
	bl FieldEvent_Wait
	movs r1, #192
	movs r0, #4
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #192
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #192
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #176
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #9
	bl Actor_ArmCallback
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #144
	movs r1, #1
	movs r2, #128
	negs r1, r1
	lsls r2, r2, #16
	movs r3, #1
	lsls r0, r0, #16
	bl Camera_MoveWithinMapBounds
	bl Func_020052b4
	movs r0, #8
	bl Actor_GetById
	movs r1, #2
	mov r10, r0
	movs r0, #8
	bl Actor_SetModeById
	movs r6, #0
.L_02009682:
	lsls r7, r6, #13
	adds r0, r7, #0
	bl Math_Sine
	ldr r3, .L_02009820
	mov r5, sp
	muls r3, r6
	cmp r3, #0
	bge .L_02009696
	adds r3, #7
.L_02009696:
	lsls r2, r0, #1
	movs r1, #208
	lsls r1, r1, #15
	adds r2, r2, r0
	asrs r3, r3, #3
	lsls r2, r2, #2
	adds r3, r3, r1
	subs r3, r3, r2
	str r3, [r5]
	adds r0, r7, #0
	bl Math_Sine
	movs r3, #176
	lsls r3, r3, #15
	adds r2, r6, #0
	muls r2, r3
	cmp r2, #0
	bge .L_020096bc
	adds r2, #7
.L_020096bc:
	movs r3, #128
	lsls r3, r3, #14
	asrs r2, r2, #3
	adds r2, r2, r3
	lsls r3, r0, #3
	subs r3, r3, r0
	subs r2, r2, r3
	movs r3, #0
	str r2, [r5, #8]
	str r3, [r5, #4]
	bl Random16Far
	lsls r3, r0, #2
	adds r3, r3, r0
	lsls r3, r3, #1
	movs r2, #200
	lsrs r3, r3, #16
	lsls r2, r2, #5
	adds r3, #4
	adds r2, #153
	adds r1, r3, #0
	muls r1, r2
	movs r2, #192
	lsls r2, r2, #4
	adds r2, #204
	muls r2, r3
	movs r0, #8
	bl Actor_SetSpeedParameters
	ldr r1, [r5]
	cmp r1, #0
	bge .L_02009704
	movs r2, #255
	lsls r2, r2, #8
	adds r2, #255
	adds r1, r1, r2
.L_02009704:
	ldr r2, [r5, #8]
	asrs r1, r1, #16
	cmp r2, #0
	bge .L_02009714
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	adds r2, r2, r3
.L_02009714:
	asrs r2, r2, #16
	movs r0, #8
	adds r6, #1
	bl Actor_SetPositionAndReset
	cmp r6, #7
	ble .L_02009682
	movs r1, #1
	movs r0, #8
	bl Actor_SetModeById
	movs r0, #10
	bl FieldEvent_Wait
	movs r0, #8
	movs r1, #2
	bl Motion_SetVarCbAndRefresh
	ldr r1, .L_02009824
	movs r0, #8
	bl Actor_SetMovementTypeById
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r1, #226
	lsls r1, r1, #1
	adds r2, r2, r1
	ldrh r3, [r2]
	movs r1, #0
	subs r3, #1
	strh r3, [r2]
	movs r0, #8
	movs r2, #0
	bl Func_0200525c
	movs r0, #8
	bl Actor_Stop
	movs r0, #20
	bl FieldEvent_Wait
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #6
	movs r0, #8
	bl Actor_ArmCallback
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #8
	movs r1, #1
	bl Camera_FollowActor
	movs r1, #2
	movs r0, #8
	bl Actor_SetModeById
	mov r2, r10
	ldr r2, [r2, #8]
	mov r3, r10
	mov r8, r2
	movs r1, #128
	movs r2, #184
	ldr r7, [r3, #16]
	lsls r1, r1, #16
	lsls r2, r2, #16
	mov r9, r1
	mov r10, r2
	movs r6, #0
.L_020097a2:
	lsls r0, r6, #13
	bl Math_Sine
	mov r1, r9
	mov r2, r8
	subs r3, r1, r2
	muls r3, r6
	cmp r3, #0
	bge .L_020097b6
	adds r3, #7
.L_020097b6:
	asrs r3, r3, #3
	add r3, r8
	lsls r2, r0, #2
	subs r3, r3, r2
	mov r1, r10
	str r3, [r5]
	subs r3, r1, r7
	muls r3, r6
	cmp r3, #0
	bge .L_020097cc
	adds r3, #7
.L_020097cc:
	asrs r3, r3, #3
	adds r3, r7, r3
	str r3, [r5, #8]
	movs r3, #0
	str r3, [r5, #4]
	bl Random16Far
	lsls r3, r0, #2
	adds r3, r3, r0
	lsls r3, r3, #1
	movs r2, #200
	lsrs r3, r3, #16
	lsls r2, r2, #5
	adds r3, #4
	adds r2, #153
	adds r1, r3, #0
	muls r1, r2
	movs r2, #192
	lsls r2, r2, #4
	adds r2, #204
	muls r2, r3
	movs r0, #8
	bl Actor_SetSpeedParameters
	ldr r1, [r5]
	cmp r1, #0
	bge .L_0200980a
	movs r2, #255
	lsls r2, r2, #8
	adds r2, #255
	adds r1, r1, r2
.L_0200980a:
	ldr r2, [r5, #8]
	asrs r1, r1, #16
	cmp r2, #0
	bge .L_02009828
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	adds r2, r2, r3
	b .L_02009828
.L_0200981c:
	.4byte MsgGaroh01
.L_02009820:
	.4byte 0xfff00000
.L_02009824:
	.4byte gData_0200545c
.L_02009828:
	asrs r2, r2, #16
	movs r0, #8
	adds r6, #1
	bl Actor_SetPositionAndReset
	cmp r6, #7
	ble .L_020097a2
	movs r0, #8
	movs r1, #1
	bl Actor_SetModeById
	movs r1, #192
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #8
	bl Actor_ArmCallback
	movs r0, #20
	bl FieldEvent_Wait
	movs r1, #0
	movs r2, #0
	movs r0, #8
	bl Actor_ArmCallback
	movs r0, #30
	bl FieldEvent_Wait
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r2, #0
	movs r0, #6
	movs r1, #0
	bl Func_0200525c
	movs r1, #2
	movs r0, #8
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl FieldEvent_Wait
	movs r1, #192
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #8
	bl Actor_ArmCallback
	movs r0, #30
	bl FieldEvent_Wait
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #50
	movs r0, #8
	bl Func_0200528c
	movs r1, #153
	movs r2, #152
	lsls r1, r1, #8
	lsls r2, r2, #7
	adds r1, #153
	adds r2, #204
	movs r0, #8
	bl Actor_SetSpeedParameters
	movs r0, #8
	bl Actor_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	strb r3, [r0]
	movs r1, #128
	movs r2, #148
	movs r0, #8
	bl Actor_SetPositionAndReset
	movs r0, #1
	bl FieldEvent_Wait
	movs r0, #8
	bl Actor_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	movs r1, #160
	movs r2, #160
	strb r3, [r0]
	lsls r1, r1, #10
	movs r0, #8
	lsls r2, r2, #9
	bl Actor_SetSpeedParameters
	movs r1, #0
	movs r2, #4
	movs r0, #8
	bl Actor_CommitPositionAndActivate
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #1
	movs r1, #1
	movs r2, #1
	movs r3, #0
	negs r0, r0
	negs r1, r1
	negs r2, r2
	bl Camera_MoveWithinMapBounds
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #8
	bl Actor_ArmCallback
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #8
	movs r1, #3
	bl Actor_StartRepeatedMotion
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #8
	bl Func_02005294
	movs r0, #40
	bl FieldEvent_Wait
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #8
	bl Actor_ArmCallback
	movs r0, #10
	bl FieldEvent_Wait
	movs r0, #8
	ldr r1, .L_02009cc8
	ldr r2, .L_02009ccc
	bl Actor_SetSpeedParameters
	movs r1, #24
	movs r2, #8
	movs r0, #8
	negs r1, r1
	negs r2, r2
	bl Actor_CommitPositionAndActivate
	movs r1, #8
	movs r2, #100
	negs r1, r1
	negs r2, r2
	movs r0, #8
	bl Actor_CommitPositionAndActivate
	movs r0, #30
	bl FieldEvent_Wait
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl Func_0200520c
	movs r0, #168
	movs r1, #1
	movs r2, #140
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #17
	lsls r0, r0, #16
	bl Camera_MoveWithinMapBounds
	bl Func_020052b4
	movs r0, #10
	bl FieldEvent_Wait
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #4
	bl Func_0200528c
	movs r1, #129
	movs r0, #5
	lsls r1, r1, #1
	movs r2, #0
	bl Func_0200528c
	movs r0, #7
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020099ce
	movs r1, #129
	movs r0, #7
	lsls r1, r1, #1
	movs r2, #0
	bl Func_0200528c
.L_020099ce:
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #40
	movs r0, #6
	bl Func_0200528c
	movs r0, #9
	movs r1, #6
	movs r2, #15
	bl Actor_Jump
	movs r0, #9
	movs r1, #6
	movs r2, #23
	bl Actor_Jump
	movs r0, #9
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #10
	bl FieldEvent_Wait
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
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
	movs r0, #4
	movs r1, #0
	movs r2, #0
	bl Actor_ArmCallback
	movs r0, #7
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009a44
	movs r1, #208
	movs r0, #7
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
.L_02009a44:
	movs r0, #25
	bl FieldEvent_Wait
	movs r1, #2
	movs r0, #5
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl FieldEvent_Wait
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #160
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #9
	bl Actor_ArmCallback
	movs r0, #25
	bl FieldEvent_Wait
	movs r2, #0
	movs r0, #9
	movs r1, #0
	bl Func_0200525c
	movs r1, #2
	movs r0, #6
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl FieldEvent_Wait
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #6
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #9
	bl Func_0200528c
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r0, #7
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009ac6
	movs r1, #2
	movs r0, #7
	adds r1, #255
	movs r2, #0
	bl Func_0200528c
.L_02009ac6:
	movs r1, #2
	adds r1, #255
	movs r2, #0
	movs r0, #6
	bl Func_0200528c
	movs r1, #2
	adds r1, #255
	movs r2, #0
	movs r0, #5
	bl Func_0200528c
	movs r1, #2
	movs r2, #50
	adds r1, #255
	movs r0, #4
	bl Func_0200528c
	movs r1, #2
	movs r0, #5
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl FieldEvent_Wait
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r0, #9
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #10
	bl FieldEvent_Wait
	movs r2, #0
	movs r0, #9
	movs r1, #0
	bl Func_0200525c
	movs r1, #2
	movs r0, #6
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl FieldEvent_Wait
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #6
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #9
	bl Func_0200528c
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #2
	adds r1, #255
	movs r2, #50
	movs r0, #6
	bl Func_0200528c
	movs r0, #9
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #10
	bl FieldEvent_Wait
	movs r2, #0
	movs r0, #9
	movs r1, #0
	bl Func_0200525c
	movs r1, #2
	movs r0, #9
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl FieldEvent_Wait
	movs r2, #0
	movs r0, #9
	movs r1, #0
	bl Func_0200525c
	movs r0, #5
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #6
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #7
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009bb6
	movs r0, #7
	movs r1, #3
	bl Actor_SetModeById
.L_02009bb6:
	movs r1, #3
	movs r0, #4
	bl Actor_SetModeById
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #30
	bl FieldEvent_Wait
	movs r1, #10
	adds r1, #255
	movs r2, #30
	movs r0, #9
	bl Func_0200528c
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r0, #9
	movs r1, #4
	bl Actor_SetModeById
	movs r0, #33
	bl FieldEvent_Wait
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #8
	movs r2, #8
	movs r0, #9
	negs r1, r1
	negs r2, r2
	bl Actor_CommitPositionAndActivate
	movs r1, #192
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #10
	movs r2, #6
	movs r0, #9
	negs r1, r1
	negs r2, r2
	bl Actor_CommitPositionAndActivate
	movs r1, #192
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #14
	movs r2, #4
	movs r0, #9
	negs r1, r1
	negs r2, r2
	bl Actor_CommitPositionAndActivate
	movs r1, #224
	movs r0, #4
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #160
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r0, #7
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009c68
	movs r1, #192
	movs r0, #7
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
.L_02009c68:
	movs r0, #30
	bl FieldEvent_Wait
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #9
	bl Actor_ArmCallback
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #9
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #10
	bl FieldEvent_Wait
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #160
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r0, #7
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009cd0
	movs r1, #176
	movs r0, #7
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	b .L_02009cd0
	.2byte 0x0000
.L_02009cc8:
	.4byte 0x00026666
.L_02009ccc:
	.4byte 0x00013333
.L_02009cd0:
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #128
	lsls r0, r0, #6
	movs r1, #0
	adds r0, #5
	bl FieldMessage_ShowAtActor
	movs r1, #128
	movs r0, #4
	lsls r1, r1, #6
	movs r2, #0
	bl Actor_ArmCallback
	movs r0, #4
	movs r1, #0
	bl Event_AskYesNoWithActorAnimation
	cmp r0, #0
	bne .L_02009d58
	movs r0, #30
	bl FieldEvent_Wait
	movs r1, #2
	movs r0, #9
	bl Motion_SetVarCbAndRefresh
	movs r0, #20
	bl FieldEvent_Wait
	movs r1, #160
	movs r0, #9
	lsls r1, r1, #7
	movs r2, #0
	bl Actor_ArmCallback
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #7
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009d36
	movs r1, #192
	movs r0, #7
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
.L_02009d36:
	movs r0, #128
	lsls r0, r0, #6
	movs r1, #0
	movs r2, #0
	adds r0, #9
	bl Func_0200525c
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r1, #226
	lsls r1, r1, #1
	adds r2, r2, r1
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_02009db4
.L_02009d58:
	movs r0, #40
	bl FieldEvent_Wait
	movs r1, #2
	movs r0, #9
	bl Motion_SetVarCbAndRefresh
	movs r0, #20
	bl FieldEvent_Wait
	movs r1, #160
	movs r0, #9
	lsls r1, r1, #7
	movs r2, #0
	bl Actor_ArmCallback
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #7
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009d94
	movs r1, #192
	movs r0, #7
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
.L_02009d94:
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #128
	adds r3, #1
	lsls r0, r0, #6
	strh r3, [r2]
	adds r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
.L_02009db4:
	movs r0, #9
	movs r1, #6
	movs r2, #15
	bl Actor_Jump
	movs r0, #9
	movs r1, #6
	movs r2, #23
	bl Actor_Jump
	movs r0, #128
	lsls r0, r0, #6
	movs r2, #0
	adds r0, #9
	movs r1, #0
	bl Func_0200525c
	movs r1, #2
	movs r0, #5
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl FieldEvent_Wait
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #5
	bl Actor_ArmCallback
	movs r0, #15
	bl FieldEvent_Wait
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #224
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #4
	bl Actor_ArmCallback
	movs r0, #25
	bl FieldEvent_Wait
	movs r0, #9
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #8
	bl FieldEvent_Wait
	movs r0, #9
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #8
	bl FieldEvent_Wait
	movs r0, #9
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #20
	bl FieldEvent_Wait
	movs r1, #2
	adds r1, #255
	movs r2, #30
	movs r0, #6
	bl Func_0200528c
	movs r0, #128
	lsls r0, r0, #6
	movs r2, #0
	adds r0, #6
	movs r1, #0
	bl Func_0200525c
	movs r0, #9
	movs r1, #3
	bl Actor_StartRepeatedMotion
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #9
	bl Func_02005294
	movs r0, #40
	bl FieldEvent_Wait
	movs r0, #128
	lsls r0, r0, #6
	movs r1, #0
	movs r2, #0
	adds r0, #9
	bl Func_0200525c
	movs r0, #6
	bl Func_020013b0
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #128
	lsls r0, r0, #6
	movs r2, #0
	adds r0, #6
	movs r1, #0
	bl Func_0200525c
	movs r1, #2
	movs r0, #9
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl FieldEvent_Wait
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #7
	lsls r2, r2, #6
	adds r1, #102
	adds r2, #51
	movs r0, #9
	bl Actor_SetSpeedParameters
	movs r0, #9
	bl Actor_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	movs r2, #16
	strb r3, [r0]
	movs r1, #0
	negs r2, r2
	movs r0, #9
	bl Actor_CommitPositionAndActivate
	movs r0, #1
	bl FieldEvent_Wait
	movs r0, #9
	bl Actor_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	strb r3, [r0]
	movs r0, #20
	bl FieldEvent_Wait
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #9
	bl Func_02005294
	movs r0, #40
	bl FieldEvent_Wait
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #160
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #5
	bl Actor_ArmCallback
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #192
	movs r0, #6
	lsls r1, r1, #7
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #128
	movs r0, #4
	lsls r1, r1, #6
	movs r2, #0
	bl Actor_ArmCallback
	movs r0, #7
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009f68
	movs r1, #176
	movs r0, #7
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
.L_02009f68:
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #6
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #7
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009f88
	movs r0, #7
	movs r1, #3
	bl Actor_SetModeById
.L_02009f88:
	movs r1, #3
	movs r0, #4
	bl Actor_SetModeById
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #35
	bl FieldEvent_Wait
	movs r0, #7
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009fb2
	movs r1, #192
	movs r0, #7
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
.L_02009fb2:
	movs r1, #192
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #160
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #224
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #4
	bl Actor_ArmCallback
	movs r0, #25
	bl FieldEvent_Wait
	movs r1, #10
	adds r1, #255
	movs r2, #30
	movs r0, #9
	bl Func_0200528c
	movs r0, #20
	bl FieldEvent_Wait
	movs r1, #128
	movs r2, #128
	movs r0, #9
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl Actor_SetSpeedParameters
	movs r1, #0
	movs r2, #16
	movs r0, #9
	bl Actor_CommitPositionAndActivate
	movs r0, #20
	bl FieldEvent_Wait
	movs r1, #4
	adds r1, #255
	movs r2, #30
	movs r0, #9
	bl Func_0200528c
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #4
	bl Func_0200528c
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #1
	movs r2, #0
	bl Func_0200528c
	movs r0, #7
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a054
	movs r1, #128
	movs r0, #7
	lsls r1, r1, #1
	movs r2, #0
	bl Func_0200528c
.L_0200a054:
	movs r1, #128
	movs r2, #50
	lsls r1, r1, #1
	movs r0, #6
	bl Func_0200528c
	movs r1, #2
	movs r0, #9
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl FieldEvent_Wait
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #128
	movs r2, #128
	movs r0, #9
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl Actor_SetSpeedParameters
	movs r1, #16
	movs r0, #9
	negs r1, r1
	movs r2, #0
	bl Actor_CommitPositionAndActivate
	movs r1, #160
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #160
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #192
	movs r0, #4
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #128
	movs r0, #9
	lsls r1, r1, #7
	movs r2, #0
	bl Actor_ArmCallback
	movs r0, #7
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a0dc
	movs r1, #160
	movs r0, #7
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
.L_0200a0dc:
	movs r0, #20
	bl FieldEvent_Wait
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #40
	movs r0, #4
	bl Func_0200528c
	movs r0, #9
	movs r1, #4
	bl Actor_SetModeById
	movs r0, #33
	bl FieldEvent_Wait
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r0, #9
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r0, #9
	movs r1, #6
	movs r2, #15
	bl Actor_Jump
	movs r0, #9
	movs r1, #6
	movs r2, #23
	bl Actor_Jump
	movs r0, #7
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a150
	movs r1, #129
	movs r0, #7
	lsls r1, r1, #1
	movs r2, #0
	bl Func_0200528c
.L_0200a150:
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #6
	bl Func_0200528c
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #4
	bl Func_0200528c
	movs r1, #129
	movs r2, #50
	lsls r1, r1, #1
	movs r0, #5
	bl Func_0200528c
	movs r0, #40
	bl FieldEvent_Wait
	movs r1, #128
	lsls r1, r1, #6
	movs r0, #4
	bl Func_02005274
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #6
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #4
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #7
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a1ac
	movs r0, #7
	movs r1, #3
	bl Actor_SetModeById
.L_0200a1ac:
	movs r1, #3
	movs r0, #5
	bl Actor_SetModeById
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #30
	bl FieldEvent_Wait
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #9
	ldr r1, .L_0200a2ec
	adds r2, #153
	bl Actor_SetSpeedParameters
	movs r0, #9
	movs r1, #2
	bl Actor_SetModeById
	ldr r3, .L_0200a2f0
	movs r1, #133
	lsls r1, r1, #2
	adds r5, r3, r1
	ldr r0, [r5]
	bl Actor_GetById
	cmp r0, #0
	beq .L_0200a1f6
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #9
	bl Actor_ResetAndSetPosition
.L_0200a1f6:
	movs r0, #9
	bl Actor_WaitForMoveById
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_0200520c
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #5
	ldr r1, .L_0200a2ec
	adds r2, #153
	bl Actor_SetSpeedParameters
	movs r0, #5
	movs r1, #2
	bl Actor_SetModeById
	ldr r0, [r5]
	bl Actor_GetById
	cmp r0, #0
	beq .L_0200a234
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #5
	bl Actor_ResetAndSetPosition
.L_0200a234:
	movs r0, #5
	bl Actor_WaitForMoveById
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl Func_0200520c
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #6
	ldr r1, .L_0200a2ec
	adds r2, #153
	bl Actor_SetSpeedParameters
	movs r0, #6
	movs r1, #2
	bl Actor_SetModeById
	ldr r0, [r5]
	bl Actor_GetById
	cmp r0, #0
	beq .L_0200a272
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #6
	bl Actor_ResetAndSetPosition
.L_0200a272:
	movs r0, #6
	bl Actor_WaitForMoveById
	movs r0, #6
	movs r1, #0
	movs r2, #0
	bl Func_0200520c
	movs r0, #7
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a2ca
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #7
	ldr r1, .L_0200a2ec
	adds r2, #153
	bl Actor_SetSpeedParameters
	movs r0, #7
	movs r1, #2
	bl Actor_SetModeById
	ldr r0, [r5]
	bl Actor_GetById
	cmp r0, #0
	beq .L_0200a2ba
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #7
	bl Actor_ResetAndSetPosition
.L_0200a2ba:
	movs r0, #7
	bl Actor_WaitForMoveById
	movs r0, #7
	movs r1, #0
	movs r2, #0
	bl Func_0200520c
.L_0200a2ca:
	ldr r3, .L_0200a2f0
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	ldr r0, [r3]
	movs r1, #1
	bl Camera_FollowActor
	bl Func_020051b4
	add sp, #12
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200a2ec:
	.4byte 0x00013333
.L_0200a2f0:
	.4byte gSequenceParentWork
	.section .text.x0200b878,"ax",%progbits
	.global Func_02003878
	.thumb_func
Func_02003878:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r0, #130
	lsls r0, r0, #4
	adds r0, #255
	sub sp, #28
	bl GameFlag_SetBit
	bl Func_020051ac
	movs r0, #0
	bl Func_02005324
	ldr r0, .L_0200ba88
	bl Func_0200524c
	movs r1, #164
	movs r2, #160
	movs r0, #10
	lsls r1, r1, #17
	lsls r2, r2, #14
	bl Func_0200520c
	movs r1, #128
	movs r0, #10
	lsls r1, r1, #7
	movs r2, #0
	bl Actor_ArmCallback
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #4
	adds r1, #204
	adds r2, #102
	bl Actor_SetSpeedParameters
	movs r1, #160
	movs r0, #4
	lsls r1, r1, #1
	movs r2, #224
	bl Actor_SetPositionAndReset
	movs r1, #16
	movs r3, #192
	movs r0, #9
	negs r1, r1
	movs r2, #0
	lsls r3, r3, #8
	bl Func_02005334
	movs r3, #192
	movs r0, #5
	movs r1, #16
	movs r2, #0
	lsls r3, r3, #8
	bl Func_02005334
	movs r3, #192
	movs r0, #6
	movs r1, #32
	movs r2, #0
	lsls r3, r3, #8
	bl Func_02005334
	movs r0, #7
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200b920
	movs r3, #192
	movs r0, #7
	movs r1, #48
	movs r2, #0
	lsls r3, r3, #8
	bl Func_02005334
.L_0200b920:
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #4
	bl Actor_ArmCallback
	movs r0, #6
	bl Actor_WaitForMoveById
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #164
	movs r1, #1
	movs r2, #144
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #16
	lsls r0, r0, #17
	bl Camera_MoveWithinMapBounds
	bl Func_020052b4
	movs r0, #20
	bl FieldEvent_Wait
	movs r1, #164
	movs r2, #136
	lsls r2, r2, #16
	lsls r1, r1, #17
	movs r0, #11
	bl Func_0200520c
	movs r0, #11
	bl Actor_GetById
	movs r1, #0
	bl Actor_SetSingleChildFlags
	movs r0, #11
	bl Func_02001418
	movs r0, #30
	bl FieldEvent_Wait
	movs r1, #179
	movs r2, #178
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #10
	adds r1, #51
	adds r2, #153
	bl Actor_SetSpeedParameters
	movs r1, #164
	movs r2, #128
	lsls r1, r1, #17
	lsls r2, r2, #16
	movs r0, #10
	bl Func_0200520c
	movs r0, #10
	bl FieldEvent_Wait
	movs r1, #0
	movs r2, #16
	movs r0, #10
	bl Actor_CommitPositionAndActivate
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #11
	bl Func_02001454
	movs r2, #0
	movs r0, #11
	movs r1, #0
	bl Func_0200520c
	movs r1, #1
	movs r0, #10
	bl Camera_FollowActor
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #10
	movs r1, #0
	movs r2, #48
	bl Actor_CommitPositionAndActivate
	movs r1, #224
	movs r0, #9
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #192
	movs r0, #4
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #192
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #160
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r0, #7
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200ba1e
	movs r1, #160
	movs r0, #7
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
.L_0200ba1e:
	movs r0, #164
	movs r1, #1
	movs r2, #184
	negs r1, r1
	lsls r2, r2, #16
	movs r3, #1
	lsls r0, r0, #17
	bl Camera_MoveWithinMapBounds
	bl Func_020052b4
	movs r0, #10
	bl FieldEvent_Wait
	movs r0, #10
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #10
	bl FieldEvent_Wait
	movs r0, #192
	lsls r0, r0, #7
	movs r2, #0
	adds r0, #10
	movs r1, #0
	bl Func_0200525c
	movs r0, #4
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #5
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #6
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #7
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200ba8c
	movs r0, #7
	movs r1, #3
	bl Actor_SetModeById
	b .L_0200ba8c
.L_0200ba88:
	.4byte MsgGaroh28
.L_0200ba8c:
	movs r1, #3
	movs r0, #9
	bl Actor_SetModeById
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #20
	bl FieldEvent_Wait
	movs r2, #16
	negs r2, r2
	movs r1, #0
	movs r0, #9
	bl Actor_CommitPositionAndActivate
	movs r0, #10
	bl FieldEvent_Wait
	movs r1, #2
	movs r0, #9
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl FieldEvent_Wait
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #8
	movs r0, #10
	negs r1, r1
	movs r2, #0
	bl Actor_CommitPositionAndActivate
	movs r1, #160
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #10
	bl Actor_ArmCallback
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #192
	lsls r0, r0, #7
	adds r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #9
	bl Func_0200528c
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #8
	adds r1, #255
	movs r2, #30
	movs r0, #10
	bl Func_0200528c
	movs r0, #192
	lsls r0, r0, #7
	adds r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #10
	adds r1, #255
	movs r2, #30
	movs r0, #9
	bl Func_0200528c
	movs r2, #0
	movs r0, #9
	movs r1, #0
	bl Func_0200525c
	movs r1, #2
	movs r0, #10
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl FieldEvent_Wait
	movs r0, #192
	lsls r0, r0, #7
	adds r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #2
	adds r1, #255
	movs r2, #30
	movs r0, #9
	bl Func_0200528c
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r0, #10
	movs r1, #16
	movs r2, #0
	bl Actor_CommitPositionAndActivate
	movs r1, #160
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #10
	bl Actor_ArmCallback
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #192
	lsls r0, r0, #7
	adds r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #20
	movs r0, #5
	bl Func_0200528c
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #128
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #10
	bl Actor_ArmCallback
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #192
	lsls r0, r0, #7
	movs r2, #0
	adds r0, #10
	movs r1, #0
	bl Func_0200525c
	movs r1, #2
	movs r0, #6
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl FieldEvent_Wait
	movs r0, #6
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #8
	movs r0, #10
	negs r1, r1
	movs r2, #0
	bl Actor_CommitPositionAndActivate
	movs r1, #160
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #10
	bl Actor_ArmCallback
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #192
	lsls r0, r0, #7
	adds r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r0, #4
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #35
	bl FieldEvent_Wait
	movs r1, #8
	movs r0, #10
	negs r1, r1
	movs r2, #0
	bl Actor_CommitPositionAndActivate
	movs r1, #160
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #10
	bl Actor_ArmCallback
	movs r0, #25
	bl FieldEvent_Wait
	movs r0, #10
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #10
	bl FieldEvent_Wait
	movs r0, #192
	lsls r0, r0, #7
	adds r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #9
	bl Func_0200528c
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r0, #10
	movs r1, #4
	bl Actor_SetModeById
	movs r0, #33
	bl FieldEvent_Wait
	movs r0, #192
	lsls r0, r0, #7
	adds r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #4
	adds r1, #255
	movs r2, #45
	movs r0, #9
	bl Func_0200528c
	movs r0, #9
	movs r1, #6
	movs r2, #15
	bl Actor_Jump
	movs r0, #9
	movs r1, #6
	movs r2, #23
	bl Actor_Jump
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #10
	bl Func_0200528c
	movs r0, #192
	lsls r0, r0, #7
	movs r2, #0
	adds r0, #10
	movs r1, #0
	bl Func_0200525c
	movs r1, #2
	movs r0, #9
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl FieldEvent_Wait
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #10
	bl Actor_ArmCallback
	movs r0, #30
	bl FieldEvent_Wait
	movs r0, #192
	lsls r0, r0, #7
	adds r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r2, #12
	negs r2, r2
	movs r1, #4
	movs r0, #10
	bl Actor_CommitPositionAndActivate
	movs r0, #10
	bl FieldEvent_Wait
	movs r1, #2
	movs r0, #10
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl FieldEvent_Wait
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r2, #12
	movs r1, #4
	negs r2, r2
	movs r0, #10
	bl Actor_CommitPositionAndActivate
	movs r0, #10
	bl FieldEvent_Wait
	movs r0, #10
	movs r1, #4
	bl Actor_SetModeById
	movs r0, #33
	bl FieldEvent_Wait
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #10
	bl Actor_ArmCallback
	movs r0, #30
	bl FieldEvent_Wait
	movs r0, #128
	lsls r0, r0, #6
	movs r2, #0
	adds r0, #10
	movs r1, #0
	bl Func_0200525c
	movs r0, #4
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #5
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #6
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #9
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #10
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #10
	bl FieldEvent_Wait
	movs r0, #128
	lsls r0, r0, #6
	movs r2, #0
	adds r0, #10
	movs r1, #0
	bl Func_0200525c
	movs r1, #2
	movs r0, #10
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl FieldEvent_Wait
	movs r0, #128
	lsls r0, r0, #6
	movs r2, #0
	adds r0, #10
	movs r1, #0
	bl Func_0200525c
	movs r1, #2
	movs r0, #9
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl FieldEvent_Wait
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #6
	adds r1, #255
	movs r2, #30
	movs r0, #10
	bl Func_0200528c
	movs r0, #128
	lsls r0, r0, #6
	movs r2, #0
	adds r0, #10
	movs r1, #0
	bl Func_0200525c
	movs r1, #2
	movs r0, #10
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl FieldEvent_Wait
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #2
	adds r1, #255
	movs r2, #30
	movs r0, #6
	bl Func_0200528c
	movs r0, #6
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #192
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #10
	bl Actor_ArmCallback
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #10
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #10
	bl FieldEvent_Wait
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #10
	bl Func_0200528c
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r0, #9
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #10
	bl FieldEvent_Wait
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #160
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #10
	bl Actor_ArmCallback
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #10
	movs r1, #4
	bl Actor_SetModeById
	movs r0, #33
	bl FieldEvent_Wait
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #128
	movs r0, #9
	lsls r1, r1, #6
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #160
	movs r0, #4
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #128
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl Actor_ArmCallback
	movs r0, #7
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200bf32
	movs r1, #128
	movs r0, #7
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
.L_0200bf32:
	movs r0, #40
	bl FieldEvent_Wait
	movs r1, #2
	movs r0, #10
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl FieldEvent_Wait
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r0, #7
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200bf6a
	movs r1, #160
	movs r0, #7
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
.L_0200bf6a:
	movs r1, #224
	movs r0, #9
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #192
	movs r0, #4
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #192
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #160
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #6
	bl Actor_ArmCallback
	movs r0, #25
	bl FieldEvent_Wait
	movs r1, #2
	movs r0, #9
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl FieldEvent_Wait
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #2
	adds r1, #255
	movs r2, #30
	movs r0, #10
	bl Func_0200528c
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r0, #9
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #10
	bl FieldEvent_Wait
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r0, #10
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #10
	bl FieldEvent_Wait
	movs r0, #10
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #10
	bl FieldEvent_Wait
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #10
	bl Func_0200528c
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r0, #9
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #10
	bl FieldEvent_Wait
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #128
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #9
	bl Actor_ArmCallback
	movs r0, #30
	bl FieldEvent_Wait
	movs r0, #9
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #10
	bl FieldEvent_Wait
	movs r2, #0
	movs r0, #9
	movs r1, #0
	bl Func_0200525c
	movs r1, #2
	movs r0, #10
	bl Motion_SetVarCbAndRefresh
	movs r0, #15
	bl FieldEvent_Wait
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #10
	bl Actor_ArmCallback
	movs r0, #30
	bl FieldEvent_Wait
	movs r0, #10
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #10
	bl FieldEvent_Wait
	movs r0, #128
	lsls r0, r0, #6
	movs r2, #0
	adds r0, #10
	movs r1, #0
	bl Func_0200525c
	movs r0, #4
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #5
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #30
	bl FieldEvent_Wait
	movs r1, #224
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #9
	bl Actor_ArmCallback
	movs r0, #25
	bl FieldEvent_Wait
	movs r0, #9
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #10
	bl FieldEvent_Wait
	movs r2, #0
	movs r0, #9
	movs r1, #0
	bl Func_0200525c
	movs r0, #10
	movs r1, #3
	bl Actor_StartRepeatedMotion
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #10
	bl Func_02005294
	movs r0, #40
	bl FieldEvent_Wait
	movs r1, #160
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #10
	bl Actor_ArmCallback
	movs r0, #30
	bl FieldEvent_Wait
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r0, #9
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #10
	bl FieldEvent_Wait
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #10
	bl Func_0200528c
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #0
	movs r2, #0
	movs r0, #9
	bl Actor_ArmCallback
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #16
	movs r2, #0
	movs r0, #9
	bl Actor_CommitPositionAndActivate
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #9
	movs r1, #16
	movs r2, #0
	bl Actor_CommitPositionAndActivate
	movs r1, #224
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #4
	bl Actor_ArmCallback
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #9
	movs r1, #16
	movs r2, #0
	bl Actor_CommitPositionAndActivate
	movs r1, #192
	movs r0, #10
	lsls r1, r1, #6
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #224
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #192
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r0, #7
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200c216
	movs r1, #192
	movs r0, #7
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
.L_0200c216:
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #9
	movs r1, #16
	movs r2, #0
	bl Actor_CommitPositionAndActivate
	movs r1, #224
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #6
	bl Actor_ArmCallback
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #172
	movs r1, #1
	movs r2, #208
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #16
	lsls r0, r0, #17
	bl Camera_MoveWithinMapBounds
	bl Func_020052b4
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #10
	ldr r1, .L_0200c430
	adds r2, #153
	bl Actor_SetSpeedParameters
	movs r0, #10
	movs r1, #16
	movs r2, #16
	bl Actor_CommitPositionAndActivate
	movs r1, #179
	movs r2, #178
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #10
	adds r1, #51
	adds r2, #153
	bl Actor_SetSpeedParameters
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r0, #9
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #10
	bl FieldEvent_Wait
	movs r0, #7
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200c2bc
	movs r1, #160
	movs r0, #7
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
.L_0200c2bc:
	movs r1, #224
	movs r0, #4
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #192
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #192
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #160
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #9
	bl Actor_ArmCallback
	movs r0, #30
	bl FieldEvent_Wait
	movs r2, #0
	movs r0, #9
	movs r1, #0
	bl Func_0200525c
	movs r1, #2
	movs r0, #10
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl FieldEvent_Wait
	movs r2, #0
	movs r0, #10
	movs r1, #0
	bl Func_0200525c
	movs r1, #2
	movs r0, #5
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl FieldEvent_Wait
	movs r2, #0
	movs r0, #5
	movs r1, #0
	bl Func_0200525c
	movs r1, #2
	movs r0, #10
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl FieldEvent_Wait
	movs r1, #160
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #10
	bl Actor_ArmCallback
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r0, #9
	movs r1, #4
	bl Actor_SetModeById
	movs r0, #33
	bl FieldEvent_Wait
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #128
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #10
	bl Actor_ArmCallback
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r0, #9
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #10
	bl FieldEvent_Wait
	movs r0, #20
	bl FieldEvent_Wait
	movs r1, #131
	lsls r1, r1, #1
	movs r2, #40
	movs r0, #6
	bl Func_0200528c
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #6
	bl Actor_ArmCallback
	movs r0, #20
	bl FieldEvent_Wait
	movs r1, #0
	movs r0, #6
	bl FieldMessage_ShowAtActor
	movs r1, #160
	movs r0, #9
	lsls r1, r1, #7
	movs r2, #0
	bl Actor_ArmCallback
	movs r0, #4
	movs r1, #0
	movs r2, #0
	bl Actor_ArmCallback
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl Actor_ArmCallback
	movs r0, #7
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200c400
	movs r1, #128
	movs r0, #7
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
.L_0200c400:
	movs r0, #4
	movs r1, #0
	bl Event_AskYesNoWithActorAnimation
	cmp r0, #0
	bne .L_0200c434
	movs r0, #25
	bl FieldEvent_Wait
	movs r2, #0
	movs r0, #6
	movs r1, #0
	bl Func_0200525c
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_0200c456
.L_0200c430:
	.4byte 0x00013333
.L_0200c434:
	movs r0, #40
	bl FieldEvent_Wait
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
	movs r2, #0
	bl Func_0200525c
.L_0200c456:
	movs r1, #2
	adds r1, #255
	movs r2, #0
	movs r0, #9
	bl Func_0200528c
	movs r1, #2
	movs r0, #5
	adds r1, #255
	movs r2, #0
	bl Func_0200528c
	movs r0, #7
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200c484
	movs r1, #2
	movs r0, #7
	adds r1, #255
	movs r2, #0
	bl Func_0200528c
.L_0200c484:
	movs r1, #2
	adds r1, #255
	movs r2, #50
	movs r0, #10
	bl Func_0200528c
	movs r1, #160
	movs r0, #9
	lsls r1, r1, #7
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #224
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #6
	bl Actor_ArmCallback
	movs r0, #30
	bl FieldEvent_Wait
	movs r0, #6
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #2
	adds r1, #255
	movs r2, #30
	movs r0, #9
	bl Func_0200528c
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #8
	adds r1, #255
	movs r2, #30
	movs r0, #6
	bl Func_0200528c
	movs r0, #6
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r0, #10
	movs r1, #4
	bl Actor_SetModeById
	movs r0, #33
	bl FieldEvent_Wait
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #6
	bl Actor_ArmCallback
	movs r0, #20
	bl FieldEvent_Wait
	movs r1, #4
	adds r1, #255
	movs r2, #30
	movs r0, #6
	bl Func_0200528c
	movs r0, #6
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r0, #6
	movs r1, #4
	bl Actor_SetModeById
	movs r0, #33
	bl FieldEvent_Wait
	movs r0, #6
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #6
	adds r1, #255
	movs r2, #0
	movs r0, #9
	bl Func_0200528c
	movs r1, #6
	movs r2, #50
	adds r1, #255
	movs r0, #10
	bl Func_0200528c
	movs r1, #2
	movs r0, #6
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl FieldEvent_Wait
	movs r2, #0
	movs r0, #6
	movs r1, #0
	bl Func_0200525c
	movs r0, #4
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #5
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #10
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #7
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200c596
	movs r0, #7
	movs r1, #3
	bl Actor_SetModeById
.L_0200c596:
	movs r1, #3
	movs r0, #9
	bl Actor_SetModeById
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #20
	bl FieldEvent_Wait
	movs r1, #2
	movs r0, #6
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl FieldEvent_Wait
	movs r2, #0
	movs r0, #6
	movs r1, #0
	bl Func_0200525c
	movs r0, #10
	movs r1, #3
	bl Actor_StartRepeatedMotion
	movs r1, #129
	movs r0, #10
	lsls r1, r1, #1
	bl Func_02005294
	movs r0, #9
	movs r1, #3
	bl Actor_StartRepeatedMotion
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #9
	bl Func_02005294
	movs r0, #40
	bl FieldEvent_Wait
	movs r1, #224
	movs r0, #4
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #224
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #160
	movs r0, #9
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #128
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #10
	bl Actor_ArmCallback
	movs r0, #25
	bl FieldEvent_Wait
	movs r0, #9
	movs r1, #6
	movs r2, #15
	bl Actor_Jump
	movs r2, #23
	movs r1, #6
	movs r0, #9
	bl Actor_Jump
	movs r0, #10
	bl FieldEvent_Wait
	movs r0, #9
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #10
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #10
	bl FieldEvent_Wait
	movs r3, #5
	movs r2, #17
	movs r1, #14
	movs r0, #20
	movs r4, #9
	str r3, [sp, #0]
	str r2, [sp, #8]
	str r1, [sp, #12]
	str r0, [sp, #16]
	movs r5, #0
	movs r0, #10
	movs r1, #9
	movs r2, #2
	movs r3, #9
	str r4, [sp, #4]
	str r4, [sp, #20]
	str r5, [sp, #24]
	bl Func_0200527c
	movs r0, #7
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200c694
	movs r1, #160
	movs r0, #7
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
.L_0200c694:
	movs r0, #10
	movs r1, #3
	bl Actor_StartRepeatedMotion
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #10
	bl Func_02005294
	movs r0, #40
	bl FieldEvent_Wait
	movs r2, #0
	movs r0, #10
	movs r1, #0
	bl Func_0200525c
	movs r0, #9
	movs r1, #3
	bl Actor_StartRepeatedMotion
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #9
	bl Func_02005294
	movs r0, #40
	bl FieldEvent_Wait
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #10
	bl Func_0200528c
	movs r2, #0
	movs r0, #10
	movs r1, #0
	bl Func_0200525c
	movs r1, #2
	movs r0, #10
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl FieldEvent_Wait
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r0, #10
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #10
	bl FieldEvent_Wait
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r0, #6
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #10
	bl FieldEvent_Wait
	movs r2, #0
	movs r0, #6
	movs r1, #0
	bl Func_0200525c
	movs r1, #2
	movs r0, #9
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl FieldEvent_Wait
	movs r1, #192
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #9
	bl Actor_ArmCallback
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #160
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #9
	bl Actor_ArmCallback
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r0, #10
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #10
	bl FieldEvent_Wait
	movs r1, #18
	movs r0, #9
	negs r1, r1
	movs r2, #0
	bl Actor_CommitPositionAndActivate
	movs r1, #192
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #160
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #9
	bl Actor_ArmCallback
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #9
	bl Actor_ArmCallback
	movs r0, #20
	bl FieldEvent_Wait
	movs r1, #160
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #10
	bl Actor_ArmCallback
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #10
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #5
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #10
	bl FieldEvent_Wait
	movs r0, #7
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200c880
	movs r1, #128
	movs r0, #9
	lsls r1, r1, #6
	movs r2, #0
	bl Actor_ArmCallback
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #160
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #10
	bl Actor_ArmCallback
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #10
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #7
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #10
	bl FieldEvent_Wait
	b .L_0200c892
.L_0200c880:
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
.L_0200c892:
	movs r1, #42
	movs r0, #9
	negs r1, r1
	movs r2, #0
	bl Actor_CommitPositionAndActivate
	movs r1, #192
	movs r0, #4
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #192
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #9
	bl Actor_ArmCallback
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #9
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #10
	bl FieldEvent_Wait
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #224
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #9
	bl Actor_ArmCallback
	movs r0, #20
	bl FieldEvent_Wait
	movs r1, #16
	movs r0, #10
	negs r1, r1
	movs r2, #0
	bl Actor_CommitPositionAndActivate
	movs r1, #160
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #160
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #10
	bl Actor_ArmCallback
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #10
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #30
	bl FieldEvent_Wait
	movs r0, #4
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #25
	bl FieldEvent_Wait
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r0, #9
	movs r1, #4
	bl Actor_SetModeById
	movs r0, #33
	bl FieldEvent_Wait
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #10
	bl Actor_ArmCallback
	movs r0, #40
	bl FieldEvent_Wait
	movs r0, #10
	bl Func_020013b0
	movs r0, #10
	bl Func_020013b0
	movs r0, #20
	bl FieldEvent_Wait
	movs r2, #0
	movs r1, #16
	movs r0, #10
	bl Actor_CommitPositionAndActivate
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #10
	movs r1, #3
	bl Actor_StartRepeatedMotion
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #10
	bl Func_02005294
	movs r0, #40
	bl FieldEvent_Wait
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #10
	bl Actor_ArmCallback
	movs r0, #20
	bl FieldEvent_Wait
	movs r1, #16
	negs r1, r1
	movs r2, #0
	movs r0, #10
	bl Actor_CommitPositionAndActivate
	movs r0, #10
	bl FieldEvent_Wait
	movs r1, #16
	negs r1, r1
	movs r2, #0
	movs r0, #10
	bl Actor_CommitPositionAndActivate
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #10
	movs r1, #4
	bl Actor_SetModeById
	movs r0, #33
	bl FieldEvent_Wait
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl Actor_ArmCallback
	movs r0, #20
	bl FieldEvent_Wait
	movs r1, #16
	movs r2, #0
	movs r0, #10
	bl Actor_CommitPositionAndActivate
	movs r0, #20
	bl FieldEvent_Wait
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #10
	bl Actor_ArmCallback
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #10
	movs r1, #3
	bl Actor_StartRepeatedMotion
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #10
	bl Func_02005294
	movs r0, #40
	bl FieldEvent_Wait
	movs r2, #12
	movs r1, #0
	negs r2, r2
	movs r0, #10
	bl Actor_CommitPositionAndActivate
	movs r0, #20
	bl FieldEvent_Wait
	movs r2, #12
	movs r1, #0
	negs r2, r2
	movs r0, #10
	bl Actor_CommitPositionAndActivate
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #20
	bl FieldEvent_Wait
	movs r1, #131
	lsls r1, r1, #1
	movs r2, #40
	movs r0, #10
	bl Func_0200528c
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #10
	bl Actor_ArmCallback
	movs r0, #20
	bl FieldEvent_Wait
	movs r2, #0
	movs r0, #10
	movs r1, #0
	bl Func_0200525c
	movs r1, #2
	movs r0, #10
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl FieldEvent_Wait
	movs r0, #10
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #5
	bl FieldEvent_Wait
	movs r0, #12
	bl Actor_GetById
	adds r5, r0, #0
	ldr r2, [r5, #80]
	movs r3, #204
	lsls r3, r3, #7
	adds r3, #102
	str r3, [r2, #12]
	movs r1, #1
	movs r0, #12
	mov r8, r2
	bl Actor_SetSpritePriority
	movs r0, #12
	bl Actor_GetById
	movs r1, #0
	bl Actor_SetSingleChildFlags
	movs r1, #164
	movs r2, #160
	movs r0, #12
	lsls r1, r1, #17
	lsls r2, r2, #16
	bl Func_0200520c
	movs r0, #12
	movs r1, #9
	movs r2, #0
	bl Actor_Jump
	movs r7, #0
	movs r6, #20
.L_0200caf6:
	adds r0, r7, #0
	movs r1, #20
	bl __divsi3
	movs r3, #204
	lsls r3, r3, #7
	adds r3, #102
	adds r0, r0, r3
	mov r2, r8
	str r0, [r2, #12]
	movs r2, #128
	ldrh r3, [r5, #6]
	lsls r2, r2, #5
	adds r3, r3, r2
	strh r3, [r5, #6]
	movs r0, #1
	bl wait
	movs r3, #153
	lsls r3, r3, #8
	adds r3, #153
	subs r6, #1
	adds r7, r7, r3
	cmp r6, #0
	bge .L_0200caf6
	movs r3, #128
	lsls r3, r3, #7
	strh r3, [r5, #6]
	movs r2, #8
	movs r1, #0
	movs r0, #12
	bl Actor_CommitPositionAndActivate
	movs r0, #12
	bl Actor_GetById
	movs r1, #1
	bl Actor_SetSingleChildFlags
	movs r0, #5
	bl FieldEvent_Wait
	movs r1, #0
	movs r2, #8
	movs r0, #12
	bl Actor_CommitPositionAndActivate
	movs r0, #5
	bl FieldEvent_Wait
	movs r1, #0
	movs r2, #8
	movs r0, #12
	bl Actor_CommitPositionAndActivate
	movs r0, #5
	bl FieldEvent_Wait
	movs r1, #0
	movs r2, #8
	movs r0, #12
	bl Actor_CommitPositionAndActivate
	movs r0, #5
	bl FieldEvent_Wait
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #5
	bl Func_0200528c
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #5
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #2
	adds r1, #255
	movs r2, #30
	movs r0, #10
	bl Func_0200528c
	movs r1, #192
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #10
	bl Actor_ArmCallback
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r0, #10
	movs r1, #0
	movs r2, #24
	bl Actor_CommitPositionAndActivate
	movs r1, #192
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #10
	bl Actor_ArmCallback
	movs r0, #10
	bl FieldEvent_Wait
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #160
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #10
	bl Actor_ArmCallback
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #128
	lsls r0, r0, #6
	movs r1, #0
	adds r0, #10
	bl FieldMessage_ShowAtActor
	movs r1, #192
	movs r0, #9
	lsls r1, r1, #6
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #128
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r0, #7
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200cc42
	movs r1, #128
	movs r0, #7
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
.L_0200cc42:
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #4
	movs r1, #0
	bl Event_AskYesNoWithActorAnimation
	cmp r0, #0
	bne .L_0200cc7c
	movs r0, #30
	bl FieldEvent_Wait
	movs r0, #128
	lsls r0, r0, #6
	movs r2, #0
	adds r0, #10
	movs r1, #0
	bl Func_0200525c
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_0200cca2
.L_0200cc7c:
	movs r0, #40
	bl FieldEvent_Wait
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #128
	adds r3, #1
	lsls r0, r0, #6
	strh r3, [r2]
	adds r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
.L_0200cca2:
	movs r1, #192
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #224
	movs r0, #9
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #160
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r0, #7
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200ccdc
	movs r1, #160
	movs r0, #7
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
.L_0200ccdc:
	movs r0, #30
	bl FieldEvent_Wait
	movs r1, #2
	movs r0, #10
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl FieldEvent_Wait
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	ldr r1, .L_0200d0d0
	ldr r2, .L_0200d0d4
	movs r0, #10
	bl Actor_SetSpeedParameters
	movs r0, #10
	bl Actor_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	movs r1, #160
	strb r3, [r0]
	lsls r1, r1, #7
	movs r0, #10
	movs r2, #0
	bl Actor_ArmCallback
	movs r0, #10
	movs r1, #0
	movs r2, #8
	bl Actor_CommitPositionAndActivate
	movs r2, #8
	movs r1, #0
	negs r2, r2
	movs r0, #10
	bl Actor_CommitPositionAndActivate
	movs r0, #1
	bl FieldEvent_Wait
	movs r0, #10
	bl Actor_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	strb r3, [r0]
	movs r0, #1
	bl FieldEvent_Wait
	movs r1, #179
	movs r2, #178
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #10
	adds r1, #51
	adds r2, #153
	bl Actor_SetSpeedParameters
	movs r1, #160
	movs r0, #10
	lsls r1, r1, #7
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #192
	movs r0, #9
	lsls r1, r1, #6
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r0, #7
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200cda2
	movs r1, #128
	movs r0, #7
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
.L_0200cda2:
	movs r1, #128
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r0, #12
	movs r1, #3
	movs r2, #9
	bl Func_0200532c
	movs r0, #1
	movs r1, #1
	movs r2, #1
	movs r3, #0
	negs r2, r2
	negs r0, r0
	negs r1, r1
	bl Camera_MoveWithinMapBounds
	movs r1, #1
	movs r0, #4
	bl Actor_SetModeById
	movs r0, #20
	bl FieldEvent_Wait
	movs r1, #192
	movs r0, #4
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #10
	bl Actor_ArmCallback
	movs r0, #20
	bl FieldEvent_Wait
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r1, #132
	adds r3, #1
	strh r3, [r2]
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #6
	bl Func_0200528c
	movs r0, #6
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #0
	movs r2, #0
	movs r0, #5
	bl Actor_ArmCallback
	movs r0, #30
	bl FieldEvent_Wait
	movs r0, #5
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #20
	bl FieldEvent_Wait
	movs r1, #224
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #9
	bl Actor_ArmCallback
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #160
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #5
	bl Actor_ArmCallback
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #10
	movs r1, #4
	bl Actor_SetModeById
	movs r0, #33
	bl FieldEvent_Wait
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r0, #9
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #10
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #10
	bl FieldEvent_Wait
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #5
	bl Func_0200528c
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #5
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #192
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #10
	bl Actor_ArmCallback
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #10
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #4
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #5
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #6
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #7
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200cf2a
	movs r0, #7
	movs r1, #3
	bl Actor_SetModeById
.L_0200cf2a:
	movs r1, #3
	movs r0, #9
	bl Actor_SetModeById
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #9
	movs r1, #12
	movs r2, #0
	bl Actor_CommitPositionAndActivate
	movs r1, #160
	movs r0, #10
	lsls r1, r1, #7
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #128
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #9
	bl Actor_ArmCallback
	movs r0, #20
	bl FieldEvent_Wait
	movs r1, #2
	movs r0, #9
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl FieldEvent_Wait
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r0, #4
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #7
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200cfbc
	movs r0, #7
	movs r1, #3
	bl Actor_SetModeById
.L_0200cfbc:
	movs r0, #5
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #6
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #4
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #20
	bl FieldEvent_Wait
	movs r2, #153
	lsls r2, r2, #8
	adds r2, #153
	movs r0, #9
	ldr r1, .L_0200d0d4
	bl Actor_SetSpeedParameters
	movs r0, #9
	movs r1, #2
	bl Actor_SetModeById
	ldr r3, .L_0200d0d8
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r3, r2
	ldr r0, [r5]
	bl Actor_GetById
	cmp r0, #0
	beq .L_0200d016
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #9
	bl Actor_ResetAndSetPosition
.L_0200d016:
	movs r0, #9
	bl Actor_WaitForMoveById
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_0200520c
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #5
	ldr r1, .L_0200d0d4
	adds r2, #153
	bl Actor_SetSpeedParameters
	movs r0, #5
	movs r1, #2
	bl Actor_SetModeById
	ldr r0, [r5]
	bl Actor_GetById
	cmp r0, #0
	beq .L_0200d054
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #5
	bl Actor_ResetAndSetPosition
.L_0200d054:
	movs r0, #5
	bl Actor_WaitForMoveById
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl Func_0200520c
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #6
	ldr r1, .L_0200d0d4
	adds r2, #153
	bl Actor_SetSpeedParameters
	movs r0, #6
	movs r1, #2
	bl Actor_SetModeById
	ldr r0, [r5]
	bl Actor_GetById
	cmp r0, #0
	beq .L_0200d092
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #6
	bl Actor_ResetAndSetPosition
.L_0200d092:
	movs r0, #6
	bl Actor_WaitForMoveById
	movs r0, #6
	movs r1, #0
	movs r2, #0
	bl Func_0200520c
	movs r0, #7
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200d0fa
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #7
	ldr r1, .L_0200d0d4
	adds r2, #153
	bl Actor_SetSpeedParameters
	movs r0, #7
	movs r1, #2
	bl Actor_SetModeById
	ldr r0, [r5]
	bl Actor_GetById
	cmp r0, #0
	beq .L_0200d0ea
	b .L_0200d0dc
	.2byte 0x0000
.L_0200d0d0:
	.4byte 0x00026666
.L_0200d0d4:
	.4byte 0x00013333
.L_0200d0d8:
	.4byte gSequenceParentWork
.L_0200d0dc:
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #7
	bl Actor_ResetAndSetPosition
.L_0200d0ea:
	movs r0, #7
	bl Actor_WaitForMoveById
	movs r0, #7
	movs r1, #0
	movs r2, #0
	bl Func_0200520c
.L_0200d0fa:
	ldr r3, .L_0200d118
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	movs r1, #1
	bl Camera_FollowActor
	bl Func_020051b4
	add sp, #28
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200d118:
	.4byte gSequenceParentWork
	.section .bss,"aw",%nobits
	.space 0x0000000c
	.global Data_02006340
Data_02006340:
