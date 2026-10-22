.syntax unified
	.thumb
	.section .text.x02008170,"ax",%progbits
	.ifdef TLA_EDITION_JA
	.balign 4
	.global Func_02000170
	.thumb_func
Func_02000170:
	push {lr}
	bl Func_02005688
	pop {pc}
	.else
	.balign 4
	.global Func_02000170
	.thumb_func
Func_02000170:
	push {r5, r6, lr}
	ldr r3, .L_020081ec
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	sub sp, #12
	bl Object_GetById
	ldrh r3, [r0, #6]
	movs r2, #12
	lsrs r6, r3, #12
	adds r3, r6, #2
	ands r3, r2
	lsls r6, r3, #12
	ldr r3, [r0, #8]
	mov r5, sp
	str r3, [r5]
	movs r2, #128
	ldr r3, [r0, #12]
	lsls r2, r2, #11
	adds r3, r3, r2
	ldr r2, .L_020081f0
	adds r1, r6, #0
	ands r3, r2
	str r3, [r5, #4]
	adds r2, r5, #0
	ldr r3, [r0, #16]
	movs r0, #128
	lsls r0, r0, #13
	str r3, [r5, #8]
	bl Vector_AddPolarOffsetFar
	adds r0, r5, #0
	movs r1, #1
	bl Func_02005680
	cmp r0, #0
	beq .L_020081e2
	ldr r3, [r0, #8]
	adds r1, r6, #0
	str r3, [r5]
	adds r2, r5, #0
	ldr r3, [r0, #12]
	str r3, [r5, #4]
	ldr r3, [r0, #16]
	movs r0, #128
	lsls r0, r0, #13
	str r3, [r5, #8]
	bl Vector_AddPolarOffsetFar
	adds r0, r5, #0
	movs r1, #1
	bl Func_02005680
	cmp r0, #0
	bne .L_020081e6
.L_020081e2:
	bl Func_02005688
.L_020081e6:
	add sp, #12
	pop {r5, r6, pc}
	.2byte 0x0000
.L_020081ec:
	.4byte gPartyState
.L_020081f0:
	.4byte 0xfff80000
	.endif
	.section .text.x0200855c,"ax",%progbits
	.balign 4
	.global Func_0200055c
	.thumb_func
Func_0200055c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	bl Object_GetById
	movs r1, #11
	mov r8, r0
	mov r11, r1
.L_02008574:
	bl Random16Far
	adds r5, r0, #0
	bl Random16Far
	mov r2, r8
	ldr r6, [r2, #8]
	lsls r5, r5, #4
	adds r6, r6, r5
	lsls r0, r0, #4
	subs r6, r6, r0
	bl Random16Far
	adds r5, r0, #0
	bl Random16Far
	mov r1, r8
	ldr r2, [r1, #12]
	lsls r5, r5, #3
	adds r5, r5, r2
	ldr r2, [r1, #16]
	adds r3, r0, #0
	lsls r3, r3, #4
	movs r0, #74
	adds r3, r3, r2
	adds r1, r6, #0
	adds r0, #255
	adds r2, r5, #0
	bl Func_02005498
	adds r6, r0, #0
	cmp r6, #0
	beq .L_0200864c
	bl Random16Far
	movs r2, #224
	lsls r2, r2, #8
	lsrs r0, r0, #2
	adds r7, r0, r2
	bl Random16Far
	lsls r0, r0, #3
	mov r10, r0
	bl Random16Far
	movs r3, #128
	lsrs r0, r0, #1
	lsls r3, r3, #7
	adds r3, r3, r0
	mov r9, r3
	bl Random16Far
	movs r1, #254
	lsls r1, r1, #7
	adds r1, #255
	cmp r0, r1
	bhi .L_020085f0
	adds r0, r6, #0
	ldr r1, .L_02008664
	bl Func_02005490
	b .L_020085f8
.L_020085f0:
	adds r0, r6, #0
	ldr r1, .L_02008668
	bl Func_02005490
.L_020085f8:
	movs r1, #0
	adds r0, r6, #0
	bl ObjectDispatch_SetSingleChildFlags
	movs r2, #128
	lsls r2, r2, #10
	lsls r3, r7, #2
	adds r3, r3, r2
	str r3, [r6, #40]
	adds r0, r7, #0
	bl Math_Cosine
	ldr r5, .L_0200866c
	adds r1, r0, #0
	mov r0, r10
	mov lr, r5
	.2byte 0xf800
	str r0, [r6, #44]
	adds r0, r7, #0
	bl Math_Sine
	adds r1, r0, #0
	mov r0, r10
	mov lr, r5
	.2byte 0xf800
	movs r3, #0
	str r3, [r6, #52]
	mov r3, r9
	str r3, [r6, #24]
	str r3, [r6, #28]
	movs r3, #168
	lsls r3, r3, #7
	adds r3, #122
	str r3, [r6, #72]
	movs r3, #128
	lsls r3, r3, #8
	str r0, [r6, #36]
	str r3, [r6, #68]
	adds r0, r6, #0
	movs r1, #1
	bl Animation_ApplyChildValues
.L_0200864c:
	movs r1, #1
	negs r1, r1
	add r11, r1
	mov r2, r11
	cmp r2, #0
	bge .L_02008574
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_02008664:
	.4byte gData_02005b68
.L_02008668:
	.4byte gData_02005bac
.L_0200866c:
	.4byte IwramMulQ16
	.section .text.x02008e7c,"ax",%progbits
	.ifdef TLA_EDITION_JA
	.balign 4
	.global Func_02000e7c
	.thumb_func
Func_02000e7c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r2, .L_ja_020090ac
	movs r3, #240
	mov r10, r2
	lsls r3, r3, #1
	add r3, r10
	movs r2, #0
	ldrsh r5, [r3, r2]
	movs r3, #241
	lsls r3, r3, #1
	movs r6, #192
	add r3, r10
	lsls r6, r6, #18
	movs r2, #0
	ldrsh r7, [r3, r2]
	ldr r3, [r6, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r0, #137
	adds r2, #88
	str r2, [r3]
	lsls r0, r0, #1
	sub sp, #8
	mov r8, r2
	bl GameFlag_SetBit
	bl Func_020025b0
	ldr r3, .L_ja_020090b0
	cmp r5, r3
	bne .L_ja_02008e46
	movs r0, #0
	bl Func_02005638
	ldr r3, [r6, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	mov r2, r8
	str r2, [r3]
	subs r3, r7, #1
	cmp r3, #1
	bhi .L_ja_02008e1c
	ldr r0, .L_ja_020090b4
	ldr r1, .L_ja_020090b8
	ldr r2, .L_ja_020090bc
	movs r3, #88
	bl Func_02002624
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #66
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_02008db8
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_02005598
	b .L_ja_02008dc2
.L_ja_02008db8:
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_02005598
.L_ja_02008dc2:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #67
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_02008dd6
	movs r0, #0
	bl Func_020006f4
.L_ja_02008dd6:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #68
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_02008dec
	movs r0, #1
	movs r1, #0
	bl Func_02000444
.L_ja_02008dec:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #69
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_02008e02
	movs r0, #1
	movs r1, #0
	bl Func_02000480
.L_ja_02008e02:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #70
	bl GameFlag_Test
	cmp r0, #0
	bne .L_ja_02008e12
	b .L_ja_02009294
.L_ja_02008e12:
	movs r0, #1
	movs r1, #0
	bl Func_020004bc
	b .L_ja_02009294
.L_ja_02008e1c:
	ldr r1, .L_ja_020090c0
	movs r2, #0
	movs r3, #88
	ldr r0, .L_ja_020090c4
	bl Func_02002624
	movs r0, #15
	bl Func_020055b8
	movs r0, #16
	bl Func_020055b8
	movs r0, #15
	bl Func_02004938
	movs r0, #16
	bl Func_02004938
	bl Func_020003b4
	b .L_ja_02009294
.L_ja_02008e46:
	ldr r3, .L_ja_020090c8
	cmp r5, r3
	beq .L_ja_02008e4e
	b .L_ja_02008ff4
.L_ja_02008e4e:
	movs r0, #0
	bl Func_02005638
	ldr r3, [r6, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	mov r2, r8
	str r2, [r3]
	ldr r1, .L_ja_020090cc
	ldr r0, .L_ja_020090d0
	ldr r2, .L_ja_020090d4
	movs r3, #88
	bl Func_02002624
	movs r0, #3
	movs r1, #1
	bl Func_020027cc
	cmp r7, #2
	bne .L_ja_02008e7c
	bl Func_02002604
.L_ja_02008e7c:
	cmp r7, #9
	bne .L_ja_02008f02
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #81
	bl GameFlag_SetBit
	bl Func_02005560
	movs r0, #0
	bl Func_02005648
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r0, r0
	negs r1, r1
	negs r2, r2
	movs r3, #0
	bl Camera_MoveWithinMapBounds
	movs r3, #133
	lsls r3, r3, #2
	add r3, r10
	ldr r0, [r3]
	movs r1, #0
	movs r2, #0
	bl Func_02005598
	movs r1, #0
	movs r2, #0
	movs r0, #17
	bl Func_02005598
	bl Event_StartScreenTransition
	bl Event_WaitTransitionFrames
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r0, #18
	movs r1, #0
	movs r2, #0
	bl Func_02005598
	movs r1, #132
	movs r2, #130
	lsls r1, r1, #17
	lsls r2, r2, #18
	movs r0, #17
	bl Func_02005598
	movs r0, #136
	bl Func_020056c0
	movs r0, #60
	bl FieldEvent_WaitFrames
	movs r0, #10
	adds r0, #255
	bl GameFlag_SetBit
	movs r0, #3
	bl Func_020055f0
	b .L_ja_02009296
.L_ja_02008f02:
	movs r0, #212
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_02008f4a
	movs r1, #150
	movs r2, #144
	movs r0, #8
	lsls r1, r1, #18
	lsls r2, r2, #16
	bl Func_02005598
	movs r1, #136
	movs r2, #208
	movs r0, #9
	lsls r1, r1, #18
	lsls r2, r2, #16
	bl Func_02005598
	movs r1, #136
	movs r2, #240
	movs r0, #10
	lsls r1, r1, #18
	lsls r2, r2, #16
	bl Func_02005598
	movs r0, #136
	lsls r0, r0, #2
	bl GameFlag_SetBit
	movs r0, #145
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
.L_ja_02008f4a:
	movs r0, #8
	bl Func_020055b8
	movs r0, #9
	bl Func_020055b8
	movs r0, #10
	bl Func_020055b8
	movs r0, #8
	bl Func_02004938
	movs r0, #9
	bl Func_02004938
	movs r0, #10
	bl Func_02004938
	movs r0, #11
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r5, #2
	orrs r3, r5
	strb r3, [r0]
	movs r0, #12
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	orrs r5, r3
	strb r5, [r0]
	movs r0, #136
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_02008f9e
	movs r0, #11
	bl Func_02000c80
.L_ja_02008f9e:
	movs r0, #145
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_02008fb2
	movs r0, #12
	bl Func_02000c80
.L_ja_02008fb2:
	bl Func_020009ec
	movs r0, #212
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_02008fce
	movs r0, #18
	movs r1, #0
	movs r2, #0
	bl Func_02005598
	b .L_ja_02008fd8
.L_ja_02008fce:
	movs r0, #17
	movs r1, #0
	movs r2, #0
	bl Func_02005598
.L_ja_02008fd8:
	movs r0, #164
	lsls r0, r0, #4
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_02008fe8
	bl Func_02000d80
.L_ja_02008fe8:
	movs r1, #144
	ldr r0, .L_ja_020090d8
	lsls r1, r1, #3
	bl Scheduler_AddOrUpdateCallback
	b .L_ja_02009294
.L_ja_02008ff4:
	ldr r3, .L_ja_020090dc
	cmp r5, r3
	beq .L_ja_02008ffc
	b .L_ja_02009170
.L_ja_02008ffc:
	movs r0, #0
	bl Func_02005638
	ldr r3, [r6, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	mov r2, r8
	str r2, [r3]
	movs r0, #1
	bl Func_020015a4
	bl Func_02005284
	movs r0, #2
	bl Func_020053bc
	movs r0, #11
	movs r1, #6
	bl Func_0200538c
	movs r0, #214
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_02009072
	movs r3, #5
	str r3, [sp, #4]
	movs r5, #71
	movs r0, #71
	movs r1, #24
	movs r2, #7
	movs r3, #1
	str r5, [sp, #0]
	bl Func_020054f0
	movs r6, #7
	movs r0, #7
	movs r1, #24
	movs r2, #7
	movs r3, #1
	str r6, [sp, #0]
	str r6, [sp, #4]
	bl Func_020054e8
	movs r0, #7
	movs r1, #88
	movs r2, #7
	movs r3, #1
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl Func_020054e8
	movs r0, #11
	movs r1, #0
	movs r2, #0
	bl Func_02005598
.L_ja_02009072:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #55
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200908e
	movs r1, #240
	movs r2, #148
	movs r0, #12
	lsls r1, r1, #15
	lsls r2, r2, #17
	bl Func_02005598
.L_ja_0200908e:
	adds r3, r7, #0
	subs r3, #14
	cmp r3, #1
	bhi .L_ja_020090ec
	ldr r1, .L_ja_020090e0
	ldr r0, .L_ja_020090e4
	ldr r2, .L_ja_020090e8
	movs r3, #88
	bl Func_02002624
	movs r0, #0
	movs r1, #1
	bl Func_020027cc
	b .L_ja_020090f8
.L_ja_020090ac:
	.4byte gPartyState
.L_ja_020090b0:
	.4byte 0x00000124
.L_ja_020090b4:
	.4byte gData_02005bf0
.L_ja_020090b8:
	.4byte gData_02005bf6
.L_ja_020090bc:
	.4byte Func_02000514
.L_ja_020090c0:
	.4byte gData_02005c0e
.L_ja_020090c4:
	.4byte gData_02005bfc
.L_ja_020090c8:
	.4byte 0x00000125
.L_ja_020090cc:
	.4byte gData_02005c26
.L_ja_020090d0:
	.4byte gData_02005c14
.L_ja_020090d4:
	.4byte Func_02001468
.L_ja_020090d8:
	.4byte Func_02000e68
.L_ja_020090dc:
	.4byte 0x00000126
.L_ja_020090e0:
	.4byte gData_02005c32
.L_ja_020090e4:
	.4byte gData_02005c2c
.L_ja_020090e8:
	.4byte Func_02001490
.L_ja_020090ec:
	ldr r0, .L_ja_020092a0
	ldr r1, .L_ja_020092a4
	movs r2, #0
	movs r3, #88
	bl Func_02002624
.L_ja_020090f8:
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_02009106
	b .L_ja_02009294
.L_ja_02009106:
	cmp r7, #2
	beq .L_ja_02009126
	cmp r7, #4
	beq .L_ja_02009126
	cmp r7, #6
	beq .L_ja_02009126
	cmp r7, #7
	beq .L_ja_02009126
	cmp r7, #8
	beq .L_ja_02009126
	cmp r7, #10
	beq .L_ja_02009126
	cmp r7, #11
	beq .L_ja_02009126
	cmp r7, #13
	bne .L_ja_02009134
.L_ja_02009126:
	ldr r3, .L_ja_020092a8
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Func_020056b0
.L_ja_02009134:
	cmp r7, #3
	beq .L_ja_02009148
	cmp r7, #5
	beq .L_ja_02009148
	cmp r7, #9
	beq .L_ja_02009148
	cmp r7, #12
	beq .L_ja_02009148
	cmp r7, #14
	bne .L_ja_02009156
.L_ja_02009148:
	ldr r3, .L_ja_020092a8
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Func_020056b8
.L_ja_02009156:
	cmp r7, #16
	beq .L_ja_0200915c
	b .L_ja_02009294
.L_ja_0200915c:
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200916a
	b .L_ja_02009294
.L_ja_0200916a:
	bl Func_02003d80
	b .L_ja_02009294
.L_ja_02009170:
	ldr r3, .L_ja_020092ac
	cmp r5, r3
	beq .L_ja_02009178
	b .L_ja_02009294
.L_ja_02009178:
	movs r0, #0
	bl Func_02005638
	ldr r3, [r6, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	mov r2, r8
	str r2, [r3]
	bl Func_02005670
	movs r1, #8
	movs r2, #9
	movs r0, #0
	bl Func_02005678
	movs r0, #8
	bl Object_GetById
	movs r5, #1
	adds r0, #98
	strb r5, [r0]
	movs r0, #9
	bl Object_GetById
	subs r3, r7, #5
	adds r0, #98
	strb r5, [r0]
	cmp r3, #1
	bls .L_ja_020091b8
	cmp r7, #8
	bne .L_ja_020091c6
.L_ja_020091b8:
	ldr r0, .L_ja_020092b0
	ldr r1, .L_ja_020092b4
	movs r2, #0
	movs r3, #88
	bl Func_02002624
	b .L_ja_020091e6
.L_ja_020091c6:
	cmp r7, #11
	beq .L_ja_020091da
	cmp r7, #13
	beq .L_ja_020091da
	cmp r7, #14
	beq .L_ja_020091da
	cmp r7, #1
	beq .L_ja_020091da
	cmp r7, #15
	bne .L_ja_020091e6
.L_ja_020091da:
	ldr r0, .L_ja_020092b8
	ldr r1, .L_ja_020092bc
	movs r2, #0
	movs r3, #88
	bl Func_02002624
.L_ja_020091e6:
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_ja_02009216
	cmp r7, #15
	bne .L_ja_02009204
	movs r1, #166
	movs r2, #178
	movs r0, #23
	lsls r1, r1, #18
	lsls r2, r2, #18
	bl Func_02005598
.L_ja_02009204:
	cmp r7, #11
	bne .L_ja_02009216
	movs r1, #134
	movs r2, #138
	movs r0, #17
	lsls r1, r1, #18
	lsls r2, r2, #18
	bl Func_02005598
.L_ja_02009216:
	movs r0, #16
	bl Object_GetById
	movs r5, #0
	adds r0, #85
	strb r5, [r0]
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #56
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200924e
	movs r1, #202
	movs r2, #156
	lsls r1, r1, #18
	lsls r2, r2, #17
	movs r0, #16
	bl Func_02005598
	movs r0, #16
	bl Object_GetById
	str r5, [r0, #12]
	movs r0, #16
	bl Object_GetById
	str r5, [r0, #20]
.L_ja_0200924e:
	ldr r3, .L_ja_020092a8
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r2, #64
	orrs r3, r2
	strb r3, [r0]
	movs r0, #10
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r2, #192
	orrs r3, r2
	strb r3, [r0]
	movs r0, #11
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r5, #128
	orrs r3, r5
	strb r3, [r0]
	movs r0, #12
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
.L_ja_02009294:
	movs r0, #0
.L_ja_02009296:
	add sp, #8
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_ja_020092a0:
	.4byte gData_02005c38
.L_ja_020092a4:
	.4byte gData_02005c46
.L_ja_020092a8:
	.4byte gPartyState
.L_ja_020092ac:
	.4byte 0x00000127
.L_ja_020092b0:
	.4byte gData_02005c4c
.L_ja_020092b4:
	.4byte gData_02005c5a
.L_ja_020092b8:
	.4byte gData_02005c60
.L_ja_020092bc:
	.4byte gData_02005c6e
	.else
	.balign 4
	.global Func_02000e7c
	.thumb_func
Func_02000e7c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r2, .L_020091f4
	movs r3, #240
	mov r10, r2
	lsls r3, r3, #1
	add r3, r10
	movs r2, #0
	ldrsh r5, [r3, r2]
	movs r3, #241
	lsls r3, r3, #1
	movs r6, #192
	add r3, r10
	lsls r6, r6, #18
	movs r2, #0
	ldrsh r7, [r3, r2]
	ldr r3, [r6, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r0, #137
	adds r2, #88
	str r2, [r3]
	lsls r0, r0, #1
	sub sp, #8
	mov r8, r2
	bl GameFlag_SetBit
	bl Func_020025b0
	ldr r3, .L_020091f8
	cmp r5, r3
	bne .L_02008f8e
	movs r0, #0
	bl Func_02005638
	ldr r3, [r6, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	mov r2, r8
	str r2, [r3]
	subs r3, r7, #1
	cmp r3, #1
	bhi .L_02008f64
	ldr r0, .L_020091fc
	ldr r1, .L_02009200
	ldr r2, .L_02009204
	movs r3, #88
	bl Func_02002624
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #66
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008f00
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_02005598
	b .L_02008f0a
.L_02008f00:
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_02005598
.L_02008f0a:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #67
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008f1e
	movs r0, #0
	bl Func_020006f4
.L_02008f1e:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #68
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008f34
	movs r0, #1
	movs r1, #0
	bl Func_02000444
.L_02008f34:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #69
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008f4a
	movs r0, #1
	movs r1, #0
	bl Func_02000480
.L_02008f4a:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #70
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008f5a
	b .L_020093e0
.L_02008f5a:
	movs r0, #1
	movs r1, #0
	bl Func_020004bc
	b .L_020093e0
.L_02008f64:
	ldr r1, .L_02009208
	movs r2, #0
	movs r3, #88
	ldr r0, .L_0200920c
	bl Func_02002624
	movs r0, #15
	bl Func_020055b8
	movs r0, #16
	bl Func_020055b8
	movs r0, #15
	bl Func_02004938
	movs r0, #16
	bl Func_02004938
	bl Func_020003b4
	b .L_020093e0
.L_02008f8e:
	ldr r3, .L_02009210
	cmp r5, r3
	beq .L_02008f96
	b .L_0200913c
.L_02008f96:
	movs r0, #0
	bl Func_02005638
	ldr r3, [r6, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	mov r2, r8
	str r2, [r3]
	ldr r1, .L_02009214
	ldr r0, .L_02009218
	ldr r2, .L_0200921c
	movs r3, #88
	bl Func_02002624
	movs r0, #3
	movs r1, #1
	bl Func_020027cc
	cmp r7, #2
	bne .L_02008fc4
	bl Func_02002604
.L_02008fc4:
	cmp r7, #9
	bne .L_0200904a
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #81
	bl GameFlag_SetBit
	bl Func_02005560
	movs r0, #0
	bl Func_02005648
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r0, r0
	negs r1, r1
	negs r2, r2
	movs r3, #0
	bl Camera_MoveWithinMapBounds
	movs r3, #133
	lsls r3, r3, #2
	add r3, r10
	ldr r0, [r3]
	movs r1, #0
	movs r2, #0
	bl Func_02005598
	movs r1, #0
	movs r2, #0
	movs r0, #17
	bl Func_02005598
	bl Event_StartScreenTransition
	bl Event_WaitTransitionFrames
	movs r0, #30
	bl FieldEvent_WaitFrames
	movs r0, #18
	movs r1, #0
	movs r2, #0
	bl Func_02005598
	movs r1, #132
	movs r2, #130
	lsls r1, r1, #17
	lsls r2, r2, #18
	movs r0, #17
	bl Func_02005598
	movs r0, #136
	bl Func_020056c0
	movs r0, #60
	bl FieldEvent_WaitFrames
	movs r0, #10
	adds r0, #255
	bl GameFlag_SetBit
	movs r0, #3
	bl Func_020055f0
	b .L_020093e2
.L_0200904a:
	movs r0, #212
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009092
	movs r1, #150
	movs r2, #144
	movs r0, #8
	lsls r1, r1, #18
	lsls r2, r2, #16
	bl Func_02005598
	movs r1, #136
	movs r2, #208
	movs r0, #9
	lsls r1, r1, #18
	lsls r2, r2, #16
	bl Func_02005598
	movs r1, #136
	movs r2, #240
	movs r0, #10
	lsls r1, r1, #18
	lsls r2, r2, #16
	bl Func_02005598
	movs r0, #136
	lsls r0, r0, #2
	bl GameFlag_SetBit
	movs r0, #145
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
.L_02009092:
	movs r0, #8
	bl Func_020055b8
	movs r0, #9
	bl Func_020055b8
	movs r0, #10
	bl Func_020055b8
	movs r0, #8
	bl Func_02004938
	movs r0, #9
	bl Func_02004938
	movs r0, #10
	bl Func_02004938
	movs r0, #11
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r5, #2
	orrs r3, r5
	strb r3, [r0]
	movs r0, #12
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	orrs r5, r3
	strb r5, [r0]
	movs r0, #136
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020090e6
	movs r0, #11
	bl Func_02000c80
.L_020090e6:
	movs r0, #145
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020090fa
	movs r0, #12
	bl Func_02000c80
.L_020090fa:
	bl Func_020009ec
	movs r0, #212
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009116
	movs r0, #18
	movs r1, #0
	movs r2, #0
	bl Func_02005598
	b .L_02009120
.L_02009116:
	movs r0, #17
	movs r1, #0
	movs r2, #0
	bl Func_02005598
.L_02009120:
	movs r0, #164
	lsls r0, r0, #4
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009130
	bl Func_02000d80
.L_02009130:
	movs r1, #144
	ldr r0, .L_02009220
	lsls r1, r1, #3
	bl Scheduler_AddOrUpdateCallback
	b .L_020093e0
.L_0200913c:
	ldr r3, .L_02009224
	cmp r5, r3
	beq .L_02009144
	b .L_020092b8
.L_02009144:
	movs r0, #0
	bl Func_02005638
	ldr r3, [r6, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	mov r2, r8
	str r2, [r3]
	movs r0, #1
	bl Func_020015a4
	bl Func_02005284
	movs r0, #2
	bl Func_020053bc
	movs r0, #11
	movs r1, #6
	bl Func_0200538c
	movs r0, #214
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020091ba
	movs r3, #5
	str r3, [sp, #4]
	movs r5, #71
	movs r0, #71
	movs r1, #24
	movs r2, #7
	movs r3, #1
	str r5, [sp, #0]
	bl Func_020054f0
	movs r6, #7
	movs r0, #7
	movs r1, #24
	movs r2, #7
	movs r3, #1
	str r6, [sp, #0]
	str r6, [sp, #4]
	bl Func_020054e8
	movs r0, #7
	movs r1, #88
	movs r2, #7
	movs r3, #1
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl Func_020054e8
	movs r0, #11
	movs r1, #0
	movs r2, #0
	bl Func_02005598
.L_020091ba:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #55
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020091d6
	movs r1, #240
	movs r2, #148
	movs r0, #12
	lsls r1, r1, #15
	lsls r2, r2, #17
	bl Func_02005598
.L_020091d6:
	adds r3, r7, #0
	subs r3, #14
	cmp r3, #1
	bhi .L_02009234
	ldr r1, .L_02009228
	ldr r0, .L_0200922c
	ldr r2, .L_02009230
	movs r3, #88
	bl Func_02002624
	movs r0, #0
	movs r1, #1
	bl Func_020027cc
	b .L_02009240
.L_020091f4:
	.4byte gPartyState
.L_020091f8:
	.4byte 0x00000124
.L_020091fc:
	.4byte gData_02005bf0
.L_02009200:
	.4byte gData_02005bf6
.L_02009204:
	.4byte Func_02000514
.L_02009208:
	.4byte gData_02005c0e
.L_0200920c:
	.4byte gData_02005bfc
.L_02009210:
	.4byte 0x00000125
.L_02009214:
	.4byte gData_02005c26
.L_02009218:
	.4byte gData_02005c14
.L_0200921c:
	.4byte Func_02001468
.L_02009220:
	.4byte Func_02000e68
.L_02009224:
	.4byte 0x00000126
.L_02009228:
	.4byte gData_02005c32
.L_0200922c:
	.4byte gData_02005c2c
.L_02009230:
	.4byte Func_02001490
.L_02009234:
	ldr r0, .L_020093ec
	ldr r1, .L_020093f0
	movs r2, #0
	movs r3, #88
	bl Func_02002624
.L_02009240:
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200924e
	b .L_020093e0
.L_0200924e:
	cmp r7, #2
	beq .L_0200926e
	cmp r7, #4
	beq .L_0200926e
	cmp r7, #6
	beq .L_0200926e
	cmp r7, #7
	beq .L_0200926e
	cmp r7, #8
	beq .L_0200926e
	cmp r7, #10
	beq .L_0200926e
	cmp r7, #11
	beq .L_0200926e
	cmp r7, #13
	bne .L_0200927c
.L_0200926e:
	ldr r3, .L_020093f4
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Func_020056b0
.L_0200927c:
	cmp r7, #3
	beq .L_02009290
	cmp r7, #5
	beq .L_02009290
	cmp r7, #9
	beq .L_02009290
	cmp r7, #12
	beq .L_02009290
	cmp r7, #14
	bne .L_0200929e
.L_02009290:
	ldr r3, .L_020093f4
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Func_020056b8
.L_0200929e:
	cmp r7, #16
	beq .L_020092a4
	b .L_020093e0
.L_020092a4:
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020092b2
	b .L_020093e0
.L_020092b2:
	bl Func_02003d80
	b .L_020093e0
.L_020092b8:
	ldr r3, .L_020093f8
	cmp r5, r3
	beq .L_020092c0
	b .L_020093e0
.L_020092c0:
	movs r0, #0
	bl Func_02005638
	ldr r3, [r6, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	mov r2, r8
	str r2, [r3]
	bl Func_02005670
	movs r1, #8
	movs r2, #9
	movs r0, #0
	bl Func_02005678
	movs r0, #8
	bl Object_GetById
	movs r5, #1
	adds r0, #98
	strb r5, [r0]
	movs r0, #9
	bl Object_GetById
	subs r3, r7, #5
	adds r0, #98
	strb r5, [r0]
	cmp r3, #1
	bls .L_02009300
	cmp r7, #8
	bne .L_02009312
.L_02009300:
	ldr r1, .L_020093fc
	movs r2, #0
	movs r3, #88
	ldr r0, .L_02009400
	bl Func_02002624
	bl Func_020002ec
	b .L_02009332
.L_02009312:
	cmp r7, #11
	beq .L_02009326
	cmp r7, #13
	beq .L_02009326
	cmp r7, #14
	beq .L_02009326
	cmp r7, #1
	beq .L_02009326
	cmp r7, #15
	bne .L_02009332
.L_02009326:
	ldr r0, .L_02009404
	ldr r1, .L_02009408
	movs r2, #0
	movs r3, #88
	bl Func_02002624
.L_02009332:
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02009362
	cmp r7, #15
	bne .L_02009350
	movs r1, #166
	movs r2, #178
	movs r0, #23
	lsls r1, r1, #18
	lsls r2, r2, #18
	bl Func_02005598
.L_02009350:
	cmp r7, #11
	bne .L_02009362
	movs r1, #134
	movs r2, #138
	movs r0, #17
	lsls r1, r1, #18
	lsls r2, r2, #18
	bl Func_02005598
.L_02009362:
	movs r0, #16
	bl Object_GetById
	movs r5, #0
	adds r0, #85
	strb r5, [r0]
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #56
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200939a
	movs r1, #202
	movs r2, #156
	lsls r1, r1, #18
	lsls r2, r2, #17
	movs r0, #16
	bl Func_02005598
	movs r0, #16
	bl Object_GetById
	str r5, [r0, #12]
	movs r0, #16
	bl Object_GetById
	str r5, [r0, #20]
.L_0200939a:
	ldr r3, .L_020093f4
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r2, #64
	orrs r3, r2
	strb r3, [r0]
	movs r0, #10
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r2, #192
	orrs r3, r2
	strb r3, [r0]
	movs r0, #11
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r5, #128
	orrs r3, r5
	strb r3, [r0]
	movs r0, #12
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
.L_020093e0:
	movs r0, #0
.L_020093e2:
	add sp, #8
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_020093ec:
	.4byte gData_02005c38
.L_020093f0:
	.4byte gData_02005c46
.L_020093f4:
	.4byte gPartyState
.L_020093f8:
	.4byte 0x00000127
.L_020093fc:
	.4byte gData_02005c5a
.L_02009400:
	.4byte gData_02005c4c
.L_02009404:
	.4byte gData_02005c60
.L_02009408:
	.4byte gData_02005c6e
	.endif
	.section .text.x020094e8,"ax",%progbits
	.balign 4
	.global Func_020014e8
	.thumb_func
Func_020014e8:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r3, .L_02009594
	sub sp, #32
	ldr r0, [r3]
	cmp r0, #0
	bge .L_020094fa
	adds r0, #3
.L_020094fa:
	asrs r0, r0, #2
	movs r1, #5
	bl __modsi3
	ldr r3, .L_02009598
	mov r8, r0
	ldr r3, [r3]
	cmp r3, #0
	beq .L_0200954e
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
	beq .L_0200952e
	movs r3, #192
	lsls r3, r3, #2
	adds r3, #255
	ands r3, r1
	cmp r3, #153
	bne .L_0200958a
.L_0200952e:
	movs r1, #192
	lsls r1, r1, #4
	adds r1, #164
	adds r3, r2, r1
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	bne .L_0200958a
	movs r0, #175
	lsls r0, r0, #1
	adds r3, r2, r0
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	bne .L_0200958a
.L_0200954e:
	movs r5, #0
	movs r6, #4
.L_02009552:
	mov r2, r8
	adds r0, r2, r5
	movs r1, #5
	mov r7, sp
	bl __modsi3
	ldr r3, .L_0200959c
	lsls r0, r0, #1
	ldrh r3, [r3, r6]
	adds r5, #1
	strh r3, [r7, r0]
	adds r6, #2
	cmp r5, #4
	ble .L_02009552
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	adds r0, r7, #0
	ldr r1, .L_020095a0
	adds r2, #2
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r2, .L_02009594
	ldr r3, [r2]
	adds r3, #1
	str r3, [r2]
.L_0200958a:
	add sp, #32
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009594:
	.4byte gData_0200703c
.L_02009598:
	.4byte gData_02007040
.L_0200959c:
	.4byte gData_02007068
.L_020095a0:
	.4byte 0x05000184
	.section .text.x0200967c,"ax",%progbits
	.balign 4
	.global Func_0200167c
	.thumb_func
Func_0200167c:
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
	ldr r3, .L_02009788
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	adds r6, r0, #0
	ldr r7, [r6, #104]
	bl Func_02005560
	movs r0, #0
	bl Func_02005648
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
	bl ObjectDispatch_SetSingleChildFlags
	movs r3, #99
	adds r3, r3, r7
	mov r8, r3
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_0200971c
.L_020096d6:
	ldr r3, [r7, #8]
	ldr r2, .L_0200978c
	str r3, [r6, #8]
	ldr r3, [r7, #12]
	adds r3, r3, r5
	str r3, [r6, #12]
	ldr r3, [r7, #16]
	str r3, [r6, #16]
	cmp r5, r2
	bgt .L_020096f2
	movs r3, #200
	lsls r3, r3, #5
	adds r3, #153
	adds r5, r5, r3
.L_020096f2:
	ldr r3, .L_02009790
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
	bl WaitFrames
	mov r2, r8
	ldrb r3, [r2]
	cmp r3, #0
	bne .L_020096d6
.L_0200971c:
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
	ldr r0, .L_02009784
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
	bl ObjectDispatch_SetSingleChildFlags
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r1, r1
	negs r2, r2
	movs r3, #0
	negs r0, r0
	b .L_02009794
.L_02009784:
	.4byte 0x00000001
.L_02009788:
	.4byte gPartyState
.L_0200978c:
	.4byte 0x0003ffff
.L_02009790:
	.4byte gFrameCount
.L_02009794:
	bl Camera_MoveWithinMapBounds
	bl Func_020055e8
	movs r3, #128
	adds r7, r0, #0
	lsls r3, r3, #12
	str r3, [r7, #48]
	movs r3, #128
	ldr r5, .L_020097dc
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
	bl Func_020054b8
	ldr r3, [r6, #20]
	movs r2, #128
	lsls r2, r2, #13
	adds r3, r3, r2
	ldr r2, [r6, #12]
	movs r5, #0
	cmp r2, r3
	ble .L_020097fa
	b .L_020097e0
	.2byte 0x0000
.L_020097dc:
	.4byte 0x00000000
.L_020097e0:
	movs r0, #1
	adds r5, #1
	bl WaitFrames
	cmp r5, #59
	bgt .L_020097fa
	ldr r3, [r6, #20]
	movs r2, #128
	lsls r2, r2, #13
	adds r3, r3, r2
	ldr r2, [r6, #12]
	cmp r2, r3
	bgt .L_020097e0
.L_020097fa:
	movs r0, #127
	bl Func_020056c0
	ldr r3, [r6, #40]
	movs r5, #0
	cmp r3, #0
	beq .L_0200981a
.L_02009808:
	movs r0, #1
	adds r5, #1
	bl WaitFrames
	cmp r5, #59
	bgt .L_0200981a
	ldr r3, [r6, #40]
	cmp r3, #0
	bne .L_02009808
.L_0200981a:
	adds r0, r7, #0
	bl Func_020054c0
	ldr r5, .L_02009864
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	ldr r0, [r5]
	bl Object_GetById
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
	bl Func_02005568
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_02009864:
	.4byte gPartyState
	.section .text.x02009894,"ax",%progbits
	.balign 4
	.global Func_02001894
	.thumb_func
Func_02001894:
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
	ldr r3, .L_020098fc
	lsls r2, r2, #2
	adds r3, r3, r2
	adds r6, r0, #0
	ldr r0, [r3]
	sub sp, #12
	bl Object_GetById
	movs r3, #179
	lsls r3, r3, #1
	add r3, r10
	movs r2, #0
	ldrsh r3, [r3, r2]
	adds r7, r0, #0
	cmp r3, #0
	bne .L_020098f2
	movs r3, #173
	lsls r3, r3, #1
	add r3, r10
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	bne .L_020098f2
	movs r3, #175
	lsls r3, r3, #1
	add r3, r10
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	bne .L_020098f2
	movs r3, #180
	lsls r3, r3, #1
	add r3, r10
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	beq .L_02009900
.L_020098f2:
	adds r0, r6, #0
	movs r1, #0
	bl ObjectDispatch_SetAnimationSpeed
	b .L_02009a3e
.L_020098fc:
	.4byte gPartyState
.L_02009900:
	adds r0, r6, #0
	movs r1, #16
	bl ObjectDispatch_SetAnimationSpeed
	adds r3, r6, #0
	adds r3, #100
	ldrh r2, [r3]
	adds r2, #1
	strh r2, [r3]
	movs r3, #31
	ands r3, r2
	cmp r3, #31
	bne .L_02009920
	movs r0, #231
	bl Func_020056c0
.L_02009920:
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
	bl Func_020056a0
	cmp r0, #255
	beq .L_02009a22
	ldr r3, [r6, #8]
	mov r5, sp
	str r3, [r5]
	adds r0, r5, #0
	ldr r3, [r6, #12]
	str r3, [r5, #4]
	ldr r3, [r6, #16]
	str r3, [r5, #8]
	bl Func_02005658
	ldr r5, [r5]
	movs r3, #136
	lsls r3, r3, #17
	cmp r5, r3
	bgt .L_02009a22
	ldr r2, .L_02009a10
	cmp r5, r2
	blt .L_02009a22
	movs r3, #98
	adds r3, r3, r6
	mov r9, r3
	ldrb r3, [r3]
	cmp r3, #0
	bne .L_020099e8
	ldr r2, [r7, #12]
	ldr r3, [r6, #12]
	subs r5, r2, r3
	cmp r5, #0
	bge .L_02009980
	subs r5, r3, r2
.L_02009980:
	adds r0, r7, #0
	adds r1, r6, #0
	movs r2, #0
	adds r0, #8
	adds r1, #8
	mov r8, r2
	bl Func_02001868
	cmp r0, #12
	bgt .L_020099a0
	movs r3, #192
	lsls r3, r3, #12
	cmp r5, r3
	bge .L_020099a0
	movs r2, #1
	mov r8, r2
.L_020099a0:
	mov r3, r8
	cmp r3, #0
	beq .L_020099e8
	movs r0, #130
	lsls r0, r0, #1
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020099e8
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
	ldr r3, .L_02009a14
	movs r2, #128
	ldr r0, .L_02009a0c
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
.L_020099e8:
	ldrh r0, [r6, #6]
	bl Math_Cosine
	ldr r1, [r6, #48]
	ldr r5, .L_02009a18
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
	b .L_02009a1c
.L_02009a0c:
	.4byte 0x00000000
.L_02009a10:
	.4byte 0xffe00000
.L_02009a14:
	.4byte gPartyState
.L_02009a18:
	.4byte IwramMulQ16
.L_02009a1c:
	adds r3, r3, r0
	str r3, [r6, #16]
	b .L_02009a3e
.L_02009a22:
	adds r3, r6, #0
	adds r3, #99
	movs r5, #0
	strb r5, [r3]
	ldr r1, .L_02009a4c
	adds r0, r6, #0
	str r5, [r6, #108]
	bl Func_02005490
	movs r0, #228
	bl Func_020056c0
	ldr r3, .L_02009a50
	str r5, [r3]
.L_02009a3e:
	add sp, #12
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009a4c:
	.4byte gData_02007044
.L_02009a50:
	.4byte Data_02007064
	.section .text.x02009a54,"ax",%progbits
	.global Func_02001a54
	.thumb_func
Func_02001a54:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r5, r0, #0
	movs r0, #222
	sub sp, #68
	bl Func_020056c0
	ldrh r0, [r5, #6]
	bl Math_Cosine
	adds r1, r0, #0
	movs r0, #128
	ldr r6, .L_02009b08
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
	bl Func_02005498
	movs r1, #2
	adds r7, r0, #0
	bl Func_02005480
	adds r0, r7, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
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
	ldr r2, .L_02009b04
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
	ldr r3, .L_02009b0c
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
	ldr r3, .L_02009b10
	ldr r1, [r5, #12]
	adds r2, r2, r3
	movs r3, #192
	lsls r3, r3, #10
	str r3, [sp, #8]
	b .L_02009b14
.L_02009b04:
	.4byte 0x00000000
.L_02009b08:
	.4byte IwramMulQ16
.L_02009b0c:
	.4byte Func_02001894
.L_02009b10:
	.4byte 0xfffa0000
.L_02009b14:
	movs r3, #0
	str r6, [sp, #0]
	str r6, [sp, #4]
	str r4, [sp, #12]
	bl Func_020035b8
	adds r0, r7, #0
	add sp, #68
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x02009b2c,"ax",%progbits
	.global Func_02001b2c
	.thumb_func
Func_02001b2c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, .L_02009bc8
	sub sp, #56
	ldr r7, [r3]
	movs r3, #7
	ands r7, r3
	mov r10, r0
	cmp r7, #0
	bne .L_02009bba
	add r6, sp, #16
	movs r3, #3
	str r3, [r6]
	movs r3, #179
	lsls r3, r3, #8
	adds r3, #51
	str r3, [r6, #8]
	str r3, [r6, #12]
	movs r3, #14
	str r3, [r6, #4]
	bl Random16Far
	mov r2, r10
	lsls r3, r0, #3
	ldr r2, [r2, #8]
	adds r3, r3, r0
	lsrs r3, r3, #16
	subs r3, #4
	lsls r3, r3, #16
	mov r8, r2
	add r8, r3
	bl Random16Far
	lsls r3, r0, #2
	adds r3, r3, r0
	lsls r3, r3, #2
	lsrs r3, r3, #16
	movs r2, #32
	subs r2, r2, r3
	mov r3, r10
	ldr r5, [r3, #12]
	lsls r2, r2, #16
	adds r5, r5, r2
	bl Random16Far
	adds r3, r0, #0
	lsls r0, r3, #2
	adds r0, r0, r3
	lsrs r0, r0, #16
	movs r2, #160
	lsls r2, r2, #11
	lsls r0, r0, #16
	adds r0, r0, r2
	movs r1, #10
	bl __divsi3
	mov r3, r10
	ldr r2, [r3, #16]
	movs r3, #176
	lsls r3, r3, #12
	str r0, [sp, #0]
	str r3, [sp, #8]
	mov r0, r8
	adds r1, r5, #0
	movs r3, #0
	str r7, [sp, #4]
	str r6, [sp, #12]
	bl Func_020035b8
.L_02009bba:
	movs r0, #0
	add sp, #56
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009bc8:
	.4byte gFrameCount
	.section .text.x02009bcc,"ax",%progbits
	.global Func_02001bcc
	.thumb_func
Func_02001bcc:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r0, #9
	bl Object_GetById
	adds r6, r0, #0
	movs r0, #10
	bl Object_GetById
	adds r7, r0, #0
	movs r0, #23
	bl Object_GetById
	adds r5, r0, #0
	ldr r2, [r5, #80]
	movs r1, #128
	mov r8, r2
	movs r2, #248
	movs r0, #24
	lsls r1, r1, #18
	lsls r2, r2, #16
	bl Func_02005598
	movs r1, #128
	movs r2, #248
	movs r0, #23
	lsls r1, r1, #18
	lsls r2, r2, #16
	bl Func_02005598
	movs r1, #236
	movs r2, #128
	movs r0, #9
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_02005598
	movs r1, #138
	movs r2, #128
	lsls r2, r2, #17
	movs r0, #10
	lsls r1, r1, #18
	bl Func_02005598
	movs r0, #24
	bl Object_GetById
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
	beq .L_02009ca8
	ldr r3, [r6, #12]
	ldr r2, .L_02009d40
	movs r0, #9
	adds r3, r3, r2
	str r3, [r6, #12]
	ldr r2, .L_02009d44
	ldr r3, [r5, #12]
	adds r3, r3, r2
	str r3, [r5, #12]
	ldr r2, .L_02009d48
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
	bl Object_GetById
	movs r1, #4
	bl Animation_ApplyChildValues
	movs r0, #11
	bl Object_GetById
	movs r1, #4
	bl Animation_ApplyChildValues
.L_02009ca8:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #11
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009cf2
	ldr r3, [r7, #12]
	ldr r2, .L_02009d40
	movs r0, #10
	adds r3, r3, r2
	str r3, [r7, #12]
	ldr r2, .L_02009d44
	ldr r3, [r5, #12]
	adds r3, r3, r2
	str r3, [r5, #12]
	ldr r2, .L_02009d4c
	ldr r3, [r5, #8]
	adds r3, r3, r2
	str r3, [r5, #8]
	mov r2, r8
	ldrh r3, [r2, #18]
	ldr r2, .L_02009d50
	adds r3, r3, r2
	mov r2, r8
	strh r3, [r2, #18]
	bl Object_GetById
	movs r1, #4
	bl Animation_ApplyChildValues
	movs r0, #12
	bl Object_GetById
	movs r1, #4
	bl Animation_ApplyChildValues
.L_02009cf2:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #10
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009d32
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #11
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009d32
	ldr r3, [r6, #12]
	ldr r2, .L_02009d54
	adds r3, r3, r2
	str r3, [r6, #12]
	ldr r3, [r7, #12]
	adds r3, r3, r2
	str r3, [r7, #12]
	ldr r2, .L_02009d58
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
.L_02009d32:
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009d40:
	.4byte 0x00066640
.L_02009d44:
	.4byte 0x0001eb80
.L_02009d48:
	.4byte 0xfffd70c0
.L_02009d4c:
	.4byte 0x00028f40
.L_02009d50:
	.4byte 0xfffff800
.L_02009d54:
	.4byte 0x00199900
.L_02009d58:
	.4byte 0x001b8480
	.section .text.x02009e20,"ax",%progbits
	.balign 4
	.global Func_02001e20
	.thumb_func
Func_02001e20:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	ldr r3, .L_02009f7c
	ldr r2, .L_02009f80
	mov r10, r3
	movs r3, #133
	lsls r3, r3, #2
	add r3, r10
	adds r6, r0, #0
	ldr r0, [r3]
	sub sp, #4
	mov r8, r2
	bl Object_GetById
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	ldr r5, [r6, #68]
	mov r9, r3
	ldr r3, [r6, #8]
	ldr r2, [r6, #72]
	adds r3, r3, r5
	str r3, [r6, #8]
	ldr r3, [r6, #12]
	ldr r7, [r6, #76]
	adds r3, r3, r2
	str r3, [r6, #12]
	ldr r3, [r6, #16]
	adds r4, r0, #0
	adds r3, r3, r7
	adds r0, r5, #0
	movs r1, #18
	str r3, [r6, #16]
	str r4, [sp, #0]
	bl __divsi3
	subs r5, r5, r0
	str r5, [r6, #68]
	adds r3, r7, #0
	ldr r4, [sp, #0]
	cmp r7, #0
	bge .L_02009e7c
	adds r3, #15
.L_02009e7c:
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
	ldr r2, [r4, #12]
	ldr r3, [r4, #20]
	cmp r2, r3
	bne .L_02009f0e
	adds r0, r6, #0
	adds r1, r4, #0
	bl Func_02001d5c
	cmp r0, #0
	beq .L_02009f0e
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #18
	add r3, r10
	ldrb r2, [r3]
	cmp r2, #0
	bne .L_02009f0e
	ldr r1, [r6, #76]
	cmp r1, #0
	beq .L_02009ee2
	mov r3, r8
	adds r3, #104
	strh r2, [r3]
	mov r2, r8
	adds r2, #106
	cmp r1, #0
	ble .L_02009eda
	movs r3, #1
	b .L_02009ee0
.L_02009eda:
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
.L_02009ee0:
	strh r3, [r2]
.L_02009ee2:
	adds r2, r6, #0
	adds r2, #35
	movs r3, #2
	strb r3, [r2]
	movs r1, #0
	ldr r2, [r6, #76]
	ldr r3, [r6, #16]
	str r1, [r6, #76]
	subs r3, r3, r2
	str r3, [r6, #16]
	ldr r3, [r6, #72]
	asrs r2, r2, #2
	adds r3, r3, r2
	str r3, [r6, #72]
	mov r2, r8
	movs r3, #1
	strh r3, [r2, #4]
	ldrh r2, [r2, #10]
	movs r3, #170
	lsls r3, r3, #1
	add r3, r9
	strh r2, [r3]
.L_02009f0e:
	movs r3, #84
	mov r2, r8
	ldrh r0, [r2, r3]
	movs r7, #0
	cmp r0, #0
	beq .L_02009f6e
	mov r5, r8
	adds r5, #84
.L_02009f1e:
	bl Object_GetById
	adds r4, r0, #0
	ldr r2, [r4, #12]
	ldr r3, [r4, #20]
	cmp r2, r3
	bne .L_02009f60
	adds r0, r6, #0
	adds r1, r4, #0
	bl Func_02001dc0
	cmp r0, #0
	beq .L_02009f60
	ldrh r1, [r5, #2]
	cmp r1, #0
	bne .L_02009f5a
	adds r2, r6, #0
	adds r2, #35
	movs r3, #2
	strb r3, [r2]
	ldr r2, [r6, #76]
	ldr r3, [r6, #16]
	str r1, [r6, #76]
	subs r3, r3, r2
	str r3, [r6, #16]
	ldr r3, [r6, #72]
	asrs r2, r2, #2
	adds r3, r3, r2
	str r3, [r6, #72]
	b .L_02009f60
.L_02009f5a:
	movs r3, #1
	mov r2, r8
	strh r3, [r2, #6]
.L_02009f60:
	adds r7, #1
	adds r5, #4
	cmp r7, #3
	bgt .L_02009f6e
	ldrh r0, [r5]
	cmp r0, #0
	bne .L_02009f1e
.L_02009f6e:
	add sp, #4
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009f7c:
	.4byte gPartyState
.L_02009f80:
	.4byte gData_020023c4 + 0x188
	.section .text.x02009ffc,"ax",%progbits
	.balign 4
	.global Func_02001ffc
	.thumb_func
Func_02001ffc:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	ldr r3, .L_0200a154
	ldr r2, .L_0200a158
	mov r10, r3
	movs r3, #133
	lsls r3, r3, #2
	add r3, r10
	adds r6, r0, #0
	ldr r0, [r3]
	sub sp, #4
	mov r8, r2
	bl Object_GetById
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	ldr r5, [r6, #68]
	mov r9, r3
	ldr r3, [r6, #16]
	ldr r2, [r6, #72]
	adds r3, r3, r5
	str r3, [r6, #16]
	ldr r3, [r6, #12]
	ldr r7, [r6, #76]
	adds r3, r3, r2
	str r3, [r6, #12]
	ldr r3, [r6, #8]
	adds r4, r0, #0
	adds r3, r3, r7
	adds r0, r5, #0
	movs r1, #18
	str r3, [r6, #8]
	str r4, [sp, #0]
	bl __divsi3
	subs r5, r5, r0
	str r5, [r6, #68]
	adds r3, r7, #0
	ldr r4, [sp, #0]
	cmp r7, #0
	bge .L_0200a058
	adds r3, #15
.L_0200a058:
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
	ldr r2, [r4, #12]
	ldr r3, [r4, #20]
	cmp r2, r3
	bne .L_0200a0e8
	adds r0, r6, #0
	adds r1, r4, #0
	bl Func_02001d5c
	cmp r0, #0
	beq .L_0200a0e8
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #18
	add r3, r10
	ldrb r3, [r3]
	cmp r3, #0
	bne .L_0200a0e8
	ldr r2, [r6, #76]
	cmp r2, #0
	beq .L_0200a0bc
	mov r1, r8
	adds r1, #106
	strh r3, [r1]
	subs r1, #2
	cmp r2, #0
	ble .L_0200a0b4
	movs r3, #1
	b .L_0200a0ba
.L_0200a0b4:
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
.L_0200a0ba:
	strh r3, [r1]
.L_0200a0bc:
	adds r2, r6, #0
	adds r2, #35
	movs r3, #2
	strb r3, [r2]
	movs r1, #0
	ldr r3, [r6, #16]
	ldr r2, [r6, #68]
	str r1, [r6, #76]
	subs r3, r3, r2
	str r3, [r6, #16]
	ldr r3, [r6, #72]
	asrs r2, r2, #2
	adds r3, r3, r2
	str r3, [r6, #72]
	mov r2, r8
	movs r3, #1
	strh r3, [r2, #4]
	ldrh r2, [r2, #10]
	movs r3, #170
	lsls r3, r3, #1
	add r3, r9
	strh r2, [r3]
.L_0200a0e8:
	movs r3, #84
	mov r2, r8
	ldrh r0, [r2, r3]
	movs r7, #0
	cmp r0, #0
	beq .L_0200a148
	mov r5, r8
	adds r5, #84
.L_0200a0f8:
	bl Object_GetById
	adds r4, r0, #0
	ldr r2, [r4, #12]
	ldr r3, [r4, #20]
	cmp r2, r3
	bne .L_0200a13a
	adds r0, r6, #0
	adds r1, r4, #0
	bl Func_02001dc0
	cmp r0, #0
	beq .L_0200a13a
	ldrh r1, [r5, #2]
	cmp r1, #0
	bne .L_0200a134
	adds r2, r6, #0
	adds r2, #35
	movs r3, #2
	strb r3, [r2]
	ldr r2, [r6, #76]
	ldr r3, [r6, #16]
	str r1, [r6, #76]
	subs r3, r3, r2
	str r3, [r6, #16]
	ldr r3, [r6, #72]
	asrs r2, r2, #2
	adds r3, r3, r2
	str r3, [r6, #72]
	b .L_0200a13a
.L_0200a134:
	movs r3, #1
	mov r2, r8
	strh r3, [r2, #6]
.L_0200a13a:
	adds r7, #1
	adds r5, #4
	cmp r7, #3
	bgt .L_0200a148
	ldrh r0, [r5]
	cmp r0, #0
	bne .L_0200a0f8
.L_0200a148:
	add sp, #4
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_0200a154:
	.4byte gPartyState
.L_0200a158:
	.4byte gData_020023c4 + 0x188
	.section .text.x0200a15c,"ax",%progbits
	.global Func_0200215c
	.thumb_func
Func_0200215c:
	push {r5, r6, lr}
	adds r5, r0, #0
	adds r3, r5, #0
	movs r6, #0
	adds r3, #85
	strb r6, [r3]
	adds r3, #4
	strb r6, [r3]
	movs r3, #3
	ldr r0, [r5, #80]
	ands r1, r3
	ldrb r2, [r0, #9]
	movs r3, #13
	negs r3, r3
	ands r3, r2
	lsls r1, r1, #2
	orrs r3, r1
	strb r3, [r0, #9]
	movs r1, #0
	adds r0, r5, #0
	bl ObjectDispatch_SetSingleChildFlags
	ldr r1, .L_0200a19c
	adds r0, r5, #0
	bl Func_02005490
	adds r3, r5, #0
	adds r3, #100
	strh r6, [r3]
	str r6, [r5, #48]
	str r6, [r5, #52]
	pop {r5, r6, pc}
.L_0200a19c:
	.4byte gData_02005d8c
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
	ldr r0, .L_0200a5a8
	sub sp, #8
	mov r8, r0
	movs r0, #192
	lsls r0, r0, #18
	ldr r1, [r0, #32]
	mov r7, r8
	adds r2, r1, #0
	adds r2, #228
	ldr r3, [r2]
	ldr r2, [r2, #4]
	mov r9, r3
	ldr r3, .L_0200a5ac
	mov r4, r9
	ands r4, r3
	ands r2, r3
	ldr r3, [r1]
	mov r9, r4
	ldr r3, [r3, #4]
	adds r7, #20
	str r3, [sp, #4]
	mov r10, r2
	ldr r0, [r0, #108]
	str r0, [sp, #0]
	mov r0, r8
	movs r4, #6
	ldrsh r3, [r0, r4]
	cmp r3, #0
	beq .L_0200a486
	movs r1, #8
	ldrsh r3, [r0, r1]
	cmp r3, #0
	bne .L_0200a486
	ldr r3, [r0, #16]
	cmp r3, #0
	beq .L_0200a486
	mov lr, r3
	.2byte 0xf800
.L_0200a486:
	mov r2, r8
	ldrh r3, [r2, #6]
	mov r4, r8
	movs r2, #0
	mov r0, r8
	strh r3, [r4, #8]
	strh r2, [r0, #6]
	movs r1, #3
	mov r11, r1
.L_0200a498:
	movs r2, #0
	ldrsh r3, [r7, r2]
	cmp r3, #0
	beq .L_0200a588
	ldr r5, [r7, #8]
	cmp r5, #0
	beq .L_0200a588
	movs r0, #131
	lsls r0, r0, #1
	bl GameFlag_Test
	ldr r4, [sp, #0]
	movs r1, #179
	lsls r1, r1, #1
	adds r3, r4, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	beq .L_0200a4c2
	movs r3, #1
	orrs r0, r3
.L_0200a4c2:
	adds r6, r5, #0
	adds r6, #91
	strb r0, [r6]
	mov r0, r8
	movs r4, #14
	ldrsh r3, [r0, r4]
	cmp r3, #0
	beq .L_0200a4dc
	movs r0, #131
	lsls r0, r0, #1
	bl GameFlag_Test
	strb r0, [r6]
.L_0200a4dc:
	ldr r2, [r5, #8]
	mov r1, r9
	ldr r3, [r5, #16]
	subs r2, r2, r1
	ldr r1, [r5, #12]
	mov r4, r10
	subs r3, r3, r4
	subs r1, r3, r1
	movs r0, #6
	ldrsh r4, [r7, r0]
	asrs r3, r1, #16
	adds r1, r3, #0
	asrs r2, r2, #16
	subs r1, #8
	cmp r4, #0
	bne .L_0200a512
	adds r3, r2, #7
	movs r2, #167
	lsls r2, r2, #1
	cmp r3, r2
	bhi .L_0200a588
	movs r3, #48
	negs r3, r3
	cmp r1, r3
	ble .L_0200a588
	cmp r1, #239
	bgt .L_0200a588
.L_0200a512:
	movs r0, #2
	ldrsh r3, [r7, r0]
	ldrh r1, [r7, #2]
	cmp r3, #0
	bgt .L_0200a584
	ldrh r3, [r7, #4]
	movs r1, #240
	ands r1, r3
	cmp r1, #32
	beq .L_0200a558
	cmp r1, #32
	bgt .L_0200a534
	cmp r1, #0
	beq .L_0200a574
	cmp r1, #16
	beq .L_0200a566
	b .L_0200a580
.L_0200a534:
	cmp r1, #48
	beq .L_0200a54a
	cmp r1, #128
	bne .L_0200a580
	ldr r0, [r7, #8]
	ldr r1, [r7, #12]
	adds r0, #8
	adds r2, r4, #0
	bl Func_020023ac
	b .L_0200a580
.L_0200a54a:
	ldr r0, [r7, #8]
	ldr r1, [r7, #12]
	adds r0, #8
	adds r2, r4, #0
	bl Func_020021a0
	b .L_0200a580
.L_0200a558:
	ldr r0, [r7, #8]
	ldr r1, [r7, #12]
	adds r0, #8
	adds r2, r4, #0
	bl Func_02002230
	b .L_0200a580
.L_0200a566:
	ldr r0, [r7, #8]
	ldr r1, [r7, #12]
	adds r0, #8
	adds r2, r4, #0
	bl Func_020021e8
	b .L_0200a580
.L_0200a574:
	ldr r0, [r7, #8]
	ldr r1, [r7, #12]
	adds r0, #8
	adds r2, r4, #0
	bl Func_02001fb4
.L_0200a580:
	movs r3, #8
	b .L_0200a586
.L_0200a584:
	subs r3, r1, #1
.L_0200a586:
	strh r3, [r7, #2]
.L_0200a588:
	movs r1, #1
	negs r1, r1
	add r11, r1
	mov r2, r11
	adds r7, #16
	cmp r2, #0
	blt .L_0200a598
	b .L_0200a498
.L_0200a598:
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200a5a8:
	.4byte gData_020023c4 + 0x188
.L_0200a5ac:
	.4byte 0xffff0000
	.section .text.x0200a624,"ax",%progbits
	.balign 4
	.global Func_02002624
	.thumb_func
Func_02002624:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	mov r8, r1
	ldr r1, .L_0200a750
	sub sp, #16
	adds r6, r0, #0
	movs r0, #10
	str r1, [sp, #4]
	adds r0, #255
	adds r1, #20
	str r2, [sp, #12]
	str r3, [sp, #8]
	mov r9, r1
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a708
	ldrh r3, [r6]
	movs r2, #0
	mov r11, r2
	mov r10, r3
	adds r6, #2
	cmp r3, #0
	ble .L_0200a6c6
.L_0200a65e:
	ldrh r7, [r6]
	movs r1, #15
	ands r1, r7
	movs r3, #240
	mov r0, r10
	str r1, [sp, #0]
	ands r7, r3
	bl Object_GetById
	adds r5, r0, #0
	adds r6, #2
	cmp r5, #0
	beq .L_0200a6b2
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	adds r3, r5, #0
	movs r2, #128
	adds r3, #98
	movs r1, #1
	ands r2, r7
	strb r1, [r3]
	cmp r2, #0
	bne .L_0200a692
	subs r3, #9
	strb r2, [r3]
.L_0200a692:
	mov r2, r9
	mov r3, r9
	strh r1, [r2]
	mov r0, r10
	strh r7, [r3, #4]
	bl Object_GetById
	mov r1, r9
	str r0, [r1, #8]
	ldr r2, [sp, #0]
	lsls r3, r2, #16
	str r3, [r1, #12]
	mov r3, r11
	strh r3, [r1, #2]
	movs r2, #16
	add r9, r2
.L_0200a6b2:
	movs r3, #1
	add r11, r3
	mov r1, r11
	cmp r1, #3
	bgt .L_0200a6c6
	ldrh r2, [r6]
	adds r6, #2
	mov r10, r2
	cmp r2, #0
	bgt .L_0200a65e
.L_0200a6c6:
	mov r3, r8
	cmp r3, #0
	beq .L_0200a708
	movs r1, #0
	ldrh r2, [r3]
	mov r11, r1
	ldr r1, [sp, #4]
	movs r3, #2
	add r8, r3
	movs r3, #84
	strh r2, [r1, r3]
	cmp r2, #0
	ble .L_0200a708
	adds r2, r1, #0
	adds r2, #84
.L_0200a6e4:
	mov r1, r8
	ldrh r3, [r1]
	movs r1, #1
	strh r3, [r2, #2]
	add r11, r1
	movs r3, #2
	add r8, r3
	mov r3, r11
	adds r2, #4
	cmp r3, #3
	bgt .L_0200a708
	mov r1, r8
	ldrh r3, [r1]
	movs r1, #2
	add r8, r1
	strh r3, [r2]
	cmp r3, #0
	bgt .L_0200a6e4
.L_0200a708:
	ldr r2, [sp, #12]
	ldr r3, [sp, #4]
	add r1, sp, #8
	str r2, [r3, #16]
	ldrh r1, [r1]
	ldr r2, [sp, #4]
	strh r1, [r2, #10]
	movs r1, #128
	lsls r1, r1, #19
	adds r1, #80
	ldrh r3, [r1]
	cmp r3, #0
	bne .L_0200a738
	movs r3, #192
	movs r2, #128
	lsls r3, r3, #4
	lsls r2, r2, #19
	adds r3, #8
	adds r2, #82
	strh r3, [r2]
	movs r3, #252
	lsls r3, r3, #6
	adds r3, #16
	strh r3, [r1]
.L_0200a738:
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_0200a754
	bl Scheduler_AddOrUpdateCallback
	add sp, #16
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0200a750:
	.4byte gData_020023c4 + 0x188
.L_0200a754:
	.4byte Func_0200242c
	.section .text.x0200a800,"ax",%progbits
	.balign 4
	.global Func_02002800
	.thumb_func
Func_02002800:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	sub sp, #16
	ldr r6, [r3, #108]
	bl Func_02005690
	mov r8, r0
	bl Object_GetById
	bl Party_CountActiveOwners
	movs r5, #0
	adds r7, r0, #0
	cmp r5, r7
	bge .L_0200a842
.L_0200a826:
	ldr r2, .L_0200a8e8
	movs r1, #134
	lsls r1, r1, #2
	adds r3, r5, r1
	ldrb r0, [r2, r3]
	bl Unit_Get
	ldrh r3, [r0, #56]
	lsls r2, r5, #1
	mov r1, sp
	adds r5, #1
	strh r3, [r1, r2]
	cmp r5, r7
	blt .L_0200a826
.L_0200a842:
	movs r0, #10
	negs r0, r0
	movs r1, #0
	bl Func_02005650
	movs r2, #182
	lsls r2, r2, #1
	movs r4, #183
	adds r3, r6, r2
	lsls r4, r4, #1
	movs r2, #0
	strh r2, [r3]
	movs r1, #129
	adds r3, r6, r4
	strh r2, [r3]
	mov r0, r8
	lsls r1, r1, #1
	movs r5, #0
	bl Func_020055c0
	cmp r5, r7
	bge .L_0200a8dc
.L_0200a86e:
	ldr r1, .L_0200a8e8
	movs r2, #134
	lsls r2, r2, #2
	adds r2, r2, r5
	ldrb r0, [r1, r2]
	mov r10, r1
	mov r8, r2
	bl Unit_Get
	movs r4, #56
	ldrsh r3, [r0, r4]
	cmp r3, #0
	ble .L_0200a896
	movs r1, #183
	lsls r1, r1, #1
	adds r2, r6, r1
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_0200a8d6
.L_0200a896:
	mov r3, sp
	lsls r2, r5, #1
	ldrsh r3, [r3, r2]
	cmp r3, #0
	beq .L_0200a8d6
	movs r2, #182
	lsls r2, r2, #1
	adds r1, r6, r2
	ldrh r3, [r1]
	movs r4, #184
	adds r2, r3, #1
	lsls r3, r3, #16
	lsls r4, r4, #1
	asrs r3, r3, #15
	strh r2, [r1]
	adds r3, r3, r4
	mov r1, r10
	mov r4, r8
	ldrb r2, [r1, r4]
	movs r1, #181
	strh r2, [r6, r3]
	movs r3, #255
	lsls r1, r1, #1
	lsls r3, r3, #8
	adds r2, r6, r1
	adds r3, #255
	strh r3, [r2]
	movs r3, #50
	adds r3, #255
	adds r2, r0, r3
	movs r3, #0
	strb r3, [r2]
.L_0200a8d6:
	adds r5, #1
	cmp r5, r7
	blt .L_0200a86e
.L_0200a8dc:
	add sp, #16
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200a8e8:
	.4byte gPartyState
	.section .text.x0200a97c,"ax",%progbits
	.ifdef TLA_EDITION_JA
	.balign 4
	.global Func_0200297c
	.thumb_func
Func_0200297c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r5, .L_ja_0200a9bc
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #18
	adds r3, r5, r0
	ldrb r3, [r3]
	sub sp, #12
	cmp r3, #0
	beq .L_ja_0200a84c
	b .L_ja_0200a9ac
.L_ja_0200a84c:
	movs r0, #10
	movs r1, #0
	negs r0, r0
	bl Func_02002800
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r5, r1
	ldr r0, [r3]
	bl Object_GetById
	ldr r2, .L_ja_0200a9c0
	adds r5, r0, #0
	mov r10, r2
	bl Func_02005560
	movs r0, #0
	bl Func_02005648
	ldr r3, .L_ja_0200a9c4
	adds r0, r5, #0
	str r3, [r5, #108]
	movs r3, #128
	lsls r3, r3, #7
	strh r3, [r5, #6]
	movs r1, #49
	bl Func_02005480
.L_ja_0200a884:
	adds r3, r5, #0
	adds r3, #34
	ldr r1, [r5, #8]
	ldrb r0, [r3]
	ldr r2, [r5, #16]
	bl Func_020054d8
	mov r6, r10
	mov r7, r10
	adds r6, #104
	adds r7, #106
	mov r1, sp
	cmp r0, #7
	bne .L_ja_0200a8fc
	movs r0, #0
	ldrsh r3, [r6, r0]
	movs r2, #0
	lsls r3, r3, #17
	str r3, [r5, #36]
	mov r8, r2
	movs r1, #0
	ldrsh r3, [r7, r1]
	lsls r3, r3, #17
	str r3, [r5, #44]
	movs r3, #128
	lsls r3, r3, #5
	str r3, [r5, #52]
.L_ja_0200a8ba:
	movs r3, #0
	ldrsh r2, [r6, r3]
	ldr r3, [r5, #8]
	lsls r2, r2, #19
	mov r1, sp
	adds r3, r3, r2
	str r3, [r1]
	ldr r3, [r5, #12]
	str r3, [r1, #4]
	movs r0, #0
	ldrsh r2, [r7, r0]
	ldr r3, [r5, #16]
	lsls r2, r2, #19
	adds r3, r3, r2
	str r3, [r1, #8]
	adds r0, r5, #0
	bl Func_020054f8
	cmp r0, #0
	beq .L_ja_0200a8ea
	movs r3, #0
	str r3, [r5, #36]
	str r3, [r5, #44]
	b .L_ja_0200a9a0
.L_ja_0200a8ea:
	movs r0, #1
	bl WaitFrames
	movs r1, #1
	add r8, r1
	mov r2, r8
	cmp r2, #9
	ble .L_ja_0200a8ba
	b .L_ja_0200a9a0
.L_ja_0200a8fc:
	movs r3, #0
	ldrsh r2, [r6, r3]
	ldr r3, [r5, #8]
	lsls r2, r2, #19
	adds r3, r3, r2
	str r3, [r1]
	ldr r3, [r5, #12]
	str r3, [r1, #4]
	movs r0, #0
	ldrsh r2, [r7, r0]
	ldr r3, [r5, #16]
	lsls r2, r2, #19
	adds r3, r3, r2
	str r3, [r1, #8]
	adds r0, r5, #0
	bl Func_020054f8
	cmp r0, #0
	bgt .L_ja_0200a9a0
	cmp r0, #0
	bge .L_ja_0200a942
	movs r3, #128
	lsls r3, r3, #7
	strh r3, [r5, #6]
	ldr r3, .L_ja_0200a9bc
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	movs r2, #1
	ldr r0, [r3]
	movs r1, #6
	negs r2, r2
	bl Func_020055a8
	b .L_ja_0200a9a0
.L_ja_0200a942:
	movs r2, #0
	ldrsh r3, [r6, r2]
	ldr r2, [r5, #8]
	lsls r3, r3, #17
	adds r1, r2, r3
	str r1, [r5, #8]
	ldr r2, [r5, #16]
	movs r0, #0
	ldrsh r3, [r7, r0]
	ldr r7, .L_ja_0200a9c8
	lsls r3, r3, #17
	ldr r0, .L_ja_0200a9cc
	adds r6, r2, r3
	adds r3, r1, #0
	ands r3, r7
	movs r4, #128
	adds r2, r3, r0
	lsls r4, r4, #9
	str r6, [r5, #16]
	cmp r2, r4
	ble .L_ja_0200a96e
	adds r2, r4, #0
.L_ja_0200a96e:
	ldr r0, .L_ja_0200a9d0
	cmp r2, r0
	bge .L_ja_0200a976
	adds r2, r0, #0
.L_ja_0200a976:
	subs r3, r1, r2
	ldr r1, .L_ja_0200a9cc
	str r3, [r5, #8]
	adds r3, r6, #0
	ands r3, r7
	adds r2, r3, r1
	cmp r2, r4
	ble .L_ja_0200a988
	adds r2, r4, #0
.L_ja_0200a988:
	cmp r2, r0
	bge .L_ja_0200a98e
	adds r2, r0, #0
.L_ja_0200a98e:
	subs r3, r6, r2
	str r3, [r5, #16]
	mov r2, r10
	movs r3, #0
	strh r3, [r2, #4]
	movs r0, #1
	bl WaitFrames
	b .L_ja_0200a884
.L_ja_0200a9a0:
	movs r3, #0
	str r3, [r5, #108]
	adds r0, r5, #0
	movs r1, #0
	bl Animation_ApplyChildValues
.L_ja_0200a9ac:
	mov r0, r10
	movs r3, #0
	strh r3, [r0, #4]
	add sp, #12
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_ja_0200a9bc:
	.4byte gPartyState
.L_ja_0200a9c0:
	.4byte gData_020023c4 + 0x188
.L_ja_0200a9c4:
	.4byte Func_02002954
.L_ja_0200a9c8:
	.4byte 0x000fffff
.L_ja_0200a9cc:
	.4byte 0xfff80000
.L_ja_0200a9d0:
	.4byte 0xffff0000
	.else
	.balign 4
	.global Func_0200297c
	.thumb_func
Func_0200297c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r5, .L_0200ab70
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #18
	adds r3, r5, r0
	ldrb r3, [r3]
	sub sp, #16
	cmp r3, #0
	beq .L_0200a99e
	b .L_0200ab5c
.L_0200a99e:
	movs r0, #10
	movs r1, #0
	negs r0, r0
	bl Func_02002800
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r5, r1
	ldr r0, [r3]
	bl Object_GetById
	ldr r2, .L_0200ab74
	adds r6, r0, #0
	str r2, [sp, #0]
	bl Func_02005560
	movs r0, #0
	bl Func_02005648
	ldr r3, .L_0200ab78
	adds r0, r6, #0
	str r3, [r6, #108]
	movs r3, #128
	lsls r3, r3, #7
	strh r3, [r6, #6]
	movs r1, #49
	bl Func_02005480
.L_0200a9d6:
	adds r3, r6, #0
	adds r3, #34
	ldrb r0, [r3]
	ldr r1, [r6, #8]
	ldr r2, [r6, #16]
	bl Func_020054d8
	ldr r3, [sp, #0]
	ldr r1, [sp, #0]
	adds r3, #104
	adds r1, #106
	mov r9, r1
	mov r10, r3
	add r1, sp, #4
	cmp r0, #7
	bne .L_0200aa52
	movs r2, #0
	ldrsh r3, [r3, r2]
	mov r1, r9
	lsls r3, r3, #17
	str r3, [r6, #36]
	movs r5, #0
	movs r0, #0
	ldrsh r3, [r1, r0]
	lsls r3, r3, #17
	str r3, [r6, #44]
	movs r3, #128
	lsls r3, r3, #5
	str r3, [r6, #52]
.L_0200aa10:
	mov r0, r10
	movs r3, #0
	ldrsh r2, [r0, r3]
	ldr r3, [r6, #8]
	lsls r2, r2, #19
	add r1, sp, #4
	adds r3, r3, r2
	str r3, [r1]
	mov r0, r9
	ldr r3, [r6, #12]
	str r3, [r1, #4]
	movs r3, #0
	ldrsh r2, [r0, r3]
	ldr r3, [r6, #16]
	lsls r2, r2, #19
	adds r3, r3, r2
	str r3, [r1, #8]
	adds r0, r6, #0
	bl Func_020054f8
	cmp r0, #0
	beq .L_0200aa44
	movs r3, #0
	str r3, [r6, #36]
	str r3, [r6, #44]
	b .L_0200ab50
.L_0200aa44:
	movs r0, #1
	adds r5, #1
	bl WaitFrames
	cmp r5, #9
	ble .L_0200aa10
	b .L_0200ab50
.L_0200aa52:
	mov r0, r10
	movs r3, #0
	ldrsh r2, [r0, r3]
	ldr r3, [r6, #8]
	lsls r2, r2, #19
	adds r3, r3, r2
	str r3, [r1]
	mov r0, r9
	ldr r3, [r6, #12]
	str r3, [r1, #4]
	movs r3, #0
	ldrsh r2, [r0, r3]
	ldr r3, [r6, #16]
	lsls r2, r2, #19
	adds r3, r3, r2
	str r3, [r1, #8]
	adds r0, r6, #0
	bl Func_020054f8
	cmp r0, #0
	bgt .L_0200ab50
	cmp r0, #0
	bge .L_0200aa9c
	movs r3, #128
	lsls r3, r3, #7
	strh r3, [r6, #6]
	ldr r3, .L_0200ab70
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	movs r2, #1
	ldr r0, [r3]
	movs r1, #6
	negs r2, r2
	bl Func_020055a8
	b .L_0200ab50
.L_0200aa9c:
	ldrh r3, [r6, #32]
	movs r2, #0
	subs r3, #2
	mov r11, r3
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #20]
	mov r8, r2
	adds r7, r5, #0
	adds r7, #89
.L_0200aab0:
	ldr r3, [r5]
	cmp r3, #0
	beq .L_0200aae0
	ldrb r2, [r7]
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	beq .L_0200aae0
	cmp r5, r6
	beq .L_0200aae0
	ldrh r3, [r5, #32]
	adds r0, r5, #0
	adds r0, #8
	subs r3, #2
	mov r1, r11
	add r2, sp, #4
	bl Func_02005538
	cmp r0, #0
	blt .L_0200aae0
	movs r0, #1
	bl WaitFrames
	b .L_0200ab50
.L_0200aae0:
	movs r3, #1
	add r8, r3
	mov r0, r8
	adds r7, #128
	adds r5, #128
	cmp r0, #63
	ble .L_0200aab0
	mov r2, r10
	movs r1, #0
	ldrsh r3, [r2, r1]
	ldr r2, [r6, #8]
	lsls r3, r3, #17
	adds r1, r2, r3
	str r1, [r6, #8]
	mov r2, r9
	movs r0, #0
	ldrsh r3, [r2, r0]
	ldr r2, [r6, #16]
	ldr r7, .L_0200ab7c
	lsls r3, r3, #17
	ldr r0, .L_0200ab80
	adds r5, r2, r3
	adds r3, r1, #0
	ands r3, r7
	movs r4, #128
	adds r2, r3, r0
	lsls r4, r4, #9
	str r5, [r6, #16]
	cmp r2, r4
	ble .L_0200ab1e
	adds r2, r4, #0
.L_0200ab1e:
	ldr r0, .L_0200ab84
	cmp r2, r0
	bge .L_0200ab26
	adds r2, r0, #0
.L_0200ab26:
	subs r3, r1, r2
	ldr r1, .L_0200ab80
	str r3, [r6, #8]
	adds r3, r5, #0
	ands r3, r7
	adds r2, r3, r1
	cmp r2, r4
	ble .L_0200ab38
	adds r2, r4, #0
.L_0200ab38:
	cmp r2, r0
	bge .L_0200ab3e
	adds r2, r0, #0
.L_0200ab3e:
	subs r3, r5, r2
	str r3, [r6, #16]
	ldr r2, [sp, #0]
	movs r3, #0
	strh r3, [r2, #4]
	movs r0, #1
	bl WaitFrames
	b .L_0200a9d6
.L_0200ab50:
	movs r3, #0
	str r3, [r6, #108]
	adds r0, r6, #0
	movs r1, #0
	bl Animation_ApplyChildValues
.L_0200ab5c:
	ldr r0, [sp, #0]
	movs r3, #0
	strh r3, [r0, #4]
	add sp, #16
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0200ab70:
	.4byte gPartyState
.L_0200ab74:
	.4byte gData_020023c4 + 0x188
.L_0200ab78:
	.4byte Func_02002954
.L_0200ab7c:
	.4byte 0x000fffff
.L_0200ab80:
	.4byte 0xfff80000
.L_0200ab84:
	.4byte 0xffff0000
	.endif
	.section .text.x0200aba0,"ax",%progbits
	.balign 4
	.global Func_02002ba0
	.thumb_func
Func_02002ba0:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #108]
	movs r1, #217
	lsls r1, r1, #1
	adds r6, r5, r1
	ldrh r3, [r6]
	sub sp, #12
	cmp r3, #0
	bne .L_0200abcc
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r5, r2
	adds r1, #2
	ldr r0, [r3]
	adds r3, r5, r1
	ldr r1, [r3]
	bl Func_02005610
	movs r3, #1
	strh r3, [r6]
.L_0200abcc:
	movs r0, #131
	lsls r0, r0, #1
	bl GameFlag_ClearBit
	movs r2, #179
	lsls r2, r2, #1
	movs r1, #173
	adds r3, r5, r2
	lsls r1, r1, #1
	movs r2, #0
	strh r2, [r3]
	adds r3, r5, r1
	adds r1, #4
	strh r2, [r3]
	adds r3, r5, r1
	subs r1, #2
	strh r2, [r3]
	adds r3, r5, r1
	adds r1, #8
	strh r2, [r3]
	movs r0, #10
	adds r3, r5, r1
	strh r2, [r3]
	movs r1, #0
	negs r0, r0
	bl Func_02002800
	movs r0, #224
	movs r1, #224
	lsls r1, r1, #8
	lsls r0, r0, #11
	bl Func_020055d0
	ldr r3, .L_0200acd0
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	adds r6, r0, #0
	movs r0, #131
	lsls r0, r0, #1
	ldr r7, .L_0200acd4
	bl GameFlag_SetBit
	bl Func_02005560
	movs r0, #0
	bl Func_02005648
	movs r0, #131
	lsls r0, r0, #1
	bl GameFlag_ClearBit
	ldr r3, .L_0200acd8
	movs r1, #49
	str r3, [r6, #108]
	movs r3, #128
	lsls r3, r3, #7
	strh r3, [r6, #6]
	adds r0, r6, #0
	bl Func_02005480
	ldr r3, [r7, #108]
	adds r3, #100
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	bne .L_0200acb8
.L_0200ac58:
	ldr r1, [r6, #8]
	ldr r2, [r6, #16]
	movs r0, #0
	bl Func_020054d8
	ldr r1, [r7, #108]
	cmp r0, #7
	beq .L_0200ac78
	ldr r2, [r1, #112]
	ldr r3, [r6, #8]
	adds r3, r3, r2
	str r3, [r6, #8]
	ldr r3, [r6, #16]
	ldr r2, [r1, #120]
	adds r3, r3, r2
	b .L_0200aca0
.L_0200ac78:
	ldr r3, [r1, #112]
	cmp r3, #0
	beq .L_0200ac8c
	ldr r3, [r6, #8]
	ldr r2, .L_0200acdc
	ands r3, r2
	movs r2, #128
	lsls r2, r2, #12
	adds r3, r3, r2
	str r3, [r6, #8]
.L_0200ac8c:
	ldr r3, [r7, #108]
	ldr r3, [r3, #120]
	cmp r3, #0
	beq .L_0200aca2
	ldr r3, [r6, #16]
	ldr r2, .L_0200acdc
	movs r1, #128
	ands r3, r2
	lsls r1, r1, #12
	adds r3, r3, r1
.L_0200aca0:
	str r3, [r6, #16]
.L_0200aca2:
	movs r3, #0
	strh r3, [r7, #4]
	movs r0, #1
	bl WaitFrames
	ldr r3, [r7, #108]
	adds r3, #100
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	beq .L_0200ac58
.L_0200acb8:
	movs r5, #0
	movs r0, #30
	bl FieldEvent_WaitFrames
	adds r0, r6, #0
	str r5, [r6, #108]
	movs r1, #0
	bl Animation_ApplyChildValues
	strh r5, [r7, #4]
	add sp, #12
	pop {r5, r6, r7, pc}
.L_0200acd0:
	.4byte gPartyState
.L_0200acd4:
	.4byte gData_020023c4 + 0x188
.L_0200acd8:
	.4byte Func_02002954
.L_0200acdc:
	.4byte 0xfff00000
	.section .text.x0200ad08,"ax",%progbits
	.balign 4
	.global Func_02002d08
	.thumb_func
Func_02002d08:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r1, .L_0200ae70
	sub sp, #4
	ldr r3, [r1, #112]
	mov r11, r0
	cmp r3, #0
	bne .L_0200ad24
	b .L_0200ae7c
.L_0200ad24:
	movs r2, #0
	str r2, [sp, #0]
.L_0200ad28:
	bl Random16Far
	adds r5, r0, #0
	bl Random16Far
	mov r3, r11
	ldr r3, [r3, #8]
	lsls r5, r5, #4
	mov r8, r3
	add r8, r5
	lsls r0, r0, #4
	mov r1, r8
	subs r1, r1, r0
	mov r8, r1
	bl Random16Far
	adds r6, r0, #0
	bl Random16Far
	adds r5, r0, #0
	bl Random16Far
	mov r2, r11
	ldr r3, [r2, #16]
	ldr r2, [r2, #12]
	lsls r5, r5, #4
	lsls r6, r6, #3
	movs r1, #128
	lsls r0, r0, #4
	adds r6, r6, r2
	lsls r1, r1, #11
	adds r3, r3, r5
	subs r3, r3, r0
	adds r6, r6, r1
	movs r0, #234
	adds r0, #255
	mov r1, r8
	adds r2, r6, #0
	bl Func_02005498
	adds r7, r0, #0
	cmp r7, #0
	beq .L_0200ae5c
	bl Random16Far
	mov r10, r0
	bl Random16Far
	adds r6, r0, #0
	bl Random16Far
	adds r5, r0, #0
	bl Random16Far
	movs r2, #128
	lsls r2, r2, #8
	adds r5, r5, r0
	ldr r1, .L_0200ae74
	adds r0, r7, #0
	mov r9, r2
	bl Func_02005490
	adds r0, r7, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	mov r1, r10
	movs r2, #128
	lsls r2, r2, #10
	lsls r3, r1, #2
	adds r3, r3, r2
	str r3, [r7, #40]
	mov r0, r10
	bl Math_Cosine
	ldr r3, .L_0200ae78
	lsls r6, r6, #3
	mov r8, r3
	adds r1, r0, #0
	adds r0, r6, #0
	mov lr, r8
	.2byte 0xf800
	str r0, [r7, #44]
	mov r0, r10
	bl Math_Sine
	adds r1, r0, #0
	adds r0, r6, #0
	mov lr, r8
	.2byte 0xf800
	ldr r2, .L_0200ae70
	movs r3, #168
	lsls r3, r3, #7
	adds r3, #122
	str r3, [r7, #72]
	lsrs r5, r5, #2
	ldr r3, [r2, #112]
	add r5, r9
	movs r6, #0
	mov r1, r9
	str r5, [r7, #24]
	str r5, [r7, #28]
	str r1, [r7, #68]
	ldr r5, [r7, #80]
	str r0, [r7, #36]
	str r6, [r7, #52]
	ldr r3, [r3, #80]
	ldrb r0, [r5, #16]
	mov r8, r3
	bl Resource_ResetEntry
	ldrb r3, [r5, #17]
	ldr r1, .L_0200ae70
	movs r2, #1
	orrs r3, r2
	strb r3, [r5, #17]
	ldrh r3, [r1, #12]
	ldr r0, [r5, #40]
	strb r3, [r5, #16]
	bl ResourceMetadata_ClearRecord
	str r6, [r5, #40]
	strb r6, [r5, #27]
	mov r2, r8
	ldrb r3, [r2, #20]
	ldrb r0, [r5, #5]
	strb r3, [r5, #20]
	ldrb r3, [r2, #21]
	strb r3, [r5, #21]
	ldrb r1, [r2, #5]
	movs r2, #63
	adds r3, r2, #0
	lsrs r1, r1, #6
	lsls r1, r1, #6
	ands r3, r0
	orrs r3, r1
	strb r3, [r5, #5]
	mov r1, r8
	ldrb r3, [r1, #7]
	ldrb r1, [r5, #7]
	lsrs r3, r3, #6
	lsls r3, r3, #6
	ands r2, r1
	orrs r2, r3
	strb r2, [r5, #7]
	mov r3, r8
	ldrh r2, [r3, #8]
	ldr r1, .L_0200ae6c
	ldrh r3, [r5, #8]
	lsls r2, r2, #22
	lsrs r2, r2, #22
	ands r3, r1
	orrs r3, r2
	strh r3, [r5, #8]
.L_0200ae5c:
	ldr r1, [sp, #0]
	subs r1, #1
	str r1, [sp, #0]
	cmp r1, #0
	blt .L_0200ae68
	b .L_0200ad28
.L_0200ae68:
	b .L_0200ae7c
	.2byte 0x0000
.L_0200ae6c:
	.4byte 0xfffffc00
.L_0200ae70:
	.4byte gData_020023c4 + 0x188
.L_0200ae74:
	.4byte gData_02005dbc
.L_0200ae78:
	.4byte IwramMulQ16
.L_0200ae7c:
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x0200ae8c,"ax",%progbits
	.global Func_02002e8c
	.thumb_func
Func_02002e8c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #16
	str r0, [sp, #12]
	str r1, [sp, #8]
	str r2, [sp, #4]
	str r3, [sp, #0]
	movs r3, #1
	mov r11, r3
.L_0200aea8:
	movs r0, #70
	adds r0, #255
	ldr r1, [sp, #12]
	ldr r2, [sp, #8]
	ldr r3, [sp, #4]
	bl Func_02005498
	adds r7, r0, #0
	cmp r7, #0
	beq .L_0200af4a
	bl Random16Far
	adds r5, r0, #0
	bl Random16Far
	ldr r3, [sp, #0]
	lsrs r5, r5, #4
	adds r5, r3, r5
	lsrs r0, r0, #4
	movs r3, #128
	subs r5, r5, r0
	lsls r3, r3, #7
	mov r10, r3
	adds r3, r5, #0
	add r3, r10
	mov r9, r3
	bl Random16Far
	mov r8, r0
	mov r3, r8
	lsls r3, r3, #3
	mov r8, r3
	bl Random16Far
	ldr r1, .L_0200af64
	adds r6, r0, #0
	adds r0, r7, #0
	bl Func_02005490
	movs r1, #0
	adds r0, r7, #0
	bl ObjectDispatch_SetSingleChildFlags
	movs r3, #160
	lsls r3, r3, #9
	adds r5, r5, r3
	str r5, [r7, #40]
	mov r0, r9
	bl Math_Cosine
	ldr r5, .L_0200af68
	adds r1, r0, #0
	mov r0, r8
	mov lr, r5
	.2byte 0xf800
	str r0, [r7, #44]
	mov r0, r9
	bl Math_Sine
	adds r1, r0, #0
	mov r0, r8
	mov lr, r5
	.2byte 0xf800
	movs r3, #0
	str r3, [r7, #52]
	movs r3, #168
	lsls r3, r3, #7
	adds r3, #122
	lsrs r6, r6, #1
	str r3, [r7, #72]
	movs r3, #128
	add r6, r10
	lsls r3, r3, #8
	str r0, [r7, #36]
	str r6, [r7, #24]
	str r6, [r7, #28]
	str r3, [r7, #68]
	adds r0, r7, #0
	movs r1, #1
	bl Animation_ApplyChildValues
.L_0200af4a:
	movs r3, #1
	negs r3, r3
	add r11, r3
	mov r3, r11
	cmp r3, #0
	bge .L_0200aea8
	add sp, #16
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0200af64:
	.4byte gData_02005e00
.L_0200af68:
	.4byte IwramMulQ16
	.section .text.x0200af80,"ax",%progbits
	.balign 4
	.global Func_02002f80
	.thumb_func
Func_02002f80:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	adds r5, r0, #0
	movs r3, #192
	movs r0, #100
	lsls r3, r3, #18
	adds r0, r0, r5
	ldr r6, [r3, #108]
	movs r1, #0
	ldrsh r3, [r0, r1]
	sub sp, #56
	mov r8, r0
	cmp r3, #0
	beq .L_0200afa8
	b .L_0200b234
.L_0200afa8:
	movs r0, #131
	lsls r0, r0, #1
	bl GameFlag_Test
	movs r2, #179
	lsls r2, r2, #1
	adds r3, r6, r2
	movs r4, #0
	ldrsh r3, [r3, r4]
	cmp r3, #0
	beq .L_0200afc2
	movs r3, #1
	orrs r0, r3
.L_0200afc2:
	adds r3, r5, #0
	adds r3, #91
	strb r0, [r3]
	add r7, sp, #44
	ldr r3, [r5, #8]
	movs r0, #128
	str r3, [r7]
	lsls r0, r0, #12
	ldr r3, [r5, #12]
	adds r2, r7, #0
	str r3, [r7, #4]
	ldr r3, [r5, #16]
	str r3, [r7, #8]
	ldrh r1, [r5, #6]
	bl Vector_AddPolarOffsetFar
	ldr r1, [r7]
	ldr r2, [r7, #8]
	movs r0, #0
	bl Func_020054d8
	cmp r0, #7
	bne .L_0200b038
	movs r3, #128
	lsls r3, r3, #24
	str r3, [r5, #64]
	str r3, [r5, #60]
	str r3, [r5, #56]
	movs r3, #128
	lsls r3, r3, #7
	str r3, [r5, #52]
	mov r0, r8
	movs r3, #1
	strh r3, [r0]
	movs r0, #145
	bl Func_020056c0
	movs r0, #160
	lsls r0, r0, #11
	movs r2, #128
	adds r1, r0, #0
	lsls r2, r2, #9
	bl Func_02005510
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	negs r0, r0
	negs r1, r1
	adds r2, #102
	bl Func_02005510
	ldr r3, [r5, #104]
	cmp r3, #0
	beq .L_0200b038
	adds r0, r5, #0
	mov lr, r3
	.2byte 0xf800
.L_0200b038:
	movs r3, #192
	lsls r3, r3, #18
	ldr r0, [r3, #32]
	ldr r3, [r5, #8]
	ldr r1, [r5, #16]
	asrs r3, r3, #20
	str r3, [sp, #32]
	movs r3, #184
	lsls r3, r3, #1
	ldr r4, [sp, #32]
	asrs r1, r1, #20
	adds r2, r0, r3
	ldr r2, [r2]
	lsls r3, r1, #7
	adds r3, r4, r3
	lsls r3, r3, #2
	adds r2, r2, r3
	str r2, [sp, #28]
	movs r4, #212
	lsls r4, r4, #1
	adds r2, r0, r4
	ldr r2, [r2]
	subs r4, #92
	adds r2, r2, r3
	str r2, [sp, #24]
	movs r2, #164
	lsls r2, r2, #1
	adds r3, r0, r2
	ldr r3, [r3]
	asrs r3, r3, #20
	str r3, [sp, #20]
	adds r3, r0, r4
	adds r4, #52
	ldr r2, [r3]
	adds r3, r0, r4
	ldr r3, [r3]
	adds r4, #4
	asrs r3, r3, #20
	str r3, [sp, #16]
	adds r3, r0, r4
	ldr r3, [r3]
	movs r0, #1
	asrs r3, r3, #20
	adds r3, r1, r3
	subs r3, #2
	asrs r2, r2, #20
	negs r0, r0
	str r3, [sp, #8]
	str r0, [sp, #40]
	subs r3, r1, #1
	adds r1, r1, r2
	subs r1, #1
	mov r8, r3
	mov r11, r1
.L_0200b0a4:
	ldr r0, [sp, #32]
	ldr r1, [sp, #16]
	ldr r2, [sp, #20]
	movs r4, #1
	adds r3, r0, r1
	subs r3, #1
	negs r4, r4
	mov r9, r3
	str r4, [sp, #36]
	adds r3, r0, r2
	adds r6, r0, #0
	subs r3, #1
	subs r6, #1
	mov r10, r3
.L_0200b0c0:
	ldr r4, [sp, #40]
	ldr r0, [sp, #36]
	lsls r3, r4, #7
	adds r3, r3, r0
	lsls r3, r3, #2
	ldr r1, [sp, #28]
	str r3, [sp, #12]
	adds r2, r3, r1
	ldrb r3, [r2, #2]
	cmp r3, #77
	bne .L_0200b120
	movs r3, #0
	strb r3, [r2, #2]
	mov r2, r10
	mov r3, r11
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r0, #64
	movs r1, #64
	movs r2, #1
	movs r3, #1
	bl Func_020054f0
	mov r4, r8
	movs r1, #64
	movs r2, #1
	movs r3, #1
	movs r0, #64
	str r4, [sp, #4]
	str r6, [sp, #0]
	bl Func_020054e8
	movs r0, #159
	lsls r0, r0, #1
	adds r0, #255
	bl Func_020056c0
	mov r3, r8
	movs r4, #128
	lsls r4, r4, #12
	lsls r2, r3, #20
	lsls r0, r6, #20
	adds r0, r0, r4
	ldr r1, [r5, #12]
	ldrh r3, [r5, #6]
	adds r2, r2, r4
	bl Func_02002e8c
.L_0200b120:
	ldr r4, [sp, #12]
	ldr r0, [sp, #24]
	adds r2, r4, r0
	ldrb r3, [r2, #2]
	cmp r3, #77
	bne .L_0200b174
	movs r3, #0
	strb r3, [r2, #2]
	ldr r2, [sp, #8]
	mov r1, r9
	str r1, [sp, #0]
	str r2, [sp, #4]
	movs r0, #64
	movs r1, #64
	movs r2, #1
	movs r3, #2
	bl Func_020054f0
	mov r3, r8
	str r3, [sp, #4]
	movs r1, #64
	movs r2, #1
	movs r3, #1
	movs r0, #64
	str r6, [sp, #0]
	bl Func_020054e8
	movs r0, #143
	lsls r0, r0, #2
	bl Func_020056c0
	mov r3, r8
	movs r4, #128
	lsls r4, r4, #12
	lsls r2, r3, #20
	lsls r0, r6, #20
	adds r0, r0, r4
	ldr r1, [r5, #12]
	ldrh r3, [r5, #6]
	adds r2, r2, r4
	bl Func_02002e8c
.L_0200b174:
	ldr r0, [sp, #36]
	movs r4, #1
	adds r0, #1
	add r9, r4
	adds r6, #1
	add r10, r4
	str r0, [sp, #36]
	cmp r0, #1
	ble .L_0200b0c0
	ldr r1, [sp, #8]
	ldr r2, [sp, #40]
	adds r1, #1
	adds r2, #1
	str r1, [sp, #8]
	add r8, r4
	add r11, r4
	str r2, [sp, #40]
	cmp r2, #1
	ble .L_0200b0a4
	ldr r3, [r5, #24]
	movs r4, #128
	lsls r4, r4, #9
	cmp r3, r4
	bge .L_0200b1b2
	movs r2, #128
	lsls r2, r2, #5
	adds r3, r3, r2
	str r3, [r5, #24]
	ldr r3, [r5, #28]
	adds r3, r3, r2
	str r3, [r5, #28]
.L_0200b1b2:
	ldr r1, [r7]
	ldr r2, [r7, #4]
	ldr r3, [r7, #8]
	adds r0, r5, #0
	bl Func_020054b8
	movs r3, #192
	lsls r3, r3, #18
	ldr r7, [r3, #108]
	ldr r3, .L_0200b278
	movs r0, #133
	lsls r0, r0, #2
	adds r3, r3, r0
	ldr r0, [r3]
	ldr r6, .L_0200b27c
	bl Object_GetById
	ldr r1, [r5, #8]
	ldr r3, [r0, #8]
	subs r2, r1, r3
	cmp r2, #0
	blt .L_0200b1e8
	movs r1, #160
	lsls r1, r1, #13
	cmp r2, r1
	blt .L_0200b1f2
	b .L_0200b268
.L_0200b1e8:
	movs r2, #160
	subs r3, r3, r1
	lsls r2, r2, #13
	cmp r3, r2
	bge .L_0200b268
.L_0200b1f2:
	ldr r3, [r5, #12]
	ldr r2, [r0, #12]
	ldr r4, .L_0200b280
	ldr r1, .L_0200b284
	subs r3, r3, r2
	adds r3, r3, r4
	cmp r3, r1
	bhi .L_0200b268
	ldr r3, [r5, #16]
	ldr r0, [r0, #16]
	subs r2, r3, r0
	cmp r2, #0
	blt .L_0200b216
	movs r3, #160
	lsls r3, r3, #13
	cmp r2, r3
	blt .L_0200b220
	b .L_0200b268
.L_0200b216:
	movs r4, #160
	subs r3, r0, r3
	lsls r4, r4, #13
	cmp r3, r4
	bge .L_0200b268
.L_0200b220:
	movs r3, #2
	strh r3, [r6, #4]
	ldrh r3, [r6, #10]
	movs r0, #170
	lsls r0, r0, #1
	adds r3, #2
	adds r2, r7, r0
	str r5, [r6, #108]
	strh r3, [r2]
	b .L_0200b268
.L_0200b234:
	cmp r3, #1
	bne .L_0200b268
	adds r3, r5, #0
	adds r3, #91
	movs r2, #0
	strb r2, [r3]
	ldr r3, [r5, #24]
	cmp r3, #0
	ble .L_0200b25a
	ldr r2, .L_0200b288
	adds r0, r5, #0
	adds r3, r3, r2
	str r3, [r5, #24]
	ldr r3, [r5, #28]
	adds r3, r3, r2
	str r3, [r5, #28]
	bl Func_02002d08
	b .L_0200b268
.L_0200b25a:
	str r2, [r5, #16]
	str r2, [r5, #12]
	str r2, [r5, #8]
	str r2, [r5, #44]
	str r2, [r5, #40]
	str r2, [r5, #36]
	str r2, [r5, #108]
.L_0200b268:
	add sp, #56
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200b278:
	.4byte gPartyState
.L_0200b27c:
	.4byte gData_020023c4 + 0x188
.L_0200b280:
	.4byte 0x0007ffff
.L_0200b284:
	.4byte 0x001ffffe
.L_0200b288:
	.4byte 0xfffff000
	.section .text.x0200b304,"ax",%progbits
	.balign 4
	.global Func_02003304
	.thumb_func
Func_02003304:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	mov r11, r0
	mov r3, r11
	adds r3, #98
	ldrb r0, [r3]
	sub sp, #12
	bl Object_GetById
	mov r3, r11
	adds r7, r0, #0
	adds r3, #102
	movs r2, #0
	ldrsh r6, [r3, r2]
	ldr r3, [r7, #80]
	mov r2, r11
	mov r9, r3
	ldr r3, [r2, #8]
	mov r5, sp
	str r3, [r5]
	movs r0, #128
	ldr r3, [r2, #12]
	ldr r2, .L_0200b394
	adds r1, r6, #0
	adds r3, r3, r2
	str r3, [r5, #4]
	mov r2, r11
	ldr r3, [r2, #16]
	lsls r0, r0, #14
	adds r2, r5, #0
	str r3, [r5, #8]
	bl Vector_AddPolarOffsetFar
	ldr r3, .L_0200b390
	movs r2, #0
	mov r10, r3
	adds r3, r7, #0
	mov r8, r2
	adds r3, #85
	mov r2, r10
	strh r6, [r7, #6]
	strb r2, [r3]
	adds r0, r7, #0
	ldr r1, [r5]
	ldr r2, [r5, #4]
	ldr r3, [r5, #8]
	bl Object_SetPositionAndResetMotion
	ldr r3, .L_0200b398
	mov r2, r10
	str r3, [r7, #108]
	adds r3, r7, #0
	adds r3, #90
	strb r2, [r3]
	movs r3, #128
	lsls r3, r3, #12
	str r3, [r7, #48]
	movs r3, #128
	lsls r3, r3, #6
	str r3, [r7, #52]
	ldr r3, .L_0200b39c
	mov r2, r9
	adds r6, r6, r3
	b .L_0200b3a0
	.2byte 0x0000
.L_0200b390:
	.4byte 0x00000000
.L_0200b394:
	.4byte 0xfff40000
.L_0200b398:
	.4byte Func_02002f80
.L_0200b39c:
	.4byte 0xffffc000
.L_0200b3a0:
	mov r3, r8
	strh r6, [r2, #18]
	str r3, [r7, #24]
	str r3, [r7, #28]
	adds r3, r7, #0
	mov r2, r8
	adds r3, #100
	strh r2, [r3]
	mov r2, r11
	ldr r3, [r2, #104]
	movs r0, #104
	str r3, [r7, #104]
	bl Func_020056c0
	add sp, #12
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x0200b5b8,"ax",%progbits
	.balign 4
	.global Func_020035b8
	.thumb_func
Func_020035b8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	mov r11, r3
	ldr r3, .L_0200b770
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
	beq .L_0200b600
	cmp r7, #0
	beq .L_0200b600
	movs r2, #24
	ldrsh r0, [r7, r2]
	adds r1, r5, #0
	adds r2, r6, #0
	b .L_0200b608
.L_0200b600:
	movs r0, #30
	adds r2, r6, #0
	adds r0, #255
	adds r1, r5, #0
.L_0200b608:
	mov r3, r10
	bl Func_02005498
	adds r6, r0, #0
	cmp r6, #0
	bne .L_0200b616
	b .L_0200b762
.L_0200b616:
	ldr r3, [r6, #80]
	mov r1, r8
	movs r5, #15
	adds r1, #1
	ands r1, r5
	adds r0, r6, #0
	str r3, [sp, #0]
	bl Func_02005480
	ldr r2, .L_0200b774
	mov r3, r8
	ands r3, r5
	lsls r3, r3, #2
	ldr r1, [r2, r3]
	adds r0, r6, #0
	mov r10, r3
	bl Func_02005490
	adds r3, r6, #0
	movs r5, #0
	adds r3, #85
	strb r5, [r3]
	adds r0, r6, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	ldr r3, .L_0200b778
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
	ldr r3, .L_0200b77c
	mov r1, r8
	ands r3, r1
	cmp r3, #0
	beq .L_0200b762
	cmp r7, #0
	beq .L_0200b762
	movs r3, #128
	lsls r3, r3, #9
	ands r3, r1
	cmp r3, #0
	beq .L_0200b698
	ldr r1, [r7, #4]
	adds r0, r6, #0
	bl Object_SetPartAttribute
.L_0200b698:
	movs r3, #128
	lsls r3, r3, #10
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_0200b6b8
	adds r1, r6, #0
	adds r1, #35
	ldrb r3, [r1]
	movs r2, #254
	ands r2, r3
	strb r2, [r1]
	ldr r1, [r7]
	adds r0, r6, #0
	bl Object_SetSpritePriority
.L_0200b6b8:
	movs r2, #128
	lsls r2, r2, #12
	mov r3, r8
	ands r2, r3
	cmp r2, #0
	beq .L_0200b6cc
	ldr r3, [r7, #8]
	str r3, [r6, #24]
	ldr r3, [r7, #12]
	str r3, [r6, #28]
.L_0200b6cc:
	movs r3, #128
	lsls r3, r3, #11
	mov r1, r8
	ands r3, r1
	cmp r3, #0
	beq .L_0200b712
	ldr r3, .L_0200b774
	mov r1, r10
	ldr r5, [r3, r1]
	ldr r3, [r7, #16]
	ldr r1, [r5, #12]
	cmp r2, #0
	beq .L_0200b6fa
	ldr r0, [r6, #24]
	subs r0, r3, r0
	bl __divsi3
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, [r6, #28]
	ldr r1, [r5, #12]
	subs r0, r0, r3
	b .L_0200b70c
.L_0200b6fa:
	ldr r2, .L_0200b77c
	adds r0, r3, r2
	bl __divsi3
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, .L_0200b77c
	ldr r1, [r5, #12]
	adds r0, r0, r3
.L_0200b70c:
	bl __divsi3
	str r0, [r6, #52]
.L_0200b712:
	movs r3, #128
	lsls r3, r3, #14
	mov r1, r8
	ands r3, r1
	cmp r3, #0
	beq .L_0200b72e
	adds r0, r6, #0
	movs r1, #1
	bl Func_02005480
	ldr r1, [r7, #28]
	adds r0, r6, #0
	bl Func_02005490
.L_0200b72e:
	movs r3, #128
	lsls r3, r3, #15
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_0200b740
	ldrh r3, [r7, #32]
	ldr r1, [sp, #0]
	strh r3, [r1, #18]
.L_0200b740:
	movs r3, #128
	lsls r3, r3, #16
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_0200b752
	ldrh r3, [r7, #34]
	mov r1, r9
	strh r3, [r1]
.L_0200b752:
	movs r3, #128
	lsls r3, r3, #17
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_0200b762
	ldr r3, [r7, #36]
	str r3, [r6, #108]
.L_0200b762:
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0200b770:
	.4byte gPartyState
.L_0200b774:
	.4byte gData_02007058
.L_0200b778:
	.4byte BattleEffect_UpdateObjectMotionScaleAndLinkedAngle
.L_0200b77c:
	.4byte 0xffff0000
	.section .text.x0200b780,"ax",%progbits
	.global Func_02003780
	.thumb_func
Func_02003780:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r4, .L_0200b898
	movs r1, #1
	movs r0, #12
	ldrsh r3, [r4, r0]
	negs r1, r1
	sub sp, #4
	cmp r3, r1
	beq .L_0200b88c
	lsls r3, r3, #3
	adds r3, r3, r4
	adds r3, #32
	mov r8, r3
	ldr r3, .L_0200b89c
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	str r4, [sp, #0]
	bl Object_GetById
	mov r1, r8
	ldr r3, [r0, #8]
	movs r5, #0
	ldrsh r2, [r1, r5]
	asrs r3, r3, #20
	ldr r4, [sp, #0]
	cmp r3, r2
	bne .L_0200b7cc
	ldr r3, [r0, #16]
	movs r5, #2
	ldrsh r2, [r1, r5]
	asrs r3, r3, #20
	cmp r3, r2
	beq .L_0200b7d4
.L_0200b7cc:
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	strh r3, [r4, #12]
.L_0200b7d4:
	movs r0, #12
	ldrsh r3, [r4, r0]
	movs r2, #1
	negs r2, r2
	ldr r1, .L_0200b8a0
	cmp r3, r2
	beq .L_0200b88c
	movs r5, #14
	ldrsh r3, [r4, r5]
	cmp r3, #0
	beq .L_0200b88c
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #32]
	str r4, [sp, #0]
	adds r3, r2, #0
	adds r3, #228
	ldr r0, [r3]
	ldr r5, [r3, #4]
	ldr r3, [r2]
	ands r0, r1
	ands r5, r1
	ldr r6, [r3, #4]
	movs r1, #16
	ldrsh r3, [r4, r1]
	ldr r2, .L_0200b8a4
	lsls r3, r3, #2
	adds r3, r3, r2
	ldrh r3, [r3, #2]
	lsrs r3, r3, #5
	mov r10, r3
	mov r3, r8
	movs r2, #0
	ldrsh r1, [r3, r2]
	lsls r1, r1, #20
	subs r7, r1, r0
	movs r0, #2
	ldrsh r2, [r3, r0]
	movs r0, #0
	lsls r2, r2, #20
	bl Map_GetTerrainHeight
	mov r2, r8
	movs r1, #2
	ldrsh r3, [r2, r1]
	subs r0, r0, r6
	lsls r3, r3, #20
	subs r3, r3, r5
	subs r3, r3, r6
	subs r2, r3, r0
	asrs r7, r7, #16
	adds r0, r0, r3
	asrs r0, r0, #16
	adds r3, r7, #0
	movs r5, #167
	asrs r2, r2, #16
	adds r1, r0, #0
	adds r3, #15
	lsls r5, r5, #1
	adds r2, #14
	adds r1, #58
	ldr r4, [sp, #0]
	cmp r3, r5
	bhi .L_0200b88c
	movs r0, #15
	negs r0, r0
	cmp r2, r0
	blt .L_0200b88c
	cmp r2, #239
	bgt .L_0200b88c
	movs r3, #128
	lsls r3, r3, #1
	adds r3, #255
	ands r7, r3
	movs r3, #255
	ands r2, r3
	movs r3, #0
	str r3, [r4, #20]
	lsls r3, r7, #16
	orrs r2, r3
	ldr r3, .L_0200b8a8
	adds r0, r4, #0
	orrs r2, r3
	movs r3, #128
	str r2, [r4, #24]
	lsls r3, r3, #3
	mov r2, r10
	orrs r2, r3
	str r2, [r4, #28]
	adds r0, #20
	bl Func_02005440
.L_0200b88c:
	add sp, #4
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200b898:
	.4byte gData_020070a8
.L_0200b89c:
	.4byte gPartyState
.L_0200b8a0:
	.4byte 0xffff0000
.L_0200b8a4:
	.4byte gResourceTableEntries
.L_0200b8a8:
	.4byte 0x80008800
	.section .text.x0200b8ac,"ax",%progbits
	.global Func_020038ac
	.thumb_func
Func_020038ac:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #48
	str r0, [sp, #44]
	ldr r0, .L_0200badc
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
	ldr r3, .L_0200bae0
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
	bge .L_0200ba30
.L_0200b960:
	ldr r1, [sp, #12]
	ldr r2, [sp, #36]
	ldr r5, [sp, #24]
	lsls r3, r1, #9
	adds r2, r2, r3
	movs r3, #0
	mov r11, r2
	str r3, [sp, #16]
	cmp r3, r5
	bge .L_0200ba24
.L_0200b974:
	mov r0, r11
	ldrb r5, [r0, #2]
	cmp r5, #0
	beq .L_0200ba14
	ldr r1, [sp, #44]
	cmp r5, r1
	bcc .L_0200ba14
	adds r1, #1
	mov r10, r1
	cmp r5, r10
	bhi .L_0200ba14
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
	bne .L_0200b9c8
	cmp r5, r10
	bne .L_0200ba06
	mov r3, r9
	movs r2, #4
	ldrsh r0, [r3, r2]
	bl GameFlag_SetBit
	b .L_0200ba06
.L_0200b9c8:
	mov r1, r9
	movs r5, #4
	ldrsh r0, [r1, r5]
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200ba06
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
	bl Func_020054e0
.L_0200ba06:
	mov r0, r8
	ldrh r3, [r0, #10]
	mov r1, r8
	adds r3, #1
	strh r3, [r1, #10]
	movs r5, #8
	add r9, r5
.L_0200ba14:
	ldr r2, [sp, #16]
	ldr r5, [sp, #24]
	adds r2, #1
	movs r3, #4
	str r2, [sp, #16]
	add r11, r3
	cmp r2, r5
	blt .L_0200b974
.L_0200ba24:
	ldr r0, [sp, #12]
	ldr r1, [sp, #20]
	adds r0, #1
	str r0, [sp, #12]
	cmp r0, r1
	blt .L_0200b960
.L_0200ba30:
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200ba88
	ldr r3, .L_0200bae4
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
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
	bge .L_0200ba88
.L_0200ba62:
	mov r0, r9
	movs r5, #0
	ldrsh r3, [r0, r5]
	cmp r3, r4
	bne .L_0200ba78
	movs r5, #2
	ldrsh r3, [r0, r5]
	cmp r3, r1
	bne .L_0200ba78
	mov r0, r8
	strh r2, [r0, #12]
.L_0200ba78:
	movs r3, #8
	mov r0, r8
	add r9, r3
	movs r5, #10
	ldrsh r3, [r0, r5]
	adds r2, #1
	cmp r2, r3
	blt .L_0200ba62
.L_0200ba88:
	movs r0, #128
	lsls r0, r0, #1
	bl MemAlloc
	adds r5, r0, #0
	adds r1, r5, #0
	movs r2, #63
.L_0200ba96:
	ldr r3, .L_0200bae8
	subs r2, #1
	stmia r1!, {r3}
	cmp r2, #0
	bge .L_0200ba96
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
	bl MemFree
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_0200baec
	bl Scheduler_AddOrUpdateCallback
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
.L_0200badc:
	.4byte gData_020070a8
.L_0200bae0:
	.4byte IwramClearWords
.L_0200bae4:
	.4byte gPartyState
.L_0200bae8:
	.4byte 0x11111111
.L_0200baec:
	.4byte Func_02003780
	.section .text.x0200bb78,"ax",%progbits
	.global Func_02003b78
	.thumb_func
Func_02003b78:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	ldr r3, .L_0200bbe4
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	mov r8, r0
	ldr r0, [r3]
	sub sp, #8
	bl Object_GetById
	mov r2, r8
	ldrh r1, [r2, #6]
	movs r2, #64
	ldr r6, [r0, #8]
	adds r3, r2, #0
	ands r3, r1
	lsls r3, r3, #16
	mov r1, r8
	asrs r3, r3, #16
	asrs r6, r6, #20
	orrs r6, r3
	ldrh r3, [r1, #8]
	ldr r5, [r0, #16]
	ands r2, r3
	lsls r2, r2, #16
	asrs r2, r2, #16
	asrs r5, r5, #20
	orrs r5, r2
	bl Func_02003af0
	movs r0, #161
	bl Func_020056c0
	mov r3, r8
	movs r2, #8
	ldrsh r1, [r3, r2]
	movs r2, #6
	ldrsh r0, [r3, r2]
	adds r1, #1
	movs r2, #1
	movs r3, #1
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl Func_020054e0
	movs r0, #12
	bl FieldEvent_WaitFrames
	add sp, #8
	pop {r3}
	mov r8, r3
	pop {r5, r6, pc}
.L_0200bbe4:
	.4byte gPartyState
	.section .text.x0200bbe8,"ax",%progbits
	.global Func_02003be8
	.thumb_func
Func_02003be8:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r1, .L_0200bc98
	movs r2, #133
	lsls r2, r2, #2
	adds r1, r1, r2
	mov r8, r0
	ldr r0, [r1]
	sub sp, #8
	mov r10, r1
	bl Object_GetById
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
	bl Func_02003af0
	movs r0, #229
	bl Func_020056c0
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
	bl Func_020054e0
	movs r0, #12
	bl FieldEvent_WaitFrames
	movs r3, #128
	ldr r2, .L_0200bc90
	lsls r3, r3, #7
	strh r3, [r6, #6]
	adds r3, r6, #0
	adds r3, #85
	strb r2, [r3]
	mov r3, r10
	ldr r0, [r3]
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
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
	ldr r2, .L_0200bc94
	movs r7, #0
	orrs r3, r2
	strh r3, [r1]
	mov r2, r10
	b .L_0200bc9c
	.2byte 0x0000
.L_0200bc90:
	.4byte 0x00000000
.L_0200bc94:
	.4byte 0x00008000
.L_0200bc98:
	.4byte gPartyState
.L_0200bc9c:
	movs r3, #1
	mov r1, r8
	strh r3, [r1, #14]
	ldr r0, [r2]
	movs r1, #28
	bl Object_SetModeById
	movs r0, #16
	bl FieldEvent_WaitFrames
.L_0200bcb0:
	cmp r7, #5
	bne .L_0200bcba
	movs r0, #204
	bl Func_020056c0
.L_0200bcba:
	ldr r3, [r6, #24]
	ldr r1, .L_0200bd18
	ldr r2, .L_0200bd1c
	adds r3, r3, r1
	str r3, [r6, #24]
	ldr r3, [r6, #28]
	ldr r1, .L_0200bd20
	adds r3, r3, r2
	str r3, [r6, #28]
	ldr r3, [r6, #12]
	movs r0, #1
	adds r3, r3, r1
	str r3, [r6, #12]
	adds r7, #1
	bl WaitFrames
	cmp r7, #39
	ble .L_0200bcb0
	ldr r3, .L_0200bd24
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
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
.L_0200bd18:
	.4byte 0xfffffc00
.L_0200bd1c:
	.4byte 0xfffffd00
.L_0200bd20:
	.4byte 0xffff6667
.L_0200bd24:
	.4byte gPartyState
	.section .text.x0200bd80,"ax",%progbits
	.balign 4
	.global Func_02003d80
	.thumb_func
Func_02003d80:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, .L_0200bf04
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	sub sp, #68
	bl Object_GetById
	adds r7, r0, #0
	bl Func_02005560
	movs r0, #0
	bl Func_02005648
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r1, r1
	negs r2, r2
	movs r3, #0
	negs r0, r0
	bl Camera_MoveWithinMapBounds
	bl Func_020054b0
	movs r0, #1
	bl WaitFrames
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
	bl Func_020056c0
	movs r3, #3
	strb r3, [r5]
	movs r0, #24
	bl FieldEvent_WaitFrames
	add r2, sp, #28
	movs r3, #7
	str r3, [r2, #4]
	ldr r3, .L_0200bf08
	mov r8, r2
	str r3, [r2, #36]
	movs r3, #163
	lsls r3, r3, #8
	adds r3, #215
	str r3, [r2, #8]
	str r3, [r2, #12]
	movs r3, #0
	mov r10, r3
.L_0200be1a:
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
	ldr r3, .L_0200bf0c
	adds r2, r2, r3
	str r2, [r6]
	bl Random16Far
	lsls r3, r0, #1
	ldr r5, [r6, #8]
	adds r3, r3, r0
	ldr r4, .L_0200bf10
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
	ldr r4, .L_0200bf14
	str r5, [sp, #4]
	str r4, [sp, #8]
	mov r4, r8
	str r4, [sp, #12]
	bl Func_020035b8
	movs r2, #1
	add r10, r2
	mov r3, r10
	cmp r3, #16
	bls .L_0200be1a
	movs r0, #188
	bl Func_020056c0
	ldr r5, .L_0200bf04
	movs r4, #133
	lsls r4, r4, #2
	adds r5, r5, r4
	movs r1, #2
	ldr r0, [r5]
	adds r1, #255
	bl Func_020055c0
	ldr r0, [r5]
	movs r1, #49
	bl Object_SetModeById
	movs r0, #160
	movs r1, #160
	movs r2, #128
	lsls r0, r0, #11
	lsls r1, r1, #11
	lsls r2, r2, #9
	bl Func_02005510
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	adds r2, #102
	negs r0, r0
	negs r1, r1
	bl Func_02005510
	bl Func_02005518
	movs r1, #128
	ldr r0, [r5]
	lsls r1, r1, #1
	bl Func_020055c0
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r7, #72]
	movs r3, #128
	lsls r3, r3, #7
	str r3, [r7, #68]
	movs r0, #10
	bl FieldEvent_WaitFrames
	ldr r0, [r5]
	movs r1, #1
	bl Object_SetModeById
	bl Func_02005568
	add sp, #68
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_0200bf04:
	.4byte gPartyState
.L_0200bf08:
	.4byte Effect_AdvanceMotion
.L_0200bf0c:
	.4byte 0xffffa000
.L_0200bf10:
	.4byte 0xffffd000
.L_0200bf14:
	.4byte 0x01090001
	.section .text.x0200bf18,"ax",%progbits
	.global Func_02003f18
	.thumb_func
Func_02003f18:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, .L_0200bfc0
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	ldr r0, [r3]
	bl Object_GetById
	ldr r3, [r0, #8]
	ldr r6, .L_0200bfc4
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
	bge .L_0200bfb4
.L_0200bf4c:
	movs r1, #0
	ldrsh r3, [r5, r1]
	cmp r3, r8
	bne .L_0200bfa8
	movs r1, #2
	ldrsh r3, [r5, r1]
	cmp r3, r10
	bne .L_0200bfa8
	movs r2, #4
	ldrsh r0, [r5, r2]
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200bf7c
	adds r0, r6, #0
	adds r1, r5, #0
	bl Func_02003b78
	movs r3, #4
	ldrsh r0, [r5, r3]
	bl GameFlag_SetBit
	strh r7, [r6, #12]
	b .L_0200bfb4
.L_0200bf7c:
	movs r1, #12
	ldrsh r3, [r6, r1]
	cmp r7, r3
	beq .L_0200bfb4
	adds r0, r6, #0
	adds r1, r5, #0
	strh r7, [r6, #12]
	bl Func_02003be8
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
	b .L_0200bfb6
.L_0200bfa8:
	lsls r3, r2, #16
	adds r7, #1
	asrs r3, r3, #16
	adds r5, #8
	cmp r7, r3
	blt .L_0200bf4c
.L_0200bfb4:
	movs r0, #0
.L_0200bfb6:
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200bfc0:
	.4byte gPartyState
.L_0200bfc4:
	.4byte gData_020070a8
	.section .text.x0200bfc8,"ax",%progbits
	.global Func_02003fc8
	.thumb_func
Func_02003fc8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #4
	ldr r3, .L_0200c078
	str r2, [sp, #0]
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	mov r9, r0
	ldr r0, [r3]
	mov r11, r1
	bl Object_GetById
	movs r3, #192
	ldr r5, .L_0200c07c
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	adds r6, r0, #0
	movs r2, #2
	ldrsh r0, [r5, r2]
	mov r10, r3
	bl GameFlag_GetByte
	adds r7, r0, #0
	movs r3, #2
	ldrsh r0, [r5, r3]
	adds r0, #8
	bl GameFlag_GetByte
	mov r8, r0
	cmp r7, #0
	bne .L_0200c016
	cmp r0, #0
	beq .L_0200c06a
.L_0200c016:
	movs r2, #2
	ldrsh r0, [r5, r2]
	movs r1, #0
	bl GameFlag_SetByte
	movs r3, #2
	ldrsh r0, [r5, r3]
	movs r1, #0
	adds r0, #8
	bl GameFlag_SetByte
	mov r3, r9
	adds r2, r7, r3
	mov r3, r8
	movs r1, #128
	add r3, r11
	lsls r1, r1, #12
	lsls r3, r3, #20
	adds r3, r3, r1
	str r3, [r6, #16]
	movs r3, #230
	lsls r3, r3, #1
	add r3, r10
	lsls r2, r2, #20
	adds r2, r2, r1
	ldr r1, [r3]
	str r2, [r6, #8]
	str r2, [r1, #8]
	ldr r3, [r6, #16]
	str r3, [r1, #16]
	bl Func_020054b0
	bl Func_02003d80
	movs r2, #192
	lsls r2, r2, #18
	ldr r3, [r2, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	ldr r2, [sp, #0]
	str r2, [r3]
.L_0200c06a:
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0200c078:
	.4byte gPartyState
.L_0200c07c:
	.4byte gData_020070a8
	.section .text.x0200c140,"ax",%progbits
	.balign 4
	.global Func_02004140
	.thumb_func
Func_02004140:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, .L_0200c200
	movs r0, #133
	lsls r0, r0, #2
	adds r3, r3, r0
	ldr r0, [r3]
	sub sp, #40
	bl ObjectTable_Get
	adds r7, r0, #0
.L_0200c160:
	bl Func_0200410c
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r7, #48]
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r7, #52]
	movs r3, #128
	lsls r3, r3, #24
	str r3, [r7, #56]
	str r3, [r7, #64]
	movs r3, #0
	str r3, [r7, #36]
	str r3, [r7, #44]
	ldr r1, .L_0200c204
	ldr r3, [r7, #8]
	add r2, sp, #28
	mov r10, r2
	movs r2, #128
	lsls r2, r2, #12
	ands r3, r1
	adds r3, r3, r2
	mov r0, r10
	str r3, [sp, #12]
	str r3, [r0]
	ldr r3, [r7, #12]
	str r3, [r0, #4]
	ldr r3, [r7, #16]
	ands r3, r1
	adds r3, r3, r2
	str r3, [sp, #8]
	str r3, [r0, #8]
	ldr r2, [sp, #12]
	str r3, [sp, #16]
	adds r3, r7, #0
	adds r3, #34
	str r2, [sp, #20]
	str r3, [sp, #4]
	adds r1, r2, #0
	ldrb r0, [r3]
	ldr r2, [sp, #16]
	bl Func_020054d0
	str r0, [sp, #24]
	ldr r2, [sp, #20]
	ldr r1, [r7, #8]
	ldr r0, [sp, #16]
	ldr r3, .L_0200c208
	ldr r6, [r7, #16]
	subs r1, r2, r1
	subs r6, r0, r6
	mov r8, r3
	adds r0, r1, #0
	mov lr, r8
	.2byte 0xf800
	adds r1, r6, #0
	adds r5, r0, #0
	adds r0, r6, #0
	mov lr, r8
	.2byte 0xf800
	adds r5, r5, r0
	movs r0, #128
	lsls r0, r0, #11
	cmp r5, r0
	bge .L_0200c218
	ldr r3, .L_0200c20c
	movs r2, #15
	ldr r3, [r3]
	ldr r1, .L_0200c210
	lsrs r3, r3, #4
	ands r3, r2
	lsls r3, r3, #1
	ldrsh r2, [r1, r3]
	mov r9, r2
	lsls r3, r2, #16
	ldr r2, .L_0200c214
	cmp r3, r2
	bne .L_0200c244
	b .L_0200c3da
.L_0200c200:
	.4byte gPartyState
.L_0200c204:
	.4byte 0xfff00000
.L_0200c208:
	.4byte IwramMulQ16
.L_0200c20c:
	.4byte gInput
.L_0200c210:
	.4byte gData_02005f38
.L_0200c214:
	.4byte 0xffff0000
.L_0200c218:
	ldr r3, [sp, #16]
	ldr r2, [sp, #20]
	ldr r0, [r7, #16]
	ldr r1, [r7, #8]
	subs r0, r3, r0
	subs r1, r2, r1
	bl ArcTan2
	lsls r0, r0, #16
	asrs r0, r0, #16
	movs r3, #128
	ldr r2, .L_0200c240
	mov r9, r0
	lsls r3, r3, #6
	add r3, r9
	ands r3, r2
	lsls r3, r3, #16
	asrs r3, r3, #16
	mov r9, r3
	b .L_0200c244
.L_0200c240:
	.4byte 0xffffc000
.L_0200c244:
	movs r0, #128
	add r2, sp, #28
	lsls r0, r0, #13
	mov r1, r9
	bl Vector_AddPolarOffsetFar
	ldr r3, [sp, #4]
	mov r2, r10
	ldr r1, [r2]
	ldrb r0, [r3]
	ldr r2, [r2, #8]
	bl Func_020054d0
	mov r11, r0
	cmp r0, #255
	beq .L_0200c2c6
	ldr r3, [sp, #4]
	mov r2, r10
	ldrb r0, [r3]
	ldr r1, [r2]
	ldr r2, [r2, #8]
	bl Map_GetTerrainHeight
	ldr r3, [r7, #12]
	subs r0, r0, r3
	movs r3, #128
	lsls r3, r3, #12
	cmp r0, r3
	bgt .L_0200c2c6
	ldr r0, [sp, #12]
	mov r2, r10
	str r0, [r2]
	ldr r3, [sp, #8]
	adds r0, r7, #0
	str r3, [r2, #8]
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r7, #48]
	movs r3, #200
	lsls r3, r3, #5
	adds r3, #153
	adds r2, r7, #0
	str r3, [r7, #52]
	adds r2, #100
	movs r3, #0
	strh r3, [r2]
	ldr r1, [sp, #12]
	ldr r3, [sp, #8]
	ldr r2, [r7, #12]
	bl Func_020054b8
	adds r0, r7, #0
	movs r1, #2
	bl Func_02005480
	adds r0, r7, #0
	movs r1, #48
	bl ObjectDispatch_SetAnimationSpeed
	adds r0, r7, #0
	bl Func_020054c0
	ldr r3, .L_0200c3e8
	str r3, [r7, #108]
	b .L_0200c370
.L_0200c2c6:
	movs r3, #0
	mov r0, r9
	strh r0, [r7, #6]
	str r3, [r7, #36]
	str r3, [r7, #44]
	ldr r2, [sp, #12]
	str r2, [r7, #8]
	ldr r3, [sp, #8]
	str r3, [r7, #16]
	b .L_0200c3bc
.L_0200c2da:
	ldr r2, [sp, #4]
	ldr r1, [r6]
	ldrb r0, [r2]
	ldr r2, [r6, #8]
	bl Map_GetTerrainHeight
	ldr r3, [r7, #12]
	subs r0, r0, r3
	movs r3, #128
	lsls r3, r3, #12
	cmp r0, r3
	bgt .L_0200c390
	ldrh r3, [r7, #32]
	movs r2, #89
	subs r3, #2
	str r3, [sp, #0]
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #20]
	movs r0, #0
	adds r2, r2, r5
	mov r10, r0
	mov r8, r2
.L_0200c308:
	ldr r3, [r5]
	cmp r3, #0
	beq .L_0200c332
	mov r3, r8
	ldrb r2, [r3]
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	beq .L_0200c332
	cmp r5, r7
	beq .L_0200c332
	ldrh r3, [r5, #32]
	adds r0, r5, #0
	adds r0, #8
	subs r3, #2
	ldr r1, [sp, #0]
	add r2, sp, #28
	bl Func_02005538
	cmp r0, #0
	bge .L_0200c390
.L_0200c332:
	movs r0, #1
	add r10, r0
	movs r2, #128
	mov r3, r10
	add r8, r2
	adds r5, #128
	cmp r3, #63
	ble .L_0200c308
	ldr r0, [r6]
	movs r3, #128
	str r0, [sp, #20]
	lsls r3, r3, #10
	ldr r2, [r6, #8]
	adds r0, r7, #0
	str r2, [sp, #16]
	str r3, [r7, #48]
	movs r3, #200
	lsls r3, r3, #5
	adds r3, #153
	str r3, [r7, #52]
	ldr r3, [r6, #8]
	ldr r1, [r6]
	ldr r2, [r6, #4]
	bl Func_020054b8
	adds r0, r7, #0
	bl Func_020054c0
	ldr r3, [sp, #24]
	cmp r11, r3
	bne .L_0200c3b6
.L_0200c370:
	movs r0, #128
	add r2, sp, #28
	lsls r0, r0, #13
	mov r1, r9
	bl Vector_AddPolarOffsetFar
	ldr r2, [sp, #4]
	add r6, sp, #28
	ldrb r0, [r2]
	ldr r1, [r6]
	ldr r2, [r6, #8]
	bl Func_020054d0
	mov r11, r0
	cmp r0, #255
	bne .L_0200c2da
.L_0200c390:
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r7, #48]
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r7, #52]
	ldr r2, [r7, #12]
	adds r0, r7, #0
	ldr r1, [sp, #20]
	ldr r3, [sp, #16]
	bl Func_020054b8
	adds r0, r7, #0
	bl Func_020054c0
	movs r0, #2
	bl WaitFrames
	b .L_0200c160
.L_0200c3b6:
	movs r0, #10
	bl WaitFrames
.L_0200c3bc:
	movs r3, #0
	str r3, [r7, #108]
	adds r1, r7, #0
	adds r1, #90
	ldrb r2, [r1]
	movs r3, #1
	orrs r3, r2
	strb r3, [r1]
	movs r3, #128
	lsls r3, r3, #7
	str r3, [r7, #52]
	adds r0, r7, #0
	movs r1, #1
	bl Func_02005480
.L_0200c3da:
	add sp, #40
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0200c3e8:
	.4byte Func_02004080
	.section .text.x0200c3ec,"ax",%progbits
	.ifdef TLA_EDITION_JA
	.global Func_020043ec
	.thumb_func
Func_020043ec:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, .L_ja_0200c298
	movs r0, #133
	lsls r0, r0, #2
	adds r3, r3, r0
	ldr r0, [r3]
	sub sp, #40
	bl ObjectTable_Get
	ldr r2, .L_ja_0200c29c
	ldr r3, .L_ja_0200c294
	adds r7, r0, #0
	strh r3, [r2]
.L_ja_0200c25e:
	bl Func_0200410c
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r7, #48]
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r7, #52]
	movs r3, #128
	lsls r3, r3, #24
	str r3, [r7, #56]
	str r3, [r7, #64]
	movs r3, #0
	str r3, [r7, #36]
	str r3, [r7, #44]
	ldr r1, .L_ja_0200c2a0
	ldr r3, [r7, #8]
	add r2, sp, #28
	mov r10, r2
	movs r2, #128
	lsls r2, r2, #12
	ands r3, r1
	adds r3, r3, r2
	mov r0, r10
	str r3, [sp, #12]
	str r3, [r0]
	b .L_ja_0200c2a4
.L_ja_0200c294:
	.4byte 0x00000000
.L_ja_0200c298:
	.4byte gPartyState
.L_ja_0200c29c:
	.4byte gData_020074c8
.L_ja_0200c2a0:
	.4byte 0xfff00000
.L_ja_0200c2a4:
	ldr r3, [r7, #12]
	str r3, [r0, #4]
	ldr r3, [r7, #16]
	ands r3, r1
	adds r3, r3, r2
	str r3, [sp, #8]
	str r3, [r0, #8]
	ldr r2, [sp, #12]
	str r3, [sp, #16]
	adds r3, r7, #0
	adds r3, #34
	str r2, [sp, #20]
	str r3, [sp, #4]
	adds r1, r2, #0
	ldrb r0, [r3]
	ldr r2, [sp, #16]
	bl Func_020054d0
	str r0, [sp, #24]
	ldr r2, [sp, #20]
	ldr r1, [r7, #8]
	ldr r0, [sp, #16]
	ldr r3, .L_ja_0200c310
	ldr r6, [r7, #16]
	subs r1, r2, r1
	subs r6, r0, r6
	mov r8, r3
	adds r0, r1, #0
	mov lr, r8
	.2byte 0xf800
	adds r1, r6, #0
	adds r5, r0, #0
	adds r0, r6, #0
	mov lr, r8
	.2byte 0xf800
	adds r5, r5, r0
	movs r0, #128
	lsls r0, r0, #11
	cmp r5, r0
	bge .L_ja_0200c320
	ldr r3, .L_ja_0200c314
	movs r2, #15
	ldr r3, [r3]
	ldr r1, .L_ja_0200c318
	lsrs r3, r3, #4
	ands r3, r2
	lsls r3, r3, #1
	ldrsh r2, [r1, r3]
	mov r9, r2
	lsls r3, r2, #16
	ldr r2, .L_ja_0200c31c
	cmp r3, r2
	bne .L_ja_0200c34c
	b .L_ja_0200c514
.L_ja_0200c310:
	.4byte IwramMulQ16
.L_ja_0200c314:
	.4byte gInput
.L_ja_0200c318:
	.4byte gData_02005f38
.L_ja_0200c31c:
	.4byte 0xffff0000
.L_ja_0200c320:
	ldr r3, [sp, #16]
	ldr r2, [sp, #20]
	ldr r0, [r7, #16]
	ldr r1, [r7, #8]
	subs r0, r3, r0
	subs r1, r2, r1
	bl ArcTan2
	lsls r0, r0, #16
	asrs r0, r0, #16
	movs r3, #128
	ldr r2, .L_ja_0200c348
	mov r9, r0
	lsls r3, r3, #6
	add r3, r9
	ands r3, r2
	lsls r3, r3, #16
	asrs r3, r3, #16
	mov r9, r3
	b .L_ja_0200c34c
.L_ja_0200c348:
	.4byte 0xffffc000
.L_ja_0200c34c:
	movs r0, #128
	add r2, sp, #28
	lsls r0, r0, #13
	mov r1, r9
	bl Vector_AddPolarOffsetFar
	ldr r3, [sp, #4]
	mov r2, r10
	ldr r1, [r2]
	ldrb r0, [r3]
	ldr r2, [r2, #8]
	bl Func_020054d0
	mov r11, r0
	cmp r0, #255
	beq .L_ja_0200c3ce
	ldr r3, [sp, #4]
	mov r2, r10
	ldrb r0, [r3]
	ldr r1, [r2]
	ldr r2, [r2, #8]
	bl Map_GetTerrainHeight
	ldr r3, [r7, #12]
	subs r0, r0, r3
	movs r3, #128
	lsls r3, r3, #12
	cmp r0, r3
	bgt .L_ja_0200c3ce
	ldr r0, [sp, #12]
	mov r2, r10
	str r0, [r2]
	ldr r3, [sp, #8]
	adds r0, r7, #0
	str r3, [r2, #8]
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r7, #48]
	movs r3, #200
	lsls r3, r3, #5
	adds r3, #153
	adds r2, r7, #0
	str r3, [r7, #52]
	adds r2, #100
	movs r3, #0
	strh r3, [r2]
	ldr r1, [sp, #12]
	ldr r3, [sp, #8]
	ldr r2, [r7, #12]
	bl Func_020054b8
	adds r0, r7, #0
	movs r1, #2
	bl Func_02005480
	adds r0, r7, #0
	movs r1, #48
	bl ObjectDispatch_SetAnimationSpeed
	adds r0, r7, #0
	bl Func_020054c0
	ldr r3, .L_ja_0200c524
	str r3, [r7, #108]
	b .L_ja_0200c4aa
.L_ja_0200c3ce:
	movs r3, #0
	mov r0, r9
	strh r0, [r7, #6]
	str r3, [r7, #36]
	str r3, [r7, #44]
	ldr r2, [sp, #12]
	str r2, [r7, #8]
	ldr r3, [sp, #8]
	str r3, [r7, #16]
	b .L_ja_0200c4f6
.L_ja_0200c3e2:
	ldr r2, [sp, #4]
	ldr r1, [r6]
	ldrb r0, [r2]
	ldr r2, [r6, #8]
	bl Map_GetTerrainHeight
	ldr r3, [r7, #12]
	subs r0, r0, r3
	movs r3, #128
	lsls r3, r3, #12
	cmp r0, r3
	bgt .L_ja_0200c4ca
	ldr r3, .L_ja_0200c528
	ldrh r3, [r3]
	lsls r3, r3, #16
	cmp r3, #0
	bne .L_ja_0200c4ca
	ldrh r3, [r7, #32]
	movs r2, #89
	subs r3, #2
	str r3, [sp, #0]
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #20]
	movs r0, #0
	adds r2, r2, r5
	mov r10, r0
	mov r8, r2
.L_ja_0200c41a:
	ldr r3, [r5]
	cmp r3, #0
	beq .L_ja_0200c444
	mov r3, r8
	ldrb r2, [r3]
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	beq .L_ja_0200c444
	cmp r5, r7
	beq .L_ja_0200c444
	ldrh r3, [r5, #32]
	adds r0, r5, #0
	adds r0, #8
	subs r3, #2
	ldr r1, [sp, #0]
	add r2, sp, #28
	bl Func_02005538
	cmp r0, #0
	bge .L_ja_0200c4ca
.L_ja_0200c444:
	movs r0, #1
	add r10, r0
	movs r2, #128
	mov r3, r10
	add r8, r2
	adds r5, #128
	cmp r3, #63
	ble .L_ja_0200c41a
	ldr r0, [r6]
	movs r3, #128
	str r0, [sp, #20]
	lsls r3, r3, #10
	ldr r2, [r6, #8]
	adds r0, r7, #0
	str r2, [sp, #16]
	str r3, [r7, #48]
	movs r3, #200
	lsls r3, r3, #5
	adds r3, #153
	str r3, [r7, #52]
	movs r5, #0
	ldr r1, [r6]
	ldr r2, [r6, #4]
	ldr r3, [r6, #8]
	bl Func_020054b8
	b .L_ja_0200c482
.L_ja_0200c47a:
	movs r0, #1
	bl WaitFrames
	adds r5, #1
.L_ja_0200c482:
	cmp r5, #179
	bgt .L_ja_0200c49a
	adds r0, r7, #0
	bl Func_02005530
	cmp r0, #0
	bne .L_ja_0200c49a
	ldr r3, .L_ja_0200c528
	ldrh r3, [r3]
	lsls r3, r3, #16
	cmp r3, #0
	beq .L_ja_0200c47a
.L_ja_0200c49a:
	ldr r3, .L_ja_0200c528
	ldrh r3, [r3]
	lsls r3, r3, #16
	cmp r3, #0
	bne .L_ja_0200c4f6
	ldr r3, [sp, #24]
	cmp r11, r3
	bne .L_ja_0200c4f0
.L_ja_0200c4aa:
	movs r0, #128
	add r2, sp, #28
	lsls r0, r0, #13
	mov r1, r9
	bl Vector_AddPolarOffsetFar
	ldr r2, [sp, #4]
	add r6, sp, #28
	ldrb r0, [r2]
	ldr r1, [r6]
	ldr r2, [r6, #8]
	bl Func_020054d0
	mov r11, r0
	cmp r0, #255
	bne .L_ja_0200c3e2
.L_ja_0200c4ca:
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r7, #48]
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r7, #52]
	ldr r2, [r7, #12]
	adds r0, r7, #0
	ldr r1, [sp, #20]
	ldr r3, [sp, #16]
	bl Func_020054b8
	adds r0, r7, #0
	bl Func_020054c0
	movs r0, #2
	bl WaitFrames
	b .L_ja_0200c25e
.L_ja_0200c4f0:
	movs r0, #10
	bl WaitFrames
.L_ja_0200c4f6:
	movs r3, #0
	str r3, [r7, #108]
	adds r1, r7, #0
	adds r1, #90
	ldrb r2, [r1]
	movs r3, #1
	orrs r3, r2
	strb r3, [r1]
	movs r3, #128
	lsls r3, r3, #7
	str r3, [r7, #52]
	adds r0, r7, #0
	movs r1, #1
	bl Func_02005480
.L_ja_0200c514:
	add sp, #40
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_ja_0200c524:
	.4byte Func_02004080
.L_ja_0200c528:
	.4byte gData_020074c8
	.else
	.global Func_020043ec
	.thumb_func
Func_020043ec:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, .L_0200c44c
	movs r0, #133
	lsls r0, r0, #2
	adds r3, r3, r0
	ldr r0, [r3]
	sub sp, #40
	bl ObjectTable_Get
	ldr r2, .L_0200c450
	ldr r3, .L_0200c448
	adds r7, r0, #0
	strh r3, [r2]
.L_0200c412:
	bl Func_0200410c
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r7, #48]
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r7, #52]
	movs r3, #128
	lsls r3, r3, #24
	str r3, [r7, #56]
	str r3, [r7, #64]
	movs r3, #0
	str r3, [r7, #36]
	str r3, [r7, #44]
	ldr r1, .L_0200c454
	ldr r3, [r7, #8]
	add r2, sp, #28
	mov r10, r2
	movs r2, #128
	lsls r2, r2, #12
	ands r3, r1
	adds r3, r3, r2
	mov r0, r10
	str r3, [sp, #12]
	str r3, [r0]
	b .L_0200c458
.L_0200c448:
	.4byte 0x00000000
.L_0200c44c:
	.4byte gPartyState
.L_0200c450:
	.4byte gData_020074c8
.L_0200c454:
	.4byte 0xfff00000
.L_0200c458:
	ldr r3, [r7, #12]
	str r3, [r0, #4]
	ldr r3, [r7, #16]
	ands r3, r1
	adds r3, r3, r2
	str r3, [sp, #8]
	str r3, [r0, #8]
	ldr r2, [sp, #12]
	str r3, [sp, #16]
	adds r3, r7, #0
	adds r3, #34
	str r2, [sp, #20]
	str r3, [sp, #4]
	adds r1, r2, #0
	ldrb r0, [r3]
	ldr r2, [sp, #16]
	bl Func_020054d0
	str r0, [sp, #24]
	ldr r2, [sp, #20]
	ldr r1, [r7, #8]
	ldr r0, [sp, #16]
	ldr r3, .L_0200c4c4
	ldr r6, [r7, #16]
	subs r1, r2, r1
	subs r6, r0, r6
	mov r8, r3
	adds r0, r1, #0
	mov lr, r8
	.2byte 0xf800
	adds r1, r6, #0
	adds r5, r0, #0
	adds r0, r6, #0
	mov lr, r8
	.2byte 0xf800
	adds r5, r5, r0
	movs r0, #128
	lsls r0, r0, #11
	cmp r5, r0
	bge .L_0200c4d4
	ldr r3, .L_0200c4c8
	movs r2, #15
	ldr r3, [r3]
	ldr r1, .L_0200c4cc
	lsrs r3, r3, #4
	ands r3, r2
	lsls r3, r3, #1
	ldrsh r2, [r1, r3]
	mov r9, r2
	lsls r3, r2, #16
	ldr r2, .L_0200c4d0
	cmp r3, r2
	bne .L_0200c500
	b .L_0200c6ca
.L_0200c4c4:
	.4byte IwramMulQ16
.L_0200c4c8:
	.4byte gInput
.L_0200c4cc:
	.4byte gData_02005f38
.L_0200c4d0:
	.4byte 0xffff0000
.L_0200c4d4:
	ldr r3, [sp, #16]
	ldr r2, [sp, #20]
	ldr r0, [r7, #16]
	ldr r1, [r7, #8]
	subs r0, r3, r0
	subs r1, r2, r1
	bl ArcTan2
	lsls r0, r0, #16
	asrs r0, r0, #16
	movs r3, #128
	ldr r2, .L_0200c4fc
	mov r9, r0
	lsls r3, r3, #6
	add r3, r9
	ands r3, r2
	lsls r3, r3, #16
	asrs r3, r3, #16
	mov r9, r3
	b .L_0200c500
.L_0200c4fc:
	.4byte 0xffffc000
.L_0200c500:
	movs r0, #128
	add r2, sp, #28
	lsls r0, r0, #13
	mov r1, r9
	bl Vector_AddPolarOffsetFar
	ldr r3, [sp, #4]
	mov r2, r10
	ldr r1, [r2]
	ldrb r0, [r3]
	ldr r2, [r2, #8]
	bl Func_020054d0
	mov r11, r0
	cmp r0, #255
	beq .L_0200c57a
	ldr r3, [sp, #4]
	mov r2, r10
	ldrb r0, [r3]
	ldr r1, [r2]
	ldr r2, [r2, #8]
	bl Map_GetTerrainHeight
	ldr r3, [r7, #12]
	subs r0, r0, r3
	movs r3, #128
	lsls r3, r3, #12
	cmp r0, r3
	bgt .L_0200c57a
	ldr r0, [sp, #12]
	mov r2, r10
	str r0, [r2]
	ldr r3, [sp, #8]
	adds r0, r7, #0
	str r3, [r2, #8]
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r7, #48]
	movs r3, #200
	lsls r3, r3, #5
	adds r3, #153
	adds r2, r7, #0
	str r3, [r7, #52]
	adds r2, #100
	movs r3, #0
	strh r3, [r2]
	ldr r1, [sp, #12]
	ldr r2, [r7, #12]
	ldr r3, [sp, #8]
	bl Func_020054b8
	adds r0, r7, #0
	movs r1, #2
	bl Func_02005480
	adds r0, r7, #0
	movs r1, #48
	bl ObjectDispatch_SetAnimationSpeed
	movs r5, #0
	b .L_0200c5a2
.L_0200c57a:
	movs r3, #0
	mov r0, r9
	strh r0, [r7, #6]
	str r3, [r7, #36]
	str r3, [r7, #44]
	ldr r2, [sp, #12]
	str r2, [r7, #8]
	ldr r3, [sp, #8]
	str r3, [r7, #16]
	b .L_0200c6ca
.L_0200c58e:
	ldr r3, .L_0200c6f8
	ldrh r3, [r3]
	lsls r3, r3, #16
	cmp r3, #0
	beq .L_0200c59a
	b .L_0200c6ca
.L_0200c59a:
	movs r0, #1
	bl WaitFrames
	adds r5, #1
.L_0200c5a2:
	cmp r5, #179
	bgt .L_0200c5b0
	adds r0, r7, #0
	bl Func_02005530
	cmp r0, #0
	beq .L_0200c58e
.L_0200c5b0:
	ldr r3, .L_0200c6fc
	str r3, [r7, #108]
	b .L_0200c67e
.L_0200c5b6:
	ldr r2, [sp, #4]
	ldr r1, [r6]
	ldrb r0, [r2]
	ldr r2, [r6, #8]
	bl Map_GetTerrainHeight
	ldr r3, [r7, #12]
	subs r0, r0, r3
	movs r3, #128
	lsls r3, r3, #12
	cmp r0, r3
	bgt .L_0200c69e
	ldr r3, .L_0200c6f8
	ldrh r3, [r3]
	lsls r3, r3, #16
	cmp r3, #0
	bne .L_0200c6ca
	ldrh r3, [r7, #32]
	movs r2, #89
	subs r3, #2
	str r3, [sp, #0]
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #20]
	movs r0, #0
	adds r2, r2, r5
	mov r10, r0
	mov r8, r2
.L_0200c5ee:
	ldr r3, [r5]
	cmp r3, #0
	beq .L_0200c618
	mov r3, r8
	ldrb r2, [r3]
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	beq .L_0200c618
	cmp r5, r7
	beq .L_0200c618
	ldrh r3, [r5, #32]
	adds r0, r5, #0
	adds r0, #8
	subs r3, #2
	ldr r1, [sp, #0]
	add r2, sp, #28
	bl Func_02005538
	cmp r0, #0
	bge .L_0200c69e
.L_0200c618:
	movs r0, #1
	add r10, r0
	movs r2, #128
	mov r3, r10
	add r8, r2
	adds r5, #128
	cmp r3, #63
	ble .L_0200c5ee
	ldr r0, [r6]
	movs r3, #128
	str r0, [sp, #20]
	lsls r3, r3, #10
	ldr r2, [r6, #8]
	adds r0, r7, #0
	str r2, [sp, #16]
	str r3, [r7, #48]
	movs r3, #200
	lsls r3, r3, #5
	adds r3, #153
	str r3, [r7, #52]
	movs r5, #0
	ldr r1, [r6]
	ldr r2, [r6, #4]
	ldr r3, [r6, #8]
	bl Func_020054b8
	b .L_0200c656
.L_0200c64e:
	movs r0, #1
	bl WaitFrames
	adds r5, #1
.L_0200c656:
	cmp r5, #179
	bgt .L_0200c66e
	adds r0, r7, #0
	bl Func_02005530
	cmp r0, #0
	bne .L_0200c66e
	ldr r3, .L_0200c6f8
	ldrh r3, [r3]
	lsls r3, r3, #16
	cmp r3, #0
	beq .L_0200c64e
.L_0200c66e:
	ldr r3, .L_0200c6f8
	ldrh r3, [r3]
	lsls r3, r3, #16
	cmp r3, #0
	bne .L_0200c6ca
	ldr r3, [sp, #24]
	cmp r11, r3
	bne .L_0200c6c4
.L_0200c67e:
	movs r0, #128
	add r2, sp, #28
	lsls r0, r0, #13
	mov r1, r9
	bl Vector_AddPolarOffsetFar
	ldr r2, [sp, #4]
	add r6, sp, #28
	ldrb r0, [r2]
	ldr r1, [r6]
	ldr r2, [r6, #8]
	bl Func_020054d0
	mov r11, r0
	cmp r0, #255
	bne .L_0200c5b6
.L_0200c69e:
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r7, #48]
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r7, #52]
	ldr r2, [r7, #12]
	adds r0, r7, #0
	ldr r1, [sp, #20]
	ldr r3, [sp, #16]
	bl Func_020054b8
	adds r0, r7, #0
	bl Func_020054c0
	movs r0, #2
	bl WaitFrames
	b .L_0200c412
.L_0200c6c4:
	movs r0, #10
	bl WaitFrames
.L_0200c6ca:
	movs r3, #0
	str r3, [r7, #108]
	adds r1, r7, #0
	adds r1, #90
	ldrb r2, [r1]
	movs r3, #1
	orrs r3, r2
	strb r3, [r1]
	movs r3, #128
	lsls r3, r3, #7
	str r3, [r7, #52]
	adds r0, r7, #0
	movs r1, #1
	bl Func_02005480
	add sp, #40
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200c6f8:
	.4byte gData_020074c8
.L_0200c6fc:
	.4byte Func_02004080
	.endif
	.section .text.x0200c70c,"ax",%progbits
	.balign 4
	.global Func_0200470c
	.thumb_func
Func_0200470c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, .L_0200c778
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	ldr r0, [r3]
	sub sp, #32
	bl ObjectTable_Get
	adds r5, r0, #0
	ldrh r3, [r5, #6]
	movs r2, #128
	lsls r2, r2, #6
	adds r3, r3, r2
	ldr r2, .L_0200c774
	ands r3, r2
	lsls r3, r3, #16
	asrs r3, r3, #16
	str r3, [sp, #16]
.L_0200c73e:
	bl Func_0200410c
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r5, #48]
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r5, #52]
	movs r3, #128
	lsls r3, r3, #24
	str r3, [r5, #56]
	str r3, [r5, #64]
	movs r3, #0
	str r3, [r5, #36]
	str r3, [r5, #44]
	ldr r2, .L_0200c77c
	ldr r3, [r5, #8]
	movs r1, #128
	lsls r1, r1, #12
	ands r3, r2
	mov r9, r1
	add r6, sp, #20
	add r3, r9
	str r3, [r6]
	mov r8, r3
	b .L_0200c780
	.2byte 0x0000
.L_0200c774:
	.4byte 0xffffc000
.L_0200c778:
	.4byte gPartyState
.L_0200c77c:
	.4byte 0xfff00000
.L_0200c780:
	ldr r3, [r5, #12]
	str r3, [r6, #4]
	ldr r3, [r5, #16]
	ands r3, r2
	adds r7, r3, r1
	mov r2, r8
	str r7, [r6, #8]
	str r2, [sp, #8]
	str r7, [sp, #4]
	movs r3, #34
	adds r3, r3, r5
	ldrb r0, [r3]
	adds r1, r2, #0
	adds r2, r7, #0
	mov r11, r3
	bl Func_020054d0
	str r0, [sp, #12]
	movs r0, #128
	ldr r1, [sp, #16]
	lsls r0, r0, #13
	adds r2, r6, #0
	bl Vector_AddPolarOffsetFar
	mov r1, r11
	ldrb r0, [r1]
	ldr r2, [r6, #8]
	ldr r1, [r6]
	bl Func_020054d0
	mov r10, r0
	cmp r0, #255
	beq .L_0200c814
	mov r2, r11
	ldrb r0, [r2]
	ldr r1, [r6]
	ldr r2, [r6, #8]
	bl Map_GetTerrainHeight
	ldr r3, [r5, #12]
	subs r0, r0, r3
	cmp r0, r9
	bgt .L_0200c814
	ldr r3, [sp, #8]
	ldr r2, .L_0200c80c
	str r3, [r6]
	ldr r1, [sp, #4]
	movs r3, #128
	lsls r3, r3, #10
	str r1, [r6, #8]
	str r3, [r5, #48]
	movs r3, #200
	lsls r3, r3, #5
	adds r3, #153
	str r3, [r5, #52]
	adds r3, r5, #0
	adds r3, #100
	strh r2, [r3]
	adds r0, r5, #0
	movs r1, #2
	bl Func_02005480
	adds r0, r5, #0
	movs r1, #48
	bl ObjectDispatch_SetAnimationSpeed
	ldr r3, .L_0200c810
	str r3, [r5, #108]
	b .L_0200c8be
	.2byte 0x0000
.L_0200c80c:
	.4byte 0x00000000
.L_0200c810:
	.4byte Func_02004080
.L_0200c814:
	add r1, sp, #16
	ldrh r1, [r1]
	movs r3, #0
	mov r2, r8
	strh r1, [r5, #6]
	str r3, [r5, #36]
	str r3, [r5, #44]
	str r2, [r5, #8]
	str r7, [r5, #16]
	b .L_0200c90a
.L_0200c828:
	mov r3, r11
	ldrb r0, [r3]
	ldr r1, [r7]
	ldr r2, [r7, #8]
	bl Map_GetTerrainHeight
	ldr r3, [r5, #12]
	movs r1, #128
	subs r0, r0, r3
	lsls r1, r1, #12
	cmp r0, r1
	bgt .L_0200c8de
	ldrh r3, [r5, #32]
	movs r2, #0
	subs r3, #2
	str r3, [sp, #0]
	movs r3, #192
	lsls r3, r3, #18
	ldr r6, [r3, #20]
	movs r3, #89
	adds r3, r3, r6
	mov r9, r2
	mov r8, r3
.L_0200c856:
	ldr r3, [r6]
	cmp r3, #0
	beq .L_0200c880
	mov r1, r8
	ldrb r2, [r1]
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	beq .L_0200c880
	cmp r6, r5
	beq .L_0200c880
	ldrh r3, [r6, #32]
	adds r0, r6, #0
	adds r0, #8
	subs r3, #2
	ldr r1, [sp, #0]
	add r2, sp, #20
	bl Func_02005538
	cmp r0, #0
	bge .L_0200c8de
.L_0200c880:
	movs r2, #1
	add r9, r2
	movs r3, #128
	mov r1, r9
	add r8, r3
	adds r6, #128
	cmp r1, #63
	ble .L_0200c856
	ldr r2, [r7]
	adds r0, r5, #0
	str r2, [sp, #8]
	ldr r3, [r7, #8]
	str r3, [sp, #4]
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r5, #48]
	movs r3, #200
	lsls r3, r3, #5
	adds r3, #153
	str r3, [r5, #52]
	ldr r1, [r7]
	ldr r2, [r7, #4]
	ldr r3, [r7, #8]
	bl Func_020054b8
	adds r0, r5, #0
	bl Func_020054c0
	ldr r1, [sp, #12]
	cmp r10, r1
	bne .L_0200c904
.L_0200c8be:
	movs r0, #128
	ldr r1, [sp, #16]
	add r2, sp, #20
	lsls r0, r0, #13
	bl Vector_AddPolarOffsetFar
	mov r2, r11
	add r7, sp, #20
	ldrb r0, [r2]
	ldr r1, [r7]
	ldr r2, [r7, #8]
	bl Func_020054d0
	mov r10, r0
	cmp r0, #255
	bne .L_0200c828
.L_0200c8de:
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r5, #48]
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r5, #52]
	ldr r2, [r5, #12]
	adds r0, r5, #0
	ldr r1, [sp, #8]
	ldr r3, [sp, #4]
	bl Func_020054b8
	adds r0, r5, #0
	bl Func_020054c0
	movs r0, #2
	bl WaitFrames
	b .L_0200c73e
.L_0200c904:
	movs r0, #10
	bl WaitFrames
.L_0200c90a:
	movs r3, #0
	str r3, [r5, #108]
	adds r1, r5, #0
	adds r1, #90
	ldrb r2, [r1]
	movs r3, #1
	orrs r3, r2
	strb r3, [r1]
	movs r3, #128
	lsls r3, r3, #7
	str r3, [r5, #52]
	adds r0, r5, #0
	movs r1, #1
	bl Func_02005480
	add sp, #32
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x0200c938,"ax",%progbits
	.global Func_02004938
	.thumb_func
Func_02004938:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	sub sp, #8
	mov r8, r3
	bl Object_GetById
	ldr r3, [r0, #80]
	ldr r5, .L_0200ca3c
	ldr r3, [r3, #40]
	movs r1, #0
	movs r2, #0
	ldrsh r3, [r3, r2]
	lsls r4, r3, #16
	ldrh r3, [r5, r1]
	lsrs r2, r4, #16
	cmp r2, r3
	beq .L_0200c97e
.L_0200c964:
	movs r2, #128
	lsls r3, r1, #16
	lsls r2, r2, #9
	adds r3, r3, r2
	lsrs r2, r3, #16
	asrs r1, r3, #16
	cmp r2, #5
	bhi .L_0200c97e
	lsls r3, r2, #1
	ldrh r3, [r5, r3]
	lsrs r2, r4, #16
	cmp r2, r3
	bne .L_0200c964
.L_0200c97e:
	lsls r3, r1, #16
	lsrs r2, r3, #16
	cmp r2, #6
	bne .L_0200c98a
	movs r0, #0
	b .L_0200ca32
.L_0200c98a:
	ldr r6, .L_0200ca40
	lsls r2, r2, #2
	ldrsb r4, [r6, r2]
	adds r1, r4, #0
	cmp r4, #0
	bge .L_0200c998
	negs r1, r4
.L_0200c998:
	adds r3, r2, #2
	ldrsb r3, [r6, r3]
	cmp r3, #0
	bge .L_0200c9a2
	negs r3, r3
.L_0200c9a2:
	adds r3, r1, r3
	asrs r7, r3, #4
	adds r3, r2, #1
	ldrsb r1, [r6, r3]
	adds r5, r1, #0
	cmp r1, #0
	bge .L_0200c9b2
	negs r5, r1
.L_0200c9b2:
	adds r3, r2, #3
	ldrsb r2, [r6, r3]
	cmp r2, #0
	bge .L_0200c9bc
	negs r2, r2
.L_0200c9bc:
	adds r5, r5, r2
	mov r10, r5
	ldr r6, [r0, #8]
	mov r3, r10
	ldr r5, [r0, #16]
	asrs r3, r3, #4
	mov r10, r3
	lsls r3, r4, #16
	adds r6, r6, r3
	lsls r3, r1, #16
	adds r5, r5, r3
	movs r3, #164
	lsls r3, r3, #1
	add r3, r8
	ldr r3, [r3]
	asrs r6, r6, #20
	asrs r1, r3, #20
	movs r3, #166
	lsls r3, r3, #1
	add r3, r8
	ldr r3, [r3]
	lsls r2, r1, #16
	asrs r3, r3, #20
	lsls r3, r3, #16
	asrs r5, r5, #20
	lsrs r2, r2, #16
	lsrs r3, r3, #16
	adds r2, r6, r2
	adds r3, r5, r3
	str r2, [sp, #0]
	str r3, [sp, #4]
	adds r0, r6, #0
	adds r1, r5, #0
	adds r2, r7, #0
	mov r3, r10
	bl Func_020054e8
	movs r3, #255
	mov r2, r10
	str r2, [sp, #0]
	str r3, [sp, #4]
	mov r8, r3
	movs r0, #0
	adds r1, r6, #0
	adds r2, r5, #0
	adds r3, r7, #0
	bl Func_02004a44
	mov r2, r10
	mov r3, r8
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r0, #2
	adds r1, r6, #0
	adds r2, r5, #0
	adds r3, r7, #0
	bl Func_02004a44
	movs r0, #1
.L_0200ca32:
	add sp, #8
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_0200ca3c:
	.4byte gData_02005f58
.L_0200ca40:
	.4byte gData_02005f64
	.section .text.x0200ca8c,"ax",%progbits
	.balign 4
	.global Func_02004a8c
	.thumb_func
Func_02004a8c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	adds r6, r0, #0
	adds r5, r6, #0
	sub sp, #40
	adds r1, r6, #0
	adds r5, #12
	add r0, sp, #24
	adds r1, #16
	adds r2, r5, #0
	bl Func_02004bf4
	adds r4, r0, #0
	cmp r4, #0
	bne .L_0200cab6
	b .L_0200cbd6
.L_0200cab6:
	ldr r5, [r5]
	ldr r0, .L_0200cbe8
	str r5, [sp, #20]
	lsls r1, r5, #2
	ldrsb r2, [r0, r1]
	cmp r2, #0
	bge .L_0200cac6
	negs r2, r2
.L_0200cac6:
	adds r3, r1, #2
	ldrsb r3, [r0, r3]
	cmp r3, #0
	bge .L_0200cad0
	negs r3, r3
.L_0200cad0:
	adds r3, r2, r3
	asrs r3, r3, #4
	str r3, [sp, #16]
	adds r3, r1, #1
	ldrsb r2, [r0, r3]
	cmp r2, #0
	bge .L_0200cae0
	negs r2, r2
.L_0200cae0:
	adds r3, r1, #3
	ldrsb r3, [r0, r3]
	cmp r3, #0
	bge .L_0200caea
	negs r3, r3
.L_0200caea:
	adds r3, r2, r3
	asrs r3, r3, #4
	str r3, [sp, #12]
	ldr r3, [sp, #24]
	ldr r2, .L_0200cbec
	ldr r1, .L_0200cbf0
	lsls r3, r3, #2
	ldr r3, [r2, r3]
	mov r9, r1
	mov r2, r9
	ands r2, r3
	lsls r3, r3, #16
	mov r10, r3
	movs r3, #0
	str r3, [r6, #20]
	mov r11, r3
	adds r3, r4, #0
	adds r3, #34
	str r3, [sp, #8]
	ldr r1, [sp, #8]
	movs r3, #2
	strb r3, [r1]
	mov r9, r2
	ldr r3, [r4, #8]
	add r3, r9
	str r3, [r6]
	ldr r3, [r4, #16]
	add r3, r10
	str r3, [r6, #8]
	ldr r3, [r4, #12]
	str r3, [sp, #32]
.L_0200cb28:
	ldr r3, [sp, #20]
	ldr r2, .L_0200cbe8
	lsls r3, r3, #2
	str r3, [sp, #4]
	adds r3, #1
	ldrsb r2, [r2, r3]
	ldr r3, [r6, #8]
	lsls r2, r2, #16
	adds r3, r3, r2
	ldr r2, [sp, #12]
	movs r1, #0
	mov r8, r1
	str r3, [sp, #36]
	cmp r8, r2
	bge .L_0200cb96
.L_0200cb46:
	ldr r3, .L_0200cbe8
	ldr r1, [sp, #4]
	add r5, sp, #28
	ldrsb r2, [r3, r1]
	ldr r3, [r6]
	lsls r2, r2, #16
	adds r3, r3, r2
	str r3, [r5]
	ldr r2, [sp, #16]
	movs r7, #0
	cmp r7, r2
	bge .L_0200cb80
.L_0200cb5e:
	adds r0, r4, #0
	add r1, sp, #28
	str r4, [sp, #0]
	bl Func_020054f8
	ldr r4, [sp, #0]
	cmp r0, #2
	beq .L_0200cba8
	ldr r3, [r5]
	movs r1, #128
	lsls r1, r1, #13
	adds r3, r3, r1
	str r3, [r5]
	ldr r2, [sp, #16]
	adds r7, #1
	cmp r7, r2
	blt .L_0200cb5e
.L_0200cb80:
	add r2, sp, #28
	ldr r3, [r2, #8]
	movs r1, #128
	lsls r1, r1, #13
	adds r3, r3, r1
	str r3, [r2, #8]
	ldr r3, [sp, #12]
	movs r2, #1
	add r8, r2
	cmp r8, r3
	blt .L_0200cb46
.L_0200cb96:
	ldr r3, [r6]
	movs r1, #1
	add r3, r9
	str r3, [r6]
	ldr r3, [r6, #8]
	add r11, r1
	add r3, r10
	str r3, [r6, #8]
	b .L_0200cb28
.L_0200cba8:
	ldr r2, [sp, #8]
	movs r3, #0
	strb r3, [r2]
	mov r3, r11
	movs r0, #0
	cmp r3, #0
	beq .L_0200cbd8
	mov r1, r9
	ldr r3, [r4, #8]
	mov r2, r11
	muls r2, r1
	adds r3, r3, r2
	str r3, [r6]
	movs r0, #1
	ldr r3, [r4, #12]
	str r3, [r6, #4]
	mov r3, r10
	mov r2, r11
	muls r2, r3
	ldr r3, [r4, #16]
	adds r3, r3, r2
	str r3, [r6, #8]
	b .L_0200cbd8
.L_0200cbd6:
	movs r0, #0
.L_0200cbd8:
	add sp, #40
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200cbe8:
	.4byte gData_02005f64
.L_0200cbec:
	.4byte gData_02005f7c
.L_0200cbf0:
	.4byte 0xffff0000
	.section .text.x0200cbf4,"ax",%progbits
	.global Func_02004bf4
	.thumb_func
Func_02004bf4:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #12
	str r0, [sp, #8]
	str r1, [sp, #4]
	str r2, [sp, #0]
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #108]
	ldr r3, .L_0200cd00
	movs r0, #133
	lsls r0, r0, #2
	adds r3, r3, r0
	ldr r0, [r3]
	bl Object_GetById
	adds r7, r0, #0
	ldrh r3, [r7, #6]
	ldr r1, [sp, #8]
	lsrs r3, r3, #12
	str r3, [r1]
	movs r2, #8
	adds r5, #52
	mov r11, r2
	mov lr, r5
.L_0200cc30:
	mov r3, lr
	ldr r6, [r3]
	movs r5, #0
.L_0200cc36:
	ldr r3, [r6, #80]
	ldr r2, .L_0200cd04
	ldr r3, [r3, #40]
	movs r0, #0
	ldrsh r1, [r3, r0]
	lsls r3, r5, #1
	ldrh r3, [r2, r3]
	cmp r1, r3
	bne .L_0200ccda
	ldr r0, [sp, #8]
	movs r2, #10
	ldrsh r1, [r7, r2]
	ldr r3, [r0]
	ldr r2, .L_0200cd08
	lsls r3, r3, #2
	ldr r3, [r2, r3]
	ldr r4, .L_0200cd0c
	asrs r2, r3, #16
	adds r1, r1, r2
	asrs r1, r1, #4
	mov r9, r1
	movs r1, #18
	ldrsh r2, [r7, r1]
	lsls r3, r3, #16
	asrs r3, r3, #16
	adds r2, r2, r3
	asrs r2, r2, #4
	mov r8, r2
	movs r2, #10
	ldrsh r0, [r6, r2]
	lsls r2, r5, #2
	ldrsb r3, [r4, r2]
	adds r3, r0, r3
	asrs r3, r3, #4
	mov r10, r3
	movs r3, #18
	ldrsh r1, [r6, r3]
	adds r3, r2, #1
	ldrsb r3, [r4, r3]
	adds r3, r1, r3
	asrs r3, r3, #4
	mov r12, r3
	adds r3, r2, #2
	ldrsb r3, [r4, r3]
	adds r2, #3
	adds r0, r0, r3
	ldrsb r3, [r4, r2]
	asrs r0, r0, #4
	adds r1, r1, r3
	asrs r1, r1, #4
	cmp r10, r9
	bgt .L_0200ccda
	cmp r9, r0
	bge .L_0200ccda
	cmp r12, r8
	bgt .L_0200ccda
	cmp r8, r1
	bge .L_0200ccda
	ldr r0, [sp, #0]
	movs r3, #1
	ands r3, r5
	str r5, [r0]
	cmp r3, #0
	beq .L_0200ccc8
	ldr r3, [r7, #8]
	asrs r3, r3, #20
	cmp r10, r3
	beq .L_0200ccda
	ldr r2, [sp, #4]
	mov r1, r11
	str r1, [r2]
	adds r0, r6, #0
	b .L_0200ccf0
.L_0200ccc8:
	ldr r3, [r7, #16]
	asrs r3, r3, #20
	cmp r12, r3
	beq .L_0200ccda
	ldr r0, [sp, #4]
	mov r3, r11
	str r3, [r0]
	adds r0, r6, #0
	b .L_0200ccf0
.L_0200ccda:
	adds r5, #1
	cmp r5, #5
	bls .L_0200cc36
	movs r2, #1
	add r11, r2
	movs r1, #4
	mov r3, r11
	add lr, r1
	cmp r3, #63
	bls .L_0200cc30
	movs r0, #0
.L_0200ccf0:
	add sp, #12
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200cd00:
	.4byte gPartyState
.L_0200cd04:
	.4byte gData_02005f58
.L_0200cd08:
	.4byte gData_02005f7c
.L_0200cd0c:
	.4byte gData_02005f64
	.section .text.x0200cd10,"ax",%progbits
	.global Func_02004d10
	.thumb_func
Func_02004d10:
	sub sp, #16
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #56
	str r0, [sp, #88]
	str r1, [sp, #92]
	str r2, [sp, #96]
	str r3, [sp, #100]
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	movs r0, #133
	str r3, [sp, #28]
	ldr r3, .L_0200cfb0
	lsls r0, r0, #2
	adds r0, r0, r3
	mov r10, r0
	ldr r0, [r0]
	bl Object_GetById
	mov r8, r0
	ldr r0, [sp, #104]
	bl Object_GetById
	mov r3, r8
	ldr r3, [r3, #48]
	mov r4, r8
	str r3, [sp, #20]
	adds r6, r0, #0
	ldr r4, [r4, #52]
	mov r0, sp
	adds r0, #32
	str r0, [sp, #12]
	str r4, [sp, #16]
	ldr r2, [sp, #100]
	ldr r3, [r6, #8]
	movs r1, #0
	str r3, [r0]
	mov r9, r1
	ldr r3, [r6, #16]
	mov r1, sp
	adds r1, #44
	str r3, [r0, #8]
	ldr r5, .L_0200cfb4
	str r1, [sp, #8]
	lsls r7, r2, #2
	ldrsb r1, [r5, r7]
	ldr r3, [r6, #8]
	lsls r2, r1, #16
	adds r3, r3, r2
	ldr r2, [sp, #8]
	asrs r3, r3, #20
	str r3, [r2]
	mov lr, r3
	adds r3, r7, #1
	ldrsb r4, [r5, r3]
	ldr r3, [r6, #16]
	ldr r0, [sp, #8]
	lsls r2, r4, #16
	adds r3, r3, r2
	asrs r3, r3, #20
	str r3, [r0, #8]
	adds r0, r1, #0
	mov r12, r3
	cmp r0, #0
	bge .L_0200cda0
	negs r0, r0
.L_0200cda0:
	adds r3, r7, #2
	ldrsb r1, [r5, r3]
	cmp r1, #0
	bge .L_0200cdaa
	negs r1, r1
.L_0200cdaa:
	adds r3, r0, r1
	asrs r3, r3, #4
	adds r1, r4, #0
	str r3, [sp, #24]
	cmp r1, #0
	bge .L_0200cdb8
	negs r1, r1
.L_0200cdb8:
	adds r3, r7, #3
	ldrsb r2, [r5, r3]
	cmp r2, #0
	bge .L_0200cdc2
	negs r2, r2
.L_0200cdc2:
	adds r3, r1, r2
	asrs r3, r3, #4
	str r3, [sp, #0]
	mov r11, r3
	movs r3, #0
	str r3, [sp, #4]
	mov r1, lr
	mov r2, r12
	ldr r3, [sp, #24]
	movs r0, #0
	bl Func_02004a44
	mov r1, r10
	movs r2, #200
	ldr r0, [r1]
	lsls r2, r2, #5
	movs r1, #128
	lsls r1, r1, #8
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	mov r2, r10
	ldr r0, [r2]
	movs r1, #8
	bl Object_SetModeById
	movs r0, #15
	bl WaitFrames
	ldr r4, [sp, #12]
	ldr r1, [sp, #88]
	ldr r3, [r4]
	ldr r2, [sp, #96]
	subs r1, r1, r3
	ldr r3, [r4, #8]
	asrs r1, r1, #17
	subs r2, r2, r3
	mov r3, r10
	asrs r2, r2, #17
	ldr r0, [r3]
	bl ObjectMotion_OffsetPositionAndResetMotion
	mov r4, r10
	ldr r0, [r4]
	bl Object_GetById
	ldr r3, .L_0200cfb8
	str r3, [r0, #108]
	movs r0, #4
	bl WaitFrames
	movs r1, #2
	adds r0, r6, #0
	bl Func_02005480
	movs r0, #239
	bl Func_020056c0
	movs r2, #200
	movs r1, #128
	lsls r2, r2, #5
	ldr r0, [sp, #104]
	lsls r1, r1, #8
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	adds r0, r6, #0
	ldr r1, [sp, #88]
	ldr r2, [sp, #92]
	ldr r3, [sp, #96]
	bl Func_020054b8
	ldr r3, .L_0200cfb0
	movs r0, #133
	lsls r0, r0, #2
	adds r5, r3, r0
	ldr r0, [r5]
	bl ObjectMotion_CommitCurrentPositionAndActivate
	ldr r0, [r5]
	movs r1, #2
	bl Object_SetModeById
	movs r1, #152
	movs r2, #200
	lsls r1, r1, #7
	lsls r2, r2, #5
	ldr r0, [r5]
	adds r1, #204
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	ldr r2, .L_0200cfbc
	mov r1, r9
	lsls r3, r1, #2
	ldr r2, [r2, r3]
	ldr r0, [r5]
	lsls r2, r2, #16
	asrs r1, r2, #31
	asrs r2, r2, #17
	bl ObjectMotion_OffsetPositionAndResetMotion
	ldr r3, [sp, #108]
	cmp r3, #0
	beq .L_0200ce98
	mov lr, r3
	.2byte 0xf800
.L_0200ce98:
	ldr r0, [r5]
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r1, #1
	ldr r0, [r5]
	bl Object_SetModeById
	mov r3, r8
	movs r2, #0
	str r2, [r3, #108]
	ldr r4, [sp, #20]
	movs r5, #255
	str r4, [r3, #48]
	ldr r0, [sp, #16]
	str r0, [r3, #52]
	adds r0, r6, #0
	bl Func_020054c0
	movs r0, #149
	lsls r0, r0, #1
	bl Func_020056c0
	movs r0, #213
	bl Func_020056c0
	ldr r2, [r6, #12]
	ldr r1, [sp, #88]
	ldr r3, [sp, #96]
	adds r0, r6, #0
	bl Object_SetPositionAndResetMotion
	adds r0, r6, #0
	movs r1, #1
	bl Func_02005480
	ldr r1, .L_0200cfb4
	ldr r0, [sp, #88]
	ldrsb r3, [r1, r7]
	adds r2, r7, #1
	lsls r3, r3, #16
	adds r0, r0, r3
	ldrsb r3, [r1, r2]
	mov r10, r1
	ldr r1, [sp, #96]
	lsls r3, r3, #16
	adds r1, r1, r3
	ldr r4, [sp, #28]
	asrs r0, r0, #20
	asrs r1, r1, #20
	str r0, [sp, #88]
	str r1, [sp, #96]
	mov r9, r2
	movs r2, #164
	lsls r2, r2, #1
	adds r3, r4, r2
	ldr r3, [r3]
	adds r2, #4
	asrs r3, r3, #20
	mov r8, r3
	adds r3, r4, r2
	ldr r6, [r3]
	mov r4, r8
	asrs r6, r6, #20
	adds r3, r4, r0
	adds r2, r6, r1
	str r3, [sp, #0]
	str r2, [sp, #4]
	mov r3, r11
	ldr r2, [sp, #24]
	bl Func_020054e8
	mov r0, r11
	ldr r1, [sp, #88]
	ldr r2, [sp, #96]
	str r0, [sp, #0]
	ldr r3, [sp, #24]
	movs r0, #0
	str r5, [sp, #4]
	bl Func_02004a44
	mov r3, r11
	ldr r1, [sp, #88]
	ldr r2, [sp, #96]
	str r3, [sp, #0]
	movs r0, #2
	ldr r3, [sp, #24]
	str r5, [sp, #4]
	bl Func_02004a44
	ldr r0, [sp, #12]
	mov r4, r10
	ldrsb r3, [r4, r7]
	ldr r1, [r0]
	ldr r2, [sp, #8]
	lsls r3, r3, #16
	adds r1, r1, r3
	asrs r1, r1, #20
	str r1, [r2]
	mov r3, r9
	ldrsb r2, [r4, r3]
	ldr r3, [r0, #8]
	ldr r4, [sp, #8]
	lsls r2, r2, #16
	adds r3, r3, r2
	asrs r3, r3, #20
	str r3, [r4, #8]
	add r8, r1
	adds r6, r6, r3
	str r1, [sp, #0]
	str r3, [sp, #4]
	ldr r2, [sp, #24]
	mov r0, r8
	adds r1, r6, #0
	mov r3, r11
	bl Func_020054e8
	ldr r0, [sp, #8]
	mov r3, r11
	ldr r1, [r0]
	ldr r2, [r0, #8]
	movs r4, #0
	str r3, [sp, #0]
	movs r0, #2
	ldr r3, [sp, #24]
	str r4, [sp, #4]
	bl Func_02004a44
	bl Func_02005668
	add sp, #56
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r3}
	add sp, #16
	bx r3
	.2byte 0x0000
.L_0200cfb0:
	.4byte gPartyState
.L_0200cfb4:
	.4byte gData_02005f64
.L_0200cfb8:
	.4byte Func_02004fc0
.L_0200cfbc:
	.4byte gData_02005f7c
	.section .text.x0200cfc0,"ax",%progbits
	.global Func_02004fc0
	.thumb_func
Func_02004fc0:
	push {r5, r6, lr}
	adds r5, r0, #0
	ldrh r3, [r5, #6]
	movs r2, #12
	lsrs r1, r3, #12
	adds r3, r1, #2
	ands r3, r2
	lsls r1, r3, #12
	ldr r3, [r5, #8]
	sub sp, #12
	mov r6, sp
	str r3, [r6]
	ldr r3, [r5, #12]
	movs r0, #128
	str r3, [r6, #4]
	ldr r3, [r5, #16]
	lsls r0, r0, #13
	adds r2, r6, #0
	str r3, [r6, #8]
	bl Vector_AddPolarOffsetFar
	adds r0, r6, #0
	movs r1, #1
	bl Func_02005680
	cmp r0, #0
	beq .L_0200d01c
	movs r4, #0
.L_0200cff8:
	ldr r3, [r0, #80]
	ldr r3, [r3, #40]
	movs r2, #0
	ldrsh r1, [r3, r2]
	ldr r2, .L_0200d044
	lsls r3, r4, #1
	ldrh r3, [r2, r3]
	cmp r1, r3
	beq .L_0200d040
	adds r4, #1
	cmp r4, #5
	bls .L_0200cff8
	ldr r1, [r5, #8]
	ldr r2, [r5, #12]
	ldr r3, [r5, #16]
	adds r0, r5, #0
	bl Object_SetPositionAndResetMotion
.L_0200d01c:
	ldr r3, [r5, #8]
	adds r0, r5, #0
	str r3, [r6]
	ldr r3, [r5, #12]
	adds r1, r6, #0
	str r3, [r6, #4]
	ldr r3, [r5, #16]
	str r3, [r6, #8]
	bl Func_020054f8
	cmp r0, #0
	ble .L_0200d040
	ldr r1, [r5, #8]
	ldr r2, [r5, #12]
	ldr r3, [r5, #16]
	adds r0, r5, #0
	bl Object_SetPositionAndResetMotion
.L_0200d040:
	add sp, #12
	pop {r5, r6, pc}
.L_0200d044:
	.4byte gData_02005f58
	.section .text.x0200d048,"ax",%progbits
	.balign 4
	.global Func_02005048
	.thumb_func
Func_02005048:
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
	ldr r3, .L_0200d1dc
	adds r2, r1, #0
	adds r2, #228
	ldr r0, [r2]
	ldr r2, [r2, #4]
	ands r0, r3
	ands r2, r3
	ldr r3, .L_0200d1e0
	mov r10, r0
	movs r0, #0
	ldrsh r3, [r3, r0]
	mov r8, r2
	ldr r2, .L_0200d1e4
	lsls r3, r3, #2
	adds r3, r3, r2
	ldrh r3, [r3, #2]
	sub sp, #8
	lsrs r3, r3, #5
	str r3, [sp, #4]
	ldr r6, .L_0200d1e8
	ldr r3, [r1]
	movs r1, #0
	ldr r3, [r3, #4]
	mov r9, r1
	str r3, [sp, #0]
	ldr r3, .L_0200d1ec
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r9, r3
	blt .L_0200d09a
	b .L_0200d1ce
.L_0200d09a:
	ldr r2, .L_0200d1f0
	mov r0, r9
	lsls r3, r0, #2
	ldr r5, [r2, r3]
	cmp r5, #0
	bne .L_0200d0a8
	b .L_0200d1be
.L_0200d0a8:
	ldr r3, [r5, #8]
	cmp r3, #0
	bne .L_0200d0b0
	b .L_0200d1be
.L_0200d0b0:
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
	ldr r3, .L_0200d1f4
	movs r1, #0
	ldrsh r2, [r3, r1]
	adds r3, r5, #0
	mov r12, r2
	asrs r1, r0, #16
	mov r0, r12
	adds r3, #100
	asrs r2, r4, #16
	cmp r0, #0
	bne .L_0200d126
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
	bhi .L_0200d1be
	movs r2, #16
	negs r2, r2
	cmp r4, r2
	ble .L_0200d1be
	cmp r4, #239
	bgt .L_0200d1be
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
	ldr r3, .L_0200d1f8
	b .L_0200d162
.L_0200d126:
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
	bhi .L_0200d1be
	movs r2, #64
	negs r2, r2
	cmp r4, r2
	ble .L_0200d1be
	cmp r4, #175
	bgt .L_0200d1be
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
	ldr r3, .L_0200d1fc
.L_0200d162:
	movs r2, #128
	orrs r4, r3
	stmia r1!, {r4}
	ldr r0, [sp, #4]
	lsls r3, r7, #3
	adds r3, r0, r3
	lsls r2, r2, #4
	orrs r3, r2
	str r3, [r1]
	ldr r3, .L_0200d200
	movs r0, #1
	ldrh r2, [r3]
	movs r1, #0
	ldrsh r3, [r3, r1]
	negs r0, r0
	cmp r3, r0
	bne .L_0200d1a0
	adds r0, r5, #0
	bl Func_02005698
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
	b .L_0200d1b4
.L_0200d1a0:
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
.L_0200d1b4:
	adds r0, r6, #0
	mov r1, r11
	bl Func_02005440
	adds r6, #12
.L_0200d1be:
	ldr r3, .L_0200d1ec
	movs r1, #1
	movs r2, #0
	ldrsh r3, [r3, r2]
	add r9, r1
	cmp r9, r3
	bge .L_0200d1ce
	b .L_0200d09a
.L_0200d1ce:
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0200d1dc:
	.4byte 0xffff0000
.L_0200d1e0:
	.4byte gData_020074cc
.L_0200d1e4:
	.4byte gResourceTableEntries
.L_0200d1e8:
	.4byte Data_0200f510
.L_0200d1ec:
	.4byte gData_020074ce
.L_0200d1f0:
	.4byte gData_020074d0
.L_0200d1f4:
	.4byte gData_020075d0
.L_0200d1f8:
	.4byte 0x40002000
.L_0200d1fc:
	.4byte 0xc000a000
.L_0200d200:
	.4byte gData_020075d2
	.section .bss,"aw",%nobits
	.global Data_02007064
Data_02007064:
	.space 0x00000004
	.global gData_02007068
gData_02007068:
	.space 0x00000020
	.global gData_02007088
gData_02007088:
	.space 0x00000004
	.global gData_0200708c
gData_0200708c:
	.space 0x0000001c
	.global gData_020070a8
gData_020070a8:
	.space 0x00000420
	.global gData_020074c8
gData_020074c8:
	.space 0x00000004
	.global gData_020074cc
gData_020074cc:
	.space 0x00000002
	.global gData_020074ce
gData_020074ce:
	.space 0x00000002
	.global gData_020074d0
gData_020074d0:
	.space 0x00000040
	.global Data_0200f510
Data_0200f510:
	.space 0x000000c0
	.global gData_020075d0
gData_020075d0:
	.space 0x00000002
	.global gData_020075d2
gData_020075d2:
