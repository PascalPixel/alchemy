.syntax unified
	.thumb
	.section .text.x0200812c,"ax",%progbits
	.balign 4
	.global Func_0200012c
	.thumb_func
Func_0200012c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	ldr r6, .L_020081dc
	subs r2, #172
	str r2, [r3]
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
	movs r3, #252
	lsls r3, r3, #6
	movs r2, #128
	adds r3, #66
	lsls r2, r2, #19
	adds r2, #80
	mov r10, r3
	mov r3, r10
	mov r8, r2
	strh r3, [r2]
	movs r3, #128
	movs r7, #128
	lsls r3, r3, #4
	lsls r7, r7, #19
	adds r3, #12
	adds r7, #82
	strh r3, [r7]
	movs r0, #24
	movs r1, #2
	bl Object_SetModeById
	movs r1, #2
	movs r0, #25
	bl Object_SetModeById
	movs r0, #24
	bl Object_GetById
	movs r5, #2
	adds r0, #35
	strb r5, [r0]
	movs r0, #25
	bl Object_GetById
	adds r0, #35
	strb r5, [r0]
	bl Event_StartScreenTransition
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r6, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #1
	bne .L_020081d2
	bl Func_020010f4
	movs r0, #128
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020081d2
	mov r3, r10
	mov r2, r8
	strh r3, [r2]
	movs r3, #128
	lsls r3, r3, #5
	strh r3, [r7]
.L_020081d2:
	movs r0, #0
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_020081dc:
	.4byte gPartyState
	.section .text.x020081e4,"ax",%progbits
	.global Func_020001e4
	.thumb_func
Func_020001e4:
	.ifdef TLA_EDITION_JA
	push {r5, r6, r7, lr}
	mov r7, fp
	mov r6, sl
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r1, #0
	sub sp, #4
	mov r8, r1
	bl Func_02001380
	movs r0, #0
	bl Func_020013f8
.L_ja_02008202:
	ldr r3, .L_ja_0200849c
	movs r0, #229
	ldr r3, [r3, #16]
	mov fp, r3
	bl PartyInventory_CountItem
	ldr r2, .L_ja_020084a0
	mov r9, r0
	mov sl, r2
	mov r0, sl
	bl Func_020013c8
	movs r0, #1
	movs r1, #0
	negs r0, r0
	bl UiText_OpenMessageAtObject
	movs r3, #2
	str r3, [sp, #0]
	movs r1, #0
	movs r2, #17
	movs r3, #4
	movs r0, #0
	bl UiWindow_Create
	ldr r5, .L_ja_020084a4
	adds r7, r0, #0
	adds r1, r7, #0
	adds r0, r5, #0
	movs r2, #0
	movs r3, #0
	bl UiText_DrawCharacterAtOffset
	movs r3, #0
	str r3, [sp, #0]
	mov r0, fp
	movs r1, #6
	adds r2, r7, #0
	movs r3, #56
	adds r6, r5, #2
	bl UiText_DrawNumberInWindow
	adds r0, r6, #0
	adds r1, r7, #0
	movs r2, #104
	movs r3, #0
	bl UiText_DrawCharacterAtOffset
	adds r0, r5, #1
	adds r1, r7, #0
	movs r2, #0
	movs r3, #8
	bl UiText_DrawCharacterAtOffset
	movs r3, #8
	str r3, [sp, #0]
	mov r0, r9
	movs r1, #6
	adds r2, r7, #0
	movs r3, #56
	bl UiText_DrawNumberInWindow
	movs r3, #8
	movs r2, #104
	adds r1, r7, #0
	adds r0, r6, #0
	bl UiText_DrawCharacterAtOffset
	mov r0, r8
	bl Menu_SelectCoinsOrMedals
	movs r1, #2
	mov r8, r0
	adds r0, r7, #0
	bl UiWork_Finalize
	bl UiWork_FinalizePendingCore
	movs r3, #1
	negs r3, r3
	cmp r8, r3
	bne .L_ja_020082a8
	b .L_ja_02008486
.L_ja_020082a8:
	mov r1, r8
	cmp r1, #0
	bne .L_ja_020082c4
	mov r2, fp
	cmp r2, #0
	bne .L_ja_020082ba
	mov r0, sl
	adds r0, #1
	b .L_ja_020082d4
.L_ja_020082ba:
	movs r0, #1
	negs r0, r0
	bl Party_AdjustCoins
	b .L_ja_02008322
.L_ja_020082c4:
	mov r3, r8
	cmp r3, #1
	bne .L_ja_02008322
	mov r1, r9
	cmp r1, #0
	bne .L_ja_020082f4
	mov r0, sl
	adds r0, #2
.L_ja_020082d4:
	bl Func_020013c8
	movs r0, #1
	negs r0, r0
	movs r1, #0
	bl Func_020013d8
	movs r0, #1
	bl WaitTicks
	b .L_ja_02008202
.L_ja_020082ea:
	movs r0, #112
	bl Func_02001408
	movs r5, #0
	b .L_ja_02008382
.L_ja_020082f4:
	movs r0, #229
	bl PartyInventory_Remove
	bl PartyInventory_CountFreeSlots
	cmp r0, #0
	bne .L_ja_02008322
	mov r0, sl
	adds r0, #4
	bl Func_020013c8
	movs r0, #1
	movs r1, #0
	negs r0, r0
	bl UiText_OpenMessageAtObject
	movs r0, #0
	movs r1, #0
	bl Event_AskYesNoWithActorAnimation
	cmp r0, #0
	beq .L_ja_02008322
	b .L_ja_02008486
.L_ja_02008322:
	movs r3, #2
	str r3, [sp, #0]
	movs r1, #15
	movs r2, #7
	movs r3, #4
	movs r0, #22
	bl UiWindow_Create
	ldr r5, .L_ja_020084a8
	adds r7, r0, #0
	adds r1, r7, #0
	adds r0, r5, #0
	movs r2, #0
	movs r3, #0
	bl UiText_DrawCharacterAtOffset
	adds r0, r5, #1
	adds r1, r7, #0
	movs r2, #0
	movs r3, #8
	bl UiText_DrawCharacterAtOffset
	movs r0, #5
	bl WaitTicks
	movs r0, #116
	bl Func_02001408
	b .L_ja_02008362
.L_ja_0200835c:
	movs r0, #1
	bl WaitTicks
.L_ja_02008362:
	ldr r1, .L_ja_020084ac
	movs r2, #1
	ldr r3, [r1, #4]
	ands r3, r2
	cmp r3, #0
	bne .L_ja_020082ea
	ldr r3, [r1, #4]
	movs r2, #2
	ands r3, r2
	cmp r3, #0
	beq .L_ja_0200835c
	movs r0, #113
	bl Func_02001408
	movs r5, #1
	negs r5, r5
.L_ja_02008382:
	adds r0, r7, #0
	movs r1, #2
	bl UiWork_Finalize
	movs r2, #1
	negs r2, r2
	cmp r5, r2
	bne .L_ja_02008394
	b .L_ja_02008486
.L_ja_02008394:
	mov r0, r8
	bl Func_02001184
	mov r3, r8
	adds r5, r0, #0
	cmp r3, #0
	bne .L_ja_020083de
	cmp r5, #4
	beq .L_ja_020083d0
	ldr r6, .L_ja_020084b0
	lsls r5, r5, #1
	ldrh r0, [r6, r5]
	bl Party_AdjustCoins
	movs r0, #91
	bl Func_02001408
	movs r1, #5
	ldrh r0, [r6, r5]
	bl Func_02001328
	ldr r0, .L_ja_020084b4
	bl Func_020013c8
	movs r0, #1
	negs r0, r0
	movs r1, #0
	bl Func_020013d8
	b .L_ja_02008486
.L_ja_020083d0:
	movs r0, #113
	bl Func_02001408
	movs r0, #10
	bl FieldEvent_WaitTicks
	b .L_ja_02008486
.L_ja_020083de:
	movs r7, #0
	cmp r5, #0
	bne .L_ja_02008406
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #11
	bl GameFlag_Test
	cmp r0, #0
	bne .L_ja_02008406
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #11
	bl GameFlag_SetBit
	movs r0, #1
	negs r0, r0
	bl Func_0200049c
	b .L_ja_02008486
.L_ja_02008406:
	lsls r3, r5, #1
	adds r0, r3, r5
	movs r6, #0
	adds r3, r0, #3
	cmp r6, r3
	bge .L_ja_02008428
	ldr r1, .L_ja_0200849c
.L_ja_02008414:
	movs r3, #158
	lsls r3, r3, #1
	adds r2, r6, r3
	adds r3, r1, #1
	ldrsb r3, [r3, r2]
	adds r6, #1
	adds r7, r7, r3
	adds r3, r0, #3
	cmp r6, r3
	blt .L_ja_02008414
.L_ja_02008428:
	bl Random16Far
	adds r3, r7, #0
	muls r3, r0
	ldr r0, .L_ja_0200849c
	movs r2, #158
	lsrs r1, r3, #16
	lsls r2, r2, #1
	adds r3, r0, #1
	ldrsb r3, [r3, r2]
	movs r6, #0
	subs r1, r1, r3
	cmp r1, #0
	blt .L_ja_0200845a
.L_ja_02008444:
	adds r6, #1
	cmp r6, #14
	bgt .L_ja_0200845a
	movs r2, #158
	lsls r2, r2, #1
	adds r3, r6, r2
	adds r2, r0, #1
	ldrsb r3, [r2, r3]
	subs r1, r1, r3
	cmp r1, #0
	bge .L_ja_02008444
.L_ja_0200845a:
	cmp r6, #15
	bne .L_ja_02008460
	movs r6, #14
.L_ja_02008460:
	ldr r2, .L_ja_020084b8
	lsls r3, r6, #2
	ldr r0, [r2, r3]
	bl Func_0200049c
	ldr r3, .L_ja_0200849c
	movs r1, #158
	lsls r1, r1, #1
	adds r2, r6, r1
	adds r0, r3, #1
	ldrb r3, [r0, r2]
	lsls r3, r3, #24
	asrs r1, r3, #24
	cmp r1, #1
	ble .L_ja_02008486
	lsrs r3, r3, #31
	adds r3, r1, r3
	asrs r3, r3, #1
	strb r3, [r0, r2]
.L_ja_02008486:
	bl Func_02001388
	movs r0, #0
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov sl, r6
	mov fp, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_ja_0200849c:
	.4byte gPartyState
.L_ja_020084a0:
	.4byte 0x000013ae
.L_ja_020084a4:
	.4byte 0x000013b4
.L_ja_020084a8:
	.4byte 0x000013b7
.L_ja_020084ac:
	.4byte gInput
.L_ja_020084b0:
	.4byte gData_0200173c
.L_ja_020084b4:
	.4byte 0x000013b1
.L_ja_020084b8:
	.4byte gData_02001700
	.else
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #4
	bl Func_02001380
	movs r0, #0
	movs r7, #0
	bl Func_020013f8
.L_020081fc:
	ldr r3, .L_0200847c
	movs r0, #229
	ldr r3, [r3, #16]
	mov r9, r3
	bl PartyInventory_CountItem
	ldr r1, .L_02008480
	mov r10, r0
	mov r8, r1
	mov r0, r8
	bl Func_020013c8
	movs r0, #1
	movs r1, #0
	negs r0, r0
	bl UiText_OpenMessageAtObject
	movs r3, #2
	str r3, [sp, #0]
	movs r1, #0
	.ifdef TLA_EDITION_FR
	movs r2, #19
	.else
	movs r2, #17
	.endif
	movs r3, #4
	movs r0, #0
	bl UiWindow_Create
	ldr r5, .L_02008484
	adds r6, r0, #0
	adds r1, r6, #0
	adds r0, r5, #0
	movs r2, #0
	movs r3, #0
	bl UiText_DrawCharacterAtOffset
	movs r3, #0
	str r3, [sp, #0]
	mov r0, r9
	movs r1, #6
	adds r2, r6, #0
	.ifdef TLA_EDITION_FR
	movs r3, #88
	.else
	movs r3, #72
	.endif
	bl UiText_DrawNumberInWindow
	adds r0, r5, #1
	adds r1, r6, #0
	movs r2, #0
	movs r3, #8
	bl UiText_DrawCharacterAtOffset
	movs r3, #8
	str r3, [sp, #0]
	adds r2, r6, #0
	.ifdef TLA_EDITION_FR
	movs r3, #88
	.else
	movs r3, #72
	.endif
	movs r1, #6
	mov r0, r10
	bl UiText_DrawNumberInWindow
	adds r0, r7, #0
	bl Menu_SelectCoinsOrMedals
	movs r1, #2
	adds r7, r0, #0
	adds r0, r6, #0
	bl UiWork_Finalize
	bl UiWork_FinalizePendingCore
	movs r2, #1
	negs r2, r2
	cmp r7, r2
	bne .L_02008288
	b .L_02008468
.L_02008288:
	cmp r7, #0
	bne .L_02008298
	mov r3, r9
	cmp r3, #0
	bne .L_020082ee
	mov r0, r8
	adds r0, #1
	b .L_020082a6
.L_02008298:
	cmp r7, #1
	bne .L_020082ee
	mov r1, r10
	cmp r1, #0
	bne .L_020082c6
	mov r0, r8
	adds r0, #2
.L_020082a6:
	bl Func_020013c8
	movs r0, #1
	negs r0, r0
	movs r1, #0
	bl Func_020013d8
	movs r0, #1
	bl WaitTicks
	b .L_020081fc
.L_020082bc:
	movs r0, #112
	bl Func_02001408
	movs r5, #0
	b .L_0200834e
.L_020082c6:
	bl PartyInventory_CountFreeSlots
	cmp r0, #0
	bne .L_020082ee
	mov r0, r8
	adds r0, #4
	bl Func_020013c8
	movs r0, #1
	movs r1, #0
	negs r0, r0
	bl UiText_OpenMessageAtObject
	movs r0, #0
	movs r1, #0
	bl Event_AskYesNoWithActorAnimation
	cmp r0, #0
	beq .L_020082ee
	b .L_02008468
.L_020082ee:
	movs r3, #2
	str r3, [sp, #0]
	movs r1, #15
	movs r2, #9
	movs r3, #4
	movs r0, #20
	bl UiWindow_Create
	ldr r5, .L_02008488
	adds r6, r0, #0
	adds r1, r6, #0
	adds r0, r5, #0
	movs r2, #0
	movs r3, #0
	bl UiText_DrawCharacterAtOffset
	adds r0, r5, #1
	adds r1, r6, #0
	movs r2, #0
	movs r3, #8
	bl UiText_DrawCharacterAtOffset
	movs r0, #5
	bl WaitTicks
	movs r0, #116
	bl Func_02001408
	b .L_0200832e
.L_02008328:
	movs r0, #1
	bl WaitTicks
.L_0200832e:
	ldr r1, .L_0200848c
	movs r2, #1
	ldr r3, [r1, #4]
	ands r3, r2
	cmp r3, #0
	bne .L_020082bc
	ldr r3, [r1, #4]
	movs r2, #2
	ands r3, r2
	cmp r3, #0
	beq .L_02008328
	movs r0, #113
	bl Func_02001408
	movs r5, #1
	negs r5, r5
.L_0200834e:
	adds r0, r6, #0
	movs r1, #2
	bl UiWork_Finalize
	movs r2, #1
	negs r2, r2
	cmp r5, r2
	bne .L_02008360
	b .L_02008468
.L_02008360:
	cmp r7, #0
	bne .L_0200836e
	movs r0, #1
	negs r0, r0
	bl Party_AdjustCoins
	b .L_02008378
.L_0200836e:
	cmp r7, #1
	bne .L_02008378
	movs r0, #229
	bl PartyInventory_Remove
.L_02008378:
	adds r0, r7, #0
	bl Func_02001184
	adds r5, r0, #0
	cmp r7, #0
	bne .L_020083c0
	cmp r5, #4
	beq .L_020083b2
	ldr r6, .L_02008490
	lsls r5, r5, #1
	ldrh r0, [r6, r5]
	bl Party_AdjustCoins
	movs r0, #91
	bl Func_02001408
	movs r1, #5
	ldrh r0, [r6, r5]
	bl Func_02001328
	ldr r0, .L_02008494
	bl Func_020013c8
	movs r0, #1
	negs r0, r0
	movs r1, #0
	bl Func_020013d8
	b .L_02008468
.L_020083b2:
	movs r0, #113
	bl Func_02001408
	movs r0, #10
	bl FieldEvent_WaitTicks
	b .L_02008468
.L_020083c0:
	movs r7, #0
	cmp r5, #0
	bne .L_020083e8
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #11
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020083e8
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #11
	bl GameFlag_SetBit
	movs r0, #1
	negs r0, r0
	bl Func_0200049c
	b .L_02008468
.L_020083e8:
	lsls r3, r5, #1
	adds r0, r3, r5
	movs r6, #0
	adds r3, r0, #3
	cmp r6, r3
	bge .L_0200840a
	ldr r1, .L_0200847c
.L_020083f6:
	movs r3, #158
	lsls r3, r3, #1
	adds r2, r6, r3
	adds r3, r1, #1
	ldrsb r3, [r3, r2]
	adds r6, #1
	adds r7, r7, r3
	adds r3, r0, #3
	cmp r6, r3
	blt .L_020083f6
.L_0200840a:
	bl Random16Far
	adds r3, r7, #0
	muls r3, r0
	ldr r0, .L_0200847c
	movs r2, #158
	lsrs r1, r3, #16
	lsls r2, r2, #1
	adds r3, r0, #1
	ldrsb r3, [r3, r2]
	movs r6, #0
	subs r1, r1, r3
	cmp r1, #0
	blt .L_0200843c
.L_02008426:
	adds r6, #1
	cmp r6, #14
	bgt .L_0200843c
	movs r2, #158
	lsls r2, r2, #1
	adds r3, r6, r2
	adds r2, r0, #1
	ldrsb r3, [r2, r3]
	subs r1, r1, r3
	cmp r1, #0
	bge .L_02008426
.L_0200843c:
	cmp r6, #15
	bne .L_02008442
	movs r6, #14
.L_02008442:
	ldr r2, .L_02008498
	lsls r3, r6, #2
	ldr r0, [r2, r3]
	bl Func_0200049c
	ldr r3, .L_0200847c
	movs r1, #158
	lsls r1, r1, #1
	adds r2, r6, r1
	adds r0, r3, #1
	ldrb r3, [r0, r2]
	lsls r3, r3, #24
	asrs r1, r3, #24
	cmp r1, #1
	ble .L_02008468
	lsrs r3, r3, #31
	adds r3, r1, r3
	asrs r3, r3, #1
	strb r3, [r0, r2]
.L_02008468:
	bl Func_02001388
	movs r0, #0
	add sp, #4
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200847c:
	.4byte gPartyState
.L_02008480:
	.4byte MsgWhatWillYou
.L_02008484:
	.4byte MsgCoins2
.L_02008488:
	.4byte MsgToss2
.L_0200848c:
	.4byte gInput
.L_02008490:
	.4byte gData_0200173c
.L_02008494:
	.4byte MsgYouWonCoin
.L_02008498:
	.4byte gData_02001700
	.endif
	.section .text.x02008884,"ax",%progbits
	.global Func_02000884
	.thumb_func
Func_02000884:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r6, r1, #0
	adds r7, r2, #0
	mov r8, r3
	bl Object_GetById
	adds r5, r0, #0
	cmp r5, #0
	beq .L_020088ba
	ldmia r6!, {r3}
	ldr r2, .L_020088cc
	str r3, [r5, #8]
	ldmia r6!, {r3}
	str r3, [r5, #12]
	ldr r3, [r6]
	strh r7, [r5, #6]
	str r3, [r5, #16]
	adds r3, r5, #0
	adds r3, #85
	strb r2, [r3]
	ldr r3, [r5, #80]
	strb r2, [r3, #26]
	ldr r1, [sp, #20]
	bl ObjectDispatch_SetAnimationSpeed
.L_020088ba:
	ldr r0, [r5, #80]
	movs r1, #0
	ldrb r3, [r0, #27]
	cmp r1, r3
	bge .L_020088e8
	adds r4, r0, #0
	adds r4, #40
	b .L_020088d0
	.2byte 0x0000
.L_020088cc:
	.4byte 0x00000000
.L_020088d0:
	ldmia r4!, {r2}
	ldrb r3, [r2, #5]
	cmp r3, r8
	beq .L_020088e0
	mov r3, r8
	strb r3, [r2, #5]
	movs r3, #255
	strb r3, [r2, #22]
.L_020088e0:
	ldrb r3, [r0, #27]
	adds r1, #1
	cmp r1, r3
	blt .L_020088d0
.L_020088e8:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x02008904,"ax",%progbits
	.global Func_02000904
	.thumb_func
Func_02000904:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r7, .L_02008b08
	movs r0, #3
	adds r2, r7, #0
	sub sp, #24
	mov r11, r0
	adds r2, #28
.L_0200891e:
	ldr r3, [r2]
	movs r1, #1
	str r3, [r2, #12]
	ldr r3, [r2, #4]
	negs r1, r1
	str r3, [r2, #16]
	ldr r3, [r2, #8]
	add r11, r1
	str r3, [r2, #20]
	mov r3, r11
	subs r2, #12
	cmp r3, #0
	bne .L_0200891e
	movs r0, #2
	ldrsh r3, [r7, r0]
	cmp r3, #31
	bgt .L_02008942
	b .L_02008c94
.L_02008942:
	ldr r3, [r7, #4]
	ldr r2, [r7, #64]
	ldr r1, [r7, #8]
	adds r3, r3, r2
	str r3, [r7, #4]
	ldr r0, [r7, #68]
	ldr r3, [r7, #12]
	ldr r2, [r7, #72]
	adds r1, r1, r0
	adds r3, r3, r2
	str r1, [r7, #8]
	str r3, [r7, #12]
	cmp r1, #0
	ble .L_02008960
	b .L_02008c8e
.L_02008960:
	mov r1, r11
	str r1, [r7, #8]
	cmp r0, #0
	beq .L_0200898c
	str r1, [r7, #68]
	ldr r3, .L_02008b0c
	ldr r3, [r3]
	cmp r3, #1
	bne .L_02008980
	movs r0, #17
	bl Object_GetById
	movs r1, #1
	bl Func_020012e8
	b .L_0200898c
.L_02008980:
	movs r0, #12
	bl Object_GetById
	movs r1, #1
	bl Func_020012e8
.L_0200898c:
	ldr r3, [r7, #76]
	cmp r3, #0
	ble .L_02008a0c
	ldr r3, [r7, #4]
	movs r5, #240
	lsls r5, r5, #15
	subs r5, r5, r3
	ldr r3, [r7, #12]
	movs r6, #142
	lsls r6, r6, #15
	subs r6, r6, r3
	asrs r5, r5, #8
	asrs r6, r6, #8
	adds r0, r5, #0
	muls r0, r5
	adds r3, r6, #0
	muls r3, r6
	adds r0, r0, r3
	ldr r3, .L_02008b10
	mov lr, r3
	.2byte 0xf800
	movs r2, #200
	lsls r2, r2, #5
	adds r2, #153
	mov r8, r2
	mov r10, r0
	mov r1, r10
	mov r0, r8
	muls r0, r5
	bl __divsi3
	ldr r5, [r7, #64]
	mov r1, r10
	adds r5, r5, r0
	str r5, [r7, #64]
	mov r0, r8
	muls r0, r6
	bl __divsi3
	ldr r3, [r7, #72]
	adds r2, r3, r0
	lsls r3, r5, #6
	subs r3, r3, r5
	lsls r3, r3, #2
	adds r3, r3, r5
	str r2, [r7, #72]
	cmp r3, #0
	bge .L_020089ee
	adds r3, #255
.L_020089ee:
	asrs r3, r3, #8
	str r3, [r7, #64]
	lsls r3, r2, #6
	subs r3, r3, r2
	lsls r3, r3, #2
	adds r3, r3, r2
	cmp r3, #0
	bge .L_02008a00
	adds r3, #255
.L_02008a00:
	asrs r3, r3, #8
	str r3, [r7, #72]
	ldr r3, [r7, #76]
	subs r3, #1
	str r3, [r7, #76]
	b .L_02008b20
.L_02008a0c:
	ldr r3, [r7, #64]
	movs r1, #220
	muls r3, r1
	cmp r3, #0
	bge .L_02008a18
	adds r3, #255
.L_02008a18:
	asrs r2, r3, #8
	ldr r3, [r7, #72]
	str r2, [r7, #64]
	muls r3, r1
	cmp r3, #0
	bge .L_02008a26
	adds r3, #255
.L_02008a26:
	movs r0, #192
	lsls r0, r0, #2
	asrs r3, r3, #8
	adds r0, #255
	str r3, [r7, #72]
	adds r3, r2, r0
	movs r2, #224
	lsls r2, r2, #3
	adds r2, #254
	cmp r3, r2
	bhi .L_02008a40
	movs r3, #0
	str r3, [r7, #64]
.L_02008a40:
	ldr r3, [r7, #72]
	movs r1, #192
	lsls r1, r1, #2
	adds r1, #255
	adds r3, r3, r1
	cmp r3, r2
	bhi .L_02008a52
	movs r3, #0
	str r3, [r7, #72]
.L_02008a52:
	ldr r3, [r7, #64]
	cmp r3, #0
	bne .L_02008b20
	ldr r3, [r7, #72]
	cmp r3, #0
	bne .L_02008b20
	ldr r3, .L_02008b0c
	ldr r3, [r3]
	cmp r3, #1
	bne .L_02008a8c
	movs r0, #17
	bl Object_GetById
	movs r1, #2
	bl Func_020012e8
	movs r0, #15
	movs r1, #0
	bl Func_020008f0
	movs r0, #14
	movs r1, #0
	bl Func_020008f0
	movs r0, #13
	movs r1, #0
	bl Func_020008f0
	b .L_02008ab0
.L_02008a8c:
	movs r0, #12
	bl Object_GetById
	movs r1, #2
	bl Func_020012e8
	movs r0, #10
	movs r1, #0
	bl Func_020008f0
	movs r0, #9
	movs r1, #0
	bl Func_020008f0
	movs r0, #8
	movs r1, #0
	bl Func_020008f0
.L_02008ab0:
	ldr r3, [r7, #4]
	movs r2, #240
	ldr r1, [r7, #12]
	lsls r2, r2, #15
	subs r2, r2, r3
	movs r3, #142
	lsls r3, r3, #15
	subs r3, r3, r1
	asrs r2, r2, #16
	asrs r3, r3, #16
	adds r0, r2, #0
	muls r0, r2
	adds r1, r3, #0
	muls r1, r3
	adds r2, r0, #0
	adds r3, r1, #0
	adds r2, r2, r3
	ldr r3, .L_02008b14
	movs r1, #1
	str r1, [r3]
	ldr r0, .L_02008b18
	cmp r2, #224
	bgt .L_02008ae2
	movs r3, #0
	b .L_02008b1e
.L_02008ae2:
	movs r3, #156
	lsls r3, r3, #2
	cmp r2, r3
	bgt .L_02008aee
	str r1, [r0]
	b .L_02008b20
.L_02008aee:
	movs r1, #136
	lsls r1, r1, #3
	cmp r2, r1
	bgt .L_02008afa
	movs r3, #2
	b .L_02008b1e
.L_02008afa:
	movs r3, #210
	lsls r3, r3, #3
	cmp r2, r3
	bgt .L_02008b1c
	movs r3, #3
	b .L_02008b1e
	.2byte 0x0000
.L_02008b08:
	.4byte gData_02001760
.L_02008b0c:
	.4byte gData_020017b0
.L_02008b10:
	.4byte IwramFillWords + 0x74
.L_02008b14:
	.4byte gData_02001824
.L_02008b18:
	.4byte gData_02001828
.L_02008b1c:
	movs r3, #4
.L_02008b1e:
	str r3, [r0]
.L_02008b20:
	ldr r5, [r7, #12]
	movs r0, #192
	movs r1, #192
	movs r2, #168
	movs r6, #192
	lsls r0, r0, #16
	lsls r1, r1, #13
	movs r4, #240
	lsls r2, r2, #14
	lsls r6, r6, #14
	mov r8, r0
	mov r10, r1
	lsls r4, r4, #15
	cmp r5, r2
	bge .L_02008b70
	movs r3, #168
	lsls r3, r3, #14
	subs r3, r3, r5
	movs r2, #42
	adds r0, r3, #0
	muls r0, r2
	movs r1, #18
	str r4, [sp, #4]
	bl __divsi3
	movs r3, #180
	adds r6, r0, r6
	lsls r3, r3, #15
	ldr r4, [sp, #4]
	cmp r6, r3
	ble .L_02008b60
	adds r6, r3, #0
.L_02008b60:
	mov r3, r8
	subs r3, r3, r0
	movs r0, #150
	mov r8, r3
	lsls r0, r0, #16
	cmp r8, r0
	bge .L_02008b70
	mov r8, r0
.L_02008b70:
	movs r1, #204
	lsls r1, r1, #15
	cmp r5, r1
	ble .L_02008bae
	movs r3, #42
	adds r0, r5, #0
	muls r0, r3
	ldr r2, .L_02008e10
	movs r1, #18
	adds r0, r0, r2
	str r4, [sp, #4]
	bl __divsi3
	movs r3, #192
	lsls r3, r3, #14
	adds r6, r0, r3
	movs r3, #180
	lsls r3, r3, #15
	ldr r4, [sp, #4]
	cmp r6, r3
	ble .L_02008b9c
	adds r6, r3, #0
.L_02008b9c:
	movs r3, #192
	lsls r3, r3, #16
	subs r3, r3, r0
	movs r0, #150
	mov r8, r3
	lsls r0, r0, #16
	cmp r8, r0
	bge .L_02008bae
	mov r8, r0
.L_02008bae:
	ldr r5, [r7, #4]
	movs r1, #180
	lsls r1, r1, #15
	cmp r5, r1
	bge .L_02008bee
	movs r3, #180
	lsls r3, r3, #15
	subs r3, r3, r5
	lsls r0, r3, #3
	adds r0, r0, r3
	lsls r0, r0, #1
	movs r1, #42
	bl __divsi3
	movs r2, #192
	lsls r2, r2, #13
	adds r2, r2, r0
	movs r3, #168
	mov r10, r2
	lsls r3, r3, #14
	cmp r10, r3
	ble .L_02008bdc
	mov r10, r3
.L_02008bdc:
	movs r3, #240
	lsls r3, r3, #15
	subs r4, r3, r0
	movs r3, #204
	lsls r3, r3, #15
	cmp r4, r3
	bge .L_02008bee
	movs r4, #204
	lsls r4, r4, #15
.L_02008bee:
	movs r0, #150
	lsls r0, r0, #16
	cmp r5, r0
	ble .L_02008c2a
	ldr r1, .L_02008e14
	lsls r0, r5, #3
	adds r0, r0, r5
	lsls r0, r0, #1
	adds r0, r0, r1
	movs r1, #42
	bl __divsi3
	movs r2, #192
	lsls r2, r2, #13
	adds r2, r2, r0
	movs r3, #168
	mov r10, r2
	lsls r3, r3, #14
	cmp r10, r3
	ble .L_02008c18
	mov r10, r3
.L_02008c18:
	movs r3, #240
	lsls r3, r3, #15
	subs r4, r3, r0
	movs r3, #204
	lsls r3, r3, #15
	cmp r4, r3
	bge .L_02008c2a
	movs r4, #204
	lsls r4, r4, #15
.L_02008c2a:
	cmp r5, r6
	bge .L_02008c40
	ldr r3, [r7, #64]
	str r6, [r7, #4]
	cmp r3, #0
	bge .L_02008c40
	negs r3, r3
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	str r3, [r7, #64]
.L_02008c40:
	ldr r3, [r7, #4]
	cmp r3, r8
	ble .L_02008c5a
	ldr r3, [r7, #64]
	mov r0, r8
	str r0, [r7, #4]
	cmp r3, #0
	ble .L_02008c5a
	negs r3, r3
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	str r3, [r7, #64]
.L_02008c5a:
	ldr r3, [r7, #12]
	cmp r3, r10
	bge .L_02008c74
	ldr r3, [r7, #72]
	mov r1, r10
	str r1, [r7, #12]
	cmp r3, #0
	bge .L_02008c74
	negs r3, r3
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	str r3, [r7, #72]
.L_02008c74:
	ldr r3, [r7, #12]
	cmp r3, r4
	ble .L_02008c94
	ldr r3, [r7, #72]
	str r4, [r7, #12]
	cmp r3, #0
	ble .L_02008c94
	negs r3, r3
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	str r3, [r7, #72]
	b .L_02008c94
.L_02008c8e:
	ldr r2, .L_02008e18
	adds r3, r0, r2
	str r3, [r7, #68]
.L_02008c94:
	movs r3, #0
	mov r11, r3
.L_02008c98:
	mov r0, r11
	ldr r2, .L_02008e1c
	lsls r3, r0, #1
	add r3, r11
	lsls r3, r3, #3
	adds r6, r3, r2
	movs r1, #18
	ldrsh r3, [r6, r1]
	ldrh r2, [r6, #18]
	cmp r3, #0
	ble .L_02008cb2
	subs r3, r2, #1
	strh r3, [r6, #18]
.L_02008cb2:
	movs r0, #20
	ldrsh r3, [r6, r0]
	ldrh r2, [r6, #20]
	cmp r3, #0
	ble .L_02008cc0
	subs r3, r2, #1
	strh r3, [r6, #20]
.L_02008cc0:
	mov r2, r11
	adds r2, #1
	mov r3, r11
	ldrh r0, [r6, #18]
	ldrh r1, [r6, #16]
	str r2, [sp, #8]
	cmp r3, #1
	bgt .L_02008d6c
	lsls r3, r1, #16
	movs r5, #128
	asrs r3, r3, #16
	lsls r5, r5, #9
	cmp r3, #1
	bne .L_02008cde
	lsls r5, r5, #1
.L_02008cde:
	cmp r3, #2
	bne .L_02008ce6
	lsls r3, r5, #1
	adds r5, r3, r5
.L_02008ce6:
	lsls r3, r0, #16
	cmp r3, #0
	ble .L_02008cfa
	mov r0, r11
	cmp r0, #0
	bne .L_02008cf6
	movs r0, #18
	b .L_02008d90
.L_02008cf6:
	movs r0, #19
	b .L_02008d90
.L_02008cfa:
	mov r1, r11
	cmp r1, #0
	bne .L_02008d0e
	movs r0, #18
	bl Object_GetById
	movs r1, #1
	bl Func_020012e8
	b .L_02008d1a
.L_02008d0e:
	movs r0, #19
	bl Object_GetById
	movs r1, #1
	bl Func_020012e8
.L_02008d1a:
	movs r0, #14
	ldrsh r3, [r6, r0]
	ldrh r2, [r6, #14]
	cmp r3, #0
	bne .L_02008d68
	movs r1, #12
	ldrsh r3, [r6, r1]
	ldr r2, [r6]
	cmp r3, #0
	bne .L_02008d32
	adds r3, r2, r5
	b .L_02008d34
.L_02008d32:
	subs r3, r2, r5
.L_02008d34:
	str r3, [r6]
	ldr r3, [r6]
	movs r2, #128
	lsls r2, r2, #15
	cmp r3, r2
	bgt .L_02008d4e
	movs r3, #0
	strh r3, [r6, #12]
	mov r3, r11
	cmp r3, #1
	bne .L_02008d4e
	movs r3, #30
	strh r3, [r6, #14]
.L_02008d4e:
	ldr r3, [r6]
	ldr r0, .L_02008e20
	cmp r3, r0
	bgt .L_02008d58
	b .L_02008e7c
.L_02008d58:
	movs r3, #1
	mov r1, r11
	strh r3, [r6, #12]
	cmp r1, #1
	beq .L_02008d64
	b .L_02008e7c
.L_02008d64:
	movs r3, #30
	b .L_02008e7a
.L_02008d68:
	subs r3, r2, #1
	b .L_02008e7a
.L_02008d6c:
	mov r2, r11
	cmp r2, #2
	bne .L_02008dde
	lsls r3, r1, #16
	movs r5, #64
	asrs r3, r3, #16
	negs r5, r5
	cmp r3, #1
	bne .L_02008d80
	lsls r5, r5, #1
.L_02008d80:
	cmp r3, #2
	bne .L_02008d88
	lsls r3, r5, #1
	adds r5, r3, r5
.L_02008d88:
	lsls r3, r0, #16
	cmp r3, #0
	ble .L_02008d9c
	movs r0, #20
.L_02008d90:
	bl Object_GetById
	movs r1, #3
	bl Func_020012e8
	b .L_02008e7c
.L_02008d9c:
	movs r0, #20
	bl Object_GetById
	movs r1, #2
	bl Func_020012e8
	movs r3, #12
	ldrsh r0, [r6, r3]
	bl Math_Sine
	lsls r3, r0, #1
	adds r3, r3, r0
	movs r0, #224
	lsls r0, r0, #15
	lsls r3, r3, #4
	adds r3, r3, r0
	str r3, [r6]
	movs r1, #12
	ldrsh r0, [r6, r1]
	bl Math_Cosine
	lsls r3, r0, #2
	adds r3, r3, r0
	movs r2, #144
	lsls r2, r2, #15
	lsls r3, r3, #3
	adds r3, r3, r2
	str r3, [r6, #8]
	ldrh r3, [r6, #12]
	adds r2, r3, r5
	ldrh r3, [r6, #14]
	strh r2, [r6, #12]
	b .L_02008e78
.L_02008dde:
	ldrh r3, [r6, #14]
	ldr r2, .L_02008e0c
	movs r5, #64
	ands r2, r3
	lsls r3, r1, #16
	asrs r3, r3, #16
	cmp r3, #1
	bne .L_02008df0
	movs r5, #128
.L_02008df0:
	cmp r3, #2
	bne .L_02008df8
	lsls r3, r5, #1
	adds r5, r3, r5
.L_02008df8:
	lsls r3, r0, #16
	cmp r3, #0
	ble .L_02008e24
	movs r0, #21
	bl Object_GetById
	movs r1, #3
	bl Func_020012e8
	b .L_02008e76
.L_02008e0c:
	.4byte 0x000001ff
.L_02008e10:
	.4byte 0xef440000
.L_02008e14:
	.4byte 0xf5740000
.L_02008e18:
	.4byte 0xffffc000
.L_02008e1c:
	.4byte gData_020017c0
.L_02008e20:
	.4byte 0x00afffff
.L_02008e24:
	movs r3, #128
	adds r3, #255
	cmp r2, r3
	bgt .L_02008e6a
	movs r1, #12
	ldrsh r0, [r6, r1]
	bl Math_Sine
	movs r3, #52
	muls r3, r0
	movs r2, #224
	lsls r2, r2, #15
	adds r3, r3, r2
	str r3, [r6]
	movs r3, #12
	ldrsh r0, [r6, r3]
	bl Math_Cosine
	lsls r3, r0, #1
	adds r3, r3, r0
	movs r0, #144
	lsls r0, r0, #15
	lsls r3, r3, #3
	adds r3, r3, r0
	str r3, [r6, #8]
	ldrh r3, [r6, #12]
	movs r0, #21
	adds r3, r3, r5
	strh r3, [r6, #12]
	bl Object_GetById
	movs r1, #2
	bl Func_020012e8
	b .L_02008e76
.L_02008e6a:
	movs r0, #21
	bl Object_GetById
	movs r1, #3
	bl Func_020012e8
.L_02008e76:
	ldrh r3, [r6, #14]
.L_02008e78:
	adds r3, #1
.L_02008e7a:
	strh r3, [r6, #14]
.L_02008e7c:
	movs r1, #20
	ldrsh r3, [r6, r1]
	cmp r3, #0
	bne .L_02008f34
	ldr r3, [r7, #8]
	cmp r3, #0
	bne .L_02008f34
	ldr r2, [r7, #4]
	ldr r3, [r6]
	subs r3, r3, r2
	asrs r3, r3, #16
	mov r8, r3
	ldr r2, [r7, #12]
	ldr r3, [r6, #8]
	subs r3, r3, r2
	asrs r3, r3, #16
	mov r10, r3
	mov r0, r10
	mov r3, r8
	mov r2, r8
	muls r2, r3
	mov r3, r10
	muls r3, r0
	adds r0, r2, r3
	cmp r0, #119
	bgt .L_02008f34
	ldr r1, [r7, #76]
	cmp r1, #30
	ble .L_02008f34
	movs r2, #192
	lsls r2, r2, #10
	mov r3, r11
	mov r9, r2
	cmp r3, #1
	bgt .L_02008ee8
	movs r0, #12
	ldrsh r3, [r6, r0]
	ldr r2, [r7, #64]
	cmp r3, #0
	bne .L_02008eda
	cmp r2, r9
	bge .L_02008f16
	adds r3, r1, #0
	mov r2, r9
	subs r3, #100
	str r2, [r7, #64]
	b .L_02008f14
.L_02008eda:
	mov r0, r9
	negs r3, r0
	cmp r2, r3
	ble .L_02008f16
	str r3, [r7, #64]
	adds r3, r1, #0
	b .L_02008f12
.L_02008ee8:
	ldr r3, .L_020090e4
	mov lr, r3
	.2byte 0xf800
	mov r1, r8
	negs r3, r1
	adds r5, r0, #0
	adds r1, r5, #0
	mov r0, r9
	muls r0, r3
	bl __divsi3
	mov r2, r10
	negs r3, r2
	str r0, [r7, #64]
	adds r1, r5, #0
	mov r0, r9
	muls r0, r3
	bl __divsi3
	ldr r3, [r7, #76]
	str r0, [r7, #72]
.L_02008f12:
	subs r3, #100
.L_02008f14:
	str r3, [r7, #76]
.L_02008f16:
	movs r0, #46
	adds r0, #255
	bl Func_02001408
	movs r3, #16
	ldrsh r0, [r6, r3]
	movs r1, #3
	adds r0, #1
	bl __modsi3
	movs r3, #36
	strh r3, [r6, #18]
	movs r3, #30
	strh r0, [r6, #16]
	strh r3, [r6, #20]
.L_02008f34:
	mov r0, r11
	cmp r0, #1
	beq .L_02008f5a
	cmp r0, #1
	bgt .L_02008f44
	cmp r0, #0
	beq .L_02008f50
	b .L_02008fb8
.L_02008f44:
	mov r1, r11
	cmp r1, #2
	beq .L_02008f74
	cmp r1, #3
	beq .L_02008f96
	b .L_02008fb8
.L_02008f50:
	movs r0, #16
	ldrsh r2, [r6, r0]
	ldr r3, .L_020090e8
	movs r0, #18
	b .L_02008f62
.L_02008f5a:
	movs r1, #16
	ldrsh r2, [r6, r1]
	ldr r3, .L_020090e8
	movs r0, #19
.L_02008f62:
	ldrb r3, [r3, r2]
	lsls r2, r2, #4
	adds r2, #16
	str r2, [sp, #0]
	adds r1, r6, #0
	movs r2, #0
	bl Func_02000884
	b .L_02008fb8
.L_02008f74:
	movs r2, #12
	ldrsh r3, [r6, r2]
	movs r2, #128
	lsls r2, r2, #8
	movs r0, #16
	ldrsh r1, [r6, r0]
	subs r2, r2, r3
	ldr r3, .L_020090ec
	movs r0, #20
	ldrb r3, [r3, r1]
	lsls r1, r1, #4
	adds r1, #16
	str r1, [sp, #0]
	adds r1, r6, #0
	bl Func_02000884
	b .L_02008fb8
.L_02008f96:
	movs r1, #12
	ldrsh r3, [r6, r1]
	movs r2, #255
	lsls r2, r2, #8
	adds r2, #255
	movs r0, #16
	ldrsh r1, [r6, r0]
	subs r2, r2, r3
	ldr r3, .L_020090ec
	movs r0, #21
	ldrb r3, [r3, r1]
	lsls r1, r1, #4
	adds r1, #16
	str r1, [sp, #0]
	adds r1, r6, #0
	bl Func_02000884
.L_02008fb8:
	ldr r1, [sp, #8]
	mov r11, r1
	cmp r1, #4
	beq .L_02008fc2
	b .L_02008c98
.L_02008fc2:
	ldr r3, [r7, #4]
	movs r2, #16
	str r3, [r7, #52]
	movs r3, #0
	str r3, [r7, #56]
	ldr r3, [r7, #12]
	adds r2, r2, r7
	str r3, [r7, #60]
	ldr r3, .L_020090f0
	mov r8, r2
	ldr r3, [r3]
	movs r0, #28
	movs r2, #40
	adds r6, r7, #0
	adds r0, r0, r7
	adds r2, r2, r7
	adds r1, r7, #4
	adds r6, #52
	mov r10, r0
	mov r9, r2
	cmp r3, #1
	bne .L_0200905a
	movs r5, #16
	movs r0, #17
	movs r2, #0
	movs r3, #0
	str r5, [sp, #0]
	bl Func_02000884
	movs r0, #16
	adds r1, r6, #0
	movs r2, #0
	movs r3, #0
	str r5, [sp, #0]
	bl Func_02000884
	movs r0, #15
	mov r1, r8
	movs r2, #0
	movs r3, #0
	str r5, [sp, #0]
	bl Func_02000884
	movs r0, #14
	mov r1, r10
	movs r2, #0
	movs r3, #0
	str r5, [sp, #0]
	bl Func_02000884
	movs r2, #0
	movs r3, #0
	mov r1, r9
	movs r0, #13
	str r5, [sp, #0]
	bl Func_02000884
	movs r0, #15
	bl Object_GetById
	movs r1, #4
	bl Func_020012e8
	movs r0, #14
	bl Object_GetById
	movs r1, #4
	bl Func_020012e8
	movs r0, #13
	bl Object_GetById
	movs r1, #4
	bl Func_020012e8
	b .L_020090c4
.L_0200905a:
	movs r5, #16
	movs r0, #12
	movs r2, #0
	movs r3, #0
	str r5, [sp, #0]
	bl Func_02000884
	movs r0, #11
	adds r1, r6, #0
	movs r2, #0
	movs r3, #0
	str r5, [sp, #0]
	bl Func_02000884
	movs r0, #10
	mov r1, r8
	movs r2, #0
	movs r3, #0
	str r5, [sp, #0]
	bl Func_02000884
	movs r0, #9
	mov r1, r10
	movs r2, #0
	movs r3, #0
	str r5, [sp, #0]
	bl Func_02000884
	movs r2, #0
	movs r3, #0
	mov r1, r9
	movs r0, #8
	str r5, [sp, #0]
	bl Func_02000884
	movs r0, #10
	bl Object_GetById
	movs r1, #4
	bl Func_020012e8
	movs r0, #9
	bl Object_GetById
	movs r1, #4
	bl Func_020012e8
	movs r0, #8
	bl Object_GetById
	movs r1, #4
	bl Func_020012e8
.L_020090c4:
	movs r0, #2
	ldrsh r3, [r7, r0]
	movs r1, #1
	negs r1, r1
	ldrh r2, [r7, #2]
	cmp r3, r1
	beq .L_020090d6
	adds r3, r2, #1
	strh r3, [r7, #2]
.L_020090d6:
	add sp, #24
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_020090e4:
	.4byte IwramFillWords + 0x74
.L_020090e8:
	.4byte gData_02001746
.L_020090ec:
	.4byte gData_02001748 + 0x1
.L_020090f0:
	.4byte gData_020017b0
	.section .bss,"aw",%nobits
	.space 0x00000004
	.global gData_02001760
gData_02001760:
	.space 0x00000050
	.global gData_020017b0
gData_020017b0:
	.space 0x00000010
	.global gData_020017c0
gData_020017c0:
	.space 0x00000060
	.global gData_02001820
gData_02001820:
	.space 0x00000004
	.global gData_02001824
gData_02001824:
	.space 0x00000004
	.global gData_02001828
gData_02001828:
