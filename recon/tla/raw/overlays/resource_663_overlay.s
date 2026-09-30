.syntax unified
	.thumb
	.section .text.x0200807e,"ax",%progbits
	.2byte 0x0000
	.section .text.x02008080,"ax",%progbits
	.global Func_02000080
	.thumb_func
Func_02000080:
	push {r5, r6, lr}
	adds r4, r0, #0
	adds r5, r1, #0
	adds r6, r2, #0
	adds r0, r3, #0
	adds r2, r5, #0
	adds r1, r4, #0
	adds r3, r6, #0
	bl Func_02003b40
	adds r5, r0, #0
	cmp r5, #0
	beq .L_020080ca
	movs r1, #0
	bl Object_SetSpritePriority
	adds r2, r5, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	adds r2, #4
	movs r3, #8
	strb r3, [r2]
	adds r0, r5, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	adds r0, r5, #0
	movs r1, #14
	bl Object_SetPartAttribute
	adds r0, r5, #0
	movs r1, #1
	bl Animation_SetStateFlags
	adds r0, r5, #0
	b .L_020080cc
.L_020080ca:
	movs r0, #0
.L_020080cc:
	pop {r5, r6, pc}
	.2byte 0x0000
	.section .text.x020080d0,"ax",%progbits
	.global Func_020000d0
	.thumb_func
Func_020000d0:
	push {r5, r6, lr}
	adds r4, r0, #0
	adds r5, r1, #0
	adds r6, r2, #0
	adds r0, r3, #0
	adds r2, r5, #0
	adds r1, r4, #0
	adds r3, r6, #0
	bl Func_02003b40
	adds r5, r0, #0
	cmp r5, #0
	beq .L_0200811e
	movs r1, #1
	bl Object_SetSpritePriority
	adds r2, r5, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	adds r2, #4
	movs r3, #8
	strb r3, [r2]
	adds r0, r5, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	adds r0, r5, #0
	movs r1, #15
	bl Object_SetPartAttribute
	adds r1, r5, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #34
	orrs r3, r2
	strb r3, [r1]
	adds r0, r5, #0
	b .L_02008120
.L_0200811e:
	movs r0, #0
.L_02008120:
	pop {r5, r6, pc}
	.2byte 0x0000
	.section .text.x02008124,"ax",%progbits
	.global Func_02000124
	.thumb_func
Func_02000124:
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
	.section .text.x0200815c,"ax",%progbits
	.global Func_0200015c
	.thumb_func
