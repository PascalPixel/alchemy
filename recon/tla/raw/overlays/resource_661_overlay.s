.syntax unified
	.thumb
	.section .text.x0200807e,"ax",%progbits
	.2byte 0x0000
	.section .text.x020080b6,"ax",%progbits
	.2byte 0x0000
	.section .text.x020086ac,"ax",%progbits
	.global Func_020006ac
	.thumb_func
Func_020006ac:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	sub sp, #8
	str r3, [sp, #4]
	ldr r3, .L_020087bc
	movs r2, #240
	lsls r2, r2, #1
	movs r1, #0
	adds r3, r3, r2
	mov r11, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_020087c0
	cmp r2, r3
	bne .L_020086e0
	ldr r2, .L_020087c4
	mov r11, r2
	b .L_020086f6
.L_020086e0:
	ldr r3, .L_020087c8
	cmp r2, r3
	bne .L_020086ec
	ldr r3, .L_020087cc
	mov r11, r3
	b .L_020086f6
.L_020086ec:
	ldr r3, .L_020087d0
	cmp r2, r3
	bne .L_020086f6
	ldr r1, .L_020087d4
	mov r11, r1
.L_020086f6:
	mov r2, r11
	cmp r2, #0
	beq .L_020087ac
	movs r1, #255
	ldrh r3, [r2]
	lsls r1, r1, #8
	adds r1, #255
	cmp r3, r1
	beq .L_020087a2
.L_02008708:
	mov r2, r11
	ldrh r0, [r2]
	bl Actor_GetById
	movs r3, #4
	add r11, r3
	mov r1, r11
	adds r7, r0, #0
	ldrh r0, [r1]
	bl GameFlag_Test
	mov r9, r0
	cmp r0, #0
	bne .L_02008790
	adds r3, r7, #0
	adds r3, #34
	ldrb r3, [r3]
	movs r1, #156
	mov r8, r3
	mov r2, r8
	lsls r3, r3, #3
	subs r3, r3, r2
	ldr r2, [sp, #4]
	lsls r1, r1, #1
	lsls r3, r3, #3
	adds r3, r3, r1
	ldr r6, [r2, r3]
	ldr r3, .L_020087d8
	ldr r1, .L_020087dc
	adds r5, r6, r3
	asrs r5, r5, #2
	adds r0, r7, #0
	adds r5, r5, r1
	bl Func_020041e0
	ldr r1, [r7, #8]
	mov r10, r0
	ldr r2, [r7, #16]
	mov r0, r8
	bl Func_02004020
	str r0, [sp, #0]
	ldr r1, [r7, #8]
	ldr r2, [r7, #16]
	mov r0, r8
	bl Map_GetTerrainHeight
	mov r1, r10
	lsls r3, r1, #2
	adds r6, r6, r3
	adds r3, r7, #0
	mov r1, r9
	adds r3, #85
	strb r1, [r3]
	strb r1, [r6, #2]
	ldrb r1, [r6, #3]
	movs r3, #128
	orrs r3, r1
	adds r2, r0, #0
	strb r3, [r6, #3]
	asrs r2, r2, #19
	adds r2, #4
	mov r0, r8
	ldr r1, [sp, #0]
	bl Func_020041e8
	add r5, r10
	strb r0, [r5]
.L_02008790:
	movs r2, #2
	add r11, r2
	mov r1, r11
	movs r2, #255
	ldrh r3, [r1]
	lsls r2, r2, #8
	adds r2, #255
	cmp r3, r2
	bne .L_02008708
.L_020087a2:
	movs r0, #134
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
.L_020087ac:
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_020087bc:
	.4byte gMainRoutineWork
.L_020087c0:
	.4byte 0x00000045
.L_020087c4:
	.4byte gData_0200478e
.L_020087c8:
	.4byte 0x00000046
.L_020087cc:
	.4byte gData_020047d2
.L_020087d0:
	.4byte 0x00000047
.L_020087d4:
	.4byte gData_020047ec
.L_020087d8:
	.4byte 0xfdff0000
.L_020087dc:
	.4byte gMapShapeGrid
	.section .text.x020087e0,"ax",%progbits
	.global Func_020007e0
	.thumb_func
Func_020007e0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	sub sp, #8
	str r3, [sp, #4]
	ldr r3, .L_020088f4
	movs r2, #240
	lsls r2, r2, #1
	movs r1, #0
	adds r3, r3, r2
	mov r11, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_020088f8
	cmp r2, r3
	bne .L_02008814
	ldr r2, .L_020088fc
	mov r11, r2
	b .L_0200882a
.L_02008814:
	ldr r3, .L_02008900
	cmp r2, r3
	bne .L_02008820
	ldr r3, .L_02008904
	mov r11, r3
	b .L_0200882a
.L_02008820:
	ldr r3, .L_02008908
	cmp r2, r3
	bne .L_0200882a
	ldr r1, .L_0200890c
	mov r11, r1
.L_0200882a:
	mov r2, r11
	cmp r2, #0
	beq .L_020088e4
	movs r1, #255
	ldrh r3, [r2]
	lsls r1, r1, #8
	adds r1, #255
	cmp r3, r1
	beq .L_020088da
.L_0200883c:
	mov r2, r11
	ldrh r0, [r2]
	bl Actor_GetById
	movs r3, #4
	add r11, r3
	mov r1, r11
	adds r7, r0, #0
	ldrh r0, [r1]
	bl GameFlag_Test
	str r0, [sp, #0]
	cmp r0, #0
	bne .L_020088c8
	adds r3, r7, #0
	adds r3, #34
	ldrb r3, [r3]
	movs r1, #156
	mov r8, r3
	mov r2, r8
	lsls r3, r3, #3
	subs r3, r3, r2
	ldr r2, [sp, #4]
	lsls r1, r1, #1
	lsls r3, r3, #3
	adds r3, r3, r1
	ldr r6, [r2, r3]
	ldr r3, .L_02008910
	ldr r1, .L_02008914
	adds r5, r6, r3
	asrs r5, r5, #2
	adds r0, r7, #0
	adds r5, r5, r1
	bl Func_020041e0
	ldr r1, [r7, #8]
	mov r10, r0
	ldr r2, [r7, #16]
	mov r0, r8
	bl Func_02004020
	ldr r1, [r7, #8]
	mov r9, r0
	ldr r2, [r7, #16]
	mov r0, r8
	bl Map_GetTerrainHeight
	mov r1, r10
	lsls r3, r1, #2
	mov r1, sp
	ldrb r1, [r1]
	adds r6, r6, r3
	adds r3, r7, #0
	adds r3, #85
	strb r1, [r3]
	ldrb r1, [r6, #3]
	movs r3, #255
	adds r2, r0, #0
	strb r3, [r6, #2]
	movs r3, #127
	ands r3, r1
	asrs r2, r2, #19
	strb r3, [r6, #3]
	subs r2, #4
	mov r0, r8
	mov r1, r9
	bl Func_020041e8
	add r5, r10
	strb r0, [r5]
.L_020088c8:
	movs r2, #2
	add r11, r2
	mov r1, r11
	movs r2, #255
	ldrh r3, [r1]
	lsls r2, r2, #8
	adds r2, #255
	cmp r3, r2
	bne .L_0200883c
.L_020088da:
	movs r0, #134
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_ClearBit
.L_020088e4:
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_020088f4:
	.4byte gMainRoutineWork
.L_020088f8:
	.4byte 0x00000045
.L_020088fc:
	.4byte gData_0200478e
.L_02008900:
	.4byte 0x00000046
.L_02008904:
	.4byte gData_020047d2
.L_02008908:
	.4byte 0x00000047
.L_0200890c:
	.4byte gData_020047ec
.L_02008910:
	.4byte 0xfdff0000
.L_02008914:
	.4byte gMapShapeGrid
	.section .text.x02008b84,"ax",%progbits
	.global Func_02000b84
	.thumb_func
Func_02000b84:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, .L_02008c28
	sub sp, #68
	add r7, sp, #28
	str r3, [r7, #36]
	movs r3, #204
	lsls r3, r3, #8
	adds r3, #204
	str r3, [r7, #8]
	str r3, [r7, #12]
	mov r10, r2
	movs r2, #0
	mov r11, r0
	mov r9, r1
	mov r8, r2
.L_02008bae:
	mov r3, r8
	lsls r6, r3, #14
	adds r0, r6, #0
	bl Math_Cosine
	add r5, sp, #16
	movs r3, #0
	str r0, [r5]
	adds r0, r6, #0
	str r3, [r5, #4]
	bl Math_Sine
	ldr r3, [r5]
	str r0, [r5, #8]
	asrs r2, r3, #1
	adds r3, r3, r2
	str r3, [r5]
	bl Random16Far
	ldr r3, [r5]
	ldr r2, .L_02008c2c
	lsls r0, r0, #17
	lsrs r0, r0, #16
	adds r3, r3, r0
	adds r3, r3, r2
	str r3, [r5]
	bl Random16Far
	ldr r2, [r5, #8]
	ldr r3, .L_02008c30
	lsls r0, r0, #16
	lsrs r0, r0, #16
	adds r2, r2, r0
	adds r2, r2, r3
	str r2, [r5, #8]
	ldr r3, [r5]
	ldr r1, [r5, #4]
	str r2, [sp, #4]
	movs r2, #128
	lsls r2, r2, #17
	adds r2, #1
	str r1, [sp, #0]
	str r2, [sp, #8]
	mov r0, r11
	mov r2, r10
	mov r1, r9
	str r7, [sp, #12]
	bl Func_020000b8
	movs r2, #1
	add r8, r2
	mov r3, r8
	cmp r3, #16
	bls .L_02008bae
	add sp, #68
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_02008c28:
	.4byte Effect_AdvanceMotionAtLeaderPriority
.L_02008c2c:
	.4byte 0xffff0000
.L_02008c30:
	.4byte 0xffff8000
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
	.section .text.x02009690,"ax",%progbits
	.balign 4
	.global Func_02001690
	.thumb_func
Func_02001690:
	push {r5, r6, lr}
	bl Func_02004040
	movs r0, #0
	bl Func_020041b8
	ldr r0, .L_02009a98
	bl Func_020040c8
	movs r0, #9
	bl Actor_Stop
	movs r0, #1
	bl wait
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #9
	bl Func_02004100
	movs r2, #20
	movs r0, #9
	movs r1, #0
	bl Actor_ArmCallback
	movs r1, #0
	movs r0, #9
	bl Func_020040d0
	movs r0, #8
	bl Actor_Stop
	movs r0, #1
	bl wait
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl Actor_ArmCallback
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #208
	movs r0, #11
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #208
	movs r0, #12
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #208
	movs r0, #13
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #208
	movs r0, #14
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	ldr r6, .L_02009a9c
	movs r3, #133
	lsls r3, r3, #2
	adds r6, r6, r3
	movs r1, #128
	ldr r0, [r6]
	lsls r1, r1, #1
	movs r2, #0
	bl Func_02004100
	movs r1, #128
	movs r2, #0
	ldr r0, [r6]
	lsls r1, r1, #8
	bl Actor_ArmCallback
	bl Func_02004130
	movs r1, #204
	movs r3, #0
	adds r0, #85
	lsls r1, r1, #6
	strb r3, [r0]
	adds r1, #51
	ldr r0, .L_02009aa0
	bl Func_02004118
	movs r0, #212
	movs r2, #212
	movs r3, #1
	lsls r2, r2, #16
	lsls r0, r0, #16
	movs r1, #0
	bl Camera_MoveWithinMapBounds
	bl Func_02004128
	movs r0, #12
	movs r1, #0
	bl Func_020040d0
	movs r2, #10
	movs r0, #11
	movs r1, #0
	bl Actor_ArmCallback
	movs r0, #11
	movs r1, #0
	bl Func_020040d0
	movs r1, #176
	movs r0, #13
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #160
	movs r0, #14
	lsls r1, r1, #7
	movs r2, #20
	bl Actor_ArmCallback
	movs r2, #40
	movs r0, #14
	movs r1, #4
	bl Actor_Jump
	movs r0, #13
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #14
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #8
	movs r1, #14
	bl Actor_WatchAndSetScript
	movs r0, #9
	movs r1, #14
	bl Actor_WatchAndSetScript
	movs r0, #10
	movs r1, #14
	bl Actor_WatchAndSetScript
	movs r0, #11
	movs r1, #14
	bl Actor_WatchAndSetScript
	movs r0, #12
	movs r1, #14
	bl Actor_WatchAndSetScript
	movs r0, #13
	movs r1, #14
	bl Actor_WatchAndSetScript
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #8
	movs r0, #13
	lsls r1, r1, #9
	bl Actor_SetSpeedParameters
	ldr r1, .L_02009aa4
	movs r0, #14
	bl Actor_SetMovementTypeById
	movs r0, #204
	movs r1, #200
	lsls r0, r0, #8
	lsls r1, r1, #5
	adds r0, #204
	adds r1, #153
	bl Func_02004118
	movs r0, #240
	movs r1, #1
	movs r2, #130
	lsls r2, r2, #17
	movs r3, #1
	negs r1, r1
	lsls r0, r0, #16
	bl Camera_MoveWithinMapBounds
	bl Func_02004128
	movs r0, #8
	bl Actor_Stop
	movs r0, #9
	bl Actor_Stop
	movs r0, #10
	bl Actor_Stop
	movs r0, #11
	bl Actor_Stop
	movs r0, #12
	bl Actor_Stop
	movs r0, #13
	bl Actor_Stop
	movs r1, #152
	lsls r1, r1, #7
	ldr r0, .L_02009aa8
	adds r1, #204
	bl Func_02004118
	movs r0, #212
	movs r1, #1
	movs r2, #212
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #16
	lsls r0, r0, #16
	bl Camera_MoveWithinMapBounds
	bl Func_02004128
	movs r0, #20
	bl FieldEvent_Wait
	movs r1, #208
	movs r0, #8
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #160
	movs r2, #20
	movs r0, #9
	lsls r1, r1, #7
	bl Actor_ArmCallback
	movs r0, #9
	movs r1, #0
	bl Func_020040d0
	movs r0, #8
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #8
	movs r1, #0
	bl Func_020040d0
	movs r0, #10
	movs r1, #4
	movs r2, #0
	bl Actor_Jump
	movs r2, #0
	movs r0, #10
	movs r1, #0
	bl Actor_ArmCallback
	movs r0, #10
	movs r1, #4
	bl Actor_SetModeById
	movs r0, #10
	movs r1, #0
	bl Func_020040d0
	movs r1, #128
	movs r0, #8
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #128
	movs r0, #9
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #8
	bl Func_02004100
	movs r1, #129
	movs r2, #80
	lsls r1, r1, #1
	movs r0, #9
	bl Func_02004100
	movs r0, #14
	movs r1, #0
	bl Func_020040d0
	movs r1, #208
	movs r0, #8
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #192
	movs r0, #10
	lsls r1, r1, #6
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #208
	movs r0, #11
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r0, #12
	movs r1, #0
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #128
	movs r2, #20
	movs r0, #13
	lsls r1, r1, #8
	bl Actor_ArmCallback
	movs r0, #8
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #9
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #10
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #11
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #12
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #13
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #8
	ldr r1, .L_02009aa0
	adds r2, #204
	bl Actor_SetSpeedParameters
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #9
	ldr r1, .L_02009aa0
	adds r2, #204
	bl Actor_SetSpeedParameters
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #10
	ldr r1, .L_02009aa0
	adds r2, #204
	bl Actor_SetSpeedParameters
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #11
	ldr r1, .L_02009aa0
	adds r2, #204
	bl Actor_SetSpeedParameters
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #12
	ldr r1, .L_02009aa0
	adds r2, #204
	bl Actor_SetSpeedParameters
	movs r2, #204
	lsls r2, r2, #8
	adds r2, #204
	movs r0, #13
	ldr r1, .L_02009aa0
	bl Actor_SetSpeedParameters
	ldr r0, [r6]
	movs r1, #13
	bl Actor_WatchAndSetScript
	ldr r5, .L_02009aac
	movs r0, #13
	adds r1, r5, #0
	bl Actor_SetMovementTypeById
	movs r0, #10
	bl FieldEvent_Wait
	adds r1, r5, #0
	movs r0, #12
	bl Actor_SetMovementTypeById
	movs r0, #204
	movs r1, #192
	lsls r0, r0, #7
	lsls r1, r1, #4
	adds r0, #102
	adds r1, #204
	bl Func_02004118
	movs r0, #240
	movs r1, #1
	movs r2, #140
	movs r3, #1
	lsls r2, r2, #17
	lsls r0, r0, #16
	negs r1, r1
	bl Camera_MoveWithinMapBounds
	adds r1, r5, #0
	movs r0, #8
	bl Actor_SetMovementTypeById
	movs r0, #10
	bl FieldEvent_Wait
	adds r1, r5, #0
	movs r0, #9
	bl Actor_SetMovementTypeById
	ldr r5, .L_02009ab0
	movs r0, #10
	adds r1, r5, #0
	bl Actor_SetMovementTypeById
	movs r0, #20
	bl FieldEvent_Wait
	adds r1, r5, #0
	movs r0, #11
	bl Actor_SetMovementTypeAndWaitById
	ldr r0, [r6]
	bl Actor_Stop
	movs r0, #1
	bl wait
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl Func_02004090
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_02004090
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_02004090
	movs r0, #11
	movs r1, #0
	movs r2, #0
	bl Func_02004090
	movs r0, #12
	movs r1, #0
	movs r2, #0
	bl Func_02004090
	movs r0, #13
	movs r1, #0
	movs r2, #0
	bl Func_02004090
	movs r6, #176
	movs r0, #14
	movs r1, #0
	movs r2, #0
	bl Func_02004090
	lsls r6, r6, #8
	movs r1, #130
	movs r2, #242
	adds r3, r6, #0
	movs r0, #8
	lsls r1, r1, #18
	lsls r2, r2, #16
	bl Func_02004098
	movs r3, #128
	lsls r3, r3, #8
	movs r0, #9
	ldr r1, .L_02009ab4
	ldr r2, .L_02009ab8
	b .L_02009abc
	.2byte 0x0000
.L_02009a98:
	.4byte MsgHeyLookUp
.L_02009a9c:
	.4byte gMainRoutineWork
.L_02009aa0:
	.4byte 0x00019999
.L_02009aa4:
	.4byte gData_0200468c
.L_02009aa8:
	.4byte 0x00026666
.L_02009aac:
	.4byte gData_020046dc
.L_02009ab0:
	.4byte gData_02004718
.L_02009ab4:
	.4byte 0x02150000
.L_02009ab8:
	.4byte 0x010f0000
.L_02009abc:
	bl Func_02004098
	movs r3, #192
	movs r2, #232
	lsls r3, r3, #6
	movs r0, #10
	ldr r1, .L_02009b64
	lsls r2, r2, #16
	bl Func_02004098
	movs r3, #160
	movs r2, #248
	lsls r3, r3, #7
	movs r0, #11
	ldr r1, .L_02009b68
	lsls r2, r2, #16
	movs r5, #208
	bl Func_02004098
	lsls r5, r5, #8
	movs r1, #231
	movs r2, #170
	adds r3, r5, #0
	movs r0, #12
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_02004098
	movs r1, #241
	movs r2, #170
	adds r3, r5, #0
	movs r0, #13
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_02004098
	movs r2, #160
	lsls r2, r2, #17
	adds r3, r6, #0
	movs r0, #14
	ldr r1, .L_02009b6c
	bl Func_02004098
	movs r0, #8
	movs r1, #1
	bl Actor_SetModeById
	movs r0, #9
	movs r1, #1
	bl Actor_SetModeById
	movs r0, #10
	movs r1, #1
	bl Actor_SetModeById
	movs r0, #11
	movs r1, #1
	bl Actor_SetModeById
	movs r0, #12
	movs r1, #1
	bl Actor_SetModeById
	movs r0, #13
	movs r1, #1
	bl Actor_SetModeById
	movs r1, #1
	movs r0, #14
	bl Actor_SetModeById
	movs r0, #143
	lsls r0, r0, #4
	bl GameFlag_SetBit
	movs r0, #242
	lsls r0, r0, #3
	adds r0, #255
	bl GameFlag_SetBit
	bl Func_02004048
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02009b64:
	.4byte 0x01b30000
.L_02009b68:
	.4byte 0x01d30000
.L_02009b6c:
	.4byte 0x01e90000
	.section .text.x02009cc4,"ax",%progbits
	.balign 4
	.global Func_02001cc4
	.thumb_func
Func_02001cc4:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r5, .L_02009e40
	mov r10, r2
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r5, r2
	adds r7, r0, #0
	ldr r0, [r3]
	mov r8, r1
	bl Actor_GetById
	movs r1, #0
	bl Actor_SetSingleChildFlags
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #18
	adds r5, r5, r3
	ldrb r3, [r5]
	cmp r3, #5
	beq .L_02009dce
	ldr r3, .L_02009e44
	ldr r5, [r3]
	movs r3, #1
	ands r5, r3
	cmp r5, #0
	bne .L_02009dce
	movs r0, #30
	ldr r1, [r7, #8]
	ldr r2, [r7, #12]
	ldr r3, [r7, #16]
	adds r0, #255
	bl Func_02003fc0
	adds r6, r0, #0
	cmp r6, #0
	beq .L_02009dce
	adds r3, r6, #0
	adds r3, #85
	strb r5, [r3]
	adds r3, #4
	strb r5, [r3]
	movs r1, #0
	bl Actor_SetSingleChildFlags
	adds r0, r6, #0
	movs r1, #1
	bl Func_02003fa8
	ldr r1, .L_02009e48
	adds r0, r6, #0
	bl Func_02003fb8
	ldr r1, [r6, #80]
	movs r3, #13
	ldrb r2, [r1, #9]
	negs r3, r3
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	strb r3, [r1, #9]
	ldr r5, .L_02009e4c
	ldr r3, [r5]
	cmp r3, #0
	beq .L_02009d88
	bl Random16Far
	lsls r0, r0, #3
	lsrs r0, r0, #16
	lsls r0, r0, #14
	bl Math_Cosine
	add r0, r8
	str r0, [r6, #68]
	bl Random16Far
	lsls r0, r0, #3
	lsrs r0, r0, #16
	lsls r0, r0, #14
	bl Math_Sine
	add r0, r10
	str r0, [r6, #76]
	movs r2, #2
	ldr r3, [r5]
	ands r3, r2
	cmp r3, #0
	beq .L_02009db0
	ldr r3, [r6, #68]
	mov r2, r10
	add r3, r8
	str r3, [r6, #68]
	adds r3, r0, r2
	str r3, [r6, #76]
	b .L_02009db0
.L_02009d88:
	bl Random16Far
	lsls r0, r0, #3
	lsrs r0, r0, #16
	lsls r0, r0, #14
	bl Math_Cosine
	asrs r0, r0, #1
	add r0, r8
	str r0, [r6, #68]
	bl Random16Far
	lsls r0, r0, #3
	lsrs r0, r0, #16
	lsls r0, r0, #14
	bl Math_Sine
	asrs r0, r0, #1
	add r0, r10
	str r0, [r6, #76]
.L_02009db0:
	movs r5, #0
	str r5, [r6, #72]
	bl Random16Far
	ldr r3, .L_02009e50
	lsls r0, r0, #9
	lsrs r0, r0, #16
	adds r0, r0, r3
	adds r3, r6, #0
	adds r3, #100
	strh r0, [r3]
	ldr r3, .L_02009e54
	str r5, [r6, #48]
	str r5, [r6, #52]
	str r3, [r6, #108]
.L_02009dce:
	mov r2, r8
	cmp r2, #0
	beq .L_02009e06
	ldr r3, [r7, #8]
	movs r2, #240
	add r3, r8
	str r3, [r7, #8]
	ldr r3, .L_02009e4c
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	bne .L_02009e36
	ldr r1, [r7, #16]
	asrs r3, r1, #16
	adds r2, r3, #0
	cmp r3, #0
	bge .L_02009df2
	adds r2, #15
.L_02009df2:
	asrs r2, r2, #4
	lsls r2, r2, #4
	subs r2, r3, r2
	movs r3, #8
	subs r3, r3, r2
	lsls r3, r3, #16
	asrs r3, r3, #3
	adds r3, r1, r3
	str r3, [r7, #16]
	b .L_02009e36
.L_02009e06:
	ldr r3, [r7, #16]
	movs r2, #240
	add r3, r10
	str r3, [r7, #16]
	ldr r3, .L_02009e4c
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	bne .L_02009e36
	ldr r1, [r7, #8]
	asrs r3, r1, #16
	adds r2, r3, #0
	cmp r3, #0
	bge .L_02009e24
	adds r2, #15
.L_02009e24:
	asrs r2, r2, #4
	lsls r2, r2, #4
	subs r2, r3, r2
	movs r3, #8
	subs r3, r3, r2
	lsls r3, r3, #16
	asrs r3, r3, #3
	adds r3, r1, r3
	str r3, [r7, #8]
.L_02009e36:
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009e40:
	.4byte gMainRoutineWork
.L_02009e44:
	.4byte gFrameCount
.L_02009e48:
	.4byte gData_020042f0
.L_02009e4c:
	.4byte gInput
.L_02009e50:
	.4byte 0xffffff00
.L_02009e54:
	.4byte Effect_AdvanceMotionAtLeaderPriority
	.section .text.x0200ad6c,"ax",%progbits
	.global Func_02002d6c
	.thumb_func
Func_02002d6c:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r9
	push {r5, r6}
	mov r6, r8
	push {r6}
	adds r6, r0, #0
	mov r8, r1
	mov r10, r2
	mov r9, r3
	bl Func_02004040
	movs r0, #0
	bl Func_020041b8
	adds r2, r6, #0
	adds r2, #85
	movs r3, #0
	ldr r5, .L_0200ae0c
	strb r3, [r2]
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	movs r1, #128
	movs r2, #128
	ldr r0, [r5]
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl Actor_SetSpeedParameters
	mov r2, r10
	ldr r0, [r5]
	mov r1, r8
	bl Actor_SetPositionAndReset
	movs r1, #192
	ldr r0, [r5]
	lsls r1, r1, #8
	bl Func_020040e8
	ldr r0, [r5]
	bl Actor_GetById
	movs r1, #0
	bl Actor_SetSingleChildFlags
	ldr r0, [r5]
	movs r1, #13
	bl Actor_SetModeById
	ldr r2, [r6, #12]
	ldr r3, .L_0200ae10
	ldr r1, [r6, #8]
	adds r2, r2, r3
	adds r0, r6, #0
	ldr r3, [r6, #16]
	bl Func_02003fe0
	adds r0, r6, #0
	bl Func_02003fe8
	movs r1, #10
	ldr r0, [r5]
	bl Actor_SetModeById
	movs r0, #123
	bl Func_020041f8
	bl Event_FinishScreenTransition
	bl Event_WaitTransitionFrames
	mov r0, r9
	bl Func_02004138
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, pc}
.L_0200ae0c:
	.4byte gMainRoutineWork
.L_0200ae10:
	.4byte 0xfff60000
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
	.section .text.x0200b88c,"ax",%progbits
	.global Func_0200388c
	.thumb_func
Func_0200388c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #224
	adds r5, r0, #0
	mov r9, r1
	mov r10, r2
	movs r1, #255
	ldr r2, [r3]
	b .L_0200b8f8
.L_0200b8a8:
	ldrh r3, [r5]
	movs r1, #26
	ldrsh r7, [r2, r1]
	cmp r7, r3
	bne .L_0200b8f4
	adds r0, r7, #0
	bl Actor_GetById
	adds r5, #2
	ldrh r2, [r5]
	mov r3, r10
	adds r6, r0, #0
	mov r8, r2
	ldrh r5, [r5, #2]
	cmp r3, #7
	bgt .L_0200b8d0
	ldr r3, [r6, #28]
	ldr r1, .L_0200b90c
	adds r3, r3, r1
	str r3, [r6, #28]
.L_0200b8d0:
	mov r2, r9
	cmp r2, #1
	bne .L_0200b902
	adds r0, r5, #0
	bl GameFlag_SetBit
	adds r3, r6, #0
	adds r3, #34
	ldrb r3, [r3]
	adds r0, r7, #0
	mov r1, r8
	adds r2, r5, #0
	bl Func_02003910
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r6, #28]
	b .L_0200b902
.L_0200b8f4:
	adds r5, #6
	movs r1, #255
.L_0200b8f8:
	ldrh r3, [r5]
	lsls r1, r1, #8
	adds r1, #255
	cmp r3, r1
	bne .L_0200b8a8
.L_0200b902:
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_0200b90c:
	.4byte 0xffffe100
	.section .text.x0200b9b8,"ax",%progbits
	.global Func_020039b8
	.thumb_func
Func_020039b8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r0, .L_0200bb6c
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #32]
	mov r9, r0
	movs r0, #158
	mov r4, r9
	lsls r0, r0, #1
	movs r3, #2
	ldrsh r2, [r4, r3]
	adds r3, r1, r0
	ldr r3, [r3]
	sub sp, #4
	str r3, [sp, #0]
	movs r4, #156
	lsls r4, r4, #1
	adds r3, r1, r4
	ldr r1, .L_0200bb70
	ldr r3, [r3]
	lsls r2, r2, #2
	mov r5, r9
	movs r0, #0
	adds r1, r1, r2
	adds r5, #4
	mov r11, r3
	mov r10, r0
	mov r8, r1
.L_0200b9fc:
	movs r2, #0
	ldrsh r0, [r5, r2]
	cmp r0, #0
	bne .L_0200ba06
	b .L_0200bb4c
.L_0200ba06:
	bl Actor_GetById
	adds r7, r0, #0
	ldr r3, [r7, #8]
	cmp r3, #0
	bne .L_0200ba1a
	ldr r3, [r7, #16]
	cmp r3, #0
	bne .L_0200ba1a
	b .L_0200bb4c
.L_0200ba1a:
	movs r3, #2
	ldrsh r2, [r5, r3]
	movs r4, #8
	ldrsh r3, [r5, r4]
	cmp r2, r3
	bne .L_0200ba30
	movs r0, #206
	bl Func_020041f8
	movs r3, #4
	strh r3, [r5, #18]
.L_0200ba30:
	movs r0, #2
	ldrsh r2, [r5, r0]
	movs r1, #10
	ldrsh r3, [r5, r1]
	cmp r2, r3
	bne .L_0200ba4c
	movs r0, #140
	adds r0, #255
	bl Func_020041f8
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #248
	strh r3, [r5, #18]
.L_0200ba4c:
	movs r4, #18
	ldrsh r3, [r5, r4]
	ldrh r2, [r5, #18]
	cmp r3, #0
	beq .L_0200ba86
	ldrh r3, [r5, #4]
	movs r0, #0
	adds r3, r3, r2
	movs r4, #12
	ldrsh r2, [r5, r4]
	strh r3, [r5, #4]
	lsls r3, r3, #16
	asrs r3, r3, #16
	ldrh r1, [r5, #12]
	cmp r3, r2
	blt .L_0200ba72
	strh r1, [r5, #4]
	strh r0, [r5, #18]
	strh r0, [r5, #22]
.L_0200ba72:
	movs r1, #4
	ldrsh r2, [r5, r1]
	movs r4, #14
	ldrsh r3, [r5, r4]
	ldrh r1, [r5, #14]
	cmp r2, r3
	bgt .L_0200ba86
	strh r1, [r5, #4]
	strh r0, [r5, #18]
	strh r0, [r5, #22]
.L_0200ba86:
	movs r0, #130
	lsls r0, r0, #1
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200baa6
	ldrh r3, [r5, #2]
	movs r1, #6
	ldrsh r2, [r5, r1]
	adds r3, #1
	strh r3, [r5, #2]
	lsls r3, r3, #16
	asrs r3, r3, #16
	cmp r3, r2
	blt .L_0200baa6
	strh r0, [r5, #2]
.L_0200baa6:
	ldr r3, [r7, #16]
	ldr r2, [r7, #8]
	asrs r3, r3, #20
	asrs r2, r2, #20
	lsls r3, r3, #7
	adds r1, r2, r3
	mov r2, r9
	ldrb r3, [r2, #2]
	ldr r4, [sp, #0]
	add r3, r10
	strb r3, [r4, r1]
	movs r0, #14
	ldrsh r2, [r7, r0]
	movs r4, #4
	ldrsh r3, [r5, r4]
	adds r2, r2, r3
	cmp r2, #0
	bge .L_0200bacc
	adds r2, #7
.L_0200bacc:
	asrs r3, r2, #3
	lsls r3, r3, #8
	mov r0, r8
	str r3, [r0]
	mov r2, r11
	lsls r3, r1, #2
	adds r1, r2, r3
	movs r4, #18
	ldrsh r3, [r5, r4]
	cmp r3, #0
	beq .L_0200bae6
	movs r3, #0
	b .L_0200baec
.L_0200bae6:
	ldrb r2, [r1, #3]
	movs r3, #128
	orrs r3, r2
.L_0200baec:
	strb r3, [r1, #3]
	movs r0, #4
	ldrsh r3, [r5, r0]
	cmp r3, #0
	beq .L_0200bafc
	ldrb r2, [r1, #3]
	movs r3, #16
	orrs r3, r2
.L_0200bafc:
	strb r3, [r1, #3]
	ldr r6, [r5, #24]
	cmp r6, #0
	beq .L_0200bb4c
	movs r1, #4
	ldrsh r3, [r5, r1]
	cmp r3, #0
	bne .L_0200bb14
	ldr r3, [r7, #12]
	ldr r2, .L_0200bb74
	adds r3, r3, r2
	b .L_0200bb42
.L_0200bb14:
	ldrh r0, [r5, #22]
	movs r3, #128
	lsls r3, r3, #5
	adds r0, r0, r3
	strh r0, [r5, #22]
	lsls r0, r0, #16
	asrs r0, r0, #16
	bl Math_Sine
	adds r1, r0, #0
	movs r0, #128
	ldr r3, .L_0200bb78
	lsls r0, r0, #10
	mov lr, r3
	.2byte 0xf800
	movs r1, #4
	ldrsh r2, [r5, r1]
	ldr r3, [r7, #12]
	ldr r4, .L_0200bb7c
	lsls r2, r2, #16
	adds r0, r0, r4
	adds r3, r3, r2
	adds r3, r3, r0
.L_0200bb42:
	str r3, [r6, #12]
	ldr r3, [r7, #8]
	str r3, [r6, #8]
	ldr r3, [r7, #16]
	str r3, [r6, #16]
.L_0200bb4c:
	movs r3, #1
	add r10, r3
	movs r2, #4
	mov r4, r10
	add r8, r2
	adds r5, #28
	cmp r4, #15
	bgt .L_0200bb5e
	b .L_0200b9fc
.L_0200bb5e:
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0200bb6c:
	.4byte gData_020023c4 + 0x188
.L_0200bb70:
	.4byte gMapCollision
.L_0200bb74:
	.4byte 0xfff00000
.L_0200bb78:
	.4byte IwramMulQ16
.L_0200bb7c:
	.4byte 0xfff20000
	.section .text.x0200bb80,"ax",%progbits
	.global Func_02003b80
	.thumb_func
Func_02003b80:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r2, .L_0200bd84
	sub sp, #36
	adds r0, r2, #4
	str r0, [sp, #32]
	ldr r1, .L_0200bd88
	movs r0, #0
	ldrsh r3, [r2, r0]
	lsls r3, r3, #2
	adds r3, r3, r1
	ldrh r3, [r3, #2]
	movs r1, #226
	lsrs r3, r3, #5
	str r3, [sp, #24]
	lsls r1, r1, #1
	adds r2, r2, r1
	ldr r2, [r2]
	movs r3, #192
	mov r9, r2
	movs r2, #0
	str r2, [sp, #16]
	lsls r3, r3, #18
	ldr r1, [r3, #32]
	adds r2, r1, #0
	adds r2, #228
	ldr r3, [r2]
	str r3, [sp, #12]
	ldr r0, [sp, #12]
	ldr r3, .L_0200bd8c
	ands r0, r3
	str r0, [sp, #12]
	ldr r2, [r2, #4]
	ands r2, r3
	str r2, [sp, #8]
	ldr r3, [r1]
	movs r1, #15
	ldr r3, [r3, #4]
	str r1, [sp, #28]
	str r3, [sp, #4]
.L_0200bbda:
	ldr r3, [sp, #32]
	movs r2, #0
	ldrsh r0, [r3, r2]
	cmp r0, #0
	bne .L_0200bbe6
	b .L_0200bd64
.L_0200bbe6:
	movs r1, #4
	ldrsh r3, [r3, r1]
	cmp r3, #0
	bne .L_0200bbf0
	b .L_0200bd64
.L_0200bbf0:
	bl Actor_GetById
	ldr r3, .L_0200bd90
	movs r1, #12
	mov r10, r0
	ldr r0, [r3]
	bl __umodsi3
	movs r1, #3
	bl __udivsi3
	lsls r0, r0, #3
	adds r0, #32
	str r0, [sp, #20]
	ldr r1, [sp, #32]
	movs r2, #0
	movs r0, #4
	ldrsh r3, [r1, r0]
	mov r11, r2
	cmp r11, r3
	bge .L_0200bcb4
.L_0200bc1a:
	ldr r2, [sp, #16]
	cmp r2, #79
	bgt .L_0200bca6
	mov r0, r10
	ldr r3, [r0, #8]
	ldr r1, [sp, #12]
	subs r7, r3, r1
	mov r3, r11
	lsls r2, r3, #16
	ldr r3, [r0, #12]
	ldr r0, [sp, #4]
	adds r3, r3, r2
	mov r1, r10
	subs r0, r3, r0
	ldr r2, [sp, #8]
	ldr r3, [r1, #16]
	mov r8, r0
	ldr r0, [sp, #4]
	subs r3, r3, r2
	subs r5, r3, r0
	mov r1, r8
	subs r6, r5, r1
	asrs r3, r6, #16
	adds r6, r3, #0
	adds r3, r1, r5
	asrs r3, r3, #16
	asrs r2, r7, #16
	adds r1, r3, #0
	movs r3, #167
	adds r7, r2, #0
	lsls r3, r3, #1
	adds r2, #7
	subs r7, #8
	subs r6, #16
	adds r1, #58
	cmp r2, r3
	bhi .L_0200bca6
	movs r0, #16
	negs r0, r0
	cmp r6, r0
	ble .L_0200bca6
	cmp r6, #239
	bgt .L_0200bca6
	adds r3, #177
	ands r7, r3
	movs r3, #255
	mov r4, r9
	ands r6, r3
	movs r3, #0
	stmia r4!, {r3}
	lsls r3, r7, #16
	orrs r6, r3
	ldr r3, .L_0200bd94
	orrs r6, r3
	stmia r4!, {r6}
	ldr r2, [sp, #24]
	ldr r0, [sp, #20]
	adds r3, r2, r0
	movs r2, #128
	lsls r2, r2, #4
	orrs r3, r2
	mov r0, r9
	str r3, [r4]
	bl Func_02003f88
	ldr r2, [sp, #16]
	movs r1, #12
	adds r2, #1
	str r2, [sp, #16]
	add r9, r1
.L_0200bca6:
	ldr r1, [sp, #32]
	movs r3, #16
	add r11, r3
	movs r0, #4
	ldrsh r3, [r1, r0]
	cmp r11, r3
	blt .L_0200bc1a
.L_0200bcb4:
	ldr r2, [sp, #16]
	cmp r2, #79
	bgt .L_0200bd64
	mov r0, r10
	ldr r3, [r0, #8]
	ldr r1, [sp, #12]
	ldr r0, [sp, #32]
	subs r7, r3, r1
	movs r3, #4
	ldrsh r2, [r0, r3]
	mov r1, r10
	ldr r3, [r1, #12]
	lsls r2, r2, #16
	adds r3, r3, r2
	ldr r2, [sp, #4]
	ldr r0, [sp, #8]
	subs r2, r3, r2
	ldr r3, [r1, #16]
	ldr r1, [sp, #4]
	subs r3, r3, r0
	subs r5, r3, r1
	ldr r3, [sp, #32]
	subs r6, r5, r2
	mov r8, r2
	movs r2, #22
	ldrsh r0, [r3, r2]
	bl Math_Sine
	ldr r3, .L_0200bd98
	adds r1, r0, #0
	ldr r0, .L_0200bd9c
	mov lr, r3
	.2byte 0xf800
	ldr r1, .L_0200bd9c
	adds r0, r6, r0
	mov r2, r8
	asrs r7, r7, #16
	adds r0, r0, r1
	adds r3, r2, r5
	mov r10, r7
	asrs r0, r0, #16
	asrs r3, r3, #16
	adds r1, r3, #0
	subs r6, r0, #4
	mov r3, r10
	movs r0, #167
	adds r3, #7
	lsls r0, r0, #1
	subs r7, #8
	adds r1, #58
	cmp r3, r0
	bhi .L_0200bd64
	movs r2, #16
	negs r2, r2
	cmp r6, r2
	ble .L_0200bd64
	cmp r6, #239
	bgt .L_0200bd64
	movs r3, #128
	lsls r3, r3, #1
	adds r3, #255
	ands r7, r3
	movs r3, #255
	mov r4, r9
	ands r6, r3
	movs r3, #0
	stmia r4!, {r3}
	lsls r3, r7, #16
	orrs r6, r3
	ldr r3, .L_0200bd94
	orrs r6, r3
	stmia r4!, {r6}
	ldr r0, [sp, #24]
	ldr r2, [sp, #20]
	adds r3, r0, r2
	movs r2, #128
	lsls r2, r2, #4
	subs r3, #32
	orrs r3, r2
	mov r0, r9
	str r3, [r4]
	bl Func_02003f88
	ldr r0, [sp, #16]
	movs r3, #12
	adds r0, #1
	str r0, [sp, #16]
	add r9, r3
.L_0200bd64:
	ldr r1, [sp, #28]
	ldr r2, [sp, #32]
	subs r1, #1
	adds r2, #28
	str r1, [sp, #28]
	str r2, [sp, #32]
	cmp r1, #0
	blt .L_0200bd76
	b .L_0200bbda
.L_0200bd76:
	add sp, #36
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0200bd84:
	.4byte gData_020023c4 + 0x188
.L_0200bd88:
	.4byte gResourceTableEntries
.L_0200bd8c:
	.4byte 0xffff0000
.L_0200bd90:
	.4byte gFrameCount
.L_0200bd94:
	.4byte 0x40002000
.L_0200bd98:
	.4byte IwramMulQ16
.L_0200bd9c:
	.4byte 0xfffe0000
	.section .text.x0200be3c,"ax",%progbits
	.balign 4
	.global Func_02003e3c
	.thumb_func
Func_02003e3c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	mov r9, r2
	mov r10, r3
	ldr r2, .L_0200bed0
	lsls r3, r0, #3
	subs r3, r3, r0
	mov r8, r1
	lsls r3, r3, #2
	adds r3, r3, r2
	mov r0, r8
	ldr r7, [sp, #28]
	adds r5, r3, #4
	bl Actor_GetById
	adds r6, r0, #0
	mov r0, r8
	bl Actor_GetById
	adds r0, r6, #0
	movs r1, #0
	bl Actor_SetSingleChildFlags
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200bed4
	cmp r7, r10
	bge .L_0200be86
	mov r12, r10
	mov r10, r7
	mov r7, r12
.L_0200be86:
	mov r3, r8
	strh r3, [r5]
	mov r3, r9
	strh r3, [r5, #2]
	movs r3, #180
	lsls r3, r3, #1
	strh r3, [r5, #6]
	movs r3, #60
	strh r3, [r5, #8]
	movs r3, #240
	strh r3, [r5, #10]
	mov r3, r10
	ldr r2, .L_0200becc
	strh r3, [r5, #14]
	movs r3, #1
	strh r3, [r5, #16]
	adds r3, r6, #0
	adds r3, #85
	strb r2, [r3]
	adds r2, r6, #0
	adds r2, #89
	movs r3, #8
	strb r3, [r2]
	adds r1, r6, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #2
	orrs r3, r2
	strh r0, [r5, #4]
	strh r7, [r5, #12]
	strh r0, [r5, #18]
	strh r0, [r5, #22]
	strh r0, [r5, #20]
	strb r3, [r1]
	b .L_0200bed4
.L_0200becc:
	.4byte 0x00000000
.L_0200bed0:
	.4byte gData_020023c4 + 0x188
.L_0200bed4:
	movs r0, #128
	ldr r1, [r6, #8]
	ldr r2, [r6, #12]
	ldr r3, [r6, #16]
	lsls r0, r0, #8
	bl Func_02003fc0
	adds r1, r0, #0
	adds r2, r1, #0
	movs r3, #0
	adds r2, #89
	strb r3, [r2]
	subs r2, #4
	strb r3, [r2]
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #128
	orrs r3, r2
	strb r3, [r0]
	str r1, [r5, #24]
	adds r0, r5, #0
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
