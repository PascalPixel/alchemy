.syntax unified
	.thumb
	.section .text.x020084d4,"ax",%progbits
	.global Func_020004d4
	.thumb_func
Func_020004d4:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	ldr r6, .L_020085d4
	.ifdef TLA_EDITION_JA
	adds r2, #88
	.else
	adds r2, #85
	.endif
	str r2, [r3]
	movs r3, #240
	lsls r3, r3, #1
	adds r5, r6, r3
	movs r3, #0
	ldrsh r2, [r5, r3]
	ldr r3, .L_020085d8
	cmp r2, r3
	beq .L_0200850e
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r6, r2
	ldr r0, [r3]
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #32
	orrs r3, r2
	strb r3, [r0]
.L_0200850e:
	movs r0, #0
	bl Func_020035ac
	movs r3, #0
	ldrsh r2, [r5, r3]
	ldr r3, .L_020085dc
	cmp r2, r3
	bne .L_020085a6
	movs r2, #241
	lsls r2, r2, #1
	adds r5, r6, r2
	movs r2, #0
	ldrsh r3, [r5, r2]
	cmp r3, #98
	bne .L_02008530
	bl Func_02001774
.L_02008530:
	movs r2, #0
	ldrsh r3, [r5, r2]
	cmp r3, #99
	bne .L_02008548
	movs r3, #245
	lsls r3, r3, #1
	adds r2, r6, r3
	movs r3, #23
	strh r3, [r2]
	strh r3, [r5]
	bl Func_02001868
.L_02008548:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #119
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008588
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #120
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008588
	movs r1, #163
	movs r0, #10
	lsls r1, r1, #1
	bl Func_0200361c
	movs r1, #166
	movs r2, #232
	movs r0, #10
	lsls r1, r1, #18
	lsls r2, r2, #16
	bl Func_020034d4
	movs r0, #10
	bl Object_GetById
	movs r3, #4
	adds r0, #85
	strb r3, [r0]
.L_02008588:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #119
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020085ce
	movs r1, #166
	movs r2, #200
	movs r0, #8
	lsls r1, r1, #18
	lsls r2, r2, #16
	bl Func_020034d4
	b .L_020085ce
.L_020085a6:
	ldr r3, .L_020085e0
	cmp r2, r3
	bne .L_020085ce
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #126
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020085ce
	movs r1, #11
	movs r0, #8
	bl Object_SetModeById
	movs r0, #8
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
.L_020085ce:
	movs r0, #0
	pop {r5, r6, pc}
	.2byte 0x0000
.L_020085d4:
	.4byte gPartyState
.L_020085d8:
	.4byte 0x000000b9
.L_020085dc:
	.4byte 0x000000bc
.L_020085e0:
	.4byte 0x000000bb
	.section .text.x02008734,"ax",%progbits
	.balign 4
	.global Func_02000734
	.thumb_func
Func_02000734:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #16
	str r0, [sp, #12]
	movs r0, #0
	adds r7, r2, #0
	mov r9, r0
	str r1, [sp, #8]
	cmp r9, r7
	bgt .L_020087f4
.L_02008752:
	movs r2, #4
	str r2, [sp, #4]
	movs r1, #0
	mov r11, r1
.L_0200875a:
	ldr r0, [sp, #12]
	mov r3, r11
	ldrh r5, [r3, r0]
	movs r3, #31
	mov r10, r3
	ldr r1, .L_020087a0
	mov r2, r10
	ands r2, r5
	lsls r5, r5, #16
	lsrs r0, r5, #21
	ands r0, r1
	mov r8, r0
	ldr r0, [sp, #8]
	mov r10, r2
	mov r2, r11
	ldrh r6, [r2, r0]
	lsrs r5, r5, #26
	ands r3, r6
	lsls r6, r6, #16
	lsrs r2, r6, #21
	lsrs r6, r6, #26
	ands r2, r1
	ands r5, r1
	ands r6, r1
	mov r1, r10
	subs r3, r3, r1
	mov r0, r9
	muls r0, r3
	adds r1, r7, #0
	str r2, [sp, #0]
	bl __divsi3
	ldr r2, [sp, #0]
	mov r3, r8
	b .L_020087a4
.L_020087a0:
	.4byte 0x0000001f
.L_020087a4:
	subs r2, r2, r3
	adds r1, r7, #0
	add r10, r0
	mov r0, r9
	muls r0, r2
	bl __divsi3
	subs r6, r6, r5
	add r8, r0
	adds r1, r7, #0
	mov r0, r9
	muls r0, r6
	bl __divsi3
	adds r5, r5, r0
	lsls r5, r5, #10
	mov r0, r8
	movs r3, #160
	lsls r0, r0, #5
	lsls r3, r3, #19
	orrs r5, r0
	mov r1, r10
	adds r3, #228
	orrs r5, r1
	add r3, r11
	strh r5, [r3]
	movs r2, #2
	ldr r3, [sp, #4]
	add r11, r2
	subs r3, #1
	str r3, [sp, #4]
	cmp r3, #0
	bge .L_0200875a
	movs r0, #1
	bl WaitFrames
	movs r0, #1
	add r9, r0
	cmp r9, r7
	ble .L_02008752
.L_020087f4:
	add sp, #16
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x02008a34,"ax",%progbits
	.balign 4
	.global Func_02000a34
	.thumb_func
Func_02000a34:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	adds r5, r0, #0
	movs r0, #10
	sub sp, #64
	bl Object_GetById
	ldr r3, .L_02008b58
	adds r7, r0, #0
	add r0, sp, #28
	str r3, [r0]
	movs r2, #160
	movs r3, #0
	str r3, [r0, #4]
	lsls r2, r2, #19
	movs r3, #238
	lsls r3, r3, #16
	adds r2, #228
	add r1, sp, #52
	str r3, [r0, #8]
	ldrh r3, [r2]
	mov r9, r1
	mov r4, r9
	strh r3, [r4]
	adds r2, #2
	ldrh r3, [r2]
	mov r11, r0
	mov r0, r9
	strh r3, [r0, #2]
	adds r2, #2
	ldrh r3, [r2]
	adds r2, #2
	strh r3, [r1, #4]
	add r6, sp, #40
	ldrh r3, [r2]
	strh r3, [r4, #6]
	ldrh r3, [r2, #2]
	movs r2, #160
	strh r3, [r0, #8]
	lsls r2, r2, #19
	adds r2, #196
	ldrh r3, [r2]
	adds r2, #2
	strh r3, [r6]
	ldrh r3, [r2]
	adds r2, #2
	strh r3, [r6, #2]
	ldrh r3, [r2]
	adds r2, #2
	strh r3, [r6, #4]
	ldrh r3, [r2]
	strh r3, [r6, #6]
	ldrh r3, [r2, #2]
	strh r3, [r6, #8]
	bl Func_0200345c
	movs r0, #0
	bl Func_020035b4
	ldr r0, .L_02008b5c
	bl Func_0200350c
	adds r0, r5, #0
	bl Func_020005e8
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #8
	bl Func_02003544
	movs r0, #8
	movs r1, #0
	movs r2, #5
	bl Func_0200351c
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #8
	bl Func_02003544
	movs r1, #0
	movs r2, #5
	movs r0, #8
	bl Func_0200351c
	movs r0, #1
	bl Func_0200067c
	movs r1, #166
	movs r2, #200
	movs r0, #8
	lsls r1, r1, #2
	bl ObjectMotion_SetPositionAndReset
	movs r1, #192
	lsls r1, r1, #6
	movs r0, #8
	bl Func_02003534
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r1, #128
	lsls r1, r1, #8
	movs r0, #8
	bl Func_02003534
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r1, #0
	movs r0, #8
	bl UiText_OpenMessageAtObject
	movs r0, #4
	movs r1, #0
	bl Event_AskYesNoWithActorAnimation
	cmp r0, #0
	bne .L_02008b60
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r1, #0
	movs r2, #5
	movs r0, #8
	bl Func_0200351c
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r1, #226
	lsls r1, r1, #1
	adds r2, r2, r1
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_02008b82
	.2byte 0x0000
.L_02008b58:
	.4byte 0x02960000
.L_02008b5c:
	.4byte MsgWellDoneYou
.L_02008b60:
	movs r0, #40
	bl FieldEvent_WaitFrames
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #8
	adds r3, #1
	strh r3, [r2]
	movs r1, #0
	movs r2, #5
	bl Func_0200351c
.L_02008b82:
	ldr r3, .L_02008f3c
	movs r4, #133
	lsls r4, r4, #2
	adds r3, r3, r4
	movs r1, #128
	ldr r0, [r3]
	movs r2, #0
	lsls r1, r1, #6
	bl ObjectMotion_ArmCallback
	movs r1, #160
	lsls r1, r1, #7
	movs r0, #8
	bl Func_02003534
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r0, #166
	movs r1, #1
	movs r2, #232
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #16
	lsls r0, r0, #18
	bl Camera_MoveWithinMapBounds
	bl Func_02003564
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r1, #8
	adds r1, #255
	movs r2, #40
	movs r0, #8
	bl Func_02003544
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #8
	movs r1, #0
	movs r2, #5
	bl Func_0200351c
	movs r0, #0
	mov r8, r0
.L_02008be0:
	ldr r3, .L_02008f40
	add r5, sp, #16
	adds r2, r5, #0
	ldmia r3!, {r0, r1, r4}
	stmia r2!, {r0, r1, r4}
	movs r1, #9
	movs r0, #8
	bl Object_SetModeById
	movs r0, #3
	bl FieldEvent_WaitFrames
	mov r1, r8
	lsls r3, r1, #2
	ldr r1, [r5, r3]
	movs r0, #10
	bl Func_0200361c
	movs r1, #167
	movs r2, #200
	movs r0, #10
	lsls r1, r1, #18
	lsls r2, r2, #16
	bl Func_020034d4
	movs r3, #128
	lsls r3, r3, #7
	str r3, [r7, #72]
	movs r2, #2
	movs r0, #10
	movs r1, #4
	bl ObjectMotion_Launch
	movs r0, #10
	movs r1, #1
	bl ObjectMotion_SetActionVariant
	movs r1, #128
	movs r2, #128
	movs r0, #10
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r1, #0
	movs r2, #40
	movs r0, #10
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r0, #155
	lsls r0, r0, #1
	bl Func_0200362c
	movs r0, #34
	bl FieldEvent_WaitFrames
	ldr r3, [r7, #8]
	add r2, sp, #4
	str r3, [r2]
	ldr r3, [r7, #12]
	movs r1, #0
	str r3, [r2, #4]
	ldr r3, [r7, #16]
	movs r0, #10
	str r3, [r2, #8]
	movs r2, #0
	bl Func_020034d4
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r0, #134
	bl Func_0200362c
	mov r0, r9
	adds r1, r6, #0
	movs r2, #12
	bl Func_02000734
	movs r2, #48
	adds r0, r6, #0
	mov r1, r9
	bl Func_02000734
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r2, #1
	add r8, r2
	mov r3, r8
	cmp r3, #2
	ble .L_02008be0
	ldr r5, .L_02008f3c
	movs r4, #133
	lsls r4, r4, #2
	adds r5, r5, r4
	ldr r0, [r5]
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r1, #254
	mov r10, r1
	mov r3, r10
	ands r3, r2
	strb r3, [r0]
	movs r1, #12
	movs r2, #12
	ldr r0, [r5]
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #1
	bl FieldEvent_WaitFrames
	ldr r0, [r5]
	bl Object_GetById
	adds r0, #90
	movs r2, #1
	ldrb r3, [r0]
	mov r8, r2
	mov r4, r8
	orrs r3, r4
	strb r3, [r0]
	movs r1, #128
	ldr r0, [r5]
	lsls r1, r1, #6
	bl Func_02003534
	movs r0, #10
	bl FieldEvent_WaitFrames
	ldr r0, [r5]
	movs r1, #22
	bl Object_SetModeById
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r1, #7
	movs r0, #8
	bl Object_SetModeById
	movs r0, #60
	bl FieldEvent_WaitFrames
	movs r0, #78
	bl Func_0200362c
	movs r1, #2
	adds r1, #255
	movs r2, #60
	movs r0, #8
	bl Func_02003544
	movs r0, #128
	lsls r0, r0, #6
	movs r1, #0
	movs r2, #5
	adds r0, #8
	bl Func_0200351c
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r0, #220
	bl Func_0200362c
	movs r2, #48
	mov r0, r9
	adds r1, r6, #0
	bl Func_02000734
	ldr r0, [r5]
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	ldr r0, [r5]
	lsls r1, r1, #1
	bl Func_0200354c
	movs r0, #50
	bl FieldEvent_WaitFrames
	movs r1, #2
	movs r2, #60
	adds r1, #255
	movs r0, #8
	bl Func_02003544
	movs r1, #128
	lsls r1, r1, #5
	movs r0, #16
	adds r1, #144
	bl Func_0200360c
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #9
	lsls r1, r1, #10
	lsls r0, r0, #10
	bl Func_02003434
	mov r0, r11
	bl Func_02003164
	movs r0, #8
	movs r1, #1
	bl Object_SetModeById
	movs r1, #6
	movs r2, #0
	movs r0, #4
	bl ObjectMotion_Launch
	movs r0, #194
	bl Func_0200362c
	movs r0, #4
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	mov r1, r10
	adds r2, r1, #0
	ands r2, r3
	strb r2, [r0]
	movs r1, #12
	movs r2, #12
	negs r2, r2
	negs r1, r1
	movs r0, #4
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #1
	bl FieldEvent_WaitFrames
	movs r0, #4
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	mov r4, r8
	orrs r4, r3
	strb r4, [r0]
	movs r0, #10
	mov r8, r4
	bl FieldEvent_WaitFrames
	movs r0, #8
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #8
	bl Func_0200354c
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #8
	movs r1, #0
	movs r2, #5
	bl Func_0200351c
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #11
	lsls r2, r2, #9
	lsls r0, r0, #11
	bl Func_02003434
	mov r0, r11
	bl Func_0200320c
	movs r0, #194
	bl Func_0200362c
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r0, #8
	movs r1, #6
	movs r2, #15
	bl ObjectMotion_Launch
	movs r1, #6
	movs r2, #23
	movs r0, #8
	bl ObjectMotion_Launch
	movs r0, #194
	bl Func_0200362c
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r0, #128
	lsls r0, r0, #6
	movs r2, #5
	adds r0, #8
	movs r1, #0
	bl Func_0200351c
	movs r1, #163
	movs r0, #10
	lsls r1, r1, #1
	bl Func_0200361c
	movs r1, #166
	movs r2, #232
	lsls r2, r2, #16
	movs r0, #10
	lsls r1, r1, #18
	bl Func_020034d4
	adds r5, r7, #0
	movs r0, #10
	movs r1, #1
	bl ObjectMotion_SetActionVariant
	adds r5, #85
	movs r3, #2
	strb r3, [r5]
	movs r3, #192
	lsls r3, r3, #13
	str r3, [r7, #12]
	ldr r3, .L_02008f44
	movs r0, #0
	str r3, [r7, #108]
	movs r1, #5
	movs r2, #0
	str r0, [r7, #20]
	str r0, [r7, #24]
	str r0, [r7, #28]
	movs r0, #10
	bl ObjectMotion_Launch
	movs r0, #8
	.ifdef TLA_EDITION_ES
	bl Field_EndActorSpriteEffect
	.else
	.ifdef TLA_EDITION_IT
	bl Field_EndActorSpriteEffect
	.else
	bl Field_StartPaletteTransition
	.endif
	.endif
	bl Func_020032a4
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	negs r0, r0
	negs r1, r1
	adds r2, #102
	bl Func_02003434
	ldr r3, [r7, #12]
	cmp r3, #0
	ble .L_02008ebe
.L_02008eb2:
	movs r0, #1
	bl WaitFrames
	ldr r3, [r7, #12]
	cmp r3, #0
	bgt .L_02008eb2
.L_02008ebe:
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r7, #72]
	movs r3, #0
	str r3, [r7, #12]
	movs r3, #4
	strb r3, [r5]
	bl Func_020035bc
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r1, #2
	movs r0, #8
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl FieldEvent_WaitFrames
	movs r0, #128
	lsls r0, r0, #6
	movs r2, #5
	adds r0, #8
	movs r1, #0
	bl Func_0200351c
	movs r1, #128
	lsls r1, r1, #8
	movs r0, #8
	bl Func_02003534
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r0, #128
	lsls r0, r0, #6
	movs r2, #5
	adds r0, #8
	movs r1, #0
	bl Func_0200351c
	movs r1, #160
	lsls r1, r1, #7
	movs r0, #8
	bl Func_02003534
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r0, #10
	movs r1, #2
	bl ObjectMotion_SetActionVariant
	bl Func_02003464
	add sp, #64
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008f3c:
	.4byte gPartyState
.L_02008f40:
	.4byte gData_02003634
.L_02008f44:
	.4byte Func_02000038
	.section .text.x02008f48,"ax",%progbits
	.global Func_02000f48
	.thumb_func
Func_02000f48:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r8
	push {r5, r6}
	ldr r3, .L_02009124
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	adds r6, r0, #0
	ldr r0, [r3]
	bl Object_GetById
	ldr r3, [r0, #8]
	asrs r3, r3, #20
	cmp r3, #41
	ble .L_02008f94
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r1, #173
	lsls r1, r1, #1
	adds r2, r3, r1
	movs r3, #1
	strh r3, [r2]
	b .L_0200911c
.L_02008f7a:
	adds r0, r5, #0
	bl PartyInventory_Remove
	b .L_02008fbc
.L_02008f82:
	movs r5, #186
	adds r5, #255
	adds r0, r5, #0
	bl PartyInventory_FindOwner
	cmp r0, r8
	bne .L_02008f7a
	adds r5, r0, #0
	b .L_02008fbc
.L_02008f94:
	movs r5, #184
	adds r5, #255
	adds r0, r5, #0
	bl PartyInventory_FindOwner
	movs r2, #1
	negs r2, r2
	mov r10, r0
	cmp r0, r2
	bne .L_02008f7a
	adds r5, #1
	adds r0, r5, #0
	bl PartyInventory_FindOwner
	mov r8, r0
	cmp r0, r10
	beq .L_02008f82
	adds r0, r5, #0
	bl PartyInventory_Remove
.L_02008fbc:
	movs r3, #1
	negs r3, r3
	cmp r5, r3
	beq .L_020090ae
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #117
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008fe4
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #117
	bl GameFlag_SetBit
	adds r0, r5, #0
	bl Func_02000804
	b .L_0200911c
.L_02008fe4:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #118
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02009004
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #118
	bl GameFlag_SetBit
	adds r0, r5, #0
	bl Func_02000978
	b .L_0200911c
.L_02009004:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #119
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009014
	b .L_0200911c
.L_02009014:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #119
	bl GameFlag_SetBit
	movs r0, #164
	movs r1, #208
	movs r3, #4
	lsls r0, r0, #18
	lsls r1, r1, #16
	movs r2, #0
	negs r3, r3
	bl Func_0200343c
	movs r0, #164
	movs r1, #224
	movs r3, #4
	lsls r0, r0, #18
	lsls r1, r1, #16
	movs r2, #0
	negs r3, r3
	bl Func_0200343c
	movs r0, #164
	movs r1, #240
	movs r3, #4
	lsls r0, r0, #18
	lsls r1, r1, #16
	movs r2, #0
	negs r3, r3
	bl Func_0200343c
	movs r0, #164
	movs r1, #128
	movs r3, #4
	lsls r1, r1, #17
	movs r2, #0
	negs r3, r3
	lsls r0, r0, #18
	bl Func_0200343c
	adds r0, r5, #0
	bl Func_02000a34
	movs r0, #164
	movs r1, #208
	lsls r0, r0, #18
	lsls r1, r1, #16
	movs r2, #0
	movs r3, #0
	bl Func_0200343c
	movs r0, #164
	movs r1, #224
	lsls r0, r0, #18
	lsls r1, r1, #16
	movs r2, #0
	movs r3, #0
	bl Func_0200343c
	movs r0, #164
	movs r1, #240
	lsls r0, r0, #18
	lsls r1, r1, #16
	movs r2, #0
	movs r3, #0
	bl Func_0200343c
	movs r0, #164
	movs r1, #128
	lsls r0, r0, #18
	lsls r1, r1, #17
	movs r2, #0
	movs r3, #0
	bl Func_0200343c
	b .L_0200911c
.L_020090ae:
	ldr r3, .L_02009124
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	adds r0, r6, #0
	ldr r1, [r3]
	movs r2, #5
	bl ObjectMotion_SetAngleToward
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #117
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020090d6
	ldr r0, .L_02009128
	bl Func_0200350c
	b .L_02009108
.L_020090d6:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #118
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020090ec
	ldr r0, .L_0200912c
	bl Func_0200350c
	b .L_02009108
.L_020090ec:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #119
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02009102
	ldr r0, .L_02009130
	bl Func_0200350c
	b .L_02009108
.L_02009102:
	ldr r0, .L_02009134
	bl Func_0200350c
.L_02009108:
	adds r0, r6, #0
	movs r1, #0
	bl Func_02003524
	movs r1, #160
	adds r0, r6, #0
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
.L_0200911c:
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, pc}
.L_02009124:
	.4byte gPartyState
.L_02009128:
	.4byte MsgIMeanIt
.L_0200912c:
	.4byte MsgTheLegendSaysThat
.L_02009130:
	.4byte MsgYesThereS
.L_02009134:
	.4byte MsgTheTridentSeems
	.section .text.x0200915c,"ax",%progbits
	.global Func_0200115c
	.thumb_func
Func_0200115c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r0, #13
	sub sp, #36
	bl Object_GetById
	adds r7, r0, #0
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #126
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200917e
	b .L_0200974a
.L_0200917e:
	bl Func_0200345c
	movs r0, #0
	bl Func_020035b4
	ldr r3, .L_02009544
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r3, r2
	movs r1, #146
	ldr r0, [r5]
	lsls r1, r1, #2
	movs r2, #248
	bl ObjectMotion_SetPositionAndReset
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #116
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009216
	movs r1, #0
	ldr r0, [r5]
	bl Func_02003534
	ldr r0, .L_02009548
	bl Func_0200350c
	movs r1, #0
	movs r2, #5
	movs r0, #11
	bl Func_0200351c
	movs r0, #36
	bl Func_0200362c
	movs r0, #166
	movs r1, #1
	movs r2, #128
	movs r3, #1
	lsls r2, r2, #17
	negs r1, r1
	lsls r0, r0, #18
	bl Camera_MoveWithinMapBounds
	bl Func_02003564
	movs r0, #5
	bl FieldEvent_WaitFrames
	movs r0, #8
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #8
	movs r1, #0
	movs r2, #5
	bl Func_0200351c
	movs r1, #10
	adds r1, #255
	movs r2, #30
	movs r0, #8
	bl Func_02003544
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #8
	movs r1, #0
	movs r2, #5
	bl Func_0200351c
	b .L_02009466
.L_02009216:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #116
	bl GameFlag_SetBit
	ldr r0, .L_0200954c
	bl Func_0200350c
	movs r1, #0
	movs r2, #5
	movs r0, #11
	bl Func_0200351c
	movs r0, #36
	bl Func_0200362c
	ldr r0, [r5]
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #166
	movs r1, #1
	movs r2, #128
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #17
	lsls r0, r0, #18
	bl Camera_MoveWithinMapBounds
	bl Func_02003564
	movs r0, #5
	bl FieldEvent_WaitFrames
	movs r0, #11
	movs r1, #6
	movs r2, #15
	bl ObjectMotion_Launch
	movs r0, #11
	movs r1, #6
	movs r2, #23
	bl ObjectMotion_Launch
	movs r0, #11
	movs r1, #0
	movs r2, #5
	bl Func_0200351c
	movs r1, #8
	adds r1, #255
	movs r2, #30
	movs r0, #11
	bl Func_02003544
	movs r0, #11
	movs r1, #0
	movs r2, #5
	bl Func_0200351c
	movs r1, #2
	movs r2, #30
	adds r1, #255
	movs r0, #8
	bl Func_02003544
	movs r1, #192
	lsls r1, r1, #6
	movs r0, #8
	bl Func_02003534
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r0, #8
	movs r1, #0
	movs r2, #5
	bl Func_0200351c
	movs r1, #128
	movs r2, #30
	lsls r1, r1, #1
	movs r0, #11
	bl Func_02003544
	movs r1, #176
	lsls r1, r1, #8
	movs r0, #11
	bl Func_02003534
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r0, #11
	movs r1, #0
	movs r2, #5
	bl Func_0200351c
	movs r1, #8
	adds r1, #255
	movs r2, #30
	movs r0, #11
	bl Func_02003544
	movs r2, #5
	movs r0, #11
	movs r1, #0
	bl Func_0200351c
	movs r0, #8
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r2, #5
	movs r0, #8
	movs r1, #0
	bl Func_0200351c
	movs r1, #128
	lsls r1, r1, #8
	movs r0, #8
	bl Func_02003534
	movs r0, #5
	bl FieldEvent_WaitFrames
	movs r0, #8
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r2, #5
	movs r0, #8
	movs r1, #0
	bl Func_0200351c
	movs r1, #128
	lsls r1, r1, #8
	movs r0, #11
	bl Func_02003534
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r1, #176
	lsls r1, r1, #8
	movs r0, #11
	bl Func_02003534
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #11
	bl Func_02003544
	movs r2, #5
	movs r0, #11
	movs r1, #0
	bl Func_0200351c
	movs r1, #128
	lsls r1, r1, #8
	movs r0, #11
	bl Func_02003534
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r1, #2
	movs r0, #11
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl FieldEvent_WaitFrames
	movs r0, #11
	movs r1, #0
	movs r2, #5
	bl Func_0200351c
	movs r1, #2
	adds r1, #255
	movs r2, #30
	movs r0, #8
	bl Func_02003544
	movs r2, #5
	movs r0, #8
	movs r1, #0
	bl Func_0200351c
	movs r1, #176
	lsls r1, r1, #8
	movs r0, #11
	bl Func_02003534
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r1, #10
	movs r2, #45
	adds r1, #255
	movs r0, #11
	bl Func_02003544
	movs r1, #192
	lsls r1, r1, #6
	adds r1, #10
	movs r0, #11
	bl Func_02003534
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r0, #11
	movs r1, #0
	movs r2, #5
	bl Func_0200351c
	movs r1, #129
	movs r2, #30
	lsls r1, r1, #1
	movs r0, #8
	bl Func_02003544
	movs r1, #192
	lsls r1, r1, #6
	movs r0, #8
	bl Func_02003534
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r2, #5
	movs r0, #8
	movs r1, #0
	bl Func_0200351c
	movs r1, #176
	lsls r1, r1, #8
	movs r0, #11
	bl Func_02003534
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r1, #4
	movs r0, #8
	bl Motion_SetModeAndWaitAnimation
	movs r0, #5
	bl FieldEvent_WaitFrames
	movs r1, #128
	lsls r1, r1, #8
	movs r0, #8
	bl Func_02003534
	movs r0, #5
	bl FieldEvent_WaitFrames
	movs r2, #5
	movs r0, #8
	movs r1, #0
	bl Func_0200351c
	movs r1, #3
	movs r0, #11
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl FieldEvent_WaitFrames
	movs r1, #3
	movs r0, #11
	bl Motion_SetModeAndWaitAnimation
	movs r0, #5
	bl FieldEvent_WaitFrames
	movs r0, #11
	movs r1, #0
	movs r2, #5
	bl Func_0200351c
	movs r1, #128
	movs r0, #11
	lsls r1, r1, #8
	bl Func_02003534
	movs r0, #5
	bl FieldEvent_WaitFrames
.L_02009466:
	ldr r0, .L_02009550
	bl Func_0200350c
	movs r1, #176
	lsls r1, r1, #8
	movs r0, #8
	bl Func_02003534
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r0, #8
	bl Object_GetById
	movs r3, #0
	mov r10, r3
	movs r3, #160
	lsls r3, r3, #7
	strh r3, [r0, #6]
	movs r1, #5
	movs r0, #8
	bl Object_SetModeById
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r0, #8
	movs r1, #6
	bl Object_SetModeById
	movs r1, #181
	movs r2, #252
	lsls r2, r2, #16
	lsls r1, r1, #18
	movs r0, #12
	bl Func_020034d4
	movs r0, #12
	bl Object_GetById
	ldr r5, .L_02009554
	str r5, [r0, #24]
	movs r0, #12
	bl Object_GetById
	str r5, [r0, #28]
	movs r0, #12
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	movs r0, #128
	lsls r0, r0, #6
	movs r1, #0
	adds r0, #8
	bl UiText_OpenMessageAtObject
	movs r0, #4
	movs r1, #0
	bl Event_AskYesNoWithActorAnimation
	cmp r0, #0
	bne .L_02009558
	bl Func_020035bc
	movs r0, #15
	bl FieldEvent_WaitFrames
	movs r2, #0
	movs r0, #12
	movs r1, #0
	bl Func_020034d4
	movs r1, #1
	movs r0, #8
	bl Object_SetModeById
	movs r0, #8
	bl Object_GetById
	movs r3, #128
	lsls r3, r3, #8
	strh r3, [r0, #6]
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #8
	movs r1, #0
	movs r2, #5
	bl Func_0200351c
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r1, #16
	adds r3, #1
	strh r3, [r2]
	movs r0, #4
	negs r1, r1
	movs r2, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #5
	bl FieldEvent_WaitFrames
	b .L_02009746
	.2byte 0x0000
.L_02009544:
	.4byte gPartyState
.L_02009548:
	.4byte MsgBackForMore
.L_0200954c:
	.4byte MsgHoldItRight
.L_02009550:
	.4byte MsgOhAllRightListen
.L_02009554:
	.4byte 0x00013333
.L_02009558:
	movs r0, #25
	bl FieldEvent_WaitFrames
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
	adds r0, #8
	movs r1, #0
	movs r2, #5
	bl Func_0200351c
	movs r1, #8
	adds r1, #255
	movs r2, #30
	movs r0, #8
	bl Func_02003544
	movs r0, #128
	lsls r0, r0, #6
	movs r1, #0
	movs r2, #5
	adds r0, #8
	bl Func_0200351c
	movs r0, #78
	bl Func_0200362c
	movs r0, #164
	movs r1, #208
	movs r3, #4
	lsls r0, r0, #18
	lsls r1, r1, #16
	movs r2, #0
	negs r3, r3
	bl Func_0200343c
	movs r0, #164
	movs r1, #224
	movs r3, #4
	lsls r0, r0, #18
	lsls r1, r1, #16
	movs r2, #0
	negs r3, r3
	bl Func_0200343c
	movs r0, #164
	movs r1, #240
	movs r3, #4
	lsls r0, r0, #18
	lsls r1, r1, #16
	movs r2, #0
	negs r3, r3
	bl Func_0200343c
	movs r0, #164
	movs r1, #128
	movs r3, #4
	lsls r0, r0, #18
	lsls r1, r1, #17
	movs r2, #0
	negs r3, r3
	bl Func_0200343c
	ldr r3, .L_02009754
	movs r5, #128
	movs r2, #85
	lsls r5, r5, #7
	adds r2, r2, r7
	movs r6, #2
	str r5, [r7, #72]
	mov r8, r2
	strb r6, [r2]
	movs r1, #10
	str r3, [r7, #20]
	movs r2, #0
	movs r0, #12
	bl ObjectMotion_Launch
	movs r0, #146
	bl Func_0200362c
	movs r0, #12
	movs r1, #1
	bl ObjectMotion_SetActionVariant
	movs r1, #192
	movs r2, #192
	movs r0, #12
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl ObjectMotion_SetSpeedParameters
	movs r1, #161
	lsls r1, r1, #2
	movs r2, #248
	movs r0, #12
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r0, #21
	bl FieldEvent_WaitFrames
	movs r1, #0
	movs r2, #0
	movs r0, #12
	bl Func_020034d4
	movs r0, #50
	bl FieldEvent_WaitFrames
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #10
	lsls r2, r2, #9
	lsls r0, r0, #10
	bl Func_02003434
	movs r0, #70
	bl FieldEvent_WaitFrames
	movs r0, #148
	bl Func_0200362c
	movs r0, #25
	bl FieldEvent_WaitFrames
	movs r2, #160
	lsls r2, r2, #19
	adds r2, #228
	ldrh r3, [r2]
	add r0, sp, #24
	strh r3, [r0]
	adds r2, #2
	ldrh r3, [r2]
	adds r2, #2
	strh r3, [r0, #2]
	add r1, sp, #12
	ldrh r3, [r2]
	adds r2, #2
	strh r3, [r0, #4]
	ldrh r3, [r2]
	strh r3, [r0, #6]
	ldrh r3, [r2, #2]
	ldr r2, .L_02009758
	strh r3, [r0, #8]
	ldrh r3, [r2]
	adds r2, #2
	strh r3, [r1]
	ldrh r3, [r2]
	adds r2, #2
	strh r3, [r1, #2]
	ldrh r3, [r2]
	adds r2, #2
	strh r3, [r1, #4]
	ldrh r3, [r2]
	strh r3, [r1, #6]
	ldrh r3, [r2, #2]
	movs r2, #48
	strh r3, [r1, #8]
	bl Func_02000734
	movs r0, #20
	bl FieldEvent_WaitFrames
	movs r0, #194
	bl Func_0200362c
	ldr r3, .L_0200975c
	mov r0, sp
	str r3, [r0]
	mov r3, r10
	str r3, [r0, #4]
	movs r3, #238
	lsls r3, r3, #16
	str r3, [r0, #8]
	bl Func_02002f58
	movs r0, #147
	bl Func_0200362c
	movs r1, #166
	movs r2, #248
	movs r0, #13
	lsls r1, r1, #18
	lsls r2, r2, #16
	bl Func_020034d4
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r7, #72]
	mov r2, r8
	mov r3, r10
	strb r6, [r2]
	str r3, [r7, #20]
	movs r3, #128
	lsls r3, r3, #14
	str r3, [r7, #12]
	ldr r3, .L_02009760
	str r5, [r7, #24]
	str r3, [r7, #108]
	str r5, [r7, #28]
	movs r1, #11
	movs r2, #0
	movs r0, #13
	bl ObjectMotion_Launch
	movs r0, #147
	bl Func_0200362c
	movs r0, #13
	ldr r1, .L_02009764
	ldr r2, .L_02009768
	bl ObjectMotion_SetSpeedParameters
	movs r1, #146
	movs r2, #248
	movs r0, #13
	lsls r1, r1, #2
	bl ObjectMotion_SetPositionAndReset
	ldr r5, .L_0200976c
	movs r1, #98
	adds r0, r5, #0
	bl Party_SetFields1f2And1f4
	adds r0, r5, #0
	movs r1, #99
	bl Party_SetFields1eeAnd1f0
	ldr r3, .L_02009770
	movs r2, #166
	lsls r2, r2, #1
	adds r2, #255
	adds r3, r3, r2
	strb r6, [r3]
	movs r0, #12
	movs r1, #5
	bl Func_0200357c
.L_02009746:
	bl Func_02003464
.L_0200974a:
	add sp, #36
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_02009754:
	.4byte 0xfc190000
.L_02009758:
	.4byte 0x050001a4
.L_0200975c:
	.4byte 0x02960000
.L_02009760:
	.4byte Func_02001138
.L_02009764:
	.4byte 0x00033333
.L_02009768:
	.4byte 0x00019999
.L_0200976c:
	.4byte 0x000000bc
.L_02009770:
	.4byte gPartyState
	.section .text.x0200a9f8,"ax",%progbits
	.global Func_020029f8
	.thumb_func
Func_020029f8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #20
	mov r9, r1
	movs r1, #133
	str r0, [sp, #4]
	lsls r1, r1, #3
	movs r0, #220
	bl Runtime_AllocateHeapBlockFar
	mov r10, r0
	ldr r0, [sp, #4]
	bl Object_GetById
	mov r1, r9
	adds r7, r0, #0
	cmp r1, #0
	bne .L_0200aa3e
	movs r5, #15
.L_0200aa28:
	ldrh r3, [r7, #6]
	movs r2, #128
	lsls r2, r2, #6
	adds r3, r3, r2
	strh r3, [r7, #6]
	movs r0, #1
	subs r5, #1
	bl WaitFrames
	cmp r5, #0
	bge .L_0200aa28
.L_0200aa3e:
	movs r0, #2
	bl WaitFrames
	movs r3, #0
	str r3, [r7, #24]
	adds r0, r7, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	movs r3, #128
	movs r2, #128
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	ldr r0, .L_0200ac18
	ldr r1, .L_0200ac1c
	adds r2, #16
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r0, .L_0200ac20
	bl Resource_GetTableEntry
	mov r1, r10
	bl Func_020033dc
	bl Resource_FindFreeEntry
	movs r1, #128
	lsls r1, r1, #3
	mov r2, r10
	mov r11, r0
	bl VramCache_Load
	movs r6, #128
	lsls r6, r6, #3
	add r6, r10
	movs r3, #128
	str r0, [sp, #0]
	movs r1, #8
	movs r2, #16
	lsls r3, r3, #23
	mov r8, r0
	adds r0, r6, #0
	bl Func_020035fc
	movs r3, #240
	strh r3, [r6, #30]
	ldrb r3, [r6, #9]
	movs r2, #13
	negs r2, r2
	ldrb r1, [r6, #5]
	ands r2, r3
	movs r3, #4
	orrs r2, r3
	movs r3, #33
	negs r3, r3
	ands r3, r1
	strb r3, [r6, #5]
	movs r3, #240
	orrs r2, r3
	strb r2, [r6, #9]
	ldr r3, [r7, #8]
	add r5, sp, #8
	str r3, [r5]
	ldr r3, [r7, #12]
	adds r0, r5, #0
	str r3, [r5, #4]
	ldr r3, [r7, #16]
	str r3, [r5, #8]
	bl Func_020035c4
	ldr r3, [r5]
	str r3, [r6, #12]
	mov r3, r9
	ldr r2, [r5, #8]
	str r2, [r6, #16]
	cmp r3, #1
	bne .L_0200aaea
	movs r3, #128
	movs r1, #160
	lsls r3, r3, #10
	lsls r1, r1, #12
	str r3, [r6, #20]
	str r3, [r6, #24]
	adds r3, r2, r1
	str r3, [r6, #16]
.L_0200aaea:
	movs r0, #145
	bl Func_0200362c
	movs r5, #0
.L_0200aaf2:
	movs r6, #128
	lsls r6, r6, #3
	subs r3, r5, #6
	add r6, r10
	cmp r3, #56
	bls .L_0200ab00
	b .L_0200ac50
.L_0200ab00:
	ldr r2, .L_0200ac24
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
.L_0200ab08:
	.4byte .L_0200abec
	.4byte .L_0200ac50
	.4byte .L_0200ac50
	.4byte .L_0200ac50
	.4byte .L_0200ac50
	.4byte .L_0200ac50
	.4byte .L_0200abf4
	.4byte .L_0200ac50
	.4byte .L_0200ac50
	.4byte .L_0200ac50
	.4byte .L_0200ac50
	.4byte .L_0200ac50
	.4byte .L_0200ac50
	.4byte .L_0200ac50
	.4byte .L_0200ac50
	.4byte .L_0200ac50
	.4byte .L_0200abfc
	.4byte .L_0200ac50
	.4byte .L_0200ac50
	.4byte .L_0200ac50
	.4byte .L_0200ac50
	.4byte .L_0200ac50
	.4byte .L_0200ac50
	.4byte .L_0200ac50
	.4byte .L_0200ac50
	.4byte .L_0200ac50
	.4byte .L_0200ac04
	.4byte .L_0200ac50
	.4byte .L_0200ac50
	.4byte .L_0200ac50
	.4byte .L_0200ac50
	.4byte .L_0200ac50
	.4byte .L_0200ac50
	.4byte .L_0200ac50
	.4byte .L_0200ac50
	.4byte .L_0200ac50
	.4byte .L_0200ac0c
	.4byte .L_0200ac50
	.4byte .L_0200ac50
	.4byte .L_0200ac50
	.4byte .L_0200ac50
	.4byte .L_0200ac50
	.4byte .L_0200ac50
	.4byte .L_0200ac50
	.4byte .L_0200ac50
	.4byte .L_0200ac50
	.4byte .L_0200ac28
	.4byte .L_0200ac50
	.4byte .L_0200ac50
	.4byte .L_0200ac50
	.4byte .L_0200ac50
	.4byte .L_0200ac50
	.4byte .L_0200ac50
	.4byte .L_0200ac50
	.4byte .L_0200ac50
	.4byte .L_0200ac50
	.4byte .L_0200ac34
.L_0200abec:
	ldr r3, .L_0200ac14
	mov r2, r8
	adds r2, #4
	b .L_0200ac3a
.L_0200abf4:
	ldr r3, .L_0200ac14
	mov r2, r8
	adds r2, #8
	b .L_0200ac3a
.L_0200abfc:
	ldr r3, .L_0200ac14
	mov r2, r8
	adds r2, #12
	b .L_0200ac3a
.L_0200ac04:
	ldr r3, .L_0200ac14
	mov r2, r8
	adds r2, #16
	b .L_0200ac3a
.L_0200ac0c:
	ldr r3, .L_0200ac14
	mov r2, r8
	adds r2, #20
	b .L_0200ac3a
.L_0200ac14:
	.4byte 0x000003ff
.L_0200ac18:
	.4byte Field_Palette685
.L_0200ac1c:
	.4byte 0x050003e0
.L_0200ac20:
	.4byte 0x000001e8
.L_0200ac24:
	.4byte .L_0200ab08
.L_0200ac28:
	ldr r3, .L_0200ac30
	mov r2, r8
	adds r2, #24
	b .L_0200ac3a
.L_0200ac30:
	.4byte 0x000003ff
.L_0200ac34:
	ldr r3, .L_0200ac48
	mov r2, r8
	adds r2, #28
.L_0200ac3a:
	ands r2, r3
	ldrh r1, [r6, #8]
	ldr r3, .L_0200ac4c
	ands r3, r1
	orrs r3, r2
	strh r3, [r6, #8]
	b .L_0200ac50
.L_0200ac48:
	.4byte 0x000003ff
.L_0200ac4c:
	.4byte 0xfffffc00
.L_0200ac50:
	adds r0, r6, #0
	bl Func_02003604
	adds r5, #1
	movs r0, #1
	bl WaitFrames
	cmp r5, #71
	bgt .L_0200ac64
	b .L_0200aaf2
.L_0200ac64:
	ldr r0, [sp, #4]
	movs r1, #0
	movs r2, #0
	bl Func_020034d4
	mov r0, r11
	bl Resource_ResetEntry
	movs r0, #220
	bl Runtime_ReleaseHeapBlock
	add sp, #20
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.section .text.x0200ac88,"ax",%progbits
	.global Func_02002c88
	.thumb_func
Func_02002c88:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r7, [r3]
	ldr r0, .L_0200ad30
	sub sp, #4
	bl Resource_GetTableEntry
	adds r1, r7, #0
	bl Func_020033dc
	movs r0, #0
	adds r2, r7, #0
.L_0200aca4:
	ldrb r1, [r2]
	movs r3, #240
	ands r3, r1
	cmp r3, #240
	bne .L_0200acb6
	movs r3, #15
	ands r3, r1
	adds r3, #96
	strb r3, [r2]
.L_0200acb6:
	movs r3, #128
	adds r0, #1
	lsls r3, r3, #5
	adds r2, #1
	cmp r0, r3
	blt .L_0200aca4
	bl Resource_FindFreeEntry
	movs r1, #128
	adds r2, r7, #0
	lsls r1, r1, #5
	adds r5, r0, #0
	bl VramCache_Load
	movs r2, #128
	lsls r2, r2, #5
	adds r2, #92
	adds r3, r7, r2
	adds r2, #2
	strh r5, [r3]
	adds r3, r7, r2
	strh r0, [r3]
	movs r3, #128
	lsls r3, r3, #5
	adds r3, #12
	adds r5, r7, r3
	movs r3, #192
	str r0, [sp, #0]
	movs r1, #31
	adds r0, r5, #0
	movs r2, #31
	lsls r3, r3, #24
	bl Func_020035fc
	ldrb r3, [r5, #5]
	movs r2, #32
	orrs r3, r2
	ldrb r2, [r5, #9]
	strb r3, [r5, #5]
	movs r3, #15
	ands r3, r2
	movs r2, #13
	negs r2, r2
	ands r3, r2
	movs r2, #8
	orrs r3, r2
	movs r2, #128
	lsls r2, r2, #5
	strb r3, [r5, #9]
	adds r2, #100
	movs r3, #1
	strh r3, [r5, #30]
	movs r6, #0
	adds r3, r7, r2
	adds r2, #4
	str r6, [r3]
	adds r3, r7, r2
	str r6, [r3]
	add sp, #4
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200ad30:
	.4byte 0x000001d9
	.section .text.x0200ad34,"ax",%progbits
	.global Func_02002d34
	.thumb_func
Func_02002d34:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r7, [r3]
	ldr r0, .L_0200addc
	sub sp, #4
	bl Resource_GetTableEntry
	adds r1, r7, #0
	bl Func_020033dc
	movs r0, #0
	adds r2, r7, #0
.L_0200ad50:
	ldrb r1, [r2]
	movs r3, #240
	ands r3, r1
	cmp r3, #240
	bne .L_0200ad62
	movs r3, #15
	ands r3, r1
	adds r3, #96
	strb r3, [r2]
.L_0200ad62:
	movs r3, #128
	adds r0, #1
	lsls r3, r3, #5
	adds r2, #1
	cmp r0, r3
	blt .L_0200ad50
	bl Resource_FindFreeEntry
	movs r1, #128
	adds r2, r7, #0
	lsls r1, r1, #4
	adds r5, r0, #0
	bl VramCache_Load
	movs r2, #131
	lsls r2, r2, #5
	adds r3, r7, r2
	adds r2, #2
	strh r5, [r3]
	adds r3, r7, r2
	strh r0, [r3]
	movs r3, #128
	lsls r3, r3, #5
	adds r3, #52
	adds r5, r7, r3
	str r0, [sp, #0]
	movs r1, #15
	adds r0, r5, #0
	movs r2, #47
	ldr r3, .L_0200ade0
	bl Func_020035fc
	ldrb r3, [r5, #5]
	movs r2, #32
	orrs r3, r2
	ldrb r2, [r5, #9]
	strb r3, [r5, #5]
	movs r3, #15
	ands r3, r2
	movs r2, #16
	orrs r3, r2
	movs r2, #13
	negs r2, r2
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	movs r2, #128
	lsls r2, r2, #5
	strb r3, [r5, #9]
	adds r2, #108
	movs r3, #1
	strh r3, [r5, #30]
	movs r6, #0
	adds r3, r7, r2
	adds r2, #4
	str r6, [r3]
	adds r3, r7, r2
	str r6, [r3]
	add sp, #4
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200addc:
	.4byte 0x000001f7
.L_0200ade0:
	.4byte 0xc0008000
	.section .text.x0200ade4,"ax",%progbits
	.global Func_02002de4
	.thumb_func
Func_02002de4:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r3, [r3]
	sub sp, #16
	mov r11, r3
	movs r7, #0
.L_0200ae00:
	movs r6, #128
	lsls r6, r6, #5
	adds r6, #116
	movs r0, #31
	add r6, r11
	mov r8, r0
	movs r2, #0
	ldrsh r0, [r6, r2]
	adds r0, r0, r7
	lsls r0, r0, #11
	bl Math_Sine
	ldrh r3, [r6]
	lsls r5, r0, #1
	lsls r3, r3, #16
	adds r5, r5, r0
	asrs r0, r3, #16
	lsrs r3, r3, #31
	adds r0, r0, r3
	asrs r0, r0, #1
	adds r0, r0, r7
	movs r3, #144
	lsls r3, r3, #7
	lsls r0, r0, #11
	adds r0, r0, r3
	bl Math_Sine
	lsls r3, r0, #1
	lsls r5, r5, #1
	adds r3, r3, r0
	asrs r5, r5, #16
	asrs r3, r3, #16
	adds r5, #20
	adds r3, #3
	lsls r2, r3, #10
	lsls r5, r5, #5
	adds r1, r7, #1
	cmp r7, #15
	bgt .L_0200ae5a
	mov r0, r8
	orrs r2, r5
	orrs r2, r0
	movs r3, #111
	ldr r0, .L_0200af54
	b .L_0200ae64
.L_0200ae5a:
	mov r3, r8
	orrs r2, r5
	orrs r2, r3
	ldr r0, .L_0200af54
	movs r3, #255
.L_0200ae64:
	subs r3, r3, r7
	lsls r3, r3, #1
	adds r3, r3, r0
	strh r2, [r3]
	adds r7, r1, #0
	cmp r7, #31
	ble .L_0200ae00
	movs r2, #128
	lsls r2, r2, #5
	add r2, r11
	ldr r3, [r2]
	add r5, sp, #4
	str r3, [r5]
	movs r3, #128
	lsls r3, r3, #5
	adds r3, #4
	add r3, r11
	mov r8, r3
	ldr r3, [r3]
	movs r6, #128
	str r3, [r5, #4]
	lsls r6, r6, #5
	adds r6, #8
	add r6, r11
	ldr r3, [r6]
	adds r0, r5, #0
	str r3, [r5, #8]
	mov r10, r2
	bl Func_020035c4
	movs r7, #128
	ldr r3, [r5]
	lsls r7, r7, #5
	adds r7, #12
	add r7, r11
	str r3, [r7, #12]
	movs r0, #128
	ldr r3, [r5, #8]
	lsls r0, r0, #5
	str r3, [r7, #16]
	adds r0, #100
	add r0, r11
	movs r2, #128
	ldr r3, [r0]
	lsls r2, r2, #5
	adds r2, #104
	add r2, r11
	str r3, [r7, #20]
	str r2, [sp, #0]
	mov r9, r0
	ldr r3, [r2]
	adds r0, r7, #0
	str r3, [r7, #24]
	bl Func_02003604
	mov r0, r10
	ldr r3, [r0]
	mov r2, r8
	str r3, [r5]
	adds r0, r5, #0
	ldr r3, [r2]
	movs r7, #128
	str r3, [r5, #4]
	lsls r7, r7, #5
	ldr r3, [r6]
	adds r7, #52
	str r3, [r5, #8]
	bl Func_020035c4
	ldr r3, [r5]
	add r7, r11
	str r3, [r7, #12]
	movs r6, #128
	ldr r3, [r5, #8]
	movs r5, #128
	str r3, [r7, #16]
	lsls r5, r5, #5
	adds r5, #108
	add r5, r11
	ldr r3, [r5]
	lsls r6, r6, #5
	str r3, [r7, #20]
	adds r6, #112
	add r6, r11
	ldr r3, [r6]
	adds r0, r7, #0
	str r3, [r7, #24]
	bl Func_02003604
	mov r0, r9
	ldr r3, [r0]
	movs r1, #40
	lsls r0, r3, #5
	subs r0, r0, r3
	bl __divsi3
	ldr r2, [sp, #0]
	movs r1, #40
	str r0, [r2]
	ldr r3, [r5]
	lsls r0, r3, #5
	subs r0, r0, r3
	bl __divsi3
	movs r2, #128
	str r0, [r6]
	lsls r2, r2, #5
	adds r2, #116
	add r2, r11
	ldrh r3, [r2]
	add sp, #16
	adds r3, #1
	strh r3, [r2]
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200af54:
	.4byte 0x05000200
	.section .text.x0200af58,"ax",%progbits
	.global Func_02002f58
	.thumb_func
Func_02002f58:
	push {r5, r6, r7, lr}
	movs r1, #128
	lsls r1, r1, #5
	adds r5, r0, #0
	adds r1, #120
	movs r0, #220
	bl Runtime_AllocateHeapBlockFar
	ldr r3, [r5]
	movs r1, #128
	adds r6, r0, #0
	lsls r1, r1, #5
	adds r2, r6, r1
	str r3, [r2]
	movs r3, #128
	lsls r3, r3, #5
	adds r3, #4
	adds r2, r6, r3
	ldr r3, [r5, #4]
	adds r1, #8
	str r3, [r2]
	adds r2, r6, r1
	ldr r3, [r5, #8]
	movs r7, #0
	str r3, [r2]
	bl Func_02002c88
	bl Func_02002d34
	movs r3, #128
	lsls r3, r3, #5
	adds r3, #116
	adds r2, r6, r3
	movs r1, #144
	movs r3, #0
	strh r3, [r2]
	lsls r1, r1, #3
	ldr r0, .L_0200b074
	bl Scheduler_AddOrUpdateCallback
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r0, r0, #11
	lsls r1, r1, #11
	lsls r2, r2, #9
	bl Func_02003434
	movs r5, #0
.L_0200afba:
	cmp r5, #1
	beq .L_0200affe
	cmp r5, #1
	bgt .L_0200afc8
	cmp r5, #0
	beq .L_0200afd2
	b .L_0200b060
.L_0200afc8:
	cmp r5, #2
	beq .L_0200b00a
	cmp r5, #3
	beq .L_0200b036
	b .L_0200b060
.L_0200afd2:
	movs r1, #128
	lsls r1, r1, #5
	adds r1, #100
	adds r2, r6, r1
	ldr r3, [r2]
	movs r1, #192
	lsls r1, r1, #3
	adds r3, r3, r1
	movs r1, #166
	lsls r1, r1, #9
	adds r1, #203
	str r3, [r2]
	cmp r3, r1
	ble .L_0200b060
	movs r3, #166
	lsls r3, r3, #9
	adds r3, #204
	movs r7, #1
	str r3, [r2]
	movs r5, #1
	negs r7, r7
	b .L_0200b060
.L_0200affe:
	cmp r7, #10
	bne .L_0200b060
	movs r7, #1
	movs r5, #2
	negs r7, r7
	b .L_0200b060
.L_0200b00a:
	movs r3, #128
	lsls r3, r3, #5
	adds r3, #108
	adds r2, r6, r3
	ldr r3, [r2]
	movs r1, #128
	lsls r1, r1, #3
	adds r3, r3, r1
	movs r1, #204
	lsls r1, r1, #7
	adds r1, #101
	str r3, [r2]
	cmp r3, r1
	ble .L_0200b060
	movs r3, #204
	lsls r3, r3, #7
	adds r3, #102
	movs r7, #1
	str r3, [r2]
	movs r5, #3
	negs r7, r7
	b .L_0200b060
.L_0200b036:
	movs r3, #128
	lsls r3, r3, #5
	adds r3, #108
	adds r2, r6, r3
	ldr r3, [r2]
	movs r1, #192
	lsls r1, r1, #6
	adds r3, r3, r1
	movs r1, #166
	lsls r1, r1, #9
	adds r1, #203
	str r3, [r2]
	cmp r3, r1
	ble .L_0200b060
	movs r3, #166
	lsls r3, r3, #9
	adds r3, #204
	movs r5, #186
	str r3, [r2]
	lsls r5, r5, #2
	adds r5, #255
.L_0200b060:
	movs r0, #1
	bl WaitFrames
	movs r3, #186
	lsls r3, r3, #2
	adds r3, #255
	adds r7, #1
	cmp r5, r3
	bne .L_0200afba
	pop {r5, r6, r7, pc}
.L_0200b074:
	.4byte Func_02002de4
	.section .text.x0200b078,"ax",%progbits
	.global Func_02003078
	.thumb_func
Func_02003078:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r6, [r3]
	movs r1, #128
	lsls r1, r1, #5
	adds r3, r6, r1
	ldr r3, [r3]
	sub sp, #12
	mov r2, sp
	str r3, [r2]
	adds r1, #4
	adds r3, r6, r1
	ldr r3, [r3]
	ldr r1, .L_0200b154
	movs r5, #0
	adds r3, r3, r1
	movs r1, #128
	str r3, [r2, #4]
	lsls r1, r1, #5
	adds r1, #8
	adds r3, r6, r1
	ldr r3, [r3]
	movs r7, #0
	str r3, [r2, #8]
.L_0200b0ac:
	cmp r5, #1
	beq .L_0200b0de
	cmp r5, #1
	bgt .L_0200b0ba
	cmp r5, #0
	beq .L_0200b0c0
	b .L_0200b108
.L_0200b0ba:
	cmp r5, #2
	beq .L_0200b0fe
	b .L_0200b108
.L_0200b0c0:
	movs r3, #128
	lsls r3, r3, #5
	adds r3, #108
	adds r2, r6, r3
	ldr r3, [r2]
	ldr r1, .L_0200b158
	adds r3, r3, r1
	str r3, [r2]
	cmp r3, #0
	bge .L_0200b108
	movs r7, #1
	str r5, [r2]
	negs r7, r7
	movs r5, #1
	b .L_0200b108
.L_0200b0de:
	movs r3, #128
	lsls r3, r3, #5
	adds r3, #100
	adds r2, r6, r3
	ldr r3, [r2]
	ldr r1, .L_0200b15c
	adds r3, r3, r1
	str r3, [r2]
	cmp r3, #0
	bge .L_0200b108
	movs r3, #0
	movs r7, #1
	str r3, [r2]
	movs r5, #2
	negs r7, r7
	b .L_0200b108
.L_0200b0fe:
	cmp r7, #1
	bne .L_0200b108
	movs r5, #186
	lsls r5, r5, #2
	adds r5, #255
.L_0200b108:
	movs r0, #1
	bl WaitFrames
	movs r2, #186
	lsls r2, r2, #2
	adds r2, #255
	adds r7, #1
	cmp r5, r2
	bne .L_0200b0ac
	ldr r0, .L_0200b160
	bl Scheduler_RemoveCallbackFar
	movs r2, #0
	movs r0, #0
	movs r1, #0
	bl Func_02003434
	movs r1, #128
	lsls r1, r1, #5
	adds r1, #92
	adds r3, r6, r1
	movs r2, #0
	ldrsh r0, [r3, r2]
	bl Resource_ResetEntry
	movs r1, #131
	lsls r1, r1, #5
	adds r3, r6, r1
	movs r2, #0
	ldrsh r0, [r3, r2]
	bl Resource_ResetEntry
	movs r0, #220
	bl Runtime_ReleaseHeapBlock
	add sp, #12
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200b154:
	.4byte 0xffe00000
.L_0200b158:
	.4byte 0xfffff000
.L_0200b15c:
	.4byte 0xfffff800
.L_0200b160:
	.4byte Func_02002de4
	.section .text.x0200b164,"ax",%progbits
	.global Func_02003164
	.thumb_func
Func_02003164:
	push {r5, r6, lr}
	movs r1, #128
	lsls r1, r1, #5
	adds r5, r0, #0
	adds r1, #120
	movs r0, #220
	bl Runtime_AllocateHeapBlockFar
	ldr r3, [r5]
	movs r1, #128
	adds r6, r0, #0
	lsls r1, r1, #5
	adds r2, r6, r1
	str r3, [r2]
	movs r3, #128
	lsls r3, r3, #5
	adds r3, #4
	adds r2, r6, r3
	ldr r3, [r5, #4]
	adds r1, #8
	str r3, [r2]
	adds r2, r6, r1
	ldr r3, [r5, #8]
	movs r5, #0
	str r3, [r2]
	bl Func_02002c88
	movs r2, #128
	lsls r2, r2, #5
	movs r1, #128
	adds r2, #108
	lsls r1, r1, #5
	adds r3, r6, r2
	adds r1, #112
	movs r2, #0
	str r2, [r3]
	adds r3, r6, r1
	adds r1, #4
	str r2, [r3]
	adds r3, r6, r1
	movs r1, #144
	strh r2, [r3]
	lsls r1, r1, #3
	ldr r0, .L_0200b208
	bl Scheduler_AddOrUpdateCallback
	movs r0, #139
	bl Func_0200362c
.L_0200b1c6:
	cmp r5, #0
	bne .L_0200b1f4
	movs r3, #128
	lsls r3, r3, #5
	adds r3, #100
	adds r2, r6, r3
	ldr r3, [r2]
	movs r1, #128
	lsls r1, r1, #5
	adds r3, r3, r1
	movs r1, #166
	lsls r1, r1, #9
	adds r1, #203
	str r3, [r2]
	cmp r3, r1
	ble .L_0200b1f4
	movs r3, #166
	lsls r3, r3, #9
	adds r3, #204
	movs r5, #186
	str r3, [r2]
	lsls r5, r5, #2
	adds r5, #255
.L_0200b1f4:
	movs r0, #1
	bl WaitFrames
	movs r2, #186
	lsls r2, r2, #2
	adds r2, #255
	cmp r5, r2
	bne .L_0200b1c6
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200b208:
	.4byte Func_02002de4
	.section .text.x0200b20c,"ax",%progbits
	.global Func_0200320c
	.thumb_func
Func_0200320c:
	push {r5, r6, lr}
	movs r1, #128
	lsls r1, r1, #5
	adds r1, #120
	movs r0, #220
	bl Runtime_AllocateHeapBlockFar
	adds r6, r0, #0
	bl Func_02002d34
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_0200b2a0
	bl Scheduler_AddOrUpdateCallback
	movs r0, #144
	bl Func_0200362c
	movs r5, #0
.L_0200b232:
	cmp r5, #0
	beq .L_0200b23c
	cmp r5, #1
	beq .L_0200b262
	b .L_0200b28c
.L_0200b23c:
	movs r1, #128
	lsls r1, r1, #5
	adds r1, #108
	adds r2, r6, r1
	ldr r3, [r2]
	subs r1, #108
	adds r3, r3, r1
	movs r1, #204
	lsls r1, r1, #7
	adds r1, #101
	str r3, [r2]
	cmp r3, r1
	ble .L_0200b28c
	movs r3, #204
	lsls r3, r3, #7
	adds r3, #102
	str r3, [r2]
	movs r5, #1
	b .L_0200b28c
.L_0200b262:
	movs r3, #128
	lsls r3, r3, #5
	adds r3, #108
	adds r2, r6, r3
	ldr r3, [r2]
	movs r1, #192
	lsls r1, r1, #6
	adds r3, r3, r1
	movs r1, #166
	lsls r1, r1, #9
	adds r1, #203
	str r3, [r2]
	cmp r3, r1
	ble .L_0200b28c
	movs r3, #166
	lsls r3, r3, #9
	adds r3, #204
	movs r5, #186
	str r3, [r2]
	lsls r5, r5, #2
	adds r5, #255
.L_0200b28c:
	movs r0, #1
	bl WaitFrames
	movs r3, #186
	lsls r3, r3, #2
	adds r3, #255
	cmp r5, r3
	bne .L_0200b232
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200b2a0:
	.4byte Func_02002de4
	.section .text.x0200b2a4,"ax",%progbits
	.global Func_020032a4
	.thumb_func
Func_020032a4:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r6, [r3]
	movs r5, #0
	movs r7, #0
.L_0200b2b2:
	cmp r5, #1
	beq .L_0200b2ee
	cmp r5, #1
	bgt .L_0200b2c0
	cmp r5, #0
	beq .L_0200b2ca
	b .L_0200b358
.L_0200b2c0:
	cmp r5, #2
	beq .L_0200b316
	cmp r5, #3
	beq .L_0200b34e
	b .L_0200b358
.L_0200b2ca:
	movs r1, #128
	lsls r1, r1, #5
	adds r1, #108
	adds r2, r6, r1
	ldr r3, [r2]
	subs r1, #108
	adds r3, r3, r1
	ldr r1, .L_0200b398
	str r3, [r2]
	cmp r3, r1
	ble .L_0200b358
	movs r3, #128
	lsls r3, r3, #10
	movs r7, #1
	str r3, [r2]
	movs r5, #1
	negs r7, r7
	b .L_0200b358
.L_0200b2ee:
	movs r3, #128
	lsls r3, r3, #5
	adds r3, #108
	adds r2, r6, r3
	ldr r3, [r2]
	ldr r1, .L_0200b39c
	adds r3, r3, r1
	movs r1, #166
	lsls r1, r1, #9
	adds r1, #203
	str r3, [r2]
	cmp r3, r1
	bgt .L_0200b316
	movs r3, #166
	lsls r3, r3, #9
	adds r3, #204
	str r3, [r2]
	movs r7, #1
	movs r5, #2
	negs r7, r7
.L_0200b316:
	movs r3, #128
	lsls r3, r3, #5
	adds r3, #108
	adds r2, r6, r3
	ldr r3, [r2]
	ldr r1, .L_0200b39c
	adds r3, r3, r1
	str r3, [r2]
	cmp r3, #0
	bge .L_0200b32e
	movs r3, #0
	str r3, [r2]
.L_0200b32e:
	movs r3, #128
	lsls r3, r3, #5
	adds r3, #100
	adds r2, r6, r3
	ldr r3, [r2]
	ldr r1, .L_0200b39c
	adds r3, r3, r1
	str r3, [r2]
	cmp r3, #0
	bge .L_0200b358
	movs r3, #0
	movs r7, #1
	str r3, [r2]
	adds r5, #1
	negs r7, r7
	b .L_0200b358
.L_0200b34e:
	cmp r7, #1
	bne .L_0200b358
	movs r5, #186
	lsls r5, r5, #2
	adds r5, #255
.L_0200b358:
	movs r0, #1
	bl WaitFrames
	movs r2, #186
	lsls r2, r2, #2
	adds r2, #255
	adds r7, #1
	cmp r5, r2
	bne .L_0200b2b2
	ldr r0, .L_0200b3a0
	bl Scheduler_RemoveCallbackFar
	movs r1, #128
	lsls r1, r1, #5
	adds r1, #92
	adds r3, r6, r1
	movs r2, #0
	ldrsh r0, [r3, r2]
	bl Resource_ResetEntry
	movs r1, #131
	lsls r1, r1, #5
	adds r3, r6, r1
	movs r2, #0
	ldrsh r0, [r3, r2]
	bl Resource_ResetEntry
	movs r0, #220
	bl Runtime_ReleaseHeapBlock
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200b398:
	.4byte 0x0001ffff
.L_0200b39c:
	.4byte 0xfffff000
.L_0200b3a0:
	.4byte Func_02002de4
