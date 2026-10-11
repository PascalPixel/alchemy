.syntax unified
	.thumb
	.section .text.x02008050,"ax",%progbits
	.global Func_02000050
	.thumb_func
Func_02000050:
	push {lr}
	ldr r3, .L_0200806c
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008070
	movs r0, #0
	cmp r2, r3
	bne .L_02008068
	ldr r0, .L_02008074
.L_02008068:
	pop {pc}
	.2byte 0x0000
.L_0200806c:
	.4byte gPartyState
.L_02008070:
	.4byte 0x0000010b
.L_02008074:
	.4byte Data_0200183c
	.section .text.x02008080,"ax",%progbits
	.global Func_02000080
	.thumb_func
Func_02000080:
	push {lr}
	ldr r3, .L_0200809c
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_020080a0
	cmp r2, r3
	bne .L_02008098
	ldr r0, .L_020080a4
	b .L_0200809a
.L_02008098:
	ldr r0, .L_020080a8
.L_0200809a:
	pop {pc}
.L_0200809c:
	.4byte gPartyState
.L_020080a0:
	.4byte 0x0000010b
.L_020080a4:
	.4byte Data_02001a80
.L_020080a8:
	.4byte Data_02001900
	.section .text.x020080ac,"ax",%progbits
	.global Func_020000ac
	.thumb_func
Func_020000ac:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	sub sp, #8
	movs r3, #24
	str r3, [sp, #0]
	mov r8, r3
	movs r5, #7
	movs r0, #69
	movs r1, #70
	movs r2, #10
	movs r3, #12
	str r5, [sp, #4]
	bl Func_020012b0
	mov r3, r8
	str r3, [sp, #0]
	movs r0, #69
	movs r1, #70
	movs r2, #10
	movs r3, #12
	str r5, [sp, #4]
	bl Func_020012b8
	movs r6, #88
	movs r0, #79
	movs r1, #70
	movs r2, #10
	movs r3, #12
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl Func_020012b0
	movs r0, #79
	movs r1, #70
	movs r2, #10
	movs r3, #12
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl Func_020012b8
	mov r3, r8
	str r3, [sp, #0]
	movs r5, #71
	movs r0, #69
	movs r1, #82
	movs r2, #10
	movs r3, #12
	str r5, [sp, #4]
	bl Func_020012b0
	mov r3, r8
	str r3, [sp, #0]
	movs r0, #69
	movs r1, #82
	movs r2, #10
	movs r3, #12
	str r5, [sp, #4]
	bl Func_020012b8
	add sp, #8
	pop {r3}
	mov r8, r3
	pop {r5, r6, pc}
	.section .text.x0200812c,"ax",%progbits
	.global Func_0200012c
	.thumb_func
Func_0200012c:
	push {r5, r6, lr}
	ldr r5, .L_02008174
	adds r6, r0, #0
	adds r0, r5, #0
	bl Func_02001380
	movs r1, #0
	adds r0, r6, #0
	bl UiText_OpenMessageAtObject
	bl Func_02001420
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_0200815c
	movs r0, #10
	bl Battle_WaitMode0
	adds r0, r5, #1
	bl Func_02001380
	b .L_02008168
.L_0200815c:
	movs r0, #20
	bl Battle_WaitMode0
	adds r0, r5, #2
	bl Func_02001380
.L_02008168:
	adds r0, r6, #0
	movs r1, #0
	bl Func_02001390
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02008174:
	.4byte MsgScene6A6No23
	.section .text.x02008178,"ax",%progbits
	.global Func_02000178
	.thumb_func
Func_02000178:
	push {lr}
	ldr r0, .L_02008194
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r1, #173
	lsls r1, r1, #1
	adds r2, r3, r1
	movs r3, #1
	strh r3, [r2]
	pop {pc}
.L_02008194:
	.4byte MsgScene6A6No27
	.section .text.x02008198,"ax",%progbits
	.global Func_02000198
	.thumb_func
Func_02000198:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r0, #4
	sub sp, #4
	bl Object_GetById
	adds r5, r0, #0
	bl Func_020012e0
	movs r0, #0
	bl Func_020013f8
	b .L_020081ec
.L_020081b6:
	cmp r1, #0
	beq .L_020081dc
	cmp r6, #0
	beq .L_020081ce
	movs r1, #216
	movs r2, #173
	movs r0, #4
	adds r1, #255
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	b .L_020081dc
.L_020081ce:
	movs r1, #253
	movs r2, #173
	movs r0, #4
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
.L_020081dc:
	cmp r6, #0
	beq .L_020081e2
	b .L_02008982
.L_020081e2:
	movs r1, #253
	movs r2, #118
	movs r0, #4
	lsls r1, r1, #1
	b .L_0200898a
.L_020081ec:
	ldr r3, [r5, #8]
	ldr r0, .L_02008208
	movs r6, #0
	movs r1, #0
	movs r2, #0
	cmp r3, r0
	bgt .L_020081fc
	movs r6, #1
.L_020081fc:
	ldr r0, [r5, #16]
	ldr r3, .L_0200820c
	cmp r0, r3
	bgt .L_02008210
	movs r1, #1
	b .L_0200821a
.L_02008208:
	.4byte 0x01e8ffff
.L_0200820c:
	.4byte 0x015affff
.L_02008210:
	movs r3, #185
	lsls r3, r3, #17
	cmp r0, r3
	ble .L_0200821a
	movs r2, #1
.L_0200821a:
	cmp r2, #0
	beq .L_020081b6
.L_0200821e:
	movs r0, #244
	movs r1, #1
	movs r2, #186
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #17
	lsls r0, r0, #17
	bl Motion_CamBounds
	movs r0, #150
	lsls r0, r0, #4
	adds r0, #255
	bl GameFlag_SetBit
	movs r1, #232
	movs r2, #122
	adds r2, #255
	movs r0, #4
	adds r1, #255
	bl ObjectMotion_SetPositionAndCommit
	movs r1, #192
	lsls r1, r1, #8
	movs r0, #4
	bl Func_020013a0
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #140
	bl Func_02001448
	movs r0, #16
	movs r1, #2
	bl ObjectMotion_SetVariantCallback
	movs r0, #18
	movs r1, #2
	bl ObjectMotion_SetVariantCallback
	movs r0, #17
	movs r1, #2
	bl Motion_SetVarCbAndRefresh
	movs r0, #16
	movs r1, #4
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #17
	movs r1, #4
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r1, #160
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #18
	bl ObjectMotion_ArmCallback
	movs r0, #25
	bl Battle_WaitMode0
	movs r0, #4
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	movs r0, #4
	lsls r1, r1, #1
	bl Func_020013b0
	movs r2, #12
	movs r0, #4
	movs r1, #24
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #192
	lsls r1, r1, #8
	movs r0, #4
	bl Func_020013a0
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #144
	bl Func_02001448
	movs r1, #230
	movs r0, #21
	lsls r1, r1, #1
	bl Func_02001428
	movs r1, #244
	movs r2, #165
	movs r0, #21
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_02001348
	movs r0, #20
	movs r1, #0
	movs r2, #8
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r1, #192
	movs r2, #192
	movs r0, #21
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl ObjectMotion_SetSpeedParameters
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r0, r0, #10
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl Func_020012c8
	movs r1, #244
	movs r2, #150
	movs r0, #21
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl ObjectMotion_ResetAndSetPosition
	movs r0, #244
	movs r1, #1
	movs r2, #154
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #17
	lsls r0, r0, #17
	bl Motion_CamBounds
	movs r0, #21
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #21
	movs r1, #0
	movs r2, #0
	bl Func_02001348
	movs r0, #224
	movs r1, #224
	movs r2, #128
	lsls r2, r2, #9
	lsls r1, r1, #11
	lsls r0, r0, #11
	bl Func_020012c8
	movs r0, #192
	lsls r0, r0, #3
	bl Runtime_BumpAllocateAlternatePool
	adds r7, r0, #0
	movs r0, #128
	lsls r0, r0, #5
	bl MemAlloc
	adds r5, r0, #0
	ldr r0, .L_02008424
	bl Resource_GetTableEntry
	adds r1, r5, #0
	bl Func_02001248
	bl Resource_FindFreeEntry
	movs r1, #128
	adds r2, r5, #0
	lsls r1, r1, #3
	mov r8, r0
	bl VramBlock_LoadCached
	mov r10, r0
	adds r0, r5, #0
	bl MemFree
	adds r5, r7, #0
	adds r5, #12
	movs r6, #95
.L_0200838c:
	bl Random16Far
	lsls r0, r0, #5
	lsrs r0, r0, #16
	negs r0, r0
	subs r6, #1
	str r0, [r5]
	adds r5, #16
	cmp r6, #0
	bge .L_0200838c
	movs r3, #128
	movs r2, #128
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	ldr r0, .L_02008428
	ldr r1, .L_0200842c
	adds r2, #16
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r6, #0
.L_020083b6:
	movs r3, #31
	ands r3, r6
	cmp r3, #0
	bne .L_020083c4
	movs r0, #145
	bl Func_02001448
.L_020083c4:
	cmp r6, #30
	bne .L_020083d4
	movs r0, #90
	bl Func_02001278
	movs r0, #141
	bl Func_02001448
.L_020083d4:
	adds r5, r7, #0
.L_020083d6:
	ldr r2, [r5, #12]
	cmp r2, #0
	bne .L_02008430
	movs r3, #128
	lsls r3, r3, #23
	str r3, [r5, #4]
	str r2, [r5, #8]
	bl Random16Far
	lsls r3, r0, #1
	adds r3, r3, r0
	ldr r2, .L_0200841c
	lsls r3, r3, #5
	lsrs r3, r3, #16
	adds r3, #72
	ands r3, r2
	ldr r1, .L_02008420
	ldrh r2, [r5, #6]
	ands r2, r1
	orrs r2, r3
	strh r2, [r5, #6]
	bl Random16Far
	ldrb r3, [r5, #9]
	lsrs r0, r0, #10
	strb r0, [r5, #4]
	movs r0, #13
	movs r2, #240
	negs r0, r0
	orrs r3, r2
	adds r2, r0, #0
	ands r3, r2
	strb r3, [r5, #9]
	b .L_02008430
	.2byte 0x0000
.L_0200841c:
	.4byte 0x000001ff
.L_02008420:
	.4byte 0xfffffe00
.L_02008424:
	.4byte 0x000001e8
.L_02008428:
	.4byte Data_02001450
.L_0200842c:
	.4byte 0x050003e0
.L_02008430:
	ldr r3, [r5, #12]
	cmp r3, #0
	blt .L_02008452
	ldr r2, .L_0200846c
	asrs r3, r3, #2
	lsls r3, r3, #2
	add r3, r10
	ands r3, r2
	ldr r1, .L_02008470
	ldrh r2, [r5, #8]
	adds r0, r5, #0
	ands r2, r1
	orrs r2, r3
	strh r2, [r5, #8]
	movs r1, #100
	bl Func_02001268
.L_02008452:
	ldr r3, [r5, #12]
	adds r3, #1
	str r3, [r5, #12]
	cmp r3, #31
	ble .L_02008474
	bl Random16Far
	lsls r3, r0, #3
	subs r3, r3, r0
	lsrs r3, r3, #16
	negs r3, r3
	str r3, [r5, #12]
	b .L_02008474
.L_0200846c:
	.4byte 0x000003ff
.L_02008470:
	.4byte 0xfffffc00
.L_02008474:
	movs r1, #190
	lsls r1, r1, #3
	adds r5, #16
	adds r3, r7, r1
	cmp r5, r3
	ble .L_020083d6
	movs r0, #1
	adds r6, #1
	bl WaitFrames
	cmp r6, #119
	ble .L_020083b6
	mov r0, r8
	bl Resource_ResetEntry
	adds r0, r7, #0
	bl MemFree
	movs r0, #244
	movs r1, #1
	movs r2, #195
	lsls r0, r0, #17
	negs r1, r1
	lsls r2, r2, #17
	movs r3, #1
	bl Motion_CamBounds
	movs r1, #248
	movs r2, #240
	movs r3, #0
	movs r0, #14
	lsls r1, r1, #16
	lsls r2, r2, #17
	bl Func_02001350
	bl Func_020000ac
	movs r2, #128
	lsls r2, r2, #9
	movs r1, #0
	movs r0, #0
	bl Func_020012c8
	movs r0, #195
	lsls r0, r0, #1
	bl Func_02001448
	movs r0, #180
	bl Battle_WaitMode0
	movs r0, #120
	bl Func_02001280
	ldr r0, .L_020088dc
	bl Func_02001380
	movs r0, #120
	bl Battle_WaitMode0
	movs r0, #18
	movs r1, #2
	bl Motion_SetVarCbAndRefresh
	movs r1, #128
	movs r0, #18
	lsls r1, r1, #8
	bl Func_020013a0
	movs r0, #18
	movs r1, #0
	bl Func_02001390
	movs r1, #128
	movs r2, #0
	movs r0, #17
	lsls r1, r1, #6
	bl ObjectMotion_ArmCallback
	movs r0, #17
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #16
	bl Object_SetModeById
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #129
	movs r2, #35
	lsls r1, r1, #1
	movs r0, #17
	bl Func_020013a8
	movs r0, #17
	movs r1, #0
	bl Func_02001390
	movs r1, #8
	adds r1, #255
	movs r2, #35
	movs r0, #16
	bl Func_020013a8
	movs r1, #12
	movs r2, #24
	movs r0, #16
	negs r1, r1
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #16
	movs r1, #2
	bl Motion_SetVarCbAndRefresh
	movs r0, #16
	movs r1, #0
	bl Func_02001390
	movs r0, #4
	movs r1, #3
	bl Object_SetModeById
	movs r1, #176
	lsls r1, r1, #8
	movs r0, #18
	bl Func_020013a0
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #128
	movs r2, #35
	lsls r1, r1, #1
	movs r0, #18
	bl Func_020013a8
	movs r0, #18
	movs r1, #0
	bl Func_02001390
	movs r1, #2
	adds r1, #255
	movs r2, #0
	movs r0, #16
	bl Func_020013a8
	movs r1, #2
	adds r1, #255
	movs r2, #35
	movs r0, #17
	bl Func_020013a8
	movs r0, #16
	movs r1, #18
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r1, #128
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #17
	bl ObjectMotion_ArmCallback
	movs r0, #60
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #18
	bl ObjectMotion_ArmCallback
	movs r0, #60
	bl Battle_WaitMode0
	movs r1, #6
	adds r1, #255
	movs r2, #50
	movs r0, #18
	bl Func_020013a8
	movs r1, #129
	movs r2, #35
	lsls r1, r1, #1
	movs r0, #18
	bl Func_020013a8
	movs r0, #18
	movs r1, #0
	bl Func_02001390
	movs r1, #17
	movs r2, #0
	movs r0, #16
	bl Object_LinkPair
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #192
	movs r0, #16
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #17
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #176
	movs r0, #18
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #244
	movs r1, #1
	movs r2, #163
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #17
	lsls r0, r0, #17
	bl Motion_CamBounds
	movs r0, #60
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #16
	bl Func_020013a8
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #50
	movs r0, #17
	bl Func_020013a8
	movs r1, #129
	movs r2, #35
	lsls r1, r1, #1
	movs r0, #17
	bl Func_020013a8
	movs r0, #17
	movs r1, #0
	bl Func_02001390
	movs r0, #16
	movs r1, #3
	bl Object_SetModeById
	movs r0, #16
	movs r1, #0
	bl Func_02001390
	movs r0, #18
	movs r1, #4
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r1, #10
	adds r1, #255
	movs r2, #35
	movs r0, #18
	bl Func_020013a8
	movs r0, #244
	movs r1, #1
	movs r2, #183
	movs r3, #1
	lsls r0, r0, #17
	negs r1, r1
	lsls r2, r2, #17
	bl Motion_CamBounds
	bl Func_020013c8
	movs r1, #160
	movs r2, #0
	movs r0, #18
	lsls r1, r1, #7
	bl ObjectMotion_ArmCallback
	movs r0, #18
	movs r1, #0
	bl Func_02001390
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #17
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r2, #16
	movs r1, #0
	movs r0, #17
	bl ObjectMotion_OffsetPositionAndReset
	movs r0, #17
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r1, #128
	movs r0, #17
	lsls r1, r1, #6
	bl Func_020013a0
	movs r0, #17
	movs r1, #4
	bl Object_SetModeById
	movs r0, #17
	movs r1, #0
	bl Func_02001390
	movs r0, #16
	movs r1, #3
	bl Object_SetModeById
	movs r0, #16
	movs r1, #0
	bl Func_02001390
	movs r0, #18
	movs r1, #3
	bl Object_SetModeById
	movs r1, #192
	movs r2, #0
	movs r0, #17
	lsls r1, r1, #6
	bl ObjectMotion_ArmCallback
	movs r0, #17
	movs r1, #0
	bl Func_02001390
	movs r1, #129
	movs r2, #35
	lsls r1, r1, #1
	movs r0, #4
	bl Func_020013a8
	movs r0, #60
	bl Battle_WaitMode0
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #7
	movs r0, #16
	bl Func_020013a0
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #16
	movs r1, #2
	bl Motion_SetVarCbAndRefresh
	movs r0, #16
	movs r1, #0
	bl Func_02001390
	movs r1, #6
	adds r1, #255
	movs r2, #0
	movs r0, #18
	bl Func_020013a8
	movs r1, #6
	adds r1, #255
	movs r2, #0
	movs r0, #17
	bl Func_020013a8
	movs r1, #6
	adds r1, #255
	movs r2, #60
	movs r0, #16
	bl Func_020013a8
	movs r1, #131
	movs r2, #35
	lsls r1, r1, #1
	movs r0, #18
	bl Func_020013a8
	movs r0, #18
	movs r1, #0
	bl Func_02001390
	movs r1, #3
	movs r0, #17
	bl Object_SetModeById
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #131
	movs r2, #35
	lsls r1, r1, #1
	movs r0, #17
	bl Func_020013a8
	movs r0, #17
	movs r1, #0
	bl Func_02001390
	movs r1, #3
	movs r0, #16
	bl Object_SetModeById
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #8
	lsls r1, r1, #5
	bl Func_020013b8
	movs r1, #1
	movs r2, #128
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #17
	ldr r0, .L_020088e0
	bl Motion_CamBounds
	movs r0, #16
	bl Object_GetById
	movs r5, #0
	adds r0, #85
	movs r1, #192
	movs r2, #192
	lsls r2, r2, #8
	strb r5, [r0]
	lsls r1, r1, #9
	movs r0, #16
	bl ObjectMotion_SetSpeedParameters
	ldr r1, .L_020088e4
	movs r0, #16
	bl ObjectMotion_EnableActionAndSetCallback
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #7
	movs r0, #18
	lsls r1, r1, #8
	bl ObjectMotion_SetSpeedParameters
	ldr r1, .L_020088e8
	movs r0, #18
	bl ObjectMotion_EnableActionAndSetCallback
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #7
	lsls r2, r2, #6
	adds r2, #51
	movs r0, #17
	adds r1, #102
	bl ObjectMotion_SetSpeedParameters
	ldr r1, .L_020088ec
	movs r0, #17
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #16
	bl Object_RefreshSelectorById
	movs r1, #192
	lsls r1, r1, #7
	movs r0, #16
	bl Func_020013a0
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #8
	movs r0, #16
	bl Func_020013a0
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #160
	lsls r1, r1, #8
	movs r0, #16
	bl Func_020013a0
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #224
	lsls r1, r1, #8
	movs r0, #16
	bl Func_020013a0
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #0
	movs r0, #16
	bl Func_020013a0
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #6
	movs r0, #16
	bl Func_020013a0
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #160
	lsls r1, r1, #7
	movs r0, #16
	bl Func_020013a0
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #16
	bl Object_SetModeById
	movs r0, #60
	bl Battle_WaitMode0
	movs r1, #0
	movs r0, #16
	bl Func_02001390
	movs r0, #17
	bl Object_RefreshSelectorById
	movs r0, #18
	bl Object_RefreshSelectorById
	movs r2, #0
	movs r1, #18
	movs r0, #17
	bl Object_LinkPair
	movs r0, #20
	bl Battle_WaitMode0
	b .L_020088f0
	.2byte 0x0000
.L_020088dc:
	.4byte MsgScene6A6No28
.L_020088e0:
	.4byte 0x021e0000
.L_020088e4:
	.4byte Data_02001470
.L_020088e8:
	.4byte Data_0200151c
.L_020088ec:
	.4byte Data_0200154c
.L_020088f0:
	movs r0, #18
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #17
	bl Object_SetModeById
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #17
	movs r1, #4
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #8
	movs r0, #16
	lsls r1, r1, #9
	bl ObjectMotion_SetSpeedParameters
	ldr r1, .L_02008be0
	movs r0, #16
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #230
	movs r1, #228
	lsls r0, r0, #9
	lsls r1, r1, #6
	adds r0, #204
	adds r1, #153
	bl Func_020013b8
	movs r1, #1
	movs r2, #175
	ldr r0, .L_02008be4
	negs r1, r1
	lsls r2, r2, #17
	movs r3, #1
	bl Motion_CamBounds
	bl Func_020013c8
	movs r1, #0
	movs r0, #17
	bl UiText_OpenMessageAtObject
	movs r0, #4
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_02008992
	movs r0, #18
	movs r1, #3
	bl Object_SetModeById
	movs r0, #18
	movs r1, #0
	bl Func_02001390
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_020089c4
.L_02008982:
	movs r1, #216
	movs r2, #118
	movs r0, #4
	adds r1, #255
.L_0200898a:
	adds r2, #255
	bl ObjectMotion_SetPositionAndReset
	b .L_0200821e
.L_02008992:
	movs r0, #18
	movs r1, #4
	bl Object_SetModeById
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r0, #226
	lsls r0, r0, #1
	adds r2, r2, r0
	ldrh r3, [r2]
	movs r0, #17
	adds r3, #1
	strh r3, [r2]
	movs r1, #0
	bl Func_02001390
	movs r0, #18
	movs r1, #3
	bl Object_SetModeById
	movs r0, #18
	movs r1, #0
	bl Func_02001390
.L_020089c4:
	movs r0, #18
	movs r1, #0
	movs r2, #24
	bl ObjectMotion_OffsetPositionAndReset
	movs r0, #17
	movs r1, #0
	movs r2, #24
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #17
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #208
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #18
	bl ObjectMotion_ArmCallback
	movs r0, #16
	bl Object_RefreshSelectorById
	ldr r1, .L_02008be8
	movs r0, #16
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #16
	bl Object_RefreshSelectorById
	movs r1, #192
	movs r0, #18
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #17
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #16
	bl ObjectMotion_ArmCallback
	movs r0, #50
	bl Battle_WaitMode0
	movs r0, #18
	movs r1, #3
	bl Object_SetModeById
	movs r0, #17
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #16
	bl Object_SetModeById
	movs r0, #60
	bl Battle_WaitMode0
	movs r0, #4
	movs r1, #16
	bl Object_LinkObjectAndSetCallback
	movs r1, #252
	movs r2, #185
	movs r0, #18
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r1, #252
	movs r2, #176
	lsls r2, r2, #1
	movs r0, #17
	lsls r1, r1, #1
	bl ObjectMotion_ResetAndSetPositionInMode2
	ldr r1, .L_02008bec
	movs r0, #16
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #17
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #18
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #16
	bl Object_RefreshSelectorById
	movs r0, #16
	movs r1, #0
	bl Func_020013a0
	movs r1, #152
	movs r2, #152
	lsls r1, r1, #7
	lsls r2, r2, #6
	movs r0, #16
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #152
	movs r2, #152
	lsls r1, r1, #7
	lsls r2, r2, #6
	movs r0, #17
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #152
	movs r2, #152
	lsls r1, r1, #7
	lsls r2, r2, #6
	adds r1, #204
	adds r2, #102
	movs r0, #18
	bl ObjectMotion_SetSpeedParameters
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #200
	movs r2, #192
	lsls r1, r1, #5
	lsls r2, r2, #4
	adds r2, #204
	adds r1, #153
	movs r0, #20
	bl ObjectMotion_SetSpeedParameters
	movs r0, #20
	bl Object_GetById
	movs r3, #0
	adds r0, #85
	strb r3, [r0]
	ldr r1, .L_02008bf0
	movs r0, #20
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #20
	bl Object_RefreshSelectorById
	movs r1, #152
	movs r2, #152
	lsls r1, r1, #7
	lsls r2, r2, #6
	adds r2, #102
	movs r0, #20
	adds r1, #204
	bl ObjectMotion_SetSpeedParameters
	movs r0, #152
	movs r1, #144
	lsls r0, r0, #7
	lsls r1, r1, #4
	adds r0, #204
	adds r1, #153
	bl Func_020013b8
	movs r0, #230
	movs r1, #1
	movs r2, #175
	negs r1, r1
	lsls r2, r2, #17
	movs r3, #1
	lsls r0, r0, #17
	bl Motion_CamBounds
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #16
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	movs r1, #40
	movs r2, #20
	strb r3, [r0]
	negs r1, r1
	movs r0, #18
	negs r2, r2
	bl ObjectMotion_OffsetPositionAndReset
	movs r1, #40
	movs r2, #20
	movs r0, #17
	negs r1, r1
	negs r2, r2
	bl ObjectMotion_OffsetPositionAndReset
	movs r1, #40
	movs r2, #20
	movs r0, #16
	negs r1, r1
	negs r2, r2
	bl ObjectMotion_OffsetPositionAndReset
	movs r1, #40
	movs r2, #20
	movs r0, #20
	negs r1, r1
	negs r2, r2
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #138
	movs r0, #18
	negs r1, r1
	movs r2, #0
	bl ObjectMotion_OffsetPositionAndReset
	movs r1, #138
	movs r0, #17
	negs r1, r1
	movs r2, #0
	bl ObjectMotion_OffsetPositionAndReset
	movs r1, #138
	movs r0, #16
	negs r1, r1
	movs r2, #0
	bl ObjectMotion_OffsetPositionAndReset
	movs r1, #138
	movs r0, #20
	negs r1, r1
	movs r2, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #16
	movs r1, #0
	movs r2, #0
	bl Func_02001348
	movs r0, #18
	movs r1, #0
	movs r2, #0
	bl Func_02001348
	movs r0, #17
	movs r1, #0
	movs r2, #0
	bl Func_02001348
	movs r0, #20
	movs r1, #0
	movs r2, #0
	bl Func_02001348
	bl Func_020012e8
	add sp, #4
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008be0:
	.4byte Data_0200158c
.L_02008be4:
	.4byte 0x021e0000
.L_02008be8:
	.4byte Data_020015e0
.L_02008bec:
	.4byte Data_02001638
.L_02008bf0:
	.4byte Data_0200166c
	.section .text.x02008bf4,"ax",%progbits
	.global Func_02000bf4
	.thumb_func
Func_02000bf4:
	push {lr}
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #10
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008c0c
	ldr r0, .L_02008c1c
	bl Func_02001380
	b .L_02008c12
.L_02008c0c:
	ldr r0, .L_02008c20
	bl Func_02001380
.L_02008c12:
	movs r0, #11
	movs r1, #0
	bl Func_02001390
	pop {pc}
.L_02008c1c:
	.4byte MsgScene6A6No32
.L_02008c20:
	.4byte MsgScene6A6No55
	.section .text.x02008c24,"ax",%progbits
	.global Func_02000c24
	.thumb_func
Func_02000c24:
	push {lr}
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #10
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008c3c
	ldr r0, .L_02008c4c
	bl Func_02001380
	b .L_02008c42
.L_02008c3c:
	ldr r0, .L_02008c50
	bl Func_02001380
.L_02008c42:
	movs r0, #11
	movs r1, #0
	bl Func_02001390
	pop {pc}
.L_02008c4c:
	.4byte MsgScene6A6No36
.L_02008c50:
	.4byte MsgScene6A6No56
	.section .text.x02008c84,"ax",%progbits
	.global Func_02000c84
	.thumb_func
Func_02000c84:
	push {lr}
	movs r0, #129
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
	movs r0, #22
	bl Object_GetById
	adds r0, #89
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	strb r3, [r0]
	movs r0, #240
	lsls r0, r0, #4
	adds r0, #247
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008cbc
	movs r1, #130
	movs r2, #136
	movs r0, #66
	lsls r1, r1, #18
	lsls r2, r2, #16
	bl Func_02001348
.L_02008cbc:
	pop {pc}
	.2byte 0x0000
	.section .text.x02008cc0,"ax",%progbits
	.global Func_02000cc0
	.thumb_func
Func_02000cc0:
	push {lr}
	ldr r3, .L_02008ce8
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008cec
	cmp r2, r3
	bne .L_02008cd8
	ldr r0, .L_02008cf0
	b .L_02008ce4
.L_02008cd8:
	ldr r3, .L_02008cf4
	cmp r2, r3
	bne .L_02008ce2
	ldr r0, .L_02008cf8
	b .L_02008ce4
.L_02008ce2:
	ldr r0, .L_02008cfc
.L_02008ce4:
	pop {pc}
	.2byte 0x0000
.L_02008ce8:
	.4byte gPartyState
.L_02008cec:
	.4byte 0x0000010a
.L_02008cf0:
	.4byte Data_02001aec
.L_02008cf4:
	.4byte 0x0000010b
.L_02008cf8:
	.4byte Data_02001dd4
.L_02008cfc:
	.4byte Data_02001ae0
	.section .text.x02008d00,"ax",%progbits
	.global Func_02000d00
	.thumb_func
Func_02000d00:
	push {lr}
	ldr r3, .L_02008d34
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	ldrh r3, [r0, #6]
	movs r2, #128
	lsls r2, r2, #6
	adds r3, r3, r2
	ldr r2, .L_02008d30
	ands r3, r2
	movs r2, #192
	lsls r3, r3, #16
	lsls r2, r2, #24
	cmp r3, r2
	bne .L_02008d3c
	ldr r0, .L_02008d38
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
	b .L_02008d3c
.L_02008d30:
	.4byte 0xffffc000
.L_02008d34:
	.4byte gPartyState
.L_02008d38:
	.4byte MsgScene6A6No57
.L_02008d3c:
	pop {pc}
	.2byte 0x0000
	.section .text.x02008d4c,"ax",%progbits
	.global Func_02000d4c
	.thumb_func
Func_02000d4c:
	push {lr}
	ldr r3, .L_02008d98
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	movs r0, #200
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008d72
	movs r0, #30
	movs r1, #8
	bl Func_02001430
	b .L_02008d96
.L_02008d72:
	movs r0, #150
	lsls r0, r0, #4
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008d88
	ldr r0, .L_02008d9c
	bl Func_02001380
	b .L_02008d8e
.L_02008d88:
	ldr r0, .L_02008da0
	bl Func_02001380
.L_02008d8e:
	movs r0, #8
	movs r1, #0
	bl Func_02001390
.L_02008d96:
	pop {pc}
.L_02008d98:
	.4byte gPartyState
.L_02008d9c:
	.4byte MsgScene6A6No51
.L_02008da0:
	.4byte MsgScene6A6No17
	.section .text.x02008da4,"ax",%progbits
	.global Func_02000da4
	.thumb_func
Func_02000da4:
	push {r5, lr}
	ldr r3, .L_02008df0
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	adds r5, r0, #0
	ldr r0, [r3]
	bl Object_GetById
	ldrh r3, [r0, #6]
	movs r2, #128
	lsls r2, r2, #6
	adds r3, r3, r2
	ldr r2, .L_02008dec
	ands r3, r2
	movs r2, #192
	lsls r3, r3, #16
	lsls r2, r2, #24
	cmp r3, r2
	bne .L_02008dd4
	adds r0, r5, #0
	bl Func_02001438
	b .L_02008e06
.L_02008dd4:
	movs r0, #150
	lsls r0, r0, #4
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008df8
	ldr r0, .L_02008df4
	bl Func_02001380
	b .L_02008dfe
	.2byte 0x0000
.L_02008dec:
	.4byte 0xffffc000
.L_02008df0:
	.4byte gPartyState
.L_02008df4:
	.4byte MsgScene6A6No53
.L_02008df8:
	ldr r0, .L_02008e08
	bl Func_02001380
.L_02008dfe:
	adds r0, r5, #0
	movs r1, #0
	bl Func_02001390
.L_02008e06:
	pop {r5, pc}
.L_02008e08:
	.4byte MsgScene6A6No19
	.section .text.x02008e54,"ax",%progbits
	.global Func_02000e54
	.thumb_func
Func_02000e54:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r6, [r3, #108]
	bl Func_020012e0
	movs r0, #0
	bl Func_020013f8
	movs r5, #8
.L_02008e68:
	adds r0, r5, #0
	bl Object_GetById
	cmp r0, #0
	beq .L_02008e7a
	adds r2, r0, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
.L_02008e7a:
	adds r5, #1
	cmp r5, #63
	bls .L_02008e68
	movs r3, #170
	lsls r3, r3, #1
	movs r0, #158
	adds r6, r6, r3
	bl Func_02001448
	ldr r3, .L_02008ee4
	ldrh r2, [r3, #6]
	ldrh r1, [r3, #4]
	ldr r0, [r3]
	bl Func_020012a0
	ldr r5, .L_02008ee8
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	ldr r0, [r5]
	movs r1, #2
	bl Object_SetModeById
	movs r1, #128
	movs r2, #128
	ldr r0, [r5]
	lsls r1, r1, #8
	lsls r2, r2, #7
	bl ObjectMotion_SetSpeedParameters
	movs r2, #4
	movs r1, #0
	negs r2, r2
	ldr r0, [r5]
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r0, #123
	bl Func_02001448
	movs r0, #6
	bl Battle_WaitMode0
	movs r3, #0
	ldrsh r0, [r6, r3]
	bl Func_020013d0
	bl Event_FinishScreenTransition
	bl Event_WaitTransitionFrames
	bl Func_020012e8
	pop {r5, r6, pc}
.L_02008ee4:
	.4byte Data_02001f14
.L_02008ee8:
	.4byte gPartyState
	.section .text.x02008eec,"ax",%progbits
	.global Func_02000eec
	.thumb_func
Func_02000eec:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, .L_02008f34
	ldr r6, [r3, #108]
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	ldr r0, [r5]
	movs r1, #2
	bl Object_SetModeById
	movs r1, #128
	movs r2, #128
	ldr r0, [r5]
	lsls r1, r1, #8
	lsls r2, r2, #7
	bl ObjectMotion_SetSpeedParameters
	movs r0, #123
	bl Func_02001448
	movs r2, #4
	ldr r0, [r5]
	movs r1, #0
	negs r2, r2
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r3, #170
	lsls r3, r3, #1
	adds r6, r6, r3
	movs r3, #0
	ldrsh r0, [r6, r3]
	bl Func_020013d0
	pop {r5, r6, pc}
.L_02008f34:
	.4byte gPartyState
	.section .text.x02008f38,"ax",%progbits
	.global Func_02000f38
	.thumb_func
Func_02000f38:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #170
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r5, [r3, r2]
	sub sp, #8
	cmp r5, #2
	bne .L_02008f60
	movs r1, #144
	movs r2, #224
	movs r0, #8
	lsls r1, r1, #16
	lsls r2, r2, #16
	bl Func_02001348
	b .L_02008f6e
.L_02008f60:
	movs r1, #224
	movs r2, #224
	movs r0, #8
	lsls r1, r1, #16
	lsls r2, r2, #16
	bl Func_02001348
.L_02008f6e:
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #4
	bl ObjectMotion_ArmCallback
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #4
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	strb r3, [r0]
	movs r2, #1
	movs r3, #2
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #14
	movs r0, #96
	movs r1, #14
	movs r2, #72
	bl Func_020012a8
	movs r2, #4
	movs r1, #0
	movs r0, #4
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r0, #4
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #4
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	movs r0, #4
	movs r1, #13
	bl Object_SetModeById
	movs r2, #16
	movs r1, #0
	movs r0, #4
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r0, #4
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r1, #10
	movs r0, #4
	bl Object_SetModeById
	movs r0, #14
	bl Battle_WaitMode0
	movs r0, #123
	bl Func_02001448
	adds r0, r5, #0
	bl Func_020013d0
	bl Func_020012e8
	add sp, #8
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x020090c0,"ax",%progbits
	.global Func_020010c0
	.thumb_func
Func_020010c0:
	push {r5, lr}
	ldr r3, .L_020091e0
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_020091e4
	cmp r2, r3
	bne .L_0200911a
	movs r0, #192
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020090e8
	movs r0, #64
	movs r1, #0
	bl Object_SetWideSprite
.L_020090e8:
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #1
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020090fe
	movs r0, #65
	movs r1, #1
	bl Object_SetWideSprite
.L_020090fe:
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #2
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02009114
	movs r0, #66
	movs r1, #1
	bl Object_SetWideSprite
.L_02009114:
	bl Func_02001080
	b .L_0200911e
.L_0200911a:
	bl Func_020010a0
.L_0200911e:
	movs r0, #150
	lsls r0, r0, #4
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020091dc
	movs r0, #20
	movs r1, #0
	movs r2, #0
	bl Func_02001348
	movs r1, #248
	movs r2, #240
	movs r0, #14
	lsls r1, r1, #16
	lsls r2, r2, #17
	movs r3, #0
	bl Func_02001350
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #10
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02009174
	movs r0, #17
	movs r1, #0
	movs r2, #0
	bl Func_02001348
	movs r0, #16
	movs r1, #0
	movs r2, #0
	bl Func_02001348
	movs r0, #18
	movs r1, #0
	movs r2, #0
	bl Func_02001348
	b .L_020091dc
.L_02009174:
	movs r3, #224
	movs r2, #254
	lsls r3, r3, #8
	movs r0, #16
	ldr r1, .L_020091e8
	lsls r2, r2, #16
	bl Func_02001350
	movs r3, #128
	movs r1, #221
	movs r2, #203
	lsls r3, r3, #8
	movs r0, #18
	lsls r1, r1, #17
	lsls r2, r2, #16
	bl Func_02001350
	movs r3, #128
	movs r1, #245
	movs r2, #188
	lsls r1, r1, #17
	lsls r2, r2, #16
	lsls r3, r3, #6
	movs r0, #17
	bl Func_02001350
	movs r0, #17
	bl Object_GetById
	adds r5, r0, #0
	movs r0, #17
	bl Object_GetById
	movs r2, #10
	ldrsh r3, [r0, r2]
	adds r5, #100
	strh r3, [r5]
	movs r0, #17
	bl Object_GetById
	adds r5, r0, #0
	movs r0, #17
	bl Object_GetById
	movs r1, #18
	ldrsh r3, [r0, r1]
	adds r5, #102
	strh r3, [r5]
	movs r0, #17
	movs r1, #2
	bl ObjectMotion_EnableActionAndSetCallback
.L_020091dc:
	movs r0, #0
	pop {r5, pc}
.L_020091e0:
	.4byte gPartyState
.L_020091e4:
	.4byte 0x0000010a
.L_020091e8:
	.4byte 0x01cb0000
	.section .text.x020091ec,"ax",%progbits
	.global Func_020011ec
	.thumb_func
Func_020011ec:
	push {lr}
	ldr r3, .L_02009218
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_0200921c
	cmp r2, r3
	bne .L_02009212
	movs r0, #150
	lsls r0, r0, #4
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009212
	bl Func_020000ac
.L_02009212:
	movs r0, #0
	pop {pc}
	.2byte 0x0000
.L_02009218:
	.4byte gPartyState
.L_0200921c:
	.4byte 0x0000010a
	.section .rodata.x02009450,"a",%progbits
	.global Data_02001450
Data_02001450:
	.4byte 0x575a0260
	.4byte 0x46754ad7
	.4byte 0x31cf3a32
	.4byte 0x28ea294c
	.4byte 0x00750009
	.4byte 0x01bf011f
	.4byte 0x031f027f
	.4byte 0x7fff03ff
