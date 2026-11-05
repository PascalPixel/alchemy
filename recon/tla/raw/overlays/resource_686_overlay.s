	.set ANKOHL_RUINS_ES_IT, 0
	.ifdef TLA_EDITION_ES
	.set ANKOHL_RUINS_ES_IT, 1
	.endif
	.ifdef TLA_EDITION_IT
	.set ANKOHL_RUINS_ES_IT, 1
	.endif
.syntax unified
	.thumb
	.section .text.x0200815a,"ax",%progbits
	.2byte 0x0000
	.section .text.x02008416,"ax",%progbits
	.ifdef TLA_EDITION_JA
	.else
	.2byte 0x0000
	.endif
	.section .text.x02008418,"ax",%progbits
	.ifdef TLA_EDITION_JA
	.ifdef TLA_EDITION_JA
	.else
	.global Func_02000418
	.thumb_func
Func_02000418:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, .L_020085ac
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	sub sp, #68
	bl Actor_GetById
	adds r7, r0, #0
	bl Func_02002e44
	movs r0, #0
	bl Func_02002edc
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r1, r1
	negs r2, r2
	movs r3, #0
	negs r0, r0
	bl Camera_MoveWithinMapBounds
	bl Func_02002dec
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
	bl Func_02002eec
	movs r3, #3
	strb r3, [r5]
	movs r0, #24
	bl FieldEvent_Wait
	add r2, sp, #28
	movs r3, #7
	str r3, [r2, #4]
	ldr r3, .L_020085b0
	mov r8, r2
	str r3, [r2, #36]
	movs r3, #163
	lsls r3, r3, #8
	adds r3, #215
	str r3, [r2, #8]
	str r3, [r2, #12]
	movs r3, #0
	mov r10, r3
.L_020084b2:
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
	ldr r3, .L_020085b4
	adds r2, r2, r3
	str r2, [r6]
	bl Random16Far
	lsls r3, r0, #1
	ldr r5, [r6, #8]
	adds r3, r3, r0
	ldr r4, .L_020085b8
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
	ldr r4, .L_020085bc
	str r5, [sp, #4]
	str r4, [sp, #8]
	mov r4, r8
	str r4, [sp, #12]
	bl Func_0200015c
	movs r2, #1
	add r10, r2
	mov r3, r10
	cmp r3, #16
	bls .L_020084b2
	movs r0, #188
	bl Func_02002eec
	ldr r5, .L_020085ac
	movs r4, #133
	lsls r4, r4, #2
	adds r5, r5, r4
	movs r1, #2
	ldr r0, [r5]
	adds r1, #255
	bl Func_02002e9c
	ldr r0, [r5]
	movs r1, #49
	bl Actor_SetModeById
	movs r0, #160
	movs r1, #160
	movs r2, #128
	lsls r0, r0, #11
	lsls r1, r1, #11
	lsls r2, r2, #9
	bl Func_02002e24
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	adds r2, #102
	negs r0, r0
	negs r1, r1
	bl Func_02002e24
	bl Func_02002e2c
	movs r1, #128
	ldr r0, [r5]
	lsls r1, r1, #1
	bl Func_02002e9c
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
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #88
	str r2, [r3]
	bl Func_02002e4c
	add sp, #68
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_020085ac:
	.4byte gMainRoutineWork
.L_020085b0:
	.4byte Effect_AdvanceMotion
.L_020085b4:
	.4byte 0xffffa000
.L_020085b8:
	.4byte 0xffffd000
.L_020085bc:
	.4byte 0x01090001
	.endif
	.endif
	.section .text.x020092c4,"ax",%progbits
	.global Func_020012c4
	.thumb_func
Func_020012c4:
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
	sub sp, #4
	mov r8, r3
	mov r2, r8
	adds r2, #228
	ldr r7, [r2]
	ldr r3, .L_02009408
	ldr r6, [r2, #4]
	mov r1, r8
	ands r7, r3
	ands r6, r3
	ldr r3, [r1]
	ldr r0, .L_0200940c
	ldr r3, [r3, #4]
	ldr r2, .L_02009410
	str r3, [sp, #0]
	mov r10, r0
	movs r1, #2
	ldrsh r3, [r0, r1]
	movs r0, #131
	lsls r3, r3, #2
	adds r3, r3, r2
	ldrh r3, [r3, #2]
	lsls r0, r0, #1
	lsrs r3, r3, #5
	mov r5, r10
	adds r0, #255
	mov r11, r3
	adds r5, #16
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020093e8
	mov r2, r10
	ldr r3, [r2, #4]
	movs r0, #128
	lsls r0, r0, #14
	cmp r3, r0
	bge .L_020093e8
	movs r1, #0
	mov r9, r1
.L_02009328:
	ldr r3, [r5, #12]
	ldr r1, [r5]
	ldr r0, [r5, #8]
	ldr r4, [r5, #4]
	cmp r3, #4
	bhi .L_02009388
	ldr r2, .L_02009414
	lsls r3, r3, #2
	ldr r2, [r3, r2]
	mov r3, r10
	mov r12, r2
	ldr r2, [r3, #4]
	mov pc, r12
	.2byte 0x0000
.L_02009344:
	.4byte .L_02009358
	.4byte .L_02009366
	.4byte .L_0200936c
	.4byte .L_02009372
	.4byte .L_0200937a
.L_02009358:
	movs r3, #128
	lsls r3, r3, #13
	subs r1, r1, r2
.L_0200935e:
	adds r0, r0, r2
	cmp r2, r3
	ble .L_02009388
	b .L_02009386
.L_02009366:
	subs r1, r1, r2
	adds r0, r0, r2
	b .L_02009388
.L_0200936c:
	adds r1, r1, r2
	adds r0, r0, r2
	b .L_02009388
.L_02009372:
	movs r3, #128
	lsls r3, r3, #13
	adds r1, r1, r2
	b .L_0200935e
.L_0200937a:
	movs r3, #128
	lsls r3, r3, #13
	cmp r2, r3
	bge .L_02009386
	adds r0, r0, r2
	b .L_02009388
.L_02009386:
	ldr r1, .L_02009418
.L_02009388:
	subs r1, r1, r7
	subs r3, r4, r6
	subs r2, r3, r0
	asrs r3, r1, #16
	movs r0, #167
	adds r1, r3, #0
	asrs r2, r2, #16
	adds r3, #7
	lsls r0, r0, #1
	subs r1, #8
	subs r2, #8
	cmp r3, r0
	bhi .L_020093dc
	movs r3, #15
	negs r3, r3
	cmp r2, r3
	blt .L_020093dc
	cmp r2, #239
	bgt .L_020093dc
	movs r3, #128
	lsls r3, r3, #1
	adds r3, #255
	ands r1, r3
	movs r3, #255
	ands r2, r3
	movs r3, #0
	str r3, [r5, #16]
	lsls r3, r1, #16
	orrs r2, r3
	ldr r3, .L_0200941c
	mov r0, r11
	orrs r2, r3
	movs r3, #128
	lsls r3, r3, #3
	orrs r3, r0
	adds r0, r5, #0
	str r2, [r5, #20]
	str r3, [r5, #24]
	adds r0, #16
	movs r1, #255
	bl Func_02002dac
.L_020093dc:
	movs r1, #1
	add r9, r1
	mov r2, r9
	adds r5, #28
	cmp r2, #7
	ble .L_02009328
.L_020093e8:
	mov r0, r10
	ldr r2, [r0, #12]
	ldr r1, [r0, #4]
	movs r3, #160
	lsls r3, r3, #1
	add r3, r8
	adds r2, r2, r1
	str r2, [r3, #12]
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009408:
	.4byte 0xffff0000
.L_0200940c:
	.4byte Data_02004030
.L_02009410:
	.4byte gResourceTableEntries
.L_02009414:
	.4byte .L_02009344
.L_02009418:
	.4byte 0xfff00000
.L_0200941c:
	.4byte 0x40000800
	.section .text.x0200a438,"ax",%progbits
	.ifdef TLA_EDITION_JA
	.global Func_02002438
	.thumb_func
Func_02002438:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r7, .L_ja_0200a544
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r7, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_ja_0200a548
	sub sp, #8
	cmp r2, r3
	beq .L_ja_0200a24e
	b .L_ja_0200a47e
.L_ja_0200a24e:
	movs r0, #0
	bl Func_02002ed4
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #210
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200a26c
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_02002e6c
.L_ja_0200a26c:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #211
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200a284
	movs r0, #11
	movs r1, #0
	movs r2, #0
	bl Func_02002e6c
.L_ja_0200a284:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #212
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200a29c
	movs r0, #12
	movs r1, #0
	movs r2, #0
	bl Func_02002e6c
.L_ja_0200a29c:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #213
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200a2b4
	movs r0, #13
	movs r1, #0
	movs r2, #0
	bl Func_02002e6c
.L_ja_0200a2b4:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #214
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200a2cc
	movs r0, #14
	movs r1, #0
	movs r2, #0
	bl Func_02002e6c
.L_ja_0200a2cc:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #215
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200a2e4
	movs r0, #15
	movs r1, #0
	movs r2, #0
	bl Func_02002e6c
.L_ja_0200a2e4:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #216
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200a2fc
	movs r0, #16
	movs r1, #0
	movs r2, #0
	bl Func_02002e6c
.L_ja_0200a2fc:
	movs r0, #153
	lsls r0, r0, #4
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200a312
	movs r0, #17
	movs r1, #0
	movs r2, #0
	bl Func_02002e6c
.L_ja_0200a312:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #145
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200a32a
	movs r0, #18
	movs r1, #0
	movs r2, #0
	bl Func_02002e6c
.L_ja_0200a32a:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #146
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200a342
	movs r0, #19
	movs r1, #0
	movs r2, #0
	bl Func_02002e6c
.L_ja_0200a342:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #147
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200a35a
	movs r0, #20
	movs r1, #0
	movs r2, #0
	bl Func_02002e6c
.L_ja_0200a35a:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #148
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200a372
	movs r0, #21
	movs r1, #0
	movs r2, #0
	bl Func_02002e6c
.L_ja_0200a372:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #149
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200a38a
	movs r0, #22
	movs r1, #0
	movs r2, #0
	bl Func_02002e6c
.L_ja_0200a38a:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #150
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200a3a2
	movs r0, #23
	movs r1, #0
	movs r2, #0
	bl Func_02002e6c
.L_ja_0200a3a2:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #151
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200a3ba
	movs r0, #24
	movs r1, #0
	movs r2, #0
	bl Func_02002e6c
.L_ja_0200a3ba:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #152
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200a3d2
	movs r0, #25
	movs r1, #0
	movs r2, #0
	bl Func_02002e6c
.L_ja_0200a3d2:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #153
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200a3ea
	movs r0, #26
	movs r1, #0
	movs r2, #0
	bl Func_02002e6c
.L_ja_0200a3ea:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #154
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200a402
	movs r0, #27
	movs r1, #0
	movs r2, #0
	bl Func_02002e6c
.L_ja_0200a402:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #155
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200a41a
	movs r0, #28
	movs r1, #0
	movs r2, #0
	bl Func_02002e6c
.L_ja_0200a41a:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #156
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200a432
	movs r0, #29
	movs r1, #0
	movs r2, #0
	bl Func_02002e6c
.L_ja_0200a432:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #157
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200a44a
	movs r0, #30
	movs r1, #0
	movs r2, #0
	bl Func_02002e6c
.L_ja_0200a44a:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #198
	bl GameFlag_Test
	cmp r0, #0
	bne .L_ja_0200a45a
	b .L_ja_0200ab02
.L_ja_0200a45a:
	movs r1, #144
	movs r2, #176
	movs r0, #8
	lsls r1, r1, #15
	lsls r2, r2, #15
	bl Func_02002e6c
	movs r3, #2
	movs r2, #69
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #4
	movs r1, #68
	movs r2, #1
	movs r3, #1
	bl Func_02002dfc
	b .L_ja_0200ab02
.L_ja_0200a47e:
	ldr r3, .L_ja_0200a54c
	cmp r2, r3
	bne .L_ja_0200a550
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #158
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200a49c
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_02002e6c
.L_ja_0200a49c:
	movs r0, #138
	lsls r0, r0, #4
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200a4b4
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_02002e6c
.L_ja_0200a4b4:
	movs r0, #154
	lsls r0, r0, #4
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200a4ca
	movs r0, #11
	movs r1, #0
	movs r2, #0
	bl Func_02002e6c
.L_ja_0200a4ca:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #220
	bl GameFlag_Test
	cmp r0, #0
	bne .L_ja_0200a4da
	b .L_ja_0200ab02
.L_ja_0200a4da:
	movs r0, #8
	bl Actor_GetById
	adds r5, r0, #0
	adds r2, r5, #0
	movs r3, #3
	adds r2, #85
	strb r3, [r2]
	movs r1, #200
	movs r2, #176
	movs r0, #8
	lsls r1, r1, #16
	lsls r2, r2, #15
	bl Func_02002e6c
	movs r3, #7
	str r3, [sp, #4]
	movs r5, #10
	movs r0, #35
	movs r1, #6
	movs r2, #7
	movs r3, #16
	str r5, [sp, #0]
	bl Func_02002df4
	movs r3, #71
	str r3, [sp, #4]
	movs r1, #70
	movs r2, #7
	movs r3, #16
	movs r0, #35
	str r5, [sp, #0]
	bl Func_02002df4
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r7, r2
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #6
	beq .L_ja_0200a536
	cmp r3, #17
	beq .L_ja_0200a536
	cmp r3, #15
	beq .L_ja_0200a536
	b .L_ja_0200ab02
.L_ja_0200a536:
	movs r0, #160
	lsls r0, r0, #1
	adds r0, #255
	bl Func_02002eec
	b .L_ja_0200ab02
	.2byte 0x0000
.L_ja_0200a544:
	.4byte gMainRoutineWork
.L_ja_0200a548:
	.4byte 0x000000bd
.L_ja_0200a54c:
	.4byte 0x000000be
.L_ja_0200a550:
	ldr r3, .L_ja_0200a8b4
	cmp r2, r3
	bne .L_ja_0200a5f2
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #199
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200a586
	movs r1, #130
	movs r2, #208
	movs r0, #8
	lsls r1, r1, #18
	lsls r2, r2, #15
	bl Func_02002e6c
	movs r3, #33
	movs r2, #69
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #32
	movs r1, #69
	movs r2, #1
	movs r3, #1
	bl Func_02002dfc
.L_ja_0200a586:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #221
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200a5a6
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_02002e6c
	movs r0, #9
	movs r1, #4
	bl Actor_SetModeById
.L_ja_0200a5a6:
	movs r0, #129
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_ja_0200a5b6
	b .L_ja_0200ab02
.L_ja_0200a5b6:
	movs r3, #24
	str r3, [sp, #4]
	movs r5, #13
	movs r0, #13
	movs r1, #30
	movs r2, #3
	movs r3, #2
	str r5, [sp, #0]
	bl Func_02002dfc
	movs r3, #14
	movs r2, #88
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #14
	movs r1, #87
	movs r2, #1
	movs r3, #1
	bl Func_02002dfc
	movs r3, #41
	str r3, [sp, #4]
	movs r0, #41
	movs r1, #48
	movs r2, #3
	movs r3, #2
	str r5, [sp, #0]
	bl Func_02002dfc
	b .L_ja_0200ab02
.L_ja_0200a5f2:
	ldr r3, .L_ja_0200a8b8
	cmp r2, r3
	beq .L_ja_0200a5fa
	b .L_ja_0200a8c0
.L_ja_0200a5fa:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #161
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200a612
	movs r0, #11
	movs r1, #0
	movs r2, #0
	bl Func_02002e6c
.L_ja_0200a612:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #162
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200a62a
	movs r0, #12
	movs r1, #0
	movs r2, #0
	bl Func_02002e6c
.L_ja_0200a62a:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #163
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200a642
	movs r0, #13
	movs r1, #0
	movs r2, #0
	bl Func_02002e6c
.L_ja_0200a642:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #164
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200a65a
	movs r0, #14
	movs r1, #0
	movs r2, #0
	bl Func_02002e6c
.L_ja_0200a65a:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #165
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200a672
	movs r0, #15
	movs r1, #0
	movs r2, #0
	bl Func_02002e6c
.L_ja_0200a672:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #166
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200a68a
	movs r0, #16
	movs r1, #0
	movs r2, #0
	bl Func_02002e6c
.L_ja_0200a68a:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #167
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200a6a2
	movs r0, #17
	movs r1, #0
	movs r2, #0
	bl Func_02002e6c
.L_ja_0200a6a2:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #200
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200a6be
	movs r1, #160
	movs r2, #194
	movs r0, #8
	lsls r1, r1, #14
	lsls r2, r2, #18
	bl Func_02002e6c
.L_ja_0200a6be:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #201
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200a6da
	movs r1, #158
	movs r2, #228
	movs r0, #18
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl Func_02002e6c
.L_ja_0200a6da:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #202
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200a70a
	movs r1, #154
	movs r2, #136
	movs r0, #19
	lsls r1, r1, #18
	lsls r2, r2, #16
	bl Func_02002e6c
	movs r3, #37
	movs r2, #71
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #37
	movs r1, #70
	movs r2, #1
	movs r3, #1
	bl Func_02002df4
.L_ja_0200a70a:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #217
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200a736
	movs r0, #9
	bl Actor_GetById
	adds r5, r0, #0
	adds r2, r5, #0
	adds r2, #85
	movs r3, #3
	strb r3, [r2]
	movs r1, #200
	movs r2, #182
	movs r0, #9
	lsls r1, r1, #16
	lsls r2, r2, #18
	bl Func_02002e6c
.L_ja_0200a736:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #218
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200a764
	movs r0, #10
	bl Actor_GetById
	movs r1, #232
	movs r2, #182
	adds r5, r0, #0
	lsls r2, r2, #18
	movs r0, #10
	lsls r1, r1, #16
	bl Func_02002e6c
	adds r2, r5, #0
	adds r2, #85
	movs r3, #3
	strb r3, [r2]
	b .L_ja_0200a78c
.L_ja_0200a764:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #219
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200a78c
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_ja_0200a78c
	movs r1, #164
	movs r2, #190
	movs r0, #10
	lsls r1, r1, #17
	lsls r2, r2, #18
	bl Func_02002e6c
.L_ja_0200a78c:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #218
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200a7ee
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #217
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200a7ee
	ldr r3, .L_ja_0200a8bc
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	ldrh r3, [r3]
	movs r1, #192
	subs r3, #13
	lsls r3, r3, #16
	lsls r1, r1, #10
	cmp r3, r1
	bhi .L_ja_0200a7c8
	movs r0, #160
	lsls r0, r0, #1
	adds r0, #255
	bl Func_02002eec
.L_ja_0200a7c8:
	movs r3, #47
	movs r5, #12
	str r3, [sp, #4]
	movs r0, #16
	movs r1, #27
	movs r2, #3
	movs r3, #13
	str r5, [sp, #0]
	bl Func_02002df4
	movs r3, #111
	str r3, [sp, #4]
	movs r0, #16
	movs r1, #91
	movs r2, #3
	movs r3, #13
	str r5, [sp, #0]
	bl Func_02002df4
.L_ja_0200a7ee:
	ldr r5, .L_ja_0200a8bc
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r5, r2
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #20
	bne .L_ja_0200a89a
	bl Func_02002e44
	movs r0, #0
	bl Func_02002edc
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r0, r0
	negs r1, r1
	negs r2, r2
	movs r3, #0
	bl Camera_MoveWithinMapBounds
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r5, r2
	ldr r0, [r3]
	movs r1, #0
	movs r2, #0
	bl Func_02002e6c
	movs r1, #0
	movs r2, #0
	movs r0, #10
	bl Func_02002e6c
	bl Event_StartScreenTransition
	movs r0, #15
	bl FieldEvent_Wait
	movs r0, #10
	bl Actor_GetById
	movs r1, #164
	movs r2, #190
	adds r5, r0, #0
	lsls r1, r1, #17
	lsls r2, r2, #18
	movs r0, #10
	bl Func_02002e6c
	ldr r3, [r5, #12]
	movs r1, #128
	lsls r1, r1, #16
	adds r3, r3, r1
	adds r2, r5, #0
	str r3, [r5, #12]
	adds r2, #85
	movs r3, #3
	movs r6, #60
	strb r3, [r2]
	b .L_ja_0200a86c
.L_ja_0200a86a:
	subs r6, #1
.L_ja_0200a86c:
	cmp r6, #0
	beq .L_ja_0200a880
	movs r0, #1
	bl wait
	ldr r3, [r5, #40]
	movs r2, #1
	negs r2, r2
	cmp r3, r2
	ble .L_ja_0200a86a
.L_ja_0200a880:
	movs r0, #188
	bl Func_02002eec
	movs r0, #60
	bl FieldEvent_Wait
	movs r0, #10
	adds r0, #255
	bl GameFlag_SetBit
	movs r0, #20
	bl Func_02002ebc
.L_ja_0200a89a:
	ldr r3, .L_ja_0200a8bc
	movs r1, #241
	lsls r1, r1, #1
	adds r3, r3, r1
	ldrh r3, [r3]
	movs r2, #192
	subs r3, #13
	lsls r3, r3, #16
	lsls r2, r2, #10
	cmp r3, r2
	bls .L_ja_0200a8b2
	b .L_ja_0200ab02
.L_ja_0200a8b2:
	b .L_ja_0200a92c
.L_ja_0200a8b4:
	.4byte 0x000000bf
.L_ja_0200a8b8:
	.4byte 0x000000c0
.L_ja_0200a8bc:
	.4byte gMainRoutineWork
.L_ja_0200a8c0:
	ldr r3, .L_ja_0200ab10
	cmp r2, r3
	bne .L_ja_0200a938
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #219
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200a8de
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_02002e6c
.L_ja_0200a8de:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #203
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200a90e
	movs r1, #140
	movs r2, #156
	movs r0, #8
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_02002e6c
	movs r3, #18
	movs r2, #84
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #16
	movs r1, #84
	movs r2, #1
	movs r3, #1
	bl Func_02002dfc
.L_ja_0200a90e:
	movs r1, #241
	lsls r1, r1, #1
	adds r3, r7, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #1
	beq .L_ja_0200a922
	cmp r3, #12
	beq .L_ja_0200a922
	b .L_ja_0200ab02
.L_ja_0200a922:
	movs r0, #160
	lsls r0, r0, #1
	adds r0, #255
	bl Func_02002eec
.L_ja_0200a92c:
	movs r1, #144
	ldr r0, .L_ja_0200ab14
	lsls r1, r1, #3
	bl Task_Add
	b .L_ja_0200ab02
.L_ja_0200a938:
	ldr r3, .L_ja_0200ab18
	cmp r2, r3
	beq .L_ja_0200a940
	b .L_ja_0200aadc
.L_ja_0200a940:
	movs r3, #241
	lsls r3, r3, #1
	adds r3, r3, r7
	mov r10, r3
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #4
	bgt .L_ja_0200a954
	bl Func_02001420
.L_ja_0200a954:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #226
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200a988
	movs r3, #65
	movs r5, #14
	str r3, [sp, #4]
	movs r0, #26
	movs r1, #67
	movs r2, #1
	movs r3, #3
	str r5, [sp, #0]
	bl Func_02002df4
	movs r3, #5
	str r3, [sp, #4]
	movs r0, #15
	movs r1, #5
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	bl Func_02002dfc
.L_ja_0200a988:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #227
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200a9bc
	movs r3, #83
	movs r5, #2
	str r3, [sp, #4]
	movs r0, #26
	movs r1, #67
	movs r2, #1
	movs r3, #3
	str r5, [sp, #0]
	bl Func_02002df4
	movs r3, #23
	str r3, [sp, #4]
	movs r0, #1
	movs r1, #23
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	bl Func_02002dfc
.L_ja_0200a9bc:
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_ja_0200a9ca
	b .L_ja_0200ab02
.L_ja_0200a9ca:
	movs r0, #129
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	bne .L_ja_0200a9d8
	b .L_ja_0200ab02
.L_ja_0200a9d8:
	ldr r3, .L_ja_0200ab1c
	movs r2, #128
	lsls r2, r2, #14
	str r2, [r3, #4]
	mov r1, r10
	ldrh r3, [r1]
	mov r8, r2
	subs r3, #1
	movs r2, #128
	lsls r3, r3, #16
	lsls r2, r2, #9
	cmp r3, r2
	bhi .L_ja_0200aa4e
	movs r5, #4
	movs r6, #5
	movs r0, #25
	movs r1, #22
	movs r2, #9
	movs r3, #2
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_02002dfc
	movs r3, #69
	str r3, [sp, #4]
	movs r0, #25
	movs r1, #86
	movs r2, #9
	movs r3, #2
	str r5, [sp, #0]
	bl Func_02002dfc
	movs r5, #1
	movs r3, #7
	movs r0, #22
	movs r1, #24
	movs r2, #15
	str r3, [sp, #4]
	str r5, [sp, #0]
	bl Func_02002dfc
	movs r3, #71
	str r3, [sp, #4]
	movs r0, #22
	movs r1, #88
	movs r2, #15
	movs r3, #7
	str r5, [sp, #0]
	bl Func_02002dfc
	movs r3, #68
	str r3, [sp, #0]
	movs r0, #69
	movs r1, #6
	movs r2, #1
	movs r3, #2
	str r6, [sp, #4]
	bl Func_02002e04
.L_ja_0200aa4e:
	mov r1, r10
	ldrh r3, [r1]
	movs r2, #128
	subs r3, #3
	lsls r3, r3, #16
	lsls r2, r2, #9
	cmp r3, r2
	bhi .L_ja_0200aabc
	movs r5, #4
	movs r6, #23
	movs r0, #25
	movs r1, #22
	movs r2, #9
	movs r3, #2
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_02002dfc
	movs r3, #87
	str r3, [sp, #4]
	movs r0, #25
	movs r1, #86
	movs r2, #9
	movs r3, #2
	str r5, [sp, #0]
	bl Func_02002dfc
	movs r3, #25
	str r3, [sp, #4]
	movs r5, #1
	movs r0, #22
	movs r1, #24
	movs r2, #15
	movs r3, #7
	str r5, [sp, #0]
	bl Func_02002dfc
	movs r3, #89
	str r3, [sp, #4]
	movs r0, #22
	movs r1, #88
	movs r2, #15
	movs r3, #7
	str r5, [sp, #0]
	bl Func_02002dfc
	movs r3, #68
	str r3, [sp, #0]
	movs r0, #69
	movs r1, #24
	movs r2, #1
	movs r3, #4
	str r6, [sp, #4]
	bl Func_02002e04
.L_ja_0200aabc:
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r7, r1
	ldr r0, [r3]
	bl Actor_GetById
	mov r2, r8
	adds r5, r0, #0
	str r2, [r5, #20]
	str r2, [r5, #12]
	movs r0, #1
	bl FieldEvent_Wait
	bl Func_02002dec
	b .L_ja_0200ab02
.L_ja_0200aadc:
	ldr r3, .L_ja_0200ab20
	cmp r2, r3
	bne .L_ja_0200ab02
	movs r0, #64
	bl Actor_GetById
	adds r5, r0, #0
	cmp r5, #0
	beq .L_ja_0200ab02
	adds r2, r5, #0
	movs r3, #0
	adds r2, #85
	strb r3, [r2]
	movs r3, #144
	lsls r3, r3, #15
	str r3, [r5, #20]
	str r3, [r5, #12]
	ldr r3, .L_ja_0200ab24
	str r3, [r5, #108]
.L_ja_0200ab02:
	movs r0, #0
	add sp, #8
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_ja_0200ab10:
	.4byte 0x000000c1
.L_ja_0200ab14:
	.4byte Func_020010f4
.L_ja_0200ab18:
	.4byte 0x000000c2
.L_ja_0200ab1c:
	.4byte Data_02004030
.L_ja_0200ab20:
	.4byte 0x000000c4
.L_ja_0200ab24:
	.4byte Func_020023d0
	.endif
	.section .bss,"aw",%nobits
	.balign 16
	.global Data_02004030
Data_02004030:
