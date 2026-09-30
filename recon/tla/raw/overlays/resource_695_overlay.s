.syntax unified
	.thumb
	.section .text.x0200804c,"ax",%progbits
	.global Func_0200004c
	.thumb_func
Func_0200004c:
	push {r5, lr}
	adds r5, r1, #0
	sub sp, #8
	cmp r5, #8
	bne .L_02008072
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #169
	bl GameFlag_SetBit
	movs r3, #2
	str r3, [sp, #0]
	movs r0, #0
	movs r1, #2
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl Func_02001060
.L_02008072:
	cmp r5, #10
	bne .L_02008094
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #170
	bl GameFlag_SetBit
	movs r3, #14
	movs r2, #38
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #32
	movs r2, #1
	movs r3, #1
	bl Func_02001060
.L_02008094:
	add sp, #8
	pop {r5, pc}
	.section .text.x02008098,"ax",%progbits
	.global Func_02000098
	.thumb_func
Func_02000098:
	push {r5, r6, r7, lr}
	bl Object_GetById
	adds r5, r0, #0
	adds r7, r5, #0
	adds r7, #85
	movs r3, #3
	movs r6, #60
	strb r3, [r7]
	b .L_020080ae
.L_020080ac:
	subs r6, #1
.L_020080ae:
	cmp r6, #0
	beq .L_020080be
	movs r0, #1
	bl WaitFrames
	ldr r3, [r5, #40]
	cmp r3, #0
	bne .L_020080ac
.L_020080be:
	movs r0, #10
	bl WaitFrames
	adds r0, r5, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r3, #0
	strb r3, [r7]
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x020080d4,"ax",%progbits
	.global Func_020000d4
	.thumb_func
Func_020000d4:
	push {lr}
	sub sp, #8
	movs r3, #99
	movs r2, #18
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #99
	movs r1, #10
	movs r2, #4
	movs r3, #3
	bl Func_02001060
	add sp, #8
	pop {pc}
	.section .text.x020080f0,"ax",%progbits
	.global Func_020000f0
	.thumb_func
Func_020000f0:
	push {r5, r6, r7, lr}
	adds r7, r1, #0
	adds r0, r7, #0
	sub sp, #8
	bl Object_GetById
	adds r5, r0, #0
	ldr r3, [r5, #8]
	asrs r6, r3, #20
	cmp r6, #38
	bne .L_02008130
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #168
	bl GameFlag_SetBit
	adds r3, r5, #0
	adds r3, #35
	movs r2, #0
	strb r2, [r3]
	adds r0, r7, #0
	bl Func_02000098
	movs r3, #19
	str r3, [sp, #4]
	movs r0, #40
	movs r1, #19
	movs r2, #1
	movs r3, #1
	str r6, [sp, #0]
	bl Func_02001060
.L_02008130:
	movs r3, #99
	movs r2, #18
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #99
	movs r1, #6
	movs r2, #4
	movs r3, #3
	bl Func_02001060
	add sp, #8
	pop {r5, r6, r7, pc}
	.section .text.x02008148,"ax",%progbits
	.global Func_02000148
	.thumb_func
Func_02000148:
	push {r5, lr}
	movs r0, #16
	sub sp, #8
	bl Object_GetById
	adds r5, r0, #0
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #2
	bl GameFlag_SetBit
	adds r2, r5, #0
	movs r3, #0
	adds r2, #85
	adds r5, #35
	strb r3, [r2]
	strb r3, [r5]
	movs r2, #42
	movs r3, #6
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #8
	movs r1, #42
	movs r2, #1
	movs r3, #1
	bl Func_02001060
	add sp, #8
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x02008184,"ax",%progbits
	.global Func_02000184
	.thumb_func
Func_02000184:
	push {r5, r6, r7, lr}
	movs r0, #17
	bl Object_GetById
	ldr r3, .L_02008308
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	adds r7, r0, #0
	ldr r0, [r3]
	bl Object_GetById
	ldr r3, [r0, #8]
	asrs r2, r3, #19
	ldr r3, [r0, #16]
	asrs r1, r3, #19
	ldr r3, [r7, #8]
	asrs r3, r3, #19
	cmp r2, #36
	bgt .L_020081b2
	cmp r3, #37
	ble .L_020081b2
	b .L_02008306
.L_020081b2:
	cmp r2, #37
	ble .L_020081bc
	cmp r3, #36
	bgt .L_020081bc
	b .L_02008306
.L_020081bc:
	movs r6, #1
	cmp r2, #36
	ble .L_020081c4
	negs r6, r6
.L_020081c4:
	movs r5, #1
	negs r5, r5
	cmp r1, #95
	bgt .L_020081ce
	movs r5, #1
.L_020081ce:
	cmp r5, #1
	bne .L_020082a0
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020082a0
	movs r0, #129
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020081ee
	b .L_02008306
.L_020081ee:
	bl Func_02001080
	movs r0, #0
	bl Func_020011e0
	movs r0, #17
	movs r1, #4
	movs r2, #20
	bl ObjectMotion_Launch
	movs r2, #32
	movs r1, #0
	movs r0, #17
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r0, #17
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #17
	movs r1, #1
	bl Func_02001188
	movs r0, #17
	movs r1, #2
	bl ObjectMotion_SetVariantCallback
	movs r1, #128
	movs r0, #17
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #8
	negs r1, r1
	movs r2, #0
	movs r0, #17
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r0, #17
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r2, #20
	movs r0, #17
	movs r1, #4
	bl ObjectMotion_Launch
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #17
	bl Func_02001168
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #17
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	strb r3, [r0]
	movs r1, #8
	movs r2, #0
	movs r0, #17
	bl ObjectMotion_OffsetPositionAndReset
	movs r0, #17
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #17
	bl Func_02001160
	movs r0, #30
	bl Battle_WaitMode0
	bl Func_02001088
	movs r0, #129
	lsls r0, r0, #2
	bl GameFlag_SetBit
	b .L_02008306
.L_020082a0:
	bl Func_02001080
	movs r0, #0
	bl Func_020011e0
	lsls r5, r5, #5
	movs r0, #17
	movs r1, #4
	movs r2, #20
	bl ObjectMotion_Launch
	adds r2, r5, #0
	movs r1, #0
	movs r0, #17
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r0, #17
	bl ObjectMotion_CommitCurrentPositionAndActivate
	lsls r1, r6, #4
	movs r2, #0
	movs r0, #17
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r0, #17
	bl ObjectMotion_CommitCurrentPositionAndActivate
	lsls r1, r6, #1
	adds r1, r1, r6
	lsls r1, r1, #4
	movs r2, #0
	movs r0, #17
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r3, #128
	lsls r3, r3, #12
	str r3, [r7, #40]
	movs r0, #17
	negs r5, r5
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #17
	movs r1, #0
	adds r2, r5, #0
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r0, #17
	bl ObjectMotion_CommitCurrentPositionAndActivate
	bl Func_02001088
.L_02008306:
	pop {r5, r6, r7, pc}
.L_02008308:
	.4byte gPartyState
	.section .text.x0200830c,"ax",%progbits
	.global Func_0200030c
	.thumb_func
Func_0200030c:
	push {lr}
	movs r0, #131
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #6
	bl GameFlag_SetBit
	movs r0, #17
	movs r1, #35
	bl Func_020011a8
	pop {pc}
	.section .text.x0200832c,"ax",%progbits
	.global Func_0200032c
	.thumb_func
Func_0200032c:
	push {r5, lr}
	movs r1, #0
	adds r5, r0, #0
	bl ObjectDispatch_SetSingleChildField26
	ldr r1, [r5, #80]
	movs r3, #13
	ldrb r2, [r1, #9]
	negs r3, r3
	ands r3, r2
	movs r2, #8
	orrs r3, r2
	strb r3, [r1, #9]
	adds r5, #85
	movs r3, #0
	strb r3, [r5]
	movs r0, #0
	pop {r5, pc}
	.section .text.x02008358,"ax",%progbits
	.global Func_02000358
	.thumb_func
Func_02000358:
	push {lr}
	movs r0, #128
	lsls r0, r0, #2
	bl GameFlag_SetBit
	pop {pc}
	.section .text.x02008364,"ax",%progbits
	.global Func_02000364
	.thumb_func
Func_02000364:
	push {lr}
	movs r0, #129
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
	pop {pc}
	.2byte 0x0000
	.section .text.x02008374,"ax",%progbits
	.global Func_02000374
	.thumb_func
Func_02000374:
	push {lr}
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #2
	bl GameFlag_SetBit
	pop {pc}
	.2byte 0x0000
	.section .text.x02008384,"ax",%progbits
	.global Func_02000384
	.thumb_func
Func_02000384:
	push {lr}
	movs r0, #130
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
	pop {pc}
	.2byte 0x0000
	.section .text.x0200839c,"ax",%progbits
	.global Func_0200039c
	.thumb_func
Func_0200039c:
	push {lr}
	bl Func_02001080
	movs r0, #0
	bl Func_020011e0
	movs r1, #200
	movs r2, #1
	movs r0, #22
	lsls r1, r1, #1
	negs r2, r2
	bl Func_02001098
	cmp r0, #0
	bne .L_020083c4
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #115
	bl GameFlag_ClearBit
.L_020083c4:
	bl Func_02001088
	pop {pc}
	.2byte 0x0000
	.section .text.x020083cc,"ax",%progbits
	.global Func_020003cc
	.thumb_func
Func_020003cc:
	push {r5, lr}
	movs r5, #160
	lsls r5, r5, #7
	movs r1, #231
	movs r2, #199
	adds r3, r5, #0
	movs r0, #19
	lsls r1, r1, #16
	lsls r2, r2, #16
	bl Func_020010f0
	movs r1, #220
	movs r2, #191
	adds r3, r5, #0
	movs r0, #20
	lsls r1, r1, #16
	lsls r2, r2, #16
	bl Func_020010f0
	movs r1, #204
	movs r2, #191
	movs r0, #21
	lsls r1, r1, #16
	lsls r2, r2, #16
	adds r3, r5, #0
	bl Func_020010f0
	pop {r5, pc}
	.section .text.x02008404,"ax",%progbits
	.global Func_02000404
	.thumb_func
Func_02000404:
	push {lr}
	movs r1, #200
	movs r0, #22
	lsls r1, r1, #1
	bl Func_02001200
	movs r3, #192
	movs r1, #212
	movs r2, #199
	lsls r3, r3, #8
	movs r0, #22
	lsls r1, r1, #16
	lsls r2, r2, #16
	bl Func_020010f0
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #115
	bl GameFlag_SetBit
	pop {pc}
	.2byte 0x0000
	.section .text.x02008430,"ax",%progbits
	.global Func_02000430
	.thumb_func
Func_02000430:
	push {r5, lr}
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #33
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008442
	b .L_0200882c
.L_02008442:
	movs r0, #0
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200844e
	b .L_0200882c
.L_0200844e:
	bl Func_02001080
	movs r0, #0
	bl Func_020011e0
	movs r0, #167
	lsls r0, r0, #4
	bl GameFlag_SetBit
	ldr r0, .L_02008830
	bl Func_02001138
	movs r0, #4
	movs r1, #1
	bl Object_SetModeById
	movs r0, #19
	movs r1, #0
	movs r2, #1
	bl Func_02001148
	movs r2, #221
	movs r0, #4
	movs r1, #200
	bl ObjectMotion_SetPositionAndReset
	movs r1, #2
	movs r0, #4
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl Battle_WaitMode0
	movs r1, #16
	movs r0, #4
	negs r1, r1
	movs r2, #8
	bl ObjectMotion_OffsetPositionAndReset
	movs r3, #192
	movs r0, #0
	movs r1, #0
	movs r2, #0
	lsls r3, r3, #8
	bl Func_020011f0
	movs r3, #192
	lsls r3, r3, #8
	movs r1, #8
	movs r2, #16
	movs r0, #18
	bl Func_020011f0
	movs r0, #0
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #18
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #0
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #4
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #18
	bl ObjectMotion_ArmCallback
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #0
	bl Func_02001160
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #18
	bl Func_02001160
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #40
	movs r0, #4
	bl Func_02001160
	movs r0, #199
	movs r1, #1
	movs r2, #177
	lsls r0, r0, #16
	negs r1, r1
	lsls r2, r2, #16
	movs r3, #1
	movs r5, #128
	bl Motion_CamBounds
	lsls r5, r5, #7
	bl Func_02001180
	movs r1, #199
	movs r2, #130
	adds r3, r5, #0
	movs r0, #19
	lsls r1, r1, #16
	lsls r2, r2, #16
	bl Func_020010f0
	movs r1, #231
	movs r2, #199
	movs r0, #19
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #199
	movs r2, #130
	adds r3, r5, #0
	movs r0, #20
	lsls r1, r1, #16
	lsls r2, r2, #16
	bl Func_020010f0
	movs r1, #220
	movs r2, #191
	movs r0, #20
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #199
	movs r2, #130
	adds r3, r5, #0
	movs r0, #21
	lsls r1, r1, #16
	lsls r2, r2, #16
	bl Func_020010f0
	movs r0, #21
	movs r1, #204
	movs r2, #191
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r0, #199
	movs r1, #1
	movs r2, #225
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #16
	lsls r0, r0, #16
	bl Motion_CamBounds
	bl Func_02001180
	movs r0, #19
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r1, #160
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #19
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r1, #160
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #20
	bl ObjectMotion_ArmCallback
	movs r0, #21
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r1, #160
	movs r0, #21
	lsls r1, r1, #7
	bl Func_02001158
	movs r0, #21
	movs r1, #2
	bl Motion_SetVarCbAndRefresh
	movs r0, #21
	movs r1, #0
	movs r2, #5
	bl Func_02001148
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #18
	bl Func_02001160
	movs r0, #18
	movs r1, #0
	movs r2, #5
	bl Func_02001148
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #19
	bl Func_02001160
	movs r2, #5
	movs r0, #19
	movs r1, #0
	bl Func_02001148
	movs r1, #2
	movs r0, #20
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl Battle_WaitMode0
	movs r0, #20
	movs r1, #0
	movs r2, #5
	bl Func_02001148
	movs r1, #8
	adds r1, #255
	movs r2, #30
	movs r0, #21
	bl Func_02001160
	movs r2, #5
	movs r0, #21
	movs r1, #0
	bl Func_02001148
	movs r1, #224
	movs r0, #0
	lsls r1, r1, #8
	bl Func_02001158
	movs r1, #3
	movs r0, #0
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #5
	movs r0, #0
	movs r1, #0
	bl Func_02001148
	movs r1, #2
	movs r0, #19
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl Battle_WaitMode0
	movs r0, #19
	movs r1, #0
	movs r2, #5
	bl Func_02001148
	movs r1, #4
	movs r2, #0
	movs r0, #18
	bl Object_LinkPair
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #2
	adds r1, #255
	movs r2, #30
	movs r0, #0
	bl Func_02001160
	movs r0, #0
	movs r1, #0
	movs r2, #5
	bl Func_02001148
	movs r1, #8
	adds r1, #255
	movs r2, #30
	movs r0, #20
	bl Func_02001160
	movs r0, #20
	movs r1, #0
	movs r2, #5
	bl Func_02001148
	movs r1, #224
	movs r2, #0
	movs r0, #4
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r1, #224
	movs r0, #18
	lsls r1, r1, #8
	bl Func_02001158
	movs r0, #18
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #21
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #21
	movs r1, #0
	movs r2, #5
	bl Func_02001148
	movs r1, #10
	adds r1, #255
	movs r2, #30
	movs r0, #18
	bl Func_02001160
	movs r2, #5
	movs r0, #18
	movs r1, #0
	bl Func_02001148
	movs r0, #21
	movs r1, #4
	bl Object_SetModeById
	movs r0, #19
	movs r1, #4
	bl Object_SetModeById
	movs r0, #20
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #19
	movs r1, #0
	movs r2, #5
	bl Func_02001148
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #4
	bl Func_02001160
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #18
	bl Func_02001160
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #0
	bl Func_02001160
	movs r0, #0
	movs r1, #0
	movs r2, #5
	bl Func_02001148
	movs r1, #21
	movs r2, #0
	movs r0, #20
	bl Object_LinkPair
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #8
	adds r1, #255
	movs r2, #30
	movs r0, #20
	bl Func_02001160
	movs r0, #20
	movs r1, #0
	movs r2, #5
	bl Func_02001148
	movs r1, #160
	movs r0, #20
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	movs r2, #0
	movs r0, #21
	lsls r1, r1, #7
	bl ObjectMotion_ArmCallback
	movs r1, #224
	lsls r1, r1, #8
	movs r0, #18
	bl Func_02001158
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #18
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r2, #5
	movs r0, #18
	movs r1, #0
	bl Func_02001148
	movs r0, #21
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #21
	movs r1, #0
	movs r2, #5
	bl Func_02001148
	movs r1, #16
	movs r0, #20
	negs r1, r1
	movs r2, #24
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r1, #16
	movs r0, #21
	negs r1, r1
	movs r2, #24
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r1, #16
	negs r1, r1
	movs r2, #24
	movs r0, #19
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r0, #19
	bl ObjectMotion_CommitCurrentPositionAndActivate
	ldr r3, .L_02008834
	movs r2, #166
	lsls r2, r2, #1
	adds r2, #255
	adds r3, r3, r2
	movs r2, #2
	strb r2, [r3]
	ldr r5, .L_02008838
	movs r1, #20
	adds r0, r5, #0
	bl Party_SetFields1eeAnd1f0
	adds r0, r5, #0
	movs r1, #21
	bl Party_SetFields1f2And1f4
	movs r0, #102
	movs r1, #2
	bl Func_020011a0
.L_0200882c:
	pop {r5, pc}
	.2byte 0x0000
.L_02008830:
	.4byte 0x00002f5d
.L_02008834:
	.4byte gPartyState
.L_02008838:
	.4byte 0x000000f0
	.section .text.x0200883c,"ax",%progbits
	.global Func_0200083c
	.thumb_func
Func_0200083c:
	push {r5, lr}
	bl Func_02001080
	movs r0, #0
	bl Func_020011e0
	ldr r3, .L_02008874
	movs r2, #166
	lsls r2, r2, #1
	adds r2, #255
	adds r3, r3, r2
	movs r2, #2
	strb r2, [r3]
	ldr r5, .L_02008878
	movs r1, #20
	adds r0, r5, #0
	bl Party_SetFields1eeAnd1f0
	adds r0, r5, #0
	movs r1, #21
	bl Party_SetFields1f2And1f4
	movs r0, #102
	movs r1, #2
	bl Func_020011a0
	pop {r5, pc}
	.2byte 0x0000
.L_02008874:
	.4byte gPartyState
.L_02008878:
	.4byte 0x000000f0
	.section .text.x0200887c,"ax",%progbits
	.global Func_0200087c
	.thumb_func
Func_0200087c:
	push {r5, lr}
	bl Func_02001080
	movs r0, #0
	bl Func_020011e0
	movs r0, #199
	movs r1, #1
	negs r1, r1
	ldr r2, .L_02008a14
	movs r3, #0
	lsls r0, r0, #16
	bl Motion_CamBounds
	movs r5, #160
	ldr r0, .L_02008a18
	bl Func_02001138
	lsls r5, r5, #8
	movs r1, #184
	movs r2, #229
	movs r0, #4
	lsls r1, r1, #16
	lsls r2, r2, #16
	adds r3, r5, #0
	bl Func_020010f0
	movs r1, #200
	movs r2, #221
	movs r0, #0
	lsls r1, r1, #16
	lsls r2, r2, #16
	adds r3, r5, #0
	bl Func_020010f0
	movs r3, #192
	movs r1, #218
	movs r2, #160
	lsls r3, r3, #8
	lsls r2, r2, #17
	movs r0, #18
	lsls r1, r1, #16
	bl Func_020010f0
	movs r0, #4
	movs r1, #19
	bl Object_SetModeById
	movs r0, #0
	movs r1, #39
	bl Object_SetModeById
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	movs r1, #2
	movs r0, #21
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl Battle_WaitMode0
	movs r1, #8
	adds r1, #255
	movs r2, #30
	movs r0, #21
	bl Func_02001160
	movs r0, #21
	movs r1, #0
	movs r2, #5
	bl Func_02001148
	movs r1, #8
	adds r1, #255
	movs r2, #30
	movs r0, #19
	bl Func_02001160
	movs r0, #19
	movs r1, #0
	bl UiText_OpenMessageAtObject
	movs r0, #1
	bl Func_020011e8
	cmp r0, #0
	bne .L_02008950
	bl UiWork_FinalizePendingCore
	movs r2, #5
	movs r0, #20
	movs r1, #0
	bl Func_02001148
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_02008970
.L_02008950:
	bl UiWork_FinalizePendingCore
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #20
	adds r3, #1
	strh r3, [r2]
	movs r1, #0
	movs r2, #5
	bl Func_02001148
.L_02008970:
	movs r0, #21
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r2, #5
	movs r0, #21
	movs r1, #0
	bl Func_02001148
	movs r1, #3
	movs r0, #19
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #5
	movs r0, #19
	movs r1, #0
	bl Func_02001148
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #8
	lsls r1, r1, #5
	bl Func_02001170
	movs r0, #199
	movs r1, #1
	movs r3, #1
	negs r1, r1
	ldr r2, .L_02008a1c
	lsls r0, r0, #16
	bl Motion_CamBounds
	movs r0, #100
	bl Battle_WaitMode0
	movs r1, #6
	adds r1, #255
	movs r2, #1
	movs r0, #18
	bl Func_02001160
	movs r0, #20
	bl Battle_WaitMode0
	ldr r5, .L_02008a20
	movs r2, #242
	lsls r2, r2, #1
	adds r3, r5, r2
	movs r2, #0
	ldrsh r0, [r3, r2]
	movs r2, #243
	lsls r2, r2, #1
	adds r3, r5, r2
	movs r2, #0
	ldrsh r1, [r3, r2]
	bl Func_02001190
	movs r2, #240
	lsls r2, r2, #1
	adds r3, r5, r2
	movs r2, #0
	ldrsh r0, [r3, r2]
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r5, r2
	movs r2, #0
	ldrsh r1, [r3, r2]
	bl Func_02001208
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	str r2, [r3]
	pop {r5, pc}
	.2byte 0x0000
.L_02008a14:
	.4byte 0x01110000
.L_02008a18:
	.4byte 0x00002f6e
.L_02008a1c:
	.4byte 0x01750000
.L_02008a20:
	.4byte gPartyState
	.section .text.x02008a24,"ax",%progbits
	.global Func_02000a24
	.thumb_func
Func_02000a24:
	push {r5, lr}
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #113
	bl GameFlag_ClearBit
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #114
	bl GameFlag_SetBit
	bl Func_02001080
	movs r0, #0
	bl Func_020011e0
	movs r0, #199
	movs r1, #1
	negs r1, r1
	ldr r2, .L_02008d74
	movs r3, #0
	lsls r0, r0, #16
	bl Motion_CamBounds
	movs r5, #224
	ldr r0, .L_02008d78
	bl Func_02001138
	lsls r5, r5, #8
	movs r1, #184
	movs r2, #229
	adds r3, r5, #0
	movs r0, #4
	lsls r1, r1, #16
	lsls r2, r2, #16
	bl Func_020010f0
	movs r1, #200
	movs r2, #221
	adds r3, r5, #0
	movs r0, #0
	lsls r1, r1, #16
	lsls r2, r2, #16
	bl Func_020010f0
	movs r1, #208
	movs r2, #237
	adds r3, r5, #0
	lsls r2, r2, #16
	movs r0, #18
	lsls r1, r1, #16
	bl Func_020010f0
	movs r0, #20
	movs r1, #8
	bl Object_SetModeById
	movs r0, #19
	movs r1, #8
	bl Object_SetModeById
	movs r0, #21
	movs r1, #8
	bl Object_SetModeById
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	movs r1, #2
	movs r0, #20
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl Battle_WaitMode0
	movs r0, #21
	movs r1, #0
	movs r2, #5
	bl Func_02001148
	movs r0, #19
	movs r1, #0
	movs r2, #5
	bl Func_02001148
	movs r0, #20
	movs r1, #0
	movs r2, #5
	bl Func_02001148
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #19
	bl Func_02001160
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #21
	bl Func_02001160
	movs r1, #8
	adds r1, #255
	movs r2, #30
	movs r0, #0
	bl Func_02001160
	movs r2, #5
	movs r0, #0
	movs r1, #0
	bl Func_02001148
	movs r1, #3
	movs r0, #21
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #21
	movs r1, #0
	movs r2, #5
	bl Func_02001148
	movs r1, #10
	adds r1, #255
	movs r2, #30
	movs r0, #19
	bl Func_02001160
	movs r1, #0
	movs r0, #19
	bl UiText_OpenMessageAtObject
	b .L_02008b5c
.L_02008b36:
	movs r1, #129
	movs r2, #30
	lsls r1, r1, #1
	movs r0, #19
	bl Func_02001160
	movs r0, #20
	movs r1, #0
	bl UiText_OpenMessageAtObject
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	subs r3, #1
	strh r3, [r2]
.L_02008b5c:
	movs r0, #0
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_02008b36
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #20
	adds r3, #1
	strh r3, [r2]
	movs r1, #0
	movs r2, #5
	bl Func_02001148
	movs r0, #20
	movs r1, #1
	bl Object_SetModeById
	movs r0, #19
	movs r1, #1
	bl Object_SetModeById
	movs r1, #1
	movs r0, #21
	bl Object_SetModeById
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #21
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #0
	movs r2, #5
	movs r0, #21
	bl Func_02001148
	bl Func_02000404
	movs r0, #60
	bl Battle_WaitMode0
	movs r1, #2
	movs r2, #30
	adds r1, #255
	movs r0, #0
	bl Func_02001160
	movs r1, #3
	movs r0, #19
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #5
	movs r0, #19
	movs r1, #0
	bl Func_02001148
	movs r1, #128
	lsls r1, r1, #7
	movs r0, #0
	bl Func_02001158
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #20
	bl Func_02001160
	movs r1, #224
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #20
	movs r1, #0
	movs r2, #5
	bl Func_02001148
	movs r1, #10
	adds r1, #255
	movs r2, #30
	movs r0, #21
	bl Func_02001160
	movs r2, #5
	movs r0, #21
	movs r1, #0
	bl Func_02001148
	movs r1, #3
	movs r0, #19
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #5
	movs r0, #19
	movs r1, #0
	bl Func_02001148
	movs r1, #3
	movs r0, #20
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #5
	movs r0, #20
	movs r1, #0
	bl Func_02001148
	movs r0, #0
	movs r1, #3
	bl Object_SetModeById
	movs r0, #4
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #18
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #128
	movs r2, #128
	movs r0, #19
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r1, #128
	movs r2, #128
	movs r0, #20
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #8
	movs r0, #21
	lsls r1, r1, #9
	bl ObjectMotion_SetSpeedParameters
	ldr r1, .L_02008d7c
	movs r0, #19
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #20
	bl Battle_WaitMode0
	ldr r5, .L_02008d80
	movs r0, #20
	adds r1, r5, #0
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #20
	bl Battle_WaitMode0
	adds r1, r5, #0
	movs r0, #21
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #100
	bl Battle_WaitMode0
	movs r0, #0
	movs r1, #4
	movs r2, #0
	bl Object_LinkPair
	movs r2, #0
	movs r1, #4
	movs r0, #18
	bl ObjectMotion_SetAngleToward
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #0
	movs r1, #3
	bl Object_SetModeById
	movs r0, #4
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #18
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #0
	movs r1, #2
	bl Object_SetModeById
	movs r0, #4
	bl Object_GetById
	cmp r0, #0
	beq .L_02008d2c
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #0
	bl ObjectMotion_ResetAndSetPosition
.L_02008d2c:
	movs r0, #0
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl Func_020010e8
	movs r0, #18
	movs r1, #2
	bl Object_SetModeById
	movs r0, #4
	bl Object_GetById
	cmp r0, #0
	beq .L_02008d5c
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #18
	bl ObjectMotion_ResetAndSetPosition
.L_02008d5c:
	movs r0, #18
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #18
	movs r1, #0
	movs r2, #0
	bl Func_020010e8
	bl Func_02001088
	pop {r5, pc}
	.2byte 0x0000
.L_02008d74:
	.4byte 0x01110000
.L_02008d78:
	.4byte 0x00002f74
.L_02008d7c:
	.4byte Data_020015e0
.L_02008d80:
	.4byte Data_0200161c
	.section .text.x02008d84,"ax",%progbits
	.global Func_02000d84
	.thumb_func
Func_02000d84:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r6, [r3, #108]
	bl Func_02001080
	movs r0, #0
	bl Func_020011e0
	movs r5, #8
.L_02008d98:
	adds r0, r5, #0
	bl Object_GetById
	cmp r0, #0
	beq .L_02008daa
	adds r2, r0, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
.L_02008daa:
	adds r5, #1
	cmp r5, #63
	bls .L_02008d98
	ldr r5, .L_02008e00
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
	ldr r0, [r5]
	bl Object_GetById
	movs r3, #0
	adds r0, #85
	strb r3, [r0]
	movs r1, #2
	ldr r0, [r5]
	bl Object_SetModeById
	movs r2, #8
	movs r1, #2
	negs r2, r2
	ldr r0, [r5]
	bl ObjectMotion_SnapHeadingAndOffset
	movs r0, #5
	bl Battle_WaitMode0
	movs r0, #123
	bl Func_02001210
	movs r2, #170
	lsls r2, r2, #1
	adds r3, r6, r2
	movs r2, #0
	ldrsh r0, [r3, r2]
	bl Func_02001198
	bl Event_ClearStatus1c6
	bl Event_WaitValue1c8Frames
	bl Func_02001088
	pop {r5, r6, pc}
.L_02008e00:
	.4byte gPartyState
	.section .text.x02008e04,"ax",%progbits
	.global Func_02000e04
	.thumb_func
Func_02000e04:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	movs r0, #144
	adds r3, r3, r2
	lsls r0, r0, #4
	adds r2, #88
	str r2, [r3]
	adds r0, #169
	sub sp, #8
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008e30
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl Func_020010e8
.L_02008e30:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #170
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008e48
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_020010e8
.L_02008e48:
	movs r0, #83
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008e5c
	movs r0, #163
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008e66
.L_02008e5c:
	movs r0, #131
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
.L_02008e66:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #168
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008eaa
	movs r0, #15
	bl Object_GetById
	adds r5, r0, #0
	adds r3, r5, #0
	movs r2, #0
	adds r3, #85
	strb r2, [r3]
	subs r3, #50
	strb r2, [r3]
	movs r3, #154
	lsls r3, r3, #18
	str r3, [r5, #8]
	str r2, [r5, #12]
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r3, #38
	movs r2, #19
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #40
	movs r1, #19
	movs r2, #1
	movs r3, #1
	bl Func_02001060
.L_02008eaa:
	movs r0, #16
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #16
	movs r1, #1
	bl Object_SetModeById
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008f0a
	movs r0, #16
	bl Object_GetById
	movs r1, #5
	adds r5, r0, #0
	movs r0, #16
	bl Object_SetModeById
	movs r0, #16
	bl Object_GetById
	movs r1, #0
	bl Object_SetPartAttribute
	adds r2, r5, #0
	movs r3, #0
	adds r2, #85
	strb r3, [r2]
	subs r2, #50
	strb r3, [r2]
	str r3, [r5, #12]
	movs r2, #42
	movs r3, #6
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #8
	movs r1, #42
	movs r2, #1
	movs r3, #1
	bl Func_02001060
.L_02008f0a:
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #6
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008f26
	movs r1, #164
	movs r2, #198
	movs r0, #17
	lsls r1, r1, #17
	lsls r2, r2, #18
	bl Func_020010e8
.L_02008f26:
	movs r0, #19
	bl Object_GetById
	movs r1, #1
	bl Object_SetPartAttribute
	movs r0, #20
	bl Object_GetById
	movs r1, #0
	bl Object_SetPartAttribute
	movs r0, #21
	bl Object_GetById
	movs r1, #2
	bl Object_SetPartAttribute
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008f6c
	ldr r3, .L_02008fe4
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	ldrh r3, [r3]
	movs r2, #128
	subs r3, #20
	lsls r3, r3, #16
	lsls r2, r2, #9
	cmp r3, r2
	bls .L_02008f7a
.L_02008f6c:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #113
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008fca
.L_02008f7a:
	bl Func_020003cc
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008fa0
	ldr r3, .L_02008fe4
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #20
	bne .L_02008fa0
	bl Func_02000a24
	b .L_02008fca
.L_02008fa0:
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008fca
	ldr r3, .L_02008fe4
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #21
	bne .L_02008fca
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #113
	bl GameFlag_SetBit
	bl Func_0200087c
.L_02008fca:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #115
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008fdc
	bl Func_02000404
.L_02008fdc:
	movs r0, #0
	add sp, #8
	pop {r5, pc}
	.2byte 0x0000
.L_02008fe4:
	.4byte gPartyState
	.section .text.x02008fe8,"ax",%progbits
	.global Func_02000fe8
	.thumb_func
Func_02000fe8:
	push {lr}
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #169
	sub sp, #8
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200900e
	movs r3, #2
	movs r2, #8
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #2
	movs r2, #1
	movs r3, #1
	bl Func_02001060
.L_0200900e:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #170
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009030
	movs r3, #14
	movs r2, #38
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #32
	movs r2, #1
	movs r3, #1
	bl Func_02001060
.L_02009030:
	movs r0, #0
	add sp, #8
	pop {pc}
	.2byte 0x0000
	.section .text.x02009038,"ax",%progbits
	.global Func_02001038
	.thumb_func
Func_02001038:
	push {lr}
	bl Func_020011c0
	pop {pc}
	.section .rodata.x02009218,"a",%progbits
	.global gSceneEntrances
gSceneEntrances:
	.4byte 0xffff0000
	.4byte 0x000000ac
	.4byte 0x40000095
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gSceneExits
gSceneExits:
	.4byte 0x000000f0
	.4byte 0x10138002
	.4byte 0xffffffff
	.4byte 0x102050f0
	.4byte 0xffffffff
	.4byte 0x103060f0
	.4byte 0xffffffff
	.4byte 0x104090f0
	.4byte 0xffffffff
	.4byte 0x105020f0
	.4byte 0xffffffff
	.4byte 0x106030f0
	.4byte 0xffffffff
	.4byte 0x107080f0
	.4byte 0xffffffff
	.4byte 0x108070f0
	.4byte 0xffffffff
	.4byte 0x109040f0
	.4byte 0xffffffff
	.4byte 0x10a39002
	.4byte 0xffffffff
	.4byte 0x000001ff
.L_020092a0:
	.4byte 0x00000016
	.4byte 0x0000000b
	.4byte 0x00018000
	.4byte 0x0000002e
	.4byte Func_0200032c
	.4byte 0x00000011
.L_020092b8:
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000000
	.4byte 0x00000011
	.global gScenePlacements
gScenePlacements:
	.4byte 0xffff0133
	.4byte .L_020092a0
	.4byte 0x00280000
	.4byte 0x00000000
	.4byte 0x00800000
	.4byte 0x0002c000
	.4byte 0xffff0149
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x0002c000
	.4byte 0xffff0133
	.4byte .L_020092a0
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x02600000
	.4byte 0x0002c000
	.4byte 0xffff0149
	.4byte 0x00000001
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x03280000
	.4byte 0x0002c000
	.4byte 0xffff0149
	.4byte 0x00000001
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x03280000
	.4byte 0x0002c000
	.4byte 0xffff0149
	.4byte 0x00000001
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x03180000
	.4byte 0x0002c000
	.4byte 0xffff0149
	.4byte 0x00000001
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x03180000
	.4byte 0x0002c000
	.4byte 0xffff0110
	.4byte .L_020092b8
	.4byte 0x02480000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x0002c000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x02a80000
	.4byte 0x00024000
	.4byte 0x005300f4
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x02f80000
	.4byte 0x00004000
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff00ef
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff00ef
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff00ef
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0002c000
	.4byte 0xffff0000
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gSceneEvents
gSceneEvents:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x0000c402
	.4byte 0xffff0002
	.4byte Func_02000d84
	.4byte 0x0000c402
	.4byte 0xffff0003
	.4byte Func_02000d84
	.4byte 0x0000c402
	.4byte 0xffff0004
	.4byte Func_02000d84
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000001
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x0000c402
	.4byte 0xffff0008
	.4byte Func_02000d84
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000001
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000002
	.4byte 0x02050032
	.4byte Func_02000184
	.4byte 0x00000602
	.4byte Data_02010020 + 0x9
	.4byte Func_02001038
	.4byte 0x00008602
	.4byte Data_02010020 + 0xa
	.4byte Func_02001038
	.4byte 0x00000602
	.4byte Data_02020004 + 0x27
	.4byte Func_02001038
	.4byte 0x00008602
	.4byte Data_02020004 + 0x28
	.4byte Func_02001038
	.4byte 0x00000002
	.4byte 0x0a700064
	.4byte Func_02000430
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte Func_0200030c
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte Func_0200083c
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte Func_0200083c
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte Func_0200083c
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte Func_0200039c
	.4byte 0x00004e15
	.4byte 0xffff0008
	.4byte Func_0200004c
	.4byte 0x00004e15
	.4byte 0xffff000a
	.4byte Func_0200004c
	.4byte 0x00008f15
	.4byte Data_02000000 + 0xb
	.4byte Func_02000358
	.4byte 0x00008f15
	.4byte Data_02010002 + 0xa
	.4byte Func_02000364
	.4byte 0x00008f15
	.4byte Data_02020004 + 0x9
	.4byte Func_02000374
	.4byte 0x00008f15
	.4byte Data_02030000 + 0xe
	.4byte Func_02000384
	.4byte 0x00001815
	.4byte 0x03020010
	.4byte Func_02000148
	.4byte 0x10008c15
	.4byte 0x09a8000f
	.4byte Func_020000d4
	.4byte 0x00008c15
	.4byte 0x09a8000f
	.4byte Func_020000f0
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte Func_020000f0
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020015e0
Data_020015e0:
.L_020095e0:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000030
	.4byte 0x00e80000
	.4byte 0x00e30000
	.4byte 0x00000001
	.4byte 0x00000030
	.4byte 0x00d90000
	.4byte 0x00f30000
	.4byte 0x00000001
	.4byte 0x00000030
	.4byte 0x00d90000
	.4byte 0x014e0000
	.4byte 0x00000001
	.4byte 0x00000026
	.global Data_0200161c
Data_0200161c:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000030
	.4byte 0x00e00000
	.4byte 0x00c30000
	.4byte 0x00000001
	.4byte 0x0000002b
	.4byte .L_020095e0
