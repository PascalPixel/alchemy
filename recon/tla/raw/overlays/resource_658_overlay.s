.syntax unified
	.thumb
	.section .text.x0200806c,"ax",%progbits
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
	.section .text.x020082bc,"ax",%progbits
	.global Func_020002bc
	.thumb_func
Func_020002bc:
	push {lr}
	ldr r0, .L_0200831c
	bl Func_02002bd0
	movs r1, #0
	movs r0, #33
	bl UiText_OpenMessageAtObject
	movs r0, #4
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_02008316
	movs r1, #4
	adds r1, #255
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
	bne .L_02008310
	movs r1, #4
	adds r1, #255
	movs r2, #30
	movs r0, #33
	bl Func_02002c08
	movs r0, #33
	movs r1, #0
	movs r2, #10
	bl Func_02002be0
	b .L_0200831a
.L_02008310:
	bl Func_02000294
	b .L_0200831a
.L_02008316:
	bl Func_02000294
.L_0200831a:
	pop {pc}
.L_0200831c:
	.4byte MsgFieldHaveYouByChanceSeen
	.section .text.x0200851c,"ax",%progbits
	.global Func_0200051c
	.thumb_func
Func_0200051c:
	push {lr}
	movs r0, #145
	lsls r0, r0, #4
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008538
	movs r0, #123
	bl Func_02002c90
	movs r0, #16
	bl Func_02002c30
	b .L_020085b0
.L_02008538:
	movs r0, #160
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020085b0
	bl Func_02002b28
	movs r0, #0
	bl Func_02002c58
	movs r3, #128
	movs r1, #186
	lsls r3, r3, #7
	lsls r1, r1, #18
	ldr r2, .L_020085b4
	movs r0, #26
	bl Func_02002b90
	movs r0, #4
	bl Object_GetById
	movs r3, #160
	lsls r3, r3, #11
	movs r1, #128
	movs r2, #128
	str r3, [r0, #40]
	lsls r1, r1, #10
	movs r0, #4
	lsls r2, r2, #9
	bl ObjectMotion_SetSpeedParameters
	movs r1, #180
	movs r2, #244
	movs r0, #4
	lsls r1, r1, #2
	bl ObjectMotion_SetPositionAndReset
	movs r1, #128
	movs r0, #4
	lsls r1, r1, #6
	bl Func_02002bf8
	movs r3, #128
	movs r1, #180
	movs r2, #244
	lsls r3, r3, #6
	lsls r1, r1, #18
	lsls r2, r2, #16
	movs r0, #4
	bl Func_02002b90
	movs r0, #160
	lsls r0, r0, #2
	bl GameFlag_SetBit
	bl Func_02000404
	bl Func_02002b30
.L_020085b0:
	pop {pc}
	.2byte 0x0000
.L_020085b4:
	.4byte 0x01010000
	.section .text.x020085b8,"ax",%progbits
	.global Func_020005b8
	.thumb_func
Func_020005b8:
	push {lr}
	movs r0, #160
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008634
	bl Func_02002b28
	movs r0, #0
	bl Func_02002c58
	movs r3, #128
	movs r1, #186
	movs r2, #128
	lsls r3, r3, #7
	lsls r1, r1, #18
	lsls r2, r2, #17
	movs r0, #26
	bl Func_02002b90
	movs r0, #4
	bl Object_GetById
	movs r3, #160
	lsls r3, r3, #11
	movs r1, #128
	movs r2, #128
	str r3, [r0, #40]
	lsls r1, r1, #10
	movs r0, #4
	lsls r2, r2, #9
	bl ObjectMotion_SetSpeedParameters
	movs r1, #180
	movs r2, #244
	movs r0, #4
	lsls r1, r1, #2
	bl ObjectMotion_SetPositionAndReset
	movs r1, #128
	movs r0, #4
	lsls r1, r1, #6
	bl Func_02002bf8
	movs r3, #128
	movs r1, #180
	movs r2, #244
	lsls r3, r3, #6
	lsls r1, r1, #18
	lsls r2, r2, #16
	movs r0, #4
	bl Func_02002b90
	movs r0, #160
	lsls r0, r0, #2
	bl GameFlag_SetBit
	bl Func_02000490
	bl Func_02002b30
.L_02008634:
	pop {pc}
	.2byte 0x0000
	.section .text.x0200868c,"ax",%progbits
	.global Func_0200068c
	.thumb_func
Func_0200068c:
	push {lr}
	sub sp, #8
	movs r3, #45
	movs r2, #10
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #51
	movs r1, #10
	movs r2, #3
	movs r3, #3
	bl Func_02002af8
	bl Func_02000670
	add sp, #8
	pop {pc}
	.section .text.x020086ac,"ax",%progbits
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
	.4byte MsgFieldYouThereHoldYourGround
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
	.4byte MsgFieldHasEscaped
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
	.4byte MsgFieldHeyYouLotHoldIt
.L_020097c0:
	.4byte gPartyState
.L_020097c4:
	.4byte MsgFieldItWasThemWasnT
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
	.4byte MsgFieldWasAnythingStolen
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
	.4byte MsgFieldWait
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
	.4byte MsgFieldTheMayorTriedToGive
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
	.4byte MsgFieldICanTBelieveIt
.L_0200a43c:
	.4byte MsgFieldWhoIsThis
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
	.section .rodata.x0200ac98,"a",%progbits
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
	.4byte 0x0000002c
	.4byte 0x1010102e
	.4byte 0xffffffff
	.4byte 0x1020102d
	.4byte 0xffffffff
	.4byte 0x1030202d
	.4byte 0xffffffff
	.4byte 0x1040802d
	.4byte 0xffffffff
	.4byte 0x1050a02d
	.4byte 0xffffffff
	.4byte 0x1060f02d
	.4byte 0xffffffff
	.4byte 0x1070502d
	.4byte 0xffffffff
	.4byte 0x1080902d
	.4byte 0xffffffff
	.4byte 0x1091402d
	.4byte 0xffffffff
	.4byte 0x10a0202e
	.4byte 0xffffffff
	.4byte 0x10b09002
	.4byte 0xffffffff
	.4byte 0x10f01030
	.4byte 0xffffffff
	.4byte 0x11003030
	.4byte 0xffffffff
	.4byte 0x000001ff
	.global Data_02002d38
Data_02002d38:
	.4byte 0xffff0047
	.4byte 0x00000001
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00013000
	.4byte 0xffff0048
	.4byte 0x00000001
	.4byte 0x02e80000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x0001c000
	.4byte 0xffff0049
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00014000
	.4byte 0xffff004a
	.4byte 0x00000001
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x01b80000
	.4byte 0x0001a000
	.4byte 0xffff004b
	.4byte 0x00000001
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00014000
	.4byte 0xffff004c
	.4byte 0x00000001
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x01800000
	.4byte 0x0001c000
	.4byte 0xffff004d
	.4byte 0x00000001
	.4byte 0x01120000
	.4byte 0x00000000
	.4byte 0x01fe0000
	.4byte 0x00014000
	.4byte 0xffff004e
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x0001c000
	.4byte 0xffff004b
	.4byte 0x00000001
	.4byte 0x01d80000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00012000
	.4byte 0xffff0049
	.4byte 0x00000001
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00004000
	.4byte 0xffff00e0
	.4byte 0x00000001
	.4byte 0x01d80000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00010000
	.4byte 0xffff004e
	.4byte 0x00000001
	.4byte 0x02280000
	.4byte 0x00000000
	.4byte 0x01030000
	.4byte 0x00014000
	.4byte 0xffff0047
	.4byte 0x00000001
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x01d80000
	.4byte 0x0001e000
	.4byte 0xffff0048
	.4byte 0x00000001
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x01b80000
	.4byte 0x0001c000
	.4byte 0xffff00bb
	.4byte 0x00000001
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00015000
	.4byte 0xffff00bb
	.4byte 0x00000001
	.4byte 0x00800000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00015000
	.4byte 0xffff0047
	.4byte 0x00000001
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00014000
	.4byte 0xffff0047
	.4byte 0x00000001
	.4byte 0x01c80000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00014000
	.4byte 0xffff0047
	.4byte 0x00000001
	.4byte 0x02e80000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x0000c000
	.4byte 0x191100bb
	.4byte 0x00000001
	.4byte 0x00700000
	.4byte 0x00000000
	.4byte 0x01b80000
	.4byte 0x0001c000
	.4byte 0x191100c0
	.4byte 0x00000001
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x01b80000
	.4byte 0x0001c000
	.4byte 0xffff00cd
	.4byte 0x00000001
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x01100000
	.4byte 0x00014000
	.4byte 0xffff00ce
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x01000000
	.4byte 0x00014000
	.4byte 0x18ab00c6
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x01000000
	.4byte 0x00014000
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x0001c000
	.4byte 0xffff00c8
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x0001c000
	.4byte 0xffff0058
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x0001c000
	.4byte 0x08ab00ba
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x01000000
	.4byte 0x00014000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02002ff0
Data_02002ff0:
	.4byte 0xffff0047
	.4byte 0x00000001
	.4byte 0x00960000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x00015000
	.4byte 0xffff0048
	.4byte 0x00000001
	.4byte 0x02e80000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x0001c000
	.4byte 0xffff0049
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00014000
	.4byte 0xffff004a
	.4byte 0x00000001
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x01b80000
	.4byte 0x0001a000
	.4byte 0xffff004b
	.4byte 0x00000001
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00014000
	.4byte 0xffff004c
	.4byte 0x00000001
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x0001c000
	.4byte 0xffff004d
	.4byte 0x00000001
	.4byte 0x01120000
	.4byte 0x00000000
	.4byte 0x01fe0000
	.4byte 0x00014000
	.4byte 0xffff004e
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x0001c000
	.4byte 0xffff004b
	.4byte 0x00000001
	.4byte 0x01d80000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00014000
	.4byte 0xffff0049
	.4byte 0x00000001
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00004000
	.4byte 0xffff00e0
	.4byte 0x00000001
	.4byte 0x01d80000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00015000
	.4byte 0xffff004e
	.4byte 0x00000001
	.4byte 0x02280000
	.4byte 0x00000000
	.4byte 0x01030000
	.4byte 0x00014000
	.4byte 0xffff0047
	.4byte 0x00000001
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x01d80000
	.4byte 0x0001e000
	.4byte 0xffff0048
	.4byte 0x00000001
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x01b80000
	.4byte 0x0001a000
	.4byte 0xffff00bb
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00015000
	.4byte 0xffff00bb
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00015000
	.4byte 0xffff0047
	.4byte 0x00000001
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00014000
	.4byte 0xffff0047
	.4byte 0x00000001
	.4byte 0x01c80000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00014000
	.4byte 0xffff0047
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000c000
	.4byte 0x191100bb
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x0001c000
	.4byte 0x191100c0
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x0001c000
	.4byte 0xffff00cd
	.4byte 0x00000001
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x01100000
	.4byte 0x00014000
	.4byte 0xffff00ce
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x01000000
	.4byte 0x00014000
	.4byte 0xffff00c6
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x01000000
	.4byte 0x00014000
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x0001c000
	.4byte 0xffff0017
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x01140000
	.4byte 0x00012000
	.4byte 0xffff0014
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x0001c000
	.4byte 0xffff00ee
	.4byte 0x00000002
	.4byte 0x00700000
	.4byte 0x00000000
	.4byte 0x01890000
	.4byte 0x00014000
	.4byte 0xffff0007
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000c000
	.4byte 0xffff0005
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000c000
	.4byte 0xffff0006
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000c000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
.L_0200b2f0:
	.4byte 0x00270001
	.4byte 0x00020001
	.4byte 0xffff0006
.L_0200b2fc:
	.4byte 0x00270003
	.4byte 0x00020001
	.4byte 0xffff0006
	.global Data_02003308
Data_02003308:
	.4byte .L_0200b2fc
	.4byte 0x00030014
	.4byte .L_0200b2f0
	.4byte 0x0017000d
	.4byte .L_0200b2f0
	.4byte 0x00110011
	.4byte .L_0200b2f0
	.4byte 0x00120017
	.4byte .L_0200b2f0
	.4byte 0x000a001c
	.4byte .L_0200b2f0
	.4byte 0x0006001c
	.4byte .L_0200b2f0
	.4byte 0x00060009
	.4byte .L_0200b2f0
	.4byte 0x00180013
	.4byte .L_0200b2f0
	.4byte 0x00000000
	.4byte .L_0200b2fc
	.4byte 0x00030011
	.global Data_02003358
Data_02003358:
	.4byte 0x0000c602
	.4byte 0xffff0001
	.4byte Func_0200006c
	.4byte 0x0000c602
	.4byte 0xffff0002
	.4byte Func_0200006c
	.4byte 0x0000c602
	.4byte 0xffff0003
	.4byte Func_0200006c
	.4byte 0x0000c602
	.4byte 0xffff0004
	.4byte Func_0200006c
	.4byte 0x0000c602
	.4byte 0xffff0005
	.4byte Func_0200006c
	.4byte 0x0000c602
	.4byte 0xffff0006
	.4byte Func_0200006c
	.4byte 0x0000c602
	.4byte 0xffff0007
	.4byte Func_0200006c
	.4byte 0x0000c602
	.4byte 0xffff0008
	.4byte Func_0200006c
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x0000c602
	.4byte 0xffff000a
	.4byte Func_0200006c
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00000001
	.4byte 0xffff000f
	.4byte 0x0000000f
	.4byte 0x00000002
	.4byte 0xffff0010
	.4byte Func_0200051c
	.4byte 0x00000002
	.4byte 0x09100013
	.4byte Func_020029ac
	.4byte 0x00000002
	.4byte 0x09100014
	.4byte Func_02000ac4
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte MsgFieldTheTownElderTookOff
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte MsgFieldYouPickedTheWrongTime
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte MsgFieldThatBigWaveMovedThe
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte MsgFieldLetMeGuessYouWant
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte MsgFieldMaybeWeLlLearnBriggs
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte MsgFieldIfICouldWalkTo
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte Func_02000894
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte MsgFieldItSLikeAllThree
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte MsgFieldTheChampaWereSwarmingOver
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte MsgFieldThoseChampaCursDugA
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte MsgFieldIDoubtTheChampaCould
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte MsgFieldIGotSomeoneToShow
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte MsgFieldIDonTCareWhat
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte MsgFieldWeTookThatUnconsciousGuy
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte MsgFieldMyFriendWasOnGuard
	.4byte 0x00000000
	.4byte 0xffff0017
	.4byte MsgFieldMyPartnerWasGuardingThe
	.4byte 0x00000000
	.4byte 0xffff0018
	.4byte MsgFieldThereSNoWayThose
	.4byte 0x00000000
	.4byte 0xffff0019
	.4byte MsgFieldAllOfIndraShiftedAnd
	.4byte 0x00000000
	.4byte 0xffff001a
	.4byte Func_02000404
	.4byte 0x00000000
	.4byte 0xffff001b
	.4byte Func_0200083c
	.4byte 0x00000000
	.4byte 0xffff001c
	.4byte MsgFieldEvenIfWeTriedTo
	.4byte 0x00000000
	.4byte 0xffff001d
	.4byte Func_02000108
	.4byte 0x00000000
	.4byte 0xffff001e
	.4byte Func_0200018c
	.4byte 0x00000000
	.4byte 0xffff001f
	.4byte Func_02000210
	.4byte 0x00000000
	.4byte 0xffff0023
	.4byte Func_02000210
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte MsgFieldEveryoneThinksBriggsFledTo
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte MsgFieldTheTownElderIsGetting
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte MsgFieldIfABigWaveCan
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte MsgFieldItSABitOf
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte MsgFieldThatChampaInTheCell
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte MsgFieldIVeAlwaysWantedTo
	.4byte 0x00008d15
	.4byte 0x0300040e
	.4byte Func_02000968
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte MsgFieldWhoWasItWhoTold
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte MsgFieldNowMadraIsPinnedBetween
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte MsgFieldWhenISawThoseChampa
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte MsgFieldTheChampaMustHaveKnown
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte MsgFieldItSNotLikeThe
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte MsgFieldIMNotSureThe
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte MsgFieldILlNeverForgiveThose
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte MsgFieldILookedIntoThatMan
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte MsgFieldIWonderHowManyOf
	.4byte 0x00008d15
	.4byte 0xffff0017
	.4byte MsgFieldIMissMyPartnerIt
	.4byte 0x00008d15
	.4byte 0xffff0018
	.4byte MsgFieldItSSoWeirdThat
	.4byte 0x00008d15
	.4byte 0xffff0019
	.4byte MsgFieldTheseAreRoughTimesWe
	.4byte 0x00008d15
	.4byte 0xffff001a
	.4byte MsgFieldTheChampaDugThisHole
	.4byte 0x00008d15
	.4byte 0xffff001b
	.4byte MsgFieldItWasAllJustA
	.4byte 0x00008d15
	.4byte 0xffff001c
	.4byte MsgFieldMySoreBottomTellsMe
	.4byte 0x00008d15
	.4byte 0xffff001d
	.4byte MsgFieldTheItemVendorGotWounded
	.4byte 0x00008d15
	.4byte 0xffff001e
	.4byte MsgFieldThoseLousyChampaNextTime
	.4byte 0x00008d15
	.4byte 0xffff001f
	.4byte MsgFieldBriggsAndHisMenAre
	.4byte 0x00008d15
	.4byte 0xffff0023
	.4byte MsgFieldBriggsAndHisMenAre
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_0200367c
Data_0200367c:
	.4byte 0x0000c602
	.4byte 0xffff0001
	.4byte Func_0200006c
	.4byte 0x0000c602
	.4byte 0xffff0002
	.4byte Func_0200006c
	.4byte 0x0000c602
	.4byte 0xffff0003
	.4byte Func_0200006c
	.4byte 0x0000c602
	.4byte 0xffff0004
	.4byte Func_0200006c
	.4byte 0x0000c602
	.4byte 0xffff0005
	.4byte Func_0200006c
	.4byte 0x0000c602
	.4byte 0xffff0006
	.4byte Func_0200006c
	.4byte 0x0000c602
	.4byte 0xffff0007
	.4byte Func_0200006c
	.4byte 0x0000c602
	.4byte 0xffff0008
	.4byte Func_0200006c
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x0000c602
	.4byte 0xffff000a
	.4byte Func_0200006c
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00000001
	.4byte 0xffff000f
	.4byte 0x0000000f
	.4byte 0x00000002
	.4byte 0xffff0010
	.4byte Func_020005b8
	.4byte 0x00000002
	.4byte 0x09140013
	.4byte Func_020029b8
	.4byte 0x00000002
	.4byte 0x09140014
	.4byte Func_020014fc
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte MsgFieldWeThinkTheKibomboCame
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte MsgFieldSoTheKibomboGotInto
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte MsgFieldTheKibomboCameInThrough
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte MsgFieldIHeardThatTheElder
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte MsgFieldIGuessThatGuyWe
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte MsgFieldGondowanSoundsLikeAFrightening
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte Func_02000894
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte MsgFieldICanTBelieveSomeone
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte MsgFieldNowThatWeReSealing
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte MsgFieldThereAreStillPlentyOf
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte MsgFieldBriggsDidnTHaveNearly
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte MsgFieldTheseCavesWereMySecret
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte MsgFieldThatGuyWasnTOne
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte MsgFieldOhWeReleasedHimAs
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte MsgFieldEveryoneYelledAtMeFor
	.4byte 0x00000000
	.4byte 0xffff0017
	.4byte MsgFieldYouShouldaSeenMyPartner
	.4byte 0x00000000
	.4byte 0xffff0018
	.4byte MsgFieldFirstPiratesRaidOurTown
	.4byte 0x00000000
	.4byte 0xffff0019
	.4byte MsgFieldMyGuessIsTheKibombo
	.4byte 0x00000000
	.4byte 0xffff001a
	.4byte Func_02000490
	.4byte 0x00000000
	.4byte 0xffff001d
	.4byte Func_02000108
	.4byte 0x00000000
	.4byte 0xffff001e
	.4byte Func_0200018c
	.4byte 0x00000000
	.4byte 0xffff001f
	.4byte Func_02000210
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte MsgFieldIfItWerenTFor
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte MsgFieldIfGondowanIsReallyThat
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte MsgFieldItSStrangeItS
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte MsgFieldTheMayorIsSupposedTo
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte MsgFieldHeToldUsHeWas
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte MsgFieldInGondowanTheyHaveEvil
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte MsgFieldIHeardTheGuysWho
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte MsgFieldIfTheyWereStrongEnough
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte MsgFieldItWasSoConvenientBeing
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte MsgFieldIWonderWhoDugThose
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte MsgFieldWaitIfBriggsOnlyHas
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte MsgFieldIVeNeverSeenAny
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte MsgFieldDidnTSeemLikeHe
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte MsgFieldWasSoHappyToGet
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte MsgFieldIDidnTHaveA
	.4byte 0x00008d15
	.4byte 0xffff0017
	.4byte MsgFieldICanTBelieveNobody
	.4byte 0x00008d15
	.4byte 0xffff0018
	.4byte MsgFieldTheWayThingsHaveBeen
	.4byte 0x00008d15
	.4byte 0xffff0019
	.4byte MsgFieldIVeBeenStandingFor
	.4byte 0x00008d15
	.4byte 0xffff001a
	.4byte MsgFieldWeSealedUpTheHole
	.4byte 0x00008d15
	.4byte 0xffff001d
	.4byte MsgFieldTheChampaWeaponsWeFound
	.4byte 0x00008d15
	.4byte 0xffff001e
	.4byte MsgFieldTheGroupThatAttackedFrom
	.4byte 0x00008d15
	.4byte 0xffff001f
	.4byte MsgFieldThatShinIsAlwaysBugging
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_0200394c
Data_0200394c:
	.4byte 0x0000c602
	.4byte 0xffff0001
	.4byte Func_0200006c
	.4byte 0x0000c602
	.4byte 0xffff0002
	.4byte Func_0200006c
	.4byte 0x0000c602
	.4byte 0xffff0003
	.4byte Func_0200006c
	.4byte 0x0000c602
	.4byte 0xffff0004
	.4byte Func_0200006c
	.4byte 0x0000c602
	.4byte 0xffff0005
	.4byte Func_0200006c
	.4byte 0x0000c602
	.4byte 0xffff0006
	.4byte Func_0200006c
	.4byte 0x0000c602
	.4byte 0xffff0007
	.4byte Func_0200006c
	.4byte 0x0000c602
	.4byte 0xffff0008
	.4byte Func_0200006c
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x0000c602
	.4byte 0xffff000a
	.4byte Func_0200006c
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00000001
	.4byte 0xffff000f
	.4byte 0x0000000f
	.4byte 0x00000002
	.4byte 0x098e0015
	.4byte Func_0200036c
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte MsgFieldIDonTThinkThe
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte MsgFieldWeGoToAllThat
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte MsgFieldIHeardTheBoatThe
	.4byte 0x00000000
	.4byte 0x0301000b
	.4byte Func_02000a48
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte MsgFieldThoseTravelersLookedWorriedAbout
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte MsgFieldIWonderIfTheLegends
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte MsgFieldIDonTKnowWhat
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte MsgFieldAGroupOfSharpLooking
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte MsgFieldIfYouHaveABig
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte MsgFieldIWasAtTheMayor
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte MsgFieldItSNotThatI
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte MsgFieldTheMayorAskedMeTo
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte MsgFieldItSeemsNoOneIs
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte MsgFieldSayThereWasAnotherGroup
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte MsgFieldIThoughtShinAndThe
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte MsgFieldWhenAndHisPartyHeard
	.4byte 0x00000000
	.4byte 0xffff0017
	.4byte MsgFieldUhhhnnnUhhhnnn
	.4byte 0x00000000
	.4byte 0xffff0018
	.4byte MsgFieldTheMayorFinallyReturnedFrom
	.4byte 0x00000000
	.4byte 0xffff0019
	.4byte MsgFieldHeyItSYouGuys
	.4byte 0x00000000
	.4byte 0xffff001a
	.4byte MsgFieldYeahWellWeReWorkin
	.4byte 0x00000000
	.4byte 0xffff001d
	.4byte Func_02000108
	.4byte 0x00000000
	.4byte 0xffff001e
	.4byte Func_0200018c
	.4byte 0x00000000
	.4byte 0xffff001f
	.4byte Func_02000210
	.4byte 0x00000000
	.4byte 0xffff0021
	.4byte Func_020002bc
	.4byte 0x00000000
	.4byte 0xffff0023
	.4byte Func_020003b8
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte MsgFieldWhyWouldAnyoneBeHappy
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte MsgFieldTheyVeBeenHavingSo
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte MsgFieldEveryoneGetsTheUrgeTo
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte MsgFieldIWonderWhyThoseTravelers
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte MsgFieldWhatWasThatIslandAgain
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte MsgFieldThePeopleOfMadraKnow
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte MsgFieldThatIslandThatGroupWas
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte MsgFieldHowBigWouldAShip
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte MsgFieldTheseNewBoatsApparentlyUse
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte MsgFieldIfABuildingHasBroken
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte MsgFieldSailorsAreSupposedToUse
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte MsgFieldOurMayorSDoneSo
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte MsgFieldIsTurningWaterToIce
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte MsgFieldMadraIsAlwaysSoHot
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte MsgFieldSoThisFellowMustBe
	.4byte 0x00008d15
	.4byte 0xffff0017
	.4byte MsgFieldIKnewIShouldHave
	.4byte 0x00008d15
	.4byte 0xffff0018
	.4byte MsgFieldHeWasGoneSoLong
	.4byte 0x00008d15
	.4byte 0xffff0019
	.4byte MsgFieldHowMuchLongerDoWe
	.4byte 0x00008d15
	.4byte 0xffff001d
	.4byte MsgFieldThePiratesAndTheKibombo
	.4byte 0x00008d15
	.4byte 0xffff001e
	.4byte MsgFieldThatNightTheChampaCame
	.4byte 0x00008d15
	.4byte 0xffff001f
	.4byte MsgFieldShinSeemsTerrifiedOfI
	.4byte 0x00008d15
	.4byte 0xffff0021
	.4byte Func_02000320
	.4byte 0x00008d15
	.4byte 0xffff0023
	.4byte MsgFieldWeReStillRecoveringFrom
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
