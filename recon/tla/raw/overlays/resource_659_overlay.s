.syntax unified
	.thumb
	.section .text.x020080fc,"ax",%progbits
	.balign 4
	.global Func_020000fc
	.thumb_func
Func_020000fc:
	.ifdef TLA_EDITION_JA
	push {lr}
	ldr r3, .L_ja_02008148
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3, #0]
	bl Object_GetById
	ldrh r3, [r0, #6]
	movs r2, #128
	lsls r2, r2, #6
	adds r3, r3, r2
	ldr r2, .L_ja_02008144
	ands r3, r2
	movs r2, #192
	lsls r3, r3, #16
	lsls r2, r2, #24
	cmp r3, r2
	bne .L_ja_0200812c
	movs r0, #1
	movs r1, #22
	bl Func_02001f08
	b .L_ja_02008174
.L_ja_0200812c:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_02008150
	ldr r0, .L_ja_0200814c
	bl Func_02001e18
	b .L_ja_0200816c
	.2byte 0x0000
.L_ja_02008144:
	.4byte 0xffffc000
.L_ja_02008148:
	.4byte gPartyState
.L_ja_0200814c:
	.4byte 0x00002253
.L_ja_02008150:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #171
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_02008166
	ldr r0, .L_ja_02008178
	bl Func_02001e18
	b .L_ja_0200816c
.L_ja_02008166:
	ldr r0, .L_ja_0200817c
	bl Func_02001e18
.L_ja_0200816c:
	movs r0, #22
	movs r1, #0
	bl Func_02001e30
.L_ja_02008174:
	pop {pc}
	.2byte 0x0000
.L_ja_02008178:
	.4byte 0x00001e6c
.L_ja_0200817c:
	.4byte 0x00001a0f
	.else
	push {r5, lr}
	ldr r3, .L_02008130
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	ldrh r3, [r0, #6]
	movs r2, #128
	lsls r2, r2, #6
	adds r3, r3, r2
	ldr r2, .L_0200812c
	ands r3, r2
	movs r2, #192
	lsls r3, r3, #16
	lsls r2, r2, #24
	cmp r3, r2
	bne .L_02008134
	movs r0, #1
	movs r1, #22
	bl Func_02001f08
	b .L_020081a8
.L_0200812c:
	.4byte 0xffffc000
.L_02008130:
	.4byte gPartyState
.L_02008134:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008184
	ldr r5, .L_020081ac
	adds r0, r5, #0
	bl Func_02001e18
	movs r1, #0
	movs r0, #22
	bl UiText_OpenMessageAtObject
	bl Func_02001ee8
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_0200816e
	movs r0, #10
	bl Battle_WaitMode0
	adds r0, r5, #1
	bl Func_02001e18
	b .L_0200817a
.L_0200816e:
	movs r0, #20
	bl Battle_WaitMode0
	adds r0, r5, #2
	bl Func_02001e18
.L_0200817a:
	movs r0, #22
	movs r1, #0
	bl Func_02001e30
	b .L_020081a8
.L_02008184:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #171
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200819a
	ldr r0, .L_020081b0
	bl Func_02001e18
	b .L_020081a0
.L_0200819a:
	ldr r0, .L_020081b4
	bl Func_02001e18
.L_020081a0:
	movs r0, #22
	movs r1, #0
	bl Func_02001e30
.L_020081a8:
	pop {r5, pc}
	.2byte 0x0000
.L_020081ac:
	.4byte MsgScene659No155
.L_020081b0:
	.4byte MsgScene659No87
.L_020081b4:
	.4byte MsgScene659No39
	.endif
	.section .text.x02008570,"ax",%progbits
	.balign 4
	.global Func_02000570
	.thumb_func
Func_02000570:
	push {lr}
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #171
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020085b4
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #208
	lsls r2, r2, #4
	adds r2, #76
	movs r0, #144
	adds r3, r3, r2
	lsls r0, r0, #4
	movs r2, #3
	strh r2, [r3]
	adds r0, #17
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020085a6
	bl Func_020007dc
	b .L_02008614
.L_020085a6:
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_02001db8
	movs r0, #10
	b .L_020085e2
.L_020085b4:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020085ec
	movs r3, #224
	lsls r3, r3, #8
	movs r0, #9
	ldr r1, .L_02008618
	ldr r2, .L_0200861c
	bl Func_02001dc0
	movs r3, #192
	movs r1, #236
	movs r0, #10
	lsls r1, r1, #17
	ldr r2, .L_02008620
	lsls r3, r3, #8
	bl Func_02001dc0
	movs r0, #8
.L_020085e2:
	movs r1, #0
	movs r2, #0
	bl Func_02001db8
	b .L_02008614
.L_020085ec:
	movs r3, #160
	lsls r3, r3, #8
	movs r0, #9
	ldr r1, .L_02008618
	ldr r2, .L_0200861c
	bl Func_02001dc0
	movs r3, #192
	movs r1, #236
	movs r0, #10
	lsls r1, r1, #17
	ldr r2, .L_02008620
	lsls r3, r3, #8
	bl Func_02001dc0
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl Func_02001db8
.L_02008614:
	pop {pc}
	.2byte 0x0000
.L_02008618:
	.4byte gEwramHeap
.L_0200861c:
	.4byte 0x03490000
.L_02008620:
	.4byte 0x036d0000
	.section .text.x02008624,"ax",%progbits
	.global Func_02000624
	.thumb_func
Func_02000624:
	push {r5, r6, lr}
	movs r6, #192
	lsls r6, r6, #18
	ldr r3, [r6, #108]
	movs r1, #214
	movs r2, #133
	lsls r1, r1, #1
	lsls r2, r2, #1
	adds r3, r3, r1
	ldr r5, .L_0200879c
	adds r2, #255
	str r2, [r3]
	adds r2, #11
	adds r3, r5, r2
	ldr r0, [r3]
	sub sp, #8
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #32
	orrs r3, r2
	strb r3, [r0]
	movs r0, #0
	bl Func_02001e80
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r5, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_020087a0
	cmp r2, r3
	bne .L_020086e6
	ldr r3, [r6, #108]
	movs r2, #208
	lsls r2, r2, #4
	adds r2, #76
	movs r0, #128
	adds r3, r3, r2
	lsls r0, r0, #4
	movs r2, #3
	strh r2, [r3]
	adds r0, #33
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200869a
	movs r0, #29
	movs r1, #0
	movs r2, #0
	bl Func_02001db8
	movs r0, #30
	movs r1, #0
	movs r2, #0
	bl Func_02001db8
	b .L_020086b2
.L_0200869a:
	movs r0, #29
	bl Object_GetById
	movs r1, #3
	bl Object_SetPartAttribute
	movs r0, #30
	bl Object_GetById
	movs r1, #5
	bl Object_SetPartAttribute
.L_020086b2:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #171
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020086e6
	movs r3, #64
	movs r5, #16
	str r3, [sp, #4]
	movs r0, #2
	movs r1, #110
	movs r2, #16
	movs r3, #10
	str r5, [sp, #0]
	bl Func_02001d28
	movs r3, #0
	str r3, [sp, #4]
	movs r0, #2
	movs r1, #46
	movs r2, #16
	movs r3, #10
	str r5, [sp, #0]
	bl Func_02001d20
.L_020086e6:
	ldr r5, .L_0200879c
	movs r1, #241
	lsls r1, r1, #1
	adds r3, r5, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #20
	bne .L_020086fa
	bl Func_02000570
.L_020086fa:
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r5, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_020087a4
	cmp r2, r3
	bne .L_0200876a
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #171
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008724
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_02001db8
	b .L_0200876a
.L_02008724:
	movs r1, #192
	lsls r1, r1, #8
	movs r0, #10
	bl Func_02001ef0
	movs r0, #10
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	movs r0, #10
	bl Object_GetById
	ldr r1, [r0, #80]
	movs r3, #8
	ldrb r2, [r1, #26]
	movs r0, #10
	orrs r3, r2
	strb r3, [r1, #26]
	bl Object_GetById
	ldr r1, [r0, #80]
	movs r3, #3
	ldrb r2, [r1, #17]
	movs r0, #10
	ands r3, r2
	movs r2, #32
	orrs r3, r2
	strb r3, [r1, #17]
	bl Object_GetById
	ldr r2, [r0, #80]
	movs r3, #1
	strb r3, [r2, #25]
.L_0200876a:
	bl Func_02000534
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200878a
	movs r0, #33
	bl Object_GetById
	movs r1, #2
	bl Object_SetPartAttribute
	b .L_02008796
.L_0200878a:
	movs r0, #28
	bl Object_GetById
	movs r1, #2
	bl Object_SetPartAttribute
.L_02008796:
	movs r0, #0
	add sp, #8
	pop {r5, r6, pc}
.L_0200879c:
	.4byte gPartyState
.L_020087a0:
	.4byte 0x0000002d
.L_020087a4:
	.4byte 0x0000002e
	.section .text.x02009008,"ax",%progbits
	.balign 4
	.global Func_02001008
	.thumb_func
Func_02001008:
	push {lr}
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #138
	bl GameFlag_SetBit
	movs r0, #196
	lsls r0, r0, #2
	bl GameFlag_SetBit
	movs r0, #224
	lsls r0, r0, #1
	bl PartyInventory_Remove
	bl Func_02001d50
	movs r0, #0
	bl Func_02001eb8
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #7
	lsls r2, r2, #6
	movs r0, #11
	adds r1, #102
	adds r2, #51
	bl ObjectMotion_SetSpeedParameters
	movs r1, #205
	movs r2, #109
	movs r0, #11
	lsls r1, r1, #1
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r0, #4
	bl Object_GetById
	ldr r3, [r0, #16]
	movs r2, #190
	lsls r2, r2, #15
	cmp r3, r2
	ble .L_02009074
	ldr r0, [r0, #8]
	cmp r0, #0
	bge .L_0200906a
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	adds r0, r0, r3
.L_0200906a:
	asrs r1, r0, #16
	movs r2, #95
	movs r0, #4
	bl ObjectMotion_SetPositionAndReset
.L_02009074:
	movs r1, #181
	movs r2, #95
	movs r0, #4
	lsls r1, r1, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #128
	lsls r1, r1, #7
	movs r0, #4
	bl Func_02001e40
	movs r0, #11
	bl ObjectMotion_CommitCurrentPositionAndActivate
	ldr r0, .L_020092f4
	bl Func_02001e18
	movs r1, #4
	movs r2, #0
	movs r0, #10
	bl ObjectMotion_SetAngleToward
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #10
	bl Func_02001e48
	movs r0, #10
	movs r1, #0
	movs r2, #10
	bl Func_02001e28
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #3
	movs r0, #11
	bl Func_02001e48
	movs r1, #189
	movs r0, #11
	lsls r1, r1, #1
	movs r2, #109
	bl ObjectMotion_SetPositionAndReset
	movs r1, #160
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #11
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #11
	movs r1, #0
	movs r2, #10
	bl Func_02001e28
	movs r1, #11
	movs r2, #0
	movs r0, #10
	bl Object_LinkPair
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #4
	movs r2, #0
	movs r0, #10
	bl ObjectMotion_SetAngleToward
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #10
	bl Func_02001e48
	movs r2, #10
	movs r0, #10
	movs r1, #0
	bl Func_02001e28
	movs r1, #3
	movs r0, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #11
	bl Func_02001e48
	movs r0, #11
	movs r1, #0
	movs r2, #10
	bl Func_02001e28
	movs r2, #0
	movs r1, #11
	movs r0, #10
	bl ObjectMotion_SetAngleToward
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #10
	bl Motion_SetModeAndWaitAnimation
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #2
	adds r1, #255
	movs r2, #30
	movs r0, #10
	bl Func_02001e48
	movs r2, #10
	movs r0, #10
	movs r1, #0
	bl Func_02001e28
	movs r1, #2
	movs r0, #11
	bl Motion_SetVarCbAndRefresh
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #11
	movs r1, #0
	bl Func_02001e28
	movs r1, #128
	lsls r1, r1, #7
	movs r0, #11
	bl Func_02001e40
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #192
	lsls r1, r1, #8
	movs r0, #11
	bl Func_02001e40
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #8
	movs r0, #11
	bl Func_02001e40
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #131
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #11
	bl Func_02001e48
	movs r0, #25
	bl ObjectMotion_EnableActionAndResetMotion
	movs r1, #128
	movs r2, #128
	movs r0, #10
	lsls r1, r1, #8
	lsls r2, r2, #7
	bl ObjectMotion_SetSpeedParameters
	movs r1, #128
	movs r2, #128
	movs r0, #11
	lsls r1, r1, #8
	lsls r2, r2, #7
	bl ObjectMotion_SetSpeedParameters
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #7
	movs r0, #25
	lsls r1, r1, #8
	bl ObjectMotion_SetSpeedParameters
	ldr r1, .L_020092f8
	movs r0, #11
	bl ObjectMotion_EnableActionAndSetCallback
	ldr r1, .L_020092fc
	movs r0, #4
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #60
	bl Battle_WaitMode0
	ldr r1, .L_02009300
	movs r0, #10
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #10
	bl Object_RefreshSelectorById
	movs r0, #120
	bl Battle_WaitMode0
	ldr r1, .L_02009304
	movs r0, #11
	bl ObjectMotion_EnableActionAndSetCallback
	movs r1, #11
	movs r0, #4
	bl Object_LinkObjectAndSetCallback
	movs r0, #60
	bl Battle_WaitMode0
	ldr r1, .L_02009308
	movs r0, #10
	bl ObjectMotion_EnableActionAndSetCallback
	ldr r1, .L_0200930c
	movs r0, #25
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #10
	bl Object_RefreshSelectorById
	movs r0, #11
	bl Object_RefreshSelectorById
	movs r0, #25
	bl Object_RefreshSelectorById
	movs r1, #2
	movs r0, #11
	bl Motion_SetVarCbAndRefresh
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #11
	movs r1, #0
	movs r2, #10
	bl Func_02001e28
	movs r0, #25
	movs r1, #2
	movs r2, #10
	bl Func_02001ec0
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #4
	adds r3, #1
	strh r3, [r2]
	movs r1, #1
	bl Object_SetModeById
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #10
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #10
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #4
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r2, #10
	movs r0, #10
	movs r1, #0
	bl Func_02001e28
	movs r1, #3
	movs r0, #11
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #11
	movs r1, #0
	movs r2, #10
	bl Func_02001e28
	bl Func_02001d58
	pop {pc}
	.2byte 0x0000
.L_020092f4:
	.4byte MsgScene659No110
.L_020092f8:
	.4byte Data_02001f18
.L_020092fc:
	.4byte Data_02001f4c
.L_02009300:
	.4byte Data_02001f90
.L_02009304:
	.4byte Data_02001fc4
.L_02009308:
	.4byte Data_02002054
.L_0200930c:
	.4byte Data_020020e4
	.section .rodata.x02009f18,"a",%progbits
	.global Data_02001f18
Data_02001f18:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000030
	.4byte 0x01640000
	.4byte 0x00480000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01440000
	.4byte 0x00000000
	.4byte 0x00490000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00000011
	.global Data_02001f4c
Data_02001f4c:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000030
	.4byte 0x01530000
	.4byte 0x006f0000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000a000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000c000
	.4byte 0x00000011
	.global Data_02001f90
Data_02001f90:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000030
	.4byte 0x01640000
	.4byte 0x00480000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01440000
	.4byte 0x00000000
	.4byte 0x00490000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00000011
	.global Data_02001fc4
Data_02001fc4:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000002
	.4byte 0x01470000
	.4byte 0x00000000
	.4byte 0x00470000
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x01600000
	.4byte 0x00000000
	.4byte 0x00480000
	.4byte 0x00000001
	.4byte 0x00000030
	.4byte 0x01750000
	.4byte 0x006d0000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000c000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00006000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000011
	.global Data_02002054
Data_02002054:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000002
	.4byte 0x01470000
	.4byte 0x00000000
	.4byte 0x00470000
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x01600000
	.4byte 0x00000000
	.4byte 0x00480000
	.4byte 0x00000001
	.4byte 0x00000030
	.4byte 0x01750000
	.4byte 0x00610000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000c000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00006000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000011
	.global Data_020020e4
Data_020020e4:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000002
	.4byte 0x01470000
	.4byte 0x00180000
	.4byte 0x00470000
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x01600000
	.4byte 0x00000000
	.4byte 0x00480000
	.4byte 0x00000001
	.4byte 0x00000030
	.4byte 0x01750000
	.4byte 0x00610000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000c000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00006000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000011
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
	.4byte 0x0000002d
	.4byte 0x1010202c
	.4byte 0xffffffff
	.4byte 0x1020302c
	.4byte 0xffffffff
	.4byte 0x1030402d
	.4byte 0xffffffff
	.4byte 0x1040302d
	.4byte 0xffffffff
	.4byte 0x1050702c
	.4byte 0xffffffff
	.4byte 0x1060702d
	.4byte 0xffffffff
	.4byte 0x1070602d
	.4byte 0xffffffff
	.4byte 0x1080402c
	.4byte 0xffffffff
	.4byte 0x1090802c
	.4byte 0xffffffff
	.4byte 0x10a0502c
	.4byte 0xffffffff
	.4byte 0x10b0c02d
	.4byte 0xffffffff
	.4byte 0x10c0b02d
	.4byte 0xffffffff
	.4byte 0x10d0e02d
	.4byte 0xffffffff
	.4byte 0x10e0d02d
	.4byte 0xffffffff
	.4byte 0x10f0602c
	.4byte 0xffffffff
	.4byte 0x1140902c
	.4byte 0xffffffff
	.4byte 0x0000002e
	.4byte 0x1010102c
	.4byte 0xffffffff
	.4byte 0x1020a02c
	.4byte 0xffffffff
	.4byte 0x000001ff
	.global Data_02002240
Data_02002240:
	.4byte 0xffff0049
	.4byte 0x00000003
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00014000
	.4byte 0xffff004a
	.4byte 0x00000001
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x00700000
	.4byte 0x00012000
	.4byte 0xffff004b
	.4byte 0x00000001
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x006d0000
	.4byte 0x00014000
	.4byte 0xffff004c
	.4byte 0x00000001
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x00600000
	.4byte 0x00016000
	.4byte 0xffff00e0
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x0001a000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x01800000
	.4byte 0x00014000
	.4byte 0xffff004a
	.4byte 0x00000001
	.4byte 0x02680000
	.4byte 0x00000000
	.4byte 0x00700000
	.4byte 0x00012000
	.4byte 0xffff00c8
	.4byte 0x00000001
	.4byte 0x02800000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x0001a000
	.4byte 0xffff004c
	.4byte 0x00000001
	.4byte 0x02670000
	.4byte 0x00000000
	.4byte 0x01370000
	.4byte 0x00015000
	.4byte 0xffff00bf
	.4byte 0x00000001
	.4byte 0x02d80000
	.4byte 0x00000000
	.4byte 0x016c0000
	.4byte 0x0001c000
	.4byte 0xffff00ba
	.4byte 0x00000001
	.4byte 0x02e80000
	.4byte 0x00000000
	.4byte 0x015c0000
	.4byte 0x00014000
	.4byte 0xffff00ca
	.4byte 0x00000001
	.4byte 0x00a00000
	.4byte 0x00000000
	.4byte 0x022c0000
	.4byte 0x00014000
	.4byte 0xffff004a
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x02700000
	.4byte 0x0001e000
	.4byte 0xffff004c
	.4byte 0x00000001
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x02880000
	.4byte 0x0001a000
	.4byte 0xffff00c7
	.4byte 0x00000001
	.4byte 0x025c0000
	.4byte 0x00000000
	.4byte 0x02240000
	.4byte 0x00012000
	.4byte 0xffff00d1
	.4byte 0x00000003
	.4byte 0x01b00000
	.4byte 0x00000000
	.4byte 0x02680000
	.4byte 0x0001e000
	.4byte 0xffff0048
	.4byte 0x00000001
	.4byte 0x02380000
	.4byte 0x00000000
	.4byte 0x02060000
	.4byte 0x0001e000
	.4byte 0x006200f5
	.4byte 0x00000002
	.4byte 0x01700000
	.4byte 0x00000000
	.4byte 0x01400000
	.4byte 0x00004000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x02580000
	.4byte 0x00000000
	.4byte 0x00380000
	.4byte 0x00024000
	.4byte 0xffff00ee
	.4byte 0x00000001
	.4byte 0x02a40000
	.4byte 0x00000000
	.4byte 0x02fa0000
	.4byte 0x00018000
	.4byte 0xffff00ee
	.4byte 0x00000001
	.4byte 0x02da0000
	.4byte 0x00000000
	.4byte 0x02c80000
	.4byte 0x00014000
	.4byte 0x08ab00ef
	.4byte 0x00000001
	.4byte 0x034f0000
	.4byte 0x00000000
	.4byte 0x02c70000
	.4byte 0x00012000
	.4byte 0x08ab00ef
	.4byte 0x00000001
	.4byte 0x038a0000
	.4byte 0x00000000
	.4byte 0x02f80000
	.4byte 0x00010000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02002480
Data_02002480:
	.4byte 0xffff0049
	.4byte 0x00000003
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00014000
	.4byte 0xffff004a
	.4byte 0x00000001
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x00700000
	.4byte 0x00012000
	.4byte 0xffff004b
	.4byte 0x00000001
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x006d0000
	.4byte 0x00014000
	.4byte 0xffff004c
	.4byte 0x00000001
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x00600000
	.4byte 0x00016000
	.4byte 0xffff00e0
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x0001a000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x01800000
	.4byte 0x00014000
	.4byte 0xffff004a
	.4byte 0x00000001
	.4byte 0x02480000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x0000e000
	.4byte 0xffff00c8
	.4byte 0x00000001
	.4byte 0x02800000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x0000a000
	.4byte 0xffff004c
	.4byte 0x00000001
	.4byte 0x026c0000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x0000e000
	.4byte 0xffff00bf
	.4byte 0x00000001
	.4byte 0x02a80000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x0000c000
	.4byte 0xffff00ba
	.4byte 0x00000001
	.4byte 0x02b80000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00004000
	.4byte 0xffff00ca
	.4byte 0x00000001
	.4byte 0x00a00000
	.4byte 0x00000000
	.4byte 0x02280000
	.4byte 0x00014000
	.4byte 0xffff004a
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x02700000
	.4byte 0x0001e000
	.4byte 0xffff004c
	.4byte 0x00000001
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x02880000
	.4byte 0x0001a000
	.4byte 0xffff00c7
	.4byte 0x00000001
	.4byte 0x025c0000
	.4byte 0x00000000
	.4byte 0x02240000
	.4byte 0x00012000
	.4byte 0xffff00d1
	.4byte 0x00000003
	.4byte 0x01b00000
	.4byte 0x00000000
	.4byte 0x02680000
	.4byte 0x0001e000
	.4byte 0xffff0048
	.4byte 0x00000001
	.4byte 0x02380000
	.4byte 0x00000000
	.4byte 0x02060000
	.4byte 0x0001e000
	.4byte 0x006200f5
	.4byte 0x00000002
	.4byte 0x01700000
	.4byte 0x00000000
	.4byte 0x01400000
	.4byte 0x00004000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x02580000
	.4byte 0x00000000
	.4byte 0x00380000
	.4byte 0x00024000
	.4byte 0xffff0013
	.4byte 0x00000001
	.4byte 0x02660000
	.4byte 0x00000000
	.4byte 0x006f0000
	.4byte 0x00002000
	.4byte 0xffff0014
	.4byte 0x00000001
	.4byte 0x027c0000
	.4byte 0x00000000
	.4byte 0x006f0000
	.4byte 0x00005000
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0x05080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00008000
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0x05080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00008000
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0x05080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00008000
	.4byte 0xffff0007
	.4byte 0x00000001
	.4byte 0x05080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x0000c000
	.4byte 0xffff0005
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000c000
	.4byte 0xffff0006
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000c000
	.4byte 0xffff00ee
	.4byte 0x00000001
	.4byte 0x02a40000
	.4byte 0x00000000
	.4byte 0x02fa0000
	.4byte 0x00018000
	.4byte 0xffff00ee
	.4byte 0x00000001
	.4byte 0x02da0000
	.4byte 0x00000000
	.4byte 0x02c80000
	.4byte 0x00014000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02002750
Data_02002750:
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x02100000
	.4byte 0x00000000
	.4byte 0x03480000
	.4byte 0x0001c000
	.4byte 0xffff00c0
	.4byte 0x00000001
	.4byte 0x020a0000
	.4byte 0x00000000
	.4byte 0x03480000
	.4byte 0x0001c000
	.4byte 0xffff00bb
	.4byte 0x00000001
	.4byte 0x01d80000
	.4byte 0x00000000
	.4byte 0x03480000
	.4byte 0x0001c000
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00008000
	.4byte 0x08ab0007
	.4byte 0x00000001
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x03280000
	.4byte 0x0002c000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x016a0000
	.4byte 0x00000000
	.4byte 0x034f0000
	.4byte 0x0002c000
	.4byte 0x18ff00bb
	.4byte 0x00000001
	.4byte 0x018a0000
	.4byte 0x00000000
	.4byte 0x035f0000
	.4byte 0x0001a000
	.4byte 0x18ff00bb
	.4byte 0x00000001
	.4byte 0x018c0000
	.4byte 0x00000000
	.4byte 0x03500000
	.4byte 0x00028000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02002828
Data_02002828:
	.4byte 0xffff0047
	.4byte 0x00000001
	.4byte 0x00c00000
	.4byte 0x00000000
	.4byte 0x00800000
	.4byte 0x00010000
	.4byte 0xffff0048
	.4byte 0x00000001
	.4byte 0x00800000
	.4byte 0x00000000
	.4byte 0x00c00000
	.4byte 0x00014000
	.4byte 0xffff00c6
	.4byte 0x00000001
	.4byte 0x01640000
	.4byte 0x00000000
	.4byte 0x006d0000
	.4byte 0x00024000
	.4byte 0x18ab00ba
	.4byte 0x00000001
	.4byte 0x01650000
	.4byte 0x00000000
	.4byte 0x00830000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020028a0
Data_020028a0:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000001
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000001
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00000001
	.4byte 0xffff000c
	.4byte 0x0000000c
	.4byte 0x00000001
	.4byte 0xffff000d
	.4byte 0x0000000d
	.4byte 0x00000001
	.4byte 0xffff000e
	.4byte 0x0000000e
	.4byte 0x00000001
	.4byte 0xffff000f
	.4byte 0x0000000f
	.4byte 0x00000001
	.4byte 0xffff0010
	.4byte 0x00000010
	.4byte 0x00000001
	.4byte 0xffff0011
	.4byte 0x00000011
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte MsgScene659No05
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte Func_020000a4
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte MsgScene659No09
	.4byte 0x0000c400
	.4byte 0xffff000b
	.4byte MsgScene659No10
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte MsgScene659No13
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte MsgScene659No14
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte MsgScene659No17
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte MsgScene659No19
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte MsgScene659No20
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte MsgScene659No21
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte MsgScene659No22
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte Func_020001b8
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte MsgScene659No34
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte MsgScene659No35
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte Func_020000fc
	.4byte 0x00000000
	.4byte 0xffff0017
	.4byte MsgScene659No40
	.4byte 0x00000000
	.4byte 0xffff0018
	.4byte MsgScene659No41
	.4byte 0x00000000
	.4byte 0xffff001a
	.4byte Func_0200023c
	.4byte 0x00000000
	.4byte 0xffff001b
	.4byte MsgScene659No42
	.4byte 0x00000000
	.4byte 0xffff001c
	.4byte MsgScene659No43
	.4byte 0x00000000
	.4byte 0xffff001d
	.4byte MsgScene659No165
	.4byte 0x00000000
	.4byte 0xffff001e
	.4byte MsgScene659No166
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte MsgScene659No07
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte MsgScene659No08
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte MsgScene659No11
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte MsgScene659No12
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte MsgScene659No15
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte MsgScene659No16
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte MsgScene659No23
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte MsgScene659No24
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte MsgScene659No25
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte MsgScene659No26
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte MsgScene659No27
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte MsgScene659No36
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte MsgScene659No37
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte MsgScene659No38
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte MsgScene659No44
	.4byte 0x00008d15
	.4byte 0xffff0017
	.4byte MsgScene659No45
	.4byte 0x00008d15
	.4byte 0xffff0018
	.4byte MsgScene659No46
	.4byte 0x00008d15
	.4byte 0xffff001b
	.4byte MsgScene659No47
	.4byte 0x00008d15
	.4byte 0xffff001c
	.4byte MsgScene659No48
	.4byte 0x00008d15
	.4byte 0xffff001d
	.4byte MsgScene659No167
	.4byte 0x00008d15
	.4byte 0xffff001e
	.4byte MsgScene659No168
	.4byte 0x00000173
	.4byte 0xffff00c8
	.2byte MsgScene659No169
	.2byte 0x0040
	.4byte 0x00000173
	.4byte 0xffff00c9
	.2byte MsgScene659No170
	.2byte 0x0040
	.4byte 0x00000173
	.4byte 0xffff00ca
	.2byte MsgScene659No171
	.2byte 0x0040
	.4byte 0x00000173
	.4byte 0xffff00cb
	.2byte MsgScene659No172
	.2byte 0x0040
	.4byte 0x00000173
	.4byte 0xffff00cc
	.2byte MsgScene659No173
	.2byte 0x0040
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02002bb8
Data_02002bb8:
	.4byte 0x00000001
	.4byte 0xffff0014
	.4byte 0x00000014
	.4byte 0x00000000
	.4byte 0x18ff0009
	.4byte MsgScene659No143
	.4byte 0x00000000
	.4byte 0x18ff000a
	.4byte MsgScene659No144
	.4byte 0x0000c400
	.4byte 0x18ff000d
	.4byte MsgWhenAndHis
	.4byte 0x0000a400
	.4byte 0x18ff000d
	.4byte MsgWhenAndHis
	.4byte 0x00008400
	.4byte 0x18ff000d
	.4byte MsgWhenAndHis
	.4byte 0x00000000
	.4byte 0x18ff000e
	.4byte Func_02000428
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte MsgScene659No75
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte MsgScene659No76
	.4byte 0x00000000
	.4byte 0xffff0007
	.4byte Func_02000ed8
	.4byte 0x00008d15
	.4byte 0x18ff0009
	.4byte MsgScene659No145
	.4byte 0x00008d15
	.4byte 0x18ff000a
	.4byte MsgScene659No146
	.4byte 0x00008d15
	.4byte 0x18ff000d
	.4byte MsgSoThisFellow
	.4byte 0x00008d15
	.4byte 0x1211000e
	.4byte MsgIKnewI
	.4byte 0x00008d15
	.4byte 0x18ff040e
	.4byte Func_02000428
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte MsgScene659No77
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte MsgScene659No78
	.4byte 0x00008d15
	.4byte 0xffff0007
	.4byte Func_02000f0c
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02002c9c
Data_02002c9c:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x08ab0008
	.4byte MsgScene659No01
	.4byte 0x00000000
	.4byte 0x08ff0008
	.4byte MsgScene659No49
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte MsgScene659No98
	.4byte 0x00000000
	.4byte 0x08ab0009
	.4byte MsgScene659No02
	.4byte 0x00000000
	.4byte 0x08ff0009
	.4byte MsgScene659No50
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte MsgScene659No99
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte MsgScene659No31
	.4byte 0x00000000
	.4byte 0x08ff000b
	.4byte MsgScene659No79
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte MsgScene659No147
	.4byte 0x00008d15
	.4byte 0x08ab0008
	.4byte MsgScene659No03
	.4byte 0x00008d15
	.4byte 0x08ff0008
	.4byte MsgScene659No51
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte MsgScene659No100
	.4byte 0x00008d15
	.4byte 0x08ab0009
	.4byte MsgScene659No04
	.4byte 0x00008d15
	.4byte 0x08ff0009
	.4byte MsgScene659No52
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte MsgScene659No101
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte MsgScene659No32
	.4byte 0x00008d15
	.4byte 0x08ff000b
	.4byte MsgScene659No80
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte MsgScene659No148
	.4byte 0x00000173
	.4byte 0xffff00cd
	.2byte MsgScene659No174
	.2byte 0x0040
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02002da4
Data_02002da4:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000001
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000001
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00000001
	.4byte 0xffff000c
	.4byte 0x0000000c
	.4byte 0x00000001
	.4byte 0xffff000d
	.4byte 0x0000000d
	.4byte 0x00000001
	.4byte 0xffff000e
	.4byte 0x0000000e
	.4byte 0x00000001
	.4byte 0xffff000f
	.4byte 0x0000000f
	.4byte 0x00000001
	.4byte 0xffff0010
	.4byte 0x00000010
	.4byte 0x00000001
	.4byte 0xffff0011
	.4byte 0x00000011
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte MsgScene659No53
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte MsgScene659No54
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte MsgScene659No57
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte MsgScene659No58
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte MsgScene659No61
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte MsgScene659No62
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte MsgScene659No65
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte MsgScene659No66
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte MsgScene659No67
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte MsgScene659No68
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte MsgScene659No69
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte Func_020001b8
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte MsgScene659No82
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte MsgScene659No83
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte Func_020000fc
	.4byte 0x00000000
	.4byte 0xffff0017
	.4byte MsgScene659No88
	.4byte 0x00000000
	.4byte 0xffff0018
	.4byte MsgScene659No89
	.4byte 0x00000000
	.4byte 0xffff001a
	.4byte Func_0200023c
	.4byte 0x00000000
	.4byte 0xffff001b
	.4byte MsgScene659No90
	.4byte 0x00000000
	.4byte 0xffff001c
	.4byte MsgScene659No91
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte MsgScene659No55
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte MsgScene659No56
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte MsgScene659No59
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte MsgScene659No60
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte MsgScene659No63
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte MsgScene659No64
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte MsgScene659No70
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte MsgScene659No71
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte MsgScene659No72
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte MsgScene659No73
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte MsgScene659No74
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte MsgScene659No84
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte MsgScene659No85
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte MsgScene659No86
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte MsgScene659No92
	.4byte 0x00008d15
	.4byte 0xffff0017
	.4byte MsgScene659No93
	.4byte 0x00008d15
	.4byte 0xffff0018
	.4byte MsgScene659No94
	.4byte 0x00008d15
	.4byte 0xffff001b
	.4byte MsgScene659No95
	.4byte 0x00008d15
	.4byte 0xffff001c
	.4byte MsgScene659No96
	.4byte 0x00000173
	.4byte 0xffff00c8
	.2byte MsgScene659No169
	.2byte 0x0040
	.4byte 0x00000173
	.4byte 0xffff00c9
	.2byte MsgScene659No170
	.2byte 0x0040
	.4byte 0x00000173
	.4byte 0xffff00ca
	.2byte MsgScene659No171
	.2byte 0x0040
	.4byte 0x00000173
	.4byte 0xffff00cb
	.2byte MsgScene659No172
	.2byte 0x0040
	.4byte 0x00000173
	.4byte 0xffff00cc
	.2byte MsgScene659No173
	.2byte 0x0040
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_0200308c
Data_0200308c:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000001
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000001
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00000001
	.4byte 0xffff000c
	.4byte 0x0000000c
	.4byte 0x00000001
	.4byte 0xffff000d
	.4byte 0x0000000d
	.4byte 0x00000001
	.4byte 0xffff000e
	.4byte 0x0000000e
	.4byte 0x00000001
	.4byte 0xffff000f
	.4byte 0x0000000f
	.4byte 0x00000001
	.4byte 0xffff0010
	.4byte 0x00000010
	.4byte 0x00000001
	.4byte 0xffff0011
	.4byte 0x00000011
	.4byte 0x00000002
	.4byte 0x0989001e
	.4byte Func_02001450
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte MsgScene659No102
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte MsgScene659No103
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte Func_0200028c
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte Func_02000308
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte MsgScene659No122
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte MsgScene659No123
	.4byte 0x00000000
	.4byte 0xffff001b
	.4byte MsgScene659No129
	.4byte 0x00000000
	.4byte 0xffff001c
	.4byte MsgScene659No130
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte MsgScene659No131
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte MsgScene659No132
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte MsgScene659No133
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte MsgScene659No134
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte MsgScene659No135
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte Func_020001b8
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte MsgScene659No150
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte MsgScene659No151
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte Func_020000fc
	.4byte 0x00000000
	.4byte 0xffff0017
	.4byte MsgScene659No156
	.4byte 0x00000000
	.4byte 0xffff0018
	.4byte MsgScene659No157
	.4byte 0x00000000
	.4byte 0xffff001a
	.4byte Func_0200023c
	.4byte 0x00000000
	.4byte 0xffff0020
	.4byte MsgScene659No158
	.4byte 0x00000000
	.4byte 0xffff0021
	.4byte MsgScene659No159
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte MsgScene659No104
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte MsgScene659No105
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte Func_02000368
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte Func_020003c8
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte MsgScene659No124
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte MsgScene659No125
	.4byte 0x00008d15
	.4byte 0xffff001b
	.4byte MsgScene659No136
	.4byte 0x00008d15
	.4byte 0xffff001c
	.4byte MsgScene659No137
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte MsgScene659No138
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte MsgScene659No139
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte MsgScene659No140
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte MsgScene659No141
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte MsgScene659No142
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte MsgScene659No152
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte MsgScene659No153
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte MsgScene659No154
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte MsgScene659No160
	.4byte 0x00008d15
	.4byte 0xffff0017
	.4byte MsgScene659No161
	.4byte 0x00008d15
	.4byte 0xffff0018
	.4byte MsgScene659No162
	.4byte 0x00008d15
	.4byte 0xffff0020
	.4byte MsgScene659No163
	.4byte 0x00008d15
	.4byte 0xffff0021
	.4byte MsgScene659No164
	.4byte 0x0001c014
	.4byte 0x098a000a
	.4byte Func_02001008
	.4byte 0x0001c114
	.4byte 0x098a000a
	.4byte Func_02001310
	.4byte 0x00000173
	.4byte 0xffff00c8
	.2byte MsgScene659No169
	.2byte 0x0040
	.4byte 0x00000173
	.4byte 0xffff00c9
	.2byte MsgScene659No170
	.2byte 0x0040
	.4byte 0x00000173
	.4byte 0xffff00ca
	.2byte MsgScene659No171
	.2byte 0x0040
	.4byte 0x00000173
	.4byte 0xffff00cb
	.2byte MsgScene659No172
	.2byte 0x0040
	.4byte 0x00000173
	.4byte 0xffff00cc
	.2byte MsgScene659No173
	.2byte 0x0040
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020033c8
Data_020033c8:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x03680000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x03880000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x03980000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000011
	.global Data_02003418
Data_02003418:
	.4byte 0x0000002e
	.4byte Func_020007ac
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000002e
	.4byte Func_020007ac
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000002e
	.4byte Func_020007ac
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000002e
	.4byte Func_020007ac
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000002e
	.4byte Func_020007ac
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000002e
	.4byte Func_020007ac
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000002e
	.4byte Func_020007ac
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000002e
	.4byte Func_020007ac
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.global Data_0200349c
Data_0200349c:
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000064
	.4byte 0x00000000
	.4byte 0x00000024
	.4byte 0x00000017
	.4byte 0x00000006
	.4byte 0x00030000
	.4byte 0x00000000
	.4byte 0x00000009
	.4byte 0x00000017
	.4byte 0x00000006
	.4byte 0x00050000
	.4byte 0x00000017
	.4byte 0x00000005
	.4byte 0x00010000
	.4byte 0x00000000
	.4byte 0x00000009
	.4byte 0x00000017
	.4byte 0x00000006
	.4byte 0x00040000
	.4byte 0x00000000
	.4byte 0x00000009
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000017
	.4byte 0x00000005
	.4byte 0x00010000
	.4byte 0x00000017
	.4byte 0x00000006
	.4byte 0x00050000
	.4byte 0x00000000
	.4byte 0x00000009
	.4byte 0x00000017
	.4byte 0x00000006
	.4byte 0x00040000
	.4byte 0x00000000
	.4byte 0x00000009
	.4byte 0x00000017
	.4byte 0x00000006
	.4byte 0x00040000
	.4byte 0x00000017
	.4byte 0x00000005
	.4byte 0x00010000
	.4byte 0x00000000
	.4byte 0x00000009
	.4byte 0x00000017
	.4byte 0x00000006
	.4byte 0x00040000
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x00000017
	.4byte 0x00000005
	.4byte 0x00010000
	.4byte 0x80010000
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00010000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x00010000
	.4byte 0x00000017
	.4byte 0x00000006
	.4byte 0xffff0000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000006
	.4byte 0x00010000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000017
	.4byte 0x00000006
	.4byte 0xffff0000
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000006
	.4byte 0x00010000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0xc0010000
	.4byte 0x00000011
