.syntax unified
	.thumb
	.section .text.x0200804c,"ax",%progbits
	.global Func_0200004c
	.thumb_func
Func_0200004c:
	push {lr}
	movs r0, #245
	lsls r0, r0, #3
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008072
	movs r0, #129
	lsls r0, r0, #4
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200806e
	ldr r0, .L_02008088
	b .L_02008086
.L_0200806e:
	ldr r0, .L_0200808c
	b .L_02008086
.L_02008072:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #171
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008084
	ldr r0, .L_02008090
	b .L_02008086
.L_02008084:
	ldr r0, .L_02008094
.L_02008086:
	pop {pc}
.L_02008088:
	.4byte Data_020025b0
.L_0200808c:
	.4byte Data_02002358
.L_02008090:
	.4byte Data_02002100
.L_02008094:
	.4byte Data_02001e48
	.section .text.x02008098,"ax",%progbits
	.global Func_02000098
	.thumb_func
Func_02000098:
	push {r5, r6, lr}
	adds r5, r0, #0
	adds r6, r1, #0
	bl Func_02001be4
	movs r0, #0
	bl Func_02001cfc
	movs r0, #158
	bl Func_02001d4c
	ldrh r1, [r5, #4]
	ldrh r2, [r5, #6]
	ldr r0, [r5]
	bl Func_02001bc4
	ldr r5, .L_02008108
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	ldr r0, [r5]
	bl Object_GetById
	movs r3, #2
	adds r0, #85
	strb r3, [r0]
	movs r1, #128
	movs r2, #128
	ldr r0, [r5]
	lsls r2, r2, #7
	lsls r1, r1, #8
	bl ObjectMotion_SetSpeedParameters
	ldr r0, [r5]
	movs r1, #2
	bl Object_SetModeById
	movs r2, #8
	movs r1, #2
	negs r2, r2
	ldr r0, [r5]
	bl ObjectMotion_CenterOnColumn
	movs r0, #10
	bl Battle_WaitMode0
	adds r0, r6, #0
	bl Func_02001cd4
	bl Event_FinishScreenTransition
	bl Event_WaitTransitionFrames
	bl Func_02001bec
	pop {r5, r6, pc}
.L_02008108:
	.4byte gPartyState
	.section .text.x0200810c,"ax",%progbits
	.global Func_0200010c
	.thumb_func
Func_0200010c:
	push {lr}
	ldr r3, .L_02008120
	adds r1, r0, #0
	lsls r0, r1, #3
	adds r0, r0, r3
	movs r2, #0
	bl Func_02000098
	pop {pc}
	.2byte 0x0000
.L_02008120:
	.4byte Data_0200277c
	.section .text.x0200813c,"ax",%progbits
	.global Func_0200013c
	.thumb_func
Func_0200013c:
	push {r5, r6, lr}
	adds r6, r0, #0
	movs r0, #139
	lsls r0, r0, #4
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200818a
	ldr r5, .L_0200819c
	adds r0, r5, #0
	bl Func_02001c74
	movs r1, #0
	adds r0, r6, #0
	bl UiText_OpenMessageAtObject
	bl Func_02001d34
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_0200817a
	adds r0, r5, #1
	bl Func_02001c74
	movs r0, #139
	lsls r0, r0, #4
	bl GameFlag_SetBit
	b .L_02008180
.L_0200817a:
	adds r0, r5, #2
	bl Func_02001c74
.L_02008180:
	adds r0, r6, #0
	movs r1, #0
	bl Func_02001c8c
	b .L_02008198
.L_0200818a:
	ldr r0, .L_020081a0
	bl Func_02001c74
	adds r0, r6, #0
	movs r1, #0
	bl Func_02001c8c
.L_02008198:
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200819c:
	.4byte MsgIHearTheTidalWave
.L_020081a0:
	.4byte MsgIGuessWhatThoseMen
	.section .text.x020081a4,"ax",%progbits
	.global Func_020001a4
	.thumb_func
Func_020001a4:
	push {r5, r6, lr}
	ldr r5, .L_020081ec
	adds r6, r0, #0
	adds r0, r5, #0
	bl Func_02001c74
	movs r1, #0
	adds r0, r6, #0
	bl UiText_OpenMessageAtObject
	bl Func_02001d34
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_020081d4
	movs r0, #10
	bl Battle_WaitMode0
	adds r0, r5, #1
	bl Func_02001c74
	b .L_020081e0
.L_020081d4:
	movs r0, #20
	bl Battle_WaitMode0
	adds r0, r5, #2
	bl Func_02001c74
.L_020081e0:
	adds r0, r6, #0
	movs r1, #0
	bl Func_02001c8c
	pop {r5, r6, pc}
	.2byte 0x0000
.L_020081ec:
	.4byte MsgLetMeGuessYouRe
	.section .text.x020081f0,"ax",%progbits
	.global Func_020001f0
	.thumb_func
Func_020001f0:
	push {r5, r6, lr}
	ldr r5, .L_02008238
	adds r6, r0, #0
	adds r0, r5, #0
	bl Func_02001c74
	movs r1, #0
	adds r0, r6, #0
	bl UiText_OpenMessageAtObject
	bl Func_02001d34
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_02008220
	movs r0, #10
	bl Battle_WaitMode0
	adds r0, r5, #1
	bl Func_02001c74
	b .L_0200822c
.L_02008220:
	movs r0, #20
	bl Battle_WaitMode0
	adds r0, r5, #2
	bl Func_02001c74
.L_0200822c:
	adds r0, r6, #0
	movs r1, #0
	bl Func_02001c8c
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02008238:
	.4byte MsgDidYouWantToSee
	.section .text.x02008398,"ax",%progbits
	.global Func_02000398
	.thumb_func
Func_02000398:
	push {r5, lr}
	adds r5, r0, #0
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #181
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020083be
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r1, #173
	lsls r1, r1, #1
	adds r2, r3, r1
	movs r3, #1
	ldr r0, .L_02008470
	strh r3, [r2]
	b .L_020083ce
.L_020083be:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #182
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020083dc
	ldr r0, .L_02008474
.L_020083ce:
	bl Func_02001c74
	adds r0, r5, #0
	movs r1, #0
	bl Func_02001c8c
	b .L_0200846c
.L_020083dc:
	ldr r0, .L_02008478
	bl Func_02001c74
	movs r0, #10
	bl Battle_WaitMode0
	adds r0, r5, #0
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r2, #10
	adds r0, r5, #0
	movs r1, #0
	bl Func_02001c84
	adds r0, r5, #0
	movs r1, #2
	bl Motion_SetVarCbAndRefresh
	movs r1, #0
	adds r0, r5, #0
	bl UiText_OpenMessageAtObject
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #4
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_0200844a
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #182
	bl GameFlag_SetBit
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #10
	adds r0, r5, #0
	movs r1, #0
	bl Func_02001c84
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_0200846c
.L_0200844a:
	movs r0, #30
	bl Battle_WaitMode0
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r1, #226
	lsls r1, r1, #1
	adds r2, r2, r1
	ldrh r3, [r2]
	adds r0, r5, #0
	adds r3, #1
	strh r3, [r2]
	movs r1, #0
	movs r2, #10
	bl Func_02001c84
.L_0200846c:
	pop {r5, pc}
	.2byte 0x0000
.L_02008470:
	.4byte MsgIMSoHungryI
.L_02008474:
	.4byte MsgIMFeelingALot
.L_02008478:
	.4byte MsgGeezMisterIDonT
	.section .text.x0200866c,"ax",%progbits
	.global Func_0200066c
	.thumb_func
Func_0200066c:
	push {r5, r6, lr}
	adds r6, r0, #0
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #185
	ldr r5, .L_02008758
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008690
	adds r0, r5, #4
	bl Func_02001c74
	adds r0, r6, #0
	movs r1, #0
	bl UiText_OpenMessageAtObject
	b .L_02008754
.L_02008690:
	adds r0, r5, #0
	bl Func_02001c74
	movs r1, #0
	adds r0, r6, #0
	bl UiText_OpenMessageAtObject
	bl Func_02001d34
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	beq .L_020086c2
	movs r0, #20
	bl Battle_WaitMode0
	adds r0, r5, #1
	bl Func_02001c74
	adds r0, r6, #0
	movs r1, #0
	bl Func_02001c8c
	b .L_02008754
.L_020086c2:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #185
	bl GameFlag_SetBit
	movs r0, #20
	bl Battle_WaitMode0
	adds r0, r5, #2
	bl Func_02001c74
	adds r0, r6, #0
	movs r1, #0
	bl Func_02001c8c
	ldr r5, .L_0200875c
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	movs r2, #172
	ldr r0, [r5]
	movs r1, #72
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	ldr r0, [r5]
	bl ObjectMotion_ArmCallback
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #128
	movs r2, #128
	adds r0, r6, #0
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r2, #164
	adds r0, r6, #0
	movs r1, #56
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #160
	ldr r0, [r5]
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #6
	adds r0, r6, #0
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #3
	adds r0, r6, #0
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	adds r0, r6, #0
	movs r1, #0
	bl Func_02001c8c
.L_02008754:
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02008758:
	.4byte MsgYouReTheOneWho
.L_0200875c:
	.4byte gPartyState
	.section .text.x02008788,"ax",%progbits
	.balign 4
	.global Func_02000788
	.thumb_func
Func_02000788:
	push {lr}
	movs r0, #245
	lsls r0, r0, #3
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020087ae
	movs r0, #129
	lsls r0, r0, #4
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020087aa
	ldr r0, .L_020087c4
	b .L_020087c2
.L_020087aa:
	ldr r0, .L_020087c8
	b .L_020087c2
.L_020087ae:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #171
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020087c0
	ldr r0, .L_020087cc
	b .L_020087c2
.L_020087c0:
	ldr r0, .L_020087d0
.L_020087c2:
	pop {pc}
.L_020087c4:
	.4byte Data_02003154
.L_020087c8:
	.4byte Data_02002e48
.L_020087cc:
	.4byte Data_02002ad0
.L_020087d0:
	.4byte Data_020027f4
	.section .text.x020087d4,"ax",%progbits
	.global Func_020007d4
	.thumb_func
Func_020007d4:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	movs r0, #245
	adds r3, r3, r2
	lsls r0, r0, #3
	subs r2, #172
	str r2, [r3]
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020088aa
	movs r0, #129
	lsls r0, r0, #4
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008836
	movs r0, #24
	bl Object_GetById
	ldr r5, .L_02008a10
	str r5, [r0, #108]
	movs r0, #19
	bl Object_GetById
	movs r1, #2
	str r5, [r0, #108]
	movs r0, #20
	bl ObjectMotion_SetActionVariant
	movs r0, #14
	movs r1, #3
	bl ObjectMotion_SetActionVariant
	movs r0, #16
	movs r1, #3
	bl ObjectMotion_SetActionVariant
	movs r0, #10
	movs r1, #1
	bl ObjectMotion_SetActionVariant
	b .L_02008a0a
.L_02008836:
	movs r0, #14
	bl Object_GetById
	ldr r5, .L_02008a10
	str r5, [r0, #108]
	movs r0, #20
	bl Object_GetById
	str r5, [r0, #108]
	movs r0, #22
	bl Object_GetById
	movs r1, #2
	str r5, [r0, #108]
	movs r0, #13
	bl ObjectMotion_SetActionVariant
	movs r0, #19
	movs r1, #3
	bl ObjectMotion_SetActionVariant
	movs r0, #23
	movs r1, #2
	bl ObjectMotion_SetActionVariant
	movs r0, #24
	movs r1, #3
	bl ObjectMotion_SetActionVariant
	movs r0, #27
	movs r1, #2
	bl ObjectMotion_SetActionVariant
	movs r0, #28
	movs r1, #3
	bl ObjectMotion_SetActionVariant
	movs r0, #29
	movs r1, #1
	bl ObjectMotion_SetActionVariant
	movs r0, #30
	movs r1, #3
	bl ObjectMotion_SetActionVariant
	movs r0, #31
	movs r1, #3
	bl ObjectMotion_SetActionVariant
	movs r0, #12
	movs r1, #3
	bl ObjectMotion_SetActionVariant
	movs r0, #10
	movs r1, #3
	bl ObjectMotion_SetActionVariant
	b .L_02008a0a
.L_020088aa:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #171
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200898a
	movs r0, #14
	bl Object_GetById
	ldr r5, .L_02008a10
	str r5, [r0, #108]
	movs r0, #20
	bl Object_GetById
	movs r1, #2
	str r5, [r0, #108]
	movs r0, #13
	bl ObjectMotion_SetActionVariant
	movs r0, #19
	movs r1, #3
	bl ObjectMotion_SetActionVariant
	movs r0, #23
	movs r1, #3
	bl ObjectMotion_SetActionVariant
	movs r0, #24
	movs r1, #3
	bl ObjectMotion_SetActionVariant
	movs r0, #22
	movs r1, #2
	bl ObjectMotion_SetActionVariant
	movs r0, #27
	movs r1, #2
	bl ObjectMotion_SetActionVariant
	movs r0, #28
	movs r1, #3
	bl ObjectMotion_SetActionVariant
	movs r0, #29
	movs r1, #1
	bl ObjectMotion_SetActionVariant
	movs r0, #30
	movs r1, #3
	bl ObjectMotion_SetActionVariant
	movs r0, #31
	movs r1, #3
	bl ObjectMotion_SetActionVariant
	movs r1, #3
	movs r0, #12
	bl ObjectMotion_SetActionVariant
	movs r0, #26
	bl Object_GetById
	movs r1, #9
	bl Object_SetActionCallback
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #181
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008944
	movs r0, #27
	movs r1, #5
	bl Object_SetModeById
.L_02008944:
	movs r0, #247
	lsls r0, r0, #3
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008962
	movs r0, #30
	movs r1, #6
	bl Object_SetModeById
	movs r0, #31
	movs r1, #6
	bl Object_SetModeById
.L_02008962:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #185
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008a0a
	movs r1, #224
	movs r2, #164
	movs r0, #12
	lsls r1, r1, #14
	lsls r2, r2, #17
	bl Func_02001c3c
	movs r0, #12
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	b .L_02008a0a
.L_0200898a:
	movs r0, #13
	movs r1, #2
	bl ObjectMotion_SetActionVariant
	movs r0, #23
	movs r1, #2
	bl ObjectMotion_SetActionVariant
	movs r0, #10
	movs r1, #3
	bl ObjectMotion_SetActionVariant
	movs r0, #24
	bl Object_GetById
	ldr r3, .L_02008a10
	str r3, [r0, #108]
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #179
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020089c8
	movs r1, #134
	movs r2, #190
	movs r0, #14
	lsls r1, r1, #18
	lsls r2, r2, #18
	bl Func_02001c3c
.L_020089c8:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #180
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020089e0
	movs r0, #25
	movs r1, #0
	movs r2, #0
	bl Func_02001c3c
.L_020089e0:
	ldr r0, .L_02008a14
	bl Func_02001d24
	movs r0, #26
	bl Object_GetById
	movs r1, #9
	bl Object_SetActionCallback
	movs r0, #27
	bl Object_GetById
	movs r1, #9
	bl Object_SetActionCallback
	movs r0, #28
	bl Object_GetById
	movs r1, #9
	bl Object_SetActionCallback
.L_02008a0a:
	movs r0, #0
	pop {r5, pc}
	.2byte 0x0000
.L_02008a10:
	.4byte Func_02000a98
.L_02008a14:
	.4byte Data_02001d54
	.section .text.x02008a18,"ax",%progbits
	.global Func_02000a18
	.thumb_func
Func_02000a18:
	push {lr}
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #171
	sub sp, #8
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008a8c
	ldr r3, .L_02008a94
	ldrh r1, [r3]
	movs r2, #160
	lsls r2, r2, #19
	adds r2, #246
	strh r1, [r2]
	adds r3, #2
	ldrh r1, [r3]
	adds r2, #2
	strh r1, [r2]
	adds r3, #2
	ldrh r1, [r3]
	adds r2, #2
	strh r1, [r2]
	ldrh r2, [r3, #2]
	movs r3, #160
	lsls r3, r3, #19
	adds r3, #252
	strh r2, [r3]
	movs r3, #37
	movs r2, #33
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #37
	movs r1, #18
	movs r2, #2
	movs r3, #2
	bl Func_02001bcc
	movs r3, #39
	movs r2, #31
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #37
	movs r1, #18
	movs r2, #2
	movs r3, #3
	bl Func_02001bcc
	movs r3, #35
	movs r2, #95
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #50
	movs r1, #80
	movs r2, #7
	movs r3, #5
	bl Func_02001bcc
.L_02008a8c:
	movs r0, #0
	add sp, #8
	pop {pc}
	.2byte 0x0000
.L_02008a94:
	.4byte 0x05000116
	.section .text.x02008aa0,"ax",%progbits
	.global Func_02000aa0
	.thumb_func
Func_02000aa0:
	push {r5, lr}
	adds r5, r0, #0
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #179
	bl GameFlag_SetBit
	bl Func_02001be4
	movs r0, #0
	bl Func_02001cfc
	ldr r0, .L_02008b84
	bl Func_02001c74
	movs r1, #128
	movs r2, #128
	movs r0, #29
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r1, #128
	movs r2, #128
	movs r0, #30
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r1, #128
	movs r2, #128
	movs r0, #31
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r1, #128
	movs r2, #128
	movs r0, #32
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r1, #128
	movs r2, #128
	movs r0, #33
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r1, #128
	movs r2, #128
	movs r0, #34
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r1, #128
	movs r2, #128
	movs r0, #35
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r1, #128
	movs r2, #128
	movs r0, #17
	lsls r1, r1, #8
	lsls r2, r2, #7
	bl ObjectMotion_SetSpeedParameters
	movs r1, #128
	movs r2, #128
	movs r0, #14
	lsls r1, r1, #8
	lsls r2, r2, #7
	bl ObjectMotion_SetSpeedParameters
	cmp r5, #20
	bne .L_02008b4c
	movs r2, #8
	movs r0, #4
	movs r1, #0
	negs r2, r2
	bl ObjectMotion_CommitPositionAndActivate
.L_02008b4c:
	movs r1, #204
	movs r2, #218
	movs r0, #31
	lsls r1, r1, #17
	lsls r2, r2, #18
	bl Func_02001c3c
	movs r1, #196
	movs r2, #222
	movs r0, #32
	lsls r1, r1, #17
	lsls r2, r2, #18
	bl Func_02001c3c
	movs r0, #31
	movs r1, #0
	movs r2, #10
	bl Func_02001c84
	cmp r5, #20
	bne .L_02008b88
	movs r1, #128
	movs r0, #4
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	b .L_02008b92
.L_02008b84:
	.4byte MsgFinallyWeMadeIt
.L_02008b88:
	movs r0, #4
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
.L_02008b92:
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #200
	movs r1, #1
	movs r2, #202
	movs r3, #1
	lsls r0, r0, #17
	negs r1, r1
	lsls r2, r2, #18
	bl Motion_CamBounds
	bl Func_02001ccc
	movs r2, #96
	movs r0, #31
	movs r1, #0
	negs r2, r2
	bl ObjectMotion_OffsetPositionAndReset
	movs r2, #96
	negs r2, r2
	movs r0, #32
	movs r1, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #1
	movs r0, #31
	bl Object_SetModeById
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #31
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #32
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #160
	movs r0, #31
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #224
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #32
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #31
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #32
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #160
	movs r0, #31
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #224
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #32
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #31
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #196
	movs r2, #218
	movs r0, #33
	lsls r1, r1, #17
	lsls r2, r2, #18
	bl Func_02001c3c
	movs r1, #204
	movs r2, #218
	movs r0, #35
	lsls r1, r1, #17
	lsls r2, r2, #18
	bl Func_02001c3c
	movs r0, #33
	movs r1, #0
	movs r2, #10
	bl Func_02001c84
	movs r1, #128
	movs r0, #31
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #32
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #48
	movs r0, #35
	movs r1, #0
	negs r2, r2
	bl ObjectMotion_OffsetPositionAndReset
	movs r2, #48
	movs r1, #0
	negs r2, r2
	movs r0, #33
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #35
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r1, #176
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #35
	bl ObjectMotion_ArmCallback
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #31
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #31
	movs r1, #0
	movs r2, #10
	bl Func_02001c84
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #33
	bl Func_02001ca4
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #6
	movs r0, #33
	bl ObjectMotion_ArmCallback
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #33
	bl Motion_SetVarCbAndRefresh
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #33
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #33
	movs r1, #0
	movs r2, #10
	bl Func_02001c84
	movs r1, #160
	movs r0, #31
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #224
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #32
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #31
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #32
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #48
	movs r0, #35
	movs r1, #32
	negs r2, r2
	bl ObjectMotion_OffsetPositionAndReset
	movs r2, #48
	movs r1, #32
	negs r2, r2
	movs r0, #33
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #35
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r1, #160
	movs r0, #35
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r2, #0
	movs r0, #33
	lsls r1, r1, #7
	bl ObjectMotion_ArmCallback
	movs r1, #3
	movs r0, #35
	bl ObjectMotion_SetActionVariant
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #200
	movs r2, #218
	movs r0, #34
	lsls r1, r1, #17
	lsls r2, r2, #18
	bl Func_02001c3c
	movs r0, #34
	movs r1, #0
	movs r2, #10
	bl Func_02001c84
	movs r1, #128
	movs r0, #31
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #32
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #33
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #32
	negs r2, r2
	movs r1, #0
	movs r0, #34
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #1
	bl Battle_WaitMode0
	movs r1, #5
	movs r0, #34
	bl Object_SetModeById
	movs r0, #90
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #34
	bl Motion_SetVarCbAndRefresh
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #34
	movs r1, #0
	movs r2, #10
	bl Func_02001c84
	movs r2, #16
	negs r2, r2
	movs r1, #0
	movs r0, #34
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #1
	bl Battle_WaitMode0
	movs r1, #5
	movs r0, #34
	bl Object_SetModeById
	movs r0, #60
	bl Battle_WaitMode0
	movs r2, #32
	movs r0, #34
	movs r1, #8
	negs r2, r2
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #34
	bl ObjectMotion_ArmCallback
	movs r0, #1
	bl Battle_WaitMode0
	movs r1, #5
	movs r0, #34
	bl Object_SetModeById
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #196
	movs r2, #218
	movs r0, #30
	lsls r1, r1, #17
	lsls r2, r2, #18
	bl Func_02001c3c
	movs r1, #204
	movs r2, #218
	movs r0, #29
	lsls r1, r1, #17
	lsls r2, r2, #18
	bl Func_02001c3c
	movs r2, #32
	movs r0, #30
	movs r1, #0
	negs r2, r2
	bl ObjectMotion_OffsetPositionAndReset
	movs r2, #32
	negs r2, r2
	movs r0, #29
	movs r1, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #1
	movs r0, #30
	bl Object_SetModeById
	movs r0, #1
	bl Battle_WaitMode0
	movs r0, #29
	movs r1, #5
	bl Object_SetModeById
	movs r0, #31
	movs r1, #30
	bl Object_LinkObjectAndSetCallback
	movs r0, #32
	movs r1, #30
	bl Object_LinkObjectAndSetCallback
	movs r0, #33
	movs r1, #30
	bl Object_LinkObjectAndSetCallback
	movs r0, #34
	movs r1, #30
	bl Object_LinkObjectAndSetCallback
	movs r1, #30
	movs r0, #35
	bl Object_LinkObjectAndSetCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #160
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #30
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #0
	movs r2, #0
	movs r0, #30
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #30
	bl ObjectMotion_ArmCallback
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #30
	movs r1, #6
	movs r2, #15
	bl ObjectMotion_Launch
	movs r2, #23
	movs r0, #30
	movs r1, #6
	bl ObjectMotion_Launch
	movs r0, #34
	movs r1, #1
	bl Object_SetModeById
	movs r2, #10
	movs r0, #30
	movs r1, #0
	bl Func_02001c84
	ldr r1, .L_02009334
	movs r0, #29
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #32
	movs r0, #30
	movs r1, #0
	negs r2, r2
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #32
	movs r2, #0
	negs r1, r1
	movs r0, #30
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #30
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #30
	movs r1, #0
	bl Func_02001c84
	movs r0, #29
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #29
	movs r1, #0
	movs r2, #10
	bl Func_02001c84
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #30
	bl Func_02001ca4
	movs r2, #0
	movs r1, #0
	movs r0, #30
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #29
	bl Motion_SetVarCbAndRefresh
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #29
	movs r1, #0
	bl Func_02001c84
	movs r0, #29
	movs r1, #5
	bl Object_SetModeById
	movs r0, #30
	movs r1, #64
	movs r2, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #160
	movs r0, #29
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #224
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #30
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #5
	movs r2, #10
	adds r0, #30
	movs r1, #0
	bl Func_02001c84
	movs r1, #1
	movs r0, #29
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #29
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #10
	movs r1, #0
	movs r0, #29
	bl Func_02001c84
	movs r0, #31
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #32
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #33
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #34
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #35
	bl ObjectMotion_EnableActionAndResetMotion
	movs r1, #5
	movs r0, #29
	bl Object_SetModeById
	movs r0, #50
	bl Battle_WaitMode0
	movs r1, #160
	movs r0, #31
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #224
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #32
	bl ObjectMotion_ArmCallback
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #192
	movs r0, #31
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #6
	movs r0, #32
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	ldr r1, .L_02009338
	movs r0, #14
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #31
	movs r1, #6
	movs r2, #15
	bl ObjectMotion_Launch
	movs r0, #31
	movs r1, #6
	movs r2, #23
	bl ObjectMotion_Launch
	movs r1, #0
	movs r2, #10
	movs r0, #31
	bl Func_02001c84
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #32
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #32
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #32
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #32
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #5
	movs r2, #0
	movs r0, #32
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #32
	movs r1, #0
	bl Func_02001c84
	movs r1, #1
	movs r0, #29
	bl Object_SetModeById
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #8
	adds r1, #255
	movs r2, #30
	movs r0, #29
	bl Func_02001ca4
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #29
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #29
	movs r1, #0
	bl Func_02001c84
	movs r0, #29
	movs r1, #1
	bl Object_AttachWorkTargetToObject
	bl Func_02001ccc
	movs r1, #244
	movs r2, #202
	movs r0, #29
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_SetPositionAndReset
	movs r0, #31
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #32
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #33
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #34
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #35
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #30
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #208
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #29
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #29
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #11
	lsls r1, r1, #8
	bl Func_02001cbc
	movs r0, #162
	movs r1, #1
	movs r2, #134
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #18
	lsls r0, r0, #18
	bl Motion_CamBounds
	bl Func_02001ccc
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #29
	movs r1, #0
	movs r2, #10
	bl Func_02001c84
	movs r0, #220
	movs r1, #1
	movs r2, #202
	movs r3, #1
	lsls r2, r2, #18
	lsls r0, r0, #17
	negs r1, r1
	bl Motion_CamBounds
	bl Func_02001ccc
	movs r0, #128
	movs r1, #128
	lsls r1, r1, #6
	lsls r0, r0, #9
	bl Func_02001cbc
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #33
	movs r1, #32
	movs r2, #36
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #33
	movs r1, #32
	movs r2, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #252
	movs r2, #202
	movs r0, #33
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_SetPositionAndReset
	movs r1, #208
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #33
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #33
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #33
	bl Func_02001cac
	movs r0, #50
	bl Battle_WaitMode0
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #33
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #33
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #33
	movs r1, #0
	bl Func_02001c84
	movs r1, #2
	movs r0, #29
	bl Motion_SetVarCbAndRefresh
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #0
	movs r2, #0
	movs r0, #29
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #29
	movs r1, #0
	movs r2, #10
	bl Func_02001c84
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #40
	movs r0, #34
	bl Func_02001ca4
	movs r0, #34
	movs r1, #0
	movs r2, #10
	bl Func_02001c84
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #29
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #29
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #29
	movs r1, #0
	bl Func_02001c84
	movs r1, #2
	movs r0, #30
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #30
	movs r1, #0
	movs r2, #10
	bl Func_02001c84
	movs r1, #160
	movs r0, #31
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #224
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #32
	bl ObjectMotion_ArmCallback
	b .L_0200933c
	.2byte 0x0000
.L_02009334:
	.4byte Data_020033a0
.L_02009338:
	.4byte Data_020033f8
.L_0200933c:
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #31
	movs r1, #6
	movs r2, #15
	bl ObjectMotion_Launch
	movs r0, #31
	movs r1, #6
	movs r2, #23
	bl ObjectMotion_Launch
	movs r2, #10
	movs r0, #31
	movs r1, #0
	bl Func_02001c84
	movs r1, #3
	movs r0, #32
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #32
	movs r1, #0
	bl Func_02001c84
	movs r1, #2
	movs r0, #29
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #192
	movs r0, #31
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #32
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r2, #10
	movs r0, #29
	movs r1, #0
	bl Func_02001c84
	movs r1, #3
	movs r0, #30
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #30
	movs r1, #0
	movs r2, #10
	bl Func_02001c84
	movs r1, #204
	movs r2, #174
	movs r0, #17
	lsls r1, r1, #17
	lsls r2, r2, #18
	bl Func_02001c3c
	movs r1, #204
	movs r2, #190
	movs r0, #17
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_SetPositionAndReset
	movs r1, #224
	movs r2, #198
	movs r0, #17
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_SetPositionAndReset
	movs r1, #224
	movs r2, #202
	movs r0, #17
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_SetPositionAndReset
	movs r0, #30
	movs r1, #8
	movs r2, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #30
	movs r1, #0
	movs r2, #10
	bl Func_02001c84
	movs r1, #2
	adds r1, #255
	movs r2, #50
	movs r0, #17
	bl Func_02001ca4
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #17
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #30
	bl Func_02001ca4
	movs r0, #128
	lsls r0, r0, #5
	movs r2, #10
	adds r0, #30
	movs r1, #0
	bl Func_02001c84
	movs r1, #2
	movs r0, #17
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #220
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	str r2, [r3]
	movs r0, #17
	movs r2, #10
	movs r1, #0
	bl Func_02001c84
	movs r1, #3
	movs r0, #30
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #128
	movs r2, #128
	movs r0, #29
	lsls r1, r1, #8
	lsls r2, r2, #7
	bl ObjectMotion_SetSpeedParameters
	movs r1, #128
	movs r2, #128
	movs r0, #30
	lsls r1, r1, #8
	lsls r2, r2, #7
	bl ObjectMotion_SetSpeedParameters
	movs r1, #128
	movs r2, #128
	movs r0, #31
	lsls r1, r1, #8
	lsls r2, r2, #7
	bl ObjectMotion_SetSpeedParameters
	movs r1, #128
	movs r2, #128
	movs r0, #32
	lsls r1, r1, #8
	lsls r2, r2, #7
	bl ObjectMotion_SetSpeedParameters
	movs r1, #128
	movs r2, #128
	movs r0, #33
	lsls r1, r1, #8
	lsls r2, r2, #7
	bl ObjectMotion_SetSpeedParameters
	movs r1, #128
	movs r2, #128
	movs r0, #34
	lsls r1, r1, #8
	lsls r2, r2, #7
	bl ObjectMotion_SetSpeedParameters
	movs r1, #128
	movs r2, #128
	movs r0, #35
	lsls r1, r1, #8
	lsls r2, r2, #7
	bl ObjectMotion_SetSpeedParameters
	movs r1, #128
	movs r2, #128
	movs r0, #17
	lsls r1, r1, #8
	lsls r2, r2, #7
	bl ObjectMotion_SetSpeedParameters
	movs r0, #30
	movs r1, #1
	bl Object_AttachWorkTargetToObject
	bl Func_02001ccc
	cmp r5, #21
	bne .L_020095e0
	movs r0, #30
	movs r1, #2
	bl ObjectMotion_SetActionVariant
	movs r0, #31
	movs r1, #29
	bl Object_LinkObjectAndSetCallback
	movs r0, #32
	movs r1, #29
	bl Object_LinkObjectAndSetCallback
	movs r0, #34
	movs r1, #29
	bl Object_LinkObjectAndSetCallback
	ldr r1, .L_020095c0
	movs r0, #17
	bl ObjectMotion_EnableActionAndSetCallback
	ldr r1, .L_020095c4
	movs r0, #29
	bl ObjectMotion_EnableActionAndSetCallback
	ldr r1, .L_020095c8
	movs r0, #33
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #80
	bl Battle_WaitMode0
	movs r0, #29
	movs r1, #3
	bl ObjectMotion_SetActionVariant
	movs r0, #30
	movs r1, #3
	bl ObjectMotion_SetActionVariant
	movs r0, #31
	movs r1, #3
	bl ObjectMotion_SetActionVariant
	movs r0, #32
	movs r1, #3
	bl ObjectMotion_SetActionVariant
	movs r0, #33
	movs r1, #3
	bl ObjectMotion_SetActionVariant
	movs r0, #34
	movs r1, #3
	bl ObjectMotion_SetActionVariant
	movs r0, #35
	movs r1, #3
	bl ObjectMotion_SetActionVariant
	movs r0, #17
	movs r1, #3
	bl ObjectMotion_SetActionVariant
	movs r0, #35
	movs r1, #29
	bl Object_LinkObjectAndSetCallback
	ldr r1, .L_020095cc
	movs r0, #30
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #170
	bl Battle_WaitMode0
	ldr r1, .L_020095d0
	movs r0, #32
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #10
	bl Battle_WaitMode0
	ldr r1, .L_020095d4
	movs r0, #31
	bl ObjectMotion_EnableActionAndSetCallback
	ldr r1, .L_020095d8
	movs r0, #34
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #60
	bl Battle_WaitMode0
	ldr r1, .L_020095dc
	movs r0, #35
	bl ObjectMotion_EnableActionAndSetCallback
	b .L_020096ae
.L_020095c0:
	.4byte Data_02003434
.L_020095c4:
	.4byte Data_02003550
.L_020095c8:
	.4byte Data_020035e8
.L_020095cc:
	.4byte Data_020034cc
.L_020095d0:
	.4byte Data_0200374c
.L_020095d4:
	.4byte Data_0200366c
.L_020095d8:
	.4byte Data_020036c8
.L_020095dc:
	.4byte Data_020037f8
.L_020095e0:
	movs r0, #30
	movs r1, #2
	bl ObjectMotion_SetActionVariant
	movs r0, #30
	movs r1, #17
	bl Object_LinkObjectAndSetCallback
	movs r0, #31
	movs r1, #30
	bl Object_LinkObjectAndSetCallback
	movs r0, #32
	movs r1, #30
	bl Object_LinkObjectAndSetCallback
	movs r0, #34
	movs r1, #30
	bl Object_LinkObjectAndSetCallback
	ldr r1, .L_02009934
	movs r0, #17
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #60
	bl Battle_WaitMode0
	ldr r1, .L_02009938
	movs r0, #29
	bl ObjectMotion_EnableActionAndSetCallback
	ldr r1, .L_0200993c
	movs r0, #33
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #70
	bl Battle_WaitMode0
	movs r0, #35
	movs r1, #29
	bl Object_LinkObjectAndSetCallback
	ldr r1, .L_02009940
	movs r0, #30
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #29
	movs r1, #3
	bl ObjectMotion_SetActionVariant
	movs r0, #30
	movs r1, #3
	bl ObjectMotion_SetActionVariant
	movs r0, #31
	movs r1, #3
	bl ObjectMotion_SetActionVariant
	movs r0, #32
	movs r1, #3
	bl ObjectMotion_SetActionVariant
	movs r0, #33
	movs r1, #3
	bl ObjectMotion_SetActionVariant
	movs r0, #34
	movs r1, #3
	bl ObjectMotion_SetActionVariant
	movs r0, #35
	movs r1, #3
	bl ObjectMotion_SetActionVariant
	movs r1, #3
	movs r0, #17
	bl ObjectMotion_SetActionVariant
	movs r0, #120
	bl Battle_WaitMode0
	ldr r1, .L_02009944
	movs r0, #34
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #40
	bl Battle_WaitMode0
	ldr r1, .L_02009948
	movs r0, #31
	bl ObjectMotion_EnableActionAndSetCallback
	ldr r1, .L_0200994c
	movs r0, #35
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #30
	bl Battle_WaitMode0
	ldr r1, .L_02009950
	movs r0, #32
	bl ObjectMotion_EnableActionAndSetCallback
.L_020096ae:
	movs r0, #150
	lsls r0, r0, #1
	bl Battle_WaitMode0
	movs r0, #34
	bl Object_RefreshSelectorById
	movs r0, #50
	bl Battle_WaitMode0
	movs r0, #248
	movs r1, #1
	movs r2, #166
	movs r3, #1
	lsls r0, r0, #16
	negs r1, r1
	lsls r2, r2, #18
	bl Motion_CamBounds
	bl Func_02001ccc
	movs r0, #17
	movs r1, #0
	movs r2, #10
	bl Func_02001c84
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #6
	movs r0, #17
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #17
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #30
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #16
	movs r0, #17
	negs r1, r1
	movs r2, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #0
	movs r2, #0
	movs r0, #17
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #128
	movs r0, #29
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #31
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #32
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #30
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #33
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r2, #0
	movs r1, #0
	movs r0, #34
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #30
	movs r1, #3
	bl Object_SetModeById
	movs r0, #29
	movs r1, #3
	bl Object_SetModeById
	movs r0, #33
	movs r1, #3
	bl Object_SetModeById
	movs r0, #31
	movs r1, #3
	bl Object_SetModeById
	movs r0, #32
	movs r1, #3
	bl Object_SetModeById
	movs r0, #35
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #34
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #192
	movs r0, #29
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #31
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #32
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #30
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #33
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #34
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #243
	movs r2, #243
	lsls r1, r1, #9
	lsls r2, r2, #8
	movs r0, #29
	adds r1, #102
	adds r2, #51
	bl ObjectMotion_SetSpeedParameters
	movs r1, #243
	movs r2, #243
	lsls r1, r1, #9
	lsls r2, r2, #8
	movs r0, #30
	adds r1, #102
	adds r2, #51
	bl ObjectMotion_SetSpeedParameters
	movs r1, #243
	movs r2, #243
	lsls r1, r1, #9
	lsls r2, r2, #8
	movs r0, #31
	adds r1, #102
	adds r2, #51
	bl ObjectMotion_SetSpeedParameters
	movs r1, #243
	movs r2, #243
	lsls r1, r1, #9
	lsls r2, r2, #8
	movs r0, #32
	adds r1, #102
	adds r2, #51
	bl ObjectMotion_SetSpeedParameters
	movs r1, #243
	movs r2, #243
	lsls r1, r1, #9
	lsls r2, r2, #8
	movs r0, #33
	adds r1, #102
	adds r2, #51
	bl ObjectMotion_SetSpeedParameters
	movs r1, #243
	movs r2, #243
	lsls r1, r1, #9
	lsls r2, r2, #8
	movs r0, #34
	adds r1, #102
	adds r2, #51
	bl ObjectMotion_SetSpeedParameters
	movs r1, #243
	movs r2, #243
	lsls r1, r1, #9
	lsls r2, r2, #8
	movs r0, #35
	adds r1, #102
	adds r2, #51
	bl ObjectMotion_SetSpeedParameters
	movs r2, #150
	movs r0, #29
	movs r1, #0
	negs r2, r2
	bl ObjectMotion_OffsetPositionAndReset
	movs r2, #150
	movs r0, #30
	movs r1, #0
	negs r2, r2
	bl ObjectMotion_OffsetPositionAndReset
	movs r2, #150
	movs r0, #31
	movs r1, #0
	negs r2, r2
	bl ObjectMotion_OffsetPositionAndReset
	movs r2, #150
	movs r0, #32
	movs r1, #0
	negs r2, r2
	bl ObjectMotion_OffsetPositionAndReset
	movs r2, #150
	movs r0, #33
	movs r1, #0
	negs r2, r2
	bl ObjectMotion_OffsetPositionAndReset
	movs r2, #150
	movs r0, #35
	movs r1, #0
	negs r2, r2
	bl ObjectMotion_OffsetPositionAndReset
	movs r2, #150
	movs r0, #34
	movs r1, #0
	negs r2, r2
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #29
	movs r1, #0
	movs r2, #0
	bl Func_02001c3c
	movs r0, #30
	movs r1, #0
	movs r2, #0
	bl Func_02001c3c
	movs r0, #31
	movs r1, #0
	movs r2, #0
	bl Func_02001c3c
	movs r0, #32
	movs r1, #0
	movs r2, #0
	bl Func_02001c3c
	movs r0, #33
	movs r1, #0
	movs r2, #0
	bl Func_02001c3c
	movs r0, #34
	movs r1, #0
	movs r2, #0
	bl Func_02001c3c
	movs r1, #0
	movs r2, #0
	movs r0, #35
	bl Func_02001c3c
	movs r0, #15
	bl Battle_WaitMode0
	ldr r3, .L_02009954
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	movs r1, #1
	bl Object_AttachWorkTargetToObject
	bl Func_02001ccc
	bl Func_02001bec
	pop {r5, pc}
	.2byte 0x0000
.L_02009934:
	.4byte Data_02003890
.L_02009938:
	.4byte Data_02003970
.L_0200993c:
	.4byte Data_020039cc
.L_02009940:
	.4byte Data_02003914
.L_02009944:
	.4byte Data_02003a28
.L_02009948:
	.4byte Data_02003b08
.L_0200994c:
	.4byte Data_02003b78
.L_02009950:
	.4byte Data_02003a98
.L_02009954:
	.4byte gPartyState
	.section .text.x02009958,"ax",%progbits
	.global Func_02001958
	.thumb_func
Func_02001958:
	push {r5, r6, lr}
	adds r6, r0, #0
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #180
	bl GameFlag_SetBit
	bl Func_02001be4
	movs r0, #0
	bl Func_02001cfc
	ldr r0, .L_02009bac
	bl Func_02001c74
	cmp r6, #1
	bne .L_0200999e
	movs r0, #10
	bl Battle_WaitMode0
	ldr r5, .L_02009bb0
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	movs r2, #184
	ldr r0, [r5]
	movs r1, #168
	lsls r2, r2, #2
	bl ObjectMotion_SetPositionAndReset
	ldr r0, [r5]
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
.L_0200999e:
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #25
	bl Func_02001ca4
	movs r2, #10
	movs r0, #25
	movs r1, #0
	bl Func_02001c84
	movs r1, #3
	movs r0, #25
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #25
	movs r1, #0
	bl Func_02001c84
	movs r0, #25
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r2, #10
	movs r0, #25
	movs r1, #0
	bl Func_02001c84
	movs r1, #2
	movs r0, #25
	bl Motion_SetVarCbAndRefresh
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #25
	movs r1, #0
	movs r2, #10
	bl Func_02001c84
	cmp r6, #1
	beq .L_020099fc
	b .L_02009b56
.L_020099fc:
	movs r1, #8
	movs r2, #0
	negs r1, r1
	movs r0, #25
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #15
	bl Battle_WaitMode0
	movs r0, #25
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #25
	bl Func_02001cac
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #25
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	movs r2, #184
	strb r3, [r0]
	movs r1, #192
	lsls r2, r2, #2
	movs r0, #25
	bl ObjectMotion_SetPositionAndReset
	movs r0, #1
	bl Battle_WaitMode0
	movs r0, #25
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	strb r3, [r0]
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #25
	movs r1, #0
	movs r2, #10
	bl Func_02001c84
	movs r1, #8
	adds r1, #255
	movs r2, #30
	movs r0, #25
	bl Func_02001ca4
	movs r1, #0
	movs r0, #25
	bl UiText_OpenMessageAtObject
	movs r0, #4
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_02009aba
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #25
	bl Motion_SetVarCbAndRefresh
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #25
	movs r1, #0
	bl Func_02001c84
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_02009adc
.L_02009aba:
	movs r0, #30
	bl Battle_WaitMode0
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #25
	adds r3, #1
	strh r3, [r2]
	movs r1, #0
	movs r2, #10
	bl Func_02001c84
.L_02009adc:
	movs r0, #4
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	movs r2, #188
	strb r3, [r0]
	movs r1, #152
	lsls r2, r2, #2
	movs r0, #4
	bl ObjectMotion_SetPositionAndReset
	movs r0, #1
	bl Battle_WaitMode0
	movs r0, #4
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	movs r1, #224
	strb r3, [r0]
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #4
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #25
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #224
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #25
	bl ObjectMotion_ArmCallback
	movs r0, #70
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #50
	movs r0, #25
	bl Func_02001ca4
	movs r0, #25
	movs r1, #0
	movs r2, #10
	bl Func_02001c84
.L_02009b56:
	movs r2, #182
	movs r0, #25
	movs r1, #184
	lsls r2, r2, #2
	bl ObjectMotion_SetPositionAndReset
	movs r2, #110
	movs r0, #25
	movs r1, #0
	negs r2, r2
	bl ObjectMotion_CommitPositionAndActivate
	cmp r6, #0
	bne .L_02009b9a
	movs r0, #50
	bl Battle_WaitMode0
	movs r1, #132
	movs r2, #162
	movs r0, #25
	lsls r1, r1, #17
	lsls r2, r2, #18
	bl Func_02001c3c
	movs r0, #25
	movs r1, #0
	movs r2, #32
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #25
	movs r1, #150
	movs r2, #0
	bl ObjectMotion_CommitPositionAndActivate
.L_02009b9a:
	movs r0, #25
	movs r1, #0
	movs r2, #0
	bl Func_02001c3c
	bl Func_02001bec
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02009bac:
	.4byte MsgOooThisAinTGood
.L_02009bb0:
	.4byte gPartyState
	.section .rodata.x02009d54,"a",%progbits
	.global Data_02001d54
Data_02001d54:
	.4byte 0x0200001a
	.4byte 0x0201001b
	.4byte 0x0202001c
	.4byte 0x0000ffff
.L_02009d64:
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000d000
	.4byte 0x00000000
	.4byte 0x00000028
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000b000
	.4byte 0x00000000
	.4byte 0x0000003c
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000d000
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000b000
	.4byte 0x00000000
	.4byte 0x0000003c
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0x00000000
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
	.4byte 0x0000005f
	.4byte 0x10101060
	.4byte 0xffffffff
	.4byte 0x10202060
	.4byte 0xffffffff
	.4byte 0x10505060
	.4byte 0xffffffff
	.4byte 0x10606060
	.4byte 0xffffffff
	.4byte 0x10707060
	.4byte 0xffffffff
	.4byte 0x10808065
	.4byte 0xffffffff
	.4byte 0x10914002
	.4byte 0xffffffff
	.4byte 0x10b01062
	.4byte 0xffffffff
	.4byte 0x10c01064
	.4byte 0xffffffff
	.4byte 0x10e01061
	.4byte 0xffffffff
	.4byte 0x000001ff
	.global Data_02001e48
Data_02001e48:
	.4byte 0xffff006c
	.4byte 0x00000001
	.4byte 0x01c80000
	.4byte 0x00000000
	.4byte 0x02200000
	.4byte 0x00014000
	.4byte 0xffff006d
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x02200000
	.4byte 0x00014000
	.4byte 0xffff00e7
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00010000
	.4byte 0xffff00e7
	.4byte 0x00000001
	.4byte 0x01100000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00010000
	.4byte 0xffff0072
	.4byte 0x00000001
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x01400000
	.4byte 0x00014000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x03380000
	.4byte 0x00002000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x03280000
	.4byte 0x0001c000
	.4byte 0xffff0072
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x02d80000
	.4byte 0x00014000
	.4byte 0xffff00bf
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x02e80000
	.4byte 0x0001c000
	.4byte 0xffff0070
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x0001c000
	.4byte 0xffff0071
	.4byte 0x00000001
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x02080000
	.4byte 0x0001a000
	.4byte 0xffff0080
	.4byte 0x00000001
	.4byte 0x02400000
	.4byte 0x00000000
	.4byte 0x02080000
	.4byte 0x00002000
	.4byte 0xffff00ba
	.4byte 0x00000001
	.4byte 0x02280000
	.4byte 0x00000000
	.4byte 0x02300000
	.4byte 0x00012000
	.4byte 0xffff00c3
	.4byte 0x00000001
	.4byte 0x02f80000
	.4byte 0x00000000
	.4byte 0x02380000
	.4byte 0x00010000
	.4byte 0xffff00d7
	.4byte 0x00000001
	.4byte 0x02080000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x00010000
	.4byte 0xffff00e0
	.4byte 0x00000001
	.4byte 0x02780000
	.4byte 0x00000000
	.4byte 0x02580000
	.4byte 0x00014000
	.4byte 0xffff00e1
	.4byte 0x00000002
	.4byte 0x02880000
	.4byte 0x00000000
	.4byte 0x02380000
	.4byte 0x00004000
	.4byte 0xffff0027
	.4byte 0x00000001
	.4byte 0x00c00000
	.4byte 0x00000000
	.4byte 0x02e00000
	.4byte 0x0002e000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x02380000
	.4byte 0x00014000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x02480000
	.4byte 0x00014000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x02c80000
	.4byte 0x00000000
	.4byte 0x02580000
	.4byte 0x00014000
	.4byte 0xffff0013
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x0001c000
	.4byte 0xffff0014
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x0001c000
	.4byte 0xffff0058
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x0001c000
	.4byte 0xffff0058
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x0001c000
	.4byte 0xffff0072
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x0001c000
	.4byte 0xffff0070
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x0001c000
	.4byte 0xffff005d
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x0001c000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02002100
Data_02002100:
	.4byte 0xffff006c
	.4byte 0x00000001
	.4byte 0x01c80000
	.4byte 0x00000000
	.4byte 0x02200000
	.4byte 0x00014000
	.4byte 0xffff006d
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x02200000
	.4byte 0x00014000
	.4byte 0xffff00e7
	.4byte 0x00000001
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00004000
	.4byte 0xffff00e7
	.4byte 0x00000001
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00003000
	.4byte 0xffff0072
	.4byte 0x00000001
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x01400000
	.4byte 0x00004000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x03380000
	.4byte 0x0001e000
	.4byte 0xffff0074
	.4byte 0x00000002
	.4byte 0x02380000
	.4byte 0x00000000
	.4byte 0x03180000
	.4byte 0x00004000
	.4byte 0xffff0072
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x02d80000
	.4byte 0x00004000
	.4byte 0xffff00bf
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x02e80000
	.4byte 0x0000c000
	.4byte 0xffff0070
	.4byte 0x00000002
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x02680000
	.4byte 0x00004000
	.4byte 0xffff0071
	.4byte 0x00000001
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x02080000
	.4byte 0x0000a000
	.4byte 0xffff0080
	.4byte 0x00000001
	.4byte 0x02980000
	.4byte 0x00000000
	.4byte 0x02080000
	.4byte 0x00018000
	.4byte 0xffff00ba
	.4byte 0x00000002
	.4byte 0x02c80000
	.4byte 0x00000000
	.4byte 0x02580000
	.4byte 0x00004000
	.4byte 0xffff00c3
	.4byte 0x00000001
	.4byte 0x02080000
	.4byte 0x00000000
	.4byte 0x02180000
	.4byte 0x00010000
	.4byte 0xffff00d7
	.4byte 0x00000001
	.4byte 0x02680000
	.4byte 0x00000000
	.4byte 0x02580000
	.4byte 0x0000e000
	.4byte 0xffff00e0
	.4byte 0x00000001
	.4byte 0x02d80000
	.4byte 0x00000000
	.4byte 0x01f80000
	.4byte 0x0001a000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x02c80000
	.4byte 0x00000000
	.4byte 0x01f80000
	.4byte 0x0001c000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x02a80000
	.4byte 0x00000000
	.4byte 0x02280000
	.4byte 0x00010000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x02880000
	.4byte 0x00000000
	.4byte 0x02400000
	.4byte 0x0001e000
	.4byte 0xffff00bf
	.4byte 0x00000001
	.4byte 0x02180000
	.4byte 0x00000000
	.4byte 0x02680000
	.4byte 0x00014000
	.4byte 0xffff0072
	.4byte 0x00000001
	.4byte 0x02180000
	.4byte 0x00000000
	.4byte 0x01e80000
	.4byte 0x0000c000
	.4byte 0xffff0080
	.4byte 0x00000001
	.4byte 0x02380000
	.4byte 0x00000000
	.4byte 0x01d80000
	.4byte 0x00004000
	.4byte 0xffff00ba
	.4byte 0x00000001
	.4byte 0x02580000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x0001c000
	.4byte 0xffff00bf
	.4byte 0x00000001
	.4byte 0x02680000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x00014000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02002358
Data_02002358:
	.4byte 0xffff006c
	.4byte 0x00000001
	.4byte 0x01c80000
	.4byte 0x00000000
	.4byte 0x02200000
	.4byte 0x00014000
	.4byte 0xffff006d
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x02200000
	.4byte 0x00014000
	.4byte 0xffff00e7
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00010000
	.4byte 0xffff00e7
	.4byte 0x00000001
	.4byte 0x01100000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00010000
	.4byte 0xffff0072
	.4byte 0x00000001
	.4byte 0x00380000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x00010000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x03380000
	.4byte 0x0000e000
	.4byte 0xffff0074
	.4byte 0x00000002
	.4byte 0x02380000
	.4byte 0x00000000
	.4byte 0x03180000
	.4byte 0x00004000
	.4byte 0xffff0072
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x02d80000
	.4byte 0x00014000
	.4byte 0xffff00bf
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x02e80000
	.4byte 0x0001c000
	.4byte 0xffff0070
	.4byte 0x00000002
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x02680000
	.4byte 0x00004000
	.4byte 0xffff0071
	.4byte 0x00000001
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x02080000
	.4byte 0x0000a000
	.4byte 0xffff0080
	.4byte 0x00000001
	.4byte 0x02a80000
	.4byte 0x00000000
	.4byte 0x02080000
	.4byte 0x00018000
	.4byte 0xffff00ba
	.4byte 0x00000002
	.4byte 0x02c80000
	.4byte 0x00000000
	.4byte 0x02580000
	.4byte 0x00004000
	.4byte 0xffff00c3
	.4byte 0x00000001
	.4byte 0x02080000
	.4byte 0x00000000
	.4byte 0x02180000
	.4byte 0x00010000
	.4byte 0xffff00d7
	.4byte 0x00000002
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x03200000
	.4byte 0x00004000
	.4byte 0xffff00e0
	.4byte 0x00000001
	.4byte 0x02780000
	.4byte 0x00000000
	.4byte 0x02580000
	.4byte 0x00004000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x02180000
	.4byte 0x00000000
	.4byte 0x02900000
	.4byte 0x0000e000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x02a80000
	.4byte 0x00000000
	.4byte 0x02280000
	.4byte 0x00000000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x02880000
	.4byte 0x00000000
	.4byte 0x02380000
	.4byte 0x0000e000
	.4byte 0xffff00bf
	.4byte 0x00000001
	.4byte 0x02f80000
	.4byte 0x00000000
	.4byte 0x02380000
	.4byte 0x00014000
	.4byte 0xffff0072
	.4byte 0x00000001
	.4byte 0x02180000
	.4byte 0x00000000
	.4byte 0x01e80000
	.4byte 0x0001c000
	.4byte 0xffff0080
	.4byte 0x00000001
	.4byte 0x02380000
	.4byte 0x00000000
	.4byte 0x01d80000
	.4byte 0x00014000
	.4byte 0xffff00ba
	.4byte 0x00000001
	.4byte 0x02580000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x0000c000
	.4byte 0xffff00bf
	.4byte 0x00000001
	.4byte 0x02680000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020025b0
Data_020025b0:
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x02a80000
	.4byte 0x00000000
	.4byte 0x02280000
	.4byte 0x00004000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x02080000
	.4byte 0x00013000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x02580000
	.4byte 0x00000000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x02200000
	.4byte 0x00015000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x02180000
	.4byte 0x00005000
	.4byte 0xffff0072
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x02680000
	.4byte 0x00013000
	.4byte 0xffff0072
	.4byte 0x00000001
	.4byte 0x02300000
	.4byte 0x00000000
	.4byte 0x02880000
	.4byte 0x0000d000
	.4byte 0xffff0072
	.4byte 0x00000001
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x01400000
	.4byte 0x00003000
	.4byte 0xffff0080
	.4byte 0x00000001
	.4byte 0x02380000
	.4byte 0x00000000
	.4byte 0x02080000
	.4byte 0x00003000
	.4byte 0xffff0080
	.4byte 0x00000001
	.4byte 0x02f80000
	.4byte 0x00000000
	.4byte 0x02380000
	.4byte 0x00010000
	.4byte 0xffff0080
	.4byte 0x00000001
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x02580000
	.4byte 0x00015000
	.4byte 0xffff0070
	.4byte 0x00000001
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x02280000
	.4byte 0x00010000
	.4byte 0xffff0070
	.4byte 0x00000001
	.4byte 0x02d80000
	.4byte 0x00000000
	.4byte 0x02700000
	.4byte 0x00000000
	.4byte 0xffff0070
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x02500000
	.4byte 0x00000000
	.4byte 0xffff0071
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x02400000
	.4byte 0x00000000
	.4byte 0xffff0071
	.4byte .L_02009d64
	.4byte 0x02880000
	.4byte 0x00000000
	.4byte 0x01f80000
	.4byte 0x0001c000
	.4byte 0xffff0071
	.4byte 0x00000001
	.4byte 0x02c80000
	.4byte 0x00000000
	.4byte 0x02600000
	.4byte 0x00000000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
.L_0200a760:
	.2byte 0xffff
.L_0200a762:
	.2byte 0x003f
	.4byte 0x00010002
	.4byte 0x00060002
	.2byte 0xffff
.L_0200a76e:
	.2byte 0x003e
	.4byte 0x00010002
	.4byte 0x00060002
	.4byte 0x0000ffff
	.global Data_0200277c
Data_0200277c:
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte .L_0200a762
	.4byte 0x0025001b
	.4byte .L_0200a762
	.4byte 0x002b001b
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte .L_0200a762
	.4byte 0x00240015
	.4byte .L_0200a762
	.4byte 0x002e0026
	.4byte .L_0200a760
	.4byte 0x00000000
	.4byte .L_0200a76e
	.4byte 0x0021000b
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte .L_0200a76e
	.4byte 0x00090005
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte .L_0200a762
	.4byte 0x00090010
	.global Data_020027f4
Data_020027f4:
	.4byte 0x0000c602
	.4byte 0xffff0001
	.4byte Func_0200010c
	.4byte 0x0000c602
	.4byte 0xffff0002
	.4byte Func_0200010c
	.4byte 0x0000c602
	.4byte 0xffff0005
	.4byte Func_0200010c
	.4byte 0x0000c602
	.4byte 0xffff0006
	.4byte Func_0200010c
	.4byte 0x0000c602
	.4byte 0xffff0008
	.4byte Func_0200010c
	.4byte 0x0000c602
	.4byte 0xffff000c
	.4byte Func_0200010c
	.4byte 0x0000c602
	.4byte 0xffff000e
	.4byte Func_0200010c
	.4byte 0x0000c602
	.4byte 0xffff0007
	.4byte Func_0200010c
	.4byte 0x0000c401
	.4byte 0xffff0061
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x000001f3
	.4byte 0xffff00c8
	.2byte MsgItLooksLikeThisWell
	.2byte 0x0040
	.4byte 0x50009085
	.4byte 0xffff0000
	.4byte Func_02000760
	.4byte 0x40009085
	.4byte 0xffff0000
	.4byte Func_02000774
	.4byte 0x0000c400
	.4byte 0xffff0008
	.4byte Func_02000124
	.4byte 0x0000c400
	.4byte 0xffff0009
	.4byte Func_02000130
	.4byte 0x00000002
	.4byte 0xffff0012
	.4byte Func_02000254
	.4byte 0x00000002
	.4byte 0xffff0013
	.4byte Func_02000260
	.4byte 0x00000002
	.4byte 0x08b30014
	.4byte Func_02000aa0
	.4byte 0x00000002
	.4byte 0x08b30015
	.4byte Func_02000aa0
	.4byte 0x00000002
	.4byte 0x08b40016
	.4byte Func_02000248
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte MsgWeWereHitByA
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte MsgLosingAllMyMostExpensive
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte MsgThatTidalWaveMayWell
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte MsgItDoesnTMatterHow
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte MsgWelcomeToAlhafraThisIs
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte MsgAMassiveTidalWaveHit
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte Func_0200013c
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte MsgRightAfterMyBoatGot
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte MsgThisGuySProbablyWondering
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte MsgLosingOurBoatsWasAwful
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte MsgIJustCanTBelieve
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte MsgThoseGuysLookingForA
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte MsgThoseMenDidnTLook
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte MsgThosePeopleThatJustCame
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte MsgWeVeHadALot
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte MsgOurPortWasRuinedBy
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte MsgTheOnlyBoatSparedBy
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte Func_020001a4
	.4byte 0x00000000
	.4byte 0xffff0017
	.4byte MsgThatBoatAtTheWharf
	.4byte 0x00000000
	.4byte 0xffff0018
	.4byte MsgWhyWouldTheMayorGo
	.4byte 0x00000000
	.4byte 0xffff0019
	.4byte MsgBlank
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte MsgIfThisGuySLooking
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte MsgWhoWouldComeToAlhafra
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte MsgThisTidalWaveSActually
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte MsgIDonTKnowWhy
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte MsgIWonderIfMyPartner
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte MsgWeCanTRaiseThe
	.4byte 0x00008d15
	.4byte 0xffff0017
	.4byte MsgIStillCanTBelieve
	.4byte 0x00008d15
	.4byte 0xffff0018
	.4byte MsgItMayBeNewfangledBut
	.4byte 0x00008d15
	.4byte 0xffff0419
	.4byte Func_0200023c
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte Func_020001f0
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte MsgListenYouLookLikeAn
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte MsgIHateMyJobI
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte MsgMyPartnerSAlwaysIn
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte MsgNoOneIsAllowedBeyond
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte MsgTheMayorSaidNoOne
	.4byte 0x00001815
	.4byte 0x0200001a
	.4byte Func_0200026c
	.4byte 0x00001815
	.4byte 0x0201001b
	.4byte Func_0200026c
	.4byte 0x00001815
	.4byte 0x0202001c
	.4byte Func_0200026c
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02002ad0
Data_02002ad0:
	.4byte 0x0000c602
	.4byte 0xffff0001
	.4byte Func_0200010c
	.4byte 0x0000c602
	.4byte 0xffff0002
	.4byte Func_0200010c
	.4byte 0x0000c602
	.4byte 0xffff0005
	.4byte Func_0200010c
	.4byte 0x0000c602
	.4byte 0xffff0006
	.4byte Func_0200010c
	.4byte 0x0000c602
	.4byte 0xffff0008
	.4byte Func_0200010c
	.4byte 0x0000c602
	.4byte 0xffff000c
	.4byte Func_0200010c
	.4byte 0x0000c602
	.4byte 0xffff000e
	.4byte Func_0200010c
	.4byte 0x0000c602
	.4byte 0xffff0007
	.4byte Func_0200010c
	.4byte 0x0000c401
	.4byte 0xffff0061
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x000001f3
	.4byte 0xffff00c8
	.2byte MsgItLooksLikeThisWell
	.2byte 0x0040
	.4byte 0x50009085
	.4byte 0xffff0000
	.4byte Func_02000760
	.4byte 0x40009085
	.4byte 0xffff0000
	.4byte Func_02000774
	.4byte 0x0000c400
	.4byte 0xffff0008
	.4byte Func_02000124
	.4byte 0x0000c400
	.4byte 0xffff0009
	.4byte Func_02000130
	.4byte 0x00000002
	.4byte 0x08b70019
	.4byte Func_0200047c
	.4byte 0x00000000
	.4byte 0x18ff0012
	.4byte MsgItLooksLikeTheMayor
	.4byte 0x00008d15
	.4byte 0x18ff0012
	.4byte MsgIGuessOurMayorDidn
	.4byte 0x00000000
	.4byte 0x18ff0011
	.4byte MsgIHearTheMayorOf
	.4byte 0x00008d15
	.4byte 0x18ff0011
	.4byte MsgHeSeemsLikeAnHonest
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte MsgWhatBriggsWasTheLeader
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte MsgBriggsWasVeryGenerousHe
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte MsgThatGroupFromMadraWas
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte MsgGettingABoatForNothing
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte MsgWeReAllGoingTo
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte MsgOnceThingsAreBackTo
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte MsgSoTheGuysWhoBought
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte MsgEveryoneIsTalkingAboutHow
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte MsgTheMayorIsTheOnly
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte MsgTheMayorOfMadraApparently
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte MsgINeverThoughtAFine
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte MsgICanHelpBuildHouses
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte MsgWeVePutOffFixing
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte MsgOnceRepairsAreDoneHere
	.4byte 0x00000000
	.4byte 0xffff0017
	.4byte MsgTheyToldMeTheyD
	.4byte 0x00000000
	.4byte 0xffff0018
	.4byte Func_02000288
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte MsgItSSoNiceOf
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte MsgEveryoneInTownIsSo
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte MsgYouKnowPiratesWhoPay
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte MsgThosePiratesHaveTheCoolest
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte MsgICanTBelieveThey
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte MsgIfOurMayorThinksI
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte MsgYouKnowSittingHereFixing
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte MsgHelpingPeopleOutFeelsReally
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte MsgTheFactThatMadraTook
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte MsgIWonderIfTheCrew
	.4byte 0x00008d15
	.4byte 0xffff0017
	.4byte MsgItSStrangeTheQuarters
	.4byte 0x00008d15
	.4byte 0xffff0018
	.4byte MsgWorkingOnDeckWithThe
	.4byte 0x00000000
	.4byte 0xffff0019
	.4byte MsgOnceTheyFixTheOld
	.4byte 0x00000000
	.4byte 0xffff001a
	.4byte MsgIWasAfraidMyHouse
	.4byte 0x00000000
	.4byte 0xffff001b
	.4byte Func_02000398
	.4byte 0x00008d15
	.4byte 0xffff0019
	.4byte MsgIfWeDonTGet
	.4byte 0x00008d15
	.4byte 0xffff001a
	.4byte MsgYouKnowSinceWeRe
	.4byte 0x00008d15
	.4byte 0x08b5001b
	.4byte MsgIWishSomeoneWouldJust
	.4byte 0x00008d15
	.4byte 0xffff001b
	.4byte MsgIWonderIfIShould
	.4byte 0x0001c914
	.4byte 0xffff001b
	.4byte Func_020002f8
	.4byte 0x00000000
	.4byte 0xffff001c
	.4byte MsgAllMyNeighborsAreGoing
	.4byte 0x00000000
	.4byte 0xffff001d
	.4byte MsgWhatAStrokeOfLuck
	.4byte 0x00000000
	.4byte 0xffff001e
	.4byte MsgMyDadLostTheDraw
	.4byte 0x00000000
	.4byte 0xffff001f
	.4byte MsgIWonderIfThereS
	.4byte 0x00008d15
	.4byte 0xffff001c
	.4byte MsgLosingTheLotteryAndBeing
	.4byte 0x00008d15
	.4byte 0xffff001d
	.4byte MsgIThoughtTheHardPart
	.4byte 0x00008d15
	.4byte 0xffff001e
	.4byte MsgAllBecauseOfMyDad
	.4byte 0x00008d15
	.4byte 0xffff001f
	.4byte MsgIReallyShouldnTBe
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte MsgAhWelcomeWelcomeListenSorry
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte MsgOhGoOnInThe
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte MsgIfIDKnownBriggs
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte MsgTheMayorWasReallyImpressed
	.4byte 0x00000000
	.4byte 0x08b6000c
	.4byte MsgNoOneIsAllowedBeyond2
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte Func_0200066c
	.4byte 0x00008d15
	.4byte 0x08b9000c
	.4byte MsgTheMayorSaidNoOne2
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte MsgNotThatItMattersThe
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02002e48
Data_02002e48:
	.4byte 0x0000c602
	.4byte 0xffff0001
	.4byte Func_0200010c
	.4byte 0x0000c602
	.4byte 0xffff0002
	.4byte Func_0200010c
	.4byte 0x0000c602
	.4byte 0xffff0005
	.4byte Func_0200010c
	.4byte 0x0000c602
	.4byte 0xffff0006
	.4byte Func_0200010c
	.4byte 0x0000c602
	.4byte 0xffff0008
	.4byte Func_0200010c
	.4byte 0x0000c602
	.4byte 0xffff000c
	.4byte Func_0200010c
	.4byte 0x0000c602
	.4byte 0xffff000e
	.4byte Func_0200010c
	.4byte 0x0000c602
	.4byte 0xffff0007
	.4byte Func_0200010c
	.4byte 0x0000c401
	.4byte 0xffff0061
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x000001f3
	.4byte 0xffff00c8
	.2byte MsgItLooksLikeThisWell
	.2byte 0x0040
	.4byte 0x50009085
	.4byte 0xffff0000
	.4byte Func_02000760
	.4byte 0x40009085
	.4byte 0xffff0000
	.4byte Func_02000774
	.4byte 0x0000c400
	.4byte 0xffff0008
	.4byte Func_02000124
	.4byte 0x0000c400
	.4byte 0xffff0009
	.4byte Func_02000130
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte MsgEveryoneSSayingThatBriggs
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte MsgSupposedlyTheCaveUnderThe
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte MsgSoBriggsStoleMadraS
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte MsgIDonTKnowI
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte MsgAlhafraLostItsShipNow
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte MsgEvenThoughWeLostThe
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte MsgAfterWeDidAllThat
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte MsgItWasGreatToSee
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte MsgWhenTheyFindThatTheir
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte MsgItSNotLikeThe
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte MsgTheRepairsAreComingAlong
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte MsgWhenIsThisHouseGoing
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte MsgItFeltSoGreatWhen
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte MsgBackBeforeBriggsDestroyedThe
	.4byte 0x00000000
	.4byte 0xffff0017
	.4byte MsgWeFixedTheMastSo
	.4byte 0x00000000
	.4byte 0xffff0018
	.4byte MsgIfTheyCaughtAGood
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte MsgIfOnlyWeHadThat
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte MsgIHopeTradeDoesnT
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte MsgItWasMadraSBoat
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte MsgTheMayorSeemedSoHappy
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte MsgTheMayorOfMadraSeems
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte MsgThereSNoPointIn
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte MsgWeReAllPullingTogether
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte MsgIfMomWouldStopMaking
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte MsgIKnowItSTime
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte MsgFirstHeStealsAShip
	.4byte 0x00008d15
	.4byte 0xffff0017
	.4byte MsgWhyWorryAboutBriggsIt
	.4byte 0x00008d15
	.4byte 0xffff0018
	.4byte MsgIfBriggsHadnTStolen
	.4byte 0x00000000
	.4byte 0xffff0019
	.4byte MsgThatGuySHouseIsn
	.4byte 0x00000000
	.4byte 0xffff001a
	.4byte MsgTheMayorSBeenHanding
	.4byte 0x00000000
	.4byte 0xffff001b
	.4byte MsgWeHaveToStoreWhat
	.4byte 0x00008d15
	.4byte 0xffff0019
	.4byte MsgWeCouldUseSomeHelp
	.4byte 0x00008d15
	.4byte 0xffff001a
	.4byte MsgIDonTKnowWhat
	.4byte 0x00008d15
	.4byte 0xffff001b
	.4byte MsgIVeSeenFatherTrying
	.4byte 0x00000000
	.4byte 0xffff001c
	.4byte MsgEvenIfWeFixIt
	.4byte 0x00000000
	.4byte 0xffff001d
	.4byte MsgWithEveryoneSHelpWe
	.4byte 0x00000000
	.4byte 0xffff001f
	.4byte MsgMomWasHappyToHear
	.4byte 0x00000000
	.4byte 0xffff001e
	.4byte MsgThereAreALotOf
	.4byte 0x00008d15
	.4byte 0xffff001c
	.4byte MsgThisIsJustTooMuch
	.4byte 0x00008d15
	.4byte 0xffff001d
	.4byte MsgHeSNeverGoingTo
	.4byte 0x00008d15
	.4byte 0xffff001f
	.4byte MsgThereSNoWayDad
	.4byte 0x00008d15
	.4byte 0xffff001e
	.4byte MsgCarryingTheseThingsAllOver
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte MsgTheMayorOfAlhafraDoes
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte MsgYouReallyMadeAMess
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte MsgTheMayorWasSoMad
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte MsgOneThingSForSure
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte MsgBriggsCameThroughHereAnd
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte MsgThePoorGuardWasBlindfolded
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02003154
Data_02003154:
	.4byte 0x0000c602
	.4byte 0xffff0001
	.4byte Func_0200010c
	.4byte 0x0000c602
	.4byte 0xffff0002
	.4byte Func_0200010c
	.4byte 0x0000c602
	.4byte 0xffff0005
	.4byte Func_0200010c
	.4byte 0x0000c602
	.4byte 0xffff0006
	.4byte Func_0200010c
	.4byte 0x0000c602
	.4byte 0xffff0008
	.4byte Func_0200010c
	.4byte 0x0000c602
	.4byte 0xffff000c
	.4byte Func_0200010c
	.4byte 0x0000c602
	.4byte 0xffff000e
	.4byte Func_0200010c
	.4byte 0x0000c602
	.4byte 0xffff0007
	.4byte Func_0200010c
	.4byte 0x0000c401
	.4byte 0xffff0061
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x000001f3
	.4byte 0xffff00c8
	.2byte MsgItLooksLikeThisWell
	.2byte 0x0040
	.4byte 0x50009085
	.4byte 0xffff0000
	.4byte Func_02000760
	.4byte 0x40009085
	.4byte 0xffff0000
	.4byte Func_02000774
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte MsgTheWholeTownIsGrateful
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte MsgIMJustStandingGuard
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte MsgOurSailboatLooksFabulousWith
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte MsgIWonderWhereBriggsAnd
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte MsgIHeardYouMadeIt
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte MsgEverSinceWeHeardBriggs
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte MsgIDonTKnowWhy2
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte MsgOhItSYouWhere
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte MsgWithThatShipAlhafraS
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte MsgOnceThatStoneWasTaken
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte MsgIfBriggsIsSmartHe
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte MsgIVeBeenWaitingFor
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte MsgTheSecretToHarnessingThe
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte MsgIMSureItS
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte MsgTheMayorWasActuallySinging
	.4byte 0x00000000
	.4byte 0xffff0017
	.4byte MsgWeirdHaveYouEverHad
	.4byte 0x00000000
	.4byte 0xffff0018
	.4byte MsgHugeBoatsCarryMoreBut
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte MsgNowThatTheBoatIs
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte MsgIMSupposedToBe
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte MsgWhoCaresIfTheBoat
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte MsgWhyDoIHaveTo
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte MsgIHateToTellBut
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte MsgIDBetThatBriggs
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte MsgBriggsPaidALotFor
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte MsgMustHaveFoundHimselfA
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte MsgIMCertainTheOnly
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte MsgNowThatAllTheDebris
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte MsgTheMayorSaidIHad
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte MsgIMProbablyTooOld
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte MsgSmallerShipsUseTheirSails
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte MsgItSBeenSoBusy
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte MsgTheMayorOnlyHadOne
	.4byte 0x00008d15
	.4byte 0xffff0017
	.4byte MsgWhatIfThePiratesWere
	.4byte 0x00008d15
	.4byte 0xffff0018
	.4byte MsgWithoutSmallerShipsHowCan
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020033a0
Data_020033a0:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xfff80000
	.4byte 0x00000001
	.4byte 0x00000005
	.4byte 0x00200000
	.4byte 0x00000000
	.4byte 0xffe00000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000005
	.4byte 0x00000011
	.global Data_020033f8
Data_020033f8:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x02280000
	.4byte 0x00000000
	.4byte 0x03280000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x02180000
	.4byte 0x00000000
	.4byte 0x02f80000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000011
	.global Data_02003434
Data_02003434:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x03200000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x02f80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x02f80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x02a80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x02a80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000d000
	.4byte 0x00000011
	.global Data_020034cc
Data_020034cc:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01a80000
	.4byte 0x00000000
	.4byte 0x03180000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x02f80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x02a80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x02a80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000d000
	.4byte 0x00000011
	.global Data_02003550
Data_02003550:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x03280000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x03200000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x02f80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x02a80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x02a80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000d000
	.4byte 0x00000011
	.global Data_020035e8
Data_020035e8:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x03280000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x03200000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x02f80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x02a80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x02a80000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000d000
	.4byte 0x00000011
	.global Data_0200366c
Data_0200366c:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x02f80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x02a80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x02a80000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000d000
	.4byte 0x00000011
	.global Data_020036c8
Data_020036c8:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x02f80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x02a80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x02a80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000d000
	.4byte 0x00000011
	.global Data_0200374c
Data_0200374c:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000005
	.4byte 0x00100000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x02f80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x02f80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x02a80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x02a80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000d000
	.4byte 0x00000011
	.global Data_020037f8
Data_020037f8:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x02f80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x02f80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x02a80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x02a80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01000000
	.4byte 0x00000000
	.4byte 0x02c80000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000d000
	.4byte 0x00000011
	.global Data_02003890
Data_02003890:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xfff00000
	.4byte 0x00000001
	.4byte 0x00000005
	.4byte 0xfff00000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000005
	.4byte 0xfff00000
	.4byte 0x00000000
	.4byte 0x00100000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x03280000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000d000
	.4byte 0x00000011
	.global Data_02003914
Data_02003914:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x03280000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x03280000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000d000
	.4byte 0x00000011
	.global Data_02003970
Data_02003970:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x03280000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x03280000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000d000
	.4byte 0x00000011
	.global Data_020039cc
Data_020039cc:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x03280000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x03280000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x02a80000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000d000
	.4byte 0x00000011
	.global Data_02003a28
Data_02003a28:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00100000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x03280000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x03280000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000d000
	.4byte 0x00000011
	.global Data_02003a98
Data_02003a98:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00100000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x03280000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x03280000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x02a80000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000d000
	.4byte 0x00000011
	.global Data_02003b08
Data_02003b08:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00200000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x03280000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x03280000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000d000
	.4byte 0x00000011
	.global Data_02003b78
Data_02003b78:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000005
	.4byte 0xffec0000
	.4byte 0x00000000
	.4byte 0x00200000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x03280000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01000000
	.4byte 0x00000000
	.4byte 0x03280000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01000000
	.4byte 0x00000000
	.4byte 0x02c80000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000d000
	.4byte 0x00000011