Func_0200015c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #4
	str r3, [sp, #0]
	ldr r3, .L_0200832c
	mov r8, r2
	movs r2, #133
	lsls r2, r2, #2
	adds r6, r1, #0
	ldr r1, [sp, #44]
	adds r3, r3, r2
	adds r5, r0, #0
	ldr r0, [r3]
	mov r10, r1
	ldr r7, [sp, #48]
	bl Object_GetById
	movs r3, #128
	lsls r3, r3, #13
	mov r1, r10
	ands r3, r1
	mov r9, r0
	cmp r3, #0
	beq .L_020081a4
	cmp r7, #0
	beq .L_020081a4
	movs r2, #24
	ldrsh r0, [r7, r2]
	adds r1, r5, #0
	adds r2, r6, #0
	b .L_020081ac
.L_020081a4:
	movs r0, #30
	adds r2, r6, #0
	adds r0, #255
	adds r1, r5, #0
.L_020081ac:
	mov r3, r8
	bl Func_02003b40
	adds r6, r0, #0
	cmp r6, #0
	bne .L_020081ba
	b .L_0200831e
.L_020081ba:
	ldr r3, [r6, #80]
	mov r1, r10
	movs r5, #15
	adds r1, #1
	ands r1, r5
	adds r0, r6, #0
	mov r8, r3
	bl Func_02003b28
	ldr r2, .L_02008330
	mov r3, r10
	ands r3, r5
	lsls r3, r3, #2
	ldr r1, [r2, r3]
	adds r0, r6, #0
	mov r11, r3
	bl Func_02003b38
	adds r3, r6, #0
	movs r5, #0
	adds r3, #85
	strb r5, [r3]
	adds r0, r6, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	ldr r3, .L_02008334
	mov r1, r9
	str r3, [r6, #108]
	ldr r3, [sp, #0]
	adds r0, r6, #0
	str r3, [r6, #68]
	ldr r3, [sp, #36]
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
	ldr r3, .L_02008338
	mov r1, r10
	ands r3, r1
	cmp r3, #0
	beq .L_0200831e
	cmp r7, #0
	beq .L_0200831e
	movs r3, #128
	lsls r3, r3, #9
	ands r3, r1
	cmp r3, #0
	beq .L_0200823c
	ldr r1, [r7, #4]
	adds r0, r6, #0
	bl Object_SetPartAttribute
.L_0200823c:
	movs r3, #128
	lsls r3, r3, #10
	mov r2, r10
	ands r3, r2
	cmp r3, #0
	beq .L_02008274
	adds r1, r6, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #254
	ands r3, r2
	strb r3, [r1]
	movs r3, #3
	ldrb r2, [r7]
	adds r0, r6, #0
	ands r2, r3
	mov r3, r8
	ldrb r1, [r3, #9]
	movs r3, #13
	negs r3, r3
	ands r3, r1
	lsls r2, r2, #2
	mov r1, r8
	orrs r3, r2
	strb r3, [r1, #9]
	ldr r1, [r7]
	bl Object_SetSpritePriority
.L_02008274:
	movs r2, #128
	lsls r2, r2, #12
	mov r3, r10
	ands r2, r3
	cmp r2, #0
	beq .L_02008288
	ldr r3, [r7, #8]
	str r3, [r6, #24]
	ldr r3, [r7, #12]
	str r3, [r6, #28]
.L_02008288:
	movs r3, #128
	lsls r3, r3, #11
	mov r1, r10
	ands r3, r1
	cmp r3, #0
	beq .L_020082ce
	ldr r3, .L_02008330
	mov r1, r11
	ldr r5, [r3, r1]
	ldr r3, [r7, #16]
	ldr r1, [r5, #12]
	cmp r2, #0
	beq .L_020082b6
	ldr r0, [r6, #24]
	subs r0, r3, r0
	bl Engine_MathDivide
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, [r6, #28]
	ldr r1, [r5, #12]
	subs r0, r0, r3
	b .L_020082c8
.L_020082b6:
	ldr r2, .L_02008338
	adds r0, r3, r2
	bl Engine_MathDivide
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, .L_02008338
	ldr r1, [r5, #12]
	adds r0, r0, r3
.L_020082c8:
	bl Engine_MathDivide
	str r0, [r6, #52]
.L_020082ce:
	movs r3, #128
	lsls r3, r3, #14
	mov r1, r10
	ands r3, r1
	cmp r3, #0
	beq .L_020082ea
	adds r0, r6, #0
	movs r1, #1
	bl Func_02003b28
	ldr r1, [r7, #28]
	adds r0, r6, #0
	bl Func_02003b38
.L_020082ea:
	movs r3, #128
	lsls r3, r3, #15
	mov r2, r10
	ands r3, r2
	cmp r3, #0
	beq .L_020082fc
	ldrh r3, [r7, #32]
	mov r1, r8
	strh r3, [r1, #18]
.L_020082fc:
	movs r3, #128
	lsls r3, r3, #16
	mov r2, r10
	ands r3, r2
	cmp r3, #0
	beq .L_0200830e
	ldrh r3, [r7, #34]
	mov r1, r9
	strh r3, [r1]
.L_0200830e:
	movs r3, #128
	lsls r3, r3, #17
	mov r2, r10
	ands r3, r2
	cmp r3, #0
	beq .L_0200831e
	ldr r3, [r7, #36]
	str r3, [r6, #108]
.L_0200831e:
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0200832c:
	.4byte gPartyState
.L_02008330:
	.4byte Data_02003da0
.L_02008334:
	.4byte Func_02000124
.L_02008338:
	.4byte 0xffff0000
	.section .text.x0200833c,"ax",%progbits
	.global Func_0200033c
	.thumb_func
Func_0200033c:
	push {lr}
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #0
	pop {pc}
	.section .text.x02008348,"ax",%progbits
	.global Func_02000348
	.thumb_func
Func_02000348:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r4, [r3, #32]
	cmp r4, #0
	bne .L_02008358
	movs r0, #0
	b .L_0200837e
.L_02008358:
	cmp r0, #2
	bhi .L_0200836c
	lsls r3, r0, #3
	subs r3, r3, r0
	movs r0, #156
	lsls r3, r3, #3
	lsls r0, r0, #1
	adds r3, r3, r0
	ldr r4, [r4, r3]
	b .L_0200836e
.L_0200836c:
	ldr r4, .L_02008380
.L_0200836e:
	lsls r3, r2, #7
	adds r3, r1, r3
	lsls r3, r3, #2
	adds r4, r4, r3
	ldrb r0, [r4, #2]
	ldrb r3, [r4, #3]
	lsls r0, r0, #8
	orrs r0, r3
.L_0200837e:
	pop {pc}
.L_02008380:
	.4byte gMapCellBuffer
	.section .text.x02008384,"ax",%progbits
	.global Func_02000384
	.thumb_func
Func_02000384:
	push {r5, lr}
	adds r5, r1, #0
	adds r1, r3, #0
	movs r3, #192
	lsls r3, r3, #18
	ldr r4, [r3, #32]
	cmp r4, #0
	bne .L_02008398
	movs r0, #0
	b .L_020083c4
.L_02008398:
	cmp r0, #2
	bhi .L_020083ac
	lsls r3, r0, #3
	subs r3, r3, r0
	movs r0, #156
	lsls r3, r3, #3
	lsls r0, r0, #1
	adds r3, r3, r0
	ldr r4, [r4, r3]
	b .L_020083ae
.L_020083ac:
	ldr r4, .L_020083c8
.L_020083ae:
	lsls r3, r2, #7
	adds r3, r5, r3
	lsls r3, r3, #2
	adds r4, r4, r3
	ldrb r0, [r4, #2]
	ldrb r3, [r4, #3]
	lsls r0, r0, #8
	orrs r0, r3
	asrs r3, r1, #8
	strb r3, [r4, #2]
	strb r1, [r4, #3]
.L_020083c4:
	pop {r5, pc}
	.2byte 0x0000
.L_020083c8:
	.4byte gMapCellBuffer
	.section .text.x020083cc,"ax",%progbits
	.global Func_020003cc
	.thumb_func
Func_020003cc:
	push {r5, r6, lr}
	adds r6, r0, #0
	movs r5, #60
.L_020083d2:
	cmp r5, #0
	beq .L_020083e4
	movs r0, #1
	bl WaitFrames
	ldr r3, [r6, #40]
	subs r5, #1
	cmp r3, #0
	bne .L_020083d2
.L_020083e4:
	pop {r5, r6, pc}
	.2byte 0x0000
	.section .text.x020083e8,"ax",%progbits
	.global Func_020003e8
	.thumb_func
Func_020003e8:
	push {r5, r6, r7, lr}
	adds r6, r0, #0
	b .L_02008458
.L_020083ee:
	ldrh r0, [r6]
	bl Object_GetById
	adds r5, r0, #0
	adds r7, r5, #0
	movs r3, #0
	adds r7, #99
	strb r3, [r7]
	movs r0, #0
	ldr r1, [r5, #8]
	ldr r2, [r5, #16]
	asrs r1, r1, #20
	asrs r2, r2, #20
	bl Func_02000348
	movs r3, #64
	ands r0, r3
	cmp r0, #0
	beq .L_02008456
	movs r3, #192
	lsls r3, r3, #18
	ldr r0, [r3, #32]
	cmp r0, #0
	beq .L_02008464
	movs r3, #212
	lsls r3, r3, #1
	adds r0, r0, r3
	ldr r2, [r5, #16]
	ldr r3, [r5, #12]
	ldr r1, [r5, #8]
	subs r2, r2, r3
	asrs r2, r2, #20
	adds r3, r2, #1
	asrs r1, r1, #20
	ldr r4, [r0]
	lsls r3, r3, #7
	adds r3, r1, r3
	lsls r3, r3, #2
	adds r4, r4, r3
	subs r2, #1
	movs r3, #255
	strb r3, [r4, #2]
	lsls r2, r2, #7
	ldr r4, [r0]
	adds r1, r1, r2
	lsls r1, r1, #2
	movs r3, #1
	negs r3, r3
	adds r4, r4, r1
	strb r3, [r4, #2]
	movs r3, #1
	strb r3, [r7]
.L_02008456:
	adds r6, #2
.L_02008458:
	movs r2, #255
	ldrh r3, [r6]
	lsls r2, r2, #8
	adds r2, #255
	cmp r3, r2
	bne .L_020083ee
.L_02008464:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x02008468,"ax",%progbits
	.global Func_02000468
	.thumb_func
Func_02000468:
	push {r5, lr}
	adds r5, r0, #0
	movs r2, #99
	adds r2, r2, r5
	ldrb r3, [r2]
	mov r12, r2
	cmp r3, #0
	beq .L_020084b6
	movs r3, #192
	lsls r3, r3, #18
	ldr r0, [r3, #32]
	cmp r0, #0
	beq .L_020084b6
	movs r3, #212
	lsls r3, r3, #1
	adds r0, r0, r3
	ldr r2, [r5, #16]
	ldr r3, [r5, #12]
	ldr r1, [r5, #8]
	subs r2, r2, r3
	asrs r2, r2, #20
	adds r3, r2, #1
	asrs r1, r1, #20
	ldr r4, [r0]
	lsls r3, r3, #7
	adds r3, r1, r3
	lsls r3, r3, #2
	adds r4, r4, r3
	subs r2, #1
	movs r3, #0
	strb r3, [r4, #2]
	lsls r2, r2, #7
	ldr r4, [r0]
	adds r1, r1, r2
	lsls r1, r1, #2
	adds r4, r4, r1
	mov r2, r12
	strb r3, [r4, #2]
	strb r3, [r2]
.L_020084b6:
	pop {r5, pc}
	.section .text.x020084b8,"ax",%progbits
	.global Func_020004b8
	.thumb_func
Func_020004b8:
	ldr r0, .L_020084bc
	bx lr
.L_020084bc:
	.4byte Data_02003e54
	.section .text.x020084c0,"ax",%progbits
	.global Func_020004c0
	.thumb_func
Func_020004c0:
	push {lr}
	ldr r3, .L_020084e8
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_020084ec
	cmp r2, r3
	bne .L_020084d8
	ldr r0, .L_020084f0
	b .L_020084e4
.L_020084d8:
	ldr r3, .L_020084f4
	cmp r2, r3
	bne .L_020084e2
	ldr r0, .L_020084f8
	b .L_020084e4
.L_020084e2:
	ldr r0, .L_020084fc
.L_020084e4:
	pop {pc}
	.2byte 0x0000
.L_020084e8:
	.4byte gPartyState
.L_020084ec:
	.4byte 0x00000052
.L_020084f0:
	.4byte Data_02003e84
.L_020084f4:
	.4byte 0x00000053
.L_020084f8:
	.4byte Data_02003ec4
.L_020084fc:
	.4byte Data_02003f44
	.section .text.x02008500,"ax",%progbits
	.global Func_02000500
	.thumb_func
Func_02000500:
	ldr r0, .L_02008504
	bx lr
.L_02008504:
	.4byte Data_02003ff4
	.section .text.x02008508,"ax",%progbits
	.global Func_02000508
	.thumb_func
Func_02000508:
	push {lr}
	ldr r3, .L_02008530
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008534
	cmp r2, r3
	bne .L_02008520
	ldr r0, .L_02008538
	b .L_0200852c
.L_02008520:
	ldr r3, .L_0200853c
	cmp r2, r3
	bne .L_0200852a
	ldr r0, .L_02008540
	b .L_0200852c
.L_0200852a:
	ldr r0, .L_02008544
.L_0200852c:
	pop {pc}
	.2byte 0x0000
.L_02008530:
	.4byte gPartyState
.L_02008534:
	.4byte 0x00000052
.L_02008538:
	.4byte Data_020040ec
.L_0200853c:
	.4byte 0x00000053
.L_02008540:
	.4byte Data_02004194
.L_02008544:
	.4byte Data_02004314
	.section .text.x02008548,"ax",%progbits
	.global Func_02000548
	.thumb_func
Func_02000548:
	push {lr}
	bl Func_02003bc8
	movs r0, #0
	bl Func_02003c88
	ldr r0, .L_02008564
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
	bl Func_02003bd0
	pop {pc}
	.2byte 0x0000
.L_02008564:
	.4byte 0x00001a94
	.section .text.x02008568,"ax",%progbits
	.global Func_02000568
	.thumb_func
Func_02000568:
	push {r5, lr}
	sub sp, #8
	bl Func_02003bc8
	movs r0, #0
	bl Func_02003c88
	movs r0, #158
	bl Func_02003cd8
	movs r3, #1
	movs r2, #2
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #64
	movs r2, #83
	movs r3, #35
	movs r0, #64
	bl Func_02003b68
	movs r0, #0
	bl Func_02003b70
	movs r0, #45
	bl Battle_WaitMode0
	ldr r5, .L_020085e8
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	movs r1, #128
	movs r2, #128
	ldr r0, [r5]
	lsls r1, r1, #8
	lsls r2, r2, #7
	bl ObjectMotion_SetSpeedParameters
	movs r1, #156
	movs r2, #170
	lsls r1, r1, #1
	lsls r2, r2, #2
	ldr r0, [r5]
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r0, #5
	bl Battle_WaitMode0
	bl Event_ClearStatus1c6
	bl Event_WaitValue1c8Frames
	ldr r0, [r5]
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #123
	bl Func_02003cd8
	movs r0, #4
	bl Func_02003c48
	bl Func_02003bd0
	add sp, #8
	pop {r5, pc}
.L_020085e8:
	.4byte gPartyState
	.section .text.x020085ec,"ax",%progbits
	.global Func_020005ec
	.thumb_func
Func_020005ec:
	push {r5, lr}
	movs r0, #10
	bl Object_GetById
	adds r5, r0, #0
	bl Func_02003bc8
	movs r0, #0
	bl Func_02003c88
	ldr r3, [r5, #8]
	asrs r3, r3, #20
	cmp r3, #31
	bne .L_02008612
	movs r0, #196
	lsls r0, r0, #2
	bl GameFlag_SetBit
	b .L_0200861a
.L_02008612:
	movs r0, #196
	lsls r0, r0, #2
	bl GameFlag_ClearBit
.L_0200861a:
	bl Func_02003bd0
	pop {r5, pc}
	.section .text.x02008620,"ax",%progbits
	.global Func_02000620
	.thumb_func
Func_02000620:
	push {r5, r6, lr}
	movs r0, #11
	sub sp, #8
	bl Object_GetById
	movs r1, #70
	movs r2, #67
	adds r5, r0, #0
	movs r0, #3
	bl Func_02000348
	adds r6, r0, #0
	bl Func_02003bc8
	movs r0, #0
	bl Func_02003c88
	ldr r3, [r5, #8]
	asrs r3, r3, #20
	cmp r3, #32
	bne .L_02008672
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #17
	bl GameFlag_SetBit
	movs r3, #255
	ands r6, r3
	movs r1, #70
	movs r2, #67
	adds r3, r6, #0
	movs r0, #4
	bl Func_02000384
	movs r0, #2
	movs r1, #33
	movs r2, #54
	adds r3, r6, #0
	bl Func_02000384
	b .L_0200869c
.L_02008672:
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #17
	bl GameFlag_ClearBit
	movs r3, #255
	ands r6, r3
	lsls r3, r3, #8
	orrs r6, r3
	movs r1, #70
	movs r2, #67
	adds r3, r6, #0
	movs r0, #4
	bl Func_02000384
	movs r0, #2
	movs r1, #33
	movs r2, #54
	adds r3, r6, #0
	bl Func_02000384
.L_0200869c:
	ldr r3, .L_020086d0
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	ldr r3, [r0, #12]
	asrs r3, r3, #19
	cmp r3, #26
	bne .L_020086c6
	movs r3, #33
	movs r2, #54
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #70
	movs r1, #67
	movs r2, #1
	movs r3, #1
	bl Func_02003b80
.L_020086c6:
	bl Func_02003bd0
	add sp, #8
	pop {r5, r6, pc}
	.2byte 0x0000
.L_020086d0:
	.4byte gPartyState
	.section .text.x020086d4,"ax",%progbits
	.global Func_020006d4
	.thumb_func
Func_020006d4:
	push {r5, lr}
	movs r0, #13
	bl Object_GetById
	adds r5, r0, #0
	bl Func_02003bc8
	movs r0, #0
	bl Func_02003c88
	ldr r3, [r5, #8]
	asrs r3, r3, #19
	cmp r3, #23
	bne .L_020086fa
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #121
	bl GameFlag_SetBit
.L_020086fa:
	bl Func_02003bd0
	pop {r5, pc}
	.section .text.x02008700,"ax",%progbits
	.global Func_02000700
	.thumb_func
Func_02000700:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r5, .L_020087dc
	movs r0, #133
	lsls r0, r0, #2
	adds r5, r5, r0
	ldr r0, [r5]
	bl Object_GetById
	adds r7, r0, #0
	movs r0, #195
	lsls r0, r0, #1
	ldr r6, [r7, #16]
	bl Func_02003cd8
	movs r1, #6
	adds r0, r7, #0
	bl Func_02003b28
	movs r0, #10
	bl WaitFrames
	adds r0, r7, #0
	movs r1, #1
	bl Func_02003b28
	adds r0, r7, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r1, #85
	adds r1, r1, r7
	ldrb r2, [r1]
	movs r3, #2
	orrs r3, r2
	movs r0, #0
	strb r3, [r1]
	mov r10, r0
	movs r3, #128
	movs r0, #192
	lsls r3, r3, #11
	lsls r0, r0, #12
	ldr r2, [r7, #12]
	mov r8, r1
	str r3, [r7, #40]
	ldr r1, [r7, #8]
	adds r3, r6, r0
	adds r0, r7, #0
	bl Func_02003b58
	movs r0, #6
	bl WaitFrames
	movs r0, #217
	bl Func_02003cd8
	movs r1, #160
	ldr r0, [r5]
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	mov r1, r10
	mov r2, r8
	strb r1, [r2]
	movs r5, #0
.L_02008788:
	ldr r3, [r7, #12]
	ldr r0, .L_020087e0
	adds r5, #1
	adds r3, r3, r0
	str r3, [r7, #12]
	str r3, [r7, #60]
	movs r0, #1
	bl WaitFrames
	cmp r5, #13
	bls .L_02008788
	mov r1, r8
	movs r3, #3
	strb r3, [r1]
	movs r0, #128
	movs r3, #192
	lsls r3, r3, #10
	lsls r0, r0, #13
	ldr r2, [r7, #12]
	ldr r1, [r7, #8]
	str r3, [r7, #40]
	adds r3, r6, r0
	adds r0, r7, #0
	bl Func_02003b58
	adds r0, r7, #0
	bl Func_02003b60
	adds r0, r7, #0
	movs r1, #1
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #195
	lsls r0, r0, #1
	bl Func_02003cd8
	bl Func_02003c98
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_020087dc:
	.4byte gPartyState
.L_020087e0:
	.4byte 0xfffe0000
	.section .text.x020087e4,"ax",%progbits
	.global Func_020007e4
	.thumb_func
Func_020007e4:
	push {lr}
	ldr r3, .L_0200880c
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008810
	cmp r2, r3
	bne .L_020087fc
	ldr r0, .L_02008814
	b .L_02008808
.L_020087fc:
	ldr r3, .L_02008818
	cmp r2, r3
	bne .L_02008806
	ldr r0, .L_0200881c
	b .L_02008808
.L_02008806:
	ldr r0, .L_02008820
.L_02008808:
	pop {pc}
	.2byte 0x0000
.L_0200880c:
	.4byte gPartyState
.L_02008810:
	.4byte 0x00000052
.L_02008814:
	.4byte Data_020043d4
.L_02008818:
	.4byte 0x00000053
.L_0200881c:
	.4byte Data_020044d0
.L_02008820:
	.4byte Data_0200459c
	.section .text.x02008824,"ax",%progbits
	.global Func_02000824
	.thumb_func
Func_02000824:
	push {lr}
	movs r0, #0
	bl Func_02003c88
	pop {pc}
	.2byte 0x0000
	.section .text.x02008830,"ax",%progbits
	.global Func_02000830
	.thumb_func
Func_02000830:
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #224
	ldr r3, [r3]
	movs r2, #1
	adds r3, #52
	strb r2, [r3]
	bx lr
	.section .text.x02008840,"ax",%progbits
	.global Func_02000840
	.thumb_func
Func_02000840:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r8
	push {r5, r6}
	adds r5, r0, #0
	ldr r6, [r5, #12]
	ldr r0, [r5, #48]
	movs r3, #128
	lsls r3, r3, #13
	asrs r6, r6, #1
	adds r6, r6, r3
	movs r3, #255
	ands r0, r3
	lsls r0, r0, #11
	mov r8, r3
	bl Math_Cosine
	ldr r3, .L_020088a8
	adds r1, r6, #0
	mov r10, r3
	mov lr, r10
	.2byte 0xf800
	ldr r3, [r5, #68]
	adds r3, r3, r0
	ldr r0, [r5, #48]
	str r3, [r5, #8]
	mov r3, r8
	ands r0, r3
	lsls r0, r0, #11
	bl Math_Sine
	adds r1, r6, #0
	mov lr, r10
	.2byte 0xf800
	movs r1, #3
	bl Engine_MathDivide
	ldr r3, [r5, #76]
	ldr r2, [r5, #72]
	adds r3, r3, r0
	str r3, [r5, #16]
	ldr r3, [r5, #12]
	adds r3, r3, r2
	str r3, [r5, #12]
	ldr r3, [r5, #48]
	adds r3, #1
	str r3, [r5, #48]
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, pc}
	.2byte 0x0000
.L_020088a8:
	.4byte IwramMulQ16
	.section .text.x020088ac,"ax",%progbits
	.global Func_020008ac
	.thumb_func
Func_020008ac:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r8
	push {r5, r6}
	adds r5, r0, #0
	ldr r6, [r5, #12]
	ldr r0, [r5, #48]
	movs r3, #192
	lsls r3, r3, #12
	asrs r6, r6, #2
	adds r6, r6, r3
	movs r3, #255
	ands r0, r3
	lsls r0, r0, #11
	mov r8, r3
	bl Math_Cosine
	ldr r3, .L_02008924
	adds r1, r6, #0
	mov r10, r3
	mov lr, r10
	.2byte 0xf800
	ldr r3, [r5, #68]
	lsls r0, r0, #1
	adds r3, r3, r0
	ldr r0, [r5, #48]
	str r3, [r5, #8]
	mov r3, r8
	ands r0, r3
	lsls r0, r0, #11
	bl Math_Sine
	adds r1, r6, #0
	mov lr, r10
	.2byte 0xf800
	ldr r3, [r5, #76]
	ldr r2, [r5, #72]
	adds r3, r3, r0
	str r3, [r5, #16]
	ldr r3, [r5, #12]
	adds r0, r5, #0
	adds r3, r3, r2
	str r3, [r5, #12]
	ldr r3, [r5, #48]
	movs r2, #2
	adds r3, #1
	str r3, [r5, #48]
	ldr r3, .L_02008928
	ldr r3, [r3]
	ands r3, r2
	lsrs r3, r3, #1
	lsls r1, r3, #3
	adds r1, r1, r3
	bl Object_SetPartAttribute
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02008924:
	.4byte IwramMulQ16
.L_02008928:
	.4byte Data_0300122c
	.section .text.x0200892c,"ax",%progbits
	.global Func_0200092c
	.thumb_func
Func_0200092c:
	push {lr}
	ldr r3, .L_02008958
	movs r2, #3
	ldr r3, [r3]
	ands r3, r2
	adds r2, r0, #0
	adds r2, #99
	ldrb r2, [r2]
	lsrs r3, r2
	movs r2, #1
	ands r3, r2
	adds r2, r0, #0
	adds r2, #98
	ldrb r2, [r2]
	adds r1, r2, #0
	muls r1, r3
	lsls r1, r1, #24
	lsrs r1, r1, #24
	bl Object_SetPartAttribute
	pop {pc}
	.2byte 0x0000
.L_02008958:
	.4byte Data_0300122c
	.section .text.x0200895c,"ax",%progbits
	.global Func_0200095c
	.thumb_func
Func_0200095c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, .L_02008ccc
	movs r0, #133
	lsls r0, r0, #2
	adds r3, r3, r0
	ldr r0, [r3]
	sub sp, #128
	bl Object_GetById
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #224
	ldr r3, [r3]
	movs r1, #0
	str r1, [sp, #16]
	mov r8, r3
	mov r10, r0
	mov r9, r1
	bl Func_02003bc8
	movs r0, #0
	bl Func_02003c88
	movs r0, #207
	bl Func_02003cd8
	mov r5, r10
	mov r2, r10
	movs r0, #140
	ldr r3, [r5, #16]
	ldr r1, [r2, #8]
	lsls r0, r0, #1
	ldr r2, [r2, #12]
	bl Func_02003b40
	movs r1, #2
	adds r7, r0, #0
	bl Func_02003b28
	adds r0, r7, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	ldr r3, .L_02008cd0
	add r0, sp, #16
	str r3, [r7, #24]
	movs r3, #204
	lsls r3, r3, #6
	ldrb r0, [r0]
	adds r3, #51
	str r3, [r7, #28]
	adds r3, r7, #0
	adds r3, #85
	mov r1, r8
	strb r0, [r3]
	ldr r0, [r1, #20]
	ldr r4, [r7, #80]
	ldr r3, [r0, #80]
	ldrb r1, [r4, #9]
	ldrb r3, [r3, #9]
	movs r2, #12
	ands r2, r3
	movs r3, #13
	negs r3, r3
	ands r3, r1
	orrs r3, r2
	strb r3, [r4, #9]
	ldr r5, .L_02008cd4
	ldr r3, [r0, #16]
	ldr r1, [r0, #8]
	ldr r2, [r0, #12]
	adds r3, r3, r5
	adds r0, r7, #0
	bl Func_02003b58
	movs r0, #166
	ldr r3, [r7, #28]
	lsls r0, r0, #9
	adds r0, #203
	cmp r3, r0
	bgt .L_02008a26
.L_02008a0a:
	movs r1, #200
	lsls r1, r1, #5
	adds r1, #153
	adds r3, r3, r1
	str r3, [r7, #28]
	movs r0, #1
	bl Battle_WaitMode0
	movs r2, #166
	ldr r3, [r7, #28]
	lsls r2, r2, #9
	adds r2, #203
	cmp r3, r2
	ble .L_02008a0a
.L_02008a26:
	adds r0, r7, #0
	mov r5, r8
	bl Func_02003b60
	ldr r3, [r5, #20]
	ldr r0, .L_02008cd4
	ldr r3, [r3, #16]
	movs r1, #230
	adds r3, r3, r0
	str r3, [r7, #16]
	ldr r3, [r7, #28]
	lsls r1, r1, #9
	adds r1, #203
	cmp r3, r1
	bgt .L_02008b00
.L_02008a44:
	ldr r2, .L_02008cd8
	movs r3, #3
	ldr r6, [r2]
	mov r11, r2
	ands r6, r3
	cmp r6, #0
	bne .L_02008aba
	mov r5, r8
	ldr r3, [r5, #20]
	movs r0, #168
	ldr r1, [r3, #8]
	movs r2, #0
	ldr r3, [r3, #16]
	lsls r0, r0, #2
	bl Func_02003b40
	adds r5, r0, #0
	adds r1, r5, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #1
	orrs r3, r2
	strb r3, [r1]
	adds r3, r5, #0
	adds r3, #85
	strb r6, [r3]
	ldr r3, [r5, #8]
	str r3, [r5, #68]
	movs r3, #204
	lsls r3, r3, #8
	adds r3, #204
	str r3, [r5, #72]
	ldr r3, [r5, #16]
	str r3, [r5, #76]
	bl Random16Far
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	ands r3, r0
	str r3, [r5, #48]
	adds r0, r5, #0
	movs r1, #7
	bl Func_02003b28
	adds r0, r5, #0
	ldr r1, .L_02008cdc
	bl Func_02003b38
	adds r0, r5, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	adds r0, r5, #0
	movs r1, #2
	bl Object_SetPartAttribute
	ldr r3, .L_02008ce0
	str r3, [r5, #108]
.L_02008aba:
	mov r0, r11
	ldr r3, [r0]
	movs r2, #1
	ands r3, r2
	cmp r3, #0
	beq .L_02008ae4
	ldr r3, [r7, #24]
	movs r2, #200
	lsls r2, r2, #1
	adds r2, #255
	adds r3, r3, r2
	str r3, [r7, #24]
	ldr r3, [r7, #28]
	mov r1, r8
	adds r3, r3, r2
	ldr r0, [r1, #20]
	str r3, [r7, #28]
	movs r1, #15
	bl Object_SetPartAttribute
	b .L_02008aee
.L_02008ae4:
	mov r2, r8
	ldr r0, [r2, #20]
	movs r1, #4
	bl Object_SetPartAttribute
.L_02008aee:
	movs r0, #1
	bl Battle_WaitMode0
	movs r5, #230
	ldr r3, [r7, #28]
	lsls r5, r5, #9
	adds r5, #203
	cmp r3, r5
	ble .L_02008a44
.L_02008b00:
	mov r1, r8
	ldr r0, [r1, #20]
	movs r1, #0
	bl Object_SetPartAttribute
	movs r0, #136
	bl Func_02003cd8
	mov r3, r8
	ldr r2, [r3, #20]
	mov r0, r10
	ldr r3, [r2, #8]
	ldrh r5, [r0, #6]
	asrs r3, r3, #20
	str r3, [sp, #20]
	ldr r3, [r2, #16]
	movs r1, #128
	asrs r3, r3, #20
	str r3, [sp, #24]
	lsls r1, r1, #6
	movs r3, #192
	adds r5, r5, r1
	lsls r3, r3, #8
	ands r5, r3
	adds r0, r5, #0
	bl Math_Cosine
	asrs r0, r0, #16
	str r0, [sp, #28]
	adds r0, r5, #0
	bl Math_Sine
	asrs r0, r0, #16
	str r0, [sp, #32]
.L_02008b44:
	ldr r1, [sp, #20]
	ldr r2, [sp, #24]
	movs r0, #2
	bl Func_02000348
	ldr r3, [sp, #20]
	ldr r5, [sp, #28]
	mov r11, r0
	adds r1, r3, r5
	ldr r0, [sp, #24]
	mov r2, r11
	ldr r3, [sp, #32]
	asrs r2, r2, #8
	mov r11, r2
	movs r5, #2
	adds r2, r0, r3
	mov r0, r11
	str r1, [sp, #20]
	str r2, [sp, #24]
	add r9, r5
	cmp r0, #0
	beq .L_02008b44
	movs r0, #2
	bl Func_02000348
	adds r1, r0, #0
	asrs r1, r1, #8
	subs r1, #1
	str r1, [sp, #16]
	movs r2, #2
	ldr r0, [sp, #28]
	mov r3, r8
	negs r2, r2
	add r9, r2
	ldr r2, [r3, #20]
	mov r3, r9
	muls r3, r0
	movs r5, #10
	ldrsh r6, [r2, r5]
	movs r1, #18
	ldrsh r5, [r2, r1]
	ldr r2, [sp, #32]
	lsls r3, r3, #3
	ldr r1, [r7, #80]
	adds r6, r6, r3
	mov r3, r9
	muls r3, r2
	ldrb r2, [r1, #9]
	lsls r3, r3, #3
	adds r5, r5, r3
	movs r3, #13
	negs r3, r3
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	strb r3, [r1, #9]
	movs r3, #128
	lsls r3, r3, #11
	str r3, [r7, #48]
	str r3, [r7, #52]
	adds r0, r5, #0
	adds r3, r6, #0
	ldr r2, [r7, #12]
	lsls r1, r3, #16
	lsls r3, r0, #16
	adds r0, r7, #0
	str r6, [sp, #20]
	str r5, [sp, #24]
	bl Func_02003b58
	movs r0, #160
	movs r1, #160
	lsls r0, r0, #11
	lsls r1, r1, #11
	bl Func_02003c20
	lsls r6, r6, #16
	lsls r5, r5, #16
	movs r1, #1
	adds r0, r6, #0
	negs r1, r1
	adds r2, r5, #0
	movs r3, #1
	bl Motion_CamBounds
	b .L_02008c6c
.L_02008bf0:
	ldr r1, .L_02008cd8
	movs r3, #1
	ldr r0, [r1]
	mov r9, r1
	mov r10, r0
	mov r2, r10
	ands r2, r3
	mov r10, r2
	cmp r2, #0
	bne .L_02008c66
	add r6, sp, #76
	str r3, [r6]
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #162
	strh r3, [r6, #24]
	movs r3, #2
	str r3, [r6, #4]
	movs r3, #128
	lsls r3, r3, #1
	adds r3, #255
	ands r0, r3
	lsls r0, r0, #12
	mov r8, r3
	bl Math_Cosine
	add r5, sp, #116
	lsls r0, r0, #1
	str r0, [r5]
	bl Random16Far
	ldr r3, [r7, #12]
	movs r2, #31
	ands r2, r0
	lsls r2, r2, #16
	adds r3, r3, r2
	str r3, [r5, #4]
	mov r1, r9
	ldr r0, [r1]
	mov r2, r8
	ands r0, r2
	lsls r0, r0, #12
	bl Math_Sine
	str r0, [r5, #8]
	ldr r4, [r7, #8]
	ldr r1, [r5, #4]
	ldr r3, [r5]
	ldr r2, [r7, #16]
	str r0, [sp, #4]
	movs r0, #152
	lsls r0, r0, #13
	mov r5, r10
	str r0, [sp, #8]
	adds r0, r4, #0
	str r5, [sp, #0]
	str r6, [sp, #12]
	bl Func_0200015c
.L_02008c66:
	movs r0, #1
	bl Battle_WaitMode0
.L_02008c6c:
	adds r0, r7, #0
	bl Func_02003ba8
	cmp r0, #0
	beq .L_02008bf0
	mov r0, r11
	cmp r0, #255
	bne .L_02008c84
	movs r0, #136
	bl Func_02003cd8
	b .L_02008e78
.L_02008c84:
	ldr r5, [sp, #28]
	movs r1, #10
	ldrsh r2, [r7, r1]
	lsls r3, r5, #1
	ldr r1, [sp, #32]
	adds r3, r3, r5
	lsls r3, r3, #4
	adds r3, r2, r3
	str r3, [sp, #20]
	movs r0, #18
	ldrsh r2, [r7, r0]
	lsls r3, r1, #1
	adds r3, r3, r1
	lsls r3, r3, #4
	adds r3, r2, r3
	str r3, [sp, #24]
	ldr r2, [sp, #20]
	ldr r5, [sp, #24]
	movs r3, #128
	lsls r3, r3, #8
	lsls r1, r2, #16
	str r3, [r7, #48]
	str r3, [r7, #52]
	ldr r2, [r7, #12]
	lsls r3, r5, #16
	adds r0, r7, #0
	bl Func_02003b58
	movs r0, #235
	bl Func_02003cd8
	movs r0, #30
	negs r0, r0
	add r11, r0
	b .L_02008e36
	.2byte 0x0000
.L_02008ccc:
	.4byte gPartyState
.L_02008cd0:
	.4byte 0x0001b333
.L_02008cd4:
	.4byte 0xffff0000
.L_02008cd8:
	.4byte Data_0300122c
.L_02008cdc:
	.4byte Data_02003d1c
.L_02008ce0:
	.4byte Func_02000840
.L_02008ce4:
	ldr r1, .L_02008ee0
	movs r2, #1
	ldr r3, [r1]
	mov r9, r1
	ands r3, r2
	cmp r3, #0
	bne .L_02008d5a
	add r3, sp, #36
	str r2, [r3]
	mov r8, r3
	bl Random16Far
	movs r3, #255
	lsls r3, r3, #8
	movs r5, #128
	lsls r5, r5, #9
	adds r3, #255
	mov r10, r5
	ands r3, r0
	add r3, r10
	mov r0, r8
	str r3, [r0, #12]
	str r3, [r0, #8]
	mov r1, r9
	ldr r0, [r1]
	movs r6, #128
	lsls r6, r6, #1
	adds r6, #255
	ands r0, r6
	lsls r0, r0, #12
	bl Math_Cosine
	mov r2, r10
	add r5, sp, #116
	lsls r0, r0, #1
	str r2, [r5, #4]
	str r0, [r5]
	mov r3, r9
	ldr r0, [r3]
	ands r0, r6
	lsls r0, r0, #12
	bl Math_Sine
	str r0, [r5, #8]
	ldr r6, [r7, #8]
	ldr r3, [r5]
	ldr r4, [r5, #4]
	ldr r1, [r7, #12]
	ldr r2, [r7, #16]
	str r0, [sp, #4]
	movs r0, #160
	lsls r0, r0, #12
	str r0, [sp, #8]
	mov r5, r8
	adds r0, r6, #0
	str r4, [sp, #0]
	str r5, [sp, #12]
	bl Func_0200015c
.L_02008d5a:
	mov r0, r9
	ldr r3, [r0]
	movs r4, #3
	ands r3, r4
	cmp r3, #0
	bne .L_02008e30
	ldr r6, .L_02008ee4
	mov r1, r11
	lsls r5, r1, #3
	adds r3, r5, #4
	ldrsh r2, [r6, r3]
	ldr r3, [r7, #8]
	ldrh r0, [r6, r5]
	asrs r3, r3, #20
	adds r2, r2, r3
	adds r3, r5, #6
	mov r10, r2
	str r2, [sp, #20]
	ldrsh r2, [r6, r3]
	ldr r3, [r7, #16]
	adds r1, r5, #2
	asrs r3, r3, #20
	adds r2, r2, r3
	mov r9, r2
	str r2, [sp, #24]
	mov r3, r11
	movs r2, #1
	ands r3, r2
	mov r8, r1
	cmp r3, #0
	beq .L_02008de0
	ldrsh r1, [r6, r1]
	lsls r0, r0, #16
	movs r2, #1
	mov r3, r9
	subs r3, #1
	str r2, [sp, #0]
	asrs r0, r0, #16
	mov r2, r10
	str r4, [sp, #4]
	bl Func_02003b68
	mov r2, r8
	ldrsh r1, [r6, r2]
	ldrsh r0, [r6, r5]
	mov r2, r10
	mov r3, r9
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r2, #1
	movs r3, #2
	bl Func_02003b80
	mov r2, r8
	ldrsh r0, [r6, r5]
	ldrsh r1, [r6, r2]
	ldr r2, [r7, #8]
	ldr r3, [r7, #16]
	asrs r2, r2, #20
	asrs r3, r3, #20
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r2, #1
	movs r3, #2
	bl Func_02003b80
	b .L_02008e2c
.L_02008de0:
	mov r2, r8
	ldrsh r1, [r6, r2]
	mov r2, r11
	asrs r3, r2, #1
	ldr r2, [sp, #24]
	lsls r0, r0, #16
	subs r3, r2, r3
	movs r2, #1
	str r2, [sp, #0]
	str r2, [sp, #4]
	asrs r0, r0, #16
	mov r2, r10
	bl Func_02003b68
	mov r2, r8
	ldrsh r1, [r6, r2]
	ldrsh r0, [r6, r5]
	mov r2, r10
	mov r3, r9
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r2, #1
	movs r3, #1
	bl Func_02003b80
	mov r2, r8
	ldrsh r0, [r6, r5]
	ldrsh r1, [r6, r2]
	ldr r2, [r7, #8]
	ldr r3, [r7, #16]
	asrs r2, r2, #20
	asrs r3, r3, #20
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r2, #1
	movs r3, #1
	bl Func_02003b80
.L_02008e2c:
	bl Func_02003b50
.L_02008e30:
	movs r0, #1
	bl Battle_WaitMode0
.L_02008e36:
	adds r0, r7, #0
	bl Func_02003ba8
	cmp r0, #0
	bne .L_02008e42
	b .L_02008ce4
.L_02008e42:
	mov r5, r11
	cmp r5, #2
	bne .L_02008e64
	ldr r3, .L_02008ee4
	movs r1, #16
	ldrsh r0, [r3, r1]
	movs r2, #18
	ldrsh r1, [r3, r2]
	movs r2, #1
	str r2, [sp, #0]
	str r2, [sp, #4]
	ldr r3, [sp, #24]
	ldr r2, [sp, #20]
	bl Func_02003b68
	bl Func_02003b50
.L_02008e64:
	movs r0, #149
	lsls r0, r0, #1
	bl Func_02003cd8
	ldr r3, [sp, #16]
	movs r5, #133
	lsls r5, r5, #4
	adds r0, r3, r5
	bl GameFlag_SetBit
.L_02008e78:
	movs r3, #0
	str r3, [r7, #52]
	str r3, [r7, #48]
	str r3, [r7, #64]
	str r3, [r7, #60]
	str r3, [r7, #56]
	movs r5, #0
.L_02008e86:
	ldr r3, [r7, #24]
	ldr r0, .L_02008ee8
	ldr r1, .L_02008eec
	adds r3, r3, r0
	str r3, [r7, #24]
	ldr r3, [r7, #28]
	movs r2, #224
	adds r3, r3, r1
	str r3, [r7, #28]
	ldr r3, [r7, #12]
	lsls r2, r2, #10
	adds r3, r3, r2
	str r3, [r7, #12]
	movs r0, #1
	adds r5, #1
	bl Battle_WaitMode0
	cmp r5, #8
	bne .L_02008e86
	adds r0, r7, #0
	bl Func_02003b48
	movs r0, #30
	bl Battle_WaitMode0
	ldr r3, .L_02008ef0
	movs r5, #133
	lsls r5, r5, #2
	adds r3, r3, r5
	ldr r0, [r3]
	movs r1, #1
	bl Func_02003c38
	bl Func_02003c30
	bl Func_02003bd0
	add sp, #128
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008ee0:
	.4byte Data_0300122c
.L_02008ee4:
	.4byte Data_02004674
.L_02008ee8:
	.4byte 0xffffd99a
.L_02008eec:
	.4byte 0xffffc000
.L_02008ef0:
	.4byte gPartyState
	.section .text.x02008ef4,"ax",%progbits
	.global Func_02000ef4
	.thumb_func
Func_02000ef4:
	push {r5, r6, r7, lr}
	ldr r6, .L_02008f8c
	movs r7, #0
.L_02008efa:
	ldr r3, .L_02008f90
	movs r5, #160
	ldrb r0, [r3]
	subs r5, r5, r7
	adds r0, r7, r0
	lsls r0, r0, #8
	bl Math_Sine
	movs r1, #144
	subs r1, r1, r7
	ldr r3, .L_02008f94
	lsls r1, r1, #10
	mov lr, r3
	.2byte 0xf800
	asrs r0, r0, #11
	subs r0, #4
	asrs r5, r5, #2
	strh r0, [r6]
	bl Random16Far
	lsls r1, r5, #1
	bl Engine_MathModulo
	ldrh r3, [r6]
	subs r0, r0, r5
	adds r2, r3, r0
	lsls r3, r2, #16
	asrs r3, r3, #16
	strh r2, [r6]
	cmp r3, #56
	ble .L_02008f3e
	adds r3, r2, #0
	subs r3, #56
	b .L_02008f4a
.L_02008f3e:
	movs r1, #64
	negs r1, r1
	cmp r3, r1
	bge .L_02008f4c
	adds r3, r2, #0
	adds r3, #64
.L_02008f4a:
	strh r3, [r6]
.L_02008f4c:
	adds r7, #1
	adds r6, #2
	cmp r7, #160
	bne .L_02008efa
	ldr r6, .L_02008f8c
	movs r1, #128
	ldrh r3, [r6]
	lsls r1, r1, #19
	adds r1, #16
	strh r3, [r1]
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #176
	ldrh r0, [r3, #10]
	movs r2, #197
	lsls r2, r2, #8
	adds r2, #255
	ands r2, r0
	strh r2, [r3, #10]
	movs r2, #254
	ldrh r0, [r3, #10]
	lsls r2, r2, #7
	adds r2, #255
	ands r2, r0
	strh r2, [r3, #10]
	adds r0, r6, #2
	ldrh r2, [r3, #10]
	ldr r2, .L_02008f98
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008f8c:
	.4byte gOverlayArea + 0x46a0
.L_02008f90:
	.4byte Data_0300122c
.L_02008f94:
	.4byte IwramMulQ16
.L_02008f98:
	.4byte 0xa2600001
	.section .text.x02008f9c,"ax",%progbits
	.global Func_02000f9c
	.thumb_func
Func_02000f9c:
	push {r5, lr}
	adds r5, r0, #0
	ldr r3, [r5, #12]
	ldr r2, [r5, #72]
	ldr r1, [r5, #8]
	adds r3, r3, r2
	ldr r0, [r5, #68]
	str r3, [r5, #12]
	ldr r2, [r5, #76]
	ldr r3, [r5, #16]
	adds r1, r1, r0
	str r1, [r5, #8]
	adds r3, r3, r2
	asrs r1, r1, #19
	str r3, [r5, #16]
	cmp r1, #39
	bgt .L_02008fd0
	movs r1, #192
	lsls r1, r1, #9
	adds r3, r0, r1
	str r3, [r5, #68]
	movs r2, #192
	ldr r3, [r5, #24]
	lsls r2, r2, #4
	adds r2, #204
	b .L_02008fda
.L_02008fd0:
	ldr r1, .L_02009008
	ldr r2, .L_0200900c
	adds r3, r0, r1
	str r3, [r5, #68]
	ldr r3, [r5, #24]
.L_02008fda:
	adds r3, r3, r2
	str r3, [r5, #24]
	str r3, [r5, #28]
	ldr r3, [r5, #68]
	cmp r3, #0
	ble .L_02008ff0
	adds r0, r5, #0
	movs r1, #1
	bl Object_SetSpritePriority
	b .L_02008ff8
.L_02008ff0:
	adds r0, r5, #0
	movs r1, #2
	bl Object_SetSpritePriority
.L_02008ff8:
	ldr r2, [r5, #80]
	movs r1, #192
	ldrh r3, [r2, #18]
	lsls r1, r1, #5
	adds r3, r3, r1
	strh r3, [r2, #18]
	pop {r5, pc}
	.2byte 0x0000
.L_02009008:
	.4byte 0xfffe8000
.L_0200900c:
	.4byte 0xfffffae2
	.section .text.x02009010,"ax",%progbits
	.global Func_02001010
	.thumb_func
Func_02001010:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r0, #208
	lsls r0, r0, #3
	sub sp, #72
	bl Runtime_BumpAllocate
	adds r5, r0, #0
	bl Func_02003bc8
	movs r0, #0
	bl Func_02003c88
	movs r1, #128
	lsls r1, r1, #19
	ldrh r2, [r1]
	movs r3, #254
	lsls r3, r3, #8
	adds r3, #255
	ands r3, r2
	strh r3, [r1]
	ldr r0, .L_020090ec
	bl Resource_GetTableEntry
	adds r1, r5, #0
	bl Func_02003af0
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	adds r0, r5, #0
	ldr r1, .L_020090f0
	adds r2, #208
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r1, #208
	movs r2, #132
	lsls r1, r1, #2
	lsls r2, r2, #24
	adds r0, r5, r1
	adds r2, #208
	ldr r1, .L_020090f4
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	adds r0, r5, #0
	bl Sys_Free
	ldr r3, .L_020090f8
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	movs r1, #1
	bl ObjectMotion_SetActionVariant
	movs r0, #8
	movs r1, #1
	bl ObjectMotion_SetActionVariant
	movs r2, #128
	movs r3, #128
	lsls r2, r2, #3
	lsls r3, r3, #19
	adds r2, #13
	adds r3, #8
	strh r2, [r3]
	movs r1, #128
	lsls r1, r1, #19
	adds r1, #10
	ldrh r2, [r1]
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #252
	ands r3, r2
	strh r3, [r1]
	ldr r2, .L_020090e4
	ldrh r3, [r1]
	mov r0, sp
	orrs r3, r2
	strh r3, [r1]
	ldr r3, .L_020090e8
	adds r0, #70
	strh r3, [r0]
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #212
	ldr r1, .L_020090fc
	ldr r2, .L_02009100
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r0, #144
	lsls r0, r0, #2
	bl Runtime_BumpAllocate
	adds r5, r0, #0
	ldr r0, .L_02009104
	bl Resource_GetTableEntry
	adds r1, r5, #0
	bl Func_02003af0
	b .L_02009108
	.2byte 0x0000
.L_020090e4:
	.4byte 0x00000002
.L_020090e8:
	.4byte 0x00000010
.L_020090ec:
	.4byte 0x000001be
.L_020090f0:
	.4byte 0x0600e800
.L_020090f4:
	.4byte 0x0600ec00
.L_020090f8:
	.4byte gPartyState
.L_020090fc:
	.4byte 0x06002000
.L_02009100:
	.4byte 0x81000280
.L_02009104:
	.4byte 0x000001bf
.L_02009108:
	movs r7, #0
	adds r4, r5, #0
.L_0200910c:
	ldrh r3, [r4]
	movs r0, #255
	lsls r0, r0, #8
	adds r0, #64
	movs r1, #144
	adds r3, r3, r0
	adds r7, #1
	lsls r1, r1, #1
	strh r3, [r4]
	adds r4, #2
	cmp r7, r1
	bne .L_0200910c
	adds r4, r5, #0
	movs r7, #0
.L_02009128:
	ldr r2, .L_020092bc
	lsls r1, r7, #6
	adds r1, r1, r2
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	adds r0, r4, #0
	adds r2, #8
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	adds r7, #1
	adds r4, #32
	cmp r7, #18
	bne .L_02009128
	adds r0, r5, #0
	bl Sys_Free
	movs r0, #246
	bl Func_02003cd8
	movs r0, #8
	bl Object_GetById
	movs r3, #9
	adds r0, #98
	strb r3, [r0]
	movs r0, #8
	bl Object_GetById
	movs r3, #1
	adds r0, #99
	strb r3, [r0]
	movs r0, #8
	bl Object_GetById
	ldr r3, .L_020092c0
	movs r7, #0
	str r3, [r0, #108]
.L_02009178:
	ldr r3, .L_020092c4
	ldr r3, [r3]
	mov r8, r3
	mov r0, r8
	movs r3, #3
	ands r0, r3
	mov r8, r0
	cmp r0, #0
	bne .L_02009222
	movs r0, #8
	bl Object_GetById
	adds r6, r0, #0
	movs r0, #8
	bl Object_GetById
	adds r5, r0, #0
	movs r0, #8
	bl Object_GetById
	ldr r3, [r0, #16]
	movs r0, #168
	ldr r2, [r5, #12]
	ldr r1, [r6, #8]
	lsls r0, r0, #2
	bl Func_02003b40
	adds r5, r0, #0
	movs r0, #8
	bl Object_GetById
	ldr r3, [r0, #80]
	ldr r4, [r5, #80]
	ldrb r3, [r3, #9]
	movs r1, #12
	ands r1, r3
	movs r0, #13
	ldrb r3, [r4, #9]
	negs r0, r0
	adds r2, r0, #0
	ands r3, r2
	orrs r3, r1
	adds r1, r5, #0
	adds r1, #35
	ldrb r2, [r1]
	strb r3, [r4, #9]
	movs r3, #1
	orrs r3, r2
	strb r3, [r1]
	adds r3, r5, #0
	mov r1, r8
	adds r3, #85
	strb r1, [r3]
	ldr r3, [r5, #8]
	str r3, [r5, #68]
	movs r3, #192
	lsls r3, r3, #9
	str r3, [r5, #72]
	ldr r3, [r5, #16]
	str r3, [r5, #76]
	bl Random16Far
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	ands r3, r0
	str r3, [r5, #48]
	adds r0, r5, #0
	movs r1, #7
	bl Func_02003b28
	adds r0, r5, #0
	ldr r1, .L_020092c8
	bl Func_02003b38
	adds r0, r5, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	adds r0, r5, #0
	movs r1, #2
	bl Object_SetPartAttribute
	ldr r3, .L_020092cc
	str r3, [r5, #108]
.L_02009222:
	movs r0, #1
	adds r7, #1
	bl Battle_WaitMode0
	cmp r7, #45
	bne .L_02009178
	ldr r3, .L_020092d0
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	movs r1, #1
	bl Object_AttachWorkTargetToObject
	bl Func_02003c30
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #9
	lsls r1, r1, #6
	bl Func_02003c20
	movs r0, #165
	movs r1, #1
	movs r2, #146
	lsls r2, r2, #15
	movs r3, #1
	lsls r0, r0, #17
	negs r1, r1
	bl Motion_CamBounds
	bl Func_02003c30
	movs r1, #200
	lsls r1, r1, #4
	ldr r0, .L_020092d4
	bl Scheduler_AddOrUpdateCallback
	movs r0, #163
	bl Func_02003cd8
	ldr r3, .L_020092ac
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	ldr r3, .L_020092b0
	subs r2, #2
	strh r3, [r2]
	movs r1, #128
	lsls r1, r1, #19
	ldrh r3, [r1]
	ldr r2, .L_020092b4
	movs r0, #128
	orrs r3, r2
	strh r3, [r1]
	movs r2, #128
	movs r1, #128
	lsls r0, r0, #9
	lsls r1, r1, #9
	lsls r2, r2, #9
	bl Func_02003b98
	movs r7, #0
.L_020092a2:
	movs r3, #128
	ldr r6, .L_020092b8
	lsls r3, r3, #19
	b .L_020092d8
	.2byte 0x0000
.L_020092ac:
	.4byte 0x00000f00
.L_020092b0:
	.4byte 0x00003f41
.L_020092b4:
	.4byte 0x00000100
.L_020092b8:
	.4byte 0x0000000f
.L_020092bc:
	.4byte 0x0600200f
.L_020092c0:
	.4byte Func_0200092c
.L_020092c4:
	.4byte Data_0300122c
.L_020092c8:
	.4byte Data_02003d1c
.L_020092cc:
	.4byte Func_020008ac
.L_020092d0:
	.4byte gPartyState
.L_020092d4:
	.4byte Func_02000ef4
.L_020092d8:
	lsrs r2, r7, #2
	adds r3, #82
	mov r8, r3
	subs r3, r6, r2
	lsls r3, r3, #8
	orrs r3, r2
	mov r0, r8
	strh r3, [r0]
	movs r0, #8
	bl Object_GetById
	adds r5, r0, #0
	bl Random16Far
	movs r3, #7
	ands r0, r3
	adds r5, #98
	strb r0, [r5]
	adds r7, #1
	movs r0, #1
	bl Battle_WaitMode0
	cmp r7, #64
	bne .L_020092a2
	movs r0, #60
	bl Battle_WaitMode0
	ldr r3, .L_02009330
	movs r2, #128
	lsls r2, r2, #19
	mov r1, r8
	adds r2, #80
	strh r6, [r1]
	movs r0, #192
	strh r3, [r2]
	movs r1, #192
	movs r2, #128
	lsls r0, r0, #10
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl Func_02003b98
	movs r7, #0
	b .L_02009334
.L_02009330:
	.4byte 0x00000c42
.L_02009334:
	adds r7, #1
	movs r2, #3
	ands r2, r7
	cmp r2, #0
	bne .L_02009396
	add r3, sp, #16
	mov r8, r3
	mov r0, r8
	movs r3, #1
	str r3, [r0]
	ldr r3, .L_020093d4
	add r6, sp, #56
	str r3, [r0, #36]
	ldr r3, .L_020093d8
	str r2, [r6]
	str r2, [r6, #4]
	str r3, [r6, #8]
	bl Random16Far
	movs r1, #144
	bl Engine_MathModulo
	movs r1, #128
	adds r5, r0, #0
	lsls r1, r1, #17
	lsls r5, r5, #16
	adds r5, r5, r1
	bl Random16Far
	movs r1, #144
	bl Engine_MathModulo
	ldr r1, [r6, #4]
	ldr r3, [r6]
	str r1, [sp, #0]
	adds r2, r0, #0
	ldr r1, [r6, #8]
	mov r0, r8
	str r1, [sp, #4]
	movs r1, #129
	lsls r1, r1, #17
	adds r1, #1
	str r1, [sp, #8]
	str r0, [sp, #12]
	lsls r2, r2, #16
	adds r0, r5, #0
	movs r1, #0
	bl Func_0200015c
.L_02009396:
	ldr r6, .L_020093cc
	movs r1, #128
	ldr r3, .L_020093d0
	lsls r1, r1, #19
	lsrs r2, r7, #4
	ands r2, r6
	adds r1, #82
	subs r3, r3, r2
	mov r8, r1
	lsls r1, r2, #8
	orrs r1, r3
	mov r2, r8
	strh r1, [r2]
	movs r0, #8
	bl Object_GetById
	adds r5, r0, #0
	bl Random16Far
	movs r3, #7
	ands r0, r3
	adds r5, #98
	strb r0, [r5]
	movs r0, #1
	bl Battle_WaitMode0
	b .L_020093dc
.L_020093cc:
	.4byte 0x0000000f
.L_020093d0:
	.4byte 0x00000010
.L_020093d4:
	.4byte Func_02000f9c
.L_020093d8:
	.4byte 0xfffe0000
.L_020093dc:
	cmp r7, #255
	bne .L_02009334
	movs r3, #11
	movs r2, #10
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #68
	movs r1, #0
	movs r2, #79
	movs r3, #0
	bl Func_02003b68
	movs r3, #16
	str r3, [sp, #0]
	movs r5, #9
	movs r0, #38
	movs r1, #9
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl Func_02003b80
	movs r3, #24
	str r3, [sp, #0]
	movs r1, #9
	movs r2, #1
	movs r3, #1
	movs r0, #38
	str r5, [sp, #4]
	bl Func_02003b80
	bl Func_02003b50
	movs r0, #1
	bl Battle_WaitMode0
	ldr r3, .L_02009460
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #80
	strh r3, [r2]
	movs r2, #230
	mov r3, r8
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	strh r6, [r3]
	negs r1, r1
	adds r2, #102
	negs r0, r0
	bl Func_02003b98
	movs r0, #195
	lsls r0, r0, #1
	bl Func_02003cd8
	movs r0, #8
	bl Object_GetById
	movs r3, #6
	adds r0, #98
	strb r3, [r0]
	movs r7, #0
.L_0200945a:
	ldr r3, .L_02009464
	lsrs r1, r7, #2
	b .L_02009468
.L_02009460:
	.4byte 0x00003f41
.L_02009464:
	.4byte 0x00000010
.L_02009468:
	movs r0, #128
	lsls r2, r1, #8
	subs r3, r3, r1
	lsls r0, r0, #19
	adds r0, #82
	orrs r2, r3
	strh r2, [r0]
	adds r7, #1
	movs r0, #1
	bl Battle_WaitMode0
	cmp r7, #65
	bne .L_0200945a
	ldr r0, .L_02009534
	bl Scheduler_RemoveCallbackFar
	movs r0, #8
	bl Object_GetById
	movs r5, #0
	adds r0, #99
	strb r5, [r0]
	movs r0, #30
	bl WaitFrames
	movs r0, #8
	bl Object_GetById
	str r5, [r0, #108]
	movs r0, #8
	bl Object_GetById
	movs r1, #6
	bl Object_SetPartAttribute
	bl Func_02003ba0
	movs r0, #60
	bl Battle_WaitMode0
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #80
	strh r5, [r3]
	bl Func_02003b00
	bl Func_02003af8
	ldr r6, .L_02009538
	movs r0, #147
	lsls r0, r0, #1
	movs r1, #128
	adds r0, #255
	lsls r1, r1, #2
	adds r3, r6, r0
	adds r1, #38
	ldrb r0, [r3]
	adds r3, r6, r1
	ldrb r1, [r3]
	bl Func_02003bb8
	movs r1, #128
	lsls r1, r1, #19
	adds r1, #10
	ldrh r2, [r1]
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #252
	ands r3, r2
	strh r3, [r1]
	ldr r2, .L_02009530
	ldrh r3, [r1]
	movs r5, #1
	orrs r3, r2
	movs r2, #133
	lsls r2, r2, #2
	adds r6, r6, r2
	strh r3, [r1]
	ldr r0, [r6]
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #223
	ands r3, r2
	strb r3, [r0]
	ldr r0, [r6]
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
	movs r0, #8
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r1, #1
	b .L_0200953c
.L_02009530:
	.4byte 0x00000001
.L_02009534:
	.4byte Func_02000ef4
.L_02009538:
	.4byte gPartyState
.L_0200953c:
	orrs r5, r3
	strb r5, [r0]
	ldr r0, [r6]
	bl Object_AttachWorkTargetToObject
	bl Func_02003c30
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #88
	bl GameFlag_SetBit
	bl Func_02003bd0
	add sp, #72
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.section .text.x02009560,"ax",%progbits
	.global Func_02001560
	.thumb_func
Func_02001560:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, .L_02009898
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	sub sp, #4
	bl Object_GetById
	adds r7, r0, #0
	ldr r6, [r7, #104]
	bl Func_02003bc8
	movs r0, #0
	bl Func_02003c88
	movs r3, #85
	adds r3, r3, r7
	mov r11, r3
	mov r2, r11
	movs r3, #4
	strb r3, [r2]
	movs r1, #0
	adds r0, r7, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #148
	lsls r0, r0, #2
	bl GameFlag_SetBit
	movs r0, #137
	bl Func_02003cd8
	movs r3, #99
	adds r3, r3, r6
	mov r8, r3
	ldrb r3, [r3]
	movs r5, #0
	cmp r3, #0
	beq .L_02009602
.L_020095bc:
	ldr r3, [r6, #8]
	ldr r2, .L_0200989c
	str r3, [r7, #8]
	ldr r3, [r6, #12]
	adds r3, r3, r5
	str r3, [r7, #12]
	ldr r3, [r6, #16]
	str r3, [r7, #16]
	cmp r5, r2
	bgt .L_020095d8
	movs r3, #200
	lsls r3, r3, #5
	adds r3, #153
	adds r5, r5, r3
.L_020095d8:
	ldr r3, .L_020098a0
	adds r1, r7, #0
	ldr r2, [r3]
	ldrb r3, [r3]
	adds r1, #35
	lsls r3, r3, #12
	strh r3, [r7, #6]
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
	bne .L_020095bc
.L_02009602:
	movs r0, #144
	lsls r0, r0, #1
	bl Func_02003cd8
	movs r2, #35
	adds r2, r2, r7
	ldrh r3, [r6, #6]
	mov r9, r2
	movs r2, #192
	lsls r2, r2, #8
	cmp r3, r2
	beq .L_0200961c
	b .L_020097b4
.L_0200961c:
	ldr r3, [r6, #104]
	movs r2, #0
	str r3, [sp, #0]
	mov r10, r2
	movs r3, #4
	mov r8, r2
	mov r2, r9
	strb r3, [r2]
	ldr r3, [r7, #8]
	movs r2, #2
	negs r2, r2
	asrs r3, r3, #19
	ands r3, r2
	adds r3, #1
	lsls r3, r3, #19
	str r3, [r7, #8]
	movs r5, #0
	ldr r3, [r6, #12]
	asrs r3, r3, #19
	ands r3, r2
	adds r3, #3
	lsls r3, r3, #19
	str r3, [r7, #12]
	ldr r3, [r7, #16]
	asrs r3, r3, #19
	ands r3, r2
	adds r3, #1
	lsls r3, r3, #19
	str r3, [r7, #16]
.L_02009656:
	ldr r0, .L_020098a4
	movs r2, #64
	ldr r3, [r0]
	movs r1, #0
	ands r3, r2
	cmp r3, #0
	beq .L_02009672
	ldr r3, .L_020098a8
	mov r10, r1
	mov r8, r3
	movs r3, #192
	lsls r3, r3, #8
	strh r3, [r7, #6]
	movs r1, #1
.L_02009672:
	ldr r3, [r0]
	movs r2, #16
	ands r3, r2
	cmp r3, #0
	beq .L_0200968c
	movs r2, #128
	movs r3, #0
	lsls r2, r2, #13
	mov r8, r3
	mov r10, r2
	mov r2, r8
	strh r2, [r7, #6]
	movs r1, #1
.L_0200968c:
	ldr r3, [r0]
	movs r2, #32
	ands r3, r2
	cmp r3, #0
	beq .L_020096a6
	ldr r3, .L_020098a8
	movs r2, #0
	mov r10, r3
	movs r3, #128
	lsls r3, r3, #8
	strh r3, [r7, #6]
	mov r8, r2
	movs r1, #1
.L_020096a6:
	cmp r1, #0
	beq .L_020096c6
	ldr r2, [r6, #16]
	ldr r3, [r6, #12]
	ldr r1, [r6, #8]
	subs r2, r2, r3
	add r1, r10
	add r2, r8
	asrs r1, r1, #20
	asrs r2, r2, #20
	movs r0, #2
	bl Func_02000348
	asrs r0, r0, #8
	cmp r0, #255
	bne .L_02009730
.L_020096c6:
	movs r0, #1
	adds r5, #1
	bl WaitFrames
	cmp r5, #60
	bne .L_02009656
	movs r3, #0
	mov r10, r3
	movs r3, #192
	lsls r3, r3, #8
	strh r3, [r7, #6]
	ldr r2, .L_020098a8
	ldr r3, [r6, #12]
	mov r8, r2
	ldr r2, [r6, #16]
	ldr r1, [r6, #8]
	subs r2, r2, r3
	add r2, r8
	asrs r1, r1, #20
	asrs r2, r2, #20
	movs r0, #2
	bl Func_02000348
	lsls r0, r0, #8
	lsrs r0, r0, #16
	cmp r0, #255
	bne .L_02009730
	movs r2, #0
	movs r3, #128
	lsls r3, r3, #13
	mov r8, r2
	mov r10, r3
	mov r3, r8
	strh r3, [r7, #6]
	movs r0, #2
	ldr r1, [r6, #8]
	ldr r2, [r6, #16]
	ldr r3, [r6, #12]
	add r1, r10
	subs r2, r2, r3
	asrs r1, r1, #20
	asrs r2, r2, #20
	bl Func_02000348
	lsls r0, r0, #8
	lsrs r0, r0, #16
	cmp r0, #255
	bne .L_02009730
	movs r3, #128
	ldr r2, .L_020098a8
	lsls r3, r3, #8
	strh r3, [r7, #6]
	mov r10, r2
.L_02009730:
	ldr r5, [sp, #0]
	ldr r1, [r6, #8]
	ldr r2, [r6, #16]
	adds r5, #98
	add r2, r8
	add r1, r10
	ldrb r0, [r5]
	bl Map_GetTerrainHeight
	movs r1, #6
	str r0, [r7, #12]
	str r0, [r7, #20]
	adds r0, r7, #0
	bl Func_02003b28
	movs r0, #6
	bl WaitFrames
	movs r0, #152
	bl Func_02003cd8
	adds r0, r7, #0
	movs r1, #7
	bl Func_02003b28
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r7, #48]
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r7, #52]
	movs r3, #128
	lsls r3, r3, #11
	str r3, [r7, #40]
	mov r2, r11
	movs r3, #2
	strb r3, [r2]
	movs r3, #1
	mov r2, r9
	strb r3, [r2]
	adds r2, r7, #0
	ldrb r3, [r5]
	adds r2, #34
	strb r3, [r2]
	ldr r1, [r7, #8]
	ldr r3, [r7, #16]
	ldr r2, [r7, #12]
	add r3, r8
	add r1, r10
	adds r0, r7, #0
	bl Func_02003b58
	adds r0, r7, #0
	bl Func_020003cc
	adds r0, r7, #0
	bl Func_02003b60
	adds r0, r7, #0
	movs r1, #1
	bl ObjectDispatch_SetSingleChildField26
	movs r3, #3
	mov r2, r11
	strb r3, [r2]
	b .L_02009844
.L_020097b4:
	ldr r3, [r6, #8]
	ldrh r1, [r6, #6]
	movs r2, #2
	negs r2, r2
	asrs r3, r3, #19
	ands r3, r2
	asrs r1, r1, #13
	adds r3, r3, r1
	subs r3, #1
	lsls r3, r3, #19
	str r3, [r7, #8]
	adds r0, r7, #0
	ldr r3, [r6, #16]
	movs r6, #0
	asrs r3, r3, #19
	ands r3, r2
	movs r2, #2
	ands r1, r2
	subs r3, r3, r1
	adds r3, #1
	lsls r3, r3, #19
	str r3, [r7, #16]
	movs r1, #1
	bl ObjectDispatch_SetSingleChildField26
	movs r3, #192
	lsls r3, r3, #11
	str r3, [r7, #40]
	mov r2, r9
	movs r3, #1
	strb r3, [r2]
	movs r3, #3
	mov r2, r11
	strb r3, [r2]
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r1, r1
	negs r2, r2
	movs r3, #0
	negs r0, r0
	bl Motion_CamBounds
	bl Func_02003c40
	movs r3, #128
	adds r5, r0, #0
	lsls r3, r3, #12
	str r3, [r5, #48]
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r5, #52]
	adds r3, r5, #0
	adds r3, #85
	strb r6, [r3]
	ldr r1, [r7, #8]
	ldr r2, [r7, #16]
	movs r0, #0
	bl Map_GetTerrainHeight
	ldr r1, [r7, #8]
	adds r2, r0, #0
	ldr r3, [r7, #16]
	adds r0, r5, #0
	bl Func_02003b58
	adds r0, r7, #0
	bl Func_020003cc
	adds r0, r5, #0
	bl Func_02003b60
.L_02009844:
	ldr r5, .L_02009898
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
	ldr r0, [r5]
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #32
	orrs r3, r2
	strb r3, [r0]
	movs r0, #148
	lsls r0, r0, #2
	bl GameFlag_ClearBit
	movs r1, #1
	ldr r0, [r5]
	bl Object_AttachWorkTargetToObject
	bl Func_02003c30
	movs r0, #10
	bl WaitFrames
	bl Func_02003bd0
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_02009898:
	.4byte gPartyState
.L_0200989c:
	.4byte 0x000bffff
.L_020098a0:
	.4byte Data_0300122c
.L_020098a4:
	.4byte gInput
.L_020098a8:
	.4byte 0xfff00000
	.section .text.x020098ac,"ax",%progbits
	.global Func_020018ac
	.thumb_func
Func_020018ac:
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
	ldr r3, .L_020098d4
	mov lr, r3
	.2byte 0xf800
	pop {pc}
	.2byte 0x0000
.L_020098d4:
	.4byte IwramFillWords + 0x74
	.section .text.x020098d8,"ax",%progbits
	.global Func_020018d8
	.thumb_func
Func_020018d8:
	push {lr}
	ldmia r1!, {r3}
	ldmia r0!, {r4}
	ldr r2, [r1]
	subs r4, r4, r3
	ldr r3, [r0]
	asrs r4, r4, #16
	subs r3, r3, r2
	movs r2, #192
	lsls r2, r2, #11
	adds r3, r3, r2
	asrs r3, r3, #16
	adds r2, r3, #0
	muls r2, r3
	adds r0, r4, #0
	muls r0, r4
	adds r3, r2, #0
	adds r0, r0, r3
	ldr r3, .L_02009904
	mov lr, r3
	.2byte 0xf800
	pop {pc}
.L_02009904:
	.4byte IwramFillWords + 0x74
	.section .text.x02009908,"ax",%progbits
	.global Func_02001908
	.thumb_func
Func_02001908:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	ldr r7, .L_020099fc
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r7, r2
	adds r6, r0, #0
	ldr r0, [r3]
	sub sp, #4
	bl Object_GetById
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	adds r5, r0, #0
	mov r8, r3
	movs r3, #179
	lsls r3, r3, #1
	add r3, r8
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	bne .L_02009976
	movs r3, #173
	lsls r3, r3, #1
	add r3, r8
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	bne .L_02009976
	movs r3, #175
	lsls r3, r3, #1
	add r3, r8
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	bne .L_02009976
	movs r3, #180
	lsls r3, r3, #1
	add r3, r8
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	bne .L_02009976
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #18
	adds r3, r3, r7
	mov r10, r3
	ldrb r3, [r3]
	cmp r3, #5
	bne .L_02009980
.L_02009976:
	adds r0, r6, #0
	movs r1, #0
	bl ObjectDispatch_ApplyValueToChildren
	b .L_02009b42
.L_02009980:
	adds r0, r6, #0
	movs r1, #16
	bl ObjectDispatch_ApplyValueToChildren
	ldr r2, [r6, #16]
	ldr r3, [r6, #12]
	ldr r1, [r6, #8]
	subs r2, r2, r3
	asrs r1, r1, #20
	asrs r2, r2, #20
	movs r0, #2
	bl Func_02000348
	asrs r0, r0, #8
	cmp r0, #212
	bne .L_020099ce
.L_020099a0:
	movs r1, #142
	lsls r1, r1, #1
	adds r0, r6, #0
	adds r1, #255
	bl Func_02003cd0
	adds r2, r6, #0
	movs r3, #0
	adds r2, #99
	strb r3, [r2]
	str r3, [r6, #108]
	ldrh r3, [r6, #6]
	movs r2, #192
	lsls r2, r2, #8
	cmp r3, r2
	bne .L_020099c4
	ldr r1, .L_02009a00
	b .L_020099c6
.L_020099c4:
	ldr r1, .L_02009a04
.L_020099c6:
	adds r0, r6, #0
	bl Func_02003b38
	b .L_02009b42
.L_020099ce:
	movs r3, #98
	adds r3, r3, r6
	mov r9, r3
	ldrb r3, [r3]
	cmp r3, #0
	bne .L_02009a7a
	mov r2, r10
	ldrb r3, [r2]
	adds r0, r5, #0
	adds r1, r6, #0
	movs r7, #0
	adds r0, #8
	adds r1, #8
	cmp r3, #2
	bne .L_02009a08
	bl Func_020018d8
	cmp r0, #16
	bgt .L_02009a1e
	ldr r2, [r5, #16]
	ldr r3, [r6, #16]
	b .L_02009a14
	.2byte 0x0000
.L_020099fc:
	.4byte gPartyState
.L_02009a00:
	.4byte Data_02003de0
.L_02009a04:
	.4byte Data_02003dcc
.L_02009a08:
	bl Func_020018ac
	cmp r0, #8
	bgt .L_02009a1e
	ldr r2, [r5, #12]
	ldr r3, [r6, #12]
.L_02009a14:
	asrs r2, r2, #20
	asrs r3, r3, #20
	cmp r2, r3
	bne .L_02009a1e
	movs r7, #1
.L_02009a1e:
	cmp r7, #0
	beq .L_02009a7a
	movs r0, #130
	lsls r0, r0, #1
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02009a7a
	ldrh r3, [r6, #6]
	str r6, [r5, #104]
	strh r3, [r5, #6]
	adds r1, r5, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #254
	ands r3, r2
	strb r3, [r1]
	movs r2, #12
	ldr r3, [r6, #80]
	ldr r0, [r5, #80]
	ldrb r3, [r3, #9]
	ldrb r1, [r0, #9]
	ands r2, r3
	movs r3, #13
	negs r3, r3
	ands r3, r1
	orrs r3, r2
	movs r2, #181
	lsls r2, r2, #1
	add r2, r8
	strb r3, [r0, #9]
	movs r3, #200
	strh r3, [r2]
	ldr r3, .L_02009a88
	movs r2, #128
	ldr r4, .L_02009a84
	lsls r2, r2, #2
	adds r2, #18
	adds r3, r3, r2
	strb r4, [r3]
	mov r2, r9
	movs r3, #1
	strb r3, [r2]
	adds r2, r6, #0
	adds r2, #99
	strb r3, [r2]
.L_02009a7a:
	mov r3, r8
	adds r3, #52
	str r3, [sp, #0]
	movs r7, #8
	b .L_02009a8c
.L_02009a84:
	.4byte 0x00000000
.L_02009a88:
	.4byte gPartyState
.L_02009a8c:
	ldr r3, [sp, #0]
	ldmia r3!, {r5}
	adds r2, r3, #0
	str r2, [sp, #0]
	ldr r3, [r5]
	cmp r3, #0
	beq .L_02009ade
	ldr r1, [r6, #104]
	ldr r3, [r5, #8]
	ldr r2, [r1, #8]
	asrs r3, r3, #20
	asrs r2, r2, #20
	cmp r2, r3
	bne .L_02009ac0
	ldr r2, [r1, #12]
	ldr r3, [r5, #12]
	asrs r2, r2, #20
	asrs r3, r3, #20
	cmp r2, r3
	bne .L_02009ac0
	ldr r2, [r1, #16]
	ldr r3, [r5, #16]
	asrs r2, r2, #20
	asrs r3, r3, #20
	cmp r2, r3
	beq .L_02009ade
.L_02009ac0:
	adds r0, r5, #0
	adds r1, r6, #0
	adds r0, #8
	adds r1, #8
	bl Func_020018ac
	cmp r0, #8
	bgt .L_02009ade
	ldr r2, [r5, #12]
	ldr r3, [r6, #12]
	asrs r2, r2, #20
	asrs r3, r3, #20
	cmp r2, r3
	bne .L_02009ade
	b .L_020099a0
.L_02009ade:
	adds r7, #1
	cmp r7, #64
	bne .L_02009a8c
	ldrh r3, [r6, #6]
	movs r2, #192
	lsls r2, r2, #8
	adds r0, r3, #0
	cmp r3, r2
	bne .L_02009b1a
	bl Math_Cosine
	movs r1, #192
	lsls r1, r1, #9
	ldr r5, .L_02009b50
	mov lr, r5
	.2byte 0xf800
	ldr r3, [r6, #8]
	adds r3, r3, r0
	ldrh r0, [r6, #6]
	str r3, [r6, #8]
	bl Math_Sine
	movs r1, #192
	lsls r1, r1, #9
	mov lr, r5
	.2byte 0xf800
	ldr r3, [r6, #12]
	subs r3, r3, r0
	str r3, [r6, #12]
	b .L_02009b42
.L_02009b1a:
	bl Math_Cosine
	movs r1, #192
	lsls r1, r1, #9
	ldr r5, .L_02009b50
	mov lr, r5
	.2byte 0xf800
	ldr r3, [r6, #8]
	adds r3, r3, r0
	ldrh r0, [r6, #6]
	str r3, [r6, #8]
	bl Math_Sine
	movs r1, #192
	lsls r1, r1, #9
	mov lr, r5
	.2byte 0xf800
	ldr r3, [r6, #16]
	adds r3, r3, r0
	str r3, [r6, #16]
.L_02009b42:
	add sp, #4
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009b50:
	.4byte IwramMulQ16
	.section .text.x02009b54,"ax",%progbits
	.global Func_02001b54
	.thumb_func
Func_02001b54:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r1, #179
	lsls r1, r1, #1
	adds r3, r2, r1
	movs r1, #0
	ldrsh r3, [r3, r1]
	sub sp, #68
	adds r7, r0, #0
	cmp r3, #0
	beq .L_02009b76
	b .L_02009c8c
.L_02009b76:
	movs r1, #173
	lsls r1, r1, #1
	adds r3, r2, r1
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	beq .L_02009b86
	b .L_02009c8c
.L_02009b86:
	movs r1, #175
	lsls r1, r1, #1
	adds r3, r2, r1
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	beq .L_02009b96
	b .L_02009c8c
.L_02009b96:
	movs r1, #180
	lsls r1, r1, #1
	adds r3, r2, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	bne .L_02009c8c
	movs r3, #100
	adds r3, r3, r7
	mov r10, r3
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #240
	bne .L_02009c82
	movs r0, #148
	lsls r0, r0, #2
	bl GameFlag_Test
	add r2, sp, #16
	add r6, sp, #56
	mov r8, r2
	cmp r0, #0
	bne .L_02009bcc
	adds r0, r7, #0
	movs r1, #202
	bl Func_02003cd0
.L_02009bcc:
	ldrh r3, [r7, #6]
	movs r1, #192
	lsls r1, r1, #8
	cmp r3, r1
	bne .L_02009bde
	ldr r3, [r7, #8]
	str r3, [r6]
	ldr r3, [r7, #16]
	b .L_02009c0a
.L_02009bde:
	ldrh r0, [r7, #6]
	bl Math_Cosine
	adds r1, r0, #0
	movs r0, #128
	ldr r5, .L_02009c98
	lsls r0, r0, #12
	mov lr, r5
	.2byte 0xf800
	ldr r3, [r7, #8]
	adds r3, r3, r0
	str r3, [r6]
	ldrh r0, [r7, #6]
	bl Math_Sine
	adds r1, r0, #0
	movs r0, #128
	lsls r0, r0, #12
	mov lr, r5
	.2byte 0xf800
	ldr r3, [r7, #16]
	adds r3, r3, r0
.L_02009c0a:
	str r3, [r6, #8]
	add r6, sp, #56
	movs r0, #140
	ldr r2, [r7, #12]
	ldr r3, [r6, #8]
	ldr r1, [r6]
	lsls r0, r0, #1
	bl Func_02003b40
	movs r1, #2
	adds r5, r0, #0
	bl Func_02003b28
	adds r0, r5, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	ldr r1, [r5, #80]
	movs r3, #13
	ldrb r2, [r1, #9]
	negs r3, r3
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	strb r3, [r1, #9]
	adds r3, r5, #0
	movs r4, #0
	adds r3, #85
	strb r4, [r3]
	adds r3, #13
	strb r4, [r3]
	adds r3, #1
	strb r4, [r3]
	ldrh r3, [r7, #6]
	mov r1, r8
	strh r3, [r5, #6]
	ldr r3, .L_02009c9c
	mov r2, r10
	str r3, [r5, #108]
	movs r3, #1
	str r7, [r5, #104]
	strh r4, [r2]
	str r3, [r1]
	movs r3, #7
	str r3, [r1, #4]
	ldr r3, .L_02009ca0
	ldr r2, [r6, #8]
	ldr r0, [r6]
	adds r2, r2, r3
	movs r3, #192
	lsls r3, r3, #10
	ldr r1, [r7, #12]
	str r3, [sp, #8]
	mov r3, r8
	str r3, [sp, #12]
	movs r3, #0
	str r4, [sp, #0]
	str r4, [sp, #4]
	bl Func_0200015c
.L_02009c82:
	adds r2, r7, #0
	adds r2, #100
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
.L_02009c8c:
	add sp, #68
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009c98:
	.4byte IwramMulQ16
.L_02009c9c:
	.4byte Func_02001908
.L_02009ca0:
	.4byte 0xfffa0000
	.section .text.x02009ca4,"ax",%progbits
	.global Func_02001ca4
	.thumb_func
Func_02001ca4:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r0, .L_02009cc4
	ldr r5, [r3, #108]
	bl Func_02003cb8
	movs r3, #192
	lsls r3, r3, #4
	adds r3, #188
	adds r5, r5, r3
	ldr r0, [r5]
	bl Func_02000468
	pop {r5, pc}
	.2byte 0x0000
.L_02009cc4:
	.4byte Data_02003d94
	.section .text.x02009cc8,"ax",%progbits
	.global Func_02001cc8
	.thumb_func
Func_02001cc8:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #192
	lsls r2, r2, #4
	adds r2, #188
	adds r3, r3, r2
	ldr r6, [r3]
	bl Func_02003bc8
	movs r0, #0
	bl Func_02003c88
	bl Func_02003cc0
	ldr r0, .L_02009da4
	bl Func_020003e8
	ldr r1, [r6, #8]
	ldr r2, [r6, #16]
	movs r0, #2
	bl Map_GetTerrainHeight
	ldr r3, [r6, #12]
	cmp r0, r3
	beq .L_02009d30
	movs r3, #34
	adds r3, r3, r6
	mov r8, r3
	mov r2, r8
	movs r3, #2
	adds r7, r6, #0
	strb r3, [r2]
	adds r7, #85
	movs r3, #3
	strb r3, [r7]
	adds r0, r6, #0
	bl Func_020003cc
	adds r0, r6, #0
	bl Func_020003cc
	movs r0, #188
	bl Func_02003cd8
	movs r5, #0
	mov r3, r8
	strb r5, [r7]
	strb r5, [r3]
.L_02009d30:
	ldr r3, [r6, #8]
	asrs r2, r3, #20
	ldr r3, [r6, #16]
	asrs r0, r3, #20
	cmp r2, #16
	bne .L_02009d4c
	cmp r0, #43
	bne .L_02009d4c
	movs r0, #236
	lsls r0, r0, #3
	adds r0, #255
	bl GameFlag_SetBit
	b .L_02009d98
.L_02009d4c:
	cmp r2, #45
	bne .L_02009d5e
	cmp r0, #45
	bne .L_02009d5e
	movs r0, #134
	lsls r0, r0, #4
	bl GameFlag_SetBit
	b .L_02009d98
.L_02009d5e:
	cmp r2, #21
	bne .L_02009d72
	cmp r0, #48
	bne .L_02009d72
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #97
	bl GameFlag_SetBit
	b .L_02009d98
.L_02009d72:
	cmp r2, #39
	bne .L_02009d86
	cmp r0, #49
	bne .L_02009d86
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #98
	bl GameFlag_SetBit
	b .L_02009d98
.L_02009d86:
	cmp r2, #49
	bne .L_02009d98
	cmp r0, #18
	bne .L_02009d98
	movs r0, #241
	lsls r0, r0, #3
	adds r0, #255
	bl GameFlag_SetBit
.L_02009d98:
	bl Func_02003bd0
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009da4:
	.4byte Data_02003d94
	.section .text.x02009da8,"ax",%progbits
	.global Func_02001da8
	.thumb_func
Func_02001da8:
	push {lr}
	bl Func_02003bc8
	movs r0, #0
	bl Func_02003c88
	ldr r0, .L_02009dc0
	bl Func_02003ca8
	bl Func_02003bd0
	pop {pc}
.L_02009dc0:
	.4byte Data_02003e4c
	.section .text.x02009dc4,"ax",%progbits
	.global Func_02001dc4
	.thumb_func
Func_02001dc4:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	mov r9, r0
	mov r10, r1
	movs r2, #0
	mov r3, r10
	movs r0, #255
	mov r1, r9
	sub sp, #68
	bl Func_02003b40
	movs r2, #1
	add r3, sp, #28
	str r2, [r3]
	movs r2, #128
	lsls r2, r2, #10
	str r2, [r3, #8]
	str r2, [r3, #12]
	mov r11, r0
	movs r0, #0
	mov r8, r0
.L_02009df8:
	bl Random16Far
	movs r5, #63
	ldr r2, .L_02009f6c
	ands r0, r5
	lsls r0, r0, #16
	add r0, r9
	add r7, sp, #16
	adds r0, r0, r2
	str r0, [r7]
	bl Random16Far
	movs r3, #15
	ands r3, r0
	lsls r3, r3, #12
	str r3, [r7, #4]
	bl Random16Far
	ldr r3, .L_02009f70
	ands r0, r5
	lsls r0, r0, #16
	add r0, r10
	adds r2, r0, r3
	movs r1, #3
	mov r0, r8
	ands r1, r0
	mov r5, r8
	str r2, [r7, #8]
	add r4, sp, #28
	ldr r0, [r7]
	ldr r3, [r7, #4]
	adds r5, #1
	cmp r1, #0
	beq .L_02009e56
	str r3, [sp, #0]
	movs r3, #0
	str r3, [sp, #4]
	movs r3, #160
	lsls r3, r3, #12
	adds r3, #1
	str r3, [sp, #8]
	movs r1, #0
	movs r3, #0
	str r4, [sp, #12]
	bl Func_0200015c
	b .L_02009e6c
.L_02009e56:
	str r3, [sp, #0]
	movs r3, #128
	lsls r3, r3, #10
	adds r3, #1
	str r1, [sp, #4]
	str r3, [sp, #8]
	movs r1, #0
	movs r3, #0
	str r4, [sp, #12]
	bl Func_0200015c
.L_02009e6c:
	mov r8, r5
	cmp r5, #12
	bne .L_02009df8
	movs r2, #0
	mov r8, r2
.L_02009e76:
	mov r3, r8
	mov r6, r11
	cmp r3, #0
	beq .L_02009e96
	mov r1, r9
	movs r2, #0
	movs r0, #255
	mov r3, r10
	bl Func_02003b40
	mov r2, r11
	adds r6, r0, #0
	ldr r0, [r6, #80]
	ldr r1, [r2, #80]
	bl Func_02003c90
.L_02009e96:
	adds r0, r6, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	adds r0, r6, #0
	ldr r1, .L_02009f74
	bl Func_02003b38
	movs r1, #2
	adds r0, r6, #0
	bl Func_02003b28
	bl Random16Far
	movs r3, #3
	ands r3, r0
	movs r0, #128
	lsls r0, r0, #11
	lsls r3, r3, #16
	adds r3, r3, r0
	str r3, [r6, #40]
	movs r3, #204
	lsls r3, r3, #6
	adds r3, #51
	str r3, [r6, #72]
	movs r3, #204
	lsls r3, r3, #7
	adds r3, #102
	str r3, [r6, #68]
	movs r3, #204
	lsls r3, r3, #8
	adds r3, #204
	str r3, [r6, #24]
	str r3, [r6, #28]
	str r3, [r6, #48]
	bl Random16Far
	movs r3, #7
	ands r0, r3
	str r0, [r7, #4]
	lsls r0, r0, #12
	bl Math_Cosine
	movs r1, #128
	lsls r1, r1, #15
	ldr r5, .L_02009f78
	mov lr, r5
	.2byte 0xf800
	str r0, [r7]
	ldr r0, [r7, #4]
	lsls r0, r0, #12
	bl Math_Sine
	movs r1, #128
	lsls r1, r1, #15
	mov lr, r5
	.2byte 0xf800
	ldr r1, [r6, #80]
	str r0, [r7, #8]
	movs r0, #13
	ldrb r3, [r1, #9]
	negs r0, r0
	adds r2, r0, #0
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	strb r3, [r1, #9]
	ldr r3, [r7]
	ldr r1, [r6, #8]
	ldr r0, [r7, #8]
	adds r1, r1, r3
	ldr r3, [r6, #16]
	ldr r2, [r6, #12]
	adds r3, r3, r0
	adds r0, r6, #0
	bl Func_02003b58
	movs r2, #1
	add r8, r2
	mov r3, r8
	cmp r3, #6
	bne .L_02009e76
	movs r0, #160
	movs r1, #224
	movs r2, #128
	lsls r0, r0, #11
	lsls r1, r1, #11
	lsls r2, r2, #9
	bl Func_02003b98
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	negs r0, r0
	negs r1, r1
	adds r2, #102
	bl Func_02003b98
	add sp, #68
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009f6c:
	.4byte 0xffe00000
.L_02009f70:
	.4byte 0xffd00000
.L_02009f74:
	.4byte Data_02003dfc
.L_02009f78:
	.4byte IwramMulQ16
	.section .text.x02009f7c,"ax",%progbits
	.global Func_02001f7c
	.thumb_func
Func_02001f7c:
	push {r5, r6, r7, lr}
	movs r0, #128
	lsls r0, r0, #4
	sub sp, #16
	bl Runtime_BumpAllocate
	adds r5, r0, #0
	bl Func_02003bc8
	movs r0, #0
	bl Func_02003c88
	movs r1, #128
	lsls r1, r1, #19
	ldrh r2, [r1]
	movs r3, #254
	lsls r3, r3, #8
	adds r3, #255
	ands r3, r2
	strh r3, [r1]
	movs r2, #128
	movs r3, #128
	lsls r3, r3, #19
	lsls r2, r2, #3
	adds r3, #8
	strh r2, [r3]
	ldr r0, .L_0200a008
	bl Resource_GetTableEntry
	adds r1, r5, #0
	bl Func_02003af0
	movs r3, #128
	lsls r3, r3, #19
	movs r1, #192
	adds r3, #212
	adds r0, r5, #0
	lsls r1, r1, #19
	ldr r2, .L_0200a00c
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	adds r0, r5, #0
	bl Sys_Free
	ldr r3, .L_0200a004
	mov r0, sp
	adds r0, #14
	strh r3, [r0]
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #212
	ldr r1, .L_0200a010
	ldr r2, .L_0200a014
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r0, #128
	bl Runtime_BumpAllocate
	adds r5, r0, #0
	ldr r0, .L_0200a018
	bl Resource_GetTableEntry
	adds r1, r5, #0
	bl Func_02003af0
	movs r7, #0
	adds r4, r5, #0
	b .L_0200a01c
.L_0200a004:
	.4byte 0x00000000
.L_0200a008:
	.4byte 0x000001c0
.L_0200a00c:
	.4byte 0x84000200
.L_0200a010:
	.4byte 0x06002000
.L_0200a014:
	.4byte 0x81000400
.L_0200a018:
	.4byte 0x000001c1
.L_0200a01c:
	ldrh r2, [r4]
	movs r3, #252
	lsls r3, r3, #8
	adds r3, #255
	ands r3, r2
	adds r7, #1
	strh r3, [r4]
	adds r4, #2
	cmp r7, #64
	bne .L_0200a01c
	adds r4, r5, #0
	movs r7, #0
.L_0200a034:
	ldr r2, .L_0200a1e0
	lsls r1, r7, #6
	adds r1, r1, r2
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	adds r0, r4, #0
	adds r2, #2
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	adds r7, #1
	adds r4, #8
	cmp r7, #16
	bne .L_0200a034
	adds r0, r5, #0
	bl Sys_Free
	movs r0, #246
	bl Func_02003cd8
	movs r0, #14
	bl Object_GetById
	movs r3, #9
	adds r0, #98
	strb r3, [r0]
	movs r0, #14
	bl Object_GetById
	movs r3, #1
	adds r0, #99
	strb r3, [r0]
	movs r0, #14
	bl Object_GetById
	ldr r3, .L_0200a1e4
	movs r7, #0
	str r3, [r0, #108]
.L_0200a084:
	ldr r3, .L_0200a1e8
	ldr r6, [r3]
	movs r3, #3
	ands r6, r3
	cmp r6, #0
	bne .L_0200a128
	movs r0, #14
	bl Object_GetById
	str r0, [sp, #8]
	movs r0, #14
	bl Object_GetById
	adds r5, r0, #0
	movs r0, #14
	bl Object_GetById
	ldr r3, [sp, #8]
	ldr r2, [r5, #12]
	ldr r1, [r3, #8]
	ldr r3, [r0, #16]
	movs r0, #168
	lsls r0, r0, #2
	bl Func_02003b40
	adds r5, r0, #0
	movs r0, #14
	bl Object_GetById
	ldr r3, [r0, #80]
	ldr r4, [r5, #80]
	ldrb r3, [r3, #9]
	movs r1, #12
	ands r1, r3
	movs r0, #13
	ldrb r3, [r4, #9]
	negs r0, r0
	adds r2, r0, #0
	ands r3, r2
	orrs r3, r1
	adds r1, r5, #0
	adds r1, #35
	ldrb r2, [r1]
	strb r3, [r4, #9]
	movs r3, #1
	orrs r3, r2
	strb r3, [r1]
	adds r3, r5, #0
	adds r3, #85
	strb r6, [r3]
	ldr r3, [r5, #8]
	str r3, [r5, #68]
	movs r3, #192
	lsls r3, r3, #9
	str r3, [r5, #72]
	ldr r3, [r5, #16]
	str r3, [r5, #76]
	bl Random16Far
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	ands r3, r0
	str r3, [r5, #48]
	adds r0, r5, #0
	movs r1, #7
	bl Func_02003b28
	adds r0, r5, #0
	ldr r1, .L_0200a1ec
	bl Func_02003b38
	adds r0, r5, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	adds r0, r5, #0
	movs r1, #2
	bl Object_SetPartAttribute
	ldr r3, .L_0200a1f0
	str r3, [r5, #108]
.L_0200a128:
	movs r0, #1
	adds r7, #1
	bl Battle_WaitMode0
	cmp r7, #45
	bne .L_0200a084
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #0
	bl Func_02003c58
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #0
	adds r0, #3
	bl Func_02003c50
	movs r0, #60
	bl Func_02003c60
	ldr r5, .L_0200a1f4
	movs r1, #133
	lsls r1, r1, #2
	adds r5, r5, r1
	ldr r0, [r5]
	movs r1, #1
	bl Object_AttachWorkTargetToObject
	bl Func_02003c30
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #9
	lsls r1, r1, #6
	bl Func_02003c20
	movs r0, #134
	movs r1, #1
	movs r2, #216
	lsls r2, r2, #16
	negs r1, r1
	movs r3, #1
	lsls r0, r0, #18
	bl Motion_CamBounds
	bl Func_02003c30
	movs r0, #14
	bl Object_GetById
	movs r3, #0
	str r3, [r0, #108]
	movs r0, #14
	bl Object_GetById
	movs r1, #6
	bl Object_SetPartAttribute
	ldr r3, .L_0200a1d8
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	ldr r3, .L_0200a1dc
	subs r2, #2
	strh r3, [r2]
	ldr r2, .L_0200a1f8
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #136
	strh r3, [r2]
	movs r3, #16
	strh r3, [r2, #2]
	movs r0, #30
	bl WaitFrames
	movs r0, #138
	bl Func_02003cd8
	movs r0, #128
	lsls r0, r0, #9
	adds r0, #3
	movs r1, #0
	bl Func_02003c58
	movs r0, #254
	lsls r0, r0, #7
	b .L_0200a1fc
.L_0200a1d8:
	.4byte 0x00001010
.L_0200a1dc:
	.4byte 0x00003f41
.L_0200a1e0:
	.4byte 0x06002000
.L_0200a1e4:
	.4byte Func_0200092c
.L_0200a1e8:
	.4byte Data_0300122c
.L_0200a1ec:
	.4byte Data_02003d1c
.L_0200a1f0:
	.4byte Func_020008ac
.L_0200a1f4:
	.4byte gPartyState
.L_0200a1f8:
	.4byte Data_03001120
.L_0200a1fc:
	movs r1, #0
	adds r0, #255
	bl Func_02003c50
	movs r0, #1
	bl Func_02003c60
	ldr r0, [r5]
	bl Object_GetById
	movs r3, #128
	lsls r3, r3, #7
	strh r3, [r0, #6]
	movs r0, #1
	bl WaitFrames
	movs r0, #254
	lsls r0, r0, #7
	adds r0, #255
	movs r1, #0
	bl Func_02003c58
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #0
	adds r0, #3
	bl Func_02003c50
	movs r0, #8
	bl Func_02003c60
	movs r1, #128
	lsls r1, r1, #19
	ldrh r3, [r1]
	ldr r2, .L_0200a27c
	movs r5, #9
	orrs r3, r2
	strh r3, [r1]
	movs r0, #52
	movs r1, #12
	movs r2, #29
	movs r3, #9
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl Func_02003b68
	movs r0, #52
	movs r1, #76
	movs r2, #29
	movs r3, #73
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl Func_02003b68
	movs r0, #138
	movs r1, #228
	lsls r0, r0, #18
	lsls r1, r1, #16
	bl Func_02001dc4
	movs r7, #0
.L_0200a276:
	ldr r3, .L_0200a280
	lsrs r2, r7, #1
	b .L_0200a284
.L_0200a27c:
	.4byte 0x00000100
.L_0200a280:
	.4byte 0x00000010
.L_0200a284:
	subs r3, r3, r2
	ldr r2, .L_0200a2c0
	movs r1, #128
	lsls r1, r1, #19
	adds r1, #82
	orrs r3, r2
	strh r3, [r1]
	movs r0, #1
	adds r7, #1
	bl WaitFrames
	cmp r7, #32
	bne .L_0200a276
	ldr r2, .L_0200a2c4
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #172
	movs r1, #0
	strh r3, [r2]
	strh r1, [r2, #2]
	movs r0, #1
	bl WaitFrames
	movs r0, #138
	bl Func_02003cd8
	movs r0, #128
	lsls r0, r0, #9
	adds r0, #3
	b .L_0200a2c8
.L_0200a2c0:
	.4byte 0x00001000
.L_0200a2c4:
	.4byte Data_03001120
.L_0200a2c8:
	movs r1, #0
	bl Func_02003c58
	movs r0, #254
	lsls r0, r0, #7
	movs r1, #0
	adds r0, #255
	bl Func_02003c50
	movs r0, #1
	bl Func_02003c60
	movs r0, #1
	bl WaitFrames
	movs r0, #254
	lsls r0, r0, #7
	adds r0, #255
	movs r1, #0
	bl Func_02003c58
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #0
	adds r0, #3
	bl Func_02003c50
	movs r0, #8
	bl Func_02003c60
	movs r1, #128
	lsls r1, r1, #19
	ldrh r3, [r1]
	ldr r2, .L_0200a348
	movs r5, #9
	orrs r3, r2
	strh r3, [r1]
	movs r0, #52
	movs r1, #22
	movs r2, #29
	movs r3, #9
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl Func_02003b68
	movs r0, #52
	movs r1, #86
	movs r2, #29
	movs r3, #73
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl Func_02003b68
	movs r0, #130
	movs r1, #242
	lsls r0, r0, #18
	lsls r1, r1, #16
	bl Func_02001dc4
	movs r7, #0
.L_0200a340:
	ldr r3, .L_0200a34c
	lsrs r2, r7, #1
	b .L_0200a350
	.2byte 0x0000
.L_0200a348:
	.4byte 0x00000100
.L_0200a34c:
	.4byte 0x00000010
.L_0200a350:
	subs r3, r3, r2
	ldr r2, .L_0200a36c
	movs r1, #128
	lsls r1, r1, #19
	adds r1, #82
	orrs r3, r2
	strh r3, [r1]
	movs r0, #1
	adds r7, #1
	bl WaitFrames
	cmp r7, #32
	bne .L_0200a340
	b .L_0200a370
.L_0200a36c:
	.4byte 0x00001000
.L_0200a370:
	movs r0, #145
	bl Func_02003cd8
	movs r0, #128
	lsls r0, r0, #9
	adds r0, #3
	movs r1, #0
	bl Func_02003c58
	movs r0, #254
	lsls r0, r0, #7
	movs r1, #0
	adds r0, #255
	movs r7, #128
	bl Func_02003c50
	lsls r7, r7, #19
	movs r0, #1
	bl Func_02003c60
	ldrh r2, [r7]
	movs r3, #254
	lsls r3, r3, #8
	adds r3, #255
	ands r3, r2
	strh r3, [r7]
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #128
	movs r1, #198
	lsls r0, r0, #18
	lsls r1, r1, #16
	bl Func_02001dc4
	movs r1, #242
	ldr r0, .L_0200a49c
	lsls r1, r1, #16
	bl Func_02001dc4
	movs r1, #223
	ldr r0, .L_0200a4a0
	lsls r1, r1, #16
	bl Func_02001dc4
	movs r5, #9
	movs r0, #62
	movs r1, #2
	movs r2, #29
	movs r3, #9
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl Func_02003b68
	movs r3, #29
	str r3, [sp, #0]
	movs r0, #62
	movs r1, #2
	movs r2, #9
	movs r3, #9
	str r5, [sp, #4]
	bl Func_02003b80
	movs r2, #29
	movs r3, #73
	movs r0, #62
	movs r1, #66
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl Func_02003b68
	movs r0, #254
	lsls r0, r0, #7
	adds r0, #255
	movs r1, #0
	bl Func_02003c58
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #0
	adds r0, #3
	bl Func_02003c50
	movs r0, #60
	bl Func_02003c60
	movs r0, #60
	bl WaitFrames
	movs r0, #128
	lsls r0, r0, #9
	adds r0, #3
	movs r1, #0
	bl Func_02003c58
	movs r0, #128
	movs r1, #0
	lsls r0, r0, #9
	bl Func_02003c50
	movs r0, #60
	bl Func_02003c60
	movs r0, #70
	bl WaitFrames
	bl Func_02003b00
	bl Func_02003af8
	ldr r3, .L_0200a4a4
	movs r0, #147
	movs r1, #128
	lsls r0, r0, #1
	lsls r1, r1, #2
	adds r0, #255
	adds r1, #38
	adds r2, r3, r0
	adds r3, r3, r1
	ldrb r1, [r3]
	ldrb r0, [r2]
	bl Func_02003bb8
	ldr r3, .L_0200a494
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #80
	strh r3, [r2]
	ldrh r3, [r7]
	ldr r2, .L_0200a498
	movs r0, #128
	orrs r3, r2
	ldr r2, .L_0200a4a8
	strh r3, [r7]
	lsls r0, r0, #4
	movs r3, #0
	strh r3, [r2]
	strh r3, [r2, #2]
	adds r0, #123
	bl GameFlag_SetBit
	bl Func_02003bd0
	add sp, #16
	b .L_0200a4ac
	.2byte 0x0000
.L_0200a494:
	.4byte 0x00000000
.L_0200a498:
	.4byte 0x00000100
.L_0200a49c:
	.4byte 0x02320000
.L_0200a4a0:
	.4byte 0x02090000
.L_0200a4a4:
	.4byte gPartyState
.L_0200a4a8:
	.4byte Data_03001120
.L_0200a4ac:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x0200a4b0,"ax",%progbits
	.global Func_020024b0
	.thumb_func
Func_020024b0:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	ldr r0, .L_0200a4f8
	movs r2, #188
	lsls r2, r2, #1
	adds r7, r3, r2
	ldr r2, .L_0200a4f4
	movs r3, #0
	ldrsb r3, [r0, r3]
	movs r1, #128
	lsls r1, r1, #19
	orrs r3, r2
	adds r1, #82
	strh r3, [r1]
	ldr r3, .L_0200a4fc
	movs r2, #15
	ldr r3, [r3]
	ldr r6, .L_0200a500
	ands r3, r2
	ldrb r4, [r0]
	cmp r3, #0
	bne .L_0200a514
	ldr r1, .L_0200a504
	ldrb r2, [r1]
	adds r3, r4, r2
	strb r3, [r0]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #8
	beq .L_0200a50c
	b .L_0200a508
	.2byte 0x0000
.L_0200a4f4:
	.4byte 0x00001000
.L_0200a4f8:
	.4byte Data_02004694
.L_0200a4fc:
	.4byte Data_0300122c
.L_0200a500:
	.4byte gOverlayArea + 0x46a0
.L_0200a504:
	.4byte Data_02004694 + 0x1
.L_0200a508:
	cmp r3, #4
	bne .L_0200a514
.L_0200a50c:
	lsls r3, r2, #24
	asrs r3, r3, #24
	negs r3, r3
	strb r3, [r1]
.L_0200a514:
	movs r5, #0
.L_0200a516:
	ldr r3, .L_0200a570
	ldrb r0, [r3]
	movs r2, #6
	ldrsh r3, [r7, r2]
	adds r0, r5, r0
	adds r0, r0, r3
	lsls r0, r0, #9
	bl Math_Sine
	movs r2, #2
	ldrsh r3, [r7, r2]
	asrs r0, r0, #13
	adds r3, r3, r0
	adds r5, #1
	strh r3, [r6]
	adds r6, #2
	cmp r5, #160
	bne .L_0200a516
	ldr r6, .L_0200a574
	movs r1, #128
	ldrh r3, [r6]
	lsls r1, r1, #19
	adds r1, #20
	strh r3, [r1]
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #176
	ldrh r0, [r3, #10]
	movs r2, #197
	lsls r2, r2, #8
	adds r2, #255
	ands r2, r0
	strh r2, [r3, #10]
	movs r2, #254
	ldrh r0, [r3, #10]
	lsls r2, r2, #7
	adds r2, #255
	ands r2, r0
	strh r2, [r3, #10]
	adds r0, r6, #2
	ldrh r2, [r3, #10]
	ldr r2, .L_0200a578
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	pop {r5, r6, r7, pc}
.L_0200a570:
	.4byte Data_0300122c
.L_0200a574:
	.4byte gOverlayArea + 0x46a0
.L_0200a578:
	.4byte 0xa2600001
	.section .text.x0200a57c,"ax",%progbits
	.global Func_0200257c
	.thumb_func
Func_0200257c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	movs r1, #188
	lsls r1, r1, #1
	ldr r6, .L_0200a5dc
	adds r1, r1, r3
	mov r10, r1
	movs r7, #0
.L_0200a596:
	ldr r3, .L_0200a5e0
	movs r5, #160
	ldrb r0, [r3]
	subs r5, r5, r7
	adds r0, r7, r0
	lsls r0, r0, #8
	bl Math_Sine
	movs r1, #144
	subs r1, r1, r7
	ldr r3, .L_0200a5e4
	lsls r1, r1, #10
	mov r8, r0
	mov lr, r3
	.2byte 0xf800
	asrs r0, r0, #11
	subs r0, #4
	asrs r5, r5, #2
	strh r0, [r6]
	bl Random16Far
	lsls r1, r5, #1
	bl Engine_MathModulo
	ldrh r3, [r6]
	subs r0, r0, r5
	adds r2, r3, r0
	lsls r3, r2, #16
	asrs r3, r3, #16
	strh r2, [r6]
	cmp r3, #56
	ble .L_0200a5e8
	adds r3, r2, #0
	subs r3, #56
	b .L_0200a5f4
.L_0200a5dc:
	.4byte gOverlayArea + 0x46a0
.L_0200a5e0:
	.4byte Data_0300122c
.L_0200a5e4:
	.4byte IwramMulQ16
.L_0200a5e8:
	movs r1, #64
	negs r1, r1
	cmp r3, r1
	bge .L_0200a5f6
	adds r3, r2, #0
	adds r3, #64
.L_0200a5f4:
	strh r3, [r6]
.L_0200a5f6:
	ldr r3, .L_0200a628
	mov r1, r10
	strh r3, [r6, #2]
	movs r2, #6
	ldrsh r3, [r1, r2]
	mov r2, r8
	adds r0, r2, r3
	movs r2, #2
	ldrsh r3, [r1, r2]
	asrs r2, r0, #13
	adds r3, r3, r2
	adds r7, #1
	strh r3, [r6, #4]
	adds r6, #6
	cmp r7, #160
	bne .L_0200a596
	ldr r0, .L_0200a630
	ldr r2, .L_0200a62c
	movs r3, #0
	ldrsb r3, [r0, r3]
	movs r1, #128
	lsls r1, r1, #19
	orrs r3, r2
	adds r1, #82
	b .L_0200a634
.L_0200a628:
	.4byte 0x00000000
.L_0200a62c:
	.4byte 0x00001000
.L_0200a630:
	.4byte Data_02004696
.L_0200a634:
	strh r3, [r1]
	ldr r3, .L_0200a6a8
	movs r2, #15
	ldr r3, [r3]
	ldrb r4, [r0]
	ands r3, r2
	cmp r3, #0
	bne .L_0200a660
	ldr r1, .L_0200a6ac
	ldrb r2, [r1]
	adds r3, r4, r2
	strb r3, [r0]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #8
	beq .L_0200a658
	cmp r3, #4
	bne .L_0200a660
.L_0200a658:
	lsls r3, r2, #24
	asrs r3, r3, #24
	negs r3, r3
	strb r3, [r1]
.L_0200a660:
	ldr r0, .L_0200a6b0
	movs r1, #128
	ldrh r3, [r0]
	lsls r1, r1, #19
	adds r1, #16
	strh r3, [r1]
	adds r0, #4
	ldrh r3, [r0]
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #20
	strh r3, [r2]
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #176
	ldrh r4, [r3, #10]
	movs r2, #197
	lsls r2, r2, #8
	adds r2, #255
	ands r2, r4
	strh r2, [r3, #10]
	movs r2, #254
	ldrh r4, [r3, #10]
	lsls r2, r2, #7
	adds r2, #255
	ands r2, r4
	strh r2, [r3, #10]
	adds r0, #2
	ldrh r2, [r3, #10]
	ldr r2, .L_0200a6b4
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_0200a6a8:
	.4byte Data_0300122c
.L_0200a6ac:
	.4byte Data_02004696 + 0x1
.L_0200a6b0:
	.4byte gOverlayArea + 0x46a0
.L_0200a6b4:
	.4byte 0xa2600003
	.section .text.x0200a6b8,"ax",%progbits
	.global Func_020026b8
	.thumb_func
Func_020026b8:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r0, #208
	lsls r0, r0, #3
	sub sp, #8
	bl Runtime_BumpAllocate
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	adds r5, r0, #0
	movs r0, #188
	lsls r0, r0, #1
	adds r0, r0, r3
	mov r8, r0
	bl Func_02003bc8
	movs r0, #0
	bl Func_02003c88
	movs r1, #128
	lsls r1, r1, #19
	ldrh r2, [r1]
	movs r3, #254
	lsls r3, r3, #8
	adds r3, #255
	ands r3, r2
	strh r3, [r1]
	ldr r0, .L_0200a770
	bl Resource_GetTableEntry
	adds r1, r5, #0
	bl Func_02003af0
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	adds r0, r5, #0
	ldr r1, .L_0200a774
	adds r2, #208
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r2, #208
	lsls r2, r2, #2
	adds r0, r5, r2
	movs r2, #132
	lsls r2, r2, #24
	ldr r1, .L_0200a778
	adds r2, #208
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	adds r0, r5, #0
	bl Sys_Free
	movs r2, #128
	movs r3, #128
	lsls r2, r2, #3
	lsls r3, r3, #19
	adds r2, #14
	adds r3, #8
	strh r2, [r3]
	mov r0, sp
	ldr r3, .L_0200a76c
	adds r0, #6
	strh r3, [r0]
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #212
	ldr r1, .L_0200a77c
	ldr r2, .L_0200a780
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r0, #144
	lsls r0, r0, #2
	bl Runtime_BumpAllocate
	adds r5, r0, #0
	ldr r0, .L_0200a784
	bl Resource_GetTableEntry
	adds r1, r5, #0
	bl Func_02003af0
	movs r7, #0
	adds r4, r5, #0
	b .L_0200a788
	.2byte 0x0000
.L_0200a76c:
	.4byte 0x00000010
.L_0200a770:
	.4byte 0x000001be
.L_0200a774:
	.4byte 0x0600e800
.L_0200a778:
	.4byte 0x0600ec00
.L_0200a77c:
	.4byte 0x06002000
.L_0200a780:
	.4byte 0x81000280
.L_0200a784:
	.4byte 0x000001bf
.L_0200a788:
	ldrh r3, [r4]
	movs r0, #255
	lsls r0, r0, #8
	adds r0, #64
	movs r2, #144
	adds r3, r3, r0
	adds r7, #1
	lsls r2, r2, #1
	strh r3, [r4]
	adds r4, #2
	cmp r7, r2
	bne .L_0200a788
	adds r4, r5, #0
	movs r7, #0
.L_0200a7a4:
	ldr r2, .L_0200a96c
	lsls r1, r7, #6
	adds r1, r1, r2
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	adds r0, r4, #0
	adds r2, #8
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	adds r7, #1
	adds r4, #32
	cmp r7, #18
	bne .L_0200a7a4
	adds r0, r5, #0
	bl Sys_Free
	movs r0, #246
	bl Func_02003cd8
	movs r0, #12
	bl Object_GetById
	movs r3, #9
	adds r0, #98
	strb r3, [r0]
	movs r0, #12
	bl Object_GetById
	movs r3, #1
	adds r0, #99
	strb r3, [r0]
	movs r0, #12
	bl Object_GetById
	ldr r3, .L_0200a970
	movs r7, #0
	str r3, [r0, #108]
.L_0200a7f4:
	ldr r3, .L_0200a974
	ldr r6, [r3]
	movs r3, #3
	ands r6, r3
	cmp r6, #0
	bne .L_0200a898
	movs r0, #12
	bl Object_GetById
	str r0, [sp, #0]
	movs r0, #12
	bl Object_GetById
	adds r5, r0, #0
	movs r0, #12
	bl Object_GetById
	ldr r3, [sp, #0]
	ldr r2, [r5, #12]
	ldr r1, [r3, #8]
	ldr r3, [r0, #16]
	movs r0, #168
	lsls r0, r0, #2
	bl Func_02003b40
	adds r5, r0, #0
	movs r0, #12
	bl Object_GetById
	ldr r3, [r0, #80]
	ldr r4, [r5, #80]
	ldrb r3, [r3, #9]
	movs r1, #12
	ands r1, r3
	movs r0, #13
	ldrb r3, [r4, #9]
	negs r0, r0
	adds r2, r0, #0
	ands r3, r2
	orrs r3, r1
	adds r1, r5, #0
	adds r1, #35
	ldrb r2, [r1]
	strb r3, [r4, #9]
	movs r3, #1
	orrs r3, r2
	strb r3, [r1]
	adds r3, r5, #0
	adds r3, #85
	strb r6, [r3]
	ldr r3, [r5, #8]
	str r3, [r5, #68]
	movs r3, #192
	lsls r3, r3, #9
	str r3, [r5, #72]
	ldr r3, [r5, #16]
	str r3, [r5, #76]
	bl Random16Far
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	ands r3, r0
	str r3, [r5, #48]
	adds r0, r5, #0
	movs r1, #7
	bl Func_02003b28
	adds r0, r5, #0
	ldr r1, .L_0200a978
	bl Func_02003b38
	adds r0, r5, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	adds r0, r5, #0
	movs r1, #2
	bl Object_SetPartAttribute
	ldr r3, .L_0200a97c
	str r3, [r5, #108]
.L_0200a898:
	movs r0, #1
	adds r7, #1
	bl Battle_WaitMode0
	cmp r7, #45
	bne .L_0200a7f4
	ldr r3, .L_0200a980
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	movs r1, #1
	bl Object_AttachWorkTargetToObject
	bl Func_02003c30
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #9
	lsls r1, r1, #6
	bl Func_02003c20
	movs r0, #152
	movs r1, #1
	movs r2, #144
	negs r1, r1
	lsls r2, r2, #16
	movs r3, #1
	lsls r0, r0, #16
	bl Motion_CamBounds
	bl Func_02003c30
	ldr r0, .L_0200a984
	bl Scheduler_RemoveCallbackFar
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #10
	ldrh r1, [r2]
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #252
	ands r3, r1
	strh r3, [r2]
	movs r0, #128
	ldrh r3, [r2]
	lsls r0, r0, #9
	strh r3, [r2]
	movs r1, #0
	bl Func_02003c58
	movs r0, #254
	lsls r0, r0, #7
	movs r1, #0
	adds r0, #255
	bl Func_02003c50
	movs r0, #1
	bl Func_02003c60
	movs r0, #1
	bl WaitFrames
	movs r0, #254
	lsls r0, r0, #7
	adds r0, #255
	movs r1, #0
	bl Func_02003c58
	movs r0, #128
	movs r1, #0
	lsls r0, r0, #9
	bl Func_02003c50
	movs r0, #8
	bl Func_02003c60
	movs r1, #200
	lsls r1, r1, #4
	ldr r0, .L_0200a988
	bl Scheduler_AddOrUpdateCallback
	movs r0, #163
	bl Func_02003cd8
	movs r1, #128
	lsls r1, r1, #19
	ldrh r3, [r1]
	ldr r2, .L_0200a968
	movs r0, #192
	orrs r3, r2
	strh r3, [r1]
	movs r2, #128
	movs r1, #192
	lsls r0, r0, #10
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl Func_02003b98
	ldr r6, .L_0200a98c
	movs r7, #128
	lsls r7, r7, #7
	b .L_0200a990
.L_0200a968:
	.4byte 0x00000100
.L_0200a96c:
	.4byte 0x0600200f
.L_0200a970:
	.4byte Func_0200092c
.L_0200a974:
	.4byte Data_0300122c
.L_0200a978:
	.4byte Data_02003d1c
.L_0200a97c:
	.4byte Func_020008ac
.L_0200a980:
	.4byte gPartyState
.L_0200a984:
	.4byte Func_020024b0
.L_0200a988:
	.4byte Func_0200257c
.L_0200a98c:
	.4byte 0xffff8000
.L_0200a990:
	mov r3, r8
	str r7, [r3, #24]
	str r6, [r3, #28]
	movs r0, #12
	bl Object_GetById
	adds r5, r0, #0
	bl Random16Far
	movs r3, #7
	ands r0, r3
	adds r5, #98
	strb r0, [r5]
	movs r0, #1
	bl WaitFrames
	movs r0, #128
	ldr r2, .L_0200aa18
	lsls r0, r0, #3
	movs r3, #144
	adds r7, r7, r0
	lsls r3, r3, #11
	adds r6, r6, r2
	cmp r7, r3
	bne .L_0200a990
	ldr r0, .L_0200aa1c
	bl Scheduler_RemoveCallbackFar
	movs r1, #200
	ldr r0, .L_0200aa20
	lsls r1, r1, #4
	bl Scheduler_AddOrUpdateCallback
	ldr r3, .L_0200aa0c
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	ldr r3, .L_0200aa10
	subs r2, #2
	strh r3, [r2]
	movs r7, #0
.L_0200a9e4:
	ldr r3, .L_0200aa0c
	asrs r2, r7, #5
	subs r3, r3, r2
	ldr r2, .L_0200aa14
	movs r6, #128
	lsls r6, r6, #19
	orrs r3, r2
	adds r6, #82
	strh r3, [r6]
	movs r0, #12
	bl Object_GetById
	adds r5, r0, #0
	bl Random16Far
	movs r3, #7
	ands r0, r3
	adds r5, #98
	b .L_0200aa24
	.2byte 0x0000
.L_0200aa0c:
	.4byte 0x00000008
.L_0200aa10:
	.4byte 0x00003f42
.L_0200aa14:
	.4byte 0x00001000
.L_0200aa18:
	.4byte 0xfffffc00
.L_0200aa1c:
	.4byte Func_0200257c
.L_0200aa20:
	.4byte Func_02000ef4
.L_0200aa24:
	strb r0, [r5]
	movs r0, #1
	bl Battle_WaitMode0
	movs r0, #2
	adds r7, #1
	adds r0, #255
	cmp r7, r0
	bne .L_0200a9e4
	movs r1, #128
	lsls r1, r1, #19
	ldrh r2, [r1]
	movs r3, #253
	lsls r3, r3, #8
	adds r3, #255
	ands r3, r2
	strh r3, [r1]
	movs r0, #30
	bl WaitFrames
	movs r0, #195
	lsls r0, r0, #1
	bl Func_02003cd8
	ldr r3, .L_0200aa90
	movs r2, #128
	strh r3, [r6]
	ldr r3, .L_0200aa94
	lsls r2, r2, #19
	adds r2, #80
	strh r3, [r2]
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	negs r0, r0
	negs r1, r1
	adds r2, #102
	bl Func_02003b98
	movs r0, #12
	bl Object_GetById
	movs r3, #6
	adds r0, #98
	strb r3, [r0]
	movs r7, #0
.L_0200aa82:
	ldr r3, .L_0200aa90
	asrs r1, r7, #3
	movs r0, #128
	lsls r2, r1, #8
	subs r3, r3, r1
	b .L_0200aa98
	.2byte 0x0000
.L_0200aa90:
	.4byte 0x00000010
.L_0200aa94:
	.4byte 0x00003f41
.L_0200aa98:
	lsls r0, r0, #19
	adds r0, #82
	orrs r2, r3
	strh r2, [r0]
	adds r7, #1
	movs r0, #1
	bl Battle_WaitMode0
	cmp r7, #129
	bne .L_0200aa82
	ldr r0, .L_0200ab5c
	bl Scheduler_RemoveCallbackFar
	movs r0, #12
	bl Object_GetById
	movs r5, #0
	adds r0, #99
	strb r5, [r0]
	movs r0, #30
	bl WaitFrames
	movs r0, #12
	bl Object_GetById
	str r5, [r0, #108]
	movs r0, #12
	bl Object_GetById
	movs r1, #6
	bl Object_SetPartAttribute
	bl Func_02003ba0
	movs r0, #60
	bl Battle_WaitMode0
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #80
	strh r5, [r3]
	bl Func_02003b00
	bl Func_02003af8
	ldr r5, .L_0200ab60
	movs r2, #147
	lsls r2, r2, #1
	adds r2, #255
	adds r3, r5, r2
	adds r2, #1
	ldrb r0, [r3]
	adds r3, r5, r2
	ldrb r1, [r3]
	bl Func_02003bb8
	movs r1, #128
	lsls r1, r1, #19
	adds r1, #10
	ldrh r2, [r1]
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #252
	ands r3, r2
	strh r3, [r1]
	ldr r2, .L_0200ab58
	ldrh r3, [r1]
	orrs r3, r2
	strh r3, [r1]
	movs r3, #133
	lsls r3, r3, #2
	adds r7, r5, r3
	ldr r0, [r7]
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #223
	ands r3, r2
	strb r3, [r0]
	ldr r0, [r7]
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r5, #1
	orrs r3, r5
	strb r3, [r0]
	movs r0, #12
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r1, #1
	b .L_0200ab64
	.2byte 0x0000
.L_0200ab58:
	.4byte 0x00000001
.L_0200ab5c:
	.4byte Func_02000ef4
.L_0200ab60:
	.4byte gPartyState
.L_0200ab64:
	orrs r5, r3
	strb r5, [r0]
	ldr r0, [r7]
	bl Object_AttachWorkTargetToObject
	bl Func_02003c30
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #122
	bl GameFlag_SetBit
	bl Func_02003bd0
	add sp, #8
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.section .text.x0200ab88,"ax",%progbits
	.global Func_02002b88
	.thumb_func
Func_02002b88:
	push {r5, lr}
	sub sp, #8
	bl Func_02003cc8
	bl Object_GetById
	adds r1, r0, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #32
	orrs r3, r2
	strb r3, [r1]
	movs r3, #128
	ldr r0, [r0, #12]
	lsls r3, r3, #14
	adds r0, r0, r3
	asrs r0, r0, #16
	movs r1, #112
	bl Engine_MathDivide
	ldr r3, .L_0200ac20
	adds r5, r0, #0
	ldr r3, [r3]
	cmp r3, r5
	beq .L_0200ac1a
	cmp r5, #1
	beq .L_0200abec
	cmp r5, #1
	bgt .L_0200abc8
	cmp r5, #0
	beq .L_0200ac02
	b .L_0200ac16
.L_0200abc8:
	cmp r5, #2
	beq .L_0200abde
	cmp r5, #3
	bne .L_0200ac16
	movs r3, #27
	movs r2, #54
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #64
	movs r1, #64
	b .L_0200abf8
.L_0200abde:
	movs r3, #27
	movs r2, #54
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #64
	movs r1, #67
	b .L_0200abf8
.L_0200abec:
	movs r3, #27
	movs r2, #54
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #64
	movs r1, #70
.L_0200abf8:
	movs r2, #21
	movs r3, #3
	bl Func_02003b80
	b .L_0200ac16
.L_0200ac02:
	movs r3, #27
	movs r2, #54
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #64
	movs r1, #73
	movs r2, #21
	movs r3, #3
	bl Func_02003b80
.L_0200ac16:
	ldr r3, .L_0200ac20
	str r5, [r3]
.L_0200ac1a:
	add sp, #8
	pop {r5, pc}
	.2byte 0x0000
.L_0200ac20:
	.4byte Data_02004698
	.section .text.x0200ac24,"ax",%progbits
	.global Func_02002c24
	.thumb_func
Func_02002c24:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, .L_0200ac9c
	adds r7, r0, #0
	mov r8, r3
	movs r3, #133
	lsls r3, r3, #2
	add r3, r8
	ldr r0, [r3]
	mov r10, r3
	bl Object_GetById
	adds r5, r0, #0
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	adds r6, r0, #0
	cmp r6, #0
	bne .L_0200ac94
	adds r3, r5, #0
	adds r3, #85
	strb r6, [r3]
	adds r0, r5, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	ldr r3, [r5, #12]
	movs r2, #128
	adds r3, r3, r7
	str r3, [r5, #12]
	lsls r2, r2, #2
	movs r3, #128
	lsls r3, r3, #9
	adds r2, #18
	str r3, [r5, #48]
	add r2, r8
	movs r3, #2
	str r6, [r5, #40]
	strb r3, [r2]
	adds r2, r5, #0
	adds r2, #90
	movs r3, #1
	strb r3, [r2]
	mov r3, r10
	ldr r0, [r3]
	movs r1, #0
	bl Object_AttachWorkTargetToObject
	bl Func_02003b50
	movs r0, #1
	bl WaitFrames
.L_0200ac94:
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_0200ac9c:
	.4byte gPartyState
	.section .text.x0200aca0,"ax",%progbits
	.global Func_02002ca0
	.thumb_func
Func_02002ca0:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	ldr r6, .L_0200b054
	movs r0, #214
	lsls r0, r0, #1
	ldr r5, .L_0200b058
	movs r2, #129
	movs r1, #152
	adds r3, r3, r0
	lsls r1, r1, #2
	lsls r2, r2, #2
	str r2, [r3]
	adds r3, r6, r1
	strh r5, [r3]
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #98
	adds r2, r6, r3
	movs r3, #1
	strh r3, [r2]
	adds r0, #104
	adds r3, r6, r0
	ldr r0, [r3]
	sub sp, #8
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #32
	movs r1, #240
	orrs r3, r2
	lsls r1, r1, #1
	strb r3, [r0]
	adds r3, r6, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, r5
	beq .L_0200acf6
	b .L_0200af9c
.L_0200acf6:
	movs r0, #241
	lsls r0, r0, #1
	adds r3, r6, r0
	movs r1, #0
	ldrsh r2, [r3, r1]
	cmp r2, #4
	bgt .L_0200ad10
	cmp r2, #2
	blt .L_0200ad10
	movs r0, #176
	lsls r0, r0, #15
	bl Func_02002c24
.L_0200ad10:
	movs r0, #133
	lsls r0, r0, #4
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200ad54
	movs r3, #3
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #64
	movs r1, #0
	movs r2, #113
	movs r3, #11
	bl Func_02003b68
	movs r3, #113
	movs r5, #12
	str r3, [sp, #0]
	movs r0, #64
	movs r1, #0
	movs r2, #3
	movs r3, #2
	str r5, [sp, #4]
	bl Func_02003b80
	movs r3, #49
	str r3, [sp, #0]
	movs r0, #64
	movs r1, #0
	movs r2, #3
	movs r3, #2
	str r5, [sp, #4]
	bl Func_02003b80
.L_0200ad54:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #81
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200ad9c
	movs r3, #1
	movs r2, #4
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #64
	movs r1, #0
	movs r2, #70
	movs r3, #39
	bl Func_02003b68
	movs r3, #70
	movs r5, #40
	str r3, [sp, #0]
	movs r0, #64
	movs r1, #0
	movs r2, #1
	movs r3, #3
	str r5, [sp, #4]
	bl Func_02003b80
	movs r3, #6
	str r3, [sp, #0]
	movs r0, #64
	movs r1, #0
	movs r2, #1
	movs r3, #3
	str r5, [sp, #4]
	bl Func_02003b80
.L_0200ad9c:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #82
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200ade2
	movs r3, #1
	movs r2, #4
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #64
	movs r1, #0
	movs r2, #103
	movs r3, #38
	bl Func_02003b68
	movs r3, #103
	movs r5, #39
	str r3, [sp, #0]
	movs r0, #64
	movs r1, #0
	movs r2, #1
	movs r3, #3
	str r5, [sp, #4]
	bl Func_02003b80
	movs r0, #64
	movs r1, #0
	movs r2, #1
	movs r3, #3
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl Func_02003b80
.L_0200ade2:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #83
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200ae2a
	movs r3, #1
	movs r2, #4
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #64
	movs r1, #0
	movs r2, #87
	movs r3, #43
	bl Func_02003b68
	movs r3, #87
	movs r5, #44
	str r3, [sp, #0]
	movs r0, #64
	movs r1, #0
	movs r2, #1
	movs r3, #3
	str r5, [sp, #4]
	bl Func_02003b80
	movs r3, #23
	str r3, [sp, #0]
	movs r0, #64
	movs r1, #0
	movs r2, #1
	movs r3, #3
	str r5, [sp, #4]
	bl Func_02003b80
.L_0200ae2a:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #84
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200ae74
	movs r3, #1
	movs r2, #4
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #64
	movs r1, #0
	movs r2, #70
	movs r3, #52
	bl Func_02003b68
	movs r3, #70
	movs r2, #52
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #64
	movs r1, #0
	movs r2, #1
	movs r3, #3
	bl Func_02003b80
	movs r3, #6
	movs r2, #53
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #64
	movs r1, #0
	movs r2, #1
	movs r3, #3
	bl Func_02003b80
.L_0200ae74:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #85
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200aeba
	movs r3, #3
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #64
	movs r1, #0
	movs r2, #79
	movs r3, #49
	bl Func_02003b68
	movs r3, #79
	movs r5, #50
	str r3, [sp, #0]
	movs r0, #64
	movs r1, #0
	movs r2, #3
	movs r3, #2
	str r5, [sp, #4]
	bl Func_02003b80
	movs r3, #15
	str r3, [sp, #0]
	movs r0, #64
	movs r1, #0
	movs r2, #3
	movs r3, #2
	str r5, [sp, #4]
	bl Func_02003b80
.L_0200aeba:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #86
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200af02
	movs r3, #1
	movs r2, #4
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #64
	movs r1, #0
	movs r2, #103
	movs r3, #48
	bl Func_02003b68
	movs r3, #103
	movs r5, #49
	str r3, [sp, #0]
	movs r0, #64
	movs r1, #0
	movs r2, #1
	movs r3, #3
	str r5, [sp, #4]
	bl Func_02003b80
	movs r3, #39
	str r3, [sp, #0]
	movs r0, #64
	movs r1, #0
	movs r2, #1
	movs r3, #3
	str r5, [sp, #4]
	bl Func_02003b80
.L_0200af02:
	movs r0, #235
	lsls r0, r0, #3
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200af48
	movs r3, #3
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #64
	movs r1, #0
	movs r2, #111
	movs r3, #52
	bl Func_02003b68
	movs r3, #111
	movs r5, #53
	str r3, [sp, #0]
	movs r0, #64
	movs r1, #0
	movs r2, #3
	movs r3, #2
	str r5, [sp, #4]
	bl Func_02003b80
	movs r3, #47
	str r3, [sp, #0]
	movs r0, #64
	movs r1, #0
	movs r2, #3
	movs r3, #2
	str r5, [sp, #4]
	bl Func_02003b80
.L_0200af48:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #88
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200af9c
	movs r3, #11
	movs r2, #10
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #68
	movs r1, #0
	movs r2, #79
	movs r3, #0
	bl Func_02003b68
	movs r3, #16
	str r3, [sp, #0]
	movs r5, #9
	movs r0, #38
	movs r1, #9
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl Func_02003b80
	movs r3, #24
	str r3, [sp, #0]
	movs r1, #9
	movs r0, #38
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl Func_02003b80
	movs r0, #8
	bl Object_GetById
	movs r1, #6
	bl Object_SetPartAttribute
.L_0200af9c:
	ldr r1, .L_0200b054
	movs r2, #240
	lsls r2, r2, #1
	adds r3, r1, r2
	movs r0, #0
	ldrsh r2, [r3, r0]
	ldr r3, .L_0200b05c
	cmp r2, r3
	beq .L_0200afb0
	b .L_0200b398
.L_0200afb0:
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r1, r2
	movs r0, #0
	ldrsh r3, [r3, r0]
	subs r3, #1
	cmp r3, #11
	bls .L_0200afc2
	b .L_0200b398
.L_0200afc2:
	ldr r2, .L_0200b060
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
	.2byte 0x0000
.L_0200afcc:
	.4byte .L_0200b028
	.4byte .L_0200b028
	.4byte .L_0200affc
	.4byte .L_0200b0a8
	.4byte .L_0200b06c
	.4byte .L_0200b0a0
	.4byte .L_0200b398
	.4byte .L_0200b398
	.4byte .L_0200b398
	.4byte .L_0200b1f2
	.4byte .L_0200b1c0
	.4byte .L_0200b21c
.L_0200affc:
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200b020
	ldr r3, .L_0200b054
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	ldr r0, [r3]
	bl Object_GetById
	ldr r3, [r0, #16]
	movs r2, #128
	lsls r2, r2, #13
	adds r3, r3, r2
	str r3, [r0, #16]
.L_0200b020:
	ldr r0, .L_0200b064
	bl Func_02002c24
	b .L_0200b0a8
.L_0200b028:
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200b04c
	ldr r3, .L_0200b054
	movs r0, #133
	lsls r0, r0, #2
	adds r3, r3, r0
	ldr r0, [r3]
	bl Object_GetById
	ldr r3, [r0, #16]
	movs r1, #128
	lsls r1, r1, #13
	adds r3, r3, r1
	str r3, [r0, #16]
.L_0200b04c:
	ldr r0, .L_0200b068
	bl Func_02002c24
	b .L_0200b0a8
.L_0200b054:
	.4byte gPartyState
.L_0200b058:
	.4byte 0x00000052
.L_0200b05c:
	.4byte 0x00000053
.L_0200b060:
	.4byte .L_0200afcc
.L_0200b064:
	.4byte 0xffb80000
.L_0200b068:
	.4byte 0xffd80000
.L_0200b06c:
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200b0a0
	ldr r5, .L_0200b1b0
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
	ldr r0, [r5]
	bl Object_GetById
	ldr r3, [r0, #8]
	movs r1, #192
	lsls r1, r1, #14
	adds r3, r3, r1
	str r3, [r0, #8]
	ldr r0, [r5]
	bl Object_GetById
	ldr r3, [r0, #12]
	movs r2, #128
	lsls r2, r2, #13
	adds r3, r3, r2
	str r3, [r0, #12]
.L_0200b0a0:
	movs r0, #224
	lsls r0, r0, #14
	bl Func_02002c24
.L_0200b0a8:
	movs r0, #8
	bl Object_GetById
	movs r6, #60
	adds r0, #100
	movs r3, #0
	strh r6, [r0]
	movs r0, #8
	mov r8, r3
	bl Object_GetById
	ldr r5, .L_0200b1b4
	str r5, [r0, #108]
	movs r0, #8
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #9
	bl Object_GetById
	movs r3, #180
	adds r0, #100
	strh r3, [r0]
	movs r0, #9
	bl Object_GetById
	str r5, [r0, #108]
	movs r0, #9
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #10
	bl Object_GetById
	mov r1, r8
	adds r0, #100
	strh r1, [r0]
	movs r0, #10
	bl Object_GetById
	str r5, [r0, #108]
	movs r0, #10
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #11
	bl Object_GetById
	adds r0, #100
	strh r6, [r0]
	movs r0, #11
	bl Object_GetById
	str r5, [r0, #108]
	movs r0, #11
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #236
	lsls r0, r0, #3
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200b148
	movs r1, #132
	movs r2, #174
	movs r0, #12
	lsls r1, r1, #17
	lsls r2, r2, #18
	bl Func_02003bf8
.L_0200b148:
	movs r0, #134
	lsls r0, r0, #4
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200b162
	movs r1, #182
	movs r2, #182
	movs r0, #13
	lsls r1, r1, #18
	lsls r2, r2, #18
	bl Func_02003bf8
.L_0200b162:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #97
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200b17e
	movs r1, #172
	movs r2, #194
	movs r0, #15
	lsls r1, r1, #17
	lsls r2, r2, #18
	bl Func_02003bf8
.L_0200b17e:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #98
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200b19a
	movs r1, #158
	movs r2, #198
	movs r0, #16
	lsls r1, r1, #18
	lsls r2, r2, #18
	bl Func_02003bf8
.L_0200b19a:
	ldr r5, .L_0200b1b8
	adds r0, r5, #0
	bl Func_02003cb0
	adds r0, r5, #0
	bl Func_020003e8
	ldr r0, .L_0200b1bc
	bl Func_02003ca0
	b .L_0200b398
.L_0200b1b0:
	.4byte gPartyState
.L_0200b1b4:
	.4byte Func_02001b54
.L_0200b1b8:
	.4byte Data_02003d94
.L_0200b1bc:
	.4byte Data_02003e4c
.L_0200b1c0:
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200b1f2
	ldr r5, .L_0200b2b4
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
	ldr r0, [r5]
	bl Object_GetById
	ldr r3, [r0, #8]
	movs r1, #128
	lsls r1, r1, #13
	adds r3, r3, r1
	str r3, [r0, #8]
	ldr r0, [r5]
	bl Object_GetById
	ldr r2, .L_0200b2b8
	ldr r3, [r0, #12]
	adds r3, r3, r2
	str r3, [r0, #12]
.L_0200b1f2:
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200b216
	ldr r3, .L_0200b2b4
	movs r0, #133
	lsls r0, r0, #2
	adds r3, r3, r0
	ldr r0, [r3]
	bl Object_GetById
	ldr r3, [r0, #16]
	movs r1, #128
	lsls r1, r1, #13
	adds r3, r3, r1
	str r3, [r0, #16]
.L_0200b216:
	ldr r0, .L_0200b2bc
	bl Func_02002c24
.L_0200b21c:
	movs r0, #241
	lsls r0, r0, #3
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200b238
	movs r1, #198
	movs r2, #148
	movs r0, #22
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl Func_02003bf8
.L_0200b238:
	ldr r5, .L_0200b2c0
	adds r0, r5, #0
	bl Func_02003cb0
	adds r0, r5, #0
	bl Func_020003e8
	movs r0, #17
	bl Object_GetById
	movs r3, #0
	adds r0, #85
	strb r3, [r0]
	movs r0, #17
	bl Object_GetById
	movs r3, #128
	lsls r3, r3, #13
	str r3, [r0, #12]
	movs r0, #17
	bl Object_GetById
	movs r3, #60
	adds r0, #100
	strh r3, [r0]
	movs r0, #17
	bl Object_GetById
	ldr r6, .L_0200b2c4
	ldr r5, .L_0200b2b0
	str r6, [r0, #108]
	movs r0, #17
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #19
	bl Object_GetById
	adds r0, #85
	strb r5, [r0]
	movs r0, #19
	bl Object_GetById
	movs r3, #160
	lsls r3, r3, #15
	str r3, [r0, #12]
	movs r0, #19
	bl Object_GetById
	movs r3, #90
	adds r0, #100
	strh r3, [r0]
	movs r0, #19
	bl Object_GetById
	str r6, [r0, #108]
	movs r0, #19
	b .L_0200b2c8
.L_0200b2b0:
	.4byte 0x00000000
.L_0200b2b4:
	.4byte gPartyState
.L_0200b2b8:
	.4byte 0xfff00000
.L_0200b2bc:
	.4byte 0xffc80000
.L_0200b2c0:
	.4byte Data_02003d94
.L_0200b2c4:
	.4byte Func_02001b54
.L_0200b2c8:
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #18
	bl Object_GetById
	adds r0, #85
	strb r5, [r0]
	movs r0, #18
	bl Object_GetById
	ldr r3, .L_0200b554
	str r3, [r0, #12]
	movs r0, #18
	bl Object_GetById
	movs r3, #100
	adds r0, #100
	strh r3, [r0]
	movs r0, #18
	bl Object_GetById
	str r6, [r0, #108]
	movs r0, #18
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #20
	bl Object_GetById
	adds r0, #85
	strb r5, [r0]
	movs r0, #20
	bl Object_GetById
	movs r3, #144
	lsls r3, r3, #16
	str r3, [r0, #12]
	movs r0, #20
	bl Object_GetById
	movs r2, #180
	mov r8, r2
	mov r3, r8
	adds r0, #100
	strh r3, [r0]
	movs r0, #20
	bl Object_GetById
	str r6, [r0, #108]
	movs r0, #20
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #21
	bl Object_GetById
	adds r0, #85
	strb r5, [r0]
	movs r0, #21
	bl Object_GetById
	ldr r3, .L_0200b558
	movs r1, #2
	str r3, [r0, #12]
	movs r0, #21
	bl ObjectMotion_SetActionVariant
	movs r0, #21
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #2
	orrs r3, r2
	strb r3, [r0]
	movs r0, #21
	bl Object_GetById
	mov r1, r8
	adds r0, #100
	strh r1, [r0]
	movs r0, #21
	bl Object_GetById
	movs r3, #1
	adds r0, #98
	strb r3, [r0]
	movs r0, #21
	bl Object_GetById
	str r6, [r0, #108]
	movs r0, #21
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
.L_0200b398:
	ldr r1, .L_0200b55c
	movs r2, #240
	lsls r2, r2, #1
	adds r3, r1, r2
	movs r0, #0
	ldrsh r2, [r3, r0]
	ldr r3, .L_0200b560
	cmp r2, r3
	beq .L_0200b3ac
	b .L_0200ba66
.L_0200b3ac:
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r1, r2
	movs r0, #0
	ldrsh r3, [r3, r0]
	subs r3, #1
	cmp r3, #98
	bls .L_0200b3be
	b .L_0200ba66
.L_0200b3be:
	ldr r2, .L_0200b564
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
	.2byte 0x0000
.L_0200b3c8:
	.4byte .L_0200b568
	.4byte .L_0200b568
	.4byte .L_0200b568
	.4byte .L_0200b70e
	.4byte .L_0200b70e
	.4byte .L_0200b6e2
	.4byte .L_0200b706
	.4byte .L_0200b720
	.4byte .L_0200b720
	.4byte .L_0200b754
	.4byte .L_0200b778
	.4byte .L_0200b8b0
	.4byte .L_0200b8d2
	.4byte .L_0200b8fc
	.4byte .L_0200ba66
	.4byte .L_0200ba66
	.4byte .L_0200ba66
	.4byte .L_0200ba66
	.4byte .L_0200ba66
	.4byte .L_0200ba66
	.4byte .L_0200ba66
	.4byte .L_0200ba66
	.4byte .L_0200ba66
	.4byte .L_0200ba66
	.4byte .L_0200ba66
	.4byte .L_0200ba66
	.4byte .L_0200ba66
	.4byte .L_0200ba66
	.4byte .L_0200ba66
	.4byte .L_0200ba66
	.4byte .L_0200ba66
	.4byte .L_0200ba66
	.4byte .L_0200ba66
	.4byte .L_0200ba66
	.4byte .L_0200ba66
	.4byte .L_0200ba66
	.4byte .L_0200ba66
	.4byte .L_0200ba66
	.4byte .L_0200ba66
	.4byte .L_0200ba66
	.4byte .L_0200ba66
	.4byte .L_0200ba66
	.4byte .L_0200ba66
	.4byte .L_0200ba66
	.4byte .L_0200ba66
	.4byte .L_0200ba66
	.4byte .L_0200ba66
	.4byte .L_0200ba66
	.4byte .L_0200ba66
	.4byte .L_0200ba66
	.4byte .L_0200ba66
	.4byte .L_0200ba66
	.4byte .L_0200ba66
	.4byte .L_0200ba66
	.4byte .L_0200ba66
	.4byte .L_0200ba66
	.4byte .L_0200ba66
	.4byte .L_0200ba66
	.4byte .L_0200ba66
	.4byte .L_0200ba66
	.4byte .L_0200ba66
	.4byte .L_0200ba66
	.4byte .L_0200ba66
	.4byte .L_0200ba66
	.4byte .L_0200ba66
	.4byte .L_0200ba66
	.4byte .L_0200ba66
	.4byte .L_0200ba66
	.4byte .L_0200ba66
	.4byte .L_0200ba66
	.4byte .L_0200ba66
	.4byte .L_0200ba66
	.4byte .L_0200ba66
	.4byte .L_0200ba66
	.4byte .L_0200ba66
	.4byte .L_0200ba66
	.4byte .L_0200ba66
	.4byte .L_0200ba66
	.4byte .L_0200ba66
	.4byte .L_0200ba66
	.4byte .L_0200ba66
	.4byte .L_0200ba66
	.4byte .L_0200ba66
	.4byte .L_0200ba66
	.4byte .L_0200ba66
	.4byte .L_0200ba66
	.4byte .L_0200ba66
	.4byte .L_0200ba66
	.4byte .L_0200ba66
	.4byte .L_0200ba66
	.4byte .L_0200ba66
	.4byte .L_0200ba66
	.4byte .L_0200ba66
	.4byte .L_0200ba66
	.4byte .L_0200ba66
	.4byte .L_0200ba66
	.4byte .L_0200ba66
	.4byte .L_0200ba66
	.4byte .L_0200b9f2
.L_0200b554:
	.4byte 0xffd00000
.L_0200b558:
	.4byte 0xff700000
.L_0200b55c:
	.4byte gPartyState
.L_0200b560:
	.4byte 0x00000054
.L_0200b564:
	.4byte .L_0200b3c8
.L_0200b568:
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_0200b5c8
	bl Scheduler_AddOrUpdateCallback
	movs r0, #8
	bl Object_GetById
	movs r3, #0
	adds r0, #85
	strb r3, [r0]
	movs r0, #8
	bl Object_GetById
	ldr r1, .L_0200b5cc
	ldr r5, .L_0200b5c4
	mov r8, r1
	str r1, [r0, #12]
	movs r1, #2
	movs r0, #8
	bl ObjectMotion_SetActionVariant
	movs r0, #8
	bl Object_GetById
	movs r3, #60
	adds r0, #100
	strh r3, [r0]
	movs r0, #8
	bl Object_GetById
	adds r0, #98
	strb r5, [r0]
	movs r0, #8
	bl Object_GetById
	ldr r6, .L_0200b5d0
	str r6, [r0, #108]
	movs r0, #8
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #9
	b .L_0200b5d4
.L_0200b5c4:
	.4byte 0x00000000
.L_0200b5c8:
	.4byte Func_02002b88
.L_0200b5cc:
	.4byte 0xfff80000
.L_0200b5d0:
	.4byte Func_02001b54
.L_0200b5d4:
	bl Object_GetById
	adds r0, #85
	strb r5, [r0]
	movs r0, #9
	bl Object_GetById
	mov r2, r8
	str r2, [r0, #12]
	movs r1, #2
	movs r0, #9
	bl ObjectMotion_SetActionVariant
	movs r0, #9
	bl Object_GetById
	movs r3, #180
	adds r0, #100
	strh r3, [r0]
	movs r0, #9
	bl Object_GetById
	adds r0, #98
	strb r5, [r0]
	movs r0, #9
	bl Object_GetById
	str r6, [r0, #108]
	movs r0, #9
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #10
	bl Object_GetById
	movs r3, #2
	mov r8, r3
	adds r0, #34
	mov r1, r8
	strb r1, [r0]
	movs r1, #1
	movs r0, #10
	bl ObjectMotion_SetActionVariant
	movs r0, #10
	bl Object_GetById
	movs r6, #208
	lsls r6, r6, #16
	str r6, [r0, #12]
	movs r0, #10
	bl Object_GetById
	adds r5, r0, #0
	movs r0, #10
	bl Object_GetById
	ldr r3, [r0, #12]
	movs r0, #11
	str r3, [r5, #20]
	bl Object_GetById
	mov r2, r8
	adds r0, #34
	strb r2, [r0]
	movs r1, #1
	movs r0, #11
	bl ObjectMotion_SetActionVariant
	movs r0, #11
	bl Object_GetById
	str r6, [r0, #12]
	movs r0, #11
	bl Object_GetById
	adds r5, r0, #0
	movs r0, #11
	bl Object_GetById
	ldr r3, [r0, #12]
	movs r0, #196
	str r3, [r5, #20]
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200b696
	movs r1, #252
	movs r2, #218
	movs r0, #10
	lsls r1, r1, #17
	lsls r2, r2, #18
	bl Func_02003bf8
.L_0200b696:
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #17
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200b6b2
	movs r1, #130
	movs r2, #218
	movs r0, #11
	lsls r1, r1, #18
	lsls r2, r2, #18
	bl Func_02003bf8
.L_0200b6b2:
	bl Func_02000620
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #32]
	movs r3, #188
	lsls r3, r3, #1
	adds r2, r2, r3
	movs r3, #176
	lsls r3, r3, #19
	str r3, [r2, #8]
	movs r3, #144
	lsls r3, r3, #18
	str r3, [r2, #12]
	movs r3, #128
	lsls r3, r3, #7
	str r3, [r2, #16]
	str r3, [r2, #20]
	bl Func_02003b50
	movs r0, #1
	bl WaitFrames
	b .L_0200ba66
.L_0200b6e2:
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200b706
	ldr r3, .L_0200b74c
	movs r0, #133
	lsls r0, r0, #2
	adds r3, r3, r0
	ldr r0, [r3]
	bl Object_GetById
	ldr r3, [r0, #12]
	movs r1, #192
	lsls r1, r1, #13
	adds r3, r3, r1
	str r3, [r0, #12]
.L_0200b706:
	movs r0, #144
	lsls r0, r0, #15
	bl Func_02002c24
.L_0200b70e:
	ldr r3, .L_0200b74c
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	movs r1, #1
	bl ObjectMotion_SetActionVariant
	b .L_0200ba66
.L_0200b720:
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200b744
	ldr r3, .L_0200b74c
	movs r0, #133
	lsls r0, r0, #2
	adds r3, r3, r0
	ldr r0, [r3]
	bl Object_GetById
	ldr r3, [r0, #16]
	movs r1, #128
	lsls r1, r1, #13
	adds r3, r3, r1
	str r3, [r0, #16]
.L_0200b744:
	ldr r0, .L_0200b750
	bl Func_02002c24
	b .L_0200b780
.L_0200b74c:
	.4byte gPartyState
.L_0200b750:
	.4byte 0xffe80000
.L_0200b754:
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200b778
	ldr r3, .L_0200b7d0
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	ldr r3, [r0, #8]
	movs r1, #192
	lsls r1, r1, #14
	adds r3, r3, r1
	str r3, [r0, #8]
.L_0200b778:
	movs r0, #144
	lsls r0, r0, #16
	bl Func_02002c24
.L_0200b780:
	movs r1, #128
	lsls r1, r1, #19
	adds r1, #10
	movs r0, #255
	ldrh r2, [r1]
	lsls r0, r0, #8
	adds r0, #252
	adds r3, r0, #0
	ands r3, r2
	strh r3, [r1]
	ldr r2, .L_0200b7c4
	ldrh r3, [r1]
	orrs r3, r2
	strh r3, [r1]
	adds r1, #2
	ldrh r2, [r1]
	adds r3, r0, #0
	ands r3, r2
	strh r3, [r1]
	ldr r2, .L_0200b7c8
	ldrh r3, [r1]
	orrs r3, r2
	strh r3, [r1]
	adds r1, #2
	ldrh r3, [r1]
	ldr r2, .L_0200b7cc
	ands r0, r3
	strh r0, [r1]
	movs r0, #12
	ldrh r3, [r1]
	orrs r3, r2
	strh r3, [r1]
	b .L_0200b7d4
	.2byte 0x0000
.L_0200b7c4:
	.4byte 0x00000001
.L_0200b7c8:
	.4byte 0x00000002
.L_0200b7cc:
	.4byte 0x00000003
.L_0200b7d0:
	.4byte gPartyState
.L_0200b7d4:
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #2
	orrs r3, r2
	strb r3, [r0]
	movs r0, #0
	bl Func_02003c80
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #121
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200b804
	movs r1, #184
	movs r2, #140
	movs r0, #13
	lsls r1, r1, #16
	lsls r2, r2, #17
	bl Func_02003bf8
.L_0200b804:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #122
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200b876
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	subs r2, #172
	str r2, [r3]
	ldr r3, .L_0200b858
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	ldr r3, .L_0200b85c
	subs r2, #2
	strh r3, [r2]
	adds r2, #96
.L_0200b834:
	ldr r3, [r2, #8]
	cmp r3, #0
	blt .L_0200b834
	movs r1, #200
	ldr r0, .L_0200b860
	lsls r1, r1, #4
	bl Scheduler_AddOrUpdateCallback
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #32]
	movs r3, #188
	lsls r3, r3, #1
	adds r2, r2, r3
	movs r3, #128
	lsls r3, r3, #7
	b .L_0200b864
	.2byte 0x0000
.L_0200b858:
	.4byte 0x00001008
.L_0200b85c:
	.4byte 0x00003f42
.L_0200b860:
	.4byte Func_020024b0
.L_0200b864:
	str r3, [r2, #24]
	ldr r3, .L_0200b8ac
	str r3, [r2, #28]
	movs r3, #3
	strh r3, [r2, #40]
	movs r3, #192
	lsls r3, r3, #1
	strh r3, [r2, #42]
	b .L_0200ba66
.L_0200b876:
	ldr r3, .L_0200b8a4
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	ldr r3, .L_0200b8a8
	subs r2, #2
	strh r3, [r2]
	movs r1, #128
	lsls r1, r1, #19
	ldrh r2, [r1]
	movs r3, #253
	lsls r3, r3, #8
	adds r3, #255
	ands r3, r2
	strh r3, [r1]
	movs r0, #12
	bl Object_GetById
	movs r1, #6
	bl Object_SetPartAttribute
	b .L_0200ba66
.L_0200b8a4:
	.4byte 0x00001000
.L_0200b8a8:
	.4byte 0x00003f42
.L_0200b8ac:
	.4byte 0xffff8000
.L_0200b8b0:
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200b8d2
	ldr r3, .L_0200ba70
	movs r0, #133
	lsls r0, r0, #2
	adds r3, r3, r0
	ldr r0, [r3]
	bl Object_GetById
	ldr r1, .L_0200ba74
	ldr r3, [r0, #12]
	adds r3, r3, r1
	str r3, [r0, #12]
.L_0200b8d2:
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200b8f6
	ldr r3, .L_0200ba70
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	ldr r3, [r0, #16]
	movs r1, #128
	lsls r1, r1, #13
	adds r3, r3, r1
	str r3, [r0, #16]
.L_0200b8f6:
	ldr r0, .L_0200ba78
	bl Func_02002c24
.L_0200b8fc:
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #32]
	movs r3, #188
	lsls r3, r3, #1
	adds r2, r2, r3
	movs r3, #144
	lsls r3, r3, #19
	str r3, [r2, #8]
	movs r3, #200
	lsls r3, r3, #18
	str r3, [r2, #12]
	movs r0, #128
	movs r3, #208
	lsls r3, r3, #8
	lsls r0, r0, #4
	str r3, [r2, #16]
	str r3, [r2, #20]
	adds r0, #123
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200b994
	movs r5, #9
	movs r0, #29
	movs r1, #9
	movs r2, #62
	movs r3, #2
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl Func_02003b68
	movs r3, #62
	movs r2, #2
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #29
	movs r1, #9
	movs r2, #9
	movs r3, #9
	bl Func_02003b80
	movs r0, #29
	movs r1, #73
	movs r2, #62
	movs r3, #66
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl Func_02003b68
	movs r0, #52
	movs r1, #2
	movs r2, #29
	movs r3, #9
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl Func_02003b68
	movs r3, #29
	str r3, [sp, #0]
	movs r0, #52
	movs r1, #2
	movs r2, #9
	movs r3, #9
	str r5, [sp, #4]
	bl Func_02003b80
	movs r0, #52
	movs r1, #66
	movs r2, #29
	movs r3, #73
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl Func_02003b68
	b .L_0200b9a0
.L_0200b994:
	movs r0, #14
	bl Object_GetById
	movs r1, #6
	bl Object_SetPartAttribute
.L_0200b9a0:
	movs r0, #14
	movs r1, #1
	bl ObjectMotion_SetActionVariant
	bl Func_02003b50
	movs r0, #1
	bl WaitFrames
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200ba66
	ldr r6, .L_0200ba70
	movs r0, #133
	lsls r0, r0, #2
	adds r6, r6, r0
	ldr r0, [r6]
	bl Object_GetById
	adds r5, r0, #0
	adds r2, r5, #0
	adds r2, #34
	movs r3, #0
	strb r3, [r2]
	movs r0, #0
	ldr r2, [r5, #16]
	ldr r3, [r5, #12]
	ldr r1, [r5, #8]
	subs r2, r2, r3
	bl Map_GetTerrainHeight
	str r0, [r5, #20]
	str r0, [r5, #12]
	movs r1, #0
	ldr r0, [r6]
	bl Object_AttachWorkTargetToObject
	b .L_0200ba66
.L_0200b9f2:
	movs r5, #9
	movs r0, #29
	movs r1, #9
	movs r2, #62
	movs r3, #2
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl Func_02003b68
	movs r3, #62
	movs r2, #2
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #29
	movs r1, #9
	movs r2, #9
	movs r3, #9
	bl Func_02003b80
	movs r0, #29
	movs r1, #73
	movs r2, #62
	movs r3, #66
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl Func_02003b68
	movs r0, #52
	movs r1, #2
	movs r2, #29
	movs r3, #9
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl Func_02003b68
	movs r3, #29
	str r3, [sp, #0]
	movs r0, #52
	movs r1, #2
	movs r2, #9
	movs r3, #9
	str r5, [sp, #4]
	bl Func_02003b80
	movs r0, #52
	movs r1, #66
	movs r2, #29
	movs r3, #73
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl Func_02003b68
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	bl Func_02001f7c
.L_0200ba66:
	movs r0, #0
	add sp, #8
	pop {r3}
	mov r8, r3
	pop {r5, r6, pc}
.L_0200ba70:
	.4byte gPartyState
.L_0200ba74:
	.4byte 0xffe00000
.L_0200ba78:
	.4byte 0xffd00000
	.section .text.x0200ba7c,"ax",%progbits
	.global Func_02003a7c
	.thumb_func
Func_02003a7c:
	push {lr}
	ldr r3, .L_0200ba98
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_0200ba9c
	cmp r2, r3
	bne .L_0200ba94
	bl Func_02002b88
.L_0200ba94:
	movs r0, #0
	pop {pc}
.L_0200ba98:
	.4byte gPartyState
.L_0200ba9c:
	.4byte 0x00000054
	.section .rodata.x0200bce0,"a",%progbits
.L_0200bce0:
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
	.global Data_02003d1c
Data_02003d1c:
.L_0200bd1c:
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
.L_0200bd58:
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
	.global Data_02003d94
Data_02003d94:
	.4byte 0x000d000c
	.4byte 0x0010000f
	.4byte 0xffff0016
	.global Data_02003da0
Data_02003da0:
	.4byte .L_0200bce0
	.4byte .L_0200bd1c
	.4byte .L_0200bd58
.L_0200bdac:
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x00018000
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x0000002e
	.4byte Func_0200033c
	.4byte 0x00000011
	.global Data_02003dcc
Data_02003dcc:
	.4byte 0x00000027
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x0000000c
	.4byte 0x00000026
	.global Data_02003de0
Data_02003de0:
	.4byte 0x00000000
	.4byte 0x0000003c
	.4byte 0x00000027
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x0000000c
	.4byte 0x00000026
	.global Data_02003dfc
Data_02003dfc:
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xfffffae2
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffffae2
	.4byte 0x0000000d
	.4byte 0x00000020
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
	.global Data_02003e4c
Data_02003e4c:
	.4byte Data_02000000 + 0xe
	.4byte 0x0000ffff
	.global Data_02003e54
Data_02003e54:
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
	.global Data_02003e84
Data_02003e84:
	.4byte 0x00600100
	.4byte 0x01100090
	.4byte 0x00a00070
	.4byte 0x0002ffff
	.4byte 0x00600260
	.4byte 0x02700090
	.4byte 0x00a00070
	.4byte 0x0003ffff
	.4byte 0x00600360
	.4byte 0x03700090
	.4byte 0x00a00070
	.4byte 0x0004ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02003ec4
Data_02003ec4:
	.4byte 0xff400100
	.4byte 0x01100330
	.4byte 0x0340ff50
	.4byte 0x0001ffff
	.4byte 0xff400260
	.4byte 0x02700330
	.4byte 0x0340ff50
	.4byte 0x0002ffff
	.4byte 0xff400360
	.4byte 0x03700330
	.4byte 0x0340ff50
	.4byte 0x0003ffff
	.4byte 0x00b00270
	.4byte 0x028002c0
	.4byte 0x02d000c0
	.4byte 0x0005ffff
	.4byte 0x00a00310
	.4byte 0x032002b0
	.4byte 0x02c000b0
	.4byte 0x0006ffff
	.4byte 0xff380270
	.4byte 0x02800140
	.4byte 0x0150ff48
	.4byte 0x0007ffff
	.4byte 0xff280310
	.4byte 0x03200130
	.4byte 0x0140ff38
	.4byte 0x0008ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02003f44
Data_02003f44:
	.4byte 0xffa00090
	.4byte 0x00a00130
	.4byte 0x0140ffb0
	.4byte 0x0006ffff
	.4byte 0xffa000d0
	.4byte 0x00e00130
	.4byte 0x0140ffb0
	.4byte 0x0007ffff
	.4byte 0xffb00090
	.4byte 0x00a00130
	.4byte 0x0140ffc0
	.4byte 0x0008ffff
	.4byte 0xffb000d0
	.4byte 0x00e00130
	.4byte 0x0140ffc0
	.4byte 0x0009ffff
	.4byte 0x00a00090
	.4byte 0x00a000c0
	.4byte 0x00d000b0
	.4byte 0x000affff
	.4byte 0x00a000d0
	.4byte 0x00e000c0
	.4byte 0x00d000b0
	.4byte 0x000bffff
	.4byte 0x00a00090
	.4byte 0x00a000c0
	.4byte 0x00d000b0
	.4byte 0x000affff
	.4byte 0x00a000d0
	.4byte 0x00e000c0
	.4byte 0x00d000b0
	.4byte 0x000bffff
	.4byte 0xffa00210
	.4byte 0x02200130
	.4byte 0x0140ffb0
	.4byte 0x000cffff
	.4byte 0xffa00250
	.4byte 0x02600130
	.4byte 0x0140ffb0
	.4byte 0x000dffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02003ff4
Data_02003ff4:
	.4byte 0x00000052
	.4byte 0x10110002
	.4byte 0xffffffff
	.4byte 0x10201053
	.4byte 0xffffffff
	.4byte 0x10302053
	.4byte 0xffffffff
	.4byte 0x10403053
	.4byte 0xffffffff
	.4byte 0x00000053
	.4byte 0x10102052
	.4byte 0xffffffff
	.4byte 0x10203052
	.4byte 0xffffffff
	.4byte 0x10304052
	.4byte 0xffffffff
	.4byte 0x1040a055
	.4byte 0xffffffff
	.4byte 0x1050a053
	.4byte 0xffffffff
	.4byte 0x1060b053
	.4byte 0xffffffff
	.4byte 0x10705053
	.4byte 0xffffffff
	.4byte 0x10806053
	.4byte 0xffffffff
	.4byte 0x10901054
	.4byte 0xffffffff
	.4byte 0x00000054
	.4byte 0x1010c053
	.4byte 0xffffffff
	.4byte 0x10204054
	.4byte 0xffffffff
	.4byte 0x10305054
	.4byte 0xffffffff
	.4byte 0x10402054
	.4byte 0xffffffff
	.4byte 0x10503054
	.4byte 0xffffffff
	.4byte 0x10608054
	.4byte 0xffffffff
	.4byte 0x10709054
	.4byte 0xffffffff
	.4byte 0x10806054
	.4byte 0xffffffff
	.4byte 0x10907054
	.4byte 0xffffffff
	.4byte 0x10a08054
	.4byte 0x0000087a
	.4byte 0x10b09054
	.4byte 0x0000087a
	.4byte 0x10a0c054
	.4byte 0xffffffff
	.4byte 0x10b0d054
	.4byte 0xffffffff
	.4byte 0x10c0a054
	.4byte 0xffffffff
	.4byte 0x10d0b054
	.4byte 0xffffffff
	.4byte 0x10e01056
	.4byte 0xffffffff
	.4byte 0x000001ff
	.global Data_020040ec
Data_020040ec:
	.4byte 0xffff017e
	.4byte 0x00000007
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00024000
	.4byte 0xffff0145
	.4byte .L_0200bdac
	.4byte 0x03980000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00024000
	.4byte 0xffff0145
	.4byte .L_0200bdac
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x02f80000
	.4byte 0x00024000
	.4byte 0xffff0145
	.4byte .L_0200bdac
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x03280000
	.4byte 0x00024000
	.4byte 0xffff0145
	.4byte .L_0200bdac
	.4byte 0x02780000
	.4byte 0x00000000
	.4byte 0x03780000
	.4byte 0x00024000
	.4byte 0xffff0145
	.4byte .L_0200bdac
	.4byte 0x03680000
	.4byte 0x00000000
	.4byte 0x03580000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02004194
Data_02004194:
	.4byte 0xffff0179
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte gMapBlocks
	.4byte 0xffff0179
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte gMapBlocks
	.4byte 0xffff0179
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte Data_02028000
	.4byte 0xffff0179
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte Data_02028000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x00000006
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte Data_02024000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte Data_02024000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x00000008
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte Data_02024000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x00000009
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte Data_02024000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x0000000a
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte Data_02024000
	.4byte 0xffff0179
	.4byte 0x00000007
	.4byte 0x02180000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00020000
	.4byte 0xffff0179
	.4byte 0x00000007
	.4byte 0x02a80000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00028000
	.4byte 0xffff0179
	.4byte 0x00000007
	.4byte 0x02180000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00020000
	.4byte 0xffff0179
	.4byte 0x00000007
	.4byte 0x02880000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00028000
	.4byte 0xffff0179
	.4byte 0x00000007
	.4byte 0x02c80000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x0002c000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x0000000b
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte Data_02024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02004314
Data_02004314:
	.4byte 0xffff0179
	.4byte 0x00000007
	.4byte 0x02180000
	.4byte 0x00000000
	.4byte 0x03880000
	.4byte 0x0002c000
	.4byte 0xffff0179
	.4byte 0x00000007
	.4byte 0x02c80000
	.4byte 0x00000000
	.4byte 0x03880000
	.4byte 0x0102c000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x02080000
	.4byte 0x00000000
	.4byte 0x03680000
	.4byte 0x0002c000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x02180000
	.4byte 0x00000000
	.4byte 0x03680000
	.4byte 0x0102c000
	.4byte 0xffff017e
	.4byte 0x00000007
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00024000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x0002c000
	.4byte 0xffff017e
	.4byte 0x00000007
	.4byte 0x02180000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020043d4
Data_020043d4:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0xffff0014
	.4byte Func_02000548
	.4byte 0x80004e15
	.4byte Resource_Data012 + 0x280008
	.4byte Func_02000824
	.4byte 0x10004e15
	.4byte Resource_Data012 + 0x280008
	.4byte Func_02000830
	.4byte 0x00004e15
	.4byte Resource_Data012 + 0x280008
	.4byte Func_02001010
	.4byte 0x80004e15
	.4byte 0xffff0009
	.4byte Func_02000824
	.4byte 0x10004e15
	.4byte 0xffff0009
	.4byte Func_02000830
	.4byte 0x00004e15
	.4byte 0xffff0009
	.4byte Func_0200095c
	.4byte 0x80004e15
	.4byte 0xffff000a
	.4byte Func_02000824
	.4byte 0x10004e15
	.4byte 0xffff000a
	.4byte Func_02000830
	.4byte 0x00004e15
	.4byte 0xffff000a
	.4byte Func_0200095c
	.4byte 0x80004e15
	.4byte 0xffff000b
	.4byte Func_02000824
	.4byte 0x10004e15
	.4byte 0xffff000b
	.4byte Func_02000830
	.4byte 0x00004e15
	.4byte 0xffff000b
	.4byte Func_0200095c
	.4byte 0x80004e15
	.4byte 0xffff000c
	.4byte Func_02000824
	.4byte 0x10004e15
	.4byte 0xffff000c
	.4byte Func_02000830
	.4byte 0x00004e15
	.4byte 0xffff000c
	.4byte Func_0200095c
	.4byte 0x80004e15
	.4byte 0xffff000d
	.4byte Func_02000824
	.4byte 0x10004e15
	.4byte 0xffff000d
	.4byte Func_02000830
	.4byte 0x00004e15
	.4byte 0xffff000d
	.4byte Func_0200095c
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020044d0
Data_020044d0:
	.4byte 0x0000c602
	.4byte 0xffff0001
	.4byte Func_02000568
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000009
	.4byte 0x00000008
	.4byte 0xffff0000
	.4byte Func_02001ca4
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte Func_02001cc8
	.4byte 0x10008c15
	.4byte Resource_Data012 + 0x2f000c
	.4byte Func_02001ca4
	.4byte 0x00008c15
	.4byte Resource_Data012 + 0x2f000c
	.4byte Func_02001cc8
	.4byte 0x10008c15
	.4byte Resource_Data012 + 0x30000d
	.4byte Func_02001ca4
	.4byte 0x00008c15
	.4byte Resource_Data012 + 0x30000d
	.4byte Func_02001cc8
	.4byte 0x10008c15
	.4byte Resource_Data012 + 0x31000f
	.4byte Func_02001ca4
	.4byte 0x00008c15
	.4byte Resource_Data012 + 0x31000f
	.4byte Func_02001cc8
	.4byte 0x10008c15
	.4byte Resource_Data012 + 0x320010
	.4byte Func_02001ca4
	.4byte 0x00008c15
	.4byte Resource_Data012 + 0x320010
	.4byte Func_02001cc8
	.4byte 0x00001815
	.4byte Data_02000000 + 0xe
	.4byte Func_02001da8
	.4byte 0x10008c15
	.4byte Summon_BellMaidenTiles + 0x105a
	.4byte Func_02001ca4
	.4byte 0x00008c15
	.4byte Summon_BellMaidenTiles + 0x105a
	.4byte Func_02001cc8
	.4byte 0x00000006
	.4byte 0xffff00c8
	.4byte Func_02001560
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_0200459c
Data_0200459c:
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
	.4byte 0x00000031
	.4byte 0xffff0006
	.4byte 0x0000000e
	.4byte 0x00004602
	.4byte 0xffff0014
	.4byte Func_02000700
	.4byte 0x00008c15
	.4byte 0xffff000a
	.4byte Func_020005ec
	.4byte 0x00008c15
	.4byte 0xffff000b
	.4byte Func_02000620
	.4byte 0x00008c15
	.4byte Battle_MistyHillsBackdrop + 0x2291
	.4byte Func_020006d4
	.4byte 0x80004e15
	.4byte Battle_BlueRuinsBackdrop + 0x103c
	.4byte Func_02000824
	.4byte 0x10004e15
	.4byte Battle_BlueRuinsBackdrop + 0x103c
	.4byte Func_02000830
	.4byte 0x00004e15
	.4byte Battle_BlueRuinsBackdrop + 0x103c
	.4byte Func_020026b8
	.4byte 0x80004e15
	.4byte Battle_ArrowBeachBackdrop + 0xc36
	.4byte Func_02000824
	.4byte 0x10004e15
	.4byte Battle_ArrowBeachBackdrop + 0xc36
	.4byte Func_02000830
	.4byte 0x00004e15
	.4byte Battle_ArrowBeachBackdrop + 0xc36
	.4byte Func_02001f7c
	.4byte 0x00000006
	.4byte 0xffff00c8
	.4byte Func_02001560
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02004674
Data_02004674:
	.4byte 0x00000040
	.4byte 0x00000040
	.4byte 0x00000040
	.4byte 0x00000040
	.4byte 0x00000040
	.4byte 0x00000040
	.4byte 0x00000040
	.4byte 0x00000040
	.global Data_02004694
Data_02004694:
	.2byte 0x0106
	.global Data_02004696
Data_02004696:
	.2byte 0x0106
	.global Data_02004698
Data_02004698:
	.4byte 0xffffffff
