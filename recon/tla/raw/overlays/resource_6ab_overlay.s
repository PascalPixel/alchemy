.syntax unified
	.thumb
	.section .text.x020082f4,"ax",%progbits
	.global Func_020002f4
	.thumb_func
Func_020002f4:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r1, .L_02008548
	movs r3, #240
	mov r8, r1
	lsls r3, r3, #1
	add r3, r8
	movs r2, #0
	ldrsh r5, [r3, r2]
	movs r3, #241
	lsls r3, r3, #1
	movs r2, #192
	lsls r2, r2, #18
	add r3, r8
	movs r1, #0
	ldrsh r7, [r3, r1]
	ldr r3, [r2, #108]
	movs r1, #214
	lsls r1, r1, #1
	mov r10, r2
	movs r2, #129
	adds r3, r3, r1
	lsls r2, r2, #2
	str r2, [r3]
	ldr r2, .L_0200854c
	movs r3, #0
	movs r0, #137
	str r3, [r2]
	lsls r0, r0, #1
	sub sp, #8
	bl GameFlag_SetBit
	bl Func_02001d7c
	movs r3, #88
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl Func_02001df0
	ldr r3, .L_02008550
	cmp r5, r3
	beq .L_02008350
	b .L_0200853a
.L_02008350:
	movs r0, #0
	bl Func_0200443c
	movs r0, #163
	lsls r0, r0, #4
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020083ac
	movs r5, #38
	movs r6, #24
	movs r0, #65
	movs r1, #65
	movs r2, #9
	movs r3, #21
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_0200423c
	movs r0, #65
	movs r1, #65
	movs r2, #9
	movs r3, #21
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_02004244
	movs r3, #87
	str r3, [sp, #4]
	movs r0, #76
	movs r1, #64
	movs r2, #9
	movs r3, #22
	str r5, [sp, #0]
	bl Func_02004244
	movs r3, #102
	movs r2, #23
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #76
	movs r1, #64
	movs r2, #9
	movs r3, #22
	bl Func_02004244
.L_020083ac:
	cmp r7, #11
	bne .L_0200840a
	movs r3, #133
	lsls r3, r3, #2
	add r3, r8
	ldr r0, [r3]
	bl Actor_GetById
	mov r8, r0
	movs r0, #208
	mov r2, r10
	lsls r0, r0, #2
	ldr r6, [r2, #108]
	bl GameFlag_GetByte
	adds r5, r0, #0
	movs r0, #210
	lsls r0, r0, #2
	bl GameFlag_GetByte
	movs r3, #128
	lsls r3, r3, #12
	lsls r5, r5, #20
	adds r5, r5, r3
	ldr r2, .L_02008554
	movs r3, #230
	lsls r3, r3, #1
	adds r6, r6, r3
	lsls r0, r0, #20
	adds r0, r0, r2
	ldr r2, [r6]
	mov r1, r8
	str r0, [r1, #16]
	str r5, [r1, #8]
	str r5, [r2, #8]
	ldr r3, [r1, #16]
	str r3, [r2, #16]
	bl Func_020041fc
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200840a
	bl Func_02003e04
.L_0200840a:
	adds r3, r7, #0
	subs r3, #9
	cmp r3, #1
	bhi .L_02008422
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008422
	bl Func_02003e04
.L_02008422:
	cmp r7, #1
	beq .L_0200842c
	cmp r7, #20
	beq .L_0200842c
	b .L_0200853a
.L_0200842c:
	movs r0, #163
	lsls r0, r0, #4
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008452
	movs r0, #11
	movs r1, #3
	bl Actor_SetSpritePriority
	movs r0, #12
	movs r1, #3
	bl Actor_SetSpritePriority
	movs r0, #12
	bl Actor_GetById
	ldr r3, .L_02008558
	str r3, [r0, #24]
.L_02008452:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #75
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008470
	bl Func_02000d88
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_0200432c
	b .L_02008494
.L_02008470:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #74
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200848a
	movs r0, #154
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
	b .L_02008494
.L_0200848a:
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_0200432c
.L_02008494:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #59
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008532
	movs r0, #163
	lsls r0, r0, #4
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020084c2
	movs r0, #11
	movs r1, #0
	movs r2, #0
	bl Func_0200432c
	movs r0, #12
	movs r1, #0
	movs r2, #0
	bl Func_0200432c
.L_020084c2:
	bl Func_02000d88
	movs r0, #10
	bl wait
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #58
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020084f8
	movs r0, #0
	bl Func_02000ae4
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020084f8
	movs r0, #13
	bl Func_02000db8
	movs r0, #14
	bl Func_02000db8
.L_020084f8:
	movs r0, #13
	bl Actor_GetById
	ldr r3, [r0, #8]
	movs r1, #200
	lsls r1, r1, #17
	cmp r3, r1
	ble .L_0200853a
	movs r3, #43
	movs r2, #13
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #39
	movs r1, #22
	movs r2, #2
	movs r3, #1
	bl Func_0200423c
	movs r3, #40
	movs r2, #12
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #31
	movs r1, #21
	movs r2, #2
	movs r3, #2
	bl Func_0200423c
	b .L_0200853a
.L_02008532:
	cmp r7, #20
	bne .L_0200853a
	bl Func_02000714
.L_0200853a:
	movs r0, #0
	add sp, #8
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008548:
	.4byte gMainRoutineWork
.L_0200854c:
	.4byte Data_02004cec
.L_02008550:
	.4byte 0x0000011e
.L_02008554:
	.4byte 0xfed80000
.L_02008558:
	.4byte 0xffff0000
	.section .text.x02008e80,"ax",%progbits
	.ifdef TLA_EDITION_JA
	.balign 4
	.global Func_02000e80
	.thumb_func
Func_02000e80:
	.ifdef TLA_EDITION_JA
	push {r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #128
	ldr r3, [r3, #0]
	sub sp, #8
	mov r9, r3
	bl Func_020042dc
	movs r0, #0
	bl Func_0200444c
	movs r1, #1
	ldr r0, .L_ja_020091e8
	bl FieldMessage_Show
	movs r0, #78
	bl Func_0200446c
	movs r0, #1
	bl wait
	movs r0, #182
	bl Func_0200446c
	ldr r2, .L_ja_020091ec
	movs r0, #234
	mov sl, r2
	movs r1, #170
	movs r2, #128
	movs r3, #147
	lsls r3, r3, #16
	adds r0, #255
	lsls r1, r1, #18
	lsls r2, r2, #12
	bl Func_020041e4
	mov r3, sl
	str r0, [r3, #0]
	cmp r0, #0
	beq .L_ja_02008f20
	ldr r6, [r0, #80]
	movs r3, #33
	ldrb r2, [r6, #5]
	negs r3, r3
	ands r3, r2
	ldrb r2, [r6, #9]
	strb r3, [r6, #5]
	movs r3, #15
	ands r3, r2
	movs r2, #13
	negs r2, r2
	ands r3, r2
	movs r2, #8
	orrs r3, r2
	strb r3, [r6, #9]
	adds r3, r0, #0
	movs r1, #0
	adds r3, #85
	adds r2, r0, #0
	strb r1, [r3, #0]
	adds r2, #92
	movs r3, #1
	strb r3, [r2, #0]
	strb r1, [r6, #26]
	strb r1, [r6, #27]
	adds r1, r0, #0
	adds r1, #35
	ldrb r2, [r1, #0]
	movs r3, #2
	orrs r3, r2
	strb r3, [r1, #0]
	movs r1, #193
	lsls r1, r1, #3
	movs r0, #68
	bl HeapSlot_AllocFar
	adds r5, r0, #0
	movs r0, #247
	bl Func_020042ac
	movs r2, #128
	lsls r2, r2, #3
	adds r5, r5, r2
	ldrb r0, [r6, #16]
	movs r1, #128
	adds r2, r5, #0
	bl VramCache_Load
	movs r0, #68
	bl HeapSlot_Release
.L_ja_02008f20:
	movs r0, #20
	bl FieldEvent_Wait
	ldr r3, .L_ja_020091f0
	movs r7, #133
	mov r8, r3
	lsls r7, r7, #2
	movs r1, #204
	movs r2, #204
	add r7, r8
	lsls r1, r1, #8
	lsls r2, r2, #7
	ldr r0, [r7, #0]
	adds r1, #204
	adds r2, #102
	bl Actor_SetSpeedParameters
	movs r1, #170
	ldr r0, [r7, #0]
	lsls r1, r1, #2
	movs r2, #186
	bl Actor_SetPositionAndReset
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #40
	ldr r0, [r7, #0]
	bl Actor_ArmCallback
	movs r0, #0
	bl Func_0200446c
	movs r0, #141
	bl Func_0200446c
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #9
	lsls r1, r1, #10
	lsls r0, r0, #10
	bl Func_02004264
	movs r0, #20
	bl FieldEvent_Wait
	movs r1, #129
	ldr r0, [r7, #0]
	lsls r1, r1, #1
	bl Func_020043bc
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #9
	lsls r1, r1, #9
	lsls r0, r0, #9
	bl Func_02004264
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #0
	bl Func_0200440c
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #0
	adds r0, #6
	bl Func_02004404
	movs r0, #80
	bl Func_02004414
	movs r0, #80
	bl FieldEvent_Wait
	ldr r5, .L_ja_020091f4
	movs r1, #144
	adds r0, r5, #0
	lsls r1, r1, #3
	bl Task_Add
	movs r1, #153
	lsls r1, r1, #8
	ldr r0, .L_ja_020091f8
	adds r1, #153
	bl Func_020043c4
	movs r0, #170
	movs r1, #1
	movs r2, #240
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #17
	lsls r0, r0, #18
	bl Camera_MoveWithinMapBounds
	bl Func_020043d4
	movs r0, #80
	bl FieldEvent_Wait
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	adds r2, #102
	negs r1, r1
	negs r0, r0
	bl Func_02004264
	movs r0, #195
	lsls r0, r0, #1
	bl Func_0200446c
	movs r0, #254
	lsls r0, r0, #7
	movs r1, #0
	adds r0, #255
	bl Func_02004404
	movs r0, #80
	bl Func_02004414
	movs r0, #60
	bl wait
	adds r0, r5, #0
	bl Task_RemoveFar
	movs r0, #20
	bl wait
	movs r1, #0
	movs r2, #0
	movs r0, #0
	bl Func_02004264
	movs r0, #20
	bl FieldEvent_Wait
	movs r5, #38
	movs r6, #24
	movs r0, #65
	movs r1, #65
	movs r2, #9
	movs r3, #21
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_0200423c
	movs r0, #65
	movs r1, #65
	movs r2, #9
	movs r3, #21
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_02004244
	movs r3, #87
	str r3, [sp, #4]
	movs r0, #76
	movs r1, #64
	movs r2, #9
	movs r3, #22
	str r5, [sp, #0]
	bl Func_02004244
	movs r3, #102
	movs r2, #23
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #76
	movs r2, #9
	movs r1, #64
	movs r3, #22
	bl Func_02004244
	movs r3, #253
	lsls r3, r3, #1
	add r3, r8
	movs r2, #0
	ldrsh r0, [r3, r2]
	ldr r2, .L_ja_020091fc
	ldr r3, .L_ja_02009200
	mov r1, r9
	subs r3, r3, r2
	adds r0, r0, r3
	bl Func_02004294
	movs r3, #0
	mov r2, r9
	movs r0, #128
	strh r3, [r2, #0]
	movs r1, #0
	lsls r0, r0, #9
	bl Func_02004404
	movs r0, #120
	bl Func_02004414
	movs r0, #160
	bl wait
	movs r0, #170
	movs r1, #1
	movs r2, #186
	movs r3, #1
	lsls r0, r0, #18
	negs r1, r1
	lsls r2, r2, #16
	bl Camera_MoveWithinMapBounds
	bl Func_020043d4
	movs r0, #40
	bl FieldEvent_Wait
	mov r3, sl
	ldr r0, [r3, #0]
	cmp r0, #0
	beq .L_ja_0200913c
	adds r2, r0, #0
	adds r2, #85
	movs r3, #3
	strb r3, [r2, #0]
	movs r3, #128
	lsls r3, r3, #10
	movs r5, #128
	str r3, [r0, #40]
	str r3, [r0, #48]
	lsls r5, r5, #9
	movs r1, #168
	movs r3, #160
	lsls r1, r1, #18
	lsls r3, r3, #16
	str r5, [r0, #52]
	movs r2, #0
	bl Func_02004204
	mov r2, sl
	ldr r0, [r2, #0]
	bl Func_0200420c
	movs r0, #135
	bl Func_0200446c
	mov r3, sl
	ldr r0, [r3, #0]
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r0, #52]
	movs r3, #128
	lsls r3, r3, #11
	str r3, [r0, #40]
	movs r1, #166
	movs r3, #158
	str r5, [r0, #48]
	movs r2, #0
	lsls r1, r1, #18
	lsls r3, r3, #16
	bl Func_02004204
	mov r2, sl
	ldr r0, [r2, #0]
	bl Func_0200420c
.L_ja_0200913c:
	movs r0, #20
	bl FieldEvent_Wait
	movs r2, #204
	lsls r2, r2, #8
	ldr r1, .L_ja_02009204
	adds r2, #204
	ldr r0, [r7, #0]
	bl Actor_SetSpeedParameters
	ldr r0, [r7, #0]
	bl Actor_GetById
	adds r0, #90
	ldrb r2, [r0, #0]
	movs r6, #254
	adds r3, r6, #0
	ands r3, r2
	movs r1, #128
	strb r3, [r0, #0]
	lsls r1, r1, #2
	adds r1, #158
	movs r2, #170
	ldr r0, [r7, #0]
	bl Actor_SetPositionAndReset
	movs r0, #1
	bl FieldEvent_Wait
	ldr r0, [r7, #0]
	bl Actor_GetById
	adds r0, #90
	ldrb r3, [r0, #0]
	movs r5, #1
	orrs r3, r5
	strb r3, [r0, #0]
	mov r3, sl
	ldr r0, [r3, #0]
	cmp r0, #0
	beq .L_ja_02009192
	bl Func_020041ec
.L_ja_02009192:
	ldr r0, [r7, #0]
	bl Actor_GetById
	adds r0, #90
	ldrb r2, [r0, #0]
	adds r3, r6, #0
	ands r3, r2
	strb r3, [r0, #0]
	movs r1, #170
	lsls r1, r1, #2
	movs r2, #186
	ldr r0, [r7, #0]
	bl Actor_SetPositionAndReset
	movs r0, #1
	bl FieldEvent_Wait
	ldr r0, [r7, #0]
	bl Actor_GetById
	adds r0, #90
	ldrb r3, [r0, #0]
	orrs r3, r5
	strb r3, [r0, #0]
	bl Func_0200445c
	movs r0, #163
	lsls r0, r0, #4
	bl GameFlag_SetBit
	ldr r3, .L_ja_02009208
	movs r2, #251
	lsls r2, r2, #1
	add r2, r8
	strh r3, [r2, #0]
	bl Func_020042e4
	add sp, #8
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov sl, r6
	pop {r5, r6, r7, pc}
.L_ja_020091e8:
	.4byte 0x00002f9b
.L_ja_020091ec:
	.4byte Data_02004cec
.L_ja_020091f0:
	.4byte gMainRoutineWork
.L_ja_020091f4:
	.4byte Func_02001424
.L_ja_020091f8:
	.4byte 0x0004cccc
.L_ja_020091fc:
	.4byte 0x0000010e
.L_ja_02009200:
	.4byte 0x00000121
.L_ja_02009204:
	.4byte 0x00019999
.L_ja_02009208:
	.4byte 0x00000069
	.else
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #158
	sub sp, #8
	bl GameFlag_Test
	mov r8, r0
	cmp r0, #0
	bne .L_02008ea2
	b .L_0200905c
.L_02008ea2:
	bl Func_020042dc
	movs r0, #0
	bl Func_0200444c
	movs r1, #1
	ldr r0, .L_0200904c
	bl FieldMessage_Show
	movs r0, #78
	bl Func_0200446c
	movs r0, #1
	bl wait
	movs r0, #182
	bl Func_0200446c
	movs r0, #234
	movs r1, #170
	movs r2, #128
	movs r3, #147
	adds r0, #255
	lsls r1, r1, #18
	lsls r2, r2, #12
	lsls r3, r3, #16
	ldr r7, .L_02009050
	bl Func_020041e4
	str r0, [r7]
	cmp r0, #0
	beq .L_02008f48
	ldr r6, [r0, #80]
	movs r3, #33
	ldrb r2, [r6, #5]
	negs r3, r3
	ands r3, r2
	ldrb r2, [r6, #9]
	strb r3, [r6, #5]
	movs r3, #15
	ands r3, r2
	movs r2, #13
	negs r2, r2
	ands r3, r2
	movs r2, #8
	orrs r3, r2
	strb r3, [r6, #9]
	adds r3, r0, #0
	movs r1, #0
	adds r3, #85
	adds r2, r0, #0
	strb r1, [r3]
	adds r2, #92
	movs r3, #1
	strb r3, [r2]
	strb r1, [r6, #26]
	strb r1, [r6, #27]
	adds r1, r0, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #2
	orrs r3, r2
	strb r3, [r1]
	movs r1, #193
	lsls r1, r1, #3
	movs r0, #68
	bl HeapSlot_AllocFar
	adds r5, r0, #0
	movs r0, #247
	bl Func_020042ac
	movs r2, #128
	lsls r2, r2, #3
	adds r5, r5, r2
	ldrb r0, [r6, #16]
	movs r1, #128
	adds r2, r5, #0
	bl VramCache_Load
	movs r0, #68
	bl HeapSlot_Release
.L_02008f48:
	movs r0, #20
	bl FieldEvent_Wait
	ldr r3, .L_02009054
	movs r2, #133
	lsls r2, r2, #2
	adds r6, r3, r2
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	ldr r0, [r6]
	adds r1, #204
	adds r2, #102
	bl Actor_SetSpeedParameters
	movs r1, #170
	ldr r0, [r6]
	lsls r1, r1, #2
	movs r2, #186
	bl Actor_SetPositionAndReset
	movs r1, #192
	ldr r0, [r6]
	lsls r1, r1, #8
	movs r2, #80
	bl Actor_ArmCallback
	movs r1, #2
	ldr r0, [r6]
	adds r1, #255
	movs r2, #40
	bl Func_020043b4
	ldr r0, [r7]
	cmp r0, #0
	beq .L_02008fe6
	adds r2, r0, #0
	adds r2, #85
	movs r3, #3
	strb r3, [r2]
	movs r3, #128
	lsls r3, r3, #10
	movs r5, #128
	str r3, [r0, #40]
	str r3, [r0, #48]
	lsls r5, r5, #9
	movs r1, #168
	movs r3, #160
	lsls r1, r1, #18
	movs r2, #0
	lsls r3, r3, #16
	str r5, [r0, #52]
	bl Func_02004204
	ldr r0, [r7]
	bl Func_0200420c
	movs r0, #135
	bl Func_0200446c
	ldr r0, [r7]
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r0, #52]
	movs r3, #128
	lsls r3, r3, #11
	str r3, [r0, #40]
	movs r1, #166
	movs r3, #158
	str r5, [r0, #48]
	lsls r1, r1, #18
	movs r2, #0
	lsls r3, r3, #16
	bl Func_02004204
	ldr r0, [r7]
	bl Func_0200420c
.L_02008fe6:
	movs r0, #20
	bl FieldEvent_Wait
	movs r2, #204
	lsls r2, r2, #8
	ldr r1, .L_02009058
	adds r2, #204
	ldr r0, [r6]
	bl Actor_SetSpeedParameters
	ldr r0, [r6]
	bl Actor_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	strb r3, [r0]
	movs r1, #128
	lsls r1, r1, #2
	movs r2, #170
	adds r1, #158
	ldr r0, [r6]
	bl Actor_SetPositionAndReset
	movs r0, #1
	bl FieldEvent_Wait
	ldr r0, [r6]
	bl Actor_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	strb r3, [r0]
	ldr r0, [r7]
	cmp r0, #0
	beq .L_02009038
	bl Func_020041ec
.L_02009038:
	movs r1, #128
	ldr r0, [r6]
	lsls r1, r1, #7
	bl Func_020043a4
	bl Func_0200445c
	bl Func_020042e4
	b .L_020093b0
.L_0200904c:
	.4byte MsgMarsStar09
.L_02009050:
	.4byte Data_02004cec
.L_02009054:
	.4byte gMainRoutineWork
.L_02009058:
	.4byte 0x00019999
.L_0200905c:
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #128
	ldr r3, [r3]
	mov r11, r3
	bl Func_020042dc
	movs r0, #0
	bl Func_0200444c
	movs r1, #1
	ldr r0, .L_020093c0
	bl FieldMessage_Show
	movs r0, #78
	bl Func_0200446c
	movs r0, #1
	bl wait
	movs r0, #182
	bl Func_0200446c
	ldr r3, .L_020093c4
	movs r0, #234
	mov r9, r3
	movs r1, #170
	movs r2, #128
	movs r3, #147
	lsls r2, r2, #12
	adds r0, #255
	lsls r1, r1, #18
	lsls r3, r3, #16
	bl Func_020041e4
	mov r2, r9
	str r0, [r2]
	cmp r0, #0
	beq .L_02009112
	ldr r6, [r0, #80]
	mov r3, r8
	ldrb r2, [r6, #5]
	strb r3, [r6, #26]
	strb r3, [r6, #27]
	movs r3, #33
	negs r3, r3
	ands r3, r2
	ldrb r2, [r6, #9]
	strb r3, [r6, #5]
	movs r3, #15
	ands r3, r2
	movs r2, #13
	negs r2, r2
	ands r3, r2
	movs r2, #8
	orrs r3, r2
	strb r3, [r6, #9]
	adds r3, r0, #0
	adds r3, #85
	mov r2, r8
	strb r2, [r3]
	adds r2, r0, #0
	adds r2, #92
	movs r3, #1
	strb r3, [r2]
	adds r1, r0, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #2
	orrs r3, r2
	strb r3, [r1]
	movs r1, #193
	lsls r1, r1, #3
	movs r0, #68
	bl HeapSlot_AllocFar
	adds r5, r0, #0
	movs r0, #247
	bl Func_020042ac
	movs r3, #128
	lsls r3, r3, #3
	adds r5, r5, r3
	ldrb r0, [r6, #16]
	movs r1, #128
	adds r2, r5, #0
	bl VramCache_Load
	movs r0, #68
	bl HeapSlot_Release
.L_02009112:
	movs r0, #20
	bl FieldEvent_Wait
	ldr r2, .L_020093c8
	movs r7, #133
	mov r10, r2
	lsls r7, r7, #2
	movs r1, #204
	movs r2, #204
	add r7, r10
	lsls r1, r1, #8
	lsls r2, r2, #7
	ldr r0, [r7]
	adds r1, #204
	adds r2, #102
	bl Actor_SetSpeedParameters
	movs r1, #170
	ldr r0, [r7]
	lsls r1, r1, #2
	movs r2, #186
	bl Actor_SetPositionAndReset
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #40
	ldr r0, [r7]
	bl Actor_ArmCallback
	movs r0, #0
	bl Func_0200446c
	movs r0, #141
	bl Func_0200446c
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #9
	lsls r1, r1, #10
	lsls r0, r0, #10
	bl Func_02004264
	movs r0, #20
	bl FieldEvent_Wait
	movs r1, #129
	ldr r0, [r7]
	lsls r1, r1, #1
	bl Func_020043bc
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #9
	lsls r1, r1, #9
	lsls r0, r0, #9
	bl Func_02004264
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #0
	bl Func_0200440c
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #0
	adds r0, #6
	bl Func_02004404
	movs r0, #80
	bl Func_02004414
	movs r0, #80
	bl FieldEvent_Wait
	ldr r5, .L_020093cc
	movs r1, #144
	adds r0, r5, #0
	lsls r1, r1, #3
	bl Task_Add
	movs r1, #153
	lsls r1, r1, #8
	ldr r0, .L_020093d0
	adds r1, #153
	bl Func_020043c4
	movs r0, #170
	movs r1, #1
	movs r2, #240
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #17
	lsls r0, r0, #18
	bl Camera_MoveWithinMapBounds
	bl Func_020043d4
	movs r0, #80
	bl FieldEvent_Wait
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	adds r2, #102
	negs r1, r1
	negs r0, r0
	bl Func_02004264
	movs r0, #195
	lsls r0, r0, #1
	bl Func_0200446c
	movs r0, #254
	lsls r0, r0, #7
	movs r1, #0
	adds r0, #255
	bl Func_02004404
	movs r0, #80
	bl Func_02004414
	movs r0, #60
	bl wait
	adds r0, r5, #0
	bl Task_RemoveFar
	movs r0, #20
	bl wait
	movs r1, #0
	movs r2, #0
	movs r0, #0
	bl Func_02004264
	movs r0, #20
	bl FieldEvent_Wait
	movs r5, #38
	movs r6, #24
	movs r0, #65
	movs r1, #65
	movs r2, #9
	movs r3, #21
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_0200423c
	movs r0, #65
	movs r1, #65
	movs r2, #9
	movs r3, #21
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_02004244
	movs r3, #87
	str r3, [sp, #4]
	movs r0, #76
	movs r1, #64
	movs r2, #9
	movs r3, #22
	str r5, [sp, #0]
	bl Func_02004244
	movs r3, #102
	movs r2, #23
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #76
	movs r2, #9
	movs r1, #64
	movs r3, #22
	bl Func_02004244
	movs r3, #253
	lsls r3, r3, #1
	add r3, r10
	movs r2, #0
	ldrsh r0, [r3, r2]
	ldr r2, .L_020093d4
	ldr r3, .L_020093d8
	mov r1, r11
	subs r3, r3, r2
	adds r0, r0, r3
	bl Func_02004294
	mov r3, r8
	mov r2, r11
	movs r0, #128
	strh r3, [r2]
	movs r1, #0
	lsls r0, r0, #9
	bl Func_02004404
	movs r0, #120
	bl Func_02004414
	movs r0, #160
	bl wait
	movs r0, #170
	movs r1, #1
	movs r2, #186
	movs r3, #1
	lsls r0, r0, #18
	negs r1, r1
	lsls r2, r2, #16
	bl Camera_MoveWithinMapBounds
	bl Func_020043d4
	movs r0, #40
	bl FieldEvent_Wait
	mov r3, r9
	ldr r0, [r3]
	cmp r0, #0
	beq .L_0200932e
	adds r2, r0, #0
	adds r2, #85
	movs r3, #3
	strb r3, [r2]
	movs r3, #128
	lsls r3, r3, #10
	movs r5, #128
	str r3, [r0, #40]
	str r3, [r0, #48]
	lsls r5, r5, #9
	movs r1, #168
	movs r3, #160
	lsls r1, r1, #18
	lsls r3, r3, #16
	str r5, [r0, #52]
	movs r2, #0
	bl Func_02004204
	mov r2, r9
	ldr r0, [r2]
	bl Func_0200420c
	movs r0, #135
	bl Func_0200446c
	mov r3, r9
	ldr r0, [r3]
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r0, #52]
	movs r3, #128
	lsls r3, r3, #11
	str r3, [r0, #40]
	movs r1, #166
	movs r3, #158
	str r5, [r0, #48]
	movs r2, #0
	lsls r1, r1, #18
	lsls r3, r3, #16
	bl Func_02004204
	mov r2, r9
	ldr r0, [r2]
	bl Func_0200420c
.L_0200932e:
	movs r0, #20
	bl FieldEvent_Wait
	movs r2, #204
	lsls r2, r2, #8
	ldr r1, .L_020093dc
	adds r2, #204
	ldr r0, [r7]
	bl Actor_SetSpeedParameters
	ldr r0, [r7]
	bl Actor_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	movs r1, #128
	strb r3, [r0]
	lsls r1, r1, #2
	movs r2, #170
	adds r1, #158
	ldr r0, [r7]
	bl Actor_SetPositionAndReset
	movs r0, #1
	bl FieldEvent_Wait
	ldr r0, [r7]
	bl Actor_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	strb r3, [r0]
	mov r3, r9
	ldr r0, [r3]
	cmp r0, #0
	beq .L_02009382
	bl Func_020041ec
.L_02009382:
	movs r1, #128
	lsls r1, r1, #7
	ldr r0, [r7]
	bl Func_020043a4
	bl Func_0200445c
	movs r0, #163
	lsls r0, r0, #4
	bl GameFlag_SetBit
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #158
	bl GameFlag_SetBit
	ldr r3, .L_020093e0
	movs r2, #251
	lsls r2, r2, #1
	add r2, r10
	strh r3, [r2]
	bl Func_020042e4
.L_020093b0:
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_020093c0:
	.4byte MsgMarsStar09
.L_020093c4:
	.4byte Data_02004cec
.L_020093c8:
	.4byte gMainRoutineWork
.L_020093cc:
	.4byte Func_02001424
.L_020093d0:
	.4byte 0x0004cccc
.L_020093d4:
	.4byte 0x0000010e
.L_020093d8:
	.4byte 0x00000121
.L_020093dc:
	.4byte 0x00019999
.L_020093e0:
	.4byte 0x00000069
	.endif
	.endif
	.section .text.x02009424,"ax",%progbits
	.balign 4
	.global Func_02001424
	.thumb_func
Func_02001424:
	push {r5, lr}
	ldr r3, .L_020094ec
	movs r1, #1
	ldr r2, [r3]
	adds r3, r2, #0
	ands r3, r1
	cmp r3, #0
	bne .L_020094e8
	lsrs r3, r2, #1
	ands r3, r1
	cmp r3, #0
	beq .L_02009454
	bl Random16Far
	lsls r5, r0, #3
	subs r5, r5, r0
	bl Random16Far
	lsls r5, r5, #2
	lsls r3, r0, #4
	subs r3, r3, r0
	lsrs r5, r5, #16
	movs r2, #155
	b .L_0200946a
.L_02009454:
	bl Random16Far
	lsls r5, r0, #3
	subs r5, r5, r0
	bl Random16Far
	lsls r5, r5, #2
	lsls r3, r0, #4
	subs r3, r3, r0
	lsrs r5, r5, #16
	movs r2, #178
.L_0200946a:
	lsls r2, r2, #18
	lsls r3, r3, #3
	lsls r5, r5, #16
	adds r5, r5, r2
	lsrs r3, r3, #16
	movs r2, #230
	lsls r2, r2, #17
	lsls r3, r3, #16
	adds r3, r3, r2
	movs r0, #30
	movs r2, #128
	adds r1, r5, #0
	adds r0, #255
	lsls r2, r2, #14
	bl Func_020041e4
	adds r5, r0, #0
	cmp r5, #0
	beq .L_020094e8
	adds r2, r5, #0
	adds r2, #92
	movs r3, #2
	strb r3, [r2]
	adds r3, r5, #0
	movs r1, #0
	adds r3, #85
	strb r1, [r3]
	adds r3, #15
	strh r1, [r3]
	adds r1, r5, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #254
	ands r3, r2
	strb r3, [r1]
	movs r3, #13
	ldr r1, [r5, #80]
	negs r3, r3
	ldrb r2, [r1, #9]
	adds r0, r5, #0
	ands r3, r2
	movs r2, #8
	orrs r3, r2
	strb r3, [r1, #9]
	movs r1, #0
	bl Actor_SetSingleChildFlags
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r5, #24]
	str r3, [r5, #28]
	movs r3, #60
	str r3, [r5, #104]
	ldr r3, .L_020094f0
	adds r0, r5, #0
	movs r1, #5
	str r3, [r5, #108]
	bl Func_020041cc
	adds r0, r5, #0
	movs r1, #1
	bl Animation_SetStateFlags
.L_020094e8:
	pop {r5, pc}
	.2byte 0x0000
.L_020094ec:
	.4byte gFrameCount
.L_020094f0:
	.4byte Func_020013e4
	.section .text.x0200a148,"ax",%progbits
	.ifdef TLA_EDITION_JA
	.global Func_02002148
	.thumb_func
Func_02002148:
	push {r5, r6, r7, lr}
	mov r7, sl
	mov r6, r8
	push {r6, r7}
	ldr r5, .L_ja_0200a0fc
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #18
	adds r3, r5, r0
	ldrb r3, [r3, #0]
	sub sp, #12
	cmp r3, #0
	beq .L_ja_02009f8c
	b .L_ja_0200a0ec
.L_ja_02009f8c:
	movs r0, #10
	movs r1, #0
	negs r0, r0
	bl Func_02001fcc
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r5, r1
	ldr r0, [r3, #0]
	bl Actor_GetById
	ldr r2, .L_ja_0200a100
	adds r5, r0, #0
	mov sl, r2
	bl Func_020042dc
	movs r0, #0
	bl Func_0200444c
	ldr r3, .L_ja_0200a104
	adds r0, r5, #0
	str r3, [r5, #108]
	movs r3, #128
	lsls r3, r3, #7
	strh r3, [r5, #6]
	movs r1, #49
	bl Func_020041cc
.L_ja_02009fc4:
	adds r3, r5, #0
	adds r3, #34
	ldr r1, [r5, #8]
	ldrb r0, [r3, #0]
	ldr r2, [r5, #16]
	bl Func_0200422c
	mov r6, sl
	mov r7, sl
	adds r6, #104
	adds r7, #106
	mov r1, sp
	cmp r0, #7
	bne .L_ja_0200a03c
	movs r0, #0
	ldrsh r3, [r6, r0]
	movs r2, #0
	lsls r3, r3, #17
	str r3, [r5, #36]
	mov r8, r2
	movs r1, #0
	ldrsh r3, [r7, r1]
	lsls r3, r3, #17
	str r3, [r5, #44]
	movs r3, #128
	lsls r3, r3, #5
	str r3, [r5, #52]
.L_ja_02009ffa:
	movs r3, #0
	ldrsh r2, [r6, r3]
	ldr r3, [r5, #8]
	lsls r2, r2, #19
	mov r1, sp
	adds r3, r3, r2
	str r3, [r1, #0]
	ldr r3, [r5, #12]
	str r3, [r1, #4]
	movs r0, #0
	ldrsh r2, [r7, r0]
	ldr r3, [r5, #16]
	lsls r2, r2, #19
	adds r3, r3, r2
	str r3, [r1, #8]
	adds r0, r5, #0
	bl Func_0200424c
	cmp r0, #0
	beq .L_ja_0200a02a
	movs r3, #0
	str r3, [r5, #36]
	str r3, [r5, #44]
	b .L_ja_0200a0e0
.L_ja_0200a02a:
	movs r0, #1
	bl wait
	movs r1, #1
	add r8, r1
	mov r2, r8
	cmp r2, #9
	ble .L_ja_02009ffa
	b .L_ja_0200a0e0
.L_ja_0200a03c:
	movs r3, #0
	ldrsh r2, [r6, r3]
	ldr r3, [r5, #8]
	lsls r2, r2, #19
	adds r3, r3, r2
	str r3, [r1, #0]
	ldr r3, [r5, #12]
	str r3, [r1, #4]
	movs r0, #0
	ldrsh r2, [r7, r0]
	ldr r3, [r5, #16]
	lsls r2, r2, #19
	adds r3, r3, r2
	str r3, [r1, #8]
	adds r0, r5, #0
	bl Func_0200424c
	cmp r0, #0
	bgt .L_ja_0200a0e0
	cmp r0, #0
	bge .L_ja_0200a082
	movs r3, #128
	lsls r3, r3, #7
	strh r3, [r5, #6]
	ldr r3, .L_ja_0200a0fc
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	movs r2, #1
	ldr r0, [r3, #0]
	movs r1, #6
	negs r2, r2
	bl Func_0200436c
	b .L_ja_0200a0e0
.L_ja_0200a082:
	movs r2, #0
	ldrsh r3, [r6, r2]
	ldr r2, [r5, #8]
	lsls r3, r3, #17
	adds r1, r2, r3
	str r1, [r5, #8]
	ldr r2, [r5, #16]
	movs r0, #0
	ldrsh r3, [r7, r0]
	ldr r7, .L_ja_0200a108
	lsls r3, r3, #17
	ldr r0, .L_ja_0200a10c
	adds r6, r2, r3
	adds r3, r1, #0
	ands r3, r7
	movs r4, #128
	adds r2, r3, r0
	lsls r4, r4, #9
	str r6, [r5, #16]
	cmp r2, r4
	ble .L_ja_0200a0ae
	adds r2, r4, #0
.L_ja_0200a0ae:
	ldr r0, .L_ja_0200a110
	cmp r2, r0
	bge .L_ja_0200a0b6
	adds r2, r0, #0
.L_ja_0200a0b6:
	subs r3, r1, r2
	ldr r1, .L_ja_0200a10c
	str r3, [r5, #8]
	adds r3, r6, #0
	ands r3, r7
	adds r2, r3, r1
	cmp r2, r4
	ble .L_ja_0200a0c8
	adds r2, r4, #0
.L_ja_0200a0c8:
	cmp r2, r0
	bge .L_ja_0200a0ce
	adds r2, r0, #0
.L_ja_0200a0ce:
	subs r3, r6, r2
	str r3, [r5, #16]
	mov r2, sl
	movs r3, #0
	strh r3, [r2, #4]
	movs r0, #1
	bl wait
	b .L_ja_02009fc4
.L_ja_0200a0e0:
	movs r3, #0
	str r3, [r5, #108]
	adds r0, r5, #0
	movs r1, #0
	bl Animation_ApplyChildValues
.L_ja_0200a0ec:
	mov r0, sl
	movs r3, #0
	strh r3, [r0, #4]
	add sp, #12
	pop {r3, r5}
	mov r8, r3
	mov sl, r5
	pop {r5, r6, r7, pc}
.L_ja_0200a0fc:
	.4byte gMainRoutineWork
.L_ja_0200a100:
	.4byte 0x0200254c
.L_ja_0200a104:
	.4byte Func_02002120
.L_ja_0200a108:
	.4byte 0x000fffff
.L_ja_0200a10c:
	.4byte 0xfff80000
.L_ja_0200a110:
	.4byte 0xffff0000
	.endif
	.section .text.x0200a4d4,"ax",%progbits
	.global Func_020024d4
	.thumb_func
Func_020024d4:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r1, .L_0200a63c
	sub sp, #4
	ldr r3, [r1, #112]
	mov r11, r0
	cmp r3, #0
	bne .L_0200a4f0
	b .L_0200a648
.L_0200a4f0:
	movs r2, #0
	str r2, [sp, #0]
.L_0200a4f4:
	bl Random16Far
	adds r5, r0, #0
	bl Random16Far
	mov r3, r11
	ldr r3, [r3, #8]
	lsls r5, r5, #4
	mov r8, r3
	add r8, r5
	lsls r0, r0, #4
	mov r1, r8
	subs r1, r1, r0
	mov r8, r1
	bl Random16Far
	adds r6, r0, #0
	bl Random16Far
	adds r5, r0, #0
	bl Random16Far
	mov r2, r11
	ldr r3, [r2, #16]
	ldr r2, [r2, #12]
	lsls r5, r5, #4
	lsls r6, r6, #3
	movs r1, #128
	lsls r0, r0, #4
	adds r6, r6, r2
	lsls r1, r1, #11
	adds r3, r3, r5
	subs r3, r3, r0
	adds r6, r6, r1
	movs r0, #234
	adds r0, #255
	mov r1, r8
	adds r2, r6, #0
	bl Func_020041e4
	adds r7, r0, #0
	cmp r7, #0
	beq .L_0200a628
	bl Random16Far
	mov r10, r0
	bl Random16Far
	adds r6, r0, #0
	bl Random16Far
	adds r5, r0, #0
	bl Random16Far
	movs r2, #128
	lsls r2, r2, #8
	adds r5, r5, r0
	ldr r1, .L_0200a640
	adds r0, r7, #0
	mov r9, r2
	bl Func_020041dc
	adds r0, r7, #0
	movs r1, #0
	bl Actor_SetSingleChildFlags
	mov r1, r10
	movs r2, #128
	lsls r2, r2, #10
	lsls r3, r1, #2
	adds r3, r3, r2
	str r3, [r7, #40]
	mov r0, r10
	bl Math_Cosine
	ldr r3, .L_0200a644
	lsls r6, r6, #3
	mov r8, r3
	adds r1, r0, #0
	adds r0, r6, #0
	mov lr, r8
	.2byte 0xf800
	str r0, [r7, #44]
	mov r0, r10
	bl Math_Sine
	adds r1, r0, #0
	adds r0, r6, #0
	mov lr, r8
	.2byte 0xf800
	ldr r2, .L_0200a63c
	movs r3, #168
	lsls r3, r3, #7
	adds r3, #122
	str r3, [r7, #72]
	lsrs r5, r5, #2
	ldr r3, [r2, #112]
	add r5, r9
	movs r6, #0
	mov r1, r9
	str r5, [r7, #24]
	str r5, [r7, #28]
	str r1, [r7, #68]
	ldr r5, [r7, #80]
	str r0, [r7, #36]
	str r6, [r7, #52]
	ldr r3, [r3, #80]
	ldrb r0, [r5, #16]
	mov r8, r3
	bl Resource_ResetEntry
	ldrb r3, [r5, #17]
	ldr r1, .L_0200a63c
	movs r2, #1
	orrs r3, r2
	strb r3, [r5, #17]
	ldrh r3, [r1, #12]
	ldr r0, [r5, #40]
	strb r3, [r5, #16]
	bl ResourceMetadata_ClearRecord
	str r6, [r5, #40]
	strb r6, [r5, #27]
	mov r2, r8
	ldrb r3, [r2, #20]
	ldrb r0, [r5, #5]
	strb r3, [r5, #20]
	ldrb r3, [r2, #21]
	strb r3, [r5, #21]
	ldrb r1, [r2, #5]
	movs r2, #63
	adds r3, r2, #0
	lsrs r1, r1, #6
	lsls r1, r1, #6
	ands r3, r0
	orrs r3, r1
	strb r3, [r5, #5]
	mov r1, r8
	ldrb r3, [r1, #7]
	ldrb r1, [r5, #7]
	lsrs r3, r3, #6
	lsls r3, r3, #6
	ands r2, r1
	orrs r2, r3
	strb r2, [r5, #7]
	mov r3, r8
	ldrh r2, [r3, #8]
	ldr r1, .L_0200a638
	ldrh r3, [r5, #8]
	lsls r2, r2, #22
	lsrs r2, r2, #22
	ands r3, r1
	orrs r3, r2
	strh r3, [r5, #8]
.L_0200a628:
	ldr r1, [sp, #0]
	subs r1, #1
	str r1, [sp, #0]
	cmp r1, #0
	blt .L_0200a634
	b .L_0200a4f4
.L_0200a634:
	b .L_0200a648
	.2byte 0x0000
.L_0200a638:
	.4byte 0xfffffc00
.L_0200a63c:
	.4byte gData_020023c4 + 0x188
.L_0200a640:
	.4byte gData_02004774
.L_0200a644:
	.4byte IwramMulQ16
.L_0200a648:
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x0200aad0,"ax",%progbits
	.balign 4
	.global Func_02002ad0
	.thumb_func
Func_02002ad0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	mov r11, r0
	mov r3, r11
	adds r3, #98
	ldrb r0, [r3]
	sub sp, #12
	bl Actor_GetById
	mov r3, r11
	adds r7, r0, #0
	adds r3, #102
	movs r2, #0
	ldrsh r6, [r3, r2]
	ldr r3, [r7, #80]
	mov r2, r11
	mov r9, r3
	ldr r3, [r2, #8]
	mov r5, sp
	str r3, [r5]
	movs r0, #128
	ldr r3, [r2, #12]
	ldr r2, .L_0200ab60
	adds r1, r6, #0
	adds r3, r3, r2
	str r3, [r5, #4]
	mov r2, r11
	ldr r3, [r2, #16]
	lsls r0, r0, #14
	adds r2, r5, #0
	str r3, [r5, #8]
	bl Vector_AddPolarOffsetFar
	ldr r3, .L_0200ab5c
	movs r2, #0
	mov r10, r3
	adds r3, r7, #0
	mov r8, r2
	adds r3, #85
	mov r2, r10
	strh r6, [r7, #6]
	strb r2, [r3]
	adds r0, r7, #0
	ldr r1, [r5]
	ldr r2, [r5, #4]
	ldr r3, [r5, #8]
	bl Actor_SetPositionAndResetMotion
	ldr r3, .L_0200ab64
	mov r2, r10
	str r3, [r7, #108]
	adds r3, r7, #0
	adds r3, #90
	strb r2, [r3]
	movs r3, #128
	lsls r3, r3, #12
	str r3, [r7, #48]
	movs r3, #128
	lsls r3, r3, #6
	str r3, [r7, #52]
	ldr r3, .L_0200ab68
	mov r2, r9
	adds r6, r6, r3
	b .L_0200ab6c
	.2byte 0x0000
.L_0200ab5c:
	.4byte 0x00000000
.L_0200ab60:
	.4byte 0xfff40000
.L_0200ab64:
	.4byte Func_0200274c
.L_0200ab68:
	.4byte 0xffffc000
.L_0200ab6c:
	mov r3, r8
	strh r6, [r2, #18]
	str r3, [r7, #24]
	str r3, [r7, #28]
	adds r3, r7, #0
	mov r2, r8
	adds r3, #100
	strh r2, [r3]
	mov r2, r11
	ldr r3, [r2, #104]
	movs r0, #104
	str r3, [r7, #104]
	bl Func_0200446c
	add sp, #12
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x0200b602,"ax",%progbits
	.2byte 0x0000
	.section .text.x0200b63a,"ax",%progbits
	.2byte 0x0000
	.section .text.x0200be02,"ax",%progbits
	.2byte 0x0000
	.section .bss,"aw",%nobits
	.global Data_02004cec
Data_02004cec:
	.space 0x00000004
	.global gData_02004cf0
gData_02004cf0:
	.space 0x00000004
	.global gData_02004cf4
gData_02004cf4:
