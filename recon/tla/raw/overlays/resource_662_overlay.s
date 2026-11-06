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
	ldr r7, [r3, #32]
	sub sp, #8
	bl Actor_GetById
	ldr r3, [r0, #80]
	ldr r5, .L_02008184
	ldr r3, [r3, #40]
	movs r1, #0
	movs r2, #0
	ldrsh r3, [r3, r2]
	lsls r4, r3, #16
	ldrh r3, [r5, r1]
	lsrs r2, r4, #16
	cmp r2, r3
	beq .L_02008082
.L_02008068:
	movs r2, #128
	lsls r3, r1, #16
	lsls r2, r2, #9
	adds r3, r3, r2
	lsrs r2, r3, #16
	asrs r1, r3, #16
	cmp r2, #5
	bhi .L_02008082
	lsls r3, r2, #1
	ldrh r3, [r5, r3]
	lsrs r2, r4, #16
	cmp r2, r3
	bne .L_02008068
.L_02008082:
	lsls r3, r1, #16
	lsrs r2, r3, #16
	cmp r2, #6
	bne .L_0200808e
	movs r0, #0
	b .L_02008174
.L_0200808e:
	ldr r6, .L_02008188
	lsls r2, r2, #2
	ldrsb r4, [r6, r2]
	adds r1, r4, #0
	cmp r4, #0
	bge .L_0200809c
	negs r1, r4
.L_0200809c:
	adds r3, r2, #2
	ldrsb r3, [r6, r3]
	cmp r3, #0
	bge .L_020080a6
	negs r3, r3
.L_020080a6:
	adds r3, r1, r3
	asrs r3, r3, #4
	mov r11, r3
	adds r3, r2, #1
	ldrsb r1, [r6, r3]
	adds r5, r1, #0
	cmp r1, #0
	bge .L_020080b8
	negs r5, r1
.L_020080b8:
	adds r3, r2, #3
	ldrsb r2, [r6, r3]
	cmp r2, #0
	bge .L_020080c2
	negs r2, r2
.L_020080c2:
	adds r5, r5, r2
	mov r9, r5
	ldr r2, [r0, #8]
	mov r3, r9
	asrs r3, r3, #4
	mov r9, r3
	mov r10, r2
	lsls r3, r4, #16
	ldr r0, [r0, #16]
	add r10, r3
	mov r3, r10
	asrs r3, r3, #20
	mov r10, r3
	mov r8, r0
	lsls r3, r1, #16
	movs r2, #164
	add r8, r3
	lsls r2, r2, #1
	mov r0, r8
	adds r3, r7, r2
	ldr r3, [r3]
	asrs r0, r0, #20
	mov r8, r0
	movs r0, #166
	lsls r0, r0, #1
	asrs r1, r3, #20
	adds r3, r7, r0
	ldr r3, [r3]
	adds r0, #52
	adds r2, r7, r0
	adds r0, #4
	ldr r6, [r2]
	asrs r3, r3, #20
	adds r2, r7, r0
	ldr r5, [r2]
	lsls r3, r3, #16
	lsls r2, r1, #16
	lsrs r2, r2, #16
	lsrs r3, r3, #16
	add r2, r10
	add r3, r8
	str r2, [sp, #0]
	str r3, [sp, #4]
	mov r0, r10
	mov r1, r8
	mov r2, r11
	mov r3, r9
	bl Func_02001848
	asrs r6, r6, #20
	asrs r5, r5, #20
	lsls r6, r6, #16
	lsls r5, r5, #16
	lsrs r6, r6, #16
	lsrs r5, r5, #16
	mov r3, r10
	mov r2, r8
	add r5, r8
	adds r3, #64
	adds r2, #64
	add r6, r10
	str r3, [sp, #0]
	str r2, [sp, #4]
	adds r1, r5, #0
	adds r0, r6, #0
	mov r2, r11
	mov r3, r9
	bl Func_02001848
	mov r2, r9
	str r2, [sp, #0]
	movs r5, #255
	movs r0, #0
	mov r1, r10
	mov r2, r8
	mov r3, r11
	str r5, [sp, #4]
	bl Map_FillCellRectByte2
	mov r3, r9
	str r3, [sp, #0]
	movs r0, #2
	mov r1, r10
	mov r2, r8
	mov r3, r11
	str r5, [sp, #4]
	bl Map_FillCellRectByte2
	movs r0, #1
.L_02008174:
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008184:
	.4byte gData_02001968
.L_02008188:
	.4byte gData_02001974
	.section .text.x020081d4,"ax",%progbits
	.balign 4
	.global Func_020001d4
	.thumb_func
Func_020001d4:
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
	bl Func_0200033c
	adds r4, r0, #0
	cmp r4, #0
	bne .L_020081fe
	b .L_0200831e
.L_020081fe:
	ldr r5, [r5]
	ldr r0, .L_02008330
	str r5, [sp, #20]
	lsls r1, r5, #2
	ldrsb r2, [r0, r1]
	cmp r2, #0
	bge .L_0200820e
	negs r2, r2
.L_0200820e:
	adds r3, r1, #2
	ldrsb r3, [r0, r3]
	cmp r3, #0
	bge .L_02008218
	negs r3, r3
.L_02008218:
	adds r3, r2, r3
	asrs r3, r3, #4
	str r3, [sp, #16]
	adds r3, r1, #1
	ldrsb r2, [r0, r3]
	cmp r2, #0
	bge .L_02008228
	negs r2, r2
.L_02008228:
	adds r3, r1, #3
	ldrsb r3, [r0, r3]
	cmp r3, #0
	bge .L_02008232
	negs r3, r3
.L_02008232:
	adds r3, r2, r3
	asrs r3, r3, #4
	str r3, [sp, #12]
	ldr r3, [sp, #24]
	ldr r2, .L_02008334
	ldr r1, .L_02008338
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
.L_02008270:
	ldr r3, [sp, #20]
	ldr r2, .L_02008330
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
	bge .L_020082de
.L_0200828e:
	ldr r3, .L_02008330
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
	bge .L_020082c8
.L_020082a6:
	adds r0, r4, #0
	add r1, sp, #28
	str r4, [sp, #0]
	bl Func_02001850
	ldr r4, [sp, #0]
	cmp r0, #2
	beq .L_020082f0
	ldr r3, [r5]
	movs r1, #128
	lsls r1, r1, #13
	adds r3, r3, r1
	str r3, [r5]
	ldr r2, [sp, #16]
	adds r7, #1
	cmp r7, r2
	blt .L_020082a6
.L_020082c8:
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
	blt .L_0200828e
.L_020082de:
	ldr r3, [r6]
	movs r1, #1
	add r3, r9
	str r3, [r6]
	ldr r3, [r6, #8]
	add r11, r1
	add r3, r10
	str r3, [r6, #8]
	b .L_02008270
.L_020082f0:
	ldr r2, [sp, #8]
	movs r3, #0
	strb r3, [r2]
	mov r3, r11
	movs r0, #0
	cmp r3, #0
	beq .L_02008320
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
	b .L_02008320
.L_0200831e:
	movs r0, #0
.L_02008320:
	add sp, #40
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008330:
	.4byte gData_02001974
.L_02008334:
	.4byte gData_0200198c
.L_02008338:
	.4byte 0xffff0000
	.section .text.x02009418,"ax",%progbits
	.ifdef TLA_EDITION_JA
	.balign 4
	.global Func_02001418
	.thumb_func
Func_02001418:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r0, #214
	lsls r0, r0, #1
	movs r2, #129
	adds r3, r3, r0
	lsls r2, r2, #2
	str r2, [r3]
	movs r0, #0
	sub sp, #8
	bl Func_02001928
	ldr r5, .L_ja_02009558
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r5, r1
	movs r0, #0
	ldrsh r2, [r3, r0]
	ldr r3, .L_ja_0200955c
	cmp r2, r3
	bne .L_ja_02009500
	ldr r1, .L_ja_02009560
	ldr r2, .L_ja_02009564
	ldr r3, .L_ja_02009568
	ldr r0, .L_ja_0200956c
	bl Func_020007dc
	movs r0, #8
	bl Func_020018c8
	movs r0, #9
	bl Func_020018c8
	movs r0, #10
	bl Func_020018c8
	movs r0, #8
	bl Func_02000038
	movs r0, #9
	bl Func_02000038
	movs r0, #10
	bl Func_02000038
	movs r0, #192
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_02009498
	movs r0, #11
	movs r1, #0
	movs r2, #0
	bl Func_020018a8
	movs r3, #34
	movs r2, #13
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #32
	movs r1, #13
	movs r2, #1
	movs r3, #1
	bl Func_02001848
.L_ja_02009498:
	movs r1, #241
	lsls r1, r1, #1
	adds r3, r5, r1
	movs r2, #0
	ldrsh r6, [r3, r2]
	cmp r6, #1
	bne .L_ja_02009500
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #41
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_020094ca
	ldr r3, .L_ja_02009570
	movs r0, #152
	movs r1, #128
	lsls r0, r0, #2
	lsls r1, r1, #2
	adds r2, r5, r0
	adds r1, #98
	strh r3, [r2]
	adds r3, r5, r1
	strh r6, [r3]
	b .L_ja_02009500
.L_ja_020094ca:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #42
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_020094dc
	ldr r2, .L_ja_02009574
	b .L_ja_020094ec
.L_ja_020094dc:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #43
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_02009500
	ldr r2, .L_ja_02009578
.L_ja_020094ec:
	movs r0, #152
	movs r1, #128
	lsls r0, r0, #2
	lsls r1, r1, #2
	adds r3, r5, r0
	adds r1, #98
	strh r2, [r3]
	adds r2, r5, r1
	movs r3, #2
	strh r3, [r2]
.L_ja_02009500:
	ldr r3, .L_ja_02009558
	movs r2, #240
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r0, #0
	ldrsh r2, [r3, r0]
	ldr r3, .L_ja_0200957c
	cmp r2, r3
	bne .L_ja_020095aa
	movs r0, #129
	lsls r0, r0, #2
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_02009580
	movs r1, #138
	movs r2, #216
	movs r0, #8
	lsls r1, r1, #18
	lsls r2, r2, #16
	bl Func_020018a8
	movs r3, #34
	movs r5, #13
	str r3, [sp, #0]
	movs r0, #32
	movs r1, #13
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl Func_02001848
	movs r3, #98
	str r3, [sp, #0]
	movs r0, #95
	movs r1, #3
	movs r2, #3
	movs r3, #3
	str r5, [sp, #4]
	bl Func_02001848
	b .L_ja_02009594
	.2byte 0x0000
.L_ja_02009558:
	.4byte gMainRoutineWork
.L_ja_0200955c:
	.4byte 0x0000004f
.L_ja_02009560:
	.4byte gData_020019fc
.L_ja_02009564:
	.4byte gData_02001a28
.L_ja_02009568:
	.4byte gData_02001a54
.L_ja_0200956c:
	.4byte gData_020019ec
.L_ja_02009570:
	.4byte 0x00000043
.L_ja_02009574:
	.4byte 0x0000004a
.L_ja_02009578:
	.4byte 0x0000004b
.L_ja_0200957c:
	.4byte 0x00000050
.L_ja_02009580:
	movs r3, #34
	movs r2, #12
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #32
	movs r1, #13
	movs r2, #1
	movs r3, #1
	bl Func_02001848
.L_ja_02009594:
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #2
	bl GameFlag_Test
	cmp r0, #0
	bne .L_ja_020095aa
	movs r0, #64
	movs r1, #1
	bl Actor_SetWideSprite
.L_ja_020095aa:
	ldr r1, .L_ja_0200961c
	movs r2, #240
	lsls r2, r2, #1
	adds r3, r1, r2
	movs r0, #0
	ldrsh r2, [r3, r0]
	ldr r3, .L_ja_02009620
	cmp r2, r3
	beq .L_ja_020095be
	b .L_ja_020097b0
.L_ja_020095be:
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r1, r2
	ldrh r3, [r3]
	movs r0, #128
	subs r3, #3
	lsls r3, r3, #16
	lsls r0, r0, #10
	cmp r3, r0
	bhi .L_ja_02009624
	movs r0, #128
	lsls r0, r0, #19
	adds r0, #10
	movs r1, #255
	ldrh r2, [r0]
	lsls r1, r1, #8
	adds r1, #252
	adds r3, r1, #0
	ands r3, r2
	strh r3, [r0]
	ldr r2, .L_ja_02009614
	ldrh r3, [r0]
	orrs r3, r2
	strh r3, [r0]
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #12
	ldrh r0, [r2]
	adds r3, r1, #0
	ands r3, r0
	strh r3, [r2]
	ldr r0, .L_ja_02009618
	ldrh r3, [r2]
	orrs r3, r0
	strh r3, [r2]
	adds r2, #2
	ldrh r3, [r2]
	ands r1, r3
	strh r1, [r2]
	ldrh r3, [r2]
	orrs r3, r0
	strh r3, [r2]
	b .L_ja_02009624
.L_ja_02009614:
	.4byte 0x00000001
.L_ja_02009618:
	.4byte 0x00000003
.L_ja_0200961c:
	.4byte gMainRoutineWork
.L_ja_02009620:
	.4byte 0x00000051
.L_ja_02009624:
	movs r0, #8
	bl Func_020018c8
	movs r0, #9
	bl Func_020018c8
	movs r0, #10
	bl Func_020018c8
	movs r0, #11
	bl Func_020018c8
	movs r0, #8
	bl Func_02000038
	movs r0, #9
	bl Func_02000038
	movs r0, #10
	bl Func_02000038
	movs r0, #11
	bl Func_02000038
	ldr r0, .L_ja_020097bc
	bl Func_02000b54
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #6
	bl GameFlag_Test
	cmp r0, #0
	bne .L_ja_0200967c
	movs r3, #46
	movs r2, #12
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #48
	movs r1, #12
	movs r2, #1
	movs r3, #1
	bl Func_02001848
.L_ja_0200967c:
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #1
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_02009694
	movs r0, #15
	movs r1, #0
	movs r2, #0
	bl Func_020018a8
.L_ja_02009694:
	movs r0, #193
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_020096c4
	movs r1, #168
	movs r2, #184
	movs r0, #13
	lsls r1, r1, #16
	lsls r2, r2, #16
	bl Func_020018a8
	movs r3, #10
	movs r2, #11
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #12
	movs r1, #11
	movs r2, #1
	movs r3, #1
	bl Func_02001848
	b .L_ja_020096d8
.L_ja_020096c4:
	movs r3, #8
	movs r2, #10
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #12
	movs r1, #11
	movs r2, #1
	movs r3, #1
	bl Func_02001848
.L_ja_020096d8:
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #5
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200970a
	movs r1, #154
	movs r2, #132
	movs r0, #12
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl Func_020018a8
	movs r3, #38
	movs r2, #16
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #38
	movs r1, #15
	movs r2, #1
	movs r3, #1
	bl Func_02001848
	b .L_ja_0200971e
.L_ja_0200970a:
	movs r3, #39
	movs r2, #16
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #38
	movs r1, #15
	movs r2, #1
	movs r3, #1
	bl Func_02001848
.L_ja_0200971e:
	movs r0, #65
	bl GameFlag_Test
	cmp r0, #0
	bne .L_ja_020097b0
	movs r0, #145
	bl GameFlag_Test
	cmp r0, #0
	bne .L_ja_020097b0
	movs r0, #158
	lsls r0, r0, #2
	bl GameFlag_GetByte
	adds r5, r0, #0
	movs r0, #185
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	adds r6, r0, #0
	cmp r6, #0
	beq .L_ja_02009776
	adds r3, r5, #0
	cmp r5, #0
	bge .L_ja_02009754
	adds r3, #31
.L_ja_02009754:
	movs r0, #16
	movs r2, #0
	movs r1, #0
	asrs r5, r3, #5
	bl Func_020018a8
	ldr r3, .L_ja_020097c0
	lsls r5, r5, #2
	ldrsh r1, [r3, r5]
	adds r5, #2
	ldrsh r2, [r3, r5]
	lsls r1, r1, #19
	lsls r2, r2, #19
	movs r0, #17
	bl Func_020018a8
	b .L_ja_020097b0
.L_ja_02009776:
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200978c
	movs r0, #158
	lsls r0, r0, #2
	movs r1, #0
	bl GameFlag_SetByte
.L_ja_0200978c:
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_ja_020097c4
	bl Task_Add
	movs r0, #156
	lsls r0, r0, #2
	bl GameFlag_SetBit
	movs r0, #16
	movs r1, #3
	bl Actor_SetSpritePriority
	movs r0, #16
	bl Actor_GetById
	adds r0, #89
	strb r6, [r0]
.L_ja_020097b0:
	movs r0, #0
	bl Func_02001928
	movs r0, #0
	sub sp, #-8
	pop {r5, r6, pc}
.L_ja_020097bc:
	.4byte gData_02001d20
.L_ja_020097c0:
	.4byte gData_020019cc
.L_ja_020097c4:
	.4byte Func_020011e0
	.else
	.balign 4
	.global Func_02001418
	.thumb_func
Func_02001418:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r0, #214
	lsls r0, r0, #1
	movs r2, #129
	adds r3, r3, r0
	lsls r2, r2, #2
	str r2, [r3]
	movs r0, #0
	sub sp, #8
	bl Func_02001928
	ldr r5, .L_02009560
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r5, r1
	movs r0, #0
	ldrsh r2, [r3, r0]
	ldr r3, .L_02009564
	cmp r2, r3
	bne .L_02009508
	ldr r1, .L_02009568
	ldr r2, .L_0200956c
	ldr r3, .L_02009570
	ldr r0, .L_02009574
	bl Func_020007dc
	movs r0, #8
	bl Func_020018c8
	movs r0, #9
	bl Func_020018c8
	movs r0, #10
	bl Func_020018c8
	movs r0, #8
	bl Func_02000038
	movs r0, #9
	bl Func_02000038
	movs r0, #10
	bl Func_02000038
	movs r0, #192
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020094a0
	movs r0, #11
	movs r1, #0
	movs r2, #0
	bl Func_020018a8
	movs r3, #34
	movs r2, #13
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #32
	movs r1, #13
	movs r2, #1
	movs r3, #1
	bl Func_02001848
.L_020094a0:
	movs r1, #241
	lsls r1, r1, #1
	adds r3, r5, r1
	movs r2, #0
	ldrsh r6, [r3, r2]
	cmp r6, #1
	bne .L_02009508
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #41
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020094d2
	ldr r3, .L_02009578
	movs r0, #152
	movs r1, #128
	lsls r0, r0, #2
	lsls r1, r1, #2
	adds r2, r5, r0
	adds r1, #98
	strh r3, [r2]
	adds r3, r5, r1
	strh r6, [r3]
	b .L_02009508
.L_020094d2:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #42
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020094e4
	ldr r2, .L_0200957c
	b .L_020094f4
.L_020094e4:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #43
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009508
	ldr r2, .L_02009580
.L_020094f4:
	movs r0, #152
	movs r1, #128
	lsls r0, r0, #2
	lsls r1, r1, #2
	adds r3, r5, r0
	adds r1, #98
	strh r2, [r3]
	adds r2, r5, r1
	movs r3, #2
	strh r3, [r2]
.L_02009508:
	ldr r3, .L_02009560
	movs r2, #240
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r0, #0
	ldrsh r2, [r3, r0]
	ldr r3, .L_02009584
	cmp r2, r3
	bne .L_020095b2
	movs r0, #129
	lsls r0, r0, #2
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009588
	movs r1, #138
	movs r2, #216
	movs r0, #8
	lsls r1, r1, #18
	lsls r2, r2, #16
	bl Func_020018a8
	movs r3, #34
	movs r5, #13
	str r3, [sp, #0]
	movs r0, #32
	movs r1, #13
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl Func_02001848
	movs r3, #98
	str r3, [sp, #0]
	movs r0, #95
	movs r1, #3
	movs r2, #3
	movs r3, #3
	str r5, [sp, #4]
	bl Func_02001848
	b .L_0200959c
	.2byte 0x0000
.L_02009560:
	.4byte gMainRoutineWork
.L_02009564:
	.4byte 0x0000004f
.L_02009568:
	.4byte gData_020019fc
.L_0200956c:
	.4byte gData_02001a28
.L_02009570:
	.4byte gData_02001a54
.L_02009574:
	.4byte gData_020019ec
.L_02009578:
	.4byte 0x00000043
.L_0200957c:
	.4byte 0x0000004a
.L_02009580:
	.4byte 0x0000004b
.L_02009584:
	.4byte 0x00000050
.L_02009588:
	movs r3, #34
	movs r2, #12
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #32
	movs r1, #13
	movs r2, #1
	movs r3, #1
	bl Func_02001848
.L_0200959c:
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #2
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020095b2
	movs r0, #64
	movs r1, #1
	bl Actor_SetWideSprite
.L_020095b2:
	ldr r1, .L_02009624
	movs r2, #240
	lsls r2, r2, #1
	adds r3, r1, r2
	movs r0, #0
	ldrsh r2, [r3, r0]
	ldr r3, .L_02009628
	cmp r2, r3
	beq .L_020095c6
	b .L_020097c2
.L_020095c6:
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r1, r2
	ldrh r3, [r3]
	movs r0, #128
	subs r3, #3
	lsls r3, r3, #16
	lsls r0, r0, #10
	cmp r3, r0
	bhi .L_0200962c
	movs r0, #128
	lsls r0, r0, #19
	adds r0, #10
	movs r1, #255
	ldrh r2, [r0]
	lsls r1, r1, #8
	adds r1, #252
	adds r3, r1, #0
	ands r3, r2
	strh r3, [r0]
	ldr r2, .L_0200961c
	ldrh r3, [r0]
	orrs r3, r2
	strh r3, [r0]
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #12
	ldrh r0, [r2]
	adds r3, r1, #0
	ands r3, r0
	strh r3, [r2]
	ldr r0, .L_02009620
	ldrh r3, [r2]
	orrs r3, r0
	strh r3, [r2]
	adds r2, #2
	ldrh r3, [r2]
	ands r1, r3
	strh r1, [r2]
	ldrh r3, [r2]
	orrs r3, r0
	strh r3, [r2]
	b .L_0200962c
.L_0200961c:
	.4byte 0x00000001
.L_02009620:
	.4byte 0x00000003
.L_02009624:
	.4byte gMainRoutineWork
.L_02009628:
	.4byte 0x00000051
.L_0200962c:
	movs r0, #8
	bl Func_020018c8
	movs r0, #9
	bl Func_020018c8
	movs r0, #10
	bl Func_020018c8
	movs r0, #11
	bl Func_020018c8
	movs r0, #8
	bl Func_02000038
	movs r0, #9
	bl Func_02000038
	movs r0, #10
	bl Func_02000038
	movs r0, #11
	bl Func_02000038
	ldr r0, .L_020097d0
	bl Func_02000b54
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #6
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02009684
	movs r3, #46
	movs r2, #12
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #48
	movs r1, #12
	movs r2, #1
	movs r3, #1
	bl Func_02001848
.L_02009684:
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #1
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200969c
	movs r0, #15
	movs r1, #0
	movs r2, #0
	bl Func_020018a8
.L_0200969c:
	movs r0, #193
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020096cc
	movs r1, #168
	movs r2, #184
	movs r0, #13
	lsls r1, r1, #16
	lsls r2, r2, #16
	bl Func_020018a8
	movs r3, #10
	movs r2, #11
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #12
	movs r1, #11
	movs r2, #1
	movs r3, #1
	bl Func_02001848
	b .L_020096ea
.L_020096cc:
	movs r0, #13
	bl Actor_GetById
	ldr r2, [r0, #8]
	ldr r3, [r0, #16]
	asrs r2, r2, #20
	asrs r3, r3, #20
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r0, #12
	movs r1, #11
	movs r2, #1
	movs r3, #1
	bl Func_02001848
.L_020096ea:
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #5
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200971c
	movs r1, #154
	movs r2, #132
	movs r0, #12
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl Func_020018a8
	movs r3, #38
	movs r2, #16
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #38
	movs r1, #15
	movs r2, #1
	movs r3, #1
	bl Func_02001848
	b .L_02009730
.L_0200971c:
	movs r3, #39
	movs r2, #16
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #38
	movs r1, #15
	movs r2, #1
	movs r3, #1
	bl Func_02001848
.L_02009730:
	movs r0, #65
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020097c2
	movs r0, #145
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020097c2
	movs r0, #158
	lsls r0, r0, #2
	bl GameFlag_GetByte
	adds r5, r0, #0
	movs r0, #185
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	adds r6, r0, #0
	cmp r6, #0
	beq .L_02009788
	adds r3, r5, #0
	cmp r5, #0
	bge .L_02009766
	adds r3, #31
.L_02009766:
	movs r0, #16
	movs r2, #0
	movs r1, #0
	asrs r5, r3, #5
	bl Func_020018a8
	ldr r3, .L_020097d4
	lsls r5, r5, #2
	ldrsh r1, [r3, r5]
	adds r5, #2
	ldrsh r2, [r3, r5]
	lsls r1, r1, #19
	lsls r2, r2, #19
	movs r0, #17
	bl Func_020018a8
	b .L_020097c2
.L_02009788:
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200979e
	movs r0, #158
	lsls r0, r0, #2
	movs r1, #0
	bl GameFlag_SetByte
.L_0200979e:
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_020097d8
	bl Task_Add
	movs r0, #156
	lsls r0, r0, #2
	bl GameFlag_SetBit
	movs r0, #16
	movs r1, #3
	bl Actor_SetSpritePriority
	movs r0, #16
	bl Actor_GetById
	adds r0, #89
	strb r6, [r0]
.L_020097c2:
	movs r0, #0
	bl Func_02001928
	movs r0, #0
	add sp, #8
	pop {r5, r6, pc}
	.2byte 0x0000
.L_020097d0:
	.4byte gData_02001d20
.L_020097d4:
	.4byte gData_020019cc
.L_020097d8:
	.4byte Func_020011e0
	.endif
	.section .bss,"aw",%nobits
	.global gData_02001fc8
gData_02001fc8:
	.space 0x00000004
	.global gData_02001fcc
gData_02001fcc:
	.space 0x00000004
	.global gData_02001fd0
gData_02001fd0:
	.space 0x00000004
	.global gData_02001fd4
gData_02001fd4:
	.space 0x00000004
	.global gData_02001fd8
gData_02001fd8:
	.space 0x00000004
	.global gData_02001fdc
gData_02001fdc:
	.space 0x00000004
	.global gData_02001fe0
gData_02001fe0:
	.space 0x00000004
	.global gData_02001fe4
gData_02001fe4:
	.space 0x00000004
	.global gData_02001fe8
gData_02001fe8:
