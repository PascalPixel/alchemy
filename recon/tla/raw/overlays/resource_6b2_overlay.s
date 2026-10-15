.syntax unified
	.thumb
	.section .text.x020080f4,"ax",%progbits
	.global Func_020000f4
	.thumb_func
Func_020000f4:
	push {r5, r6, r7, lr}
	movs r0, #8
	movs r1, #3
	bl ObjectMotion_SetActionVariant
	movs r1, #3
	movs r0, #9
	bl ObjectMotion_SetActionVariant
	ldr r3, .L_020081a0
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	ldr r3, .L_020081a4
	ldr r7, [r3]
	movs r3, #15
	ands r7, r3
	cmp r7, #0
	bne .L_02008194
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #162
	movs r1, #0
	movs r2, #0
	movs r3, #0
	bl Func_02000550
	adds r6, r0, #0
	cmp r6, #0
	beq .L_02008194
	ldr r5, [r6, #80]
	bl Random16Far
	lsls r3, r0, #2
	adds r3, r3, r0
	movs r2, #203
	lsls r2, r2, #18
	lsls r3, r3, #3
	adds r1, r6, #0
	adds r3, r3, r2
	adds r1, #85
	strb r7, [r1]
	movs r2, #174
	str r3, [r6, #8]
	movs r3, #160
	lsls r2, r2, #18
	lsls r3, r3, #16
	str r3, [r6, #12]
	str r2, [r6, #16]
	adds r0, r6, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	adds r1, r6, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #254
	ands r3, r2
	ldrb r2, [r5, #9]
	strb r3, [r1]
	movs r3, #13
	negs r3, r3
	ands r3, r2
	movs r2, #8
	orrs r3, r2
	ldr r1, .L_020081a8
	adds r0, r6, #0
	strb r3, [r5, #9]
	bl Func_02000548
	adds r0, r6, #0
	movs r1, #1
	bl Func_02000540
.L_02008194:
	ldr r2, .L_020081ac
	ldr r3, [r2]
	adds r3, #1
	str r3, [r2]
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_020081a0:
	.4byte gPartyState
.L_020081a4:
	.4byte gFrameCount
.L_020081a8:
	.4byte Data_02000618
.L_020081ac:
	.4byte Data_020007e4
	.section .text.x020081b0,"ax",%progbits
	.global Func_020001b0
	.thumb_func
Func_020001b0:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	bl Func_02000568
	movs r0, #0
	bl Func_020005f8
	ldr r3, .L_020084b0
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	ldr r0, [r3]
	bl Object_GetById
	movs r1, #15
	bl Object_SetPartAttribute
	movs r0, #1
	bl WaitFrames
	ldr r3, .L_020084b4
	movs r6, #0
	movs r1, #144
	str r6, [r3]
	lsls r1, r1, #3
	ldr r0, .L_020084b8
	bl Scheduler_AddOrUpdateCallback
	movs r0, #8
	movs r1, #2
	bl ObjectMotion_SetActionVariant
	movs r0, #9
	movs r1, #2
	bl ObjectMotion_SetActionVariant
	movs r2, #192
	lsls r2, r2, #18
	movs r3, #218
	mov r8, r2
	lsls r3, r3, #1
	ldr r2, [r2, #108]
	mov r10, r3
	mov r1, r10
	movs r3, #40
	str r3, [r2, r1]
	bl Event_StartScreenTransition
	bl Event_WaitTransitionFrames
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #14
	movs r0, #13
	bl Func_02000600
	ldr r0, .L_020084bc
	bl Func_02000598
	movs r0, #3
	movs r1, #0
	bl Func_020005a0
	bl Func_02000608
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #8
	bl Func_020005c0
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #20
	movs r0, #8
	bl Func_020005c0
	movs r1, #192
	movs r0, #8
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r2, #10
	movs r0, #8
	movs r1, #4
	bl ObjectMotion_Launch
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #8
	movs r1, #0
	bl Func_020005a0
	movs r1, #2
	adds r1, #255
	movs r2, #0
	movs r0, #11
	bl Func_020005c0
	movs r1, #2
	adds r1, #255
	movs r2, #40
	movs r0, #12
	bl Func_020005c0
	movs r1, #208
	movs r0, #11
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #176
	movs r2, #0
	movs r0, #12
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r0, #13
	movs r1, #14
	bl Func_02000600
	movs r0, #3
	movs r1, #0
	bl Func_020005a0
	bl Func_02000608
	movs r0, #9
	movs r1, #3
	bl Object_SetModeById
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #9
	movs r1, #0
	bl Func_020005a0
	movs r0, #12
	movs r1, #0
	bl Func_020005b0
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #12
	movs r1, #0
	bl Func_020005a0
	movs r1, #128
	movs r0, #11
	lsls r1, r1, #8
	bl Func_020005b0
	movs r0, #11
	movs r1, #4
	bl Object_SetModeById
	movs r0, #128
	lsls r0, r0, #7
	movs r1, #0
	adds r0, #11
	bl Func_020005a0
	movs r0, #11
	bl Object_GetById
	adds r7, r0, #0
	bl Random16Far
	lsls r3, r0, #2
	adds r3, r3, r0
	lsls r3, r3, #4
	adds r2, r7, #0
	lsrs r3, r3, #16
	adds r2, #98
	ldr r5, .L_020084c0
	adds r3, #40
	strb r3, [r2]
	adds r3, r7, #0
	adds r3, #99
	strb r6, [r3]
	movs r0, #12
	str r5, [r7, #108]
	bl Object_GetById
	adds r7, r0, #0
	bl Random16Far
	lsls r3, r0, #2
	adds r3, r3, r0
	lsls r3, r3, #4
	adds r2, r7, #0
	lsrs r3, r3, #16
	adds r3, #40
	adds r2, #98
	strb r3, [r2]
	movs r3, #2
	adds r2, #1
	strb r3, [r2]
	movs r0, #13
	str r5, [r7, #108]
	movs r1, #14
	bl Func_02000600
	movs r0, #3
	movs r1, #0
	bl Func_020005a0
	bl Func_02000608
	movs r0, #8
	movs r1, #4
	bl Object_SetModeById
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #8
	movs r1, #0
	bl Func_020005a0
	movs r0, #13
	movs r1, #14
	bl Func_02000600
	movs r0, #3
	movs r1, #0
	bl Func_020005a0
	bl Func_02000608
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #9
	movs r1, #0
	bl Func_020005a0
	movs r0, #13
	movs r1, #14
	bl Func_02000600
	movs r0, #3
	movs r1, #0
	bl Func_020005a0
	bl Func_02000608
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #8
	movs r1, #0
	bl Func_020005a0
	movs r0, #13
	movs r1, #14
	bl Func_02000600
	movs r0, #3
	movs r1, #0
	bl Func_020005a0
	bl Func_02000608
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #9
	movs r1, #0
	bl Func_020005a0
	movs r0, #13
	movs r1, #14
	bl Func_02000600
	movs r0, #3
	movs r1, #0
	bl Func_020005a0
	bl Func_02000608
	movs r0, #8
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #8
	movs r1, #0
	bl Func_020005a0
	movs r0, #13
	movs r1, #14
	bl Func_02000600
	movs r0, #1
	movs r1, #0
	bl Func_020005a0
	bl Func_02000608
	movs r0, #9
	movs r1, #2
	movs r2, #0
	bl ObjectMotion_Launch
	movs r1, #8
	movs r2, #20
	adds r1, #255
	movs r0, #9
	bl Func_020005c0
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #9
	movs r1, #0
	bl Func_020005a0
	movs r0, #13
	movs r1, #14
	bl Func_02000600
	movs r0, #3
	movs r1, #0
	bl Func_020005a0
	bl Func_02000608
	movs r0, #8
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #8
	movs r1, #0
	bl Func_020005a0
	movs r0, #13
	movs r1, #14
	bl Func_02000600
	movs r1, #0
	movs r0, #6
	bl Func_020005a0
	bl Func_02000608
	movs r0, #20
	bl Battle_WaitMode0
	movs r3, #160
	lsls r3, r3, #19
	movs r0, #1
	strh r6, [r3]
	bl WaitFrames
	mov r2, r8
	ldr r3, [r2, #108]
	mov r1, r10
	movs r2, #16
	str r2, [r3, r1]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #85
	str r2, [r3]
	bl Event_FinishScreenTransition
	bl Event_WaitTransitionFrames
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #60
	bl GameFlag_SetBit
	movs r0, #3
	bl Func_020005c8
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_020084b0:
	.4byte gPartyState
.L_020084b4:
	.4byte Data_020007e4
.L_020084b8:
	.4byte Func_020000f4
.L_020084bc:
	.4byte MsgImilVoicesDisbeliefReply
.L_020084c0:
	.4byte Func_02000060
	.section .bss,"aw",%nobits
	.global Data_020007e4
Data_020007e4:
