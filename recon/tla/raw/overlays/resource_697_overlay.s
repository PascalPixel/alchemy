.syntax unified
	.thumb
	.section .text.x02008038,"ax",%progbits
	.global Func_02000038
	.thumb_func
Func_02000038:
	push {lr}
	movs r0, #8
	movs r1, #75
	bl Func_020059a0
	pop {pc}
	.section .text.x02008058,"ax",%progbits
	.global Func_02000058
	.thumb_func
Func_02000058:
	push {r5, lr}
	adds r5, r0, #0
	ldr r2, [r5, #16]
	ldr r1, [r5, #8]
	movs r0, #0
	bl Map_GetTerrainHeight
	movs r3, #128
	lsls r3, r3, #10
	adds r2, r5, #0
	adds r0, r0, r3
	adds r2, #85
	movs r3, #0
	str r0, [r5, #20]
	str r0, [r5, #12]
	strb r3, [r2]
	adds r2, #4
	strb r3, [r2]
	movs r3, #192
	lsls r3, r3, #9
	str r3, [r5, #28]
	movs r0, #0
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x02008088,"ax",%progbits
	.global Func_02000088
	.thumb_func
Func_02000088:
	push {lr}
	ldr r3, .L_020080d0
	movs r2, #240
	movs r0, #16
	ldrsh r1, [r3, r0]
	ldr r3, .L_020080d4
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r0, #0
	ldrsh r2, [r3, r0]
	ldr r3, .L_020080d8
	cmp r2, r3
	bne .L_020080ae
	movs r3, #7
	ldr r2, .L_020080dc
	ands r1, r3
	lsls r3, r1, #2
	ldr r0, [r2, r3]
	b .L_020080ce
.L_020080ae:
	ldr r3, .L_020080e0
	cmp r2, r3
	bne .L_020080b8
	ldr r0, .L_020080e4
	b .L_020080ce
.L_020080b8:
	ldr r3, .L_020080e8
	cmp r2, r3
	bne .L_020080c2
	ldr r0, .L_020080ec
	b .L_020080ce
.L_020080c2:
	ldr r3, .L_020080f0
	cmp r2, r3
	bne .L_020080cc
	ldr r0, .L_020080f4
	b .L_020080ce
.L_020080cc:
	ldr r0, .L_020080f8
.L_020080ce:
	pop {pc}
.L_020080d0:
	.4byte Data_020023c4 + 0x88
.L_020080d4:
	.4byte gPartyState
.L_020080d8:
	.4byte 0x000000f7
.L_020080dc:
	.4byte Data_02007024
.L_020080e0:
	.4byte 0x000000f9
.L_020080e4:
	.4byte Data_02006ea4
.L_020080e8:
	.4byte 0x000000fa
.L_020080ec:
	.4byte Data_02006ed4
.L_020080f0:
	.4byte 0x000000f8
.L_020080f4:
	.4byte Data_02006f04
.L_020080f8:
	.4byte Data_0200646c
	.section .text.x020080fc,"ax",%progbits
	.global Func_020000fc
	.thumb_func
Func_020000fc:
	push {r5, r6, lr}
	ldr r5, .L_02008134
	adds r6, r0, #0
	movs r1, #26
	ldrsh r2, [r5, r1]
	movs r1, #28
	ldrsh r3, [r5, r1]
	movs r0, #1
	cmp r2, r3
	blt .L_02008132
	movs r0, #0
	cmp r2, r3
	bgt .L_02008132
	movs r0, #161
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008126
	movs r0, #0
	b .L_02008132
.L_02008126:
	movs r2, #20
	ldrsh r3, [r5, r2]
	movs r0, #1
	cmp r3, #0
	bne .L_02008132
	adds r0, r6, #0
.L_02008132:
	pop {r5, r6, pc}
.L_02008134:
	.4byte Data_020023c4 + 0x88
	.section .text.x02008138,"ax",%progbits
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
	.section .text.x02008218,"ax",%progbits
	.global Func_02000218
	.thumb_func
Func_02000218:
	push {lr}
	sub sp, #8
	cmp r0, #0
	bne .L_02008260
	cmp r1, #3
	bne .L_02008236
	movs r3, #23
	str r3, [sp, #0]
	str r1, [sp, #4]
	movs r0, #0
	movs r1, #120
	movs r2, #1
	movs r3, #2
	bl Func_02005820
.L_02008236:
	movs r3, #57
	movs r2, #3
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #127
	movs r1, #3
	movs r2, #1
	movs r3, #2
	bl Func_02005820
	movs r3, #23
	movs r2, #8
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #127
	movs r2, #1
	movs r3, #1
	bl Func_02005818
	b .L_0200829e
.L_02008260:
	cmp r1, #3
	bne .L_02008276
	movs r3, #9
	str r3, [sp, #0]
	str r1, [sp, #4]
	movs r0, #0
	movs r1, #120
	movs r2, #1
	movs r3, #2
	bl Func_02005820
.L_02008276:
	movs r3, #43
	movs r2, #3
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #127
	movs r1, #3
	movs r2, #1
	movs r3, #2
	bl Func_02005820
	movs r3, #9
	movs r2, #8
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #1
	movs r1, #127
	movs r2, #1
	movs r3, #1
	bl Func_02005818
.L_0200829e:
	add sp, #8
	pop {pc}
	.2byte 0x0000
	.section .text.x020082a4,"ax",%progbits
	.global Func_020002a4
	.thumb_func
Func_020002a4:
	push {lr}
	sub sp, #8
	cmp r0, #0
	bne .L_020082ec
	cmp r1, #3
	bne .L_020082c2
	movs r3, #28
	str r3, [sp, #0]
	str r1, [sp, #4]
	movs r0, #0
	movs r1, #120
	movs r2, #1
	movs r3, #2
	bl Func_02005820
.L_020082c2:
	movs r3, #62
	movs r2, #3
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #127
	movs r1, #3
	movs r2, #1
	movs r3, #2
	bl Func_02005820
	movs r3, #28
	movs r2, #8
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #1
	movs r1, #127
	movs r2, #1
	movs r3, #1
	bl Func_02005818
	b .L_0200832a
.L_020082ec:
	cmp r1, #3
	bne .L_02008302
	movs r3, #4
	str r3, [sp, #0]
	str r1, [sp, #4]
	movs r0, #0
	movs r1, #120
	movs r2, #1
	movs r3, #2
	bl Func_02005820
.L_02008302:
	movs r3, #38
	movs r2, #3
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #127
	movs r1, #3
	movs r2, #1
	movs r3, #2
	bl Func_02005820
	movs r3, #4
	movs r2, #8
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #127
	movs r2, #1
	movs r3, #1
	bl Func_02005818
.L_0200832a:
	add sp, #8
	pop {pc}
	.2byte 0x0000
	.section .text.x02008330,"ax",%progbits
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
	.4byte 0x000029c6
	.section .text.x02008418,"ax",%progbits
	.global Func_02000418
	.thumb_func
Func_02000418:
	push {lr}
	movs r0, #193
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_ClearBit
	pop {pc}
	.2byte 0x0000
	.section .text.x02008428,"ax",%progbits
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
	bl ColossoLogRollingStage_Idle
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
	.4byte 0x000029c5
	.section .text.x020085b4,"ax",%progbits
	.global Func_020005b4
	.thumb_func
Func_020005b4:
	push {lr}
	cmp r0, #9
	beq .L_020085c4
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #134
	cmp r0, #11
	bne .L_020085ca
.L_020085c4:
	movs r3, #195
	lsls r3, r3, #1
	adds r3, #255
.L_020085ca:
	adds r0, r3, #0
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020085ec
	bl Func_02005880
	movs r0, #0
	bl Func_020059e8
	ldr r0, .L_0200861c
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
	bl Func_02005888
	b .L_02008618
.L_020085ec:
	ldr r0, .L_02008620
	movs r1, #13
	bl UiText_ShowPositionedMessageAndWait
	ldr r3, .L_02008624
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	ldr r0, [r3]
	movs r1, #1
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_02008618
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r1, #173
	lsls r1, r1, #1
	adds r2, r3, r1
	movs r3, #1
	strh r3, [r2]
.L_02008618:
	pop {pc}
	.2byte 0x0000
.L_0200861c:
	.4byte 0x000029c4
.L_02008620:
	.4byte 0x000029c3
.L_02008624:
	.4byte gPartyState
	.global Data_02000628
Data_02000628:
	.4byte 0x00004770
	.section .text.x0200862c,"ax",%progbits
	.global Func_0200062c
	.thumb_func
Func_0200062c:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #32]
	bl Object_GetById
	cmp r0, #0
	beq .L_0200865c
	movs r2, #156
	lsls r2, r2, #1
	adds r3, r5, r2
	ldr r1, [r3]
	ldr r3, [r0, #16]
	ldr r2, [r0, #8]
	asrs r3, r3, #20
	lsls r3, r3, #7
	asrs r2, r2, #20
	adds r2, r2, r3
	lsls r2, r2, #2
	adds r1, r1, r2
	ldrb r2, [r1, #3]
	movs r3, #128
	orrs r3, r2
	strb r3, [r1, #3]
.L_0200865c:
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x02008660,"ax",%progbits
	.global Func_02000660
	.thumb_func
Func_02000660:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #32]
	bl Object_GetById
	cmp r0, #0
	beq .L_02008690
	movs r2, #156
	lsls r2, r2, #1
	adds r3, r5, r2
	ldr r1, [r3]
	ldr r3, [r0, #16]
	ldr r2, [r0, #8]
	asrs r3, r3, #20
	lsls r3, r3, #7
	asrs r2, r2, #20
	adds r2, r2, r3
	lsls r2, r2, #2
	adds r1, r1, r2
	ldrb r2, [r1, #3]
	movs r3, #127
	ands r3, r2
	strb r3, [r1, #3]
.L_02008690:
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x02008694,"ax",%progbits
	.global Func_02000694
	.thumb_func
Func_02000694:
	ldr r1, [r0, #104]
	ldr r3, [r1, #8]
	str r3, [r0, #8]
	adds r3, r0, #0
	adds r3, #99
	ldrb r2, [r3]
	ldr r3, [r1, #12]
	lsls r2, r2, #16
	adds r3, r3, r2
	str r3, [r0, #12]
	ldr r3, [r1, #16]
	str r3, [r0, #16]
	ldr r3, [r1, #20]
	str r3, [r0, #20]
	movs r0, #1
	bx lr
	.section .text.x020086b4,"ax",%progbits
	.global Func_020006b4
	.thumb_func
Func_020006b4:
	push {r5, r6, lr}
	bl Object_GetById
	adds r6, r0, #0
	cmp r6, #0
	beq .L_02008706
	movs r0, #234
	ldr r1, [r6, #8]
	ldr r2, [r6, #12]
	ldr r3, [r6, #16]
	adds r0, #255
	bl Func_020057d8
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02008706
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	ldr r1, .L_02008708
	adds r0, r5, #0
	bl Func_020057d0
	str r6, [r5, #104]
	adds r0, r5, #0
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #128
	orrs r3, r2
	strb r3, [r0]
	adds r3, r5, #0
	movs r1, #0
	adds r3, #85
	strb r1, [r3]
	adds r2, r5, #0
	adds r3, #4
	strb r1, [r3]
	adds r2, #99
	movs r3, #16
	strb r3, [r2]
	str r5, [r6, #104]
.L_02008706:
	pop {r5, r6, pc}
.L_02008708:
	.4byte Data_02005a7c
	.section .text.x0200870c,"ax",%progbits
	.global Func_0200070c
	.thumb_func
Func_0200070c:
	push {r5, lr}
	adds r5, r1, #0
	bl Object_GetById
	cmp r0, #0
	beq .L_02008724
	ldr r0, [r0, #104]
	cmp r0, #0
	beq .L_02008724
	adds r3, r0, #0
	adds r3, #99
	strb r5, [r3]
.L_02008724:
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x02008728,"ax",%progbits
	.global Func_02000728
	.thumb_func
Func_02000728:
	push {lr}
	ldr r3, .L_02008744
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #18
	adds r3, r3, r2
	ldrb r3, [r3]
	adds r0, r1, #0
	cmp r3, #0
	bne .L_02008740
	bl Func_02000660
.L_02008740:
	pop {pc}
	.2byte 0x0000
.L_02008744:
	.4byte gPartyState
	.section .text.x02008748,"ax",%progbits
	.global Func_02000748
	.thumb_func
Func_02000748:
	push {lr}
	ldr r3, .L_02008764
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #18
	adds r3, r3, r2
	ldrb r3, [r3]
	adds r0, r1, #0
	cmp r3, #0
	bne .L_02008760
	bl Func_0200062c
.L_02008760:
	pop {pc}
	.2byte 0x0000
.L_02008764:
	.4byte gPartyState
	.section .text.x02008768,"ax",%progbits
	.global Func_02000768
	.thumb_func
Func_02000768:
	push {lr}
	movs r0, #17
	bl Object_GetById
	movs r3, #0
	adds r0, #98
	strb r3, [r0]
	movs r0, #130
	lsls r0, r0, #2
	adds r0, #255
	bl GameFlag_SetBit
	pop {pc}
	.2byte 0x0000
	.section .text.x02008784,"ax",%progbits
	.global Func_02000784
	.thumb_func
Func_02000784:
	push {lr}
	bl Func_020059c0
	pop {pc}
	.section .text.x0200878c,"ax",%progbits
	.global Func_0200078c
	.thumb_func
Func_0200078c:
	push {lr}
	sub sp, #8
	bl Object_GetById
	cmp r0, #0
	beq .L_020087b0
	ldr r2, [r0, #8]
	ldr r3, [r0, #16]
	asrs r2, r2, #20
	asrs r3, r3, #20
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r0, #14
	movs r1, #27
	movs r2, #1
	movs r3, #1
	bl Func_02005820
.L_020087b0:
	add sp, #8
	pop {pc}
	.section .text.x020087b4,"ax",%progbits
	.global Func_020007b4
	.thumb_func
Func_020007b4:
	push {lr}
	ldr r3, .L_020087e4
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	ldr r3, [r0, #12]
	movs r2, #128
	lsls r2, r2, #14
	cmp r3, r2
	bne .L_020087d8
	movs r0, #130
	lsls r0, r0, #2
	bl GameFlag_SetBit
	b .L_020087e0
.L_020087d8:
	movs r0, #130
	lsls r0, r0, #2
	bl GameFlag_ClearBit
.L_020087e0:
	pop {pc}
	.2byte 0x0000
.L_020087e4:
	.4byte gPartyState
	.section .text.x020087e8,"ax",%progbits
	.global Func_020007e8
	.thumb_func
Func_020007e8:
	push {lr}
	movs r0, #16
	bl Object_GetById
	ldr r3, [r0, #16]
	asrs r3, r3, #20
	cmp r3, #23
	bne .L_020087fe
	bl Func_020059c0
	b .L_02008810
.L_020087fe:
	ldr r3, .L_02008814
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	movs r1, #6
	movs r2, #0
	bl Func_02005918
.L_02008810:
	pop {pc}
	.2byte 0x0000
.L_02008814:
	.4byte gPartyState
	.section .text.x02008818,"ax",%progbits
	.global Func_02000818
	.thumb_func
Func_02000818:
	push {lr}
	adds r0, r1, #0
	adds r3, r0, #0
	subs r3, #15
	cmp r3, #1
	bhi .L_02008828
	bl Func_02000660
.L_02008828:
	pop {pc}
	.2byte 0x0000
	.section .text.x0200882c,"ax",%progbits
	.global Func_0200082c
	.thumb_func
Func_0200082c:
	push {r5, r6, r7, lr}
	adds r5, r1, #0
	adds r3, r5, #0
	subs r3, #15
	cmp r3, #1
	bhi .L_02008840
	adds r0, r5, #0
	bl Func_0200062c
	b .L_020088a2
.L_02008840:
	adds r0, r5, #0
	bl Object_GetById
	ldr r3, [r0, #8]
	movs r5, #0
	asrs r0, r3, #20
	cmp r0, #13
	bne .L_02008852
	movs r5, #17
.L_02008852:
	cmp r0, #15
	bne .L_02008858
	movs r5, #18
.L_02008858:
	cmp r0, #17
	bne .L_0200885e
	movs r5, #19
.L_0200885e:
	cmp r0, #19
	bne .L_02008864
	movs r5, #20
.L_02008864:
	cmp r5, #0
	beq .L_020088a2
	movs r3, #130
	lsls r3, r3, #1
	adds r3, #255
	adds r7, r5, r3
	adds r0, r7, #0
	bl GameFlag_Test
	adds r6, r0, #0
	cmp r6, #0
	bne .L_020088a2
	adds r0, r5, #0
	bl Object_GetById
	movs r1, #0
	str r6, [r0, #108]
	bl Animation_ApplyChildValues
	adds r0, r5, #0
	movs r1, #6
	bl Object_SetModeById
	movs r1, #16
	adds r0, r5, #0
	negs r1, r1
	bl Func_0200070c
	adds r0, r7, #0
	bl GameFlag_SetBit
.L_020088a2:
	pop {r5, r6, r7, pc}
	.section .text.x020088a4,"ax",%progbits
	.global Func_020008a4
	.thumb_func
Func_020008a4:
	push {lr}
	ldr r3, .L_020088cc
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Func_020059b8
	movs r3, #130
	lsls r3, r3, #1
	adds r3, #255
	adds r0, r0, r3
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020088c8
	bl Func_020059c0
.L_020088c8:
	pop {pc}
	.2byte 0x0000
.L_020088cc:
	.4byte gPartyState
	.section .text.x020088d0,"ax",%progbits
	.global Func_020008d0
	.thumb_func
Func_020008d0:
	push {r5, r6, lr}
	adds r5, r1, #0
	adds r0, r5, #0
	bl Object_GetById
	movs r1, #16
	adds r6, r0, #0
	adds r0, r5, #0
	bl Func_0200070c
	movs r3, #130
	lsls r3, r3, #1
	adds r3, #255
	adds r5, r5, r3
	adds r0, r5, #0
	bl GameFlag_ClearBit
	cmp r6, #0
	beq .L_02008902
	adds r1, r6, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #253
	ands r3, r2
	strb r3, [r1]
.L_02008902:
	pop {r5, r6, pc}
	.section .text.x02008904,"ax",%progbits
	.global Func_02000904
	.thumb_func
Func_02000904:
	push {r5, lr}
	adds r5, r1, #0
	subs r0, r5, #5
	bl Object_GetById
	movs r3, #0
	adds r0, #98
	strb r3, [r0]
	movs r3, #133
	lsls r3, r3, #2
	adds r3, #255
	adds r5, r5, r3
	adds r0, r5, #0
	bl GameFlag_SetBit
	pop {r5, pc}
	.section .text.x02008924,"ax",%progbits
	.global Func_02000924
	.thumb_func
Func_02000924:
	push {r5, r6, lr}
	adds r6, r0, #0
	bl Object_GetById
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02008958
	movs r1, #16
	negs r1, r1
	adds r0, r6, #0
	bl Func_0200070c
	adds r2, r5, #0
	adds r2, #89
	movs r3, #0
	strb r3, [r2]
	adds r1, r5, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #2
	orrs r3, r2
	strb r3, [r1]
	adds r0, r5, #0
	movs r1, #6
	bl Func_020057c8
.L_02008958:
	pop {r5, r6, pc}
	.2byte 0x0000
	.section .text.x0200895c,"ax",%progbits
	.global Func_0200095c
	.thumb_func
Func_0200095c:
	push {r5, r6, lr}
	adds r6, r0, #0
	bl Object_GetById
	adds r5, r0, #0
	cmp r5, #0
	beq .L_0200898e
	adds r0, r6, #0
	movs r1, #16
	bl Func_0200070c
	adds r2, r5, #0
	adds r2, #89
	movs r3, #1
	strb r3, [r2]
	adds r1, r5, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #253
	ands r3, r2
	strb r3, [r1]
	adds r0, r5, #0
	movs r1, #5
	bl Func_020057c8
.L_0200898e:
	pop {r5, r6, pc}
	.section .text.x02008990,"ax",%progbits
	.global Func_02000990
	.thumb_func
Func_02000990:
	push {lr}
	movs r0, #16
	bl Object_GetById
	movs r3, #0
	adds r0, #98
	strb r3, [r0]
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #57
	bl GameFlag_SetBit
	pop {pc}
	.2byte 0x0000
	.section .text.x020089ac,"ax",%progbits
	.global Func_020009ac
	.thumb_func
Func_020009ac:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r7, #18
.L_020089b6:
	adds r0, r7, #0
	bl Object_GetById
	ldr r3, [r0, #8]
	ldr r1, .L_02008a14
	asrs r3, r3, #20
	mov r10, r3
	ldr r3, [r0, #16]
	movs r2, #3
	asrs r3, r3, #20
	mov r8, r3
	adds r3, r7, #0
	subs r3, #18
	ands r3, r2
	ldrsb r0, [r1, r3]
	bl Object_GetById
	adds r6, r0, #0
	adds r6, #99
	movs r3, #0
	movs r5, #15
	strb r3, [r6]
	b .L_020089e6
.L_020089e4:
	adds r5, #1
.L_020089e6:
	cmp r5, #17
	bgt .L_02008a04
	adds r0, r5, #0
	bl Object_GetById
	ldr r3, [r0, #8]
	asrs r3, r3, #20
	cmp r10, r3
	bne .L_020089e4
	ldr r3, [r0, #16]
	asrs r3, r3, #20
	cmp r8, r3
	bne .L_020089e4
	movs r3, #1
	strb r3, [r6]
.L_02008a04:
	adds r7, #1
	cmp r7, #21
	ble .L_020089b6
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008a14:
	.4byte Data_02005a88
	.section .text.x02008a18,"ax",%progbits
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
	.section .text.x02008b3c,"ax",%progbits
	.global Func_02000b3c
	.thumb_func
Func_02000b3c:
	push {lr}
	adds r3, r0, #0
	adds r2, r1, #0
	movs r0, #18
	adds r1, r3, #0
	bl Func_02000a18
	pop {pc}
	.section .text.x02008b4c,"ax",%progbits
	.global Func_02000b4c
	.thumb_func
Func_02000b4c:
	push {lr}
	adds r3, r0, #0
	adds r2, r1, #0
	movs r0, #19
	adds r1, r3, #0
	bl Func_02000a18
	pop {pc}
	.section .text.x02008b5c,"ax",%progbits
	.global Func_02000b5c
	.thumb_func
Func_02000b5c:
	push {lr}
	adds r3, r0, #0
	adds r2, r1, #0
	movs r0, #20
	adds r1, r3, #0
	bl Func_02000a18
	pop {pc}
	.section .text.x02008b6c,"ax",%progbits
	.global Func_02000b6c
	.thumb_func
Func_02000b6c:
	push {lr}
	adds r3, r0, #0
	adds r2, r1, #0
	movs r0, #21
	adds r1, r3, #0
	bl Func_02000a18
	pop {pc}
	.section .text.x02008b7c,"ax",%progbits
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
	.section .text.x02008c34,"ax",%progbits
	.global Func_02000c34
	.thumb_func
Func_02000c34:
	push {lr}
	movs r2, #192
	lsls r2, r2, #2
	adds r2, #53
	movs r0, #18
	movs r1, #19
	movs r3, #1
	bl Func_02000b7c
	pop {pc}
	.section .text.x02008c48,"ax",%progbits
	.global Func_02000c48
	.thumb_func
Func_02000c48:
	push {lr}
	movs r2, #192
	lsls r2, r2, #2
	adds r2, #53
	movs r0, #19
	movs r1, #18
	movs r3, #0
	bl Func_02000b7c
	pop {pc}
	.section .text.x02008c5c,"ax",%progbits
	.global Func_02000c5c
	.thumb_func
Func_02000c5c:
	push {lr}
	movs r2, #142
	lsls r2, r2, #2
	adds r2, #255
	movs r0, #20
	movs r1, #21
	movs r3, #1
	bl Func_02000b7c
	pop {pc}
	.section .text.x02008c70,"ax",%progbits
	.global Func_02000c70
	.thumb_func
Func_02000c70:
	push {lr}
	movs r2, #142
	lsls r2, r2, #2
	adds r2, #255
	movs r0, #21
	movs r1, #20
	movs r3, #0
	bl Func_02000b7c
	pop {pc}
	.section .text.x02008c84,"ax",%progbits
	.global Func_02000c84
	.thumb_func
Func_02000c84:
	push {lr}
	ldr r3, .L_02008cbc
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	ldr r3, [r0, #12]
	movs r2, #128
	lsls r2, r2, #13
	cmp r3, r2
	blt .L_02008ca4
	bl Func_020059c0
	b .L_02008cb8
.L_02008ca4:
	bl Func_02005a30
	cmp r0, #0
	beq .L_02008cb8
	bl Func_02005a38
	movs r0, #0
	movs r1, #0
	bl Func_020009ac
.L_02008cb8:
	pop {pc}
	.2byte 0x0000
.L_02008cbc:
	.4byte gPartyState
	.section .text.x02008cc0,"ax",%progbits
	.global Func_02000cc0
	.thumb_func
Func_02000cc0:
	push {lr}
	ldr r0, .L_02008ccc
	bl Func_02004418
	pop {pc}
	.2byte 0x0000
.L_02008ccc:
	.4byte Data_02007734
	.section .text.x02008cd0,"ax",%progbits
	.global Func_02000cd0
	.thumb_func
Func_02000cd0:
	push {lr}
	cmp r0, #3
	bne .L_02008cde
	ldr r0, .L_02008ce0
	movs r1, #15
	bl Func_020045ac
.L_02008cde:
	pop {pc}
.L_02008ce0:
	.4byte Data_02007734
	.section .text.x02008ce4,"ax",%progbits
	.global Func_02000ce4
	.thumb_func
Func_02000ce4:
	push {lr}
	cmp r0, #3
	bne .L_02008cf2
	ldr r0, .L_02008cf4
	movs r1, #16
	bl Func_020045ac
.L_02008cf2:
	pop {pc}
.L_02008cf4:
	.4byte Data_02007734
	.section .text.x02008cf8,"ax",%progbits
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
	.section .text.x02008d70,"ax",%progbits
	.global Func_02000d70
	.thumb_func
Func_02000d70:
	push {r5, lr}
	sub sp, #8
	movs r3, #11
	str r3, [sp, #0]
	movs r5, #22
	movs r0, #11
	movs r1, #64
	movs r2, #11
	movs r3, #8
	str r5, [sp, #4]
	bl Func_02005818
	movs r3, #79
	str r3, [sp, #0]
	movs r2, #11
	movs r3, #8
	movs r0, #79
	movs r1, #64
	str r5, [sp, #4]
	bl Func_02005818
	movs r0, #16
	movs r1, #0
	bl Func_02000cf8
	movs r0, #17
	movs r1, #0
	bl Func_02000cf8
	movs r0, #18
	movs r1, #1
	bl Func_02000cf8
	movs r0, #170
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008dd4
	movs r3, #19
	movs r2, #23
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #48
	movs r2, #1
	movs r3, #3
	bl Func_02005818
.L_02008dd4:
	movs r0, #149
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008df4
	movs r3, #19
	movs r2, #26
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #48
	movs r2, #1
	movs r3, #3
	bl Func_02005818
.L_02008df4:
	add sp, #8
	pop {r5, pc}
	.section .text.x02008df8,"ax",%progbits
	.global Func_02000df8
	.thumb_func
Func_02000df8:
	push {r5, r6, lr}
	adds r6, r0, #0
	movs r0, #15
	adds r5, r1, #0
	bl Object_GetById
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #32]
	ldr r3, [r0, #8]
	asrs r3, r3, #20
	cmp r3, r6
	bne .L_02008e1c
	ldr r3, [r0, #16]
	movs r0, #1
	asrs r3, r3, #20
	cmp r3, r5
	beq .L_02008e3c
.L_02008e1c:
	movs r1, #156
	lsls r1, r1, #1
	adds r3, r2, r1
	ldr r2, [r3]
	lsls r3, r5, #7
	adds r3, r6, r3
	lsls r3, r3, #2
	adds r2, r2, r3
	ldrb r2, [r2, #2]
	movs r3, #255
	eors r2, r3
	negs r3, r2
	orrs r3, r2
	lsrs r3, r3, #31
	movs r0, #1
	subs r0, r0, r3
.L_02008e3c:
	pop {r5, r6, pc}
	.2byte 0x0000
	.section .text.x02008e40,"ax",%progbits
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
	.section .text.x0200906c,"ax",%progbits
	.global Func_0200106c
	.thumb_func
Func_0200106c:
	push {lr}
	movs r1, #1
	negs r1, r1
	movs r0, #16
	movs r2, #0
	movs r3, #0
	bl Func_02000e40
	pop {pc}
	.2byte 0x0000
	.section .text.x02009080,"ax",%progbits
	.global Func_02001080
	.thumb_func
Func_02001080:
	push {lr}
	movs r0, #16
	movs r1, #1
	movs r2, #0
	movs r3, #0
	bl Func_02000e40
	pop {pc}
	.section .text.x02009090,"ax",%progbits
	.global Func_02001090
	.thumb_func
Func_02001090:
	push {lr}
	movs r1, #1
	negs r1, r1
	movs r0, #17
	movs r2, #0
	movs r3, #0
	bl Func_02000e40
	pop {pc}
	.2byte 0x0000
	.section .text.x020090a4,"ax",%progbits
	.global Func_020010a4
	.thumb_func
Func_020010a4:
	push {lr}
	movs r0, #17
	movs r1, #1
	movs r2, #0
	movs r3, #0
	bl Func_02000e40
	pop {pc}
	.section .text.x020090b4,"ax",%progbits
	.global Func_020010b4
	.thumb_func
Func_020010b4:
	push {lr}
	movs r2, #1
	negs r2, r2
	movs r0, #18
	movs r1, #0
	movs r3, #1
	bl Func_02000e40
	pop {pc}
	.2byte 0x0000
	.section .text.x020090c8,"ax",%progbits
	.global Func_020010c8
	.thumb_func
Func_020010c8:
	push {lr}
	movs r0, #18
	movs r1, #0
	movs r2, #1
	movs r3, #1
	bl Func_02000e40
	pop {pc}
	.section .text.x020090d8,"ax",%progbits
	.global Func_020010d8
	.thumb_func
Func_020010d8:
	push {lr}
	ldr r3, .L_02009100
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #18
	adds r3, r3, r2
	ldrb r3, [r3]
	cmp r3, #2
	bne .L_020090f0
	bl Func_020059c8
	b .L_020090fc
.L_020090f0:
	movs r0, #18
	movs r1, #0
	movs r2, #1
	movs r3, #1
	bl Func_02000e40
.L_020090fc:
	pop {pc}
	.2byte 0x0000
.L_02009100:
	.4byte gPartyState
	.section .text.x02009104,"ax",%progbits
	.global Func_02001104
	.thumb_func
Func_02001104:
	push {r5, r6, lr}
	adds r6, r0, #0
	cmp r2, #30
	bne .L_02009144
	bl Object_GetById
	adds r5, r0, #0
	adds r3, r5, #0
	adds r3, #100
	movs r0, #0
	ldrsh r1, [r3, r0]
	adds r3, #2
	lsls r1, r1, #16
	str r1, [r5, #8]
	movs r0, #0
	ldrsh r2, [r3, r0]
	movs r0, #0
	lsls r2, r2, #16
	str r2, [r5, #16]
	bl Map_GetTerrainHeight
	str r0, [r5, #20]
	str r0, [r5, #12]
	adds r0, r6, #0
	bl Func_0200078c
	movs r2, #149
	lsls r2, r2, #2
	adds r2, #255
	adds r0, r6, r2
	bl GameFlag_SetBit
.L_02009144:
	pop {r5, r6, pc}
	.2byte 0x0000
	.section .text.x02009148,"ax",%progbits
	.global Func_02001148
	.thumb_func
Func_02001148:
	push {lr}
	adds r3, r0, #0
	adds r2, r1, #0
	movs r0, #16
	adds r1, r3, #0
	bl Func_02001104
	pop {pc}
	.section .text.x02009158,"ax",%progbits
	.global Func_02001158
	.thumb_func
Func_02001158:
	push {lr}
	adds r3, r0, #0
	adds r2, r1, #0
	movs r0, #17
	adds r1, r3, #0
	bl Func_02001104
	pop {pc}
	.section .text.x02009168,"ax",%progbits
	.global Func_02001168
	.thumb_func
Func_02001168:
	push {lr}
	adds r3, r0, #0
	adds r2, r1, #0
	movs r0, #18
	adds r1, r3, #0
	bl Func_02001104
	pop {pc}
	.section .text.x02009178,"ax",%progbits
	.global Func_02001178
	.thumb_func
Func_02001178:
	push {r5, lr}
	ldr r3, .L_020091bc
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	adds r5, r0, #0
	movs r0, #15
	bl Object_GetById
	ldr r3, [r5, #8]
	ldr r2, [r0, #8]
	asrs r3, r3, #20
	asrs r2, r2, #20
	cmp r2, r3
	bne .L_020091b8
	ldr r2, [r0, #16]
	ldr r3, [r5, #16]
	asrs r2, r2, #20
	asrs r3, r3, #20
	cmp r2, r3
	bne .L_020091b8
	ldr r3, [r5, #12]
	movs r2, #128
	lsls r2, r2, #14
	cmp r3, r2
	bge .L_020091b8
	movs r3, #128
	lsls r3, r3, #14
	str r3, [r5, #12]
.L_020091b8:
	pop {r5, pc}
	.2byte 0x0000
.L_020091bc:
	.4byte gPartyState
	.section .text.x020091c0,"ax",%progbits
	.global Func_020011c0
	.thumb_func
Func_020011c0:
	push {lr}
	ldr r3, .L_020091d8
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	movs r3, #2
	adds r0, #34
	strb r3, [r0]
	pop {pc}
.L_020091d8:
	.4byte gPartyState
	.section .text.x020091dc,"ax",%progbits
	.global Func_020011dc
	.thumb_func
Func_020011dc:
	push {lr}
	ldr r3, .L_020091f4
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	movs r3, #0
	adds r0, #34
	strb r3, [r0]
	pop {pc}
.L_020091f4:
	.4byte gPartyState
	.section .text.x020091f8,"ax",%progbits
	.global Func_020011f8
	.thumb_func
Func_020011f8:
	push {lr}
	sub sp, #8
	movs r3, #19
	movs r2, #21
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #21
	movs r2, #1
	movs r3, #1
	movs r0, #12
	bl Func_02005818
	movs r0, #221
	lsls r0, r0, #2
	bl GameFlag_SetBit
	add sp, #8
	pop {pc}
	.section .text.x02009262,"ax",%progbits
	.2byte 0x0000
	.section .text.x02009264,"ax",%progbits
	.global Func_02001264
	.thumb_func
Func_02001264:
	ldr r3, [r0, #8]
	ldr r2, [r0, #68]
	ldr r1, [r0, #80]
	adds r3, r3, r2
	str r3, [r0, #8]
	ldr r2, [r0, #72]
	ldr r3, [r0, #12]
	adds r3, r3, r2
	str r3, [r0, #12]
	ldr r2, [r0, #76]
	ldr r3, [r0, #16]
	adds r3, r3, r2
	str r3, [r0, #16]
	ldr r2, [r0, #48]
	ldr r3, [r0, #24]
	adds r3, r3, r2
	str r3, [r0, #24]
	ldr r2, [r0, #52]
	ldr r3, [r0, #28]
	adds r3, r3, r2
	str r3, [r0, #28]
	adds r0, #100
	ldrh r3, [r1, #18]
	ldrh r2, [r0]
	adds r3, r3, r2
	strh r3, [r1, #18]
	bx lr
	.2byte 0x0000
	.section .text.x0200929c,"ax",%progbits
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
	bl ObjectDispatch_SetSingleChildField26
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
	bl Engine_MathDivide
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, [r6, #28]
	ldr r1, [r5, #12]
	subs r0, r0, r3
	b .L_020093f0
.L_020093de:
	ldr r2, .L_02009460
	adds r0, r3, r2
	bl Engine_MathDivide
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, .L_02009460
	ldr r1, [r5, #12]
	adds r0, r0, r3
.L_020093f0:
	bl Engine_MathDivide
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
	.4byte Func_02001264
.L_02009460:
	.4byte 0xffff0000
	.section .text.x02009464,"ax",%progbits
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
	bl Engine_MathDivide
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
	.4byte Data_0300122c
.L_0200959c:
	.4byte Func_02001464
.L_020095a0:
	.4byte 0xffff0000
.L_020095a4:
	.4byte 0xffff8000
	.section .text.x020095a8,"ax",%progbits
	.global Func_020015a8
	.thumb_func
Func_020015a8:
	push {lr}
	movs r0, #197
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
	pop {pc}
	.2byte 0x0000
	.section .text.x020095b8,"ax",%progbits
	.global Func_020015b8
	.thumb_func
Func_020015b8:
	push {lr}
	sub sp, #12
	movs r2, #64
	movs r3, #13
	str r3, [sp, #0]
	str r2, [sp, #4]
	str r2, [sp, #8]
	movs r0, #68
	movs r1, #6
	movs r2, #1
	movs r3, #21
	bl Func_02005a50
	add sp, #12
	pop {pc}
	.2byte 0x0000
	.section .text.x020095d8,"ax",%progbits
	.global Func_020015d8
	.thumb_func
Func_020015d8:
	push {lr}
	sub sp, #12
	movs r2, #64
	movs r3, #13
	str r3, [sp, #0]
	str r2, [sp, #4]
	str r2, [sp, #8]
	movs r0, #68
	movs r1, #6
	movs r2, #1
	movs r3, #21
	bl Func_02005a50
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #185
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200962e
	movs r0, #249
	movs r1, #72
	movs r2, #152
	bl Func_02005a60
	ldr r2, .L_02009634
	movs r3, #149
	lsls r3, r3, #2
	adds r1, r2, r3
	movs r3, #200
	lsls r3, r3, #5
	adds r3, #185
	strh r3, [r1]
	movs r3, #166
	lsls r3, r3, #1
	adds r3, #255
	adds r2, r2, r3
	movs r3, #2
	strb r3, [r2]
	movs r0, #106
	movs r1, #4
	bl Func_02005998
.L_0200962e:
	add sp, #12
	pop {pc}
	.2byte 0x0000
.L_02009634:
	.4byte gPartyState
	.section .text.x02009638,"ax",%progbits
	.global Func_02001638
	.thumb_func
Func_02001638:
	movs r0, #1
	bx lr
	.section .text.x0200963c,"ax",%progbits
	.global Func_0200163c
	.thumb_func
Func_0200163c:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r8
	push {r5, r6}
	sub sp, #8
	movs r2, #11
	movs r3, #10
	str r3, [sp, #0]
	str r2, [sp, #4]
	mov r10, r2
	movs r0, #33
	movs r1, #30
	movs r2, #1
	movs r3, #1
	bl Func_02005810
	movs r3, #7
	str r3, [sp, #4]
	movs r5, #18
	mov r8, r3
	movs r0, #33
	movs r1, #30
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	bl Func_02005810
	movs r6, #15
	movs r0, #33
	movs r1, #30
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_02005810
	mov r2, r10
	movs r3, #74
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #35
	movs r1, #30
	movs r2, #1
	movs r3, #1
	bl Func_02005810
	mov r3, r8
	str r3, [sp, #4]
	movs r5, #82
	movs r0, #35
	movs r1, #30
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	bl Func_02005810
	movs r0, #35
	movs r1, #30
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_02005810
	add sp, #8
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, pc}
	.2byte 0x0000
	.section .text.x020096c8,"ax",%progbits
	.global Func_020016c8
	.thumb_func
Func_020016c8:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r8
	push {r5, r6}
	sub sp, #8
	movs r2, #11
	movs r3, #10
	str r3, [sp, #0]
	str r2, [sp, #4]
	mov r10, r2
	movs r0, #34
	movs r1, #30
	movs r2, #1
	movs r3, #1
	bl Func_02005810
	movs r3, #7
	str r3, [sp, #4]
	movs r5, #18
	mov r8, r3
	movs r0, #34
	movs r1, #30
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	bl Func_02005810
	movs r6, #15
	movs r0, #34
	movs r1, #30
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_02005810
	mov r2, r10
	movs r3, #74
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #36
	movs r1, #30
	movs r2, #1
	movs r3, #1
	bl Func_02005810
	mov r3, r8
	str r3, [sp, #4]
	movs r5, #82
	movs r0, #36
	movs r1, #30
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	bl Func_02005810
	movs r0, #36
	movs r1, #30
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_02005810
	add sp, #8
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, pc}
	.2byte 0x0000
	.section .text.x02009754,"ax",%progbits
	.global Func_02001754
	.thumb_func
Func_02001754:
	push {r5, r6, lr}
	adds r6, r0, #0
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #255
	sub sp, #8
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020097cc
	ldr r3, .L_020097dc
	movs r2, #18
	ldrsh r3, [r3, r2]
	cmp r3, #0
	bne .L_0200979c
	movs r3, #13
	movs r2, #79
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #26
	movs r1, #79
	movs r2, #1
	movs r3, #2
	bl Func_02005820
	movs r3, #77
	movs r2, #18
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #78
	movs r1, #18
	movs r2, #1
	movs r3, #1
	bl Func_02005818
	b .L_020097c2
.L_0200979c:
	movs r3, #79
	movs r5, #18
	str r3, [sp, #4]
	movs r0, #26
	movs r1, #79
	movs r2, #1
	movs r3, #2
	str r5, [sp, #0]
	bl Func_02005820
	movs r3, #82
	str r3, [sp, #0]
	movs r0, #81
	movs r1, #18
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl Func_02005818
.L_020097c2:
	cmp r6, #0
	beq .L_020097cc
	movs r0, #138
	bl Func_02005a68
.L_020097cc:
	movs r0, #194
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
	add sp, #8
	pop {r5, r6, pc}
	.2byte 0x0000
.L_020097dc:
	.4byte Data_020023c4 + 0x88
	.section .text.x020097e0,"ax",%progbits
	.global Func_020017e0
	.thumb_func
Func_020017e0:
	push {r5, r6, lr}
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #129
	bl GameFlag_Test
	adds r6, r0, #0
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #130
	bl GameFlag_Test
	adds r6, r6, r0
	movs r0, #161
	lsls r0, r0, #2
	adds r0, #255
	bl GameFlag_Test
	adds r6, r6, r0
	movs r0, #225
	lsls r0, r0, #2
	bl GameFlag_Test
	adds r6, r6, r0
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #133
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009824
	bl Func_02002dec
	b .L_02009828
.L_02009824:
	bl Func_0200322c
.L_02009828:
	ldr r3, .L_02009854
	movs r2, #166
	lsls r2, r2, #1
	adds r2, #255
	adds r3, r3, r2
	movs r2, #2
	strb r2, [r3]
	ldr r5, .L_02009858
	movs r1, #4
	adds r0, r5, #0
	bl Party_SetFields1eeAnd1f0
	adds r0, r5, #0
	movs r1, #5
	bl Party_SetFields1f2And1f4
	movs r1, #4
	subs r1, r1, r6
	movs r0, #13
	bl Func_02005998
	pop {r5, r6, pc}
.L_02009854:
	.4byte gPartyState
.L_02009858:
	.4byte 0x000000f8
	.section .text.x0200985c,"ax",%progbits
	.global Func_0200185c
	.thumb_func
Func_0200185c:
	push {r5, lr}
	adds r0, r1, #0
	bl Object_GetById
	adds r5, r0, #0
	ldr r3, [r5, #8]
	asrs r3, r3, #20
	cmp r3, #8
	bne .L_02009888
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #252
	bl GameFlag_SetBit
	adds r2, r5, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	ldr r2, .L_0200988c
	ldr r3, [r5, #12]
	adds r3, r3, r2
	str r3, [r5, #12]
.L_02009888:
	pop {r5, pc}
	.2byte 0x0000
.L_0200988c:
	.4byte 0xffff0000
	.section .text.x02009890,"ax",%progbits
	.global Func_02001890
	.thumb_func
Func_02001890:
	push {lr}
	ldr r3, .L_020098d8
	movs r2, #240
	movs r0, #16
	ldrsh r1, [r3, r0]
	ldr r3, .L_020098dc
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r0, #0
	ldrsh r2, [r3, r0]
	ldr r3, .L_020098e0
	cmp r2, r3
	bne .L_020098b6
	movs r3, #7
	ldr r2, .L_020098e4
	ands r1, r3
	lsls r3, r1, #2
	ldr r0, [r2, r3]
	b .L_020098d6
.L_020098b6:
	ldr r3, .L_020098e8
	cmp r2, r3
	bne .L_020098c0
	ldr r0, .L_020098ec
	b .L_020098d6
.L_020098c0:
	ldr r3, .L_020098f0
	cmp r2, r3
	bne .L_020098ca
	ldr r0, .L_020098f4
	b .L_020098d6
.L_020098ca:
	ldr r3, .L_020098f8
	cmp r2, r3
	bne .L_020098d4
	ldr r0, .L_020098fc
	b .L_020098d6
.L_020098d4:
	ldr r0, .L_02009900
.L_020098d6:
	pop {pc}
.L_020098d8:
	.4byte Data_020023c4 + 0x88
.L_020098dc:
	.4byte gPartyState
.L_020098e0:
	.4byte 0x000000f7
.L_020098e4:
	.4byte Data_02007e00
.L_020098e8:
	.4byte 0x000000f9
.L_020098ec:
	.4byte Data_02007cf8
.L_020098f0:
	.4byte 0x000000fa
.L_020098f4:
	.4byte Data_02007d40
.L_020098f8:
	.4byte 0x000000f8
.L_020098fc:
	.4byte Data_02007d94
.L_02009900:
	.4byte Data_02007044
	.section .text.x02009904,"ax",%progbits
	.global Func_02001904
	.thumb_func
Func_02001904:
	push {lr}
	movs r0, #130
	lsls r0, r0, #2
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02009922
	movs r0, #17
	bl Object_GetById
	movs r3, #1
	adds r0, #98
	strb r3, [r0]
	b .L_0200992c
.L_02009922:
	movs r0, #20
	movs r1, #0
	movs r2, #0
	bl Func_020058e0
.L_0200992c:
	movs r0, #15
	bl Func_020006b4
	movs r0, #16
	bl Func_020006b4
	movs r0, #17
	bl Func_020006b4
	movs r0, #18
	bl Func_020006b4
	movs r0, #19
	bl Func_020006b4
	movs r0, #16
	bl Func_0200062c
	movs r0, #17
	bl Func_0200062c
	movs r0, #18
	bl Func_0200062c
	movs r0, #19
	bl Func_0200062c
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_02009970
	bl Scheduler_AddOrUpdateCallback
	pop {pc}
	.2byte 0x0000
.L_02009970:
	.4byte Func_020007b4
	.section .text.x02009974,"ax",%progbits
	.global Func_02001974
	.thumb_func
Func_02001974:
	push {r5, lr}
	movs r0, #15
	bl Func_020006b4
	movs r0, #16
	bl Func_020006b4
	movs r0, #17
	bl Func_020006b4
	movs r0, #18
	bl Func_020006b4
	movs r0, #19
	bl Func_020006b4
	movs r0, #20
	bl Func_020006b4
	movs r0, #15
	bl Func_0200062c
	movs r0, #16
	bl Func_0200062c
	movs r0, #17
	bl Func_0200062c
	movs r0, #18
	bl Func_0200062c
	movs r0, #19
	bl Func_0200062c
	movs r0, #20
	bl Func_0200062c
	movs r0, #17
	bl Object_GetById
	movs r5, #1
	adds r0, #98
	strb r5, [r0]
	movs r0, #18
	bl Object_GetById
	adds r0, #98
	strb r5, [r0]
	movs r0, #19
	bl Object_GetById
	adds r0, #98
	strb r5, [r0]
	movs r0, #20
	bl Object_GetById
	adds r0, #98
	strb r5, [r0]
	movs r0, #133
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020099fe
	movs r0, #17
	movs r1, #5
	bl Object_SetModeById
	b .L_02009a08
.L_020099fe:
	movs r1, #16
	negs r1, r1
	movs r0, #17
	bl Func_0200070c
.L_02009a08:
	movs r0, #139
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02009a20
	movs r0, #18
	movs r1, #5
	bl Object_SetModeById
	b .L_02009a2a
.L_02009a20:
	movs r1, #16
	negs r1, r1
	movs r0, #18
	bl Func_0200070c
.L_02009a2a:
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #22
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02009a42
	movs r0, #19
	movs r1, #5
	bl Object_SetModeById
	b .L_02009a4c
.L_02009a42:
	movs r1, #16
	negs r1, r1
	movs r0, #19
	bl Func_0200070c
.L_02009a4c:
	movs r0, #140
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02009a64
	movs r0, #20
	movs r1, #5
	bl Object_SetModeById
	b .L_02009a6e
.L_02009a64:
	movs r1, #16
	negs r1, r1
	movs r0, #20
	bl Func_0200070c
.L_02009a6e:
	pop {r5, pc}
	.section .text.x02009a70,"ax",%progbits
	.global Func_02001a70
	.thumb_func
Func_02001a70:
	push {lr}
	movs r0, #138
	lsls r0, r0, #2
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02009a8e
	movs r0, #15
	bl Object_GetById
	movs r3, #1
	adds r0, #98
	strb r3, [r0]
	b .L_02009a98
.L_02009a8e:
	movs r0, #20
	movs r1, #0
	movs r2, #0
	bl Func_020058e0
.L_02009a98:
	movs r0, #202
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02009ab2
	movs r0, #16
	bl Object_GetById
	movs r3, #1
	adds r0, #98
	strb r3, [r0]
	b .L_02009abc
.L_02009ab2:
	movs r0, #21
	movs r1, #0
	movs r2, #0
	bl Func_020058e0
.L_02009abc:
	movs r0, #15
	bl Func_020006b4
	movs r0, #16
	bl Func_020006b4
	movs r0, #17
	bl Func_020006b4
	movs r0, #18
	bl Func_020006b4
	movs r0, #19
	bl Func_020006b4
	movs r0, #15
	bl Func_0200062c
	movs r0, #16
	bl Func_0200062c
	movs r0, #17
	bl Func_0200062c
	movs r0, #18
	bl Func_0200062c
	movs r0, #19
	bl Func_0200062c
	pop {pc}
	.2byte 0x0000
	.section .text.x02009afc,"ax",%progbits
	.global Func_02001afc
	.thumb_func
Func_02001afc:
	push {r5, lr}
	movs r0, #18
	bl Object_GetById
	movs r5, #1
	adds r0, #98
	strb r5, [r0]
	movs r0, #19
	bl Object_GetById
	adds r0, #98
	strb r5, [r0]
	movs r0, #20
	bl Object_GetById
	adds r0, #98
	strb r5, [r0]
	movs r0, #21
	bl Object_GetById
	adds r0, #98
	strb r5, [r0]
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #57
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02009b42
	movs r0, #16
	bl Object_GetById
	adds r0, #98
	strb r5, [r0]
	b .L_02009b4c
.L_02009b42:
	movs r0, #22
	movs r1, #0
	movs r2, #0
	bl Func_020058e0
.L_02009b4c:
	movs r0, #15
	bl Func_020006b4
	movs r0, #16
	bl Func_020006b4
	movs r0, #17
	bl Func_020006b4
	movs r0, #18
	bl Func_020006b4
	movs r0, #19
	bl Func_020006b4
	movs r0, #20
	bl Func_020006b4
	movs r0, #21
	bl Func_020006b4
	movs r0, #15
	bl Func_0200062c
	movs r0, #16
	bl Func_0200062c
	movs r0, #17
	bl Func_0200062c
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #53
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02009ba4
	movs r0, #18
	bl Func_0200095c
	movs r0, #19
	bl Func_02000924
	b .L_02009bb0
.L_02009ba4:
	movs r0, #19
	bl Func_0200095c
	movs r0, #18
	bl Func_02000924
.L_02009bb0:
	movs r0, #142
	lsls r0, r0, #2
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02009bcc
	movs r0, #20
	bl Func_0200095c
	movs r0, #21
	bl Func_02000924
	b .L_02009bd8
.L_02009bcc:
	movs r0, #21
	bl Func_0200095c
	movs r0, #20
	bl Func_02000924
.L_02009bd8:
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x02009bdc,"ax",%progbits
	.global Func_02001bdc
	.thumb_func
Func_02001bdc:
	push {lr}
	ldr r0, .L_02009be8
	bl Func_020042f0
	pop {pc}
	.2byte 0x0000
.L_02009be8:
	.4byte Data_02007734
	.section .text.x02009bec,"ax",%progbits
	.global Func_02001bec
	.thumb_func
Func_02001bec:
	push {r5, r6, lr}
	sub sp, #8
	movs r3, #11
	str r3, [sp, #0]
	movs r5, #64
	movs r0, #11
	movs r1, #22
	movs r2, #11
	movs r3, #8
	str r5, [sp, #4]
	bl Func_02005818
	movs r3, #79
	str r3, [sp, #0]
	movs r1, #22
	movs r2, #11
	movs r3, #8
	movs r0, #79
	str r5, [sp, #4]
	bl Func_02005818
	movs r0, #16
	bl Object_GetById
	movs r5, #1
	adds r0, #98
	strb r5, [r0]
	movs r0, #17
	bl Object_GetById
	adds r0, #98
	strb r5, [r0]
	movs r0, #18
	bl Object_GetById
	adds r0, #98
	strb r5, [r0]
	movs r0, #16
	bl Object_GetById
	movs r6, #0
	adds r0, #89
	strb r6, [r0]
	movs r0, #17
	bl Object_GetById
	adds r0, #89
	strb r6, [r0]
	movs r0, #18
	bl Object_GetById
	adds r0, #89
	strb r6, [r0]
	bl Func_02000d70
	movs r0, #15
	bl Func_020006b4
	movs r0, #15
	bl Func_0200062c
	add sp, #8
	pop {r5, r6, pc}
	.2byte 0x0000
	.section .text.x02009c6c,"ax",%progbits
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
	.section .text.x02009d8c,"ax",%progbits
	.global Func_02001d8c
	.thumb_func
Func_02001d8c:
	push {lr}
	sub sp, #8
	bl Func_02005a08
	movs r0, #0
	movs r1, #15
	movs r2, #16
	bl Func_02005a10
	movs r0, #221
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02009dc0
	movs r3, #19
	movs r2, #21
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #18
	movs r1, #21
	movs r2, #1
	movs r3, #1
	bl Func_02005818
	b .L_02009dca
.L_02009dc0:
	movs r0, #17
	movs r1, #0
	movs r2, #0
	bl Func_020058e0
.L_02009dca:
	add sp, #8
	pop {pc}
	.2byte 0x0000
	.section .text.x02009dd0,"ax",%progbits
	.global Func_02001dd0
	.thumb_func
Func_02001dd0:
	bx lr
	.2byte 0x0000
	.section .text.x02009dd4,"ax",%progbits
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
	.section .text.x02009eb0,"ax",%progbits
	.global Func_02001eb0
	.thumb_func
Func_02001eb0:
	push {r5, r6, lr}
	movs r0, #123
	bl GameFlag_Test
	adds r6, r0, #0
	cmp r6, #0
	bne .L_02009ef6
	movs r0, #8
	bl Object_GetById
	adds r5, r0, #0
	movs r0, #162
	lsls r0, r0, #2
	bl GameFlag_Test
	adds r2, r5, #0
	adds r2, #85
	cmp r0, #0
	bne .L_02009ee6
	movs r3, #5
	strb r3, [r2]
	movs r2, #136
	ldr r3, [r5, #12]
	lsls r2, r2, #12
	adds r3, r3, r2
	str r3, [r5, #12]
	b .L_02009eec
.L_02009ee6:
	movs r3, #3
	strb r3, [r2]
	str r6, [r5, #12]
.L_02009eec:
	movs r1, #144
	ldr r0, .L_02009ef8
	lsls r1, r1, #3
	bl Scheduler_AddOrUpdateCallback
.L_02009ef6:
	pop {r5, r6, pc}
.L_02009ef8:
	.4byte Func_02001dd4
	.section .text.x02009efc,"ax",%progbits
	.global Func_02001efc
	.thumb_func
Func_02001efc:
	ldr r3, .L_02009f04
	str r0, [r3]
	bx lr
	.2byte 0x0000
.L_02009f04:
	.4byte Data_02007e24
	.section .text.x02009f08,"ax",%progbits
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
	bl Engine_MathRemainder
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
	bl Engine_MathRemainder
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
	.4byte gOverlayArea + 0x7e28
	.section .text.x02009fc4,"ax",%progbits
	.global Func_02001fc4
	.thumb_func
Func_02001fc4:
	push {r5, lr}
	ldr r2, .L_02009ff8
	movs r3, #1
	adds r5, r0, #0
	str r3, [r2]
	cmp r5, #2
	beq .L_02009fe2
	movs r1, #160
	lsls r1, r1, #19
	ldr r0, .L_02009ffc
	ldr r3, .L_0200a000
	adds r1, #160
	movs r2, #32
	mov lr, r3
	.2byte 0xf800
.L_02009fe2:
	cmp r5, #1
	bne .L_02009ff6
	ldr r3, .L_0200a004
	movs r2, #0
	movs r1, #144
	str r2, [r3]
	ldr r0, .L_0200a008
	lsls r1, r1, #3
	bl Scheduler_AddOrUpdateCallback
.L_02009ff6:
	pop {r5, pc}
.L_02009ff8:
	.4byte Data_02007e24
.L_02009ffc:
	.4byte gOverlayArea + 0x7e28
.L_0200a000:
	.4byte IwramCopyWords
.L_0200a004:
	.4byte Data_02007e20
.L_0200a008:
	.4byte Func_02001f08
	.section .text.x0200a00c,"ax",%progbits
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
	.4byte Data_02024000
.L_0200a230:
	.4byte gMapCellBuffer
.L_0200a234:
	.4byte 0xffff0000
.L_0200a238:
	.4byte 0xfffffe00
	.section .text.x0200a23c,"ax",%progbits
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
	bl Owner_GetState
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
	bl Engine_MathRemainder
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
	bl Owner_GetState
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
	bl Engine_MathRemainder
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
	bl Owner_GetState
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
	.4byte Data_02024000
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
	.4byte Data_030011bc
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
	.4byte Data_030011bc
	.section .text.x0200ace8,"ax",%progbits
	.global Func_02002ce8
	.thumb_func
Func_02002ce8:
	ldr r2, .L_0200acf8
	movs r3, #1
	ands r3, r0
	lsls r3, r3, #1
	adds r2, r2, r3
	movs r3, #22
	ldrsh r0, [r2, r3]
	bx lr
.L_0200acf8:
	.4byte Data_020023c4 + 0x88
	.section .text.x0200acfc,"ax",%progbits
	.global Func_02002cfc
	.thumb_func
Func_02002cfc:
	ldr r3, .L_0200ad04
	movs r2, #18
	ldrsh r0, [r3, r2]
	bx lr
.L_0200ad04:
	.4byte Data_020023c4 + 0x88
	.section .text.x0200ad08,"ax",%progbits
	.global Func_02002d08
	.thumb_func
Func_02002d08:
	push {r5, lr}
	adds r5, r0, #0
	bl Func_02002cfc
	cmp r0, #0
	beq .L_0200ad1c
	movs r0, #128
	lsls r0, r0, #2
	subs r0, r0, r5
	b .L_0200ad1e
.L_0200ad1c:
	adds r0, r5, #0
.L_0200ad1e:
	pop {r5, pc}
	.section .text.x0200ad20,"ax",%progbits
	.global Func_02002d20
	.thumb_func
Func_02002d20:
	push {r5, lr}
	adds r5, r0, #0
	bl Func_02002cfc
	cmp r0, #0
	beq .L_0200ad34
	movs r0, #64
	subs r0, r0, r5
	lsls r0, r0, #3
	b .L_0200ad36
.L_0200ad34:
	lsls r0, r5, #3
.L_0200ad36:
	pop {r5, pc}
	.section .text.x0200ad38,"ax",%progbits
	.global Func_02002d38
	.thumb_func
Func_02002d38:
	push {r5, lr}
	adds r5, r0, #0
	bl Func_02002cfc
	cmp r0, #0
	beq .L_0200ad4a
	negs r0, r5
	lsls r0, r0, #3
	b .L_0200ad4c
.L_0200ad4a:
	lsls r0, r5, #3
.L_0200ad4c:
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x0200ad50,"ax",%progbits
	.global Func_02002d50
	.thumb_func
Func_02002d50:
	push {r5, lr}
	adds r5, r0, #0
	bl Func_02002cfc
	cmp r0, #0
	beq .L_0200ad6c
	movs r0, #128
	movs r3, #255
	lsls r0, r0, #8
	lsls r3, r3, #8
	subs r0, r0, r5
	adds r3, #255
	ands r0, r3
	b .L_0200ad74
.L_0200ad6c:
	movs r0, #255
	lsls r0, r0, #8
	adds r0, #255
	ands r0, r5
.L_0200ad74:
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x0200ad78,"ax",%progbits
	.global Func_02002d78
	.thumb_func
Func_02002d78:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r8
	push {r5, r6}
	mov r10, r0
	adds r0, r1, #0
	mov r8, r3
	adds r6, r2, #0
	bl Func_02002d08
	adds r5, r0, #0
	mov r0, r8
	bl Func_02002d50
	adds r3, r0, #0
	lsls r5, r5, #16
	lsls r6, r6, #16
	lsls r3, r3, #16
	lsrs r3, r3, #16
	mov r0, r10
	adds r1, r5, #0
	adds r2, r6, #0
	bl Func_020058e8
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, pc}
	.section .text.x0200adb0,"ax",%progbits
	.global Func_02002db0
	.thumb_func
Func_02002db0:
	bx lr
	.2byte 0x0000
	.section .text.x0200adb4,"ax",%progbits
	.global Func_02002db4
	.thumb_func
Func_02002db4:
	push {r5, r6, lr}
	movs r5, #128
	lsls r5, r5, #8
	movs r1, #144
	movs r2, #132
	movs r6, #148
	adds r3, r5, #0
	lsls r6, r6, #1
	lsls r1, r1, #1
	lsls r2, r2, #1
	movs r0, #8
	bl Func_02002d78
	adds r1, r6, #0
	adds r3, r5, #0
	movs r0, #13
	movs r2, #248
	bl Func_02002d78
	movs r2, #140
	lsls r2, r2, #1
	movs r0, #14
	adds r1, r6, #0
	adds r3, r5, #0
	bl Func_02002d78
	pop {r5, r6, pc}
	.2byte 0x0000
	.section .text.x0200adec,"ax",%progbits
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
	.4byte 0x000029cd
.L_0200b228:
	.4byte 0x0001b333
	.section .text.x0200b22c,"ax",%progbits
	.global Func_0200322c
	.thumb_func
Func_0200322c:
	push {r5, lr}
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
	ldr r0, .L_0200b4ac
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
	movs r1, #128
	movs r2, #128
	movs r0, #4
	lsls r1, r1, #10
	lsls r2, r2, #9
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
	ldr r1, .L_0200b4b0
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
	movs r1, #128
	movs r2, #128
	movs r0, #8
	lsls r1, r1, #10
	lsls r2, r2, #9
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
	ldr r1, .L_0200b4b0
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
	pop {r5, pc}
	.2byte 0x0000
.L_0200b4ac:
	.4byte 0x000029ca
.L_0200b4b0:
	.4byte 0x0001b333
	.section .text.x0200b4b4,"ax",%progbits
	.global Func_020034b4
	.thumb_func
Func_020034b4:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r8
	push {r5, r6}
	bl Func_02005880
	movs r0, #0
	bl Func_020059e8
	movs r6, #136
	movs r0, #1
	movs r1, #1
	movs r2, #1
	lsls r6, r6, #1
	negs r0, r0
	negs r1, r1
	negs r2, r2
	movs r3, #0
	bl Motion_CamBounds
	adds r2, r6, #0
	movs r0, #4
	movs r1, #248
	movs r3, #0
	bl Func_02002d78
	movs r0, #7
	movs r1, #224
	movs r2, #248
	movs r3, #0
	bl Func_02002d78
	movs r3, #132
	lsls r3, r3, #1
	mov r10, r3
	mov r2, r10
	movs r0, #5
	movs r1, #232
	movs r3, #0
	bl Func_02002d78
	movs r3, #140
	lsls r3, r3, #1
	mov r8, r3
	mov r2, r8
	movs r0, #6
	movs r1, #224
	movs r3, #0
	bl Func_02002d78
	movs r5, #128
	movs r2, #128
	lsls r5, r5, #7
	lsls r2, r2, #1
	movs r0, #9
	movs r1, #248
	movs r3, #0
	bl Func_02002d78
	adds r1, r6, #0
	mov r2, r10
	adds r3, r5, #0
	adds r6, #16
	movs r0, #8
	bl Func_02002d78
	adds r1, r6, #0
	adds r3, r5, #0
	movs r0, #13
	movs r2, #248
	bl Func_02002d78
	adds r3, r5, #0
	mov r2, r8
	adds r1, r6, #0
	movs r0, #14
	bl Func_02002d78
	movs r0, #8
	movs r1, #6
	bl Object_SetModeById
	movs r0, #13
	movs r1, #6
	bl Object_SetModeById
	movs r1, #6
	movs r0, #14
	bl Object_SetModeById
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	movs r0, #30
	bl Battle_WaitMode0
	ldr r0, .L_0200b7c4
	bl Func_02005938
	movs r1, #2
	movs r0, #13
	bl ObjectMotion_SetVariantCallback
	movs r0, #4
	bl Task_Wait
	movs r1, #2
	movs r0, #14
	bl ObjectMotion_SetVariantCallback
	movs r0, #10
	bl Task_Wait
	movs r0, #8
	movs r1, #2
	bl Motion_SetVarCbAndRefresh
	movs r0, #8
	movs r1, #0
	bl Func_02005940
	movs r1, #7
	movs r0, #8
	bl Object_SetModeById
	movs r0, #15
	bl Battle_WaitMode0
	movs r1, #7
	movs r0, #13
	bl Object_SetModeById
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #7
	movs r0, #14
	bl Object_SetModeById
	movs r0, #5
	bl Battle_WaitMode0
	movs r5, #128
	movs r1, #1
	movs r0, #8
	bl Object_SetModeById
	lsls r5, r5, #8
	movs r0, #15
	bl Battle_WaitMode0
	adds r0, r5, #0
	bl Func_02002d50
	movs r2, #0
	adds r1, r0, #0
	movs r0, #8
	bl ObjectMotion_ArmCallback
	movs r1, #1
	movs r0, #13
	bl Object_SetModeById
	movs r0, #10
	bl Battle_WaitMode0
	adds r0, r5, #0
	bl Func_02002d50
	movs r2, #0
	adds r1, r0, #0
	movs r0, #13
	bl ObjectMotion_ArmCallback
	movs r1, #1
	movs r0, #14
	bl Object_SetModeById
	movs r0, #5
	bl Battle_WaitMode0
	adds r0, r5, #0
	bl Func_02002d50
	movs r2, #0
	adds r1, r0, #0
	movs r0, #14
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #8
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #8
	movs r1, #0
	bl Func_02005940
	movs r1, #128
	adds r2, r5, #0
	lsls r1, r1, #9
	movs r0, #8
	bl ObjectMotion_SetSpeedParameters
	movs r0, #32
	bl Func_02002d20
	mov r2, r10
	adds r1, r0, #0
	movs r0, #8
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r0, #9
	movs r1, #7
	movs r2, #0
	bl Object_LinkPair
	movs r1, #6
	movs r2, #0
	movs r0, #5
	bl Object_LinkPair
	movs r0, #8
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #9
	movs r1, #4
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #7
	movs r1, #4
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #6
	movs r1, #4
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r2, #0
	movs r1, #4
	movs r0, #5
	bl ObjectMotion_SetAngleToward
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #65
	bl Func_02005860
	movs r1, #0
	movs r0, #215
	bl PartyInventory_GiveItem
	movs r1, #8
	movs r2, #0
	movs r0, #4
	bl ObjectMotion_SetAngleToward
	movs r0, #8
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	strb r3, [r0]
	movs r0, #35
	bl Func_02002d20
	mov r2, r10
	adds r1, r0, #0
	movs r0, #8
	bl ObjectMotion_SetPositionAndReset
	movs r0, #1
	bl Battle_WaitMode0
	movs r0, #8
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	strb r3, [r0]
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #6
	movs r2, #10
	adds r1, #255
	movs r0, #8
	bl Func_02005960
	movs r1, #0
	movs r0, #8
	bl Func_02005940
	movs r0, #0
	bl Func_02002d50
	adds r1, r0, #0
	movs r0, #8
	bl Func_02005950
	movs r0, #13
	movs r1, #8
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r2, #0
	movs r0, #14
	movs r1, #8
	bl ObjectMotion_SetAngleToward
	movs r0, #8
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #8
	movs r1, #0
	bl Func_02005940
	movs r1, #3
	movs r0, #13
	bl Object_SetModeById
	movs r0, #3
	bl Task_Wait
	movs r0, #14
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #4
	movs r1, #9
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #7
	movs r1, #9
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #5
	movs r1, #9
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r2, #0
	movs r0, #6
	movs r1, #9
	bl ObjectMotion_SetAngleToward
	movs r0, #9
	movs r1, #0
	bl Func_02005940
	movs r1, #3
	movs r0, #4
	bl Object_SetModeById
	movs r0, #2
	bl Task_Wait
	movs r0, #6
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #7
	bl Object_SetModeById
	movs r0, #1
	bl Task_Wait
	movs r0, #5
	movs r1, #3
	bl Object_SetModeById
	movs r0, #30
	bl Task_Wait
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, pc}
.L_0200b7c4:
	.4byte 0x000029d1
	.section .text.x0200b7c8,"ax",%progbits
	.global Func_020037c8
	.thumb_func
Func_020037c8:
	push {r5, r6, lr}
	bl Func_02005880
	movs r0, #0
	bl Func_020059e8
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r0, r0
	negs r1, r1
	negs r2, r2
	movs r3, #0
	bl Motion_CamBounds
	movs r6, #128
	movs r2, #132
	lsls r6, r6, #8
	lsls r2, r2, #1
	movs r5, #140
	lsls r5, r5, #1
	adds r1, r2, #0
	adds r3, r6, #0
	movs r0, #8
	bl Func_02002d78
	adds r1, r5, #0
	adds r3, r6, #0
	movs r0, #13
	movs r2, #248
	bl Func_02002d78
	adds r1, r5, #0
	adds r2, r5, #0
	adds r3, r6, #0
	movs r0, #14
	bl Func_02002d78
	movs r2, #138
	lsls r2, r2, #1
	movs r0, #4
	movs r1, #240
	movs r3, #0
	bl Func_02002d78
	movs r0, #7
	movs r1, #216
	movs r2, #254
	movs r3, #0
	bl Func_02002d78
	movs r0, #5
	movs r1, #232
	movs r2, #244
	movs r3, #0
	bl Func_02002d78
	movs r2, #148
	lsls r2, r2, #1
	movs r3, #0
	movs r0, #6
	movs r1, #224
	bl Func_02002d78
	movs r0, #4
	movs r1, #38
	bl Object_SetModeById
	movs r0, #5
	movs r1, #19
	bl Object_SetModeById
	movs r0, #6
	movs r1, #19
	bl Object_SetModeById
	movs r0, #7
	movs r1, #19
	bl Object_SetModeById
	movs r1, #128
	lsls r1, r1, #7
	movs r0, #7
	bl Func_02005a48
	movs r0, #4
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #5
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #6
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #7
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	movs r0, #30
	bl Battle_WaitMode0
	ldr r0, .L_0200b8fc
	bl Func_02005938
	movs r0, #8
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #8
	movs r1, #0
	bl Func_02005940
	movs r1, #2
	movs r0, #4
	bl ObjectMotion_SetVariantCallback
	movs r0, #1
	bl Task_Wait
	movs r1, #2
	movs r0, #6
	bl ObjectMotion_SetVariantCallback
	movs r0, #2
	bl Task_Wait
	movs r0, #5
	movs r1, #2
	bl ObjectMotion_SetVariantCallback
	movs r0, #7
	movs r1, #2
	bl ObjectMotion_SetVariantCallback
	movs r0, #15
	bl Battle_WaitMode0
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200b8fc:
	.4byte 0x000029d0
	.section .text.x0200b900,"ax",%progbits
	.global Func_02003900
	.thumb_func
Func_02003900:
	push {r5, r6, lr}
	sub sp, #8
	bl Func_02005880
	movs r0, #0
	bl Func_020059e8
	ldr r3, .L_0200bd00
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	movs r2, #254
	ldr r0, [r3]
	movs r1, #0
	lsls r2, r2, #18
	bl Func_020058e0
	movs r1, #254
	movs r2, #254
	movs r0, #8
	lsls r1, r1, #18
	lsls r2, r2, #18
	bl Func_020058e0
	movs r5, #9
.L_0200b932:
	adds r0, r5, #0
	bl Object_GetById
	ldr r3, [r0, #8]
	cmp r3, #0
	beq .L_0200b944
	ldr r2, .L_0200bd04
	adds r3, r3, r2
	str r3, [r0, #8]
.L_0200b944:
	adds r5, #1
	cmp r5, #79
	ble .L_0200b932
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r2, r2
	movs r3, #0
	negs r0, r0
	negs r1, r1
	bl Motion_CamBounds
	ldr r1, .L_0200bd08
	movs r0, #13
	bl ObjectMotion_EnableActionAndSetCallback
	ldr r1, .L_0200bd0c
	movs r0, #14
	bl ObjectMotion_EnableActionAndSetCallback
	bl Event_SetStatus1c6
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #10
	lsls r1, r1, #7
	bl Func_02005970
	movs r0, #184
	movs r1, #1
	movs r2, #150
	lsls r0, r0, #16
	negs r1, r1
	lsls r2, r2, #18
	movs r3, #1
	bl Motion_CamBounds
	bl Func_02005980
	movs r0, #240
	movs r1, #1
	movs r2, #138
	lsls r0, r0, #15
	negs r1, r1
	lsls r2, r2, #18
	movs r3, #1
	bl Motion_CamBounds
	bl Func_02005980
	movs r0, #240
	movs r1, #1
	movs r2, #204
	lsls r0, r0, #15
	negs r1, r1
	lsls r2, r2, #17
	movs r3, #1
	bl Motion_CamBounds
	bl Func_02005980
	movs r0, #240
	movs r1, #1
	movs r2, #240
	lsls r2, r2, #15
	movs r3, #1
	negs r1, r1
	lsls r0, r0, #15
	bl Motion_CamBounds
	ldr r0, .L_0200bd10
	bl Func_02005938
	movs r0, #8
	movs r1, #0
	bl Func_02005940
	bl Func_02005980
	movs r0, #216
	movs r1, #1
	movs r2, #240
	movs r3, #1
	lsls r2, r2, #15
	lsls r0, r0, #16
	negs r1, r1
	bl Motion_CamBounds
	bl Func_02005980
	movs r0, #8
	movs r1, #0
	bl Func_02005940
	movs r0, #4
	movs r1, #0
	bl Func_02005940
	ldr r1, .L_0200bd14
	movs r0, #13
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #13
	bl Object_RefreshSelectorById
	movs r1, #1
	movs r0, #13
	bl Object_SetModeById
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #7
	lsls r2, r2, #6
	movs r0, #13
	bl ObjectMotion_SetSpeedParameters
	movs r0, #13
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r5, #254
	adds r3, r5, #0
	ands r3, r2
	strb r3, [r0]
	movs r1, #0
	movs r2, #8
	movs r0, #13
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #1
	bl Battle_WaitMode0
	movs r0, #13
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	movs r6, #1
	orrs r3, r6
	strb r3, [r0]
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #9
	movs r0, #13
	lsls r1, r1, #10
	bl ObjectMotion_SetSpeedParameters
	movs r0, #13
	movs r1, #5
	bl Object_SetModeById
	movs r2, #40
	movs r1, #0
	negs r2, r2
	movs r0, #13
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r0, #13
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #103
	bl Func_02005a68
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #11
	lsls r2, r2, #9
	lsls r0, r0, #10
	bl Func_02005838
	movs r0, #13
	bl Object_GetById
	movs r3, #128
	lsls r3, r3, #11
	movs r2, #230
	str r3, [r0, #40]
	movs r1, #1
	movs r0, #1
	lsls r2, r2, #8
	negs r1, r1
	adds r2, #102
	negs r0, r0
	bl Func_02005838
	movs r0, #13
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	movs r1, #0
	ands r5, r3
	movs r2, #16
	strb r5, [r0]
	movs r0, #13
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #1
	bl Battle_WaitMode0
	movs r0, #13
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	movs r5, #8
	orrs r6, r3
	strb r6, [r0]
	movs r0, #60
	bl Battle_WaitMode0
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #13
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #13
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #8
	movs r1, #0
	bl Func_02005940
	movs r1, #160
	movs r2, #0
	movs r0, #13
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r1, #3
	movs r0, #13
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #128
	movs r2, #128
	movs r0, #13
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r1, #16
	movs r2, #16
	movs r0, #13
	negs r1, r1
	negs r2, r2
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #13
	bl ObjectMotion_ArmCallback
	movs r0, #124
	bl Func_02005a68
	movs r1, #6
	movs r0, #9
	bl Object_SetModeById
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #8
	movs r1, #0
	bl Func_02005940
	movs r0, #13
	movs r1, #200
	movs r2, #168
	bl ObjectMotion_SetPositionAndReset
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #13
	bl ObjectMotion_ArmCallback
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #125
	bl Func_02005a68
	movs r1, #5
	movs r0, #9
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #138
	bl Func_02005a68
	movs r3, #48
	str r3, [sp, #0]
	movs r6, #3
	movs r0, #127
	movs r1, #3
	movs r2, #1
	movs r3, #2
	str r6, [sp, #4]
	bl Func_02005820
	movs r3, #14
	str r3, [sp, #0]
	movs r2, #1
	movs r3, #1
	movs r1, #127
	movs r0, #0
	str r5, [sp, #4]
	bl Func_02005818
	movs r0, #20
	bl Battle_WaitMode0
	ldr r1, .L_0200bd18
	movs r0, #13
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #13
	bl Object_RefreshSelectorById
	movs r0, #123
	bl Func_02005a68
	movs r0, #8
	movs r1, #0
	bl Func_02005940
	ldr r1, .L_0200bd1c
	movs r0, #14
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #164
	movs r1, #1
	movs r2, #240
	movs r3, #1
	lsls r2, r2, #15
	lsls r0, r0, #17
	negs r1, r1
	bl Motion_CamBounds
	bl Func_02005980
	movs r0, #8
	movs r1, #0
	bl Func_02005940
	movs r0, #8
	movs r1, #0
	bl Func_02005940
	movs r1, #224
	movs r2, #0
	movs r0, #14
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r1, #3
	movs r0, #14
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #128
	movs r2, #128
	movs r0, #14
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r2, #16
	movs r0, #14
	movs r1, #16
	negs r2, r2
	bl ObjectMotion_CommitPositionAndActivate
	movs r2, #0
	movs r1, #0
	movs r0, #14
	bl ObjectMotion_ArmCallback
	movs r0, #124
	bl Func_02005a68
	movs r0, #11
	movs r1, #6
	bl Object_SetModeById
	movs r1, #6
	movs r0, #12
	bl Object_SetModeById
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #175
	movs r0, #14
	lsls r1, r1, #1
	movs r2, #168
	bl ObjectMotion_SetPositionAndReset
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #14
	bl ObjectMotion_ArmCallback
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #125
	bl Func_02005a68
	movs r1, #5
	movs r0, #11
	bl Object_SetModeById
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #182
	movs r0, #14
	lsls r1, r1, #1
	movs r2, #168
	bl ObjectMotion_SetPositionAndReset
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #14
	bl ObjectMotion_ArmCallback
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #125
	bl Func_02005a68
	movs r1, #5
	movs r0, #12
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #138
	bl Func_02005a68
	movs r3, #53
	str r3, [sp, #0]
	movs r0, #127
	movs r1, #3
	movs r2, #1
	movs r3, #2
	str r6, [sp, #4]
	b .L_0200bd20
.L_0200bd00:
	.4byte gPartyState
.L_0200bd04:
	.4byte 0xff700000
.L_0200bd08:
	.4byte Data_02005c88
.L_0200bd0c:
	.4byte Data_02005be8
.L_0200bd10:
	.4byte 0x0000299a
.L_0200bd14:
	.4byte Data_02005d28
.L_0200bd18:
	.4byte Data_02005dc8
.L_0200bd1c:
	.4byte Data_02005e40
.L_0200bd20:
	bl Func_02005820
	movs r3, #19
	str r3, [sp, #0]
	movs r1, #127
	movs r3, #1
	movs r2, #1
	movs r0, #0
	str r5, [sp, #4]
	bl Func_02005818
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #160
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #14
	bl ObjectMotion_ArmCallback
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #14
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #8
	movs r1, #0
	bl Func_02005940
	movs r0, #204
	movs r1, #1
	movs r2, #240
	lsls r2, r2, #15
	movs r3, #1
	lsls r0, r0, #17
	negs r1, r1
	bl Motion_CamBounds
	bl Func_02005980
	movs r0, #8
	movs r1, #0
	bl Func_02005940
	ldr r1, .L_0200bd9c
	movs r0, #14
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #14
	bl Object_RefreshSelectorById
	movs r1, #0
	movs r0, #8
	bl Func_02005940
	movs r0, #10
	bl Func_02005990
	add sp, #8
	pop {r5, r6, pc}
.L_0200bd9c:
	.4byte Data_02005ed4
	.section .text.x0200bda0,"ax",%progbits
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
	bl Engine_MathModulo
	movs r1, #3
	bl IwramUnsignedDivideEntry
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
	.4byte Data_0300122c
.L_0200c17c:
	.4byte 0x40002000
.L_0200c180:
	.4byte IwramMulQ16
.L_0200c184:
	.4byte 0xfffe0000
	.section .text.x0200c188,"ax",%progbits
	.global Func_02004188
	.thumb_func
Func_02004188:
	push {r5, r6, r7, lr}
	movs r0, #10
	adds r0, #255
	adds r7, r1, #0
	ldr r6, .L_0200c210
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200c1a6
	movs r1, #228
	ldr r3, .L_0200c214
	adds r0, r6, #0
	lsls r1, r1, #1
	mov lr, r3
	.2byte 0xf800
.L_0200c1a6:
	movs r0, #128
	lsls r0, r0, #4
	bl Runtime_BumpAllocateAlternatePool
	adds r5, r0, #0
	adds r1, r5, #0
	ldr r0, .L_0200c218
	bl Func_02005770
	bl Resource_FindFreeEntry
	movs r1, #128
	strh r0, [r6]
	lsls r0, r0, #16
	adds r2, r5, #0
	lsls r1, r1, #4
	asrs r0, r0, #16
	bl VramBlock_LoadCached
	adds r0, r5, #0
	bl Sys_Free
	movs r0, #240
	lsls r0, r0, #2
	bl Runtime_BumpAllocateAlternatePool
	movs r2, #226
	lsls r2, r2, #1
	movs r1, #128
	adds r3, r6, r2
	lsls r1, r1, #3
	str r0, [r3]
	adds r1, #141
	strh r7, [r6, #2]
	ldr r0, .L_0200c21c
	bl Scheduler_AddOrUpdateCallback
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_0200c220
	bl Scheduler_AddOrUpdateCallback
	bl Func_02005a40
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #64
	orrs r3, r2
	strb r3, [r0]
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200c210:
	.4byte Data_020023c4 + 0x188
.L_0200c214:
	.4byte IwramClearWords
.L_0200c218:
	.4byte Data_02005fa0
.L_0200c21c:
	.4byte Func_02003da0
.L_0200c220:
	.4byte Func_02003f68
	.section .text.x0200c224,"ax",%progbits
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
	bl ObjectDispatch_SetSingleChildField26
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
	.4byte Data_02024000
.L_0200c414:
	.4byte gPartyState
	.section .text.x0200c418,"ax",%progbits
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
	.section .text.x0200c68c,"ax",%progbits
	.global Func_0200468c
	.thumb_func
Func_0200468c:
	push {r5, lr}
	adds r5, r0, #0
	bl Object_GetById
	movs r3, #3
	adds r0, #92
	strb r3, [r0]
	adds r0, r5, #0
	bl Func_02005958
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x0200c6a4,"ax",%progbits
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
	.4byte Data_02024000
	.section .text.x0200c80c,"ax",%progbits
	.global Func_0200480c
	.thumb_func
Func_0200480c:
	push {r5, lr}
	adds r5, r1, #0
	ldr r1, [r5, #8]
	ldr r2, [r5, #12]
	ldr r3, [r5, #16]
	bl Object_SetPositionAndResetMotion
	adds r0, r5, #0
	movs r1, #0
	movs r2, #0
	movs r3, #0
	bl Object_SetPositionAndResetMotion
	pop {r5, pc}
	.section .text.x0200c828,"ax",%progbits
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
	bl Engine_MathDivide
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
	bl ObjectDispatch_SetSingleChildField26
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
	bl ObjectDispatch_SetSingleChildField26
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
	.4byte Data_02024000
.L_0200cd68:
	.4byte 0x000fffff
	.section .text.x0200cd6c,"ax",%progbits
	.global Func_02004d6c
	.thumb_func
Func_02004d6c:
	push {lr}
	ldr r3, .L_0200cd80
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r2, [r3]
	bl Func_02004ce0
	pop {pc}
	.2byte 0x0000
.L_0200cd80:
	.4byte gPartyState
	.section .text.x0200cd84,"ax",%progbits
	.global Func_02004d84
	.thumb_func
Func_02004d84:
	push {lr}
	ldr r4, .L_0200cda8
	adds r0, #8
	movs r3, #0
	strb r3, [r0]
	movs r2, #7
	subs r0, #1
.L_0200cd92:
	movs r3, #15
	ands r3, r1
	ldrb r3, [r4, r3]
	subs r2, #1
	strb r3, [r0]
	lsrs r1, r1, #4
	subs r0, #1
	cmp r2, #0
	bge .L_0200cd92
	pop {pc}
	.2byte 0x0000
.L_0200cda8:
	.4byte Data_020063ac
	.section .text.x0200cddc,"ax",%progbits
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
	.4byte Data_02003874
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
	bl Sys_Free
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
	.4byte Data_0300122c
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
	bl Sys_Free
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
	.section .text.x0200d688,"ax",%progbits
	.global Func_02005688
	.thumb_func
Func_02005688:
	push {r5, r6, r7, lr}
	movs r3, #192
	adds r7, r0, #0
	lsls r3, r3, #18
	movs r0, #10
	adds r3, #236
	adds r0, #255
	ldr r6, [r3]
	ldr r5, .L_0200d6bc
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200d6ae
	adds r3, r6, #0
	adds r3, #228
	ldrh r3, [r3]
	strh r7, [r5, #2]
	strh r3, [r5, #4]
	strh r0, [r5, #8]
.L_0200d6ae:
	movs r1, #128
	lsls r1, r1, #3
	adds r1, #133
	ldr r0, .L_0200d6c0
	bl Scheduler_AddOrUpdateCallback
	pop {r5, r6, r7, pc}
.L_0200d6bc:
	.4byte gSceneState
.L_0200d6c0:
	.4byte Func_02004ddc
	.section .text.x0200d6c4,"ax",%progbits
	.global Func_020056c4
	.thumb_func
Func_020056c4:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #236
	ldr r3, [r3]
	ldr r2, .L_0200d6e4
	movs r1, #0
	adds r3, #234
	strh r1, [r3]
	movs r3, #1
	strh r0, [r2, #10]
	strh r1, [r2, #6]
	strh r3, [r2]
	bl Func_02004ddc
	pop {pc}
.L_0200d6e4:
	.4byte gSceneState
	.section .text.x0200d6e8,"ax",%progbits
	.global Func_020056e8
	.thumb_func
Func_020056e8:
	ldr r3, .L_0200d700
	movs r0, #1
	movs r1, #0
	ldrsh r2, [r3, r1]
	movs r3, #9
	eors r2, r3
	negs r3, r2
	orrs r3, r2
	lsrs r3, r3, #31
	subs r0, r0, r3
	bx lr
	.2byte 0x0000
.L_0200d700:
	.4byte gSceneState
	.4byte 0x00004770
	.section .rodata.x0200da70,"a",%progbits
.L_0200da70:
	.4byte 0x0000002e
	.4byte Func_02000058
	.4byte 0x00000011
	.global Data_02005a7c
Data_02005a7c:
	.4byte 0x0000002e
	.4byte Func_02000694
	.4byte 0x00000011
	.global Data_02005a88
Data_02005a88:
	.4byte 0x14151213
.L_0200da8c:
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000016
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.4byte 0x00000011
.L_0200dac8:
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000002c
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.4byte 0x00000011
.L_0200db04:
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000007e
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.4byte 0x00000011
	.4byte 0x00000011
	.global Data_02005b44
Data_02005b44:
	.4byte 0x00000017
	.4byte 0x00000003
	.4byte 0x00001000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0x00000000
	.global Data_02005b64
Data_02005b64:
	.4byte Func_02001904
	.4byte Func_02001974
	.4byte Func_02001a70
	.4byte Func_02001afc
	.4byte Func_02001bdc
	.4byte Func_02001bec
	.4byte Func_02001c6c
	.4byte Func_02001d8c
	.global Data_02005b84
Data_02005b84:
	.4byte 0x000001c2
	.4byte 0x000001c3
	.4byte 0x000001c4
	.4byte 0x000001c5
	.4byte 0x000001c6
	.4byte 0x000001c7
	.4byte 0x000001c8
	.4byte 0x000001c9
	.4byte 0x000001ca
	.4byte 0x000001cb
	.global Data_02005bac
Data_02005bac:
	.4byte 0x00300001
	.4byte 0x0030000f
	.4byte 0x0030001d
	.4byte 0x00460001
	.4byte 0x0046000f
	.4byte 0x0046001d
	.4byte 0x005c0001
	.4byte 0x005c000f
	.global Data_02005bcc
Data_02005bcc:
	.4byte 0x03020100
	.4byte 0x05040404
	.2byte 0x0706
	.global Data_02005bd6
Data_02005bd6:
	.2byte 0x0100
	.4byte 0x04040302
	.4byte 0x07060504
	.global Data_02005be0
Data_02005be0:
	.4byte 0x07030200
	.4byte 0x06050401
	.global Data_02005be8
Data_02005be8:
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00018000
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00010000
	.4byte 0x00000016
	.4byte 0x00000005
	.4byte 0x01380000
	.4byte 0x00000016
	.4byte 0x00000007
	.4byte 0x02a80000
	.4byte 0x00000027
	.4byte 0x00000005
	.4byte 0x00000004
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x02680000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x02680000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01a80000
	.4byte 0x00000000
	.4byte 0x02580000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01a80000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x00000001
	.4byte 0x00000011
	.global Data_02005c88
Data_02005c88:
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00018000
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00010000
	.4byte 0x00000016
	.4byte 0x00000005
	.4byte 0x00e80000
	.4byte 0x00000016
	.4byte 0x00000007
	.4byte 0x02a80000
	.4byte 0x00000027
	.4byte 0x00000005
	.4byte 0x00000004
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x02680000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x02680000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x02580000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x00000001
	.4byte 0x00000011
	.global Data_02005d28
Data_02005d28:
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00018000
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00010000
	.4byte 0x00000016
	.4byte 0x00000005
	.4byte 0x00780000
	.4byte 0x00000016
	.4byte 0x00000007
	.4byte 0x01280000
	.4byte 0x00000027
	.4byte 0x00000005
	.4byte 0x00000004
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000c000
	.4byte 0x00000011
	.global Data_02005dc8
Data_02005dc8:
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00018000
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00010000
	.4byte 0x00000027
	.4byte 0x00000005
	.4byte 0x00000004
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x00000016
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000011
	.global Data_02005e40
Data_02005e40:
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00010000
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00008000
	.4byte 0x00000016
	.4byte 0x00000005
	.4byte 0x01580000
	.4byte 0x00000016
	.4byte 0x00000007
	.4byte 0x00b80000
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00006000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x00000011
	.global Data_02005ed4
Data_02005ed4:
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00018000
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00010000
	.4byte 0x00000027
	.4byte 0x00000005
	.4byte 0x00000004
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01d80000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000a000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00006000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x00000011
	.global Data_02005fa0
Data_02005fa0:
	.4byte 0xc13c0100
	.4byte 0xb9d2cf52
	.4byte 0x13465bb3
	.4byte 0x5afce9e8
	.4byte 0xafd1ba81
	.4byte 0x9a39eb47
	.4byte 0x7445b435
	.4byte 0x2ff63444
	.4byte 0x74759d7d
	.4byte 0x581ed0e8
	.4byte 0x499ece98
	.4byte 0x66cc9bb5
	.4byte 0x28a91658
	.4byte 0xd6468754
	.4byte 0xb8fc3ca3
	.4byte 0x0394251c
	.4byte 0xa3921e4e
	.4byte 0xed0a619d
	.4byte 0x290915e5
	.4byte 0xd0d9ce84
	.4byte 0xcb0f067c
	.4byte 0x3a788051
	.4byte 0x4d0804f7
	.4byte 0xe32e6224
	.4byte 0x26d300f1
	.4byte 0xc780ce20
	.4byte 0x883c5604
	.4byte 0x991f8fbc
	.4byte 0x64fd4ba6
	.4byte 0x5026593a
	.4byte 0x42e4728b
	.4byte 0x4d0a3424
	.4byte 0xbe74d99b
	.4byte 0xe6f939cd
	.4byte 0xb126f91e
	.4byte 0xf32807ec
	.4byte 0x7c84cbcd
	.4byte 0xdc069263
	.4byte 0x984841e2
	.4byte 0x998e2689
	.4byte 0xd98df5e0
	.4byte 0x7ce0c6f9
	.4byte 0x70784b43
	.4byte 0xf7f6bc36
	.4byte 0xc7b95e4d
	.4byte 0xe7719be3
	.4byte 0x17a9ad75
	.4byte 0x586f8f25
	.4byte 0x75fcd4d3
	.4byte 0x08068958
	.4byte 0x9c9a2b93
	.4byte 0xfbcfa216
	.4byte 0x7e458240
	.4byte 0x86f9f834
	.4byte 0xfc221ebe
	.4byte 0xef811f5e
	.4byte 0x813ce478
	.4byte 0x242f015d
	.4byte 0xc6de6c1d
	.4byte 0x6faf3beb
	.4byte 0x5013be3c
	.4byte 0xf8f0a8bc
	.4byte 0x78c181c6
	.4byte 0xcf386e27
	.4byte 0xde5c3c49
	.4byte 0x6677cfc8
	.4byte 0x2a0df303
	.4byte 0x8f8df3f1
	.4byte 0xdf1e77c1
	.4byte 0x3e78c038
	.4byte 0x3de7e014
	.4byte 0x638c3f8f
	.4byte 0x4481687e
	.4byte 0x401077ce
	.4byte 0xf831f014
	.4byte 0x36800cc6
	.4byte 0x7d8b83c0
	.4byte 0x80cfbf02
	.4byte 0xe1c3400a
	.4byte 0xb40d23c6
	.4byte 0xde213ce2
	.4byte 0x9e3c2f19
	.4byte 0xe39df5f3
	.4byte 0xa8c6f9ae
	.4byte 0x8291fe62
	.4byte 0x80556f9c
	.4byte 0x88e0f404
	.4byte 0x38c0bf22
	.4byte 0xe8dadf3f
	.4byte 0xe01df023
	.4byte 0x102cef93
	.4byte 0x12c8cc41
	.4byte 0xbe40f187
	.4byte 0x39f09e71
	.4byte 0xb7dfcef8
	.4byte 0x0b10cf5e
	.4byte 0x3e012227
	.4byte 0x0e4fbaf0
	.4byte 0x344d6c12
	.4byte 0xc30f8e72
	.4byte 0xf8225e77
	.4byte 0x3bebd6fb
	.4byte 0xbdc9e3ef
	.4byte 0x3cf1a54a
	.4byte 0x323dcfe0
	.4byte 0x859f5350
	.4byte 0x38339cab
	.4byte 0xf5e0510f
	.4byte 0x3410af12
	.4byte 0x1f8088f5
	.4byte 0x9fbaf811
	.4byte 0xf19223df
	.4byte 0x607f04a3
	.4byte 0xadf82f92
	.4byte 0x04abd09e
	.4byte 0xc14f8168
	.4byte 0xaf1efc75
	.4byte 0xf82f19f3
	.4byte 0x301f82fa
	.4byte 0xd7cc2978
	.4byte 0xe0bebc73
	.4byte 0xc17dfdfd
	.4byte 0x033e31e3
	.4byte 0x1f45f3f4
	.4byte 0x18ebc7bf
	.4byte 0x043e08c6
	.4byte 0x60fe70ff
	.4byte 0x904fe2be
	.4byte 0xbaf1735e
	.4byte 0xbb803205
	.4byte 0xa18451f8
	.4byte 0xc00d1f80
	.4byte 0xf0ba1e1e
	.4byte 0x9f9af819
	.4byte 0xc17d79e3
	.4byte 0x1f7008cf
	.4byte 0x2df20be7
	.4byte 0x31f1147f
	.4byte 0xc04f8120
	.4byte 0x32147c17
	.4byte 0x5e1eefe1
	.4byte 0x7c2640a1
	.4byte 0x181fd39d
	.4byte 0xefe15ebc
	.4byte 0x67053ea8
	.4byte 0x8e7809f0
	.4byte 0xd1fc1bc7
	.4byte 0xfc0bd7c0
	.4byte 0x4aebc0dd
	.4byte 0x39f48a5e
	.4byte 0xc10f8eb8
	.4byte 0xbe147c63
	.4byte 0x3e1defe0
	.4byte 0x49f24f08
	.4byte 0x00b382f9
	.4byte 0xf82f891f
	.4byte 0xe3240ab9
	.4byte 0x0c3ea7e7
	.4byte 0x2009f2ef
	.4byte 0xf907af8a
	.4byte 0xc688dc18
	.4byte 0x1e07e087
	.4byte 0xe754707f
	.4byte 0x3f13b043
	.4byte 0xb851f3b3
	.4byte 0x09008f99
	.4byte 0xebeb483c
	.4byte 0x4b053e49
	.4byte 0xf3f2c632
	.4byte 0x3e026724
	.4byte 0xa3f2ef0e
	.4byte 0xf09a5f2b
	.4byte 0x00003efe
	.global Data_02006254
Data_02006254:
	.4byte 0x00000000
	.4byte 0x00000021
	.4byte 0x00000016
	.4byte 0x00000026
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.global Data_02006284
Data_02006284:
	.4byte 0x00000000
	.4byte 0x0000002c
	.4byte 0x00000016
	.4byte 0x00000026
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.global Data_020062b4
Data_020062b4:
	.4byte 0x377f10a0
	.4byte 0x121722bb
	.4byte 0x0ccc1172
	.4byte 0x7df07ef7
	.4byte 0x610b7ce7
	.4byte 0x50007c00
	.4byte 0x00147fff
	.4byte 0x2f1f0000
	.global Data_020062d4
Data_020062d4:
	.4byte 0x04060100
	.4byte 0x06e21008
	.4byte 0x44bad8c7
	.4byte 0x1dfaab43
	.4byte 0x8b8c4623
	.4byte 0x0b2ef778
	.4byte 0x01d0f313
	.4byte 0x111ae0ee
	.4byte 0x1bd5eee2
	.4byte 0xad56ab63
	.4byte 0x46239571
	.4byte 0xfc318244
	.4byte 0x5aac0e23
	.4byte 0x8486a2b5
	.4byte 0x0463843e
	.4byte 0xcaaa595a
	.4byte 0x157f9552
	.4byte 0x18214fa5
	.4byte 0x32bbb121
	.4byte 0x094ceaad
	.4byte 0xc6071c7c
	.4byte 0xc060b551
	.4byte 0x8c4471c7
	.4byte 0x06237627
	.4byte 0x1859f456
	.4byte 0x818e5893
	.4byte 0xfe018a92
	.4byte 0x63083e05
	.4byte 0x61131804
	.4byte 0xc239114d
	.4byte 0xcf8181e8
	.4byte 0x18028101
	.4byte 0x0501bd57
	.4byte 0xc4606351
	.4byte 0xeb1e31d0
	.4byte 0x7041167c
	.4byte 0xe4d59d1d
	.4byte 0x75761cf6
	.4byte 0x99d54eef
	.4byte 0xd0741bc3
	.4byte 0xd87bdd91
	.4byte 0x66403da5
	.4byte 0xf66782cb
	.4byte 0xd877e03a
	.4byte 0x97641fec
	.4byte 0x7640e601
	.4byte 0xcf81c6ad
	.4byte 0xf891f720
	.4byte 0x717cdc12
	.4byte 0x7c54f882
	.4byte 0x0e62cee9
	.4byte 0xee209704
	.4byte 0xe2f921f1
	.4byte 0x00000003
	.global Data_020063ac
Data_020063ac:
	.4byte 0x33323130
	.4byte 0x37363534
	.4byte 0x42413938
	.4byte 0x46454443
	.4byte 0x00000000
	.global gSceneEntrances
gSceneEntrances:
	.4byte 0xffff0000
	.4byte 0x000001b8
	.4byte 0x800000b8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0009
	.4byte 0x00000158
	.4byte 0x80000108
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
	.4byte 0x000000f7
	.4byte 0x001030f7
	.4byte 0x002040f7
	.4byte 0x003010f7
	.4byte 0x004020f7
	.4byte 0x005010f1
	.4byte 0x006020f1
	.4byte 0x007010f8
	.4byte 0x008020f8
	.4byte 0x009090f1
	.4byte 0x00a0b0f1
	.4byte 0x000000f9
	.4byte 0x001020f9
	.4byte 0x002010f9
	.4byte 0x003030f8
	.4byte 0x004010fa
	.4byte 0x000000fa
	.4byte 0x001040f9
	.4byte 0x000000f8
	.4byte 0x001210f7
	.4byte 0x002220f7
	.4byte 0x003030f9
	.4byte 0x0041e0f4
	.4byte 0x0051f0f4
	.4byte 0x000001ff
	.global Data_0200646c
Data_0200646c:
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
.L_0200e484:
	.4byte 0xffff0029
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0xffff01e7
	.4byte 0x00000001
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00020000
	.4byte 0xffff01e7
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00020000
	.4byte 0xffff01e7
	.4byte 0x00000001
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00020000
	.4byte 0xffff01e7
	.4byte 0x00000001
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00020000
	.4byte 0xffff0028
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0xffff0028
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0xffff0138
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x00020000
	.4byte 0xffff0138
	.4byte 0x00000001
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x01020000
	.4byte 0xffff0138
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x01020000
	.4byte 0xffff0138
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x01020000
	.4byte 0xffff0138
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x01020000
	.4byte 0xffff0133
	.4byte .L_0200da70
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x01900000
	.4byte 0x00020000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
.L_0200e5d4:
	.4byte 0xffff0029
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0xffff01e7
	.4byte 0x00000001
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00020000
	.4byte 0xffff01e7
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00020000
	.4byte 0xffff01e7
	.4byte 0x00000001
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00020000
	.4byte 0xffff01e7
	.4byte 0x00000001
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00020000
	.4byte 0xffff0028
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0xffff0028
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0xffff0138
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00020000
	.4byte 0xffff0138
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x01020000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x01b80000
	.4byte 0x00020000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x01b80000
	.4byte 0x00020000
	.4byte 0xffff0130
	.4byte 0x00000001
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x00020000
	.4byte 0xffff0130
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x01020000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
.L_0200e754:
	.4byte 0xffff0029
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0xffff01e7
	.4byte 0x00000001
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00020000
	.4byte 0xffff01e7
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00020000
	.4byte 0xffff01e7
	.4byte 0x00000001
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00020000
	.4byte 0xffff01e7
	.4byte 0x00000001
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00020000
	.4byte 0xffff0028
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0xffff0028
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0xffff0138
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00020000
	.4byte 0xffff0138
	.4byte 0x00000001
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x01020000
	.4byte 0xffff0138
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x01b80000
	.4byte 0x01020000
	.4byte 0xffff0138
	.4byte 0x00000001
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x01020000
	.4byte 0xffff0138
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x01020000
	.4byte 0xffff0133
	.4byte .L_0200da70
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x01b00000
	.4byte 0x00020000
	.4byte 0xffff0133
	.4byte .L_0200da70
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x01b00000
	.4byte 0x01020000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
.L_0200e8bc:
	.4byte 0xffff0029
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0xffff01e7
	.4byte 0x00000001
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00020000
	.4byte 0xffff01e7
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00020000
	.4byte 0xffff01e7
	.4byte 0x00000001
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00020000
	.4byte 0xffff01e7
	.4byte 0x00000001
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00020000
	.4byte 0xffff0028
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0xffff0028
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0xffff0138
	.4byte 0x00000001
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x00020000
	.4byte 0xffff0138
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x01020000
	.4byte 0xffff0138
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x01020000
	.4byte 0xffff0196
	.4byte 0x00000001
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00020000
	.4byte 0xffff0196
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00020000
	.4byte 0xffff0196
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00020000
	.4byte 0xffff0196
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00020000
	.4byte 0xffff0133
	.4byte .L_0200da70
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x01d00000
	.4byte 0x00020000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
.L_0200ea3c:
	.4byte 0xffff0029
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0xffff01e7
	.4byte 0x00000001
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00020000
	.4byte 0xffff01e7
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00020000
	.4byte 0xffff01e7
	.4byte 0x00000001
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00020000
	.4byte 0xffff01e7
	.4byte 0x00000001
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00020000
	.4byte 0xffff0028
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0xffff0028
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0xffff0194
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x01b80000
	.4byte 0x00020000
	.4byte 0xffff0194
	.4byte 0x00000001
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x01b80000
	.4byte 0x00020000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
.L_0200eb2c:
	.4byte 0xffff0029
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0xffff01e7
	.4byte 0x00000001
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00020000
	.4byte 0xffff01e7
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00020000
	.4byte 0xffff01e7
	.4byte 0x00000001
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00020000
	.4byte 0xffff01e7
	.4byte 0x00000001
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00020000
	.4byte 0xffff0028
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0xffff0028
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0xffff0138
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00020000
	.4byte 0xffff0105
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00020000
	.4byte 0xffff0105
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x01b80000
	.4byte 0x00020000
	.4byte 0xffff0104
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x00020000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
.L_0200ec4c:
	.4byte 0xffff0029
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0xffff01e7
	.4byte 0x00000001
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00020000
	.4byte 0xffff01e7
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00020000
	.4byte 0xffff01e7
	.4byte 0x00000001
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00020000
	.4byte 0xffff01e7
	.4byte 0x00000001
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00020000
	.4byte 0xffff0028
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0xffff0028
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0xffff0138
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x01d80000
	.4byte 0x00020000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x00020000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x01020000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x01020000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x01b80000
	.4byte 0x01020000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x01b80000
	.4byte 0x01020000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
.L_0200ed9c:
	.4byte 0xffff0029
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0xffff01e7
	.4byte 0x00000001
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00020000
	.4byte 0xffff01e7
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00020000
	.4byte 0xffff01e7
	.4byte 0x00000001
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00020000
	.4byte 0xffff01e7
	.4byte 0x00000001
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00020000
	.4byte 0xffff0028
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0xffff0028
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00020000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x01020000
	.4byte 0xffff0133
	.4byte .L_0200da70
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x01500000
	.4byte 0x00020000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02006ea4
Data_02006ea4:
	.4byte 0xffff0149
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x0002c000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02006ed4
Data_02006ed4:
	.4byte 0x007b00f6
	.4byte 0x00000001
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02006f04
Data_02006f04:
	.4byte 0xffff0004
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0xffff0005
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0xffff0006
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0xffff0007
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0xffff0029
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x00020000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0xffff0028
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0xffff0028
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02007024
Data_02007024:
	.4byte .L_0200e484
	.4byte .L_0200e5d4
	.4byte .L_0200e754
	.4byte .L_0200e8bc
	.4byte .L_0200ea3c
	.4byte .L_0200eb2c
	.4byte .L_0200ec4c
	.4byte .L_0200ed9c
	.global Data_02007044
Data_02007044:
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
.L_0200f050:
	.4byte 0x00000002
	.4byte 0xffff0001
	.4byte Func_02000138
	.4byte 0x00000002
	.4byte 0xffff0002
	.4byte Func_02000138
	.4byte 0x00000002
	.4byte 0xffff0003
	.4byte Func_02000138
	.4byte 0x00000002
	.4byte 0xffff0004
	.4byte Func_02000138
	.4byte 0x00000002
	.4byte 0x1281000a
	.4byte Func_02000418
	.4byte 0x00000002
	.4byte 0x0281000b
	.4byte Func_02000330
	.4byte 0x00000002
	.4byte 0x12810014
	.4byte Func_02000418
	.4byte 0x00000002
	.4byte 0x02810015
	.4byte Func_02000330
	.4byte 0x00000002
	.4byte 0x1284001e
	.4byte Data_02000628 + 0x1
	.4byte 0x00000002
	.4byte 0x0284001f
	.4byte Func_02000428
	.4byte 0x00000002
	.4byte 0x12840028
	.4byte Data_02000628 + 0x1
	.4byte 0x00000002
	.4byte 0x02840029
	.4byte Func_02000428
	.4byte 0x0001fe14
	.4byte 0x12850409
	.4byte Func_020004f4
	.4byte 0x0001fe14
	.4byte 0x1286040a
	.4byte Func_020004f4
	.4byte 0x0001fe14
	.4byte 0x1285040b
	.4byte Func_020004f4
	.4byte 0x0001fe14
	.4byte 0x1286040c
	.4byte Func_020004f4
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte Func_020005b4
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte Func_020005b4
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte Func_020005b4
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte Func_020005b4
	.4byte 0x00004602
	.4byte 0x12080033
	.4byte Func_020007e8
	.4byte 0x00004602
	.4byte 0x12080034
	.4byte Func_02000784
	.4byte 0x00000008
	.4byte 0xffff0000
	.4byte Func_02000728
	.4byte 0x10008c15
	.4byte 0xffff000f
	.4byte Func_02000728
	.4byte 0x10008c15
	.4byte 0xffff0010
	.4byte Func_02000728
	.4byte 0x10008c15
	.4byte 0x13070011
	.4byte Func_02000728
	.4byte 0x10008c15
	.4byte 0xffff0012
	.4byte Func_02000728
	.4byte 0x10008c15
	.4byte 0xffff0013
	.4byte Func_02000728
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte Func_02000748
	.4byte 0x00008c15
	.4byte 0xffff000f
	.4byte Func_02000748
	.4byte 0x00008c15
	.4byte 0xffff0010
	.4byte Func_02000748
	.4byte 0x00008c15
	.4byte 0x13070011
	.4byte Func_02000748
	.4byte 0x00008c15
	.4byte 0xffff0012
	.4byte Func_02000748
	.4byte 0x00008c15
	.4byte 0xffff0013
	.4byte Func_02000748
	.4byte 0x00004e15
	.4byte 0x03070014
	.4byte Func_02000768
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
.L_0200f200:
	.4byte 0x00000002
	.4byte 0xffff0001
	.4byte Func_02000138
	.4byte 0x00000002
	.4byte 0xffff0002
	.4byte Func_02000138
	.4byte 0x00000002
	.4byte 0xffff0003
	.4byte Func_02000138
	.4byte 0x00000002
	.4byte 0xffff0004
	.4byte Func_02000138
	.4byte 0x00000002
	.4byte 0x1281000a
	.4byte Func_02000418
	.4byte 0x00000002
	.4byte 0x0281000b
	.4byte Func_02000330
	.4byte 0x00000002
	.4byte 0x12810014
	.4byte Func_02000418
	.4byte 0x00000002
	.4byte 0x02810015
	.4byte Func_02000330
	.4byte 0x00000002
	.4byte 0x1284001e
	.4byte Data_02000628 + 0x1
	.4byte 0x00000002
	.4byte 0x0284001f
	.4byte Func_02000428
	.4byte 0x00000002
	.4byte 0x12840028
	.4byte Data_02000628 + 0x1
	.4byte 0x00000002
	.4byte 0x02840029
	.4byte Func_02000428
	.4byte 0x0001fe14
	.4byte 0x12850409
	.4byte Func_020004f4
	.4byte 0x0001fe14
	.4byte 0x1286040a
	.4byte Func_020004f4
	.4byte 0x0001fe14
	.4byte 0x1285040b
	.4byte Func_020004f4
	.4byte 0x0001fe14
	.4byte 0x1286040c
	.4byte Func_020004f4
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte Func_020005b4
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte Func_020005b4
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte Func_020005b4
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte Func_020005b4
	.4byte 0x0000c602
	.4byte 0xffff0021
	.4byte Func_020008a4
	.4byte 0x00004602
	.4byte 0xffff0021
	.4byte Func_020008a4
	.4byte 0x00000008
	.4byte 0xffff0000
	.4byte Func_02000818
	.4byte 0x10008c15
	.4byte 0xffff000f
	.4byte Func_02000818
	.4byte 0x10008c15
	.4byte 0xffff0010
	.4byte Func_02000818
	.4byte 0x10008c15
	.4byte 0xffff0015
	.4byte Func_02000818
	.4byte 0x10008c15
	.4byte 0xffff0016
	.4byte Func_02000818
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte Func_0200082c
	.4byte 0x00008c15
	.4byte 0xffff000f
	.4byte Func_0200082c
	.4byte 0x00008c15
	.4byte 0xffff0010
	.4byte Func_0200082c
	.4byte 0x00008c15
	.4byte 0xffff0015
	.4byte Func_0200082c
	.4byte 0x00008c15
	.4byte 0xffff0016
	.4byte Func_0200082c
	.4byte 0x00001815
	.4byte 0x12140011
	.4byte Func_020008d0
	.4byte 0x00001815
	.4byte 0x12150012
	.4byte Func_020008d0
	.4byte 0x00001815
	.4byte 0x12160013
	.4byte Func_020008d0
	.4byte 0x00001815
	.4byte 0x12170014
	.4byte Func_020008d0
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
.L_0200f3bc:
	.4byte 0x00000002
	.4byte 0xffff0001
	.4byte Func_02000138
	.4byte 0x00000002
	.4byte 0xffff0002
	.4byte Func_02000138
	.4byte 0x00000002
	.4byte 0xffff0003
	.4byte Func_02000138
	.4byte 0x00000002
	.4byte 0xffff0004
	.4byte Func_02000138
	.4byte 0x00000002
	.4byte 0x1281000a
	.4byte Func_02000418
	.4byte 0x00000002
	.4byte 0x0281000b
	.4byte Func_02000330
	.4byte 0x00000002
	.4byte 0x12810014
	.4byte Func_02000418
	.4byte 0x00000002
	.4byte 0x02810015
	.4byte Func_02000330
	.4byte 0x00000002
	.4byte 0x1284001e
	.4byte Data_02000628 + 0x1
	.4byte 0x00000002
	.4byte 0x0284001f
	.4byte Func_02000428
	.4byte 0x00000002
	.4byte 0x12840028
	.4byte Data_02000628 + 0x1
	.4byte 0x00000002
	.4byte 0x02840029
	.4byte Func_02000428
	.4byte 0x0001fe14
	.4byte 0x12850409
	.4byte Func_020004f4
	.4byte 0x0001fe14
	.4byte 0x1286040a
	.4byte Func_020004f4
	.4byte 0x0001fe14
	.4byte 0x1285040b
	.4byte Func_020004f4
	.4byte 0x0001fe14
	.4byte 0x1286040c
	.4byte Func_020004f4
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte Func_020005b4
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte Func_020005b4
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte Func_020005b4
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte Func_020005b4
	.4byte 0x00004602
	.4byte 0xffff0021
	.4byte Func_02000784
	.4byte 0x00000008
	.4byte 0xffff0000
	.4byte Func_02000728
	.4byte 0x10008c15
	.4byte 0x1327000f
	.4byte Func_02000728
	.4byte 0x10008c15
	.4byte 0x13280010
	.4byte Func_02000728
	.4byte 0x10008c15
	.4byte 0xffff0011
	.4byte Func_02000728
	.4byte 0x10008c15
	.4byte 0xffff0012
	.4byte Func_02000728
	.4byte 0x10008c15
	.4byte 0xffff0013
	.4byte Func_02000728
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte Func_02000748
	.4byte 0x00008c15
	.4byte 0x1327000f
	.4byte Func_02000748
	.4byte 0x00008c15
	.4byte 0x13280010
	.4byte Func_02000748
	.4byte 0x00008c15
	.4byte 0xffff0011
	.4byte Func_02000748
	.4byte 0x00008c15
	.4byte 0xffff0012
	.4byte Func_02000748
	.4byte 0x00008c15
	.4byte 0xffff0013
	.4byte Func_02000748
	.4byte 0x00004e15
	.4byte 0x03270014
	.4byte Func_02000904
	.4byte 0x00004e15
	.4byte 0x03280015
	.4byte Func_02000904
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
.L_0200f56c:
	.4byte 0x00000002
	.4byte 0xffff0001
	.4byte Func_02000138
	.4byte 0x00000002
	.4byte 0xffff0002
	.4byte Func_02000138
	.4byte 0x00000002
	.4byte 0xffff0003
	.4byte Func_02000138
	.4byte 0x00000002
	.4byte 0xffff0004
	.4byte Func_02000138
	.4byte 0x00000002
	.4byte 0x1281000a
	.4byte Func_02000418
	.4byte 0x00000002
	.4byte 0x0281000b
	.4byte Func_02000330
	.4byte 0x00000002
	.4byte 0x12810014
	.4byte Func_02000418
	.4byte 0x00000002
	.4byte 0x02810015
	.4byte Func_02000330
	.4byte 0x00000002
	.4byte 0x1284001e
	.4byte Data_02000628 + 0x1
	.4byte 0x00000002
	.4byte 0x0284001f
	.4byte Func_02000428
	.4byte 0x00000002
	.4byte 0x12840028
	.4byte Data_02000628 + 0x1
	.4byte 0x00000002
	.4byte 0x02840029
	.4byte Func_02000428
	.4byte 0x0001fe14
	.4byte 0x12850409
	.4byte Func_020004f4
	.4byte 0x0001fe14
	.4byte 0x1286040a
	.4byte Func_020004f4
	.4byte 0x0001fe14
	.4byte 0x1285040b
	.4byte Func_020004f4
	.4byte 0x0001fe14
	.4byte 0x1286040c
	.4byte Func_020004f4
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte Func_020005b4
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte Func_020005b4
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte Func_020005b4
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte Func_020005b4
	.4byte 0x00000202
	.4byte 0xffff0032
	.4byte Func_02000c84
	.4byte 0x00000202
	.4byte 0xffff0033
	.4byte Func_02000c84
	.4byte 0x00000202
	.4byte 0xffff0034
	.4byte Func_02000c84
	.4byte 0x00000202
	.4byte 0xffff0035
	.4byte Func_02000c84
	.4byte 0x00000002
	.4byte 0x03350032
	.4byte Func_02000c34
	.4byte 0x00000002
	.4byte 0x13350033
	.4byte Func_02000c48
	.4byte 0x00000002
	.4byte 0x03370034
	.4byte Func_02000c5c
	.4byte 0x00000002
	.4byte 0x13370035
	.4byte Func_02000c70
	.4byte 0x00000202
	.4byte 0xffff003c
	.4byte Func_02000c84
	.4byte 0x00008c15
	.4byte 0xffff000f
	.4byte Func_020009ac
	.4byte 0x00008c15
	.4byte 0x13390010
	.4byte Func_020009ac
	.4byte 0x00008c15
	.4byte 0xffff0011
	.4byte Func_020009ac
	.4byte 0x00004e15
	.4byte 0x03390016
	.4byte Func_02000990
	.4byte 0x50008615
	.4byte 0x03350012
	.4byte Func_02000b3c
	.4byte 0x50008615
	.4byte 0x13350013
	.4byte Func_02000b4c
	.4byte 0x50008615
	.4byte 0x03370014
	.4byte Func_02000b5c
	.4byte 0x50008615
	.4byte 0x13370015
	.4byte Func_02000b6c
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02007734
Data_02007734:
	.4byte 0x0342000f
	.4byte 0x03440010
	.4byte 0x0000ffff
.L_0200f740:
	.4byte 0x00000002
	.4byte 0xffff0001
	.4byte Func_02000138
	.4byte 0x00000002
	.4byte 0xffff0002
	.4byte Func_02000138
	.4byte 0x00000002
	.4byte 0xffff0003
	.4byte Func_02000138
	.4byte 0x00000002
	.4byte 0xffff0004
	.4byte Func_02000138
	.4byte 0x00000002
	.4byte 0x1281000a
	.4byte Func_02000418
	.4byte 0x00000002
	.4byte 0x0281000b
	.4byte Func_02000330
	.4byte 0x00000002
	.4byte 0x12810014
	.4byte Func_02000418
	.4byte 0x00000002
	.4byte 0x02810015
	.4byte Func_02000330
	.4byte 0x00000002
	.4byte 0x1284001e
	.4byte Data_02000628 + 0x1
	.4byte 0x00000002
	.4byte 0x0284001f
	.4byte Func_02000428
	.4byte 0x00000002
	.4byte 0x12840028
	.4byte Data_02000628 + 0x1
	.4byte 0x00000002
	.4byte 0x02840029
	.4byte Func_02000428
	.4byte 0x0001fe14
	.4byte 0x12850409
	.4byte Func_020004f4
	.4byte 0x0001fe14
	.4byte 0x1286040a
	.4byte Func_020004f4
	.4byte 0x0001fe14
	.4byte 0x1285040b
	.4byte Func_020004f4
	.4byte 0x0001fe14
	.4byte 0x1286040c
	.4byte Func_020004f4
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte Func_020005b4
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte Func_020005b4
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte Func_020005b4
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte Func_020005b4
	.4byte 0x00004602
	.4byte 0xffff0021
	.4byte Func_02000784
	.4byte 0x0000c602
	.4byte 0xffff0021
	.4byte Func_02000784
	.4byte 0x0000c602
	.4byte 0xffff0032
	.4byte Func_02000784
	.4byte 0x0000c602
	.4byte 0xffff0033
	.4byte Func_02000784
	.4byte 0x00000602
	.4byte 0xffff0022
	.4byte Func_02000cc0
	.4byte 0x00008602
	.4byte 0xffff0022
	.4byte Func_02000cc0
	.4byte 0x50009705
	.4byte 0x03430032
	.4byte Func_02000cd0
	.4byte 0x50009705
	.4byte 0x03450033
	.4byte Func_02000ce4
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
.L_0200f89c:
	.4byte 0x00000002
	.4byte 0xffff0001
	.4byte Func_02000138
	.4byte 0x00000002
	.4byte 0xffff0002
	.4byte Func_02000138
	.4byte 0x00000002
	.4byte 0xffff0003
	.4byte Func_02000138
	.4byte 0x00000002
	.4byte 0xffff0004
	.4byte Func_02000138
	.4byte 0x00000002
	.4byte 0x1281000a
	.4byte Func_02000418
	.4byte 0x00000002
	.4byte 0x0281000b
	.4byte Func_02000330
	.4byte 0x00000002
	.4byte 0x12810014
	.4byte Func_02000418
	.4byte 0x00000002
	.4byte 0x02810015
	.4byte Func_02000330
	.4byte 0x00000002
	.4byte 0x1284001e
	.4byte Data_02000628 + 0x1
	.4byte 0x00000002
	.4byte 0x0284001f
	.4byte Func_02000428
	.4byte 0x00000002
	.4byte 0x12840028
	.4byte Data_02000628 + 0x1
	.4byte 0x00000002
	.4byte 0x02840029
	.4byte Func_02000428
	.4byte 0x0001fe14
	.4byte 0x12850409
	.4byte Func_020004f4
	.4byte 0x0001fe14
	.4byte 0x1286040a
	.4byte Func_020004f4
	.4byte 0x0001fe14
	.4byte 0x1285040b
	.4byte Func_020004f4
	.4byte 0x0001fe14
	.4byte 0x1286040c
	.4byte Func_020004f4
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte Func_020005b4
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte Func_020005b4
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte Func_020005b4
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte Func_020005b4
	.4byte 0x00000602
	.4byte 0xffff0021
	.4byte Func_02000784
	.4byte 0x00008602
	.4byte 0xffff0032
	.4byte Func_0200106c
	.4byte 0x00000602
	.4byte 0xffff0032
	.4byte Func_02001080
	.4byte 0x00008602
	.4byte 0xffff0033
	.4byte Func_02001090
	.4byte 0x00000602
	.4byte 0xffff0033
	.4byte Func_020010a4
	.4byte 0x0000c602
	.4byte 0xffff0032
	.4byte Func_020010b4
	.4byte 0x00004602
	.4byte 0xffff0032
	.4byte Func_020010c8
	.4byte 0x0000c602
	.4byte 0xffff0033
	.4byte Func_020010b4
	.4byte 0x00004602
	.4byte 0xffff0033
	.4byte Func_020010c8
	.4byte 0x0000c602
	.4byte 0xffff0034
	.4byte Func_020010b4
	.4byte 0x00004602
	.4byte 0xffff0034
	.4byte Func_020010d8
	.4byte 0x0000c602
	.4byte 0xffff0035
	.4byte Func_020010b4
	.4byte 0x00004602
	.4byte 0xffff0035
	.4byte Func_020010c8
	.4byte 0x00000008
	.4byte 0xffff0000
	.4byte Func_02000728
	.4byte 0x10008c15
	.4byte 0xffff000f
	.4byte Func_02000728
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte Func_02000748
	.4byte 0x00008c15
	.4byte 0xffff000f
	.4byte Func_02000748
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
.L_0200fa64:
	.4byte 0x00000002
	.4byte 0xffff0001
	.4byte Func_02000138
	.4byte 0x00000002
	.4byte 0xffff0002
	.4byte Func_02000138
	.4byte 0x00000002
	.4byte 0xffff0003
	.4byte Func_02000138
	.4byte 0x00000002
	.4byte 0xffff0004
	.4byte Func_02000138
	.4byte 0x00000002
	.4byte 0x1281000a
	.4byte Func_02000418
	.4byte 0x00000002
	.4byte 0x0281000b
	.4byte Func_02000330
	.4byte 0x00000002
	.4byte 0x12810014
	.4byte Func_02000418
	.4byte 0x00000002
	.4byte 0x02810015
	.4byte Func_02000330
	.4byte 0x00000002
	.4byte 0x1284001e
	.4byte Data_02000628 + 0x1
	.4byte 0x00000002
	.4byte 0x0284001f
	.4byte Func_02000428
	.4byte 0x00000002
	.4byte 0x12840028
	.4byte Data_02000628 + 0x1
	.4byte 0x00000002
	.4byte 0x02840029
	.4byte Func_02000428
	.4byte 0x0001fe14
	.4byte 0x12850409
	.4byte Func_020004f4
	.4byte 0x0001fe14
	.4byte 0x1286040a
	.4byte Func_020004f4
	.4byte 0x0001fe14
	.4byte 0x1285040b
	.4byte Func_020004f4
	.4byte 0x0001fe14
	.4byte 0x1286040c
	.4byte Func_020004f4
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte Func_020005b4
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte Func_020005b4
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte Func_020005b4
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte Func_020005b4
	.4byte 0x00000002
	.4byte 0xffff0021
	.4byte Func_02001178
	.4byte 0x00000008
	.4byte 0xffff0000
	.4byte Func_02000728
	.4byte 0x10008c15
	.4byte 0xffff000f
	.4byte Func_02000728
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte Func_02000748
	.4byte 0x00008c15
	.4byte 0xffff000f
	.4byte Func_02000748
	.4byte 0x50008805
	.4byte 0x03630032
	.4byte Func_02001148
	.4byte 0x50008805
	.4byte 0x03640033
	.4byte Func_02001158
	.4byte 0x50008805
	.4byte 0x03650034
	.4byte Func_02001168
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02007bc0
Data_02007bc0:
	.4byte .L_0200da8c
	.4byte .L_0200dac8
	.4byte .L_0200db04
.L_0200fbcc:
	.4byte 0x00000002
	.4byte 0xffff0001
	.4byte Func_02000138
	.4byte 0x00000002
	.4byte 0xffff0002
	.4byte Func_02000138
	.4byte 0x00000002
	.4byte 0xffff0003
	.4byte Func_02000138
	.4byte 0x00000002
	.4byte 0xffff0004
	.4byte Func_02000138
	.4byte 0x00000002
	.4byte 0x1281000a
	.4byte Func_02000418
	.4byte 0x00000002
	.4byte 0x0281000b
	.4byte Func_02000330
	.4byte 0x00000002
	.4byte 0x12810014
	.4byte Func_02000418
	.4byte 0x00000002
	.4byte 0x02810015
	.4byte Func_02000330
	.4byte 0x00000002
	.4byte 0x1284001e
	.4byte Data_02000628 + 0x1
	.4byte 0x00000002
	.4byte 0x0284001f
	.4byte Func_02000428
	.4byte 0x00000002
	.4byte 0x12840028
	.4byte Data_02000628 + 0x1
	.4byte 0x00000002
	.4byte 0x02840029
	.4byte Func_02000428
	.4byte 0x0001fe14
	.4byte 0x12850409
	.4byte Func_020004f4
	.4byte 0x0001fe14
	.4byte 0x1286040a
	.4byte Func_020004f4
	.4byte 0x0001fe14
	.4byte 0x1285040b
	.4byte Func_020004f4
	.4byte 0x0001fe14
	.4byte 0x1286040c
	.4byte Func_020004f4
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte Func_020005b4
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte Func_020005b4
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte Func_020005b4
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte Func_020005b4
	.4byte 0x00000002
	.4byte 0xffff0032
	.4byte Func_020014e4
	.4byte 0x00008b85
	.4byte 0xffff0000
	.4byte Func_020011c0
	.4byte 0x40008b85
	.4byte 0xffff0000
	.4byte Func_020011dc
	.4byte 0x00004e15
	.4byte 0x03740011
	.4byte Func_020011f8
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02007cf8
Data_02007cf8:
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
	.4byte 0x00008f15
	.4byte 0x02890008
	.4byte Func_020015a8
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02007d40
Data_02007d40:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x50008905
	.4byte 0xffff0021
	.4byte Func_020015b8
	.4byte 0x50008905
	.4byte 0xffff0022
	.4byte Func_020015d8
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte Func_02000038
	.4byte 0x50009085
	.4byte 0xffff0000
	.4byte Func_0200163c
	.4byte 0x40009085
	.4byte 0xffff0000
	.4byte Func_020016c8
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02007d94
Data_02007d94:
	.4byte 0x00000002
	.4byte 0x02830021
	.4byte Func_02001754
	.4byte 0x00000002
	.4byte 0x19ff0028
	.4byte Func_020017e0
	.4byte 0x00000002
	.4byte 0x19ff0029
	.4byte Func_020017e0
	.4byte 0x00000031
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000031
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00008c15
	.4byte 0x09fc000a
	.4byte Func_0200185c
	.4byte 0x00000009
	.4byte 0x09fc0000
	.4byte Func_0200185c
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02007e00
Data_02007e00:
	.4byte .L_0200f050
	.4byte .L_0200f200
	.4byte .L_0200f3bc
	.4byte .L_0200f56c
	.4byte .L_0200f740
	.4byte .L_0200f89c
	.4byte .L_0200fa64
	.4byte .L_0200fbcc
	.global Data_02007e20
Data_02007e20:
	.4byte 0xffffffff
	.global Data_02007e24
Data_02007e24:
	.4byte 0x00000001
