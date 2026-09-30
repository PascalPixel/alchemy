.syntax unified
	.thumb
	.section .text.x02008038,"ax",%progbits
	.global Func_02000038
	.thumb_func
Func_02000038:
	push {lr}
	ldr r3, [r0, #24]
	movs r2, #128
	lsls r2, r2, #5
	adds r3, r3, r2
	movs r2, #128
	lsls r2, r2, #9
	str r3, [r0, #24]
	cmp r3, r2
	blt .L_02008056
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r0, #24]
	movs r3, #0
	str r3, [r0, #108]
.L_02008056:
	ldr r3, [r0, #24]
	str r3, [r0, #28]
	pop {pc}
	.section .text.x02008070,"ax",%progbits
	.global Func_02000070
	.thumb_func
Func_02000070:
	push {lr}
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #126
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020080b4
	ldr r3, .L_02008128
	movs r0, #240
	lsls r0, r0, #1
	adds r3, r3, r0
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_0200812c
	cmp r2, r3
	bne .L_02008096
	ldr r0, .L_02008130
	b .L_02008124
.L_02008096:
	ldr r3, .L_02008134
	cmp r2, r3
	bne .L_020080a0
	ldr r0, .L_02008138
	b .L_02008124
.L_020080a0:
	ldr r3, .L_0200813c
	cmp r2, r3
	bne .L_020080aa
	ldr r0, .L_02008140
	b .L_02008124
.L_020080aa:
	ldr r3, .L_02008144
	cmp r2, r3
	bne .L_02008122
	ldr r0, .L_02008148
	b .L_02008124
.L_020080b4:
	movs r0, #136
	lsls r0, r0, #4
	adds r0, #255
	bl GameFlag_Test
	ldr r3, .L_02008128
	ldr r1, .L_0200812c
	cmp r0, #0
	beq .L_020080f6
	movs r2, #240
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r0, #0
	ldrsh r2, [r3, r0]
	cmp r2, r1
	bne .L_020080d8
	ldr r0, .L_0200814c
	b .L_02008124
.L_020080d8:
	ldr r3, .L_02008134
	cmp r2, r3
	bne .L_020080e2
	ldr r0, .L_02008150
	b .L_02008124
.L_020080e2:
	ldr r3, .L_0200813c
	cmp r2, r3
	bne .L_020080ec
	ldr r0, .L_02008154
	b .L_02008124
.L_020080ec:
	ldr r3, .L_02008144
	cmp r2, r3
	bne .L_02008122
	ldr r0, .L_02008158
	b .L_02008124
.L_020080f6:
	movs r2, #240
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r0, #0
	ldrsh r2, [r3, r0]
	cmp r2, r1
	beq .L_02008122
	ldr r3, .L_02008134
	cmp r2, r3
	bne .L_0200810e
	ldr r0, .L_0200815c
	b .L_02008124
.L_0200810e:
	ldr r3, .L_0200813c
	cmp r2, r3
	bne .L_02008118
	ldr r0, .L_02008160
	b .L_02008124
.L_02008118:
	ldr r3, .L_02008144
	cmp r2, r3
	bne .L_02008122
	ldr r0, .L_02008164
	b .L_02008124
.L_02008122:
	ldr r0, .L_02008168
.L_02008124:
	pop {pc}
	.2byte 0x0000
.L_02008128:
	.4byte gPartyState
.L_0200812c:
	.4byte 0x000000b9
.L_02008130:
	.4byte Data_02003aec
.L_02008134:
	.4byte 0x000000ba
.L_02008138:
	.4byte Data_02003b94
.L_0200813c:
	.4byte 0x000000bb
.L_02008140:
	.4byte Data_02003c0c
.L_02008144:
	.4byte 0x000000bc
.L_02008148:
	.4byte Data_02003c84
.L_0200814c:
	.4byte Data_020038ac
.L_02008150:
	.4byte Data_0200390c
.L_02008154:
	.4byte Data_02003984
.L_02008158:
	.4byte Data_020039cc
.L_0200815c:
	.4byte Data_020037a4
.L_02008160:
	.4byte Data_0200381c
.L_02008164:
	.4byte Data_02003864
.L_02008168:
	.4byte Data_0200375c
	.section .text.x0200816c,"ax",%progbits
	.global Func_0200016c
	.thumb_func
Func_0200016c:
	push {r5, r6, r7, lr}
	adds r5, r0, #0
	adds r7, r1, #0
	adds r6, r2, #0
	bl Func_0200345c
	movs r0, #0
	bl Func_020035b4
	movs r0, #158
	bl Func_0200362c
	ldrh r1, [r5, #4]
	ldrh r2, [r5, #6]
	ldr r0, [r5]
	bl Func_02003414
	ldr r5, .L_020081f0
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	ldr r0, [r5]
	bl Object_GetById
	movs r3, #2
	adds r0, #85
	strb r3, [r0]
	movs r1, #128
	movs r2, #128
	ldr r0, [r5]
	lsls r1, r1, #8
	lsls r2, r2, #7
	bl ObjectMotion_SetSpeedParameters
	ldr r0, [r5]
	movs r1, #2
	bl Object_SetModeById
	ldr r0, [r5]
	cmp r6, #0
	bne .L_020081ca
	movs r2, #8
	movs r1, #2
	negs r2, r2
	bl ObjectMotion_SnapHeadingAndOffset
	b .L_020081d4
.L_020081ca:
	movs r2, #8
	movs r1, #0
	negs r2, r2
	bl ObjectMotion_OffsetPositionAndResetMotion
.L_020081d4:
	movs r0, #10
	bl Battle_WaitMode0
	adds r0, r7, #0
	bl Func_02003574
	bl Event_ClearStatus1c6
	bl Event_WaitValue1c8Frames
	bl Func_02003464
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_020081f0:
	.4byte gPartyState
	.section .text.x020081f4,"ax",%progbits
	.global Func_020001f4
	.thumb_func
Func_020001f4:
	push {lr}
	adds r1, r0, #0
	movs r2, #0
	ldr r0, .L_02008204
	bl Func_0200016c
	pop {pc}
	.2byte 0x0000
.L_02008204:
	.4byte Data_02003d10
	.section .text.x02008208,"ax",%progbits
	.global Func_02000208
	.thumb_func
Func_02000208:
	push {r5, r6, lr}
	ldr r5, .L_020082f4
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	movs r2, #212
	ldr r6, .L_020082f8
	lsls r2, r2, #1
	ldr r0, [r5]
	movs r1, #168
	bl ObjectMotion_SetPositionAndReset
	ldr r0, [r5]
	movs r1, #0
	bl Func_02003534
	movs r0, #158
	bl Func_0200362c
	ldrh r1, [r6, #4]
	ldrh r2, [r6, #6]
	ldr r0, [r6]
	bl Func_02003414
	movs r0, #8
	bl Object_GetById
	movs r3, #0
	adds r0, #85
	movs r1, #184
	movs r2, #204
	strb r3, [r0]
	lsls r1, r1, #16
	movs r0, #8
	lsls r2, r2, #17
	bl Func_020034d4
	movs r1, #230
	movs r2, #230
	lsls r1, r1, #9
	lsls r2, r2, #8
	movs r0, #8
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r2, #16
	movs r0, #8
	movs r1, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #128
	lsls r1, r1, #8
	movs r0, #8
	bl Func_02003534
	ldr r5, .L_020082fc
	adds r0, r5, #0
	bl Func_0200350c
	movs r1, #0
	movs r0, #8
	bl UiText_OpenMessageAtObject
	bl Func_020035f4
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_020082a4
	movs r0, #10
	bl Battle_WaitMode0
	adds r0, r5, #1
	bl Func_0200350c
	b .L_020082b0
.L_020082a4:
	movs r0, #20
	bl Battle_WaitMode0
	adds r0, r5, #2
	bl Func_0200350c
.L_020082b0:
	movs r0, #8
	movs r1, #0
	bl Func_02003524
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #8
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r2, #14
	movs r0, #8
	movs r1, #0
	negs r2, r2
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl Func_020034d4
	ldr r6, .L_02008300
	ldr r0, [r6]
	ldrh r1, [r6, #4]
	ldrh r2, [r6, #6]
	bl Func_02003414
	movs r0, #158
	bl Func_0200362c
	pop {r5, r6, pc}
.L_020082f4:
	.4byte gPartyState
.L_020082f8:
	.4byte Data_02003d10
.L_020082fc:
	.4byte 0x0000241d
.L_02008300:
	.4byte Data_02003d18
	.section .text.x02008304,"ax",%progbits
	.global Func_02000304
	.thumb_func
Func_02000304:
	push {r5, r6, lr}
	ldr r5, .L_0200834c
	adds r6, r0, #0
	adds r0, r5, #0
	bl Func_0200350c
	movs r1, #0
	adds r0, r6, #0
	bl UiText_OpenMessageAtObject
	bl Func_020035f4
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_02008334
	movs r0, #10
	bl Battle_WaitMode0
	adds r0, r5, #1
	bl Func_0200350c
	b .L_02008340
.L_02008334:
	movs r0, #20
	bl Battle_WaitMode0
	adds r0, r5, #2
	bl Func_0200350c
.L_02008340:
	adds r0, r6, #0
	movs r1, #0
	bl Func_02003524
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200834c:
	.4byte 0x00002600
	.section .text.x02008350,"ax",%progbits
	.global Func_02000350
	.thumb_func
Func_02000350:
	push {r5, lr}
	adds r5, r0, #0
	ldr r0, .L_02008384
	bl Func_0200350c
	movs r1, #0
	adds r0, r5, #0
	bl Func_02003524
	adds r0, r5, #0
	bl Object_GetById
	ldr r3, .L_02008388
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	adds r5, r0, #0
	ldr r0, [r3]
	bl Object_GetById
	adds r1, r0, #0
	adds r0, r5, #0
	bl Func_02003624
	pop {r5, pc}
	.2byte 0x0000
.L_02008384:
	.4byte 0x000025fa
.L_02008388:
	.4byte gPartyState
	.section .text.x0200838c,"ax",%progbits
	.global Func_0200038c
	.thumb_func
Func_0200038c:
	push {r5, lr}
	adds r5, r0, #0
	ldr r0, .L_020083b8
	bl Func_0200350c
	adds r0, r5, #0
	movs r1, #0
	bl Func_02003524
	movs r1, #208
	adds r0, r5, #0
	lsls r1, r1, #8
	bl Func_02003534
	adds r0, r5, #0
	bl Object_GetById
	movs r1, #8
	bl Object_SetActionCallback
	pop {r5, pc}
	.2byte 0x0000
.L_020083b8:
	.4byte 0x000025ff
	.section .text.x020083bc,"ax",%progbits
	.global Func_020003bc
	.thumb_func
Func_020003bc:
	push {r5, lr}
	sub sp, #8
	movs r3, #77
	str r3, [sp, #4]
	movs r5, #41
	movs r0, #41
	movs r1, #97
	movs r2, #1
	movs r3, #4
	str r5, [sp, #0]
	bl Func_02003424
	movs r3, #13
	str r3, [sp, #4]
	movs r0, #41
	movs r1, #33
	movs r2, #1
	movs r3, #4
	str r5, [sp, #0]
	bl Func_0200341c
	add sp, #8
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x020083ec,"ax",%progbits
	.global Func_020003ec
	.thumb_func
Func_020003ec:
	push {r5, r6, lr}
	sub sp, #8
	movs r3, #77
	str r3, [sp, #4]
	movs r5, #41
	movs r0, #50
	movs r1, #97
	movs r2, #1
	movs r3, #4
	str r5, [sp, #0]
	bl Func_02003424
	movs r3, #13
	str r3, [sp, #4]
	movs r1, #33
	movs r2, #1
	movs r3, #4
	movs r0, #46
	str r5, [sp, #0]
	bl Func_0200341c
	ldr r5, .L_0200845c
	movs r0, #133
	lsls r0, r0, #2
	adds r6, r5, r0
	ldr r0, [r6]
	bl Object_GetById
	ldr r3, [r0, #8]
	movs r2, #240
	asrs r4, r3, #20
	ldr r3, [r0, #16]
	lsls r2, r2, #1
	asrs r1, r3, #20
	adds r3, r5, r2
	movs r0, #0
	ldrsh r2, [r3, r0]
	ldr r3, .L_02008460
	cmp r2, r3
	bne .L_02008456
	cmp r4, #41
	bne .L_02008456
	adds r3, r1, #0
	subs r3, #13
	cmp r3, #4
	bhi .L_02008456
	movs r1, #166
	movs r2, #140
	ldr r0, [r6]
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl Func_020034d4
.L_02008456:
	add sp, #8
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200845c:
	.4byte gPartyState
.L_02008460:
	.4byte 0x000000bc
	.section .text.x02008464,"ax",%progbits
	.global Func_02000464
	.thumb_func
Func_02000464:
	push {lr}
	movs r2, #144
	movs r1, #163
	lsls r2, r2, #4
	lsls r1, r1, #1
	adds r2, #120
	bl Func_02003474
	pop {pc}
	.2byte 0x0000
	.section .text.x02008478,"ax",%progbits
	.global Func_02000478
	.thumb_func
Func_02000478:
	push {lr}
	ldr r3, .L_020084b0
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_020084b4
	cmp r2, r3
	beq .L_020084aa
	ldr r3, .L_020084b8
	cmp r2, r3
	bne .L_02008496
	ldr r0, .L_020084bc
	b .L_020084ac
.L_02008496:
	ldr r3, .L_020084c0
	cmp r2, r3
	bne .L_020084a0
	ldr r0, .L_020084c4
	b .L_020084ac
.L_020084a0:
	ldr r3, .L_020084c8
	cmp r2, r3
	bne .L_020084aa
	ldr r0, .L_020084cc
	b .L_020084ac
.L_020084aa:
	ldr r0, .L_020084d0
.L_020084ac:
	pop {pc}
	.2byte 0x0000
.L_020084b0:
	.4byte gPartyState
.L_020084b4:
	.4byte 0x000000b9
.L_020084b8:
	.4byte 0x000000ba
.L_020084bc:
	.4byte Data_02003e7c
.L_020084c0:
	.4byte 0x000000bb
.L_020084c4:
	.4byte Data_02003ffc
.L_020084c8:
	.4byte 0x000000bc
.L_020084cc:
	.4byte Data_02004104
.L_020084d0:
	.4byte Data_02003d20
	.section .text.x020084d4,"ax",%progbits
	.global Func_020004d4
	.thumb_func
Func_020004d4:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	ldr r6, .L_020085d4
	adds r2, #85
	str r2, [r3]
	movs r3, #240
	lsls r3, r3, #1
	adds r5, r6, r3
	movs r3, #0
	ldrsh r2, [r5, r3]
	ldr r3, .L_020085d8
	cmp r2, r3
	beq .L_0200850e
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r6, r2
	ldr r0, [r3]
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #32
	orrs r3, r2
	strb r3, [r0]
.L_0200850e:
	movs r0, #0
	bl Func_020035ac
	movs r3, #0
	ldrsh r2, [r5, r3]
	ldr r3, .L_020085dc
	cmp r2, r3
	bne .L_020085a6
	movs r2, #241
	lsls r2, r2, #1
	adds r5, r6, r2
	movs r2, #0
	ldrsh r3, [r5, r2]
	cmp r3, #98
	bne .L_02008530
	bl Func_02001774
.L_02008530:
	movs r2, #0
	ldrsh r3, [r5, r2]
	cmp r3, #99
	bne .L_02008548
	movs r3, #245
	lsls r3, r3, #1
	adds r2, r6, r3
	movs r3, #23
	strh r3, [r2]
	strh r3, [r5]
	bl Func_02001868
.L_02008548:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #119
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008588
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #120
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008588
	movs r1, #163
	movs r0, #10
	lsls r1, r1, #1
	bl Func_0200361c
	movs r1, #166
	movs r2, #232
	movs r0, #10
	lsls r1, r1, #18
	lsls r2, r2, #16
	bl Func_020034d4
	movs r0, #10
	bl Object_GetById
	movs r3, #4
	adds r0, #85
	strb r3, [r0]
.L_02008588:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #119
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020085ce
	movs r1, #166
	movs r2, #200
	movs r0, #8
	lsls r1, r1, #18
	lsls r2, r2, #16
	bl Func_020034d4
	b .L_020085ce
.L_020085a6:
	ldr r3, .L_020085e0
	cmp r2, r3
	bne .L_020085ce
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #126
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020085ce
	movs r1, #11
	movs r0, #8
	bl Object_SetModeById
	movs r0, #8
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
.L_020085ce:
	movs r0, #0
	pop {r5, r6, pc}
	.2byte 0x0000
.L_020085d4:
	.4byte gPartyState
.L_020085d8:
	.4byte 0x000000b9
.L_020085dc:
	.4byte 0x000000bc
.L_020085e0:
	.4byte 0x000000bb
	.section .text.x020085e8,"ax",%progbits
	.global Func_020005e8
	.thumb_func
Func_020005e8:
	push {r5, r6, lr}
	ldr r5, .L_02008678
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	movs r1, #162
	movs r2, #200
	adds r6, r0, #0
	lsls r1, r1, #2
	ldr r0, [r5]
	bl ObjectMotion_SetPositionAndReset
	ldr r0, [r5]
	movs r1, #0
	bl Func_02003534
	movs r0, #5
	bl Battle_WaitMode0
	ldr r0, [r5]
	movs r1, #6
	bl Motion_SetModeAndWaitAnimation
	movs r0, #7
	bl Battle_WaitMode0
	adds r1, r6, #0
	movs r0, #10
	bl Func_0200361c
	movs r1, #162
	movs r2, #196
	lsls r1, r1, #18
	lsls r2, r2, #16
	movs r0, #10
	bl Func_020034d4
	ldr r0, [r5]
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	strb r3, [r0]
	movs r1, #16
	movs r2, #8
	negs r1, r1
	negs r2, r2
	ldr r0, [r5]
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #1
	bl Battle_WaitMode0
	ldr r0, [r5]
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	strb r3, [r0]
	movs r1, #0
	ldr r0, [r5]
	bl Func_02003534
	movs r0, #10
	bl Battle_WaitMode0
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02008678:
	.4byte gPartyState
	.section .text.x0200867c,"ax",%progbits
	.global Func_0200067c
	.thumb_func
Func_0200067c:
	push {r5, r6, r7, lr}
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	adds r1, #204
	adds r2, #102
	adds r5, r0, #0
	movs r0, #8
	bl ObjectMotion_SetSpeedParameters
	movs r0, #8
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r7, #254
	adds r3, r7, #0
	ands r3, r2
	strb r3, [r0]
	movs r2, #6
	movs r1, #0
	movs r0, #8
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #1
	bl Battle_WaitMode0
	movs r0, #8
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	movs r6, #1
	orrs r3, r6
	strb r3, [r0]
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #8
	movs r0, #8
	bl Object_SetModeById
	movs r0, #15
	bl Battle_WaitMode0
	movs r1, #0
	movs r2, #0
	movs r0, #10
	bl Func_020034d4
	movs r0, #15
	bl Battle_WaitMode0
	movs r0, #8
	movs r1, #1
	bl Object_SetModeById
	movs r0, #15
	bl Battle_WaitMode0
	cmp r5, #0
	bne .L_02008730
	movs r0, #8
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	adds r3, r7, #0
	ands r3, r2
	movs r2, #6
	strb r3, [r0]
	movs r1, #0
	negs r2, r2
	movs r0, #8
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #1
	bl Battle_WaitMode0
	movs r0, #8
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	orrs r3, r6
	strb r3, [r0]
	movs r0, #10
	bl Battle_WaitMode0
.L_02008730:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x02008734,"ax",%progbits
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
	bl Engine_MathDivide
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
	bl Engine_MathDivide
	subs r6, r6, r5
	add r8, r0
	adds r1, r7, #0
	mov r0, r9
	muls r0, r6
	bl Engine_MathDivide
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
	bl WaitFrames
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
	.section .text.x02008804,"ax",%progbits
	.global Func_02000804
	.thumb_func
Func_02000804:
	push {r5, lr}
	adds r5, r0, #0
	bl Func_0200345c
	movs r0, #0
	bl Func_020035b4
	ldr r0, .L_02008974
	bl Func_0200350c
	adds r0, r5, #0
	bl Func_020005e8
	movs r1, #192
	movs r0, #8
	lsls r1, r1, #6
	bl Func_02003534
	movs r1, #7
	movs r0, #8
	bl Object_SetModeById
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #2
	movs r2, #50
	adds r1, #255
	movs r0, #8
	bl Func_02003544
	movs r1, #1
	movs r0, #8
	bl Object_SetModeById
	movs r0, #15
	bl Battle_WaitMode0
	movs r2, #5
	movs r0, #8
	movs r1, #0
	bl Func_0200351c
	movs r1, #7
	movs r0, #8
	bl Object_SetModeById
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #6
	movs r2, #50
	adds r1, #255
	movs r0, #8
	bl Func_02003544
	movs r1, #1
	movs r0, #8
	bl Object_SetModeById
	movs r0, #15
	bl Battle_WaitMode0
	movs r2, #5
	movs r0, #8
	movs r1, #0
	bl Func_0200351c
	movs r1, #7
	movs r0, #8
	bl Object_SetModeById
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #128
	movs r2, #50
	lsls r1, r1, #1
	movs r0, #8
	bl Func_02003544
	movs r1, #1
	movs r0, #8
	bl Object_SetModeById
	movs r0, #15
	bl Battle_WaitMode0
	movs r2, #5
	movs r0, #8
	movs r1, #0
	bl Func_0200351c
	movs r1, #2
	movs r0, #8
	bl Motion_SetVarCbAndRefresh
	movs r0, #8
	bl Battle_WaitMode0
	movs r2, #5
	movs r1, #0
	movs r0, #8
	bl Func_0200351c
	movs r0, #0
	bl Func_0200067c
	movs r1, #128
	lsls r1, r1, #8
	movs r0, #8
	bl Func_02003534
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #8
	bl Func_02003544
	movs r1, #0
	movs r0, #8
	bl UiText_OpenMessageAtObject
	movs r0, #4
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_02008930
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #5
	movs r0, #8
	movs r1, #0
	bl Func_0200351c
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_02008952
.L_02008930:
	movs r0, #35
	bl Battle_WaitMode0
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #8
	adds r3, #1
	strh r3, [r2]
	movs r1, #0
	movs r2, #5
	bl Func_0200351c
.L_02008952:
	movs r0, #8
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r2, #5
	movs r0, #8
	movs r1, #0
	bl Func_0200351c
	movs r1, #160
	movs r0, #8
	lsls r1, r1, #7
	bl Func_02003534
	bl Func_02003464
	pop {r5, pc}
.L_02008974:
	.4byte 0x0000266e
	.section .text.x02008978,"ax",%progbits
	.global Func_02000978
	.thumb_func
Func_02000978:
	push {r5, lr}
	adds r5, r0, #0
	bl Func_0200345c
	movs r0, #0
	bl Func_020035b4
	ldr r0, .L_02008a30
	bl Func_0200350c
	adds r0, r5, #0
	bl Func_020005e8
	movs r1, #192
	movs r0, #8
	lsls r1, r1, #6
	bl Func_02003534
	movs r1, #7
	movs r0, #8
	bl Object_SetModeById
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #129
	movs r2, #50
	lsls r1, r1, #1
	movs r0, #8
	bl Func_02003544
	movs r1, #1
	movs r0, #8
	bl Object_SetModeById
	movs r0, #15
	bl Battle_WaitMode0
	movs r2, #5
	movs r0, #8
	movs r1, #0
	bl Func_0200351c
	movs r1, #7
	movs r0, #8
	bl Object_SetModeById
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #132
	movs r2, #50
	lsls r1, r1, #1
	movs r0, #8
	bl Func_02003544
	movs r1, #1
	movs r0, #8
	bl Object_SetModeById
	movs r0, #15
	bl Battle_WaitMode0
	movs r2, #5
	movs r1, #0
	movs r0, #8
	bl Func_0200351c
	movs r0, #0
	bl Func_0200067c
	movs r1, #128
	lsls r1, r1, #8
	movs r0, #8
	bl Func_02003534
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #5
	movs r0, #8
	movs r1, #0
	bl Func_0200351c
	movs r1, #160
	movs r0, #8
	lsls r1, r1, #7
	bl Func_02003534
	bl Func_02003464
	pop {r5, pc}
.L_02008a30:
	.4byte 0x00002678
	.section .text.x02008a34,"ax",%progbits
	.global Func_02000a34
	.thumb_func
Func_02000a34:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	adds r5, r0, #0
	movs r0, #10
	sub sp, #64
	bl Object_GetById
	ldr r3, .L_02008b58
	adds r7, r0, #0
	add r0, sp, #28
	str r3, [r0]
	movs r2, #160
	movs r3, #0
	str r3, [r0, #4]
	lsls r2, r2, #19
	movs r3, #238
	lsls r3, r3, #16
	adds r2, #228
	add r1, sp, #52
	str r3, [r0, #8]
	ldrh r3, [r2]
	mov r9, r1
	mov r4, r9
	strh r3, [r4]
	adds r2, #2
	ldrh r3, [r2]
	mov r11, r0
	mov r0, r9
	strh r3, [r0, #2]
	adds r2, #2
	ldrh r3, [r2]
	adds r2, #2
	strh r3, [r1, #4]
	add r6, sp, #40
	ldrh r3, [r2]
	strh r3, [r4, #6]
	ldrh r3, [r2, #2]
	movs r2, #160
	strh r3, [r0, #8]
	lsls r2, r2, #19
	adds r2, #196
	ldrh r3, [r2]
	adds r2, #2
	strh r3, [r6]
	ldrh r3, [r2]
	adds r2, #2
	strh r3, [r6, #2]
	ldrh r3, [r2]
	adds r2, #2
	strh r3, [r6, #4]
	ldrh r3, [r2]
	strh r3, [r6, #6]
	ldrh r3, [r2, #2]
	strh r3, [r6, #8]
	bl Func_0200345c
	movs r0, #0
	bl Func_020035b4
	ldr r0, .L_02008b5c
	bl Func_0200350c
	adds r0, r5, #0
	bl Func_020005e8
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #8
	bl Func_02003544
	movs r0, #8
	movs r1, #0
	movs r2, #5
	bl Func_0200351c
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #8
	bl Func_02003544
	movs r1, #0
	movs r2, #5
	movs r0, #8
	bl Func_0200351c
	movs r0, #1
	bl Func_0200067c
	movs r1, #166
	movs r2, #200
	movs r0, #8
	lsls r1, r1, #2
	bl ObjectMotion_SetPositionAndReset
	movs r1, #192
	lsls r1, r1, #6
	movs r0, #8
	bl Func_02003534
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #8
	movs r0, #8
	bl Func_02003534
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #0
	movs r0, #8
	bl UiText_OpenMessageAtObject
	movs r0, #4
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_02008b60
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #0
	movs r2, #5
	movs r0, #8
	bl Func_0200351c
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r1, #226
	lsls r1, r1, #1
	adds r2, r2, r1
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_02008b82
	.2byte 0x0000
.L_02008b58:
	.4byte 0x02960000
.L_02008b5c:
	.4byte 0x0000267d
.L_02008b60:
	movs r0, #40
	bl Battle_WaitMode0
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #8
	adds r3, #1
	strh r3, [r2]
	movs r1, #0
	movs r2, #5
	bl Func_0200351c
.L_02008b82:
	ldr r3, .L_02008f3c
	movs r4, #133
	lsls r4, r4, #2
	adds r3, r3, r4
	movs r1, #128
	ldr r0, [r3]
	movs r2, #0
	lsls r1, r1, #6
	bl ObjectMotion_ArmCallback
	movs r1, #160
	lsls r1, r1, #7
	movs r0, #8
	bl Func_02003534
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #166
	movs r1, #1
	movs r2, #232
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #16
	lsls r0, r0, #18
	bl Motion_CamBounds
	bl Func_02003564
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #8
	adds r1, #255
	movs r2, #40
	movs r0, #8
	bl Func_02003544
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #8
	movs r1, #0
	movs r2, #5
	bl Func_0200351c
	movs r0, #0
	mov r8, r0
.L_02008be0:
	ldr r3, .L_02008f40
	add r5, sp, #16
	adds r2, r5, #0
	ldmia r3!, {r0, r1, r4}
	stmia r2!, {r0, r1, r4}
	movs r1, #9
	movs r0, #8
	bl Object_SetModeById
	movs r0, #3
	bl Battle_WaitMode0
	mov r1, r8
	lsls r3, r1, #2
	ldr r1, [r5, r3]
	movs r0, #10
	bl Func_0200361c
	movs r1, #167
	movs r2, #200
	movs r0, #10
	lsls r1, r1, #18
	lsls r2, r2, #16
	bl Func_020034d4
	movs r3, #128
	lsls r3, r3, #7
	str r3, [r7, #72]
	movs r2, #2
	movs r0, #10
	movs r1, #4
	bl ObjectMotion_Launch
	movs r0, #10
	movs r1, #1
	bl ObjectMotion_SetActionVariant
	movs r1, #128
	movs r2, #128
	movs r0, #10
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r1, #0
	movs r2, #40
	movs r0, #10
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r0, #155
	lsls r0, r0, #1
	bl Func_0200362c
	movs r0, #34
	bl Battle_WaitMode0
	ldr r3, [r7, #8]
	add r2, sp, #4
	str r3, [r2]
	ldr r3, [r7, #12]
	movs r1, #0
	str r3, [r2, #4]
	ldr r3, [r7, #16]
	movs r0, #10
	str r3, [r2, #8]
	movs r2, #0
	bl Func_020034d4
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #134
	bl Func_0200362c
	mov r0, r9
	adds r1, r6, #0
	movs r2, #12
	bl Func_02000734
	movs r2, #48
	adds r0, r6, #0
	mov r1, r9
	bl Func_02000734
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #1
	add r8, r2
	mov r3, r8
	cmp r3, #2
	ble .L_02008be0
	ldr r5, .L_02008f3c
	movs r4, #133
	lsls r4, r4, #2
	adds r5, r5, r4
	ldr r0, [r5]
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r1, #254
	mov r10, r1
	mov r3, r10
	ands r3, r2
	strb r3, [r0]
	movs r1, #12
	movs r2, #12
	ldr r0, [r5]
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #1
	bl Battle_WaitMode0
	ldr r0, [r5]
	bl Object_GetById
	adds r0, #90
	movs r2, #1
	ldrb r3, [r0]
	mov r8, r2
	mov r4, r8
	orrs r3, r4
	strb r3, [r0]
	movs r1, #128
	ldr r0, [r5]
	lsls r1, r1, #6
	bl Func_02003534
	movs r0, #10
	bl Battle_WaitMode0
	ldr r0, [r5]
	movs r1, #22
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #7
	movs r0, #8
	bl Object_SetModeById
	movs r0, #60
	bl Battle_WaitMode0
	movs r0, #78
	bl Func_0200362c
	movs r1, #2
	adds r1, #255
	movs r2, #60
	movs r0, #8
	bl Func_02003544
	movs r0, #128
	lsls r0, r0, #6
	movs r1, #0
	movs r2, #5
	adds r0, #8
	bl Func_0200351c
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #220
	bl Func_0200362c
	movs r2, #48
	mov r0, r9
	adds r1, r6, #0
	bl Func_02000734
	ldr r0, [r5]
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	ldr r0, [r5]
	lsls r1, r1, #1
	bl Func_0200354c
	movs r0, #50
	bl Battle_WaitMode0
	movs r1, #2
	movs r2, #60
	adds r1, #255
	movs r0, #8
	bl Func_02003544
	movs r1, #128
	lsls r1, r1, #5
	movs r0, #16
	adds r1, #144
	bl Func_0200360c
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #9
	lsls r1, r1, #10
	lsls r0, r0, #10
	bl Func_02003434
	mov r0, r11
	bl Func_02003164
	movs r0, #8
	movs r1, #1
	bl Object_SetModeById
	movs r1, #6
	movs r2, #0
	movs r0, #4
	bl ObjectMotion_Launch
	movs r0, #194
	bl Func_0200362c
	movs r0, #4
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	mov r1, r10
	adds r2, r1, #0
	ands r2, r3
	strb r2, [r0]
	movs r1, #12
	movs r2, #12
	negs r2, r2
	negs r1, r1
	movs r0, #4
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #1
	bl Battle_WaitMode0
	movs r0, #4
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	mov r4, r8
	orrs r4, r3
	strb r4, [r0]
	movs r0, #10
	mov r8, r4
	bl Battle_WaitMode0
	movs r0, #8
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #8
	bl Func_0200354c
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #8
	movs r1, #0
	movs r2, #5
	bl Func_0200351c
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #11
	lsls r2, r2, #9
	lsls r0, r0, #11
	bl Func_02003434
	mov r0, r11
	bl Func_0200320c
	movs r0, #194
	bl Func_0200362c
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #8
	movs r1, #6
	movs r2, #15
	bl ObjectMotion_Launch
	movs r1, #6
	movs r2, #23
	movs r0, #8
	bl ObjectMotion_Launch
	movs r0, #194
	bl Func_0200362c
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	movs r2, #5
	adds r0, #8
	movs r1, #0
	bl Func_0200351c
	movs r1, #163
	movs r0, #10
	lsls r1, r1, #1
	bl Func_0200361c
	movs r1, #166
	movs r2, #232
	lsls r2, r2, #16
	movs r0, #10
	lsls r1, r1, #18
	bl Func_020034d4
	adds r5, r7, #0
	movs r0, #10
	movs r1, #1
	bl ObjectMotion_SetActionVariant
	adds r5, #85
	movs r3, #2
	strb r3, [r5]
	movs r3, #192
	lsls r3, r3, #13
	str r3, [r7, #12]
	ldr r3, .L_02008f44
	movs r0, #0
	str r3, [r7, #108]
	movs r1, #5
	movs r2, #0
	str r0, [r7, #20]
	str r0, [r7, #24]
	str r0, [r7, #28]
	movs r0, #10
	bl ObjectMotion_Launch
	movs r0, #8
	bl Field_BeginPaletteTransition
	bl Func_020032a4
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	negs r0, r0
	negs r1, r1
	adds r2, #102
	bl Func_02003434
	ldr r3, [r7, #12]
	cmp r3, #0
	ble .L_02008ebe
.L_02008eb2:
	movs r0, #1
	bl WaitFrames
	ldr r3, [r7, #12]
	cmp r3, #0
	bgt .L_02008eb2
.L_02008ebe:
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r7, #72]
	movs r3, #0
	str r3, [r7, #12]
	movs r3, #4
	strb r3, [r5]
	bl Func_020035bc
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #8
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	movs r2, #5
	adds r0, #8
	movs r1, #0
	bl Func_0200351c
	movs r1, #128
	lsls r1, r1, #8
	movs r0, #8
	bl Func_02003534
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	movs r2, #5
	adds r0, #8
	movs r1, #0
	bl Func_0200351c
	movs r1, #160
	lsls r1, r1, #7
	movs r0, #8
	bl Func_02003534
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #10
	movs r1, #2
	bl ObjectMotion_SetActionVariant
	bl Func_02003464
	add sp, #64
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008f3c:
	.4byte gPartyState
.L_02008f40:
	.4byte Data_02003634
.L_02008f44:
	.4byte Func_02000038
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
	bl Object_GetById
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
	bl ObjectMotion_SetAngleToward
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
	bl ObjectMotion_ArmCallback
.L_0200911c:
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, pc}
.L_02009124:
	.4byte gPartyState
.L_02009128:
	.4byte 0x0000266c
.L_0200912c:
	.4byte 0x00002676
.L_02009130:
	.4byte 0x0000267b
.L_02009134:
	.4byte 0x00002688
	.section .text.x02009138,"ax",%progbits
	.global Func_02001138
	.thumb_func
Func_02001138:
	push {lr}
	ldr r3, [r0, #24]
	movs r2, #192
	lsls r2, r2, #4
	adds r3, r3, r2
	movs r2, #128
	lsls r2, r2, #9
	str r3, [r0, #24]
	cmp r3, r2
	blt .L_02009156
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r0, #24]
	movs r3, #0
	str r3, [r0, #108]
.L_02009156:
	ldr r3, [r0, #24]
	str r3, [r0, #28]
	pop {pc}
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
	bl Object_GetById
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
	bl ObjectMotion_SetPositionAndReset
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
	bl Motion_CamBounds
	bl Func_02003564
	movs r0, #5
	bl Battle_WaitMode0
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
	bl ObjectMotion_ArmCallback
	movs r0, #166
	movs r1, #1
	movs r2, #128
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #17
	lsls r0, r0, #18
	bl Motion_CamBounds
	bl Func_02003564
	movs r0, #5
	bl Battle_WaitMode0
	movs r0, #11
	movs r1, #6
	movs r2, #15
	bl ObjectMotion_Launch
	movs r0, #11
	movs r1, #6
	movs r2, #23
	bl ObjectMotion_Launch
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
	bl Battle_WaitMode0
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
	bl Battle_WaitMode0
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
	bl Battle_WaitMode0
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
	bl Battle_WaitMode0
	movs r1, #176
	lsls r1, r1, #8
	movs r0, #11
	bl Func_02003534
	movs r0, #10
	bl Battle_WaitMode0
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
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #11
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl Battle_WaitMode0
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
	bl Battle_WaitMode0
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
	bl Battle_WaitMode0
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
	bl Battle_WaitMode0
	movs r2, #5
	movs r0, #8
	movs r1, #0
	bl Func_0200351c
	movs r1, #176
	lsls r1, r1, #8
	movs r0, #11
	bl Func_02003534
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #4
	movs r0, #8
	bl Motion_SetModeAndWaitAnimation
	movs r0, #5
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #8
	movs r0, #8
	bl Func_02003534
	movs r0, #5
	bl Battle_WaitMode0
	movs r2, #5
	movs r0, #8
	movs r1, #0
	bl Func_0200351c
	movs r1, #3
	movs r0, #11
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #11
	bl Motion_SetModeAndWaitAnimation
	movs r0, #5
	bl Battle_WaitMode0
	movs r0, #11
	movs r1, #0
	movs r2, #5
	bl Func_0200351c
	movs r1, #128
	movs r0, #11
	lsls r1, r1, #8
	bl Func_02003534
	movs r0, #5
	bl Battle_WaitMode0
.L_02009466:
	ldr r0, .L_02009550
	bl Func_0200350c
	movs r1, #176
	lsls r1, r1, #8
	movs r0, #8
	bl Func_02003534
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #8
	bl Object_GetById
	movs r3, #0
	mov r10, r3
	movs r3, #160
	lsls r3, r3, #7
	strh r3, [r0, #6]
	movs r1, #5
	movs r0, #8
	bl Object_SetModeById
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #8
	movs r1, #6
	bl Object_SetModeById
	movs r1, #181
	movs r2, #252
	lsls r2, r2, #16
	lsls r1, r1, #18
	movs r0, #12
	bl Func_020034d4
	movs r0, #12
	bl Object_GetById
	ldr r5, .L_02009554
	str r5, [r0, #24]
	movs r0, #12
	bl Object_GetById
	str r5, [r0, #28]
	movs r0, #12
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #128
	lsls r0, r0, #6
	movs r1, #0
	adds r0, #8
	bl UiText_OpenMessageAtObject
	movs r0, #4
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_02009558
	bl Func_020035bc
	movs r0, #15
	bl Battle_WaitMode0
	movs r2, #0
	movs r0, #12
	movs r1, #0
	bl Func_020034d4
	movs r1, #1
	movs r0, #8
	bl Object_SetModeById
	movs r0, #8
	bl Object_GetById
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
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #5
	bl Battle_WaitMode0
	b .L_02009746
	.2byte 0x0000
.L_02009544:
	.4byte gPartyState
.L_02009548:
	.4byte 0x00002624
.L_0200954c:
	.4byte 0x0000260f
.L_02009550:
	.4byte 0x0000261e
.L_02009554:
	.4byte 0x00013333
.L_02009558:
	movs r0, #25
	bl Battle_WaitMode0
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
	bl ObjectMotion_Launch
	movs r0, #146
	bl Func_0200362c
	movs r0, #12
	movs r1, #1
	bl ObjectMotion_SetActionVariant
	movs r1, #192
	movs r2, #192
	movs r0, #12
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl ObjectMotion_SetSpeedParameters
	movs r1, #161
	lsls r1, r1, #2
	movs r2, #248
	movs r0, #12
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r0, #21
	bl Battle_WaitMode0
	movs r1, #0
	movs r2, #0
	movs r0, #12
	bl Func_020034d4
	movs r0, #50
	bl Battle_WaitMode0
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #10
	lsls r2, r2, #9
	lsls r0, r0, #10
	bl Func_02003434
	movs r0, #70
	bl Battle_WaitMode0
	movs r0, #148
	bl Func_0200362c
	movs r0, #25
	bl Battle_WaitMode0
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
	bl Battle_WaitMode0
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
	bl ObjectMotion_Launch
	movs r0, #147
	bl Func_0200362c
	movs r0, #13
	ldr r1, .L_02009764
	ldr r2, .L_02009768
	bl ObjectMotion_SetSpeedParameters
	movs r1, #146
	movs r2, #248
	movs r0, #13
	lsls r1, r1, #2
	bl ObjectMotion_SetPositionAndReset
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
	.4byte gPartyState
	.section .text.x02009774,"ax",%progbits
	.global Func_02001774
	.thumb_func
Func_02001774:
	push {r5, r6, lr}
	bl Func_0200345c
	movs r0, #0
	bl Func_020035b4
	ldr r0, .L_02009860
	bl Func_0200350c
	ldr r6, .L_02009864
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r6, r2
	movs r1, #142
	movs r2, #248
	ldr r0, [r5]
	lsls r1, r1, #18
	lsls r2, r2, #16
	bl Func_020034d4
	movs r1, #146
	movs r2, #224
	movs r0, #8
	lsls r1, r1, #18
	lsls r2, r2, #16
	bl Func_020034d4
	movs r1, #150
	movs r2, #240
	movs r0, #13
	lsls r1, r1, #18
	lsls r2, r2, #16
	bl Func_020034d4
	movs r1, #146
	movs r2, #132
	movs r0, #11
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl Func_020034d4
	movs r1, #160
	movs r0, #8
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #176
	movs r2, #0
	movs r0, #11
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	ldr r0, [r5]
	movs r1, #41
	bl Object_SetModeById
	ldr r0, [r5]
	movs r1, #0
	bl Object_AttachWorkTargetToObject
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #11
	bl Func_02003544
	movs r2, #5
	movs r0, #11
	movs r1, #0
	bl Func_0200351c
	movs r0, #8
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #8
	movs r1, #0
	movs r2, #20
	bl Func_0200351c
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #108]
	movs r3, #218
	lsls r3, r3, #1
	adds r2, r1, r3
	movs r3, #40
	str r3, [r2]
	movs r3, #214
	lsls r3, r3, #1
	adds r2, r1, r3
	adds r3, #93
	str r3, [r2]
	bl Event_ClearStatus1c6
	bl Event_WaitValue1c8Frames
	movs r2, #242
	lsls r2, r2, #1
	adds r3, r6, r2
	movs r2, #0
	ldrsh r0, [r3, r2]
	movs r2, #243
	lsls r2, r2, #1
	adds r3, r6, r2
	movs r2, #0
	ldrsh r1, [r3, r2]
	bl Func_0200356c
	pop {r5, r6, pc}
.L_02009860:
	.4byte 0x00002622
.L_02009864:
	.4byte gPartyState
	.section .text.x02009868,"ax",%progbits
	.global Func_02001868
	.thumb_func
Func_02001868:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r8
	push {r5, r6}
	bl Func_0200345c
	movs r0, #0
	bl Func_020035b4
	ldr r0, .L_02009c58
	bl Func_0200350c
	ldr r5, .L_02009c5c
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
	movs r1, #142
	movs r2, #128
	ldr r0, [r5]
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl Func_020034d4
	movs r1, #148
	movs r2, #240
	lsls r2, r2, #16
	movs r0, #13
	lsls r1, r1, #18
	bl Func_020034d4
	movs r0, #13
	movs r1, #3
	bl Object_SetModeById
	ldr r0, [r5]
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #164
	movs r1, #1
	movs r2, #248
	movs r3, #0
	negs r1, r1
	lsls r2, r2, #16
	lsls r0, r0, #18
	bl Motion_CamBounds
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #5
	movs r0, #8
	movs r1, #0
	bl Func_0200351c
	movs r1, #2
	movs r0, #13
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl Battle_WaitMode0
	movs r0, #13
	movs r1, #1
	bl Func_020029f8
	movs r1, #128
	movs r2, #128
	movs r0, #8
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r1, #12
	movs r2, #18
	movs r0, #8
	negs r1, r1
	negs r2, r2
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #10
	movs r2, #14
	movs r0, #8
	negs r1, r1
	negs r2, r2
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #18
	movs r2, #12
	movs r0, #8
	negs r1, r1
	negs r2, r2
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #166
	movs r2, #200
	movs r0, #8
	lsls r1, r1, #2
	bl ObjectMotion_SetPositionAndReset
	movs r1, #160
	lsls r1, r1, #7
	movs r0, #8
	bl Func_02003534
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #8
	movs r1, #0
	movs r2, #5
	bl Func_0200351c
	movs r1, #16
	movs r0, #8
	negs r1, r1
	movs r2, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #16
	movs r0, #8
	negs r1, r1
	movs r2, #8
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #16
	movs r0, #8
	negs r1, r1
	movs r2, #16
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #8
	movs r0, #8
	negs r1, r1
	movs r2, #16
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #152
	movs r2, #248
	movs r0, #8
	lsls r1, r1, #2
	bl ObjectMotion_SetPositionAndReset
	movs r1, #128
	lsls r1, r1, #8
	movs r0, #8
	bl Func_02003534
	movs r0, #5
	bl Battle_WaitMode0
	movs r0, #8
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #8
	movs r1, #0
	movs r2, #5
	bl Func_0200351c
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #40
	movs r0, #11
	bl Func_02003544
	movs r2, #217
	lsls r2, r2, #8
	movs r0, #11
	ldr r1, .L_02009c60
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r1, #16
	negs r1, r1
	movs r2, #0
	movs r0, #11
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #5
	bl Battle_WaitMode0
	movs r2, #5
	movs r0, #11
	movs r1, #0
	bl Func_0200351c
	movs r1, #0
	movs r0, #8
	bl Func_02003534
	movs r0, #5
	bl Battle_WaitMode0
	movs r0, #8
	movs r1, #0
	movs r2, #5
	bl Func_0200351c
	movs r0, #11
	movs r1, #6
	movs r2, #15
	bl ObjectMotion_Launch
	movs r0, #11
	movs r1, #6
	movs r2, #23
	bl ObjectMotion_Launch
	movs r0, #11
	movs r1, #0
	movs r2, #5
	bl Func_0200351c
	movs r1, #8
	adds r1, #255
	movs r2, #30
	movs r0, #8
	bl Func_02003544
	movs r2, #5
	movs r0, #8
	movs r1, #0
	bl Func_0200351c
	movs r0, #11
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #11
	bl Func_0200354c
	movs r0, #38
	bl Battle_WaitMode0
	movs r2, #5
	movs r0, #11
	movs r1, #0
	bl Func_0200351c
	movs r0, #8
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r2, #5
	movs r0, #8
	movs r1, #0
	bl Func_0200351c
	movs r0, #11
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #11
	bl Func_0200354c
	movs r0, #37
	bl Battle_WaitMode0
	movs r2, #5
	movs r0, #11
	movs r1, #0
	bl Func_0200351c
	movs r1, #254
	lsls r1, r1, #7
	adds r1, #246
	movs r0, #8
	bl Func_02003534
	movs r0, #25
	bl Battle_WaitMode0
	movs r1, #0
	movs r0, #8
	bl Func_02003534
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #5
	movs r0, #8
	movs r1, #0
	bl Func_0200351c
	movs r1, #2
	movs r0, #11
	bl Motion_SetVarCbAndRefresh
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #131
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #11
	bl Func_02003544
	movs r0, #11
	movs r1, #0
	movs r2, #10
	bl Func_0200351c
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #40
	movs r0, #8
	bl Func_02003544
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl Func_0200351c
	movs r2, #5
	movs r0, #15
	movs r1, #0
	bl Func_0200351c
	movs r1, #254
	lsls r1, r1, #7
	adds r1, #246
	movs r0, #8
	bl Func_02003534
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #2
	adds r1, #255
	movs r2, #40
	movs r0, #8
	bl Func_02003544
	movs r2, #16
	movs r0, #7
	movs r1, #0
	negs r2, r2
	movs r3, #0
	bl Func_020035cc
	movs r1, #16
	movs r0, #5
	negs r1, r1
	movs r2, #0
	movs r3, #0
	bl Func_020035cc
	movs r1, #16
	movs r2, #16
	movs r0, #6
	negs r1, r1
	negs r2, r2
	movs r3, #0
	bl Func_020035cc
	movs r2, #16
	movs r1, #16
	negs r2, r2
	movs r3, #0
	movs r0, #15
	bl Func_020035cc
	movs r0, #15
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #160
	movs r1, #1
	movs r2, #248
	movs r3, #1
	lsls r2, r2, #16
	negs r1, r1
	lsls r0, r0, #18
	bl Motion_CamBounds
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #6
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #128
	lsls r0, r0, #5
	movs r2, #5
	adds r0, #6
	movs r1, #0
	bl Func_0200351c
	movs r0, #8
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	movs r0, #8
	lsls r1, r1, #1
	bl Func_0200354c
	movs r0, #11
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #11
	bl Func_0200354c
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	movs r2, #5
	adds r0, #8
	movs r1, #0
	bl Func_0200351c
	movs r1, #3
	movs r0, #5
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #5
	movs r2, #5
	adds r0, #5
	movs r1, #0
	bl Func_0200351c
	movs r1, #0
	movs r0, #8
	bl Func_02003534
	movs r0, #5
	bl Battle_WaitMode0
	movs r2, #5
	movs r0, #8
	movs r1, #0
	bl Func_0200351c
	movs r0, #11
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #11
	bl Func_0200354c
	movs r0, #35
	bl Battle_WaitMode0
	movs r2, #0
	movs r0, #11
	movs r1, #0
	bl Func_0200351c
	movs r0, #7
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r2, #5
	movs r0, #7
	movs r1, #0
	bl Func_0200351c
	movs r1, #160
	lsls r1, r1, #7
	movs r0, #15
	bl Func_02003534
	movs r0, #5
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	movs r1, #0
	adds r0, #15
	bl UiText_OpenMessageAtObject
	movs r0, #4
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_02009caa
	b .L_02009c64
.L_02009c58:
	.4byte 0x00002627
.L_02009c5c:
	.4byte gPartyState
.L_02009c60:
	.4byte 0x0001b333
.L_02009c64:
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #6
	movs r0, #6
	bl Func_02003534
	movs r0, #5
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #6
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #5
	movs r2, #5
	adds r0, #6
	movs r1, #0
	bl Func_0200351c
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_02009ce8
.L_02009caa:
	movs r0, #35
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #6
	movs r0, #6
	bl Func_02003534
	movs r0, #5
	bl Battle_WaitMode0
	movs r0, #6
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #128
	adds r3, #1
	lsls r0, r0, #5
	strh r3, [r2]
	adds r0, #6
	movs r1, #0
	movs r2, #5
	bl Func_0200351c
.L_02009ce8:
	movs r2, #0
	movs r0, #15
	movs r1, #0
	bl ObjectMotion_ArmCallback
	movs r0, #6
	movs r1, #0
	bl Func_02003534
	movs r0, #8
	movs r1, #0
	movs r2, #5
	bl Func_0200351c
	movs r1, #179
	movs r2, #179
	lsls r1, r1, #10
	lsls r2, r2, #9
	movs r0, #8
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	ldr r2, .L_02009f8c
	movs r0, #11
	ldr r1, .L_02009f90
	bl ObjectMotion_SetSpeedParameters
	movs r0, #11
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	movs r0, #11
	lsls r1, r1, #1
	bl Func_0200354c
	ldr r5, .L_02009f94
	movs r0, #8
	adds r1, r5, #0
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #166
	movs r2, #145
	lsls r1, r1, #2
	lsls r2, r2, #1
	movs r0, #11
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r0, #8
	bl Object_RefreshSelectorById
	ldr r2, .L_02009f98
	movs r0, #8
	mov r10, r2
	mov r1, r10
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #11
	bl ObjectMotion_CommitCurrentPositionAndActivate
	ldr r6, .L_02009f9c
	movs r0, #11
	adds r1, r6, #0
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #8
	bl Object_RefreshSelectorById
	ldr r3, .L_02009fa0
	movs r0, #8
	mov r8, r3
	mov r1, r8
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #11
	bl Object_RefreshSelectorById
	adds r1, r5, #0
	movs r0, #11
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #8
	bl Object_RefreshSelectorById
	movs r1, #176
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #8
	bl ObjectMotion_ArmCallback
	movs r0, #11
	bl Object_RefreshSelectorById
	movs r1, #192
	movs r0, #11
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r2, #5
	movs r0, #11
	movs r1, #0
	bl Func_0200351c
	movs r0, #8
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #8
	movs r1, #0
	movs r2, #5
	bl Func_0200351c
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #11
	bl Func_02003544
	movs r0, #11
	movs r1, #0
	movs r2, #5
	bl Func_0200351c
	movs r1, #4
	adds r1, #255
	movs r2, #30
	movs r0, #8
	bl Func_02003544
	movs r2, #5
	movs r0, #8
	movs r1, #0
	bl Func_0200351c
	movs r0, #11
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	movs r0, #11
	lsls r1, r1, #1
	bl Func_0200354c
	adds r1, r6, #0
	movs r0, #8
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #11
	bl ObjectMotion_CommitCurrentPositionAndActivate
	mov r1, r10
	movs r0, #11
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #8
	bl Object_RefreshSelectorById
	adds r1, r5, #0
	movs r0, #8
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #11
	bl Object_RefreshSelectorById
	mov r1, r8
	movs r0, #11
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #8
	bl Object_RefreshSelectorById
	movs r1, #192
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #8
	bl ObjectMotion_ArmCallback
	movs r0, #11
	bl Object_RefreshSelectorById
	movs r1, #176
	movs r2, #0
	movs r0, #11
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r0, #11
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r2, #5
	movs r0, #11
	movs r1, #0
	bl Func_0200351c
	movs r0, #8
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #8
	movs r1, #0
	movs r2, #5
	bl Func_0200351c
	movs r2, #5
	movs r0, #14
	movs r1, #0
	bl Func_0200351c
	movs r0, #4
	movs r1, #3
	bl ObjectMotion_SetActionVariant
	movs r0, #5
	movs r1, #3
	bl ObjectMotion_SetActionVariant
	movs r0, #6
	movs r1, #3
	bl ObjectMotion_SetActionVariant
	movs r0, #7
	movs r1, #3
	bl ObjectMotion_SetActionVariant
	movs r1, #160
	movs r0, #4
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	movs r0, #7
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #15
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r2, #0
	movs r0, #11
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r1, #128
	lsls r1, r1, #8
	movs r0, #8
	bl Func_02003534
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #142
	movs r1, #1
	movs r2, #248
	movs r3, #1
	lsls r2, r2, #16
	negs r1, r1
	lsls r0, r0, #18
	bl Motion_CamBounds
	bl Func_02003564
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #14
	bl ObjectMotion_SetActionVariant
	movs r0, #14
	bl Object_GetById
	movs r6, #192
	lsls r6, r6, #6
	strh r6, [r0, #6]
	movs r0, #14
	bl Object_GetById
	ldr r5, .L_02009f88
	adds r0, #85
	movs r1, #130
	movs r2, #200
	strb r5, [r0]
	lsls r1, r1, #18
	movs r0, #14
	lsls r2, r2, #16
	bl Func_020034d4
	movs r1, #128
	movs r2, #128
	movs r0, #14
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r0, #14
	movs r1, #0
	movs r2, #16
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #14
	adds r1, r6, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r2, #5
	movs r0, #11
	movs r1, #0
	b .L_02009fa4
	.2byte 0x0000
.L_02009f88:
	.4byte 0x00000000
.L_02009f8c:
	.4byte 0x0001b333
.L_02009f90:
	.4byte 0x00036666
.L_02009f94:
	.4byte Data_020041e8
.L_02009f98:
	.4byte Data_02004254
.L_02009f9c:
	.4byte Data_0200432c
.L_02009fa0:
	.4byte Data_020042d4
.L_02009fa4:
	bl Func_0200351c
	movs r0, #4
	movs r1, #14
	bl Object_LinkObjectAndSetCallback
	movs r0, #7
	movs r1, #14
	bl Object_LinkObjectAndSetCallback
	movs r0, #5
	movs r1, #14
	bl Object_LinkObjectAndSetCallback
	movs r0, #6
	movs r1, #14
	bl Object_LinkObjectAndSetCallback
	movs r0, #15
	movs r1, #14
	bl Object_LinkObjectAndSetCallback
	movs r0, #8
	movs r1, #14
	bl Object_LinkObjectAndSetCallback
	movs r2, #16
	movs r0, #14
	movs r1, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #14
	movs r1, #3
	bl ObjectMotion_SetActionVariant
	movs r0, #14
	movs r1, #0
	movs r2, #32
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #14
	movs r1, #8
	movs r2, #8
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #14
	movs r1, #64
	movs r2, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #160
	movs r0, #8
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #150
	movs r1, #1
	movs r2, #128
	movs r3, #1
	lsls r0, r0, #18
	negs r1, r1
	lsls r2, r2, #17
	bl Motion_CamBounds
	movs r1, #128
	movs r2, #128
	movs r0, #11
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r1, #32
	movs r0, #11
	negs r1, r1
	movs r2, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #16
	movs r2, #16
	negs r2, r2
	movs r0, #11
	negs r1, r1
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #128
	movs r0, #11
	lsls r1, r1, #8
	bl Func_02003534
	movs r1, #3
	movs r0, #14
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #11
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #5
	movs r0, #11
	movs r1, #0
	bl Func_0200351c
	movs r1, #176
	lsls r1, r1, #8
	movs r0, #14
	bl Func_02003534
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	movs r2, #5
	adds r0, #14
	movs r1, #0
	bl Func_0200351c
	movs r1, #2
	movs r0, #8
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #128
	movs r2, #128
	movs r0, #8
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r1, #16
	movs r0, #8
	negs r1, r1
	movs r2, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #16
	movs r0, #8
	negs r1, r1
	movs r2, #8
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #16
	movs r0, #8
	negs r1, r1
	movs r2, #16
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #8
	movs r2, #8
	movs r0, #8
	negs r1, r1
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #160
	movs r0, #8
	lsls r1, r1, #7
	bl Func_02003534
	movs r0, #8
	movs r1, #0
	movs r2, #5
	bl Func_0200351c
	movs r1, #176
	movs r2, #0
	movs r0, #11
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r1, #208
	lsls r1, r1, #8
	movs r0, #14
	bl Func_02003534
	movs r0, #15
	bl Battle_WaitMode0
	movs r0, #8
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r2, #5
	movs r0, #8
	movs r1, #0
	bl Func_0200351c
	movs r0, #14
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #14
	movs r1, #0
	movs r2, #5
	bl Func_0200351c
	movs r1, #10
	adds r1, #255
	movs r2, #30
	movs r0, #8
	bl Func_02003544
	movs r2, #5
	movs r0, #8
	movs r1, #0
	bl Func_0200351c
	movs r0, #11
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r1, #0
	movs r0, #11
	bl UiText_OpenMessageAtObject
	movs r1, #224
	movs r0, #4
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	movs r0, #8
	lsls r1, r1, #7
	bl Func_02003534
	movs r0, #4
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_0200a1dc
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #8
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl Battle_WaitMode0
	movs r2, #5
	movs r0, #8
	movs r1, #0
	bl Func_0200351c
	movs r1, #128
	lsls r1, r1, #8
	movs r0, #11
	bl Func_02003534
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #11
	bl Func_02003544
	movs r2, #5
	movs r0, #11
	movs r1, #0
	bl Func_0200351c
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #2
	strh r3, [r2]
	b .L_0200a238
.L_0200a1dc:
	movs r0, #35
	bl Battle_WaitMode0
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #8
	adds r3, #2
	strh r3, [r2]
	movs r1, #0
	movs r2, #5
	bl Func_0200351c
	movs r1, #128
	lsls r1, r1, #8
	movs r0, #11
	bl Func_02003534
	movs r0, #5
	bl Battle_WaitMode0
	movs r1, #8
	adds r1, #255
	movs r2, #30
	movs r0, #11
	bl Func_02003544
	movs r0, #11
	movs r1, #0
	movs r2, #5
	bl Func_0200351c
	movs r0, #11
	movs r1, #6
	movs r2, #15
	bl ObjectMotion_Launch
	movs r0, #11
	movs r1, #6
	movs r2, #23
	bl ObjectMotion_Launch
.L_0200a238:
	movs r1, #128
	lsls r1, r1, #6
	movs r0, #4
	bl Func_02003534
	movs r0, #15
	bl Battle_WaitMode0
	movs r1, #10
	adds r1, #255
	movs r2, #30
	movs r0, #14
	bl Func_02003544
	movs r2, #5
	movs r0, #14
	movs r1, #0
	bl Func_0200351c
	movs r1, #176
	lsls r1, r1, #8
	movs r0, #14
	bl Func_02003534
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #14
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #14
	movs r1, #0
	movs r2, #5
	bl Func_0200351c
	movs r1, #129
	lsls r1, r1, #1
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
	bl Battle_WaitMode0
	movs r2, #5
	movs r0, #11
	movs r1, #0
	bl Func_0200351c
	movs r1, #208
	lsls r1, r1, #8
	movs r0, #14
	bl Func_02003534
	movs r0, #5
	bl Battle_WaitMode0
	movs r2, #5
	movs r0, #14
	movs r1, #0
	bl Func_0200351c
	movs r1, #0
	movs r0, #14
	bl Func_02003534
	movs r0, #5
	bl Battle_WaitMode0
	movs r0, #14
	movs r1, #0
	movs r2, #5
	bl Func_0200351c
	movs r1, #6
	adds r1, #255
	movs r2, #60
	movs r0, #11
	bl Func_02003544
	movs r1, #2
	adds r1, #255
	movs r2, #30
	movs r0, #14
	bl Func_02003544
	movs r2, #5
	movs r0, #14
	movs r1, #0
	bl Func_0200351c
	movs r0, #11
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #11
	bl Func_0200354c
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #8
	movs r0, #11
	bl Func_02003534
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #11
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #5
	movs r0, #11
	movs r1, #0
	bl Func_0200351c
	movs r1, #3
	movs r0, #14
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #208
	lsls r1, r1, #8
	movs r0, #14
	bl Func_02003534
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #14
	movs r1, #0
	movs r2, #5
	bl Func_0200351c
	movs r1, #6
	adds r1, #255
	movs r2, #50
	movs r0, #8
	bl Func_02003544
	movs r2, #5
	movs r0, #8
	movs r1, #0
	bl Func_0200351c
	movs r1, #176
	lsls r1, r1, #8
	movs r0, #14
	bl Func_02003534
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	movs r1, #0
	adds r0, #14
	bl UiText_OpenMessageAtObject
	movs r1, #160
	movs r0, #8
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #11
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #176
	movs r0, #14
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #7
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #6
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	movs r0, #15
	lsls r1, r1, #7
	bl Func_02003534
	movs r0, #4
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_0200a43a
	movs r0, #15
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #15
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	movs r2, #5
	adds r0, #15
	movs r1, #0
	bl Func_0200351c
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_0200a46e
.L_0200a43a:
	movs r0, #35
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #15
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
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
	adds r0, #15
	movs r1, #0
	movs r2, #5
	bl Func_0200351c
.L_0200a46e:
	movs r1, #160
	movs r0, #8
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #4
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #7
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #6
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r2, #0
	movs r0, #15
	lsls r1, r1, #6
	bl ObjectMotion_ArmCallback
	movs r1, #0
	movs r0, #14
	bl Func_02003534
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #14
	movs r1, #0
	bl Func_0200351c
	movs r0, #11
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #11
	bl Func_0200354c
	movs r0, #50
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #11
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #11
	movs r1, #0
	movs r2, #5
	bl Func_0200351c
	movs r1, #2
	adds r1, #255
	movs r2, #30
	movs r0, #14
	bl Func_02003544
	movs r0, #14
	movs r1, #0
	movs r2, #5
	bl Func_0200351c
	movs r1, #6
	movs r2, #30
	adds r1, #255
	movs r0, #11
	bl Func_02003544
	movs r1, #160
	lsls r1, r1, #7
	movs r0, #11
	bl Func_02003534
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #11
	movs r1, #0
	movs r2, #15
	bl Func_0200351c
	movs r1, #131
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #14
	bl Func_02003544
	movs r0, #14
	movs r1, #0
	movs r2, #5
	bl Func_0200351c
	movs r1, #2
	movs r2, #35
	adds r1, #255
	movs r0, #11
	bl Func_02003544
	movs r1, #128
	lsls r1, r1, #8
	movs r0, #11
	bl Func_02003534
	movs r0, #5
	bl Battle_WaitMode0
	movs r2, #5
	movs r0, #11
	movs r1, #0
	bl Func_0200351c
	movs r0, #14
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r2, #5
	movs r0, #14
	movs r1, #0
	bl Func_0200351c
	movs r1, #176
	lsls r1, r1, #8
	movs r0, #11
	bl Func_02003534
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #5
	movs r0, #11
	movs r1, #0
	bl Func_0200351c
	movs r0, #14
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #14
	movs r1, #0
	movs r2, #5
	bl Func_0200351c
	movs r1, #8
	movs r0, #14
	negs r1, r1
	movs r2, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #128
	movs r0, #11
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #8
	movs r0, #14
	negs r1, r1
	movs r2, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #11
	bl Func_02003544
	movs r1, #56
	movs r2, #0
	movs r0, #14
	negs r1, r1
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #176
	movs r0, #14
	lsls r1, r1, #8
	bl Func_02003534
	movs r1, #2
	movs r2, #30
	adds r1, #255
	movs r0, #14
	bl Func_02003544
	movs r1, #0
	movs r0, #14
	bl Func_02003534
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #5
	movs r0, #14
	movs r1, #0
	bl Func_0200351c
	movs r1, #2
	movs r0, #11
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl Battle_WaitMode0
	movs r2, #20
	movs r0, #11
	movs r1, #6
	bl ObjectMotion_Launch
	movs r0, #4
	movs r1, #11
	bl Object_LinkObjectAndSetCallback
	movs r0, #7
	movs r1, #11
	bl Object_LinkObjectAndSetCallback
	movs r0, #5
	movs r1, #11
	bl Object_LinkObjectAndSetCallback
	movs r0, #6
	movs r1, #11
	bl Object_LinkObjectAndSetCallback
	movs r0, #15
	movs r1, #11
	bl Object_LinkObjectAndSetCallback
	movs r1, #128
	movs r2, #128
	movs r0, #11
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl ObjectMotion_SetSpeedParameters
	movs r1, #80
	movs r2, #0
	negs r1, r1
	movs r0, #11
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #14
	movs r1, #3
	bl ObjectMotion_SetActionVariant
	movs r0, #11
	movs r1, #3
	bl ObjectMotion_SetActionVariant
	movs r1, #128
	movs r2, #128
	movs r0, #11
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r2, #64
	movs r0, #14
	movs r1, #0
	negs r2, r2
	bl ObjectMotion_OffsetPositionAndReset
	movs r1, #16
	movs r0, #11
	negs r1, r1
	movs r2, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r2, #64
	negs r2, r2
	movs r1, #0
	movs r0, #11
	bl ObjectMotion_OffsetPositionAndReset
	movs r0, #15
	bl Battle_WaitMode0
	movs r0, #14
	movs r1, #2
	bl ObjectMotion_SetActionVariant
	movs r1, #2
	movs r0, #11
	bl ObjectMotion_SetActionVariant
	movs r0, #14
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r1, #0
	movs r2, #0
	movs r0, #14
	bl Func_020034d4
	movs r0, #11
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #11
	movs r1, #0
	movs r2, #0
	bl Func_020034d4
	movs r1, #160
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #4
	bl ObjectMotion_ArmCallback
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #60
	bl Battle_WaitMode0
	movs r2, #16
	movs r0, #8
	movs r1, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #128
	movs r0, #8
	lsls r1, r1, #8
	bl Func_02003534
	movs r0, #8
	movs r1, #0
	movs r2, #5
	bl Func_0200351c
	movs r1, #2
	adds r1, #255
	movs r2, #0
	movs r0, #4
	bl Func_02003544
	movs r1, #2
	adds r1, #255
	movs r2, #0
	movs r0, #7
	bl Func_02003544
	movs r1, #2
	adds r1, #255
	movs r2, #0
	movs r0, #5
	bl Func_02003544
	movs r1, #2
	adds r1, #255
	movs r2, #0
	movs r0, #6
	bl Func_02003544
	movs r1, #2
	adds r1, #255
	movs r2, #30
	movs r0, #15
	bl Func_02003544
	movs r0, #4
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #7
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r2, #0
	movs r0, #6
	movs r1, #0
	bl ObjectMotion_ArmCallback
	movs r1, #0
	movs r0, #15
	bl Func_02003534
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #8
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #8
	movs r1, #0
	movs r2, #10
	bl Func_0200351c
	movs r1, #192
	movs r0, #4
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #7
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r2, #0
	movs r0, #6
	lsls r1, r1, #7
	bl ObjectMotion_ArmCallback
	movs r1, #160
	lsls r1, r1, #7
	movs r0, #15
	bl Func_02003534
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #4
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #7
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r2, #0
	movs r0, #6
	movs r1, #0
	bl ObjectMotion_ArmCallback
	movs r1, #0
	movs r0, #15
	bl Func_02003534
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #8
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #8
	movs r1, #0
	movs r2, #5
	bl Func_0200351c
	movs r1, #179
	movs r2, #178
	lsls r1, r1, #8
	lsls r2, r2, #7
	adds r2, #153
	movs r0, #11
	adds r1, #51
	bl ObjectMotion_SetSpeedParameters
	ldr r1, .L_0200a9ec
	movs r0, #8
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #8
	bl ObjectMotion_EnableActionAndResetMotion
	movs r1, #162
	movs r2, #188
	movs r0, #8
	lsls r1, r1, #2
	bl ObjectMotion_SetPositionAndReset
	movs r1, #160
	lsls r1, r1, #7
	movs r0, #8
	bl Func_02003534
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #192
	movs r0, #4
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #7
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r2, #0
	movs r0, #6
	lsls r1, r1, #6
	bl ObjectMotion_ArmCallback
	movs r1, #160
	lsls r1, r1, #7
	movs r0, #15
	bl Func_02003534
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #4
	bl ObjectMotion_SetActionVariant
	movs r0, #4
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	movs r2, #153
	lsls r2, r2, #8
	strb r3, [r0]
	adds r2, #153
	movs r0, #5
	ldr r1, .L_0200a9f0
	bl ObjectMotion_SetSpeedParameters
	movs r0, #5
	movs r1, #2
	bl Object_SetModeById
	ldr r3, .L_0200a9f4
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r3, r2
	ldr r0, [r5]
	bl Object_GetById
	cmp r0, #0
	beq .L_0200a90c
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #5
	bl ObjectMotion_ResetAndSetPosition
.L_0200a90c:
	movs r0, #5
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl Func_020034d4
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #6
	ldr r1, .L_0200a9f0
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r0, #6
	movs r1, #2
	bl Object_SetModeById
	ldr r0, [r5]
	bl Object_GetById
	cmp r0, #0
	beq .L_0200a94a
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #6
	bl ObjectMotion_ResetAndSetPosition
.L_0200a94a:
	movs r0, #6
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #6
	movs r1, #0
	movs r2, #0
	bl Func_020034d4
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #7
	ldr r1, .L_0200a9f0
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r0, #7
	movs r1, #2
	bl Object_SetModeById
	ldr r0, [r5]
	bl Object_GetById
	cmp r0, #0
	beq .L_0200a988
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #7
	bl ObjectMotion_ResetAndSetPosition
.L_0200a988:
	movs r0, #7
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #7
	movs r1, #0
	movs r2, #0
	bl Func_020034d4
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #15
	ldr r1, .L_0200a9f0
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r0, #15
	movs r1, #2
	bl Object_SetModeById
	ldr r0, [r5]
	bl Object_GetById
	cmp r0, #0
	beq .L_0200a9c6
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #15
	bl ObjectMotion_ResetAndSetPosition
.L_0200a9c6:
	movs r0, #15
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r1, #0
	movs r2, #0
	movs r0, #15
	bl Func_020034d4
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #126
	bl GameFlag_SetBit
	bl Func_02003464
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, pc}
.L_0200a9ec:
	.4byte Data_020041e8
.L_0200a9f0:
	.4byte 0x00013333
.L_0200a9f4:
	.4byte gPartyState
	.section .text.x0200a9f8,"ax",%progbits
	.global Func_020029f8
	.thumb_func
Func_020029f8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #20
	mov r9, r1
	movs r1, #133
	str r0, [sp, #4]
	lsls r1, r1, #3
	movs r0, #220
	bl Runtime_AllocateHeapBlockFar
	mov r10, r0
	ldr r0, [sp, #4]
	bl Object_GetById
	mov r1, r9
	adds r7, r0, #0
	cmp r1, #0
	bne .L_0200aa3e
	movs r5, #15
.L_0200aa28:
	ldrh r3, [r7, #6]
	movs r2, #128
	lsls r2, r2, #6
	adds r3, r3, r2
	strh r3, [r7, #6]
	movs r0, #1
	subs r5, #1
	bl WaitFrames
	cmp r5, #0
	bge .L_0200aa28
.L_0200aa3e:
	movs r0, #2
	bl WaitFrames
	movs r3, #0
	str r3, [r7, #24]
	adds r0, r7, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r3, #128
	movs r2, #128
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	ldr r0, .L_0200ac18
	ldr r1, .L_0200ac1c
	adds r2, #16
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r0, .L_0200ac20
	bl Resource_GetTableEntry
	mov r1, r10
	bl Func_020033dc
	bl Resource_FindFreeEntry
	movs r1, #128
	lsls r1, r1, #3
	mov r2, r10
	mov r11, r0
	bl VramBlock_LoadCached
	movs r6, #128
	lsls r6, r6, #3
	add r6, r10
	movs r3, #128
	str r0, [sp, #0]
	movs r1, #8
	movs r2, #16
	lsls r3, r3, #23
	mov r8, r0
	adds r0, r6, #0
	bl Func_020035fc
	movs r3, #240
	strh r3, [r6, #30]
	ldrb r3, [r6, #9]
	movs r2, #13
	negs r2, r2
	ldrb r1, [r6, #5]
	ands r2, r3
	movs r3, #4
	orrs r2, r3
	movs r3, #33
	negs r3, r3
	ands r3, r1
	strb r3, [r6, #5]
	movs r3, #240
	orrs r2, r3
	strb r2, [r6, #9]
	ldr r3, [r7, #8]
	add r5, sp, #8
	str r3, [r5]
	ldr r3, [r7, #12]
	adds r0, r5, #0
	str r3, [r5, #4]
	ldr r3, [r7, #16]
	str r3, [r5, #8]
	bl Func_020035c4
	ldr r3, [r5]
	str r3, [r6, #12]
	mov r3, r9
	ldr r2, [r5, #8]
	str r2, [r6, #16]
	cmp r3, #1
	bne .L_0200aaea
	movs r3, #128
	movs r1, #160
	lsls r3, r3, #10
	lsls r1, r1, #12
	str r3, [r6, #20]
	str r3, [r6, #24]
	adds r3, r2, r1
	str r3, [r6, #16]
.L_0200aaea:
	movs r0, #145
	bl Func_0200362c
	movs r5, #0
.L_0200aaf2:
	movs r6, #128
	lsls r6, r6, #3
	subs r3, r5, #6
	add r6, r10
	cmp r3, #56
	bls .L_0200ab00
	b .L_0200ac50
.L_0200ab00:
	ldr r2, .L_0200ac24
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
.L_0200ab08:
	.4byte .L_0200abec
	.4byte .L_0200ac50
	.4byte .L_0200ac50
	.4byte .L_0200ac50
	.4byte .L_0200ac50
	.4byte .L_0200ac50
	.4byte .L_0200abf4
	.4byte .L_0200ac50
	.4byte .L_0200ac50
	.4byte .L_0200ac50
	.4byte .L_0200ac50
	.4byte .L_0200ac50
	.4byte .L_0200ac50
	.4byte .L_0200ac50
	.4byte .L_0200ac50
	.4byte .L_0200ac50
	.4byte .L_0200abfc
	.4byte .L_0200ac50
	.4byte .L_0200ac50
	.4byte .L_0200ac50
	.4byte .L_0200ac50
	.4byte .L_0200ac50
	.4byte .L_0200ac50
	.4byte .L_0200ac50
	.4byte .L_0200ac50
	.4byte .L_0200ac50
	.4byte .L_0200ac04
	.4byte .L_0200ac50
	.4byte .L_0200ac50
	.4byte .L_0200ac50
	.4byte .L_0200ac50
	.4byte .L_0200ac50
	.4byte .L_0200ac50
	.4byte .L_0200ac50
	.4byte .L_0200ac50
	.4byte .L_0200ac50
	.4byte .L_0200ac0c
	.4byte .L_0200ac50
	.4byte .L_0200ac50
	.4byte .L_0200ac50
	.4byte .L_0200ac50
	.4byte .L_0200ac50
	.4byte .L_0200ac50
	.4byte .L_0200ac50
	.4byte .L_0200ac50
	.4byte .L_0200ac50
	.4byte .L_0200ac28
	.4byte .L_0200ac50
	.4byte .L_0200ac50
	.4byte .L_0200ac50
	.4byte .L_0200ac50
	.4byte .L_0200ac50
	.4byte .L_0200ac50
	.4byte .L_0200ac50
	.4byte .L_0200ac50
	.4byte .L_0200ac50
	.4byte .L_0200ac34
.L_0200abec:
	ldr r3, .L_0200ac14
	mov r2, r8
	adds r2, #4
	b .L_0200ac3a
.L_0200abf4:
	ldr r3, .L_0200ac14
	mov r2, r8
	adds r2, #8
	b .L_0200ac3a
.L_0200abfc:
	ldr r3, .L_0200ac14
	mov r2, r8
	adds r2, #12
	b .L_0200ac3a
.L_0200ac04:
	ldr r3, .L_0200ac14
	mov r2, r8
	adds r2, #16
	b .L_0200ac3a
.L_0200ac0c:
	ldr r3, .L_0200ac14
	mov r2, r8
	adds r2, #20
	b .L_0200ac3a
.L_0200ac14:
	.4byte 0x000003ff
.L_0200ac18:
	.4byte Data_02003640
.L_0200ac1c:
	.4byte 0x050003e0
.L_0200ac20:
	.4byte 0x000001e8
.L_0200ac24:
	.4byte .L_0200ab08
.L_0200ac28:
	ldr r3, .L_0200ac30
	mov r2, r8
	adds r2, #24
	b .L_0200ac3a
.L_0200ac30:
	.4byte 0x000003ff
.L_0200ac34:
	ldr r3, .L_0200ac48
	mov r2, r8
	adds r2, #28
.L_0200ac3a:
	ands r2, r3
	ldrh r1, [r6, #8]
	ldr r3, .L_0200ac4c
	ands r3, r1
	orrs r3, r2
	strh r3, [r6, #8]
	b .L_0200ac50
.L_0200ac48:
	.4byte 0x000003ff
.L_0200ac4c:
	.4byte 0xfffffc00
.L_0200ac50:
	adds r0, r6, #0
	bl Func_02003604
	adds r5, #1
	movs r0, #1
	bl WaitFrames
	cmp r5, #71
	bgt .L_0200ac64
	b .L_0200aaf2
.L_0200ac64:
	ldr r0, [sp, #4]
	movs r1, #0
	movs r2, #0
	bl Func_020034d4
	mov r0, r11
	bl Resource_ResetEntry
	movs r0, #220
	bl Runtime_ReleaseHeapBlock
	add sp, #20
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.section .text.x0200ac88,"ax",%progbits
	.global Func_02002c88
	.thumb_func
Func_02002c88:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r7, [r3]
	ldr r0, .L_0200ad30
	sub sp, #4
	bl Resource_GetTableEntry
	adds r1, r7, #0
	bl Func_020033dc
	movs r0, #0
	adds r2, r7, #0
.L_0200aca4:
	ldrb r1, [r2]
	movs r3, #240
	ands r3, r1
	cmp r3, #240
	bne .L_0200acb6
	movs r3, #15
	ands r3, r1
	adds r3, #96
	strb r3, [r2]
.L_0200acb6:
	movs r3, #128
	adds r0, #1
	lsls r3, r3, #5
	adds r2, #1
	cmp r0, r3
	blt .L_0200aca4
	bl Resource_FindFreeEntry
	movs r1, #128
	adds r2, r7, #0
	lsls r1, r1, #5
	adds r5, r0, #0
	bl VramBlock_LoadCached
	movs r2, #128
	lsls r2, r2, #5
	adds r2, #92
	adds r3, r7, r2
	adds r2, #2
	strh r5, [r3]
	adds r3, r7, r2
	strh r0, [r3]
	movs r3, #128
	lsls r3, r3, #5
	adds r3, #12
	adds r5, r7, r3
	movs r3, #192
	str r0, [sp, #0]
	movs r1, #31
	adds r0, r5, #0
	movs r2, #31
	lsls r3, r3, #24
	bl Func_020035fc
	ldrb r3, [r5, #5]
	movs r2, #32
	orrs r3, r2
	ldrb r2, [r5, #9]
	strb r3, [r5, #5]
	movs r3, #15
	ands r3, r2
	movs r2, #13
	negs r2, r2
	ands r3, r2
	movs r2, #8
	orrs r3, r2
	movs r2, #128
	lsls r2, r2, #5
	strb r3, [r5, #9]
	adds r2, #100
	movs r3, #1
	strh r3, [r5, #30]
	movs r6, #0
	adds r3, r7, r2
	adds r2, #4
	str r6, [r3]
	adds r3, r7, r2
	str r6, [r3]
	add sp, #4
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200ad30:
	.4byte 0x000001d9
	.section .text.x0200ad34,"ax",%progbits
	.global Func_02002d34
	.thumb_func
Func_02002d34:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r7, [r3]
	ldr r0, .L_0200addc
	sub sp, #4
	bl Resource_GetTableEntry
	adds r1, r7, #0
	bl Func_020033dc
	movs r0, #0
	adds r2, r7, #0
.L_0200ad50:
	ldrb r1, [r2]
	movs r3, #240
	ands r3, r1
	cmp r3, #240
	bne .L_0200ad62
	movs r3, #15
	ands r3, r1
	adds r3, #96
	strb r3, [r2]
.L_0200ad62:
	movs r3, #128
	adds r0, #1
	lsls r3, r3, #5
	adds r2, #1
	cmp r0, r3
	blt .L_0200ad50
	bl Resource_FindFreeEntry
	movs r1, #128
	adds r2, r7, #0
	lsls r1, r1, #4
	adds r5, r0, #0
	bl VramBlock_LoadCached
	movs r2, #131
	lsls r2, r2, #5
	adds r3, r7, r2
	adds r2, #2
	strh r5, [r3]
	adds r3, r7, r2
	strh r0, [r3]
	movs r3, #128
	lsls r3, r3, #5
	adds r3, #52
	adds r5, r7, r3
	str r0, [sp, #0]
	movs r1, #15
	adds r0, r5, #0
	movs r2, #47
	ldr r3, .L_0200ade0
	bl Func_020035fc
	ldrb r3, [r5, #5]
	movs r2, #32
	orrs r3, r2
	ldrb r2, [r5, #9]
	strb r3, [r5, #5]
	movs r3, #15
	ands r3, r2
	movs r2, #16
	orrs r3, r2
	movs r2, #13
	negs r2, r2
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	movs r2, #128
	lsls r2, r2, #5
	strb r3, [r5, #9]
	adds r2, #108
	movs r3, #1
	strh r3, [r5, #30]
	movs r6, #0
	adds r3, r7, r2
	adds r2, #4
	str r6, [r3]
	adds r3, r7, r2
	str r6, [r3]
	add sp, #4
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200addc:
	.4byte 0x000001f7
.L_0200ade0:
	.4byte 0xc0008000
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
	bl Engine_MathDivide
	ldr r2, [sp, #0]
	movs r1, #40
	str r0, [r2]
	ldr r3, [r5]
	lsls r0, r3, #5
	subs r0, r0, r3
	bl Engine_MathDivide
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
	.section .text.x0200af58,"ax",%progbits
	.global Func_02002f58
	.thumb_func
Func_02002f58:
	push {r5, r6, r7, lr}
	movs r1, #128
	lsls r1, r1, #5
	adds r5, r0, #0
	adds r1, #120
	movs r0, #220
	bl Runtime_AllocateHeapBlockFar
	ldr r3, [r5]
	movs r1, #128
	adds r6, r0, #0
	lsls r1, r1, #5
	adds r2, r6, r1
	str r3, [r2]
	movs r3, #128
	lsls r3, r3, #5
	adds r3, #4
	adds r2, r6, r3
	ldr r3, [r5, #4]
	adds r1, #8
	str r3, [r2]
	adds r2, r6, r1
	ldr r3, [r5, #8]
	movs r7, #0
	str r3, [r2]
	bl Func_02002c88
	bl Func_02002d34
	movs r3, #128
	lsls r3, r3, #5
	adds r3, #116
	adds r2, r6, r3
	movs r1, #144
	movs r3, #0
	strh r3, [r2]
	lsls r1, r1, #3
	ldr r0, .L_0200b074
	bl Scheduler_AddOrUpdateCallback
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r0, r0, #11
	lsls r1, r1, #11
	lsls r2, r2, #9
	bl Func_02003434
	movs r5, #0
.L_0200afba:
	cmp r5, #1
	beq .L_0200affe
	cmp r5, #1
	bgt .L_0200afc8
	cmp r5, #0
	beq .L_0200afd2
	b .L_0200b060
.L_0200afc8:
	cmp r5, #2
	beq .L_0200b00a
	cmp r5, #3
	beq .L_0200b036
	b .L_0200b060
.L_0200afd2:
	movs r1, #128
	lsls r1, r1, #5
	adds r1, #100
	adds r2, r6, r1
	ldr r3, [r2]
	movs r1, #192
	lsls r1, r1, #3
	adds r3, r3, r1
	movs r1, #166
	lsls r1, r1, #9
	adds r1, #203
	str r3, [r2]
	cmp r3, r1
	ble .L_0200b060
	movs r3, #166
	lsls r3, r3, #9
	adds r3, #204
	movs r7, #1
	str r3, [r2]
	movs r5, #1
	negs r7, r7
	b .L_0200b060
.L_0200affe:
	cmp r7, #10
	bne .L_0200b060
	movs r7, #1
	movs r5, #2
	negs r7, r7
	b .L_0200b060
.L_0200b00a:
	movs r3, #128
	lsls r3, r3, #5
	adds r3, #108
	adds r2, r6, r3
	ldr r3, [r2]
	movs r1, #128
	lsls r1, r1, #3
	adds r3, r3, r1
	movs r1, #204
	lsls r1, r1, #7
	adds r1, #101
	str r3, [r2]
	cmp r3, r1
	ble .L_0200b060
	movs r3, #204
	lsls r3, r3, #7
	adds r3, #102
	movs r7, #1
	str r3, [r2]
	movs r5, #3
	negs r7, r7
	b .L_0200b060
.L_0200b036:
	movs r3, #128
	lsls r3, r3, #5
	adds r3, #108
	adds r2, r6, r3
	ldr r3, [r2]
	movs r1, #192
	lsls r1, r1, #6
	adds r3, r3, r1
	movs r1, #166
	lsls r1, r1, #9
	adds r1, #203
	str r3, [r2]
	cmp r3, r1
	ble .L_0200b060
	movs r3, #166
	lsls r3, r3, #9
	adds r3, #204
	movs r5, #186
	str r3, [r2]
	lsls r5, r5, #2
	adds r5, #255
.L_0200b060:
	movs r0, #1
	bl WaitFrames
	movs r3, #186
	lsls r3, r3, #2
	adds r3, #255
	adds r7, #1
	cmp r5, r3
	bne .L_0200afba
	pop {r5, r6, r7, pc}
.L_0200b074:
	.4byte Func_02002de4
	.section .text.x0200b078,"ax",%progbits
	.global Func_02003078
	.thumb_func
Func_02003078:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r6, [r3]
	movs r1, #128
	lsls r1, r1, #5
	adds r3, r6, r1
	ldr r3, [r3]
	sub sp, #12
	mov r2, sp
	str r3, [r2]
	adds r1, #4
	adds r3, r6, r1
	ldr r3, [r3]
	ldr r1, .L_0200b154
	movs r5, #0
	adds r3, r3, r1
	movs r1, #128
	str r3, [r2, #4]
	lsls r1, r1, #5
	adds r1, #8
	adds r3, r6, r1
	ldr r3, [r3]
	movs r7, #0
	str r3, [r2, #8]
.L_0200b0ac:
	cmp r5, #1
	beq .L_0200b0de
	cmp r5, #1
	bgt .L_0200b0ba
	cmp r5, #0
	beq .L_0200b0c0
	b .L_0200b108
.L_0200b0ba:
	cmp r5, #2
	beq .L_0200b0fe
	b .L_0200b108
.L_0200b0c0:
	movs r3, #128
	lsls r3, r3, #5
	adds r3, #108
	adds r2, r6, r3
	ldr r3, [r2]
	ldr r1, .L_0200b158
	adds r3, r3, r1
	str r3, [r2]
	cmp r3, #0
	bge .L_0200b108
	movs r7, #1
	str r5, [r2]
	negs r7, r7
	movs r5, #1
	b .L_0200b108
.L_0200b0de:
	movs r3, #128
	lsls r3, r3, #5
	adds r3, #100
	adds r2, r6, r3
	ldr r3, [r2]
	ldr r1, .L_0200b15c
	adds r3, r3, r1
	str r3, [r2]
	cmp r3, #0
	bge .L_0200b108
	movs r3, #0
	movs r7, #1
	str r3, [r2]
	movs r5, #2
	negs r7, r7
	b .L_0200b108
.L_0200b0fe:
	cmp r7, #1
	bne .L_0200b108
	movs r5, #186
	lsls r5, r5, #2
	adds r5, #255
.L_0200b108:
	movs r0, #1
	bl WaitFrames
	movs r2, #186
	lsls r2, r2, #2
	adds r2, #255
	adds r7, #1
	cmp r5, r2
	bne .L_0200b0ac
	ldr r0, .L_0200b160
	bl Scheduler_RemoveCallbackFar
	movs r2, #0
	movs r0, #0
	movs r1, #0
	bl Func_02003434
	movs r1, #128
	lsls r1, r1, #5
	adds r1, #92
	adds r3, r6, r1
	movs r2, #0
	ldrsh r0, [r3, r2]
	bl Resource_ResetEntry
	movs r1, #131
	lsls r1, r1, #5
	adds r3, r6, r1
	movs r2, #0
	ldrsh r0, [r3, r2]
	bl Resource_ResetEntry
	movs r0, #220
	bl Runtime_ReleaseHeapBlock
	add sp, #12
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200b154:
	.4byte 0xffe00000
.L_0200b158:
	.4byte 0xfffff000
.L_0200b15c:
	.4byte 0xfffff800
.L_0200b160:
	.4byte Func_02002de4
	.section .text.x0200b164,"ax",%progbits
	.global Func_02003164
	.thumb_func
Func_02003164:
	push {r5, r6, lr}
	movs r1, #128
	lsls r1, r1, #5
	adds r5, r0, #0
	adds r1, #120
	movs r0, #220
	bl Runtime_AllocateHeapBlockFar
	ldr r3, [r5]
	movs r1, #128
	adds r6, r0, #0
	lsls r1, r1, #5
	adds r2, r6, r1
	str r3, [r2]
	movs r3, #128
	lsls r3, r3, #5
	adds r3, #4
	adds r2, r6, r3
	ldr r3, [r5, #4]
	adds r1, #8
	str r3, [r2]
	adds r2, r6, r1
	ldr r3, [r5, #8]
	movs r5, #0
	str r3, [r2]
	bl Func_02002c88
	movs r2, #128
	lsls r2, r2, #5
	movs r1, #128
	adds r2, #108
	lsls r1, r1, #5
	adds r3, r6, r2
	adds r1, #112
	movs r2, #0
	str r2, [r3]
	adds r3, r6, r1
	adds r1, #4
	str r2, [r3]
	adds r3, r6, r1
	movs r1, #144
	strh r2, [r3]
	lsls r1, r1, #3
	ldr r0, .L_0200b208
	bl Scheduler_AddOrUpdateCallback
	movs r0, #139
	bl Func_0200362c
.L_0200b1c6:
	cmp r5, #0
	bne .L_0200b1f4
	movs r3, #128
	lsls r3, r3, #5
	adds r3, #100
	adds r2, r6, r3
	ldr r3, [r2]
	movs r1, #128
	lsls r1, r1, #5
	adds r3, r3, r1
	movs r1, #166
	lsls r1, r1, #9
	adds r1, #203
	str r3, [r2]
	cmp r3, r1
	ble .L_0200b1f4
	movs r3, #166
	lsls r3, r3, #9
	adds r3, #204
	movs r5, #186
	str r3, [r2]
	lsls r5, r5, #2
	adds r5, #255
.L_0200b1f4:
	movs r0, #1
	bl WaitFrames
	movs r2, #186
	lsls r2, r2, #2
	adds r2, #255
	cmp r5, r2
	bne .L_0200b1c6
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200b208:
	.4byte Func_02002de4
	.section .text.x0200b20c,"ax",%progbits
	.global Func_0200320c
	.thumb_func
Func_0200320c:
	push {r5, r6, lr}
	movs r1, #128
	lsls r1, r1, #5
	adds r1, #120
	movs r0, #220
	bl Runtime_AllocateHeapBlockFar
	adds r6, r0, #0
	bl Func_02002d34
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_0200b2a0
	bl Scheduler_AddOrUpdateCallback
	movs r0, #144
	bl Func_0200362c
	movs r5, #0
.L_0200b232:
	cmp r5, #0
	beq .L_0200b23c
	cmp r5, #1
	beq .L_0200b262
	b .L_0200b28c
.L_0200b23c:
	movs r1, #128
	lsls r1, r1, #5
	adds r1, #108
	adds r2, r6, r1
	ldr r3, [r2]
	subs r1, #108
	adds r3, r3, r1
	movs r1, #204
	lsls r1, r1, #7
	adds r1, #101
	str r3, [r2]
	cmp r3, r1
	ble .L_0200b28c
	movs r3, #204
	lsls r3, r3, #7
	adds r3, #102
	str r3, [r2]
	movs r5, #1
	b .L_0200b28c
.L_0200b262:
	movs r3, #128
	lsls r3, r3, #5
	adds r3, #108
	adds r2, r6, r3
	ldr r3, [r2]
	movs r1, #192
	lsls r1, r1, #6
	adds r3, r3, r1
	movs r1, #166
	lsls r1, r1, #9
	adds r1, #203
	str r3, [r2]
	cmp r3, r1
	ble .L_0200b28c
	movs r3, #166
	lsls r3, r3, #9
	adds r3, #204
	movs r5, #186
	str r3, [r2]
	lsls r5, r5, #2
	adds r5, #255
.L_0200b28c:
	movs r0, #1
	bl WaitFrames
	movs r3, #186
	lsls r3, r3, #2
	adds r3, #255
	cmp r5, r3
	bne .L_0200b232
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200b2a0:
	.4byte Func_02002de4
	.section .text.x0200b2a4,"ax",%progbits
	.global Func_020032a4
	.thumb_func
Func_020032a4:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r6, [r3]
	movs r5, #0
	movs r7, #0
.L_0200b2b2:
	cmp r5, #1
	beq .L_0200b2ee
	cmp r5, #1
	bgt .L_0200b2c0
	cmp r5, #0
	beq .L_0200b2ca
	b .L_0200b358
.L_0200b2c0:
	cmp r5, #2
	beq .L_0200b316
	cmp r5, #3
	beq .L_0200b34e
	b .L_0200b358
.L_0200b2ca:
	movs r1, #128
	lsls r1, r1, #5
	adds r1, #108
	adds r2, r6, r1
	ldr r3, [r2]
	subs r1, #108
	adds r3, r3, r1
	ldr r1, .L_0200b398
	str r3, [r2]
	cmp r3, r1
	ble .L_0200b358
	movs r3, #128
	lsls r3, r3, #10
	movs r7, #1
	str r3, [r2]
	movs r5, #1
	negs r7, r7
	b .L_0200b358
.L_0200b2ee:
	movs r3, #128
	lsls r3, r3, #5
	adds r3, #108
	adds r2, r6, r3
	ldr r3, [r2]
	ldr r1, .L_0200b39c
	adds r3, r3, r1
	movs r1, #166
	lsls r1, r1, #9
	adds r1, #203
	str r3, [r2]
	cmp r3, r1
	bgt .L_0200b316
	movs r3, #166
	lsls r3, r3, #9
	adds r3, #204
	str r3, [r2]
	movs r7, #1
	movs r5, #2
	negs r7, r7
.L_0200b316:
	movs r3, #128
	lsls r3, r3, #5
	adds r3, #108
	adds r2, r6, r3
	ldr r3, [r2]
	ldr r1, .L_0200b39c
	adds r3, r3, r1
	str r3, [r2]
	cmp r3, #0
	bge .L_0200b32e
	movs r3, #0
	str r3, [r2]
.L_0200b32e:
	movs r3, #128
	lsls r3, r3, #5
	adds r3, #100
	adds r2, r6, r3
	ldr r3, [r2]
	ldr r1, .L_0200b39c
	adds r3, r3, r1
	str r3, [r2]
	cmp r3, #0
	bge .L_0200b358
	movs r3, #0
	movs r7, #1
	str r3, [r2]
	adds r5, #1
	negs r7, r7
	b .L_0200b358
.L_0200b34e:
	cmp r7, #1
	bne .L_0200b358
	movs r5, #186
	lsls r5, r5, #2
	adds r5, #255
.L_0200b358:
	movs r0, #1
	bl WaitFrames
	movs r2, #186
	lsls r2, r2, #2
	adds r2, #255
	adds r7, #1
	cmp r5, r2
	bne .L_0200b2b2
	ldr r0, .L_0200b3a0
	bl Scheduler_RemoveCallbackFar
	movs r1, #128
	lsls r1, r1, #5
	adds r1, #92
	adds r3, r6, r1
	movs r2, #0
	ldrsh r0, [r3, r2]
	bl Resource_ResetEntry
	movs r1, #131
	lsls r1, r1, #5
	adds r3, r6, r1
	movs r2, #0
	ldrsh r0, [r3, r2]
	bl Resource_ResetEntry
	movs r0, #220
	bl Runtime_ReleaseHeapBlock
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200b398:
	.4byte 0x0001ffff
.L_0200b39c:
	.4byte 0xfffff000
.L_0200b3a0:
	.4byte Func_02002de4
	.section .rodata.x0200b634,"a",%progbits
	.global Data_02003634
Data_02003634:
	.4byte 0x000001b7
	.4byte 0x000001b8
	.4byte 0x000001b9
	.global Data_02003640
Data_02003640:
	.4byte 0x575a0260
	.4byte 0x46754ad7
	.4byte 0x31cf3a32
	.4byte 0x28ea294c
	.4byte 0x00750009
	.4byte 0x01bf011f
	.4byte 0x031f027f
	.4byte 0x7fff03ff
	.global gSceneEntrances
gSceneEntrances:
	.4byte 0xffff0000
	.4byte 0x000001b8
	.4byte 0x40000208
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
	.4byte 0x000000b9
	.4byte 0x101080b6
	.4byte 0xffffffff
	.4byte 0x1020c0ba
	.4byte 0xffffffff
	.4byte 0x103040b9
	.4byte 0xffffffff
	.4byte 0x104030b9
	.4byte 0xffffffff
	.4byte 0x105060b9
	.4byte 0xffffffff
	.4byte 0x106050b9
	.4byte 0xffffffff
	.4byte 0x000000ba
	.4byte 0x1070a0b6
	.4byte 0xffffffff
	.4byte 0x108090ba
	.4byte 0xffffffff
	.4byte 0x109080ba
	.4byte 0xffffffff
	.4byte 0x10a0b0ba
	.4byte 0xffffffff
	.4byte 0x10b0a0ba
	.4byte 0xffffffff
	.4byte 0x10c020b9
	.4byte 0xffffffff
	.4byte 0x10d0e0bb
	.4byte 0xffffffff
	.4byte 0x000000bb
	.4byte 0x10e0d0ba
	.4byte 0xffffffff
	.4byte 0x10f100bb
	.4byte 0xffffffff
	.4byte 0x1100f0bb
	.4byte 0xffffffff
	.4byte 0x111120bb
	.4byte 0xffffffff
	.4byte 0x112110bb
	.4byte 0xffffffff
	.4byte 0x1130b0b6
	.4byte 0xffffffff
	.4byte 0x114150bc
	.4byte 0xffffffff
	.4byte 0x000000bc
	.4byte 0x115140bb
	.4byte 0xffffffff
	.4byte 0x116170bc
	.4byte 0xffffffff
	.4byte 0x117160bc
	.4byte 0xffffffff
	.4byte 0x000001ff
	.global Data_0200375c
Data_0200375c:
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x02380000
	.4byte 0x00013000
	.4byte 0xffff0074
	.4byte 0x00000002
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x00e00000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020037a4
Data_020037a4:
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x00c00000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00013000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x02080000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00013000
	.4byte 0xffff0088
	.4byte 0x00000001
	.4byte 0x02380000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00008000
	.4byte 0xffff008a
	.4byte 0x00000001
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x0000d000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_0200381c
Data_0200381c:
	.4byte 0xffff0024
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00003000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00015000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02003864
Data_02003864:
	.4byte 0xffff0024
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00003000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x02680000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00013000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020038ac
Data_020038ac:
	.4byte 0xffff0027
	.4byte 0x00000001
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x02280000
	.4byte 0x00038000
	.4byte 0xffff0027
	.4byte 0x00000008
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x02380000
	.4byte 0x00008000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x01f80000
	.4byte 0x00033000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_0200390c
Data_0200390c:
	.4byte 0xffff0027
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x02d80000
	.4byte 0x0003d000
	.4byte 0xffff0027
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x02d80000
	.4byte 0x0003b000
	.4byte 0xffff0088
	.4byte 0x00000001
	.4byte 0x02380000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00038000
	.4byte 0xffff008a
	.4byte 0x00000001
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x0003d000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02003984
Data_02003984:
	.4byte 0xffff0027
	.4byte 0x00000001
	.4byte 0x02080000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00035000
	.4byte 0xffff0027
	.4byte 0x00000001
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00033000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020039cc
Data_020039cc:
	.4byte 0xffff0024
	.4byte 0x00000001
	.4byte 0x02e00000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00028000
	.4byte 0xffff0073
	.4byte 0x00000008
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x01d00000
	.4byte 0x0001d000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00024000
	.4byte 0xffff0010
	.4byte 0x00000001
	.4byte 0x02f00000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00028000
	.4byte 0xffff0152
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00028000
	.4byte 0xffff009e
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00028000
	.4byte 0xffff0025
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00018000
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x0002c000
	.4byte 0xffff0005
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00024000
	.4byte 0xffff0006
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00024000
	.4byte 0xffff0007
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x0002c000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02003aec
Data_02003aec:
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x02380000
	.4byte 0x00033000
	.4byte 0xffff0074
	.4byte 0x00000002
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x00e00000
	.4byte 0x00004000
	.4byte 0xffff0027
	.4byte 0x00000001
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x00d00000
	.4byte 0x00003000
	.4byte 0xffff0027
	.4byte 0x00000001
	.4byte 0x02580000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00005000
	.4byte 0xffff0027
	.4byte 0x00000002
	.4byte 0x02400000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00004000
	.4byte 0xffff0027
	.4byte 0x00000001
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02003b94
Data_02003b94:
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x00c00000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00013000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x02080000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00003000
	.4byte 0xffff0088
	.4byte 0x00000001
	.4byte 0x02380000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00008000
	.4byte 0xffff008a
	.4byte 0x00000001
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02003c0c
Data_02003c0c:
	.4byte 0xffff0010
	.4byte 0x00000001
	.4byte 0x02300000
	.4byte 0x00000000
	.4byte 0x02c00000
	.4byte 0x00024000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00018000
	.4byte 0xffff0026
	.4byte 0x00000001
	.4byte 0x01e00000
	.4byte 0x00000000
	.4byte 0x02c00000
	.4byte 0x00030000
	.4byte 0xffff0025
	.4byte 0x00000001
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x02c80000
	.4byte 0x00018000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02003c84
Data_02003c84:
	.4byte 0xffff0024
	.4byte 0x00000001
	.4byte 0x02880000
	.4byte 0x00000000
	.4byte 0x00bc0000
	.4byte 0x00025000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x01d00000
	.4byte 0x00035000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
.L_0200bce4:
	.4byte 0x00400001
	.4byte 0x00020001
	.4byte 0x00020006
	.4byte 0x00010040
	.4byte 0x00060002
	.2byte 0xffff
.L_0200bcfa:
	.2byte 0x0001
	.4byte 0x00010040
	.4byte 0x00060002
	.4byte 0x00400000
	.4byte 0x00020001
	.4byte 0xffff0006
	.global Data_02003d10
Data_02003d10:
	.4byte .L_0200bce4
	.4byte 0x0058000b
	.global Data_02003d18
Data_02003d18:
	.4byte .L_0200bcfa
	.4byte 0x0058000b
	.global Data_02003d20
Data_02003d20:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000021
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
	.4byte 0x00000000
	.4byte 0x197e0008
	.4byte 0x0000268b
	.4byte 0x00000000
	.4byte 0x197e0009
	.4byte 0x0000268d
	.4byte 0x00000000
	.4byte 0x197e000a
	.4byte 0x00002694
	.4byte 0x00000000
	.4byte 0x197e000b
	.4byte 0x00002695
	.4byte 0x00000000
	.4byte 0x197e000c
	.4byte 0x00002696
	.4byte 0x00000000
	.4byte 0x197e000d
	.4byte 0x00002697
	.4byte 0x00008d15
	.4byte 0x197e0008
	.4byte 0x0000269b
	.4byte 0x00008d15
	.4byte 0x197e0009
	.4byte 0x0000269d
	.4byte 0x00008d15
	.4byte 0x197e000a
	.4byte 0x000026a4
	.4byte 0x00008d15
	.4byte 0x197e000b
	.4byte 0x000026a5
	.4byte 0x00008d15
	.4byte 0x197e000c
	.4byte 0x000026a6
	.4byte 0x00008d15
	.4byte 0x197e000d
	.4byte 0x000026a7
	.4byte 0x00000000
	.4byte 0x197f0008
	.4byte 0x000025f9
	.4byte 0x00000000
	.4byte 0x197f0009
	.4byte Func_02000350
	.4byte 0x00000000
	.4byte 0x197f000a
	.4byte Func_02000304
	.4byte 0x00008d15
	.4byte 0x197f0008
	.4byte 0x00002605
	.4byte 0x00008d15
	.4byte 0x197f0009
	.4byte 0x00002606
	.4byte 0x00008d15
	.4byte 0x197f000a
	.4byte 0x0000260c
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00002422
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00002424
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x0000242b
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x0000242d
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02003e7c
Data_02003e7c:
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
	.4byte 0x00000031
	.4byte 0xffff000c
	.4byte 0x0000000c
	.4byte 0x00000021
	.4byte 0xffff000d
	.4byte 0x0000000d
	.4byte 0x00000000
	.4byte 0x197e0008
	.4byte 0x0000268a
	.4byte 0x00000000
	.4byte 0x197e0009
	.4byte 0x0000268e
	.4byte 0x00000000
	.4byte 0x197e000a
	.4byte 0x0000268f
	.4byte 0x00000000
	.4byte 0x197e000b
	.4byte 0x00002690
	.4byte 0x00008d15
	.4byte 0x197e0008
	.4byte 0x0000269a
	.4byte 0x00008d15
	.4byte 0x197e0009
	.4byte 0x0000269e
	.4byte 0x00008d15
	.4byte 0x197e000a
	.4byte 0x0000269f
	.4byte 0x00008d15
	.4byte 0x197e000b
	.4byte 0x000026a0
	.4byte 0x00000000
	.4byte 0x197f0008
	.4byte 0x000025fb
	.4byte 0x00000000
	.4byte 0x197f0009
	.4byte 0x000025fd
	.4byte 0x00000000
	.4byte 0x197f000a
	.4byte 0x00002603
	.4byte 0x00000000
	.4byte 0x197f000b
	.4byte 0x00002604
	.4byte 0x00008d15
	.4byte 0x197f0008
	.4byte 0x00002607
	.4byte 0x00008d15
	.4byte 0x197f0009
	.4byte 0x00002609
	.4byte 0x00008d15
	.4byte 0x197f000a
	.4byte 0x0000260d
	.4byte 0x00008d15
	.4byte 0x197f000b
	.4byte 0x0000260e
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00002421
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00002425
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x00002426
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x00002427
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x0000242a
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x0000242e
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x0000242f
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00002430
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02003ffc
Data_02003ffc:
	.4byte 0x0000c602
	.4byte 0x197e000f
	.4byte Func_020001f4
	.4byte 0x00000031
	.4byte 0xffff000e
	.4byte 0x0000000e
	.4byte 0x0000c602
	.4byte 0xffff000f
	.4byte Func_02000208
	.4byte 0x00000001
	.4byte 0xffff0010
	.4byte 0x00000010
	.4byte 0x00000001
	.4byte 0xffff0011
	.4byte 0x00000011
	.4byte 0x00000001
	.4byte 0xffff0012
	.4byte 0x00000012
	.4byte 0x00000001
	.4byte 0xffff0013
	.4byte 0x00000013
	.4byte 0x00000021
	.4byte 0xffff0014
	.4byte 0x00000014
	.4byte 0x00000000
	.4byte 0x197e0008
	.4byte 0x00002691
	.4byte 0x0000c400
	.4byte 0x197e000b
	.4byte 0x00002692
	.4byte 0x00000000
	.4byte 0x197e000a
	.4byte 0x00002693
	.4byte 0x00008d15
	.4byte 0x197e0008
	.4byte 0x000026a1
	.4byte 0x00008d15
	.4byte 0x197e000b
	.4byte 0x000026a2
	.4byte 0x00008d15
	.4byte 0x197e000a
	.4byte 0x000026a3
	.4byte 0x00000000
	.4byte 0x197f0008
	.4byte 0x000025fc
	.4byte 0x00000000
	.4byte 0x197f0009
	.4byte 0x000025fe
	.4byte 0x00008d15
	.4byte 0x197f0008
	.4byte 0x00002608
	.4byte 0x00008d15
	.4byte 0x197f0009
	.4byte 0x0000260a
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x0000241c
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00002428
	.4byte 0x00000003
	.4byte 0x097e0028
	.4byte Func_02000208
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02004104
Data_02004104:
	.4byte 0x00000031
	.4byte 0xffff0015
	.4byte 0x00000015
	.4byte 0x00000001
	.4byte 0xffff0016
	.4byte 0x00000016
	.4byte 0x00000001
	.4byte 0xffff0017
	.4byte 0x00000017
	.4byte 0x00000002
	.4byte 0x197f001e
	.4byte Func_0200115c
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte Func_02000f48
	.4byte 0x00008d15
	.4byte 0x09750008
	.4byte 0x0000266d
	.4byte 0x00008d15
	.4byte 0x09760008
	.4byte 0x00002677
	.4byte 0x00008d15
	.4byte 0x09770008
	.4byte 0x0000267c
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00002698
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte Func_02000464
	.4byte 0x00000000
	.4byte 0x197e0009
	.4byte 0x00002689
	.4byte 0x00008d15
	.4byte 0x197e0009
	.4byte 0x00002699
	.4byte 0x00000000
	.4byte 0x197f0009
	.4byte Func_0200038c
	.4byte 0x00008d15
	.4byte 0x197f0009
	.4byte 0x0000260b
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00002420
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00002429
	.4byte 0x50009085
	.4byte 0xffff0000
	.4byte Func_020003bc
	.4byte 0x40009085
	.4byte 0xffff0000
	.4byte Func_020003ec
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020041e8
Data_020041e8:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x02680000
	.4byte 0x00000000
	.4byte 0x00dc0000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x02780000
	.4byte 0x00000000
	.4byte 0x00ce0000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x02880000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x02980000
	.4byte 0x00000000
	.4byte 0x00c60000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.global Data_02004254
Data_02004254:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x02a80000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x02b60000
	.4byte 0x00000000
	.4byte 0x00ce0000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x02c60000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x02d00000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x02d00000
	.4byte 0x00000000
	.4byte 0x00f60000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.global Data_020042d4
Data_020042d4:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x02c60000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x02ba0000
	.4byte 0x00000000
	.4byte 0x01160000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x02980000
	.4byte 0x00000000
	.4byte 0x01220000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.global Data_0200432c
Data_0200432c:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x02780000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x02680000
	.4byte 0x00000000
	.4byte 0x010a0000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x02600000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x02600000
	.4byte 0x00000000
	.4byte 0x00e60000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
