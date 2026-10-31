.syntax unified
	.thumb
	.section .text.x02008198,"ax",%progbits
	.global Func_02000198
	.thumb_func
Func_02000198:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r0, #36
	bl Actor_GetById
	ldr r5, .L_02008364
	adds r6, r0, #0
	ldr r3, [r5]
	movs r7, #0
	cmp r3, #48
	bls .L_020081b4
	b .L_02008390
.L_020081b4:
	ldr r2, .L_02008368
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
.L_020081bc:
	.4byte .L_02008280
	.4byte .L_02008390
	.4byte .L_02008390
	.4byte .L_02008390
	.4byte .L_02008390
	.4byte .L_02008390
	.4byte .L_02008390
	.4byte .L_02008390
	.4byte .L_0200829a
	.4byte .L_02008390
	.4byte .L_02008390
	.4byte .L_02008390
	.4byte .L_02008390
	.4byte .L_02008390
	.4byte .L_02008390
	.4byte .L_02008390
	.4byte .L_020082ac
	.4byte .L_02008390
	.4byte .L_02008390
	.4byte .L_02008390
	.4byte .L_02008390
	.4byte .L_02008390
	.4byte .L_02008390
	.4byte .L_02008390
	.4byte .L_020082be
	.4byte .L_020082e6
	.4byte .L_020082fe
	.4byte .L_02008346
	.4byte .L_02008346
	.4byte .L_02008346
	.4byte .L_02008346
	.4byte .L_02008346
	.4byte .L_02008346
	.4byte .L_02008346
	.4byte .L_02008346
	.4byte .L_02008390
	.4byte .L_0200834a
	.4byte .L_02008390
	.4byte .L_02008390
	.4byte .L_02008390
	.4byte .L_02008390
	.4byte .L_02008390
	.4byte .L_02008390
	.4byte .L_02008390
	.4byte .L_02008390
	.4byte .L_02008390
	.4byte .L_02008390
	.4byte .L_02008390
	.4byte .L_02008380
.L_02008280:
	movs r0, #220
	bl Func_020059f4
	movs r0, #192
	movs r1, #192
	movs r2, #128
	lsls r0, r0, #11
	lsls r1, r1, #11
	lsls r2, r2, #9
	bl Func_020057ac
	ldr r0, .L_0200836c
	b .L_0200829e
.L_0200829a:
	movs r0, #128
	lsls r0, r0, #9
.L_0200829e:
	movs r1, #1
	bl Func_02005954
	movs r0, #8
	bl Func_02005964
	b .L_02008390
.L_020082ac:
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r0, r0, #9
	lsls r1, r1, #9
	lsls r2, r2, #9
	bl Func_020057ac
	b .L_02008390
.L_020082be:
	movs r3, #152
	lsls r3, r3, #17
	str r3, [r6, #8]
	ldr r3, .L_02008370
	adds r0, r6, #0
	str r3, [r6, #12]
	movs r3, #164
	lsls r3, r3, #16
	str r3, [r6, #16]
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r6, #24]
	str r3, [r6, #28]
	bl SceneActor_ParkRecord
	ldr r1, .L_02008374
	movs r0, #36
	bl Actor_SetMovementTypeById
	b .L_02008390
.L_020082e6:
	ldr r3, [r5]
	subs r3, #1
	str r3, [r5]
	ldr r3, [r6, #12]
	cmp r3, #0
	ble .L_02008326
	ldr r0, .L_02008378
	movs r1, #0
	bl Func_02005954
	movs r0, #16
	b .L_0200831a
.L_020082fe:
	ldr r3, [r5]
	movs r2, #160
	subs r3, #1
	str r3, [r5]
	lsls r2, r2, #14
	ldr r3, [r6, #12]
	cmp r3, r2
	ble .L_02008326
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #0
	bl Func_02005954
	movs r0, #40
.L_0200831a:
	bl Func_02005964
	ldr r3, [r5]
	adds r3, #1
	str r3, [r5]
	b .L_02008390
.L_02008326:
	ldr r3, .L_0200837c
	movs r2, #7
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	bne .L_02008338
	movs r0, #246
	bl Func_020059f4
.L_02008338:
	ldr r3, [r6, #12]
	movs r1, #144
	lsls r1, r1, #10
	adds r3, r3, r1
	movs r7, #1
	str r3, [r6, #12]
	b .L_02008390
.L_02008346:
	movs r7, #1
	b .L_02008390
.L_0200834a:
	movs r0, #187
	bl Func_020059f4
	movs r0, #254
	lsls r0, r0, #7
	adds r0, #255
	movs r1, #0
	bl Func_02005954
	movs r0, #12
	bl Func_02005964
	b .L_02008390
.L_02008364:
	.4byte gData_02006a24
.L_02008368:
	.4byte .L_020081bc
.L_0200836c:
	.4byte 0x002063ff
.L_02008370:
	.4byte 0xfe980000
.L_02008374:
	.4byte gData_0200641c
.L_02008378:
	.4byte 0x00203210
.L_0200837c:
	.4byte gFrameCount
.L_02008380:
	movs r0, #36
	bl Actor_Stop
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #2
	bl GameFlag_SetBit
.L_02008390:
	cmp r7, #0
	beq .L_02008448
	bl Random16Far
	lsls r3, r0, #2
	adds r3, r3, r0
	ldr r2, [r6, #12]
	lsls r3, r3, #4
	lsrs r3, r3, #16
	lsls r3, r3, #16
	subs r2, r2, r3
	ldr r3, .L_02008428
	movs r0, #168
	adds r2, r2, r3
	ldr r1, [r6, #8]
	ldr r3, [r6, #16]
	lsls r0, r0, #2
	bl Func_0200576c
	adds r7, r0, #0
	cmp r7, #0
	beq .L_02008448
	ldr r1, [r7, #80]
	movs r5, #0
	mov r10, r1
	ldr r1, .L_0200842c
	bl Func_02005764
	movs r1, #1
	adds r0, r7, #0
	bl Actor_SetPartAttribute
	adds r3, r7, #0
	adds r3, #85
	strb r5, [r3]
	bl Random16Far
	ldr r3, .L_02008430
	adds r2, r7, #0
	adds r2, #100
	ands r3, r0
	strh r3, [r2]
	ldr r1, .L_02008424
	adds r3, r7, #0
	adds r3, #102
	strh r5, [r3]
	mov r8, r1
	bl Random16Far
	adds r3, r7, #0
	lsrs r0, r0, #13
	adds r3, #98
	strb r0, [r3]
	ldr r3, .L_02008434
	str r3, [r7, #108]
	bl Random16Far
	adds r3, r0, #0
	lsls r0, r3, #16
	subs r0, r0, r3
	lsrs r0, r0, #20
	bl Math_Sine
	lsls r3, r0, #1
	adds r3, r3, r0
	lsls r3, r3, #3
	str r3, [r7, #48]
	mov r1, r10
	movs r2, #50
	ldrsh r3, [r6, r2]
	str r3, [r7, #48]
	mov r3, r8
	b .L_02008438
	.2byte 0x0000
.L_02008424:
	.4byte 0x00000000
.L_02008428:
	.4byte 0xfff80000
.L_0200842c:
	.4byte gData_02006470
.L_02008430:
	.4byte 0x0ffff000
.L_02008434:
	.4byte Func_020000ec
.L_02008438:
	ldrb r2, [r1, #9]
	strb r3, [r1, #26]
	movs r3, #13
	negs r3, r3
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	strb r3, [r1, #9]
.L_02008448:
	ldr r2, .L_02008458
	ldr r3, [r2]
	adds r3, #1
	str r3, [r2]
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_02008458:
	.4byte gData_02006a24
	.section .text.x020084c8,"ax",%progbits
	.global Func_020004c8
	.thumb_func
Func_020004c8:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r0, #129
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020084ea
	ldr r3, .L_0200850c
	movs r1, #3
	ldr r0, [r3]
	bl __umodsi3
	cmp r0, #0
	bne .L_020085b0
.L_020084ea:
	movs r0, #31
	bl Actor_GetById
	adds r5, r0, #0
	movs r0, #129
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008510
	bl Random16Far
	adds r2, r0, #0
	ldr r3, [r5, #12]
	lsls r2, r2, #8
	b .L_0200851a
.L_0200850c:
	.4byte gFrameCount
.L_02008510:
	bl Random16Far
	adds r2, r0, #0
	ldr r3, [r5, #12]
	lsls r2, r2, #6
.L_0200851a:
	lsrs r2, r2, #16
	lsls r2, r2, #16
	adds r2, r2, r3
	ldr r3, .L_020085a0
	movs r0, #168
	adds r2, r2, r3
	ldr r1, [r5, #8]
	ldr r3, [r5, #16]
	lsls r0, r0, #2
	bl Func_0200576c
	adds r7, r0, #0
	cmp r7, #0
	beq .L_020085b0
	ldr r1, .L_020085a4
	adds r0, r7, #0
	ldr r6, [r7, #80]
	bl Func_02005764
	movs r1, #1
	adds r0, r7, #0
	bl Actor_SetPartAttribute
	adds r3, r7, #0
	adds r3, #85
	movs r5, #0
	strb r5, [r3]
	bl Random16Far
	ldr r3, .L_020085a8
	adds r2, r7, #0
	adds r2, #100
	ands r3, r0
	strh r3, [r2]
	adds r3, r7, #0
	adds r3, #102
	strh r5, [r3]
	ldr r3, .L_020085ac
	ldr r1, .L_0200859c
	str r3, [r7, #108]
	mov r8, r1
	bl Random16Far
	adds r3, r0, #0
	lsls r0, r3, #16
	subs r0, r0, r3
	lsrs r0, r0, #20
	bl Math_Sine
	lsls r3, r0, #1
	adds r3, r3, r0
	lsls r3, r3, #3
	asrs r3, r3, #16
	str r3, [r7, #48]
	mov r3, r8
	ldrb r2, [r6, #9]
	strb r3, [r6, #26]
	movs r3, #13
	negs r3, r3
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	strb r3, [r6, #9]
	b .L_020085b0
	.2byte 0x0000
.L_0200859c:
	.4byte 0x00000000
.L_020085a0:
	.4byte 0xffe40000
.L_020085a4:
	.4byte gData_020068b0
.L_020085a8:
	.4byte 0x0ffff000
.L_020085ac:
	.4byte Func_02000090
.L_020085b0:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
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
	.section .text.x02008cd0,"ax",%progbits
	.global Func_02000cd0
	.thumb_func
Func_02000cd0:
	push {r5, r6, r7, lr}
	movs r7, #192
	bl Func_02005814
	lsls r7, r7, #18
	movs r0, #0
	bl Func_02005984
	ldr r2, [r7, #108]
	movs r1, #214
	lsls r1, r1, #1
	movs r6, #128
	adds r3, r2, r1
	lsls r6, r6, #1
	str r6, [r3]
	movs r3, #218
	lsls r3, r3, #1
	adds r2, r2, r3
	movs r3, #120
	str r3, [r2]
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r1, r1
	negs r2, r2
	movs r3, #0
	negs r0, r0
	bl Camera_MoveWithinMapBounds
	bl Func_0200592c
	movs r3, #0
	adds r0, #85
	strb r3, [r0]
	movs r1, #192
	movs r0, #128
	movs r2, #240
	lsls r1, r1, #15
	lsls r2, r2, #16
	lsls r0, r0, #18
	bl Camera_MoveWithinMapBounds
	movs r0, #1
	bl wait
	bl Func_0200577c
	movs r0, #1
	bl wait
	movs r5, #192
	ldr r0, .L_020090cc
	bl Func_020058c4
	lsls r5, r5, #8
	movs r1, #254
	movs r2, #139
	adds r3, r5, #0
	movs r0, #4
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_0200588c
	movs r1, #133
	movs r2, #139
	adds r3, r5, #0
	lsls r1, r1, #18
	lsls r2, r2, #17
	movs r0, #7
	bl Func_0200588c
	movs r0, #141
	bl Func_020059f4
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r0, r0, #11
	lsls r1, r1, #11
	lsls r2, r2, #9
	bl Func_020057ac
	bl Event_StartScreenTransition
	bl Event_WaitTransitionFrames
	movs r0, #192
	movs r1, #192
	movs r2, #128
	lsls r1, r1, #10
	lsls r2, r2, #9
	lsls r0, r0, #10
	bl Func_020057ac
	movs r0, #60
	bl FieldEvent_Wait
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #10
	lsls r2, r2, #9
	lsls r0, r0, #10
	bl Func_020057ac
	movs r0, #60
	bl FieldEvent_Wait
	movs r0, #195
	lsls r0, r0, #1
	bl Func_020059f4
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	adds r2, #102
	negs r0, r0
	negs r1, r1
	bl Func_020057ac
	bl Func_020057b4
	movs r0, #204
	movs r1, #192
	lsls r0, r0, #7
	lsls r1, r1, #4
	adds r0, #102
	adds r1, #204
	bl Func_02005914
	movs r0, #128
	movs r1, #192
	movs r2, #148
	lsls r1, r1, #15
	lsls r2, r2, #17
	movs r3, #1
	lsls r0, r0, #18
	bl Camera_MoveWithinMapBounds
	bl Func_02005924
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #7
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #4
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #7
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #7
	bl Actor_ArmCallback
	movs r1, #128
	lsls r1, r1, #7
	movs r0, #4
	bl Func_020058ec
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #4
	movs r1, #0
	movs r2, #8
	bl Actor_OffsetPositionAndReset
	movs r0, #7
	movs r1, #0
	movs r2, #8
	bl Actor_OffsetPositionAndReset
	movs r2, #16
	movs r0, #27
	movs r1, #32
	negs r2, r2
	bl Actor_OffsetPositionAndReset
	movs r2, #16
	movs r1, #48
	negs r2, r2
	movs r0, #28
	bl Actor_OffsetPositionAndReset
	movs r0, #4
	bl Actor_WaitForMoveById
	movs r0, #7
	bl Actor_WaitForMoveById
	movs r0, #27
	bl Actor_WaitForMoveById
	movs r0, #28
	bl Actor_WaitForMoveById
	movs r1, #128
	movs r0, #4
	lsls r1, r1, #6
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #128
	movs r0, #7
	lsls r1, r1, #7
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #160
	movs r0, #27
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #176
	movs r2, #0
	movs r0, #28
	lsls r1, r1, #8
	bl Actor_ArmCallback
	movs r1, #2
	movs r0, #7
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl FieldEvent_Wait
	movs r0, #78
	bl Func_020059f4
	movs r0, #7
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r0, #28
	movs r1, #4
	bl Actor_SetModeById
	movs r0, #33
	bl FieldEvent_Wait
	movs r0, #34
	bl Func_020059f4
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #28
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	adds r1, r6, #0
	movs r2, #0
	movs r0, #7
	bl Func_020058fc
	adds r1, r6, #0
	movs r2, #30
	movs r0, #4
	bl Func_020058fc
	movs r0, #27
	movs r1, #4
	bl Actor_SetModeById
	movs r0, #33
	bl FieldEvent_Wait
	movs r0, #27
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r2, #0
	movs r0, #4
	movs r1, #0
	bl Actor_ArmCallback
	movs r1, #128
	lsls r1, r1, #8
	movs r0, #7
	bl Func_020058ec
	movs r0, #10
	bl FieldEvent_Wait
	movs r0, #28
	movs r1, #4
	bl Actor_SetModeById
	movs r0, #33
	bl FieldEvent_Wait
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #28
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r1, #128
	movs r2, #0
	movs r0, #4
	lsls r1, r1, #6
	bl Actor_ArmCallback
	movs r1, #128
	lsls r1, r1, #7
	movs r0, #7
	bl Func_020058ec
	movs r0, #10
	bl FieldEvent_Wait
	movs r0, #27
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #27
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r1, #129
	movs r2, #30
	lsls r1, r1, #1
	movs r0, #7
	bl Func_020058fc
	movs r1, #2
	movs r0, #28
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl FieldEvent_Wait
	movs r0, #128
	lsls r0, r0, #5
	movs r1, #0
	adds r0, #28
	bl FieldMessage_ShowAtActor
	movs r0, #4
	movs r1, #0
	bl Event_AskYesNoWithActorAnimation
	cmp r0, #0
	bne .L_02008fc4
	movs r0, #128
	lsls r0, r0, #5
	movs r1, #0
	movs r2, #0
	adds r0, #28
	bl Func_020058d4
	ldr r2, [r7, #108]
	movs r1, #226
	lsls r1, r1, #1
	adds r2, r2, r1
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_02008fe0
.L_02008fc4:
	ldr r2, [r7, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #128
	adds r3, #1
	lsls r0, r0, #5
	strh r3, [r2]
	adds r0, #28
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
.L_02008fe0:
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #7
	bl Func_020058fc
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #4
	bl Func_020058fc
	movs r1, #128
	movs r2, #0
	movs r0, #7
	lsls r1, r1, #8
	bl Actor_ArmCallback
	movs r1, #0
	movs r0, #4
	bl Func_020058ec
	movs r0, #10
	bl FieldEvent_Wait
	movs r1, #16
	movs r2, #24
	movs r0, #28
	negs r1, r1
	negs r2, r2
	bl Actor_CommitPositionAndActivate
	movs r1, #208
	movs r0, #28
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #128
	movs r0, #4
	lsls r1, r1, #7
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #7
	bl Func_020058fc
	movs r0, #28
	bl Actor_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	strb r3, [r0]
	movs r0, #28
	bl Actor_GetById
	movs r3, #128
	lsls r3, r3, #11
	str r3, [r0, #40]
	movs r1, #0
	movs r2, #24
	movs r0, #28
	bl Actor_OffsetPositionAndResetMotion
	movs r0, #28
	bl Actor_WaitForMoveById
	movs r0, #28
	bl Actor_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	movs r1, #160
	strb r3, [r0]
	lsls r1, r1, #8
	movs r0, #28
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #128
	movs r0, #7
	lsls r1, r1, #7
	movs r2, #0
	bl Actor_ArmCallback
	movs r0, #7
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r1, #226
	lsls r1, r1, #1
	adds r3, r3, r1
	ldrh r0, [r3]
	movs r1, #5
	adds r2, r0, #1
	lsls r0, r0, #16
	strh r2, [r3]
	asrs r0, r0, #16
	bl FieldMessage_Show
	movs r0, #224
	bl PartyInventory_Add
	movs r5, #0
	movs r6, #216
	b .L_020090d4
	.2byte 0x0000
.L_020090cc:
	.4byte MsgWeHaveIgnited
.L_020090d0:
	adds r6, #2
	adds r5, #1
.L_020090d4:
	cmp r5, #14
	bgt .L_020090f8
	movs r0, #4
	bl Unit_Get
	ldr r3, .L_020090f4
	ldrh r2, [r0, r6]
	ands r3, r2
	cmp r3, #224
	bne .L_020090d0
	movs r0, #4
	adds r1, r5, #0
	bl Func_020057ec
	b .L_020090f8
	.2byte 0x0000
.L_020090f4:
	.4byte 0x000001ff
.L_020090f8:
	movs r1, #2
	movs r0, #4
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl FieldEvent_Wait
	movs r0, #4
	movs r1, #0
	movs r2, #8
	bl Actor_OffsetPositionAndReset
	movs r1, #24
	movs r2, #8
	negs r1, r1
	negs r2, r2
	movs r0, #27
	bl Actor_CommitPositionAndActivate
	movs r0, #4
	bl Actor_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r5, #254
	adds r3, r5, #0
	ands r3, r2
	movs r2, #8
	strb r3, [r0]
	movs r1, #0
	movs r0, #4
	negs r2, r2
	bl Actor_OffsetPositionAndResetMotion
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #28
	bl Func_020058fc
	movs r1, #0
	movs r2, #0
	movs r0, #28
	bl Func_020058d4
	movs r0, #4
	bl Actor_WaitForMoveById
	movs r0, #4
	bl Actor_GetById
	adds r0, #90
	ldrb r3, [r0]
	movs r1, #10
	ands r5, r3
	movs r2, #30
	strb r5, [r0]
	adds r1, #255
	movs r0, #4
	bl Func_020058fc
	movs r1, #128
	movs r0, #7
	lsls r1, r1, #8
	bl Func_020058ec
	movs r0, #7
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #27
	bl Func_020058fc
	movs r1, #208
	movs r0, #27
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r0, #128
	lsls r0, r0, #6
	movs r2, #0
	adds r0, #27
	movs r1, #0
	bl Func_020058d4
	movs r1, #128
	lsls r1, r1, #7
	movs r0, #7
	bl Func_020058ec
	movs r0, #10
	bl FieldEvent_Wait
	movs r1, #129
	movs r0, #7
	lsls r1, r1, #1
	bl Func_02005904
	ldr r3, .L_02009380
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	movs r1, #129
	ldr r0, [r3]
	lsls r1, r1, #1
	bl Func_02005904
	movs r0, #30
	bl FieldEvent_Wait
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #28
	bl Func_020058fc
	movs r1, #176
	movs r0, #28
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #0
	movs r0, #28
	bl FieldMessage_ShowAtActor
	movs r0, #4
	movs r1, #0
	bl Event_AskYesNoWithActorAnimation
	cmp r0, #0
	bne .L_02009236
	movs r1, #3
	movs r0, #28
	bl Actor_SetModeById
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #28
	movs r2, #0
	movs r1, #0
	bl Func_020058d4
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_02009260
.L_02009236:
	movs r1, #3
	movs r0, #28
	bl Actor_SetModeById
	movs r0, #20
	bl FieldEvent_Wait
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r1, #226
	lsls r1, r1, #1
	adds r2, r2, r1
	ldrh r3, [r2]
	movs r0, #28
	adds r3, #1
	strh r3, [r2]
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
.L_02009260:
	movs r1, #2
	movs r0, #7
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl FieldEvent_Wait
	movs r0, #7
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r1, #10
	adds r1, #255
	movs r2, #30
	movs r0, #27
	bl Func_020058fc
	movs r1, #208
	movs r0, #27
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #27
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #4
	bl Func_020058fc
	movs r1, #208
	movs r0, #28
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r0, #28
	movs r1, #4
	bl Actor_SetModeById
	movs r0, #33
	bl FieldEvent_Wait
	movs r0, #28
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r1, #2
	adds r1, #255
	movs r2, #30
	movs r0, #4
	bl Func_020058fc
	movs r1, #176
	movs r0, #27
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r0, #27
	movs r1, #4
	bl Actor_SetModeById
	movs r0, #33
	bl FieldEvent_Wait
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #27
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r0, #28
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #28
	movs r2, #0
	movs r1, #0
	bl Func_020058d4
	movs r0, #4
	movs r1, #2
	bl Actor_StartRepeatedMotion
	movs r1, #2
	movs r0, #7
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl FieldEvent_Wait
	movs r0, #27
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #27
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	ldr r3, .L_02009380
	movs r2, #166
	lsls r2, r2, #1
	adds r2, #255
	adds r3, r3, r2
	movs r2, #2
	strb r2, [r3]
	ldr r5, .L_02009384
	movs r1, #10
	adds r0, r5, #0
	bl Party_SetFields1eeAnd1f0
	adds r0, r5, #0
	movs r1, #10
	bl Party_SetFields1f2And1f4
	movs r0, #28
	adds r0, #255
	bl GameFlag_SetBit
	movs r0, #13
	movs r1, #5
	bl Func_0200593c
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009380:
	.4byte gMainRoutineWork
.L_02009384:
	.4byte 0x00000109
	.section .text.x02009388,"ax",%progbits
	.global Func_02001388
	.thumb_func
Func_02001388:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	bl Func_02005814
	movs r6, #160
	movs r0, #0
	bl Func_02005984
	lsls r6, r6, #8
	movs r1, #128
	movs r2, #156
	adds r3, r6, #0
	movs r0, #4
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl Func_0200588c
	movs r1, #132
	movs r2, #152
	adds r3, r6, #0
	movs r0, #7
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl Func_0200588c
	movs r2, #128
	lsls r2, r2, #6
	mov r8, r2
	movs r1, #252
	movs r2, #152
	mov r3, r8
	movs r0, #27
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_0200588c
	movs r1, #130
	movs r2, #144
	mov r3, r8
	movs r0, #28
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl Func_0200588c
	movs r3, #192
	movs r1, #138
	movs r2, #196
	lsls r3, r3, #8
	lsls r2, r2, #17
	movs r0, #5
	lsls r1, r1, #18
	bl Func_0200588c
	movs r0, #27
	movs r1, #9
	bl Actor_SetModeById
	movs r0, #28
	movs r1, #8
	bl Actor_SetModeById
	movs r0, #4
	movs r1, #50
	bl Actor_SetModeById
	movs r0, #7
	movs r1, #12
	bl Actor_SetModeById
	movs r0, #130
	movs r1, #208
	movs r2, #156
	movs r3, #0
	lsls r1, r1, #15
	lsls r2, r2, #17
	lsls r0, r0, #18
	bl Camera_MoveWithinMapBounds
	bl Event_StartScreenTransition
	bl Event_WaitTransitionFrames
	ldr r0, .L_020096dc
	bl Func_020058c4
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #4
	bl Func_020058fc
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #7
	bl Func_020058fc
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #28
	bl Func_020058fc
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #27
	bl Func_020058fc
	movs r0, #4
	bl Actor_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r5, #254
	adds r3, r5, #0
	ands r3, r2
	strb r3, [r0]
	movs r0, #7
	bl Actor_GetById
	adds r0, #90
	ldrb r2, [r0]
	adds r3, r5, #0
	ands r3, r2
	strb r3, [r0]
	movs r0, #27
	bl Actor_GetById
	adds r0, #90
	ldrb r2, [r0]
	adds r3, r5, #0
	ands r3, r2
	strb r3, [r0]
	movs r0, #28
	bl Actor_GetById
	adds r0, #90
	ldrb r3, [r0]
	ands r5, r3
	strb r5, [r0]
	movs r0, #4
	bl Actor_GetById
	movs r5, #192
	lsls r5, r5, #10
	str r5, [r0, #40]
	movs r0, #7
	bl Actor_GetById
	str r5, [r0, #40]
	movs r0, #27
	bl Actor_GetById
	str r5, [r0, #40]
	movs r0, #28
	bl Actor_GetById
	movs r1, #8
	str r5, [r0, #40]
	movs r2, #4
	movs r0, #4
	bl Actor_OffsetPositionAndResetMotion
	movs r0, #7
	movs r1, #8
	movs r2, #4
	bl Actor_OffsetPositionAndResetMotion
	movs r1, #16
	movs r2, #8
	movs r0, #27
	negs r1, r1
	negs r2, r2
	bl Actor_OffsetPositionAndResetMotion
	movs r1, #16
	movs r2, #8
	negs r2, r2
	negs r1, r1
	movs r0, #28
	bl Actor_OffsetPositionAndResetMotion
	movs r0, #4
	bl Actor_WaitForMoveById
	movs r0, #4
	bl Actor_GetById
	adds r0, #90
	ldrb r3, [r0]
	movs r5, #1
	orrs r3, r5
	strb r3, [r0]
	movs r0, #7
	bl Actor_GetById
	adds r0, #90
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
	movs r0, #27
	bl Actor_GetById
	adds r0, #90
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
	movs r0, #28
	bl Actor_GetById
	adds r0, #90
	ldrb r3, [r0]
	movs r1, #1
	orrs r5, r3
	strb r5, [r0]
	movs r0, #4
	bl Camera_FollowActor
	movs r1, #138
	movs r2, #172
	movs r0, #5
	lsls r1, r1, #2
	lsls r2, r2, #1
	bl Actor_SetPositionAndReset
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #5
	bl Func_020058fc
	movs r2, #0
	movs r0, #5
	movs r1, #0
	bl Func_020058d4
	movs r1, #128
	lsls r1, r1, #7
	movs r0, #7
	bl Func_020058ec
	movs r0, #10
	bl FieldEvent_Wait
	movs r2, #0
	movs r0, #7
	movs r1, #0
	bl Func_020058d4
	movs r1, #192
	movs r0, #28
	lsls r1, r1, #7
	bl Func_020058ec
	movs r1, #8
	adds r1, #255
	movs r2, #30
	movs r0, #28
	bl Func_020058fc
	movs r0, #28
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r0, #27
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r2, #0
	mov r1, r8
	movs r0, #28
	bl Actor_ArmCallback
	mov r1, r8
	movs r0, #27
	bl Func_020058ec
	adds r1, r6, #0
	movs r0, #5
	bl Func_020058ec
	movs r0, #10
	bl FieldEvent_Wait
	movs r0, #5
	movs r1, #0
	bl Func_020058dc
	movs r1, #8
	movs r2, #24
	negs r2, r2
	movs r0, #5
	negs r1, r1
	bl Actor_OffsetPositionAndReset
	movs r0, #4
	mov r1, r8
	bl Func_020058ec
	movs r0, #4
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #7
	adds r1, r6, #0
	movs r2, #0
	bl Actor_ArmCallback
	movs r0, #4
	adds r1, r6, #0
	bl Func_020058ec
	adds r1, r6, #0
	movs r0, #5
	bl Func_020058ec
	movs r0, #10
	bl FieldEvent_Wait
	movs r1, #16
	movs r2, #8
	movs r0, #4
	negs r1, r1
	negs r2, r2
	bl Actor_OffsetPositionAndResetMotion
	movs r1, #16
	movs r2, #8
	movs r0, #7
	negs r1, r1
	negs r2, r2
	bl Actor_OffsetPositionAndResetMotion
	movs r1, #16
	movs r2, #8
	movs r0, #5
	negs r1, r1
	negs r2, r2
	bl Actor_OffsetPositionAndResetMotion
	movs r0, #27
	movs r1, #16
	movs r2, #8
	bl Actor_OffsetPositionAndResetMotion
	movs r2, #8
	movs r1, #16
	movs r0, #28
	bl Actor_OffsetPositionAndResetMotion
	movs r0, #4
	bl Actor_GetById
	movs r5, #128
	lsls r5, r5, #11
	str r5, [r0, #40]
	movs r0, #7
	bl Actor_GetById
	str r5, [r0, #40]
	movs r0, #5
	bl Actor_GetById
	str r5, [r0, #40]
	movs r0, #27
	bl Actor_GetById
	str r5, [r0, #40]
	movs r0, #28
	bl Actor_GetById
	str r5, [r0, #40]
	movs r0, #4
	bl FieldEvent_Wait
	ldr r5, .L_020096e0
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #86
	adds r3, r5, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	movs r2, #1
	negs r2, r2
	cmp r3, r2
	bne .L_020096ae
	movs r1, #10
	movs r0, #4
	negs r1, r1
	bl Unit_AddHp
.L_020096ae:
	movs r2, #166
	lsls r2, r2, #1
	adds r2, #255
	adds r3, r5, r2
	movs r2, #2
	strb r2, [r3]
	ldr r5, .L_020096e4
	movs r1, #11
	adds r0, r5, #0
	bl Party_SetFields1eeAnd1f0
	adds r0, r5, #0
	movs r1, #11
	bl Party_SetFields1f2And1f4
	movs r0, #13
	movs r1, #6
	bl Func_0200593c
	pop {r3}
	mov r8, r3
	pop {r5, r6, pc}
	.2byte 0x0000
.L_020096dc:
	.4byte MsgBlank5
.L_020096e0:
	.4byte gMainRoutineWork
.L_020096e4:
	.4byte 0x00000109
	.section .text.x02009bc4,"ax",%progbits
	.global Func_02001bc4
	.thumb_func
Func_02001bc4:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r9
	push {r5, r6}
	mov r6, r8
	push {r6}
	bl Func_02005814
	movs r0, #0
	bl Func_02005984
	movs r0, #0
	bl Actor_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r5, #1
	orrs r3, r5
	strb r3, [r0]
	movs r0, #1
	bl Actor_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r6, #192
	orrs r3, r5
	strb r3, [r0]
	movs r0, #2
	bl Actor_GetById
	adds r0, #35
	ldrb r3, [r0]
	lsls r6, r6, #8
	orrs r3, r5
	strb r3, [r0]
	movs r0, #3
	bl Actor_GetById
	adds r0, #35
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
	movs r0, #4
	bl Actor_GetById
	adds r0, #35
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
	movs r0, #5
	bl Actor_GetById
	adds r0, #35
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
	movs r0, #7
	bl Actor_GetById
	adds r0, #35
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
	movs r0, #6
	bl Actor_GetById
	adds r0, #35
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
	movs r0, #30
	bl Actor_GetById
	adds r0, #35
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
	movs r0, #27
	bl Actor_GetById
	adds r0, #35
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
	movs r0, #28
	bl Actor_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r1, #252
	orrs r5, r3
	movs r2, #152
	movs r3, #160
	strb r5, [r0]
	lsls r3, r3, #8
	movs r0, #4
	lsls r1, r1, #17
	lsls r2, r2, #17
	mov r10, r3
	bl Func_0200588c
	movs r1, #132
	movs r2, #144
	mov r3, r10
	movs r0, #7
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl Func_0200588c
	movs r3, #128
	movs r1, #244
	movs r2, #148
	lsls r3, r3, #6
	movs r0, #27
	lsls r1, r1, #17
	lsls r2, r2, #17
	mov r8, r3
	bl Func_0200588c
	movs r1, #252
	movs r2, #140
	mov r3, r8
	movs r0, #28
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_0200588c
	movs r1, #132
	movs r2, #152
	mov r3, r10
	movs r0, #5
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl Func_0200588c
	movs r1, #138
	movs r2, #144
	adds r3, r6, #0
	movs r0, #6
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl Func_0200588c
	movs r1, #240
	movs r2, #180
	adds r3, r6, #0
	movs r0, #29
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_0200588c
	movs r1, #148
	movs r2, #196
	lsls r2, r2, #17
	movs r3, #0
	movs r0, #30
	lsls r1, r1, #18
	bl Func_0200588c
	movs r0, #5
	movs r1, #19
	bl Actor_SetModeById
	movs r0, #6
	movs r1, #19
	bl Actor_SetModeById
	movs r0, #4
	movs r1, #19
	bl Actor_SetModeById
	movs r1, #13
	movs r0, #7
	bl Actor_SetModeById
	movs r0, #4
	bl Actor_GetById
	ldr r3, [r0, #24]
	negs r3, r3
	str r3, [r0, #24]
	movs r0, #6
	bl Actor_GetById
	ldr r3, [r0, #24]
	movs r1, #208
	negs r3, r3
	str r3, [r0, #24]
	movs r2, #156
	movs r0, #130
	movs r3, #0
	lsls r2, r2, #17
	lsls r1, r1, #15
	lsls r0, r0, #18
	bl Camera_MoveWithinMapBounds
	bl Event_StartScreenTransition
	bl Event_WaitTransitionFrames
	ldr r0, .L_0200a150
	bl Func_020058c4
	movs r0, #27
	movs r1, #9
	bl Actor_SetModeById
	movs r0, #28
	movs r1, #8
	bl Actor_SetModeById
	movs r1, #2
	movs r0, #28
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl FieldEvent_Wait
	movs r2, #0
	movs r0, #28
	movs r1, #0
	bl Func_020058d4
	movs r1, #224
	movs r0, #27
	lsls r1, r1, #8
	bl Func_020058ec
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #27
	bl Func_020058fc
	movs r2, #0
	movs r0, #27
	movs r1, #0
	bl Func_020058d4
	movs r1, #2
	movs r0, #4
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl FieldEvent_Wait
	mov r1, r8
	movs r0, #28
	bl Func_020058ec
	mov r1, r8
	movs r0, #27
	bl Func_020058ec
	movs r0, #10
	bl FieldEvent_Wait
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #27
	bl Func_020058fc
	movs r1, #129
	movs r2, #30
	lsls r1, r1, #1
	movs r0, #28
	bl Func_020058fc
	movs r0, #28
	movs r1, #2
	bl Motion_SetVarCbAndRefresh
	movs r0, #28
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r0, #27
	movs r1, #4
	bl Actor_SetModeById
	movs r0, #33
	bl FieldEvent_Wait
	movs r0, #27
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r1, #8
	adds r1, #255
	movs r2, #30
	movs r0, #28
	bl Func_020058fc
	movs r0, #28
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r1, #0
	movs r2, #0
	movs r0, #29
	bl Func_020058d4
	movs r0, #78
	bl Func_020059f4
	movs r1, #128
	movs r2, #0
	movs r0, #27
	lsls r1, r1, #7
	bl Actor_ArmCallback
	movs r1, #128
	lsls r1, r1, #7
	movs r0, #28
	bl Func_020058ec
	movs r0, #10
	bl FieldEvent_Wait
	movs r0, #130
	movs r1, #208
	movs r2, #172
	movs r3, #1
	lsls r1, r1, #15
	lsls r2, r2, #17
	lsls r0, r0, #18
	bl Camera_MoveWithinMapBounds
	bl Func_02005924
	movs r0, #10
	bl FieldEvent_Wait
	movs r0, #39
	bl Func_020059f4
	movs r1, #8
	adds r1, #255
	movs r2, #30
	movs r0, #27
	bl Func_020058fc
	movs r0, #27
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r0, #29
	movs r1, #4
	bl Actor_SetModeById
	movs r0, #33
	bl FieldEvent_Wait
	movs r0, #29
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r1, #8
	adds r1, #255
	movs r2, #30
	movs r0, #28
	bl Func_020058fc
	movs r0, #28
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #29
	bl Func_020058fc
	movs r2, #0
	movs r0, #29
	movs r1, #0
	bl Func_020058d4
	movs r0, #28
	movs r1, #1
	bl Actor_SetModeById
	movs r1, #192
	movs r2, #0
	movs r0, #28
	lsls r1, r1, #7
	bl Actor_ArmCallback
	movs r1, #224
	lsls r1, r1, #8
	movs r0, #27
	bl Func_020058ec
	movs r0, #10
	bl FieldEvent_Wait
	movs r0, #29
	movs r1, #4
	bl Actor_SetModeById
	movs r0, #33
	bl FieldEvent_Wait
	movs r0, #29
	movs r2, #0
	movs r1, #0
	bl Func_020058d4
	movs r0, #29
	movs r1, #0
	bl Func_020058ec
	movs r0, #29
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r2, #0
	movs r0, #30
	movs r1, #0
	bl Func_020058d4
	movs r0, #28
	movs r1, #2
	bl Actor_StartRepeatedMotion
	movs r0, #27
	movs r1, #2
	bl Motion_SetVarCbAndRefresh
	movs r2, #0
	mov r1, r8
	movs r0, #28
	bl Actor_ArmCallback
	mov r1, r8
	movs r0, #27
	bl Func_020058ec
	movs r0, #5
	bl FieldEvent_Wait
	movs r0, #29
	movs r1, #4
	bl Actor_SetModeById
	movs r0, #33
	bl FieldEvent_Wait
	movs r0, #29
	adds r1, r6, #0
	bl Func_020058ec
	movs r0, #29
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #28
	bl Func_020058fc
	movs r1, #128
	movs r0, #28
	lsls r1, r1, #7
	movs r2, #0
	bl Actor_ArmCallback
	movs r0, #28
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r0, #29
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #29
	movs r2, #0
	movs r1, #0
	bl Func_020058d4
	movs r1, #192
	lsls r1, r1, #7
	movs r0, #28
	bl Func_020058ec
	movs r0, #10
	bl FieldEvent_Wait
	movs r1, #224
	movs r0, #27
	lsls r1, r1, #8
	bl Func_020058ec
	movs r0, #27
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #27
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r0, #29
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #10
	bl FieldEvent_Wait
	movs r2, #0
	movs r0, #29
	movs r1, #0
	bl Func_020058d4
	movs r0, #27
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #28
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #10
	bl FieldEvent_Wait
	movs r0, #27
	movs r1, #1
	bl Camera_FollowActor
	ldr r5, .L_0200a154
	movs r0, #27
	adds r1, r5, #0
	bl Actor_SetMovementTypeById
	movs r0, #30
	bl FieldEvent_Wait
	adds r1, r5, #0
	movs r0, #29
	bl Actor_SetMovementTypeById
	adds r1, r5, #0
	movs r0, #28
	bl Actor_SetMovementTypeById
	movs r0, #29
	bl Actor_RefreshSelectorById
	movs r0, #29
	bl Actor_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r5, #64
	orrs r3, r5
	strb r3, [r0]
	movs r0, #27
	bl Actor_GetById
	adds r0, #35
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
	movs r0, #28
	bl Actor_GetById
	adds r0, #35
	ldrb r3, [r0]
	orrs r5, r3
	strb r5, [r0]
	movs r0, #29
	bl Actor_GetById
	ldr r5, .L_0200a158
	movs r3, #2
	adds r0, #85
	strb r3, [r0]
	adds r1, r5, #0
	movs r0, #29
	bl Actor_SetMovementTypeById
	movs r0, #27
	bl Actor_RefreshSelectorById
	adds r1, r5, #0
	movs r0, #27
	bl Actor_SetMovementTypeById
	movs r0, #28
	bl Actor_RefreshSelectorById
	adds r1, r5, #0
	movs r0, #28
	bl Actor_SetMovementTypeById
	movs r0, #29
	bl Actor_RefreshSelectorById
	movs r1, #56
	movs r2, #128
	movs r0, #29
	adds r1, #255
	lsls r2, r2, #1
	bl Actor_SetPositionAndReset
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #29
	bl Actor_ArmCallback
	movs r0, #27
	bl Actor_RefreshSelectorById
	movs r1, #166
	movs r2, #133
	movs r0, #27
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl Actor_SetPositionAndReset
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #27
	bl Actor_ArmCallback
	movs r0, #28
	bl Actor_RefreshSelectorById
	movs r1, #145
	movs r2, #133
	movs r0, #28
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl Actor_SetPositionAndReset
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #28
	bl Actor_ArmCallback
	movs r0, #29
	bl Actor_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r5, #32
	orrs r3, r5
	strb r3, [r0]
	movs r0, #27
	bl Actor_GetById
	adds r0, #35
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
	movs r0, #28
	bl Actor_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r1, #1
	orrs r5, r3
	strb r5, [r0]
	movs r2, #1
	movs r0, #1
	movs r3, #0
	negs r1, r1
	negs r2, r2
	negs r0, r0
	bl Camera_MoveWithinMapBounds
	movs r0, #78
	bl Func_020059f4
	movs r0, #30
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r1, #128
	movs r2, #164
	adds r3, r6, #0
	lsls r1, r1, #18
	b .L_0200a15c
.L_0200a150:
	.4byte MsgWasnTThe
.L_0200a154:
	.4byte gData_02005ff8
.L_0200a158:
	.4byte gData_02006088
.L_0200a15c:
	lsls r2, r2, #17
	movs r0, #30
	bl Func_0200588c
	movs r0, #0
	bl Func_02003a6c
	bl Func_0200598c
	movs r0, #10
	bl FieldEvent_Wait
	movs r1, #134
	movs r2, #184
	adds r3, r6, #0
	movs r0, #0
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl Func_0200588c
	movs r1, #134
	movs r2, #192
	adds r3, r6, #0
	movs r0, #1
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl Func_0200588c
	movs r1, #138
	movs r2, #184
	adds r3, r6, #0
	movs r0, #2
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl Func_0200588c
	movs r1, #138
	movs r2, #176
	adds r3, r6, #0
	movs r0, #3
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl Func_0200588c
	movs r2, #0
	movs r0, #0
	movs r1, #0
	bl Func_020058d4
	movs r0, #0
	movs r1, #1
	bl Camera_FollowActor
	movs r1, #134
	movs r2, #168
	movs r0, #0
	lsls r1, r1, #2
	lsls r2, r2, #1
	bl Actor_ResetAndSetPositionInMode2
	movs r1, #134
	movs r2, #176
	movs r0, #1
	lsls r1, r1, #2
	lsls r2, r2, #1
	bl Actor_ResetAndSetPositionInMode2
	movs r1, #138
	movs r2, #168
	movs r0, #2
	lsls r1, r1, #2
	lsls r2, r2, #1
	bl Actor_ResetAndSetPositionInMode2
	movs r1, #138
	movs r2, #160
	lsls r2, r2, #1
	lsls r1, r1, #2
	movs r0, #3
	bl Actor_ResetAndSetPositionInMode2
	movs r0, #0
	bl Actor_WaitForMoveById
	movs r0, #1
	bl Actor_WaitForMoveById
	movs r0, #2
	bl Actor_WaitForMoveById
	movs r0, #3
	bl Actor_WaitForMoveById
	movs r1, #192
	lsls r1, r1, #6
	movs r0, #30
	bl Func_020058ec
	movs r0, #10
	bl FieldEvent_Wait
	movs r0, #30
	movs r1, #4
	bl Actor_SetModeById
	movs r0, #33
	bl FieldEvent_Wait
	movs r0, #30
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r0, #2
	movs r1, #4
	bl Actor_SetModeById
	movs r0, #33
	bl FieldEvent_Wait
	movs r0, #2
	movs r2, #0
	movs r1, #0
	bl Func_020058d4
	movs r1, #128
	lsls r1, r1, #7
	movs r0, #3
	bl Func_020058ec
	movs r0, #10
	bl FieldEvent_Wait
	movs r2, #0
	movs r0, #3
	movs r1, #0
	bl Func_020058d4
	mov r1, r10
	movs r0, #0
	bl Func_020058ec
	movs r0, #160
	movs r1, #192
	movs r2, #180
	movs r3, #1
	lsls r1, r1, #15
	lsls r2, r2, #17
	lsls r0, r0, #17
	bl Camera_MoveWithinMapBounds
	bl Func_02005924
	movs r0, #10
	bl FieldEvent_Wait
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r0, #128
	movs r1, #192
	movs r2, #176
	lsls r1, r1, #15
	lsls r2, r2, #17
	movs r3, #1
	lsls r0, r0, #18
	bl Camera_MoveWithinMapBounds
	bl Func_02005924
	movs r0, #0
	bl Func_02003c58
	movs r0, #1
	movs r1, #4
	bl Actor_SetModeById
	movs r0, #33
	bl FieldEvent_Wait
	movs r0, #1
	movs r2, #0
	movs r1, #0
	bl Func_020058d4
	mov r1, r10
	movs r0, #2
	bl Func_020058ec
	movs r0, #10
	bl FieldEvent_Wait
	movs r1, #2
	movs r0, #2
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl FieldEvent_Wait
	movs r0, #2
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #3
	bl Func_020058fc
	movs r2, #0
	movs r0, #3
	movs r1, #0
	bl Func_020058d4
	movs r1, #2
	movs r0, #4
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl FieldEvent_Wait
	adds r1, r6, #0
	movs r0, #30
	bl Func_020058ec
	movs r1, #2
	movs r0, #30
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl FieldEvent_Wait
	movs r2, #0
	movs r1, #0
	movs r0, #30
	bl Func_020058d4
	movs r0, #10
	bl FieldEvent_Wait
	movs r1, #2
	movs r0, #5
	bl Actor_StartRepeatedMotion
	movs r0, #5
	bl FieldEvent_Wait
	movs r1, #2
	movs r0, #7
	bl Actor_StartRepeatedMotion
	movs r0, #5
	bl FieldEvent_Wait
	movs r1, #2
	movs r0, #6
	bl Motion_SetVarCbAndRefresh
	movs r0, #30
	bl FieldEvent_Wait
	movs r1, #224
	movs r0, #0
	lsls r1, r1, #8
	bl Func_020058ec
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r0, #3
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #10
	bl FieldEvent_Wait
	mov r1, r10
	movs r0, #1
	bl Func_020058ec
	movs r2, #0
	movs r0, #1
	movs r1, #0
	bl Func_020058d4
	mov r1, r8
	movs r0, #30
	bl Func_020058ec
	movs r0, #10
	bl FieldEvent_Wait
	movs r2, #0
	movs r0, #30
	movs r1, #0
	bl Func_020058d4
	adds r1, r6, #0
	movs r0, #30
	bl Func_020058ec
	movs r1, #48
	movs r0, #3
	negs r1, r1
	movs r2, #0
	bl Actor_CommitPositionAndActivate
	adds r1, r6, #0
	movs r2, #0
	movs r0, #3
	bl Actor_ArmCallback
	movs r0, #3
	movs r1, #4
	bl Func_02001ac4
	mov r1, r8
	movs r2, #0
	movs r0, #4
	bl Actor_ArmCallback
	movs r0, #4
	bl Actor_GetById
	ldr r3, [r0, #24]
	movs r1, #40
	negs r3, r3
	str r3, [r0, #24]
	movs r2, #0
	movs r0, #3
	bl Actor_CommitPositionAndActivate
	movs r2, #16
	movs r0, #3
	movs r1, #8
	negs r2, r2
	bl Actor_CommitPositionAndActivate
	adds r1, r6, #0
	movs r2, #0
	movs r0, #3
	bl Actor_ArmCallback
	movs r0, #3
	movs r1, #6
	bl Func_02001ac4
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #6
	bl Actor_ArmCallback
	movs r0, #6
	bl Actor_GetById
	ldr r3, [r0, #24]
	mov r1, r10
	negs r3, r3
	str r3, [r0, #24]
	movs r2, #0
	movs r0, #3
	bl Actor_ArmCallback
	movs r0, #3
	movs r1, #7
	bl Func_02001ac4
	movs r1, #128
	movs r0, #7
	lsls r1, r1, #7
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #128
	movs r2, #0
	movs r0, #3
	lsls r1, r1, #8
	bl Actor_ArmCallback
	movs r0, #3
	movs r1, #5
	bl Func_02001ac4
	movs r1, #128
	movs r2, #0
	movs r0, #5
	lsls r1, r1, #7
	bl Actor_ArmCallback
	movs r0, #0
	movs r1, #2
	bl Motion_SetVarCbAndRefresh
	mov r1, r10
	movs r0, #0
	bl Func_020058ec
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	mov r1, r8
	movs r0, #30
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #10
	adds r1, #255
	movs r2, #30
	movs r0, #30
	bl Func_020058fc
	movs r0, #30
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r1, #8
	adds r1, #255
	movs r2, #30
	movs r0, #1
	bl Func_020058fc
	movs r0, #1
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r1, #10
	adds r1, #255
	movs r2, #30
	movs r0, #2
	bl Func_020058fc
	movs r1, #138
	movs r2, #160
	lsls r2, r2, #1
	movs r0, #2
	lsls r1, r1, #2
	bl Actor_ResetAndSetPositionInMode2
	movs r1, #128
	movs r0, #2
	lsls r1, r1, #8
	bl Func_020058ec
	movs r0, #1
	adds r1, r6, #0
	movs r2, #0
	bl Actor_ArmCallback
	movs r0, #2
	movs r1, #4
	bl Actor_SetModeById
	movs r0, #33
	bl FieldEvent_Wait
	movs r0, #2
	movs r1, #2
	bl Motion_SetVarCbAndRefresh
	movs r2, #0
	movs r0, #2
	movs r1, #0
	bl Func_020058d4
	movs r1, #224
	movs r0, #0
	lsls r1, r1, #8
	bl Func_020058ec
	movs r1, #2
	movs r0, #0
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl FieldEvent_Wait
	movs r2, #0
	movs r0, #0
	movs r1, #0
	bl Func_020058d4
	movs r1, #192
	movs r0, #2
	lsls r1, r1, #7
	bl Func_020058ec
	movs r1, #2
	movs r0, #2
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl FieldEvent_Wait
	movs r0, #2
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r0, #5
	movs r1, #6
	movs r2, #0
	bl Actor_TurnToFaceEachOther
	movs r2, #0
	movs r1, #4
	movs r0, #7
	bl Actor_TurnToFaceEachOther
	movs r0, #10
	bl FieldEvent_Wait
	movs r0, #30
	movs r1, #0
	bl Func_020058ec
	movs r0, #30
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #10
	bl FieldEvent_Wait
	movs r0, #30
	movs r1, #0
	bl Func_020058ec
	movs r0, #30
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r1, #128
	movs r0, #4
	lsls r1, r1, #7
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #7
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #128
	movs r0, #7
	lsls r1, r1, #7
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #128
	movs r0, #6
	lsls r1, r1, #7
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #192
	movs r2, #0
	movs r0, #3
	lsls r1, r1, #7
	bl Actor_ArmCallback
	movs r1, #128
	lsls r1, r1, #7
	movs r0, #0
	bl Func_020058ec
	movs r0, #50
	bl FieldEvent_Wait
	movs r0, #1
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #10
	bl FieldEvent_Wait
	adds r1, r6, #0
	movs r0, #0
	bl Func_020058ec
	movs r0, #50
	bl FieldEvent_Wait
	movs r0, #3
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #7
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #6
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #5
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #10
	bl FieldEvent_Wait
	movs r0, #0
	mov r1, r10
	bl Func_020058ec
	movs r0, #4
	mov r1, r8
	bl Func_020058ec
	movs r0, #4
	movs r1, #0
	bl Event_AskYesNoWithActorAnimation
	cmp r0, #0
	bne .L_0200a678
	movs r1, #3
	movs r0, #0
	bl Actor_SetModeById
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #10
	bl FieldEvent_Wait
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	b .L_0200a6fe
.L_0200a678:
	movs r1, #8
	adds r1, #255
	movs r2, #30
	movs r0, #0
	bl Func_020058fc
	movs r0, #5
	movs r1, #4
	bl Actor_SetModeById
	movs r0, #33
	bl FieldEvent_Wait
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r1, #128
	adds r3, #2
	strh r3, [r2]
	movs r0, #5
	lsls r1, r1, #8
	bl Func_020058ec
	movs r2, #0
	movs r0, #5
	movs r1, #0
	bl Func_020058d4
	movs r1, #128
	lsls r1, r1, #7
	movs r0, #5
	bl Func_020058ec
	movs r0, #10
	bl FieldEvent_Wait
	movs r1, #2
	movs r0, #5
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl FieldEvent_Wait
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r0, #0
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #10
	bl FieldEvent_Wait
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
.L_0200a6fe:
	movs r0, #2
	movs r1, #2
	bl Actor_SetModeById
	movs r0, #0
	bl Actor_GetById
	cmp r0, #0
	beq .L_0200a71e
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #2
	bl Actor_ResetAndSetPosition
.L_0200a71e:
	movs r0, #3
	movs r1, #2
	bl Actor_SetModeById
	movs r0, #0
	bl Actor_GetById
	cmp r0, #0
	beq .L_0200a73e
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #3
	bl Actor_ResetAndSetPosition
.L_0200a73e:
	movs r0, #1
	movs r1, #2
	bl Actor_SetModeById
	movs r0, #0
	bl Actor_GetById
	cmp r0, #0
	beq .L_0200a75e
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #1
	bl Actor_ResetAndSetPosition
.L_0200a75e:
	ldr r5, .L_0200aad4
	movs r0, #2
	adds r1, r5, #0
	bl Actor_SetMovementTypeById
	adds r1, r5, #0
	movs r0, #3
	bl Actor_SetMovementTypeById
	adds r1, r5, #0
	movs r0, #1
	bl Actor_SetMovementTypeById
	movs r0, #2
	bl Actor_RefreshSelectorById
	movs r0, #3
	bl Actor_RefreshSelectorById
	movs r0, #1
	bl Actor_RefreshSelectorById
	movs r0, #0
	movs r1, #1
	bl Camera_FollowActor
	movs r0, #4
	movs r1, #0
	bl Actor_WatchAndSetScript
	movs r0, #7
	movs r1, #0
	bl Actor_WatchAndSetScript
	movs r0, #5
	movs r1, #0
	bl Actor_WatchAndSetScript
	movs r0, #6
	movs r1, #0
	bl Actor_WatchAndSetScript
	movs r1, #0
	movs r0, #30
	bl Actor_WatchAndSetScript
	movs r0, #10
	bl FieldEvent_Wait
	movs r1, #192
	movs r2, #192
	movs r0, #0
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl Actor_SetSpeedParameters
	movs r1, #144
	lsls r1, r1, #1
	movs r2, #108
	adds r2, #255
	adds r1, #255
	movs r0, #0
	bl Actor_SetPositionAndReset
	movs r0, #0
	bl Actor_WaitForMoveById
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	bl Func_020058ec
	movs r1, #2
	movs r0, #0
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl FieldEvent_Wait
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r1, #159
	lsls r1, r1, #1
	movs r2, #179
	movs r0, #0
	adds r1, #255
	lsls r2, r2, #1
	bl Actor_SetPositionAndReset
	movs r1, #146
	movs r2, #150
	movs r0, #0
	lsls r1, r1, #2
	lsls r2, r2, #1
	bl Actor_SetPositionAndReset
	movs r1, #146
	movs r0, #0
	lsls r1, r1, #2
	movs r2, #252
	bl Actor_SetPositionAndReset
	movs r1, #154
	movs r0, #0
	lsls r1, r1, #2
	movs r2, #247
	bl Actor_SetPositionAndReset
	movs r1, #181
	lsls r1, r1, #1
	movs r2, #156
	lsls r2, r2, #1
	adds r1, #255
	movs r0, #0
	bl Actor_SetPositionAndReset
	movs r0, #0
	bl Actor_GetById
	movs r6, #2
	adds r0, #85
	strb r6, [r0]
	ldr r1, .L_0200aad8
	movs r0, #0
	bl Actor_SetMovementTypeById
	movs r0, #0
	bl Actor_RefreshSelectorById
	movs r1, #128
	lsls r1, r1, #7
	movs r0, #0
	bl Func_020058ec
	movs r0, #30
	bl FieldEvent_Wait
	movs r0, #0
	bl Actor_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #32
	orrs r3, r2
	strb r3, [r0]
	movs r1, #0
	movs r0, #4
	movs r2, #0
	bl Actor_ArmCallback
	movs r0, #7
	movs r1, #0
	movs r2, #0
	bl Actor_ArmCallback
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl Actor_ArmCallback
	movs r0, #6
	movs r1, #0
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #0
	movs r2, #0
	movs r0, #30
	bl Actor_ArmCallback
	movs r0, #1
	bl Func_02003a6c
	movs r0, #100
	bl FieldEvent_Wait
	movs r1, #128
	movs r0, #4
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #128
	movs r0, #7
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #128
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #30
	bl Actor_ArmCallback
	movs r0, #1
	bl Func_02003c58
	movs r0, #60
	bl FieldEvent_Wait
	movs r1, #1
	movs r0, #4
	bl Camera_FollowActor
	bl Func_02005924
	movs r0, #60
	bl FieldEvent_Wait
	movs r1, #0
	movs r0, #4
	bl Func_020058ec
	movs r0, #60
	bl FieldEvent_Wait
	movs r0, #5
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #10
	bl FieldEvent_Wait
	movs r1, #224
	movs r0, #4
	lsls r1, r1, #8
	bl Func_020058ec
	movs r0, #7
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #6
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #7
	movs r1, #2
	bl Actor_SetModeById
	movs r0, #4
	bl Actor_GetById
	cmp r0, #0
	beq .L_0200a980
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #7
	bl Actor_ResetAndSetPosition
.L_0200a980:
	movs r0, #6
	movs r1, #2
	bl Actor_SetModeById
	movs r0, #4
	bl Actor_GetById
	cmp r0, #0
	beq .L_0200a9a0
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #6
	bl Actor_ResetAndSetPosition
.L_0200a9a0:
	movs r0, #5
	movs r1, #2
	bl Actor_SetModeById
	movs r0, #4
	bl Actor_GetById
	cmp r0, #0
	beq .L_0200a9c0
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #5
	bl Actor_ResetAndSetPosition
.L_0200a9c0:
	movs r0, #30
	movs r1, #2
	bl Actor_SetModeById
	movs r0, #4
	bl Actor_GetById
	cmp r0, #0
	beq .L_0200a9e0
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #30
	bl Actor_ResetAndSetPosition
.L_0200a9e0:
	adds r1, r5, #0
	movs r0, #7
	bl Actor_SetMovementTypeById
	adds r1, r5, #0
	movs r0, #6
	bl Actor_SetMovementTypeById
	adds r1, r5, #0
	movs r0, #5
	bl Actor_SetMovementTypeById
	adds r1, r5, #0
	movs r0, #30
	bl Actor_SetMovementTypeById
	movs r0, #7
	bl Actor_RefreshSelectorById
	movs r0, #6
	bl Actor_RefreshSelectorById
	movs r0, #5
	bl Actor_RefreshSelectorById
	movs r0, #30
	bl Actor_RefreshSelectorById
	movs r0, #4
	movs r1, #1
	bl Camera_FollowActor
	movs r1, #16
	movs r2, #0
	movs r0, #4
	negs r1, r1
	bl Actor_CommitPositionAndActivate
	ldr r1, .L_0200aadc
	movs r0, #4
	bl Actor_SetMovementTypeById
	movs r0, #4
	bl Actor_RefreshSelectorById
	movs r0, #4
	bl Actor_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r2, #64
	orrs r3, r2
	strb r3, [r0]
	movs r0, #29
	bl Actor_GetById
	adds r0, #85
	strb r6, [r0]
	ldr r1, .L_0200aae0
	movs r0, #4
	bl Actor_SetMovementTypeById
	movs r0, #4
	bl Actor_RefreshSelectorById
	movs r1, #56
	movs r2, #128
	lsls r2, r2, #1
	movs r0, #4
	adds r1, #255
	bl Actor_SetPositionAndReset
	movs r1, #128
	lsls r1, r1, #7
	movs r0, #4
	bl Func_020058ec
	movs r0, #10
	bl FieldEvent_Wait
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r1, r1
	negs r2, r2
	movs r3, #0
	negs r0, r0
	bl Camera_MoveWithinMapBounds
	movs r0, #2
	bl Func_02003a6c
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #108]
	movs r3, #214
	lsls r3, r3, #1
	adds r2, r1, r3
	adds r3, #84
	str r3, [r2]
	subs r3, #76
	adds r2, r1, r3
	movs r3, #24
	str r3, [r2]
	bl Event_FinishScreenTransition
	bl Event_WaitTransitionFrames
	movs r0, #0
	mov r9, sp
	bl Func_02001b04
	movs r0, #1
	bl Func_02005934
	bl Func_0200581c
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, pc}
.L_0200aad4:
	.4byte gData_02005ff0
.L_0200aad8:
	.4byte gData_020061e0
.L_0200aadc:
	.4byte gData_02005ff8
.L_0200aae0:
	.4byte gData_02006088
	.section .text.x0200aae4,"ax",%progbits
	.global Func_02002ae4
	.thumb_func
Func_02002ae4:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	bl Func_02005814
	movs r0, #0
	bl Func_02005984
	movs r0, #0
	bl Actor_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r5, #1
	orrs r3, r5
	strb r3, [r0]
	movs r0, #1
	bl Actor_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r6, #128
	orrs r3, r5
	strb r3, [r0]
	movs r0, #2
	bl Actor_GetById
	adds r0, #35
	ldrb r3, [r0]
	lsls r6, r6, #6
	orrs r3, r5
	strb r3, [r0]
	movs r0, #3
	bl Actor_GetById
	adds r0, #35
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
	movs r0, #4
	bl Actor_GetById
	adds r0, #35
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
	movs r0, #5
	bl Actor_GetById
	adds r0, #35
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
	movs r0, #7
	bl Actor_GetById
	adds r0, #35
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
	movs r0, #6
	bl Actor_GetById
	adds r0, #35
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
	movs r0, #30
	bl Actor_GetById
	adds r0, #35
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
	movs r0, #27
	bl Actor_GetById
	adds r0, #35
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
	movs r0, #28
	bl Actor_GetById
	adds r0, #35
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
	movs r0, #29
	bl Actor_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r1, #130
	orrs r5, r3
	movs r2, #152
	movs r3, #160
	strb r5, [r0]
	lsls r3, r3, #8
	movs r0, #4
	lsls r1, r1, #18
	lsls r2, r2, #17
	mov r8, r3
	bl Func_0200588c
	movs r1, #134
	movs r2, #144
	mov r3, r8
	movs r0, #7
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl Func_0200588c
	movs r1, #240
	movs r2, #148
	adds r3, r6, #0
	movs r0, #27
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_0200588c
	movs r1, #248
	movs r2, #140
	adds r3, r6, #0
	movs r0, #28
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_0200588c
	movs r1, #136
	movs r2, #152
	mov r3, r8
	movs r0, #5
	lsls r1, r1, #18
	lsls r2, r2, #17
	movs r5, #192
	bl Func_0200588c
	lsls r5, r5, #8
	movs r1, #140
	movs r2, #144
	adds r3, r5, #0
	movs r0, #6
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl Func_0200588c
	movs r1, #240
	movs r2, #180
	adds r3, r5, #0
	movs r0, #29
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_0200588c
	movs r1, #148
	movs r2, #196
	lsls r2, r2, #17
	movs r3, #0
	movs r0, #30
	lsls r1, r1, #18
	bl Func_0200588c
	movs r0, #27
	movs r1, #10
	bl Actor_SetModeById
	movs r0, #28
	movs r1, #9
	bl Actor_SetModeById
	movs r0, #128
	adds r1, r6, #0
	lsls r0, r0, #9
	bl Func_02005914
	movs r0, #130
	movs r1, #208
	movs r2, #156
	lsls r1, r1, #15
	lsls r2, r2, #17
	movs r3, #0
	lsls r0, r0, #18
	bl Camera_MoveWithinMapBounds
	bl Event_StartScreenTransition
	bl Event_WaitTransitionFrames
	ldr r0, .L_0200add8
	bl Func_020058c4
	movs r0, #7
	movs r1, #4
	bl Actor_SetModeById
	movs r0, #33
	bl FieldEvent_Wait
	movs r0, #7
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r0, #5
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #10
	bl FieldEvent_Wait
	movs r2, #0
	movs r0, #5
	movs r1, #0
	bl Func_020058d4
	movs r1, #192
	lsls r1, r1, #7
	movs r0, #6
	bl Func_020058ec
	movs r0, #10
	bl FieldEvent_Wait
	movs r2, #0
	movs r0, #6
	movs r1, #0
	bl Func_020058d4
	movs r1, #2
	movs r0, #27
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl FieldEvent_Wait
	movs r2, #0
	movs r0, #27
	movs r1, #0
	bl Func_020058d4
	movs r1, #2
	movs r0, #28
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl FieldEvent_Wait
	movs r0, #28
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r2, #0
	movs r0, #4
	movs r1, #0
	bl Actor_ArmCallback
	movs r1, #128
	lsls r1, r1, #8
	movs r0, #5
	bl Func_020058ec
	movs r0, #40
	bl FieldEvent_Wait
	movs r2, #0
	mov r1, r8
	movs r0, #4
	bl Actor_ArmCallback
	mov r1, r8
	movs r0, #5
	bl Func_020058ec
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #5
	movs r1, #4
	bl Actor_SetModeById
	movs r0, #33
	bl FieldEvent_Wait
	movs r0, #5
	movs r2, #0
	movs r1, #0
	bl Func_020058d4
	movs r1, #2
	movs r0, #27
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl FieldEvent_Wait
	movs r0, #27
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r1, #224
	movs r0, #4
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #192
	movs r0, #7
	lsls r1, r1, #7
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #224
	movs r2, #0
	movs r0, #5
	lsls r1, r1, #8
	bl Actor_ArmCallback
	movs r1, #192
	lsls r1, r1, #7
	movs r0, #6
	bl Func_020058ec
	movs r0, #30
	bl FieldEvent_Wait
	movs r0, #28
	movs r1, #2
	bl Motion_SetVarCbAndRefresh
	movs r1, #0
	movs r0, #28
	bl FieldMessage_ShowAtActor
	movs r1, #192
	movs r2, #0
	movs r0, #6
	lsls r1, r1, #7
	bl Actor_ArmCallback
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #8
	bl Func_020058ec
	movs r1, #128
	movs r0, #4
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r0, #4
	movs r1, #0
	bl Event_AskYesNoWithActorAnimation
	cmp r0, #0
	bne .L_0200addc
	movs r2, #0
	movs r0, #29
	movs r1, #0
	bl Func_020058d4
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_0200adf8
	.2byte 0x0000
.L_0200add8:
	.4byte MsgWeDidItWe
.L_0200addc:
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #29
	adds r3, #1
	strh r3, [r2]
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
.L_0200adf8:
	movs r0, #78
	bl Func_020059f4
	movs r0, #130
	movs r1, #208
	movs r2, #176
	movs r3, #1
	lsls r1, r1, #15
	lsls r2, r2, #17
	lsls r0, r0, #18
	bl Camera_MoveWithinMapBounds
	bl Func_02005924
	movs r0, #10
	bl FieldEvent_Wait
	movs r0, #39
	bl Func_020059f4
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #5
	bl Func_020058fc
	movs r1, #192
	movs r0, #5
	lsls r1, r1, #7
	movs r2, #0
	bl Actor_ArmCallback
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r0, #130
	movs r1, #208
	movs r2, #156
	movs r3, #1
	lsls r0, r0, #18
	lsls r1, r1, #15
	lsls r2, r2, #17
	bl Camera_MoveWithinMapBounds
	movs r1, #240
	movs r2, #160
	movs r0, #29
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl Actor_SetPositionAndReset
	movs r0, #29
	movs r1, #4
	bl Actor_SetModeById
	movs r0, #33
	bl FieldEvent_Wait
	movs r0, #29
	movs r2, #0
	movs r1, #0
	bl Func_020058d4
	movs r0, #29
	movs r1, #0
	bl Func_020058ec
	movs r0, #29
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r1, #10
	adds r1, #255
	movs r2, #0
	movs r0, #7
	bl Func_020058fc
	movs r1, #10
	adds r1, #255
	movs r2, #0
	movs r0, #5
	bl Func_020058fc
	movs r1, #10
	adds r1, #255
	movs r2, #0
	movs r0, #6
	bl Func_020058fc
	movs r1, #10
	movs r2, #30
	adds r1, #255
	movs r0, #4
	bl Func_020058fc
	movs r1, #192
	movs r0, #29
	lsls r1, r1, #8
	bl Func_020058ec
	movs r2, #20
	negs r2, r2
	movs r0, #29
	movs r1, #20
	bl Actor_CommitPositionAndActivate
	movs r1, #128
	movs r0, #29
	lsls r1, r1, #8
	bl Func_020058ec
	movs r0, #29
	movs r1, #27
	bl Func_02001ac4
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #6
	bl Func_020058fc
	movs r2, #0
	movs r0, #6
	movs r1, #0
	bl Func_020058d4
	movs r1, #192
	movs r0, #29
	lsls r1, r1, #8
	bl Func_020058ec
	movs r0, #29
	movs r1, #28
	bl Func_02001ac4
	movs r1, #20
	movs r2, #20
	movs r0, #29
	negs r1, r1
	bl Actor_CommitPositionAndActivate
	movs r0, #29
	movs r1, #0
	bl Func_020058ec
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #29
	bl Func_020058fc
	movs r2, #0
	movs r0, #29
	movs r1, #0
	bl Func_020058d4
	movs r0, #27
	movs r1, #2
	bl Actor_StartRepeatedMotion
	movs r1, #2
	movs r0, #28
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl FieldEvent_Wait
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #5
	bl Func_020058fc
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r0, #29
	movs r1, #4
	bl Actor_SetModeById
	movs r0, #33
	bl FieldEvent_Wait
	movs r0, #29
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r0, #7
	movs r1, #4
	bl Actor_SetModeById
	movs r0, #33
	bl FieldEvent_Wait
	movs r0, #7
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r1, #10
	adds r1, #255
	movs r2, #30
	movs r0, #29
	bl Func_020058fc
	movs r2, #0
	movs r0, #29
	movs r1, #0
	bl Func_020058d4
	movs r1, #128
	movs r0, #27
	lsls r1, r1, #7
	bl Func_020058ec
	movs r1, #8
	adds r1, #255
	movs r2, #30
	movs r0, #27
	bl Func_020058fc
	movs r2, #0
	movs r0, #27
	movs r1, #0
	bl Func_020058d4
	movs r1, #128
	movs r0, #28
	lsls r1, r1, #7
	bl Func_020058ec
	movs r1, #8
	adds r1, #255
	movs r2, #30
	movs r0, #28
	bl Func_020058fc
	movs r0, #28
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #29
	bl Func_020058fc
	movs r0, #29
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r0, #30
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r1, #224
	movs r2, #0
	movs r0, #27
	lsls r1, r1, #8
	bl Actor_ArmCallback
	movs r1, #192
	lsls r1, r1, #7
	movs r0, #28
	bl Func_020058ec
	movs r0, #10
	bl FieldEvent_Wait
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #28
	bl Func_020058fc
	movs r1, #129
	movs r2, #30
	lsls r1, r1, #1
	movs r0, #27
	bl Func_020058fc
	movs r0, #29
	movs r1, #0
	bl Func_020058ec
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #28
	bl Actor_ArmCallback
	movs r0, #10
	bl FieldEvent_Wait
	movs r0, #29
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #10
	bl FieldEvent_Wait
	movs r2, #0
	movs r0, #29
	movs r1, #0
	bl Func_020058d4
	movs r1, #2
	movs r0, #6
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl FieldEvent_Wait
	movs r0, #6
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #29
	bl Func_020058fc
	movs r0, #29
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r1, #224
	movs r2, #0
	movs r0, #5
	lsls r1, r1, #8
	bl Actor_ArmCallback
	movs r1, #192
	lsls r1, r1, #7
	movs r0, #6
	bl Func_020058ec
	movs r0, #10
	bl FieldEvent_Wait
	movs r0, #29
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #10
	bl FieldEvent_Wait
	movs r0, #29
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r1, #128
	movs r2, #0
	movs r0, #5
	lsls r1, r1, #8
	bl Actor_ArmCallback
	movs r1, #192
	movs r0, #29
	lsls r1, r1, #8
	bl Func_020058ec
	movs r0, #29
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #10
	bl FieldEvent_Wait
	movs r1, #192
	movs r2, #0
	movs r0, #28
	lsls r1, r1, #7
	bl Actor_ArmCallback
	movs r1, #224
	lsls r1, r1, #8
	movs r0, #27
	bl Func_020058ec
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #28
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #27
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #10
	bl FieldEvent_Wait
	movs r0, #27
	movs r1, #1
	bl Camera_FollowActor
	ldr r5, .L_0200b544
	movs r0, #29
	adds r1, r5, #0
	bl Actor_SetMovementTypeById
	movs r0, #30
	bl FieldEvent_Wait
	adds r1, r5, #0
	movs r0, #27
	bl Actor_SetMovementTypeById
	movs r0, #30
	bl FieldEvent_Wait
	adds r1, r5, #0
	movs r0, #28
	bl Actor_SetMovementTypeById
	movs r0, #29
	bl Actor_RefreshSelectorById
	movs r0, #29
	bl Actor_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r5, #64
	orrs r3, r5
	strb r3, [r0]
	movs r0, #27
	bl Actor_GetById
	adds r0, #35
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
	movs r0, #28
	bl Actor_GetById
	adds r0, #35
	ldrb r3, [r0]
	orrs r5, r3
	strb r5, [r0]
	movs r0, #29
	bl Actor_GetById
	ldr r5, .L_0200b548
	movs r3, #2
	adds r0, #85
	strb r3, [r0]
	adds r1, r5, #0
	movs r0, #29
	bl Actor_SetMovementTypeById
	movs r0, #27
	bl Actor_RefreshSelectorById
	adds r1, r5, #0
	movs r0, #27
	bl Actor_SetMovementTypeById
	movs r0, #28
	bl Actor_RefreshSelectorById
	adds r1, r5, #0
	movs r0, #28
	bl Actor_SetMovementTypeById
	movs r0, #29
	bl Actor_RefreshSelectorById
	movs r1, #56
	movs r2, #128
	movs r0, #29
	adds r1, #255
	lsls r2, r2, #1
	bl Actor_SetPositionAndReset
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #29
	bl Actor_ArmCallback
	movs r0, #27
	bl Actor_RefreshSelectorById
	movs r1, #166
	movs r2, #133
	movs r0, #27
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl Actor_SetPositionAndReset
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #27
	bl Actor_ArmCallback
	movs r0, #28
	bl Actor_RefreshSelectorById
	movs r1, #145
	movs r2, #133
	movs r0, #28
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl Actor_SetPositionAndReset
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #28
	bl Actor_ArmCallback
	movs r0, #29
	bl Actor_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r5, #32
	orrs r3, r5
	strb r3, [r0]
	movs r0, #27
	bl Actor_GetById
	adds r0, #35
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
	movs r0, #28
	bl Actor_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r1, #1
	orrs r5, r3
	strb r5, [r0]
	movs r2, #1
	movs r0, #1
	movs r3, #0
	negs r1, r1
	negs r2, r2
	negs r0, r0
	bl Camera_MoveWithinMapBounds
	movs r0, #78
	bl Func_020059f4
	movs r5, #192
	movs r0, #30
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	lsls r5, r5, #8
	movs r1, #128
	movs r2, #164
	adds r3, r5, #0
	movs r0, #30
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl Func_0200588c
	movs r1, #134
	movs r2, #168
	adds r3, r5, #0
	movs r0, #0
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl Func_0200588c
	movs r1, #134
	movs r2, #176
	adds r3, r5, #0
	movs r0, #1
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl Func_0200588c
	movs r1, #138
	movs r2, #176
	adds r3, r5, #0
	movs r0, #2
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl Func_0200588c
	movs r1, #138
	movs r2, #184
	adds r3, r5, #0
	movs r0, #3
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl Func_0200588c
	movs r1, #128
	movs r0, #4
	lsls r1, r1, #7
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #7
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #128
	movs r0, #6
	lsls r1, r1, #7
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #7
	bl Actor_ArmCallback
	movs r0, #3
	bl Func_02003a6c
	bl Func_0200598c
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #4
	bl Func_020058fc
	movs r1, #10
	adds r1, #255
	movs r2, #30
	movs r0, #5
	bl Func_020058fc
	movs r2, #0
	movs r0, #5
	movs r1, #0
	bl Func_020058d4
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #8
	bl Func_020058ec
	adds r1, r5, #0
	movs r0, #1
	bl Func_020058ec
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #0
	bl Func_02003c58
	movs r1, #4
	adds r1, #255
	movs r2, #30
	movs r0, #1
	bl Func_020058fc
	movs r2, #0
	movs r0, #1
	movs r1, #0
	bl Func_020058d4
	movs r1, #160
	movs r0, #0
	lsls r1, r1, #8
	bl Func_020058ec
	movs r1, #2
	movs r0, #0
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl FieldEvent_Wait
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r1, #10
	movs r2, #30
	adds r1, #255
	movs r0, #30
	bl Func_020058fc
	movs r1, #128
	movs r0, #30
	lsls r1, r1, #6
	bl Func_020058ec
	movs r0, #30
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r1, #8
	adds r1, #255
	movs r2, #30
	movs r0, #1
	bl Func_020058fc
	movs r0, #1
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r1, #10
	adds r1, #255
	movs r2, #30
	movs r0, #2
	bl Func_020058fc
	movs r1, #138
	movs r2, #160
	lsls r2, r2, #1
	movs r0, #2
	lsls r1, r1, #2
	bl Actor_ResetAndSetPositionInMode2
	adds r1, r5, #0
	movs r0, #2
	bl Func_020058ec
	movs r1, #192
	movs r0, #2
	lsls r1, r1, #7
	bl Func_020058ec
	movs r1, #128
	movs r0, #2
	lsls r1, r1, #8
	bl Func_020058ec
	movs r0, #2
	movs r1, #4
	bl Actor_SetModeById
	movs r0, #33
	bl FieldEvent_Wait
	movs r0, #2
	movs r1, #2
	bl Motion_SetVarCbAndRefresh
	movs r2, #0
	movs r0, #2
	movs r1, #0
	bl Func_020058d4
	movs r0, #0
	movs r1, #2
	bl Motion_SetVarCbAndRefresh
	movs r1, #224
	lsls r1, r1, #8
	movs r0, #0
	bl Func_020058ec
	movs r0, #5
	bl FieldEvent_Wait
	movs r2, #0
	movs r0, #0
	movs r1, #0
	bl Func_020058d4
	movs r1, #192
	movs r0, #2
	lsls r1, r1, #7
	bl Func_020058ec
	movs r1, #2
	movs r0, #2
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl FieldEvent_Wait
	movs r0, #2
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r1, #224
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #192
	movs r0, #6
	lsls r1, r1, #7
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #192
	movs r0, #7
	lsls r1, r1, #7
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #224
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #4
	bl Actor_ArmCallback
	movs r0, #10
	bl FieldEvent_Wait
	movs r0, #30
	movs r1, #0
	bl Func_020058ec
	movs r0, #30
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #10
	bl FieldEvent_Wait
	movs r0, #30
	movs r1, #0
	bl Func_020058ec
	movs r0, #30
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r1, #128
	movs r0, #4
	lsls r1, r1, #7
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #7
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #128
	movs r0, #7
	lsls r1, r1, #7
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #128
	movs r2, #0
	movs r0, #6
	lsls r1, r1, #7
	bl Actor_ArmCallback
	movs r1, #128
	lsls r1, r1, #7
	movs r0, #0
	bl Func_020058ec
	movs r0, #60
	bl FieldEvent_Wait
	adds r1, r5, #0
	movs r0, #1
	bl Func_020058ec
	movs r0, #3
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #1
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #10
	bl FieldEvent_Wait
	movs r1, #224
	lsls r1, r1, #8
	movs r0, #0
	bl Func_020058ec
	movs r0, #60
	bl FieldEvent_Wait
	movs r0, #6
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #7
	b .L_0200b54c
	.2byte 0x0000
.L_0200b544:
	.4byte gData_02005ff8
.L_0200b548:
	.4byte gData_02006088
.L_0200b54c:
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #5
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #10
	bl FieldEvent_Wait
	adds r1, r5, #0
	movs r0, #0
	bl Func_020058ec
	movs r0, #30
	bl FieldEvent_Wait
	movs r1, #128
	movs r0, #4
	lsls r1, r1, #7
	bl Func_020058ec
	movs r0, #4
	movs r1, #0
	bl Event_AskYesNoWithActorAnimation
	cmp r0, #0
	bne .L_0200b5be
	movs r1, #3
	movs r0, #0
	bl Actor_SetModeById
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #10
	bl FieldEvent_Wait
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r0, #0
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #10
	bl FieldEvent_Wait
	b .L_0200b644
.L_0200b5be:
	movs r1, #8
	adds r1, #255
	movs r2, #30
	movs r0, #0
	bl Func_020058fc
	movs r0, #5
	movs r1, #4
	bl Actor_SetModeById
	movs r0, #33
	bl FieldEvent_Wait
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r1, #128
	adds r3, #2
	strh r3, [r2]
	movs r0, #5
	lsls r1, r1, #8
	bl Func_020058ec
	movs r2, #0
	movs r0, #5
	movs r1, #0
	bl Func_020058d4
	movs r1, #128
	lsls r1, r1, #7
	movs r0, #5
	bl Func_020058ec
	movs r0, #10
	bl FieldEvent_Wait
	movs r1, #2
	movs r0, #5
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl FieldEvent_Wait
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r0, #0
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #10
	bl FieldEvent_Wait
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
.L_0200b644:
	movs r0, #2
	movs r1, #2
	bl Actor_SetModeById
	movs r0, #0
	bl Actor_GetById
	cmp r0, #0
	beq .L_0200b664
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #2
	bl Actor_ResetAndSetPosition
.L_0200b664:
	movs r0, #3
	movs r1, #2
	bl Actor_SetModeById
	movs r0, #0
	bl Actor_GetById
	cmp r0, #0
	beq .L_0200b684
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #3
	bl Actor_ResetAndSetPosition
.L_0200b684:
	movs r0, #1
	movs r1, #2
	bl Actor_SetModeById
	movs r0, #0
	bl Actor_GetById
	cmp r0, #0
	beq .L_0200b6a4
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #1
	bl Actor_ResetAndSetPosition
.L_0200b6a4:
	ldr r5, .L_0200ba10
	movs r0, #2
	adds r1, r5, #0
	bl Actor_SetMovementTypeById
	adds r1, r5, #0
	movs r0, #3
	bl Actor_SetMovementTypeById
	adds r1, r5, #0
	movs r0, #1
	bl Actor_SetMovementTypeById
	movs r0, #2
	bl Actor_RefreshSelectorById
	movs r0, #3
	bl Actor_RefreshSelectorById
	movs r0, #1
	bl Actor_RefreshSelectorById
	movs r0, #0
	movs r1, #1
	bl Camera_FollowActor
	movs r0, #4
	movs r1, #0
	bl Actor_WatchAndSetScript
	movs r0, #7
	movs r1, #0
	bl Actor_WatchAndSetScript
	movs r0, #5
	movs r1, #0
	bl Actor_WatchAndSetScript
	movs r0, #6
	movs r1, #0
	bl Actor_WatchAndSetScript
	movs r1, #0
	movs r0, #30
	bl Actor_WatchAndSetScript
	movs r0, #10
	bl FieldEvent_Wait
	movs r1, #192
	movs r2, #192
	movs r0, #0
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl Actor_SetSpeedParameters
	movs r1, #144
	lsls r1, r1, #1
	movs r2, #108
	adds r2, #255
	movs r0, #0
	adds r1, #255
	bl Actor_SetPositionAndReset
	movs r1, #192
	lsls r1, r1, #8
	movs r0, #0
	bl Func_020058ec
	movs r0, #10
	bl FieldEvent_Wait
	movs r1, #2
	movs r0, #0
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl FieldEvent_Wait
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r1, #159
	lsls r1, r1, #1
	movs r2, #179
	movs r0, #0
	adds r1, #255
	lsls r2, r2, #1
	bl Actor_SetPositionAndReset
	movs r1, #146
	movs r2, #150
	movs r0, #0
	lsls r1, r1, #2
	lsls r2, r2, #1
	bl Actor_SetPositionAndReset
	movs r1, #146
	movs r0, #0
	lsls r1, r1, #2
	movs r2, #252
	bl Actor_SetPositionAndReset
	movs r1, #154
	movs r0, #0
	lsls r1, r1, #2
	movs r2, #247
	bl Actor_SetPositionAndReset
	movs r1, #181
	lsls r1, r1, #1
	movs r2, #156
	lsls r2, r2, #1
	adds r1, #255
	movs r0, #0
	bl Actor_SetPositionAndReset
	movs r0, #0
	bl Actor_GetById
	movs r6, #2
	adds r0, #85
	strb r6, [r0]
	ldr r1, .L_0200ba14
	movs r0, #0
	bl Actor_SetMovementTypeById
	movs r0, #0
	bl Actor_RefreshSelectorById
	movs r1, #128
	lsls r1, r1, #7
	movs r0, #0
	bl Func_020058ec
	movs r0, #30
	bl FieldEvent_Wait
	movs r0, #0
	bl Actor_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #32
	orrs r3, r2
	strb r3, [r0]
	movs r1, #0
	movs r0, #4
	movs r2, #0
	bl Actor_ArmCallback
	movs r0, #7
	movs r1, #0
	movs r2, #0
	bl Actor_ArmCallback
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl Actor_ArmCallback
	movs r0, #6
	movs r1, #0
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #0
	movs r2, #0
	movs r0, #30
	bl Actor_ArmCallback
	movs r0, #1
	bl Func_02003a6c
	movs r0, #100
	bl FieldEvent_Wait
	movs r1, #128
	movs r0, #4
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #128
	movs r0, #7
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #128
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #0
	bl Actor_ArmCallback
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #30
	bl Actor_ArmCallback
	movs r0, #1
	bl Func_02003c58
	movs r0, #60
	bl FieldEvent_Wait
	movs r1, #1
	movs r0, #4
	bl Camera_FollowActor
	bl Func_02005924
	movs r0, #60
	bl FieldEvent_Wait
	movs r1, #0
	movs r0, #4
	bl Func_020058ec
	movs r0, #60
	bl FieldEvent_Wait
	movs r0, #5
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #10
	bl FieldEvent_Wait
	movs r1, #224
	movs r0, #4
	lsls r1, r1, #8
	bl Func_020058ec
	movs r0, #7
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #6
	movs r1, #3
	bl Actor_SetModeById
	movs r0, #20
	bl FieldEvent_Wait
	movs r0, #7
	movs r1, #2
	bl Actor_SetModeById
	movs r0, #4
	bl Actor_GetById
	cmp r0, #0
	beq .L_0200b8c6
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #7
	bl Actor_ResetAndSetPosition
.L_0200b8c6:
	movs r0, #6
	movs r1, #2
	bl Actor_SetModeById
	movs r0, #4
	bl Actor_GetById
	cmp r0, #0
	beq .L_0200b8e6
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #6
	bl Actor_ResetAndSetPosition
.L_0200b8e6:
	movs r0, #5
	movs r1, #2
	bl Actor_SetModeById
	movs r0, #4
	bl Actor_GetById
	cmp r0, #0
	beq .L_0200b906
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #5
	bl Actor_ResetAndSetPosition
.L_0200b906:
	movs r0, #30
	movs r1, #2
	bl Actor_SetModeById
	movs r0, #4
	bl Actor_GetById
	cmp r0, #0
	beq .L_0200b926
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #30
	bl Actor_ResetAndSetPosition
.L_0200b926:
	adds r1, r5, #0
	movs r0, #7
	bl Actor_SetMovementTypeById
	adds r1, r5, #0
	movs r0, #6
	bl Actor_SetMovementTypeById
	adds r1, r5, #0
	movs r0, #5
	bl Actor_SetMovementTypeById
	adds r1, r5, #0
	movs r0, #30
	bl Actor_SetMovementTypeById
	movs r0, #7
	bl Actor_RefreshSelectorById
	movs r0, #6
	bl Actor_RefreshSelectorById
	movs r0, #5
	bl Actor_RefreshSelectorById
	movs r0, #30
	bl Actor_RefreshSelectorById
	movs r0, #4
	movs r1, #1
	bl Camera_FollowActor
	movs r1, #16
	movs r2, #0
	movs r0, #4
	negs r1, r1
	bl Actor_CommitPositionAndActivate
	ldr r1, .L_0200ba18
	movs r0, #4
	bl Actor_SetMovementTypeById
	movs r0, #4
	bl Actor_RefreshSelectorById
	movs r0, #4
	bl Actor_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r2, #64
	orrs r3, r2
	strb r3, [r0]
	movs r0, #29
	bl Actor_GetById
	adds r0, #85
	strb r6, [r0]
	ldr r1, .L_0200ba1c
	movs r0, #4
	bl Actor_SetMovementTypeById
	movs r0, #4
	bl Actor_RefreshSelectorById
	movs r1, #56
	movs r2, #128
	lsls r2, r2, #1
	movs r0, #4
	adds r1, #255
	bl Actor_SetPositionAndReset
	movs r1, #128
	lsls r1, r1, #7
	movs r0, #4
	bl Func_020058ec
	movs r0, #10
	bl FieldEvent_Wait
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r1, r1
	negs r2, r2
	movs r3, #0
	negs r0, r0
	bl Camera_MoveWithinMapBounds
	movs r0, #2
	bl Func_02003a6c
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #108]
	movs r3, #214
	lsls r3, r3, #1
	adds r2, r1, r3
	adds r3, #84
	str r3, [r2]
	subs r3, #76
	adds r2, r1, r3
	movs r3, #24
	str r3, [r2]
	bl Event_FinishScreenTransition
	bl Event_WaitTransitionFrames
	movs r0, #1
	bl Func_02005934
	bl Func_0200581c
	pop {r3}
	mov r8, r3
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200ba10:
	.4byte gData_02005ff0
.L_0200ba14:
	.4byte gData_020061e0
.L_0200ba18:
	.4byte gData_02005ff8
.L_0200ba1c:
	.4byte gData_02006088
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
	.section .text.x0200bc58,"ax",%progbits
	.global Func_02003c58
	.thumb_func
Func_02003c58:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #4
	str r0, [sp, #0]
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	movs r2, #188
	lsls r2, r2, #1
	adds r2, r2, r3
	mov r10, r2
	ldr r2, .L_0200bd94
	movs r3, #192
	lsls r3, r3, #9
	mov r9, r3
	ldr r3, [r2]
	movs r4, #0
	mov r11, r4
	mov r8, r4
	cmp r3, #64
	beq .L_0200bcac
	adds r5, r2, #0
.L_0200bc8e:
	ldmia r5!, {r0}
	bl Actor_GetById
	movs r3, #0
	adds r0, #85
	strb r3, [r0]
	movs r3, #1
	add r8, r3
	mov r4, r8
	add r11, r3
	cmp r4, #4
	bhi .L_0200bcac
	ldr r3, [r5]
	cmp r3, #64
	bne .L_0200bc8e
.L_0200bcac:
	ldr r2, [sp, #0]
	cmp r2, #0
	beq .L_0200bd74
	movs r0, #223
	bl Func_020059f4
	movs r3, #0
	mov r4, r10
	mov r8, r3
	ldr r3, [r4, #12]
	movs r2, #192
	lsls r2, r2, #9
	adds r3, r3, r2
	str r3, [r4, #12]
	b .L_0200bd6c
.L_0200bcca:
	movs r7, #0
	cmp r7, r11
	bcs .L_0200bcf8
	ldr r6, .L_0200bd94
.L_0200bcd2:
	ldr r0, [r6]
	bl Actor_GetById
	ldr r3, [r0, #16]
	mov r4, r9
	subs r3, r3, r4
	str r3, [r0, #16]
	adds r7, #1
	ldr r0, [r6]
	bl Actor_GetById
	adds r5, r0, #0
	ldmia r6!, {r0}
	bl Actor_GetById
	ldr r3, [r0, #16]
	str r3, [r5, #64]
	cmp r7, r11
	bcc .L_0200bcd2
.L_0200bcf8:
	movs r3, #3
	mov r2, r8
	ands r3, r2
	cmp r3, #3
	bne .L_0200bd24
	mov r4, r9
	lsls r3, r4, #4
	subs r0, r3, r4
	mov r4, r10
	ldr r2, [r4, #12]
	ldr r1, .L_0200bd98
	lsls r3, r0, #1
	adds r3, r3, r2
	ldr r2, [r1]
	cmp r2, r3
	bge .L_0200bd24
	adds r3, r0, #0
	cmp r3, #0
	bge .L_0200bd20
	adds r3, #15
.L_0200bd20:
	asrs r3, r3, #4
	mov r9, r3
.L_0200bd24:
	movs r2, #192
	lsls r2, r2, #4
	adds r2, #203
	cmp r9, r2
	bgt .L_0200bd36
	movs r3, #192
	lsls r3, r3, #4
	adds r3, #204
	mov r9, r3
.L_0200bd36:
	ldr r4, [sp, #0]
	cmp r4, #1
	bne .L_0200bd54
	mov r2, r8
	cmp r2, #40
	bne .L_0200bd54
	movs r0, #144
	movs r1, #208
	movs r2, #184
	lsls r0, r0, #17
	lsls r1, r1, #15
	lsls r2, r2, #17
	movs r3, #1
	bl Camera_MoveWithinMapBounds
.L_0200bd54:
	movs r0, #1
	bl wait
	movs r3, #1
	add r8, r3
	mov r4, r8
	cmp r4, #119
	bhi .L_0200bd74
	mov r2, r10
	ldr r3, [r2, #12]
	add r3, r9
	str r3, [r2, #12]
.L_0200bd6c:
	ldr r2, .L_0200bd98
	ldr r2, [r2]
	cmp r3, r2
	blt .L_0200bcca
.L_0200bd74:
	ldr r3, .L_0200bd98
	mov r4, r10
	ldr r3, [r3]
	str r3, [r4, #12]
	bl Func_0200577c
	movs r0, #2
	bl wait
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0200bd94:
	.4byte gData_02005a20
.L_0200bd98:
	.4byte Data_02006a1c
	.section .text.x0200bf60,"ax",%progbits
	.global Func_02003f60
	.thumb_func
Func_02003f60:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r3, .L_0200c00c
	sub sp, #32
	ldr r0, [r3]
	cmp r0, #0
	bge .L_0200bf72
	adds r0, #3
.L_0200bf72:
	asrs r0, r0, #2
	movs r1, #5
	bl __modsi3
	ldr r3, .L_0200c010
	mov r8, r0
	ldr r3, [r3]
	cmp r3, #0
	beq .L_0200bfc6
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r0, #179
	lsls r0, r0, #1
	adds r3, r2, r0
	ldrh r1, [r3]
	movs r0, #0
	ldrsh r3, [r3, r0]
	cmp r3, #0
	beq .L_0200bfa6
	movs r3, #192
	lsls r3, r3, #2
	adds r3, #255
	ands r3, r1
	cmp r3, #153
	bne .L_0200c002
.L_0200bfa6:
	movs r1, #192
	lsls r1, r1, #4
	adds r1, #164
	adds r3, r2, r1
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	bne .L_0200c002
	movs r0, #175
	lsls r0, r0, #1
	adds r3, r2, r0
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	bne .L_0200c002
.L_0200bfc6:
	movs r5, #0
	movs r6, #4
.L_0200bfca:
	mov r2, r8
	adds r0, r2, r5
	movs r1, #5
	mov r7, sp
	bl __modsi3
	ldr r3, .L_0200c014
	lsls r0, r0, #1
	ldrh r3, [r3, r6]
	adds r5, #1
	strh r3, [r7, r0]
	adds r6, #2
	cmp r5, #4
	ble .L_0200bfca
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	adds r0, r7, #0
	ldr r1, .L_0200c018
	adds r2, #2
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r2, .L_0200c00c
	ldr r3, [r2]
	adds r3, #1
	str r3, [r2]
.L_0200c002:
	add sp, #32
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200c00c:
	.4byte gData_020069f4
.L_0200c010:
	.4byte gData_020069f8
.L_0200c014:
	.4byte gData_02006a2c
.L_0200c018:
	.4byte 0x05000184
	.section .text.x0200c0f4,"ax",%progbits
	.global Func_020040f4
	.thumb_func
Func_020040f4:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #133
	mov r10, r3
	ldr r3, .L_0200c200
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Actor_GetById
	adds r6, r0, #0
	ldr r7, [r6, #104]
	bl Func_02005814
	movs r0, #0
	bl Func_02005984
	movs r3, #208
	lsls r3, r3, #4
	adds r3, #70
	add r3, r10
	movs r5, #0
	strh r5, [r3]
	movs r3, #85
	adds r3, r3, r6
	mov r9, r3
	mov r2, r9
	movs r3, #4
	strb r3, [r2]
	adds r0, r6, #0
	movs r1, #0
	bl Actor_SetSingleChildFlags
	movs r3, #99
	adds r3, r3, r7
	mov r8, r3
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_0200c194
.L_0200c14e:
	ldr r3, [r7, #8]
	ldr r2, .L_0200c204
	str r3, [r6, #8]
	ldr r3, [r7, #12]
	adds r3, r3, r5
	str r3, [r6, #12]
	ldr r3, [r7, #16]
	str r3, [r6, #16]
	cmp r5, r2
	bgt .L_0200c16a
	movs r3, #200
	lsls r3, r3, #5
	adds r3, #153
	adds r5, r5, r3
.L_0200c16a:
	ldr r3, .L_0200c208
	adds r1, r6, #0
	ldr r2, [r3]
	ldrb r3, [r3]
	adds r1, #35
	lsls r3, r3, #12
	strh r3, [r6, #6]
	movs r3, #1
	ands r2, r3
	movs r3, #2
	lsls r3, r2
	ldrb r2, [r1]
	movs r0, #1
	eors r3, r2
	strb r3, [r1]
	bl wait
	mov r2, r8
	ldrb r3, [r2]
	cmp r3, #0
	bne .L_0200c14e
.L_0200c194:
	movs r3, #208
	lsls r3, r3, #4
	adds r3, #68
	movs r2, #1
	add r3, r10
	strh r2, [r3]
	movs r3, #208
	lsls r3, r3, #4
	adds r3, #70
	add r3, r10
	strh r2, [r3]
	ldr r3, [r7, #8]
	ldrh r1, [r7, #6]
	subs r2, #3
	asrs r3, r3, #19
	ands r3, r2
	asrs r1, r1, #13
	adds r3, r3, r1
	subs r3, #1
	lsls r3, r3, #19
	str r3, [r6, #8]
	ldr r3, [r7, #16]
	ldr r0, .L_0200c1fc
	asrs r3, r3, #19
	ands r3, r2
	movs r2, #2
	ands r1, r2
	subs r3, r3, r1
	adds r3, #1
	lsls r3, r3, #19
	str r3, [r6, #16]
	movs r3, #192
	lsls r3, r3, #11
	str r3, [r6, #40]
	adds r3, r6, #0
	adds r3, #35
	strb r0, [r3]
	mov r2, r9
	movs r3, #3
	strb r3, [r2]
	adds r0, r6, #0
	movs r1, #1
	bl Actor_SetSingleChildFlags
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r1, r1
	negs r2, r2
	movs r3, #0
	negs r0, r0
	b .L_0200c20c
.L_0200c1fc:
	.4byte 0x00000001
.L_0200c200:
	.4byte gMainRoutineWork
.L_0200c204:
	.4byte 0x0003ffff
.L_0200c208:
	.4byte gFrameCount
.L_0200c20c:
	bl Camera_MoveWithinMapBounds
	bl Func_0200592c
	movs r3, #128
	adds r7, r0, #0
	lsls r3, r3, #12
	str r3, [r7, #48]
	movs r3, #128
	ldr r5, .L_0200c254
	lsls r3, r3, #9
	str r3, [r7, #52]
	adds r3, r7, #0
	adds r3, #85
	strb r5, [r3]
	movs r0, #0
	ldr r1, [r6, #8]
	ldr r2, [r6, #16]
	bl Map_GetTerrainHeight
	ldr r3, [r6, #16]
	adds r2, r0, #0
	ldr r1, [r6, #8]
	adds r0, r7, #0
	bl Func_02005784
	ldr r3, [r6, #20]
	movs r2, #128
	lsls r2, r2, #13
	adds r3, r3, r2
	ldr r2, [r6, #12]
	movs r5, #0
	cmp r2, r3
	ble .L_0200c272
	b .L_0200c258
	.2byte 0x0000
.L_0200c254:
	.4byte 0x00000000
.L_0200c258:
	movs r0, #1
	adds r5, #1
	bl wait
	cmp r5, #59
	bgt .L_0200c272
	ldr r3, [r6, #20]
	movs r2, #128
	lsls r2, r2, #13
	adds r3, r3, r2
	ldr r2, [r6, #12]
	cmp r2, r3
	bgt .L_0200c258
.L_0200c272:
	movs r0, #127
	bl Func_020059f4
	ldr r3, [r6, #40]
	movs r5, #0
	cmp r3, #0
	beq .L_0200c292
.L_0200c280:
	movs r0, #1
	adds r5, #1
	bl wait
	cmp r5, #59
	bgt .L_0200c292
	ldr r3, [r6, #40]
	cmp r3, #0
	bne .L_0200c280
.L_0200c292:
	adds r0, r7, #0
	bl Func_0200578c
	ldr r5, .L_0200c2dc
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	ldr r0, [r5]
	bl Actor_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	strb r3, [r0]
	movs r1, #1
	ldr r0, [r5]
	bl Camera_FollowActor
	movs r2, #208
	lsls r2, r2, #4
	adds r2, #70
	add r2, r10
	movs r3, #1
	strh r3, [r2]
	movs r3, #170
	lsls r3, r3, #1
	movs r6, #0
	add r3, r10
	strh r6, [r3]
	bl Func_0200581c
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_0200c2dc:
	.4byte gMainRoutineWork
	.section .text.x0200c30c,"ax",%progbits
	.global Func_0200430c
	.thumb_func
Func_0200430c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #133
	mov r10, r3
	ldr r3, .L_0200c374
	lsls r2, r2, #2
	adds r3, r3, r2
	adds r6, r0, #0
	ldr r0, [r3]
	sub sp, #12
	bl Actor_GetById
	movs r3, #179
	lsls r3, r3, #1
	add r3, r10
	movs r2, #0
	ldrsh r3, [r3, r2]
	adds r7, r0, #0
	cmp r3, #0
	bne .L_0200c36a
	movs r3, #173
	lsls r3, r3, #1
	add r3, r10
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	bne .L_0200c36a
	movs r3, #175
	lsls r3, r3, #1
	add r3, r10
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	bne .L_0200c36a
	movs r3, #180
	lsls r3, r3, #1
	add r3, r10
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	beq .L_0200c378
.L_0200c36a:
	adds r0, r6, #0
	movs r1, #0
	bl Actor_SetAnimationSpeed
	b .L_0200c4b6
.L_0200c374:
	.4byte gMainRoutineWork
.L_0200c378:
	adds r0, r6, #0
	movs r1, #16
	bl Actor_SetAnimationSpeed
	adds r3, r6, #0
	adds r3, #100
	ldrh r2, [r3]
	adds r2, #1
	strh r2, [r3]
	movs r3, #31
	ands r3, r2
	cmp r3, #31
	bne .L_0200c398
	movs r0, #231
	bl Func_020059f4
.L_0200c398:
	ldr r3, [r7, #80]
	ldr r0, [r6, #80]
	ldrb r3, [r3, #9]
	ldrb r1, [r0, #9]
	movs r2, #12
	ands r2, r3
	movs r3, #13
	negs r3, r3
	ands r3, r1
	orrs r3, r2
	strb r3, [r0, #9]
	movs r2, #2
	ldr r0, [r6, #8]
	ldr r1, [r6, #16]
	bl Func_020059e4
	cmp r0, #255
	beq .L_0200c49a
	ldr r3, [r6, #8]
	mov r5, sp
	str r3, [r5]
	adds r0, r5, #0
	ldr r3, [r6, #12]
	str r3, [r5, #4]
	ldr r3, [r6, #16]
	str r3, [r5, #8]
	bl Func_02005994
	ldr r5, [r5]
	movs r3, #136
	lsls r3, r3, #17
	cmp r5, r3
	bgt .L_0200c49a
	ldr r2, .L_0200c488
	cmp r5, r2
	blt .L_0200c49a
	movs r3, #98
	adds r3, r3, r6
	mov r9, r3
	ldrb r3, [r3]
	cmp r3, #0
	bne .L_0200c460
	ldr r2, [r7, #12]
	ldr r3, [r6, #12]
	subs r5, r2, r3
	cmp r5, #0
	bge .L_0200c3f8
	subs r5, r3, r2
.L_0200c3f8:
	adds r0, r7, #0
	adds r1, r6, #0
	movs r2, #0
	adds r0, #8
	adds r1, #8
	mov r8, r2
	bl Func_020042e0
	cmp r0, #12
	bgt .L_0200c418
	movs r3, #192
	lsls r3, r3, #12
	cmp r5, r3
	bge .L_0200c418
	movs r2, #1
	mov r8, r2
.L_0200c418:
	mov r3, r8
	cmp r3, #0
	beq .L_0200c460
	movs r0, #130
	lsls r0, r0, #1
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200c460
	ldrh r3, [r6, #6]
	str r6, [r7, #104]
	strh r3, [r7, #6]
	adds r1, r7, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #254
	ands r3, r2
	movs r2, #181
	lsls r2, r2, #1
	strb r3, [r1]
	add r2, r10
	movs r3, #200
	strh r3, [r2]
	ldr r3, .L_0200c48c
	movs r2, #128
	ldr r0, .L_0200c484
	lsls r2, r2, #2
	adds r2, #18
	adds r3, r3, r2
	strb r0, [r3]
	mov r2, r9
	movs r3, #1
	strb r3, [r2]
	adds r2, r6, #0
	adds r2, #99
	strb r3, [r2]
.L_0200c460:
	ldrh r0, [r6, #6]
	bl Math_Cosine
	ldr r1, [r6, #48]
	ldr r5, .L_0200c490
	mov lr, r5
	.2byte 0xf800
	ldr r3, [r6, #8]
	adds r3, r3, r0
	ldrh r0, [r6, #6]
	str r3, [r6, #8]
	bl Math_Sine
	ldr r1, [r6, #48]
	mov lr, r5
	.2byte 0xf800
	ldr r3, [r6, #16]
	b .L_0200c494
.L_0200c484:
	.4byte 0x00000000
.L_0200c488:
	.4byte 0xffe00000
.L_0200c48c:
	.4byte gMainRoutineWork
.L_0200c490:
	.4byte IwramMulQ16
.L_0200c494:
	adds r3, r3, r0
	str r3, [r6, #16]
	b .L_0200c4b6
.L_0200c49a:
	adds r3, r6, #0
	adds r3, #99
	movs r5, #0
	strb r5, [r3]
	ldr r1, .L_0200c4c4
	adds r0, r6, #0
	str r5, [r6, #108]
	bl Func_02005764
	movs r0, #228
	bl Func_020059f4
	ldr r3, .L_0200c4c8
	str r5, [r3]
.L_0200c4b6:
	add sp, #12
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200c4c4:
	.4byte gData_020069fc
.L_0200c4c8:
	.4byte Data_02006a28
	.section .text.x0200c4cc,"ax",%progbits
	.global Func_020044cc
	.thumb_func
Func_020044cc:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r5, r0, #0
	movs r0, #222
	sub sp, #68
	bl Func_020059f4
	ldrh r0, [r5, #6]
	bl Math_Cosine
	adds r1, r0, #0
	movs r0, #128
	ldr r6, .L_0200c580
	lsls r0, r0, #12
	mov lr, r6
	.2byte 0xf800
	ldr r3, [r5, #8]
	add r2, sp, #56
	adds r3, r3, r0
	str r3, [r2]
	mov r8, r2
	ldrh r0, [r5, #6]
	bl Math_Sine
	adds r1, r0, #0
	movs r0, #128
	lsls r0, r0, #12
	mov lr, r6
	.2byte 0xf800
	ldr r3, [r5, #16]
	mov r2, r8
	adds r3, r3, r0
	str r3, [r2, #8]
	movs r0, #140
	ldr r1, [r2]
	lsls r0, r0, #1
	ldr r2, [r5, #12]
	bl Func_0200576c
	movs r1, #2
	adds r7, r0, #0
	bl Func_02005754
	adds r0, r7, #0
	movs r1, #0
	bl Actor_SetSingleChildFlags
	adds r3, r7, #0
	movs r6, #0
	adds r3, #85
	strb r6, [r3]
	adds r1, r7, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #254
	ands r3, r2
	strb r3, [r1]
	ldr r2, .L_0200c57c
	ldrh r3, [r5, #6]
	add r4, sp, #16
	strh r3, [r7, #6]
	adds r3, r7, #0
	adds r3, #100
	strh r6, [r3]
	subs r3, #2
	strb r2, [r3]
	adds r3, #1
	strb r2, [r3]
	ldr r3, .L_0200c584
	str r3, [r7, #108]
	movs r3, #192
	lsls r3, r3, #9
	str r3, [r7, #48]
	movs r3, #1
	str r3, [r4]
	movs r3, #7
	str r3, [r4, #4]
	mov r3, r8
	ldr r0, [r3]
	ldr r2, [r3, #8]
	ldr r3, .L_0200c588
	ldr r1, [r5, #12]
	adds r2, r2, r3
	movs r3, #192
	lsls r3, r3, #10
	str r3, [sp, #8]
	b .L_0200c58c
.L_0200c57c:
	.4byte 0x00000000
.L_0200c580:
	.4byte IwramMulQ16
.L_0200c584:
	.4byte Func_0200430c
.L_0200c588:
	.4byte 0xfffa0000
.L_0200c58c:
	movs r3, #0
	str r6, [sp, #0]
	str r6, [sp, #4]
	str r4, [sp, #12]
	bl Func_02004bd4
	adds r0, r7, #0
	add sp, #68
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x0200c644,"ax",%progbits
	.global Func_02004644
	.thumb_func
Func_02004644:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r0, #9
	bl Actor_GetById
	adds r6, r0, #0
	movs r0, #10
	bl Actor_GetById
	adds r7, r0, #0
	movs r0, #23
	bl Actor_GetById
	adds r5, r0, #0
	ldr r2, [r5, #80]
	movs r1, #128
	mov r8, r2
	movs r2, #248
	movs r0, #24
	lsls r1, r1, #18
	lsls r2, r2, #16
	bl Func_02005884
	movs r1, #128
	movs r2, #248
	movs r0, #23
	lsls r1, r1, #18
	lsls r2, r2, #16
	bl Func_02005884
	movs r1, #236
	movs r2, #128
	movs r0, #9
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_02005884
	movs r1, #138
	movs r2, #128
	lsls r2, r2, #17
	movs r0, #10
	lsls r1, r1, #18
	bl Func_02005884
	movs r0, #24
	bl Actor_GetById
	movs r3, #85
	movs r2, #0
	adds r3, r3, r5
	str r2, [r0, #24]
	strb r2, [r3]
	mov r11, r3
	ldr r3, [r5, #20]
	movs r0, #160
	str r3, [r5, #12]
	movs r3, #85
	adds r3, r3, r6
	strb r2, [r3]
	mov r9, r3
	ldr r3, [r6, #20]
	lsls r0, r0, #4
	str r3, [r6, #12]
	movs r3, #85
	adds r3, r3, r7
	strb r2, [r3]
	mov r10, r3
	ldr r3, [r7, #20]
	adds r0, #10
	str r3, [r7, #12]
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200c720
	ldr r3, [r6, #12]
	ldr r2, .L_0200c7b8
	movs r0, #9
	adds r3, r3, r2
	str r3, [r6, #12]
	ldr r2, .L_0200c7bc
	ldr r3, [r5, #12]
	adds r3, r3, r2
	str r3, [r5, #12]
	ldr r2, .L_0200c7c0
	ldr r3, [r5, #8]
	adds r3, r3, r2
	str r3, [r5, #8]
	mov r2, r8
	ldrh r3, [r2, #18]
	movs r2, #128
	lsls r2, r2, #4
	adds r3, r3, r2
	mov r2, r8
	strh r3, [r2, #18]
	bl Actor_GetById
	movs r1, #4
	bl Animation_ApplyChildValues
	movs r0, #11
	bl Actor_GetById
	movs r1, #4
	bl Animation_ApplyChildValues
.L_0200c720:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #11
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200c76a
	ldr r3, [r7, #12]
	ldr r2, .L_0200c7b8
	movs r0, #10
	adds r3, r3, r2
	str r3, [r7, #12]
	ldr r2, .L_0200c7bc
	ldr r3, [r5, #12]
	adds r3, r3, r2
	str r3, [r5, #12]
	ldr r2, .L_0200c7c4
	ldr r3, [r5, #8]
	adds r3, r3, r2
	str r3, [r5, #8]
	mov r2, r8
	ldrh r3, [r2, #18]
	ldr r2, .L_0200c7c8
	adds r3, r3, r2
	mov r2, r8
	strh r3, [r2, #18]
	bl Actor_GetById
	movs r1, #4
	bl Animation_ApplyChildValues
	movs r0, #12
	bl Actor_GetById
	movs r1, #4
	bl Animation_ApplyChildValues
.L_0200c76a:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #10
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200c7aa
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #11
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200c7aa
	ldr r3, [r6, #12]
	ldr r2, .L_0200c7cc
	adds r3, r3, r2
	str r3, [r6, #12]
	ldr r3, [r7, #12]
	adds r3, r3, r2
	str r3, [r7, #12]
	ldr r2, .L_0200c7d0
	ldr r3, [r5, #12]
	adds r3, r3, r2
	str r3, [r5, #12]
	mov r2, r9
	movs r3, #4
	strb r3, [r2]
	mov r2, r10
	strb r3, [r2]
	mov r2, r11
	strb r3, [r2]
.L_0200c7aa:
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200c7b8:
	.4byte 0x00066640
.L_0200c7bc:
	.4byte 0x0001eb80
.L_0200c7c0:
	.4byte 0xfffd70c0
.L_0200c7c4:
	.4byte 0x00028f40
.L_0200c7c8:
	.4byte 0xfffff800
.L_0200c7cc:
	.4byte 0x00199900
.L_0200c7d0:
	.4byte 0x001b8480
	.section .text.x0200c7d4,"ax",%progbits
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
	.section .text.x0200cec8,"ax",%progbits
	.global Func_02004ec8
	.thumb_func
Func_02004ec8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #48
	str r0, [sp, #44]
	ldr r0, .L_0200d0f8
	str r1, [sp, #40]
	mov r8, r0
	movs r1, #32
	add r1, r8
	mov r9, r1
	mov r12, r9
	adds r5, r2, #0
	mov r2, r12
	adds r6, r3, #0
	str r2, [sp, #8]
	ldr r3, .L_0200d0fc
	movs r1, #4
	ldr r7, [sp, #80]
	mov lr, r3
	.2byte 0xf800
	add r0, sp, #44
	ldrh r0, [r0]
	mov r1, r8
	strh r0, [r1, #4]
	add r1, sp, #40
	ldrh r1, [r1]
	mov r3, r8
	strh r1, [r3]
	strh r5, [r3, #2]
	movs r3, #255
	lsls r3, r3, #8
	mov r5, r8
	mov r0, r8
	adds r3, #255
	mov r1, r8
	strh r6, [r5, #6]
	movs r2, #0
	strh r7, [r0, #8]
	strh r3, [r1, #12]
	mov r3, r8
	strh r2, [r3, #10]
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	movs r2, #132
	mov r12, r3
	lsls r2, r2, #1
	mov r1, r12
	add r2, r12
	adds r1, #236
	ldr r0, [r1]
	ldr r3, [r2, #8]
	ldr r5, [r2, #48]
	adds r3, r3, r0
	asrs r3, r3, #20
	str r3, [sp, #32]
	adds r1, #4
	ldr r3, [r2, #12]
	ldr r2, [r1]
	adds r3, r3, r2
	asrs r3, r3, #20
	str r3, [sp, #28]
	mov r3, r12
	adds r3, #244
	ldr r3, [r3]
	subs r3, r3, r0
	asrs r3, r3, #20
	str r3, [sp, #24]
	mov r3, r12
	adds r3, #248
	ldr r3, [r3]
	asrs r0, r0, #20
	subs r3, r3, r2
	asrs r2, r2, #20
	lsls r2, r2, #7
	adds r2, r2, r0
	lsls r2, r2, #2
	asrs r3, r3, #20
	adds r5, r5, r2
	movs r0, #0
	str r3, [sp, #20]
	str r5, [sp, #36]
	str r0, [sp, #12]
	cmp r0, r3
	bge .L_0200d04c
.L_0200cf7c:
	ldr r1, [sp, #12]
	ldr r2, [sp, #36]
	ldr r5, [sp, #24]
	lsls r3, r1, #9
	adds r2, r2, r3
	movs r3, #0
	mov r11, r2
	str r3, [sp, #16]
	cmp r3, r5
	bge .L_0200d040
.L_0200cf90:
	mov r0, r11
	ldrb r5, [r0, #2]
	cmp r5, #0
	beq .L_0200d030
	ldr r1, [sp, #44]
	cmp r5, r1
	bcc .L_0200d030
	adds r1, #1
	mov r10, r1
	cmp r5, r10
	bhi .L_0200d030
	ldr r2, [sp, #16]
	ldr r3, [sp, #32]
	mov r0, r9
	adds r7, r2, r3
	strh r7, [r0]
	ldr r1, [sp, #12]
	ldr r2, [sp, #28]
	add r0, sp, #40
	ldrh r0, [r0]
	adds r6, r1, r2
	mov r3, r9
	mov r1, r9
	strh r6, [r3, #2]
	strh r0, [r1, #4]
	ldr r1, [sp, #40]
	movs r0, #10
	adds r1, #1
	adds r0, #255
	str r1, [sp, #40]
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200cfe4
	cmp r5, r10
	bne .L_0200d022
	mov r3, r9
	movs r2, #4
	ldrsh r0, [r3, r2]
	bl GameFlag_SetBit
	b .L_0200d022
.L_0200cfe4:
	mov r1, r9
	movs r5, #4
	ldrsh r0, [r1, r5]
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200d022
	mov r2, r8
	ldrh r4, [r2, #6]
	ldrh r5, [r2, #8]
	movs r3, #8
	ldrsh r1, [r2, r3]
	movs r3, #6
	ldrsh r0, [r2, r3]
	movs r2, #64
	adds r3, r2, #0
	ands r3, r4
	ands r2, r5
	lsls r3, r3, #16
	lsls r2, r2, #16
	asrs r3, r3, #16
	asrs r2, r2, #16
	orrs r7, r3
	orrs r6, r2
	adds r1, #1
	movs r2, #1
	movs r3, #1
	str r7, [sp, #0]
	str r6, [sp, #4]
	bl Func_0200579c
.L_0200d022:
	mov r0, r8
	ldrh r3, [r0, #10]
	mov r1, r8
	adds r3, #1
	strh r3, [r1, #10]
	movs r5, #8
	add r9, r5
.L_0200d030:
	ldr r2, [sp, #16]
	ldr r5, [sp, #24]
	adds r2, #1
	movs r3, #4
	str r2, [sp, #16]
	add r11, r3
	cmp r2, r5
	blt .L_0200cf90
.L_0200d040:
	ldr r0, [sp, #12]
	ldr r1, [sp, #20]
	adds r0, #1
	str r0, [sp, #12]
	cmp r0, r1
	blt .L_0200cf7c
.L_0200d04c:
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200d0a4
	ldr r3, .L_0200d100
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Actor_GetById
	ldr r3, [r0, #8]
	movs r2, #0
	asrs r4, r3, #20
	ldr r3, [r0, #16]
	mov r0, r8
	asrs r1, r3, #20
	ldr r3, [sp, #8]
	mov r9, r3
	movs r5, #10
	ldrsh r3, [r0, r5]
	cmp r2, r3
	bge .L_0200d0a4
.L_0200d07e:
	mov r0, r9
	movs r5, #0
	ldrsh r3, [r0, r5]
	cmp r3, r4
	bne .L_0200d094
	movs r5, #2
	ldrsh r3, [r0, r5]
	cmp r3, r1
	bne .L_0200d094
	mov r0, r8
	strh r2, [r0, #12]
.L_0200d094:
	movs r3, #8
	mov r0, r8
	add r9, r3
	movs r5, #10
	ldrsh r3, [r0, r5]
	adds r2, #1
	cmp r2, r3
	blt .L_0200d07e
.L_0200d0a4:
	movs r0, #128
	lsls r0, r0, #1
	bl Heap_Alloc
	adds r5, r0, #0
	adds r1, r5, #0
	movs r2, #63
.L_0200d0b2:
	ldr r3, .L_0200d104
	subs r2, #1
	stmia r1!, {r3}
	cmp r2, #0
	bge .L_0200d0b2
	bl Resource_FindFreeEntry
	mov r1, r8
	strh r0, [r1, #16]
	lsls r0, r0, #16
	movs r1, #128
	adds r2, r5, #0
	lsls r1, r1, #1
	asrs r0, r0, #16
	bl VramCache_Load
	adds r0, r5, #0
	bl Heap_Release
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_0200d108
	bl Task_Add
	mov r3, r8
	movs r2, #10
	ldrsh r0, [r3, r2]
	add sp, #48
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200d0f8:
	.4byte Data_02006b74
.L_0200d0fc:
	.4byte IwramClearWords
.L_0200d100:
	.4byte gMainRoutineWork
.L_0200d104:
	.4byte 0x11111111
.L_0200d108:
	.4byte Func_02004d9c
	.section .text.x0200d204,"ax",%progbits
	.global Func_02005204
	.thumb_func
Func_02005204:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r1, .L_0200d2b4
	movs r2, #133
	lsls r2, r2, #2
	adds r1, r1, r2
	mov r8, r0
	ldr r0, [r1]
	sub sp, #8
	mov r10, r1
	bl Actor_GetById
	adds r6, r0, #0
	ldr r3, [r6, #8]
	movs r2, #64
	asrs r7, r3, #20
	mov r3, r8
	ldrh r1, [r3, #6]
	adds r3, r2, #0
	ands r3, r1
	lsls r3, r3, #16
	mov r1, r8
	asrs r3, r3, #16
	orrs r7, r3
	ldrh r3, [r1, #8]
	ldr r5, [r6, #16]
	ands r2, r3
	lsls r2, r2, #16
	asrs r2, r2, #16
	asrs r5, r5, #20
	orrs r5, r2
	bl Func_0200510c
	movs r0, #229
	bl Func_020059f4
	mov r3, r8
	movs r2, #8
	ldrsh r1, [r3, r2]
	movs r2, #6
	ldrsh r0, [r3, r2]
	adds r1, #2
	movs r2, #1
	movs r3, #1
	str r7, [sp, #0]
	str r5, [sp, #4]
	bl Func_0200579c
	movs r0, #12
	bl FieldEvent_Wait
	movs r3, #128
	ldr r2, .L_0200d2ac
	lsls r3, r3, #7
	strh r3, [r6, #6]
	adds r3, r6, #0
	adds r3, #85
	strb r2, [r3]
	mov r3, r10
	ldr r0, [r3]
	bl Actor_GetById
	movs r1, #0
	bl Actor_SetSingleChildFlags
	movs r2, #226
	movs r3, #128
	lsls r2, r2, #4
	lsls r3, r3, #19
	adds r2, #255
	adds r3, #74
	strh r2, [r3]
	movs r1, #128
	lsls r1, r1, #19
	ldrh r3, [r1]
	ldr r2, .L_0200d2b0
	movs r7, #0
	orrs r3, r2
	strh r3, [r1]
	mov r2, r10
	b .L_0200d2b8
	.2byte 0x0000
.L_0200d2ac:
	.4byte 0x00000000
.L_0200d2b0:
	.4byte 0x00008000
.L_0200d2b4:
	.4byte gMainRoutineWork
.L_0200d2b8:
	movs r3, #1
	mov r1, r8
	strh r3, [r1, #14]
	ldr r0, [r2]
	movs r1, #28
	bl Actor_SetModeById
	movs r0, #16
	bl FieldEvent_Wait
.L_0200d2cc:
	cmp r7, #5
	bne .L_0200d2d6
	movs r0, #204
	bl Func_020059f4
.L_0200d2d6:
	ldr r3, [r6, #24]
	ldr r1, .L_0200d334
	ldr r2, .L_0200d338
	adds r3, r3, r1
	str r3, [r6, #24]
	ldr r3, [r6, #28]
	ldr r1, .L_0200d33c
	adds r3, r3, r2
	str r3, [r6, #28]
	ldr r3, [r6, #12]
	movs r0, #1
	adds r3, r3, r1
	str r3, [r6, #12]
	adds r7, #1
	bl wait
	cmp r7, #39
	ble .L_0200d2cc
	ldr r3, .L_0200d340
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Actor_GetById
	movs r3, #0
	adds r0, #84
	strb r3, [r0]
	mov r1, r8
	strh r3, [r1, #14]
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #93
	str r2, [r3]
	bl Event_FinishScreenTransition
	bl Event_WaitTransitionFrames
	add sp, #8
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_0200d334:
	.4byte 0xfffffc00
.L_0200d338:
	.4byte 0xfffffd00
.L_0200d33c:
	.4byte 0xffff6667
.L_0200d340:
	.4byte gMainRoutineWork
	.section .text.x0200d39c,"ax",%progbits
	.balign 4
	.global Func_0200539c
	.thumb_func
Func_0200539c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, .L_0200d520
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	sub sp, #68
	bl Actor_GetById
	adds r7, r0, #0
	bl Func_02005814
	movs r0, #0
	bl Func_02005984
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r1, r1
	negs r2, r2
	movs r3, #0
	negs r0, r0
	bl Camera_MoveWithinMapBounds
	bl Func_0200577c
	movs r0, #1
	bl wait
	movs r3, #130
	lsls r3, r3, #16
	str r3, [r7, #12]
	movs r3, #128
	lsls r3, r3, #8
	adds r5, r7, #0
	str r3, [r7, #72]
	adds r5, #85
	movs r3, #0
	str r3, [r7, #68]
	strb r3, [r5]
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r4, #214
	lsls r4, r4, #1
	movs r2, #128
	adds r3, r3, r4
	lsls r2, r2, #1
	str r2, [r3]
	bl Event_StartScreenTransition
	bl Event_WaitTransitionFrames
	movs r0, #204
	bl Func_020059f4
	movs r3, #3
	strb r3, [r5]
	movs r0, #24
	bl FieldEvent_Wait
	add r2, sp, #28
	movs r3, #7
	str r3, [r2, #4]
	ldr r3, .L_0200d524
	mov r8, r2
	str r3, [r2, #36]
	movs r3, #163
	lsls r3, r3, #8
	adds r3, #215
	str r3, [r2, #8]
	str r3, [r2, #12]
	movs r3, #0
	mov r10, r3
.L_0200d436:
	mov r4, r10
	lsls r5, r4, #12
	adds r0, r5, #0
	bl Math_Cosine
	add r6, sp, #16
	movs r3, #0
	str r0, [r6]
	adds r0, r5, #0
	str r3, [r6, #4]
	bl Math_Sine
	ldr r3, [r6]
	str r0, [r6, #8]
	asrs r2, r3, #1
	adds r3, r3, r2
	str r3, [r6]
	bl Random16Far
	lsls r3, r0, #1
	ldr r2, [r6]
	adds r3, r3, r0
	lsls r3, r3, #14
	lsrs r3, r3, #16
	adds r2, r2, r3
	ldr r3, .L_0200d528
	adds r2, r2, r3
	str r2, [r6]
	bl Random16Far
	lsls r3, r0, #1
	ldr r5, [r6, #8]
	adds r3, r3, r0
	ldr r4, .L_0200d52c
	lsls r3, r3, #13
	lsrs r3, r3, #16
	adds r5, r5, r3
	adds r5, r5, r4
	ldr r4, [r6, #4]
	str r5, [r6, #8]
	ldr r2, [r7, #16]
	ldr r3, [r6]
	ldr r0, [r7, #8]
	ldr r1, [r7, #12]
	str r4, [sp, #0]
	ldr r4, .L_0200d530
	str r5, [sp, #4]
	str r4, [sp, #8]
	mov r4, r8
	str r4, [sp, #12]
	bl Func_02004bd4
	movs r2, #1
	add r10, r2
	mov r3, r10
	cmp r3, #16
	bls .L_0200d436
	movs r0, #188
	bl Func_020059f4
	ldr r5, .L_0200d520
	movs r4, #133
	lsls r4, r4, #2
	adds r5, r5, r4
	movs r1, #2
	ldr r0, [r5]
	adds r1, #255
	bl Func_02005904
	ldr r0, [r5]
	movs r1, #49
	bl Actor_SetModeById
	movs r0, #160
	movs r1, #160
	movs r2, #128
	lsls r0, r0, #11
	lsls r1, r1, #11
	lsls r2, r2, #9
	bl Func_020057ac
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	adds r2, #102
	negs r0, r0
	negs r1, r1
	bl Func_020057ac
	bl Func_020057b4
	movs r1, #128
	ldr r0, [r5]
	lsls r1, r1, #1
	bl Func_02005904
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r7, #72]
	movs r3, #128
	lsls r3, r3, #7
	str r3, [r7, #68]
	movs r0, #10
	bl FieldEvent_Wait
	ldr r0, [r5]
	movs r1, #1
	bl Actor_SetModeById
	bl Func_0200581c
	add sp, #68
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_0200d520:
	.4byte gMainRoutineWork
.L_0200d524:
	.4byte Effect_AdvanceMotion
.L_0200d528:
	.4byte 0xffffa000
.L_0200d52c:
	.4byte 0xffffd000
.L_0200d530:
	.4byte 0x01090001
	.section .text.x0200d534,"ax",%progbits
	.global Func_02005534
	.thumb_func
Func_02005534:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, .L_0200d5dc
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	ldr r0, [r3]
	bl Actor_GetById
	ldr r3, [r0, #8]
	ldr r6, .L_0200d5e0
	asrs r3, r3, #20
	mov r8, r3
	ldr r3, [r0, #16]
	adds r5, r6, #0
	asrs r3, r3, #20
	mov r10, r3
	movs r1, #10
	ldrsh r3, [r6, r1]
	movs r7, #0
	adds r5, #32
	ldrh r2, [r6, #10]
	cmp r7, r3
	bge .L_0200d5d0
.L_0200d568:
	movs r1, #0
	ldrsh r3, [r5, r1]
	cmp r3, r8
	bne .L_0200d5c4
	movs r1, #2
	ldrsh r3, [r5, r1]
	cmp r3, r10
	bne .L_0200d5c4
	movs r2, #4
	ldrsh r0, [r5, r2]
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200d598
	adds r0, r6, #0
	adds r1, r5, #0
	bl Func_02005194
	movs r3, #4
	ldrsh r0, [r5, r3]
	bl GameFlag_SetBit
	strh r7, [r6, #12]
	b .L_0200d5d0
.L_0200d598:
	movs r1, #12
	ldrsh r3, [r6, r1]
	cmp r7, r3
	beq .L_0200d5d0
	adds r0, r6, #0
	adds r1, r5, #0
	strh r7, [r6, #12]
	bl Func_02005204
	movs r2, #2
	ldrsh r0, [r6, r2]
	mov r1, r8
	bl GameFlag_SetByte
	movs r3, #2
	ldrsh r0, [r6, r3]
	mov r1, r10
	adds r0, #8
	bl GameFlag_SetByte
	movs r0, #1
	b .L_0200d5d2
.L_0200d5c4:
	lsls r3, r2, #16
	adds r7, #1
	asrs r3, r3, #16
	adds r5, #8
	cmp r7, r3
	blt .L_0200d568
.L_0200d5d0:
	movs r0, #0
.L_0200d5d2:
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200d5dc:
	.4byte gMainRoutineWork
.L_0200d5e0:
	.4byte Data_02006b74
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
