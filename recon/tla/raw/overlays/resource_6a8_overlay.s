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
	bl Func_020043dc
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
	bl Func_020043c4
	ldr r2, .L_02008274
	mov r3, r8
	ands r3, r5
	lsls r3, r3, #2
	ldr r1, [r2, r3]
	adds r0, r6, #0
	mov r10, r3
	bl Func_020043d4
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
	bl Func_02004514
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
	bl Engine_MathDivide
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, [r6, #28]
	ldr r1, [r5, #12]
	subs r0, r0, r3
	b .L_0200820c
.L_020081fa:
	ldr r2, .L_0200827c
	adds r0, r3, r2
	bl Engine_MathDivide
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, .L_0200827c
	ldr r1, [r5, #12]
	adds r0, r0, r3
.L_0200820c:
	bl Engine_MathDivide
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
	bl Func_020043c4
	ldr r1, [r7, #28]
	adds r0, r6, #0
	bl Func_020043d4
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
	.4byte Data_020047a0
.L_02008278:
	.4byte Func_02000080
.L_0200827c:
	.4byte 0xffff0000
	.section .text.x02008280,"ax",%progbits
	.global Func_02000280
	.thumb_func
Func_02000280:
	push {lr}
	movs r0, #20
	movs r1, #0
	movs r2, #16
	bl Func_020045f4
	pop {pc}
	.2byte 0x0000
	.section .text.x02008290,"ax",%progbits
	.global Func_02000290
	.thumb_func
Func_02000290:
	push {lr}
	movs r0, #24
	movs r1, #1
	movs r2, #19
	bl Func_020045f4
	pop {pc}
	.2byte 0x0000
	.section .text.x020082a0,"ax",%progbits
	.global Func_020002a0
	.thumb_func
Func_020002a0:
	push {r5, lr}
	ldmia r0!, {r5}
	ldmia r1!, {r3}
	ldmia r0!, {r4}
	subs r5, r5, r3
	ldmia r1!, {r3}
	asrs r5, r5, #16
	ldr r2, [r1]
	subs r4, r4, r3
	ldr r3, [r0]
	asrs r4, r4, #16
	subs r3, r3, r2
	asrs r3, r3, #16
	adds r0, r5, #0
	muls r0, r5
	adds r2, r4, #0
	muls r2, r4
	adds r1, r3, #0
	muls r1, r3
	adds r0, r0, r2
	adds r3, r1, #0
	adds r0, r0, r3
	ldr r3, .L_020082d4
	mov lr, r3
	.2byte 0xf800
	pop {r5, pc}
.L_020082d4:
	.4byte IwramFillWords + 0x74
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
	bl Func_020002a0
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
	bl Func_02004364
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
	ldr r3, [r3, #108]
	mov r10, r2
	mov r8, r3
	ldr r3, .L_02008474
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	adds r5, r0, #0
	ldr r0, [r3]
	bl Object_GetById
	adds r1, r5, #0
	adds r0, #8
	adds r1, #8
	movs r7, #0
	bl Func_020002a0
	cmp r0, #11
	bgt .L_02008400
	adds r3, r5, #0
	adds r3, #91
	adds r0, r5, #0
	strb r7, [r3]
	movs r1, #2
	bl Func_020043c4
	b .L_0200846a
.L_02008400:
	adds r6, r5, #0
	adds r6, #100
	movs r2, #0
	ldrsh r3, [r6, r2]
	cmp r3, #0
	bne .L_02008410
	movs r0, #18
	b .L_0200841a
.L_02008410:
	cmp r3, #1
	bne .L_02008418
	movs r0, #16
	b .L_0200841a
.L_02008418:
	movs r0, #17
.L_0200841a:
	bl Object_GetById
	adds r1, r0, #0
	adds r0, r5, #0
	movs r2, #24
	movs r3, #0
	bl Func_020002d8
	cmp r0, #0
	bne .L_0200846a
	ldr r3, .L_02008474
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	movs r3, #176
	lsls r3, r3, #1
	add r3, r8
	movs r2, #0
	ldrsh r3, [r3, r2]
	adds r1, r0, #0
	cmp r3, #0
	bne .L_02008454
	mov r2, r10
	ldrb r3, [r2, #4]
	cmp r3, #0
	beq .L_02008460
.L_02008454:
	ldrh r2, [r6]
	movs r3, #2
	ands r3, r2
	cmp r3, #0
	beq .L_02008460
	movs r7, #1
.L_02008460:
	adds r0, r5, #0
	movs r2, #32
	adds r3, r7, #0
	bl Func_020002d8
.L_0200846a:
	movs r0, #0
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_02008474:
	.4byte gPartyState
	.section .text.x02008478,"ax",%progbits
	.global Func_02000478
	.thumb_func
Func_02000478:
	ldr r0, .L_0200847c
	bx lr
.L_0200847c:
	.4byte Data_02004a44
	.section .text.x02008480,"ax",%progbits
	.global Func_02000480
	.thumb_func
Func_02000480:
	movs r0, #0
	bx lr
	.section .text.x02008484,"ax",%progbits
	.global Func_02000484
	.thumb_func
Func_02000484:
	ldr r0, .L_02008488
	bx lr
.L_02008488:
	.4byte Data_02004a74
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
	bl Func_020043ac
	cmp r0, #0
	beq .L_020084c0
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #204
	bl Func_020043ac
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
	bl Func_020043ac
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
	bl Func_020043ac
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
	bl Func_020043ac
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
	.4byte gPartyState
.L_02008548:
	.4byte 0x0000010d
.L_0200854c:
	.4byte Data_02004b2c
.L_02008550:
	.4byte 0x0000010e
.L_02008554:
	.4byte Data_02004c4c
.L_02008558:
	.4byte 0x0000010f
.L_0200855c:
	.4byte Data_02004f04
.L_02008560:
	.4byte Data_02004d9c
.L_02008564:
	.4byte 0x00000110
.L_02008568:
	.4byte Data_0200521c
.L_0200856c:
	.4byte Data_0200506c
.L_02008570:
	.4byte 0x00000111
.L_02008574:
	.4byte Data_020054a4
.L_02008578:
	.4byte Data_02005444
.L_0200857c:
	.4byte 0x00000112
.L_02008580:
	.4byte Data_0200551c
.L_02008584:
	.4byte Data_02004b14
	.section .text.x02008588,"ax",%progbits
	.global Func_02000588
	.thumb_func
Func_02000588:
	push {lr}
	movs r2, #192
	movs r1, #64
	lsls r2, r2, #2
	bl Func_020045cc
	pop {pc}
	.2byte 0x0000
	.section .text.x02008598,"ax",%progbits
	.global Func_02000598
	.thumb_func
Func_02000598:
	push {lr}
	ldr r3, .L_020085e8
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_020085ec
	cmp r2, r3
	bne .L_020085b0
	ldr r0, .L_020085f0
	b .L_020085e4
.L_020085b0:
	ldr r3, .L_020085f4
	cmp r2, r3
	bne .L_020085ba
	ldr r0, .L_020085f8
	b .L_020085e4
.L_020085ba:
	ldr r3, .L_020085fc
	cmp r2, r3
	bne .L_020085c4
	ldr r0, .L_02008600
	b .L_020085e4
.L_020085c4:
	ldr r3, .L_02008604
	cmp r2, r3
	bne .L_020085ce
	ldr r0, .L_02008608
	b .L_020085e4
.L_020085ce:
	ldr r3, .L_0200860c
	cmp r2, r3
	bne .L_020085d8
	ldr r0, .L_02008610
	b .L_020085e4
.L_020085d8:
	ldr r3, .L_02008614
	cmp r2, r3
	bne .L_020085e2
	ldr r0, .L_02008618
	b .L_020085e4
.L_020085e2:
	ldr r0, .L_0200861c
.L_020085e4:
	pop {pc}
	.2byte 0x0000
.L_020085e8:
	.4byte gPartyState
.L_020085ec:
	.4byte 0x0000010d
.L_020085f0:
	.4byte Data_02005558
.L_020085f4:
	.4byte 0x0000010e
.L_020085f8:
	.4byte Data_02005588
.L_020085fc:
	.4byte 0x0000010f
.L_02008600:
	.4byte Data_020057f8
.L_02008604:
	.4byte 0x00000110
.L_02008608:
	.4byte Data_02005a20
.L_0200860c:
	.4byte 0x00000111
.L_02008610:
	.4byte Data_02005da4
.L_02008614:
	.4byte 0x00000112
.L_02008618:
	.4byte Data_02005eac
.L_0200861c:
	.4byte Data_0200554c
	.section .text.x02008620,"ax",%progbits
	.global Func_02000620
	.thumb_func
Func_02000620:
	push {lr}
	bl Func_02004464
	movs r0, #0
	bl Func_020045ec
	ldr r0, .L_02008640
	bl Func_0200451c
	movs r1, #0
	movs r0, #15
	bl Func_0200453c
	bl Func_0200446c
	pop {pc}
.L_02008640:
	.4byte 0x00002cfd
	.section .text.x02008644,"ax",%progbits
	.global Func_02000644
	.thumb_func
Func_02000644:
	push {lr}
	ldr r3, .L_0200866c
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	movs r2, #190
	ldrh r3, [r0, #6]
	lsls r2, r2, #7
	adds r2, #255
	adds r3, r3, r2
	ldr r2, .L_02008670
	lsls r3, r3, #16
	movs r0, #1
	cmp r3, r2
	bls .L_0200866a
	movs r0, #0
.L_0200866a:
	pop {pc}
.L_0200866c:
	.4byte gPartyState
.L_02008670:
	.4byte 0x3ffe0000
	.section .text.x02008674,"ax",%progbits
	.global Func_02000674
	.thumb_func
Func_02000674:
	push {lr}
	bl Func_02000644
	cmp r0, #0
	beq .L_02008688
	movs r0, #14
	movs r1, #22
	bl Func_02004634
	b .L_020086ba
.L_02008688:
	bl Func_02004464
	movs r0, #0
	bl Func_020045ec
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #61
	bl Func_020043ac
	cmp r0, #0
	beq .L_020086a8
	ldr r0, .L_020086bc
	bl Func_0200451c
	b .L_020086ae
.L_020086a8:
	ldr r0, .L_020086c0
	bl Func_0200451c
.L_020086ae:
	movs r0, #22
	movs r1, #0
	bl Func_02004534
	bl Func_0200446c
.L_020086ba:
	pop {pc}
.L_020086bc:
	.4byte 0x00002ed6
.L_020086c0:
	.4byte 0x00002d1d
	.section .text.x020086c4,"ax",%progbits
	.global Func_020006c4
	.thumb_func
Func_020006c4:
	push {lr}
	bl Func_02000644
	cmp r0, #0
	beq .L_020086d8
	movs r0, #31
	movs r1, #10
	bl Func_02004624
	b .L_0200870a
.L_020086d8:
	bl Func_02004464
	movs r0, #0
	bl Func_020045ec
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #61
	bl Func_020043ac
	cmp r0, #0
	beq .L_020086f8
	ldr r0, .L_0200870c
	bl Func_0200451c
	b .L_020086fe
.L_020086f8:
	ldr r0, .L_02008710
	bl Func_0200451c
.L_020086fe:
	movs r0, #10
	movs r1, #0
	bl Func_02004534
	bl Func_0200446c
.L_0200870a:
	pop {pc}
.L_0200870c:
	.4byte 0x00002ed3
.L_02008710:
	.4byte 0x00002d1a
	.section .text.x02008714,"ax",%progbits
	.global Func_02000714
	.thumb_func
Func_02000714:
	push {lr}
	bl Func_02000644
	cmp r0, #0
	beq .L_02008726
	movs r0, #8
	bl Func_0200462c
	b .L_02008758
.L_02008726:
	bl Func_02004464
	movs r0, #0
	bl Func_020045ec
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #61
	bl Func_020043ac
	cmp r0, #0
	beq .L_02008746
	ldr r0, .L_0200875c
	bl Func_0200451c
	b .L_0200874c
.L_02008746:
	ldr r0, .L_02008760
	bl Func_0200451c
.L_0200874c:
	movs r0, #8
	movs r1, #0
	bl Func_02004534
	bl Func_0200446c
.L_02008758:
	pop {pc}
	.2byte 0x0000
.L_0200875c:
	.4byte 0x00002eda
.L_02008760:
	.4byte 0x00002d21
	.section .text.x02008764,"ax",%progbits
	.global Func_02000764
	.thumb_func
Func_02000764:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	movs r0, #16
	ldr r7, [r3, #108]
	bl Object_GetById
	adds r5, r0, #0
	movs r3, #6
	ldrsh r2, [r5, r3]
	adds r6, r5, #0
	mov r8, r2
	bl Func_02004464
	movs r0, #0
	bl Func_020045ec
	movs r2, #179
	movs r3, #1
	lsls r2, r2, #1
	adds r6, #99
	strb r3, [r6]
	adds r3, r7, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	bne .L_020087b8
	ldr r3, .L_020087e8
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	movs r0, #16
	ldr r1, [r3]
	movs r2, #0
	bl Func_0200450c
	ldr r0, .L_020087ec
	bl Func_0200451c
	b .L_020087be
.L_020087b8:
	ldr r0, .L_020087f0
	bl Func_0200451c
.L_020087be:
	movs r0, #16
	movs r1, #0
	bl Func_02004534
	movs r2, #179
	lsls r2, r2, #1
	adds r3, r7, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	bne .L_020087d8
	mov r3, r8
	strh r3, [r5, #6]
.L_020087d8:
	movs r3, #0
	strb r3, [r6]
	bl Func_0200446c
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_020087e8:
	.4byte gPartyState
.L_020087ec:
	.4byte 0x00002ec0
.L_020087f0:
	.4byte 0x00002ec4
	.section .text.x020087f4,"ax",%progbits
	.global Func_020007f4
	.thumb_func
Func_020007f4:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	movs r0, #17
	ldr r7, [r3, #108]
	bl Object_GetById
	adds r5, r0, #0
	movs r3, #6
	ldrsh r2, [r5, r3]
	adds r6, r5, #0
	mov r8, r2
	bl Func_02004464
	movs r0, #0
	bl Func_020045ec
	movs r2, #179
	movs r3, #1
	lsls r2, r2, #1
	adds r6, #99
	strb r3, [r6]
	adds r3, r7, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	bne .L_02008848
	ldr r3, .L_02008878
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	movs r0, #17
	ldr r1, [r3]
	movs r2, #0
	bl Func_0200450c
	ldr r0, .L_0200887c
	bl Func_0200451c
	b .L_0200884e
.L_02008848:
	ldr r0, .L_02008880
	bl Func_0200451c
.L_0200884e:
	movs r0, #17
	movs r1, #0
	bl Func_02004534
	movs r2, #179
	lsls r2, r2, #1
	adds r3, r7, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	bne .L_02008868
	mov r3, r8
	strh r3, [r5, #6]
.L_02008868:
	movs r3, #0
	strb r3, [r6]
	bl Func_0200446c
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008878:
	.4byte gPartyState
.L_0200887c:
	.4byte 0x00002ec1
.L_02008880:
	.4byte 0x00002ec5
	.section .text.x02008884,"ax",%progbits
	.global Func_02000884
	.thumb_func
Func_02000884:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	movs r0, #18
	ldr r7, [r3, #108]
	bl Object_GetById
	adds r5, r0, #0
	movs r3, #6
	ldrsh r2, [r5, r3]
	adds r6, r5, #0
	mov r8, r2
	bl Func_02004464
	movs r0, #0
	bl Func_020045ec
	movs r2, #179
	movs r3, #1
	lsls r2, r2, #1
	adds r6, #99
	strb r3, [r6]
	adds r3, r7, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	bne .L_020088d8
	ldr r3, .L_02008908
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	movs r0, #18
	ldr r1, [r3]
	movs r2, #0
	bl Func_0200450c
	ldr r0, .L_0200890c
	bl Func_0200451c
	b .L_020088de
.L_020088d8:
	ldr r0, .L_02008910
	bl Func_0200451c
.L_020088de:
	movs r0, #18
	movs r1, #0
	bl Func_02004534
	movs r2, #179
	lsls r2, r2, #1
	adds r3, r7, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	bne .L_020088f8
	mov r3, r8
	strh r3, [r5, #6]
.L_020088f8:
	movs r3, #0
	strb r3, [r6]
	bl Func_0200446c
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008908:
	.4byte gPartyState
.L_0200890c:
	.4byte 0x00002ec2
.L_02008910:
	.4byte 0x00002ec6
	.section .text.x02008914,"ax",%progbits
	.global Func_02000914
	.thumb_func
Func_02000914:
	push {r5, lr}
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #61
	sub sp, #8
	bl Func_020043ac
	adds r5, r0, #0
	cmp r5, #0
	beq .L_0200892a
	b .L_02008ac8
.L_0200892a:
	bl Func_02004464
	movs r0, #0
	bl Func_020045ec
	movs r0, #158
	bl Func_0200463c
	movs r3, #1
	movs r2, #2
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #0
	movs r2, #77
	movs r3, #16
	movs r0, #89
	bl Func_02004404
	movs r0, #128
	lsls r0, r0, #2
	bl Func_020043b4
	movs r0, #10
	bl Battle_WaitMode0
	movs r3, #192
	movs r1, #130
	movs r2, #180
	lsls r3, r3, #6
	lsls r2, r2, #17
	lsls r1, r1, #18
	movs r0, #8
	bl Func_020044d4
	bl Func_02004584
	movs r1, #204
	adds r0, #85
	lsls r1, r1, #6
	strb r5, [r0]
	adds r1, #51
	ldr r0, .L_02008acc
	bl Func_0200456c
	movs r0, #138
	movs r1, #1
	movs r2, #194
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #17
	lsls r0, r0, #18
	bl Func_02004574
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #8
	ldr r1, .L_02008ad0
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r1, #128
	lsls r1, r1, #2
	movs r2, #192
	movs r0, #8
	adds r1, #14
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #128
	movs r0, #9
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02004544
	movs r1, #135
	movs r2, #194
	lsls r2, r2, #1
	movs r0, #8
	lsls r1, r1, #2
	bl ObjectMotion_SetPositionAndReset
	movs r0, #8
	movs r1, #0
	bl Func_0200454c
	movs r1, #4
	movs r0, #8
	bl Object_SetModeById
	ldr r0, .L_02008ad4
	bl Func_0200451c
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #8
	movs r1, #0
	bl Func_02004534
	movs r1, #2
	movs r2, #0
	adds r1, #255
	movs r0, #9
	bl Func_0200455c
	movs r0, #9
	movs r1, #0
	bl Func_02004534
	movs r0, #8
	movs r1, #3
	bl Object_SetModeById
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #8
	movs r1, #0
	bl Func_02004534
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #20
	movs r0, #9
	bl Func_0200455c
	movs r1, #176
	movs r2, #0
	movs r0, #9
	lsls r1, r1, #8
	bl Func_02004544
	movs r0, #9
	movs r1, #0
	bl Func_02004534
	movs r1, #176
	movs r2, #40
	movs r0, #8
	lsls r1, r1, #8
	bl Func_02004544
	movs r0, #8
	movs r1, #0
	bl Func_0200454c
	movs r0, #8
	movs r1, #4
	bl Object_SetModeById
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #8
	movs r1, #0
	bl Func_02004534
	movs r1, #128
	movs r0, #9
	lsls r1, r1, #8
	bl Func_0200454c
	movs r0, #9
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #9
	movs r1, #0
	bl Func_02004534
	movs r1, #10
	movs r2, #20
	adds r1, #255
	movs r0, #8
	bl Func_0200455c
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #8
	movs r1, #0
	bl Func_02004534
	movs r0, #9
	movs r1, #3
	bl Object_SetModeById
	movs r0, #9
	movs r1, #0
	bl Func_02004534
	movs r1, #176
	movs r2, #0
	movs r0, #9
	lsls r1, r1, #8
	bl Func_02004544
	movs r1, #176
	movs r0, #8
	lsls r1, r1, #8
	bl Func_0200454c
	movs r0, #156
	lsls r0, r0, #4
	bl Func_020043b4
	bl Func_0200446c
.L_02008ac8:
	add sp, #8
	pop {r5, pc}
.L_02008acc:
	.4byte 0x00019999
.L_02008ad0:
	.4byte 0x00013333
.L_02008ad4:
	.4byte 0x00002d0d
	.section .text.x02008ad8,"ax",%progbits
	.global Func_02000ad8
	.thumb_func
Func_02000ad8:
	push {lr}
	bl Func_02004464
	movs r0, #0
	bl Func_020045ec
	movs r1, #153
	lsls r1, r1, #8
	ldr r0, .L_02008da4
	adds r1, #153
	bl Func_0200456c
	movs r0, #160
	movs r1, #1
	movs r2, #160
	movs r3, #1
	lsls r2, r2, #16
	negs r1, r1
	lsls r0, r0, #16
	bl Func_02004574
	bl Func_0200457c
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #10
	movs r1, #1
	bl Motion_SetVarCbAndRefresh
	movs r1, #176
	lsls r1, r1, #8
	movs r0, #10
	bl Func_0200454c
	ldr r0, .L_02008da8
	bl Func_0200451c
	movs r0, #10
	movs r1, #0
	bl Func_02004534
	movs r1, #129
	movs r0, #12
	lsls r1, r1, #1
	bl Func_02004564
	movs r1, #129
	movs r0, #13
	lsls r1, r1, #1
	bl Func_02004564
	movs r1, #129
	movs r0, #14
	lsls r1, r1, #1
	bl Func_02004564
	movs r1, #129
	movs r0, #15
	lsls r1, r1, #1
	bl Func_02004564
	movs r1, #129
	movs r0, #16
	lsls r1, r1, #1
	bl Func_02004564
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #17
	bl Func_02004564
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #160
	movs r0, #10
	lsls r1, r1, #7
	bl Func_0200454c
	movs r0, #10
	movs r1, #0
	bl Func_02004534
	movs r0, #10
	movs r1, #0
	bl Func_02004534
	movs r1, #129
	movs r0, #12
	lsls r1, r1, #1
	bl Func_02004564
	movs r1, #129
	movs r0, #13
	lsls r1, r1, #1
	bl Func_02004564
	movs r1, #129
	movs r0, #14
	lsls r1, r1, #1
	bl Func_02004564
	movs r1, #129
	movs r0, #15
	lsls r1, r1, #1
	bl Func_02004564
	movs r1, #129
	movs r0, #16
	lsls r1, r1, #1
	bl Func_02004564
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #17
	bl Func_02004564
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #12
	movs r1, #4
	bl Object_SetModeById
	movs r0, #12
	movs r1, #0
	bl Func_02004534
	movs r1, #192
	movs r0, #10
	lsls r1, r1, #6
	movs r2, #0
	bl Func_02004544
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #1
	movs r0, #10
	bl Func_0200455c
	movs r0, #13
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #13
	movs r1, #0
	bl Func_02004534
	movs r2, #10
	movs r0, #15
	movs r1, #4
	bl ObjectMotion_Launch
	movs r0, #15
	movs r1, #0
	bl Func_02004534
	movs r1, #160
	movs r0, #10
	lsls r1, r1, #7
	movs r2, #0
	bl Func_02004544
	movs r1, #6
	movs r2, #40
	adds r1, #255
	movs r0, #10
	bl Func_0200455c
	movs r0, #10
	movs r1, #0
	bl Func_02004534
	movs r1, #8
	movs r2, #20
	adds r1, #255
	movs r0, #16
	bl Func_0200455c
	movs r0, #16
	movs r1, #0
	bl Func_02004534
	movs r2, #0
	movs r0, #14
	movs r1, #2
	bl ObjectMotion_Launch
	movs r0, #14
	movs r1, #3
	bl Object_SetModeById
	movs r0, #14
	movs r1, #0
	bl Func_02004534
	movs r0, #11
	movs r1, #0
	bl Func_02004534
	movs r1, #128
	movs r0, #10
	lsls r1, r1, #8
	bl Func_0200454c
	movs r1, #132
	movs r2, #40
	lsls r1, r1, #1
	movs r0, #10
	bl Func_0200455c
	movs r1, #160
	movs r0, #10
	lsls r1, r1, #7
	bl Func_0200454c
	movs r0, #10
	movs r1, #0
	bl Func_02004534
	movs r0, #11
	movs r1, #0
	bl Func_0200454c
	movs r0, #11
	movs r1, #0
	bl Func_02004534
	movs r1, #128
	movs r2, #20
	lsls r1, r1, #1
	movs r0, #10
	bl Func_0200455c
	movs r1, #128
	movs r0, #10
	lsls r1, r1, #8
	bl Func_0200454c
	movs r0, #11
	movs r1, #4
	bl Object_SetModeById
	movs r0, #11
	movs r1, #0
	bl Func_02004534
	movs r1, #129
	movs r0, #10
	lsls r1, r1, #1
	bl Func_02004564
	movs r1, #128
	movs r0, #12
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02004544
	movs r0, #13
	movs r1, #0
	movs r2, #0
	bl Func_02004544
	movs r1, #128
	movs r0, #14
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02004544
	movs r0, #15
	movs r1, #0
	movs r2, #0
	bl Func_02004544
	movs r1, #128
	movs r0, #16
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02004544
	movs r0, #17
	movs r1, #0
	movs r2, #40
	bl Func_02004544
	movs r1, #176
	movs r0, #12
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02004544
	movs r1, #176
	movs r0, #13
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02004544
	movs r1, #208
	movs r0, #14
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02004544
	movs r1, #208
	movs r0, #15
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02004544
	movs r1, #208
	movs r0, #16
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02004544
	movs r1, #208
	movs r2, #20
	movs r0, #17
	lsls r1, r1, #8
	bl Func_02004544
	movs r0, #10
	movs r1, #0
	bl Func_02004534
	movs r1, #10
	movs r2, #40
	adds r1, #255
	movs r0, #11
	bl Func_0200455c
	movs r0, #11
	movs r1, #0
	bl Func_02004534
	movs r1, #176
	movs r2, #80
	movs r0, #10
	lsls r1, r1, #8
	bl Func_02004544
	movs r0, #10
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r1, #0
	movs r0, #10
	bl Func_02004534
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #193
	bl Func_020043b4
	bl Func_0200446c
	pop {pc}
.L_02008da4:
	.4byte 0x0004cccc
.L_02008da8:
	.4byte 0x00002d23
	.section .text.x02008dac,"ax",%progbits
	.global Func_02000dac
	.thumb_func
Func_02000dac:
	push {r5, r6, lr}
	bl Func_02004464
	movs r0, #0
	bl Func_020045ec
	movs r0, #204
	movs r1, #200
	lsls r0, r0, #8
	lsls r1, r1, #5
	adds r0, #204
	adds r1, #153
	bl Func_0200456c
	movs r0, #160
	movs r1, #1
	movs r2, #160
	lsls r0, r0, #16
	negs r1, r1
	lsls r2, r2, #16
	movs r3, #1
	bl Func_02004574
	ldr r5, .L_02009124
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
	ldr r0, [r5]
	movs r2, #156
	movs r1, #134
	bl ObjectMotion_SetPositionAndReset
	movs r1, #192
	ldr r0, [r5]
	lsls r1, r1, #8
	bl Func_0200454c
	movs r1, #128
	movs r2, #20
	lsls r1, r1, #1
	movs r0, #11
	bl Func_0200455c
	movs r1, #160
	lsls r1, r1, #7
	movs r0, #11
	bl Func_0200454c
	ldr r0, .L_02009128
	bl Func_0200451c
	movs r0, #11
	movs r1, #0
	bl Func_02004534
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #10
	bl Func_0200455c
	movs r1, #128
	movs r2, #40
	movs r0, #10
	lsls r1, r1, #8
	bl Func_02004544
	movs r0, #10
	movs r1, #0
	bl Func_02004534
	ldr r0, [r5]
	movs r1, #144
	movs r2, #154
	bl ObjectMotion_SetPositionAndReset
	movs r1, #192
	movs r2, #0
	ldr r0, [r5]
	lsls r1, r1, #8
	bl Func_02004544
	ldr r1, [r5]
	movs r0, #18
	bl Func_020044dc
	ldr r1, [r5]
	movs r0, #0
	bl Func_020044dc
	movs r0, #1
	bl WaitFrames
	movs r1, #176
	movs r0, #14
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02004544
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #18
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	adds r2, #102
	movs r0, #0
	adds r1, #204
	bl ObjectMotion_SetSpeedParameters
	ldr r1, .L_0200912c
	movs r0, #0
	bl ObjectMotion_EnableActionAndSetCallback
	ldr r1, .L_02009130
	movs r0, #18
	bl Object_SetActionCallbackAndRefreshById
	movs r1, #192
	movs r0, #11
	lsls r1, r1, #6
	movs r2, #0
	bl Func_02004544
	movs r1, #132
	movs r2, #20
	lsls r1, r1, #1
	movs r0, #11
	bl Func_0200455c
	movs r0, #11
	movs r1, #0
	bl Func_02004534
	movs r0, #18
	movs r1, #3
	bl Object_SetModeById
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #18
	movs r1, #0
	bl Func_02004534
	movs r0, #0
	movs r1, #3
	bl Object_SetModeById
	movs r0, #160
	lsls r0, r0, #8
	movs r1, #0
	bl Func_02004534
	movs r0, #11
	movs r1, #0
	movs r2, #0
	bl Func_02004544
	movs r1, #128
	movs r2, #40
	movs r0, #10
	lsls r1, r1, #8
	bl Func_02004544
	movs r1, #192
	movs r0, #11
	lsls r1, r1, #6
	bl Func_0200454c
	movs r0, #11
	movs r1, #0
	bl Func_02004534
	movs r1, #160
	movs r0, #10
	lsls r1, r1, #7
	bl Func_0200454c
	movs r0, #10
	movs r1, #0
	bl Func_02004534
	movs r1, #128
	movs r0, #12
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02004544
	movs r0, #13
	movs r1, #0
	movs r2, #0
	bl Func_02004544
	movs r1, #128
	movs r0, #14
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02004544
	movs r0, #15
	movs r1, #0
	movs r2, #0
	bl Func_02004544
	movs r1, #128
	movs r0, #16
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02004544
	movs r2, #0
	movs r0, #17
	movs r1, #0
	bl Func_02004544
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #7
	bl Func_0200454c
	ldr r1, .L_02009134
	ldr r2, .L_02009138
	movs r0, #0
	bl ObjectMotion_SetSpeedParameters
	movs r0, #0
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	strb r3, [r0]
	movs r1, #160
	movs r2, #140
	movs r0, #0
	bl ObjectMotion_SetPositionAndReset
	movs r0, #1
	bl Battle_WaitMode0
	movs r0, #0
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	strb r3, [r0]
	movs r1, #21
	movs r0, #0
	bl Motion_SetModeAndWaitAnimation
	movs r1, #176
	movs r0, #12
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02004544
	movs r1, #176
	movs r0, #13
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02004544
	movs r1, #176
	movs r0, #14
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02004544
	movs r1, #208
	movs r0, #15
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02004544
	movs r1, #208
	movs r0, #16
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02004544
	movs r1, #208
	movs r0, #17
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02004544
	movs r1, #224
	movs r2, #0
	ldr r0, [r5]
	lsls r1, r1, #8
	bl Func_02004544
	movs r1, #176
	movs r0, #18
	lsls r1, r1, #8
	bl Func_0200454c
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #18
	movs r1, #0
	bl Func_02004534
	movs r1, #0
	movs r0, #0
	bl Object_SetModeById
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #1
	movs r0, #0
	bl Object_SetModeById
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #6
	bl Func_0200454c
	movs r0, #18
	movs r1, #3
	bl Object_SetModeById
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #18
	movs r1, #0
	bl Func_02004534
	movs r1, #192
	ldr r0, [r5]
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02004544
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #0
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r0, #0
	movs r1, #160
	movs r2, #154
	bl ObjectMotion_SetPositionAndReset
	movs r1, #160
	movs r2, #0
	movs r0, #0
	lsls r1, r1, #8
	bl Func_02004544
	movs r0, #18
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #18
	movs r1, #0
	bl Func_02004534
	movs r1, #10
	movs r2, #20
	adds r1, #255
	movs r0, #18
	bl Func_0200455c
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #18
	movs r1, #0
	bl Func_02004534
	movs r1, #6
	adds r1, #255
	movs r2, #0
	movs r0, #11
	bl Func_0200455c
	movs r1, #6
	adds r1, #255
	movs r2, #60
	movs r0, #10
	bl Func_0200455c
	movs r1, #10
	movs r2, #20
	adds r1, #255
	movs r0, #0
	bl Func_0200455c
	movs r0, #160
	lsls r0, r0, #8
	movs r1, #0
	bl Func_02004534
	ldr r0, [r5]
	movs r1, #0
	movs r2, #0
	bl Func_02004544
	movs r1, #128
	movs r2, #40
	movs r0, #18
	lsls r1, r1, #8
	bl Func_02004544
	movs r0, #0
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #160
	lsls r0, r0, #8
	movs r1, #0
	bl Func_02004534
	movs r5, #0
	b .L_0200913e
.L_02009124:
	.4byte gPartyState
.L_02009128:
	.4byte 0x00002d3f
.L_0200912c:
	.4byte Data_020049c4
.L_02009130:
	.4byte Data_02004980
.L_02009134:
	.4byte 0x00026666
.L_02009138:
	.4byte 0x00013333
.L_0200913c:
	adds r5, #1
.L_0200913e:
	cmp r5, #79
	bhi .L_02009150
	movs r0, #1
	bl WaitFrames
	ldr r3, .L_02009364
	ldr r3, [r3]
	cmp r3, #0
	beq .L_0200913c
.L_02009150:
	movs r0, #12
	movs r1, #1
	bl Motion_SetVarCbAndRefresh
	movs r0, #12
	movs r1, #0
	bl Func_02004534
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #1
	movs r0, #0
	bl Func_0200455c
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #6
	bl Func_0200454c
	movs r0, #0
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r1, #10
	movs r2, #20
	adds r1, #255
	movs r0, #15
	bl Func_0200455c
	movs r0, #15
	movs r1, #0
	bl Func_02004534
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #7
	bl Func_0200454c
	movs r0, #0
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r1, #128
	movs r2, #0
	movs r0, #10
	lsls r1, r1, #8
	bl Func_02004544
	movs r0, #11
	movs r1, #0
	bl Func_0200454c
	movs r0, #13
	movs r1, #3
	bl Object_SetModeById
	movs r0, #13
	movs r1, #0
	bl Func_02004534
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #6
	bl Func_0200454c
	movs r0, #0
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r1, #192
	movs r0, #11
	lsls r1, r1, #6
	movs r2, #0
	bl Func_02004544
	movs r1, #160
	movs r2, #0
	movs r0, #10
	lsls r1, r1, #7
	bl Func_02004544
	movs r0, #16
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #16
	movs r1, #0
	bl Func_02004534
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #7
	bl Func_0200454c
	movs r1, #132
	movs r2, #20
	lsls r1, r1, #1
	movs r0, #0
	bl Func_0200455c
	movs r0, #14
	movs r1, #3
	bl Object_SetModeById
	movs r0, #14
	movs r1, #0
	bl Func_02004534
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #7
	bl Func_0200454c
	movs r0, #0
	movs r1, #3
	bl Object_SetModeById
	movs r0, #17
	movs r1, #4
	bl Object_SetModeById
	movs r0, #17
	movs r1, #0
	bl Func_02004534
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #7
	movs r2, #20
	bl Func_02004544
	ldr r5, .L_02009368
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	movs r1, #192
	ldr r0, [r5]
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02004544
	movs r1, #176
	movs r0, #18
	lsls r1, r1, #8
	movs r2, #20
	bl Func_02004544
	movs r1, #10
	movs r2, #40
	adds r1, #255
	movs r0, #11
	bl Func_0200455c
	movs r0, #11
	movs r1, #0
	bl Func_02004534
	movs r1, #192
	movs r2, #40
	movs r0, #0
	lsls r1, r1, #8
	bl Func_02004544
	movs r0, #10
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	movs r1, #0
	bl Func_02004534
	movs r1, #208
	movs r0, #18
	lsls r1, r1, #8
	bl Func_0200454c
	movs r0, #18
	movs r1, #3
	bl Object_SetModeById
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #18
	movs r1, #0
	bl Func_02004534
	movs r0, #11
	movs r1, #3
	bl Object_SetModeById
	movs r0, #11
	movs r1, #0
	bl Func_02004534
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	bl Func_0200454c
	movs r0, #160
	lsls r0, r0, #8
	movs r1, #0
	bl Func_02004534
	movs r0, #10
	movs r1, #3
	bl Object_SetModeById
	movs r0, #10
	movs r1, #0
	bl Func_02004534
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #18
	movs r1, #0
	bl Func_02004534
	movs r0, #11
	movs r1, #4
	bl Object_SetModeById
	movs r0, #11
	movs r1, #0
	bl Func_02004534
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #8
	bl Func_0200454c
	movs r0, #160
	movs r1, #0
	lsls r0, r0, #8
	bl Func_02004524
	ldr r0, [r5]
	movs r1, #0
	movs r2, #0
	bl Func_02004544
	ldr r0, [r5]
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	ldr r5, [r5]
	cmp r0, #0
	bne .L_0200936c
	adds r0, r5, #0
	bl ObjectMotion_WaitForAnimationChange
	movs r0, #160
	lsls r0, r0, #8
	movs r1, #0
	bl Func_02004534
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_0200938e
.L_02009364:
	.4byte gInput
.L_02009368:
	.4byte gPartyState
.L_0200936c:
	adds r0, r5, #0
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #160
	adds r3, #1
	strh r3, [r2]
	lsls r0, r0, #8
	movs r1, #0
	bl Func_02004534
.L_0200938e:
	movs r1, #132
	movs r2, #0
	lsls r1, r1, #1
	movs r0, #18
	bl Func_0200455c
	movs r1, #128
	movs r0, #18
	lsls r1, r1, #8
	bl Func_0200454c
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #18
	movs r1, #0
	bl Func_02004534
	movs r2, #0
	movs r0, #11
	movs r1, #0
	bl Func_02004544
	movs r1, #128
	movs r0, #10
	lsls r1, r1, #8
	bl Func_0200454c
	movs r0, #11
	movs r1, #3
	bl Object_SetModeById
	movs r0, #10
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r1, #192
	movs r2, #0
	movs r0, #11
	lsls r1, r1, #6
	bl Func_02004544
	movs r1, #160
	movs r0, #10
	lsls r1, r1, #7
	bl Func_0200454c
	movs r1, #132
	movs r2, #20
	lsls r1, r1, #1
	movs r0, #10
	bl Func_0200455c
	movs r0, #10
	movs r1, #0
	bl Func_02004534
	movs r0, #11
	movs r1, #3
	bl Object_SetModeById
	movs r0, #11
	movs r1, #0
	bl Func_02004534
	ldr r6, .L_020095c8
	movs r3, #133
	lsls r3, r3, #2
	adds r6, r6, r3
	movs r1, #192
	ldr r0, [r6]
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02004544
	movs r1, #160
	movs r2, #0
	movs r0, #0
	lsls r1, r1, #8
	bl Func_02004544
	movs r1, #160
	movs r0, #18
	lsls r1, r1, #8
	bl Func_0200454c
	ldr r0, [r6]
	movs r1, #3
	bl Object_SetModeById
	movs r0, #0
	movs r1, #3
	bl Object_SetModeById
	movs r0, #18
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r1, #128
	movs r0, #12
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02004544
	movs r0, #13
	movs r1, #0
	movs r2, #0
	bl Func_02004544
	movs r1, #128
	movs r0, #14
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02004544
	movs r0, #15
	movs r1, #0
	movs r2, #0
	bl Func_02004544
	movs r1, #128
	movs r2, #0
	movs r0, #16
	lsls r1, r1, #8
	bl Func_02004544
	movs r0, #17
	movs r1, #0
	bl Func_0200454c
	movs r0, #12
	movs r1, #3
	bl Object_SetModeById
	movs r0, #13
	movs r1, #3
	bl Object_SetModeById
	movs r0, #14
	movs r1, #3
	bl Object_SetModeById
	movs r0, #15
	movs r1, #3
	bl Object_SetModeById
	movs r0, #16
	movs r1, #3
	bl Object_SetModeById
	movs r0, #17
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r1, #128
	movs r0, #18
	lsls r1, r1, #8
	bl Func_0200454c
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #18
	movs r1, #0
	bl Func_02004534
	movs r2, #0
	ldr r0, [r6]
	movs r1, #0
	bl Func_02004544
	movs r0, #0
	movs r1, #0
	bl Func_0200454c
	ldr r0, [r6]
	movs r1, #3
	bl Object_SetModeById
	movs r0, #0
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	ldr r5, .L_020095cc
	movs r0, #0
	adds r1, r5, #0
	bl ObjectMotion_EnableActionAndSetCallback
	adds r1, r5, #0
	movs r0, #18
	bl Object_SetActionCallbackAndRefreshById
	movs r2, #154
	ldr r0, [r6]
	movs r1, #160
	bl ObjectMotion_SetPositionAndReset
	movs r1, #192
	ldr r0, [r6]
	lsls r1, r1, #8
	bl Func_0200454c
	movs r0, #10
	movs r1, #1
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	movs r1, #0
	bl Func_02004534
	movs r1, #2
	ldr r0, [r6]
	adds r1, #255
	movs r2, #0
	bl Func_0200455c
	movs r1, #224
	movs r2, #20
	ldr r0, [r6]
	lsls r1, r1, #8
	bl Func_02004544
	movs r0, #10
	movs r1, #4
	bl Object_SetModeById
	movs r0, #10
	movs r1, #0
	bl Func_02004534
	movs r1, #128
	movs r2, #20
	lsls r1, r1, #1
	movs r0, #11
	bl Func_0200455c
	movs r0, #11
	movs r1, #0
	bl Func_0200454c
	movs r0, #11
	movs r1, #0
	bl Func_02004534
	movs r1, #128
	movs r0, #10
	lsls r1, r1, #8
	bl Func_0200454c
	movs r0, #10
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r1, #160
	movs r0, #10
	lsls r1, r1, #7
	bl Func_0200454c
	movs r0, #10
	movs r1, #0
	bl Func_02004534
	ldr r0, [r6]
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r1, #192
	movs r0, #11
	lsls r1, r1, #6
	bl Func_0200454c
	movs r0, #11
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r1, #0
	movs r0, #11
	bl Func_02004534
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #194
	bl Func_020043b4
	bl Func_0200446c
	pop {r5, r6, pc}
.L_020095c8:
	.4byte gPartyState
.L_020095cc:
	.4byte Data_02004a08
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
	bl ObjectMotion_SetSpeedParameters
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #9
	lsls r1, r1, #10
	movs r0, #20
	bl ObjectMotion_SetSpeedParameters
	movs r0, #20
	bl Object_GetById
	movs r3, #1
	mov r8, r3
	adds r0, #34
	movs r3, #1
	strb r3, [r0]
	movs r1, #3
	movs r0, #20
	bl Func_02004554
	ldr r0, [r6]
	bl Object_GetById
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
	bl ObjectMotion_SetPositionAndReset
	movs r0, #1
	bl Battle_WaitMode0
	ldr r0, [r6]
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	mov r2, r8
	orrs r3, r2
	strb r3, [r0]
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	ldr r0, [r6]
	bl Func_02004544
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
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	movs r1, #204
	ands r5, r3
	lsls r1, r1, #1
	movs r2, #200
	strb r5, [r0]
	movs r0, #20
	bl ObjectMotion_SetPositionAndReset
	movs r0, #1
	bl Battle_WaitMode0
	movs r0, #20
	bl Object_GetById
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
	bl Battle_WaitMode0
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
	bl Battle_WaitMode0
	movs r0, #146
	bl Func_0200463c
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #6
	movs r2, #40
	ldr r0, [r6]
	adds r1, #255
	bl Func_0200455c
	movs r1, #2
	movs r0, #20
	bl Func_02004554
	movs r0, #20
	bl Object_GetById
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
	bl Func_020043b4
	bl Func_0200446c
	add sp, #8
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, pc}
.L_0200976c:
	.4byte gPartyState
	.section .text.x02009770,"ax",%progbits
	.global Func_02001770
	.thumb_func
Func_02001770:
	push {r5, lr}
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #61
	bl Func_020043ac
	cmp r0, #0
	bne .L_020097dc
	movs r0, #64
	bl Func_020043ac
	cmp r0, #0
	bne .L_020097dc
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #108]
	bl Func_02004464
	movs r0, #1
	bl Func_020045ec
	movs r3, #128
	movs r1, #204
	movs r2, #145
	lsls r3, r3, #7
	lsls r1, r1, #17
	lsls r2, r2, #16
	movs r0, #20
	bl Func_020044d4
	movs r0, #1
	bl WaitFrames
	movs r0, #20
	movs r1, #4
	movs r2, #20
	bl ObjectMotion_Launch
	ldr r3, .L_020097e0
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r1, [r3]
	movs r2, #0
	movs r0, #20
	bl Func_0200450c
	movs r3, #181
	lsls r3, r3, #1
	adds r2, r5, r3
	movs r3, #200
	strh r3, [r2]
	bl Func_0200446c
.L_020097dc:
	pop {r5, pc}
	.2byte 0x0000
.L_020097e0:
	.4byte gPartyState
	.section .text.x020097e4,"ax",%progbits
	.global Func_020017e4
	.thumb_func
Func_020017e4:
	push {lr}
	sub sp, #8
	movs r3, #25
	movs r2, #9
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #25
	movs r1, #2
	movs r2, #1
	movs r3, #1
	bl Func_0200442c
	add sp, #8
	pop {pc}
	.section .text.x02009800,"ax",%progbits
	.global Func_02001800
	.thumb_func
Func_02001800:
	push {lr}
	sub sp, #8
	movs r3, #25
	movs r2, #9
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #25
	movs r1, #3
	movs r2, #1
	movs r3, #1
	bl Func_0200442c
	add sp, #8
	pop {pc}
	.section .text.x0200981c,"ax",%progbits
	.global Func_0200181c
	.thumb_func
Func_0200181c:
	push {lr}
	adds r1, r0, #0
	adds r1, #100
	movs r3, #0
	ldrsh r2, [r1, r3]
	ldr r3, [r0, #8]
	lsls r2, r2, #8
	adds r3, r3, r2
	str r3, [r0, #8]
	ldr r3, [r0, #12]
	movs r2, #128
	lsls r2, r2, #8
	adds r3, r3, r2
	str r3, [r0, #12]
	movs r2, #160
	ldr r3, [r0, #24]
	lsls r2, r2, #3
	adds r2, #30
	adds r3, r3, r2
	str r3, [r0, #24]
	ldr r3, [r0, #28]
	adds r3, r3, r2
	str r3, [r0, #28]
	ldrh r3, [r1]
	adds r3, #2
	strh r3, [r1]
	ldr r3, [r0, #104]
	subs r3, #1
	str r3, [r0, #104]
	cmp r3, #0
	bne .L_0200985e
	bl Func_020043e4
.L_0200985e:
	pop {pc}
	.section .text.x02009860,"ax",%progbits
	.global Func_02001860
	.thumb_func
Func_02001860:
	push {r5, lr}
	adds r4, r0, #0
	adds r5, r1, #0
	movs r0, #30
	adds r3, r2, #0
	adds r0, #255
	adds r2, r5, #0
	adds r1, r4, #0
	bl Func_020043dc
	adds r5, r0, #0
	cmp r5, #0
	beq .L_020098d8
	adds r2, r5, #0
	adds r2, #92
	movs r3, #2
	strb r3, [r2]
	adds r3, r5, #0
	movs r1, #0
	adds r3, #85
	strb r1, [r3]
	adds r3, #15
	strh r1, [r3]
	adds r1, r5, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #254
	ands r3, r2
	strb r3, [r1]
	movs r1, #7
	bl Func_02004514
	ldr r1, [r5, #80]
	movs r3, #13
	ldrb r2, [r1, #9]
	negs r3, r3
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	strb r3, [r1, #9]
	adds r0, r5, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r5, #24]
	str r3, [r5, #28]
	movs r3, #60
	str r3, [r5, #104]
	ldr r3, .L_020098dc
	adds r0, r5, #0
	movs r1, #5
	str r3, [r5, #108]
	bl Func_020043c4
	adds r0, r5, #0
	movs r1, #1
	bl Animation_SetStateFlags
.L_020098d8:
	pop {r5, pc}
	.2byte 0x0000
.L_020098dc:
	.4byte Func_0200181c
	.section .text.x020098e0,"ax",%progbits
	.global Func_020018e0
	.thumb_func
Func_020018e0:
	push {r5, r6, lr}
	ldr r3, .L_02009994
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02009998
	cmp r2, r3
	bne .L_02009970
	ldr r5, .L_0200999c
	movs r6, #63
	ldr r3, [r5]
	ands r3, r6
	cmp r3, #0
	bne .L_02009910
	movs r0, #134
	movs r1, #160
	movs r2, #232
	lsls r0, r0, #18
	lsls r1, r1, #14
	lsls r2, r2, #16
	bl Func_02001860
.L_02009910:
	ldr r3, [r5]
	adds r3, #60
	ands r3, r6
	cmp r3, #0
	bne .L_0200992a
	movs r0, #168
	movs r1, #160
	movs r2, #148
	lsls r0, r0, #16
	lsls r1, r1, #14
	lsls r2, r2, #17
	bl Func_02001860
.L_0200992a:
	ldr r3, [r5]
	adds r3, #20
	ands r3, r6
	cmp r3, #0
	bne .L_02009954
	movs r0, #140
	movs r1, #168
	movs r2, #184
	lsls r0, r0, #17
	lsls r1, r1, #14
	lsls r2, r2, #16
	bl Func_02001860
	movs r0, #152
	movs r1, #192
	movs r2, #212
	lsls r0, r0, #16
	lsls r1, r1, #14
	lsls r2, r2, #17
	bl Func_02001860
.L_02009954:
	ldr r3, [r5]
	adds r3, #40
	ands r3, r6
	cmp r3, #0
	bne .L_02009992
	movs r0, #168
	movs r1, #168
	movs r2, #181
	lsls r0, r0, #17
	lsls r1, r1, #14
	lsls r2, r2, #17
	bl Func_02001860
	b .L_02009992
.L_02009970:
	ldr r3, .L_020099a0
	cmp r2, r3
	bne .L_02009992
	ldr r3, .L_0200999c
	movs r2, #63
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	bne .L_02009992
	movs r0, #142
	movs r1, #208
	movs r2, #148
	lsls r0, r0, #18
	lsls r1, r1, #15
	lsls r2, r2, #17
	bl Func_02001860
.L_02009992:
	pop {r5, r6, pc}
.L_02009994:
	.4byte gPartyState
.L_02009998:
	.4byte 0x0000010e
.L_0200999c:
	.4byte Data_0300122c
.L_020099a0:
	.4byte 0x0000010f
	.section .text.x020099a4,"ax",%progbits
	.global Func_020019a4
	.thumb_func
Func_020019a4:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	mov r10, r3
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	sub sp, #8
	adds r5, r0, #0
	adds r7, r1, #0
	mov r8, r2
	ldr r6, [sp, #36]
	mov r9, r3
	bl Func_02004464
	movs r0, #0
	bl Func_020045ec
	movs r0, #128
	lsls r0, r0, #2
	bl Func_020043ac
	cmp r0, #0
	bne .L_02009a02
	cmp r6, #2
	bne .L_020099e4
	movs r0, #188
	bl Func_0200463c
	b .L_020099ea
.L_020099e4:
	movs r0, #158
	bl Func_0200463c
.L_020099ea:
	ldr r3, [sp, #40]
	adds r0, r5, #0
	str r3, [sp, #4]
	adds r1, r7, #0
	mov r2, r8
	mov r3, r10
	str r6, [sp, #0]
	bl Func_02004404
	movs r0, #20
	bl Battle_WaitMode0
.L_02009a02:
	ldr r3, .L_02009a94
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
	bne .L_02009a54
	ldr r0, [r5]
	bl Object_GetById
	adds r5, r0, #0
	ldr r0, [r5, #8]
	asrs r2, r0, #16
	adds r3, r2, #0
	cmp r2, #0
	bge .L_02009a30
	adds r3, #15
.L_02009a30:
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
	bl Func_020043f4
	adds r0, r5, #0
	bl Func_020043fc
	movs r3, #192
	lsls r3, r3, #8
	strh r3, [r5, #6]
.L_02009a54:
	ldr r3, .L_02009a94
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	ldr r1, .L_02009a98
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #12
	bl WaitFrames
	movs r0, #123
	bl Func_0200463c
	bl Event_ClearStatus1c6
	bl Event_WaitValue1c8Frames
	movs r3, #170
	lsls r3, r3, #1
	add r3, r9
	movs r2, #0
	ldrsh r0, [r3, r2]
	bl Func_0200458c
	add sp, #8
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009a94:
	.4byte gPartyState
.L_02009a98:
	.4byte Data_020046f8
	.section .text.x02009a9c,"ax",%progbits
	.global Func_02001a9c
	.thumb_func
Func_02001a9c:
	push {lr}
	sub sp, #8
	movs r3, #1
	str r3, [sp, #0]
	movs r3, #2
	str r3, [sp, #4]
	movs r0, #48
	movs r1, #39
	movs r2, #57
	movs r3, #26
	bl Func_020019a4
	add sp, #8
	pop {pc}
	.section .text.x02009ab8,"ax",%progbits
	.global Func_02001ab8
	.thumb_func
Func_02001ab8:
	push {lr}
	sub sp, #8
	movs r3, #1
	str r3, [sp, #0]
	movs r3, #2
	str r3, [sp, #4]
	movs r0, #49
	movs r1, #39
	movs r2, #65
	movs r3, #24
	bl Func_020019a4
	add sp, #8
	pop {pc}
	.section .text.x02009ad4,"ax",%progbits
	.global Func_02001ad4
	.thumb_func
Func_02001ad4:
	push {lr}
	sub sp, #8
	movs r3, #1
	str r3, [sp, #0]
	movs r3, #2
	str r3, [sp, #4]
	movs r0, #50
	movs r1, #39
	movs r2, #56
	movs r3, #19
	bl Func_020019a4
	add sp, #8
	pop {pc}
	.section .text.x02009af0,"ax",%progbits
	.global Func_02001af0
	.thumb_func
Func_02001af0:
	push {lr}
	sub sp, #8
	movs r3, #1
	str r3, [sp, #0]
	movs r3, #2
	str r3, [sp, #4]
	movs r0, #51
	movs r1, #39
	movs r2, #61
	movs r3, #13
	bl Func_020019a4
	add sp, #8
	pop {pc}
	.section .text.x02009b0c,"ax",%progbits
	.global Func_02001b0c
	.thumb_func
Func_02001b0c:
	push {lr}
	sub sp, #8
	movs r3, #1
	str r3, [sp, #0]
	movs r3, #2
	str r3, [sp, #4]
	movs r0, #51
	movs r1, #39
	movs r2, #80
	movs r3, #16
	bl Func_020019a4
	add sp, #8
	pop {pc}
	.section .text.x02009b28,"ax",%progbits
	.global Func_02001b28
	.thumb_func
Func_02001b28:
	push {lr}
	sub sp, #8
	movs r3, #2
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #52
	movs r1, #39
	movs r2, #50
	movs r3, #9
	bl Func_020019a4
	add sp, #8
	pop {pc}
	.2byte 0x0000
	.section .text.x02009b44,"ax",%progbits
	.global Func_02001b44
	.thumb_func
Func_02001b44:
	push {lr}
	sub sp, #8
	movs r3, #1
	str r3, [sp, #0]
	movs r3, #2
	str r3, [sp, #4]
	movs r0, #87
	movs r1, #2
	movs r2, #80
	movs r3, #7
	bl Func_020019a4
	add sp, #8
	pop {pc}
	.section .text.x02009b60,"ax",%progbits
	.global Func_02001b60
	.thumb_func
Func_02001b60:
	push {lr}
	sub sp, #8
	movs r3, #1
	str r3, [sp, #0]
	movs r3, #2
	str r3, [sp, #4]
	movs r0, #89
	movs r1, #0
	movs r2, #77
	movs r3, #16
	bl Func_020019a4
	add sp, #8
	pop {pc}
	.section .text.x02009b7c,"ax",%progbits
	.global Func_02001b7c
	.thumb_func
Func_02001b7c:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r0, #170
	lsls r0, r0, #1
	adds r3, r3, r0
	movs r2, #0
	ldrsh r7, [r3, r2]
	ldr r3, .L_02009c1c
	adds r0, #192
	adds r6, r3, r0
	ldr r0, [r6]
	bl Object_GetById
	movs r1, #128
	movs r2, #128
	adds r5, r0, #0
	lsls r2, r2, #7
	ldr r0, [r6]
	lsls r1, r1, #8
	bl ObjectMotion_SetSpeedParameters
	ldr r0, [r5, #8]
	asrs r2, r0, #16
	adds r3, r2, #0
	cmp r2, #0
	bge .L_02009bb6
	adds r3, #15
.L_02009bb6:
	asrs r3, r3, #4
	lsls r3, r3, #4
	subs r3, r2, r3
	movs r1, #8
	subs r1, r1, r3
	lsls r1, r1, #16
	adds r1, r1, r0
	ldr r3, [r5, #16]
	ldr r0, .L_02009c20
	ldr r2, [r5, #12]
	adds r3, r3, r0
	adds r0, r5, #0
	bl Func_020043f4
	adds r0, r5, #0
	bl Func_020043fc
	movs r3, #192
	lsls r3, r3, #8
	strh r3, [r5, #6]
	movs r0, #128
	bl Func_0200463c
	ldr r0, [r6]
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	ldr r0, [r6]
	cmp r7, #6
	bne .L_02009bfe
	movs r1, #25
	bl Object_SetModeById
	b .L_02009c04
.L_02009bfe:
	movs r1, #26
	bl Object_SetModeById
.L_02009c04:
	movs r0, #4
	bl WaitFrames
	bl Event_ClearStatus1c6
	bl Event_WaitValue1c8Frames
	adds r0, r7, #0
	bl Func_0200458c
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009c1c:
	.4byte gPartyState
.L_02009c20:
	.4byte 0xfffc0000
	.section .text.x02009c24,"ax",%progbits
	.global Func_02001c24
	.thumb_func
Func_02001c24:
	push {r5, r6, r7, lr}
	ldr r3, .L_02009ce4
	adds r5, r0, #0
	movs r0, #133
	lsls r0, r0, #2
	adds r7, r3, r0
	ldr r0, [r7]
	bl Object_GetById
	adds r6, r0, #0
	bl Func_02004464
	movs r0, #0
	bl Func_020045ec
	cmp r5, #0
	beq .L_02009c7a
	ldr r0, [r7]
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	ldr r1, [r6, #8]
	ldr r2, .L_02009ce8
	ldr r3, [r6, #16]
	ldr r0, .L_02009cec
	adds r1, r1, r2
	adds r3, r3, r0
	ldr r2, [r6, #12]
	adds r0, r6, #0
	bl Object_SetPositionAndResetMotion
	movs r0, #1
	bl WaitFrames
	bl Event_SetStatus1c6
	ldr r0, [r7]
	movs r1, #23
	bl Motion_SetModeAndWaitAnimation
	b .L_02009ca2
.L_02009c7a:
	ldr r1, [r6, #8]
	ldr r3, [r6, #16]
	ldr r0, .L_02009cec
	movs r2, #128
	lsls r2, r2, #9
	adds r1, r1, r2
	adds r3, r3, r0
	ldr r2, [r6, #12]
	adds r0, r6, #0
	bl Object_SetPositionAndResetMotion
	movs r0, #1
	bl WaitFrames
	bl Event_SetStatus1c6
	ldr r0, [r7]
	movs r1, #24
	bl Motion_SetModeAndWaitAnimation
.L_02009ca2:
	ldr r5, .L_02009ce4
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
	ldr r0, [r5]
	bl Object_GetById
	movs r1, #1
	bl ObjectDispatch_SetSingleChildField26
	movs r1, #128
	movs r2, #128
	ldr r0, [r5]
	lsls r1, r1, #8
	lsls r2, r2, #7
	bl ObjectMotion_SetSpeedParameters
	movs r2, #8
	ldr r0, [r5]
	movs r1, #0
	bl ObjectMotion_OffsetPositionAndResetMotion
	ldr r0, [r5]
	bl ObjectMotion_CommitCurrentPositionAndActivate
	ldr r0, [r5]
	movs r1, #1
	bl Object_SetModeById
	bl Func_0200446c
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009ce4:
	.4byte gPartyState
.L_02009ce8:
	.4byte 0xffff0000
.L_02009cec:
	.4byte 0xfffa0000
	.section .text.x02009cf0,"ax",%progbits
	.global Func_02001cf0
	.thumb_func
Func_02001cf0:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	bl Func_02004464
	movs r0, #0
	bl Func_020045ec
	movs r0, #1
	movs r1, #1
	movs r2, #1
	movs r3, #0
	negs r1, r1
	negs r2, r2
	negs r0, r0
	bl Func_02004574
	movs r0, #20
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #1
	bl WaitFrames
	ldr r5, .L_0200a070
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
	movs r1, #184
	movs r2, #242
	ldr r0, [r5]
	lsls r1, r1, #16
	lsls r2, r2, #17
	bl Func_020044cc
	movs r3, #192
	movs r1, #168
	movs r2, #242
	lsls r3, r3, #8
	movs r0, #0
	lsls r1, r1, #16
	lsls r2, r2, #17
	movs r6, #176
	mov r10, r3
	lsls r6, r6, #8
	bl Func_020044d4
	movs r1, #200
	movs r2, #242
	movs r0, #27
	lsls r1, r1, #16
	lsls r2, r2, #17
	adds r3, r6, #0
	bl Func_020044d4
	movs r2, #192
	lsls r2, r2, #6
	mov r8, r2
	movs r1, #138
	movs r2, #206
	movs r0, #17
	lsls r1, r1, #16
	lsls r2, r2, #17
	mov r3, r8
	bl Func_020044d4
	movs r1, #220
	movs r2, #150
	adds r3, r6, #0
	movs r0, #20
	lsls r1, r1, #16
	lsls r2, r2, #18
	bl Func_020044d4
	movs r1, #184
	movs r2, #171
	lsls r1, r1, #16
	lsls r2, r2, #18
	movs r0, #5
	movs r7, #192
	bl Func_020044cc
	lsls r7, r7, #18
	movs r0, #1
	bl WaitFrames
	ldr r3, [r7, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	subs r2, #172
	str r2, [r3]
	mov r9, r2
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	movs r0, #204
	movs r1, #200
	lsls r0, r0, #8
	lsls r1, r1, #5
	adds r0, #204
	adds r1, #153
	bl Func_0200456c
	movs r0, #184
	movs r1, #1
	movs r2, #224
	movs r3, #1
	lsls r2, r2, #17
	negs r1, r1
	lsls r0, r0, #16
	bl Func_02004574
	bl Func_0200457c
	movs r0, #20
	bl Battle_WaitMode0
	ldr r0, .L_0200a074
	bl Func_0200451c
	movs r0, #26
	movs r1, #0
	bl Func_02004534
	movs r0, #0
	movs r1, #3
	bl Object_SetModeById
	movs r0, #27
	movs r1, #3
	bl Object_SetModeById
	ldr r0, [r5]
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r1, #160
	movs r0, #26
	lsls r1, r1, #7
	movs r2, #0
	bl Func_02004544
	movs r1, #132
	movs r2, #20
	lsls r1, r1, #1
	movs r0, #26
	bl Func_0200455c
	movs r0, #26
	movs r1, #0
	bl Func_02004534
	movs r1, #128
	ldr r0, [r5]
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02004544
	movs r0, #0
	movs r1, #0
	movs r2, #20
	bl Func_02004544
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #0
	ldr r0, [r5]
	bl Func_0200455c
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #40
	movs r0, #0
	bl Func_0200455c
	movs r2, #0
	movs r0, #0
	mov r1, r10
	bl Func_02004544
	ldr r0, [r5]
	mov r1, r10
	bl Func_0200454c
	movs r0, #0
	movs r1, #0
	bl Func_02004534
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #25
	bl Func_02004564
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #25
	movs r1, #0
	bl Func_02004534
	movs r0, #27
	movs r1, #4
	bl Object_SetModeById
	movs r0, #27
	movs r1, #0
	bl Func_02004534
	movs r0, #19
	movs r1, #4
	bl Object_SetModeById
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #19
	movs r1, #0
	bl Func_02004534
	movs r1, #10
	movs r2, #20
	adds r1, #255
	movs r0, #0
	bl Func_0200455c
	movs r0, #0
	movs r1, #0
	bl Func_02004534
	movs r1, #128
	movs r0, #27
	lsls r1, r1, #8
	movs r2, #20
	bl Func_02004544
	movs r1, #132
	movs r2, #0
	lsls r1, r1, #1
	movs r0, #27
	bl Func_0200455c
	movs r0, #27
	movs r1, #0
	bl Func_02004534
	movs r0, #17
	movs r1, #0
	bl Func_02004534
	movs r0, #27
	adds r1, r6, #0
	movs r2, #0
	bl Func_02004544
	movs r1, #10
	movs r2, #20
	adds r1, #255
	movs r0, #27
	bl Func_0200455c
	movs r0, #27
	movs r1, #0
	bl Func_02004534
	movs r0, #26
	mov r1, r8
	bl Func_0200454c
	movs r0, #26
	movs r1, #4
	bl Object_SetModeById
	movs r0, #26
	movs r1, #0
	bl Func_02004534
	movs r0, #0
	movs r1, #0
	bl Func_02004534
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #19
	bl Func_02004564
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #19
	movs r1, #0
	bl Func_02004534
	movs r1, #224
	movs r0, #0
	lsls r1, r1, #8
	bl Func_0200454c
	movs r0, #0
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #0
	movs r1, #0
	bl Func_02004534
	movs r1, #160
	movs r2, #0
	movs r0, #26
	lsls r1, r1, #7
	bl Func_02004544
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #26
	bl Func_02004564
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #26
	movs r1, #0
	bl Func_02004534
	movs r1, #10
	movs r2, #20
	adds r1, #255
	movs r0, #27
	bl Func_0200455c
	movs r0, #27
	movs r1, #0
	bl Func_02004534
	movs r0, #25
	mov r1, r8
	movs r2, #0
	bl Func_02004544
	movs r1, #4
	movs r2, #20
	adds r1, #255
	movs r0, #25
	bl Func_0200455c
	movs r0, #25
	movs r1, #0
	bl Func_02004534
	movs r2, #20
	mov r1, r9
	movs r0, #27
	bl Func_0200455c
	movs r0, #27
	movs r1, #0
	bl Func_02004534
	movs r2, #0
	movs r0, #26
	mov r1, r8
	bl Func_02004544
	movs r0, #26
	movs r1, #0
	bl Func_02004534
	movs r2, #0
	movs r0, #0
	mov r1, r10
	bl Func_02004544
	movs r0, #0
	movs r1, #0
	bl Func_02004534
	movs r1, #160
	movs r0, #25
	lsls r1, r1, #7
	movs r2, #0
	bl Func_02004544
	movs r1, #6
	movs r2, #20
	adds r1, #255
	movs r0, #25
	bl Func_0200455c
	movs r0, #25
	movs r1, #0
	bl Func_02004534
	movs r0, #26
	movs r1, #4
	bl Object_SetModeById
	movs r2, #20
	movs r0, #26
	movs r1, #0
	bl Func_0200452c
	movs r0, #0
	movs r1, #0
	bl Func_0200454c
	movs r1, #132
	movs r2, #0
	lsls r1, r1, #1
	movs r0, #0
	bl Func_0200455c
	movs r0, #128
	movs r1, #0
	lsls r0, r0, #7
	bl Func_02004524
	movs r1, #128
	ldr r0, [r5]
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02004544
	ldr r0, [r5]
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_0200a078
	movs r0, #26
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #26
	movs r1, #0
	bl Func_02004534
	ldr r2, [r7, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_0200a096
	.2byte 0x0000
.L_0200a070:
	.4byte gPartyState
.L_0200a074:
	.4byte 0x00002e70
.L_0200a078:
	ldr r2, [r7, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #26
	adds r3, #1
	movs r1, #4
	strh r3, [r2]
	bl Motion_SetModeAndWaitAnimation
	movs r0, #26
	movs r1, #0
	bl Func_02004534
.L_0200a096:
	movs r1, #128
	movs r0, #27
	lsls r1, r1, #8
	bl Func_0200454c
	movs r0, #27
	movs r1, #4
	bl Object_SetModeById
	movs r0, #27
	movs r1, #0
	bl Func_02004534
	movs r1, #192
	movs r0, #25
	lsls r1, r1, #6
	movs r2, #0
	bl Func_02004544
	movs r1, #132
	movs r2, #20
	lsls r1, r1, #1
	movs r0, #25
	bl Func_0200455c
	movs r0, #25
	movs r1, #0
	bl Func_02004534
	movs r1, #176
	movs r2, #20
	movs r0, #27
	lsls r1, r1, #8
	bl Func_02004544
	movs r0, #27
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r1, #128
	movs r0, #27
	lsls r1, r1, #8
	bl Func_0200454c
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #27
	movs r1, #0
	bl Func_02004534
	ldr r5, .L_0200a340
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
	ldr r0, [r5]
	movs r2, #0
	movs r1, #0
	bl Func_02004544
	movs r0, #0
	movs r1, #0
	bl Func_0200454c
	ldr r0, [r5]
	movs r1, #3
	bl Object_SetModeById
	movs r0, #0
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #128
	lsls r0, r0, #7
	movs r1, #0
	bl Func_02004534
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #26
	bl Func_02004564
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #26
	movs r1, #0
	bl Func_02004534
	movs r1, #192
	ldr r0, [r5]
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02004544
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02004544
	movs r1, #176
	movs r2, #0
	movs r0, #27
	lsls r1, r1, #8
	bl Func_02004544
	movs r1, #128
	movs r0, #25
	lsls r1, r1, #8
	bl Func_0200454c
	movs r0, #25
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r1, #160
	movs r0, #25
	lsls r1, r1, #7
	bl Func_0200454c
	movs r0, #25
	movs r1, #0
	bl Func_02004534
	movs r1, #10
	movs r2, #20
	adds r1, #255
	movs r0, #27
	bl Func_0200455c
	movs r0, #27
	movs r1, #0
	bl Func_02004534
	movs r0, #27
	movs r1, #4
	bl Object_SetModeById
	movs r0, #27
	movs r1, #0
	bl Func_02004534
	ldr r0, [r5]
	movs r1, #0
	movs r2, #0
	bl Func_02004544
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl Func_02004544
	movs r1, #192
	movs r0, #25
	lsls r1, r1, #6
	movs r2, #0
	bl Func_02004544
	movs r1, #2
	adds r1, #255
	movs r2, #0
	ldr r0, [r5]
	bl Func_0200455c
	movs r1, #2
	adds r1, #255
	movs r2, #0
	movs r0, #0
	bl Func_0200455c
	movs r1, #2
	adds r1, #255
	movs r2, #0
	movs r0, #26
	bl Func_0200455c
	movs r1, #2
	movs r2, #40
	adds r1, #255
	movs r0, #25
	bl Func_0200455c
	movs r0, #27
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #27
	movs r1, #0
	bl Func_02004534
	movs r0, #0
	movs r1, #3
	bl Object_SetModeById
	ldr r0, [r5]
	movs r1, #3
	bl Object_SetModeById
	movs r0, #26
	movs r1, #3
	bl Object_SetModeById
	movs r0, #25
	movs r1, #3
	bl Object_SetModeById
	movs r1, #128
	movs r0, #27
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02004544
	movs r1, #132
	movs r2, #20
	lsls r1, r1, #1
	movs r0, #27
	bl Func_0200455c
	movs r0, #27
	movs r1, #0
	bl Func_02004534
	movs r1, #10
	adds r1, #255
	movs r2, #20
	movs r0, #26
	bl Func_0200455c
	movs r2, #20
	movs r0, #26
	movs r1, #0
	bl Func_0200452c
	movs r0, #5
	movs r1, #0
	bl Func_02004534
	movs r1, #129
	movs r0, #27
	lsls r1, r1, #1
	bl Func_02004564
	movs r1, #128
	ldr r0, [r5]
	lsls r1, r1, #7
	movs r2, #0
	bl Func_02004544
	movs r1, #128
	movs r2, #0
	movs r0, #0
	lsls r1, r1, #7
	bl Func_02004544
	movs r1, #160
	movs r0, #27
	lsls r1, r1, #7
	bl Func_0200454c
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #27
	movs r1, #0
	bl Func_02004534
	movs r0, #0
	movs r1, #0
	bl Func_0200454c
	movs r0, #128
	movs r1, #0
	lsls r0, r0, #7
	bl Func_02004524
	movs r1, #128
	ldr r0, [r5]
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02004544
	ldr r0, [r5]
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_0200a34c
	movs r1, #152
	lsls r1, r1, #7
	ldr r0, .L_0200a344
	adds r1, #204
	bl Func_0200456c
	movs r0, #184
	movs r1, #1
	movs r2, #128
	movs r3, #1
	lsls r2, r2, #18
	lsls r0, r0, #16
	negs r1, r1
	bl Func_02004574
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #7
	bl Func_0200454c
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #0
	ldr r1, .L_0200a348
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r2, #130
	lsls r2, r2, #2
	movs r0, #0
	movs r1, #168
	bl ObjectMotion_SetPositionAndReset
	movs r0, #128
	lsls r0, r0, #7
	movs r1, #0
	bl Func_02004534
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #5
	movs r1, #0
	bl Func_02004534
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #2
	strh r3, [r2]
	b .L_0200a3b6
	.2byte 0x0000
.L_0200a340:
	.4byte gPartyState
.L_0200a344:
	.4byte 0x00026666
.L_0200a348:
	.4byte 0x00019999
.L_0200a34c:
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r1, #152
	adds r3, #2
	lsls r1, r1, #7
	strh r3, [r2]
	ldr r0, .L_0200a604
	adds r1, #204
	bl Func_0200456c
	movs r0, #184
	movs r1, #1
	movs r2, #128
	movs r3, #1
	lsls r2, r2, #18
	lsls r0, r0, #16
	negs r1, r1
	bl Func_02004574
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #7
	bl Func_0200454c
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #0
	ldr r1, .L_0200a608
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r2, #130
	movs r0, #0
	movs r1, #168
	lsls r2, r2, #2
	bl ObjectMotion_SetPositionAndReset
	movs r0, #128
	lsls r0, r0, #7
	movs r1, #0
	bl Func_02004534
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #5
	movs r1, #0
	bl Func_02004534
.L_0200a3b6:
	bl Func_02004584
	movs r3, #0
	adds r0, #85
	strb r3, [r0]
	movs r1, #1
	movs r0, #158
	movs r2, #216
	movs r3, #1
	lsls r0, r0, #16
	negs r1, r1
	lsls r2, r2, #17
	bl Func_02004574
	bl Func_0200457c
	movs r2, #10
	movs r0, #17
	movs r1, #4
	bl ObjectMotion_Launch
	movs r0, #17
	movs r1, #0
	bl Func_02004534
	movs r1, #152
	lsls r1, r1, #6
	ldr r0, .L_0200a60c
	adds r1, #102
	bl Func_0200456c
	movs r0, #172
	movs r1, #1
	movs r2, #130
	lsls r2, r2, #18
	movs r3, #1
	lsls r0, r0, #16
	negs r1, r1
	bl Func_02004574
	movs r0, #0
	movs r1, #17
	bl Object_LinkObjectAndSetCallback
	ldr r3, .L_0200a610
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r3, r2
	ldr r0, [r5]
	movs r1, #17
	bl Object_LinkObjectAndSetCallback
	movs r0, #27
	movs r1, #17
	bl Object_LinkObjectAndSetCallback
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #17
	ldr r1, .L_0200a608
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r2, #239
	movs r0, #17
	movs r1, #140
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r2, #135
	movs r0, #17
	movs r1, #150
	lsls r2, r2, #2
	bl ObjectMotion_SetPositionAndReset
	movs r2, #140
	movs r0, #17
	movs r1, #160
	lsls r2, r2, #2
	bl ObjectMotion_SetPositionAndReset
	movs r2, #160
	movs r1, #190
	lsls r2, r2, #2
	movs r0, #17
	bl ObjectMotion_SetPositionAndReset
	movs r0, #0
	bl ObjectMotion_EnableActionAndResetMotion
	ldr r0, [r5]
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #27
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #1
	bl WaitFrames
	movs r2, #0
	movs r1, #0
	movs r0, #17
	bl Func_020044cc
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #152
	lsls r1, r1, #7
	ldr r0, .L_0200a604
	adds r1, #204
	bl Func_0200456c
	movs r0, #184
	movs r1, #1
	movs r2, #224
	movs r3, #1
	lsls r0, r0, #16
	negs r1, r1
	lsls r2, r2, #17
	bl Func_02004574
	movs r2, #242
	lsls r2, r2, #1
	movs r0, #0
	movs r1, #168
	bl ObjectMotion_SetPositionAndReset
	movs r0, #0
	movs r1, #0
	bl Func_0200454c
	movs r0, #128
	lsls r0, r0, #7
	movs r1, #0
	bl Func_02004534
	movs r1, #128
	movs r2, #0
	ldr r0, [r5]
	lsls r1, r1, #8
	bl Func_02004544
	movs r1, #128
	movs r0, #27
	lsls r1, r1, #8
	bl Func_0200454c
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #27
	movs r1, #0
	bl Func_02004534
	movs r1, #160
	movs r2, #0
	movs r0, #25
	lsls r1, r1, #7
	bl Func_02004544
	movs r0, #25
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #25
	movs r1, #0
	bl Func_02004534
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02004544
	movs r1, #176
	movs r2, #0
	movs r0, #27
	lsls r1, r1, #8
	bl Func_02004544
	movs r1, #192
	ldr r0, [r5]
	lsls r1, r1, #8
	bl Func_0200454c
	movs r0, #26
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #26
	movs r1, #0
	bl Func_02004534
	movs r0, #0
	movs r1, #3
	bl Object_SetModeById
	ldr r0, [r5]
	movs r1, #3
	bl Object_SetModeById
	movs r0, #27
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #0
	ldr r1, .L_0200a608
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #27
	ldr r1, .L_0200a608
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r0, #0
	movs r1, #2
	bl Object_SetModeById
	ldr r0, [r5]
	bl Object_GetById
	cmp r0, #0
	beq .L_0200a58c
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #0
	bl ObjectMotion_ResetAndSetPosition
.L_0200a58c:
	movs r0, #0
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl Func_020044cc
	movs r0, #27
	movs r1, #2
	bl Object_SetModeById
	ldr r0, [r5]
	bl Object_GetById
	cmp r0, #0
	beq .L_0200a5bc
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #27
	bl ObjectMotion_ResetAndSetPosition
.L_0200a5bc:
	movs r0, #27
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #27
	movs r1, #0
	movs r2, #0
	bl Func_020044cc
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #108]
	movs r3, #214
	lsls r3, r3, #1
	adds r2, r1, r3
	adds r3, #93
	str r3, [r2]
	subs r3, #85
	adds r2, r1, r3
	movs r0, #48
	movs r3, #16
	str r3, [r2]
	adds r0, #255
	bl Func_020043bc
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #197
	bl Func_020043b4
	bl Func_0200446c
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_0200a604:
	.4byte 0x00026666
.L_0200a608:
	.4byte 0x00019999
.L_0200a60c:
	.4byte 0x00013333
.L_0200a610:
	.4byte gPartyState
	.section .text.x0200a614,"ax",%progbits
	.global Func_02002614
	.thumb_func
Func_02002614:
	push {lr}
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #61
	bl Func_020043ac
	cmp r0, #0
	beq .L_0200a634
	movs r0, #190
	lsls r0, r0, #1
	bl Func_020043b4
	movs r0, #162
	lsls r0, r0, #1
	bl Func_020043b4
.L_0200a634:
	ldr r3, .L_0200a6c8
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_0200a6cc
	cmp r2, r3
	bne .L_0200a65c
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #93
	str r2, [r3]
	bl Func_020029c8
	b .L_0200a786
.L_0200a65c:
	ldr r3, .L_0200a6d0
	cmp r2, r3
	bne .L_0200a67c
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r1, #214
	movs r2, #133
	lsls r1, r1, #1
	lsls r2, r2, #1
	adds r3, r3, r1
	adds r2, #255
	str r2, [r3]
	bl Func_02002a38
	b .L_0200a786
.L_0200a67c:
	ldr r3, .L_0200a6d4
	cmp r2, r3
	bne .L_0200a69a
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #93
	movs r0, #0
	str r2, [r3]
	bl Func_020045dc
	b .L_0200a786
.L_0200a69a:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #61
	bl Func_020043ac
	cmp r0, #0
	bne .L_0200a6b2
	movs r0, #129
	lsls r0, r0, #1
	adds r0, #255
	bl Func_020043b4
.L_0200a6b2:
	movs r0, #129
	lsls r0, r0, #1
	adds r0, #255
	bl Func_020043ac
	cmp r0, #0
	beq .L_0200a6d8
	movs r0, #1
	bl Func_0200461c
	b .L_0200a72a
.L_0200a6c8:
	.4byte gPartyState
.L_0200a6cc:
	.4byte 0x00000110
.L_0200a6d0:
	.4byte 0x00000111
.L_0200a6d4:
	.4byte 0x00000112
.L_0200a6d8:
	ldr r3, .L_0200a714
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	ldr r3, .L_0200a718
	subs r2, #2
	movs r1, #144
	strh r3, [r2]
	lsls r1, r1, #3
	ldr r0, .L_0200a71c
	bl Func_0200434c
	movs r0, #0
	bl Func_02004414
	movs r0, #1
	bl Func_02004414
	movs r0, #2
	bl Func_0200440c
	movs r0, #3
	bl Func_0200440c
	movs r0, #4
	bl Func_0200440c
	movs r0, #5
	b .L_0200a720
.L_0200a714:
	.4byte 0x00000c08
.L_0200a718:
	.4byte 0x00003f10
.L_0200a71c:
	.4byte Func_020018e0
.L_0200a720:
	bl Func_0200440c
	movs r0, #6
	bl Func_0200440c
.L_0200a72a:
	ldr r3, .L_0200a78c
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_0200a790
	cmp r2, r3
	bne .L_0200a756
	movs r0, #192
	lsls r0, r0, #2
	bl Func_020043ac
	cmp r0, #0
	bne .L_0200a750
	movs r0, #64
	movs r1, #1
	bl Func_020045c4
.L_0200a750:
	bl Func_020027e4
	b .L_0200a760
.L_0200a756:
	ldr r3, .L_0200a794
	cmp r2, r3
	bne .L_0200a760
	bl Func_02002878
.L_0200a760:
	movs r0, #129
	lsls r0, r0, #1
	adds r0, #255
	bl Func_020043ac
	cmp r0, #0
	beq .L_0200a786
	movs r0, #128
	lsls r0, r0, #9
	adds r0, #4
	movs r1, #1
	bl Func_0200459c
	movs r0, #16
	bl Func_020045a4
	movs r0, #16
	bl WaitFrames
.L_0200a786:
	movs r0, #0
	pop {pc}
	.2byte 0x0000
.L_0200a78c:
	.4byte gPartyState
.L_0200a790:
	.4byte 0x0000010e
.L_0200a794:
	.4byte 0x0000010f
	.section .text.x0200a798,"ax",%progbits
	.global Func_02002798
	.thumb_func
Func_02002798:
	movs r0, #0
	bx lr
	.section .text.x0200a79c,"ax",%progbits
	.global Func_0200279c
	.thumb_func
Func_0200279c:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r8
	push {r5, r6}
	mov r8, r3
	mov r10, r0
	adds r5, r1, #0
	adds r6, r2, #0
	bl Object_GetById
	adds r1, r0, #0
	adds r3, r1, #0
	ldr r2, .L_0200a7d4
	mov r0, r8
	adds r3, #100
	strh r0, [r3]
	subs r3, #1
	strb r2, [r3]
	ldr r3, .L_0200a7d8
	lsls r5, r5, #16
	lsls r6, r6, #16
	str r3, [r1, #108]
	mov r0, r10
	adds r1, r5, #0
	adds r2, r6, #0
	bl Func_020044cc
	b .L_0200a7dc
.L_0200a7d4:
	.4byte 0x00000000
.L_0200a7d8:
	.4byte Func_020003bc
.L_0200a7dc:
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, pc}
	.section .text.x0200a7e4,"ax",%progbits
	.global Func_020027e4
	.thumb_func
Func_020027e4:
	push {r5, lr}
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #61
	bl Func_020043ac
	cmp r0, #0
	beq .L_0200a854
	movs r0, #10
	adds r0, #255
	bl Func_020043ac
	cmp r0, #0
	bne .L_0200a854
	movs r5, #134
	lsls r5, r5, #2
	movs r2, #180
	lsls r2, r2, #1
	movs r0, #16
	adds r1, r5, #0
	movs r3, #0
	bl Func_0200279c
	movs r2, #212
	lsls r2, r2, #1
	movs r0, #17
	adds r1, r5, #0
	movs r3, #1
	bl Func_0200279c
	movs r1, #145
	movs r2, #210
	lsls r2, r2, #1
	movs r3, #2
	lsls r1, r1, #2
	movs r0, #18
	bl Func_0200279c
	movs r0, #1
	bl WaitFrames
	ldr r1, .L_0200a86c
	movs r0, #16
	bl ObjectMotion_EnableActionAndSetCallback
	ldr r1, .L_0200a870
	movs r0, #17
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #18
	ldr r1, .L_0200a874
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #1
	bl WaitFrames
.L_0200a854:
	movs r1, #1
	movs r0, #20
	bl Func_02004554
	movs r0, #20
	bl Object_GetById
	movs r1, #15
	bl Func_02004514
	pop {r5, pc}
	.2byte 0x0000
.L_0200a86c:
	.4byte Data_020047ac
.L_0200a870:
	.4byte Data_02004848
.L_0200a874:
	.4byte Data_020048e4
	.section .text.x0200a878,"ax",%progbits
	.global Func_02002878
	.thumb_func
Func_02002878:
	push {r5, lr}
	movs r1, #1
	movs r0, #19
	sub sp, #8
	bl Func_02004554
	movs r0, #19
	bl Object_GetById
	movs r1, #15
	bl Func_02004514
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #61
	bl Func_020043ac
	cmp r0, #0
	bne .L_0200a8f4
	movs r0, #156
	lsls r0, r0, #4
	bl Func_020043ac
	cmp r0, #0
	bne .L_0200a8b4
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl Func_020044cc
.L_0200a8b4:
	movs r0, #128
	lsls r0, r0, #2
	bl Func_020043ac
	cmp r0, #0
	beq .L_0200a8d4
	movs r3, #1
	movs r2, #2
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #89
	movs r1, #0
	movs r2, #77
	movs r3, #16
	bl Func_02004404
.L_0200a8d4:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #193
	bl Func_020043ac
	cmp r0, #0
	beq .L_0200a8f4
	movs r0, #10
	bl Object_GetById
	movs r3, #176
	lsls r3, r3, #8
	strh r3, [r0, #6]
	movs r0, #1
	bl WaitFrames
.L_0200a8f4:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #61
	bl Func_020043ac
	cmp r0, #0
	beq .L_0200a946
	movs r5, #3
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
	movs r0, #72
	movs r1, #13
	movs r2, #1
	movs r3, #3
	bl Func_0200442c
	movs r0, #20
	movs r1, #0
	movs r2, #0
	bl Func_020044cc
	b .L_0200a9a0
.L_0200a946:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #45
	bl Func_020043ac
	cmp r0, #0
	beq .L_0200a9a0
	movs r5, #3
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
	movs r0, #72
	movs r1, #13
	movs r2, #1
	movs r3, #3
	bl Func_0200442c
	movs r0, #64
	bl Func_020043ac
	cmp r0, #0
	bne .L_0200a9a0
	movs r0, #20
	movs r1, #0
	movs r2, #0
	bl Func_020044cc
.L_0200a9a0:
	add sp, #8
	pop {r5, pc}
	.section .text.x0200a9a4,"ax",%progbits
	.global Func_020029a4
	.thumb_func
Func_020029a4:
	push {lr}
	ldr r3, .L_0200a9c4
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	ldr r3, [r0, #80]
	movs r0, #16
	ldrb r1, [r3, #9]
	lsls r1, r1, #28
	lsrs r1, r1, #30
	bl Func_02004554
	pop {pc}
.L_0200a9c4:
	.4byte gPartyState
	.section .text.x0200a9c8,"ax",%progbits
	.global Func_020029c8
	.thumb_func
Func_020029c8:
	push {lr}
	movs r0, #0
	bl Func_020045dc
	movs r1, #144
	ldr r0, .L_0200aa00
	lsls r1, r1, #3
	bl Func_0200434c
	ldr r3, .L_0200aa04
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #20
	bne .L_0200a9fc
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #197
	bl Func_020043ac
	cmp r0, #0
	bne .L_0200a9fc
	bl Func_02001cf0
.L_0200a9fc:
	pop {pc}
	.2byte 0x0000
.L_0200aa00:
	.4byte Func_020029a4
.L_0200aa04:
	.4byte gPartyState
	.section .text.x0200aa08,"ax",%progbits
	.global Func_02002a08
	.thumb_func
Func_02002a08:
	push {r5, lr}
	ldr r3, .L_0200aa34
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	ldr r3, [r0, #80]
	movs r0, #8
	ldrb r5, [r3, #9]
	lsls r5, r5, #28
	lsrs r5, r5, #30
	adds r1, r5, #0
	bl Func_02004554
	movs r0, #10
	adds r1, r5, #0
	bl Func_02004554
	pop {r5, pc}
	.2byte 0x0000
.L_0200aa34:
	.4byte gPartyState
	.section .text.x0200aa38,"ax",%progbits
	.global Func_02002a38
	.thumb_func
Func_02002a38:
	push {lr}
	movs r0, #10
	adds r0, #255
	bl Func_020043ac
	cmp r0, #0
	bne .L_0200aa72
	ldr r3, .L_0200aa84
	movs r1, #241
	lsls r1, r1, #1
	adds r3, r3, r1
	ldrh r2, [r3]
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #6
	bne .L_0200aa60
	movs r0, #0
	bl Func_02001c24
	b .L_0200aa72
.L_0200aa60:
	subs r3, r2, #7
	movs r2, #128
	lsls r3, r3, #16
	lsls r2, r2, #9
	cmp r3, r2
	bhi .L_0200aa72
	movs r0, #1
	bl Func_02001c24
.L_0200aa72:
	movs r0, #0
	bl Func_020045dc
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_0200aa88
	bl Func_0200434c
	pop {pc}
.L_0200aa84:
	.4byte gPartyState
.L_0200aa88:
	.4byte Func_02002a08
	.section .text.x0200aa8c,"ax",%progbits
	.global Func_02002a8c
	.thumb_func
Func_02002a8c:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #197
	bl Func_020043ac
	cmp r0, #0
	bne .L_0200aaa4
	bl .L_0200b84e
.L_0200aaa4:
	bl Func_02004464
	movs r0, #0
	bl Func_020045ec
	movs r0, #78
	bl Func_0200463c
	movs r0, #5
	movs r1, #2
	movs r2, #10
	bl ObjectMotion_Launch
	movs r2, #10
	movs r1, #4
	movs r0, #5
	bl ObjectMotion_Launch
	ldr r0, .L_0200ae58
	bl Func_0200451c
	movs r0, #5
	movs r1, #0
	bl Func_02004534
	movs r1, #204
	lsls r1, r1, #7
	ldr r0, .L_0200ae5c
	adds r1, #102
	bl Func_0200456c
	movs r0, #144
	movs r1, #1
	movs r2, #228
	movs r3, #1
	lsls r0, r0, #16
	negs r1, r1
	lsls r2, r2, #16
	bl Func_02004574
	bl Func_0200457c
	movs r1, #132
	movs r2, #0
	lsls r1, r1, #1
	movs r0, #10
	bl Func_0200455c
	movs r0, #128
	lsls r0, r0, #6
	movs r1, #0
	adds r0, #10
	bl Func_02004534
	movs r0, #37
	bl Func_0200463c
	ldr r5, .L_0200ae60
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
	ldr r0, [r5]
	movs r1, #136
	movs r2, #196
	bl ObjectMotion_SetPositionAndReset
	movs r1, #128
	movs r2, #0
	ldr r0, [r5]
	lsls r1, r1, #7
	bl Func_02004544
	ldr r1, [r5]
	movs r0, #0
	bl Func_020044dc
	ldr r1, [r5]
	movs r0, #11
	bl Func_020044dc
	movs r0, #1
	bl WaitFrames
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #0
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	adds r2, #102
	movs r0, #11
	adds r1, #204
	bl ObjectMotion_SetSpeedParameters
	ldr r1, .L_0200ae64
	movs r0, #0
	bl ObjectMotion_EnableActionAndSetCallback
	ldr r1, .L_0200ae68
	movs r0, #11
	bl Object_SetActionCallbackAndRefreshById
	movs r0, #9
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #9
	movs r1, #0
	bl Func_02004534
	movs r1, #192
	movs r0, #5
	lsls r1, r1, #7
	bl Func_0200454c
	movs r1, #4
	movs r2, #20
	adds r1, #255
	movs r0, #5
	bl Func_0200455c
	movs r0, #5
	movs r1, #0
	bl Func_02004534
	movs r1, #224
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02004544
	movs r1, #132
	movs r2, #20
	lsls r1, r1, #1
	movs r0, #6
	bl Func_0200455c
	movs r0, #6
	movs r1, #0
	bl Func_02004534
	movs r1, #128
	movs r0, #7
	lsls r1, r1, #8
	bl Func_0200454c
	movs r0, #7
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r1, #224
	movs r0, #7
	lsls r1, r1, #8
	bl Func_0200454c
	movs r0, #7
	movs r1, #0
	bl Func_02004534
	movs r1, #8
	movs r2, #20
	adds r1, #255
	movs r0, #5
	bl Func_0200455c
	movs r0, #5
	movs r1, #0
	bl Func_02004534
	movs r0, #3
	movs r1, #4
	bl Object_SetModeById
	movs r0, #3
	movs r1, #0
	bl Func_02004534
	movs r1, #132
	movs r2, #0
	lsls r1, r1, #1
	movs r0, #10
	bl Func_0200455c
	movs r1, #208
	movs r0, #10
	lsls r1, r1, #8
	bl Func_0200454c
	movs r0, #10
	movs r1, #0
	bl Func_02004534
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #7
	bl Func_0200454c
	movs r2, #10
	movs r0, #5
	movs r1, #4
	bl ObjectMotion_Launch
	movs r0, #5
	movs r1, #0
	bl Func_02004534
	movs r1, #10
	movs r2, #0
	adds r1, #255
	movs r0, #1
	bl Func_0200455c
	movs r1, #160
	movs r0, #1
	lsls r1, r1, #8
	bl Func_0200454c
	movs r0, #1
	movs r1, #0
	bl Func_02004534
	movs r1, #6
	movs r2, #40
	adds r1, #255
	movs r0, #2
	bl Func_0200455c
	movs r1, #160
	movs r0, #2
	lsls r1, r1, #8
	bl Func_0200454c
	movs r0, #2
	movs r1, #0
	bl Func_02004534
	movs r1, #10
	movs r2, #40
	adds r1, #255
	movs r0, #0
	bl Func_0200455c
	movs r0, #0
	movs r1, #0
	bl Func_0200454c
	movs r0, #128
	lsls r0, r0, #8
	movs r1, #0
	movs r2, #10
	bl Func_0200452c
	movs r1, #192
	movs r0, #8
	lsls r1, r1, #6
	movs r2, #20
	bl Func_02004544
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #8
	movs r1, #0
	movs r2, #20
	bl Func_0200452c
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #36
	bl Func_020043ac
	cmp r0, #0
	beq .L_0200ad24
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #7
	bl Func_0200454c
	movs r0, #0
	movs r1, #4
	bl Object_SetModeById
	movs r0, #0
	movs r1, #0
	bl Func_02004534
	movs r1, #208
	movs r0, #8
	lsls r1, r1, #8
	bl Func_0200454c
	movs r0, #8
	movs r1, #0
	bl Func_02004534
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_0200ad48
.L_0200ad24:
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r1, #208
	adds r3, #2
	movs r0, #8
	lsls r1, r1, #8
	strh r3, [r2]
	bl Func_0200454c
	movs r0, #8
	movs r1, #0
	bl Func_02004534
.L_0200ad48:
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #7
	bl Func_0200454c
	movs r0, #0
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #11
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #11
	movs r1, #0
	bl Func_02004534
	movs r1, #192
	movs r0, #11
	lsls r1, r1, #6
	movs r2, #20
	bl Func_02004544
	movs r1, #128
	movs r2, #20
	movs r0, #11
	lsls r1, r1, #8
	bl Func_02004544
	movs r1, #160
	movs r0, #11
	lsls r1, r1, #7
	bl Func_0200454c
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #11
	movs r1, #0
	bl Func_02004534
	movs r1, #128
	movs r0, #10
	lsls r1, r1, #8
	bl Func_0200454c
	movs r0, #192
	lsls r0, r0, #7
	adds r0, #10
	movs r1, #0
	bl Func_02004534
	movs r0, #9
	movs r1, #0
	bl Func_0200454c
	movs r0, #9
	movs r1, #3
	bl Object_SetModeById
	movs r0, #160
	lsls r0, r0, #7
	adds r0, #9
	movs r1, #0
	bl Func_02004534
	movs r0, #8
	movs r1, #0
	bl Func_0200454c
	movs r0, #160
	lsls r0, r0, #7
	adds r0, #8
	movs r1, #0
	bl Func_02004534
	movs r0, #11
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #11
	movs r1, #0
	bl Func_02004534
	movs r0, #0
	movs r1, #0
	bl Func_0200454c
	movs r0, #0
	movs r1, #3
	bl Object_SetModeById
	movs r0, #128
	movs r1, #0
	lsls r0, r0, #8
	bl Func_02004524
	ldr r5, .L_0200ae60
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	movs r1, #128
	ldr r0, [r5]
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02004544
	ldr r0, [r5]
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_0200ae6c
	movs r0, #0
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #128
	lsls r0, r0, #8
	movs r1, #0
	bl Func_02004534
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_0200ae90
	.2byte 0x0000
.L_0200ae58:
	.4byte 0x00002ee8
.L_0200ae5c:
	.4byte 0x00033333
.L_0200ae60:
	.4byte gPartyState
.L_0200ae64:
	.4byte Data_02005ee8
.L_0200ae68:
	.4byte Data_02005f2c
.L_0200ae6c:
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #0
	adds r3, #1
	movs r1, #4
	strh r3, [r2]
	bl Motion_SetModeAndWaitAnimation
	movs r0, #128
	lsls r0, r0, #8
	movs r1, #0
	bl Func_02004534
.L_0200ae90:
	movs r1, #160
	movs r0, #5
	lsls r1, r1, #8
	bl Func_0200454c
	movs r0, #5
	movs r1, #2
	movs r2, #10
	bl ObjectMotion_Launch
	movs r2, #10
	movs r0, #5
	movs r1, #4
	bl ObjectMotion_Launch
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #5
	movs r1, #0
	bl Func_02004534
	movs r0, #0
	movs r1, #3
	bl Object_SetModeById
	ldr r3, .L_0200b1b4
	movs r1, #3
	mov r8, r3
	movs r3, #133
	lsls r3, r3, #2
	add r8, r3
	mov r3, r8
	ldr r0, [r3]
	bl Object_SetModeById
	movs r0, #11
	movs r1, #3
	bl Object_SetModeById
	movs r0, #5
	movs r1, #3
	bl Object_SetModeById
	movs r0, #6
	movs r1, #3
	bl Object_SetModeById
	movs r0, #7
	movs r1, #3
	bl Object_SetModeById
	movs r0, #8
	movs r1, #3
	bl Object_SetModeById
	movs r0, #9
	movs r1, #3
	bl Object_SetModeById
	movs r0, #10
	movs r1, #3
	bl Object_SetModeById
	movs r0, #3
	movs r1, #3
	bl Object_SetModeById
	movs r0, #2
	movs r1, #3
	bl Object_SetModeById
	movs r0, #1
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #0
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #204
	movs r2, #204
	mov r3, r8
	lsls r1, r1, #8
	lsls r2, r2, #7
	ldr r0, [r3]
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #11
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #5
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #6
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #7
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #8
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #9
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #10
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #3
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #2
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	adds r2, #102
	movs r0, #1
	adds r1, #204
	bl ObjectMotion_SetSpeedParameters
	movs r0, #0
	movs r1, #2
	bl Func_02004554
	mov r3, r8
	ldr r0, [r3]
	movs r1, #2
	bl Func_02004554
	movs r0, #11
	movs r1, #2
	bl Func_02004554
	movs r0, #5
	movs r1, #2
	bl Func_02004554
	movs r0, #6
	movs r1, #2
	bl Func_02004554
	movs r0, #7
	movs r1, #2
	bl Func_02004554
	movs r0, #8
	movs r1, #2
	bl Func_02004554
	movs r0, #9
	movs r1, #2
	bl Func_02004554
	movs r0, #10
	movs r1, #2
	bl Func_02004554
	movs r0, #3
	movs r1, #2
	bl Func_02004554
	movs r0, #2
	movs r1, #2
	bl Func_02004554
	movs r0, #1
	movs r1, #2
	bl Func_02004554
	movs r0, #204
	movs r1, #192
	lsls r0, r0, #7
	lsls r1, r1, #4
	adds r0, #102
	adds r1, #204
	bl Func_0200456c
	movs r0, #144
	movs r1, #1
	movs r2, #246
	lsls r2, r2, #16
	movs r3, #1
	lsls r0, r0, #16
	negs r1, r1
	bl Func_02004574
	ldr r1, .L_0200b1b8
	movs r0, #9
	bl ObjectMotion_EnableActionAndSetCallback
	ldr r1, .L_0200b1bc
	movs r0, #10
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #10
	bl Battle_WaitMode0
	ldr r5, .L_0200b1c0
	movs r0, #8
	adds r1, r5, #0
	bl ObjectMotion_EnableActionAndSetCallback
	ldr r6, .L_0200b1c4
	movs r0, #3
	adds r1, r6, #0
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #6
	adds r1, r5, #0
	bl ObjectMotion_EnableActionAndSetCallback
	adds r1, r5, #0
	movs r0, #7
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #2
	adds r1, r6, #0
	bl ObjectMotion_EnableActionAndSetCallback
	ldr r1, .L_0200b1c8
	movs r0, #1
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #30
	bl Battle_WaitMode0
	ldr r1, .L_0200b1cc
	movs r0, #5
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #20
	bl Battle_WaitMode0
	ldr r1, .L_0200b1d0
	movs r0, #0
	bl ObjectMotion_EnableActionAndSetCallback
	mov r3, r8
	ldr r0, [r3]
	ldr r1, .L_0200b1d4
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #10
	bl Battle_WaitMode0
	ldr r1, .L_0200b1d8
	movs r0, #11
	bl Object_SetActionCallbackAndRefreshById
	movs r1, #2
	adds r1, #255
	movs r2, #20
	movs r0, #11
	bl Func_0200455c
	movs r1, #176
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #11
	bl Func_02004544
	movs r0, #5
	bl Object_RefreshSelectorById
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02004544
	movs r0, #1
	movs r1, #0
	movs r2, #40
	bl Func_02004544
	movs r1, #160
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02004544
	movs r1, #192
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl Func_02004544
	movs r1, #132
	movs r2, #40
	lsls r1, r1, #1
	movs r0, #11
	bl Func_0200455c
	movs r0, #160
	lsls r0, r0, #8
	movs r1, #0
	adds r0, #11
	bl Func_02004524
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl Func_02004544
	mov r3, r8
	ldr r0, [r3]
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_0200b1dc
	movs r0, #11
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #160
	lsls r0, r0, #8
	adds r0, #11
	movs r1, #0
	bl Func_02004534
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_0200b202
	.2byte 0x0000
.L_0200b1b4:
	.4byte gPartyState
.L_0200b1b8:
	.4byte Data_02005f70
.L_0200b1bc:
	.4byte Data_02005fac
.L_0200b1c0:
	.4byte Data_02005fe8
.L_0200b1c4:
	.4byte Data_02006038
.L_0200b1c8:
	.4byte Data_02006088
.L_0200b1cc:
	.4byte Data_020060cc
.L_0200b1d0:
	.4byte Data_020060fc
.L_0200b1d4:
	.4byte Data_0200617c
.L_0200b1d8:
	.4byte Data_020061fc
.L_0200b1dc:
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #11
	adds r3, #1
	movs r1, #4
	strh r3, [r2]
	bl Motion_SetModeAndWaitAnimation
	movs r0, #160
	lsls r0, r0, #8
	adds r0, #11
	movs r1, #0
	bl Func_02004534
.L_0200b202:
	movs r1, #192
	movs r2, #20
	movs r0, #0
	lsls r1, r1, #8
	bl Func_02004544
	movs r0, #0
	movs r1, #0
	bl Func_02004534
	movs r0, #11
	movs r1, #4
	bl Object_SetModeById
	movs r0, #160
	lsls r0, r0, #8
	adds r0, #11
	movs r1, #0
	bl Func_02004534
	movs r2, #10
	movs r0, #1
	movs r1, #2
	bl ObjectMotion_Launch
	movs r0, #1
	movs r1, #0
	bl Func_02004534
	ldr r5, .L_0200b5ac
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	movs r1, #128
	ldr r0, [r5]
	lsls r1, r1, #7
	movs r2, #0
	bl Func_02004544
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #6
	movs r2, #0
	bl Func_02004544
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #11
	bl Func_0200455c
	movs r1, #192
	movs r2, #20
	movs r0, #11
	lsls r1, r1, #6
	bl Func_02004544
	movs r0, #11
	movs r1, #0
	bl Func_02004534
	movs r0, #5
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #5
	movs r1, #0
	bl Func_02004534
	movs r0, #11
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #11
	movs r1, #0
	bl Func_02004534
	movs r1, #4
	adds r1, #255
	movs r2, #0
	movs r0, #1
	bl Func_0200455c
	movs r2, #20
	movs r0, #1
	movs r1, #0
	bl Func_0200452c
	movs r0, #11
	movs r1, #3
	bl Object_SetModeById
	movs r0, #11
	movs r1, #0
	bl Func_02004534
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl Func_02004544
	movs r1, #128
	ldr r0, [r5]
	lsls r1, r1, #8
	movs r2, #40
	bl Func_02004544
	movs r1, #128
	movs r2, #0
	ldr r0, [r5]
	lsls r1, r1, #7
	bl Func_02004544
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #6
	bl Func_0200454c
	movs r0, #0
	movs r1, #0
	bl Func_02004534
	movs r1, #176
	movs r0, #11
	lsls r1, r1, #8
	bl Func_0200454c
	movs r1, #4
	adds r1, #255
	movs r2, #0
	movs r0, #11
	bl Func_0200455c
	movs r0, #160
	lsls r0, r0, #8
	movs r1, #0
	adds r0, #11
	bl Func_02004524
	ldr r0, [r5]
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_0200b34c
	movs r0, #11
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #160
	lsls r0, r0, #8
	adds r0, #11
	movs r1, #0
	bl Func_02004534
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_0200b372
.L_0200b34c:
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #11
	adds r3, #1
	movs r1, #3
	strh r3, [r2]
	bl Motion_SetModeAndWaitAnimation
	movs r0, #160
	lsls r0, r0, #8
	adds r0, #11
	movs r1, #0
	bl Func_02004534
.L_0200b372:
	movs r1, #2
	movs r2, #20
	adds r1, #255
	movs r0, #1
	bl Func_0200455c
	movs r0, #1
	movs r1, #0
	bl Func_02004534
	movs r1, #192
	movs r0, #11
	lsls r1, r1, #6
	bl Func_0200454c
	movs r0, #11
	movs r1, #0
	bl Func_02004534
	movs r1, #2
	movs r2, #0
	adds r1, #255
	movs r0, #5
	bl Func_0200455c
	movs r0, #5
	movs r1, #0
	bl Func_02004534
	movs r0, #11
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #0
	movs r1, #0
	bl Func_02004534
	movs r1, #176
	movs r0, #11
	lsls r1, r1, #8
	bl Func_0200454c
	movs r0, #11
	movs r1, #3
	bl Object_SetModeById
	movs r0, #160
	lsls r0, r0, #8
	adds r0, #11
	movs r1, #0
	bl Func_02004534
	ldr r5, .L_0200b5ac
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	ldr r0, [r5]
	movs r1, #3
	bl Object_SetModeById
	movs r0, #0
	movs r1, #3
	bl Object_SetModeById
	movs r0, #1
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #5
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #1
	movs r1, #0
	bl Func_02004534
	movs r1, #4
	movs r2, #0
	adds r1, #255
	movs r0, #11
	bl Func_0200455c
	movs r1, #192
	movs r0, #11
	lsls r1, r1, #6
	bl Func_0200454c
	movs r0, #11
	movs r1, #0
	bl Func_02004534
	movs r1, #129
	movs r0, #1
	lsls r1, r1, #1
	bl Func_02004564
	movs r0, #1
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #11
	movs r1, #3
	bl Object_SetModeById
	movs r0, #11
	movs r1, #0
	bl Func_02004534
	movs r0, #5
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #5
	movs r1, #0
	bl Func_02004534
	movs r2, #10
	movs r0, #1
	movs r1, #2
	bl ObjectMotion_Launch
	movs r0, #1
	movs r1, #0
	bl Func_02004534
	movs r0, #11
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #7
	bl Func_0200454c
	movs r0, #0
	movs r1, #0
	bl Func_02004534
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #8
	bl Func_0200454c
	movs r0, #5
	movs r1, #3
	bl Object_SetModeById
	movs r0, #5
	movs r1, #0
	bl Func_02004534
	movs r0, #1
	movs r1, #0
	bl Func_0200454c
	movs r0, #5
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #1
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #5
	ldr r1, .L_0200b5b0
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r2, #204
	lsls r2, r2, #8
	adds r2, #204
	movs r0, #1
	ldr r1, .L_0200b5b0
	bl ObjectMotion_SetSpeedParameters
	ldr r1, .L_0200b5b4
	movs r0, #5
	bl ObjectMotion_EnableActionAndSetCallback
	ldr r1, .L_0200b5b8
	movs r0, #1
	bl Object_SetActionCallbackAndRefreshById
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #176
	movs r0, #11
	lsls r1, r1, #8
	bl Func_0200454c
	movs r0, #11
	movs r1, #3
	bl Object_SetModeById
	movs r0, #160
	lsls r0, r0, #8
	adds r0, #11
	movs r1, #0
	bl Func_02004534
	movs r2, #128
	movs r0, #11
	movs r1, #156
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #1
	movs r0, #11
	bl Func_0200455c
	movs r0, #0
	movs r1, #0
	bl Func_02004534
	movs r0, #0
	movs r1, #4
	bl Object_SetModeById
	movs r0, #0
	movs r1, #0
	bl Func_02004534
	movs r1, #176
	movs r0, #11
	lsls r1, r1, #8
	bl Func_0200454c
	movs r0, #0
	movs r1, #0
	bl Func_02004534
	movs r1, #10
	movs r2, #40
	adds r1, #255
	movs r0, #11
	bl Func_0200455c
	movs r0, #160
	lsls r0, r0, #8
	movs r1, #0
	adds r0, #11
	bl Func_02004524
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl Func_02004544
	ldr r0, [r5]
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_0200b5bc
	movs r0, #11
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #160
	lsls r0, r0, #8
	adds r0, #11
	movs r1, #0
	bl Func_02004534
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_0200b5e2
.L_0200b5ac:
	.4byte gPartyState
.L_0200b5b0:
	.4byte 0x00019999
.L_0200b5b4:
	.4byte Data_0200622c
.L_0200b5b8:
	.4byte Data_02006268
.L_0200b5bc:
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #11
	adds r3, #1
	movs r1, #4
	strh r3, [r2]
	bl Motion_SetModeAndWaitAnimation
	movs r0, #160
	lsls r0, r0, #8
	adds r0, #11
	movs r1, #0
	bl Func_02004534
.L_0200b5e2:
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #6
	bl Func_0200454c
	movs r0, #11
	movs r1, #4
	bl Object_SetModeById
	movs r0, #160
	lsls r0, r0, #8
	movs r2, #20
	adds r0, #11
	movs r1, #0
	bl Func_0200452c
	movs r1, #0
	movs r0, #0
	bl Func_02004534
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #11
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #160
	lsls r0, r0, #8
	adds r0, #11
	movs r1, #0
	bl Func_02004534
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl Func_02004544
	ldr r5, .L_0200b854
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	movs r1, #128
	ldr r0, [r5]
	lsls r1, r1, #8
	movs r2, #40
	bl Func_02004544
	movs r1, #128
	movs r2, #0
	ldr r0, [r5]
	lsls r1, r1, #7
	bl Func_02004544
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #6
	bl Func_0200454c
	movs r0, #0
	movs r1, #0
	bl Func_02004534
	movs r0, #11
	movs r1, #4
	bl Object_SetModeById
	movs r0, #160
	lsls r0, r0, #8
	adds r0, #11
	movs r1, #0
	bl Func_02004534
	movs r1, #2
	adds r1, #255
	movs r2, #0
	movs r0, #0
	bl Func_0200455c
	movs r1, #2
	movs r2, #40
	ldr r0, [r5]
	adds r1, #255
	bl Func_0200455c
	movs r0, #160
	lsls r0, r0, #8
	adds r0, #11
	movs r1, #0
	bl Func_02004534
	movs r0, #0
	movs r1, #3
	bl Object_SetModeById
	ldr r0, [r5]
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #11
	movs r1, #3
	bl Object_SetModeById
	movs r0, #160
	lsls r0, r0, #8
	adds r0, #11
	movs r1, #0
	bl Func_02004534
	movs r0, #0
	movs r1, #3
	bl Object_SetModeById
	ldr r0, [r5]
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #11
	bl Func_0200455c
	movs r1, #192
	movs r0, #11
	lsls r1, r1, #6
	movs r2, #20
	bl Func_02004544
	movs r2, #10
	movs r0, #11
	movs r1, #4
	bl ObjectMotion_Launch
	movs r0, #160
	lsls r0, r0, #8
	adds r0, #11
	movs r1, #0
	bl Func_02004534
	movs r1, #176
	movs r0, #11
	lsls r1, r1, #8
	bl Func_0200454c
	movs r0, #160
	lsls r0, r0, #8
	adds r0, #11
	movs r1, #0
	bl Func_02004534
	movs r1, #192
	movs r0, #11
	lsls r1, r1, #6
	bl Func_0200454c
	movs r0, #11
	movs r1, #2
	movs r2, #10
	bl ObjectMotion_Launch
	movs r2, #10
	movs r0, #11
	movs r1, #4
	bl ObjectMotion_Launch
	movs r0, #160
	lsls r0, r0, #8
	adds r0, #11
	movs r1, #0
	bl Func_02004534
	movs r2, #0
	movs r0, #0
	movs r1, #0
	bl Func_02004544
	movs r1, #128
	ldr r0, [r5]
	lsls r1, r1, #8
	bl Func_0200454c
	movs r1, #176
	movs r0, #11
	lsls r1, r1, #8
	bl Func_0200454c
	movs r0, #160
	lsls r0, r0, #8
	adds r0, #11
	movs r1, #0
	bl Func_02004534
	movs r1, #128
	movs r2, #0
	movs r0, #0
	lsls r1, r1, #6
	bl Func_02004544
	movs r1, #128
	ldr r0, [r5]
	lsls r1, r1, #7
	bl Func_0200454c
	movs r0, #0
	movs r1, #3
	bl Object_SetModeById
	ldr r0, [r5]
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r1, #132
	movs r2, #0
	lsls r1, r1, #1
	movs r0, #11
	bl Func_0200455c
	movs r1, #192
	movs r0, #11
	lsls r1, r1, #6
	bl Func_0200454c
	movs r0, #160
	lsls r0, r0, #8
	adds r0, #11
	movs r1, #0
	bl Func_02004534
	movs r0, #11
	movs r1, #9
	bl Object_SetModeById
	ldr r0, [r5]
	movs r1, #33
	bl Object_SetModeById
	movs r1, #40
	movs r0, #0
	bl Object_SetModeById
	movs r0, #2
	bl Func_02004414
	movs r0, #3
	bl Func_02004414
	movs r0, #4
	bl Func_02004414
	movs r0, #5
	bl Func_02004414
	movs r0, #6
	bl Func_02004414
	movs r0, #78
	bl Func_0200463c
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #0
	adds r0, #9
	bl Func_0200459c
	movs r0, #120
	bl Func_020045a4
	movs r0, #120
	bl WaitFrames
	movs r5, #0
	b .L_0200b818
.L_0200b816:
	adds r5, #1
.L_0200b818:
	cmp r5, #119
	bhi .L_0200b82a
	movs r0, #1
	bl WaitFrames
	ldr r3, .L_0200b858
	ldr r3, [r3]
	cmp r3, #0
	beq .L_0200b816
.L_0200b82a:
	movs r1, #0
	movs r0, #0
	bl Func_0200459c
	movs r0, #16
	bl Func_020045a4
	movs r0, #16
	bl WaitFrames
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #204
	bl Func_020043b4
	movs r0, #3
	bl Func_0200458c
.L_0200b84e:
	pop {r3}
	mov r8, r3
	pop {r5, r6, pc}
.L_0200b854:
	.4byte gPartyState
.L_0200b858:
	.4byte gInput
	.section .text.x0200b85c,"ax",%progbits
	.global Func_0200385c
	.thumb_func
Func_0200385c:
	push {r5, lr}
	adds r5, r0, #0
	adds r2, r5, #0
	adds r2, #100
	movs r0, #0
	ldrsh r1, [r2, r0]
	ldrh r3, [r2]
	cmp r1, #0
	beq .L_0200b874
	subs r3, #1
	strh r3, [r2]
	b .L_0200b8da
.L_0200b874:
	adds r3, r5, #0
	adds r3, #90
	movs r0, #131
	strb r1, [r3]
	lsls r0, r0, #1
	bl Func_020043ac
	movs r3, #1
	negs r3, r3
	cmp r0, #0
	bne .L_0200b89a
	ldr r3, .L_0200b8dc
	movs r2, #15
	ldr r3, [r3]
	ldr r1, .L_0200b8e0
	lsrs r3, r3, #4
	ands r3, r2
	lsls r3, r3, #1
	ldrsh r3, [r1, r3]
.L_0200b89a:
	movs r0, #1
	negs r0, r0
	cmp r3, r0
	bne .L_0200b8ac
	adds r0, r5, #0
	movs r1, #9
	bl Func_020043c4
	b .L_0200b8da
.L_0200b8ac:
	ldrh r1, [r5, #6]
	movs r2, #128
	subs r3, r3, r1
	lsls r3, r3, #16
	asrs r3, r3, #16
	lsls r2, r2, #5
	cmp r3, r2
	ble .L_0200b8be
	adds r3, r2, #0
.L_0200b8be:
	ldr r2, .L_0200b8e4
	cmp r3, r2
	bge .L_0200b8c6
	adds r3, r2, #0
.L_0200b8c6:
	adds r3, r1, r3
	adds r0, r5, #0
	movs r1, #2
	strh r3, [r5, #6]
	bl Func_020043c4
	adds r0, r5, #0
	movs r1, #48
	bl ObjectDispatch_ApplyValueToChildren
.L_0200b8da:
	pop {r5, pc}
.L_0200b8dc:
	.4byte gInput
.L_0200b8e0:
	.4byte Data_02004740
.L_0200b8e4:
	.4byte 0xfffff000
	.section .text.x0200b8e8,"ax",%progbits
	.global Func_020038e8
	.thumb_func
Func_020038e8:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #192
	lsls r2, r2, #4
	adds r2, #162
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	beq .L_0200b918
	movs r0, #131
	lsls r0, r0, #1
	bl Func_020043b4
	bl Func_02004594
	bl Func_020045e4
	movs r0, #131
	lsls r0, r0, #1
	bl Func_020043bc
.L_0200b918:
	pop {pc}
	.2byte 0x0000
	.section .text.x0200b91c,"ax",%progbits
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
	.4byte gPartyState
.L_0200b9e0:
	.4byte 0xfff00000
.L_0200b9e4:
	.4byte IwramMulQ16
.L_0200b9e8:
	.4byte gInput
.L_0200b9ec:
	.4byte Data_02004780
.L_0200b9f0:
	.4byte 0xffff0000
.L_0200b9f4:
	ldr r3, [sp, #16]
	ldr r2, [sp, #20]
	ldr r0, [r7, #16]
	ldr r1, [r7, #8]
	subs r0, r3, r0
	subs r1, r2, r1
	bl Func_02004364
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
	bl ObjectDispatch_ApplyValueToChildren
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
	bl WaitFrames
	b .L_0200b93c
.L_0200bb92:
	movs r0, #10
	bl WaitFrames
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
	.4byte gPartyState
.L_0200bc2c:
	.4byte gOverlayArea + 0x62a4
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
	.4byte Data_02004780
.L_0200bcac:
	.4byte 0xffff0000
.L_0200bcb0:
	ldr r3, [sp, #16]
	ldr r2, [sp, #20]
	ldr r0, [r7, #16]
	ldr r1, [r7, #8]
	subs r0, r3, r0
	subs r1, r2, r1
	bl Func_02004364
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
	bl ObjectDispatch_ApplyValueToChildren
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
	bl WaitFrames
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
	bl WaitFrames
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
	bl WaitFrames
	b .L_0200bbee
.L_0200bea0:
	movs r0, #10
	bl WaitFrames
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
	.4byte gOverlayArea + 0x62a4
.L_0200bed8:
	.4byte Func_0200385c
	.section .text.x0200bedc,"ax",%progbits
	.global Func_02003edc
	.thumb_func
Func_02003edc:
	ldr r3, .L_0200bee4
	strh r0, [r3]
	bx lr
	.2byte 0x0000
.L_0200bee4:
	.4byte gOverlayArea + 0x62a4
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
	.4byte gPartyState
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
	bl ObjectDispatch_ApplyValueToChildren
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
	bl WaitFrames
	b .L_0200bf1a
.L_0200c0e0:
	movs r0, #10
	bl WaitFrames
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
	.section .text.x0200c198,"ax",%progbits
	.global Func_02004198
	.thumb_func
Func_02004198:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #12
	str r2, [sp, #0]
	str r0, [sp, #8]
	str r1, [sp, #4]
	adds r2, r3, #0
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r3, [r3]
	mov r11, r3
	cmp r2, #0
	ble .L_0200c244
	adds r7, r2, #0
.L_0200c1c0:
	bl Random16Far
	movs r1, #176
	lsls r1, r1, #5
	add r1, r11
	movs r2, #0
	ldrsh r3, [r1, r2]
	mov r10, r1
	lsls r6, r3, #3
	subs r6, r6, r3
	lsls r6, r6, #2
	movs r3, #160
	add r6, r11
	lsls r3, r3, #3
	adds r5, r6, r3
	movs r1, #0
	str r1, [r5, #24]
	ldr r2, [sp, #8]
	mov r8, r1
	str r2, [r5]
	ldr r3, [sp, #4]
	mov r9, r0
	str r3, [r5, #4]
	ldr r1, [sp, #0]
	subs r7, #1
	str r1, [r5, #8]
	bl Random16Far
	movs r2, #128
	lsls r2, r2, #12
	lsls r0, r0, #3
	adds r0, r0, r2
	mov r1, r9
	adds r2, r5, #0
	bl Vector_AddPolarOffsetFar
	mov r3, r8
	str r3, [r5, #12]
	movs r3, #160
	lsls r3, r3, #11
	mov r1, r8
	str r3, [r5, #16]
	str r1, [r5, #20]
	bl Random16Far
	movs r3, #160
	lsls r3, r3, #3
	adds r3, #12
	movs r2, #128
	adds r6, r6, r3
	lsls r2, r2, #10
	lsls r0, r0, #1
	adds r0, r0, r2
	mov r1, r9
	adds r2, r6, #0
	bl Vector_AddPolarOffsetFar
	mov r1, r10
	ldrh r3, [r1]
	movs r2, #63
	adds r3, #1
	ands r3, r2
	mov r2, r10
	strh r3, [r2]
	cmp r7, #0
	bne .L_0200c1c0
.L_0200c244:
	add sp, #12
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x0200c254,"ax",%progbits
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
	bl Runtime_AllocateHeapBlockFar
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
	bl VramBlock_LoadCached
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
	bl Func_0200434c
	add sp, #4
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_0200c308:
	.4byte 0x000001f0
.L_0200c30c:
	.4byte Func_02004114
	.section .text.x0200c310,"ax",%progbits
	.global Func_02004310
	.thumb_func
Func_02004310:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r0, .L_0200c338
	ldr r5, [r3]
	bl Scheduler_RemoveCallbackFar
	movs r3, #176
	lsls r3, r3, #5
	adds r3, #4
	adds r5, r5, r3
	movs r3, #0
	ldrsh r0, [r5, r3]
	bl Func_0200438c
	movs r0, #220
	bl Runtime_ReleaseHeapBlock
	pop {r5, pc}
.L_0200c338:
	.4byte Func_02004114
	.section .rodata.x0200c644,"a",%progbits
.L_0200c644:
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
.L_0200c680:
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
.L_0200c6bc:
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
	.global Data_020046f8
Data_020046f8:
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
	.global Data_02004740
Data_02004740:
	.4byte 0x0000ffff
	.4byte 0xffff8000
	.4byte 0xe000c000
	.4byte 0xc000a000
	.4byte 0x20004000
	.4byte 0x40006000
	.4byte 0x0000ffff
	.4byte 0xffff8000
	.4byte 0x0000ffff
	.4byte 0xffff8000
	.4byte 0xffffc000
	.4byte 0xc000ffff
	.4byte 0xffff4000
	.4byte 0x4000ffff
	.4byte 0x0000ffff
	.4byte 0xffff8000
	.global Data_02004780
Data_02004780:
	.4byte 0x0000ffff
	.4byte 0xffff8000
	.4byte 0x0000c000
	.4byte 0xc0008000
	.4byte 0x00004000
	.4byte 0x40008000
	.4byte 0x0000ffff
	.4byte 0xffff8000
	.global Data_020047a0
Data_020047a0:
	.4byte .L_0200c644
	.4byte .L_0200c680
	.4byte .L_0200c6bc
	.global Data_020047ac
Data_020047ac:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00010000
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00006666
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x02300000
	.4byte 0x00000000
	.4byte 0x017c0000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x02340000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x02180000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x02180000
	.4byte 0x00000000
	.4byte 0x017c0000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0x00000000
	.global Data_02004848
Data_02004848:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00010000
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00006666
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x02180000
	.4byte 0x00000000
	.4byte 0x017c0000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x02300000
	.4byte 0x00000000
	.4byte 0x017c0000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x02340000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x02180000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0x00000000
	.global Data_020048e4
Data_020048e4:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00010000
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00006666
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x02180000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x02180000
	.4byte 0x00000000
	.4byte 0x017c0000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x02300000
	.4byte 0x00000000
	.4byte 0x017c0000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x02340000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0x00000000
	.global Data_02004980
Data_02004980:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00b00000
	.4byte 0x00000000
	.4byte 0x009a0000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000b000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000011
	.global Data_020049c4
Data_020049c4:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00a00000
	.4byte 0x00000000
	.4byte 0x009a0000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000a000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000011
	.global Data_02004a08
Data_02004a08:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00900000
	.4byte 0x00000000
	.4byte 0x009a0000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.global Data_02004a44
Data_02004a44:
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
	.global Data_02004a74
Data_02004a74:
	.4byte 0x0000010d
	.4byte 0x0013e002
	.4byte 0x0020110e
	.4byte 0x00300003
	.4byte 0x0000010e
	.4byte 0x0010210d
	.4byte 0x00208110
	.4byte 0x00301110
	.4byte 0x0040a110
	.4byte 0x00504110
	.4byte 0x00605110
	.4byte 0x00702110
	.4byte 0x00807112
	.4byte 0x0090210f
	.4byte 0x00a09111
	.4byte 0x00b0110f
	.4byte 0x0000010f
	.4byte 0x0010b10e
	.4byte 0x0020910e
	.4byte 0x0033f002
	.4byte 0x00407110
	.4byte 0x00000110
	.4byte 0x0010310e
	.4byte 0x0020710e
	.4byte 0x00306111
	.4byte 0x0040510e
	.4byte 0x0050610e
	.4byte 0x00607111
	.4byte 0x0070410f
	.4byte 0x0080210e
	.4byte 0x00908111
	.4byte 0x00a0410e
	.4byte 0x00000111
	.4byte 0x00603110
	.4byte 0x00706110
	.4byte 0x00809110
	.4byte 0x0090a10e
	.4byte 0x00000112
	.4byte 0x0070810e
	.4byte 0x000001ff
	.global Data_02004b14
Data_02004b14:
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02004b2c
Data_02004b2c:
	.4byte 0xffff0006
	.4byte 0x00000001
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x0002c000
	.4byte 0xffff0007
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x0002c000
	.4byte 0xffff003e
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x0002d000
	.4byte 0xffff003c
	.4byte 0x00000001
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x0002d000
	.4byte 0xffff003d
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x0002b000
	.4byte 0xffff0003
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x0002c000
	.4byte 0xffff0002
	.4byte 0x00000001
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x0002c000
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x0002c000
	.4byte 0xffff0005
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x0002a000
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0000
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
	.global Data_02004c4c
Data_02004c4c:
	.4byte 0xffff00af
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x00033000
	.4byte 0xffff00ac
	.4byte 0x00000001
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x01500000
	.4byte 0x00015000
	.4byte 0xffff00ae
	.4byte 0x00000003
	.4byte 0x00700000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0xffff00b0
	.4byte 0x00000001
	.4byte 0x01c40000
	.4byte 0x00000000
	.4byte 0x012c0000
	.4byte 0x00015000
	.4byte 0xffff00ab
	.4byte 0x00000001
	.4byte 0x01140000
	.4byte 0x00000000
	.4byte 0x00940000
	.4byte 0x0001b000
	.4byte 0xffff00ad
	.4byte 0x00000001
	.4byte 0x02440000
	.4byte 0x00000000
	.4byte 0x015c0000
	.4byte 0x00038000
	.4byte 0xffff00b2
	.4byte 0x00000001
	.4byte 0x00f00000
	.4byte 0x00000000
	.4byte 0x00f40000
	.4byte 0x00033000
	.4byte 0xffff00b3
	.4byte 0x00000001
	.4byte 0x01760000
	.4byte 0x00000000
	.4byte 0x01a40000
	.4byte 0x00015000
	.4byte 0xffff00b2
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0xffff00b3
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0002c000
	.4byte 0xffff00b2
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00028000
	.4byte 0xffff0149
	.4byte 0x00000001
	.4byte 0x019c0000
	.4byte 0x00000000
	.4byte 0x01d80000
	.4byte 0x00024000
	.4byte 0xffff011e
	.4byte 0x00000007
	.4byte 0x02380000
	.4byte 0x00000000
	.4byte 0x00b00000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02004d9c
Data_02004d9c:
	.4byte 0xffff00af
	.4byte 0x00000001
	.4byte 0x021c0000
	.4byte 0x00000000
	.4byte 0x01840000
	.4byte 0x0003b000
	.4byte 0xffff00ac
	.4byte 0x00000001
	.4byte 0x02380000
	.4byte 0x00000000
	.4byte 0x01840000
	.4byte 0x0001b000
	.4byte 0xffff002c
	.4byte 0x00000001
	.4byte 0x00b40000
	.4byte 0x00000000
	.4byte 0x00840000
	.4byte 0x00035000
	.4byte 0xffff002d
	.4byte 0x00000001
	.4byte 0x008c0000
	.4byte 0x00000000
	.4byte 0x00840000
	.4byte 0x00033000
	.4byte 0xffff009b
	.4byte 0x00000001
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x00b20000
	.4byte 0x0001b000
	.4byte 0xffff009b
	.4byte 0x00000001
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x00b20000
	.4byte 0x0001b000
	.4byte 0xffff009b
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x00b20000
	.4byte 0x0001d000
	.4byte 0xffff0099
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00b20000
	.4byte 0x0001d000
	.4byte 0xffff0099
	.4byte 0x00000001
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x00b20000
	.4byte 0x0001d000
	.4byte 0xffff0099
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x00b20000
	.4byte 0x0001d000
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff011e
	.4byte 0x00000007
	.4byte 0x01b40000
	.4byte 0x00000000
	.4byte 0x00d00000
	.4byte 0x00024000
	.4byte 0x004000f3
	.4byte 0x00000001
	.4byte 0x01940000
	.4byte 0x00000000
	.4byte 0x01400000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02004f04
Data_02004f04:
	.4byte 0xffff00af
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0002b000
	.4byte 0xffff00ac
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0002b000
	.4byte 0xffff002c
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00025000
	.4byte 0xffff002d
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00023000
	.4byte 0xffff009b
	.4byte 0x00000001
	.4byte 0x00a00000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x0001b000
	.4byte 0xffff009b
	.4byte 0x00000001
	.4byte 0x00b00000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x0001b000
	.4byte 0xffff009b
	.4byte 0x00000003
	.4byte 0x00b00000
	.4byte 0x00000000
	.4byte 0x00a20000
	.4byte 0x0000d000
	.4byte 0xffff0099
	.4byte 0x00000001
	.4byte 0x00800000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x0000d000
	.4byte 0xffff0099
	.4byte 0x00000001
	.4byte 0x00920000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x0001d000
	.4byte 0xffff0099
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x00a00000
	.4byte 0x00033000
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff011e
	.4byte 0x00000007
	.4byte 0x01b40000
	.4byte 0x00000000
	.4byte 0x00d00000
	.4byte 0x00024000
	.4byte 0x004000f3
	.4byte 0x00000001
	.4byte 0x01940000
	.4byte 0x00000000
	.4byte 0x01400000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_0200506c
Data_0200506c:
	.4byte 0xffff00af
	.4byte 0x00000001
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x00c00000
	.4byte 0x00008000
	.4byte 0xffff00ac
	.4byte 0x00000001
	.4byte 0x00800000
	.4byte 0x00000000
	.4byte 0x006a0000
	.4byte 0x0001b000
	.4byte 0xffff00b2
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00700000
	.4byte 0x00005000
	.4byte 0xffff00b0
	.4byte 0x00000003
	.4byte 0x016c0000
	.4byte 0x00000000
	.4byte 0x007b0000
	.4byte 0x00013000
	.4byte 0xffff00ac
	.4byte 0x00000001
	.4byte 0x01d40000
	.4byte 0x00000000
	.4byte 0x00a00000
	.4byte 0x00003000
	.4byte 0xffff00ab
	.4byte 0x00000003
	.4byte 0x02d80000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00018000
	.4byte 0xffff00ad
	.4byte 0x00000001
	.4byte 0x02ae0000
	.4byte 0x00000000
	.4byte 0x006f0000
	.4byte 0x0001d000
	.4byte 0xffff00b2
	.4byte 0x00000001
	.4byte 0x02240000
	.4byte 0x00000000
	.4byte 0x03300000
	.4byte 0x00008000
	.4byte 0xffff00b3
	.4byte 0x00000001
	.4byte 0x01d20000
	.4byte 0x00000000
	.4byte 0x032a0000
	.4byte 0x0001d000
	.4byte 0xffff00b2
	.4byte 0x00000001
	.4byte 0x01e00000
	.4byte 0x00000000
	.4byte 0x03200000
	.4byte 0x00015000
	.4byte 0xffff00ac
	.4byte 0x00000001
	.4byte 0x02120000
	.4byte 0x00000000
	.4byte 0x03200000
	.4byte 0x00035000
	.4byte 0xffff00ac
	.4byte 0x00000003
	.4byte 0x00a40000
	.4byte 0x00000000
	.4byte 0x020c0000
	.4byte 0x00010000
	.4byte 0xffff00ab
	.4byte 0x00000003
	.4byte 0x00de0000
	.4byte 0x00000000
	.4byte 0x01ea0000
	.4byte 0x00015000
	.4byte 0xffff00ad
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x018a0000
	.4byte 0x0001d000
	.4byte 0xffff00ae
	.4byte 0x00000001
	.4byte 0x010c0000
	.4byte 0x00000000
	.4byte 0x031a0000
	.4byte 0x00015000
	.4byte 0xffff00b0
	.4byte 0x00000001
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x03300000
	.4byte 0x00013000
	.4byte 0x005700f4
	.4byte 0x00000001
	.4byte 0x02380000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00008000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_0200521c
Data_0200521c:
	.4byte 0xffff00af
	.4byte 0x00000003
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x00c40000
	.4byte 0x00008000
	.4byte 0xffff00ac
	.4byte 0x00000001
	.4byte 0x00800000
	.4byte 0x00000000
	.4byte 0x006a0000
	.4byte 0x0001b000
	.4byte 0xffff00b2
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00700000
	.4byte 0x00005000
	.4byte 0xffff00b0
	.4byte 0x00000003
	.4byte 0x016c0000
	.4byte 0x00000000
	.4byte 0x007b0000
	.4byte 0x00013000
	.4byte 0xffff00ac
	.4byte 0x00000001
	.4byte 0x01d40000
	.4byte 0x00000000
	.4byte 0x00a00000
	.4byte 0x00003000
	.4byte 0xffff00ab
	.4byte 0x00000003
	.4byte 0x02d80000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00018000
	.4byte 0xffff00ad
	.4byte 0x00000003
	.4byte 0x029a0000
	.4byte 0x00000000
	.4byte 0x00860000
	.4byte 0x0001d000
	.4byte 0xffff00b2
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00025000
	.4byte 0xffff00b3
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0002d000
	.4byte 0xffff00b2
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00025000
	.4byte 0xffff00ac
	.4byte 0x00000001
	.4byte 0x02120000
	.4byte 0x00000000
	.4byte 0x03200000
	.4byte 0x00035000
	.4byte 0xffff00ac
	.4byte 0x00000001
	.4byte 0x00ea0000
	.4byte 0x00000000
	.4byte 0x01b80000
	.4byte 0x00005000
	.4byte 0xffff00ab
	.4byte 0x00000003
	.4byte 0x00e00000
	.4byte 0x00000000
	.4byte 0x022a0000
	.4byte 0x00005000
	.4byte 0xffff00ad
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0002d000
	.4byte 0xffff00ae
	.4byte 0x00000001
	.4byte 0x010c0000
	.4byte 0x00000000
	.4byte 0x031a0000
	.4byte 0x00015000
	.4byte 0xffff00b0
	.4byte 0x00000001
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x03300000
	.4byte 0x00013000
	.4byte 0x005700f4
	.4byte 0x00000001
	.4byte 0x02380000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00008000
	.4byte 0xffff002c
	.4byte 0x00000001
	.4byte 0x00c00000
	.4byte 0x00000000
	.4byte 0x01ac0000
	.4byte 0x00035000
	.4byte 0xffff002d
	.4byte 0x00000001
	.4byte 0x00b00000
	.4byte 0x00000000
	.4byte 0x01ac0000
	.4byte 0x00013000
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0005
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
	.global Data_02005444
Data_02005444:
	.4byte 0xffff00ae
	.4byte 0x00000001
	.4byte 0x02740000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00015000
	.4byte 0xffff00ae
	.4byte 0x00000001
	.4byte 0x01740000
	.4byte 0x00000000
	.4byte 0x03280000
	.4byte 0x0003b000
	.4byte 0xffff00b0
	.4byte 0x00000001
	.4byte 0x01720000
	.4byte 0x00000000
	.4byte 0x02e40000
	.4byte 0x00015000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020054a4
Data_020054a4:
	.4byte 0xffff00ae
	.4byte 0x00000003
	.4byte 0x029a0000
	.4byte 0x00000000
	.4byte 0x007c0000
	.4byte 0x00015000
	.4byte 0xffff00ae
	.4byte 0x00000001
	.4byte 0x01740000
	.4byte 0x00000000
	.4byte 0x03280000
	.4byte 0x0003b000
	.4byte 0xffff00b0
	.4byte 0x00000001
	.4byte 0x01720000
	.4byte 0x00000000
	.4byte 0x02e40000
	.4byte 0x00015000
	.4byte 0xffff00ad
	.4byte 0x00000001
	.4byte 0x02980000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x00005000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_0200551c
Data_0200551c:
	.4byte 0xffff00c9
	.4byte 0x00000001
	.4byte 0x00b00000
	.4byte 0x00000000
	.4byte 0x03090000
	.4byte 0x00015000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_0200554c
Data_0200554c:
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02005558
Data_02005558:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000002
	.4byte 0x09cc000a
	.4byte Func_02002a8c
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02005588
Data_02005588:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x0000c602
	.4byte 0xffff0002
	.4byte Func_02001a9c
	.4byte 0x0000c602
	.4byte 0xffff0003
	.4byte Func_02001ab8
	.4byte 0x0000ce01
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x0000c602
	.4byte 0xffff0005
	.4byte Func_02001ad4
	.4byte 0x0000c602
	.4byte 0xffff0006
	.4byte Func_02001af0
	.4byte 0x0000c602
	.4byte 0xffff0007
	.4byte Func_02001b0c
	.4byte 0x0000c602
	.4byte 0xffff0008
	.4byte Func_02001b28
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x0000c602
	.4byte 0xffff000a
	.4byte Func_02001b44
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00000000
	.4byte 0x0a3d0008
	.4byte 0x00002cdd
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00002ea0
	.4byte 0x00008d15
	.4byte 0x0a3d0008
	.4byte 0x00002ce5
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00002ea8
	.4byte 0x00000000
	.4byte 0x0a3d0009
	.4byte 0x00002cde
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00002ea1
	.4byte 0x00008d15
	.4byte 0x0a3d0009
	.4byte 0x00002ce6
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00002ea9
	.4byte 0x00000000
	.4byte 0x0a3d000a
	.4byte 0x00002cdf
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x00002ea2
	.4byte 0x00008d15
	.4byte 0x0a3d000a
	.4byte 0x00002ce7
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00002eaa
	.4byte 0x00000000
	.4byte 0x0a3d000b
	.4byte 0x00002ce0
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x00002ea3
	.4byte 0x00008d15
	.4byte 0x0a3d000b
	.4byte 0x00002ce8
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00002eab
	.4byte 0x00000000
	.4byte 0x0a3d000c
	.4byte 0x00002ce1
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x00002ea4
	.4byte 0x00008d15
	.4byte 0x0a3d000c
	.4byte 0x00002ce9
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00002eac
	.4byte 0x00000000
	.4byte 0x0a3d000d
	.4byte 0x00002ce2
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x00002ea5
	.4byte 0x00008d15
	.4byte 0x0a3d000d
	.4byte 0x00002cea
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00002ead
	.4byte 0x00000000
	.4byte 0x0a3d000e
	.4byte 0x00002ce3
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x00002ea6
	.4byte 0x00008d15
	.4byte 0x0a3d000e
	.4byte 0x00002ceb
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x00002eae
	.4byte 0x00000000
	.4byte 0x0a3d000f
	.4byte 0x00002ce4
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x00002ea7
	.4byte 0x00008d15
	.4byte 0x0a3d000f
	.4byte 0x00002cec
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x00002eaf
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte Func_02000764
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte Func_02000764
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte Func_020007f4
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte Func_020007f4
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte Func_02000884
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte Func_02000884
	.4byte 0x50008805
	.4byte gHeapSlots + 0x64
	.4byte Func_02000588
	.4byte 0x00008f15
	.4byte 0xffff0013
	.4byte 0x00000000
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020057f8
Data_020057f8:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x0000c602
	.4byte 0xffff0004
	.4byte Func_02001b60
	.4byte 0x00000002
	.4byte 0x0a2d0009
	.4byte Func_020015d0
	.4byte 0x00000002
	.4byte 0xffff000a
	.4byte Func_0200391c
	.4byte 0x10008805
	.4byte 0xffff00ff
	.4byte Func_020017e4
	.4byte 0x00008805
	.4byte 0xffff00ff
	.4byte Func_02001800
	.4byte 0x50008805
	.4byte 0xffff0008
	.4byte Func_02001770
	.4byte 0x00000006
	.4byte 0x0a3d00c8
	.4byte Func_02000280
	.4byte 0x00000002
	.4byte 0x09c0000b
	.4byte Func_02000914
	.4byte 0x00000002
	.4byte 0x09c1000c
	.4byte Func_02000ad8
	.4byte 0x00000002
	.4byte 0x09c2000d
	.4byte Func_02000dac
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00002d15
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00002d17
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00002d16
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00002d18
	.4byte 0x00000000
	.4byte 0x0a3d000c
	.4byte 0x00002d33
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x00002edc
	.4byte 0x00008d15
	.4byte 0x0a3d000c
	.4byte 0x00002d39
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00002ee2
	.4byte 0x00000000
	.4byte 0x0a3d000d
	.4byte 0x00002d34
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x00002edd
	.4byte 0x00008d15
	.4byte 0x0a3d000d
	.4byte 0x00002d3a
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00002ee3
	.4byte 0x00000000
	.4byte 0x0a3d000e
	.4byte 0x00002d35
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x00002ede
	.4byte 0x00008d15
	.4byte 0x0a3d000e
	.4byte 0x00002d3b
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x00002ee4
	.4byte 0x00000000
	.4byte 0x0a3d000f
	.4byte 0x00002d36
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x00002edf
	.4byte 0x00008d15
	.4byte 0x0a3d000f
	.4byte 0x00002d3c
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x00002ee5
	.4byte 0x00000000
	.4byte 0x0a3d0010
	.4byte 0x00002d37
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x00002ee0
	.4byte 0x00008d15
	.4byte 0x0a3d0010
	.4byte 0x00002d3d
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x00002ee6
	.4byte 0x00000000
	.4byte 0x0a3d0011
	.4byte 0x00002d38
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x00002ee1
	.4byte 0x00008d15
	.4byte 0x0a3d0011
	.4byte 0x00002d3e
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x00002ee7
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x00002d66
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00002d68
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x00002d67
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00002d69
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02005a20
Data_02005a20:
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
	.4byte 0x00000000
	.4byte 0x0a3d0008
	.4byte 0x00002ced
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00002eb0
	.4byte 0x00008d15
	.4byte 0x0a3d0008
	.4byte 0x00002cf0
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00002eb3
	.4byte 0x00000000
	.4byte 0x0a3d0009
	.4byte 0x00002cee
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00002eb1
	.4byte 0x00008d15
	.4byte 0x0a3d0009
	.4byte 0x00002cf1
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00002eb4
	.4byte 0x00000000
	.4byte 0x0a3d000a
	.4byte 0x00002cef
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x00002eb2
	.4byte 0x00008d15
	.4byte 0x0a3d000a
	.4byte 0x00002cf2
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00002eb5
	.4byte 0x00000000
	.4byte 0x0a3d000b
	.4byte 0x00002cf4
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x00002eb7
	.4byte 0x00008d15
	.4byte 0x0a3d000b
	.4byte 0x00002cf7
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00002eba
	.4byte 0x00000000
	.4byte 0x0a3d000c
	.4byte 0x00002cf5
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x00002eb8
	.4byte 0x00008d15
	.4byte 0x0a3d000c
	.4byte 0x00002cf8
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00002ebb
	.4byte 0x00000000
	.4byte 0x0a3d000d
	.4byte 0x00002cf9
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x00002ebc
	.4byte 0x00008d15
	.4byte 0x0a3d000d
	.4byte 0x00002cfb
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00002ebe
	.4byte 0x00000000
	.4byte 0x0a3d000e
	.4byte 0x00002cfa
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x00002ebd
	.4byte 0x00008d15
	.4byte 0x0a3d000e
	.4byte 0x00002cfc
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x00002ebf
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte Func_02000620
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x00002d03
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x00002d00
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x00002d04
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x00002d01
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x00002d05
	.4byte 0x00000000
	.4byte 0x0a3d0012
	.4byte 0x00002d02
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x00002ec3
	.4byte 0x00008d15
	.4byte 0x0a3d0012
	.4byte 0x00002d06
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x00002ec7
	.4byte 0x00000000
	.4byte 0x0a3d0013
	.4byte 0x00002d07
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x00002eca
	.4byte 0x00008d15
	.4byte 0x0a3d0013
	.4byte 0x00002d0a
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x00002ecf
	.4byte 0x00000000
	.4byte 0x0a3d0014
	.4byte 0x00002d08
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte 0x00002ecb
	.4byte 0x00008d15
	.4byte 0x0a3d0014
	.4byte 0x00002d0b
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte 0x00002ed0
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte 0x00002d09
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte 0x00002d0c
	.4byte 0x00000000
	.4byte 0xffff0019
	.4byte 0x00002ec8
	.4byte 0x00008d15
	.4byte 0xffff0019
	.4byte 0x00002ecd
	.4byte 0x00000000
	.4byte 0xffff001a
	.4byte 0x00002ec9
	.4byte 0x00008d15
	.4byte 0xffff001a
	.4byte 0x00002ece
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte Func_02000674
	.4byte 0x00008d15
	.4byte 0x0a3d0016
	.4byte 0x00002d1f
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte 0x00002ed8
	.4byte 0x00000000
	.4byte 0x0a3d0017
	.4byte 0x00002d1e
	.4byte 0x00000000
	.4byte 0xffff0017
	.4byte 0x00002ed7
	.4byte 0x00008d15
	.4byte 0x0a3d0017
	.4byte 0x00002d20
	.4byte 0x00008d15
	.4byte 0xffff0017
	.4byte 0x00002ed9
	.4byte 0x00000000
	.4byte 0xffff0018
	.4byte Func_02000290
	.4byte 0x00000173
	.4byte 0xffff00c8
	.4byte 0x0040305f
	.4byte 0x00000173
	.4byte 0xffff00c9
	.4byte 0x00403060
	.4byte 0x00000173
	.4byte 0xffff00ca
	.4byte 0x00403061
	.4byte 0x00000173
	.4byte 0xffff00cb
	.4byte 0x00403062
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02005da4
Data_02005da4:
	.4byte 0x0000ca02
	.4byte 0xffff0006
	.4byte Func_02001b7c
	.4byte 0x0000ca02
	.4byte 0xffff0007
	.4byte Func_02001b7c
	.4byte 0x0000ca02
	.4byte 0xffff0008
	.4byte Func_02001b7c
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000000
	.4byte 0x0a3d0008
	.4byte 0x00002cf3
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00002eb6
	.4byte 0x00008d15
	.4byte 0x0a3d0008
	.4byte 0x00002cf6
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00002eb9
	.4byte 0x00000000
	.4byte 0x0a3d0009
	.4byte 0x00002d19
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00002ed2
	.4byte 0x00008d15
	.4byte 0x0a3d0009
	.4byte 0x00002d1b
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00002ed4
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte Func_020006c4
	.4byte 0x00008d15
	.4byte 0x0a3d000a
	.4byte 0x00002d1c
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00002ed5
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x00002ecc
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00002ed1
	.4byte 0x00000173
	.4byte 0xffff00c8
	.4byte 0x0040305f
	.4byte 0x00000173
	.4byte 0xffff00c9
	.4byte 0x00403060
	.4byte 0x00000173
	.4byte 0xffff00ca
	.4byte 0x00403061
	.4byte 0x00000173
	.4byte 0xffff00cb
	.4byte 0x00403062
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02005eac
Data_02005eac:
	.4byte 0x00000001
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte Func_02000714
	.4byte 0x00008d15
	.4byte 0x0a3d0008
	.4byte 0x00002d22
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00002edb
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02005ee8
Data_02005ee8:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x00c40000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000011
	.global Data_02005f2c
Data_02005f2c:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00c40000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00005000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000011
	.global Data_02005f70
Data_02005f70:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x015c0000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.global Data_02005fac
Data_02005fac:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x015c0000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.global Data_02005fe8
Data_02005fe8:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x015c0000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.global Data_02006038
Data_02006038:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x015c0000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.global Data_02006088
Data_02006088:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x01100000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.global Data_020060cc
Data_020060cc:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x01100000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.global Data_020060fc
Data_020060fc:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x00dc0000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000e000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000c000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.global Data_0200617c
Data_0200617c:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x008c0000
	.4byte 0x00000000
	.4byte 0x00dc0000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00006000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000a000
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000c000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.global Data_020061fc
Data_020061fc:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00f00000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000011
	.global Data_0200622c
Data_0200622c:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x015c0000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.global Data_02006268
Data_02006268:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x015c0000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
