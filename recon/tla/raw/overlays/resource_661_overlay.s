.syntax unified
	.thumb
	.section .text.x020093f8,"ax",%progbits
	.global Func_020013f8
	.thumb_func
Func_020013f8:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r3, .L_02009514
	movs r2, #133
	lsls r2, r2, #2
	adds r2, r2, r3
	ldr r0, [r2]
	sub sp, #8
	mov r8, r2
	bl Actor_GetById
	ldr r3, [r0, #8]
	asrs r7, r3, #20
	ldr r3, [r0, #16]
	movs r0, #64
	asrs r6, r3, #20
	bl Actor_GetById
	cmp r0, #0
	beq .L_0200942c
	movs r0, #64
	movs r1, #0
	movs r2, #0
	bl Func_02004090
.L_0200942c:
	movs r3, #6
	str r3, [sp, #4]
	movs r5, #5
	movs r0, #82
	movs r1, #89
	movs r2, #15
	movs r3, #40
	str r5, [sp, #0]
	bl Func_02003ff0
	movs r3, #15
	movs r2, #40
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #82
	movs r1, #89
	movs r2, #5
	movs r3, #6
	bl Func_02004000
	movs r0, #88
	movs r1, #89
	movs r2, #36
	movs r3, #25
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl Func_02003ff0
	movs r3, #36
	movs r2, #25
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #88
	movs r1, #89
	movs r2, #5
	movs r3, #5
	bl Func_02004000
	cmp r7, #15
	bne .L_020094a0
	cmp r6, #44
	bne .L_020094a0
	movs r0, #15
	bl Func_020012d4
	adds r5, r0, #0
	movs r0, #43
	bl Func_020012d4
	lsls r5, r5, #16
	adds r2, r0, #0
	mov r3, r8
	ldr r0, [r3]
	lsls r2, r2, #16
	adds r1, r5, #0
	bl Func_02004090
	b .L_0200950a
.L_020094a0:
	cmp r7, #17
	bne .L_020094b4
	cmp r6, #44
	bne .L_020094b4
	movs r0, #17
	bl Func_020012d4
	adds r6, r0, #0
	movs r0, #43
	b .L_020094c6
.L_020094b4:
	cmp r7, #19
	bne .L_0200950a
	cmp r6, #40
	bne .L_020094e2
	movs r0, #19
	bl Func_020012d4
	adds r6, r0, #0
	movs r0, #41
.L_020094c6:
	bl Func_020012d4
	ldr r5, .L_02009514
	movs r3, #133
	lsls r3, r3, #2
	adds r2, r0, #0
	adds r5, r5, r3
	lsls r6, r6, #16
	ldr r0, [r5]
	lsls r2, r2, #16
	adds r1, r6, #0
	bl Func_02004090
	b .L_0200950a
.L_020094e2:
	cmp r6, #42
	bne .L_0200950a
	movs r0, #18
	bl Func_020012d4
	adds r6, r0, #0
	movs r0, #42
	bl Func_020012d4
	ldr r5, .L_02009514
	movs r3, #133
	lsls r3, r3, #2
	adds r2, r0, #0
	adds r5, r5, r3
	lsls r6, r6, #16
	ldr r0, [r5]
	lsls r2, r2, #16
	adds r1, r6, #0
	bl Func_02004090
.L_0200950a:
	add sp, #8
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009514:
	.4byte gMainRoutineWork
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
