.syntax unified
	.thumb
	.section .text.x02008d74,"ax",%progbits
	.balign 4
	.global Func_02000d74
	.thumb_func
Func_02000d74:
	push {r5, r6, lr}
	bl Func_020032c8
	movs r0, #0
	bl Func_02005c9c
	movs r0, #17
	movs r1, #0
	bl Func_02005b64
	movs r0, #7
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	ldr r3, .L_02009184
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	ldr r0, [r3]
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #26
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r2, #10
	movs r0, #6
	movs r1, #0
	bl ObjectMotion_ArmCallback
	movs r1, #153
	lsls r1, r1, #8
	ldr r0, .L_02009188
	adds r1, #153
	bl Func_02005ba4
	movs r0, #132
	movs r1, #1
	movs r2, #194
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #17
	lsls r0, r0, #17
	bl Camera_MoveWithinMapBounds
	bl Func_02005bb4
	movs r0, #40
	bl FieldEvent_WaitFrames
	movs r1, #192
	movs r2, #20
	movs r0, #17
	lsls r1, r1, #6
	bl ObjectMotion_ArmCallback
	movs r0, #17
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #128
	lsls r0, r0, #7
	movs r1, #0
	adds r0, #17
	bl Func_02005b64
	movs r0, #26
	bl Func_02005c9c
	movs r0, #27
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	movs r0, #27
	bl Object_GetById
	movs r3, #0
	adds r0, #85
	strb r3, [r0]
	movs r0, #27
	bl Object_GetById
	movs r2, #192
	movs r3, #172
	lsls r2, r2, #13
	lsls r3, r3, #17
	ldr r1, .L_0200918c
	bl Object_SetPositionAndResetMotion
	movs r0, #204
	movs r1, #192
	lsls r0, r0, #7
	lsls r1, r1, #4
	adds r0, #102
	adds r1, #204
	bl Func_02005ba4
	movs r0, #132
	movs r1, #1
	movs r2, #170
	movs r3, #1
	lsls r0, r0, #17
	negs r1, r1
	lsls r2, r2, #17
	bl Camera_MoveWithinMapBounds
	movs r1, #204
	movs r2, #200
	lsls r1, r1, #6
	lsls r2, r2, #5
	movs r0, #17
	adds r1, #51
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r1, #132
	movs r2, #187
	lsls r2, r2, #1
	movs r0, #17
	lsls r1, r1, #1
	bl ObjectMotion_SetPositionAndReset
	ldr r6, .L_02009190
	movs r1, #144
	lsls r1, r1, #3
	adds r0, r6, #0
	bl Scheduler_AddOrUpdateCallback
	movs r0, #20
	bl FieldEvent_WaitFrames
	ldr r5, .L_02009194
	adds r0, r5, #0
	bl Scheduler_RemoveCallbackFar
	bl Func_020032e0
	movs r1, #144
	lsls r1, r1, #3
	adds r0, r5, #0
	bl Scheduler_AddOrUpdateCallback
	movs r0, #7
	movs r1, #0
	bl Func_02005b64
	movs r1, #0
	movs r0, #17
	bl Func_02005b64
	movs r0, #11
	bl Func_02005c9c
	bl Func_02003610
	movs r0, #160
	bl FieldEvent_WaitFrames
	movs r2, #80
	movs r1, #0
	movs r0, #17
	bl Func_02005b5c
	movs r0, #144
	lsls r0, r0, #2
	bl Func_02005c9c
	movs r1, #1
	movs r0, #1
	bl Func_02003640
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r2, #80
	movs r1, #0
	movs r0, #17
	bl Func_02005b5c
	movs r0, #161
	bl Func_02005c9c
	movs r1, #1
	movs r0, #0
	bl Func_02003640
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #17
	bl Func_02005b9c
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r1, #0
	movs r2, #20
	movs r0, #17
	bl Func_02005b5c
	adds r0, r6, #0
	bl Scheduler_RemoveCallbackFar
	movs r0, #1
	bl WaitFrames
	movs r0, #27
	movs r1, #0
	movs r2, #0
	bl Func_02005b0c
	movs r1, #208
	movs r0, #17
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	ldr r3, .L_02009198
	ldr r3, [r3]
	cmp r3, #0
	beq .L_02008f7c
	ldr r3, [r3, #12]
	movs r2, #144
	lsls r2, r2, #14
	cmp r3, r2
	ble .L_02008f7c
.L_02008f52:
	movs r1, #208
	movs r0, #17
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	ldr r5, .L_02009198
	ldr r1, .L_0200919c
	ldr r2, [r5]
	movs r0, #1
	ldr r3, [r2, #12]
	adds r3, r3, r1
	str r3, [r2, #12]
	bl WaitFrames
	ldr r3, [r5]
	movs r2, #144
	ldr r3, [r3, #12]
	lsls r2, r2, #14
	cmp r3, r2
	bgt .L_02008f52
.L_02008f7c:
	movs r1, #192
	movs r0, #17
	lsls r1, r1, #6
	bl Func_02005b7c
	movs r0, #17
	movs r1, #0
	bl Func_02005b64
	movs r1, #153
	lsls r1, r1, #8
	ldr r0, .L_02009188
	adds r1, #153
	bl Func_02005ba4
	movs r0, #132
	movs r1, #128
	movs r2, #210
	movs r3, #1
	lsls r0, r0, #17
	lsls r1, r1, #13
	lsls r2, r2, #17
	bl Camera_MoveWithinMapBounds
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #17
	ldr r1, .L_020091a0
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r1, #132
	movs r2, #196
	lsls r2, r2, #1
	lsls r1, r1, #1
	movs r0, #17
	bl ObjectMotion_SetPositionAndReset
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r0, #128
	lsls r0, r0, #8
	movs r1, #0
	adds r0, #17
	bl Func_02005b64
	movs r0, #148
	bl Func_02005c9c
	movs r0, #11
	bl Func_02005c9c
	bl Func_020032b0
	movs r0, #80
	bl FieldEvent_WaitFrames
	movs r0, #176
	movs r1, #128
	movs r2, #196
	movs r3, #1
	lsls r2, r2, #17
	lsls r1, r1, #14
	lsls r0, r0, #15
	bl Camera_MoveWithinMapBounds
	bl Func_02005bb4
	movs r0, #40
	bl FieldEvent_WaitFrames
	movs r0, #7
	movs r1, #0
	bl Func_02005b64
	movs r1, #129
	movs r0, #7
	lsls r1, r1, #1
	bl Func_02005b9c
	movs r0, #7
	movs r1, #0
	bl Func_02005b64
	movs r0, #7
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #7
	movs r1, #0
	bl Func_02005b64
	movs r0, #7
	movs r1, #2
	bl ObjectMotion_SetActionVariant
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #7
	ldr r1, .L_020091a0
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r2, #193
	movs r0, #7
	movs r1, #152
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #224
	movs r0, #7
	lsls r1, r1, #8
	movs r2, #40
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #7
	lsls r1, r1, #6
	movs r2, #20
	bl ObjectMotion_ArmCallback
	movs r1, #224
	movs r0, #7
	lsls r1, r1, #8
	movs r2, #20
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r2, #40
	movs r0, #7
	lsls r1, r1, #6
	bl ObjectMotion_ArmCallback
	movs r0, #7
	movs r1, #0
	bl Func_02005b64
	movs r2, #178
	movs r0, #7
	movs r1, #124
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #224
	movs r0, #7
	lsls r1, r1, #8
	movs r2, #20
	bl ObjectMotion_ArmCallback
	movs r1, #160
	movs r0, #7
	lsls r1, r1, #8
	movs r2, #20
	bl ObjectMotion_ArmCallback
	movs r1, #224
	movs r2, #20
	movs r0, #7
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #7
	movs r1, #0
	bl Func_02005b64
	movs r1, #128
	movs r2, #0
	movs r0, #5
	lsls r1, r1, #6
	bl ObjectMotion_ArmCallback
	movs r0, #5
	movs r1, #0
	bl Func_02005b64
	ldr r5, .L_02009184
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	movs r1, #224
	ldr r0, [r5]
	lsls r1, r1, #8
	movs r2, #40
	bl ObjectMotion_ArmCallback
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #26
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r2, #184
	lsls r2, r2, #1
	movs r1, #104
	movs r0, #26
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r0, #204
	movs r1, #200
	lsls r0, r0, #8
	lsls r1, r1, #5
	adds r0, #204
	adds r1, #153
	bl Func_02005ba4
	movs r0, #222
	movs r1, #1
	movs r2, #178
	movs r3, #1
	lsls r2, r2, #17
	negs r1, r1
	lsls r0, r0, #15
	bl Camera_MoveWithinMapBounds
	movs r0, #26
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #26
	movs r1, #1
	bl Object_SetModeById
	movs r1, #208
	movs r0, #26
	lsls r1, r1, #8
	movs r2, #40
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r2, #20
	lsls r1, r1, #1
	movs r0, #26
	bl Func_02005b94
	movs r0, #26
	movs r1, #0
	bl Func_02005b64
	movs r1, #192
	ldr r0, [r5]
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	b .L_020091a4
	.2byte 0x0000
.L_02009184:
	.4byte gPartyState
.L_02009188:
	.4byte 0x0004cccc
.L_0200918c:
	.4byte 0x01090000
.L_02009190:
	.4byte Func_02002eb0
.L_02009194:
	.4byte Func_02002ccc
.L_02009198:
	.4byte gData_02007950
.L_0200919c:
	.4byte 0xffffc000
.L_020091a0:
	.4byte 0x00019999
.L_020091a4:
	movs r1, #192
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	movs r0, #26
	lsls r1, r1, #7
	movs r2, #20
	bl ObjectMotion_ArmCallback
	movs r0, #192
	lsls r0, r0, #7
	movs r1, #0
	adds r0, #26
	bl UiText_OpenMessageAtObject
	ldr r0, [r5]
	movs r1, #0
	bl Event_AskYesNoWithActorAnimation
	cmp r0, #0
	bne .L_020091f4
	movs r0, #192
	lsls r0, r0, #7
	movs r1, #0
	adds r0, #26
	bl Func_02005b64
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r1, #226
	lsls r1, r1, #1
	adds r2, r2, r1
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_02009216
.L_020091f4:
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #192
	adds r3, #1
	lsls r0, r0, #7
	strh r3, [r2]
	adds r0, #26
	movs r1, #0
	bl Func_02005b64
	bl Func_02005c7c
.L_02009216:
	pop {r5, r6, pc}
	.section .text.x0200af32,"ax",%progbits
	.2byte 0x0000
	.section .text.x0200b308,"ax",%progbits
	.balign 4
	.global Func_02003308
	.thumb_func
Func_02003308:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r6, r0, #0
	movs r2, #99
	adds r2, r2, r6
	ldrb r3, [r2]
	sub sp, #12
	movs r1, #0
	mov r8, r2
	cmp r3, #0
	beq .L_0200b342
	adds r3, r6, #0
	adds r2, r6, #0
	adds r3, #100
	adds r2, #102
	movs r1, #0
	ldrsh r2, [r2, r1]
	movs r4, #0
	ldrsh r3, [r3, r4]
	adds r3, r3, r2
	lsls r0, r3, #3
	adds r0, r0, r3
	lsls r0, r0, #9
	bl Math_Sine
	lsls r3, r0, #1
	adds r3, r3, r0
	lsls r1, r3, #1
.L_0200b342:
	ldr r3, [r6, #56]
	mov r5, sp
	str r3, [r5]
	adds r7, r6, #0
	ldr r3, [r6, #60]
	adds r7, #100
	str r3, [r5, #4]
	ldr r3, [r6, #64]
	str r3, [r5, #8]
	adds r3, r6, #0
	ldr r0, [r6, #76]
	adds r3, #102
	movs r4, #0
	ldrsh r2, [r3, r4]
	movs r4, #0
	ldrsh r3, [r7, r4]
	lsls r0, r0, #16
	adds r0, r0, r1
	lsls r1, r3, #1
	adds r1, r1, r3
	lsls r1, r1, #8
	adds r1, r1, r2
	adds r2, r5, #0
	bl Vector_AddPolarOffsetFar
	ldr r3, [r5]
	str r3, [r6, #8]
	ldr r3, [r5, #4]
	str r3, [r6, #12]
	ldr r3, [r5, #8]
	adds r5, r6, #0
	str r3, [r6, #16]
	adds r5, #98
	ldrb r3, [r5]
	cmp r3, #7
	bls .L_0200b38c
	b .L_0200b4f2
.L_0200b38c:
	ldr r2, .L_0200b4fc
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
.L_0200b394:
	.4byte .L_0200b3b4
	.4byte .L_0200b3f8
	.4byte .L_0200b42c
	.4byte .L_0200b44a
	.4byte .L_0200b468
	.4byte .L_0200b482
	.4byte .L_0200b4b0
	.4byte .L_0200b4e4
.L_0200b3b4:
	ldrh r3, [r7]
	movs r0, #1
	adds r3, #4
	strh r3, [r7]
	str r0, [r6, #76]
	ldr r2, [r6, #80]
	movs r1, #128
	ldrh r3, [r2, #18]
	lsls r1, r1, #5
	adds r3, r3, r1
	strh r3, [r2, #18]
	movs r1, #192
	ldr r3, [r6, #28]
	lsls r1, r1, #3
	ldr r2, [r6, #24]
	adds r1, #102
	adds r3, r3, r1
	str r3, [r6, #28]
	movs r3, #192
	adds r2, r2, r1
	lsls r3, r3, #9
	str r2, [r6, #24]
	cmp r2, r3
	bgt .L_0200b3e6
	b .L_0200b4f2
.L_0200b3e6:
	ldr r3, [r6, #80]
	ldrh r3, [r3, #18]
	cmp r3, #0
	beq .L_0200b3f0
	b .L_0200b4f2
.L_0200b3f0:
	mov r4, r8
	strb r0, [r5]
	strb r0, [r4]
	b .L_0200b4f2
.L_0200b3f8:
	ldrh r3, [r7]
	movs r1, #128
	adds r3, #4
	strh r3, [r7]
	lsls r1, r1, #5
	ldr r2, [r6, #80]
	ldrh r3, [r2, #18]
	adds r3, r3, r1
	strh r3, [r2, #18]
	ldr r3, [r6, #80]
	ldrh r3, [r3, #18]
	cmp r3, #0
	bne .L_0200b4f2
	mov r2, r8
	ldrb r3, [r2]
	adds r3, #255
	strb r3, [r2]
	lsls r3, r3, #24
	cmp r3, #0
	bne .L_0200b4f2
	movs r0, #220
	bl Func_02005c9c
	movs r3, #2
	strb r3, [r5]
	b .L_0200b4f2
.L_0200b42c:
	ldrh r3, [r7]
	movs r4, #128
	adds r3, #1
	strh r3, [r7]
	lsls r4, r4, #11
	ldr r3, [r6, #60]
	movs r1, #200
	adds r3, r3, r4
	lsls r1, r1, #15
	str r3, [r6, #60]
	cmp r3, r1
	ble .L_0200b4f2
	movs r3, #3
	strb r3, [r5]
	b .L_0200b4f2
.L_0200b44a:
	ldrh r3, [r7]
	mov r2, r8
	adds r3, #1
	strh r3, [r7]
	ldr r3, [r6, #76]
	adds r3, #2
	str r3, [r6, #76]
	movs r3, #10
	strb r3, [r2]
	ldr r3, [r6, #76]
	cmp r3, #40
	ble .L_0200b4f2
	movs r3, #4
	strb r3, [r5]
	b .L_0200b4f2
.L_0200b468:
	ldrh r3, [r7]
	mov r4, r8
	adds r3, #1
	strh r3, [r7]
	ldrb r3, [r4]
	adds r3, #255
	strb r3, [r4]
	lsls r3, r3, #24
	cmp r3, #0
	bne .L_0200b4f2
	movs r3, #5
	strb r3, [r5]
	b .L_0200b4f2
.L_0200b482:
	ldrh r3, [r7]
	movs r1, #136
	adds r3, #1
	strh r3, [r7]
	lsls r1, r1, #16
	ldr r3, [r6, #76]
	subs r3, #2
	str r3, [r6, #76]
	ldr r3, [r6, #60]
	cmp r3, r1
	bge .L_0200b4a0
	movs r2, #128
	lsls r2, r2, #10
	adds r3, r3, r2
	str r3, [r6, #60]
.L_0200b4a0:
	ldr r3, [r6, #76]
	cmp r3, #1
	bgt .L_0200b4f2
	movs r3, #0
	str r3, [r6, #76]
	movs r3, #6
	strb r3, [r5]
	b .L_0200b4f2
.L_0200b4b0:
	ldrh r3, [r7]
	movs r4, #128
	adds r3, #4
	strh r3, [r7]
	lsls r4, r4, #5
	ldr r2, [r6, #80]
	ldr r1, .L_0200b500
	ldrh r3, [r2, #18]
	adds r3, r3, r4
	strh r3, [r2, #18]
	ldr r2, [r6, #24]
	ldr r3, [r6, #28]
	adds r2, r2, r1
	adds r3, r3, r1
	movs r1, #144
	lsls r1, r1, #5
	str r2, [r6, #24]
	str r3, [r6, #28]
	cmp r2, r1
	bge .L_0200b4f2
	movs r3, #100
	str r3, [r6, #56]
	str r3, [r6, #64]
	movs r3, #7
	strb r3, [r5]
	b .L_0200b4f2
.L_0200b4e4:
	ldr r3, [r6, #80]
	ldrb r0, [r3, #16]
	bl Resource_ResetEntry
	adds r0, r6, #0
	bl Func_02005a3c
.L_0200b4f2:
	add sp, #12
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200b4fc:
	.4byte .L_0200b394
.L_0200b500:
	.4byte 0xfffff000
	.section .text.x0200b504,"ax",%progbits
	.global Func_02003504
	.thumb_func
Func_02003504:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r0, #17
	bl Object_GetById
	mov r8, r0
	movs r0, #0
	mov r11, r0
	movs r0, #154
	bl Func_02005c9c
	movs r2, #0
	mov r10, r2
.L_0200b528:
	movs r0, #17
	bl Object_GetById
	movs r1, #7
	bl Object_SetPartAttribute
	movs r0, #2
	bl WaitFrames
	movs r0, #17
	bl Object_GetById
	movs r1, #0
	bl Object_SetPartAttribute
	movs r0, #2
	bl WaitFrames
	movs r3, #1
	add r10, r3
	mov r0, r10
	cmp r0, #7
	bls .L_0200b528
	movs r0, #209
	bl Func_02005c9c
	movs r2, #0
	mov r10, r2
	mov r9, r2
.L_0200b562:
	mov r3, r8
	ldr r2, [r3, #12]
	movs r0, #192
	lsls r0, r0, #13
	adds r2, r2, r0
	movs r0, #209
	lsls r0, r0, #1
	ldr r1, [r3, #8]
	adds r0, #255
	ldr r3, [r3, #16]
	bl Func_02005a34
	adds r7, r0, #0
	cmp r7, #0
	beq .L_0200b5f4
	mov r1, r11
	ldr r0, [r7, #80]
	bl Func_02005c44
	adds r3, r7, #0
	movs r5, #0
	adds r3, #85
	strb r5, [r3]
	ldr r1, [r7, #80]
	mov r11, r0
	ldrb r3, [r1, #9]
	movs r0, #13
	negs r0, r0
	adds r2, r0, #0
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	strb r3, [r1, #9]
	adds r0, r7, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	adds r0, r7, #0
	movs r1, #1
	bl Func_02005a24
	adds r3, r7, #0
	adds r3, #100
	movs r1, #180
	strh r5, [r3]
	lsls r1, r1, #1
	str r5, [r7, #76]
	mov r0, r9
	bl __udivsi3
	ldr r6, .L_0200b5ec
	adds r3, r7, #0
	adds r3, #102
	strh r0, [r3]
	subs r3, #4
	strb r6, [r3]
	mov r2, r8
	ldr r3, [r2, #8]
	movs r0, #192
	str r3, [r7, #56]
	lsls r0, r0, #13
	ldr r3, [r2, #12]
	adds r3, r3, r0
	str r3, [r7, #60]
	ldr r3, [r2, #16]
	str r3, [r7, #64]
	ldr r3, .L_0200b5f0
	str r3, [r7, #108]
	b .L_0200b5f4
.L_0200b5ec:
	.4byte 0x00000000
.L_0200b5f0:
	.4byte Func_02003308
.L_0200b5f4:
	movs r3, #1
	movs r2, #176
	add r10, r3
	lsls r2, r2, #13
	mov r0, r10
	add r9, r2
	cmp r0, #15
	bls .L_0200b562
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.section .text.x0200c524,"ax",%progbits
	.global Func_02004524
	.thumb_func
Func_02004524:
	push {r5, lr}
	bl Func_02005a9c
	movs r0, #0
	bl Func_02005c3c
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r2, r2
	negs r1, r1
	movs r3, #0
	negs r0, r0
	bl Camera_MoveWithinMapBounds
	bl Func_02005bbc
	movs r3, #0
	adds r0, #85
	ldr r5, .L_0200c5c4
	strb r3, [r0]
	movs r1, #133
	lsls r1, r1, #2
	adds r5, r5, r1
	ldr r0, [r5]
	bl Object_GetById
	movs r1, #15
	bl Object_SetPartAttribute
	ldr r0, [r5]
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_0200c5c8
	bl Scheduler_AddOrUpdateCallback
	movs r0, #1
	movs r1, #1
	bl Func_02003640
	ldr r5, .L_0200c5cc
	ldr r2, [r5]
	cmp r2, #0
	beq .L_0200c59e
	ldr r3, [r2, #12]
	movs r1, #128
	lsls r1, r1, #14
	adds r3, r3, r1
	str r3, [r2, #12]
	movs r0, #1
	bl WaitFrames
	ldr r3, [r5]
	movs r2, #4
	adds r3, #85
	strb r2, [r3]
.L_0200c59e:
	movs r0, #1
	bl WaitFrames
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #85
	str r2, [r3]
	bl Event_StartScreenTransition
	bl Event_WaitTransitionFrames
	movs r0, #40
	bl FieldEvent_WaitFrames
	pop {r5, pc}
.L_0200c5c4:
	.4byte gPartyState
.L_0200c5c8:
	.4byte Func_02002ccc
.L_0200c5cc:
	.4byte gData_02007950
	.section .text.x0200ca28,"ax",%progbits
	.balign 4
	.global Func_02004a28
	.thumb_func
Func_02004a28:
	push {r5, r6, r7, lr}
	movs r0, #14
	bl Object_GetById
	adds r6, r0, #0
	bl Func_02004524
	movs r1, #1
	movs r0, #17
	bl ObjectMotion_SetActionVariant
	movs r0, #144
	lsls r0, r0, #2
	bl Func_02005c9c
	movs r0, #12
	bl Object_GetById
	movs r1, #7
	bl Object_SetPartAttribute
	movs r0, #13
	bl Object_GetById
	movs r1, #7
	bl Object_SetPartAttribute
	movs r0, #8
	bl WaitFrames
	movs r0, #12
	bl Object_GetById
	movs r1, #0
	bl Object_SetPartAttribute
	movs r0, #13
	bl Object_GetById
	movs r1, #0
	bl Object_SetPartAttribute
	movs r0, #8
	bl WaitFrames
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #20
	movs r0, #17
	bl Func_02005b94
	ldr r0, .L_0200cce4
	bl Func_02005b4c
	movs r2, #20
	movs r1, #0
	movs r0, #17
	bl Func_02005b5c
	movs r0, #144
	lsls r0, r0, #2
	bl Func_02005c9c
	movs r1, #0
	movs r0, #1
	bl Func_020036e0
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r0, #233
	bl Func_02005c9c
	movs r3, #200
	lsls r3, r3, #5
	adds r3, #153
	str r3, [r6, #28]
	movs r0, #14
	bl Object_GetById
	movs r1, #132
	movs r2, #192
	movs r3, #154
	lsls r1, r1, #17
	lsls r2, r2, #13
	lsls r3, r3, #17
	bl Object_SetPositionAndResetMotion
	movs r0, #1
	bl WaitFrames
	movs r0, #14
	movs r1, #3
	bl Object_SetModeById
	movs r5, #0
.L_0200cae8:
	ldr r3, [r6, #28]
	movs r1, #128
	lsls r1, r1, #3
	adds r3, r3, r1
	str r3, [r6, #28]
	ldr r3, [r6, #12]
	movs r2, #128
	lsls r2, r2, #6
	adds r3, r3, r2
	str r3, [r6, #12]
	movs r0, #1
	adds r5, #1
	bl WaitFrames
	cmp r5, #63
	bls .L_0200cae8
	movs r0, #149
	lsls r0, r0, #1
	bl Func_02005c9c
	ldr r3, .L_0200cce8
	ldr r2, [r3]
	cmp r2, #0
	beq .L_0200cb48
	adds r1, r2, #0
	adds r1, #85
	movs r3, #0
	strb r3, [r1]
	movs r1, #144
	ldr r3, [r2, #12]
	lsls r1, r1, #14
	cmp r3, r1
	ble .L_0200cb48
.L_0200cb2a:
	ldr r5, .L_0200cce8
	ldr r1, .L_0200ccec
	ldr r2, [r5]
	movs r0, #1
	ldr r3, [r2, #12]
	adds r3, r3, r1
	str r3, [r2, #12]
	bl WaitFrames
	ldr r3, [r5]
	movs r2, #144
	ldr r3, [r3, #12]
	lsls r2, r2, #14
	cmp r3, r2
	bgt .L_0200cb2a
.L_0200cb48:
	movs r0, #232
	bl Func_02005c9c
	ldr r3, .L_0200cce8
	ldr r2, [r3]
	cmp r2, #0
	beq .L_0200cbb2
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	movs r5, #0
.L_0200cb5e:
	ldr r7, .L_0200cce8
	movs r1, #192
	ldr r2, [r7]
	lsls r1, r1, #11
	ldr r3, [r2, #12]
	movs r0, #1
	adds r3, r3, r1
	str r3, [r2, #12]
	ldr r2, .L_0200ccf0
	ldr r3, [r6, #28]
	ldr r1, .L_0200ccec
	adds r3, r3, r2
	str r3, [r6, #28]
	ldr r3, [r6, #12]
	adds r5, #1
	adds r3, r3, r1
	str r3, [r6, #12]
	bl WaitFrames
	cmp r5, #3
	bls .L_0200cb5e
	ldr r0, [r7]
	movs r1, #0
	movs r2, #0
	movs r3, #0
	bl Object_SetPositionAndResetMotion
	movs r5, #0
.L_0200cb96:
	ldr r3, [r6, #28]
	ldr r2, .L_0200ccf0
	ldr r1, .L_0200ccec
	adds r3, r3, r2
	str r3, [r6, #28]
	ldr r3, [r6, #12]
	movs r0, #1
	adds r3, r3, r1
	str r3, [r6, #12]
	adds r5, #1
	bl WaitFrames
	cmp r5, #3
	bls .L_0200cb96
.L_0200cbb2:
	movs r2, #0
	movs r0, #14
	movs r1, #0
	bl Func_02005b0c
	movs r0, #0
	movs r1, #0
	bl Func_020036e0
	movs r1, #1
	movs r0, #1
	bl Func_02003640
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r1, #1
	movs r0, #0
	bl Func_02003640
	movs r0, #40
	bl FieldEvent_WaitFrames
	movs r0, #144
	lsls r0, r0, #2
	bl Func_02005c9c
	movs r0, #1
	movs r1, #1
	bl Func_02003640
	movs r1, #0
	movs r0, #1
	bl Func_020036e0
	movs r0, #132
	bl Func_02005c9c
	movs r0, #14
	bl Object_GetById
	movs r1, #132
	movs r2, #192
	movs r3, #154
	lsls r1, r1, #17
	lsls r2, r2, #13
	lsls r3, r3, #17
	bl Object_SetPositionAndResetMotion
	movs r0, #1
	bl WaitFrames
	movs r5, #0
.L_0200cc1c:
	ldr r3, [r6, #28]
	movs r2, #128
	lsls r2, r2, #6
	adds r3, r3, r2
	str r3, [r6, #28]
	ldr r3, [r6, #12]
	movs r1, #128
	lsls r1, r1, #8
	adds r3, r3, r1
	str r3, [r6, #12]
	movs r0, #1
	adds r5, #1
	bl WaitFrames
	cmp r5, #7
	bls .L_0200cc1c
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #17
	bl Func_02005b9c
	movs r0, #40
	bl FieldEvent_WaitFrames
	movs r1, #192
	movs r0, #17
	lsls r1, r1, #6
	bl Func_02005b7c
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #17
	movs r1, #0
	bl Func_02005b64
	movs r1, #132
	movs r2, #20
	lsls r1, r1, #1
	movs r0, #17
	bl Func_02005b94
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #17
	movs r1, #0
	bl Func_02005b64
	movs r0, #17
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #17
	movs r1, #0
	bl Func_02005b64
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #17
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #132
	movs r2, #160
	movs r0, #17
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #0
	movs r0, #17
	movs r2, #0
	bl Func_02005b0c
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #87
	str r2, [r3]
	bl Event_FinishScreenTransition
	bl Event_WaitTransitionFrames
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #251
	bl GameFlag_SetBit
	movs r0, #13
	bl Func_02005bc4
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200cce4:
	.4byte MsgLookTheGreatGabomba
.L_0200cce8:
	.4byte gData_02007950
.L_0200ccec:
	.4byte 0xffff8000
.L_0200ccf0:
	.4byte 0xffffe000
	.section .text.x0200d16c,"ax",%progbits
	.balign 4
	.global Func_0200516c
	.thumb_func
Func_0200516c:
	push {r5, r6, r7, lr}
	movs r0, #11
	bl Object_GetById
	adds r7, r0, #0
	bl Func_02005a9c
	movs r0, #0
	bl Func_02005c3c
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200d26a
	adds r3, r7, #0
	adds r3, #85
	strb r0, [r3]
	movs r3, #168
	lsls r3, r3, #14
	ldr r1, .L_0200d2bc
	str r3, [r7, #12]
	ldr r3, [r7, #16]
	movs r0, #11
	adds r3, r3, r1
	str r3, [r7, #16]
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	movs r1, #1
	movs r0, #11
	bl ObjectMotion_SetActionVariant
	ldr r3, .L_0200d2c0
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #19
	bne .L_0200d26a
	movs r0, #7
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200d26a
	movs r0, #7
	bl Unit_Get
	adds r5, r0, #0
	ldrh r1, [r5, #52]
	ldrh r3, [r5, #54]
	strh r1, [r5, #56]
	strh r3, [r5, #58]
	lsls r1, r1, #16
	asrs r1, r1, #16
	lsls r0, r1, #14
	bl __divsi3
	movs r3, #128
	lsls r3, r3, #7
	cmp r0, r3
	bgt .L_0200d1f8
	movs r3, #0
	cmp r0, #0
	blt .L_0200d1f8
	adds r3, r0, #0
.L_0200d1f8:
	strh r3, [r5, #20]
	lsls r3, r3, #16
	cmp r3, #0
	bne .L_0200d20c
	movs r2, #56
	ldrsh r3, [r5, r2]
	cmp r3, #0
	beq .L_0200d20c
	movs r3, #1
	strh r3, [r5, #20]
.L_0200d20c:
	movs r3, #58
	ldrsh r0, [r5, r3]
	movs r2, #54
	ldrsh r1, [r5, r2]
	lsls r0, r0, #14
	bl __divsi3
	movs r3, #128
	lsls r3, r3, #7
	cmp r0, r3
	bgt .L_0200d22a
	movs r3, #0
	cmp r0, #0
	blt .L_0200d22a
	adds r3, r0, #0
.L_0200d22a:
	strh r3, [r5, #22]
	lsls r3, r3, #16
	cmp r3, #0
	bne .L_0200d23e
	movs r1, #58
	ldrsh r3, [r5, r1]
	cmp r3, #0
	beq .L_0200d23e
	movs r3, #1
	strh r3, [r5, #22]
.L_0200d23e:
	movs r2, #50
	adds r2, #255
	movs r1, #160
	adds r3, r5, r2
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #131
	strb r2, [r3]
	lsls r0, r0, #4
	adds r3, r5, r1
	strb r2, [r3]
	adds r0, #255
	bl GameFlag_SetBit
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #46
	bl GameFlag_ClearBit
	movs r0, #7
	bl Party_RemoveOwnerRestored
.L_0200d26a:
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #2
	bl GameFlag_SetBit
	movs r3, #160
	lsls r3, r3, #19
	adds r3, #18
	ldr r2, .L_0200d2c4
	ldrh r3, [r3]
	ldr r5, .L_0200d2b8
	strh r3, [r2]
	movs r3, #160
	lsls r3, r3, #19
	adds r3, #174
	ldrh r3, [r3]
	movs r6, #0
	strh r3, [r2, #2]
	movs r3, #160
	lsls r3, r3, #19
	adds r3, #206
	ldrh r3, [r3]
	strh r3, [r2, #4]
	movs r3, #160
	lsls r3, r3, #19
	adds r3, #238
	ldrh r3, [r3]
	strh r3, [r2, #6]
	bl Func_020050ac
	movs r0, #30
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	movs r0, #31
	b .L_0200d2c8
	.2byte 0x0000
.L_0200d2b8:
	.4byte 0x00000000
.L_0200d2bc:
	.4byte 0xffd60000
.L_0200d2c0:
	.4byte gPartyState
.L_0200d2c4:
	.4byte gData_02007948
.L_0200d2c8:
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	movs r0, #32
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	movs r0, #33
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	movs r0, #34
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	movs r0, #35
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	movs r0, #133
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
	movs r0, #29
	bl Object_GetById
	adds r7, r0, #0
	adds r3, r7, #0
	adds r3, #85
	strb r5, [r3]
	movs r0, #29
	str r6, [r7, #12]
	bl Func_020031f0
	movs r0, #30
	movs r1, #3
	bl ObjectMotion_SetActionVariant
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #253
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200d364
	movs r0, #26
	movs r1, #0
	movs r2, #0
	bl Func_02005b0c
	movs r0, #27
	movs r1, #0
	movs r2, #0
	bl Func_02005b0c
	movs r0, #28
	movs r1, #0
	movs r2, #0
	bl Func_02005b0c
	movs r1, #151
	movs r2, #172
	movs r0, #21
	lsls r1, r1, #17
	lsls r2, r2, #16
	bl Func_02005b0c
.L_0200d364:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #107
	bl GameFlag_SetBit
	bl Event_StartScreenTransition
	bl Event_WaitTransitionFrames
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_0200d388
	bl Scheduler_AddOrUpdateCallback
	bl Func_02005aa4
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200d388:
	.4byte Func_02002c34
	.section .text.x0200d898,"ax",%progbits
	.global Func_02005898
	.thumb_func
Func_02005898:
	.ifdef TLA_EDITION_JA
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #93
	str r2, [r3, #0]
	bl Func_02003288
	ldr r3, .L_ja_0200d964
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #9
	bne .L_ja_0200d90e
	movs r0, #254
	lsls r0, r0, #3
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200d90a
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_02005b0c
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #238
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200d8f6
	movs r3, #176
	movs r1, #217
	lsls r3, r3, #8
	movs r0, #8
	lsls r1, r1, #17
	ldr r2, .L_ja_0200d968
	bl Func_02005b14
	b .L_ja_0200d90e
.L_ja_0200d8f6:
	movs r3, #176
	movs r1, #182
	movs r2, #144
	lsls r3, r3, #8
	movs r0, #8
	lsls r1, r1, #17
	lsls r2, r2, #18
	bl Func_02005b14
	b .L_ja_0200d90e
.L_ja_0200d90a:
	bl Func_02004cf4
.L_ja_0200d90e:
	movs r1, #0
	movs r0, #16
	bl Func_02005c74
	movs r0, #16
	bl Object_GetById
	ldr r2, [r0, #80]
	movs r3, #0
	strh r3, [r2, #18]
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r0, #24]
	str r3, [r0, #28]
	ldr r3, [r0, #8]
	movs r2, #224
	lsls r2, r2, #12
	adds r3, r3, r2
	str r3, [r0, #8]
	ldr r3, [r0, #16]
	movs r2, #128
	lsls r2, r2, #12
	adds r3, r3, r2
	str r3, [r0, #16]
	movs r1, #6
	movs r0, #16
	bl Object_SetModeById
	ldr r3, .L_ja_0200d964
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #14
	bne .L_ja_0200d960
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #238
	bl GameFlag_SetBit
.L_ja_0200d960:
	pop {pc}
	.2byte 0x0000
.L_ja_0200d964:
	.4byte gPartyState
.L_ja_0200d968:
	.4byte 0x027a0000
	.else
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #93
	str r2, [r3]
	bl Func_02003288
	ldr r3, .L_0200d984
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #9
	bne .L_0200d90e
	movs r0, #254
	lsls r0, r0, #3
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200d90a
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_02005b0c
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #238
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200d8f6
	movs r3, #176
	movs r1, #217
	lsls r3, r3, #8
	movs r0, #8
	lsls r1, r1, #17
	ldr r2, .L_0200d988
	bl Func_02005b14
	b .L_0200d90e
.L_0200d8f6:
	movs r3, #176
	movs r1, #182
	movs r2, #144
	lsls r3, r3, #8
	movs r0, #8
	lsls r1, r1, #17
	lsls r2, r2, #18
	bl Func_02005b14
	b .L_0200d90e
.L_0200d90a:
	bl Func_02004cf4
.L_0200d90e:
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200d930
	movs r0, #16
	bl Object_GetById
	ldr r2, .L_0200d98c
	ldr r3, [r0, #8]
	adds r3, r3, r2
	str r3, [r0, #8]
	ldr r2, .L_0200d990
	ldr r3, [r0, #16]
	adds r3, r3, r2
	str r3, [r0, #16]
.L_0200d930:
	movs r1, #0
	movs r0, #16
	bl Func_02005c74
	movs r0, #16
	bl Object_GetById
	ldr r2, [r0, #80]
	movs r3, #0
	strh r3, [r2, #18]
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r0, #24]
	str r3, [r0, #28]
	ldr r3, [r0, #8]
	movs r2, #224
	lsls r2, r2, #12
	adds r3, r3, r2
	str r3, [r0, #8]
	ldr r3, [r0, #16]
	movs r2, #128
	lsls r2, r2, #12
	adds r3, r3, r2
	str r3, [r0, #16]
	movs r1, #6
	movs r0, #16
	bl Object_SetModeById
	ldr r3, .L_0200d984
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #14
	bne .L_0200d982
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #238
	bl GameFlag_SetBit
.L_0200d982:
	pop {pc}
.L_0200d984:
	.4byte gPartyState
.L_0200d988:
	.4byte 0x027a0000
.L_0200d98c:
	.4byte 0xfff20000
.L_0200d990:
	.4byte 0xfff80000
	.endif
	.section .bss,"aw",%nobits
	.space 0x00000004
	.global gData_02007948
gData_02007948:
	.space 0x00000008
	.global gData_02007950
gData_02007950:
