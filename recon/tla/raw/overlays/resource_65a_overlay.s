.syntax unified
	.thumb
	.section .text.x02008038,"ax",%progbits
	.global Func_02000038
	.thumb_func
Func_02000038:
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
	sub sp, #8
	str r3, [sp, #4]
	movs r2, #255
	ldrh r3, [r0]
	lsls r2, r2, #8
	adds r2, #255
	mov r11, r0
	cmp r3, r2
	beq .L_02008108
.L_0200805e:
	mov r3, r11
	ldrh r0, [r3]
	bl Object_GetById
	mov r8, r0
	mov r7, r8
	adds r7, #34
	mov r2, r8
	ldr r1, [r2, #8]
	ldrb r0, [r7]
	ldr r2, [r2, #16]
	bl Func_020010ec
	str r0, [sp, #0]
	mov r3, r8
	ldr r1, [r3, #8]
	ldr r2, [r3, #16]
	ldrb r0, [r7]
	bl Map_GetTerrainHeight
	mov r10, r0
	mov r2, r10
	asrs r2, r2, #19
	mov r0, r8
	mov r10, r2
	bl Func_020011d4
	ldrb r2, [r7]
	mov r9, r0
	lsls r3, r2, #3
	subs r3, r3, r2
	movs r2, #156
	lsls r2, r2, #1
	lsls r3, r3, #3
	adds r3, r3, r2
	ldr r2, [sp, #4]
	mov r0, r8
	ldr r6, [r2, r3]
	ldr r3, .L_02008140
	ldr r2, .L_02008144
	adds r5, r6, r3
	asrs r5, r5, #2
	adds r5, r5, r2
	mov r2, r8
	movs r3, #0
	adds r2, #85
	strb r3, [r2]
	adds r2, #4
	strb r3, [r2]
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r3, #4
	add r10, r3
	mov r2, r10
	ldrb r0, [r7]
	ldr r1, [sp, #0]
	bl Func_020011f4
	mov r2, r9
	lsls r2, r2, #2
	add r5, r9
	mov r9, r2
	add r6, r9
	ldrb r3, [r6, #3]
	movs r2, #192
	orrs r3, r2
	strb r3, [r6, #3]
	movs r3, #128
	lsls r3, r3, #2
	adds r6, r6, r3
	ldrb r2, [r6, #3]
	movs r3, #64
	orrs r3, r2
	movs r2, #2
	add r11, r2
	mov r2, r11
	strb r3, [r6, #3]
	ldrh r3, [r2]
	movs r2, #255
	lsls r2, r2, #8
	adds r2, #255
	strb r0, [r5]
	cmp r3, r2
	bne .L_0200805e
.L_02008108:
	ldr r3, .L_02008148
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
	bge .L_02008130
	str r0, [r5, #20]
	str r0, [r5, #12]
.L_02008130:
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008140:
	.4byte 0xfdff0000
.L_02008144:
	.4byte gMapShapeGrid
.L_02008148:
	.4byte gPartyState
	.section .text.x0200814c,"ax",%progbits
	.global Func_0200014c
	.thumb_func
Func_0200014c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #8
	str r0, [sp, #4]
	bl Func_0200120c
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	adds r7, r0, #0
	str r3, [sp, #0]
	cmp r7, #0
	bne .L_0200818e
	ldr r3, .L_02008264
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	ldr r0, [r3]
	movs r1, #1
	bl Func_0200118c
	movs r2, #1
	negs r2, r2
	cmp r0, r2
	beq .L_0200818e
	bl Object_GetById
	adds r7, r0, #0
.L_0200818e:
	ldr r1, [sp, #0]
	movs r2, #192
	lsls r2, r2, #4
	adds r2, #188
	adds r3, r1, r2
	movs r2, #0
	str r2, [r3]
	ldr r1, [sp, #4]
	movs r2, #255
	ldrh r3, [r1]
	lsls r2, r2, #8
	adds r2, #255
	cmp r3, r2
	beq .L_02008254
.L_020081aa:
	ldr r3, [sp, #4]
	ldrh r0, [r3]
	bl Object_GetById
	cmp r7, r0
	bne .L_02008242
	movs r1, #34
	adds r1, r1, r7
	ldrb r0, [r1]
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #32]
	lsls r3, r0, #3
	mov r11, r1
	subs r3, r3, r0
	movs r1, #156
	lsls r1, r1, #1
	lsls r3, r3, #3
	adds r3, r3, r1
	ldr r2, [r2, r3]
	ldr r3, .L_02008268
	mov r10, r2
	ldr r2, .L_0200826c
	ldr r1, [r7, #8]
	add r2, r10
	asrs r2, r2, #2
	mov r8, r2
	ldr r2, [r7, #16]
	add r8, r3
	bl Func_020010ec
	mov r1, r11
	ldr r2, [r7, #16]
	mov r9, r0
	ldrb r0, [r1]
	ldr r1, [r7, #8]
	bl Map_GetTerrainHeight
	adds r5, r0, #0
	adds r0, r7, #0
	bl Func_020011d4
	asrs r5, r5, #19
	mov r2, r11
	subs r5, #4
	adds r6, r0, #0
	mov r1, r9
	ldrb r0, [r2]
	adds r2, r5, #0
	bl Func_020011f4
	add r8, r6
	lsls r6, r6, #2
	add r10, r6
	mov r1, r10
	ldrb r2, [r1, #3]
	mov r3, r8
	strb r0, [r3]
	movs r3, #63
	ands r3, r2
	movs r2, #128
	strb r3, [r1, #3]
	lsls r2, r2, #2
	add r10, r2
	mov r3, r10
	ldrb r2, [r3, #3]
	movs r3, #191
	ands r3, r2
	mov r1, r10
	strb r3, [r1, #3]
	ldr r2, [sp, #0]
	movs r1, #192
	lsls r1, r1, #4
	adds r1, #188
	adds r3, r2, r1
	str r7, [r3]
.L_02008242:
	ldr r2, [sp, #4]
	movs r1, #255
	adds r2, #2
	str r2, [sp, #4]
	lsls r1, r1, #8
	ldrh r3, [r2]
	adds r1, #255
	cmp r3, r1
	bne .L_020081aa
.L_02008254:
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008264:
	.4byte gPartyState
.L_02008268:
	.4byte gMapShapeGrid
.L_0200826c:
	.4byte 0xfdff0000
	.section .text.x02008270,"ax",%progbits
	.global Func_02000270
	.thumb_func
Func_02000270:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r2, #192
	lsls r2, r2, #18
	ldr r3, [r2, #108]
	movs r1, #192
	lsls r1, r1, #4
	adds r1, #188
	adds r3, r3, r1
	ldr r6, [r3]
	cmp r6, #0
	beq .L_02008306
	adds r7, r6, #0
	adds r7, #34
	ldrb r0, [r7]
	ldr r2, [r2, #32]
	lsls r3, r0, #3
	subs r3, r3, r0
	movs r1, #156
	lsls r1, r1, #1
	lsls r3, r3, #3
	adds r3, r3, r1
	ldr r2, [r2, r3]
	ldr r3, .L_02008310
	mov r10, r2
	ldr r2, .L_02008314
	ldr r1, [r6, #8]
	add r2, r10
	asrs r2, r2, #2
	mov r8, r2
	ldr r2, [r6, #16]
	add r8, r3
	bl Func_020010ec
	ldr r1, [r6, #8]
	ldr r2, [r6, #16]
	mov r9, r0
	ldrb r0, [r7]
	bl Map_GetTerrainHeight
	adds r5, r0, #0
	adds r0, r6, #0
	bl Func_020011d4
	asrs r5, r5, #19
	adds r5, #4
	adds r6, r0, #0
	mov r1, r9
	adds r2, r5, #0
	ldrb r0, [r7]
	bl Func_020011f4
	add r8, r6
	lsls r6, r6, #2
	add r10, r6
	mov r3, r10
	ldrb r2, [r3, #3]
	mov r1, r8
	movs r3, #192
	orrs r3, r2
	strb r0, [r1]
	movs r2, #128
	mov r1, r10
	strb r3, [r1, #3]
	lsls r2, r2, #2
	add r10, r2
	mov r3, r10
	ldrb r2, [r3, #3]
	movs r3, #64
	orrs r3, r2
	mov r1, r10
	strb r3, [r1, #3]
.L_02008306:
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_02008310:
	.4byte gMapShapeGrid
.L_02008314:
	.4byte 0xfdff0000
	.section .text.x02008318,"ax",%progbits
	.global Func_02000318
	.thumb_func
Func_02000318:
	push {lr}
	ldr r3, .L_02008340
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008344
	cmp r2, r3
	bne .L_02008330
	ldr r0, .L_02008348
	b .L_0200833c
.L_02008330:
	ldr r3, .L_0200834c
	cmp r2, r3
	bne .L_0200833a
	ldr r0, .L_02008350
	b .L_0200833c
.L_0200833a:
	ldr r0, .L_02008354
.L_0200833c:
	pop {pc}
	.2byte 0x0000
.L_02008340:
	.4byte gPartyState
.L_02008344:
	.4byte 0x00000032
.L_02008348:
	.4byte Data_02001400
.L_0200834c:
	.4byte 0x00000033
.L_02008350:
	.4byte Data_02001430
.L_02008354:
	.4byte Data_020013d0
	.section .text.x02008358,"ax",%progbits
	.global Func_02000358
	.thumb_func
Func_02000358:
	push {lr}
	ldr r3, .L_02008374
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008378
	movs r0, #0
	cmp r2, r3
	bne .L_02008370
	ldr r0, .L_0200837c
.L_02008370:
	pop {pc}
	.2byte 0x0000
.L_02008374:
	.4byte gPartyState
.L_02008378:
	.4byte 0x00000030
.L_0200837c:
	.4byte Data_020014a8
	.section .text.x02008388,"ax",%progbits
	.global Func_02000388
	.thumb_func
Func_02000388:
	push {lr}
	ldr r3, .L_020083c4
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_020083c8
	cmp r2, r3
	bne .L_020083a0
	ldr r0, .L_020083cc
	b .L_020083c0
.L_020083a0:
	ldr r3, .L_020083d0
	cmp r2, r3
	bne .L_020083aa
	ldr r0, .L_020083d4
	b .L_020083c0
.L_020083aa:
	ldr r3, .L_020083d8
	cmp r2, r3
	bne .L_020083b4
	ldr r0, .L_020083dc
	b .L_020083c0
.L_020083b4:
	ldr r3, .L_020083e0
	cmp r2, r3
	bne .L_020083be
	ldr r0, .L_020083e4
	b .L_020083c0
.L_020083be:
	ldr r0, .L_020083e8
.L_020083c0:
	pop {pc}
	.2byte 0x0000
.L_020083c4:
	.4byte gPartyState
.L_020083c8:
	.4byte 0x0000002f
.L_020083cc:
	.4byte Data_020015b4
.L_020083d0:
	.4byte 0x00000031
.L_020083d4:
	.4byte Data_0200165c
.L_020083d8:
	.4byte 0x00000033
.L_020083dc:
	.4byte Data_020016bc
.L_020083e0:
	.4byte 0x00000034
.L_020083e4:
	.4byte Data_020016ec
.L_020083e8:
	.4byte Data_0200159c
	.section .text.x020083ec,"ax",%progbits
	.global Func_020003ec
	.thumb_func
Func_020003ec:
	push {lr}
	ldr r3, .L_0200843c
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008440
	cmp r2, r3
	bne .L_02008404
	ldr r0, .L_02008444
	b .L_02008438
.L_02008404:
	ldr r3, .L_02008448
	cmp r2, r3
	bne .L_0200840e
	ldr r0, .L_0200844c
	b .L_02008438
.L_0200840e:
	ldr r3, .L_02008450
	cmp r2, r3
	bne .L_02008418
	ldr r0, .L_02008454
	b .L_02008438
.L_02008418:
	ldr r3, .L_02008458
	cmp r2, r3
	bne .L_02008422
	ldr r0, .L_0200845c
	b .L_02008438
.L_02008422:
	ldr r3, .L_02008460
	cmp r2, r3
	bne .L_0200842c
	ldr r0, .L_02008464
	b .L_02008438
.L_0200842c:
	ldr r3, .L_02008468
	cmp r2, r3
	bne .L_02008436
	ldr r0, .L_0200846c
	b .L_02008438
.L_02008436:
	ldr r0, .L_02008470
.L_02008438:
	pop {pc}
	.2byte 0x0000
.L_0200843c:
	.4byte gPartyState
.L_02008440:
	.4byte 0x0000002f
.L_02008444:
	.4byte Data_02001920
.L_02008448:
	.4byte 0x00000030
.L_0200844c:
	.4byte Data_020019f8
.L_02008450:
	.4byte 0x00000031
.L_02008454:
	.4byte Data_02001a1c
.L_02008458:
	.4byte 0x00000032
.L_0200845c:
	.4byte Data_02001aac
.L_02008460:
	.4byte 0x00000033
.L_02008464:
	.4byte Data_02001b54
.L_02008468:
	.4byte 0x00000034
.L_0200846c:
	.4byte Data_02001bcc
.L_02008470:
	.4byte Data_02001914
	.section .text.x02008474,"ax",%progbits
	.global Func_02000474
	.thumb_func
Func_02000474:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #108]
	bl Func_0200110c
	movs r0, #0
	bl Func_020011ac
	ldr r0, .L_020084a0
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
	movs r3, #173
	lsls r3, r3, #1
	adds r2, r5, r3
	movs r3, #1
	strh r3, [r2]
	bl Func_02001114
	pop {r5, pc}
	.2byte 0x0000
.L_020084a0:
	.4byte MsgFieldDoorWontBudge
	.section .text.x020084a4,"ax",%progbits
	.global Func_020004a4
	.thumb_func
Func_020004a4:
	push {r5, r6, r7, lr}
	adds r6, r0, #0
	adds r7, r6, #0
	adds r7, #85
	movs r3, #3
	strb r3, [r7]
.L_020084b0:
	movs r0, #1
	bl WaitFrames
	ldr r5, [r6, #40]
	cmp r5, #0
	bne .L_020084b0
	movs r0, #188
	bl Func_02001214
	movs r0, #10
	bl WaitFrames
	strb r5, [r7]
	pop {r5, r6, r7, pc}
	.section .text.x020084cc,"ax",%progbits
	.global Func_020004cc
	.thumb_func
Func_020004cc:
	push {r5, r6, lr}
	movs r0, #11
	sub sp, #8
	bl Object_GetById
	adds r5, r0, #0
	ldr r3, [r5, #8]
	asrs r3, r3, #20
	cmp r3, #40
	bne .L_02008518
	ldr r3, [r5, #16]
	asrs r6, r3, #20
	cmp r6, #13
	bne .L_02008518
	bl Func_0200110c
	movs r0, #0
	bl Func_020011ac
	adds r0, r5, #0
	bl Func_020004a4
	movs r3, #39
	str r3, [sp, #0]
	movs r0, #41
	movs r1, #13
	movs r2, #2
	movs r3, #1
	str r6, [sp, #4]
	bl Func_020010dc
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #200
	bl GameFlag_SetBit
	bl Func_02001114
.L_02008518:
	add sp, #8
	pop {r5, r6, pc}
	.section .text.x0200851c,"ax",%progbits
	.global Func_0200051c
	.thumb_func
Func_0200051c:
	push {lr}
	ldr r0, .L_02008528
	bl Func_020011e4
	pop {pc}
	.2byte 0x0000
.L_02008528:
	.4byte Data_020013c0
	.section .text.x0200852c,"ax",%progbits
	.global Func_0200052c
	.thumb_func
Func_0200052c:
	push {r5, lr}
	movs r0, #12
	bl Object_GetById
	adds r5, r0, #0
	bl Func_020011ec
	ldr r3, [r5, #8]
	asrs r3, r3, #20
	cmp r3, #44
	bne .L_02008554
	ldr r3, [r5, #16]
	asrs r3, r3, #20
	cmp r3, #26
	bne .L_02008554
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #202
	bl GameFlag_SetBit
.L_02008554:
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x02008558,"ax",%progbits
	.global Func_02000558
	.thumb_func
Func_02000558:
	push {lr}
	ldr r0, .L_02008564
	bl Func_020011cc
	pop {pc}
	.2byte 0x0000
.L_02008564:
	.4byte Data_020013c4
	.section .text.x02008568,"ax",%progbits
	.global Func_02000568
	.thumb_func
Func_02000568:
	push {r5, r6, r7, lr}
	movs r0, #8
	bl Object_GetById
	adds r7, r0, #0
	movs r0, #9
	bl Object_GetById
	adds r6, r0, #0
	bl Func_0200110c
	movs r0, #0
	bl Func_020011ac
	movs r0, #183
	bl Func_02001214
	movs r5, #0
.L_0200858c:
	ldr r3, [r7, #8]
	ldr r2, .L_0200871c
	movs r0, #1
	adds r3, r3, r2
	str r3, [r7, #8]
	movs r2, #200
	ldr r3, [r6, #8]
	lsls r2, r2, #5
	adds r2, #153
	adds r3, r3, r2
	str r3, [r6, #8]
	adds r5, #1
	bl WaitFrames
	cmp r5, #119
	bls .L_0200858c
	movs r1, #204
	lsls r1, r1, #7
	ldr r0, .L_02008720
	adds r1, #102
	bl Func_02001164
	movs r0, #160
	movs r1, #1
	movs r2, #240
	lsls r2, r2, #16
	movs r3, #1
	lsls r0, r0, #16
	negs r1, r1
	bl Motion_CamBounds
	bl Func_02001174
	movs r0, #204
	movs r1, #192
	lsls r0, r0, #7
	lsls r1, r1, #4
	adds r0, #102
	adds r1, #204
	bl Func_02001164
	movs r0, #160
	movs r1, #1
	movs r2, #186
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #16
	lsls r0, r0, #16
	bl Motion_CamBounds
	movs r0, #168
	bl Func_02001214
	movs r1, #136
	movs r2, #129
	movs r0, #14
	lsls r1, r1, #16
	lsls r2, r2, #17
	bl Func_02001134
	movs r1, #184
	movs r2, #129
	lsls r2, r2, #17
	movs r0, #19
	lsls r1, r1, #16
	bl Func_02001134
	ldr r5, .L_02008724
	movs r0, #14
	adds r1, r5, #0
	bl ObjectMotion_EnableActionAndSetCallback
	adds r1, r5, #0
	movs r0, #19
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #168
	bl Func_02001214
	movs r1, #240
	movs r2, #226
	movs r0, #13
	lsls r1, r1, #15
	lsls r2, r2, #16
	bl Func_02001134
	movs r1, #200
	movs r2, #226
	lsls r2, r2, #16
	movs r0, #18
	lsls r1, r1, #16
	bl Func_02001134
	adds r1, r5, #0
	movs r0, #13
	bl ObjectMotion_EnableActionAndSetCallback
	adds r1, r5, #0
	movs r0, #18
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #168
	bl Func_02001214
	movs r1, #240
	movs r2, #194
	movs r0, #12
	lsls r1, r1, #15
	lsls r2, r2, #16
	bl Func_02001134
	movs r1, #200
	movs r2, #194
	lsls r2, r2, #16
	movs r0, #17
	lsls r1, r1, #16
	bl Func_02001134
	adds r1, r5, #0
	movs r0, #12
	bl ObjectMotion_EnableActionAndSetCallback
	adds r1, r5, #0
	movs r0, #17
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #60
	bl Battle_WaitMode0
	movs r0, #229
	bl Func_02001214
	movs r1, #136
	movs r2, #162
	movs r0, #11
	lsls r1, r1, #16
	lsls r2, r2, #16
	bl Func_02001134
	movs r1, #184
	movs r2, #162
	movs r0, #16
	lsls r1, r1, #16
	lsls r2, r2, #16
	bl Func_02001134
	movs r1, #136
	movs r2, #138
	movs r0, #10
	lsls r1, r1, #16
	lsls r2, r2, #16
	bl Func_02001134
	movs r1, #184
	movs r2, #138
	lsls r2, r2, #16
	movs r0, #15
	lsls r1, r1, #16
	bl Func_02001134
	ldr r5, .L_02008728
	movs r0, #11
	adds r1, r5, #0
	bl ObjectMotion_EnableActionAndSetCallback
	adds r1, r5, #0
	movs r0, #16
	bl ObjectMotion_EnableActionAndSetCallback
	adds r1, r5, #0
	movs r0, #10
	bl ObjectMotion_EnableActionAndSetCallback
	adds r1, r5, #0
	movs r0, #15
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #80
	bl Battle_WaitMode0
	bl Func_02000958
	movs r0, #204
	adds r0, #255
	bl PartyInventory_Remove
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #201
	bl GameFlag_SetBit
	bl Func_02001114
	pop {r5, r6, r7, pc}
.L_0200871c:
	.4byte 0xffffe667
.L_02008720:
	.4byte 0x00033333
.L_02008724:
	.4byte Data_02001264
.L_02008728:
	.4byte Data_020012f4
	.section .text.x0200872c,"ax",%progbits
	.global Func_0200072c
	.thumb_func
Func_0200072c:
	push {lr}
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #203
	sub sp, #8
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008754
	movs r3, #1
	movs r2, #2
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #72
	movs r1, #63
	movs r2, #36
	movs r3, #38
	bl Func_020010cc
	b .L_02008768
.L_02008754:
	movs r3, #1
	movs r2, #2
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #71
	movs r1, #63
	movs r2, #36
	movs r3, #38
	bl Func_020010cc
.L_02008768:
	movs r0, #130
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
	add sp, #8
	pop {pc}
	.2byte 0x0000
	.section .text.x02008778,"ax",%progbits
	.global Func_02000778
	.thumb_func
Func_02000778:
	push {lr}
	sub sp, #8
	movs r3, #1
	movs r2, #2
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #63
	movs r2, #36
	movs r3, #38
	movs r0, #70
	bl Func_020010cc
	movs r0, #130
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_ClearBit
	add sp, #8
	pop {pc}
	.2byte 0x0000
	.section .text.x020087a0,"ax",%progbits
	.global Func_020007a0
	.thumb_func
Func_020007a0:
	push {r5, lr}
	movs r0, #130
	lsls r0, r0, #1
	adds r0, #255
	sub sp, #8
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008856
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #203
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008856
	bl Func_0200110c
	movs r0, #1
	bl Func_020011ac
	movs r0, #161
	bl Func_02001214
	movs r3, #1
	str r3, [sp, #0]
	movs r5, #2
	movs r1, #63
	movs r2, #36
	movs r3, #38
	movs r0, #72
	str r5, [sp, #4]
	bl Func_020010cc
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #188
	bl Func_02001214
	movs r1, #66
	movs r2, #97
	movs r3, #39
	movs r0, #71
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl Func_020010cc
	movs r0, #2
	bl WaitFrames
	movs r1, #66
	movs r2, #97
	movs r3, #39
	movs r0, #73
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl Func_020010cc
	movs r0, #2
	bl WaitFrames
	movs r0, #75
	movs r1, #66
	movs r2, #97
	movs r3, #39
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl Func_020010cc
	movs r3, #33
	movs r2, #39
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #37
	movs r2, #2
	movs r3, #2
	movs r0, #33
	bl Func_020010dc
	movs r0, #1
	bl WaitFrames
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #203
	bl GameFlag_SetBit
	bl Func_02001114
	b .L_0200886c
.L_02008856:
	bl Func_0200110c
	movs r0, #0
	bl Func_020011ac
	ldr r0, .L_02008870
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
	bl Func_02001114
.L_0200886c:
	add sp, #8
	pop {r5, pc}
.L_02008870:
	.4byte MsgFieldEyesOfTruth
	.section .text.x02008874,"ax",%progbits
	.global Func_02000874
	.thumb_func
Func_02000874:
	push {r5, lr}
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #204
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020088cc
	movs r0, #64
	bl Object_GetById
	adds r5, r0, #0
	ldr r3, [r5, #8]
	movs r2, #186
	lsls r2, r2, #16
	cmp r3, r2
	bge .L_020088cc
	bl Func_0200110c
	movs r0, #0
	bl Func_020011ac
	movs r0, #204
	bl Func_02001214
	adds r2, r5, #0
	movs r3, #3
	adds r2, #85
	strb r3, [r2]
	movs r0, #10
	bl WaitFrames
	movs r0, #64
	movs r1, #0
	movs r2, #0
	bl Func_02001134
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #204
	bl GameFlag_SetBit
	bl Func_02001114
.L_020088cc:
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x020088d0,"ax",%progbits
	.global Func_020008d0
	.thumb_func
Func_020008d0:
	push {r5, lr}
	adds r5, r0, #0
	bl Func_02000270
	ldr r3, [r5, #8]
	asrs r3, r3, #20
	cmp r3, #35
	bne .L_020088f2
	ldr r3, [r5, #16]
	asrs r3, r3, #20
	cmp r3, #17
	bne .L_020088f2
	movs r0, #143
	lsls r0, r0, #4
	adds r0, #255
	bl GameFlag_SetBit
.L_020088f2:
	pop {r5, pc}
	.section .text.x020088f4,"ax",%progbits
	.global Func_020008f4
	.thumb_func
Func_020008f4:
	push {r5, lr}
	bl Func_020011fc
	cmp r0, #0
	beq .L_02008922
	ldr r3, .L_02008924
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Func_02001184
	bl Object_GetById
	adds r5, r0, #0
	ldr r0, .L_02008928
	bl Func_0200014c
	bl Func_02001204
	adds r0, r5, #0
	bl Func_020008d0
.L_02008922:
	pop {r5, pc}
.L_02008924:
	.4byte gPartyState
.L_02008928:
	.4byte Data_020013ca
	.section .text.x0200892c,"ax",%progbits
	.global Func_0200092c
	.thumb_func
Func_0200092c:
	push {lr}
	ldr r0, .L_02008938
	bl Func_0200014c
	pop {pc}
	.2byte 0x0000
.L_02008938:
	.4byte Data_020013ca
	.section .text.x0200893c,"ax",%progbits
	.global Func_0200093c
	.thumb_func
Func_0200093c:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #192
	lsls r2, r2, #4
	adds r2, #188
	adds r3, r3, r2
	ldr r0, [r3]
	cmp r0, #0
	beq .L_02008956
	bl Func_020008d0
.L_02008956:
	pop {pc}
	.section .text.x02008958,"ax",%progbits
	.global Func_02000958
	.thumb_func
Func_02000958:
	push {r5, r6, r7, lr}
	movs r1, #136
	movs r2, #129
	movs r0, #14
	lsls r1, r1, #16
	lsls r2, r2, #17
	sub sp, #8
	bl Func_02001134
	movs r1, #184
	movs r2, #129
	movs r0, #19
	lsls r1, r1, #16
	lsls r2, r2, #17
	bl Func_02001134
	movs r1, #240
	movs r2, #226
	movs r0, #13
	lsls r1, r1, #15
	lsls r2, r2, #16
	bl Func_02001134
	movs r1, #200
	movs r2, #226
	movs r0, #18
	lsls r1, r1, #16
	lsls r2, r2, #16
	bl Func_02001134
	movs r1, #240
	movs r2, #194
	movs r0, #12
	lsls r1, r1, #15
	lsls r2, r2, #16
	bl Func_02001134
	movs r1, #200
	movs r2, #194
	movs r0, #17
	lsls r1, r1, #16
	lsls r2, r2, #16
	bl Func_02001134
	movs r1, #136
	movs r2, #162
	movs r0, #11
	lsls r1, r1, #16
	lsls r2, r2, #16
	bl Func_02001134
	movs r1, #184
	movs r2, #162
	movs r0, #16
	lsls r1, r1, #16
	lsls r2, r2, #16
	bl Func_02001134
	movs r1, #136
	movs r2, #138
	movs r0, #10
	lsls r1, r1, #16
	lsls r2, r2, #16
	bl Func_02001134
	movs r1, #184
	movs r2, #138
	movs r0, #15
	lsls r1, r1, #16
	lsls r2, r2, #16
	bl Func_02001134
	movs r7, #10
	movs r6, #20
	movs r5, #0
.L_020089ee:
	adds r0, r6, #0
	movs r1, #0
	bl Object_SetModeById
	adds r5, #1
	adds r0, r6, #0
	adds r1, r7, #0
	bl Func_0200113c
	adds r7, #1
	adds r6, #1
	cmp r5, #9
	bls .L_020089ee
	movs r3, #9
	movs r2, #22
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #0
	movs r2, #2
	movs r3, #2
	movs r0, #0
	bl Func_020010dc
	movs r0, #1
	bl WaitFrames
	add sp, #8
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x02008a28,"ax",%progbits
	.global Func_02000a28
	.thumb_func
Func_02000a28:
	push {r5, r6, r7, lr}
	movs r0, #8
	bl Object_GetById
	adds r5, r0, #0
	movs r0, #64
	bl Object_GetById
	ldr r3, [r5, #80]
	adds r6, r0, #0
	ldr r7, [r6, #80]
	ldrh r3, [r3, #18]
	movs r0, #129
	strh r3, [r7, #18]
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008a74
	ldrh r3, [r7, #18]
	cmp r3, #0
	bne .L_02008a7a
	movs r0, #140
	lsls r0, r0, #1
	adds r0, #255
	bl Func_02001214
	ldr r3, [r6, #8]
	ldr r2, .L_02008a84
	movs r0, #129
	adds r3, r3, r2
	str r3, [r6, #8]
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r6, #40]
	lsls r0, r0, #2
	bl GameFlag_ClearBit
.L_02008a74:
	ldrh r3, [r7, #18]
	cmp r3, #0
	beq .L_02008a82
.L_02008a7a:
	movs r0, #129
	lsls r0, r0, #2
	bl GameFlag_SetBit
.L_02008a82:
	pop {r5, r6, r7, pc}
.L_02008a84:
	.4byte 0xfffe0000
	.section .text.x02008a88,"ax",%progbits
	.global Func_02000a88
	.thumb_func
Func_02000a88:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r7, [r3, #108]
	ldr r3, .L_02008b30
	movs r2, #133
	lsls r2, r2, #2
	adds r6, r3, r2
	ldr r0, [r6]
	bl Object_GetById
	adds r5, r0, #0
	bl Func_0200110c
	movs r0, #0
	bl Func_020011ac
	movs r0, #1
	movs r1, #1
	movs r2, #1
	movs r3, #0
	negs r2, r2
	negs r0, r0
	negs r1, r1
	bl Motion_CamBounds
	ldr r0, [r6]
	movs r1, #2
	bl Object_SetModeById
	movs r1, #128
	movs r2, #128
	ldr r0, [r6]
	lsls r2, r2, #7
	lsls r1, r1, #8
	bl ObjectMotion_SetSpeedParameters
	ldr r0, [r5, #8]
	asrs r2, r0, #16
	adds r3, r2, #0
	cmp r2, #0
	bge .L_02008ade
	adds r3, #15
.L_02008ade:
	asrs r3, r3, #4
	lsls r3, r3, #4
	subs r3, r2, r3
	movs r1, #8
	subs r1, r1, r3
	lsls r1, r1, #16
	ldr r2, [r5, #12]
	adds r1, r1, r0
	ldr r3, [r5, #16]
	adds r0, r5, #0
	bl Func_020010bc
	adds r0, r5, #0
	bl Func_020010c4
	movs r3, #192
	lsls r3, r3, #8
	strh r3, [r5, #6]
	ldr r1, .L_02008b34
	ldr r0, [r6]
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #12
	bl WaitFrames
	movs r0, #123
	bl Func_02001214
	bl Event_ClearStatus1c6
	bl Event_WaitValue1c8Frames
	movs r2, #170
	lsls r2, r2, #1
	adds r3, r7, r2
	movs r2, #0
	ldrsh r0, [r3, r2]
	bl Func_0200117c
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008b30:
	.4byte gPartyState
.L_02008b34:
	.4byte Data_0200121c
	.section .text.x02008b38,"ax",%progbits
	.global Func_02000b38
	.thumb_func
Func_02000b38:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	mov r10, r3
	movs r3, #192
	lsls r3, r3, #18
	sub sp, #8
	ldr r3, [r3, #108]
	ldr r6, [sp, #36]
	adds r5, r0, #0
	adds r7, r1, #0
	mov r8, r2
	mov r9, r3
	bl Func_0200110c
	movs r0, #0
	bl Func_020011ac
	cmp r6, #2
	bne .L_02008b6c
	movs r0, #188
	bl Func_02001214
	b .L_02008b72
.L_02008b6c:
	movs r0, #158
	bl Func_02001214
.L_02008b72:
	ldr r3, [sp, #40]
	adds r0, r5, #0
	str r3, [sp, #4]
	adds r1, r7, #0
	mov r2, r8
	mov r3, r10
	str r6, [sp, #0]
	bl Func_020010cc
	movs r0, #20
	bl Battle_WaitMode0
	ldr r3, .L_02008c1c
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r3, r2
	movs r1, #128
	movs r2, #128
	ldr r0, [r5]
	lsls r1, r1, #8
	lsls r2, r2, #7
	bl ObjectMotion_SetSpeedParameters
	cmp r6, #1
	bne .L_02008bdc
	ldr r0, [r5]
	bl Object_GetById
	adds r5, r0, #0
	ldr r0, [r5, #8]
	asrs r2, r0, #16
	adds r3, r2, #0
	cmp r2, #0
	bge .L_02008bb8
	adds r3, #15
.L_02008bb8:
	asrs r3, r3, #4
	lsls r3, r3, #4
	subs r3, r2, r3
	movs r1, #8
	subs r1, r1, r3
	lsls r1, r1, #16
	adds r1, r1, r0
	ldr r3, [r5, #16]
	ldr r2, [r5, #12]
	adds r0, r5, #0
	bl Func_020010bc
	adds r0, r5, #0
	bl Func_020010c4
	movs r3, #192
	lsls r3, r3, #8
	strh r3, [r5, #6]
.L_02008bdc:
	ldr r3, .L_02008c1c
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	ldr r1, .L_02008c20
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #12
	bl WaitFrames
	movs r0, #123
	bl Func_02001214
	bl Event_ClearStatus1c6
	bl Event_WaitValue1c8Frames
	movs r3, #170
	lsls r3, r3, #1
	add r3, r9
	movs r2, #0
	ldrsh r0, [r3, r2]
	bl Func_0200117c
	add sp, #8
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008c1c:
	.4byte gPartyState
.L_02008c20:
	.4byte Data_0200121c
	.section .text.x02008c24,"ax",%progbits
	.global Func_02000c24
	.thumb_func
Func_02000c24:
	push {lr}
	sub sp, #8
	movs r3, #2
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #78
	movs r1, #29
	movs r2, #78
	movs r3, #31
	bl Func_02000b38
	add sp, #8
	pop {pc}
	.2byte 0x0000
	.section .text.x02008c40,"ax",%progbits
	.global Func_02000c40
	.thumb_func
Func_02000c40:
	push {lr}
	sub sp, #8
	movs r3, #1
	str r3, [sp, #0]
	movs r3, #2
	str r3, [sp, #4]
	movs r0, #109
	movs r1, #1
	movs r2, #109
	movs r3, #3
	bl Func_02000b38
	add sp, #8
	pop {pc}
	.section .text.x02008c5c,"ax",%progbits
	.global Func_02000c5c
	.thumb_func
Func_02000c5c:
	push {lr}
	sub sp, #8
	movs r3, #1
	str r3, [sp, #0]
	movs r3, #2
	str r3, [sp, #4]
	movs r0, #118
	movs r1, #1
	movs r2, #118
	movs r3, #3
	bl Func_02000b38
	add sp, #8
	pop {pc}
	.section .text.x02008c78,"ax",%progbits
	.global Func_02000c78
	.thumb_func
Func_02000c78:
	push {lr}
	sub sp, #8
	movs r3, #1
	str r3, [sp, #0]
	movs r3, #2
	str r3, [sp, #4]
	movs r0, #92
	movs r1, #29
	movs r2, #92
	movs r3, #31
	bl Func_02000b38
	add sp, #8
	pop {pc}
	.section .text.x02008c94,"ax",%progbits
	.global Func_02000c94
	.thumb_func
Func_02000c94:
	push {r5, lr}
	bl Func_0200110c
	movs r0, #0
	bl Func_020011ac
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r2, r2
	negs r1, r1
	negs r0, r0
	movs r3, #0
	bl Motion_CamBounds
	ldr r5, .L_02008cf0
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	ldr r0, [r5]
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	ldr r0, [r5]
	bl Object_GetById
	movs r1, #15
	bl Object_SetPartAttribute
	movs r0, #1
	bl WaitFrames
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #3
	bl Func_0200117c
	pop {r5, pc}
	.2byte 0x0000
.L_02008cf0:
	.4byte gPartyState
	.section .text.x02008cf4,"ax",%progbits
	.global Func_02000cf4
	.thumb_func
Func_02000cf4:
	push {r5, lr}
	bl Func_0200110c
	movs r0, #0
	bl Func_020011ac
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r2, r2
	negs r1, r1
	negs r0, r0
	movs r3, #0
	bl Motion_CamBounds
	ldr r5, .L_02008d50
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	ldr r0, [r5]
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	ldr r0, [r5]
	bl Object_GetById
	movs r1, #15
	bl Object_SetPartAttribute
	movs r0, #1
	bl WaitFrames
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #8
	bl Func_0200117c
	pop {r5, pc}
	.2byte 0x0000
.L_02008d50:
	.4byte gPartyState
	.section .text.x02008d54,"ax",%progbits
	.global Func_02000d54
	.thumb_func
Func_02000d54:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r1, #214
	lsls r1, r1, #1
	movs r2, #129
	adds r3, r3, r1
	lsls r2, r2, #2
	str r2, [r3]
	ldr r3, .L_02008db0
	subs r2, #36
	adds r3, r3, r2
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008db4
	cmp r2, r3
	bne .L_02008d7e
	bl Func_02000dcc
	b .L_02008dac
.L_02008d7e:
	ldr r3, .L_02008db8
	cmp r2, r3
	bne .L_02008d8a
	bl Func_02000f2c
	b .L_02008dac
.L_02008d8a:
	ldr r3, .L_02008dbc
	cmp r2, r3
	bne .L_02008d96
	bl Func_02000fbc
	b .L_02008dac
.L_02008d96:
	ldr r3, .L_02008dc0
	cmp r2, r3
	bne .L_02008da2
	bl Func_02000fd8
	b .L_02008dac
.L_02008da2:
	ldr r3, .L_02008dc4
	cmp r2, r3
	bne .L_02008dac
	bl Func_0200102c
.L_02008dac:
	movs r0, #0
	pop {pc}
.L_02008db0:
	.4byte gPartyState
.L_02008db4:
	.4byte 0x0000002f
.L_02008db8:
	.4byte 0x00000031
.L_02008dbc:
	.4byte 0x00000032
.L_02008dc0:
	.4byte 0x00000033
.L_02008dc4:
	.4byte 0x00000034
	.section .text.x02008dcc,"ax",%progbits
	.global Func_02000dcc
	.thumb_func
Func_02000dcc:
	push {lr}
	movs r0, #130
	lsls r0, r0, #1
	adds r0, #255
	sub sp, #8
	bl GameFlag_ClearBit
	movs r0, #64
	movs r1, #3
	bl ObjectMotion_SetActionVariant
	bl Func_020011b4
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #1
	adds r1, #255
	movs r2, #9
	movs r3, #10
	bl Func_020011bc
	movs r0, #11
	bl Func_0200115c
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #200
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008e32
	movs r3, #39
	movs r2, #13
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #13
	movs r2, #2
	movs r3, #1
	movs r0, #41
	bl Func_020010dc
	movs r0, #11
	bl Object_GetById
	movs r1, #162
	movs r3, #216
	lsls r1, r1, #18
	movs r2, #0
	lsls r3, r3, #16
	bl Object_SetPositionAndResetMotion
.L_02008e32:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #202
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008e4e
	movs r1, #178
	movs r2, #212
	movs r0, #12
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl Func_02001134
.L_02008e4e:
	ldr r0, .L_02008f1c
	bl Func_020011dc
	movs r0, #143
	lsls r0, r0, #4
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008e70
	movs r1, #142
	movs r2, #140
	movs r0, #8
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl Func_02001134
.L_02008e70:
	movs r0, #8
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	strb r3, [r0]
	ldr r0, .L_02008f20
	bl Func_02000038
	ldr r0, .L_02008f24
	bl Func_020011c4
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #203
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008ec6
	movs r3, #2
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #75
	movs r1, #66
	movs r2, #97
	movs r3, #39
	bl Func_020010cc
	movs r3, #33
	movs r2, #39
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #33
	movs r1, #37
	movs r2, #2
	movs r3, #2
	bl Func_020010dc
	movs r0, #1
	bl WaitFrames
.L_02008ec6:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #171
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008f18
	movs r3, #5
	movs r2, #3
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #82
	movs r1, #74
	movs r2, #116
	movs r3, #16
	bl Func_020010cc
	movs r3, #52
	movs r2, #18
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #1
	movs r2, #5
	movs r0, #52
	movs r1, #11
	bl Func_020010dc
	ldr r3, .L_02008f28
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #3
	bne .L_02008f12
	bl Func_02000c94
	b .L_02008f18
.L_02008f12:
	movs r0, #10
	bl WaitFrames
.L_02008f18:
	add sp, #8
	pop {pc}
.L_02008f1c:
	.4byte Data_020013c0
.L_02008f20:
	.4byte Data_020013ca
.L_02008f24:
	.4byte Data_020013c4
.L_02008f28:
	.4byte gPartyState
	.section .text.x02008f2c,"ax",%progbits
	.global Func_02000f2c
	.thumb_func
Func_02000f2c:
	push {lr}
	sub sp, #8
	bl Func_020011b4
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #2
	movs r2, #8
	movs r3, #9
	bl Func_020011bc
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #171
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008fb2
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_02001134
	movs r3, #5
	movs r2, #3
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #68
	movs r1, #70
	movs r2, #113
	movs r3, #11
	bl Func_020010cc
	movs r3, #2
	movs r2, #1
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #50
	movs r1, #74
	movs r2, #50
	movs r3, #75
	bl Func_020010cc
	movs r3, #49
	movs r2, #13
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #1
	movs r2, #5
	movs r0, #49
	movs r1, #9
	bl Func_020010dc
	ldr r3, .L_02008fb8
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #8
	bne .L_02008fac
	bl Func_02000cf4
	b .L_02008fb2
.L_02008fac:
	movs r0, #1
	bl WaitFrames
.L_02008fb2:
	add sp, #8
	pop {pc}
	.2byte 0x0000
.L_02008fb8:
	.4byte gPartyState
	.section .text.x02008fbc,"ax",%progbits
	.global Func_02000fbc
	.thumb_func
Func_02000fbc:
	push {lr}
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #204
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008fd6
	movs r0, #65
	movs r1, #0
	movs r2, #0
	bl Func_02001134
.L_02008fd6:
	pop {pc}
	.section .text.x02008fd8,"ax",%progbits
	.global Func_02000fd8
	.thumb_func
Func_02000fd8:
	push {lr}
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #204
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008ff4
	movs r0, #64
	movs r1, #0
	movs r2, #0
	bl Func_02001134
	b .L_02009024
.L_02008ff4:
	movs r0, #64
	bl Object_GetById
	adds r2, r0, #0
	adds r1, r2, #0
	adds r1, #89
	movs r3, #8
	strb r3, [r1]
	subs r1, #4
	movs r3, #2
	strb r3, [r1]
	movs r3, #128
	lsls r3, r3, #14
	str r3, [r2, #12]
	str r3, [r2, #20]
	ldr r3, .L_02009028
	movs r0, #8
	str r3, [r2, #108]
	bl Object_GetById
	movs r3, #204
	lsls r3, r3, #8
	adds r3, #204
	str r3, [r0, #28]
.L_02009024:
	pop {pc}
	.2byte 0x0000
.L_02009028:
	.4byte Func_02000a28
	.section .text.x0200902c,"ax",%progbits
	.global Func_0200102c
	.thumb_func
Func_0200102c:
	push {r5, r6, lr}
	movs r0, #8
	bl Object_GetById
	adds r5, r0, #0
	movs r0, #9
	bl Object_GetById
	movs r3, #160
	lsls r3, r3, #9
	str r3, [r5, #24]
	ldr r3, .L_0200908c
	adds r6, r0, #0
	adds r2, r5, #0
	str r3, [r6, #24]
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	movs r0, #128
	adds r2, r6, #0
	adds r2, #85
	lsls r0, r0, #4
	strb r3, [r2]
	adds r0, #201
	str r3, [r5, #12]
	str r3, [r6, #12]
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200908a
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02009086
	ldr r3, [r5, #8]
	ldr r2, .L_02009090
	adds r3, r3, r2
	str r3, [r5, #8]
	movs r2, #192
	ldr r3, [r6, #8]
	lsls r2, r2, #12
	adds r3, r3, r2
	str r3, [r6, #8]
.L_02009086:
	bl Func_02000958
.L_0200908a:
	pop {r5, r6, pc}
.L_0200908c:
	.4byte 0xfffec000
.L_02009090:
	.4byte 0xfff40000
	.section .rodata.x0200921c,"a",%progbits
	.global Data_0200121c
Data_0200121c:
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000000
	.4byte 0x00000017
	.4byte 0x00000007
	.4byte 0xffff8000
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xfffffc00
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffffc00
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000018
	.4byte 0x00000000
	.4byte 0x00000011
	.global Data_02001264
Data_02001264:
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x00001000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x80010000
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00004000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000004
	.4byte 0xc0010000
	.4byte 0x80020000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000004
	.4byte 0xc0020000
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00010000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x00010000
	.4byte 0x00000011
	.global Data_020012f4
Data_020012f4:
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x00001000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x80010000
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00008000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0x00004000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000004
	.4byte 0xc0010000
	.4byte 0x80020000
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xffffe000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0x00006000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000004
	.4byte 0xc0020000
	.4byte 0x80030000
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xffffe000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xffffc000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000004
	.4byte 0xc0030000
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00010000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x00010000
	.4byte 0x00000011
	.global Data_020013c0
Data_020013c0:
	.4byte 0xffff000c
	.global Data_020013c4
Data_020013c4:
	.4byte 0x0202000d
	.2byte 0xffff
	.global Data_020013ca
Data_020013ca:
	.2byte 0x0008
	.4byte 0x0000ffff
	.global Data_020013d0
Data_020013d0:
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
	.global Data_02001400
Data_02001400:
	.4byte 0xffff0003
	.4byte 0x00000200
	.4byte 0xc00000e0
	.4byte 0x01880000
	.4byte 0x02780028
	.4byte 0x00000118
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02001430
Data_02001430:
	.4byte 0xffff0005
	.4byte 0x000002d8
	.4byte 0x40000058
	.4byte 0x02a80000
	.4byte 0x03980020
	.4byte 0x000000c0
	.4byte 0xffff0006
	.4byte 0x00000368
	.4byte 0x40000058
	.4byte 0x02a80000
	.4byte 0x03980020
	.4byte 0x000000c0
	.4byte 0xffff0007
	.4byte 0x00000368
	.4byte 0x00000078
	.4byte 0x02a80000
	.4byte 0x03980020
	.4byte 0x000000c0
	.4byte 0xffff0008
	.4byte 0x00000318
	.4byte 0xc0000098
	.4byte 0x02a80000
	.4byte 0x03980020
	.4byte 0x000000c0
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020014a8
Data_020014a8:
	.4byte 0x00300070
	.4byte 0x00800090
	.4byte 0x00a00040
	.4byte 0x0001ffff
	.4byte 0x00300150
	.4byte 0x01600090
	.4byte 0x00a00040
	.4byte 0x0003ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gSceneExits
gSceneExits:
	.4byte 0x0000002f
	.4byte 0x00108033
	.4byte 0x00207032
	.4byte 0x00301031
	.4byte 0x0040d032
	.4byte 0x00506031
	.4byte 0x00000030
	.4byte 0x0010f02c
	.4byte 0x00207031
	.4byte 0x0031002c
	.4byte 0x00404031
	.4byte 0x00000031
	.4byte 0x0010302f
	.4byte 0x00203031
	.4byte 0x00302031
	.4byte 0x00404030
	.4byte 0x00508031
	.4byte 0x0060502f
	.4byte 0x00702030
	.4byte 0x00805031
	.4byte 0x00000032
	.4byte 0x00101033
	.4byte 0x0020b032
	.4byte 0x00309032
	.4byte 0x00402033
	.4byte 0x00504034
	.4byte 0x00601034
	.4byte 0x0070202f
	.4byte 0x0080c032
	.4byte 0x00903032
	.4byte 0x00a07033
	.4byte 0x00b02032
	.4byte 0x00c08032
	.4byte 0x00d0402f
	.4byte 0x00000033
	.4byte 0x00101032
	.4byte 0x00204032
	.4byte 0x00305033
	.4byte 0x00406033
	.4byte 0x00503033
	.4byte 0x00604033
	.4byte 0x0070a032
	.4byte 0x0080102f
	.4byte 0x00000034
	.4byte 0x00106032
	.4byte 0x00203034
	.4byte 0x00302034
	.4byte 0x00405032
	.4byte 0x000001ff
	.global Data_0200159c
Data_0200159c:
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020015b4
Data_020015b4:
	.4byte 0xffff00fa
	.4byte 0x00000001
	.4byte 0x02280000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x02480000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x01024000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x02780000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00024000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x02b80000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00024000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_0200165c
Data_0200165c:
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x03080000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x02b80000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x01024000
	.4byte 0xffff0047
	.4byte 0x00000001
	.4byte 0x031a0000
	.4byte 0x00000000
	.4byte 0x00ec0000
	.4byte 0x00018000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020016bc
Data_020016bc:
	.4byte 0xffff02a7
	.4byte 0x00000001
	.4byte 0x00ca0000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020016ec
Data_020016ec:
	.4byte 0xffff014a
	.4byte 0x00000007
	.4byte 0x00960000
	.4byte 0x00000000
	.4byte 0x016e0000
	.4byte 0x00024000
	.4byte 0xffff014a
	.4byte 0x00000007
	.4byte 0x00aa0000
	.4byte 0x00000000
	.4byte 0x016e0000
	.4byte 0x00024000
	.4byte 0xffff0132
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0132
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0132
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0132
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0132
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0132
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0132
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0132
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0132
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0132
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff019a
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff019a
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff019a
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff019a
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff019a
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff019a
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff019a
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff019a
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff019a
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff019a
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02001914
Data_02001914:
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02001920
Data_02001920:
	.4byte 0x0000c602
	.4byte 0xffff0001
	.4byte Func_02000a88
	.4byte 0x0000c602
	.4byte 0xffff0002
	.4byte Func_02000a88
	.4byte 0x0000c602
	.4byte 0xffff0003
	.4byte Func_02000a88
	.4byte 0x0000c602
	.4byte 0xffff0004
	.4byte Func_02000a88
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00008515
	.4byte 0x02010009
	.4byte 0x00000000
	.4byte 0x00000009
	.4byte 0x08c8000a
	.4byte Func_020004cc
	.4byte 0x00008c15
	.4byte 0x08c8000b
	.4byte Func_020004cc
	.4byte 0x10008c15
	.4byte 0x08ca000c
	.4byte Func_0200051c
	.4byte 0x00008c15
	.4byte 0x08ca000c
	.4byte Func_0200052c
	.4byte 0x00000602
	.4byte 0x09ef000b
	.4byte Func_020008f4
	.4byte 0x10008c15
	.4byte 0x09ef0008
	.4byte Func_0200092c
	.4byte 0x00008c15
	.4byte 0x09ef0008
	.4byte Func_0200093c
	.4byte 0x00001815
	.4byte 0x0202000d
	.4byte Func_02000558
	.4byte 0x50009085
	.4byte 0xffff0000
	.4byte Func_0200072c
	.4byte 0x40009085
	.4byte 0xffff0000
	.4byte Func_02000778
	.4byte 0x0000c403
	.4byte 0xffff000a
	.4byte Func_020007a0
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020019f8
Data_020019f8:
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02001a1c
Data_02001a1c:
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
	.4byte 0x00008515
	.4byte 0x02000008
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte MsgFieldCaveFoundByChampa
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte MsgFieldWhoDugCave
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02001aac
Data_02001aac:
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
	.4byte 0x0000c602
	.4byte 0xffff0009
	.4byte Func_02000c24
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
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02001b54
Data_02001b54:
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
	.4byte 0x0000c602
	.4byte 0xffff0005
	.4byte Func_02000c40
	.4byte 0x0000c602
	.4byte 0xffff0006
	.4byte Func_02000c5c
	.4byte 0x00000001
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00008715
	.4byte 0xffff0008
	.4byte Func_02000874
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02001bcc
Data_02001bcc:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x0000c602
	.4byte 0xffff0003
	.4byte Func_02000c78
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x0001cb04
	.4byte 0xffff000a
	.4byte Func_02000568
	.4byte 0x00000003
	.4byte 0x08c9000a
	.4byte Func_02000474
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
