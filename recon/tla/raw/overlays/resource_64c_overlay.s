.syntax unified
	.thumb
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
	.4byte gData_02003dbe
.L_02008274:
	bl ObjectDispatch_SetSingleChildFlags
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
	bl Event_StartScreenTransition
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
	.4byte gBgScroll
.L_020083f0:
	.4byte gData_02003d20
.L_020083f4:
	.4byte gData_02003d5c
.L_020083f8:
	.4byte IwramClearWords
.L_020083fc:
	.4byte 0x050001c0
.L_02008400:
	.4byte gPartyState
.L_02008404:
	.4byte gData_02003d34
.L_02008408:
	.4byte gData_02003d84
.L_0200840c:
	.4byte gData_02003d70
.L_02008410:
	.4byte gData_02003d88 + 0x1
.L_02008414:
	.4byte Data_02002469
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
	.4byte gBgScroll
.L_020085f8:
	.4byte gIoWriteQueue
.L_020085fc:
	.4byte 0x04000208
.L_02008600:
	.4byte Data_0200213c
.L_02008604:
	.4byte 0x050001c0
.L_02008608:
	.4byte gData_0200215c
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
	.4byte gData_02003d9c
.L_02008750:
	.4byte gIoWriteQueue
.L_02008754:
	.4byte 0x04000208
.L_02008758:
	.4byte Data_020020bc
.L_0200875c:
	.4byte 0x050001c0
.L_02008760:
	.4byte gData_02003d34
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
	.4byte gData_02003dbc
.L_0200877c:
	.4byte gData_02003dbe
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
	.4byte gData_02003dbc
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
	.4byte gData_02003dbc
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
	.4byte gData_02003dbc
.L_02008a14:
	.4byte gData_02003d8e
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
	.4byte gData_02003dbc
.L_02008a50:
	.4byte gData_02003dbe
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
	bl Event_FinishScreenTransition
.L_02008afc:
	movs r0, #1
	adds r7, #1
	bl WaitFrames
	cmp r7, #124
	bne .L_02008ad0
	bl Event_WaitTransitionFrames
	b .L_02008b16
.L_02008b0e:
	ldr r2, .L_02008b54
	movs r3, #0
	strh r3, [r2]
	strh r3, [r2, #2]
.L_02008b16:
	mov r0, r10
	bl MemFree
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
	.4byte gBgScroll
.L_02008b58:
	.4byte Func_0200005c
.L_02008b5c:
	.4byte gData_02003d48
.L_02008b60:
	.4byte gData_02003dbc
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
	.4byte gData_02003dc0
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
	.4byte gData_02003dc0
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
	.4byte gData_02003dc0
.L_02008f84:
	.4byte 0xff600000
.L_02008f88:
	.4byte 0xfed40000
.L_02008f8c:
	.4byte 0xff100000
.L_02008f90:
	.4byte 0xfe3e0000
.L_02008f94:
	.4byte gData_02003da8
.L_02008f98:
	.4byte IwramMulQ16
.L_02008f9c:
	.4byte gPartyState
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
	bl Event_StartScreenTransition
	bl Event_WaitTransitionFrames
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
	.4byte gWorldMapPackedTiles
.L_0200954c:
	.4byte gWorldMapPackedFrames
.L_02009550:
	.4byte gIoWriteQueue
.L_02009554:
	.4byte 0x04000208
.L_02009558:
	.4byte gWorldMapPalettes
.L_0200955c:
	.4byte 0x050001c0
.L_02009560:
	.4byte 0x06001000
.L_02009564:
	.4byte 0x84000400
.L_02009568:
	.4byte Func_0200130c
.L_0200956c:
	.4byte gData_02003fc0
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
	bl MemFree
	movs r0, #2
	adds r0, #255
	bl GameFlag_SetBit
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.section .text.x02009ad0,"ax",%progbits
	.balign 4
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
	.4byte gResourceTableEntries
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
	bl VramCache_Load
.L_02009d4a:
	pop {r5, r6, r7, pc}
.L_02009d4c:
	.4byte gFrameCount
.L_02009d50:
	.4byte gData_02003fc4
.L_02009d54:
	.4byte gData_02003fd8
.L_02009d58:
	.4byte IwramClearWords
.L_02009d5c:
	.4byte IwramCopyWords
.L_02009d60:
	.4byte Data_020041d8
.L_02009d64:
	.4byte Data_02004158
.L_02009d68:
	.4byte gData_020040d8
.L_02009d6c:
	.4byte Data_02003ff8
.L_02009d70:
	.4byte Data_020040f8
.L_02009d74:
	.4byte gData_02004018
.L_02009d78:
	.4byte Data_02004118
.L_02009d7c:
	.4byte Data_02004038
.L_02009d80:
	.4byte Data_02004138
.L_02009d84:
	.4byte gData_020040dc
.L_02009d88:
	.4byte Data_02003fdc
.L_02009d8c:
	.4byte gData_02003fc6
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
	.4byte gData_02003fc6
.L_02009e34:
	.4byte gResourceTableEntries
.L_02009e38:
	.4byte gInput
.L_02009e3c:
	.4byte gData_02003fc8
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
	.section .bss,"aw",%nobits
	.global gData_02003dbc
gData_02003dbc:
	.space 0x00000002
	.global gData_02003dbe
gData_02003dbe:
	.space 0x00000002
	.global gData_02003dc0
gData_02003dc0:
	.space 0x00000200
	.global gData_02003fc0
gData_02003fc0:
	.space 0x00000004
	.global gData_02003fc4
gData_02003fc4:
	.space 0x00000002
	.global gData_02003fc6
gData_02003fc6:
	.space 0x00000002
	.global gData_02003fc8
gData_02003fc8:
	.space 0x00000004
	.global Data_02003fcc
Data_02003fcc:
	.space 0x0000000c
	.global gData_02003fd8
gData_02003fd8:
	.space 0x00000004
	.global Data_02003fdc
Data_02003fdc:
	.space 0x0000001c
	.global Data_02003ff8
Data_02003ff8:
	.space 0x00000020
	.global gData_02004018
gData_02004018:
	.space 0x00000020
	.global Data_02004038
Data_02004038:
	.space 0x000000a0
	.global gData_020040d8
gData_020040d8:
	.space 0x00000004
	.global gData_020040dc
gData_020040dc:
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
