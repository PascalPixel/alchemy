.syntax unified
	.thumb
	.section .text.x02008870,"ax",%progbits
	.balign 4
	.global Func_02000870
	.thumb_func
Func_02000870:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #131
	sub sp, #8
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200888c
	b .L_02008ad8
.L_0200888c:
	ldr r3, .L_02008ae4
	movs r2, #133
	lsls r2, r2, #2
	adds r7, r3, r2
	ldr r0, [r7]
	bl Actor_GetById
	adds r5, r0, #0
	bl Func_02002c68
	movs r0, #0
	bl Func_02002da8
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	ldr r0, [r7]
	adds r1, #204
	adds r2, #102
	bl Actor_SetSpeedParameters
	movs r1, #210
	ldr r0, [r7]
	lsls r1, r1, #2
	movs r2, #72
	bl Actor_SetPositionAndReset
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #50
	bl GameFlag_Test
	adds r5, #34
	mov r9, r0
	mov r10, r5
	cmp r0, #0
	beq .L_02008972
	movs r0, #243
	bl Func_02002de0
	movs r1, #129
	ldr r0, [r7]
	lsls r1, r1, #1
	bl Func_02002d48
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #10
	lsls r2, r2, #9
	movs r0, #0
	bl Func_02002c48
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl Func_02002c48
	movs r5, #3
	movs r1, #68
	movs r0, #68
	movs r2, #50
	movs r3, #64
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl Func_02002c30
	movs r3, #2
	mov r2, r10
	strb r3, [r2]
	movs r0, #20
	bl FieldEvent_Wait
	ldr r0, [r7]
	movs r1, #4
	movs r2, #0
	bl Actor_Jump
	movs r2, #204
	lsls r2, r2, #8
	ldr r0, [r7]
	ldr r1, .L_02008ae8
	adds r2, #204
	bl Actor_SetSpeedParameters
	movs r1, #210
	lsls r1, r1, #2
	movs r2, #88
	ldr r0, [r7]
	bl Actor_SetPositionAndCommit
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #232
	bl Func_02002de0
	movs r2, #50
	movs r3, #64
	movs r0, #72
	movs r1, #68
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl Func_02002c30
	movs r0, #20
	bl FieldEvent_Wait
	movs r3, #1
	mov r2, r10
	strb r3, [r2]
	b .L_02008ad4
.L_02008972:
	movs r0, #9
	bl Actor_GetById
	movs r1, #2
	adds r6, r0, #0
	movs r0, #9
	bl Actor_SetSpritePriority
	ldr r1, .L_02008aec
	ldr r2, .L_02008af0
	movs r0, #9
	bl Actor_SetSpeedParameters
	movs r0, #9
	bl Actor_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	movs r1, #206
	strb r3, [r0]
	lsls r1, r1, #2
	movs r2, #88
	movs r0, #9
	bl Actor_SetPositionAndReset
	movs r0, #1
	bl FieldEvent_Wait
	movs r0, #9
	bl Actor_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	strb r3, [r0]
	movs r0, #243
	bl Func_02002de0
	movs r1, #129
	ldr r0, [r7]
	lsls r1, r1, #1
	bl Func_02002d48
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #10
	lsls r2, r2, #9
	movs r0, #0
	bl Func_02002c48
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl Func_02002c48
	movs r5, #3
	movs r1, #68
	movs r0, #68
	movs r2, #50
	movs r3, #64
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl Func_02002c30
	movs r3, #2
	mov r8, r3
	mov r2, r8
	mov r3, r10
	strb r2, [r3]
	movs r0, #20
	bl FieldEvent_Wait
	movs r1, #192
	ldr r0, [r7]
	lsls r1, r1, #7
	movs r2, #20
	bl Actor_ArmCallback
	ldr r0, [r7]
	movs r1, #4
	movs r2, #0
	bl Actor_Jump
	movs r2, #204
	lsls r2, r2, #8
	ldr r0, [r7]
	ldr r1, .L_02008ae8
	adds r2, #204
	bl Actor_SetSpeedParameters
	movs r1, #210
	movs r2, #88
	ldr r0, [r7]
	lsls r1, r1, #2
	bl Actor_SetPositionAndCommit
	movs r1, #128
	ldr r0, [r7]
	lsls r1, r1, #8
	bl Func_02002d30
	ldr r0, [r7]
	movs r1, #9
	bl Actor_WatchAndSetScript
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r6, #72]
	mov r2, r8
	adds r6, #34
	strb r2, [r6]
	movs r0, #9
	movs r1, #4
	movs r2, #0
	bl Actor_Jump
	movs r1, #208
	movs r0, #9
	lsls r1, r1, #2
	movs r2, #74
	bl Actor_SetPositionAndCommit
	movs r0, #9
	movs r1, #4
	movs r2, #0
	bl Actor_Jump
	movs r1, #214
	movs r2, #88
	movs r0, #9
	lsls r1, r1, #2
	bl Actor_SetPositionAndCommit
	movs r1, #129
	lsls r1, r1, #1
	ldr r0, [r7]
	bl Func_02002d48
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #232
	bl Func_02002de0
	movs r1, #68
	movs r2, #50
	movs r3, #64
	movs r0, #72
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl Func_02002c30
	movs r0, #20
	bl FieldEvent_Wait
	ldr r0, [r7]
	bl Actor_Stop
	movs r3, #1
	mov r2, r10
	strb r3, [r2]
	mov r3, r9
	movs r0, #1
	strb r3, [r6]
	bl wait
	movs r0, #129
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_ClearBit
.L_02008ad4:
	bl Func_02002c70
.L_02008ad8:
	add sp, #8
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_02008ae4:
	.4byte gMainRoutineWork
.L_02008ae8:
	.4byte 0x00019999
.L_02008aec:
	.4byte 0x00026666
.L_02008af0:
	.4byte 0x00013333
