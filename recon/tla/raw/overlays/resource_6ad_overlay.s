.syntax unified
	.thumb
	.section .text.x02008038,"ax",%progbits
	.global Func_02000038
	.thumb_func
Func_02000038:
	ldr r0, .L_0200803c
	bx lr
.L_0200803c:
	.4byte Data_02006430
	.section .text.x02008040,"ax",%progbits
	.global Func_02000040
	.thumb_func
Func_02000040:
	movs r0, #0
	bx lr
	.section .text.x02008044,"ax",%progbits
	.global Func_02000044
	.thumb_func
Func_02000044:
	ldr r0, .L_02008048
	bx lr
.L_02008048:
	.4byte Data_02006460
	.section .text.x0200804c,"ax",%progbits
	.global Func_0200004c
	.thumb_func
Func_0200004c:
	push {lr}
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #0
	pop {pc}
	.section .text.x02008058,"ax",%progbits
	.global Func_02000058
	.thumb_func
Func_02000058:
	push {lr}
	ldr r3, .L_02008094
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008098
	cmp r2, r3
	bne .L_02008070
	ldr r0, .L_0200809c
	b .L_02008090
.L_02008070:
	ldr r3, .L_020080a0
	cmp r2, r3
	bne .L_0200807a
	ldr r0, .L_020080a4
	b .L_02008090
.L_0200807a:
	ldr r3, .L_020080a8
	cmp r2, r3
	bne .L_02008084
	ldr r0, .L_020080ac
	b .L_02008090
.L_02008084:
	ldr r3, .L_020080b0
	cmp r2, r3
	bne .L_0200808e
	ldr r0, .L_020080b4
	b .L_02008090
.L_0200808e:
	ldr r0, .L_020080b8
.L_02008090:
	pop {pc}
	.2byte 0x0000
.L_02008094:
	.4byte gPartyState
.L_02008098:
	.4byte 0x00000127
.L_0200809c:
	.4byte Data_02006530
.L_020080a0:
	.4byte 0x00000125
.L_020080a4:
	.4byte Data_02006a0c
.L_020080a8:
	.4byte 0x00000124
.L_020080ac:
	.4byte Data_02006710
.L_020080b0:
	.4byte 0x00000126
.L_020080b4:
	.4byte Data_02006890
.L_020080b8:
	.4byte Data_02006518
	.section .text.x020080bc,"ax",%progbits
	.global Func_020000bc
	.thumb_func
Func_020000bc:
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #208
	lsls r2, r2, #4
	adds r2, #56
	adds r3, r3, r2
	movs r2, #1
	strb r2, [r3]
	bx lr
	.section .text.x020080d0,"ax",%progbits
	.global Func_020000d0
	.thumb_func
Func_020000d0:
	push {r5, r6, lr}
	ldr r3, .L_02008164
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	ldrh r2, [r0, #6]
	cmp r2, #0
	beq .L_020080ee
	movs r3, #128
	lsls r3, r3, #8
	cmp r2, r3
	bne .L_02008148
.L_020080ee:
	ldr r3, [r0, #8]
	asrs r5, r3, #20
	ldr r3, [r0, #16]
	asrs r6, r3, #20
	cmp r2, #0
	beq .L_020080fe
	subs r5, #1
	b .L_02008100
.L_020080fe:
	adds r5, #1
.L_02008100:
	movs r0, #10
	bl Object_GetById
	ldr r3, [r0, #8]
	asrs r3, r3, #20
	cmp r3, r5
	bne .L_02008116
	ldr r3, [r0, #16]
	asrs r3, r3, #20
	cmp r3, r6
	beq .L_02008148
.L_02008116:
	movs r0, #11
	bl Object_GetById
	ldr r3, [r0, #8]
	asrs r3, r3, #20
	cmp r3, r5
	bne .L_0200812c
	ldr r3, [r0, #16]
	asrs r3, r3, #20
	cmp r3, r6
	beq .L_02008148
.L_0200812c:
	movs r0, #12
	bl Object_GetById
	ldr r3, [r0, #8]
	asrs r3, r3, #20
	cmp r3, r5
	bne .L_02008142
	ldr r3, [r0, #16]
	asrs r3, r3, #20
	cmp r3, r6
	beq .L_02008148
.L_02008142:
	bl Func_02005600
	b .L_02008162
.L_02008148:
	ldr r3, .L_02008164
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Func_020055f8
	movs r3, #1
	negs r3, r3
	cmp r0, r3
	beq .L_02008162
	bl Func_02005688
.L_02008162:
	pop {r5, r6, pc}
.L_02008164:
	.4byte gPartyState
	.section .text.x02008168,"ax",%progbits
	.global Func_02000168
	.thumb_func
Func_02000168:
	push {lr}
	bl Func_02005600
	pop {pc}
	.section .text.x02008170,"ax",%progbits
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
	.section .text.x020081f4,"ax",%progbits
	.global Func_020001f4
	.thumb_func
Func_020001f4:
	push {r5, lr}
	adds r0, r1, #0
	bl Object_GetById
	adds r5, r0, #0
	ldr r3, [r5, #8]
	asrs r3, r3, #20
	cmp r3, #50
	bne .L_02008218
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #56
	bl GameFlag_SetBit
	adds r2, r5, #0
	adds r2, #85
	movs r3, #3
	strb r3, [r2]
.L_02008218:
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x0200821c,"ax",%progbits
	.global Func_0200021c
	.thumb_func
Func_0200021c:
	push {r5, lr}
	movs r0, #16
	bl Object_GetById
	adds r5, r0, #0
	bl Func_02005688
	ldr r3, [r5, #8]
	asrs r3, r3, #20
	cmp r3, #50
	bne .L_02008244
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #56
	bl GameFlag_SetBit
	adds r2, r5, #0
	adds r2, #85
	movs r3, #3
	strb r3, [r2]
.L_02008244:
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x02008248,"ax",%progbits
	.global Func_02000248
	.thumb_func
Func_02000248:
	push {lr}
	cmp r1, #10
	bne .L_0200825a
	movs r0, #10
	bl Object_GetById
	movs r3, #0
	adds r0, #85
	strb r3, [r0]
.L_0200825a:
	pop {pc}
	.section .text.x0200825c,"ax",%progbits
	.global Func_0200025c
	.thumb_func
Func_0200025c:
	push {lr}
	cmp r1, #16
	bne .L_0200826a
	movs r1, #16
	bl Func_020001f4
	b .L_0200827e
.L_0200826a:
	cmp r1, #10
	bne .L_0200827e
	movs r0, #10
	bl Object_GetById
	movs r3, #3
	adds r0, #85
	strb r3, [r0]
	bl Func_020002ec
.L_0200827e:
	pop {pc}
	.section .text.x02008280,"ax",%progbits
	.global Func_02000280
	.thumb_func
Func_02000280:
	push {r5, lr}
	adds r5, r1, #0
	adds r0, r5, #0
	bl Object_GetById
	ldr r3, .L_020082b0
	lsls r5, r5, #1
	subs r5, #36
	ldrsh r4, [r3, r5]
	adds r3, r0, #0
	adds r3, #100
	adds r0, #102
	movs r2, #0
	ldrsh r1, [r3, r2]
	movs r3, #0
	ldrsh r2, [r0, r3]
	ldr r3, .L_020082b4
	lsls r2, r2, #16
	lsls r1, r1, #16
	adds r2, r2, r3
	adds r0, r4, #0
	bl Func_02005598
	pop {r5, pc}
.L_020082b0:
	.4byte Data_02005b5c
.L_020082b4:
	.4byte 0xfff80000
	.section .text.x020082b8,"ax",%progbits
	.global Func_020002b8
	.thumb_func
Func_020002b8:
	push {r5, lr}
	adds r5, r1, #0
	adds r0, r5, #0
	bl Object_GetById
	ldr r3, .L_020082e8
	lsls r5, r5, #1
	ldr r2, [r0, #8]
	subs r5, #36
	adds r0, #100
	ldrsh r1, [r3, r5]
	ldrh r3, [r0]
	asrs r2, r2, #20
	lsls r3, r3, #16
	asrs r3, r3, #20
	cmp r2, r3
	bne .L_020082e4
	adds r0, r1, #0
	movs r2, #0
	movs r1, #0
	bl Func_02005598
.L_020082e4:
	pop {r5, pc}
	.2byte 0x0000
.L_020082e8:
	.4byte Data_02005b5c
	.section .text.x020082ec,"ax",%progbits
	.global Func_020002ec
	.thumb_func
Func_020002ec:
	push {r5, lr}
	sub sp, #8
	movs r3, #38
	str r3, [sp, #0]
	movs r5, #72
	movs r0, #53
	movs r1, #72
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl Func_020054e8
	movs r3, #43
	str r3, [sp, #0]
	movs r0, #53
	movs r1, #72
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl Func_020054e8
	movs r3, #47
	movs r2, #8
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #53
	movs r1, #8
	movs r2, #1
	movs r3, #1
	bl Func_020054e8
	movs r5, #10
.L_0200832c:
	adds r0, r5, #0
	bl Object_GetById
	ldr r3, [r0, #8]
	asrs r2, r3, #20
	ldr r3, [r0, #16]
	asrs r0, r3, #20
	cmp r2, #38
	bne .L_02008354
	cmp r0, #8
	bne .L_02008354
	movs r3, #72
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r0, #54
	movs r1, #72
	movs r2, #1
	movs r3, #1
	bl Func_020054e8
.L_02008354:
	adds r5, #1
	cmp r5, #12
	ble .L_0200832c
	movs r5, #10
.L_0200835c:
	adds r0, r5, #0
	bl Object_GetById
	ldr r3, [r0, #8]
	asrs r2, r3, #20
	ldr r3, [r0, #16]
	asrs r0, r3, #20
	cmp r2, #43
	bne .L_02008384
	cmp r0, #8
	bne .L_02008384
	movs r3, #72
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r0, #54
	movs r1, #72
	movs r2, #1
	movs r3, #1
	bl Func_020054e8
.L_02008384:
	adds r5, #1
	cmp r5, #12
	ble .L_0200835c
	movs r0, #10
	bl Object_GetById
	ldr r3, [r0, #8]
	asrs r2, r3, #20
	ldr r3, [r0, #16]
	asrs r0, r3, #20
	cmp r2, #47
	bne .L_020083b0
	cmp r0, #8
	bne .L_020083b0
	str r2, [sp, #0]
	str r0, [sp, #4]
	movs r1, #8
	movs r0, #54
	movs r2, #1
	movs r3, #1
	bl Func_020054e8
.L_020083b0:
	add sp, #8
	pop {r5, pc}
	.section .text.x020083b4,"ax",%progbits
	.global Func_020003b4
	.thumb_func
Func_020003b4:
	push {lr}
	movs r0, #15
	bl Object_GetById
	ldr r3, [r0, #16]
	asrs r3, r3, #20
	cmp r3, #14
	bne .L_020083d6
	movs r1, #65
	movs r0, #0
	bl Func_020027ac
	movs r0, #2
	movs r1, #19
	bl Func_020027ac
	b .L_020083e6
.L_020083d6:
	movs r1, #17
	movs r0, #0
	bl Func_020027ac
	movs r0, #2
	movs r1, #67
	bl Func_020027ac
.L_020083e6:
	movs r0, #16
	bl Object_GetById
	ldr r3, [r0, #8]
	asrs r3, r3, #20
	cmp r3, #6
	bne .L_02008406
	movs r1, #65
	movs r0, #1
	bl Func_020027ac
	movs r0, #3
	movs r1, #3
	bl Func_020027ac
	b .L_02008416
.L_02008406:
	movs r1, #49
	movs r0, #1
	bl Func_020027ac
	movs r0, #3
	movs r1, #67
	bl Func_020027ac
.L_02008416:
	pop {pc}
	.section .text.x02008418,"ax",%progbits
	.global Func_02000418
	.thumb_func
Func_02000418:
	push {r5, lr}
	sub sp, #32
	add r5, sp, #8
	adds r0, r5, #0
	bl Func_02004a8c
	cmp r0, #0
	beq .L_02008440
	mov r2, sp
	add r3, sp, #24
	ldmia r3!, {r0, r1}
	stmia r2!, {r0, r1}
	ldr r0, [r5]
	ldr r1, [r5, #4]
	ldr r2, [r5, #8]
	ldr r3, [r5, #12]
	bl Func_02004d10
	bl Func_020003b4
.L_02008440:
	add sp, #32
	pop {r5, pc}
	.section .text.x02008444,"ax",%progbits
	.global Func_02000444
	.thumb_func
Func_02000444:
	push {r5, lr}
	sub sp, #8
	cmp r0, #1
	bne .L_0200847c
	movs r3, #10
	str r3, [sp, #0]
	movs r5, #50
	movs r0, #29
	movs r1, #56
	movs r2, #3
	movs r3, #4
	str r5, [sp, #4]
	bl Func_020054e0
	movs r3, #74
	str r3, [sp, #0]
	movs r0, #93
	movs r1, #56
	movs r2, #3
	movs r3, #4
	str r5, [sp, #4]
	bl Func_020054f0
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #68
	bl GameFlag_SetBit
.L_0200847c:
	add sp, #8
	pop {r5, pc}
	.section .text.x02008480,"ax",%progbits
	.global Func_02000480
	.thumb_func
Func_02000480:
	push {r5, lr}
	sub sp, #8
	cmp r0, #1
	bne .L_020084b8
	movs r3, #18
	str r3, [sp, #0]
	movs r5, #50
	movs r0, #29
	movs r1, #56
	movs r2, #3
	movs r3, #4
	str r5, [sp, #4]
	bl Func_020054e0
	movs r3, #82
	str r3, [sp, #0]
	movs r0, #93
	movs r1, #56
	movs r2, #3
	movs r3, #4
	str r5, [sp, #4]
	bl Func_020054f0
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #69
	bl GameFlag_SetBit
.L_020084b8:
	add sp, #8
	pop {r5, pc}
	.section .text.x020084bc,"ax",%progbits
	.global Func_020004bc
	.thumb_func
Func_020004bc:
	push {r5, lr}
	sub sp, #8
	cmp r0, #1
	bne .L_020084f4
	movs r3, #14
	str r3, [sp, #0]
	movs r5, #50
	movs r0, #29
	movs r1, #56
	movs r2, #3
	movs r3, #4
	str r5, [sp, #4]
	bl Func_020054e0
	movs r3, #78
	str r3, [sp, #0]
	movs r0, #93
	movs r1, #56
	movs r2, #3
	movs r3, #4
	str r5, [sp, #4]
	bl Func_020054f0
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #70
	bl GameFlag_SetBit
.L_020084f4:
	add sp, #8
	pop {r5, pc}
	.section .text.x020084f8,"ax",%progbits
	.global Func_020004f8
	.thumb_func
Func_020004f8:
	push {lr}
	movs r0, #146
	lsls r0, r0, #2
	bl Func_020056c0
	movs r1, #3
	movs r0, #0
	bl Func_020027ac
	movs r0, #136
	lsls r0, r0, #2
	bl GameFlag_SetBit
	pop {pc}
	.section .text.x02008514,"ax",%progbits
	.global Func_02000514
	.thumb_func
Func_02000514:
	push {r5, lr}
	movs r0, #9
	bl Object_GetById
	movs r2, #0
	adds r5, r0, #0
	movs r1, #0
	movs r0, #10
	bl Func_02005598
	adds r3, r5, #0
	adds r3, #100
	adds r5, #102
	movs r2, #0
	ldrsh r1, [r3, r2]
	movs r3, #0
	ldrsh r2, [r5, r3]
	lsls r1, r1, #16
	lsls r2, r2, #16
	movs r0, #9
	bl Func_02005598
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #66
	bl GameFlag_SetBit
	movs r0, #136
	bl Func_020056c0
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x02008554,"ax",%progbits
	.global Func_02000554
	.thumb_func
Func_02000554:
	push {lr}
	bl Func_02001448
	pop {pc}
	.section .text.x0200855c,"ax",%progbits
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
	bl ObjectDispatch_SetSingleChildField26
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
	.4byte Data_02005b68
.L_02008668:
	.4byte Data_02005bac
.L_0200866c:
	.4byte IwramMulQ16
	.section .text.x02008670,"ax",%progbits
	.global Func_02000670
	.thumb_func
Func_02000670:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r1, #179
	lsls r1, r1, #1
	adds r2, r3, r1
	movs r0, #131
	movs r3, #0
	strh r3, [r2]
	lsls r0, r0, #1
	bl GameFlag_ClearBit
	movs r0, #130
	lsls r0, r0, #1
	bl GameFlag_ClearBit
	bl Func_02005560
	movs r0, #0
	bl Func_02005648
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #10
	lsls r1, r1, #7
	bl Func_020055d0
	movs r0, #248
	movs r1, #1
	movs r2, #178
	lsls r2, r2, #18
	movs r3, #1
	negs r1, r1
	lsls r0, r0, #16
	bl Motion_CamBounds
	bl Func_020055e0
	movs r0, #20
	bl WaitFrames
	movs r0, #160
	movs r1, #160
	lsls r0, r0, #11
	lsls r1, r1, #8
	bl Func_020055d0
	movs r0, #248
	movs r1, #1
	movs r2, #218
	negs r1, r1
	lsls r2, r2, #18
	movs r3, #1
	lsls r0, r0, #16
	bl Motion_CamBounds
	bl Func_020055e0
	movs r0, #120
	bl Battle_WaitMode0
	bl Func_02005568
	pop {pc}
	.2byte 0x0000
	.section .text.x020086f4,"ax",%progbits
	.global Func_020006f4
	.thumb_func
Func_020006f4:
	push {r5, r6, lr}
	adds r6, r0, #0
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #70
	sub sp, #8
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008750
	movs r3, #14
	movs r5, #50
	str r3, [sp, #0]
	movs r0, #29
	movs r1, #60
	movs r2, #3
	movs r3, #4
	str r5, [sp, #4]
	bl Func_020054e0
	movs r3, #78
	str r3, [sp, #0]
	movs r0, #93
	movs r1, #60
	movs r2, #3
	movs r3, #4
	str r5, [sp, #4]
	bl Func_020054f0
	cmp r6, #0
	beq .L_02008746
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #67
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008746
	movs r0, #14
	bl Func_0200055c
.L_02008746:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #67
	bl GameFlag_SetBit
.L_02008750:
	add sp, #8
	pop {r5, r6, pc}
	.section .text.x02008754,"ax",%progbits
	.global Func_02000754
	.thumb_func
Func_02000754:
	push {lr}
	movs r0, #2
	bl Func_020055f0
	pop {pc}
	.2byte 0x0000
	.section .text.x02008760,"ax",%progbits
	.global Func_02000760
	.thumb_func
Func_02000760:
	push {lr}
	movs r0, #3
	bl Func_020055f0
	pop {pc}
	.2byte 0x0000
	.section .text.x0200876c,"ax",%progbits
	.global Func_0200076c
	.thumb_func
Func_0200076c:
	push {lr}
	movs r0, #4
	bl Func_020055f0
	pop {pc}
	.2byte 0x0000
	.section .text.x02008778,"ax",%progbits
	.global Func_02000778
	.thumb_func
Func_02000778:
	push {lr}
	movs r0, #5
	bl Func_020055f0
	pop {pc}
	.2byte 0x0000
	.section .text.x02008784,"ax",%progbits
	.global Func_02000784
	.thumb_func
Func_02000784:
	push {lr}
	movs r0, #6
	bl Func_020055f0
	pop {pc}
	.2byte 0x0000
	.section .text.x02008790,"ax",%progbits
	.global Func_02000790
	.thumb_func
Func_02000790:
	push {lr}
	movs r0, #7
	bl Func_020055f0
	pop {pc}
	.2byte 0x0000
	.section .text.x0200879c,"ax",%progbits
	.global Func_0200079c
	.thumb_func
Func_0200079c:
	push {lr}
	movs r0, #8
	bl Func_020055f0
	pop {pc}
	.2byte 0x0000
	.section .text.x020087a8,"ax",%progbits
	.global Func_020007a8
	.thumb_func
Func_020007a8:
	push {lr}
	movs r0, #9
	bl Func_020055f0
	pop {pc}
	.2byte 0x0000
	.section .text.x020087b4,"ax",%progbits
	.global Func_020007b4
	.thumb_func
Func_020007b4:
	push {lr}
	movs r0, #10
	bl Func_020055f0
	pop {pc}
	.2byte 0x0000
	.section .text.x020087c0,"ax",%progbits
	.global Func_020007c0
	.thumb_func
Func_020007c0:
	push {lr}
	movs r0, #11
	bl Func_020055f0
	pop {pc}
	.2byte 0x0000
	.section .text.x020087cc,"ax",%progbits
	.global Func_020007cc
	.thumb_func
Func_020007cc:
	push {lr}
	movs r0, #12
	bl Func_020055f0
	pop {pc}
	.2byte 0x0000
	.section .text.x020087d8,"ax",%progbits
	.global Func_020007d8
	.thumb_func
Func_020007d8:
	push {lr}
	movs r0, #13
	bl Func_020055f0
	pop {pc}
	.2byte 0x0000
	.section .text.x020087e4,"ax",%progbits
	.global Func_020007e4
	.thumb_func
Func_020007e4:
	push {lr}
	movs r0, #14
	bl Func_020055f0
	pop {pc}
	.2byte 0x0000
	.section .text.x020087f0,"ax",%progbits
	.global Func_020007f0
	.thumb_func
Func_020007f0:
	push {r5, r6, r7, lr}
	adds r7, r1, #0
	adds r0, r7, #0
	sub sp, #8
	bl Object_GetById
	adds r5, r0, #0
	ldr r3, [r5, #8]
	asrs r3, r3, #20
	cmp r3, #13
	bne .L_0200889a
	adds r2, r5, #0
	adds r2, #85
	movs r3, #3
	strb r3, [r2]
	adds r3, r5, #0
	adds r3, #34
	ldrb r0, [r3]
	ldr r1, [r5, #8]
	ldr r2, [r5, #16]
	bl Map_GetTerrainHeight
	ldr r3, [r5, #20]
	cmp r3, r0
	beq .L_0200884c
	movs r0, #2
	bl WaitFrames
	ldr r2, [r5, #12]
	ldr r3, [r5, #20]
	movs r6, #0
	cmp r2, r3
	ble .L_02008846
.L_02008832:
	movs r0, #1
	adds r6, #1
	bl WaitFrames
	cmp r6, #29
	bgt .L_02008846
	ldr r2, [r5, #12]
	ldr r3, [r5, #20]
	cmp r2, r3
	bgt .L_02008832
.L_02008846:
	movs r0, #133
	bl Func_020056c0
.L_0200884c:
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
	movs r1, #88
	movs r2, #7
	movs r3, #1
	movs r0, #7
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl Func_020054e8
	movs r0, #167
	bl Func_020056c0
	adds r0, r7, #0
	movs r1, #0
	movs r2, #0
	bl Func_02005598
	movs r0, #214
	lsls r0, r0, #2
	bl GameFlag_SetBit
.L_0200889a:
	add sp, #8
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x020088a0,"ax",%progbits
	.global Func_020008a0
	.thumb_func
Func_020008a0:
	push {lr}
	adds r0, r1, #0
	bl Object_GetById
	ldr r3, [r0, #8]
	asrs r3, r3, #20
	cmp r3, #7
	bne .L_020088ba
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #55
	bl GameFlag_SetBit
.L_020088ba:
	pop {pc}
	.section .text.x020088bc,"ax",%progbits
	.global Func_020008bc
	.thumb_func
Func_020008bc:
	push {lr}
	sub sp, #8
	movs r3, #17
	movs r2, #13
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #17
	movs r1, #24
	movs r2, #3
	movs r3, #1
	bl Func_020054e0
	movs r3, #18
	movs r2, #77
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #20
	movs r1, #24
	movs r2, #1
	movs r3, #1
	bl Func_020054e8
	movs r3, #54
	movs r2, #20
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #62
	movs r1, #20
	movs r2, #1
	movs r3, #2
	bl Func_020054e0
	add sp, #8
	pop {pc}
	.section .text.x02008900,"ax",%progbits
	.global Func_02000900
	.thumb_func
Func_02000900:
	push {lr}
	sub sp, #8
	movs r3, #17
	movs r2, #13
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #17
	movs r1, #25
	movs r2, #3
	movs r3, #1
	bl Func_020054e0
	movs r3, #18
	movs r2, #77
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #20
	movs r1, #25
	movs r2, #1
	movs r3, #1
	bl Func_020054e8
	movs r3, #54
	movs r2, #20
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #62
	movs r1, #22
	movs r2, #1
	movs r3, #2
	bl Func_020054e0
	add sp, #8
	pop {pc}
	.section .text.x02008944,"ax",%progbits
	.global Func_02000944
	.thumb_func
Func_02000944:
	push {r5, r6, r7, lr}
	ldr r5, .L_020089dc
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
	ldr r0, [r5]
	bl Object_GetById
	adds r6, r0, #0
	bl Func_02005608
	adds r0, r6, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	ldr r0, [r5]
	movs r1, #28
	bl Object_SetModeById
	movs r0, #16
	bl Battle_WaitMode0
	movs r5, #0
.L_02008972:
	cmp r5, #5
	bne .L_0200897c
	movs r0, #204
	bl Func_020056c0
.L_0200897c:
	adds r3, r6, #0
	adds r3, #85
	movs r7, #0
	strb r7, [r3]
	ldr r2, .L_020089e0
	ldr r3, [r6, #24]
	movs r0, #1
	adds r3, r3, r2
	str r3, [r6, #24]
	ldr r2, .L_020089e4
	ldr r3, [r6, #28]
	adds r5, #1
	adds r3, r3, r2
	str r3, [r6, #28]
	ldr r2, .L_020089e8
	ldr r3, [r6, #12]
	adds r3, r3, r2
	str r3, [r6, #12]
	bl WaitFrames
	cmp r5, #39
	ble .L_02008972
	ldr r3, .L_020089dc
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	adds r0, #84
	strb r7, [r0]
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
	movs r0, #16
	bl Func_020055f0
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_020089dc:
	.4byte gPartyState
.L_020089e0:
	.4byte 0xfffffc00
.L_020089e4:
	.4byte 0xfffffd00
.L_020089e8:
	.4byte 0xffff6667
	.section .text.x020089ec,"ax",%progbits
	.global Func_020009ec
	.thumb_func
Func_020009ec:
	push {lr}
	movs r1, #65
	movs r0, #2
	bl Func_020027ac
	movs r1, #65
	movs r0, #1
	bl Func_020027ac
	movs r1, #65
	movs r0, #0
	bl Func_020027ac
	movs r0, #212
	lsls r0, r0, #2
	bl GameFlag_ClearBit
	movs r0, #10
	bl Object_GetById
	ldr r3, [r0, #16]
	asrs r3, r3, #20
	cmp r3, #15
	beq .L_02008a34
	movs r0, #9
	bl Object_GetById
	ldr r3, [r0, #16]
	asrs r3, r3, #20
	cmp r3, #15
	beq .L_02008a34
	movs r0, #1
	movs r1, #33
	bl Func_020027ac
	b .L_02008aa8
.L_02008a34:
	movs r0, #9
	bl Object_GetById
	ldr r3, [r0, #16]
	asrs r3, r3, #20
	cmp r3, #13
	beq .L_02008a4c
	movs r0, #0
	movs r1, #17
	bl Func_020027ac
	b .L_02008aa8
.L_02008a4c:
	movs r0, #8
	bl Object_GetById
	ldr r3, [r0, #8]
	asrs r3, r3, #20
	cmp r3, #37
	beq .L_02008a64
	movs r0, #2
	movs r1, #49
	bl Func_020027ac
	b .L_02008aa8
.L_02008a64:
	movs r0, #212
	lsls r0, r0, #2
	bl GameFlag_SetBit
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #81
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008aa8
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #10
	lsls r1, r1, #7
	bl Func_020055d0
	movs r0, #154
	movs r1, #1
	movs r2, #192
	negs r1, r1
	lsls r2, r2, #13
	movs r3, #1
	lsls r0, r0, #18
	bl Motion_CamBounds
	movs r0, #60
	bl Battle_WaitMode0
	bl Func_02005660
	movs r0, #9
	bl Func_020055f0
.L_02008aa8:
	pop {pc}
	.2byte 0x0000
	.section .text.x02008aac,"ax",%progbits
	.global Func_02000aac
	.thumb_func
Func_02000aac:
	push {lr}
	movs r1, #3
	movs r0, #3
	bl Func_020027ac
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #9
	lsls r1, r1, #6
	bl Func_020055d0
	movs r0, #184
	movs r1, #1
	movs r2, #140
	lsls r0, r0, #16
	negs r1, r1
	lsls r2, r2, #18
	movs r3, #1
	bl Motion_CamBounds
	pop {pc}
	.2byte 0x0000
	.section .text.x02008ad8,"ax",%progbits
	.global Func_02000ad8
	.thumb_func
Func_02000ad8:
	push {r5, lr}
	movs r3, #192
	movs r0, #131
	lsls r3, r3, #18
	lsls r0, r0, #1
	ldr r5, [r3, #108]
	bl GameFlag_SetBit
	bl Func_02005560
	movs r0, #0
	bl Func_02005648
	movs r3, #179
	lsls r3, r3, #1
	adds r2, r5, r3
	movs r0, #131
	movs r3, #0
	strh r3, [r2]
	lsls r0, r0, #1
	bl GameFlag_ClearBit
	movs r0, #130
	lsls r0, r0, #1
	bl GameFlag_ClearBit
	movs r0, #60
	bl Battle_WaitMode0
	movs r0, #192
	movs r1, #192
	lsls r0, r0, #10
	lsls r1, r1, #7
	bl Func_020055d0
	movs r0, #184
	movs r1, #1
	movs r2, #186
	negs r1, r1
	lsls r2, r2, #18
	movs r3, #1
	lsls r0, r0, #16
	bl Motion_CamBounds
	bl Func_020055e0
	movs r0, #60
	bl Battle_WaitMode0
	bl Func_02005568
	pop {r5, pc}
	.section .text.x02008b40,"ax",%progbits
	.global Func_02000b40
	.thumb_func
Func_02000b40:
	push {r5, r6, r7, lr}
	sub sp, #44
	add r5, sp, #20
	adds r0, r5, #0
	bl Func_02004a8c
	cmp r0, #0
	beq .L_02008be6
	mov r3, sp
	add r2, sp, #36
	ldmia r2!, {r0, r1}
	stmia r3!, {r0, r1}
	ldr r0, [r5]
	ldr r1, [r5, #4]
	ldr r2, [r5, #8]
	ldr r3, [r5, #12]
	bl Func_02004d10
	bl Func_020009ec
	movs r0, #0
	bl Func_020027dc
	cmp r0, #64
	beq .L_02008ba8
	movs r0, #145
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008ba8
	movs r0, #12
	bl Object_GetById
	movs r3, #0
	str r3, [r0, #108]
	movs r1, #0
	bl Animation_ApplyChildValues
	movs r0, #12
	movs r1, #6
	bl Object_SetModeById
	movs r0, #145
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_ClearBit
	movs r0, #12
	bl Func_02000cb8
.L_02008ba8:
	movs r0, #2
	bl Func_020027dc
	cmp r0, #64
	beq .L_02008c68
	movs r0, #136
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008c68
	movs r0, #11
	bl Object_GetById
	movs r3, #0
	str r3, [r0, #108]
	movs r1, #0
	bl Animation_ApplyChildValues
	movs r0, #11
	movs r1, #6
	bl Object_SetModeById
	movs r0, #136
	lsls r0, r0, #2
	bl GameFlag_ClearBit
	movs r0, #11
	bl Func_02000cb8
	b .L_02008c68
.L_02008be6:
	ldr r3, .L_02008c6c
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r3, r2
	ldr r0, [r5]
	bl Object_GetById
	ldr r3, .L_02008c70
	ldr r1, .L_02008c74
	ldr r3, [r3]
	movs r2, #15
	lsrs r3, r3, #4
	ands r3, r2
	lsls r3, r3, #1
	ldrsh r6, [r1, r3]
	movs r1, #1
	negs r1, r1
	cmp r6, r1
	beq .L_02008c68
	movs r3, #192
	lsls r3, r3, #18
	ldr r0, [r5]
	ldr r7, [r3, #32]
	bl Object_GetById
	ldr r1, .L_02008c78
	ldr r3, [r0, #8]
	movs r2, #128
	lsls r2, r2, #12
	ands r3, r1
	add r5, sp, #8
	adds r3, r3, r2
	str r3, [r5]
	ldr r3, [r0, #12]
	str r3, [r5, #4]
	ldr r3, [r0, #16]
	movs r0, #128
	ands r3, r1
	adds r3, r3, r2
	str r3, [r5, #8]
	lsls r0, r0, #13
	adds r1, r6, #0
	adds r2, r5, #0
	bl Vector_AddPolarOffsetFar
	ldr r3, [r5]
	ldr r2, .L_02008c7c
	asrs r0, r3, #20
	ldr r3, [r5, #8]
	asrs r1, r3, #20
	cmp r7, #0
	beq .L_02008c56
	movs r2, #212
	lsls r2, r2, #1
	adds r3, r7, r2
	ldr r2, [r3]
.L_02008c56:
	lsls r3, r1, #7
	adds r3, r0, r3
	lsls r3, r3, #2
	adds r2, r2, r3
	ldrb r3, [r2, #2]
	cmp r3, #255
	beq .L_02008c68
	bl Func_02005600
.L_02008c68:
	add sp, #44
	pop {r5, r6, r7, pc}
.L_02008c6c:
	.4byte gPartyState
.L_02008c70:
	.4byte gInput
.L_02008c74:
	.4byte Data_02005f18
.L_02008c78:
	.4byte 0xfff00000
.L_02008c7c:
	.4byte gMapCellBuffer
	.section .text.x02008c80,"ax",%progbits
	.global Func_02000c80
	.thumb_func
Func_02000c80:
	push {r5, lr}
	adds r5, r0, #0
	bl Object_GetById
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #32]
	ldr r2, [r0, #16]
	ldr r3, [r0, #8]
	movs r0, #212
	lsls r0, r0, #1
	adds r1, r1, r0
	asrs r2, r2, #20
	lsls r2, r2, #7
	ldr r1, [r1]
	asrs r3, r3, #20
	adds r3, r3, r2
	lsls r3, r3, #2
	adds r1, r1, r3
	movs r3, #255
	strb r3, [r1, #2]
	adds r0, r5, #0
	bl Object_GetById
	movs r3, #0
	adds r0, #35
	strb r3, [r0]
	pop {r5, pc}
	.section .text.x02008cb8,"ax",%progbits
	.global Func_02000cb8
	.thumb_func
Func_02000cb8:
	push {r5, lr}
	adds r5, r0, #0
	bl Object_GetById
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #32]
	ldr r2, [r0, #16]
	ldr r3, [r0, #8]
	movs r0, #212
	lsls r0, r0, #1
	adds r1, r1, r0
	asrs r2, r2, #20
	lsls r2, r2, #7
	ldr r1, [r1]
	asrs r3, r3, #20
	adds r3, r3, r2
	lsls r3, r3, #2
	adds r1, r1, r3
	movs r3, #0
	strb r3, [r1, #2]
	adds r0, r5, #0
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #2
	orrs r3, r2
	strb r3, [r0]
	pop {r5, pc}
	.section .text.x02008cf4,"ax",%progbits
	.global Func_02000cf4
	.thumb_func
Func_02000cf4:
	push {r5, lr}
	adds r5, r1, #0
	adds r0, r5, #0
	bl Func_02000c80
	movs r0, #136
	lsls r0, r0, #2
	bl GameFlag_SetBit
	movs r0, #2
	bl Func_020027dc
	cmp r0, #64
	beq .L_02008d36
	adds r0, r5, #0
	bl Object_GetById
	movs r3, #0
	str r3, [r0, #108]
	movs r1, #0
	bl Animation_ApplyChildValues
	adds r0, r5, #0
	movs r1, #6
	bl Object_SetModeById
	movs r0, #136
	lsls r0, r0, #2
	bl GameFlag_ClearBit
	adds r0, r5, #0
	bl Func_02000cb8
.L_02008d36:
	pop {r5, pc}
	.section .text.x02008d38,"ax",%progbits
	.global Func_02000d38
	.thumb_func
Func_02000d38:
	push {r5, lr}
	adds r5, r1, #0
	adds r0, r5, #0
	bl Func_02000c80
	movs r0, #145
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
	movs r0, #0
	bl Func_020027dc
	cmp r0, #64
	beq .L_02008d7e
	adds r0, r5, #0
	bl Object_GetById
	movs r3, #0
	str r3, [r0, #108]
	movs r1, #0
	bl Animation_ApplyChildValues
	adds r0, r5, #0
	movs r1, #6
	bl Object_SetModeById
	movs r0, #145
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_ClearBit
	adds r0, r5, #0
	bl Func_02000cb8
.L_02008d7e:
	pop {r5, pc}
	.section .text.x02008d80,"ax",%progbits
	.global Func_02000d80
	.thumb_func
Func_02000d80:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	movs r0, #164
	lsls r0, r0, #4
	sub sp, #8
	bl GameFlag_SetBit
	movs r3, #38
	str r3, [sp, #4]
	movs r5, #10
	mov r8, r3
	movs r0, #24
	movs r1, #38
	movs r2, #3
	movs r3, #9
	str r5, [sp, #0]
	bl Func_020054e8
	movs r3, #36
	str r3, [sp, #4]
	movs r6, #74
	movs r0, #64
	movs r1, #64
	movs r2, #3
	movs r3, #9
	str r6, [sp, #0]
	bl Func_020054f0
	movs r3, #100
	str r3, [sp, #4]
	movs r0, #64
	movs r1, #64
	movs r2, #3
	movs r3, #9
	str r5, [sp, #0]
	bl Func_020054f0
	mov r3, r8
	str r3, [sp, #4]
	movs r0, #64
	movs r1, #64
	movs r2, #3
	movs r3, #9
	str r6, [sp, #0]
	bl Func_020054e8
	movs r3, #102
	str r3, [sp, #4]
	movs r0, #64
	movs r1, #64
	movs r2, #3
	movs r3, #9
	str r5, [sp, #0]
	bl Func_020054e8
	add sp, #8
	pop {r3}
	mov r8, r3
	pop {r5, r6, pc}
	.section .text.x02008df8,"ax",%progbits
	.global Func_02000df8
	.thumb_func
Func_02000df8:
	push {lr}
	bl Func_020043ec
	bl Func_02002ce0
	pop {pc}
	.section .text.x02008e04,"ax",%progbits
	.global Func_02000e04
	.thumb_func
Func_02000e04:
	push {lr}
	ldr r3, .L_02008e40
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008e44
	cmp r2, r3
	bne .L_02008e1c
	ldr r0, .L_02008e48
	b .L_02008e3c
.L_02008e1c:
	ldr r3, .L_02008e4c
	cmp r2, r3
	bne .L_02008e26
	ldr r0, .L_02008e50
	b .L_02008e3c
.L_02008e26:
	ldr r3, .L_02008e54
	cmp r2, r3
	bne .L_02008e30
	ldr r0, .L_02008e58
	b .L_02008e3c
.L_02008e30:
	ldr r3, .L_02008e5c
	cmp r2, r3
	bne .L_02008e3a
	ldr r0, .L_02008e60
	b .L_02008e3c
.L_02008e3a:
	ldr r0, .L_02008e64
.L_02008e3c:
	pop {pc}
	.2byte 0x0000
.L_02008e40:
	.4byte gPartyState
.L_02008e44:
	.4byte 0x00000127
.L_02008e48:
	.4byte Data_02006b98
.L_02008e4c:
	.4byte 0x00000125
.L_02008e50:
	.4byte Data_02006f64
.L_02008e54:
	.4byte 0x00000124
.L_02008e58:
	.4byte Data_02006d78
.L_02008e5c:
	.4byte 0x00000126
.L_02008e60:
	.4byte Data_02006e38
.L_02008e64:
	.4byte Data_02006b8c
	.section .text.x02008e68,"ax",%progbits
	.global Func_02000e68
	.thumb_func
Func_02000e68:
	push {lr}
	bl Func_02002cfc
	cmp r0, #0
	beq .L_02008e78
	movs r0, #1
	bl Func_02004700
.L_02008e78:
	pop {pc}
	.2byte 0x0000
	.section .text.x02008e7c,"ax",%progbits
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
	bl Motion_CamBounds
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
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	movs r0, #30
	bl Battle_WaitMode0
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
	bl Battle_WaitMode0
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
	.4byte Data_02005bf0
.L_02009200:
	.4byte Data_02005bf6
.L_02009204:
	.4byte Func_02000514
.L_02009208:
	.4byte Data_02005c0e
.L_0200920c:
	.4byte Data_02005bfc
.L_02009210:
	.4byte 0x00000125
.L_02009214:
	.4byte Data_02005c26
.L_02009218:
	.4byte Data_02005c14
.L_0200921c:
	.4byte Func_02001468
.L_02009220:
	.4byte Func_02000e68
.L_02009224:
	.4byte 0x00000126
.L_02009228:
	.4byte Data_02005c32
.L_0200922c:
	.4byte Data_02005c2c
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
	.4byte Data_02005c38
.L_020093f0:
	.4byte Data_02005c46
.L_020093f4:
	.4byte gPartyState
.L_020093f8:
	.4byte 0x00000127
.L_020093fc:
	.4byte Data_02005c5a
.L_02009400:
	.4byte Data_02005c4c
.L_02009404:
	.4byte Data_02005c60
.L_02009408:
	.4byte Data_02005c6e
	.section .text.x0200940c,"ax",%progbits
	.global Func_0200140c
	.thumb_func
Func_0200140c:
	push {r5, lr}
	movs r0, #163
	lsls r0, r0, #4
	ldr r5, .L_0200943c
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009436
	movs r2, #253
	lsls r2, r2, #1
	adds r3, r5, r2
	movs r2, #0
	ldrsh r0, [r3, r2]
	ldr r3, .L_02009440
	ldr r2, .L_02009444
	movs r1, #160
	subs r3, r3, r2
	adds r0, r0, r3
	lsls r1, r1, #19
	bl Func_02005548
.L_02009436:
	movs r0, #0
	pop {r5, pc}
	.2byte 0x0000
.L_0200943c:
	.4byte gPartyState
.L_02009440:
	.4byte 0x00000121
.L_02009444:
	.4byte 0x0000010e
	.section .text.x02009448,"ax",%progbits
	.global Func_02001448
	.thumb_func
Func_02001448:
	push {lr}
	ldr r2, .L_02009464
	sub sp, #4
	movs r3, #128
	str r2, [sp, #0]
	lsls r3, r3, #7
	movs r1, #12
	movs r2, #13
	movs r0, #11
	bl Func_0200328c
	add sp, #4
	pop {pc}
	.2byte 0x0000
.L_02009464:
	.4byte Data_02005c84
	.section .text.x02009468,"ax",%progbits
	.global Func_02001468
	.thumb_func
Func_02001468:
	push {lr}
	ldr r2, .L_0200948c
	sub sp, #4
	movs r3, #128
	str r2, [sp, #0]
	lsls r3, r3, #7
	movs r2, #22
	movs r1, #21
	movs r0, #19
	bl Func_0200328c
	movs r1, #131
	movs r0, #3
	bl Func_020027ac
	add sp, #4
	pop {pc}
	.2byte 0x0000
.L_0200948c:
	.4byte Data_02005ce0
	.section .text.x02009490,"ax",%progbits
	.global Func_02001490
	.thumb_func
Func_02001490:
	push {lr}
	ldr r2, .L_020094ac
	sub sp, #4
	movs r3, #128
	str r2, [sp, #0]
	lsls r3, r3, #7
	movs r1, #15
	movs r2, #16
	movs r0, #14
	bl Func_0200328c
	add sp, #4
	pop {pc}
	.2byte 0x0000
.L_020094ac:
	.4byte Data_02005d3c
	.section .text.x020094b0,"ax",%progbits
	.global Func_020014b0
	.thumb_func
Func_020014b0:
	push {lr}
	movs r0, #20
	adds r0, #255
	bl GameFlag_SetBit
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #37
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020094d8
	movs r0, #98
	adds r0, #255
	bl GameFlag_SetBit
	movs r0, #162
	lsls r0, r0, #1
	bl GameFlag_SetBit
.L_020094d8:
	pop {pc}
	.2byte 0x0000
	.section .text.x020094dc,"ax",%progbits
	.global Func_020014dc
	.thumb_func
Func_020014dc:
	ldr r3, .L_020094e4
	str r0, [r3]
	bx lr
	.2byte 0x0000
.L_020094e4:
	.4byte Data_02007040
	.section .text.x020094e8,"ax",%progbits
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
	bl Engine_MathRemainder
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
	bl Engine_MathRemainder
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
	.4byte Data_0200703c
.L_02009598:
	.4byte Data_02007040
.L_0200959c:
	.4byte gOverlayArea + 0x7068
.L_020095a0:
	.4byte 0x05000184
	.section .text.x020095a4,"ax",%progbits
	.global Func_020015a4
	.thumb_func
Func_020015a4:
	push {r5, r6, lr}
	ldr r2, .L_02009600
	movs r3, #1
	adds r6, r0, #0
	str r3, [r2]
	cmp r6, #2
	beq .L_020095c8
	ldr r1, .L_02009604
	movs r2, #32
	ldr r0, .L_02009608
	ldr r5, .L_0200960c
	mov lr, r5
	.2byte 0xf800
	ldr r0, .L_02009610
	ldr r1, .L_02009614
	movs r2, #32
	mov lr, r5
	.2byte 0xf800
.L_020095c8:
	movs r0, #160
	lsls r0, r0, #4
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020095d8
	cmp r6, #1
	bne .L_020095ea
.L_020095d8:
	ldr r3, .L_02009618
	movs r2, #0
	movs r1, #144
	str r2, [r3]
	ldr r0, .L_0200961c
	lsls r1, r1, #3
	bl Scheduler_AddOrUpdateCallback
	b .L_020095fe
.L_020095ea:
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	ldr r0, .L_02009620
	ldr r1, .L_02009624
	adds r2, #2
	stmia r3!, {r0, r1, r2}
	subs r3, #12
.L_020095fe:
	pop {r5, r6, pc}
.L_02009600:
	.4byte Data_02007040
.L_02009604:
	.4byte 0x05000180
.L_02009608:
	.4byte gOverlayArea + 0x7068
.L_0200960c:
	.4byte IwramCopyWords
.L_02009610:
	.4byte gOverlayArea + 0x7088
.L_02009614:
	.4byte 0x050001a0
.L_02009618:
	.4byte Data_0200703c
.L_0200961c:
	.4byte Func_020014e8
.L_02009620:
	.4byte gOverlayArea + 0x708c
.L_02009624:
	.4byte 0x05000184
	.section .text.x02009628,"ax",%progbits
	.global Func_02001628
	.thumb_func
Func_02001628:
	push {r5, lr}
	bl Object_GetById
	adds r5, r0, #0
	adds r1, r5, #0
	adds r1, #85
	movs r3, #4
	strb r3, [r1]
	movs r2, #0
	ldr r3, [r5, #20]
	str r2, [r5, #68]
	movs r2, #128
	lsls r2, r2, #14
	adds r3, r3, r2
	str r3, [r5, #12]
	subs r1, #50
	ldrb r2, [r1]
	movs r3, #128
	orrs r3, r2
	strb r3, [r1]
	adds r3, r5, #0
	adds r3, #34
	ldr r1, [r5, #8]
	ldr r2, [r5, #16]
	ldrb r0, [r3]
	bl Map_GetTerrainHeight
	adds r3, r0, #0
	asrs r3, r3, #19
	ldr r0, [r5, #8]
	ldr r1, [r5, #16]
	adds r3, #6
	movs r2, #0
	bl Func_02005540
	ldr r0, [r5, #8]
	ldr r1, [r5, #16]
	movs r2, #0
	movs r3, #128
	bl Func_020056a8
	pop {r5, pc}
	.section .text.x0200967c,"ax",%progbits
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
	bl ObjectDispatch_SetSingleChildField26
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
	bl ObjectDispatch_SetSingleChildField26
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
	.4byte Data_0300122c
.L_02009794:
	bl Motion_CamBounds
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
	bl Object_AttachWorkTargetToObject
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
	.section .text.x02009868,"ax",%progbits
	.global Func_02001868
	.thumb_func
Func_02001868:
	push {lr}
	ldr r3, [r1]
	ldr r4, [r0]
	ldr r2, [r1, #8]
	subs r4, r4, r3
	ldr r3, [r0, #8]
	asrs r4, r4, #16
	subs r3, r3, r2
	asrs r3, r3, #16
	adds r2, r3, #0
	muls r2, r3
	adds r0, r4, #0
	muls r0, r4
	adds r3, r2, #0
	adds r0, r0, r3
	ldr r3, .L_02009890
	mov lr, r3
	.2byte 0xf800
	pop {pc}
	.2byte 0x0000
.L_02009890:
	.4byte IwramFillWords + 0x74
	.section .text.x02009894,"ax",%progbits
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
	bl ObjectDispatch_ApplyValueToChildren
	b .L_02009a3e
.L_020098fc:
	.4byte gPartyState
.L_02009900:
	adds r0, r6, #0
	movs r1, #16
	bl ObjectDispatch_ApplyValueToChildren
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
	.4byte Data_02007044
.L_02009a50:
	.4byte gOverlayArea + 0x7064
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
	bl ObjectDispatch_SetSingleChildField26
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
	bl Engine_MathDivide
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
	.4byte Data_0300122c
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
	.section .text.x02009d5c,"ax",%progbits
	.global Func_02001d5c
	.thumb_func
Func_02001d5c:
	push {lr}
	ldr r4, [r0, #8]
	ldr r3, [r1, #8]
	subs r2, r4, r3
	cmp r2, #0
	blt .L_02009d72
	movs r3, #128
	lsls r3, r3, #12
	cmp r2, r3
	blt .L_02009d7c
	b .L_02009dbc
.L_02009d72:
	movs r2, #128
	subs r3, r3, r4
	lsls r2, r2, #12
	cmp r3, r2
	bge .L_02009dbc
.L_02009d7c:
	ldr r4, [r0, #12]
	ldr r3, [r1, #12]
	subs r2, r4, r3
	cmp r2, #0
	blt .L_02009d90
	movs r3, #128
	lsls r3, r3, #12
	cmp r2, r3
	blt .L_02009d9a
	b .L_02009dbc
.L_02009d90:
	movs r2, #128
	subs r3, r3, r4
	lsls r2, r2, #12
	cmp r3, r2
	bge .L_02009dbc
.L_02009d9a:
	ldr r0, [r0, #16]
	ldr r1, [r1, #16]
	subs r2, r0, r1
	cmp r2, #0
	blt .L_02009dae
	movs r3, #128
	lsls r3, r3, #12
	cmp r2, r3
	blt .L_02009db8
	b .L_02009dbc
.L_02009dae:
	movs r2, #128
	subs r3, r1, r0
	lsls r2, r2, #12
	cmp r3, r2
	bge .L_02009dbc
.L_02009db8:
	movs r0, #1
	b .L_02009dbe
.L_02009dbc:
	movs r0, #0
.L_02009dbe:
	pop {pc}
	.section .text.x02009dc0,"ax",%progbits
	.global Func_02001dc0
	.thumb_func
Func_02001dc0:
	push {lr}
	ldr r4, [r0, #8]
	ldr r3, [r1, #8]
	subs r2, r4, r3
	cmp r2, #0
	blt .L_02009dd6
	movs r3, #192
	lsls r3, r3, #12
	cmp r2, r3
	blt .L_02009de0
	b .L_02009e12
.L_02009dd6:
	movs r2, #192
	subs r3, r3, r4
	lsls r2, r2, #12
	cmp r3, r2
	bge .L_02009e12
.L_02009de0:
	ldr r2, [r1, #12]
	ldr r3, [r0, #12]
	subs r3, r3, r2
	ldr r2, .L_02009e18
	adds r3, r3, r2
	ldr r2, .L_02009e1c
	cmp r3, r2
	bhi .L_02009e12
	ldr r0, [r0, #16]
	ldr r1, [r1, #16]
	subs r2, r0, r1
	cmp r2, #0
	blt .L_02009e04
	movs r3, #192
	lsls r3, r3, #12
	cmp r2, r3
	blt .L_02009e0e
	b .L_02009e12
.L_02009e04:
	movs r2, #192
	subs r3, r1, r0
	lsls r2, r2, #12
	cmp r3, r2
	bge .L_02009e12
.L_02009e0e:
	movs r0, #1
	b .L_02009e14
.L_02009e12:
	movs r0, #0
.L_02009e14:
	pop {pc}
	.2byte 0x0000
.L_02009e18:
	.4byte 0x0007ffff
.L_02009e1c:
	.4byte 0x001ffffe
	.section .text.x02009e20,"ax",%progbits
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
	bl Engine_MathDivide
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
	.4byte Data_020023c4 + 0x188
	.section .text.x02009f84,"ax",%progbits
	.global Func_02001f84
	.thumb_func
Func_02001f84:
	push {r5, r6, lr}
	adds r5, r0, #0
	adds r3, r5, #0
	movs r6, #0
	adds r3, #85
	strb r6, [r3]
	adds r3, #4
	strb r6, [r3]
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	ldr r1, .L_02009fb0
	adds r0, r5, #0
	bl Func_02005490
	adds r3, r5, #0
	adds r3, #100
	strh r6, [r3]
	str r6, [r5, #48]
	str r6, [r5, #52]
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02009fb0:
	.4byte Data_02005d8c
	.section .text.x02009fb4,"ax",%progbits
	.global Func_02001fb4
	.thumb_func
Func_02001fb4:
	push {r5, r6, r7, lr}
	ldr r3, [r0, #8]
	adds r6, r1, #0
	adds r7, r2, #0
	ldr r1, [r0]
	ldr r2, [r0, #4]
	movs r0, #183
	lsls r0, r0, #1
	bl Func_02005498
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02009ff4
	movs r3, #0
	str r3, [r5, #68]
	str r3, [r5, #72]
	str r6, [r5, #76]
	movs r1, #2
	bl Func_02001f84
	ldr r3, .L_02009ff8
	str r3, [r5, #108]
	cmp r7, #0
	beq .L_02009fec
	adds r2, r5, #0
	adds r2, #92
	movs r3, #2
	strb r3, [r2]
.L_02009fec:
	adds r0, r5, #0
	movs r1, #2
	bl Func_02005480
.L_02009ff4:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009ff8:
	.4byte Func_02001e20
	.section .text.x02009ffc,"ax",%progbits
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
	bl Engine_MathDivide
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
	.4byte Data_020023c4 + 0x188
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
	bl ObjectDispatch_SetSingleChildField26
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
	.4byte Data_02005d8c
	.section .text.x0200a1a0,"ax",%progbits
	.global Func_020021a0
	.thumb_func
Func_020021a0:
	push {r5, r6, r7, lr}
	ldr r3, [r0, #8]
	adds r6, r1, #0
	adds r7, r2, #0
	ldr r1, [r0]
	ldr r2, [r0, #4]
	movs r0, #183
	lsls r0, r0, #1
	bl Func_02005498
	adds r5, r0, #0
	cmp r5, #0
	beq .L_0200a1e2
	movs r3, #0
	str r3, [r5, #68]
	str r3, [r5, #72]
	negs r3, r6
	str r3, [r5, #76]
	movs r1, #3
	bl Func_0200215c
	ldr r3, .L_0200a1e4
	str r3, [r5, #108]
	cmp r7, #0
	beq .L_0200a1da
	adds r2, r5, #0
	adds r2, #92
	movs r3, #2
	strb r3, [r2]
.L_0200a1da:
	adds r0, r5, #0
	movs r1, #2
	bl Func_02005480
.L_0200a1e2:
	pop {r5, r6, r7, pc}
.L_0200a1e4:
	.4byte Func_02001e20
	.section .text.x0200a1e8,"ax",%progbits
	.global Func_020021e8
	.thumb_func
Func_020021e8:
	push {r5, r6, r7, lr}
	ldr r3, [r0, #8]
	adds r6, r1, #0
	adds r7, r2, #0
	ldr r1, [r0]
	ldr r2, [r0, #4]
	movs r0, #183
	lsls r0, r0, #1
	bl Func_02005498
	adds r5, r0, #0
	cmp r5, #0
	beq .L_0200a228
	movs r3, #0
	str r3, [r5, #68]
	str r3, [r5, #72]
	str r6, [r5, #76]
	movs r1, #3
	bl Func_0200215c
	ldr r3, .L_0200a22c
	str r3, [r5, #108]
	cmp r7, #0
	beq .L_0200a220
	adds r2, r5, #0
	adds r2, #92
	movs r3, #2
	strb r3, [r2]
.L_0200a220:
	adds r0, r5, #0
	movs r1, #2
	bl Func_02005480
.L_0200a228:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200a22c:
	.4byte Func_02001ffc
	.section .text.x0200a230,"ax",%progbits
	.global Func_02002230
	.thumb_func
Func_02002230:
	push {r5, r6, r7, lr}
	ldr r3, [r0, #8]
	adds r6, r1, #0
	adds r7, r2, #0
	ldr r1, [r0]
	ldr r2, [r0, #4]
	movs r0, #183
	lsls r0, r0, #1
	bl Func_02005498
	adds r5, r0, #0
	cmp r5, #0
	beq .L_0200a272
	movs r3, #0
	str r3, [r5, #68]
	str r3, [r5, #72]
	negs r3, r6
	str r3, [r5, #76]
	movs r1, #3
	bl Func_0200215c
	ldr r3, .L_0200a274
	str r3, [r5, #108]
	cmp r7, #0
	beq .L_0200a26a
	adds r2, r5, #0
	adds r2, #92
	movs r3, #2
	strb r3, [r2]
.L_0200a26a:
	adds r0, r5, #0
	movs r1, #2
	bl Func_02005480
.L_0200a272:
	pop {r5, r6, r7, pc}
.L_0200a274:
	.4byte Func_02001ffc
	.section .text.x0200a278,"ax",%progbits
	.global Func_02002278
	.thumb_func
Func_02002278:
	push {lr}
	ldr r4, [r0, #8]
	ldr r3, [r1, #8]
	subs r2, r4, r3
	cmp r2, #0
	blt .L_0200a28e
	movs r3, #192
	lsls r3, r3, #12
	cmp r2, r3
	blt .L_0200a298
	b .L_0200a2c8
.L_0200a28e:
	movs r2, #192
	subs r3, r3, r4
	lsls r2, r2, #12
	cmp r3, r2
	bge .L_0200a2c8
.L_0200a298:
	ldr r2, [r1, #12]
	ldr r3, [r0, #12]
	subs r3, r3, r2
	ldr r2, .L_0200a2cc
	subs r3, #1
	cmp r3, r2
	bhi .L_0200a2c8
	ldr r0, [r0, #16]
	ldr r1, [r1, #16]
	subs r2, r0, r1
	cmp r2, #0
	blt .L_0200a2ba
	movs r3, #128
	lsls r3, r3, #12
	cmp r2, r3
	blt .L_0200a2c4
	b .L_0200a2c8
.L_0200a2ba:
	movs r2, #128
	subs r3, r1, r0
	lsls r2, r2, #12
	cmp r3, r2
	bge .L_0200a2c8
.L_0200a2c4:
	movs r0, #1
	b .L_0200a2ca
.L_0200a2c8:
	movs r0, #0
.L_0200a2ca:
	pop {pc}
.L_0200a2cc:
	.4byte 0x000ffffe
	.section .text.x0200a2d0,"ax",%progbits
	.global Func_020022d0
	.thumb_func
Func_020022d0:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r3, .L_0200a368
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	adds r6, r0, #0
	ldr r0, [r3]
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
	mov r8, r0
	adds r3, r3, r7
	adds r0, r5, #0
	movs r1, #18
	str r3, [r6, #16]
	bl Engine_MathDivide
	subs r5, r5, r0
	str r5, [r6, #68]
	adds r3, r7, #0
	cmp r7, #0
	bge .L_0200a314
	adds r3, #15
.L_0200a314:
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
	ldr r2, [r3, #12]
	ldr r3, [r3, #20]
	cmp r2, r3
	bne .L_0200a362
	adds r0, r6, #0
	mov r1, r8
	bl Func_02002278
	cmp r0, #0
	beq .L_0200a362
	ldr r2, [r6, #76]
	ldr r3, [r6, #16]
	subs r3, r3, r2
	str r3, [r6, #16]
	ldr r3, [r6, #72]
	asrs r2, r2, #2
	adds r3, r3, r2
	str r3, [r6, #72]
	movs r3, #0
	str r3, [r6, #76]
.L_0200a362:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_0200a368:
	.4byte gPartyState
	.section .text.x0200a36c,"ax",%progbits
	.global Func_0200236c
	.thumb_func
Func_0200236c:
	push {r5, r6, lr}
	adds r5, r0, #0
	adds r3, r5, #0
	movs r6, #0
	adds r3, #85
	strb r6, [r3]
	adds r3, #4
	strb r6, [r3]
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	adds r0, r5, #0
	movs r1, #2
	bl Func_02005480
	adds r0, r5, #0
	ldr r1, .L_0200a3a8
	bl Func_02005490
	adds r0, r5, #0
	movs r1, #10
	bl Object_SetPartAttribute
	adds r3, r5, #0
	adds r3, #100
	strh r6, [r3]
	str r6, [r5, #48]
	str r6, [r5, #52]
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200a3a8:
	.4byte Data_02005d8c
	.section .text.x0200a3ac,"ax",%progbits
	.global Func_020023ac
	.thumb_func
Func_020023ac:
	push {r5, r6, lr}
	ldr r3, [r0, #8]
	ldr r2, [r0, #4]
	adds r6, r1, #0
	ldr r1, [r0]
	ldr r0, .L_0200a420
	adds r3, r3, r0
	movs r0, #30
	adds r0, #255
	bl Func_02005498
	adds r5, r0, #0
	cmp r5, #0
	beq .L_0200a41e
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
	ldr r3, .L_0200a424
	lsls r0, r0, #16
	lsrs r0, r0, #16
	adds r0, r0, r3
	adds r3, r5, #0
	adds r3, #100
	strh r0, [r3]
	movs r1, #2
	adds r0, r5, #0
	bl Func_0200236c
	ldr r3, .L_0200a428
	adds r0, r5, #0
	str r3, [r5, #108]
	movs r1, #1
	bl Animation_SetStateFlags
.L_0200a41e:
	pop {r5, r6, pc}
.L_0200a420:
	.4byte 0xfffe0000
.L_0200a424:
	.4byte 0xffff8000
.L_0200a428:
	.4byte Func_020022d0
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
	.4byte Data_020023c4 + 0x188
.L_0200a5ac:
	.4byte 0xffff0000
	.section .text.x0200a5b0,"ax",%progbits
	.global Func_020025b0
	.thumb_func
Func_020025b0:
	push {r5, r6, lr}
	movs r0, #10
	adds r0, #255
	ldr r6, .L_0200a5fc
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a5ca
	ldr r3, .L_0200a600
	adds r0, r6, #0
	movs r1, #116
	mov lr, r3
	.2byte 0xf800
.L_0200a5ca:
	movs r0, #110
	movs r1, #1
	movs r2, #0
	movs r3, #0
	adds r0, #255
	bl Func_02005498
	adds r5, r0, #0
	adds r2, r5, #0
	adds r2, #92
	movs r3, #2
	strb r3, [r2]
	movs r1, #1
	bl Func_02005480
	ldr r1, [r5, #80]
	movs r2, #1
	ldrb r3, [r1, #16]
	str r5, [r6, #112]
	strh r3, [r6, #12]
	ldrb r3, [r1, #17]
	orrs r3, r2
	strb r3, [r1, #17]
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200a5fc:
	.4byte Data_020023c4 + 0x188
.L_0200a600:
	.4byte IwramClearWords
	.section .text.x0200a604,"ax",%progbits
	.global Func_02002604
	.thumb_func
Func_02002604:
	push {r5, lr}
	ldr r5, .L_0200a614
	ldr r0, [r5, #112]
	bl Func_020054a0
	movs r3, #0
	str r3, [r5, #112]
	pop {r5, pc}
.L_0200a614:
	.4byte Data_020023c4 + 0x188
	.section .text.x0200a618,"ax",%progbits
	.global Func_02002618
	.thumb_func
Func_02002618:
	ldr r3, .L_0200a620
	strh r0, [r3, #14]
	bx lr
	.2byte 0x0000
.L_0200a620:
	.4byte Data_020023c4 + 0x188
	.section .text.x0200a624,"ax",%progbits
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
	bl ObjectDispatch_SetSingleChildField26
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
	.4byte Data_020023c4 + 0x188
.L_0200a754:
	.4byte Func_0200242c
	.section .text.x0200a758,"ax",%progbits
	.global Func_02002758
	.thumb_func
Func_02002758:
	ldr r3, .L_0200a764
	lsls r0, r0, #4
	adds r0, r0, r3
	movs r3, #20
	ldrsh r0, [r0, r3]
	bx lr
.L_0200a764:
	.4byte Data_020023c4 + 0x188
	.section .text.x0200a768,"ax",%progbits
	.global Func_02002768
	.thumb_func
Func_02002768:
	ldr r3, .L_0200a774
	lsls r0, r0, #4
	adds r0, r0, r3
	strh r1, [r0, #20]
	bx lr
	.2byte 0x0000
.L_0200a774:
	.4byte Data_020023c4 + 0x188
	.section .text.x0200a778,"ax",%progbits
	.global Func_02002778
	.thumb_func
Func_02002778:
	ldr r3, .L_0200a780
	ldr r0, [r3]
	bx lr
	.2byte 0x0000
.L_0200a780:
	.4byte Data_020023c4 + 0x188
	.section .text.x0200a784,"ax",%progbits
	.global Func_02002784
	.thumb_func
Func_02002784:
	push {r5, r6, lr}
	adds r6, r0, #0
	movs r0, #130
	lsls r0, r0, #1
	ldr r5, .L_0200a7a8
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a7a2
	ldr r3, [r5]
	adds r3, #1
	str r3, [r5]
	cmp r3, r6
	blt .L_0200a7a2
	str r0, [r5]
.L_0200a7a2:
	ldr r0, [r5]
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200a7a8:
	.4byte Data_020023c4 + 0x188
	.section .text.x0200a7ac,"ax",%progbits
	.global Func_020027ac
	.thumb_func
Func_020027ac:
	ldr r3, .L_0200a7c8
	lsls r0, r0, #4
	adds r0, r0, r3
	movs r3, #15
	ands r3, r1
	adds r0, #20
	lsls r3, r3, #16
	str r3, [r0, #12]
	ldr r3, .L_0200a7c4
	ands r1, r3
	strh r1, [r0, #4]
	bx lr
.L_0200a7c4:
	.4byte 0x000000f0
.L_0200a7c8:
	.4byte Data_020023c4 + 0x188
	.section .text.x0200a7cc,"ax",%progbits
	.global Func_020027cc
	.thumb_func
Func_020027cc:
	ldr r3, .L_0200a7d8
	lsls r0, r0, #4
	adds r0, r0, r3
	strh r1, [r0, #26]
	bx lr
	.2byte 0x0000
.L_0200a7d8:
	.4byte Data_020023c4 + 0x188
	.section .text.x0200a7dc,"ax",%progbits
	.global Func_020027dc
	.thumb_func
Func_020027dc:
	ldr r3, .L_0200a7e8
	lsls r0, r0, #4
	adds r0, r0, r3
	movs r3, #24
	ldrsh r0, [r0, r3]
	bx lr
.L_0200a7e8:
	.4byte Data_020023c4 + 0x188
	.section .text.x0200a7ec,"ax",%progbits
	.global Func_020027ec
	.thumb_func
Func_020027ec:
	push {lr}
	ldr r2, .L_0200a7fc
	cmp r0, #3
	bhi .L_0200a7fa
	lsls r3, r0, #2
	adds r3, #84
	strh r1, [r2, r3]
.L_0200a7fa:
	pop {pc}
.L_0200a7fc:
	.4byte Data_020023c4 + 0x188
	.section .text.x0200a800,"ax",%progbits
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
	bl Owner_GetState
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
	bl Owner_GetState
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
	.section .text.x0200a8ec,"ax",%progbits
	.global Func_020028ec
	.thumb_func
Func_020028ec:
	push {r5, r6, r7, lr}
	ldr r3, .L_0200a950
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	adds r6, r0, #0
	ldr r0, [r3]
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
	adds r0, r5, #0
	adds r3, r3, r7
	movs r1, #18
	str r3, [r6, #16]
	bl Engine_MathDivide
	subs r5, r5, r0
	str r5, [r6, #68]
	adds r3, r7, #0
	cmp r7, #0
	bge .L_0200a92a
	adds r3, #15
.L_0200a92a:
	asrs r3, r3, #4
	subs r3, r7, r3
	str r3, [r6, #76]
	ldr r2, [r6, #48]
	ldr r3, [r6, #24]
	movs r1, #128
	adds r3, r3, r2
	str r3, [r6, #24]
	ldr r2, [r6, #52]
	ldr r3, [r6, #28]
	lsls r1, r1, #5
	adds r3, r3, r2
	str r3, [r6, #28]
	ldr r2, [r6, #80]
	ldrh r3, [r2, #18]
	adds r3, r3, r1
	strh r3, [r2, #18]
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200a950:
	.4byte gPartyState
	.section .text.x0200a954,"ax",%progbits
	.global Func_02002954
	.thumb_func
Func_02002954:
	push {lr}
	ldr r3, .L_0200a978
	ldr r2, [r3]
	movs r3, #7
	ands r2, r3
	cmp r2, #0
	beq .L_0200a968
	cmp r2, #4
	beq .L_0200a970
	b .L_0200a976
.L_0200a968:
	movs r1, #10
	bl Animation_ApplyChildValues
	b .L_0200a976
.L_0200a970:
	movs r1, #0
	bl Animation_ApplyChildValues
.L_0200a976:
	pop {pc}
.L_0200a978:
	.4byte Data_0300122c
	.section .text.x0200a97c,"ax",%progbits
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
	.4byte Data_020023c4 + 0x188
.L_0200ab78:
	.4byte Func_02002954
.L_0200ab7c:
	.4byte 0x000fffff
.L_0200ab80:
	.4byte 0xfff80000
.L_0200ab84:
	.4byte 0xffff0000
	.section .text.x0200ab88,"ax",%progbits
	.global Func_02002b88
	.thumb_func
Func_02002b88:
	push {lr}
	ldr r3, .L_0200ab9c
	movs r2, #4
	ldrsh r3, [r3, r2]
	cmp r3, #0
	beq .L_0200ab98
	bl Func_0200297c
.L_0200ab98:
	pop {pc}
	.2byte 0x0000
.L_0200ab9c:
	.4byte Data_020023c4 + 0x188
	.section .text.x0200aba0,"ax",%progbits
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
	bl Battle_WaitMode0
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
	.4byte Data_020023c4 + 0x188
.L_0200acd8:
	.4byte Func_02002954
.L_0200acdc:
	.4byte 0xfff00000
	.section .text.x0200ace0,"ax",%progbits
	.global Func_02002ce0
	.thumb_func
Func_02002ce0:
	push {lr}
	ldr r3, .L_0200acf8
	movs r2, #4
	ldrsh r3, [r3, r2]
	cmp r3, #0
	beq .L_0200acf4
	bl Func_02002ba0
	movs r0, #1
	b .L_0200acf6
.L_0200acf4:
	movs r0, #0
.L_0200acf6:
	pop {pc}
.L_0200acf8:
	.4byte Data_020023c4 + 0x188
	.section .text.x0200acfc,"ax",%progbits
	.global Func_02002cfc
	.thumb_func
Func_02002cfc:
	ldr r3, .L_0200ad04
	movs r2, #4
	ldrsh r0, [r3, r2]
	bx lr
.L_0200ad04:
	.4byte Data_020023c4 + 0x188
	.section .text.x0200ad08,"ax",%progbits
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
	bl ObjectDispatch_SetSingleChildField26
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
	.4byte Data_020023c4 + 0x188
.L_0200ae74:
	.4byte Data_02005dbc
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
	bl ObjectDispatch_SetSingleChildField26
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
	.4byte Data_02005e00
.L_0200af68:
	.4byte IwramMulQ16
	.section .text.x0200af6c,"ax",%progbits
	.global Func_02002f6c
	.thumb_func
Func_02002f6c:
	push {r5, lr}
	adds r5, r0, #0
	movs r0, #130
	lsls r0, r0, #1
	bl GameFlag_Test
	adds r5, #91
	strb r0, [r5]
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x0200af80,"ax",%progbits
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
	.4byte Data_020023c4 + 0x188
.L_0200b280:
	.4byte 0x0007ffff
.L_0200b284:
	.4byte 0x001ffffe
.L_0200b288:
	.4byte 0xfffff000
	.section .text.x0200b28c,"ax",%progbits
	.global Func_0200328c
	.thumb_func
Func_0200328c:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r9
	push {r5, r6}
	mov r6, r8
	push {r6}
	ldr r6, [sp, #24]
	adds r5, r1, #0
	mov r9, r2
	mov r10, r3
	bl Object_GetById
	mov r8, r0
	adds r0, r5, #0
	bl Object_GetById
	adds r5, r0, #0
	mov r0, r8
	ldr r2, [r0, #12]
	ldr r3, [r0, #16]
	ldr r1, [r0, #8]
	adds r0, r5, #0
	bl Object_SetPositionAndResetMotion
	adds r0, r5, #0
	adds r1, r6, #0
	bl Func_02005490
	adds r0, r5, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	adds r3, r5, #0
	adds r3, #85
	movs r6, #0
	strb r6, [r3]
	adds r0, r5, #0
	movs r1, #1
	bl Func_02005480
	adds r3, r5, #0
	mov r2, r8
	adds r3, #100
	str r2, [r5, #104]
	mov r0, r10
	strh r6, [r3]
	adds r3, #2
	strh r0, [r3]
	mov r2, r9
	subs r3, #4
	strb r2, [r3]
	ldr r3, .L_0200b300
	str r3, [r5, #108]
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, pc}
.L_0200b300:
	.4byte Func_02002f6c
	.section .text.x0200b304,"ax",%progbits
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
	.section .text.x0200b3cc,"ax",%progbits
	.global Func_020033cc
	.thumb_func
Func_020033cc:
	push {r5, r6, r7, lr}
	adds r5, r0, #0
	adds r7, r5, #0
	adds r7, #100
	movs r3, #0
	ldrsh r6, [r7, r3]
	cmp r6, #0
	bne .L_0200b404
	ldr r1, [r5, #8]
	ldr r2, [r5, #16]
	movs r0, #0
	bl Map_GetTerrainHeight
	movs r3, #128
	lsls r3, r3, #13
	adds r0, r0, r3
	movs r3, #128
	lsls r3, r3, #9
	str r0, [r5, #12]
	movs r1, #1
	adds r0, r5, #0
	str r6, [r5, #24]
	str r6, [r5, #28]
	str r3, [r5, #48]
	str r3, [r5, #52]
	bl Func_02005528
	b .L_0200b46c
.L_0200b404:
	cmp r6, #30
	bgt .L_0200b41c
	cmp r6, #30
	bne .L_0200b46c
	movs r0, #136
	bl Func_020056c0
	ldr r0, [r5, #104]
	movs r1, #2
	bl Func_02005480
	b .L_0200b46c
.L_0200b41c:
	cmp r6, #60
	bgt .L_0200b444
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r5, #24]
	str r3, [r5, #28]
	ldrh r3, [r7]
	ldr r0, [r5, #104]
	lsls r3, r3, #16
	asrs r2, r3, #16
	lsrs r3, r3, #31
	adds r2, r2, r3
	ldr r3, .L_0200b468
	asrs r2, r2, #1
	ands r2, r3
	lsls r1, r2, #3
	subs r1, r1, r2
	bl Animation_ApplyChildValues
	b .L_0200b46c
.L_0200b444:
	movs r1, #1
	ldr r0, [r5, #104]
	bl Func_02005480
	movs r0, #184
	bl Func_020056c0
	ldr r1, [r5, #8]
	ldr r2, [r5, #16]
	movs r0, #0
	bl Map_GetTerrainHeight
	movs r3, #0
	str r0, [r5, #12]
	str r3, [r5, #104]
	movs r0, #0
	b .L_0200b474
	.2byte 0x0000
.L_0200b468:
	.4byte 0x00000001
.L_0200b46c:
	ldrh r3, [r7]
	movs r0, #1
	adds r3, #1
	strh r3, [r7]
.L_0200b474:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x0200b478,"ax",%progbits
	.global Func_02003478
	.thumb_func
Func_02003478:
	push {r5, r6, r7, lr}
	adds r5, r0, #0
	adds r7, r5, #0
	adds r7, #100
	movs r3, #0
	ldrsh r6, [r7, r3]
	cmp r6, #0
	bne .L_0200b4c2
	movs r0, #136
	bl Func_020056c0
	ldr r0, [r5, #104]
	movs r1, #2
	bl Func_02005480
	ldr r1, [r5, #8]
	ldr r2, [r5, #16]
	movs r0, #0
	bl Map_GetTerrainHeight
	movs r3, #128
	lsls r3, r3, #13
	adds r0, r0, r3
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r5, #48]
	movs r3, #128
	str r0, [r5, #12]
	lsls r3, r3, #8
	adds r0, r5, #0
	movs r1, #1
	str r6, [r5, #24]
	str r6, [r5, #28]
	str r3, [r5, #52]
	bl Func_02005528
	b .L_0200b510
.L_0200b4c2:
	cmp r6, #32
	bgt .L_0200b4ea
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r5, #24]
	str r3, [r5, #28]
	ldrh r3, [r7]
	ldr r0, [r5, #104]
	lsls r3, r3, #16
	asrs r2, r3, #16
	lsrs r3, r3, #31
	adds r2, r2, r3
	ldr r3, .L_0200b50c
	asrs r2, r2, #1
	ands r2, r3
	lsls r1, r2, #3
	subs r1, r1, r2
	bl Animation_ApplyChildValues
	b .L_0200b510
.L_0200b4ea:
	movs r1, #1
	ldr r0, [r5, #104]
	bl Func_02005480
	movs r0, #184
	bl Func_020056c0
	ldr r1, [r5, #8]
	ldr r2, [r5, #16]
	movs r0, #0
	bl Map_GetTerrainHeight
	movs r3, #0
	str r0, [r5, #12]
	str r3, [r5, #104]
	movs r0, #0
	b .L_0200b518
.L_0200b50c:
	.4byte 0x00000001
.L_0200b510:
	ldrh r3, [r7]
	movs r0, #1
	adds r3, #1
	strh r3, [r7]
.L_0200b518:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x0200b51c,"ax",%progbits
	.global Func_0200351c
	.thumb_func
Func_0200351c:
	push {r5, lr}
	adds r5, r0, #0
	bl Func_02003304
	movs r3, #0
	str r3, [r5, #8]
	str r3, [r5, #12]
	str r3, [r5, #16]
	str r3, [r5, #36]
	str r3, [r5, #40]
	str r3, [r5, #44]
	movs r0, #0
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x0200b57e,"ax",%progbits
	.2byte 0x0000
	.section .text.x0200b580,"ax",%progbits
	.global Func_02003580
	.thumb_func
Func_02003580:
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
	.section .text.x0200b5b8,"ax",%progbits
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
	bl ObjectDispatch_SetSingleChildField26
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
	bl Engine_MathDivide
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, [r6, #28]
	ldr r1, [r5, #12]
	subs r0, r0, r3
	b .L_0200b70c
.L_0200b6fa:
	ldr r2, .L_0200b77c
	adds r0, r3, r2
	bl Engine_MathDivide
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, .L_0200b77c
	ldr r1, [r5, #12]
	adds r0, r0, r3
.L_0200b70c:
	bl Engine_MathDivide
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
	.4byte Data_02007058
.L_0200b778:
	.4byte Func_02003580
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
	.4byte gOverlayArea + 0x70a8
.L_0200b89c:
	.4byte gPartyState
.L_0200b8a0:
	.4byte 0xffff0000
.L_0200b8a4:
	.4byte ResourceTableEntries
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
	bl Runtime_BumpAllocate
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
	bl VramBlock_LoadCached
	adds r0, r5, #0
	bl Sys_Free
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
	.4byte gOverlayArea + 0x70a8
.L_0200bae0:
	.4byte IwramClearWords
.L_0200bae4:
	.4byte gPartyState
.L_0200bae8:
	.4byte 0x11111111
.L_0200baec:
	.4byte Func_02003780
	.section .text.x0200baf0,"ax",%progbits
	.global Func_02003af0
	.thumb_func
Func_02003af0:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r8
	push {r5, r6}
	ldr r1, .L_0200bb70
	movs r2, #133
	mov r8, r1
	lsls r2, r2, #2
	add r8, r2
	mov r3, r8
	ldr r0, [r3]
	bl Object_GetById
	mov r1, r8
	ldr r5, [r0, #8]
	ldr r6, [r0, #16]
	mov r10, r0
	movs r2, #128
	ldr r0, [r1]
	movs r1, #128
	lsls r1, r1, #11
	lsls r2, r2, #10
	bl ObjectMotion_SetSpeedParameters
	asrs r5, r5, #20
	mov r2, r8
	asrs r6, r6, #20
	ldr r0, [r2]
	lsls r1, r5, #4
	lsls r2, r6, #4
	adds r1, #8
	adds r2, #8
	bl ObjectMotion_SetPositionAndCommit
	movs r0, #1
	bl WaitFrames
	movs r3, #128
	lsls r3, r3, #12
	lsls r5, r5, #20
	lsls r6, r6, #20
	adds r5, r5, r3
	mov r1, r10
	adds r6, r6, r3
	ldr r2, [r1, #12]
	adds r3, r6, #0
	adds r1, r5, #0
	mov r0, r10
	bl Object_SetPositionAndResetMotion
	movs r0, #4
	bl Battle_WaitMode0
	bl Func_020055e8
	ldr r2, .L_0200bb74
	ldr r3, [r0, #12]
	adds r3, r3, r2
	str r3, [r0, #12]
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200bb70:
	.4byte gPartyState
.L_0200bb74:
	.4byte 0xfff80000
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
	bl Battle_WaitMode0
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
	bl Battle_WaitMode0
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
	bl ObjectDispatch_SetSingleChildField26
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
	bl Battle_WaitMode0
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
	bl Event_ClearStatus1c6
	bl Event_WaitValue1c8Frames
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
	.section .text.x0200bd28,"ax",%progbits
	.global Func_02003d28
	.thumb_func
Func_02003d28:
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
	bge .L_0200bd58
	adds r3, #15
.L_0200bd58:
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
	.section .text.x0200bd80,"ax",%progbits
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
	bl Motion_CamBounds
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
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	movs r0, #204
	bl Func_020056c0
	movs r3, #3
	strb r3, [r5]
	movs r0, #24
	bl Battle_WaitMode0
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
	bl Battle_WaitMode0
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
	.4byte Func_02003d28
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
	.4byte gOverlayArea + 0x70a8
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
	.4byte gOverlayArea + 0x70a8
	.section .text.x0200c080,"ax",%progbits
	.global Func_02004080
	.thumb_func
Func_02004080:
	push {r5, lr}
	adds r5, r0, #0
	adds r2, r5, #0
	adds r2, #100
	movs r0, #0
	ldrsh r1, [r2, r0]
	ldrh r3, [r2]
	cmp r1, #0
	beq .L_0200c098
	subs r3, #1
	strh r3, [r2]
	b .L_0200c0fe
.L_0200c098:
	adds r3, r5, #0
	adds r3, #90
	movs r0, #131
	strb r1, [r3]
	lsls r0, r0, #1
	bl GameFlag_Test
	movs r3, #1
	negs r3, r3
	cmp r0, #0
	bne .L_0200c0be
	ldr r3, .L_0200c100
	movs r2, #15
	ldr r3, [r3]
	ldr r1, .L_0200c104
	lsrs r3, r3, #4
	ands r3, r2
	lsls r3, r3, #1
	ldrsh r3, [r1, r3]
.L_0200c0be:
	movs r0, #1
	negs r0, r0
	cmp r3, r0
	bne .L_0200c0d0
	adds r0, r5, #0
	movs r1, #9
	bl Func_02005480
	b .L_0200c0fe
.L_0200c0d0:
	ldrh r1, [r5, #6]
	movs r2, #128
	subs r3, r3, r1
	lsls r3, r3, #16
	asrs r3, r3, #16
	lsls r2, r2, #5
	cmp r3, r2
	ble .L_0200c0e2
	adds r3, r2, #0
.L_0200c0e2:
	ldr r2, .L_0200c108
	cmp r3, r2
	bge .L_0200c0ea
	adds r3, r2, #0
.L_0200c0ea:
	adds r3, r1, r3
	adds r0, r5, #0
	movs r1, #2
	strh r3, [r5, #6]
	bl Func_02005480
	adds r0, r5, #0
	movs r1, #48
	bl ObjectDispatch_ApplyValueToChildren
.L_0200c0fe:
	pop {r5, pc}
.L_0200c100:
	.4byte gInput
.L_0200c104:
	.4byte Data_02005ef8
.L_0200c108:
	.4byte 0xfffff000
	.section .text.x0200c10c,"ax",%progbits
	.global Func_0200410c
	.thumb_func
Func_0200410c:
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
	beq .L_0200c13c
	movs r0, #131
	lsls r0, r0, #1
	bl GameFlag_SetBit
	bl Func_02005608
	bl Func_02005640
	movs r0, #131
	lsls r0, r0, #1
	bl GameFlag_ClearBit
.L_0200c13c:
	pop {pc}
	.2byte 0x0000
	.section .text.x0200c140,"ax",%progbits
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
	.4byte Data_02005f38
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
	bl ObjectDispatch_ApplyValueToChildren
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
	.4byte gOverlayArea + 0x74c8
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
	.4byte Data_02005f38
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
	bl ObjectDispatch_ApplyValueToChildren
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
	.4byte gOverlayArea + 0x74c8
.L_0200c6fc:
	.4byte Func_02004080
	.section .text.x0200c700,"ax",%progbits
	.global Func_02004700
	.thumb_func
Func_02004700:
	ldr r3, .L_0200c708
	strh r0, [r3]
	bx lr
	.2byte 0x0000
.L_0200c708:
	.4byte gOverlayArea + 0x74c8
	.section .text.x0200c70c,"ax",%progbits
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
	bl ObjectDispatch_ApplyValueToChildren
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
	.4byte Data_02005f58
.L_0200ca40:
	.4byte Data_02005f64
	.section .text.x0200ca44,"ax",%progbits
	.global Func_02004a44
	.thumb_func
Func_02004a44:
	push {r5, r6, lr}
	adds r5, r3, #0
	ldr r3, [sp, #12]
	lsls r2, r2, #7
	mov r12, r3
	movs r3, #192
	lsls r3, r3, #18
	ldr r4, [r3, #32]
	lsls r3, r0, #3
	subs r3, r3, r0
	movs r0, #156
	lsls r0, r0, #1
	lsls r3, r3, #3
	adds r3, r3, r0
	ldr r0, [r4, r3]
	adds r1, r1, r2
	lsls r1, r1, #2
	adds r0, r0, r1
	movs r1, #0
	ldr r6, [sp, #16]
	cmp r1, r12
	bcs .L_0200ca8a
.L_0200ca70:
	lsls r3, r1, #9
	movs r2, #0
	adds r3, r0, r3
	cmp r2, r5
	bcs .L_0200ca84
.L_0200ca7a:
	adds r2, #1
	strb r6, [r3, #2]
	adds r3, #4
	cmp r2, r5
	bcc .L_0200ca7a
.L_0200ca84:
	adds r1, #1
	cmp r1, r12
	bcc .L_0200ca70
.L_0200ca8a:
	pop {r5, r6, pc}
	.section .text.x0200ca8c,"ax",%progbits
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
	.4byte Data_02005f64
.L_0200cbec:
	.4byte Data_02005f7c
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
	.4byte Data_02005f58
.L_0200cd08:
	.4byte Data_02005f7c
.L_0200cd0c:
	.4byte Data_02005f64
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
	.4byte Data_02005f64
.L_0200cfb8:
	.4byte Func_02004fc0
.L_0200cfbc:
	.4byte Data_02005f7c
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
	.4byte Data_02005f58
	.section .text.x0200d048,"ax",%progbits
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
	.4byte gOverlayArea + 0x74cc
.L_0200d1e4:
	.4byte ResourceTableEntries
.L_0200d1e8:
	.4byte gOverlayArea + 0x7510
.L_0200d1ec:
	.4byte gOverlayArea + 0x74ce
.L_0200d1f0:
	.4byte gOverlayArea + 0x74d0
.L_0200d1f4:
	.4byte gOverlayArea + 0x75d0
.L_0200d1f8:
	.4byte 0x40002000
.L_0200d1fc:
	.4byte 0xc000a000
.L_0200d200:
	.4byte gOverlayArea + 0x75d2
	.section .text.x0200d204,"ax",%progbits
	.global Func_02005204
	.thumb_func
Func_02005204:
	push {r5, r6, lr}
	movs r0, #192
	lsls r0, r0, #4
	bl Runtime_BumpAllocate
	ldr r3, .L_0200d264
	adds r6, r0, #0
	movs r1, #64
	ldr r0, .L_0200d268
	mov lr, r3
	.2byte 0xf800
	adds r1, r6, #0
	ldr r0, .L_0200d26c
	bl Func_02005420
	ldr r5, .L_0200d270
	bl Resource_FindFreeEntry
	movs r1, #192
	strh r0, [r5]
	lsls r0, r0, #16
	adds r2, r6, #0
	lsls r1, r1, #4
	asrs r0, r0, #16
	bl VramBlock_LoadCached
	adds r0, r6, #0
	bl Sys_Free
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_0200d274
	bl Scheduler_AddOrUpdateCallback
	ldr r3, .L_0200d278
	ldr r2, .L_0200d25c
	strh r2, [r3]
	ldr r3, .L_0200d27c
	strh r2, [r3]
	ldr r2, .L_0200d280
	ldr r3, .L_0200d260
	strh r3, [r2]
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200d25c:
	.4byte 0x00000000
.L_0200d260:
	.4byte 0xffffffff
.L_0200d264:
	.4byte IwramClearWords
.L_0200d268:
	.4byte gOverlayArea + 0x74d0
.L_0200d26c:
	.4byte Data_02005fbc
.L_0200d270:
	.4byte gOverlayArea + 0x74cc
.L_0200d274:
	.4byte Func_02005048
.L_0200d278:
	.4byte gOverlayArea + 0x74ce
.L_0200d27c:
	.4byte gOverlayArea + 0x75d0
.L_0200d280:
	.4byte gOverlayArea + 0x75d2
	.section .text.x0200d284,"ax",%progbits
	.global Func_02005284
	.thumb_func
Func_02005284:
	push {r5, r6, lr}
	movs r0, #192
	lsls r0, r0, #4
	bl Runtime_BumpAllocate
	ldr r3, .L_0200d2e4
	adds r6, r0, #0
	movs r1, #64
	ldr r0, .L_0200d2e8
	mov lr, r3
	.2byte 0xf800
	adds r1, r6, #0
	ldr r0, .L_0200d2ec
	bl Func_02005420
	ldr r5, .L_0200d2f0
	bl Resource_FindFreeEntry
	movs r1, #128
	strh r0, [r5]
	lsls r0, r0, #16
	adds r2, r6, #0
	lsls r1, r1, #4
	asrs r0, r0, #16
	bl VramBlock_LoadCached
	adds r0, r6, #0
	bl Sys_Free
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_0200d2f4
	bl Scheduler_AddOrUpdateCallback
	ldr r3, .L_0200d2f8
	ldr r2, .L_0200d2dc
	strh r2, [r3]
	ldr r3, .L_0200d2fc
	strh r2, [r3]
	ldr r2, .L_0200d300
	ldr r3, .L_0200d2e0
	strh r3, [r2]
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200d2dc:
	.4byte 0x00000000
.L_0200d2e0:
	.4byte 0xffffffff
.L_0200d2e4:
	.4byte IwramClearWords
.L_0200d2e8:
	.4byte gOverlayArea + 0x74d0
.L_0200d2ec:
	.4byte Data_0200611e + 0x1
.L_0200d2f0:
	.4byte gOverlayArea + 0x74cc
.L_0200d2f4:
	.4byte Func_02005048
.L_0200d2f8:
	.4byte gOverlayArea + 0x74ce
.L_0200d2fc:
	.4byte gOverlayArea + 0x75d0
.L_0200d300:
	.4byte gOverlayArea + 0x75d2
	.section .text.x0200d304,"ax",%progbits
	.global Func_02005304
	.thumb_func
Func_02005304:
	push {r5, r6, lr}
	movs r0, #128
	lsls r0, r0, #4
	bl Runtime_BumpAllocate
	ldr r3, .L_0200d368
	adds r6, r0, #0
	movs r1, #64
	ldr r0, .L_0200d36c
	mov lr, r3
	.2byte 0xf800
	adds r1, r6, #0
	ldr r0, .L_0200d370
	bl Func_02005420
	ldr r5, .L_0200d374
	bl Resource_FindFreeEntry
	movs r1, #128
	strh r0, [r5]
	lsls r0, r0, #16
	adds r2, r6, #0
	lsls r1, r1, #4
	asrs r0, r0, #16
	bl VramBlock_LoadCached
	adds r0, r6, #0
	bl Sys_Free
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_0200d378
	bl Scheduler_AddOrUpdateCallback
	ldr r2, .L_0200d37c
	ldr r3, .L_0200d35c
	strh r3, [r2]
	ldr r2, .L_0200d380
	ldr r3, .L_0200d360
	strh r3, [r2]
	ldr r2, .L_0200d384
	ldr r3, .L_0200d364
	strh r3, [r2]
	b .L_0200d388
.L_0200d35c:
	.4byte 0x00000000
.L_0200d360:
	.4byte 0x00000001
.L_0200d364:
	.4byte 0xffffffff
.L_0200d368:
	.4byte IwramClearWords
.L_0200d36c:
	.4byte gOverlayArea + 0x74d0
.L_0200d370:
	.4byte Data_0200634e
.L_0200d374:
	.4byte gOverlayArea + 0x74cc
.L_0200d378:
	.4byte Func_02005048
.L_0200d37c:
	.4byte gOverlayArea + 0x74ce
.L_0200d380:
	.4byte gOverlayArea + 0x75d0
.L_0200d384:
	.4byte gOverlayArea + 0x75d2
.L_0200d388:
	pop {r5, r6, pc}
	.2byte 0x0000
	.section .text.x0200d38c,"ax",%progbits
	.global Func_0200538c
	.thumb_func
Func_0200538c:
	push {r5, lr}
	adds r5, r1, #0
	bl Object_GetById
	adds r4, r0, #0
	cmp r4, #0
	beq .L_0200d3b2
	adds r3, r4, #0
	adds r3, #100
	strh r5, [r3]
	ldr r1, .L_0200d3b4
	ldr r0, .L_0200d3b8
	ldrh r2, [r1]
	movs r5, #0
	ldrsh r3, [r1, r5]
	adds r2, #1
	lsls r3, r3, #2
	str r4, [r0, r3]
	strh r2, [r1]
.L_0200d3b2:
	pop {r5, pc}
.L_0200d3b4:
	.4byte gOverlayArea + 0x74ce
.L_0200d3b8:
	.4byte gOverlayArea + 0x74d0
	.section .text.x0200d3bc,"ax",%progbits
	.global Func_020053bc
	.thumb_func
Func_020053bc:
	ldr r3, .L_0200d3c4
	strh r0, [r3]
	bx lr
	.2byte 0x0000
.L_0200d3c4:
	.4byte gOverlayArea + 0x75d2
	.section .rodata.x0200d6c8,"a",%progbits
.L_0200d6c8:
	.4byte 0x0000002e
	.4byte Func_0200004c
	.4byte 0x00000011
.L_0200d6d4:
	.4byte 0x00000016
	.4byte 0x0000001f
	.4byte 0x00000000
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00008000
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00000800
	.4byte 0x00000004
	.4byte 0x02a80000
	.4byte 0x00000000
	.4byte 0x01300000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000004
	.4byte 0x02f80000
	.4byte 0x00000000
	.4byte 0x01300000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0x00000000
.L_0200d748:
	.4byte 0x00000016
	.4byte 0x0000001f
	.4byte 0x00000000
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00008000
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00000800
	.4byte 0x00000004
	.4byte 0x02480000
	.4byte 0x00000000
	.4byte 0x01300000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000004
	.4byte 0x02980000
	.4byte 0x00000000
	.4byte 0x01300000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0x00000000
.L_0200d7bc:
	.4byte 0x00000016
	.4byte 0x0000001f
	.4byte 0x00000000
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000006
	.4byte 0x00300000
	.4byte 0x00000016
	.4byte 0x00000016
	.4byte 0x00300000
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00008000
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00000800
	.4byte 0x80010000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000012
	.4byte 0x00000220
	.4byte 0x00000010
	.4byte 0xc0010000
	.4byte 0x80020000
	.4byte 0x00000004
	.4byte 0x00b00000
	.4byte 0x00000000
	.4byte 0x02400000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x02400000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000004
	.4byte 0x00b00000
	.4byte 0x00000000
	.4byte 0x02400000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x02400000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0xc0020000
.L_0200d890:
	.4byte 0x00000016
	.4byte 0x0000001f
	.4byte 0x00000000
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000006
	.4byte 0x00200000
	.4byte 0x00000016
	.4byte 0x00000016
	.4byte 0x00200000
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00008000
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00000800
	.4byte 0x00000004
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x00700000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x80010000
	.4byte 0x00000004
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x00700000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000004
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x00700000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0xc0010000
.L_0200d93c:
	.4byte 0x00000016
	.4byte 0x0000001f
	.4byte 0x00000000
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000000
	.4byte 0x00000016
	.4byte 0x00000006
	.4byte 0x00200000
	.4byte 0x00000016
	.4byte 0x00000016
	.4byte 0x00200000
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00008000
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00000800
	.4byte 0x80010000
	.4byte 0x00000004
	.4byte 0x01080000
	.4byte 0x00200000
	.4byte 0x00700000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000004
	.4byte 0x01080000
	.4byte 0x00500000
	.4byte 0x00700000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000004
	.4byte 0x00e80000
	.4byte 0x00500000
	.4byte 0x00700000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000004
	.4byte 0x01080000
	.4byte 0x00500000
	.4byte 0x00700000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000004
	.4byte 0x01080000
	.4byte 0x00200000
	.4byte 0x00700000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000004
	.4byte 0x01680000
	.4byte 0x00200000
	.4byte 0x00700000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0xc0010000
.L_0200da3c:
	.4byte 0x00000016
	.4byte 0x0000001f
	.4byte 0x00000000
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000006
	.4byte 0x00200000
	.4byte 0x00000016
	.4byte 0x00000016
	.4byte 0x00200000
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00008000
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00000800
	.4byte 0x00000011
.L_0200da88:
	.4byte 0x00000016
	.4byte 0x0000001f
	.4byte 0x00000000
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000006
	.4byte 0x00300000
	.4byte 0x00000016
	.4byte 0x00000016
	.4byte 0x00300000
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00008000
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00000800
	.4byte 0x00000004
	.4byte 0x03c80000
	.4byte 0x00000000
	.4byte 0x00500000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x80010000
	.4byte 0x00000004
	.4byte 0x03680000
	.4byte 0x00000000
	.4byte 0x00500000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x03080000
	.4byte 0x00000000
	.4byte 0x00500000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000004
	.4byte 0x03680000
	.4byte 0x00000000
	.4byte 0x00500000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x03c80000
	.4byte 0x00000000
	.4byte 0x00500000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0xc0010000
	.global Data_02005b5c
Data_02005b5c:
	.4byte 0x00190018
	.4byte 0x0018001a
	.4byte 0x00000019
	.global Data_02005b68
Data_02005b68:
	.4byte 0x00000000
	.4byte 0x00000028
	.4byte 0x80010000
	.4byte 0x00000016
	.4byte 0x00000019
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000019
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000000a
	.4byte 0xc0010000
	.4byte 0x00000026
	.global Data_02005bac
Data_02005bac:
	.4byte 0x00000000
	.4byte 0x0000002a
	.4byte 0x80010000
	.4byte 0x00000016
	.4byte 0x00000019
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000019
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000000a
	.4byte 0xc0010000
	.4byte 0x00000026
	.global Data_02005bf0
Data_02005bf0:
	.4byte 0x00430008
	.2byte 0x0000
	.global Data_02005bf6
Data_02005bf6:
	.2byte 0x000a
	.4byte 0x00000001
	.global Data_02005bfc
Data_02005bfc:
	.4byte 0x00110012
	.4byte 0x00310013
	.4byte 0x00130014
	.4byte 0x00030015
	.2byte 0x0000
	.global Data_02005c0e
Data_02005c0e:
	.2byte 0x0016
	.4byte 0x00000000
	.global Data_02005c14
Data_02005c14:
	.4byte 0x0011000d
	.4byte 0x0021000e
	.4byte 0x0031000f
	.4byte 0x00830010
	.2byte 0x0000
	.global Data_02005c26
Data_02005c26:
	.2byte 0x0013
	.4byte 0x00000001
	.global Data_02005c2c
Data_02005c2c:
	.4byte 0x0003000d
	.2byte 0x0000
	.global Data_02005c32
Data_02005c32:
	.2byte 0x000e
	.4byte 0x00000001
	.global Data_02005c38
Data_02005c38:
	.4byte 0x00030008
	.4byte 0x00030009
	.4byte 0x0003000a
	.2byte 0x0000
	.global Data_02005c46
Data_02005c46:
	.2byte 0x000c
	.4byte 0x00000000
	.global Data_02005c4c
Data_02005c4c:
	.4byte 0x0003000d
	.4byte 0x0003000e
	.4byte 0x0003000f
	.2byte 0x0000
	.global Data_02005c5a
Data_02005c5a:
	.2byte 0x000a
	.4byte 0x00000000
	.global Data_02005c60
Data_02005c60:
	.4byte 0x00030018
	.4byte 0x00030019
	.4byte 0x0003001a
	.2byte 0x0000
	.global Data_02005c6e
Data_02005c6e:
	.2byte 0x0012
	.4byte 0x00130000
	.4byte 0x00140000
	.4byte 0x00150000
	.4byte 0x00160000
	.4byte 0x00000000
	.global Data_02005c84
Data_02005c84:
	.4byte 0x0000002e
	.4byte Func_02003478
	.4byte 0x00000004
	.4byte 0x00f80000
	.4byte 0x00200000
	.4byte 0x02a00000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00f80000
	.4byte 0x00160000
	.4byte 0x02a00000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00f80000
	.4byte 0x00160000
	.4byte 0x02b00000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000025
	.4byte Func_020006f4
	.4byte 0x0000002e
	.4byte Func_0200351c
	.4byte 0x00000011
	.global Data_02005ce0
Data_02005ce0:
	.4byte 0x0000002e
	.4byte Func_02003478
	.4byte 0x00000004
	.4byte 0x00b80000
	.4byte 0x00400000
	.4byte 0x02300000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00b80000
	.4byte 0x00360000
	.4byte 0x02300000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00b80000
	.4byte 0x00360000
	.4byte 0x02400000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000025
	.4byte Func_02000d80
	.4byte 0x0000002e
	.4byte Func_0200351c
	.4byte 0x00000011
	.global Data_02005d3c
Data_02005d3c:
	.4byte 0x0000002e
	.4byte Func_02003478
	.4byte 0x00000004
	.4byte 0x03680000
	.4byte 0x00200000
	.4byte 0x00800000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x03680000
	.4byte 0x00160000
	.4byte 0x00800000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x03680000
	.4byte 0x00160000
	.4byte 0x00900000
	.4byte 0x00000001
	.4byte 0x0000002e
	.4byte Func_0200351c
	.4byte 0x00000011
	.global Data_02005d8c
Data_02005d8c:
	.4byte 0x00000000
	.4byte 0x0000001e
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
	.global Data_02005dbc
Data_02005dbc:
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x80010000
	.4byte 0x00000016
	.4byte 0x00000019
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000019
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000000a
	.4byte 0xc0010000
	.4byte 0x00000026
	.global Data_02005e00
Data_02005e00:
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x80010000
	.4byte 0x00000016
	.4byte 0x00000019
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000019
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000000a
	.4byte 0xc0010000
	.4byte 0x00000026
.L_0200de44:
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
.L_0200de80:
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
.L_0200debc:
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
	.global Data_02005ef8
Data_02005ef8:
	.4byte 0x0000ffff
	.4byte 0xffff8000
	.4byte 0xe000c000
	.4byte 0xc000a000
	.4byte 0x20004000
	.4byte 0x40006000
	.4byte 0x0000ffff
	.4byte 0xffff8000
	.global Data_02005f18
Data_02005f18:
	.4byte 0x0000ffff
	.4byte 0xffff8000
	.4byte 0xffffc000
	.4byte 0xc000ffff
	.4byte 0xffff4000
	.4byte 0x4000ffff
	.4byte 0x0000ffff
	.4byte 0xffff8000
	.global Data_02005f38
Data_02005f38:
	.4byte 0x0000ffff
	.4byte 0xffff8000
	.4byte 0x0000c000
	.4byte 0xc0008000
	.4byte 0x00004000
	.4byte 0x40008000
	.4byte 0x0000ffff
	.4byte 0xffff8000
	.global Data_02005f58
Data_02005f58:
	.4byte 0x01030102
	.4byte 0x01260125
	.4byte 0x014b014c
	.global Data_02005f64
Data_02005f64:
	.4byte Sound_Wave29 + 0x134c
	.4byte 0x2008e0f8
	.4byte 0x0020f0e0
	.4byte 0x2008e0f8
	.4byte Sound_Wave29 + 0x134c
	.4byte 0x2008e0f8
	.global Data_02005f7c
Data_02005f7c:
	.4byte 0x00100000
	.4byte 0x00100000
	.4byte 0x00100000
	.4byte 0x00000010
	.4byte 0x00000010
	.4byte 0x00000010
	.4byte 0x00000010
	.4byte 0xfff00000
	.4byte 0xfff00000
	.4byte 0xfff00000
	.4byte 0xfff00000
	.4byte 0x0000fff0
	.4byte 0x0000fff0
	.4byte 0x0000fff0
	.4byte 0x0000fff0
	.4byte 0x00100000
	.global Data_02005fbc
Data_02005fbc:
	.4byte 0x06345d01
	.4byte Runtime_ReciprocalTable + 0x1259
	.4byte 0x2f010026
	.4byte 0x5f1f7000
	.4byte 0x667b0906
	.4byte 0x01040901
	.4byte 0x0e287800
	.4byte 0x56053b3f
	.4byte 0x5f6f080e
	.4byte 0x08100800
	.4byte 0xa1076601
	.4byte 0x001d0800
	.4byte Text_MessageContexts + 0x10b38
	.4byte 0x660015bb
	.4byte 0x0e020016
	.4byte 0x66177000
	.4byte 0x3b02080d
	.4byte Data_02001024 + 0xbb
	.4byte 0x66128010
	.4byte 0x01037a01
	.4byte 0x04277910
	.4byte 0xfb44013d
	.4byte Data_0200752c + 0x2d4
	.4byte 0x20590414
	.4byte 0x57052700
	.4byte 0x50002066
	.4byte 0x20ff0016
	.4byte 0x00162a00
	.4byte 0x0016002e
	.4byte 0x162a0020
	.4byte 0x4e002000
	.4byte 0x7f320040
	.4byte 0x04aa023b
	.4byte 0x02500006
	.4byte 0xe7169207
	.4byte 0x0010e103
	.4byte 0x0301ff0d
	.4byte 0x0c01e709
	.4byte 0x5906dd03
	.4byte 0x210a6118
	.4byte 0xfd340030
	.4byte 0x0403a425
	.4byte 0x5f290630
	.4byte 0x02490100
	.4byte 0xf8063b58
	.4byte 0x3b5704af
	.4byte 0x163bb406
	.4byte Resource_DecodeHalfwordLzCode + 0x12
	.4byte 0x283ea00f
	.4byte 0x070820ff
	.4byte 0x2e050010
	.4byte 0x057807f8
	.4byte 0x0a980488
	.4byte 0xdd660ba8
	.4byte 0x07080030
	.4byte 0x38205f22
	.4byte 0x33481706
	.4byte 0x00603b58
	.4byte 0x783ffe78
	.4byte Text_MessageContexts + 0x15ce8
	.4byte 0x1f017900
	.4byte 0x0a782700
	.4byte 0x2aff5f11
	.4byte 0x701f07a8
	.4byte 0xef273000
	.4byte 0x00505704
	.4byte 0x4c002037
	.4byte 0xf7000040
	.4byte 0x0040002a
	.4byte 0x1100206e
	.4byte 0x403b0046
	.4byte 0x00200000
	.4byte 0x2a00402c
	.4byte 0x244819f7
	.4byte 0x2d004000
	.4byte 0x043b0803
	.4byte 0x05830722
	.4byte 0x1817b705
	.4byte 0x0028003b
	.4byte 0x66015920
	.4byte 0x00072260
	.4byte 0x6805041a
	.4byte 0x3f1b01ff
	.4byte 0x06150458
	.4byte 0x40182a2c
	.4byte 0x8f000900
	.4byte 0xe0130302
	.4byte 0x0030cd0c
	.2byte 0x0002
	.global Data_0200611e
Data_0200611e:
	.2byte 0x0000
	.4byte 0xac862b05
	.4byte 0xaf643138
	.4byte 0x426905df
	.4byte 0xabd8ac4e
	.4byte 0xbe810f20
	.4byte 0x7015130e
	.4byte 0x58aaf4f3
	.4byte 0x1c57fed1
	.4byte 0xba72f5fa
	.4byte 0x15f3c78f
	.4byte 0x2c4bddfc
	.4byte 0x8c405c2e
	.4byte 0x5a901050
	.4byte 0x669c3d02
	.4byte 0x813f657c
	.4byte 0xc48af4a0
	.4byte 0x70f27d1e
	.4byte 0xdfcf8f1f
	.4byte 0x0be667d7
	.4byte 0x54be3c30
	.4byte 0xa442f8f4
	.4byte 0xeb17cfc7
	.4byte 0x058022f4
	.4byte 0xc3a1d0c0
	.4byte 0xf05c0a2b
	.4byte 0xbcc4f23e
	.4byte 0xe8ae7760
	.4byte 0x38745730
	.4byte 0xe6259c57
	.4byte 0x0e73008e
	.4byte 0x075f2408
	.4byte 0x1297a050
	.4byte 0x8f028687
	.4byte 0xebc03222
	.4byte 0x3e706883
	.4byte 0x8034e810
	.4byte 0x161c479c
	.4byte 0x9c090038
	.4byte 0x0a75ee0f
	.4byte 0xbf6039f8
	.4byte 0xf613af4e
	.4byte Tileset_Set112TilesA + 0x21e
	.4byte 0x3df8973f
	.4byte 0xf81f363e
	.4byte 0x63e27cd8
	.4byte 0x9df381f3
	.4byte 0xf7e793f3
	.4byte 0xd0e1c9fa
	.4byte 0xf258ce1d
	.4byte 0x3873c780
	.4byte 0xb93be3cc
	.4byte 0x2f3be3cc
	.4byte 0x6118fcc1
	.4byte 0x980e30ca
	.4byte 0x7cef9c33
	.4byte 0x29f1be0c
	.4byte 0xc7c786f0
	.4byte 0x5edecbe6
	.4byte 0x9239f837
	.4byte 0xebc2b9d8
	.4byte 0xd2f88ad8
	.4byte 0xa9bcc731
	.4byte 0x71e99cc7
	.4byte 0x5f547988
	.4byte 0x27049ac1
	.4byte 0xd7ae264f
	.4byte 0xc532a694
	.4byte 0x12fa7ae5
	.4byte 0xc43ef833
	.4byte 0x7d1e54f6
	.4byte 0x97fdb2f9
	.4byte 0xc6a7c3dd
	.4byte 0x3415957d
	.4byte 0x1f1897ad
	.4byte 0xac141210
	.4byte 0x047b4ae7
	.4byte 0x9eb43c03
	.4byte 0xcd180907
	.4byte 0x000079ea
	.4byte 0xf6450423
	.4byte 0xa7d80d99
	.4byte 0x78059b49
	.4byte 0x467a0580
	.4byte 0xb85c205b
	.4byte 0xd050008c
	.4byte 0x500b31ab
	.4byte 0x7833d893
	.4byte 0x8307f302
	.4byte 0x3a9c0502
	.4byte 0x3831102c
	.4byte 0x306a301c
	.4byte 0x3831101d
	.4byte 0x3026301c
	.4byte 0x38f2201a
	.4byte 0x0f2f2e80
	.4byte 0xc0861624
	.4byte 0x1f1ec23c
	.4byte 0x04f95397
	.4byte 0xc067c07c
	.4byte 0xc701f244
	.4byte 0x644c7e42
	.4byte 0x28ae605c
	.4byte 0xa2b8f289
	.4byte Battle_PurpleCaveBackdrop + 0x3607
	.4byte 0xb7fbd5ce
	.4byte 0x8db92233
	.4byte 0xa5310573
	.4byte 0x8e792a73
	.4byte 0x80804174
	.4byte 0x783318f3
	.4byte 0x3905033e
	.4byte 0x7c0bd78f
	.4byte 0x71e9ce5c
	.4byte 0xebc2b80d
	.4byte 0xfe12e605
	.4byte 0x385d04be
	.4byte 0xc06a9047
	.4byte 0x83a09e03
	.4byte 0x1efc23d7
	.4byte 0xf02b869f
	.4byte 0x3002f9f9
	.4byte 0x3f0df1e0
	.4byte 0x793f904f
	.4byte 0xc78f423c
	.4byte 0x3e067c3b
	.4byte 0x3e29ebe7
	.4byte 0xf7f39f16
	.4byte 0x33967f4d
	.4byte 0x780c9067
	.4byte 0x6ce3c57c
	.4byte 0x37ce61c6
	.4byte 0x37cc0e8c
	.4byte 0xcfc6d8fc
	.4byte 0xfcf83ef7
	.4byte 0xe07f9f03
	.4byte 0xfe7c0ff3
	.4byte 0xf03fcf81
	.4byte 0xff3e07f9
	.4byte 0xf816c7c0
	.2byte 0x0000
	.global Data_0200634e
Data_0200634e:
	.2byte 0x0100
	.4byte 0xcf81fe7c
	.4byte 0x07f9f03f
	.4byte 0x6080ff3e
	.4byte 0xead9f7b4
	.4byte 0x1e7cf8d3
	.4byte 0xca9f3dc1
	.4byte 0x8bf7f3e3
	.4byte 0x5b2fc50f
	.4byte 0xf00e1c7d
	.4byte 0xe7c5f8b1
	.4byte 0x4a1f1bef
	.4byte 0x8bf0f3e0
	.4byte 0x3e3dc58f
	.4byte 0xa1f0df7f
	.4byte 0x0a7dbf05
	.4byte 0xf8d3e16e
	.4byte 0x829f7b46
	.4byte 0xe7b8e75e
	.4byte 0x828df1f3
	.4byte 0x5ad8295e
	.4byte 0x3e060bd0
	.4byte 0x49a8e07d
	.4byte 0x6264f270
	.4byte 0x32a6829c
	.4byte 0x127b65c5
	.4byte 0xf6fcf033
	.4byte 0x54f6c43c
	.4byte 0xb2f97d1e
	.4byte 0xc3dd97fd
	.4byte 0x957dc6a7
	.4byte 0x8914e015
	.4byte 0x012101f1
	.4byte 0x166b2b8f
	.4byte 0xf00c11e4
	.4byte 0x241e7ad0
	.4byte 0xe7ab3460
	.4byte 0x108c0001
	.4byte 0x3667d914
	.4byte 0xde0f9f60
	.4byte 0x1b4b2a37
	.4byte 0x2c23c02c
	.4byte 0x02da33d0
	.4byte 0x0465c6e1
	.4byte 0x8d5e8282
	.4byte 0xc49a8459
	.4byte 0x9813c19e
	.4byte 0x2854187f
	.4byte 0x8161d4e0
	.4byte 0x80e1c388
	.4byte 0x80e98751
	.4byte 0x80e1c388
	.4byte 0x00d18131
	.4byte 0x7401c791
	.4byte 0xb1207979
	.4byte 0x7c37dd8f
	.4byte 0x00000000
	.global Data_02006430
Data_02006430:
	.4byte 0xffff0000
	.4byte 0x000000c0
	.4byte 0x800000c0
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02006460
Data_02006460:
	.4byte 0x00000124
	.4byte 0x00102122
	.4byte 0x00203124
	.4byte 0x00302124
	.4byte 0x00403128
	.4byte 0x00000125
	.4byte 0x00102120
	.4byte 0x00203125
	.4byte 0x00302125
	.4byte 0x00405125
	.4byte 0x00504125
	.4byte 0x00607125
	.4byte 0x00706125
	.4byte 0x00802128
	.4byte 0x00909125
	.4byte 0x00000126
	.4byte 0x00102121
	.4byte 0x00203126
	.4byte 0x00302126
	.4byte 0x00405126
	.4byte 0x00504126
	.4byte 0x00606126
	.4byte 0x00707126
	.4byte 0x00809126
	.4byte 0x00908126
	.4byte 0x00a0a126
	.4byte 0x00b0c126
	.4byte 0x00c0b126
	.4byte 0x00d0e126
	.4byte 0x00e0d126
	.4byte 0x00f04128
	.4byte 0x01010126
	.4byte 0x00000127
	.4byte 0x00102123
	.4byte 0x00607127
	.4byte 0x00706127
	.4byte 0x00809127
	.4byte 0x00908127
	.4byte 0x00a0b127
	.4byte 0x00b0a127
	.4byte 0x00c0d127
	.4byte 0x00d0c127
	.4byte 0x00e0f127
	.4byte 0x00f0e127
	.4byte 0x00501128
	.4byte 0x000001ff
	.global Data_02006518
Data_02006518:
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02006530
Data_02006530:
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x03080000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x03080000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00024000
	.4byte 0xffff013b
	.4byte 0x00000001
	.4byte 0x02580000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00024000
	.4byte 0xffff013b
	.4byte 0x00000001
	.4byte 0x02780000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00024000
	.4byte 0xffff013b
	.4byte 0x00000001
	.4byte 0x02a80000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00024000
	.4byte 0xffff019e
	.4byte 0x00000001
	.4byte 0x02e80000
	.4byte 0x00000000
	.4byte 0x00800000
	.4byte 0x00024000
	.4byte 0xffff019e
	.4byte .L_0200d748
	.4byte 0x02980000
	.4byte 0x00000000
	.4byte 0x01300000
	.4byte 0x00024000
	.4byte 0xffff019e
	.4byte .L_0200d6d4
	.4byte 0x02f80000
	.4byte 0x00000000
	.4byte 0x01300000
	.4byte 0x00024000
	.4byte 0xffff00fe
	.4byte 0x00000001
	.4byte 0x03180000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00024000
	.4byte 0xffff00fe
	.4byte 0x00000001
	.4byte 0x02280000
	.4byte 0x00000000
	.4byte 0x02280000
	.4byte 0x00024000
	.4byte 0xffff00fe
	.4byte 0x00000001
	.4byte 0x02680000
	.4byte 0x00000000
	.4byte 0x02280000
	.4byte 0x00024000
	.4byte 0xffff00fe
	.4byte 0x00000001
	.4byte 0x02a80000
	.4byte 0x00000000
	.4byte 0x02280000
	.4byte 0x00024000
	.4byte 0xffff00fe
	.4byte 0x00000001
	.4byte 0x02e80000
	.4byte 0x00000000
	.4byte 0x02280000
	.4byte 0x00024000
	.4byte 0xffff00fe
	.4byte 0x00000001
	.4byte 0x02280000
	.4byte 0x00000000
	.4byte 0x02c80000
	.4byte 0x00024000
	.4byte 0xffff00fe
	.4byte 0x00000001
	.4byte 0x02680000
	.4byte 0x00000000
	.4byte 0x02c80000
	.4byte 0x00024000
	.4byte 0xffff00fe
	.4byte 0x00000001
	.4byte 0x02a80000
	.4byte 0x00000000
	.4byte 0x02c80000
	.4byte 0x00024000
	.4byte 0xffff019e
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff019e
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff019e
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
	.global Data_02006710
Data_02006710:
	.4byte 0xffff019e
	.4byte .L_0200d7bc
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x02400000
	.4byte 0x00024000
	.4byte 0xffff01a2
	.4byte .L_0200d6c8
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x02680000
	.4byte 0x00024000
	.4byte 0xffff01a3
	.4byte .L_0200d6c8
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x02680000
	.4byte 0x00024000
	.4byte 0xffff01a3
	.4byte .L_0200d6c8
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x00024000
	.4byte 0xffff02a1
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff019c
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff01e9
	.4byte 0x00000007
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x03500000
	.4byte 0x00024000
	.4byte 0xffff0125
	.4byte 0x00000007
	.4byte 0x00900000
	.4byte 0x00000000
	.4byte 0x01000000
	.4byte 0x00024000
	.4byte 0xffff0126
	.4byte 0x00000007
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x01600000
	.4byte 0x00024000
	.4byte 0xffff0127
	.4byte 0x00000007
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00024000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x00700000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00024000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x00024000
	.4byte 0xffff019f
	.4byte 0x00000001
	.4byte 0x01700000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00024000
	.4byte 0xffff019e
	.4byte 0x00000001
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x01300000
	.4byte 0x00024000
	.4byte 0xffff019d
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00028000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02006890
Data_02006890:
	.4byte 0xffff019e
	.4byte .L_0200d890
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x00700000
	.4byte 0x00024000
	.4byte 0xffff019e
	.4byte .L_0200d93c
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x00700000
	.4byte 0x00024000
	.4byte 0xffff019e
	.4byte .L_0200da3c
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x01200000
	.4byte 0x00024000
	.4byte 0xffff01ac
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00024000
	.4byte 0xffff019d
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00020000
	.4byte 0xffff019e
	.4byte .L_0200da88
	.4byte 0x03c80000
	.4byte 0x00000000
	.4byte 0x00500000
	.4byte 0x00024000
	.4byte 0xffff01a3
	.4byte .L_0200d6c8
	.4byte 0x03680000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00024000
	.4byte 0xffff02a1
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff019c
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
.L_0200e980:
	.4byte 0x00000016
	.4byte 0x0000001f
	.4byte 0x00000000
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000006
	.4byte 0x00500000
	.4byte 0x00000016
	.4byte 0x00000016
	.4byte 0x00500000
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00008000
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00000800
	.4byte 0x00000004
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte Data_02000000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000004
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte Data_02000000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0x00000000
	.global Data_02006a0c
Data_02006a0c:
	.4byte 0xffff0126
	.4byte 0x00000007
	.4byte 0x02380000
	.4byte 0x00000000
	.4byte 0x00900000
	.4byte 0x00024000
	.4byte 0xffff0125
	.4byte 0x00000007
	.4byte 0x02200000
	.4byte 0x00000000
	.4byte 0x00f00000
	.4byte 0x00024000
	.4byte 0xffff0125
	.4byte 0x00000007
	.4byte 0x02200000
	.4byte 0x00000000
	.4byte 0x01100000
	.4byte 0x00024000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x02680000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00024000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x02080000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00024000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte Data_02000000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00020000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x02400000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00028000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x02580000
	.4byte 0x00000000
	.4byte 0x00b00000
	.4byte 0x00028000
	.4byte 0xffff019e
	.4byte .L_0200e980
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte Data_02000000
	.4byte 0x00024000
	.4byte 0xffff01a2
	.4byte .L_0200d6c8
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x02080000
	.4byte 0x00024000
	.4byte 0xffff01a3
	.4byte .L_0200d6c8
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x02080000
	.4byte 0x00024000
	.4byte 0xffff01a3
	.4byte .L_0200d6c8
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x02280000
	.4byte 0x00024000
	.4byte 0xffff0127
	.4byte 0x00000007
	.4byte 0x02580000
	.4byte 0x00000000
	.4byte 0x00cc0000
	.4byte 0x00024000
	.4byte 0xffff02a1
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff019c
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
	.global Data_02006b8c
Data_02006b8c:
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02006b98
Data_02006b98:
	.4byte 0x00000202
	.4byte 0xffff001e
	.4byte Func_020000d0
	.4byte 0x00008602
	.4byte 0xffff001f
	.4byte Func_02000168
	.4byte 0x00000202
	.4byte 0xffff001f
	.4byte Func_02000170
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000031
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000021
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000031
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000021
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000001
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000051
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00000001
	.4byte 0xffff000c
	.4byte 0x0000000c
	.4byte 0x00000051
	.4byte 0xffff000d
	.4byte 0x0000000d
	.4byte 0x00000001
	.4byte 0xffff000e
	.4byte 0x0000000e
	.4byte 0x00000051
	.4byte 0xffff000f
	.4byte 0x0000000f
	.4byte 0x00000008
	.4byte 0xffff0000
	.4byte Func_02000248
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte Func_0200025c
	.4byte 0x10008c15
	.4byte 0xffff000a
	.4byte Func_02000248
	.4byte 0x00008c15
	.4byte 0xffff000a
	.4byte Func_0200025c
	.4byte 0x00008c15
	.4byte 0xffff000b
	.4byte Func_020002ec
	.4byte 0x00008c15
	.4byte 0xffff000c
	.4byte Func_020002ec
	.4byte 0x00009315
	.4byte 0xffff000a
	.4byte Func_020002ec
	.4byte 0x00009315
	.4byte 0xffff000b
	.4byte Func_020002ec
	.4byte 0x00009315
	.4byte 0xffff000c
	.4byte Func_020002ec
	.4byte 0x00000602
	.4byte 0x0a380021
	.4byte Func_0200021c
	.4byte 0x00008c15
	.4byte 0x0a380010
	.4byte Func_020001f4
	.4byte 0x00008c15
	.4byte 0xffff0011
	.4byte 0x00000000
	.4byte 0x10008c15
	.4byte 0xffff0012
	.4byte Func_02000280
	.4byte 0x10008c15
	.4byte 0xffff0013
	.4byte Func_02000280
	.4byte 0x10008c15
	.4byte 0xffff0014
	.4byte Func_02000280
	.4byte 0x00008c15
	.4byte 0xffff0012
	.4byte Func_020002b8
	.4byte 0x00008c15
	.4byte 0xffff0013
	.4byte Func_020002b8
	.4byte 0x00008c15
	.4byte 0xffff0014
	.4byte Func_020002b8
	.4byte 0x10008c15
	.4byte 0xffff0015
	.4byte Func_02000280
	.4byte 0x10008c15
	.4byte 0xffff0016
	.4byte Func_02000280
	.4byte 0x00008c15
	.4byte 0xffff0015
	.4byte Func_020002b8
	.4byte 0x00008c15
	.4byte 0xffff0016
	.4byte Func_020002b8
	.4byte 0x00008c15
	.4byte 0xffff0017
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0xffff0058
	.4byte Func_0200297c
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02006d78
Data_02006d78:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000021
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000031
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000002
	.4byte 0x02200021
	.4byte Func_020004f8
	.4byte 0x00000202
	.4byte 0xffff001e
	.4byte Func_02000418
	.4byte 0x00000002
	.4byte 0xffff001e
	.4byte Func_02002b88
	.4byte 0x00000002
	.4byte 0xffff0058
	.4byte Func_0200297c
	.4byte 0x00008c15
	.4byte 0xffff0016
	.4byte 0x00000000
	.4byte 0x50009705
	.4byte 0x0a440014
	.4byte Func_02000444
	.4byte 0x50009705
	.4byte 0x0a450015
	.4byte Func_02000480
	.4byte 0x50009705
	.4byte 0x0a460016
	.4byte Func_020004bc
	.4byte 0x10009a15
	.4byte 0xffff0009
	.4byte Func_020000bc
	.4byte 0x60009a15
	.4byte 0xffff000b
	.4byte Func_02000554
	.4byte 0x20009a15
	.4byte 0xffff000b
	.4byte Func_02000670
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02006e38
Data_02006e38:
	.4byte 0x00000002
	.4byte 0xffff0014
	.4byte Func_02000944
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff000f
	.4byte 0x0000000f
	.4byte 0x00000001
	.4byte 0xffff0010
	.4byte 0x00000010
	.4byte 0x50008905
	.4byte 0xffff0002
	.4byte Func_02000754
	.4byte 0x50008905
	.4byte 0xffff0003
	.4byte Func_02000760
	.4byte 0x50008905
	.4byte 0xffff0004
	.4byte Func_0200076c
	.4byte 0x50008905
	.4byte 0xffff0005
	.4byte Func_02000778
	.4byte 0x50008905
	.4byte 0xffff0006
	.4byte Func_02000784
	.4byte 0x50008905
	.4byte 0xffff0007
	.4byte Func_02000790
	.4byte 0x50008905
	.4byte 0xffff0008
	.4byte Func_0200079c
	.4byte 0x50008905
	.4byte 0xffff0009
	.4byte Func_020007a8
	.4byte 0x50008905
	.4byte 0xffff000a
	.4byte Func_020007b4
	.4byte 0x50008905
	.4byte 0xffff000b
	.4byte Func_020007c0
	.4byte 0x50008905
	.4byte 0xffff000c
	.4byte Func_020007cc
	.4byte 0x50008905
	.4byte 0xffff000d
	.4byte Func_020007d8
	.4byte 0x50008905
	.4byte 0xffff000e
	.4byte Func_020007e4
	.4byte 0x00008c15
	.4byte 0x0358000b
	.4byte Func_020007f0
	.4byte 0x00008c15
	.4byte 0x0a37000c
	.4byte Func_020008a0
	.4byte 0x50009085
	.4byte 0xffff0000
	.4byte Func_020008bc
	.4byte 0x40009085
	.4byte 0xffff0000
	.4byte Func_02000900
	.4byte 0x00000002
	.4byte 0xffff0058
	.4byte Func_0200297c
	.4byte 0x00000002
	.4byte 0xffff005a
	.4byte Func_02002ba0
	.4byte 0x00000002
	.4byte 0xffff000e
	.4byte Func_02002ce0
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02006f64
Data_02006f64:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000021
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000031
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000021
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000031
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000051
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000002
	.4byte 0xffff001e
	.4byte Func_02004140
	.4byte 0x00000202
	.4byte 0xffff0021
	.4byte Func_02000b40
	.4byte 0x00000002
	.4byte 0xffff0021
	.4byte Func_02002b88
	.4byte 0x00000002
	.4byte 0xffff0058
	.4byte Func_0200297c
	.4byte 0x10009a15
	.4byte 0xffff0011
	.4byte Func_020000bc
	.4byte 0x60009a15
	.4byte 0xffff0010
	.4byte Func_02000aac
	.4byte 0x20009a15
	.4byte 0xffff0010
	.4byte Func_02000ad8
	.4byte 0x00001815
	.4byte 0x0220000b
	.4byte Func_02000cf4
	.4byte 0x00001815
	.4byte 0x0221000c
	.4byte Func_02000d38
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_0200703c
Data_0200703c:
	.4byte 0xffffffff
	.global Data_02007040
Data_02007040:
	.4byte 0x00000001
	.global Data_02007044
Data_02007044:
	.4byte 0x00000027
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x0000000c
	.4byte 0x00000026
	.global Data_02007058
Data_02007058:
	.4byte .L_0200de44
	.4byte .L_0200de80
	.4byte .L_0200debc
