.syntax unified
	.thumb
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
	bl FieldMessage_ShowAtActor
	movs r3, #2
	str r3, [sp, #0]
	movs r1, #0
	movs r2, #17
	movs r3, #4
	movs r0, #0
	bl Window_Create
	ldr r5, .L_ja_020084a4
	adds r7, r0, #0
	adds r1, r7, #0
	adds r0, r5, #0
	movs r2, #0
	movs r3, #0
	bl Text_DrawCharacterAtOffset
	movs r3, #0
	str r3, [sp, #0]
	mov r0, fp
	movs r1, #6
	adds r2, r7, #0
	movs r3, #56
	adds r6, r5, #2
	bl Text_DrawNumberInWindow
	adds r0, r6, #0
	adds r1, r7, #0
	movs r2, #104
	movs r3, #0
	bl Text_DrawCharacterAtOffset
	adds r0, r5, #1
	adds r1, r7, #0
	movs r2, #0
	movs r3, #8
	bl Text_DrawCharacterAtOffset
	movs r3, #8
	str r3, [sp, #0]
	mov r0, r9
	movs r1, #6
	adds r2, r7, #0
	movs r3, #56
	bl Text_DrawNumberInWindow
	movs r3, #8
	movs r2, #104
	adds r1, r7, #0
	adds r0, r6, #0
	bl Text_DrawCharacterAtOffset
	mov r0, r8
	bl Menu_SelectCoinsOrMedals
	movs r1, #2
	mov r8, r0
	adds r0, r7, #0
	bl Window_Close
	bl Message_CloseFinishedWindows
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
	bl wait
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
	bl FieldMessage_ShowAtActor
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
	bl Window_Create
	ldr r5, .L_ja_020084a8
	adds r7, r0, #0
	adds r1, r7, #0
	adds r0, r5, #0
	movs r2, #0
	movs r3, #0
	bl Text_DrawCharacterAtOffset
	adds r0, r5, #1
	adds r1, r7, #0
	movs r2, #0
	movs r3, #8
	bl Text_DrawCharacterAtOffset
	movs r0, #5
	bl wait
	movs r0, #116
	bl Func_02001408
	b .L_ja_02008362
.L_ja_0200835c:
	movs r0, #1
	bl wait
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
	bl Window_Close
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
	bl FieldEvent_Wait
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
	.4byte gMainRoutineWork
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
	bl FieldMessage_ShowAtActor
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
	bl Window_Create
	ldr r5, .L_02008484
	adds r6, r0, #0
	adds r1, r6, #0
	adds r0, r5, #0
	movs r2, #0
	movs r3, #0
	bl Text_DrawCharacterAtOffset
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
	bl Text_DrawNumberInWindow
	adds r0, r5, #1
	adds r1, r6, #0
	movs r2, #0
	movs r3, #8
	bl Text_DrawCharacterAtOffset
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
	bl Text_DrawNumberInWindow
	adds r0, r7, #0
	bl Menu_SelectCoinsOrMedals
	movs r1, #2
	adds r7, r0, #0
	adds r0, r6, #0
	bl Window_Close
	bl Message_CloseFinishedWindows
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
	bl wait
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
	bl FieldMessage_ShowAtActor
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
	bl Window_Create
	ldr r5, .L_02008488
	adds r6, r0, #0
	adds r1, r6, #0
	adds r0, r5, #0
	movs r2, #0
	movs r3, #0
	bl Text_DrawCharacterAtOffset
	adds r0, r5, #1
	adds r1, r6, #0
	movs r2, #0
	movs r3, #8
	bl Text_DrawCharacterAtOffset
	movs r0, #5
	bl wait
	movs r0, #116
	bl Func_02001408
	b .L_0200832e
.L_02008328:
	movs r0, #1
	bl wait
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
	bl Window_Close
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
	bl FieldEvent_Wait
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
	.4byte gMainRoutineWork
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
