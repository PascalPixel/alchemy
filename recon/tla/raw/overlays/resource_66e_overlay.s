.syntax unified
	.thumb
	.section .text.x02008390,"ax",%progbits
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
	bl Battle_WaitMode0
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
	bl UiText_OpenMessageAtObject
	bl Func_02001188
	movs r1, #0
	bl Event_AskYesNoWithActorAnimation
	cmp r0, #1
	bne .L_ja_020083a8
	adds r0, r5, #1
	b .L_ja_02008496
.L_ja_020083ea:
	ldr r0, [sp, #4]
	bl UnitGet
	ldr r3, [sp, #0]
	adds r5, r0, #0
	lsls r3, r3, #1
	adds r3, #216
	ldrh r0, [r5, r3]
	bl Func_020010b8
	movs r0, #18
	bl Object_GetById
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
	bl Battle_WaitMode0
	ldr r3, [sp, #0]
	movs r1, #2
	lsls r3, r3, #1
	adds r3, #216
	ldrh r0, [r5, r3]
	bl Func_020010b0
	movs r1, #1
	ldr r0, .L_ja_02008730
	bl UiText_ShowPositionedMessageAndWait
	ldr r6, .L_ja_02008734
	adds r0, r6, #0
	bl Func_02001138
	movs r1, #0
	movs r0, #10
	bl UiText_OpenMessageAtObject
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
	bl UiText_ShowPositionedMessageAndWait
	adds r0, r6, #2
	bl Func_02001138
	movs r1, #0
	movs r0, #10
	bl UiText_OpenMessageAtObject
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
	bl Battle_WaitMode0
	movs r1, #1
	ldr r0, .L_ja_02008738
	bl Func_02001168
	movs r0, #30
	bl Func_02001178
	movs r0, #120
	bl Battle_WaitMode0
	movs r0, #11
	bl Func_020011a8
	movs r1, #5
	movs r0, #10
	bl Object_SetModeById
	movs r0, #10
	bl ObjectMotion_WaitForAnimationChange
	movs r0, #10
	bl Object_GetById
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
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r1, #5
	movs r0, #10
	bl Object_SetModeById
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #10
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0, #0]
	adds r3, r5, #0
	ands r3, r2
	movs r1, #170
	strb r3, [r0, #0]
	movs r2, #96
	movs r0, #10
	lsls r1, r1, #2
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r0, #10
	movs r1, #5
	bl Object_SetModeById
	movs r1, #4
	movs r2, #90
	movs r0, #10
	bl ObjectMotion_Launch
	movs r0, #10
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0, #0]
	adds r3, r5, #0
	ands r3, r2
	movs r1, #166
	strb r3, [r0, #0]
	movs r2, #96
	movs r0, #10
	lsls r1, r1, #2
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r1, #5
	movs r0, #10
	bl Object_SetModeById
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #10
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0, #0]
	adds r3, r5, #0
	ands r3, r2
	movs r1, #170
	strb r3, [r0, #0]
	movs r2, #96
	movs r0, #10
	lsls r1, r1, #2
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r0, #10
	movs r1, #5
	bl Object_SetModeById
	movs r1, #4
	movs r2, #90
	movs r0, #10
	bl ObjectMotion_Launch
	movs r0, #10
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0, #0]
	movs r1, #168
	ands r5, r3
	strb r5, [r0, #0]
	movs r2, #96
	movs r0, #10
	lsls r1, r1, #2
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r0, #10
	movs r1, #5
	bl Object_SetModeById
	movs r0, #10
	movs r1, #4
	movs r2, #30
	bl ObjectMotion_Launch
	movs r2, #30
	movs r0, #10
	movs r1, #4
	bl ObjectMotion_Launch
	movs r0, #10
	movs r1, #5
	bl Object_SetModeById
	movs r1, #0
	movs r0, #10
	bl Object_SetModeById
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
	bl Object_SetModeById
	movs r0, #20
	bl ObjectMotion_WaitForAnimationChange
	movs r1, #0
	movs r2, #0
	movs r0, #20
	bl Func_02001108
	movs r0, #4
	bl Battle_WaitMode0
	movs r1, #168
	movs r2, #208
	lsls r2, r2, #15
	movs r0, #19
	lsls r1, r1, #18
	bl Func_02001108
	movs r1, #4
	movs r0, #19
	bl Object_SetModeById
	movs r0, #19
	bl ObjectMotion_WaitForAnimationChange
	movs r0, #15
	bl Battle_WaitMode0
	movs r1, #0
	movs r2, #0
	movs r0, #19
	bl Func_02001108
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #0
	ldr r0, [r7, #0]
	bl Func_02001160
	movs r0, #60
	bl Battle_WaitMode0
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
	bl UnitGet
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
	.4byte gPartyState
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
	bl Battle_WaitMode0
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
	bl UiText_OpenMessageAtObject
	bl Func_02001188
	movs r1, #0
	bl Event_AskYesNoWithActorAnimation
	cmp r0, #1
	bne .L_020083a8
	adds r0, r5, #1
	b .L_02008496
.L_020083ea:
	ldr r0, [sp, #4]
	bl UnitGet
	ldr r3, [sp, #0]
	adds r5, r0, #0
	lsls r3, r3, #1
	adds r3, #216
	ldrh r0, [r5, r3]
	bl Func_020010b8
	movs r0, #18
	bl Object_GetById
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
	bl Battle_WaitMode0
	ldr r3, [sp, #0]
	movs r1, #2
	lsls r3, r3, #1
	adds r3, #216
	ldrh r0, [r5, r3]
	bl Func_020010b0
	movs r1, #1
	ldr r0, .L_02008734
	bl UiText_ShowPositionedMessageAndWait
	ldr r6, .L_02008738
	adds r0, r6, #0
	bl Func_02001138
	movs r1, #0
	movs r0, #10
	bl UiText_OpenMessageAtObject
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
	bl UiText_ShowPositionedMessageAndWait
	adds r0, r6, #2
	bl Func_02001138
	movs r1, #0
	movs r0, #10
	bl UiText_OpenMessageAtObject
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
	bl Battle_WaitMode0
	movs r1, #1
	ldr r0, .L_0200873c
	bl Func_02001168
	movs r0, #30
	bl Func_02001178
	movs r0, #120
	bl Battle_WaitMode0
	movs r0, #11
	bl Func_020011a8
	movs r1, #5
	movs r0, #10
	bl Object_SetModeById
	movs r0, #10
	bl ObjectMotion_WaitForAnimationChange
	movs r0, #10
	bl Object_GetById
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
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r1, #5
	movs r0, #10
	bl Object_SetModeById
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #10
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	adds r3, r5, #0
	ands r3, r2
	movs r1, #170
	strb r3, [r0]
	movs r2, #96
	movs r0, #10
	lsls r1, r1, #2
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r0, #10
	movs r1, #5
	bl Object_SetModeById
	movs r1, #4
	movs r2, #90
	movs r0, #10
	bl ObjectMotion_Launch
	movs r0, #10
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	adds r3, r5, #0
	ands r3, r2
	movs r1, #166
	strb r3, [r0]
	movs r2, #96
	movs r0, #10
	lsls r1, r1, #2
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r1, #5
	movs r0, #10
	bl Object_SetModeById
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #10
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	adds r3, r5, #0
	ands r3, r2
	movs r1, #170
	strb r3, [r0]
	movs r2, #96
	movs r0, #10
	lsls r1, r1, #2
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r0, #10
	movs r1, #5
	bl Object_SetModeById
	movs r1, #4
	movs r2, #90
	movs r0, #10
	bl ObjectMotion_Launch
	movs r0, #10
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	movs r1, #168
	ands r5, r3
	strb r5, [r0]
	movs r2, #96
	movs r0, #10
	lsls r1, r1, #2
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r0, #10
	movs r1, #5
	bl Object_SetModeById
	movs r0, #10
	movs r1, #4
	movs r2, #30
	bl ObjectMotion_Launch
	movs r2, #30
	movs r0, #10
	movs r1, #4
	bl ObjectMotion_Launch
	movs r0, #10
	movs r1, #5
	bl Object_SetModeById
	movs r1, #0
	movs r0, #10
	bl Object_SetModeById
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
	bl Object_SetModeById
	movs r0, #20
	bl ObjectMotion_WaitForAnimationChange
	movs r1, #0
	movs r2, #0
	movs r0, #20
	bl Func_02001108
	movs r0, #4
	bl Battle_WaitMode0
	movs r1, #168
	movs r2, #208
	lsls r2, r2, #15
	movs r0, #19
	lsls r1, r1, #18
	bl Func_02001108
	movs r1, #4
	movs r0, #19
	bl Object_SetModeById
	movs r0, #19
	bl ObjectMotion_WaitForAnimationChange
	movs r0, #15
	bl Battle_WaitMode0
	movs r1, #0
	movs r2, #0
	movs r0, #19
	bl Func_02001108
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #0
	ldr r0, [r7]
	bl Func_02001160
	movs r0, #60
	bl Battle_WaitMode0
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
	bl UnitGet
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
	.4byte gPartyState
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
	.section .text.x02008c04,"ax",%progbits
	.global Func_02000c04
	.thumb_func
Func_02000c04:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r1, #160
	movs r0, #252
	lsls r1, r1, #1
	sub sp, #16
	bl Runtime_AllocateBlock
	movs r3, #156
	lsls r3, r3, #6
	movs r6, #0
	adds r3, #15
	adds r7, r0, #0
	mov r8, r3
	mov r10, r6
	movs r5, #0
.L_02008c28:
	adds r0, r5, #0
	bl Func_02000b60
	lsls r3, r5, #2
	mov r2, sp
	str r0, [r2, r3]
	cmp r8, r0
	ble .L_02008c3c
	mov r8, r0
	mov r10, r5
.L_02008c3c:
	movs r3, #1
	negs r3, r3
	cmp r0, r3
	beq .L_02008c50
	lsls r2, r6, #2
	adds r2, r7, r2
	adds r1, r5, #0
	bl Func_02000ba0
	adds r6, r6, r0
.L_02008c50:
	adds r5, #1
	cmp r5, #3
	bls .L_02008c28
	cmp r6, #0
	bne .L_02008c8c
	movs r5, #0
.L_02008c5c:
	lsls r2, r6, #2
	adds r1, r5, #0
	adds r2, r7, r2
	movs r0, #18
	bl Func_02000ba0
	adds r5, #1
	adds r6, r6, r0
	cmp r5, #3
	bls .L_02008c5c
	cmp r6, #0
	bne .L_02008c7e
	movs r0, #252
	bl Runtime_ReleaseHeapBlock
	ldr r0, .L_02008e7c
	b .L_02008e72
.L_02008c7e:
	movs r0, #18
	mov r1, r10
	adds r2, r7, #0
	bl Func_02000ba0
	movs r0, #0
	b .L_02008c96
.L_02008c8c:
	bl Random16Far
	adds r1, r6, #0
	bl __umodsi3
.L_02008c96:
	lsls r3, r0, #2
	ldr r3, [r3, r7]
	adds r0, r3, #0
	subs r0, #56
	cmp r0, #69
	bls .L_02008ca4
	b .L_02008e68
.L_02008ca4:
	ldr r2, .L_02008e80
	lsls r3, r0, #2
	ldr r3, [r3, r2]
	mov pc, r3
.L_02008cac:
	.4byte .L_02008dc4
	.4byte .L_02008dc8
	.4byte .L_02008dcc
	.4byte .L_02008dd0
	.4byte .L_02008dd4
	.4byte .L_02008dd8
	.4byte .L_02008ddc
	.4byte .L_02008de0
	.4byte .L_02008de4
	.4byte .L_02008de8
	.4byte .L_02008e68
	.4byte .L_02008e68
	.4byte .L_02008e68
	.4byte .L_02008e68
	.4byte .L_02008e68
	.4byte .L_02008e68
	.4byte .L_02008e68
	.4byte .L_02008e68
	.4byte .L_02008e68
	.4byte .L_02008dec
	.4byte .L_02008df0
	.4byte .L_02008e68
	.4byte .L_02008e68
	.4byte .L_02008df4
	.4byte .L_02008df8
	.4byte .L_02008dfc
	.4byte .L_02008e00
	.4byte .L_02008e04
	.4byte .L_02008e08
	.4byte .L_02008e0c
	.4byte .L_02008e68
	.4byte .L_02008e68
	.4byte .L_02008e68
	.4byte .L_02008e68
	.4byte .L_02008e68
	.4byte .L_02008e68
	.4byte .L_02008e68
	.4byte .L_02008e68
	.4byte .L_02008e68
	.4byte .L_02008e10
	.4byte .L_02008e14
	.4byte .L_02008e18
	.4byte .L_02008e1c
	.4byte .L_02008e20
	.4byte .L_02008e24
	.4byte .L_02008e28
	.4byte .L_02008e2c
	.4byte .L_02008e30
	.4byte .L_02008e34
	.4byte .L_02008e38
	.4byte .L_02008e68
	.4byte .L_02008e68
	.4byte .L_02008e68
	.4byte .L_02008e68
	.4byte .L_02008e68
	.4byte .L_02008e68
	.4byte .L_02008e68
	.4byte .L_02008e68
	.4byte .L_02008e68
	.4byte .L_02008e3c
	.4byte .L_02008e40
	.4byte .L_02008e44
	.4byte .L_02008e48
	.4byte .L_02008e4c
	.4byte .L_02008e50
	.4byte .L_02008e54
	.4byte .L_02008e58
	.4byte .L_02008e5c
	.4byte .L_02008e60
	.4byte .L_02008e64
.L_02008dc4:
	ldr r5, .L_02008e84
	b .L_02008e6a
.L_02008dc8:
	ldr r5, .L_02008e88
	b .L_02008e6a
.L_02008dcc:
	ldr r5, .L_02008e8c
	b .L_02008e6a
.L_02008dd0:
	ldr r5, .L_02008e90
	b .L_02008e6a
.L_02008dd4:
	ldr r5, .L_02008e94
	b .L_02008e6a
.L_02008dd8:
	ldr r5, .L_02008e98
	b .L_02008e6a
.L_02008ddc:
	ldr r5, .L_02008e9c
	b .L_02008e6a
.L_02008de0:
	ldr r5, .L_02008ea0
	b .L_02008e6a
.L_02008de4:
	ldr r5, .L_02008ea4
	b .L_02008e6a
.L_02008de8:
	ldr r5, .L_02008ea8
	b .L_02008e6a
.L_02008dec:
	ldr r5, .L_02008eac
	b .L_02008e6a
.L_02008df0:
	ldr r5, .L_02008eb0
	b .L_02008e6a
.L_02008df4:
	ldr r5, .L_02008eb4
	b .L_02008e6a
.L_02008df8:
	ldr r5, .L_02008eb8
	b .L_02008e6a
.L_02008dfc:
	ldr r5, .L_02008ebc
	b .L_02008e6a
.L_02008e00:
	ldr r5, .L_02008ec0
	b .L_02008e6a
.L_02008e04:
	ldr r5, .L_02008ec4
	b .L_02008e6a
.L_02008e08:
	ldr r5, .L_02008ec8
	b .L_02008e6a
.L_02008e0c:
	ldr r5, .L_02008ecc
	b .L_02008e6a
.L_02008e10:
	ldr r5, .L_02008ed0
	b .L_02008e6a
.L_02008e14:
	ldr r5, .L_02008ed4
	b .L_02008e6a
.L_02008e18:
	ldr r5, .L_02008ed8
	b .L_02008e6a
.L_02008e1c:
	ldr r5, .L_02008edc
	b .L_02008e6a
.L_02008e20:
	ldr r5, .L_02008ee0
	b .L_02008e6a
.L_02008e24:
	ldr r5, .L_02008ee4
	b .L_02008e6a
.L_02008e28:
	ldr r5, .L_02008ee8
	b .L_02008e6a
.L_02008e2c:
	ldr r5, .L_02008eec
	b .L_02008e6a
.L_02008e30:
	ldr r5, .L_02008ef0
	b .L_02008e6a
.L_02008e34:
	ldr r5, .L_02008ef4
	b .L_02008e6a
.L_02008e38:
	ldr r5, .L_02008ef8
	b .L_02008e6a
.L_02008e3c:
	ldr r5, .L_02008efc
	b .L_02008e6a
.L_02008e40:
	ldr r5, .L_02008f00
	b .L_02008e6a
.L_02008e44:
	ldr r5, .L_02008f04
	b .L_02008e6a
.L_02008e48:
	ldr r5, .L_02008f08
	b .L_02008e6a
.L_02008e4c:
	ldr r5, .L_02008f0c
	b .L_02008e6a
.L_02008e50:
	ldr r5, .L_02008f10
	b .L_02008e6a
.L_02008e54:
	ldr r5, .L_02008f14
	b .L_02008e6a
.L_02008e58:
	ldr r5, .L_02008f18
	b .L_02008e6a
.L_02008e5c:
	ldr r5, .L_02008f1c
	b .L_02008e6a
.L_02008e60:
	ldr r5, .L_02008f20
	b .L_02008e6a
.L_02008e64:
	ldr r5, .L_02008f24
	b .L_02008e6a
.L_02008e68:
	ldr r5, .L_02008e7c
.L_02008e6a:
	movs r0, #252
	bl Runtime_ReleaseHeapBlock
	adds r0, r5, #0
.L_02008e72:
	add sp, #16
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_02008e7c:
	.4byte MsgInTheItem
.L_02008e80:
	.4byte .L_02008cac
.L_02008e84:
	.4byte MsgToTheEast
.L_02008e88:
	.4byte MsgToTheNorth
.L_02008e8c:
	.4byte MsgToTheNorthBeneath
.L_02008e90:
	.4byte MsgHeadEastBy
.L_02008e94:
	.4byte MsgHeadEastByNortheast
.L_02008e98:
	.4byte MsgHeadNorthBy
.L_02008e9c:
	.4byte MsgToTheNorthwest
.L_02008ea0:
	.4byte MsgHeadWestBy
.L_02008ea4:
	.4byte MsgToTheNorthIn
.L_02008ea8:
	.4byte MsgHeadEastBySoutheast
.L_02008eac:
	.4byte MsgHeadEastByNortheastIn
.L_02008eb0:
	.4byte MsgToTheSoutheast
.L_02008eb4:
	.4byte MsgToTheSouth
.L_02008eb8:
	.4byte MsgToTheEastBeside
.L_02008ebc:
	.4byte MsgHeadEastByNortheastInThe
.L_02008ec0:
	.4byte MsgToTheNorthSomewhere
.L_02008ec4:
	.4byte MsgHeadNorthByNorthwest
.L_02008ec8:
	.4byte MsgToTheNorthInRuins
.L_02008ecc:
	.4byte MsgHeadEastByNortheastInA
.L_02008ed0:
	.4byte MsgToTheEastIn
.L_02008ed4:
	.4byte MsgToTheSoutheastIn
.L_02008ed8:
	.4byte MsgHeadSouthBy
.L_02008edc:
	.4byte MsgHeadEastBySoutheastIn
.L_02008ee0:
	.4byte MsgHeadEastByNortheastOn
.L_02008ee4:
	.4byte MsgHeadSouthBySoutheast
.L_02008ee8:
	.4byte MsgToTheNorthwestIn
.L_02008eec:
	.4byte MsgToTheNorthwestAlong
.L_02008ef0:
	.4byte MsgToTheNorthwestBeside
.L_02008ef4:
	.4byte MsgHeadNorthByNorthwestAt
.L_02008ef8:
	.4byte MsgToTheNorthInThe
.L_02008efc:
	.4byte MsgHeadEastByNortheastInACave
.L_02008f00:
	.4byte MsgHeadEastBySoutheastInThe
.L_02008f04:
	.4byte MsgHeadEastBySoutheastInA
.L_02008f08:
	.4byte MsgToTheNorthInTheMountain
.L_02008f0c:
	.4byte MsgToTheEastSomewhere
.L_02008f10:
	.4byte MsgToTheEastInThe
.L_02008f14:
	.4byte MsgHeadNorthByNorthwestIn
.L_02008f18:
	.4byte MsgInTheNorthwest
.L_02008f1c:
	.4byte MsgHeadNorthByNorthwestNear
.L_02008f20:
	.4byte MsgToTheNorthInTheRuins
.L_02008f24:
	.4byte MsgHeadNorthByNortheast
