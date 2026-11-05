.syntax unified
	.thumb
	.section .text.x0200865c,"ax",%progbits
	.global Func_0200065c
	.thumb_func
Func_0200065c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #35
	sub sp, #8
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200867c
	b .L_02008cac
.L_0200867c:
	bl Func_02005814
	movs r0, #0
	bl Func_02005984
	ldr r2, .L_020086c0
	movs r1, #5
	mov r8, r2
	mov r0, r8
	bl FieldMessage_Show
	movs r0, #4
	movs r1, #0
	bl Event_AskYesNoWithActorAnimation
	mov r11, r0
	cmp r0, #0
	beq .L_020086c4
	movs r1, #128
	movs r2, #128
	movs r0, #4
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl Actor_SetSpeedParameters
	movs r1, #128
	lsls r1, r1, #2
	movs r2, #54
	movs r0, #4
	adds r1, #2
	adds r2, #255
	bl Actor_SetPositionAndReset
	b .L_02008ca8
.L_020086c0:
	.4byte MsgThereSeemsTo
.L_020086c4:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #33
	bl GameFlag_SetBit
	movs r0, #252
	lsls r0, r0, #3
	adds r0, #255
	bl GameFlag_SetBit
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #37
	bl GameFlag_ClearBit
	movs r0, #98
	adds r0, #255
	bl GameFlag_ClearBit
	movs r0, #162
	lsls r0, r0, #1
	bl GameFlag_ClearBit
	movs r1, #208
	movs r0, #27
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #208
	movs r0, #28
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #128
	movs r2, #128
	movs r0, #4
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl Actor_SetSpeedParameters
	movs r1, #128
	movs r2, #147
	lsls r2, r2, #1
	movs r0, #4
	lsls r1, r1, #2
	bl Actor_SetPositionAndReset
	movs r1, #192
	lsls r1, r1, #8
	movs r0, #4
	bl Func_020058ec
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #0
	bl Func_0200595c
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #1
	adds r0, #3
	bl Func_02005954
	movs r0, #20
	bl Func_02005964
	movs r0, #20
	bl wait
	movs r0, #245
	bl PartyInventory_Remove
	movs r0, #4
	bl Actor_GetById
	adds r5, r0, #0
	ldr r2, [r5, #12]
	movs r3, #128
	lsls r3, r3, #12
	movs r0, #234
	adds r2, r2, r3
	ldr r1, [r5, #8]
	ldr r3, [r5, #16]
	adds r0, #255
	bl Func_0200576c
	adds r7, r0, #0
	cmp r7, #0
	beq .L_020087f4
	ldr r6, [r7, #80]
	mov r2, r11
	strb r2, [r6, #27]
	ldrb r2, [r6, #5]
	movs r3, #33
	negs r3, r3
	ands r3, r2
	ldrb r2, [r6, #9]
	strb r3, [r6, #5]
	movs r3, #15
	ands r3, r2
	adds r1, r7, #0
	movs r2, #13
	adds r1, #35
	negs r2, r2
	ands r3, r2
	ldrb r2, [r1]
	strb r3, [r6, #9]
	movs r3, #254
	ands r3, r2
	strb r3, [r1]
	adds r3, r7, #0
	adds r3, #85
	mov r2, r11
	strb r2, [r3]
	adds r2, r7, #0
	adds r2, #92
	movs r3, #1
	strb r3, [r2]
	movs r3, #204
	lsls r3, r3, #7
	adds r3, #102
	str r3, [r7, #48]
	movs r3, #192
	lsls r3, r3, #4
	adds r3, #204
	movs r1, #193
	str r3, [r7, #52]
	lsls r1, r1, #3
	movs r0, #68
	bl HeapSlot_AllocFar
	str r0, [sp, #4]
	movs r0, #246
	bl Func_020057d4
	ldr r3, [sp, #4]
	movs r2, #128
	lsls r2, r2, #3
	ldrb r0, [r6, #16]
	adds r2, r3, r2
	movs r1, #128
	str r2, [sp, #0]
	bl VramCache_Load
	movs r0, #68
	bl HeapSlot_Release
.L_020087f4:
	movs r1, #1
	movs r0, #37
	bl Actor_SetSpritePriority
	movs r0, #37
	bl Actor_GetById
	ldr r3, [r5, #8]
	adds r6, r0, #0
	str r3, [r6, #8]
	movs r3, #192
	lsls r3, r3, #15
	str r3, [r6, #12]
	adds r2, r6, #0
	ldr r3, [r5, #16]
	adds r2, #85
	str r3, [r6, #16]
	movs r3, #3
	strb r3, [r2]
	movs r3, #192
	lsls r3, r3, #4
	adds r3, #204
	movs r2, #204
	str r3, [r6, #52]
	lsls r2, r2, #7
	movs r3, #128
	adds r2, #102
	lsls r3, r3, #6
	mov r0, r8
	adds r0, #1
	str r2, [r6, #48]
	str r3, [r6, #24]
	str r3, [r6, #28]
	mov r9, r2
	bl Func_020058c4
	movs r0, #36
	bl Func_020059f4
	cmp r7, #0
	bne .L_02008848
	b .L_020089ca
.L_02008848:
	movs r3, #85
	adds r3, r3, r7
	mov r2, r11
	mov r10, r3
	strb r2, [r3]
	movs r3, #153
	lsls r3, r3, #8
	adds r3, #153
	str r3, [r7, #72]
	movs r3, #204
	lsls r3, r3, #8
	movs r2, #128
	adds r3, #204
	lsls r2, r2, #12
	mov r8, r3
	str r3, [r7, #68]
	str r2, [r7, #40]
	movs r1, #128
	movs r2, #128
	movs r3, #147
	adds r0, r7, #0
	lsls r1, r1, #18
	lsls r2, r2, #16
	lsls r3, r3, #17
	bl Func_02005784
	movs r1, #128
	movs r2, #1
	movs r3, #147
	adds r0, r6, #0
	lsls r1, r1, #18
	negs r2, r2
	lsls r3, r3, #17
	bl Func_02005784
	ldr r3, .L_020089b8
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r1, [r3]
	movs r0, #7
	bl Func_02005894
	movs r0, #1
	bl wait
	mov r1, r8
	movs r0, #7
	mov r2, r9
	bl Actor_SetSpeedParameters
	movs r0, #7
	movs r1, #12
	movs r2, #0
	bl Actor_OffsetPositionAndReset
	movs r1, #12
	movs r2, #0
	negs r1, r1
	movs r0, #4
	bl Actor_CommitPositionAndActivate
	movs r0, #7
	bl Actor_WaitForMoveById
	movs r0, #7
	bl Actor_Stop
	movs r0, #1
	bl wait
	movs r1, #1
	movs r0, #7
	bl Actor_SetModeById
	movs r0, #1
	bl wait
	movs r1, #192
	movs r0, #4
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #7
	bl Actor_ArmCallback
	adds r0, r7, #0
	bl Func_0200578c
	movs r3, #4
	mov r8, r3
	mov r2, r8
	mov r3, r10
	strb r2, [r3]
	movs r0, #90
	bl FieldEvent_Wait
	mov r2, r11
	mov r3, r10
	strb r2, [r3]
	movs r1, #128
	movs r2, #128
	movs r3, #132
	adds r0, r7, #0
	lsls r1, r1, #18
	lsls r2, r2, #16
	lsls r3, r3, #17
	bl Func_02005784
	movs r1, #128
	movs r2, #1
	movs r3, #132
	lsls r1, r1, #18
	adds r0, r6, #0
	negs r2, r2
	lsls r3, r3, #17
	bl Func_02005784
	mov r2, r8
	mov r3, r10
	strb r2, [r3]
	movs r0, #60
	bl FieldEvent_Wait
	mov r2, r11
	mov r3, r10
	strb r2, [r3]
	movs r3, #128
	lsls r3, r3, #24
	mov r2, r11
	str r3, [r6, #56]
	str r3, [r6, #60]
	str r3, [r6, #64]
	adds r3, r6, #0
	str r2, [r6, #8]
	str r2, [r6, #12]
	str r2, [r6, #16]
	str r2, [r6, #36]
	str r2, [r6, #40]
	str r2, [r6, #44]
	adds r3, #100
	mov r2, r11
	strh r2, [r3]
	adds r0, r7, #0
	bl Func_0200578c
	ldr r6, .L_020089b4
	mov r3, r8
	mov r2, r10
	strb r3, [r2]
	movs r0, #90
	bl FieldEvent_Wait
	mov r3, r10
	strb r6, [r3]
	movs r1, #128
	movs r2, #184
	movs r3, #132
	lsls r2, r2, #15
	lsls r3, r3, #17
	adds r0, r7, #0
	lsls r1, r1, #18
	bl Func_02005784
	adds r0, r7, #0
	bl Func_0200578c
	movs r3, #128
	lsls r3, r3, #24
	mov r2, r11
	str r3, [r7, #56]
	str r3, [r7, #60]
	str r3, [r7, #64]
	adds r3, r7, #0
	str r2, [r7, #8]
	str r2, [r7, #12]
	b .L_020089bc
	.2byte 0x0000
.L_020089b4:
	.4byte 0x00000000
.L_020089b8:
	.4byte gMainRoutineWork
.L_020089bc:
	str r2, [r7, #16]
	str r2, [r7, #36]
	str r2, [r7, #40]
	str r2, [r7, #44]
	adds r3, #100
	mov r2, r11
	strh r2, [r3]
.L_020089ca:
	movs r0, #40
	bl FieldEvent_Wait
	movs r0, #128
	movs r1, #0
	lsls r0, r0, #9
	bl Func_02005954
	movs r0, #20
	bl Func_02005964
	movs r0, #20
	bl wait
	movs r2, #16
	movs r0, #4
	movs r1, #0
	negs r2, r2
	bl Actor_OffsetPositionAndReset
	movs r2, #16
	negs r2, r2
	movs r1, #0
	movs r0, #7
	bl Actor_OffsetPositionAndReset
	ldr r6, .L_02008cbc
	movs r3, #133
	lsls r3, r3, #2
	adds r6, r6, r3
	ldr r0, [r6]
	bl Actor_WaitForMoveById
	ldr r0, [r6]
	movs r1, #1
	bl Actor_SetModeById
	movs r1, #1
	movs r0, #7
	bl Actor_SetModeById
	movs r0, #50
	bl FieldEvent_Wait
	movs r0, #141
	bl Func_020059f4
	movs r0, #30
	bl FieldEvent_Wait
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #9
	lsls r2, r2, #9
	lsls r0, r0, #9
	bl Func_020057ac
	movs r0, #30
	bl FieldEvent_Wait
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #10
	lsls r2, r2, #9
	lsls r0, r0, #10
	bl Func_020057ac
	movs r0, #30
	bl FieldEvent_Wait
	movs r0, #0
	bl Actor_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r2, #1
	orrs r3, r2
	strb r3, [r0]
	movs r0, #1
	bl Actor_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r2, #1
	orrs r3, r2
	strb r3, [r0]
	movs r0, #2
	bl Actor_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r2, #1
	orrs r3, r2
	strb r3, [r0]
	movs r0, #3
	bl Actor_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r2, #1
	orrs r3, r2
	strb r3, [r0]
	movs r0, #4
	bl Actor_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r2, #1
	orrs r3, r2
	strb r3, [r0]
	movs r0, #5
	bl Actor_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r2, #1
	orrs r3, r2
	strb r3, [r0]
	movs r0, #7
	bl Actor_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r2, #1
	orrs r3, r2
	strb r3, [r0]
	movs r0, #6
	bl Actor_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r2, #1
	orrs r3, r2
	strb r3, [r0]
	movs r0, #30
	bl Actor_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r2, #1
	orrs r3, r2
	strb r3, [r0]
	movs r0, #27
	bl Actor_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r2, #1
	orrs r3, r2
	strb r3, [r0]
	movs r0, #28
	bl Actor_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r2, #1
	orrs r3, r2
	strb r3, [r0]
	movs r0, #31
	bl Actor_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r2, #1
	orrs r3, r2
	strb r3, [r0]
	movs r0, #32
	bl Actor_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r2, #1
	orrs r3, r2
	strb r3, [r0]
	movs r1, #192
	movs r0, #128
	movs r2, #132
	lsls r2, r2, #17
	movs r3, #1
	lsls r0, r0, #18
	lsls r1, r1, #15
	bl Camera_MoveWithinMapBounds
	bl Func_02005924
	movs r0, #31
	movs r1, #1
	bl Actor_SetSpritePriority
	movs r0, #32
	movs r1, #1
	bl Actor_SetSpritePriority
	ldr r0, [r6]
	movs r1, #1
	bl Actor_SetSpritePriority
	movs r1, #1
	movs r0, #7
	bl Actor_SetSpritePriority
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #31
	bl Actor_GetById
	movs r1, #0
	bl Actor_SetSingleChildFlags
	movs r0, #31
	bl Actor_GetById
	movs r1, #7
	bl Actor_SetPartAttribute
	movs r0, #31
	bl Actor_GetById
	ldr r3, .L_02008cc0
	movs r2, #200
	lsls r2, r2, #5
	str r3, [r0, #28]
	adds r2, #153
	mov r9, r3
	adds r3, r0, #0
	str r2, [r0, #24]
	mov r10, r2
	adds r3, #85
	mov r2, r11
	strb r2, [r3]
	movs r3, #128
	lsls r3, r3, #18
	str r3, [r0, #8]
	mov r8, r3
	movs r6, #132
	movs r3, #128
	lsls r6, r6, #17
	lsls r3, r3, #16
	str r3, [r0, #12]
	str r6, [r0, #16]
	adds r0, #35
	ldrb r3, [r0]
	movs r7, #2
	orrs r3, r7
	strb r3, [r0]
	movs r0, #32
	bl Actor_GetById
	movs r1, #0
	bl Actor_SetSingleChildFlags
	movs r0, #32
	bl Actor_GetById
	movs r1, #7
	bl Actor_SetPartAttribute
	movs r0, #32
	bl Actor_GetById
	mov r3, r10
	str r3, [r0, #24]
	mov r2, r9
	adds r3, r0, #0
	str r2, [r0, #28]
	adds r3, #85
	mov r2, r11
	strb r2, [r3]
	mov r3, r8
	str r3, [r0, #8]
	movs r3, #160
	lsls r3, r3, #16
	str r3, [r0, #12]
	str r6, [r0, #16]
	adds r0, #35
	ldrb r3, [r0]
	ldr r1, .L_02008cc4
	orrs r3, r7
	strb r3, [r0]
	movs r0, #31
	bl Actor_SetMovementTypeById
	movs r0, #32
	ldr r1, .L_02008cc4
	bl Actor_SetMovementTypeById
	ldr r6, .L_02008cc8
	movs r1, #144
	lsls r1, r1, #3
	adds r0, r6, #0
	bl Task_Add
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_02005884
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_02005884
	movs r0, #23
	movs r1, #0
	movs r2, #0
	bl Func_02005884
	movs r1, #0
	movs r2, #0
	movs r0, #24
	bl Func_02005884
	movs r0, #145
	bl Func_020059f4
	movs r0, #192
	movs r1, #192
	movs r2, #128
	lsls r2, r2, #9
	lsls r0, r0, #11
	lsls r1, r1, #11
	bl Func_020057ac
	movs r1, #0
	ldr r0, .L_02008ccc
	bl Func_02005954
	movs r0, #16
	bl Func_02005964
	movs r0, #16
	bl wait
	movs r0, #63
	bl Func_020059f4
	movs r0, #180
	bl wait
	movs r0, #254
	lsls r0, r0, #7
	movs r1, #0
	adds r0, #255
	bl Func_02005954
	movs r0, #60
	bl Func_02005964
	movs r0, #141
	bl Func_020059f4
	movs r0, #60
	bl wait
	adds r0, r6, #0
	bl Task_RemoveFar
	movs r0, #141
	lsls r0, r0, #1
	bl GameFlag_SetBit
	movs r0, #2
	bl Func_02005934
.L_02008ca8:
	bl Func_0200581c
.L_02008cac:
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008cbc:
	.4byte gMainRoutineWork
.L_02008cc0:
	.4byte 0xffff0000
.L_02008cc4:
	.4byte gData_02006338
.L_02008cc8:
	.4byte Func_020004c8
.L_02008ccc:
	.4byte 0x004063ff
	.section .text.x0200ba6c,"ax",%progbits
	.balign 4
	.global Func_02003a6c
	.thumb_func
Func_02003a6c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	movs r1, #188
	movs r2, #200
	lsls r1, r1, #1
	lsls r2, r2, #5
	adds r1, r1, r3
	sub sp, #8
	adds r2, #153
	movs r3, #0
	str r2, [sp, #4]
	str r3, [sp, #0]
	ldr r2, .L_0200bc3c
	ldr r3, [r1, #12]
	mov r8, r0
	mov r11, r1
	str r3, [r2]
	cmp r0, #1
	beq .L_0200baba
	cmp r0, #1
	bgt .L_0200baac
	cmp r0, #0
	beq .L_0200bab6
	b .L_0200bac6
.L_0200baac:
	mov r1, r8
	cmp r1, #2
	beq .L_0200bac0
	cmp r1, #3
	bne .L_0200bac6
.L_0200bab6:
	ldr r2, .L_0200bc40
	b .L_0200bac8
.L_0200baba:
	ldr r3, .L_0200bc44
	mov r9, r3
	b .L_0200baca
.L_0200bac0:
	ldr r1, .L_0200bc48
	mov r9, r1
	b .L_0200baca
.L_0200bac6:
	ldr r2, .L_0200bc4c
.L_0200bac8:
	mov r9, r2
.L_0200baca:
	mov r1, r9
	ldr r3, [r1]
	movs r7, #0
	cmp r3, #64
	beq .L_0200bafc
	mov r5, r9
.L_0200bad6:
	ldr r0, [r5]
	bl Actor_GetById
	movs r3, #0
	adds r0, #85
	strb r3, [r0]
	movs r1, #1
	ldmia r5!, {r0}
	bl Actor_SetSpritePriority
	ldr r2, [sp, #0]
	adds r7, #1
	adds r2, #1
	str r2, [sp, #0]
	cmp r7, #4
	bhi .L_0200bafc
	ldr r3, [r5]
	cmp r3, #64
	bne .L_0200bad6
.L_0200bafc:
	movs r7, #0
.L_0200bafe:
	ldr r6, .L_0200bc50
	lsls r5, r7, #2
	ldr r0, [r6, r5]
	bl Actor_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #32
	orrs r3, r2
	strb r3, [r0]
	movs r1, #0
	ldr r0, [r6, r5]
	adds r7, #1
	bl Actor_SetSpritePriority
	cmp r7, #5
	bls .L_0200bafe
	movs r0, #223
	bl Func_020059f4
	movs r7, #0
.L_0200bb28:
	mov r1, r11
	ldr r3, [r1, #12]
	ldr r2, [sp, #4]
	subs r3, r3, r2
	str r3, [r1, #12]
	ldr r1, [sp, #0]
	movs r3, #0
	mov r10, r3
	cmp r10, r1
	bcs .L_0200bb68
	mov r6, r9
.L_0200bb3e:
	ldr r0, [r6]
	bl Actor_GetById
	ldr r3, [r0, #16]
	ldr r2, [sp, #4]
	adds r3, r3, r2
	str r3, [r0, #16]
	ldr r0, [r6]
	bl Actor_GetById
	adds r5, r0, #0
	ldmia r6!, {r0}
	bl Actor_GetById
	ldr r3, [r0, #16]
	str r3, [r5, #64]
	ldr r1, [sp, #0]
	movs r3, #1
	add r10, r3
	cmp r10, r1
	bcc .L_0200bb3e
.L_0200bb68:
	movs r3, #3
	ands r3, r7
	cmp r3, #3
	bne .L_0200bb7c
	ldr r2, [sp, #4]
	movs r3, #200
	lsls r3, r3, #5
	adds r3, #153
	adds r2, r2, r3
	str r2, [sp, #4]
.L_0200bb7c:
	ldr r1, [sp, #4]
	ldr r2, .L_0200bc54
	cmp r1, r2
	ble .L_0200bb8a
	movs r3, #192
	lsls r3, r3, #9
	str r3, [sp, #4]
.L_0200bb8a:
	movs r0, #1
	bl wait
	mov r1, r8
	cmp r1, #0
	bne .L_0200bba2
	cmp r7, #40
	bne .L_0200bbbe
	movs r0, #128
	movs r1, #208
	movs r2, #152
	b .L_0200bbb2
.L_0200bba2:
	mov r2, r8
	cmp r2, #1
	bne .L_0200bbc4
	cmp r7, #40
	bne .L_0200bbbe
	movs r0, #128
	movs r1, #208
	movs r2, #176
.L_0200bbb2:
	lsls r0, r0, #18
	lsls r1, r1, #15
	lsls r2, r2, #17
	movs r3, #1
	bl Camera_MoveWithinMapBounds
.L_0200bbbe:
	cmp r7, #120
	bne .L_0200bbda
	b .L_0200bbe0
.L_0200bbc4:
	mov r3, r8
	cmp r3, #3
	bne .L_0200bbda
	cmp r7, #40
	bne .L_0200bbd6
	movs r0, #0
	movs r1, #1
	bl Camera_FollowActor
.L_0200bbd6:
	cmp r7, #120
	beq .L_0200bbe0
.L_0200bbda:
	adds r7, #1
	cmp r7, #227
	bls .L_0200bb28
.L_0200bbe0:
	movs r7, #0
.L_0200bbe2:
	ldr r6, .L_0200bc50
	lsls r5, r7, #2
	ldr r0, [r6, r5]
	bl Actor_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #223
	ands r3, r2
	strb r3, [r0]
	movs r1, #1
	ldr r0, [r6, r5]
	adds r7, #1
	bl Actor_SetSpritePriority
	cmp r7, #5
	bls .L_0200bbe2
	ldr r1, [sp, #0]
	movs r7, #0
	cmp r7, r1
	bcs .L_0200bc2c
	mov r5, r9
.L_0200bc0e:
	movs r2, #0
	ldr r0, [r5]
	movs r1, #0
	bl Func_02005884
	ldmia r5!, {r0}
	bl Actor_GetById
	movs r3, #0
	adds r0, #85
	strb r3, [r0]
	ldr r2, [sp, #0]
	adds r7, #1
	cmp r7, r2
	bcc .L_0200bc0e
.L_0200bc2c:
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200bc3c:
	.4byte Data_02006a1c
.L_0200bc40:
	.4byte gData_020059fc
.L_0200bc44:
	.4byte gData_02005a0c
.L_0200bc48:
	.4byte gData_02005a14
.L_0200bc4c:
	.4byte gData_02005a1c
.L_0200bc50:
	.4byte gData_020069dc
.L_0200bc54:
	.4byte 0x00017fff
	.section .text.x0200c7d4,"ax",%progbits
	.balign 4
	.global Func_020047d4
	.thumb_func
Func_020047d4:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #32]
	ldr r3, .L_0200c968
	adds r2, r1, #0
	adds r2, #228
	ldr r0, [r2]
	ldr r2, [r2, #4]
	ands r0, r3
	ands r2, r3
	ldr r3, .L_0200c96c
	mov r10, r0
	movs r0, #0
	ldrsh r3, [r3, r0]
	mov r8, r2
	ldr r2, .L_0200c970
	lsls r3, r3, #2
	adds r3, r3, r2
	ldrh r3, [r3, #2]
	sub sp, #8
	lsrs r3, r3, #5
	str r3, [sp, #4]
	ldr r6, .L_0200c974
	ldr r3, [r1]
	movs r1, #0
	ldr r3, [r3, #4]
	mov r9, r1
	str r3, [sp, #0]
	ldr r3, .L_0200c978
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r9, r3
	blt .L_0200c826
	b .L_0200c95a
.L_0200c826:
	ldr r2, .L_0200c97c
	mov r0, r9
	lsls r3, r0, #2
	ldr r5, [r2, r3]
	cmp r5, #0
	bne .L_0200c834
	b .L_0200c94a
.L_0200c834:
	ldr r3, [r5, #8]
	cmp r3, #0
	bne .L_0200c83c
	b .L_0200c94a
.L_0200c83c:
	mov r1, r10
	subs r0, r3, r1
	ldr r2, [sp, #0]
	ldr r3, [r5, #12]
	movs r1, #128
	subs r3, r3, r2
	ldr r2, [r5, #16]
	lsls r1, r1, #12
	adds r3, r3, r1
	mov r1, r8
	subs r2, r2, r1
	ldr r1, [sp, #0]
	subs r2, r2, r1
	subs r4, r2, r3
	adds r3, r3, r2
	asrs r3, r3, #16
	adds r3, #58
	mov r11, r3
	ldr r3, .L_0200c980
	movs r1, #0
	ldrsh r2, [r3, r1]
	adds r3, r5, #0
	mov r12, r2
	asrs r1, r0, #16
	mov r0, r12
	adds r3, #100
	asrs r2, r4, #16
	cmp r0, #0
	bne .L_0200c8b2
	movs r0, #0
	ldrsh r7, [r3, r0]
	adds r0, r1, #0
	adds r3, r1, #7
	movs r1, #167
	adds r4, r2, #0
	lsls r1, r1, #1
	subs r0, #8
	subs r4, #16
	cmp r3, r1
	bhi .L_0200c94a
	movs r2, #16
	negs r2, r2
	cmp r4, r2
	ble .L_0200c94a
	cmp r4, #239
	bgt .L_0200c94a
	movs r3, #128
	lsls r3, r3, #1
	adds r3, #255
	ands r0, r3
	movs r3, #255
	adds r1, r6, #0
	ands r4, r3
	mov r3, r12
	stmia r1!, {r3}
	lsls r3, r0, #16
	orrs r4, r3
	ldr r3, .L_0200c984
	b .L_0200c8ee
.L_0200c8b2:
	movs r0, #0
	ldrsh r7, [r3, r0]
	adds r0, r1, #0
	adds r3, r1, #0
	movs r1, #175
	adds r4, r2, #0
	adds r3, #23
	lsls r1, r1, #1
	subs r0, #8
	subs r4, #64
	cmp r3, r1
	bhi .L_0200c94a
	movs r2, #64
	negs r2, r2
	cmp r4, r2
	ble .L_0200c94a
	cmp r4, #175
	bgt .L_0200c94a
	movs r3, #128
	lsls r3, r3, #1
	adds r3, #255
	ands r0, r3
	movs r3, #255
	adds r1, r6, #0
	ands r4, r3
	movs r3, #0
	stmia r1!, {r3}
	lsls r3, r0, #16
	orrs r4, r3
	ldr r3, .L_0200c988
.L_0200c8ee:
	movs r2, #128
	orrs r4, r3
	stmia r1!, {r4}
	ldr r0, [sp, #4]
	lsls r3, r7, #3
	adds r3, r0, r3
	lsls r2, r2, #4
	orrs r3, r2
	str r3, [r1]
	ldr r3, .L_0200c98c
	movs r0, #1
	ldrh r2, [r3]
	movs r1, #0
	ldrsh r3, [r3, r1]
	negs r0, r0
	cmp r3, r0
	bne .L_0200c92c
	adds r0, r5, #0
	bl Func_020059bc
	movs r3, #3
	ands r0, r3
	movs r1, #13
	ldrb r3, [r6, #9]
	negs r1, r1
	adds r2, r1, #0
	lsls r0, r0, #2
	ands r3, r2
	orrs r3, r0
	strb r3, [r6, #9]
	b .L_0200c940
.L_0200c92c:
	movs r3, #3
	ands r3, r2
	movs r0, #13
	ldrb r2, [r6, #9]
	negs r0, r0
	adds r1, r0, #0
	lsls r3, r3, #2
	ands r2, r1
	orrs r2, r3
	strb r2, [r6, #9]
.L_0200c940:
	adds r0, r6, #0
	mov r1, r11
	bl Func_0200571c
	adds r6, #12
.L_0200c94a:
	ldr r3, .L_0200c978
	movs r1, #1
	movs r2, #0
	ldrsh r3, [r3, r2]
	add r9, r1
	cmp r9, r3
	bge .L_0200c95a
	b .L_0200c826
.L_0200c95a:
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0200c968:
	.4byte 0xffff0000
.L_0200c96c:
	.4byte gData_02006a6c
.L_0200c970:
	.4byte gResourceTableEntries
.L_0200c974:
	.4byte Data_02006ab0
.L_0200c978:
	.4byte gData_02006a6e
.L_0200c97c:
	.4byte gData_02006a70
.L_0200c980:
	.4byte gData_02006b70
.L_0200c984:
	.4byte 0x40002000
.L_0200c988:
	.4byte 0xc000a000
.L_0200c98c:
	.4byte gData_02006b72
	.section .bss,"aw",%nobits
	.global Data_02006a1c
Data_02006a1c:
	.space 0x00000008
	.global gData_02006a24
gData_02006a24:
	.space 0x00000004
	.global Data_02006a28
Data_02006a28:
	.space 0x00000004
	.global gData_02006a2c
gData_02006a2c:
	.space 0x00000020
	.global gData_02006a4c
gData_02006a4c:
	.space 0x00000004
	.global gData_02006a50
gData_02006a50:
	.space 0x0000001c
	.global gData_02006a6c
gData_02006a6c:
	.space 0x00000002
	.global gData_02006a6e
gData_02006a6e:
	.space 0x00000002
	.global gData_02006a70
gData_02006a70:
	.space 0x00000040
	.global Data_02006ab0
Data_02006ab0:
	.space 0x000000c0
	.global gData_02006b70
gData_02006b70:
	.space 0x00000002
	.global gData_02006b72
gData_02006b72:
	.space 0x00000002
	.global Data_02006b74
Data_02006b74:
