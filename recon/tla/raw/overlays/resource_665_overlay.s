.syntax unified
	.thumb
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
	.4byte MsgScene665No65
.L_02008474:
	.4byte MsgScene665No71
.L_02008478:
	.4byte MsgScene665No70
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
	.4byte MsgScene665No92
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
	.4byte MsgScene665No10
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
	.4byte MsgScene665No28
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
	.2byte MsgScene665No180
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
	.4byte MsgScene665No29
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte MsgScene665No30
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte MsgScene665No31
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte MsgScene665No32
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte MsgScene665No01
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte MsgScene665No02
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte Func_0200013c
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte MsgScene665No05
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte MsgScene665No06
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte MsgScene665No07
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte MsgScene665No08
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte MsgScene665No09
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte MsgScene665No11
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte MsgScene665No12
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte MsgScene665No13
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte MsgScene665No14
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte MsgScene665No15
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte Func_020001a4
	.4byte 0x00000000
	.4byte 0xffff0017
	.4byte MsgScene665No17
	.4byte 0x00000000
	.4byte 0xffff0018
	.4byte MsgScene665No18
	.4byte 0x00000000
	.4byte 0xffff0019
	.4byte MsgScene665No19
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte MsgScene665No20
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte MsgScene665No21
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte MsgScene665No22
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte MsgScene665No23
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte MsgScene665No24
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte MsgScene665No25
	.4byte 0x00008d15
	.4byte 0xffff0017
	.4byte MsgScene665No26
	.4byte 0x00008d15
	.4byte 0xffff0018
	.4byte MsgScene665No27
	.4byte 0x00008d15
	.4byte 0xffff0419
	.4byte Func_0200023c
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte Func_020001f0
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte MsgScene665No34
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte MsgScene665No35
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte MsgScene665No36
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte MsgScene665No37
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte MsgScene665No38
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
	.2byte MsgScene665No180
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
	.4byte MsgScene665No94
	.4byte 0x00008d15
	.4byte 0x18ff0012
	.4byte MsgScene665No95
	.4byte 0x00000000
	.4byte 0x18ff0011
	.4byte MsgScene665No96
	.4byte 0x00008d15
	.4byte 0x18ff0011
	.4byte MsgScene665No97
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte MsgScene665No82
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte MsgScene665No83
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte MsgScene665No84
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte MsgScene665No85
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte MsgScene665No39
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte MsgScene665No40
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte MsgScene665No41
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte MsgScene665No42
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte MsgScene665No43
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte MsgScene665No44
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte MsgScene665No45
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte MsgScene665No46
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte MsgScene665No47
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte MsgScene665No48
	.4byte 0x00000000
	.4byte 0xffff0017
	.4byte MsgScene665No49
	.4byte 0x00000000
	.4byte 0xffff0018
	.4byte Func_02000288
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte MsgScene665No51
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte MsgScene665No52
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte MsgScene665No53
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte MsgScene665No54
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte MsgScene665No55
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte MsgScene665No56
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte MsgScene665No57
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte MsgScene665No58
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte MsgScene665No59
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte MsgScene665No60
	.4byte 0x00008d15
	.4byte 0xffff0017
	.4byte MsgScene665No61
	.4byte 0x00008d15
	.4byte 0xffff0018
	.4byte MsgScene665No62
	.4byte 0x00000000
	.4byte 0xffff0019
	.4byte MsgScene665No63
	.4byte 0x00000000
	.4byte 0xffff001a
	.4byte MsgScene665No64
	.4byte 0x00000000
	.4byte 0xffff001b
	.4byte Func_02000398
	.4byte 0x00008d15
	.4byte 0xffff0019
	.4byte MsgScene665No66
	.4byte 0x00008d15
	.4byte 0xffff001a
	.4byte MsgScene665No67
	.4byte 0x00008d15
	.4byte 0x08b5001b
	.4byte MsgScene665No68
	.4byte 0x00008d15
	.4byte 0xffff001b
	.4byte MsgScene665No72
	.4byte 0x0001c914
	.4byte 0xffff001b
	.4byte Func_020002f8
	.4byte 0x00000000
	.4byte 0xffff001c
	.4byte MsgScene665No73
	.4byte 0x00000000
	.4byte 0xffff001d
	.4byte MsgScene665No75
	.4byte 0x00000000
	.4byte 0xffff001e
	.4byte MsgScene665No77
	.4byte 0x00000000
	.4byte 0xffff001f
	.4byte MsgScene665No76
	.4byte 0x00008d15
	.4byte 0xffff001c
	.4byte MsgScene665No78
	.4byte 0x00008d15
	.4byte 0xffff001d
	.4byte MsgScene665No79
	.4byte 0x00008d15
	.4byte 0xffff001e
	.4byte MsgScene665No81
	.4byte 0x00008d15
	.4byte 0xffff001f
	.4byte MsgScene665No80
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte MsgScene665No86
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte MsgScene665No87
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte MsgScene665No88
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte MsgScene665No89
	.4byte 0x00000000
	.4byte 0x08b6000c
	.4byte MsgScene665No90
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte Func_0200066c
	.4byte 0x00008d15
	.4byte 0x08b9000c
	.4byte MsgScene665No91
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte MsgScene665No93
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
	.2byte MsgScene665No180
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
	.4byte MsgScene665No170
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte MsgScene665No171
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte MsgScene665No172
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte MsgScene665No173
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte MsgScene665No132
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte MsgScene665No133
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte MsgScene665No134
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte MsgScene665No135
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte MsgScene665No136
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte MsgScene665No137
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte MsgScene665No138
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte MsgScene665No139
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte MsgScene665No140
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte MsgScene665No141
	.4byte 0x00000000
	.4byte 0xffff0017
	.4byte MsgScene665No142
	.4byte 0x00000000
	.4byte 0xffff0018
	.4byte MsgScene665No143
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte MsgScene665No144
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte MsgScene665No145
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte MsgScene665No146
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte MsgScene665No147
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte MsgScene665No148
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte MsgScene665No155
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte MsgScene665No149
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte MsgScene665No150
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte MsgScene665No151
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte MsgScene665No152
	.4byte 0x00008d15
	.4byte 0xffff0017
	.4byte MsgScene665No153
	.4byte 0x00008d15
	.4byte 0xffff0018
	.4byte MsgScene665No154
	.4byte 0x00000000
	.4byte 0xffff0019
	.4byte MsgScene665No156
	.4byte 0x00000000
	.4byte 0xffff001a
	.4byte MsgScene665No157
	.4byte 0x00000000
	.4byte 0xffff001b
	.4byte MsgScene665No158
	.4byte 0x00008d15
	.4byte 0xffff0019
	.4byte MsgScene665No159
	.4byte 0x00008d15
	.4byte 0xffff001a
	.4byte MsgScene665No160
	.4byte 0x00008d15
	.4byte 0xffff001b
	.4byte MsgScene665No161
	.4byte 0x00000000
	.4byte 0xffff001c
	.4byte MsgScene665No162
	.4byte 0x00000000
	.4byte 0xffff001d
	.4byte MsgScene665No163
	.4byte 0x00000000
	.4byte 0xffff001f
	.4byte MsgScene665No164
	.4byte 0x00000000
	.4byte 0xffff001e
	.4byte MsgScene665No165
	.4byte 0x00008d15
	.4byte 0xffff001c
	.4byte MsgScene665No166
	.4byte 0x00008d15
	.4byte 0xffff001d
	.4byte MsgScene665No167
	.4byte 0x00008d15
	.4byte 0xffff001f
	.4byte MsgScene665No168
	.4byte 0x00008d15
	.4byte 0xffff001e
	.4byte MsgScene665No169
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte MsgScene665No174
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte MsgScene665No175
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte MsgScene665No176
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte MsgScene665No177
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte MsgScene665No178
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte MsgScene665No179
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
	.2byte MsgScene665No180
	.2byte 0x0040
	.4byte 0x50009085
	.4byte 0xffff0000
	.4byte Func_02000760
	.4byte 0x40009085
	.4byte 0xffff0000
	.4byte Func_02000774
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte MsgScene665No98
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte MsgScene665No99
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte MsgScene665No100
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte MsgScene665No101
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte MsgScene665No102
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte MsgScene665No103
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte MsgScene665No104
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte MsgScene665No105
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte MsgScene665No106
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte MsgScene665No107
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte MsgScene665No108
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte MsgScene665No109
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte MsgScene665No110
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte MsgScene665No111
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte MsgScene665No112
	.4byte 0x00000000
	.4byte 0xffff0017
	.4byte MsgScene665No113
	.4byte 0x00000000
	.4byte 0xffff0018
	.4byte MsgScene665No114
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte MsgScene665No115
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte MsgScene665No116
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte MsgScene665No117
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte MsgScene665No118
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte MsgScene665No119
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte MsgScene665No120
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte MsgScene665No121
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte MsgScene665No122
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte MsgScene665No123
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte MsgScene665No124
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte MsgScene665No125
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte MsgScene665No126
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte MsgScene665No127
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte MsgScene665No128
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte MsgScene665No129
	.4byte 0x00008d15
	.4byte 0xffff0017
	.4byte MsgScene665No130
	.4byte 0x00008d15
	.4byte 0xffff0018
	.4byte MsgScene665No131
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
