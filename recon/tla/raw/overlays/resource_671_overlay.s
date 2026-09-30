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
	bl Func_0200563c
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
	bl Func_02005624
	ldr r2, .L_02008274
	mov r3, r8
	ands r3, r5
	lsls r3, r3, #2
	ldr r1, [r2, r3]
	adds r0, r6, #0
	mov r10, r3
	bl Func_02005634
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
	bl Func_0200574c
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
	bl Func_02005624
	ldr r1, [r7, #28]
	adds r0, r6, #0
	bl Func_02005634
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
	.4byte Data_02005ae4
.L_02008278:
	.4byte Func_02000080
.L_0200827c:
	.4byte 0xffff0000
	.section .text.x02008280,"ax",%progbits
	.global Func_02000280
	.thumb_func
Func_02000280:
	push {lr}
	movs r0, #33
	movs r1, #9
	bl Func_020057cc
	pop {pc}
	.section .text.x0200828c,"ax",%progbits
	.global Func_0200028c
	.thumb_func
Func_0200028c:
	push {r5, r6, r7, lr}
	adds r6, r0, #0
	adds r5, r6, #0
	adds r5, #98
	ldrb r3, [r5]
	adds r7, r3, #0
	cmp r7, #0
	beq .L_020082a2
	adds r3, #255
	strb r3, [r5]
	b .L_020082d0
.L_020082a2:
	bl Random16Far
	lsls r3, r0, #2
	adds r3, r3, r0
	lsls r3, r3, #3
	lsrs r3, r3, #16
	adds r3, #10
	strb r3, [r5]
	adds r2, r6, #0
	adds r2, #99
	ldrb r3, [r2]
	cmp r3, #0
	beq .L_020082c4
	strb r7, [r2]
	adds r3, r6, #0
	adds r3, #100
	b .L_020082cc
.L_020082c4:
	movs r3, #1
	strb r3, [r2]
	adds r3, r6, #0
	adds r3, #102
.L_020082cc:
	ldrh r3, [r3]
	strh r3, [r6, #6]
.L_020082d0:
	movs r0, #1
	pop {r5, r6, r7, pc}
	.section .text.x020082d4,"ax",%progbits
	.global Func_020002d4
	.thumb_func
Func_020002d4:
	ldr r0, .L_020082d8
	bx lr
.L_020082d8:
	.4byte Data_02005dcc
	.section .text.x020082dc,"ax",%progbits
	.global Func_020002dc
	.thumb_func
Func_020002dc:
	push {lr}
	ldr r3, .L_02008328
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_0200832c
	cmp r2, r3
	bne .L_020082f4
	ldr r0, .L_02008330
	b .L_02008326
.L_020082f4:
	ldr r3, .L_02008334
	cmp r2, r3
	bne .L_020082fe
	ldr r0, .L_02008338
	b .L_02008326
.L_020082fe:
	ldr r3, .L_0200833c
	cmp r2, r3
	bne .L_02008308
	ldr r0, .L_02008340
	b .L_02008326
.L_02008308:
	ldr r3, .L_02008344
	cmp r2, r3
	bne .L_02008312
	ldr r0, .L_02008348
	b .L_02008326
.L_02008312:
	ldr r3, .L_0200834c
	cmp r2, r3
	bne .L_0200831c
	ldr r0, .L_02008350
	b .L_02008326
.L_0200831c:
	ldr r3, .L_02008354
	movs r0, #0
	cmp r2, r3
	bne .L_02008326
	ldr r0, .L_02008358
.L_02008326:
	pop {pc}
.L_02008328:
	.4byte gPartyState
.L_0200832c:
	.4byte 0x0000007b
.L_02008330:
	.4byte Data_02005dfc
.L_02008334:
	.4byte 0x00000073
.L_02008338:
	.4byte Data_02005e2c
.L_0200833c:
	.4byte 0x00000074
.L_02008340:
	.4byte Data_02005e4c
.L_02008344:
	.4byte 0x00000075
.L_02008348:
	.4byte Data_02005e8c
.L_0200834c:
	.4byte 0x00000076
.L_02008350:
	.4byte Data_02005ecc
.L_02008354:
	.4byte 0x00000077
.L_02008358:
	.4byte Data_02005eec
	.section .text.x0200835c,"ax",%progbits
	.global Func_0200035c
	.thumb_func
Func_0200035c:
	ldr r0, .L_02008360
	bx lr
.L_02008360:
	.4byte Data_02005f0c
	.section .text.x02008364,"ax",%progbits
	.global Func_02000364
	.thumb_func
Func_02000364:
	push {r5, r6, lr}
	ldr r3, .L_02008394
	adds r5, r0, #0
	ldr r6, [r3]
	movs r1, #10
	adds r0, r6, #0
	bl Engine_MathModulo
	cmp r0, #0
	bne .L_0200838e
	ldr r3, [r5, #80]
	movs r1, #20
	adds r0, r6, #0
	ldr r5, [r3, #40]
	bl Engine_MathModulo
	movs r1, #10
	bl IwramUnsignedDivideEntry
	adds r0, #3
	strb r0, [r5, #5]
.L_0200838e:
	movs r0, #1
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02008394:
	.4byte Data_0300122c
	.section .text.x02008398,"ax",%progbits
	.global Func_02000398
	.thumb_func
Func_02000398:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r7, r0, #0
	adds r3, r7, #0
	adds r3, #98
	ldrb r0, [r3]
	adds r6, r1, #0
	lsls r0, r0, #16
	movs r1, #180
	mov r8, r2
	sub sp, #12
	bl Engine_MathDivide
	adds r3, r7, #0
	adds r3, #100
	movs r2, #0
	ldrsh r3, [r3, r2]
	mov r5, sp
	adds r3, #16
	lsls r3, r3, #16
	str r3, [r5]
	movs r3, #128
	lsls r3, r3, #13
	str r3, [r5, #4]
	adds r3, r7, #0
	adds r3, #102
	movs r2, #0
	ldrsh r3, [r3, r2]
	adds r2, r5, #0
	lsls r3, r3, #16
	str r3, [r5, #8]
	ldr r3, .L_02008404
	ldr r3, [r3]
	lsls r1, r3, #1
	adds r1, r1, r3
	lsls r1, r1, #12
	movs r3, #128
	lsls r3, r3, #8
	adds r1, r1, r0
	movs r0, #208
	adds r1, r1, r3
	lsls r0, r0, #12
	bl Vector_AddPolarOffsetFar
	ldr r3, [r5]
	mov r2, r8
	str r3, [r6]
	ldr r3, [r5, #8]
	add sp, #12
	str r3, [r2]
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_02008404:
	.4byte Data_02005af0
	.section .text.x02008408,"ax",%progbits
	.global Func_02000408
	.thumb_func
Func_02000408:
	push {r5, r6, lr}
	adds r5, r0, #0
	adds r6, r5, #0
	adds r6, #98
	ldrb r0, [r6]
	movs r1, #180
	lsls r0, r0, #16
	bl Engine_MathDivide
	adds r2, r5, #0
	adds r2, #35
	movs r3, #0
	strb r3, [r2]
	adds r3, r5, #0
	adds r3, #100
	movs r2, #0
	ldrsh r3, [r3, r2]
	adds r1, r0, #0
	adds r3, #16
	lsls r3, r3, #16
	str r3, [r5, #8]
	movs r3, #128
	lsls r3, r3, #13
	str r3, [r5, #12]
	adds r3, r5, #0
	adds r3, #102
	movs r2, #0
	ldrsh r3, [r3, r2]
	movs r0, #208
	lsls r3, r3, #16
	str r3, [r5, #16]
	movs r3, #128
	lsls r3, r3, #8
	adds r2, r5, #0
	adds r1, r1, r3
	adds r2, #8
	lsls r0, r0, #12
	bl Vector_AddPolarOffsetFar
	ldr r3, .L_02008474
	ldrb r2, [r6]
	ldr r3, [r3]
	adds r2, r2, r3
	cmp r2, #0
	bge .L_02008464
	movs r2, #179
.L_02008464:
	cmp r2, #179
	ble .L_0200846a
	movs r2, #0
.L_0200846a:
	ldr r3, .L_02008478
	strb r2, [r6]
	movs r0, #1
	str r3, [r5, #104]
	pop {r5, r6, pc}
.L_02008474:
	.4byte Data_02005af0
.L_02008478:
	.4byte Func_02000398
	.section .text.x0200847c,"ax",%progbits
	.global Func_0200047c
	.thumb_func
Func_0200047c:
	push {r5, lr}
	ldr r2, [r0, #104]
	cmp r2, #0
	beq .L_020084b4
	ldr r3, [r2, #8]
	movs r1, #128
	str r3, [r0, #8]
	lsls r1, r1, #13
	ldr r3, [r2, #12]
	adds r5, r0, #0
	adds r3, r3, r1
	str r3, [r0, #12]
	adds r5, #98
	ldr r3, [r2, #16]
	adds r2, r0, #0
	str r3, [r0, #16]
	adds r2, #8
	ldrb r3, [r5]
	lsls r3, r3, #8
	strh r3, [r0, #6]
	movs r0, #192
	adds r1, r3, #0
	lsls r0, r0, #11
	bl Vector_AddPolarOffsetFar
	ldrb r3, [r5]
	adds r3, #16
	strb r3, [r5]
.L_020084b4:
	movs r0, #1
	pop {r5, pc}
	.section .text.x020084b8,"ax",%progbits
	.global Func_020004b8
	.thumb_func
Func_020004b8:
	push {lr}
	ldr r3, .L_02008510
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008514
	ldr r0, .L_02008518
	cmp r2, r3
	bne .L_020084d2
	ldr r0, .L_0200851c
	b .L_0200850c
.L_020084d2:
	ldr r3, .L_02008520
	cmp r2, r3
	bne .L_020084dc
	ldr r0, .L_02008524
	b .L_0200850c
.L_020084dc:
	ldr r3, .L_02008528
	cmp r2, r3
	bne .L_020084e6
	ldr r0, .L_0200852c
	b .L_0200850c
.L_020084e6:
	ldr r3, .L_02008530
	cmp r2, r3
	bne .L_020084f0
	ldr r0, .L_02008534
	b .L_0200850c
.L_020084f0:
	ldr r3, .L_02008538
	cmp r2, r3
	bne .L_020084fa
	ldr r0, .L_0200853c
	b .L_0200850c
.L_020084fa:
	ldr r3, .L_02008540
	cmp r2, r3
	bne .L_02008504
	ldr r0, .L_02008544
	b .L_0200850c
.L_02008504:
	ldr r3, .L_02008548
	cmp r2, r3
	bne .L_0200850c
	ldr r0, .L_0200854c
.L_0200850c:
	pop {pc}
	.2byte 0x0000
.L_02008510:
	.4byte gPartyState
.L_02008514:
	.4byte 0x00000073
.L_02008518:
	.4byte Data_02006b20
.L_0200851c:
	.4byte Data_02005fd4
.L_02008520:
	.4byte 0x00000074
.L_02008524:
	.4byte Data_02006004
.L_02008528:
	.4byte 0x00000075
.L_0200852c:
	.4byte Data_02006244
.L_02008530:
	.4byte 0x00000076
.L_02008534:
	.4byte Data_020064cc
.L_02008538:
	.4byte 0x00000077
.L_0200853c:
	.4byte Data_020067e4
.L_02008540:
	.4byte 0x00000079
.L_02008544:
	.4byte Data_02006898
.L_02008548:
	.4byte 0x0000007b
.L_0200854c:
	.4byte Data_02006a90
	.section .text.x02008550,"ax",%progbits
	.global Func_02000550
	.thumb_func
Func_02000550:
	push {r5, r6, r7, lr}
	movs r0, #9
	bl Object_GetById
	adds r7, r0, #0
	movs r0, #0
	bl Func_02005834
	cmp r0, #1
	bne .L_020085a8
	ldr r3, .L_020085ac
	movs r0, #0
	ldr r6, [r3]
	bl Func_020020f0
	movs r0, #130
	lsls r0, r0, #1
	adds r0, #255
	bl Func_0200560c
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r1, #179
	lsls r1, r1, #1
	adds r2, r3, r1
	movs r3, #0
	strh r3, [r2]
	movs r5, #0
.L_0200858a:
	ldr r3, .L_020085b0
	adds r1, r7, #0
	ldrsb r3, [r3, r5]
	adds r1, #98
	adds r2, r6, #0
	muls r2, r3
	ldrb r3, [r1]
	movs r0, #1
	adds r3, r3, r2
	strb r3, [r1]
	adds r5, #1
	bl WaitFrames
	cmp r5, #24
	ble .L_0200858a
.L_020085a8:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_020085ac:
	.4byte Data_02005af0
.L_020085b0:
	.4byte Data_02006b38
	.section .text.x020085b4,"ax",%progbits
	.global Func_020005b4
	.thumb_func
Func_020005b4:
	push {r5, r6, r7, lr}
	adds r6, r0, #0
	adds r7, r1, #0
	movs r5, #8
.L_020085bc:
	adds r0, r5, #0
	bl Object_GetById
	cmp r0, #0
	beq .L_02008614
	adds r3, r0, #0
	adds r3, #84
	ldrb r3, [r3]
	cmp r3, #1
	bne .L_02008614
	ldr r3, [r0, #80]
	ldr r3, [r3, #40]
	cmp r3, #0
	beq .L_02008614
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, r7
	bne .L_02008614
	ldr r1, [r0, #8]
	ldr r3, [r6, #8]
	subs r2, r1, r3
	cmp r2, #0
	blt .L_020085f2
	ldr r3, .L_02008620
	cmp r2, r3
	ble .L_020085fa
	b .L_02008614
.L_020085f2:
	ldr r2, .L_02008620
	subs r3, r3, r1
	cmp r3, r2
	bgt .L_02008614
.L_020085fa:
	ldr r1, [r0, #16]
	ldr r3, [r6, #16]
	subs r2, r1, r3
	cmp r2, #0
	blt .L_0200860c
	ldr r3, .L_02008620
	cmp r2, r3
	ble .L_0200861c
	b .L_02008614
.L_0200860c:
	ldr r2, .L_02008620
	subs r3, r3, r1
	cmp r3, r2
	ble .L_0200861c
.L_02008614:
	adds r5, #1
	cmp r5, #63
	ble .L_020085bc
	movs r0, #0
.L_0200861c:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008620:
	.4byte 0x0017ffff
	.section .text.x02008624,"ax",%progbits
	.global Func_02000624
	.thumb_func
Func_02000624:
	push {r5, r6, lr}
	ldr r3, [r0, #8]
	sub sp, #12
	mov r2, sp
	str r3, [r2]
	adds r6, r1, #0
	ldr r3, [r0, #16]
	movs r5, #8
	str r3, [r2, #8]
	ldrh r1, [r0, #6]
	movs r0, #128
	lsls r0, r0, #13
	bl Vector_AddPolarOffsetFar
.L_02008640:
	adds r0, r5, #0
	bl Object_GetById
	cmp r0, #0
	beq .L_0200869a
	adds r3, r0, #0
	adds r3, #84
	ldrb r3, [r3]
	cmp r3, #1
	bne .L_0200869a
	ldr r3, [r0, #80]
	ldr r3, [r3, #40]
	cmp r3, #0
	beq .L_0200869a
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, r6
	bne .L_0200869a
	mov r4, sp
	ldr r1, [r0, #8]
	ldr r3, [r4]
	subs r2, r1, r3
	cmp r2, #0
	blt .L_02008678
	ldr r3, .L_020086a8
	cmp r2, r3
	ble .L_02008680
	b .L_0200869a
.L_02008678:
	ldr r2, .L_020086a8
	subs r3, r3, r1
	cmp r3, r2
	bgt .L_0200869a
.L_02008680:
	ldr r1, [r0, #16]
	ldr r3, [r4, #8]
	subs r2, r1, r3
	cmp r2, #0
	blt .L_02008692
	ldr r3, .L_020086a8
	cmp r2, r3
	ble .L_020086a2
	b .L_0200869a
.L_02008692:
	ldr r2, .L_020086a8
	subs r3, r3, r1
	cmp r3, r2
	ble .L_020086a2
.L_0200869a:
	adds r5, #1
	cmp r5, #63
	ble .L_02008640
	movs r0, #0
.L_020086a2:
	add sp, #12
	pop {r5, r6, pc}
	.2byte 0x0000
.L_020086a8:
	.4byte 0x0017ffff
	.section .text.x020086ac,"ax",%progbits
	.global Func_020006ac
	.thumb_func
Func_020006ac:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	adds r5, r0, #0
	adds r7, r5, #0
	adds r7, #85
	mov r10, r2
	mov r8, r1
	adds r6, r3, #0
	bl Func_020057d4
	ldrb r3, [r7]
	movs r1, #6
	adds r0, r5, #0
	mov r9, r3
	bl Func_02005624
	movs r0, #6
	bl WaitFrames
	movs r0, #152
	bl Func_02005854
	adds r0, r5, #0
	movs r1, #7
	bl Func_02005624
	movs r3, #192
	lsls r3, r3, #10
	str r3, [r5, #48]
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r5, #52]
	str r6, [r5, #40]
	ldrb r2, [r7]
	movs r3, #126
	ands r3, r2
	strb r3, [r7]
	adds r0, r5, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	ldr r1, [r5, #8]
	ldr r3, [r5, #16]
	ldr r2, [r5, #12]
	add r1, r8
	add r3, r10
	adds r0, r5, #0
	bl Func_0200565c
	adds r0, r5, #0
	bl Func_02005664
	mov r3, r9
	strb r3, [r7]
	adds r0, r5, #0
	movs r1, #6
	bl Func_02005624
	adds r0, r5, #0
	movs r1, #1
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #3
	bl WaitFrames
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x02008740,"ax",%progbits
	.global Func_02000740
	.thumb_func
Func_02000740:
	push {r5, r6, r7, lr}
	ldr r3, .L_02008780
	ldr r2, [r3]
	cmp r2, #0
	beq .L_0200877e
	lsls r3, r2, #4
	adds r3, r3, r2
	lsls r3, r3, #4
	adds r3, r3, r2
	lsls r6, r3, #2
	bl Func_0200584c
	bl Object_GetById
	movs r1, #179
	lsls r1, r1, #1
	negs r7, r6
	adds r5, r0, #0
	bl Func_020005b4
	cmp r0, #0
	beq .L_02008778
	ldrh r3, [r0, #6]
	movs r2, #128
	lsls r2, r2, #8
	cmp r3, r2
	bne .L_02008778
	adds r7, r6, #0
.L_02008778:
	ldrh r3, [r5, #6]
	adds r3, r3, r7
	strh r3, [r5, #6]
.L_0200877e:
	pop {r5, r6, r7, pc}
.L_02008780:
	.4byte Data_02005af0
	.section .text.x02008784,"ax",%progbits
	.global Func_02000784
	.thumb_func
Func_02000784:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r5, .L_02008968
	sub sp, #36
	ldr r3, [r5]
	cmp r3, #0
	bne .L_0200879e
	b .L_0200895a
.L_0200879e:
	bl Func_0200584c
	bl Object_GetById
	movs r1, #102
	adds r1, #255
	adds r7, r0, #0
	bl Func_020005b4
	ldr r2, [r5]
	ldr r3, .L_0200896c
	mov r9, r0
	adds r1, r2, #0
	muls r1, r3
	str r1, [sp, #20]
	cmp r0, #0
	bne .L_020087c2
	b .L_02008952
.L_020087c2:
	ldr r3, [r0, #12]
	movs r2, #128
	lsls r2, r2, #13
	adds r3, r3, r2
	ldr r2, [r7, #12]
	cmp r3, r2
	ble .L_020087d2
	b .L_0200895a
.L_020087d2:
	ldrh r3, [r0, #6]
	movs r1, #128
	lsls r1, r1, #8
	cmp r3, r1
	bne .L_020087e2
	ldr r2, [sp, #20]
	negs r2, r2
	str r2, [sp, #20]
.L_020087e2:
	mov r1, r9
	ldr r3, [r1, #8]
	ldr r2, [r7, #8]
	subs r2, r2, r3
	str r2, [sp, #8]
	ldr r2, [r7, #16]
	ldr r3, [r1, #16]
	subs r2, r2, r3
	str r2, [sp, #4]
	ldr r2, [sp, #20]
	negs r2, r2
	adds r0, r2, #0
	str r2, [sp, #0]
	bl Math_Sine
	adds r6, r0, #0
	ldr r0, [sp, #0]
	bl Math_Cosine
	ldr r3, .L_02008970
	ldr r1, [sp, #8]
	mov r8, r0
	mov lr, r3
	.2byte 0xf800
	ldr r2, .L_02008970
	ldr r1, [sp, #4]
	adds r5, r0, #0
	adds r0, r6, #0
	mov lr, r2
	.2byte 0xf800
	mov r1, r9
	ldr r3, [r1, #8]
	ldr r2, .L_02008970
	adds r3, r3, r5
	adds r3, r3, r0
	str r3, [sp, #16]
	ldr r1, [sp, #8]
	adds r0, r6, #0
	mov lr, r2
	.2byte 0xf800
	ldr r3, .L_02008970
	adds r5, r0, #0
	ldr r1, [sp, #4]
	mov r0, r8
	mov lr, r3
	.2byte 0xf800
	mov r1, r9
	ldr r3, [r1, #16]
	ldr r1, [sp, #16]
	subs r3, r3, r5
	adds r3, r3, r0
	str r3, [sp, #12]
	ldr r3, [r7, #36]
	add r2, sp, #24
	adds r3, r1, r3
	str r3, [r2]
	mov r10, r2
	mov r2, r9
	ldr r3, [r2, #12]
	mov r1, r10
	str r3, [r1, #4]
	ldr r3, [r7, #44]
	ldr r2, [sp, #12]
	adds r0, r7, #0
	adds r3, r2, r3
	str r3, [r1, #8]
	bl Func_02005684
	cmp r0, #0
	bne .L_02008952
	ldr r3, [sp, #20]
	movs r5, #168
	lsls r5, r5, #5
	adds r5, #85
	subs r5, r5, r3
	adds r0, r5, #0
	bl Math_Sine
	adds r6, r0, #0
	adds r0, r5, #0
	bl Math_Cosine
	ldr r2, .L_02008970
	ldr r1, [sp, #8]
	mov r8, r0
	mov lr, r2
	.2byte 0xf800
	ldr r3, .L_02008970
	ldr r1, [sp, #4]
	adds r5, r0, #0
	adds r0, r6, #0
	mov lr, r3
	.2byte 0xf800
	mov r1, r9
	ldr r3, [r1, #8]
	ldr r2, .L_02008970
	adds r3, r3, r5
	adds r3, r3, r0
	ldr r1, [sp, #8]
	adds r0, r6, #0
	mov r11, r3
	mov lr, r2
	.2byte 0xf800
	ldr r3, .L_02008970
	adds r5, r0, #0
	ldr r1, [sp, #4]
	mov r0, r8
	mov lr, r3
	.2byte 0xf800
	mov r1, r9
	ldr r3, [r1, #16]
	mov r2, r10
	subs r3, r3, r5
	adds r0, r3, r0
	ldr r3, [r7, #36]
	mov r1, r10
	add r3, r11
	str r3, [r2]
	ldr r3, [r7, #44]
	adds r3, r0, r3
	str r3, [r2, #8]
	adds r0, r7, #0
	bl Func_02005684
	cmp r0, #0
	bne .L_02008952
	ldr r3, [sp, #0]
	ldr r1, .L_02008974
	adds r5, r3, r1
	adds r0, r5, #0
	bl Math_Sine
	adds r6, r0, #0
	adds r0, r5, #0
	bl Math_Cosine
	ldr r2, .L_02008970
	ldr r1, [sp, #8]
	mov r8, r0
	mov lr, r2
	.2byte 0xf800
	ldr r3, .L_02008970
	ldr r1, [sp, #4]
	adds r5, r0, #0
	adds r0, r6, #0
	mov lr, r3
	.2byte 0xf800
	mov r1, r9
	ldr r3, [r1, #8]
	ldr r2, .L_02008970
	adds r3, r3, r5
	adds r3, r3, r0
	ldr r1, [sp, #8]
	adds r0, r6, #0
	mov r11, r3
	mov lr, r2
	.2byte 0xf800
	ldr r3, .L_02008970
	adds r5, r0, #0
	ldr r1, [sp, #4]
	mov r0, r8
	mov lr, r3
	.2byte 0xf800
	mov r1, r9
	ldr r3, [r1, #16]
	mov r2, r10
	subs r3, r3, r5
	adds r0, r3, r0
	ldr r3, [r7, #36]
	mov r1, r10
	add r3, r11
	str r3, [r2]
	ldr r3, [r7, #44]
	adds r3, r0, r3
	str r3, [r2, #8]
	adds r0, r7, #0
	bl Func_02005684
	cmp r0, #0
	bne .L_02008952
	ldr r3, [sp, #16]
	str r3, [r7, #8]
	ldr r1, [sp, #12]
	str r1, [r7, #16]
.L_02008952:
	ldrh r3, [r7, #6]
	ldr r2, [sp, #20]
	adds r3, r3, r2
	strh r3, [r7, #6]
.L_0200895a:
	add sp, #36
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_02008968:
	.4byte Data_02005af0
.L_0200896c:
	.4byte 0xfffffe94
.L_02008970:
	.4byte IwramMulQ16
.L_02008974:
	.4byte 0xffffeaab
	.section .text.x02008978,"ax",%progbits
	.global Func_02000978
	.thumb_func
Func_02000978:
	push {r5, r6, lr}
	ldr r6, .L_02008a18
	ldr r3, [r6]
	cmp r3, #0
	beq .L_02008a16
	bl Func_0200584c
	bl Object_GetById
	movs r1, #104
	adds r1, #255
	adds r5, r0, #0
	bl Func_020005b4
	ldr r3, [r6]
	lsls r1, r3, #15
	cmp r0, #0
	beq .L_02008a16
	ldrh r3, [r0, #6]
	movs r2, #128
	lsls r2, r2, #8
	cmp r3, r2
	bne .L_020089a8
	negs r1, r1
.L_020089a8:
	ldr r2, [r0, #16]
	ldr r3, [r5, #16]
	ldr r4, .L_02008a1c
	subs r3, r2, r3
	cmp r3, r4
	bge .L_020089bc
	movs r4, #128
	lsls r4, r4, #10
	adds r3, r2, r4
	str r3, [r5, #16]
.L_020089bc:
	ldr r2, [r0, #16]
	ldr r3, [r5, #16]
	movs r4, #128
	subs r3, r2, r3
	lsls r4, r4, #10
	cmp r3, r4
	ble .L_020089d0
	ldr r4, .L_02008a1c
	adds r3, r2, r4
	str r3, [r5, #16]
.L_020089d0:
	ldr r3, [r5, #8]
	adds r3, r3, r1
	str r3, [r5, #8]
	ldr r0, [r0, #8]
	subs r2, r0, r3
	cmp r2, #0
	blt .L_020089e8
	movs r3, #160
	lsls r3, r3, #13
	cmp r2, r3
	bge .L_020089f2
	b .L_02008a16
.L_020089e8:
	movs r4, #160
	subs r3, r3, r0
	lsls r4, r4, #13
	cmp r3, r4
	blt .L_02008a16
.L_020089f2:
	ldr r3, [r5, #8]
	movs r2, #0
	subs r0, r3, r0
	cmp r0, #0
	beq .L_02008a04
	movs r2, #1
	cmp r0, #0
	bgt .L_02008a04
	negs r2, r2
.L_02008a04:
	lsls r1, r2, #1
	adds r1, r1, r2
	movs r3, #128
	lsls r1, r1, #18
	lsls r3, r3, #10
	adds r0, r5, #0
	movs r2, #0
	bl Func_020006ac
.L_02008a16:
	pop {r5, r6, pc}
.L_02008a18:
	.4byte Data_02005af0
.L_02008a1c:
	.4byte 0xfffe0000
	.section .text.x02008a20,"ax",%progbits
	.global Func_02000a20
	.thumb_func
Func_02000a20:
	push {r5, r6, lr}
	adds r6, r0, #0
	bl Func_020056b4
	movs r0, #0
	bl Func_0200580c
	adds r0, r6, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	adds r2, r6, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	movs r3, #192
	lsls r3, r3, #11
	str r3, [r6, #12]
	ldr r3, .L_02008ac4
	ldr r5, .L_02008ac8
	str r3, [r6, #16]
	movs r3, #128
	lsls r3, r3, #7
	strh r3, [r6, #6]
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
	movs r1, #129
	ldr r0, [r5]
	lsls r1, r1, #1
	bl Func_02005794
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
	movs r5, #0
.L_02008a7e:
	ldr r2, .L_02008acc
	ldr r3, [r6, #24]
	movs r0, #1
	adds r3, r3, r2
	str r3, [r6, #24]
	ldr r3, [r6, #28]
	adds r3, r3, r2
	str r3, [r6, #28]
	ldr r2, .L_02008ad0
	ldr r3, [r6, #12]
	adds r3, r3, r2
	str r3, [r6, #12]
	bl WaitFrames
	cmp r5, #10
	bne .L_02008aa4
	movs r0, #204
	bl Func_02005854
.L_02008aa4:
	cmp r5, #20
	bne .L_02008abc
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #93
	str r2, [r3]
	bl Event_ClearStatus1c6
.L_02008abc:
	adds r5, #1
	cmp r5, #19
	bls .L_02008a7e
	pop {r5, r6, pc}
.L_02008ac4:
	.4byte 0x00f741cf
.L_02008ac8:
	.4byte gPartyState
.L_02008acc:
	.4byte 0xfffffd00
.L_02008ad0:
	.4byte 0xfffe8000
	.section .text.x02008ad4,"ax",%progbits
	.global Func_02000ad4
	.thumb_func
Func_02000ad4:
	push {r5, r6, lr}
	ldr r6, .L_02008c28
	ldr r3, [r6]
	cmp r3, #0
	bne .L_02008ae0
	b .L_02008c24
.L_02008ae0:
	bl Func_0200584c
	bl Object_GetById
	ldr r3, [r6]
	movs r1, #180
	lsls r1, r1, #1
	adds r5, r0, #0
	lsls r6, r3, #15
	bl Func_020005b4
	cmp r0, #0
	beq .L_02008b68
	ldr r2, [r0, #8]
	ldr r3, [r5, #8]
	ldr r1, .L_02008c2c
	subs r3, r2, r3
	cmp r3, r1
	bge .L_02008b0e
	movs r1, #128
	lsls r1, r1, #10
	adds r3, r2, r1
	str r3, [r5, #8]
.L_02008b0e:
	ldr r2, [r0, #8]
	ldr r3, [r5, #8]
	movs r1, #128
	subs r3, r2, r3
	lsls r1, r1, #10
	cmp r3, r1
	ble .L_02008b22
	ldr r1, .L_02008c2c
	adds r3, r2, r1
	str r3, [r5, #8]
.L_02008b22:
	ldr r3, [r5, #16]
	adds r3, r3, r6
	str r3, [r5, #16]
	ldr r0, [r0, #16]
	subs r2, r0, r3
	cmp r2, #0
	blt .L_02008b3a
	movs r3, #160
	lsls r3, r3, #13
	cmp r2, r3
	bge .L_02008b44
	b .L_02008b68
.L_02008b3a:
	movs r1, #160
	subs r3, r3, r0
	lsls r1, r1, #13
	cmp r3, r1
	blt .L_02008b68
.L_02008b44:
	ldr r3, [r5, #16]
	movs r1, #0
	subs r0, r3, r0
	cmp r0, #0
	beq .L_02008b56
	movs r1, #1
	cmp r0, #0
	bgt .L_02008b56
	negs r1, r1
.L_02008b56:
	lsls r2, r1, #1
	adds r2, r2, r1
	movs r3, #128
	lsls r2, r2, #18
	lsls r3, r3, #10
	adds r0, r5, #0
	movs r1, #0
	bl Func_020006ac
.L_02008b68:
	movs r1, #106
	adds r1, #255
	adds r0, r5, #0
	bl Func_020005b4
	cmp r0, #0
	beq .L_02008c24
	ldr r2, [r0, #8]
	ldr r3, [r5, #8]
	ldr r1, .L_02008c2c
	subs r3, r2, r3
	cmp r3, r1
	bge .L_02008b8a
	movs r1, #128
	lsls r1, r1, #10
	adds r3, r2, r1
	str r3, [r5, #8]
.L_02008b8a:
	ldr r2, [r0, #8]
	ldr r3, [r5, #8]
	movs r1, #128
	subs r3, r2, r3
	lsls r1, r1, #10
	cmp r3, r1
	ble .L_02008b9e
	ldr r1, .L_02008c2c
	adds r3, r2, r1
	str r3, [r5, #8]
.L_02008b9e:
	ldr r3, [r5, #16]
	subs r3, r3, r6
	str r3, [r5, #16]
	ldr r0, [r0, #16]
	subs r2, r0, r3
	cmp r2, #0
	blt .L_02008bb6
	movs r3, #160
	lsls r3, r3, #13
	cmp r2, r3
	bge .L_02008bc0
	b .L_02008be4
.L_02008bb6:
	movs r1, #160
	subs r3, r3, r0
	lsls r1, r1, #13
	cmp r3, r1
	blt .L_02008be4
.L_02008bc0:
	ldr r3, [r5, #16]
	movs r1, #0
	subs r0, r3, r0
	cmp r0, #0
	beq .L_02008bd2
	movs r1, #1
	cmp r0, #0
	bgt .L_02008bd2
	negs r1, r1
.L_02008bd2:
	lsls r2, r1, #1
	adds r2, r2, r1
	movs r3, #128
	lsls r2, r2, #18
	lsls r3, r3, #10
	adds r0, r5, #0
	movs r1, #0
	bl Func_020006ac
.L_02008be4:
	ldr r3, .L_02008c30
	movs r2, #240
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008c34
	cmp r2, r3
	bne .L_02008c24
	ldr r3, [r5, #8]
	asrs r2, r3, #20
	ldr r3, [r5, #16]
	asrs r3, r3, #20
	cmp r3, #15
	bne .L_02008c24
	cmp r2, #28
	bne .L_02008c14
	adds r0, r5, #0
	bl Func_02000a20
	movs r0, #5
	bl Func_020057c4
	b .L_02008c24
.L_02008c14:
	cmp r2, #30
	bne .L_02008c24
	adds r0, r5, #0
	bl Func_02000a20
	movs r0, #6
	bl Func_020057c4
.L_02008c24:
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02008c28:
	.4byte Data_02005af0
.L_02008c2c:
	.4byte 0xfffe0000
.L_02008c30:
	.4byte gPartyState
.L_02008c34:
	.4byte 0x00000076
	.section .text.x02008c38,"ax",%progbits
	.global Func_02000c38
	.thumb_func
Func_02000c38:
	push {r5, r6, lr}
	ldr r6, .L_02008c88
	ldr r3, [r6]
	cmp r3, #0
	beq .L_02008c86
	bl Func_0200584c
	bl Object_GetById
	movs r1, #104
	adds r1, #255
	adds r5, r0, #0
	bl Func_02000624
	ldr r3, [r6]
	lsls r1, r3, #20
	cmp r0, #0
	beq .L_02008c86
	ldrh r3, [r0, #6]
	movs r2, #128
	lsls r2, r2, #8
	cmp r3, r2
	bne .L_02008c68
	negs r1, r1
.L_02008c68:
	ldrh r3, [r5, #6]
	cmp r3, #0
	bne .L_02008c72
	cmp r1, #0
	bgt .L_02008c7a
.L_02008c72:
	cmp r3, r2
	bne .L_02008c86
	cmp r1, #0
	bge .L_02008c86
.L_02008c7a:
	movs r3, #128
	lsls r3, r3, #11
	adds r0, r5, #0
	movs r2, #0
	bl Func_020006ac
.L_02008c86:
	pop {r5, r6, pc}
.L_02008c88:
	.4byte Data_02005af0
	.section .text.x02008c8c,"ax",%progbits
	.global Func_02000c8c
	.thumb_func
Func_02000c8c:
	push {r5, r6, lr}
	ldr r5, .L_02008cec
	ldr r3, [r5]
	cmp r3, #0
	beq .L_02008ce8
	bl Func_0200584c
	bl Object_GetById
	movs r1, #180
	lsls r1, r1, #1
	adds r6, r0, #0
	bl Func_02000624
	ldr r3, [r5]
	lsls r5, r3, #20
	cmp r0, #0
	bne .L_02008cc0
	movs r1, #106
	adds r1, #255
	adds r0, r6, #0
	bl Func_02000624
	negs r5, r5
	cmp r0, #0
	beq .L_02008ce8
.L_02008cc0:
	ldrh r3, [r6, #6]
	movs r2, #128
	lsls r2, r2, #7
	cmp r3, r2
	bne .L_02008cce
	cmp r5, #0
	bgt .L_02008cda
.L_02008cce:
	movs r2, #192
	lsls r2, r2, #8
	cmp r3, r2
	bne .L_02008ce8
	cmp r5, #0
	bge .L_02008ce8
.L_02008cda:
	movs r3, #128
	lsls r3, r3, #11
	adds r0, r6, #0
	movs r1, #0
	adds r2, r5, #0
	bl Func_020006ac
.L_02008ce8:
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02008cec:
	.4byte Data_02005af0
	.section .text.x02008cf0,"ax",%progbits
	.global Func_02000cf0
	.thumb_func
Func_02000cf0:
	push {r5, r6, lr}
	bl Func_020056b4
	movs r0, #0
	bl Func_0200580c
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #9
	lsls r1, r1, #6
	bl Func_020057a4
	movs r1, #164
	movs r2, #148
	lsls r1, r1, #17
	lsls r2, r2, #17
	movs r0, #39
	bl Func_02005714
	movs r0, #30
	bl Battle_WaitMode0
	ldr r5, .L_02008f4c
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	movs r1, #128
	movs r2, #0
	ldr r0, [r5]
	lsls r1, r1, #6
	bl Func_0200576c
	movs r0, #15
	bl Battle_WaitMode0
	ldr r0, [r5]
	movs r1, #39
	bl Object_LinkObjectAndSetCallback
	movs r1, #2
	movs r2, #30
	ldr r0, [r5]
	adds r1, #255
	bl Func_0200578c
	movs r1, #1
	movs r0, #39
	bl Func_0200579c
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #39
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #32
	orrs r3, r2
	strb r3, [r0]
	movs r1, #1
	movs r0, #39
	bl Object_SetModeById
	movs r1, #160
	movs r0, #39
	lsls r1, r1, #1
	movs r2, #240
	bl ObjectMotion_SetPositionAndReset
	movs r1, #152
	movs r0, #39
	lsls r1, r1, #1
	movs r2, #216
	bl ObjectMotion_SetPositionAndReset
	movs r1, #140
	movs r0, #39
	lsls r1, r1, #1
	movs r2, #216
	bl ObjectMotion_SetPositionAndReset
	movs r1, #140
	lsls r1, r1, #1
	movs r2, #184
	movs r0, #39
	bl ObjectMotion_SetPositionAndReset
	movs r0, #20
	bl Battle_WaitMode0
	ldr r1, [r5]
	movs r2, #0
	movs r0, #39
	bl Func_02005744
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #131
	lsls r0, r0, #1
	bl Func_02005854
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #39
	bl Func_0200578c
	movs r1, #172
	movs r0, #39
	lsls r1, r1, #1
	movs r2, #184
	bl ObjectMotion_SetPositionAndReset
	movs r1, #172
	lsls r1, r1, #1
	movs r2, #200
	movs r0, #39
	bl ObjectMotion_SetPositionAndReset
	movs r0, #2
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #39
	bl Func_0200576c
	movs r0, #15
	bl Battle_WaitMode0
	movs r0, #152
	bl Func_02005854
	movs r0, #39
	bl Object_GetById
	movs r6, #128
	lsls r6, r6, #11
	movs r2, #128
	str r6, [r0, #40]
	adds r1, r6, #0
	movs r0, #39
	lsls r2, r2, #10
	bl ObjectMotion_SetSpeedParameters
	movs r1, #156
	lsls r1, r1, #1
	movs r2, #200
	movs r0, #39
	bl ObjectMotion_SetPositionAndReset
	movs r0, #152
	bl Func_02005854
	movs r0, #39
	bl Object_GetById
	movs r5, #192
	lsls r5, r5, #11
	movs r2, #128
	str r5, [r0, #40]
	adds r1, r6, #0
	movs r0, #39
	lsls r2, r2, #10
	bl ObjectMotion_SetSpeedParameters
	movs r1, #156
	movs r0, #39
	lsls r1, r1, #1
	movs r2, #168
	bl ObjectMotion_SetPositionAndReset
	movs r1, #128
	movs r2, #128
	movs r0, #39
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl ObjectMotion_SetSpeedParameters
	movs r1, #164
	movs r0, #39
	lsls r1, r1, #1
	movs r2, #152
	bl ObjectMotion_SetPositionAndReset
	movs r0, #1
	movs r1, #1
	movs r2, #1
	movs r3, #0
	negs r1, r1
	negs r2, r2
	negs r0, r0
	bl Func_020057ac
	movs r0, #15
	bl Battle_WaitMode0
	movs r0, #152
	bl Func_02005854
	movs r0, #39
	bl Object_GetById
	movs r2, #128
	str r5, [r0, #40]
	adds r1, r6, #0
	movs r0, #39
	lsls r2, r2, #10
	bl ObjectMotion_SetSpeedParameters
	movs r1, #180
	lsls r1, r1, #1
	movs r2, #152
	movs r0, #39
	bl ObjectMotion_SetPositionAndReset
	movs r0, #15
	bl Battle_WaitMode0
	movs r0, #152
	bl Func_02005854
	movs r0, #39
	bl Object_GetById
	movs r1, #128
	movs r2, #128
	str r5, [r0, #40]
	lsls r1, r1, #10
	movs r0, #39
	lsls r2, r2, #9
	bl ObjectMotion_SetSpeedParameters
	movs r1, #180
	movs r0, #39
	lsls r1, r1, #1
	movs r2, #184
	bl ObjectMotion_SetPositionAndReset
	movs r1, #128
	movs r2, #128
	movs r0, #39
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl ObjectMotion_SetSpeedParameters
	movs r1, #188
	movs r0, #39
	lsls r1, r1, #1
	movs r2, #184
	bl ObjectMotion_SetPositionAndReset
	movs r1, #196
	movs r0, #39
	lsls r1, r1, #1
	movs r2, #200
	bl ObjectMotion_SetPositionAndReset
	movs r1, #204
	lsls r1, r1, #1
	movs r2, #184
	movs r0, #39
	bl ObjectMotion_SetPositionAndReset
	movs r0, #15
	bl Battle_WaitMode0
	movs r0, #152
	bl Func_02005854
	movs r0, #39
	bl Object_GetById
	movs r2, #128
	str r5, [r0, #40]
	adds r1, r6, #0
	movs r0, #39
	lsls r2, r2, #10
	bl ObjectMotion_SetSpeedParameters
	movs r1, #220
	movs r0, #39
	lsls r1, r1, #1
	movs r2, #184
	bl ObjectMotion_SetPositionAndReset
	movs r1, #0
	movs r2, #0
	movs r0, #39
	bl Func_02005714
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #108
	bl Func_0200560c
	bl Func_020056bc
	pop {r5, r6, pc}
.L_02008f4c:
	.4byte gPartyState
	.section .text.x02008f50,"ax",%progbits
	.global Func_02000f50
	.thumb_func
Func_02000f50:
	push {r5, lr}
	adds r5, r0, #0
	movs r1, #0
	movs r2, #0
	bl Func_02005714
	adds r0, r5, #0
	bl ObjectMotion_EnableActionAndResetMotion
	adds r0, r5, #0
	movs r1, #1
	bl Object_SetModeById
	pop {r5, pc}
	.section .text.x02008f6c,"ax",%progbits
	.global Func_02000f6c
	.thumb_func
Func_02000f6c:
	push {lr}
	movs r0, #11
	bl Func_02000f50
	movs r0, #12
	bl Func_02000f50
	movs r0, #13
	bl Func_02000f50
	movs r0, #14
	bl Func_02000f50
	movs r0, #11
	movs r1, #11
	movs r2, #9
	movs r3, #0
	bl Func_020022d0
	movs r3, #128
	lsls r3, r3, #8
	movs r0, #12
	movs r1, #11
	movs r2, #9
	bl Func_020022d0
	movs r3, #128
	lsls r3, r3, #7
	movs r0, #13
	movs r1, #10
	movs r2, #10
	bl Func_020022d0
	movs r3, #192
	lsls r3, r3, #8
	movs r0, #14
	movs r1, #10
	movs r2, #10
	bl Func_020022d0
	pop {pc}
	.2byte 0x0000
	.section .text.x02008fc0,"ax",%progbits
	.global Func_02000fc0
	.thumb_func
Func_02000fc0:
	push {r5, lr}
	movs r0, #131
	lsls r0, r0, #1
	adds r0, #255
	bl Func_0200560c
	bl Func_020056b4
	movs r0, #0
	bl Func_0200580c
	movs r0, #78
	bl Func_02005854
	bl Func_0200533c
	movs r0, #40
	bl Battle_WaitMode0
	ldr r0, .L_0200911c
	bl Func_02005754
	movs r0, #24
	movs r1, #0
	bl Func_02005764
	movs r0, #196
	movs r1, #1
	movs r2, #128
	movs r3, #1
	lsls r2, r2, #17
	negs r1, r1
	lsls r0, r0, #17
	bl Func_020057ac
	bl Func_020057b4
	movs r0, #220
	bl Func_02005854
	movs r0, #1
	bl WaitFrames
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #0
	bl Func_020057e4
	movs r0, #254
	lsls r0, r0, #7
	movs r1, #0
	adds r0, #255
	bl Func_020057dc
	movs r0, #40
	bl Func_020057ec
	movs r0, #80
	bl WaitFrames
	ldr r5, .L_02009120
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
	ldr r0, [r5]
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	ldr r0, [r5]
	bl Object_GetById
	movs r1, #15
	bl Func_0200574c
	bl Func_02000f6c
	movs r0, #1
	bl WaitFrames
	movs r0, #128
	movs r1, #0
	lsls r0, r0, #9
	bl Func_020057dc
	movs r0, #10
	bl Func_020057ec
	movs r0, #20
	bl WaitFrames
	movs r0, #130
	bl Func_02005854
	movs r0, #9
	bl Object_GetById
	movs r1, #7
	bl Func_0200574c
	movs r0, #10
	bl Object_GetById
	movs r1, #7
	bl Func_0200574c
	movs r0, #8
	bl WaitFrames
	movs r0, #10
	bl Object_GetById
	movs r1, #3
	bl Func_0200574c
	movs r0, #9
	bl Object_GetById
	movs r1, #0
	bl Func_0200574c
	movs r0, #8
	bl WaitFrames
	movs r0, #11
	bl Func_02005854
	movs r1, #1
	movs r0, #11
	bl Func_02004d48
	adds r5, r0, #0
	bl Func_0200581c
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #85
	str r2, [r3]
	bl Event_ClearStatus1c6
	bl Event_WaitValue1c8Frames
	cmp r5, #1
	beq .L_02009104
	cmp r5, #1
	bgt .L_020090f4
	cmp r5, #0
	beq .L_02009114
	b .L_0200911a
.L_020090f4:
	cmp r5, #2
	beq .L_0200910c
	cmp r5, #3
	bne .L_0200911a
	movs r0, #14
	bl Func_020057c4
	b .L_0200911a
.L_02009104:
	movs r0, #15
	bl Func_020057c4
	b .L_0200911a
.L_0200910c:
	movs r0, #16
	bl Func_020057c4
	b .L_0200911a
.L_02009114:
	movs r0, #17
	bl Func_020057c4
.L_0200911a:
	pop {r5, pc}
.L_0200911c:
	.4byte 0x00001f48
.L_02009120:
	.4byte gPartyState
	.section .text.x02009124,"ax",%progbits
	.global Func_02001124
	.thumb_func
Func_02001124:
	push {lr}
	ldr r3, .L_0200917c
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02009180
	ldr r0, .L_02009184
	cmp r2, r3
	bne .L_0200913e
	ldr r0, .L_02009188
	b .L_02009178
.L_0200913e:
	ldr r3, .L_0200918c
	cmp r2, r3
	bne .L_02009148
	ldr r0, .L_02009190
	b .L_02009178
.L_02009148:
	ldr r3, .L_02009194
	cmp r2, r3
	bne .L_02009152
	ldr r0, .L_02009198
	b .L_02009178
.L_02009152:
	ldr r3, .L_0200919c
	cmp r2, r3
	bne .L_0200915c
	ldr r0, .L_020091a0
	b .L_02009178
.L_0200915c:
	ldr r3, .L_020091a4
	cmp r2, r3
	bne .L_02009166
	ldr r0, .L_020091a8
	b .L_02009178
.L_02009166:
	ldr r3, .L_020091ac
	cmp r2, r3
	bne .L_02009170
	ldr r0, .L_020091b0
	b .L_02009178
.L_02009170:
	ldr r3, .L_020091b4
	cmp r2, r3
	bne .L_02009178
	ldr r0, .L_020091b8
.L_02009178:
	pop {pc}
	.2byte 0x0000
.L_0200917c:
	.4byte gPartyState
.L_02009180:
	.4byte 0x00000073
.L_02009184:
	.4byte Data_02006e9c
.L_02009188:
	.4byte Data_02006b54
.L_0200918c:
	.4byte 0x00000074
.L_02009190:
	.4byte Data_02006b6c
.L_02009194:
	.4byte 0x00000075
.L_02009198:
	.4byte Data_02006bd8
.L_0200919c:
	.4byte 0x00000076
.L_020091a0:
	.4byte Data_02006c80
.L_020091a4:
	.4byte 0x00000077
.L_020091a8:
	.4byte Data_02006d1c
.L_020091ac:
	.4byte 0x00000079
.L_020091b0:
	.4byte Data_02006db8
.L_020091b4:
	.4byte 0x0000007b
.L_020091b8:
	.4byte Data_02006e78
	.section .text.x020091bc,"ax",%progbits
	.global Func_020011bc
	.thumb_func
Func_020011bc:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r9
	push {r5, r6}
	mov r6, r8
	push {r6}
	ldr r5, .L_0200927c
	movs r3, #192
	lsls r3, r3, #18
	movs r2, #133
	ldr r3, [r3, #108]
	lsls r2, r2, #2
	adds r5, r5, r2
	adds r6, r0, #0
	ldr r0, [r5]
	mov r8, r1
	mov r9, r3
	bl Object_GetById
	mov r10, r0
	bl Func_020056b4
	movs r0, #0
	bl Func_0200580c
	mov r2, r10
	movs r3, #0
	adds r2, #85
	strb r3, [r2]
	movs r1, #128
	movs r2, #128
	ldr r0, [r5]
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	ldr r0, [r5]
	adds r1, r6, #0
	mov r2, r8
	bl ObjectMotion_SetPositionAndReset
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #8
	ldr r0, [r5]
	bl Func_0200576c
	ldr r0, [r5]
	bl Object_RefreshSelectorById
	ldr r0, [r5]
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	ldr r0, [r5]
	movs r1, #13
	bl Object_SetModeById
	lsls r6, r6, #16
	mov r3, r8
	lsls r3, r3, #16
	ldr r2, .L_02009280
	adds r1, r6, #0
	mov r0, r10
	mov r8, r3
	bl Func_0200565c
	mov r0, r10
	bl Func_02005664
	movs r1, #10
	ldr r0, [r5]
	bl Object_SetModeById
	movs r0, #123
	bl Func_02005854
	bl Event_ClearStatus1c6
	bl Event_WaitValue1c8Frames
	movs r2, #170
	lsls r2, r2, #1
	add r9, r2
	mov r2, r9
	movs r3, #0
	ldrsh r0, [r2, r3]
	bl Func_020057c4
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, pc}
.L_0200927c:
	.4byte gPartyState
.L_02009280:
	.4byte 0xfff40000
	.section .text.x02009284,"ax",%progbits
	.global Func_02001284
	.thumb_func
Func_02001284:
	push {lr}
	ldr r3, .L_020092e4
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	ldr r3, [r0, #8]
	asrs r1, r3, #20
	ldr r3, [r0, #16]
	asrs r3, r3, #20
	cmp r3, #11
	bne .L_020092c2
	cmp r1, #18
	bne .L_020092b0
	ldr r3, .L_020092e8
	movs r2, #16
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	beq .L_020092e0
.L_020092b0:
	cmp r1, #20
	bne .L_020092d6
	ldr r3, .L_020092e8
	movs r2, #32
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	bne .L_020092d6
	b .L_020092e0
.L_020092c2:
	cmp r1, #19
	bne .L_020092d6
	cmp r3, #12
	bne .L_020092d6
	ldr r3, .L_020092e8
	movs r2, #64
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	beq .L_020092e0
.L_020092d6:
	movs r0, #156
	lsls r0, r0, #1
	movs r1, #180
	bl Func_020011bc
.L_020092e0:
	pop {pc}
	.2byte 0x0000
.L_020092e4:
	.4byte gPartyState
.L_020092e8:
	.4byte gInput
	.section .text.x020092ec,"ax",%progbits
	.global Func_020012ec
	.thumb_func
Func_020012ec:
	push {lr}
	ldr r3, .L_0200935c
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	ldr r3, [r0, #8]
	asrs r4, r3, #20
	ldr r3, [r0, #16]
	asrs r1, r3, #20
	cmp r4, #29
	bne .L_0200932a
	cmp r1, #11
	bne .L_02009318
	ldr r3, .L_02009360
	movs r2, #128
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	beq .L_02009358
.L_02009318:
	cmp r1, #13
	bne .L_0200934e
	ldr r3, .L_02009360
	movs r2, #64
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	bne .L_0200934e
	b .L_02009358
.L_0200932a:
	cmp r1, #12
	bne .L_0200934e
	cmp r4, #28
	bne .L_0200933e
	ldr r3, .L_02009360
	movs r2, #16
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	beq .L_02009358
.L_0200933e:
	cmp r4, #30
	bne .L_0200934e
	ldr r3, .L_02009360
	movs r2, #32
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	beq .L_02009358
.L_0200934e:
	movs r0, #236
	lsls r0, r0, #1
	movs r1, #196
	bl Func_020011bc
.L_02009358:
	pop {pc}
	.2byte 0x0000
.L_0200935c:
	.4byte gPartyState
.L_02009360:
	.4byte gInput
	.section .text.x02009364,"ax",%progbits
	.global Func_02001364
	.thumb_func
Func_02001364:
	push {lr}
	ldr r3, .L_020093b4
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	ldr r3, [r0, #8]
	asrs r2, r3, #20
	ldr r3, [r0, #16]
	asrs r3, r3, #20
	cmp r3, #7
	bne .L_02009392
	cmp r2, #32
	bne .L_020093a6
	ldr r3, .L_020093b8
	movs r2, #16
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	bne .L_020093a6
	b .L_020093b0
.L_02009392:
	cmp r2, #33
	bne .L_020093a6
	cmp r3, #8
	bne .L_020093a6
	ldr r3, .L_020093b8
	movs r2, #64
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	beq .L_020093b0
.L_020093a6:
	movs r0, #134
	lsls r0, r0, #2
	movs r1, #112
	bl Func_020011bc
.L_020093b0:
	pop {pc}
	.2byte 0x0000
.L_020093b4:
	.4byte gPartyState
.L_020093b8:
	.4byte gInput
	.section .text.x020093bc,"ax",%progbits
	.global Func_020013bc
	.thumb_func
Func_020013bc:
	push {lr}
	ldr r3, .L_02009428
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	ldr r3, [r0, #8]
	asrs r4, r3, #20
	ldr r3, [r0, #16]
	asrs r1, r3, #20
	cmp r1, #13
	bne .L_020093fa
	cmp r4, #5
	bne .L_020093e8
	ldr r3, .L_0200942c
	movs r2, #16
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	beq .L_02009426
.L_020093e8:
	cmp r4, #7
	bne .L_0200941e
	ldr r3, .L_0200942c
	movs r2, #32
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	bne .L_0200941e
	b .L_02009426
.L_020093fa:
	cmp r4, #6
	bne .L_0200941e
	cmp r1, #12
	bne .L_0200940e
	ldr r3, .L_0200942c
	movs r2, #128
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	beq .L_02009426
.L_0200940e:
	cmp r1, #14
	bne .L_0200941e
	ldr r3, .L_0200942c
	movs r2, #64
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	beq .L_02009426
.L_0200941e:
	movs r0, #104
	movs r1, #208
	bl Func_020011bc
.L_02009426:
	pop {pc}
.L_02009428:
	.4byte gPartyState
.L_0200942c:
	.4byte gInput
	.section .text.x02009430,"ax",%progbits
	.global Func_02001430
	.thumb_func
Func_02001430:
	push {lr}
	ldr r3, .L_02009490
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	ldr r3, [r0, #8]
	asrs r2, r3, #20
	ldr r3, [r0, #16]
	asrs r1, r3, #20
	cmp r2, #42
	bne .L_0200946e
	cmp r1, #12
	bne .L_0200945c
	ldr r3, .L_02009494
	movs r2, #128
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	beq .L_0200948c
.L_0200945c:
	cmp r1, #14
	bne .L_02009482
	ldr r3, .L_02009494
	movs r2, #64
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	bne .L_02009482
	b .L_0200948c
.L_0200946e:
	cmp r2, #43
	bne .L_02009482
	cmp r1, #13
	bne .L_02009482
	ldr r3, .L_02009494
	movs r2, #16
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	beq .L_0200948c
.L_02009482:
	movs r0, #170
	lsls r0, r0, #2
	movs r1, #208
	bl Func_020011bc
.L_0200948c:
	pop {pc}
	.2byte 0x0000
.L_02009490:
	.4byte gPartyState
.L_02009494:
	.4byte gInput
	.section .text.x02009498,"ax",%progbits
	.global Func_02001498
	.thumb_func
Func_02001498:
	push {lr}
	ldr r3, .L_020094f8
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	ldr r3, [r0, #8]
	asrs r1, r3, #20
	ldr r3, [r0, #16]
	asrs r3, r3, #20
	cmp r3, #7
	bne .L_020094d6
	cmp r1, #23
	bne .L_020094c4
	ldr r3, .L_020094fc
	movs r2, #16
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	beq .L_020094f4
.L_020094c4:
	cmp r1, #25
	bne .L_020094ea
	ldr r3, .L_020094fc
	movs r2, #32
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	bne .L_020094ea
	b .L_020094f4
.L_020094d6:
	cmp r1, #24
	bne .L_020094ea
	cmp r3, #8
	bne .L_020094ea
	ldr r3, .L_020094fc
	movs r2, #64
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	beq .L_020094f4
.L_020094ea:
	movs r0, #196
	lsls r0, r0, #1
	movs r1, #112
	bl Func_020011bc
.L_020094f4:
	pop {pc}
	.2byte 0x0000
.L_020094f8:
	.4byte gPartyState
.L_020094fc:
	.4byte gInput
	.section .text.x02009500,"ax",%progbits
	.global Func_02001500
	.thumb_func
Func_02001500:
	push {lr}
	ldr r3, .L_0200955c
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	ldr r3, [r0, #8]
	asrs r1, r3, #20
	ldr r3, [r0, #16]
	asrs r3, r3, #20
	cmp r3, #9
	bne .L_0200953e
	cmp r1, #5
	bne .L_0200952c
	ldr r3, .L_02009560
	movs r2, #16
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	beq .L_0200955a
.L_0200952c:
	cmp r1, #7
	bne .L_02009552
	ldr r3, .L_02009560
	movs r2, #32
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	bne .L_02009552
	b .L_0200955a
.L_0200953e:
	cmp r1, #6
	bne .L_02009552
	cmp r3, #10
	bne .L_02009552
	ldr r3, .L_02009560
	movs r2, #64
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	beq .L_0200955a
.L_02009552:
	movs r0, #104
	movs r1, #146
	bl Func_020011bc
.L_0200955a:
	pop {pc}
.L_0200955c:
	.4byte gPartyState
.L_02009560:
	.4byte gInput
	.section .text.x02009564,"ax",%progbits
	.global Func_02001564
	.thumb_func
Func_02001564:
	push {lr}
	ldr r3, .L_020095b0
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	ldr r3, [r0, #8]
	asrs r4, r3, #20
	ldr r3, [r0, #16]
	asrs r1, r3, #20
	cmp r4, #42
	bne .L_02009590
	cmp r1, #10
	bne .L_02009590
	ldr r3, .L_020095b4
	movs r2, #64
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	beq .L_020095ae
.L_02009590:
	cmp r4, #43
	bne .L_020095a4
	cmp r1, #9
	bne .L_020095a4
	ldr r3, .L_020095b4
	movs r2, #32
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	beq .L_020095ae
.L_020095a4:
	movs r0, #170
	lsls r0, r0, #2
	movs r1, #146
	bl Func_020011bc
.L_020095ae:
	pop {pc}
.L_020095b0:
	.4byte gPartyState
.L_020095b4:
	.4byte gInput
	.section .text.x020095b8,"ax",%progbits
	.global Func_020015b8
	.thumb_func
Func_020015b8:
	push {lr}
	ldr r3, .L_02009628
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	ldr r3, [r0, #8]
	asrs r4, r3, #20
	ldr r3, [r0, #16]
	asrs r1, r3, #20
	cmp r4, #27
	bne .L_020095f6
	cmp r1, #17
	bne .L_020095e4
	ldr r3, .L_0200962c
	movs r2, #128
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	beq .L_02009626
.L_020095e4:
	cmp r1, #19
	bne .L_0200961a
	ldr r3, .L_0200962c
	movs r2, #64
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	bne .L_0200961a
	b .L_02009626
.L_020095f6:
	cmp r1, #18
	bne .L_0200961a
	cmp r4, #26
	bne .L_0200960a
	ldr r3, .L_0200962c
	movs r2, #16
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	beq .L_02009626
.L_0200960a:
	cmp r4, #28
	bne .L_0200961a
	ldr r3, .L_0200962c
	movs r2, #32
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	beq .L_02009626
.L_0200961a:
	movs r0, #220
	movs r1, #144
	lsls r0, r0, #1
	lsls r1, r1, #1
	bl Func_020011bc
.L_02009626:
	pop {pc}
.L_02009628:
	.4byte gPartyState
.L_0200962c:
	.4byte gInput
	.section .text.x02009630,"ax",%progbits
	.global Func_02001630
	.thumb_func
Func_02001630:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	ldr r3, .L_02009720
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	ldr r3, [r0, #8]
	asrs r4, r3, #20
	ldr r3, [r0, #16]
	asrs r1, r3, #20
	cmp r4, #28
	bne .L_02009660
	cmp r1, #9
	bne .L_02009660
	ldr r3, .L_02009724
	movs r2, #16
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	beq .L_0200971a
.L_02009660:
	cmp r4, #29
	bne .L_02009674
	cmp r1, #10
	bne .L_02009674
	ldr r3, .L_02009724
	movs r2, #64
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	beq .L_0200971a
.L_02009674:
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	ldr r5, .L_02009720
	mov r8, r3
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	ldr r0, [r5]
	bl Object_GetById
	adds r6, r0, #0
	bl Func_020056b4
	movs r0, #0
	bl Func_0200580c
	adds r2, r6, #0
	movs r3, #0
	adds r2, #85
	strb r3, [r2]
	movs r1, #128
	movs r2, #128
	ldr r0, [r5]
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r1, #236
	ldr r0, [r5]
	lsls r1, r1, #1
	movs r2, #144
	bl ObjectMotion_SetPositionAndReset
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #8
	ldr r0, [r5]
	bl Func_0200576c
	ldr r0, [r5]
	bl Object_RefreshSelectorById
	ldr r0, [r5]
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	ldr r0, [r5]
	movs r1, #13
	bl Object_SetModeById
	movs r1, #236
	movs r3, #156
	movs r2, #0
	lsls r3, r3, #16
	lsls r1, r1, #17
	adds r0, r6, #0
	bl Func_0200565c
	adds r0, r6, #0
	bl Func_02005664
	movs r1, #10
	ldr r0, [r5]
	bl Object_SetModeById
	movs r0, #123
	bl Func_02005854
	bl Event_ClearStatus1c6
	bl Event_WaitValue1c8Frames
	movs r2, #170
	lsls r2, r2, #1
	add r8, r2
	mov r2, r8
	movs r3, #0
	ldrsh r0, [r2, r3]
	bl Func_020057c4
.L_0200971a:
	pop {r3}
	mov r8, r3
	pop {r5, r6, pc}
.L_02009720:
	.4byte gPartyState
.L_02009724:
	.4byte gInput
	.section .text.x02009728,"ax",%progbits
	.global Func_02001728
	.thumb_func
Func_02001728:
	push {r5, lr}
	bl Func_020056b4
	movs r0, #0
	bl Func_0200580c
	movs r0, #163
	lsls r0, r0, #1
	adds r0, #255
	bl Func_02005854
	ldr r5, .L_02009764
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	ldr r0, [r5]
	ldr r1, .L_02009768
	ldr r2, .L_02009768
	bl ObjectMotion_SetSpeedParameters
	movs r2, #212
	ldr r0, [r5]
	movs r1, #152
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndCommit
	bl Func_020056bc
	pop {r5, pc}
	.2byte 0x0000
.L_02009764:
	.4byte gPartyState
.L_02009768:
	.4byte 0x00019999
	.section .text.x0200976c,"ax",%progbits
	.global Func_0200176c
	.thumb_func
Func_0200176c:
	push {r5, r6, lr}
	ldr r6, .L_020097d4
	movs r3, #133
	lsls r3, r3, #2
	adds r6, r6, r3
	ldr r0, [r6]
	bl Object_GetById
	adds r5, r0, #0
	bl Func_020056b4
	movs r0, #0
	bl Func_0200580c
	movs r0, #163
	lsls r0, r0, #1
	adds r0, #255
	bl Func_02005854
	adds r5, #85
	movs r3, #0
	strb r3, [r5]
	ldr r1, .L_020097d8
	ldr r0, [r6]
	ldr r2, .L_020097d8
	bl ObjectMotion_SetSpeedParameters
	movs r2, #244
	lsls r2, r2, #1
	movs r1, #152
	ldr r0, [r6]
	bl ObjectMotion_SetPositionAndCommit
	movs r3, #3
	strb r3, [r5]
	movs r0, #8
	bl Battle_WaitMode0
	movs r0, #161
	bl Func_02005854
	ldr r0, [r6]
	movs r1, #22
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	bl Func_020056bc
	pop {r5, r6, pc}
	.2byte 0x0000
.L_020097d4:
	.4byte gPartyState
.L_020097d8:
	.4byte 0x00019999
	.section .text.x020097dc,"ax",%progbits
	.global Func_020017dc
	.thumb_func
Func_020017dc:
	push {r5, lr}
	bl Func_020056b4
	movs r0, #0
	bl Func_0200580c
	movs r0, #163
	lsls r0, r0, #1
	adds r0, #255
	bl Func_02005854
	ldr r5, .L_02009818
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	ldr r0, [r5]
	ldr r1, .L_0200981c
	ldr r2, .L_0200981c
	bl ObjectMotion_SetSpeedParameters
	movs r1, #188
	ldr r0, [r5]
	lsls r1, r1, #1
	movs r2, #200
	bl ObjectMotion_SetPositionAndCommit
	bl Func_020056bc
	pop {r5, pc}
	.2byte 0x0000
.L_02009818:
	.4byte gPartyState
.L_0200981c:
	.4byte 0x00019999
	.section .text.x02009820,"ax",%progbits
	.global Func_02001820
	.thumb_func
Func_02001820:
	push {r5, lr}
	bl Func_020056b4
	movs r0, #0
	bl Func_0200580c
	movs r0, #163
	lsls r0, r0, #1
	adds r0, #255
	bl Func_02005854
	ldr r5, .L_0200985c
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	ldr r0, [r5]
	ldr r1, .L_02009860
	ldr r2, .L_02009860
	bl ObjectMotion_SetSpeedParameters
	movs r1, #188
	ldr r0, [r5]
	lsls r1, r1, #1
	movs r2, #136
	bl ObjectMotion_SetPositionAndCommit
	bl Func_020056bc
	pop {r5, pc}
	.2byte 0x0000
.L_0200985c:
	.4byte gPartyState
.L_02009860:
	.4byte 0x00019999
	.section .text.x02009864,"ax",%progbits
	.global Func_02001864
	.thumb_func
Func_02001864:
	push {r5, lr}
	bl Func_020056b4
	movs r0, #0
	bl Func_0200580c
	movs r0, #163
	lsls r0, r0, #1
	adds r0, #255
	bl Func_02005854
	ldr r5, .L_020098a0
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	ldr r0, [r5]
	ldr r1, .L_020098a4
	ldr r2, .L_020098a4
	bl ObjectMotion_SetSpeedParameters
	movs r1, #220
	ldr r0, [r5]
	lsls r1, r1, #1
	movs r2, #248
	bl ObjectMotion_SetPositionAndCommit
	bl Func_020056bc
	pop {r5, pc}
	.2byte 0x0000
.L_020098a0:
	.4byte gPartyState
.L_020098a4:
	.4byte 0x00019999
	.section .text.x020098a8,"ax",%progbits
	.global Func_020018a8
	.thumb_func
Func_020018a8:
	push {r5, lr}
	bl Func_020056b4
	movs r0, #0
	bl Func_0200580c
	movs r0, #163
	lsls r0, r0, #1
	adds r0, #255
	bl Func_02005854
	ldr r5, .L_020098e4
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	ldr r0, [r5]
	ldr r1, .L_020098e8
	ldr r2, .L_020098e8
	bl ObjectMotion_SetSpeedParameters
	movs r1, #220
	ldr r0, [r5]
	lsls r1, r1, #1
	movs r2, #184
	bl ObjectMotion_SetPositionAndCommit
	bl Func_020056bc
	pop {r5, pc}
	.2byte 0x0000
.L_020098e4:
	.4byte gPartyState
.L_020098e8:
	.4byte 0x00019999
	.section .text.x020098ec,"ax",%progbits
	.global Func_020018ec
	.thumb_func
Func_020018ec:
	push {lr}
	ldr r3, .L_0200991c
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02009920
	cmp r2, r3
	bne .L_0200990a
	movs r1, #8
	movs r2, #1
	bl Func_02001950
	b .L_02009918
.L_0200990a:
	ldr r3, .L_02009924
	cmp r2, r3
	bne .L_02009918
	movs r1, #29
	movs r2, #2
	bl Func_02001950
.L_02009918:
	pop {pc}
	.2byte 0x0000
.L_0200991c:
	.4byte gPartyState
.L_02009920:
	.4byte 0x00000073
.L_02009924:
	.4byte 0x00000074
	.section .text.x02009928,"ax",%progbits
	.global Func_02001928
	.thumb_func
Func_02001928:
	push {lr}
	movs r0, #1
	bl Func_020018ec
	movs r0, #128
	lsls r0, r0, #2
	bl Func_0200560c
	pop {pc}
	.2byte 0x0000
	.section .text.x0200993c,"ax",%progbits
	.global Func_0200193c
	.thumb_func
Func_0200193c:
	push {lr}
	movs r0, #0
	bl Func_020018ec
	movs r0, #128
	lsls r0, r0, #2
	bl Func_02005614
	pop {pc}
	.2byte 0x0000
	.section .text.x02009950,"ax",%progbits
	.global Func_02001950
	.thumb_func
Func_02001950:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r6, r2, #0
	movs r5, #0
	mov r8, r0
	adds r7, r1, #0
	cmp r5, r6
	bcs .L_02009988
.L_02009962:
	adds r0, r7, r5
	bl Object_GetById
	mov r3, r8
	adds r0, #35
	adds r1, r5, #1
	cmp r3, #0
	beq .L_0200997a
	ldrb r2, [r0]
	movs r3, #239
	ands r3, r2
	b .L_02009980
.L_0200997a:
	ldrb r2, [r0]
	movs r3, #16
	orrs r3, r2
.L_02009980:
	strb r3, [r0]
	adds r5, r1, #0
	cmp r5, r6
	bcc .L_02009962
.L_02009988:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x02009990,"ax",%progbits
	.global Func_02001990
	.thumb_func
Func_02001990:
	push {lr}
	adds r3, r0, #0
	adds r2, r1, #0
	ldr r0, .L_020099cc
	adds r1, r3, #0
	bl Func_02005428
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #206
	bl Func_02005604
	cmp r0, #0
	beq .L_020099ca
	movs r0, #250
	lsls r0, r0, #3
	adds r0, #255
	bl Func_02005604
	cmp r0, #0
	beq .L_020099ca
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r1, #181
	lsls r1, r1, #1
	adds r2, r3, r1
	movs r3, #200
	strh r3, [r2]
.L_020099ca:
	pop {pc}
.L_020099cc:
	.4byte Data_02005d7c
	.section .text.x020099d0,"ax",%progbits
	.global Func_020019d0
	.thumb_func
Func_020019d0:
	push {r5, r6, lr}
	movs r0, #26
	bl Object_GetById
	adds r6, r0, #0
	bl Func_020056b4
	movs r0, #0
	bl Func_0200580c
	bl Func_020057bc
	movs r5, #0
	adds r0, #85
	strb r5, [r0]
	movs r1, #152
	movs r0, #153
	lsls r0, r0, #8
	lsls r1, r1, #5
	adds r0, #153
	adds r1, #51
	bl Func_020057a4
	movs r0, #180
	movs r2, #248
	movs r1, #0
	lsls r2, r2, #17
	movs r3, #1
	lsls r0, r0, #17
	bl Func_020057ac
	bl Func_020057b4
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #74
	bl Func_02005854
	movs r0, #0
	bl Func_020020f0
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #70
	bl Func_02005854
	adds r3, r6, #0
	adds r3, #85
	strb r5, [r3]
	movs r3, #128
	lsls r3, r3, #12
	str r3, [r6, #12]
	movs r0, #10
	bl Battle_WaitMode0
	str r5, [r6, #12]
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #1
	negs r0, r0
	bl Func_020020f0
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #205
	bl Func_0200560c
	movs r0, #20
	bl Battle_WaitMode0
	bl Func_020056bc
	pop {r5, r6, pc}
	.2byte 0x0000
	.section .text.x02009a6c,"ax",%progbits
	.global Func_02001a6c
	.thumb_func
Func_02001a6c:
	push {r5, lr}
	adds r5, r0, #0
	bl Func_020043a4
	cmp r5, #1
	bne .L_02009a88
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r1, #181
	lsls r1, r1, #1
	adds r2, r3, r1
	movs r3, #200
	strh r3, [r2]
.L_02009a88:
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x02009a8c,"ax",%progbits
	.global Func_02001a8c
	.thumb_func
Func_02001a8c:
	push {r5, lr}
	movs r0, #15
	bl Object_GetById
	adds r5, r0, #0
	bl Func_020056b4
	movs r0, #0
	bl Func_0200580c
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #214
	bl Func_02005854
	adds r0, r5, #0
	movs r1, #5
	bl Func_02005624
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r5, #28]
	bl Func_020056bc
	pop {r5, pc}
	.section .text.x02009ac0,"ax",%progbits
	.global Func_02001ac0
	.thumb_func
Func_02001ac0:
	push {lr}
	bl Func_020053a0
	cmp r0, #0
	bne .L_02009ada
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r1, #181
	lsls r1, r1, #1
	adds r2, r3, r1
	movs r3, #201
	strh r3, [r2]
.L_02009ada:
	pop {pc}
	.section .text.x02009adc,"ax",%progbits
	.global Func_02001adc
	.thumb_func
Func_02001adc:
	push {r5, lr}
	adds r5, r0, #0
	bl Func_02005784
	adds r0, r5, #0
	bl Object_GetById
	movs r1, #15
	bl Func_0200574c
	adds r0, r5, #0
	movs r1, #1
	bl Func_0200577c
	adds r0, r5, #0
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #2
	orrs r3, r2
	strb r3, [r0]
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x02009b0c,"ax",%progbits
	.global Func_02001b0c
	.thumb_func
Func_02001b0c:
	push {lr}
	movs r0, #10
	adds r0, #255
	bl Func_02005604
	cmp r0, #0
	beq .L_02009b2c
	movs r0, #128
	lsls r0, r0, #2
	bl Func_02005604
	cmp r0, #0
	bne .L_02009b34
	bl Func_0200193c
	b .L_02009b34
.L_02009b2c:
	movs r0, #128
	lsls r0, r0, #2
	bl Func_0200560c
.L_02009b34:
	pop {pc}
	.2byte 0x0000
	.section .text.x02009b38,"ax",%progbits
	.global Func_02001b38
	.thumb_func
Func_02001b38:
	push {r5, r6, r7, lr}
	movs r7, #0
.L_02009b3c:
	adds r5, r7, #0
	adds r5, #8
	adds r0, r5, #0
	bl Object_GetById
	adds r6, r0, #0
	adds r0, r5, #0
	bl Func_02001adc
	adds r2, r6, #0
	movs r3, #0
	adds r2, #85
	strb r3, [r2]
	movs r3, #144
	lsls r3, r3, #14
	adds r7, #1
	str r3, [r6, #12]
	cmp r7, #0
	beq .L_02009b3c
	bl Func_02001b0c
	pop {r5, r6, r7, pc}
	.section .text.x02009b68,"ax",%progbits
	.global Func_02001b68
	.thumb_func
Func_02001b68:
	push {r5, r6, r7, lr}
	movs r7, #0
.L_02009b6c:
	adds r5, r7, #0
	adds r5, #29
	adds r0, r5, #0
	bl Object_GetById
	adds r6, r0, #0
	adds r0, r5, #0
	bl Func_02001adc
	adds r2, r6, #0
	movs r3, #0
	adds r2, #85
	strb r3, [r2]
	movs r3, #144
	lsls r3, r3, #14
	adds r7, #1
	str r3, [r6, #12]
	cmp r7, #1
	bls .L_02009b6c
	bl Func_02001b0c
	pop {r5, r6, r7, pc}
	.section .text.x02009b98,"ax",%progbits
	.global Func_02001b98
	.thumb_func
Func_02001b98:
	push {r5, r6, r7, lr}
	movs r3, #192
	movs r0, #131
	lsls r3, r3, #18
	lsls r0, r0, #1
	movs r6, #16
	movs r7, #0
	ldr r5, [r3, #108]
	bl Func_02005604
	cmp r0, #0
	bne .L_02009bbe
	movs r2, #179
	lsls r2, r2, #1
	adds r3, r5, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	beq .L_02009bc2
.L_02009bbe:
	movs r7, #1
	movs r6, #0
.L_02009bc2:
	movs r5, #8
.L_02009bc4:
	adds r0, r5, #0
	bl Object_GetById
	cmp r0, #0
	beq .L_02009c08
	adds r3, r0, #0
	adds r3, #84
	ldrb r3, [r3]
	cmp r3, #1
	bne .L_02009c08
	ldr r3, [r0, #80]
	ldr r3, [r3, #40]
	cmp r3, #0
	beq .L_02009c08
	movs r2, #0
	ldrsh r3, [r3, r2]
	movs r2, #178
	lsls r2, r2, #1
	cmp r3, r2
	ble .L_02009c08
	adds r2, #6
	cmp r3, r2
	ble .L_02009bfa
	adds r2, #45
	cmp r3, r2
	beq .L_02009c02
	b .L_02009c08
.L_02009bfa:
	adds r1, r6, #0
	bl ObjectDispatch_ApplyValueToChildren
	b .L_02009c08
.L_02009c02:
	adds r3, r0, #0
	adds r3, #91
	strb r7, [r3]
.L_02009c08:
	adds r5, #1
	cmp r5, #63
	ble .L_02009bc4
	pop {r5, r6, r7, pc}
	.section .text.x02009c10,"ax",%progbits
	.global Func_02001c10
	.thumb_func
Func_02001c10:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #32]
	movs r4, #158
	lsls r4, r4, #1
	adds r3, r2, r4
	ldr r3, [r3]
	movs r6, #186
	lsls r6, r6, #1
	mov r9, r3
	adds r3, r2, r6
	ldr r3, [r3]
	subs r4, #4
	mov r10, r3
	adds r3, r2, r4
	ldr r3, [r3]
	subs r6, #4
	sub sp, #4
	mov r8, r3
	adds r3, r2, r6
	movs r2, #0
	ldr r5, [r3]
	ldr r0, .L_0200a000
	ldr r1, .L_0200a004
	str r2, [sp, #0]
.L_02009c50:
	ldmia r1!, {r3}
	stmia r0!, {r3}
	ldr r3, [sp, #0]
	adds r3, #1
	str r3, [sp, #0]
	cmp r3, #11
	bls .L_02009c50
	movs r4, #8
	str r4, [sp, #0]
.L_02009c62:
	ldr r0, [sp, #0]
	bl Object_GetById
	mov r11, r0
	cmp r0, #0
	bne .L_02009c70
	b .L_0200a04a
.L_02009c70:
	mov r3, r11
	adds r3, #84
	ldrb r3, [r3]
	cmp r3, #1
	beq .L_02009c7c
	b .L_0200a04a
.L_02009c7c:
	mov r6, r11
	ldr r3, [r6, #80]
	ldr r0, [r3, #40]
	cmp r0, #0
	bne .L_02009c88
	b .L_0200a04a
.L_02009c88:
	ldr r3, [r6, #8]
	movs r2, #0
	asrs r1, r3, #20
	ldr r3, [r6, #16]
	asrs r4, r3, #20
	movs r6, #0
	ldrsh r3, [r0, r6]
	ldr r0, .L_0200a008
	adds r3, r3, r0
	cmp r3, #5
	bls .L_02009ca0
	b .L_0200a02e
.L_02009ca0:
	ldr r2, .L_0200a00c
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
.L_02009ca8:
	.4byte .L_02009ce8
	.4byte .L_02009cc0
	.4byte .L_02009e02
	.4byte .L_02009f4a
	.4byte .L_02009ea6
	.4byte .L_0200a02c
.L_02009cc0:
	lsls r3, r4, #7
	adds r6, r1, r3
	movs r3, #116
	mov r1, r9
	strb r3, [r1, r6]
	mov r2, r10
	movs r3, #117
	strb r3, [r2, r6]
	lsls r1, r6, #2
	mov r3, r8
	adds r7, r3, r1
	ldr r3, [r7]
	ldr r2, .L_0200a010
.L_02009cda:
	orrs r3, r2
	str r3, [r7]
	adds r7, r5, r1
	ldr r3, [r7]
	orrs r3, r2
	str r3, [r7]
	b .L_0200a02c
.L_02009ce8:
	lsls r3, r4, #7
	adds r1, r1, r3
	mov r12, r1
	mov r6, r12
	subs r6, #129
	lsls r2, r6, #2
	movs r3, #118
	mov r0, r10
	mov r4, r9
	mov r1, r8
	strb r3, [r4, r6]
	adds r7, r1, r2
	strb r3, [r0, r6]
	ldr r3, [r7]
	movs r4, #162
	lsls r4, r4, #15
	orrs r3, r4
	str r3, [r7]
	adds r7, r5, r2
	ldr r3, [r7]
	movs r1, #117
	adds r6, #1
	orrs r3, r4
	mov r2, r9
	str r3, [r7]
	strb r1, [r2, r6]
	mov r3, r8
	lsls r2, r6, #2
	strb r1, [r0, r6]
	adds r7, r3, r2
	ldr r3, [r7]
	ldr r0, .L_0200a014
	adds r6, #1
	orrs r3, r0
	str r3, [r7]
	adds r7, r5, r2
	ldr r3, [r7]
	mov r2, r9
	orrs r3, r0
	str r3, [r7]
	movs r3, #119
	strb r3, [r2, r6]
	mov r2, r10
	strb r3, [r2, r6]
	lsls r2, r6, #2
	mov r3, r8
	adds r7, r3, r2
	ldr r3, [r7]
	adds r6, #126
	orrs r3, r4
	str r3, [r7]
	adds r7, r5, r2
	ldr r3, [r7]
	mov r2, r9
	orrs r3, r4
	str r3, [r7]
	mov r3, r10
	strb r1, [r2, r6]
	strb r1, [r3, r6]
	lsls r2, r6, #2
	mov r6, r8
	adds r7, r6, r2
	ldr r3, [r7]
	mov r6, r12
	orrs r3, r0
	str r3, [r7]
	adds r7, r5, r2
	ldr r3, [r7]
	mov r2, r9
	orrs r3, r0
	str r3, [r7]
	mov r3, r10
	strb r1, [r2, r6]
	strb r1, [r3, r6]
	lsls r2, r6, #2
	mov r3, r8
	adds r7, r3, r2
	ldr r3, [r7]
	adds r6, #1
	orrs r3, r4
	str r3, [r7]
	adds r7, r5, r2
	ldr r3, [r7]
	mov r2, r9
	orrs r3, r4
	str r3, [r7]
	mov r3, r10
	strb r1, [r2, r6]
	strb r1, [r3, r6]
	lsls r2, r6, #2
	mov r6, r8
	adds r7, r6, r2
	ldr r3, [r7]
	mov r6, r12
	orrs r3, r0
	str r3, [r7]
	adds r7, r5, r2
	ldr r3, [r7]
	adds r6, #127
	orrs r3, r0
	str r3, [r7]
	mov r2, r9
	movs r3, #120
	strb r3, [r2, r6]
	mov r2, r10
	strb r3, [r2, r6]
	lsls r2, r6, #2
	mov r3, r8
	adds r7, r3, r2
	ldr r3, [r7]
	adds r6, #1
	orrs r3, r4
	str r3, [r7]
	adds r7, r5, r2
	ldr r3, [r7]
	mov r2, r9
	orrs r3, r4
	str r3, [r7]
	mov r3, r10
	strb r1, [r2, r6]
	strb r1, [r3, r6]
	lsls r2, r6, #2
	mov r6, r8
	adds r7, r6, r2
	ldr r3, [r7]
	mov r6, r12
	orrs r3, r0
	str r3, [r7]
	adds r7, r5, r2
	ldr r3, [r7]
	adds r6, #129
	orrs r3, r0
	str r3, [r7]
	mov r0, r9
	movs r3, #121
	mov r1, r10
	strb r3, [r0, r6]
	lsls r2, r6, #2
	strb r3, [r1, r6]
	mov r3, r8
	b .L_02009fec
.L_02009e02:
	lsls r4, r4, #7
	adds r4, r1, r4
	subs r6, r4, #2
	movs r0, #127
	mov r1, r9
	mov r2, r10
	strb r0, [r1, r6]
	mov r3, r8
	strb r0, [r2, r6]
	lsls r2, r6, #2
	adds r7, r3, r2
	ldr r3, [r7]
	movs r6, #168
	lsls r6, r6, #15
	orrs r3, r6
	str r3, [r7]
	adds r7, r5, r2
	ldr r3, [r7]
	mov lr, r6
	orrs r3, r6
	str r3, [r7]
	subs r6, r4, #1
	movs r3, #122
	strb r3, [r1, r6]
	lsls r2, r6, #2
	mov r12, r0
	mov r1, r8
	mov r0, r10
	strb r3, [r0, r6]
	adds r7, r1, r2
	ldr r3, [r7]
	ldr r0, .L_0200a018
	adds r6, r4, #0
	orrs r3, r0
	str r3, [r7]
	adds r7, r5, r2
	ldr r3, [r7]
	mov r2, r9
	orrs r3, r0
	str r3, [r7]
	mov r1, r10
	movs r3, #117
	strb r3, [r2, r6]
	strb r3, [r1, r6]
	mov r2, r8
	lsls r1, r6, #2
	adds r7, r2, r1
	ldr r3, [r7]
	ldr r2, .L_0200a01c
	adds r6, #1
	orrs r3, r2
	str r3, [r7]
	adds r7, r5, r1
	ldr r3, [r7]
	mov r1, r9
	orrs r3, r2
	str r3, [r7]
	mov r2, r10
	movs r3, #123
	strb r3, [r1, r6]
	strb r3, [r2, r6]
	lsls r2, r6, #2
	mov r3, r8
	adds r7, r3, r2
	ldr r3, [r7]
	adds r6, r4, #2
	orrs r3, r0
	str r3, [r7]
	adds r7, r5, r2
	ldr r3, [r7]
	mov r4, r12
	orrs r3, r0
	str r3, [r7]
	lsls r2, r6, #2
	strb r4, [r1, r6]
	mov r0, r10
	mov r1, r8
	strb r4, [r0, r6]
	adds r7, r1, r2
	ldr r3, [r7]
	mov r4, lr
	b .L_02009ff0
.L_02009ea6:
	lsls r3, r4, #7
	adds r0, r1, r3
	ldr r1, .L_0200a020
	movs r3, #127
	adds r6, r0, r1
	mov r4, r10
	mov r2, r9
	strb r3, [r2, r6]
	lsls r1, r6, #2
	strb r3, [r4, r6]
	mov r6, r8
	adds r7, r6, r1
	ldr r3, [r7]
	movs r2, #168
	lsls r2, r2, #15
	orrs r3, r2
	str r3, [r7]
	adds r7, r5, r1
	ldr r3, [r7]
	adds r6, r0, #0
	orrs r3, r2
	str r3, [r7]
	subs r6, #128
	movs r3, #124
	mov r1, r9
	strb r3, [r1, r6]
	lsls r2, r6, #2
	strb r3, [r4, r6]
	mov r3, r8
	adds r7, r3, r2
	ldr r3, [r7]
	ldr r1, .L_0200a024
	adds r6, r0, #0
	orrs r3, r1
	str r3, [r7]
	adds r7, r5, r2
	ldr r3, [r7]
	mov r4, r9
	orrs r3, r1
	str r3, [r7]
	mov r2, r10
	movs r3, #117
	strb r3, [r4, r6]
	strb r3, [r2, r6]
	lsls r2, r6, #2
	mov r3, r8
	adds r7, r3, r2
	ldr r3, [r7]
	adds r6, #128
	orrs r3, r1
	str r3, [r7]
	adds r7, r5, r2
	ldr r3, [r7]
	lsls r2, r6, #2
	orrs r3, r1
	str r3, [r7]
	movs r3, #125
	strb r3, [r4, r6]
	mov r4, r10
	strb r3, [r4, r6]
	mov r6, r8
	adds r7, r6, r2
	ldr r3, [r7]
	orrs r3, r1
	str r3, [r7]
	adds r7, r5, r2
	ldr r3, [r7]
	mov r2, r9
	orrs r3, r1
	movs r1, #128
	lsls r1, r1, #1
	str r3, [r7]
	adds r6, r0, r1
	movs r3, #126
	strb r3, [r2, r6]
	lsls r1, r6, #2
	strb r3, [r4, r6]
	mov r3, r8
	adds r7, r3, r1
	ldr r3, [r7]
	ldr r2, .L_0200a028
	b .L_02009cda
.L_02009f4a:
	lsls r3, r4, #7
	ldr r6, .L_0200a020
	adds r1, r1, r3
	mov r12, r1
	add r6, r12
	movs r0, #127
	lsls r2, r6, #2
	mov r4, r9
	mov r1, r10
	mov r3, r8
	strb r0, [r4, r6]
	adds r7, r3, r2
	strb r0, [r1, r6]
	ldr r3, [r7]
	movs r4, #168
	lsls r4, r4, #15
	orrs r3, r4
	str r3, [r7]
	adds r7, r5, r2
	ldr r3, [r7]
	mov r6, r12
	orrs r3, r4
	str r3, [r7]
	subs r6, #128
	movs r3, #124
	mov r1, r9
	mov r2, r10
	strb r3, [r1, r6]
	strb r3, [r2, r6]
	lsls r2, r6, #2
	mov r3, r8
	adds r7, r3, r2
	ldr r3, [r7]
	ldr r1, .L_0200a024
	mov r6, r12
	orrs r3, r1
	str r3, [r7]
	adds r7, r5, r2
	ldr r3, [r7]
	mov r2, r9
	orrs r3, r1
	str r3, [r7]
	movs r3, #117
	strb r3, [r2, r6]
	mov r2, r10
	strb r3, [r2, r6]
	lsls r2, r6, #2
	mov r3, r8
	adds r7, r3, r2
	ldr r3, [r7]
	adds r6, #128
	orrs r3, r1
	str r3, [r7]
	adds r7, r5, r2
	ldr r3, [r7]
	mov r2, r9
	orrs r3, r1
	str r3, [r7]
	movs r3, #125
	strb r3, [r2, r6]
	mov r2, r10
	strb r3, [r2, r6]
	lsls r2, r6, #2
	mov r3, r8
	adds r7, r3, r2
	ldr r3, [r7]
	movs r6, #128
	orrs r3, r1
	str r3, [r7]
	adds r7, r5, r2
	ldr r3, [r7]
	lsls r6, r6, #1
	orrs r3, r1
	add r6, r12
	mov r2, r10
	mov r1, r9
	str r3, [r7]
	strb r0, [r1, r6]
	mov r3, r8
	strb r0, [r2, r6]
	lsls r2, r6, #2
.L_02009fec:
	adds r7, r3, r2
	ldr r3, [r7]
.L_02009ff0:
	orrs r3, r4
	str r3, [r7]
	adds r7, r5, r2
	ldr r3, [r7]
	movs r2, #1
	orrs r3, r4
	str r3, [r7]
	b .L_0200a02e
.L_0200a000:
	.4byte Data_0202c001 + 0x1cf
.L_0200a004:
	.4byte Data_02005910
.L_0200a008:
	.4byte 0xfffffe9b
.L_0200a00c:
	.4byte .L_02009ca8
.L_0200a010:
	.4byte 0x80500000
.L_0200a014:
	.4byte 0x80510000
.L_0200a018:
	.4byte 0x00529000
.L_0200a01c:
	.4byte 0x80526000
.L_0200a020:
	.4byte 0xffffff00
.L_0200a024:
	.4byte 0x00539000
.L_0200a028:
	.4byte 0x80550000
.L_0200a02c:
	movs r2, #1
.L_0200a02e:
	cmp r2, #0
	beq .L_0200a04a
	mov r2, r11
	movs r3, #0
	adds r2, #89
	strb r3, [r2]
	subs r2, #4
	strb r3, [r2]
	subs r2, #50
	strb r3, [r2]
	mov r0, r11
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
.L_0200a04a:
	ldr r4, [sp, #0]
	adds r4, #1
	str r4, [sp, #0]
	cmp r4, #63
	bgt .L_0200a056
	b .L_02009c62
.L_0200a056:
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_0200a070
	bl Func_0200557c
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200a070:
	.4byte Func_02001b98
	.section .text.x0200a074,"ax",%progbits
	.global Func_02002074
	.thumb_func
Func_02002074:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r1, #230
	lsls r1, r1, #1
	adds r3, r3, r1
	ldr r3, [r3]
	ldr r1, .L_0200a0ac
	ldr r2, [r3, #16]
	adds r3, r2, r1
	asrs r2, r3, #19
	cmp r2, #0
	bge .L_0200a092
	movs r2, #0
.L_0200a092:
	cmp r2, #16
	ble .L_0200a098
	movs r2, #16
.L_0200a098:
	movs r3, #16
	subs r3, r3, r2
	lsls r3, r3, #8
	adds r3, r3, r2
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	pop {pc}
	.2byte 0x0000
.L_0200a0ac:
	.4byte 0xff180000
	.section .text.x0200a0b0,"ax",%progbits
	.global Func_020020b0
	.thumb_func
Func_020020b0:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r8
	push {r5, r6}
	mov r10, r2
	adds r5, r1, #0
	bl Object_GetById
	adds r6, r0, #0
	ldr r3, [r6, #80]
	ldr r2, .L_0200a0ec
	mov r8, r3
	adds r5, #1
	movs r3, #3
	ands r5, r3
	ldrsb r1, [r2, r5]
	bl Func_02005624
	adds r6, #92
	movs r3, #2
	strb r3, [r6]
	mov r0, r8
	mov r1, r10
	bl Func_0200561c
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200a0ec:
	.4byte Data_02005940
	.section .text.x0200a0f0,"ax",%progbits
	.global Func_020020f0
	.thumb_func
Func_020020f0:
	push {r5, r6, lr}
	ldr r3, .L_0200a2b0
	adds r5, r0, #0
	ldr r6, .L_0200a2b4
	str r5, [r3]
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r6, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_0200a2b8
	sub sp, #8
	cmp r2, r3
	bne .L_0200a172
	movs r0, #14
	adds r1, r5, #0
	movs r2, #0
	bl Func_020020b0
	movs r0, #21
	adds r1, r5, #0
	movs r2, #1
	bl Func_020020b0
	adds r1, r5, #0
	movs r2, #2
	movs r0, #26
	bl Func_020020b0
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r6, r2
	ldrh r3, [r3]
	movs r1, #128
	subs r3, #3
	lsls r3, r3, #16
	lsls r1, r1, #9
	cmp r3, r1
	bls .L_0200a148
	movs r0, #27
	adds r1, r5, #0
	movs r2, #3
	bl Func_020020b0
.L_0200a148:
	cmp r5, #0
	bne .L_0200a15a
	movs r3, #23
	movs r2, #9
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #23
	movs r1, #21
	b .L_0200a284
.L_0200a15a:
	movs r2, #1
	negs r2, r2
	cmp r5, r2
	beq .L_0200a164
	b .L_0200a2aa
.L_0200a164:
	movs r3, #23
	movs r2, #9
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #24
	movs r1, #21
	b .L_0200a284
.L_0200a172:
	ldr r3, .L_0200a2bc
	cmp r2, r3
	bne .L_0200a1a4
	movs r0, #8
	adds r1, r5, #0
	movs r2, #0
	bl Func_020020b0
	movs r0, #15
	adds r1, r5, #0
	movs r2, #1
	bl Func_020020b0
	movs r0, #20
	adds r1, r5, #0
	movs r2, #2
	bl Func_020020b0
	movs r0, #25
	adds r1, r5, #0
	movs r2, #3
	bl Func_020020b0
	movs r0, #29
	b .L_0200a1d4
.L_0200a1a4:
	ldr r3, .L_0200a2c0
	cmp r2, r3
	bne .L_0200a1de
	movs r0, #8
	adds r1, r5, #0
	movs r2, #0
	bl Func_020020b0
	movs r0, #21
	adds r1, r5, #0
	movs r2, #1
	bl Func_020020b0
	movs r0, #29
	adds r1, r5, #0
	movs r2, #2
	bl Func_020020b0
	movs r0, #35
	adds r1, r5, #0
	movs r2, #3
	bl Func_020020b0
	movs r0, #38
.L_0200a1d4:
	adds r1, r5, #0
	movs r2, #4
	bl Func_020020b0
	b .L_0200a2aa
.L_0200a1de:
	ldr r3, .L_0200a2c4
	cmp r2, r3
	bne .L_0200a220
	movs r0, #10
	adds r1, r5, #0
	movs r2, #0
	bl Func_020020b0
	movs r0, #12
	adds r1, r5, #0
	movs r2, #1
	bl Func_020020b0
	cmp r5, #0
	bne .L_0200a20a
	movs r3, #27
	movs r2, #12
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #34
	movs r1, #12
	b .L_0200a284
.L_0200a20a:
	movs r3, #1
	negs r3, r3
	cmp r5, r3
	bne .L_0200a2aa
	movs r3, #27
	movs r2, #12
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #35
	movs r1, #12
	b .L_0200a284
.L_0200a220:
	ldr r3, .L_0200a2c8
	cmp r2, r3
	bne .L_0200a23c
	movs r1, #5
	movs r0, #13
	bl Object_SetModeById
	movs r0, #13
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	b .L_0200a2aa
.L_0200a23c:
	ldr r3, .L_0200a2cc
	cmp r2, r3
	bne .L_0200a2aa
	movs r0, #8
	adds r1, r5, #0
	movs r2, #0
	bl Func_020020b0
	movs r0, #9
	adds r1, r5, #0
	movs r2, #1
	bl Func_020020b0
	movs r0, #10
	adds r1, r5, #0
	movs r2, #2
	bl Func_020020b0
	movs r0, #11
	adds r1, r5, #0
	movs r2, #3
	bl Func_020020b0
	movs r0, #12
	adds r1, r5, #0
	movs r2, #4
	bl Func_020020b0
	cmp r5, #0
	bne .L_0200a28e
	movs r3, #9
	movs r2, #27
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #0
.L_0200a284:
	movs r2, #1
	movs r3, #3
	bl Func_02005674
	b .L_0200a2aa
.L_0200a28e:
	movs r1, #1
	negs r1, r1
	cmp r5, r1
	bne .L_0200a2aa
	movs r3, #9
	movs r2, #27
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #1
	movs r1, #0
	movs r2, #1
	movs r3, #3
	bl Func_02005674
.L_0200a2aa:
	add sp, #8
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200a2b0:
	.4byte Data_02005af0
.L_0200a2b4:
	.4byte gPartyState
.L_0200a2b8:
	.4byte 0x00000074
.L_0200a2bc:
	.4byte 0x00000075
.L_0200a2c0:
	.4byte 0x00000076
.L_0200a2c4:
	.4byte 0x00000077
.L_0200a2c8:
	.4byte 0x00000079
.L_0200a2cc:
	.4byte 0x0000007b
	.section .text.x0200a2d0,"ax",%progbits
	.global Func_020022d0
	.thumb_func
Func_020022d0:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	adds r5, r2, #0
	adds r6, r3, #0
	mov r10, r1
	mov r8, r0
	bl Object_GetById
	adds r7, r0, #0
	adds r0, r5, #0
	bl Object_GetById
	movs r1, #0
	adds r5, r0, #0
	adds r0, r7, #0
	bl Func_02005624
	ldr r1, .L_0200a338
	adds r0, r7, #0
	bl Func_02005634
	adds r0, r7, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	adds r2, r7, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	str r5, [r7, #104]
	cmp r6, #0
	bge .L_0200a316
	adds r6, #255
.L_0200a316:
	adds r2, r7, #0
	asrs r3, r6, #8
	adds r2, #98
	strb r3, [r2]
	adds r3, r7, #0
	adds r3, #99
	mov r2, r8
	strb r2, [r3]
	adds r0, r7, #0
	mov r1, r10
	bl Animation_ApplyChildValues
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200a338:
	.4byte Data_0200e88c
	.section .text.x0200a33c,"ax",%progbits
	.global Func_0200233c
	.thumb_func
Func_0200233c:
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
	bge .L_0200a36c
	adds r3, #15
.L_0200a36c:
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
	.section .text.x0200a394,"ax",%progbits
	.global Func_02002394
	.thumb_func
Func_02002394:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, .L_0200a528
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	sub sp, #68
	bl Object_GetById
	adds r7, r0, #0
	bl Func_020056b4
	movs r0, #0
	bl Func_0200580c
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r1, r1
	negs r2, r2
	movs r3, #0
	negs r0, r0
	bl Func_020057ac
	bl Func_0200564c
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
	bl Func_02005854
	movs r3, #3
	strb r3, [r5]
	movs r0, #24
	bl Battle_WaitMode0
	add r2, sp, #28
	movs r3, #7
	str r3, [r2, #4]
	ldr r3, .L_0200a52c
	mov r8, r2
	str r3, [r2, #36]
	movs r3, #204
	lsls r3, r3, #8
	adds r3, #204
	str r3, [r2, #8]
	str r3, [r2, #12]
	movs r3, #0
	mov r10, r3
.L_0200a42e:
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
	ldr r3, .L_0200a530
	adds r2, r2, r3
	str r2, [r6]
	bl Random16Far
	lsls r3, r0, #1
	ldr r5, [r6, #8]
	adds r3, r3, r0
	ldr r4, .L_0200a534
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
	ldr r4, .L_0200a538
	str r5, [sp, #4]
	str r4, [sp, #8]
	mov r4, r8
	str r4, [sp, #12]
	bl Func_020000b8
	movs r2, #1
	add r10, r2
	mov r3, r10
	cmp r3, #16
	bls .L_0200a42e
	movs r0, #188
	bl Func_02005854
	ldr r5, .L_0200a528
	movs r4, #133
	lsls r4, r4, #2
	adds r5, r5, r4
	movs r1, #2
	ldr r0, [r5]
	adds r1, #255
	bl Func_02005794
	ldr r0, [r5]
	movs r1, #49
	bl Object_SetModeById
	movs r0, #160
	movs r1, #160
	movs r2, #128
	lsls r0, r0, #11
	lsls r1, r1, #11
	lsls r2, r2, #9
	bl Func_02005694
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	adds r2, #102
	negs r0, r0
	negs r1, r1
	bl Func_02005694
	bl Func_0200569c
	movs r1, #128
	ldr r0, [r5]
	lsls r1, r1, #1
	bl Func_02005794
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r7, #72]
	movs r3, #128
	lsls r3, r3, #7
	str r3, [r7, #68]
	movs r0, #10
	bl WaitFrames
	ldr r0, [r5]
	movs r1, #1
	bl Object_SetModeById
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #93
	str r2, [r3]
	bl Func_020056bc
	add sp, #68
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_0200a528:
	.4byte gPartyState
.L_0200a52c:
	.4byte Func_0200233c
.L_0200a530:
	.4byte 0xffffa000
.L_0200a534:
	.4byte 0xffffd000
.L_0200a538:
	.4byte 0x01090001
	.section .text.x0200a53c,"ax",%progbits
	.global Func_0200253c
	.thumb_func
Func_0200253c:
	push {r5, r6, lr}
	bl Func_020056b4
	movs r0, #0
	bl Func_0200580c
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r2, r2
	negs r1, r1
	movs r3, #0
	negs r0, r0
	bl Func_020057ac
	bl Func_020057bc
	movs r3, #0
	adds r0, #85
	strb r3, [r0]
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	movs r0, #40
	bl Battle_WaitMode0
	ldr r0, .L_0200a7a8
	bl Func_02005754
	movs r0, #24
	movs r1, #0
	bl Func_02005764
	ldr r6, .L_0200a7ac
	movs r3, #133
	lsls r3, r3, #2
	adds r6, r6, r3
	ldr r1, [r6]
	movs r0, #23
	bl Func_02005724
	ldr r1, [r6]
	movs r0, #5
	bl Func_02005724
	ldr r1, [r6]
	movs r0, #6
	bl Func_02005724
	ldr r1, [r6]
	movs r0, #7
	bl Func_02005724
	movs r0, #1
	bl WaitFrames
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	ldr r0, [r6]
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
	adds r2, #102
	movs r0, #23
	adds r1, #204
	bl ObjectMotion_SetSpeedParameters
	ldr r1, .L_0200a7b0
	movs r0, #23
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #10
	bl WaitFrames
	ldr r0, [r6]
	ldr r1, .L_0200a7b4
	bl ObjectMotion_EnableActionAndSetCallback
	ldr r1, .L_0200a7b8
	movs r0, #5
	bl ObjectMotion_EnableActionAndSetCallback
	ldr r1, .L_0200a7bc
	movs r0, #7
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #23
	bl Object_RefreshSelectorById
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #152
	lsls r1, r1, #6
	ldr r0, .L_0200a7c0
	adds r1, #102
	bl Func_020057a4
	movs r0, #217
	movs r1, #1
	movs r2, #143
	movs r3, #1
	lsls r0, r0, #17
	negs r1, r1
	lsls r2, r2, #17
	bl Func_020057ac
	movs r2, #153
	lsls r2, r2, #8
	adds r2, #153
	movs r0, #23
	ldr r1, .L_0200a7c0
	bl ObjectMotion_SetSpeedParameters
	ldr r1, .L_0200a7c4
	movs r0, #23
	bl Object_SetActionCallbackAndRefreshById
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #208
	movs r0, #23
	lsls r1, r1, #8
	bl Func_02005774
	movs r0, #23
	movs r1, #0
	bl Func_02005764
	movs r1, #153
	lsls r1, r1, #8
	ldr r0, .L_0200a7c8
	adds r1, #153
	bl Func_020057a4
	movs r0, #217
	movs r1, #1
	movs r2, #168
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #16
	lsls r0, r0, #17
	bl Func_020057ac
	bl Func_020057b4
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #128
	movs r0, #6
	lsls r1, r1, #6
	movs r2, #0
	bl Func_0200576c
	movs r1, #160
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl Func_0200576c
	movs r1, #160
	ldr r0, [r6]
	lsls r1, r1, #8
	movs r2, #0
	bl Func_0200576c
	movs r1, #128
	lsls r1, r1, #6
	movs r2, #40
	movs r0, #7
	bl Func_0200576c
	movs r0, #7
	bl Object_GetById
	movs r5, #128
	lsls r5, r5, #7
	strh r5, [r0, #6]
	ldr r0, [r6]
	bl Object_GetById
	strh r5, [r0, #6]
	movs r0, #6
	bl Object_GetById
	strh r5, [r0, #6]
	movs r0, #5
	bl Object_GetById
	strh r5, [r0, #6]
	movs r0, #7
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r5, #254
	adds r3, r5, #0
	ands r3, r2
	strb r3, [r0]
	ldr r0, [r6]
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	adds r3, r5, #0
	ands r3, r2
	strb r3, [r0]
	movs r0, #6
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	adds r3, r5, #0
	ands r3, r2
	strb r3, [r0]
	movs r0, #5
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	movs r1, #200
	ands r5, r3
	movs r2, #136
	strb r5, [r0]
	lsls r1, r1, #1
	movs r0, #7
	lsls r2, r2, #1
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r1, #210
	movs r2, #136
	ldr r0, [r6]
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r1, #222
	movs r2, #136
	movs r0, #6
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r1, #234
	movs r2, #136
	lsls r2, r2, #1
	lsls r1, r1, #1
	movs r0, #5
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #0
	bl Func_020057e4
	movs r1, #0
	movs r0, #0
	bl Func_020057dc
	movs r0, #80
	bl Func_020057ec
	movs r0, #80
	bl WaitFrames
	movs r0, #11
	bl Func_020057c4
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200a7a8:
	.4byte 0x00001f25
.L_0200a7ac:
	.4byte gPartyState
.L_0200a7b0:
	.4byte Data_02005ba4
.L_0200a7b4:
	.4byte Data_02005af4
.L_0200a7b8:
	.4byte Data_02005b1c
.L_0200a7bc:
	.4byte Data_02005b60
.L_0200a7c0:
	.4byte 0x00013333
.L_0200a7c4:
	.4byte Data_02005be8
.L_0200a7c8:
	.4byte 0x0004cccc
	.section .text.x0200a7cc,"ax",%progbits
	.global Func_020027cc
	.thumb_func
Func_020027cc:
	push {r5, r6, lr}
	bl Func_020056b4
	movs r0, #0
	bl Func_0200580c
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r1, r1
	negs r2, r2
	movs r3, #0
	negs r0, r0
	bl Func_020057ac
	movs r0, #78
	bl Func_02005854
	ldr r6, .L_0200aa58
	movs r2, #133
	lsls r2, r2, #2
	movs r5, #128
	adds r6, r6, r2
	lsls r5, r5, #7
	movs r1, #174
	movs r2, #156
	ldr r0, [r6]
	adds r3, r5, #0
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_0200571c
	movs r1, #165
	movs r2, #156
	adds r3, r5, #0
	movs r0, #7
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_0200571c
	movs r3, #192
	movs r1, #216
	movs r2, #156
	lsls r3, r3, #6
	movs r0, #23
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_0200571c
	movs r1, #224
	movs r2, #156
	adds r3, r5, #0
	movs r0, #6
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_0200571c
	movs r1, #234
	movs r2, #156
	lsls r1, r1, #17
	adds r3, r5, #0
	lsls r2, r2, #17
	movs r0, #5
	bl Func_0200571c
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
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #220
	bl Func_02005854
	movs r0, #1
	bl WaitFrames
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #0
	bl Func_020057e4
	movs r0, #254
	lsls r0, r0, #7
	movs r1, #0
	adds r0, #255
	bl Func_020057dc
	movs r0, #40
	bl Func_020057ec
	movs r0, #80
	bl WaitFrames
	bl Func_02000f6c
	movs r0, #1
	bl WaitFrames
	movs r0, #128
	movs r1, #0
	lsls r0, r0, #9
	bl Func_020057dc
	movs r0, #10
	bl Func_020057ec
	movs r0, #20
	bl WaitFrames
	movs r1, #128
	ldr r0, [r6]
	lsls r1, r1, #1
	movs r2, #0
	bl Func_0200578c
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #5
	bl Func_0200578c
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #6
	bl Func_0200578c
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #7
	bl Func_0200578c
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #23
	bl Func_0200578c
	movs r1, #192
	ldr r0, [r6]
	lsls r1, r1, #8
	movs r2, #0
	bl Func_0200576c
	movs r1, #192
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl Func_0200576c
	movs r1, #192
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #0
	bl Func_0200576c
	movs r1, #192
	movs r0, #7
	lsls r1, r1, #8
	movs r2, #0
	bl Func_0200576c
	movs r1, #176
	movs r2, #60
	lsls r1, r1, #8
	movs r0, #23
	bl Func_0200576c
	movs r0, #130
	bl Func_02005854
	movs r0, #9
	bl Object_GetById
	movs r1, #7
	bl Func_0200574c
	movs r0, #10
	bl Object_GetById
	movs r1, #7
	bl Func_0200574c
	movs r0, #8
	bl WaitFrames
	movs r0, #10
	bl Object_GetById
	movs r1, #3
	bl Func_0200574c
	movs r0, #9
	bl Object_GetById
	movs r1, #0
	bl Func_0200574c
	movs r0, #8
	bl WaitFrames
	ldr r0, [r6]
	bl Object_GetById
	adds r5, r0, #0
	bl Random16Far
	lsls r3, r0, #2
	adds r3, r3, r0
	lsls r3, r3, #3
	lsrs r3, r3, #16
	adds r3, #10
	adds r5, #98
	strb r3, [r5]
	movs r0, #7
	bl Object_GetById
	adds r5, r0, #0
	bl Random16Far
	lsls r3, r0, #2
	adds r3, r3, r0
	lsls r3, r3, #3
	lsrs r3, r3, #16
	adds r3, #10
	adds r5, #98
	strb r3, [r5]
	movs r0, #5
	bl Object_GetById
	adds r5, r0, #0
	bl Random16Far
	lsls r3, r0, #2
	adds r3, r3, r0
	lsls r3, r3, #3
	lsrs r3, r3, #16
	adds r3, #10
	adds r5, #98
	strb r3, [r5]
	movs r0, #6
	bl Object_GetById
	adds r5, r0, #0
	bl Random16Far
	lsls r3, r0, #2
	adds r3, r3, r0
	lsls r3, r3, #3
	lsrs r3, r3, #16
	adds r3, #10
	adds r5, #98
	strb r3, [r5]
	movs r0, #23
	bl Object_GetById
	adds r5, r0, #0
	bl Random16Far
	lsls r3, r0, #2
	adds r3, r3, r0
	lsls r3, r3, #3
	lsrs r3, r3, #16
	adds r3, #10
	adds r5, #98
	strb r3, [r5]
	ldr r5, .L_0200aa5c
	ldr r0, [r6]
	adds r1, r5, #0
	bl ObjectMotion_EnableActionAndSetCallback
	adds r1, r5, #0
	movs r0, #7
	bl ObjectMotion_EnableActionAndSetCallback
	ldr r5, .L_0200aa60
	movs r0, #5
	adds r1, r5, #0
	bl ObjectMotion_EnableActionAndSetCallback
	adds r1, r5, #0
	movs r0, #6
	bl ObjectMotion_EnableActionAndSetCallback
	adds r1, r5, #0
	movs r0, #23
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #11
	bl Func_02005854
	movs r1, #0
	movs r0, #11
	bl Func_02004d48
	movs r0, #80
	bl Battle_WaitMode0
	movs r0, #255
	lsls r0, r0, #3
	adds r0, #255
	bl Func_0200560c
	bl Event_ClearStatus1c6
	bl Event_WaitValue1c8Frames
	movs r0, #12
	bl Func_020057c4
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200aa58:
	.4byte gPartyState
.L_0200aa5c:
	.4byte Data_02005c74
.L_0200aa60:
	.4byte Data_02005ca0
	.section .text.x0200aa64,"ax",%progbits
	.global Func_02002a64
	.thumb_func
Func_02002a64:
	push {r5, r6, lr}
	bl Func_020056b4
	movs r0, #0
	bl Func_0200580c
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r0, r0
	negs r1, r1
	negs r2, r2
	movs r3, #0
	bl Func_020057ac
	ldr r3, .L_0200ac1c
	movs r2, #133
	lsls r2, r2, #2
	movs r5, #192
	adds r6, r3, r2
	lsls r5, r5, #8
	movs r1, #174
	movs r2, #156
	adds r3, r5, #0
	ldr r0, [r6]
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_0200571c
	movs r1, #165
	movs r2, #156
	adds r3, r5, #0
	movs r0, #7
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_0200571c
	movs r3, #176
	movs r1, #216
	movs r2, #156
	lsls r3, r3, #8
	movs r0, #23
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_0200571c
	movs r1, #224
	movs r2, #156
	adds r3, r5, #0
	movs r0, #6
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_0200571c
	movs r1, #234
	movs r2, #156
	lsls r1, r1, #17
	adds r3, r5, #0
	lsls r2, r2, #17
	movs r0, #5
	movs r5, #192
	bl Func_0200571c
	lsls r5, r5, #18
	movs r0, #1
	bl WaitFrames
	ldr r3, [r5, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #85
	str r2, [r3]
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #128
	movs r2, #20
	lsls r1, r1, #1
	movs r0, #23
	bl Func_0200578c
	ldr r0, .L_0200ac20
	bl Func_02005754
	movs r1, #0
	movs r0, #23
	bl Func_02005764
	bl Func_020057bc
	movs r3, #0
	adds r0, #85
	strb r3, [r0]
	movs r1, #200
	movs r0, #204
	lsls r0, r0, #8
	lsls r1, r1, #5
	adds r0, #204
	adds r1, #153
	bl Func_020057a4
	movs r0, #196
	movs r1, #1
	movs r2, #146
	movs r3, #1
	lsls r0, r0, #17
	negs r1, r1
	lsls r2, r2, #17
	bl Func_020057ac
	bl Func_020057b4
	ldr r0, [r6]
	movs r1, #0
	movs r2, #0
	bl Func_0200576c
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl Func_0200576c
	movs r2, #0
	movs r0, #7
	movs r1, #0
	bl Func_0200576c
	movs r1, #128
	movs r0, #6
	lsls r1, r1, #8
	bl Func_02005774
	movs r0, #6
	movs r1, #3
	bl Object_SetModeById
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #6
	movs r1, #0
	bl Func_02005764
	movs r0, #192
	lsls r0, r0, #7
	movs r1, #0
	adds r0, #5
	bl Func_0200575c
	ldr r0, [r6]
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_0200ac24
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #20
	movs r0, #7
	bl Func_0200578c
	movs r1, #128
	movs r2, #128
	movs r0, #7
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl ObjectMotion_SetSpeedParameters
	movs r1, #170
	movs r2, #148
	movs r0, #7
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #177
	movs r2, #148
	movs r0, #7
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #182
	movs r2, #154
	movs r0, #7
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #224
	movs r2, #0
	ldr r0, [r6]
	lsls r1, r1, #8
	bl Func_0200576c
	movs r1, #192
	movs r0, #7
	lsls r1, r1, #7
	bl Func_02005774
	movs r0, #192
	lsls r0, r0, #7
	adds r0, #7
	movs r1, #0
	bl Func_02005764
	ldr r2, [r5, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_0200ac94
	.2byte 0x0000
.L_0200ac1c:
	.4byte gPartyState
.L_0200ac20:
	.4byte 0x00001f2e
.L_0200ac24:
	ldr r3, [r5, #108]
	movs r2, #226
	lsls r2, r2, #1
	adds r3, r3, r2
	ldrh r2, [r3]
	movs r1, #128
	adds r2, #1
	strh r2, [r3]
	movs r2, #128
	movs r0, #7
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl ObjectMotion_SetSpeedParameters
	movs r1, #170
	movs r2, #148
	movs r0, #7
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #177
	movs r2, #148
	movs r0, #7
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #182
	movs r2, #154
	movs r0, #7
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #224
	movs r2, #0
	ldr r0, [r6]
	lsls r1, r1, #8
	bl Func_0200576c
	movs r1, #192
	movs r0, #7
	lsls r1, r1, #7
	bl Func_02005774
	movs r0, #7
	movs r1, #3
	bl Object_SetModeById
	movs r0, #192
	lsls r0, r0, #7
	adds r0, #7
	movs r1, #0
	bl Func_02005764
.L_0200ac94:
	movs r1, #128
	movs r0, #23
	lsls r1, r1, #8
	bl Func_02005774
	movs r0, #23
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #192
	lsls r0, r0, #7
	adds r0, #23
	movs r1, #0
	bl Func_02005764
	movs r0, #7
	movs r1, #0
	movs r2, #0
	bl Func_0200576c
	movs r1, #2
	movs r2, #40
	adds r1, #255
	movs r0, #7
	bl Func_0200578c
	movs r0, #192
	lsls r0, r0, #7
	adds r0, #23
	movs r1, #0
	bl Func_02005764
	movs r0, #7
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r1, #10
	movs r2, #20
	adds r1, #255
	movs r0, #23
	bl Func_0200578c
	movs r0, #192
	lsls r0, r0, #7
	adds r0, #23
	movs r1, #0
	bl Func_02005764
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #7
	movs r1, #0
	bl Func_02005764
	movs r1, #128
	movs r2, #40
	lsls r1, r1, #1
	movs r0, #7
	bl Func_0200578c
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #7
	movs r1, #0
	bl Func_02005764
	movs r0, #23
	movs r1, #3
	bl Object_SetModeById
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #23
	movs r1, #0
	bl Func_02005764
	movs r1, #10
	movs r2, #10
	adds r1, #255
	movs r0, #7
	bl Func_0200578c
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #7
	movs r1, #0
	bl Func_02005764
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #6
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #220
	movs r2, #148
	lsls r2, r2, #1
	movs r0, #6
	lsls r1, r1, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #128
	movs r0, #6
	lsls r1, r1, #8
	bl Func_02005774
	movs r0, #6
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #6
	movs r1, #0
	bl Func_02005764
	movs r1, #224
	movs r0, #7
	lsls r1, r1, #8
	movs r2, #0
	bl Func_0200576c
	movs r1, #6
	adds r1, #255
	movs r2, #0
	movs r0, #7
	bl Func_0200578c
	movs r1, #128
	movs r0, #6
	lsls r1, r1, #6
	movs r2, #0
	bl Func_0200576c
	movs r1, #160
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl Func_0200576c
	ldr r5, .L_0200af60
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	ldr r0, [r5]
	movs r1, #0
	movs r2, #80
	bl Func_0200576c
	movs r1, #6
	movs r2, #40
	adds r1, #255
	movs r0, #7
	bl Func_0200578c
	movs r0, #7
	movs r1, #0
	bl Func_02005774
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #7
	movs r1, #0
	bl Func_02005764
	movs r1, #128
	movs r2, #0
	movs r0, #5
	lsls r1, r1, #8
	bl Func_0200576c
	movs r1, #128
	movs r0, #6
	lsls r1, r1, #8
	bl Func_02005774
	ldr r0, [r5]
	movs r1, #3
	bl Object_SetModeById
	movs r0, #23
	movs r1, #3
	bl Object_SetModeById
	movs r0, #5
	movs r1, #3
	bl Object_SetModeById
	movs r0, #6
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #7
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r1, #176
	movs r0, #23
	lsls r1, r1, #8
	bl Func_02005774
	movs r0, #23
	movs r1, #0
	bl Func_02005764
	movs r1, #2
	adds r1, #255
	movs r2, #0
	movs r0, #23
	bl Func_0200578c
	movs r1, #208
	movs r2, #20
	movs r0, #23
	lsls r1, r1, #8
	bl Func_0200576c
	movs r1, #176
	movs r0, #23
	lsls r1, r1, #8
	bl Func_02005774
	movs r0, #23
	movs r1, #0
	bl Func_02005764
	movs r1, #153
	lsls r1, r1, #8
	ldr r0, .L_0200af64
	adds r1, #153
	bl Func_020057a4
	movs r0, #196
	movs r1, #1
	movs r2, #176
	lsls r2, r2, #16
	movs r3, #1
	lsls r0, r0, #17
	negs r1, r1
	bl Func_020057ac
	bl Func_020057b4
	movs r0, #23
	movs r1, #0
	bl Func_02005764
	movs r0, #6
	movs r1, #0
	bl Func_02005764
	movs r0, #23
	movs r1, #0
	bl Func_02005764
	movs r0, #7
	movs r1, #0
	bl Func_02005764
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #5
	movs r1, #0
	bl Func_02005764
	movs r0, #196
	movs r1, #1
	movs r2, #146
	movs r3, #1
	lsls r2, r2, #17
	lsls r0, r0, #17
	negs r1, r1
	bl Func_020057ac
	bl Func_020057b4
	movs r0, #23
	movs r1, #3
	bl Object_SetModeById
	movs r0, #23
	movs r1, #0
	bl Func_02005764
	movs r1, #128
	movs r0, #23
	lsls r1, r1, #8
	bl Func_02005774
	movs r0, #23
	movs r1, #0
	bl Func_02005764
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #5
	ldr r1, .L_0200af68
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #6
	ldr r1, .L_0200af68
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #23
	ldr r1, .L_0200af68
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	adds r2, #102
	movs r0, #7
	adds r1, #204
	bl ObjectMotion_SetSpeedParameters
	ldr r5, .L_0200af6c
	movs r0, #7
	adds r1, r5, #0
	bl ObjectMotion_EnableActionAndSetCallback
	adds r1, r5, #0
	movs r0, #23
	bl ObjectMotion_EnableActionAndSetCallback
	adds r1, r5, #0
	movs r0, #6
	bl ObjectMotion_EnableActionAndSetCallback
	adds r1, r5, #0
	movs r0, #5
	bl Object_SetActionCallbackAndRefreshById
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #246
	bl Func_0200560c
	bl Event_ClearStatus1c6
	bl Event_WaitValue1c8Frames
	bl Func_0200581c
	movs r0, #13
	bl Func_020057c4
	pop {r5, r6, pc}
.L_0200af60:
	.4byte gPartyState
.L_0200af64:
	.4byte 0x0004cccc
.L_0200af68:
	.4byte 0x00019999
.L_0200af6c:
	.4byte Data_02005c40
	.section .text.x0200af70,"ax",%progbits
	.global Func_02002f70
	.thumb_func
Func_02002f70:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r9
	push {r5, r6}
	mov r6, r8
	push {r6}
	sub sp, #8
	bl Func_02003604
	adds r5, r0, #0
	bl Func_020056b4
	movs r0, #0
	bl Func_0200580c
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r0, r0
	negs r1, r1
	negs r2, r2
	movs r3, #0
	bl Func_020057ac
	ldr r3, .L_0200b320
	lsls r5, r5, #2
	ldr r2, [r3, r5]
	movs r1, #11
	movs r3, #16
	movs r0, #19
	bl Func_020048a4
	movs r0, #15
	movs r1, #5
	bl Object_SetModeById
	movs r0, #11
	movs r1, #0
	movs r2, #0
	bl Func_02005714
	movs r0, #12
	movs r1, #0
	movs r2, #0
	bl Func_02005714
	movs r0, #13
	movs r1, #0
	movs r2, #0
	bl Func_02005714
	movs r0, #14
	movs r1, #0
	movs r2, #0
	bl Func_02005714
	movs r0, #16
	movs r1, #0
	movs r2, #0
	bl Func_02005714
	movs r0, #17
	movs r1, #0
	movs r2, #0
	bl Func_02005714
	movs r0, #18
	movs r1, #0
	movs r2, #0
	bl Func_02005714
	movs r0, #19
	movs r1, #0
	movs r2, #0
	bl Func_02005714
	movs r0, #20
	movs r1, #0
	movs r2, #0
	bl Func_02005714
	movs r0, #21
	movs r1, #0
	movs r2, #0
	bl Func_02005714
	movs r1, #0
	movs r2, #0
	movs r0, #22
	bl Func_02005714
	movs r0, #1
	bl WaitFrames
	movs r0, #220
	movs r1, #1
	movs r2, #182
	lsls r0, r0, #17
	negs r1, r1
	lsls r2, r2, #16
	movs r3, #0
	bl Func_020057ac
	ldr r2, .L_0200b324
	movs r3, #133
	lsls r3, r3, #2
	mov r8, r2
	add r8, r3
	mov r2, r8
	ldr r0, [r2]
	movs r1, #0
	movs r2, #0
	bl Func_02005714
	movs r3, #128
	movs r1, #236
	movs r2, #216
	lsls r3, r3, #7
	movs r0, #5
	lsls r1, r1, #17
	lsls r2, r2, #16
	mov r10, r3
	bl Func_0200571c
	movs r2, #192
	lsls r2, r2, #6
	mov r9, r2
	movs r1, #236
	movs r2, #200
	lsls r1, r1, #17
	mov r3, r9
	lsls r2, r2, #16
	movs r0, #24
	bl Func_0200571c
	movs r0, #1
	bl WaitFrames
	bl Func_0200564c
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
	movs r1, #152
	lsls r1, r1, #6
	ldr r0, .L_0200b328
	adds r1, #102
	bl Func_020057a4
	movs r0, #220
	movs r1, #1
	movs r2, #156
	movs r3, #1
	lsls r0, r0, #17
	negs r1, r1
	lsls r2, r2, #17
	bl Func_020057ac
	movs r1, #128
	movs r2, #128
	movs r0, #5
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #8
	movs r0, #24
	lsls r1, r1, #9
	bl ObjectMotion_SetSpeedParameters
	ldr r1, .L_0200b32c
	movs r0, #5
	bl ObjectMotion_EnableActionAndSetCallback
	ldr r1, .L_0200b330
	movs r0, #24
	bl Object_SetActionCallbackAndRefreshById
	ldr r0, .L_0200b334
	bl Func_02005754
	movs r0, #5
	movs r1, #0
	bl Func_02005764
	movs r1, #176
	movs r0, #24
	lsls r1, r1, #8
	movs r2, #0
	bl Func_0200576c
	movs r1, #220
	movs r2, #148
	lsls r2, r2, #1
	movs r0, #5
	lsls r1, r1, #1
	bl ObjectMotion_SetPositionAndReset
	movs r0, #5
	movs r1, #0
	bl Func_02005774
	movs r0, #5
	movs r1, #0
	bl Func_02005764
	movs r1, #160
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl Func_0200576c
	movs r0, #207
	movs r1, #1
	movs r2, #136
	lsls r2, r2, #17
	movs r3, #1
	lsls r0, r0, #17
	negs r1, r1
	bl Func_020057ac
	bl Func_020057b4
	movs r0, #5
	movs r1, #0
	bl Func_02005764
	add r0, sp, #4
	mov r1, sp
	bl Func_02004f40
	ldr r1, [sp, #4]
	movs r3, #156
	lsls r3, r3, #17
	lsls r1, r1, #20
	adds r1, r1, r3
	ldr r3, [sp, #0]
	movs r2, #184
	lsls r2, r2, #16
	lsls r3, r3, #20
	movs r0, #217
	adds r3, r3, r2
	lsls r0, r0, #1
	movs r2, #0
	str r1, [sp, #4]
	str r3, [sp, #0]
	bl Func_0200563c
	adds r6, r0, #0
	adds r1, r6, #0
	adds r1, #85
	ldrb r2, [r1]
	movs r3, #4
	orrs r3, r2
	strb r3, [r1]
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r1, #1
	adds r0, r6, #0
	bl Func_02005624
	movs r0, #1
	bl WaitFrames
	movs r0, #5
	bl Object_GetById
	adds r5, r0, #0
	ldr r3, [r5, #16]
	ldr r0, [r6, #16]
	ldr r1, [r6, #8]
	subs r0, r0, r3
	ldr r3, [r5, #8]
	subs r1, r1, r3
	bl Func_02005594
	strh r0, [r5, #6]
	movs r1, #1
	ldr r2, [r6, #16]
	movs r3, #1
	ldr r0, [r6, #8]
	negs r1, r1
	bl Func_020057ac
	bl Func_020057b4
	movs r1, #0
	movs r0, #5
	bl Func_02005764
	adds r0, r6, #0
	bl Func_02005644
	movs r0, #220
	movs r1, #1
	movs r2, #152
	lsls r2, r2, #17
	movs r3, #1
	lsls r0, r0, #17
	negs r1, r1
	bl Func_020057ac
	bl Func_020057b4
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #6
	bl Func_02005774
	movs r0, #5
	movs r1, #0
	bl Func_02005764
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #24
	bl Func_02005794
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #24
	movs r1, #0
	bl Func_02005764
	mov r3, r8
	movs r1, #236
	movs r2, #204
	ldr r0, [r3]
	lsls r1, r1, #17
	mov r3, r10
	lsls r2, r2, #16
	bl Func_0200571c
	movs r1, #220
	movs r2, #204
	mov r3, r10
	movs r0, #7
	lsls r1, r1, #17
	lsls r2, r2, #16
	bl Func_0200571c
	movs r1, #228
	movs r2, #204
	mov r3, r9
	movs r0, #23
	lsls r1, r1, #17
	lsls r2, r2, #16
	bl Func_0200571c
	movs r1, #244
	movs r2, #204
	mov r3, r10
	lsls r2, r2, #16
	lsls r1, r1, #17
	movs r0, #6
	bl Func_0200571c
	movs r0, #1
	bl WaitFrames
	movs r0, #23
	movs r1, #0
	bl Func_02005764
	movs r1, #208
	movs r0, #24
	lsls r1, r1, #8
	movs r2, #0
	bl Func_0200576c
	movs r0, #220
	movs r1, #1
	movs r2, #132
	movs r3, #1
	lsls r0, r0, #17
	negs r1, r1
	lsls r2, r2, #17
	bl Func_020057ac
	bl Func_020057b4
	movs r2, #10
	movs r0, #24
	movs r1, #4
	bl ObjectMotion_Launch
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #24
	movs r1, #0
	bl Func_02005764
	movs r0, #7
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #7
	movs r1, #0
	bl Func_02005764
	movs r1, #2
	movs r2, #40
	adds r1, #255
	movs r0, #24
	bl Func_0200578c
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #6
	movs r1, #0
	bl Func_02005764
	movs r0, #23
	movs r1, #4
	bl Object_SetModeById
	movs r0, #144
	lsls r0, r0, #8
	movs r1, #0
	adds r0, #23
	bl Func_02005764
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #0
	bl Func_020057e4
	movs r1, #0
	movs r0, #0
	bl Func_020057dc
	movs r0, #60
	bl Func_020057ec
	movs r0, #10
	adds r0, #255
	bl Func_02005614
	movs r0, #60
	bl WaitFrames
	movs r0, #18
	bl Func_020057c4
	add sp, #8
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200b320:
	.4byte Data_02005d8c
.L_0200b324:
	.4byte gPartyState
.L_0200b328:
	.4byte 0x00013333
.L_0200b32c:
	.4byte Data_02005ccc
.L_0200b330:
	.4byte Data_02005d24
.L_0200b334:
	.4byte 0x00001f74
	.section .text.x0200b338,"ax",%progbits
	.global Func_02003338
	.thumb_func
Func_02003338:
	push {lr}
	bl Func_0200533c
	ldr r3, .L_0200b390
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r1, [r3]
	movs r0, #23
	bl Func_02005724
	movs r0, #23
	bl Object_GetById
	movs r1, #15
	bl Func_0200574c
	movs r0, #10
	bl WaitFrames
	ldr r0, .L_0200b394
	bl Func_02005754
	movs r0, #23
	movs r1, #0
	bl Func_02005764
	movs r1, #0
	movs r2, #0
	movs r0, #23
	bl Func_02005714
	movs r0, #1
	bl WaitFrames
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #236
	bl Func_0200560c
	bl Func_02005364
	pop {pc}
	.2byte 0x0000
.L_0200b390:
	.4byte gPartyState
.L_0200b394:
	.4byte 0x00001fe0
	.section .text.x0200b398,"ax",%progbits
	.global Func_02003398
	.thumb_func
Func_02003398:
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
	ldr r3, .L_0200b478
	subs r2, #41
	adds r3, r3, r2
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_0200b47c
	sub sp, #8
	cmp r2, r3
	beq .L_0200b3f8
	movs r0, #130
	lsls r0, r0, #1
	adds r0, #255
	bl Func_02005604
	cmp r0, #0
	beq .L_0200b3d6
	movs r0, #0
	bl Func_020020f0
	b .L_0200b3f4
.L_0200b3d6:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #205
	bl Func_02005604
	cmp r0, #0
	beq .L_0200b3ee
	movs r0, #1
	negs r0, r0
	bl Func_020020f0
	b .L_0200b3f4
.L_0200b3ee:
	movs r0, #1
	bl Func_020020f0
.L_0200b3f4:
	bl Func_02001c10
.L_0200b3f8:
	bl Func_0200584c
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #32
	orrs r3, r2
	strb r3, [r0]
	ldr r3, .L_0200b478
	movs r2, #240
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_0200b47c
	cmp r2, r3
	bne .L_0200b422
	bl Func_020034ec
	b .L_0200b470
.L_0200b422:
	ldr r3, .L_0200b480
	cmp r2, r3
	bne .L_0200b442
	movs r3, #15
	movs r2, #4
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #15
	movs r1, #68
	movs r2, #8
	movs r3, #3
	bl Func_02005674
	bl Func_020034f4
	b .L_0200b470
.L_0200b442:
	ldr r3, .L_0200b484
	cmp r2, r3
	bne .L_0200b44e
	bl Func_0200359c
	b .L_0200b470
.L_0200b44e:
	ldr r3, .L_0200b488
	cmp r2, r3
	bne .L_0200b45a
	bl Func_020035f0
	b .L_0200b470
.L_0200b45a:
	ldr r3, .L_0200b48c
	cmp r2, r3
	bne .L_0200b466
	bl Func_02003634
	b .L_0200b470
.L_0200b466:
	ldr r3, .L_0200b490
	cmp r2, r3
	bne .L_0200b470
	bl Func_02003868
.L_0200b470:
	movs r0, #0
	add sp, #8
	pop {pc}
	.2byte 0x0000
.L_0200b478:
	.4byte gPartyState
.L_0200b47c:
	.4byte 0x00000073
.L_0200b480:
	.4byte 0x00000074
.L_0200b484:
	.4byte 0x00000075
.L_0200b488:
	.4byte 0x00000077
.L_0200b48c:
	.4byte 0x00000079
.L_0200b490:
	.4byte 0x0000007b
	.section .text.x0200b494,"ax",%progbits
	.global Func_02003494
	.thumb_func
Func_02003494:
	push {lr}
	ldr r1, .L_0200b4e4
	movs r0, #240
	lsls r0, r0, #1
	adds r3, r1, r0
	movs r0, #0
	ldrsh r2, [r3, r0]
	ldr r3, .L_0200b4e8
	cmp r2, r3
	bne .L_0200b4e0
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r1, r2
	movs r0, #0
	ldrsh r3, [r3, r0]
	cmp r3, #1
	bne .L_0200b4e0
	movs r0, #10
	adds r0, #255
	bl Func_02005604
	cmp r0, #0
	bne .L_0200b4e0
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #205
	bl Func_02005614
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #206
	bl Func_02005614
	movs r0, #250
	lsls r0, r0, #3
	adds r0, #255
	bl Func_02005614
.L_0200b4e0:
	movs r0, #0
	pop {pc}
.L_0200b4e4:
	.4byte gPartyState
.L_0200b4e8:
	.4byte 0x0000007b
	.section .text.x0200b4ec,"ax",%progbits
	.global Func_020034ec
	.thumb_func
Func_020034ec:
	push {lr}
	bl Func_02001b38
	pop {pc}
	.section .text.x0200b4f4,"ax",%progbits
	.global Func_020034f4
	.thumb_func
Func_020034f4:
	push {r5, lr}
	ldr r3, .L_0200b590
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	ldrh r3, [r3]
	movs r2, #128
	subs r3, #3
	lsls r3, r3, #16
	lsls r2, r2, #9
	sub sp, #8
	cmp r3, r2
	bhi .L_0200b534
	ldr r5, .L_0200b594
	movs r0, #27
	adds r1, r5, #0
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #28
	adds r1, r5, #0
	bl ObjectMotion_EnableActionAndSetCallback
	movs r3, #27
	movs r2, #29
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #33
	movs r1, #24
	movs r2, #3
	movs r3, #3
	bl Func_02005674
.L_0200b534:
	ldr r0, .L_0200b598
	bl Func_020053bc
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #206
	bl Func_02005604
	cmp r0, #0
	beq .L_0200b556
	movs r0, #250
	lsls r0, r0, #3
	adds r0, #255
	bl Func_02005604
	cmp r0, #0
	bne .L_0200b562
.L_0200b556:
	movs r0, #26
	bl Object_GetById
	movs r3, #128
	lsls r3, r3, #13
	str r3, [r0, #12]
.L_0200b562:
	bl Func_02005824
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #0
	adds r1, #255
	movs r2, #8
	movs r3, #9
	bl Func_0200583c
	movs r1, #128
	lsls r1, r1, #2
	movs r0, #1
	adds r1, #2
	movs r2, #10
	movs r3, #11
	bl Func_0200583c
	bl Func_02001b68
	add sp, #8
	pop {r5, pc}
	.2byte 0x0000
.L_0200b590:
	.4byte gPartyState
.L_0200b594:
	.4byte Data_02005944
.L_0200b598:
	.4byte Data_02005d7c
	.section .text.x0200b59c,"ax",%progbits
	.global Func_0200359c
	.thumb_func
Func_0200359c:
	push {r5, lr}
	bl Func_02005824
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #0
	adds r1, #255
	movs r2, #31
	movs r3, #32
	bl Func_0200583c
	movs r0, #10
	adds r0, #255
	bl Func_02005604
	cmp r0, #0
	bne .L_0200b5e8
	ldr r3, .L_0200b5ec
	movs r2, #241
	lsls r2, r2, #1
	adds r5, r3, r2
	ldrh r3, [r5]
	movs r2, #128
	subs r3, #7
	lsls r3, r3, #16
	lsls r2, r2, #9
	cmp r3, r2
	bhi .L_0200b5d8
	bl Func_02002394
.L_0200b5d8:
	movs r2, #0
	ldrsh r3, [r5, r2]
	cmp r3, #7
	bne .L_0200b5e8
	movs r0, #129
	lsls r0, r0, #2
	bl Func_0200560c
.L_0200b5e8:
	pop {r5, pc}
	.2byte 0x0000
.L_0200b5ec:
	.4byte gPartyState
	.section .text.x0200b5f0,"ax",%progbits
	.global Func_020035f0
	.thumb_func
Func_020035f0:
	push {lr}
	bl Func_02005824
	movs r1, #8
	movs r2, #9
	movs r0, #0
	bl Func_0200582c
	pop {pc}
	.2byte 0x0000
	.section .text.x0200b604,"ax",%progbits
	.global Func_02003604
	.thumb_func
Func_02003604:
	push {lr}
	ldr r3, .L_0200b630
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Owner_GetState
	adds r3, r0, #0
	adds r2, r3, #0
	adds r1, r2, #0
	movs r0, #0
	adds r1, #13
.L_0200b61e:
	ldrb r3, [r2]
	adds r2, #1
	adds r0, r0, r3
	cmp r2, r1
	bls .L_0200b61e
	movs r3, #15
	ands r0, r3
	pop {pc}
	.2byte 0x0000
.L_0200b630:
	.4byte gPartyState
	.section .text.x0200b634,"ax",%progbits
	.global Func_02003634
	.thumb_func
Func_02003634:
	push {r5, r6, lr}
	movs r0, #10
	bl Object_GetById
	movs r1, #3
	bl Func_0200574c
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_0200b858
	bl Func_0200557c
	movs r0, #11
	bl Object_GetById
	movs r1, #11
	bl Func_0200574c
	movs r0, #12
	bl Object_GetById
	movs r1, #11
	bl Func_0200574c
	movs r0, #13
	bl Object_GetById
	movs r1, #10
	bl Func_0200574c
	movs r0, #14
	bl Object_GetById
	movs r1, #10
	bl Func_0200574c
	movs r0, #16
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #17
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #18
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #19
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #20
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #21
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #22
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #251
	bl Func_02005604
	cmp r0, #0
	beq .L_0200b6f2
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #253
	bl Func_02005604
	cmp r0, #0
	bne .L_0200b6f2
	bl Func_02002f70
	b .L_0200b856
.L_0200b6f2:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #252
	bl Func_02005604
	cmp r0, #0
	beq .L_0200b762
	ldr r3, .L_0200b85c
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	movs r1, #0
	adds r5, r0, #0
	movs r2, #0
	movs r0, #11
	bl Func_02005714
	movs r0, #12
	movs r1, #0
	movs r2, #0
	bl Func_02005714
	movs r0, #13
	movs r1, #0
	movs r2, #0
	bl Func_02005714
	movs r1, #0
	movs r2, #0
	movs r0, #14
	bl Func_02005714
	bl Func_020053ac
	bl Func_020056b4
	movs r0, #0
	bl Func_0200580c
	movs r1, #1
	ldr r0, [r5, #8]
	ldr r2, [r5, #16]
	negs r1, r1
	movs r3, #0
	bl Func_020057ac
	bl Func_0200564c
	movs r0, #1
	bl WaitFrames
	bl Func_020056bc
.L_0200b762:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #246
	bl Func_02005604
	cmp r0, #0
	beq .L_0200b78e
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #253
	bl Func_02005604
	cmp r0, #0
	bne .L_0200b78e
	movs r0, #0
	bl Func_020052a4
	movs r1, #144
	ldr r0, .L_0200b860
	lsls r1, r1, #3
	bl Func_0200557c
.L_0200b78e:
	bl Func_02003604
	ldr r3, .L_0200b864
	lsls r0, r0, #2
	ldr r2, [r3, r0]
	movs r1, #11
	movs r3, #16
	movs r0, #19
	bl Func_020048a4
	movs r0, #15
	movs r1, #5
	bl Object_SetModeById
	ldr r3, .L_0200b85c
	movs r2, #241
	lsls r2, r2, #1
	adds r5, r3, r2
	movs r2, #0
	ldrsh r3, [r5, r2]
	cmp r3, #2
	bne .L_0200b7c0
	bl Func_020027cc
	b .L_0200b856
.L_0200b7c0:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #246
	bl Func_02005604
	cmp r0, #0
	bne .L_0200b7e4
	movs r2, #0
	ldrsh r3, [r5, r2]
	cmp r3, #1
	bne .L_0200b7dc
	bl Func_0200253c
	b .L_0200b7e4
.L_0200b7dc:
	cmp r3, #3
	bne .L_0200b7e4
	bl Func_02002a64
.L_0200b7e4:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #252
	bl Func_02005604
	cmp r0, #0
	beq .L_0200b856
	movs r5, #192
	bl Func_020056b4
	lsls r5, r5, #18
	movs r0, #0
	bl Func_0200580c
	ldr r2, [r5, #108]
	movs r3, #129
	movs r6, #214
	lsls r3, r3, #1
	adds r3, #255
	lsls r6, r6, #1
	str r3, [r2, r6]
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	ldr r2, [r5, #108]
	movs r3, #133
	lsls r3, r3, #1
	movs r0, #128
	adds r3, #255
	lsls r0, r0, #4
	adds r0, #252
	str r3, [r2, r6]
	bl Func_02005614
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #235
	bl Func_02005604
	cmp r0, #0
	beq .L_0200b84a
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #236
	bl Func_02005604
	cmp r0, #0
	bne .L_0200b84a
	bl Func_02003338
.L_0200b84a:
	movs r0, #48
	adds r0, #255
	bl Func_02005614
	bl Func_020056bc
.L_0200b856:
	pop {r5, r6, pc}
.L_0200b858:
	.4byte Func_02002074
.L_0200b85c:
	.4byte gPartyState
.L_0200b860:
	.4byte Func_02001ac0
.L_0200b864:
	.4byte Data_02005d8c
	.section .text.x0200b868,"ax",%progbits
	.global Func_02003868
	.thumb_func
Func_02003868:
	push {r5, lr}
	ldr r5, .L_0200b8a8
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r5, r2
	ldr r0, [r3]
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #32
	orrs r3, r2
	movs r2, #241
	lsls r2, r2, #1
	strb r3, [r0]
	adds r3, r5, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #1
	bne .L_0200b8a4
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #39
	bl Func_0200560c
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #40
	bl Func_02005614
.L_0200b8a4:
	pop {r5, pc}
	.2byte 0x0000
.L_0200b8a8:
	.4byte gPartyState
	.section .text.x0200b8ac,"ax",%progbits
	.global Func_020038ac
	.thumb_func
Func_020038ac:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	movs r0, #192
	lsls r0, r0, #4
	ldr r6, .L_0200b8ec
	bl Runtime_BumpAllocate
	adds r5, r0, #0
	adds r1, r5, #0
	ldr r0, .L_0200b8f0
	bl Func_020055cc
	bl Resource_FindFreeEntry
	movs r1, #192
	lsls r1, r1, #4
	adds r2, r5, #0
	mov r8, r0
	bl VramBlock_LoadCached
	movs r3, #180
	lsls r3, r3, #1
	adds r6, r6, r3
	mov r3, r8
	strh r3, [r6]
	adds r0, r5, #0
	bl Sys_Free
	pop {r3}
	mov r8, r3
	pop {r5, r6, pc}
.L_0200b8ec:
	.4byte gSceneState
.L_0200b8f0:
	.4byte Data_02005948
	.section .text.x0200b8f4,"ax",%progbits
	.global Func_020038f4
	.thumb_func
Func_020038f4:
	push {lr}
	ldr r2, .L_0200b930
	cmp r0, #10
	bhi .L_0200b900
	cmp r1, #7
	bls .L_0200b904
.L_0200b900:
	movs r0, #0
	b .L_0200b92e
.L_0200b904:
	movs r3, #11
	muls r3, r1
	adds r3, r3, r0
	lsls r3, r3, #2
	adds r0, r3, r2
	ldrb r3, [r0, #3]
	cmp r3, #0
	bne .L_0200b91a
	ldrb r3, [r0, #1]
	cmp r3, #0
	beq .L_0200b91e
.L_0200b91a:
	movs r0, #1
	b .L_0200b92e
.L_0200b91e:
	ldrb r2, [r0]
	movs r3, #10
	eors r2, r3
	negs r3, r2
	orrs r3, r2
	lsrs r3, r3, #31
	movs r0, #1
	subs r0, r0, r3
.L_0200b92e:
	pop {pc}
.L_0200b930:
	.4byte gSceneState
	.section .text.x0200b934,"ax",%progbits
	.global Func_02003934
	.thumb_func
Func_02003934:
	push {r5, r6, lr}
	adds r6, r1, #0
	subs r1, r6, #1
	adds r5, r0, #0
	bl Func_020038f4
	cmp r0, #0
	bne .L_0200b95c
	adds r1, r6, #1
	adds r0, r5, #0
	bl Func_020038f4
	cmp r0, #0
	bne .L_0200b95c
	subs r0, r5, #1
	adds r1, r6, #0
	bl Func_020038f4
	cmp r0, #0
	beq .L_0200b960
.L_0200b95c:
	movs r0, #1
	b .L_0200b970
.L_0200b960:
	adds r0, r5, #1
	adds r1, r6, #0
	bl Func_020038f4
	adds r3, r0, #0
	negs r0, r3
	orrs r0, r3
	lsrs r0, r0, #31
.L_0200b970:
	pop {r5, r6, pc}
	.2byte 0x0000
	.section .text.x0200b974,"ax",%progbits
	.global Func_02003974
	.thumb_func
Func_02003974:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r7, .L_0200ba60
	movs r0, #0
	sub sp, #8
	mov r10, r0
.L_0200b984:
	movs r1, #0
	mov r8, r1
.L_0200b988:
	movs r3, #11
	mov r2, r10
	muls r2, r3
	adds r3, r2, #0
	add r3, r8
	movs r0, #128
	lsls r3, r3, #2
	lsls r0, r0, #4
	adds r5, r7, r3
	adds r0, #251
	ldrb r6, [r5, #3]
	bl Func_02005604
	cmp r0, #0
	beq .L_0200b9b2
	ldrb r3, [r5, #3]
	cmp r3, #2
	bne .L_0200b9b2
	movs r3, #1
	strb r3, [r5, #3]
	movs r6, #1
.L_0200b9b2:
	cmp r6, #2
	bne .L_0200b9d6
	movs r0, #182
	lsls r0, r0, #1
	adds r3, r7, r0
	adds r0, #2
	movs r1, #0
	ldrsh r2, [r3, r1]
	adds r3, r7, r0
	movs r1, #0
	ldrsh r3, [r3, r1]
	add r2, r8
	add r3, r10
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r0, #0
	movs r1, #11
	b .L_0200ba06
.L_0200b9d6:
	cmp r6, #1
	bne .L_0200ba10
	movs r3, #11
	mov r2, r10
	muls r2, r3
	adds r3, r2, #0
	add r3, r8
	movs r0, #182
	lsls r0, r0, #1
	lsls r3, r3, #2
	ldrb r1, [r7, r3]
	adds r3, r7, r0
	movs r0, #0
	ldrsh r2, [r3, r0]
	movs r0, #183
	lsls r0, r0, #1
	adds r3, r7, r0
	movs r0, #0
	ldrsh r3, [r3, r0]
	add r2, r8
	add r3, r10
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r0, #1
.L_0200ba06:
	movs r2, #1
	movs r3, #1
	bl Func_0200566c
	b .L_0200ba44
.L_0200ba10:
	movs r3, #11
	mov r1, r10
	muls r1, r3
	adds r3, r1, #0
	add r3, r8
	movs r2, #182
	lsls r3, r3, #2
	lsls r2, r2, #1
	ldrb r1, [r7, r3]
	adds r3, r7, r2
	movs r0, #0
	ldrsh r2, [r3, r0]
	movs r0, #183
	lsls r0, r0, #1
	adds r3, r7, r0
	movs r0, #0
	ldrsh r3, [r3, r0]
	add r2, r8
	add r3, r10
	str r2, [sp, #0]
	str r3, [sp, #4]
	adds r0, r6, #0
	movs r2, #1
	movs r3, #1
	bl Func_0200566c
.L_0200ba44:
	movs r1, #1
	add r8, r1
	mov r2, r8
	cmp r2, #10
	bls .L_0200b988
	add r10, r1
	mov r3, r10
	cmp r3, #7
	bls .L_0200b984
	add sp, #8
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_0200ba60:
	.4byte gSceneState
	.section .text.x0200ba64,"ax",%progbits
	.global Func_02003a64
	.thumb_func
Func_02003a64:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r1, .L_0200bdec
	movs r3, #176
	mov r11, r1
	lsls r3, r3, #1
	add r3, r11
	subs r0, #1
	str r0, [r3]
	sub sp, #24
.L_0200ba82:
	movs r1, #176
	lsls r1, r1, #1
	mov r3, r11
	adds r2, r3, r1
	ldr r3, [r2]
	mov r0, r11
	adds r3, #1
	str r3, [r2]
	ldr r2, .L_0200bdf0
	movs r7, #0
	str r3, [r2]
	ldr r3, .L_0200bdf4
	mov lr, r3
	.2byte 0xf800
	mov r3, r11
	movs r2, #10
	adds r3, #152
	strb r2, [r3]
	movs r3, #156
	lsls r3, r3, #1
	add r3, r11
	strb r2, [r3]
	movs r3, #172
	lsls r3, r3, #1
	add r3, r11
	strb r2, [r3]
	movs r2, #54
	adds r2, #255
	movs r3, #1
	add r2, r11
	strb r3, [r2]
	movs r2, #62
	adds r2, #255
	add r2, r11
	strb r3, [r2]
	movs r2, #86
	adds r2, #255
	add r2, r11
	strb r3, [r2]
	movs r2, #94
	adds r2, #255
	add r2, r11
	strb r3, [r2]
.L_0200bad8:
	movs r4, #0
	mov r8, r4
.L_0200badc:
	movs r3, #11
	muls r3, r7
	add r3, r8
	mov r0, r11
	lsls r3, r3, #2
	ldrb r1, [r0, r3]
	movs r3, #182
	lsls r3, r3, #1
	add r3, r11
	movs r4, #0
	ldrsh r2, [r3, r4]
	movs r3, #183
	lsls r3, r3, #1
	add r3, r11
	movs r0, #0
	ldrsh r3, [r3, r0]
	add r2, r8
	adds r3, r3, r7
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r2, #1
	movs r0, #0
	movs r3, #1
	bl Func_0200566c
	movs r1, #1
	add r8, r1
	mov r2, r8
	cmp r2, #10
	ble .L_0200badc
	adds r7, #1
	cmp r7, #7
	ble .L_0200bad8
	movs r3, #0
	mov r9, r3
	bl Random16Far
	movs r4, #128
	lsls r4, r4, #8
	cmp r0, r4
	bcs .L_0200bbb8
	movs r0, #4
	mov r2, r9
	movs r1, #0
	str r0, [sp, #0]
	str r2, [sp, #4]
	movs r5, #2
	movs r6, #3
	mov r8, r0
	mov r10, r1
	movs r2, #5
	movs r1, #7
	movs r3, #7
	movs r0, #2
	str r6, [sp, #8]
	str r5, [sp, #12]
	bl Func_02003fc0
	mov r3, r8
	str r3, [sp, #0]
	mov r9, r0
	movs r1, #7
	movs r2, #6
	movs r3, #6
	movs r0, #8
	str r5, [sp, #4]
	str r5, [sp, #8]
	str r6, [sp, #12]
	bl Func_02003fc0
	mov r4, r9
	orrs r4, r0
	movs r1, #6
	movs r0, #5
	movs r2, #9
	movs r3, #12
	mov r9, r4
	bl Func_02004270
	movs r3, #12
	mov r0, r10
	str r3, [sp, #0]
	str r0, [sp, #8]
	movs r1, #6
	movs r2, #4
	movs r3, #3
	movs r0, #4
	str r5, [sp, #4]
	str r5, [sp, #12]
	bl Func_02003fc0
	mov r1, r9
	movs r3, #14
	orrs r1, r0
	str r3, [sp, #0]
	movs r3, #1
	mov r9, r1
	str r3, [sp, #8]
	movs r2, #5
	movs r0, #5
	movs r1, #5
	movs r3, #2
	str r6, [sp, #4]
	str r6, [sp, #12]
	bl Func_02003fc0
	mov r2, r9
	orrs r2, r0
	mov r9, r2
	b .L_0200bc3e
.L_0200bbb8:
	movs r3, #0
	mov r4, r9
	movs r5, #4
	mov r8, r3
	movs r6, #2
	movs r1, #7
	movs r2, #4
	movs r3, #6
	movs r0, #2
	str r5, [sp, #0]
	str r4, [sp, #4]
	str r4, [sp, #8]
	str r6, [sp, #12]
	bl Func_02003fc0
	movs r1, #7
	str r5, [sp, #0]
	mov r9, r0
	movs r5, #3
	movs r2, #5
	movs r3, #7
	movs r0, #8
	str r6, [sp, #4]
	str r5, [sp, #8]
	str r5, [sp, #12]
	bl Func_02003fc0
	mov r1, r9
	orrs r1, r0
	mov r9, r1
	movs r0, #5
	movs r1, #6
	movs r2, #8
	movs r3, #12
	bl Func_02004270
	movs r3, #12
	mov r2, r8
	str r3, [sp, #0]
	str r2, [sp, #8]
	movs r1, #5
	movs r2, #4
	movs r3, #3
	movs r0, #5
	str r5, [sp, #4]
	str r6, [sp, #12]
	bl Func_02003fc0
	mov r3, r9
	orrs r3, r0
	mov r9, r3
	movs r3, #14
	str r3, [sp, #0]
	movs r3, #1
	mov r4, r8
	str r3, [sp, #8]
	movs r1, #6
	movs r0, #6
	movs r2, #5
	movs r3, #2
	str r4, [sp, #4]
	str r5, [sp, #12]
	bl Func_02003fc0
	mov r1, r9
	orrs r1, r0
	mov r9, r1
.L_0200bc3e:
	mov r2, r9
	cmp r2, #0
	beq .L_0200bc46
	b .L_0200ba82
.L_0200bc46:
	movs r3, #19
	str r3, [sp, #0]
	movs r3, #3
	str r3, [sp, #8]
	str r2, [sp, #12]
	movs r5, #2
	movs r1, #7
	movs r2, #5
	movs r3, #4
	movs r0, #0
	str r5, [sp, #4]
	bl Func_02003fc0
	movs r3, #17
	str r3, [sp, #0]
	movs r3, #1
	mov r9, r0
	str r3, [sp, #12]
	movs r6, #0
	movs r3, #3
	movs r0, #10
	movs r1, #7
	movs r2, #6
	str r6, [sp, #4]
	str r5, [sp, #8]
	bl Func_02003fc0
	mov r3, r9
	orrs r3, r0
	cmp r3, #0
	beq .L_0200bc86
	b .L_0200ba82
.L_0200bc86:
	movs r2, #0
	movs r1, #0
	movs r0, #0
	movs r7, #0
.L_0200bc8e:
	movs r4, #0
	mov r8, r4
.L_0200bc92:
	movs r3, #11
	muls r3, r7
	add r3, r8
	lsls r3, r3, #2
	mov r4, r11
	ldrb r5, [r4, r3]
	add r3, r11
	ldrb r6, [r3, #2]
	cmp r6, #5
	bne .L_0200bca8
	adds r2, #1
.L_0200bca8:
	cmp r6, #10
	bne .L_0200bcae
	adds r1, #1
.L_0200bcae:
	cmp r6, #12
	bne .L_0200bcbc
	adds r3, r5, #0
	subs r3, #8
	cmp r3, #1
	bhi .L_0200bcbc
	adds r0, #1
.L_0200bcbc:
	movs r3, #1
	add r8, r3
	mov r4, r8
	cmp r4, #10
	ble .L_0200bc92
	adds r7, #1
	cmp r7, #7
	ble .L_0200bc8e
	cmp r2, #0
	bne .L_0200bcd2
	b .L_0200ba82
.L_0200bcd2:
	cmp r1, #0
	bne .L_0200bcd8
	b .L_0200ba82
.L_0200bcd8:
	cmp r0, #0
	bne .L_0200bcde
	b .L_0200ba82
.L_0200bcde:
	bl Random16Far
	lsls r0, r0, #1
	lsrs r0, r0, #16
	mov r10, r0
	movs r1, #1
	movs r0, #7
	str r0, [sp, #16]
	str r1, [sp, #20]
.L_0200bcf0:
	bl Random16Far
	movs r5, #11
	adds r3, r0, #0
	muls r3, r5
	lsrs r3, r3, #16
	mov r8, r3
	bl Random16Far
	lsls r3, r0, #1
	adds r3, r3, r0
	lsrs r3, r3, #16
	adds r7, r3, #5
	mov r0, r8
	adds r1, r7, #0
	bl Func_02003934
	cmp r0, #0
	bne .L_0200bd64
	adds r3, r7, #0
	muls r3, r5
	add r3, r8
	lsls r2, r3, #2
	mov r3, r11
	ldrb r5, [r3, r2]
	subs r3, r5, #1
	cmp r3, #1
	bhi .L_0200bd64
	mov r4, r11
	adds r3, r4, r2
	mov r0, r10
	ldrb r6, [r3, #2]
	cmp r0, #0
	bne .L_0200bd3c
	movs r3, #6
	ands r3, r6
	cmp r3, #0
	bne .L_0200bd4a
.L_0200bd3c:
	mov r1, r10
	cmp r1, #1
	bne .L_0200bd64
	movs r3, #9
	ands r3, r6
	cmp r3, #0
	beq .L_0200bd64
.L_0200bd4a:
	movs r3, #11
	muls r3, r7
	add r3, r8
	lsls r3, r3, #2
	movs r2, #10
	mov r4, r11
	strb r2, [r4, r3]
	movs r0, #6
	add r3, r11
	movs r2, #1
	strb r2, [r3, #3]
	str r0, [sp, #16]
	b .L_0200bd6e
.L_0200bd64:
	ldr r1, [sp, #20]
	adds r1, #1
	str r1, [sp, #20]
	cmp r1, #88
	ble .L_0200bcf0
.L_0200bd6e:
	movs r2, #3
	mov r9, r2
.L_0200bd72:
	movs r3, #1
	str r3, [sp, #20]
.L_0200bd76:
	bl Random16Far
	movs r5, #11
	adds r3, r0, #0
	muls r3, r5
	lsrs r3, r3, #16
	mov r8, r3
	bl Random16Far
	lsls r0, r0, #3
	lsrs r7, r0, #16
	adds r1, r7, #0
	mov r0, r8
	bl Func_02003934
	cmp r0, #0
	bne .L_0200be30
	adds r3, r7, #0
	muls r3, r5
	add r3, r8
	lsls r3, r3, #2
	mov r4, r11
	ldrb r5, [r4, r3]
	cmp r5, #3
	beq .L_0200bdb0
	cmp r5, #8
	beq .L_0200bdb0
	cmp r5, #9
	bne .L_0200be30
.L_0200bdb0:
	movs r3, #11
	muls r3, r7
	add r3, r8
	lsls r3, r3, #2
	add r3, r11
	ldrb r6, [r3, #2]
	mov r10, r3
	cmp r6, #6
	beq .L_0200be30
	cmp r6, #9
	beq .L_0200be30
	cmp r5, #8
	beq .L_0200bdf8
	cmp r5, #8
	bgt .L_0200bdd4
	cmp r5, #3
	beq .L_0200bdda
	b .L_0200be1a
.L_0200bdd4:
	cmp r5, #9
	beq .L_0200be0a
	b .L_0200be1a
.L_0200bdda:
	bl Random16Far
	lsls r0, r0, #1
	lsrs r0, r0, #16
	adds r0, #1
	mov r1, r10
	strb r0, [r1]
	b .L_0200be1a
	.2byte 0x0000
.L_0200bdec:
	.4byte gSceneState
.L_0200bdf0:
	.4byte Data_030011bc
.L_0200bdf4:
	.4byte IwramClearWords
.L_0200bdf8:
	bl Random16Far
	lsls r0, r0, #1
	lsrs r0, r0, #16
	lsls r0, r0, #1
	adds r0, #4
	mov r2, r10
	strb r0, [r2]
	b .L_0200be1a
.L_0200be0a:
	bl Random16Far
	lsls r0, r0, #1
	lsrs r0, r0, #16
	lsls r0, r0, #1
	adds r0, #5
	mov r3, r10
	strb r0, [r3]
.L_0200be1a:
	movs r3, #11
	muls r3, r7
	add r3, r8
	lsls r3, r3, #2
	add r3, r11
	movs r2, #1
	strb r2, [r3, #3]
	ldr r4, [sp, #16]
	subs r4, #1
	str r4, [sp, #16]
	b .L_0200be3a
.L_0200be30:
	ldr r0, [sp, #20]
	adds r0, #1
	str r0, [sp, #20]
	cmp r0, #88
	ble .L_0200bd76
.L_0200be3a:
	movs r1, #1
	negs r1, r1
	add r9, r1
	mov r2, r9
	cmp r2, #0
	bge .L_0200bd72
	movs r3, #1
	str r3, [sp, #20]
.L_0200be4a:
	bl Random16Far
	movs r5, #11
	adds r3, r0, #0
	muls r3, r5
	lsrs r3, r3, #16
	mov r8, r3
	bl Random16Far
	lsls r0, r0, #3
	lsrs r7, r0, #16
	adds r1, r7, #0
	mov r0, r8
	bl Func_02003934
	cmp r0, #0
	bne .L_0200be9e
	adds r3, r7, #0
	muls r3, r5
	add r3, r8
	lsls r3, r3, #2
	mov r4, r11
	ldrb r5, [r4, r3]
	cmp r5, #4
	beq .L_0200be88
	cmp r5, #6
	beq .L_0200be88
	cmp r5, #5
	beq .L_0200be88
	cmp r5, #7
	bne .L_0200be9e
.L_0200be88:
	movs r3, #11
	muls r3, r7
	add r3, r8
	lsls r3, r3, #2
	add r3, r11
	movs r2, #1
	strb r2, [r3, #3]
	ldr r0, [sp, #16]
	subs r0, #1
	str r0, [sp, #16]
	b .L_0200bea8
.L_0200be9e:
	ldr r1, [sp, #20]
	adds r1, #1
	str r1, [sp, #20]
	cmp r1, #88
	ble .L_0200be4a
.L_0200bea8:
	ldr r2, [sp, #16]
	cmp r2, #0
	ble .L_0200bf06
	mov r9, r2
.L_0200beb0:
	movs r3, #1
	str r3, [sp, #20]
.L_0200beb4:
	bl Random16Far
	movs r5, #11
	adds r3, r0, #0
	muls r3, r5
	lsrs r3, r3, #16
	mov r8, r3
	bl Random16Far
	lsls r0, r0, #3
	lsrs r7, r0, #16
	adds r1, r7, #0
	mov r0, r8
	bl Func_02003934
	cmp r0, #0
	bne .L_0200bef0
	adds r3, r7, #0
	muls r3, r5
	add r3, r8
	lsls r0, r3, #2
	mov r4, r11
	ldrb r5, [r4, r0]
	subs r3, r5, #1
	cmp r3, #1
	bhi .L_0200bef0
	adds r2, r4, r0
	movs r3, #1
	strb r3, [r2, #3]
	b .L_0200befa
.L_0200bef0:
	ldr r0, [sp, #20]
	adds r0, #1
	str r0, [sp, #20]
	cmp r0, #88
	ble .L_0200beb4
.L_0200befa:
	movs r1, #1
	negs r1, r1
	add r9, r1
	mov r2, r9
	cmp r2, #0
	bne .L_0200beb0
.L_0200bf06:
	movs r7, #0
.L_0200bf08:
	movs r3, #0
	mov r8, r3
.L_0200bf0c:
	movs r3, #11
	muls r3, r7
	add r3, r8
	lsls r3, r3, #2
	mov r4, r11
	ldrb r5, [r4, r3]
	add r3, r11
	ldrb r6, [r3, #3]
	cmp r6, #0
	beq .L_0200bf6e
	bl Random16Far
	lsls r0, r0, #3
	lsrs r0, r0, #16
	cmp r0, #6
	bhi .L_0200bf6e
	ldr r2, .L_0200bfbc
	lsls r3, r0, #2
	ldr r3, [r3, r2]
	mov pc, r3
.L_0200bf34:
	.4byte .L_0200bf62
	.4byte .L_0200bf62
	.4byte .L_0200bf62
	.4byte .L_0200bf50
	.4byte .L_0200bf50
	.4byte .L_0200bf50
	.4byte .L_0200bf58
.L_0200bf50:
	mov r0, r8
	adds r1, r7, #0
	movs r2, #1
	b .L_0200bf68
.L_0200bf58:
	adds r1, r7, #0
	movs r2, #0
	mov r0, r8
	bl Func_02004380
.L_0200bf62:
	mov r0, r8
	adds r1, r7, #0
	movs r2, #0
.L_0200bf68:
	bl Func_02004380
	adds r5, r0, #0
.L_0200bf6e:
	movs r3, #182
	lsls r3, r3, #1
	add r3, r11
	movs r0, #0
	ldrsh r2, [r3, r0]
	movs r3, #183
	lsls r3, r3, #1
	add r3, r11
	movs r1, #0
	ldrsh r3, [r3, r1]
	add r2, r8
	adds r3, r3, r7
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r2, #1
	movs r3, #1
	adds r0, r6, #0
	adds r1, r5, #0
	bl Func_0200566c
	movs r2, #1
	add r8, r2
	mov r3, r8
	cmp r3, #10
	ble .L_0200bf0c
	adds r7, #1
	cmp r7, #7
	ble .L_0200bf08
	movs r3, #176
	lsls r3, r3, #1
	add r3, r11
	ldr r0, [r3]
	add sp, #24
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0200bfbc:
	.4byte .L_0200bf34
	.section .text.x0200bfc0,"ax",%progbits
	.global Func_02003fc0
	.thumb_func
Func_02003fc0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #16
	str r2, [sp, #12]
	adds r7, r0, #0
	mov r8, r1
	ldr r0, [sp, #48]
	subs r2, r3, r1
	ldr r1, [sp, #12]
	subs r0, #1
	subs r6, r1, r7
	str r3, [sp, #8]
	str r0, [sp, #48]
	cmp r6, #0
	bge .L_0200bfea
	negs r6, r6
.L_0200bfea:
	ldr r0, [sp, #48]
	subs r3, r0, r6
	adds r6, r2, #0
	cmp r6, #0
	bge .L_0200bff6
	negs r6, r6
.L_0200bff6:
	ldr r5, .L_0200c1fc
	movs r1, #32
	ldr r0, .L_0200c200
	subs r6, r3, r6
	mov lr, r5
	.2byte 0xf800
	movs r1, #32
	ldr r0, .L_0200c204
	mov lr, r5
	.2byte 0xf800
	movs r1, #32
	ldr r0, .L_0200c208
	mov lr, r5
	.2byte 0xf800
	movs r1, #32
	ldr r0, .L_0200c20c
	mov lr, r5
	.2byte 0xf800
	movs r1, #32
	ldr r0, .L_0200c210
	mov lr, r5
	.2byte 0xf800
	ldr r0, .L_0200c214
	movs r1, #32
	mov lr, r5
	.2byte 0xf800
	movs r3, #1
	ands r3, r6
	cmp r3, #0
	beq .L_0200c038
.L_0200c032:
	movs r0, #1
	negs r0, r0
	b .L_0200c1ec
.L_0200c038:
	movs r5, #0
	str r5, [sp, #0]
	mov r9, r7
.L_0200c03e:
	ldr r1, [sp, #0]
	movs r2, #160
	adds r1, #1
	lsls r2, r2, #3
	str r1, [sp, #0]
	cmp r1, r2
	bhi .L_0200c032
	movs r3, #11
	mov r0, r8
	muls r0, r3
	adds r3, r0, #0
	ldr r0, .L_0200c218
	add r3, r9
	lsls r3, r3, #2
	ldrb r2, [r0, r3]
	ldr r1, .L_0200c20c
	adds r3, r0, r3
	strb r2, [r1, r5]
	ldrb r3, [r3, #2]
	ldr r2, .L_0200c214
	mov r0, r8
	strb r3, [r2, r5]
	ldr r3, .L_0200c204
	mov r2, r9
	strb r2, [r3, r5]
	ldr r3, .L_0200c208
	add r2, sp, #52
	strb r0, [r3, r5]
	ldrb r2, [r2]
	ldr r3, .L_0200c200
	ldr r0, .L_0200c210
	strb r2, [r3, r5]
	ldr r2, .L_0200c21c
	ldrb r3, [r1, r5]
	ldrb r1, [r0, r5]
	ldrb r3, [r2, r3]
	ldr r2, [sp, #52]
	orrs r1, r3
	adds r2, #2
	movs r3, #3
	ands r2, r3
	movs r3, #1
	lsls r3, r2
	orrs r1, r3
	movs r3, #0
	strb r1, [r0, r5]
	str r3, [sp, #4]
.L_0200c09c:
	ldr r0, [sp, #4]
	adds r0, #1
	str r0, [sp, #4]
	cmp r0, #63
	bhi .L_0200c188
	bl Random16Far
	movs r1, #254
	lsls r1, r1, #7
	adds r1, #255
	ldr r6, [sp, #52]
	cmp r0, r1
	bls .L_0200c0be
	bl Random16Far
	lsls r0, r0, #2
	lsrs r6, r0, #16
.L_0200c0be:
	ldr r2, .L_0200c210
	movs r1, #1
	ldrb r0, [r2, r5]
	adds r3, r0, #0
	asrs r3, r6
	ands r3, r1
	cmp r3, #0
	bne .L_0200c09c
	ldr r1, .L_0200c220
	lsls r3, r6, #1
	ldrsb r2, [r1, r3]
	adds r3, #1
	ldrsb r3, [r1, r3]
	add r2, r9
	add r3, r8
	mov r11, r2
	mov r10, r3
	cmp r2, #10
	bhi .L_0200c172
	cmp r3, #7
	bhi .L_0200c172
	ldr r3, [sp, #48]
	subs r3, #1
	cmp r5, r3
	bne .L_0200c0fc
	ldr r2, [sp, #12]
	cmp r11, r2
	bne .L_0200c172
	ldr r3, [sp, #8]
	cmp r10, r3
	bne .L_0200c172
.L_0200c0fc:
	movs r1, #11
	mov r3, r10
	muls r3, r1
	ldr r1, .L_0200c218
	add r3, r11
	lsls r3, r3, #2
	adds r2, r1, r3
	ldrb r3, [r2, #1]
	cmp r3, #0
	bne .L_0200c172
	ldrb r7, [r2, #2]
	ldr r3, [sp, #60]
	movs r1, #1
	asrs r7, r3
	ands r7, r1
	cmp r7, #0
	bne .L_0200c172
	ldrb r3, [r2]
	cmp r3, #10
	beq .L_0200c172
	ldr r2, .L_0200c210
	movs r3, #1
	lsls r3, r6
	orrs r0, r3
	strb r0, [r2, r5]
	ldr r3, .L_0200c20c
	ldr r0, [sp, #52]
	ldrb r2, [r3, r5]
	adds r1, r6, #0
	bl Func_02004224
	adds r4, r0, #0
	movs r0, #11
	mov r3, r8
	muls r3, r0
	ldr r1, .L_0200c218
	add r3, r9
	lsls r3, r3, #2
	ldr r0, [sp, #60]
	adds r3, r1, r3
	ldrb r3, [r3, #2]
	movs r2, #1
	lsls r2, r0
	mov r1, r8
	orrs r3, r2
	mov r0, r9
	adds r2, r4, #0
	bl Func_02004270
	ldr r1, [sp, #48]
	adds r5, #1
	mov r9, r11
	mov r8, r10
	str r6, [sp, #52]
	cmp r5, r1
	beq .L_0200c1ba
	ldr r2, .L_0200c210
	strb r7, [r2, r5]
	b .L_0200c03e
.L_0200c172:
	ldr r0, .L_0200c210
	movs r3, #1
	ldrb r2, [r0, r5]
	lsls r3, r6
	orrs r3, r2
	movs r1, #240
	strb r3, [r0, r5]
	lsls r1, r1, #20
	lsls r3, r3, #24
	cmp r3, r1
	bne .L_0200c09c
.L_0200c188:
	ldr r2, .L_0200c210
	movs r3, #0
	strb r3, [r2, r5]
	cmp r5, #0
	bne .L_0200c194
	b .L_0200c032
.L_0200c194:
	ldr r3, .L_0200c204
	subs r5, #1
	ldrb r3, [r3, r5]
	mov r9, r3
	ldr r3, .L_0200c208
	mov r0, r9
	ldrb r3, [r3, r5]
	mov r8, r3
	ldr r3, .L_0200c200
	mov r1, r8
	ldrb r3, [r3, r5]
	str r3, [sp, #52]
	ldr r3, .L_0200c20c
	ldrb r2, [r3, r5]
	ldr r3, .L_0200c214
	ldrb r3, [r3, r5]
	bl Func_02004270
	b .L_0200c03e
.L_0200c1ba:
	movs r3, #11
	mov r5, r10
	muls r5, r3
	ldr r3, .L_0200c218
	add r5, r11
	lsls r5, r5, #2
	ldrb r2, [r3, r5]
	ldr r1, [sp, #56]
	adds r0, r6, #0
	bl Func_02004224
	adds r4, r0, #0
	ldr r0, .L_0200c218
	ldr r1, [sp, #60]
	adds r5, r0, r5
	ldrb r3, [r5, #2]
	movs r2, #1
	lsls r2, r1
	orrs r3, r2
	mov r0, r11
	mov r1, r10
	adds r2, r4, #0
	bl Func_02004270
	movs r0, #0
.L_0200c1ec:
	add sp, #16
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200c1fc:
	.4byte IwramClearWords
.L_0200c200:
	.4byte gOverlayArea + 0x6fc8
.L_0200c204:
	.4byte gOverlayArea + 0x6fe8
.L_0200c208:
	.4byte gOverlayArea + 0x7008
.L_0200c20c:
	.4byte gOverlayArea + 0x7028
.L_0200c210:
	.4byte gOverlayArea + 0x7048
.L_0200c214:
	.4byte gOverlayArea + 0x7068
.L_0200c218:
	.4byte gSceneState
.L_0200c21c:
	.4byte Data_02005ac8 + 0x1
.L_0200c220:
	.4byte Data_02005ac0 + 0x1
	.section .text.x0200c224,"ax",%progbits
	.global Func_02004224
	.thumb_func
Func_02004224:
	push {lr}
	ldr r3, .L_0200c26c
	lsls r0, r0, #2
	adds r0, r0, r1
	ldrb r0, [r3, r0]
	cmp r2, #2
	bne .L_0200c238
	cmp r0, #1
	bne .L_0200c238
	movs r0, #3
.L_0200c238:
	cmp r2, #1
	bne .L_0200c242
	cmp r0, #2
	bne .L_0200c242
	movs r0, #3
.L_0200c242:
	cmp r2, #4
	bne .L_0200c24c
	cmp r0, #6
	bne .L_0200c24c
	movs r0, #8
.L_0200c24c:
	cmp r2, #6
	bne .L_0200c256
	cmp r0, #4
	bne .L_0200c256
	movs r0, #8
.L_0200c256:
	cmp r2, #7
	bne .L_0200c260
	cmp r0, #5
	bne .L_0200c260
	movs r0, #9
.L_0200c260:
	cmp r2, #5
	bne .L_0200c26a
	cmp r0, #7
	bne .L_0200c26a
	movs r0, #9
.L_0200c26a:
	pop {pc}
.L_0200c26c:
	.4byte Data_02005ad4
	.section .text.x0200c270,"ax",%progbits
	.global Func_02004270
	.thumb_func
Func_02004270:
	push {r5, lr}
	movs r4, #11
	muls r1, r4
	ldr r5, .L_0200c284
	adds r1, r1, r0
	lsls r1, r1, #2
	strb r2, [r5, r1]
	adds r1, r1, r5
	strb r3, [r1, #2]
	pop {r5, pc}
.L_0200c284:
	.4byte gSceneState
	.section .text.x0200c288,"ax",%progbits
	.global Func_02004288
	.thumb_func
Func_02004288:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	ldr r5, .L_0200c37c
	mov r10, r0
	movs r0, #184
	lsls r0, r0, #1
	mov r9, r1
	movs r2, #178
	adds r3, r5, r0
	mov r1, r9
	lsls r2, r2, #1
	strh r1, [r3]
	mov r4, r10
	adds r3, r5, r2
	strh r4, [r3]
	movs r0, #0
	sub sp, #8
	mov r8, r0
.L_0200c2b2:
	movs r7, #0
.L_0200c2b4:
	movs r3, #11
	mov r1, r8
	muls r1, r3
	adds r3, r1, #0
	adds r3, r3, r7
	lsls r3, r3, #2
	adds r6, r5, r3
	ldrb r3, [r6, #3]
	cmp r3, #0
	beq .L_0200c356
	mov r0, r10
	bl Object_GetById
	mov r2, r9
	cmp r2, #0
	beq .L_0200c322
	movs r3, #182
	lsls r3, r3, #1
	adds r1, r5, r3
	movs r4, #0
	ldrsh r3, [r1, r4]
	movs r2, #128
	adds r3, r3, r7
	lsls r2, r2, #12
	lsls r3, r3, #20
	adds r3, r3, r2
	str r3, [r0, #8]
	movs r3, #183
	lsls r3, r3, #1
	adds r3, r3, r5
	mov r12, r3
	movs r4, #0
	ldrsh r3, [r3, r4]
	add r3, r8
	lsls r3, r3, #20
	adds r3, r3, r2
	str r3, [r0, #16]
	movs r4, #0
	ldrsh r3, [r1, r4]
	mov r1, r12
	movs r0, #0
	ldrsh r2, [r1, r0]
	adds r3, r3, r7
	add r2, r8
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #1
	movs r0, #0
	movs r1, #11
	movs r2, #1
	bl Func_0200566c
	movs r3, #2
	strb r3, [r6, #3]
	b .L_0200c352
.L_0200c322:
	mov r2, r9
	movs r3, #1
	str r2, [r0, #8]
	str r2, [r0, #16]
	movs r4, #182
	strb r3, [r6, #3]
	lsls r4, r4, #1
	adds r3, r5, r4
	adds r4, #2
	movs r0, #0
	ldrsh r2, [r3, r0]
	adds r3, r5, r4
	movs r0, #0
	ldrsh r3, [r3, r0]
	adds r2, r2, r7
	add r3, r8
	ldrb r1, [r6]
	movs r0, #1
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r2, #1
	movs r3, #1
	bl Func_0200566c
.L_0200c352:
	movs r1, #1
	add r10, r1
.L_0200c356:
	adds r7, #1
	cmp r7, #10
	ble .L_0200c2b4
	movs r2, #1
	add r8, r2
	mov r3, r8
	cmp r3, #7
	ble .L_0200c2b2
	movs r4, #179
	lsls r4, r4, #1
	adds r3, r5, r4
	mov r0, r10
	strh r0, [r3]
	add sp, #8
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_0200c37c:
	.4byte gSceneState
	.section .text.x0200c380,"ax",%progbits
	.global Func_02004380
	.thumb_func
Func_02004380:
	movs r3, #11
	muls r1, r3
	ldr r4, .L_0200c39c
	adds r1, r1, r0
	lsls r1, r1, #2
	ldrb r3, [r4, r1]
	ldr r0, .L_0200c3a0
	lsls r3, r3, #1
	adds r3, r3, r2
	ldrb r3, [r0, r3]
	strb r3, [r4, r1]
	ldrb r0, [r4, r1]
	bx lr
	.2byte 0x0000
.L_0200c39c:
	.4byte gSceneState
.L_0200c3a0:
	.4byte Data_02006ea8
	.section .text.x0200c3a4,"ax",%progbits
	.global Func_020043a4
	.thumb_func
Func_020043a4:
	push {r5, r6, r7, lr}
	adds r7, r0, #0
	movs r0, #15
	adds r5, r1, #0
	bl Object_GetById
	adds r6, r0, #0
	cmp r5, #7
	bgt .L_0200c3be
	ldr r3, [r6, #28]
	ldr r2, .L_0200c3e4
	adds r3, r3, r2
	str r3, [r6, #28]
.L_0200c3be:
	cmp r7, #1
	bne .L_0200c3e2
	ldr r3, .L_0200c3e8
	movs r2, #178
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r0, [r3, r2]
	movs r1, #1
	bl Func_02004288
	adds r0, r6, #0
	movs r1, #6
	bl Func_02005624
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r6, #28]
.L_0200c3e2:
	pop {r5, r6, r7, pc}
.L_0200c3e4:
	.4byte 0xffffe100
.L_0200c3e8:
	.4byte gSceneState
	.section .text.x0200c3ec,"ax",%progbits
	.global Func_020043ec
	.thumb_func
Func_020043ec:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r6, r0, #0
	sub sp, #8
	cmp r6, #1
	bne .L_0200c4a6
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #224
	ldr r3, [r3]
	ldr r7, .L_0200c4b0
	movs r1, #26
	ldrsh r0, [r3, r1]
	bl Object_GetById
	movs r3, #182
	adds r5, r0, #0
	lsls r3, r3, #1
	ldr r4, [r5, #8]
	adds r2, r7, r3
	movs r1, #0
	ldrsh r3, [r2, r1]
	asrs r4, r4, #20
	subs r4, r4, r3
	movs r3, #183
	lsls r3, r3, #1
	ldr r0, [r5, #16]
	adds r3, r3, r7
	mov r12, r3
	movs r1, #0
	ldrsh r3, [r3, r1]
	asrs r0, r0, #20
	subs r0, r0, r3
	movs r3, #11
	muls r3, r0
	adds r3, r3, r4
	lsls r3, r3, #2
	ldrb r1, [r7, r3]
	adds r3, r3, r7
	mov lr, r3
	movs r3, #0
	mov r8, r3
	mov r3, lr
	strb r6, [r3, #3]
	movs r3, #0
	ldrsh r6, [r2, r3]
	mov r3, r12
	mov lr, r6
	movs r6, #0
	ldrsh r2, [r3, r6]
	mov r6, lr
	adds r3, r6, r4
	adds r2, r2, r0
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #1
	movs r2, #1
	movs r3, #1
	bl Func_0200566c
	mov r1, r8
	str r1, [r5, #8]
	str r1, [r5, #16]
	movs r0, #0
.L_0200c46e:
	movs r3, #11
	muls r3, r1
	lsls r3, r3, #2
	adds r3, r7, r3
	ldrb r3, [r3, #3]
	movs r2, #0
	b .L_0200c48e
.L_0200c47c:
	adds r2, #1
	cmp r2, #10
	bhi .L_0200c494
	movs r3, #11
	muls r3, r1
	adds r3, r3, r2
	lsls r3, r3, #2
	adds r3, r7, r3
	ldrb r3, [r3, #3]
.L_0200c48e:
	cmp r3, #2
	bne .L_0200c47c
	movs r0, #1
.L_0200c494:
	cmp r0, #0
	bne .L_0200c4a6
	adds r1, #1
	cmp r1, #7
	bls .L_0200c46e
	movs r2, #184
	lsls r2, r2, #1
	adds r3, r7, r2
	strh r0, [r3]
.L_0200c4a6:
	add sp, #8
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200c4b0:
	.4byte gSceneState
	.section .text.x0200c4b4,"ax",%progbits
	.global Func_020044b4
	.thumb_func
Func_020044b4:
	push {lr}
	ldr r3, .L_0200c4cc
	movs r2, #178
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r1, r0, #0
	movs r2, #0
	ldrsh r0, [r3, r2]
	bl Object_SetModeById
	pop {pc}
	.2byte 0x0000
.L_0200c4cc:
	.4byte gSceneState
	.section .text.x0200c4d0,"ax",%progbits
	.global Func_020044d0
	.thumb_func
Func_020044d0:
	push {r5, r6, r7, lr}
	ldr r6, .L_0200c52c
	movs r1, #178
	lsls r1, r1, #1
	adds r3, r6, r1
	adds r1, #2
	movs r2, #0
	ldrsh r5, [r3, r2]
	adds r3, r6, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	adds r7, r0, #0
	cmp r5, r3
	bge .L_0200c52a
.L_0200c4ec:
	adds r0, r5, #0
	bl Object_GetById
	movs r1, #182
	adds r2, r0, #0
	lsls r1, r1, #1
	ldr r0, [r2, #8]
	adds r3, r6, r1
	movs r1, #0
	ldrsh r3, [r3, r1]
	ldr r1, [r2, #16]
	movs r2, #183
	lsls r2, r2, #1
	asrs r0, r0, #20
	subs r0, r0, r3
	adds r3, r6, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	asrs r1, r1, #20
	subs r1, r1, r3
	adds r2, r7, #0
	bl Func_02004380
	movs r1, #179
	lsls r1, r1, #1
	adds r3, r6, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	adds r5, #1
	cmp r5, r3
	blt .L_0200c4ec
.L_0200c52a:
	pop {r5, r6, r7, pc}
.L_0200c52c:
	.4byte gSceneState
	.section .text.x0200c530,"ax",%progbits
	.global Func_02004530
	.thumb_func
Func_02004530:
	push {r5, r6, lr}
	ldr r5, .L_0200c5d0
	movs r1, #181
	lsls r1, r1, #1
	adds r3, r5, r1
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	sub sp, #8
	cmp r3, #0
	bne .L_0200c5cc
	movs r2, #182
	lsls r2, r2, #1
	adds r3, r5, r2
	movs r1, #0
	ldrsh r2, [r3, r1]
	movs r1, #183
	lsls r1, r1, #1
	adds r3, r5, r1
	movs r1, #0
	ldrsh r3, [r3, r1]
	adds r2, #3
	subs r3, #2
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r1, #13
	movs r2, #1
	movs r3, #1
	movs r0, #1
	bl Func_0200567c
	movs r2, #184
	lsls r2, r2, #1
	adds r3, r5, r2
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	beq .L_0200c5c2
	movs r0, #248
	bl Func_02005854
	bl Func_020056b4
	movs r0, #0
	bl Func_0200580c
	movs r0, #3
	bl Func_020044b4
	movs r6, #19
.L_0200c594:
	movs r3, #186
	lsls r3, r3, #1
	adds r2, r5, r3
	ldr r3, [r2]
	ldr r1, .L_0200c5d4
	movs r0, #1
	adds r3, r3, r1
	str r3, [r2]
	subs r6, #1
	bl WaitFrames
	cmp r6, #0
	bge .L_0200c594
	movs r0, #0
	bl Func_020044d0
	bl Func_020056bc
	movs r3, #186
	lsls r3, r3, #1
	adds r2, r5, r3
	movs r3, #0
	str r3, [r2]
.L_0200c5c2:
	movs r1, #181
	lsls r1, r1, #1
	adds r2, r5, r1
	movs r3, #1
	strb r3, [r2]
.L_0200c5cc:
	add sp, #8
	pop {r5, r6, pc}
.L_0200c5d0:
	.4byte gSceneState
.L_0200c5d4:
	.4byte 0xfffffccd
	.section .text.x0200c5d8,"ax",%progbits
	.global Func_020045d8
	.thumb_func
Func_020045d8:
	push {r5, lr}
	ldr r1, .L_0200c61c
	movs r0, #181
	lsls r0, r0, #1
	adds r5, r1, r0
	movs r3, #0
	ldrsb r3, [r5, r3]
	sub sp, #8
	cmp r3, #0
	beq .L_0200c618
	movs r2, #182
	lsls r2, r2, #1
	adds r3, r1, r2
	movs r0, #0
	ldrsh r2, [r3, r0]
	movs r0, #183
	lsls r0, r0, #1
	adds r3, r1, r0
	movs r1, #0
	ldrsh r3, [r3, r1]
	adds r2, #3
	subs r3, #2
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r0, #1
	movs r3, #1
	movs r1, #12
	movs r2, #1
	bl Func_0200567c
	movs r3, #0
	strb r3, [r5]
.L_0200c618:
	add sp, #8
	pop {r5, pc}
.L_0200c61c:
	.4byte gSceneState
	.section .text.x0200c620,"ax",%progbits
	.global Func_02004620
	.thumb_func
Func_02004620:
	push {r5, r6, lr}
	ldr r5, .L_0200c6c4
	movs r1, #108
	adds r1, #255
	adds r3, r5, r1
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	sub sp, #8
	cmp r3, #0
	bne .L_0200c6c0
	movs r2, #182
	lsls r2, r2, #1
	adds r3, r5, r2
	movs r1, #0
	ldrsh r2, [r3, r1]
	movs r1, #183
	lsls r1, r1, #1
	adds r3, r5, r1
	movs r1, #0
	ldrsh r3, [r3, r1]
	adds r2, #7
	subs r3, #2
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r1, #13
	movs r2, #1
	movs r3, #1
	movs r0, #0
	bl Func_0200567c
	movs r2, #184
	lsls r2, r2, #1
	adds r3, r5, r2
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	beq .L_0200c6b6
	movs r0, #248
	bl Func_02005854
	bl Func_020056b4
	movs r0, #0
	bl Func_0200580c
	movs r0, #2
	bl Func_020044b4
	movs r6, #19
.L_0200c684:
	movs r3, #186
	lsls r3, r3, #1
	adds r2, r5, r3
	ldr r3, [r2]
	movs r1, #141
	lsls r1, r1, #2
	adds r1, #255
	adds r3, r3, r1
	str r3, [r2]
	movs r0, #1
	subs r6, #1
	bl WaitFrames
	cmp r6, #0
	bge .L_0200c684
	movs r0, #1
	bl Func_020044d0
	bl Func_020056bc
	movs r3, #186
	lsls r3, r3, #1
	adds r2, r5, r3
	movs r3, #0
	str r3, [r2]
.L_0200c6b6:
	movs r1, #108
	adds r1, #255
	adds r2, r5, r1
	movs r3, #1
	strb r3, [r2]
.L_0200c6c0:
	add sp, #8
	pop {r5, r6, pc}
.L_0200c6c4:
	.4byte gSceneState
	.section .text.x0200c6c8,"ax",%progbits
	.global Func_020046c8
	.thumb_func
Func_020046c8:
	push {r5, lr}
	ldr r1, .L_0200c70c
	movs r0, #108
	adds r0, #255
	adds r5, r1, r0
	movs r3, #0
	ldrsb r3, [r5, r3]
	sub sp, #8
	cmp r3, #0
	beq .L_0200c708
	movs r2, #182
	lsls r2, r2, #1
	adds r3, r1, r2
	movs r0, #0
	ldrsh r2, [r3, r0]
	movs r0, #183
	lsls r0, r0, #1
	adds r3, r1, r0
	movs r1, #0
	ldrsh r3, [r3, r1]
	adds r2, #7
	subs r3, #2
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r0, #0
	movs r3, #1
	movs r1, #12
	movs r2, #1
	bl Func_0200567c
	movs r3, #0
	strb r3, [r5]
.L_0200c708:
	add sp, #8
	pop {r5, pc}
.L_0200c70c:
	.4byte gSceneState
	.section .text.x0200c710,"ax",%progbits
	.global Func_02004710
	.thumb_func
Func_02004710:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r0, #0
	sub sp, #12
	str r0, [sp, #0]
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	ldr r6, .L_0200c894
	adds r3, #228
	movs r1, #188
	ldr r2, [r3]
	lsls r1, r1, #1
	adds r1, r1, r6
	mov r8, r1
	ldr r1, .L_0200c898
	mov r9, r2
	ldr r3, [r3, #4]
	mov r5, r9
	ands r5, r1
	mov r10, r3
	mov r9, r5
	movs r5, #180
	mov r0, r10
	lsls r5, r5, #1
	ands r0, r1
	adds r3, r6, r5
	mov r10, r0
	movs r0, #0
	ldrsh r3, [r3, r0]
	ldr r2, .L_0200c89c
	lsls r3, r3, #2
	adds r3, r3, r2
	ldrh r3, [r3, #2]
	movs r2, #186
	lsrs r3, r3, #5
	lsls r2, r2, #1
	mov r11, r3
	adds r3, r6, r2
	ldr r2, [r3]
	cmp r2, #0
	beq .L_0200c79c
	add r0, sp, #4
	ldr r3, [r0, #4]
	lsls r2, r2, #16
	lsrs r2, r2, #16
	ands r3, r1
	orrs r3, r2
	str r3, [r0, #4]
	ldr r3, [sp, #4]
	movs r2, #128
	ands r3, r1
	lsls r2, r2, #1
	orrs r3, r2
	movs r2, #255
	lsls r2, r2, #8
	adds r2, #255
	ands r3, r2
	movs r2, #128
	lsls r2, r2, #17
	orrs r3, r2
	str r3, [sp, #4]
	bl Func_020055e4
	str r0, [sp, #0]
.L_0200c79c:
	movs r5, #178
	lsls r5, r5, #1
	movs r1, #179
	adds r3, r6, r5
	lsls r1, r1, #1
	movs r0, #0
	ldrsh r7, [r3, r0]
	adds r3, r6, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r7, r3
	bge .L_0200c884
.L_0200c7b4:
	adds r0, r7, #0
	bl Object_GetById
	ldr r5, [r0, #8]
	cmp r5, #0
	beq .L_0200c874
	mov r3, r9
	subs r4, r5, r3
	ldr r3, [r0, #12]
	movs r1, #128
	ldr r0, [r0, #16]
	lsls r1, r1, #13
	adds r3, r3, r1
	mov r1, r10
	subs r2, r0, r1
	subs r1, r2, r3
	adds r3, r3, r2
	asrs r3, r3, #16
	asrs r2, r5, #20
	movs r5, #182
	lsls r5, r5, #1
	adds r3, #58
	mov r12, r3
	adds r3, r6, r5
	movs r5, #0
	ldrsh r3, [r3, r5]
	movs r5, #183
	lsls r5, r5, #1
	subs r2, r2, r3
	adds r3, r6, r5
	movs r5, #0
	ldrsh r3, [r3, r5]
	asrs r0, r0, #20
	subs r0, r0, r3
	movs r3, #11
	muls r3, r0
	adds r3, r3, r2
	lsls r3, r3, #2
	ldrb r2, [r6, r3]
	movs r3, #128
	asrs r4, r4, #16
	lsls r3, r3, #1
	adds r3, #255
	subs r4, #8
	asrs r1, r1, #16
	ands r4, r3
	subs r1, #8
	movs r3, #255
	mov r0, r8
	ands r1, r3
	movs r3, #0
	stmia r0!, {r3}
	ldr r3, .L_0200c8a0
	lsls r4, r4, #16
	orrs r1, r4
	orrs r1, r3
	stmia r0!, {r1}
	lsls r2, r2, #3
	movs r3, #128
	lsls r3, r3, #4
	add r2, r11
	orrs r2, r3
	str r2, [r0]
	movs r0, #186
	lsls r0, r0, #1
	adds r3, r6, r0
	ldr r3, [r3]
	cmp r3, #0
	beq .L_0200c868
	mov r1, r8
	ldrb r3, [r1, #5]
	movs r5, #4
	negs r5, r5
	adds r2, r5, #0
	ands r3, r2
	movs r2, #1
	orrs r3, r2
	strb r3, [r1, #5]
	ldr r2, [sp, #0]
	movs r3, #31
	ands r2, r3
	movs r0, #63
	ldrb r3, [r1, #7]
	negs r0, r0
	adds r1, r0, #0
	ands r3, r1
	lsls r2, r2, #1
	orrs r3, r2
	mov r1, r8
	strb r3, [r1, #7]
.L_0200c868:
	mov r0, r8
	mov r1, r12
	bl Func_020055ec
	movs r2, #12
	add r8, r2
.L_0200c874:
	movs r5, #179
	lsls r5, r5, #1
	adds r3, r6, r5
	movs r0, #0
	ldrsh r3, [r3, r0]
	adds r7, #1
	cmp r7, r3
	blt .L_0200c7b4
.L_0200c884:
	add sp, #12
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200c894:
	.4byte gSceneState
.L_0200c898:
	.4byte 0xffff0000
.L_0200c89c:
	.4byte ResourceTableEntries
.L_0200c8a0:
	.4byte 0x40002000
	.section .text.x0200c8a4,"ax",%progbits
	.global Func_020048a4
	.thumb_func
Func_020048a4:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	adds r6, r0, #0
	movs r0, #10
	adds r0, #255
	adds r7, r1, #0
	mov r8, r2
	mov r10, r3
	ldr r5, .L_0200c92c
	bl Func_02005604
	cmp r0, #0
	beq .L_0200c8de
	bl Func_020038ac
	bl Func_02003974
	movs r2, #176
	lsls r2, r2, #1
	movs r1, #144
	adds r3, r5, r2
	ldr r0, .L_0200c930
	lsls r1, r1, #3
	ldr r5, [r3]
	bl Func_0200557c
	b .L_0200c916
.L_0200c8de:
	movs r1, #248
	lsls r1, r1, #1
	ldr r3, .L_0200c934
	adds r0, r5, #0
	mov lr, r3
	.2byte 0xf800
	movs r2, #182
	lsls r2, r2, #1
	adds r3, r5, r2
	adds r2, #2
	strh r6, [r3]
	adds r3, r5, r2
	strh r7, [r3]
	bl Func_020038ac
	mov r0, r8
	bl Func_02003a64
	movs r1, #0
	adds r5, r0, #0
	mov r0, r10
	bl Func_02004288
	movs r1, #144
	ldr r0, .L_0200c930
	lsls r1, r1, #3
	bl Func_0200557c
.L_0200c916:
	mov r0, r10
	bl Object_GetById
	movs r3, #2
	adds r0, #92
	strb r3, [r0]
	adds r0, r5, #0
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_0200c92c:
	.4byte gSceneState
.L_0200c930:
	.4byte Func_02004710
.L_0200c934:
	.4byte IwramClearWords
	.section .text.x0200c938,"ax",%progbits
	.global Func_02004938
	.thumb_func
Func_02004938:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #40
	mov r11, r0
	str r1, [sp, #36]
	mov r1, r11
	adds r1, #100
	str r1, [sp, #32]
	ldr r5, .L_0200cc88
	movs r3, #0
	ldrsh r2, [r1, r3]
	adds r1, #2
	str r1, [sp, #28]
	mov r10, r2
	movs r3, #0
	ldrsh r2, [r1, r3]
	subs r1, #4
	str r1, [sp, #24]
	mov r8, r2
	movs r3, #11
	ldrb r2, [r1]
	mov r1, r8
	muls r1, r3
	mov r9, r2
	lsls r2, r2, #14
	str r2, [sp, #20]
	adds r3, r1, #0
	add r3, r10
	lsls r3, r3, #2
	ldrb r2, [r5, r3]
	adds r3, r3, r5
	ldrb r3, [r3, #3]
	mov r1, r9
	str r3, [sp, #16]
	lsls r3, r1, #8
	adds r7, r3, r2
	ldr r3, [sp, #36]
	ldr r2, [sp, #36]
	lsls r3, r3, #10
	subs r2, #8
	adds r0, r3, #0
	str r2, [sp, #12]
	str r3, [sp, #8]
	bl Math_Sine
	adds r1, r0, #0
	movs r0, #128
	ldr r6, .L_0200cc8c
	lsls r0, r0, #12
	mov lr, r6
	.2byte 0xf800
	adds r4, r0, #0
	ldr r0, [sp, #8]
	str r4, [sp, #0]
	bl Math_Cosine
	adds r1, r0, #0
	movs r0, #128
	lsls r0, r0, #12
	mov lr, r6
	.2byte 0xf800
	movs r1, #182
	lsls r1, r1, #1
	adds r3, r5, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	movs r2, #128
	add r3, r10
	lsls r2, r2, #12
	lsls r3, r3, #20
	adds r1, #2
	adds r6, r3, r2
	adds r3, r5, r1
	movs r1, #0
	ldrsh r3, [r3, r1]
	ldr r4, [sp, #0]
	add r3, r8
	lsls r3, r3, #20
	adds r5, r3, r2
	ldr r3, [sp, #16]
	mov r2, r11
	adds r2, #99
	str r2, [sp, #4]
	cmp r3, #2
	bne .L_0200ca14
	ldrb r3, [r2]
	movs r1, #0
	mov r10, r1
	mov r8, r1
	mov r9, r1
	cmp r3, #1
	beq .L_0200c9fc
	b .L_0200ccf6
.L_0200c9fc:
	movs r0, #204
	bl Func_02005854
	ldr r1, .L_0200cc90
	mov r0, r11
	bl Func_02005634
	add r2, sp, #16
	ldrb r2, [r2]
	ldr r3, [sp, #4]
	strb r2, [r3]
	b .L_0200ccf6
.L_0200ca14:
	movs r3, #12
	adds r3, #255
	cmp r7, r3
	bne .L_0200ca1e
	b .L_0200cc74
.L_0200ca1e:
	cmp r7, r3
	bgt .L_0200ca9c
	cmp r7, #11
	bne .L_0200ca28
	b .L_0200cc6a
.L_0200ca28:
	cmp r7, #11
	bgt .L_0200ca5e
	cmp r7, #6
	bne .L_0200ca32
	b .L_0200cbb4
.L_0200ca32:
	cmp r7, #6
	bgt .L_0200ca4a
	cmp r7, #2
	bge .L_0200ca3c
	b .L_0200ccd2
.L_0200ca3c:
	cmp r7, #3
	bgt .L_0200ca42
	b .L_0200cb54
.L_0200ca42:
	cmp r7, #5
	bne .L_0200ca48
	b .L_0200cc30
.L_0200ca48:
	b .L_0200ccd2
.L_0200ca4a:
	cmp r7, #9
	bne .L_0200ca50
	b .L_0200cc30
.L_0200ca50:
	cmp r7, #9
	ble .L_0200ca56
	b .L_0200cc98
.L_0200ca56:
	cmp r7, #8
	bne .L_0200ca5c
	b .L_0200cbb4
.L_0200ca5c:
	b .L_0200ccd2
.L_0200ca5e:
	movs r3, #8
	adds r3, #255
	cmp r7, r3
	bne .L_0200ca68
	b .L_0200cbd0
.L_0200ca68:
	cmp r7, r3
	bgt .L_0200ca8a
	subs r3, #4
	cmp r7, r3
	beq .L_0200cb60
	cmp r7, r3
	bgt .L_0200ca7e
	subs r3, #2
	cmp r7, r3
	beq .L_0200cb60
	b .L_0200ccd2
.L_0200ca7e:
	movs r1, #131
	lsls r1, r1, #1
	cmp r7, r1
	bne .L_0200ca88
	b .L_0200cc10
.L_0200ca88:
	b .L_0200ccd2
.L_0200ca8a:
	movs r3, #10
	adds r3, #255
	cmp r7, r3
	bne .L_0200ca94
	b .L_0200cbd0
.L_0200ca94:
	cmp r7, r3
	ble .L_0200ca9a
	b .L_0200cc98
.L_0200ca9a:
	b .L_0200cc10
.L_0200ca9c:
	movs r3, #134
	lsls r3, r3, #1
	adds r3, #255
	cmp r7, r3
	bne .L_0200caa8
	b .L_0200cc7e
.L_0200caa8:
	cmp r7, r3
	bgt .L_0200cae4
	subs r3, #4
	cmp r7, r3
	bne .L_0200cab4
	b .L_0200cbf2
.L_0200cab4:
	cmp r7, r3
	bgt .L_0200cad0
	movs r2, #129
	lsls r2, r2, #1
	adds r2, #255
	cmp r7, r2
	bgt .L_0200cac4
	b .L_0200ccd2
.L_0200cac4:
	subs r3, #3
	cmp r7, r3
	blt .L_0200cb46
	cmp r7, r3
	beq .L_0200cb7a
	b .L_0200ccd2
.L_0200cad0:
	movs r3, #133
	lsls r3, r3, #1
	adds r3, #255
	cmp r7, r3
	bne .L_0200cadc
	b .L_0200cbf2
.L_0200cadc:
	cmp r7, r3
	ble .L_0200cae2
	b .L_0200cc98
.L_0200cae2:
	b .L_0200cb7a
.L_0200cae4:
	movs r3, #192
	lsls r3, r3, #2
	adds r3, #5
	cmp r7, r3
	beq .L_0200cb94
	cmp r7, r3
	bgt .L_0200cb0a
	subs r3, #2
	cmp r7, r3
	beq .L_0200cb6c
	cmp r7, r3
	ble .L_0200cafe
	b .L_0200cc4a
.L_0200cafe:
	movs r1, #192
	lsls r1, r1, #2
	adds r1, #1
	cmp r7, r1
	beq .L_0200cb6c
	b .L_0200ccd2
.L_0200cb0a:
	movs r3, #192
	lsls r3, r3, #2
	adds r3, #9
	cmp r7, r3
	beq .L_0200cb94
	cmp r7, r3
	bgt .L_0200cb24
	movs r2, #194
	lsls r2, r2, #2
	cmp r7, r2
	bne .L_0200cb22
	b .L_0200cc4a
.L_0200cb22:
	b .L_0200ccd2
.L_0200cb24:
	movs r3, #192
	lsls r3, r3, #2
	adds r3, #10
	cmp r7, r3
	bne .L_0200cb30
	b .L_0200cc98
.L_0200cb30:
	movs r2, #131
	lsls r2, r2, #2
	movs r1, #0
	adds r2, #255
	mov r10, r1
	mov r8, r1
	mov r9, r1
	cmp r7, r2
	bne .L_0200cb44
	b .L_0200ccf6
.L_0200cb44:
	b .L_0200ccd2
.L_0200cb46:
	ldr r1, [sp, #12]
	movs r3, #1
	negs r3, r3
	add r10, r3
	lsls r3, r1, #16
	subs r6, r6, r3
	b .L_0200ccf6
.L_0200cb54:
	ldr r1, [sp, #12]
	movs r2, #1
	lsls r3, r1, #16
	add r10, r2
	adds r6, r6, r3
	b .L_0200ccf6
.L_0200cb60:
	ldr r1, [sp, #12]
	movs r2, #1
	lsls r3, r1, #16
	add r8, r2
	adds r5, r5, r3
	b .L_0200ccf6
.L_0200cb6c:
	ldr r1, [sp, #12]
	movs r2, #1
	negs r2, r2
	lsls r3, r1, #16
	add r8, r2
	subs r5, r5, r3
	b .L_0200ccf6
.L_0200cb7a:
	movs r2, #1
	add r8, r2
	mov r9, r2
	movs r2, #128
	subs r3, r6, r4
	lsls r2, r2, #12
	adds r6, r3, r2
	ldr r1, [sp, #8]
	subs r3, r5, r0
	adds r5, r3, r2
	movs r3, #128
	lsls r3, r3, #8
	b .L_0200cbec
.L_0200cb94:
	ldr r1, .L_0200cc94
	movs r2, #1
	negs r2, r2
	movs r3, #2
	add r10, r2
	mov r9, r3
	movs r2, #128
	adds r3, r6, r0
	adds r6, r3, r1
	lsls r2, r2, #12
	subs r3, r5, r4
	ldr r1, [sp, #8]
	adds r5, r3, r2
	movs r3, #192
	lsls r3, r3, #8
	b .L_0200cbec
.L_0200cbb4:
	movs r2, #1
	negs r2, r2
	add r8, r2
	ldr r1, [sp, #8]
	ldr r2, .L_0200cc94
	movs r3, #3
	mov r9, r3
	adds r3, r6, r4
	adds r6, r3, r2
	negs r1, r1
	adds r3, r5, r0
	adds r5, r3, r2
	str r1, [sp, #20]
	b .L_0200ccf6
.L_0200cbd0:
	movs r2, #1
	movs r3, #0
	add r10, r2
	movs r1, #128
	ldr r2, .L_0200cc94
	lsls r1, r1, #12
	mov r9, r3
	subs r3, r6, r0
	adds r6, r3, r1
	adds r3, r5, r4
	ldr r1, [sp, #8]
	adds r5, r3, r2
	movs r3, #128
	lsls r3, r3, #7
.L_0200cbec:
	subs r3, r3, r1
	str r3, [sp, #20]
	b .L_0200ccf6
.L_0200cbf2:
	movs r2, #1
	negs r2, r2
	movs r3, #3
	add r8, r2
	movs r1, #128
	ldr r2, .L_0200cc94
	lsls r1, r1, #12
	mov r9, r3
	subs r3, r6, r4
	adds r6, r3, r1
	adds r3, r5, r0
	adds r5, r3, r2
	ldr r3, [sp, #8]
	movs r1, #128
	b .L_0200cc62
.L_0200cc10:
	movs r2, #1
	negs r2, r2
	add r10, r2
	ldr r2, .L_0200cc94
	movs r3, #2
	mov r9, r3
	adds r3, r6, r0
	adds r6, r3, r2
	ldr r1, [sp, #8]
	adds r3, r5, r4
	adds r5, r3, r2
	movs r2, #128
	lsls r2, r2, #7
	adds r2, r1, r2
	str r2, [sp, #20]
	b .L_0200ccf6
.L_0200cc30:
	ldr r1, .L_0200cc94
	movs r3, #1
	add r8, r3
	mov r9, r3
	movs r2, #128
	adds r3, r6, r4
	adds r6, r3, r1
	lsls r2, r2, #12
	subs r3, r5, r0
	adds r5, r3, r2
	ldr r3, [sp, #8]
	str r3, [sp, #20]
	b .L_0200ccf6
.L_0200cc4a:
	movs r2, #0
	mov r9, r2
	movs r2, #128
	subs r3, r6, r0
	lsls r2, r2, #12
	adds r6, r3, r2
	subs r3, r5, r4
	movs r1, #1
	adds r5, r3, r2
	ldr r3, [sp, #8]
	add r10, r1
	movs r1, #192
.L_0200cc62:
	lsls r1, r1, #8
	adds r1, r3, r1
	str r1, [sp, #20]
	b .L_0200ccf6
.L_0200cc6a:
	movs r2, #0
	mov r10, r2
	mov r8, r2
	mov r9, r2
	b .L_0200ccf6
.L_0200cc74:
	movs r3, #0
	mov r10, r3
	mov r8, r3
	mov r9, r3
	b .L_0200ccf6
.L_0200cc7e:
	movs r1, #0
	mov r10, r1
	mov r8, r1
	mov r9, r1
	b .L_0200ccf6
.L_0200cc88:
	.4byte gSceneState
.L_0200cc8c:
	.4byte IwramMulQ16
.L_0200cc90:
	.4byte Data_02006f38
.L_0200cc94:
	.4byte 0xfff80000
.L_0200cc98:
	ldr r2, [sp, #4]
	ldrb r3, [r2]
	cmp r3, #1
	bne .L_0200ccf6
	add r2, sp, #20
	mov r3, r11
	ldrh r2, [r2]
	str r6, [r3, #8]
	mov r1, r11
	movs r3, #0
	str r3, [r1, #12]
	mov r3, r11
	str r5, [r1, #16]
	strh r2, [r3, #6]
	asrs r3, r6, #20
	cmp r3, #24
	bne .L_0200ccc8
	asrs r3, r5, #20
	cmp r3, #14
	bne .L_0200ccc8
	ldr r1, [sp, #4]
	movs r3, #7
	strb r3, [r1]
	b .L_0200ccf6
.L_0200ccc8:
	movs r0, #204
	bl Func_02005854
	ldr r1, .L_0200cd40
	b .L_0200ccea
.L_0200ccd2:
	ldr r1, [sp, #4]
	movs r3, #0
	mov r10, r3
	mov r8, r3
	mov r9, r3
	ldrb r3, [r1]
	cmp r3, #1
	bne .L_0200ccf6
	movs r0, #132
	bl Func_02005854
	ldr r1, .L_0200cd44
.L_0200ccea:
	mov r0, r11
	bl Func_02005634
	ldr r2, [sp, #4]
	movs r3, #2
	strb r3, [r2]
.L_0200ccf6:
	ldr r3, [sp, #36]
	cmp r3, #15
	bne .L_0200cd0e
	ldr r2, [sp, #32]
	mov r1, r10
	strh r1, [r2]
	ldr r1, [sp, #28]
	mov r3, r8
	strh r3, [r1]
	ldr r3, [sp, #24]
	mov r2, r9
	strb r2, [r3]
.L_0200cd0e:
	mov r3, r11
	adds r3, #99
	ldrb r3, [r3]
	cmp r3, #1
	bne .L_0200cd30
	mov r1, r11
	movs r3, #0
	add r2, sp, #20
	str r3, [r1, #12]
	str r6, [r1, #8]
	str r5, [r1, #16]
	ldrh r2, [r2]
	mov r3, r11
	strh r2, [r3, #6]
	ldr r3, [r3, #76]
	adds r3, #1
	str r3, [r1, #76]
.L_0200cd30:
	add sp, #40
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200cd40:
	.4byte Data_02006f38
.L_0200cd44:
	.4byte Data_02006ec0
	.section .text.x0200cd48,"ax",%progbits
	.global Func_02004d48
	.thumb_func
Func_02004d48:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	mov r11, r0
	mov r2, r11
	sub sp, #8
	adds r2, #1
	str r1, [sp, #4]
	str r2, [sp, #0]
	bl Object_GetById
	movs r2, #0
	adds r7, r0, #0
	mov r9, r2
	adds r3, r7, #0
	adds r3, #100
	mov r2, r9
	strh r2, [r3]
	movs r2, #7
	mov r10, r2
	mov r2, r10
	adds r3, #2
	strh r2, [r3]
	movs r2, #1
	mov r8, r2
	subs r3, #4
	movs r5, #2
	mov r2, r8
	strb r5, [r3]
	adds r3, #1
	strb r2, [r3]
	mov r3, r9
	str r3, [r7, #76]
	bl Func_02005654
	movs r1, #2
	adds r0, r7, #0
	bl Func_02005624
	ldr r0, [sp, #0]
	ldr r6, [sp, #0]
	bl Object_GetById
	adds r7, r0, #0
	adds r3, r7, #0
	adds r3, #100
	strh r5, [r3]
	mov r2, r10
	adds r3, #2
	strh r2, [r3]
	subs r3, #4
	movs r2, #0
	strb r2, [r3]
	mov r2, r8
	adds r3, #1
	strb r2, [r3]
	mov r3, r9
	str r3, [r7, #76]
	bl Func_02005654
	adds r6, #1
	movs r1, #2
	adds r0, r7, #0
	bl Func_02005624
	adds r0, r6, #0
	bl Object_GetById
	adds r7, r0, #0
	adds r2, r7, #0
	adds r2, #100
	movs r3, #8
	strh r3, [r2]
	adds r3, r7, #0
	mov r2, r10
	adds r3, #102
	strh r2, [r3]
	subs r3, #4
	movs r2, #2
	strb r2, [r3]
	mov r2, r8
	adds r3, #1
	strb r2, [r3]
	mov r3, r9
	str r3, [r7, #76]
	bl Func_02005654
	adds r6, #1
	movs r1, #2
	adds r0, r7, #0
	bl Func_02005624
	adds r0, r6, #0
	bl Object_GetById
	adds r7, r0, #0
	adds r2, r7, #0
	adds r2, #100
	movs r3, #10
	strh r3, [r2]
	adds r3, r7, #0
	mov r2, r10
	adds r3, #102
	strh r2, [r3]
	subs r3, #4
	movs r2, #0
	strb r2, [r3]
	mov r2, r8
	adds r3, #1
	strb r2, [r3]
	mov r3, r9
	str r3, [r7, #76]
	bl Func_02005654
	adds r0, r7, #0
	movs r1, #2
	bl Func_02005624
	movs r2, #0
	mov r10, r2
.L_0200ce40:
	movs r3, #1
	mov r9, r3
	movs r5, #0
.L_0200ce46:
	movs r2, #4
	add r2, r11
	mov r8, r2
	mov r6, r11
	cmp r11, r8
	bge .L_0200ce7c
.L_0200ce52:
	adds r0, r6, #0
	bl Object_GetById
	adds r7, r0, #0
	adds r3, r7, #0
	adds r3, #99
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_0200ce6c
	cmp r3, #7
	beq .L_0200ce6c
	movs r3, #0
	mov r9, r3
.L_0200ce6c:
	adds r2, r6, #0
	adds r0, r7, #0
	adds r1, r5, #0
	adds r6, #1
	bl Func_02004938
	cmp r6, r8
	blt .L_0200ce52
.L_0200ce7c:
	movs r0, #1
	adds r5, #1
	bl WaitFrames
	cmp r5, #15
	ble .L_0200ce46
	mov r2, r9
	cmp r2, #0
	bne .L_0200ce98
	movs r3, #1
	add r10, r3
	mov r2, r10
	cmp r2, #127
	ble .L_0200ce40
.L_0200ce98:
	movs r0, #20
	bl WaitFrames
	ldr r3, [sp, #4]
	movs r0, #3
	cmp r3, #0
	beq .L_0200cf32
	mov r0, r11
	bl Object_GetById
	ldr r3, [r0, #8]
	movs r6, #0
	movs r5, #0
	cmp r3, #0
	beq .L_0200ceb8
	ldr r6, [r0, #76]
.L_0200ceb8:
	ldr r0, [sp, #0]
	bl Object_GetById
	ldr r3, [r0, #8]
	cmp r3, #0
	beq .L_0200ced0
	ldr r0, [r0, #76]
	cmp r6, #0
	beq .L_0200cece
	cmp r6, r0
	bls .L_0200ced0
.L_0200cece:
	adds r6, r0, #0
.L_0200ced0:
	mov r0, r11
	adds r0, #2
	bl Object_GetById
	ldr r3, [r0, #8]
	cmp r3, #0
	beq .L_0200cee0
	ldr r5, [r0, #76]
.L_0200cee0:
	mov r0, r11
	adds r0, #3
	bl Object_GetById
	ldr r3, [r0, #8]
	cmp r3, #0
	beq .L_0200cefa
	ldr r0, [r0, #76]
	cmp r5, #0
	beq .L_0200cef8
	cmp r5, r0
	bls .L_0200cefa
.L_0200cef8:
	adds r5, r0, #0
.L_0200cefa:
	cmp r6, #0
	beq .L_0200cf2a
	movs r0, #1
	cmp r5, #0
	beq .L_0200cf32
	movs r0, #0
	cmp r6, r5
	beq .L_0200cf32
	cmp r6, r5
	bcs .L_0200cf1c
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #235
	bl Func_0200560c
	movs r0, #1
	b .L_0200cf32
.L_0200cf1c:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #235
	bl Func_0200560c
	movs r0, #2
	b .L_0200cf32
.L_0200cf2a:
	movs r0, #2
	cmp r5, #0
	bne .L_0200cf32
	movs r0, #3
.L_0200cf32:
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.section .text.x0200cf40,"ax",%progbits
	.global Func_02004f40
	.thumb_func
Func_02004f40:
	push {r5, lr}
	ldr r5, .L_0200cf80
	movs r4, #0
.L_0200cf46:
	movs r2, #0
.L_0200cf48:
	movs r3, #11
	muls r3, r4
	adds r3, r3, r2
	lsls r3, r3, #2
	ldrb r3, [r5, r3]
	cmp r3, #10
	bne .L_0200cf70
	cmp r2, #5
	bne .L_0200cf5e
	cmp r4, #3
	beq .L_0200cf70
.L_0200cf5e:
	cmp r4, #7
	bne .L_0200cf6a
	cmp r2, #1
	beq .L_0200cf70
	cmp r2, #9
	beq .L_0200cf70
.L_0200cf6a:
	str r2, [r0]
	str r4, [r1]
	b .L_0200cf7c
.L_0200cf70:
	adds r2, #1
	cmp r2, #10
	bls .L_0200cf48
	adds r4, #1
	cmp r4, #7
	bls .L_0200cf46
.L_0200cf7c:
	pop {r5, pc}
	.2byte 0x0000
.L_0200cf80:
	.4byte gSceneState
	.section .text.x0200cf84,"ax",%progbits
	.global Func_02004f84
	.thumb_func
Func_02004f84:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r7, r0, #0
	adds r6, r1, #0
	cmp r7, #0
	bge .L_0200cf94
	movs r7, #0
.L_0200cf94:
	movs r5, #140
	lsls r5, r5, #8
	adds r5, #160
	adds r1, r5, #0
	adds r0, r7, #0
	bl Engine_MathDivide
	adds r0, #48
	strb r0, [r6]
	adds r1, r5, #0
	adds r0, r7, #0
	bl Engine_MathRemainder
	movs r5, #225
	lsls r5, r5, #4
	adds r1, r5, #0
	adds r7, r0, #0
	bl Engine_MathDivide
	adds r6, #1
	adds r0, #48
	strb r0, [r6]
	adds r1, r5, #0
	adds r0, r7, #0
	bl Engine_MathRemainder
	movs r5, #150
	movs r3, #58
	lsls r5, r5, #2
	adds r6, #1
	strb r3, [r6]
	adds r1, r5, #0
	mov r8, r3
	adds r7, r0, #0
	bl Engine_MathDivide
	adds r6, #1
	adds r0, #48
	strb r0, [r6]
	adds r1, r5, #0
	adds r0, r7, #0
	bl Engine_MathRemainder
	movs r1, #60
	adds r7, r0, #0
	bl Engine_MathDivide
	adds r6, #1
	adds r0, #48
	strb r0, [r6]
	movs r1, #60
	adds r0, r7, #0
	bl Engine_MathRemainder
	adds r6, #1
	mov r3, r8
	strb r3, [r6]
	movs r1, #6
	adds r7, r0, #0
	bl Engine_MathDivide
	adds r6, #1
	adds r0, #48
	movs r3, #0
	strb r0, [r6]
	strb r3, [r6, #1]
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x0200d020,"ax",%progbits
	.global Func_02005020
	.thumb_func
Func_02005020:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r3, .L_0200d088
	movs r1, #60
	mov r8, r3
	bl Engine_MathRemainder
	movs r7, #128
	mov r6, r8
	adds r5, r0, #0
	lsls r7, r7, #2
	adds r6, #136
	cmp r5, #0
	beq .L_0200d044
	movs r3, #1
	strb r3, [r6]
	b .L_0200d05c
.L_0200d044:
	ldrb r3, [r6]
	cmp r3, #0
	beq .L_0200d05c
	mov r3, r8
	adds r3, #132
	ldr r3, [r3]
	cmp r3, #0
	beq .L_0200d05a
	movs r0, #236
	bl Func_02005854
.L_0200d05a:
	strb r5, [r6]
.L_0200d05c:
	cmp r5, #19
	bgt .L_0200d07a
	movs r1, #10
	lsls r0, r5, #15
	bl Engine_MathDivide
	bl Math_Cosine
	adds r1, r0, #0
	movs r0, #128
	ldr r3, .L_0200d08c
	lsls r0, r0, #2
	mov lr, r3
	.2byte 0xf800
	adds r7, r0, #0
.L_0200d07a:
	cmp r7, #16
	bgt .L_0200d080
	movs r7, #16
.L_0200d080:
	adds r0, r7, #0
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_0200d088:
	.4byte Data_020023c4 + 0x288
.L_0200d08c:
	.4byte IwramMulQ16
	.section .text.x0200d090,"ax",%progbits
	.global Func_02005090
	.thumb_func
Func_02005090:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r5, .L_0200d1f0
	movs r0, #24
	adds r0, r0, r5
	mov r9, r0
	ldr r0, .L_0200d1f4
	ldr r2, .L_0200d1f8
	ldr r3, [r0]
	sub sp, #20
	lsls r3, r3, #2
	adds r3, r3, r2
	ldrh r3, [r3, #2]
	ldr r1, [r5]
	lsrs r3, r3, #5
	ldr r7, [r5, #4]
	str r3, [sp, #8]
	movs r2, #132
	adds r2, r2, r5
	ldr r0, [r2]
	mov r10, r1
	movs r1, #32
	adds r1, r1, r5
	mov r11, r2
	adds r0, #50
	mov r8, r1
	mov r1, r9
	bl Func_02004f84
	mov r3, r11
	ldr r0, [r3]
	ldr r1, .L_0200d1fc
	adds r0, #60
	bl Func_02004f84
	mov r1, r11
	ldr r0, [r1]
	ldr r1, .L_0200d200
	bl Func_02004f84
	movs r6, #4
.L_0200d0ec:
	mov r2, r9
	ldrb r1, [r2]
	ldr r3, .L_0200d204
	lsls r1, r1, #6
	add r1, r10
	adds r1, r1, r3
	adds r0, r7, #0
	ldr r3, .L_0200d208
	movs r2, #64
	mov lr, r3
	.2byte 0xf800
	subs r6, #1
	movs r0, #1
	adds r7, #64
	add r9, r0
	cmp r6, #0
	bge .L_0200d0ec
	ldr r1, .L_0200d1f4
	ldr r2, [r5, #4]
	ldr r0, [r1]
	movs r1, #160
	lsls r1, r1, #1
	bl VramBlock_LoadCached
	add r5, sp, #12
	ldr r3, [r5, #4]
	ldr r2, .L_0200d20c
	adds r0, r5, #0
	ands r3, r2
	str r3, [r5, #4]
	ldr r3, [sp, #12]
	movs r6, #0
	ands r3, r2
	movs r2, #128
	lsls r2, r2, #2
	orrs r3, r2
	movs r2, #255
	lsls r2, r2, #8
	adds r2, #255
	ands r3, r2
	movs r2, #128
	lsls r2, r2, #18
	orrs r3, r2
	str r3, [sp, #12]
	bl Func_020055e4
	str r0, [sp, #4]
	mov r2, r11
	ldr r0, [r2]
	bl Func_02005020
	movs r3, #0
	strh r3, [r5, #4]
	strh r0, [r5]
	strh r0, [r5, #2]
	adds r0, r5, #0
	bl Func_020055e4
	ldr r3, .L_0200d1fc
	str r0, [sp, #0]
	ldr r7, .L_0200d200
	mov r10, r3
.L_0200d168:
	mov r1, r8
	movs r3, #0
	stmia r1!, {r3}
	movs r2, #128
	lsls r2, r2, #8
	lsls r3, r6, #20
	orrs r3, r2
	stmia r1!, {r3}
	ldr r0, [sp, #8]
	movs r3, #128
	lsls r3, r3, #3
	orrs r3, r0
	str r3, [r1]
	mov r1, r8
	ldrb r3, [r1, #5]
	movs r2, #3
	orrs r3, r2
	strb r3, [r1, #5]
	mov r0, r10
	ldrb r3, [r0]
	ldrb r2, [r7]
	mov r12, r3
	ldr r5, [sp, #8]
	adds r3, r6, #1
	mov r11, r3
	movs r3, #12
	movs r1, #1
	mov r0, r8
	add r3, r8
	add r10, r1
	adds r7, #1
	ldrb r1, [r0, #7]
	adds r5, #2
	mov r9, r3
	cmp r12, r2
	bne .L_0200d1b4
	cmp r6, #2
	bne .L_0200d1b8
.L_0200d1b4:
	ldr r2, [sp, #0]
	b .L_0200d1ba
.L_0200d1b8:
	ldr r2, [sp, #4]
.L_0200d1ba:
	movs r0, #63
	movs r3, #31
	negs r0, r0
	ands r2, r3
	adds r3, r0, #0
	ands r3, r1
	lsls r2, r2, #1
	orrs r3, r2
	mov r1, r8
	strb r3, [r1, #7]
	mov r0, r8
	movs r1, #236
	bl Func_020055ec
	mov r6, r11
	str r5, [sp, #8]
	mov r8, r9
	cmp r6, #4
	ble .L_0200d168
	add sp, #20
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200d1f0:
	.4byte Data_020023c4 + 0x288
.L_0200d1f4:
	.4byte Data_020023c4 + 0x308
.L_0200d1f8:
	.4byte ResourceTableEntries
.L_0200d1fc:
	.4byte Data_020023c4 + 0x290
.L_0200d200:
	.4byte Data_020023c4 + 0x298
.L_0200d204:
	.4byte 0xfffff400
.L_0200d208:
	.4byte IwramCopyWords
.L_0200d20c:
	.4byte 0xffff0000
	.section .text.x0200d210,"ax",%progbits
	.global Func_02005210
	.thumb_func
Func_02005210:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r0, #179
	lsls r0, r0, #1
	adds r3, r2, r0
	movs r0, #0
	ldrsh r3, [r3, r0]
	ldr r1, .L_0200d258
	cmp r3, #0
	bne .L_0200d252
	movs r0, #173
	lsls r0, r0, #1
	adds r3, r2, r0
	movs r0, #0
	ldrsh r3, [r3, r0]
	cmp r3, #0
	bne .L_0200d252
	movs r0, #175
	lsls r0, r0, #1
	adds r3, r2, r0
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	bne .L_0200d252
	adds r2, r1, #0
	adds r2, #132
	ldr r3, [r2]
	cmp r3, #0
	ble .L_0200d252
	subs r3, #1
	str r3, [r2]
.L_0200d252:
	bl Func_02005090
	pop {pc}
.L_0200d258:
	.4byte Data_020023c4 + 0x288
	.section .text.x0200d25c,"ax",%progbits
	.global Func_0200525c
	.thumb_func
Func_0200525c:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r0, #179
	lsls r0, r0, #1
	adds r3, r2, r0
	movs r0, #0
	ldrsh r3, [r3, r0]
	ldr r1, .L_0200d2a0
	cmp r3, #0
	bne .L_0200d29e
	movs r0, #173
	lsls r0, r0, #1
	adds r3, r2, r0
	movs r0, #0
	ldrsh r3, [r3, r0]
	cmp r3, #0
	bne .L_0200d29e
	movs r0, #175
	lsls r0, r0, #1
	adds r3, r2, r0
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	bne .L_0200d29e
	adds r2, r1, #0
	adds r2, #132
	ldr r3, [r2]
	cmp r3, #0
	ble .L_0200d29e
	subs r3, #1
	str r3, [r2]
.L_0200d29e:
	pop {pc}
.L_0200d2a0:
	.4byte Data_020023c4 + 0x288
	.section .text.x0200d2a4,"ax",%progbits
	.global Func_020052a4
	.thumb_func
Func_020052a4:
	push {r5, r6, r7, lr}
	adds r7, r0, #0
	movs r0, #10
	adds r0, #255
	ldr r6, .L_0200d328
	bl Func_02005604
	cmp r0, #0
	bne .L_0200d2d0
	ldr r3, .L_0200d32c
	adds r0, r6, #0
	movs r1, #140
	mov lr, r3
	.2byte 0xf800
	adds r2, r6, #0
	movs r3, #225
	adds r2, #132
	lsls r3, r3, #4
	str r3, [r2]
	adds r2, #4
	movs r3, #1
	strb r3, [r2]
.L_0200d2d0:
	movs r0, #128
	lsls r0, r0, #3
	bl Func_020055bc
	adds r5, r0, #0
	ldr r0, .L_0200d330
	bl Resource_GetTableEntry
	adds r1, r5, #0
	bl Func_020055cc
	movs r3, #176
	lsls r3, r3, #2
	str r5, [r6]
	adds r5, r5, r3
	str r5, [r6, #4]
	bl Resource_FindFreeEntry
	adds r3, r6, #0
	adds r3, #128
	movs r1, #160
	str r0, [r3]
	lsls r1, r1, #1
	movs r2, #0
	bl VramBlock_LoadCached
	adds r3, r6, #0
	adds r3, #137
	strb r7, [r3]
	cmp r7, #0
	beq .L_0200d31a
	movs r1, #144
	ldr r0, .L_0200d334
	lsls r1, r1, #3
	bl Func_0200557c
	b .L_0200d324
.L_0200d31a:
	movs r1, #144
	ldr r0, .L_0200d338
	lsls r1, r1, #3
	bl Func_0200557c
.L_0200d324:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200d328:
	.4byte Data_020023c4 + 0x288
.L_0200d32c:
	.4byte IwramClearWords
.L_0200d330:
	.4byte 0x00000015
.L_0200d334:
	.4byte Func_02005210
.L_0200d338:
	.4byte Func_0200525c
	.section .text.x0200d33c,"ax",%progbits
	.global Func_0200533c
	.thumb_func
Func_0200533c:
	push {lr}
	ldr r3, .L_0200d358
	adds r3, #137
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_0200d350
	ldr r0, .L_0200d35c
	bl Scheduler_RemoveCallbackFar
	b .L_0200d356
.L_0200d350:
	ldr r0, .L_0200d360
	bl Scheduler_RemoveCallbackFar
.L_0200d356:
	pop {pc}
.L_0200d358:
	.4byte Data_020023c4 + 0x288
.L_0200d35c:
	.4byte Func_02005210
.L_0200d360:
	.4byte Func_0200525c
	.section .text.x0200d364,"ax",%progbits
	.global Func_02005364
	.thumb_func
Func_02005364:
	push {r5, lr}
	ldr r5, .L_0200d394
	adds r3, r5, #0
	adds r3, #137
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_0200d37e
	movs r1, #144
	ldr r0, .L_0200d398
	lsls r1, r1, #3
	bl Func_0200557c
	b .L_0200d388
.L_0200d37e:
	movs r1, #144
	ldr r0, .L_0200d39c
	lsls r1, r1, #3
	bl Func_0200557c
.L_0200d388:
	adds r2, r5, #0
	movs r3, #225
	adds r2, #132
	lsls r3, r3, #4
	str r3, [r2]
	pop {r5, pc}
.L_0200d394:
	.4byte Data_020023c4 + 0x288
.L_0200d398:
	.4byte Func_02005210
.L_0200d39c:
	.4byte Func_0200525c
	.section .text.x0200d3a0,"ax",%progbits
	.global Func_020053a0
	.thumb_func
Func_020053a0:
	ldr r3, .L_0200d3a8
	adds r3, #132
	ldr r0, [r3]
	bx lr
.L_0200d3a8:
	.4byte Data_020023c4 + 0x288
	.section .text.x0200d3ac,"ax",%progbits
	.global Func_020053ac
	.thumb_func
Func_020053ac:
	ldr r3, .L_0200d3b8
	movs r2, #225
	adds r3, #132
	lsls r2, r2, #4
	str r2, [r3]
	bx lr
.L_0200d3b8:
	.4byte Data_020023c4 + 0x288
	.section .text.x0200d3bc,"ax",%progbits
	.global Func_020053bc
	.thumb_func
Func_020053bc:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r2, #255
	ldrh r3, [r0]
	lsls r2, r2, #8
	adds r2, #255
	sub sp, #4
	cmp r3, r2
	beq .L_0200d420
	adds r7, r0, #0
.L_0200d3d2:
	ldrh r3, [r7]
	mov r8, r3
	mov r0, r8
	bl Object_GetById
	adds r5, r0, #0
	adds r1, r5, #0
	adds r1, #89
	movs r2, #2
	ldrsh r6, [r7, r2]
	ldrb r2, [r1]
	movs r3, #4
	ldrsh r4, [r7, r3]
	movs r3, #254
	ands r3, r2
	strb r3, [r1]
	movs r1, #0
	str r4, [sp, #0]
	bl ObjectDispatch_SetSingleChildField26
	ldr r4, [sp, #0]
	lsls r6, r6, #16
	lsls r4, r4, #16
	lsrs r4, r4, #16
	lsrs r6, r6, #16
	adds r5, #34
	ldrb r3, [r5]
	adds r2, r4, #0
	mov r0, r8
	adds r1, r6, #0
	adds r7, #6
	bl Func_020054ac
	movs r2, #255
	ldrh r3, [r7]
	lsls r2, r2, #8
	adds r2, #255
	cmp r3, r2
	bne .L_0200d3d2
.L_0200d420:
	add sp, #4
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.section .text.x0200d428,"ax",%progbits
	.global Func_02005428
	.thumb_func
Func_02005428:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #224
	adds r5, r0, #0
	mov r9, r1
	mov r10, r2
	movs r1, #255
	ldr r2, [r3]
	b .L_0200d494
.L_0200d444:
	ldrh r3, [r5]
	movs r1, #26
	ldrsh r7, [r2, r1]
	cmp r7, r3
	bne .L_0200d490
	adds r0, r7, #0
	bl Object_GetById
	adds r5, #2
	ldrh r2, [r5]
	mov r3, r10
	adds r6, r0, #0
	mov r8, r2
	ldrh r5, [r5, #2]
	cmp r3, #7
	bgt .L_0200d46c
	ldr r3, [r6, #28]
	ldr r1, .L_0200d4a8
	adds r3, r3, r1
	str r3, [r6, #28]
.L_0200d46c:
	mov r2, r9
	cmp r2, #1
	bne .L_0200d49e
	adds r0, r5, #0
	bl Func_0200560c
	adds r3, r6, #0
	adds r3, #34
	ldrb r3, [r3]
	adds r0, r7, #0
	mov r1, r8
	adds r2, r5, #0
	bl Func_020054ac
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r6, #28]
	b .L_0200d49e
.L_0200d490:
	adds r5, #6
	movs r1, #255
.L_0200d494:
	ldrh r3, [r5]
	lsls r1, r1, #8
	adds r1, #255
	cmp r3, r1
	bne .L_0200d444
.L_0200d49e:
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_0200d4a8:
	.4byte 0xffffe100
	.section .text.x0200d4ac,"ax",%progbits
	.global Func_020054ac
	.thumb_func
Func_020054ac:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r5, r3, #0
	mov r8, r2
	adds r6, r1, #0
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
	ldr r5, [r2, r3]
	adds r7, r0, #0
	bl Func_02005844
	lsls r0, r0, #2
	adds r5, r5, r0
	mov r0, r8
	bl Func_02005604
	cmp r0, #0
	beq .L_0200d520
	adds r1, r7, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #0
	strb r3, [r5, #2]
	movs r3, #2
	orrs r3, r2
	strb r3, [r1]
	cmp r6, #1
	beq .L_0200d50c
	cmp r6, #1
	bcc .L_0200d502
	cmp r6, #2
	beq .L_0200d516
	b .L_0200d54e
.L_0200d502:
	adds r0, r7, #0
	movs r1, #2
	bl Func_02005624
	b .L_0200d54e
.L_0200d50c:
	adds r0, r7, #0
	movs r1, #4
	bl Func_02005624
	b .L_0200d54e
.L_0200d516:
	adds r0, r7, #0
	movs r1, #6
	bl Func_02005624
	b .L_0200d54e
.L_0200d520:
	movs r3, #255
	strb r3, [r5, #2]
	cmp r6, #1
	beq .L_0200d53c
	cmp r6, #1
	bcc .L_0200d532
	cmp r6, #2
	beq .L_0200d546
	b .L_0200d54e
.L_0200d532:
	adds r0, r7, #0
	movs r1, #1
	bl Func_02005624
	b .L_0200d54e
.L_0200d53c:
	adds r0, r7, #0
	movs r1, #3
	bl Func_02005624
	b .L_0200d54e
.L_0200d546:
	adds r0, r7, #0
	movs r1, #5
	bl Func_02005624
.L_0200d54e:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.section .rodata.x0200d85c,"a",%progbits
.L_0200d85c:
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
.L_0200d898:
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
.L_0200d8d4:
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
	.global Data_02005910
Data_02005910:
	.4byte 0x00020007
	.4byte 0x00000200
	.4byte 0x00020003
	.4byte 0x00020004
	.4byte 0x00000204
	.4byte 0x00000203
	.4byte 0x00020101
	.4byte 0x00010201
	.4byte 0x00020102
	.4byte 0x00010202
	.4byte 0x0000f800
	.4byte 0x00000000
	.global Data_02005940
Data_02005940:
	.4byte 0x00020103
	.global Data_02005944
Data_02005944:
	.4byte 0x00000026
	.global Data_02005948
Data_02005948:
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
	.4byte 0x00000002
	.4byte 0x01020200
	.4byte 0x07030301
	.4byte 0x05060405
	.4byte 0x09040607
	.4byte 0x0a080809
	.global Data_02005ac0
Data_02005ac0:
	.4byte 0x0000010a
	.4byte 0x0000ff01
	.global Data_02005ac8
Data_02005ac8:
	.4byte 0x050a00ff
	.4byte 0x0c06030f
	.4byte 0x0f0f0f09
	.global Data_02005ad4
Data_02005ad4:
	.4byte 0x060a0502
	.4byte 0x0a060107
	.4byte 0x0702040a
	.4byte 0x01050a04
	.global Data_02005ae4
Data_02005ae4:
	.4byte .L_0200d85c
	.4byte .L_0200d898
	.4byte .L_0200d8d4
	.global Data_02005af0
Data_02005af0:
	.4byte 0x00000000
	.global Data_02005af4
Data_02005af4:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01d80000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000011
	.global Data_02005b1c
Data_02005b1c:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x00b00000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.global Data_02005b60
Data_02005b60:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01c80000
	.4byte 0x00000000
	.4byte 0x00b00000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.global Data_02005ba4
Data_02005ba4:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01d00000
	.4byte 0x00000000
	.4byte 0x00d00000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00005000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.global Data_02005be8
Data_02005be8:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01b00000
	.4byte 0x00000000
	.4byte 0x011e0000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01b00000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00003000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.global Data_02005c40
Data_02005c40:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x015c0000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.global Data_02005c74
Data_02005c74:
	.4byte 0x00000016
	.4byte 0x00000023
	.4byte 0x0000c000
	.4byte 0x00000016
	.4byte 0x00000024
	.4byte 0x0000e000
	.4byte 0x00000016
	.4byte 0x00000028
	.4byte 0x00000000
	.4byte 0x0000002e
	.4byte Func_0200028c
	.global Data_02005ca0
Data_02005ca0:
	.4byte 0x00000016
	.4byte 0x00000023
	.4byte 0x0000c000
	.4byte 0x00000016
	.4byte 0x00000024
	.4byte 0x0000a000
	.4byte 0x00000016
	.4byte 0x00000028
	.4byte 0x00000000
	.4byte 0x0000002e
	.4byte Func_0200028c
	.global Data_02005ccc
Data_02005ccc:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01d80000
	.4byte 0x00000000
	.4byte 0x01340000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00004000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.global Data_02005d24
Data_02005d24:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01d80000
	.4byte 0x00000000
	.4byte 0x01340000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01cc0000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00005000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.global Data_02005d7c
Data_02005d7c:
	.4byte 0x0002000c
	.4byte 0x000d08ce
	.4byte Field_Map155 + 0x786
	.4byte 0x0000ffff
	.global Data_02005d8c
Data_02005d8c:
	.4byte 0x00000024
	.4byte 0x0000007a
	.4byte 0x000000c9
	.4byte 0x00000138
	.4byte 0x000001af
	.4byte 0x000001f8
	.4byte 0x0000026b
	.4byte 0x000002c8
	.4byte 0x00000337
	.4byte 0x0000038c
	.4byte 0x0000041e
	.4byte 0x00000461
	.4byte 0x000004b7
	.4byte 0x0000051f
	.4byte 0x00000579
	.4byte 0x00000613
	.global Data_02005dcc
Data_02005dcc:
	.4byte 0xffff0000
	.4byte 0x00000188
	.4byte 0x40000098
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02005dfc
Data_02005dfc:
	.4byte 0x00100310
	.4byte 0x03200050
	.4byte 0x00600020
	.4byte 0x0001ffff
	.4byte 0x00300050
	.4byte 0x00600170
	.4byte 0x01800040
	.4byte 0x0002ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02005e2c
Data_02005e2c:
	.4byte 0x001001d0
	.4byte 0x01e000f0
	.4byte 0x01000020
	.4byte 0x0002ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02005e4c
Data_02005e4c:
	.4byte 0x00100210
	.4byte 0x02200040
	.4byte 0x00500020
	.4byte 0x0002ffff
	.4byte 0x00100060
	.4byte 0x007000a0
	.4byte 0x00b00020
	.4byte 0x0003ffff
	.4byte 0x001002a0
	.4byte 0x02b000a0
	.4byte 0x00b00020
	.4byte 0x0004ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02005e8c
Data_02005e8c:
	.4byte 0x00100180
	.4byte 0x01900070
	.4byte 0x00800020
	.4byte 0x0004ffff
	.4byte 0x00100060
	.4byte 0x00700090
	.4byte 0x00a00020
	.4byte 0x0005ffff
	.4byte 0x001002a0
	.4byte 0x02b00090
	.4byte 0x00a00020
	.4byte 0x0006ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02005ecc
Data_02005ecc:
	.4byte 0x001001b0
	.4byte 0x01c001c0
	.4byte 0x01d00020
	.4byte 0x0004ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02005eec
Data_02005eec:
	.4byte 0x00100130
	.4byte 0x01400090
	.4byte 0x00a00020
	.4byte 0x0002ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02005f0c
Data_02005f0c:
	.4byte 0x0000007b
	.4byte 0x1010306f
	.4byte 0x0000086b
	.4byte 0x10103072
	.4byte 0xffffffff
	.4byte 0x10201073
	.4byte 0xffffffff
	.4byte 0x00000073
	.4byte 0x0010207b
	.4byte 0x00201074
	.4byte 0x00000074
	.4byte 0x00102073
	.4byte 0x00201075
	.4byte 0x00302075
	.4byte 0x00403075
	.4byte 0x00000075
	.4byte 0x00102074
	.4byte 0x00203074
	.4byte 0x00304074
	.4byte 0x00401076
	.4byte 0x00502076
	.4byte 0x00603076
	.4byte 0x00000076
	.4byte 0x00104075
	.4byte 0x00205075
	.4byte 0x00306075
	.4byte 0x00401077
	.4byte 0x00507075
	.4byte 0x00608075
	.4byte 0x00000077
	.4byte 0x00104076
	.4byte 0x00201078
	.4byte 0x00000079
	.4byte 0x00102078
	.4byte 0x00a0a07d
	.4byte 0x00b0b07d
	.4byte 0x00c0b072
	.4byte 0x00d0c072
	.4byte 0x00e0d072
	.4byte 0x00f0e072
	.4byte 0x0100f072
	.4byte 0x0110a07d
	.4byte 0x01206078
	.4byte 0x000001ff
.L_0200dfbc:
	.4byte 0x0000002e
	.4byte Func_02000364
	.4byte 0x00000011
.L_0200dfc8:
	.4byte 0x0000002e
	.4byte Func_02000408
	.4byte 0x00000011
	.global Data_02005fd4
Data_02005fd4:
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x01d80000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02006004
Data_02006004:
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00020000
	.4byte 0xffff0197
	.4byte .L_0200dfc8
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00020000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x02180000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00020000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00020000
	.4byte 0xffff0196
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x01e80000
	.4byte 0x00020000
	.4byte 0xffff0196
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x01e80000
	.4byte 0x00020000
	.4byte 0xffff0166
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x00480000
	.4byte 0x00020000
	.4byte 0xffff0166
	.4byte 0x00000001
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x00480000
	.4byte 0x01028000
	.4byte 0xffff0166
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x01020000
	.4byte 0xffff0166
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x01020000
	.4byte 0xffff0166
	.4byte 0x00000001
	.4byte 0x02080000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x01020000
	.4byte 0xffff0166
	.4byte 0x00000001
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x02080000
	.4byte 0x01020000
	.4byte 0xffff0166
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x02280000
	.4byte 0x01020000
	.4byte 0xffff0165
	.4byte 0x00000001
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00028000
	.4byte 0xffff0165
	.4byte 0x00000001
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x01028000
	.4byte 0xffff0165
	.4byte 0x00000001
	.4byte 0x02080000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x01028000
	.4byte 0xffff0165
	.4byte 0x00000001
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x02280000
	.4byte 0x01028000
	.4byte 0xffff0165
	.4byte 0x00000001
	.4byte 0x01c80000
	.4byte 0x00000000
	.4byte 0x01e80000
	.4byte 0x01028000
	.4byte 0xffff0165
	.4byte .L_0200dfbc
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x01e80000
	.4byte 0x00020000
	.4byte 0xffff016a
	.4byte 0x00000001
	.4byte 0x01600000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00028000
	.4byte 0xffff016a
	.4byte 0x00000001
	.4byte 0x01900000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x01020000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00024000
	.4byte 0xffff0151
	.4byte 0x00000001
	.4byte 0x02a80000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x01024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02006244
Data_02006244:
	.4byte 0xffff0166
	.4byte 0x00000001
	.4byte 0x00380000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00020000
	.4byte 0xffff0166
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x01028000
	.4byte 0xffff0166
	.4byte 0x00000001
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x01020000
	.4byte 0xffff0166
	.4byte 0x00000001
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x01020000
	.4byte 0xffff0166
	.4byte 0x00000001
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x01020000
	.4byte 0xffff0166
	.4byte 0x00000001
	.4byte 0x02780000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x01020000
	.4byte 0xffff0166
	.4byte 0x00000001
	.4byte 0x02d80000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x01028000
	.4byte 0xffff0165
	.4byte 0x00000001
	.4byte 0x00380000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00020000
	.4byte 0xffff0165
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x01020000
	.4byte 0xffff0165
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x01028000
	.4byte 0xffff0165
	.4byte 0x00000001
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x01020000
	.4byte 0xffff0165
	.4byte 0x00000001
	.4byte 0x02c80000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x01020000
	.4byte 0xffff0167
	.4byte 0x00000001
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00020000
	.4byte 0xffff0167
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x01028000
	.4byte 0xffff0167
	.4byte 0x00000001
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x01020000
	.4byte 0xffff0167
	.4byte 0x00000001
	.4byte 0x02980000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x01020000
	.4byte 0xffff0167
	.4byte 0x00000001
	.4byte 0x02980000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x01020000
	.4byte 0xffff0168
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00020000
	.4byte 0xffff0168
	.4byte 0x00000001
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x01020000
	.4byte 0xffff0168
	.4byte 0x00000001
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x01020000
	.4byte 0xffff0168
	.4byte 0x00000001
	.4byte 0x02780000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x01020000
	.4byte 0xffff0169
	.4byte 0x00000001
	.4byte 0x01a80000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00020000
	.4byte 0xffff0169
	.4byte 0x00000001
	.4byte 0x01d80000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x01020000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x01a80000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x01024000
	.4byte 0x003900f3
	.4byte 0x00000001
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020064cc
Data_020064cc:
	.4byte 0xffff0166
	.4byte 0x00000001
	.4byte 0x00380000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00020000
	.4byte 0xffff0166
	.4byte 0x00000001
	.4byte 0x00280000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x01028000
	.4byte 0xffff0166
	.4byte 0x00000001
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x01028000
	.4byte 0xffff0166
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x01028000
	.4byte 0xffff0166
	.4byte 0x00000001
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x01020000
	.4byte 0xffff0166
	.4byte 0x00000001
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x01028000
	.4byte 0xffff0166
	.4byte 0x00000001
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x01020000
	.4byte 0xffff0166
	.4byte 0x00000001
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x01020000
	.4byte 0xffff0166
	.4byte 0x00000001
	.4byte 0x02080000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x01028000
	.4byte 0xffff0166
	.4byte 0x00000001
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x01020000
	.4byte 0xffff0166
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x01020000
	.4byte 0xffff0166
	.4byte 0x00000001
	.4byte 0x02a80000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x01028000
	.4byte 0xffff0166
	.4byte 0x00000001
	.4byte 0x02d80000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x01028000
	.4byte 0xffff0165
	.4byte 0x00000001
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00020000
	.4byte 0xffff0165
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x01020000
	.4byte 0xffff0165
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x01020000
	.4byte 0xffff0165
	.4byte 0x00000001
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x01028000
	.4byte 0xffff0165
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x01028000
	.4byte 0xffff0165
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x01020000
	.4byte 0xffff0165
	.4byte 0x00000001
	.4byte 0x02880000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x01028000
	.4byte 0xffff0165
	.4byte 0x00000001
	.4byte 0x02a80000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x01020000
	.4byte 0xffff0167
	.4byte 0x00000001
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00020000
	.4byte 0xffff0167
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x01028000
	.4byte 0xffff0167
	.4byte 0x00000001
	.4byte 0x02080000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x01020000
	.4byte 0xffff0167
	.4byte 0x00000001
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x01020000
	.4byte 0xffff0167
	.4byte 0x00000001
	.4byte 0x02c80000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x01028000
	.4byte 0xffff0168
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00020000
	.4byte 0xffff0168
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x01020000
	.4byte 0xffff0168
	.4byte 0x00000001
	.4byte 0x02380000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x01020000
	.4byte 0xffff0169
	.4byte 0x00000001
	.4byte 0x01c80000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00020000
	.4byte 0xffff0169
	.4byte 0x00000001
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x01020000
	.4byte 0x0a6c0084
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0002c000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020067e4
Data_020067e4:
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00020000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00020000
	.4byte 0xffff0167
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00020000
	.4byte 0xffff0167
	.4byte 0x00000001
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x01020000
	.4byte 0xffff016a
	.4byte 0x00000001
	.4byte 0x01a00000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00028000
	.4byte 0xffff016a
	.4byte 0x00000001
	.4byte 0x01d00000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x01020000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_0200e88c
Data_0200e88c:
	.4byte 0x0000002e
	.4byte Func_0200047c
	.4byte 0x00000011
	.global Data_02006898
Data_02006898:
	.4byte 0xffff0178
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00024000
	.4byte 0xffff0176
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00024000
	.4byte 0xffff0176
	.4byte 0x00000001
	.4byte 0x01c80000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00024000
	.4byte 0xffff015d
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff015d
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff015d
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff015d
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0196
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00024000
	.4byte 0xffff019b
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff019b
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff019b
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff019b
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff019b
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff019b
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff019b
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff001a
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x01ae0000
	.4byte 0x00024000
	.4byte 0xffff0005
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0006
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0007
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
	.global Data_02006a90
Data_02006a90:
	.4byte 0xffff016a
	.4byte 0x00000001
	.4byte 0x02680000
	.4byte 0x00000000
	.4byte 0x01200000
	.4byte 0x00020000
	.4byte 0xffff016a
	.4byte 0x00000001
	.4byte 0x02680000
	.4byte 0x00000000
	.4byte 0x01500000
	.4byte 0x01020000
	.4byte 0xffff016a
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x01020000
	.4byte 0xffff016a
	.4byte 0x00000001
	.4byte 0x00800000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x01020000
	.4byte 0xffff016a
	.4byte 0x00000001
	.4byte 0x00b00000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x01028000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02006b20
Data_02006b20:
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02006b38
Data_02006b38:
	.4byte 0x00010101
	.4byte 0xffff0000
	.4byte 0x000000ff
	.4byte 0x00000101
	.4byte 0x0000ffff
	.4byte 0x00ff0001
	.4byte 0x00000000
	.global Data_02006b54
Data_02006b54:
	.4byte 0x00000202
	.4byte 0xffff0001
	.4byte Func_02001284
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02006b6c
Data_02006b6c:
	.4byte 0x00000202
	.4byte 0xffff0001
	.4byte Func_020012ec
	.4byte 0x00000002
	.4byte 0xffff000a
	.4byte Func_020017dc
	.4byte 0x00000002
	.4byte 0xffff000b
	.4byte Func_02001820
	.4byte 0x00008515
	.4byte Data_02010002 + 0x6
	.4byte Func_02000550
	.4byte 0x00008515
	.4byte Data_02020004 + 0x6
	.4byte 0x00000000
	.4byte 0x50008615
	.4byte Field_Map149 + 0x1e28
	.4byte Func_02001990
	.4byte 0x50008615
	.4byte Field_Map155 + 0x791
	.4byte Func_02001990
	.4byte 0x00000006
	.4byte Tileset_Set69TilesD + 0x20c
	.4byte Func_020019d0
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02006bd8
Data_02006bd8:
	.4byte 0x00000202
	.4byte 0xffff0001
	.4byte Func_02001364
	.4byte 0x00000202
	.4byte 0xffff0002
	.4byte Func_020013bc
	.4byte 0x00000202
	.4byte 0xffff0003
	.4byte Func_02001430
	.4byte 0x00000002
	.4byte 0xffff0050
	.4byte Func_02000740
	.4byte 0x00000002
	.4byte 0xffff0051
	.4byte Func_02000784
	.4byte 0x00000002
	.4byte 0xffff0052
	.4byte Func_02000978
	.4byte 0x00000002
	.4byte 0xffff0053
	.4byte Func_02000ad4
	.4byte 0x00000602
	.4byte 0xffff0054
	.4byte Func_02000c38
	.4byte 0x00008602
	.4byte 0xffff0054
	.4byte Func_02000c38
	.4byte 0x0000c602
	.4byte 0xffff0054
	.4byte Func_02000c8c
	.4byte 0x00004602
	.4byte 0xffff0054
	.4byte Func_02000c8c
	.4byte 0x00008515
	.4byte Data_02010018 + 0x7
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x12040021
	.4byte Func_02000280
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02006c80
Data_02006c80:
	.4byte 0x00000202
	.4byte 0xffff0001
	.4byte Func_02001498
	.4byte 0x00000202
	.4byte 0xffff0002
	.4byte Func_02001500
	.4byte 0x00000202
	.4byte 0xffff0003
	.4byte Func_02001564
	.4byte 0x00000002
	.4byte 0xffff0050
	.4byte Func_02000740
	.4byte 0x00000002
	.4byte 0xffff0051
	.4byte Func_02000784
	.4byte 0x00000002
	.4byte 0xffff0052
	.4byte Func_02000978
	.4byte 0x00000002
	.4byte 0xffff0053
	.4byte Func_02000ad4
	.4byte 0x00000602
	.4byte 0xffff0054
	.4byte Func_02000c38
	.4byte 0x00008602
	.4byte 0xffff0054
	.4byte Func_02000c38
	.4byte 0x0000c602
	.4byte 0xffff0054
	.4byte Func_02000c8c
	.4byte 0x00004602
	.4byte 0xffff0054
	.4byte Func_02000c8c
	.4byte 0x00000002
	.4byte 0x0a6c004d
	.4byte Func_02000cf0
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02006d1c
Data_02006d1c:
	.4byte 0x00000202
	.4byte 0xffff0001
	.4byte Func_020015b8
	.4byte 0x00000002
	.4byte 0xffff0050
	.4byte Func_02000740
	.4byte 0x00000002
	.4byte 0xffff0051
	.4byte Func_02000784
	.4byte 0x00000002
	.4byte 0xffff0052
	.4byte Func_02000978
	.4byte 0x00000002
	.4byte 0xffff0053
	.4byte Func_02000ad4
	.4byte 0x00000602
	.4byte 0xffff0054
	.4byte Func_02000c38
	.4byte 0x00008602
	.4byte 0xffff0054
	.4byte Func_02000c38
	.4byte 0x0000c602
	.4byte 0xffff0054
	.4byte Func_02000c8c
	.4byte 0x00004602
	.4byte 0xffff0054
	.4byte Func_02000c8c
	.4byte 0x00000002
	.4byte 0xffff000a
	.4byte Func_02001864
	.4byte 0x00000002
	.4byte 0xffff000b
	.4byte Func_020018a8
	.4byte 0x00008515
	.4byte 0xffff0008
	.4byte 0x00000000
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02006db8
Data_02006db8:
	.4byte 0x00000202
	.4byte 0xffff0001
	.4byte Func_02001630
	.4byte 0x00000002
	.4byte 0xffff0050
	.4byte Func_020045d8
	.4byte 0x00000002
	.4byte 0xffff0051
	.4byte Func_02004530
	.4byte 0x00000002
	.4byte 0xffff0052
	.4byte Func_020046c8
	.4byte 0x00000002
	.4byte 0xffff0053
	.4byte Func_02004620
	.4byte 0x00000006
	.4byte 0x020500c9
	.4byte Func_02000fc0
	.4byte 0x50008615
	.4byte 0x08fb000f
	.4byte Func_02001a6c
	.4byte 0x00000006
	.4byte 0xffff00c8
	.4byte Func_02001a8c
	.4byte 0x50008615
	.4byte 0xffff0010
	.4byte Func_020043ec
	.4byte 0x50008615
	.4byte 0xffff0011
	.4byte Func_020043ec
	.4byte 0x50008615
	.4byte 0xffff0012
	.4byte Func_020043ec
	.4byte 0x50008615
	.4byte 0xffff0013
	.4byte Func_020043ec
	.4byte 0x50008615
	.4byte 0xffff0014
	.4byte Func_020043ec
	.4byte 0x50008615
	.4byte 0xffff0015
	.4byte Func_020043ec
	.4byte 0x50008615
	.4byte 0xffff0016
	.4byte Func_020043ec
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02006e78
Data_02006e78:
	.4byte 0x00000002
	.4byte 0xffff000a
	.4byte Func_02001728
	.4byte 0x00000002
	.4byte 0xffff000b
	.4byte Func_0200176c
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02006e9c
Data_02006e9c:
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02006ea8
Data_02006ea8:
	.4byte gMapBlocks
	.4byte 0x03030101
	.4byte 0x06040507
	.4byte 0x04060705
	.4byte Text_MessageContexts + 0x1fcd9
	.4byte 0x0b0b0a0a
	.global Data_02006ec0
Data_02006ec0:
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x80010000
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00000a00
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0x00000a00
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000018
	.4byte 0xc0010000
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00010000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x00010000
	.4byte 0x00000016
	.4byte 0x00000028
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.global Data_02006f38
Data_02006f38:
	.4byte 0x00000027
	.4byte 0x00000000
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00020000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x00020000
	.4byte 0x80010000
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xfffff600
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffff600
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000030
	.4byte 0xc0010000
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00010000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x00010000
	.4byte 0x00000016
	.4byte 0x00000028
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
