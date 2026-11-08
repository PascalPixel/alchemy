.syntax unified
	.thumb
	.section .text.x02009e34,"ax",%progbits
	.balign 4
	.global Func_02001e34
	.thumb_func
Func_02001e34:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r0, #9
	sub sp, #4
	bl Actor_GetById
	ldr r3, .L_0200a08c
	movs r1, #133
	lsls r1, r1, #2
	adds r5, r3, r1
	adds r6, r0, #0
	ldr r0, [r5]
	bl Actor_GetById
	ldr r2, .L_0200a090
	ldr r3, [r0, #8]
	ldr r1, .L_0200a094
	adds r3, r3, r2
	lsrs r2, r3, #31
	adds r3, r3, r2
	mov r9, r1
	asrs r3, r3, #1
	add r3, r9
	str r3, [sp, #0]
	ldr r2, .L_0200a098
	ldr r3, [r0, #16]
	ldr r1, .L_0200a09c
	adds r3, r3, r2
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	mov r8, r1
	movs r0, #183
	add r3, r8
	lsls r0, r0, #1
	mov r11, r3
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009e90
	b .L_0200a368
.L_02009e90:
	ldr r3, .L_0200a0a0
	movs r2, #64
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	beq .L_02009ea4
	movs r0, #118
	adds r0, #255
	bl GameFlag_SetBit
.L_02009ea4:
	bl Func_02004c20
	movs r0, #0
	bl Func_02004db8
	bl Event_StartScreenTransition
	bl Event_WaitTransitionFrames
	movs r0, #0
	bl Party_RemoveActiveOwner
	movs r0, #1
	bl Party_RemoveActiveOwner
	movs r0, #2
	bl Party_RemoveActiveOwner
	movs r0, #3
	bl Party_RemoveActiveOwner
	movs r0, #4
	bl Party_RemoveActiveOwner
	movs r0, #5
	bl Party_RemoveActiveOwner
	movs r0, #6
	bl Party_RemoveActiveOwner
	movs r0, #7
	bl Party_RemoveActiveOwner
	movs r0, #4
	bl Party_AddActiveOwner
	movs r0, #5
	bl Party_AddActiveOwner
	movs r0, #6
	bl Party_AddActiveOwner
	movs r0, #1
	ldr r7, [r5]
	bl Func_02004bf8
	movs r0, #183
	lsls r0, r0, #1
	bl GameFlag_SetBit
	bl Func_02004c20
	movs r0, #0
	bl Func_02004db8
	movs r1, #0
	movs r2, #7
	adds r0, r7, #0
	bl Djinn_AddToOwner
	movs r1, #0
	movs r2, #7
	adds r0, r7, #0
	bl DjinnRecovery_AddStandby
	bl Func_02004da8
	movs r1, #9
	movs r2, #0
	adds r0, r7, #0
	bl Actor_SetAngleToward
	movs r0, #10
	bl FieldEvent_Wait
	movs r1, #2
	adds r1, #255
	movs r2, #60
	adds r0, r7, #0
	bl Func_02004d08
	adds r2, r6, #0
	movs r3, #1
	adds r2, #102
	strh r3, [r2]
	adds r1, r7, #0
	movs r2, #0
	movs r0, #9
	bl Actor_SetAngleToward
	movs r0, #16
	bl wait
	movs r0, #118
	adds r0, #255
	bl GameFlag_Test
	ldr r2, .L_0200a0a4
	ldr r3, .L_0200a0a8
	movs r5, #15
	subs r3, r3, r2
	muls r0, r3
	ldr r3, .L_0200a0ac
	adds r0, r0, r3
	bl Func_02004cd8
	movs r0, #9
	movs r1, #0
	bl Func_02004cf0
	bl Func_02004db0
	ldr r0, .L_0200a0b0
	movs r1, #6
	bl Func_02004df8
	bl Func_02004e00
	bl Func_02004da8
	movs r2, #85
	adds r2, r2, r6
	movs r3, #2
	strb r3, [r2]
	movs r3, #128
	lsls r3, r3, #7
	str r3, [r6, #72]
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r6, #48]
	str r3, [r6, #52]
	movs r3, #0
	str r3, [r6, #40]
	str r3, [r6, #20]
	mov r10, r2
	adds r0, r6, #0
	mov r1, r9
	movs r2, #0
	mov r3, r8
	bl Func_02004b50
.L_02009fbe:
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
	bl wait
	cmp r5, #0
	bge .L_02009fbe
	movs r0, #9
	adds r1, r7, #0
	movs r2, #0
	bl Actor_SetAngleToward
	movs r2, #0
	movs r1, #9
	adds r0, r7, #0
	bl Actor_SetAngleToward
	movs r0, #16
	bl wait
	movs r3, #0
	str r3, [r6, #108]
	adds r0, r6, #0
	movs r1, #0
	bl Animation_ApplyChildValues
	movs r3, #128
	lsls r3, r3, #9
	movs r1, #0
	str r3, [r6, #72]
	movs r0, #9
	bl FieldMessage_ShowAtActor
	adds r0, r7, #0
	movs r1, #0
	bl Event_AskYesNoWithActorAnimation
	cmp r0, #1
	bne .L_0200a0b8
	movs r0, #9
	movs r1, #2
	movs r2, #20
	bl Actor_Jump
	movs r1, #2
	movs r2, #20
	movs r0, #9
	bl Actor_Jump
	movs r0, #118
	adds r0, #255
	bl GameFlag_Test
	ldr r2, .L_0200a0a8
	ldr r3, .L_0200a0a4
	ldr r1, .L_0200a0b4
	subs r5, r2, r3
	muls r0, r5
	mov r8, r1
	add r0, r8
	bl Func_02004cd8
	movs r1, #0
	movs r0, #9
	bl FieldMessage_ShowAtActor
	adds r0, r7, #0
	movs r1, #0
	bl Event_AskYesNoWithActorAnimation
	cmp r0, #1
	bne .L_0200a06c
	movs r0, #118
	adds r0, #255
	bl GameFlag_Test
	muls r0, r5
	mov r3, r8
	adds r3, #1
	b .L_0200a07a
.L_0200a06c:
	movs r0, #118
	adds r0, #255
	bl GameFlag_Test
	muls r0, r5
	mov r3, r8
	adds r3, #2
.L_0200a07a:
	adds r0, r0, r3
	bl Func_02004cd8
	movs r0, #9
	movs r1, #0
	bl Func_02004cf0
	b .L_0200a0d8
	.2byte 0x0000
.L_0200a08c:
	.4byte gMainRoutineWork
.L_0200a090:
	.4byte 0xd8780000
.L_0200a094:
	.4byte 0x27880000
.L_0200a098:
	.4byte 0xdf5c0000
.L_0200a09c:
	.4byte 0x20a40000
.L_0200a0a0:
	.4byte gInput
.L_0200a0a4:
	.4byte MsgDjinnGiveTutorialA
.L_0200a0a8:
	.4byte MsgDjinnGiveTutorialB
.L_0200a0ac:
	.4byte MsgHey2
.L_0200a0b0:
	.4byte 0x00013333
.L_0200a0b4:
	.4byte MsgWellYouWeren
.L_0200a0b8:
	movs r0, #118
	adds r0, #255
	bl GameFlag_Test
	ldr r3, .L_0200a348
	ldr r2, .L_0200a34c
	subs r3, r3, r2
	muls r0, r3
	ldr r3, .L_0200a350
	adds r0, r0, r3
	bl Func_02004cd8
	movs r0, #9
	movs r1, #0
	bl Func_02004cf0
.L_0200a0d8:
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #30
	adds r0, r7, #0
	bl Func_02004d08
	movs r0, #9
	movs r1, #2
	movs r2, #20
	bl Actor_Jump
	movs r1, #2
	movs r2, #20
	movs r0, #9
	bl Actor_Jump
	movs r0, #118
	adds r0, #255
	bl GameFlag_Test
	ldr r2, .L_0200a34c
	ldr r3, .L_0200a348
	movs r5, #15
	subs r3, r3, r2
	muls r0, r3
	ldr r3, .L_0200a354
	adds r0, r0, r3
	bl Func_02004cd8
	movs r0, #9
	movs r1, #0
	bl Func_02004cf0
	movs r1, #129
	movs r2, #30
	lsls r1, r1, #1
	adds r0, r7, #0
	bl Func_02004d08
	movs r0, #9
	movs r1, #0
	bl Func_02004cf0
	movs r1, #128
	movs r2, #30
	lsls r1, r1, #1
	adds r0, r7, #0
	bl Func_02004d08
	movs r0, #9
	movs r1, #0
	bl Func_02004cf0
	adds r0, r7, #0
	movs r1, #2
	bl Motion_SetVarCbAndRefresh
	movs r0, #9
	movs r1, #0
	bl Func_02004cf0
	movs r2, #30
	movs r0, #9
	movs r1, #2
	bl Actor_Jump
	movs r0, #9
	movs r1, #0
	bl Func_02004cf0
	movs r3, #0
	mov r2, r10
	strb r3, [r2]
	movs r2, #128
	adds r0, r6, #0
	ldr r1, [sp, #0]
	lsls r2, r2, #13
	mov r3, r11
	bl Func_02004b50
.L_0200a178:
	ldrh r3, [r6, #6]
	movs r1, #128
	lsls r1, r1, #5
	adds r3, r3, r1
	strh r3, [r6, #6]
	movs r0, #1
	subs r5, #1
	bl wait
	cmp r5, #0
	bge .L_0200a178
	adds r0, r7, #0
	movs r1, #1
	bl Actor_SetModeById
	movs r1, #0
	movs r0, #9
	bl Func_02004cf0
	movs r2, #0
	movs r3, #2
	mov r1, r10
	strb r3, [r1]
	str r2, [r6, #40]
	str r2, [r6, #20]
	movs r5, #7
.L_0200a1ac:
	ldrh r3, [r6, #6]
	movs r2, #128
	lsls r2, r2, #5
	adds r3, r3, r2
	strh r3, [r6, #6]
	movs r0, #1
	subs r5, #1
	bl wait
	cmp r5, #0
	bge .L_0200a1ac
	adds r0, r7, #0
	movs r1, #22
	bl Actor_SetModeById
	movs r0, #9
	movs r1, #0
	bl Func_02004cf0
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #9
	bl Func_02004d08
	movs r2, #0
	movs r0, #9
	adds r1, r7, #0
	bl Actor_SetAngleToward
	movs r0, #9
	movs r1, #2
	bl Motion_SetVarCbAndRefresh
	movs r0, #9
	movs r1, #0
	bl Func_02004cf0
	movs r0, #9
	movs r1, #2
	movs r2, #30
	bl Actor_Jump
	movs r0, #9
	movs r1, #0
	bl FieldMessage_ShowAtActor
	movs r6, #0
.L_0200a20c:
	adds r0, r7, #0
	movs r1, #0
	bl Event_AskYesNoWithActorAnimation
	adds r5, r0, #0
	cmp r6, #7
	bne .L_0200a254
	cmp r5, #0
	bne .L_0200a292
	movs r0, #9
	movs r1, #2
	movs r2, #20
	bl Actor_Jump
	movs r1, #2
	movs r2, #20
	movs r0, #9
	bl Actor_Jump
	movs r0, #118
	adds r0, #255
	bl GameFlag_Test
	ldr r3, .L_0200a348
	ldr r2, .L_0200a34c
	subs r3, r3, r2
	muls r0, r3
	ldr r3, .L_0200a358
	adds r0, r0, r3
	bl Func_02004cd8
	movs r0, #9
	movs r1, #0
	bl Func_02004cf0
	b .L_0200a2ce
.L_0200a254:
	cmp r5, #1
	bne .L_0200a292
	movs r0, #9
	movs r1, #2
	movs r2, #20
	bl Actor_Jump
	movs r1, #2
	movs r2, #20
	movs r0, #9
	bl Actor_Jump
	movs r0, #118
	adds r0, #255
	bl GameFlag_Test
	ldr r2, .L_0200a34c
	ldr r3, .L_0200a348
	subs r3, r3, r2
	muls r0, r3
	ldr r3, .L_0200a35c
	adds r0, r6, r0
	adds r0, r0, r3
	bl Func_02004cd8
	movs r0, #9
	movs r1, #0
	bl FieldMessage_ShowAtActor
	adds r6, #1
	b .L_0200a20c
.L_0200a292:
	adds r0, r7, #0
	movs r1, #22
	bl Actor_SetModeById
	movs r0, #9
	movs r1, #2
	movs r2, #20
	bl Actor_Jump
	movs r1, #4
	movs r2, #20
	movs r0, #9
	bl Actor_Jump
	movs r0, #118
	adds r0, #255
	bl GameFlag_Test
	ldr r3, .L_0200a348
	ldr r2, .L_0200a34c
	subs r3, r3, r2
	muls r0, r3
	ldr r3, .L_0200a360
	adds r0, r0, r3
	bl Func_02004cd8
	movs r0, #9
	movs r1, #0
	bl Func_02004cf0
.L_0200a2ce:
	movs r0, #52
	movs r1, #4
	adds r0, #255
	bl Func_02004ba8
	movs r0, #81
	bl Func_02004e30
	movs r0, #118
	adds r0, #255
	bl GameFlag_Test
	ldr r3, .L_0200a34c
	ldr r6, .L_0200a348
	ldr r5, .L_0200a364
	subs r6, r6, r3
	muls r0, r6
	movs r1, #3
	adds r0, r0, r5
	bl FieldMessage_Show
	movs r0, #118
	adds r0, #255
	bl GameFlag_Test
	muls r0, r6
	adds r5, #1
	adds r0, r0, r5
	bl Func_02004cd8
	movs r0, #9
	movs r1, #2
	movs r2, #20
	bl Actor_Jump
	movs r1, #0
	movs r0, #9
	bl FieldMessage_ShowAtActor
	adds r0, r7, #0
	movs r1, #0
	bl Event_AskYesNoWithActorAnimation
	cmp r0, #1
	bne .L_0200a32a
	b .L_0200a4c8
.L_0200a32a:
	movs r0, #118
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a33e
	movs r0, #17
	bl Func_02004e30
	b .L_0200a4be
.L_0200a33e:
	movs r0, #35
	bl Func_02004e30
	b .L_0200a4be
	.2byte 0x0000
.L_0200a348:
	.4byte MsgDjinnGiveTutorialB
.L_0200a34c:
	.4byte MsgDjinnGiveTutorialA
.L_0200a350:
	.4byte MsgSeeYouKnow
.L_0200a354:
	.4byte MsgEvenIKnow
.L_0200a358:
	.4byte MsgOhAllRight
.L_0200a35c:
	.4byte MsgOhComeOn
.L_0200a360:
	.4byte MsgIMSo
.L_0200a364:
	.4byte MsgTheVenusDjinni
.L_0200a368:
	bl Func_02004c20
	movs r0, #0
	bl Func_02004db8
	bl Event_StartScreenTransition
	bl Event_WaitTransitionFrames
	movs r0, #9
	adds r1, r7, #0
	bl Func_02004c98
	movs r3, #160
	lsls r3, r3, #12
	str r3, [r6, #40]
	ldr r1, [sp, #0]
	mov r3, r11
	movs r2, #0
	adds r0, r6, #0
	bl Func_02004b50
	movs r0, #30
	bl FieldEvent_Wait
	bl Func_02004da8
	movs r0, #9
	adds r1, r7, #0
	movs r2, #0
	bl Actor_SetAngleToward
	movs r2, #0
	adds r0, r7, #0
	movs r1, #9
	bl Actor_SetAngleToward
	movs r1, #22
	adds r0, r7, #0
	bl Actor_SetModeById
	movs r0, #118
	adds r0, #255
	bl GameFlag_Test
	ldr r2, .L_0200a57c
	ldr r3, .L_0200a580
	subs r5, r2, r3
	muls r0, r5
	ldr r3, .L_0200a584
	adds r0, r0, r3
	bl Func_02004cd8
	movs r0, #9
	movs r1, #2
	movs r2, #20
	bl Actor_Jump
	movs r2, #20
	movs r0, #9
	movs r1, #2
	bl Actor_Jump
	movs r0, #9
	movs r1, #0
	bl Func_02004cf0
	movs r0, #9
	movs r1, #2
	bl Motion_SetVarCbAndRefresh
	movs r1, #0
	movs r0, #9
	bl Func_02004cf0
	movs r0, #111
	bl Func_02004e30
	movs r1, #2
	movs r0, #0
	bl Menu_AnimateSelectionToEntry
	movs r0, #112
	adds r0, #255
	bl GameFlag_SetBit
	movs r0, #114
	adds r0, #255
	bl GameFlag_ClearBit
	bl Func_02004e28
	movs r0, #118
	adds r0, #255
	bl GameFlag_Test
	ldr r3, .L_0200a588
	muls r0, r5
	adds r0, r0, r3
	bl Func_02004cd8
	mov r3, r8
	movs r2, #0
	mov r1, r9
	adds r0, r6, #0
	bl Func_02004b50
	movs r0, #30
	bl FieldEvent_Wait
	movs r0, #9
	movs r1, #0
	bl Func_02004cf0
	movs r2, #0
	movs r0, #9
	adds r1, r7, #0
	bl Actor_SetAngleToward
	movs r0, #9
	movs r1, #0
	bl Func_02004cf0
	movs r1, #0
	movs r0, #9
	bl FieldMessage_ShowAtActor
	adds r0, r7, #0
	movs r1, #0
	bl Event_AskYesNoWithActorAnimation
	cmp r0, #1
	bne .L_0200a4c8
	adds r0, r7, #0
	movs r1, #22
	bl Actor_SetModeById
	movs r1, #2
	movs r0, #9
	bl Motion_SetVarCbAndRefresh
	movs r0, #118
	adds r0, #255
	bl GameFlag_Test
	ldr r3, .L_0200a58c
	muls r0, r5
	adds r0, r0, r3
	bl Func_02004cd8
	movs r1, #0
	movs r0, #9
	bl FieldMessage_ShowAtActor
	adds r0, r7, #0
	movs r1, #0
	bl Event_AskYesNoWithActorAnimation
	cmp r0, #1
	beq .L_0200a4c8
	movs r0, #9
	movs r1, #0
	bl Func_02004cf0
	ldr r3, [sp, #0]
	movs r0, #9
	asrs r1, r3, #16
	mov r3, r11
	asrs r2, r3, #16
	bl Actor_SetPositionAndReset
.L_0200a4be:
	bl Func_020011fc
	bl Func_02004db0
	b .L_0200a56c
.L_0200a4c8:
	movs r1, #22
	adds r0, r7, #0
	bl Actor_SetModeById
	movs r0, #118
	adds r0, #255
	bl GameFlag_Test
	ldr r2, .L_0200a580
	ldr r3, .L_0200a57c
	subs r3, r3, r2
	muls r0, r3
	ldr r3, .L_0200a590
	adds r0, r0, r3
	bl Func_02004cd8
	movs r0, #9
	movs r1, #2
	movs r2, #20
	bl Actor_Jump
	movs r2, #20
	movs r0, #9
	movs r1, #2
	bl Actor_Jump
	movs r0, #9
	movs r1, #0
	bl Func_02004cf0
	movs r1, #128
	movs r2, #30
	lsls r1, r1, #1
	movs r0, #9
	bl Func_02004d08
	movs r1, #0
	movs r0, #9
	bl Func_02004cf0
	movs r0, #112
	adds r0, #255
	bl GameFlag_SetBit
	movs r0, #114
	adds r0, #255
	bl GameFlag_SetBit
	bl Func_02004e28
	movs r2, #20
	movs r0, #9
	movs r1, #2
	bl Actor_Jump
	movs r0, #9
	movs r1, #0
	bl Func_02004cf0
	bl Func_02004db0
	movs r1, #0
	movs r2, #7
	movs r0, #9
	bl Func_02004dd0
	bl Func_02004dc0
	bl Func_02004c28
	movs r0, #183
	lsls r0, r0, #1
	bl GameFlag_ClearBit
	movs r0, #112
	adds r0, #255
	bl GameFlag_ClearBit
	movs r0, #114
	adds r0, #255
	bl GameFlag_ClearBit
.L_0200a56c:
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200a57c:
	.4byte MsgDjinnGiveTutorialB
.L_0200a580:
	.4byte MsgDjinnGiveTutorialA
.L_0200a584:
	.4byte MsgSeeWeDjinn
.L_0200a588:
	.4byte MsgSoWeGo
.L_0200a58c:
	.4byte MsgHmmmmDoYou
.L_0200a590:
	.4byte MsgWellThatS
	.section .bss,"aw",%nobits
	.global Data_02006be8
Data_02006be8:
	.space 0x00000020
	.global gData_02006c08
gData_02006c08:
	.space 0x00000200
	.global gData_02006e08
gData_02006e08:
	.space 0x00000004
	.global gData_02006e0c
gData_02006e0c:
	.space 0x00000002
	.global gData_02006e0e
gData_02006e0e:
	.space 0x00000002
	.global gData_02006e10
gData_02006e10:
	.space 0x00000004
	.global Data_02006e14
Data_02006e14:
	.space 0x0000000c
	.global Data_02006e20
Data_02006e20:
	.space 0x00000004
	.global Data_02006e24
Data_02006e24:
	.space 0x0000001c
	.global Data_02006e40
Data_02006e40:
	.space 0x00000020
	.global Data_02006e60
Data_02006e60:
	.space 0x00000020
	.global Data_02006e80
Data_02006e80:
	.space 0x000000a0
	.global gData_02006f20
gData_02006f20:
	.space 0x00000004
	.global Data_02006f24
Data_02006f24:
	.space 0x0000001c
	.global Data_02006f40
Data_02006f40:
	.space 0x00000020
	.global Data_02006f60
Data_02006f60:
	.space 0x00000020
	.global Data_02006f80
Data_02006f80:
	.space 0x00000020
	.global gData_02006fa0
gData_02006fa0:
	.space 0x00000080
	.global Data_02007020
Data_02007020:
