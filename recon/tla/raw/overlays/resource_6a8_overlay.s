.syntax unified
	.thumb
	.section .text.x0200807e,"ax",%progbits
	.2byte 0x0000
	.section .text.x020080b6,"ax",%progbits
	.2byte 0x0000
	.section .text.x020082d8,"ax",%progbits
	.global Func_020002d8
	.thumb_func
Func_020002d8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	adds r5, r0, #0
	mov r9, r3
	adds r3, r5, #0
	adds r3, #99
	ldrb r3, [r3]
	mov r10, r2
	movs r2, #0
	mov r8, r1
	mov r11, r2
	cmp r3, #0
	beq .L_02008308
	adds r2, r5, #0
	adds r2, #91
	movs r3, #1
	strb r3, [r2]
	movs r0, #1
	b .L_020083ac
.L_02008308:
	mov r6, r8
	adds r7, r5, #0
	adds r6, #8
	adds r7, #8
	adds r0, r6, #0
	adds r1, r7, #0
	bl SceneActor_CalculateFixedPointDistance
	cmp r0, r10
	blt .L_02008322
	mov r3, r9
	cmp r3, #0
	beq .L_0200839a
.L_02008322:
	mov r2, r8
	ldr r0, [r2, #16]
	ldr r3, [r5, #16]
	ldr r1, [r6]
	subs r0, r0, r3
	ldr r3, [r7]
	subs r1, r1, r3
	bl ArcTan2
	ldr r3, .L_020083b8
	lsls r0, r0, #16
	movs r2, #128
	lsrs r0, r0, #16
	lsls r2, r2, #5
	adds r1, r0, r2
	ldrh r2, [r5, #6]
	adds r4, r0, r3
	movs r3, #240
	lsls r3, r3, #8
	ands r4, r3
	ands r1, r3
	ands r0, r3
	ands r3, r2
	cmp r0, r3
	beq .L_02008362
	cmp r1, r3
	beq .L_02008362
	cmp r4, r3
	beq .L_02008362
	mov r3, r9
	cmp r3, #0
	beq .L_020083aa
.L_02008362:
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #194
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	adds r2, r5, #0
	adds r2, #91
	cmp r3, #120
	ble .L_02008388
	movs r3, #0
	strb r3, [r2]
	adds r0, r5, #0
	movs r1, #2
	bl Func_020043c4
	b .L_020083aa
.L_02008388:
	movs r3, #1
	strb r3, [r2]
	adds r0, r5, #0
	movs r1, #1
	bl Func_020043c4
	movs r3, #1
	mov r11, r3
	b .L_020083aa
.L_0200839a:
	adds r3, r5, #0
	adds r3, #91
	mov r2, r11
	strb r2, [r3]
	adds r0, r5, #0
	movs r1, #2
	bl Func_020043c4
.L_020083aa:
	mov r0, r11
.L_020083ac:
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_020083b8:
	.4byte 0xfffff000
	.section .text.x020083bc,"ax",%progbits
	.ifdef TLA_EDITION_JA
	.global Func_020003bc
	.thumb_func
Func_020003bc:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #60]
	adds r5, r0, #0
	adds r6, r5, #0
	ldr r7, [r3, #108]
	adds r6, #100
	movs r3, #0
	mov r8, r2
	mov r10, r3
	movs r2, #0
	ldrsh r3, [r6, r2]
	cmp r3, #0
	bne .L_ja_020083e4
	movs r0, #18
	b .L_ja_020083ee
.L_ja_020083e4:
	cmp r3, #1
	bne .L_ja_020083ec
	movs r0, #16
	b .L_ja_020083ee
.L_ja_020083ec:
	movs r0, #17
.L_ja_020083ee:
	bl Actor_GetById
	adds r1, r0, #0
	adds r0, r5, #0
	movs r2, #24
	movs r3, #0
	bl Func_020002d8
	cmp r0, #0
	bne .L_ja_02008440
	ldr r3, .L_ja_0200844c
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Actor_GetById
	movs r2, #176
	lsls r2, r2, #1
	adds r3, r7, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	adds r1, r0, #0
	cmp r3, #0
	bne .L_ja_02008428
	mov r2, r8
	ldrb r3, [r2, #4]
	cmp r3, #0
	beq .L_ja_02008436
.L_ja_02008428:
	ldrh r2, [r6]
	movs r3, #2
	ands r3, r2
	cmp r3, #0
	beq .L_ja_02008436
	movs r3, #1
	mov r10, r3
.L_ja_02008436:
	adds r0, r5, #0
	movs r2, #32
	mov r3, r10
	bl Func_020002d8
.L_ja_02008440:
	movs r0, #0
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_ja_0200844c:
	.4byte gSequenceParentWork
	.endif
	.section .text.x0200848c,"ax",%progbits
	.global Func_0200048c
	.thumb_func
Func_0200048c:
	push {lr}
	ldr r3, .L_02008544
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008548
	cmp r2, r3
	bne .L_020084c0
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #197
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020084c0
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #204
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020084c0
	ldr r0, .L_0200854c
	b .L_02008542
.L_020084c0:
	ldr r3, .L_02008544
	movs r2, #240
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008550
	cmp r2, r3
	bne .L_020084d6
	ldr r0, .L_02008554
	b .L_02008542
.L_020084d6:
	ldr r3, .L_02008558
	cmp r2, r3
	bne .L_020084f2
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #61
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020084ee
	ldr r0, .L_0200855c
	b .L_02008542
.L_020084ee:
	ldr r0, .L_02008560
	b .L_02008542
.L_020084f2:
	ldr r3, .L_02008564
	cmp r2, r3
	bne .L_0200851a
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #61
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008516
	ldr r0, .L_02008568
	movs r3, #3
	adds r2, r0, #0
	adds r2, #46
	strb r3, [r2]
	adds r2, #72
	strb r3, [r2]
	b .L_02008542
.L_02008516:
	ldr r0, .L_0200856c
	b .L_02008542
.L_0200851a:
	ldr r3, .L_02008570
	cmp r2, r3
	bne .L_02008536
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #61
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008532
	ldr r0, .L_02008574
	b .L_02008542
.L_02008532:
	ldr r0, .L_02008578
	b .L_02008542
.L_02008536:
	ldr r3, .L_0200857c
	cmp r2, r3
	bne .L_02008540
	ldr r0, .L_02008580
	b .L_02008542
.L_02008540:
	ldr r0, .L_02008584
.L_02008542:
	pop {pc}
.L_02008544:
	.4byte gSequenceParentWork
.L_02008548:
	.4byte 0x0000010d
.L_0200854c:
	.4byte gData_02004b2c
.L_02008550:
	.4byte 0x0000010e
.L_02008554:
	.4byte gData_02004c4c
.L_02008558:
	.4byte 0x0000010f
.L_0200855c:
	.4byte gData_02004f04
.L_02008560:
	.4byte gData_02004d9c
.L_02008564:
	.4byte 0x00000110
.L_02008568:
	.4byte gData_0200521c
.L_0200856c:
	.4byte gData_0200506c
.L_02008570:
	.4byte 0x00000111
.L_02008574:
	.4byte gData_020054a4
.L_02008578:
	.4byte gData_02005444
.L_0200857c:
	.4byte 0x00000112
.L_02008580:
	.4byte gData_0200551c
.L_02008584:
	.4byte gData_02004b14
	.section .text.x020095d0,"ax",%progbits
	.global Func_020015d0
	.thumb_func
Func_020015d0:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r8
	push {r5, r6}
	sub sp, #8
	bl Func_02004464
	movs r0, #0
	bl Func_020045ec
	ldr r6, .L_0200976c
	movs r2, #133
	lsls r2, r2, #2
	adds r6, r6, r2
	movs r1, #128
	movs r2, #128
	ldr r0, [r6]
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl Actor_SetSpeedParameters
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #9
	lsls r1, r1, #10
	movs r0, #20
	bl Actor_SetSpeedParameters
	movs r0, #20
	bl Actor_GetById
	movs r3, #1
	mov r8, r3
	adds r0, #34
	movs r3, #1
	strb r3, [r0]
	movs r1, #3
	movs r0, #20
	bl Actor_SetSpritePriority
	ldr r0, [r6]
	bl Actor_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r5, #254
	adds r3, r5, #0
	ands r3, r2
	strb r3, [r0]
	movs r1, #204
	movs r2, #163
	lsls r1, r1, #1
	lsls r2, r2, #1
	ldr r0, [r6]
	bl Actor_SetPositionAndReset
	movs r0, #1
	bl FieldEvent_Wait
	ldr r0, [r6]
	bl Actor_GetById
	adds r0, #90
	ldrb r3, [r0]
	mov r2, r8
	orrs r3, r2
	strb r3, [r0]
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	ldr r0, [r6]
	bl Actor_ArmCallback
	movs r0, #161
	bl Func_0200463c
	bl Func_02004584
	movs r2, #128
	ldr r3, [r0, #12]
	lsls r2, r2, #10
	mov r10, r2
	add r3, r10
	str r3, [r0, #12]
	movs r0, #20
	bl Actor_GetById
	adds r0, #90
	ldrb r3, [r0]
	movs r1, #204
	ands r5, r3
	lsls r1, r1, #1
	movs r2, #200
	strb r5, [r0]
	movs r0, #20
	bl Actor_SetPositionAndReset
	movs r0, #1
	bl FieldEvent_Wait
	movs r0, #20
	bl Actor_GetById
	adds r0, #90
	ldrb r3, [r0]
	mov r2, r8
	orrs r3, r2
	strb r3, [r0]
	movs r1, #0
	movs r2, #0
	movs r0, #20
	bl Func_020044cc
	movs r0, #176
	bl Func_0200463c
	movs r0, #128
	movs r2, #128
	mov r1, r10
	lsls r2, r2, #9
	lsls r0, r0, #9
	bl Func_02004444
	movs r0, #30
	bl FieldEvent_Wait
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	negs r1, r1
	adds r2, #102
	negs r0, r0
	bl Func_02004444
	movs r0, #127
	bl Func_0200463c
	movs r0, #30
	bl FieldEvent_Wait
	movs r0, #146
	bl Func_0200463c
	movs r0, #20
	bl FieldEvent_Wait
	movs r1, #6
	movs r2, #40
	ldr r0, [r6]
	adds r1, #255
	bl Func_0200455c
	movs r1, #2
	movs r0, #20
	bl Actor_SetSpritePriority
	movs r0, #20
	bl Actor_GetById
	adds r0, #35
	ldrb r3, [r0]
	mov r2, r8
	orrs r2, r3
	strb r2, [r0]
	movs r5, #3
	mov r8, r2
	movs r0, #72
	movs r1, #3
	movs r2, #69
	movs r3, #3
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl Func_02004404
	movs r3, #2
	str r3, [sp, #4]
	movs r0, #48
	movs r1, #36
	movs r2, #69
	movs r3, #8
	str r5, [sp, #0]
	bl Func_02004404
	movs r3, #70
	movs r2, #20
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #13
	movs r2, #1
	movs r3, #3
	movs r0, #72
	bl Func_0200442c
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #45
	bl GameFlag_SetBit
	bl Func_0200446c
	add sp, #8
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, pc}
.L_0200976c:
	.4byte gSequenceParentWork
	.section .text.x0200b91c,"ax",%progbits
	.balign 4
	.global Func_0200391c
	.thumb_func
Func_0200391c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, .L_0200b9dc
	movs r0, #133
	lsls r0, r0, #2
	adds r3, r3, r0
	ldr r0, [r3]
	sub sp, #40
	bl ObjectTable_Get
	adds r7, r0, #0
.L_0200b93c:
	bl Func_020038e8
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
	ldr r1, .L_0200b9e0
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
	bl Func_02004424
	str r0, [sp, #24]
	ldr r2, [sp, #20]
	ldr r1, [r7, #8]
	ldr r0, [sp, #16]
	ldr r3, .L_0200b9e4
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
	bge .L_0200b9f4
	ldr r3, .L_0200b9e8
	movs r2, #15
	ldr r3, [r3]
	ldr r1, .L_0200b9ec
	lsrs r3, r3, #4
	ands r3, r2
	lsls r3, r3, #1
	ldrsh r2, [r1, r3]
	mov r9, r2
	lsls r3, r2, #16
	ldr r2, .L_0200b9f0
	cmp r3, r2
	bne .L_0200ba20
	b .L_0200bbb6
.L_0200b9dc:
	.4byte gSequenceParentWork
.L_0200b9e0:
	.4byte 0xfff00000
.L_0200b9e4:
	.4byte IwramMulQ16
.L_0200b9e8:
	.4byte gInput
.L_0200b9ec:
	.4byte gData_02004780
.L_0200b9f0:
	.4byte 0xffff0000
.L_0200b9f4:
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
	ldr r2, .L_0200ba1c
	mov r9, r0
	lsls r3, r3, #6
	add r3, r9
	ands r3, r2
	lsls r3, r3, #16
	asrs r3, r3, #16
	mov r9, r3
	b .L_0200ba20
.L_0200ba1c:
	.4byte 0xffffc000
.L_0200ba20:
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
	bl Func_02004424
	mov r11, r0
	cmp r0, #255
	beq .L_0200baa2
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
	bgt .L_0200baa2
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
	bl Func_020043f4
	adds r0, r7, #0
	movs r1, #2
	bl Func_020043c4
	adds r0, r7, #0
	movs r1, #48
	bl Actor_SetAnimationSpeed
	adds r0, r7, #0
	bl Func_020043fc
	ldr r3, .L_0200bbc4
	str r3, [r7, #108]
	b .L_0200bb4c
.L_0200baa2:
	movs r3, #0
	mov r0, r9
	strh r0, [r7, #6]
	str r3, [r7, #36]
	str r3, [r7, #44]
	ldr r2, [sp, #12]
	str r2, [r7, #8]
	ldr r3, [sp, #8]
	str r3, [r7, #16]
	b .L_0200bb98
.L_0200bab6:
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
	bgt .L_0200bb6c
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
.L_0200bae4:
	ldr r3, [r5]
	cmp r3, #0
	beq .L_0200bb0e
	mov r3, r8
	ldrb r2, [r3]
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	beq .L_0200bb0e
	cmp r5, r7
	beq .L_0200bb0e
	ldrh r3, [r5, #32]
	adds r0, r5, #0
	adds r0, #8
	subs r3, #2
	ldr r1, [sp, #0]
	add r2, sp, #28
	bl Func_02004454
	cmp r0, #0
	bge .L_0200bb6c
.L_0200bb0e:
	movs r0, #1
	add r10, r0
	movs r2, #128
	mov r3, r10
	add r8, r2
	adds r5, #128
	cmp r3, #63
	ble .L_0200bae4
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
	bl Func_020043f4
	adds r0, r7, #0
	bl Func_020043fc
	ldr r3, [sp, #24]
	cmp r11, r3
	bne .L_0200bb92
.L_0200bb4c:
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
	bl Func_02004424
	mov r11, r0
	cmp r0, #255
	bne .L_0200bab6
.L_0200bb6c:
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
	bl Func_020043f4
	adds r0, r7, #0
	bl Func_020043fc
	movs r0, #2
	bl wait
	b .L_0200b93c
.L_0200bb92:
	movs r0, #10
	bl wait
.L_0200bb98:
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
	bl Func_020043c4
.L_0200bbb6:
	add sp, #40
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0200bbc4:
	.4byte Func_0200385c
	.section .text.x0200bbc8,"ax",%progbits
	.ifdef TLA_EDITION_JA
	.global Func_02003bc8
	.thumb_func
Func_02003bc8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, .L_ja_0200bc00
	movs r0, #133
	lsls r0, r0, #2
	adds r3, r3, r0
	ldr r0, [r3]
	sub sp, #40
	bl ObjectTable_Get
	ldr r2, .L_ja_0200bc04
	ldr r3, .L_ja_0200bbfc
	adds r7, r0, #0
	strh r3, [r2]
.L_ja_0200bbc6:
	bl Func_020038e8
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
	ldr r1, .L_ja_0200bc08
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
	b .L_ja_0200bc0c
.L_ja_0200bbfc:
	.4byte 0x00000000
.L_ja_0200bc00:
	.4byte gSequenceParentWork
.L_ja_0200bc04:
	.4byte gData_020062a4
.L_ja_0200bc08:
	.4byte 0xfff00000
.L_ja_0200bc0c:
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
	bl Func_02004424
	str r0, [sp, #24]
	ldr r2, [sp, #20]
	ldr r1, [r7, #8]
	ldr r0, [sp, #16]
	ldr r3, .L_ja_0200bc78
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
	bge .L_ja_0200bc88
	ldr r3, .L_ja_0200bc7c
	movs r2, #15
	ldr r3, [r3]
	ldr r1, .L_ja_0200bc80
	lsrs r3, r3, #4
	ands r3, r2
	lsls r3, r3, #1
	ldrsh r2, [r1, r3]
	mov r9, r2
	lsls r3, r2, #16
	ldr r2, .L_ja_0200bc84
	cmp r3, r2
	bne .L_ja_0200bcb4
	b .L_ja_0200be7c
.L_ja_0200bc78:
	.4byte IwramMulQ16
.L_ja_0200bc7c:
	.4byte gInput
.L_ja_0200bc80:
	.4byte gData_02004780
.L_ja_0200bc84:
	.4byte 0xffff0000
.L_ja_0200bc88:
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
	ldr r2, .L_ja_0200bcb0
	mov r9, r0
	lsls r3, r3, #6
	add r3, r9
	ands r3, r2
	lsls r3, r3, #16
	asrs r3, r3, #16
	mov r9, r3
	b .L_ja_0200bcb4
.L_ja_0200bcb0:
	.4byte 0xffffc000
.L_ja_0200bcb4:
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
	bl Func_02004424
	mov r11, r0
	cmp r0, #255
	beq .L_ja_0200bd36
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
	bgt .L_ja_0200bd36
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
	bl Func_020043f4
	adds r0, r7, #0
	movs r1, #2
	bl Func_020043c4
	adds r0, r7, #0
	movs r1, #48
	bl Actor_SetAnimationSpeed
	adds r0, r7, #0
	bl Func_020043fc
	ldr r3, .L_ja_0200be8c
	str r3, [r7, #108]
	b .L_ja_0200be12
.L_ja_0200bd36:
	movs r3, #0
	mov r0, r9
	strh r0, [r7, #6]
	str r3, [r7, #36]
	str r3, [r7, #44]
	ldr r2, [sp, #12]
	str r2, [r7, #8]
	ldr r3, [sp, #8]
	str r3, [r7, #16]
	b .L_ja_0200be5e
.L_ja_0200bd4a:
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
	bgt .L_ja_0200be32
	ldr r3, .L_ja_0200be90
	ldrh r3, [r3]
	lsls r3, r3, #16
	cmp r3, #0
	bne .L_ja_0200be32
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
.L_ja_0200bd82:
	ldr r3, [r5]
	cmp r3, #0
	beq .L_ja_0200bdac
	mov r3, r8
	ldrb r2, [r3]
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	beq .L_ja_0200bdac
	cmp r5, r7
	beq .L_ja_0200bdac
	ldrh r3, [r5, #32]
	adds r0, r5, #0
	adds r0, #8
	subs r3, #2
	ldr r1, [sp, #0]
	add r2, sp, #28
	bl Func_02004454
	cmp r0, #0
	bge .L_ja_0200be32
.L_ja_0200bdac:
	movs r0, #1
	add r10, r0
	movs r2, #128
	mov r3, r10
	add r8, r2
	adds r5, #128
	cmp r3, #63
	ble .L_ja_0200bd82
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
	bl Func_020043f4
	b .L_ja_0200bdea
.L_ja_0200bde2:
	movs r0, #1
	bl wait
	adds r5, #1
.L_ja_0200bdea:
	cmp r5, #179
	bgt .L_ja_0200be02
	adds r0, r7, #0
	bl Func_0200444c
	cmp r0, #0
	bne .L_ja_0200be02
	ldr r3, .L_ja_0200be90
	ldrh r3, [r3]
	lsls r3, r3, #16
	cmp r3, #0
	beq .L_ja_0200bde2
.L_ja_0200be02:
	ldr r3, .L_ja_0200be90
	ldrh r3, [r3]
	lsls r3, r3, #16
	cmp r3, #0
	bne .L_ja_0200be5e
	ldr r3, [sp, #24]
	cmp r11, r3
	bne .L_ja_0200be58
.L_ja_0200be12:
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
	bl Func_02004424
	mov r11, r0
	cmp r0, #255
	bne .L_ja_0200bd4a
.L_ja_0200be32:
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
	bl Func_020043f4
	adds r0, r7, #0
	bl Func_020043fc
	movs r0, #2
	bl wait
	b .L_ja_0200bbc6
.L_ja_0200be58:
	movs r0, #10
	bl wait
.L_ja_0200be5e:
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
	bl Func_020043c4
.L_ja_0200be7c:
	add sp, #40
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_ja_0200be8c:
	.4byte Func_0200385c
.L_ja_0200be90:
	.4byte gData_020062a4
	.else
	.global Func_02003bc8
	.thumb_func
Func_02003bc8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, .L_0200bc28
	movs r0, #133
	lsls r0, r0, #2
	adds r3, r3, r0
	ldr r0, [r3]
	sub sp, #40
	bl ObjectTable_Get
	ldr r2, .L_0200bc2c
	ldr r3, .L_0200bc24
	adds r7, r0, #0
	strh r3, [r2]
.L_0200bbee:
	bl Func_020038e8
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
	ldr r1, .L_0200bc30
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
	b .L_0200bc34
.L_0200bc24:
	.4byte 0x00000000
.L_0200bc28:
	.4byte gSequenceParentWork
.L_0200bc2c:
	.4byte gData_020062a4
.L_0200bc30:
	.4byte 0xfff00000
.L_0200bc34:
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
	bl Func_02004424
	str r0, [sp, #24]
	ldr r2, [sp, #20]
	ldr r1, [r7, #8]
	ldr r0, [sp, #16]
	ldr r3, .L_0200bca0
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
	bge .L_0200bcb0
	ldr r3, .L_0200bca4
	movs r2, #15
	ldr r3, [r3]
	ldr r1, .L_0200bca8
	lsrs r3, r3, #4
	ands r3, r2
	lsls r3, r3, #1
	ldrsh r2, [r1, r3]
	mov r9, r2
	lsls r3, r2, #16
	ldr r2, .L_0200bcac
	cmp r3, r2
	bne .L_0200bcdc
	b .L_0200bea6
.L_0200bca0:
	.4byte IwramMulQ16
.L_0200bca4:
	.4byte gInput
.L_0200bca8:
	.4byte gData_02004780
.L_0200bcac:
	.4byte 0xffff0000
.L_0200bcb0:
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
	ldr r2, .L_0200bcd8
	mov r9, r0
	lsls r3, r3, #6
	add r3, r9
	ands r3, r2
	lsls r3, r3, #16
	asrs r3, r3, #16
	mov r9, r3
	b .L_0200bcdc
.L_0200bcd8:
	.4byte 0xffffc000
.L_0200bcdc:
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
	bl Func_02004424
	mov r11, r0
	cmp r0, #255
	beq .L_0200bd56
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
	bgt .L_0200bd56
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
	bl Func_020043f4
	adds r0, r7, #0
	movs r1, #2
	bl Func_020043c4
	adds r0, r7, #0
	movs r1, #48
	bl Actor_SetAnimationSpeed
	movs r5, #0
	b .L_0200bd7e
.L_0200bd56:
	movs r3, #0
	mov r0, r9
	strh r0, [r7, #6]
	str r3, [r7, #36]
	str r3, [r7, #44]
	ldr r2, [sp, #12]
	str r2, [r7, #8]
	ldr r3, [sp, #8]
	str r3, [r7, #16]
	b .L_0200bea6
.L_0200bd6a:
	ldr r3, .L_0200bed4
	ldrh r3, [r3]
	lsls r3, r3, #16
	cmp r3, #0
	beq .L_0200bd76
	b .L_0200bea6
.L_0200bd76:
	movs r0, #1
	bl wait
	adds r5, #1
.L_0200bd7e:
	cmp r5, #179
	bgt .L_0200bd8c
	adds r0, r7, #0
	bl Func_0200444c
	cmp r0, #0
	beq .L_0200bd6a
.L_0200bd8c:
	ldr r3, .L_0200bed8
	str r3, [r7, #108]
	b .L_0200be5a
.L_0200bd92:
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
	bgt .L_0200be7a
	ldr r3, .L_0200bed4
	ldrh r3, [r3]
	lsls r3, r3, #16
	cmp r3, #0
	bne .L_0200bea6
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
.L_0200bdca:
	ldr r3, [r5]
	cmp r3, #0
	beq .L_0200bdf4
	mov r3, r8
	ldrb r2, [r3]
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	beq .L_0200bdf4
	cmp r5, r7
	beq .L_0200bdf4
	ldrh r3, [r5, #32]
	adds r0, r5, #0
	adds r0, #8
	subs r3, #2
	ldr r1, [sp, #0]
	add r2, sp, #28
	bl Func_02004454
	cmp r0, #0
	bge .L_0200be7a
.L_0200bdf4:
	movs r0, #1
	add r10, r0
	movs r2, #128
	mov r3, r10
	add r8, r2
	adds r5, #128
	cmp r3, #63
	ble .L_0200bdca
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
	bl Func_020043f4
	b .L_0200be32
.L_0200be2a:
	movs r0, #1
	bl wait
	adds r5, #1
.L_0200be32:
	cmp r5, #179
	bgt .L_0200be4a
	adds r0, r7, #0
	bl Func_0200444c
	cmp r0, #0
	bne .L_0200be4a
	ldr r3, .L_0200bed4
	ldrh r3, [r3]
	lsls r3, r3, #16
	cmp r3, #0
	beq .L_0200be2a
.L_0200be4a:
	ldr r3, .L_0200bed4
	ldrh r3, [r3]
	lsls r3, r3, #16
	cmp r3, #0
	bne .L_0200bea6
	ldr r3, [sp, #24]
	cmp r11, r3
	bne .L_0200bea0
.L_0200be5a:
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
	bl Func_02004424
	mov r11, r0
	cmp r0, #255
	bne .L_0200bd92
.L_0200be7a:
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
	bl Func_020043f4
	adds r0, r7, #0
	bl Func_020043fc
	movs r0, #2
	bl wait
	b .L_0200bbee
.L_0200bea0:
	movs r0, #10
	bl wait
.L_0200bea6:
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
	bl Func_020043c4
	add sp, #40
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200bed4:
	.4byte gData_020062a4
.L_0200bed8:
	.4byte Func_0200385c
	.endif
	.section .text.x0200bee8,"ax",%progbits
	.global Func_02003ee8
	.thumb_func
Func_02003ee8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, .L_0200bf54
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
	ldr r2, .L_0200bf50
	ands r3, r2
	lsls r3, r3, #16
	asrs r3, r3, #16
	str r3, [sp, #16]
.L_0200bf1a:
	bl Func_020038e8
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
	ldr r2, .L_0200bf58
	ldr r3, [r5, #8]
	movs r1, #128
	lsls r1, r1, #12
	ands r3, r2
	mov r9, r1
	add r6, sp, #20
	add r3, r9
	str r3, [r6]
	mov r8, r3
	b .L_0200bf5c
	.2byte 0x0000
.L_0200bf50:
	.4byte 0xffffc000
.L_0200bf54:
	.4byte gSequenceParentWork
.L_0200bf58:
	.4byte 0xfff00000
.L_0200bf5c:
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
	bl Func_02004424
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
	bl Func_02004424
	mov r10, r0
	cmp r0, #255
	beq .L_0200bff0
	mov r2, r11
	ldrb r0, [r2]
	ldr r1, [r6]
	ldr r2, [r6, #8]
	bl Map_GetTerrainHeight
	ldr r3, [r5, #12]
	subs r0, r0, r3
	cmp r0, r9
	bgt .L_0200bff0
	ldr r3, [sp, #8]
	ldr r2, .L_0200bfe8
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
	bl Func_020043c4
	adds r0, r5, #0
	movs r1, #48
	bl Actor_SetAnimationSpeed
	ldr r3, .L_0200bfec
	str r3, [r5, #108]
	b .L_0200c09a
	.2byte 0x0000
.L_0200bfe8:
	.4byte 0x00000000
.L_0200bfec:
	.4byte Func_0200385c
.L_0200bff0:
	add r1, sp, #16
	ldrh r1, [r1]
	movs r3, #0
	mov r2, r8
	strh r1, [r5, #6]
	str r3, [r5, #36]
	str r3, [r5, #44]
	str r2, [r5, #8]
	str r7, [r5, #16]
	b .L_0200c0e6
.L_0200c004:
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
	bgt .L_0200c0ba
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
.L_0200c032:
	ldr r3, [r6]
	cmp r3, #0
	beq .L_0200c05c
	mov r1, r8
	ldrb r2, [r1]
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	beq .L_0200c05c
	cmp r6, r5
	beq .L_0200c05c
	ldrh r3, [r6, #32]
	adds r0, r6, #0
	adds r0, #8
	subs r3, #2
	ldr r1, [sp, #0]
	add r2, sp, #20
	bl Func_02004454
	cmp r0, #0
	bge .L_0200c0ba
.L_0200c05c:
	movs r2, #1
	add r9, r2
	movs r3, #128
	mov r1, r9
	add r8, r3
	adds r6, #128
	cmp r1, #63
	ble .L_0200c032
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
	bl Func_020043f4
	adds r0, r5, #0
	bl Func_020043fc
	ldr r1, [sp, #12]
	cmp r10, r1
	bne .L_0200c0e0
.L_0200c09a:
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
	bl Func_02004424
	mov r10, r0
	cmp r0, #255
	bne .L_0200c004
.L_0200c0ba:
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
	bl Func_020043f4
	adds r0, r5, #0
	bl Func_020043fc
	movs r0, #2
	bl wait
	b .L_0200bf1a
.L_0200c0e0:
	movs r0, #10
	bl wait
.L_0200c0e6:
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
	bl Func_020043c4
	add sp, #32
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x0200c114,"ax",%progbits
	.global Func_02004114
	.thumb_func
Func_02004114:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r7, [r3]
	movs r2, #160
	lsls r2, r2, #3
	movs r3, #192
	adds r5, r7, r2
	lsls r3, r3, #4
	movs r2, #63
	adds r6, r7, r3
	mov r8, r2
.L_0200c132:
	ldr r3, [r5, #24]
	cmp r3, #19
	bhi .L_0200c180
	movs r2, #176
	lsls r2, r2, #5
	adds r2, #2
	adds r1, r7, r2
	ldrh r1, [r1]
	movs r2, #7
	asrs r3, r3, #2
	ands r3, r2
	lsls r3, r3, #3
	adds r1, r1, r3
	ldr r3, .L_0200c174
	ldr r2, .L_0200c178
	ands r1, r3
	ldrh r3, [r6, #8]
	adds r0, r6, #0
	ands r3, r2
	orrs r3, r1
	strh r3, [r6, #8]
	adds r1, r5, #0
	bl Func_0200460c
	adds r0, r5, #0
	movs r1, #63
	ldr r2, .L_0200c17c
	bl Func_02004614
	ldr r3, [r5, #24]
	adds r3, #1
	str r3, [r5, #24]
	b .L_0200c180
.L_0200c174:
	.4byte 0x000003ff
.L_0200c178:
	.4byte 0xfffffc00
.L_0200c17c:
	.4byte 0xffff8000
.L_0200c180:
	movs r3, #1
	negs r3, r3
	add r8, r3
	mov r2, r8
	adds r6, #40
	adds r5, #28
	cmp r2, #0
	bge .L_0200c132
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x0200c254,"ax",%progbits
	.balign 4
	.global Func_02004254
	.thumb_func
Func_02004254:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r1, #176
	lsls r1, r1, #5
	adds r1, #8
	movs r0, #220
	sub sp, #4
	bl HeapSlot_AllocFar
	adds r6, r0, #0
	ldr r0, .L_0200c308
	bl Resource_GetTableEntry
	adds r1, r6, #0
	bl Func_02004384
	bl Resource_FindFreeEntry
	movs r1, #160
	lsls r1, r1, #3
	adds r2, r6, #0
	adds r5, r0, #0
	bl VramCache_Load
	movs r1, #176
	lsls r1, r1, #5
	adds r1, #2
	mov r10, r0
	adds r3, r6, r1
	mov r2, r10
	adds r1, #2
	strh r2, [r3]
	adds r3, r6, r1
	strh r5, [r3]
	movs r2, #160
	movs r3, #192
	lsls r2, r2, #3
	lsls r3, r3, #4
	movs r1, #63
	adds r7, r6, r2
	adds r5, r6, r3
	mov r8, r1
.L_0200c2ac:
	mov r2, r10
	movs r3, #128
	str r2, [sp, #0]
	adds r0, r5, #0
	movs r1, #8
	movs r2, #8
	lsls r3, r3, #23
	bl Func_02004604
	ldrb r3, [r5, #5]
	movs r2, #32
	orrs r3, r2
	ldrb r2, [r5, #9]
	movs r1, #13
	strb r3, [r5, #5]
	negs r1, r1
	movs r3, #15
	ands r3, r2
	adds r2, r1, #0
	ands r3, r2
	strb r3, [r5, #9]
	movs r3, #240
	strh r3, [r5, #30]
	subs r3, #241
	add r8, r3
	mov r2, r8
	str r3, [r7, #24]
	adds r5, #40
	adds r7, #28
	cmp r2, #0
	bge .L_0200c2ac
	movs r1, #176
	lsls r1, r1, #5
	adds r2, r6, r1
	movs r3, #0
	movs r1, #144
	strh r3, [r2]
	lsls r1, r1, #3
	ldr r0, .L_0200c30c
	bl Task_Add
	add sp, #4
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_0200c308:
	.4byte 0x000001f0
.L_0200c30c:
	.4byte Func_02004114
	.section .bss,"aw",%nobits
	.global gData_020062a4
gData_020062a4:
