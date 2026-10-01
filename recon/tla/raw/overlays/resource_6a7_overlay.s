.syntax unified
	.thumb
	.section .text.x02008038,"ax",%progbits
	.global Func_02000038
	.thumb_func
Func_02000038:
	ldr r2, [r0, #80]
	movs r3, #0
	strb r3, [r2, #26]
	adds r0, #34
	movs r3, #1
	strb r3, [r0]
	movs r0, #1
	bx lr
	.section .text.x02008064,"ax",%progbits
	.global Func_02000064
	.thumb_func
Func_02000064:
	push {r5, lr}
	sub sp, #8
	movs r3, #9
	str r3, [sp, #4]
	movs r5, #25
	movs r0, #47
	movs r1, #9
	movs r2, #9
	movs r3, #4
	str r5, [sp, #0]
	bl Func_02000a40
	movs r3, #69
	str r3, [sp, #4]
	movs r0, #47
	movs r1, #69
	movs r2, #9
	movs r3, #10
	str r5, [sp, #0]
	bl Func_02000a40
	movs r3, #89
	movs r2, #8
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #111
	movs r1, #8
	movs r2, #9
	movs r3, #5
	bl Func_02000a38
	add sp, #8
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x020080a8,"ax",%progbits
	.global Func_020000a8
	.thumb_func
Func_020000a8:
	push {lr}
	movs r0, #128
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008100
	movs r0, #128
	lsls r0, r0, #2
	bl GameFlag_SetBit
	bl Func_02000a68
	movs r0, #0
	bl Func_02000b30
	ldr r0, .L_02008104
	bl Func_02000ac0
	movs r0, #8
	bl Object_GetById
	bl Func_02000a30
	movs r1, #192
	lsls r1, r1, #8
	movs r0, #8
	bl Func_02000ad8
	movs r0, #5
	bl Battle_WaitMode0
	movs r0, #5
	movs r1, #0
	movs r2, #5
	bl Func_02000ad0
	movs r0, #11
	movs r1, #0
	movs r2, #5
	bl Func_02000ad0
	bl Func_02000a70
.L_02008100:
	pop {pc}
	.2byte 0x0000
.L_02008104:
	.4byte MsgShipIceWallFirstApproach
	.section .text.x02008108,"ax",%progbits
	.global Func_02000108
	.thumb_func
Func_02000108:
	push {lr}
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #12
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020081d2
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #12
	bl GameFlag_SetBit
	bl Func_02000a68
	movs r0, #0
	bl Func_02000b30
	ldr r0, .L_020081d4
	bl Func_02000ac0
	movs r0, #8
	bl Object_GetById
	bl Func_02000a30
	movs r0, #5
	bl Battle_WaitMode0
	movs r0, #5
	movs r1, #0
	movs r2, #5
	bl Func_02000ad0
	movs r0, #232
	movs r1, #1
	movs r2, #176
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #16
	lsls r0, r0, #17
	bl Motion_CamBounds
	bl Func_02000af0
	movs r0, #5
	bl Battle_WaitMode0
	movs r1, #204
	movs r2, #144
	movs r0, #11
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_02000ab0
	movs r1, #204
	movs r2, #144
	movs r0, #5
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_02000ab0
	movs r1, #204
	movs r2, #144
	movs r0, #6
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_02000ab0
	movs r0, #11
	movs r1, #0
	movs r2, #5
	bl Func_02000ad0
	movs r0, #6
	movs r1, #0
	movs r2, #5
	bl Func_02000ad0
	movs r0, #11
	movs r1, #0
	movs r2, #5
	bl Func_02000ad0
	movs r0, #11
	movs r1, #0
	movs r2, #0
	bl Func_02000ab0
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl Func_02000ab0
	movs r0, #6
	movs r1, #0
	movs r2, #0
	bl Func_02000ab0
	bl Func_02000a70
.L_020081d2:
	pop {pc}
.L_020081d4:
	.4byte MsgShipIceWallCrewReposition
	.section .text.x020081d8,"ax",%progbits
	.global Func_020001d8
	.thumb_func
Func_020001d8:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r0, #9
	sub sp, #24
	bl Object_GetById
	adds r6, r0, #0
	movs r0, #10
	bl Object_GetById
	mov r8, r0
	movs r0, #8
	bl Object_GetById
	adds r7, r0, #0
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #14
	bl GameFlag_Test
	adds r5, r0, #0
	cmp r5, #0
	beq .L_0200820c
	b .L_020085f2
.L_0200820c:
	bl Func_02000a68
	movs r0, #0
	bl Func_02000b30
	ldr r2, .L_02008328
	mov r10, r2
	mov r0, r10
	bl Func_02000ac0
	movs r1, #232
	movs r2, #216
	movs r0, #8
	lsls r1, r1, #1
	bl ObjectMotion_SetPositionAndCommit
	movs r1, #192
	lsls r1, r1, #8
	movs r0, #8
	bl Func_02000ad8
	movs r0, #5
	bl Battle_WaitMode0
	movs r0, #11
	bl Object_GetById
	str r5, [r0, #24]
	movs r0, #7
	bl Object_GetById
	str r5, [r0, #24]
	movs r0, #5
	bl Object_GetById
	str r5, [r0, #24]
	movs r0, #6
	bl Object_GetById
	str r5, [r0, #24]
	movs r0, #11
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #7
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #5
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #6
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r1, #232
	movs r2, #216
	movs r0, #11
	lsls r1, r1, #17
	lsls r2, r2, #16
	bl Func_02000ab0
	movs r1, #232
	movs r2, #216
	movs r0, #7
	lsls r1, r1, #17
	lsls r2, r2, #16
	bl Func_02000ab0
	movs r1, #232
	movs r2, #216
	movs r0, #5
	lsls r1, r1, #17
	lsls r2, r2, #16
	bl Func_02000ab0
	movs r1, #232
	movs r2, #216
	movs r0, #6
	lsls r1, r1, #17
	lsls r2, r2, #16
	bl Func_02000ab0
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #13
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020082ea
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #13
	bl GameFlag_SetBit
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #7
	movs r1, #0
	movs r2, #5
	bl Func_02000ad0
.L_020082ea:
	mov r0, r10
	adds r0, #1
	bl Func_02000ac0
	movs r1, #0
	movs r0, #11
	bl UiText_OpenMessageAtObject
	movs r0, #4
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	beq .L_0200832c
	movs r0, #11
	movs r1, #0
	movs r2, #5
	bl Func_02000ad0
	movs r0, #8
	movs r1, #0
	movs r2, #16
	bl ObjectMotion_OffsetPositionAndResetMotion
	adds r0, r7, #0
	bl Func_02000a30
	movs r0, #5
	bl Battle_WaitMode0
	b .L_020085c6
.L_02008328:
	.4byte MsgShipSafeFiringDistance
.L_0200832c:
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #11
	adds r3, #1
	strh r3, [r2]
	movs r1, #0
	movs r2, #5
	bl Func_02000ad0
	movs r1, #192
	movs r2, #192
	lsls r1, r1, #10
	lsls r2, r2, #9
	movs r0, #8
	bl ObjectMotion_SetSpeedParameters
	movs r0, #8
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	strb r3, [r0]
	movs r1, #0
	movs r2, #16
	movs r0, #8
	bl ObjectMotion_OffsetPositionAndReset
	movs r0, #144
	bl Func_02000b58
	movs r0, #4
	bl Battle_WaitMode0
	movs r3, #192
	lsls r3, r3, #9
	mov r2, r8
	str r3, [r2, #24]
	str r3, [r2, #28]
	movs r0, #10
	bl Object_GetById
	movs r1, #6
	bl Animation_ApplyChildValues
	movs r1, #232
	movs r2, #200
	lsls r2, r2, #16
	movs r0, #10
	lsls r1, r1, #17
	bl Func_02000ab0
	movs r0, #10
	movs r1, #1
	bl Object_SetModeById
	movs r3, #128
	lsls r3, r3, #8
	movs r1, #128
	movs r2, #128
	str r3, [r6, #24]
	str r3, [r6, #28]
	movs r0, #9
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl ObjectMotion_SetSpeedParameters
	movs r1, #232
	movs r2, #216
	movs r0, #9
	lsls r1, r1, #17
	lsls r2, r2, #16
	bl Func_02000ab0
	movs r1, #232
	movs r0, #9
	lsls r1, r1, #1
	movs r2, #144
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r5, #15
.L_020083da:
	ldr r3, [r6, #24]
	movs r2, #128
	lsls r2, r2, #4
	adds r3, r3, r2
	str r3, [r6, #24]
	ldr r3, [r6, #28]
	movs r0, #1
	adds r3, r3, r2
	str r3, [r6, #28]
	subs r5, #1
	bl WaitFrames
	cmp r5, #0
	bge .L_020083da
	movs r0, #236
	movs r1, #1
	movs r2, #176
	negs r1, r1
	lsls r2, r2, #16
	movs r3, #1
	lsls r0, r0, #17
	bl Motion_CamBounds
	bl Func_02000af0
	movs r0, #9
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #145
	bl Func_02000b58
	ldr r3, [r6, #8]
	ldr r2, .L_020085fc
	add r0, sp, #12
	adds r3, r3, r2
	str r3, [r0]
	ldr r2, .L_02008600
	ldr r3, [r6, #12]
	mov r1, sp
	str r3, [r0, #4]
	movs r5, #192
	ldr r3, [r6, #16]
	lsls r5, r5, #18
	adds r3, r3, r2
	str r3, [r0, #8]
	movs r3, #200
	lsls r3, r3, #17
	str r3, [r1]
	movs r3, #0
	str r3, [r1, #4]
	movs r3, #160
	lsls r3, r3, #15
	str r3, [r1, #8]
	ldr r2, .L_02008604
	bl Func_020006b0
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_02000ab0
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_02000ab0
	ldr r2, [r5, #108]
	movs r6, #218
	movs r3, #100
	lsls r6, r6, #1
	movs r0, #128
	str r3, [r2, r6]
	lsls r0, r0, #9
	movs r1, #0
	bl Func_02000b10
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	ldr r2, [r5, #108]
	movs r0, #128
	movs r3, #8
	lsls r0, r0, #6
	str r3, [r2, r6]
	adds r0, #5
	movs r1, #0
	movs r2, #5
	bl Func_02000ad0
	movs r0, #11
	movs r1, #0
	movs r2, #5
	bl Func_02000ad0
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #6
	movs r1, #0
	movs r2, #5
	bl Func_02000ad0
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #8
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #232
	movs r2, #176
	movs r0, #8
	lsls r1, r1, #1
	bl ObjectMotion_SetPositionAndCommit
	movs r0, #8
	movs r1, #1
	bl Object_AttachWorkTargetToObject
	movs r1, #232
	movs r0, #8
	lsls r1, r1, #1
	movs r2, #152
	bl ObjectMotion_SetPositionAndCommit
	movs r1, #153
	movs r2, #152
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #8
	adds r1, #153
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	ldr r5, .L_02008608
	ldr r3, [r7, #28]
	movs r2, #16
	adds r3, r3, r5
	str r3, [r7, #28]
	str r3, [r7, #24]
	movs r1, #0
	negs r2, r2
	movs r0, #8
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r0, #8
	bl ObjectMotion_CommitCurrentPositionAndActivate
	ldr r3, [r7, #28]
	movs r2, #16
	adds r3, r3, r5
	str r3, [r7, #28]
	str r3, [r7, #24]
	movs r1, #0
	negs r2, r2
	movs r0, #8
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r0, #8
	bl ObjectMotion_CommitCurrentPositionAndActivate
	ldr r3, [r7, #28]
	movs r2, #16
	adds r3, r3, r5
	str r3, [r7, #28]
	str r3, [r7, #24]
	movs r1, #0
	negs r2, r2
	movs r0, #8
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r0, #8
	bl ObjectMotion_CommitCurrentPositionAndActivate
	ldr r3, [r7, #28]
	movs r2, #16
	adds r3, r3, r5
	str r3, [r7, #28]
	str r3, [r7, #24]
	movs r1, #0
	negs r2, r2
	movs r0, #8
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r0, #8
	bl ObjectMotion_CommitCurrentPositionAndActivate
	ldr r3, [r7, #28]
	movs r2, #16
	adds r3, r3, r5
	str r3, [r7, #28]
	str r3, [r7, #24]
	movs r1, #0
	negs r2, r2
	movs r0, #8
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r0, #8
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r1, #204
	movs r2, #200
	lsls r1, r1, #6
	lsls r2, r2, #5
	movs r0, #8
	adds r1, #51
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r2, #12
	movs r0, #8
	movs r1, #0
	negs r2, r2
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r5, #19
.L_02008594:
	ldr r3, [r7, #28]
	ldr r2, .L_0200860c
	movs r0, #7
	adds r3, r3, r2
	str r3, [r7, #28]
	str r3, [r7, #24]
	subs r5, #1
	bl WaitFrames
	cmp r5, #0
	bge .L_02008594
	movs r3, #0
	str r3, [r7, #28]
	str r3, [r7, #24]
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #14
	bl GameFlag_SetBit
	movs r0, #4
	bl Func_02000b00
.L_020085c6:
	movs r0, #11
	movs r1, #0
	movs r2, #0
	bl Func_02000ab0
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl Func_02000ab0
	movs r0, #6
	movs r1, #0
	movs r2, #0
	bl Func_02000ab0
	movs r0, #7
	movs r1, #0
	movs r2, #0
	bl Func_02000ab0
	bl Func_02000a70
.L_020085f2:
	add sp, #24
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_020085fc:
	.4byte 0xfffe0000
.L_02008600:
	.4byte 0xfff80000
.L_02008604:
	.4byte Func_02000064
.L_02008608:
	.4byte 0xfffff000
.L_0200860c:
	.4byte 0xfffff700
	.section .text.x02008618,"ax",%progbits
	.global Func_02000618
	.thumb_func
Func_02000618:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	ldr r5, .L_02008678
	subs r2, #172
	str r2, [r3]
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r5, r2
	ldr r0, [r3]
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #32
	orrs r3, r2
	movs r2, #241
	lsls r2, r2, #1
	strb r3, [r0]
	adds r3, r5, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #90
	bne .L_0200866e
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #10
	bl GameFlag_SetBit
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #118
	adds r3, r5, r2
	movs r2, #1
	strh r2, [r3]
	ldr r0, .L_0200867c
	movs r1, #4
	bl Func_02000af8
.L_0200866e:
	bl Func_02000b08
	movs r0, #0
	pop {r5, pc}
	.2byte 0x0000
.L_02008678:
	.4byte gPartyState
.L_0200867c:
	.4byte 0x0000010c
	.section .text.x02008680,"ax",%progbits
	.global Func_02000680
	.thumb_func
Func_02000680:
	push {lr}
	ldr r3, .L_020086ac
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #3
	bne .L_02008696
	bl Func_02000064
.L_02008696:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #14
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020086a8
	bl Func_02000064
.L_020086a8:
	movs r0, #0
	pop {pc}
.L_020086ac:
	.4byte gPartyState
	.section .text.x020086b0,"ax",%progbits
	.global Func_020006b0
	.thumb_func
Func_020006b0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	mov r10, r1
	movs r1, #240
	lsls r1, r1, #5
	sub sp, #28
	adds r1, #88
	movs r0, #220
	str r2, [sp, #20]
	bl Runtime_AllocateHeapBlockFar
	movs r1, #0
	mov r9, r0
	ldr r0, .L_020088fc
	str r1, [sp, #8]
	bl Resource_GetTableEntry
	mov r1, r9
	bl Func_020009f0
	bl Resource_FindFreeEntry
	movs r1, #160
	mov r2, r9
	lsls r1, r1, #3
	str r0, [sp, #16]
	bl VramBlock_LoadCached
	movs r7, #192
	lsls r7, r7, #5
	movs r6, #128
	str r0, [sp, #12]
	movs r2, #0
	adds r7, #112
	lsls r6, r6, #5
	mov r8, r2
	add r7, r9
	add r6, r9
.L_02008706:
	ldr r2, [sp, #12]
	mov r1, r8
	movs r3, #3
	ands r3, r1
	lsls r3, r3, #3
	adds r3, r3, r2
	str r3, [sp, #0]
	movs r3, #128
	adds r0, r6, #0
	movs r1, #0
	movs r2, #0
	lsls r3, r3, #23
	bl Func_02000b40
	ldrb r1, [r6, #9]
	movs r3, #0
	movs r2, #13
	mov r11, r3
	negs r2, r2
	movs r3, #250
	strh r3, [r6, #30]
	adds r3, r2, #0
	ands r1, r3
	ldrb r3, [r6, #5]
	movs r2, #32
	orrs r3, r2
	strb r3, [r6, #5]
	movs r3, #15
	ands r1, r3
	strb r1, [r6, #9]
	mov r0, r8
	movs r1, #6
	bl __divsi3
	mov r1, r10
	ldr r3, [r1]
	adds r5, r0, #0
	lsls r2, r5, #20
	adds r3, r3, r2
	movs r1, #6
	str r3, [r7]
	mov r0, r8
	bl __modsi3
	mov r2, r10
	ldr r3, [r2, #4]
	lsls r0, r0, #20
	subs r3, r3, r0
	str r3, [r7, #4]
	subs r5, #4
	ldr r3, [r2, #8]
	adds r6, #40
	str r3, [r7, #8]
	bl Random16Far
	lsls r3, r0, #1
	adds r3, r3, r0
	lsrs r3, r3, #1
	muls r3, r5
	str r3, [r7, #12]
	bl Random16Far
	movs r3, #128
	movs r2, #1
	lsls r3, r3, #10
	lsls r0, r0, #2
	add r8, r2
	adds r0, r0, r3
	mov r1, r11
	mov r3, r8
	str r0, [r7, #16]
	str r1, [r7, #20]
	str r1, [r7, #24]
	adds r7, #28
	cmp r3, #53
	ble .L_02008706
	movs r0, #192
	lsls r0, r0, #3
	bl Runtime_BumpAllocateAlternatePool
	movs r5, #128
	lsls r5, r5, #5
	mov r10, r0
	adds r0, r5, #0
	bl Runtime_BumpAllocate
	adds r5, r0, #0
	ldr r0, .L_02008900
	bl Resource_GetTableEntry
	adds r1, r5, #0
	bl Func_020009f0
	bl Resource_FindFreeEntry
	movs r1, #128
	lsls r1, r1, #3
	adds r2, r5, #0
	mov r11, r0
	bl VramBlock_LoadCached
	str r0, [sp, #4]
	adds r0, r5, #0
	bl Sys_Free
	mov r5, r10
	movs r1, #95
	adds r5, #12
	mov r8, r1
.L_020087e0:
	bl Random16Far
	movs r2, #1
	lsls r0, r0, #5
	negs r2, r2
	lsrs r0, r0, #16
	add r8, r2
	negs r0, r0
	mov r3, r8
	str r0, [r5]
	adds r5, #16
	cmp r3, #0
	bge .L_020087e0
	movs r3, #128
	movs r2, #128
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	ldr r0, .L_02008904
	ldr r1, .L_02008908
	adds r2, #16
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r6, #0
.L_02008810:
	movs r3, #31
	ands r3, r6
	cmp r3, #0
	bne .L_0200881e
	movs r0, #145
	bl Func_02000b58
.L_0200881e:
	cmp r6, #60
	bne .L_0200883a
	movs r0, #254
	lsls r0, r0, #7
	adds r0, #255
	movs r1, #0
	bl Func_02000b10
	movs r0, #90
	bl Func_02000b18
	movs r0, #141
	bl Func_02000b58
.L_0200883a:
	cmp r6, #105
	bne .L_0200885e
	ldr r1, [sp, #20]
	mov lr, r1
	.2byte 0xf800
	movs r2, #1
	str r2, [sp, #8]
	movs r0, #192
	movs r1, #192
	movs r2, #128
	lsls r0, r0, #11
	lsls r1, r1, #11
	lsls r2, r2, #9
	bl Func_02000a50
	movs r0, #228
	bl Func_02000b58
.L_0200885e:
	cmp r6, #130
	bne .L_02008866
	movs r3, #1
	str r3, [sp, #8]
.L_02008866:
	ldr r1, [sp, #8]
	cmp r1, #0
	beq .L_020088ac
	movs r7, #192
	lsls r7, r7, #5
	movs r5, #128
	adds r7, #112
	lsls r5, r5, #5
	movs r2, #53
	add r7, r9
	add r5, r9
	mov r8, r2
.L_0200887e:
	ldr r3, [r7, #24]
	cmp r3, #59
	bhi .L_0200889c
	adds r0, r5, #0
	adds r1, r7, #0
	bl Func_02000b48
	adds r0, r7, #0
	movs r1, #63
	ldr r2, .L_0200890c
	bl Func_02000b50
	ldr r3, [r7, #24]
	adds r3, #1
	str r3, [r7, #24]
.L_0200889c:
	movs r3, #1
	negs r3, r3
	add r8, r3
	mov r1, r8
	adds r5, #40
	adds r7, #28
	cmp r1, #0
	bge .L_0200887e
.L_020088ac:
	mov r5, r10
.L_020088ae:
	ldr r2, [r5, #12]
	cmp r2, #0
	bne .L_02008910
	movs r3, #128
	lsls r3, r3, #23
	str r3, [r5, #4]
	str r2, [r5, #8]
	bl Random16Far
	ldr r3, .L_020088f4
	lsls r0, r0, #7
	lsrs r0, r0, #16
	adds r0, #52
	ands r0, r3
	ldr r2, .L_020088f8
	ldrh r3, [r5, #6]
	ands r3, r2
	orrs r3, r0
	strh r3, [r5, #6]
	bl Random16Far
	ldrb r3, [r5, #9]
	movs r1, #13
	movs r2, #240
	lsls r0, r0, #6
	negs r1, r1
	orrs r3, r2
	lsrs r0, r0, #16
	adds r2, r1, #0
	adds r0, #16
	ands r3, r2
	strb r0, [r5, #4]
	strb r3, [r5, #9]
	b .L_02008910
	.2byte 0x0000
.L_020088f4:
	.4byte 0x000001ff
.L_020088f8:
	.4byte 0xfffffe00
.L_020088fc:
	.4byte 0x000001f2
.L_02008900:
	.4byte 0x000001e8
.L_02008904:
	.4byte Data_02000b60
.L_02008908:
	.4byte 0x050003e0
.L_0200890c:
	.4byte 0xffffd000
.L_02008910:
	ldr r3, [r5, #12]
	cmp r3, #0
	blt .L_02008934
	ldr r2, [sp, #4]
	asrs r3, r3, #2
	lsls r3, r3, #2
	adds r3, r2, r3
	ldr r2, .L_02008950
	ldr r1, .L_02008954
	ands r3, r2
	ldrh r2, [r5, #8]
	adds r0, r5, #0
	ands r2, r1
	orrs r2, r3
	strh r2, [r5, #8]
	movs r1, #100
	bl Func_02000a10
.L_02008934:
	ldr r3, [r5, #12]
	adds r3, #1
	str r3, [r5, #12]
	cmp r3, #31
	ble .L_02008958
	bl Random16Far
	lsls r3, r0, #3
	subs r3, r3, r0
	lsrs r3, r3, #16
	negs r3, r3
	str r3, [r5, #12]
	b .L_02008958
	.2byte 0x0000
.L_02008950:
	.4byte 0x000003ff
.L_02008954:
	.4byte 0xfffffc00
.L_02008958:
	movs r3, #190
	lsls r3, r3, #3
	adds r5, #16
	add r3, r10
	cmp r5, r3
	ble .L_020088ae
	movs r0, #1
	adds r6, #1
	bl WaitFrames
	cmp r6, #149
	bgt .L_02008972
	b .L_02008810
.L_02008972:
	mov r0, r11
	bl Resource_ResetEntry
	mov r0, r10
	bl Sys_Free
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #0
	movs r2, #0
	movs r0, #0
	bl Func_02000a50
	ldr r0, [sp, #16]
	bl Resource_ResetEntry
	movs r0, #220
	bl Runtime_ReleaseHeapBlock
	add sp, #28
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.section .rodata.x02008b60,"a",%progbits
	.global Data_02000b60
Data_02000b60:
	.4byte 0x575a0260
	.4byte 0x46754ad7
	.4byte 0x31cf3a32
	.4byte 0x28ea294c
	.4byte 0x00750009
	.4byte 0x01bf011f
	.4byte 0x031f027f
	.4byte 0x7fff03ff
.L_02008b80:
	.4byte 0x0000002e
	.4byte Func_02000038
	.4byte 0x00000027
	.4byte 0x00000007
	.4byte 0x00000011
	.global gSceneEntrances
gSceneEntrances:
	.4byte 0xffff0000
	.4byte 0x000001b8
	.4byte 0x40000208
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0002
	.4byte 0x000001d6
	.4byte 0xc00000c0
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0003
	.4byte 0x000001d6
	.4byte 0xc00000c0
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
	.4byte 0x0000010c
	.4byte 0x1013c002
	.4byte 0xffffffff
	.4byte 0x1043d002
	.4byte 0xffffffff
	.4byte 0x000001ff
	.global gScenePlacements
gScenePlacements:
	.4byte 0xffff0008
	.4byte .L_02008b80
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00024000
	.4byte 0xffff0107
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00024000
	.4byte 0xffff011d
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00024000
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00020000
	.4byte 0xffff0005
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x0002e000
	.4byte 0xffff0006
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x0002e000
	.4byte 0xffff0007
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x0002e000
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
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000002
	.4byte 0x190a0014
	.4byte Func_02000108
	.4byte 0x00000002
	.4byte 0x190a0015
	.4byte Func_020001d8
	.4byte 0x00000002
	.4byte 0x090a0016
	.4byte Func_020000a8
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
