.syntax unified
	.thumb
	.section .text.x02008138,"ax",%progbits
	.balign 4
	.global Func_02000138
	.thumb_func
Func_02000138:
	push {r5, r6, lr}
	adds r6, r0, #0
	subs r3, r6, #1
	ldr r5, .L_02008210
	cmp r3, #1
	bhi .L_02008194
	movs r0, #22
	ldrsh r3, [r5, r0]
	ldrh r2, [r5, #22]
	cmp r3, #0
	bgt .L_02008184
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200817c
	movs r0, #0
	bl Func_02000330
	cmp r0, #0
	bne .L_0200820c
	ldr r3, .L_02008214
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	movs r2, #16
	ldr r0, [r3]
	movs r1, #0
	negs r2, r2
	bl ObjectMotion_CommitPositionAndActivate
	b .L_0200820c
.L_0200817c:
	adds r0, r6, #4
	bl Func_02005990
	b .L_0200818e
.L_02008184:
	subs r3, r2, #1
	strh r3, [r5, #22]
	adds r0, r6, #0
	bl Func_02005990
.L_0200818e:
	movs r0, #123
	bl Func_02005a68
.L_02008194:
	subs r3, r6, #3
	cmp r3, #1
	bhi .L_020081fa
	ldrh r3, [r5, #22]
	movs r2, #192
	adds r3, #1
	strh r3, [r5, #22]
	lsls r2, r2, #10
	lsls r3, r3, #16
	cmp r3, r2
	ble .L_020081de
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020081cc
	movs r0, #0
	bl Func_020000fc
	cmp r0, #0
	bne .L_020081cc
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #133
	bl GameFlag_SetBit
.L_020081cc:
	movs r0, #128
	bl Func_02005a68
	bl ObjectEffect_BeginContextEffect26
	adds r0, r6, #4
	bl Func_02005990
	b .L_020081ea
.L_020081de:
	adds r0, r6, #0
	bl Func_02005990
	movs r0, #123
	bl Func_02005a68
.L_020081ea:
	movs r3, #22
	ldrsh r2, [r5, r3]
	movs r0, #26
	ldrsh r3, [r5, r0]
	ldrh r1, [r5, #22]
	cmp r2, r3
	ble .L_020081fa
	strh r1, [r5, #26]
.L_020081fa:
	movs r1, #22
	ldrsh r3, [r5, r1]
	movs r0, #18
	ldrsh r2, [r5, r0]
	lsls r3, r3, #1
	adds r3, r3, r2
	lsls r3, r3, #1
	ldrh r3, [r5, r3]
	strh r3, [r5, #16]
.L_0200820c:
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02008210:
	.4byte Data_020023c4 + 0x88
.L_02008214:
	.4byte gPartyState
	.section .text.x02008330,"ax",%progbits
	.balign 4
	.global Func_02000330
	.thumb_func
Func_02000330:
	push {r5, r6, r7, lr}
	ldr r3, .L_02008410
	movs r2, #133
	lsls r2, r2, #2
	adds r7, r3, r2
	ldr r0, [r7]
	bl Object_GetById
	adds r5, r0, #0
	movs r0, #144
	movs r3, #192
	lsls r0, r0, #4
	lsls r3, r3, #18
	adds r0, #255
	ldr r6, [r3, #108]
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008400
	bl Func_02005880
	movs r0, #0
	bl Func_020059e8
	ldr r1, [r5, #8]
	ldr r2, [r5, #16]
	asrs r1, r1, #20
	asrs r2, r2, #20
	lsls r1, r1, #4
	lsls r2, r2, #4
	adds r1, #8
	adds r2, #8
	ldr r0, [r7]
	bl ObjectMotion_SetPositionAndReset
	movs r0, #229
	bl Func_02005a68
	movs r1, #128
	movs r2, #0
	ldr r0, [r7]
	lsls r1, r1, #7
	bl ObjectMotion_ArmCallback
	ldr r5, .L_02008414
	movs r1, #13
	adds r0, r5, #0
	bl UiText_ShowPositionedMessageAndWait
	movs r2, #203
	lsls r2, r2, #4
	adds r3, r6, r2
	adds r5, #1
	strh r5, [r3]
	movs r3, #192
	lsls r3, r3, #4
	adds r3, #178
	adds r2, r6, r3
	movs r3, #4
	strh r3, [r2]
	movs r3, #192
	lsls r3, r3, #4
	adds r3, #180
	adds r2, r6, r3
	movs r3, #1
	strh r3, [r2]
	ldr r0, [r7]
	movs r1, #1
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_02008400
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #7
	ldr r0, [r7]
	bl ObjectMotion_ArmCallback
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #22
	ldr r0, [r7]
	bl Object_SetModeById
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #10
	adds r1, #255
	movs r2, #0
	ldr r0, [r7]
	bl Func_02005960
	movs r0, #30
	bl Battle_WaitMode0
	bl Func_02005868
	movs r0, #9
	bl Func_02005990
	movs r0, #1
	b .L_0200840c
.L_02008400:
	movs r0, #193
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
	movs r0, #0
.L_0200840c:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008410:
	.4byte gPartyState
.L_02008414:
	.4byte MsgTrialRoadGiveUpPrompt
	.section .text.x02008428,"ax",%progbits
	.balign 4
	.global Func_02000428
	.thumb_func
Func_02000428:
	push {r5, r6, r7, lr}
	ldr r5, .L_020084ec
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
	ldr r0, [r5]
	bl Object_GetById
	ldr r7, .L_020084f0
	adds r6, r0, #0
	bl Func_02005880
	movs r0, #0
	bl Func_020059e8
	ldr r1, [r6, #8]
	ldr r2, [r6, #16]
	asrs r1, r1, #20
	asrs r2, r2, #20
	lsls r1, r1, #4
	lsls r2, r2, #4
	ldr r0, [r5]
	adds r1, #8
	adds r2, #8
	bl ObjectMotion_SetPositionAndReset
	bl TrialRoad_Idle
	movs r0, #0
	bl Func_020000fc
	adds r5, r0, #0
	cmp r5, #0
	beq .L_0200847c
	movs r3, #22
	ldrsh r0, [r7, r3]
	movs r2, #192
	lsls r2, r2, #2
	adds r2, #129
	adds r0, r0, r2
	bl GameFlag_SetBit
.L_0200847c:
	movs r0, #124
	bl Func_02005a68
	movs r2, #18
	ldrsh r3, [r7, r2]
	cmp r3, #0
	bne .L_020084b4
	movs r0, #9
	movs r1, #6
	bl Object_SetModeById
	movs r0, #195
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
	cmp r5, #0
	beq .L_020084dc
	movs r0, #10
	movs r1, #6
	bl Object_SetModeById
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #134
	bl GameFlag_SetBit
	b .L_020084dc
.L_020084b4:
	movs r0, #11
	movs r1, #6
	bl Object_SetModeById
	movs r0, #195
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
	cmp r5, #0
	beq .L_020084dc
	movs r0, #12
	movs r1, #6
	bl Object_SetModeById
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #134
	bl GameFlag_SetBit
.L_020084dc:
	movs r0, #161
	lsls r0, r0, #2
	bl GameFlag_SetBit
	bl Func_02005888
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_020084ec:
	.4byte gPartyState
.L_020084f0:
	.4byte Data_020023c4 + 0x88
	.section .text.x020084f4,"ax",%progbits
	.balign 4
	.global Func_020004f4
	.thumb_func
Func_020004f4:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	adds r7, r1, #0
	mov r10, r2
	adds r5, r0, #0
	bl Func_02005a40
	bl Func_020059b8
	ldr r2, .L_020085ac
	adds r6, r0, #0
	mov r8, r2
	bl Func_02005880
	movs r0, #0
	bl Func_020059e8
	adds r0, r7, #0
	movs r1, #1
	bl Func_02005858
	adds r0, r5, #0
	movs r1, #2
	bl Func_02005858
	movs r1, #1
	ldr r0, .L_020085b0
	bl UiText_ShowPositionedMessageAndWait
	movs r0, #125
	bl Func_02005a68
	adds r0, r6, #0
	movs r1, #5
	bl Motion_SetModeAndWaitAnimation
	cmp r6, #9
	beq .L_02008548
	cmp r6, #11
	bne .L_02008554
.L_02008548:
	movs r0, #195
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_ClearBit
	b .L_0200855e
.L_02008554:
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #134
	bl GameFlag_ClearBit
.L_0200855e:
	movs r0, #195
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008598
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #134
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008598
	mov r2, r8
	movs r3, #18
	ldrsh r0, [r2, r3]
	movs r3, #22
	ldrsh r1, [r2, r3]
	bl Func_02000218
	movs r0, #138
	bl Func_02005a68
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #130
	bl GameFlag_SetBit
.L_02008598:
	mov r1, r10
	adds r0, r7, #0
	bl Func_02005870
	bl Func_02005888
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_020085ac:
	.4byte Data_020023c4 + 0x88
.L_020085b0:
	.4byte MsgTrialRoadStoredItem
	.section .text.x02008a18,"ax",%progbits
	.balign 4
	.global Func_02000a18
	.thumb_func
Func_02000a18:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	adds r5, r0, #0
	mov r11, r1
	adds r3, r5, #0
	ldr r1, .L_02008b28
	mov r10, r2
	subs r3, #18
	movs r2, #3
	ands r3, r2
	ldrsb r3, [r1, r3]
	mov r9, r3
	bl Object_GetById
	mov r8, r0
	mov r0, r9
	bl Object_GetById
	ldr r3, .L_02008b2c
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	adds r7, r0, #0
	ldr r0, [r3]
	bl Object_GetById
	mov r3, r10
	adds r6, r0, #0
	cmp r3, #10
	bgt .L_02008a68
	lsls r2, r3, #12
	movs r3, #128
	lsls r3, r3, #9
	subs r3, r3, r2
	b .L_02008a6c
.L_02008a68:
	movs r3, #128
	lsls r3, r3, #9
.L_02008a6c:
	mov r2, r8
	str r3, [r2, #28]
	mov r3, r11
	cmp r3, #1
	bne .L_02008af4
	adds r0, r5, #0
	bl Func_02000924
	mov r0, r9
	bl Func_0200095c
	cmp r5, #19
	beq .L_02008aa6
	cmp r5, #19
	bgt .L_02008a90
	cmp r5, #18
	beq .L_02008a9a
	b .L_02008ac8
.L_02008a90:
	cmp r5, #20
	beq .L_02008ab2
	cmp r5, #21
	beq .L_02008abe
	b .L_02008ac8
.L_02008a9a:
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #53
	bl GameFlag_SetBit
	b .L_02008ac8
.L_02008aa6:
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #53
	bl GameFlag_ClearBit
	b .L_02008ac8
.L_02008ab2:
	movs r0, #142
	lsls r0, r0, #2
	adds r0, #255
	bl GameFlag_SetBit
	b .L_02008ac8
.L_02008abe:
	movs r0, #142
	lsls r0, r0, #2
	adds r0, #255
	bl GameFlag_ClearBit
.L_02008ac8:
	ldr r3, [r7, #8]
	ldr r4, [r6, #8]
	asrs r3, r3, #20
	asrs r2, r4, #20
	cmp r3, r2
	bne .L_02008af4
	ldr r3, [r7, #16]
	ldr r0, [r6, #16]
	asrs r3, r3, #20
	asrs r2, r0, #20
	cmp r3, r2
	bne .L_02008af4
	ldr r1, .L_02008b30
	movs r3, #128
	lsls r3, r3, #12
	ands r4, r1
	ands r0, r1
	adds r2, r4, r3
	str r3, [r6, #40]
	adds r3, r0, r3
	str r2, [r6, #8]
	str r3, [r6, #16]
.L_02008af4:
	mov r2, r10
	cmp r2, #9
	bne .L_02008afe
	ldr r3, .L_02008b34
	b .L_02008b1a
.L_02008afe:
	mov r3, r10
	cmp r3, #11
	bne .L_02008b08
	ldr r3, .L_02008b38
	b .L_02008b1a
.L_02008b08:
	mov r2, r10
	cmp r2, #13
	bne .L_02008b16
	movs r3, #134
	lsls r3, r3, #9
	adds r3, #204
	b .L_02008b1a
.L_02008b16:
	movs r3, #128
	lsls r3, r3, #9
.L_02008b1a:
	str r3, [r7, #28]
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_02008b28:
	.4byte Data_02005a88
.L_02008b2c:
	.4byte gPartyState
.L_02008b30:
	.4byte 0xfff00000
.L_02008b34:
	.4byte 0x00013333
.L_02008b38:
	.4byte 0x00011999
	.section .text.x02008b7c,"ax",%progbits
	.balign 4
	.global Func_02000b7c
	.thumb_func
Func_02000b7c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	adds r6, r1, #0
	mov r11, r3
	mov r9, r2
	mov r8, r0
	bl Object_GetById
	adds r7, r0, #0
	adds r0, r6, #0
	bl Object_GetById
	adds r3, r7, #0
	adds r3, #99
	ldrb r3, [r3]
	mov r10, r0
	cmp r3, #0
	bne .L_02008c26
	bl Func_02005880
	movs r5, #128
	movs r0, #0
	bl Func_020059e8
	lsls r5, r5, #6
	mov r3, r10
	str r5, [r3, #28]
	movs r0, #1
	bl Task_Wait
	adds r0, r6, #0
	bl Func_0200095c
	adds r6, r5, #0
.L_02008bca:
	movs r3, #128
	lsls r3, r3, #8
	subs r1, r3, r6
	cmp r1, #0
	bge .L_02008bdc
	movs r3, #224
	lsls r3, r3, #3
	adds r3, #255
	adds r1, r1, r3
.L_02008bdc:
	movs r5, #128
	asrs r1, r1, #11
	mov r0, r8
	lsls r5, r5, #9
	bl Func_0200070c
	subs r3, r5, r6
	str r3, [r7, #28]
	mov r3, r10
	str r6, [r3, #28]
	movs r0, #1
	bl Task_Wait
	movs r3, #128
	lsls r3, r3, #6
	adds r6, r6, r3
	cmp r6, r5
	ble .L_02008bca
	mov r0, r8
	str r5, [r7, #28]
	bl Func_02000924
	movs r0, #138
	bl Func_02005a68
	mov r3, r11
	cmp r3, #0
	beq .L_02008c1c
	mov r0, r9
	bl GameFlag_SetBit
	b .L_02008c22
.L_02008c1c:
	mov r0, r9
	bl GameFlag_ClearBit
.L_02008c22:
	bl Func_02005888
.L_02008c26:
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x02008cf8,"ax",%progbits
	.balign 4
	.global Func_02000cf8
	.thumb_func
Func_02000cf8:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r6, r1, #0
	ldr r5, [r3, #32]
	bl Object_GetById
	cmp r0, #0
	beq .L_02008d6a
	ldr r3, [r0, #16]
	ldr r2, [r0, #8]
	asrs r3, r3, #20
	movs r1, #156
	lsls r3, r3, #7
	lsls r1, r1, #1
	asrs r2, r2, #20
	adds r2, r2, r3
	adds r3, r5, r1
	ldr r3, [r3]
	lsls r2, r2, #2
	adds r1, #112
	adds r4, r3, r2
	adds r3, r5, r1
	ldr r3, [r3]
	adds r0, r3, r2
	cmp r6, #0
	bne .L_02008d54
	ldr r3, .L_02008d6c
	adds r1, #88
	adds r2, r4, r3
	movs r3, #255
	strb r3, [r2, #2]
	movs r3, #1
	negs r3, r3
	adds r2, r4, r1
	strb r3, [r4, #2]
	strb r3, [r2, #2]
	ldr r3, .L_02008d6c
	adds r1, r0, r1
	adds r2, r0, r3
	movs r3, #1
	negs r3, r3
	strb r3, [r2, #2]
	strb r3, [r0, #2]
	strb r3, [r1, #2]
	b .L_02008d6a
.L_02008d54:
	subs r2, r4, #4
	movs r3, #255
	strb r3, [r2, #2]
	movs r3, #1
	negs r3, r3
	subs r2, r0, #4
	strb r3, [r4, #2]
	strb r3, [r4, #6]
	strb r3, [r2, #2]
	strb r3, [r0, #2]
	strb r3, [r0, #6]
.L_02008d6a:
	pop {r5, r6, pc}
.L_02008d6c:
	.4byte 0xfffffe00
	.section .text.x02008e40,"ax",%progbits
	.balign 4
	.global Func_02000e40
	.thumb_func
Func_02000e40:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #36
	mov r9, r1
	mov r11, r2
	str r3, [sp, #16]
	str r0, [sp, #20]
	bl Object_GetById
	ldr r3, .L_02009068
	adds r7, r0, #0
	movs r0, #133
	lsls r0, r0, #2
	adds r3, r3, r0
	ldr r0, [r3]
	bl Object_GetById
	ldr r3, [r7, #8]
	asrs r3, r3, #20
	str r3, [sp, #12]
	ldr r3, [r7, #16]
	ldr r2, [sp, #12]
	asrs r3, r3, #20
	str r3, [sp, #8]
	mov r8, r3
	movs r3, #2
	str r3, [sp, #0]
	mov r10, r2
	ldr r3, [r0, #8]
	asrs r5, r3, #20
	ldr r3, [r0, #16]
	mov r0, r9
	asrs r6, r3, #20
	cmp r0, #0
	beq .L_02008e96
	adds r3, r5, r0
	cmp r10, r3
	bne .L_02008ea2
.L_02008e96:
	mov r2, r11
	cmp r2, #0
	beq .L_02008eb4
	adds r3, r6, r2
	cmp r8, r3
	beq .L_02008eb4
.L_02008ea2:
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r0, #194
	lsls r0, r0, #1
	adds r3, r3, r0
	movs r2, #13
	strh r2, [r3]
	b .L_0200905a
.L_02008eb4:
	movs r2, #0
	str r2, [sp, #4]
	b .L_02008edc
.L_02008eba:
	adds r0, r5, #0
	adds r1, r6, #0
	bl Func_02000df8
	cmp r0, #0
	bne .L_02008f22
	adds r0, r5, #1
	adds r1, r6, #0
	bl Func_02000df8
	cmp r0, #0
	bne .L_02008f22
.L_02008ed2:
	ldr r3, [sp, #4]
	mov r10, r5
	adds r3, #1
	str r3, [sp, #4]
	mov r8, r6
.L_02008edc:
	ldr r0, [sp, #4]
	cmp r0, #5
	bgt .L_02008f22
	ldr r2, [sp, #16]
	mov r5, r10
	mov r6, r8
	add r5, r9
	add r6, r11
	cmp r2, #0
	bne .L_02008f16
	subs r1, r6, #1
	adds r0, r5, #0
	bl Func_02000df8
	cmp r0, #0
	bne .L_02008f22
	adds r0, r5, #0
	adds r1, r6, #0
	bl Func_02000df8
	cmp r0, #0
	bne .L_02008f22
	adds r1, r6, #1
	adds r0, r5, #0
	bl Func_02000df8
	cmp r0, #0
	beq .L_02008ed2
	b .L_02008f22
.L_02008f16:
	subs r0, r5, #1
	adds r1, r6, #0
	bl Func_02000df8
	cmp r0, #0
	beq .L_02008eba
.L_02008f22:
	ldr r3, [sp, #12]
	cmp r3, r10
	bne .L_02008f30
	ldr r0, [sp, #8]
	cmp r0, r8
	bne .L_02008f30
	b .L_0200905a
.L_02008f30:
	bl Func_02005880
	movs r0, #0
	bl Func_020059e8
	ldr r3, .L_02009068
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	movs r1, #8
	bl Object_SetModeById
	movs r0, #6
	bl Battle_WaitMode0
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r7, #48]
	movs r3, #204
	lsls r3, r3, #6
	adds r3, #51
	str r3, [r7, #52]
	movs r0, #239
	bl Func_02005a68
	mov r3, r9
	cmp r3, #0
	blt .L_02008f70
	mov r0, r11
	cmp r0, #0
	bge .L_02008f74
.L_02008f70:
	movs r2, #3
	str r2, [sp, #0]
.L_02008f74:
	ldr r1, [sp, #0]
	adds r0, r7, #0
	bl Func_020057c8
	mov r3, r10
	mov r0, r8
	movs r2, #128
	lsls r2, r2, #12
	lsls r1, r3, #20
	lsls r3, r0, #20
	adds r6, r3, r2
	adds r3, r6, #0
	adds r1, r1, r2
	adds r0, r7, #0
	movs r2, #0
	bl Func_020057f8
	movs r0, #6
	bl Battle_WaitMode0
	ldr r5, .L_02009068
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
	ldr r0, [r5]
	movs r1, #2
	bl Object_SetModeById
	movs r1, #152
	movs r2, #204
	lsls r1, r1, #7
	lsls r2, r2, #6
	ldr r0, [r5]
	adds r1, #204
	adds r2, #51
	bl ObjectMotion_SetSpeedParameters
	mov r3, r9
	lsls r1, r3, #3
	mov r3, r11
	lsls r2, r3, #3
	ldr r0, [r5]
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r0, #24
	bl Battle_WaitMode0
	ldr r0, [r5]
	movs r1, #1
	bl Object_SetModeById
	adds r0, r7, #0
	bl Func_02005800
	mov r0, r10
	cmp r0, #17
	bgt .L_02008ffe
	adds r0, r7, #0
	movs r1, #1
	bl Func_020057c8
	movs r0, #149
	lsls r0, r0, #1
	bl Func_02005a68
	movs r0, #213
	bl Func_02005a68
	b .L_0200904c
.L_02008ffe:
	adds r0, r7, #0
	movs r1, #3
	bl Func_020057c8
	movs r1, #156
	movs r2, #0
	adds r3, r6, #0
	lsls r1, r1, #17
	adds r0, r7, #0
	bl Func_020057f8
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #8
	adds r0, r7, #0
	bl Func_020057c8
	adds r0, r7, #0
	bl Func_02005800
	adds r3, r7, #0
	adds r3, #35
	movs r2, #2
	movs r0, #149
	strb r2, [r3]
	lsls r0, r0, #1
	bl Func_02005a68
	movs r0, #240
	bl Func_02005a68
	movs r3, #162
	ldr r2, [sp, #20]
	lsls r3, r3, #1
	adds r3, #255
	adds r0, r2, r3
	bl GameFlag_SetBit
.L_0200904c:
	movs r0, #15
	bl Battle_WaitMode0
	bl Func_02005888
	bl Func_02000d70
.L_0200905a:
	add sp, #36
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_02009068:
	.4byte gPartyState
	.section .text.x02009262,"ax",%progbits
	.2byte 0x0000
	.section .text.x0200929a,"ax",%progbits
	.2byte 0x0000
	.section .text.x0200929c,"ax",%progbits
	.balign 4
	.global Func_0200129c
	.thumb_func
Func_0200129c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	mov r11, r3
	ldr r3, .L_02009454
	sub sp, #4
	mov r10, r2
	movs r2, #133
	lsls r2, r2, #2
	adds r6, r1, #0
	ldr r1, [sp, #44]
	adds r3, r3, r2
	adds r5, r0, #0
	ldr r0, [r3]
	mov r8, r1
	ldr r7, [sp, #48]
	bl Object_GetById
	movs r3, #128
	lsls r3, r3, #13
	mov r1, r8
	ands r3, r1
	mov r9, r0
	cmp r3, #0
	beq .L_020092e4
	cmp r7, #0
	beq .L_020092e4
	movs r2, #24
	ldrsh r0, [r7, r2]
	adds r1, r5, #0
	adds r2, r6, #0
	b .L_020092ec
.L_020092e4:
	movs r0, #30
	adds r2, r6, #0
	adds r0, #255
	adds r1, r5, #0
.L_020092ec:
	mov r3, r10
	bl Func_020057d8
	adds r6, r0, #0
	cmp r6, #0
	bne .L_020092fa
	b .L_02009446
.L_020092fa:
	ldr r3, [r6, #80]
	mov r1, r8
	movs r5, #15
	adds r1, #1
	ands r1, r5
	adds r0, r6, #0
	str r3, [sp, #0]
	bl Func_020057c8
	ldr r2, .L_02009458
	mov r3, r8
	ands r3, r5
	lsls r3, r3, #2
	ldr r1, [r2, r3]
	adds r0, r6, #0
	mov r10, r3
	bl Func_020057d0
	adds r3, r6, #0
	movs r5, #0
	adds r3, #85
	strb r5, [r3]
	adds r0, r6, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	ldr r3, .L_0200945c
	mov r1, r9
	str r3, [r6, #108]
	mov r3, r11
	str r3, [r6, #68]
	ldr r3, [sp, #36]
	adds r0, r6, #0
	str r3, [r6, #72]
	ldr r3, [sp, #40]
	str r3, [r6, #76]
	ldr r3, [r1, #80]
	ldrb r1, [r3, #9]
	lsls r1, r1, #28
	lsrs r1, r1, #30
	bl Object_SetSpritePriority
	movs r2, #100
	adds r2, r2, r6
	mov r9, r2
	mov r3, r9
	str r5, [r6, #48]
	str r5, [r6, #52]
	strh r5, [r3]
	ldr r3, .L_02009460
	mov r1, r8
	ands r3, r1
	cmp r3, #0
	beq .L_02009446
	cmp r7, #0
	beq .L_02009446
	movs r3, #128
	lsls r3, r3, #9
	ands r3, r1
	cmp r3, #0
	beq .L_0200937c
	ldr r1, [r7, #4]
	adds r0, r6, #0
	bl Object_SetPartAttribute
.L_0200937c:
	movs r3, #128
	lsls r3, r3, #10
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_0200939c
	adds r1, r6, #0
	adds r1, #35
	ldrb r3, [r1]
	movs r2, #254
	ands r2, r3
	strb r2, [r1]
	ldr r1, [r7]
	adds r0, r6, #0
	bl Object_SetSpritePriority
.L_0200939c:
	movs r2, #128
	lsls r2, r2, #12
	mov r3, r8
	ands r2, r3
	cmp r2, #0
	beq .L_020093b0
	ldr r3, [r7, #8]
	str r3, [r6, #24]
	ldr r3, [r7, #12]
	str r3, [r6, #28]
.L_020093b0:
	movs r3, #128
	lsls r3, r3, #11
	mov r1, r8
	ands r3, r1
	cmp r3, #0
	beq .L_020093f6
	ldr r3, .L_02009458
	mov r1, r10
	ldr r5, [r3, r1]
	ldr r3, [r7, #16]
	ldr r1, [r5, #12]
	cmp r2, #0
	beq .L_020093de
	ldr r0, [r6, #24]
	subs r0, r3, r0
	bl __divsi3
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, [r6, #28]
	ldr r1, [r5, #12]
	subs r0, r0, r3
	b .L_020093f0
.L_020093de:
	ldr r2, .L_02009460
	adds r0, r3, r2
	bl __divsi3
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, .L_02009460
	ldr r1, [r5, #12]
	adds r0, r0, r3
.L_020093f0:
	bl __divsi3
	str r0, [r6, #52]
.L_020093f6:
	movs r3, #128
	lsls r3, r3, #14
	mov r1, r8
	ands r3, r1
	cmp r3, #0
	beq .L_02009412
	adds r0, r6, #0
	movs r1, #1
	bl Func_020057c8
	ldr r1, [r7, #28]
	adds r0, r6, #0
	bl Func_020057d0
.L_02009412:
	movs r3, #128
	lsls r3, r3, #15
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_02009424
	ldrh r3, [r7, #32]
	ldr r1, [sp, #0]
	strh r3, [r1, #18]
.L_02009424:
	movs r3, #128
	lsls r3, r3, #16
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_02009436
	ldrh r3, [r7, #34]
	mov r1, r9
	strh r3, [r1]
.L_02009436:
	movs r3, #128
	lsls r3, r3, #17
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_02009446
	ldr r3, [r7, #36]
	str r3, [r6, #108]
.L_02009446:
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_02009454:
	.4byte gPartyState
.L_02009458:
	.4byte Data_02007bc0
.L_0200945c:
	.4byte BattleFx_UpdateObjectMotionScaleAndLinkedAngle
.L_02009460:
	.4byte 0xffff0000
	.section .text.x02009464,"ax",%progbits
	.balign 4
	.global Func_02001464
	.thumb_func
Func_02001464:
	push {r5, r6, r7, lr}
	adds r6, r0, #0
	ldr r5, [r6, #68]
	ldr r3, [r6, #8]
	ldr r2, [r6, #72]
	adds r3, r3, r5
	str r3, [r6, #8]
	ldr r3, [r6, #12]
	ldr r7, [r6, #76]
	adds r3, r3, r2
	str r3, [r6, #12]
	ldr r3, [r6, #16]
	adds r0, r5, #0
	adds r3, r3, r7
	movs r1, #18
	str r3, [r6, #16]
	bl __divsi3
	subs r5, r5, r0
	str r5, [r6, #68]
	adds r3, r7, #0
	cmp r7, #0
	bge .L_02009494
	adds r3, #15
.L_02009494:
	asrs r3, r3, #4
	subs r3, r7, r3
	str r3, [r6, #76]
	ldr r2, [r6, #48]
	ldr r3, [r6, #24]
	ldr r1, [r6, #80]
	adds r3, r3, r2
	str r3, [r6, #24]
	ldr r2, [r6, #52]
	ldr r3, [r6, #28]
	adds r3, r3, r2
	str r3, [r6, #28]
	adds r2, r6, #0
	adds r2, #100
	ldrh r3, [r1, #18]
	ldrh r2, [r2]
	adds r3, r3, r2
	strh r3, [r1, #18]
	ldr r3, .L_020094e0
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	ldr r3, [r0, #80]
	ldr r4, [r6, #80]
	ldrb r3, [r3, #9]
	ldrb r1, [r4, #9]
	movs r2, #12
	ands r2, r3
	movs r3, #13
	negs r3, r3
	ands r3, r1
	orrs r3, r2
	strb r3, [r4, #9]
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_020094e0:
	.4byte gPartyState
	.section .text.x020094e4,"ax",%progbits
	.balign 4
	.global Func_020014e4
	.thumb_func
Func_020014e4:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r5, .L_02009594
	movs r0, #133
	lsls r0, r0, #2
	adds r3, r5, r0
	ldr r0, [r3]
	sub sp, #68
	bl Object_GetById
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #18
	adds r5, r5, r2
	ldrb r3, [r5]
	adds r7, r0, #0
	cmp r3, #4
	beq .L_0200958a
	ldr r3, [r7, #16]
	movs r1, #128
	lsls r1, r1, #9
	adds r3, r3, r1
	str r3, [r7, #16]
	ldr r3, .L_02009598
	ldr r2, [r3]
	movs r3, #7
	ands r2, r3
	cmp r2, #0
	bne .L_0200958a
	ldr r3, [r7, #44]
	cmp r3, #0
	bne .L_0200952c
	ldr r3, [r7, #36]
	cmp r3, #0
	beq .L_0200958a
.L_0200952c:
	add r3, sp, #28
	mov r8, r3
	ldr r3, .L_0200959c
	mov r4, r8
	str r3, [r4, #36]
	movs r3, #204
	lsls r3, r3, #8
	adds r3, #204
	add r6, sp, #16
	str r3, [r4, #8]
	str r3, [r4, #12]
	str r2, [r6, #4]
	str r1, [r6, #8]
	str r2, [r6]
	bl Random16Far
	ldr r3, [r6]
	lsls r0, r0, #17
	lsrs r0, r0, #16
	adds r3, r3, r0
	ldr r0, .L_020095a0
	adds r3, r3, r0
	str r3, [r6]
	bl Random16Far
	ldr r5, [r6, #8]
	ldr r2, .L_020095a4
	lsls r0, r0, #16
	lsrs r0, r0, #16
	ldr r4, [r6, #4]
	adds r5, r5, r0
	adds r5, r5, r2
	str r5, [r6, #8]
	ldr r0, [r7, #8]
	ldr r1, [r7, #12]
	ldr r2, [r7, #16]
	ldr r3, [r6]
	str r4, [sp, #0]
	movs r4, #128
	lsls r4, r4, #17
	adds r4, #1
	str r4, [sp, #8]
	mov r4, r8
	str r5, [sp, #4]
	str r4, [sp, #12]
	bl Func_0200129c
.L_0200958a:
	add sp, #68
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009594:
	.4byte gPartyState
.L_02009598:
	.4byte gFrameCount
.L_0200959c:
	.4byte Func_02001464
.L_020095a0:
	.4byte 0xffff0000
.L_020095a4:
	.4byte 0xffff8000
	.section .text.x02009c6c,"ax",%progbits
	.balign 4
	.global Func_02001c6c
	.thumb_func
Func_02001c6c:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r8
	push {r5, r6}
	movs r0, #0
	movs r1, #240
	sub sp, #4
	bl Func_02004188
	movs r5, #64
	movs r1, #16
	movs r2, #0
	movs r3, #0
	movs r0, #0
	str r5, [sp, #0]
	bl Func_02004224
	movs r3, #240
	mov r10, r3
	mov r3, r10
	strh r3, [r0, #6]
	movs r3, #120
	mov r8, r3
	movs r6, #150
	mov r3, r8
	strh r3, [r0, #8]
	strh r6, [r0, #10]
	movs r1, #17
	movs r2, #0
	movs r3, #0
	movs r0, #1
	str r5, [sp, #0]
	bl Func_02004224
	mov r3, r10
	strh r3, [r0, #6]
	mov r3, r8
	strh r3, [r0, #8]
	strh r6, [r0, #10]
	movs r1, #18
	movs r2, #0
	movs r3, #0
	movs r0, #2
	str r5, [sp, #0]
	bl Func_02004224
	mov r3, r10
	strh r3, [r0, #6]
	mov r3, r8
	strh r3, [r0, #8]
	strh r6, [r0, #10]
	movs r5, #32
	movs r1, #19
	movs r2, #0
	movs r3, #0
	movs r0, #3
	str r5, [sp, #0]
	bl Func_02004224
	mov r3, r10
	strh r3, [r0, #6]
	mov r3, r8
	strh r3, [r0, #8]
	strh r6, [r0, #10]
	movs r3, #0
	movs r0, #4
	movs r1, #20
	movs r2, #0
	str r5, [sp, #0]
	bl Func_02004224
	mov r3, r10
	strh r3, [r0, #6]
	mov r3, r8
	strh r3, [r0, #8]
	strh r6, [r0, #10]
	movs r0, #153
	lsls r0, r0, #2
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02009d1e
	movs r0, #16
	movs r1, #0
	movs r2, #0
	bl Func_020058e0
	b .L_02009d24
.L_02009d1e:
	movs r0, #16
	bl Func_0200078c
.L_02009d24:
	movs r0, #217
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02009d3c
	movs r0, #17
	movs r1, #0
	movs r2, #0
	bl Func_020058e0
	b .L_02009d42
.L_02009d3c:
	movs r0, #17
	bl Func_0200078c
.L_02009d42:
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #101
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02009d5c
	movs r0, #18
	movs r1, #0
	movs r2, #0
	bl Func_020058e0
	b .L_02009d62
.L_02009d5c:
	movs r0, #18
	bl Func_0200078c
.L_02009d62:
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #32]
	movs r3, #13
	ldrb r2, [r1, #23]
	negs r3, r3
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	strb r3, [r1, #23]
	movs r0, #15
	bl Func_020006b4
	movs r0, #15
	bl Func_0200062c
	add sp, #4
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, pc}
	.section .text.x02009dd4,"ax",%progbits
	.balign 4
	.global Func_02001dd4
	.thumb_func
Func_02001dd4:
	push {r5, r6, r7, lr}
	movs r0, #8
	bl Object_GetById
	ldr r3, .L_02009ea4
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	adds r6, r0, #0
	ldr r0, [r3]
	bl Object_GetById
	adds r7, r0, #0
	movs r0, #162
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02009ea2
	ldr r0, [r7, #8]
	ldr r3, [r6, #8]
	ldr r2, [r7, #16]
	subs r3, r0, r3
	asrs r5, r3, #16
	ldr r3, [r6, #16]
	adds r1, r5, #0
	muls r1, r5
	subs r3, r2, r3
	asrs r4, r3, #16
	adds r3, r4, #0
	muls r3, r4
	adds r1, r1, r3
	ldr r3, .L_02009ea8
	asrs r2, r2, #16
	adds r4, r2, #0
	asrs r0, r0, #16
	adds r5, r0, r3
	subs r4, #248
	adds r2, r5, #0
	muls r2, r5
	adds r3, r4, #0
	muls r3, r4
	adds r7, r2, r3
	movs r2, #200
	lsls r2, r2, #5
	cmp r7, r2
	ble .L_02009e3a
	movs r3, #144
	lsls r3, r3, #4
	cmp r1, r3
	bgt .L_02009e50
.L_02009e3a:
	movs r1, #148
	lsls r1, r1, #1
	movs r3, #248
	subs r1, r1, r5
	subs r3, r3, r4
	lsls r1, r1, #16
	ldr r2, [r6, #12]
	lsls r3, r3, #16
	adds r0, r6, #0
	bl Func_020057f8
.L_02009e50:
	cmp r7, #15
	bgt .L_02009ea2
	movs r0, #146
	bl Func_02005a68
	movs r0, #122
	bl Func_02005a68
	movs r3, #128
	lsls r3, r3, #10
	adds r1, r6, #0
	str r3, [r6, #48]
	str r3, [r6, #52]
	adds r1, #85
	movs r2, #0
	movs r3, #2
	strb r3, [r1]
	str r2, [r6, #20]
	adds r2, r6, #0
	adds r2, #89
	movs r3, #1
	strb r3, [r2]
	movs r3, #160
	lsls r3, r3, #12
	str r3, [r6, #40]
	ldr r1, .L_02009eac
	movs r0, #8
	bl ObjectMotion_EnableActionAndSetCallback
	movs r1, #172
	movs r3, #132
	adds r0, r6, #0
	ldr r2, [r6, #12]
	lsls r1, r1, #17
	lsls r3, r3, #17
	bl Func_020057f8
	movs r0, #162
	lsls r0, r0, #2
	bl GameFlag_SetBit
.L_02009ea2:
	pop {r5, r6, r7, pc}
.L_02009ea4:
	.4byte gPartyState
.L_02009ea8:
	.4byte 0xfffffed8
.L_02009eac:
	.4byte Data_02005b44
	.section .text.x02009f08,"ax",%progbits
	.balign 4
	.global Func_02001f08
	.thumb_func
Func_02001f08:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r3, .L_02009fb8
	sub sp, #32
	ldr r0, [r3]
	cmp r0, #0
	bge .L_02009f1a
	adds r0, #3
.L_02009f1a:
	asrs r0, r0, #2
	movs r1, #5
	bl __modsi3
	ldr r3, .L_02009fbc
	mov r8, r0
	ldr r3, [r3]
	cmp r3, #0
	beq .L_02009f6e
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
	beq .L_02009f4e
	movs r3, #192
	lsls r3, r3, #2
	adds r3, #255
	ands r3, r1
	cmp r3, #153
	bne .L_02009fae
.L_02009f4e:
	movs r1, #192
	lsls r1, r1, #4
	adds r1, #164
	adds r3, r2, r1
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	bne .L_02009fae
	movs r0, #175
	lsls r0, r0, #1
	adds r3, r2, r0
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	bne .L_02009fae
.L_02009f6e:
	movs r5, #0
	movs r6, #4
.L_02009f72:
	mov r2, r8
	adds r0, r2, r5
	movs r1, #5
	mov r7, sp
	bl __modsi3
	ldr r3, .L_02009fc0
	lsls r0, r0, #1
	ldrh r3, [r3, r6]
	adds r5, #1
	strh r3, [r7, r0]
	adds r6, #2
	cmp r5, #4
	ble .L_02009f72
	movs r3, #128
	movs r1, #160
	movs r2, #132
	lsls r3, r3, #19
	lsls r1, r1, #19
	lsls r2, r2, #24
	adds r3, #212
	adds r0, r7, #0
	adds r1, #164
	adds r2, #2
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r2, .L_02009fb8
	ldr r3, [r2]
	adds r3, #1
	str r3, [r2]
.L_02009fae:
	add sp, #32
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009fb8:
	.4byte Data_02007e20
.L_02009fbc:
	.4byte Data_02007e24
.L_02009fc0:
	.4byte Data_02007e28
	.section .text.x0200a00c,"ax",%progbits
	.balign 4
	.global Func_0200200c
	.thumb_func
Func_0200200c:
	push {lr}
	ldr r3, .L_0200a0c0
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #4
	bne .L_0200a02e
	bl Func_02005868
	bl Func_020034b4
	movs r0, #4
	bl Func_02005990
	b .L_0200a07c
.L_0200a02e:
	cmp r3, #5
	bne .L_0200a042
	bl Func_02005868
	bl Func_020037c8
	movs r0, #5
	bl Func_02005990
	b .L_0200a07c
.L_0200a042:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a07c
	movs r0, #194
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a064
	movs r0, #0
	bl Func_02001754
.L_0200a064:
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #133
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a078
	bl Func_02002db0
	b .L_0200a07c
.L_0200a078:
	bl Func_02002db4
.L_0200a07c:
	ldr r3, .L_0200a0c0
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #64
	orrs r3, r2
	strb r3, [r0]
	movs r0, #1
	bl Func_02001fc4
	movs r0, #10
	bl Func_020006b4
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #252
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a0bc
	movs r1, #136
	movs r2, #164
	movs r0, #10
	lsls r1, r1, #16
	lsls r2, r2, #17
	bl Func_020058e0
.L_0200a0bc:
	pop {pc}
	.2byte 0x0000
.L_0200a0c0:
	.4byte gPartyState
	.section .text.x0200a0c4,"ax",%progbits
	.balign 4
	.global Func_020020c4
	.thumb_func
Func_020020c4:
	push {r5, r6, r7, lr}
	ldr r5, .L_0200a160
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r5, r1
	adds r1, #2
	movs r2, #0
	ldrsh r7, [r3, r2]
	adds r3, r5, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_0200a164
	movs r0, #0
	movs r1, #16
	ldrsh r6, [r3, r1]
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r1, #172
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	bne .L_0200a15e
	ldr r3, .L_0200a168
	cmp r7, r3
	bne .L_0200a136
	cmp r2, #7
	bgt .L_0200a12e
	movs r0, #162
	lsls r0, r0, #1
	bl GameFlag_SetBit
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r5, r2
	ldr r0, [r3]
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #64
	orrs r3, r2
	strb r3, [r0]
	ldr r2, .L_0200a16c
	movs r3, #7
	ands r6, r3
	lsls r3, r6, #2
	ldr r0, [r2, r3]
	mov lr, r0
	.2byte 0xf800
	b .L_0200a136
.L_0200a12e:
	cmp r2, #55
	bne .L_0200a136
	bl Func_02003900
.L_0200a136:
	ldr r3, .L_0200a170
	cmp r7, r3
	bne .L_0200a140
	bl Func_02001dd0
.L_0200a140:
	ldr r3, .L_0200a174
	cmp r7, r3
	bne .L_0200a14a
	bl Func_02001eb0
.L_0200a14a:
	ldr r3, .L_0200a178
	cmp r7, r3
	bne .L_0200a15c
	movs r0, #162
	lsls r0, r0, #1
	bl GameFlag_SetBit
	bl Func_0200200c
.L_0200a15c:
	movs r0, #0
.L_0200a15e:
	pop {r5, r6, r7, pc}
.L_0200a160:
	.4byte gPartyState
.L_0200a164:
	.4byte Data_020023c4 + 0x88
.L_0200a168:
	.4byte 0x000000f7
.L_0200a16c:
	.4byte Data_02005b64
.L_0200a170:
	.4byte 0x000000f9
.L_0200a174:
	.4byte 0x000000fa
.L_0200a178:
	.4byte 0x000000f8
	.section .text.x0200a17c,"ax",%progbits
	.balign 4
	.global Func_0200217c
	.thumb_func
Func_0200217c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	mov r10, r2
	ldr r2, [sp, #32]
	ldr r4, [sp, #28]
	lsls r2, r2, #7
	adds r2, r2, r4
	ldr r4, .L_0200a22c
	ldr r5, .L_0200a230
	lsls r1, r1, #7
	adds r1, r1, r0
	subs r3, #1
	adds r6, r2, r4
	adds r0, r1, r4
	lsls r2, r2, #2
	lsls r1, r1, #2
	lsls r3, r3, #16
	adds r4, r2, r5
	adds r5, r1, r5
	asrs r1, r3, #16
	cmp r1, #0
	blt .L_0200a220
	ldr r2, .L_0200a234
	lsls r3, r1, #16
	adds r2, r2, r3
	lsls r3, r1, #7
	mov r1, r10
	mov r8, r2
	adds r2, r0, r1
	adds r2, r2, r3
	mov lr, r2
	adds r2, r6, r1
	adds r2, r2, r3
	add r3, r10
	lsls r3, r3, #2
	mov r12, r2
	adds r7, r3, r5
	adds r6, r3, r4
.L_0200a1ce:
	mov r3, r10
	subs r3, #1
	lsls r3, r3, #16
	mov r4, r12
	mov r0, lr
	asrs r3, r3, #16
	subs r5, r6, #4
	subs r1, r7, #4
	subs r4, #1
	subs r0, #1
	cmp r3, #0
	blt .L_0200a208
	lsls r2, r3, #16
	ldr r3, .L_0200a234
	adds r2, r2, r3
.L_0200a1ec:
	ldr r3, [r1]
	mov r9, r2
	str r3, [r5]
	subs r1, #4
	ldrb r3, [r0]
	subs r5, #4
	strb r3, [r4]
	ldr r3, .L_0200a234
	subs r4, #1
	adds r2, r2, r3
	mov r3, r9
	subs r0, #1
	cmp r3, #0
	bge .L_0200a1ec
.L_0200a208:
	movs r3, #128
	negs r3, r3
	ldr r1, .L_0200a234
	add lr, r3
	add r12, r3
	ldr r3, .L_0200a238
	mov r2, r8
	adds r7, r7, r3
	add r8, r1
	adds r6, r6, r3
	cmp r2, #0
	bge .L_0200a1ce
.L_0200a220:
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200a22c:
	.4byte gMapShapeGrid
.L_0200a230:
	.4byte gMapCellBuffer
.L_0200a234:
	.4byte 0xffff0000
.L_0200a238:
	.4byte 0xfffffe00
	.section .text.x0200a23c,"ax",%progbits
	.balign 4
	.global Func_0200223c
	.thumb_func
Func_0200223c:
	push {r5, r6, r7, lr}
	ldr r7, .L_0200a2d8
	movs r0, #24
	ldrsh r3, [r7, r0]
	cmp r3, #3
	ble .L_0200a24a
	b .L_0200a42a
.L_0200a24a:
	bl Func_020056e8
	cmp r0, #0
	beq .L_0200a2ee
	movs r1, #20
	ldrsh r3, [r7, r1]
	cmp r3, #0
	bne .L_0200a280
	movs r0, #1
	bl Func_020000fc
	movs r3, #1
	adds r6, r0, #0
	eors r6, r3
	ldr r2, .L_0200a2dc
	lsls r5, r6, #2
	adds r3, r5, #0
	adds r3, #32
	ldr r0, [r2, r3]
	adds r5, r5, r6
	bl Func_020056c4
	lsls r3, r5, #4
	subs r3, r3, r5
	lsls r3, r3, #3
	adds r3, #1
	b .L_0200a2ec
.L_0200a280:
	movs r3, #24
	ldrsh r2, [r7, r3]
	movs r0, #28
	ldrsh r3, [r7, r0]
	ldrh r1, [r7, #24]
	cmp r2, r3
	blt .L_0200a292
	adds r3, r1, #1
	strh r3, [r7, #28]
.L_0200a292:
	ldrh r3, [r7, #24]
	movs r5, #0
	adds r3, #1
	strh r3, [r7, #24]
	lsls r3, r3, #16
	asrs r1, r3, #16
	cmp r1, #3
	bgt .L_0200a2e0
	ldr r2, .L_0200a2d4
	ldrh r3, [r7, #18]
	lsls r1, r1, #1
	eors r3, r2
	lsls r3, r3, #16
	asrs r3, r3, #16
	adds r1, r1, r3
	lsls r1, r1, #1
	ldrsh r3, [r7, r1]
	ldr r1, .L_0200a2dc
	movs r2, #7
	ands r3, r2
	lsls r3, r3, #2
	ldr r0, [r1, r3]
	bl Func_020056c4
	movs r0, #8
	bl Object_GetById
	str r5, [r0, #20]
	movs r0, #8
	bl Object_GetById
	str r5, [r0, #12]
	b .L_0200a2ea
.L_0200a2d4:
	.4byte 0x00000001
.L_0200a2d8:
	.4byte Data_020023c4 + 0x88
.L_0200a2dc:
	.4byte Data_02005b84
.L_0200a2e0:
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl Func_020058e0
.L_0200a2ea:
	movs r3, #0
.L_0200a2ec:
	strh r3, [r7, #20]
.L_0200a2ee:
	movs r0, #8
	bl Object_GetById
	movs r3, #22
	ldrsh r2, [r7, r3]
	movs r1, #24
	ldrsh r3, [r7, r1]
	adds r0, #84
	cmp r2, r3
	bne .L_0200a306
	movs r3, #1
	b .L_0200a308
.L_0200a306:
	movs r3, #0
.L_0200a308:
	strb r3, [r0]
	movs r2, #20
	ldrsh r3, [r7, r2]
	cmp r3, #0
	bne .L_0200a314
	b .L_0200a42a
.L_0200a314:
	movs r0, #130
	lsls r0, r0, #1
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a322
	b .L_0200a42a
.L_0200a322:
	movs r3, #22
	ldrsh r2, [r7, r3]
	movs r0, #24
	ldrsh r3, [r7, r0]
	ldrh r1, [r7, #24]
	cmp r2, r3
	bne .L_0200a424
	movs r2, #20
	ldrsh r3, [r7, r2]
	movs r2, #199
	lsls r2, r2, #1
	adds r2, #255
	cmp r3, r2
	beq .L_0200a3e8
	cmp r3, r2
	bgt .L_0200a360
	cmp r3, #50
	beq .L_0200a3f6
	cmp r3, #50
	bgt .L_0200a350
	cmp r3, #1
	beq .L_0200a38e
	b .L_0200a424
.L_0200a350:
	cmp r3, #70
	beq .L_0200a412
	movs r0, #173
	lsls r0, r0, #1
	adds r0, #255
	cmp r3, r0
	beq .L_0200a38e
	b .L_0200a424
.L_0200a360:
	movs r2, #177
	lsls r2, r2, #2
	cmp r3, r2
	beq .L_0200a3e8
	cmp r3, r2
	bgt .L_0200a378
	movs r1, #200
	lsls r1, r1, #1
	adds r1, #255
	cmp r3, r1
	beq .L_0200a3d2
	b .L_0200a424
.L_0200a378:
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #198
	cmp r3, r2
	beq .L_0200a3f6
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #218
	cmp r3, r0
	beq .L_0200a412
	b .L_0200a424
.L_0200a38e:
	movs r0, #124
	bl Func_02005a68
	movs r1, #18
	ldrsh r3, [r7, r1]
	cmp r3, #0
	beq .L_0200a3b4
	movs r0, #9
	movs r1, #6
	bl Object_SetModeById
	movs r2, #20
	ldrsh r3, [r7, r2]
	movs r0, #150
	lsls r0, r0, #2
	cmp r3, r0
	ble .L_0200a424
	movs r0, #10
	b .L_0200a3ca
.L_0200a3b4:
	movs r1, #6
	movs r0, #11
	bl Object_SetModeById
	movs r1, #20
	ldrsh r3, [r7, r1]
	movs r2, #150
	lsls r2, r2, #2
	cmp r3, r2
	ble .L_0200a424
	movs r0, #12
.L_0200a3ca:
	movs r1, #6
	bl Object_SetModeById
	b .L_0200a424
.L_0200a3d2:
	movs r0, #125
	bl Func_02005a68
	movs r0, #18
	ldrsh r3, [r7, r0]
	cmp r3, #0
	beq .L_0200a3e4
	movs r0, #10
	b .L_0200a40a
.L_0200a3e4:
	movs r0, #12
	b .L_0200a40a
.L_0200a3e8:
	movs r0, #8
	bl Object_GetById
	movs r3, #192
	lsls r3, r3, #8
	strh r3, [r0, #6]
	b .L_0200a424
.L_0200a3f6:
	movs r0, #125
	bl Func_02005a68
	movs r1, #18
	ldrsh r3, [r7, r1]
	cmp r3, #0
	beq .L_0200a408
	movs r0, #9
	b .L_0200a40a
.L_0200a408:
	movs r0, #11
.L_0200a40a:
	movs r1, #5
	bl Object_SetModeById
	b .L_0200a424
.L_0200a412:
	lsls r1, r1, #16
	movs r2, #18
	ldrsh r0, [r7, r2]
	asrs r1, r1, #16
	bl Func_020002a4
	movs r0, #138
	bl Func_02005a68
.L_0200a424:
	ldrh r3, [r7, #20]
	adds r3, #1
	strh r3, [r7, #20]
.L_0200a42a:
	pop {r5, r6, r7, pc}
	.section .text.x0200a42c,"ax",%progbits
	.balign 4
	.global Func_0200242c
	.thumb_func
Func_0200242c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r5, .L_0200a4f4
	movs r0, #240
	lsls r0, r0, #1
	adds r3, r5, r0
	adds r0, #2
	movs r1, #0
	ldrsh r2, [r3, r1]
	adds r3, r5, r0
	movs r0, #0
	ldrsh r1, [r3, r0]
	ldr r3, .L_0200a4f8
	sub sp, #16
	mov r10, r1
	cmp r2, r3
	beq .L_0200a45a
	b .L_0200abac
.L_0200a45a:
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	ldr r1, .L_0200a4fc
	mov r8, r3
	mov r3, r10
	subs r3, #31
	mov r11, r1
	cmp r3, #1
	bhi .L_0200a51a
	movs r0, #224
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a4b2
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r5, r2
	ldr r0, [r3]
	bl UnitGet
	movs r1, #32
	adds r5, r0, #0
	ldr r3, .L_0200a500
	mov r0, r11
	mov lr, r3
	.2byte 0xf800
	adds r2, r5, #0
	adds r1, r2, #0
	movs r6, #0
	adds r1, #13
.L_0200a49a:
	ldrb r3, [r2]
	adds r2, #1
	adds r6, r6, r3
	cmp r2, r1
	ble .L_0200a49a
	adds r0, r6, #0
	bl Func_02002c30
	movs r0, #224
	lsls r0, r0, #2
	bl GameFlag_SetBit
.L_0200a4b2:
	ldr r1, .L_0200a4f0
	mov r3, r10
	subs r3, #1
	mov r0, r11
	ands r3, r1
	movs r2, #0
	strh r3, [r0, #18]
	strh r2, [r0, #22]
	strh r2, [r0, #24]
	strh r2, [r0, #26]
	strh r2, [r0, #28]
	eors r3, r1
	lsls r3, r3, #1
	ldrsh r6, [r0, r3]
	ldr r2, .L_0200a504
	movs r3, #7
	ands r6, r3
	lsls r3, r6, #2
	ldr r0, [r2, r3]
	bl Func_020056c4
	mov r0, r11
	movs r2, #22
	ldrsh r3, [r0, r2]
	movs r1, #18
	ldrsh r2, [r0, r1]
	lsls r3, r3, #1
	adds r3, r3, r2
	lsls r3, r3, #1
	b .L_0200a508
	.2byte 0x0000
.L_0200a4f0:
	.4byte 0x00000001
.L_0200a4f4:
	.4byte gPartyState
.L_0200a4f8:
	.4byte 0x000000f7
.L_0200a4fc:
	.4byte Data_020023c4 + 0x88
.L_0200a500:
	.4byte IwramClearWords
.L_0200a504:
	.4byte Data_02005b84
.L_0200a508:
	ldrh r3, [r0, r3]
	mov r2, r11
	movs r1, #10
	strh r3, [r2, #16]
	mov r0, r10
	bl __modsi3
	adds r1, r0, #0
	b .L_0200a616
.L_0200a51a:
	mov r3, r10
	subs r3, #33
	cmp r3, #1
	bhi .L_0200a5c8
	movs r0, #224
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a566
	movs r0, #133
	lsls r0, r0, #2
	adds r3, r5, r0
	ldr r0, [r3]
	bl UnitGet
	movs r1, #32
	adds r5, r0, #0
	ldr r3, .L_0200a5a8
	mov r0, r11
	mov lr, r3
	.2byte 0xf800
	adds r2, r5, #0
	adds r1, r2, #0
	movs r6, #0
	adds r1, #13
.L_0200a54e:
	ldrb r3, [r2]
	adds r2, #1
	adds r6, r6, r3
	cmp r2, r1
	ble .L_0200a54e
	adds r0, r6, #0
	bl Func_02002c30
	movs r0, #224
	lsls r0, r0, #2
	bl GameFlag_SetBit
.L_0200a566:
	ldr r1, .L_0200a5a4
	mov r3, r10
	subs r3, #1
	ands r3, r1
	mov r2, r11
	strh r3, [r2, #18]
	mov r0, r11
	movs r2, #3
	strh r2, [r0, #22]
	strh r2, [r0, #24]
	movs r2, #4
	eors r3, r1
	strh r2, [r0, #26]
	strh r2, [r0, #28]
	adds r3, #6
	lsls r3, r3, #1
	ldrsh r6, [r0, r3]
	ldr r2, .L_0200a5ac
	movs r3, #7
	ands r6, r3
	lsls r3, r6, #2
	ldr r0, [r2, r3]
	bl Func_020056c4
	mov r0, r11
	movs r2, #22
	ldrsh r3, [r0, r2]
	movs r1, #18
	ldrsh r2, [r0, r1]
	b .L_0200a5b0
	.2byte 0x0000
.L_0200a5a4:
	.4byte 0x00000001
.L_0200a5a8:
	.4byte IwramClearWords
.L_0200a5ac:
	.4byte Data_02005b84
.L_0200a5b0:
	lsls r3, r3, #1
	adds r3, r3, r2
	lsls r3, r3, #1
	ldrh r3, [r0, r3]
	mov r2, r11
	movs r1, #10
	strh r3, [r2, #16]
	mov r0, r10
	bl __modsi3
	adds r1, r0, #0
	b .L_0200a616
.L_0200a5c8:
	mov r3, r10
	cmp r3, #50
	bne .L_0200a61e
	movs r0, #133
	lsls r0, r0, #2
	adds r3, r5, r0
	ldr r0, [r3]
	bl UnitGet
	movs r1, #32
	adds r5, r0, #0
	ldr r3, .L_0200a90c
	mov r0, r11
	mov lr, r3
	.2byte 0xf800
	adds r2, r5, #0
	adds r1, r2, #0
	movs r6, #0
	adds r1, #13
.L_0200a5ee:
	ldrb r3, [r2]
	adds r2, #1
	adds r6, r6, r3
	cmp r2, r1
	ble .L_0200a5ee
	adds r0, r6, #0
	bl Func_02002c30
	movs r3, #0
	mov r1, r11
	mov r2, r11
	mov r0, r11
	strh r3, [r1, #18]
	strh r3, [r2, #22]
	strh r3, [r0, #24]
	strh r3, [r1, #26]
	strh r3, [r2, #28]
	ldrh r3, [r2]
	movs r1, #55
	strh r3, [r0, #16]
.L_0200a616:
	ldr r0, .L_0200a910
	bl Func_02005988
	b .L_0200ac0e
.L_0200a61e:
	mov r1, r8
	mov r2, r8
	adds r1, #228
	adds r2, #232
	mov r3, r10
	str r1, [sp, #12]
	str r2, [sp, #8]
	cmp r3, #55
	beq .L_0200a632
	b .L_0200a7ce
.L_0200a632:
	movs r7, #160
	lsls r7, r7, #1
	movs r3, #136
	add r7, r8
	lsls r3, r3, #18
	str r3, [r7, #8]
	ldr r3, .L_0200a914
	ldr r5, .L_0200a918
	adds r3, #136
	str r3, [r7, #48]
	adds r3, r5, #0
	movs r0, #0
	adds r3, #34
	str r0, [r7, #12]
	str r3, [r7, #52]
	ldr r2, [sp, #12]
	movs r1, #34
	mov r9, r0
	mov r10, r1
	ldr r0, [r2]
	ldr r1, [r7, #16]
	ldr r3, .L_0200a91c
	mov lr, r3
	.2byte 0xf800
	ldr r3, [r7, #8]
	ldr r2, .L_0200a91c
	adds r0, r0, r3
	str r0, [r7]
	ldr r1, [sp, #8]
	adds r5, #68
	ldr r0, [r1]
	ldr r1, [r7, #20]
	mov lr, r2
	.2byte 0xf800
	ldr r3, [r7, #12]
	ldr r1, .L_0200a914
	adds r0, r0, r3
	str r0, [r7, #4]
	movs r7, #188
	lsls r7, r7, #1
	movs r3, #136
	add r7, r8
	lsls r3, r3, #19
	movs r2, #136
	str r3, [r7, #8]
	lsls r2, r2, #1
	mov r3, r9
	str r3, [r7, #12]
	adds r3, r1, r2
	str r5, [r7, #52]
	str r3, [r7, #48]
	ldr r3, [sp, #12]
	movs r0, #68
	mov r8, r0
	ldr r1, [r7, #16]
	ldr r0, [r3]
	ldr r2, .L_0200a91c
	mov lr, r2
	.2byte 0xf800
	ldr r3, [r7, #8]
	ldr r2, .L_0200a91c
	adds r0, r0, r3
	str r0, [r7]
	ldr r3, [sp, #8]
	ldr r1, [r7, #20]
	ldr r0, [r3]
	mov lr, r2
	.2byte 0xf800
	ldr r3, [r7, #12]
	movs r5, #13
	adds r0, r0, r3
	str r0, [r7, #4]
	movs r0, #9
	bl Object_GetById
	movs r1, #3
	bl Object_SetPartAttribute
	movs r0, #10
	bl Object_GetById
	movs r1, #3
	bl Object_SetPartAttribute
	movs r0, #11
	bl Object_GetById
	movs r1, #3
	bl Object_SetPartAttribute
	movs r0, #12
	bl Object_GetById
	movs r1, #3
	bl Object_SetPartAttribute
	mov r3, r9
	str r3, [sp, #0]
	movs r0, #0
	movs r1, #12
	movs r2, #1
	movs r3, #21
	str r5, [sp, #4]
	bl Func_02005828
	mov r0, r10
	movs r1, #12
	movs r2, #1
	movs r3, #21
	str r0, [sp, #0]
	str r5, [sp, #4]
	bl Func_02005828
	mov r1, r8
	str r1, [sp, #0]
	movs r0, #68
	movs r1, #12
	movs r2, #1
	movs r3, #21
	str r5, [sp, #4]
	bl Func_02005828
	movs r2, #1
	str r2, [sp, #0]
	movs r0, #0
	movs r1, #13
	movs r2, #33
	movs r3, #21
	str r5, [sp, #4]
	bl Func_02005828
	movs r3, #35
	str r3, [sp, #0]
	mov r8, r3
	movs r0, #34
	movs r1, #13
	movs r2, #33
	movs r3, #21
	str r5, [sp, #4]
	bl Func_02005828
	movs r0, #69
	str r0, [sp, #0]
	movs r1, #13
	mov r10, r0
	movs r2, #33
	movs r0, #68
	movs r3, #21
	str r5, [sp, #4]
	bl Func_02005828
	mov r2, r11
	movs r1, #16
	ldrsh r6, [r2, r1]
	ldr r2, .L_0200a920
	lsls r3, r6, #2
	ldrsh r7, [r2, r3]
	adds r3, #2
	ldrsh r1, [r2, r3]
	adds r0, r7, #0
	mov r9, r1
	movs r1, #1
	str r1, [sp, #0]
	movs r2, #13
	mov r1, r9
	movs r3, #21
	str r5, [sp, #4]
	bl Func_02005828
	mov r2, r8
	adds r0, r7, #0
	str r2, [sp, #0]
	adds r0, #42
	mov r1, r9
	movs r2, #13
	movs r3, #21
	str r5, [sp, #4]
	bl Func_02005828
	mov r3, r10
	adds r0, r7, #0
	str r3, [sp, #0]
	adds r0, #84
	mov r1, r9
	movs r2, #13
	movs r3, #21
	str r5, [sp, #4]
	bl Func_02005828
	movs r5, #0
.L_0200a7ae:
	ldr r3, .L_0200a924
	ldrsb r3, [r3, r5]
	cmp r3, r6
	beq .L_0200a7c2
	adds r0, r5, #0
	adds r0, #64
	movs r1, #0
	movs r2, #0
	bl Func_020058e0
.L_0200a7c2:
	adds r5, #1
	cmp r5, #9
	bls .L_0200a7ae
	bl Func_020057e8
	b .L_0200ac0e
.L_0200a7ce:
	movs r7, #160
	lsls r7, r7, #1
	movs r3, #136
	add r7, r8
	lsls r3, r3, #18
	str r3, [r7, #8]
	ldr r3, .L_0200a914
	ldr r5, .L_0200a918
	adds r3, #136
	str r3, [r7, #48]
	adds r3, r5, #0
	movs r0, #0
	adds r3, #34
	str r0, [r7, #12]
	str r3, [r7, #52]
	ldr r1, [sp, #12]
	mov r9, r0
	ldr r2, .L_0200a91c
	ldr r0, [r1]
	ldr r1, [r7, #16]
	mov lr, r2
	.2byte 0xf800
	ldr r3, [r7, #8]
	ldr r1, [r7, #20]
	adds r0, r0, r3
	str r0, [r7]
	ldr r3, [sp, #8]
	ldr r2, .L_0200a91c
	ldr r0, [r3]
	mov lr, r2
	.2byte 0xf800
	ldr r3, [r7, #12]
	ldr r1, .L_0200a914
	adds r0, r0, r3
	str r0, [r7, #4]
	movs r7, #188
	lsls r7, r7, #1
	movs r3, #136
	add r7, r8
	lsls r3, r3, #19
	movs r2, #136
	str r3, [r7, #8]
	lsls r2, r2, #1
	mov r3, r9
	str r3, [r7, #12]
	adds r5, #68
	adds r3, r1, r2
	str r3, [r7, #48]
	str r5, [r7, #52]
	ldr r3, [sp, #12]
	movs r0, #68
	ldr r1, [r7, #16]
	ldr r2, .L_0200a91c
	mov r8, r0
	ldr r0, [r3]
	mov lr, r2
	.2byte 0xf800
	ldr r3, [r7, #8]
	ldr r1, [r7, #20]
	adds r0, r0, r3
	str r0, [r7]
	ldr r3, [sp, #8]
	ldr r2, .L_0200a91c
	ldr r0, [r3]
	mov lr, r2
	.2byte 0xf800
	ldr r3, [r7, #12]
	movs r6, #1
	adds r0, r0, r3
	str r0, [r7, #4]
	adds r3, r6, #0
	mov r0, r10
	ands r3, r0
	cmp r3, #0
	beq .L_0200a928
	mov r1, r9
	movs r5, #9
	movs r0, #0
	movs r2, #102
	movs r3, #13
	str r1, [sp, #4]
	str r5, [sp, #0]
	bl Func_0200217c
	mov r2, r9
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #0
	movs r2, #10
	movs r3, #1
	str r6, [sp, #0]
	bl Func_02005828
	mov r3, r9
	str r3, [sp, #0]
	movs r0, #0
	movs r1, #0
	movs r2, #10
	movs r3, #12
	str r6, [sp, #4]
	bl Func_02005828
	movs r0, #34
	movs r1, #0
	movs r2, #10
	movs r3, #12
	str r0, [sp, #0]
	str r6, [sp, #4]
	bl Func_02005828
	mov r1, r8
	str r1, [sp, #0]
	movs r0, #68
	movs r1, #0
	movs r2, #10
	movs r3, #12
	str r6, [sp, #4]
	bl Func_02005828
	movs r2, #34
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #34
	movs r2, #102
	movs r3, #10
	str r5, [sp, #0]
	bl Func_0200217c
	mov r3, r9
	movs r0, #34
	str r3, [sp, #0]
	str r0, [sp, #4]
	movs r1, #33
	movs r0, #0
	movs r2, #10
	movs r3, #10
	bl Func_02005828
	movs r1, #34
	str r1, [sp, #0]
	str r1, [sp, #4]
	movs r0, #34
	movs r1, #33
	movs r2, #10
	movs r3, #10
	bl Func_02005828
	mov r2, r8
	movs r3, #34
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r0, #68
	movs r1, #33
	movs r2, #10
	movs r3, #10
	bl Func_02005828
	b .L_0200aa0a
	.2byte 0x0000
.L_0200a90c:
	.4byte IwramClearWords
.L_0200a910:
	.4byte 0x000000f7
.L_0200a914:
	.4byte gMapCellBuffer
.L_0200a918:
	.4byte gMapShapeGrid
.L_0200a91c:
	.4byte IwramMulQ16
.L_0200a920:
	.4byte Data_02005bac
.L_0200a924:
	.4byte Data_02005bcc
.L_0200a928:
	mov r0, r9
	str r0, [sp, #0]
	str r0, [sp, #4]
	movs r1, #0
	movs r0, #10
	movs r2, #92
	movs r3, #13
	bl Func_02005828
	mov r1, r9
	str r1, [sp, #4]
	movs r5, #24
	movs r0, #23
	movs r1, #0
	movs r2, #10
	movs r3, #1
	str r5, [sp, #0]
	bl Func_02005828
	movs r3, #23
	str r3, [sp, #0]
	movs r0, #23
	movs r1, #0
	movs r2, #10
	movs r3, #12
	str r6, [sp, #4]
	bl Func_02005828
	movs r3, #57
	str r3, [sp, #0]
	movs r0, #57
	movs r1, #0
	movs r2, #10
	movs r3, #12
	str r6, [sp, #4]
	bl Func_02005828
	movs r3, #91
	str r3, [sp, #0]
	movs r0, #91
	movs r1, #0
	movs r2, #10
	movs r3, #12
	str r6, [sp, #4]
	bl Func_02005828
	mov r2, r9
	movs r3, #34
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r0, #10
	movs r1, #34
	movs r2, #92
	movs r3, #10
	bl Func_02005828
	movs r0, #34
	str r0, [sp, #4]
	movs r1, #33
	movs r0, #24
	movs r2, #10
	movs r3, #10
	str r5, [sp, #0]
	bl Func_02005828
	movs r3, #58
	movs r1, #34
	str r3, [sp, #0]
	str r1, [sp, #4]
	movs r0, #58
	movs r1, #33
	movs r2, #10
	movs r3, #10
	bl Func_02005828
	movs r3, #92
	movs r2, #34
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #10
	movs r0, #92
	movs r1, #33
	movs r2, #10
	bl Func_02005828
	movs r1, #160
	movs r2, #152
	movs r0, #9
	lsls r1, r1, #14
	lsls r2, r2, #16
	bl Func_020058e0
	movs r1, #192
	movs r2, #152
	movs r0, #10
	lsls r1, r1, #13
	lsls r2, r2, #16
	bl Func_020058e0
	movs r1, #184
	movs r2, #152
	movs r0, #11
	lsls r1, r1, #16
	lsls r2, r2, #16
	bl Func_020058e0
	movs r1, #200
	movs r2, #152
	movs r0, #12
	lsls r1, r1, #16
	lsls r2, r2, #16
	bl Func_020058e0
.L_0200aa0a:
	movs r0, #9
	bl Object_GetById
	movs r1, #3
	bl Object_SetPartAttribute
	movs r0, #10
	bl Object_GetById
	movs r1, #3
	bl Object_SetPartAttribute
	movs r0, #11
	bl Object_GetById
	movs r1, #3
	bl Object_SetPartAttribute
	movs r0, #12
	bl Object_GetById
	movs r1, #3
	bl Object_SetPartAttribute
	mov r0, r11
	movs r3, #16
	ldrsh r6, [r0, r3]
	ldr r2, .L_0200ab68
	lsls r3, r6, #2
	ldrsh r7, [r2, r3]
	adds r3, #2
	ldrsh r0, [r2, r3]
	movs r3, #10
	mov r9, r0
	movs r5, #13
	str r3, [sp, #0]
	adds r0, r7, #0
	mov r1, r9
	movs r2, #13
	movs r3, #21
	str r5, [sp, #4]
	bl Func_02005828
	movs r3, #44
	adds r0, r7, #0
	str r3, [sp, #0]
	adds r0, #42
	mov r1, r9
	movs r2, #13
	movs r3, #21
	str r5, [sp, #4]
	bl Func_02005828
	movs r3, #78
	adds r0, r7, #0
	str r3, [sp, #0]
	adds r0, #84
	mov r1, r9
	movs r2, #13
	movs r3, #21
	str r5, [sp, #4]
	bl Func_02005828
	movs r5, #0
.L_0200aa8a:
	ldr r3, .L_0200ab6c
	ldrsb r3, [r3, r5]
	cmp r3, r6
	beq .L_0200aa9e
	adds r0, r5, #0
	adds r0, #64
	movs r1, #0
	movs r2, #0
	bl Func_020058e0
.L_0200aa9e:
	adds r5, #1
	cmp r5, #9
	bls .L_0200aa8a
	bl Func_020057e8
	ldr r3, .L_0200ab70
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	adds r5, r0, #0
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200aad2
	ldr r1, [r5, #8]
	ldr r2, [r5, #16]
	movs r0, #0
	bl Map_GetTerrainHeight
	str r0, [r5, #20]
	str r0, [r5, #12]
.L_0200aad2:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200ab7c
	movs r0, #162
	lsls r0, r0, #1
	bl GameFlag_SetBit
	ldr r5, .L_0200ab74
	movs r3, #22
	ldrsh r2, [r5, r3]
	movs r0, #26
	ldrsh r3, [r5, r0]
	cmp r2, r3
	blt .L_0200ab04
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #130
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200ab22
.L_0200ab04:
	movs r1, #18
	ldrsh r0, [r5, r1]
	movs r2, #22
	ldrsh r1, [r5, r2]
	bl Func_02000218
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #130
	bl GameFlag_SetBit
	movs r0, #161
	lsls r0, r0, #2
	bl GameFlag_SetBit
.L_0200ab22:
	movs r3, #22
	ldrsh r1, [r5, r3]
	movs r0, #28
	ldrsh r3, [r5, r0]
	cmp r1, r3
	bge .L_0200ab36
	movs r2, #18
	ldrsh r0, [r5, r2]
	bl Func_020002a4
.L_0200ab36:
	ldr r3, .L_0200ab70
	movs r0, #133
	lsls r0, r0, #2
	adds r3, r3, r0
	ldr r0, [r3]
	movs r1, #8
	bl Func_020055c4
	mov r1, r11
	ldrh r0, [r1, #18]
	ldr r3, .L_0200ab64
	eors r0, r3
	lsls r0, r0, #16
	asrs r0, r0, #16
	bl Func_02005688
	movs r1, #144
	ldr r0, .L_0200ab78
	lsls r1, r1, #3
	bl Scheduler_AddOrUpdateCallback
	b .L_0200ac0e
	.2byte 0x0000
.L_0200ab64:
	.4byte 0x00000001
.L_0200ab68:
	.4byte Data_02005bac
.L_0200ab6c:
	.4byte Data_02005bd6
.L_0200ab70:
	.4byte gPartyState
.L_0200ab74:
	.4byte Data_020023c4 + 0x88
.L_0200ab78:
	.4byte Func_0200223c
.L_0200ab7c:
	mov r3, r11
	movs r2, #18
	ldrsh r0, [r3, r2]
	movs r2, #22
	ldrsh r1, [r3, r2]
	bl Func_02000218
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #130
	bl GameFlag_SetBit
	movs r0, #161
	lsls r0, r0, #2
	bl GameFlag_SetBit
	mov r1, r11
	movs r3, #18
	ldrsh r0, [r1, r3]
	movs r2, #22
	ldrsh r1, [r1, r2]
	bl Func_020002a4
	b .L_0200ac0e
.L_0200abac:
	ldr r3, .L_0200ac20
	cmp r2, r3
	beq .L_0200ac0e
	ldr r3, .L_0200ac24
	cmp r2, r3
	beq .L_0200ac0e
	ldr r3, .L_0200ac28
	cmp r2, r3
	bne .L_0200ac0e
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200ac0e
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #133
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200ac0e
	ldr r3, .L_0200ac2c
	movs r0, #18
	ldrsh r3, [r3, r0]
	cmp r3, #0
	beq .L_0200abfa
	movs r3, #13
	movs r2, #79
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #26
	movs r1, #79
	movs r2, #1
	movs r3, #2
	bl Func_02005820
	b .L_0200ac0e
.L_0200abfa:
	movs r3, #18
	movs r2, #79
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #26
	movs r1, #79
	movs r2, #1
	movs r3, #2
	bl Func_02005820
.L_0200ac0e:
	movs r0, #0
	add sp, #16
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200ac20:
	.4byte 0x000000f9
.L_0200ac24:
	.4byte 0x000000fa
.L_0200ac28:
	.4byte 0x000000f8
.L_0200ac2c:
	.4byte Data_020023c4 + 0x88
	.section .text.x0200ac30,"ax",%progbits
	.balign 4
	.global Func_02002c30
	.thumb_func
Func_02002c30:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r3, .L_0200ac5c
	ldr r7, .L_0200ac60
	ldr r2, [r3]
	str r0, [r3]
	sub sp, #8
	mov r8, r2
.L_0200ac42:
	adds r2, r7, #0
	movs r5, #7
.L_0200ac46:
	ldr r3, .L_0200ac58
	subs r5, #1
	strh r3, [r2]
	adds r2, #2
	cmp r5, #0
	bge .L_0200ac46
	movs r5, #0
	b .L_0200ac64
	.2byte 0x0000
.L_0200ac58:
	.4byte 0xffffffff
.L_0200ac5c:
	.4byte gRandomState
.L_0200ac60:
	.4byte Data_020023c4 + 0x88
.L_0200ac64:
	bl Random16Far
	lsls r0, r0, #3
	lsrs r0, r0, #16
	lsls r0, r0, #1
	ldrsh r3, [r7, r0]
	movs r2, #1
	negs r2, r2
	cmp r3, r2
	bne .L_0200ac64
	strh r5, [r7, r0]
	adds r5, #1
	cmp r5, #7
	ble .L_0200ac64
	movs r3, #0
	mov r6, sp
	str r3, [r6, #4]
	str r3, [sp, #0]
	ldr r3, .L_0200ace0
	movs r5, #0
	mov r12, r3
	adds r0, r7, #0
.L_0200ac90:
	movs r4, #0
	mov r1, r12
.L_0200ac94:
	ldrb r3, [r1]
	adds r1, #1
	lsls r3, r3, #24
	mov lr, r3
	movs r3, #0
	ldrsh r2, [r0, r3]
	mov r3, lr
	asrs r3, r3, #24
	mov lr, r3
	cmp r2, lr
	bne .L_0200acb8
	movs r3, #1
	ands r3, r5
	lsls r3, r3, #2
	adds r3, r3, r6
	ldr r2, [r3]
	adds r2, #1
	str r2, [r3]
.L_0200acb8:
	adds r4, #1
	cmp r4, #3
	ble .L_0200ac94
	adds r5, #1
	adds r0, #2
	cmp r5, #7
	ble .L_0200ac90
	ldr r3, [sp, #0]
	cmp r3, #0
	beq .L_0200ac42
	ldr r3, [sp, #4]
	cmp r3, #0
	beq .L_0200ac42
	ldr r3, .L_0200ace4
	mov r2, r8
	str r2, [r3]
	add sp, #8
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_0200ace0:
	.4byte Data_02005be0
.L_0200ace4:
	.4byte gRandomState
	.section .text.x0200adec,"ax",%progbits
	.balign 4
	.global Func_02002dec
	.thumb_func
Func_02002dec:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r9
	push {r5, r6}
	mov r6, r8
	push {r6}
	bl Func_02005880
	movs r0, #0
	bl Func_020059e8
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #10
	lsls r1, r1, #7
	bl Func_02005970
	movs r0, #128
	movs r1, #1
	movs r2, #132
	movs r3, #1
	lsls r2, r2, #17
	lsls r0, r0, #17
	negs r1, r1
	bl Motion_CamBounds
	movs r0, #5
	movs r1, #4
	bl Func_020058f0
	movs r0, #6
	movs r1, #4
	bl Func_020058f0
	movs r0, #7
	movs r1, #4
	bl Func_020058f0
	movs r1, #128
	movs r2, #128
	movs r0, #4
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r1, #128
	movs r2, #128
	movs r0, #5
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r1, #128
	movs r2, #128
	movs r0, #6
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #9
	lsls r2, r2, #8
	movs r0, #7
	bl ObjectMotion_SetSpeedParameters
	movs r0, #28
	bl Func_02002d20
	movs r2, #136
	adds r1, r0, #0
	lsls r2, r2, #1
	movs r0, #4
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r0, #28
	bl Func_02002d20
	movs r2, #128
	adds r1, r0, #0
	lsls r2, r2, #1
	movs r0, #7
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r0, #27
	bl Func_02002d20
	movs r2, #248
	adds r1, r0, #0
	movs r0, #5
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r0, #27
	bl Func_02002d20
	movs r2, #140
	adds r1, r0, #0
	lsls r2, r2, #1
	movs r0, #6
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r0, #4
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #0
	bl Func_02002d50
	movs r2, #0
	adds r1, r0, #0
	movs r0, #4
	bl ObjectMotion_ArmCallback
	movs r0, #7
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #0
	bl Func_02002d50
	movs r2, #0
	adds r1, r0, #0
	movs r0, #7
	bl ObjectMotion_ArmCallback
	movs r0, #5
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #0
	bl Func_02002d50
	movs r2, #0
	adds r1, r0, #0
	movs r0, #5
	bl ObjectMotion_ArmCallback
	movs r0, #6
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #0
	bl Func_02002d50
	movs r2, #0
	adds r1, r0, #0
	movs r0, #6
	bl ObjectMotion_ArmCallback
	bl Func_02005980
	movs r0, #20
	bl Task_Wait
	movs r0, #138
	bl Func_02005a68
	movs r0, #20
	bl Task_Wait
	movs r1, #172
	movs r2, #148
	movs r3, #192
	lsls r1, r1, #1
	lsls r2, r2, #1
	lsls r3, r3, #8
	movs r0, #8
	bl Func_02002d78
	movs r0, #8
	bl Object_GetById
	movs r6, #128
	adds r5, r0, #0
	movs r2, #85
	movs r0, #128
	lsls r6, r6, #8
	adds r2, r2, r5
	lsls r0, r0, #9
	movs r3, #0
	str r0, [r5, #48]
	str r6, [r5, #52]
	strb r3, [r2]
	mov r10, r2
	ldr r2, [r5, #12]
	movs r3, #176
	lsls r3, r3, #15
	mov r8, r0
	adds r2, r2, r3
	ldr r1, [r5, #8]
	ldr r3, [r5, #16]
	adds r0, r5, #0
	bl Func_020057f8
	adds r0, r5, #0
	movs r1, #2
	bl Func_020057c8
	adds r0, r5, #0
	bl Func_02005800
	adds r0, r5, #0
	movs r1, #1
	bl Func_020057c8
	mov r0, r10
	movs r3, #2
	strb r3, [r0]
	movs r3, #128
	lsls r3, r3, #16
	str r3, [r5, #20]
	movs r3, #128
	lsls r3, r3, #11
	str r3, [r5, #40]
	movs r3, #128
	lsls r3, r3, #10
	mov r9, r3
	str r3, [r5, #52]
	ldr r0, .L_0200b220
	ldr r3, [r5, #16]
	movs r2, #192
	lsls r2, r2, #10
	ldr r1, [r5, #8]
	adds r3, r3, r0
	mov r10, r2
	str r2, [r5, #48]
	adds r0, r5, #0
	ldr r2, [r5, #12]
	bl Func_020057f8
	adds r0, r5, #0
	bl Func_02005800
	movs r0, #12
	bl Task_Wait
	mov r2, r8
	str r2, [r5, #48]
	str r6, [r5, #52]
	movs r0, #41
	bl Func_02002d20
	movs r3, #132
	adds r1, r0, #0
	ldr r2, [r5, #12]
	lsls r1, r1, #16
	lsls r3, r3, #17
	adds r0, r5, #0
	bl Func_020057f8
	adds r0, r5, #0
	bl Func_02005800
	movs r0, #6
	bl Task_Wait
	movs r0, #152
	bl Func_02005a68
	movs r3, #192
	lsls r3, r3, #11
	str r3, [r5, #40]
	mov r0, r9
	mov r3, r10
	str r3, [r5, #48]
	str r0, [r5, #52]
	movs r0, #37
	bl Func_02002d20
	movs r3, #132
	adds r1, r0, #0
	lsls r3, r3, #17
	ldr r2, [r5, #12]
	lsls r1, r1, #16
	adds r0, r5, #0
	bl Func_020057f8
	adds r0, r5, #0
	bl Func_02005800
	movs r0, #6
	bl Task_Wait
	movs r0, #13
	movs r1, #8
	bl Func_020058f0
	movs r0, #14
	movs r1, #8
	bl Func_020058f0
	mov r1, r8
	adds r2, r6, #0
	movs r0, #8
	bl ObjectMotion_SetSpeedParameters
	mov r1, r8
	adds r2, r6, #0
	movs r0, #13
	bl ObjectMotion_SetSpeedParameters
	mov r1, r8
	adds r2, r6, #0
	movs r0, #14
	bl ObjectMotion_SetSpeedParameters
	movs r0, #36
	bl Func_02002d20
	movs r2, #132
	adds r1, r0, #0
	lsls r2, r2, #1
	movs r0, #8
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r0, #37
	bl Func_02002d20
	movs r2, #248
	adds r1, r0, #0
	movs r0, #13
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r0, #37
	bl Func_02002d20
	movs r2, #140
	adds r1, r0, #0
	lsls r2, r2, #1
	movs r0, #14
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r0, #8
	bl ObjectMotion_CommitCurrentPositionAndActivate
	adds r0, r6, #0
	bl Func_02002d50
	movs r2, #0
	adds r1, r0, #0
	movs r0, #8
	bl ObjectMotion_ArmCallback
	movs r0, #13
	bl ObjectMotion_CommitCurrentPositionAndActivate
	adds r0, r6, #0
	bl Func_02002d50
	movs r2, #0
	adds r1, r0, #0
	movs r0, #13
	bl ObjectMotion_ArmCallback
	movs r0, #14
	bl ObjectMotion_CommitCurrentPositionAndActivate
	adds r0, r6, #0
	bl Func_02002d50
	movs r2, #0
	adds r1, r0, #0
	movs r0, #14
	bl ObjectMotion_ArmCallback
	ldr r0, .L_0200b224
	bl Func_02005938
	movs r1, #128
	movs r2, #10
	lsls r1, r1, #1
	movs r0, #8
	bl Func_02005960
	movs r0, #8
	movs r1, #0
	bl Func_02005940
	movs r0, #8
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #8
	movs r1, #0
	bl Func_02005940
	movs r0, #8
	movs r1, #2
	bl Motion_SetVarCbAndRefresh
	movs r0, #8
	movs r1, #0
	bl Func_02005940
	movs r0, #4
	movs r1, #3
	bl Object_SetModeById
	movs r0, #5
	movs r1, #3
	bl Object_SetModeById
	movs r0, #6
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #7
	bl Object_SetModeById
	movs r0, #15
	bl Battle_WaitMode0
	mov r1, r9
	mov r2, r8
	movs r0, #4
	bl ObjectMotion_SetSpeedParameters
	movs r1, #243
	movs r2, #243
	lsls r1, r1, #9
	lsls r2, r2, #8
	movs r0, #5
	adds r1, #102
	adds r2, #51
	bl ObjectMotion_SetSpeedParameters
	movs r2, #217
	lsls r2, r2, #8
	movs r0, #6
	ldr r1, .L_0200b228
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r1, #230
	movs r2, #230
	lsls r1, r1, #9
	lsls r2, r2, #8
	movs r0, #7
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	mov r1, r9
	mov r2, r8
	movs r0, #8
	bl ObjectMotion_SetSpeedParameters
	movs r1, #243
	movs r2, #243
	lsls r1, r1, #9
	lsls r2, r2, #8
	movs r0, #13
	adds r1, #102
	adds r2, #51
	bl ObjectMotion_SetSpeedParameters
	movs r2, #217
	lsls r2, r2, #8
	ldr r1, .L_0200b228
	adds r2, #153
	movs r0, #14
	bl ObjectMotion_SetSpeedParameters
	movs r0, #3
	bl Func_02002d38
	movs r5, #3
	adds r1, r0, #0
	negs r5, r5
	movs r2, #0
	movs r0, #4
	bl ObjectMotion_OffsetPositionAndReset
	adds r0, r5, #0
	bl Func_02002d38
	movs r2, #0
	adds r1, r0, #0
	movs r0, #8
	bl ObjectMotion_OffsetPositionAndReset
	movs r0, #1
	bl Task_Wait
	movs r0, #3
	bl Func_02002d38
	movs r2, #0
	adds r1, r0, #0
	movs r0, #5
	bl ObjectMotion_OffsetPositionAndReset
	adds r0, r5, #0
	bl Func_02002d38
	movs r2, #0
	adds r1, r0, #0
	movs r0, #13
	bl ObjectMotion_OffsetPositionAndReset
	movs r0, #1
	bl Task_Wait
	movs r0, #3
	bl Func_02002d38
	movs r2, #0
	adds r1, r0, #0
	movs r0, #7
	bl ObjectMotion_OffsetPositionAndReset
	adds r0, r5, #0
	bl Func_02002d38
	movs r2, #0
	adds r1, r0, #0
	movs r0, #14
	bl ObjectMotion_OffsetPositionAndReset
	movs r0, #1
	bl Task_Wait
	movs r0, #3
	bl Func_02002d38
	movs r2, #0
	adds r1, r0, #0
	movs r0, #6
	bl ObjectMotion_OffsetPositionAndReset
	movs r0, #1
	bl Task_Wait
	movs r0, #6
	bl Battle_WaitMode0
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, pc}
.L_0200b220:
	.4byte 0xfff00000
.L_0200b224:
	.4byte MsgTrialRoadSwiftChallenge
.L_0200b228:
	.4byte 0x0001b333
	.section .text.x0200bda0,"ax",%progbits
	.balign 4
	.global Func_02003da0
	.thumb_func
Func_02003da0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r0, .L_0200bf54
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #32]
	mov r9, r0
	movs r0, #158
	mov r4, r9
	lsls r0, r0, #1
	movs r3, #2
	ldrsh r2, [r4, r3]
	adds r3, r1, r0
	ldr r3, [r3]
	sub sp, #4
	str r3, [sp, #0]
	movs r4, #156
	lsls r4, r4, #1
	adds r3, r1, r4
	ldr r1, .L_0200bf58
	ldr r3, [r3]
	lsls r2, r2, #2
	mov r5, r9
	movs r0, #0
	adds r1, r1, r2
	adds r5, #4
	mov r11, r3
	mov r10, r0
	mov r8, r1
.L_0200bde4:
	movs r2, #0
	ldrsh r0, [r5, r2]
	cmp r0, #0
	bne .L_0200bdee
	b .L_0200bf34
.L_0200bdee:
	bl Object_GetById
	adds r7, r0, #0
	ldr r3, [r7, #8]
	cmp r3, #0
	bne .L_0200be02
	ldr r3, [r7, #16]
	cmp r3, #0
	bne .L_0200be02
	b .L_0200bf34
.L_0200be02:
	movs r3, #2
	ldrsh r2, [r5, r3]
	movs r4, #8
	ldrsh r3, [r5, r4]
	cmp r2, r3
	bne .L_0200be18
	movs r0, #206
	bl Func_02005a68
	movs r3, #4
	strh r3, [r5, #18]
.L_0200be18:
	movs r0, #2
	ldrsh r2, [r5, r0]
	movs r1, #10
	ldrsh r3, [r5, r1]
	cmp r2, r3
	bne .L_0200be34
	movs r0, #140
	adds r0, #255
	bl Func_02005a68
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #248
	strh r3, [r5, #18]
.L_0200be34:
	movs r4, #18
	ldrsh r3, [r5, r4]
	ldrh r2, [r5, #18]
	cmp r3, #0
	beq .L_0200be6e
	ldrh r3, [r5, #4]
	movs r0, #0
	adds r3, r3, r2
	movs r4, #12
	ldrsh r2, [r5, r4]
	strh r3, [r5, #4]
	lsls r3, r3, #16
	asrs r3, r3, #16
	ldrh r1, [r5, #12]
	cmp r3, r2
	blt .L_0200be5a
	strh r1, [r5, #4]
	strh r0, [r5, #18]
	strh r0, [r5, #22]
.L_0200be5a:
	movs r1, #4
	ldrsh r2, [r5, r1]
	movs r4, #14
	ldrsh r3, [r5, r4]
	ldrh r1, [r5, #14]
	cmp r2, r3
	bgt .L_0200be6e
	strh r1, [r5, #4]
	strh r0, [r5, #18]
	strh r0, [r5, #22]
.L_0200be6e:
	movs r0, #130
	lsls r0, r0, #1
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200be8e
	ldrh r3, [r5, #2]
	movs r1, #6
	ldrsh r2, [r5, r1]
	adds r3, #1
	strh r3, [r5, #2]
	lsls r3, r3, #16
	asrs r3, r3, #16
	cmp r3, r2
	blt .L_0200be8e
	strh r0, [r5, #2]
.L_0200be8e:
	ldr r3, [r7, #16]
	ldr r2, [r7, #8]
	asrs r3, r3, #20
	asrs r2, r2, #20
	lsls r3, r3, #7
	adds r1, r2, r3
	mov r2, r9
	ldrb r3, [r2, #2]
	ldr r4, [sp, #0]
	add r3, r10
	strb r3, [r4, r1]
	movs r0, #14
	ldrsh r2, [r7, r0]
	movs r4, #4
	ldrsh r3, [r5, r4]
	adds r2, r2, r3
	cmp r2, #0
	bge .L_0200beb4
	adds r2, #7
.L_0200beb4:
	asrs r3, r2, #3
	lsls r3, r3, #8
	mov r0, r8
	str r3, [r0]
	mov r2, r11
	lsls r3, r1, #2
	adds r1, r2, r3
	movs r4, #18
	ldrsh r3, [r5, r4]
	cmp r3, #0
	beq .L_0200bece
	movs r3, #0
	b .L_0200bed4
.L_0200bece:
	ldrb r2, [r1, #3]
	movs r3, #128
	orrs r3, r2
.L_0200bed4:
	strb r3, [r1, #3]
	movs r0, #4
	ldrsh r3, [r5, r0]
	cmp r3, #0
	beq .L_0200bee4
	ldrb r2, [r1, #3]
	movs r3, #16
	orrs r3, r2
.L_0200bee4:
	strb r3, [r1, #3]
	ldr r6, [r5, #24]
	cmp r6, #0
	beq .L_0200bf34
	movs r1, #4
	ldrsh r3, [r5, r1]
	cmp r3, #0
	bne .L_0200befc
	ldr r3, [r7, #12]
	ldr r2, .L_0200bf5c
	adds r3, r3, r2
	b .L_0200bf2a
.L_0200befc:
	ldrh r0, [r5, #22]
	movs r3, #128
	lsls r3, r3, #5
	adds r0, r0, r3
	strh r0, [r5, #22]
	lsls r0, r0, #16
	asrs r0, r0, #16
	bl Math_Sine
	adds r1, r0, #0
	movs r0, #128
	ldr r3, .L_0200bf60
	lsls r0, r0, #10
	mov lr, r3
	.2byte 0xf800
	movs r1, #4
	ldrsh r2, [r5, r1]
	ldr r3, [r7, #12]
	ldr r4, .L_0200bf64
	lsls r2, r2, #16
	adds r0, r0, r4
	adds r3, r3, r2
	adds r3, r3, r0
.L_0200bf2a:
	str r3, [r6, #12]
	ldr r3, [r7, #8]
	str r3, [r6, #8]
	ldr r3, [r7, #16]
	str r3, [r6, #16]
.L_0200bf34:
	movs r3, #1
	add r10, r3
	movs r2, #4
	mov r4, r10
	add r8, r2
	adds r5, #28
	cmp r4, #15
	bgt .L_0200bf46
	b .L_0200bde4
.L_0200bf46:
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0200bf54:
	.4byte Data_020023c4 + 0x188
.L_0200bf58:
	.4byte gMapCollision
.L_0200bf5c:
	.4byte 0xfff00000
.L_0200bf60:
	.4byte IwramMulQ16
.L_0200bf64:
	.4byte 0xfff20000
	.section .text.x0200bf68,"ax",%progbits
	.balign 4
	.global Func_02003f68
	.thumb_func
Func_02003f68:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r2, .L_0200c16c
	sub sp, #36
	adds r0, r2, #4
	str r0, [sp, #32]
	ldr r1, .L_0200c170
	movs r0, #0
	ldrsh r3, [r2, r0]
	lsls r3, r3, #2
	adds r3, r3, r1
	ldrh r3, [r3, #2]
	movs r1, #226
	lsrs r3, r3, #5
	str r3, [sp, #24]
	lsls r1, r1, #1
	adds r2, r2, r1
	ldr r2, [r2]
	movs r3, #192
	mov r9, r2
	movs r2, #0
	str r2, [sp, #16]
	lsls r3, r3, #18
	ldr r1, [r3, #32]
	adds r2, r1, #0
	adds r2, #228
	ldr r3, [r2]
	str r3, [sp, #12]
	ldr r0, [sp, #12]
	ldr r3, .L_0200c174
	ands r0, r3
	str r0, [sp, #12]
	ldr r2, [r2, #4]
	ands r2, r3
	str r2, [sp, #8]
	ldr r3, [r1]
	movs r1, #15
	ldr r3, [r3, #4]
	str r1, [sp, #28]
	str r3, [sp, #4]
.L_0200bfc2:
	ldr r3, [sp, #32]
	movs r2, #0
	ldrsh r0, [r3, r2]
	cmp r0, #0
	bne .L_0200bfce
	b .L_0200c14c
.L_0200bfce:
	movs r1, #4
	ldrsh r3, [r3, r1]
	cmp r3, #0
	bne .L_0200bfd8
	b .L_0200c14c
.L_0200bfd8:
	bl Object_GetById
	ldr r3, .L_0200c178
	movs r1, #12
	mov r10, r0
	ldr r0, [r3]
	bl __umodsi3
	movs r1, #3
	bl __udivsi3
	lsls r0, r0, #3
	adds r0, #32
	str r0, [sp, #20]
	ldr r1, [sp, #32]
	movs r2, #0
	movs r0, #4
	ldrsh r3, [r1, r0]
	mov r11, r2
	cmp r11, r3
	bge .L_0200c09c
.L_0200c002:
	ldr r2, [sp, #16]
	cmp r2, #79
	bgt .L_0200c08e
	mov r0, r10
	ldr r3, [r0, #8]
	ldr r1, [sp, #12]
	subs r7, r3, r1
	mov r3, r11
	lsls r2, r3, #16
	ldr r3, [r0, #12]
	ldr r0, [sp, #4]
	adds r3, r3, r2
	mov r1, r10
	subs r0, r3, r0
	ldr r2, [sp, #8]
	ldr r3, [r1, #16]
	mov r8, r0
	ldr r0, [sp, #4]
	subs r3, r3, r2
	subs r5, r3, r0
	mov r1, r8
	subs r6, r5, r1
	asrs r3, r6, #16
	adds r6, r3, #0
	adds r3, r1, r5
	asrs r3, r3, #16
	asrs r2, r7, #16
	adds r1, r3, #0
	movs r3, #167
	adds r7, r2, #0
	lsls r3, r3, #1
	adds r2, #7
	subs r7, #8
	subs r6, #16
	adds r1, #58
	cmp r2, r3
	bhi .L_0200c08e
	movs r0, #16
	negs r0, r0
	cmp r6, r0
	ble .L_0200c08e
	cmp r6, #239
	bgt .L_0200c08e
	adds r3, #177
	ands r7, r3
	movs r3, #255
	mov r4, r9
	ands r6, r3
	movs r3, #0
	stmia r4!, {r3}
	lsls r3, r7, #16
	orrs r6, r3
	ldr r3, .L_0200c17c
	orrs r6, r3
	stmia r4!, {r6}
	ldr r2, [sp, #24]
	ldr r0, [sp, #20]
	adds r3, r2, r0
	movs r2, #128
	lsls r2, r2, #4
	orrs r3, r2
	mov r0, r9
	str r3, [r4]
	bl Func_02005790
	ldr r2, [sp, #16]
	movs r1, #12
	adds r2, #1
	str r2, [sp, #16]
	add r9, r1
.L_0200c08e:
	ldr r1, [sp, #32]
	movs r3, #16
	add r11, r3
	movs r0, #4
	ldrsh r3, [r1, r0]
	cmp r11, r3
	blt .L_0200c002
.L_0200c09c:
	ldr r2, [sp, #16]
	cmp r2, #79
	bgt .L_0200c14c
	mov r0, r10
	ldr r3, [r0, #8]
	ldr r1, [sp, #12]
	ldr r0, [sp, #32]
	subs r7, r3, r1
	movs r3, #4
	ldrsh r2, [r0, r3]
	mov r1, r10
	ldr r3, [r1, #12]
	lsls r2, r2, #16
	adds r3, r3, r2
	ldr r2, [sp, #4]
	ldr r0, [sp, #8]
	subs r2, r3, r2
	ldr r3, [r1, #16]
	ldr r1, [sp, #4]
	subs r3, r3, r0
	subs r5, r3, r1
	ldr r3, [sp, #32]
	subs r6, r5, r2
	mov r8, r2
	movs r2, #22
	ldrsh r0, [r3, r2]
	bl Math_Sine
	ldr r3, .L_0200c180
	adds r1, r0, #0
	ldr r0, .L_0200c184
	mov lr, r3
	.2byte 0xf800
	ldr r1, .L_0200c184
	adds r0, r6, r0
	mov r2, r8
	asrs r7, r7, #16
	adds r0, r0, r1
	adds r3, r2, r5
	mov r10, r7
	asrs r0, r0, #16
	asrs r3, r3, #16
	adds r1, r3, #0
	subs r6, r0, #4
	mov r3, r10
	movs r0, #167
	adds r3, #7
	lsls r0, r0, #1
	subs r7, #8
	adds r1, #58
	cmp r3, r0
	bhi .L_0200c14c
	movs r2, #16
	negs r2, r2
	cmp r6, r2
	ble .L_0200c14c
	cmp r6, #239
	bgt .L_0200c14c
	movs r3, #128
	lsls r3, r3, #1
	adds r3, #255
	ands r7, r3
	movs r3, #255
	mov r4, r9
	ands r6, r3
	movs r3, #0
	stmia r4!, {r3}
	lsls r3, r7, #16
	orrs r6, r3
	ldr r3, .L_0200c17c
	orrs r6, r3
	stmia r4!, {r6}
	ldr r0, [sp, #24]
	ldr r2, [sp, #20]
	adds r3, r0, r2
	movs r2, #128
	lsls r2, r2, #4
	subs r3, #32
	orrs r3, r2
	mov r0, r9
	str r3, [r4]
	bl Func_02005790
	ldr r0, [sp, #16]
	movs r3, #12
	adds r0, #1
	str r0, [sp, #16]
	add r9, r3
.L_0200c14c:
	ldr r1, [sp, #28]
	ldr r2, [sp, #32]
	subs r1, #1
	adds r2, #28
	str r1, [sp, #28]
	str r2, [sp, #32]
	cmp r1, #0
	blt .L_0200c15e
	b .L_0200bfc2
.L_0200c15e:
	add sp, #36
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0200c16c:
	.4byte Data_020023c4 + 0x188
.L_0200c170:
	.4byte ResourceTableEntries
.L_0200c174:
	.4byte 0xffff0000
.L_0200c178:
	.4byte gFrameCount
.L_0200c17c:
	.4byte 0x40002000
.L_0200c180:
	.4byte IwramMulQ16
.L_0200c184:
	.4byte 0xfffe0000
	.section .text.x0200c224,"ax",%progbits
	.balign 4
	.global Func_02004224
	.thumb_func
Func_02004224:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	mov r9, r2
	mov r10, r3
	ldr r2, .L_0200c2b8
	lsls r3, r0, #3
	subs r3, r3, r0
	mov r8, r1
	lsls r3, r3, #2
	adds r3, r3, r2
	mov r0, r8
	ldr r7, [sp, #28]
	adds r5, r3, #4
	bl Object_GetById
	adds r6, r0, #0
	mov r0, r8
	bl Object_GetById
	adds r0, r6, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200c2bc
	cmp r7, r10
	bge .L_0200c26e
	mov r12, r10
	mov r10, r7
	mov r7, r12
.L_0200c26e:
	mov r3, r8
	strh r3, [r5]
	mov r3, r9
	strh r3, [r5, #2]
	movs r3, #180
	lsls r3, r3, #1
	strh r3, [r5, #6]
	movs r3, #60
	strh r3, [r5, #8]
	movs r3, #240
	strh r3, [r5, #10]
	mov r3, r10
	ldr r2, .L_0200c2b4
	strh r3, [r5, #14]
	movs r3, #1
	strh r3, [r5, #16]
	adds r3, r6, #0
	adds r3, #85
	strb r2, [r3]
	adds r2, r6, #0
	adds r2, #89
	movs r3, #8
	strb r3, [r2]
	adds r1, r6, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #2
	orrs r3, r2
	strh r0, [r5, #4]
	strh r7, [r5, #12]
	strh r0, [r5, #18]
	strh r0, [r5, #22]
	strh r0, [r5, #20]
	strb r3, [r1]
	b .L_0200c2bc
.L_0200c2b4:
	.4byte 0x00000000
.L_0200c2b8:
	.4byte Data_020023c4 + 0x188
.L_0200c2bc:
	movs r0, #128
	ldr r1, [r6, #8]
	ldr r2, [r6, #12]
	ldr r3, [r6, #16]
	lsls r0, r0, #8
	bl Func_020057d8
	adds r1, r0, #0
	adds r2, r1, #0
	movs r3, #0
	adds r2, #89
	strb r3, [r2]
	subs r2, #4
	strb r3, [r2]
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #128
	orrs r3, r2
	strb r3, [r0]
	str r1, [r5, #24]
	adds r0, r5, #0
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.section .text.x0200c2f0,"ax",%progbits
	.balign 4
	.global Func_020042f0
	.thumb_func
Func_020042f0:
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
	sub sp, #20
	str r3, [sp, #16]
	movs r2, #255
	ldrh r3, [r0]
	lsls r2, r2, #8
	adds r2, #255
	mov r11, r0
	cmp r3, r2
	beq .L_0200c3d6
.L_0200c316:
	mov r3, r11
	ldrh r3, [r3]
	adds r0, r3, #0
	str r3, [sp, #12]
	bl Object_GetById
	mov r2, r11
	ldrh r2, [r2, #2]
	adds r7, r0, #0
	str r2, [sp, #8]
	movs r3, #34
	adds r3, r3, r7
	adds r0, r2, #0
	ldrb r2, [r3]
	mov r9, r3
	lsls r3, r2, #3
	subs r3, r3, r2
	movs r2, #156
	lsls r2, r2, #1
	lsls r3, r3, #3
	adds r3, r3, r2
	ldr r2, [sp, #16]
	adds r0, #1
	ldr r5, [r2, r3]
	ldr r2, .L_0200c40c
	adds r3, r5, r2
	ldr r2, .L_0200c410
	asrs r3, r3, #2
	adds r6, r3, r2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200c364
	ldr r0, [sp, #12]
	movs r1, #0
	movs r2, #0
	bl Func_020058e0
	b .L_0200c3c4
.L_0200c364:
	adds r0, r7, #0
	bl Func_02005a18
	mov r8, r0
	mov r3, r8
	lsls r3, r3, #2
	adds r5, r5, r3
	str r5, [sp, #4]
	mov r2, r9
	ldrb r0, [r2]
	ldr r1, [r7, #8]
	ldr r2, [r7, #16]
	bl Func_02005848
	mov r3, r9
	ldr r1, [r7, #8]
	ldr r2, [r7, #16]
	mov r10, r0
	ldrb r0, [r3]
	bl Map_GetTerrainHeight
	adds r5, r0, #0
	ldr r0, [sp, #12]
	bl Func_02005958
	ldr r2, [sp, #4]
	movs r3, #128
	asrs r5, r5, #19
	strb r3, [r2, #3]
	adds r5, #4
	mov r3, r9
	adds r2, r5, #0
	ldrb r0, [r3]
	mov r1, r10
	bl Func_02005a20
	add r8, r6
	mov r2, r8
	strb r0, [r2]
	ldr r0, [sp, #8]
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200c3c4
	adds r0, r7, #0
	movs r1, #0
	bl Func_020057c8
.L_0200c3c4:
	movs r3, #4
	add r11, r3
	mov r2, r11
	ldrh r3, [r2]
	movs r2, #255
	lsls r2, r2, #8
	adds r2, #255
	cmp r3, r2
	bne .L_0200c316
.L_0200c3d6:
	ldr r3, .L_0200c414
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	adds r5, r0, #0
	adds r3, r5, #0
	adds r3, #34
	ldrb r0, [r3]
	ldr r1, [r5, #8]
	ldr r2, [r5, #16]
	bl Map_GetTerrainHeight
	ldr r3, [r5, #12]
	cmp r3, r0
	bge .L_0200c3fe
	str r0, [r5, #20]
	str r0, [r5, #12]
.L_0200c3fe:
	add sp, #20
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0200c40c:
	.4byte 0xfdff0000
.L_0200c410:
	.4byte gMapShapeGrid
.L_0200c414:
	.4byte gPartyState
	.section .text.x0200c418,"ax",%progbits
	.balign 4
	.global Func_02004418
	.thumb_func
Func_02004418:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	sub sp, #12
	adds r5, r0, #0
	bl Func_020059c0
	cmp r0, #0
	beq .L_0200c42e
	b .L_0200c59e
.L_0200c42e:
	ldr r3, .L_0200c5a8
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	adds r6, r0, #0
	ldr r3, [r6, #8]
	mov r0, sp
	str r3, [r0]
	movs r1, #0
	ldr r3, [r6, #12]
	str r3, [r0, #4]
	ldr r3, [r6, #16]
	str r3, [r0, #8]
	bl Func_02005a28
	mov r8, r0
	cmp r0, #0
	bne .L_0200c45a
	b .L_0200c59e
.L_0200c45a:
	b .L_0200c590
.L_0200c45c:
	ldrh r7, [r5]
	adds r0, r7, #0
	bl Object_GetById
	cmp r0, r8
	beq .L_0200c46c
	adds r5, #4
	b .L_0200c590
.L_0200c46c:
	ldrh r5, [r5, #2]
	bl Func_02005880
	adds r0, r5, #0
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200c4da
	movs r0, #125
	bl Func_02005a68
	adds r0, r7, #0
	bl Object_GetById
	movs r1, #7
	bl Object_SetPartAttribute
	movs r0, #2
	bl Task_Wait
	movs r1, #0
	mov r0, r8
	bl Func_020057c8
	adds r0, r7, #0
	bl Object_GetById
	movs r1, #0
	bl Object_SetPartAttribute
	movs r0, #2
	bl Task_Wait
	adds r0, r7, #0
	bl Object_GetById
	movs r1, #7
	bl Object_SetPartAttribute
	movs r0, #4
	bl Task_Wait
	adds r0, r7, #0
	bl Object_GetById
	movs r1, #0
	bl Object_SetPartAttribute
	movs r0, #0
	bl Func_02004a94
	adds r0, r5, #0
	bl GameFlag_SetBit
	b .L_0200c58a
.L_0200c4da:
	adds r5, #1
	mov r10, r5
	mov r0, r10
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200c58a
	adds r6, #85
	strb r0, [r6]
	movs r0, #185
	bl Func_02005a68
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #9
	lsls r1, r1, #11
	lsls r0, r0, #9
	bl Func_02005838
	movs r0, #0
	bl Func_02004a94
	movs r5, #2
	movs r0, #8
	mov r7, r8
	bl Task_Wait
	negs r5, r5
	mov r0, r8
	movs r1, #2
	adds r7, #34
	bl Func_020057c8
	ldrb r1, [r7]
	adds r0, r5, #0
	bl Func_02004d6c
	movs r0, #1
	bl Func_02004a94
	movs r0, #16
	bl Task_Wait
	ldrb r1, [r7]
	adds r0, r5, #0
	bl Func_02004d6c
	movs r0, #4
	bl Task_Wait
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	negs r1, r1
	adds r2, #102
	negs r0, r0
	bl Func_02005838
	movs r0, #8
	bl Task_Wait
	movs r3, #3
	strb r3, [r6]
	movs r0, #5
	bl Task_Wait
	movs r1, #0
	movs r2, #0
	movs r3, #0
	mov r0, r8
	bl Object_SetPositionAndResetMotion
	movs r0, #2
	bl Task_Wait
	movs r0, #188
	bl Func_02005a68
	bl Func_02004bc8
	movs r0, #20
	bl Task_Wait
	mov r0, r10
	bl GameFlag_SetBit
.L_0200c58a:
	bl Func_02005888
	b .L_0200c59e
.L_0200c590:
	movs r2, #255
	ldrh r3, [r5]
	lsls r2, r2, #8
	adds r2, #255
	cmp r3, r2
	beq .L_0200c59e
	b .L_0200c45c
.L_0200c59e:
	add sp, #12
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_0200c5a8:
	.4byte gPartyState
	.section .text.x0200c5ac,"ax",%progbits
	.balign 4
	.global Func_020045ac
	.thumb_func
Func_020045ac:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r6, r1, #0
	adds r5, r0, #0
	adds r0, r6, #0
	bl Object_GetById
	movs r2, #255
	ldrh r3, [r5]
	lsls r2, r2, #8
	adds r2, #255
	adds r7, r0, #0
	cmp r3, r2
	beq .L_0200c686
.L_0200c5ca:
	ldrh r3, [r5]
	cmp r3, r6
	beq .L_0200c5d4
	adds r5, #4
	b .L_0200c67a
.L_0200c5d4:
	ldrh r5, [r5, #2]
	bl Func_02005880
	adds r3, r5, #1
	mov r8, r3
	mov r0, r8
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200c674
	movs r0, #185
	bl Func_02005a68
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #9
	lsls r1, r1, #11
	lsls r0, r0, #9
	bl Func_02005838
	movs r0, #0
	bl Func_02004a94
	movs r0, #8
	bl Task_Wait
	adds r0, r7, #0
	movs r1, #2
	bl Func_020057c8
	adds r3, r7, #0
	adds r3, #34
	movs r0, #4
	ldrb r1, [r3]
	adds r2, r6, #0
	negs r0, r0
	bl Func_02004ce0
	movs r0, #1
	bl Func_02004a94
	movs r0, #16
	bl Task_Wait
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	negs r1, r1
	adds r2, #102
	negs r0, r0
	bl Func_02005838
	movs r0, #8
	bl Task_Wait
	movs r1, #0
	movs r2, #0
	movs r3, #0
	adds r0, r7, #0
	bl Object_SetPositionAndResetMotion
	movs r0, #2
	bl Task_Wait
	movs r0, #188
	bl Func_02005a68
	bl Func_02004bc8
	movs r0, #20
	bl Task_Wait
	adds r0, r5, #0
	bl GameFlag_SetBit
	mov r0, r8
	bl GameFlag_SetBit
.L_0200c674:
	bl Func_02005888
	b .L_0200c686
.L_0200c67a:
	movs r2, #255
	ldrh r3, [r5]
	lsls r2, r2, #8
	adds r2, #255
	cmp r3, r2
	bne .L_0200c5ca
.L_0200c686:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.section .text.x0200c6a4,"ax",%progbits
	.balign 4
	.global Func_020046a4
	.thumb_func
Func_020046a4:
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
	sub sp, #16
	ldr r5, .L_0200c800
	str r3, [sp, #12]
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
	mov r11, r0
	ldr r1, [r5]
	movs r0, #8
	bl Func_020058f0
	ldr r1, [r5]
	movs r0, #9
	bl Func_020058f0
	ldr r1, [r5]
	movs r0, #10
	bl Func_020058f0
	movs r0, #1
	bl Task_Wait
	movs r0, #8
	bl Func_0200468c
	movs r0, #9
	bl Func_0200468c
	movs r0, #10
	bl Func_0200468c
	movs r1, #0
	movs r0, #9
	bl Object_SetModeById
	movs r0, #1
	bl Task_Wait
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl Func_020058e0
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_020058e0
	movs r2, #0
	movs r0, #10
	movs r1, #0
	bl Func_020058e0
	movs r0, #1
	bl Task_Wait
	b .L_0200c7de
.L_0200c72a:
	mov r3, r11
	ldrh r3, [r3]
	mov r9, r3
	mov r0, r9
	bl Object_GetById
	mov r2, r11
	ldrh r2, [r2, #2]
	adds r5, r0, #0
	str r2, [sp, #8]
	adds r7, r5, #0
	adds r7, #34
	adds r0, r2, #0
	ldrb r2, [r7]
	adds r0, #1
	lsls r3, r2, #3
	subs r3, r3, r2
	movs r2, #156
	lsls r2, r2, #1
	lsls r3, r3, #3
	adds r3, r3, r2
	ldr r2, [sp, #12]
	ldr r6, [r2, r3]
	ldr r2, .L_0200c804
	adds r3, r6, r2
	ldr r2, .L_0200c808
	asrs r3, r3, #2
	adds r2, r2, r3
	mov r10, r2
	adds r2, r5, #0
	adds r2, #92
	movs r3, #3
	strb r3, [r2]
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200c780
	mov r0, r9
	movs r1, #0
	movs r2, #0
	bl Func_020058e0
	b .L_0200c7da
.L_0200c780:
	adds r0, r5, #0
	bl Func_02005a18
	mov r8, r0
	mov r3, r8
	lsls r3, r3, #2
	adds r6, r6, r3
	str r6, [sp, #4]
	add r8, r10
	ldr r1, [r5, #8]
	ldr r2, [r5, #16]
	ldrb r0, [r7]
	bl Func_02005848
	ldr r1, [r5, #8]
	ldr r2, [r5, #16]
	mov r10, r0
	ldrb r0, [r7]
	bl Map_GetTerrainHeight
	adds r5, r0, #0
	mov r0, r9
	bl Func_02005958
	ldr r6, [sp, #4]
	asrs r5, r5, #19
	movs r3, #128
	adds r5, #4
	adds r2, r5, #0
	strb r3, [r6, #3]
	ldrb r0, [r7]
	mov r1, r10
	bl Func_02005a20
	mov r2, r8
	strb r0, [r2]
	ldr r0, [sp, #8]
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200c7da
	mov r0, r9
	movs r1, #9
	bl ObjectVisual_CopyAttributes
.L_0200c7da:
	movs r3, #4
	add r11, r3
.L_0200c7de:
	mov r2, r11
	ldrh r3, [r2]
	movs r2, #255
	lsls r2, r2, #8
	adds r2, #255
	cmp r3, r2
	bne .L_0200c72a
	movs r0, #10
	bl Task_Wait
	add sp, #16
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0200c800:
	.4byte gPartyState
.L_0200c804:
	.4byte 0xfdff0000
.L_0200c808:
	.4byte gMapShapeGrid
	.section .text.x0200c828,"ax",%progbits
	.balign 4
	.global Func_02004828
	.thumb_func
Func_02004828:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #12
	adds r6, r0, #0
	bl Func_020059c0
	cmp r0, #0
	beq .L_0200c840
	b .L_0200ca02
.L_0200c840:
	ldr r3, .L_0200ca10
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	adds r5, r0, #0
	movs r0, #8
	bl Object_GetById
	ldr r3, [r5, #8]
	adds r7, r0, #0
	mov r0, sp
	str r3, [r0]
	movs r1, #0
	ldr r3, [r5, #12]
	str r3, [r0, #4]
	ldr r3, [r5, #16]
	str r3, [r0, #8]
	bl Func_02005a28
	mov r10, r0
	cmp r0, #0
	bne .L_0200c874
	b .L_0200ca02
.L_0200c874:
	b .L_0200c9f4
.L_0200c876:
	ldrh r3, [r6]
	mov r8, r3
	mov r0, r8
	bl Object_GetById
	cmp r0, r10
	beq .L_0200c888
	adds r6, #4
	b .L_0200c9f4
.L_0200c888:
	ldrh r6, [r6, #2]
	bl Func_02005880
	adds r0, r6, #0
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200c922
	adds r0, r7, #0
	movs r1, #1
	bl Func_020057c8
	mov r1, r10
	adds r0, r7, #0
	bl Func_0200480c
	movs r0, #1
	bl Task_Wait
	movs r0, #125
	bl Func_02005a68
	movs r0, #8
	bl Object_GetById
	movs r1, #7
	bl Object_SetPartAttribute
	movs r0, #2
	bl Task_Wait
	adds r0, r7, #0
	movs r1, #0
	bl Func_020057c8
	movs r1, #9
	mov r0, r8
	bl ObjectVisual_CopyAttributes
	movs r0, #8
	bl Object_GetById
	movs r1, #0
	bl Object_SetPartAttribute
	movs r0, #2
	bl Task_Wait
	movs r0, #8
	bl Object_GetById
	movs r1, #7
	bl Object_SetPartAttribute
	movs r0, #4
	bl Task_Wait
	movs r0, #8
	bl Object_GetById
	movs r1, #0
	bl Object_SetPartAttribute
	movs r0, #0
	bl Func_02004a94
	mov r0, r10
	adds r1, r7, #0
	bl Func_0200480c
	movs r0, #1
	bl Task_Wait
	adds r0, r6, #0
	bl GameFlag_SetBit
	b .L_0200c9ee
.L_0200c922:
	adds r6, #1
	mov r9, r6
	mov r0, r9
	bl GameFlag_Test
	adds r6, r0, #0
	cmp r6, #0
	bne .L_0200c9ee
	adds r0, r7, #0
	movs r1, #0
	bl Func_020057c8
	mov r1, r10
	adds r0, r7, #0
	bl Func_0200480c
	adds r5, #85
	movs r0, #1
	bl Task_Wait
	strb r6, [r5]
	movs r0, #185
	bl Func_02005a68
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #9
	lsls r1, r1, #11
	lsls r0, r0, #9
	bl Func_02005838
	movs r0, #0
	bl Func_02004a94
	mov r8, r5
	movs r0, #8
	movs r6, #2
	mov r5, r10
	bl Task_Wait
	negs r6, r6
	adds r0, r7, #0
	movs r1, #2
	adds r5, #34
	bl Func_020057c8
	ldrb r1, [r5]
	adds r0, r6, #0
	bl Func_02004d6c
	movs r0, #1
	bl Func_02004a94
	movs r0, #16
	bl Task_Wait
	ldrb r1, [r5]
	adds r0, r6, #0
	bl Func_02004d6c
	movs r0, #4
	bl Task_Wait
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	negs r1, r1
	adds r2, #102
	negs r0, r0
	bl Func_02005838
	movs r0, #8
	bl Task_Wait
	movs r3, #3
	mov r2, r8
	strb r3, [r2]
	movs r0, #5
	bl Task_Wait
	movs r1, #0
	movs r2, #0
	movs r3, #0
	adds r0, r7, #0
	bl Object_SetPositionAndResetMotion
	movs r0, #2
	bl Task_Wait
	movs r0, #188
	bl Func_02005a68
	bl Func_02004bc8
	movs r0, #20
	bl Task_Wait
	mov r0, r9
	bl GameFlag_SetBit
.L_0200c9ee:
	bl Func_02005888
	b .L_0200ca02
.L_0200c9f4:
	movs r2, #255
	ldrh r3, [r6]
	lsls r2, r2, #8
	adds r2, #255
	cmp r3, r2
	beq .L_0200ca02
	b .L_0200c876
.L_0200ca02:
	add sp, #12
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200ca10:
	.4byte gPartyState
	.section .text.x0200ca14,"ax",%progbits
	.balign 4
	.global Func_02004a14
	.thumb_func
Func_02004a14:
	push {r5, r6, r7, lr}
	adds r6, r0, #0
	ldr r5, [r6, #68]
	ldr r3, [r6, #8]
	ldr r2, [r6, #72]
	adds r3, r3, r5
	str r3, [r6, #8]
	ldr r3, [r6, #12]
	ldr r7, [r6, #76]
	adds r3, r3, r2
	str r3, [r6, #12]
	ldr r3, [r6, #16]
	adds r0, r5, #0
	adds r3, r3, r7
	movs r1, #18
	str r3, [r6, #16]
	bl __divsi3
	subs r5, r5, r0
	str r5, [r6, #68]
	adds r3, r7, #0
	cmp r7, #0
	bge .L_0200ca44
	adds r3, #15
.L_0200ca44:
	asrs r3, r3, #4
	subs r3, r7, r3
	str r3, [r6, #76]
	ldr r2, [r6, #48]
	ldr r3, [r6, #24]
	ldr r1, [r6, #80]
	adds r3, r3, r2
	str r3, [r6, #24]
	ldr r2, [r6, #52]
	ldr r3, [r6, #28]
	adds r3, r3, r2
	str r3, [r6, #28]
	adds r2, r6, #0
	adds r2, #100
	ldrh r3, [r1, #18]
	ldrh r2, [r2]
	adds r3, r3, r2
	strh r3, [r1, #18]
	ldr r3, .L_0200ca90
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	ldr r3, [r0, #80]
	ldr r4, [r6, #80]
	ldrb r3, [r3, #9]
	ldrb r1, [r4, #9]
	movs r2, #12
	ands r2, r3
	movs r3, #13
	negs r3, r3
	ands r3, r1
	orrs r3, r2
	strb r3, [r4, #9]
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200ca90:
	.4byte gPartyState
	.section .text.x0200ca94,"ax",%progbits
	.balign 4
	.global Func_02004a94
	.thumb_func
Func_02004a94:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	ldr r3, .L_0200cba0
	mov r8, r0
	movs r0, #133
	lsls r0, r0, #2
	adds r3, r3, r0
	ldr r0, [r3]
	bl Object_GetById
	movs r2, #0
	adds r7, r0, #0
	mov r9, r2
	mov r10, r2
.L_0200cab6:
	bl Random16Far
	lsls r3, r0, #3
	subs r3, r3, r0
	ldr r2, [r7, #12]
	lsls r3, r3, #1
	lsrs r3, r3, #16
	lsls r3, r3, #16
	subs r2, r2, r3
	mov r3, r10
	lsls r1, r3, #17
	ldr r3, [r7, #8]
	ldr r0, .L_0200cba4
	adds r1, r1, r3
	ldr r3, .L_0200cba8
	adds r1, r1, r0
	movs r0, #30
	adds r2, r2, r3
	adds r0, #255
	ldr r3, [r7, #16]
	bl Func_020057d8
	adds r6, r0, #0
	cmp r6, #0
	beq .L_0200cb8a
	mov r1, r9
	ldr r0, [r6, #80]
	bl Func_020059f0
	adds r2, r6, #0
	movs r3, #0
	adds r2, #85
	strb r3, [r2]
	adds r2, #4
	strb r3, [r2]
	movs r1, #0
	mov r9, r0
	adds r0, r6, #0
	bl ObjectDispatch_SetSingleChildFlags
	adds r0, r6, #0
	movs r1, #2
	bl Func_020057c8
	adds r0, r6, #0
	ldr r1, .L_0200cbac
	bl Func_020057d0
	movs r3, #179
	lsls r3, r3, #8
	adds r3, #51
	str r3, [r6, #24]
	str r3, [r6, #28]
	ldr r1, [r6, #80]
	movs r0, #13
	ldrb r3, [r1, #9]
	negs r0, r0
	adds r2, r0, #0
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	mov r2, r8
	strb r3, [r1, #9]
	cmp r2, #0
	beq .L_0200cb54
	mov r3, r10
	lsls r5, r3, #13
	adds r0, r5, #0
	bl Math_Cosine
	ldr r3, .L_0200cbb0
	ldr r1, .L_0200cbb4
	mov lr, r3
	.2byte 0xf800
	str r0, [r6, #68]
	adds r0, r5, #0
	bl Math_Sine
	b .L_0200cb58
.L_0200cb54:
	mov r0, r8
	str r0, [r6, #68]
.L_0200cb58:
	str r0, [r6, #76]
	bl Random16Far
	movs r2, #192
	lsls r0, r0, #14
	lsls r2, r2, #7
	lsrs r0, r0, #16
	adds r0, r0, r2
	negs r0, r0
	str r0, [r6, #72]
	bl Random16Far
	ldr r3, .L_0200cbb8
	lsls r0, r0, #9
	lsrs r0, r0, #16
	adds r0, r0, r3
	adds r3, r6, #0
	adds r3, #100
	strh r0, [r3]
	ldr r3, .L_0200cbbc
	str r3, [r6, #48]
	ldr r3, .L_0200cbc0
	str r3, [r6, #52]
	ldr r3, .L_0200cbc4
	str r3, [r6, #108]
.L_0200cb8a:
	movs r0, #1
	add r10, r0
	mov r2, r10
	cmp r2, #7
	bls .L_0200cab6
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200cba0:
	.4byte gPartyState
.L_0200cba4:
	.4byte 0xfff80000
.L_0200cba8:
	.4byte 0xfffe0000
.L_0200cbac:
	.4byte Data_02006254
.L_0200cbb0:
	.4byte IwramMulQ16
.L_0200cbb4:
	.4byte 0x00013333
.L_0200cbb8:
	.4byte 0xffffff00
.L_0200cbbc:
	.4byte 0xfffff800
.L_0200cbc0:
	.4byte 0xfffffa00
.L_0200cbc4:
	.4byte Func_02004a14
	.section .text.x0200cbc8,"ax",%progbits
	.balign 4
	.global Func_02004bc8
	.thumb_func
Func_02004bc8:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	ldr r3, .L_0200ccc8
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	adds r6, r0, #0
	cmp r6, #0
	beq .L_0200ccbc
	movs r3, #0
	mov r9, r3
	mov r10, r3
.L_0200cbec:
	movs r0, #30
	ldr r1, [r6, #8]
	ldr r2, [r6, #12]
	ldr r3, [r6, #16]
	adds r0, #255
	bl Func_020057d8
	adds r7, r0, #0
	cmp r7, #0
	beq .L_0200ccb2
	mov r1, r9
	ldr r0, [r7, #80]
	bl Func_020059f0
	movs r4, #0
	mov r8, r4
	adds r3, r7, #0
	mov r2, r8
	adds r3, #85
	strb r2, [r3]
	adds r3, #4
	strb r2, [r3]
	movs r1, #0
	mov r9, r0
	adds r0, r7, #0
	bl ObjectDispatch_SetSingleChildFlags
	adds r0, r7, #0
	movs r1, #2
	bl Func_020057c8
	ldr r1, .L_0200cccc
	adds r0, r7, #0
	bl Func_020057d0
	mov r3, r10
	lsls r5, r3, #12
	adds r0, r5, #0
	bl Math_Cosine
	mov r4, r8
	str r4, [r7, #72]
	str r0, [r7, #68]
	adds r0, r5, #0
	bl Math_Sine
	ldr r3, [r7, #68]
	str r0, [r7, #76]
	asrs r2, r3, #1
	adds r3, r3, r2
	str r3, [r7, #68]
	bl Random16Far
	lsls r3, r0, #1
	ldr r2, [r7, #68]
	adds r3, r3, r0
	lsls r3, r3, #14
	lsrs r3, r3, #16
	adds r2, r2, r3
	ldr r3, .L_0200ccd0
	adds r2, r2, r3
	str r2, [r7, #68]
	bl Random16Far
	lsls r3, r0, #1
	ldr r2, [r7, #76]
	adds r3, r3, r0
	ldr r4, .L_0200ccd4
	lsls r3, r3, #13
	lsrs r3, r3, #16
	adds r2, r2, r3
	adds r2, r2, r4
	str r2, [r7, #76]
	bl Random16Far
	ldr r2, .L_0200ccd8
	lsls r0, r0, #12
	lsrs r0, r0, #16
	adds r3, r7, #0
	adds r0, r0, r2
	adds r3, #100
	strh r0, [r3]
	mov r3, r8
	str r3, [r7, #48]
	str r3, [r7, #52]
	ldr r3, .L_0200ccdc
	ldr r0, [r7, #80]
	str r3, [r7, #108]
	ldr r3, [r6, #80]
	movs r1, #12
	ldrb r3, [r3, #9]
	movs r4, #13
	ands r1, r3
	ldrb r3, [r0, #9]
	negs r4, r4
	adds r2, r4, #0
	ands r3, r2
	orrs r3, r1
	strb r3, [r0, #9]
.L_0200ccb2:
	movs r2, #1
	add r10, r2
	mov r3, r10
	cmp r3, #16
	bls .L_0200cbec
.L_0200ccbc:
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200ccc8:
	.4byte gPartyState
.L_0200cccc:
	.4byte Data_02006284
.L_0200ccd0:
	.4byte 0xffffa000
.L_0200ccd4:
	.4byte 0xffffd000
.L_0200ccd8:
	.4byte 0xfffff800
.L_0200ccdc:
	.4byte Func_02004a14
	.section .text.x0200cce0,"ax",%progbits
	.balign 4
	.global Func_02004ce0
	.thumb_func
Func_02004ce0:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	mov r10, r0
	adds r0, r2, #0
	adds r5, r1, #0
	bl Object_GetById
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #32]
	lsls r3, r5, #3
	subs r3, r3, r5
	movs r1, #156
	lsls r1, r1, #1
	lsls r3, r3, #3
	adds r3, r3, r1
	ldr r3, [r2, r3]
	ldr r2, .L_0200cd60
	adds r7, r0, #0
	ldr r1, .L_0200cd64
	adds r3, r3, r2
	adds r5, r7, #0
	asrs r3, r3, #2
	adds r5, #34
	adds r6, r3, r1
	ldr r2, [r7, #16]
	ldr r1, [r7, #8]
	ldrb r0, [r5]
	bl Func_02005848
	ldr r2, [r7, #16]
	mov r8, r0
	ldr r1, [r7, #8]
	ldrb r0, [r5]
	bl Map_GetTerrainHeight
	ldr r3, [r7, #8]
	asrs r2, r0, #19
	add r2, r10
	cmp r3, #0
	bge .L_0200cd3a
	ldr r1, .L_0200cd68
	adds r3, r3, r1
.L_0200cd3a:
	ldr r0, [r7, #16]
	asrs r1, r3, #20
	cmp r0, #0
	bge .L_0200cd46
	ldr r3, .L_0200cd68
	adds r0, r0, r3
.L_0200cd46:
	asrs r3, r0, #20
	lsls r3, r3, #7
	adds r3, r1, r3
	ldrb r0, [r5]
	mov r1, r8
	adds r6, r6, r3
	bl Func_02005a20
	strb r0, [r6]
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_0200cd60:
	.4byte 0xfdff0000
.L_0200cd64:
	.4byte gMapShapeGrid
.L_0200cd68:
	.4byte 0x000fffff
	.section .text.x0200cddc,"ax",%progbits
	.balign 4
	.global Func_02004ddc
	.thumb_func
Func_02004ddc:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r0, .L_0200d0f0
	adds r3, #236
	ldr r3, [r3]
	mov r11, r0
	movs r1, #4
	ldrsh r0, [r0, r1]
	mov r8, r3
	sub sp, #8
	bl Object_GetById
	ldr r5, .L_0200d0f4
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r5, r2
	adds r7, r0, #0
	ldr r0, [r3]
	bl Object_GetById
	cmp r7, #0
	bne .L_0200ce18
	b .L_0200d190
.L_0200ce18:
	mov r1, r11
	movs r3, #0
	ldrsh r2, [r1, r3]
	cmp r2, #4
	bne .L_0200ce9e
	ldr r4, [r0, #16]
	movs r2, #255
	ldr r6, [r0, #8]
	lsls r2, r2, #24
	movs r0, #152
	adds r3, r4, r2
	lsls r0, r0, #17
	cmp r3, r0
	bls .L_0200ce3e
	ldr r3, .L_0200d0f8
	movs r2, #132
	ands r3, r6
	lsls r2, r2, #18
	subs r6, r2, r3
.L_0200ce3e:
	movs r1, #241
	lsls r1, r1, #1
	adds r3, r5, r1
	ldrh r2, [r3]
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	beq .L_0200ce56
	movs r2, #152
	lsls r2, r2, #17
	adds r6, r6, r2
	b .L_0200ce5a
.L_0200ce56:
	ldr r3, .L_0200d0fc
	adds r6, r6, r3
.L_0200ce5a:
	ldr r1, [r7, #8]
	cmp r1, r6
	bne .L_0200ce68
	ldr r3, [r7, #16]
	cmp r3, r4
	bne .L_0200ce68
	b .L_0200d0d2
.L_0200ce68:
	ldr r3, [r7, #16]
	subs r1, r6, r1
	subs r0, r4, r3
	str r4, [sp, #0]
	bl ArcTan2
	ldrh r3, [r7, #6]
	lsls r0, r0, #16
	lsrs r0, r0, #16
	subs r0, r0, r3
	lsls r0, r0, #16
	movs r2, #128
	asrs r0, r0, #16
	lsls r2, r2, #5
	ldr r4, [sp, #0]
	cmp r0, r2
	ble .L_0200ce8c
	adds r0, r2, #0
.L_0200ce8c:
	ldr r2, .L_0200d100
	cmp r0, r2
	bge .L_0200ce94
	adds r0, r2, #0
.L_0200ce94:
	adds r3, r3, r0
	strh r3, [r7, #6]
	str r6, [r7, #8]
	str r4, [r7, #16]
	b .L_0200d0d2
.L_0200ce9e:
	cmp r2, #3
	beq .L_0200cea4
	b .L_0200cfdc
.L_0200cea4:
	movs r0, #190
	lsls r0, r0, #2
	bl GameFlag_GetByte
	movs r1, #1
	eors r0, r1
	lsls r3, r0, #1
	ldr r2, .L_0200d104
	adds r3, r3, r0
	lsls r3, r3, #3
	adds r5, r3, r2
	ldr r3, .L_0200d108
	ldrh r3, [r3]
	asrs r3, r0
	ands r3, r1
	cmp r3, #0
	bne .L_0200cec8
	b .L_0200d190
.L_0200cec8:
	movs r2, #0
	ldrsh r6, [r5, r2]
	adds r5, #2
	movs r3, #0
	ldrsh r4, [r5, r3]
	adds r5, #2
	movs r1, #0
	ldrsh r0, [r5, r1]
	adds r5, #2
	ldrh r3, [r5]
	mov r9, r0
	lsls r3, r3, #16
	asrs r2, r3, #24
	str r2, [sp, #4]
	movs r2, #255
	lsls r2, r2, #16
	movs r0, #137
	ands r2, r3
	lsls r0, r0, #1
	asrs r2, r2, #16
	adds r0, #255
	str r4, [sp, #0]
	mov r10, r2
	bl GameFlag_Test
	adds r5, #2
	ldr r4, [sp, #0]
	cmp r0, #0
	beq .L_0200cf7e
	ldr r3, [sp, #4]
	cmp r3, #0
	beq .L_0200cf7e
	movs r3, #9
	mov r0, r11
	strh r3, [r0]
	movs r1, #1
	adds r0, r7, #0
	bl Func_020057c8
	mov r3, r8
	adds r3, #236
	ldr r2, [r3]
	ldr r3, [r7, #8]
	movs r1, #192
	lsls r1, r1, #12
	adds r6, r2, r1
	cmp r2, r3
	blt .L_0200cf2c
	ldr r3, .L_0200d10c
	adds r6, r2, r3
.L_0200cf2c:
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #18
	bl GameFlag_Test
	mov r3, r8
	adds r2, r7, #0
	adds r3, #240
	adds r2, #100
	cmp r0, #0
	bne .L_0200cf50
	ldr r3, [r3]
	movs r0, #128
	lsls r0, r0, #13
	adds r4, r3, r0
	mov r3, r8
	adds r3, #232
	b .L_0200cf5a
.L_0200cf50:
	ldr r3, [r3]
	ldr r1, .L_0200d110
	adds r4, r3, r1
	mov r3, r8
	adds r3, #230
.L_0200cf5a:
	ldrh r3, [r3]
	strh r3, [r2]
	movs r3, #128
	lsls r3, r3, #7
	str r3, [r7, #52]
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r7, #48]
	adds r0, r7, #0
	adds r1, r6, #0
	movs r2, #0
	adds r3, r4, #0
	bl Func_020057f8
	adds r0, r7, #0
	bl Func_020057f0
	b .L_0200cf92
.L_0200cf7e:
	lsls r6, r6, #16
	lsls r4, r4, #16
	mov r2, r9
	strh r2, [r7, #6]
	str r6, [r7, #8]
	str r4, [r7, #16]
	adds r0, r7, #0
	mov r1, r10
	bl Func_020057c8
.L_0200cf92:
	movs r3, #0
	ldrsh r0, [r5, r3]
	adds r5, #2
	movs r1, #0
	ldrsh r6, [r5, r1]
	adds r5, #2
	movs r2, #0
	ldrsh r4, [r5, r2]
	adds r5, #2
	movs r1, #0
	ldrsh r3, [r5, r1]
	mov r9, r3
	movs r3, #2
	ldrsh r2, [r5, r3]
	mov r10, r2
	cmp r0, #0
	bne .L_0200cfb6
	b .L_0200d190
.L_0200cfb6:
	str r4, [sp, #0]
	bl Object_GetById
	adds r7, r0, #0
	ldr r4, [sp, #0]
	cmp r7, #0
	bne .L_0200cfc6
	b .L_0200d190
.L_0200cfc6:
	lsls r3, r6, #16
	mov r0, r9
	str r3, [r7, #8]
	lsls r3, r4, #16
	strh r0, [r7, #6]
	str r3, [r7, #16]
	adds r0, r7, #0
	mov r1, r10
	bl Func_020057c8
	b .L_0200d190
.L_0200cfdc:
	cmp r2, #1
	beq .L_0200cfe2
	b .L_0200d114
.L_0200cfe2:
	mov r5, r8
	adds r5, #234
	mov r2, r11
	movs r1, #10
	ldrsh r0, [r2, r1]
	movs r1, #0
	ldrsh r3, [r5, r1]
	cmp r0, r3
	beq .L_0200d006
	bl Resource_GetTableEntry
	mov r1, r8
	adds r1, #244
	bl Func_02005770
	mov r2, r11
	ldrh r3, [r2, #10]
	strh r3, [r5]
.L_0200d006:
	mov r1, r11
	movs r0, #6
	ldrsh r3, [r1, r0]
	mov r0, r8
	lsls r3, r3, #1
	adds r2, r3, #0
	adds r2, #244
	adds r3, #246
	ldrsh r6, [r0, r2]
	ldrsh r4, [r0, r3]
	movs r0, #130
	lsls r0, r0, #1
	str r4, [sp, #0]
	bl GameFlag_Test
	ldr r4, [sp, #0]
	cmp r0, #0
	bne .L_0200d034
	mov r0, r11
	ldrh r3, [r0, #6]
	mov r1, r11
	adds r3, #2
	strh r3, [r1, #6]
.L_0200d034:
	cmp r6, #0
	bne .L_0200d040
	cmp r4, #0
	bne .L_0200d040
	movs r3, #9
	b .L_0200d186
.L_0200d040:
	mov r1, r11
	movs r0, #2
	ldrsh r3, [r1, r0]
	lsls r6, r6, #16
	lsls r4, r4, #16
	cmp r3, #0
	beq .L_0200d062
	movs r2, #255
	lsls r2, r2, #24
	movs r0, #152
	adds r3, r4, r2
	lsls r0, r0, #17
	cmp r3, r0
	bls .L_0200d062
	movs r3, #132
	lsls r3, r3, #18
	subs r6, r3, r6
.L_0200d062:
	ldr r3, .L_0200d0f4
	movs r1, #241
	lsls r1, r1, #1
	adds r3, r3, r1
	ldrh r2, [r3]
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	beq .L_0200d07c
	movs r2, #152
	lsls r2, r2, #17
	adds r6, r6, r2
	b .L_0200d080
.L_0200d07c:
	ldr r3, .L_0200d0fc
	adds r6, r6, r3
.L_0200d080:
	ldr r1, [r7, #8]
	cmp r1, r6
	bne .L_0200d08c
	ldr r3, [r7, #16]
	cmp r3, r4
	beq .L_0200d0c8
.L_0200d08c:
	ldr r3, [r7, #16]
	subs r1, r6, r1
	subs r0, r4, r3
	str r4, [sp, #0]
	bl ArcTan2
	ldrh r3, [r7, #6]
	lsls r0, r0, #16
	lsrs r0, r0, #16
	subs r0, r0, r3
	lsls r0, r0, #16
	movs r2, #128
	asrs r0, r0, #16
	lsls r2, r2, #5
	ldr r4, [sp, #0]
	cmp r0, r2
	ble .L_0200d0b0
	adds r0, r2, #0
.L_0200d0b0:
	ldr r2, .L_0200d100
	cmp r0, r2
	bge .L_0200d0b8
	adds r0, r2, #0
.L_0200d0b8:
	adds r3, r3, r0
	movs r2, #0
	mov r0, r11
	strh r3, [r7, #6]
	str r6, [r7, #8]
	str r4, [r7, #16]
	strh r2, [r0, #8]
	b .L_0200d0d2
.L_0200d0c8:
	mov r1, r11
	ldrh r3, [r1, #8]
	mov r2, r11
	adds r3, #1
	strh r3, [r2, #8]
.L_0200d0d2:
	mov r1, r11
	movs r0, #8
	ldrsh r3, [r1, r0]
	cmp r3, #2
	ble .L_0200d0e6
	adds r0, r7, #0
	movs r1, #1
	bl Func_020057c8
	b .L_0200d190
.L_0200d0e6:
	adds r0, r7, #0
	movs r1, #5
	bl Func_020057c8
	b .L_0200d190
.L_0200d0f0:
	.4byte gSceneState
.L_0200d0f4:
	.4byte gPartyState
.L_0200d0f8:
	.4byte 0xffff0000
.L_0200d0fc:
	.4byte 0xfed00000
.L_0200d100:
	.4byte 0xfffff000
.L_0200d104:
	.4byte gLinkPeerSignatures
.L_0200d108:
	.4byte gLinkStatus
.L_0200d10c:
	.4byte 0xfff40000
.L_0200d110:
	.4byte 0xfff00000
.L_0200d114:
	cmp r2, #2
	bne .L_0200d190
	mov r1, r11
	movs r3, #18
	ldrsh r4, [r7, r3]
	movs r0, #6
	ldrsh r3, [r1, r0]
	movs r2, #10
	ldrsh r6, [r7, r2]
	lsls r3, r3, #1
	adds r2, r3, #0
	mov r0, r8
	adds r2, #244
	strh r6, [r0, r2]
	adds r3, #246
	mov r1, r8
	movs r0, #130
	strh r4, [r1, r3]
	lsls r0, r0, #1
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200d14c
	mov r2, r11
	ldrh r3, [r2, #6]
	mov r0, r11
	adds r3, #2
	strh r3, [r0, #6]
.L_0200d14c:
	mov r1, r11
	ldrh r3, [r1, #6]
	movs r0, #6
	ldrsh r2, [r1, r0]
	movs r1, #224
	lsls r1, r1, #5
	adds r1, #30
	cmp r2, r1
	bne .L_0200d190
	ldr r1, .L_0200d18c
	adds r3, #1
	lsls r2, r2, #1
	lsls r3, r3, #16
	adds r2, #244
	mov r0, r8
	asrs r3, r3, #15
	strh r1, [r0, r2]
	adds r3, #244
	mov r2, r8
	strh r1, [r2, r3]
	mov r3, r8
	adds r3, #228
	ldrh r3, [r3]
	movs r2, #0
	mov r0, r11
	mov r1, r11
	strh r3, [r0, #4]
	strh r2, [r1, #6]
	movs r3, #1
.L_0200d186:
	mov r2, r11
	strh r3, [r2]
	b .L_0200d190
.L_0200d18c:
	.4byte 0x00000000
.L_0200d190:
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x0200d1a0,"ax",%progbits
	.balign 4
	.global Func_020051a0
	.thumb_func
Func_020051a0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r1, #192
	lsls r1, r1, #18
	adds r3, r1, #0
	adds r3, #236
	ldr r3, [r3]
	sub sp, #40
	str r3, [sp, #36]
	str r3, [sp, #32]
	adds r7, r3, #0
	adds r7, #216
	adds r0, r3, #0
	movs r4, #0
	ldrsh r3, [r7, r4]
	ldr r2, .L_0200d508
	lsls r3, r3, #2
	adds r3, r3, r2
	ldrh r3, [r3, #2]
	mov r11, r0
	lsrs r3, r3, #5
	str r3, [sp, #20]
	adds r3, r0, #0
	adds r3, #224
	movs r0, #0
	ldrsh r3, [r3, r0]
	mov r5, r11
	ldr r6, [r1, #108]
	adds r5, #218
	cmp r3, #0
	beq .L_0200d1ee
	movs r3, #2
	strh r3, [r5]
	b .L_0200d26a
.L_0200d1ee:
	movs r0, #131
	lsls r0, r0, #1
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200d208
	movs r1, #179
	lsls r1, r1, #1
	adds r3, r6, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	beq .L_0200d218
.L_0200d208:
	movs r4, #0
	ldrsh r3, [r5, r4]
	ldrh r2, [r5]
	cmp r3, #0
	ble .L_0200d26a
	subs r3, r2, #1
	strh r3, [r5]
	b .L_0200d26a
.L_0200d218:
	movs r0, #0
	ldrsh r3, [r5, r0]
	ldrh r2, [r5]
	cmp r3, #1
	bgt .L_0200d26a
	adds r3, r2, #1
	movs r1, #128
	strh r3, [r5]
	lsls r1, r1, #9
	lsls r3, r3, #16
	cmp r3, r1
	bne .L_0200d26a
	movs r3, #128
	movs r2, #128
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	ldr r0, .L_0200d50c
	ldr r1, .L_0200d510
	adds r2, #16
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r0, #144
	lsls r0, r0, #2
	bl Runtime_BumpAllocateAlternatePool
	adds r5, r0, #0
	adds r1, r5, #0
	ldr r0, .L_0200d514
	bl Func_02005770
	movs r1, #144
	movs r2, #0
	ldrsh r0, [r7, r2]
	lsls r1, r1, #2
	adds r2, r5, #0
	bl VramBlock_LoadCached
	adds r0, r5, #0
	bl MemFree
.L_0200d26a:
	ldr r3, [sp, #36]
	adds r3, #218
	movs r4, #0
	ldrsh r3, [r3, r4]
	cmp r3, #0
	bne .L_0200d284
	ldr r3, [sp, #36]
	adds r3, #216
	movs r7, #0
	ldrsh r0, [r3, r7]
	bl Resource_ActivateEntry
	b .L_0200d5b2
.L_0200d284:
	movs r0, #0
	str r0, [sp, #28]
.L_0200d288:
	mov r1, r11
	adds r1, #222
	str r1, [sp, #12]
	mov r4, r11
	movs r2, #0
	ldrsh r3, [r1, r2]
	adds r4, #220
	adds r7, r3, #0
	adds r7, #8
	movs r3, #255
	ands r7, r3
	ldr r3, [sp, #28]
	subs r1, #4
	cmp r3, #0
	bne .L_0200d2cc
	movs r0, #0
	ldrsh r2, [r1, r0]
	lsls r3, r2, #1
	adds r3, r3, r2
	movs r1, #0
	ldrsh r2, [r4, r1]
	lsls r3, r3, #1
	adds r3, r3, r2
	subs r3, #12
	mov r9, r3
	movs r3, #128
	lsls r3, r3, #1
	adds r3, #255
	mov r2, r9
	ands r2, r3
	movs r3, #0
	mov r9, r2
	str r3, [sp, #16]
	b .L_0200d2f4
.L_0200d2cc:
	movs r0, #0
	ldrsh r2, [r1, r0]
	lsls r3, r2, #1
	adds r3, r3, r2
	movs r1, #0
	ldrsh r2, [r4, r1]
	lsls r3, r3, #1
	negs r3, r3
	subs r3, r3, r2
	adds r3, #236
	mov r9, r3
	movs r3, #128
	lsls r3, r3, #1
	adds r3, #255
	mov r2, r9
	ands r2, r3
	movs r3, #128
	lsls r3, r3, #21
	str r3, [sp, #16]
	mov r9, r2
.L_0200d2f4:
	movs r4, #0
	str r4, [sp, #24]
	cmp r7, #159
	bgt .L_0200d37a
	ldr r4, [sp, #36]
.L_0200d2fe:
	movs r0, #0
	str r0, [r4]
	ldr r2, [sp, #16]
	mov r1, r9
	lsls r6, r1, #16
	adds r3, r7, #0
	orrs r3, r6
	orrs r3, r2
	ldr r2, .L_0200d518
	movs r5, #228
	orrs r3, r2
	str r3, [r4, #4]
	ldr r3, [sp, #20]
	lsls r5, r5, #8
	orrs r3, r5
	str r3, [r4, #8]
	ldr r3, [sp, #32]
	mov r8, r0
	adds r3, #12
	ldr r0, [sp, #32]
	movs r1, #255
	mov r10, r3
	str r4, [sp, #4]
	bl Func_02005790
	ldr r4, [sp, #4]
	mov r0, r8
	str r0, [r4, #12]
	ldr r1, [sp, #16]
	adds r3, r7, #0
	adds r3, #32
	orrs r3, r6
	movs r2, #128
	orrs r3, r1
	lsls r2, r2, #7
	orrs r3, r2
	str r3, [r4, #16]
	ldr r3, [sp, #20]
	mov r0, r10
	adds r3, #8
	orrs r3, r5
	str r3, [r4, #20]
	ldr r2, [sp, #36]
	ldr r3, [sp, #32]
	adds r4, #24
	adds r2, #24
	adds r3, #24
	movs r1, #255
	str r4, [sp, #4]
	str r2, [sp, #36]
	str r3, [sp, #32]
	bl Func_02005790
	ldr r0, [sp, #24]
	adds r7, #36
	adds r0, #1
	str r0, [sp, #24]
	ldr r4, [sp, #4]
	cmp r0, #3
	bhi .L_0200d37a
	cmp r7, #159
	ble .L_0200d2fe
.L_0200d37a:
	ldr r1, [sp, #28]
	adds r1, #1
	str r1, [sp, #28]
	cmp r1, #1
	bls .L_0200d288
	ldr r3, .L_0200d51c
	movs r2, #15
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #4
	bhi .L_0200d392
	b .L_0200d5b2
.L_0200d392:
	mov r3, r11
	adds r3, #226
	movs r2, #0
	ldrsh r0, [r3, r2]
	bl ObjectTable_Get
	adds r5, r0, #0
	cmp r5, #0
	beq .L_0200d492
	ldr r3, .L_0200d520
	ldr r7, [sp, #36]
	movs r4, #241
	ldr r0, [sp, #32]
	lsls r4, r4, #1
	adds r3, r3, r4
	adds r7, #4
	ldrh r3, [r3]
	adds r0, #12
	mov r10, r7
	ldr r7, [sp, #20]
	str r0, [sp, #8]
	movs r2, #1
	ands r2, r3
	mov r1, r11
	mov r4, r11
	mov r3, r11
	mov r6, r11
	adds r7, #10
	adds r1, #218
	adds r4, #220
	adds r3, #236
	ldr r0, [r5, #8]
	adds r6, #240
	mov r8, r7
	cmp r2, #0
	beq .L_0200d400
	ldr r3, [r3]
	subs r2, r0, r3
	cmp r2, #0
	bge .L_0200d3e6
	ldr r0, .L_0200d524
	adds r2, r2, r0
.L_0200d3e6:
	movs r3, #0
	ldrsh r1, [r1, r3]
	asrs r2, r2, #20
	lsls r3, r1, #1
	adds r3, r3, r1
	lsls r3, r3, #1
	adds r2, r2, r3
	movs r7, #0
	ldrsh r3, [r4, r7]
	movs r0, #0
	adds r2, r2, r3
	subs r2, #19
	b .L_0200d424
.L_0200d400:
	ldr r3, [r3]
	subs r2, r0, r3
	cmp r2, #0
	bge .L_0200d40c
	ldr r0, .L_0200d524
	adds r2, r2, r0
.L_0200d40c:
	movs r3, #0
	ldrsh r1, [r1, r3]
	asrs r2, r2, #20
	lsls r3, r1, #1
	adds r3, r3, r1
	lsls r3, r3, #1
	subs r2, r2, r3
	movs r7, #0
	ldrsh r3, [r4, r7]
	movs r0, #0
	subs r2, r2, r3
	adds r2, #232
.L_0200d424:
	mov r9, r2
	bl Func_02002ce8
	ldr r2, [r5, #16]
	ldr r3, [r6]
	adds r1, r0, #0
	subs r0, r2, r3
	cmp r0, #0
	bge .L_0200d43a
	ldr r2, .L_0200d524
	adds r0, r0, r2
.L_0200d43a:
	ldr r7, [sp, #12]
	asrs r2, r0, #20
	movs r4, #0
	ldrsh r3, [r7, r4]
	adds r2, r2, r3
	lsls r3, r1, #3
	adds r3, r3, r1
	lsls r3, r3, #2
	subs r2, r2, r3
	adds r7, r2, #4
	movs r3, #128
	lsls r3, r3, #1
	ldr r1, [sp, #36]
	adds r3, #255
	mov r0, r9
	ands r0, r3
	movs r3, #255
	ands r7, r3
	movs r3, #0
	str r3, [r1]
	lsls r3, r0, #16
	orrs r7, r3
	movs r3, #128
	mov r2, r10
	lsls r3, r3, #23
	adds r0, r2, #0
	orrs r7, r3
	stmia r0!, {r7}
	movs r3, #228
	lsls r3, r3, #8
	mov r1, r8
	adds r4, r0, #0
	orrs r1, r3
	str r4, [sp, #36]
	stmia r0!, {r1}
	ldr r3, [sp, #32]
	ldr r4, [sp, #8]
	adds r2, r0, #0
	movs r1, #255
	adds r0, r3, #0
	str r2, [sp, #36]
	str r4, [sp, #32]
	bl Func_02005790
.L_0200d492:
	mov r3, r11
	adds r3, #228
	movs r7, #0
	ldrsh r0, [r3, r7]
	bl ObjectTable_Get
	adds r5, r0, #0
	cmp r5, #0
	bne .L_0200d4a6
	b .L_0200d5b2
.L_0200d4a6:
	ldr r0, [r5, #8]
	cmp r0, #0
	bne .L_0200d4ae
	b .L_0200d5b2
.L_0200d4ae:
	ldr r3, .L_0200d520
	movs r1, #241
	ldr r7, [sp, #36]
	lsls r1, r1, #1
	adds r3, r3, r1
	ldrh r3, [r3]
	adds r7, #4
	mov r10, r7
	ldr r7, [sp, #20]
	movs r2, #1
	ands r2, r3
	mov r3, r11
	adds r3, #222
	mov r1, r11
	mov r4, r11
	mov r6, r11
	adds r7, #14
	str r3, [sp, #0]
	adds r1, #218
	adds r4, #220
	adds r3, #14
	adds r6, #240
	mov r8, r7
	cmp r2, #0
	beq .L_0200d528
	ldr r3, [r3]
	subs r2, r0, r3
	cmp r2, #0
	bge .L_0200d4ec
	ldr r0, .L_0200d524
	adds r2, r2, r0
.L_0200d4ec:
	movs r3, #0
	ldrsh r1, [r1, r3]
	asrs r2, r2, #20
	lsls r3, r1, #1
	adds r3, r3, r1
	lsls r3, r3, #1
	adds r2, r2, r3
	movs r7, #0
	ldrsh r3, [r4, r7]
	movs r0, #1
	adds r2, r2, r3
	adds r2, #189
	b .L_0200d54c
	.2byte 0x0000
.L_0200d508:
	.4byte ResourceTableEntries
.L_0200d50c:
	.4byte Data_020062b4
.L_0200d510:
	.4byte 0x050003c0
.L_0200d514:
	.4byte Data_020062d4
.L_0200d518:
	.4byte 0x80008000
.L_0200d51c:
	.4byte gFrameCount
.L_0200d520:
	.4byte gPartyState
.L_0200d524:
	.4byte 0x000fffff
.L_0200d528:
	ldr r3, [r3]
	subs r2, r0, r3
	cmp r2, #0
	bge .L_0200d534
	ldr r0, .L_0200d5c0
	adds r2, r2, r0
.L_0200d534:
	movs r3, #0
	ldrsh r1, [r1, r3]
	asrs r2, r2, #20
	lsls r3, r1, #1
	adds r3, r3, r1
	lsls r3, r3, #1
	subs r2, r2, r3
	movs r7, #0
	ldrsh r3, [r4, r7]
	movs r0, #1
	adds r2, r2, r3
	adds r2, #23
.L_0200d54c:
	mov r9, r2
	bl Func_02002ce8
	ldr r2, [r5, #16]
	ldr r3, [r6]
	adds r1, r0, #0
	subs r0, r2, r3
	cmp r0, #0
	bge .L_0200d562
	ldr r2, .L_0200d5c0
	adds r0, r0, r2
.L_0200d562:
	ldr r7, [sp, #0]
	asrs r2, r0, #20
	movs r4, #0
	ldrsh r3, [r7, r4]
	adds r2, r2, r3
	lsls r3, r1, #3
	adds r3, r3, r1
	lsls r3, r3, #2
	subs r2, r2, r3
	adds r7, r2, #4
	movs r3, #128
	lsls r3, r3, #1
	ldr r1, [sp, #36]
	adds r3, #255
	mov r0, r9
	ands r0, r3
	movs r3, #255
	ands r7, r3
	movs r3, #0
	str r3, [r1]
	lsls r3, r0, #16
	orrs r7, r3
	movs r3, #128
	lsls r3, r3, #23
	mov r2, r10
	adds r0, r2, #0
	orrs r7, r3
	stmia r0!, {r7}
	movs r3, #228
	lsls r3, r3, #8
	mov r1, r8
	orrs r1, r3
	adds r4, r0, #0
	str r4, [sp, #36]
	str r1, [r0]
	ldr r3, [sp, #32]
	movs r1, #255
	adds r0, r3, #0
	bl Func_02005790
.L_0200d5b2:
	add sp, #40
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0200d5c0:
	.4byte 0x000fffff
	.section .text.x0200d5c4,"ax",%progbits
	.balign 4
	.global Func_020055c4
	.thumb_func
Func_020055c4:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	mov r9, r1
	movs r1, #228
	lsls r1, r1, #6
	adds r1, #52
	adds r6, r0, #0
	movs r0, #236
	bl Runtime_AllocateBlock
	adds r5, r0, #0
	movs r0, #144
	lsls r0, r0, #2
	bl Runtime_BumpAllocateAlternatePool
	adds r3, r5, #0
	adds r3, #226
	strh r6, [r3]
	adds r7, r0, #0
	adds r3, #2
	mov r0, r9
	strh r0, [r3]
	ldr r3, .L_0200d678
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	movs r0, #218
	adds r0, r0, r5
	ldr r3, [r3]
	mov r8, r0
	movs r0, #224
	adds r2, r5, #0
	adds r1, r5, #0
	adds r0, r0, r5
	adds r2, #236
	adds r1, #240
	mov r10, r0
	adds r5, #216
	cmp r6, r3
	bne .L_0200d620
	movs r3, #160
	lsls r3, r3, #16
	b .L_0200d624
.L_0200d620:
	movs r3, #224
	lsls r3, r3, #15
.L_0200d624:
	str r3, [r2]
	ldr r3, .L_0200d67c
	str r3, [r1]
	adds r0, r6, #0
	bl Object_GetById
	mov r0, r9
	bl Object_GetById
	movs r3, #0
	mov r2, r8
	mov r0, r10
	strh r3, [r2]
	adds r1, r7, #0
	strh r3, [r0]
	ldr r0, .L_0200d680
	bl Func_02005770
	bl Resource_FindFreeEntry
	movs r1, #144
	strh r0, [r5]
	lsls r0, r0, #16
	adds r2, r7, #0
	lsls r1, r1, #2
	asrs r0, r0, #16
	bl VramBlock_LoadCached
	movs r1, #128
	lsls r1, r1, #3
	adds r1, #118
	ldr r0, .L_0200d684
	bl Scheduler_AddOrUpdateCallback
	adds r0, r7, #0
	bl MemFree
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_0200d678:
	.4byte gPartyState
.L_0200d67c:
	.4byte 0xf9a00000
.L_0200d680:
	.4byte Data_020062d4
.L_0200d684:
	.4byte Func_020051a0
	.section .bss,"aw",%nobits
	.global Data_02007e28
Data_02007e28:
