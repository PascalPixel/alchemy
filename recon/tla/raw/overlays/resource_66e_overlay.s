.syntax unified
	.thumb
	.section .text.x02008390,"ax",%progbits
	.ifdef TLA_EDITION_JA
	.balign 4
	.global Func_02000390
	.thumb_func
Func_02000390:
	.ifdef TLA_EDITION_JA
	push {r5, r6, r7, lr}
	ldr r0, .L_ja_02008724
	sub sp, #8
	bl Func_02001138
	movs r0, #10
	movs r1, #0
	bl Func_02001148
	movs r0, #30
	bl FieldEvent_Wait
.L_ja_020083a8:
	ldr r3, .L_ja_02008728
	movs r1, #133
	lsls r1, r1, #2
	adds r7, r3, r1
	ldr r0, [r7, #0]
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	add r0, sp, #4
	mov r1, sp
	bl Func_020011a0
	movs r2, #1
	negs r2, r2
	cmp r0, r2
	bne .L_ja_020083ea
	ldr r5, .L_ja_0200872c
	adds r0, r5, #0
	bl Func_02001138
	movs r1, #0
	movs r0, #10
	bl FieldMessage_ShowAtActor
	bl Func_02001188
	movs r1, #0
	bl Event_AskYesNoWithActorAnimation
	cmp r0, #1
	bne .L_ja_020083a8
	adds r0, r5, #1
	b .L_ja_02008496
.L_ja_020083ea:
	ldr r0, [sp, #4]
	bl Unit_Get
	ldr r3, [sp, #0]
	adds r5, r0, #0
	lsls r3, r3, #1
	adds r3, #216
	ldrh r0, [r5, r3]
	bl Func_020010b8
	movs r0, #18
	bl Actor_GetById
	ldr r3, [sp, #0]
	lsls r3, r3, #1
	adds r3, #216
	ldrh r1, [r5, r3]
	bl Func_020010a0
	movs r1, #168
	movs r2, #208
	lsls r2, r2, #15
	lsls r1, r1, #18
	movs r0, #18
	bl Func_02001108
	movs r0, #60
	bl FieldEvent_Wait
	ldr r3, [sp, #0]
	movs r1, #2
	lsls r3, r3, #1
	adds r3, #216
	ldrh r0, [r5, r3]
	bl Func_020010b0
	movs r1, #1
	ldr r0, .L_ja_02008730
	bl FieldMessage_Show
	ldr r6, .L_ja_02008734
	adds r0, r6, #0
	bl Func_02001138
	movs r1, #0
	movs r0, #10
	bl FieldMessage_ShowAtActor
	bl Func_02001188
	movs r1, #0
	bl Event_AskYesNoWithActorAnimation
	cmp r0, #1
	bne .L_ja_020084a4
	movs r2, #0
	movs r0, #18
	movs r1, #0
	bl Func_02001108
	ldr r3, [sp, #0]
	movs r1, #2
	lsls r3, r3, #1
	adds r3, #216
	ldrh r0, [r5, r3]
	bl Func_020010b0
	movs r1, #1
	adds r0, r6, #1
	bl FieldMessage_Show
	adds r0, r6, #2
	bl Func_02001138
	movs r1, #0
	movs r0, #10
	bl FieldMessage_ShowAtActor
	bl Func_02001188
	movs r1, #0
	bl Event_AskYesNoWithActorAnimation
	cmp r0, #1
	bne .L_ja_020083a8
	adds r0, r6, #3
.L_ja_02008496:
	bl Func_02001138
	movs r0, #10
	movs r1, #0
	bl Func_02001148
	b .L_ja_02008756
.L_ja_020084a4:
	adds r0, r6, #4
	bl Func_02001138
	movs r0, #10
	movs r1, #0
	bl Func_02001148
	movs r0, #128
	movs r1, #0
	lsls r0, r0, #9
	bl Func_02001170
	movs r0, #30
	bl FieldEvent_Wait
	movs r1, #1
	ldr r0, .L_ja_02008738
	bl Func_02001168
	movs r0, #30
	bl Func_02001178
	movs r0, #120
	bl FieldEvent_Wait
	movs r0, #11
	bl Func_020011a8
	movs r1, #5
	movs r0, #10
	bl Actor_SetModeById
	movs r0, #10
	bl Actor_WaitForAnimationChange
	movs r0, #10
	bl Actor_GetById
	adds r0, #90
	ldrb r2, [r0, #0]
	movs r5, #254
	adds r3, r5, #0
	ands r3, r2
	movs r1, #166
	strb r3, [r0, #0]
	movs r2, #96
	movs r0, #10
	lsls r1, r1, #2
	bl Actor_ResetAndSetPositionInMode2
	movs r1, #5
	movs r0, #10
	bl Actor_SetModeById
	movs r0, #30
	bl FieldEvent_Wait
	movs r0, #10
	bl Actor_GetById
	adds r0, #90
	ldrb r2, [r0, #0]
	adds r3, r5, #0
	ands r3, r2
	movs r1, #170
	strb r3, [r0, #0]
	movs r2, #96
	movs r0, #10
	lsls r1, r1, #2
	bl Actor_ResetAndSetPositionInMode2
	movs r0, #10
	movs r1, #5
	bl Actor_SetModeById
	movs r1, #4
	movs r2, #90
	movs r0, #10
	bl Actor_Jump
	movs r0, #10
	bl Actor_GetById
	adds r0, #90
	ldrb r2, [r0, #0]
	adds r3, r5, #0
	ands r3, r2
	movs r1, #166
	strb r3, [r0, #0]
	movs r2, #96
	movs r0, #10
	lsls r1, r1, #2
	bl Actor_ResetAndSetPositionInMode2
	movs r1, #5
	movs r0, #10
	bl Actor_SetModeById
	movs r0, #30
	bl FieldEvent_Wait
	movs r0, #10
	bl Actor_GetById
	adds r0, #90
	ldrb r2, [r0, #0]
	adds r3, r5, #0
	ands r3, r2
	movs r1, #170
	strb r3, [r0, #0]
	movs r2, #96
	movs r0, #10
	lsls r1, r1, #2
	bl Actor_ResetAndSetPositionInMode2
	movs r0, #10
	movs r1, #5
	bl Actor_SetModeById
	movs r1, #4
	movs r2, #90
	movs r0, #10
	bl Actor_Jump
	movs r0, #10
	bl Actor_GetById
	adds r0, #90
	ldrb r3, [r0, #0]
	movs r1, #168
	ands r5, r3
	strb r5, [r0, #0]
	movs r2, #96
	movs r0, #10
	lsls r1, r1, #2
	bl Actor_ResetAndSetPositionInMode2
	movs r0, #10
	movs r1, #5
	bl Actor_SetModeById
	movs r0, #10
	movs r1, #4
	movs r2, #30
	bl Actor_Jump
	movs r2, #30
	movs r0, #10
	movs r1, #4
	bl Actor_Jump
	movs r0, #10
	movs r1, #5
	bl Actor_SetModeById
	movs r1, #0
	movs r0, #10
	bl Actor_SetModeById
	movs r0, #11
	bl Func_020011a8
	movs r0, #128
	movs r1, #1
	lsls r0, r0, #9
	bl Func_02001168
	movs r0, #30
	bl Func_02001178
	movs r1, #168
	movs r2, #208
	lsls r2, r2, #15
	lsls r1, r1, #18
	movs r0, #20
	bl Func_02001108
	movs r0, #144
	bl Func_020011a8
	movs r1, #2
	movs r0, #20
	bl Actor_SetModeById
	movs r0, #20
	bl Actor_WaitForAnimationChange
	movs r1, #0
	movs r2, #0
	movs r0, #20
	bl Func_02001108
	movs r0, #4
	bl FieldEvent_Wait
	movs r1, #168
	movs r2, #208
	lsls r2, r2, #15
	movs r0, #19
	lsls r1, r1, #18
	bl Func_02001108
	movs r1, #4
	movs r0, #19
	bl Actor_SetModeById
	movs r0, #19
	bl Actor_WaitForAnimationChange
	movs r0, #15
	bl FieldEvent_Wait
	movs r1, #0
	movs r2, #0
	movs r0, #19
	bl Func_02001108
	movs r0, #30
	bl FieldEvent_Wait
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #0
	ldr r0, [r7, #0]
	bl Func_02001160
	movs r0, #60
	bl FieldEvent_Wait
	movs r0, #10
	bl Func_020011a8
	movs r1, #128
	movs r2, #60
	lsls r1, r1, #1
	movs r0, #10
	bl Func_02001160
	movs r1, #0
	movs r0, #10
	bl Func_02001148
	ldr r0, [sp, #4]
	bl Unit_Get
	ldr r3, [sp, #0]
	adds r5, r0, #0
	lsls r3, r3, #1
	adds r3, #216
	ldrh r0, [r5, r3]
	bl Func_020010b8
	ldr r3, [sp, #0]
	ldr r1, .L_ja_0200873c
	lsls r3, r3, #1
	adds r3, #216
	adds r2, r0, #0
	ldrh r0, [r5, r3]
	adds r3, r0, r1
	movs r1, #128
	lsls r3, r3, #16
	lsls r1, r1, #10
	cmp r3, r1
	bhi .L_ja_020086ba
	bl Func_02000920
	b .L_ja_02008744
.L_ja_020086ba:
	movs r1, #163
	lsls r3, r0, #16
	lsls r1, r1, #17
	cmp r3, r1
	bne .L_ja_020086f8
	adds r0, r6, #0
	adds r0, #29
	bl Func_02001138
	movs r0, #10
	movs r1, #0
	bl Func_02001148
	movs r0, #148
	lsls r0, r0, #4
	bl GameFlag_Test
	cmp r0, #0
	bne .L_ja_020086e6
	adds r0, r6, #0
	adds r0, #30
	b .L_ja_020086ea
.L_ja_020086e6:
	adds r0, r6, #0
	adds r0, #31
.L_ja_020086ea:
	bl Func_02001138
	movs r0, #10
	movs r1, #0
	bl Func_02001148
	b .L_ja_02008744
.L_ja_020086f8:
	ldrb r0, [r2, #2]
	adds r2, r0, #0
	cmp r2, #6
	bne .L_ja_02008706
	bl Func_020007b4
	b .L_ja_02008744
.L_ja_02008706:
	cmp r2, #1
	bne .L_ja_02008710
	bl Func_02000760
	b .L_ja_02008744
.L_ja_02008710:
	adds r3, r0, #0
	adds r3, #254
	movs r2, #192
	lsls r3, r3, #24
	lsls r2, r2, #18
	cmp r3, r2
	bhi .L_ja_02008740
	bl Func_02000838
	b .L_ja_02008744
.L_ja_02008724:
	.4byte 0x00001ec1
.L_ja_02008728:
	.4byte gMainRoutineWork
.L_ja_0200872c:
	.4byte 0x00001ec4
.L_ja_02008730:
	.4byte 0x00001ebb
.L_ja_02008734:
	.4byte 0x00001ec2
.L_ja_02008738:
	.4byte 0x0040250d
.L_ja_0200873c:
	.4byte 0xfffffe49
.L_ja_02008740:
	bl Func_02000878
.L_ja_02008744:
	movs r0, #18
	movs r1, #0
	movs r2, #0
	bl Func_02001108
	movs r0, #20
	negs r0, r0
	bl Party_AdjustCoins
.L_ja_02008756:
	add sp, #8
	pop {r5, r6, r7, pc}
	movs r0, r0
	.else
	push {r5, r6, r7, lr}
	ldr r0, .L_02008728
	sub sp, #8
	bl Func_02001138
	movs r0, #10
	movs r1, #0
	bl Func_02001148
	movs r0, #30
	bl FieldEvent_Wait
.L_020083a8:
	ldr r3, .L_0200872c
	movs r1, #133
	lsls r1, r1, #2
	adds r7, r3, r1
	ldr r0, [r7]
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	add r0, sp, #4
	mov r1, sp
	bl Func_020011a0
	movs r2, #1
	negs r2, r2
	cmp r0, r2
	bne .L_020083ea
	ldr r5, .L_02008730
	adds r0, r5, #0
	bl Func_02001138
	movs r1, #0
	movs r0, #10
	bl FieldMessage_ShowAtActor
	bl Func_02001188
	movs r1, #0
	bl Event_AskYesNoWithActorAnimation
	cmp r0, #1
	bne .L_020083a8
	adds r0, r5, #1
	b .L_02008496
.L_020083ea:
	ldr r0, [sp, #4]
	bl Unit_Get
	ldr r3, [sp, #0]
	adds r5, r0, #0
	lsls r3, r3, #1
	adds r3, #216
	ldrh r0, [r5, r3]
	bl Func_020010b8
	movs r0, #18
	bl Actor_GetById
	ldr r3, [sp, #0]
	lsls r3, r3, #1
	adds r3, #216
	ldrh r1, [r5, r3]
	bl Func_020010a0
	movs r1, #168
	movs r2, #208
	lsls r2, r2, #15
	lsls r1, r1, #18
	movs r0, #18
	bl Func_02001108
	movs r0, #60
	bl FieldEvent_Wait
	ldr r3, [sp, #0]
	movs r1, #2
	lsls r3, r3, #1
	adds r3, #216
	ldrh r0, [r5, r3]
	bl Func_020010b0
	movs r1, #1
	ldr r0, .L_02008734
	bl FieldMessage_Show
	ldr r6, .L_02008738
	adds r0, r6, #0
	bl Func_02001138
	movs r1, #0
	movs r0, #10
	bl FieldMessage_ShowAtActor
	bl Func_02001188
	movs r1, #0
	bl Event_AskYesNoWithActorAnimation
	cmp r0, #1
	bne .L_020084a4
	movs r2, #0
	movs r0, #18
	movs r1, #0
	bl Func_02001108
	ldr r3, [sp, #0]
	movs r1, #2
	lsls r3, r3, #1
	adds r3, #216
	ldrh r0, [r5, r3]
	bl Func_020010b0
	movs r1, #1
	adds r0, r6, #1
	bl FieldMessage_Show
	adds r0, r6, #2
	bl Func_02001138
	movs r1, #0
	movs r0, #10
	bl FieldMessage_ShowAtActor
	bl Func_02001188
	movs r1, #0
	bl Event_AskYesNoWithActorAnimation
	cmp r0, #1
	bne .L_020083a8
	adds r0, r6, #3
.L_02008496:
	bl Func_02001138
	movs r0, #10
	movs r1, #0
	bl Func_02001148
	b .L_0200875a
.L_020084a4:
	adds r0, r6, #4
	bl Func_02001138
	movs r0, #10
	movs r1, #0
	bl Func_02001148
	movs r0, #128
	movs r1, #0
	lsls r0, r0, #9
	bl Func_02001170
	movs r0, #30
	bl FieldEvent_Wait
	movs r1, #1
	ldr r0, .L_0200873c
	bl Func_02001168
	movs r0, #30
	bl Func_02001178
	movs r0, #120
	bl FieldEvent_Wait
	movs r0, #11
	bl Func_020011a8
	movs r1, #5
	movs r0, #10
	bl Actor_SetModeById
	movs r0, #10
	bl Actor_WaitForAnimationChange
	movs r0, #10
	bl Actor_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r5, #254
	adds r3, r5, #0
	ands r3, r2
	movs r1, #166
	strb r3, [r0]
	movs r2, #96
	movs r0, #10
	lsls r1, r1, #2
	bl Actor_ResetAndSetPositionInMode2
	movs r1, #5
	movs r0, #10
	bl Actor_SetModeById
	movs r0, #30
	bl FieldEvent_Wait
	movs r0, #10
	bl Actor_GetById
	adds r0, #90
	ldrb r2, [r0]
	adds r3, r5, #0
	ands r3, r2
	movs r1, #170
	strb r3, [r0]
	movs r2, #96
	movs r0, #10
	lsls r1, r1, #2
	bl Actor_ResetAndSetPositionInMode2
	movs r0, #10
	movs r1, #5
	bl Actor_SetModeById
	movs r1, #4
	movs r2, #90
	movs r0, #10
	bl Actor_Jump
	movs r0, #10
	bl Actor_GetById
	adds r0, #90
	ldrb r2, [r0]
	adds r3, r5, #0
	ands r3, r2
	movs r1, #166
	strb r3, [r0]
	movs r2, #96
	movs r0, #10
	lsls r1, r1, #2
	bl Actor_ResetAndSetPositionInMode2
	movs r1, #5
	movs r0, #10
	bl Actor_SetModeById
	movs r0, #30
	bl FieldEvent_Wait
	movs r0, #10
	bl Actor_GetById
	adds r0, #90
	ldrb r2, [r0]
	adds r3, r5, #0
	ands r3, r2
	movs r1, #170
	strb r3, [r0]
	movs r2, #96
	movs r0, #10
	lsls r1, r1, #2
	bl Actor_ResetAndSetPositionInMode2
	movs r0, #10
	movs r1, #5
	bl Actor_SetModeById
	movs r1, #4
	movs r2, #90
	movs r0, #10
	bl Actor_Jump
	movs r0, #10
	bl Actor_GetById
	adds r0, #90
	ldrb r3, [r0]
	movs r1, #168
	ands r5, r3
	strb r5, [r0]
	movs r2, #96
	movs r0, #10
	lsls r1, r1, #2
	bl Actor_ResetAndSetPositionInMode2
	movs r0, #10
	movs r1, #5
	bl Actor_SetModeById
	movs r0, #10
	movs r1, #4
	movs r2, #30
	bl Actor_Jump
	movs r2, #30
	movs r0, #10
	movs r1, #4
	bl Actor_Jump
	movs r0, #10
	movs r1, #5
	bl Actor_SetModeById
	movs r1, #0
	movs r0, #10
	bl Actor_SetModeById
	movs r0, #11
	bl Func_020011a8
	movs r0, #128
	movs r1, #1
	lsls r0, r0, #9
	bl Func_02001168
	movs r0, #30
	bl Func_02001178
	movs r1, #168
	movs r2, #208
	lsls r2, r2, #15
	lsls r1, r1, #18
	movs r0, #20
	bl Func_02001108
	movs r0, #144
	bl Func_020011a8
	movs r1, #2
	movs r0, #20
	bl Actor_SetModeById
	movs r0, #20
	bl Actor_WaitForAnimationChange
	movs r1, #0
	movs r2, #0
	movs r0, #20
	bl Func_02001108
	movs r0, #4
	bl FieldEvent_Wait
	movs r1, #168
	movs r2, #208
	lsls r2, r2, #15
	movs r0, #19
	lsls r1, r1, #18
	bl Func_02001108
	movs r1, #4
	movs r0, #19
	bl Actor_SetModeById
	movs r0, #19
	bl Actor_WaitForAnimationChange
	movs r0, #15
	bl FieldEvent_Wait
	movs r1, #0
	movs r2, #0
	movs r0, #19
	bl Func_02001108
	movs r0, #30
	bl FieldEvent_Wait
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #0
	ldr r0, [r7]
	bl Func_02001160
	movs r0, #60
	bl FieldEvent_Wait
	movs r0, #10
	bl Func_020011a8
	movs r1, #128
	movs r2, #60
	lsls r1, r1, #1
	movs r0, #10
	bl Func_02001160
	movs r1, #0
	movs r0, #10
	bl Func_02001148
	ldr r0, [sp, #4]
	bl Unit_Get
	ldr r3, [sp, #0]
	adds r5, r0, #0
	lsls r3, r3, #1
	adds r3, #216
	ldrh r0, [r5, r3]
	bl Func_020010b8
	ldr r3, [sp, #0]
	movs r2, #128
	lsls r3, r3, #1
	adds r3, #216
	ldrh r3, [r5, r3]
	ldr r1, .L_02008740
	lsls r2, r2, #1
	adds r2, #255
	ands r2, r3
	adds r3, r2, r1
	movs r1, #128
	lsls r3, r3, #16
	lsls r1, r1, #10
	cmp r3, r1
	bhi .L_020086c0
	bl Func_02000920
	b .L_02008748
.L_020086c0:
	movs r3, #163
	lsls r3, r3, #1
	cmp r2, r3
	bne .L_020086fc
	adds r0, r6, #0
	adds r0, #29
	bl Func_02001138
	movs r0, #10
	movs r1, #0
	bl Func_02001148
	movs r0, #148
	lsls r0, r0, #4
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020086ea
	adds r0, r6, #0
	adds r0, #30
	b .L_020086ee
.L_020086ea:
	adds r0, r6, #0
	adds r0, #31
.L_020086ee:
	bl Func_02001138
	movs r0, #10
	movs r1, #0
	bl Func_02001148
	b .L_02008748
.L_020086fc:
	ldrb r0, [r0, #2]
	adds r2, r0, #0
	cmp r2, #6
	bne .L_0200870a
	bl Func_020007b4
	b .L_02008748
.L_0200870a:
	cmp r2, #1
	bne .L_02008714
	bl Func_02000760
	b .L_02008748
.L_02008714:
	adds r3, r0, #0
	adds r3, #254
	movs r1, #192
	lsls r3, r3, #24
	lsls r1, r1, #18
	cmp r3, r1
	bhi .L_02008744
	bl Func_02000838
	b .L_02008748
.L_02008728:
	.4byte MsgVeryWellShow
.L_0200872c:
	.4byte gMainRoutineWork
.L_02008730:
	.4byte MsgShallIGive
.L_02008734:
	.4byte MsgPlacedOnThe
.L_02008738:
	.4byte MsgAreYouPrepared
.L_0200873c:
	.4byte 0x0040250d
.L_02008740:
	.4byte 0xfffffe49
.L_02008744:
	bl Func_02000878
.L_02008748:
	movs r0, #18
	movs r1, #0
	movs r2, #0
	bl Func_02001108
	movs r0, #20
	negs r0, r0
	bl Party_AdjustCoins
.L_0200875a:
	add sp, #8
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.endif
	.endif
	.section .text.x02008ba0,"ax",%progbits
	.balign 4
	.global Func_02000ba0
	.thumb_func
Func_02000ba0:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r3, #0
	mov r10, r0
	movs r5, #7
	sub sp, #4
	adds r7, r1, #0
	mov r8, r3
	cmp r5, r10
	bge .L_02008bf6
	str r2, [sp, #0]
.L_02008bba:
	cmp r7, #0
	bne .L_02008bc2
	cmp r5, #7
	beq .L_02008bf0
.L_02008bc2:
	cmp r7, #1
	bne .L_02008bce
	cmp r5, #9
	beq .L_02008bf0
	cmp r5, #10
	beq .L_02008bf0
.L_02008bce:
	lsls r3, r7, #2
	adds r3, r3, r7
	lsls r3, r3, #2
	adds r3, r3, r5
	adds r6, r3, #0
	adds r6, #48
	adds r0, r6, #0
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008bf0
	ldr r3, [sp, #0]
	stmia r3!, {r6}
	adds r2, r3, #0
	str r2, [sp, #0]
	movs r2, #1
	add r8, r2
.L_02008bf0:
	adds r5, #1
	cmp r5, r10
	blt .L_02008bba
.L_02008bf6:
	mov r0, r8
	add sp, #4
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
