.syntax unified
	.thumb
	.section .text.x02008070,"ax",%progbits
	.global Func_02000070
	.thumb_func
Func_02000070:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	mov r8, r0
	adds r6, r1, #0
	adds r7, r6, #0
	mov r5, r8
	adds r7, #8
	adds r5, #8
	mov r10, r2
	adds r0, r7, #0
	movs r2, #0
	adds r1, r5, #0
	mov r9, r2
	bl Func_02000038
	cmp r0, r10
	bge .L_020080d8
	mov r2, r8
	ldr r3, [r2, #16]
	ldr r0, [r6, #16]
	ldr r1, [r7]
	subs r0, r0, r3
	ldr r3, [r5]
	movs r5, #128
	subs r1, r1, r3
	bl ArcTan2
	lsls r0, r0, #16
	lsrs r0, r0, #16
	ldr r3, .L_020080e4
	lsls r5, r5, #5
	adds r1, r0, r5
	mov r5, r8
	ldrh r2, [r5, #6]
	adds r4, r0, r3
	movs r3, #240
	lsls r3, r3, #8
	ands r4, r3
	ands r1, r3
	ands r0, r3
	ands r3, r2
	cmp r0, r3
	beq .L_020080d4
	cmp r1, r3
	beq .L_020080d4
	cmp r4, r3
	bne .L_020080d8
.L_020080d4:
	movs r2, #1
	mov r9, r2
.L_020080d8:
	mov r0, r9
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_020080e4:
	.4byte 0xfffff000
	.section .text.x020080e8,"ax",%progbits
	.global Func_020000e8
	.thumb_func
Func_020000e8:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #108]
	ldr r3, .L_0200819c
	movs r2, #133
	lsls r2, r2, #2
	adds r7, r3, r2
	adds r6, r0, #0
	ldr r0, [r7]
	bl Object_GetById
	bl Func_02001d54
	adds r1, r0, #0
	adds r0, r6, #0
	bl Func_02001ba4
	movs r2, #179
	lsls r2, r2, #1
	adds r3, r5, r2
	ldrh r2, [r3]
	movs r3, #192
	lsls r3, r3, #2
	adds r3, #255
	adds r5, r6, #0
	ands r3, r2
	adds r5, #91
	cmp r3, #141
	bne .L_0200813e
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #224
	ldr r3, [r3]
	movs r2, #224
	lsls r2, r2, #3
	adds r2, #19
	adds r3, r3, r2
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	bne .L_0200818c
.L_0200813e:
	adds r3, r6, #0
	adds r3, #100
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #1
	beq .L_0200818c
	ldr r0, [r7]
	bl Object_GetById
	adds r1, r0, #0
	adds r0, r6, #0
	adds r0, #8
	adds r1, #8
	bl Func_02000038
	cmp r0, #11
	ble .L_02008182
	ldr r1, [r6, #104]
	adds r0, r6, #0
	movs r2, #18
	bl Func_02000070
	cmp r0, #0
	bne .L_0200818c
	ldr r0, [r7]
	bl Object_GetById
	movs r2, #26
	adds r1, r0, #0
	adds r0, r6, #0
	bl Func_02000070
	cmp r0, #0
	bne .L_0200818c
.L_02008182:
	movs r3, #0
	adds r0, r6, #0
	strb r3, [r5]
	movs r1, #2
	b .L_02008194
.L_0200818c:
	movs r3, #1
	adds r0, r6, #0
	strb r3, [r5]
	movs r1, #1
.L_02008194:
	bl Func_02001b7c
	movs r0, #0
	pop {r5, r6, r7, pc}
.L_0200819c:
	.4byte gPartyState
	.section .text.x0200862c,"ax",%progbits
	.balign 4
	.global Func_0200062c
	.thumb_func
Func_0200062c:
	push {r5, lr}
	ldr r3, .L_020088d8
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	ldr r0, [r3]
	bl Object_GetById
	ldr r3, [r0, #12]
	movs r2, #192
	lsls r2, r2, #15
	cmp r3, r2
	bgt .L_02008654
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #105
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008666
.L_02008654:
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r1, #173
	lsls r1, r1, #1
	adds r2, r3, r1
	movs r3, #1
	strh r3, [r2]
	b .L_020088d4
.L_02008666:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #102
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008682
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #104
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008694
.L_02008682:
	ldr r0, .L_020088dc
	bl Func_02001c6c
	movs r0, #1
	negs r0, r0
	movs r1, #0
	bl Func_02001c84
	b .L_020088d4
.L_02008694:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #104
	bl GameFlag_SetBit
	bl Func_02001bcc
	movs r0, #0
	bl Func_02001d04
	ldr r0, .L_020088e0
	bl Func_02001c6c
	movs r1, #218
	movs r2, #220
	lsls r2, r2, #18
	lsls r1, r1, #18
	movs r0, #12
	bl Func_02001c2c
	movs r0, #12
	bl Object_GetById
	movs r1, #43
	bl Animation_SetIndexAndInitObjects
	movs r0, #12
	movs r1, #3
	bl Object_SetModeById
	movs r1, #160
	movs r0, #12
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #12
	movs r1, #0
	movs r2, #10
	bl Func_02001c7c
	movs r0, #218
	movs r1, #1
	movs r2, #194
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #18
	lsls r0, r0, #18
	bl Camera_MoveWithinMapBounds
	bl Func_02001cc4
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r1, #10
	adds r1, #255
	movs r2, #30
	movs r0, #12
	bl Func_02001ca4
	movs r2, #10
	movs r0, #12
	movs r1, #0
	bl Func_02001c7c
	movs r0, #12
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #12
	movs r1, #0
	movs r2, #10
	bl Func_02001c7c
	movs r0, #218
	movs r1, #1
	movs r2, #214
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #18
	lsls r0, r0, #18
	bl Camera_MoveWithinMapBounds
	bl Func_02001cc4
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r0, #12
	movs r1, #0
	movs r2, #10
	bl Func_02001c7c
	movs r0, #218
	movs r1, #1
	movs r2, #232
	lsls r2, r2, #18
	lsls r0, r0, #18
	negs r1, r1
	movs r3, #1
	bl Camera_MoveWithinMapBounds
	bl Func_02001cc4
	movs r0, #133
	bl Func_02001bbc
	movs r2, #1
	adds r5, r0, #0
	negs r2, r2
	cmp r5, r2
	beq .L_02008778
	b .L_020088cc
.L_02008778:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #98
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008788
	b .L_020088cc
.L_02008788:
	ldr r0, .L_020088e4
	bl Func_02001c6c
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r1, #214
	movs r2, #238
	lsls r2, r2, #2
	movs r0, #4
	lsls r1, r1, #2
	bl ObjectMotion_SetPositionAndReset
	movs r1, #192
	lsls r1, r1, #8
	movs r0, #4
	bl Func_02001c94
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r1, #6
	adds r1, #255
	movs r2, #60
	movs r0, #4
	bl Func_02001ca4
	movs r0, #218
	movs r2, #198
	movs r3, #1
	adds r1, r5, #0
	lsls r2, r2, #18
	lsls r0, r0, #18
	bl Camera_MoveWithinMapBounds
	bl Func_02001cc4
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r1, #2
	adds r1, #255
	movs r2, #30
	movs r0, #12
	bl Func_02001ca4
	movs r0, #12
	movs r1, #0
	movs r2, #10
	bl Func_02001c7c
	movs r0, #218
	movs r2, #214
	movs r3, #1
	lsls r0, r0, #18
	adds r1, r5, #0
	lsls r2, r2, #18
	bl Camera_MoveWithinMapBounds
	movs r0, #12
	movs r1, #4
	movs r2, #0
	bl ObjectMotion_Launch
	movs r1, #218
	movs r2, #228
	lsls r1, r1, #2
	lsls r2, r2, #2
	movs r0, #12
	bl ObjectMotion_SetPositionAndReset
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r1, #128
	movs r2, #128
	movs r0, #12
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r1, #32
	movs r2, #0
	movs r0, #12
	negs r1, r1
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #192
	movs r0, #12
	lsls r1, r1, #6
	bl Func_02001c94
	movs r0, #212
	movs r2, #226
	movs r3, #1
	adds r1, r5, #0
	lsls r2, r2, #18
	lsls r0, r0, #18
	bl Camera_MoveWithinMapBounds
	bl Func_02001cc4
	movs r0, #15
	bl FieldEvent_WaitFrames
	movs r0, #12
	movs r1, #0
	movs r2, #5
	bl Func_02001c7c
	movs r1, #210
	movs r2, #230
	movs r0, #28
	lsls r1, r1, #18
	lsls r2, r2, #18
	bl Func_02001c2c
	movs r1, #210
	movs r2, #234
	movs r0, #27
	lsls r1, r1, #18
	lsls r2, r2, #18
	bl Func_02001c2c
	movs r1, #28
	movs r2, #27
	movs r0, #1
	bl Func_02001d2c
	movs r1, #28
	movs r2, #27
	movs r0, #1
	bl Func_02001d34
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r0, #12
	movs r1, #0
	movs r2, #5
	bl Func_02001c7c
	movs r2, #0
	movs r0, #12
	movs r1, #32
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #4
	movs r1, #1
	bl Camera_FollowActor
	movs r2, #32
	movs r0, #12
	movs r1, #0
	negs r2, r2
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #135
	lsls r0, r0, #4
	adds r0, #255
	bl GameFlag_SetBit
.L_020088cc:
	bl Func_020001bc
	bl Func_02001bd4
.L_020088d4:
	pop {r5, pc}
	.2byte 0x0000
.L_020088d8:
	.4byte gPartyState
.L_020088dc:
	.4byte MsgTheDoorDoesn
.L_020088e0:
	.4byte MsgAreYouKids
.L_020088e4:
	.4byte MsgWhatSThe
	.section .text.x02008b28,"ax",%progbits
	.global Func_02000b28
	.thumb_func
Func_02000b28:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	movs r1, #160
	lsls r1, r1, #1
	adds r1, r1, r3
	ldr r3, .L_02008bb0
	sub sp, #8
	ldrb r2, [r3]
	movs r3, #1
	eors r3, r2
	lsls r2, r3, #2
	adds r2, r2, r3
	ldr r3, .L_02008bb4
	lsls r2, r2, #6
	adds r2, r2, r3
	add r7, sp, #4
	movs r3, #0
	str r3, [r7]
	mov r10, r1
	mov r8, r2
.L_02008b5a:
	ldr r3, .L_02008bb8
	ldr r0, [r7]
	ldrb r3, [r3]
	mov r1, r10
	adds r0, r0, r3
	movs r2, #6
	ldrsh r3, [r1, r2]
	adds r0, r0, r3
	lsls r0, r0, #9
	bl Math_Sine
	str r0, [sp, #0]
	mov r3, r10
	movs r2, #2
	ldrsh r5, [r3, r2]
	ldr r6, [r7]
	asrs r0, r0, #15
	adds r5, r5, r0
	movs r1, #3
	adds r0, r6, #0
	bl __modsi3
	adds r5, r5, r0
	mov r1, r8
	subs r5, #1
	movs r2, #2
	adds r6, #1
	strh r5, [r1]
	add r8, r2
	str r6, [r7]
	cmp r6, #160
	bne .L_02008b5a
	ldr r3, .L_02008bb0
	movs r1, #1
	ldrb r2, [r3]
	add sp, #8
	eors r2, r1
	strb r2, [r3]
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008bb0:
	.4byte gData_02002590
.L_02008bb4:
	.4byte gData_020025a0
.L_02008bb8:
	.4byte gFrameCount
	.section .text.x02008bdc,"ax",%progbits
	.global Func_02000bdc
	.thumb_func
Func_02000bdc:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r1, #200
	adds r2, #85
	str r2, [r3]
	lsls r1, r1, #4
	ldr r0, .L_02008e64
	sub sp, #8
	bl Scheduler_AddOrUpdateCallback
	movs r0, #0
	bl Func_02001cfc
	movs r0, #11
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	movs r0, #9
	movs r1, #3
	bl ObjectMotion_SetActionVariant
	movs r1, #3
	movs r0, #29
	bl ObjectMotion_SetActionVariant
	movs r0, #21
	bl Object_GetById
	ldr r3, .L_02008e68
	movs r1, #3
	str r3, [r0, #108]
	movs r0, #22
	bl Object_SetModeById
	movs r1, #160
	movs r2, #238
	lsls r2, r2, #18
	movs r0, #18
	lsls r1, r1, #14
	bl Func_02001c2c
	ldr r1, .L_02008e6c
	movs r0, #18
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #18
	bl Object_GetById
	adds r6, r0, #0
	movs r0, #19
	bl Object_GetById
	ldr r5, .L_02008e70
	movs r1, #176
	movs r2, #226
	lsls r2, r2, #18
	str r0, [r6, #104]
	str r5, [r6, #108]
	movs r0, #19
	lsls r1, r1, #15
	bl Func_02001c2c
	ldr r1, .L_02008e74
	movs r0, #19
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #19
	bl Object_GetById
	adds r6, r0, #0
	movs r0, #18
	bl Object_GetById
	str r0, [r6, #104]
	movs r0, #144
	lsls r0, r0, #4
	str r5, [r6, #108]
	adds r0, #8
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008c9c
	movs r1, #220
	movs r2, #131
	movs r0, #30
	lsls r1, r1, #17
	lsls r2, r2, #19
	bl Func_02001c2c
.L_02008c9c:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #100
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008cd4
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #105
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008cd4
	movs r1, #228
	movs r2, #130
	movs r0, #24
	lsls r1, r1, #17
	lsls r2, r2, #18
	bl Func_02001c2c
	movs r1, #236
	movs r2, #130
	movs r0, #25
	lsls r1, r1, #17
	lsls r2, r2, #18
	bl Func_02001c2c
.L_02008cd4:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #105
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008cee
	movs r0, #26
	movs r1, #0
	movs r2, #0
	bl Func_02001c2c
	b .L_02008cf8
.L_02008cee:
	movs r0, #26
	bl Object_GetById
	ldr r3, .L_02008e68
	str r3, [r0, #108]
.L_02008cf8:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #102
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008d2a
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #105
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008d2a
	movs r1, #224
	movs r2, #254
	movs r0, #7
	lsls r1, r1, #14
	lsls r2, r2, #18
	bl Func_02001c2c
	movs r0, #7
	movs r1, #9
	bl Object_SetModeById
.L_02008d2a:
	bl Func_02001d24
	movs r1, #144
	lsls r1, r1, #4
	adds r1, #98
	movs r2, #8
	movs r3, #9
	movs r0, #0
	bl Func_02001d3c
	movs r0, #8
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r5, #32
	orrs r3, r5
	strb r3, [r0]
	movs r0, #9
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	orrs r5, r3
	strb r5, [r0]
	movs r0, #135
	lsls r0, r0, #4
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008da6
	movs r0, #28
	movs r1, #2
	bl ObjectMotion_SetActionVariant
	movs r0, #27
	movs r1, #2
	bl ObjectMotion_SetActionVariant
	movs r1, #210
	movs r2, #230
	movs r0, #28
	lsls r1, r1, #18
	lsls r2, r2, #18
	bl Func_02001c2c
	movs r1, #210
	movs r2, #234
	movs r0, #27
	lsls r1, r1, #18
	lsls r2, r2, #18
	bl Func_02001c2c
	movs r1, #135
	lsls r1, r1, #4
	movs r0, #1
	adds r1, #255
	movs r2, #28
	movs r3, #27
	bl Func_02001d3c
.L_02008da6:
	movs r0, #28
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #99
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008dcc
	movs r0, #10
	movs r1, #1
	bl Func_02001d4c
	bl Func_02000560
.L_02008dcc:
	movs r0, #129
	lsls r0, r0, #2
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008de2
	movs r0, #67
	movs r1, #0
	bl Object_SetWideSprite
.L_02008de2:
	movs r0, #193
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008df6
	movs r0, #68
	movs r1, #0
	bl Object_SetWideSprite
.L_02008df6:
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #6
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008e0c
	movs r0, #70
	movs r1, #0
	bl Object_SetWideSprite
.L_02008e0c:
	movs r0, #130
	lsls r0, r0, #2
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008e22
	movs r0, #71
	movs r1, #0
	bl Object_SetWideSprite
.L_02008e22:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #102
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008e44
	movs r3, #54
	movs r2, #45
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #3
	movs r1, #1
	movs r2, #1
	movs r3, #1
	bl Func_02001b8c
.L_02008e44:
	movs r0, #31
	bl Object_GetById
	adds r6, r0, #0
	adds r2, r6, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	movs r3, #128
	lsls r3, r3, #15
	movs r0, #0
	str r3, [r6, #20]
	str r3, [r6, #12]
	add sp, #8
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02008e64:
	.4byte Func_02000a6c
.L_02008e68:
	.4byte Func_02001b3c
.L_02008e6c:
	.4byte gData_02001d84
.L_02008e70:
	.4byte Func_020000e8
.L_02008e74:
	.4byte gData_02001e04
	.section .bss,"aw",%nobits
	.global gData_02002590
gData_02002590:
	.space 0x00000010
	.global gData_020025a0
gData_020025a0:
