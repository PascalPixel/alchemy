.syntax unified
	.thumb
	.section .text.x02008398,"ax",%progbits
	.balign 4
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
	.balign 4
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
