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
	bl Func_0200146c
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
	bl Func_0200145c
	ldr r2, .L_02008274
	mov r3, r8
	ands r3, r5
	lsls r3, r3, #2
	ldr r1, [r2, r3]
	adds r0, r6, #0
	mov r10, r3
	bl Func_02001464
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
	bl Func_0200145c
	ldr r1, [r7, #28]
	adds r0, r6, #0
	bl Func_02001464
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
	.4byte Data_02001650
.L_02008278:
	.4byte Func_02000080
.L_0200827c:
	.4byte 0xffff0000
	.section .text.x020082a4,"ax",%progbits
	.global Func_020002a4
	.thumb_func
Func_020002a4:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	mov r8, r2
	mov r3, r8
	movs r2, #0
	adds r7, r0, #0
	mov r12, r1
	mov lr, r2
	cmp r3, #0
	beq .L_02008324
.L_020082ba:
	mov r2, r12
	ldrh r3, [r2]
	movs r1, #31
	adds r0, r1, #0
	ands r0, r3
	lsls r3, r3, #16
	lsrs r5, r3, #21
	lsrs r6, r3, #26
	ldrh r3, [r7]
	ldr r2, .L_020082f4
	ands r1, r3
	lsls r3, r3, #16
	lsrs r4, r3, #21
	lsrs r3, r3, #26
	ands r5, r2
	ands r6, r2
	ands r4, r2
	ands r3, r2
	cmp r1, r0
	bge .L_020082e6
	adds r1, #1
	b .L_020082ec
.L_020082e6:
	cmp r1, r0
	ble .L_020082ec
	subs r1, #1
.L_020082ec:
	cmp r4, r5
	bge .L_020082f8
	adds r4, #1
	b .L_020082fe
.L_020082f4:
	.4byte 0x0000001f
.L_020082f8:
	cmp r4, r5
	ble .L_020082fe
	subs r4, #1
.L_020082fe:
	cmp r3, r6
	bge .L_02008306
	adds r3, #1
	b .L_0200830c
.L_02008306:
	cmp r3, r6
	ble .L_0200830c
	subs r3, #1
.L_0200830c:
	lsls r2, r4, #5
	lsls r3, r3, #10
	orrs r3, r2
	orrs r3, r1
	strh r3, [r7]
	movs r3, #1
	movs r2, #2
	add lr, r3
	add r12, r2
	adds r7, #2
	cmp lr, r8
	bne .L_020082ba
.L_02008324:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x0200832c,"ax",%progbits
	.global Func_0200032c
	.thumb_func
Func_0200032c:
	push {r5, r6, r7, lr}
	adds r5, r0, #0
	mov r12, r1
	adds r6, r2, #0
	movs r7, #0
	cmp r1, #0
	beq .L_02008390
.L_0200833a:
	ldrh r2, [r5]
	ldr r1, .L_02008368
	movs r3, #31
	ands r3, r2
	lsls r2, r2, #16
	adds r0, r3, r6
	lsrs r3, r2, #21
	lsrs r2, r2, #26
	ands r3, r1
	ands r2, r1
	adds r4, r3, r6
	adds r2, r2, r6
	cmp r0, #31
	ble .L_02008358
	movs r0, #31
.L_02008358:
	cmp r0, #0
	bge .L_0200835e
	movs r0, #0
.L_0200835e:
	cmp r4, #31
	ble .L_0200836c
	movs r4, #31
	b .L_0200836c
	.2byte 0x0000
.L_02008368:
	.4byte 0x0000001f
.L_0200836c:
	cmp r4, #0
	bge .L_02008372
	movs r4, #0
.L_02008372:
	cmp r2, #31
	ble .L_02008378
	movs r2, #31
.L_02008378:
	cmp r2, #0
	bge .L_0200837e
	movs r2, #0
.L_0200837e:
	lsls r3, r2, #10
	lsls r2, r4, #5
	orrs r3, r2
	orrs r3, r0
	adds r7, #1
	strh r3, [r5]
	adds r5, #2
	cmp r7, r12
	bne .L_0200833a
.L_02008390:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x02008394,"ax",%progbits
	.global Func_02000394
	.thumb_func
Func_02000394:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r3, #160
	lsls r3, r3, #19
	adds r3, #194
	mov r8, r3
	ldr r3, .L_020083ec
	movs r7, #160
	ldr r0, [r3]
	lsls r7, r7, #19
	lsls r0, r0, #11
	lsrs r0, r0, #1
	bl Math_Sine
	lsls r0, r0, #1
	adds r7, #130
	asrs r5, r0, #16
	movs r6, #0
.L_020083ba:
	mov r3, r8
	ldrh r2, [r3]
	ldr r1, .L_020083e8
	movs r3, #31
	ands r3, r2
	lsls r2, r2, #16
	adds r0, r3, r5
	lsrs r3, r2, #21
	lsrs r2, r2, #26
	ands r3, r1
	ands r2, r1
	adds r4, r3, r5
	adds r2, r2, r5
	cmp r0, #31
	ble .L_020083da
	movs r0, #31
.L_020083da:
	cmp r0, #0
	bge .L_020083e0
	movs r0, #0
.L_020083e0:
	cmp r4, #31
	ble .L_020083f0
	movs r4, #31
	b .L_020083f0
.L_020083e8:
	.4byte 0x0000001f
.L_020083ec:
	.4byte Data_0300122c
.L_020083f0:
	cmp r4, #0
	bge .L_020083f6
	movs r4, #0
.L_020083f6:
	cmp r2, #31
	ble .L_020083fc
	movs r2, #31
.L_020083fc:
	cmp r2, #0
	bge .L_02008402
	movs r2, #0
.L_02008402:
	lsls r3, r2, #10
	lsls r2, r4, #5
	orrs r3, r2
	orrs r3, r0
	strh r3, [r7]
	adds r6, #1
	movs r3, #2
	add r8, r3
	adds r7, #2
	cmp r6, #15
	bne .L_020083ba
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x02008420,"ax",%progbits
	.global Func_02000420
	.thumb_func
Func_02000420:
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
	bge .L_02008450
	adds r3, #15
.L_02008450:
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
	.section .text.x02008478,"ax",%progbits
	.global Func_02000478
	.thumb_func
Func_02000478:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	mov r11, r0
	movs r0, #8
	sub sp, #68
	mov r9, r1
	bl Object_GetById
	ldr r3, .L_02008558
	add r7, sp, #28
	str r3, [r7, #36]
	movs r3, #204
	lsls r3, r3, #8
	adds r3, #204
	str r3, [r7, #8]
	str r3, [r7, #12]
	ldr r3, [r0, #80]
	movs r2, #0
	ldrb r3, [r3, #9]
	mov r8, r0
	lsls r3, r3, #28
	lsrs r3, r3, #30
	str r3, [r7]
	mov r10, r2
.L_020084b2:
	movs r0, #128
	mov r1, r9
	lsls r0, r0, #9
	bl __divsi3
	mov r5, r10
	muls r5, r0
	adds r0, r5, #0
	bl Math_Cosine
	mov r3, r11
	muls r3, r0
	add r6, sp, #16
	cmp r3, #0
	bge .L_020084d2
	adds r3, #255
.L_020084d2:
	asrs r3, r3, #8
	str r3, [r6]
	adds r0, r5, #0
	movs r3, #0
	str r3, [r6, #4]
	bl Math_Sine
	mov r3, r11
	muls r3, r0
	cmp r3, #0
	bge .L_020084ea
	adds r3, #255
.L_020084ea:
	asrs r3, r3, #8
	str r3, [r6, #8]
	ldr r3, [r6]
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
	ldr r3, .L_0200855c
	adds r2, r2, r3
	str r2, [r6]
	bl Random16Far
	lsls r3, r0, #1
	ldr r5, [r6, #8]
	adds r3, r3, r0
	ldr r2, .L_02008560
	lsls r3, r3, #13
	lsrs r3, r3, #16
	adds r5, r5, r3
	adds r5, r5, r2
	str r5, [r6, #8]
	ldr r4, [r6, #4]
	mov r3, r8
	ldr r2, [r3, #16]
	ldr r0, [r3, #8]
	ldr r1, [r3, #12]
	ldr r3, [r6]
	str r4, [sp, #0]
	movs r4, #133
	lsls r4, r4, #17
	adds r4, #1
	str r5, [sp, #4]
	str r4, [sp, #8]
	str r7, [sp, #12]
	bl Func_020000b8
	movs r2, #1
	add r10, r2
	cmp r10, r9
	bls .L_020084b2
	add sp, #68
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008558:
	.4byte Func_02000420
.L_0200855c:
	.4byte 0xffffa000
.L_02008560:
	.4byte 0xffffd000
	.section .text.x02008564,"ax",%progbits
	.global Func_02000564
	.thumb_func
Func_02000564:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r0, #128
	lsls r0, r0, #2
	bl Runtime_BumpAllocateAlternatePool
	mov r9, r0
	bl Func_02001454
	ldr r5, .L_020085bc
	movs r1, #147
	lsls r1, r1, #1
	movs r2, #128
	adds r1, #255
	lsls r2, r2, #2
	adds r3, r5, r1
	adds r2, #38
	ldrb r0, [r3]
	adds r3, r5, r2
	ldrb r1, [r3]
	bl Func_02001484
	movs r1, #0
	movs r2, #0
	movs r0, #4
	bl Func_020014b4
	movs r1, #241
	lsls r1, r1, #1
	adds r3, r5, r1
	movs r2, #0
	ldrsh r5, [r3, r2]
	cmp r5, #10
	bne .L_020085c4
	ldr r0, .L_020085c0
	movs r1, #72
	bl Func_02001534
	bl .L_020093d6
	.2byte 0x0000
.L_020085bc:
	.4byte gPartyState
.L_020085c0:
	.4byte 0x00000003
.L_020085c4:
	cmp r5, #2
	beq .L_020085ca
	b .L_020087fc
.L_020085ca:
	movs r0, #128
	movs r1, #1
	movs r2, #128
	lsls r2, r2, #16
	movs r3, #0
	lsls r0, r0, #17
	negs r1, r1
	bl Motion_CamBounds
	movs r0, #128
	movs r1, #128
	lsls r1, r1, #6
	lsls r0, r0, #9
	bl Func_0200151c
	movs r0, #8
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #8
	movs r1, #12
	bl Object_SetModeById
	movs r0, #8
	movs r1, #8
	bl Object_SetActionById
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r1, #214
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #128
	lsls r1, r1, #19
	movs r2, #0
	str r2, [r3]
	ldrh r2, [r1]
	movs r3, #253
	lsls r3, r3, #8
	adds r3, #255
	ands r3, r2
	strh r3, [r1]
	movs r0, #255
	adds r1, #10
	ldrh r2, [r1]
	lsls r0, r0, #8
	adds r0, #252
	adds r3, r0, #0
	ands r3, r2
	strh r3, [r1]
	ldr r2, .L_0200866c
	ldrh r3, [r1]
	movs r5, #0
	orrs r3, r2
	strh r3, [r1]
	adds r1, #4
	ldrh r3, [r1]
	ldr r2, .L_02008670
	ands r0, r3
	strh r0, [r1]
	ldrh r3, [r1]
	orrs r3, r2
	strh r3, [r1]
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	movs r0, #128
	movs r1, #1
	movs r2, #159
	lsls r0, r0, #17
	negs r1, r1
	lsls r2, r2, #17
	movs r3, #1
	bl Motion_CamBounds
	b .L_02008674
	.2byte 0x0000
.L_0200866c:
	.4byte 0x00000003
.L_02008670:
	.4byte 0x00000002
.L_02008674:
	bl Func_0200152c
	movs r0, #158
	movs r1, #1
	movs r2, #159
	lsls r0, r0, #17
	negs r1, r1
	lsls r2, r2, #17
	movs r3, #1
	bl Motion_CamBounds
	bl Func_0200152c
.L_0200868e:
	cmp r5, #3
	bne .L_020086b4
	movs r0, #200
	movs r1, #141
	lsls r0, r0, #5
	lsls r1, r1, #2
	adds r0, #153
	adds r1, #255
	bl Func_0200151c
	movs r0, #158
	movs r1, #1
	movs r2, #150
	lsls r0, r0, #17
	negs r1, r1
	lsls r2, r2, #17
	movs r3, #1
	bl Motion_CamBounds
.L_020086b4:
	movs r0, #8
	bl Object_GetById
	ldr r2, .L_02008728
	ldr r3, [r0, #16]
	adds r5, #1
	adds r3, r3, r2
	str r3, [r0, #16]
	movs r0, #30
	bl WaitFrames
	cmp r5, #10
	bne .L_0200868e
	movs r0, #8
	movs r1, #0
	bl Object_SetActionById
	movs r1, #160
	lsls r1, r1, #19
	ldr r3, .L_0200872c
	mov r0, r9
	adds r1, #8
	movs r2, #14
	mov lr, r3
	.2byte 0xf800
	movs r6, #0
.L_020086e8:
	movs r7, #160
	lsls r7, r7, #19
	adds r7, #14
	mov r12, r7
	movs r5, #0
.L_020086f2:
	mov r1, r12
	ldrh r3, [r1]
	ldr r2, .L_02008724
	movs r4, #31
	ands r4, r3
	lsls r3, r3, #16
	lsrs r0, r3, #21
	lsrs r1, r3, #26
	ands r0, r2
	ands r1, r2
	adds r4, r4, r6
	adds r3, r6, #0
	cmp r6, #0
	bge .L_02008710
	adds r3, #15
.L_02008710:
	asrs r3, r3, #4
	adds r0, r0, r3
	subs r1, r1, r6
	cmp r4, #31
	ble .L_0200871c
	movs r4, #31
.L_0200871c:
	cmp r0, #31
	ble .L_02008730
	movs r0, #31
	b .L_02008730
.L_02008724:
	.4byte 0x0000001f
.L_02008728:
	.4byte 0xffff0000
.L_0200872c:
	.4byte IwramCopyWords
.L_02008730:
	cmp r1, #7
	bgt .L_02008736
	movs r1, #8
.L_02008736:
	cmp r1, #31
	ble .L_0200873c
	movs r1, #31
.L_0200873c:
	lsls r2, r0, #5
	lsls r3, r1, #10
	orrs r3, r2
	orrs r3, r4
	movs r2, #2
	adds r5, #1
	strh r3, [r7]
	add r12, r2
	adds r7, #2
	cmp r5, #8
	bne .L_020086f2
	movs r0, #4
	adds r6, #1
	bl WaitFrames
	cmp r6, #10
	bne .L_020086e8
	movs r1, #129
	movs r2, #0
	lsls r1, r1, #1
	movs r0, #8
	bl Func_0200150c
	movs r0, #30
	bl WaitFrames
	ldr r0, .L_020087f0
	bl Func_020014e4
	movs r0, #128
	lsls r0, r0, #7
	movs r1, #0
	adds r0, #8
	bl Func_020014f4
	movs r0, #30
	bl WaitFrames
	movs r0, #8
	movs r1, #2
	bl Motion_SetVarCbAndRefresh
	movs r0, #128
	lsls r0, r0, #7
	adds r0, #8
	movs r1, #0
	bl Func_020014f4
	movs r0, #8
	movs r1, #32
	bl Object_SetActionById
	movs r5, #0
.L_020087a6:
	cmp r5, #16
	bne .L_020087c8
	movs r0, #158
	movs r1, #1
	movs r2, #128
	lsls r0, r0, #17
	negs r1, r1
	lsls r2, r2, #16
	movs r3, #1
	bl Motion_CamBounds
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #10
	lsls r1, r1, #7
	bl Func_0200151c
.L_020087c8:
	cmp r5, #19
	bne .L_020087d0
	bl Event_ClearStatus1c6
.L_020087d0:
	movs r0, #8
	bl Object_GetById
	ldr r1, .L_020087f4
	ldr r3, [r0, #16]
	adds r5, #1
	adds r3, r3, r1
	str r3, [r0, #16]
	movs r0, #8
	bl WaitFrames
	cmp r5, #20
	bne .L_020087a6
	ldr r0, .L_020087f8
	bl .L_02009370
.L_020087f0:
	.4byte 0x00002f3e
.L_020087f4:
	.4byte 0xfffe0000
.L_020087f8:
	.4byte 0x00000003
.L_020087fc:
	cmp r5, #3
	beq .L_02008804
	bl .L_02009378
.L_02008804:
	movs r2, #192
	lsls r2, r2, #18
	mov r8, r2
	mov r3, r8
	adds r3, #128
	ldr r3, [r3]
	movs r1, #160
	movs r2, #224
	mov r10, r3
	mov r0, r9
	ldr r3, .L_02008930
	lsls r1, r1, #19
	lsls r2, r2, #1
	mov lr, r3
	.2byte 0xf800
	movs r0, #128
	movs r1, #1
	movs r2, #144
	lsls r2, r2, #16
	movs r3, #0
	lsls r0, r0, #17
	negs r1, r1
	bl Motion_CamBounds
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #9
	lsls r1, r1, #6
	bl Func_0200151c
	movs r1, #14
	movs r0, #8
	bl Object_SetModeById
	movs r0, #8
	bl Object_GetById
	movs r7, #0
	strh r7, [r0, #6]
	movs r0, #8
	bl Object_GetById
	movs r3, #248
	lsls r3, r3, #16
	str r3, [r0, #8]
	movs r0, #8
	bl Object_GetById
	movs r3, #152
	lsls r3, r3, #16
	str r3, [r0, #16]
	ldr r0, .L_02008934
	ldr r1, .L_02008938
	ldrh r3, [r1]
	adds r4, r3, #0
	strh r1, [r1]
	ldrh r2, [r0]
	cmp r2, #31
	bgt .L_0200889e
	lsls r3, r2, #1
	adds r3, r3, r2
	adds r2, #1
	lsls r3, r3, #2
	strh r2, [r0]
	movs r2, #192
	adds r3, r3, r0
	lsls r2, r2, #1
	adds r3, #4
	adds r2, #255
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #80
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #10
	str r2, [r3]
.L_0200889e:
	strh r4, [r1]
	ldrh r3, [r1]
	adds r4, r3, #0
	strh r1, [r1]
	ldrh r2, [r0]
	cmp r2, #31
	bgt .L_020088ce
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	adds r2, #1
	adds r3, r3, r0
	strh r2, [r0]
	movs r2, #128
	adds r3, #4
	lsls r2, r2, #5
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #10
	str r2, [r3]
.L_020088ce:
	strh r4, [r1]
	movs r6, #128
	lsls r6, r6, #19
	movs r5, #253
	ldrh r2, [r6]
	lsls r5, r5, #8
	adds r5, #255
	adds r3, r5, #0
	ands r3, r2
	strh r3, [r6]
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
	ldr r2, .L_02008928
	ldrh r3, [r1]
	orrs r3, r2
	strh r3, [r1]
	adds r1, #4
	ldrh r3, [r1]
	ldr r2, .L_0200892c
	ands r0, r3
	strh r0, [r1]
	movs r0, #1
	ldrh r3, [r1]
	orrs r3, r2
	strh r3, [r1]
	bl WaitFrames
	mov r1, r8
	ldr r3, [r1, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	str r7, [r3]
	ldrh r3, [r6]
	ands r5, r3
	strh r5, [r6]
	b .L_0200893c
.L_02008928:
	.4byte 0x00000003
.L_0200892c:
	.4byte 0x00000002
.L_02008930:
	.4byte IwramCopyWords
.L_02008934:
	.4byte gIoWriteQueue
.L_02008938:
	.4byte 0x04000208
.L_0200893c:
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r0, r0, #11
	lsls r1, r1, #11
	lsls r2, r2, #9
	bl Func_0200147c
	movs r5, #0
.L_02008956:
	ldr r2, .L_020089bc
	movs r0, #5
	ldrh r3, [r2, #10]
	adds r5, #1
	adds r3, #8
	strh r3, [r2, #10]
	bl Battle_WaitMode0
	cmp r5, #30
	bne .L_02008956
	movs r1, #15
	movs r0, #8
	bl Object_SetModeById
	movs r0, #60
	bl WaitFrames
	movs r0, #212
	bl Func_02001594
	movs r1, #128
	lsls r1, r1, #19
	adds r1, #10
	ldrh r2, [r1]
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #252
	ands r3, r2
	strh r3, [r1]
	ldr r2, .L_020089b4
	ldrh r3, [r1]
	orrs r3, r2
	strh r3, [r1]
	movs r2, #128
	ldr r3, .L_020089b8
	ldr r1, .L_020089c0
	lsls r2, r2, #19
	adds r2, #80
	strh r3, [r2]
	ldr r0, .L_020089c4
	ldrh r3, [r0]
	adds r4, r3, #0
	strh r0, [r0]
	ldrh r2, [r1]
	cmp r2, #31
	bgt .L_020089ec
	b .L_020089c8
.L_020089b4:
	.4byte 0x00000001
.L_020089b8:
	.4byte 0x00003f42
.L_020089bc:
	.4byte Data_03001120
.L_020089c0:
	.4byte gIoWriteQueue
.L_020089c4:
	.4byte 0x04000208
.L_020089c8:
	lsls r3, r2, #1
	adds r3, r3, r2
	adds r2, #1
	lsls r3, r3, #2
	strh r2, [r1]
	movs r2, #128
	adds r3, r3, r1
	lsls r2, r2, #5
	adds r3, #4
	adds r2, #2
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #10
	str r2, [r3]
.L_020089ec:
	strh r4, [r0]
	movs r6, #0
.L_020089f0:
	cmp r6, #16
	bne .L_02008a02
	movs r0, #140
	bl Func_02001594
	movs r0, #8
	movs r1, #16
	bl Object_SetModeById
.L_02008a02:
	cmp r6, #51
	bne .L_02008a2e
	movs r0, #107
	bl Func_02001594
	movs r1, #160
	movs r2, #224
	lsls r1, r1, #19
	lsls r2, r2, #1
	ldr r5, .L_02008de0
	mov r0, r9
	mov lr, r5
	.2byte 0xf800
	movs r0, #248
	movs r1, #130
	lsls r0, r0, #4
	lsls r1, r1, #5
	add r0, r10
	add r1, r10
	movs r2, #96
	mov lr, r5
	.2byte 0xf800
.L_02008a2e:
	adds r2, r6, #0
	subs r2, #16
	cmp r2, #12
	bhi .L_02008a74
	lsrs r3, r2, #31
	adds r3, r2, r3
	ldr r0, .L_02008de4
	asrs r3, r3, #1
	adds r1, r3, #2
	ldr r4, .L_02008de8
	ldrh r3, [r4]
	adds r5, r3, #0
	strh r4, [r4]
	ldrh r2, [r0]
	cmp r2, #31
	bgt .L_02008a72
	lsls r3, r2, #1
	adds r3, r3, r2
	adds r2, #1
	lsls r3, r3, #2
	strh r2, [r0]
	movs r2, #128
	lsls r2, r2, #5
	adds r3, r3, r0
	adds r3, #4
	orrs r1, r2
	stmia r3!, {r1}
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #10
	str r2, [r3]
.L_02008a72:
	strh r5, [r4]
.L_02008a74:
	adds r2, r6, #0
	subs r2, #34
	cmp r2, #16
	bhi .L_02008abc
	lsrs r3, r2, #31
	adds r3, r2, r3
	asrs r3, r3, #1
	ldr r0, .L_02008de4
	adds r1, r3, #0
	adds r1, #8
	ldr r4, .L_02008de8
	ldrh r3, [r4]
	adds r5, r3, #0
	strh r4, [r4]
	ldrh r2, [r0]
	cmp r2, #31
	bgt .L_02008aba
	lsls r3, r2, #1
	adds r3, r3, r2
	adds r2, #1
	lsls r3, r3, #2
	strh r2, [r0]
	movs r2, #128
	lsls r2, r2, #5
	adds r3, r3, r0
	adds r3, #4
	orrs r1, r2
	stmia r3!, {r1}
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #10
	str r2, [r3]
.L_02008aba:
	strh r5, [r4]
.L_02008abc:
	adds r3, r6, #0
	subs r3, #20
	cmp r3, #31
	bhi .L_02008ad6
	movs r0, #160
	movs r1, #160
	lsls r0, r0, #19
	lsls r1, r1, #19
	adds r0, #128
	adds r1, #192
	movs r2, #16
	bl Func_020002a4
.L_02008ad6:
	cmp r6, #49
	ble .L_02008ae6
	movs r0, #160
	lsls r0, r0, #19
	movs r1, #224
	movs r2, #1
	bl Func_0200032c
.L_02008ae6:
	movs r0, #4
	adds r6, #1
	bl WaitFrames
	cmp r6, #84
	beq .L_02008af4
	b .L_020089f0
.L_02008af4:
	movs r0, #8
	movs r1, #3
	bl ObjectMotion_SetActionVariant
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	negs r1, r1
	adds r2, #102
	negs r0, r0
	bl Func_0200147c
	movs r0, #195
	lsls r0, r0, #1
	bl Func_02001594
	movs r0, #128
	movs r1, #1
	movs r2, #159
	lsls r0, r0, #17
	negs r1, r1
	lsls r2, r2, #17
	movs r3, #0
	bl Motion_CamBounds
	ldr r1, .L_02008de4
	ldr r4, .L_02008de8
	ldrh r3, [r4]
	adds r5, r3, #0
	strh r4, [r4]
	ldrh r3, [r1]
	cmp r3, #31
	bgt .L_02008b62
	lsls r2, r3, #1
	adds r2, r2, r3
	adds r3, #1
	strh r3, [r1]
	movs r0, #128
	lsls r0, r0, #19
	adds r0, #10
	lsls r2, r2, #2
	adds r2, r2, r1
	ldrh r1, [r0]
	movs r3, #4
	negs r3, r3
	ands r3, r1
	movs r1, #3
	adds r2, #4
	orrs r3, r1
	stmia r2!, {r3}
	movs r3, #128
	stmia r2!, {r0}
	lsls r3, r3, #10
	str r3, [r2]
.L_02008b62:
	strh r5, [r4]
	movs r0, #212
	bl Func_02001594
	movs r0, #1
	bl WaitFrames
	movs r1, #144
	ldr r0, .L_02008dec
	lsls r1, r1, #3
	bl Scheduler_AddOrUpdateCallback
	movs r6, #0
.L_02008b7c:
	movs r0, #160
	lsls r0, r0, #19
	mov r1, r9
	movs r2, #224
	bl Func_020002a4
	adds r6, #1
	movs r0, #4
	bl WaitFrames
	cmp r6, #32
	bne .L_02008b7c
	movs r0, #8
	movs r1, #1
	bl ObjectMotion_SetActionVariant
	movs r0, #8
	movs r1, #1
	bl Object_SetModeById
	movs r0, #128
	movs r1, #1
	movs r2, #128
	movs r3, #1
	lsls r2, r2, #16
	lsls r0, r0, #17
	negs r1, r1
	bl Motion_CamBounds
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #9
	lsls r1, r1, #6
	bl Func_0200151c
	bl Func_0200152c
	movs r1, #152
	movs r2, #152
	lsls r1, r1, #7
	lsls r2, r2, #6
	movs r0, #8
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r2, #128
	movs r0, #8
	movs r1, #248
	bl ObjectMotion_SetPositionAndReset
	movs r1, #2
	movs r0, #8
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl Battle_WaitMode0
	movs r0, #78
	bl Func_02001594
	ldr r5, .L_02008df0
	movs r6, #0
	adds r0, r5, #0
	bl Func_020014e4
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #8
	movs r1, #0
	movs r2, #5
	bl Func_020014ec
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #8
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #8
	movs r1, #14
	bl Object_SetModeById
	movs r0, #128
	lsls r0, r0, #6
	movs r2, #5
	adds r0, #8
	movs r1, #0
	bl Func_020014ec
	movs r0, #8
	movs r1, #15
	bl Object_SetModeById
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #8
	movs r1, #0
	movs r2, #150
	bl Func_020014ec
	movs r1, #2
	movs r2, #30
	adds r1, #255
	movs r0, #8
	bl Func_0200150c
	movs r0, #8
	movs r1, #1
	bl Object_SetModeById
	movs r1, #192
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #8
	bl ObjectMotion_ArmCallback
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #8
	bl ObjectMotion_ArmCallback
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #192
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #8
	bl ObjectMotion_ArmCallback
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #8
	bl ObjectMotion_ArmCallback
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #8
	bl ObjectMotion_ArmCallback
	movs r0, #50
	bl Battle_WaitMode0
	movs r1, #15
	movs r0, #8
	bl Object_SetModeById
	movs r0, #50
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	movs r2, #5
	movs r1, #0
	adds r0, #8
	bl Func_020014ec
	movs r0, #50
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #8
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl Battle_WaitMode0
	movs r0, #9
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #9
	movs r1, #216
	movs r2, #96
	bl ObjectMotion_SetPositionAndReset
	movs r1, #128
	movs r0, #8
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #8
	bl Func_0200150c
	adds r0, r5, #4
	bl Func_020014e4
	movs r0, #128
	lsls r0, r0, #6
	movs r1, #0
	movs r2, #5
	adds r0, #8
	bl Func_020014ec
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #210
	bl Func_02001594
	movs r2, #5
	movs r0, #9
	movs r1, #0
	bl Func_020014ec
	movs r0, #8
	movs r1, #1
	bl Object_SetModeById
	movs r1, #160
	movs r0, #8
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #8
	bl Func_0200150c
	movs r0, #8
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	movs r1, #128
	strb r3, [r0]
	lsls r1, r1, #1
	movs r0, #8
	movs r2, #136
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r1, #153
	movs r2, #152
	lsls r1, r1, #8
	lsls r2, r2, #7
	adds r2, #204
	adds r1, #153
	movs r0, #8
	bl ObjectMotion_SetSpeedParameters
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #8
	movs r1, #1
	bl Object_SetModeById
	movs r0, #128
	lsls r0, r0, #6
	movs r2, #5
	adds r0, #8
	movs r1, #0
	bl Func_020014ec
	movs r0, #9
	movs r1, #4
	bl Object_SetModeById
	movs r2, #5
	movs r0, #9
	movs r1, #0
	bl Func_020014ec
	movs r1, #2
	movs r0, #8
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	movs r2, #5
	adds r0, #8
	movs r1, #0
	bl Func_020014ec
	movs r0, #9
	movs r1, #5
	b .L_02008df4
	.2byte 0x0000
.L_02008de0:
	.4byte IwramCopyWords
.L_02008de4:
	.4byte gIoWriteQueue
.L_02008de8:
	.4byte 0x04000208
.L_02008dec:
	.4byte Func_02000394
.L_02008df0:
	.4byte 0x00002f40
.L_02008df4:
	bl Object_SetModeById
	movs r2, #5
	movs r0, #9
	movs r1, #0
	bl Func_020014ec
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #8
	bl Func_02001514
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	movs r2, #5
	adds r0, #8
	movs r1, #0
	bl Func_020014ec
	movs r0, #9
	movs r1, #6
	bl Object_SetModeById
	movs r0, #9
	movs r1, #0
	movs r2, #5
	bl Func_020014ec
	movs r1, #8
	adds r1, #255
	movs r2, #30
	movs r0, #8
	adds r5, #12
	bl Func_0200150c
	adds r0, r5, #0
	bl Func_020014e4
	movs r0, #128
	lsls r0, r0, #6
	movs r2, #5
	adds r0, #8
	movs r1, #0
	bl Func_020014ec
	movs r1, #13
	movs r0, #8
	bl Object_SetModeById
	bl Func_02001574
	movs r0, #8
	bl Object_GetById
	movs r1, #2
	bl Func_0200158c
	movs r0, #178
	bl Func_02001594
	movs r1, #128
	lsls r1, r1, #7
	adds r1, #132
	movs r0, #8
	bl Func_02001564
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #9
	movs r1, #3
	bl Object_SetModeById
	movs r1, #200
	movs r2, #192
	lsls r1, r1, #5
	lsls r2, r2, #4
	movs r0, #9
	adds r1, #153
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
.L_02008e9e:
	movs r3, #1
	ands r3, r6
	cmp r3, #0
	beq .L_02008eb2
	movs r0, #9
	movs r1, #216
	movs r2, #96
	bl ObjectMotion_SetPositionAndReset
	b .L_02008ecc
.L_02008eb2:
	movs r0, #9
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	strb r3, [r0]
	movs r1, #208
	movs r0, #9
	movs r2, #88
	bl ObjectMotion_ResetAndSetPositionInMode2
.L_02008ecc:
	movs r5, #0
.L_02008ece:
	movs r1, #128
	lsls r1, r1, #1
	movs r0, #9
	bl Func_020014d4
	movs r0, #3
	bl Battle_WaitMode0
	movs r0, #9
	movs r1, #0
	bl Func_020014d4
	adds r5, #1
	movs r0, #3
	bl Battle_WaitMode0
	cmp r5, #10
	bne .L_02008ece
	adds r6, #1
	cmp r6, #4
	bne .L_02008e9e
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #8
	bl Func_0200150c
	movs r0, #128
	lsls r0, r0, #6
	movs r2, #5
	movs r1, #0
	adds r0, #8
	bl Func_020014ec
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #8
	bl Object_GetById
	movs r1, #0
	bl Func_0200158c
	bl Func_02001584
	bl Func_0200157c
	movs r1, #6
	movs r0, #9
	bl Object_SetModeById
	bl Func_02001574
	movs r0, #9
	bl Object_GetById
	movs r1, #2
	bl Func_0200158c
	movs r0, #30
	bl WaitFrames
	movs r0, #212
	bl Func_02001594
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r0, r0, #13
	lsls r1, r1, #13
	lsls r2, r2, #9
	bl Func_0200147c
	movs r5, #0
.L_02008f62:
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #1
	adds r0, #5
	bl Func_0200153c
	movs r0, #1
	bl Func_02001544
	movs r0, #2
	bl Battle_WaitMode0
	movs r0, #128
	movs r1, #1
	lsls r0, r0, #9
	bl Func_0200153c
	movs r0, #1
	bl Func_02001544
	adds r5, #1
	movs r0, #2
	bl Battle_WaitMode0
	cmp r5, #4
	bne .L_02008f62
	movs r2, #0
	movs r0, #0
	movs r1, #0
	bl Func_0200147c
	movs r1, #2
	movs r0, #8
	bl Motion_SetVarCbAndRefresh
	movs r0, #1
	bl Field_BeginPaletteTransition
	movs r1, #136
	lsls r1, r1, #5
	adds r1, #16
	movs r0, #8
	bl Func_02001564
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #11
	movs r0, #8
	lsls r1, r1, #12
	bl ObjectMotion_SetSpeedParameters
	movs r1, #8
	movs r0, #128
	bl Func_02000478
	movs r0, #8
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	movs r1, #140
	strb r3, [r0]
	movs r2, #160
	movs r0, #8
	lsls r1, r1, #1
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r1, #17
	movs r0, #8
	bl Object_SetModeById
	movs r0, #134
	bl Func_02001594
	movs r0, #2
	bl Battle_WaitMode0
	movs r1, #8
	movs r0, #128
	bl Func_02000478
	movs r0, #2
	bl Battle_WaitMode0
	movs r1, #8
	movs r0, #128
	bl Func_02000478
	movs r0, #56
	bl Battle_WaitMode0
	movs r0, #178
	bl Func_02001594
	movs r0, #8
	bl Object_GetById
	movs r5, #0
	adds r0, #85
	strb r5, [r0]
.L_02009034:
	cmp r5, #16
	bne .L_02009040
	movs r0, #8
	movs r1, #18
	bl Object_SetModeById
.L_02009040:
	movs r0, #8
	bl Object_GetById
	movs r1, #152
	ldr r3, [r0, #12]
	lsls r1, r1, #6
	adds r1, #102
	adds r3, r3, r1
	str r3, [r0, #12]
	adds r5, #1
	movs r0, #1
	bl Battle_WaitMode0
	cmp r5, #64
	bne .L_02009034
	movs r1, #2
	movs r0, #8
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #8
	movs r1, #0
	movs r2, #5
	bl Func_020014ec
	movs r5, #0
.L_0200907c:
	movs r0, #8
	bl Object_GetById
	ldr r3, [r0, #12]
	movs r2, #128
	lsls r2, r2, #9
	adds r3, r3, r2
	str r3, [r0, #12]
	adds r5, #1
	movs r0, #1
	bl Battle_WaitMode0
	cmp r5, #32
	bne .L_0200907c
	movs r0, #8
	bl Object_GetById
	movs r3, #1
	adds r0, #85
	strb r3, [r0]
	movs r5, #0
.L_020090a6:
	movs r0, #8
	bl Object_GetById
	ldr r1, .L_020093e8
	ldr r3, [r0, #12]
	adds r5, #1
	adds r3, r3, r1
	str r3, [r0, #12]
	movs r0, #1
	bl Battle_WaitMode0
	cmp r5, #5
	bne .L_020090a6
	movs r0, #128
	lsls r0, r0, #1
	movs r1, #16
	bl Func_02000478
	movs r1, #17
	movs r0, #8
	bl Object_SetModeById
	movs r0, #145
	bl Func_02001594
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #13
	lsls r2, r2, #9
	lsls r0, r0, #13
	bl Func_0200147c
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	adds r2, #102
	negs r1, r1
	negs r0, r0
	bl Func_0200147c
	movs r0, #9
	bl Object_GetById
	movs r1, #0
	bl Func_0200158c
	bl Func_02001584
	bl Func_0200157c
	movs r0, #195
	lsls r0, r0, #1
	bl Func_02001594
	movs r1, #4
	movs r0, #9
	bl Object_SetModeById
	movs r0, #32
	bl Field_BeginPaletteTransition
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #5
	movs r0, #9
	movs r1, #0
	bl Func_020014ec
	movs r1, #2
	movs r0, #8
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	movs r2, #5
	adds r0, #8
	movs r1, #0
	bl Func_020014ec
	movs r0, #9
	movs r1, #3
	bl Object_SetModeById
	movs r2, #5
	movs r0, #9
	movs r1, #0
	bl Func_020014ec
	movs r1, #18
	movs r0, #8
	bl Object_SetModeById
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	movs r1, #0
	movs r2, #5
	adds r0, #8
	bl Func_020014ec
	movs r0, #120
	bl Battle_WaitMode0
	movs r0, #107
	bl Func_02001594
	movs r5, #0
.L_02009192:
	movs r2, #128
	lsls r2, r2, #9
	lsls r1, r5, #16
	adds r1, r1, r2
	adds r0, r1, #0
	bl Func_0200147c
	adds r5, #1
	movs r0, #8
	bl Battle_WaitMode0
	cmp r5, #2
	bne .L_02009192
	movs r1, #8
	movs r0, #9
	bl Object_SetModeById
	movs r0, #60
	bl Battle_WaitMode0
	movs r0, #9
	movs r1, #7
	bl Object_SetModeById
	movs r0, #9
	movs r1, #0
	movs r2, #5
	bl Func_020014ec
	movs r1, #2
	adds r1, #255
	movs r2, #30
	movs r0, #8
	bl Func_0200150c
	movs r0, #128
	lsls r0, r0, #6
	movs r2, #5
	adds r0, #8
	movs r1, #0
	bl Func_020014ec
	movs r0, #9
	movs r1, #8
	bl Object_SetModeById
	movs r0, #9
	movs r1, #0
	movs r2, #5
	bl Func_020014ec
	movs r6, #128
	movs r5, #0
	lsls r6, r6, #10
.L_020091fe:
	movs r2, #128
	adds r0, r6, #0
	adds r1, r6, #0
	lsls r2, r2, #9
	bl Func_0200147c
	movs r0, #8
	bl Battle_WaitMode0
	movs r2, #128
	lsls r2, r2, #9
	adds r5, #1
	adds r6, r6, r2
	cmp r5, #4
	bne .L_020091fe
	movs r1, #2
	movs r0, #8
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	movs r2, #5
	adds r0, #8
	movs r1, #0
	bl Func_020014ec
	movs r0, #9
	movs r1, #3
	bl Object_SetModeById
	movs r2, #5
	movs r0, #9
	movs r1, #0
	bl Func_020014ec
	movs r1, #2
	movs r0, #8
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl Battle_WaitMode0
	movs r0, #9
	movs r1, #4
	bl Object_SetModeById
	movs r1, #0
	movs r2, #5
	movs r0, #9
	bl Func_020014ec
	movs r0, #10
	bl Battle_WaitMode0
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r0, r0, #13
	lsls r1, r1, #13
	lsls r2, r2, #9
	mov r8, r3
	bl Func_0200147c
	ldr r7, .L_020093ec
	movs r6, #0
.L_0200928c:
	cmp r6, #20
	bne .L_02009298
	movs r0, #9
	movs r1, #7
	bl Object_SetModeById
.L_02009298:
	adds r3, r6, #0
	subs r3, #30
	cmp r3, #16
	bhi .L_020092c6
	movs r0, #9
	bl Object_GetById
	movs r5, #128
	lsls r5, r5, #9
	subs r3, r5, r7
	str r3, [r0, #24]
	movs r0, #9
	bl Object_GetById
	adds r5, r7, r5
	str r5, [r0, #28]
	movs r0, #9
	bl Object_GetById
	ldr r1, .L_020093f0
	ldr r3, [r0, #16]
	adds r3, r3, r1
	str r3, [r0, #16]
.L_020092c6:
	cmp r6, #47
	bne .L_020092d4
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_020014b4
.L_020092d4:
	cmp r6, #64
	bne .L_020092ea
	movs r0, #254
	lsls r0, r0, #7
	adds r0, #255
	movs r1, #0
	bl Func_0200153c
	movs r0, #64
	bl Func_02001544
.L_020092ea:
	movs r2, #166
	lsls r2, r2, #1
	add r2, r8
	ldr r3, [r2]
	ldr r1, .L_020093f4
	movs r0, #8
	adds r3, r3, r1
	str r3, [r2]
	bl Object_GetById
	ldr r3, [r0, #16]
	movs r2, #128
	lsls r2, r2, #9
	adds r3, r3, r2
	str r3, [r0, #16]
	movs r0, #1
	bl WaitFrames
	movs r3, #128
	lsls r3, r3, #5
	adds r6, #1
	adds r7, r7, r3
	cmp r6, #128
	bne .L_0200928c
	movs r0, #78
	bl Func_02001594
	movs r0, #32
	bl Battle_WaitMode0
	movs r0, #208
	bl Func_02001594
	movs r0, #120
	bl Battle_WaitMode0
	ldr r0, .L_020093f8
	bl Scheduler_RemoveCallbackFar
	movs r0, #254
	lsls r0, r0, #7
	movs r1, #0
	adds r0, #255
	bl Func_0200153c
	movs r0, #1
	bl Func_02001544
	movs r0, #1
	bl WaitFrames
	movs r0, #195
	lsls r0, r0, #1
	bl Func_02001594
	movs r1, #0
	movs r0, #0
	bl Func_0200153c
	movs r0, #120
	bl Func_02001544
	movs r0, #150
	lsls r0, r0, #1
	bl WaitFrames
	ldr r0, .L_020093fc
.L_02009370:
	movs r1, #1
	bl Func_02001534
	b .L_020093d6
.L_02009378:
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r1, #214
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r2, #0
	str r2, [r3]
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	movs r6, #0
	b .L_020093ae
.L_02009394:
	cmp r6, #0
	bne .L_020093ac
	movs r0, #8
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #8
	movs r1, #12
	bl Object_SetModeById
.L_020093ac:
	adds r6, #1
.L_020093ae:
	movs r2, #160
	lsls r2, r2, #1
	cmp r6, r2
	beq .L_020093ce
	ldr r1, .L_02009400
	movs r2, #2
	ldr r3, [r1]
	ands r3, r2
	cmp r3, #0
	beq .L_020093c4
	movs r6, #0
.L_020093c4:
	ldr r3, [r1]
	movs r2, #1
	ands r3, r2
	cmp r3, #0
	beq .L_02009394
.L_020093ce:
	ldr r0, .L_02009404
	movs r1, #0
	bl Func_02001534
.L_020093d6:
	mov r0, r9
	bl Sys_Free
	movs r0, #0
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_020093e8:
	.4byte 0xfff7ae15
.L_020093ec:
	.4byte 0xfffe2000
.L_020093f0:
	.4byte 0xfffe0000
.L_020093f4:
	.4byte 0xffff0000
.L_020093f8:
	.4byte Func_02000394
.L_020093fc:
	.4byte 0x00000136
.L_02009400:
	.4byte gInput
.L_02009404:
	.4byte 0x00000003
	.section .rodata.x0200959c,"a",%progbits
.L_0200959c:
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
.L_020095d8:
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
.L_02009614:
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
	.global Data_02001650
Data_02001650:
	.4byte .L_0200959c
	.4byte .L_020095d8
	.4byte .L_02009614
	.global gSceneEntrances
gSceneEntrances:
	.4byte 0xffff0000
	.4byte 0x000000ac
	.4byte 0x40000095
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
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
	.4byte 0xffff0039
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x0000a000
	.4byte 0xffff0137
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x00180000
	.4byte 0x00002000
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
