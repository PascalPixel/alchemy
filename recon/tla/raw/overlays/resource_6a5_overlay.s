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
