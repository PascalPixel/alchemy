.syntax unified
	.thumb
	.section .text.x0200805c,"ax",%progbits
	.global Func_0200005c
	.thumb_func
Func_0200005c:
	push {lr}
	movs r0, #128
	movs r1, #252
	lsls r0, r0, #19
	lsls r1, r1, #6
	adds r0, #80
	adds r1, #65
	bl QueueIoWriteDelay2
	ldr r3, .L_0200809c
	movs r2, #2
	ldr r3, [r3]
	ands r3, r2
	ldr r2, .L_020080a0
	cmp r3, #0
	beq .L_020080a8
	ldr r3, .L_020080a4
	ldrh r3, [r3]
	cmp r3, #0
	bne .L_020080a8
	ldrh r3, [r2]
	ldr r1, .L_02008098
	movs r0, #128
	lsls r0, r0, #19
	orrs r1, r3
	adds r0, #82
	bl QueueIoWriteDelay2
	b .L_020080b8
	.2byte 0x0000
.L_02008098:
	.4byte 0x0000000f
.L_0200809c:
	.4byte gFrameCount
.L_020080a0:
	.4byte Data_02003dbc
.L_020080a4:
	.4byte Data_02003dbe
.L_020080a8:
	ldrh r3, [r2]
	ldr r1, .L_020080bc
	movs r0, #128
	lsls r0, r0, #19
	orrs r1, r3
	adds r0, #82
	bl QueueIoWriteDelay2
.L_020080b8:
	pop {pc}
	.2byte 0x0000
.L_020080bc:
	.4byte 0x00000010
	.section .text.x020080c0,"ax",%progbits
	.global Func_020000c0
	.thumb_func
Func_020000c0:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r7, r1, #0
	adds r5, r0, #0
	mov lr, r2
	mov r12, r3
	movs r6, #0
	cmp r7, #0
	beq .L_02008132
.L_020080d4:
	ldrh r2, [r5]
	ldr r1, .L_02008104
	movs r3, #31
	ands r3, r2
	mov r4, lr
	lsls r2, r2, #16
	adds r0, r3, r4
	lsrs r3, r2, #21
	mov r8, r2
	ands r3, r1
	mov r2, r12
	adds r4, r3, r2
	mov r3, r8
	lsrs r2, r3, #26
	ldr r3, [sp, #20]
	ands r2, r1
	adds r2, r2, r3
	cmp r0, #31
	ble .L_020080fc
	movs r0, #31
.L_020080fc:
	cmp r0, #0
	bge .L_02008108
	movs r0, #0
	b .L_02008108
.L_02008104:
	.4byte 0x0000001f
.L_02008108:
	cmp r4, #31
	ble .L_0200810e
	movs r4, #31
.L_0200810e:
	cmp r4, #0
	bge .L_02008114
	movs r4, #0
.L_02008114:
	cmp r2, #31
	ble .L_0200811a
	movs r2, #31
.L_0200811a:
	cmp r2, #0
	bge .L_02008120
	movs r2, #0
.L_02008120:
	lsls r3, r2, #10
	lsls r2, r4, #5
	orrs r3, r2
	orrs r3, r0
	adds r6, #1
	strh r3, [r5]
	adds r5, #2
	cmp r6, r7
	bne .L_020080d4
.L_02008132:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.section .text.x02008138,"ax",%progbits
	.global Func_02000138
	.thumb_func
Func_02000138:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #4
	mov r11, r1
	movs r1, #128
	str r2, [sp, #0]
	ldr r3, .L_02008204
	ldr r2, .L_02008208
	lsls r1, r1, #5
	mov r8, r0
	mov lr, r3
	.2byte 0xf800
	mov r2, r11
	movs r7, #0
	cmp r2, #1
	beq .L_020081f4
.L_02008162:
	movs r4, #1
	negs r4, r4
	add r4, r11
	mov r9, r4
	movs r3, #0
	mov r1, r9
	lsls r0, r7, #14
	mov r10, r3
	bl __divsi3
	bl Math_Cosine
	lsls r3, r0, #4
	ldr r2, [sp, #0]
	subs r3, r3, r0
	asrs r1, r3, #16
	adds r3, r2, #0
	muls r3, r1
	adds r5, r7, #0
	adds r6, r7, #0
	cmp r3, #0
	bge .L_02008196
	movs r4, #255
	lsls r4, r4, #8
	adds r4, #255
	adds r3, r3, r4
.L_02008196:
	asrs r1, r3, #16
	cmp r1, #15
	ble .L_0200819e
	movs r1, #15
.L_0200819e:
	cmp r1, #0
	bgt .L_020081a4
	movs r1, #1
.L_020081a4:
	cmp r7, #0
	blt .L_020081ee
.L_020081a8:
	movs r2, #63
	mov r3, r10
	subs r0, r2, r5
	subs r2, r2, r3
	lsls r3, r2, #6
	adds r3, r3, r0
	add r3, r8
	strb r1, [r3]
	cmp r0, #63
	bge .L_020081be
	strb r1, [r3, #1]
.L_020081be:
	movs r4, #63
	adds r0, r2, #0
	subs r2, r4, r5
	lsls r3, r2, #6
	adds r3, r3, r0
	add r3, r8
	strb r1, [r3]
	cmp r0, #63
	bge .L_020081d2
	strb r1, [r3, #1]
.L_020081d2:
	mov r2, r10
	lsls r3, r2, #1
	subs r3, r6, r3
	subs r6, r3, #1
	cmp r6, #0
	bge .L_020081e6
	lsls r3, r5, #1
	adds r3, r6, r3
	subs r6, r3, #2
	subs r5, #1
.L_020081e6:
	movs r3, #1
	add r10, r3
	cmp r5, r10
	bge .L_020081a8
.L_020081ee:
	adds r7, #1
	cmp r7, r9
	bne .L_02008162
.L_020081f4:
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008204:
	.4byte IwramFillWords
.L_02008208:
	.4byte 0x01010101
	.section .text.x0200820c,"ax",%progbits
	.global Func_0200020c
	.thumb_func
Func_0200020c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r7, #192
	lsls r7, r7, #18
	mov r11, r0
	ldr r0, [r7, #32]
	sub sp, #16
	str r0, [sp, #12]
	movs r1, #0
	movs r0, #8
	movs r2, #0
	bl Func_02002034
	ldr r1, .L_0200826c
	ldr r3, .L_02008270
	mov r8, r1
	mov r2, r8
	strh r2, [r3]
	movs r0, #1
	bl WaitFrames
	movs r0, #10
	adds r0, #255
	bl GameFlag_ClearBit
	ldr r3, [r7, #108]
	movs r4, #214
	lsls r4, r4, #1
	adds r3, r3, r4
	mov r0, r11
	movs r5, #0
	str r5, [r3]
	str r0, [sp, #8]
	movs r0, #192
	lsls r0, r0, #7
	bl Runtime_BumpAllocateAlternatePool
	mov r10, r0
	movs r0, #8
	bl Object_GetById
	movs r1, #0
	b .L_02008274
.L_0200826c:
	.4byte 0x00000000
.L_02008270:
	.4byte Data_02003dbe
.L_02008274:
	bl ObjectDispatch_SetSingleChildField26
	mov r1, r11
	cmp r1, #4
	bne .L_02008282
	movs r2, #0
	str r2, [sp, #8]
.L_02008282:
	ldr r3, .L_020083ec
	mov r4, r11
	strh r5, [r3, #2]
	strh r5, [r3]
	ldr r3, .L_020083f0
	lsls r6, r4, #2
	adds r5, r6, #2
	ldrsh r0, [r3, r6]
	ldrsh r2, [r3, r5]
	movs r1, #1
	lsls r2, r2, #16
	lsls r0, r0, #16
	negs r1, r1
	movs r3, #0
	bl Motion_CamBounds
	bl Func_02001fdc
	ldr r3, .L_020083f4
	movs r1, #1
	ldr r0, [r3, r6]
	bl Func_0200208c
	ldr r0, [sp, #12]
	movs r1, #143
	lsls r1, r1, #1
	movs r3, #128
	adds r2, r0, r1
	lsls r3, r3, #5
	strh r3, [r2]
	movs r3, #142
	lsls r3, r3, #1
	adds r2, r0, r3
	movs r3, #208
	lsls r3, r3, #8
	strh r3, [r2]
	movs r1, #32
	ldr r3, .L_020083f8
	ldr r0, .L_020083fc
	mov lr, r3
	.2byte 0xf800
	movs r0, #1
	bl WaitFrames
	ldr r3, .L_02008400
	movs r4, #133
	lsls r4, r4, #2
	adds r3, r3, r4
	ldr r0, [r3]
	bl Object_GetById
	mov r1, r8
	adds r0, #84
	strb r1, [r0]
	ldr r3, [r7, #108]
	movs r2, #218
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #64
	str r2, [r3]
	bl Event_SetStatus1c6
	ldr r3, .L_02008404
	movs r7, #0
	ldrsh r2, [r3, r5]
	ldrsh r0, [r3, r6]
	movs r1, #1
	lsls r2, r2, #16
	movs r3, #1
	lsls r0, r0, #16
	negs r1, r1
	bl Motion_CamBounds
	ldr r3, .L_02008408
	mov r2, r11
	ldrb r3, [r3, r2]
	cmp r3, #0
	beq .L_02008370
.L_0200831e:
	ldr r5, .L_02008408
	mov r3, r11
	ldrb r2, [r5, r3]
	ldr r4, [sp, #12]
	movs r0, #143
	subs r3, r2, r7
	lsls r0, r0, #1
	lsls r3, r3, #5
	adds r1, r4, r0
	subs r2, #59
	strh r3, [r1]
	cmp r7, r2
	bne .L_02008342
	ldr r3, .L_0200840c
	movs r1, #60
	ldr r0, [r3, r6]
	bl Func_0200208c
.L_02008342:
	mov r1, r11
	ldrb r3, [r5, r1]
	subs r3, #64
	cmp r7, r3
	blt .L_02008360
	ldr r2, [sp, #12]
	movs r3, #142
	lsls r3, r3, #1
	adds r1, r2, r3
	ldr r3, .L_02008410
	mov r4, r11
	ldrb r2, [r3, r4]
	ldrh r3, [r1]
	adds r3, r3, r2
	strh r3, [r1]
.L_02008360:
	movs r0, #1
	bl WaitFrames
	mov r0, r11
	ldrb r3, [r5, r0]
	adds r7, #1
	cmp r7, r3
	bne .L_0200831e
.L_02008370:
	bl Func_02002074
	mov r1, r11
	cmp r1, #4
	bne .L_02008430
	movs r5, #128
	lsls r5, r5, #6
	mov r1, r10
	add r5, r10
	ldr r0, .L_02008414
	bl Func_02001f54
	adds r1, r5, #0
	ldr r0, .L_02008418
	bl Func_02001f54
	ldr r0, .L_0200841c
	ldr r1, .L_02008420
	ldrh r3, [r1]
	adds r4, r3, #0
	strh r1, [r1]
	ldrh r2, [r0]
	cmp r2, #31
	bgt .L_020083bc
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	adds r2, #1
	adds r3, r3, r0
	adds r3, #4
	strh r2, [r0]
	mov r2, r10
	stmia r3!, {r2}
	movs r2, #192
	lsls r2, r2, #19
	stmia r3!, {r2}
	ldr r2, .L_02008424
	str r2, [r3]
.L_020083bc:
	strh r4, [r1]
	ldrh r3, [r1]
	adds r4, r3, #0
	strh r1, [r1]
	ldrh r2, [r0]
	cmp r2, #31
	bgt .L_020083e2
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	adds r3, r3, r0
	adds r3, #4
	adds r2, #1
	stmia r3!, {r5}
	strh r2, [r0]
	ldr r2, .L_02008428
	stmia r3!, {r2}
	ldr r2, .L_0200842c
	str r2, [r3]
.L_020083e2:
	strh r4, [r1]
	movs r0, #1
	bl WaitFrames
	b .L_0200855a
.L_020083ec:
	.4byte Data_03001120
.L_020083f0:
	.4byte Data_02003d20
.L_020083f4:
	.4byte Data_02003d5c
.L_020083f8:
	.4byte IwramClearWords
.L_020083fc:
	.4byte 0x050001c0
.L_02008400:
	.4byte gPartyState
.L_02008404:
	.4byte Data_02003d34
.L_02008408:
	.4byte Data_02003d84
.L_0200840c:
	.4byte Data_02003d70
.L_02008410:
	.4byte Data_02003d88 + 0x1
.L_02008414:
	.4byte Data_02002468 + 0x1
.L_02008418:
	.4byte Data_0200217c
.L_0200841c:
	.4byte gIoWriteQueue
.L_02008420:
	.4byte 0x04000208
.L_02008424:
	.4byte 0x84000800
.L_02008428:
	.4byte 0x06002000
.L_0200842c:
	.4byte 0x84000140
.L_02008430:
	ldr r1, .L_020084c8
	ldr r0, .L_020084cc
	ldrh r3, [r0]
	adds r4, r3, #0
	strh r0, [r0]
	ldrh r2, [r1]
	cmp r2, #31
	bgt .L_02008464
	lsls r3, r2, #1
	adds r3, r3, r2
	adds r2, #1
	strh r2, [r1]
	lsls r3, r3, #2
	adds r3, r3, r1
	ldr r1, [sp, #8]
	adds r3, #4
	lsls r2, r1, #5
	ldr r1, .L_020084d0
	adds r2, r2, r1
	stmia r3!, {r2}
	ldr r2, .L_020084d4
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #24
	adds r2, #16
	str r2, [r3]
.L_02008464:
	strh r4, [r0]
	ldr r3, .L_020084d8
	movs r1, #128
	lsls r1, r1, #6
	ldr r0, .L_020084dc
	mov lr, r3
	.2byte 0xf800
	movs r0, #1
	bl WaitFrames
	movs r6, #0
.L_0200847a:
	movs r5, #0
.L_0200847c:
	lsls r0, r6, #4
	adds r0, r0, r5
	ldr r4, .L_020084b8
	adds r0, #128
	lsls r2, r6, #5
	adds r1, r2, r5
	adds r3, r0, #0
	orrs r3, r4
	lsls r1, r1, #1
	mov r4, r10
	strh r3, [r1, r4]
	ldr r1, .L_020084bc
	subs r2, r2, r5
	adds r3, r0, #0
	lsls r2, r2, #1
	orrs r3, r1
	add r2, r10
	strh r3, [r2, #62]
	movs r2, #15
	ldr r4, .L_020084c0
	subs r2, r2, r6
	lsls r2, r2, #5
	adds r1, r2, r5
	adds r3, r0, #0
	orrs r3, r4
	lsls r1, r1, #1
	mov r4, r10
	strh r3, [r1, r4]
	ldr r3, .L_020084c4
	b .L_020084e0
.L_020084b8:
	.4byte 0xffffe000
.L_020084bc:
	.4byte 0xffffe400
.L_020084c0:
	.4byte 0xffffe800
.L_020084c4:
	.4byte 0xffffec00
.L_020084c8:
	.4byte gIoWriteQueue
.L_020084cc:
	.4byte 0x04000208
.L_020084d0:
	.4byte Data_020020bc
.L_020084d4:
	.4byte 0x050001c0
.L_020084d8:
	.4byte IwramClearWords
.L_020084dc:
	.4byte 0x06001000
.L_020084e0:
	subs r2, r2, r5
	lsls r2, r2, #1
	add r2, r10
	orrs r0, r3
	adds r5, #1
	strh r0, [r2, #62]
	cmp r5, #16
	bne .L_0200847c
	adds r6, #1
	cmp r6, #8
	bne .L_0200847a
	movs r6, #0
.L_020084f8:
	movs r5, #0
.L_020084fa:
	lsls r3, r6, #5
	adds r3, r3, r5
	lsls r3, r3, #1
	movs r0, #128
	ldr r2, .L_0200852c
	add r3, r10
	lsls r0, r0, #3
	adds r3, r3, r0
	adds r5, #1
	strh r2, [r3]
	cmp r5, #32
	bne .L_020084fa
	adds r6, #1
	cmp r6, #16
	bne .L_020084f8
	ldr r1, .L_02008530
	ldr r0, .L_02008534
	ldrh r3, [r0]
	adds r4, r3, #0
	strh r0, [r0]
	ldrh r2, [r1]
	cmp r2, #31
	bgt .L_02008552
	b .L_02008538
	.2byte 0x0000
.L_0200852c:
	.4byte 0x0000e080
.L_02008530:
	.4byte gIoWriteQueue
.L_02008534:
	.4byte 0x04000208
.L_02008538:
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	adds r3, r3, r1
	adds r2, #1
	adds r3, #4
	strh r2, [r1]
	mov r1, r10
	stmia r3!, {r1}
	ldr r2, .L_020085e8
	stmia r3!, {r2}
	ldr r2, .L_020085ec
	str r2, [r3]
.L_02008552:
	strh r4, [r0]
	movs r0, #1
	bl WaitFrames
.L_0200855a:
	bl Func_02002024
	movs r0, #0
	bl Func_02002084
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_020085f0
	bl Scheduler_AddOrUpdateCallback
	movs r0, #246
	bl Func_020020b4
	movs r7, #0
.L_02008576:
	cmp r7, #0
	bne .L_02008656
	mov r2, r11
	ldr r0, .L_020085f4
	cmp r2, #4
	bne .L_02008610
	movs r3, #8
	strh r3, [r0]
	strh r7, [r0, #2]
	ldr r0, .L_020085f8
	ldr r1, .L_020085fc
	ldrh r3, [r1]
	adds r4, r3, #0
	strh r1, [r1]
	ldrh r2, [r0]
	cmp r2, #31
	bgt .L_020085b6
	lsls r3, r2, #1
	adds r3, r3, r2
	adds r2, #1
	lsls r3, r3, #2
	strh r2, [r0]
	ldr r2, .L_02008600
	adds r3, r3, r0
	adds r3, #4
	stmia r3!, {r2}
	ldr r2, .L_02008604
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #24
	adds r2, #16
	str r2, [r3]
.L_020085b6:
	strh r4, [r1]
	ldrh r3, [r1]
	adds r4, r3, #0
	strh r1, [r1]
	ldrh r2, [r0]
	cmp r2, #31
	bgt .L_020085e2
	lsls r3, r2, #1
	adds r3, r3, r2
	adds r2, #1
	lsls r3, r3, #2
	strh r2, [r0]
	ldr r2, .L_02008608
	adds r3, r3, r0
	adds r3, #4
	stmia r3!, {r2}
	ldr r2, .L_0200860c
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #24
	adds r2, #16
	str r2, [r3]
.L_020085e2:
	strh r4, [r1]
	b .L_02008656
	.2byte 0x0000
.L_020085e8:
	.4byte 0x06002000
.L_020085ec:
	.4byte 0x80000400
.L_020085f0:
	.4byte Func_0200005c
.L_020085f4:
	.4byte Data_03001120
.L_020085f8:
	.4byte gIoWriteQueue
.L_020085fc:
	.4byte 0x04000208
.L_02008600:
	.4byte Data_0200213c
.L_02008604:
	.4byte 0x050001c0
.L_02008608:
	.4byte Data_0200215c
.L_0200860c:
	.4byte 0x050001e0
.L_02008610:
	ldr r1, .L_0200874c
	mov r4, r11
	lsls r3, r4, #1
	ldrsb r2, [r1, r3]
	adds r3, #1
	ldrsb r3, [r1, r3]
	ldr r1, .L_02008750
	strh r2, [r0]
	strh r3, [r0, #2]
	ldr r0, .L_02008754
	ldrh r3, [r0]
	adds r4, r3, #0
	strh r0, [r0]
	ldrh r2, [r1]
	cmp r2, #31
	bgt .L_02008654
	lsls r3, r2, #1
	adds r3, r3, r2
	adds r2, #1
	strh r2, [r1]
	lsls r3, r3, #2
	adds r3, r3, r1
	ldr r1, [sp, #8]
	adds r3, #4
	lsls r2, r1, #5
	ldr r1, .L_02008758
	adds r2, r2, r1
	stmia r3!, {r2}
	ldr r2, .L_0200875c
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #24
	adds r2, #16
	str r2, [r3]
.L_02008654:
	strh r4, [r0]
.L_02008656:
	mov r2, r11
	cmp r2, #4
	beq .L_0200865e
	b .L_02008912
.L_0200865e:
	ldr r3, .L_02008760
	movs r5, #0
	movs r4, #16
	ldrsh r1, [r3, r4]
	movs r0, #18
	ldrsh r2, [r3, r0]
	ldr r4, .L_02008764
	ldr r3, .L_02008768
	lsls r1, r1, #16
	lsls r2, r2, #16
	adds r1, r1, r3
	adds r2, r2, r4
	movs r0, #8
	bl Func_02002034
	movs r0, #8
	bl Object_GetById
	movs r1, #1
	bl Animation_SetStateFlags
	movs r1, #0
	movs r0, #8
	bl Object_SetModeById
	movs r0, #8
	bl Object_GetById
	str r5, [r0, #24]
	movs r0, #8
	bl Object_GetById
	str r5, [r0, #28]
	cmp r7, #22
	ble .L_020086c6
	ldr r0, .L_0200876c
	lsls r3, r7, #10
	movs r1, #128
	adds r5, r3, r0
	lsls r1, r1, #9
	cmp r5, r1
	ble .L_020086b6
	movs r5, #128
	lsls r5, r5, #9
.L_020086b6:
	movs r0, #8
	bl Object_GetById
	str r5, [r0, #24]
	movs r0, #8
	bl Object_GetById
	str r5, [r0, #28]
.L_020086c6:
	cmp r7, #31
	bgt .L_02008716
	adds r3, r7, #0
	cmp r7, #0
	bge .L_020086d2
	adds r3, r7, #3
.L_020086d2:
	asrs r1, r3, #2
	cmp r1, #6
	bgt .L_02008710
	ldr r0, .L_02008750
	ldr r4, .L_02008754
	ldrh r3, [r4]
	adds r5, r3, #0
	strh r4, [r4]
	ldrh r3, [r0]
	cmp r3, #31
	bgt .L_0200870e
	lsls r2, r3, #1
	adds r2, r2, r3
	adds r3, #1
	strh r3, [r0]
	lsls r3, r1, #2
	lsls r2, r2, #2
	adds r3, r3, r1
	adds r2, r2, r0
	lsls r3, r3, #8
	movs r0, #128
	add r3, r10
	lsls r0, r0, #6
	adds r2, #4
	adds r3, r3, r0
	stmia r2!, {r3}
	ldr r3, .L_02008770
	stmia r2!, {r3}
	ldr r3, .L_02008774
	str r3, [r2]
.L_0200870e:
	strh r5, [r4]
.L_02008710:
	ldr r2, .L_02008778
	ldr r3, .L_0200873c
	strh r3, [r2]
.L_02008716:
	cmp r7, #32
	bne .L_02008726
	ldr r2, .L_0200877c
	ldr r3, .L_02008740
	strh r3, [r2]
	ldr r2, .L_02008778
	ldr r3, .L_0200873c
	strh r3, [r2]
.L_02008726:
	cmp r7, #36
	bne .L_02008730
	ldr r2, .L_02008778
	ldr r3, .L_02008744
	strh r3, [r2]
.L_02008730:
	cmp r7, #40
	bne .L_02008780
	ldr r2, .L_02008778
	ldr r3, .L_02008748
	strh r3, [r2]
	b .L_02008780
.L_0200873c:
	.4byte 0x00000a00
.L_02008740:
	.4byte 0x00000001
.L_02008744:
	.4byte 0x00000b00
.L_02008748:
	.4byte 0x00000c00
.L_0200874c:
	.4byte Data_02003d9c
.L_02008750:
	.4byte gIoWriteQueue
.L_02008754:
	.4byte 0x04000208
.L_02008758:
	.4byte Data_020020bc
.L_0200875c:
	.4byte 0x050001c0
.L_02008760:
	.4byte Data_02003d34
.L_02008764:
	.4byte 0xffe20000
.L_02008768:
	.4byte 0xffce0000
.L_0200876c:
	.4byte 0xffffac00
.L_02008770:
	.4byte 0x06002000
.L_02008774:
	.4byte 0x84000140
.L_02008778:
	.4byte Data_02003dbc
.L_0200877c:
	.4byte Data_02003dbe
.L_02008780:
	cmp r7, #44
	bne .L_0200878a
	ldr r2, .L_020087bc
	ldr r3, .L_020087ac
	strh r3, [r2]
.L_0200878a:
	cmp r7, #48
	bne .L_02008794
	ldr r2, .L_020087bc
	ldr r3, .L_020087b0
	strh r3, [r2]
.L_02008794:
	cmp r7, #52
	bne .L_0200879e
	ldr r2, .L_020087bc
	ldr r3, .L_020087b4
	strh r3, [r2]
.L_0200879e:
	cmp r7, #56
	bne .L_020087c0
	ldr r2, .L_020087bc
	ldr r3, .L_020087b8
	strh r3, [r2]
	b .L_020087c0
	.2byte 0x0000
.L_020087ac:
	.4byte 0x00000d00
.L_020087b0:
	.4byte 0x00000e00
.L_020087b4:
	.4byte 0x00000f00
.L_020087b8:
	.4byte 0x00001000
.L_020087bc:
	.4byte Data_02003dbc
.L_020087c0:
	cmp r7, #70
	bne .L_020087f4
	ldr r1, .L_020088c8
	ldr r0, .L_020088cc
	ldrh r3, [r0]
	adds r4, r3, #0
	strh r0, [r0]
	ldrh r2, [r1]
	cmp r2, #31
	bgt .L_020087f2
	lsls r3, r2, #1
	adds r3, r3, r2
	adds r2, #1
	lsls r3, r3, #2
	strh r2, [r1]
	movs r2, #134
	adds r3, r3, r1
	lsls r2, r2, #7
	adds r3, #4
	add r2, r10
	stmia r3!, {r2}
	ldr r2, .L_020088d0
	stmia r3!, {r2}
	ldr r2, .L_020088d4
	str r2, [r3]
.L_020087f2:
	strh r4, [r0]
.L_020087f4:
	cmp r7, #72
	bne .L_02008828
	ldr r1, .L_020088c8
	ldr r0, .L_020088cc
	ldrh r3, [r0]
	adds r4, r3, #0
	strh r0, [r0]
	ldrh r2, [r1]
	cmp r2, #31
	bgt .L_02008826
	lsls r3, r2, #1
	adds r3, r3, r2
	adds r2, #1
	lsls r3, r3, #2
	strh r2, [r1]
	movs r2, #144
	adds r3, r3, r1
	lsls r2, r2, #7
	adds r3, #4
	add r2, r10
	stmia r3!, {r2}
	ldr r2, .L_020088d0
	stmia r3!, {r2}
	ldr r2, .L_020088d4
	str r2, [r3]
.L_02008826:
	strh r4, [r0]
.L_02008828:
	cmp r7, #74
	bne .L_0200885c
	ldr r1, .L_020088c8
	ldr r0, .L_020088cc
	ldrh r3, [r0]
	adds r4, r3, #0
	strh r0, [r0]
	ldrh r2, [r1]
	cmp r2, #31
	bgt .L_0200885a
	lsls r3, r2, #1
	adds r3, r3, r2
	adds r2, #1
	lsls r3, r3, #2
	strh r2, [r1]
	movs r2, #154
	adds r3, r3, r1
	lsls r2, r2, #7
	adds r3, #4
	add r2, r10
	stmia r3!, {r2}
	ldr r2, .L_020088d0
	stmia r3!, {r2}
	ldr r2, .L_020088d4
	str r2, [r3]
.L_0200885a:
	strh r4, [r0]
.L_0200885c:
	cmp r7, #76
	bne .L_02008890
	ldr r1, .L_020088c8
	ldr r0, .L_020088cc
	ldrh r3, [r0]
	adds r4, r3, #0
	strh r0, [r0]
	ldrh r2, [r1]
	cmp r2, #31
	bgt .L_0200888e
	lsls r3, r2, #1
	adds r3, r3, r2
	adds r2, #1
	lsls r3, r3, #2
	strh r2, [r1]
	movs r2, #164
	adds r3, r3, r1
	lsls r2, r2, #7
	adds r3, #4
	add r2, r10
	stmia r3!, {r2}
	ldr r2, .L_020088d0
	stmia r3!, {r2}
	ldr r2, .L_020088d4
	str r2, [r3]
.L_0200888e:
	strh r4, [r0]
.L_02008890:
	cmp r7, #153
	ble .L_020088e0
	ldr r2, .L_020088d8
	ldr r3, .L_020088c4
	movs r6, #128
	movs r5, #2
	negs r5, r5
	lsls r6, r6, #1
	movs r0, #160
	strh r3, [r2]
	lsls r0, r0, #19
	adds r1, r6, #0
	adds r2, r5, #0
	adds r3, r5, #0
	str r5, [sp, #0]
	bl Func_020000c0
	ldr r0, .L_020088dc
	adds r1, r6, #0
	adds r2, r5, #0
	adds r3, r5, #0
	str r5, [sp, #0]
	bl Func_020000c0
	b .L_020089d8
	.2byte 0x0000
.L_020088c4:
	.4byte 0x00000000
.L_020088c8:
	.4byte gIoWriteQueue
.L_020088cc:
	.4byte 0x04000208
.L_020088d0:
	.4byte 0x06002000
.L_020088d4:
	.4byte 0x84000140
.L_020088d8:
	.4byte Data_02003dbc
.L_020088dc:
	.4byte 0x05000200
.L_020088e0:
	cmp r7, #79
	ble .L_020089d8
	movs r6, #1
	adds r3, r7, #0
	ands r3, r6
	cmp r3, #0
	beq .L_020089d8
	movs r5, #128
	lsls r5, r5, #1
	movs r0, #160
	lsls r0, r0, #19
	adds r1, r5, #0
	movs r2, #1
	movs r3, #1
	str r6, [sp, #0]
	bl Func_020000c0
	ldr r0, .L_020089fc
	adds r1, r5, #0
	movs r2, #1
	movs r3, #1
	str r6, [sp, #0]
	bl Func_020000c0
	b .L_020089d8
.L_02008912:
	cmp r7, #0
	blt .L_020089d8
	movs r1, #128
	lsls r1, r1, #6
	add r1, r10
	str r1, [sp, #4]
	adds r1, r7, #0
	adds r1, #8
	cmp r1, #63
	bgt .L_020089d8
	movs r2, #0
	mov r8, r2
	movs r2, #128
	ldr r0, [sp, #4]
	lsls r2, r2, #9
	bl Func_02000138
	movs r3, #0
	mov r9, r3
	mov lr, r3
	b .L_020089a6
.L_0200893c:
	mov r2, r8
	movs r0, #0
	add r2, r10
.L_02008942:
	movs r1, #128
	movs r3, #17
	movs r4, #1
	adds r0, #1
	lsls r1, r1, #1
	strb r3, [r2]
	add r8, r4
	adds r2, #1
	cmp r0, r1
	bne .L_02008942
	mov r2, lr
	movs r0, #0
	lsls r4, r2, #3
	b .L_0200899a
.L_0200895e:
	movs r3, #0
	mov r12, r3
	movs r6, #0
.L_02008964:
	mov r1, r8
	movs r5, #0
	add r1, r10
.L_0200896a:
	lsls r2, r5, #1
	adds r3, r4, r6
	adds r2, r2, r3
	ldr r3, [sp, #4]
	adds r5, #1
	adds r2, r3, r2
	ldrb r3, [r2, #1]
	lsls r3, r3, #4
	strb r3, [r1]
	ldrb r2, [r2]
	orrs r3, r2
	movs r2, #1
	strb r3, [r1]
	add r8, r2
	adds r1, #1
	cmp r5, #4
	bne .L_0200896a
	add r12, r2
	mov r3, r12
	adds r6, #64
	cmp r3, #8
	bne .L_02008964
	adds r4, #8
	adds r0, #1
.L_0200899a:
	cmp r0, #8
	bne .L_0200895e
	movs r4, #64
	movs r0, #1
	add lr, r4
	add r9, r0
.L_020089a6:
	mov r1, r9
	cmp r1, #8
	bne .L_0200893c
	ldr r1, .L_02008a00
	ldr r0, .L_02008a04
	ldrh r3, [r0]
	adds r4, r3, #0
	strh r0, [r0]
	ldrh r2, [r1]
	cmp r2, #31
	bgt .L_020089d6
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	adds r2, #1
	adds r3, r3, r1
	adds r3, #4
	strh r2, [r1]
	mov r2, r10
	stmia r3!, {r2}
	ldr r2, .L_02008a08
	stmia r3!, {r2}
	ldr r2, .L_02008a0c
	str r2, [r3]
.L_020089d6:
	strh r4, [r0]
.L_020089d8:
	mov r3, r11
	cmp r3, #4
	beq .L_02008a7e
	movs r4, #32
	negs r4, r4
	adds r4, r4, r7
	ldr r6, .L_02008a10
	mov r8, r4
	cmp r7, #6
	bgt .L_02008a18
	adds r0, r7, #0
	movs r1, #6
	bl __modsi3
	ldr r5, .L_02008a14
	lsls r0, r0, #1
	ldrh r3, [r5, r0]
	b .L_02008a1a
.L_020089fc:
	.4byte 0x05000200
.L_02008a00:
	.4byte gIoWriteQueue
.L_02008a04:
	.4byte 0x04000208
.L_02008a08:
	.4byte 0x06001000
.L_02008a0c:
	.4byte 0x84000400
.L_02008a10:
	.4byte Data_02003dbc
.L_02008a14:
	.4byte Data_02003d8e
.L_02008a18:
	ldr r3, .L_02008a44
.L_02008a1a:
	strh r3, [r6]
	ldr r2, .L_02008a4c
	ldr r3, .L_02008a44
	strh r3, [r2]
	cmp r7, #138
	bne .L_02008a2c
	ldr r2, .L_02008a50
	ldr r3, .L_02008a48
	strh r3, [r2]
.L_02008a2c:
	cmp r7, #64
	ble .L_02008a5c
	movs r2, #1
	adds r3, r7, #0
	ands r3, r2
	cmp r3, #0
	beq .L_02008a5c
	ldr r0, .L_02008a54
	movs r1, #16
	movs r3, #1
	str r2, [sp, #0]
	b .L_02008a58
.L_02008a44:
	.4byte 0x00001000
.L_02008a48:
	.4byte 0x00000001
.L_02008a4c:
	.4byte Data_02003dbc
.L_02008a50:
	.4byte Data_02003dbe
.L_02008a54:
	.4byte 0x050001c0
.L_02008a58:
	bl Func_020000c0
.L_02008a5c:
	cmp r7, #140
	ble .L_02008a70
	movs r3, #2
	negs r3, r3
	ldr r0, .L_02008b50
	movs r1, #16
	adds r2, r3, #0
	str r3, [sp, #0]
	bl Func_020000c0
.L_02008a70:
	mov r0, r8
	cmp r0, #95
	bhi .L_02008a7e
	ldr r2, .L_02008b54
	ldrh r3, [r2, #2]
	adds r3, #1
	strh r3, [r2, #2]
.L_02008a7e:
	movs r0, #1
	adds r7, #1
	bl WaitFrames
	cmp r7, #172
	beq .L_02008a8c
	b .L_02008576
.L_02008a8c:
	ldr r0, .L_02008b58
	bl Scheduler_RemoveCallbackFar
	mov r1, r11
	cmp r1, #4
	beq .L_02008b0e
	movs r0, #192
	movs r1, #192
	lsls r1, r1, #8
	lsls r0, r0, #11
	bl Func_02002044
	bl Func_0200207c
	ldr r2, .L_02008b5c
	mov r4, r11
	lsls r3, r4, #2
	ldrsh r0, [r2, r3]
	adds r3, #2
	ldrsh r2, [r2, r3]
	movs r1, #1
	lsls r0, r0, #16
	lsls r2, r2, #16
	negs r1, r1
	movs r3, #1
	bl Motion_CamBounds
	movs r0, #179
	lsls r0, r0, #8
	adds r0, #51
	movs r1, #60
	bl Func_0200208c
	movs r7, #0
.L_02008ad0:
	ldr r0, [sp, #12]
	movs r1, #143
	lsls r1, r1, #1
	adds r2, r0, r1
	ldrh r3, [r2]
	movs r4, #255
	lsls r4, r4, #8
	adds r4, #224
	adds r3, r3, r4
	strh r3, [r2]
	cmp r7, #60
	bne .L_02008afc
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r0, #218
	lsls r0, r0, #1
	adds r3, r3, r0
	movs r2, #64
	str r2, [r3]
	bl Event_ClearStatus1c6
.L_02008afc:
	movs r0, #1
	adds r7, #1
	bl WaitFrames
	cmp r7, #124
	bne .L_02008ad0
	bl Event_WaitValue1c8Frames
	b .L_02008b16
.L_02008b0e:
	ldr r2, .L_02008b54
	movs r3, #0
	strh r3, [r2]
	strh r3, [r2, #2]
.L_02008b16:
	mov r0, r10
	bl Sys_Free
	bl Func_02001f74
	ldr r3, .L_02008b60
	movs r0, #128
	lsls r0, r0, #19
	ldrh r1, [r3]
	adds r0, #82
	bl QueueIoWriteDelay2
	movs r0, #128
	lsls r0, r0, #19
	movs r1, #0
	adds r0, #80
	bl QueueIoWriteDelay2
	movs r0, #2
	adds r0, #255
	bl GameFlag_SetBit
	add sp, #16
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_02008b50:
	.4byte 0x050001c0
.L_02008b54:
	.4byte Data_03001120
.L_02008b58:
	.4byte Func_0200005c
.L_02008b5c:
	.4byte Data_02003d48
.L_02008b60:
	.4byte Data_02003dbc
	.section .text.x02008b64,"ax",%progbits
	.global Func_02000b64
	.thumb_func
Func_02000b64:
	push {lr}
	movs r0, #162
	lsls r0, r0, #1
	bl GameFlag_SetBit
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #108]
	movs r3, #214
	lsls r3, r3, #1
	adds r2, r1, r3
	movs r3, #0
	str r3, [r2]
	movs r3, #218
	lsls r3, r3, #1
	adds r2, r1, r3
	movs r3, #16
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	movs r0, #128
	movs r1, #128
	lsls r1, r1, #9
	lsls r0, r0, #12
	bl Func_02002044
	movs r0, #48
	adds r0, #255
	bl GameFlag_ClearBit
	bl Func_0200101c
	ldr r3, .L_02008c84
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #6
	bhi .L_02008c80
	ldr r2, .L_02008c88
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
	.2byte 0x0000
.L_02008bc0:
	.4byte .L_02008bdc
	.4byte .L_02008bec
	.4byte .L_02008c02
	.4byte .L_02008c1e
	.4byte .L_02008c3a
	.4byte .L_02008c56
	.4byte .L_02008c72
.L_02008bdc:
	ldr r0, .L_02008c8c
	bl Func_02001f8c
	movs r0, #2
	bl Func_020020ac
	ldr r0, .L_02008c90
	b .L_02008bfa
.L_02008bec:
	ldr r0, .L_02008c8c
	bl Func_02001f8c
	movs r0, #3
	bl Func_020020ac
	ldr r0, .L_02008c94
.L_02008bfa:
	movs r1, #2
	bl Func_02002054
	b .L_02008c80
.L_02008c02:
	movs r0, #0
	bl Func_0200020c
	ldr r0, .L_02008c8c
	bl Func_02001f8c
	movs r0, #4
	bl Func_020020ac
	ldr r0, .L_02008c94
	movs r1, #3
	bl Func_02002054
	b .L_02008c80
.L_02008c1e:
	movs r0, #1
	bl Func_0200020c
	ldr r0, .L_02008c8c
	bl Func_02001f8c
	movs r0, #5
	bl Func_020020ac
	ldr r0, .L_02008c94
	movs r1, #4
	bl Func_02002054
	b .L_02008c80
.L_02008c3a:
	movs r0, #2
	bl Func_0200020c
	ldr r0, .L_02008c8c
	bl Func_02001f8c
	movs r0, #6
	bl Func_020020ac
	ldr r0, .L_02008c94
	movs r1, #5
	bl Func_02002054
	b .L_02008c80
.L_02008c56:
	movs r0, #3
	bl Func_0200020c
	ldr r0, .L_02008c8c
	bl Func_02001f8c
	movs r0, #7
	bl Func_020020ac
	ldr r0, .L_02008c94
	movs r1, #6
	bl Func_02002054
	b .L_02008c80
.L_02008c72:
	movs r0, #4
	bl Func_0200020c
	ldr r0, .L_02008c90
	movs r1, #3
	bl Func_02002054
.L_02008c80:
	movs r0, #0
	pop {pc}
.L_02008c84:
	.4byte gPartyState
.L_02008c88:
	.4byte .L_02008bc0
.L_02008c8c:
	.4byte 0x0000000a
.L_02008c90:
	.4byte 0x00000137
.L_02008c94:
	.4byte 0x00000003
	.section .text.x02008c9c,"ax",%progbits
	.global Func_02000c9c
	.thumb_func
Func_02000c9c:
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	movs r2, #143
	lsls r2, r2, #1
	adds r3, r3, r2
	ldr r1, [r0, #80]
	ldrh r3, [r3]
	ldr r2, .L_02008cb8
	strh r3, [r1, #18]
	strb r2, [r1, #26]
	movs r0, #1
	bx lr
	.2byte 0x0000
.L_02008cb8:
	.4byte 0x00000000
	.section .text.x02008cbc,"ax",%progbits
	.global Func_02000cbc
	.thumb_func
Func_02000cbc:
	push {r5, r6, r7, lr}
	adds r6, r2, #0
	ldr r2, .L_02008d04
	ldr r3, .L_02008d08
	lsls r0, r0, #2
	adds r0, r0, r2
	ldr r5, [r3, r0]
	ldr r3, .L_02008d0c
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	adds r7, r1, #0
	bl Object_GetById
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #108]
	ldr r2, [r0, #8]
	ldr r3, [r5, #8]
	cmp r2, r3
	bge .L_02008cf2
	movs r2, #172
	lsls r2, r2, #1
	adds r3, r1, r2
	strh r7, [r3]
	b .L_02008cfa
.L_02008cf2:
	movs r2, #172
	lsls r2, r2, #1
	adds r3, r1, r2
	strh r6, [r3]
.L_02008cfa:
	movs r0, #123
	bl Func_020020b4
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008d04:
	.4byte 0xfffffe70
.L_02008d08:
	.4byte Data_02003dc0
.L_02008d0c:
	.4byte gPartyState
	.section .text.x02008d10,"ax",%progbits
	.global Func_02000d10
	.thumb_func
Func_02000d10:
	push {r5, r6, r7, lr}
	adds r6, r2, #0
	ldr r2, .L_02008d58
	ldr r3, .L_02008d5c
	lsls r0, r0, #2
	adds r0, r0, r2
	ldr r5, [r3, r0]
	ldr r3, .L_02008d60
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	adds r7, r1, #0
	bl Object_GetById
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #108]
	ldr r2, [r0, #16]
	ldr r3, [r5, #16]
	cmp r2, r3
	bge .L_02008d46
	movs r2, #172
	lsls r2, r2, #1
	adds r3, r1, r2
	strh r7, [r3]
	b .L_02008d4e
.L_02008d46:
	movs r2, #172
	lsls r2, r2, #1
	adds r3, r1, r2
	strh r6, [r3]
.L_02008d4e:
	movs r0, #123
	bl Func_020020b4
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008d58:
	.4byte 0xfffffe70
.L_02008d5c:
	.4byte Data_02003dc0
.L_02008d60:
	.4byte gPartyState
	.section .text.x02008d64,"ax",%progbits
	.global Func_02000d64
	.thumb_func
Func_02000d64:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #24
	bl Func_02002094
	mov r8, r0
	bl Func_0200209c
	bl Object_GetById
	ldr r3, [r0, #80]
	ldrh r2, [r0, #32]
	ldr r3, [r3, #12]
	mov r10, r0
	adds r0, r2, #0
	muls r0, r3
	str r0, [sp, #20]
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #108]
	movs r2, #230
	mov r9, r1
	lsls r2, r2, #1
	add r2, r9
	ldr r3, [r3, #32]
	ldr r6, [r2]
	movs r2, #150
	lsls r2, r2, #4
	adds r3, r3, r2
	mov r5, r8
	ldr r3, [r3]
	ldrb r2, [r5]
	movs r5, #128
	lsls r5, r5, #9
	ldr r1, [r6, #8]
	ldr r0, [r6, #16]
	ldr r4, .L_02008f80
	cmp r3, r5
	bgt .L_02008dda
	ldr r3, .L_02008f84
	movs r5, #160
	lsls r5, r5, #16
	adds r3, r1, r3
	adds r5, r1, r5
	ldr r1, .L_02008f88
	str r3, [sp, #12]
	movs r3, #200
	lsls r3, r3, #16
	adds r1, r0, r1
	adds r3, r0, r3
	str r5, [sp, #8]
	str r1, [sp, #4]
	str r3, [sp, #0]
	b .L_02008df6
.L_02008dda:
	ldr r5, .L_02008f8c
	movs r3, #240
	adds r5, r1, r5
	str r5, [sp, #12]
	lsls r3, r3, #16
	ldr r5, .L_02008f90
	adds r3, r1, r3
	movs r1, #150
	lsls r1, r1, #17
	adds r5, r0, r5
	adds r1, r0, r1
	str r3, [sp, #8]
	str r5, [sp, #4]
	str r1, [sp, #0]
.L_02008df6:
	movs r3, #0
	str r3, [sp, #16]
	adds r3, r2, #0
	mov r11, r4
	cmp r3, #0
	bne .L_02008e04
	b .L_02008f72
.L_02008e04:
	mov r5, r8
	ldrb r3, [r5, #1]
	cmp r3, #15
	bne .L_02008e0e
	b .L_02008f56
.L_02008e0e:
	movs r1, #4
	ldrsh r3, [r5, r1]
	mov r0, r11
	ldr r6, [r0]
	mov r0, r8
	lsls r5, r3, #16
	ldr r1, [sp, #12]
	movs r2, #6
	ldrsh r3, [r0, r2]
	lsls r7, r3, #16
	cmp r5, r1
	bgt .L_02008e28
	b .L_02008f46
.L_02008e28:
	ldr r2, [sp, #8]
	cmp r5, r2
	blt .L_02008e30
	b .L_02008f46
.L_02008e30:
	ldr r3, [sp, #4]
	cmp r7, r3
	bgt .L_02008e38
	b .L_02008f46
.L_02008e38:
	ldr r0, [sp, #0]
	cmp r7, r0
	blt .L_02008e40
	b .L_02008f46
.L_02008e40:
	mov r2, r8
	movs r1, #10
	ldrsh r0, [r2, r1]
	movs r3, #1
	negs r3, r3
	movs r2, #1
	cmp r0, r3
	beq .L_02008e70
	movs r3, #128
	lsls r3, r3, #5
	ands r3, r0
	cmp r3, #0
	beq .L_02008e62
	bl GameFlag_Test
	adds r2, r0, #0
	b .L_02008e70
.L_02008e62:
	bl GameFlag_Test
	negs r3, r0
	orrs r3, r0
	lsrs r3, r3, #31
	movs r2, #1
	subs r2, r2, r3
.L_02008e70:
	cmp r2, #0
	beq .L_02008f46
	cmp r6, #0
	bne .L_02008ec4
	mov r2, r8
	movs r1, #2
	ldrsh r0, [r2, r1]
	adds r3, r7, #0
	adds r1, r5, #0
	movs r2, #0
	bl Func_02001fcc
	adds r6, r0, #0
	cmp r6, #0
	beq .L_02008f56
	ldr r1, .L_02008f94
	bl Func_02001fc4
	adds r2, r6, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	mov r5, r8
	adds r2, #4
	movs r3, #1
	strb r3, [r2]
	ldrh r3, [r5, #8]
	adds r0, r6, #0
	strh r3, [r6, #6]
	movs r1, #1
	bl Func_02001fbc
	ldr r5, [r6, #80]
	movs r1, #192
	ldr r0, [r5, #12]
	ldr r3, .L_02008f98
	lsls r1, r1, #8
	mov lr, r3
	.2byte 0xf800
	str r0, [r5, #12]
	mov r0, r11
	str r6, [r0]
.L_02008ec4:
	ldr r3, .L_02008f9c
	movs r1, #128
	lsls r1, r1, #2
	adds r1, #118
	adds r3, r3, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	bne .L_02008f56
	ldr r3, [r6, #80]
	mov r5, r10
	ldr r0, [r3, #12]
	ldr r2, [r6, #8]
	ldr r3, [r5, #8]
	subs r1, r2, r3
	cmp r1, #0
	bge .L_02008ee8
	subs r1, r3, r2
.L_02008ee8:
	ldrh r3, [r6, #32]
	muls r3, r0
	ldr r0, [sp, #20]
	adds r2, r0, r3
	mov r3, r10
	ldr r0, [r6, #16]
	ldr r4, [r3, #16]
	subs r3, r0, r4
	cmp r3, #0
	blt .L_02008f04
	adds r3, r1, r3
	cmp r3, r2
	blt .L_02008f0c
	b .L_02008f56
.L_02008f04:
	subs r3, r4, r0
	adds r3, r1, r3
	cmp r3, r2
	bge .L_02008f56
.L_02008f0c:
	movs r0, #130
	lsls r0, r0, #1
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008f56
	ldr r3, .L_02008f9c
	movs r5, #128
	lsls r5, r5, #2
	adds r5, #18
	adds r3, r3, r5
	ldrb r3, [r3]
	cmp r3, #9
	bne .L_02008f38
	movs r2, #179
	movs r3, #128
	lsls r2, r2, #1
	lsls r3, r3, #6
	add r2, r9
	adds r3, #139
	strh r3, [r2]
	b .L_02008f56
.L_02008f38:
	ldr r2, [sp, #16]
	movs r3, #170
	lsls r3, r3, #1
	adds r2, #100
	add r3, r9
	strh r2, [r3]
	b .L_02008f56
.L_02008f46:
	cmp r6, #0
	beq .L_02008f56
	adds r0, r6, #0
	bl Func_02001fd4
	movs r3, #0
	mov r0, r11
	str r3, [r0]
.L_02008f56:
	ldr r1, [sp, #16]
	movs r2, #4
	adds r1, #1
	movs r3, #20
	str r1, [sp, #16]
	add r11, r2
	add r8, r3
	cmp r1, #127
	bhi .L_02008f72
	mov r5, r8
	ldrb r3, [r5]
	cmp r3, #0
	beq .L_02008f72
	b .L_02008e04
.L_02008f72:
	add sp, #24
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_02008f80:
	.4byte Data_02003dc0
.L_02008f84:
	.4byte 0xff600000
.L_02008f88:
	.4byte 0xfed40000
.L_02008f8c:
	.4byte 0xff100000
.L_02008f90:
	.4byte 0xfe3e0000
.L_02008f94:
	.4byte Data_02003da8
.L_02008f98:
	.4byte IwramMulQ16
.L_02008f9c:
	.4byte gPartyState
	.section .text.x02008fa0,"ax",%progbits
	.global Func_02000fa0
	.thumb_func
Func_02000fa0:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #24]
	ldrh r3, [r3, #4]
	cmp r3, #0
	bne .L_02009012
	ldr r0, .L_02009014
	ldr r1, .L_02009018
	ldrh r3, [r1]
	adds r4, r3, #0
	strh r1, [r1]
	ldrh r2, [r0]
	cmp r2, #31
	bgt .L_02008fe2
	lsls r3, r2, #1
	adds r3, r3, r2
	adds r2, #1
	lsls r3, r3, #2
	strh r2, [r0]
	movs r2, #252
	adds r3, r3, r0
	lsls r2, r2, #6
	adds r3, #4
	adds r2, #142
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #80
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #10
	str r2, [r3]
.L_02008fe2:
	strh r4, [r1]
	ldrh r3, [r1]
	adds r4, r3, #0
	strh r1, [r1]
	ldrh r2, [r0]
	cmp r2, #31
	bgt .L_02009010
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	adds r2, #1
	adds r3, r3, r0
	adds r3, #4
	strh r2, [r0]
	movs r2, #12
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #84
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #10
	str r2, [r3]
.L_02009010:
	strh r4, [r1]
.L_02009012:
	pop {pc}
.L_02009014:
	.4byte gIoWriteQueue
.L_02009018:
	.4byte 0x04000208
	.section .text.x0200901c,"ax",%progbits
	.global Func_0200101c
	.thumb_func
Func_0200101c:
	push {lr}
	movs r1, #128
	ldr r3, .L_02009038
	lsls r1, r1, #2
	ldr r0, .L_0200903c
	mov lr, r3
	.2byte 0xf800
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_02009040
	bl Scheduler_AddOrUpdateCallback
	pop {pc}
	.2byte 0x0000
.L_02009038:
	.4byte IwramClearWords
.L_0200903c:
	.4byte Data_02003dc0
.L_02009040:
	.4byte Func_02000d64
	.section .text.x02009044,"ax",%progbits
	.global Func_02001044
	.thumb_func
Func_02001044:
	push {lr}
	cmp r0, #0
	bge .L_0200904e
	ldr r2, .L_02009060
	adds r0, r0, r2
.L_0200904e:
	asrs r3, r0, #20
	cmp r1, #0
	bge .L_02009058
	ldr r2, .L_02009060
	adds r1, r1, r2
.L_02009058:
	asrs r0, r1, #20
	lsls r0, r0, #7
	adds r0, r3, r0
	pop {pc}
.L_02009060:
	.4byte 0x000fffff
	.section .text.x02009064,"ax",%progbits
	.global Func_02001064
	.thumb_func
Func_02001064:
	push {r5, r6, lr}
	adds r5, r0, #0
	adds r2, r5, #0
	adds r2, #100
	ldrh r3, [r2]
	ldr r6, [r5, #104]
	adds r3, #1
	strh r3, [r2]
	lsls r3, r3, #16
	asrs r0, r3, #16
	cmp r0, #31
	ble .L_02009080
	movs r0, #0
	b .L_020090a8
.L_02009080:
	lsls r0, r0, #10
	bl Math_Sine
	movs r1, #160
	ldr r3, .L_020090ac
	lsls r1, r1, #9
	mov lr, r3
	.2byte 0xf800
	str r0, [r5, #24]
	str r0, [r5, #28]
	movs r2, #128
	ldr r3, [r6, #8]
	lsls r2, r2, #9
	str r3, [r5, #8]
	ldr r3, [r5, #12]
	movs r0, #1
	adds r3, r3, r2
	str r3, [r5, #12]
	ldr r3, [r6, #16]
	str r3, [r5, #16]
.L_020090a8:
	pop {r5, r6, pc}
	.2byte 0x0000
.L_020090ac:
	.4byte IwramMulQ16
	.section .text.x020090b0,"ax",%progbits
	.global Func_020010b0
	.thumb_func
Func_020010b0:
	push {r5, r6, r7, lr}
	adds r7, r0, #0
	movs r1, #192
	ldr r3, .L_02009138
	lsls r1, r1, #9
	ldr r0, [r7, #24]
	mov lr, r3
	.2byte 0xf800
	movs r0, #16
	ldr r1, [r7, #8]
	ldr r2, [r7, #12]
	ldr r3, [r7, #16]
	adds r0, #255
	bl Func_02001fcc
	adds r5, r0, #0
	cmp r5, #0
	beq .L_0200910c
	ldr r3, [r7, #20]
	ldr r1, .L_0200913c
	str r3, [r5, #20]
	ldr r6, [r5, #80]
	bl Func_02001fc4
	adds r3, r5, #0
	adds r3, #85
	movs r2, #0
	strb r2, [r3]
	adds r3, #15
	strh r2, [r3]
	str r7, [r5, #104]
	cmp r6, #0
	beq .L_0200910c
	adds r0, r6, #0
	movs r1, #2
	bl Animation_ApplyChildArgument
	ldr r3, .L_02009134
	ldrb r2, [r6, #9]
	strb r3, [r6, #26]
	movs r3, #13
	negs r3, r3
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	strb r3, [r6, #9]
.L_0200910c:
	movs r0, #16
	ldr r1, [r7, #8]
	ldr r2, [r7, #12]
	ldr r3, [r7, #16]
	adds r0, #255
	bl Func_02001fcc
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02009160
	ldr r3, [r7, #20]
	ldr r1, .L_0200913c
	str r3, [r5, #20]
	ldr r6, [r5, #80]
	bl Func_02001fc4
	adds r3, r5, #0
	movs r2, #0
	adds r3, #85
	b .L_02009140
.L_02009134:
	.4byte 0x00000000
.L_02009138:
	.4byte IwramMulQ16
.L_0200913c:
	.4byte Data_02002e74
.L_02009140:
	strb r2, [r3]
	adds r3, #15
	strh r2, [r3]
	adds r2, r5, #0
	adds r2, #34
	movs r3, #1
	str r7, [r5, #104]
	strb r3, [r2]
	cmp r6, #0
	beq .L_02009160
	adds r0, r6, #0
	movs r1, #1
	bl Animation_ApplyChildArgument
	ldr r3, .L_02009164
	strb r3, [r6, #26]
.L_02009160:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009164:
	.4byte 0x00000000
	.section .text.x02009168,"ax",%progbits
	.global Func_02001168
	.thumb_func
Func_02001168:
	push {r5, r6, r7, lr}
	adds r7, r0, #0
	movs r1, #192
	lsls r1, r1, #9
	ldr r3, .L_020091ec
	ldr r0, [r7, #24]
	sub sp, #12
	mov lr, r3
	.2byte 0xf800
	ldr r3, [r7, #8]
	mov r5, sp
	str r3, [r5]
	ldr r3, [r7, #12]
	movs r2, #128
	lsls r2, r2, #10
	adds r3, r3, r2
	str r3, [r5, #4]
	ldr r3, [r7, #16]
	str r3, [r5, #8]
	bl Random16Far
	adds r6, r0, #0
	bl Random16Far
	adds r1, r0, #0
	lsls r0, r6, #2
	adds r0, r0, r6
	adds r2, r5, #0
	lsls r0, r0, #2
	bl Vector_AddPolarOffsetFar
	movs r0, #128
	lsls r0, r0, #2
	ldr r1, [r5]
	ldr r2, [r5, #4]
	ldr r3, [r5, #8]
	adds r0, #162
	bl Func_02001fcc
	adds r5, r0, #0
	cmp r5, #0
	beq .L_020091e8
	ldr r3, [r7, #20]
	ldr r6, [r5, #80]
	str r3, [r5, #20]
	ldr r1, .L_020091f0
	bl Func_02001fc4
	adds r0, r6, #0
	movs r1, #2
	bl Animation_ApplyChildArgument
	movs r3, #128
	lsls r3, r3, #2
	str r3, [r5, #72]
	movs r3, #204
	lsls r3, r3, #8
	adds r3, #204
	str r3, [r5, #24]
	str r3, [r5, #28]
	cmp r6, #0
	beq .L_020091e8
	movs r3, #0
	strb r3, [r6, #26]
.L_020091e8:
	add sp, #12
	pop {r5, r6, r7, pc}
.L_020091ec:
	.4byte IwramMulQ16
.L_020091f0:
	.4byte Data_02002e80
	.section .text.x020091f4,"ax",%progbits
	.global Func_020011f4
	.thumb_func
Func_020011f4:
	push {r5, lr}
	ldr r3, .L_02009228
	movs r2, #2
	ldr r3, [r3]
	adds r5, r0, #0
	ands r3, r2
	cmp r3, #0
	beq .L_0200920c
	movs r1, #7
	bl Animation_ApplyChildValues
	b .L_02009214
.L_0200920c:
	adds r0, r5, #0
	movs r1, #0
	bl Animation_ApplyChildValues
.L_02009214:
	ldr r3, .L_02009228
	movs r2, #15
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	bne .L_02009226
	adds r0, r5, #0
	bl Func_020010b0
.L_02009226:
	pop {r5, pc}
.L_02009228:
	.4byte gFrameCount
	.section .text.x0200922c,"ax",%progbits
	.global Func_0200122c
	.thumb_func
Func_0200122c:
	push {r5, r6, r7, lr}
	ldr r7, .L_02009268
	movs r2, #1
	ldr r3, [r7]
	adds r6, r0, #0
	ands r3, r2
	cmp r3, #0
	beq .L_02009254
	ldr r5, .L_0200926c
	ldr r2, .L_02009270
	ldr r3, [r5]
	lsls r3, r3, #2
	ldr r1, [r2, r3]
	bl Animation_ApplyChildValues
	ldr r3, [r5]
	movs r2, #3
	adds r3, #1
	ands r3, r2
	str r3, [r5]
.L_02009254:
	ldr r3, [r7]
	movs r2, #3
	ands r3, r2
	cmp r3, #0
	bne .L_02009264
	adds r0, r6, #0
	bl Func_02001168
.L_02009264:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009268:
	.4byte gFrameCount
.L_0200926c:
	.4byte Data_02003db8
.L_02009270:
	.4byte Data_02002eac
	.section .text.x02009274,"ax",%progbits
	.global Func_02001274
	.thumb_func
Func_02001274:
	push {lr}
	cmp r1, #0
	bne .L_0200928c
	str r1, [r0, #108]
	movs r1, #0
	bl Animation_ApplyChildValues
	movs r0, #195
	lsls r0, r0, #1
	bl Func_020020b4
	b .L_020092a2
.L_0200928c:
	cmp r1, #1
	bne .L_02009296
	ldr r3, .L_020092a4
	str r3, [r0, #108]
	b .L_020092a2
.L_02009296:
	ldr r3, .L_020092a8
	str r3, [r0, #108]
	movs r0, #36
	adds r0, #255
	bl Func_020020b4
.L_020092a2:
	pop {pc}
.L_020092a4:
	.4byte Func_020011f4
.L_020092a8:
	.4byte Func_0200122c
	.section .text.x020092ac,"ax",%progbits
	.global Func_020012ac
	.thumb_func
Func_020012ac:
	push {r5, r6, lr}
	bl Func_0200209c
	bl Object_GetById
	adds r5, r0, #0
	movs r0, #212
	bl Func_020020b4
	adds r0, r5, #0
	movs r1, #1
	bl Func_02001274
	movs r6, #0
	b .L_020092d2
.L_020092ca:
	movs r0, #1
	bl WaitFrames
	adds r6, #1
.L_020092d2:
	cmp r6, #19
	bgt .L_020092ee
	adds r0, r5, #0
	adds r0, #8
	bl Func_02002004
	adds r1, r0, #0
	cmp r1, #0
	beq .L_020092ca
	movs r0, #2
	bl Func_020020a4
	cmp r0, #0
	beq .L_020092ca
.L_020092ee:
	adds r0, r5, #0
	movs r1, #2
	bl Func_02001274
	pop {r5, r6, pc}
	.section .text.x020092f8,"ax",%progbits
	.global Func_020012f8
	.thumb_func
Func_020012f8:
	push {lr}
	bl Func_0200209c
	bl Object_GetById
	movs r1, #0
	bl Func_02001274
	pop {pc}
	.2byte 0x0000
	.section .text.x0200930c,"ax",%progbits
	.global Func_0200130c
	.thumb_func
Func_0200130c:
	push {lr}
	movs r0, #128
	movs r1, #252
	lsls r0, r0, #19
	lsls r1, r1, #6
	adds r0, #80
	adds r1, #65
	bl QueueIoWriteDelay2
	ldr r3, .L_02009344
	movs r2, #2
	ldr r3, [r3]
	ands r3, r2
	ldr r2, .L_02009348
	cmp r3, #0
	beq .L_0200934c
	ldrh r3, [r2]
	ldr r1, .L_02009340
	movs r0, #128
	lsls r0, r0, #19
	orrs r1, r3
	adds r0, #82
	bl QueueIoWriteDelay2
	b .L_0200935c
	.2byte 0x0000
.L_02009340:
	.4byte 0x0000000c
.L_02009344:
	.4byte gFrameCount
.L_02009348:
	.4byte Data_02003fc0
.L_0200934c:
	ldrh r3, [r2]
	ldr r1, .L_02009360
	movs r0, #128
	lsls r0, r0, #19
	orrs r1, r3
	adds r0, #82
	bl QueueIoWriteDelay2
.L_0200935c:
	pop {pc}
	.2byte 0x0000
.L_02009360:
	.4byte 0x00000010
	.section .text.x02009364,"ax",%progbits
	.global Func_02001364
	.thumb_func
Func_02001364:
	push {r5, r6, lr}
	ldr r3, .L_020093f4
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	movs r3, #10
	ldrsh r5, [r0, r3]
	ldr r3, .L_020093f8
	movs r2, #18
	ldrsh r6, [r0, r2]
	movs r1, #3
	ldr r0, [r3]
	bl __umodsi3
	cmp r0, #0
	bne .L_020093f0
	bl Random16Far
	lsls r0, r0, #2
	lsrs r0, r0, #16
	cmp r0, #1
	beq .L_020093b4
	cmp r0, #1
	bcc .L_020093a4
	cmp r0, #2
	beq .L_020093c4
	cmp r0, #3
	beq .L_020093dc
	b .L_020093f0
.L_020093a4:
	ldr r3, .L_020093fc
	lsls r0, r5, #16
	adds r0, r0, r3
	movs r3, #128
	lsls r3, r3, #9
	lsls r2, r6, #16
	movs r1, #1
	b .L_020093d0
.L_020093b4:
	ldr r3, .L_020093fc
	movs r2, #128
	lsls r2, r2, #9
	lsls r0, r5, #16
	adds r0, r0, r2
	movs r1, #1
	lsls r2, r6, #16
	b .L_020093d0
.L_020093c4:
	movs r3, #128
	lsls r3, r3, #9
	lsls r0, r5, #16
	lsls r2, r6, #16
	movs r1, #1
	adds r0, r0, r3
.L_020093d0:
	adds r2, r2, r3
	negs r1, r1
	movs r3, #1
	bl Motion_CamBounds
	b .L_020093f0
.L_020093dc:
	ldr r3, .L_020093fc
	lsls r0, r5, #16
	lsls r2, r6, #16
	movs r1, #1
	adds r0, r0, r3
	adds r2, r2, r3
	negs r1, r1
	movs r3, #1
	bl Motion_CamBounds
.L_020093f0:
	pop {r5, r6, pc}
	.2byte 0x0000
.L_020093f4:
	.4byte gPartyState
.L_020093f8:
	.4byte gFrameCount
.L_020093fc:
	.4byte 0xffff0000
	.section .text.x02009400,"ax",%progbits
	.global Func_02001400
	.thumb_func
Func_02001400:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	mov r10, r0
	movs r0, #1
	bl WaitFrames
	movs r6, #192
	movs r0, #10
	adds r0, #255
	lsls r6, r6, #18
	bl GameFlag_ClearBit
	ldr r3, [r6, #108]
	movs r0, #214
	lsls r0, r0, #1
	adds r3, r3, r0
	movs r5, #0
	str r5, [r3]
	ldr r3, .L_02009544
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	ldr r0, [r3]
	bl Object_GetById
	adds r0, #84
	strb r5, [r0]
	movs r2, #218
	ldr r3, [r6, #108]
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #16
	str r2, [r3]
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	mov r3, r10
	cmp r3, #2
	bne .L_02009470
	movs r0, #128
	movs r1, #192
	movs r2, #128
	lsls r0, r0, #10
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl Func_02001ff4
	movs r0, #141
	bl Func_020020b4
.L_02009470:
	bl Func_02002074
	movs r0, #128
	lsls r0, r0, #7
	bl Runtime_BumpAllocateAlternatePool
	adds r7, r0, #0
	adds r1, r7, #0
	ldr r0, .L_02009548
	bl Func_02001f54
	movs r2, #128
	lsls r2, r2, #5
	adds r1, r7, r2
	ldr r0, .L_0200954c
	bl Func_02001f54
	ldr r6, .L_02009550
	ldr r5, .L_02009554
	ldrh r3, [r5]
	adds r0, r3, #0
	strh r5, [r5]
	ldrh r2, [r6]
	cmp r2, #31
	bgt .L_020094c6
	lsls r3, r2, #1
	adds r3, r3, r2
	mov r1, r10
	adds r2, #1
	strh r2, [r6]
	lsls r2, r1, #5
	ldr r1, .L_02009558
	lsls r3, r3, #2
	adds r3, r3, r6
	adds r3, #4
	adds r2, r2, r1
	stmia r3!, {r2}
	ldr r2, .L_0200955c
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #24
	adds r2, #16
	str r2, [r3]
.L_020094c6:
	strh r0, [r5]
	ldrh r3, [r5]
	adds r1, r3, #0
	strh r5, [r5]
	ldrh r2, [r6]
	cmp r2, #31
	bgt .L_020094ec
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	adds r3, r3, r6
	adds r3, #4
	adds r2, #1
	stmia r3!, {r7}
	strh r2, [r6]
	ldr r2, .L_02009560
	stmia r3!, {r2}
	ldr r2, .L_02009564
	str r2, [r3]
.L_020094ec:
	strh r1, [r5]
	ldr r0, .L_02009568
	movs r1, #144
	lsls r1, r1, #3
	bl Scheduler_AddOrUpdateCallback
	bl Func_02002024
	movs r0, #0
	bl Func_02002084
	movs r0, #246
	bl Func_020020b4
	ldr r2, .L_0200956c
	ldr r3, .L_02009540
	mov r8, r2
	mov r0, r8
	strh r3, [r0]
	ldrh r3, [r5]
	adds r1, r3, #0
	strh r5, [r5]
	ldrh r2, [r6]
	cmp r2, #31
	bgt .L_02009578
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	movs r0, #210
	adds r2, #1
	adds r3, r3, r6
	lsls r0, r0, #6
	adds r3, #4
	strh r2, [r6]
	adds r2, r7, r0
	stmia r3!, {r2}
	ldr r2, .L_02009570
	stmia r3!, {r2}
	ldr r2, .L_02009574
	str r2, [r3]
	b .L_02009578
	.2byte 0x0000
.L_02009540:
	.4byte 0x00000e00
.L_02009544:
	.4byte gPartyState
.L_02009548:
	.4byte Data_020031f6
.L_0200954c:
	.4byte Data_02002f3c
.L_02009550:
	.4byte gIoWriteQueue
.L_02009554:
	.4byte 0x04000208
.L_02009558:
	.4byte Data_02002ebc
.L_0200955c:
	.4byte 0x050001c0
.L_02009560:
	.4byte 0x06001000
.L_02009564:
	.4byte 0x84000400
.L_02009568:
	.4byte Func_0200130c
.L_0200956c:
	.4byte Data_02003fc0
.L_02009570:
	.4byte 0x06002000
.L_02009574:
	.4byte 0x84000140
.L_02009578:
	strh r1, [r5]
	movs r0, #2
	bl Battle_WaitMode0
	ldr r3, .L_020095b4
	mov r1, r8
	strh r3, [r1]
	ldrh r3, [r5]
	adds r1, r3, #0
	strh r5, [r5]
	ldrh r2, [r6]
	cmp r2, #31
	bgt .L_020095c0
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	movs r0, #186
	adds r2, #1
	adds r3, r3, r6
	lsls r0, r0, #6
	adds r3, #4
	strh r2, [r6]
	adds r2, r7, r0
	stmia r3!, {r2}
	ldr r2, .L_020095b8
	stmia r3!, {r2}
	ldr r2, .L_020095bc
	str r2, [r3]
	b .L_020095c0
	.2byte 0x0000
.L_020095b4:
	.4byte 0x00000d00
.L_020095b8:
	.4byte 0x06002000
.L_020095bc:
	.4byte 0x84000140
.L_020095c0:
	strh r1, [r5]
	movs r0, #2
	bl Battle_WaitMode0
	ldr r3, .L_020095fc
	mov r1, r8
	strh r3, [r1]
	ldrh r3, [r5]
	adds r1, r3, #0
	strh r5, [r5]
	ldrh r2, [r6]
	cmp r2, #31
	bgt .L_02009608
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	movs r0, #162
	adds r2, #1
	adds r3, r3, r6
	lsls r0, r0, #6
	adds r3, #4
	strh r2, [r6]
	adds r2, r7, r0
	stmia r3!, {r2}
	ldr r2, .L_02009600
	stmia r3!, {r2}
	ldr r2, .L_02009604
	str r2, [r3]
	b .L_02009608
	.2byte 0x0000
.L_020095fc:
	.4byte 0x00000c00
.L_02009600:
	.4byte 0x06002000
.L_02009604:
	.4byte 0x84000140
.L_02009608:
	strh r1, [r5]
	movs r0, #2
	bl Battle_WaitMode0
	movs r1, #176
	lsls r1, r1, #4
	mov r2, r8
	strh r1, [r2]
	ldrh r3, [r5]
	adds r1, r3, #0
	strh r5, [r5]
	ldrh r2, [r6]
	cmp r2, #31
	bgt .L_02009642
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	movs r0, #138
	adds r2, #1
	adds r3, r3, r6
	lsls r0, r0, #6
	adds r3, #4
	strh r2, [r6]
	adds r2, r7, r0
	stmia r3!, {r2}
	ldr r2, .L_020096fc
	stmia r3!, {r2}
	ldr r2, .L_02009700
	str r2, [r3]
.L_02009642:
	strh r1, [r5]
	movs r0, #2
	bl Battle_WaitMode0
	movs r1, #160
	lsls r1, r1, #4
	mov r11, r1
	mov r2, r11
	mov r3, r8
	strh r2, [r3]
	ldrh r3, [r5]
	adds r1, r3, #0
	strh r5, [r5]
	ldrh r2, [r6]
	cmp r2, #31
	bgt .L_02009680
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	movs r0, #228
	adds r2, #1
	adds r3, r3, r6
	lsls r0, r0, #5
	adds r3, #4
	strh r2, [r6]
	adds r2, r7, r0
	stmia r3!, {r2}
	ldr r2, .L_020096fc
	stmia r3!, {r2}
	ldr r2, .L_02009700
	str r2, [r3]
.L_02009680:
	strh r1, [r5]
	movs r0, #2
	bl Battle_WaitMode0
	movs r1, #144
	lsls r1, r1, #4
	mov r9, r1
	mov r2, r9
	mov r3, r8
	strh r2, [r3]
	ldrh r3, [r5]
	adds r1, r3, #0
	strh r5, [r5]
	ldrh r2, [r6]
	cmp r2, #31
	bgt .L_020096be
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	movs r0, #180
	adds r2, #1
	adds r3, r3, r6
	lsls r0, r0, #5
	adds r3, #4
	strh r2, [r6]
	adds r2, r7, r0
	stmia r3!, {r2}
	ldr r2, .L_020096fc
	stmia r3!, {r2}
	ldr r2, .L_02009700
	str r2, [r3]
.L_020096be:
	strh r1, [r5]
	movs r0, #2
	bl Battle_WaitMode0
	ldr r3, .L_020096f8
	mov r1, r8
	strh r3, [r1]
	ldrh r3, [r5]
	adds r1, r3, #0
	strh r5, [r5]
	ldrh r2, [r6]
	cmp r2, #31
	bgt .L_02009704
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	movs r0, #132
	adds r2, #1
	adds r3, r3, r6
	lsls r0, r0, #5
	adds r3, #4
	strh r2, [r6]
	adds r2, r7, r0
	stmia r3!, {r2}
	ldr r2, .L_020096fc
	stmia r3!, {r2}
	ldr r2, .L_02009700
	str r2, [r3]
	b .L_02009704
.L_020096f8:
	.4byte 0x00000800
.L_020096fc:
	.4byte 0x06002000
.L_02009700:
	.4byte 0x84000140
.L_02009704:
	strh r1, [r5]
	movs r0, #140
	bl Battle_WaitMode0
	ldrh r3, [r5]
	adds r1, r3, #0
	strh r5, [r5]
	ldrh r2, [r6]
	cmp r2, #31
	bgt .L_02009736
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	movs r0, #180
	adds r2, #1
	adds r3, r3, r6
	lsls r0, r0, #5
	adds r3, #4
	strh r2, [r6]
	adds r2, r7, r0
	stmia r3!, {r2}
	ldr r2, .L_02009884
	stmia r3!, {r2}
	ldr r2, .L_02009888
	str r2, [r3]
.L_02009736:
	strh r1, [r5]
	movs r0, #4
	bl Battle_WaitMode0
	ldrh r3, [r5]
	adds r1, r3, #0
	strh r5, [r5]
	ldrh r2, [r6]
	cmp r2, #31
	bgt .L_02009768
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	movs r0, #228
	adds r2, #1
	adds r3, r3, r6
	lsls r0, r0, #5
	adds r3, #4
	strh r2, [r6]
	adds r2, r7, r0
	stmia r3!, {r2}
	ldr r2, .L_02009884
	stmia r3!, {r2}
	ldr r2, .L_02009888
	str r2, [r3]
.L_02009768:
	strh r1, [r5]
	movs r0, #4
	bl Battle_WaitMode0
	ldrh r3, [r5]
	adds r1, r3, #0
	strh r5, [r5]
	ldrh r2, [r6]
	cmp r2, #31
	bgt .L_0200979a
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	movs r0, #138
	adds r2, #1
	adds r3, r3, r6
	lsls r0, r0, #6
	adds r3, #4
	strh r2, [r6]
	adds r2, r7, r0
	stmia r3!, {r2}
	ldr r2, .L_02009884
	stmia r3!, {r2}
	ldr r2, .L_02009888
	str r2, [r3]
.L_0200979a:
	strh r1, [r5]
	movs r0, #4
	bl Battle_WaitMode0
	mov r1, r9
	mov r2, r8
	strh r1, [r2]
	ldrh r3, [r5]
	adds r1, r3, #0
	strh r5, [r5]
	ldrh r2, [r6]
	cmp r2, #31
	bgt .L_020097d2
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	movs r0, #162
	adds r2, #1
	adds r3, r3, r6
	lsls r0, r0, #6
	adds r3, #4
	strh r2, [r6]
	adds r2, r7, r0
	stmia r3!, {r2}
	ldr r2, .L_02009884
	stmia r3!, {r2}
	ldr r2, .L_02009888
	str r2, [r3]
.L_020097d2:
	strh r1, [r5]
	movs r0, #4
	bl Battle_WaitMode0
	mov r1, r11
	mov r2, r8
	strh r1, [r2]
	ldrh r3, [r5]
	adds r1, r3, #0
	strh r5, [r5]
	ldrh r2, [r6]
	cmp r2, #31
	bgt .L_0200980a
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	movs r0, #186
	adds r2, #1
	adds r3, r3, r6
	lsls r0, r0, #6
	adds r3, #4
	strh r2, [r6]
	adds r2, r7, r0
	stmia r3!, {r2}
	ldr r2, .L_02009884
	stmia r3!, {r2}
	ldr r2, .L_02009888
	str r2, [r3]
.L_0200980a:
	strh r1, [r5]
	movs r0, #4
	bl Battle_WaitMode0
	movs r1, #176
	lsls r1, r1, #4
	mov r2, r8
	strh r1, [r2]
	ldrh r3, [r5]
	adds r1, r3, #0
	strh r5, [r5]
	ldrh r2, [r6]
	cmp r2, #31
	bgt .L_02009844
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	movs r0, #210
	adds r2, #1
	adds r3, r3, r6
	lsls r0, r0, #6
	adds r3, #4
	strh r2, [r6]
	adds r2, r7, r0
	stmia r3!, {r2}
	ldr r2, .L_02009884
	stmia r3!, {r2}
	ldr r2, .L_02009888
	str r2, [r3]
.L_02009844:
	strh r1, [r5]
	movs r0, #4
	bl Battle_WaitMode0
	ldr r3, .L_02009880
	mov r1, r8
	strh r3, [r1]
	ldrh r3, [r5]
	adds r1, r3, #0
	strh r5, [r5]
	ldrh r2, [r6]
	cmp r2, #31
	bgt .L_0200988c
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	movs r0, #234
	adds r2, #1
	adds r3, r3, r6
	lsls r0, r0, #6
	adds r3, #4
	strh r2, [r6]
	adds r2, r7, r0
	stmia r3!, {r2}
	ldr r2, .L_02009884
	stmia r3!, {r2}
	ldr r2, .L_02009888
	str r2, [r3]
	b .L_0200988c
	.2byte 0x0000
.L_02009880:
	.4byte 0x00000c00
.L_02009884:
	.4byte 0x06002000
.L_02009888:
	.4byte 0x84000140
.L_0200988c:
	strh r1, [r5]
	mov r1, r10
	cmp r1, #1
	bne .L_02009918
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #10
	lsls r2, r2, #9
	lsls r0, r0, #10
	bl Func_02001ff4
	movs r0, #141
	bl Func_020020b4
	ldr r3, .L_020098e4
	mov r2, r8
	strh r3, [r2]
	movs r0, #4
	bl Battle_WaitMode0
	ldr r3, .L_020098e8
	mov r0, r8
	strh r3, [r0]
	movs r0, #4
	bl Battle_WaitMode0
	ldr r3, .L_020098ec
	mov r1, r8
	strh r3, [r1]
	movs r0, #4
	bl Battle_WaitMode0
	ldr r3, .L_020098f0
	mov r2, r8
	strh r3, [r2]
	movs r0, #45
	bl Battle_WaitMode0
	ldr r0, .L_020098f4
	bl Scheduler_RemoveCallbackFar
	b .L_020098f8
	.2byte 0x0000
.L_020098e4:
	.4byte 0x00000d00
.L_020098e8:
	.4byte 0x00000e00
.L_020098ec:
	.4byte 0x00000f00
.L_020098f0:
	.4byte 0x00001000
.L_020098f4:
	.4byte Func_0200130c
.L_020098f8:
	bl Func_02001f74
	movs r0, #128
	mov r3, r8
	lsls r0, r0, #19
	ldrh r1, [r3]
	adds r0, #82
	bl QueueIoWriteDelay2
	movs r0, #128
	lsls r0, r0, #19
	adds r0, #80
	movs r1, #0
	bl QueueIoWriteDelay2
	b .L_02009982
.L_02009918:
	ldr r3, .L_02009950
	mov r0, r8
	strh r3, [r0]
	movs r0, #4
	bl Battle_WaitMode0
	ldr r3, .L_02009954
	mov r1, r8
	strh r3, [r1]
	movs r0, #4
	bl Battle_WaitMode0
	ldr r3, .L_02009958
	mov r2, r8
	strh r3, [r2]
	movs r0, #4
	bl Battle_WaitMode0
	ldr r3, .L_0200995c
	mov r0, r8
	strh r3, [r0]
	movs r0, #45
	bl Battle_WaitMode0
	ldr r0, .L_02009960
	bl Scheduler_RemoveCallbackFar
	b .L_02009964
.L_02009950:
	.4byte 0x00000d00
.L_02009954:
	.4byte 0x00000e00
.L_02009958:
	.4byte 0x00000f00
.L_0200995c:
	.4byte 0x00001000
.L_02009960:
	.4byte Func_0200130c
.L_02009964:
	bl Func_02001f74
	movs r0, #128
	mov r2, r8
	lsls r0, r0, #19
	ldrh r1, [r2]
	adds r0, #82
	bl QueueIoWriteDelay2
	movs r0, #128
	lsls r0, r0, #19
	adds r0, #80
	movs r1, #0
	bl QueueIoWriteDelay2
.L_02009982:
	adds r0, r7, #0
	bl Sys_Free
	movs r0, #2
	adds r0, #255
	bl GameFlag_SetBit
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.section .text.x0200999c,"ax",%progbits
	.global Func_0200199c
	.thumb_func
Func_0200199c:
	push {r5, lr}
	ldr r3, .L_020099d0
	movs r2, #2
	ldr r3, [r3]
	adds r5, r0, #0
	ands r3, r2
	cmp r3, #0
	beq .L_020099b4
	movs r1, #7
	bl Animation_ApplyChildValues
	b .L_020099bc
.L_020099b4:
	adds r0, r5, #0
	movs r1, #0
	bl Animation_ApplyChildValues
.L_020099bc:
	ldr r3, .L_020099d0
	movs r2, #15
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	bne .L_020099ce
	adds r0, r5, #0
	bl Func_02001ad0
.L_020099ce:
	pop {r5, pc}
.L_020099d0:
	.4byte gFrameCount
	.section .text.x020099d4,"ax",%progbits
	.global Func_020019d4
	.thumb_func
Func_020019d4:
	push {r5, r6, lr}
	ldr r6, .L_02009a08
	adds r5, r0, #0
	ldr r0, [r6]
	movs r3, #1
	ands r3, r0
	cmp r3, #0
	beq .L_020099f4
	movs r1, #6
	lsrs r0, r0, #1
	bl __umodsi3
	adds r1, r0, #0
	adds r0, r5, #0
	bl Animation_ApplyChildValues
.L_020099f4:
	ldr r3, [r6]
	movs r2, #15
	ands r3, r2
	cmp r3, #0
	bne .L_02009a04
	adds r0, r5, #0
	bl Func_02001ad0
.L_02009a04:
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02009a08:
	.4byte gFrameCount
	.section .text.x02009a0c,"ax",%progbits
	.global Func_02001a0c
	.thumb_func
Func_02001a0c:
	push {r5, lr}
	ldr r3, .L_02009a30
	adds r5, r0, #0
	ldr r0, [r3]
	movs r3, #1
	ands r3, r0
	cmp r3, #0
	beq .L_02009a2c
	movs r1, #6
	lsrs r0, r0, #1
	bl __umodsi3
	adds r1, r0, #0
	adds r0, r5, #0
	bl Animation_ApplyChildValues
.L_02009a2c:
	pop {r5, pc}
	.2byte 0x0000
.L_02009a30:
	.4byte gFrameCount
	.section .text.x02009a34,"ax",%progbits
	.global Func_02001a34
	.thumb_func
Func_02001a34:
	push {r5, r6, lr}
	adds r5, r0, #0
	adds r2, r5, #0
	adds r2, #100
	ldrh r3, [r2]
	ldr r6, [r5, #104]
	adds r3, #1
	strh r3, [r2]
	lsls r3, r3, #16
	asrs r0, r3, #16
	cmp r0, #31
	ble .L_02009a54
	adds r0, r5, #0
	bl Func_02001fd4
	b .L_02009a7e
.L_02009a54:
	lsls r0, r0, #10
	bl Math_Sine
	str r0, [r5, #24]
	str r0, [r5, #28]
	movs r1, #128
	ldr r3, [r6, #8]
	lsls r1, r1, #9
	str r3, [r5, #8]
	ldr r3, [r5, #12]
	adds r3, r3, r1
	str r3, [r5, #12]
	subs r1, r1, r0
	ldr r3, [r6, #16]
	lsls r2, r1, #2
	adds r2, r2, r1
	adds r3, r3, r2
	movs r2, #128
	lsls r2, r2, #12
	adds r3, r3, r2
	str r3, [r5, #16]
.L_02009a7e:
	pop {r5, r6, pc}
	.section .text.x02009a80,"ax",%progbits
	.global Func_02001a80
	.thumb_func
Func_02001a80:
	push {r5, r6, lr}
	adds r5, r0, #0
	adds r2, r5, #0
	adds r2, #100
	ldrh r3, [r2]
	ldr r6, [r5, #104]
	adds r3, #1
	strh r3, [r2]
	lsls r3, r3, #16
	asrs r0, r3, #16
	cmp r0, #31
	ble .L_02009aa0
	adds r0, r5, #0
	bl Func_02001fd4
	b .L_02009acc
.L_02009aa0:
	lsls r0, r0, #10
	bl Math_Sine
	negs r3, r0
	str r0, [r5, #24]
	str r3, [r5, #28]
	movs r1, #128
	ldr r3, [r6, #8]
	lsls r1, r1, #9
	str r3, [r5, #8]
	ldr r3, [r5, #12]
	adds r3, r3, r1
	str r3, [r5, #12]
	subs r1, r1, r0
	ldr r3, [r6, #16]
	lsls r2, r1, #2
	adds r2, r2, r1
	subs r3, r3, r2
	movs r2, #128
	lsls r2, r2, #13
	adds r3, r3, r2
	str r3, [r5, #16]
.L_02009acc:
	pop {r5, r6, pc}
	.2byte 0x0000
	.section .text.x02009ad0,"ax",%progbits
	.global Func_02001ad0
	.thumb_func
Func_02001ad0:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #224
	ldr r3, [r3]
	sub sp, #8
	adds r6, r0, #0
	mov r9, r3
	movs r7, #0
.L_02009aea:
	movs r0, #16
	ldr r1, [r6, #8]
	ldr r3, [r6, #16]
	ldr r2, [r6, #12]
	adds r0, #255
	bl Func_02001fcc
	mov r8, sp
	lsls r3, r7, #2
	mov r1, r8
	str r0, [r3, r1]
	cmp r0, #0
	beq .L_02009b94
	ldr r3, [r6, #20]
	ldr r5, [r0, #80]
	str r3, [r0, #20]
	ldr r1, .L_02009b24
	adds r3, r0, #0
	adds r3, #85
	movs r2, #0
	strb r2, [r3]
	adds r3, #15
	strh r2, [r3]
	mov r10, r1
	str r6, [r0, #104]
	cmp r5, #0
	beq .L_02009b94
	b .L_02009b28
	.2byte 0x0000
.L_02009b24:
	.4byte 0x00000000
.L_02009b28:
	movs r1, #0
	adds r0, r5, #0
	bl Animation_ApplyChildArgument
	mov r2, r10
	strb r2, [r5, #26]
	ldrb r0, [r5, #16]
	bl Resource_ResetEntry
	movs r3, #248
	lsls r3, r3, #3
	add r3, r9
	ldrh r3, [r3]
	movs r2, #1
	strb r3, [r5, #16]
	ldrb r3, [r5, #17]
	ldr r1, .L_02009b84
	orrs r3, r2
	strb r3, [r5, #17]
	ldrb r3, [r5, #16]
	ldr r2, .L_02009b88
	lsls r3, r3, #2
	adds r3, r3, r2
	ldrh r2, [r3, #2]
	ldrh r3, [r5, #8]
	lsls r2, r2, #17
	lsrs r2, r2, #22
	ands r3, r1
	orrs r3, r2
	strh r3, [r5, #8]
	movs r1, #33
	ldrb r3, [r5, #5]
	negs r1, r1
	adds r2, r1, #0
	ands r3, r2
	movs r2, #63
	ands r3, r2
	movs r1, #64
	orrs r3, r1
	strb r3, [r5, #5]
	ldrb r3, [r5, #7]
	ands r2, r3
	movs r3, #128
	orrs r2, r3
	b .L_02009b8c
	.2byte 0x0000
.L_02009b84:
	.4byte 0xfffffc00
.L_02009b88:
	.4byte ResourceTableEntries
.L_02009b8c:
	ldr r3, [r5, #40]
	strb r2, [r5, #7]
	mov r2, r10
	strb r2, [r3, #22]
.L_02009b94:
	adds r7, #1
	cmp r7, #1
	ble .L_02009aea
	ldr r2, [sp, #0]
	ldr r3, .L_02009bd8
	ldr r0, [r2, #80]
	str r3, [r2, #108]
	ldrb r1, [r0, #9]
	movs r2, #13
	negs r2, r2
	adds r3, r2, #0
	movs r4, #4
	ands r3, r1
	orrs r3, r4
	strb r3, [r0, #9]
	mov r3, r8
	ldr r1, [r3, #4]
	add sp, #8
	ldr r0, [r1, #80]
	ldrb r3, [r0, #9]
	ands r2, r3
	ldr r3, .L_02009bdc
	orrs r2, r4
	str r3, [r1, #108]
	adds r1, #35
	movs r3, #2
	strb r2, [r0, #9]
	strb r3, [r1]
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009bd8:
	.4byte Func_02001a80
.L_02009bdc:
	.4byte Func_02001a34
	.section .text.x02009be0,"ax",%progbits
	.global Func_02001be0
	.thumb_func
Func_02001be0:
	push {r5, r6, r7, lr}
	lsls r3, r0, #4
	subs r3, r3, r0
	movs r2, #128
	lsls r3, r3, #1
	lsls r2, r2, #8
	adds r3, r3, r2
	asrs r3, r3, #16
	adds r6, r3, #1
	movs r7, #0
	cmp r0, #0
	beq .L_02009bfa
	movs r7, #100
.L_02009bfa:
	cmp r6, #15
	bgt .L_02009c0c
	ldr r3, .L_02009d4c
	movs r2, #15
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #1
	bhi .L_02009c0c
	movs r7, #200
.L_02009c0c:
	cmp r6, #7
	bgt .L_02009c1e
	ldr r3, .L_02009d4c
	movs r2, #7
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #1
	bhi .L_02009c1e
	movs r7, #200
.L_02009c1e:
	ldr r1, .L_02009d50
	adds r2, r7, r6
	movs r0, #0
	ldrsh r3, [r1, r0]
	cmp r2, r3
	bne .L_02009c2c
	b .L_02009d4a
.L_02009c2c:
	ldr r5, .L_02009d54
	strh r2, [r1]
	movs r1, #128
	ldr r3, .L_02009d58
	adds r0, r5, #0
	lsls r1, r1, #1
	mov lr, r3
	.2byte 0xf800
	ldr r3, .L_02009d5c
	cmp r7, #0
	bne .L_02009c4e
	ldr r1, .L_02009d60
	adds r0, r5, #0
	movs r2, #128
	mov lr, r3
	.2byte 0xf800
	b .L_02009d3a
.L_02009c4e:
	cmp r7, #100
	bne .L_02009c5e
	ldr r1, .L_02009d64
	adds r0, r5, #0
	movs r2, #128
	mov lr, r3
	.2byte 0xf800
	b .L_02009c68
.L_02009c5e:
	ldr r1, .L_02009d60
	adds r0, r5, #0
	movs r2, #128
	mov lr, r3
	.2byte 0xf800
.L_02009c68:
	movs r2, #0
	adds r5, r6, #0
	mov r12, r2
	cmp r5, #7
	ble .L_02009c84
	ldr r3, .L_02009d5c
	ldr r0, .L_02009d54
	ldr r1, .L_02009d68
	movs r2, #32
	mov lr, r3
	.2byte 0xf800
	movs r3, #8
	mov r12, r3
	subs r5, #8
.L_02009c84:
	cmp r6, #15
	ble .L_02009c9c
	ldr r0, .L_02009d6c
	ldr r1, .L_02009d70
	ldr r3, .L_02009d5c
	movs r2, #32
	mov lr, r3
	.2byte 0xf800
	adds r5, r6, #0
	movs r0, #16
	mov r12, r0
	subs r5, #16
.L_02009c9c:
	cmp r6, #23
	ble .L_02009cb4
	movs r2, #32
	ldr r0, .L_02009d74
	ldr r1, .L_02009d78
	ldr r3, .L_02009d5c
	mov lr, r3
	.2byte 0xf800
	adds r5, r6, #0
	movs r2, #24
	mov r12, r2
	subs r5, #24
.L_02009cb4:
	cmp r6, #30
	ble .L_02009cca
	ldr r3, .L_02009d5c
	ldr r0, .L_02009d7c
	ldr r1, .L_02009d80
	movs r2, #32
	mov lr, r3
	.2byte 0xf800
	movs r3, #0
	mov r12, r3
	movs r5, #0
.L_02009cca:
	cmp r5, #0
	beq .L_02009d3a
	movs r7, #0
	cmp r7, r5
	bge .L_02009d3a
.L_02009cd4:
	mov r3, r12
	cmp r3, #0
	bge .L_02009cdc
	adds r3, #7
.L_02009cdc:
	asrs r3, r3, #3
	lsls r1, r3, #5
	lsrs r3, r7, #31
	adds r3, r7, r3
	asrs r0, r3, #1
	ldr r2, .L_02009d84
	ldr r3, .L_02009d88
	adds r2, r1, r2
	adds r3, r1, r3
	adds r6, r2, r0
	adds r0, r3, r0
	movs r3, #1
	ands r3, r7
	movs r4, #2
	cmp r3, #0
	bne .L_02009d1a
	movs r4, #1
.L_02009cfe:
	ldrb r3, [r0]
	ldrb r1, [r6]
	movs r2, #240
	ands r2, r3
	movs r3, #15
	ands r3, r1
	orrs r2, r3
	adds r4, #1
	strb r2, [r0]
	adds r6, #4
	adds r0, #4
	cmp r4, #3
	ble .L_02009cfe
	b .L_02009d34
.L_02009d1a:
	ldrb r3, [r0]
	ldrb r1, [r6]
	movs r2, #15
	ands r2, r3
	movs r3, #240
	ands r3, r1
	orrs r2, r3
	subs r4, #1
	strb r2, [r0]
	adds r6, #4
	adds r0, #4
	cmp r4, #0
	bge .L_02009d1a
.L_02009d34:
	adds r7, #1
	cmp r7, r5
	blt .L_02009cd4
.L_02009d3a:
	ldr r3, .L_02009d8c
	movs r1, #128
	movs r2, #0
	ldrsh r0, [r3, r2]
	lsls r1, r1, #1
	ldr r2, .L_02009d54
	bl VramBlock_LoadCached
.L_02009d4a:
	pop {r5, r6, r7, pc}
.L_02009d4c:
	.4byte gFrameCount
.L_02009d50:
	.4byte Data_02003fc4
.L_02009d54:
	.4byte Data_02003fd8
.L_02009d58:
	.4byte IwramClearWords
.L_02009d5c:
	.4byte IwramCopyWords
.L_02009d60:
	.4byte Data_020041d8
.L_02009d64:
	.4byte Data_02004158
.L_02009d68:
	.4byte Data_020040d8
.L_02009d6c:
	.4byte Data_02003ff8
.L_02009d70:
	.4byte Data_020040f8
.L_02009d74:
	.4byte Data_02004018
.L_02009d78:
	.4byte Data_02004118
.L_02009d7c:
	.4byte Data_02004038
.L_02009d80:
	.4byte Data_02004138
.L_02009d84:
	.4byte Data_020040dc
.L_02009d88:
	.4byte Data_02003fdc
.L_02009d8c:
	.4byte Data_02003fc6
	.section .text.x02009d90,"ax",%progbits
	.global Func_02001d90
	.thumb_func
Func_02001d90:
	push {r5, r6, r7, lr}
	ldr r0, .L_02009e2c
	sub sp, #16
	str r0, [sp, #0]
	ldr r3, .L_02009e30
	ldr r2, .L_02009e34
	movs r1, #0
	ldrsh r3, [r3, r1]
	lsls r3, r3, #2
	adds r3, r3, r2
	ldrh r3, [r3, #2]
	lsrs r6, r3, #5
	bl Func_0200209c
	bl Object_GetById
	ldr r3, .L_02009e38
	movs r2, #3
	ldr r3, [r3]
	adds r7, r0, #0
	ands r3, r2
	cmp r3, #0
	beq .L_02009dec
	movs r0, #131
	lsls r0, r0, #1
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02009dec
	ldr r3, [r7, #12]
	movs r2, #128
	lsls r2, r2, #11
	cmp r3, r2
	ble .L_02009dec
	ldr r5, .L_02009e3c
	ldrh r3, [r5]
	subs r3, #1
	strh r3, [r5]
	lsls r3, r3, #16
	cmp r3, #0
	bgt .L_02009dec
	movs r0, #1
	bl Func_0200200c
	ldr r3, .L_02009e28
	strh r3, [r5]
.L_02009dec:
	bl Func_02002014
	bl Func_02001be0
	adds r0, r7, #0
	add r1, sp, #4
	adds r0, #8
	bl Render_ProjectPoint
	add r3, sp, #4
	ldr r0, [sp, #0]
	ldr r1, [r3]
	ldr r3, [r3, #4]
	movs r2, #0
	stmia r0!, {r2}
	ldr r5, .L_02009e40
	subs r1, #16
	lsls r1, r1, #16
	subs r3, #32
	orrs r3, r1
	adds r4, r0, #0
	orrs r3, r5
	str r4, [sp, #0]
	stmia r0!, {r3}
	movs r3, #128
	lsls r3, r3, #3
	adds r1, r0, #0
	orrs r6, r3
	b .L_02009e44
	.2byte 0x0000
.L_02009e28:
	.4byte 0x0000000a
.L_02009e2c:
	.4byte Data_02003fcc
.L_02009e30:
	.4byte Data_02003fc6
.L_02009e34:
	.4byte ResourceTableEntries
.L_02009e38:
	.4byte gInput
.L_02009e3c:
	.4byte Data_02003fc8
.L_02009e40:
	.4byte 0x80004000
.L_02009e44:
	str r1, [sp, #0]
	str r6, [r0]
	movs r1, #255
	ldr r0, .L_02009e54
	bl Func_02001f7c
	add sp, #16
	pop {r5, r6, r7, pc}
.L_02009e54:
	.4byte Data_02003fcc
	.section .text.x02009e58,"ax",%progbits
	.global Func_02001e58
	.thumb_func
Func_02001e58:
	push {r5, lr}
	ldr r1, .L_02009e9c
	ldr r0, .L_02009ea0
	bl Func_02001f54
	ldr r5, .L_02009ea4
	bl Resource_FindFreeEntry
	movs r1, #128
	strh r0, [r5]
	lsls r0, r0, #16
	lsls r1, r1, #1
	movs r2, #0
	asrs r0, r0, #16
	bl VramBlock_LoadCached
	ldr r2, .L_02009ea8
	ldr r3, .L_02009e94
	movs r1, #128
	strh r3, [r2]
	ldr r2, .L_02009eac
	ldr r3, .L_02009e98
	lsls r1, r1, #3
	strh r3, [r2]
	adds r1, #118
	ldr r0, .L_02009eb0
	bl Scheduler_AddOrUpdateCallback
	b .L_02009eb4
	.2byte 0x0000
.L_02009e94:
	.4byte 0xffffffff
.L_02009e98:
	.4byte 0x0000000a
.L_02009e9c:
	.4byte Data_020040d8
.L_02009ea0:
	.4byte Data_02003432
.L_02009ea4:
	.4byte Data_02003fc6
.L_02009ea8:
	.4byte Data_02003fc4
.L_02009eac:
	.4byte Data_02003fc8
.L_02009eb0:
	.4byte Func_02001d90
.L_02009eb4:
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x02009eb8,"ax",%progbits
	.global Func_02001eb8
	.thumb_func
Func_02001eb8:
	push {lr}
	ldr r1, .L_02009ee0
	ldr r0, .L_02009ee4
	bl Func_02001f54
	ldr r3, .L_02009ee8
	movs r2, #0
	ldrsh r0, [r3, r2]
	bl Resource_ResetEntry
	ldr r2, .L_02009eec
	ldr r3, .L_02009edc
	ldr r0, .L_02009ef0
	strh r3, [r2]
	bl Scheduler_RemoveCallbackFar
	pop {pc}
	.2byte 0x0000
.L_02009edc:
	.4byte 0xffffffff
.L_02009ee0:
	.4byte Data_020040d8
.L_02009ee4:
	.4byte Data_02003432
.L_02009ee8:
	.4byte Data_02003fc6
.L_02009eec:
	.4byte Data_02003fc4
.L_02009ef0:
	.4byte Func_02001d90
	.section .rodata.x0200a0bc,"a",%progbits
	.global Data_020020bc
Data_020020bc:
	.4byte 0x00007c1f
	.4byte 0x00080005
	.4byte 0x0850042c
	.4byte 0x0c770853
	.4byte 0x1cfd109b
	.4byte 0x3dfd2d7d
	.4byte 0x5efe4e7e
	.4byte 0x7fff6f7e
	.4byte 0x00007c1f
	.4byte 0x24821441
	.4byte 0x454434e3
	.4byte 0x660655a5
	.4byte 0x7eaa7668
	.4byte 0x7f117eed
	.4byte 0x7f787f54
	.4byte 0x7fff7fbb
	.4byte 0x00007c1f
	.4byte 0x00680025
	.4byte 0x053000cc
	.4byte 0x05f70593
	.4byte 0x129d0a5b
	.4byte 0x371d22dd
	.4byte 0x5b7e473e
	.4byte 0x7fff6bbe
	.4byte 0x00007c1f
	.4byte 0x20281005
	.4byte 0x408e304b
	.4byte 0x60d450b1
	.4byte 0x7d5a7518
	.4byte 0x7e3c7dbb
	.4byte 0x7f1d7e9c
	.4byte 0x7fff7f7e
	.global Data_0200213c
Data_0200213c:
	.4byte 0x529c29c8
	.4byte 0x295b3dfb
	.4byte 0x001a14ba
	.4byte 0x62146ab7
	.4byte 0x50ef5992
	.4byte 0x77bc4c6d
	.4byte 0x033c3b7c
	.4byte 0x019f025d
	.global Data_0200215c
Data_0200215c:
	.4byte 0x775329c8
	.4byte 0x7aca770e
	.4byte 0x7e607a85
	.4byte 0x3f7c539c
	.4byte 0x173c2b5c
	.4byte 0x77bc033c
	.4byte 0x033c3b7c
	.4byte 0x019f025d
	.global Data_0200217c
Data_0200217c:
	.4byte 0xe0003e01
	.4byte 0x00ff0200
	.4byte 0x0200ff02
	.4byte 0xff0200ff
	.4byte 0x01b10200
	.4byte 0xe002e000
	.4byte 0xe004e003
	.4byte 0x06e02005
	.4byte 0x5c0e2a00
	.4byte 0x5ef05df0
	.4byte 0xf05ff001
	.4byte 0xf061f060
	.4byte 0xe4401204
	.4byte 0xe0081a07
	.4byte 0xe0a3e009
	.4byte 0x4000a440
	.4byte 0xa6f0a50e
	.4byte 0x7ef065f0
	.4byte 0x0c3c0566
	.4byte 0x26020040
	.4byte 0x00ff4000
	.4byte 0x4000ff40
	.4byte 0xf001a986
	.4byte 0xf098f0aa
	.4byte 0x4002f099
	.4byte 0x24420080
	.4byte 0xf09bf09a
	.4byte 0xc19df09c
	.4byte 0x01284000
	.4byte 0x9ff09e02
	.4byte 0x4006a0f0
	.4byte 0xa7e04805
	.4byte 0x4200a8e0
	.4byte 0x4004a122
	.4byte 0x4fe04e01
	.4byte 0x51e050e0
	.4byte 0x44228400
	.4byte 0x544004a2
	.4byte 0x801055e0
	.4byte 0xc159e42b
	.4byte 0x5030c010
	.4byte 0xe00a3700
	.4byte 0x000ce00b
	.4byte 0x62040a3e
	.4byte 0x64f063f0
	.4byte 0x0d0a0050
	.4byte 0xe0ab10e0
	.4byte 0x0a4000ac
	.4byte 0xf0aef0ad
	.4byte 0x3c096a72
	.4byte 0x00ff8030
	.4byte 0xec06d240
	.4byte 0xb173c000
	.4byte 0xf0b2f004
	.4byte 0x4000f094
	.4byte 0x16f09529
	.4byte 0x5f97f096
	.4byte 0x0202e800
	.4byte 0x42280050
	.4byte 0xe0afe008
	.4byte 0x2a0050b0
	.4byte 0x304ae049
	.4byte 0x00504be0
	.4byte 0x620050ff
	.4byte 0xe00fe00e
	.4byte 0x3c001040
	.4byte 0x67fc6604
	.4byte 0x44f068f0
	.4byte 0x10005069
	.4byte 0x00b4e0b3
	.4byte 0xf06b0440
	.4byte 0x6df06c08
	.4byte 0x270030f0
	.4byte 0x38b6f0b5
	.4byte 0x3e0070f0
	.4byte 0xffc04001
	.4byte 0xb9904000
	.4byte 0xf001baf0
	.4byte 0xf08ef08d
	.4byte 0x8000f08f
	.4byte 0xf0900127
	.4byte 0xf092f091
	.4byte 0x04005093
	.4byte 0xb7e03604
	.4byte 0x0050b8e0
	.4byte 0x023ce027
	.4byte 0x3ee03de0
	.4byte 0x00503fe0
	.4byte 0xe00c4326
	.4byte 0x5045e044
	.4byte 0x0050ff00
	.4byte 0x06e011e2
	.4byte 0xe013e012
	.4byte 0x003e0014
	.4byte 0x15190050
	.4byte 0xe016e008
	.4byte 0x6e140e17
	.4byte 0x50826ff0
	.4byte 0xe0181a00
	.4byte 0x0abce0bb
	.4byte 0xf0067110
	.4byte 0xf073f072
	.4byte 0x8d1bc030
	.4byte 0xf010bde8
	.4byte 0xc040f0be
	.4byte 0x81f0c1ef
	.4byte 0xf08204f0
	.4byte 0xc084f083
	.4byte 0xe0291200
	.4byte 0xc0e0bf10
	.4byte 0xf0c2140e
	.4byte 0x8a04f089
	.4byte 0x8cf08bf0
	.4byte 0x300e0050
	.4byte 0xe03104e0
	.4byte 0x5033e032
	.4byte 0xe0372600
	.4byte 0x39e0381c
	.4byte 0x50ff0050
	.4byte 0x0050ff00
	.4byte 0x44e019d2
	.4byte 0x2e00501a
	.4byte 0x061ce01b
	.4byte 0x18f0740c
	.4byte 0x5076f075
	.4byte 0x84012000
	.4byte 0x821ee01d
	.4byte 0xf0770a04
	.4byte 0x5079f078
	.4byte 0x211f2240
	.4byte 0x0804c3e0
	.4byte 0xf07bf0c4
	.4byte 0x41630050
	.4byte 0xc50a0825
	.4byte 0x80f07ff0
	.4byte 0x041c0050
	.4byte 0xe02be02a
	.4byte 0xc610082c
	.4byte 0xf0871ef0
	.4byte 0xff005088
	.4byte 0x50ff0050
	.4byte 0x0050ff00
	.4byte 0x028820f2
	.4byte 0x7af02106
	.4byte 0x22280050
	.4byte 0xe00423e0
	.4byte 0xf07cf024
	.4byte 0x26270050
	.4byte 0xe02701e0
	.4byte 0xf07df028
	.4byte 0x2600507e
	.4byte 0x2ee02d00
	.4byte 0x85f02fe0
	.4byte 0x50867ef0
	.4byte 0x0050ff00
	.4byte 0x032c20e5
	.4byte 0x50ff0050
	.4byte 0x0050ff00
	.4byte 0xe03fc7ee
	.4byte 0x50be0ec8
	.4byte 0x0050ff00
	.4byte 0xff0050ff
	.4byte 0x50ff0050
	.4byte 0xc9119d00
	.4byte 0x0050cae0
	.4byte 0xd1e0d02c
	.4byte 0xf802fe00
	.4byte 0x50ff0050
	.4byte 0x0050ff00
	.4byte 0xff0050ff
	.4byte 0xcb590050
	.4byte 0x5088cce0
	.4byte 0xe0d22c00
	.4byte 0x2c0050d3
	.4byte 0xfcd7e0d6
	.4byte 0x50043e10
	.4byte 0x0050ff00
	.4byte 0xff0050ff
	.4byte 0x50ff0050
	.4byte 0xe0cd1700
	.4byte 0x0050ce44
	.4byte 0xd5e0d42c
	.4byte 0xd82c0050
	.4byte 0x50d947e0
	.4byte 0xe0da2c00
	.4byte 0x067e10db
	.4byte 0x00f70050
	.global Data_02002468
Data_02002468:
	.4byte 0x7c010000
	.4byte 0x1d22c130
	.4byte 0x791c7ca1
	.4byte 0x462523a2
	.4byte 0x64534a24
	.4byte 0x6744a844
	.4byte 0x4408048a
	.4byte 0xda894012
	.4byte 0x08ff7e40
	.4byte 0x0280409f
	.4byte 0xa21c0a01
	.4byte 0xe70a2118
	.4byte 0x80e436c1
	.4byte 0xcbaa428e
	.4byte 0x63e218e8
	.4byte 0x05a330f0
	.4byte 0xa1c748e6
	.4byte 0x94451c04
	.4byte 0x6893290c
	.4byte 0x24481201
	.4byte 0xc64f90e0
	.4byte 0xf0254fa7
	.4byte 0x31f47821
	.4byte 0x8f063e04
	.4byte 0x7cbf093e
	.4byte 0x4bf5f25d
	.4byte 0x0900a1e6
	.4byte 0x6239a632
	.4byte 0x4c238926
	.4byte 0xe20b033a
	.4byte 0x38389528
	.4byte 0x84cc0e24
	.4byte 0xf1e02822
	.4byte 0x881047cb
	.4byte 0x68132c03
	.4byte 0x2c795267
	.4byte 0xbc69af13
	.4byte 0x0c2cf9bc
	.4byte 0x029819f2
	.4byte 0x0e0a6627
	.4byte 0x404c81c1
	.4byte 0x6bc1335c
	.4byte 0x0b415ca1
	.4byte 0x14a64573
	.4byte 0xe3700711
	.4byte 0x19f1204c
	.4byte 0x1435e2b8
	.4byte 0x2411e481
	.4byte 0x9c094798
	.4byte 0x769c0219
	.4byte 0x9c6d839f
	.4byte 0xcc08046f
	.4byte 0x85c40400
	.4byte 0x10c62222
	.4byte 0x21ebcf51
	.4byte 0x88106b3f
	.4byte 0x6641b82a
	.4byte 0x3ebe0241
	.4byte 0x1b307673
	.4byte 0x65e448f8
	.4byte 0x8a4133ae
	.4byte 0xf1c67e14
	.4byte 0x6cd33839
	.4byte 0x02802481
	.4byte 0x57902092
	.4byte 0x9823c88a
	.4byte 0x6f023cc2
	.4byte 0x16cc0b9c
	.4byte 0x43966489
	.4byte 0x209a01d2
	.4byte 0x45440342
	.4byte 0x80241e64
	.4byte 0x2f9c38b5
	.4byte 0x03be2a7c
	.4byte 0x10032202
	.4byte 0xb043e450
	.4byte 0x3e5dc783
	.4byte 0xfd791703
	.4byte 0x20e06c62
	.4byte 0x0ea00482
	.4byte 0x03611012
	.4byte 0x1f10d10e
	.4byte 0x233871b1
	.4byte 0x003e61da
	.4byte 0x303e7e0a
	.4byte 0xc41199d3
	.4byte 0x67441d40
	.4byte 0x328707e0
	.4byte 0x068ac044
	.4byte 0x4c01a0c8
	.4byte 0x49c20680
	.4byte 0x41190222
	.4byte 0x9163b00a
	.4byte 0xe647c0cf
	.4byte 0x3382f321
	.4byte 0xbc3b328d
	.4byte 0x1879f3ce
	.4byte 0xe030913d
	.4byte 0x401802d1
	.4byte 0x40681c0b
	.4byte 0x4454a2e1
	.4byte 0x5a8474ab
	.4byte 0x0247cab5
	.4byte 0x0661cf63
	.4byte 0xf2e80400
	.4byte 0x8e0e7c3b
	.4byte 0x8760cc60
	.4byte 0x5601008f
	.4byte 0x01e50c29
	.4byte 0xf302731c
	.4byte 0x5661ef07
	.4byte 0x7c3823e0
	.4byte 0xaad04124
	.4byte 0xaabd923e
	.4byte 0x7e086702
	.4byte 0x3022cf87
	.4byte 0xf06d309f
	.4byte 0x099f0793
	.4byte 0x8208f806
	.4byte 0x67331490
	.4byte 0x21b810fa
	.4byte 0x433b4d40
	.4byte 0x6e06f852
	.4byte 0x9c19f5a5
	.4byte 0x5450260a
	.4byte 0x9c280240
	.4byte 0x98e97041
	.4byte 0x8d02b55a
	.4byte 0x51849f3e
	.4byte 0x7c020810
	.4byte 0x83e1861c
	.4byte 0x03530871
	.4byte 0xf8030c9f
	.4byte 0xc4d9021c
	.4byte 0x96fe89cc
	.4byte 0x90a85081
	.4byte 0xa88c4c53
	.4byte 0x0b0aa222
	.4byte 0x316e0c7c
	.4byte 0xec4c6233
	.4byte 0x40320542
	.4byte 0xed204061
	.4byte 0xfbaa60b0
	.4byte 0x8224f854
	.4byte 0xba29dff0
	.4byte 0x7c100e6f
	.4byte 0xbf00ddf3
	.4byte 0xb6f472f3
	.4byte 0xe8473323
	.4byte 0x763979cd
	.4byte 0x01ffede7
	.4byte 0x9dddc801
	.4byte 0x0eee6ee4
	.4byte 0xebf7971c
	.4byte 0x9991ced8
	.4byte 0x0dee72e3
	.4byte 0x7c39bb1c
	.4byte 0x13e14630
	.4byte 0xce0998f0
	.4byte 0x78aa2228
	.4byte 0x8a9de2a7
	.4byte 0xf30c2277
	.4byte 0x1e2a778a
	.4byte 0x267582ba
	.4byte 0x22a1420d
	.4byte 0x10281300
	.4byte 0x22a1604f
	.4byte 0xf8599f80
	.4byte 0x38228c1c
	.4byte 0x047be106
	.4byte 0x0a403bf0
	.4byte 0xc08303bf
	.4byte 0x9b073c25
	.4byte 0xf84f01ef
	.4byte 0x33083e1e
	.4byte 0x09204192
	.4byte 0x86a028f1
	.4byte 0x3ddf1390
	.4byte 0xa142c673
	.4byte 0x41061e2b
	.4byte 0x19a28ef1
	.4byte 0x1476b349
	.4byte 0x1e293134
	.4byte 0x0f041162
	.4byte 0x40e4c455
	.4byte 0x13134420
	.4byte 0x71081118
	.4byte 0xb30d8384
	.4byte 0xf1673f08
	.4byte 0x16fe0481
	.4byte 0x2e38a3e1
	.4byte 0x48f8508b
	.4byte 0xad8a1f0e
	.4byte 0xcc782540
	.4byte 0x1122b382
	.4byte 0x785399d5
	.4byte 0xd6785e86
	.4byte 0xa120bd0c
	.4byte 0x5d4cd140
	.4byte 0xb5761533
	.4byte 0xb9d0f753
	.4byte 0x05287685
	.4byte 0x09da1335
	.4byte 0x88f15058
	.4byte 0x4460042e
	.4byte 0xbbf93008
	.4byte 0xb420ff05
	.4byte 0x11df821c
	.4byte 0xf870e7dc
	.4byte 0x9f827c94
	.4byte 0xc857c045
	.4byte 0xa669a822
	.4byte 0x2642d647
	.4byte 0x81082221
	.4byte 0x16607d11
	.4byte 0xe6058541
	.4byte 0x78288c11
	.4byte 0xd782241f
	.4byte 0x2354cc25
	.4byte 0x1671e08f
	.4byte 0xb859f1ee
	.4byte 0xe2205f04
	.4byte 0x3e0970b3
	.4byte 0x17009f13
	.4byte 0x1c7c3df1
	.4byte 0x850cc12e
	.4byte 0x87bf0479
	.4byte 0xf825d7d1
	.4byte 0x4c826c04
	.4byte 0x14403b82
	.4byte 0xac412e7e
	.4byte 0x296067c2
	.4byte 0xc12e207c
	.4byte 0xc213e1c7
	.4byte 0x023ff5ef
	.4byte 0x5811f5ec
	.4byte 0xcb6e3b04
	.4byte 0x021ef8eb
	.4byte 0x77053e00
	.4byte 0x07b023e0
	.4byte 0x01dc0398
	.4byte 0x9f0ec25e
	.4byte 0x77303d86
	.4byte 0x07b143e0
	.4byte 0x40ce702e
	.4byte 0xc49f5f47
	.4byte 0x818cd9f1
	.4byte 0x127dd71d
	.4byte 0xce1d7e47
	.4byte 0x6c24f899
	.4byte 0xf8204f81
	.4byte 0x7b023dec
	.4byte 0x813ed08f
	.4byte 0x0119b823
	.4byte 0x0f78323b
	.4byte 0x877e2523
	.4byte 0x61f0cf3f
	.4byte 0x6baf59d0
	.4byte 0xbb381af4
	.4byte 0x26e50ae4
	.4byte 0xa39a4fcd
	.4byte 0x4619db34
	.4byte 0xd788f360
	.4byte 0xf5179a96
	.4byte 0x8058f021
	.4byte 0x93e1cfa1
	.4byte 0x3d815439
	.4byte 0x5c342194
	.4byte 0xd44607c8
	.4byte 0x74428a97
	.4byte 0x40f99f50
	.4byte 0xe520411f
	.4byte 0x4c0a23e7
	.4byte 0xa010e20a
	.4byte 0x38907208
	.4byte 0x10674084
	.4byte 0xf5997607
	.4byte 0xceaa7f34
	.4byte 0x409f0439
	.4byte 0x6027c430
	.4byte 0xcd646412
	.4byte 0x0aa1cabd
	.4byte 0x267e8a61
	.4byte 0x1e698f30
	.4byte 0xa332efc1
	.4byte 0xa10063e9
	.4byte 0xe08f063e
	.4byte 0x5e8870d3
	.4byte 0xb11f212b
	.4byte 0x4a106808
	.4byte 0x41bcc238
	.4byte 0x7302e11a
	.4byte 0x0c7caa12
	.4byte 0xaebcc236
	.4byte 0x9910c1ac
	.4byte 0x7822e758
	.4byte 0x6ccb13cc
	.4byte 0xc7350f9a
	.4byte 0xcfd31669
	.4byte 0x13e3da75
	.4byte 0xb6fe0671
	.4byte 0x16564680
	.4byte 0xbe46a18d
	.4byte 0x6bc0b640
	.4byte 0xb61e7c33
	.4byte 0x1d814cc0
	.4byte 0x6146a342
	.4byte 0x2d9a8ecd
	.4byte 0x13e9b730
	.4byte 0x111a04f0
	.4byte 0x3291a20b
	.4byte 0x811572a7
	.4byte 0x1bfae3cc
	.4byte 0x1f829f02
	.4byte 0xbc603b30
	.4byte 0xcc28e3d7
	.4byte 0xec08fafd
	.4byte 0x39a76880
	.4byte 0x3ea1f35c
	.4byte 0x5e61d601
	.4byte 0xcc246bd3
	.4byte 0x54080011
	.4byte 0x2acaedc9
	.4byte 0xa33812c8
	.4byte 0x6a1b75e0
	.4byte 0xc07e0c7c
	.4byte 0xb302c26c
	.4byte 0xc1367ec2
	.4byte 0x7c21e327
	.4byte 0x0a13e604
	.4byte 0x88c05b00
	.4byte 0x4a046108
	.4byte 0x541135e2
	.4byte 0x25995443
	.4byte 0x0327432e
	.4byte 0x70584e61
	.4byte 0xd6ae4980
	.4byte 0x0a33af0c
	.4byte 0x809f00f0
	.4byte 0xe0933021
	.4byte 0x06bc12e7
	.4byte 0x6d790ec2
	.4byte 0x9366c103
	.4byte 0x78c41005
	.4byte 0xe5ce1bb1
	.4byte 0x3740459a
	.4byte 0x9f0459f8
	.4byte 0x08c16f0b
	.4byte 0x200b8194
	.4byte 0x3f207571
	.4byte 0x38325038
	.4byte 0xb0ae2c5c
	.4byte 0x804e052c
	.4byte 0x07c68643
	.4byte 0x087c25e1
	.4byte 0xc000403c
	.4byte 0x02595b19
	.4byte 0x1d3e34b3
	.4byte 0x84d78a61
	.4byte 0xe15556b6
	.4byte 0x357ba063
	.4byte 0x7d45a539
	.4byte 0x6bd4be08
	.4byte 0x0aa6314f
	.4byte 0x2cd4e226
	.4byte 0x9c3a0940
	.4byte 0xc9e8d829
	.4byte 0x66cd1765
	.4byte 0x7417902a
	.4byte 0x19a12881
	.4byte 0x1f98ac00
	.4byte 0xfc09f3d6
	.4byte 0x64ccb086
	.4byte 0xfef8b131
	.4byte 0x0302a832
	.4byte 0x001886f2
	.4byte 0x006743f1
	.4byte 0x8019f02c
	.4byte 0x1928d93f
	.4byte 0x209ebc00
	.4byte 0x5c8074a2
	.4byte 0xc7c01219
	.4byte 0x53cd3281
	.4byte 0x3c5800c2
	.4byte 0xc207d5be
	.4byte 0x2d8aab53
	.4byte 0xa1904cf8
	.4byte 0xf838c08f
	.4byte 0x95337e3c
	.4byte 0x44c08c3e
	.4byte 0x82107383
	.4byte 0xdf8a3eac
	.4byte 0x8216039c
	.4byte 0x62c0fc05
	.4byte 0x60c7c204
	.4byte 0x839cfd7e
	.4byte 0xdbc801c7
	.4byte 0x82b005e1
	.4byte 0x3c390178
	.4byte 0xd7656767
	.4byte 0x0e62a530
	.4byte 0x47c21ebe
	.4byte 0x39e40160
	.4byte 0x131e00f1
	.4byte 0x38107581
	.4byte 0x05900315
	.4byte 0x37d0a670
	.4byte 0x7e0473f0
	.4byte 0xa13cd396
	.4byte 0x6704da3c
	.4byte 0x38f8950d
	.4byte 0x00f382dc
	.4byte 0x088037ce
	.4byte 0x3825c894
	.4byte 0xd04e60e3
	.4byte 0x21981067
	.4byte 0x1e605030
	.4byte 0x99a13653
	.4byte 0x005158d9
	.4byte 0x03c7827c
	.4byte 0x8f3810c8
	.4byte 0x9c0833e7
	.4byte 0x7662504f
	.4byte 0xf9ac8cd3
	.4byte 0x7e72734c
	.4byte 0x6047c0e6
	.4byte 0xe6147c02
	.4byte 0xd80263c0
	.4byte 0xb68804cc
	.4byte 0xf8940062
	.4byte 0xcf814c04
	.4byte 0x1cf820c0
	.4byte 0x34d7814c
	.4byte 0x1738038a
	.4byte 0xb20535e0
	.4byte 0xd316be82
	.4byte 0xb9f81067
	.4byte 0xe67b7302
	.4byte 0xce0090f7
	.4byte 0x17387015
	.4byte 0x1227c8a0
	.4byte 0x73a16c78
	.4byte 0xf86ace07
	.4byte 0x73847c28
	.4byte 0x1558e630
	.4byte 0x0ee01c89
	.4byte 0x75179914
	.4byte 0xa620d30e
	.4byte 0x083ead33
	.4byte 0x1667e077
	.4byte 0x11f1759c
	.4byte 0xe24503d8
	.4byte 0x6709580a
	.4byte 0x247c891f
	.4byte 0xb302c0b6
	.4byte 0xf7033e05
	.4byte 0x0f84bce0
	.4byte 0xbaf817c7
	.4byte 0x4f811f09
	.4byte 0x660237f0
	.4byte 0x3b1fcd5f
	.4byte 0x7053e027
	.4byte 0xc0d6be06
	.4byte 0xfb9cd563
	.4byte 0x344c0672
	.4byte 0xf61d6cd1
	.4byte 0xce4ab64f
	.4byte 0x2a1988ea
	.4byte 0xce4354b3
	.4byte 0xe64328e8
	.4byte 0xf8fac077
	.4byte 0x678e5310
	.4byte 0x609acc79
	.4byte 0xce194897
	.4byte 0x73cbc64b
	.4byte 0xccc20ec8
	.4byte 0x7c38f8ca
	.4byte 0xe0463320
	.4byte 0x725e038c
	.4byte 0x6315ce07
	.4byte 0x3e147c12
	.4byte 0x99844217
	.4byte 0x25927490
	.4byte 0x288e2592
	.4byte 0xa1a3a31b
	.4byte 0xe59f84d4
	.4byte 0x9976b8f4
	.4byte 0xae031816
	.4byte 0x836c9e61
	.4byte 0xce1d06f9
	.4byte 0x188fc837
	.4byte 0x6d5b320f
	.4byte 0xe147c9da
	.4byte 0xa6027c24
	.4byte 0x4e4ee641
	.4byte 0x82bc9161
	.4byte 0x5f8f4819
	.4byte 0x78006593
	.4byte 0x4a076739
	.4byte 0x0fa66538
	.4byte 0x0701c005
	.4byte 0x8004c3e0
	.4byte 0xf987cccc
	.4byte 0x7a7343f9
	.4byte 0xc3a79876
	.4byte 0x88d1a036
	.4byte 0xc3a66531
	.4byte 0xd0fe247c
	.4byte 0x7ce86027
	.4byte 0xdc5abe1e
	.4byte 0x8641be4a
	.4byte 0xd9dc565a
	.4byte 0xaf829f6a
	.4byte 0xf908f8f6
	.4byte 0x80809906
	.4byte 0xbcf31182
	.4byte 0xd9dc43de
	.4byte 0x4f671df9
	.4byte 0x1c6477e4
	.4byte 0xc77e167c
	.4byte 0x83380699
	.4byte 0x04790699
	.4byte 0xeed78754
	.4byte 0x12e606f9
	.4byte 0xbcc1a63c
	.4byte 0x817e071e
	.4byte 0xe657d20b
	.4byte 0xc8509214
	.4byte 0x6e53942e
	.4byte 0x4a18575e
	.4byte 0xa30ccf36
	.4byte 0xabece1df
	.4byte 0xc20f9ff4
	.4byte 0x95ff8a2f
	.4byte 0xed604041
	.4byte 0xe5a621f0
	.4byte 0xdb7c067c
	.4byte 0xfdfbe107
	.4byte 0x17186e6b
	.4byte 0xc1020206
	.4byte 0x5ce55829
	.4byte 0xe10899b7
	.4byte 0x16381506
	.4byte 0xe7873c3a
	.4byte 0xb7631b30
	.4byte 0x59e22cf3
	.4byte 0x859f133e
	.4byte 0x405c40d3
	.4byte 0x1507241c
	.4byte 0x14c83ac9
	.4byte 0x32843aca
	.4byte 0xacc8b0eb
	.4byte 0x18332ac3
	.4byte 0x1dd44e7e
	.4byte 0x033e0d22
	.4byte 0x11be6813
	.4byte 0x10e98016
	.4byte 0xb6640880
	.4byte 0xbe421843
	.4byte 0x05780865
	.4byte 0xe12e2033
	.4byte 0xc20fa7ec
	.4byte 0x1c20fa37
	.4byte 0x33ff9fac
	.4byte 0xe0ca25d1
	.4byte 0x907ff073
	.4byte 0xa806d105
	.4byte 0x83440442
	.4byte 0x86bc954c
	.4byte 0x90998fb9
	.4byte 0xbb021f3d
	.4byte 0xe7980013
	.4byte 0x3e7ff063
	.4byte 0xece71f15
	.4byte 0x1c0334a2
	.4byte 0x46b07405
	.4byte 0x031f2847
	.4byte 0x348f90b9
	.4byte 0x123e62ab
	.4byte 0xe0c98972
	.4byte 0xf87247c0
	.4byte 0xafaf2024
	.4byte 0x90279763
	.4byte 0x0a33f029
	.4byte 0x8a1f0588
	.4byte 0x59d053df
	.4byte 0x3cee5aec
	.4byte 0xa1ef5df0
	.4byte 0x7a1d802b
	.4byte 0x95d9d7f1
	.4byte 0xbebc165c
	.4byte 0x9f9c3deb
	.4byte 0xd7af69d7
	.4byte 0x91d77f5f
	.4byte 0x7c026bd9
	.4byte 0xfedec70c
	.4byte 0x8d7b724a
	.4byte 0x30e28fb8
	.4byte 0xac23a043
	.4byte 0x1670e24f
	.4byte 0x05c10cdc
	.4byte 0x67e1c49f
	.4byte 0xc03ebdc1
	.4byte 0xc28f802c
	.4byte 0x1f358408
	.4byte 0x47c01660
	.4byte 0xf0ec0461
	.4byte 0xbdbd04c9
	.4byte 0x06c3bc87
	.4byte 0x1e7eef33
	.4byte 0xdbe0cede
	.4byte 0x54976c95
	.4byte 0xe660ef37
	.4byte 0xd82dcfdd
	.4byte 0x720ff5eb
	.4byte 0x1c7da106
	.4byte 0xcfb0217e
	.4byte 0x2c081bc0
	.4byte 0x82fc08f8
	.4byte 0xbb0c3e1d
	.4byte 0x7c05efe0
	.4byte 0xdcc17614
	.4byte 0xf0119f83
	.4byte 0x7b7a9809
	.4byte 0xdf96047c
	.4byte 0x273f7af9
	.4byte 0xc08f8f61
	.4byte 0x7c04fbf2
	.4byte 0x17c18fbd
	.4byte 0x3f3e0cf8
	.4byte 0x27c19087
	.4byte 0x0a7c1360
	.4byte 0xf5cfc27e
	.4byte 0x73ed08eb
	.4byte 0x3ebefdf0
	.4byte 0xfbe507c0
	.4byte 0xcf81fe7d
	.4byte 0x07f9f03f
	.4byte 0xe7c0ff3e
	.4byte 0x03fcf81f
	.4byte 0xf3e07f9f
	.4byte 0x81267c0f
	.4byte 0x0000000f
	.global Data_02002e74
Data_02002e74:
	.4byte 0x0000002e
	.4byte Func_02001064
	.4byte 0x00000026
	.global Data_02002e80
Data_02002e80:
	.4byte 0x00000000
	.4byte 0x00000027
	.4byte 0x00000016
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x00000016
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.global Data_02002eac
Data_02002eac:
	.4byte 0x00000000
	.4byte 0x00000009
	.4byte 0x00000000
	.4byte 0x00000006
	.global Data_02002ebc
Data_02002ebc:
	.4byte 0x7c1f7c1f
	.4byte 0x20a21861
	.4byte 0x45643503
	.4byte 0x6a2659c5
	.4byte 0x7ecd7e87
	.4byte 0x7f997f33
	.4byte 0x7c1f7fff
	.4byte 0x00007c1f
	.4byte 0x7c1f7c1f
	.4byte 0x013300ce
	.4byte 0x01b80176
	.4byte 0x023b01fa
	.4byte 0x02de027d
	.4byte 0x337f033f
	.4byte 0x613d7fff
	.4byte 0x00006dff
	.4byte 0x7c1f7c1f
	.4byte 0x2c2b1c09
	.4byte 0x4c713c4e
	.4byte 0x6cb75c94
	.4byte 0x7dbb7cfa
	.4byte 0x7f3d7e7c
	.4byte 0x7c1f7fff
	.4byte 0x00007c1f
	.4byte 0x7c1f7c1f
	.4byte 0x000d000a
	.4byte 0x00150011
	.4byte 0x001d0019
	.4byte 0x211e109d
	.4byte 0x5efe3dfe
	.4byte 0x7c1f7fff
	.4byte 0x00007c1f
	.global Data_02002f3c
Data_02002f3c:
	.4byte 0xe8802b01
	.4byte 0xec020200
	.4byte 0x0596020b
	.4byte 0x03280008
	.4byte 0x103c00c4
	.4byte 0xec870201
	.4byte 0x2c400086
	.4byte 0x873fe086
	.4byte 0x2b4000e0
	.4byte 0x4000be06
	.4byte 0x00be0826
	.4byte 0x120d2340
	.4byte 0x60800088
	.4byte 0x0192e091
	.4byte 0xe491e402
	.4byte 0x25400082
	.4byte 0xe0a1e0a0
	.4byte 0xe40201a2
	.4byte 0xa0e4a108
	.4byte 0x234000e4
	.4byte 0x21b1e0b0
	.4byte 0x0201b2e0
	.4byte 0xb0e4b1e4
	.4byte 0x09254000
	.4byte 0xb2e8b1e8
	.4byte 0xb1ec0201
	.4byte 0x00840a01
	.4byte 0xa1e825c0
	.4byte 0x0201a2e8
	.4byte 0x01cfa1ec
	.4byte 0x2740100a
	.4byte 0x020192e8
	.4byte 0x40200601
	.4byte 0x020e202c
	.4byte 0xff4000f7
	.4byte 0x007f4000
	.4byte 0x40000750
	.4byte 0x8000e049
	.4byte 0x0e40106e
	.4byte 0xe2ff0060
	.4byte 0x081b4000
	.4byte 0x628000be
	.4byte 0x019de09c
	.4byte 0xbf41e402
	.4byte 0xab264000
	.4byte 0xade0ace0
	.4byte 0xe4080201
	.4byte 0x50abe4ac
	.4byte 0xe0bb24c0
	.4byte 0xbde021bc
	.4byte 0xbce40201
	.4byte 0x4000bbe4
	.4byte 0xbce80925
	.4byte 0x0201bde8
	.4byte 0x0a01bcec
	.4byte 0x25c00084
	.4byte 0xade8ace8
	.4byte 0xacec0201
	.4byte 0x100a01cf
	.4byte 0x9de82740
	.4byte 0x06010201
	.4byte 0x0f278010
	.4byte 0x4000fa14
	.4byte 0xeb40609f
	.4byte 0x502f4050
	.4byte 0x40003c80
	.4byte 0x0060e04e
	.4byte 0x60aae02e
	.4byte 0x60e02e00
	.4byte 0x60e02e00
	.4byte 0x50e02e00
	.4byte 0xc4e02e80
	.4byte 0x002e0060
	.4byte 0xe08e8880
	.4byte 0xe402018f
	.4byte 0x00e4448e
	.4byte 0xe08c2740
	.4byte 0xe402018d
	.4byte 0x40009a8c
	.4byte 0x018de829
	.4byte 0xec060102
	.4byte 0xe828c000
	.4byte 0x02018f7f
	.4byte 0x00300601
	.4byte 0xff400030
	.4byte 0xb0ce4000
	.4byte 0x40001f40
	.4byte 0x00c0f8af
	.4byte 0x8700c0ff
	.4byte 0x00174400
	.4byte 0x40000780
	.4byte 0x9ae0ba20
	.4byte 0xe4020188
	.4byte 0x4000e4ba
	.4byte 0xaae0b927
	.4byte 0xe4020193
	.4byte 0x294000b9
	.4byte 0x0201aae8
	.4byte 0xec4f0601
	.4byte 0xe828c000
	.4byte 0x0102019a
	.4byte 0x2c401006
	.4byte 0xfe228490
	.4byte 0x40b34000
	.4byte 0x80002b44
	.4byte 0x2f045073
	.4byte 0x00373c00
	.4byte 0xb610a340
	.4byte 0x00888800
	.4byte 0xec822c40
	.4byte 0x2c4000b5
	.4byte 0xbe82e0b5
	.4byte 0x882c0060
	.4byte 0x082c0010
	.4byte 0x264000be
	.4byte 0x4000be08
	.4byte 0x40e18826
	.4byte 0xbe0a2c00
	.4byte 0x83244000
	.4byte 0x00e483e0
	.4byte 0x85112940
	.4byte 0x020193e0
	.4byte 0x00e485e4
	.4byte 0xe8392840
	.4byte 0x01020193
	.4byte 0x2bc00006
	.4byte 0xc05083e8
	.4byte 0xc030f6b9
	.4byte 0xff400028
	.4byte 0x001e3050
	.4byte 0x00e04f40
	.4byte 0x40106e40
	.4byte 0xec22b70e
	.4byte 0x2c4000b6
	.4byte 0x00a6eca7
	.4byte 0x3fa62c40
	.4byte 0x0060a7e0
	.4byte 0x00be082a
	.4byte 0xbe082640
	.4byte 0x08264000
	.4byte 0x4000febe
	.4byte 0x003e0a24
	.4byte 0xfc0a2240
	.4byte 0x0e268000
	.4byte 0x20c04042
	.4byte 0xb3e013b3
	.4byte 0x2c4000e4
	.4byte 0x40a0b3e8
	.4byte 0x1f309035
	.4byte 0xab4000f5
	.4byte 0x00ff0200
	.4byte 0x0060ff02
	.4byte 0x80709820
	.4byte 0x4000972e
	.4byte 0xb0977f2c
	.4byte 0xbe082c40
	.4byte 0x00298000
	.4byte 0xbe082c40
	.4byte 0x08268050
	.4byte 0x8000b0be
	.4byte 0x8000ec27
	.4byte 0x22c05038
	.4byte 0xe481e081
	.4byte 0x2c40009f
	.4byte 0x006081e8
	.4byte 0xff0200ff
	.4byte 0x00ff0200
	.4byte 0xa2412102
	.4byte 0xff0200df
	.4byte 0xb77a0200
	.4byte 0x00ff1210
	.4byte 0x0200ff02
	.4byte 0xff0200ff
	.4byte 0x80100200
	.2byte 0x0000
	.global Data_020031f6
Data_020031f6:
	.2byte 0xff00
	.4byte 0x1141387d
	.4byte 0x7ffe6715
	.4byte 0xe6889329
	.4byte 0x65ffaad0
	.4byte 0x72fbb552
	.4byte 0x8e4bd968
	.4byte 0xfbff3375
	.4byte 0x982f82bc
	.4byte 0x4f9985e1
	.4byte 0x947bc509
	.4byte 0x3e1c09f1
	.4byte 0x1567c0ff
	.4byte 0xea943994
	.4byte 0xab4499cb
	.4byte 0x4cb541dc
	.4byte 0x90cd1d9d
	.4byte 0x183b9574
	.4byte 0x465bbdda
	.4byte 0x4519b111
	.4byte 0x630f76cc
	.4byte 0x71176d4e
	.4byte 0x3177ddea
	.4byte 0xbaa666eb
	.4byte 0xc39531cc
	.4byte 0xbc9f60a7
	.4byte 0xb2350a43
	.4byte 0x3521ccf8
	.4byte 0x45c80c78
	.4byte 0x19ccf490
	.4byte 0x44c62e2a
	.4byte 0x4aac5489
	.4byte 0x2ad56a94
	.4byte 0x41d8658a
	.4byte 0x0547ccd1
	.4byte 0x611c0ab3
	.4byte 0x10c0a336
	.4byte 0x2a05033e
	.4byte 0x500a11c0
	.4byte 0x00a11c84
	.4byte 0x892fc845
	.4byte 0x905596e4
	.4byte 0x483bd970
	.4byte 0xa9e670f8
	.4byte 0xa6ab3176
	.4byte 0x5251886a
	.4byte 0x0eebacbd
	.4byte 0x7cb3bf9f
	.4byte 0xa59f8176
	.4byte 0x52c78d30
	.4byte 0x06a7ca38
	.4byte 0x8f9b3306
	.4byte 0x4238c682
	.4byte 0xf8473360
	.4byte 0x13e27104
	.4byte 0xd119b370
	.4byte 0x9890f139
	.4byte 0x644c4898
	.4byte 0xcd4a915e
	.4byte 0x10f8352a
	.4byte 0xcd3532cd
	.4byte 0x2b3870d4
	.4byte 0x045e40b2
	.4byte 0x4ef05917
	.4byte 0x854ef524
	.4byte 0x7d118782
	.4byte 0xd3e2f4fe
	.4byte 0xa038cc0c
	.4byte 0xdb034c2b
	.4byte 0xb87032c0
	.4byte 0x2c213398
	.4byte 0xfc99aaec
	.4byte 0xb37b0a8d
	.4byte 0x656367c3
	.4byte 0x6ac38f1e
	.4byte 0x5268c2cc
	.4byte 0x04c3c1a9
	.4byte 0x1d3f2227
	.4byte 0x1c9af02e
	.4byte 0xa0014098
	.4byte 0x0a40a398
	.4byte 0x8a398a08
	.4byte 0x75180e8f
	.4byte 0x0be60b60
	.4byte 0x48dc87be
	.4byte 0x88876728
	.4byte 0xe7289db1
	.4byte 0x02e81d9d
	.4byte 0xc10ee0a0
	.4byte 0x7687ba82
	.4byte 0x42227088
	.4byte 0xd267118f
	.4byte 0x40899ccc
	.4byte 0x834aacce
	.4byte 0x1be0706f
	.4byte 0x61b62448
	.4byte 0x6571c787
	.4byte 0x05d814c3
	.4byte 0xc4c327c8
	.4byte 0x7c808af9
	.4byte 0x4e5ea933
	.4byte 0xe7d551d5
	.4byte 0x103e350f
	.4byte 0xe5ce7182
	.4byte 0xe7107013
	.4byte 0x0f87be1e
	.4byte 0x127ccbc6
	.4byte 0xf80c066e
	.4byte 0x65cfa21d
	.4byte 0xb264e43a
	.4byte 0x7989e132
	.4byte 0x29e70e8a
	.4byte 0x942246e6
	.4byte 0x622eb888
	.4byte 0x27e0a182
	.4byte 0x827e2e98
	.4byte 0xfc18fa90
	.4byte 0x041687c4
	.4byte 0xfa0421f0
	.4byte 0xd7b0ebc2
	.4byte 0x3f863edd
	.4byte 0xea6f07c0
	.4byte 0xfe7d8873
	.4byte 0xf03fcf81
	.4byte 0x953e07f9
	.4byte 0xf86b8fc0
	.4byte 0x00ff0e38
	.4byte 0xc3bc61fa
	.4byte 0xfe7cf0ff
	.4byte 0x6cbfe050
	.4byte 0x00737848
	.4byte 0xd7f76b76
	.4byte 0xd001ce01
	.4byte 0x1f780049
	.4byte 0x183c1f85
	.4byte 0xfcfb9bc0
	.4byte 0xe07f9f03
	.4byte 0x3e1df763
	.4byte 0x1fe7c0ff
	.4byte 0x9f03fcf8
	.4byte 0x0ff3e07f
	.4byte 0xcf81fe7c
	.4byte 0x04e1f03f
	.2byte 0x003e
	.global Data_02003432
Data_02003432:
	.2byte 0x8100
	.4byte 0xffa41112
	.4byte 0x1bc8904f
	.4byte 0xf611d25c
	.4byte 0x6f00412e
	.4byte 0xdf7aa760
	.4byte 0x3ef502df
	.4byte 0xfa813fa2
	.4byte 0x3e813e4f
	.4byte 0x7e0bfaad
	.4byte 0xce26f9bf
	.4byte 0x08a4904c
	.4byte 0x0a3e1d81
	.4byte 0x5599f55f
	.4byte 0xe147c0bb
	.4byte 0x27f2a7cb
	.4byte 0x75499f50
	.4byte 0x147c3b21
	.4byte 0x208c52be
	.4byte 0x20983c48
	.4byte 0x1f020e45
	.4byte 0x23f52f85
	.4byte 0x9f560fd4
	.4byte 0x8be147c3
	.4byte 0xa04fe54f
	.4byte 0x41fa907e
	.4byte 0x18f933ea
	.4byte 0x0001f17c
	.global gSceneEntrances
gSceneEntrances:
	.4byte 0xffff0000
	.4byte 0x0000267a
	.4byte 0x40002614
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x000027a6
	.4byte 0x40001ff4
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0002
	.4byte 0x000027a0
	.4byte 0x4000208a
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0003
	.4byte 0x000027a0
	.4byte 0xc0002030
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0004
	.4byte 0x00002799
	.4byte 0x4000219a
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0005
	.4byte 0x000028b7
	.4byte 0x400020a2
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0006
	.4byte 0x000027ce
	.4byte 0x800022b3
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0007
	.4byte 0x00002952
	.4byte 0x400022f8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0008
	.4byte 0x0000298b
	.4byte 0xc0002350
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0009
	.4byte 0x000027b8
	.4byte 0x4000259e
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000a
	.4byte 0x0000281f
	.4byte 0x40002544
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000b
	.4byte 0x0000281f
	.4byte 0xc00024cc
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000c
	.4byte 0x0000288c
	.4byte 0x8000249a
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000d
	.4byte 0x000028dc
	.4byte 0x000024b8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000e
	.4byte 0x00002b75
	.4byte 0x400029c7
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000f
	.4byte 0x00002f26
	.4byte 0x400028e6
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0010
	.4byte 0x00002e68
	.4byte 0x4000276a
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0011
	.4byte 0x00002ae0
	.4byte 0x800026e8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0012
	.4byte 0x00002dd8
	.4byte 0xc000253a
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0013
	.4byte 0x00002e86
	.4byte 0x40002614
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0014
	.4byte 0x00002d71
	.4byte 0x400024be
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0015
	.4byte 0x00002418
	.4byte 0x8000253c
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0016
	.4byte 0x00002480
	.4byte 0x0000253a
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0017
	.4byte 0x000023bc
	.4byte 0x40002358
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0018
	.4byte 0x000023bc
	.4byte 0xc000231c
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0019
	.4byte 0x000023f0
	.4byte 0x400020f8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff001a
	.4byte 0x00002364
	.4byte 0x80002062
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff001b
	.4byte 0x00002428
	.4byte 0x40001f0e
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff001c
	.4byte 0x0000254a
	.4byte 0x4000207c
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff001d
	.4byte 0x000031e2
	.4byte 0x40002d32
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff001e
	.4byte 0x00002dcc
	.4byte 0x40001f80
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff001f
	.4byte 0x00002da0
	.4byte 0x40001a16
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0020
	.4byte 0x000034ae
	.4byte 0xc0001d3a
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0021
	.4byte 0x00003279
	.4byte 0xc00025a7
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0022
	.4byte 0x00003279
	.4byte 0x40002618
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0023
	.4byte 0x000031e2
	.4byte 0x800027ec
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0024
	.4byte 0x0000388e
	.4byte 0x800020da
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0025
	.4byte 0x000037aa
	.4byte 0x40002218
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0026
	.4byte 0x0000343a
	.4byte 0x800017b6
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0027
	.4byte 0x0000348c
	.4byte 0x4000183c
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0028
	.4byte 0x00002a92
	.4byte 0x4000189c
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0029
	.4byte 0x00002bbe
	.4byte 0x400017f4
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff002a
	.4byte 0x000030b8
	.4byte 0x40001da8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff002b
	.4byte 0x00002f3a
	.4byte 0x80001bfd
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff002c
	.4byte 0x000032ac
	.4byte 0xc00004d8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff002d
	.4byte 0x00003320
	.4byte 0x40000442
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff002e
	.4byte 0x00002512
	.4byte 0x40002ad0
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff002f
	.4byte 0x000016cc
	.4byte 0xc0002788
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0030
	.4byte 0x00002186
	.4byte 0x40001cfc
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0031
	.4byte 0x000018aa
	.4byte 0x4000184f
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0032
	.4byte 0x00002290
	.4byte 0x4000122a
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0033
	.4byte 0x0000230a
	.4byte 0x40001d4c
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0034
	.4byte 0x00001c58
	.4byte 0x40002085
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0035
	.4byte 0x00001c70
	.4byte 0x40001ff4
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0036
	.4byte 0x00001e22
	.4byte 0x40001e50
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0037
	.4byte 0x00001c6b
	.4byte 0x40001cc5
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0038
	.4byte 0x00001cd7
	.4byte 0x40001679
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0039
	.4byte 0x00001d50
	.4byte 0x40001612
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff003a
	.4byte 0x00001dee
	.4byte 0x400015b6
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff003b
	.4byte 0x00002300
	.4byte 0x800016e0
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff003c
	.4byte 0x000022e5
	.4byte 0x40000ed8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff003d
	.4byte 0x000022d4
	.4byte 0xc0000668
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff003e
	.4byte 0x00002328
	.4byte 0x4000051b
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff003f
	.4byte 0x00002350
	.4byte 0xc00004dd
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0040
	.4byte 0x000007c8
	.4byte 0x00001c68
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0041
	.4byte 0x000007f0
	.4byte 0x00001db0
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0043
	.4byte 0x00002d71
	.4byte 0x400024be
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0044
	.4byte 0x00002990
	.4byte 0x00002373
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0045
	.4byte 0x00002990
	.4byte 0x00002373
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0047
	.4byte 0x000029e0
	.4byte 0x00002373
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0048
	.4byte 0x00001c6f
	.4byte 0x00001c8b
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0050
	.4byte 0x00002dea
	.4byte 0x00001f5e
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0051
	.4byte 0x00002db4
	.4byte 0x80001f62
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0052
	.4byte 0x00002d4c
	.4byte 0x400027dc
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0053
	.4byte 0x00002800
	.4byte 0x400023b6
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0054
	.4byte 0x0000233c
	.4byte 0x40000372
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0055
	.4byte 0x00002448
	.4byte 0xc00024da
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0056
	.4byte 0x00002448
	.4byte 0x400025a8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0057
	.4byte 0x00003160
	.4byte 0x40001126
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0058
	.4byte 0x00001ebc
	.4byte 0x400023a0
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0059
	.4byte 0x00002507
	.4byte 0x400013c4
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0062
	.4byte 0x000022d4
	.4byte 0x40000668
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0063
	.4byte 0x0000281f
	.4byte 0x40002544
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0064
	.4byte 0x000000ac
	.4byte 0x40000095
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0065
	.4byte 0x000000ac
	.4byte 0x40000095
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0066
	.4byte 0x000000ac
	.4byte 0x40000095
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0067
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
	.global gSceneExits
gSceneExits:
	.4byte 0x000001ff
	.global gScenePlacements
gScenePlacements:
	.4byte 0xffff029a
	.4byte 0x00000001
	.4byte 0x233c0000
	.4byte 0x00000000
	.4byte 0x03720000
	.4byte 0x0000a000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gSceneEvents
gSceneEvents:
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02003d20
Data_02003d20:
	.4byte 0x06f6243c
	.4byte 0x0c412a02
	.4byte 0x187c27b0
	.4byte 0x1ca62033
	.4byte 0x13ec22de
	.global Data_02003d34
Data_02003d34:
	.4byte 0x030e233c
	.4byte 0x10bc2775
	.4byte 0x1c6627c3
	.4byte 0x1c891c57
	.4byte 0x13ec2649
	.global Data_02003d48
Data_02003d48:
	.4byte 0x0a0d2562
	.4byte 0x121f2f93
	.4byte 0x1c952217
	.4byte 0x1aad2617
	.4byte 0x14642617
	.global Data_02003d5c
Data_02003d5c:
	.4byte 0x00008000
	.4byte 0x0000cccc
	.4byte 0x00018000
	.4byte 0x00009999
	.4byte 0x0000e666
	.global Data_02003d70
Data_02003d70:
	.4byte 0x00010000
	.4byte 0x0000cccc
	.4byte 0x0000b333
	.4byte 0x00014ccc
	.4byte 0x00009999
	.global Data_02003d84
Data_02003d84:
	.4byte 0x7e78aa82
	.global Data_02003d88
Data_02003d88:
	.4byte 0x6c406c6b
	.2byte 0x6c6c
	.global Data_02003d8e
Data_02003d8e:
	.2byte 0x0e00
	.4byte 0x0c000d00
	.4byte 0x0a000b00
	.4byte 0x08000900
	.global Data_02003d9c
Data_02003d9c:
	.4byte 0xf808fc16
	.4byte 0x0c0ee402
	.4byte 0x00000808
	.global Data_02003da8
Data_02003da8:
	.4byte 0x0000002e
	.4byte Func_02000c9c
	.4byte 0x00000011
	.4byte 0x00000000
	.global Data_02003db8
Data_02003db8:
	.4byte 0x00000000
	.section .bss,"aw",%nobits
	.global Data_02003dbc
Data_02003dbc:
	.space 0x00000002
	.global Data_02003dbe
Data_02003dbe:
	.space 0x00000002
	.global Data_02003dc0
Data_02003dc0:
	.space 0x00000200
	.global Data_02003fc0
Data_02003fc0:
	.space 0x00000004
	.global Data_02003fc4
Data_02003fc4:
	.space 0x00000002
	.global Data_02003fc6
Data_02003fc6:
	.space 0x00000002
	.global Data_02003fc8
Data_02003fc8:
	.space 0x00000004
	.global Data_02003fcc
Data_02003fcc:
	.space 0x0000000c
	.global Data_02003fd8
Data_02003fd8:
	.space 0x00000004
	.global Data_02003fdc
Data_02003fdc:
	.space 0x0000001c
	.global Data_02003ff8
Data_02003ff8:
	.space 0x00000020
	.global Data_02004018
Data_02004018:
	.space 0x00000020
	.global Data_02004038
Data_02004038:
	.space 0x000000a0
	.global Data_020040d8
Data_020040d8:
	.space 0x00000004
	.global Data_020040dc
Data_020040dc:
	.space 0x0000001c
	.global Data_020040f8
Data_020040f8:
	.space 0x00000020
	.global Data_02004118
Data_02004118:
	.space 0x00000020
	.global Data_02004138
Data_02004138:
	.space 0x00000020
	.global Data_02004158
Data_02004158:
	.space 0x00000080
	.global Data_020041d8
Data_020041d8:
