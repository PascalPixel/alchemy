.syntax unified
	.thumb
	.section .text.x0200aebc,"ax",%progbits
	.global Func_02002ebc
	.thumb_func
Func_02002ebc:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r1, .L_0200b084
	movs r5, #0
	ldr r0, [r1]
	bl Actor_GetById
	adds r6, r0, #0
	movs r0, #8
	bl Actor_GetById
	mov r8, r0
	bl Func_02004040
	movs r0, #0
	bl Func_020041b8
	movs r0, #78
	bl Func_020041f8
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r0, r0
	negs r1, r1
	negs r2, r2
	movs r3, #0
	bl Camera_MoveWithinMapBounds
	movs r0, #232
	movs r1, #1
	movs r2, #184
	movs r3, #0
	negs r1, r1
	lsls r2, r2, #16
	lsls r0, r0, #16
	bl Camera_MoveWithinMapBounds
	bl Func_02003fd8
	movs r0, #1
	bl wait
	ldr r2, .L_0200b084
	adds r7, r6, #0
	ldr r0, [r2]
	bl Actor_GetById
	movs r1, #0
	bl Actor_SetSingleChildFlags
	movs r3, #130
	movs r1, #128
	lsls r3, r3, #16
	lsls r1, r1, #8
	adds r7, #85
	strb r5, [r7]
	movs r0, #8
	str r3, [r6, #12]
	str r1, [r6, #72]
	str r5, [r6, #68]
	mov r9, r3
	mov r10, r1
	bl Actor_GetById
	movs r1, #0
	bl Actor_SetSingleChildFlags
	movs r3, #128
	movs r1, #232
	movs r2, #148
	lsls r3, r3, #7
	movs r0, #8
	lsls r1, r1, #16
	lsls r2, r2, #16
	bl Func_02004098
	movs r2, #85
	add r2, r8
	strb r5, [r2]
	mov r1, r8
	mov r3, r9
	mov r11, r2
	mov r2, r10
	movs r0, #1
	str r3, [r1, #12]
	str r2, [r1, #72]
	str r5, [r1, #68]
	bl wait
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r1, #214
	movs r2, #130
	lsls r1, r1, #1
	lsls r2, r2, #1
	adds r3, r3, r1
	adds r2, #255
	str r2, [r3]
	bl Event_StartScreenTransition
	movs r5, #3
	bl Event_WaitTransitionFrames
	movs r0, #204
	bl Func_020041f8
	ldr r2, .L_0200b084
	strb r5, [r7]
	ldr r0, [r2]
	bl Actor_GetById
	movs r1, #1
	bl Actor_SetSingleChildFlags
	movs r0, #24
	bl FieldEvent_Wait
	ldr r2, [r6, #16]
	ldr r1, [r6, #12]
	ldr r0, [r6, #8]
	bl Func_02000b84
	movs r0, #188
	bl Func_020041f8
	ldr r3, .L_0200b084
	movs r1, #2
	ldr r0, [r3]
	adds r1, #255
	bl Func_02004108
	ldr r1, .L_0200b084
	ldr r0, [r1]
	movs r1, #49
	bl Motion_SetModeAndWaitAnimation
	movs r0, #204
	bl Func_020041f8
	mov r2, r11
	strb r5, [r2]
	movs r0, #8
	bl Actor_GetById
	movs r1, #1
	bl Actor_SetSingleChildFlags
	movs r0, #24
	bl FieldEvent_Wait
	mov r3, r8
	ldr r0, [r3, #8]
	ldr r1, [r3, #12]
	ldr r2, [r3, #16]
	bl Func_02000b84
	movs r0, #188
	bl Func_020041f8
	ldr r1, .L_0200b084
	movs r2, #0
	ldr r0, [r1]
	movs r1, #192
	lsls r1, r1, #8
	bl Actor_ArmCallback
	movs r0, #40
	bl FieldEvent_Wait
	movs r0, #147
	bl Func_020041f8
	movs r1, #3
	movs r0, #8
	bl Actor_SetModeById
	movs r0, #20
	bl FieldEvent_Wait
	movs r3, #128
	mov r2, r8
	lsls r3, r3, #12
	str r3, [r2, #40]
	movs r3, #230
	lsls r3, r3, #8
	adds r3, #102
	str r3, [r2, #72]
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #8
	ldr r1, .L_0200b088
	adds r2, #204
	bl Actor_SetSpeedParameters
	movs r0, #8
	movs r1, #232
	movs r2, #184
	bl Actor_SetPositionAndCommit
	ldr r3, .L_0200b08c
	movs r1, #166
	lsls r1, r1, #1
	adds r1, #255
	adds r2, r3, r1
	movs r3, #2
	strb r3, [r2]
	ldr r0, .L_0200b090
	movs r1, #2
	bl Party_SetFields1eeAnd1f0
	movs r0, #10
	movs r1, #0
	bl Func_02004140
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200b084:
	.4byte Data_02000454
.L_0200b088:
	.4byte 0x00019999
.L_0200b08c:
	.4byte gMainRoutineWork
.L_0200b090:
	.4byte 0x0000004c
