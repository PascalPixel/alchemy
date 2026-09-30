.syntax unified
	.thumb
	.section .text.x02008038,"ax",%progbits
	.global Func_02000038
	.thumb_func
Func_02000038:
	ldr r2, [r0, #80]
	movs r3, #128
	lsls r3, r3, #8
	strh r3, [r2, #18]
	movs r0, #0
	bx lr
	.section .text.x02008044,"ax",%progbits
	.global Func_02000044
	.thumb_func
Func_02000044:
	push {lr}
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #0
	pop {pc}
	.section .text.x02008050,"ax",%progbits
	.global Func_02000050
	.thumb_func
Func_02000050:
	push {r5, lr}
	ldmia r0!, {r5}
	ldmia r1!, {r3}
	ldmia r0!, {r4}
	subs r5, r5, r3
	ldmia r1!, {r3}
	asrs r5, r5, #16
	ldr r2, [r1]
	subs r4, r4, r3
	ldr r3, [r0]
	asrs r4, r4, #16
	subs r3, r3, r2
	asrs r3, r3, #16
	adds r0, r5, #0
	muls r0, r5
	adds r2, r4, #0
	muls r2, r4
	adds r1, r3, #0
	muls r1, r3
	adds r0, r0, r2
	adds r3, r1, #0
	adds r0, r0, r3
	ldr r3, .L_02008084
	mov lr, r3
	.2byte 0xf800
	pop {r5, pc}
.L_02008084:
	.4byte IwramFillWords + 0x74
	.section .text.x02008088,"ax",%progbits
	.global Func_02000088
	.thumb_func
Func_02000088:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	adds r5, r0, #0
	mov r11, r3
	adds r3, r5, #0
	adds r3, #99
	ldrb r3, [r3]
	mov r10, r2
	movs r2, #0
	mov r8, r1
	mov r9, r2
	cmp r3, #0
	beq .L_020080b8
	adds r2, r5, #0
	adds r2, #91
	movs r3, #1
	strb r3, [r2]
	movs r0, #1
	b .L_0200813a
.L_020080b8:
	mov r6, r8
	adds r7, r5, #0
	adds r6, #8
	adds r7, #8
	adds r0, r6, #0
	adds r1, r7, #0
	bl Func_02000050
	cmp r0, r10
	blt .L_020080d2
	mov r3, r11
	cmp r3, #0
	beq .L_02008128
.L_020080d2:
	mov r2, r8
	ldr r0, [r2, #16]
	ldr r3, [r5, #16]
	ldr r1, [r6]
	subs r0, r0, r3
	ldr r3, [r7]
	subs r1, r1, r3
	bl ArcTan2
	ldr r3, .L_02008148
	lsls r0, r0, #16
	movs r2, #128
	lsrs r0, r0, #16
	lsls r2, r2, #5
	adds r1, r0, r2
	ldrh r2, [r5, #6]
	adds r4, r0, r3
	movs r3, #240
	lsls r3, r3, #8
	ands r4, r3
	ands r1, r3
	ands r0, r3
	ands r3, r2
	cmp r0, r3
	beq .L_02008112
	cmp r1, r3
	beq .L_02008112
	cmp r4, r3
	beq .L_02008112
	mov r3, r11
	cmp r3, #0
	beq .L_02008138
.L_02008112:
	adds r2, r5, #0
	adds r2, #91
	movs r3, #1
	strb r3, [r2]
	adds r0, r5, #0
	movs r1, #1
	bl Func_02002448
	movs r2, #1
	mov r9, r2
	b .L_02008138
.L_02008128:
	adds r3, r5, #0
	adds r3, #91
	mov r2, r9
	strb r2, [r3]
	adds r0, r5, #0
	movs r1, #2
	bl Func_02002448
.L_02008138:
	mov r0, r9
.L_0200813a:
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008148:
	.4byte 0xfffff000
	.section .text.x0200814c,"ax",%progbits
	.global Func_0200014c
	.thumb_func
Func_0200014c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #60]
	ldr r3, [r3, #108]
	mov r10, r2
	mov r8, r3
	ldr r3, .L_02008200
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	adds r5, r0, #0
	ldr r0, [r3]
	bl Object_GetById
	adds r1, r5, #0
	adds r0, #8
	adds r1, #8
	movs r7, #0
	bl Func_02000050
	cmp r0, #11
	bgt .L_02008190
	adds r3, r5, #0
	adds r3, #91
	adds r0, r5, #0
	strb r7, [r3]
	movs r1, #2
	bl Func_02002448
	b .L_020081f4
.L_02008190:
	adds r6, r5, #0
	adds r6, #100
	ldrh r2, [r6]
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	beq .L_020081a2
	movs r0, #10
	b .L_020081a4
.L_020081a2:
	movs r0, #9
.L_020081a4:
	bl Object_GetById
	adds r1, r0, #0
	adds r0, r5, #0
	movs r2, #24
	movs r3, #0
	bl Func_02000088
	cmp r0, #0
	bne .L_020081f4
	ldr r3, .L_02008200
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	movs r3, #176
	lsls r3, r3, #1
	add r3, r8
	movs r2, #0
	ldrsh r3, [r3, r2]
	adds r1, r0, #0
	cmp r3, #0
	bne .L_020081de
	mov r2, r10
	ldrb r3, [r2, #4]
	cmp r3, #0
	beq .L_020081ea
.L_020081de:
	ldrh r2, [r6]
	movs r3, #2
	ands r3, r2
	cmp r3, #0
	beq .L_020081ea
	movs r7, #1
.L_020081ea:
	adds r0, r5, #0
	movs r2, #24
	adds r3, r7, #0
	bl Func_02000088
.L_020081f4:
	movs r0, #0
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008200:
	.4byte gPartyState
	.section .text.x02008218,"ax",%progbits
	.global Func_02000218
	.thumb_func
Func_02000218:
	push {lr}
	ldr r1, .L_02008258
	movs r0, #240
	lsls r0, r0, #1
	adds r3, r1, r0
	movs r0, #0
	ldrsh r2, [r3, r0]
	ldr r3, .L_0200825c
	cmp r2, r3
	bne .L_0200823e
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r1, r2
	movs r0, #0
	ldrsh r3, [r3, r0]
	cmp r3, #15
	bne .L_0200823e
	ldr r0, .L_02008260
	b .L_02008254
.L_0200823e:
	movs r2, #240
	lsls r2, r2, #1
	adds r3, r1, r2
	movs r0, #0
	ldrsh r2, [r3, r0]
	ldr r3, .L_02008264
	cmp r2, r3
	bne .L_02008252
	ldr r0, .L_02008268
	b .L_02008254
.L_02008252:
	ldr r0, .L_0200826c
.L_02008254:
	pop {pc}
	.2byte 0x0000
.L_02008258:
	.4byte gPartyState
.L_0200825c:
	.4byte 0x0000008b
.L_02008260:
	.4byte Data_02002bfc
.L_02008264:
	.4byte 0x0000008c
.L_02008268:
	.4byte Data_02002be4
.L_0200826c:
	.4byte Data_02002ac4
	.section .text.x02008270,"ax",%progbits
	.global Func_02000270
	.thumb_func
Func_02000270:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r7, [r3, #108]
	bl Func_02002488
	movs r0, #0
	bl Func_02002590
	movs r5, #8
.L_02008284:
	adds r0, r5, #0
	bl Object_GetById
	cmp r0, #0
	beq .L_02008296
	adds r2, r0, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
.L_02008296:
	adds r5, #1
	cmp r5, #63
	bls .L_02008284
	movs r2, #170
	lsls r2, r2, #1
	adds r3, r7, r2
	movs r2, #0
	ldrsh r6, [r3, r2]
	movs r0, #158
	bl Func_020025a0
	subs r6, #1
	ldr r0, .L_02008320
	lsls r4, r6, #3
	adds r3, r4, #4
	ldrh r1, [r0, r3]
	adds r3, r3, r0
	ldrh r2, [r3, #2]
	ldr r0, [r0, r4]
	bl Func_02002460
	ldr r5, .L_02008324
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	ldr r0, [r5]
	movs r1, #2
	bl Object_SetModeById
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	ldr r0, [r5]
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	ldr r0, [r5]
	cmp r6, #7
	bne .L_020082f4
	movs r2, #4
	movs r1, #0
	negs r2, r2
	bl ObjectMotion_OffsetPositionAndResetMotion
	b .L_020082fe
.L_020082f4:
	movs r2, #4
	movs r1, #2
	negs r2, r2
	bl ObjectMotion_SnapHeadingAndOffset
.L_020082fe:
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #170
	lsls r2, r2, #1
	adds r3, r7, r2
	movs r2, #0
	ldrsh r0, [r3, r2]
	bl Func_02002578
	bl Event_ClearStatus1c6
	bl Event_WaitValue1c8Frames
	bl Func_02002490
	pop {r5, r6, r7, pc}
.L_02008320:
	.4byte Data_02002ce4
.L_02008324:
	.4byte gPartyState
	.section .text.x02008328,"ax",%progbits
	.global Func_02000328
	.thumb_func
Func_02000328:
	push {r5, r6, lr}
	sub sp, #12
	movs r3, #67
	str r3, [sp, #0]
	movs r5, #1
	movs r6, #5
	movs r0, #67
	movs r1, #25
	movs r2, #5
	movs r3, #5
	str r6, [sp, #4]
	str r5, [sp, #8]
	bl Func_02002598
	movs r0, #23
	movs r1, #29
	movs r2, #5
	movs r3, #6
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl Func_02002468
	movs r3, #8
	str r3, [sp, #4]
	movs r0, #23
	movs r1, #29
	movs r2, #1
	movs r3, #1
	str r6, [sp, #0]
	bl Func_02002470
	add sp, #12
	pop {r5, r6, pc}
	.2byte 0x0000
	.section .text.x0200836c,"ax",%progbits
	.global Func_0200036c
	.thumb_func
Func_0200036c:
	push {lr}
	sub sp, #12
	movs r3, #79
	movs r2, #22
	movs r1, #1
	str r3, [sp, #0]
	str r2, [sp, #4]
	str r1, [sp, #8]
	movs r0, #67
	movs r1, #25
	movs r2, #2
	movs r3, #1
	bl Func_02002598
	add sp, #12
	pop {pc}
	.section .text.x0200838c,"ax",%progbits
	.global Func_0200038c
	.thumb_func
Func_0200038c:
	push {r5, r6, lr}
	ldr r3, .L_02008418
	movs r2, #133
	lsls r2, r2, #2
	adds r6, r3, r2
	ldr r0, [r6]
	sub sp, #12
	bl Object_GetById
	movs r3, #26
	movs r2, #76
	movs r1, #2
	str r3, [sp, #0]
	str r2, [sp, #4]
	str r1, [sp, #8]
	adds r5, r0, #0
	movs r1, #95
	movs r0, #26
	movs r2, #2
	movs r3, #1
	bl Func_02002598
	movs r0, #240
	lsls r0, r0, #4
	adds r0, #98
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008412
	movs r1, #212
	movs r2, #232
	movs r0, #66
	lsls r1, r1, #17
	lsls r2, r2, #16
	bl Func_020024f8
	ldr r3, [r5, #8]
	asrs r3, r3, #20
	cmp r3, #26
	bne .L_02008412
	ldr r3, [r5, #16]
	asrs r3, r3, #20
	cmp r3, #14
	bne .L_02008412
	bl Func_02002488
	movs r0, #0
	bl Func_02002590
	movs r1, #129
	ldr r0, [r6]
	lsls r1, r1, #1
	bl Func_02002550
	ldr r0, [r6]
	movs r1, #16
	movs r2, #0
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r3, #192
	lsls r3, r3, #11
	str r3, [r5, #40]
	ldr r0, [r6]
	bl ObjectMotion_CommitCurrentPositionAndActivate
	bl Func_02002490
.L_02008412:
	add sp, #12
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02008418:
	.4byte gPartyState
	.section .text.x0200841c,"ax",%progbits
	.global Func_0200041c
	.thumb_func
Func_0200041c:
	push {lr}
	sub sp, #12
	movs r3, #106
	movs r2, #20
	movs r1, #1
	str r3, [sp, #0]
	str r2, [sp, #4]
	str r1, [sp, #8]
	movs r0, #67
	movs r1, #25
	movs r2, #3
	movs r3, #3
	bl Func_02002598
	add sp, #12
	pop {pc}
	.section .text.x0200843c,"ax",%progbits
	.global Func_0200043c
	.thumb_func
Func_0200043c:
	push {r5, r6, lr}
	ldr r3, .L_020084c8
	movs r2, #133
	lsls r2, r2, #2
	adds r6, r3, r2
	ldr r0, [r6]
	sub sp, #12
	bl Object_GetById
	movs r3, #106
	movs r2, #20
	movs r1, #1
	str r3, [sp, #0]
	str r2, [sp, #4]
	str r1, [sp, #8]
	adds r5, r0, #0
	movs r1, #25
	movs r0, #67
	movs r2, #3
	movs r3, #3
	bl Func_02002598
	movs r0, #246
	lsls r0, r0, #4
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020084c2
	movs r1, #170
	movs r2, #172
	movs r0, #64
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl Func_020024f8
	ldr r3, [r5, #8]
	asrs r3, r3, #20
	cmp r3, #42
	bne .L_020084c2
	ldr r3, [r5, #16]
	asrs r3, r3, #20
	cmp r3, #21
	bne .L_020084c2
	bl Func_02002488
	movs r0, #0
	bl Func_02002590
	movs r1, #129
	ldr r0, [r6]
	lsls r1, r1, #1
	bl Func_02002550
	movs r2, #16
	ldr r0, [r6]
	movs r1, #0
	negs r2, r2
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r3, #192
	lsls r3, r3, #11
	str r3, [r5, #40]
	ldr r0, [r6]
	bl ObjectMotion_CommitCurrentPositionAndActivate
	bl Func_02002490
.L_020084c2:
	add sp, #12
	pop {r5, r6, pc}
	.2byte 0x0000
.L_020084c8:
	.4byte gPartyState
	.section .text.x020084cc,"ax",%progbits
	.global Func_020004cc
	.thumb_func
Func_020004cc:
	push {lr}
	sub sp, #12
	movs r3, #87
	movs r2, #8
	movs r1, #1
	str r3, [sp, #0]
	str r2, [sp, #4]
	str r1, [sp, #8]
	movs r0, #67
	movs r1, #25
	movs r2, #2
	movs r3, #2
	bl Func_02002598
	add sp, #12
	pop {pc}
	.section .text.x020084ec,"ax",%progbits
	.global Func_020004ec
	.thumb_func
Func_020004ec:
	push {lr}
	ldr r1, .L_0200852c
	movs r0, #240
	lsls r0, r0, #1
	adds r3, r1, r0
	movs r0, #0
	ldrsh r2, [r3, r0]
	ldr r3, .L_02008530
	cmp r2, r3
	bne .L_02008512
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r1, r2
	movs r0, #0
	ldrsh r3, [r3, r0]
	cmp r3, #15
	bne .L_02008512
	ldr r0, .L_02008534
	b .L_02008528
.L_02008512:
	movs r2, #240
	lsls r2, r2, #1
	adds r3, r1, r2
	movs r0, #0
	ldrsh r2, [r3, r0]
	ldr r3, .L_02008538
	cmp r2, r3
	bne .L_02008526
	ldr r0, .L_0200853c
	b .L_02008528
.L_02008526:
	ldr r0, .L_02008540
.L_02008528:
	pop {pc}
	.2byte 0x0000
.L_0200852c:
	.4byte gPartyState
.L_02008530:
	.4byte 0x0000008b
.L_02008534:
	.4byte Data_02002f10
.L_02008538:
	.4byte 0x0000008c
.L_0200853c:
	.4byte Data_02002eec
.L_02008540:
	.4byte Data_02002d24
	.section .text.x02008544,"ax",%progbits
	.global Func_02000544
	.thumb_func
Func_02000544:
	push {lr}
	bl Func_02002488
	movs r0, #0
	bl Func_02002590
	ldr r0, .L_02008580
	bl Func_02002520
	movs r1, #0
	movs r0, #8
	bl UiText_OpenMessageAtObject
	movs r0, #0
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_02008570
	bl Func_02001944
	b .L_02008578
.L_02008570:
	movs r0, #8
	movs r1, #0
	bl Func_02002538
.L_02008578:
	bl Func_02002490
	pop {pc}
	.2byte 0x0000
.L_02008580:
	.4byte 0x0000223f
	.section .text.x02008584,"ax",%progbits
	.global Func_02000584
	.thumb_func
Func_02000584:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	movs r0, #9
	ldr r7, [r3, #108]
	bl Object_GetById
	adds r5, r0, #0
	movs r3, #6
	ldrsh r2, [r5, r3]
	adds r6, r5, #0
	mov r8, r2
	bl Func_02002488
	movs r0, #0
	bl Func_02002590
	movs r2, #179
	movs r3, #1
	lsls r2, r2, #1
	adds r6, #99
	strb r3, [r6]
	adds r3, r7, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	bne .L_020085d8
	ldr r3, .L_02008608
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	movs r0, #9
	ldr r1, [r3]
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	ldr r0, .L_0200860c
	bl Func_02002520
	b .L_020085de
.L_020085d8:
	ldr r0, .L_02008610
	bl Func_02002520
.L_020085de:
	movs r0, #9
	movs r1, #0
	bl Func_02002538
	movs r2, #179
	lsls r2, r2, #1
	adds r3, r7, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	bne .L_020085f8
	mov r3, r8
	strh r3, [r5, #6]
.L_020085f8:
	movs r3, #0
	strb r3, [r6]
	bl Func_02002490
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008608:
	.4byte gPartyState
.L_0200860c:
	.4byte 0x00002241
.L_02008610:
	.4byte 0x00002245
	.section .text.x02008614,"ax",%progbits
	.global Func_02000614
	.thumb_func
Func_02000614:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	movs r0, #10
	ldr r7, [r3, #108]
	bl Object_GetById
	adds r5, r0, #0
	movs r3, #6
	ldrsh r2, [r5, r3]
	adds r6, r5, #0
	mov r8, r2
	bl Func_02002488
	movs r0, #0
	bl Func_02002590
	movs r2, #179
	movs r3, #1
	lsls r2, r2, #1
	adds r6, #99
	strb r3, [r6]
	adds r3, r7, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	bne .L_02008668
	ldr r3, .L_02008698
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	movs r0, #10
	ldr r1, [r3]
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	ldr r0, .L_0200869c
	bl Func_02002520
	b .L_0200866e
.L_02008668:
	ldr r0, .L_020086a0
	bl Func_02002520
.L_0200866e:
	movs r0, #10
	movs r1, #0
	bl Func_02002538
	movs r2, #179
	lsls r2, r2, #1
	adds r3, r7, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	bne .L_02008688
	mov r3, r8
	strh r3, [r5, #6]
.L_02008688:
	movs r3, #0
	strb r3, [r6]
	bl Func_02002490
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008698:
	.4byte gPartyState
.L_0200869c:
	.4byte 0x00002242
.L_020086a0:
	.4byte 0x00002246
	.section .text.x020086a4,"ax",%progbits
	.global Func_020006a4
	.thumb_func
Func_020006a4:
	push {lr}
	ldr r2, [r0, #56]
	movs r3, #128
	lsls r3, r3, #24
	cmp r2, r3
	bne .L_020086b8
	ldr r3, [r0, #64]
	movs r0, #1
	cmp r3, r2
	beq .L_020086ba
.L_020086b8:
	movs r0, #0
.L_020086ba:
	pop {pc}
	.section .text.x020086bc,"ax",%progbits
	.global Func_020006bc
	.thumb_func
Func_020006bc:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r6, r0, #0
	bl Object_GetById
	movs r2, #98
	adds r5, r0, #0
	adds r2, r2, r5
	ldrb r3, [r2]
	mov r8, r2
	cmp r3, #1
	bne .L_020086da
	bl .L_0200967e
.L_020086da:
	adds r7, r5, #0
	adds r7, #100
	cmp r3, #1
	bgt .L_020086e8
	cmp r3, #0
	beq .L_020086f2
	b .L_02008724
.L_020086e8:
	cmp r3, #2
	beq .L_02008704
	cmp r3, #3
	beq .L_020086fc
	b .L_02008724
.L_020086f2:
	adds r0, r5, #0
	movs r1, #2
	bl Func_02002448
	b .L_02008724
.L_020086fc:
	adds r0, r5, #0
	movs r1, #1
	bl Func_02002448
.L_02008704:
	ldr r2, [r5, #12]
	ldr r3, [r5, #16]
	ldr r1, [r5, #8]
	adds r0, r5, #0
	bl Func_02002450
	ldrh r2, [r7]
	movs r3, #2
	negs r3, r3
	ands r3, r2
	strh r3, [r7]
	mov r2, r8
	movs r3, #1
	strb r3, [r2]
	bl .L_0200967e
.L_02008724:
	movs r2, #0
	ldrsh r3, [r7, r2]
	movs r2, #162
	lsls r2, r2, #1
	cmp r3, r2
	bcc .L_02008734
	bl .L_0200967e
.L_02008734:
	ldr r2, .L_0200873c
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	b .L_02008740
.L_0200873c:
	.4byte .L_02008744
.L_02008740:
	mov pc, r3
	.2byte 0x0000
.L_02008744:
	.4byte .L_02008c54
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x8c66
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x8c78
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x8c8a
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x8c9c
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x8cae
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x8cc0
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x8cd2
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x8ce4
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x8cf6
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x8d08
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x8d1a
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x8d2c
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x8d3e
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x8d50
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x8d62
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x8d74
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x8d86
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x8daa
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x8dbc
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x8dce
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x8de0
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x8dfc
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x8e0e
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x8e20
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x8e32
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x8e44
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x8e56
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x8e68
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x8e7a
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x8e8a
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x8e9a
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x8eaa
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x8eba
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x8eca
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x8eda
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x8eea
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x8efa
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x8f0a
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x8f1a
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x8f2a
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x8f3a
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x8f4a
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x8f5a
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x8f6a
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x8f7a
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x8f8a
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x8f9a
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x8faa
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x8fba
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x8fca
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x8fda
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x8fea
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x8ffa
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x900a
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x901a
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x902a
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x903a
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x904a
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x905a
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x906a
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x907a
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x908a
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x909a
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x90aa
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x90ba
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x90ca
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x90da
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x90ea
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x90fa
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x910a
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x911a
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x912a
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x913a
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x914a
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x915a
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x916a
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x9180
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x9190
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x919e
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x91ac
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x91ba
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x91c8
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x91d6
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x91e6
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x91f6
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x9206
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x9214
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x9222
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x9230
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x923e
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x924c
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x925c
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x926c
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x927c
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x928a
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x9298
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x92a6
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x92b4
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x92c2
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x92d2
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x92e2
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x92f2
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x9300
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x930e
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x931c
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x932a
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x9338
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x9346
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x9354
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x9362
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x9370
	.2byte 0x0200
	.2byte 0x93e6
	.2byte 0x0200
	.2byte 0x93f6
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x937e
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x938e
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x939e
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x93ae
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x93bc
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x93ca
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x93d8
	.2byte 0x0200
	.2byte 0x93e6
	.2byte 0x0200
	.2byte 0x93f6
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x940e
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x941e
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x942e
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x943e
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x944c
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x945a
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x9468
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x9488
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x9496
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x94ae
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x94bc
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x94ca
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x94d8
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x94e6
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x94f4
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x9502
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x9510
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x951e
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x952c
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x953a
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x9548
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x9556
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x9564
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x9572
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x9580
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x958e
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x959c
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x95aa
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x95b8
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x95c6
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x95d4
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x95e2
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x95f0
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x95fe
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x960c
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x961a
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x9628
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x9636
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x9644
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x9652
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
	.2byte 0x9660
	.2byte 0x0200
	.2byte 0x966e
	.2byte 0x0200
.L_02008c54:
	movs r1, #172
	movs r2, #206
	adds r0, r6, #0
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	bl .L_02009678
	.2byte 0x21a2
	.2byte 0x22cd
	.2byte 0x1c30
	.2byte 0x0049
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xfc2a
	.2byte 0xf000
	.2byte 0xfd00
	.2byte 0x21a0
	.2byte 0x22ca
	.2byte 0x1c30
	.2byte 0x0049
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xfc21
	.2byte 0xf000
	.2byte 0xfcf7
	.2byte 0x21a2
	.2byte 0x22c7
	.2byte 0x1c30
	.2byte 0x0049
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xfc18
	.2byte 0xf000
	.2byte 0xfcee
	.2byte 0x21ac
	.2byte 0x22c6
	.2byte 0x1c30
	.2byte 0x0049
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xfc0f
	.2byte 0xf000
	.2byte 0xfce5
	.2byte 0x21b6
	.2byte 0x22c7
	.2byte 0x1c30
	.2byte 0x0049
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xfc06
	.2byte 0xf000
	.2byte 0xfcdc
	.2byte 0x21b8
	.2byte 0x22ca
	.2byte 0x1c30
	.2byte 0x0049
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xfbfd
	.2byte 0xf000
	.2byte 0xfcd3
	.2byte 0x21b6
	.2byte 0x22cd
	.2byte 0x1c30
	.2byte 0x0049
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xfbf4
	.2byte 0xf000
	.2byte 0xfcca
	.2byte 0x21ac
	.2byte 0x22ce
	.2byte 0x1c30
	.2byte 0x0049
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xfbeb
	.2byte 0xf000
	.2byte 0xfcc1
	.2byte 0x21a2
	.2byte 0x22cd
	.2byte 0x1c30
	.2byte 0x0049
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xfbe2
	.2byte 0xf000
	.2byte 0xfcb8
	.2byte 0x21a0
	.2byte 0x22ca
	.2byte 0x1c30
	.2byte 0x0049
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xfbd9
	.2byte 0xf000
	.2byte 0xfcaf
	.2byte 0x21a2
	.2byte 0x22c7
	.2byte 0x1c30
	.2byte 0x0049
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xfbd0
	.2byte 0xf000
	.2byte 0xfca6
	.2byte 0x21ac
	.2byte 0x22c6
	.2byte 0x1c30
	.2byte 0x0049
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xfbc7
	.2byte 0xf000
	.2byte 0xfc9d
	.2byte 0x21b6
	.2byte 0x22c7
	.2byte 0x1c30
	.2byte 0x0049
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xfbbe
	.2byte 0xf000
	.2byte 0xfc94
	.2byte 0x21b8
	.2byte 0x22ca
	.2byte 0x1c30
	.2byte 0x0049
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xfbb5
	.2byte 0xf000
	.2byte 0xfc8b
	.2byte 0x21b6
	.2byte 0x22cd
	.2byte 0x1c30
	.2byte 0x0049
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xfbac
	.2byte 0xf000
	.2byte 0xfc82
	.2byte 0x21ac
	.2byte 0x22ce
	.2byte 0x1c30
	.2byte 0x0049
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xfba3
	.2byte 0xf000
	.2byte 0xfc79
	.2byte 0x21e6
	.2byte 0x22e6
	.2byte 0x0249
	.2byte 0x0252
	.2byte 0x1c30
	.2byte 0x31cc
	.2byte 0x32cc
	.2byte 0xf001
	.2byte 0xfb88
	.2byte 0x21b6
	.2byte 0x22cd
	.2byte 0x1c30
	.2byte 0x0049
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xfb91
	.2byte 0xf000
	.2byte 0xfc67
	.2byte 0x21b8
	.2byte 0x22ca
	.2byte 0x1c30
	.2byte 0x0049
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xfb88
	.2byte 0xf000
	.2byte 0xfc5e
	.2byte 0x21c4
	.2byte 0x22ca
	.2byte 0x1c30
	.2byte 0x0049
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xfb7f
	.2byte 0xf000
	.2byte 0xfc55
	.2byte 0x21c4
	.2byte 0x22ba
	.2byte 0x1c30
	.2byte 0x0049
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xfb76
	.2byte 0xf000
	.2byte 0xfc4c
	.2byte 0x1c30
	.2byte 0x49e6
	.2byte 0x4ae5
	.2byte 0xf001
	.2byte 0xfb5f
	.2byte 0x21ba
	.2byte 0x22b9
	.2byte 0x1c30
	.2byte 0x0049
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xfb68
	.2byte 0xf000
	.2byte 0xfc3e
	.2byte 0x21b8
	.2byte 0x22b6
	.2byte 0x1c30
	.2byte 0x0049
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xfb5f
	.2byte 0xf000
	.2byte 0xfc35
	.2byte 0x21ba
	.2byte 0x22b3
	.2byte 0x1c30
	.2byte 0x0049
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xfb56
	.2byte 0xf000
	.2byte 0xfc2c
	.2byte 0x21c4
	.2byte 0x22b2
	.2byte 0x1c30
	.2byte 0x0049
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xfb4d
	.2byte 0xf000
	.2byte 0xfc23
	.2byte 0x21ce
	.2byte 0x22b3
	.2byte 0x1c30
	.2byte 0x0049
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xfb44
	.2byte 0xf000
	.2byte 0xfc1a
	.2byte 0x21d0
	.2byte 0x22b6
	.2byte 0x1c30
	.2byte 0x0049
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xfb3b
	.2byte 0xf000
	.2byte 0xfc11
	.2byte 0x21ce
	.2byte 0x22b9
	.2byte 0x1c30
	.2byte 0x0049
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xfb32
	.2byte 0xf000
	.2byte 0xfc08
	.2byte 0x21c4
	.2byte 0x22ba
	.2byte 0x1c30
	.2byte 0x0049
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xfb29
	.2byte 0xf000
	.2byte 0xfbff
	.2byte 0x21ba
	.2byte 0x22b9
	.2byte 0x1c30
	.2byte 0x0049
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xfb20
	.2byte 0xe3f6
	.2byte 0x21b8
	.2byte 0x22b6
	.2byte 0x1c30
	.2byte 0x0049
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xfb18
	.2byte 0xe3ee
	.2byte 0x21ba
	.2byte 0x22b3
	.2byte 0x1c30
	.2byte 0x0049
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xfb10
	.2byte 0xe3e6
	.2byte 0x21c4
	.2byte 0x22b2
	.2byte 0x1c30
	.2byte 0x0049
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xfb08
	.2byte 0xe3de
	.2byte 0x21ce
	.2byte 0x22b3
	.2byte 0x1c30
	.2byte 0x0049
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xfb00
	.2byte 0xe3d6
	.2byte 0x21d0
	.2byte 0x22b6
	.2byte 0x1c30
	.2byte 0x0049
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xfaf8
	.2byte 0xe3ce
	.2byte 0x21ce
	.2byte 0x22b9
	.2byte 0x1c30
	.2byte 0x0049
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xfaf0
	.2byte 0xe3c6
	.2byte 0x21c4
	.2byte 0x22ba
	.2byte 0x1c30
	.2byte 0x0049
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xfae8
	.2byte 0xe3be
	.2byte 0x21ba
	.2byte 0x22b9
	.2byte 0x1c30
	.2byte 0x0049
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xfae0
	.2byte 0xe3b6
	.2byte 0x21b8
	.2byte 0x22b6
	.2byte 0x1c30
	.2byte 0x0049
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xfad8
	.2byte 0xe3ae
	.2byte 0x21ba
	.2byte 0x22b3
	.2byte 0x1c30
	.2byte 0x0049
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xfad0
	.2byte 0xe3a6
	.2byte 0x21c4
	.2byte 0x22b2
	.2byte 0x1c30
	.2byte 0x0049
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xfac8
	.2byte 0xe39e
	.2byte 0x21ce
	.2byte 0x22b3
	.2byte 0x1c30
	.2byte 0x0049
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xfac0
	.2byte 0xe396
	.2byte 0x21d0
	.2byte 0x22b6
	.2byte 0x1c30
	.2byte 0x0049
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xfab8
	.2byte 0xe38e
	.2byte 0x21ce
	.2byte 0x22b9
	.2byte 0x1c30
	.2byte 0x0049
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xfab0
	.2byte 0xe386
	.2byte 0x21c4
	.2byte 0x22ba
	.2byte 0x1c30
	.2byte 0x0049
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xfaa8
	.2byte 0xe37e
	.2byte 0x21ba
	.2byte 0x22b9
	.2byte 0x1c30
	.2byte 0x0049
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xfaa0
	.2byte 0xe376
	.2byte 0x21b8
	.2byte 0x22b6
	.2byte 0x1c30
	.2byte 0x0049
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xfa98
	.2byte 0xe36e
	.2byte 0x21ba
	.2byte 0x22b3
	.2byte 0x1c30
	.2byte 0x0049
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xfa90
	.2byte 0xe366
	.2byte 0x21c4
	.2byte 0x22b2
	.2byte 0x1c30
	.2byte 0x0049
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xfa88
	.2byte 0xe35e
	.2byte 0x21c4
	.2byte 0x22ae
	.2byte 0x1c30
	.2byte 0x0049
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xfa80
	.2byte 0xe356
	.2byte 0x21ce
	.2byte 0x22ad
	.2byte 0x1c30
	.2byte 0x0049
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xfa78
	.2byte 0xe34e
	.2byte 0x21d0
	.2byte 0x22aa
	.2byte 0x1c30
	.2byte 0x0049
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xfa70
	.2byte 0xe346
	.2byte 0x21ce
	.2byte 0x22a7
	.2byte 0x1c30
	.2byte 0x0049
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xfa68
	.2byte 0xe33e
	.2byte 0x21c4
	.2byte 0x22a6
	.2byte 0x1c30
	.2byte 0x0049
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xfa60
	.2byte 0xe336
	.2byte 0x21ba
	.2byte 0x22a7
	.2byte 0x1c30
	.2byte 0x0049
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xfa58
	.2byte 0xe32e
	.2byte 0x21b8
	.2byte 0x22aa
	.2byte 0x1c30
	.2byte 0x0049
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xfa50
	.2byte 0xe326
	.2byte 0x21a0
	.2byte 0x22aa
	.2byte 0x1c30
	.2byte 0x0049
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xfa48
	.2byte 0xe31e
	.2byte 0x219e
	.2byte 0x22ad
	.2byte 0x1c30
	.2byte 0x0049
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xfa40
	.2byte 0xe316
	.2byte 0x2194
	.2byte 0x22ae
	.2byte 0x1c30
	.2byte 0x0049
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xfa38
	.2byte 0xe30e
	.2byte 0x218a
	.2byte 0x22ad
	.2byte 0x1c30
	.2byte 0x0049
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xfa30
	.2byte 0xe306
	.2byte 0x2188
	.2byte 0x22aa
	.2byte 0x1c30
	.2byte 0x0049
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xfa28
	.2byte 0xe2fe
	.2byte 0x218a
	.2byte 0x22a7
	.2byte 0x1c30
	.2byte 0x0049
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xfa20
	.2byte 0xe2f6
	.2byte 0x2194
	.2byte 0x22a6
	.2byte 0x1c30
	.2byte 0x0049
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xfa18
	.2byte 0xe2ee
	.2byte 0x219e
	.2byte 0x22a7
	.2byte 0x1c30
	.2byte 0x0049
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xfa10
	.2byte 0xe2e6
	.2byte 0x21a0
	.2byte 0x22aa
	.2byte 0x1c30
	.2byte 0x0049
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xfa08
	.2byte 0xe2de
	.2byte 0x219e
	.2byte 0x22ad
	.2byte 0x1c30
	.2byte 0x0049
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xfa00
	.2byte 0xe2d6
	.2byte 0x2194
	.2byte 0x22ae
	.2byte 0x1c30
	.2byte 0x0049
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xf9f8
	.2byte 0xe2ce
	.2byte 0x218a
	.2byte 0x22ad
	.2byte 0x1c30
	.2byte 0x0049
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xf9f0
	.2byte 0xe2c6
	.2byte 0x2188
	.2byte 0x22aa
	.2byte 0x1c30
	.2byte 0x0049
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xf9e8
	.2byte 0xe2be
	.2byte 0x218a
	.2byte 0x22a7
	.2byte 0x1c30
	.2byte 0x0049
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xf9e0
	.2byte 0xe2b6
	.2byte 0x2194
	.2byte 0x22a6
	.2byte 0x1c30
	.2byte 0x0049
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xf9d8
	.2byte 0xe2ae
	.2byte 0x219e
	.2byte 0x22a7
	.2byte 0x1c30
	.2byte 0x0049
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xf9d0
	.2byte 0xe2a6
	.2byte 0x21a0
	.2byte 0x22aa
	.2byte 0x1c30
	.2byte 0x0049
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xf9c8
	.2byte 0xe29e
	.2byte 0x219e
	.2byte 0x22ad
	.2byte 0x1c30
	.2byte 0x0049
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xf9c0
	.2byte 0xe296
	.2byte 0x2194
	.2byte 0x22ae
	.2byte 0x1c30
	.2byte 0x0049
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xf9b8
	.2byte 0xe28e
	.2byte 0x2194
	.2byte 0x22b6
	.2byte 0x1c30
	.2byte 0x0049
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xf9b0
	.2byte 0xe286
	.2byte 0x2188
	.2byte 0x22b6
	.2byte 0x1c30
	.2byte 0x0049
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xf9a8
	.2byte 0xe27e
	.2byte 0x0000
	.2byte 0x3333
	.2byte 0x0001
	.2byte 0x2186
	.2byte 0x22b9
	.2byte 0x1c30
	.2byte 0x0049
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xf99d
	.2byte 0xe273
	.2byte 0x22ba
	.2byte 0x1c30
	.2byte 0x21f8
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xf996
	.2byte 0xe26c
	.2byte 0x22b9
	.2byte 0x1c30
	.2byte 0x21e4
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xf98f
	.2byte 0xe265
	.2byte 0x22b6
	.2byte 0x1c30
	.2byte 0x21e0
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xf988
	.2byte 0xe25e
	.2byte 0x22b3
	.2byte 0x1c30
	.2byte 0x21e4
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xf981
	.2byte 0xe257
	.2byte 0x22b2
	.2byte 0x1c30
	.2byte 0x21f8
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xf97a
	.2byte 0xe250
	.2byte 0x2186
	.2byte 0x22b3
	.2byte 0x1c30
	.2byte 0x0049
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xf972
	.2byte 0xe248
	.2byte 0x2188
	.2byte 0x22b6
	.2byte 0x1c30
	.2byte 0x0049
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xf96a
	.2byte 0xe240
	.2byte 0x2186
	.2byte 0x22b9
	.2byte 0x1c30
	.2byte 0x0049
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xf962
	.2byte 0xe238
	.2byte 0x22ba
	.2byte 0x1c30
	.2byte 0x21f8
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xf95b
	.2byte 0xe231
	.2byte 0x22b9
	.2byte 0x1c30
	.2byte 0x21e4
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xf954
	.2byte 0xe22a
	.2byte 0x22b6
	.2byte 0x1c30
	.2byte 0x21e0
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xf94d
	.2byte 0xe223
	.2byte 0x22b3
	.2byte 0x1c30
	.2byte 0x21e4
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xf946
	.2byte 0xe21c
	.2byte 0x22b2
	.2byte 0x1c30
	.2byte 0x21f8
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xf93f
	.2byte 0xe215
	.2byte 0x2186
	.2byte 0x22b3
	.2byte 0x1c30
	.2byte 0x0049
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xf937
	.2byte 0xe20d
	.2byte 0x2188
	.2byte 0x22b6
	.2byte 0x1c30
	.2byte 0x0049
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xf92f
	.2byte 0xe205
	.2byte 0x2186
	.2byte 0x22b9
	.2byte 0x1c30
	.2byte 0x0049
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xf927
	.2byte 0xe1fd
	.2byte 0x22ba
	.2byte 0x1c30
	.2byte 0x21f8
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xf920
	.2byte 0xe1f6
	.2byte 0x22b9
	.2byte 0x1c30
	.2byte 0x21e4
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xf919
	.2byte 0xe1ef
	.2byte 0x22b6
	.2byte 0x1c30
	.2byte 0x21e0
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xf912
	.2byte 0xe1e8
	.2byte 0x22b3
	.2byte 0x1c30
	.2byte 0x21e4
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xf90b
	.2byte 0xe1e1
	.2byte 0x22b2
	.2byte 0x1c30
	.2byte 0x21f8
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xf904
	.2byte 0xe1da
	.2byte 0x2186
	.2byte 0x22b3
	.2byte 0x1c30
	.2byte 0x0049
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xf8fc
	.2byte 0xe1d2
	.2byte 0x2188
	.2byte 0x22b6
	.2byte 0x1c30
	.2byte 0x0049
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xf8f4
	.2byte 0xe1ca
	.2byte 0x2186
	.2byte 0x22b9
	.2byte 0x1c30
	.2byte 0x0049
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xf8ec
	.2byte 0xe1c2
	.2byte 0x22ba
	.2byte 0x1c30
	.2byte 0x21f8
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xf8e5
	.2byte 0xe1bb
	.2byte 0x22be
	.2byte 0x1c30
	.2byte 0x21f8
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xf8de
	.2byte 0xe1b4
	.2byte 0x22bf
	.2byte 0x1c30
	.2byte 0x21e4
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xf8d7
	.2byte 0xe1ad
	.2byte 0x22c2
	.2byte 0x1c30
	.2byte 0x21e0
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xf8d0
	.2byte 0xe1a6
	.2byte 0x22c5
	.2byte 0x1c30
	.2byte 0x21e4
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xf8c9
	.2byte 0xe19f
	.2byte 0x22c6
	.2byte 0x1c30
	.2byte 0x21f8
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xf8c2
	.2byte 0xe198
	.2byte 0x22d2
	.2byte 0x1c30
	.2byte 0x21f8
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xf8bb
	.2byte 0xe191
	.2byte 0x22d3
	.2byte 0x1c30
	.2byte 0x21e4
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xf8b4
	.2byte 0xe18a
	.2byte 0x22d6
	.2byte 0x1c30
	.2byte 0x21e0
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xf8ad
	.2byte 0xe183
	.2byte 0x22d9
	.2byte 0x1c30
	.2byte 0x21e4
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xf8a6
	.2byte 0xe17c
	.2byte 0x2186
	.2byte 0x22d9
	.2byte 0x1c30
	.2byte 0x0049
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xf89e
	.2byte 0xe174
	.2byte 0x2188
	.2byte 0x22d6
	.2byte 0x1c30
	.2byte 0x0049
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xf896
	.2byte 0xe16c
	.2byte 0x2186
	.2byte 0x22d3
	.2byte 0x1c30
	.2byte 0x0049
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xf88e
	.2byte 0xe164
	.2byte 0x22d2
	.2byte 0x1c30
	.2byte 0x21f8
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xf887
	.2byte 0xe15d
	.2byte 0x22d3
	.2byte 0x1c30
	.2byte 0x21e4
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xf880
	.2byte 0xe156
	.2byte 0x22d6
	.2byte 0x1c30
	.2byte 0x21e0
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xf879
	.2byte 0xe14f
	.2byte 0x22d9
	.2byte 0x1c30
	.2byte 0x21e4
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xf872
	.2byte 0xe148
	.2byte 0x1c28
	.2byte 0xf7ff
	.2byte 0xf95c
	.2byte 0x2800
	.2byte 0xd002
	.2byte 0x883b
	.2byte 0x3301
	.2byte 0x803b
	.2byte 0x22da
	.2byte 0x0092
	.2byte 0x1c30
	.2byte 0x21f8
	.2byte 0xf001
	.2byte 0xf863
	.2byte 0x1c2a
	.2byte 0x3264
	.2byte 0x8813
	.2byte 0x3301
	.2byte 0x8013
	.2byte 0xe137
	.2byte 0x2186
	.2byte 0x22d9
	.2byte 0x1c30
	.2byte 0x0049
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xf856
	.2byte 0xe12c
	.2byte 0x2188
	.2byte 0x22d6
	.2byte 0x1c30
	.2byte 0x0049
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xf84e
	.2byte 0xe124
	.2byte 0x2186
	.2byte 0x22d3
	.2byte 0x1c30
	.2byte 0x0049
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xf846
	.2byte 0xe11c
	.2byte 0x22d2
	.2byte 0x1c30
	.2byte 0x21f8
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xf83f
	.2byte 0xe115
	.2byte 0x22d3
	.2byte 0x1c30
	.2byte 0x21e4
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xf838
	.2byte 0xe10e
	.2byte 0x22d6
	.2byte 0x1c30
	.2byte 0x21e0
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xf831
	.2byte 0xe107
	.2byte 0x21e6
	.2byte 0x22e6
	.2byte 0x0249
	.2byte 0x0252
	.2byte 0x1c30
	.2byte 0x31cc
	.2byte 0x32cc
	.2byte 0xf001
	.2byte 0xf817
	.2byte 0x22d6
	.2byte 0x1c30
	.2byte 0x21c8
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xf821
	.2byte 0xe0f7
	.2byte 0x22ba
	.2byte 0x1c30
	.2byte 0x21c8
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xf81a
	.2byte 0xe0f0
	.2byte 0x1c30
	.2byte 0x497a
	.2byte 0x4a7a
	.2byte 0xf001
	.2byte 0xf804
	.2byte 0x22b9
	.2byte 0x1c30
	.2byte 0x21b4
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xf80e
	.2byte 0xe0e4
	.2byte 0x22b6
	.2byte 0x1c30
	.2byte 0x21b0
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xf807
	.2byte 0xe0dd
	.2byte 0x22b3
	.2byte 0x1c30
	.2byte 0x21b4
	.2byte 0x0092
	.2byte 0xf001
	.2byte 0xf800
	.2byte 0xe0d6
	.2byte 0x22b2
	.2byte 0x1c30
	.2byte 0x21c8
	.2byte 0x0092
	.2byte 0xf000
	.2byte 0xfff9
	.2byte 0xe0cf
	.2byte 0x22b3
	.2byte 0x1c30
	.2byte 0x21dc
	.2byte 0x0092
	.2byte 0xf000
	.2byte 0xfff2
	.2byte 0xe0c8
	.2byte 0x22b6
	.2byte 0x1c30
	.2byte 0x21e0
	.2byte 0x0092
	.2byte 0xf000
	.2byte 0xffeb
	.2byte 0xe0c1
	.2byte 0x22b9
	.2byte 0x1c30
	.2byte 0x21dc
	.2byte 0x0092
	.2byte 0xf000
	.2byte 0xffe4
	.2byte 0xe0ba
	.2byte 0x22ba
	.2byte 0x1c30
	.2byte 0x21c8
	.2byte 0x0092
	.2byte 0xf000
	.2byte 0xffdd
	.2byte 0xe0b3
	.2byte 0x22b9
	.2byte 0x1c30
	.2byte 0x21b4
	.2byte 0x0092
	.2byte 0xf000
	.2byte 0xffd6
	.2byte 0xe0ac
	.2byte 0x22b6
	.2byte 0x1c30
	.2byte 0x21b0
	.2byte 0x0092
	.2byte 0xf000
	.2byte 0xffcf
	.2byte 0xe0a5
	.2byte 0x22b3
	.2byte 0x1c30
	.2byte 0x21b4
	.2byte 0x0092
	.2byte 0xf000
	.2byte 0xffc8
	.2byte 0xe09e
	.2byte 0x22b2
	.2byte 0x1c30
	.2byte 0x21c8
	.2byte 0x0092
	.2byte 0xf000
	.2byte 0xffc1
	.2byte 0xe097
	.2byte 0x22b3
	.2byte 0x1c30
	.2byte 0x21dc
	.2byte 0x0092
	.2byte 0xf000
	.2byte 0xffba
	.2byte 0xe090
	.2byte 0x22b6
	.2byte 0x1c30
	.2byte 0x21e0
	.2byte 0x0092
	.2byte 0xf000
	.2byte 0xffb3
	.2byte 0xe089
	.2byte 0x22b9
	.2byte 0x1c30
	.2byte 0x21dc
	.2byte 0x0092
	.2byte 0xf000
	.2byte 0xffac
	.2byte 0xe082
	.2byte 0x22ba
	.2byte 0x1c30
	.2byte 0x21c8
	.2byte 0x0092
	.2byte 0xf000
	.2byte 0xffa5
	.2byte 0xe07b
	.2byte 0x22b9
	.2byte 0x1c30
	.2byte 0x21b4
	.2byte 0x0092
	.2byte 0xf000
	.2byte 0xff9e
	.2byte 0xe074
	.2byte 0x22b6
	.2byte 0x1c30
	.2byte 0x21b0
	.2byte 0x0092
	.2byte 0xf000
	.2byte 0xff97
	.2byte 0xe06d
	.2byte 0x22b3
	.2byte 0x1c30
	.2byte 0x21b4
	.2byte 0x0092
	.2byte 0xf000
	.2byte 0xff90
	.2byte 0xe066
	.2byte 0x22b2
	.2byte 0x1c30
	.2byte 0x21c8
	.2byte 0x0092
	.2byte 0xf000
	.2byte 0xff89
	.2byte 0xe05f
	.2byte 0x22b3
	.2byte 0x1c30
	.2byte 0x21dc
	.2byte 0x0092
	.2byte 0xf000
	.2byte 0xff82
	.2byte 0xe058
	.2byte 0x22b6
	.2byte 0x1c30
	.2byte 0x21e0
	.2byte 0x0092
	.2byte 0xf000
	.2byte 0xff7b
	.2byte 0xe051
	.2byte 0x22b9
	.2byte 0x1c30
	.2byte 0x21dc
	.2byte 0x0092
	.2byte 0xf000
	.2byte 0xff74
	.2byte 0xe04a
	.2byte 0x22ba
	.2byte 0x1c30
	.2byte 0x21c8
	.2byte 0x0092
	.2byte 0xf000
	.2byte 0xff6d
	.2byte 0xe043
	.2byte 0x22b9
	.2byte 0x1c30
	.2byte 0x21b4
	.2byte 0x0092
	.2byte 0xf000
	.2byte 0xff66
	.2byte 0xe03c
	.2byte 0x22b6
	.2byte 0x1c30
	.2byte 0x21b0
	.2byte 0x0092
	.2byte 0xf000
	.2byte 0xff5f
	.2byte 0xe035
	.2byte 0x22b3
	.2byte 0x1c30
	.2byte 0x21b4
	.2byte 0x0092
	.2byte 0xf000
	.2byte 0xff58
	.2byte 0xe02e
	.2byte 0x22b2
	.2byte 0x1c30
	.2byte 0x21c8
	.2byte 0x0092
	.2byte 0xf000
	.2byte 0xff51
	.2byte 0xe027
	.2byte 0x22aa
	.2byte 0x1c30
	.2byte 0x21c8
	.2byte 0x0092
	.2byte 0xf000
	.2byte 0xff4a
	.2byte 0xe020
	.2byte 0x22a9
	.2byte 0x1c30
	.2byte 0x21d4
	.2byte 0x0092
	.2byte 0xf000
	.2byte 0xff43
	.2byte 0xe019
	.2byte 0x22a6
	.2byte 0x1c30
	.2byte 0x21d8
	.2byte 0x0092
	.2byte 0xf000
	.2byte 0xff3c
	.2byte 0xe012
	.2byte 0x22a3
	.2byte 0x1c30
	.2byte 0x21d4
	.2byte 0x0092
	.2byte 0xf000
	.2byte 0xff35
	.2byte 0xe00b
	.2byte 0x22a2
	.2byte 0x1c30
	.2byte 0x21c8
	.2byte 0x0092
	.2byte 0xf000
	.2byte 0xff2e
	.2byte 0xe004
	.2byte 0x1c28
	.2byte 0xf7ff
	.2byte 0xf818
	.2byte 0x2800
	.2byte 0xd002
.L_02009678:
	ldrh r3, [r7]
	adds r3, #1
	strh r3, [r7]
.L_0200967e:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.4byte 0x00013333
	.section .text.x02009688,"ax",%progbits
	.global Func_02001688
	.thumb_func
Func_02001688:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r0, #8
	bl Object_GetById
	adds r5, r0, #0
	movs r0, #10
	bl Object_GetById
	adds r6, r0, #0
	movs r0, #9
	bl Object_GetById
	adds r7, r0, #0
	movs r0, #11
	bl Object_GetById
	ldr r3, .L_0200972c
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	mov r8, r0
	ldr r0, [r3]
	bl Object_GetById
	adds r1, r5, #0
	adds r1, #102
	movs r3, #0
	ldrsh r2, [r1, r3]
	cmp r2, #2
	beq .L_020096de
	cmp r2, #2
	bgt .L_020096d2
	cmp r2, #1
	beq .L_020096d8
	b .L_020096fc
.L_020096d2:
	cmp r2, #3
	beq .L_020096de
	b .L_020096fc
.L_020096d8:
	adds r3, r5, #0
	movs r2, #0
	b .L_020096e0
.L_020096de:
	adds r3, r5, #0
.L_020096e0:
	adds r3, #98
	strb r2, [r3]
	adds r3, r6, #0
	adds r3, #98
	strb r2, [r3]
	adds r3, r7, #0
	adds r3, #98
	strb r2, [r3]
	mov r3, r8
	adds r3, #98
	strb r2, [r3]
	adds r3, r0, #0
	adds r3, #98
	strb r2, [r3]
.L_020096fc:
	movs r3, #0
	strh r3, [r1]
	movs r0, #8
	bl Func_020006bc
	movs r0, #10
	bl Func_020006bc
	movs r0, #9
	bl Func_020006bc
	movs r0, #11
	bl Func_020006bc
	ldr r3, .L_0200972c
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Func_020006bc
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_0200972c:
	.4byte gPartyState
	.section .text.x02009730,"ax",%progbits
	.global Func_02001730
	.thumb_func
Func_02001730:
	push {r5, lr}
	movs r0, #8
	bl Object_GetById
	adds r2, r0, #0
	adds r3, r2, #0
	adds r3, #100
	ldr r5, .L_02009820
	movs r1, #0
	ldrsh r3, [r3, r1]
	movs r0, #10
	str r3, [r5]
	adds r3, r2, #0
	adds r3, #102
	movs r1, #0
	ldrsh r3, [r3, r1]
	str r3, [r5, #4]
	adds r3, r2, #0
	adds r3, #98
	ldrb r3, [r3]
	str r3, [r5, #8]
	adds r3, r2, #0
	adds r3, #99
	ldrb r3, [r3]
	str r3, [r5, #12]
	bl Object_GetById
	adds r2, r0, #0
	adds r3, r2, #0
	adds r3, #100
	movs r1, #0
	ldrsh r3, [r3, r1]
	movs r0, #9
	str r3, [r5, #16]
	adds r3, r2, #0
	adds r3, #102
	movs r1, #0
	ldrsh r3, [r3, r1]
	str r3, [r5, #20]
	adds r3, r2, #0
	adds r3, #98
	ldrb r3, [r3]
	str r3, [r5, #24]
	adds r3, r2, #0
	adds r3, #99
	ldrb r3, [r3]
	str r3, [r5, #28]
	bl Object_GetById
	adds r2, r0, #0
	adds r3, r2, #0
	adds r3, #100
	movs r1, #0
	ldrsh r3, [r3, r1]
	movs r0, #11
	str r3, [r5, #32]
	adds r3, r2, #0
	adds r3, #102
	movs r1, #0
	ldrsh r3, [r3, r1]
	str r3, [r5, #36]
	adds r3, r2, #0
	adds r3, #98
	ldrb r3, [r3]
	str r3, [r5, #40]
	adds r3, r2, #0
	adds r3, #99
	ldrb r3, [r3]
	str r3, [r5, #44]
	bl Object_GetById
	adds r2, r0, #0
	adds r3, r2, #0
	adds r3, #100
	movs r1, #0
	ldrsh r3, [r3, r1]
	str r3, [r5, #48]
	adds r3, r2, #0
	adds r3, #102
	movs r1, #0
	ldrsh r3, [r3, r1]
	str r3, [r5, #52]
	adds r3, r2, #0
	adds r3, #98
	ldrb r3, [r3]
	str r3, [r5, #56]
	adds r3, r2, #0
	adds r3, #99
	ldrb r3, [r3]
	movs r2, #133
	str r3, [r5, #60]
	ldr r3, .L_02009824
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	adds r2, r0, #0
	adds r3, r2, #0
	adds r3, #100
	movs r1, #0
	ldrsh r3, [r3, r1]
	ldr r0, .L_02009828
	str r3, [r5, #64]
	adds r3, r2, #0
	adds r3, #102
	movs r1, #0
	ldrsh r3, [r3, r1]
	str r3, [r5, #68]
	adds r3, r2, #0
	adds r3, #98
	ldrb r3, [r3]
	str r3, [r5, #72]
	adds r3, r2, #0
	adds r3, #99
	ldrb r3, [r3]
	str r3, [r5, #76]
	bl Scheduler_RemoveCallbackFar
	pop {r5, pc}
.L_02009820:
	.4byte gOverlayArea + 0x2fc0
.L_02009824:
	.4byte gPartyState
.L_02009828:
	.4byte Func_02001688
	.section .text.x0200982c,"ax",%progbits
	.global Func_0200182c
	.thumb_func
Func_0200182c:
	push {r5, lr}
	movs r0, #8
	bl Object_GetById
	ldr r5, .L_020098f8
	adds r1, r0, #0
	ldr r2, [r5]
	adds r3, r1, #0
	adds r3, #100
	strh r2, [r3]
	adds r2, r1, #0
	ldr r3, [r5, #4]
	adds r2, #102
	strh r3, [r2]
	subs r2, #4
	ldr r3, [r5, #8]
	movs r0, #10
	strb r3, [r2]
	adds r2, #1
	ldr r3, [r5, #12]
	strb r3, [r2]
	bl Object_GetById
	ldr r2, [r5, #16]
	adds r1, r0, #0
	adds r3, r1, #0
	adds r3, #100
	strh r2, [r3]
	adds r2, r1, #0
	ldr r3, [r5, #20]
	adds r2, #102
	strh r3, [r2]
	subs r2, #4
	ldr r3, [r5, #24]
	movs r0, #9
	strb r3, [r2]
	adds r2, #1
	ldr r3, [r5, #28]
	strb r3, [r2]
	bl Object_GetById
	ldr r2, [r5, #32]
	adds r1, r0, #0
	adds r3, r1, #0
	adds r3, #100
	strh r2, [r3]
	adds r2, r1, #0
	ldr r3, [r5, #36]
	adds r2, #102
	strh r3, [r2]
	subs r2, #4
	ldr r3, [r5, #40]
	movs r0, #11
	strb r3, [r2]
	adds r2, #1
	ldr r3, [r5, #44]
	strb r3, [r2]
	bl Object_GetById
	ldr r2, [r5, #48]
	adds r1, r0, #0
	adds r3, r1, #0
	adds r3, #100
	strh r2, [r3]
	adds r2, r1, #0
	ldr r3, [r5, #52]
	adds r2, #102
	strh r3, [r2]
	subs r2, #4
	ldr r3, [r5, #56]
	strb r3, [r2]
	adds r2, #1
	ldr r3, [r5, #60]
	strb r3, [r2]
	ldr r3, .L_020098fc
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	ldr r2, [r5, #64]
	adds r1, r0, #0
	adds r3, r1, #0
	adds r3, #100
	strh r2, [r3]
	adds r2, r1, #0
	ldr r3, [r5, #68]
	adds r2, #102
	strh r3, [r2]
	subs r2, #4
	ldr r3, [r5, #72]
	movs r1, #144
	strb r3, [r2]
	adds r2, #1
	ldr r3, [r5, #76]
	lsls r1, r1, #3
	strb r3, [r2]
	ldr r0, .L_02009900
	bl Scheduler_AddOrUpdateCallback
	pop {r5, pc}
.L_020098f8:
	.4byte gOverlayArea + 0x2fc0
.L_020098fc:
	.4byte gPartyState
.L_02009900:
	.4byte Func_02001688
	.section .text.x02009904,"ax",%progbits
	.global Func_02001904
	.thumb_func
Func_02001904:
	push {r5, r6, lr}
	adds r5, r0, #0
	bl Object_GetById
	ldr r2, .L_02009940
	adds r6, r0, #0
	ldr r1, .L_02009940
	adds r0, r5, #0
	bl ObjectMotion_SetSpeedParameters
	adds r0, r5, #0
	movs r1, #2
	bl Object_SetModeById
	adds r2, r6, #0
	movs r3, #0
	adds r2, #100
	strh r3, [r2]
	adds r2, #2
	strh r3, [r2]
	ldr r1, .L_0200993c
	subs r2, #4
	movs r3, #1
	strb r3, [r2]
	adds r3, r6, #0
	adds r3, #99
	strb r1, [r3]
	pop {r5, r6, pc}
.L_0200993c:
	.4byte 0x00000000
.L_02009940:
	.4byte 0x00013333
	.section .text.x02009944,"ax",%progbits
	.global Func_02001944
	.thumb_func
Func_02001944:
	push {r5, r6, r7, lr}
	movs r0, #8
	bl Object_GetById
	adds r7, r0, #0
	ldr r0, .L_02009cd8
	bl Func_02002520
	movs r1, #0
	movs r0, #8
	bl Func_02002538
	movs r0, #9
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #10
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #9
	bl Object_GetById
	movs r5, #0
	str r5, [r0, #108]
	movs r0, #10
	bl Object_GetById
	str r5, [r0, #108]
	movs r0, #1
	bl WaitFrames
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #9
	ldr r1, .L_02009cdc
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r2, #204
	lsls r2, r2, #8
	adds r2, #204
	movs r0, #10
	ldr r1, .L_02009cdc
	bl ObjectMotion_SetSpeedParameters
	ldr r1, .L_02009ce0
	movs r0, #9
	bl ObjectMotion_EnableActionAndSetCallback
	ldr r1, .L_02009ce4
	movs r0, #10
	bl ObjectMotion_EnableActionAndSetCallback
	bl Func_02002570
	adds r0, #85
	strb r5, [r0]
	movs r1, #200
	movs r0, #204
	lsls r0, r0, #8
	lsls r1, r1, #5
	adds r0, #204
	adds r1, #153
	bl Func_02002560
	movs r0, #152
	movs r1, #1
	lsls r0, r0, #17
	negs r1, r1
	ldr r2, .L_02009ce8
	movs r3, #1
	bl Motion_CamBounds
	ldr r3, .L_02009cec
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r3, r2
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	ldr r0, [r5]
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #148
	movs r2, #218
	lsls r2, r2, #2
	ldr r0, [r5]
	lsls r1, r1, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #0
	ldr r0, [r5]
	bl Func_02002548
	movs r0, #78
	bl Func_020025a0
	movs r1, #176
	movs r2, #20
	movs r0, #8
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r1, #208
	movs r0, #8
	lsls r1, r1, #8
	bl Func_02002548
	movs r0, #8
	movs r1, #2
	movs r2, #10
	bl ObjectMotion_Launch
	movs r2, #10
	movs r0, #8
	movs r1, #4
	bl ObjectMotion_Launch
	movs r0, #8
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #8
	ldr r1, .L_02009cdc
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r2, #204
	lsls r2, r2, #8
	adds r2, #204
	movs r0, #11
	ldr r1, .L_02009cdc
	bl ObjectMotion_SetSpeedParameters
	ldr r1, .L_02009cf0
	movs r0, #8
	bl ObjectMotion_EnableActionAndSetCallback
	ldr r1, .L_02009cf4
	movs r0, #9
	bl ObjectMotion_EnableActionAndSetCallback
	ldr r1, .L_02009cf8
	movs r0, #10
	bl ObjectMotion_EnableActionAndSetCallback
	ldr r1, .L_02009cfc
	movs r0, #11
	bl Object_SetActionCallbackAndRefreshById
	movs r1, #148
	movs r2, #218
	lsls r1, r1, #17
	lsls r2, r2, #18
	movs r0, #5
	bl Func_020024f8
	movs r0, #1
	bl WaitFrames
	movs r1, #224
	ldr r0, [r5]
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #5
	ldr r1, .L_02009d00
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r2, #192
	movs r1, #156
	lsls r2, r2, #2
	adds r2, #86
	movs r0, #5
	lsls r1, r1, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #192
	movs r0, #5
	lsls r1, r1, #7
	bl Func_02002548
	movs r0, #5
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	ldr r0, [r5]
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #5
	movs r1, #2
	bl Object_SetModeById
	ldr r0, [r5]
	bl Object_GetById
	cmp r0, #0
	beq .L_02009af0
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #5
	bl ObjectMotion_ResetAndSetPosition
.L_02009af0:
	movs r0, #5
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl Func_020024f8
	movs r2, #204
	lsls r2, r2, #8
	ldr r0, [r5]
	ldr r1, .L_02009cdc
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r1, #158
	movs r2, #224
	ldr r0, [r5]
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_SetPositionAndReset
	movs r1, #172
	movs r2, #228
	lsls r2, r2, #2
	ldr r0, [r5]
	lsls r1, r1, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #192
	ldr r0, [r5]
	lsls r1, r1, #8
	bl Func_02002548
	movs r0, #17
	bl Func_020025a0
	movs r1, #192
	movs r0, #8
	lsls r1, r1, #8
	bl Func_02002548
	movs r0, #8
	movs r1, #0
	bl Func_02002538
	movs r0, #8
	movs r1, #1
	bl Object_AttachWorkTargetToObject
	ldr r1, .L_02009d04
	movs r0, #8
	bl ObjectMotion_EnableActionAndSetCallback
	ldr r1, .L_02009d08
	movs r0, #10
	bl ObjectMotion_EnableActionAndSetCallback
	ldr r1, .L_02009d0c
	movs r0, #9
	bl Object_SetActionCallbackAndRefreshById
	movs r2, #192
	movs r1, #172
	lsls r2, r2, #2
	adds r2, #122
	movs r0, #11
	lsls r1, r1, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #192
	lsls r1, r1, #8
	movs r0, #11
	bl Func_02002548
	movs r0, #8
	bl Func_02001904
	movs r0, #10
	bl Func_02001904
	movs r0, #9
	bl Func_02001904
	movs r0, #11
	bl Func_02001904
	ldr r0, [r5]
	bl Func_02001904
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_02009d10
	bl Scheduler_AddOrUpdateCallback
	adds r5, r7, #0
	movs r0, #8
	movs r1, #0
	bl Func_02002538
	adds r5, #102
	movs r3, #1
	strh r3, [r5]
.L_02009bbe:
	adds r6, r7, #0
	movs r0, #1
	adds r6, #100
	bl WaitFrames
	movs r2, #0
	ldrsh r3, [r6, r2]
	cmp r3, #34
	bne .L_02009bbe
	movs r3, #2
	strh r3, [r5]
	movs r0, #1
	bl WaitFrames
	movs r0, #8
	movs r1, #0
	bl Func_02002538
	movs r3, #1
	strh r3, [r5]
.L_02009be6:
	movs r0, #1
	bl WaitFrames
	movs r2, #0
	ldrsh r3, [r6, r2]
	cmp r3, #42
	bne .L_02009be6
	movs r3, #2
	strh r3, [r5]
	movs r0, #1
	bl WaitFrames
	movs r0, #8
	movs r1, #0
	bl Func_02002538
	movs r3, #1
	strh r3, [r5]
.L_02009c0a:
	movs r0, #1
	bl WaitFrames
	movs r2, #0
	ldrsh r3, [r6, r2]
	cmp r3, #108
	bne .L_02009c0a
	movs r3, #3
	strh r3, [r5]
	movs r0, #1
	bl WaitFrames
	movs r0, #8
	movs r1, #0
	bl Func_02002538
	bl Func_02001730
	movs r0, #8
	movs r1, #4
	movs r2, #0
	bl ObjectMotion_Launch
	movs r0, #10
	movs r1, #4
	movs r2, #0
	bl ObjectMotion_Launch
	movs r0, #9
	movs r1, #4
	movs r2, #0
	bl ObjectMotion_Launch
	movs r0, #11
	movs r1, #4
	movs r2, #40
	bl ObjectMotion_Launch
	ldr r5, .L_02009cec
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	movs r1, #192
	ldr r0, [r5]
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	movs r0, #8
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	movs r0, #10
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r2, #0
	movs r0, #9
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #11
	lsls r1, r1, #8
	bl Func_02002548
	movs r1, #0
	movs r0, #8
	bl UiText_OpenMessageAtObject
	ldr r0, [r5]
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_02009cb2
	bl Func_02001d14
	b .L_02009cd6
.L_02009cb2:
	movs r1, #0
	movs r2, #20
	movs r0, #8
	bl Func_02002530
	bl Event_ClearStatus1c6
	bl Event_WaitValue1c8Frames
	movs r0, #123
	bl Func_020025a0
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #15
	bl Func_02002578
.L_02009cd6:
	pop {r5, r6, r7, pc}
.L_02009cd8:
	.4byte 0x00002254
.L_02009cdc:
	.4byte 0x00019999
.L_02009ce0:
	.4byte Data_0200277c
.L_02009ce4:
	.4byte Data_020027c0
.L_02009ce8:
	.4byte 0x036e0000
.L_02009cec:
	.4byte gPartyState
.L_02009cf0:
	.4byte Data_02002804
.L_02009cf4:
	.4byte Data_02002870
.L_02009cf8:
	.4byte Data_020028dc
.L_02009cfc:
	.4byte Data_02002948
.L_02009d00:
	.4byte 0x00013333
.L_02009d04:
	.4byte Data_02002998
.L_02009d08:
	.4byte Data_020029c0
.L_02009d0c:
	.4byte Data_02002a04
.L_02009d10:
	.4byte Func_02001688
	.section .text.x02009d14,"ax",%progbits
	.global Func_02001d14
	.thumb_func
Func_02001d14:
	push {r5, r6, r7, lr}
	movs r0, #8
	bl Object_GetById
	movs r1, #128
	adds r7, r0, #0
	lsls r1, r1, #8
	movs r0, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #10
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #176
	movs r0, #9
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #176
	movs r0, #11
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	ldr r3, .L_02009eb8
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	movs r1, #0
	bl Func_02002548
	ldr r0, .L_02009ebc
	bl Func_02002520
	adds r5, r7, #0
	movs r0, #8
	movs r1, #0
	bl Func_02002538
	adds r5, #102
	bl Func_0200182c
	movs r3, #1
	strh r3, [r5]
.L_02009d78:
	adds r6, r7, #0
	movs r0, #1
	adds r6, #100
	bl WaitFrames
	movs r2, #0
	ldrsh r3, [r6, r2]
	cmp r3, #114
	bne .L_02009d78
	movs r3, #2
	strh r3, [r5]
	movs r0, #1
	bl WaitFrames
	movs r0, #8
	movs r1, #0
	bl Func_02002538
	movs r3, #1
	strh r3, [r5]
.L_02009da0:
	movs r0, #1
	bl WaitFrames
	movs r2, #0
	ldrsh r3, [r6, r2]
	cmp r3, #150
	bne .L_02009da0
	movs r3, #2
	strh r3, [r5]
	movs r0, #1
	bl WaitFrames
	movs r0, #8
	movs r1, #0
	bl Func_02002538
	movs r3, #1
	strh r3, [r5]
.L_02009dc4:
	movs r0, #1
	bl WaitFrames
	movs r2, #0
	ldrsh r3, [r6, r2]
	cmp r3, #154
	bne .L_02009dc4
	movs r3, #2
	strh r3, [r5]
	movs r0, #1
	bl WaitFrames
	movs r0, #8
	movs r1, #0
	bl Func_02002538
	movs r3, #1
	strh r3, [r5]
.L_02009de8:
	movs r0, #1
	bl WaitFrames
	movs r2, #0
	ldrsh r3, [r6, r2]
	cmp r3, #216
	bne .L_02009de8
	movs r3, #3
	strh r3, [r5]
	movs r0, #1
	bl WaitFrames
	movs r0, #8
	movs r1, #0
	bl Func_02002538
	bl Func_02001730
	movs r0, #8
	movs r1, #4
	movs r2, #0
	bl ObjectMotion_Launch
	movs r0, #10
	movs r1, #4
	movs r2, #0
	bl ObjectMotion_Launch
	movs r0, #9
	movs r1, #4
	movs r2, #0
	bl ObjectMotion_Launch
	movs r0, #11
	movs r1, #4
	movs r2, #40
	bl ObjectMotion_Launch
	ldr r5, .L_02009eb8
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	movs r1, #128
	ldr r0, [r5]
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #208
	movs r0, #8
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #208
	movs r0, #10
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #208
	movs r2, #0
	movs r0, #9
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r1, #208
	movs r0, #11
	lsls r1, r1, #8
	bl Func_02002548
	movs r1, #0
	movs r0, #8
	bl UiText_OpenMessageAtObject
	ldr r0, [r5]
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_02009e90
	bl Func_02001ec0
	b .L_02009eb4
.L_02009e90:
	movs r1, #0
	movs r2, #20
	movs r0, #8
	bl Func_02002530
	bl Event_ClearStatus1c6
	bl Event_WaitValue1c8Frames
	movs r0, #123
	bl Func_020025a0
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #15
	bl Func_02002578
.L_02009eb4:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009eb8:
	.4byte gPartyState
.L_02009ebc:
	.4byte 0x0000225c
	.section .text.x02009ec0,"ax",%progbits
	.global Func_02001ec0
	.thumb_func
Func_02001ec0:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r0, #8
	bl Object_GetById
	movs r1, #192
	adds r6, r0, #0
	lsls r1, r1, #6
	movs r0, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #9
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	movs r0, #11
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	ldr r3, .L_0200a234
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	movs r1, #192
	ldr r0, [r3]
	lsls r1, r1, #7
	bl Func_02002548
	ldr r0, .L_0200a238
	bl Func_02002520
	adds r5, r6, #0
	movs r0, #8
	movs r1, #0
	bl Func_02002538
	adds r5, #102
	bl Func_0200182c
	movs r3, #1
	strh r3, [r5]
.L_02009f28:
	adds r7, r6, #0
	movs r0, #1
	adds r7, #100
	bl WaitFrames
	movs r2, #0
	ldrsh r3, [r7, r2]
	cmp r3, #218
	bne .L_02009f28
	movs r3, #2
	strh r3, [r5]
	movs r0, #1
	bl WaitFrames
	movs r0, #8
	movs r1, #0
	bl Func_02002538
	movs r3, #1
	strh r3, [r5]
.L_02009f50:
	movs r0, #1
	bl WaitFrames
	movs r2, #0
	ldrsh r3, [r7, r2]
	cmp r3, #254
	bne .L_02009f50
	movs r3, #2
	strh r3, [r5]
	movs r0, #1
	bl WaitFrames
	movs r0, #8
	movs r1, #0
	bl Func_02002538
	movs r3, #1
	strh r3, [r5]
.L_02009f74:
	movs r0, #1
	bl WaitFrames
	movs r2, #0
	ldrsh r3, [r7, r2]
	movs r2, #129
	lsls r2, r2, #1
	cmp r3, r2
	bne .L_02009f74
	movs r3, #2
	strh r3, [r5]
	movs r0, #1
	bl WaitFrames
	movs r0, #8
	movs r1, #0
	bl Func_02002538
	movs r3, #1
	strh r3, [r5]
.L_02009f9c:
	movs r0, #1
	bl WaitFrames
	movs r2, #0
	ldrsh r3, [r7, r2]
	movs r2, #162
	lsls r2, r2, #1
	cmp r3, r2
	bne .L_02009f9c
	movs r3, #3
	strh r3, [r5]
	movs r0, #1
	bl WaitFrames
	movs r0, #8
	movs r1, #0
	bl Func_02002538
	bl Func_02001730
	movs r1, #160
	lsls r1, r1, #7
	movs r0, #8
	bl Func_02002548
	movs r0, #8
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r5, #254
	adds r3, r5, #0
	ands r3, r2
	strb r3, [r0]
	movs r0, #8
	bl Object_GetById
	adds r6, r0, #0
	ldr r3, [r6, #16]
	movs r0, #8
	ldr r1, .L_0200a23c
	ldr r2, .L_0200a240
	mov r8, r3
	ldr r7, [r6, #8]
	bl ObjectMotion_SetSpeedParameters
	movs r0, #8
	movs r1, #6
	movs r2, #0
	bl ObjectMotion_Launch
	movs r2, #128
	lsls r2, r2, #2
	movs r0, #8
	movs r1, #200
	adds r2, #158
	bl ObjectMotion_SetPositionAndCommit
	movs r0, #8
	movs r1, #4
	movs r2, #0
	bl ObjectMotion_Launch
	mov r3, r8
	movs r2, #0
	adds r1, r7, #0
	adds r0, r6, #0
	bl Func_02002450
	movs r0, #5
	bl Battle_WaitMode0
	movs r1, #160
	lsls r1, r1, #7
	movs r0, #10
	bl Func_02002548
	movs r0, #10
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	adds r3, r5, #0
	ands r3, r2
	strb r3, [r0]
	movs r0, #10
	bl Object_GetById
	adds r6, r0, #0
	ldr r2, [r6, #16]
	movs r0, #10
	mov r8, r2
	ldr r1, .L_0200a23c
	ldr r2, .L_0200a240
	ldr r7, [r6, #8]
	bl ObjectMotion_SetSpeedParameters
	movs r0, #10
	movs r1, #6
	movs r2, #0
	bl ObjectMotion_Launch
	movs r2, #128
	lsls r2, r2, #2
	movs r0, #10
	movs r1, #200
	adds r2, #158
	bl ObjectMotion_SetPositionAndCommit
	movs r0, #10
	movs r1, #4
	movs r2, #0
	bl ObjectMotion_Launch
	mov r3, r8
	movs r2, #0
	adds r1, r7, #0
	adds r0, r6, #0
	bl Func_02002450
	movs r0, #5
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #8
	movs r0, #9
	bl Func_02002548
	movs r0, #9
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	adds r3, r5, #0
	ands r3, r2
	strb r3, [r0]
	movs r0, #9
	bl Object_GetById
	adds r6, r0, #0
	ldr r3, [r6, #16]
	movs r0, #9
	ldr r1, .L_0200a23c
	ldr r2, .L_0200a240
	mov r8, r3
	ldr r7, [r6, #8]
	bl ObjectMotion_SetSpeedParameters
	movs r0, #9
	movs r1, #6
	movs r2, #0
	bl ObjectMotion_Launch
	movs r2, #128
	lsls r2, r2, #2
	movs r0, #9
	movs r1, #200
	adds r2, #158
	bl ObjectMotion_SetPositionAndCommit
	movs r0, #9
	movs r1, #4
	movs r2, #0
	bl ObjectMotion_Launch
	movs r2, #0
	mov r3, r8
	adds r1, r7, #0
	adds r0, r6, #0
	bl Func_02002450
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #176
	lsls r1, r1, #8
	movs r0, #11
	bl Func_02002548
	movs r0, #11
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	ands r5, r3
	strb r5, [r0]
	movs r0, #11
	bl Object_GetById
	adds r6, r0, #0
	ldr r2, [r6, #16]
	movs r0, #11
	mov r8, r2
	ldr r1, .L_0200a23c
	ldr r2, .L_0200a240
	ldr r7, [r6, #8]
	bl ObjectMotion_SetSpeedParameters
	movs r0, #11
	movs r1, #6
	movs r2, #0
	bl ObjectMotion_Launch
	movs r2, #128
	lsls r2, r2, #2
	movs r0, #11
	movs r1, #200
	adds r2, #158
	bl ObjectMotion_SetPositionAndCommit
	movs r0, #11
	movs r1, #4
	movs r2, #0
	bl ObjectMotion_Launch
	adds r1, r7, #0
	mov r3, r8
	adds r0, r6, #0
	movs r2, #0
	bl Func_02002450
	movs r0, #11
	bl ObjectMotion_CommitCurrentPositionAndActivate
	ldr r5, .L_0200a234
	movs r3, #133
	lsls r3, r3, #2
	movs r1, #204
	movs r2, #204
	adds r5, r5, r3
	lsls r1, r1, #8
	lsls r2, r2, #7
	ldr r0, [r5]
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r2, #128
	lsls r2, r2, #2
	ldr r0, [r5]
	movs r1, #184
	adds r2, #174
	bl ObjectMotion_SetPositionAndReset
	movs r2, #128
	lsls r2, r2, #2
	ldr r0, [r5]
	movs r1, #186
	adds r2, #162
	bl ObjectMotion_SetPositionAndReset
	movs r1, #224
	lsls r1, r1, #8
	movs r2, #0
	ldr r0, [r5]
	bl ObjectMotion_ArmCallback
	movs r0, #8
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	movs r5, #1
	orrs r3, r5
	strb r3, [r0]
	movs r0, #10
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
	movs r0, #9
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
	movs r0, #11
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	movs r1, #160
	orrs r5, r3
	strb r5, [r0]
	lsls r1, r1, #7
	movs r0, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #10
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #9
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #11
	lsls r1, r1, #8
	movs r2, #20
	bl ObjectMotion_ArmCallback
	movs r1, #0
	movs r2, #20
	movs r0, #8
	bl Func_02002530
	bl Event_ClearStatus1c6
	bl Event_WaitValue1c8Frames
	movs r0, #123
	bl Func_020025a0
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #15
	bl Func_02002578
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200a234:
	.4byte gPartyState
.L_0200a238:
	.4byte 0x00002263
.L_0200a23c:
	.4byte 0x00026666
.L_0200a240:
	.4byte 0x00013333
	.section .text.x0200a244,"ax",%progbits
	.global Func_02002244
	.thumb_func
Func_02002244:
	push {lr}
	movs r0, #192
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a274
	movs r0, #9
	bl Object_GetById
	movs r3, #168
	lsls r3, r3, #16
	str r3, [r0, #8]
	movs r0, #9
	bl Object_GetById
	movs r3, #0
	str r3, [r0, #12]
	movs r0, #9
	bl Object_GetById
	movs r3, #192
	lsls r3, r3, #16
	str r3, [r0, #16]
.L_0200a274:
	pop {pc}
	.2byte 0x0000
	.section .text.x0200a278,"ax",%progbits
	.global Func_02002278
	.thumb_func
Func_02002278:
	push {lr}
	ldr r1, .L_0200a2a8
	movs r0, #240
	lsls r0, r0, #1
	adds r3, r1, r0
	movs r0, #0
	ldrsh r2, [r3, r0]
	ldr r3, .L_0200a2ac
	cmp r2, r3
	bne .L_0200a2a4
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r1, r2
	movs r0, #0
	ldrsh r3, [r3, r0]
	cmp r3, #15
	bne .L_0200a2a0
	bl Func_020023c8
	b .L_0200a2a4
.L_0200a2a0:
	bl Func_02002244
.L_0200a2a4:
	movs r0, #0
	pop {pc}
.L_0200a2a8:
	.4byte gPartyState
.L_0200a2ac:
	.4byte 0x0000008b
	.section .text.x0200a2b0,"ax",%progbits
	.global Func_020022b0
	.thumb_func
Func_020022b0:
	push {lr}
	ldr r3, .L_0200a2ec
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_0200a2f0
	sub sp, #8
	cmp r2, r3
	bne .L_0200a2e6
	movs r0, #192
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a2e6
	movs r3, #10
	movs r2, #12
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #8
	movs r1, #12
	movs r2, #1
	movs r3, #1
	bl Func_02002470
.L_0200a2e6:
	movs r0, #0
	add sp, #8
	pop {pc}
.L_0200a2ec:
	.4byte gPartyState
.L_0200a2f0:
	.4byte 0x0000008b
	.global Data_020022f4
Data_020022f4:
	.4byte 0x00004770
	.section .text.x0200a2f8,"ax",%progbits
	.global Func_020022f8
	.thumb_func
Func_020022f8:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r0, #9
	sub sp, #8
	bl Object_GetById
	adds r6, r0, #0
	ldr r2, [r6, #80]
	movs r0, #192
	lsls r0, r0, #2
	mov r8, r2
	bl GameFlag_SetBit
	movs r3, #192
	lsls r3, r3, #9
	str r3, [r6, #52]
	movs r3, #128
	lsls r3, r3, #12
	str r3, [r6, #48]
	movs r0, #227
	bl Func_020025a0
	movs r1, #176
	movs r2, #160
	movs r3, #184
	adds r0, r6, #0
	lsls r1, r1, #16
	lsls r2, r2, #14
	lsls r3, r3, #16
	bl Func_02002450
	movs r7, #0
	movs r5, #15
.L_0200a33c:
	mov r2, r8
	ldrh r3, [r2, #18]
	movs r0, #1
	subs r3, r3, r7
	strh r3, [r2, #18]
	subs r5, #1
	bl WaitFrames
	adds r7, #40
	cmp r5, #0
	bge .L_0200a33c
	movs r1, #168
	movs r2, #128
	movs r3, #184
	adds r0, r6, #0
	lsls r1, r1, #16
	lsls r2, r2, #13
	lsls r3, r3, #16
	bl Func_02002450
	movs r7, #200
	lsls r7, r7, #1
	movs r5, #25
.L_0200a36a:
	mov r2, r8
	ldrh r3, [r2, #18]
	movs r0, #1
	subs r3, r3, r7
	strh r3, [r2, #18]
	subs r5, #1
	bl WaitFrames
	adds r7, #40
	cmp r5, #0
	bge .L_0200a36a
	adds r0, r6, #0
	bl Func_02002458
	movs r0, #2
	bl Battle_WaitMode0
	movs r0, #240
	bl Func_020025a0
	movs r5, #0
	mov r3, r8
	strh r5, [r3, #18]
	movs r3, #168
	lsls r3, r3, #16
	str r3, [r6, #8]
	movs r3, #192
	lsls r3, r3, #16
	str r3, [r6, #16]
	movs r2, #12
	movs r3, #10
	str r5, [r6, #12]
	str r5, [r6, #40]
	str r5, [r6, #36]
	movs r0, #8
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #12
	movs r2, #1
	movs r3, #1
	bl Func_02002470
	add sp, #8
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x0200a3c8,"ax",%progbits
	.global Func_020023c8
	.thumb_func
Func_020023c8:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	movs r0, #9
	bl Object_GetById
	adds r1, r0, #0
	adds r2, r1, #0
	movs r3, #0
	ldr r5, .L_0200a40c
	adds r2, #100
	ldr r6, .L_0200a408
	mov r8, r3
	movs r3, #1
	strh r3, [r2]
	adds r3, r1, #0
	adds r3, #99
	strb r6, [r3]
	movs r0, #10
	str r5, [r1, #108]
	bl Object_GetById
	adds r2, r0, #0
	adds r3, r2, #0
	adds r3, #100
	mov r1, r8
	strh r1, [r3]
	subs r3, #1
	strb r6, [r3]
	str r5, [r2, #108]
	b .L_0200a410
	.2byte 0x0000
.L_0200a408:
	.4byte 0x00000000
.L_0200a40c:
	.4byte Func_0200014c
.L_0200a410:
	pop {r3}
	mov r8, r3
	pop {r5, r6, pc}
	.2byte 0x0000
	.section .rodata.x0200a5a8,"a",%progbits
.L_0200a5a8:
	.4byte 0x00000027
	.4byte 0x00000003
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000000
	.4byte 0x00000017
	.4byte 0x00000006
	.4byte 0x00020000
	.4byte 0x0000002e
	.4byte Func_02000044
	.4byte 0x00000011
.L_0200a5d4:
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000000
	.4byte 0x00000017
	.4byte 0x00000006
	.4byte 0xfff00000
	.4byte 0x0000002e
	.4byte Func_02000044
	.4byte 0x00000011
.L_0200a5f8:
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00009999
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x00009999
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000000
	.4byte 0x00000017
	.4byte 0x00000006
	.4byte 0xfff00000
	.4byte 0x0000002e
	.4byte Func_02000044
	.4byte 0x00000011
.L_0200a634:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00010000
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00006666
	.4byte 0x00000004
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x02f00000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x02f00000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x03080000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x03180000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x03180000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x03080000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0x00000000
.L_0200a6d8:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00010000
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00006666
	.4byte 0x00000004
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x03180000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x03180000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x03080000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x02f00000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x02f00000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x03080000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0x00000000
	.global Data_0200277c
Data_0200277c:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01500000
	.4byte 0x00000000
	.4byte 0x03180000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00005000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000011
	.global Data_020027c0
Data_020027c0:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01600000
	.4byte 0x00000000
	.4byte 0x03180000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00005000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000011
	.global Data_02002804
Data_02002804:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x013c0000
	.4byte 0x00000000
	.4byte 0x03780000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01500000
	.4byte 0x00000000
	.4byte 0x03780000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x03680000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000011
	.global Data_02002870
Data_02002870:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x016a0000
	.4byte 0x00000000
	.4byte 0x03180000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01800000
	.4byte 0x00000000
	.4byte 0x03500000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x017e0000
	.4byte 0x00000000
	.4byte 0x03780000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000011
	.global Data_020028dc
Data_020028dc:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x016c0000
	.4byte 0x00000000
	.4byte 0x03280000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01760000
	.4byte 0x00000000
	.4byte 0x036e0000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x03780000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000011
	.global Data_02002948
Data_02002948:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01120000
	.4byte 0x00000000
	.4byte 0x03780000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x03780000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000011
	.global Data_02002998
Data_02002998:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x03480000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000011
	.global Data_020029c0
Data_020029c0:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x03780000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x03580000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.global Data_02002a04
Data_02002a04:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x03780000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x036a0000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
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
	.4byte 0x0000008b
	.4byte 0x0010108d
	.4byte 0x0020208d
	.4byte 0x0030108e
	.4byte 0x0040408d
	.4byte 0x0050308e
	.4byte 0x0060208e
	.4byte 0x0070308d
	.4byte 0x0080403a
	.4byte 0x0090608c
	.4byte 0x00a22002
	.4byte 0x00b21002
	.4byte 0x00c0f08b
	.4byte 0x00d0708c
	.4byte 0x00f0c08b
	.4byte 0x0000008c
	.4byte 0x0060908b
	.4byte 0x0070d08b
	.4byte 0x000001ff
	.global Data_02002ac4
Data_02002ac4:
	.4byte 0xffff0146
	.4byte .L_0200a5d4
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00008000
	.4byte 0xffff0146
	.4byte .L_0200a5a8
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00008000
	.4byte 0xffff0147
	.4byte .L_0200a5f8
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00008000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x02680000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00032000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x02c80000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00014000
	.4byte 0xffff0072
	.4byte 0x00000003
	.4byte 0x02280000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00016000
	.4byte 0xffff0080
	.4byte 0x00000002
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x0001a000
	.4byte 0xffff0070
	.4byte 0x00000001
	.4byte 0x01c80000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00012000
	.4byte 0xffff0071
	.4byte 0x00000001
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x0001e000
	.4byte 0xffff00e2
	.4byte 0x00000001
	.4byte 0x01d00000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00016000
	.4byte 0xffff00e2
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00014000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02002be4
Data_02002be4:
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02002bfc
Data_02002bfc:
	.4byte 0xffff0088
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x03680000
	.4byte 0x00018000
	.4byte 0xffff0088
	.4byte .L_0200a634
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x03080000
	.4byte 0x0002c000
	.4byte 0xffff008a
	.4byte .L_0200a6d8
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x03080000
	.4byte 0x00024000
	.4byte 0xffff008a
	.4byte 0x00000001
	.4byte 0x01100000
	.4byte 0x00000000
	.4byte 0x03200000
	.4byte 0x0000b000
	.4byte 0xffff0005
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
.L_0200ac8c:
	.4byte 0x001c005d
	.4byte 0x00020001
	.4byte 0x005c0006
	.4byte 0x0001001c
	.4byte 0x00060002
	.2byte 0xffff
.L_0200aca2:
	.2byte 0x005f
	.4byte 0x0001001c
	.4byte 0x00060002
	.4byte 0x001c005e
	.4byte 0x00020001
	.4byte 0xffff0006
.L_0200acb8:
	.4byte 0x001c0059
	.4byte 0x00020001
	.4byte 0x005a0006
	.4byte 0x0001001c
	.4byte 0x00060002
	.2byte 0xffff
.L_0200acce:
	.2byte 0x005c
	.4byte 0x0002001e
	.4byte 0x00060002
	.4byte 0x001e005e
	.4byte 0x00020002
	.4byte 0xffff0006
	.global Data_02002ce4
Data_02002ce4:
	.4byte .L_0200ac8c
	.4byte 0x00130062
	.4byte .L_0200aca2
	.4byte 0x0014005c
	.4byte .L_0200acb8
	.4byte 0x00120054
	.4byte .L_0200ac8c
	.4byte 0x0007004f
	.4byte .L_0200acb8
	.4byte 0x00100050
	.4byte .L_0200acb8
	.4byte 0x0011004d
	.4byte .L_0200aca2
	.4byte 0x00080063
	.4byte .L_0200acce
	.4byte 0x0006005a
	.global Data_02002d24
Data_02002d24:
	.4byte 0x0000c602
	.4byte 0xffff0001
	.4byte Func_02000270
	.4byte 0x0000c602
	.4byte 0xffff0002
	.4byte Func_02000270
	.4byte 0x0000c602
	.4byte 0xffff0003
	.4byte Func_02000270
	.4byte 0x0000c602
	.4byte 0xffff0004
	.4byte Func_02000270
	.4byte 0x0000c602
	.4byte 0xffff0005
	.4byte Func_02000270
	.4byte 0x0000c602
	.4byte 0xffff0006
	.4byte Func_02000270
	.4byte 0x0000c602
	.4byte 0xffff0007
	.4byte Func_02000270
	.4byte 0x0000c602
	.4byte 0xffff0008
	.4byte Func_02000270
	.4byte 0x0000c401
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000001
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00000001
	.4byte 0xffff000c
	.4byte 0x0000000c
	.4byte 0x00004401
	.4byte 0xffff000d
	.4byte 0x0000000d
	.4byte 0x50008905
	.4byte 0xffff0014
	.4byte Func_02000328
	.4byte 0x50008905
	.4byte 0xffff0015
	.4byte Func_0200036c
	.4byte 0x50008905
	.4byte 0xffff0016
	.4byte Func_0200038c
	.4byte 0x50008905
	.4byte 0xffff0017
	.4byte Func_0200041c
	.4byte 0x50008905
	.4byte 0xffff0018
	.4byte Func_0200043c
	.4byte 0x50008905
	.4byte 0xffff0019
	.4byte Func_020004cc
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x000021f9
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x000021fa
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x000021fb
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x000021fc
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x000021fd
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x000021fe
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x000021ff
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x00002200
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00002201
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00002202
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00002203
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x00002204
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x00002205
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x00002206
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x00002207
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x00002208
	.4byte 0x10008e15
	.4byte gHeapSlots + 0x8
	.4byte Data_020022f4 + 0x1
	.4byte 0x00008e15
	.4byte gHeapSlots + 0x8
	.4byte Func_020022f8
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02002eec
Data_02002eec:
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x0000c401
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02002f10
Data_02002f10:
	.4byte 0x00000001
	.4byte 0xffff000f
	.4byte 0x0000000f
	.4byte 0x00000400
	.4byte 0xffff0008
	.4byte Func_02000544
	.4byte 0x00002400
	.4byte 0xffff0008
	.4byte Func_02000544
	.4byte 0x0000e400
	.4byte 0xffff0008
	.4byte Func_02000544
	.4byte 0x0000c400
	.4byte 0xffff0008
	.4byte Func_02000544
	.4byte 0x00004400
	.4byte 0xffff0008
	.4byte Func_02000544
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00002244
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte Func_02000584
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte Func_02000584
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte Func_02000614
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte Func_02000614
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x00002243
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00002247
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
