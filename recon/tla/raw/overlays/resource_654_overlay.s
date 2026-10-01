.syntax unified
	.thumb
	.section .text.x0200807e,"ax",%progbits
	.2byte 0x0000
	.section .text.x02008080,"ax",%progbits
	.global Func_02000080
	.thumb_func
Func_02000080:
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
	.section .text.x020080b8,"ax",%progbits
	.global Func_020000b8
	.thumb_func
Func_020000b8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	mov r11, r3
	ldr r3, .L_02008270
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
	beq .L_02008100
	cmp r7, #0
	beq .L_02008100
	movs r2, #24
	ldrsh r0, [r7, r2]
	adds r1, r5, #0
	adds r2, r6, #0
	b .L_02008108
.L_02008100:
	movs r0, #30
	adds r2, r6, #0
	adds r0, #255
	adds r1, r5, #0
.L_02008108:
	mov r3, r10
	bl Func_02003978
	adds r6, r0, #0
	cmp r6, #0
	bne .L_02008116
	b .L_02008262
.L_02008116:
	ldr r3, [r6, #80]
	mov r1, r8
	movs r5, #15
	adds r1, #1
	ands r1, r5
	adds r0, r6, #0
	str r3, [sp, #0]
	bl Func_02003968
	ldr r2, .L_02008274
	mov r3, r8
	ands r3, r5
	lsls r3, r3, #2
	ldr r1, [r2, r3]
	adds r0, r6, #0
	mov r10, r3
	bl Func_02003970
	adds r3, r6, #0
	movs r5, #0
	adds r3, #85
	strb r5, [r3]
	adds r0, r6, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	ldr r3, .L_02008278
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
	ldr r3, .L_0200827c
	mov r1, r8
	ands r3, r1
	cmp r3, #0
	beq .L_02008262
	cmp r7, #0
	beq .L_02008262
	movs r3, #128
	lsls r3, r3, #9
	ands r3, r1
	cmp r3, #0
	beq .L_02008198
	ldr r1, [r7, #4]
	adds r0, r6, #0
	bl Object_SetPartAttribute
.L_02008198:
	movs r3, #128
	lsls r3, r3, #10
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_020081b8
	adds r1, r6, #0
	adds r1, #35
	ldrb r3, [r1]
	movs r2, #254
	ands r2, r3
	strb r2, [r1]
	ldr r1, [r7]
	adds r0, r6, #0
	bl Object_SetSpritePriority
.L_020081b8:
	movs r2, #128
	lsls r2, r2, #12
	mov r3, r8
	ands r2, r3
	cmp r2, #0
	beq .L_020081cc
	ldr r3, [r7, #8]
	str r3, [r6, #24]
	ldr r3, [r7, #12]
	str r3, [r6, #28]
.L_020081cc:
	movs r3, #128
	lsls r3, r3, #11
	mov r1, r8
	ands r3, r1
	cmp r3, #0
	beq .L_02008212
	ldr r3, .L_02008274
	mov r1, r10
	ldr r5, [r3, r1]
	ldr r3, [r7, #16]
	ldr r1, [r5, #12]
	cmp r2, #0
	beq .L_020081fa
	ldr r0, [r6, #24]
	subs r0, r3, r0
	bl __divsi3
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, [r6, #28]
	ldr r1, [r5, #12]
	subs r0, r0, r3
	b .L_0200820c
.L_020081fa:
	ldr r2, .L_0200827c
	adds r0, r3, r2
	bl __divsi3
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, .L_0200827c
	ldr r1, [r5, #12]
	adds r0, r0, r3
.L_0200820c:
	bl __divsi3
	str r0, [r6, #52]
.L_02008212:
	movs r3, #128
	lsls r3, r3, #14
	mov r1, r8
	ands r3, r1
	cmp r3, #0
	beq .L_0200822e
	adds r0, r6, #0
	movs r1, #1
	bl Func_02003968
	ldr r1, [r7, #28]
	adds r0, r6, #0
	bl Func_02003970
.L_0200822e:
	movs r3, #128
	lsls r3, r3, #15
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_02008240
	ldrh r3, [r7, #32]
	ldr r1, [sp, #0]
	strh r3, [r1, #18]
.L_02008240:
	movs r3, #128
	lsls r3, r3, #16
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_02008252
	ldrh r3, [r7, #34]
	mov r1, r9
	strh r3, [r1]
.L_02008252:
	movs r3, #128
	lsls r3, r3, #17
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_02008262
	ldr r3, [r7, #36]
	str r3, [r6, #108]
.L_02008262:
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_02008270:
	.4byte gPartyState
.L_02008274:
	.4byte Data_02003c04
.L_02008278:
	.4byte Func_02000080
.L_0200827c:
	.4byte 0xffff0000
	.section .text.x02008280,"ax",%progbits
	.global Func_02000280
	.thumb_func
Func_02000280:
	push {r5, r6, lr}
	ldr r3, .L_020082e8
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	adds r6, r0, #0
	movs r0, #14
	bl Object_GetById
	adds r5, r0, #0
	adds r5, #98
	ldrb r3, [r5]
	movs r2, #128
	adds r3, #252
	lsls r3, r3, #24
	lsls r2, r2, #17
	cmp r3, r2
	bhi .L_020082e4
	ldr r3, [r6, #12]
	movs r2, #192
	lsls r2, r2, #12
	cmp r3, r2
	ble .L_020082e4
	movs r0, #208
	lsls r0, r0, #2
	bl GameFlag_SetBit
	ldrb r0, [r5]
	cmp r0, #4
	bne .L_020082ce
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #65
	bl GameFlag_SetBit
	b .L_020082dc
.L_020082ce:
	cmp r0, #5
	bne .L_020082dc
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #66
	bl GameFlag_SetBit
.L_020082dc:
	movs r0, #14
	movs r1, #67
	bl Func_02003ae0
.L_020082e4:
	pop {r5, r6, pc}
	.2byte 0x0000
.L_020082e8:
	.4byte gPartyState
	.section .text.x020082f4,"ax",%progbits
	.global Func_020002f4
	.thumb_func
Func_020002f4:
	push {lr}
	ldr r3, .L_02008324
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008328
	cmp r2, r3
	bne .L_0200830c
	ldr r0, .L_0200832c
	b .L_02008320
.L_0200830c:
	ldr r3, .L_02008330
	cmp r2, r3
	bne .L_02008316
	ldr r0, .L_02008334
	b .L_02008320
.L_02008316:
	ldr r3, .L_02008338
	movs r0, #0
	cmp r2, r3
	bne .L_02008320
	ldr r0, .L_0200833c
.L_02008320:
	pop {pc}
	.2byte 0x0000
.L_02008324:
	.4byte gPartyState
.L_02008328:
	.4byte 0x0000001f
.L_0200832c:
	.4byte Data_02003df4
.L_02008330:
	.4byte 0x00000020
.L_02008334:
	.4byte Data_02003e14
.L_02008338:
	.4byte 0x00000021
.L_0200833c:
	.4byte Data_02003e44
	.section .text.x02008348,"ax",%progbits
	.global Func_02000348
	.thumb_func
Func_02000348:
	push {lr}
	ldr r3, .L_020083ac
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_020083b0
	cmp r2, r3
	bne .L_02008360
	ldr r0, .L_020083b4
	b .L_020083a8
.L_02008360:
	ldr r3, .L_020083b8
	cmp r2, r3
	bne .L_0200836a
	ldr r0, .L_020083bc
	b .L_020083a8
.L_0200836a:
	ldr r3, .L_020083c0
	cmp r2, r3
	bne .L_02008374
	ldr r0, .L_020083c4
	b .L_020083a8
.L_02008374:
	ldr r3, .L_020083c8
	cmp r2, r3
	bne .L_0200837e
	ldr r0, .L_020083cc
	b .L_020083a8
.L_0200837e:
	ldr r3, .L_020083d0
	cmp r2, r3
	bne .L_02008388
	ldr r0, .L_020083d4
	b .L_020083a8
.L_02008388:
	ldr r3, .L_020083d8
	cmp r2, r3
	bne .L_02008392
	ldr r0, .L_020083dc
	b .L_020083a8
.L_02008392:
	ldr r3, .L_020083e0
	cmp r2, r3
	bne .L_0200839c
	ldr r0, .L_020083e4
	b .L_020083a8
.L_0200839c:
	ldr r3, .L_020083e8
	cmp r2, r3
	bne .L_020083a6
	ldr r0, .L_020083ec
	b .L_020083a8
.L_020083a6:
	ldr r0, .L_020083f0
.L_020083a8:
	pop {pc}
	.2byte 0x0000
.L_020083ac:
	.4byte gPartyState
.L_020083b0:
	.4byte 0x00000017
.L_020083b4:
	.4byte Data_02003f20
.L_020083b8:
	.4byte 0x00000018
.L_020083bc:
	.4byte Data_02003fe0
.L_020083c0:
	.4byte 0x00000019
.L_020083c4:
	.4byte Data_02004058
.L_020083c8:
	.4byte 0x0000001a
.L_020083cc:
	.4byte Data_02004130
.L_020083d0:
	.4byte 0x0000001b
.L_020083d4:
	.4byte Data_020042c8
.L_020083d8:
	.4byte 0x0000001c
.L_020083dc:
	.4byte Data_02004388
.L_020083e0:
	.4byte 0x0000001d
.L_020083e4:
	.4byte Data_02004400
.L_020083e8:
	.4byte 0x0000001e
.L_020083ec:
	.4byte Data_020045f8
.L_020083f0:
	.4byte Data_02003f08
	.section .text.x020083f4,"ax",%progbits
	.global Func_020003f4
	.thumb_func
Func_020003f4:
	push {lr}
	ldr r3, .L_02008460
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008464
	cmp r2, r3
	bne .L_0200840c
	ldr r0, .L_02008468
	b .L_0200845e
.L_0200840c:
	ldr r3, .L_0200846c
	cmp r2, r3
	bne .L_02008416
	ldr r0, .L_02008470
	b .L_0200845e
.L_02008416:
	ldr r3, .L_02008474
	cmp r2, r3
	bne .L_02008420
	ldr r0, .L_02008478
	b .L_0200845e
.L_02008420:
	ldr r3, .L_0200847c
	cmp r2, r3
	bne .L_0200842a
	ldr r0, .L_02008480
	b .L_0200845e
.L_0200842a:
	ldr r3, .L_02008484
	cmp r2, r3
	bne .L_02008434
	ldr r0, .L_02008488
	b .L_0200845e
.L_02008434:
	ldr r3, .L_0200848c
	cmp r2, r3
	bne .L_0200843e
	ldr r0, .L_02008490
	b .L_0200845e
.L_0200843e:
	ldr r3, .L_02008494
	cmp r2, r3
	bne .L_02008448
	ldr r0, .L_02008498
	b .L_0200845e
.L_02008448:
	ldr r3, .L_0200849c
	cmp r2, r3
	bne .L_02008452
	ldr r0, .L_020084a0
	b .L_0200845e
.L_02008452:
	ldr r3, .L_020084a4
	cmp r2, r3
	bne .L_0200845c
	ldr r0, .L_020084a8
	b .L_0200845e
.L_0200845c:
	ldr r0, .L_020084ac
.L_0200845e:
	pop {pc}
.L_02008460:
	.4byte gPartyState
.L_02008464:
	.4byte 0x00000017
.L_02008468:
	.4byte Data_02004664
.L_0200846c:
	.4byte 0x00000018
.L_02008470:
	.4byte Data_020046c4
.L_02008474:
	.4byte 0x00000019
.L_02008478:
	.4byte Data_020046f4
.L_0200847c:
	.4byte 0x0000001a
.L_02008480:
	.4byte Data_02004790
.L_02008484:
	.4byte 0x0000001b
.L_02008488:
	.4byte Data_020047cc
.L_0200848c:
	.4byte 0x0000001c
.L_02008490:
	.4byte Data_020048ec
.L_02008494:
	.4byte 0x0000001d
.L_02008498:
	.4byte Data_02004928
.L_0200849c:
	.4byte 0x0000001e
.L_020084a0:
	.4byte Data_020049a0
.L_020084a4:
	.4byte 0x0000001f
.L_020084a8:
	.4byte Data_020049f4
.L_020084ac:
	.4byte Data_02004658
	.section .text.x020084b0,"ax",%progbits
	.global Func_020004b0
	.thumb_func
Func_020004b0:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #108]
	bl Func_020039d8
	movs r0, #0
	bl Func_02003b08
	ldr r0, .L_020084dc
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
	movs r3, #173
	lsls r3, r3, #1
	adds r2, r5, r3
	movs r3, #1
	strh r3, [r2]
	bl Func_020039e0
	pop {r5, pc}
	.2byte 0x0000
.L_020084dc:
	.4byte MsgFieldStatueLikeDeriShrine
	.section .text.x020084e0,"ax",%progbits
	.global Func_020004e0
	.thumb_func
Func_020004e0:
	push {lr}
	bl Func_020039d8
	movs r0, #0
	bl Func_02003b08
	movs r0, #11
	bl ObjectMotion_EnableActionAndResetMotion
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #20
	movs r0, #11
	bl Func_02003aa8
	ldr r3, .L_02008548
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r1, [r3]
	movs r2, #0
	movs r0, #11
	bl ObjectMotion_SetAngleToward
	ldr r0, .L_0200854c
	bl Func_02003a70
	movs r0, #11
	movs r1, #0
	bl Func_02003a80
	movs r0, #11
	movs r1, #0
	bl Func_02003a80
	movs r0, #11
	movs r1, #4
	bl Object_SetModeById
	movs r2, #20
	movs r0, #11
	movs r1, #0
	bl Func_02003a78
	ldr r1, .L_02008550
	movs r0, #11
	bl ObjectMotion_EnableActionAndSetCallback
	bl Func_020039e0
	pop {pc}
	.2byte 0x0000
.L_02008548:
	.4byte gPartyState
.L_0200854c:
	.4byte MsgFieldStartledCompanion
.L_02008550:
	.4byte Data_02003c10
	.section .text.x02008554,"ax",%progbits
	.global Func_02000554
	.thumb_func
Func_02000554:
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r1, #173
	lsls r1, r1, #1
	adds r2, r3, r1
	movs r3, #1
	strh r3, [r2]
	bx lr
	.2byte 0x0000
	.section .text.x02008568,"ax",%progbits
	.global Func_02000568
	.thumb_func
Func_02000568:
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
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #160
	adds r5, r3, r1
	lsls r2, r2, #1
	adds r1, #112
	adds r7, r3, r1
	adds r2, r2, r3
	ldr r3, .L_02008610
	mov r8, r2
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	adds r6, r0, #0
	bl Func_02003ad0
	mov r10, r0
	movs r0, #8
	bl Object_GetById
	mov r9, r0
	movs r0, #9
	bl Object_GetById
	mov r11, r0
	movs r0, #10
	bl Object_GetById
	ldr r3, [r5, #12]
	ldr r1, .L_02008614
	mov r2, r8
	adds r3, r3, r1
	str r3, [r5, #12]
	mov r1, r8
	ldr r3, [r2, #12]
	movs r2, #128
	lsls r2, r2, #8
	adds r3, r3, r2
	str r3, [r1, #12]
	ldr r3, [r7, #12]
	mov r1, r10
	adds r3, r3, r2
	str r3, [r7, #12]
	ldr r2, .L_02008614
	ldr r3, [r6, #16]
	adds r3, r3, r2
	str r3, [r6, #16]
	ldr r3, [r1, #12]
	adds r3, r3, r2
	str r3, [r1, #12]
	mov r2, r9
	ldr r3, [r2, #16]
	ldr r1, .L_02008614
	adds r3, r3, r1
	str r3, [r2, #16]
	mov r2, r11
	ldr r3, [r2, #16]
	adds r3, r3, r1
	str r3, [r2, #16]
	ldr r3, [r0, #16]
	adds r3, r3, r1
	str r3, [r0, #16]
	movs r0, #4
	bl WaitFrames
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_02008610:
	.4byte gPartyState
.L_02008614:
	.4byte 0xffff8000
	.section .text.x02008618,"ax",%progbits
	.global Func_02000618
	.thumb_func
Func_02000618:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #32]
	movs r1, #132
	lsls r1, r1, #1
	adds r2, r2, r1
	ldr r3, [r2, #12]
	ldr r1, .L_02008638
	movs r0, #4
	adds r3, r3, r1
	str r3, [r2, #12]
	bl WaitFrames
	pop {pc}
	.2byte 0x0000
.L_02008638:
	.4byte 0xffff0000
	.section .text.x0200863c,"ax",%progbits
	.global Func_0200063c
	.thumb_func
Func_0200063c:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	movs r2, #132
	lsls r2, r2, #1
	adds r7, r3, r2
	adds r2, #56
	sub sp, #8
	adds r6, r3, r2
	movs r3, #18
	movs r2, #6
	str r3, [sp, #0]
	movs r3, #56
	str r2, [sp, #4]
	bl Func_02003998
	movs r5, #0
.L_02008660:
	ldr r3, [r7, #12]
	ldr r2, .L_02008684
	movs r0, #1
	adds r3, r3, r2
	str r3, [r7, #12]
	ldr r3, [r6, #12]
	adds r3, r3, r2
	str r3, [r6, #12]
	bl WaitFrames
	adds r3, r5, #1
	lsls r3, r3, #24
	lsrs r5, r3, #24
	cmp r5, #15
	bls .L_02008660
	add sp, #8
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008684:
	.4byte 0xffffe000
	.section .text.x02008688,"ax",%progbits
	.global Func_02000688
	.thumb_func
Func_02000688:
	push {r5, r6, lr}
	sub sp, #8
	bl Func_020039d8
	movs r0, #0
	bl Func_02003b08
	bl Func_020039a0
	ldr r6, .L_02008878
	movs r2, #133
	lsls r2, r2, #2
	adds r6, r6, r2
	movs r1, #128
	ldr r0, [r6]
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #248
	movs r1, #1
	movs r2, #168
	negs r1, r1
	lsls r2, r2, #16
	movs r3, #1
	lsls r0, r0, #16
	bl Motion_CamBounds
	bl Func_02003ac8
	movs r0, #244
	bl Func_02003b48
	movs r3, #128
	movs r5, #128
	lsls r3, r3, #5
	lsls r5, r5, #19
	adds r3, #1
	adds r5, #82
	strh r3, [r5]
	movs r0, #10
	bl WaitFrames
	movs r3, #128
	lsls r3, r3, #5
	adds r3, #2
	strh r3, [r5]
	movs r0, #10
	bl WaitFrames
	movs r3, #128
	lsls r3, r3, #5
	adds r3, #3
	strh r3, [r5]
	movs r0, #10
	bl WaitFrames
	movs r3, #128
	lsls r3, r3, #5
	adds r3, #4
	strh r3, [r5]
	movs r0, #10
	bl WaitFrames
	movs r1, #128
	movs r2, #128
	movs r0, #0
	lsls r1, r1, #9
	lsls r2, r2, #9
	bl Func_020039b8
	movs r2, #3
	str r2, [sp, #4]
	movs r3, #12
	movs r2, #9
	movs r1, #124
	movs r0, #59
	str r3, [sp, #0]
	bl Func_02003998
	ldr r0, [r6]
	bl Object_GetById
	movs r3, #0
	adds r0, #85
	strb r3, [r0]
	movs r1, #1
	ldr r0, [r6]
	bl ObjectMotion_SetActionVariant
	movs r0, #8
	movs r1, #1
	bl ObjectMotion_SetActionVariant
	movs r0, #9
	movs r1, #1
	bl ObjectMotion_SetActionVariant
	movs r0, #10
	movs r1, #1
	bl ObjectMotion_SetActionVariant
	movs r3, #18
	movs r2, #25
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #56
	movs r1, #51
	movs r2, #6
	movs r0, #46
	bl Func_02003998
	movs r0, #10
	bl WaitFrames
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #9
	lsls r0, r0, #9
	lsls r1, r1, #9
	bl Func_020039b8
	movs r0, #65
	movs r1, #51
	bl Func_0200063c
	movs r0, #65
	movs r1, #58
	bl Func_0200063c
	movs r3, #240
	lsls r3, r3, #4
	adds r3, #5
	strh r3, [r5]
	movs r0, #65
	movs r1, #65
	bl Func_0200063c
	movs r0, #65
	movs r1, #72
	bl Func_0200063c
	movs r3, #224
	lsls r3, r3, #4
	adds r3, #6
	strh r3, [r5]
	movs r0, #65
	movs r1, #79
	bl Func_0200063c
	movs r0, #65
	movs r1, #86
	bl Func_0200063c
	movs r3, #208
	lsls r3, r3, #4
	adds r3, #7
	strh r3, [r5]
	movs r0, #65
	movs r1, #93
	bl Func_0200063c
	movs r0, #65
	movs r1, #100
	bl Func_0200063c
	movs r3, #192
	lsls r3, r3, #4
	adds r3, #8
	strh r3, [r5]
	movs r0, #84
	movs r1, #51
	bl Func_0200063c
	movs r0, #84
	movs r1, #58
	bl Func_0200063c
	movs r3, #176
	lsls r3, r3, #4
	adds r3, #9
	strh r3, [r5]
	movs r0, #84
	movs r1, #65
	bl Func_0200063c
	movs r0, #84
	movs r1, #72
	bl Func_0200063c
	movs r3, #160
	lsls r3, r3, #4
	adds r3, #10
	strh r3, [r5]
	movs r0, #84
	movs r1, #79
	bl Func_0200063c
	movs r0, #84
	movs r1, #86
	bl Func_0200063c
	movs r3, #144
	lsls r3, r3, #4
	adds r3, #11
	strh r3, [r5]
	movs r0, #84
	movs r1, #93
	bl Func_0200063c
	movs r0, #84
	movs r1, #100
	bl Func_0200063c
	movs r3, #128
	lsls r3, r3, #4
	adds r3, #12
	strh r3, [r5]
	movs r5, #0
.L_02008840:
	adds r5, #1
	bl Func_02000618
	cmp r5, #23
	bls .L_02008840
	movs r5, #0
.L_0200884c:
	adds r5, #1
	bl Func_02000568
	cmp r5, #23
	bls .L_0200884c
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #93
	str r2, [r3]
	bl Event_ClearStatus1c6
	bl Event_WaitValue1c8Frames
	movs r0, #3
	bl Func_02003ad8
	add sp, #8
	pop {r5, r6, pc}
.L_02008878:
	.4byte gPartyState
	.section .text.x0200887c,"ax",%progbits
	.global Func_0200087c
	.thumb_func
Func_0200087c:
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
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #160
	adds r5, r3, r1
	lsls r2, r2, #1
	adds r1, #112
	adds r7, r3, r1
	adds r2, r2, r3
	ldr r3, .L_02008924
	mov r8, r2
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	adds r6, r0, #0
	bl Func_02003ad0
	mov r10, r0
	movs r0, #8
	bl Object_GetById
	mov r9, r0
	movs r0, #9
	bl Object_GetById
	mov r11, r0
	movs r0, #10
	bl Object_GetById
	ldr r3, [r5, #12]
	movs r2, #128
	lsls r2, r2, #8
	adds r3, r3, r2
	str r3, [r5, #12]
	mov r1, r8
	ldr r3, [r1, #12]
	ldr r1, .L_02008928
	adds r3, r3, r1
	mov r1, r8
	str r3, [r1, #12]
	ldr r3, [r7, #12]
	ldr r1, .L_02008928
	adds r3, r3, r1
	str r3, [r7, #12]
	mov r1, r10
	ldr r3, [r6, #16]
	adds r3, r3, r2
	str r3, [r6, #16]
	ldr r3, [r1, #12]
	adds r3, r3, r2
	str r3, [r1, #12]
	mov r1, r9
	ldr r3, [r1, #16]
	adds r3, r3, r2
	str r3, [r1, #16]
	mov r1, r11
	ldr r3, [r1, #16]
	adds r3, r3, r2
	str r3, [r1, #16]
	ldr r3, [r0, #16]
	adds r3, r3, r2
	str r3, [r0, #16]
	movs r0, #4
	bl WaitFrames
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008924:
	.4byte gPartyState
.L_02008928:
	.4byte 0xffff8000
	.section .text.x0200892c,"ax",%progbits
	.global Func_0200092c
	.thumb_func
Func_0200092c:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #32]
	movs r1, #132
	lsls r1, r1, #1
	adds r2, r2, r1
	ldr r3, [r2, #12]
	movs r1, #128
	lsls r1, r1, #9
	adds r3, r3, r1
	str r3, [r2, #12]
	movs r0, #4
	bl WaitFrames
	pop {pc}
	.section .text.x0200894c,"ax",%progbits
	.global Func_0200094c
	.thumb_func
Func_0200094c:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	movs r2, #132
	lsls r2, r2, #1
	adds r7, r3, r2
	adds r2, #56
	sub sp, #8
	adds r6, r3, r2
	movs r3, #18
	movs r2, #6
	str r3, [sp, #0]
	movs r3, #56
	str r2, [sp, #4]
	bl Func_02003998
	movs r5, #0
.L_02008970:
	ldr r3, [r7, #12]
	movs r2, #128
	lsls r2, r2, #6
	adds r3, r3, r2
	str r3, [r7, #12]
	movs r0, #1
	ldr r3, [r6, #12]
	adds r3, r3, r2
	str r3, [r6, #12]
	bl WaitFrames
	adds r3, r5, #1
	lsls r3, r3, #24
	lsrs r5, r3, #24
	cmp r5, #15
	bls .L_02008970
	add sp, #8
	pop {r5, r6, r7, pc}
	.section .text.x02008994,"ax",%progbits
	.global Func_02000994
	.thumb_func
Func_02000994:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r2, #192
	lsls r2, r2, #18
	ldr r3, [r2, #32]
	movs r2, #132
	lsls r2, r2, #1
	adds r6, r3, r2
	adds r2, #56
	adds r2, r2, r3
	mov r8, r2
	movs r2, #188
	lsls r2, r2, #1
	sub sp, #16
	adds r2, r3, r2
	ldr r5, .L_02008cd8
	str r2, [sp, #12]
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	ldr r0, [r5]
	bl Object_GetById
	adds r7, r0, #0
	bl Func_02003ad0
	mov r10, r0
	movs r0, #8
	bl Object_GetById
	mov r9, r0
	movs r0, #9
	bl Object_GetById
	mov r11, r0
	movs r0, #10
	bl Object_GetById
	str r0, [sp, #8]
	bl Func_020039d8
	movs r0, #0
	bl Func_02003b08
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r2, r2
	negs r1, r1
	movs r3, #0
	negs r0, r0
	bl Motion_CamBounds
	ldr r0, [r5]
	bl Object_GetById
	movs r3, #0
	adds r0, #85
	strb r3, [r0]
	movs r1, #1
	ldr r0, [r5]
	bl ObjectMotion_SetActionVariant
	movs r0, #8
	movs r1, #1
	bl ObjectMotion_SetActionVariant
	movs r0, #9
	movs r1, #1
	bl ObjectMotion_SetActionVariant
	movs r1, #1
	movs r0, #10
	bl ObjectMotion_SetActionVariant
	bl Func_020039a0
	movs r0, #1
	bl WaitFrames
	movs r3, #25
	str r3, [sp, #4]
	movs r5, #18
	movs r0, #46
	movs r1, #51
	movs r2, #6
	movs r3, #56
	str r5, [sp, #0]
	bl Func_02003998
	movs r3, #6
	str r3, [sp, #4]
	movs r0, #84
	movs r1, #100
	movs r2, #6
	movs r3, #56
	str r5, [sp, #0]
	bl Func_02003998
	movs r2, #3
	movs r3, #12
	str r2, [sp, #4]
	movs r0, #59
	movs r1, #124
	movs r2, #9
	str r3, [sp, #0]
	bl Func_02003998
	movs r2, #128
	movs r3, #128
	lsls r2, r2, #4
	lsls r3, r3, #19
	adds r2, #12
	adds r3, #82
	strh r2, [r3]
	movs r0, #1
	bl WaitFrames
	ldr r3, [r6, #12]
	ldr r2, .L_02008cdc
	movs r5, #0
	adds r3, r3, r2
	str r3, [r6, #12]
	mov r2, r8
	ldr r3, [r2, #12]
	ldr r2, .L_02008ce0
	adds r3, r3, r2
	mov r2, r8
	str r3, [r2, #12]
	ldr r2, [sp, #12]
	ldr r3, [r2, #12]
	movs r2, #192
	lsls r2, r2, #12
	adds r3, r3, r2
	ldr r2, [sp, #12]
	str r3, [r2, #12]
	ldr r3, [r7, #16]
	ldr r2, .L_02008ce4
	adds r3, r3, r2
	str r3, [r7, #16]
	mov r2, r10
	ldr r3, [r2, #12]
	ldr r2, .L_02008ce4
	adds r3, r3, r2
	mov r2, r10
	str r3, [r2, #12]
	mov r2, r9
	ldr r3, [r2, #16]
	ldr r2, .L_02008ce4
	adds r3, r3, r2
	mov r2, r9
	str r3, [r2, #16]
	mov r2, r11
	ldr r3, [r2, #16]
	ldr r2, .L_02008ce4
	adds r3, r3, r2
	mov r2, r11
	str r3, [r2, #16]
	ldr r2, [sp, #8]
	ldr r3, [r2, #16]
	ldr r2, .L_02008ce4
	adds r3, r3, r2
	ldr r2, [sp, #8]
	str r3, [r2, #16]
	bl Func_02003988
	movs r0, #4
	bl WaitFrames
	movs r2, #192
	lsls r2, r2, #18
	ldr r3, [r2, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #93
	str r2, [r3]
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r0, r0, #9
	lsls r1, r1, #9
	lsls r2, r2, #9
	bl Func_020039b8
	movs r0, #245
	bl Func_02003b48
.L_02008b1e:
	adds r5, #1
	bl Func_0200087c
	cmp r5, #24
	bls .L_02008b1e
	movs r5, #0
.L_02008b2a:
	adds r5, #1
	bl Func_0200092c
	cmp r5, #24
	bls .L_02008b2a
	movs r0, #84
	movs r1, #93
	bl Func_0200094c
	movs r0, #84
	movs r1, #86
	bl Func_0200094c
	movs r3, #144
	movs r5, #128
	lsls r3, r3, #4
	lsls r5, r5, #19
	adds r3, #11
	adds r5, #82
	strh r3, [r5]
	movs r0, #84
	movs r1, #79
	bl Func_0200094c
	movs r0, #84
	movs r1, #72
	bl Func_0200094c
	movs r3, #160
	lsls r3, r3, #4
	adds r3, #10
	strh r3, [r5]
	movs r0, #84
	movs r1, #65
	bl Func_0200094c
	movs r0, #84
	movs r1, #58
	bl Func_0200094c
	movs r3, #176
	lsls r3, r3, #4
	adds r3, #9
	strh r3, [r5]
	movs r0, #84
	movs r1, #51
	bl Func_0200094c
	movs r0, #65
	movs r1, #100
	bl Func_0200094c
	movs r3, #192
	lsls r3, r3, #4
	adds r3, #8
	strh r3, [r5]
	movs r0, #65
	movs r1, #93
	bl Func_0200094c
	movs r0, #65
	movs r1, #86
	bl Func_0200094c
	movs r3, #208
	lsls r3, r3, #4
	adds r3, #7
	strh r3, [r5]
	movs r0, #65
	movs r1, #79
	bl Func_0200094c
	movs r0, #65
	movs r1, #72
	bl Func_0200094c
	movs r3, #224
	lsls r3, r3, #4
	adds r3, #6
	strh r3, [r5]
	movs r0, #65
	movs r1, #65
	bl Func_0200094c
	movs r0, #65
	movs r1, #58
	bl Func_0200094c
	movs r3, #240
	lsls r3, r3, #4
	adds r3, #5
	strh r3, [r5]
	movs r0, #65
	movs r1, #51
	bl Func_0200094c
	movs r1, #128
	movs r2, #128
	movs r0, #0
	lsls r1, r1, #9
	lsls r2, r2, #9
	bl Func_020039b8
	movs r3, #128
	lsls r3, r3, #5
	adds r3, #4
	strh r3, [r5]
	movs r0, #10
	bl WaitFrames
	movs r0, #195
	lsls r0, r0, #1
	bl Func_02003b48
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	negs r0, r0
	negs r1, r1
	adds r2, #102
	bl Func_020039b8
	movs r3, #128
	lsls r3, r3, #5
	adds r3, #3
	strh r3, [r5]
	movs r0, #10
	bl WaitFrames
	movs r3, #128
	lsls r3, r3, #5
	adds r3, #2
	strh r3, [r5]
	movs r0, #10
	bl WaitFrames
	movs r3, #128
	lsls r3, r3, #5
	adds r3, #1
	strh r3, [r5]
	movs r0, #10
	bl WaitFrames
	movs r3, #128
	lsls r3, r3, #5
	strh r3, [r5]
	movs r3, #18
	movs r2, #25
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #103
	movs r1, #51
	movs r2, #6
	movs r3, #56
	bl Func_02003998
	movs r5, #3
	movs r1, #108
	movs r3, #12
	movs r2, #9
	movs r0, #46
	str r3, [sp, #0]
	str r5, [sp, #4]
	bl Func_02003998
	movs r0, #10
	bl WaitFrames
	bl Func_020039c0
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #88
	str r2, [r3]
	movs r0, #8
	bl Func_02003854
	movs r0, #9
	bl Func_02003854
	movs r0, #10
	bl Func_02003854
	ldr r3, .L_02008cd8
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	strb r3, [r0]
	adds r3, r7, #0
	movs r1, #0
	adds r3, #85
	strb r5, [r3]
	str r1, [r7, #12]
	str r1, [r7, #20]
	bl Func_020039e0
	add sp, #16
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_02008cd8:
	.4byte gPartyState
.L_02008cdc:
	.4byte 0xffbc0000
.L_02008ce0:
	.4byte 0xffec0000
.L_02008ce4:
	.4byte 0xfff40000
	.section .text.x02008ce8,"ax",%progbits
	.global Func_02000ce8
	.thumb_func
Func_02000ce8:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r2, #192
	lsls r2, r2, #18
	ldr r3, [r2, #32]
	mov r8, r2
	movs r2, #160
	lsls r2, r2, #1
	ldr r5, .L_02008e30
	adds r6, r3, r2
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	ldr r0, [r5]
	sub sp, #8
	bl Object_GetById
	adds r7, r0, #0
	bl Func_020039d8
	movs r0, #0
	bl Func_02003b08
	movs r0, #244
	bl Func_02003b48
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r0, r0
	negs r1, r1
	negs r2, r2
	movs r3, #0
	bl Motion_CamBounds
	movs r0, #172
	movs r1, #1
	movs r2, #248
	lsls r0, r0, #17
	negs r1, r1
	lsls r2, r2, #16
	movs r3, #0
	bl Motion_CamBounds
	movs r3, #19
	movs r2, #6
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #18
	movs r2, #62
	movs r1, #77
	movs r0, #62
	bl Func_02003998
	movs r0, #1
	bl WaitFrames
	movs r1, #2
	ldr r0, [r5]
	bl ObjectMotion_SetActionVariant
	adds r2, r7, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	ldr r2, .L_02008e34
	movs r5, #0
	str r2, [r7, #12]
	str r2, [r7, #20]
	ldr r3, [r6, #12]
	adds r3, r3, r2
	str r3, [r6, #12]
	bl Func_02003988
	movs r0, #1
	bl WaitFrames
	mov r2, r8
	ldr r3, [r2, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #87
	str r2, [r3]
	bl Event_SetStatus1c6
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r0, r0, #9
	lsls r1, r1, #9
	lsls r2, r2, #9
	bl Func_020039b8
.L_02008da6:
	ldr r3, [r7, #12]
	movs r2, #128
	lsls r2, r2, #8
	adds r3, r3, r2
	str r3, [r7, #12]
	ldr r3, [r7, #20]
	movs r0, #2
	adds r3, r3, r2
	str r3, [r7, #20]
	adds r5, #1
	ldr r3, [r6, #12]
	adds r3, r3, r2
	str r3, [r6, #12]
	bl WaitFrames
	cmp r5, #99
	bls .L_02008da6
	movs r0, #144
	lsls r0, r0, #1
	bl Func_02003b48
	movs r0, #195
	lsls r0, r0, #1
	bl Func_02003b48
	movs r1, #0
	movs r0, #0
	movs r2, #0
	bl Func_020039b8
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #93
	str r2, [r3]
	ldr r3, .L_02008e30
	adds r2, #11
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	strb r3, [r0]
	movs r2, #6
	movs r3, #19
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #69
	movs r2, #62
	movs r3, #18
	movs r0, #62
	bl Func_02003998
	movs r0, #1
	bl WaitFrames
	bl Func_020039e0
	add sp, #8
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008e30:
	.4byte gPartyState
.L_02008e34:
	.4byte 0xffce0000
	.section .text.x02008e38,"ax",%progbits
	.global Func_02000e38
	.thumb_func
Func_02000e38:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	movs r2, #160
	lsls r2, r2, #1
	ldr r5, .L_02008f10
	adds r7, r3, r2
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	ldr r0, [r5]
	sub sp, #8
	bl Object_GetById
	adds r6, r0, #0
	bl Func_020039d8
	movs r0, #0
	bl Func_02003b08
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r0, r0
	negs r1, r1
	negs r2, r2
	movs r3, #0
	bl Motion_CamBounds
	movs r0, #172
	movs r1, #1
	movs r2, #248
	lsls r0, r0, #17
	negs r1, r1
	lsls r2, r2, #16
	movs r3, #1
	bl Motion_CamBounds
	movs r3, #19
	movs r2, #6
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #18
	movs r0, #62
	movs r1, #77
	movs r2, #62
	bl Func_02003998
	movs r1, #128
	ldr r0, [r5]
	movs r2, #20
	lsls r1, r1, #7
	bl ObjectMotion_ArmCallback
	ldr r0, [r5]
	movs r1, #2
	bl ObjectMotion_SetActionVariant
	adds r2, r6, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	movs r0, #245
	bl Func_02003b48
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r0, r0, #9
	lsls r1, r1, #9
	lsls r2, r2, #9
	bl Func_020039b8
	movs r5, #0
.L_02008ece:
	ldr r3, [r6, #12]
	ldr r2, .L_02008f14
	movs r0, #2
	adds r3, r3, r2
	str r3, [r6, #12]
	ldr r3, [r6, #20]
	adds r3, r3, r2
	str r3, [r6, #20]
	ldr r3, [r7, #12]
	adds r3, r3, r2
	str r3, [r7, #12]
	bl WaitFrames
	cmp r5, #90
	bne .L_02008f00
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #87
	str r2, [r3]
	bl Event_ClearStatus1c6
.L_02008f00:
	adds r5, #1
	cmp r5, #99
	bls .L_02008ece
	movs r0, #1
	bl Func_02003ad8
	add sp, #8
	pop {r5, r6, r7, pc}
.L_02008f10:
	.4byte gPartyState
.L_02008f14:
	.4byte 0xffff8000
	.section .text.x02008f18,"ax",%progbits
	.global Func_02000f18
	.thumb_func
Func_02000f18:
	push {r5, lr}
	adds r5, r0, #0
	bl Func_02003aa0
	adds r0, r5, #0
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #32
	orrs r3, r2
	strb r3, [r0]
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x02008f34,"ax",%progbits
	.global Func_02000f34
	.thumb_func
Func_02000f34:
	push {lr}
	sub sp, #8
	movs r3, #8
	movs r2, #18
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #0
	movs r2, #1
	movs r3, #1
	movs r0, #0
	bl Func_020039a8
	movs r0, #128
	lsls r0, r0, #2
	bl GameFlag_SetBit
	add sp, #8
	pop {pc}
	.section .text.x02008f58,"ax",%progbits
	.global Func_02000f58
	.thumb_func
Func_02000f58:
	push {lr}
	sub sp, #8
	movs r3, #10
	movs r2, #18
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #0
	movs r2, #1
	movs r3, #1
	movs r0, #0
	bl Func_020039a8
	movs r0, #129
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
	add sp, #8
	pop {pc}
	.2byte 0x0000
	.section .text.x02008f80,"ax",%progbits
	.global Func_02000f80
	.thumb_func
Func_02000f80:
	push {lr}
	sub sp, #8
	movs r3, #10
	movs r2, #17
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #0
	movs r2, #1
	movs r3, #1
	movs r0, #0
	bl Func_020039a8
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #2
	bl GameFlag_SetBit
	add sp, #8
	pop {pc}
	.2byte 0x0000
	.section .text.x02008fa8,"ax",%progbits
	.global Func_02000fa8
	.thumb_func
Func_02000fa8:
	push {lr}
	ldr r0, .L_02008fb4
	bl Func_02003b38
	pop {pc}
	.2byte 0x0000
.L_02008fb4:
	.4byte Data_02003cbc
	.section .text.x02008fb8,"ax",%progbits
	.global Func_02000fb8
	.thumb_func
Func_02000fb8:
	push {lr}
	bl Func_02003b40
	movs r0, #8
	bl Object_GetById
	ldr r3, [r0, #8]
	asrs r3, r3, #20
	cmp r3, #21
	bne .L_02008fe0
	ldr r3, [r0, #16]
	asrs r3, r3, #20
	cmp r3, #13
	bne .L_02008fe0
	movs r0, #132
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
	b .L_02008fea
.L_02008fe0:
	movs r0, #132
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_ClearBit
.L_02008fea:
	pop {pc}
	.section .text.x02008fec,"ax",%progbits
	.global Func_02000fec
	.thumb_func
Func_02000fec:
	push {r5, lr}
	movs r0, #8
	bl Object_GetById
	adds r5, r0, #0
	bl Func_02003b40
	ldr r3, [r5, #8]
	asrs r3, r3, #20
	cmp r3, #12
	bne .L_0200900c
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #76
	bl GameFlag_SetBit
.L_0200900c:
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x02009010,"ax",%progbits
	.global Func_02001010
	.thumb_func
Func_02001010:
	push {r5, r6, r7, lr}
	adds r6, r0, #0
	adds r7, r6, #0
	adds r7, #85
	movs r3, #3
	strb r3, [r7]
.L_0200901c:
	movs r0, #1
	bl WaitFrames
	ldr r5, [r6, #40]
	cmp r5, #0
	bne .L_0200901c
	movs r0, #188
	bl Func_02003b48
	movs r0, #10
	bl WaitFrames
	strb r5, [r7]
	pop {r5, r6, r7, pc}
	.section .text.x02009038,"ax",%progbits
	.global Func_02001038
	.thumb_func
Func_02001038:
	push {r5, r6, lr}
	movs r0, #8
	sub sp, #8
	bl Object_GetById
	adds r5, r0, #0
	ldr r3, [r5, #16]
	asrs r6, r3, #20
	cmp r6, #16
	bne .L_02009084
	bl Func_020039d8
	movs r0, #0
	bl Func_02003b08
	adds r0, r5, #0
	bl Func_02001010
	movs r3, #15
	str r3, [sp, #0]
	movs r2, #1
	movs r3, #1
	movs r0, #0
	movs r1, #0
	str r6, [sp, #4]
	bl Func_020039a8
	movs r0, #8
	movs r1, #3
	bl ObjectMotion_SetActionVariant
	bl Func_020039e0
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #1
	bl GameFlag_SetBit
.L_02009084:
	add sp, #8
	pop {r5, r6, pc}
	.section .text.x02009088,"ax",%progbits
	.global Func_02001088
	.thumb_func
Func_02001088:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r3, .L_020090e4
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	movs r6, #9
	mov r8, r0
.L_020090a0:
	adds r0, r6, #0
	bl Object_GetById
	mov r2, r8
	ldr r3, [r2, #12]
	movs r2, #128
	adds r5, r0, #0
	lsls r2, r2, #13
	adds r5, #35
	adds r7, r6, #1
	cmp r3, r2
	ble .L_020090c8
	adds r0, r6, #0
	movs r1, #2
	bl ObjectMotion_SetActionVariant
	ldrb r2, [r5]
	movs r3, #2
	orrs r3, r2
	b .L_020090d6
.L_020090c8:
	adds r0, r6, #0
	movs r1, #1
	bl ObjectMotion_SetActionVariant
	ldrb r2, [r5]
	movs r3, #253
	ands r3, r2
.L_020090d6:
	strb r3, [r5]
	adds r6, r7, #0
	cmp r6, #13
	bls .L_020090a0
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_020090e4:
	.4byte gPartyState
	.section .text.x020090e8,"ax",%progbits
	.global Func_020010e8
	.thumb_func
Func_020010e8:
	push {r5, r6, lr}
	ldr r6, .L_02009124
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r6, r2
	ldr r0, [r3]
	bl Object_GetById
	adds r5, r0, #0
	bl Func_02003b28
	ldr r3, [r5, #12]
	movs r2, #128
	lsls r2, r2, #13
	cmp r3, r2
	bge .L_02009114
	movs r3, #226
	lsls r3, r3, #1
	adds r3, #255
	adds r2, r6, r3
	movs r3, #0
	b .L_0200911e
.L_02009114:
	movs r3, #226
	lsls r3, r3, #1
	adds r3, #255
	adds r2, r6, r3
	movs r3, #1
.L_0200911e:
	strb r3, [r2]
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02009124:
	.4byte gPartyState
	.section .text.x02009128,"ax",%progbits
	.global Func_02001128
	.thumb_func
Func_02001128:
	push {lr}
	ldr r3, .L_0200919c
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	ldr r3, [r0, #12]
	movs r2, #128
	lsls r2, r2, #13
	cmp r3, r2
	bge .L_02009180
	ldr r3, [r0, #16]
	movs r2, #148
	lsls r2, r2, #17
	cmp r3, r2
	bge .L_02009166
	movs r0, #10
	movs r1, #1
	bl ObjectMotion_SetActionVariant
	movs r0, #11
	movs r1, #1
	bl ObjectMotion_SetActionVariant
	movs r0, #12
	movs r1, #2
	bl ObjectMotion_SetActionVariant
	b .L_02009198
.L_02009166:
	movs r0, #10
	movs r1, #2
	bl ObjectMotion_SetActionVariant
	movs r0, #11
	movs r1, #2
	bl ObjectMotion_SetActionVariant
	movs r0, #12
	movs r1, #3
	bl ObjectMotion_SetActionVariant
	b .L_02009198
.L_02009180:
	movs r0, #10
	movs r1, #2
	bl ObjectMotion_SetActionVariant
	movs r0, #11
	movs r1, #2
	bl ObjectMotion_SetActionVariant
	movs r0, #12
	movs r1, #3
	bl ObjectMotion_SetActionVariant
.L_02009198:
	pop {pc}
	.2byte 0x0000
.L_0200919c:
	.4byte gPartyState
	.section .text.x020091a0,"ax",%progbits
	.global Func_020011a0
	.thumb_func
Func_020011a0:
	push {r5, lr}
	movs r0, #8
	bl Object_GetById
	adds r5, r0, #0
	movs r0, #9
	bl Object_GetById
	ldr r3, [r5, #8]
	str r3, [r0, #8]
	movs r3, #160
	lsls r3, r3, #13
	str r3, [r0, #12]
	ldr r3, [r5, #16]
	str r3, [r0, #16]
	pop {r5, pc}
	.section .text.x020091c0,"ax",%progbits
	.global Func_020011c0
	.thumb_func
Func_020011c0:
	push {lr}
	ldr r3, .L_020091dc
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #223
	ands r3, r2
	strb r3, [r0]
	pop {pc}
.L_020091dc:
	.4byte gPartyState
	.section .text.x020091e0,"ax",%progbits
	.global Func_020011e0
	.thumb_func
Func_020011e0:
	push {lr}
	ldr r3, .L_020091fc
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #32
	orrs r3, r2
	strb r3, [r0]
	pop {pc}
.L_020091fc:
	.4byte gPartyState
	.section .text.x02009200,"ax",%progbits
	.global Func_02001200
	.thumb_func
Func_02001200:
	push {lr}
	ldr r3, .L_02009220
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02009224
	cmp r2, r3
	bne .L_0200921c
	movs r1, #9
	movs r2, #3
	bl Func_02001250
.L_0200921c:
	pop {pc}
	.2byte 0x0000
.L_02009220:
	.4byte gPartyState
.L_02009224:
	.4byte 0x0000001c
	.section .text.x02009228,"ax",%progbits
	.global Func_02001228
	.thumb_func
Func_02001228:
	push {lr}
	movs r0, #1
	bl Func_02001200
	movs r0, #130
	lsls r0, r0, #2
	bl GameFlag_SetBit
	pop {pc}
	.2byte 0x0000
	.section .text.x0200923c,"ax",%progbits
	.global Func_0200123c
	.thumb_func
Func_0200123c:
	push {lr}
	movs r0, #0
	bl Func_02001200
	movs r0, #130
	lsls r0, r0, #2
	bl GameFlag_ClearBit
	pop {pc}
	.2byte 0x0000
	.section .text.x02009250,"ax",%progbits
	.global Func_02001250
	.thumb_func
Func_02001250:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r6, r2, #0
	movs r5, #0
	mov r8, r0
	adds r7, r1, #0
	cmp r5, r6
	bcs .L_02009288
.L_02009262:
	adds r0, r7, r5
	bl Object_GetById
	mov r3, r8
	adds r0, #35
	adds r1, r5, #1
	cmp r3, #0
	beq .L_0200927a
	ldrb r2, [r0]
	movs r3, #239
	ands r3, r2
	b .L_02009280
.L_0200927a:
	ldrb r2, [r0]
	movs r3, #16
	orrs r3, r2
.L_02009280:
	strb r3, [r0]
	adds r5, r1, #0
	cmp r5, r6
	bcc .L_02009262
.L_02009288:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x02009290,"ax",%progbits
	.global Func_02001290
	.thumb_func
Func_02001290:
	push {lr}
	ldr r3, .L_020092b0
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	movs r1, #1
	bl ObjectMotion_SetActionVariant
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #10
	bl GameFlag_SetBit
	pop {pc}
	.2byte 0x0000
.L_020092b0:
	.4byte gPartyState
	.section .text.x020092b4,"ax",%progbits
	.global Func_020012b4
	.thumb_func
Func_020012b4:
	push {r5, lr}
	ldr r5, .L_020092e4
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	ldr r0, [r5]
	movs r1, #2
	bl ObjectMotion_SetActionVariant
	ldr r0, [r5]
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	strb r3, [r0]
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #10
	bl GameFlag_ClearBit
	pop {r5, pc}
	.2byte 0x0000
.L_020092e4:
	.4byte gPartyState
	.section .text.x020092e8,"ax",%progbits
	.global Func_020012e8
	.thumb_func
Func_020012e8:
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
	bge .L_02009318
	adds r3, #15
.L_02009318:
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
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x02009340,"ax",%progbits
	.global Func_02001340
	.thumb_func
Func_02001340:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, .L_02009420
	mov r8, r0
	ldr r7, [r0, #80]
	movs r0, #133
	lsls r0, r0, #2
	adds r3, r3, r0
	adds r6, r1, #0
	movs r1, #129
	ldr r0, [r3]
	lsls r1, r1, #1
	bl Func_02003ab0
	movs r0, #210
	bl Func_02003b48
	mov r2, r8
	movs r3, #0
	adds r2, #85
	strb r3, [r2]
	mov r10, r3
.L_02009370:
	ldr r3, [r6, #12]
	ldr r2, .L_02009424
	movs r5, #0
	adds r3, r3, r2
	str r3, [r6, #12]
.L_0200937a:
	ldrh r3, [r7, #18]
	movs r0, #1
	adds r3, #128
	strh r3, [r7, #18]
	adds r5, #1
	bl WaitFrames
	cmp r5, #3
	bls .L_0200937a
	ldr r3, [r6, #12]
	movs r0, #128
	lsls r0, r0, #9
	adds r3, r3, r0
	str r3, [r6, #12]
	movs r5, #0
.L_02009398:
	ldrh r3, [r7, #18]
	movs r2, #255
	lsls r2, r2, #8
	adds r2, #128
	adds r3, r3, r2
	strh r3, [r7, #18]
	movs r0, #1
	adds r5, #1
	bl WaitFrames
	cmp r5, #7
	bls .L_02009398
	ldr r3, [r6, #12]
	ldr r0, .L_02009424
	movs r5, #0
	adds r3, r3, r0
	str r3, [r6, #12]
.L_020093ba:
	ldrh r3, [r7, #18]
	movs r0, #1
	adds r3, #128
	strh r3, [r7, #18]
	adds r5, #1
	bl WaitFrames
	cmp r5, #3
	bls .L_020093ba
	movs r2, #1
	add r10, r2
	mov r3, r10
	cmp r3, #1
	bls .L_02009370
	ldr r3, [r6, #12]
	movs r0, #128
	lsls r0, r0, #9
	adds r3, r3, r0
	str r3, [r6, #12]
	movs r0, #20
	bl WaitFrames
	mov r0, r8
	ldr r2, [r0, #8]
	ldr r3, [r6, #8]
	ldr r1, .L_02009428
	subs r3, r3, r2
	str r3, [r1]
	ldr r2, [r0, #12]
	ldr r3, [r6, #12]
	subs r3, r3, r2
	str r3, [r1, #4]
	ldr r2, [r0, #16]
	ldr r3, [r6, #16]
	subs r3, r3, r2
	str r3, [r1, #8]
	ldr r3, .L_02009420
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009420:
	.4byte gPartyState
.L_02009424:
	.4byte 0xffff0000
.L_02009428:
	.4byte Data_02004a18
	.section .text.x0200942c,"ax",%progbits
	.global Func_0200142c
	.thumb_func
Func_0200142c:
	push {r5, lr}
	ldr r3, .L_0200945c
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	adds r5, r0, #0
	ldr r0, [r3]
	bl Object_GetById
	ldr r1, .L_02009460
	ldr r3, [r0, #8]
	ldr r2, [r1]
	adds r3, r3, r2
	str r3, [r5, #8]
	ldr r2, [r1, #4]
	ldr r3, [r0, #12]
	adds r3, r3, r2
	str r3, [r5, #12]
	ldr r3, [r0, #16]
	ldr r2, [r1, #8]
	adds r3, r3, r2
	str r3, [r5, #16]
	pop {r5, pc}
	.2byte 0x0000
.L_0200945c:
	.4byte gPartyState
.L_02009460:
	.4byte Data_02004a18
	.section .text.x02009464,"ax",%progbits
	.global Func_02001464
	.thumb_func
Func_02001464:
	push {lr}
	movs r0, #14
	bl Object_GetById
	bl Func_0200142c
	pop {pc}
	.2byte 0x0000
	.section .text.x02009474,"ax",%progbits
	.global Func_02001474
	.thumb_func
Func_02001474:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r5, .L_0200963c
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
	ldr r0, [r5]
	sub sp, #68
	bl Object_GetById
	mov r10, r0
	movs r0, #14
	bl Object_GetById
	ldr r3, [r0, #80]
	mov r9, r0
	mov r11, r3
	bl Func_020039d8
	movs r0, #0
	bl Func_02003b08
	movs r1, #204
	movs r2, #164
	lsls r2, r2, #1
	ldr r0, [r5]
	lsls r1, r1, #1
	bl ObjectMotion_SetPositionAndReset
	mov r0, r10
	mov r1, r9
	bl Func_02001340
	movs r3, #24
	movs r2, #70
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #50
	movs r2, #3
	movs r3, #1
	movs r0, #0
	bl Func_020039a8
	movs r0, #140
	lsls r0, r0, #2
	bl GameFlag_SetBit
	movs r0, #1
	bl WaitFrames
	mov r2, r10
	adds r2, #85
	movs r3, #3
	strb r3, [r2]
	movs r0, #14
	movs r1, #3
	bl ObjectMotion_SetActionVariant
	mov r1, r9
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #2
	orrs r3, r2
	strb r3, [r1]
	movs r1, #144
	ldr r0, .L_02009640
	lsls r1, r1, #3
	bl Scheduler_AddOrUpdateCallback
.L_02009508:
	movs r0, #1
	bl WaitFrames
	mov r4, r10
	ldr r3, [r4, #40]
	cmp r3, #0
	bne .L_02009508
	movs r0, #227
	bl Func_02003b48
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #9
	lsls r0, r0, #9
	lsls r1, r1, #10
	bl Func_020039b8
	add r2, sp, #28
	movs r3, #7
	str r3, [r2, #4]
	ldr r3, .L_02009644
	mov r8, r2
	str r3, [r2, #36]
	movs r3, #163
	lsls r3, r3, #8
	adds r3, #215
	str r3, [r2, #8]
	str r3, [r2, #12]
	movs r7, #0
.L_02009544:
	lsls r5, r7, #12
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
	ldr r3, .L_02009648
	adds r7, #1
	adds r2, r2, r3
	str r2, [r6]
	bl Random16Far
	lsls r3, r0, #1
	ldr r5, [r6, #8]
	adds r3, r3, r0
	ldr r4, .L_0200964c
	lsls r3, r3, #13
	lsrs r3, r3, #16
	adds r5, r5, r3
	adds r5, r5, r4
	str r5, [r6, #8]
	mov r2, r10
	ldr r1, [r2, #12]
	ldr r4, [r6, #4]
	ldr r3, .L_02009650
	ldr r0, [r2, #8]
	adds r1, r1, r3
	ldr r2, [r2, #16]
	ldr r3, [r6]
	str r4, [sp, #0]
	ldr r4, .L_02009654
	str r5, [sp, #4]
	str r4, [sp, #8]
	mov r4, r8
	str r4, [sp, #12]
	bl Func_020000b8
	cmp r7, #16
	bls .L_02009544
	movs r3, #128
	lsls r3, r3, #11
	mov r2, r10
	str r3, [r2, #40]
	movs r0, #2
	bl WaitFrames
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	negs r0, r0
	negs r1, r1
	adds r2, #102
	bl Func_020039b8
	ldr r0, .L_02009640
	bl Scheduler_RemoveCallbackFar
	movs r7, #0
.L_020095de:
	mov r4, r11
	ldrh r3, [r4, #18]
	movs r2, #128
	lsls r2, r2, #2
	adds r3, r3, r2
	strh r3, [r4, #18]
	mov r2, r9
	ldr r3, [r2, #16]
	movs r4, #128
	lsls r4, r4, #9
	adds r3, r3, r4
	str r3, [r2, #16]
	movs r0, #1
	adds r7, #1
	bl WaitFrames
	cmp r7, #7
	bls .L_020095de
	bl Func_020039c0
	ldr r3, .L_0200963c
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	movs r1, #1
	bl ObjectDispatch_SetSingleChildField26
	mov r2, r10
	adds r2, #34
	movs r3, #0
	strb r3, [r2]
	movs r0, #10
	bl WaitFrames
	bl Func_020039e0
	add sp, #68
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200963c:
	.4byte gPartyState
.L_02009640:
	.4byte Func_02001464
.L_02009644:
	.4byte Func_020012e8
.L_02009648:
	.4byte 0xffffa000
.L_0200964c:
	.4byte 0xffffd000
.L_02009650:
	.4byte 0xfffc0000
.L_02009654:
	.4byte 0x01090001
	.section .text.x02009658,"ax",%progbits
	.global Func_02001658
	.thumb_func
Func_02001658:
	push {r5, r6, r7, lr}
	adds r7, r0, #0
	adds r5, r1, #0
.L_0200965e:
	movs r0, #1
	bl WaitFrames
	ldr r3, [r7, #40]
	cmp r3, #0
	bne .L_0200965e
	adds r6, r5, #0
	adds r6, #21
	movs r0, #240
	bl Func_02003b48
	adds r5, #22
	adds r0, r6, #0
	movs r1, #3
	bl ObjectMotion_SetActionVariant
	adds r0, r5, #0
	movs r1, #3
	bl ObjectMotion_SetActionVariant
	adds r0, r6, #0
	bl Object_GetById
	ldr r1, [r7, #8]
	ldr r3, [r7, #16]
	movs r2, #0
	bl Object_SetPositionAndResetMotion
	adds r0, r5, #0
	bl Object_GetById
	ldr r3, [r7, #16]
	ldr r2, .L_020096c4
	ldr r1, [r7, #8]
	adds r3, r3, r2
	movs r2, #0
	bl Object_SetPositionAndResetMotion
	adds r0, r6, #0
	movs r1, #1
	bl Object_SetModeById
	adds r0, r5, #0
	movs r1, #1
	bl Object_SetModeById
	movs r0, #20
	bl WaitFrames
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_020096c4:
	.4byte 0xfff00000
	.section .text.x020096c8,"ax",%progbits
	.global Func_020016c8
	.thumb_func
Func_020016c8:
	push {lr}
	movs r0, #15
	bl Object_GetById
	bl Func_0200142c
	pop {pc}
	.2byte 0x0000
	.section .text.x020096d8,"ax",%progbits
	.global Func_020016d8
	.thumb_func
Func_020016d8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r6, .L_020098a4
	movs r2, #133
	lsls r2, r2, #2
	adds r6, r6, r2
	ldr r0, [r6]
	sub sp, #8
	bl Object_GetById
	mov r10, r0
	movs r0, #15
	bl Object_GetById
	mov r8, r0
	bl Func_020039d8
	movs r0, #0
	bl Func_02003b08
	movs r1, #164
	ldr r0, [r6]
	movs r2, #216
	lsls r1, r1, #1
	bl ObjectMotion_SetPositionAndReset
	mov r0, r10
	mov r1, r8
	bl Func_02001340
	movs r3, #13
	str r3, [sp, #4]
	movs r5, #19
	movs r0, #0
	movs r1, #0
	movs r2, #3
	movs r3, #1
	str r5, [sp, #0]
	bl Func_020039a8
	movs r3, #33
	str r3, [sp, #4]
	movs r1, #0
	movs r2, #2
	movs r3, #1
	movs r0, #40
	str r5, [sp, #0]
	mov r7, r10
	bl Func_020039a8
	movs r0, #1
	bl WaitFrames
	adds r7, #85
	movs r2, #3
	movs r3, #0
	strb r2, [r7]
	movs r0, #15
	movs r1, #3
	mov r11, r3
	bl ObjectMotion_SetActionVariant
	movs r3, #35
	add r8, r3
	mov r2, r8
	ldrb r3, [r2]
	movs r2, #2
	mov r9, r2
	mov r2, r9
	orrs r3, r2
	mov r2, r8
	strb r3, [r2]
	ldr r3, .L_020098a8
	movs r1, #144
	mov r8, r3
	mov r0, r8
	lsls r1, r1, #3
	bl Scheduler_AddOrUpdateCallback
	mov r0, r10
	movs r1, #0
	bl Func_02001658
	movs r1, #1
	movs r0, #9
	bl ObjectMotion_SetActionVariant
	movs r0, #9
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r5, #253
	adds r3, r5, #0
	ands r3, r2
	strb r3, [r0]
	movs r1, #1
	movs r0, #11
	bl ObjectMotion_SetActionVariant
	movs r0, #11
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	mov r2, r11
	ands r5, r3
	strb r5, [r0]
	strb r2, [r7]
	ldr r0, [r6]
	bl Object_GetById
	movs r1, #1
	bl ObjectDispatch_SetSingleChildField26
	movs r1, #153
	movs r2, #200
	lsls r1, r1, #8
	lsls r2, r2, #5
	ldr r0, [r6]
	adds r1, #153
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r1, #164
	ldr r0, [r6]
	lsls r1, r1, #1
	movs r2, #224
	bl ObjectMotion_SetPositionAndCommit
	movs r1, #66
	movs r2, #128
	ldr r0, [r6]
	adds r1, #255
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndCommit
	movs r0, #21
	movs r1, #0
	movs r2, #0
	bl Func_02003a30
	movs r0, #22
	movs r1, #0
	movs r2, #0
	bl Func_02003a30
	movs r1, #66
	movs r2, #160
	ldr r0, [r6]
	adds r1, #255
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndCommit
	movs r1, #66
	movs r2, #200
	ldr r0, [r6]
	adds r1, #255
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndCommit
	movs r1, #66
	movs r2, #240
	ldr r0, [r6]
	adds r1, #255
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndCommit
	movs r1, #66
	movs r2, #134
	lsls r2, r2, #2
	ldr r0, [r6]
	adds r1, #255
	bl ObjectMotion_SetPositionAndCommit
	movs r0, #161
	bl Func_02003b48
	movs r1, #2
	movs r0, #9
	bl ObjectMotion_SetActionVariant
	movs r0, #9
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	mov r2, r9
	orrs r3, r2
	movs r1, #2
	strb r3, [r0]
	movs r0, #11
	bl ObjectMotion_SetActionVariant
	movs r0, #11
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	mov r2, r9
	orrs r2, r3
	movs r3, #3
	strb r2, [r0]
	strb r3, [r7]
	mov r0, r8
	mov r9, r2
	bl Scheduler_RemoveCallbackFar
	movs r0, #1
	bl WaitFrames
	bl Func_020039e0
	movs r0, #153
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_020098a4:
	.4byte gPartyState
.L_020098a8:
	.4byte Func_020016c8
	.section .text.x020098ac,"ax",%progbits
	.global Func_020018ac
	.thumb_func
Func_020018ac:
	push {lr}
	movs r0, #16
	bl Object_GetById
	bl Func_0200142c
	pop {pc}
	.2byte 0x0000
	.section .text.x020098bc,"ax",%progbits
	.global Func_020018bc
	.thumb_func
Func_020018bc:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r8
	push {r5, r6}
	ldr r6, .L_020099d4
	movs r2, #133
	lsls r2, r2, #2
	adds r6, r6, r2
	ldr r0, [r6]
	sub sp, #8
	bl Object_GetById
	mov r10, r0
	movs r0, #16
	bl Object_GetById
	mov r8, r0
	bl Func_020039d8
	movs r0, #0
	bl Func_02003b08
	movs r1, #204
	movs r2, #180
	ldr r0, [r6]
	lsls r2, r2, #1
	lsls r1, r1, #1
	bl ObjectMotion_SetPositionAndReset
	mov r0, r10
	mov r1, r8
	bl Func_02001340
	movs r3, #22
	str r3, [sp, #4]
	movs r5, #24
	movs r0, #0
	movs r1, #0
	movs r2, #3
	movs r3, #1
	str r5, [sp, #0]
	bl Func_020039a8
	movs r1, #0
	movs r2, #2
	movs r3, #1
	movs r0, #40
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl Func_020039a8
	movs r0, #1
	bl WaitFrames
	mov r2, r10
	adds r2, #85
	movs r3, #3
	strb r3, [r2]
	movs r0, #16
	movs r1, #3
	bl ObjectMotion_SetActionVariant
	movs r3, #35
	add r8, r3
	mov r3, r8
	ldrb r2, [r3]
	ldr r5, .L_020099d8
	movs r3, #2
	orrs r3, r2
	movs r1, #144
	mov r2, r8
	strb r3, [r2]
	adds r0, r5, #0
	lsls r1, r1, #3
	bl Scheduler_AddOrUpdateCallback
	mov r0, r10
	movs r1, #2
	bl Func_02001658
	ldr r0, [r6]
	bl Object_GetById
	movs r1, #1
	bl ObjectDispatch_SetSingleChildField26
	movs r1, #153
	movs r2, #200
	lsls r1, r1, #8
	lsls r2, r2, #5
	ldr r0, [r6]
	adds r1, #153
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r1, #203
	movs r2, #180
	ldr r0, [r6]
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndCommit
	movs r1, #200
	movs r2, #196
	ldr r0, [r6]
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndCommit
	movs r0, #161
	bl Func_02003b48
	movs r0, #23
	movs r1, #0
	movs r2, #0
	bl Func_02003a30
	movs r1, #0
	movs r2, #0
	movs r0, #24
	bl Func_02003a30
	adds r0, r5, #0
	bl Scheduler_RemoveCallbackFar
	movs r0, #1
	bl WaitFrames
	bl Func_020039e0
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #50
	bl GameFlag_SetBit
	add sp, #8
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, pc}
.L_020099d4:
	.4byte gPartyState
.L_020099d8:
	.4byte Func_020018ac
	.section .text.x020099dc,"ax",%progbits
	.global Func_020019dc
	.thumb_func
Func_020019dc:
	push {lr}
	sub sp, #8
	movs r3, #1
	movs r2, #2
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #42
	movs r1, #1
	movs r2, #15
	movs r3, #14
	bl Func_02003998
	movs r3, #15
	movs r2, #14
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #0
	movs r2, #1
	movs r3, #1
	bl Func_020039a8
	add sp, #8
	pop {pc}
	.section .text.x02009a0c,"ax",%progbits
	.global Func_02001a0c
	.thumb_func
Func_02001a0c:
	push {lr}
	sub sp, #8
	movs r3, #1
	movs r2, #2
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #15
	movs r1, #9
	movs r2, #15
	movs r3, #14
	bl Func_02003998
	movs r3, #15
	movs r2, #14
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #14
	movs r1, #14
	movs r2, #1
	movs r3, #1
	bl Func_020039a8
	add sp, #8
	pop {pc}
	.section .text.x02009a3c,"ax",%progbits
	.global Func_02001a3c
	.thumb_func
Func_02001a3c:
	push {r5, lr}
	bl Func_020039d8
	movs r0, #0
	bl Func_02003b08
	ldr r0, .L_02009d10
	bl Func_02003a70
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #12
	movs r1, #0
	bl Func_02003a80
	ldr r5, .L_02009d14
	movs r3, #133
	lsls r3, r3, #2
	movs r1, #204
	movs r2, #204
	adds r5, r5, r3
	lsls r1, r1, #8
	lsls r2, r2, #7
	ldr r0, [r5]
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #14
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #136
	movs r2, #168
	ldr r0, [r5]
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #192
	ldr r0, [r5]
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #136
	movs r2, #168
	lsls r1, r1, #17
	lsls r2, r2, #17
	movs r0, #14
	bl Func_02003a30
	movs r0, #1
	bl WaitFrames
	movs r1, #128
	movs r2, #168
	movs r0, #14
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r0, #14
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	ldr r0, [r5]
	lsls r1, r1, #8
	movs r2, #40
	bl ObjectMotion_ArmCallback
	movs r1, #208
	movs r0, #14
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	ldr r0, [r5]
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r2, r2
	movs r3, #0
	negs r0, r0
	negs r1, r1
	bl Motion_CamBounds
	movs r1, #153
	lsls r1, r1, #8
	ldr r0, .L_02009d18
	adds r1, #153
	bl Func_02003ab8
	movs r0, #144
	movs r1, #1
	movs r2, #252
	movs r3, #1
	lsls r0, r0, #17
	negs r1, r1
	lsls r2, r2, #15
	bl Motion_CamBounds
	bl Func_02003ac8
	movs r2, #20
	movs r0, #13
	movs r1, #4
	bl ObjectMotion_Launch
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #13
	movs r1, #0
	bl Func_02003a80
	movs r1, #3
	movs r0, #12
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #12
	movs r1, #2
	bl Motion_SetVarCbAndRefresh
	movs r2, #80
	movs r0, #12
	movs r1, #6
	bl ObjectMotion_Launch
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #13
	movs r1, #0
	bl Func_02003a80
	movs r0, #12
	movs r1, #4
	bl Object_SetModeById
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #12
	movs r1, #0
	bl Func_02003a80
	movs r1, #192
	movs r0, #13
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #10
	movs r2, #20
	adds r1, #255
	movs r0, #13
	bl Func_02003aa8
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #13
	movs r1, #0
	bl Func_02003a80
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #12
	movs r1, #0
	bl Func_02003a80
	movs r2, #0
	movs r0, #13
	movs r1, #0
	bl ObjectMotion_ArmCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #13
	bl Func_02003ab0
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #13
	movs r1, #0
	bl Func_02003a80
	movs r0, #12
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #12
	movs r1, #0
	bl Func_02003a80
	movs r1, #204
	lsls r1, r1, #7
	ldr r0, .L_02009d1c
	adds r1, #102
	bl Func_02003ab8
	movs r0, #210
	movs r1, #1
	movs r2, #200
	movs r3, #1
	lsls r0, r0, #17
	negs r1, r1
	lsls r2, r2, #16
	bl Motion_CamBounds
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #12
	ldr r1, .L_02009d20
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r1, #173
	movs r0, #12
	lsls r1, r1, #1
	movs r2, #186
	bl ObjectMotion_SetPositionAndReset
	movs r1, #187
	movs r0, #12
	lsls r1, r1, #1
	movs r2, #192
	bl ObjectMotion_SetPositionAndReset
	movs r1, #196
	movs r0, #12
	lsls r1, r1, #1
	movs r2, #224
	bl ObjectMotion_SetPositionAndReset
	movs r1, #197
	movs r0, #12
	lsls r1, r1, #1
	movs r2, #246
	bl ObjectMotion_SetPositionAndReset
	movs r1, #210
	movs r2, #134
	movs r0, #12
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #228
	movs r2, #134
	movs r0, #12
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #244
	movs r0, #12
	lsls r1, r1, #1
	movs r2, #240
	bl ObjectMotion_SetPositionAndReset
	movs r1, #244
	movs r0, #12
	lsls r1, r1, #1
	movs r2, #226
	bl ObjectMotion_SetPositionAndReset
	movs r2, #0
	movs r0, #12
	movs r1, #0
	bl Func_02003a30
	movs r1, #153
	lsls r1, r1, #8
	ldr r0, .L_02009d18
	adds r1, #153
	bl Func_02003ab8
	movs r0, #132
	movs r1, #1
	movs r2, #160
	movs r3, #1
	lsls r0, r0, #17
	negs r1, r1
	lsls r2, r2, #17
	bl Motion_CamBounds
	bl Func_02003ac8
	movs r1, #128
	ldr r0, [r5]
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r2, #20
	movs r0, #14
	movs r1, #0
	bl ObjectMotion_ArmCallback
	ldr r0, [r5]
	movs r1, #3
	bl Object_SetModeById
	movs r0, #14
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #14
	movs r1, #2
	bl Object_SetModeById
	ldr r0, [r5]
	bl Object_GetById
	cmp r0, #0
	beq .L_02009ce6
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #14
	bl ObjectMotion_ResetAndSetPosition
.L_02009ce6:
	movs r0, #14
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #14
	movs r1, #0
	movs r2, #0
	bl Func_02003a30
	movs r1, #0
	movs r2, #0
	movs r0, #13
	bl Func_02003a30
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #114
	bl GameFlag_SetBit
	bl Func_020039e0
	pop {r5, pc}
.L_02009d10:
	.4byte MsgFieldRopeThrownAgain
.L_02009d14:
	.4byte gPartyState
.L_02009d18:
	.4byte 0x0004cccc
.L_02009d1c:
	.4byte 0x00033333
.L_02009d20:
	.4byte 0x00019999
	.section .text.x02009d24,"ax",%progbits
	.global Func_02001d24
	.thumb_func
Func_02001d24:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #115
	bl GameFlag_Test
	adds r6, r0, #0
	cmp r6, #0
	beq .L_02009d40
	b .L_0200a372
.L_02009d40:
	bl Func_020039d8
	movs r0, #0
	bl Func_02003b08
	bl Func_02003ad0
	adds r0, #85
	strb r6, [r0]
	movs r1, #1
	movs r0, #170
	movs r2, #176
	movs r3, #1
	lsls r0, r0, #17
	negs r1, r1
	lsls r2, r2, #16
	bl Motion_CamBounds
	ldr r1, .L_0200a160
	movs r2, #133
	lsls r2, r2, #2
	adds r1, r1, r2
	mov r9, r1
	ldr r0, [r1]
	movs r2, #204
	movs r1, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #14
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	mov r3, r9
	movs r1, #172
	ldr r0, [r3]
	lsls r1, r1, #1
	movs r2, #184
	bl ObjectMotion_SetPositionAndReset
	mov r1, r9
	ldr r0, [r1]
	movs r1, #128
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #172
	movs r2, #184
	lsls r1, r1, #17
	lsls r2, r2, #16
	movs r0, #14
	bl Func_02003a30
	movs r0, #1
	bl WaitFrames
	movs r1, #178
	movs r2, #196
	movs r0, #14
	lsls r1, r1, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #176
	lsls r1, r1, #8
	movs r0, #14
	bl Func_02003a90
	ldr r0, .L_0200a164
	bl Func_02003a70
	movs r0, #14
	movs r1, #0
	bl Func_02003a80
	mov r2, r9
	ldr r0, [r2]
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #13
	movs r1, #0
	bl Func_02003a80
	movs r1, #184
	movs r2, #168
	movs r0, #13
	lsls r1, r1, #16
	lsls r2, r2, #16
	bl Func_02003a30
	mov r3, r9
	movs r1, #128
	ldr r0, [r3]
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r2, #0
	movs r0, #14
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r1, #153
	lsls r1, r1, #8
	ldr r0, .L_0200a168
	adds r1, #153
	bl Func_02003ab8
	movs r0, #232
	movs r1, #128
	movs r2, #176
	movs r3, #1
	lsls r0, r0, #16
	lsls r1, r1, #15
	lsls r2, r2, #16
	bl Motion_CamBounds
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #13
	ldr r1, .L_0200a16c
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r2, #168
	movs r1, #232
	movs r0, #13
	bl ObjectMotion_SetPositionAndReset
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #13
	movs r1, #0
	bl Func_02003a80
	movs r1, #152
	lsls r1, r1, #6
	ldr r0, .L_0200a170
	adds r1, #102
	bl Func_02003ab8
	movs r0, #174
	movs r1, #1
	movs r2, #216
	movs r3, #1
	lsls r2, r2, #16
	negs r1, r1
	lsls r0, r0, #17
	bl Motion_CamBounds
	movs r0, #13
	bl Object_GetById
	adds r5, r0, #0
	ldr r1, [r5, #80]
	movs r0, #13
	mov r8, r1
	movs r1, #1
	bl ObjectMotion_SetActionVariant
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #13
	ldr r1, .L_0200a16c
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r0, #13
	movs r1, #4
	movs r2, #0
	bl ObjectMotion_Launch
	movs r2, #168
	movs r0, #13
	movs r1, #248
	bl ObjectMotion_SetPositionAndCommit
	movs r0, #13
	movs r1, #6
	bl Object_SetModeById
	movs r2, #208
	ldr r3, .L_0200a174
	lsls r2, r2, #8
	mov r10, r2
	adds r7, r5, #0
	mov r2, r8
	mov r1, r10
	adds r7, #85
	str r3, [r5, #24]
	strh r1, [r2, #18]
	strb r3, [r7]
	movs r1, #192
	ldr r3, [r5, #12]
	lsls r1, r1, #11
	adds r3, r3, r1
	str r3, [r5, #12]
	movs r0, #13
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #7
	lsls r2, r2, #6
	movs r0, #13
	adds r1, #102
	adds r2, #51
	bl ObjectMotion_SetSpeedParameters
	movs r1, #164
	movs r2, #152
	movs r3, #168
	adds r0, r5, #0
	lsls r1, r1, #17
	lsls r2, r2, #14
	lsls r3, r3, #16
	bl Func_02003990
	movs r0, #13
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r3, #3
	strb r3, [r7]
	movs r3, #128
	lsls r3, r3, #14
	str r3, [r5, #20]
	movs r3, #128
	lsls r3, r3, #9
	mov r2, r8
	str r3, [r5, #24]
	movs r1, #1
	strh r6, [r2, #18]
	movs r0, #13
	bl Object_SetModeById
	movs r0, #13
	bl Object_GetById
	movs r1, #1
	bl ObjectDispatch_SetSingleChildField26
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #13
	ldr r1, .L_0200a16c
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r0, #13
	movs r1, #4
	movs r2, #0
	bl ObjectMotion_Launch
	movs r1, #164
	movs r0, #13
	lsls r1, r1, #1
	movs r2, #184
	bl ObjectMotion_SetPositionAndCommit
	movs r0, #13
	movs r1, #0
	movs r2, #20
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r2, #40
	lsls r1, r1, #1
	movs r0, #13
	bl Func_02003aa8
	movs r0, #13
	movs r1, #2
	bl ObjectMotion_SetActionVariant
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #13
	movs r1, #0
	bl Func_02003a80
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #14
	movs r1, #0
	bl Func_02003a80
	movs r1, #2
	movs r2, #20
	adds r1, #255
	movs r0, #13
	bl Func_02003aa8
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #13
	movs r1, #0
	bl Func_02003a80
	movs r1, #176
	movs r0, #14
	lsls r1, r1, #8
	movs r2, #40
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #14
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #128
	lsls r0, r0, #6
	movs r2, #60
	adds r0, #14
	movs r1, #0
	bl Func_02003a78
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #13
	movs r1, #0
	bl Func_02003a80
	movs r0, #13
	movs r1, #3
	bl Object_SetModeById
	movs r1, #197
	movs r2, #143
	lsls r1, r1, #17
	lsls r2, r2, #17
	movs r0, #12
	bl Func_02003a30
	movs r0, #1
	bl WaitFrames
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #12
	ldr r1, .L_0200a16c
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r1, #197
	movs r0, #12
	lsls r1, r1, #1
	movs r2, #248
	bl ObjectMotion_SetPositionAndReset
	movs r1, #176
	movs r0, #12
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r2, #20
	movs r0, #12
	movs r1, #4
	bl ObjectMotion_Launch
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #12
	movs r1, #0
	bl Func_02003a80
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #13
	bl Func_02003aa8
	movs r1, #192
	movs r0, #13
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	mov r3, r9
	movs r1, #128
	ldr r0, [r3]
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r2, #0
	movs r0, #14
	lsls r1, r1, #6
	bl ObjectMotion_ArmCallback
	movs r1, #153
	lsls r1, r1, #8
	ldr r0, .L_0200a168
	adds r1, #153
	bl Func_02003ab8
	movs r0, #188
	movs r1, #1
	movs r2, #132
	lsls r2, r2, #17
	movs r3, #1
	lsls r0, r0, #17
	negs r1, r1
	bl Motion_CamBounds
	bl Func_02003ac8
	movs r0, #12
	movs r1, #0
	bl Func_02003a80
	movs r1, #152
	lsls r1, r1, #7
	ldr r0, .L_0200a178
	adds r1, #204
	bl Func_02003ab8
	movs r0, #174
	movs r1, #1
	movs r2, #216
	movs r3, #1
	lsls r2, r2, #16
	lsls r0, r0, #17
	negs r1, r1
	bl Motion_CamBounds
	mov r1, r9
	ldr r0, [r1]
	movs r1, #12
	bl Object_LinkObjectAndSetCallback
	movs r0, #14
	movs r1, #12
	bl Object_LinkObjectAndSetCallback
	movs r0, #13
	movs r1, #12
	bl Object_LinkObjectAndSetCallback
	movs r1, #197
	movs r0, #12
	lsls r1, r1, #1
	movs r2, #220
	bl ObjectMotion_SetPositionAndReset
	movs r1, #185
	movs r0, #12
	lsls r1, r1, #1
	movs r2, #198
	bl ObjectMotion_SetPositionAndReset
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #14
	ldr r1, .L_0200a16c
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r1, #184
	movs r2, #180
	movs r0, #14
	lsls r1, r1, #1
	bl ObjectMotion_SetPositionAndCommit
	movs r0, #14
	movs r1, #12
	bl Object_LinkObjectAndSetCallback
	movs r1, #168
	movs r0, #12
	lsls r1, r1, #1
	movs r2, #198
	bl ObjectMotion_SetPositionAndReset
	movs r1, #176
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #12
	bl ObjectMotion_ArmCallback
	movs r0, #13
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #13
	movs r1, #0
	bl Func_02003a80
	movs r2, #0
	movs r0, #12
	mov r1, r10
	bl ObjectMotion_ArmCallback
	movs r0, #12
	movs r1, #0
	bl Func_02003a80
	movs r1, #176
	movs r0, #12
	lsls r1, r1, #8
	bl Func_02003a90
	movs r0, #12
	movs r1, #4
	movs r2, #20
	bl ObjectMotion_Launch
	movs r0, #12
	movs r1, #0
	movs r2, #20
	b .L_0200a17c
.L_0200a160:
	.4byte gPartyState
.L_0200a164:
	.4byte MsgFieldPraiseRopeWork
.L_0200a168:
	.4byte 0x0004cccc
.L_0200a16c:
	.4byte 0x00019999
.L_0200a170:
	.4byte 0x00013333
.L_0200a174:
	.4byte 0xffff0000
.L_0200a178:
	.4byte 0x00026666
.L_0200a17c:
	bl Func_02003a78
	movs r1, #6
	adds r1, #255
	movs r2, #40
	movs r0, #13
	bl Func_02003aa8
	movs r1, #2
	movs r2, #60
	adds r1, #255
	movs r0, #12
	bl Func_02003aa8
	movs r0, #12
	movs r1, #0
	bl Func_02003a80
	movs r0, #13
	movs r1, #4
	bl Object_SetModeById
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #13
	movs r1, #0
	bl Func_02003a80
	movs r1, #128
	movs r2, #20
	lsls r1, r1, #1
	movs r0, #12
	bl Func_02003aa8
	movs r0, #12
	movs r1, #0
	bl Func_02003a80
	movs r1, #128
	movs r2, #20
	movs r0, #13
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #13
	movs r1, #0
	bl Func_02003a80
	movs r1, #128
	movs r2, #0
	movs r0, #12
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r0, #12
	movs r1, #0
	bl Func_02003a80
	movs r0, #13
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #13
	movs r1, #0
	bl Func_02003a80
	movs r1, #10
	movs r2, #40
	adds r1, #255
	movs r0, #12
	bl Func_02003aa8
	movs r0, #12
	movs r1, #0
	bl Func_02003a80
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #12
	bl Func_02003aa8
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #13
	bl Func_02003aa8
	movs r1, #176
	movs r0, #12
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #13
	lsls r1, r1, #6
	movs r2, #40
	bl ObjectMotion_ArmCallback
	movs r2, #40
	movs r0, #13
	movs r1, #4
	bl ObjectMotion_Launch
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #13
	movs r1, #0
	bl Func_02003a80
	movs r0, #12
	movs r1, #3
	bl Object_SetModeById
	movs r0, #12
	movs r1, #0
	bl Func_02003a80
	movs r0, #13
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #13
	movs r1, #0
	bl Func_02003a80
	movs r0, #12
	movs r1, #0
	bl Func_02003a80
	movs r0, #12
	movs r1, #3
	bl Object_SetModeById
	movs r0, #13
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #12
	ldr r1, .L_0200a37c
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r2, #153
	lsls r2, r2, #8
	adds r2, #153
	movs r0, #13
	ldr r1, .L_0200a37c
	bl ObjectMotion_SetSpeedParameters
	ldr r5, .L_0200a380
	movs r0, #12
	adds r1, r5, #0
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #10
	bl Battle_WaitMode0
	adds r1, r5, #0
	movs r0, #13
	bl Object_SetActionCallbackAndRefreshById
	movs r0, #40
	bl Battle_WaitMode0
	mov r2, r9
	ldr r0, [r2]
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #14
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #1
	bl WaitFrames
	movs r0, #14
	movs r1, #0
	movs r2, #40
	bl Func_02003a78
	movs r0, #13
	movs r1, #0
	movs r2, #0
	bl Func_02003a30
	movs r2, #0
	movs r0, #12
	movs r1, #0
	bl Func_02003a30
	movs r0, #14
	movs r1, #0
	bl Func_02003a80
	mov r3, r9
	ldr r0, [r3]
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r2, #20
	movs r0, #14
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r0, #14
	movs r1, #0
	bl Func_02003a80
	movs r0, #14
	movs r1, #2
	bl Object_SetModeById
	mov r1, r9
	ldr r0, [r1]
	bl Object_GetById
	cmp r0, #0
	beq .L_0200a354
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #14
	bl ObjectMotion_ResetAndSetPosition
.L_0200a354:
	movs r0, #14
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #14
	movs r1, #0
	movs r2, #0
	bl Func_02003a30
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #115
	bl GameFlag_SetBit
	bl Func_020039e0
.L_0200a372:
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_0200a37c:
	.4byte 0x00013333
.L_0200a380:
	.4byte Data_02003c58
	.section .text.x0200a384,"ax",%progbits
	.global Func_02002384
	.thumb_func
Func_02002384:
	push {r5, r6, lr}
	bl Func_020039d8
	movs r0, #0
	bl Func_02003b08
	bl Func_02003ad0
	movs r6, #0
	adds r0, #85
	strb r6, [r0]
	movs r1, #128
	movs r0, #240
	movs r2, #170
	lsls r0, r0, #17
	lsls r1, r1, #14
	lsls r2, r2, #17
	movs r3, #1
	bl Motion_CamBounds
	bl Func_02003ac8
	ldr r5, .L_0200a428
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	ldr r1, [r5]
	movs r0, #15
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r1, #128
	movs r2, #40
	lsls r1, r1, #1
	movs r0, #15
	bl Func_02003aa8
	movs r1, #1
	movs r0, #15
	bl ObjectMotion_SetActionVariant
	movs r0, #15
	bl Object_GetById
	movs r2, #204
	adds r0, #85
	lsls r2, r2, #8
	strb r6, [r0]
	ldr r1, .L_0200a42c
	movs r0, #15
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r1, #250
	movs r2, #172
	movs r0, #15
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #145
	movs r2, #172
	lsls r2, r2, #1
	movs r0, #15
	lsls r1, r1, #2
	bl ObjectMotion_SetPositionAndReset
	ldr r1, [r5]
	movs r0, #15
	bl Object_LinkObjectAndSetCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #116
	bl GameFlag_SetBit
	bl Func_020039e0
	pop {r5, r6, pc}
.L_0200a428:
	.4byte gPartyState
.L_0200a42c:
	.4byte 0x00019999
	.section .text.x0200a430,"ax",%progbits
	.global Func_02002430
	.thumb_func
Func_02002430:
	push {r5, lr}
	bl Func_020039d8
	movs r0, #0
	bl Func_02003b08
	movs r0, #15
	bl ObjectMotion_EnableActionAndResetMotion
	bl Func_02003ad0
	movs r5, #0
	adds r0, #85
	strb r5, [r0]
	movs r1, #128
	movs r0, #141
	movs r2, #172
	lsls r0, r0, #18
	lsls r1, r1, #14
	lsls r2, r2, #17
	movs r3, #1
	bl Motion_CamBounds
	bl Func_02003ac8
	ldr r3, .L_0200a4dc
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r1, [r3]
	movs r0, #15
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r1, #128
	movs r2, #40
	lsls r1, r1, #1
	movs r0, #15
	bl Func_02003aa8
	movs r1, #1
	movs r0, #15
	bl ObjectMotion_SetActionVariant
	movs r0, #15
	bl Object_GetById
	movs r2, #204
	adds r0, #85
	lsls r2, r2, #8
	strb r5, [r0]
	ldr r1, .L_0200a4e0
	movs r0, #15
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r1, #154
	movs r2, #188
	movs r0, #15
	lsls r1, r1, #2
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #154
	movs r2, #230
	movs r0, #15
	lsls r1, r1, #2
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #0
	movs r2, #0
	movs r0, #15
	bl Func_02003a30
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #117
	bl GameFlag_SetBit
	bl Func_020039e0
	pop {r5, pc}
.L_0200a4dc:
	.4byte gPartyState
.L_0200a4e0:
	.4byte 0x00019999
	.section .text.x0200a4e4,"ax",%progbits
	.global Func_020024e4
	.thumb_func
Func_020024e4:
	push {r5, r6, lr}
	movs r0, #13
	bl Object_GetById
	adds r6, r0, #0
	bl Func_020039d8
	movs r0, #0
	bl Func_02003b08
	ldr r5, .L_0200a5f4
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	ldr r0, [r5]
	bl Func_02003aa8
	bl Func_02003ad0
	movs r3, #0
	adds r0, #85
	strb r3, [r0]
	movs r1, #128
	movs r0, #164
	movs r2, #138
	lsls r0, r0, #17
	lsls r1, r1, #14
	lsls r2, r2, #18
	movs r3, #1
	bl Motion_CamBounds
	bl Func_02003ac8
	movs r3, #192
	movs r1, #164
	movs r2, #156
	lsls r3, r3, #8
	lsls r2, r2, #18
	lsls r1, r1, #17
	movs r0, #14
	bl Func_02003a38
	movs r0, #1
	bl WaitFrames
	adds r6, #35
	ldr r0, [r5]
	movs r1, #14
	bl Object_LinkObjectAndSetCallback
	ldrb r2, [r6]
	movs r3, #253
	ands r3, r2
	strb r3, [r6]
	movs r0, #14
	movs r1, #3
	bl ObjectMotion_SetActionVariant
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #14
	ldr r1, .L_0200a5f8
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r1, #164
	movs r2, #142
	movs r0, #14
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_SetPositionAndReset
	movs r1, #164
	movs r2, #248
	movs r0, #14
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #140
	movs r2, #216
	lsls r1, r1, #1
	lsls r2, r2, #1
	movs r0, #14
	bl ObjectMotion_SetPositionAndReset
	ldr r0, [r5]
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #1
	bl WaitFrames
	movs r3, #128
	movs r1, #140
	movs r2, #164
	lsls r3, r3, #7
	lsls r2, r2, #17
	movs r0, #14
	lsls r1, r1, #17
	bl Func_02003a38
	movs r1, #2
	movs r0, #14
	bl ObjectMotion_SetActionVariant
	movs r0, #14
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	strb r3, [r0]
	movs r3, #2
	ldrb r2, [r6]
	movs r0, #1
	orrs r3, r2
	strb r3, [r6]
	bl WaitFrames
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #118
	bl GameFlag_SetBit
	movs r0, #239
	lsls r0, r0, #3
	adds r0, #255
	bl GameFlag_SetBit
	bl Func_020039e0
	pop {r5, r6, pc}
.L_0200a5f4:
	.4byte gPartyState
.L_0200a5f8:
	.4byte 0x00019999
	.section .text.x0200a5fc,"ax",%progbits
	.global Func_020025fc
	.thumb_func
Func_020025fc:
	push {r5, r6, lr}
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #118
	bl GameFlag_Test
	adds r6, r0, #0
	cmp r6, #0
	bne .L_0200a6ec
	bl Func_020039d8
	movs r0, #0
	bl Func_02003b08
	ldr r5, .L_0200a6f0
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	ldr r0, [r5]
	bl Func_02003aa8
	bl Func_02003ad0
	adds r0, #85
	strb r6, [r0]
	movs r1, #192
	movs r0, #164
	movs r2, #138
	lsls r0, r0, #17
	lsls r1, r1, #14
	lsls r2, r2, #18
	movs r3, #1
	bl Motion_CamBounds
	bl Func_02003ac8
	movs r3, #128
	movs r1, #212
	movs r2, #130
	lsls r3, r3, #7
	lsls r2, r2, #18
	lsls r1, r1, #17
	movs r0, #14
	bl Func_02003a38
	movs r0, #1
	bl WaitFrames
	ldr r0, [r5]
	movs r1, #14
	bl Object_LinkObjectAndSetCallback
	movs r0, #14
	movs r1, #1
	bl ObjectMotion_SetActionVariant
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #14
	ldr r1, .L_0200a6f4
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r1, #190
	movs r2, #138
	movs r0, #14
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_SetPositionAndReset
	movs r1, #132
	movs r2, #138
	movs r0, #14
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_SetPositionAndReset
	movs r1, #132
	movs r2, #135
	lsls r1, r1, #1
	lsls r2, r2, #2
	movs r0, #14
	bl ObjectMotion_SetPositionAndReset
	ldr r0, [r5]
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #1
	bl WaitFrames
	movs r2, #0
	movs r0, #14
	movs r1, #0
	bl Func_02003a30
	movs r1, #2
	movs r0, #14
	bl ObjectMotion_SetActionVariant
	movs r0, #14
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	strb r3, [r0]
	movs r0, #1
	bl WaitFrames
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #118
	bl GameFlag_SetBit
	bl Func_020039e0
.L_0200a6ec:
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200a6f0:
	.4byte gPartyState
.L_0200a6f4:
	.4byte 0x00019999
	.section .text.x0200a6f8,"ax",%progbits
	.global Func_020026f8
	.thumb_func
Func_020026f8:
	push {r5, lr}
	movs r0, #239
	lsls r0, r0, #3
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a792
	bl Func_020039d8
	movs r0, #0
	bl Func_02003b08
	bl Func_02003ad0
	movs r5, #0
	adds r0, #85
	strb r5, [r0]
	movs r1, #1
	movs r0, #160
	movs r2, #150
	movs r3, #1
	lsls r0, r0, #17
	negs r1, r1
	lsls r2, r2, #17
	bl Motion_CamBounds
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #40
	movs r0, #14
	bl Func_02003aa8
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #14
	ldr r1, .L_0200a794
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r1, #90
	movs r2, #148
	movs r0, #14
	adds r1, #255
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #196
	movs r2, #148
	movs r0, #14
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	ldr r3, .L_0200a798
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r1, [r3]
	movs r0, #14
	bl Object_LinkObjectAndSetCallback
	movs r0, #14
	bl Object_GetById
	adds r0, #98
	strb r5, [r0]
	movs r0, #10
	bl WaitFrames
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #124
	bl GameFlag_SetBit
	bl Func_020039e0
.L_0200a792:
	pop {r5, pc}
.L_0200a794:
	.4byte 0x00013333
.L_0200a798:
	.4byte gPartyState
	.section .text.x0200a79c,"ax",%progbits
	.global Func_0200279c
	.thumb_func
Func_0200279c:
	push {lr}
	bl Func_020039d8
	movs r0, #0
	bl Func_02003b08
	bl Func_02003ad0
	movs r3, #0
	adds r0, #85
	strb r3, [r0]
	movs r0, #14
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #1
	bl WaitFrames
	movs r0, #14
	ldr r1, .L_0200a7cc
	ldr r2, .L_0200a7d0
	bl ObjectMotion_SetSpeedParameters
	pop {pc}
	.2byte 0x0000
.L_0200a7cc:
	.4byte 0x00026666
.L_0200a7d0:
	.4byte 0x00013333
	.section .text.x0200a7d4,"ax",%progbits
	.global Func_020027d4
	.thumb_func
Func_020027d4:
	push {lr}
	ldr r3, .L_0200a7f4
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r1, [r3]
	movs r0, #14
	bl Object_LinkObjectAndSetCallback
	movs r0, #5
	bl Battle_WaitMode0
	bl Func_020039e0
	pop {pc}
	.2byte 0x0000
.L_0200a7f4:
	.4byte gPartyState
	.section .text.x0200a7f8,"ax",%progbits
	.global Func_020027f8
	.thumb_func
Func_020027f8:
	push {lr}
	bl Func_0200279c
	movs r0, #196
	movs r1, #128
	movs r2, #216
	movs r3, #1
	lsls r0, r0, #17
	lsls r1, r1, #13
	lsls r2, r2, #16
	bl Motion_CamBounds
	movs r1, #196
	lsls r1, r1, #1
	movs r2, #216
	movs r0, #14
	bl ObjectMotion_SetPositionAndReset
	movs r0, #14
	bl Object_GetById
	movs r3, #1
	adds r0, #98
	strb r3, [r0]
	bl Func_020027d4
	pop {pc}
	.2byte 0x0000
	.section .text.x0200a830,"ax",%progbits
	.global Func_02002830
	.thumb_func
Func_02002830:
	push {r5, r6, lr}
	movs r0, #14
	bl Object_GetById
	adds r5, r0, #0
	bl Func_0200279c
	movs r0, #180
	movs r1, #128
	movs r2, #216
	movs r3, #1
	lsls r0, r0, #17
	lsls r1, r1, #13
	lsls r2, r2, #16
	bl Motion_CamBounds
	movs r1, #180
	movs r0, #14
	lsls r1, r1, #1
	movs r2, #216
	bl ObjectMotion_SetPositionAndReset
	movs r0, #132
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	adds r6, r5, #0
	adds r6, #98
	cmp r0, #0
	beq .L_0200a894
	movs r3, #128
	lsls r3, r3, #8
	movs r1, #129
	strh r3, [r5, #6]
	movs r0, #14
	lsls r1, r1, #1
	bl Func_02003ab0
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #128
	movs r3, #4
	lsls r0, r0, #2
	strb r3, [r6]
	adds r0, #6
	bl GameFlag_SetBit
	b .L_0200a90a
.L_0200a894:
	movs r0, #5
	bl Battle_WaitMode0
	movs r0, #148
	movs r1, #128
	movs r2, #216
	movs r3, #1
	lsls r2, r2, #16
	lsls r1, r1, #13
	lsls r0, r0, #17
	bl Motion_CamBounds
	movs r0, #153
	bl Func_02003b48
	movs r0, #14
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #14
	ldr r1, .L_0200a910
	ldr r2, .L_0200a914
	bl ObjectMotion_SetSpeedParameters
	movs r3, #128
	lsls r3, r3, #12
	movs r1, #164
	str r3, [r5, #40]
	movs r2, #216
	lsls r1, r1, #1
	movs r0, #14
	bl ObjectMotion_SetPositionAndCommit
	movs r0, #14
	bl Object_GetById
	movs r1, #1
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #14
	ldr r1, .L_0200a918
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r1, #148
	movs r0, #14
	lsls r1, r1, #1
	movs r2, #216
	bl ObjectMotion_SetPositionAndReset
	movs r3, #2
	strb r3, [r6]
.L_0200a90a:
	bl Func_020027d4
	pop {r5, r6, pc}
.L_0200a910:
	.4byte 0x0004cccc
.L_0200a914:
	.4byte 0x00026666
.L_0200a918:
	.4byte 0x00019999
	.section .text.x0200a91c,"ax",%progbits
	.global Func_0200291c
	.thumb_func
Func_0200291c:
	push {lr}
	bl Func_0200279c
	movs r0, #140
	movs r1, #128
	movs r2, #148
	movs r3, #1
	lsls r0, r0, #17
	lsls r1, r1, #13
	lsls r2, r2, #17
	bl Motion_CamBounds
	movs r1, #148
	movs r0, #14
	lsls r1, r1, #1
	movs r2, #248
	bl ObjectMotion_SetPositionAndReset
	movs r1, #140
	movs r0, #14
	lsls r1, r1, #1
	movs r2, #248
	bl ObjectMotion_SetPositionAndReset
	movs r1, #140
	movs r2, #148
	lsls r1, r1, #1
	lsls r2, r2, #1
	movs r0, #14
	bl ObjectMotion_SetPositionAndReset
	movs r0, #14
	bl Object_GetById
	movs r3, #3
	adds r0, #98
	strb r3, [r0]
	bl Func_020027d4
	pop {pc}
	.section .text.x0200a96c,"ax",%progbits
	.global Func_0200296c
	.thumb_func
Func_0200296c:
	push {lr}
	bl Func_0200279c
	movs r0, #196
	movs r1, #128
	movs r2, #148
	movs r3, #1
	lsls r0, r0, #17
	lsls r1, r1, #13
	lsls r2, r2, #17
	bl Motion_CamBounds
	movs r1, #196
	movs r2, #148
	lsls r1, r1, #1
	lsls r2, r2, #1
	movs r0, #14
	bl ObjectMotion_SetPositionAndReset
	movs r0, #14
	bl Object_GetById
	movs r3, #0
	adds r0, #98
	strb r3, [r0]
	bl Func_020027d4
	pop {pc}
	.section .text.x0200a9a4,"ax",%progbits
	.global Func_020029a4
	.thumb_func
Func_020029a4:
	push {lr}
	bl Func_0200279c
	movs r0, #140
	movs r1, #128
	movs r2, #148
	movs r3, #1
	lsls r0, r0, #17
	lsls r1, r1, #13
	lsls r2, r2, #17
	bl Motion_CamBounds
	movs r1, #140
	movs r2, #148
	lsls r1, r1, #1
	lsls r2, r2, #1
	movs r0, #14
	bl ObjectMotion_SetPositionAndReset
	movs r0, #14
	bl Object_GetById
	movs r3, #3
	adds r0, #98
	strb r3, [r0]
	bl Func_020027d4
	pop {pc}
	.section .text.x0200a9dc,"ax",%progbits
	.global Func_020029dc
	.thumb_func
Func_020029dc:
	push {lr}
	bl Func_0200279c
	movs r0, #148
	movs r1, #128
	movs r2, #216
	movs r3, #1
	lsls r0, r0, #17
	lsls r1, r1, #13
	lsls r2, r2, #16
	bl Motion_CamBounds
	movs r1, #140
	movs r0, #14
	lsls r1, r1, #1
	movs r2, #248
	bl ObjectMotion_SetPositionAndReset
	movs r1, #148
	movs r0, #14
	lsls r1, r1, #1
	movs r2, #248
	bl ObjectMotion_SetPositionAndReset
	movs r1, #148
	lsls r1, r1, #1
	movs r2, #216
	movs r0, #14
	bl ObjectMotion_SetPositionAndReset
	movs r0, #14
	bl Object_GetById
	movs r3, #2
	adds r0, #98
	strb r3, [r0]
	bl Func_020027d4
	pop {pc}
	.2byte 0x0000
	.section .text.x0200aa2c,"ax",%progbits
	.global Func_02002a2c
	.thumb_func
Func_02002a2c:
	push {r5, r6, lr}
	movs r0, #14
	bl Object_GetById
	adds r5, r0, #0
	bl Func_0200279c
	movs r0, #164
	movs r1, #128
	movs r2, #216
	movs r3, #1
	lsls r0, r0, #17
	lsls r1, r1, #13
	lsls r2, r2, #16
	bl Motion_CamBounds
	movs r1, #164
	movs r0, #14
	lsls r1, r1, #1
	movs r2, #216
	bl ObjectMotion_SetPositionAndReset
	movs r0, #132
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	adds r6, r5, #0
	adds r6, #98
	cmp r0, #0
	beq .L_0200aa8e
	movs r3, #0
	movs r1, #129
	strh r3, [r5, #6]
	movs r0, #14
	lsls r1, r1, #1
	bl Func_02003ab0
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #128
	movs r3, #5
	lsls r0, r0, #2
	strb r3, [r6]
	adds r0, #6
	bl GameFlag_SetBit
	b .L_0200ab04
.L_0200aa8e:
	movs r0, #5
	bl Battle_WaitMode0
	movs r0, #196
	movs r1, #128
	movs r2, #216
	movs r3, #1
	lsls r2, r2, #16
	lsls r1, r1, #13
	lsls r0, r0, #17
	bl Motion_CamBounds
	movs r0, #153
	bl Func_02003b48
	movs r0, #14
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #14
	ldr r1, .L_0200ab0c
	ldr r2, .L_0200ab10
	bl ObjectMotion_SetSpeedParameters
	movs r3, #128
	lsls r3, r3, #12
	movs r1, #180
	str r3, [r5, #40]
	movs r2, #216
	lsls r1, r1, #1
	movs r0, #14
	bl ObjectMotion_SetPositionAndCommit
	movs r0, #14
	bl Object_GetById
	movs r1, #1
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #14
	ldr r1, .L_0200ab14
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r1, #196
	movs r0, #14
	lsls r1, r1, #1
	movs r2, #216
	bl ObjectMotion_SetPositionAndReset
	movs r3, #1
	strb r3, [r6]
.L_0200ab04:
	bl Func_020027d4
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200ab0c:
	.4byte 0x0004cccc
.L_0200ab10:
	.4byte 0x00026666
.L_0200ab14:
	.4byte 0x00019999
	.section .text.x0200ab18,"ax",%progbits
	.global Func_02002b18
	.thumb_func
Func_02002b18:
	push {lr}
	bl Func_0200279c
	movs r0, #196
	movs r1, #128
	movs r2, #148
	movs r3, #1
	lsls r0, r0, #17
	lsls r1, r1, #13
	lsls r2, r2, #17
	bl Motion_CamBounds
	movs r1, #196
	movs r2, #148
	lsls r1, r1, #1
	lsls r2, r2, #1
	movs r0, #14
	bl ObjectMotion_SetPositionAndReset
	movs r0, #14
	bl Object_GetById
	movs r3, #0
	adds r0, #98
	strb r3, [r0]
	bl Func_020027d4
	pop {pc}
	.section .text.x0200ab50,"ax",%progbits
	.global Func_02002b50
	.thumb_func
Func_02002b50:
	push {r5, r6, lr}
	movs r0, #14
	bl Object_GetById
	adds r5, r0, #0
	bl Func_0200279c
	movs r0, #132
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	adds r6, r5, #0
	adds r6, #98
	cmp r0, #0
	beq .L_0200ab96
	movs r3, #128
	lsls r3, r3, #8
	movs r1, #129
	strh r3, [r5, #6]
	movs r0, #14
	lsls r1, r1, #1
	bl Func_02003ab0
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #128
	movs r3, #4
	lsls r0, r0, #2
	strb r3, [r6]
	adds r0, #6
	bl GameFlag_SetBit
	b .L_0200ac06
.L_0200ab96:
	movs r0, #148
	movs r1, #128
	movs r2, #216
	movs r3, #1
	lsls r2, r2, #16
	lsls r1, r1, #13
	lsls r0, r0, #17
	bl Motion_CamBounds
	movs r0, #153
	bl Func_02003b48
	movs r0, #14
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #14
	ldr r1, .L_0200ac0c
	ldr r2, .L_0200ac10
	bl ObjectMotion_SetSpeedParameters
	movs r3, #128
	lsls r3, r3, #12
	movs r1, #164
	str r3, [r5, #40]
	movs r2, #216
	lsls r1, r1, #1
	movs r0, #14
	bl ObjectMotion_SetPositionAndCommit
	movs r0, #14
	bl Object_GetById
	movs r1, #1
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #14
	ldr r1, .L_0200ac14
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r1, #148
	movs r0, #14
	lsls r1, r1, #1
	movs r2, #216
	bl ObjectMotion_SetPositionAndReset
	movs r3, #2
	strb r3, [r6]
.L_0200ac06:
	bl Func_020027d4
	pop {r5, r6, pc}
.L_0200ac0c:
	.4byte 0x0004cccc
.L_0200ac10:
	.4byte 0x00026666
.L_0200ac14:
	.4byte 0x00019999
	.section .text.x0200ac18,"ax",%progbits
	.global Func_02002c18
	.thumb_func
Func_02002c18:
	push {lr}
	bl Func_0200279c
	movs r0, #196
	movs r1, #128
	movs r2, #216
	movs r3, #1
	lsls r0, r0, #17
	lsls r1, r1, #13
	lsls r2, r2, #16
	bl Motion_CamBounds
	movs r1, #196
	lsls r1, r1, #1
	movs r2, #216
	movs r0, #14
	bl ObjectMotion_SetPositionAndReset
	movs r0, #14
	bl Object_GetById
	movs r3, #1
	adds r0, #98
	strb r3, [r0]
	bl Func_020027d4
	pop {pc}
	.2byte 0x0000
	.section .text.x0200ac50,"ax",%progbits
	.global Func_02002c50
	.thumb_func
Func_02002c50:
	push {lr}
	bl Func_0200279c
	movs r0, #148
	movs r1, #128
	movs r2, #216
	movs r3, #1
	lsls r0, r0, #17
	lsls r1, r1, #13
	lsls r2, r2, #16
	bl Motion_CamBounds
	movs r1, #148
	lsls r1, r1, #1
	movs r2, #216
	movs r0, #14
	bl ObjectMotion_SetPositionAndReset
	movs r0, #14
	bl Object_GetById
	movs r3, #2
	adds r0, #98
	strb r3, [r0]
	bl Func_020027d4
	pop {pc}
	.2byte 0x0000
	.section .text.x0200ac88,"ax",%progbits
	.global Func_02002c88
	.thumb_func
Func_02002c88:
	push {r5, r6, lr}
	movs r0, #14
	bl Object_GetById
	adds r5, r0, #0
	bl Func_0200279c
	movs r0, #132
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	adds r6, r5, #0
	adds r6, #98
	cmp r0, #0
	beq .L_0200accc
	movs r3, #0
	movs r1, #129
	strh r3, [r5, #6]
	movs r0, #14
	lsls r1, r1, #1
	bl Func_02003ab0
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #128
	movs r3, #5
	lsls r0, r0, #2
	strb r3, [r6]
	adds r0, #6
	bl GameFlag_SetBit
	b .L_0200ad3c
.L_0200accc:
	movs r0, #196
	movs r1, #128
	movs r2, #216
	movs r3, #1
	lsls r2, r2, #16
	lsls r1, r1, #13
	lsls r0, r0, #17
	bl Motion_CamBounds
	movs r0, #153
	bl Func_02003b48
	movs r0, #14
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #14
	ldr r1, .L_0200ad44
	ldr r2, .L_0200ad48
	bl ObjectMotion_SetSpeedParameters
	movs r3, #128
	lsls r3, r3, #12
	movs r1, #180
	str r3, [r5, #40]
	movs r2, #216
	lsls r1, r1, #1
	movs r0, #14
	bl ObjectMotion_SetPositionAndCommit
	movs r0, #14
	bl Object_GetById
	movs r1, #1
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #14
	ldr r1, .L_0200ad4c
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r1, #196
	movs r0, #14
	lsls r1, r1, #1
	movs r2, #216
	bl ObjectMotion_SetPositionAndReset
	movs r3, #1
	strb r3, [r6]
.L_0200ad3c:
	bl Func_020027d4
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200ad44:
	.4byte 0x0004cccc
.L_0200ad48:
	.4byte 0x00026666
.L_0200ad4c:
	.4byte 0x00019999
	.section .text.x0200ad50,"ax",%progbits
	.global Func_02002d50
	.thumb_func
Func_02002d50:
	push {r5, lr}
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #124
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200ad62
	b .L_0200ae6a
.L_0200ad62:
	movs r0, #115
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200ae6a
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #108]
	movs r0, #14
	bl Object_GetById
	movs r2, #170
	lsls r2, r2, #1
	adds r5, r5, r2
	movs r2, #0
	ldrsh r3, [r5, r2]
	subs r3, #13
	cmp r3, #7
	bhi .L_0200ae6a
	ldr r2, .L_0200ae6c
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	adds r0, #98
	mov pc, r3
	.2byte 0x0000
.L_0200ad94:
	.4byte .L_0200adb4
	.4byte .L_0200adca
	.4byte .L_0200add6
	.4byte .L_0200adfa
	.4byte .L_0200ae10
	.4byte .L_0200ae26
	.4byte .L_0200ae4a
	.4byte .L_0200ae56
.L_0200adb4:
	ldrb r3, [r0]
	cmp r3, #0
	bne .L_0200adbe
	bl Func_020027f8
.L_0200adbe:
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #6
	bl GameFlag_ClearBit
	b .L_0200ae6a
.L_0200adca:
	ldrb r3, [r0]
	cmp r3, #0
	bne .L_0200ae6a
	bl Func_020029a4
	b .L_0200ae6a
.L_0200add6:
	ldrb r0, [r0]
	cmp r0, #1
	bne .L_0200ade2
	bl Func_02002830
	b .L_0200ae6a
.L_0200ade2:
	cmp r0, #4
	bne .L_0200ae6a
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #6
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200ae6a
	bl Func_02002b50
	b .L_0200ae6a
.L_0200adfa:
	ldrb r0, [r0]
	cmp r0, #1
	bne .L_0200ae06
	bl Func_02002b18
	b .L_0200ae6a
.L_0200ae06:
	cmp r0, #5
	bne .L_0200ae6a
	bl Func_02002c50
	b .L_0200ae6a
.L_0200ae10:
	ldrb r0, [r0]
	cmp r0, #2
	bne .L_0200ae1c
	bl Func_0200291c
	b .L_0200ae6a
.L_0200ae1c:
	cmp r0, #4
	bne .L_0200ae6a
	bl Func_02002c18
	b .L_0200ae6a
.L_0200ae26:
	ldrb r0, [r0]
	cmp r0, #2
	bne .L_0200ae32
	bl Func_02002a2c
	b .L_0200ae6a
.L_0200ae32:
	cmp r0, #5
	bne .L_0200ae6a
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #6
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200ae6a
	bl Func_02002c88
	b .L_0200ae6a
.L_0200ae4a:
	ldrb r3, [r0]
	cmp r3, #3
	bne .L_0200ae6a
	bl Func_0200296c
	b .L_0200ae6a
.L_0200ae56:
	ldrb r3, [r0]
	cmp r3, #3
	bne .L_0200ae60
	bl Func_020029dc
.L_0200ae60:
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #6
	bl GameFlag_ClearBit
.L_0200ae6a:
	pop {r5, pc}
.L_0200ae6c:
	.4byte .L_0200ad94
	.section .text.x0200ae70,"ax",%progbits
	.global Func_02002e70
	.thumb_func
Func_02002e70:
	push {lr}
	movs r0, #8
	movs r1, #2
	bl Object_SetModeById
	movs r0, #9
	movs r1, #0
	bl Object_SetModeById
	movs r0, #9
	bl Func_02003aa0
	pop {pc}
	.2byte 0x0000
	.section .text.x0200ae8c,"ax",%progbits
	.global Func_02002e8c
	.thumb_func
Func_02002e8c:
	push {lr}
	ldr r3, .L_0200aea4
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	movs r3, #2
	adds r0, #34
	strb r3, [r0]
	pop {pc}
.L_0200aea4:
	.4byte gPartyState
	.section .text.x0200aea8,"ax",%progbits
	.global Func_02002ea8
	.thumb_func
Func_02002ea8:
	push {lr}
	ldr r3, .L_0200aec0
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	movs r3, #0
	adds r0, #34
	strb r3, [r0]
	pop {pc}
.L_0200aec0:
	.4byte gPartyState
	.section .text.x0200aec4,"ax",%progbits
	.global Func_02002ec4
	.thumb_func
Func_02002ec4:
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
	ldr r3, .L_0200af5c
	subs r2, #36
	adds r3, r3, r2
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_0200af60
	cmp r2, r3
	bne .L_0200aeee
	bl Func_02003094
	b .L_0200af58
.L_0200aeee:
	ldr r3, .L_0200af64
	cmp r2, r3
	bne .L_0200aefa
	bl Func_02003134
	b .L_0200af58
.L_0200aefa:
	ldr r3, .L_0200af68
	cmp r2, r3
	bne .L_0200af06
	bl Func_0200320c
	b .L_0200af58
.L_0200af06:
	ldr r3, .L_0200af6c
	cmp r2, r3
	bne .L_0200af12
	bl Func_02003354
	b .L_0200af58
.L_0200af12:
	ldr r3, .L_0200af70
	cmp r2, r3
	bne .L_0200af1e
	bl Func_0200338c
	b .L_0200af58
.L_0200af1e:
	ldr r3, .L_0200af74
	cmp r2, r3
	bne .L_0200af2a
	bl Func_02003504
	b .L_0200af58
.L_0200af2a:
	ldr r3, .L_0200af78
	cmp r2, r3
	bne .L_0200af36
	bl Func_02003598
	b .L_0200af58
.L_0200af36:
	ldr r3, .L_0200af7c
	cmp r2, r3
	bne .L_0200af42
	bl Func_02003740
	b .L_0200af58
.L_0200af42:
	ldr r3, .L_0200af80
	cmp r2, r3
	bne .L_0200af4e
	bl Func_0200377c
	b .L_0200af58
.L_0200af4e:
	ldr r3, .L_0200af84
	cmp r2, r3
	bne .L_0200af58
	bl Func_0200380c
.L_0200af58:
	movs r0, #0
	pop {pc}
.L_0200af5c:
	.4byte gPartyState
.L_0200af60:
	.4byte 0x00000017
.L_0200af64:
	.4byte 0x00000018
.L_0200af68:
	.4byte 0x00000019
.L_0200af6c:
	.4byte 0x0000001a
.L_0200af70:
	.4byte 0x0000001b
.L_0200af74:
	.4byte 0x0000001c
.L_0200af78:
	.4byte 0x0000001d
.L_0200af7c:
	.4byte 0x0000001e
.L_0200af80:
	.4byte 0x0000001f
.L_0200af84:
	.4byte 0x00000021
	.section .text.x0200af88,"ax",%progbits
	.global Func_02002f88
	.thumb_func
Func_02002f88:
	push {r5, r6, lr}
	adds r6, r0, #0
	bl Object_GetById
	adds r5, r0, #0
	adds r1, r5, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #2
	orrs r3, r2
	strb r3, [r1]
	movs r3, #192
	lsls r3, r3, #12
	adds r0, r6, #0
	str r3, [r5, #12]
	bl Func_02003aa0
	adds r0, r5, #0
	pop {r5, r6, pc}
	.2byte 0x0000
	.section .text.x0200afb0,"ax",%progbits
	.global Func_02002fb0
	.thumb_func
Func_02002fb0:
	push {r5, lr}
	adds r5, r0, #0
	bl Func_02003aa0
	adds r0, r5, #0
	bl Object_GetById
	movs r1, #15
	bl Object_SetPartAttribute
	adds r0, r5, #0
	movs r1, #1
	bl ObjectMotion_SetActionVariant
	adds r0, r5, #0
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #2
	orrs r3, r2
	strb r3, [r0]
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x0200afe0,"ax",%progbits
	.global Func_02002fe0
	.thumb_func
Func_02002fe0:
	push {lr}
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200b000
	movs r0, #130
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200b008
	bl Func_0200123c
	b .L_0200b008
.L_0200b000:
	movs r0, #130
	lsls r0, r0, #2
	bl GameFlag_SetBit
.L_0200b008:
	pop {pc}
	.2byte 0x0000
	.section .text.x0200b00c,"ax",%progbits
	.global Func_0200300c
	.thumb_func
Func_0200300c:
	push {r5, lr}
	movs r5, #0
.L_0200b010:
	adds r0, r5, #0
	adds r0, #9
	adds r5, #1
	bl Func_02002fb0
	cmp r5, #2
	bls .L_0200b010
	bl Func_02002fe0
	pop {r5, pc}
	.section .text.x0200b024,"ax",%progbits
	.global Func_02003024
	.thumb_func
Func_02003024:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r7, .L_0200b090
	movs r3, #0
	mov r8, r3
.L_0200b030:
	mov r5, r8
	adds r5, #8
	adds r0, r5, #0
	bl Object_GetById
	adds r6, r0, #0
	adds r0, r5, #0
	bl Func_02003aa0
	adds r0, r5, #0
	movs r1, #0
	bl Object_SetModeById
	adds r0, r5, #0
	movs r1, #3
	bl ObjectMotion_SetActionVariant
	adds r1, r6, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #2
	orrs r3, r2
	strb r3, [r1]
	adds r0, r5, #0
	bl Object_GetById
	movs r1, #12
	bl Object_SetPartAttribute
	ldr r3, [r7]
	ldr r2, [r6, #80]
	str r3, [r6, #8]
	ldr r3, [r7, #4]
	str r3, [r6, #16]
	ldr r3, [r7, #8]
	str r3, [r6, #24]
	ldr r3, [r7, #12]
	adds r7, #16
	strh r3, [r2, #18]
	movs r3, #1
	add r8, r3
	mov r3, r8
	cmp r3, #15
	bls .L_0200b030
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200b090:
	.4byte Data_02003cc0
	.section .text.x0200b094,"ax",%progbits
	.global Func_02003094
	.thumb_func
Func_02003094:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #32]
	movs r0, #8
	bl Func_02003854
	movs r0, #9
	bl Func_02003854
	ldrb r2, [r5, #23]
	movs r3, #13
	negs r3, r3
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	movs r1, #144
	strb r3, [r5, #23]
	lsls r1, r1, #3
	ldr r0, .L_0200b0f8
	bl Scheduler_AddOrUpdateCallback
	bl Func_02003b18
	movs r1, #130
	lsls r1, r1, #1
	movs r0, #0
	adds r1, #255
	movs r2, #10
	movs r3, #11
	bl Func_02003b20
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #114
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200b0f6
	movs r0, #12
	movs r1, #0
	movs r2, #0
	bl Func_02003a30
	movs r0, #13
	movs r1, #0
	movs r2, #0
	bl Func_02003a30
.L_0200b0f6:
	pop {r5, pc}
.L_0200b0f8:
	.4byte Func_020010e8
	.section .text.x0200b0fc,"ax",%progbits
	.global Func_020030fc
	.thumb_func
Func_020030fc:
	push {r5, lr}
	ldr r3, .L_0200b130
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	adds r5, r0, #0
	movs r0, #11
	bl Object_GetById
	ldr r3, [r5, #8]
	ldr r2, [r0, #8]
	adds r0, #89
	cmp r2, r3
	ble .L_0200b126
	ldrb r2, [r0]
	movs r3, #8
	orrs r3, r2
	b .L_0200b12c
.L_0200b126:
	ldrb r2, [r0]
	movs r3, #247
	ands r3, r2
.L_0200b12c:
	strb r3, [r0]
	pop {r5, pc}
.L_0200b130:
	.4byte gPartyState
	.section .text.x0200b134,"ax",%progbits
	.global Func_02003134
	.thumb_func
Func_02003134:
	push {lr}
	movs r0, #10
	sub sp, #8
	bl Func_02002f88
	ldr r3, .L_0200b1f8
	movs r1, #3
	str r3, [r0, #12]
	movs r0, #10
	bl ObjectMotion_SetActionVariant
	ldr r0, .L_0200b1fc
	bl Func_02003b30
	bl Func_02002e70
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_0200b200
	bl Scheduler_AddOrUpdateCallback
	movs r0, #11
	bl Func_02003aa0
	movs r0, #12
	bl Func_02003aa0
	movs r0, #12
	bl Object_GetById
	movs r3, #0
	adds r0, #85
	strb r3, [r0]
	movs r2, #15
	movs r3, #6
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #3
	movs r1, #15
	movs r2, #1
	movs r3, #1
	bl Func_020039a8
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #115
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200b1a4
	movs r0, #11
	movs r1, #0
	movs r2, #0
	bl Func_02003a30
	b .L_0200b1ea
.L_0200b1a4:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #114
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200b1ea
	movs r3, #128
	movs r1, #164
	movs r2, #200
	lsls r3, r3, #8
	lsls r2, r2, #16
	lsls r1, r1, #17
	movs r0, #11
	bl Func_02003a38
	movs r0, #1
	bl WaitFrames
	ldr r1, .L_0200b204
	movs r0, #11
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #11
	bl Object_GetById
	movs r1, #1
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #11
	bl Object_GetById
	movs r3, #1
	adds r0, #89
	strb r3, [r0]
.L_0200b1ea:
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_0200b208
	bl Scheduler_AddOrUpdateCallback
	add sp, #8
	pop {pc}
.L_0200b1f8:
	.4byte 0xfff00000
.L_0200b1fc:
	.4byte Data_02003cbc
.L_0200b200:
	.4byte Func_020011a0
.L_0200b204:
	.4byte Data_02003c10
.L_0200b208:
	.4byte Func_020030fc
	.section .text.x0200b20c,"ax",%progbits
	.global Func_0200320c
	.thumb_func
Func_0200320c:
	push {r5, lr}
	movs r0, #9
	sub sp, #8
	bl Func_02002f88
	movs r0, #10
	bl Func_02002f88
	movs r0, #11
	bl Func_02002f88
	movs r0, #12
	bl Func_02002f88
	movs r0, #13
	bl Func_02002f88
	movs r0, #14
	bl Func_02002f88
	movs r1, #0
	adds r5, r0, #0
	movs r0, #9
	bl Object_SetModeById
	movs r0, #13
	movs r1, #0
	bl Object_SetModeById
	movs r0, #14
	movs r1, #2
	bl Object_SetModeById
	movs r0, #9
	movs r1, #1
	bl ObjectMotion_SetActionVariant
	movs r0, #10
	movs r1, #1
	bl ObjectMotion_SetActionVariant
	movs r0, #11
	movs r1, #1
	bl ObjectMotion_SetActionVariant
	movs r0, #12
	movs r1, #1
	bl ObjectMotion_SetActionVariant
	movs r0, #13
	movs r1, #1
	bl ObjectMotion_SetActionVariant
	movs r0, #14
	movs r1, #1
	bl ObjectMotion_SetActionVariant
	movs r1, #144
	ldr r0, .L_0200b348
	lsls r1, r1, #3
	bl Scheduler_AddOrUpdateCallback
	movs r0, #140
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200b2d4
	movs r1, #204
	movs r3, #168
	adds r0, r5, #0
	lsls r1, r1, #17
	ldr r2, .L_0200b34c
	lsls r3, r3, #17
	bl Object_SetPositionAndResetMotion
	ldr r2, [r5, #80]
	movs r3, #128
	lsls r3, r3, #5
	strh r3, [r2, #18]
	movs r0, #14
	movs r1, #3
	bl ObjectMotion_SetActionVariant
	adds r1, r5, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #2
	orrs r3, r2
	strb r3, [r1]
	movs r2, #70
	movs r3, #24
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #50
	movs r2, #3
	movs r3, #1
	bl Func_020039a8
.L_0200b2d4:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #76
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200b2f6
	movs r0, #8
	bl Object_GetById
	movs r1, #200
	movs r3, #148
	lsls r1, r1, #16
	movs r2, #0
	lsls r3, r3, #17
	bl Object_SetPositionAndResetMotion
.L_0200b2f6:
	ldr r0, .L_0200b350
	bl Func_02003b30
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #117
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200b316
	movs r0, #15
	movs r1, #0
	movs r2, #0
	bl Func_02003a30
	b .L_0200b336
.L_0200b316:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #116
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200b336
	movs r3, #128
	movs r1, #145
	movs r2, #172
	lsls r3, r3, #7
	movs r0, #15
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl Func_02003a38
.L_0200b336:
	movs r0, #0
	bl Func_02003b00
	movs r0, #1
	bl WaitFrames
	add sp, #8
	pop {r5, pc}
	.2byte 0x0000
.L_0200b348:
	.4byte Func_02001088
.L_0200b34c:
	.4byte 0xfff30000
.L_0200b350:
	.4byte Data_02003cbc
	.section .text.x0200b354,"ax",%progbits
	.global Func_02003354
	.thumb_func
Func_02003354:
	push {r5, lr}
	movs r0, #128
	movs r3, #192
	lsls r0, r0, #4
	lsls r3, r3, #18
	adds r0, #118
	ldr r5, [r3, #32]
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200b36e
	bl Func_02003024
.L_0200b36e:
	ldrb r2, [r5, #23]
	movs r3, #13
	negs r3, r3
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	movs r1, #144
	strb r3, [r5, #23]
	lsls r1, r1, #3
	ldr r0, .L_0200b388
	bl Scheduler_AddOrUpdateCallback
	pop {r5, pc}
.L_0200b388:
	.4byte Func_020010e8
	.section .text.x0200b38c,"ax",%progbits
	.global Func_0200338c
	.thumb_func
Func_0200338c:
	push {r5, lr}
	movs r0, #13
	sub sp, #8
	bl Func_02002f88
	ldr r3, .L_0200b4f4
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	ldrh r3, [r3]
	movs r2, #128
	subs r3, #2
	adds r5, r0, #0
	lsls r3, r3, #16
	lsls r2, r2, #9
	adds r5, #35
	cmp r3, r2
	bhi .L_0200b3c0
	movs r0, #13
	movs r1, #3
	bl ObjectMotion_SetActionVariant
	ldrb r2, [r5]
	movs r3, #2
	orrs r3, r2
	b .L_0200b3ce
.L_0200b3c0:
	movs r0, #13
	movs r1, #1
	bl ObjectMotion_SetActionVariant
	ldrb r2, [r5]
	movs r3, #253
	ands r3, r2
.L_0200b3ce:
	strb r3, [r5]
	movs r0, #10
	bl Func_02000f18
	movs r0, #11
	bl Func_02000f18
	movs r0, #12
	bl Func_02000f18
	movs r0, #128
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200b402
	movs r3, #8
	movs r2, #18
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #0
	movs r2, #1
	movs r3, #1
	bl Func_020039a8
.L_0200b402:
	movs r0, #129
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200b424
	movs r3, #10
	movs r2, #18
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #0
	movs r2, #1
	movs r3, #1
	bl Func_020039a8
.L_0200b424:
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200b446
	movs r3, #10
	movs r2, #17
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #0
	movs r2, #1
	movs r3, #1
	bl Func_020039a8
.L_0200b446:
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_0200b4f8
	bl Scheduler_AddOrUpdateCallback
	ldr r0, .L_0200b4fc
	bl Func_02003b30
	bl Func_02002e70
	movs r1, #144
	ldr r0, .L_0200b500
	lsls r1, r1, #3
	bl Scheduler_AddOrUpdateCallback
	movs r0, #115
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200b47a
	movs r0, #208
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200b486
.L_0200b47a:
	movs r0, #131
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
	b .L_0200b4ee
.L_0200b486:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #124
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200b4ce
	movs r0, #14
	bl Object_GetById
	adds r5, r0, #0
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200b4bc
	adds r3, r5, #0
	adds r3, #98
	movs r1, #196
	movs r2, #148
	strb r0, [r3]
	lsls r1, r1, #17
	movs r0, #14
	lsls r2, r2, #17
	bl Func_02003a30
.L_0200b4bc:
	ldr r3, .L_0200b4f4
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r1, [r3]
	movs r0, #14
	bl Object_LinkObjectAndSetCallback
	b .L_0200b4ee
.L_0200b4ce:
	movs r0, #239
	lsls r0, r0, #3
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200b4ee
	movs r3, #128
	movs r1, #140
	movs r2, #164
	lsls r3, r3, #7
	movs r0, #14
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_02003a38
.L_0200b4ee:
	add sp, #8
	pop {r5, pc}
	.2byte 0x0000
.L_0200b4f4:
	.4byte gPartyState
.L_0200b4f8:
	.4byte Func_02001128
.L_0200b4fc:
	.4byte Data_02003cbc
.L_0200b500:
	.4byte Func_020011a0
	.section .text.x0200b504,"ax",%progbits
	.global Func_02003504
	.thumb_func
Func_02003504:
	push {r5, lr}
	ldr r3, .L_0200b594
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	sub sp, #8
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #32
	orrs r3, r2
	strb r3, [r0]
	movs r0, #8
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #1
	bl GameFlag_Test
	adds r5, r0, #0
	cmp r5, #0
	beq .L_0200b57a
	movs r1, #248
	movs r2, #132
	lsls r2, r2, #17
	movs r0, #8
	lsls r1, r1, #16
	bl Func_02003a30
	movs r1, #3
	movs r0, #8
	bl ObjectMotion_SetActionVariant
	movs r0, #8
	bl Object_GetById
	movs r3, #0
	adds r0, #85
	strb r3, [r0]
	movs r2, #16
	movs r3, #15
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #0
	movs r2, #1
	movs r3, #1
	bl Func_020039a8
	movs r0, #8
	bl Func_02003aa0
	b .L_0200b58c
.L_0200b57a:
	movs r0, #8
	movs r1, #1
	bl ObjectMotion_SetActionVariant
	movs r0, #8
	bl Object_GetById
	adds r0, #85
	strb r5, [r0]
.L_0200b58c:
	bl Func_0200300c
	add sp, #8
	pop {r5, pc}
.L_0200b594:
	.4byte gPartyState
	.section .text.x0200b598,"ax",%progbits
	.global Func_02003598
	.thumb_func
Func_02003598:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
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
	movs r0, #8
	sub sp, #8
	bl Func_02002f88
	movs r0, #9
	bl Func_02002f88
	movs r0, #10
	bl Func_02002f88
	movs r0, #11
	bl Func_02002f88
	movs r0, #12
	bl Func_02002f88
	movs r0, #13
	bl Func_02002f88
	movs r0, #14
	bl Func_02002f88
	movs r0, #15
	bl Func_02002f88
	movs r0, #16
	bl Func_02002f88
	movs r0, #13
	movs r1, #0
	bl Object_SetModeById
	movs r0, #14
	movs r1, #0
	bl Object_SetModeById
	movs r0, #15
	movs r1, #2
	bl Object_SetModeById
	movs r0, #16
	movs r1, #2
	bl Object_SetModeById
	movs r0, #153
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200b658
	movs r3, #13
	str r3, [sp, #4]
	movs r5, #19
	movs r0, #0
	movs r1, #0
	movs r2, #3
	movs r3, #1
	str r5, [sp, #0]
	bl Func_020039a8
	movs r3, #33
	str r3, [sp, #4]
	movs r1, #0
	movs r2, #2
	movs r3, #1
	movs r0, #40
	str r5, [sp, #0]
	bl Func_020039a8
	movs r0, #15
	bl Object_GetById
	movs r1, #160
	lsls r1, r1, #17
	ldr r2, .L_0200b738
	ldr r3, .L_0200b73c
	bl Object_SetPositionAndResetMotion
	movs r0, #15
	movs r1, #3
	bl ObjectMotion_SetActionVariant
.L_0200b658:
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #50
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200b6a6
	movs r3, #22
	str r3, [sp, #4]
	movs r5, #24
	movs r0, #0
	movs r1, #0
	movs r2, #3
	movs r3, #1
	str r5, [sp, #0]
	bl Func_020039a8
	movs r1, #0
	movs r2, #2
	movs r3, #1
	movs r0, #40
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl Func_020039a8
	movs r0, #16
	bl Object_GetById
	movs r1, #200
	movs r3, #195
	lsls r1, r1, #17
	ldr r2, .L_0200b738
	lsls r3, r3, #17
	bl Object_SetPositionAndResetMotion
	movs r0, #16
	movs r1, #3
	bl ObjectMotion_SetActionVariant
.L_0200b6a6:
	bl Func_02003b18
	movs r1, #130
	lsls r1, r1, #1
	movs r0, #0
	adds r1, #255
	movs r2, #17
	movs r3, #18
	bl Func_02003b20
	movs r1, #129
	lsls r1, r1, #2
	movs r2, #19
	movs r3, #20
	movs r0, #1
	bl Func_02003b20
	movs r0, #21
	bl Func_02003aa0
	movs r0, #21
	bl Object_GetById
	movs r3, #0
	mov r8, r3
	mov r3, r8
	adds r0, #85
	strb r3, [r0]
	movs r0, #22
	bl Object_GetById
	adds r5, r0, #0
	movs r0, #22
	bl Func_02003aa0
	adds r2, r5, #0
	adds r2, #35
	ldrb r3, [r2]
	movs r6, #2
	orrs r3, r6
	strb r3, [r2]
	adds r5, #85
	mov r3, r8
	strb r3, [r5]
	movs r0, #23
	bl Func_02003aa0
	movs r0, #23
	bl Object_GetById
	mov r3, r8
	adds r0, #85
	strb r3, [r0]
	movs r0, #24
	bl Object_GetById
	adds r5, r0, #0
	movs r0, #24
	bl Func_02003aa0
	adds r2, r5, #0
	adds r2, #35
	ldrb r3, [r2]
	adds r5, #85
	orrs r6, r3
	mov r3, r8
	strb r6, [r2]
	add sp, #8
	strb r3, [r5]
	pop {r3}
	mov r8, r3
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200b738:
	.4byte 0xffeb0000
.L_0200b73c:
	.4byte 0x02160000
	.section .text.x0200b740,"ax",%progbits
	.global Func_02003740
	.thumb_func
Func_02003740:
	push {lr}
	movs r0, #8
	bl Func_02003854
	movs r0, #9
	bl Func_02003854
	movs r0, #10
	bl Func_02003854
	ldr r3, .L_0200b778
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #3
	bne .L_0200b774
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200b774
	bl Func_02000994
.L_0200b774:
	pop {pc}
	.2byte 0x0000
.L_0200b778:
	.4byte gPartyState
	.section .text.x0200b77c,"ax",%progbits
	.global Func_0200377c
	.thumb_func
Func_0200377c:
	push {lr}
	ldr r3, .L_0200b7a0
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #1
	bne .L_0200b79e
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200b79e
	bl Func_02000ce8
.L_0200b79e:
	pop {pc}
.L_0200b7a0:
	.4byte gPartyState
	.section .text.x0200b7a4,"ax",%progbits
	.global Func_020037a4
	.thumb_func
Func_020037a4:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r3, .L_0200b808
	movs r6, #7
	ldr r7, [r3]
	sub sp, #56
	ands r7, r6
	mov r8, r0
	cmp r7, #0
	bne .L_0200b7fe
	bl Random16Far
	movs r5, #15
	ands r5, r0
	bl Random16Far
	movs r3, #209
	lsls r3, r3, #1
	ands r0, r6
	adds r3, #255
	add r6, sp, #16
	strh r3, [r6, #24]
	mov r3, r8
	ldr r4, [r3, #8]
	ldr r1, [r3, #12]
	ldr r2, [r3, #16]
	movs r3, #128
	lsls r3, r3, #8
	subs r5, #8
	lsls r5, r5, #16
	subs r0, #8
	str r3, [sp, #0]
	movs r3, #128
	lsls r0, r0, #16
	lsls r3, r3, #13
	adds r4, r4, r5
	adds r1, r1, r0
	str r3, [sp, #8]
	adds r0, r4, #0
	movs r3, #0
	str r7, [sp, #4]
	str r6, [sp, #12]
	bl Func_020000b8
.L_0200b7fe:
	add sp, #56
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200b808:
	.4byte Data_0300122c
	.section .text.x0200b80c,"ax",%progbits
	.global Func_0200380c
	.thumb_func
Func_0200380c:
	push {r5, lr}
	movs r0, #64
	bl Object_GetById
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #32]
	movs r1, #188
	lsls r1, r1, #1
	adds r2, r2, r1
	ldr r3, [r2, #8]
	movs r1, #128
	lsls r1, r1, #16
	adds r3, r3, r1
	str r3, [r2, #8]
	adds r5, r0, #0
	bl Func_02003988
	movs r0, #1
	bl WaitFrames
	cmp r5, #0
	beq .L_0200b84c
	adds r2, r5, #0
	movs r3, #0
	adds r2, #85
	strb r3, [r2]
	movs r3, #132
	lsls r3, r3, #15
	str r3, [r5, #12]
	ldr r3, .L_0200b850
	str r3, [r5, #108]
.L_0200b84c:
	pop {r5, pc}
	.2byte 0x0000
.L_0200b850:
	.4byte Func_020037a4
	.section .text.x0200b854,"ax",%progbits
	.global Func_02003854
	.thumb_func
Func_02003854:
	push {r5, r6, lr}
	adds r5, r0, #0
	bl Object_GetById
	movs r1, #3
	adds r6, r0, #0
	adds r0, r5, #0
	bl ObjectMotion_SetActionVariant
	adds r0, r5, #0
	bl Func_02003aa0
	adds r0, r6, #0
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #2
	orrs r3, r2
	strb r3, [r0]
	adds r3, r6, #0
	movs r1, #0
	adds r3, #85
	strb r1, [r3]
	movs r3, #204
	lsls r3, r3, #8
	adds r3, #204
	str r3, [r6, #24]
	movs r3, #230
	lsls r3, r3, #8
	adds r3, #102
	str r3, [r6, #28]
	pop {r5, r6, pc}
	.2byte 0x0000
	.section .text.x0200b894,"ax",%progbits
	.global Func_02003894
	.thumb_func
Func_02003894:
	push {lr}
	movs r0, #208
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200b910
	ldr r3, .L_0200b914
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #86
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #1
	bne .L_0200b8f4
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #65
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200b8d4
	movs r1, #180
	movs r2, #216
	movs r0, #14
	lsls r1, r1, #17
	lsls r2, r2, #16
	movs r3, #0
	bl Func_02003a38
	b .L_0200b8f4
.L_0200b8d4:
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #66
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200b8f4
	movs r3, #128
	movs r1, #164
	movs r2, #216
	lsls r3, r3, #8
	movs r0, #14
	lsls r1, r1, #17
	lsls r2, r2, #16
	bl Func_02003a38
.L_0200b8f4:
	movs r0, #208
	lsls r0, r0, #2
	bl GameFlag_ClearBit
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #65
	bl GameFlag_ClearBit
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #66
	bl GameFlag_ClearBit
.L_0200b910:
	movs r0, #0
	pop {pc}
.L_0200b914:
	.4byte gPartyState
	.section .rodata.x0200bb50,"a",%progbits
.L_0200bb50:
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
.L_0200bb8c:
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
.L_0200bbc8:
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
	.global Data_02003c04
Data_02003c04:
	.4byte .L_0200bb50
	.4byte .L_0200bb8c
	.4byte .L_0200bbc8
	.global Data_02003c10
Data_02003c10:
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000b000
	.4byte 0x00000000
	.4byte 0x00000050
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00005000
	.4byte 0x00000000
	.4byte 0x00000050
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x00000078
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0x00000000
	.global Data_02003c58
Data_02003c58:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x00c60000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x017a0000
	.4byte 0x00000000
	.4byte 0x00c60000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x00dc0000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x011c0000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000011
	.global Data_02003cbc
Data_02003cbc:
	.4byte 0xffff0008
	.global Data_02003cc0
Data_02003cc0:
	.4byte 0x013c0000
	.4byte 0x00880000
	.4byte 0xffff0000
	.4byte 0x00008000
	.4byte 0x013e0000
	.4byte 0x008e0000
	.4byte 0x00010000
	.4byte 0x00007000
	.4byte 0x01440000
	.4byte 0x009a0000
	.4byte 0xffff0000
	.4byte 0x00006000
	.4byte 0x014a0000
	.4byte 0x009e0000
	.4byte 0x00010000
	.4byte 0x00005000
	.4byte 0x01520000
	.4byte 0x00a60000
	.4byte 0xffff0000
	.4byte 0x00004800
	.4byte 0x01580000
	.4byte 0x00a60000
	.4byte 0x00010000
	.4byte 0x00004800
	.4byte 0x01200000
	.4byte 0x00f00000
	.4byte 0x00010000
	.4byte 0x0000d800
	.4byte 0x011c0000
	.4byte 0x00ec0000
	.4byte 0xffff0000
	.4byte 0x0000d800
	.4byte 0x01100000
	.4byte 0x00e80000
	.4byte 0x00010000
	.4byte 0x0000c800
	.4byte 0x010a0000
	.4byte 0x00e60000
	.4byte 0xffff0000
	.4byte 0x0000c800
	.4byte 0x01000000
	.4byte 0x00ee0000
	.4byte 0x00010000
	.4byte 0x0000b000
	.4byte 0x00f80000
	.4byte 0x00ee0000
	.4byte 0xffff0000
	.4byte 0x0000b800
	.4byte 0x00f00000
	.4byte 0x00fa0000
	.4byte 0x00010000
	.4byte 0x0000a800
	.4byte 0x00ea0000
	.4byte 0x00fc0000
	.4byte 0xffff0000
	.4byte 0x0000b000
	.4byte 0x00e00000
	.4byte 0x01060000
	.4byte 0x00010000
	.4byte 0x0000a800
	.4byte 0x00dc0000
	.4byte 0x01060000
	.4byte 0xffff0000
	.4byte 0x0000a800
	.4byte 0x0000ffff
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
	.global Data_02003df4
Data_02003df4:
	.4byte 0x00200100
	.4byte 0x011000c0
	.4byte 0x00d00030
	.4byte 0x0002ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02003e14
Data_02003e14:
	.4byte 0xffda02d0
	.4byte 0x02e000ee
	.4byte 0x00feffea
	.4byte 0x0001ffff
	.4byte 0x00200360
	.4byte 0x037000d0
	.4byte 0x00e00030
	.4byte 0x0002ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02003e44
Data_02003e44:
	.4byte 0xffda0310
	.4byte 0x032000a0
	.4byte 0x00b0ffea
	.4byte 0x0001ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gSceneExits
gSceneExits:
	.4byte 0x00000017
	.4byte 0x00105019
	.4byte 0x00201018
	.4byte 0x00305002
	.4byte 0x00000018
	.4byte 0x00102017
	.4byte 0x00000019
	.4byte 0x0010201d
	.4byte 0x0020101e
	.4byte 0x0030201e
	.4byte 0x0040101a
	.4byte 0x00501017
	.4byte 0x0000001a
	.4byte 0x00104019
	.4byte 0x0020201b
	.4byte 0x0030401b
	.4byte 0x0040301b
	.4byte 0x0000001b
	.4byte 0x0010101c
	.4byte 0x0020201a
	.4byte 0x0030401a
	.4byte 0x0040301a
	.4byte 0x0000001c
	.4byte 0x0010101b
	.4byte 0x0020101d
	.4byte 0x0000001d
	.4byte 0x0010201c
	.4byte 0x00201019
	.4byte 0x0000001e
	.4byte 0x00102019
	.4byte 0x00203019
	.4byte 0x0030101f
	.4byte 0x0000001f
	.4byte 0x0010301e
	.4byte 0x00228020
	.4byte 0x00000020
	.4byte 0x0010201f
	.4byte 0x00228021
	.4byte 0x00000021
	.4byte 0x00129020
	.4byte 0x000001ff
	.global Data_02003f08
Data_02003f08:
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02003f20
Data_02003f20:
	.4byte 0xffff0129
	.4byte 0x00000001
	.4byte 0x00e00000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00024000
	.4byte 0xffff0129
	.4byte 0x00000001
	.4byte 0x01000000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x01024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x01024000
	.4byte 0xffff0012
	.4byte 0x00000001
	.4byte 0x01540000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00028000
	.4byte 0xffff0011
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00020000
	.4byte 0xffff0038
	.4byte 0x00000001
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
	.global Data_02003fe0
Data_02003fe0:
	.4byte 0xffff014f
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00024000
	.4byte 0xffff019a
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0171
	.4byte 0x00000001
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x01100000
	.4byte 0x00024000
	.4byte 0xffff0012
	.4byte 0x00000001
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
	.global Data_02004058
Data_02004058:
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00024000
	.4byte 0xffff0172
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00024000
	.4byte 0xffff0171
	.4byte 0x00000001
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00024000
	.4byte 0xffff0171
	.4byte 0x00000001
	.4byte 0x02080000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x01024000
	.4byte 0xffff0171
	.4byte 0x00000001
	.4byte 0x02180000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x01024000
	.4byte 0xffff0171
	.4byte 0x00000001
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x00024000
	.4byte 0xffff0171
	.4byte 0x00000001
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x00024000
	.4byte 0xffff00f6
	.4byte 0x00000001
	.4byte 0x01d80000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02004130
Data_02004130:
	.4byte 0xffff010e
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff010e
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff010e
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff010e
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff010e
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff010e
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff010e
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff010e
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff010e
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff010e
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff010e
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff010e
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff010e
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff010e
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff010e
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff010e
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020042c8
Data_020042c8:
	.4byte 0xffff014f
	.4byte 0x00000001
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00024000
	.4byte 0xffff019a
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00024000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00024000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00024000
	.4byte 0xffff0171
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x02280000
	.4byte 0x00024000
	.4byte 0x007300f6
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02004388
Data_02004388:
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x01024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x01024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02004400
Data_02004400:
	.4byte 0xffff0171
	.4byte 0x00000001
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00024000
	.4byte 0xffff0171
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x01024000
	.4byte 0xffff0171
	.4byte 0x00000001
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x01024000
	.4byte 0xffff0171
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x01024000
	.4byte 0xffff0172
	.4byte 0x00000001
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00024000
	.4byte 0xffff0171
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00024000
	.4byte 0xffff0171
	.4byte 0x00000001
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x01024000
	.4byte 0xffff0171
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00024000
	.4byte 0xffff0171
	.4byte 0x00000001
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x01024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x02680000
	.4byte 0x00024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x02680000
	.4byte 0x01024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x01c80000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x01024000
	.4byte 0xffff0114
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0114
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff0114
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0114
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff01e9
	.4byte 0x00000007
	.4byte 0x01c00000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00024000
	.4byte 0xffff01e9
	.4byte 0x00000007
	.4byte 0x01d00000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00024000
	.4byte 0xffff01e9
	.4byte 0x00000007
	.4byte 0x01c80000
	.4byte 0x00000000
	.4byte 0x00f00000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020045f8
Data_020045f8:
	.4byte 0xffff0129
	.4byte 0x00000001
	.4byte 0x00a00000
	.4byte 0x00000000
	.4byte 0x00ca0000
	.4byte 0x00024000
	.4byte 0xffff0129
	.4byte 0x00000001
	.4byte 0x00b00000
	.4byte 0x00000000
	.4byte 0x012a0000
	.4byte 0x01024000
	.4byte 0xffff0129
	.4byte 0x00000001
	.4byte 0x01400000
	.4byte 0x00000000
	.4byte 0x00da0000
	.4byte 0x01024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02004658
Data_02004658:
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02004664
Data_02004664:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00008515
	.4byte 0x0203000a
	.4byte Func_02001d24
	.4byte 0x00008b85
	.4byte 0xffff0000
	.4byte Func_02002e8c
	.4byte 0x40008b85
	.4byte 0xffff0000
	.4byte Func_02002ea8
	.4byte 0x00000002
	.4byte 0x0872000a
	.4byte Func_02001a3c
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020046c4
Data_020046c4:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00008400
	.4byte 0xffff000b
	.4byte Func_020004e0
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte MsgFieldICantFindAnythingAroundHere
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020046f4
Data_020046f4:
	.4byte 0x00000031
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000031
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000031
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000031
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x10008c15
	.4byte 0x084c0008
	.4byte Func_02000fa8
	.4byte 0x00008c15
	.4byte 0x084c0008
	.4byte Func_02000fec
	.4byte 0x00000002
	.4byte 0x020a000a
	.4byte Func_02001290
	.4byte 0x00000002
	.4byte 0x120a000b
	.4byte Func_020012b4
	.4byte 0x00000002
	.4byte 0x0230000f
	.4byte Func_02001474
	.4byte 0x00000002
	.4byte 0x0874000b
	.4byte Func_02002384
	.4byte 0x00000002
	.4byte 0x0875000c
	.4byte Func_02002430
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02004790
Data_02004790:
	.4byte 0x00000021
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000021
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000021
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000021
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020047cc
Data_020047cc:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000031
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000031
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000031
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00001815
	.4byte 0x0200000a
	.4byte Func_02000f34
	.4byte 0x00001815
	.4byte 0x0201000b
	.4byte Func_02000f58
	.4byte 0x00001815
	.4byte 0x0202000c
	.4byte Func_02000f80
	.4byte 0x00000008
	.4byte 0xffff0000
	.4byte Func_02000fa8
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte Func_02000fb8
	.4byte 0x10008c15
	.4byte 0xffff0008
	.4byte Func_02000fa8
	.4byte 0x00008c15
	.4byte 0xffff0008
	.4byte Func_02000fb8
	.4byte 0x00000002
	.4byte 0x0877000a
	.4byte Func_020024e4
	.4byte 0x00000002
	.4byte 0x0877000b
	.4byte Func_020025fc
	.4byte 0x00000002
	.4byte 0x087c000c
	.4byte Func_020026f8
	.4byte 0x00000002
	.4byte 0x0205000d
	.4byte Func_02002d50
	.4byte 0x00000002
	.4byte 0x0205000e
	.4byte Func_02002d50
	.4byte 0x00000002
	.4byte 0x0205000f
	.4byte Func_02002d50
	.4byte 0x00000002
	.4byte 0x02050010
	.4byte Func_02002d50
	.4byte 0x00000002
	.4byte 0x02050011
	.4byte Func_02002d50
	.4byte 0x00000002
	.4byte 0x02050012
	.4byte Func_02002d50
	.4byte 0x00000002
	.4byte 0x02050013
	.4byte Func_02002d50
	.4byte 0x00000002
	.4byte 0x02050014
	.4byte Func_02002d50
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte Func_02000280
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020048ec
Data_020048ec:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000031
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000009
	.4byte 0x03010000
	.4byte Func_02001038
	.4byte 0x00008c15
	.4byte 0x03010008
	.4byte Func_02001038
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02004928
Data_02004928:
	.4byte 0x00000021
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000021
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00008515
	.4byte 0x02030011
	.4byte 0x00000000
	.4byte 0x00008515
	.4byte 0x02040013
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x02310005
	.4byte Func_020016d8
	.4byte 0x00000002
	.4byte 0x02320006
	.4byte Func_020018bc
	.4byte 0x00000000
	.4byte 0xffff0019
	.4byte Func_02000554
	.4byte 0x00000000
	.4byte 0xffff001a
	.4byte Func_02000554
	.4byte 0x00000000
	.4byte 0xffff001b
	.4byte Func_02000554
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020049a0
Data_020049a0:
	.4byte 0x00000021
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000021
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0xffff000a
	.4byte Func_020004b0
	.4byte 0x0001ca04
	.4byte 0xffff000a
	.4byte Func_02000688
	.4byte 0x50009085
	.4byte 0xffff0000
	.4byte Func_020019dc
	.4byte 0x40009085
	.4byte 0xffff0000
	.4byte Func_02001a0c
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020049f4
Data_020049f4:
	.4byte 0x00000003
	.4byte 0xffff0001
	.4byte Func_020004b0
	.4byte 0x0001ca04
	.4byte 0xffff0001
	.4byte Func_02000e38
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.section .bss,"aw",%nobits
	.global Data_02004a18
Data_02004a18:
