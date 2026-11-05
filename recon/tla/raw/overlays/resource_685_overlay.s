.syntax unified
	.thumb
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
	bl wait
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
	bl Actor_GetById
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
	bl Actor_SetAngleToward
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
	bl Actor_ArmCallback
.L_0200911c:
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, pc}
.L_02009124:
	.4byte gMainRoutineWork
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
	bl Actor_GetById
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
	bl Actor_SetPositionAndReset
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
	bl FieldEvent_Wait
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
	bl Actor_ArmCallback
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
	bl FieldEvent_Wait
	movs r0, #11
	movs r1, #6
	movs r2, #15
	bl Actor_Jump
	movs r0, #11
	movs r1, #6
	movs r2, #23
	bl Actor_Jump
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
	bl FieldEvent_Wait
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
	bl FieldEvent_Wait
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
	bl FieldEvent_Wait
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
	bl FieldEvent_Wait
	movs r1, #176
	lsls r1, r1, #8
	movs r0, #11
	bl Func_02003534
	movs r0, #10
	bl FieldEvent_Wait
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
	bl FieldEvent_Wait
	movs r1, #2
	movs r0, #11
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl FieldEvent_Wait
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
	bl FieldEvent_Wait
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
	bl FieldEvent_Wait
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
	bl FieldEvent_Wait
	movs r2, #5
	movs r0, #8
	movs r1, #0
	bl Func_0200351c
	movs r1, #176
	lsls r1, r1, #8
	movs r0, #11
	bl Func_02003534
	movs r0, #10
	bl FieldEvent_Wait
	movs r1, #4
	movs r0, #8
	bl Motion_SetModeAndWaitAnimation
	movs r0, #5
	bl FieldEvent_Wait
	movs r1, #128
	lsls r1, r1, #8
	movs r0, #8
	bl Func_02003534
	movs r0, #5
	bl FieldEvent_Wait
	movs r2, #5
	movs r0, #8
	movs r1, #0
	bl Func_0200351c
	movs r1, #3
	movs r0, #11
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl FieldEvent_Wait
	movs r1, #3
	movs r0, #11
	bl Motion_SetModeAndWaitAnimation
	movs r0, #5
	bl FieldEvent_Wait
	movs r0, #11
	movs r1, #0
	movs r2, #5
	bl Func_0200351c
	movs r1, #128
	movs r0, #11
	lsls r1, r1, #8
	bl Func_02003534
	movs r0, #5
	bl FieldEvent_Wait
.L_02009466:
	ldr r0, .L_02009550
	bl Func_0200350c
	movs r1, #176
	lsls r1, r1, #8
	movs r0, #8
	bl Func_02003534
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #8
	bl Actor_GetById
	movs r3, #0
	mov r10, r3
	movs r3, #160
	lsls r3, r3, #7
	strh r3, [r0, #6]
	movs r1, #5
	movs r0, #8
	bl Actor_SetModeById
	movs r0, #30
	bl FieldEvent_Wait
	movs r0, #8
	movs r1, #6
	bl Actor_SetModeById
	movs r1, #181
	movs r2, #252
	lsls r2, r2, #16
	lsls r1, r1, #18
	movs r0, #12
	bl Func_020034d4
	movs r0, #12
	bl Actor_GetById
	ldr r5, .L_02009554
	str r5, [r0, #24]
	movs r0, #12
	bl Actor_GetById
	str r5, [r0, #28]
	movs r0, #12
	bl Actor_GetById
	movs r1, #0
	bl Actor_SetSingleChildFlags
	movs r0, #128
	lsls r0, r0, #6
	movs r1, #0
	adds r0, #8
	bl FieldMessage_ShowAtActor
	movs r0, #4
	movs r1, #0
	bl Event_AskYesNoWithActorAnimation
	cmp r0, #0
	bne .L_02009558
	bl Func_020035bc
	movs r0, #15
	bl FieldEvent_Wait
	movs r2, #0
	movs r0, #12
	movs r1, #0
	bl Func_020034d4
	movs r1, #1
	movs r0, #8
	bl Actor_SetModeById
	movs r0, #8
	bl Actor_GetById
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
	bl Actor_CommitPositionAndActivate
	movs r0, #5
	bl FieldEvent_Wait
	b .L_02009746
	.2byte 0x0000
.L_02009544:
	.4byte gMainRoutineWork
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
	bl FieldEvent_Wait
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
	bl Actor_Jump
	movs r0, #146
	bl Func_0200362c
	movs r0, #12
	movs r1, #1
	bl Actor_SetSpritePriority
	movs r1, #192
	movs r2, #192
	movs r0, #12
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl Actor_SetSpeedParameters
	movs r1, #161
	lsls r1, r1, #2
	movs r2, #248
	movs r0, #12
	bl Actor_ResetAndSetPositionInMode2
	movs r0, #21
	bl FieldEvent_Wait
	movs r1, #0
	movs r2, #0
	movs r0, #12
	bl Func_020034d4
	movs r0, #50
	bl FieldEvent_Wait
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #10
	lsls r2, r2, #9
	lsls r0, r0, #10
	bl Func_02003434
	movs r0, #70
	bl FieldEvent_Wait
	movs r0, #148
	bl Func_0200362c
	movs r0, #25
	bl FieldEvent_Wait
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
	bl FieldEvent_Wait
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
	bl Actor_Jump
	movs r0, #147
	bl Func_0200362c
	movs r0, #13
	ldr r1, .L_02009764
	ldr r2, .L_02009768
	bl Actor_SetSpeedParameters
	movs r1, #146
	movs r2, #248
	movs r0, #13
	lsls r1, r1, #2
	bl Actor_SetPositionAndReset
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
	.4byte gMainRoutineWork
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
