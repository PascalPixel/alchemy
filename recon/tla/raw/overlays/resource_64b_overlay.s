.syntax unified
	.thumb
	.section .text.x02008dc4,"ax",%progbits
	.balign 4
	.global Func_02000dc4
	.thumb_func
Func_02000dc4:
	push {r5, r6, r7, lr}
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008e24
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #222
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008e24
	ldr r5, .L_02009188
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #118
	adds r3, r5, r2
	movs r2, #0
	ldrsh r6, [r3, r2]
	cmp r6, #0
	bne .L_02008e36
	movs r3, #158
	lsls r3, r3, #2
	adds r7, r5, r3
	ldr r3, [r7]
	cmp r3, #0
	beq .L_02008e24
	movs r0, #8
	bl Actor_GetById
	ldr r3, [r7]
	movs r2, #159
	str r3, [r0, #8]
	lsls r2, r2, #2
	adds r3, r5, r2
	ldr r3, [r3]
	adds r2, #4
	str r3, [r0, #16]
	adds r3, r5, r2
	ldr r3, [r3]
	strh r3, [r0, #6]
	adds r3, r0, #0
	adds r3, #100
	adds r0, #102
	strh r6, [r3]
	strh r6, [r0]
.L_02008e24:
	ldr r3, .L_02009188
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #118
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	beq .L_02008e40
.L_02008e36:
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #1
	bl Func_02004df8
.L_02008e40:
	movs r0, #162
	lsls r0, r0, #1
	bl GameFlag_SetBit
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #108]
	movs r3, #214
	lsls r3, r3, #1
	adds r2, r1, r3
	movs r3, #128
	lsls r3, r3, #3
	str r3, [r2]
	movs r3, #218
	lsls r3, r3, #1
	adds r2, r1, r3
	movs r3, #16
	str r3, [r2]
	movs r0, #1
	bl wait
	movs r0, #128
	movs r1, #128
	lsls r1, r1, #9
	lsls r0, r0, #12
	bl Func_02004d20
	movs r0, #48
	adds r0, #255
	bl GameFlag_ClearBit
	bl Func_02003960
	movs r0, #129
	lsls r0, r0, #4
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008ea0
	movs r2, #168
	movs r3, #144
	movs r0, #0
	movs r1, #240
	lsls r2, r2, #1
	lsls r3, r3, #1
	bl Func_02004b60
.L_02008ea0:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #33
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008eca
	movs r3, #128
	movs r0, #0
	movs r1, #224
	movs r2, #208
	lsls r3, r3, #1
	bl Func_02004b60
	movs r3, #128
	movs r0, #16
	movs r1, #224
	movs r2, #224
	lsls r3, r3, #1
	bl Func_02004b60
.L_02008eca:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #119
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008edc
	bl Func_02000d4c
.L_02008edc:
	ldr r1, .L_02009188
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r1, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	subs r3, #2
	cmp r3, #97
	bls .L_02008ef0
	b .L_020091ee
.L_02008ef0:
	ldr r2, .L_0200918c
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
.L_02008ef8:
	.4byte .L_0200910c
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_0200909c
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_02009080
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020090ca
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020090fa
	.4byte .L_02009100
	.4byte .L_020091ee
	.4byte .L_02009124
	.4byte .L_0200912a
	.4byte .L_02009130
	.4byte .L_02009106
	.4byte .L_020091ee
	.4byte .L_02009136
	.4byte .L_0200909c
	.4byte .L_0200909c
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020090be
	.4byte .L_020090c4
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091a4
	.4byte .L_020091de
.L_02009080:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #39
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02009090
	b .L_020091ee
.L_02009090:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #171
	bl GameFlag_SetBit
	b .L_020091ee
.L_0200909c:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #118
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020090ac
	b .L_020091ee
.L_020090ac:
	ldr r3, .L_02009188
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r0, [r3, r2]
	bl Func_020012bc
	b .L_020091ee
.L_020090be:
	bl Func_020029a4
	b .L_020091ee
.L_020090c4:
	bl Func_02003490
	b .L_020091ee
.L_020090ca:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #10
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020090da
	b .L_020091ee
.L_020090da:
	movs r0, #150
	lsls r0, r0, #4
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020090ea
	b .L_020091ee
.L_020090ea:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #10
	bl GameFlag_SetBit
	bl Func_02001c5c
	b .L_020091ee
.L_020090fa:
	bl Func_02002698
	b .L_020091ee
.L_02009100:
	bl Func_02002ab0
	b .L_020091ee
.L_02009106:
	bl Func_02004c00
	b .L_020091ee
.L_0200910c:
	movs r0, #55
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020091ee
	ldr r1, .L_02009190
	movs r0, #9
	bl Actor_SetMovementTypeById
	bl Func_02001e34
	b .L_020091ee
.L_02009124:
	bl Func_02002e00
	b .L_020091ee
.L_0200912a:
	bl Func_02002e78
	b .L_020091ee
.L_02009130:
	bl Func_0200310c
	b .L_020091ee
.L_02009136:
	movs r0, #2
	bl Func_02003d44
	movs r0, #7
	bl Actor_GetById
	movs r1, #15
	bl Actor_SetPartAttribute
	ldr r2, .L_02009194
	ldr r1, .L_02009198
	movs r0, #7
	bl Func_02004c88
	movs r0, #1
	bl wait
	ldr r0, .L_0200919c
	bl Func_02004cd8
	movs r1, #0
	movs r0, #7
	bl Func_02004cf0
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #7
	movs r1, #0
	bl Func_02004cf0
	bl Event_FinishScreenTransition
	bl Event_WaitTransitionFrames
	ldr r0, .L_020091a0
	movs r1, #9
	bl Func_02004d38
	b .L_020091ee
	.2byte 0x0000
.L_02009188:
	.4byte gMainRoutineWork
.L_0200918c:
	.4byte .L_02008ef8
.L_02009190:
	.4byte gData_0200632c
.L_02009194:
	.4byte 0x1c840000
.L_02009198:
	.4byte 0x1c6d0000
.L_0200919c:
	.4byte MsgIVeHeard
.L_020091a0:
	.4byte 0x00000109
.L_020091a4:
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #118
	movs r0, #128
	adds r3, r1, r2
	lsls r0, r0, #4
	movs r2, #1
	strh r2, [r3]
	adds r0, #222
	bl GameFlag_SetBit
	movs r0, #252
	lsls r0, r0, #3
	adds r0, #255
	bl GameFlag_SetBit
	movs r0, #100
	adds r0, #255
	bl GameFlag_SetBit
	movs r0, #30
	adds r0, #255
	bl GameFlag_SetBit
	ldr r0, .L_020091f4
	movs r1, #61
	bl Func_02004d38
	b .L_020091ee
.L_020091de:
	movs r0, #100
	adds r0, #255
	bl GameFlag_SetBit
	movs r0, #30
	adds r0, #255
	bl GameFlag_SetBit
.L_020091ee:
	movs r0, #0
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_020091f4:
	.4byte 0x00000002
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
	.section .text.x0200c2e0,"ax",%progbits
	.global Func_020042e0
	.thumb_func
Func_020042e0:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r0, #128
	lsls r0, r0, #7
	bl Heap_AllocEwram
	adds r7, r0, #0
	adds r1, r7, #0
	ldr r0, .L_0200c4d4
	bl Func_02004aa8
	movs r1, #128
	lsls r1, r1, #6
	adds r1, r1, r7
	ldr r0, .L_0200c4d8
	mov r8, r1
	bl Func_02004aa8
	ldr r6, .L_0200c4dc
	ldr r5, .L_0200c4e0
	ldrh r3, [r5]
	adds r1, r3, #0
	strh r5, [r5]
	ldrh r2, [r6]
	cmp r2, #31
	bgt .L_0200c334
	lsls r3, r2, #1
	adds r3, r3, r2
	adds r2, #1
	lsls r3, r3, #2
	strh r2, [r6]
	ldr r2, .L_0200c4e4
	adds r3, r3, r6
	adds r3, #4
	stmia r3!, {r2}
	ldr r2, .L_0200c4e8
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #24
	adds r2, #16
	str r2, [r3]
.L_0200c334:
	strh r1, [r5]
	ldrh r3, [r5]
	adds r1, r3, #0
	strh r5, [r5]
	ldrh r2, [r6]
	cmp r2, #31
	bgt .L_0200c35c
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	adds r3, r3, r6
	adds r3, #4
	adds r2, #1
	stmia r3!, {r7}
	strh r2, [r6]
	movs r2, #192
	lsls r2, r2, #19
	stmia r3!, {r2}
	ldr r2, .L_0200c4ec
	str r2, [r3]
.L_0200c35c:
	strh r1, [r5]
	ldrh r3, [r5]
	adds r1, r3, #0
	strh r5, [r5]
	ldrh r2, [r6]
	cmp r2, #31
	bgt .L_0200c384
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	adds r2, #1
	adds r3, r3, r6
	adds r3, #4
	strh r2, [r6]
	mov r2, r8
	stmia r3!, {r2}
	ldr r2, .L_0200c4f0
	stmia r3!, {r2}
	ldr r2, .L_0200c4f4
	str r2, [r3]
.L_0200c384:
	strh r1, [r5]
	movs r0, #128
	movs r1, #252
	lsls r0, r0, #19
	lsls r1, r1, #6
	adds r0, #80
	adds r1, #65
	bl QueueIoWriteDelay2
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #19
	lsls r1, r1, #5
	adds r0, #82
	adds r1, #16
	bl QueueIoWriteDelay2
	ldr r2, .L_0200c4f8
	movs r3, #0
	strh r3, [r2]
	movs r3, #176
	lsls r3, r3, #1
	strh r3, [r2, #2]
	ldrh r3, [r5]
	adds r1, r3, #0
	strh r5, [r5]
	ldrh r2, [r6]
	cmp r2, #31
	bgt .L_0200c3e0
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	adds r2, #1
	adds r3, r3, r6
	strh r2, [r6]
	movs r2, #194
	adds r3, #4
	lsls r2, r2, #8
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #8
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #10
	str r2, [r3]
.L_0200c3e0:
	strh r1, [r5]
	movs r7, #180
	movs r6, #153
	lsls r7, r7, #6
	lsls r6, r6, #1
	adds r7, #96
	adds r6, #255
	movs r5, #95
.L_0200c3f0:
	lsls r3, r6, #4
	cmp r3, #0
	bge .L_0200c3f8
	adds r3, #127
.L_0200c3f8:
	asrs r2, r3, #7
	negs r3, r7
	cmp r3, #0
	bge .L_0200c402
	adds r3, #127
.L_0200c402:
	movs r1, #176
	lsls r1, r1, #1
	asrs r3, r3, #7
	adds r3, r3, r1
	ldr r1, .L_0200c4f8
	movs r0, #1
	mov r8, r1
	strh r2, [r1]
	mov r2, r8
	strh r3, [r2, #2]
	bl wait
	movs r3, #176
	lsls r3, r3, #1
	subs r5, #1
	adds r7, r7, r3
	adds r6, #17
	cmp r5, #0
	bge .L_0200c3f0
	movs r0, #164
	movs r5, #192
	bl Func_02004e30
	lsls r5, r5, #18
	movs r0, #120
	bl wait
	ldr r3, [r5, #108]
	movs r1, #214
	lsls r1, r1, #1
	movs r2, #128
	movs r0, #254
	adds r3, r3, r1
	lsls r2, r2, #1
	lsls r0, r0, #7
	str r2, [r3]
	movs r1, #0
	adds r0, #255
	bl Func_02004d78
	movs r0, #1
	bl Func_02004d88
	movs r0, #128
	lsls r0, r0, #19
	movs r1, #128
	lsls r1, r1, #5
	adds r0, #82
	bl QueueIoWriteDelay2
	movs r0, #1
	bl wait
	movs r1, #0
	movs r0, #0
	bl Func_02004d78
	movs r0, #60
	bl Func_02004d88
	movs r0, #78
	bl Func_02004e30
	movs r0, #60
	bl wait
	movs r0, #195
	lsls r0, r0, #1
	bl Func_02004e30
	movs r0, #120
	bl wait
	ldr r2, [r5, #24]
	movs r3, #1
	strh r3, [r2, #4]
	movs r0, #1
	bl wait
	bl Func_02004ad8
	bl Func_02004ac8
	ldr r3, .L_0200c4fc
	movs r1, #147
	lsls r1, r1, #1
	adds r1, #255
	adds r2, r3, r1
	ldrb r0, [r2]
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #38
	adds r3, r3, r2
	ldrb r1, [r3]
	bl Func_02004bb8
	movs r6, #0
	mov r3, r8
	mov r1, r8
	strh r6, [r3]
	strh r6, [r1, #2]
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200c4d4:
	.4byte gWorldMapPackedTilesB
.L_0200c4d8:
	.4byte gWorldMapPackedPicturesB
.L_0200c4dc:
	.4byte gIoWriteQueue
.L_0200c4e0:
	.4byte 0x04000208
.L_0200c4e4:
	.4byte gWorldMapPalettesB
.L_0200c4e8:
	.4byte 0x050001c0
.L_0200c4ec:
	.4byte 0x84000400
.L_0200c4f0:
	.4byte 0x06001000
.L_0200c4f4:
	.4byte 0x84000800
.L_0200c4f8:
	.4byte gBgScroll
.L_0200c4fc:
	.4byte gMainRoutineWork
	.section .text.x0200c634,"ax",%progbits
	.balign 4
	.global Func_02004634
	.thumb_func
Func_02004634:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #224
	ldr r3, [r3]
	sub sp, #8
	adds r6, r0, #0
	mov r9, r3
	movs r7, #0
.L_0200c64e:
	movs r0, #16
	ldr r1, [r6, #8]
	ldr r3, [r6, #16]
	ldr r2, [r6, #12]
	adds r0, #255
	bl Func_02004b30
	mov r8, sp
	lsls r3, r7, #2
	mov r1, r8
	str r0, [r3, r1]
	cmp r0, #0
	beq .L_0200c6f8
	ldr r3, [r6, #20]
	ldr r5, [r0, #80]
	str r3, [r0, #20]
	ldr r1, .L_0200c688
	adds r3, r0, #0
	adds r3, #85
	movs r2, #0
	strb r2, [r3]
	adds r3, #15
	strh r2, [r3]
	mov r10, r1
	str r6, [r0, #104]
	cmp r5, #0
	beq .L_0200c6f8
	b .L_0200c68c
	.2byte 0x0000
.L_0200c688:
	.4byte 0x00000000
.L_0200c68c:
	movs r1, #0
	adds r0, r5, #0
	bl Animation_ApplyChildArgument
	mov r2, r10
	strb r2, [r5, #26]
	ldrb r0, [r5, #16]
	bl Resource_ResetEntry
	movs r3, #248
	lsls r3, r3, #3
	add r3, r9
	ldrh r3, [r3]
	movs r2, #1
	strb r3, [r5, #16]
	ldrb r3, [r5, #17]
	ldr r1, .L_0200c6e8
	orrs r3, r2
	strb r3, [r5, #17]
	ldrb r3, [r5, #16]
	ldr r2, .L_0200c6ec
	lsls r3, r3, #2
	adds r3, r3, r2
	ldrh r2, [r3, #2]
	ldrh r3, [r5, #8]
	lsls r2, r2, #17
	lsrs r2, r2, #22
	ands r3, r1
	orrs r3, r2
	strh r3, [r5, #8]
	movs r1, #33
	ldrb r3, [r5, #5]
	negs r1, r1
	adds r2, r1, #0
	ands r3, r2
	movs r2, #63
	ands r3, r2
	movs r1, #64
	orrs r3, r1
	strb r3, [r5, #5]
	ldrb r3, [r5, #7]
	ands r2, r3
	movs r3, #128
	orrs r2, r3
	b .L_0200c6f0
	.2byte 0x0000
.L_0200c6e8:
	.4byte 0xfffffc00
.L_0200c6ec:
	.4byte gResourceTableEntries
.L_0200c6f0:
	ldr r3, [r5, #40]
	strb r2, [r5, #7]
	mov r2, r10
	strb r2, [r3, #22]
.L_0200c6f8:
	adds r7, #1
	cmp r7, #1
	ble .L_0200c64e
	.global Data_020046fe
Data_020046fe:
	ldr r2, [sp, #0]
	ldr r3, .L_0200c73c
	ldr r0, [r2, #80]
	str r3, [r2, #108]
	ldrb r1, [r0, #9]
	movs r2, #13
	negs r2, r2
	adds r3, r2, #0
	movs r4, #4
	ands r3, r1
	orrs r3, r4
	strb r3, [r0, #9]
	mov r3, r8
	ldr r1, [r3, #4]
	add sp, #8
	ldr r0, [r1, #80]
	ldrb r3, [r0, #9]
	ands r2, r3
	ldr r3, .L_0200c740
	orrs r2, r4
	str r3, [r1, #108]
	adds r1, #35
	movs r3, #2
	strb r2, [r0, #9]
	strb r3, [r1]
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200c73c:
	.4byte Func_020045e4
.L_0200c740:
	.4byte Func_02004598
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
