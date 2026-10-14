.syntax unified
	.thumb
	.section .text.x020080f4,"ax",%progbits
	.balign 4
	.global Func_020000f4
	.thumb_func
Func_020000f4:
	push {r5, lr}
	ldr r3, .L_02008140
	movs r1, #6
	ldr r0, [r3]
	bl __umodsi3
	cmp r0, #0
	bne .L_02008126
	ldr r3, .L_02008144
	movs r0, #14
	ldrh r3, [r3]
	lsls r3, r3, #16
	asrs r1, r3, #16
.L_0200810e:
	ldr r4, .L_02008148
	lsls r3, r0, #1
	adds r2, r3, r4
	subs r4, #2
	adds r3, r3, r4
	ldrh r3, [r3]
	subs r0, #1
	strh r3, [r2]
	cmp r0, #1
	bne .L_0200810e
	ldr r3, .L_0200814c
	strh r1, [r3]
.L_02008126:
	ldr r3, .L_02008140
	movs r1, #12
	ldr r0, [r3]
	ldr r5, .L_02008150
	lsrs r0, r0, #2
	bl __umodsi3
	lsls r0, r0, #1
	ldrsh r1, [r5, r0]
	ldr r3, .L_02008154
	strh r1, [r3]
	pop {r5, pc}
	.2byte 0x0000
.L_02008140:
	.4byte gFrameCount
.L_02008144:
	.4byte 0x0500019c
.L_02008148:
	.4byte 0x05000180
.L_0200814c:
	.4byte 0x05000182
.L_02008150:
	.4byte Data_02002608
.L_02008154:
	.4byte 0x0500019e
	.section .text.x02008158,"ax",%progbits
	.global Func_02000158
	.thumb_func
Func_02000158:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r1, #214
	movs r2, #129
	lsls r1, r1, #1
	lsls r2, r2, #1
	adds r3, r3, r1
	adds r2, #255
	movs r1, #200
	str r2, [r3]
	lsls r1, r1, #4
	ldr r0, .L_020081cc
	bl Scheduler_AddOrUpdateCallback
	movs r1, #3
	movs r0, #8
	bl ObjectMotion_SetActionVariant
	ldr r2, .L_020081d0
	movs r1, #241
	lsls r1, r1, #1
	adds r3, r2, r1
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #6
	bne .L_020081a6
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r2, r1
	ldr r0, [r3]
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #32
	orrs r3, r2
	strb r3, [r0]
.L_020081a6:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #106
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020081c8
	movs r0, #7
	movs r1, #0
	movs r2, #0
	bl Func_02002024
	movs r0, #15
	movs r1, #0
	movs r2, #0
	bl Func_02002024
.L_020081c8:
	movs r0, #0
	pop {pc}
.L_020081cc:
	.4byte Func_020000f4
.L_020081d0:
	.4byte gPartyState
	.section .text.x020081d8,"ax",%progbits
	.balign 4
	.global Func_020001d8
	.thumb_func
Func_020001d8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r0, #13
	bl Object_GetById
	movs r1, #5
	adds r6, r0, #0
	movs r0, #13
	bl Object_SetModeById
	movs r0, #28
	bl Battle_WaitMode0
	movs r0, #112
	bl Func_02002124
	ldr r1, [r6, #8]
	ldr r2, [r6, #12]
	movs r3, #128
	lsls r3, r3, #13
	movs r7, #160
	movs r0, #128
	mov r11, r3
	lsls r7, r7, #12
	lsls r0, r0, #2
	adds r1, r1, r7
	add r2, r11
	ldr r3, [r6, #16]
	adds r0, #162
	bl Object_Spawn
	movs r2, #128
	adds r5, r0, #0
	lsls r2, r2, #10
	str r2, [r5, #48]
	str r2, [r5, #52]
	mov r8, r2
	movs r2, #0
	mov r9, r2
	adds r3, r5, #0
	mov r2, r9
	adds r3, #85
	strb r2, [r3]
	movs r3, #192
	ldr r1, [r5, #8]
	lsls r3, r3, #11
	adds r1, r1, r3
	ldr r2, [r5, #12]
	movs r3, #128
	lsls r3, r3, #12
	mov r10, r3
	add r2, r10
	ldr r3, [r5, #16]
	bl Func_02001fa4
	ldr r1, [r6, #8]
	ldr r2, [r6, #12]
	movs r0, #128
	lsls r0, r0, #2
	adds r1, r1, r7
	add r2, r11
	ldr r3, [r6, #16]
	adds r0, #162
	bl Object_Spawn
	adds r6, r0, #0
	mov r2, r8
	adds r3, r6, #0
	str r2, [r6, #48]
	str r2, [r6, #52]
	adds r3, #85
	mov r2, r9
	strb r2, [r3]
	ldr r3, .L_020082b0
	ldr r1, [r5, #8]
	ldr r2, [r5, #12]
	add r1, r10
	adds r2, r2, r3
	ldr r3, [r5, #16]
	bl Func_02001fa4
	adds r0, r5, #0
	bl Func_02001fac
	adds r0, r5, #0
	bl Func_02001f9c
	adds r0, r6, #0
	bl Func_02001fac
	adds r0, r6, #0
	bl Func_02001f9c
	movs r0, #30
	bl Battle_WaitMode0
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_020082b0:
	.4byte 0xfffc0000
	.section .text.x020082b4,"ax",%progbits
	.global Func_020002b4
	.thumb_func
Func_020002b4:
	push {r5, r6, lr}
	ldr r3, .L_02008380
	sub sp, #12
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #1
	bne .L_020082e8
	ldr r4, .L_02008384
	ldr r1, .L_02008388
	ldr r0, [r4]
	movs r2, #15
	asrs r3, r0, #3
	ands r3, r2
	ldrb r1, [r1, r3]
	lsls r2, r1, #8
	movs r3, #16
	subs r3, r3, r1
	orrs r2, r3
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #82
	strh r2, [r3]
	adds r0, #1
	str r0, [r4]
	b .L_02008338
.L_020082e8:
	cmp r3, #2
	bne .L_02008338
	ldr r6, .L_02008384
	movs r1, #28
	ldr r0, [r6]
	asrs r0, r0, #2
	bl __modsi3
	ldr r2, .L_0200838c
	adds r5, r0, #0
	lsls r3, r5, #1
	ldrh r2, [r2, r3]
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #82
	strh r2, [r3]
	cmp r5, #0
	bne .L_0200831c
	ldr r3, .L_02008390
	movs r2, #1
	str r2, [r3]
	ldr r3, .L_02008394
	movs r0, #153
	str r5, [r3]
	bl Func_02002124
.L_0200831c:
	cmp r5, #14
	bne .L_02008332
	ldr r2, .L_02008390
	movs r3, #0
	str r3, [r2]
	ldr r2, .L_02008394
	movs r3, #1
	str r3, [r2]
	movs r0, #153
	bl Func_02002124
.L_02008332:
	ldr r3, [r6]
	adds r3, #1
	str r3, [r6]
.L_02008338:
	movs r3, #208
	mov r5, sp
	lsls r3, r3, #18
	str r3, [r5]
	movs r3, #0
	str r3, [r5, #4]
	movs r3, #224
	lsls r3, r3, #17
	str r3, [r5, #8]
	adds r0, r5, #0
	bl Func_020020d4
	ldr r3, .L_02008390
	ldr r3, [r3]
	cmp r3, #0
	beq .L_02008366
	ldr r3, [r5]
	ldr r0, .L_02008398
	str r3, [r0, #12]
	ldr r3, [r5, #8]
	str r3, [r0, #16]
	bl Func_0200210c
.L_02008366:
	ldr r3, .L_02008394
	ldr r3, [r3]
	cmp r3, #0
	beq .L_0200837c
	ldr r3, [r5]
	ldr r0, .L_0200839c
	str r3, [r0, #12]
	ldr r3, [r5, #8]
	str r3, [r0, #16]
	bl Func_0200210c
.L_0200837c:
	add sp, #12
	pop {r5, r6, pc}
.L_02008380:
	.4byte Data_0200308c
.L_02008384:
	.4byte Data_02003098
.L_02008388:
	.4byte Data_02002fd7
.L_0200838c:
	.4byte Data_02002fe8
.L_02008390:
	.4byte Data_02003090
.L_02008394:
	.4byte Data_02003024
.L_02008398:
	.4byte Data_02003060
.L_0200839c:
	.4byte Data_02003030
	.section .text.x020083a0,"ax",%progbits
	.global Func_020003a0
	.thumb_func
Func_020003a0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r0, #128
	lsls r0, r0, #5
	sub sp, #4
	bl MemAlloc
	mov r9, r0
	movs r2, #253
	movs r3, #128
	lsls r3, r3, #19
	lsls r2, r2, #6
	adds r3, #80
	strh r2, [r3]
	adds r3, #2
	movs r1, #128
	lsls r1, r1, #5
	strh r1, [r3]
	ldr r0, .L_020084b4
	movs r7, #32
	mov r1, r9
	bl Func_02001f6c
	ldr r5, .L_020084b8
	bl Resource_FindFreeEntry
	movs r1, #128
	mov r2, r9
	str r0, [r5]
	lsls r1, r1, #5
	ldr r5, .L_020084bc
	bl VramBlock_LoadCached
	ldr r6, .L_020084c0
	movs r3, #192
	str r0, [r5]
	movs r1, #0
	str r0, [sp, #0]
	movs r2, #0
	adds r0, r6, #0
	lsls r3, r3, #24
	bl Func_02002104
	movs r3, #1
	strh r3, [r6, #30]
	movs r5, #13
	ldrb r3, [r6, #9]
	negs r5, r5
	adds r2, r5, #0
	ands r2, r3
	movs r3, #8
	mov r8, r3
	mov r1, r8
	orrs r2, r1
	ldrb r1, [r6, #5]
	adds r3, r5, #0
	ands r3, r1
	movs r1, #4
	mov r10, r1
	mov r1, r10
	orrs r3, r1
	orrs r3, r7
	strb r3, [r6, #5]
	movs r3, #15
	ands r2, r3
	strb r2, [r6, #9]
	mov r1, r9
	ldr r0, .L_020084c4
	mov r11, r3
	bl Func_02001f6c
	ldr r6, .L_020084c8
	bl Resource_FindFreeEntry
	movs r1, #128
	mov r2, r9
	str r0, [r6]
	lsls r1, r1, #5
	ldr r6, .L_020084cc
	bl VramBlock_LoadCached
	str r0, [r6]
	ldr r6, .L_020084d0
	movs r3, #192
	str r0, [sp, #0]
	movs r1, #0
	adds r0, r6, #0
	movs r2, #0
	lsls r3, r3, #24
	bl Func_02002104
	ldrb r2, [r6, #9]
	movs r3, #2
	strh r3, [r6, #30]
	adds r3, r5, #0
	mov r1, r8
	ands r3, r2
	ldrb r2, [r6, #5]
	orrs r3, r1
	mov r1, r11
	ands r3, r1
	strb r3, [r6, #9]
	ands r5, r2
	ldr r3, .L_020084d4
	mov r2, r10
	orrs r5, r2
	orrs r5, r7
	movs r2, #0
	strb r5, [r6, #5]
	str r2, [r3]
	ldr r3, .L_020084d8
	movs r1, #0
	str r2, [r3]
	ldr r3, .L_020084dc
	ldr r0, .L_020084e0
	str r2, [r3]
	ldr r3, .L_020084e4
	strb r1, [r3]
	movs r1, #144
	lsls r1, r1, #3
	bl Scheduler_AddOrUpdateCallback
	mov r0, r9
	bl MemFree
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_020084b4:
	.4byte Data_02002620
.L_020084b8:
	.4byte Data_02003020
.L_020084bc:
	.4byte Data_02003088
.L_020084c0:
	.4byte Data_02003060
.L_020084c4:
	.4byte Data_02002af8
.L_020084c8:
	.4byte Data_02003094
.L_020084cc:
	.4byte Data_02003058
.L_020084d0:
	.4byte Data_02003030
.L_020084d4:
	.4byte Data_02003090
.L_020084d8:
	.4byte Data_02003024
.L_020084dc:
	.4byte Data_02003098
.L_020084e0:
	.4byte Func_020002b4
.L_020084e4:
	.4byte Data_0200308c
	.section .text.x0200858c,"ax",%progbits
	.global Func_0200058c
	.thumb_func
Func_0200058c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	bl Func_02001fcc
	movs r0, #0
	bl Func_020020cc
	ldr r0, .L_02008630
	bl Func_0200205c
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #16
	ldr r1, .L_02008634
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r1, #24
	movs r2, #0
	movs r0, #16
	negs r1, r1
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #160
	movs r0, #16
	lsls r1, r1, #7
	bl Func_0200207c
	ldr r3, .L_02008638
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r3, r2
	ldr r1, [r5]
	movs r2, #0
	movs r0, #16
	bl ObjectMotion_SetAngleToward
	movs r1, #0
	movs r0, #16
	bl UiText_OpenMessageAtObject
	movs r0, #4
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	adds r7, r0, #0
	cmp r7, #0
	beq .L_0200863c
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #16
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #16
	movs r1, #0
	movs r2, #5
	bl Func_0200206c
	ldr r0, [r5]
	movs r1, #0
	movs r2, #16
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #16
	movs r1, #24
	movs r2, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #160
	movs r0, #16
	lsls r1, r1, #7
	bl Func_0200207c
	bl Func_02001fd4
	bl .L_02009f20
.L_02008630:
	.4byte MsgScene68DNo11
.L_02008634:
	.4byte 0x00013333
.L_02008638:
	.4byte gPartyState
.L_0200863c:
	movs r1, #3
	movs r0, #16
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #16
	movs r1, #24
	movs r2, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #160
	movs r2, #0
	movs r0, #16
	lsls r1, r1, #7
	bl ObjectMotion_ArmCallback
	ldr r1, [r5]
	movs r0, #16
	bl Object_LinkObjectAndSetCallback
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #106
	bl GameFlag_SetBit
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #107
	bl GameFlag_SetBit
	movs r0, #7
	bl Party_AddActiveOwner
	movs r0, #64
	bl Object_GetById
	movs r1, #0
	str r7, [r0, #28]
	movs r2, #0
	movs r0, #14
	bl ObjectMotion_ArmCallback
	movs r1, #179
	movs r2, #178
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #13
	adds r1, #51
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r1, #179
	movs r2, #178
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #14
	adds r1, #51
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r1, #179
	movs r2, #178
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #7
	adds r1, #51
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r1, #218
	movs r2, #132
	movs r0, #4
	lsls r1, r1, #2
	lsls r2, r2, #2
	bl ObjectMotion_SetPositionAndReset
	movs r0, #218
	movs r1, #1
	movs r2, #204
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #17
	lsls r0, r0, #18
	bl Motion_CamBounds
	bl Func_020020ac
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #160
	movs r0, #16
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r2, #45
	lsls r1, r1, #1
	movs r0, #13
	bl Func_0200208c
	movs r1, #160
	lsls r1, r1, #7
	movs r0, #13
	bl Func_0200207c
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #13
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #128
	adds r3, #1
	lsls r0, r0, #6
	strh r3, [r2]
	adds r0, #13
	movs r1, #0
	movs r2, #5
	bl Func_0200206c
	movs r1, #128
	movs r0, #7
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r2, #0
	movs r0, #14
	lsls r1, r1, #6
	bl ObjectMotion_ArmCallback
	movs r1, #160
	lsls r1, r1, #7
	movs r0, #15
	bl Func_0200207c
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #0
	movs r0, #14
	bl Func_0200207c
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #14
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #5
	movs r0, #14
	movs r1, #0
	bl Func_0200206c
	movs r0, #13
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #128
	lsls r0, r0, #6
	movs r2, #5
	adds r0, #13
	movs r1, #0
	bl Func_0200206c
	movs r1, #2
	movs r0, #15
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl Battle_WaitMode0
	movs r1, #128
	movs r0, #15
	lsls r1, r1, #8
	bl Func_0200207c
	movs r1, #4
	adds r1, #255
	movs r2, #30
	movs r0, #15
	bl Func_0200208c
	movs r0, #15
	movs r1, #0
	movs r2, #5
	bl Func_0200206c
	movs r1, #224
	movs r0, #7
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r2, #0
	movs r0, #13
	movs r1, #0
	bl ObjectMotion_ArmCallback
	movs r1, #0
	movs r0, #14
	bl Func_0200207c
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #13
	bl Func_0200208c
	movs r2, #5
	movs r0, #13
	movs r1, #0
	bl Func_0200206c
	movs r0, #15
	movs r1, #2
	bl Motion_SetVarCbAndRefresh
	movs r2, #5
	movs r0, #15
	movs r1, #0
	bl Func_0200206c
	movs r1, #3
	movs r0, #13
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #5
	movs r0, #13
	movs r1, #0
	bl Func_0200206c
	movs r1, #160
	lsls r1, r1, #7
	movs r0, #13
	bl Func_0200207c
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #13
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	movs r2, #5
	adds r0, #13
	movs r1, #0
	bl Func_0200206c
	movs r0, #13
	movs r1, #1
	bl Object_AttachWorkTargetToObject
	movs r1, #216
	movs r2, #222
	lsls r1, r1, #2
	lsls r2, r2, #1
	movs r0, #13
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #212
	movs r2, #222
	lsls r1, r1, #2
	lsls r2, r2, #1
	movs r0, #14
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #208
	movs r2, #222
	lsls r2, r2, #1
	lsls r1, r1, #2
	movs r0, #7
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r0, #13
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #13
	movs r1, #1
	bl Object_SetModeById
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #6
	movs r0, #13
	bl ObjectMotion_ArmCallback
	movs r0, #14
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #14
	movs r1, #1
	bl Object_SetModeById
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #6
	movs r0, #14
	bl ObjectMotion_ArmCallback
	movs r0, #7
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #7
	movs r1, #1
	bl Object_SetModeById
	movs r1, #128
	movs r0, #7
	lsls r1, r1, #7
	bl Func_0200207c
	movs r0, #218
	movs r1, #1
	movs r2, #248
	negs r1, r1
	lsls r2, r2, #17
	movs r3, #1
	lsls r0, r0, #18
	bl Motion_CamBounds
	bl Func_020020ac
	movs r0, #5
	bl Battle_WaitMode0
	movs r3, #176
	movs r0, #17
	movs r1, #16
	movs r2, #0
	lsls r3, r3, #8
	bl Func_020020dc
	movs r1, #16
	movs r3, #192
	movs r0, #5
	negs r1, r1
	movs r2, #0
	lsls r3, r3, #8
	bl Func_020020dc
	movs r1, #32
	movs r3, #192
	lsls r3, r3, #8
	movs r0, #6
	negs r1, r1
	movs r2, #0
	bl Func_020020dc
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #4
	bl ObjectMotion_ArmCallback
	movs r0, #6
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #13
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	movs r2, #5
	adds r0, #13
	movs r1, #0
	bl Func_0200206c
	movs r1, #0
	movs r0, #7
	bl Func_0200207c
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #7
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #7
	movs r0, #7
	bl Func_0200207c
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #7
	movs r1, #0
	movs r2, #5
	bl Func_0200206c
	movs r1, #160
	movs r2, #0
	movs r0, #14
	lsls r1, r1, #7
	bl ObjectMotion_ArmCallback
	movs r1, #160
	movs r0, #13
	lsls r1, r1, #7
	bl Func_0200207c
	movs r2, #8
	negs r2, r2
	movs r1, #0
	movs r0, #6
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #6
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #128
	movs r0, #7
	lsls r1, r1, #7
	bl Func_0200207c
	movs r0, #7
	movs r1, #0
	movs r2, #5
	bl Func_0200206c
	movs r1, #192
	movs r2, #0
	movs r0, #14
	lsls r1, r1, #6
	bl ObjectMotion_ArmCallback
	movs r1, #160
	movs r0, #13
	lsls r1, r1, #7
	bl Func_0200207c
	movs r2, #8
	negs r2, r2
	movs r1, #0
	movs r0, #5
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #5
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #128
	movs r0, #7
	lsls r1, r1, #6
	bl Func_0200207c
	movs r0, #7
	movs r1, #0
	movs r2, #5
	bl Func_0200206c
	movs r1, #192
	movs r2, #0
	movs r0, #14
	lsls r1, r1, #6
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #13
	lsls r1, r1, #6
	bl Func_0200207c
	movs r2, #8
	negs r2, r2
	movs r1, #0
	movs r0, #4
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #128
	movs r0, #7
	lsls r1, r1, #6
	bl Func_0200207c
	movs r0, #7
	movs r1, #0
	movs r2, #5
	bl Func_0200206c
	movs r1, #192
	movs r2, #0
	movs r0, #14
	lsls r1, r1, #6
	bl ObjectMotion_ArmCallback
	movs r1, #192
	lsls r1, r1, #6
	movs r0, #13
	bl Func_0200207c
	movs r0, #17
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	mov r10, r3
	ands r3, r2
	movs r2, #8
	strb r3, [r0]
	movs r1, #0
	negs r2, r2
	movs r0, #17
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #1
	bl Battle_WaitMode0
	movs r0, #17
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	movs r2, #1
	mov r9, r2
	mov r2, r9
	orrs r3, r2
	strb r3, [r0]
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #17
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #0
	movs r0, #14
	bl Func_0200207c
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #5
	movs r0, #14
	movs r1, #0
	bl Func_0200206c
	movs r1, #128
	lsls r1, r1, #8
	movs r0, #13
	bl Func_0200207c
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #13
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #192
	movs r2, #0
	movs r0, #14
	lsls r1, r1, #6
	bl ObjectMotion_ArmCallback
	movs r1, #192
	lsls r1, r1, #6
	movs r0, #13
	bl Func_0200207c
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #13
	bl Func_0200208c
	movs r0, #128
	lsls r0, r0, #6
	movs r2, #5
	adds r0, #13
	movs r1, #0
	bl Func_0200206c
	movs r0, #17
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #17
	bl Func_02002094
	movs r0, #37
	bl Battle_WaitMode0
	movs r2, #5
	movs r0, #17
	movs r1, #0
	bl Func_0200206c
	movs r1, #3
	movs r0, #13
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #13
	movs r1, #0
	movs r2, #0
	bl Func_0200206c
	movs r2, #5
	movs r0, #15
	movs r1, #0
	bl Func_0200206c
	movs r1, #208
	lsls r1, r1, #8
	movs r0, #13
	bl Func_0200207c
	movs r0, #15
	bl Battle_WaitMode0
	movs r2, #5
	movs r0, #15
	movs r1, #0
	bl Func_0200206c
	movs r1, #192
	lsls r1, r1, #6
	movs r0, #13
	bl Func_0200207c
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #13
	movs r1, #0
	movs r2, #5
	bl Func_0200206c
	movs r1, #6
	adds r1, #255
	movs r2, #30
	movs r0, #13
	bl Func_0200208c
	movs r0, #128
	lsls r0, r0, #6
	movs r2, #5
	adds r0, #13
	movs r1, #0
	bl Func_0200206c
	movs r1, #2
	movs r0, #13
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #13
	movs r1, #0
	movs r2, #0
	bl Func_0200206c
	movs r2, #5
	movs r0, #15
	movs r1, #0
	bl Func_0200206c
	movs r1, #2
	movs r0, #7
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #224
	lsls r1, r1, #8
	movs r0, #7
	bl Func_0200207c
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #7
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r2, #5
	movs r0, #7
	movs r1, #0
	bl Func_0200206c
	movs r1, #208
	lsls r1, r1, #8
	movs r0, #13
	bl Func_0200207c
	movs r0, #5
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	movs r2, #5
	adds r0, #13
	movs r1, #0
	bl Func_0200206c
	movs r0, #13
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #128
	lsls r0, r0, #6
	movs r2, #5
	adds r0, #13
	movs r1, #0
	bl Func_0200206c
	movs r0, #17
	movs r1, #2
	bl Motion_SetVarCbAndRefresh
	movs r0, #17
	movs r1, #0
	movs r2, #5
	bl Func_0200206c
	movs r1, #128
	movs r0, #7
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r2, #0
	movs r0, #14
	lsls r1, r1, #6
	bl ObjectMotion_ArmCallback
	movs r1, #192
	lsls r1, r1, #6
	movs r0, #13
	bl Func_0200207c
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #13
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #13
	bl Func_020001d8
	movs r1, #132
	lsls r1, r1, #6
	adds r1, #8
	movs r0, #32
	bl Func_02002114
	movs r0, #32
	bl Battle_WaitMode0
	movs r0, #20
	bl Battle_WaitMode0
	bl Func_020003a0
	ldr r5, .L_02008fe0
	ldr r6, .L_02008fe4
	movs r3, #1
	str r3, [r6]
	movs r0, #153
	str r7, [r5]
	mov r8, r3
	bl Func_02002124
	bl Func_02000510
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #4
	bl Func_0200208c
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #17
	bl Func_0200208c
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #6
	bl Func_0200208c
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #55
	movs r0, #5
	bl Func_0200208c
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #13
	movs r1, #0
	movs r2, #5
	bl Func_0200206c
	movs r0, #15
	movs r1, #0
	movs r2, #10
	bl Func_0200206c
	movs r1, #6
	adds r1, #255
	movs r2, #50
	movs r0, #17
	bl Func_0200208c
	movs r0, #13
	bl Func_020001d8
	bl Func_02000550
	movs r0, #30
	bl Battle_WaitMode0
	mov r2, r8
	movs r0, #153
	str r7, [r6]
	str r2, [r5]
	bl Func_02002124
	bl Func_02000510
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #13
	movs r1, #0
	movs r2, #10
	bl Func_0200206c
	movs r1, #0
	movs r2, #5
	movs r0, #14
	bl Func_0200206c
	movs r0, #17
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	mov r2, r10
	ands r2, r3
	strb r2, [r0]
	mov r10, r2
	movs r2, #16
	movs r1, #0
	negs r2, r2
	movs r0, #17
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #1
	bl Battle_WaitMode0
	movs r0, #17
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	mov r2, r9
	orrs r2, r3
	strb r2, [r0]
	movs r0, #25
	mov r9, r2
	bl Battle_WaitMode0
	movs r0, #17
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #17
	bl Func_02002094
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #17
	movs r1, #0
	movs r2, #5
	bl Func_0200206c
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #13
	bl Func_0200208c
	movs r0, #128
	lsls r0, r0, #6
	movs r2, #5
	adds r0, #13
	movs r1, #0
	bl Func_0200206c
	movs r1, #0
	movs r0, #6
	bl Func_0200207c
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #2
	adds r1, #255
	movs r2, #30
	movs r0, #6
	bl Func_0200208c
	movs r0, #6
	movs r1, #0
	movs r2, #5
	bl Func_0200206c
	movs r2, #16
	movs r0, #17
	movs r1, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #128
	lsls r1, r1, #8
	movs r0, #17
	bl Func_0200207c
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #10
	adds r1, #255
	movs r2, #30
	movs r0, #17
	bl Func_0200208c
	movs r0, #17
	movs r1, #0
	movs r2, #5
	bl Func_0200206c
	movs r1, #128
	movs r2, #30
	lsls r1, r1, #1
	movs r0, #5
	bl Func_0200208c
	movs r1, #0
	movs r0, #5
	bl Func_0200207c
	movs r0, #5
	bl Battle_WaitMode0
	movs r2, #5
	movs r0, #5
	movs r1, #0
	bl Func_0200206c
	movs r0, #14
	movs r1, #2
	bl Motion_SetVarCbAndRefresh
	movs r0, #14
	movs r1, #0
	movs r2, #5
	bl Func_0200206c
	movs r1, #176
	movs r0, #17
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r2, #0
	movs r0, #5
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r1, #192
	lsls r1, r1, #8
	movs r0, #6
	bl Func_0200207c
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #13
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	movs r1, #0
	movs r2, #5
	adds r0, #13
	bl Func_0200206c
	movs r0, #13
	bl Func_020001d8
	bl Func_02000550
	movs r0, #10
	bl Battle_WaitMode0
	ldr r3, .L_02008fe8
	ldr r2, .L_02008fec
	str r7, [r3]
	movs r3, #2
	strb r3, [r2]
	movs r0, #150
	bl Battle_WaitMode0
	movs r0, #17
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #17
	bl Func_02002094
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #17
	movs r1, #0
	movs r2, #0
	bl Func_0200206c
	movs r0, #15
	movs r1, #0
	movs r2, #5
	bl Func_0200206c
	movs r1, #128
	movs r2, #128
	movs r0, #15
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r1, #222
	movs r2, #222
	lsls r2, r2, #1
	movs r0, #15
	lsls r1, r1, #2
	bl ObjectMotion_SetPositionAndReset
	movs r1, #160
	lsls r1, r1, #7
	movs r0, #15
	bl Func_0200207c
	movs r0, #5
	bl Battle_WaitMode0
	movs r0, #218
	movs r1, #1
	movs r2, #236
	movs r3, #1
	lsls r2, r2, #17
	negs r1, r1
	lsls r0, r0, #18
	bl Motion_CamBounds
	bl Func_020020ac
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #8
	movs r0, #15
	bl Func_0200207c
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #5
	movs r0, #15
	movs r1, #0
	bl Func_0200206c
	movs r1, #0
	movs r0, #13
	bl Func_0200207c
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #13
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r2, #5
	movs r0, #13
	movs r1, #0
	bl Func_0200206c
	movs r1, #2
	movs r0, #15
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl Battle_WaitMode0
	movs r0, #15
	movs r1, #0
	movs r2, #5
	bl Func_0200206c
	movs r1, #10
	movs r2, #30
	adds r1, #255
	movs r0, #13
	bl Func_0200208c
	movs r1, #192
	lsls r1, r1, #6
	movs r0, #13
	bl Func_0200207c
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #13
	bl Func_020001d8
	movs r0, #153
	bl Func_02002124
	movs r5, #0
	b .L_02008ff2
	.2byte 0x0000
.L_02008fe0:
	.4byte Data_02003024
.L_02008fe4:
	.4byte Data_02003090
.L_02008fe8:
	.4byte Data_02003098
.L_02008fec:
	.4byte Data_0200308c
.L_02008ff0:
	adds r5, #1
.L_02008ff2:
	cmp r5, #119
	bgt .L_0200900c
	movs r0, #1
	bl WaitFrames
	ldr r3, .L_020093b4
	movs r1, #14
	ldr r0, [r3]
	asrs r0, r0, #2
	bl __modsi3
	cmp r0, #0
	bne .L_02008ff0
.L_0200900c:
	bl Func_020004e8
	movs r0, #32
	.ifdef TLA_EDITION_ES
	bl Field_EndActorSpriteEffect
	.else
	.ifdef TLA_EDITION_IT
	bl Field_EndActorSpriteEffect
	.else
	bl Field_BeginPaletteTransition
	.endif
	.endif
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #0
	movs r0, #13
	bl Func_0200207c
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #5
	movs r0, #13
	movs r1, #0
	bl Func_0200206c
	movs r1, #2
	movs r0, #13
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl Battle_WaitMode0
	movs r2, #5
	movs r0, #13
	movs r1, #0
	bl Func_0200206c
	movs r1, #3
	movs r0, #15
	bl Motion_SetModeAndWaitAnimation
	movs r0, #15
	bl Battle_WaitMode0
	movs r1, #128
	movs r2, #0
	movs r0, #14
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r1, #254
	lsls r1, r1, #7
	adds r1, #255
	movs r0, #13
	bl Func_0200207c
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	movs r2, #5
	adds r0, #13
	movs r1, #0
	bl Func_0200206c
	movs r1, #0
	movs r0, #7
	bl Func_0200207c
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #7
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #7
	movs r1, #0
	movs r2, #5
	bl Func_0200206c
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #13
	bl Func_0200208c
	movs r0, #128
	lsls r0, r0, #6
	movs r2, #5
	adds r0, #13
	movs r1, #0
	bl Func_0200206c
	movs r0, #7
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #7
	movs r1, #0
	movs r2, #5
	bl Func_0200206c
	movs r1, #2
	adds r1, #255
	movs r2, #50
	movs r0, #13
	bl Func_0200208c
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #7
	bl Func_0200208c
	movs r0, #7
	movs r1, #0
	movs r2, #5
	bl Func_0200206c
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #14
	bl Func_0200208c
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #50
	movs r0, #13
	bl Func_0200208c
	movs r2, #0
	movs r0, #14
	movs r1, #0
	bl ObjectMotion_ArmCallback
	movs r1, #0
	movs r0, #13
	bl Func_0200207c
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #15
	movs r1, #2
	bl Motion_SetVarCbAndRefresh
	movs r2, #5
	movs r0, #15
	movs r1, #0
	bl Func_0200206c
	movs r1, #2
	movs r0, #17
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl Battle_WaitMode0
	movs r0, #17
	movs r1, #0
	movs r2, #5
	bl Func_0200206c
	movs r1, #128
	movs r0, #7
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	movs r0, #15
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #14
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #13
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #6
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r2, #0
	movs r0, #5
	movs r1, #0
	bl ObjectMotion_ArmCallback
	movs r1, #0
	movs r0, #4
	bl Func_0200207c
	movs r0, #25
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #8
	movs r0, #17
	bl Func_0200207c
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #17
	bl Func_0200208c
	movs r2, #5
	movs r0, #17
	movs r1, #0
	bl Func_0200206c
	movs r1, #176
	lsls r1, r1, #8
	movs r0, #17
	bl Func_0200207c
	movs r0, #15
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #17
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #5
	movs r0, #17
	movs r1, #0
	bl Func_0200206c
	movs r1, #2
	movs r0, #14
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl Battle_WaitMode0
	movs r0, #14
	movs r1, #0
	movs r2, #5
	bl Func_0200206c
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #13
	bl Func_0200208c
	movs r0, #128
	lsls r0, r0, #6
	movs r2, #5
	adds r0, #13
	movs r1, #0
	bl Func_0200206c
	movs r1, #3
	movs r0, #17
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #5
	movs r0, #17
	movs r1, #0
	bl Func_0200206c
	movs r0, #13
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #13
	movs r1, #0
	movs r2, #5
	bl Func_0200206c
	movs r1, #10
	adds r1, #255
	movs r2, #30
	movs r0, #17
	bl Func_0200208c
	movs r2, #5
	movs r0, #17
	movs r1, #0
	bl Func_0200206c
	movs r1, #2
	movs r0, #7
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl Battle_WaitMode0
	movs r0, #7
	movs r1, #0
	movs r2, #5
	bl Func_0200206c
	movs r1, #192
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r2, #0
	movs r0, #5
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r1, #192
	lsls r1, r1, #8
	movs r0, #4
	bl Func_0200207c
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #14
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #14
	movs r1, #0
	movs r2, #5
	bl Func_0200206c
	movs r1, #129
	movs r2, #40
	lsls r1, r1, #1
	movs r0, #15
	bl Func_0200208c
	movs r1, #128
	lsls r1, r1, #8
	movs r0, #15
	bl Func_0200207c
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #5
	movs r0, #15
	movs r1, #0
	bl Func_0200206c
	movs r0, #13
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r1, #0
	movs r0, #13
	bl Func_0200207c
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #13
	movs r1, #0
	movs r2, #5
	bl Func_0200206c
	movs r1, #2
	movs r2, #50
	adds r1, #255
	movs r0, #15
	bl Func_0200208c
	movs r0, #15
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r2, #5
	movs r0, #15
	movs r1, #0
	bl Func_0200206c
	movs r1, #3
	movs r0, #13
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #13
	movs r1, #0
	movs r2, #5
	bl Func_0200206c
	movs r1, #128
	movs r2, #45
	lsls r1, r1, #1
	movs r0, #15
	bl Func_0200208c
	movs r1, #192
	lsls r1, r1, #6
	movs r0, #13
	bl Func_0200207c
	movs r0, #5
	bl Battle_WaitMode0
	movs r2, #5
	movs r0, #13
	movs r1, #0
	bl Func_0200206c
	movs r1, #160
	lsls r1, r1, #7
	movs r0, #15
	bl Func_0200207c
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #4
	adds r1, #255
	movs r2, #30
	movs r0, #15
	bl Func_0200208c
	movs r1, #0
	movs r0, #15
	bl UiText_OpenMessageAtObject
	movs r0, #4
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_020093b8
	movs r0, #15
	bl Battle_WaitMode0
	movs r2, #5
	movs r0, #15
	movs r1, #0
	bl Func_0200206c
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_020093da
	.2byte 0x0000
.L_020093b4:
	.4byte Data_02003098
.L_020093b8:
	movs r0, #35
	bl Battle_WaitMode0
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #15
	adds r3, #1
	strh r3, [r2]
	movs r1, #0
	movs r2, #5
	bl Func_0200206c
.L_020093da:
	movs r1, #128
	lsls r1, r1, #8
	movs r0, #15
	bl Func_0200207c
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #8
	adds r1, #255
	movs r2, #30
	movs r0, #15
	bl Func_0200208c
	movs r2, #5
	movs r0, #15
	movs r1, #0
	bl Func_0200206c
	movs r1, #0
	movs r0, #13
	bl Func_0200207c
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #13
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #13
	movs r1, #0
	movs r2, #5
	bl Func_0200206c
	movs r1, #10
	adds r1, #255
	movs r2, #30
	movs r0, #13
	bl Func_0200208c
	movs r2, #5
	movs r0, #13
	movs r1, #0
	bl Func_0200206c
	movs r0, #15
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #15
	bl ObjectMotion_SetSpeedParameters
	movs r0, #15
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	strb r3, [r0]
	movs r1, #32
	movs r2, #0
	movs r0, #15
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #1
	bl Battle_WaitMode0
	movs r0, #15
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	strb r3, [r0]
	movs r1, #0
	movs r0, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #14
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #13
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #208
	movs r0, #17
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #224
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #224
	movs r2, #0
	movs r0, #5
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r1, #224
	lsls r1, r1, #8
	movs r0, #4
	bl Func_0200207c
	movs r0, #5
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #15
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #5
	movs r0, #15
	movs r1, #0
	bl Func_0200206c
	movs r0, #15
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r2, #5
	movs r0, #15
	movs r1, #0
	bl Func_0200206c
	movs r1, #2
	movs r0, #7
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #5
	movs r0, #7
	movs r1, #0
	bl Func_0200206c
	movs r1, #3
	movs r0, #15
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #15
	movs r1, #0
	movs r2, #5
	bl Func_0200206c
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #4
	bl Func_0200208c
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #5
	bl Func_0200208c
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #6
	bl Func_0200208c
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #17
	bl Func_0200208c
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #60
	movs r0, #7
	bl Func_0200208c
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #15
	bl Func_0200208c
	movs r2, #5
	movs r0, #15
	movs r1, #0
	bl Func_0200206c
	movs r1, #2
	movs r0, #15
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl Battle_WaitMode0
	movs r0, #15
	movs r1, #0
	movs r2, #5
	bl Func_0200206c
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #15
	ldr r1, .L_0200997c
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r1, #50
	movs r2, #24
	movs r0, #15
	negs r1, r1
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #13
	movs r1, #15
	bl Object_LinkObjectAndSetCallback
	movs r0, #14
	movs r1, #15
	bl Object_LinkObjectAndSetCallback
	movs r0, #7
	movs r1, #15
	bl Object_LinkObjectAndSetCallback
	movs r0, #4
	movs r1, #15
	bl Object_LinkObjectAndSetCallback
	movs r0, #5
	movs r1, #15
	bl Object_LinkObjectAndSetCallback
	movs r0, #6
	movs r1, #15
	bl Object_LinkObjectAndSetCallback
	movs r0, #17
	movs r1, #15
	bl Object_LinkObjectAndSetCallback
	movs r0, #15
	movs r1, #1
	bl Object_AttachWorkTargetToObject
	movs r1, #8
	movs r0, #15
	negs r1, r1
	movs r2, #32
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #160
	movs r2, #0
	movs r0, #16
	lsls r1, r1, #7
	bl ObjectMotion_ArmCallback
	movs r0, #4
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	movs r0, #4
	lsls r1, r1, #1
	bl Func_02002094
	movs r0, #5
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #5
	bl Func_02002094
	movs r0, #37
	bl Battle_WaitMode0
	movs r0, #4
	ldr r1, .L_02009980
	ldr r2, .L_02009984
	bl ObjectMotion_SetSpeedParameters
	movs r0, #5
	ldr r1, .L_02009980
	ldr r2, .L_02009984
	bl ObjectMotion_SetSpeedParameters
	movs r1, #8
	movs r0, #5
	negs r1, r1
	movs r2, #16
	bl ObjectMotion_OffsetPositionAndReset
	movs r1, #8
	movs r2, #16
	movs r0, #4
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #5
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r1, #128
	movs r2, #0
	movs r0, #4
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r1, #0
	movs r0, #5
	bl Func_0200207c
	movs r0, #15
	bl Battle_WaitMode0
	movs r0, #4
	movs r1, #15
	bl Object_LinkObjectAndSetCallback
	movs r0, #5
	movs r1, #15
	bl Object_LinkObjectAndSetCallback
	movs r2, #64
	movs r1, #0
	movs r0, #15
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #15
	bl Battle_WaitMode0
	movs r1, #208
	lsls r1, r1, #8
	movs r0, #15
	bl Func_0200207c
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #8
	adds r1, #255
	movs r2, #40
	movs r0, #15
	bl Func_0200208c
	movs r2, #5
	movs r0, #15
	movs r1, #0
	bl Func_0200206c
	movs r1, #160
	lsls r1, r1, #7
	movs r0, #15
	bl Func_0200207c
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #8
	movs r0, #16
	bl Func_0200207c
	movs r0, #15
	bl Battle_WaitMode0
	movs r1, #192
	lsls r1, r1, #6
	movs r0, #16
	bl Func_0200207c
	movs r0, #15
	bl Battle_WaitMode0
	movs r0, #16
	movs r1, #5
	bl Object_SetModeById
	movs r0, #1
	movs r1, #1
	movs r2, #1
	movs r3, #0
	negs r0, r0
	negs r1, r1
	negs r2, r2
	bl Motion_CamBounds
	movs r1, #0
	movs r2, #96
	movs r0, #15
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #15
	movs r1, #0
	movs r2, #0
	bl Func_02002024
	movs r1, #128
	movs r2, #128
	movs r0, #4
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r1, #128
	movs r2, #128
	movs r0, #5
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r2, #16
	movs r0, #5
	movs r1, #8
	negs r2, r2
	bl ObjectMotion_OffsetPositionAndReset
	movs r1, #8
	movs r2, #16
	negs r1, r1
	negs r2, r2
	movs r0, #4
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #5
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r1, #128
	movs r0, #7
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #14
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #13
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #4
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
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
	movs r0, #218
	movs r1, #1
	movs r2, #240
	movs r3, #1
	lsls r2, r2, #17
	negs r1, r1
	lsls r0, r0, #18
	bl Motion_CamBounds
	bl Func_020020ac
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #16
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #16
	movs r1, #1
	bl Object_SetModeById
	movs r1, #176
	lsls r1, r1, #8
	movs r0, #17
	bl Func_0200207c
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #5
	movs r0, #17
	movs r1, #0
	bl Func_0200206c
	movs r0, #13
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #128
	lsls r0, r0, #6
	movs r2, #5
	adds r0, #13
	movs r1, #0
	bl Func_0200206c
	movs r0, #13
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #13
	movs r1, #0
	movs r2, #5
	bl Func_0200206c
	movs r0, #7
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #14
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #6
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r2, #0
	movs r0, #17
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r1, #0
	movs r0, #4
	bl Func_0200207c
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #128
	movs r0, #7
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #14
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	movs r0, #13
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #176
	movs r2, #0
	movs r0, #17
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r1, #192
	lsls r1, r1, #8
	movs r0, #4
	bl Func_0200207c
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #13
	movs r1, #2
	bl Motion_SetVarCbAndRefresh
	movs r0, #128
	lsls r0, r0, #6
	movs r2, #5
	adds r0, #13
	movs r1, #0
	bl Func_0200206c
	movs r0, #6
	movs r1, #2
	bl Motion_SetVarCbAndRefresh
	movs r0, #6
	movs r1, #0
	movs r2, #5
	bl Func_0200206c
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #13
	bl Func_0200208c
	movs r0, #128
	lsls r0, r0, #6
	movs r2, #5
	adds r0, #13
	movs r1, #0
	bl Func_0200206c
	movs r1, #128
	lsls r1, r1, #8
	movs r0, #13
	bl Func_0200207c
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #0
	movs r0, #14
	bl Func_0200207c
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #14
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #192
	movs r2, #0
	movs r0, #13
	lsls r1, r1, #6
	bl ObjectMotion_ArmCallback
	movs r1, #192
	lsls r1, r1, #6
	movs r0, #14
	bl Func_0200207c
	movs r0, #5
	bl Battle_WaitMode0
	movs r0, #14
	movs r1, #24
	movs r2, #26
	bl ObjectMotion_CommitPositionAndActivate
	movs r2, #8
	movs r1, #0
	movs r0, #14
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #14
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #14
	movs r1, #4
	movs r2, #20
	bl ObjectMotion_Launch
	movs r5, #15
	b .L_02009988
	.2byte 0x0000
.L_0200997c:
	.4byte 0x00013333
.L_02009980:
	.4byte 0x00023333
.L_02009984:
	.4byte 0x00011999
.L_02009988:
	movs r0, #64
	bl Object_GetById
	ldr r3, [r0, #28]
	movs r2, #128
	lsls r2, r2, #5
	adds r3, r3, r2
	str r3, [r0, #28]
	subs r5, #1
	movs r0, #1
	bl WaitFrames
	cmp r5, #0
	bge .L_02009988
	movs r0, #64
	bl Object_GetById
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r0, #28]
	movs r0, #5
	bl Battle_WaitMode0
	movs r1, #2
	adds r1, #255
	movs r2, #0
	movs r0, #5
	bl Func_0200208c
	movs r1, #2
	adds r1, #255
	movs r2, #0
	movs r0, #17
	bl Func_0200208c
	movs r1, #2
	adds r1, #255
	movs r2, #0
	movs r0, #6
	bl Func_0200208c
	movs r1, #2
	adds r1, #255
	movs r2, #60
	movs r0, #4
	bl Func_0200208c
	movs r1, #24
	movs r2, #26
	movs r0, #14
	negs r1, r1
	negs r2, r2
	bl ObjectMotion_CommitPositionAndActivate
	movs r2, #8
	negs r2, r2
	movs r0, #14
	movs r1, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #192
	lsls r1, r1, #6
	movs r0, #14
	bl Func_0200207c
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #14
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #14
	movs r1, #0
	movs r2, #5
	bl Func_0200206c
	movs r0, #6
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r2, #0
	movs r0, #17
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r1, #0
	movs r0, #4
	bl Func_0200207c
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #192
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #176
	movs r2, #0
	movs r0, #17
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r1, #192
	lsls r1, r1, #8
	movs r0, #4
	bl Func_0200207c
	movs r0, #15
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #13
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	movs r2, #5
	adds r0, #13
	movs r1, #0
	bl Func_0200206c
	movs r1, #0
	movs r0, #7
	bl Func_0200207c
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #2
	adds r1, #255
	movs r2, #30
	movs r0, #7
	bl Func_0200208c
	movs r2, #5
	movs r0, #7
	movs r1, #0
	bl Func_0200206c
	movs r1, #128
	movs r0, #13
	lsls r1, r1, #8
	bl Func_0200207c
	movs r0, #13
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #13
	movs r1, #0
	movs r2, #5
	bl Func_0200206c
	movs r1, #128
	movs r2, #0
	movs r0, #7
	lsls r1, r1, #6
	bl ObjectMotion_ArmCallback
	movs r1, #192
	lsls r1, r1, #6
	movs r0, #13
	bl Func_0200207c
	movs r0, #5
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	movs r2, #5
	adds r0, #13
	movs r1, #0
	bl Func_0200206c
	movs r1, #3
	movs r0, #17
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #17
	movs r1, #0
	movs r2, #5
	bl Func_0200206c
	movs r2, #0
	movs r0, #14
	movs r1, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	lsls r1, r1, #8
	movs r0, #13
	bl Func_0200207c
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #14
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #13
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #192
	movs r2, #0
	movs r0, #14
	lsls r1, r1, #6
	bl ObjectMotion_ArmCallback
	movs r1, #192
	lsls r1, r1, #6
	movs r0, #13
	bl Func_0200207c
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	movs r2, #5
	adds r0, #13
	movs r1, #0
	bl Func_0200206c
	movs r1, #3
	movs r0, #14
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #14
	movs r1, #0
	movs r2, #5
	bl Func_0200206c
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #13
	bl Func_0200208c
	movs r0, #128
	lsls r0, r0, #6
	movs r2, #5
	adds r0, #13
	movs r1, #0
	bl Func_0200206c
	movs r1, #3
	movs r0, #13
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	movs r2, #5
	adds r0, #13
	movs r1, #0
	bl Func_0200206c
	movs r1, #2
	movs r0, #14
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #14
	movs r1, #0
	movs r2, #5
	bl Func_0200206c
	movs r0, #6
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r2, #0
	movs r0, #17
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r1, #0
	movs r0, #4
	bl Func_0200207c
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #4
	movs r1, #3
	bl Object_SetModeById
	movs r0, #5
	movs r1, #3
	bl Object_SetModeById
	movs r0, #6
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #17
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #192
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #176
	movs r2, #0
	movs r0, #17
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r1, #192
	lsls r1, r1, #8
	movs r0, #4
	bl Func_0200207c
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #13
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	movs r2, #10
	adds r0, #13
	movs r1, #0
	bl Func_0200206c
	movs r1, #208
	lsls r1, r1, #8
	movs r0, #13
	bl Func_0200207c
	movs r0, #5
	bl Battle_WaitMode0
	movs r1, #216
	movs r2, #196
	lsls r2, r2, #1
	movs r0, #13
	lsls r1, r1, #2
	bl ObjectMotion_SetPositionAndReset
	movs r1, #192
	movs r0, #13
	lsls r1, r1, #6
	bl Func_0200207c
	movs r1, #2
	movs r0, #14
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl Battle_WaitMode0
	movs r2, #5
	movs r0, #14
	movs r1, #0
	bl Func_0200206c
	movs r1, #128
	lsls r1, r1, #8
	movs r0, #14
	bl Func_0200207c
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #14
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #5
	movs r0, #14
	movs r1, #0
	bl Func_0200206c
	movs r1, #192
	lsls r1, r1, #6
	movs r0, #14
	bl Func_0200207c
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #14
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #14
	movs r1, #0
	movs r2, #5
	bl Func_0200206c
	movs r1, #128
	movs r0, #17
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #6
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r2, #0
	movs r0, #5
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r1, #0
	movs r0, #4
	bl Func_0200207c
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #4
	movs r1, #3
	bl Object_SetModeById
	movs r0, #5
	movs r1, #3
	bl Object_SetModeById
	movs r0, #6
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #17
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #192
	movs r0, #4
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r2, #153
	lsls r2, r2, #8
	adds r2, #153
	movs r0, #17
	ldr r1, .L_02009f2c
	bl ObjectMotion_SetSpeedParameters
	movs r0, #17
	movs r1, #2
	bl Object_SetModeById
	ldr r3, .L_02009f30
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r3, r2
	ldr r0, [r5]
	bl Object_GetById
	cmp r0, #0
	beq .L_02009dbc
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #17
	bl ObjectMotion_ResetAndSetPosition
.L_02009dbc:
	movs r0, #17
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #17
	movs r1, #0
	movs r2, #0
	bl Func_02002024
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #5
	ldr r1, .L_02009f2c
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r0, #5
	movs r1, #2
	bl Object_SetModeById
	ldr r0, [r5]
	bl Object_GetById
	cmp r0, #0
	beq .L_02009dfa
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #5
	bl ObjectMotion_ResetAndSetPosition
.L_02009dfa:
	movs r0, #5
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl Func_02002024
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #6
	ldr r1, .L_02009f2c
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r0, #6
	movs r1, #2
	bl Object_SetModeById
	ldr r0, [r5]
	bl Object_GetById
	cmp r0, #0
	beq .L_02009e38
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #6
	bl ObjectMotion_ResetAndSetPosition
.L_02009e38:
	movs r0, #6
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r1, #0
	movs r2, #0
	movs r0, #6
	bl Func_02002024
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #32
	movs r2, #48
	movs r0, #4
	negs r1, r1
	negs r2, r2
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #128
	movs r0, #7
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	movs r2, #0
	movs r0, #14
	lsls r1, r1, #7
	bl ObjectMotion_ArmCallback
	movs r1, #192
	lsls r1, r1, #8
	movs r0, #4
	bl Func_0200207c
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #7
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #7
	ldr r1, .L_02009f2c
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r0, #7
	movs r1, #2
	bl Object_SetModeById
	ldr r0, [r5]
	bl Object_GetById
	cmp r0, #0
	beq .L_02009ece
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #7
	bl ObjectMotion_ResetAndSetPosition
.L_02009ece:
	movs r0, #7
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r2, #0
	movs r1, #0
	movs r0, #7
	bl Func_02002024
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #14
	bl Motion_SetModeAndWaitAnimation
	movs r0, #15
	bl Battle_WaitMode0
	movs r1, #210
	movs r2, #204
	lsls r2, r2, #1
	movs r0, #14
	lsls r1, r1, #2
	bl ObjectMotion_SetPositionAndReset
	movs r1, #192
	movs r0, #14
	lsls r1, r1, #6
	bl Func_0200207c
	movs r1, #160
	movs r0, #16
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #10
	bl Battle_WaitMode0
	bl Func_02001fd4
.L_02009f20:
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009f2c:
	.4byte 0x00013333
.L_02009f30:
	.4byte gPartyState
	.section .bss,"aw",%nobits
	.global Data_02003020
Data_02003020:
	.space 0x00000004
	.global Data_02003024
Data_02003024:
	.space 0x0000000c
	.global Data_02003030
Data_02003030:
	.space 0x00000028
	.global Data_02003058
Data_02003058:
	.space 0x00000008
	.global Data_02003060
Data_02003060:
	.space 0x00000028
	.global Data_02003088
Data_02003088:
	.space 0x00000004
	.global Data_0200308c
Data_0200308c:
	.space 0x00000004
	.global Data_02003090
Data_02003090:
	.space 0x00000004
	.global Data_02003094
Data_02003094:
	.space 0x00000004
	.global Data_02003098
Data_02003098:
