.syntax unified
	.thumb
	.section .text.x0200806c,"ax",%progbits
	.balign 4
	.global Func_0200006c
	.thumb_func
Func_0200006c:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r6, [r3, #108]
	bl Func_02002b28
	movs r0, #0
	bl Func_02002c58
	movs r5, #8
.L_02008080:
	adds r0, r5, #0
	bl Object_GetById
	cmp r0, #0
	beq .L_02008092
	adds r2, r0, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
.L_02008092:
	adds r5, #1
	cmp r5, #63
	bls .L_02008080
	movs r3, #170
	lsls r3, r3, #1
	adds r6, r6, r3
	movs r3, #0
	ldrsh r5, [r6, r3]
	movs r0, #158
	bl Func_02002c90
	subs r5, #1
	ldr r0, .L_02008100
	lsls r5, r5, #3
	adds r3, r5, #4
	ldrh r1, [r0, r3]
	adds r3, r3, r0
	ldrh r2, [r3, #2]
	ldr r0, [r0, r5]
	bl Func_02002ae8
	ldr r5, .L_02008104
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
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
	bl ObjectMotion_CenterOnColumn
	movs r0, #10
	bl Battle_WaitMode0
	movs r3, #0
	ldrsh r0, [r6, r3]
	bl Func_02002c30
	bl Event_FinishScreenTransition
	bl Event_WaitTransitionFrames
	bl Func_02002b30
	pop {r5, r6, pc}
.L_02008100:
	.4byte Data_02003308
.L_02008104:
	.4byte gPartyState
	.section .text.x020086ac,"ax",%progbits
	.balign 4
	.global Func_020006ac
	.thumb_func
Func_020006ac:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	movs r0, #128
	adds r3, r3, r2
	lsls r0, r0, #4
	subs r2, #172
	str r2, [r3]
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020086d2
	bl Func_0200068c
	b .L_020087a0
.L_020086d2:
	movs r0, #26
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	movs r0, #26
	bl Object_GetById
	adds r6, r0, #0
	movs r0, #26
	bl Object_GetById
	ldrh r0, [r0, #32]
	movs r1, #3
	lsls r0, r0, #2
	bl __divsi3
	strh r0, [r6, #32]
	movs r0, #26
	bl Object_GetById
	ldr r1, [r0, #80]
	movs r3, #8
	ldrb r2, [r1, #26]
	movs r0, #26
	orrs r3, r2
	strb r3, [r1, #26]
	bl Object_GetById
	ldr r2, [r0, #80]
	movs r5, #3
	ldrb r3, [r2, #17]
	movs r0, #26
	ands r5, r3
	movs r3, #32
	orrs r5, r3
	strb r5, [r2, #17]
	bl Object_GetById
	ldr r2, [r0, #80]
	movs r0, #128
	movs r3, #1
	lsls r0, r0, #4
	strb r3, [r2, #25]
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008770
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #171
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008770
	movs r0, #27
	movs r1, #0
	movs r2, #0
	bl Func_02002b88
	movs r0, #28
	movs r1, #0
	movs r2, #0
	bl Func_02002b88
	movs r0, #160
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020087a0
	movs r0, #26
	movs r1, #0
	movs r2, #0
	bl Func_02002b88
	b .L_020087a0
.L_02008770:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008796
	movs r0, #145
	lsls r0, r0, #4
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008796
	movs r0, #160
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020087a0
.L_02008796:
	movs r0, #26
	movs r1, #0
	movs r2, #0
	bl Func_02002b88
.L_020087a0:
	ldr r3, .L_02008834
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #9
	bne .L_020087c2
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #19
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020087c2
	bl Func_02000f80
.L_020087c2:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008824
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #137
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008824
	ldr r3, .L_02008834
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #7
	bne .L_0200880a
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #142
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200880a
	bl Func_020019d8
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #142
	bl GameFlag_SetBit
.L_0200880a:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #142
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200882e
	movs r0, #33
	movs r1, #0
	movs r2, #0
	bl Func_02002b88
	b .L_0200882e
.L_02008824:
	movs r0, #66
	movs r1, #0
	movs r2, #0
	bl Func_02002b88
.L_0200882e:
	movs r0, #0
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02008834:
	.4byte gPartyState
	.section .text.x02008ac4,"ax",%progbits
	.balign 4
	.global Func_02000ac4
	.thumb_func
Func_02000ac4:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	bl Func_02002b28
	movs r0, #0
	bl Func_02002c58
	ldr r0, .L_02008ed0
	bl Func_02002bd0
	movs r0, #24
	movs r1, #0
	movs r2, #2
	bl Func_02002be0
	movs r0, #238
	movs r1, #1
	movs r2, #210
	movs r3, #1
	lsls r0, r0, #17
	negs r1, r1
	lsls r2, r2, #17
	bl Motion_CamBounds
	movs r0, #25
	movs r1, #3
	movs r2, #8
	bl ObjectMotion_OffsetPositionAndReset
	movs r1, #3
	negs r1, r1
	movs r2, #8
	movs r0, #24
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #25
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r1, #236
	movs r2, #213
	lsls r2, r2, #1
	lsls r1, r1, #1
	movs r0, #4
	bl ObjectMotion_SetPositionAndReset
	movs r0, #10
	bl Battle_WaitMode0
	ldr r3, .L_02008ed4
	movs r1, #192
	mov r8, r3
	movs r3, #133
	lsls r3, r3, #2
	add r8, r3
	mov r3, r8
	ldr r0, [r3]
	lsls r1, r1, #8
	bl Func_02002bf8
	movs r3, #192
	lsls r3, r3, #8
	movs r1, #16
	movs r2, #1
	movs r0, #32
	bl Func_02002c68
	bl Func_02002c28
	movs r0, #32
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #4
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #32
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #192
	movs r0, #4
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #32
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #32
	bl Motion_SetVarCbAndRefresh
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #2
	movs r1, #0
	movs r0, #32
	bl Func_02002be0
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #24
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #0
	movs r2, #2
	movs r0, #25
	bl Func_02002be0
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #25
	movs r2, #0
	movs r0, #24
	bl ObjectMotion_SetAngleToward
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #0
	movs r1, #24
	movs r0, #25
	bl ObjectMotion_SetAngleToward
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #24
	bl Motion_SetModeAndWaitAnimation
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #24
	movs r2, #0
	movs r0, #25
	bl ObjectMotion_SetAngleToward
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #0
	movs r0, #25
	movs r1, #32
	bl ObjectMotion_SetAngleToward
	movs r1, #3
	movs r0, #25
	bl Motion_SetModeAndWaitAnimation
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #160
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #24
	bl ObjectMotion_ArmCallback
	movs r0, #25
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r5, #254
	adds r3, r5, #0
	ands r3, r2
	strb r3, [r0]
	movs r1, #3
	movs r0, #25
	bl ObjectMotion_SetActionVariant
	movs r1, #202
	movs r2, #201
	movs r0, #25
	adds r1, #255
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r0, #25
	movs r1, #16
	movs r2, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #236
	movs r2, #210
	lsls r1, r1, #1
	lsls r2, r2, #1
	movs r0, #25
	bl ObjectMotion_SetPositionAndReset
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #242
	movs r2, #210
	lsls r1, r1, #1
	lsls r2, r2, #1
	movs r0, #25
	bl ObjectMotion_SetPositionAndReset
	movs r0, #25
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r6, #1
	orrs r3, r6
	movs r1, #128
	strb r3, [r0]
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #25
	bl ObjectMotion_ArmCallback
	movs r0, #60
	bl Battle_WaitMode0
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #25
	bl ObjectMotion_ArmCallback
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #25
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r2, #2
	movs r1, #0
	movs r0, #25
	bl Func_02002be0
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #24
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r2, #2
	movs r0, #24
	movs r1, #0
	bl Func_02002be0
	movs r1, #128
	lsls r1, r1, #7
	movs r0, #25
	bl Func_02002bf8
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #25
	bl Motion_SetModeAndWaitAnimation
	movs r0, #5
	bl Battle_WaitMode0
	movs r2, #2
	movs r1, #0
	movs r0, #25
	bl Func_02002be0
	movs r0, #25
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r1, #3
	ands r5, r3
	strb r5, [r0]
	movs r0, #25
	bl ObjectMotion_SetActionVariant
	movs r2, #16
	movs r0, #25
	movs r1, #0
	negs r2, r2
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #220
	movs r2, #200
	movs r0, #25
	adds r1, #255
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #16
	negs r1, r1
	movs r2, #0
	movs r0, #25
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #25
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r1, #1
	orrs r6, r3
	strb r6, [r0]
	movs r2, #198
	movs r0, #238
	movs r3, #1
	lsls r0, r0, #17
	negs r1, r1
	lsls r2, r2, #17
	bl Motion_CamBounds
	movs r1, #204
	movs r2, #208
	movs r0, #25
	adds r1, #255
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #128
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #25
	bl ObjectMotion_ArmCallback
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #2
	adds r1, #255
	movs r2, #50
	movs r0, #32
	bl Func_02002c08
	movs r2, #2
	movs r1, #0
	movs r0, #32
	bl Func_02002be0
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #24
	bl Motion_SetVarCbAndRefresh
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #0
	movs r2, #2
	movs r0, #24
	bl Func_02002be0
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #32
	bl Func_02002c08
	movs r1, #128
	movs r2, #50
	lsls r1, r1, #1
	movs r0, #4
	bl Func_02002c08
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #25
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r1, #176
	movs r0, #32
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #0
	movs r2, #2
	movs r0, #25
	bl Func_02002be0
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #32
	movs r2, #0
	movs r0, #4
	bl Object_LinkPair
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #192
	movs r0, #4
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #32
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #10
	adds r1, #255
	movs r2, #50
	movs r0, #25
	bl Func_02002c08
	movs r0, #24
	movs r1, #0
	movs r2, #2
	bl Func_02002be0
	movs r1, #4
	adds r1, #255
	movs r2, #20
	movs r0, #25
	bl Func_02002c08
	movs r1, #176
	movs r0, #32
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r2, #2
	movs r1, #0
	movs r0, #25
	bl Func_02002be0
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #24
	bl Motion_SetModeAndWaitAnimation
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #208
	movs r0, #32
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #24
	movs r1, #0
	movs r2, #2
	bl Func_02002be0
	movs r1, #152
	movs r2, #152
	lsls r1, r1, #7
	lsls r2, r2, #6
	movs r0, #24
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #152
	movs r2, #152
	lsls r1, r1, #7
	lsls r2, r2, #6
	movs r0, #25
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r2, #8
	movs r0, #24
	movs r1, #3
	negs r2, r2
	b .L_02008ed8
	.2byte 0x0000
.L_02008ed0:
	.4byte MsgYouThereHold
.L_02008ed4:
	.4byte gPartyState
.L_02008ed8:
	bl ObjectMotion_OffsetPositionAndReset
	movs r1, #3
	movs r2, #8
	negs r1, r1
	negs r2, r2
	movs r0, #25
	bl ObjectMotion_OffsetPositionAndReset
	movs r0, #24
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #25
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r1, #192
	movs r0, #25
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #24
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #32
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #32
	movs r1, #2
	bl Object_SetModeById
	mov r3, r8
	ldr r0, [r3]
	bl Object_GetById
	cmp r0, #0
	beq .L_02008f52
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #32
	bl ObjectMotion_ResetAndSetPosition
.L_02008f52:
	movs r0, #32
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r1, #0
	movs r2, #0
	movs r0, #32
	bl Func_02002b88
	movs r0, #145
	lsls r0, r0, #4
	bl GameFlag_SetBit
	movs r0, #26
	movs r1, #0
	movs r2, #0
	bl Func_02002b88
	bl Func_02002b30
	pop {r3}
	mov r8, r3
	pop {r5, r6, pc}
	.2byte 0x0000
	.section .text.x02008f80,"ax",%progbits
	.global Func_02000f80
	.thumb_func
Func_02000f80:
	push {lr}
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #17
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008f92
	b .L_020094f8
.L_02008f92:
	bl Func_02002b28
	movs r0, #0
	bl Func_02002c58
	movs r0, #248
	movs r1, #1
	movs r2, #210
	movs r3, #1
	lsls r0, r0, #15
	negs r1, r1
	lsls r2, r2, #17
	bl Motion_CamBounds
	bl Func_02002c28
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #33
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #34
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	bl Event_StartScreenTransition
	movs r1, #172
	movs r2, #166
	movs r0, #33
	lsls r1, r1, #16
	lsls r2, r2, #17
	bl Func_02002b88
	movs r1, #172
	movs r2, #162
	lsls r1, r1, #16
	lsls r2, r2, #17
	movs r0, #34
	bl Func_02002b88
	ldr r0, .L_020093b4
	bl Func_02002bd0
	bl Event_WaitTransitionFrames
	movs r0, #33
	movs r1, #0
	movs r2, #10
	bl Func_02002be0
	movs r2, #186
	movs r1, #156
	lsls r2, r2, #1
	movs r0, #33
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #186
	movs r0, #34
	movs r1, #156
	lsls r2, r2, #1
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r2, #194
	movs r1, #76
	lsls r2, r2, #1
	movs r0, #4
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r0, #33
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r2, #190
	movs r1, #108
	lsls r2, r2, #1
	movs r0, #33
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r0, #34
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r2, #190
	movs r1, #124
	lsls r2, r2, #1
	movs r0, #34
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r0, #4
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r1, #128
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #4
	bl ObjectMotion_ArmCallback
	movs r0, #33
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #34
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r1, #192
	movs r0, #33
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #34
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #192
	movs r0, #33
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r2, #0
	movs r0, #34
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r1, #129
	movs r0, #34
	lsls r1, r1, #1
	bl Func_02002c10
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #33
	bl Func_02002c10
	movs r0, #50
	bl Battle_WaitMode0
	movs r0, #34
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r1, #3
	movs r0, #33
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #170
	movs r1, #102
	lsls r2, r2, #1
	movs r0, #33
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #170
	movs r1, #102
	lsls r2, r2, #1
	movs r0, #34
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r0, #33
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #11
	lsls r2, r2, #11
	movs r0, #33
	bl Func_02002b88
	movs r0, #34
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r1, #128
	movs r2, #128
	movs r0, #34
	lsls r1, r1, #11
	lsls r2, r2, #11
	bl Func_02002b88
	movs r1, #224
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #4
	bl ObjectMotion_ArmCallback
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #4
	bl Func_02002c08
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #120
	bl Battle_WaitMode0
	movs r1, #204
	movs r2, #170
	movs r0, #33
	lsls r1, r1, #15
	lsls r2, r2, #17
	bl Func_02002b88
	movs r1, #204
	movs r2, #170
	movs r0, #34
	lsls r1, r1, #15
	lsls r2, r2, #17
	bl Func_02002b88
	movs r2, #182
	movs r0, #34
	movs r1, #100
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r2, #190
	movs r0, #34
	movs r1, #124
	lsls r2, r2, #1
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r2, #190
	movs r1, #100
	lsls r2, r2, #1
	movs r0, #33
	bl ObjectMotion_SetPositionAndReset
	movs r0, #34
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r2, #198
	movs r0, #34
	movs r1, #132
	lsls r2, r2, #1
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r2, #198
	movs r1, #108
	lsls r2, r2, #1
	movs r0, #33
	bl ObjectMotion_SetPositionAndReset
	movs r0, #34
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #4
	movs r1, #33
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r1, #192
	movs r0, #34
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #33
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #33
	bl Func_02002c08
	movs r0, #33
	movs r1, #0
	movs r2, #10
	bl Func_02002be0
	movs r2, #0
	movs r0, #34
	movs r1, #33
	bl ObjectMotion_SetAngleToward
	movs r1, #3
	movs r0, #34
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #34
	movs r1, #0
	movs r2, #10
	bl Func_02002be0
	movs r2, #0
	movs r1, #34
	movs r0, #33
	bl ObjectMotion_SetAngleToward
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #33
	bl Motion_SetVarCbAndRefresh
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #33
	movs r1, #0
	bl Func_02002be0
	movs r1, #3
	movs r0, #34
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #34
	movs r1, #0
	movs r2, #10
	bl Func_02002be0
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #33
	bl Func_02002c08
	movs r0, #33
	movs r1, #0
	movs r2, #10
	bl Func_02002be0
	movs r1, #192
	movs r0, #33
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #34
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #34
	bl Motion_SetVarCbAndRefresh
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #34
	movs r1, #0
	bl Func_02002be0
	movs r1, #3
	movs r0, #33
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #0
	movs r2, #10
	movs r0, #33
	bl Func_02002be0
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #33
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #2
	adds r1, #255
	movs r2, #30
	movs r0, #33
	bl Func_02002c08
	movs r0, #33
	movs r1, #0
	movs r2, #10
	bl Func_02002be0
	movs r2, #10
	movs r0, #33
	movs r1, #0
	bl Func_02002be0
	movs r0, #34
	movs r1, #2
	bl Motion_SetVarCbAndRefresh
	movs r1, #33
	movs r2, #0
	movs r0, #34
	bl ObjectMotion_SetAngleToward
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #34
	movs r1, #0
	movs r2, #10
	bl Func_02002be0
	movs r2, #0
	movs r1, #34
	movs r0, #33
	bl ObjectMotion_SetAngleToward
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #4
	movs r0, #33
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #33
	movs r1, #0
	movs r2, #10
	bl Func_02002be0
	movs r1, #4
	movs r2, #0
	movs r0, #33
	bl ObjectMotion_SetAngleToward
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #33
	movs r1, #0
	movs r2, #10
	bl Func_02002be0
	movs r1, #6
	movs r2, #30
	adds r1, #255
	movs r0, #4
	bl Func_02002c08
	movs r1, #2
	movs r0, #34
	bl Motion_SetVarCbAndRefresh
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #0
	movs r2, #10
	movs r0, #34
	bl Func_02002be0
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #131
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #33
	bl Func_02002c08
	movs r1, #0
	movs r0, #33
	bl UiText_OpenMessageAtObject
	movs r0, #4
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_020093b8
	movs r2, #10
	movs r0, #33
	movs r1, #0
	bl Func_02002be0
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_020093d4
	.2byte 0x0000
.L_020093b4:
	.4byte MsgHasEscaped2
.L_020093b8:
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #33
	adds r3, #1
	strh r3, [r2]
	movs r1, #0
	movs r2, #10
	bl Func_02002be0
.L_020093d4:
	movs r1, #34
	movs r2, #0
	movs r0, #33
	bl ObjectMotion_SetAngleToward
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #33
	movs r1, #0
	bl Func_02002be0
	movs r1, #3
	movs r0, #34
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #33
	bl Motion_SetVarCbAndRefresh
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #33
	movs r1, #0
	movs r2, #10
	bl Func_02002be0
	movs r1, #129
	movs r2, #30
	lsls r1, r1, #1
	movs r0, #34
	bl Func_02002c08
	movs r1, #3
	movs r0, #33
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #33
	movs r1, #0
	bl Func_02002be0
	movs r1, #2
	movs r0, #34
	bl Motion_SetVarCbAndRefresh
	movs r0, #60
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #34
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #34
	movs r1, #6
	movs r2, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r2, #158
	lsls r2, r2, #1
	movs r1, #164
	movs r0, #34
	bl ObjectMotion_SetPositionAndReset
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #128
	movs r0, #33
	lsls r1, r1, #8
	bl Func_02002bf8
	movs r1, #3
	movs r0, #33
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #33
	movs r1, #0
	movs r2, #10
	bl Func_02002be0
	movs r2, #0
	movs r0, #34
	movs r1, #0
	bl Func_02002b88
	movs r1, #3
	movs r0, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #33
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #19
	bl GameFlag_SetBit
	movs r0, #33
	movs r1, #30
	movs r2, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r2, #158
	movs r1, #164
	lsls r2, r2, #1
	movs r0, #33
	bl ObjectMotion_SetPositionAndReset
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #33
	movs r1, #0
	movs r2, #0
	bl Func_02002b88
	movs r0, #20
	bl Battle_WaitMode0
	bl Func_02002b30
.L_020094f8:
	pop {pc}
	.2byte 0x0000
	.section .text.x020094fc,"ax",%progbits
	.global Func_020014fc
	.thumb_func
Func_020014fc:
	push {lr}
	bl Func_02002b28
	movs r0, #0
	bl Func_02002c58
	ldr r0, .L_020097bc
	bl Func_02002bd0
	movs r0, #24
	movs r1, #0
	movs r2, #2
	bl Func_02002be0
	movs r0, #238
	movs r1, #1
	movs r2, #200
	movs r3, #1
	lsls r0, r0, #17
	negs r1, r1
	lsls r2, r2, #17
	bl Motion_CamBounds
	movs r1, #3
	movs r0, #24
	negs r1, r1
	movs r2, #8
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #236
	movs r2, #213
	lsls r1, r1, #1
	lsls r2, r2, #1
	movs r0, #4
	bl ObjectMotion_SetPositionAndReset
	movs r0, #10
	bl Battle_WaitMode0
	ldr r3, .L_020097c0
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	movs r1, #192
	ldr r0, [r3]
	lsls r1, r1, #8
	bl Func_02002bf8
	movs r3, #192
	lsls r3, r3, #8
	movs r1, #16
	movs r2, #1
	movs r0, #32
	bl Func_02002c68
	bl Func_02002c28
	movs r0, #32
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r2, #8
	movs r0, #25
	movs r1, #3
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #25
	movs r1, #0
	bl Func_02002bf8
	movs r1, #4
	movs r0, #25
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #25
	movs r1, #0
	movs r2, #10
	bl Func_02002be0
	movs r0, #4
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #32
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #2
	movs r2, #30
	adds r1, #255
	movs r0, #32
	bl Func_02002c08
	movs r0, #32
	movs r1, #0
	bl Func_02002be8
	movs r0, #4
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r1, #160
	movs r0, #24
	lsls r1, r1, #7
	bl Func_02002bf8
	movs r0, #24
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #24
	movs r1, #0
	movs r2, #10
	bl Func_02002be0
	movs r1, #208
	movs r0, #32
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #4
	adds r1, #255
	movs r2, #0
	movs r0, #32
	bl Func_02002c08
	movs r0, #32
	movs r1, #6
	movs r2, #15
	bl ObjectMotion_Launch
	movs r2, #23
	movs r0, #32
	movs r1, #6
	bl ObjectMotion_Launch
	movs r0, #32
	movs r1, #0
	bl Func_02002be8
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #4
	bl Func_02002c08
	movs r1, #24
	movs r2, #0
	movs r0, #4
	bl ObjectMotion_SetAngleToward
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #128
	movs r2, #30
	lsls r1, r1, #1
	movs r0, #32
	bl Func_02002c08
	movs r1, #208
	movs r0, #32
	lsls r1, r1, #8
	bl Func_02002bf8
	movs r0, #32
	movs r1, #0
	bl Func_02002be8
	movs r1, #128
	movs r0, #25
	lsls r1, r1, #6
	bl Func_02002bf8
	movs r0, #25
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r1, #176
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #32
	bl ObjectMotion_ArmCallback
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #0
	movs r0, #25
	bl UiText_OpenMessageAtObject
	movs r1, #176
	movs r0, #4
	lsls r1, r1, #8
	bl Func_02002bf8
	movs r0, #4
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_020096d2
	movs r0, #25
	bl Battle_WaitMode0
	movs r1, #176
	movs r0, #32
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r2, #10
	movs r0, #25
	movs r1, #0
	bl Func_02002be0
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_02009700
.L_020096d2:
	movs r0, #25
	bl Battle_WaitMode0
	movs r1, #208
	movs r0, #32
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #25
	adds r3, #1
	strh r3, [r2]
	movs r1, #0
	movs r2, #10
	bl Func_02002be0
.L_02009700:
	movs r1, #2
	movs r0, #32
	bl Motion_SetVarCbAndRefresh
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #176
	movs r0, #32
	lsls r1, r1, #8
	bl Func_02002bf8
	movs r0, #32
	movs r1, #0
	bl Func_02002be8
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #25
	bl Func_02002c08
	movs r2, #10
	movs r0, #25
	movs r1, #0
	bl Func_02002be0
	movs r1, #2
	movs r0, #24
	bl Motion_SetVarCbAndRefresh
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #24
	movs r1, #0
	movs r2, #10
	bl Func_02002be0
	movs r0, #137
	lsls r0, r0, #4
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020097f0
	ldr r0, .L_020097c4
	bl Func_02002bd0
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #32
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #0
	movs r0, #32
	bl UiText_OpenMessageAtObject
	movs r0, #4
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #4
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_020097c8
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #32
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #32
	movs r1, #0
	bl Func_02002be8
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_020097f0
.L_020097bc:
	.4byte MsgHeyYouLot
.L_020097c0:
	.4byte gPartyState
.L_020097c4:
	.4byte MsgItWasThem
.L_020097c8:
	movs r0, #20
	bl Battle_WaitMode0
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #32
	adds r3, #1
	movs r1, #4
	strh r3, [r2]
	bl Motion_SetModeAndWaitAnimation
	movs r0, #32
	movs r1, #0
	bl Func_02002be8
.L_020097f0:
	ldr r0, .L_020099d0
	bl Func_02002bd0
	movs r1, #208
	movs r0, #32
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #208
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #4
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #0
	movs r0, #32
	bl Func_02002be8
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #2
	adds r1, #255
	movs r2, #30
	movs r0, #24
	bl Func_02002c08
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #24
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #176
	movs r0, #32
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #176
	movs r0, #4
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #129
	movs r2, #30
	lsls r1, r1, #1
	movs r0, #25
	bl Func_02002c08
	movs r1, #4
	movs r0, #25
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #128
	movs r0, #25
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #25
	movs r1, #0
	movs r2, #10
	bl Func_02002be0
	movs r1, #2
	adds r1, #255
	movs r2, #0
	movs r0, #32
	bl Func_02002c08
	movs r1, #2
	movs r2, #30
	adds r1, #255
	movs r0, #4
	bl Func_02002c08
	movs r0, #32
	movs r1, #0
	bl Func_02002be8
	movs r1, #160
	movs r0, #24
	lsls r1, r1, #7
	bl Func_02002bf8
	movs r1, #4
	movs r0, #24
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #24
	movs r1, #0
	bl Func_02002be0
	movs r1, #3
	movs r0, #25
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #25
	movs r1, #0
	movs r2, #10
	bl Func_02002be0
	movs r1, #152
	movs r2, #152
	lsls r1, r1, #7
	lsls r2, r2, #6
	movs r0, #24
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #152
	movs r2, #152
	lsls r1, r1, #7
	lsls r2, r2, #6
	movs r0, #25
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r2, #8
	movs r0, #24
	movs r1, #3
	negs r2, r2
	bl ObjectMotion_OffsetPositionAndReset
	movs r1, #3
	movs r2, #8
	negs r1, r1
	negs r2, r2
	movs r0, #25
	bl ObjectMotion_OffsetPositionAndReset
	movs r0, #24
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #25
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r1, #192
	movs r0, #25
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #24
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #4
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #32
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #32
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #20
	bl GameFlag_SetBit
	movs r0, #32
	movs r1, #2
	bl Object_SetModeById
	ldr r3, .L_020099d4
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	cmp r0, #0
	beq .L_020099ae
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #32
	bl ObjectMotion_ResetAndSetPosition
.L_020099ae:
	movs r0, #32
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r2, #0
	movs r0, #32
	movs r1, #0
	bl Func_02002b88
	movs r1, #192
	movs r0, #4
	lsls r1, r1, #8
	bl Func_02002bf8
	bl Func_02002b30
	pop {pc}
	.2byte 0x0000
.L_020099d0:
	.4byte MsgWasAnythingStolen
.L_020099d4:
	.4byte gPartyState
	.section .text.x020099d8,"ax",%progbits
	.global Func_020019d8
	.thumb_func
Func_020019d8:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	bl Func_02002b28
	movs r0, #0
	bl Func_02002c58
	bl Event_StartScreenTransition
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #37
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02009a08
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #38
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009a12
.L_02009a08:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #118
	bl GameFlag_SetBit
.L_02009a12:
	ldr r0, .L_02009ce8
	bl Func_02002bd0
	movs r0, #34
	movs r1, #0
	movs r2, #0
	bl Func_02002be0
	movs r2, #140
	movs r0, #4
	movs r1, #150
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r0, #4
	movs r1, #16
	movs r2, #0
	bl ObjectMotion_OffsetPositionAndReset
	movs r3, #192
	movs r0, #5
	movs r1, #8
	movs r2, #16
	lsls r3, r3, #8
	bl Func_02002c68
	movs r3, #192
	movs r0, #32
	movs r1, #0
	movs r2, #0
	lsls r3, r3, #8
	bl Func_02002c68
	movs r1, #8
	movs r3, #192
	movs r0, #6
	negs r1, r1
	movs r2, #16
	lsls r3, r3, #8
	bl Func_02002c68
	movs r1, #16
	movs r3, #192
	lsls r3, r3, #8
	negs r1, r1
	movs r2, #0
	movs r0, #7
	bl Func_02002c68
	movs r0, #4
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #4
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	ldr r3, .L_02009cec
	movs r5, #128
	ldrh r1, [r3, #52]
	ldrh r2, [r3, #54]
	ldr r0, [r3, #48]
	bl Func_02002ae8
	lsls r5, r5, #7
	movs r1, #153
	adds r3, r5, #0
	movs r0, #34
	lsls r1, r1, #16
	ldr r2, .L_02009cf0
	bl Func_02002b90
	movs r0, #34
	movs r1, #0
	movs r2, #4
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #34
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #34
	movs r1, #0
	bl UiText_OpenMessageAtObject
	movs r0, #32
	movs r1, #4
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #5
	movs r1, #4
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #7
	movs r1, #4
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #6
	movs r1, #4
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	adds r1, r5, #0
	movs r0, #34
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #4
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_02009b7e
	movs r1, #192
	movs r0, #32
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #7
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	adds r1, r5, #0
	movs r2, #0
	movs r0, #34
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #34
	movs r1, #4
	bl Object_SetModeById
	movs r0, #33
	bl Battle_WaitMode0
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #0
	movs r0, #34
	bl Func_02002be8
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r1, #226
	lsls r1, r1, #1
	adds r2, r2, r1
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_02009bee
.L_02009b7e:
	movs r1, #192
	movs r0, #32
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #7
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	adds r1, r5, #0
	movs r2, #0
	movs r0, #34
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #34
	movs r1, #4
	bl Object_SetModeById
	movs r0, #33
	bl Battle_WaitMode0
	movs r0, #20
	bl Battle_WaitMode0
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #34
	adds r3, #1
	strh r3, [r2]
	movs r1, #0
	movs r2, #0
	bl Func_02002be0
.L_02009bee:
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #34
	bl Func_02002c08
	movs r0, #34
	movs r1, #0
	movs r2, #0
	bl Func_02002be0
	movs r0, #34
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #34
	movs r1, #0
	movs r2, #0
	bl Func_02002be0
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #6
	bl Func_02002c08
	movs r2, #0
	movs r1, #5
	movs r0, #6
	bl ObjectMotion_SetAngleToward
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #6
	movs r1, #0
	bl Func_02002be8
	movs r1, #6
	movs r2, #0
	movs r0, #5
	bl ObjectMotion_SetAngleToward
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #5
	movs r1, #4
	bl Object_SetModeById
	movs r0, #33
	bl Battle_WaitMode0
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #5
	movs r1, #0
	bl Func_02002be8
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #34
	bl Func_02002c08
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #34
	movs r1, #0
	movs r2, #0
	bl Func_02002be0
	movs r0, #34
	movs r1, #16
	movs r2, #8
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #128
	movs r0, #34
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #210
	bl Func_02002b18
	cmp r0, #0
	blt .L_02009cf4
	movs r0, #210
	movs r1, #3
	bl Func_02002c50
	movs r1, #0
	movs r0, #210
	bl PartyInventory_GiveItem
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r1, #226
	lsls r1, r1, #1
	adds r2, r2, r1
	ldrh r3, [r2]
	movs r1, #192
	adds r3, #1
	strh r3, [r2]
	movs r0, #4
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #24
	bl GameFlag_SetBit
	b .L_02009d88
.L_02009ce8:
	.4byte MsgWait3
.L_02009cec:
	.4byte Data_02003308
.L_02009cf0:
	.4byte 0x01030000
.L_02009cf4:
	movs r6, #192
	lsls r6, r6, #18
	ldr r3, [r6, #108]
	movs r5, #226
	lsls r5, r5, #1
	ldrsh r2, [r3, r5]
	ldr r0, .L_0200a0a8
	mov r8, r2
	movs r2, #1
	add r8, r2
	bl Func_02002bd0
	movs r0, #210
	movs r1, #2
	bl Func_02002b10
	ldr r3, [r6, #108]
	movs r1, #5
	adds r3, r3, r5
	ldrh r0, [r3]
	adds r2, r0, #1
	lsls r0, r0, #16
	strh r2, [r3]
	asrs r0, r0, #16
	bl UiText_ShowPositionedMessageAndWait
	movs r0, #34
	movs r1, #2
	bl Motion_SetVarCbAndRefresh
	movs r0, #34
	movs r1, #0
	bl Func_02002be8
	movs r2, #0
	movs r0, #34
	movs r1, #16
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #210
	movs r1, #2
	bl Func_02002b10
	ldr r3, [r6, #108]
	movs r1, #5
	adds r3, r3, r5
	ldrh r0, [r3]
	adds r2, r0, #1
	lsls r0, r0, #16
	strh r2, [r3]
	asrs r0, r0, #16
	bl UiText_ShowPositionedMessageAndWait
	movs r1, #16
	movs r2, #0
	movs r0, #34
	negs r1, r1
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #128
	movs r0, #34
	lsls r1, r1, #7
	bl Func_02002bf8
	movs r1, #192
	movs r2, #128
	lsls r1, r1, #16
	movs r0, #66
	lsls r2, r2, #17
	bl Func_02002b88
	ldr r3, [r6, #108]
	mov r1, r8
	strh r1, [r3, r5]
.L_02009d88:
	movs r1, #16
	movs r2, #8
	negs r2, r2
	movs r0, #34
	negs r1, r1
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #128
	movs r0, #34
	lsls r1, r1, #7
	bl Func_02002bf8
	movs r1, #6
	adds r1, #255
	movs r2, #30
	movs r0, #34
	bl Func_02002c08
	movs r0, #34
	movs r1, #0
	movs r2, #0
	bl Func_02002be0
	movs r0, #34
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #192
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r2, #0
	movs r0, #34
	movs r1, #0
	bl Func_02002be0
	movs r0, #32
	movs r1, #3
	bl Object_SetModeById
	movs r0, #4
	movs r1, #3
	bl Object_SetModeById
	movs r0, #5
	movs r1, #3
	bl Object_SetModeById
	movs r0, #6
	movs r1, #3
	bl Object_SetModeById
	movs r0, #7
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #192
	movs r0, #34
	lsls r1, r1, #8
	bl Func_02002bf8
	movs r2, #4
	movs r0, #34
	movs r1, #153
	adds r2, #255
	bl ObjectMotion_SetPositionAndReset
	movs r0, #34
	movs r1, #0
	movs r2, #0
	bl Func_02002b88
	movs r0, #32
	movs r1, #4
	bl Object_SetModeById
	movs r0, #33
	bl Battle_WaitMode0
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #32
	movs r1, #0
	bl Func_02002be8
	movs r1, #0
	movs r0, #33
	bl Func_02002be8
	movs r0, #78
	bl Func_02002c90
	movs r1, #2
	adds r1, #255
	movs r2, #0
	movs r0, #4
	bl Func_02002c08
	movs r1, #2
	adds r1, #255
	movs r2, #0
	movs r0, #5
	bl Func_02002c08
	movs r1, #2
	adds r1, #255
	movs r2, #0
	movs r0, #7
	bl Func_02002c08
	movs r1, #2
	adds r1, #255
	movs r2, #0
	movs r0, #6
	bl Func_02002c08
	movs r1, #2
	adds r1, #255
	movs r2, #30
	movs r0, #32
	bl Func_02002c08
	movs r0, #4
	movs r1, #33
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #7
	movs r1, #33
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #6
	movs r1, #33
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #5
	movs r1, #33
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r2, #0
	movs r1, #33
	movs r0, #32
	bl ObjectMotion_SetAngleToward
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #1
	movs r0, #33
	bl Object_AttachWorkTargetToObject
	movs r0, #60
	bl Battle_WaitMode0
	movs r0, #36
	bl Func_02002c90
	movs r1, #8
	movs r2, #30
	adds r1, #255
	movs r0, #33
	bl Func_02002c08
	movs r0, #33
	movs r1, #0
	bl Func_02002be8
	movs r1, #128
	movs r0, #33
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #6
	movs r1, #5
	movs r2, #0
	bl Object_LinkPair
	movs r1, #32
	movs r2, #0
	movs r0, #7
	bl Object_LinkPair
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #129
	movs r2, #30
	lsls r1, r1, #1
	movs r0, #4
	bl Func_02002c08
	movs r1, #2
	movs r0, #33
	bl Motion_SetVarCbAndRefresh
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #33
	movs r1, #0
	bl Func_02002be8
	movs r0, #4
	movs r1, #33
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #7
	movs r1, #33
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #6
	movs r1, #33
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #5
	movs r1, #33
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r2, #0
	movs r1, #33
	movs r0, #32
	bl ObjectMotion_SetAngleToward
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #128
	movs r0, #33
	lsls r1, r1, #8
	bl Func_02002bf8
	movs r1, #0
	movs r0, #33
	bl Func_02002bf8
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #33
	movs r1, #0
	bl Func_02002be8
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #6
	bl Func_02002c08
	movs r2, #0
	movs r1, #5
	movs r0, #6
	bl ObjectMotion_SetAngleToward
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #6
	movs r1, #0
	bl Func_02002be8
	movs r1, #128
	movs r2, #30
	lsls r1, r1, #1
	movs r0, #5
	bl Func_02002c08
	movs r0, #5
	movs r1, #0
	bl Func_02002be8
	movs r1, #8
	movs r2, #30
	adds r1, #255
	movs r0, #33
	bl Func_02002c08
	movs r0, #33
	movs r1, #0
	bl Func_02002be8
	movs r1, #4
	movs r2, #0
	movs r0, #33
	bl ObjectMotion_SetAngleToward
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #0
	movs r0, #33
	bl UiText_OpenMessageAtObject
	movs r0, #4
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	beq .L_0200a0ac
	movs r0, #33
	movs r1, #0
	bl Func_02002be8
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #4
	bl Func_02002c08
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #33
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #33
	movs r1, #0
	bl Func_02002be8
	movs r1, #5
	movs r2, #0
	movs r0, #6
	bl Object_LinkPair
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #6
	movs r1, #33
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r2, #0
	movs r1, #33
	movs r0, #5
	bl ObjectMotion_SetAngleToward
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #5
	movs r1, #4
	bl Object_SetModeById
	movs r0, #6
	movs r1, #4
	bl Object_SetModeById
	movs r0, #33
	bl Battle_WaitMode0
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #10
	adds r1, #255
	movs r0, #33
	movs r2, #30
	bl Func_02002c08
	movs r0, #33
	movs r1, #0
	bl Func_02002be8
	b .L_0200a114
.L_0200a0a8:
	.4byte MsgTheMayorTried
.L_0200a0ac:
	ldr r0, .L_0200a438
	bl Func_02002bd0
	movs r0, #33
	movs r1, #0
	bl Func_02002be8
	movs r1, #5
	movs r2, #0
	movs r0, #6
	bl Object_LinkPair
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #6
	movs r1, #33
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r2, #0
	movs r1, #33
	movs r0, #5
	bl ObjectMotion_SetAngleToward
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #5
	movs r1, #4
	bl Object_SetModeById
	movs r0, #6
	movs r1, #4
	bl Object_SetModeById
	movs r0, #33
	bl Battle_WaitMode0
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #10
	adds r1, #255
	movs r0, #6
	movs r2, #30
	bl Func_02002c08
	movs r0, #33
	movs r1, #0
	bl Func_02002be8
.L_0200a114:
	ldr r0, .L_0200a43c
	bl Func_02002bd0
	movs r1, #2
	movs r0, #33
	bl Motion_SetVarCbAndRefresh
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #33
	movs r1, #0
	bl Func_02002be8
	movs r0, #32
	movs r1, #4
	bl Object_SetModeById
	movs r0, #4
	movs r1, #4
	bl Object_SetModeById
	movs r0, #7
	movs r1, #4
	bl Object_SetModeById
	movs r0, #6
	movs r1, #4
	bl Object_SetModeById
	movs r0, #5
	movs r1, #4
	bl Object_SetModeById
	movs r0, #33
	bl Battle_WaitMode0
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #33
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #33
	movs r1, #0
	bl Func_02002be8
	movs r1, #10
	adds r1, #255
	movs r2, #0
	movs r0, #4
	bl Func_02002c08
	movs r1, #10
	adds r1, #255
	movs r2, #0
	movs r0, #5
	bl Func_02002c08
	movs r1, #10
	adds r1, #255
	movs r2, #0
	movs r0, #6
	bl Func_02002c08
	movs r1, #10
	adds r1, #255
	movs r2, #30
	movs r0, #32
	bl Func_02002c08
	movs r0, #60
	bl Battle_WaitMode0
	movs r1, #131
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #33
	bl Func_02002c08
	movs r0, #33
	movs r1, #0
	movs r2, #0
	bl Func_02002be0
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #4
	bl Func_02002c08
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #5
	bl Func_02002c08
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #6
	bl Func_02002c08
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #32
	bl Func_02002c08
	movs r0, #33
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #33
	movs r1, #0
	bl Func_02002be8
	movs r2, #0
	movs r1, #0
	movs r0, #33
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #33
	movs r1, #0
	bl Func_02002be8
	movs r1, #128
	movs r2, #30
	lsls r1, r1, #1
	movs r0, #32
	bl Func_02002c08
	movs r0, #32
	movs r1, #0
	bl Func_02002be8
	movs r1, #2
	movs r0, #33
	bl Motion_SetVarCbAndRefresh
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #33
	movs r1, #0
	bl Func_02002be8
	movs r0, #33
	movs r1, #4
	bl Object_SetModeById
	movs r0, #33
	bl Battle_WaitMode0
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #33
	movs r1, #0
	bl Func_02002be8
	movs r1, #4
	adds r1, #255
	movs r2, #0
	movs r0, #4
	bl Func_02002c08
	movs r1, #4
	adds r1, #255
	movs r2, #0
	movs r0, #6
	bl Func_02002c08
	movs r1, #4
	adds r1, #255
	movs r2, #0
	movs r0, #5
	bl Func_02002c08
	movs r1, #4
	adds r1, #255
	movs r2, #30
	movs r0, #7
	bl Func_02002c08
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #33
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #33
	movs r1, #0
	bl Func_02002be8
	movs r1, #132
	movs r2, #30
	lsls r1, r1, #1
	movs r0, #33
	bl Func_02002c08
	movs r0, #33
	movs r1, #0
	bl Func_02002be8
	movs r1, #2
	movs r0, #4
	bl Motion_SetVarCbAndRefresh
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #33
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #33
	movs r1, #0
	bl Func_02002be8
	movs r0, #33
	movs r1, #4
	bl Object_SetModeById
	movs r0, #33
	bl Battle_WaitMode0
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #33
	movs r1, #0
	bl Func_02002be8
	movs r1, #2
	movs r0, #33
	bl Motion_SetVarCbAndRefresh
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #33
	movs r1, #0
	bl Func_02002be8
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #4
	bl Func_02002c08
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #7
	bl Func_02002c08
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #5
	bl Func_02002c08
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #6
	bl Func_02002c08
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #4
	bl Func_02002c08
	movs r1, #2
	movs r2, #30
	adds r1, #255
	movs r0, #33
	bl Func_02002c08
	movs r0, #33
	movs r1, #0
	bl Func_02002be8
	movs r1, #0
	movs r0, #33
	bl Func_02002bf8
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #8
	movs r0, #33
	bl Func_02002bf8
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #0
	movs r0, #33
	bl Func_02002bf8
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #33
	movs r1, #0
	bl Func_02002be8
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #33
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #33
	movs r1, #0
	bl Func_02002be8
	movs r1, #2
	movs r0, #33
	bl Motion_SetVarCbAndRefresh
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #33
	movs r1, #0
	bl Func_02002be8
	movs r0, #33
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #0
	movs r0, #33
	bl UiText_OpenMessageAtObject
	movs r0, #4
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_0200a440
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #33
	bl Func_02002c08
	movs r2, #0
	movs r0, #33
	movs r1, #0
	bl Func_02002be0
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_0200a468
	.2byte 0x0000
.L_0200a438:
	.4byte MsgICanTBelieveIt
.L_0200a43c:
	.4byte MsgWhoIsThis
.L_0200a440:
	movs r1, #4
	adds r1, #255
	movs r2, #30
	movs r0, #33
	bl Func_02002c08
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r1, #226
	lsls r1, r1, #1
	adds r2, r2, r1
	ldrh r3, [r2]
	movs r0, #33
	adds r3, #1
	strh r3, [r2]
	movs r1, #0
	movs r2, #0
	bl Func_02002be0
.L_0200a468:
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #32
	bl Func_02002c08
	movs r0, #32
	movs r1, #4
	bl Object_SetModeById
	movs r0, #33
	bl Battle_WaitMode0
	movs r0, #32
	movs r1, #4
	bl Object_SetModeById
	movs r0, #33
	bl Battle_WaitMode0
	movs r0, #32
	movs r1, #0
	bl Func_02002be8
	movs r0, #33
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #33
	movs r1, #0
	bl Func_02002be8
	movs r1, #2
	movs r0, #33
	bl Motion_SetVarCbAndRefresh
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #33
	movs r1, #0
	bl Func_02002be8
	movs r2, #0
	movs r1, #0
	movs r0, #33
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #0
	movs r0, #33
	bl Func_02002be8
	movs r0, #78
	bl Func_02002c90
	movs r0, #1
	movs r1, #1
	movs r2, #1
	movs r3, #0
	negs r0, r0
	negs r1, r1
	negs r2, r2
	bl Motion_CamBounds
	movs r0, #33
	movs r1, #24
	movs r2, #24
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #33
	movs r1, #132
	movs r2, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r2, #0
	movs r0, #33
	movs r1, #0
	bl Func_02002b88
	bl Func_02002c60
	movs r0, #4
	movs r1, #1
	bl Object_AttachWorkTargetToObject
	movs r1, #129
	movs r2, #30
	lsls r1, r1, #1
	movs r0, #6
	bl Func_02002c08
	movs r0, #6
	movs r1, #0
	bl Func_02002be8
	movs r0, #5
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #5
	movs r1, #0
	bl Func_02002be8
	movs r1, #2
	adds r1, #255
	movs r2, #30
	movs r0, #7
	bl Func_02002c08
	movs r1, #4
	movs r2, #0
	movs r0, #7
	bl ObjectMotion_SetAngleToward
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #7
	movs r1, #0
	movs r2, #0
	bl Func_02002be0
	movs r0, #32
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #32
	movs r1, #0
	bl Func_02002be8
	movs r1, #2
	movs r0, #6
	bl Motion_SetVarCbAndRefresh
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #6
	movs r1, #0
	bl Func_02002be8
	movs r2, #0
	movs r1, #6
	movs r0, #5
	bl ObjectMotion_SetAngleToward
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #5
	movs r1, #0
	bl Func_02002be8
	movs r0, #32
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #32
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #32
	movs r1, #0
	bl Func_02002be8
	movs r1, #2
	adds r1, #255
	movs r2, #20
	movs r0, #7
	bl Func_02002c08
	movs r0, #7
	movs r1, #32
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #4
	movs r1, #32
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #6
	movs r1, #32
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r1, #32
	movs r2, #0
	movs r0, #5
	bl ObjectMotion_SetAngleToward
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #7
	movs r1, #0
	movs r2, #0
	bl Func_02002be0
	movs r2, #0
	movs r1, #7
	movs r0, #32
	bl ObjectMotion_SetAngleToward
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #32
	movs r1, #0
	bl Func_02002be8
	movs r2, #0
	movs r1, #5
	movs r0, #6
	bl ObjectMotion_SetAngleToward
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #0
	movs r0, #6
	bl Func_02002be8
	movs r0, #5
	bl Object_GetById
	movs r1, #10
	bl Object_SetPartAttribute
	movs r0, #5
	movs r1, #0
	bl Func_02002be8
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #5
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #5
	movs r1, #0
	bl Func_02002be8
	movs r2, #0
	movs r1, #7
	movs r0, #4
	bl Object_LinkPair
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #32
	bl Motion_SetVarCbAndRefresh
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #128
	movs r0, #32
	lsls r1, r1, #7
	bl Func_02002bf8
	movs r1, #0
	movs r0, #32
	bl Func_02002be8
	movs r0, #5
	bl Object_GetById
	movs r1, #0
	bl Object_SetPartAttribute
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #5
	bl Func_02002c08
	movs r2, #0
	movs r1, #32
	movs r0, #5
	bl ObjectMotion_SetAngleToward
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #5
	movs r1, #0
	bl Func_02002be8
	movs r1, #132
	movs r2, #30
	lsls r1, r1, #1
	movs r0, #6
	bl Func_02002c08
	movs r1, #0
	movs r0, #6
	bl Func_02002be8
	movs r0, #5
	bl Object_GetById
	movs r1, #10
	bl Object_SetPartAttribute
	movs r2, #0
	movs r1, #6
	movs r0, #5
	bl ObjectMotion_SetAngleToward
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #5
	movs r1, #0
	bl Func_02002be8
	movs r1, #2
	movs r0, #32
	bl Motion_SetVarCbAndRefresh
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #0
	movs r0, #32
	bl Func_02002be8
	movs r0, #5
	bl Object_GetById
	movs r1, #0
	bl Object_SetPartAttribute
	movs r0, #4
	movs r1, #32
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #5
	movs r1, #32
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #6
	movs r1, #32
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r1, #32
	movs r2, #0
	movs r0, #7
	bl ObjectMotion_SetAngleToward
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #32
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #32
	movs r1, #0
	bl Func_02002be8
	movs r0, #4
	movs r1, #3
	bl Object_SetModeById
	movs r0, #5
	movs r1, #3
	bl Object_SetModeById
	movs r0, #6
	movs r1, #3
	bl Object_SetModeById
	movs r0, #7
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #32
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #32
	movs r1, #0
	bl Func_02002be8
	movs r0, #5
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #132
	movs r2, #30
	lsls r1, r1, #1
	movs r0, #32
	bl Func_02002c08
	movs r0, #32
	movs r1, #0
	bl Func_02002be8
	movs r0, #6
	movs r1, #3
	bl Object_SetModeById
	movs r0, #7
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #32
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #32
	movs r1, #0
	bl Func_02002bf8
	movs r0, #32
	movs r1, #0
	bl Func_02002be8
	movs r0, #4
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #6
	movs r1, #3
	bl Object_SetModeById
	movs r0, #5
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #4
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #32
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #32
	movs r2, #153
	lsls r2, r2, #8
	ldr r1, .L_0200a9a8
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r0, #32
	movs r1, #2
	bl Object_SetModeById
	movs r0, #4
	bl Object_GetById
	cmp r0, #0
	beq .L_0200a8c2
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #32
	bl ObjectMotion_ResetAndSetPosition
.L_0200a8c2:
	movs r0, #32
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #32
	movs r1, #0
	movs r2, #0
	bl Func_02002b88
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #5
	ldr r1, .L_0200a9a8
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r0, #5
	movs r1, #2
	bl Object_SetModeById
	movs r0, #4
	bl Object_GetById
	cmp r0, #0
	beq .L_0200a900
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #5
	bl ObjectMotion_ResetAndSetPosition
.L_0200a900:
	movs r0, #5
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl Func_02002b88
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #6
	ldr r1, .L_0200a9a8
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r0, #6
	movs r1, #2
	bl Object_SetModeById
	movs r0, #4
	bl Object_GetById
	cmp r0, #0
	beq .L_0200a93e
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #6
	bl ObjectMotion_ResetAndSetPosition
.L_0200a93e:
	movs r0, #6
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #6
	movs r1, #0
	movs r2, #0
	bl Func_02002b88
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #7
	ldr r1, .L_0200a9a8
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r0, #7
	movs r1, #2
	bl Object_SetModeById
	movs r0, #4
	bl Object_GetById
	cmp r0, #0
	beq .L_0200a97c
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #7
	bl ObjectMotion_ResetAndSetPosition
.L_0200a97c:
	movs r0, #7
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r1, #0
	movs r2, #0
	movs r0, #7
	bl Func_02002b88
	movs r0, #152
	lsls r0, r0, #4
	bl GameFlag_SetBit
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #142
	bl GameFlag_SetBit
	bl Func_02002b30
	pop {r3}
	mov r8, r3
	pop {r5, r6, pc}
.L_0200a9a8:
	.4byte 0x00013333
