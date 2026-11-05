.syntax unified
	.thumb
	.section .text.x02009d24,"ax",%progbits
	.balign 4
	.global Func_02001d24
	.thumb_func
Func_02001d24:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #115
	bl GameFlag_Test
	adds r6, r0, #0
	cmp r6, #0
	beq .L_02009d40
	b .L_0200a372
.L_02009d40:
	bl Func_020039d8
	movs r0, #0
	bl Func_02003b08
	bl Func_02003ad0
	adds r0, #85
	strb r6, [r0]
	movs r1, #1
	movs r0, #170
	movs r2, #176
	movs r3, #1
	lsls r0, r0, #17
	negs r1, r1
	lsls r2, r2, #16
	bl Camera_MoveWithinMapBounds
	ldr r1, .L_0200a160
	movs r2, #133
	lsls r2, r2, #2
	adds r1, r1, r2
	mov r9, r1
	ldr r0, [r1]
	movs r2, #204
	movs r1, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	adds r1, #204
	adds r2, #102
	bl Actor_SetSpeedParameters
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #14
	adds r1, #204
	adds r2, #102
	bl Actor_SetSpeedParameters
	mov r3, r9
	movs r1, #172
	ldr r0, [r3]
	lsls r1, r1, #1
	movs r2, #184
	bl Actor_SetPositionAndReset
	mov r1, r9
	ldr r0, [r1]
	movs r1, #128
	lsls r1, r1, #6
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #172
	movs r2, #184
	lsls r1, r1, #17
	lsls r2, r2, #16
	movs r0, #14
	bl Func_02003a30
	movs r0, #1
	bl wait
	movs r1, #178
	movs r2, #196
	movs r0, #14
	lsls r1, r1, #1
	bl Actor_SetPositionAndReset
	movs r1, #176
	lsls r1, r1, #8
	movs r0, #14
	bl Func_02003a90
	ldr r0, .L_0200a164
	bl Func_02003a70
	movs r0, #14
	movs r1, #0
	bl Func_02003a80
	mov r2, r9
	ldr r0, [r2]
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #13
	movs r1, #0
	bl Func_02003a80
	movs r1, #184
	movs r2, #168
	movs r0, #13
	lsls r1, r1, #16
	lsls r2, r2, #16
	bl Func_02003a30
	mov r3, r9
	movs r1, #128
	ldr r0, [r3]
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #128
	movs r2, #0
	movs r0, #14
	lsls r1, r1, #8
	bl Actor_ArmCallback
	movs r1, #153
	lsls r1, r1, #8
	ldr r0, .L_0200a168
	adds r1, #153
	bl Func_02003ab8
	movs r0, #232
	movs r1, #128
	movs r2, #176
	movs r3, #1
	lsls r0, r0, #16
	lsls r1, r1, #15
	lsls r2, r2, #16
	bl Camera_MoveWithinMapBounds
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #13
	ldr r1, .L_0200a16c
	adds r2, #204
	bl Actor_SetSpeedParameters
	movs r2, #168
	movs r1, #232
	movs r0, #13
	bl Actor_SetPositionAndReset
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #13
	movs r1, #0
	bl Func_02003a80
	movs r1, #152
	lsls r1, r1, #6
	ldr r0, .L_0200a170
	adds r1, #102
	bl Func_02003ab8
	movs r0, #174
	movs r1, #1
	movs r2, #216
	movs r3, #1
	lsls r2, r2, #16
	negs r1, r1
	lsls r0, r0, #17
	bl Camera_MoveWithinMapBounds
	movs r0, #13
	bl Actor_GetById
	adds r5, r0, #0
	ldr r1, [r5, #80]
	movs r0, #13
	mov r8, r1
	movs r1, #1
	bl Actor_SetSpritePriority
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #13
	ldr r1, .L_0200a16c
	adds r2, #204
	bl Actor_SetSpeedParameters
	movs r0, #13
	movs r1, #4
	movs r2, #0
	bl Actor_Jump
	movs r2, #168
	movs r0, #13
	movs r1, #248
	bl Actor_SetPositionAndCommit
	movs r0, #13
	movs r1, #6
	bl Actor_SetModeById
	movs r2, #208
	ldr r3, .L_0200a174
	lsls r2, r2, #8
	mov r10, r2
	adds r7, r5, #0
	mov r2, r8
	mov r1, r10
	adds r7, #85
	str r3, [r5, #24]
	strh r1, [r2, #18]
	strb r3, [r7]
	movs r1, #192
	ldr r3, [r5, #12]
	lsls r1, r1, #11
	adds r3, r3, r1
	str r3, [r5, #12]
	movs r0, #13
	bl Actor_GetById
	movs r1, #0
	bl Actor_SetSingleChildFlags
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #7
	lsls r2, r2, #6
	movs r0, #13
	adds r1, #102
	adds r2, #51
	bl Actor_SetSpeedParameters
	movs r1, #164
	movs r2, #152
	movs r3, #168
	adds r0, r5, #0
	lsls r1, r1, #17
	lsls r2, r2, #14
	lsls r3, r3, #16
	bl Func_02003990
	movs r0, #13
	bl Actor_WaitForMoveById
	movs r3, #3
	strb r3, [r7]
	movs r3, #128
	lsls r3, r3, #14
	str r3, [r5, #20]
	movs r3, #128
	lsls r3, r3, #9
	mov r2, r8
	str r3, [r5, #24]
	movs r1, #1
	strh r6, [r2, #18]
	movs r0, #13
	bl Actor_SetModeById
	movs r0, #13
	bl Actor_GetById
	movs r1, #1
	bl Actor_SetSingleChildFlags
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #13
	ldr r1, .L_0200a16c
	adds r2, #204
	bl Actor_SetSpeedParameters
	movs r0, #13
	movs r1, #4
	movs r2, #0
	bl Actor_Jump
	movs r1, #164
	movs r0, #13
	lsls r1, r1, #1
	movs r2, #184
	bl Actor_SetPositionAndCommit
	movs r0, #13
	movs r1, #0
	movs r2, #20
	bl Actor_ArmCallback
	movs r1, #128
	movs r2, #40
	lsls r1, r1, #1
	movs r0, #13
	bl Func_02003aa8
	movs r0, #13
	movs r1, #2
	bl Actor_SetSpritePriority
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #13
	movs r1, #0
	bl Func_02003a80
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #14
	movs r1, #0
	bl Func_02003a80
	movs r1, #2
	movs r2, #20
	adds r1, #255
	movs r0, #13
	bl Func_02003aa8
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #13
	movs r1, #0
	bl Func_02003a80
	movs r1, #176
	movs r0, #14
	lsls r1, r1, #8
	movs r2, #40
	bl Actor_ArmCallback
	movs r1, #128
	movs r0, #14
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r0, #128
	lsls r0, r0, #6
	movs r2, #60
	adds r0, #14
	movs r1, #0
	bl Func_02003a78
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #13
	movs r1, #0
	bl Func_02003a80
	movs r0, #13
	movs r1, #3
	bl Actor_SetModeById
	movs r1, #197
	movs r2, #143
	lsls r1, r1, #17
	lsls r2, r2, #17
	movs r0, #12
	bl Func_02003a30
	movs r0, #1
	bl wait
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #12
	ldr r1, .L_0200a16c
	adds r2, #204
	bl Actor_SetSpeedParameters
	movs r1, #197
	movs r0, #12
	lsls r1, r1, #1
	movs r2, #248
	bl Actor_SetPositionAndReset
	movs r1, #176
	movs r0, #12
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r2, #20
	movs r0, #12
	movs r1, #4
	bl Actor_Jump
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #12
	movs r1, #0
	bl Func_02003a80
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #13
	bl Func_02003aa8
	movs r1, #192
	movs r0, #13
	lsls r1, r1, #6
	movs r2, #0
	bl Actor_ArmCallback
	mov r3, r9
	movs r1, #128
	ldr r0, [r3]
	lsls r1, r1, #6
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #192
	movs r2, #0
	movs r0, #14
	lsls r1, r1, #6
	bl Actor_ArmCallback
	movs r1, #153
	lsls r1, r1, #8
	ldr r0, .L_0200a168
	adds r1, #153
	bl Func_02003ab8
	movs r0, #188
	movs r1, #1
	movs r2, #132
	lsls r2, r2, #17
	movs r3, #1
	lsls r0, r0, #17
	negs r1, r1
	bl Camera_MoveWithinMapBounds
	bl Func_02003ac8
	movs r0, #12
	movs r1, #0
	bl Func_02003a80
	movs r1, #152
	lsls r1, r1, #7
	ldr r0, .L_0200a178
	adds r1, #204
	bl Func_02003ab8
	movs r0, #174
	movs r1, #1
	movs r2, #216
	movs r3, #1
	lsls r2, r2, #16
	lsls r0, r0, #17
	negs r1, r1
	bl Camera_MoveWithinMapBounds
	mov r1, r9
	ldr r0, [r1]
	movs r1, #12
	bl Actor_WatchAndSetScript
	movs r0, #14
	movs r1, #12
	bl Actor_WatchAndSetScript
	movs r0, #13
	movs r1, #12
	bl Actor_WatchAndSetScript
	movs r1, #197
	movs r0, #12
	lsls r1, r1, #1
	movs r2, #220
	bl Actor_SetPositionAndReset
	movs r1, #185
	movs r0, #12
	lsls r1, r1, #1
	movs r2, #198
	bl Actor_SetPositionAndReset
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #14
	ldr r1, .L_0200a16c
	adds r2, #204
	bl Actor_SetSpeedParameters
	movs r1, #184
	movs r2, #180
	movs r0, #14
	lsls r1, r1, #1
	bl Actor_SetPositionAndCommit
	movs r0, #14
	movs r1, #12
	bl Actor_WatchAndSetScript
	movs r1, #168
	movs r0, #12
	lsls r1, r1, #1
	movs r2, #198
	bl Actor_SetPositionAndReset
	movs r1, #176
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #12
	bl Actor_ArmCallback
	movs r0, #13
	bl Actor_Stop
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #13
	movs r1, #0
	bl Func_02003a80
	movs r2, #0
	movs r0, #12
	mov r1, r10
	bl Actor_ArmCallback
	movs r0, #12
	movs r1, #0
	bl Func_02003a80
	movs r1, #176
	movs r0, #12
	lsls r1, r1, #8
	bl Func_02003a90
	movs r0, #12
	movs r1, #4
	movs r2, #20
	bl Actor_Jump
	movs r0, #12
	movs r1, #0
	movs r2, #20
	b .L_0200a17c
.L_0200a160:
	.4byte gMainRoutineWork
.L_0200a164:
	.4byte MsgFieldPraiseRopeWork
.L_0200a168:
	.4byte 0x0004cccc
.L_0200a16c:
	.4byte 0x00019999
.L_0200a170:
	.4byte 0x00013333
.L_0200a174:
	.4byte 0xffff0000
.L_0200a178:
	.4byte 0x00026666
.L_0200a17c:
	bl Func_02003a78
	movs r1, #6
	adds r1, #255
	movs r2, #40
	movs r0, #13
	bl Func_02003aa8
	movs r1, #2
	movs r2, #60
	adds r1, #255
	movs r0, #12
	bl Func_02003aa8
	movs r0, #12
	movs r1, #0
	bl Func_02003a80
	movs r0, #13
	movs r1, #4
	bl Actor_SetModeById
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #13
	movs r1, #0
	bl Func_02003a80
	movs r1, #128
	movs r2, #20
	lsls r1, r1, #1
	movs r0, #12
	bl Func_02003aa8
	movs r0, #12
	movs r1, #0
	bl Func_02003a80
	movs r1, #128
	movs r2, #20
	movs r0, #13
	lsls r1, r1, #8
	bl Actor_ArmCallback
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #13
	movs r1, #0
	bl Func_02003a80
	movs r1, #128
	movs r2, #0
	movs r0, #12
	lsls r1, r1, #8
	bl Actor_ArmCallback
	movs r0, #12
	movs r1, #0
	bl Func_02003a80
	movs r0, #13
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #13
	movs r1, #0
	bl Func_02003a80
	movs r1, #10
	movs r2, #40
	adds r1, #255
	movs r0, #12
	bl Func_02003aa8
	movs r0, #12
	movs r1, #0
	bl Func_02003a80
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #12
	bl Func_02003aa8
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #13
	bl Func_02003aa8
	movs r1, #176
	movs r0, #12
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #192
	movs r0, #13
	lsls r1, r1, #6
	movs r2, #40
	bl Actor_ArmCallback
	movs r2, #40
	movs r0, #13
	movs r1, #4
	bl Actor_Jump
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #13
	movs r1, #0
	bl Func_02003a80
	movs r0, #12
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #12
	movs r1, #0
	bl Func_02003a80
	movs r0, #13
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #13
	movs r1, #0
	bl Func_02003a80
	movs r0, #12
	movs r1, #0
	bl Func_02003a80
	movs r0, #12
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #13
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #12
	ldr r1, .L_0200a37c
	adds r2, #153
	bl Actor_SetSpeedParameters
	movs r2, #153
	lsls r2, r2, #8
	adds r2, #153
	movs r0, #13
	ldr r1, .L_0200a37c
	bl Actor_SetSpeedParameters
	ldr r5, .L_0200a380
	movs r0, #12
	adds r1, r5, #0
	bl Actor_SetMovementTypeById
	movs r0, #10
	bl FieldEvent_Wait
	adds r1, r5, #0
	movs r0, #13
	bl Actor_SetMovementTypeAndWaitById
	movs r0, #40
	bl FieldEvent_Wait
	mov r2, r9
	ldr r0, [r2]
	bl Actor_Stop
	movs r0, #14
	bl Actor_Stop
	movs r0, #1
	bl wait
	movs r0, #14
	movs r1, #0
	movs r2, #40
	bl Func_02003a78
	movs r0, #13
	movs r1, #0
	movs r2, #0
	bl Func_02003a30
	movs r2, #0
	movs r0, #12
	movs r1, #0
	bl Func_02003a30
	movs r0, #14
	movs r1, #0
	bl Func_02003a80
	mov r3, r9
	ldr r0, [r3]
	movs r1, #0
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #128
	movs r2, #20
	movs r0, #14
	lsls r1, r1, #8
	bl Actor_ArmCallback
	movs r0, #14
	movs r1, #0
	bl Func_02003a80
	movs r0, #14
	movs r1, #2
	bl Actor_SetModeById
	mov r1, r9
	ldr r0, [r1]
	bl Actor_GetById
	cmp r0, #0
	beq .L_0200a354
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #14
	bl Actor_ResetAndSetPosition
.L_0200a354:
	movs r0, #14
	bl Actor_WaitForMoveById
	movs r0, #14
	movs r1, #0
	movs r2, #0
	bl Func_02003a30
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #115
	bl GameFlag_SetBit
	bl Func_020039e0
.L_0200a372:
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_0200a37c:
	.4byte 0x00013333
.L_0200a380:
	.4byte gData_02003c58
	.section .text.x0200ad50,"ax",%progbits
	.balign 4
	.global Func_02002d50
	.thumb_func
Func_02002d50:
	push {r5, lr}
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #124
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200ad62
	b .L_0200ae6a
.L_0200ad62:
	movs r0, #115
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200ae6a
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #108]
	movs r0, #14
	bl Actor_GetById
	movs r2, #170
	lsls r2, r2, #1
	adds r5, r5, r2
	movs r2, #0
	ldrsh r3, [r5, r2]
	subs r3, #13
	cmp r3, #7
	bhi .L_0200ae6a
	ldr r2, .L_0200ae6c
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	adds r0, #98
	mov pc, r3
	.2byte 0x0000
.L_0200ad94:
	.4byte .L_0200adb4
	.4byte .L_0200adca
	.4byte .L_0200add6
	.4byte .L_0200adfa
	.4byte .L_0200ae10
	.4byte .L_0200ae26
	.4byte .L_0200ae4a
	.4byte .L_0200ae56
.L_0200adb4:
	ldrb r3, [r0]
	cmp r3, #0
	bne .L_0200adbe
	bl Func_020027f8
.L_0200adbe:
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #6
	bl GameFlag_ClearBit
	b .L_0200ae6a
.L_0200adca:
	ldrb r3, [r0]
	cmp r3, #0
	bne .L_0200ae6a
	bl Func_020029a4
	b .L_0200ae6a
.L_0200add6:
	ldrb r0, [r0]
	cmp r0, #1
	bne .L_0200ade2
	bl Func_02002830
	b .L_0200ae6a
.L_0200ade2:
	cmp r0, #4
	bne .L_0200ae6a
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #6
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200ae6a
	bl Func_02002b50
	b .L_0200ae6a
.L_0200adfa:
	ldrb r0, [r0]
	cmp r0, #1
	bne .L_0200ae06
	bl Func_02002b18
	b .L_0200ae6a
.L_0200ae06:
	cmp r0, #5
	bne .L_0200ae6a
	bl Func_02002c50
	b .L_0200ae6a
.L_0200ae10:
	ldrb r0, [r0]
	cmp r0, #2
	bne .L_0200ae1c
	bl Func_0200291c
	b .L_0200ae6a
.L_0200ae1c:
	cmp r0, #4
	bne .L_0200ae6a
	bl Func_02002c18
	b .L_0200ae6a
.L_0200ae26:
	ldrb r0, [r0]
	cmp r0, #2
	bne .L_0200ae32
	bl Func_02002a2c
	b .L_0200ae6a
.L_0200ae32:
	cmp r0, #5
	bne .L_0200ae6a
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #6
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200ae6a
	bl Func_02002c88
	b .L_0200ae6a
.L_0200ae4a:
	ldrb r3, [r0]
	cmp r3, #3
	bne .L_0200ae6a
	bl Func_0200296c
	b .L_0200ae6a
.L_0200ae56:
	ldrb r3, [r0]
	cmp r3, #3
	bne .L_0200ae60
	bl Func_020029dc
.L_0200ae60:
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #6
	bl GameFlag_ClearBit
.L_0200ae6a:
	pop {r5, pc}
.L_0200ae6c:
	.4byte .L_0200ad94
	.section .bss,"aw",%nobits
	.global gData_02004a18
gData_02004a18:
