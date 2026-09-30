.syntax unified
	.thumb
	.section .text.x02008038,"ax",%progbits
	.global Func_02000038
	.thumb_func
Func_02000038:
	push {r5, lr}
	adds r5, r0, #0
	adds r4, r1, #0
	cmp r5, #0
	beq .L_0200807c
	adds r3, r5, #0
	adds r3, #84
	ldrb r2, [r3]
	movs r3, #15
	ands r3, r2
	cmp r3, #0
	beq .L_0200807c
	ldr r1, [r5, #80]
	movs r2, #13
	ldrb r0, [r1, #9]
	movs r3, #3
	negs r2, r2
	ands r4, r3
	adds r3, r2, #0
	lsls r4, r4, #2
	ands r3, r0
	orrs r3, r4
	strb r3, [r1, #9]
	adds r1, #37
	ldrb r3, [r1]
	ands r2, r3
	orrs r2, r4
	strb r2, [r1]
	adds r1, r5, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #254
	ands r3, r2
	strb r3, [r1]
.L_0200807c:
	pop {r5, pc}
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
	bl Func_02005140
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
	bl Func_02005130
	ldr r2, .L_02008274
	mov r3, r8
	ands r3, r5
	lsls r3, r3, #2
	ldr r1, [r2, r3]
	adds r0, r6, #0
	mov r10, r3
	bl Func_02005138
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
	bl Func_02000038
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
	bl Func_02005278
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
	bl Func_02000038
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
	bl Func_02005130
	ldr r1, [r7, #28]
	adds r0, r6, #0
	bl Func_02005138
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
	.4byte Data_02005508
.L_02008278:
	.4byte Func_02000080
.L_0200827c:
	.4byte 0xffff0000
	.section .text.x02008280,"ax",%progbits
	.global Func_02000280
	.thumb_func
Func_02000280:
	push {lr}
	movs r0, #11
	movs r1, #27
	bl Func_02005308
	pop {pc}
	.section .text.x0200828c,"ax",%progbits
	.global Func_0200028c
	.thumb_func
Func_0200028c:
	adds r0, #102
	ldrh r3, [r0]
	subs r3, #1
	strh r3, [r0]
	lsls r3, r3, #16
	asrs r3, r3, #16
	negs r0, r3
	orrs r0, r3
	lsrs r0, r0, #31
	bx lr
	.section .text.x020082a0,"ax",%progbits
	.global Func_020002a0
	.thumb_func
Func_020002a0:
	push {r5, lr}
	adds r5, r0, #0
	adds r5, #100
	movs r1, #0
	ldrsh r3, [r5, r1]
	ldrh r2, [r5]
	cmp r3, #0
	beq .L_020082b4
	subs r3, r2, #1
	b .L_020082d4
.L_020082b4:
	adds r3, r0, #0
	adds r3, #102
	movs r1, #6
	movs r2, #0
	ldrsh r0, [r3, r2]
	adds r1, #255
	movs r2, #0
	bl Func_020052c8
	bl Random16Far
	lsls r3, r0, #4
	subs r3, r3, r0
	lsls r3, r3, #4
	lsrs r3, r3, #16
	adds r3, #240
.L_020082d4:
	strh r3, [r5]
	movs r0, #1
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x020082dc,"ax",%progbits
	.global Func_020002dc
	.thumb_func
Func_020002dc:
	push {r5, lr}
	adds r5, r0, #0
	adds r5, #100
	movs r2, #0
	ldrsh r3, [r5, r2]
	cmp r3, #2
	beq .L_0200831a
	cmp r3, #2
	bgt .L_020082f4
	cmp r3, #0
	beq .L_0200832e
	b .L_02008344
.L_020082f4:
	cmp r3, #4
	beq .L_0200830c
	cmp r3, #6
	bne .L_02008344
	ldr r3, [r0, #24]
	ldr r2, .L_02008350
	adds r3, r3, r2
	str r3, [r0, #24]
	ldr r3, [r0, #28]
	movs r2, #128
	lsls r2, r2, #6
	b .L_02008328
.L_0200830c:
	ldr r3, [r0, #24]
	movs r2, #128
	lsls r2, r2, #6
	adds r3, r3, r2
	str r3, [r0, #24]
	ldr r2, .L_02008354
	b .L_02008326
.L_0200831a:
	ldr r3, [r0, #24]
	movs r2, #128
	lsls r2, r2, #5
	adds r3, r3, r2
	str r3, [r0, #24]
	ldr r2, .L_02008358
.L_02008326:
	ldr r3, [r0, #28]
.L_02008328:
	adds r3, r3, r2
	str r3, [r0, #28]
	b .L_02008344
.L_0200832e:
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r0, #24]
	str r3, [r0, #28]
	bl Random16Far
	movs r1, #90
	bl Engine_MathModulo
	adds r0, #60
	strh r0, [r5]
.L_02008344:
	ldrh r3, [r5]
	movs r0, #1
	subs r3, #1
	strh r3, [r5]
	pop {r5, pc}
	.2byte 0x0000
.L_02008350:
	.4byte 0xffffc000
.L_02008354:
	.4byte 0xfffff000
.L_02008358:
	.4byte 0xfffff800
	.section .text.x0200835c,"ax",%progbits
	.global Func_0200035c
	.thumb_func
Func_0200035c:
	push {lr}
	adds r2, r0, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	movs r3, #204
	lsls r3, r3, #8
	adds r3, #204
	str r3, [r0, #24]
	str r3, [r0, #28]
	adds r1, r0, #0
	adds r1, #89
	ldrb r2, [r1]
	movs r3, #8
	orrs r3, r2
	strb r3, [r1]
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #0
	pop {pc}
	.2byte 0x0000
	.section .text.x02008388,"ax",%progbits
	.global Func_02000388
	.thumb_func
Func_02000388:
	ldr r0, .L_0200838c
	bx lr
.L_0200838c:
	.4byte Data_02005670
	.section .text.x02008390,"ax",%progbits
	.global Func_02000390
	.thumb_func
Func_02000390:
	push {lr}
	ldr r3, .L_020083b4
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_020083b8
	cmp r2, r3
	bne .L_020083a8
	ldr r0, .L_020083bc
	b .L_020083b2
.L_020083a8:
	ldr r3, .L_020083c0
	movs r0, #0
	cmp r2, r3
	bne .L_020083b2
	ldr r0, .L_020083c4
.L_020083b2:
	pop {pc}
.L_020083b4:
	.4byte gPartyState
.L_020083b8:
	.4byte 0x00000010
.L_020083bc:
	.4byte Data_020056a0
.L_020083c0:
	.4byte 0x00000012
.L_020083c4:
	.4byte Data_020056c0
	.section .text.x020083c8,"ax",%progbits
	.global Func_020003c8
	.thumb_func
Func_020003c8:
	ldr r0, .L_020083cc
	bx lr
.L_020083cc:
	.4byte Data_020056e0
	.section .text.x020083d0,"ax",%progbits
	.global Func_020003d0
	.thumb_func
Func_020003d0:
	push {lr}
	ldr r3, .L_0200846c
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008470
	cmp r2, r3
	bne .L_020083e8
	ldr r0, .L_02008474
	b .L_02008468
.L_020083e8:
	ldr r3, .L_02008478
	cmp r2, r3
	bne .L_020083f2
	ldr r0, .L_0200847c
	b .L_02008468
.L_020083f2:
	ldr r3, .L_02008480
	cmp r2, r3
	bne .L_020083fc
	ldr r0, .L_02008484
	b .L_02008468
.L_020083fc:
	ldr r3, .L_02008488
	cmp r2, r3
	bne .L_02008406
	ldr r0, .L_0200848c
	b .L_02008468
.L_02008406:
	ldr r3, .L_02008490
	cmp r2, r3
	bne .L_02008430
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #73
	bl Func_02005110
	cmp r0, #0
	beq .L_0200842c
	ldr r2, .L_02008494
	movs r3, #0
	adds r1, r2, #0
	adds r1, #94
	strb r3, [r1]
	adds r2, #142
	adds r1, #24
	strb r3, [r1]
	strb r3, [r2]
.L_0200842c:
	ldr r0, .L_02008494
	b .L_02008468
.L_02008430:
	ldr r3, .L_02008498
	cmp r2, r3
	bne .L_02008466
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #73
	bl Func_02005110
	cmp r0, #0
	beq .L_02008462
	ldr r1, .L_0200849c
	movs r3, #3
	adds r2, r1, #0
	adds r2, #142
	strb r3, [r2]
	movs r3, #184
	adds r2, #10
	lsls r3, r3, #17
	str r3, [r2]
	ldr r3, .L_020084a0
	adds r2, #8
	str r3, [r2]
	adds r2, #30
	movs r3, #1
	strb r3, [r2]
.L_02008462:
	ldr r0, .L_0200849c
	b .L_02008468
.L_02008466:
	ldr r0, .L_020084a4
.L_02008468:
	pop {pc}
	.2byte 0x0000
.L_0200846c:
	.4byte gPartyState
.L_02008470:
	.4byte 0x0000000e
.L_02008474:
	.4byte Data_020057c0
.L_02008478:
	.4byte 0x00000010
.L_0200847c:
	.4byte Data_020058b0
.L_02008480:
	.4byte 0x00000011
.L_02008484:
	.4byte Data_02005958
.L_02008488:
	.4byte 0x00000014
.L_0200848c:
	.4byte Data_02005aa8
.L_02008490:
	.4byte 0x00000015
.L_02008494:
	.4byte Data_02005b20
.L_02008498:
	.4byte 0x00000016
.L_0200849c:
	.4byte Data_02005be0
.L_020084a0:
	.4byte 0x02b60000
.L_020084a4:
	.4byte Data_020057a8
	.section .text.x020084a8,"ax",%progbits
	.global Func_020004a8
	.thumb_func
Func_020004a8:
	push {lr}
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #66
	bl Func_02005110
	cmp r0, #0
	bne .L_02008574
	movs r0, #140
	lsls r0, r0, #2
	bl Func_02005118
	bl Func_020051c0
	movs r0, #0
	bl Func_02005370
	movs r0, #4
	movs r1, #104
	movs r2, #252
	bl ObjectMotion_SetPositionAndReset
	movs r1, #160
	movs r0, #4
	lsls r1, r1, #8
	movs r2, #0
	bl Func_020052a8
	movs r1, #16
	movs r2, #16
	movs r3, #192
	lsls r3, r3, #8
	negs r2, r2
	negs r1, r1
	movs r0, #14
	bl Func_02005378
	movs r0, #14
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #20
	bl Battle_WaitMode0
	ldr r0, .L_02008578
	bl Func_02005280
	movs r0, #14
	movs r1, #0
	bl Func_02005298
	movs r1, #128
	lsls r1, r1, #6
	movs r0, #14
	bl Func_020052b0
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #14
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #14
	ldr r1, .L_0200857c
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r0, #14
	movs r1, #2
	bl Object_SetModeById
	movs r0, #4
	bl Object_GetById
	cmp r0, #0
	beq .L_02008560
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #14
	bl ObjectMotion_ResetAndSetPosition
.L_02008560:
	movs r0, #14
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #14
	movs r1, #0
	movs r2, #0
	bl Func_02005230
	bl Func_020051c8
.L_02008574:
	pop {pc}
	.2byte 0x0000
.L_02008578:
	.4byte 0x000017b9
.L_0200857c:
	.4byte 0x00013333
	.section .text.x02008580,"ax",%progbits
	.global Func_02000580
	.thumb_func
Func_02000580:
	push {lr}
	ldr r3, .L_020085ec
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_020085f0
	cmp r2, r3
	bne .L_02008598
	ldr r0, .L_020085f4
	b .L_020085ea
.L_02008598:
	ldr r3, .L_020085f8
	cmp r2, r3
	bne .L_020085a2
	ldr r0, .L_020085fc
	b .L_020085ea
.L_020085a2:
	ldr r3, .L_02008600
	cmp r2, r3
	bne .L_020085ac
	ldr r0, .L_02008604
	b .L_020085ea
.L_020085ac:
	ldr r3, .L_02008608
	cmp r2, r3
	bne .L_020085b6
	ldr r0, .L_0200860c
	b .L_020085ea
.L_020085b6:
	ldr r3, .L_02008610
	cmp r2, r3
	bne .L_020085c0
	ldr r0, .L_02008614
	b .L_020085ea
.L_020085c0:
	ldr r3, .L_02008618
	cmp r2, r3
	bne .L_020085ca
	ldr r0, .L_0200861c
	b .L_020085ea
.L_020085ca:
	ldr r3, .L_02008620
	cmp r2, r3
	bne .L_020085d4
	ldr r0, .L_02008624
	b .L_020085ea
.L_020085d4:
	ldr r3, .L_02008628
	cmp r2, r3
	bne .L_020085de
	ldr r0, .L_0200862c
	b .L_020085ea
.L_020085de:
	ldr r3, .L_02008630
	cmp r2, r3
	bne .L_020085e8
	ldr r0, .L_02008634
	b .L_020085ea
.L_020085e8:
	ldr r0, .L_02008638
.L_020085ea:
	pop {pc}
.L_020085ec:
	.4byte gPartyState
.L_020085f0:
	.4byte 0x0000000e
.L_020085f4:
	.4byte Data_02005d54
.L_020085f8:
	.4byte 0x0000000f
.L_020085fc:
	.4byte Data_02005de4
.L_02008600:
	.4byte 0x00000010
.L_02008604:
	.4byte Data_02005e20
.L_02008608:
	.4byte 0x00000011
.L_0200860c:
	.4byte Data_02005e8c
.L_02008610:
	.4byte 0x00000012
.L_02008614:
	.4byte Data_02005ef8
.L_02008618:
	.4byte 0x00000013
.L_0200861c:
	.4byte Data_02005f10
.L_02008620:
	.4byte 0x00000014
.L_02008624:
	.4byte Data_02005f40
.L_02008628:
	.4byte 0x00000015
.L_0200862c:
	.4byte Data_02005fc4
.L_02008630:
	.4byte 0x00000016
.L_02008634:
	.4byte Data_0200615c
.L_02008638:
	.4byte Data_02005d48
	.section .text.x0200863c,"ax",%progbits
	.global Func_0200063c
	.thumb_func
Func_0200063c:
	push {r5, r6, lr}
	ldr r5, .L_02008664
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	ldr r0, [r5]
	bl Object_GetById
	adds r6, r0, #0
	ldr r0, [r5]
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	adds r0, r6, #0
	movs r1, #0
	bl Func_02000668
	pop {r5, r6, pc}
.L_02008664:
	.4byte gPartyState
	.section .text.x02008668,"ax",%progbits
	.global Func_02000668
	.thumb_func
Func_02000668:
	push {r5, lr}
	ldr r2, [r0, #44]
	ldr r3, [r0, #36]
	adds r3, r3, r2
	asrs r3, r3, #1
	ldr r2, .L_020086ac
	cmp r3, #0
	bge .L_0200867a
	negs r3, r3
.L_0200867a:
	movs r4, #187
	lsls r4, r4, #8
	adds r4, #128
	cmp r3, r4
	bls .L_0200868a
	ldr r3, [r2]
	adds r3, #4
	b .L_02008692
.L_0200868a:
	cmp r3, #0
	beq .L_02008694
	ldr r3, [r2]
	adds r3, #2
.L_02008692:
	str r3, [r2]
.L_02008694:
	ldr r5, .L_020086ac
	ldr r3, [r5]
	adds r3, r3, r1
	str r3, [r5]
	cmp r3, #40
	ble .L_020086a8
	bl Func_020006b0
	movs r3, #0
	str r3, [r5]
.L_020086a8:
	pop {r5, pc}
	.2byte 0x0000
.L_020086ac:
	.4byte gOverlayArea + 0x6294
	.section .text.x020086b0,"ax",%progbits
	.global Func_020006b0
	.thumb_func
Func_020006b0:
	push {r5, r6, r7, lr}
	ldr r1, [r0, #8]
	ldr r2, [r0, #12]
	ldr r3, [r0, #16]
	movs r0, #14
	adds r0, #255
	bl Func_02005140
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02008708
	ldr r1, .L_0200870c
	ldr r6, [r5, #80]
	bl Func_02005138
	adds r3, r5, #0
	adds r3, #85
	movs r7, #0
	adds r2, r5, #0
	strb r7, [r3]
	adds r2, #34
	movs r3, #1
	strb r3, [r2]
	adds r2, #1
	movs r3, #2
	strb r3, [r2]
	cmp r6, #0
	beq .L_02008708
	adds r0, r6, #0
	movs r1, #2
	bl Animation_ApplyChildArgument
	ldrb r2, [r6, #5]
	movs r3, #13
	negs r3, r3
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	strb r3, [r6, #5]
	ldrb r3, [r6, #9]
	movs r2, #12
	orrs r3, r2
	strb r7, [r6, #26]
	strb r3, [r6, #9]
.L_02008708:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200870c:
	.4byte Data_020054e4
	.section .text.x02008710,"ax",%progbits
	.global Func_02000710
	.thumb_func
Func_02000710:
	push {r5, r6, lr}
	ldr r3, .L_0200876c
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	ldr r3, .L_02008770
	movs r2, #7
	ldr r3, [r3]
	adds r6, r0, #0
	ands r3, r2
	cmp r3, #0
	bne .L_0200875c
	ldr r2, [r6, #12]
	movs r3, #192
	lsls r3, r3, #11
	adds r2, r2, r3
	ldr r1, [r6, #8]
	ldr r3, [r6, #16]
	movs r0, #14
	bl Func_02005140
	adds r5, r0, #0
	cmp r5, #0
	beq .L_0200875c
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	adds r0, r5, #0
	movs r1, #1
	bl Func_02005130
	ldr r1, .L_02008774
	adds r0, r5, #0
	bl Func_02005138
.L_0200875c:
	movs r1, #217
	lsls r1, r1, #8
	adds r1, #153
	adds r0, r6, #0
	bl Func_02000778
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200876c:
	.4byte gPartyState
.L_02008770:
	.4byte Data_0300122c
.L_02008774:
	.4byte Data_020054f0
	.section .text.x02008778,"ax",%progbits
	.global Func_02000778
	.thumb_func
Func_02000778:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #230
	lsls r2, r2, #1
	adds r3, r3, r2
	ldr r3, [r3]
	adds r6, r0, #0
	mov r8, r3
	ldr r3, [r6, #8]
	sub sp, #12
	mov r5, sp
	str r3, [r5]
	ldr r3, [r6, #12]
	adds r7, r1, #0
	str r3, [r5, #4]
	ldr r3, [r6, #16]
	adds r1, r5, #0
	adds r3, r3, r7
	str r3, [r5, #8]
	bl Func_02005180
	ldr r3, [r6, #8]
	movs r2, #128
	str r3, [r5]
	ldr r3, [r6, #12]
	lsls r2, r2, #12
	str r3, [r5, #4]
	ldr r3, [r6, #16]
	adds r0, r6, #0
	adds r3, r3, r2
	adds r1, r5, #0
	str r3, [r5, #8]
	bl Func_02005180
	cmp r0, #0
	bgt .L_02008814
	ldr r2, .L_0200881c
	ldr r3, [r6, #8]
	adds r0, r6, #0
	adds r3, r3, r2
	str r3, [r5]
	ldr r3, [r6, #12]
	adds r1, r5, #0
	str r3, [r5, #4]
	ldr r3, [r6, #16]
	adds r3, r3, r2
	str r3, [r5, #8]
	bl Func_02005180
	cmp r0, #0
	bgt .L_02008814
	ldr r2, .L_02008820
	ldr r3, [r6, #8]
	adds r0, r6, #0
	adds r3, r3, r2
	str r3, [r5]
	ldr r3, [r6, #12]
	ldr r2, .L_0200881c
	str r3, [r5, #4]
	ldr r3, [r6, #16]
	adds r1, r5, #0
	adds r3, r3, r2
	str r3, [r5, #8]
	bl Func_02005180
	cmp r0, #0
	bgt .L_02008814
	mov r2, r8
	ldr r3, [r2, #16]
	adds r3, r3, r7
	str r3, [r2, #16]
	ldr r3, [r6, #16]
	adds r3, r3, r7
	str r3, [r6, #16]
.L_02008814:
	add sp, #12
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_0200881c:
	.4byte 0x0005b333
.L_02008820:
	.4byte 0xfffa4ccd
	.section .text.x02008824,"ax",%progbits
	.global Func_02000824
	.thumb_func
Func_02000824:
	push {lr}
	ldr r3, [r1, #8]
	ldr r4, .L_0200885c
	adds r2, r3, r4
	movs r4, #192
	lsls r4, r4, #12
	adds r3, r3, r4
	ldr r4, [r0, #8]
	cmp r4, r2
	ble .L_02008856
	cmp r4, r3
	bge .L_02008856
	ldr r1, [r1, #16]
	ldr r2, .L_02008860
	ldr r0, [r0, #16]
	adds r3, r1, r2
	cmp r0, r3
	ble .L_02008856
	movs r4, #128
	lsls r4, r4, #12
	adds r3, r1, r4
	cmp r0, r3
	bge .L_02008856
	movs r0, #1
	b .L_02008858
.L_02008856:
	movs r0, #0
.L_02008858:
	pop {pc}
	.2byte 0x0000
.L_0200885c:
	.4byte 0xfff40000
.L_02008860:
	.4byte 0xfff80000
	.section .text.x02008864,"ax",%progbits
	.global Func_02000864
	.thumb_func
Func_02000864:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, .L_0200892c
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	adds r6, r0, #0
	ldr r0, [r3]
	bl Object_GetById
	mov r8, r0
	movs r0, #8
	bl Object_GetById
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
	mov r10, r0
	adds r3, r3, r7
	adds r0, r5, #0
	movs r1, #18
	str r3, [r6, #16]
	bl Engine_MathDivide
	subs r5, r5, r0
	str r5, [r6, #68]
	adds r3, r7, #0
	cmp r7, #0
	bge .L_020088b2
	adds r3, #15
.L_020088b2:
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
	mov r3, r8
	ldr r5, [r3, #12]
	cmp r5, #0
	bne .L_02008922
	adds r0, r6, #0
	mov r1, r8
	bl Func_02000824
	cmp r0, #0
	beq .L_020088fc
	ldr r2, [r6, #76]
	ldr r3, [r6, #16]
	str r5, [r6, #76]
	subs r3, r3, r2
	str r3, [r6, #16]
	ldr r3, [r6, #72]
	asrs r2, r2, #2
	adds r3, r3, r2
	str r3, [r6, #72]
.L_020088fc:
	mov r2, r8
	ldr r5, [r2, #12]
	cmp r5, #0
	bne .L_02008922
	adds r0, r6, #0
	mov r1, r10
	bl Func_02000824
	cmp r0, #0
	beq .L_02008922
	ldr r2, [r6, #76]
	ldr r3, [r6, #16]
	str r5, [r6, #76]
	subs r3, r3, r2
	str r3, [r6, #16]
	ldr r3, [r6, #72]
	asrs r2, r2, #2
	adds r3, r3, r2
	str r3, [r6, #72]
.L_02008922:
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200892c:
	.4byte gPartyState
	.section .text.x02008930,"ax",%progbits
	.global Func_02000930
	.thumb_func
Func_02000930:
	push {r5, r6, lr}
	adds r5, r0, #0
	adds r3, r5, #0
	movs r6, #0
	adds r3, #85
	strb r6, [r3]
	adds r2, r5, #0
	adds r3, #4
	strb r6, [r3]
	adds r2, #35
	movs r3, #2
	strb r3, [r2]
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
	bl ObjectDispatch_SetSingleChildField26
	adds r0, r5, #0
	movs r1, #2
	bl Func_02005130
	adds r0, r5, #0
	ldr r1, .L_02008988
	bl Func_02005138
	adds r0, r5, #0
	movs r1, #9
	bl Func_02005278
	adds r3, r5, #0
	adds r3, #100
	strh r6, [r3]
	str r6, [r5, #48]
	str r6, [r5, #52]
	pop {r5, r6, pc}
.L_02008988:
	.4byte Data_02005494
	.section .text.x0200898c,"ax",%progbits
	.global Func_0200098c
	.thumb_func
Func_0200098c:
	push {r5, r6, lr}
	ldr r3, [r0, #8]
	ldr r2, [r0, #4]
	adds r6, r1, #0
	ldr r1, [r0]
	ldr r0, .L_02008a00
	adds r3, r3, r0
	movs r0, #30
	adds r0, #255
	bl Func_02005140
	adds r5, r0, #0
	cmp r5, #0
	beq .L_020089fe
	bl Random16Far
	adds r3, r0, #0
	lsls r0, r3, #1
	adds r0, r0, r3
	lsls r0, r0, #12
	movs r3, #192
	lsls r3, r3, #6
	lsrs r0, r0, #16
	adds r0, r0, r3
	bl Math_Cosine
	str r0, [r5, #68]
	bl Random16Far
	lsls r0, r0, #16
	lsrs r0, r0, #16
	str r0, [r5, #72]
	bl Random16Far
	lsls r0, r0, #17
	lsrs r0, r0, #16
	adds r0, r0, r6
	str r0, [r5, #76]
	bl Random16Far
	ldr r3, .L_02008a04
	lsls r0, r0, #16
	lsrs r0, r0, #16
	adds r0, r0, r3
	adds r3, r5, #0
	adds r3, #100
	strh r0, [r3]
	movs r1, #3
	adds r0, r5, #0
	bl Func_02000930
	ldr r3, .L_02008a08
	adds r0, r5, #0
	str r3, [r5, #108]
	movs r1, #1
	bl Animation_SetStateFlags
.L_020089fe:
	pop {r5, r6, pc}
.L_02008a00:
	.4byte 0xfffe0000
.L_02008a04:
	.4byte 0xffff8000
.L_02008a08:
	.4byte Func_02000864
	.section .text.x02008a0c,"ax",%progbits
	.global Func_02000a0c
	.thumb_func
Func_02000a0c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, .L_02008ad8
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	adds r6, r0, #0
	ldr r0, [r3]
	bl Object_GetById
	mov r8, r0
	movs r0, #12
	bl Object_GetById
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
	mov r10, r0
	adds r3, r3, r7
	adds r0, r5, #0
	movs r1, #18
	str r3, [r6, #16]
	bl Engine_MathDivide
	subs r5, r5, r0
	str r5, [r6, #68]
	adds r3, r7, #0
	cmp r7, #0
	bge .L_02008a5a
	adds r3, #15
.L_02008a5a:
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
	adds r0, r6, #0
	adds r3, r3, r2
	str r3, [r6, #28]
	adds r2, r6, #0
	adds r2, #100
	ldrh r3, [r1, #18]
	ldrh r2, [r2]
	movs r5, #0
	adds r3, r3, r2
	strh r3, [r1, #18]
	mov r1, r8
	bl Func_02000824
	cmp r0, #0
	beq .L_02008aa8
	ldr r1, [r6, #80]
	movs r2, #12
	ldrb r3, [r1, #9]
	orrs r3, r2
	strb r3, [r1, #9]
	ldr r2, [r6, #76]
	ldr r3, [r6, #16]
	str r5, [r6, #76]
	subs r3, r3, r2
	str r3, [r6, #16]
	ldr r3, [r6, #72]
	asrs r2, r2, #2
	adds r3, r3, r2
	str r3, [r6, #72]
.L_02008aa8:
	adds r0, r6, #0
	mov r1, r10
	bl Func_02000824
	cmp r0, #0
	beq .L_02008ad0
	ldr r1, [r6, #80]
	movs r2, #12
	ldrb r3, [r1, #9]
	orrs r3, r2
	strb r3, [r1, #9]
	ldr r2, [r6, #76]
	ldr r3, [r6, #16]
	str r5, [r6, #76]
	subs r3, r3, r2
	str r3, [r6, #16]
	ldr r3, [r6, #72]
	asrs r2, r2, #3
	adds r3, r3, r2
	str r3, [r6, #72]
.L_02008ad0:
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_02008ad8:
	.4byte gPartyState
	.section .text.x02008adc,"ax",%progbits
	.global Func_02000adc
	.thumb_func
Func_02000adc:
	push {r5, lr}
	movs r0, #30
	movs r1, #174
	movs r2, #224
	movs r3, #156
	adds r0, #255
	lsls r1, r1, #18
	lsls r2, r2, #13
	lsls r3, r3, #16
	bl Func_02005140
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02008b52
	bl Random16Far
	adds r3, r0, #0
	lsls r0, r3, #1
	adds r0, r0, r3
	lsls r0, r0, #12
	movs r3, #192
	lsls r3, r3, #6
	lsrs r0, r0, #16
	adds r0, r0, r3
	bl Math_Cosine
	str r0, [r5, #68]
	bl Random16Far
	lsls r0, r0, #16
	lsrs r0, r0, #16
	str r0, [r5, #72]
	bl Random16Far
	movs r3, #176
	lsls r0, r0, #17
	lsls r3, r3, #11
	lsrs r0, r0, #16
	adds r0, r0, r3
	str r0, [r5, #76]
	bl Random16Far
	ldr r3, .L_02008b54
	lsls r0, r0, #16
	lsrs r0, r0, #16
	adds r0, r0, r3
	adds r3, r5, #0
	adds r3, #100
	strh r0, [r3]
	movs r1, #1
	adds r0, r5, #0
	bl Func_02000930
	ldr r3, .L_02008b58
	adds r0, r5, #0
	str r3, [r5, #108]
	movs r1, #1
	bl Animation_SetStateFlags
.L_02008b52:
	pop {r5, pc}
.L_02008b54:
	.4byte 0xffff8000
.L_02008b58:
	.4byte Func_02000a0c
	.section .text.x02008b5c,"ax",%progbits
	.global Func_02000b5c
	.thumb_func
Func_02000b5c:
	push {lr}
	ldr r3, .L_02008b74
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	movs r1, #1
	bl ObjectDispatch_SetSingleChildField26
	pop {pc}
.L_02008b74:
	.4byte gPartyState
	.section .text.x02008b78,"ax",%progbits
	.global Func_02000b78
	.thumb_func
Func_02000b78:
	push {r5, lr}
	adds r5, r0, #0
	bl Object_GetById
	adds r2, r0, #0
	adds r2, #98
	movs r3, #1
	strb r3, [r2]
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #2
	orrs r3, r2
	strb r3, [r0]
	movs r1, #1
	adds r0, r5, #0
	bl Object_SetModeById
	adds r0, r5, #0
	movs r1, #3
	bl Func_020052b8
	adds r0, r5, #0
	bl Func_020052c0
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x02008bac,"ax",%progbits
	.global Func_02000bac
	.thumb_func
Func_02000bac:
	push {r5, lr}
	adds r5, r0, #0
	bl Object_GetById
	movs r3, #1
	adds r0, #98
	strb r3, [r0]
	movs r1, #2
	adds r0, r5, #0
	bl Func_020052b8
	adds r0, r5, #0
	bl Func_020052c0
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x02008bcc,"ax",%progbits
	.global Func_02000bcc
	.thumb_func
Func_02000bcc:
	push {lr}
	movs r0, #133
	movs r1, #1
	bl Func_02005348
	movs r1, #10
	movs r0, #12
	bl Func_02005350
	bl Func_02005368
	movs r0, #1
	bl Func_02005340
	bl Func_02005358
	bl Func_02005360
	pop {pc}
	.2byte 0x0000
	.section .text.x02008bf4,"ax",%progbits
	.global Func_02000bf4
	.thumb_func
Func_02000bf4:
	push {lr}
	ldr r3, .L_02008c2c
	sub sp, #12
	ldr r3, [r3]
	cmp r3, #0
	beq .L_02008c28
	ldr r3, .L_02008c30
	movs r1, #9
	ldr r0, [r3]
	bl Engine_MathModulo
	adds r2, r0, #0
	cmp r2, #0
	bne .L_02008c28
	movs r3, #148
	mov r0, sp
	lsls r3, r3, #17
	str r3, [r0]
	movs r3, #176
	lsls r3, r3, #16
	movs r1, #128
	str r2, [r0, #4]
	str r3, [r0, #8]
	lsls r1, r1, #10
	bl Func_0200098c
.L_02008c28:
	add sp, #12
	pop {pc}
.L_02008c2c:
	.4byte gOverlayArea + 0x6298
.L_02008c30:
	.4byte Data_0300122c
	.section .text.x02008c34,"ax",%progbits
	.global Func_02000c34
	.thumb_func
Func_02000c34:
	push {lr}
	ldr r3, .L_02008c6c
	sub sp, #12
	ldr r3, [r3]
	cmp r3, #0
	beq .L_02008c68
	ldr r3, .L_02008c70
	movs r1, #5
	ldr r0, [r3]
	bl Engine_MathModulo
	adds r2, r0, #0
	cmp r2, #0
	bne .L_02008c68
	movs r3, #172
	mov r0, sp
	lsls r3, r3, #17
	str r3, [r0]
	movs r3, #160
	lsls r3, r3, #16
	movs r1, #128
	str r2, [r0, #4]
	str r3, [r0, #8]
	lsls r1, r1, #11
	bl Func_0200098c
.L_02008c68:
	add sp, #12
	pop {pc}
.L_02008c6c:
	.4byte gOverlayArea + 0x629c
.L_02008c70:
	.4byte Data_0300122c
	.section .text.x02008c74,"ax",%progbits
	.global Func_02000c74
	.thumb_func
Func_02000c74:
	push {lr}
	ldr r3, .L_02008c8c
	movs r1, #5
	ldr r0, [r3]
	bl Engine_MathModulo
	cmp r0, #0
	bne .L_02008c88
	bl Func_02000adc
.L_02008c88:
	pop {pc}
	.2byte 0x0000
.L_02008c8c:
	.4byte Data_0300122c
	.section .text.x02008c90,"ax",%progbits
	.global Func_02000c90
	.thumb_func
Func_02000c90:
	push {r5, lr}
	adds r5, r0, #0
	bl Object_GetById
	movs r3, #128
	lsls r3, r3, #4
	str r3, [r0, #28]
	adds r0, r5, #0
	bl Object_GetById
	movs r1, #216
	movs r3, #216
	lsls r1, r1, #16
	movs r2, #0
	lsls r3, r3, #16
	bl Object_SetPositionAndResetMotion
	pop {r5, pc}
	.section .text.x02008cb4,"ax",%progbits
	.global Func_02000cb4
	.thumb_func
Func_02000cb4:
	push {r5, lr}
	adds r5, r0, #0
	bl Object_GetById
	movs r3, #128
	lsls r3, r3, #4
	str r3, [r0, #28]
	adds r0, r5, #0
	bl Object_GetById
	movs r1, #188
	movs r3, #248
	lsls r1, r1, #17
	movs r2, #0
	lsls r3, r3, #16
	bl Object_SetPositionAndResetMotion
	pop {r5, pc}
	.section .text.x02008cd8,"ax",%progbits
	.global Func_02000cd8
	.thumb_func
Func_02000cd8:
	push {r5, r6, lr}
	adds r6, r1, #0
	adds r5, r2, #0
	bl Object_GetById
	ldr r3, [r0, #28]
	adds r3, r3, r5
	str r3, [r0, #28]
	ldr r3, [r0, #12]
	adds r3, r3, r6
	str r3, [r0, #12]
	ldr r3, [r0, #16]
	subs r3, r3, r6
	str r3, [r0, #16]
	pop {r5, r6, pc}
	.2byte 0x0000
	.section .text.x02008cf8,"ax",%progbits
	.global Func_02000cf8
	.thumb_func
Func_02000cf8:
	push {r5, r6, lr}
	adds r6, r1, #0
	adds r5, r2, #0
	bl Object_GetById
	ldr r3, [r0, #28]
	subs r3, r3, r5
	str r3, [r0, #28]
	ldr r3, [r0, #12]
	subs r3, r3, r6
	str r3, [r0, #12]
	ldr r3, [r0, #16]
	adds r3, r3, r6
	str r3, [r0, #16]
	pop {r5, r6, pc}
	.2byte 0x0000
	.section .text.x02008d18,"ax",%progbits
	.global Func_02000d18
	.thumb_func
Func_02000d18:
	push {lr}
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #10
	bl Func_02005110
	cmp r0, #0
	bne .L_02008d34
	movs r0, #192
	lsls r0, r0, #2
	bl Func_02005110
	cmp r0, #0
	beq .L_02008d38
.L_02008d34:
	movs r0, #1
	b .L_02008d3a
.L_02008d38:
	movs r0, #0
.L_02008d3a:
	pop {pc}
	.section .text.x02008d3c,"ax",%progbits
	.global Func_02000d3c
	.thumb_func
Func_02000d3c:
	push {lr}
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #10
	bl Func_02005110
	cmp r0, #0
	bne .L_02008d5a
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #1
	bl Func_02005110
	cmp r0, #0
	beq .L_02008d5e
.L_02008d5a:
	movs r0, #1
	b .L_02008d60
.L_02008d5e:
	movs r0, #0
.L_02008d60:
	pop {pc}
	.2byte 0x0000
	.section .text.x02008d64,"ax",%progbits
	.global Func_02000d64
	.thumb_func
Func_02000d64:
	push {r5, lr}
	movs r0, #11
	movs r1, #2
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
	movs r0, #9
	movs r1, #0
	bl Object_SetModeById
	movs r1, #2
	movs r0, #9
	bl Func_020052b8
	movs r0, #9
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #253
	ands r3, r2
	strb r3, [r0]
	movs r0, #9
	bl Object_GetById
	movs r1, #216
	movs r2, #128
	movs r3, #216
	lsls r1, r1, #16
	lsls r2, r2, #12
	lsls r3, r3, #16
	bl Object_SetPositionAndResetMotion
	movs r0, #130
	lsls r0, r0, #2
	bl Func_02005110
	cmp r0, #0
	beq .L_02008e02
	movs r1, #4
	movs r0, #9
	bl Object_SetModeById
	ldr r5, .L_02008e58
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	ldr r0, [r5]
	bl Object_GetById
	movs r1, #216
	movs r2, #192
	movs r3, #216
	lsls r1, r1, #16
	lsls r2, r2, #12
	lsls r3, r3, #16
	bl Object_SetPositionAndResetMotion
	ldr r0, [r5]
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
.L_02008e02:
	movs r0, #11
	bl Object_GetById
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r0, #28]
	movs r0, #11
	bl Object_GetById
	movs r1, #216
	movs r3, #216
	lsls r1, r1, #16
	movs r2, #0
	lsls r3, r3, #16
	bl Object_SetPositionAndResetMotion
	movs r0, #13
	bl Func_02000c90
	movs r0, #14
	bl Func_02000c90
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #1
	bl Func_02005110
	cmp r0, #0
	beq .L_02008e48
	movs r0, #15
	bl Func_02000c90
	movs r0, #16
	bl Func_02000c90
.L_02008e48:
	ldr r2, .L_02008e5c
	movs r3, #1
	movs r0, #131
	str r3, [r2]
	lsls r0, r0, #2
	bl Func_02005118
	pop {r5, pc}
.L_02008e58:
	.4byte gPartyState
.L_02008e5c:
	.4byte gOverlayArea + 0x6298
	.section .text.x02008e60,"ax",%progbits
	.global Func_02000e60
	.thumb_func
Func_02000e60:
	push {r5, lr}
	movs r0, #12
	movs r1, #2
	bl Object_SetModeById
	movs r0, #17
	movs r1, #3
	bl Object_SetModeById
	movs r0, #18
	movs r1, #3
	bl Object_SetModeById
	movs r0, #19
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	movs r1, #3
	bl Object_SetModeById
	movs r0, #10
	movs r1, #0
	bl Object_SetModeById
	movs r1, #2
	movs r0, #10
	bl Func_020052b8
	movs r0, #10
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #253
	ands r3, r2
	strb r3, [r0]
	movs r0, #10
	bl Object_GetById
	movs r1, #188
	movs r2, #128
	movs r3, #248
	lsls r1, r1, #17
	lsls r2, r2, #12
	lsls r3, r3, #16
	bl Object_SetPositionAndResetMotion
	movs r0, #133
	lsls r0, r0, #1
	adds r0, #255
	bl Func_02005110
	cmp r0, #0
	beq .L_02008f00
	movs r1, #4
	movs r0, #10
	bl Object_SetModeById
	ldr r5, .L_02008f58
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	ldr r0, [r5]
	bl Object_GetById
	movs r1, #188
	movs r2, #192
	movs r3, #248
	lsls r1, r1, #17
	lsls r2, r2, #12
	lsls r3, r3, #16
	bl Object_SetPositionAndResetMotion
	ldr r0, [r5]
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
.L_02008f00:
	movs r0, #12
	bl Object_GetById
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r0, #28]
	movs r0, #12
	bl Object_GetById
	movs r1, #188
	movs r3, #248
	lsls r1, r1, #17
	movs r2, #0
	lsls r3, r3, #16
	bl Object_SetPositionAndResetMotion
	movs r0, #17
	bl Func_02000cb4
	movs r0, #18
	bl Func_02000cb4
	movs r0, #192
	lsls r0, r0, #2
	bl Func_02005110
	cmp r0, #0
	beq .L_02008f44
	movs r0, #19
	bl Func_02000cb4
	movs r0, #20
	bl Func_02000cb4
.L_02008f44:
	ldr r2, .L_02008f5c
	movs r0, #135
	movs r3, #1
	lsls r0, r0, #1
	str r3, [r2]
	adds r0, #255
	bl Func_02005118
	pop {r5, pc}
	.2byte 0x0000
.L_02008f58:
	.4byte gPartyState
.L_02008f5c:
	.4byte gOverlayArea + 0x629c
	.section .text.x02008f60,"ax",%progbits
	.global Func_02000f60
	.thumb_func
Func_02000f60:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r7, .L_020092a4
	movs r2, #150
	ldr r3, [r7]
	lsls r2, r2, #1
	sub sp, #8
	cmp r3, r2
	bge .L_02008f80
	bl Func_02000d18
	cmp r0, #0
	bne .L_02008f7e
	b .L_02009294
.L_02008f7e:
	b .L_0200929c
.L_02008f80:
	movs r2, #150
	lsls r2, r2, #1
	cmp r3, r2
	bne .L_02008fb2
	movs r5, #13
	movs r0, #15
	movs r1, #15
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl Func_02005178
	movs r3, #63
	str r3, [sp, #0]
	movs r0, #65
	movs r1, #15
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl Func_02005178
	bl Func_02000d64
	b .L_02009294
.L_02008fb2:
	movs r2, #176
	lsls r2, r2, #4
	adds r2, #183
	mov r8, r2
	cmp r3, r8
	ble .L_02008fc0
	b .L_0200919c
.L_02008fc0:
	movs r0, #9
	bl Object_GetById
	adds r6, r0, #0
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #1
	bl Func_02005110
	ldr r3, [r7]
	cmp r0, #0
	beq .L_020090ae
	movs r2, #158
	lsls r2, r2, #1
	cmp r3, r2
	bgt .L_02009022
	ldr r3, [r6, #12]
	movs r2, #144
	lsls r2, r2, #11
	movs r5, #128
	adds r3, r3, r2
	lsls r5, r5, #5
	movs r1, #128
	str r3, [r6, #12]
	lsls r1, r1, #10
	movs r0, #11
	adds r2, r5, #0
	bl Func_02000cd8
	movs r1, #192
	lsls r1, r1, #9
	movs r0, #13
	adds r2, r5, #0
	bl Func_02000cd8
	movs r1, #128
	lsls r1, r1, #9
	movs r0, #14
	adds r2, r5, #0
	bl Func_02000cd8
	movs r1, #128
	lsls r1, r1, #8
	movs r0, #15
	adds r2, r5, #0
	bl Func_02000cd8
	movs r0, #16
	b .L_020090dc
.L_02009022:
	movs r2, #150
	lsls r2, r2, #2
	cmp r3, r2
	bge .L_0200905a
	movs r0, #130
	lsls r0, r0, #2
	bl Func_02005110
	cmp r0, #0
	beq .L_020090ee
	ldr r3, [r7]
	movs r2, #200
	lsls r2, r2, #1
	cmp r3, r2
	bne .L_020090ee
	movs r3, #13
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #22
	movs r1, #0
	movs r2, #1
	movs r3, #1
	bl Func_02005178
	ldr r0, .L_020092a8
	bl Scheduler_RemoveCallbackFar
	b .L_020090ee
.L_0200905a:
	movs r1, #186
	lsls r1, r1, #2
	adds r1, #255
	cmp r3, r1
	ble .L_020090fc
	ldr r3, [r6, #12]
	ldr r2, .L_020092ac
	movs r5, #192
	adds r3, r3, r2
	lsls r5, r5, #5
	movs r1, #192
	str r3, [r6, #12]
	lsls r1, r1, #10
	movs r0, #11
	adds r2, r5, #0
	bl Func_02000cf8
	movs r1, #144
	lsls r1, r1, #10
	movs r0, #13
	adds r2, r5, #0
	bl Func_02000cf8
	movs r1, #192
	lsls r1, r1, #9
	movs r0, #14
	adds r2, r5, #0
	bl Func_02000cf8
	movs r1, #192
	lsls r1, r1, #8
	movs r0, #15
	adds r2, r5, #0
	bl Func_02000cf8
	movs r0, #16
	movs r1, #0
	adds r2, r5, #0
	bl Func_02000cf8
	movs r0, #16
	b .L_0200915c
.L_020090ae:
	movs r2, #158
	lsls r2, r2, #1
	cmp r3, r2
	bgt .L_020090e6
	ldr r3, [r6, #12]
	ldr r2, .L_020092b0
	movs r5, #128
	adds r3, r3, r2
	lsls r5, r5, #5
	movs r1, #128
	str r3, [r6, #12]
	lsls r1, r1, #9
	movs r0, #11
	adds r2, r5, #0
	bl Func_02000cd8
	movs r1, #128
	lsls r1, r1, #8
	movs r0, #13
	adds r2, r5, #0
	bl Func_02000cd8
	movs r0, #14
.L_020090dc:
	movs r1, #0
	adds r2, r5, #0
	bl Func_02000cd8
	b .L_0200916a
.L_020090e6:
	movs r2, #150
	lsls r2, r2, #2
	cmp r3, r2
	bge .L_020090f2
.L_020090ee:
	ldr r5, .L_020092b4
	b .L_02009112
.L_020090f2:
	movs r1, #186
	lsls r1, r1, #2
	adds r1, #255
	cmp r3, r1
	bgt .L_0200912c
.L_020090fc:
	ldr r5, .L_020092b4
	ldrh r2, [r5]
	cmp r2, #0
	bne .L_02009112
	ldr r3, .L_020092b8
	movs r0, #131
	str r2, [r3]
	lsls r0, r0, #2
	str r1, [r7]
	bl Func_02005120
.L_02009112:
	ldrh r0, [r5]
	bl Math_Sine
	ldr r3, [r6, #12]
	asrs r0, r0, #1
	adds r3, r3, r0
	str r3, [r6, #12]
	ldrh r3, [r5]
	movs r2, #128
	lsls r2, r2, #5
	adds r3, r3, r2
	strh r3, [r5]
	b .L_0200916a
.L_0200912c:
	ldr r3, [r6, #12]
	ldr r2, .L_020092bc
	movs r5, #192
	adds r3, r3, r2
	lsls r5, r5, #5
	movs r1, #192
	str r3, [r6, #12]
	lsls r1, r1, #9
	movs r0, #11
	adds r2, r5, #0
	bl Func_02000cf8
	movs r1, #192
	lsls r1, r1, #8
	movs r0, #13
	adds r2, r5, #0
	bl Func_02000cf8
	movs r0, #14
	movs r1, #0
	adds r2, r5, #0
	bl Func_02000cf8
	movs r0, #14
.L_0200915c:
	bl Object_GetById
	ldr r3, [r0, #28]
	cmp r3, r5
	bgt .L_0200916a
	mov r3, r8
	str r3, [r7]
.L_0200916a:
	movs r0, #130
	lsls r0, r0, #2
	bl Func_02005110
	cmp r0, #0
	bne .L_02009178
	b .L_02009294
.L_02009178:
	ldr r3, .L_020092c0
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	ldr r3, [r6, #12]
	movs r2, #128
	lsls r2, r2, #11
	adds r3, r3, r2
	str r3, [r0, #12]
	movs r3, #128
	lsls r3, r3, #24
	str r3, [r0, #60]
	movs r3, #0
	str r3, [r0, #40]
	b .L_02009294
.L_0200919c:
	movs r2, #176
	lsls r2, r2, #4
	adds r2, #184
	cmp r3, r2
	bne .L_02009262
	movs r5, #13
	movs r0, #8
	movs r1, #0
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl Func_02005178
	movs r3, #63
	str r3, [sp, #0]
	movs r0, #58
	movs r3, #1
	movs r1, #0
	movs r2, #1
	str r5, [sp, #4]
	bl Func_02005178
	movs r0, #11
	movs r1, #0
	movs r2, #0
	bl Func_02005230
	movs r0, #13
	movs r1, #0
	movs r2, #0
	bl Func_02005230
	movs r0, #14
	movs r1, #0
	movs r2, #0
	bl Func_02005230
	movs r0, #15
	movs r1, #0
	movs r2, #0
	bl Func_02005230
	movs r2, #0
	movs r0, #16
	movs r1, #0
	bl Func_02005230
	movs r1, #1
	movs r0, #9
	bl Object_SetModeById
	movs r0, #9
	bl Object_GetById
	movs r1, #216
	movs r3, #216
	lsls r1, r1, #16
	movs r2, #0
	lsls r3, r3, #16
	bl Object_SetPositionAndResetMotion
	movs r0, #9
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #2
	orrs r3, r2
	strb r3, [r0]
	movs r1, #3
	movs r0, #9
	bl Func_020052b8
	movs r0, #130
	lsls r0, r0, #2
	bl Func_02005110
	cmp r0, #0
	beq .L_02009294
	ldr r5, .L_020092c0
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	ldr r0, [r5]
	bl Object_GetById
	movs r2, #0
	ldr r1, [r0, #8]
	ldr r3, [r0, #16]
	bl Object_SetPositionAndResetMotion
	ldr r0, [r5]
	bl Object_GetById
	movs r1, #1
	bl ObjectDispatch_SetSingleChildField26
	b .L_02009294
.L_02009262:
	bl Func_02000d18
	adds r5, r0, #0
	cmp r5, #0
	bne .L_0200929c
	bl Random16Far
	ldr r3, [r7]
	lsls r0, r0, #7
	lsrs r0, r0, #16
	adds r3, r3, r0
	str r3, [r7]
	bl Random16Far
	movs r2, #156
	lsls r2, r2, #6
	adds r2, #16
	adds r3, r0, #0
	muls r3, r2
	lsrs r3, r3, #16
	adds r3, r3, r2
	ldr r2, [r7]
	cmp r2, r3
	bls .L_02009294
	str r5, [r7]
.L_02009294:
	ldr r2, .L_020092a4
	ldr r3, [r2]
	adds r3, #1
	str r3, [r2]
.L_0200929c:
	add sp, #8
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_020092a4:
	.4byte gOverlayArea + 0x62a0
.L_020092a8:
	.4byte Func_02000f60
.L_020092ac:
	.4byte 0xfff94000
.L_020092b0:
	.4byte 0x00027999
.L_020092b4:
	.4byte gOverlayArea + 0x62a8
.L_020092b8:
	.4byte gOverlayArea + 0x6298
.L_020092bc:
	.4byte 0xfffc499a
.L_020092c0:
	.4byte gPartyState
	.section .text.x020092c4,"ax",%progbits
	.global Func_020012c4
	.thumb_func
Func_020012c4:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r7, .L_02009614
	movs r2, #150
	ldr r3, [r7]
	lsls r2, r2, #1
	sub sp, #8
	cmp r3, r2
	bge .L_020092e4
	bl Func_02000d3c
	cmp r0, #0
	bne .L_020092e2
	b .L_02009604
.L_020092e2:
	b .L_0200960c
.L_020092e4:
	movs r2, #150
	lsls r2, r2, #1
	cmp r3, r2
	bne .L_02009318
	movs r3, #23
	str r3, [sp, #0]
	movs r5, #15
	movs r0, #15
	movs r1, #15
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl Func_02005178
	movs r3, #73
	str r3, [sp, #0]
	movs r0, #65
	movs r1, #15
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl Func_02005178
	bl Func_02000e60
	b .L_02009604
.L_02009318:
	movs r2, #176
	lsls r2, r2, #4
	adds r2, #183
	mov r8, r2
	cmp r3, r8
	ble .L_02009326
	b .L_02009508
.L_02009326:
	movs r0, #10
	bl Object_GetById
	adds r6, r0, #0
	movs r0, #192
	lsls r0, r0, #2
	bl Func_02005110
	ldr r3, [r7]
	cmp r0, #0
	beq .L_02009416
	movs r2, #158
	lsls r2, r2, #1
	cmp r3, r2
	bgt .L_02009386
	ldr r3, [r6, #12]
	movs r2, #144
	lsls r2, r2, #11
	movs r5, #128
	adds r3, r3, r2
	lsls r5, r5, #5
	movs r1, #128
	str r3, [r6, #12]
	lsls r1, r1, #10
	movs r0, #12
	adds r2, r5, #0
	bl Func_02000cd8
	movs r1, #192
	lsls r1, r1, #9
	movs r0, #17
	adds r2, r5, #0
	bl Func_02000cd8
	movs r1, #128
	lsls r1, r1, #9
	movs r0, #18
	adds r2, r5, #0
	bl Func_02000cd8
	movs r1, #128
	lsls r1, r1, #8
	movs r0, #19
	adds r2, r5, #0
	bl Func_02000cd8
	movs r0, #20
	b .L_02009444
.L_02009386:
	movs r2, #150
	lsls r2, r2, #2
	cmp r3, r2
	bge .L_020093c2
	movs r0, #133
	lsls r0, r0, #1
	adds r0, #255
	bl Func_02005110
	cmp r0, #0
	beq .L_02009456
	ldr r3, [r7]
	movs r2, #200
	lsls r2, r2, #1
	cmp r3, r2
	bne .L_02009456
	movs r3, #23
	movs r2, #15
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #23
	movs r1, #0
	movs r2, #1
	movs r3, #1
	bl Func_02005178
	ldr r0, .L_02009618
	bl Scheduler_RemoveCallbackFar
	b .L_02009456
.L_020093c2:
	movs r1, #186
	lsls r1, r1, #2
	adds r1, #255
	cmp r3, r1
	ble .L_02009464
	ldr r3, [r6, #12]
	ldr r2, .L_0200961c
	movs r5, #192
	adds r3, r3, r2
	lsls r5, r5, #5
	movs r1, #192
	str r3, [r6, #12]
	lsls r1, r1, #10
	movs r0, #12
	adds r2, r5, #0
	bl Func_02000cf8
	movs r1, #144
	lsls r1, r1, #10
	movs r0, #17
	adds r2, r5, #0
	bl Func_02000cf8
	movs r1, #192
	lsls r1, r1, #9
	movs r0, #18
	adds r2, r5, #0
	bl Func_02000cf8
	movs r1, #192
	lsls r1, r1, #8
	movs r0, #19
	adds r2, r5, #0
	bl Func_02000cf8
	movs r0, #20
	movs r1, #0
	adds r2, r5, #0
	bl Func_02000cf8
	movs r0, #20
	b .L_020094c6
.L_02009416:
	movs r2, #158
	lsls r2, r2, #1
	cmp r3, r2
	bgt .L_0200944e
	ldr r3, [r6, #12]
	ldr r2, .L_02009620
	movs r5, #128
	adds r3, r3, r2
	lsls r5, r5, #5
	movs r1, #128
	str r3, [r6, #12]
	lsls r1, r1, #9
	movs r0, #12
	adds r2, r5, #0
	bl Func_02000cd8
	movs r1, #128
	lsls r1, r1, #8
	movs r0, #17
	adds r2, r5, #0
	bl Func_02000cd8
	movs r0, #18
.L_02009444:
	movs r1, #0
	adds r2, r5, #0
	bl Func_02000cd8
	b .L_020094d4
.L_0200944e:
	movs r2, #150
	lsls r2, r2, #2
	cmp r3, r2
	bge .L_0200945a
.L_02009456:
	ldr r5, .L_02009624
	b .L_0200947c
.L_0200945a:
	movs r1, #186
	lsls r1, r1, #2
	adds r1, #255
	cmp r3, r1
	bgt .L_02009496
.L_02009464:
	ldr r5, .L_02009624
	ldrh r2, [r5]
	cmp r2, #0
	bne .L_0200947c
	ldr r3, .L_02009628
	movs r0, #135
	lsls r0, r0, #1
	str r2, [r3]
	adds r0, #255
	str r1, [r7]
	bl Func_02005120
.L_0200947c:
	ldrh r0, [r5]
	bl Math_Sine
	ldr r3, [r6, #12]
	asrs r0, r0, #1
	adds r3, r3, r0
	str r3, [r6, #12]
	ldrh r3, [r5]
	movs r2, #128
	lsls r2, r2, #5
	adds r3, r3, r2
	strh r3, [r5]
	b .L_020094d4
.L_02009496:
	ldr r3, [r6, #12]
	ldr r2, .L_0200962c
	movs r5, #192
	adds r3, r3, r2
	lsls r5, r5, #5
	movs r1, #192
	str r3, [r6, #12]
	lsls r1, r1, #9
	movs r0, #12
	adds r2, r5, #0
	bl Func_02000cf8
	movs r1, #192
	lsls r1, r1, #8
	movs r0, #17
	adds r2, r5, #0
	bl Func_02000cf8
	movs r0, #18
	movs r1, #0
	adds r2, r5, #0
	bl Func_02000cf8
	movs r0, #18
.L_020094c6:
	bl Object_GetById
	ldr r3, [r0, #28]
	cmp r3, r5
	bgt .L_020094d4
	mov r3, r8
	str r3, [r7]
.L_020094d4:
	movs r0, #133
	lsls r0, r0, #1
	adds r0, #255
	bl Func_02005110
	cmp r0, #0
	bne .L_020094e4
	b .L_02009604
.L_020094e4:
	ldr r3, .L_02009630
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	ldr r3, [r6, #12]
	movs r2, #128
	lsls r2, r2, #11
	adds r3, r3, r2
	str r3, [r0, #12]
	movs r3, #128
	lsls r3, r3, #24
	str r3, [r0, #60]
	movs r3, #0
	str r3, [r0, #40]
	b .L_02009604
.L_02009508:
	movs r2, #176
	lsls r2, r2, #4
	adds r2, #184
	cmp r3, r2
	bne .L_020095d2
	movs r3, #23
	str r3, [sp, #0]
	movs r5, #15
	movs r0, #8
	movs r1, #0
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl Func_02005178
	movs r3, #73
	str r3, [sp, #0]
	movs r0, #58
	movs r3, #1
	movs r1, #0
	movs r2, #1
	str r5, [sp, #4]
	bl Func_02005178
	movs r0, #12
	movs r1, #0
	movs r2, #0
	bl Func_02005230
	movs r0, #17
	movs r1, #0
	movs r2, #0
	bl Func_02005230
	movs r0, #18
	movs r1, #0
	movs r2, #0
	bl Func_02005230
	movs r0, #19
	movs r1, #0
	movs r2, #0
	bl Func_02005230
	movs r2, #0
	movs r0, #20
	movs r1, #0
	bl Func_02005230
	movs r1, #1
	movs r0, #10
	bl Object_SetModeById
	movs r0, #10
	bl Object_GetById
	movs r1, #188
	movs r3, #248
	lsls r1, r1, #17
	movs r2, #0
	lsls r3, r3, #16
	bl Object_SetPositionAndResetMotion
	movs r0, #10
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #2
	orrs r3, r2
	strb r3, [r0]
	movs r1, #3
	movs r0, #10
	bl Func_020052b8
	movs r0, #133
	lsls r0, r0, #1
	adds r0, #255
	bl Func_02005110
	cmp r0, #0
	beq .L_02009604
	ldr r5, .L_02009630
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	ldr r0, [r5]
	bl Object_GetById
	movs r2, #0
	ldr r1, [r0, #8]
	ldr r3, [r0, #16]
	bl Object_SetPositionAndResetMotion
	ldr r0, [r5]
	bl Object_GetById
	movs r1, #1
	bl ObjectDispatch_SetSingleChildField26
	b .L_02009604
.L_020095d2:
	bl Func_02000d3c
	adds r5, r0, #0
	cmp r5, #0
	bne .L_0200960c
	bl Random16Far
	ldr r3, [r7]
	lsls r0, r0, #7
	lsrs r0, r0, #16
	adds r3, r3, r0
	str r3, [r7]
	bl Random16Far
	movs r2, #156
	lsls r2, r2, #6
	adds r2, #16
	adds r3, r0, #0
	muls r3, r2
	lsrs r3, r3, #16
	adds r3, r3, r2
	ldr r2, [r7]
	cmp r2, r3
	bls .L_02009604
	str r5, [r7]
.L_02009604:
	ldr r2, .L_02009614
	ldr r3, [r2]
	adds r3, #1
	str r3, [r2]
.L_0200960c:
	add sp, #8
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_02009614:
	.4byte gOverlayArea + 0x62a4
.L_02009618:
	.4byte Func_020012c4
.L_0200961c:
	.4byte 0xfff94000
.L_02009620:
	.4byte 0x00027999
.L_02009624:
	.4byte gOverlayArea + 0x62aa
.L_02009628:
	.4byte gOverlayArea + 0x629c
.L_0200962c:
	.4byte 0xfffc499a
.L_02009630:
	.4byte gPartyState
	.section .text.x02009634,"ax",%progbits
	.global Func_02001634
	.thumb_func
Func_02001634:
	push {r5, r6, r7, lr}
	movs r0, #13
	sub sp, #8
	bl Object_GetById
	adds r5, r0, #0
	movs r0, #15
	bl Object_GetById
	ldr r7, .L_020097e8
	movs r1, #150
	ldr r3, [r7]
	lsls r1, r1, #1
	adds r6, r0, #0
	cmp r3, r1
	bne .L_020096b6
	movs r0, #162
	bl Func_02005388
	movs r0, #13
	bl Object_GetById
	movs r1, #220
	movs r3, #204
	lsls r1, r1, #17
	ldr r2, .L_020097ec
	lsls r3, r3, #16
	bl Object_SetPositionAndResetMotion
	movs r0, #15
	bl Object_GetById
	movs r1, #220
	movs r3, #200
	ldr r2, .L_020097ec
	lsls r3, r3, #16
	lsls r1, r1, #17
	bl Object_SetPositionAndResetMotion
	movs r0, #13
	movs r1, #3
	bl Func_020052b8
	movs r0, #15
	movs r1, #3
	bl Func_020052b8
	adds r1, r6, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #2
	orrs r3, r2
	strb r3, [r1]
	movs r3, #128
	lsls r3, r3, #3
	movs r0, #13
	movs r1, #0
	str r3, [r6, #28]
	bl Object_SetModeById
	movs r0, #15
	movs r1, #2
	bl Object_SetModeById
	b .L_020097da
.L_020096b6:
	ldr r1, .L_020097f0
	adds r2, r3, r1
	movs r1, #192
	lsls r1, r1, #3
	adds r1, #162
	cmp r2, r1
	bhi .L_0200970c
	ldr r3, [r5, #12]
	movs r2, #128
	lsls r2, r2, #11
	adds r3, r3, r2
	movs r2, #152
	lsls r2, r2, #7
	adds r2, #204
	str r3, [r5, #12]
	adds r1, r2, #0
	movs r0, #15
	bl Func_02000cd8
	ldr r3, [r5, #12]
	ldr r1, .L_020097f4
	cmp r3, r1
	ble .L_020096ec
	movs r0, #13
	movs r1, #2
	bl Func_020052b8
.L_020096ec:
	ldr r3, [r5, #12]
	cmp r3, #0
	blt .L_020097da
	movs r3, #27
	movs r2, #12
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #1
	movs r0, #0
	movs r1, #0
	movs r2, #1
	bl Func_02005178
	movs r3, #250
	lsls r3, r3, #3
	b .L_020097d8
.L_0200970c:
	movs r2, #175
	lsls r2, r2, #4
	cmp r3, r2
	bge .L_0200978e
	ldr r6, .L_020097f8
	ldrh r0, [r6]
	bl Math_Sine
	ldr r3, [r5, #12]
	asrs r0, r0, #1
	adds r3, r3, r0
	str r3, [r5, #12]
	ldrh r3, [r6]
	movs r1, #128
	lsls r1, r1, #5
	adds r3, r3, r1
	strh r3, [r6]
	movs r2, #128
	ldr r3, [r7]
	lsls r2, r2, #4
	adds r2, #232
	cmp r3, r2
	bne .L_0200974e
	movs r3, #27
	movs r2, #12
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #27
	movs r1, #10
	movs r2, #1
	movs r3, #1
	bl Func_02005178
.L_0200974e:
	movs r1, #128
	ldr r3, [r7]
	lsls r1, r1, #4
	adds r1, #252
	cmp r3, r1
	ble .L_020097da
	ldrh r3, [r6]
	cmp r3, #0
	bne .L_020097da
	movs r0, #130
	lsls r0, r0, #2
	bl Func_02005110
	cmp r0, #0
	beq .L_02009788
	movs r3, #27
	movs r2, #12
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #21
	movs r1, #0
	movs r2, #1
	movs r3, #1
	bl Func_02005178
	ldr r0, .L_020097fc
	bl Scheduler_RemoveCallbackFar
	b .L_020097da
.L_02009788:
	movs r3, #175
	lsls r3, r3, #4
	b .L_020097d8
.L_0200978e:
	ldr r3, [r5, #12]
	ldr r2, .L_02009800
	movs r0, #15
	adds r3, r3, r2
	movs r2, #152
	lsls r2, r2, #7
	adds r2, #204
	str r3, [r5, #12]
	adds r1, r2, #0
	bl Func_02000cf8
	ldr r3, [r5, #12]
	ldr r1, .L_020097f4
	cmp r3, r1
	bge .L_020097b4
	movs r0, #13
	movs r1, #3
	bl Func_020052b8
.L_020097b4:
	ldr r3, [r5, #12]
	ldr r2, .L_020097ec
	cmp r3, r2
	bgt .L_020097da
	movs r0, #245
	bl Func_02005388
	movs r0, #13
	movs r1, #0
	movs r2, #0
	bl Func_02005230
	movs r0, #15
	movs r1, #0
	movs r2, #0
	bl Func_02005230
	movs r3, #0
.L_020097d8:
	str r3, [r7]
.L_020097da:
	ldr r2, .L_020097e8
	add sp, #8
	ldr r3, [r2]
	adds r3, #1
	str r3, [r2]
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_020097e8:
	.4byte gOverlayArea + 0x62a0
.L_020097ec:
	.4byte 0xffec0000
.L_020097f0:
	.4byte 0xfffffed3
.L_020097f4:
	.4byte 0xfffb0000
.L_020097f8:
	.4byte gOverlayArea + 0x62a8
.L_020097fc:
	.4byte Func_02001634
.L_02009800:
	.4byte 0xfffc0000
	.section .text.x02009804,"ax",%progbits
	.global Func_02001804
	.thumb_func
Func_02001804:
	push {r5, r6, r7, lr}
	movs r0, #14
	sub sp, #8
	bl Object_GetById
	adds r5, r0, #0
	movs r0, #16
	bl Object_GetById
	ldr r7, .L_020099b8
	movs r1, #150
	ldr r3, [r7]
	lsls r1, r1, #1
	adds r6, r0, #0
	cmp r3, r1
	bne .L_02009886
	movs r0, #162
	bl Func_02005388
	movs r0, #14
	bl Object_GetById
	movs r1, #150
	movs r3, #236
	lsls r1, r1, #18
	ldr r2, .L_020099bc
	lsls r3, r3, #16
	bl Object_SetPositionAndResetMotion
	movs r0, #16
	bl Object_GetById
	movs r1, #150
	movs r3, #232
	ldr r2, .L_020099bc
	lsls r3, r3, #16
	lsls r1, r1, #18
	bl Object_SetPositionAndResetMotion
	movs r0, #14
	movs r1, #3
	bl Func_020052b8
	movs r0, #16
	movs r1, #3
	bl Func_020052b8
	adds r1, r6, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #2
	orrs r3, r2
	strb r3, [r1]
	movs r3, #128
	lsls r3, r3, #3
	movs r0, #14
	movs r1, #0
	str r3, [r6, #28]
	bl Object_SetModeById
	movs r0, #16
	movs r1, #2
	bl Object_SetModeById
	b .L_020099ac
.L_02009886:
	ldr r1, .L_020099c0
	adds r2, r3, r1
	movs r1, #192
	lsls r1, r1, #3
	adds r1, #162
	cmp r2, r1
	bhi .L_020098dc
	ldr r3, [r5, #12]
	movs r2, #128
	lsls r2, r2, #11
	adds r3, r3, r2
	movs r2, #152
	lsls r2, r2, #7
	adds r2, #204
	str r3, [r5, #12]
	adds r1, r2, #0
	movs r0, #16
	bl Func_02000cd8
	ldr r3, [r5, #12]
	ldr r1, .L_020099c4
	cmp r3, r1
	ble .L_020098bc
	movs r0, #14
	movs r1, #2
	bl Func_020052b8
.L_020098bc:
	ldr r3, [r5, #12]
	cmp r3, #0
	blt .L_020099ac
	movs r3, #37
	movs r2, #14
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #1
	movs r0, #0
	movs r1, #0
	movs r2, #1
	bl Func_02005178
	movs r3, #250
	lsls r3, r3, #3
	b .L_020099aa
.L_020098dc:
	movs r2, #175
	lsls r2, r2, #4
	cmp r3, r2
	bge .L_02009960
	ldr r6, .L_020099c8
	ldrh r0, [r6]
	bl Math_Sine
	ldr r3, [r5, #12]
	asrs r0, r0, #1
	adds r3, r3, r0
	str r3, [r5, #12]
	ldrh r3, [r6]
	movs r1, #128
	lsls r1, r1, #5
	adds r3, r3, r1
	strh r3, [r6]
	movs r2, #128
	ldr r3, [r7]
	lsls r2, r2, #4
	adds r2, #232
	cmp r3, r2
	bne .L_0200991e
	movs r3, #37
	movs r2, #14
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #37
	movs r1, #12
	movs r2, #1
	movs r3, #1
	bl Func_02005178
.L_0200991e:
	movs r1, #128
	ldr r3, [r7]
	lsls r1, r1, #4
	adds r1, #252
	cmp r3, r1
	ble .L_020099ac
	ldrh r3, [r6]
	cmp r3, #0
	bne .L_020099ac
	movs r0, #133
	lsls r0, r0, #1
	adds r0, #255
	bl Func_02005110
	cmp r0, #0
	beq .L_0200995a
	movs r3, #37
	movs r2, #14
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #22
	movs r1, #0
	movs r2, #1
	movs r3, #1
	bl Func_02005178
	ldr r0, .L_020099cc
	bl Scheduler_RemoveCallbackFar
	b .L_020099ac
.L_0200995a:
	movs r3, #175
	lsls r3, r3, #4
	b .L_020099aa
.L_02009960:
	ldr r3, [r5, #12]
	ldr r2, .L_020099d0
	movs r0, #16
	adds r3, r3, r2
	movs r2, #152
	lsls r2, r2, #7
	adds r2, #204
	str r3, [r5, #12]
	adds r1, r2, #0
	bl Func_02000cf8
	ldr r3, [r5, #12]
	ldr r1, .L_020099c4
	cmp r3, r1
	bge .L_02009986
	movs r0, #14
	movs r1, #3
	bl Func_020052b8
.L_02009986:
	ldr r3, [r5, #12]
	ldr r2, .L_020099bc
	cmp r3, r2
	bgt .L_020099ac
	movs r0, #245
	bl Func_02005388
	movs r0, #14
	movs r1, #0
	movs r2, #0
	bl Func_02005230
	movs r0, #16
	movs r1, #0
	movs r2, #0
	bl Func_02005230
	movs r3, #0
.L_020099aa:
	str r3, [r7]
.L_020099ac:
	ldr r2, .L_020099b8
	add sp, #8
	ldr r3, [r2]
	adds r3, #1
	str r3, [r2]
	pop {r5, r6, r7, pc}
.L_020099b8:
	.4byte gOverlayArea + 0x62a4
.L_020099bc:
	.4byte 0xffec0000
.L_020099c0:
	.4byte 0xfffffed3
.L_020099c4:
	.4byte 0xfffb0000
.L_020099c8:
	.4byte gOverlayArea + 0x62aa
.L_020099cc:
	.4byte Func_02001804
.L_020099d0:
	.4byte 0xfffc0000
	.section .text.x020099d4,"ax",%progbits
	.global Func_020019d4
	.thumb_func
Func_020019d4:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r3, .L_02009b38
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	adds r5, r0, #0
	ldr r3, [r5, #8]
	movs r6, #0
	asrs r3, r3, #20
	mov r8, r3
	ldr r3, [r5, #16]
	asrs r7, r3, #20
	mov r3, r8
	cmp r3, #13
	bne .L_02009a0c
	cmp r7, #13
	bne .L_02009a0c
	movs r0, #130
	lsls r0, r0, #2
	bl Func_02005118
	movs r6, #1
	b .L_02009a14
.L_02009a0c:
	movs r0, #130
	lsls r0, r0, #2
	bl Func_02005120
.L_02009a14:
	mov r2, r8
	cmp r2, #23
	bne .L_02009a2c
	cmp r7, #15
	bne .L_02009a2c
	movs r0, #133
	lsls r0, r0, #1
	adds r0, #255
	bl Func_02005118
	movs r6, #1
	b .L_02009a36
.L_02009a2c:
	movs r0, #133
	lsls r0, r0, #1
	adds r0, #255
	bl Func_02005120
.L_02009a36:
	cmp r6, #0
	beq .L_02009a44
	movs r0, #126
	adds r0, #255
	bl Func_02005118
	b .L_02009a4c
.L_02009a44:
	movs r0, #126
	adds r0, #255
	bl Func_02005120
.L_02009a4c:
	ldr r3, .L_02009b3c
	ldr r3, [r3]
	cmp r3, #0
	beq .L_02009a8c
	ldr r3, [r5, #12]
	cmp r3, #0
	bne .L_02009a8c
	ldr r3, [r5, #8]
	movs r2, #142
	lsls r2, r2, #17
	cmp r3, r2
	ble .L_02009a8c
	movs r2, #154
	lsls r2, r2, #17
	cmp r3, r2
	bge .L_02009a8c
	ldr r3, [r5, #16]
	movs r2, #200
	lsls r2, r2, #16
	cmp r3, r2
	bge .L_02009a8c
	movs r0, #134
	lsls r0, r0, #1
	adds r0, #255
	bl Func_02005110
	cmp r0, #0
	bne .L_02009a8c
	ldr r1, .L_02009b40
	adds r0, r5, #0
	bl Func_02000778
.L_02009a8c:
	ldr r3, .L_02009b44
	ldr r3, [r3]
	cmp r3, #0
	beq .L_02009aca
	ldr r3, [r5, #12]
	cmp r3, #0
	bne .L_02009aca
	ldr r3, [r5, #8]
	movs r2, #165
	lsls r2, r2, #17
	cmp r3, r2
	ble .L_02009aca
	ldr r2, .L_02009b48
	cmp r3, r2
	bgt .L_02009aca
	ldr r3, [r5, #16]
	movs r2, #216
	lsls r2, r2, #16
	cmp r3, r2
	bge .L_02009aca
	movs r0, #134
	lsls r0, r0, #1
	adds r0, #255
	bl Func_02005110
	cmp r0, #0
	bne .L_02009aca
	ldr r1, .L_02009b4c
	adds r0, r5, #0
	bl Func_02000778
.L_02009aca:
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #14
	bl Func_02005110
	cmp r0, #0
	beq .L_02009b06
	movs r0, #131
	lsls r0, r0, #2
	bl Func_02005110
	cmp r0, #0
	bne .L_02009b30
	movs r0, #135
	lsls r0, r0, #1
	adds r0, #255
	bl Func_02005110
	cmp r0, #0
	bne .L_02009b30
	movs r0, #1
	negs r0, r0
	bl Func_02005388
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #14
	bl Func_02005120
	b .L_02009b30
.L_02009b06:
	movs r0, #131
	lsls r0, r0, #2
	bl Func_02005110
	cmp r0, #0
	bne .L_02009b20
	movs r0, #135
	lsls r0, r0, #1
	adds r0, #255
	bl Func_02005110
	cmp r0, #0
	beq .L_02009b30
.L_02009b20:
	movs r0, #162
	bl Func_02005388
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #14
	bl Func_02005118
.L_02009b30:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009b38:
	.4byte gPartyState
.L_02009b3c:
	.4byte gOverlayArea + 0x6298
.L_02009b40:
	.4byte 0x00026666
.L_02009b44:
	.4byte gOverlayArea + 0x629c
.L_02009b48:
	.4byte 0x0165ffff
.L_02009b4c:
	.4byte 0x00039999
	.section .text.x02009b50,"ax",%progbits
	.global Func_02001b50
	.thumb_func
Func_02001b50:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	ldr r3, .L_02009c80
	movs r0, #133
	lsls r0, r0, #2
	adds r0, r0, r3
	mov r9, r0
	ldr r0, [r0]
	bl Object_GetById
	adds r6, r0, #0
	ldr r2, [r6, #8]
	ldr r1, [r6, #16]
	asrs r3, r2, #20
	asrs r0, r1, #20
	mov r10, r3
	ldr r3, [r6, #12]
	mov r8, r0
	movs r0, #128
	lsls r0, r0, #13
	movs r7, #0
	cmp r3, r0
	bgt .L_02009c76
	ldr r0, .L_02009c84
	adds r3, r2, r0
	movs r2, #240
	lsls r2, r2, #16
	cmp r3, r2
	bhi .L_02009c76
	ldr r0, .L_02009c88
	movs r2, #200
	adds r3, r1, r0
	lsls r2, r2, #15
	cmp r3, r2
	bhi .L_02009c76
	mov r3, r10
	cmp r3, #27
	bne .L_02009bda
	mov r0, r8
	cmp r0, #12
	bne .L_02009bda
	movs r0, #13
	bl Object_GetById
	mov r2, r9
	adds r5, r0, #0
	ldr r0, [r2]
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #13
	movs r1, #4
	bl Object_SetModeById
	adds r3, r6, #0
	adds r3, #85
	strb r7, [r3]
	movs r0, #130
	ldr r3, [r5, #12]
	lsls r0, r0, #2
	str r3, [r6, #12]
	bl Func_02005118
	movs r7, #1
.L_02009bda:
	mov r3, r10
	cmp r3, #37
	bne .L_02009c22
	mov r0, r8
	cmp r0, #14
	bne .L_02009c22
	movs r0, #14
	bl Object_GetById
	ldr r3, .L_02009c80
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	adds r5, r0, #0
	ldr r0, [r3]
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #14
	movs r1, #4
	bl Object_SetModeById
	adds r2, r6, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	movs r0, #133
	ldr r3, [r5, #12]
	lsls r0, r0, #1
	str r3, [r6, #12]
	adds r0, #255
	bl Func_02005118
	movs r7, #1
.L_02009c22:
	cmp r7, #0
	beq .L_02009c30
	movs r0, #126
	adds r0, #255
	bl Func_02005118
	b .L_02009c76
.L_02009c30:
	movs r0, #13
	movs r1, #0
	bl Object_SetModeById
	movs r1, #0
	movs r0, #14
	bl Object_SetModeById
	ldr r3, .L_02009c80
	movs r0, #133
	lsls r0, r0, #2
	adds r3, r3, r0
	ldr r0, [r3]
	bl Object_GetById
	movs r1, #1
	bl ObjectDispatch_SetSingleChildField26
	adds r2, r6, #0
	adds r2, #85
	movs r3, #3
	movs r0, #130
	strb r3, [r2]
	lsls r0, r0, #2
	bl Func_02005120
	movs r0, #133
	lsls r0, r0, #1
	adds r0, #255
	bl Func_02005120
	movs r0, #126
	adds r0, #255
	bl Func_02005120
.L_02009c76:
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_02009c80:
	.4byte gPartyState
.L_02009c84:
	.4byte 0xfe700000
.L_02009c88:
	.4byte 0xff4c0000
	.section .text.x02009c8c,"ax",%progbits
	.global Func_02001c8c
	.thumb_func
Func_02001c8c:
	push {r5, lr}
	ldr r3, .L_02009cc0
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	ldr r3, [r0, #80]
	movs r0, #13
	ldrb r5, [r3, #9]
	lsls r5, r5, #28
	lsrs r5, r5, #30
	adds r1, r5, #0
	bl Func_020052b8
	adds r1, r5, #0
	movs r0, #14
	bl Func_020052b8
	movs r0, #15
	adds r1, r5, #0
	bl Func_020052b8
	pop {r5, pc}
	.2byte 0x0000
.L_02009cc0:
	.4byte gPartyState
	.section .text.x02009cc4,"ax",%progbits
	.global Func_02001cc4
	.thumb_func
Func_02001cc4:
	push {r5, r6, lr}
	ldr r3, .L_02009cfc
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #32]
	adds r6, r0, #0
	bl Func_020053b0
	movs r3, #184
	lsls r3, r3, #1
	adds r5, r5, r3
	ldr r3, [r5]
	lsls r0, r0, #2
	adds r3, r3, r0
	ldrb r3, [r3, #2]
	cmp r3, #0
	beq .L_02009cf8
	movs r3, #0
	str r3, [r6, #12]
	str r3, [r6, #20]
.L_02009cf8:
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02009cfc:
	.4byte gPartyState
	.section .text.x02009d00,"ax",%progbits
	.global Func_02001d00
	.thumb_func
Func_02001d00:
	push {lr}
	bl Func_020051c0
	movs r0, #0
	bl Func_02005370
	ldr r0, .L_02009d1c
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
	bl Func_020051c8
	pop {pc}
	.2byte 0x0000
.L_02009d1c:
	.4byte 0x000017b7
	.section .text.x02009d20,"ax",%progbits
	.global Func_02001d20
	.thumb_func
Func_02001d20:
	push {lr}
	bl Func_020051c0
	movs r0, #0
	bl Func_02005370
	ldr r0, .L_02009d3c
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
	bl Func_020051c8
	pop {pc}
	.2byte 0x0000
.L_02009d3c:
	.4byte 0x00000dfa
	.section .text.x02009d40,"ax",%progbits
	.global Func_02001d40
	.thumb_func
Func_02001d40:
	push {lr}
	bl Func_020051c0
	movs r0, #0
	bl Func_02005370
	movs r0, #9
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #1
	bl WaitFrames
	ldr r3, .L_02009da8
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r1, [r3]
	movs r2, #0
	movs r0, #9
	bl Func_02005268
	ldr r0, .L_02009dac
	bl Func_02005280
	movs r1, #0
	movs r0, #9
	bl Func_020052a0
	movs r0, #9
	bl Object_GetById
	movs r3, #192
	lsls r3, r3, #6
	strh r3, [r0, #6]
	movs r0, #1
	bl WaitFrames
	ldr r1, .L_02009db0
	movs r0, #9
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #1
	bl WaitFrames
	bl Func_020051c8
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #69
	bl Func_02005118
	pop {pc}
.L_02009da8:
	.4byte gPartyState
.L_02009dac:
	.4byte 0x00001759
.L_02009db0:
	.4byte Data_02005514
	.section .text.x02009db4,"ax",%progbits
	.global Func_02001db4
	.thumb_func
Func_02001db4:
	push {lr}
	bl Func_020051c0
	movs r0, #0
	bl Func_02005370
	ldr r3, .L_02009e04
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r1, [r3]
	movs r2, #0
	movs r0, #10
	bl Func_02005268
	ldr r0, .L_02009e08
	bl Func_02005280
	movs r1, #0
	movs r0, #10
	bl Func_02005298
	movs r0, #10
	bl Object_GetById
	movs r3, #160
	lsls r3, r3, #7
	strh r3, [r0, #6]
	movs r0, #1
	bl WaitFrames
	bl Func_020051c8
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #70
	bl Func_02005118
	pop {pc}
	.2byte 0x0000
.L_02009e04:
	.4byte gPartyState
.L_02009e08:
	.4byte 0x0000175c
	.section .text.x02009e0c,"ax",%progbits
	.global Func_02001e0c
	.thumb_func
Func_02001e0c:
	push {lr}
	bl Func_020051c0
	movs r0, #0
	bl Func_02005370
	ldr r0, .L_02009e2c
	bl Func_02005280
	movs r1, #0
	movs r0, #13
	bl Func_020052a0
	bl Func_020051c8
	pop {pc}
.L_02009e2c:
	.4byte 0x000017a9
	.section .text.x02009e30,"ax",%progbits
	.global Func_02001e30
	.thumb_func
Func_02001e30:
	push {lr}
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #9
	bl Func_020052c8
	bl Func_02001d40
	pop {pc}
	.section .text.x02009e44,"ax",%progbits
	.global Func_02001e44
	.thumb_func
Func_02001e44:
	push {lr}
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #10
	bl Func_020052c8
	bl Func_02001db4
	pop {pc}
	.section .text.x02009e58,"ax",%progbits
	.global Func_02001e58
	.thumb_func
Func_02001e58:
	push {r5, r6, lr}
	ldr r3, .L_02009f30
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	adds r6, r0, #0
	ldr r3, [r6, #8]
	asrs r1, r3, #20
	ldr r3, [r6, #16]
	asrs r3, r3, #20
	cmp r3, #5
	bne .L_02009e98
	cmp r1, #9
	bne .L_02009e86
	ldr r3, .L_02009f34
	movs r2, #16
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	beq .L_02009f2e
.L_02009e86:
	cmp r1, #11
	bne .L_02009eac
	ldr r3, .L_02009f34
	movs r2, #32
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	bne .L_02009eac
	b .L_02009f2e
.L_02009e98:
	cmp r1, #10
	bne .L_02009eac
	cmp r3, #6
	bne .L_02009eac
	ldr r3, .L_02009f34
	movs r2, #64
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	beq .L_02009f2e
.L_02009eac:
	bl Func_020051c0
	movs r0, #0
	bl Func_02005370
	adds r2, r6, #0
	adds r2, #85
	movs r3, #0
	ldr r5, .L_02009f30
	strb r3, [r2]
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	movs r1, #128
	movs r2, #128
	ldr r0, [r5]
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r2, #80
	ldr r0, [r5]
	movs r1, #168
	bl ObjectMotion_SetPositionAndReset
	movs r1, #192
	lsls r1, r1, #8
	ldr r0, [r5]
	bl Func_020052b0
	ldr r0, [r5]
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	ldr r0, [r5]
	movs r1, #13
	bl Object_SetModeById
	movs r1, #168
	movs r3, #160
	ldr r2, .L_02009f38
	lsls r3, r3, #15
	lsls r1, r1, #16
	adds r0, r6, #0
	bl Func_02005160
	adds r0, r6, #0
	bl Func_02005168
	movs r1, #10
	ldr r0, [r5]
	bl Object_SetModeById
	movs r0, #123
	bl Func_020053d8
	bl Event_ClearStatus1c6
	bl Event_WaitValue1c8Frames
	movs r0, #8
	bl Func_020052f8
.L_02009f2e:
	pop {r5, r6, pc}
.L_02009f30:
	.4byte gPartyState
.L_02009f34:
	.4byte gInput
.L_02009f38:
	.4byte 0xfff40000
	.section .text.x02009f3c,"ax",%progbits
	.global Func_02001f3c
	.thumb_func
Func_02001f3c:
	push {r5, lr}
	sub sp, #8
	movs r5, #2
	movs r1, #39
	movs r2, #24
	movs r3, #38
	movs r0, #39
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl Func_02005170
	movs r0, #10
	bl WaitFrames
	movs r1, #41
	movs r2, #24
	movs r3, #38
	movs r0, #39
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl Func_02005170
	movs r0, #20
	bl WaitFrames
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #18
	bl Func_02005118
	add sp, #8
	pop {r5, pc}
	.section .text.x02009f7c,"ax",%progbits
	.global Func_02001f7c
	.thumb_func
Func_02001f7c:
	push {r5, lr}
	bl Func_020051c0
	movs r0, #0
	bl Func_02005370
	ldr r3, .L_0200a010
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r3, r2
	movs r1, #128
	movs r2, #128
	ldr r0, [r5]
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r1, #200
	movs r2, #162
	ldr r0, [r5]
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_SetPositionAndReset
	movs r1, #192
	ldr r0, [r5]
	lsls r1, r1, #8
	bl Func_020052b0
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #18
	bl Func_02005110
	cmp r0, #0
	bne .L_02009fce
	movs r0, #188
	bl Func_020053d8
	bl Func_02001f3c
.L_02009fce:
	ldr r0, [r5]
	movs r1, #2
	bl Object_SetModeById
	movs r1, #128
	movs r2, #128
	ldr r0, [r5]
	lsls r2, r2, #7
	lsls r1, r1, #8
	bl ObjectMotion_SetSpeedParameters
	ldr r0, [r5]
	movs r1, #3
	bl Func_020052b8
	movs r1, #200
	movs r2, #160
	lsls r1, r1, #1
	lsls r2, r2, #2
	ldr r0, [r5]
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r0, #123
	bl Func_020053d8
	bl Event_ClearStatus1c6
	bl Event_WaitValue1c8Frames
	movs r0, #9
	bl Func_020052f8
	pop {r5, pc}
.L_0200a010:
	.4byte gPartyState
	.section .text.x0200a014,"ax",%progbits
	.global Func_02002014
	.thumb_func
Func_02002014:
	push {r5, r6, lr}
	ldr r5, .L_0200a0c0
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
	ldr r0, [r5]
	bl Object_GetById
	adds r6, r0, #0
	bl Func_020051c0
	movs r0, #0
	bl Func_02005370
	movs r1, #208
	movs r2, #144
	ldr r0, [r5]
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #128
	ldr r0, [r5]
	lsls r1, r1, #7
	movs r2, #10
	bl Func_020052a8
	movs r1, #192
	ldr r0, [r5]
	lsls r1, r1, #8
	movs r2, #0
	bl Func_020052a8
	adds r2, r6, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	movs r1, #1
	ldr r0, [r5]
	bl Func_020052b8
	ldr r0, [r5]
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r5, #0
.L_0200a074:
	ldr r3, [r6, #16]
	movs r2, #128
	lsls r2, r2, #9
	adds r3, r3, r2
	str r3, [r6, #16]
	movs r0, #1
	adds r5, #1
	bl WaitFrames
	cmp r5, #7
	bls .L_0200a074
	ldr r3, .L_0200a0c0
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	movs r1, #13
	bl Object_SetModeById
	movs r5, #0
.L_0200a09c:
	ldr r3, [r6, #12]
	ldr r2, .L_0200a0c4
	movs r0, #1
	adds r3, r3, r2
	str r3, [r6, #12]
	adds r5, #1
	bl WaitFrames
	cmp r5, #7
	bls .L_0200a09c
	bl Event_ClearStatus1c6
	bl Event_WaitValue1c8Frames
	movs r0, #4
	bl Func_020052f8
	pop {r5, r6, pc}
.L_0200a0c0:
	.4byte gPartyState
.L_0200a0c4:
	.4byte 0xffff0000
	.section .text.x0200a0c8,"ax",%progbits
	.global Func_020020c8
	.thumb_func
Func_020020c8:
	push {lr}
	ldr r0, .L_0200a0d4
	bl Func_020053c0
	pop {pc}
	.2byte 0x0000
.L_0200a0d4:
	.4byte Data_020054dc
	.section .text.x0200a0d8,"ax",%progbits
	.global Func_020020d8
	.thumb_func
Func_020020d8:
	push {r5, lr}
	movs r0, #12
	sub sp, #8
	bl Object_GetById
	ldr r3, [r0, #8]
	asrs r5, r3, #20
	bl Func_020053c8
	cmp r5, #43
	bne .L_0200a0fa
	movs r0, #137
	lsls r0, r0, #1
	adds r0, #255
	bl Func_02005118
	b .L_0200a104
.L_0200a0fa:
	movs r0, #137
	lsls r0, r0, #1
	adds r0, #255
	bl Func_02005120
.L_0200a104:
	cmp r5, #39
	beq .L_0200a11c
	movs r3, #39
	movs r2, #10
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #39
	movs r1, #0
	movs r2, #1
	movs r3, #1
	bl Func_02005178
.L_0200a11c:
	add sp, #8
	pop {r5, pc}
	.section .text.x0200a120,"ax",%progbits
	.global Func_02002120
	.thumb_func
Func_02002120:
	push {lr}
	movs r0, #8
	bl Object_GetById
	ldr r3, [r0, #8]
	asrs r3, r3, #20
	subs r3, #15
	cmp r3, #6
	bls .L_0200a13c
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #10
	bl Func_02005118
.L_0200a13c:
	movs r0, #134
	lsls r0, r0, #1
	adds r0, #255
	bl Func_02005118
	ldr r0, .L_0200a150
	bl Func_020053c0
	pop {pc}
	.2byte 0x0000
.L_0200a150:
	.4byte Data_020054e0
	.section .text.x0200a154,"ax",%progbits
	.global Func_02002154
	.thumb_func
Func_02002154:
	push {r5, r6, lr}
	movs r0, #8
	bl Object_GetById
	ldr r3, [r0, #8]
	asrs r6, r3, #20
	ldr r3, [r0, #16]
	asrs r5, r3, #20
	bl Func_020053c8
	cmp r6, #13
	bne .L_0200a17a
	cmp r5, #13
	bne .L_0200a17a
	movs r0, #192
	lsls r0, r0, #2
	bl Func_02005118
	b .L_0200a182
.L_0200a17a:
	movs r0, #192
	lsls r0, r0, #2
	bl Func_02005120
.L_0200a182:
	cmp r6, #23
	bne .L_0200a196
	cmp r5, #15
	bne .L_0200a196
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #1
	bl Func_02005118
	b .L_0200a1a0
.L_0200a196:
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #1
	bl Func_02005120
.L_0200a1a0:
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #10
	bl Func_02005120
	movs r0, #134
	lsls r0, r0, #1
	adds r0, #255
	bl Func_02005120
	pop {r5, r6, pc}
	.2byte 0x0000
	.section .text.x0200a1b8,"ax",%progbits
	.global Func_020021b8
	.thumb_func
Func_020021b8:
	push {lr}
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #2
	bl Func_02005120
	movs r0, #129
	lsls r0, r0, #2
	adds r0, #255
	bl Func_02005120
	ldr r0, .L_0200a1d8
	bl Func_020053c0
	pop {pc}
	.2byte 0x0000
.L_0200a1d8:
	.4byte Data_020054e0
	.section .text.x0200a1dc,"ax",%progbits
	.global Func_020021dc
	.thumb_func
Func_020021dc:
	push {r5, lr}
	movs r0, #8
	sub sp, #8
	bl Object_GetById
	ldr r3, [r0, #8]
	asrs r5, r3, #20
	bl Func_020053c8
	cmp r5, #19
	bne .L_0200a218
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #2
	bl Func_02005118
	movs r0, #193
	lsls r0, r0, #2
	bl Func_02005118
	movs r3, #28
	str r3, [sp, #4]
	movs r0, #0
	movs r1, #0
	movs r2, #3
	movs r3, #3
	str r5, [sp, #0]
	bl Func_02005178
	b .L_0200a242
.L_0200a218:
	cmp r5, #21
	bne .L_0200a242
	movs r0, #129
	lsls r0, r0, #2
	adds r0, #255
	bl Func_02005118
	movs r0, #193
	lsls r0, r0, #2
	bl Func_02005118
	movs r3, #19
	movs r2, #28
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #4
	movs r2, #3
	movs r3, #3
	bl Func_02005178
.L_0200a242:
	add sp, #8
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x0200a248,"ax",%progbits
	.global Func_02002248
	.thumb_func
Func_02002248:
	push {r5, r6, lr}
	ldr r5, .L_0200a2bc
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
	ldr r0, [r5]
	bl Object_GetById
	adds r6, r0, #0
	bl Func_020051c0
	movs r0, #0
	bl Func_02005370
	movs r1, #128
	movs r2, #20
	lsls r1, r1, #1
	ldr r0, [r5]
	bl Func_020052c8
	movs r0, #132
	bl Func_020053d8
	ldr r0, [r5]
	bl Object_GetById
	movs r1, #10
	bl Func_02005278
	movs r3, #192
	lsls r3, r3, #11
	str r3, [r6, #40]
	movs r0, #20
	bl WaitFrames
	ldr r3, [r6, #40]
	cmp r3, #0
	beq .L_0200a2a0
.L_0200a294:
	movs r0, #1
	bl WaitFrames
	ldr r3, [r6, #40]
	cmp r3, #0
	bne .L_0200a294
.L_0200a2a0:
	ldr r3, .L_0200a2bc
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	movs r1, #0
	bl Func_02005278
	bl Func_020051c8
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200a2bc:
	.4byte gPartyState
	.section .text.x0200a2c0,"ax",%progbits
	.global Func_020022c0
	.thumb_func
Func_020022c0:
	push {lr}
	sub sp, #8
	bl Func_020051c0
	movs r0, #0
	bl Func_02005370
	movs r0, #188
	bl Func_020053d8
	movs r3, #2
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #77
	movs r1, #33
	movs r2, #77
	movs r3, #24
	bl Func_02005170
	movs r3, #17
	movs r2, #25
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #30
	movs r2, #2
	movs r3, #1
	movs r0, #32
	bl Func_02005178
	movs r0, #10
	bl WaitFrames
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #18
	bl Func_02005118
	bl Func_020051c8
	add sp, #8
	pop {pc}
	.2byte 0x0000
	.section .text.x0200a314,"ax",%progbits
	.global Func_02002314
	.thumb_func
Func_02002314:
	push {lr}
	sub sp, #8
	movs r3, #5
	movs r2, #13
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #1
	movs r1, #0
	movs r2, #1
	movs r0, #0
	bl Func_02005178
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #66
	bl Func_02005118
	bl Func_020051c0
	movs r0, #0
	bl Func_02005370
	movs r0, #4
	movs r1, #89
	movs r2, #237
	bl ObjectMotion_SetPositionAndReset
	movs r1, #128
	movs r0, #4
	lsls r1, r1, #6
	movs r2, #0
	bl Func_020052a8
	movs r3, #160
	lsls r3, r3, #8
	movs r2, #16
	movs r1, #16
	movs r0, #14
	bl Func_02005378
	movs r0, #14
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #20
	bl Battle_WaitMode0
	ldr r0, .L_0200a3e0
	bl Func_02005280
	movs r1, #0
	movs r0, #14
	bl Func_02005298
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #14
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #14
	ldr r1, .L_0200a3e4
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r0, #14
	movs r1, #2
	bl Object_SetModeById
	movs r0, #4
	bl Object_GetById
	cmp r0, #0
	beq .L_0200a3c8
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #14
	bl ObjectMotion_ResetAndSetPosition
.L_0200a3c8:
	movs r0, #14
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #14
	movs r1, #0
	movs r2, #0
	bl Func_02005230
	bl Func_020051c8
	add sp, #8
	pop {pc}
.L_0200a3e0:
	.4byte 0x000017ba
.L_0200a3e4:
	.4byte 0x00013333
	.section .text.x0200a3e8,"ax",%progbits
	.global Func_020023e8
	.thumb_func
Func_020023e8:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, .L_0200a5a4
	movs r2, #133
	lsls r2, r2, #2
	adds r2, r2, r3
	ldr r0, [r2]
	mov r8, r2
	bl Object_GetById
	movs r3, #6
	ldrsh r5, [r0, r3]
	bl Func_020051c0
	movs r0, #0
	bl Func_02005370
	bl Func_020052f0
	movs r2, #0
	mov r10, r2
	mov r3, r10
	adds r0, #85
	strb r3, [r0]
	movs r1, #0
	movs r0, #12
	bl Func_020052a8
	ldr r0, .L_0200a5a8
	bl Func_02005280
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #12
	movs r1, #0
	bl Func_02005298
	mov r2, r8
	ldr r0, [r2]
	movs r1, #12
	movs r2, #0
	bl Func_02005268
	movs r0, #12
	movs r1, #3
	bl Object_SetModeById
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #12
	movs r1, #0
	bl Func_02005298
	movs r0, #12
	movs r1, #3
	bl Object_SetModeById
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #12
	movs r1, #0
	bl Func_02005298
	movs r0, #12
	movs r1, #4
	bl Object_SetModeById
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #12
	movs r1, #0
	bl Func_02005298
	mov r3, r8
	ldr r0, [r3]
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #12
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #12
	movs r1, #0
	lsls r5, r5, #16
	bl Func_02005298
	lsrs r5, r5, #16
	mov r2, r8
	ldr r0, [r2]
	adds r1, r5, #0
	movs r2, #10
	bl Func_020052a8
	ldr r5, .L_0200a5ac
	ldr r0, [r5]
	cmp r0, #0
	beq .L_0200a4ba
	movs r1, #3
	bl Func_02005330
.L_0200a4ba:
	movs r0, #198
	movs r1, #0
	bl PartyInventory_GiveItem
	ldr r0, [r5]
	cmp r0, #0
	beq .L_0200a4cc
	bl Func_02005148
.L_0200a4cc:
	movs r0, #12
	movs r1, #4
	bl Object_SetModeById
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #12
	movs r1, #0
	bl Func_02005298
	movs r0, #204
	movs r1, #192
	lsls r0, r0, #7
	lsls r1, r1, #4
	adds r0, #102
	adds r1, #204
	bl Func_020052d8
	movs r0, #212
	movs r2, #200
	movs r3, #1
	movs r1, #0
	lsls r2, r2, #17
	lsls r0, r0, #17
	bl Func_020052e0
	movs r0, #12
	bl Object_GetById
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	adds r5, r0, #0
	adds r1, #204
	movs r0, #12
	adds r2, #102
	ldr r6, [r5, #80]
	bl ObjectMotion_SetSpeedParameters
	movs r1, #212
	movs r2, #188
	lsls r2, r2, #1
	movs r0, #12
	lsls r1, r1, #1
	bl ObjectMotion_SetPositionAndReset
	movs r0, #12
	movs r1, #1
	bl Func_020052b8
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #12
	ldr r1, .L_0200a5b0
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r0, #12
	movs r1, #4
	movs r2, #0
	bl ObjectMotion_Launch
	movs r1, #222
	movs r2, #188
	lsls r2, r2, #1
	movs r0, #12
	lsls r1, r1, #1
	bl ObjectMotion_SetPositionAndCommit
	movs r1, #7
	movs r0, #12
	bl Object_SetModeById
	ldr r3, .L_0200a5b4
	ldr r2, .L_0200a5a0
	str r3, [r5, #24]
	adds r7, r5, #0
	movs r3, #224
	adds r7, #85
	lsls r3, r3, #8
	strb r2, [r7]
	strh r3, [r6, #18]
	movs r2, #128
	ldr r3, [r5, #12]
	lsls r2, r2, #11
	adds r3, r3, r2
	str r3, [r5, #12]
	movs r0, #12
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #7
	lsls r2, r2, #6
	movs r0, #12
	adds r1, #102
	adds r2, #51
	bl ObjectMotion_SetSpeedParameters
	movs r1, #246
	movs r2, #128
	b .L_0200a5b8
.L_0200a5a0:
	.4byte 0x00000000
.L_0200a5a4:
	.4byte gPartyState
.L_0200a5a8:
	.4byte 0x000017a1
.L_0200a5ac:
	.4byte gOverlayArea + 0x62ac
.L_0200a5b0:
	.4byte 0x00019999
.L_0200a5b4:
	.4byte 0xffff0000
.L_0200a5b8:
	movs r3, #188
	adds r0, r5, #0
	lsls r1, r1, #17
	lsls r2, r2, #12
	lsls r3, r3, #17
	bl Func_02005160
	movs r0, #12
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r3, #3
	strb r3, [r7]
	mov r3, r10
	str r3, [r5, #20]
	movs r3, #128
	lsls r3, r3, #9
	mov r2, r10
	str r3, [r5, #24]
	movs r1, #1
	strh r2, [r6, #18]
	movs r0, #12
	bl Object_SetModeById
	movs r0, #12
	bl Object_GetById
	movs r1, #1
	bl ObjectDispatch_SetSingleChildField26
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #12
	ldr r1, .L_0200a670
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r0, #12
	movs r1, #4
	movs r2, #0
	bl ObjectMotion_Launch
	movs r1, #252
	movs r2, #188
	movs r0, #12
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndCommit
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #12
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #248
	movs r2, #197
	movs r0, #12
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #216
	movs r2, #197
	lsls r2, r2, #1
	movs r0, #12
	lsls r1, r1, #1
	bl ObjectMotion_SetPositionAndReset
	mov r3, r8
	ldr r1, [r3]
	movs r0, #12
	bl Object_LinkObjectAndSetCallback
	movs r1, #2
	movs r0, #12
	bl Func_020052b8
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #74
	bl Func_02005118
	bl Func_020051c8
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200a670:
	.4byte 0x00019999
	.section .text.x0200a674,"ax",%progbits
	.global Func_02002674
	.thumb_func
Func_02002674:
	push {r5, lr}
	ldr r5, .L_0200a698
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	ldr r0, [r5]
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #32
	orrs r3, r2
	strb r3, [r0]
	movs r1, #1
	ldr r0, [r5]
	bl Func_020052b8
	pop {r5, pc}
.L_0200a698:
	.4byte gPartyState
	.section .text.x0200a69c,"ax",%progbits
	.global Func_0200269c
	.thumb_func
Func_0200269c:
	push {r5, lr}
	ldr r5, .L_0200a6c8
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	ldr r0, [r5]
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #223
	ands r3, r2
	strb r3, [r0]
	ldr r0, [r5]
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	strb r3, [r0]
	pop {r5, pc}
.L_0200a6c8:
	.4byte gPartyState
	.section .text.x0200a6cc,"ax",%progbits
	.global Func_020026cc
	.thumb_func
Func_020026cc:
	push {r5, r6, lr}
	bl Func_020051c0
	movs r0, #0
	bl Func_02005370
	movs r0, #164
	movs r1, #1
	movs r2, #174
	negs r1, r1
	lsls r2, r2, #16
	movs r3, #1
	lsls r0, r0, #17
	bl Func_020052e0
	bl Func_020052e8
	movs r0, #20
	bl Battle_WaitMode0
	ldr r6, .L_0200a7a8
	movs r3, #133
	lsls r3, r3, #2
	movs r2, #204
	adds r5, r6, r3
	lsls r2, r2, #8
	ldr r0, [r5]
	ldr r1, .L_0200a7ac
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r1, #164
	movs r2, #174
	ldr r0, [r5]
	lsls r1, r1, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #2
	movs r0, #11
	bl Motion_SetVarCbAndRefresh
	movs r0, #146
	bl Func_020053d8
	movs r1, #2
	movs r0, #11
	bl Object_SetModeById
	movs r0, #11
	bl Object_GetById
	movs r3, #128
	lsls r3, r3, #11
	str r3, [r0, #40]
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #146
	bl Func_020053d8
	movs r0, #12
	movs r1, #2
	bl Object_SetModeById
	movs r1, #2
	movs r0, #13
	bl Object_SetModeById
	movs r0, #12
	bl Object_GetById
	movs r5, #192
	lsls r5, r5, #11
	str r5, [r0, #40]
	movs r0, #13
	bl Object_GetById
	movs r1, #150
	str r5, [r0, #40]
	lsls r1, r1, #1
	movs r0, #12
	movs r2, #164
	bl ObjectMotion_ResetAndSetPosition
	movs r1, #178
	lsls r1, r1, #1
	movs r2, #164
	movs r0, #13
	bl ObjectMotion_SetPositionAndCommit
	movs r0, #40
	bl Battle_WaitMode0
	movs r3, #166
	lsls r3, r3, #1
	adds r3, #255
	adds r6, r6, r3
	movs r3, #2
	strb r3, [r6]
	ldr r0, .L_0200a7b0
	movs r1, #3
	bl Func_02005310
	movs r0, #9
	movs r1, #4
	bl Func_02005300
	bl Func_020051c8
	pop {r5, r6, pc}
.L_0200a7a8:
	.4byte gPartyState
.L_0200a7ac:
	.4byte 0x00019999
.L_0200a7b0:
	.4byte 0x00000010
	.section .text.x0200a7b4,"ax",%progbits
	.global Func_020027b4
	.thumb_func
Func_020027b4:
	push {lr}
	sub sp, #12
	movs r3, #73
	movs r2, #14
	movs r1, #1
	str r3, [sp, #0]
	str r2, [sp, #4]
	str r1, [sp, #8]
	movs r0, #76
	movs r1, #0
	movs r2, #2
	movs r3, #2
	bl Func_020053d0
	add sp, #12
	pop {pc}
	.section .text.x0200a7d4,"ax",%progbits
	.global Func_020027d4
	.thumb_func
Func_020027d4:
	push {lr}
	sub sp, #12
	movs r3, #75
	movs r2, #16
	movs r1, #1
	str r3, [sp, #0]
	str r2, [sp, #4]
	str r1, [sp, #8]
	movs r0, #76
	movs r1, #0
	movs r2, #1
	movs r3, #1
	bl Func_020053d0
	add sp, #12
	pop {pc}
	.section .text.x0200a7f4,"ax",%progbits
	.global Func_020027f4
	.thumb_func
Func_020027f4:
	push {lr}
	sub sp, #12
	movs r3, #74
	movs r2, #18
	movs r1, #1
	str r3, [sp, #0]
	str r2, [sp, #4]
	str r1, [sp, #8]
	movs r0, #76
	movs r1, #0
	movs r2, #1
	movs r3, #1
	bl Func_020053d0
	add sp, #12
	pop {pc}
	.section .text.x0200a814,"ax",%progbits
	.global Func_02002814
	.thumb_func
Func_02002814:
	push {lr}
	sub sp, #12
	movs r3, #80
	movs r2, #18
	movs r1, #1
	str r3, [sp, #0]
	str r2, [sp, #4]
	str r1, [sp, #8]
	movs r0, #76
	movs r1, #0
	movs r2, #1
	movs r3, #1
	bl Func_020053d0
	add sp, #12
	pop {pc}
	.section .text.x0200a834,"ax",%progbits
	.global Func_02002834
	.thumb_func
Func_02002834:
	push {lr}
	sub sp, #12
	movs r3, #87
	movs r2, #15
	movs r1, #1
	str r3, [sp, #0]
	str r2, [sp, #4]
	str r1, [sp, #8]
	movs r0, #76
	movs r1, #0
	movs r2, #1
	movs r3, #1
	bl Func_020053d0
	add sp, #12
	pop {pc}
	.section .text.x0200a854,"ax",%progbits
	.global Func_02002854
	.thumb_func
Func_02002854:
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
	ldr r3, .L_0200a8e0
	subs r2, #36
	adds r3, r3, r2
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_0200a8e4
	cmp r2, r3
	bne .L_0200a87e
	bl Func_0200290c
	b .L_0200a8dc
.L_0200a87e:
	ldr r3, .L_0200a8e8
	cmp r2, r3
	bne .L_0200a88a
	bl Func_02002a5c
	b .L_0200a8dc
.L_0200a88a:
	ldr r3, .L_0200a8ec
	cmp r2, r3
	bne .L_0200a896
	bl Func_02002a84
	b .L_0200a8dc
.L_0200a896:
	ldr r3, .L_0200a8f0
	cmp r2, r3
	bne .L_0200a8a2
	bl Func_02002c1c
	b .L_0200a8dc
.L_0200a8a2:
	ldr r3, .L_0200a8f4
	cmp r2, r3
	bne .L_0200a8ae
	bl Func_02003a14
	b .L_0200a8dc
.L_0200a8ae:
	ldr r3, .L_0200a8f8
	cmp r2, r3
	bne .L_0200a8ba
	bl Func_02003a5c
	b .L_0200a8dc
.L_0200a8ba:
	ldr r3, .L_0200a8fc
	cmp r2, r3
	bne .L_0200a8c6
	bl Func_02003a8c
	b .L_0200a8dc
.L_0200a8c6:
	ldr r3, .L_0200a900
	cmp r2, r3
	bne .L_0200a8d2
	bl Func_02003b2c
	b .L_0200a8dc
.L_0200a8d2:
	ldr r3, .L_0200a904
	cmp r2, r3
	bne .L_0200a8dc
	bl Func_02003c8c
.L_0200a8dc:
	movs r0, #0
	pop {pc}
.L_0200a8e0:
	.4byte gPartyState
.L_0200a8e4:
	.4byte 0x0000000e
.L_0200a8e8:
	.4byte 0x0000000f
.L_0200a8ec:
	.4byte 0x00000010
.L_0200a8f0:
	.4byte 0x00000011
.L_0200a8f4:
	.4byte 0x00000012
.L_0200a8f8:
	.4byte 0x00000013
.L_0200a8fc:
	.4byte 0x00000014
.L_0200a900:
	.4byte 0x00000015
.L_0200a904:
	.4byte 0x00000016
	.section .text.x0200a908,"ax",%progbits
	.global Func_02002908
	.thumb_func
Func_02002908:
	movs r0, #0
	bx lr
	.section .text.x0200a90c,"ax",%progbits
	.global Func_0200290c
	.thumb_func
Func_0200290c:
	push {lr}
	bl Func_02005398
	movs r1, #8
	movs r2, #9
	movs r0, #0
	bl Func_020053a0
	movs r2, #11
	movs r1, #10
	movs r0, #1
	bl Func_020053a0
	movs r0, #8
	movs r1, #4
	bl Object_SetModeById
	movs r0, #9
	movs r1, #4
	bl Object_SetModeById
	movs r0, #10
	movs r1, #4
	bl Object_SetModeById
	movs r1, #4
	movs r0, #11
	bl Object_SetModeById
	ldr r0, .L_0200a990
	bl Func_020053b8
	ldr r3, .L_0200a988
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	ldr r3, .L_0200a98c
	subs r2, #2
	strh r3, [r2]
	movs r0, #245
	bl Func_02005388
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_0200a994
	bl Func_020050d0
	movs r0, #13
	movs r1, #0
	movs r2, #0
	bl Func_02005230
	movs r0, #14
	movs r1, #0
	movs r2, #0
	bl Func_02005230
	movs r0, #15
	movs r1, #0
	movs r2, #0
	b .L_0200a998
.L_0200a988:
	.4byte 0x00000c08
.L_0200a98c:
	.4byte 0x00003f10
.L_0200a990:
	.4byte Data_020054dc
.L_0200a994:
	.4byte Func_02000c74
.L_0200a998:
	bl Func_02005230
	movs r2, #0
	movs r1, #0
	movs r0, #16
	bl Func_02005230
	movs r0, #13
	bl Func_02000b78
	movs r0, #14
	bl Func_02000b78
	movs r0, #15
	bl Func_02000bac
	movs r0, #16
	bl Func_02000bac
	ldr r3, .L_0200aa3c
	movs r2, #0
	str r2, [r3]
	ldr r3, .L_0200aa40
	movs r0, #10
	str r2, [r3]
	ldr r3, .L_0200aa44
	adds r0, #255
	strh r2, [r3]
	ldr r3, .L_0200aa48
	strh r2, [r3]
	bl Func_02005110
	cmp r0, #0
	bne .L_0200a9fa
	ldr r3, .L_0200aa4c
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #5
	bne .L_0200a9f2
	bl Func_02003684
	b .L_0200a9fa
.L_0200a9f2:
	cmp r3, #6
	bne .L_0200a9fa
	bl Func_020037e0
.L_0200a9fa:
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #1
	bl Func_02005110
	cmp r0, #0
	beq .L_0200aa14
	movs r1, #144
	ldr r0, .L_0200aa50
	lsls r1, r1, #3
	bl Func_020050d0
	b .L_0200aa2a
.L_0200aa14:
	movs r0, #192
	lsls r0, r0, #2
	bl Func_02005110
	cmp r0, #0
	beq .L_0200aa2a
	movs r1, #144
	ldr r0, .L_0200aa54
	lsls r1, r1, #3
	bl Func_020050d0
.L_0200aa2a:
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_0200aa58
	bl Func_020050d0
	movs r0, #0
	bl Func_02005338
	pop {pc}
.L_0200aa3c:
	.4byte gOverlayArea + 0x62a0
.L_0200aa40:
	.4byte gOverlayArea + 0x62a4
.L_0200aa44:
	.4byte gOverlayArea + 0x62a8
.L_0200aa48:
	.4byte gOverlayArea + 0x62aa
.L_0200aa4c:
	.4byte gPartyState
.L_0200aa50:
	.4byte Func_02001634
.L_0200aa54:
	.4byte Func_02001804
.L_0200aa58:
	.4byte Func_02001b50
	.section .text.x0200aa5c,"ax",%progbits
	.global Func_02002a5c
	.thumb_func
Func_02002a5c:
	push {lr}
	movs r0, #0
	bl Func_02005338
	ldr r3, .L_0200aa80
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #1
	bne .L_0200aa7c
	movs r0, #48
	adds r0, #255
	bl Func_02005120
.L_0200aa7c:
	pop {pc}
	.2byte 0x0000
.L_0200aa80:
	.4byte gPartyState
	.section .text.x0200aa84,"ax",%progbits
	.global Func_02002a84
	.thumb_func
Func_02002a84:
	push {r5, r6, lr}
	movs r0, #9
	sub sp, #8
	bl Object_GetById
	adds r6, r0, #0
	movs r0, #9
	bl Func_020052c0
	adds r1, r6, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #2
	orrs r3, r2
	strb r3, [r1]
	movs r0, #9
	movs r1, #2
	bl Object_SetModeById
	movs r0, #8
	bl Object_GetById
	adds r6, r0, #0
	adds r3, r6, #0
	movs r0, #192
	movs r5, #0
	adds r3, #85
	lsls r0, r0, #2
	strb r5, [r3]
	adds r0, #2
	bl Func_02005110
	cmp r0, #0
	beq .L_0200aaf4
	movs r0, #8
	bl Object_GetById
	movs r1, #156
	movs r2, #128
	movs r3, #236
	lsls r1, r1, #17
	lsls r2, r2, #14
	lsls r3, r3, #17
	bl Object_SetPositionAndResetMotion
	movs r3, #19
	movs r2, #28
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #0
	movs r2, #3
	movs r3, #3
	bl Func_02005178
	b .L_0200ab2c
.L_0200aaf4:
	movs r0, #129
	lsls r0, r0, #2
	adds r0, #255
	bl Func_02005110
	cmp r0, #0
	beq .L_0200ab2c
	movs r0, #8
	bl Object_GetById
	movs r1, #172
	movs r2, #128
	movs r3, #236
	lsls r1, r1, #17
	lsls r2, r2, #14
	lsls r3, r3, #17
	bl Object_SetPositionAndResetMotion
	movs r3, #19
	movs r2, #28
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #4
	movs r2, #3
	movs r3, #3
	bl Func_02005178
.L_0200ab2c:
	ldr r0, .L_0200ac14
	bl Func_020053b8
	movs r0, #10
	bl Object_GetById
	movs r3, #179
	adds r6, r0, #0
	lsls r3, r3, #8
	adds r3, #51
	adds r2, r6, #0
	str r3, [r6, #24]
	str r3, [r6, #28]
	adds r2, #89
	movs r3, #0
	movs r0, #0
	strb r3, [r2]
	bl Func_02005338
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #45
	bl Func_02005110
	cmp r0, #0
	beq .L_0200ab92
	movs r3, #1
	movs r2, #4
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #0
	movs r0, #41
	movs r1, #0
	movs r2, #20
	bl Func_02005170
	movs r0, #11
	movs r1, #0
	movs r2, #0
	bl Func_02005230
	movs r0, #12
	movs r1, #0
	movs r2, #0
	bl Func_02005230
	movs r0, #13
	movs r1, #0
	movs r2, #0
	bl Func_02005230
.L_0200ab92:
	movs r0, #10
	adds r0, #255
	bl Func_02005110
	cmp r0, #0
	bne .L_0200abf6
	ldr r2, .L_0200ac18
	movs r1, #241
	lsls r1, r1, #1
	adds r3, r2, r1
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #2
	bne .L_0200abc2
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r2, r1
	ldr r0, [r3]
	bl Object_GetById
	movs r3, #128
	lsls r3, r3, #15
	str r3, [r0, #12]
	b .L_0200abf6
.L_0200abc2:
	cmp r3, #3
	bne .L_0200abf6
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #45
	bl Func_02005110
	cmp r0, #0
	bne .L_0200abf6
	movs r0, #11
	movs r1, #0
	movs r2, #0
	bl Func_02005230
	movs r0, #12
	movs r1, #0
	movs r2, #0
	bl Func_02005230
	movs r0, #13
	movs r1, #0
	movs r2, #0
	bl Func_02005230
	bl Func_02005028
.L_0200abf6:
	ldr r3, .L_0200ac18
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #2
	bne .L_0200ac0e
	movs r0, #48
	adds r0, #255
	bl Func_02005120
.L_0200ac0e:
	add sp, #8
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200ac14:
	.4byte Data_020054e0
.L_0200ac18:
	.4byte gPartyState
	.section .text.x0200ac1c,"ax",%progbits
	.global Func_02002c1c
	.thumb_func
Func_02002c1c:
	push {lr}
	movs r0, #10
	adds r0, #255
	bl Func_02005110
	cmp r0, #0
	beq .L_0200acd2
	movs r0, #9
	bl Object_GetById
	movs r1, #216
	movs r3, #216
	lsls r3, r3, #16
	lsls r1, r1, #16
	movs r2, #0
	bl Object_SetPositionAndResetMotion
	movs r0, #11
	movs r1, #0
	movs r2, #0
	bl Func_02005230
	movs r0, #13
	movs r1, #0
	movs r2, #0
	bl Func_02005230
	movs r0, #14
	movs r1, #0
	movs r2, #0
	bl Func_02005230
	movs r0, #15
	movs r1, #0
	movs r2, #0
	bl Func_02005230
	movs r1, #0
	movs r2, #0
	movs r0, #16
	bl Func_02005230
	movs r0, #10
	bl Object_GetById
	movs r1, #188
	movs r3, #248
	lsls r3, r3, #16
	lsls r1, r1, #17
	movs r2, #0
	bl Object_SetPositionAndResetMotion
	movs r0, #12
	movs r1, #0
	movs r2, #0
	bl Func_02005230
	movs r0, #17
	movs r1, #0
	movs r2, #0
	bl Func_02005230
	movs r0, #18
	movs r1, #0
	movs r2, #0
	bl Func_02005230
	movs r0, #19
	movs r1, #0
	movs r2, #0
	bl Func_02005230
	movs r1, #0
	movs r2, #0
	movs r0, #20
	bl Func_02005230
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #14
	bl Func_02005120
	movs r0, #131
	lsls r0, r0, #2
	bl Func_02005120
	movs r0, #135
	lsls r0, r0, #1
	adds r0, #255
	bl Func_02005120
.L_0200acd2:
	movs r0, #192
	lsls r0, r0, #2
	bl Func_02005110
	cmp r0, #0
	beq .L_0200acee
	movs r1, #216
	movs r2, #216
	movs r0, #8
	lsls r1, r1, #16
	lsls r2, r2, #16
	bl Func_02005230
	b .L_0200ad0a
.L_0200acee:
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #1
	bl Func_02005110
	cmp r0, #0
	beq .L_0200ad0a
	movs r1, #188
	movs r2, #248
	movs r0, #8
	lsls r1, r1, #17
	lsls r2, r2, #16
	bl Func_02005230
.L_0200ad0a:
	ldr r3, .L_0200ad48
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	ldr r3, .L_0200ad4c
	subs r2, #2
	strh r3, [r2]
	ldr r3, .L_0200ad50
	movs r2, #0
	str r2, [r3]
	ldr r3, .L_0200ad54
	movs r1, #144
	str r2, [r3]
	lsls r1, r1, #3
	ldr r0, .L_0200ad58
	bl Func_020050d0
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_0200ad5c
	bl Func_020050d0
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_0200ad60
	bl Func_020050d0
	movs r0, #9
	b .L_0200ad64
	.2byte 0x0000
.L_0200ad48:
	.4byte 0x00000c08
.L_0200ad4c:
	.4byte 0x00003f10
.L_0200ad50:
	.4byte gOverlayArea + 0x6298
.L_0200ad54:
	.4byte gOverlayArea + 0x629c
.L_0200ad58:
	.4byte Func_02000bf4
.L_0200ad5c:
	.4byte Func_02000c34
.L_0200ad60:
	.4byte Func_02001cc4
.L_0200ad64:
	bl Func_02000b78
	movs r0, #10
	bl Func_02000b78
	movs r0, #11
	bl Func_02000bac
	movs r0, #13
	bl Func_02000bac
	movs r0, #14
	bl Func_02000bac
	movs r0, #15
	bl Func_02000bac
	movs r0, #16
	bl Func_02000bac
	movs r0, #12
	bl Func_02000bac
	movs r0, #17
	bl Func_02000bac
	movs r0, #18
	bl Func_02000bac
	movs r0, #19
	bl Func_02000bac
	movs r0, #20
	bl Func_02000bac
	ldr r0, .L_0200ae48
	bl Func_020053b8
	movs r0, #10
	adds r0, #255
	bl Func_02005110
	cmp r0, #0
	bne .L_0200adda
	ldr r3, .L_0200ae4c
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #3
	bne .L_0200add2
	bl Func_0200306c
	b .L_0200adda
.L_0200add2:
	cmp r3, #4
	bne .L_0200adda
	bl Func_020032c8
.L_0200adda:
	ldr r3, .L_0200ae50
	movs r2, #0
	ldr r1, .L_0200ae54
	str r2, [r3]
	movs r3, #250
	lsls r3, r3, #4
	str r3, [r1]
	ldr r3, .L_0200ae58
	movs r1, #144
	strh r2, [r3]
	ldr r3, .L_0200ae5c
	lsls r1, r1, #3
	strh r2, [r3]
	ldr r0, .L_0200ae60
	bl Func_020050d0
	movs r1, #144
	ldr r0, .L_0200ae64
	lsls r1, r1, #3
	bl Func_020050d0
	movs r0, #10
	adds r0, #255
	bl Func_02005110
	cmp r0, #0
	bne .L_0200ae24
	ldr r3, .L_0200ae4c
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #5
	bne .L_0200ae24
	bl Func_02002ec4
.L_0200ae24:
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_0200ae68
	bl Func_020050d0
	ldr r3, .L_0200ae4c
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	movs r1, #2
	bl Func_020052b8
	movs r0, #0
	bl Func_02005338
	pop {pc}
	.2byte 0x0000
.L_0200ae48:
	.4byte Data_020054e0
.L_0200ae4c:
	.4byte gPartyState
.L_0200ae50:
	.4byte gOverlayArea + 0x62a0
.L_0200ae54:
	.4byte gOverlayArea + 0x62a4
.L_0200ae58:
	.4byte gOverlayArea + 0x62a8
.L_0200ae5c:
	.4byte gOverlayArea + 0x62aa
.L_0200ae60:
	.4byte Func_02000f60
.L_0200ae64:
	.4byte Func_020012c4
.L_0200ae68:
	.4byte Func_020019d4
	.section .text.x0200ae6c,"ax",%progbits
	.global Func_02002e6c
	.thumb_func
Func_02002e6c:
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
	bge .L_0200ae9c
	adds r3, #15
.L_0200ae9c:
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
	.section .text.x0200aec4,"ax",%progbits
	.global Func_02002ec4
	.thumb_func
Func_02002ec4:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, .L_0200b058
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	sub sp, #68
	bl Object_GetById
	adds r7, r0, #0
	bl Func_020051c0
	movs r0, #0
	bl Func_02005370
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r1, r1
	negs r2, r2
	movs r3, #0
	negs r0, r0
	bl Func_020052e0
	bl Func_02005158
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
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	movs r0, #204
	bl Func_020053d8
	movs r3, #3
	strb r3, [r5]
	movs r0, #24
	bl Battle_WaitMode0
	add r2, sp, #28
	movs r3, #7
	str r3, [r2, #4]
	ldr r3, .L_0200b05c
	mov r8, r2
	str r3, [r2, #36]
	movs r3, #204
	lsls r3, r3, #8
	adds r3, #204
	str r3, [r2, #8]
	str r3, [r2, #12]
	movs r3, #0
	mov r10, r3
.L_0200af5e:
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
	ldr r3, .L_0200b060
	adds r2, r2, r3
	str r2, [r6]
	bl Random16Far
	lsls r3, r0, #1
	ldr r5, [r6, #8]
	adds r3, r3, r0
	ldr r4, .L_0200b064
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
	ldr r4, .L_0200b068
	str r5, [sp, #4]
	str r4, [sp, #8]
	mov r4, r8
	str r4, [sp, #12]
	bl Func_020000b8
	movs r2, #1
	add r10, r2
	mov r3, r10
	cmp r3, #16
	bls .L_0200af5e
	movs r0, #188
	bl Func_020053d8
	ldr r5, .L_0200b058
	movs r4, #133
	lsls r4, r4, #2
	adds r5, r5, r4
	movs r1, #2
	ldr r0, [r5]
	adds r1, #255
	bl Func_020052d0
	ldr r0, [r5]
	movs r1, #49
	bl Object_SetModeById
	movs r0, #160
	movs r1, #160
	movs r2, #128
	lsls r0, r0, #11
	lsls r1, r1, #11
	lsls r2, r2, #9
	bl Func_02005198
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	adds r2, #102
	negs r0, r0
	negs r1, r1
	bl Func_02005198
	bl Func_020051a0
	movs r1, #128
	ldr r0, [r5]
	lsls r1, r1, #1
	bl Func_020052d0
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r7, #72]
	movs r3, #128
	lsls r3, r3, #7
	str r3, [r7, #68]
	movs r0, #10
	bl Battle_WaitMode0
	ldr r0, [r5]
	movs r1, #1
	bl Object_SetModeById
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #88
	str r2, [r3]
	bl Func_020051c8
	add sp, #68
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_0200b058:
	.4byte gPartyState
.L_0200b05c:
	.4byte Func_02002e6c
.L_0200b060:
	.4byte 0xffffa000
.L_0200b064:
	.4byte 0xffffd000
.L_0200b068:
	.4byte 0x01090001
	.section .text.x0200b06c,"ax",%progbits
	.global Func_0200306c
	.thumb_func
Func_0200306c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, .L_0200b2b8
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	ldr r0, [r3]
	bl Object_GetById
	adds r6, r0, #0
	movs r0, #9
	bl Object_GetById
	mov r8, r0
	bl Func_020052f0
	adds r7, r0, #0
	bl Func_020051c0
	movs r0, #0
	bl Func_02005370
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r1, r1
	negs r2, r2
	movs r3, #0
	negs r0, r0
	bl Func_020052e0
	bl Func_020052f0
	adds r2, r6, #0
	movs r3, #0
	adds r2, #85
	adds r0, #85
	strb r3, [r0]
	strb r3, [r2]
	movs r0, #1
	bl WaitFrames
	movs r0, #130
	lsls r0, r0, #2
	bl Func_02005118
	movs r0, #133
	lsls r0, r0, #1
	adds r0, #255
	bl Func_02005120
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #1
	bl Func_02005118
	movs r0, #192
	lsls r0, r0, #2
	bl Func_02005120
	movs r0, #162
	bl Func_020053d8
	bl Func_02000d64
	movs r2, #0
	mov r10, r2
.L_0200b0f6:
	ldr r3, [r6, #12]
	movs r2, #144
	lsls r2, r2, #11
	adds r3, r3, r2
	str r3, [r6, #12]
	mov r1, r8
	ldr r3, [r1, #12]
	movs r5, #128
	adds r3, r3, r2
	str r3, [r1, #12]
	ldr r3, [r7, #12]
	movs r2, #230
	lsls r2, r2, #9
	adds r2, #204
	adds r3, r3, r2
	lsls r5, r5, #5
	movs r1, #128
	str r3, [r7, #12]
	movs r0, #11
	lsls r1, r1, #10
	adds r2, r5, #0
	bl Func_02000cd8
	movs r1, #192
	movs r0, #13
	lsls r1, r1, #9
	adds r2, r5, #0
	bl Func_02000cd8
	movs r1, #128
	movs r0, #14
	lsls r1, r1, #9
	adds r2, r5, #0
	bl Func_02000cd8
	movs r1, #128
	movs r0, #15
	lsls r1, r1, #8
	adds r2, r5, #0
	bl Func_02000cd8
	movs r1, #0
	adds r2, r5, #0
	movs r0, #16
	bl Func_02000cd8
	mov r1, r10
	movs r2, #128
	lsls r3, r1, #16
	lsls r2, r2, #9
	adds r3, r3, r2
	movs r2, #240
	asrs r1, r3, #16
	lsls r2, r2, #12
	mov r10, r1
	cmp r3, r2
	bls .L_0200b0f6
	bl Func_02005158
	movs r0, #1
	bl WaitFrames
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
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	movs r0, #10
	bl Battle_WaitMode0
.L_0200b194:
	ldr r3, [r6, #12]
	ldr r2, .L_0200b2bc
	mov r1, r8
	adds r3, r3, r2
	str r3, [r6, #12]
	movs r5, #192
	ldr r3, [r1, #12]
	lsls r5, r5, #5
	adds r3, r3, r2
	str r3, [r1, #12]
	ldr r3, [r7, #12]
	ldr r2, .L_0200b2c0
	movs r1, #192
	adds r3, r3, r2
	str r3, [r7, #12]
	movs r0, #11
	lsls r1, r1, #10
	adds r2, r5, #0
	bl Func_02000cf8
	movs r1, #144
	movs r0, #13
	lsls r1, r1, #10
	adds r2, r5, #0
	bl Func_02000cf8
	movs r1, #192
	movs r0, #14
	lsls r1, r1, #9
	adds r2, r5, #0
	bl Func_02000cf8
	movs r1, #192
	movs r0, #15
	lsls r1, r1, #8
	adds r2, r5, #0
	bl Func_02000cf8
	movs r1, #0
	adds r2, r5, #0
	movs r0, #16
	bl Func_02000cf8
	movs r0, #1
	bl WaitFrames
	movs r0, #16
	bl Object_GetById
	ldr r3, [r0, #28]
	cmp r3, r5
	bgt .L_0200b194
	ldr r2, .L_0200b2c4
	movs r3, #0
	movs r0, #131
	str r3, [r2]
	lsls r0, r0, #2
	bl Func_02005120
	movs r0, #195
	lsls r0, r0, #1
	bl Func_020053d8
	movs r0, #11
	movs r1, #0
	movs r2, #0
	bl Func_02005230
	movs r0, #13
	movs r1, #0
	movs r2, #0
	bl Func_02005230
	movs r0, #14
	movs r1, #0
	movs r2, #0
	bl Func_02005230
	movs r0, #15
	movs r1, #0
	movs r2, #0
	bl Func_02005230
	movs r2, #0
	movs r0, #16
	movs r1, #0
	bl Func_02005230
	movs r1, #1
	movs r0, #9
	bl Object_SetModeById
	movs r0, #9
	bl Object_GetById
	movs r1, #216
	movs r3, #216
	lsls r1, r1, #16
	movs r2, #0
	lsls r3, r3, #16
	bl Object_SetPositionAndResetMotion
	mov r1, r8
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #2
	orrs r3, r2
	strb r3, [r1]
	movs r0, #9
	movs r1, #3
	bl Func_020052b8
	movs r2, #0
	ldr r1, [r6, #8]
	ldr r3, [r6, #16]
	adds r0, r6, #0
	bl Object_SetPositionAndResetMotion
	ldr r3, .L_0200b2b8
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	ldr r0, [r3]
	bl Object_GetById
	movs r1, #1
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #1
	bl WaitFrames
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #88
	str r2, [r3]
	bl Func_020051c8
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200b2b8:
	.4byte gPartyState
.L_0200b2bc:
	.4byte 0xfff94000
.L_0200b2c0:
	.4byte 0xfffd4ccd
.L_0200b2c4:
	.4byte gOverlayArea + 0x6298
	.section .text.x0200b2c8,"ax",%progbits
	.global Func_020032c8
	.thumb_func
Func_020032c8:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, .L_0200b514
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	ldr r0, [r3]
	bl Object_GetById
	adds r6, r0, #0
	movs r0, #10
	bl Object_GetById
	mov r8, r0
	bl Func_020052f0
	adds r7, r0, #0
	bl Func_020051c0
	movs r0, #0
	bl Func_02005370
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r1, r1
	negs r2, r2
	movs r3, #0
	negs r0, r0
	bl Func_020052e0
	bl Func_020052f0
	adds r2, r6, #0
	movs r3, #0
	adds r2, #85
	adds r0, #85
	strb r3, [r0]
	strb r3, [r2]
	movs r0, #1
	bl WaitFrames
	movs r0, #130
	lsls r0, r0, #2
	bl Func_02005120
	movs r0, #133
	lsls r0, r0, #1
	adds r0, #255
	bl Func_02005118
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #1
	bl Func_02005120
	movs r0, #192
	lsls r0, r0, #2
	bl Func_02005118
	movs r0, #162
	bl Func_020053d8
	bl Func_02000e60
	movs r2, #0
	mov r10, r2
.L_0200b352:
	ldr r3, [r6, #12]
	movs r2, #144
	lsls r2, r2, #11
	adds r3, r3, r2
	str r3, [r6, #12]
	mov r1, r8
	ldr r3, [r1, #12]
	movs r5, #128
	adds r3, r3, r2
	str r3, [r1, #12]
	ldr r3, [r7, #12]
	movs r2, #230
	lsls r2, r2, #9
	adds r2, #204
	adds r3, r3, r2
	lsls r5, r5, #5
	movs r1, #128
	str r3, [r7, #12]
	movs r0, #12
	lsls r1, r1, #10
	adds r2, r5, #0
	bl Func_02000cd8
	movs r1, #192
	movs r0, #17
	lsls r1, r1, #9
	adds r2, r5, #0
	bl Func_02000cd8
	movs r1, #128
	movs r0, #18
	lsls r1, r1, #9
	adds r2, r5, #0
	bl Func_02000cd8
	movs r1, #128
	movs r0, #19
	lsls r1, r1, #8
	adds r2, r5, #0
	bl Func_02000cd8
	movs r1, #0
	adds r2, r5, #0
	movs r0, #20
	bl Func_02000cd8
	mov r1, r10
	movs r2, #128
	lsls r3, r1, #16
	lsls r2, r2, #9
	adds r3, r3, r2
	movs r2, #240
	asrs r1, r3, #16
	lsls r2, r2, #12
	mov r10, r1
	cmp r3, r2
	bls .L_0200b352
	bl Func_02005158
	movs r0, #1
	bl WaitFrames
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
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	movs r0, #10
	bl Battle_WaitMode0
.L_0200b3f0:
	ldr r3, [r6, #12]
	ldr r2, .L_0200b518
	mov r1, r8
	adds r3, r3, r2
	str r3, [r6, #12]
	movs r5, #192
	ldr r3, [r1, #12]
	lsls r5, r5, #5
	adds r3, r3, r2
	str r3, [r1, #12]
	ldr r3, [r7, #12]
	ldr r2, .L_0200b51c
	movs r1, #192
	adds r3, r3, r2
	str r3, [r7, #12]
	movs r0, #12
	lsls r1, r1, #10
	adds r2, r5, #0
	bl Func_02000cf8
	movs r1, #144
	movs r0, #17
	lsls r1, r1, #10
	adds r2, r5, #0
	bl Func_02000cf8
	movs r1, #192
	movs r0, #18
	lsls r1, r1, #9
	adds r2, r5, #0
	bl Func_02000cf8
	movs r1, #192
	movs r0, #19
	lsls r1, r1, #8
	adds r2, r5, #0
	bl Func_02000cf8
	movs r1, #0
	adds r2, r5, #0
	movs r0, #20
	bl Func_02000cf8
	movs r0, #1
	bl WaitFrames
	movs r0, #20
	bl Object_GetById
	ldr r3, [r0, #28]
	cmp r3, r5
	bgt .L_0200b3f0
	ldr r2, .L_0200b520
	movs r0, #135
	movs r3, #0
	lsls r0, r0, #1
	str r3, [r2]
	adds r0, #255
	bl Func_02005120
	movs r0, #195
	lsls r0, r0, #1
	bl Func_020053d8
	movs r0, #12
	movs r1, #0
	movs r2, #0
	bl Func_02005230
	movs r0, #17
	movs r1, #0
	movs r2, #0
	bl Func_02005230
	movs r0, #18
	movs r1, #0
	movs r2, #0
	bl Func_02005230
	movs r0, #19
	movs r1, #0
	movs r2, #0
	bl Func_02005230
	movs r2, #0
	movs r0, #20
	movs r1, #0
	bl Func_02005230
	movs r1, #1
	movs r0, #10
	bl Object_SetModeById
	movs r0, #10
	bl Object_GetById
	movs r1, #188
	movs r3, #248
	lsls r1, r1, #17
	movs r2, #0
	lsls r3, r3, #16
	bl Object_SetPositionAndResetMotion
	mov r1, r8
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #2
	orrs r3, r2
	strb r3, [r1]
	movs r0, #10
	movs r1, #3
	bl Func_020052b8
	movs r2, #0
	ldr r1, [r6, #8]
	ldr r3, [r6, #16]
	adds r0, r6, #0
	bl Object_SetPositionAndResetMotion
	ldr r3, .L_0200b514
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	ldr r0, [r3]
	bl Object_GetById
	movs r1, #1
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #1
	bl WaitFrames
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #88
	str r2, [r3]
	bl Func_020051c8
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_0200b514:
	.4byte gPartyState
.L_0200b518:
	.4byte 0xfff94000
.L_0200b51c:
	.4byte 0xfffd4ccd
.L_0200b520:
	.4byte gOverlayArea + 0x629c
	.section .text.x0200b524,"ax",%progbits
	.global Func_02003524
	.thumb_func
Func_02003524:
	push {lr}
	bl Func_020051c0
	movs r0, #0
	bl Func_02005370
	ldr r3, .L_0200b560
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	movs r3, #0
	adds r0, #85
	strb r3, [r0]
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
	pop {pc}
	.2byte 0x0000
.L_0200b560:
	.4byte gPartyState
	.section .text.x0200b564,"ax",%progbits
	.global Func_02003564
	.thumb_func
Func_02003564:
	push {lr}
	bl Func_02003524
	movs r0, #3
	bl Func_020052f8
	pop {pc}
	.2byte 0x0000
	.section .text.x0200b574,"ax",%progbits
	.global Func_02003574
	.thumb_func
Func_02003574:
	push {lr}
	bl Func_02003524
	movs r0, #4
	bl Func_020052f8
	pop {pc}
	.2byte 0x0000
	.section .text.x0200b584,"ax",%progbits
	.global Func_02003584
	.thumb_func
Func_02003584:
	push {r5, r6, r7, lr}
	ldr r5, .L_0200b678
	movs r0, #133
	lsls r0, r0, #2
	adds r5, r5, r0
	ldr r0, [r5]
	bl Object_GetById
	adds r7, r0, #0
	bl Func_020051c0
	movs r0, #0
	bl Func_02005370
	adds r3, r7, #0
	adds r3, #85
	movs r6, #0
	strb r6, [r3]
	movs r1, #1
	ldr r0, [r5]
	bl Func_020052b8
	ldr r0, [r5]
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	strh r6, [r7, #6]
	movs r1, #16
	ldr r0, [r5]
	bl Object_SetModeById
	movs r5, #0
.L_0200b5c8:
	ldr r3, [r7, #16]
	movs r0, #192
	lsls r0, r0, #10
	adds r3, r3, r0
	ldr r1, [r7, #8]
	ldr r2, [r7, #12]
	adds r0, r7, #0
	bl Object_SetPositionAndResetMotion
	adds r5, #1
	movs r0, #1
	bl WaitFrames
	cmp r5, #7
	bls .L_0200b5c8
	movs r0, #20
	bl Battle_WaitMode0
	movs r3, #128
	lsls r3, r3, #7
	ldr r5, .L_0200b678
	strh r3, [r7, #6]
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
	movs r1, #129
	ldr r0, [r5]
	lsls r1, r1, #1
	bl Func_020052d0
	ldr r0, [r5]
	movs r1, #27
	bl Object_SetModeById
	movs r0, #30
	bl Battle_WaitMode0
	ldr r0, [r5]
	movs r1, #28
	bl Object_SetModeById
	movs r0, #10
	bl Battle_WaitMode0
	ldr r0, [r5]
	movs r1, #3
	bl Func_020052b8
	movs r5, #0
.L_0200b62a:
	ldr r3, [r7, #24]
	ldr r2, .L_0200b67c
	ldr r0, .L_0200b680
	adds r3, r3, r2
	str r3, [r7, #24]
	ldr r3, [r7, #28]
	adds r3, r3, r2
	str r3, [r7, #28]
	ldr r3, [r7, #12]
	adds r3, r3, r0
	str r3, [r7, #12]
	movs r0, #1
	bl WaitFrames
	cmp r5, #10
	bne .L_0200b650
	movs r0, #204
	bl Func_020053d8
.L_0200b650:
	cmp r5, #20
	bne .L_0200b668
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #93
	str r2, [r3]
	bl Event_ClearStatus1c6
.L_0200b668:
	adds r5, #1
	cmp r5, #39
	bls .L_0200b62a
	movs r0, #5
	bl Func_020052f8
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200b678:
	.4byte gPartyState
.L_0200b67c:
	.4byte 0xfffffc00
.L_0200b680:
	.4byte 0xfffe0000
	.section .text.x0200b684,"ax",%progbits
	.global Func_02003684
	.thumb_func
Func_02003684:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r5, .L_0200b7d4
	movs r1, #133
	lsls r1, r1, #2
	adds r5, r5, r1
	ldr r0, [r5]
	sub sp, #8
	bl Object_GetById
	mov r8, r0
	movs r0, #13
	bl Object_GetById
	adds r7, r0, #0
	movs r0, #15
	bl Object_GetById
	adds r6, r0, #0
	bl Func_020051c0
	movs r0, #0
	bl Func_02005370
	movs r0, #162
	bl Func_02005388
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r1, r1
	negs r2, r2
	movs r3, #0
	negs r0, r0
	bl Func_020052e0
	movs r0, #1
	bl WaitFrames
	mov r2, r8
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	ldr r0, [r5]
	bl Object_GetById
	movs r1, #220
	movs r3, #200
	ldr r2, .L_0200b7d8
	lsls r3, r3, #16
	lsls r1, r1, #17
	bl Object_SetPositionAndResetMotion
	ldr r0, [r5]
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #13
	bl Object_GetById
	movs r1, #220
	movs r3, #204
	lsls r1, r1, #17
	ldr r2, .L_0200b7d8
	lsls r3, r3, #16
	bl Object_SetPositionAndResetMotion
	movs r0, #15
	bl Object_GetById
	movs r1, #220
	movs r3, #200
	ldr r2, .L_0200b7d8
	lsls r3, r3, #16
	lsls r1, r1, #17
	bl Object_SetPositionAndResetMotion
	movs r0, #13
	movs r1, #2
	bl Func_020052b8
	movs r0, #15
	movs r1, #3
	bl Func_020052b8
	adds r1, r6, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #2
	orrs r3, r2
	strb r3, [r1]
	movs r3, #128
	lsls r3, r3, #3
	str r3, [r6, #28]
	movs r0, #13
	movs r1, #4
	bl Object_SetModeById
	movs r0, #15
	movs r1, #2
	bl Object_SetModeById
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #93
	str r2, [r3]
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
.L_0200b76e:
	ldr r3, [r7, #12]
	movs r2, #128
	lsls r2, r2, #11
	adds r3, r3, r2
	str r3, [r7, #12]
	mov r1, r8
	ldr r3, [r1, #12]
	movs r0, #15
	adds r3, r3, r2
	movs r2, #152
	lsls r2, r2, #7
	adds r2, #204
	str r3, [r1, #12]
	adds r1, r2, #0
	bl Func_02000cd8
	movs r0, #1
	bl WaitFrames
	ldr r3, [r7, #12]
	cmp r3, #0
	blt .L_0200b76e
	movs r3, #27
	movs r2, #12
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #0
	movs r2, #1
	movs r3, #1
	bl Func_02005178
	ldr r2, .L_0200b7dc
	movs r3, #250
	lsls r3, r3, #3
	str r3, [r2]
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #88
	str r2, [r3]
	bl Func_020051c8
	add sp, #8
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200b7d4:
	.4byte gPartyState
.L_0200b7d8:
	.4byte 0xffec0000
.L_0200b7dc:
	.4byte gOverlayArea + 0x62a0
	.section .text.x0200b7e0,"ax",%progbits
	.global Func_020037e0
	.thumb_func
Func_020037e0:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r5, .L_0200b930
	movs r1, #133
	lsls r1, r1, #2
	adds r5, r5, r1
	ldr r0, [r5]
	sub sp, #8
	bl Object_GetById
	mov r8, r0
	movs r0, #14
	bl Object_GetById
	adds r7, r0, #0
	movs r0, #16
	bl Object_GetById
	adds r6, r0, #0
	bl Func_020051c0
	movs r0, #0
	bl Func_02005370
	movs r0, #162
	bl Func_02005388
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r1, r1
	negs r2, r2
	movs r3, #0
	negs r0, r0
	bl Func_020052e0
	movs r0, #1
	bl WaitFrames
	mov r2, r8
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	ldr r0, [r5]
	bl Object_GetById
	movs r1, #150
	movs r3, #232
	ldr r2, .L_0200b934
	lsls r3, r3, #16
	lsls r1, r1, #18
	bl Object_SetPositionAndResetMotion
	ldr r0, [r5]
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #14
	bl Object_GetById
	movs r1, #150
	movs r3, #236
	lsls r1, r1, #18
	ldr r2, .L_0200b934
	lsls r3, r3, #16
	bl Object_SetPositionAndResetMotion
	movs r0, #16
	bl Object_GetById
	movs r1, #150
	movs r3, #232
	ldr r2, .L_0200b934
	lsls r3, r3, #16
	lsls r1, r1, #18
	bl Object_SetPositionAndResetMotion
	movs r0, #14
	movs r1, #2
	bl Func_020052b8
	movs r0, #16
	movs r1, #3
	bl Func_020052b8
	adds r1, r6, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #2
	orrs r3, r2
	strb r3, [r1]
	movs r3, #128
	lsls r3, r3, #3
	str r3, [r6, #28]
	movs r0, #14
	movs r1, #4
	bl Object_SetModeById
	movs r0, #16
	movs r1, #2
	bl Object_SetModeById
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #93
	str r2, [r3]
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
.L_0200b8ca:
	ldr r3, [r7, #12]
	movs r2, #128
	lsls r2, r2, #11
	adds r3, r3, r2
	str r3, [r7, #12]
	mov r1, r8
	ldr r3, [r1, #12]
	movs r0, #16
	adds r3, r3, r2
	movs r2, #152
	lsls r2, r2, #7
	adds r2, #204
	str r3, [r1, #12]
	adds r1, r2, #0
	bl Func_02000cd8
	movs r0, #1
	bl WaitFrames
	ldr r3, [r7, #12]
	cmp r3, #0
	blt .L_0200b8ca
	movs r3, #37
	movs r2, #14
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #0
	movs r2, #1
	movs r3, #1
	bl Func_02005178
	ldr r2, .L_0200b938
	movs r3, #250
	lsls r3, r3, #3
	str r3, [r2]
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #88
	str r2, [r3]
	bl Func_020051c8
	add sp, #8
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200b930:
	.4byte gPartyState
.L_0200b934:
	.4byte 0xffec0000
.L_0200b938:
	.4byte gOverlayArea + 0x62a4
	.section .text.x0200b93c,"ax",%progbits
	.global Func_0200393c
	.thumb_func
Func_0200393c:
	push {r5, lr}
	movs r0, #13
	bl Object_GetById
	adds r5, r0, #0
	bl Func_020051c0
	movs r0, #0
	bl Func_02005370
.L_0200b950:
	ldr r3, [r5, #12]
	ldr r2, .L_0200b9a0
	movs r0, #15
	adds r3, r3, r2
	movs r2, #152
	lsls r2, r2, #7
	adds r2, #204
	str r3, [r5, #12]
	adds r1, r2, #0
	bl Func_02000cf8
	movs r0, #1
	bl WaitFrames
	ldr r3, [r5, #12]
	ldr r2, .L_0200b9a4
	cmp r3, r2
	bgt .L_0200b950
	movs r0, #15
	movs r1, #0
	movs r2, #0
	bl Func_02005230
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
	movs r0, #6
	bl Func_020052f8
	pop {r5, pc}
	.2byte 0x0000
.L_0200b9a0:
	.4byte 0xfffc0000
.L_0200b9a4:
	.4byte 0xffec0000
	.section .text.x0200b9a8,"ax",%progbits
	.global Func_020039a8
	.thumb_func
Func_020039a8:
	push {r5, lr}
	movs r0, #14
	bl Object_GetById
	adds r5, r0, #0
	bl Func_020051c0
	movs r0, #0
	bl Func_02005370
.L_0200b9bc:
	ldr r3, [r5, #12]
	ldr r2, .L_0200ba0c
	movs r0, #16
	adds r3, r3, r2
	movs r2, #152
	lsls r2, r2, #7
	adds r2, #204
	str r3, [r5, #12]
	adds r1, r2, #0
	bl Func_02000cf8
	movs r0, #1
	bl WaitFrames
	ldr r3, [r5, #12]
	ldr r2, .L_0200ba10
	cmp r3, r2
	bgt .L_0200b9bc
	movs r0, #16
	movs r1, #0
	movs r2, #0
	bl Func_02005230
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
	movs r0, #7
	bl Func_020052f8
	pop {r5, pc}
	.2byte 0x0000
.L_0200ba0c:
	.4byte 0xfffc0000
.L_0200ba10:
	.4byte 0xffec0000
	.section .text.x0200ba14,"ax",%progbits
	.global Func_02003a14
	.thumb_func
Func_02003a14:
	push {lr}
	ldr r2, .L_0200ba54
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #32]
	movs r3, #0
	str r3, [r2]
	subs r3, #13
	ldrb r2, [r1, #23]
	movs r0, #0
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	strb r3, [r1, #23]
	bl Func_02005338
	ldr r3, .L_0200ba58
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	ldrh r3, [r3]
	movs r2, #128
	subs r3, #1
	lsls r3, r3, #16
	lsls r2, r2, #9
	cmp r3, r2
	bhi .L_0200ba52
	movs r0, #48
	adds r0, #255
	bl Func_02005120
.L_0200ba52:
	pop {pc}
.L_0200ba54:
	.4byte gOverlayArea + 0x6294
.L_0200ba58:
	.4byte gPartyState
	.section .text.x0200ba5c,"ax",%progbits
	.global Func_02003a5c
	.thumb_func
Func_02003a5c:
	push {r5, lr}
	movs r3, #192
	ldr r2, .L_0200ba88
	lsls r3, r3, #18
	ldr r5, [r3, #32]
	movs r3, #0
	str r3, [r2]
	movs r0, #170
	bl Func_02005388
	ldrb r2, [r5, #23]
	movs r3, #13
	negs r3, r3
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	strb r3, [r5, #23]
	movs r0, #0
	bl Func_02005338
	pop {r5, pc}
	.2byte 0x0000
.L_0200ba88:
	.4byte gOverlayArea + 0x6294
	.section .text.x0200ba8c,"ax",%progbits
	.global Func_02003a8c
	.thumb_func
Func_02003a8c:
	push {lr}
	bl Func_02005390
	movs r1, #136
	lsls r1, r1, #1
	movs r0, #0
	adds r1, #255
	movs r2, #8
	movs r3, #9
	bl Func_020053a8
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #73
	bl Func_02005110
	cmp r0, #0
	beq .L_0200babc
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_02005230
	b .L_0200bad8
.L_0200babc:
	movs r0, #10
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #10
	movs r1, #6
	bl Object_SetModeById
	ldr r1, .L_0200bae0
	movs r0, #10
	bl ObjectMotion_EnableActionAndSetCallback
.L_0200bad8:
	movs r0, #0
	bl Func_02005338
	pop {pc}
.L_0200bae0:
	.4byte Data_02005550
	.section .text.x0200bae4,"ax",%progbits
	.global Func_02003ae4
	.thumb_func
Func_02003ae4:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	mov r8, r0
	adds r6, r1, #0
	bl Object_GetById
	movs r1, #5
	adds r5, r0, #0
	mov r0, r8
	bl Object_SetModeById
	bl Random16Far
	lsls r3, r0, #1
	adds r3, r3, r0
	lsls r3, r3, #3
	adds r3, r3, r0
	lsls r3, r3, #4
	lsrs r3, r3, #16
	adds r6, r6, r3
	adds r3, r5, #0
	adds r3, #100
	strh r6, [r3]
	adds r5, #102
	mov r3, r8
	strh r3, [r5]
	ldr r1, .L_0200bb28
	mov r0, r8
	bl ObjectMotion_EnableActionAndSetCallback
	pop {r3}
	mov r8, r3
	pop {r5, r6, pc}
.L_0200bb28:
	.4byte Data_02005548
	.section .text.x0200bb2c,"ax",%progbits
	.global Func_02003b2c
	.thumb_func
Func_02003b2c:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	movs r0, #128
	adds r3, r3, r2
	lsls r0, r0, #4
	subs r2, #172
	str r2, [r3]
	adds r0, #66
	sub sp, #8
	bl Func_02005110
	cmp r0, #0
	beq .L_0200bb6e
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl Func_02005230
	movs r3, #5
	movs r2, #13
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #0
	movs r2, #1
	movs r3, #1
	bl Func_02005178
	b .L_0200bb74
.L_0200bb6e:
	movs r0, #8
	bl Func_020052c0
.L_0200bb74:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #73
	bl Func_02005110
	cmp r0, #0
	beq .L_0200bbe0
	movs r3, #17
	movs r2, #24
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #3
	movs r0, #2
	movs r1, #0
	movs r2, #2
	bl Func_02005178
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_02005230
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_02005230
	movs r1, #187
	movs r2, #141
	movs r0, #11
	lsls r1, r1, #17
	lsls r2, r2, #17
	movs r3, #0
	bl Func_02005238
	movs r3, #208
	movs r1, #136
	movs r2, #148
	lsls r3, r3, #8
	movs r0, #12
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_02005238
	movs r3, #192
	movs r1, #216
	movs r2, #132
	lsls r3, r3, #6
	movs r0, #13
	lsls r1, r1, #16
	lsls r2, r2, #17
	bl Func_02005238
	b .L_0200bbfa
.L_0200bbe0:
	movs r0, #11
	movs r1, #0
	bl Func_02003ae4
	movs r0, #12
	movs r1, #240
	bl Func_02003ae4
	movs r1, #240
	lsls r1, r1, #1
	movs r0, #13
	bl Func_02003ae4
.L_0200bbfa:
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #18
	bl Func_02005110
	cmp r0, #0
	beq .L_0200bc2e
	movs r3, #2
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #77
	movs r1, #33
	movs r2, #77
	movs r3, #24
	bl Func_02005170
	movs r3, #17
	movs r2, #25
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #32
	movs r1, #30
	movs r2, #2
	movs r3, #1
	bl Func_02005178
.L_0200bc2e:
	ldr r3, .L_0200bc50
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	ldrh r3, [r3]
	movs r2, #128
	subs r3, #3
	lsls r3, r3, #16
	lsls r2, r2, #9
	cmp r3, r2
	bhi .L_0200bc4c
	movs r0, #48
	adds r0, #255
	bl Func_02005120
.L_0200bc4c:
	add sp, #8
	pop {pc}
.L_0200bc50:
	.4byte gPartyState
	.section .text.x0200bc54,"ax",%progbits
	.global Func_02003c54
	.thumb_func
Func_02003c54:
	push {lr}
	movs r0, #132
	lsls r0, r0, #2
	bl Func_02005110
	cmp r0, #0
	bne .L_0200bc78
	movs r0, #132
	lsls r0, r0, #2
	bl Func_02005118
	movs r1, #132
	movs r0, #1
	lsls r1, r1, #2
	movs r2, #10
	movs r3, #11
	bl Func_020053a8
.L_0200bc78:
	movs r3, #192
	movs r1, #196
	movs r2, #180
	lsls r3, r3, #6
	movs r0, #12
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_02005238
	pop {pc}
	.section .text.x0200bc8c,"ax",%progbits
	.global Func_02003c8c
	.thumb_func
Func_02003c8c:
	push {lr}
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
	bl Func_02005390
	movs r1, #136
	lsls r1, r1, #1
	movs r0, #0
	adds r1, #255
	movs r2, #8
	movs r3, #9
	bl Func_020053a8
	movs r1, #132
	movs r0, #1
	lsls r1, r1, #2
	movs r2, #10
	movs r3, #11
	bl Func_020053a8
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #73
	bl Func_02005110
	cmp r0, #0
	bne .L_0200bce2
	movs r0, #13
	movs r1, #5
	bl Object_SetModeById
	movs r0, #15
	movs r1, #5
	bl Object_SetModeById
.L_0200bce2:
	movs r0, #14
	movs r1, #5
	bl Object_SetModeById
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_0200be2c
	bl Func_020050d0
	ldr r3, .L_0200be30
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	ldrh r2, [r3]
	movs r1, #128
	subs r3, r2, #4
	lsls r3, r3, #16
	lsls r1, r1, #10
	cmp r3, r1
	bls .L_0200bd14
	lsls r3, r2, #16
	movs r2, #224
	lsls r2, r2, #12
	cmp r3, r2
	bne .L_0200bd4c
.L_0200bd14:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #74
	bl Func_02005110
	cmp r0, #0
	beq .L_0200bd36
	movs r3, #160
	movs r1, #216
	movs r2, #197
	lsls r3, r3, #7
	movs r0, #12
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_02005238
	b .L_0200bd4c
.L_0200bd36:
	bl Func_02004fb4
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #73
	bl Func_02005110
	cmp r0, #0
	beq .L_0200bd4c
	bl Func_02003c54
.L_0200bd4c:
	ldr r3, .L_0200be30
	movs r1, #241
	lsls r1, r1, #1
	adds r3, r3, r1
	ldrh r2, [r3]
	movs r1, #128
	subs r3, r2, #1
	lsls r3, r3, #16
	lsls r1, r1, #10
	cmp r3, r1
	bls .L_0200bd6a
	lsls r3, r2, #16
	asrs r3, r3, #16
	cmp r3, #9
	bne .L_0200bdd2
.L_0200bd6a:
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #18
	bl Func_02005110
	cmp r0, #0
	beq .L_0200bd7c
	bl Func_02001f3c
.L_0200bd7c:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #73
	bl Func_02005110
	cmp r0, #0
	beq .L_0200bda8
	movs r0, #12
	movs r1, #0
	movs r2, #0
	bl Func_02005230
	movs r3, #176
	movs r1, #204
	movs r2, #166
	lsls r3, r3, #8
	movs r0, #15
	lsls r1, r1, #17
	lsls r2, r2, #18
	bl Func_02005238
	b .L_0200be0a
.L_0200bda8:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #72
	bl Func_02005110
	cmp r0, #0
	beq .L_0200bdcc
	movs r0, #12
	movs r1, #0
	movs r2, #0
	bl Func_02005230
	movs r0, #15
	movs r1, #0
	movs r2, #0
	bl Func_02005230
	b .L_0200be0a
.L_0200bdcc:
	bl Func_02003e8c
	b .L_0200be0a
.L_0200bdd2:
	cmp r3, #6
	bne .L_0200bdea
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #73
	bl Func_02005110
	cmp r0, #0
	bne .L_0200be0a
	bl Func_020044d4
	b .L_0200be0a
.L_0200bdea:
	cmp r3, #13
	bne .L_0200bdf4
	bl Func_020046d8
	b .L_0200be0a
.L_0200bdf4:
	cmp r3, #14
	bne .L_0200be0a
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #73
	bl Func_02005110
	cmp r0, #0
	bne .L_0200be0a
	bl Func_0200482c
.L_0200be0a:
	ldr r3, .L_0200be30
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	ldrh r3, [r3]
	movs r1, #128
	subs r3, #8
	lsls r3, r3, #16
	lsls r1, r1, #9
	cmp r3, r1
	bhi .L_0200be28
	movs r0, #48
	adds r0, #255
	bl Func_02005120
.L_0200be28:
	pop {pc}
	.2byte 0x0000
.L_0200be2c:
	.4byte Func_02001c8c
.L_0200be30:
	.4byte gPartyState
	.section .text.x0200be34,"ax",%progbits
	.global Func_02003e34
	.thumb_func
Func_02003e34:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r6, r1, #0
	movs r5, #0
	adds r7, r0, #0
	mov r8, r2
	cmp r5, r6
	bcs .L_0200be58
.L_0200be46:
	ldrh r3, [r7, #18]
	movs r0, #1
	add r3, r8
	strh r3, [r7, #18]
	adds r5, #1
	bl Battle_WaitMode0
	cmp r5, r6
	bcc .L_0200be46
.L_0200be58:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x0200be60,"ax",%progbits
	.global Func_02003e60
	.thumb_func
Func_02003e60:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r6, r1, #0
	movs r5, #0
	adds r7, r0, #0
	mov r8, r2
	cmp r5, r6
	bcs .L_0200be84
.L_0200be72:
	ldr r3, [r7, #8]
	movs r0, #1
	add r3, r8
	str r3, [r7, #8]
	adds r5, #1
	bl Battle_WaitMode0
	cmp r5, r6
	bcc .L_0200be72
.L_0200be84:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x0200be8c,"ax",%progbits
	.global Func_02003e8c
	.thumb_func
Func_02003e8c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	bl Func_020051c0
	movs r0, #0
	bl Func_02005370
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r1, r1
	negs r2, r2
	movs r3, #0
	negs r0, r0
	bl Func_020052e0
	bl Func_020052f0
	movs r2, #0
	mov r10, r2
	mov r3, r10
	adds r0, #85
	strb r3, [r0]
	movs r0, #1
	bl WaitFrames
	ldr r5, .L_0200c268
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
	movs r1, #200
	movs r2, #199
	lsls r1, r1, #17
	lsls r2, r2, #18
	ldr r0, [r5]
	bl Func_02005230
	movs r0, #1
	bl WaitFrames
	bl Event_SetStatus1c6
	movs r1, #200
	movs r2, #188
	ldr r0, [r5]
	lsls r2, r2, #2
	lsls r1, r1, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #153
	lsls r1, r1, #8
	ldr r0, .L_0200c26c
	adds r1, #153
	bl Func_020052d8
	movs r0, #218
	movs r1, #144
	movs r2, #166
	movs r3, #1
	lsls r0, r0, #17
	lsls r1, r1, #15
	lsls r2, r2, #18
	bl Func_020052e0
	bl Func_020052e8
	movs r2, #0
	movs r1, #4
	movs r0, #12
	bl ObjectMotion_Launch
	ldr r0, .L_0200c270
	bl Func_02005280
	movs r0, #12
	movs r1, #0
	bl Func_02005298
	movs r0, #228
	movs r2, #172
	lsls r2, r2, #18
	movs r3, #1
	lsls r0, r0, #17
	movs r1, #0
	bl Func_020052e0
	bl Func_020052e8
	movs r1, #2
	movs r0, #13
	bl Motion_SetVarCbAndRefresh
	movs r0, #131
	bl Func_020053d8
	movs r1, #128
	lsls r1, r1, #1
	movs r0, #13
	bl Func_02005270
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #0
	movs r0, #13
	bl Func_02005270
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #13
	bl Object_GetById
	movs r3, #85
	adds r7, r0, #0
	adds r3, r3, r7
	mov r2, r10
	ldr r6, [r7, #80]
	strb r2, [r3]
	mov r8, r3
	movs r5, #0
.L_0200bf82:
	ldr r3, [r7, #12]
	movs r2, #192
	lsls r2, r2, #4
	adds r2, #204
	adds r3, r3, r2
	str r3, [r7, #12]
	movs r0, #1
	adds r5, #1
	bl Battle_WaitMode0
	cmp r5, #63
	bls .L_0200bf82
	movs r2, #32
	movs r1, #32
	adds r0, r6, #0
	bl Func_02003e34
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #129
	movs r0, #13
	lsls r1, r1, #1
	bl Func_020052d0
	movs r2, #64
	negs r2, r2
	adds r0, r6, #0
	movs r1, #32
	bl Func_02003e34
	adds r0, r6, #0
	movs r1, #32
	movs r2, #64
	bl Func_02003e34
	movs r2, #32
	adds r0, r6, #0
	movs r1, #64
	bl Func_02003e34
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #13
	bl Func_020052d0
	movs r0, #60
	bl Battle_WaitMode0
	mov r2, r8
	movs r3, #3
	strb r3, [r2]
	movs r0, #5
	bl Battle_WaitMode0
	movs r0, #159
	bl Func_020053d8
	movs r3, #192
	lsls r3, r3, #10
	movs r5, #0
	str r3, [r7, #40]
	movs r0, #60
	strh r5, [r6, #18]
	mov r8, r3
	bl Battle_WaitMode0
	movs r0, #13
	movs r1, #0
	bl Func_02005298
	movs r0, #218
	movs r1, #144
	movs r2, #166
	movs r3, #1
	lsls r0, r0, #17
	lsls r1, r1, #15
	lsls r2, r2, #18
	bl Func_020052e0
	bl Func_020052e8
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #12
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #238
	movs r2, #166
	movs r0, #12
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_SetPositionAndReset
	movs r1, #192
	movs r2, #0
	movs r0, #12
	lsls r1, r1, #6
	bl Func_020052a8
	movs r0, #12
	movs r1, #4
	bl Object_SetModeById
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #12
	movs r1, #0
	bl Func_02005298
	movs r1, #237
	movs r2, #166
	movs r0, #12
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_SetPositionAndReset
	movs r1, #236
	movs r2, #162
	movs r0, #12
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_SetPositionAndReset
	movs r2, #0
	movs r0, #12
	movs r1, #0
	bl Func_02005230
	movs r1, #152
	lsls r1, r1, #6
	ldr r0, .L_0200c274
	adds r1, #102
	bl Func_020052d8
	movs r0, #182
	movs r1, #144
	movs r2, #166
	movs r3, #1
	lsls r0, r0, #17
	lsls r1, r1, #15
	lsls r2, r2, #18
	bl Func_020052e0
	bl Func_020052e8
	movs r1, #164
	movs r2, #162
	lsls r1, r1, #17
	lsls r2, r2, #18
	movs r0, #12
	bl Func_02005230
	movs r0, #1
	bl WaitFrames
	movs r1, #163
	movs r2, #166
	movs r0, #12
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_SetPositionAndReset
	movs r1, #160
	movs r2, #166
	movs r0, #12
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_SetPositionAndReset
	movs r1, #160
	movs r2, #0
	movs r0, #12
	lsls r1, r1, #7
	bl Func_020052a8
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #12
	movs r1, #0
	bl Func_02005298
	movs r1, #153
	lsls r1, r1, #8
	ldr r0, .L_0200c26c
	adds r1, #153
	bl Func_020052d8
	movs r0, #171
	movs r2, #172
	lsls r2, r2, #18
	movs r3, #1
	lsls r0, r0, #17
	movs r1, #0
	bl Func_020052e0
	bl Func_020052e8
	movs r1, #2
	movs r0, #14
	bl Motion_SetVarCbAndRefresh
	movs r0, #131
	bl Func_020053d8
	movs r1, #128
	lsls r1, r1, #1
	movs r0, #14
	bl Func_02005270
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #0
	movs r0, #14
	bl Func_02005270
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #14
	adds r1, #255
	bl Func_020052d0
	movs r0, #14
	bl Object_GetById
	movs r2, #85
	adds r7, r0, #0
	adds r2, r2, r7
	mov r3, r8
	strb r3, [r2]
	mov r10, r2
.L_0200c15e:
	ldr r3, [r7, #12]
	movs r2, #152
	lsls r2, r2, #6
	adds r2, #102
	adds r3, r3, r2
	str r3, [r7, #12]
	movs r0, #1
	adds r5, #1
	bl Battle_WaitMode0
	cmp r5, #63
	bls .L_0200c15e
	movs r3, #128
	lsls r3, r3, #5
	ldr r5, .L_0200c278
	mov r8, r3
	adds r0, r7, #0
	movs r1, #64
	mov r2, r8
	bl Func_02003e60
	adds r0, r7, #0
	adds r2, r5, #0
	movs r1, #128
	bl Func_02003e60
	adds r0, r7, #0
	movs r1, #128
	mov r2, r8
	bl Func_02003e60
	adds r2, r5, #0
	adds r0, r7, #0
	movs r1, #64
	bl Func_02003e60
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #1
	movs r0, #14
	bl Func_020052d0
	mov r2, r10
	movs r3, #3
	strb r3, [r2]
	movs r0, #5
	bl Battle_WaitMode0
	movs r0, #159
	bl Func_020053d8
	movs r3, #192
	lsls r3, r3, #10
	str r3, [r7, #40]
	movs r0, #60
	bl Battle_WaitMode0
	movs r0, #14
	movs r1, #0
	bl Func_02005298
	movs r0, #182
	movs r1, #144
	movs r2, #166
	movs r3, #1
	lsls r2, r2, #18
	lsls r0, r0, #17
	lsls r1, r1, #15
	bl Func_020052e0
	bl Func_020052e8
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #12
	movs r1, #0
	bl Func_02005298
	movs r1, #192
	movs r2, #10
	movs r0, #12
	lsls r1, r1, #6
	bl Func_020052a8
	movs r0, #12
	movs r1, #0
	bl Func_02005298
	movs r0, #200
	movs r2, #172
	movs r3, #1
	lsls r2, r2, #18
	lsls r0, r0, #17
	movs r1, #0
	bl Func_020052e0
	bl Func_020052e8
	movs r1, #2
	movs r0, #15
	bl Motion_SetVarCbAndRefresh
	movs r0, #131
	bl Func_020053d8
	movs r1, #128
	lsls r1, r1, #1
	movs r0, #15
	bl Func_02005270
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #0
	movs r0, #15
	bl Func_02005270
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #15
	bl Object_GetById
	adds r7, r0, #0
	adds r3, r7, #0
	movs r5, #0
	adds r3, #85
	ldr r6, [r7, #80]
	strb r5, [r3]
	b .L_0200c27c
	.2byte 0x0000
.L_0200c268:
	.4byte gPartyState
.L_0200c26c:
	.4byte 0x0004cccc
.L_0200c270:
	.4byte 0x00001765
.L_0200c274:
	.4byte 0x00013333
.L_0200c278:
	.4byte 0xfffff000
.L_0200c27c:
	ldr r3, [r7, #12]
	movs r2, #204
	lsls r2, r2, #6
	adds r2, #51
	adds r3, r3, r2
	str r3, [r7, #12]
	movs r0, #1
	adds r5, #1
	bl Battle_WaitMode0
	cmp r5, #63
	bls .L_0200c27c
	movs r2, #64
	adds r0, r6, #0
	movs r1, #32
	movs r5, #0
	bl Func_02003e34
	movs r1, #1
	movs r0, #15
	strh r5, [r6, #18]
	bl Motion_SetVarCbAndRefresh
	movs r0, #60
	bl Battle_WaitMode0
	movs r2, #64
	negs r2, r2
	adds r0, r6, #0
	movs r1, #32
	bl Func_02003e34
	movs r0, #15
	strh r5, [r6, #18]
	movs r1, #1
	bl Motion_SetVarCbAndRefresh
	movs r0, #60
	bl Battle_WaitMode0
.L_0200c2cc:
	ldr r3, [r7, #12]
	ldr r2, .L_0200c4cc
	movs r0, #1
	adds r3, r3, r2
	str r3, [r7, #12]
	adds r5, #1
	bl Battle_WaitMode0
	cmp r5, #63
	bls .L_0200c2cc
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #15
	bl Func_020052c8
	movs r2, #20
	movs r0, #15
	movs r1, #0
	bl Func_02005290
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #12
	movs r1, #0
	bl Func_02005298
	movs r1, #4
	movs r2, #20
	adds r1, #255
	movs r0, #15
	bl Func_020052c8
	movs r0, #15
	movs r1, #1
	bl Object_SetModeById
	movs r0, #15
	movs r1, #4
	movs r2, #0
	bl ObjectMotion_Launch
	movs r1, #176
	movs r2, #20
	movs r0, #15
	lsls r1, r1, #8
	bl Func_020052a8
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #15
	movs r1, #0
	bl Func_02005298
	movs r0, #182
	movs r1, #144
	movs r2, #166
	movs r3, #1
	lsls r0, r0, #17
	lsls r1, r1, #15
	lsls r2, r2, #18
	bl Func_020052e0
	bl Func_020052e8
	movs r1, #129
	movs r2, #20
	lsls r1, r1, #1
	movs r0, #12
	bl Func_020052c8
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #12
	movs r1, #0
	bl Func_02005298
	movs r0, #15
	movs r1, #0
	bl Func_02005298
	movs r1, #176
	movs r2, #40
	movs r0, #12
	lsls r1, r1, #8
	bl Func_020052a8
	movs r0, #12
	movs r1, #4
	bl Object_SetModeById
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #12
	movs r1, #0
	bl Func_02005298
	movs r0, #15
	movs r1, #0
	bl Func_02005298
	movs r1, #6
	movs r2, #80
	adds r1, #255
	movs r0, #12
	bl Func_020052c8
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #12
	movs r1, #0
	bl Func_02005298
	movs r1, #192
	movs r2, #20
	movs r0, #12
	lsls r1, r1, #6
	bl Func_020052a8
	movs r0, #12
	movs r1, #3
	bl Object_SetModeById
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #12
	movs r1, #0
	bl Func_02005298
	movs r1, #163
	movs r2, #166
	movs r0, #12
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_SetPositionAndReset
	movs r1, #164
	movs r2, #162
	lsls r2, r2, #2
	lsls r1, r1, #1
	movs r0, #12
	bl ObjectMotion_SetPositionAndReset
	movs r0, #12
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #12
	bl Object_GetById
	movs r1, #15
	bl Func_02005278
	movs r1, #220
	movs r2, #150
	movs r0, #12
	lsls r1, r1, #17
	lsls r2, r2, #18
	bl Func_02005230
	movs r0, #200
	movs r2, #172
	movs r3, #1
	lsls r0, r0, #17
	movs r1, #0
	lsls r2, r2, #18
	bl Func_020052e0
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #15
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #200
	movs r2, #161
	lsls r2, r2, #2
	lsls r1, r1, #1
	movs r0, #15
	bl ObjectMotion_SetPositionAndReset
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #188
	bl Func_020053d8
	bl Func_02001f3c
	movs r1, #0
	movs r0, #12
	bl Func_02005298
	ldr r5, .L_0200c4d0
	adds r0, r5, #0
	bl Scheduler_RemoveCallbackFar
	movs r0, #15
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #15
	movs r1, #3
	bl Func_020052b8
	movs r1, #200
	movs r2, #160
	movs r0, #15
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_SetPositionAndReset
	movs r0, #15
	movs r1, #0
	movs r2, #0
	bl Func_02005230
	movs r0, #12
	movs r1, #0
	movs r2, #0
	bl Func_02005230
	movs r0, #200
	movs r2, #188
	lsls r2, r2, #18
	movs r3, #1
	lsls r0, r0, #17
	movs r1, #0
	bl Func_020052e0
	bl Func_020052e8
	movs r1, #144
	lsls r1, r1, #3
	adds r0, r5, #0
	bl Func_020050d0
	bl Func_020051c8
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #72
	bl Func_02005118
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_0200c4cc:
	.4byte 0xffffcccd
.L_0200c4d0:
	.4byte Func_02001c8c
	.section .text.x0200c4d4,"ax",%progbits
	.global Func_020044d4
	.thumb_func
Func_020044d4:
	push {r5, r6, lr}
	bl Func_020051c0
	movs r0, #0
	bl Func_02005370
	movs r0, #1
	movs r1, #1
	movs r2, #1
	movs r3, #0
	negs r1, r1
	negs r2, r2
	negs r0, r0
	bl Func_020052e0
	movs r0, #1
	bl WaitFrames
	ldr r5, .L_0200c6cc
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
	movs r1, #164
	movs r2, #180
	ldr r0, [r5]
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_02005230
	movs r3, #160
	movs r1, #236
	movs r2, #204
	lsls r3, r3, #7
	lsls r1, r1, #17
	lsls r2, r2, #17
	movs r0, #12
	bl Func_02005238
	movs r0, #16
	bl Object_GetById
	movs r6, #192
	lsls r6, r6, #8
	strh r6, [r0, #6]
	movs r0, #1
	bl WaitFrames
	bl Event_SetStatus1c6
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	ldr r0, [r5]
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #164
	movs r2, #196
	ldr r0, [r5]
	lsls r2, r2, #1
	lsls r1, r1, #1
	bl ObjectMotion_SetPositionAndReset
	movs r0, #204
	movs r1, #200
	lsls r0, r0, #8
	lsls r1, r1, #5
	adds r0, #204
	adds r1, #153
	bl Func_020052d8
	movs r0, #204
	movs r1, #1
	movs r2, #200
	movs r3, #1
	lsls r0, r0, #17
	negs r1, r1
	lsls r2, r2, #17
	bl Func_020052e0
	movs r1, #188
	movs r2, #204
	lsls r2, r2, #1
	ldr r0, [r5]
	lsls r1, r1, #1
	bl ObjectMotion_SetPositionAndReset
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #1
	movs r0, #12
	bl Motion_SetVarCbAndRefresh
	ldr r0, .L_0200c6d0
	bl Func_02005280
	movs r0, #12
	movs r1, #0
	bl Func_02005298
	movs r1, #128
	movs r0, #12
	lsls r1, r1, #8
	movs r2, #20
	bl Func_020052a8
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #12
	bl Func_020052c8
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #12
	ldr r1, .L_0200c6d4
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r1, #228
	movs r2, #204
	lsls r2, r2, #1
	movs r0, #12
	lsls r1, r1, #1
	bl ObjectMotion_SetPositionAndReset
	movs r0, #12
	movs r1, #0
	bl Func_02005298
	ldr r1, [r5]
	movs r0, #16
	bl Func_02005240
	movs r0, #1
	bl WaitFrames
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #16
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #188
	movs r2, #196
	movs r0, #16
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	ldr r0, [r5]
	adds r1, r6, #0
	movs r2, #0
	bl Func_020052a8
	movs r1, #192
	movs r0, #16
	lsls r1, r1, #6
	movs r2, #40
	bl Func_020052a8
	ldr r0, [r5]
	movs r1, #0
	movs r2, #0
	bl Func_020052a8
	movs r2, #20
	movs r0, #16
	movs r1, #0
	bl Func_020052a8
	movs r1, #129
	movs r0, #12
	lsls r1, r1, #1
	bl Func_020052d0
	movs r0, #12
	movs r1, #0
	bl Func_02005298
	movs r1, #10
	adds r1, #255
	movs r2, #0
	movs r0, #16
	bl Func_020052c8
	movs r1, #192
	movs r0, #16
	lsls r1, r1, #6
	movs r2, #0
	bl Func_020052a8
	movs r2, #20
	ldr r0, [r5]
	adds r1, r6, #0
	bl Func_020052a8
	movs r0, #16
	movs r1, #0
	bl Func_02005298
	ldr r0, [r5]
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	ldr r0, [r5]
	movs r1, #0
	movs r2, #0
	bl Func_020052a8
	movs r0, #16
	movs r1, #0
	movs r2, #0
	bl Func_020052a8
	movs r1, #234
	movs r2, #214
	movs r0, #12
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #236
	movs r2, #227
	movs r0, #12
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #86
	str r2, [r3]
	bl Event_ClearStatus1c6
	bl Event_WaitValue1c8Frames
	movs r0, #13
	bl Func_020052f8
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200c6cc:
	.4byte gPartyState
.L_0200c6d0:
	.4byte 0x0000177c
.L_0200c6d4:
	.4byte 0x00019999
	.section .text.x0200c6d8,"ax",%progbits
	.global Func_020046d8
	.thumb_func
Func_020046d8:
	push {lr}
	bl Func_020051c0
	movs r0, #0
	bl Func_02005370
	ldr r3, .L_0200c7e4
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	movs r1, #0
	movs r2, #0
	bl Func_02005230
	movs r0, #12
	movs r1, #0
	movs r2, #0
	bl Func_02005230
	movs r0, #15
	movs r1, #0
	movs r2, #0
	bl Func_02005230
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r1, r1
	negs r2, r2
	movs r3, #0
	negs r0, r0
	bl Func_020052e0
	bl Func_020052f0
	movs r3, #0
	adds r0, #85
	strb r3, [r0]
	movs r0, #1
	bl WaitFrames
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #86
	str r2, [r3]
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	movs r1, #236
	movs r2, #162
	lsls r1, r1, #17
	lsls r2, r2, #18
	movs r0, #12
	bl Func_02005230
	movs r0, #1
	bl WaitFrames
	movs r1, #236
	movs r2, #166
	movs r0, #12
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_SetPositionAndReset
	movs r1, #192
	movs r0, #12
	lsls r1, r1, #6
	movs r2, #20
	bl Func_020052a8
	movs r1, #160
	movs r0, #12
	lsls r1, r1, #7
	movs r2, #20
	bl Func_020052a8
	movs r1, #192
	movs r2, #0
	movs r0, #12
	lsls r1, r1, #6
	bl Func_020052a8
	movs r1, #153
	lsls r1, r1, #8
	ldr r0, .L_0200c7e8
	adds r1, #153
	bl Func_020052d8
	movs r0, #228
	movs r1, #160
	movs r2, #166
	movs r3, #1
	lsls r2, r2, #18
	lsls r1, r1, #14
	lsls r0, r0, #17
	bl Func_020052e0
	bl Func_020052e8
	movs r0, #40
	bl Battle_WaitMode0
	ldr r0, .L_0200c7ec
	bl Func_02005280
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #12
	movs r1, #0
	bl Func_02005298
	movs r1, #236
	movs r2, #162
	movs r0, #12
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_SetPositionAndReset
	bl Event_ClearStatus1c6
	bl Event_WaitValue1c8Frames
	movs r0, #14
	bl Func_020052f8
	pop {pc}
	.2byte 0x0000
.L_0200c7e4:
	.4byte gPartyState
.L_0200c7e8:
	.4byte 0x0004cccc
.L_0200c7ec:
	.4byte 0x00001780
	.section .text.x0200c7f0,"ax",%progbits
	.global Func_020047f0
	.thumb_func
Func_020047f0:
	push {lr}
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #12
	ldr r1, .L_0200c828
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r1, #248
	movs r2, #196
	movs r0, #12
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #128
	movs r0, #12
	lsls r1, r1, #8
	movs r2, #0
	bl Func_020052a8
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #12
	movs r1, #0
	bl Func_02005298
	pop {pc}
.L_0200c828:
	.4byte 0x00019999
	.section .text.x0200c82c,"ax",%progbits
	.global Func_0200482c
	.thumb_func
Func_0200482c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	bl Func_020051c0
	movs r0, #0
	bl Func_02005370
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r1, r1
	negs r2, r2
	movs r3, #0
	negs r0, r0
	bl Func_020052e0
	bl Func_020052f0
	movs r3, #0
	adds r0, #85
	strb r3, [r0]
	movs r0, #1
	bl WaitFrames
	movs r1, #188
	movs r2, #196
	movs r0, #16
	lsls r1, r1, #17
	lsls r2, r2, #17
	movs r3, #0
	bl Func_02005238
	ldr r6, .L_0200cb50
	movs r1, #133
	lsls r1, r1, #2
	adds r6, r6, r1
	movs r2, #204
	movs r1, #188
	ldr r0, [r6]
	lsls r1, r1, #17
	lsls r2, r2, #17
	movs r3, #0
	bl Func_02005238
	movs r7, #192
	movs r3, #176
	movs r1, #236
	movs r2, #227
	movs r0, #12
	lsls r1, r1, #17
	lsls r3, r3, #8
	lsls r2, r2, #17
	lsls r7, r7, #18
	bl Func_02005238
	ldr r2, [r7, #108]
	movs r3, #128
	movs r5, #214
	lsls r3, r3, #2
	adds r3, #2
	lsls r5, r5, #1
	str r3, [r2, r5]
	bl Event_SetStatus1c6
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #12
	ldr r1, .L_0200cb54
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r1, #232
	movs r0, #12
	lsls r1, r1, #1
	adds r2, r5, #0
	bl ObjectMotion_SetPositionAndReset
	movs r1, #216
	movs r2, #204
	movs r0, #12
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #128
	movs r2, #0
	movs r0, #12
	lsls r1, r1, #8
	bl Func_020052a8
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #12
	bl Func_020052d0
	ldr r0, .L_0200cb58
	bl Func_02005280
	movs r0, #12
	movs r1, #0
	bl Func_02005298
	movs r1, #6
	movs r2, #40
	adds r1, #255
	movs r0, #12
	bl Func_020052c8
	movs r0, #12
	movs r1, #0
	bl Func_02005298
	movs r2, #40
	movs r0, #12
	movs r1, #0
	bl Func_020052a8
	movs r0, #12
	movs r1, #0
	bl Func_02005298
	movs r0, #16
	movs r1, #0
	bl Func_02005298
	movs r0, #12
	movs r1, #2
	bl Motion_SetVarCbAndRefresh
	movs r1, #128
	movs r2, #20
	movs r0, #12
	lsls r1, r1, #8
	bl Func_020052a8
	movs r0, #12
	movs r1, #4
	bl Object_SetModeById
	movs r0, #12
	movs r1, #0
	bl Func_02005298
	movs r1, #192
	movs r0, #16
	lsls r1, r1, #6
	movs r2, #10
	bl Func_020052a8
	movs r1, #192
	ldr r0, [r6]
	lsls r1, r1, #8
	movs r2, #0
	bl Func_020052a8
	movs r1, #0
	movs r0, #16
	bl Func_02005288
	ldr r0, [r6]
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_0200c99a
	movs r0, #12
	movs r1, #4
	bl Object_SetModeById
	movs r0, #12
	movs r1, #0
	bl Func_02005298
	ldr r2, [r7, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_0200c9b8
.L_0200c99a:
	ldr r2, [r7, #108]
	movs r1, #226
	lsls r1, r1, #1
	adds r2, r2, r1
	ldrh r3, [r2]
	movs r0, #12
	adds r3, #1
	movs r1, #3
	strh r3, [r2]
	bl Object_SetModeById
	movs r0, #12
	movs r1, #0
	bl Func_02005298
.L_0200c9b8:
	ldr r5, .L_0200cb50
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
	ldr r0, [r5]
	movs r1, #0
	movs r2, #0
	bl Func_020052a8
	movs r0, #16
	movs r1, #0
	movs r2, #0
	bl Func_020052a8
	movs r1, #2
	movs r2, #20
	adds r1, #255
	movs r0, #16
	bl Func_020052c8
	movs r0, #16
	movs r1, #0
	bl Func_02005298
	movs r1, #129
	movs r0, #12
	lsls r1, r1, #1
	bl Func_020052d0
	movs r0, #12
	movs r1, #0
	bl Func_02005298
	movs r0, #16
	movs r1, #3
	bl Object_SetModeById
	movs r0, #16
	movs r1, #0
	movs r2, #40
	bl Func_02005290
	movs r1, #160
	movs r2, #40
	movs r0, #12
	lsls r1, r1, #7
	bl Func_020052a8
	movs r0, #12
	movs r1, #0
	bl Func_02005298
	movs r1, #192
	movs r0, #16
	lsls r1, r1, #6
	movs r2, #0
	bl Func_020052a8
	movs r1, #192
	ldr r0, [r5]
	lsls r1, r1, #8
	movs r2, #40
	bl Func_020052a8
	movs r1, #128
	movs r2, #20
	movs r0, #12
	lsls r1, r1, #8
	bl Func_020052a8
	movs r0, #12
	movs r1, #3
	bl Object_SetModeById
	movs r0, #12
	movs r1, #0
	bl Func_02005298
	movs r0, #16
	movs r1, #0
	movs r2, #0
	bl Func_020052a8
	ldr r0, [r5]
	movs r1, #0
	movs r2, #0
	bl Func_020052a8
	movs r1, #2
	movs r2, #0
	adds r1, #255
	movs r0, #16
	bl Func_020052c8
	movs r0, #16
	movs r1, #0
	bl Func_02005298
	movs r0, #12
	movs r1, #3
	bl Object_SetModeById
	movs r0, #12
	movs r1, #0
	bl Func_02005298
	movs r0, #204
	movs r1, #200
	lsls r0, r0, #8
	lsls r1, r1, #5
	adds r0, #204
	adds r1, #153
	bl Func_020052d8
	movs r0, #212
	movs r1, #128
	movs r2, #192
	movs r3, #1
	lsls r0, r0, #17
	lsls r1, r1, #13
	lsls r2, r2, #17
	bl Func_020052e0
	movs r1, #204
	movs r2, #204
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
	movs r0, #16
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	adds r2, #102
	movs r0, #12
	adds r1, #204
	bl ObjectMotion_SetSpeedParameters
	ldr r0, [r5]
	ldr r1, .L_0200cb5c
	bl ObjectMotion_EnableActionAndSetCallback
	ldr r1, .L_0200cb60
	movs r0, #16
	bl ObjectMotion_EnableActionAndSetCallback
	ldr r1, .L_0200cb64
	movs r0, #12
	bl Object_SetActionCallbackAndRefreshById
	movs r0, #40
	bl Battle_WaitMode0
	bl Func_02000bcc
	movs r1, #192
	movs r0, #16
	lsls r1, r1, #6
	movs r2, #0
	bl Func_020052a8
	movs r1, #160
	ldr r0, [r5]
	lsls r1, r1, #8
	movs r2, #20
	bl Func_020052a8
	movs r0, #160
	lsls r0, r0, #7
	movs r1, #0
	adds r0, #16
	bl Func_02005288
	ldr r0, [r5]
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_0200cb68
	bl Func_020047f0
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_0200cb7e
	.2byte 0x0000
.L_0200cb50:
	.4byte gPartyState
.L_0200cb54:
	.4byte 0x00013333
.L_0200cb58:
	.4byte 0x00001781
.L_0200cb5c:
	.4byte Data_02005564
.L_0200cb60:
	.4byte Data_020055a0
.L_0200cb64:
	.4byte Data_020055e4
.L_0200cb68:
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r1, #226
	lsls r1, r1, #1
	adds r2, r2, r1
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	bl Func_020047f0
.L_0200cb7e:
	movs r0, #16
	movs r1, #0
	movs r2, #0
	bl Func_020052a8
	ldr r5, .L_0200cc70
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
	ldr r0, [r5]
	movs r2, #0
	movs r1, #0
	bl Func_020052a8
	movs r0, #12
	movs r1, #3
	bl Object_SetModeById
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #12
	movs r1, #0
	bl Func_02005298
	ldr r0, [r5]
	movs r1, #3
	bl Object_SetModeById
	movs r0, #16
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #12
	movs r1, #0
	bl Func_02005298
	movs r1, #128
	movs r2, #40
	lsls r1, r1, #1
	movs r0, #16
	bl Func_020052c8
	movs r0, #160
	lsls r0, r0, #7
	adds r0, #16
	movs r1, #0
	bl Func_02005298
	movs r0, #12
	movs r1, #3
	bl Object_SetModeById
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #12
	movs r1, #0
	bl Func_02005298
	movs r1, #192
	movs r0, #16
	lsls r1, r1, #6
	movs r2, #0
	bl Func_020052a8
	movs r1, #160
	ldr r0, [r5]
	lsls r1, r1, #8
	movs r2, #0
	bl Func_020052a8
	movs r0, #160
	lsls r0, r0, #7
	movs r1, #0
	adds r0, #16
	bl Func_02005288
	ldr r0, [r5]
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_0200cc74
	movs r0, #12
	movs r1, #3
	bl Object_SetModeById
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #12
	movs r1, #0
	bl Func_02005298
	movs r0, #16
	movs r1, #0
	movs r2, #0
	bl Func_020052a8
	movs r1, #8
	movs r2, #20
	adds r1, #255
	movs r0, #16
	bl Func_020052c8
	movs r0, #160
	lsls r0, r0, #7
	adds r0, #16
	movs r1, #0
	bl Func_02005298
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #2
	strh r3, [r2]
	b .L_0200ccbc
.L_0200cc70:
	.4byte gPartyState
.L_0200cc74:
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r1, #226
	lsls r1, r1, #1
	adds r2, r2, r1
	ldrh r3, [r2]
	movs r0, #12
	adds r3, #2
	strh r3, [r2]
	movs r1, #4
	bl Object_SetModeById
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #12
	movs r1, #0
	bl Func_02005298
	movs r0, #16
	movs r1, #0
	movs r2, #0
	bl Func_020052a8
	movs r1, #8
	adds r1, #255
	movs r0, #16
	movs r2, #20
	bl Func_020052c8
	movs r0, #160
	lsls r0, r0, #7
	adds r0, #16
	movs r1, #0
	bl Func_02005298
.L_0200ccbc:
	ldr r6, .L_0200ce58
	movs r2, #133
	lsls r2, r2, #2
	adds r6, r6, r2
	ldr r0, [r6]
	movs r1, #0
	movs r2, #0
	bl Func_020052a8
	movs r0, #212
	movs r2, #200
	movs r3, #1
	lsls r2, r2, #17
	lsls r0, r0, #17
	movs r1, #0
	bl Func_020052e0
	movs r0, #153
	movs r1, #152
	lsls r0, r0, #8
	lsls r1, r1, #5
	adds r0, #153
	adds r1, #51
	bl Func_020052d8
	movs r1, #153
	movs r2, #152
	lsls r1, r1, #8
	lsls r2, r2, #7
	adds r2, #204
	movs r0, #12
	adds r1, #153
	bl ObjectMotion_SetSpeedParameters
	ldr r0, [r6]
	movs r1, #12
	bl Object_LinkObjectAndSetCallback
	movs r0, #16
	movs r1, #12
	bl Object_LinkObjectAndSetCallback
	movs r1, #244
	movs r2, #208
	movs r0, #12
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #236
	movs r2, #216
	movs r0, #12
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #160
	movs r2, #0
	movs r0, #12
	lsls r1, r1, #7
	bl Func_020052a8
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #12
	movs r1, #0
	bl Func_02005298
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #16
	movs r1, #0
	movs r2, #80
	bl Func_02005290
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #12
	bl Func_020052c8
	movs r1, #176
	movs r2, #20
	movs r0, #12
	lsls r1, r1, #8
	bl Func_020052a8
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #12
	movs r1, #0
	bl Func_02005298
	movs r0, #12
	movs r1, #3
	bl Object_SetModeById
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #12
	movs r1, #0
	bl Func_02005298
	movs r0, #212
	movs r1, #128
	movs r2, #192
	movs r3, #1
	lsls r0, r0, #17
	lsls r1, r1, #13
	lsls r2, r2, #17
	bl Func_020052e0
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #12
	ldr r1, .L_0200ce5c
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r1, #244
	movs r2, #208
	movs r0, #12
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #252
	movs r2, #188
	movs r0, #12
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #128
	movs r2, #20
	lsls r1, r1, #8
	movs r0, #12
	bl Func_020052a8
	movs r0, #12
	bl Object_GetById
	adds r5, r0, #0
	ldr r3, [r5, #80]
	movs r0, #12
	movs r1, #1
	mov r10, r3
	bl Func_020052b8
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #12
	ldr r1, .L_0200ce60
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r0, #12
	movs r1, #4
	movs r2, #0
	bl ObjectMotion_Launch
	movs r1, #246
	movs r2, #188
	lsls r2, r2, #1
	movs r0, #12
	lsls r1, r1, #1
	bl ObjectMotion_SetPositionAndCommit
	movs r0, #12
	movs r1, #7
	bl Object_SetModeById
	ldr r3, .L_0200ce64
	ldr r2, .L_0200ce54
	str r3, [r5, #24]
	movs r1, #0
	movs r3, #224
	adds r7, r5, #0
	mov r8, r1
	lsls r3, r3, #8
	mov r1, r10
	adds r7, #85
	strh r3, [r1, #18]
	strb r2, [r7]
	movs r2, #128
	ldr r3, [r5, #12]
	lsls r2, r2, #11
	adds r3, r3, r2
	str r3, [r5, #12]
	movs r0, #12
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #7
	lsls r2, r2, #6
	movs r0, #12
	adds r1, #102
	adds r2, #51
	b .L_0200ce68
	.2byte 0x0000
.L_0200ce54:
	.4byte 0x00000000
.L_0200ce58:
	.4byte gPartyState
.L_0200ce5c:
	.4byte 0x00013333
.L_0200ce60:
	.4byte 0x00019999
.L_0200ce64:
	.4byte 0xffff0000
.L_0200ce68:
	bl ObjectMotion_SetSpeedParameters
	movs r1, #222
	movs r2, #160
	movs r3, #188
	lsls r2, r2, #14
	lsls r1, r1, #17
	lsls r3, r3, #17
	adds r0, r5, #0
	bl Func_02005160
	movs r0, #12
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r3, #3
	strb r3, [r7]
	movs r0, #12
	bl Object_GetById
	movs r1, #1
	bl ObjectDispatch_SetSingleChildField26
	mov r3, r8
	mov r1, r10
	strh r3, [r1, #18]
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r5, #24]
	movs r3, #128
	lsls r3, r3, #14
	str r3, [r5, #20]
	movs r0, #12
	movs r1, #1
	bl Object_SetModeById
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #12
	ldr r1, .L_0200cfb0
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r0, #12
	movs r1, #4
	movs r2, #0
	bl ObjectMotion_Launch
	movs r1, #212
	movs r2, #188
	movs r0, #12
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndCommit
	movs r0, #204
	movs r1, #128
	movs r2, #200
	movs r3, #1
	lsls r0, r0, #17
	lsls r1, r1, #13
	lsls r2, r2, #17
	bl Func_020052e0
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #12
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #196
	movs r2, #180
	movs r0, #12
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #192
	movs r2, #0
	movs r0, #12
	lsls r1, r1, #6
	bl Func_020052a8
	movs r1, #2
	movs r0, #12
	bl Func_020052b8
	movs r0, #16
	bl ObjectMotion_EnableActionAndResetMotion
	ldr r0, [r6]
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #1
	bl WaitFrames
	movs r1, #192
	movs r0, #16
	lsls r1, r1, #6
	movs r2, #0
	bl Func_020052a8
	movs r1, #160
	movs r2, #10
	ldr r0, [r6]
	lsls r1, r1, #8
	bl Func_020052a8
	ldr r0, [r6]
	movs r1, #3
	bl Object_SetModeById
	movs r0, #16
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #16
	movs r1, #2
	bl Object_SetModeById
	ldr r0, [r6]
	bl Object_GetById
	cmp r0, #0
	beq .L_0200cf74
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #16
	bl ObjectMotion_ResetAndSetPosition
.L_0200cf74:
	movs r0, #16
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r1, #0
	movs r2, #0
	movs r0, #16
	bl Func_02005230
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #73
	bl Func_02005118
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
	bl Func_020051c8
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200cfb0:
	.4byte 0x00019999
	.section .text.x0200cfb4,"ax",%progbits
	.global Func_02004fb4
	.thumb_func
Func_02004fb4:
	push {r5, r6, r7, lr}
	movs r0, #234
	movs r1, #204
	movs r2, #160
	movs r3, #172
	adds r0, #255
	lsls r1, r1, #17
	lsls r2, r2, #14
	lsls r3, r3, #17
	ldr r5, .L_0200d024
	bl Func_02005140
	movs r7, #0
	str r0, [r5]
	cmp r0, #0
	beq .L_0200d022
	ldr r6, [r0, #80]
	movs r3, #33
	ldrb r2, [r6, #5]
	negs r3, r3
	ands r3, r2
	ldrb r2, [r6, #9]
	strb r3, [r6, #5]
	movs r3, #15
	ands r3, r2
	strb r3, [r6, #9]
	adds r3, r0, #0
	adds r3, #85
	adds r2, r0, #0
	adds r2, #92
	strb r7, [r3]
	movs r1, #193
	movs r3, #1
	strb r3, [r2]
	lsls r1, r1, #3
	strb r7, [r6, #26]
	strb r7, [r6, #27]
	movs r0, #68
	bl Runtime_AllocateHeapBlockFar
	adds r5, r0, #0
	movs r0, #198
	bl Func_020051b0
	movs r3, #128
	lsls r3, r3, #3
	adds r5, r5, r3
	ldrb r0, [r6, #16]
	movs r1, #128
	adds r2, r5, #0
	bl VramBlock_LoadCached
	movs r0, #68
	bl Runtime_ReleaseHeapBlock
.L_0200d022:
	pop {r5, r6, r7, pc}
.L_0200d024:
	.4byte gOverlayArea + 0x62ac
	.section .text.x0200d028,"ax",%progbits
	.global Func_02005028
	.thumb_func
Func_02005028:
	push {r5, lr}
	sub sp, #8
	bl Func_020051c0
	movs r0, #0
	bl Func_02005370
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #121
	bl Func_020053d8
	movs r5, #1
	movs r1, #0
	movs r2, #20
	movs r3, #0
	movs r0, #41
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl Func_02005170
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #1
	movs r2, #20
	movs r3, #1
	movs r0, #41
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl Func_02005170
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #2
	movs r2, #20
	movs r3, #2
	movs r0, #41
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl Func_02005170
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #3
	movs r2, #20
	movs r3, #3
	movs r0, #41
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl Func_02005170
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #45
	bl Func_02005118
	bl Func_020051c8
	add sp, #8
	pop {r5, pc}
	.2byte 0x0000
	.section .rodata.x0200d3e0,"a",%progbits
.L_0200d3e0:
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
.L_0200d41c:
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
.L_0200d458:
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
	.global Data_02005494
Data_02005494:
	.4byte 0x00000000
	.4byte 0x0000002c
	.4byte 0x00000016
	.4byte 0x00000026
	.4byte 0x00000000
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.4byte 0x0000002e
	.4byte Func_0200028c
	.4byte 0x00000016
	.4byte 0x00000026
	.4byte 0x00000000
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.global Data_020054dc
Data_020054dc:
	.4byte 0xffff000c
	.global Data_020054e0
Data_020054e0:
	.4byte 0xffff0008
	.global Data_020054e4
Data_020054e4:
	.4byte 0x00000000
	.4byte 0x00000018
	.4byte 0x00000026
	.global Data_020054f0
Data_020054f0:
	.4byte 0x00000000
	.4byte 0x0000000f
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.global Data_02005508
Data_02005508:
	.4byte .L_0200d3e0
	.4byte .L_0200d41c
	.4byte .L_0200d458
	.global Data_02005514
Data_02005514:
.L_0200d514:
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00005000
	.4byte 0x00000000
	.4byte 0x00000050
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00003000
	.4byte 0x00000000
	.4byte 0x00000050
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0x00000000
	.global Data_02005548
Data_02005548:
	.4byte 0x0000002e
	.4byte Func_020002a0
	.global Data_02005550
Data_02005550:
	.4byte 0x0000002e
	.4byte Func_020002dc
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0x00000000
	.global Data_02005564
Data_02005564:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01c80000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000d000
	.4byte 0x00000011
	.global Data_020055a0
Data_020055a0:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000d000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.global Data_020055e4
Data_020055e4:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01f00000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
.L_0200d634:
	.4byte 0x0000002e
	.4byte Func_0200035c
	.4byte 0x00000016
	.4byte 0x00000006
	.4byte 0x00580000
	.4byte 0x00000011
.L_0200d64c:
	.4byte 0x0000002e
	.4byte Func_0200035c
	.4byte 0x00000016
	.4byte 0x00000006
	.4byte 0x002c0000
	.4byte 0x00000011
.L_0200d664:
	.4byte 0x0000002e
	.4byte Func_0200035c
	.4byte 0x00000011
	.global Data_02005670
Data_02005670:
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
	.global Data_020056a0
Data_020056a0:
	.4byte 0x004a0140
	.4byte 0x01500060
	.4byte 0x0070005a
	.4byte 0x0002ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020056c0
Data_020056c0:
	.4byte 0x002a0200
	.4byte 0x02100110
	.4byte 0x0120003a
	.4byte 0x0002ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020056e0
Data_020056e0:
	.4byte 0x0000000e
	.4byte 0x00101014
	.4byte 0x00203014
	.4byte 0x00301013
	.4byte 0x00402011
	.4byte 0x00505011
	.4byte 0x00603011
	.4byte 0x00704011
	.4byte 0x0000000f
	.4byte 0x00109016
	.4byte 0x00204014
	.4byte 0x00301010
	.4byte 0x00402014
	.4byte 0x00000010
	.4byte 0x0010300f
	.4byte 0x00208016
	.4byte 0x00000011
	.4byte 0x00102013
	.4byte 0x0020400e
	.4byte 0x0030500e
	.4byte 0x0040600e
	.4byte 0x00000012
	.4byte 0x00103015
	.4byte 0x00204015
	.4byte 0x00000013
	.4byte 0x0010300e
	.4byte 0x00201011
	.4byte 0x00000014
	.4byte 0x0010100e
	.4byte 0x0020400f
	.4byte 0x0030200e
	.4byte 0x0040200f
	.4byte 0x00000015
	.4byte 0x00104002
	.4byte 0x00201016
	.4byte 0x00301012
	.4byte 0x00402012
	.4byte 0x00000016
	.4byte 0x00102015
	.4byte 0x00204016
	.4byte 0x00305016
	.4byte 0x00402016
	.4byte 0x00503016
	.4byte 0x00607016
	.4byte 0x00706016
	.4byte 0x00802010
	.4byte 0x0090100f
	.4byte 0x00d0d016
	.4byte 0x00e0e016
	.4byte 0x000001ff
	.global Data_020057a8
Data_020057a8:
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020057c0
Data_020057c0:
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte Data_02024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x03024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x03024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x03024000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte Data_02024000
	.4byte 0xffff015f
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff015f
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff015f
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff015f
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
	.global Data_020058b0
Data_020058b0:
	.4byte 0xffff0170
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte Data_02024000
	.4byte 0xffff0170
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte Data_02024000
	.4byte 0xffff01c2
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x01f80000
	.4byte 0x00024000
	.4byte 0xffff00b8
	.4byte 0x00000007
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00024000
	.4byte 0xffff00b8
	.4byte 0x00000007
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x008c0000
	.4byte 0x00024000
	.4byte 0xffff00b8
	.4byte 0x00000007
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x008c0000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02005958
Data_02005958:
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte Data_02024000
	.4byte 0xffff015f
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte Data_02028000
	.4byte 0xffff015f
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte gMapBlocks
	.4byte 0xffff015f
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff015f
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff015f
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff015f
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff015f
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff015f
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff015f
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff015f
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff015f
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff015f
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
	.global Data_02005aa8
Data_02005aa8:
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte Data_02024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x03024000
	.4byte 0xffff005b
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x01540000
	.4byte 0x00024000
	.4byte 0x004b00f4
	.4byte 0x00000001
	.4byte 0x02780000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02005b20
Data_02005b20:
	.4byte 0xffff0133
	.4byte 0x00000001
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x00e00000
	.4byte 0x00024000
	.4byte 0xffff005b
	.4byte .L_0200d514
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00013000
	.4byte 0xffff005b
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00015000
	.4byte 0xffff005b
	.4byte 0x00000001
	.4byte 0x014e0000
	.4byte 0x00000000
	.4byte 0x013d0000
	.4byte 0x00023000
	.4byte 0xffff005b
	.4byte 0x00000001
	.4byte 0x011c0000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x00023000
	.4byte 0xffff005b
	.4byte 0x00000001
	.4byte 0x00f60000
	.4byte 0x00000000
	.4byte 0x013e0000
	.4byte 0x00025000
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
	.global Data_02005be0
Data_02005be0:
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x01c80000
	.4byte 0x00000000
	.4byte 0x02a40000
	.4byte 0x00024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x02a40000
	.4byte 0x01024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x00024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x01024000
	.4byte 0xffff0015
	.4byte 0x00000001
	.4byte 0x01c80000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x00034000
	.4byte 0xffff005b
	.4byte 0x00000001
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00024000
	.4byte 0xffff005b
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00024000
	.4byte 0xffff005b
	.4byte 0x00000001
	.4byte 0x01900000
	.4byte 0x00000000
	.4byte 0x02c80000
	.4byte 0x00024000
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0132
	.4byte .L_0200d634
	.4byte 0x01b70000
	.4byte 0x00000000
	.4byte 0x02a00000
	.4byte 0x00024000
	.4byte 0xffff0132
	.4byte .L_0200d634
	.4byte 0x01670000
	.4byte 0x00000000
	.4byte 0x02a00000
	.4byte 0x00024000
	.4byte 0xffff0132
	.4byte .L_0200d64c
	.4byte 0x019c0000
	.4byte 0x00000000
	.4byte 0x01440000
	.4byte 0x00024000
	.4byte 0xffff0132
	.4byte .L_0200d664
	.4byte 0x005a0000
	.4byte 0x00000000
	.4byte 0x00e60000
	.4byte 0x00024000
	.4byte 0xffff0132
	.4byte .L_0200d664
	.4byte 0x00ae0000
	.4byte 0x00000000
	.4byte 0x00480000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02005d48
Data_02005d48:
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02005d54
Data_02005d54:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000031
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000031
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000008
	.4byte 0xffff0000
	.4byte Func_020020c8
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte Func_020020d8
	.4byte 0x10008c15
	.4byte 0xffff000c
	.4byte Func_020020c8
	.4byte 0x00008c15
	.4byte 0xffff000c
	.4byte Func_020020d8
	.4byte 0x00000002
	.4byte 0x0211000a
	.4byte Func_02003584
	.4byte 0x00000002
	.4byte 0xffff000c
	.4byte Func_0200393c
	.4byte 0x00000002
	.4byte 0xffff000d
	.4byte Func_020039a8
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02005de4
Data_02005de4:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000031
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000021
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000031
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02005e20
Data_02005e20:
	.4byte 0x00000031
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x10008c15
	.4byte 0x03040008
	.4byte Func_020021b8
	.4byte 0x00008c15
	.4byte 0x03040008
	.4byte Func_020021dc
	.4byte 0x00000602
	.4byte 0x0304000a
	.4byte Func_02002248
	.4byte 0x0000c602
	.4byte 0x0304000b
	.4byte Func_02002248
	.4byte 0x00008602
	.4byte 0x0304000c
	.4byte Func_02002248
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte Func_02001d00
	.4byte 0x00000002
	.4byte 0x092d0006
	.4byte Func_020026cc
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02005e8c
Data_02005e8c:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000021
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000008
	.4byte 0xffff0000
	.4byte Func_02002120
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte Func_02002154
	.4byte 0x10008c15
	.4byte 0xffff0008
	.4byte Func_02002120
	.4byte 0x00008c15
	.4byte 0xffff0008
	.4byte Func_02002154
	.4byte 0x00000002
	.4byte 0xffff0014
	.4byte Func_02003564
	.4byte 0x00000002
	.4byte 0xffff0015
	.4byte Func_02003574
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02005ef8
Data_02005ef8:
	.4byte 0x00000021
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02005f10
Data_02005f10:
	.4byte 0x00000021
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000002
	.4byte 0xffff000a
	.4byte Func_02000710
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02005f40
Data_02005f40:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000021
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000021
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00008515
	.4byte 0x020f0008
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte Func_02000280
	.4byte 0x00000002
	.4byte 0xffff000a
	.4byte Func_02002674
	.4byte 0x00000002
	.4byte 0xffff000b
	.4byte Func_0200269c
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x0000177a
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x0000177b
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02005fc4
Data_02005fc4:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000031
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00004602
	.4byte 0xffff0006
	.4byte Func_02002014
	.4byte 0x000001f3
	.4byte 0xffff00c8
	.4byte 0x0040303d
	.4byte 0x00000002
	.4byte 0x02300014
	.4byte Func_020004a8
	.4byte 0x00004602
	.4byte 0x02120004
	.4byte Func_020022c0
	.4byte 0x0000c602
	.4byte 0x02120005
	.4byte Func_020022c0
	.4byte 0x00000003
	.4byte 0xffff000a
	.4byte Func_02001d20
	.4byte 0x00004e15
	.4byte 0xffff0008
	.4byte Func_02002314
	.4byte 0x50008905
	.4byte 0xffff001e
	.4byte Func_020027b4
	.4byte 0x50008905
	.4byte 0xffff001f
	.4byte Func_020027d4
	.4byte 0x50008905
	.4byte 0xffff0020
	.4byte Func_020027f4
	.4byte 0x50008905
	.4byte 0xffff0021
	.4byte Func_02002814
	.4byte 0x50008905
	.4byte 0xffff0022
	.4byte Func_02002834
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte Func_02001d40
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte Func_02001db4
	.4byte 0x00008d15
	.4byte Resource_Data012 + 0x150409
	.4byte Func_02001e30
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x0000175d
	.4byte 0x00008d15
	.4byte Resource_Data012 + 0x16040a
	.4byte Func_02001e44
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x0000175e
	.4byte 0x00000000
	.4byte Resource_Data012 + 0x19000b
	.4byte 0x0000175f
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x000017b1
	.4byte 0x00000000
	.4byte Resource_Data012 + 0x19000c
	.4byte 0x00001760
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x000017b2
	.4byte 0x00000000
	.4byte Resource_Data012 + 0x19000d
	.4byte 0x00001761
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x000017b3
	.4byte 0x00008d15
	.4byte Resource_Data012 + 0x19000b
	.4byte 0x00001762
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x000017b4
	.4byte 0x00008d15
	.4byte Resource_Data012 + 0x19000c
	.4byte 0x00001763
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x000017b5
	.4byte 0x00008d15
	.4byte Resource_Data012 + 0x19000d
	.4byte 0x00001764
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x000017b6
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_0200615c
Data_0200615c:
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
	.4byte 0x00000202
	.4byte 0xffff0008
	.4byte Func_02001e58
	.4byte 0x0000c602
	.4byte 0xffff0009
	.4byte Func_02001f7c
	.4byte 0x00008515
	.4byte 0x020f0008
	.4byte 0x00000000
	.4byte 0x00008515
	.4byte 0x0210000a
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte Resource_Data012 + 0x19000d
	.4byte 0x00001776
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte Func_02001e0c
	.4byte 0x00000000
	.4byte Resource_Data012 + 0x19000e
	.4byte 0x00001777
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x000017ac
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x000017ad
	.4byte 0x00008d15
	.4byte Resource_Data012 + 0x19000d
	.4byte 0x00001778
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x000017ae
	.4byte 0x00008d15
	.4byte Resource_Data012 + 0x19000e
	.4byte 0x00001779
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x000017af
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x000017b0
	.4byte 0x00000003
	.4byte Resource_Data012 + 0x1a0010
	.4byte Func_020023e8
	.4byte 0x00000000
	.4byte Resource_Data012 + 0x1a000c
	.4byte 0x000017a0
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x000017a7
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x000017a8
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
