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
	bl Func_02001f5c
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
	bl Func_02001f5c
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
	bl Func_02001f5c
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
	bl Func_02001f4c
	ldr r2, .L_02008330
	mov r3, r10
	ands r3, r5
	lsls r3, r3, #2
	ldr r1, [r2, r3]
	adds r0, r6, #0
	mov r11, r3
	bl Func_02001f54
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
	bl __divsi3
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, [r6, #28]
	ldr r1, [r5, #12]
	subs r0, r0, r3
	b .L_020082c8
.L_020082b6:
	ldr r2, .L_02008338
	adds r0, r3, r2
	bl __divsi3
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, .L_02008338
	ldr r1, [r5, #12]
	adds r0, r0, r3
.L_020082c8:
	bl __divsi3
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
	bl Func_02001f4c
	ldr r1, [r7, #28]
	adds r0, r6, #0
	bl Func_02001f54
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
	.4byte Data_02002148
.L_02008334:
	.4byte Func_02000124
.L_02008338:
	.4byte 0xffff0000
	.section .text.x0200833c,"ax",%progbits
	.global Func_0200033c
	.thumb_func
Func_0200033c:
	ldr r2, [r0, #80]
	movs r3, #0
	strb r3, [r2, #26]
	adds r0, #34
	movs r3, #1
	strb r3, [r0]
	movs r0, #1
	bx lr
	.section .text.x0200834c,"ax",%progbits
	.global Func_0200034c
	.thumb_func
Func_0200034c:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r4, [r3, #32]
	cmp r4, #0
	bne .L_0200835c
	movs r0, #0
	b .L_02008382
.L_0200835c:
	cmp r0, #2
	bhi .L_02008370
	lsls r3, r0, #3
	subs r3, r3, r0
	movs r0, #156
	lsls r3, r3, #3
	lsls r0, r0, #1
	adds r3, r3, r0
	ldr r4, [r4, r3]
	b .L_02008372
.L_02008370:
	ldr r4, .L_02008384
.L_02008372:
	lsls r3, r2, #7
	adds r3, r1, r3
	lsls r3, r3, #2
	adds r4, r4, r3
	ldrb r0, [r4, #2]
	ldrb r3, [r4, #3]
	lsls r0, r0, #8
	orrs r0, r3
.L_02008382:
	pop {pc}
.L_02008384:
	.4byte gMapCellBuffer
	.section .text.x02008388,"ax",%progbits
	.global Func_02000388
	.thumb_func
Func_02000388:
	push {r5, lr}
	adds r5, r1, #0
	adds r1, r3, #0
	movs r3, #192
	lsls r3, r3, #18
	ldr r4, [r3, #32]
	cmp r4, #0
	bne .L_0200839c
	movs r0, #0
	b .L_020083c8
.L_0200839c:
	cmp r0, #2
	bhi .L_020083b0
	lsls r3, r0, #3
	subs r3, r3, r0
	movs r0, #156
	lsls r3, r3, #3
	lsls r0, r0, #1
	adds r3, r3, r0
	ldr r4, [r4, r3]
	b .L_020083b2
.L_020083b0:
	ldr r4, .L_020083cc
.L_020083b2:
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
.L_020083c8:
	pop {r5, pc}
	.2byte 0x0000
.L_020083cc:
	.4byte gMapCellBuffer
	.section .text.x020083d0,"ax",%progbits
	.global Func_020003d0
	.thumb_func
Func_020003d0:
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
	ldr r3, .L_020083f8
	mov lr, r3
	.2byte 0xf800
	pop {pc}
	.2byte 0x0000
.L_020083f8:
	.4byte IwramFillWords + 0x74
	.section .text.x02008410,"ax",%progbits
	.global Func_02000410
	.thumb_func
Func_02000410:
	push {lr}
	ldr r3, .L_0200842c
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008430
	cmp r2, r3
	bne .L_02008428
	ldr r0, .L_02008434
	b .L_0200842a
.L_02008428:
	ldr r0, .L_02008438
.L_0200842a:
	pop {pc}
.L_0200842c:
	.4byte gPartyState
.L_02008430:
	.4byte 0x000000c6
.L_02008434:
	.4byte Data_02002214
.L_02008438:
	.4byte Data_020021e4
	.section .text.x0200843c,"ax",%progbits
	.global Func_0200043c
	.thumb_func
Func_0200043c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r0, #8
	sub sp, #16
	bl Object_GetById
	ldr r3, [r0, #8]
	movs r2, #128
	asrs r3, r3, #21
	lsls r2, r2, #13
	lsls r3, r3, #21
	add r7, sp, #4
	adds r3, r3, r2
	str r3, [r7]
	mov r11, r0
	ldr r3, [r0, #16]
	mov r5, r11
	asrs r3, r3, #21
	lsls r3, r3, #21
	adds r3, r3, r2
	adds r5, #8
	str r3, [r7, #8]
	adds r0, r5, #0
	adds r1, r7, #0
	bl Func_020003d0
	cmp r0, #4
	ble .L_02008532
	ldr r2, [r5]
	ldr r3, [r7]
	cmp r2, r3
	bge .L_0200848e
	movs r1, #204
	lsls r1, r1, #6
	adds r1, #51
	b .L_02008490
.L_0200848e:
	ldr r1, .L_02008540
.L_02008490:
	adds r3, r2, r1
	str r3, [r5]
	mov r3, r11
	ldr r2, [r3, #16]
	ldr r3, [r7, #8]
	cmp r2, r3
	bge .L_020084a6
	movs r1, #204
	lsls r1, r1, #6
	adds r1, #51
	b .L_020084a8
.L_020084a6:
	ldr r1, .L_02008540
.L_020084a8:
	adds r3, r2, r1
	mov r2, r11
	str r3, [r2, #16]
	ldr r1, [r7]
	ldr r2, [r7, #8]
	asrs r1, r1, #20
	asrs r2, r2, #20
	movs r0, #1
	bl Func_0200034c
	asrs r0, r0, #8
	cmp r0, #40
	bne .L_020084ce
	ldr r0, .L_02008544
	bl Math_Cosine
	str r0, [sp, #0]
	ldr r0, .L_02008544
	b .L_020084dc
.L_020084ce:
	movs r0, #128
	lsls r0, r0, #2
	bl Math_Cosine
	str r0, [sp, #0]
	movs r0, #128
	lsls r0, r0, #2
.L_020084dc:
	bl Math_Sine
	mov r9, r0
	mov r3, r11
	ldr r5, [r3, #8]
	mov r1, r11
	ldr r3, [r7]
	ldr r1, [r1, #16]
	subs r5, r5, r3
	ldr r3, [r7, #8]
	mov r10, r1
	mov r2, r10
	subs r2, r2, r3
	ldr r3, .L_02008548
	ldr r1, [sp, #0]
	mov r8, r3
	adds r0, r5, #0
	mov r10, r2
	mov lr, r8
	.2byte 0xf800
	mov r1, r9
	adds r6, r0, #0
	mov r0, r10
	mov lr, r8
	.2byte 0xf800
	mov r1, r9
	subs r6, r6, r0
	adds r0, r5, #0
	mov lr, r8
	.2byte 0xf800
	adds r5, r0, #0
	ldr r1, [sp, #0]
	mov r0, r10
	mov lr, r8
	.2byte 0xf800
	ldr r3, [r7]
	adds r5, r5, r0
	adds r6, r6, r3
	ldr r3, [r7, #8]
	mov r1, r11
	adds r5, r5, r3
	str r6, [r1, #8]
	str r5, [r1, #16]
.L_02008532:
	add sp, #16
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_02008540:
	.4byte 0xffffcccd
.L_02008544:
	.4byte 0xfffffe00
.L_02008548:
	.4byte IwramMulQ16
	.section .text.x0200854c,"ax",%progbits
	.global Func_0200054c
	.thumb_func
Func_0200054c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	movs r0, #8
	ldr r5, [r3, #108]
	sub sp, #88
	bl Object_GetById
	movs r1, #170
	lsls r1, r1, #1
	adds r5, r5, r1
	movs r2, #0
	ldrsh r3, [r5, r2]
	mov r9, r0
	mov r6, r9
	subs r3, #30
	add r1, sp, #76
	adds r6, #8
	cmp r3, #7
	bhi .L_020085e8
	ldr r2, .L_020087fc
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
.L_02008588:
	.4byte .L_020085a8
	.4byte .L_020085c6
	.4byte .L_020085ac
	.4byte .L_020085ce
	.4byte .L_020085b6
	.4byte .L_020085d8
	.4byte .L_020085be
	.4byte .L_020085de
.L_020085a8:
	movs r3, #0
	b .L_020085e0
.L_020085ac:
	movs r3, #1
	negs r3, r3
	str r3, [r1]
	movs r3, #0
	b .L_020085e6
.L_020085b6:
	movs r3, #0
	str r3, [r1]
	movs r3, #1
	b .L_020085e6
.L_020085be:
	movs r3, #1
	str r3, [r1]
	movs r3, #0
	b .L_020085e6
.L_020085c6:
	movs r3, #1
	negs r3, r3
	str r3, [r1]
	b .L_020085e6
.L_020085ce:
	movs r3, #1
	negs r3, r3
	str r3, [r1]
	movs r3, #1
	b .L_020085e6
.L_020085d8:
	movs r3, #1
	str r3, [r1]
	b .L_020085e6
.L_020085de:
	movs r3, #1
.L_020085e0:
	str r3, [r1]
	movs r3, #1
	negs r3, r3
.L_020085e6:
	str r3, [r1, #8]
.L_020085e8:
	mov r1, r9
	add r5, sp, #76
	ldr r3, [r1, #8]
	ldr r2, [r5]
	asrs r3, r3, #21
	adds r3, r3, r2
	movs r1, #128
	lsls r1, r1, #13
	lsls r3, r3, #21
	adds r3, r3, r1
	str r3, [r5]
	mov r2, r9
	ldr r3, [r2, #16]
	ldr r2, [r5, #8]
	asrs r3, r3, #21
	adds r3, r3, r2
	lsls r3, r3, #21
	adds r3, r3, r1
	str r3, [r5, #8]
	adds r0, r6, #0
	adds r1, r5, #0
	bl Func_020003d0
	cmp r0, #46
	ble .L_02008648
	ldr r2, [r6]
	ldr r3, [r5]
	cmp r2, r3
	bge .L_02008628
	movs r1, #128
	lsls r1, r1, #8
	b .L_0200862a
.L_02008628:
	ldr r1, .L_02008800
.L_0200862a:
	adds r3, r2, r1
	str r3, [r6]
	mov r2, r9
	ldr r0, [r2, #16]
	ldr r3, [r5, #8]
	cmp r0, r3
	bge .L_02008640
	movs r5, #128
	lsls r5, r5, #8
	adds r3, r0, r5
	b .L_02008646
.L_02008640:
	ldr r1, .L_02008800
	mov r2, r9
	adds r3, r0, r1
.L_02008646:
	str r3, [r2, #16]
.L_02008648:
	add r3, sp, #76
	ldr r1, [r3]
	ldr r2, [r3, #8]
	asrs r1, r1, #20
	asrs r2, r2, #20
	movs r0, #1
	mov r11, r3
	bl Func_0200034c
	asrs r0, r0, #8
	cmp r0, #40
	bne .L_0200866c
	ldr r0, .L_02008804
	bl Math_Cosine
	str r0, [sp, #16]
	ldr r0, .L_02008804
	b .L_0200867a
.L_0200866c:
	movs r0, #128
	lsls r0, r0, #2
	bl Math_Cosine
	str r0, [sp, #16]
	movs r0, #128
	lsls r0, r0, #2
.L_0200867a:
	bl Math_Sine
	adds r7, r0, #0
	mov r1, r9
	mov r2, r11
	ldr r3, [r2]
	ldr r5, [r1, #8]
	subs r5, r5, r3
	ldr r3, [r1, #16]
	adds r0, r5, #0
	mov r10, r3
	ldr r3, [r2, #8]
	ldr r2, .L_02008808
	mov r1, r10
	subs r1, r1, r3
	mov r8, r2
	mov r10, r1
	ldr r1, [sp, #16]
	mov lr, r8
	.2byte 0xf800
	adds r1, r7, #0
	adds r6, r0, #0
	mov r0, r10
	mov lr, r8
	.2byte 0xf800
	adds r1, r7, #0
	subs r6, r6, r0
	adds r0, r5, #0
	mov lr, r8
	.2byte 0xf800
	adds r5, r0, #0
	ldr r1, [sp, #16]
	mov r0, r10
	mov lr, r8
	.2byte 0xf800
	mov r1, r11
	ldr r3, [r1]
	adds r5, r5, r0
	adds r6, r6, r3
	ldr r3, [r1, #8]
	ldr r7, .L_0200880c
	adds r5, r5, r3
	mov r2, r9
	str r5, [r2, #16]
	str r6, [r2, #8]
	ldr r2, [r7]
	movs r3, #3
	mov r10, r3
	mov r5, r10
	ands r5, r2
	mov r10, r5
	cmp r5, #0
	beq .L_020086e6
	b .L_020087ee
.L_020086e6:
	movs r3, #10
	ands r3, r2
	cmp r3, #0
	bne .L_020086f8
	movs r0, #171
	lsls r0, r0, #1
	adds r0, #255
	bl Func_02002054
.L_020086f8:
	add r1, sp, #20
	movs r3, #2
	str r3, [r1]
	movs r3, #7
	str r3, [r1, #4]
	movs r3, #204
	lsls r3, r3, #8
	adds r3, #204
	str r3, [r1, #8]
	str r3, [r1, #12]
	mov r2, r9
	ldrh r0, [r2, #6]
	mov r8, r1
	bl Math_Cosine
	mov r3, r9
	adds r5, r0, #0
	ldrh r0, [r3, #6]
	bl Math_Sine
	lsls r3, r5, #2
	lsls r2, r0, #2
	adds r3, r3, r5
	adds r2, r2, r0
	lsls r3, r3, #1
	lsls r2, r2, #1
	add r6, sp, #60
	negs r0, r3
	negs r4, r2
	str r3, [r6]
	str r2, [r6, #4]
	str r0, [r6, #8]
	str r4, [r6, #12]
	mov r5, r9
	ldr r1, [r5, #8]
	adds r3, r3, r1
	str r3, [r6]
	movs r1, #128
	ldr r3, [r5, #16]
	lsls r1, r1, #8
	adds r2, r2, r3
	str r2, [r6, #4]
	mov r11, r1
	ldr r3, [r5, #8]
	adds r0, r0, r3
	str r0, [r6, #8]
	ldr r3, [r5, #16]
	adds r4, r4, r3
	str r4, [r6, #12]
	ldr r3, [r5, #16]
	cmp r2, r3
	beq .L_0200876a
	ldr r3, .L_02008810
	adds r2, r2, r3
	adds r3, r4, r3
	str r2, [r6, #4]
	str r3, [r6, #12]
.L_0200876a:
	ldr r7, [r7]
	movs r3, #4
	mov r2, r9
	ands r7, r3
	ldr r1, [r2, #12]
	ldr r0, [r6]
	ldr r2, [r6, #4]
	cmp r7, #0
	beq .L_020087b8
	movs r3, #192
	mov r5, r10
	lsls r3, r3, #10
	adds r0, r0, r3
	str r5, [sp, #0]
	str r5, [sp, #4]
	mov r3, r8
	movs r5, #176
	lsls r5, r5, #12
	str r3, [sp, #12]
	mov r3, r11
	str r5, [sp, #8]
	bl Func_0200015c
	ldr r0, [r6, #8]
	ldr r1, .L_02008814
	mov r3, r10
	mov r2, r9
	adds r0, r0, r1
	ldr r1, [r2, #12]
	ldr r2, [r6, #12]
	str r3, [sp, #0]
	str r3, [sp, #4]
	str r5, [sp, #8]
	ldr r3, .L_02008800
	mov r5, r8
	str r5, [sp, #12]
	bl Func_0200015c
	b .L_020087ee
.L_020087b8:
	ldr r3, .L_02008814
	movs r5, #176
	adds r0, r0, r3
	mov r3, r8
	lsls r5, r5, #12
	str r3, [sp, #12]
	ldr r3, .L_02008800
	str r7, [sp, #0]
	str r7, [sp, #4]
	str r5, [sp, #8]
	bl Func_0200015c
	ldr r0, [r6, #8]
	movs r1, #192
	lsls r1, r1, #10
	mov r2, r9
	mov r3, r8
	adds r0, r0, r1
	ldr r1, [r2, #12]
	ldr r2, [r6, #12]
	str r3, [sp, #12]
	mov r3, r11
	str r7, [sp, #0]
	str r7, [sp, #4]
	str r5, [sp, #8]
	bl Func_0200015c
.L_020087ee:
	add sp, #88
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_020087fc:
	.4byte .L_02008588
.L_02008800:
	.4byte 0xffff8000
.L_02008804:
	.4byte 0xfffffe00
.L_02008808:
	.4byte IwramMulQ16
.L_0200880c:
	.4byte Data_0300122c
.L_02008810:
	.4byte 0xfff40000
.L_02008814:
	.4byte 0xfffd0000
	.section .text.x02008818,"ax",%progbits
	.global Func_02000818
	.thumb_func
Func_02000818:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	sub sp, #96
	movs r0, #8
	str r3, [sp, #36]
	bl Object_GetById
	mov r8, r0
	ldr r0, [sp, #36]
	movs r1, #170
	lsls r1, r1, #1
	adds r6, r0, r1
	ldrh r5, [r6]
	bl Func_02001fac
	movs r0, #0
	bl Func_02002044
	mov r2, r8
	ldr r3, [r2, #8]
	subs r5, #45
	asrs r3, r3, #20
	str r3, [sp, #32]
	lsls r5, r5, #30
	ldr r3, [r2, #16]
	asrs r5, r5, #16
	asrs r3, r3, #20
	adds r0, r5, #0
	str r3, [sp, #28]
	bl Math_Cosine
	asrs r0, r0, #16
	mov r11, r0
	adds r0, r5, #0
	bl Math_Sine
	asrs r0, r0, #16
	mov r9, r0
	movs r3, #128
	lsls r3, r3, #11
	mov r5, r8
	movs r0, #0
	mov r1, r9
	str r3, [r5, #48]
	str r3, [r5, #52]
	str r0, [sp, #20]
	str r0, [sp, #24]
	cmp r1, #0
	bge .L_0200888c
	negs r1, r1
.L_0200888c:
	ldr r2, [sp, #32]
	mov r3, r11
	adds r1, r2, r1
	cmp r3, #0
	bge .L_02008898
	negs r3, r3
.L_02008898:
	ldr r5, [sp, #28]
	movs r0, #1
	adds r2, r5, r3
	bl Func_0200034c
	movs r1, #0
	ldrsh r3, [r6, r1]
	asrs r0, r0, #8
	cmp r0, r3
	bne .L_020088cc
	mov r2, r9
	cmp r2, #0
	bge .L_020088b4
	negs r2, r2
.L_020088b4:
	lsls r3, r2, #4
	subs r3, r3, r2
	lsls r3, r3, #16
	mov r2, r11
	str r3, [sp, #24]
	cmp r2, #0
	bge .L_020088c4
	negs r2, r2
.L_020088c4:
	lsls r3, r2, #4
	subs r3, r3, r2
	lsls r3, r3, #16
	str r3, [sp, #20]
.L_020088cc:
	ldr r2, [sp, #32]
	ldr r5, [sp, #24]
	ldr r0, [sp, #28]
	ldr r1, [sp, #20]
	lsls r3, r2, #20
	adds r5, r5, r3
	lsls r3, r0, #20
	adds r1, r1, r3
	str r5, [sp, #24]
	str r1, [sp, #20]
	b .L_02008a3c
.L_020088e2:
	ldr r2, [sp, #32]
	ldr r3, [sp, #28]
	add r2, r11
	ldr r0, [sp, #24]
	str r2, [sp, #32]
	add r3, r9
	ldr r2, [sp, #20]
	mov r5, r11
	str r3, [sp, #28]
	mov r1, r9
	lsls r3, r5, #20
	adds r0, r0, r3
	lsls r3, r1, #20
	adds r2, r2, r3
	str r0, [sp, #24]
	str r2, [sp, #20]
	mov r3, r8
	ldr r2, [r3, #12]
	mov r0, r8
	ldr r1, [sp, #24]
	ldr r3, [sp, #20]
	bl Func_02001f6c
	b .L_02008a30
.L_02008912:
	ldr r7, .L_02008a6c
	movs r5, #3
	ldr r2, [r7]
	ands r5, r2
	str r5, [sp, #16]
	cmp r5, #0
	beq .L_02008922
	b .L_02008a2a
.L_02008922:
	movs r3, #10
	ands r3, r2
	cmp r3, #0
	bne .L_02008934
	movs r0, #171
	lsls r0, r0, #1
	adds r0, #255
	bl Func_02002054
.L_02008934:
	add r0, sp, #40
	movs r3, #2
	str r3, [r0]
	movs r3, #7
	str r3, [r0, #4]
	movs r3, #179
	lsls r3, r3, #9
	adds r3, #102
	str r3, [r0, #8]
	ldr r3, .L_02008a70
	mov r1, r8
	str r3, [r0, #12]
	mov r10, r0
	ldrh r0, [r1, #6]
	bl Math_Cosine
	mov r2, r8
	adds r5, r0, #0
	ldrh r0, [r2, #6]
	bl Math_Sine
	lsls r3, r5, #1
	lsls r2, r0, #1
	adds r3, r3, r5
	adds r2, r2, r0
	lsls r3, r3, #2
	lsls r2, r2, #2
	add r6, sp, #80
	negs r0, r3
	negs r4, r2
	str r3, [r6]
	str r2, [r6, #4]
	str r0, [r6, #8]
	str r4, [r6, #12]
	mov r5, r8
	ldr r1, [r5, #8]
	adds r3, r3, r1
	str r3, [r6]
	ldr r3, [r5, #16]
	adds r2, r2, r3
	str r2, [r6, #4]
	ldr r3, [r5, #8]
	adds r0, r0, r3
	str r0, [r6, #8]
	ldr r3, [r5, #16]
	adds r4, r4, r3
	str r4, [r6, #12]
	ldr r3, [r5, #16]
	cmp r2, r3
	beq .L_020089a2
	ldr r3, .L_02008a74
	adds r2, r2, r3
	adds r3, r4, r3
	str r2, [r6, #4]
	str r3, [r6, #12]
.L_020089a2:
	ldr r7, [r7]
	movs r3, #4
	mov r0, r8
	ands r7, r3
	ldr r1, [r0, #12]
	ldr r2, [r6, #4]
	ldr r0, [r6]
	cmp r7, #0
	beq .L_020089f2
	ldr r5, [sp, #16]
	movs r3, #128
	lsls r3, r3, #11
	adds r0, r0, r3
	mov r3, r10
	str r5, [sp, #0]
	str r5, [sp, #4]
	str r3, [sp, #12]
	movs r5, #176
	movs r3, #128
	lsls r5, r5, #12
	lsls r3, r3, #8
	str r5, [sp, #8]
	bl Func_0200015c
	ldr r0, [r6, #8]
	ldr r3, [sp, #16]
	ldr r1, .L_02008a78
	mov r2, r8
	adds r0, r0, r1
	ldr r1, [r2, #12]
	ldr r2, [r6, #12]
	str r3, [sp, #0]
	str r3, [sp, #4]
	str r5, [sp, #8]
	ldr r3, .L_02008a7c
	mov r5, r10
	str r5, [sp, #12]
	bl Func_0200015c
	b .L_02008a2a
.L_020089f2:
	ldr r3, .L_02008a78
	movs r5, #176
	adds r0, r0, r3
	mov r3, r10
	lsls r5, r5, #12
	str r3, [sp, #12]
	ldr r3, .L_02008a7c
	str r7, [sp, #0]
	str r7, [sp, #4]
	str r5, [sp, #8]
	bl Func_0200015c
	ldr r0, [r6, #8]
	movs r1, #128
	lsls r1, r1, #11
	mov r2, r8
	mov r3, r10
	adds r0, r0, r1
	ldr r1, [r2, #12]
	ldr r2, [r6, #12]
	str r3, [sp, #12]
	movs r3, #128
	lsls r3, r3, #8
	str r7, [sp, #0]
	str r7, [sp, #4]
	str r5, [sp, #8]
	bl Func_0200015c
.L_02008a2a:
	movs r0, #1
	bl WaitFrames
.L_02008a30:
	mov r0, r8
	bl Func_02001f9c
	cmp r0, #0
	bne .L_02008a3c
	b .L_02008912
.L_02008a3c:
	ldr r1, [sp, #32]
	ldr r2, [sp, #28]
	movs r0, #1
	bl Func_0200034c
	ldr r5, [sp, #36]
	movs r1, #170
	lsls r1, r1, #1
	adds r3, r5, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	asrs r0, r0, #8
	cmp r0, r3
	bne .L_02008a5a
	b .L_020088e2
.L_02008a5a:
	bl Func_02001fb4
	add sp, #96
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_02008a6c:
	.4byte Data_0300122c
.L_02008a70:
	.4byte 0x00013333
.L_02008a74:
	.4byte 0xfff40000
.L_02008a78:
	.4byte 0xfffc0000
.L_02008a7c:
	.4byte 0xffff8000
	.section .text.x02008a80,"ax",%progbits
	.global Func_02000a80
	.thumb_func
Func_02000a80:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r0, #8
	sub sp, #108
	mov r11, r3
	bl Object_GetById
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	asrs r1, r1, #20
	asrs r2, r2, #20
	movs r0, #0
	bl Func_0200034c
	asrs r0, r0, #8
	subs r0, #1
	lsls r0, r0, #16
	asrs r0, r0, #16
	str r0, [sp, #16]
	ldr r6, .L_02008af8
	ldr r7, .L_02008afc
	ldr r3, [r6]
	ldr r2, [r7]
	subs r3, r3, r2
	cmp r3, #50
	bls .L_02008ace
	ldr r3, .L_02008b00
	ldr r2, .L_02008af4
	strh r2, [r3]
	ldr r3, .L_02008b04
	strh r2, [r3]
.L_02008ace:
	ldr r3, .L_02008b08
	ldrh r2, [r3]
	movs r3, #170
	lsls r3, r3, #1
	add r3, r11
	movs r0, #0
	ldrsh r4, [r3, r0]
	ldrh r1, [r3]
	cmp r2, r4
	beq .L_02008b5e
	ldr r0, .L_02008b04
	movs r5, #0
	ldrsh r3, [r0, r5]
	cmp r3, #0
	bne .L_02008b0c
	subs r3, r2, r1
	strh r3, [r0]
	b .L_02008b0c
	.2byte 0x0000
.L_02008af4:
	.4byte 0x00000000
.L_02008af8:
	.4byte Data_0300122c
.L_02008afc:
	.4byte Data_020022c4
.L_02008b00:
	.4byte Data_020022be
.L_02008b04:
	.4byte Data_020022c0
.L_02008b08:
	.4byte Data_020022bc
.L_02008b0c:
	movs r1, #0
	ldrsh r3, [r0, r1]
	cmp r3, #7
	bne .L_02008b18
	ldr r3, .L_02008b40
	strh r3, [r0]
.L_02008b18:
	movs r5, #0
	ldrsh r3, [r0, r5]
	movs r1, #7
	negs r1, r1
	cmp r3, r1
	bne .L_02008b28
	ldr r3, .L_02008b44
	strh r3, [r0]
.L_02008b28:
	movs r5, #0
	ldrsh r1, [r0, r5]
	subs r3, r2, r4
	ldr r5, .L_02008b48
	cmp r3, r1
	beq .L_02008b4c
	lsls r3, r1, #3
	subs r2, r4, r2
	subs r3, r3, r1
	cmp r2, r3
	bne .L_02008b58
	b .L_02008b4c
.L_02008b40:
	.4byte 0xffffffff
.L_02008b44:
	.4byte 0x00000001
.L_02008b48:
	.4byte Data_020022be
.L_02008b4c:
	ldrh r3, [r5]
	adds r3, #1
	strh r3, [r5]
	ldr r3, [r6]
	str r3, [r7]
	b .L_02008b5e
.L_02008b58:
	ldr r3, .L_02008b88
	strh r3, [r5]
	strh r3, [r0]
.L_02008b5e:
	movs r3, #170
	lsls r3, r3, #1
	add r3, r11
	ldrh r3, [r3]
	ldr r2, .L_02008b8c
	strh r3, [r2]
	ldr r3, .L_02008b90
	ldrh r3, [r3]
	cmp r3, #3
	bls .L_02008c0c
	ldr r3, .L_02008b94
	ldr r7, [r3]
	movs r3, #3
	ands r7, r3
	cmp r7, #0
	bne .L_02008c0c
	add r0, sp, #60
	ldr r3, .L_02008b98
	mov r10, r0
	mov r2, r10
	b .L_02008b9c
.L_02008b88:
	.4byte 0x00000000
.L_02008b8c:
	.4byte Data_020022bc
.L_02008b90:
	.4byte Data_020022be
.L_02008b94:
	.4byte Data_0300122c
.L_02008b98:
	.4byte Data_02002118
.L_02008b9c:
	ldmia r3!, {r1, r4, r5}
	stmia r2!, {r1, r4, r5}
	ldmia r3!, {r0, r1, r4}
	stmia r2!, {r0, r1, r4}
	ldmia r3!, {r0, r1, r5}
	stmia r2!, {r0, r1, r5}
	movs r1, #20
	ldmia r3!, {r0, r4, r5}
	stmia r2!, {r0, r4, r5}
	add r1, sp
	movs r3, #209
	mov r9, r1
	lsls r3, r3, #1
	mov r2, r9
	adds r3, #255
	strh r3, [r2, #24]
	movs r3, #2
	str r3, [r2]
	bl Random16Far
	movs r3, #31
	adds r6, r0, #0
	mov r8, r3
	ands r6, r3
	bl Random16Far
	mov r4, r8
	adds r5, r0, #0
	movs r0, #246
	ands r5, r4
	bl Func_02002054
	ldr r0, [sp, #16]
	mov r1, r10
	lsls r3, r0, #16
	lsrs r3, r3, #13
	ldr r0, [r1, r3]
	adds r3, #4
	ldr r2, [r1, r3]
	ldr r3, .L_02008c40
	lsls r6, r6, #16
	str r3, [sp, #0]
	movs r3, #144
	lsls r3, r3, #13
	adds r3, #1
	lsls r5, r5, #16
	str r3, [sp, #8]
	mov r3, r9
	str r3, [sp, #12]
	adds r0, r0, r6
	adds r2, r2, r5
	movs r1, #0
	movs r3, #0
	str r7, [sp, #4]
	bl Func_0200015c
.L_02008c0c:
	ldr r4, [sp, #16]
	ldr r0, .L_02008c44
	ldr r1, .L_02008c48
	lsls r3, r4, #16
	lsrs r3, r3, #16
	ldrh r2, [r0]
	ldrb r3, [r1, r3]
	cmp r2, r3
	bne .L_02008c30
	movs r1, #181
	lsls r1, r1, #1
	movs r2, #200
	add r1, r11
	strh r2, [r1]
	ldr r2, .L_02008c4c
	movs r3, #0
	strh r3, [r0]
	strh r3, [r2]
.L_02008c30:
	add sp, #108
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008c40:
	.4byte 0x00013333
.L_02008c44:
	.4byte Data_020022be
.L_02008c48:
	.4byte Data_02002110
.L_02008c4c:
	.4byte Data_020022c0
	.section .text.x02008c50,"ax",%progbits
	.global Func_02000c50
	.thumb_func
Func_02000c50:
	push {r5, lr}
	ldr r4, .L_02008c84
	ldr r0, .L_02008c88
	ldr r3, [r4]
	ldr r2, [r0]
	ldr r1, .L_02008c8c
	subs r3, r3, r2
	cmp r3, #120
	bls .L_02008c72
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r5, #181
	lsls r5, r5, #1
	adds r2, r3, r5
	movs r3, #201
	strh r3, [r2]
.L_02008c72:
	ldr r3, [r0]
	stmia r1!, {r3}
	ldr r3, .L_02008c90
	ldr r3, [r3]
	stmia r1!, {r3}
	ldr r3, [r4]
	str r3, [r1]
	pop {r5, pc}
	.2byte 0x0000
.L_02008c84:
	.4byte Data_0300122c
.L_02008c88:
	.4byte Data_020022c8
.L_02008c8c:
	.4byte gSceneState
.L_02008c90:
	.4byte Data_020022cc
	.section .text.x02008c94,"ax",%progbits
	.global Func_02000c94
	.thumb_func
Func_02000c94:
	push {r5, r6, r7, lr}
	bl Func_02001fac
	movs r0, #0
	bl Func_02002044
	movs r0, #128
	lsls r0, r0, #2
	bl GameFlag_SetBit
	movs r0, #151
	lsls r0, r0, #1
	adds r0, #255
	bl Func_02002054
.L_02008cb2:
	movs r0, #10
	bl WaitFrames
	movs r6, #160
	lsls r6, r6, #19
	movs r5, #0
	movs r7, #1
	adds r6, #98
.L_02008cc2:
	ldrh r3, [r6]
	ldr r1, .L_02008cf8
	movs r4, #31
	lsls r2, r3, #16
	ands r4, r3
	lsrs r0, r2, #21
	adds r3, r4, #0
	lsrs r2, r2, #26
	ands r0, r1
	ands r2, r1
	cmp r3, #10
	bhi .L_02008cde
	adds r4, r3, #1
	movs r5, #1
.L_02008cde:
	lsls r3, r4, #16
	lsrs r3, r3, #16
	cmp r3, #11
	bls .L_02008cfc
	movs r1, #255
	lsls r1, r1, #8
	adds r1, #255
	adds r3, r3, r1
	lsls r3, r3, #16
	asrs r4, r3, #16
	movs r5, #1
	b .L_02008cfc
	.2byte 0x0000
.L_02008cf8:
	.4byte 0x0000001f
.L_02008cfc:
	lsls r3, r0, #16
	lsrs r3, r3, #16
	cmp r3, #5
	bhi .L_02008d0c
	adds r3, #1
	lsls r3, r3, #16
	asrs r0, r3, #16
	movs r5, #1
.L_02008d0c:
	lsls r3, r0, #16
	lsrs r3, r3, #16
	cmp r3, #6
	bls .L_02008d22
	movs r1, #255
	lsls r1, r1, #8
	adds r1, #255
	adds r3, r3, r1
	lsls r3, r3, #16
	asrs r0, r3, #16
	movs r5, #1
.L_02008d22:
	lsls r3, r2, #16
	lsrs r3, r3, #16
	cmp r3, #22
	bhi .L_02008d32
	adds r3, #1
	lsls r3, r3, #16
	asrs r2, r3, #16
	movs r5, #1
.L_02008d32:
	lsls r3, r2, #16
	lsrs r3, r3, #16
	cmp r3, #23
	bls .L_02008d48
	movs r2, #255
	lsls r2, r2, #8
	adds r2, #255
	adds r3, r3, r2
	lsls r3, r3, #16
	asrs r2, r3, #16
	movs r5, #1
.L_02008d48:
	lsls r3, r2, #16
	lsls r2, r0, #16
	lsrs r2, r2, #11
	lsrs r3, r3, #6
	orrs r3, r2
	lsls r2, r4, #16
	lsrs r2, r2, #16
	orrs r2, r3
	adds r7, #1
	strh r2, [r6]
	adds r6, #2
	cmp r7, #14
	bne .L_02008cc2
	ldr r3, .L_02008d90
	ldr r1, .L_02008d94
	ldr r0, [r3]
	ldr r3, [r1]
	movs r2, #1
	ands r2, r0
	adds r3, r3, r2
	str r3, [r1]
	cmp r5, #0
	bne .L_02008cb2
	movs r3, #180
	lsls r3, r3, #1
	str r3, [r1]
	ldr r3, .L_02008d98
	movs r1, #144
	str r0, [r3]
	lsls r1, r1, #3
	ldr r0, .L_02008d9c
	bl Scheduler_AddOrUpdateCallback
	bl Func_02001fb4
	pop {r5, r6, r7, pc}
.L_02008d90:
	.4byte Data_0300122c
.L_02008d94:
	.4byte Data_020022cc
.L_02008d98:
	.4byte Data_020022c8
.L_02008d9c:
	.4byte Func_02000c50
	.section .text.x02008da0,"ax",%progbits
	.global Func_02000da0
	.thumb_func
Func_02000da0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r0, .L_02008e24
	bl Scheduler_RemoveCallbackFar
	bl Func_02001fac
	movs r0, #0
	bl Func_02002044
	movs r0, #139
	lsls r0, r0, #2
	bl Func_02002054
	ldr r2, .L_02008e28
	movs r3, #1
	str r3, [r2]
.L_02008dcc:
	movs r0, #10
	bl WaitFrames
	ldr r3, .L_02008e2c
	movs r4, #160
	lsls r4, r4, #19
	movs r0, #0
	movs r2, #1
	adds r4, #98
	mov lr, r0
	mov r9, r2
	mov r11, r3
	mov r10, r0
	mov r8, r4
.L_02008de8:
	mov r0, r8
	ldrh r1, [r0]
	mov r3, r10
	mov r0, r11
	ldrsh r2, [r3, r0]
	ldr r3, .L_02008e20
	movs r4, #31
	lsls r2, r2, #16
	lsls r0, r1, #16
	adds r5, r4, #0
	ands r5, r1
	lsrs r6, r2, #21
	lsrs r1, r0, #21
	lsrs r7, r2, #26
	lsrs r0, r0, #26
	lsrs r2, r2, #16
	ands r1, r3
	ands r0, r3
	ands r6, r3
	ands r7, r3
	ands r2, r4
	adds r3, r5, #0
	cmp r3, r2
	bcs .L_02008e30
	adds r5, r3, #1
	movs r3, #1
	mov lr, r3
	b .L_02008e30
.L_02008e20:
	.4byte 0x0000001f
.L_02008e24:
	.4byte Func_02000c50
.L_02008e28:
	.4byte Data_020022cc
.L_02008e2c:
	.4byte Data_020026c0
.L_02008e30:
	lsls r3, r5, #16
	lsrs r3, r3, #16
	cmp r3, r2
	bls .L_02008e48
	movs r4, #255
	lsls r4, r4, #8
	adds r4, #255
	adds r3, r3, r4
	lsls r3, r3, #16
	movs r2, #1
	asrs r5, r3, #16
	mov lr, r2
.L_02008e48:
	lsls r3, r1, #16
	lsrs r3, r3, #16
	mov r12, r6
	cmp r3, r12
	bcs .L_02008e5c
	adds r3, #1
	lsls r3, r3, #16
	asrs r1, r3, #16
	movs r3, #1
	mov lr, r3
.L_02008e5c:
	lsls r3, r1, #16
	lsrs r3, r3, #16
	cmp r3, r12
	bls .L_02008e74
	movs r4, #255
	lsls r4, r4, #8
	adds r4, #255
	adds r3, r3, r4
	lsls r3, r3, #16
	movs r2, #1
	asrs r1, r3, #16
	mov lr, r2
.L_02008e74:
	lsls r3, r0, #16
	lsrs r3, r3, #16
	mov r12, r7
	cmp r3, r12
	bcs .L_02008e88
	adds r3, #1
	lsls r3, r3, #16
	asrs r0, r3, #16
	movs r3, #1
	mov lr, r3
.L_02008e88:
	lsls r3, r0, #16
	lsrs r3, r3, #16
	cmp r3, r12
	bls .L_02008ea0
	movs r4, #255
	lsls r4, r4, #8
	adds r4, #255
	adds r3, r3, r4
	lsls r3, r3, #16
	movs r2, #1
	asrs r0, r3, #16
	mov lr, r2
.L_02008ea0:
	lsls r3, r0, #16
	lsls r2, r1, #16
	lsrs r2, r2, #11
	lsrs r3, r3, #6
	orrs r3, r2
	lsls r2, r5, #16
	lsrs r2, r2, #16
	movs r0, #1
	orrs r2, r3
	add r9, r0
	mov r3, r8
	movs r4, #2
	strh r2, [r3]
	mov r2, r9
	add r8, r4
	add r10, r4
	cmp r2, #15
	bne .L_02008de8
	mov r3, lr
	cmp r3, #0
	beq .L_02008ecc
	b .L_02008dcc
.L_02008ecc:
	ldr r2, .L_02008eec
	movs r3, #1
	movs r0, #128
	str r3, [r2]
	lsls r0, r0, #2
	bl GameFlag_ClearBit
	bl Func_02001fb4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008eec:
	.4byte Data_020022cc
	.section .text.x02008ef0,"ax",%progbits
	.global Func_02000ef0
	.thumb_func
Func_02000ef0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r0, #8
	sub sp, #72
	bl Object_GetById
	mov r8, r0
	bl Func_02001fac
	movs r0, #0
	bl Func_02002044
	mov r0, r8
	ldr r3, [r0, #16]
	movs r5, #128
	lsls r5, r5, #12
	ldr r1, [r0, #8]
	ldr r2, [r0, #12]
	adds r3, r3, r5
	bl Func_02001f6c
	b .L_02009036
.L_02008f26:
	ldr r7, .L_02009054
	movs r3, #3
	ldr r0, [r7]
	mov r10, r0
	mov r1, r10
	ands r1, r3
	mov r10, r1
	cmp r1, #0
	bne .L_02009030
	add r2, sp, #16
	movs r3, #2
	str r3, [r2]
	movs r3, #7
	str r3, [r2, #4]
	movs r3, #179
	lsls r3, r3, #9
	adds r3, #102
	str r3, [r2, #8]
	ldr r3, .L_02009058
	mov r9, r2
	str r3, [r2, #12]
	mov r3, r8
	ldrh r0, [r3, #6]
	bl Math_Cosine
	mov r1, r8
	adds r5, r0, #0
	ldrh r0, [r1, #6]
	bl Math_Sine
	lsls r3, r5, #1
	lsls r2, r0, #1
	adds r3, r3, r5
	adds r2, r2, r0
	lsls r3, r3, #2
	lsls r2, r2, #2
	add r6, sp, #56
	negs r0, r3
	negs r4, r2
	str r3, [r6]
	str r2, [r6, #4]
	str r0, [r6, #8]
	str r4, [r6, #12]
	mov r5, r8
	ldr r1, [r5, #8]
	adds r3, r3, r1
	str r3, [r6]
	ldr r3, [r5, #16]
	adds r2, r2, r3
	str r2, [r6, #4]
	ldr r3, [r5, #8]
	adds r0, r0, r3
	str r0, [r6, #8]
	movs r0, #128
	ldr r3, [r5, #16]
	lsls r0, r0, #8
	adds r4, r4, r3
	str r4, [r6, #12]
	mov r11, r0
	ldr r3, [r5, #16]
	cmp r2, r3
	beq .L_02008fac
	ldr r3, .L_0200905c
	adds r2, r2, r3
	adds r3, r4, r3
	str r2, [r6, #4]
	str r3, [r6, #12]
.L_02008fac:
	ldr r7, [r7]
	movs r3, #4
	mov r2, r8
	ands r7, r3
	ldr r1, [r2, #12]
	ldr r0, [r6]
	ldr r2, [r6, #4]
	cmp r7, #0
	beq .L_02008ffa
	movs r3, #128
	mov r5, r10
	lsls r3, r3, #11
	adds r0, r0, r3
	str r5, [sp, #0]
	str r5, [sp, #4]
	mov r3, r9
	movs r5, #176
	lsls r5, r5, #12
	str r3, [sp, #12]
	mov r3, r11
	str r5, [sp, #8]
	bl Func_0200015c
	ldr r0, [r6, #8]
	ldr r1, .L_02009060
	mov r3, r10
	mov r2, r8
	adds r0, r0, r1
	ldr r1, [r2, #12]
	ldr r2, [r6, #12]
	str r3, [sp, #0]
	str r3, [sp, #4]
	str r5, [sp, #8]
	ldr r3, .L_02009064
	mov r5, r9
	str r5, [sp, #12]
	bl Func_0200015c
	b .L_02009030
.L_02008ffa:
	ldr r3, .L_02009060
	movs r5, #176
	adds r0, r0, r3
	mov r3, r9
	lsls r5, r5, #12
	str r3, [sp, #12]
	ldr r3, .L_02009064
	str r7, [sp, #0]
	str r7, [sp, #4]
	str r5, [sp, #8]
	bl Func_0200015c
	ldr r0, [r6, #8]
	movs r1, #128
	lsls r1, r1, #11
	mov r2, r8
	mov r3, r9
	adds r0, r0, r1
	ldr r1, [r2, #12]
	ldr r2, [r6, #12]
	str r3, [sp, #12]
	mov r3, r11
	str r7, [sp, #0]
	str r7, [sp, #4]
	str r5, [sp, #8]
	bl Func_0200015c
.L_02009030:
	movs r0, #1
	bl WaitFrames
.L_02009036:
	mov r0, r8
	bl Func_02001f9c
	cmp r0, #0
	bne .L_02009042
	b .L_02008f26
.L_02009042:
	bl Func_02001fb4
	add sp, #72
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_02009054:
	.4byte Data_0300122c
.L_02009058:
	.4byte 0x00013333
.L_0200905c:
	.4byte 0xfff40000
.L_02009060:
	.4byte 0xfffc0000
.L_02009064:
	.4byte 0xffff8000
	.section .text.x02009068,"ax",%progbits
	.global Func_02001068
	.thumb_func
Func_02001068:
	push {lr}
	bl Func_02001fac
	movs r0, #0
	bl Func_02002044
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #221
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02009094
	movs r0, #4
	bl Func_02002004
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #221
	bl GameFlag_SetBit
	b .L_0200909a
.L_02009094:
	movs r0, #1
	bl Func_02002004
.L_0200909a:
	bl Event_ClearStatus1c6
	bl Event_WaitValue1c8Frames
	bl Func_02001fb4
	pop {pc}
	.section .text.x020090a8,"ax",%progbits
	.global Func_020010a8
	.thumb_func
Func_020010a8:
	push {r5, r6, r7, lr}
	ldr r3, .L_02009100
	movs r7, #1
	ldr r5, [r3]
	sub sp, #56
	ands r5, r7
	adds r6, r0, #0
	cmp r5, #0
	bne .L_020090fa
	bl Random16Far
	ldr r2, .L_02009104
	movs r3, #31
	ands r3, r0
	lsls r3, r3, #16
	adds r3, r3, r2
	movs r2, #179
	lsls r2, r2, #8
	add r1, sp, #16
	adds r2, #51
	str r2, [r1, #8]
	str r2, [r1, #12]
	movs r2, #7
	str r2, [r1, #4]
	str r7, [r1]
	str r1, [sp, #12]
	ldr r0, [r6, #8]
	ldr r2, [r6, #16]
	adds r0, r0, r3
	movs r3, #128
	lsls r3, r3, #13
	adds r2, r2, r3
	movs r3, #176
	lsls r3, r3, #12
	str r3, [sp, #8]
	movs r1, #0
	movs r3, #0
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl Func_0200015c
.L_020090fa:
	add sp, #56
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009100:
	.4byte Data_0300122c
.L_02009104:
	.4byte 0xfff20000
	.section .text.x02009108,"ax",%progbits
	.global Func_02001108
	.thumb_func
Func_02001108:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #100
	bl Func_02001fac
	movs r0, #0
	bl Func_02002044
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #221
	bl GameFlag_SetBit
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #9
	lsls r2, r2, #8
	movs r0, #8
	bl ObjectMotion_SetSpeedParameters
	movs r0, #8
	bl Object_GetById
	movs r3, #192
	lsls r3, r3, #8
	movs r1, #176
	movs r2, #132
	strh r3, [r0, #6]
	lsls r2, r2, #1
	movs r0, #8
	lsls r1, r1, #1
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #9
	lsls r1, r1, #6
	bl Func_02001fec
	movs r0, #176
	movs r1, #1
	movs r2, #196
	negs r1, r1
	lsls r2, r2, #16
	movs r3, #1
	lsls r0, r0, #17
	bl Motion_CamBounds
	bl Func_02001ffc
	movs r0, #8
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #78
	bl Func_02002054
	movs r0, #139
	lsls r0, r0, #2
	bl Func_02002054
	ldr r3, .L_020091bc
	mov r0, sp
	adds r0, #98
	strh r3, [r0]
	movs r1, #160
	movs r3, #128
	movs r2, #129
	lsls r3, r3, #19
	lsls r1, r1, #19
	lsls r2, r2, #24
	adds r3, #212
	adds r1, #98
	adds r2, #14
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r3, #6
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #49
	movs r1, #70
	movs r2, #19
	movs r3, #73
	bl Func_02001f74
	b .L_020091c0
.L_020091bc:
	.4byte 0x00005ccb
.L_020091c0:
	movs r0, #10
	bl WaitFrames
	ldr r3, .L_02009218
	movs r4, #160
	lsls r4, r4, #19
	movs r0, #0
	movs r2, #1
	adds r4, #98
	mov lr, r0
	mov r8, r2
	mov r11, r3
	mov r9, r0
	mov r10, r4
.L_020091dc:
	mov r0, r10
	ldrh r1, [r0]
	mov r3, r9
	mov r0, r11
	ldrsh r2, [r3, r0]
	ldr r3, .L_02009214
	movs r4, #31
	lsls r2, r2, #16
	lsls r0, r1, #16
	adds r5, r4, #0
	ands r5, r1
	lsrs r6, r2, #21
	lsrs r1, r0, #21
	lsrs r7, r2, #26
	lsrs r0, r0, #26
	lsrs r2, r2, #16
	ands r1, r3
	ands r0, r3
	ands r6, r3
	ands r7, r3
	ands r2, r4
	adds r3, r5, #0
	cmp r3, r2
	bcs .L_0200921c
	adds r5, r3, #1
	movs r3, #1
	mov lr, r3
	b .L_0200921c
.L_02009214:
	.4byte 0x0000001f
.L_02009218:
	.4byte Data_020026c0
.L_0200921c:
	lsls r3, r5, #16
	lsrs r3, r3, #16
	cmp r3, r2
	bls .L_02009234
	movs r4, #255
	lsls r4, r4, #8
	adds r4, #255
	adds r3, r3, r4
	lsls r3, r3, #16
	movs r2, #1
	asrs r5, r3, #16
	mov lr, r2
.L_02009234:
	lsls r3, r1, #16
	lsrs r3, r3, #16
	mov r12, r6
	cmp r3, r12
	bcs .L_02009248
	adds r3, #1
	lsls r3, r3, #16
	asrs r1, r3, #16
	movs r3, #1
	mov lr, r3
.L_02009248:
	lsls r3, r1, #16
	lsrs r3, r3, #16
	cmp r3, r12
	bls .L_02009260
	movs r4, #255
	lsls r4, r4, #8
	adds r4, #255
	adds r3, r3, r4
	lsls r3, r3, #16
	movs r2, #1
	asrs r1, r3, #16
	mov lr, r2
.L_02009260:
	lsls r3, r0, #16
	lsrs r3, r3, #16
	mov r12, r7
	cmp r3, r12
	bcs .L_02009274
	adds r3, #1
	lsls r3, r3, #16
	asrs r0, r3, #16
	movs r3, #1
	mov lr, r3
.L_02009274:
	lsls r3, r0, #16
	lsrs r3, r3, #16
	cmp r3, r12
	bls .L_0200928c
	movs r4, #255
	lsls r4, r4, #8
	adds r4, #255
	adds r3, r3, r4
	lsls r3, r3, #16
	movs r2, #1
	asrs r0, r3, #16
	mov lr, r2
.L_0200928c:
	lsls r3, r0, #16
	lsls r2, r1, #16
	lsrs r2, r2, #11
	lsrs r3, r3, #6
	orrs r3, r2
	lsls r2, r5, #16
	lsrs r2, r2, #16
	movs r0, #1
	orrs r2, r3
	add r8, r0
	mov r3, r10
	movs r4, #2
	strh r2, [r3]
	mov r2, r8
	add r10, r4
	add r9, r4
	cmp r2, #15
	bne .L_020091dc
	mov r3, lr
	cmp r3, #0
	beq .L_020092b8
	b .L_020091c0
.L_020092b8:
	movs r0, #30
	bl Battle_WaitMode0
	movs r4, #0
	mov r8, r4
.L_020092c2:
	movs r3, #63
	mov r0, r8
	ands r3, r0
	cmp r3, #0
	bne .L_020092d2
	movs r0, #164
	bl Func_02002054
.L_020092d2:
	mov r2, r8
	cmp r2, #119
	bhi .L_020092e2
	movs r3, #15
	ands r3, r2
	cmp r3, #0
	beq .L_020092fe
	b .L_02009344
.L_020092e2:
	mov r3, r8
	cmp r3, #239
	bhi .L_020092f4
	movs r3, #7
	mov r4, r8
	ands r3, r4
	cmp r3, #0
	beq .L_020092fe
	b .L_02009344
.L_020092f4:
	movs r3, #1
	mov r0, r8
	ands r3, r0
	cmp r3, #0
	bne .L_02009344
.L_020092fe:
	bl Random16Far
	adds r5, r0, #0
	bl Random16Far
	movs r6, #31
	add r1, sp, #56
	movs r3, #7
	str r3, [r1, #4]
	ands r5, r6
	movs r3, #168
	lsls r3, r3, #17
	lsls r5, r5, #16
	adds r2, r0, #0
	adds r5, r5, r3
	movs r3, #0
	str r3, [sp, #0]
	str r3, [sp, #4]
	ands r2, r6
	movs r4, #176
	movs r3, #128
	lsls r4, r4, #16
	lsls r3, r3, #9
	lsls r2, r2, #16
	str r3, [sp, #8]
	str r1, [sp, #12]
	adds r0, r5, #0
	adds r2, r2, r4
	movs r1, #0
	movs r3, #0
	bl Func_0200015c
	movs r0, #1
	bl WaitFrames
.L_02009344:
	movs r0, #1
	movs r2, #180
	add r8, r0
	lsls r2, r2, #1
	cmp r8, r2
	bne .L_020092c2
	movs r3, #0
	mov r8, r3
.L_02009354:
	bl Random16Far
	adds r5, r0, #0
	bl Random16Far
	movs r3, #192
	add r1, sp, #16
	lsls r3, r3, #9
	str r3, [r1, #8]
	str r3, [r1, #12]
	movs r3, #7
	str r3, [r1, #4]
	movs r3, #1
	str r3, [r1]
	movs r3, #128
	movs r6, #31
	lsls r3, r3, #9
	str r3, [sp, #0]
	adds r2, r0, #0
	movs r3, #0
	ands r5, r6
	movs r4, #168
	lsls r4, r4, #17
	str r3, [sp, #4]
	ands r2, r6
	lsls r5, r5, #16
	movs r0, #176
	movs r3, #176
	adds r5, r5, r4
	lsls r0, r0, #16
	lsls r3, r3, #12
	lsls r2, r2, #16
	adds r2, r2, r0
	str r3, [sp, #8]
	str r1, [sp, #12]
	movs r3, #0
	adds r0, r5, #0
	movs r1, #0
	bl Func_0200015c
	movs r0, #1
	bl WaitFrames
	movs r2, #1
	add r8, r2
	mov r3, r8
	cmp r3, #16
	bne .L_02009354
	movs r1, #174
	movs r3, #184
	movs r2, #0
	lsls r1, r1, #17
	lsls r3, r3, #16
	movs r0, #159
	bl Func_02001f5c
	ldr r3, .L_02009410
	adds r5, r0, #0
	str r3, [r5, #108]
	movs r0, #60
	bl Battle_WaitMode0
	adds r0, r5, #0
	movs r1, #1
	bl Func_02001f4c
	movs r0, #40
	bl Battle_WaitMode0
	ldr r3, .L_02009414
	movs r4, #166
	lsls r4, r4, #1
	adds r4, #255
	adds r3, r3, r4
	movs r2, #2
	strb r2, [r3]
	ldr r0, .L_02009418
	movs r1, #4
	bl Party_SetFields1eeAnd1f0
	movs r0, #12
	movs r1, #6
	bl Func_0200200c
	bl Func_02001fb4
	add sp, #100
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009410:
	.4byte Func_020010a8
.L_02009414:
	.4byte gPartyState
.L_02009418:
	.4byte 0x000000c6
	.section .text.x0200941c,"ax",%progbits
	.global Func_0200141c
	.thumb_func
Func_0200141c:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r9
	push {r5, r6}
	mov r6, r8
	push {r6}
	adds r5, r0, #0
	asrs r5, r5, #20
	adds r6, r1, #0
	asrs r6, r6, #20
	subs r3, r5, #1
	mov r10, r3
	subs r3, r6, #1
	mov r8, r3
	mov r9, r2
	mov r1, r10
	mov r2, r8
	movs r3, #0
	movs r0, #1
	bl Func_02000388
	adds r1, r5, #0
	mov r2, r8
	movs r3, #0
	movs r0, #1
	bl Func_02000388
	mov r1, r10
	adds r2, r6, #0
	movs r3, #0
	movs r0, #1
	bl Func_02000388
	adds r1, r5, #0
	adds r2, r6, #0
	movs r3, #0
	movs r0, #1
	bl Func_02000388
	mov r0, r9
	movs r1, #0
	movs r2, #0
	bl Func_02001fdc
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, pc}
	.2byte 0x0000
	.section .text.x02009480,"ax",%progbits
	.global Func_02001480
	.thumb_func
Func_02001480:
	push {r5, lr}
	bl Func_02001fac
	movs r0, #0
	bl Func_02002044
	movs r0, #9
	bl Object_GetById
	adds r5, r0, #0
	movs r0, #9
	bl Object_GetById
	ldr r3, [r5, #8]
	ldr r1, [r0, #16]
	movs r2, #9
	adds r0, r3, #0
	bl Func_0200141c
	movs r0, #169
	lsls r0, r0, #4
	bl GameFlag_SetBit
	bl Func_02001fb4
	pop {r5, pc}
	.section .text.x020094b4,"ax",%progbits
	.global Func_020014b4
	.thumb_func
Func_020014b4:
	push {r5, lr}
	bl Func_02001fac
	movs r0, #0
	bl Func_02002044
	movs r0, #10
	bl Object_GetById
	adds r5, r0, #0
	movs r0, #10
	bl Object_GetById
	ldr r3, [r5, #8]
	ldr r1, [r0, #16]
	movs r2, #10
	adds r0, r3, #0
	bl Func_0200141c
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #145
	bl GameFlag_SetBit
	bl Func_02001fb4
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x020094ec,"ax",%progbits
	.global Func_020014ec
	.thumb_func
Func_020014ec:
	push {r5, lr}
	bl Func_02001fac
	movs r0, #0
	bl Func_02002044
	movs r0, #11
	bl Object_GetById
	adds r5, r0, #0
	movs r0, #11
	bl Object_GetById
	ldr r3, [r5, #8]
	ldr r1, [r0, #16]
	movs r2, #11
	adds r0, r3, #0
	bl Func_0200141c
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #146
	bl GameFlag_SetBit
	bl Func_02001fb4
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x02009524,"ax",%progbits
	.global Func_02001524
	.thumb_func
Func_02001524:
	push {r5, lr}
	bl Func_02001fac
	movs r0, #0
	bl Func_02002044
	movs r0, #12
	bl Object_GetById
	adds r5, r0, #0
	movs r0, #12
	bl Object_GetById
	ldr r3, [r5, #8]
	ldr r1, [r0, #16]
	movs r2, #12
	adds r0, r3, #0
	bl Func_0200141c
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #147
	bl GameFlag_SetBit
	bl Func_02001fb4
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x0200955c,"ax",%progbits
	.global Func_0200155c
	.thumb_func
Func_0200155c:
	push {r5, lr}
	bl Func_02001fac
	movs r0, #0
	bl Func_02002044
	movs r0, #13
	bl Object_GetById
	adds r5, r0, #0
	movs r0, #13
	bl Object_GetById
	ldr r3, [r5, #8]
	ldr r1, [r0, #16]
	movs r2, #13
	adds r0, r3, #0
	bl Func_0200141c
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #148
	bl GameFlag_SetBit
	bl Func_02001fb4
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x02009594,"ax",%progbits
	.global Func_02001594
	.thumb_func
Func_02001594:
	push {r5, lr}
	bl Func_02001fac
	movs r0, #0
	bl Func_02002044
	movs r0, #9
	bl Object_GetById
	adds r5, r0, #0
	movs r0, #9
	bl Object_GetById
	ldr r3, [r5, #8]
	ldr r1, [r0, #16]
	movs r2, #9
	adds r0, r3, #0
	bl Func_0200141c
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #149
	bl GameFlag_SetBit
	bl Func_02001fb4
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x020095cc,"ax",%progbits
	.global Func_020015cc
	.thumb_func
Func_020015cc:
	push {lr}
	bl Func_02001fac
	movs r0, #0
	bl Func_02002044
	movs r0, #4
	bl Func_02002004
	bl Event_ClearStatus1c6
	bl Event_WaitValue1c8Frames
	bl Func_02001fb4
	pop {pc}
	.section .text.x020095ec,"ax",%progbits
	.global Func_020015ec
	.thumb_func
Func_020015ec:
	push {lr}
	bl Func_02001fac
	movs r0, #0
	bl Func_02002044
	movs r0, #5
	bl Func_02002004
	bl Event_ClearStatus1c6
	bl Event_WaitValue1c8Frames
	bl Func_02001fb4
	pop {pc}
	.section .text.x0200960c,"ax",%progbits
	.global Func_0200160c
	.thumb_func
Func_0200160c:
	push {lr}
	ldr r1, .L_0200963c
	movs r0, #240
	lsls r0, r0, #1
	adds r3, r1, r0
	movs r0, #0
	ldrsh r2, [r3, r0]
	ldr r3, .L_02009640
	cmp r2, r3
	bne .L_02009624
	ldr r0, .L_02009644
	b .L_02009638
.L_02009624:
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r1, r2
	movs r0, #0
	ldrsh r3, [r3, r0]
	cmp r3, #4
	bgt .L_02009636
	ldr r0, .L_02009648
	b .L_02009638
.L_02009636:
	ldr r0, .L_0200964c
.L_02009638:
	pop {pc}
	.2byte 0x0000
.L_0200963c:
	.4byte gPartyState
.L_02009640:
	.4byte 0x000000c5
.L_02009644:
	.4byte Data_020022d0
.L_02009648:
	.4byte Data_0200242c
.L_0200964c:
	.4byte Data_0200251c
	.section .text.x02009650,"ax",%progbits
	.global Func_02001650
	.thumb_func
Func_02001650:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r2, #192
	lsls r2, r2, #18
	ldr r3, [r2, #32]
	ldr r0, .L_02009798
	movs r1, #188
	lsls r1, r1, #1
	adds r1, r1, r3
	adds r3, r2, #0
	ldr r2, [r2, #108]
	mov r10, r0
	movs r0, #179
	adds r3, #128
	lsls r0, r0, #1
	mov r8, r1
	ldr r1, [r3]
	adds r3, r2, r0
	movs r0, #0
	ldrsh r3, [r3, r0]
	cmp r3, #0
	beq .L_02009682
	b .L_02009894
.L_02009682:
	movs r0, #168
	lsls r0, r0, #6
	adds r0, #1
	adds r3, r1, r0
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	beq .L_02009696
	b .L_02009894
.L_02009696:
	movs r1, #217
	lsls r1, r1, #1
	adds r3, r2, r1
	ldrh r3, [r3]
	cmp r3, #0
	bne .L_020096a4
	b .L_02009894
.L_020096a4:
	ldr r3, .L_0200979c
	ldr r1, .L_020097a0
	ldr r2, [r3]
	ldr r3, [r1]
	subs r2, r2, r3
	ldr r3, .L_020097a4
	ldr r3, [r3]
	cmp r2, r3
	bls .L_0200970c
	movs r3, #160
	lsls r3, r3, #19
	adds r3, #124
	ldrh r3, [r3]
	movs r5, #14
	lsls r3, r3, #16
	asrs r4, r3, #16
	ldr r3, .L_020097a8
	movs r0, #24
	movs r2, #26
	ldrsh r7, [r3, r2]
	adds r6, r3, #0
	movs r1, #26
.L_020096d0:
	movs r2, #160
	lsls r2, r2, #19
	lsls r3, r5, #1
	adds r2, #96
	adds r2, r2, r3
	mov r12, r2
	movs r2, #160
	lsls r2, r2, #19
	adds r2, #94
	adds r3, r3, r2
	ldrh r3, [r3]
	mov r2, r12
	strh r3, [r2]
	subs r5, #1
	ldrh r3, [r6, r0]
	subs r0, #2
	strh r3, [r6, r1]
	subs r1, #2
	cmp r5, #0
	bne .L_020096d0
	movs r3, #160
	lsls r3, r3, #19
	adds r3, #98
	strh r4, [r3]
	ldr r3, .L_020097a8
	ldr r2, .L_020097a0
	strh r7, [r3]
	ldr r3, .L_0200979c
	ldr r3, [r3]
	str r3, [r2]
.L_0200970c:
	ldr r3, .L_0200979c
	movs r2, #1
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	beq .L_0200971a
	b .L_0200981c
.L_0200971a:
	movs r3, #160
	lsls r3, r3, #19
	adds r3, #156
	ldrh r3, [r3]
	movs r5, #14
	lsls r3, r3, #16
	asrs r4, r3, #16
.L_02009728:
	movs r0, #160
	movs r1, #160
	lsls r0, r0, #19
	lsls r1, r1, #19
	lsls r3, r5, #1
	adds r0, #128
	adds r1, #126
	adds r2, r3, r0
	adds r3, r3, r1
	ldrh r3, [r3]
	subs r5, #1
	strh r3, [r2]
	cmp r5, #0
	bne .L_02009728
	movs r3, #160
	lsls r3, r3, #19
	adds r3, #130
	strh r4, [r3]
	ldr r3, .L_0200979c
	movs r2, #3
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	bne .L_0200981c
	movs r3, #160
	lsls r3, r3, #19
	adds r3, #30
	ldrh r3, [r3]
	ldr r2, .L_02009794
	lsls r5, r3, #16
	movs r0, #128
	lsrs r6, r5, #21
	movs r7, #31
	lsrs r5, r5, #26
	lsls r0, r0, #2
	ands r7, r3
	ands r6, r2
	ands r5, r2
	bl GameFlag_Test
	lsls r6, r6, #16
	lsls r4, r7, #16
	lsls r1, r5, #16
	cmp r0, #0
	bne .L_020097d0
	ldr r0, .L_020097ac
	lsrs r3, r4, #16
	ldrh r2, [r0]
	adds r3, r3, r2
	lsls r3, r3, #16
	asrs r7, r3, #16
	lsrs r3, r3, #16
	b .L_020097b0
	.2byte 0x0000
.L_02009794:
	.4byte 0x0000001f
.L_02009798:
	.4byte Data_02002580
.L_0200979c:
	.4byte Data_0300122c
.L_020097a0:
	.4byte Data_020022c8
.L_020097a4:
	.4byte Data_020022cc
.L_020097a8:
	.4byte Data_020026c0
.L_020097ac:
	.4byte Data_0200257c
.L_020097b0:
	cmp r3, #17
	bhi .L_020097bc
	ldr r3, .L_020097c8
	movs r7, #18
	strh r3, [r0]
	b .L_02009804
.L_020097bc:
	cmp r3, #31
	bls .L_02009804
	ldr r3, .L_020097cc
	movs r7, #31
	strh r3, [r0]
	b .L_02009804
.L_020097c8:
	.4byte 0x00000001
.L_020097cc:
	.4byte 0xffffffff
.L_020097d0:
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #181
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r0, #0
	ldrsh r3, [r3, r0]
	cmp r3, #200
	bne .L_020097f8
	lsrs r3, r4, #16
	cmp r3, #0
	beq .L_02009804
	movs r2, #255
	lsls r2, r2, #8
	adds r2, #255
	adds r3, r3, r2
	lsls r3, r3, #16
	asrs r7, r3, #16
	b .L_02009804
.L_020097f8:
	cmp r3, #201
	bne .L_02009804
	lsrs r3, r4, #16
	cmp r3, #29
	bhi .L_02009804
	adds r7, r3, #1
.L_02009804:
	lsrs r3, r6, #11
	lsrs r1, r1, #6
	orrs r1, r3
	lsls r3, r7, #16
	movs r0, #160
	lsrs r3, r3, #16
	movs r2, #31
	lsls r0, r0, #19
	ands r3, r2
	adds r0, #30
	orrs r1, r3
	strh r1, [r0]
.L_0200981c:
	movs r2, #192
	lsls r2, r2, #18
	adds r2, #128
	ldr r1, [r2]
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	movs r0, #160
	lsls r2, r2, #24
	adds r3, #212
	lsls r0, r0, #19
	adds r2, #112
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #0
	bl Func_0200201c
	movs r3, #0
	mov r0, r8
	str r3, [r0, #28]
	movs r5, #0
.L_0200984a:
	ldr r3, .L_0200989c
	mov r2, r8
	ldrb r0, [r3]
	movs r1, #6
	ldrsh r3, [r2, r1]
	adds r0, r5, r0
	adds r0, r0, r3
	lsls r0, r0, #9
	bl Math_Sine
	mov r2, r8
	movs r1, #2
	ldrsh r3, [r2, r1]
	asrs r0, r0, #15
	adds r3, r3, r0
	movs r1, #2
	mov r0, r10
	adds r5, #1
	strh r3, [r0]
	add r10, r1
	cmp r5, #160
	bne .L_0200984a
	ldr r2, .L_020098a0
	movs r1, #128
	ldrh r3, [r2]
	lsls r1, r1, #19
	adds r1, #20
	mov r10, r2
	strh r3, [r1]
	movs r3, #128
	mov r0, r10
	lsls r3, r3, #19
	adds r3, #176
	adds r0, #2
	ldr r2, .L_020098a4
	stmia r3!, {r0, r1, r2}
	subs r3, #12
.L_02009894:
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_0200989c:
	.4byte Data_0300122c
.L_020098a0:
	.4byte Data_02002580
.L_020098a4:
	.4byte 0xa2600001
	.section .text.x020098a8,"ax",%progbits
	.global Func_020018a8
	.thumb_func
Func_020018a8:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r8
	push {r5, r6}
	adds r6, r2, #0
	mov r8, r0
	adds r0, r6, #0
	adds r5, r1, #0
	bl Object_GetById
	adds r0, #89
	ldrb r2, [r0]
	movs r3, #16
	orrs r3, r2
	strb r3, [r0]
	mov r3, r8
	lsls r1, r3, #19
	movs r3, #128
	lsls r3, r3, #12
	lsls r2, r5, #19
	adds r1, r1, r3
	adds r2, r2, r3
	adds r0, r6, #0
	bl Func_02001fdc
	mov r3, r8
	asrs r3, r3, #1
	movs r6, #255
	mov r8, r3
	lsls r6, r6, #8
	asrs r5, r5, #1
	mov r1, r8
	adds r2, r5, #0
	adds r3, r6, #0
	movs r0, #1
	bl Func_02000388
	movs r3, #1
	add r3, r8
	mov r10, r3
	mov r1, r10
	adds r2, r5, #0
	adds r3, r6, #0
	adds r5, #1
	movs r0, #1
	bl Func_02000388
	mov r1, r8
	adds r2, r5, #0
	adds r3, r6, #0
	movs r0, #1
	bl Func_02000388
	mov r1, r10
	adds r2, r5, #0
	adds r3, r6, #0
	movs r0, #1
	bl Func_02000388
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, pc}
	.2byte 0x0000
	.section .text.x02009928,"ax",%progbits
	.global Func_02001928
	.thumb_func
Func_02001928:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	mov r8, r0
	mov r10, r1
	movs r0, #192
	movs r1, #192
	lsls r0, r0, #11
	lsls r1, r1, #8
	sub sp, #56
	adds r7, r2, #0
	bl Func_02001fec
	mov r2, r8
	lsls r6, r2, #19
	movs r3, #128
	mov r2, r10
	lsls r3, r3, #12
	lsls r5, r2, #19
	adds r5, r5, r3
	adds r6, r6, r3
	movs r1, #1
	movs r3, #1
	adds r2, r5, #0
	negs r1, r1
	adds r0, r6, #0
	bl Motion_CamBounds
	bl Func_02001ffc
	movs r0, #144
	bl Func_02002054
	movs r1, #128
	movs r2, #128
	movs r0, #0
	lsls r1, r1, #13
	lsls r2, r2, #9
	bl Func_02001f8c
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	negs r0, r0
	negs r1, r1
	adds r2, #102
	bl Func_02001f8c
	ldr r3, .L_02009a10
	add r2, sp, #16
	str r3, [r2, #8]
	movs r3, #192
	lsls r3, r3, #9
	str r3, [r2, #12]
	movs r3, #1
	str r3, [r2]
	movs r3, #138
	lsls r3, r3, #1
	strh r3, [r2, #24]
	movs r3, #208
	lsls r3, r3, #13
	movs r1, #0
	str r3, [sp, #8]
	str r2, [sp, #12]
	movs r3, #0
	adds r2, r5, #0
	adds r0, r6, #0
	str r1, [sp, #0]
	str r1, [sp, #4]
	bl Func_0200015c
	adds r0, r7, #0
	bl Object_GetById
	movs r3, #200
	lsls r3, r3, #5
	adds r3, #153
	str r3, [r0, #28]
	mov r1, r10
	mov r0, r8
	adds r2, r7, #0
	bl Func_020018a8
	movs r5, #0
.L_020099d4:
	adds r0, r7, #0
	bl Object_GetById
	movs r2, #200
	ldr r3, [r0, #28]
	lsls r2, r2, #5
	adds r2, #153
	adds r3, r3, r2
	str r3, [r0, #28]
	adds r5, #1
	movs r0, #1
	bl Battle_WaitMode0
	cmp r5, #10
	bne .L_020099d4
	adds r0, r7, #0
	bl Object_GetById
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r0, #28]
	movs r0, #20
	bl Battle_WaitMode0
	add sp, #56
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009a10:
	.4byte 0x00013333
	.section .text.x02009a14,"ax",%progbits
	.global Func_02001a14
	.thumb_func
Func_02001a14:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #96
	bl Func_02001fac
	movs r0, #0
	bl Func_02002044
	movs r0, #0
	bl Func_02002054
	movs r3, #6
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #49
	movs r1, #70
	movs r2, #19
	movs r3, #73
	bl Func_02001f74
	movs r1, #174
	movs r3, #184
	lsls r1, r1, #17
	movs r2, #0
	lsls r3, r3, #16
	movs r0, #159
	bl Func_02001f5c
	ldr r3, .L_02009b14
	mov r8, r0
	str r3, [r0, #108]
	movs r1, #1
	movs r0, #176
	movs r2, #196
	movs r3, #0
	lsls r2, r2, #16
	negs r1, r1
	lsls r0, r0, #17
	bl Motion_CamBounds
	bl Event_SetStatus1c6
	movs r0, #1
	bl WaitFrames
	movs r1, #200
	lsls r1, r1, #4
	ldr r0, .L_02009b18
	bl Scheduler_AddOrUpdateCallback
	bl Event_WaitValue1c8Frames
	movs r0, #60
	bl Battle_WaitMode0
	movs r0, #21
	movs r1, #23
	movs r2, #9
	bl Func_02001928
	movs r0, #37
	movs r1, #17
	movs r2, #10
	bl Func_02001928
	movs r0, #47
	movs r1, #27
	movs r2, #12
	bl Func_02001928
	movs r0, #59
	movs r1, #19
	movs r2, #13
	bl Func_02001928
	bl Func_02001f94
	movs r0, #176
	movs r1, #1
	movs r2, #196
	negs r1, r1
	lsls r2, r2, #16
	movs r3, #1
	lsls r0, r0, #17
	bl Motion_CamBounds
	bl Func_02001ffc
	movs r0, #128
	lsls r0, r0, #2
	bl GameFlag_SetBit
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r1, #181
	lsls r1, r1, #1
	movs r0, #128
	adds r2, r3, r1
	lsls r0, r0, #2
	movs r3, #200
	strh r3, [r2]
	adds r0, #18
	bl Func_02002054
	movs r0, #40
	bl Battle_WaitMode0
	movs r7, #0
.L_02009af0:
	movs r3, #31
	ands r3, r7
	cmp r3, #0
	bne .L_02009afe
	movs r0, #164
	bl Func_02002054
.L_02009afe:
	cmp r7, #119
	bhi .L_02009b06
	movs r3, #15
	b .L_02009b0c
.L_02009b06:
	cmp r7, #239
	bhi .L_02009b1c
	movs r3, #7
.L_02009b0c:
	ands r3, r7
	cmp r3, #0
	beq .L_02009b24
	b .L_02009b78
.L_02009b14:
	.4byte Func_020010a8
.L_02009b18:
	.4byte Func_02001650
.L_02009b1c:
	movs r3, #1
	ands r3, r7
	cmp r3, #0
	bne .L_02009b78
.L_02009b24:
	bl Random16Far
	adds r5, r0, #0
	bl Random16Far
	movs r3, #204
	lsls r3, r3, #8
	add r1, sp, #56
	adds r3, #204
	str r3, [r1, #8]
	str r3, [r1, #12]
	movs r3, #1
	str r3, [r1]
	movs r6, #31
	movs r3, #7
	str r3, [r1, #4]
	ands r5, r6
	movs r3, #168
	adds r2, r0, #0
	lsls r3, r3, #17
	lsls r5, r5, #16
	adds r5, r5, r3
	ands r2, r6
	movs r3, #176
	lsls r3, r3, #16
	lsls r2, r2, #16
	adds r2, r2, r3
	movs r3, #0
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r3, #176
	lsls r3, r3, #12
	str r3, [sp, #8]
	str r1, [sp, #12]
	adds r0, r5, #0
	movs r1, #0
	movs r3, #0
	bl Func_0200015c
	movs r0, #1
	bl WaitFrames
.L_02009b78:
	movs r1, #180
	adds r7, #1
	lsls r1, r1, #1
	cmp r7, r1
	bne .L_02009af0
	movs r7, #0
.L_02009b84:
	bl Random16Far
	adds r5, r0, #0
	bl Random16Far
	movs r3, #192
	add r1, sp, #16
	lsls r3, r3, #9
	str r3, [r1, #8]
	str r3, [r1, #12]
	movs r3, #7
	str r3, [r1, #4]
	movs r6, #31
	movs r3, #1
	str r3, [r1]
	ands r5, r6
	movs r3, #168
	adds r2, r0, #0
	lsls r3, r3, #17
	lsls r5, r5, #16
	adds r5, r5, r3
	ands r2, r6
	movs r3, #176
	lsls r3, r3, #16
	lsls r2, r2, #16
	adds r2, r2, r3
	movs r3, #128
	lsls r3, r3, #9
	str r3, [sp, #0]
	movs r3, #0
	str r3, [sp, #4]
	movs r3, #176
	lsls r3, r3, #12
	str r3, [sp, #8]
	str r1, [sp, #12]
	adds r0, r5, #0
	movs r1, #0
	movs r3, #0
	bl Func_0200015c
	adds r7, #1
	movs r0, #1
	bl WaitFrames
	cmp r7, #16
	bne .L_02009b84
	mov r0, r8
	bl Func_02001f64
	movs r0, #151
	lsls r0, r0, #1
	adds r0, #255
	bl Func_02002054
.L_02009bf0:
	movs r0, #10
	bl WaitFrames
	movs r6, #160
	lsls r6, r6, #19
	movs r5, #0
	movs r7, #1
	adds r6, #98
.L_02009c00:
	ldrh r3, [r6]
	ldr r1, .L_02009c34
	movs r4, #31
	lsls r2, r3, #16
	ands r4, r3
	lsrs r0, r2, #21
	adds r3, r4, #0
	lsrs r2, r2, #26
	ands r0, r1
	ands r2, r1
	cmp r3, #10
	bhi .L_02009c1c
	adds r4, r3, #1
	movs r5, #1
.L_02009c1c:
	lsls r3, r4, #16
	lsrs r3, r3, #16
	cmp r3, #11
	bls .L_02009c38
	movs r1, #255
	lsls r1, r1, #8
	adds r1, #255
	adds r3, r3, r1
	lsls r3, r3, #16
	asrs r4, r3, #16
	movs r5, #1
	b .L_02009c38
.L_02009c34:
	.4byte 0x0000001f
.L_02009c38:
	lsls r3, r0, #16
	lsrs r3, r3, #16
	cmp r3, #5
	bhi .L_02009c48
	adds r3, #1
	lsls r3, r3, #16
	asrs r0, r3, #16
	movs r5, #1
.L_02009c48:
	lsls r3, r0, #16
	lsrs r3, r3, #16
	cmp r3, #6
	bls .L_02009c5e
	movs r1, #255
	lsls r1, r1, #8
	adds r1, #255
	adds r3, r3, r1
	lsls r3, r3, #16
	asrs r0, r3, #16
	movs r5, #1
.L_02009c5e:
	lsls r3, r2, #16
	lsrs r3, r3, #16
	cmp r3, #22
	bhi .L_02009c6e
	adds r3, #1
	lsls r3, r3, #16
	asrs r2, r3, #16
	movs r5, #1
.L_02009c6e:
	lsls r3, r2, #16
	lsrs r3, r3, #16
	cmp r3, #23
	bls .L_02009c84
	movs r2, #255
	lsls r2, r2, #8
	adds r2, #255
	adds r3, r3, r2
	lsls r3, r3, #16
	asrs r2, r3, #16
	movs r5, #1
.L_02009c84:
	lsls r3, r2, #16
	lsls r2, r0, #16
	lsrs r2, r2, #11
	lsrs r3, r3, #6
	orrs r3, r2
	lsls r2, r4, #16
	lsrs r2, r2, #16
	orrs r2, r3
	adds r7, #1
	strh r2, [r6]
	adds r6, #2
	cmp r7, #14
	bne .L_02009c00
	ldr r3, .L_02009cd0
	ldr r1, .L_02009cd4
	ldr r2, [r3]
	movs r3, #1
	ands r2, r3
	ldr r3, [r1]
	adds r3, r3, r2
	str r3, [r1]
	cmp r5, #0
	bne .L_02009bf0
	ldr r0, .L_02009cd8
	bl Scheduler_RemoveCallbackFar
	bl Func_0200204c
	movs r0, #148
	lsls r0, r0, #4
	bl GameFlag_SetBit
	bl Func_02001fb4
	add sp, #96
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_02009cd0:
	.4byte Data_0300122c
.L_02009cd4:
	.4byte Data_020022cc
.L_02009cd8:
	.4byte Func_02001650
	.section .text.x02009cdc,"ax",%progbits
	.global Func_02001cdc
	.thumb_func
Func_02001cdc:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r0, #214
	lsls r0, r0, #1
	movs r2, #128
	adds r3, r3, r0
	lsls r2, r2, #1
	ldr r1, .L_02009d68
	str r2, [r3]
	adds r2, #224
	adds r3, r1, r2
	movs r0, #0
	ldrsh r2, [r3, r0]
	ldr r3, .L_02009d6c
	cmp r2, r3
	bne .L_02009d88
	ldr r7, .L_02009d70
	ldr r6, .L_02009d74
	ldr r3, [r7]
	ldr r5, .L_02009d78
	str r3, [r6]
	movs r3, #1
	str r3, [r5]
	movs r0, #160
	movs r3, #128
	movs r2, #128
	lsls r3, r3, #19
	lsls r0, r0, #19
	lsls r2, r2, #24
	adds r3, #212
	adds r0, #98
	ldr r1, .L_02009d7c
	adds r2, #14
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r0, #128
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02009d34
	b .L_02009e9e
.L_02009d34:
	ldr r3, .L_02009d80
	movs r0, #1
	ldmia r3!, {r2}
	movs r1, #98
	str r2, [r6]
	ldmia r3!, {r2}
	str r2, [r5]
	ldr r3, [r3]
	str r3, [r7]
.L_02009d46:
	movs r3, #160
	lsls r3, r3, #19
	adds r2, r1, r3
	ldr r3, .L_02009d64
	adds r0, #1
	strh r3, [r2]
	adds r1, #2
	cmp r0, #14
	bne .L_02009d46
	movs r1, #144
	ldr r0, .L_02009d84
	lsls r1, r1, #3
	bl Scheduler_AddOrUpdateCallback
	b .L_02009e9e
.L_02009d64:
	.4byte 0x00005ccb
.L_02009d68:
	.4byte gPartyState
.L_02009d6c:
	.4byte 0x000000c5
.L_02009d70:
	.4byte Data_0300122c
.L_02009d74:
	.4byte Data_020022c8
.L_02009d78:
	.4byte Data_020022cc
.L_02009d7c:
	.4byte Data_020026c0
.L_02009d80:
	.4byte gSceneState
.L_02009d84:
	.4byte Func_02000c50
.L_02009d88:
	ldr r3, .L_02009eac
	cmp r2, r3
	beq .L_02009d90
	b .L_02009e9e
.L_02009d90:
	ldr r3, .L_02009eb0
	ldr r2, .L_02009eb4
	ldr r3, [r3]
	movs r0, #241
	str r3, [r2]
	ldr r2, .L_02009eb8
	movs r3, #0
	str r3, [r2]
	lsls r0, r0, #1
	adds r3, r1, r0
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #4
	bgt .L_02009e86
	movs r0, #148
	lsls r0, r0, #4
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02009dd2
	movs r3, #128
	movs r0, #160
	movs r2, #128
	lsls r3, r3, #19
	lsls r0, r0, #19
	lsls r2, r2, #24
	adds r3, #212
	adds r0, #98
	ldr r1, .L_02009ebc
	adds r2, #14
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	b .L_02009e66
.L_02009dd2:
	movs r0, #169
	lsls r0, r0, #4
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02009de8
	movs r0, #21
	movs r1, #23
	movs r2, #9
	bl Func_020018a8
.L_02009de8:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #145
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02009e00
	movs r0, #37
	movs r1, #17
	movs r2, #10
	bl Func_020018a8
.L_02009e00:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #147
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02009e18
	movs r0, #47
	movs r1, #27
	movs r2, #12
	bl Func_020018a8
.L_02009e18:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #148
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02009e30
	movs r0, #59
	movs r1, #19
	movs r2, #13
	bl Func_020018a8
.L_02009e30:
	movs r3, #192
	movs r0, #160
	lsls r3, r3, #18
	lsls r0, r0, #19
	adds r0, #30
	adds r3, #128
	ldrh r2, [r0]
	ldr r1, [r3]
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #224
	ands r3, r2
	strh r3, [r0]
	movs r2, #132
	movs r3, #128
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	subs r0, #30
	adds r2, #112
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #0
	bl Func_0200201c
.L_02009e66:
	ldr r3, .L_02009ec0
	movs r0, #241
	lsls r0, r0, #1
	adds r3, r3, r0
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #4
	bne .L_02009e9e
	subs r0, #217
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02009e9e
	bl Func_02001a14
	b .L_02009e9e
.L_02009e86:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #149
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02009e9e
	movs r0, #89
	movs r1, #85
	movs r2, #9
	bl Func_020018a8
.L_02009e9e:
	movs r1, #200
	lsls r1, r1, #4
	ldr r0, .L_02009ec4
	bl Scheduler_AddOrUpdateCallback
	movs r0, #0
	pop {r5, r6, r7, pc}
.L_02009eac:
	.4byte 0x000000c6
.L_02009eb0:
	.4byte Data_0300122c
.L_02009eb4:
	.4byte Data_020022c8
.L_02009eb8:
	.4byte Data_020022cc
.L_02009ebc:
	.4byte Data_020026c0
.L_02009ec0:
	.4byte gPartyState
.L_02009ec4:
	.4byte Func_02001650
	.section .text.x02009ec8,"ax",%progbits
	.global Func_02001ec8
	.thumb_func
Func_02001ec8:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #128
	sub sp, #4
	ldr r1, [r3]
	movs r3, #1
	mov r0, sp
	negs r3, r3
	str r3, [r0]
	movs r2, #133
	movs r3, #128
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	adds r2, #112
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #0
	bl Func_02002024
	movs r0, #0
	add sp, #4
	pop {pc}
	.section .rodata.x0200a05c,"a",%progbits
.L_0200a05c:
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
.L_0200a098:
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
.L_0200a0d4:
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
	.global Data_02002110
Data_02002110:
	.4byte 0x18101810
	.4byte 0x00001810
	.global Data_02002118
Data_02002118:
	.4byte 0x02a00000
	.4byte 0x02c00000
	.4byte 0x03800000
	.4byte 0x01400000
	.4byte 0x02000000
	.4byte 0x00c00000
	.4byte 0x01400000
	.4byte 0x01a00000
	.4byte 0x01400000
	.4byte 0x03400000
	.4byte 0x00600000
	.4byte 0x00a00000
	.global Data_02002148
Data_02002148:
	.4byte .L_0200a05c
	.4byte .L_0200a098
	.4byte .L_0200a0d4
.L_0200a154:
	.4byte 0x0000002e
	.4byte Func_0200033c
	.4byte 0x00000011
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
	.global gSceneExits
gSceneExits:
	.4byte 0x000000c5
	.4byte 0x1012a002
	.4byte 0xffffffff
	.4byte 0x102010c6
	.4byte 0xffffffff
	.4byte 0x103010c5
	.4byte 0xffffffff
	.4byte 0x1041402a
	.4byte 0xffffffff
	.4byte 0x000000c6
	.4byte 0x101020c5
	.4byte 0xffffffff
	.4byte 0x102060c6
	.4byte 0xffffffff
	.4byte 0x1032c002
	.4byte 0xffffffff
	.4byte 0x1042b002
	.4byte 0xffffffff
	.4byte 0x105020c6
	.4byte 0xffffffff
	.4byte 0x000001ff
	.global Data_020021e4
Data_020021e4:
	.4byte 0xffff0008
	.4byte .L_0200a154
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x0002c000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02002214
Data_02002214:
	.4byte 0xffff0008
	.4byte .L_0200a154
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x0002c000
	.4byte 0xffff01a8
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0002c000
	.4byte 0xffff01a8
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0002c000
	.4byte 0xffff01a8
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0002c000
	.4byte 0xffff01a8
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0002c000
	.4byte 0xffff01a8
	.4byte 0x00000007
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
	.global Data_020022bc
Data_020022bc:
	.2byte 0x0000
	.global Data_020022be
Data_020022be:
	.2byte 0x0000
	.global Data_020022c0
Data_020022c0:
	.4byte 0x00000000
	.global Data_020022c4
Data_020022c4:
	.4byte 0x00000000
	.global Data_020022c8
Data_020022c8:
	.4byte 0x00000000
	.global Data_020022cc
Data_020022cc:
	.4byte 0x00000000
	.global Data_020022d0
Data_020022d0:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000002
	.4byte 0xffff0031
	.4byte Func_02001068
	.4byte 0x00000002
	.4byte 0xffff001e
	.4byte Func_0200054c
	.4byte 0x00000002
	.4byte 0xffff001f
	.4byte Func_0200054c
	.4byte 0x00000002
	.4byte 0xffff0020
	.4byte Func_0200054c
	.4byte 0x00000002
	.4byte 0xffff0021
	.4byte Func_0200054c
	.4byte 0x00000002
	.4byte 0xffff0022
	.4byte Func_0200054c
	.4byte 0x00000002
	.4byte 0xffff0023
	.4byte Func_0200054c
	.4byte 0x00000002
	.4byte 0xffff0024
	.4byte Func_0200054c
	.4byte 0x00000002
	.4byte 0xffff0025
	.4byte Func_0200054c
	.4byte 0x00000002
	.4byte 0xffff0028
	.4byte Func_0200043c
	.4byte 0x00000002
	.4byte 0xffff0029
	.4byte Func_0200043c
	.4byte 0x00000002
	.4byte 0xffff002c
	.4byte Func_02000ef0
	.4byte 0x00000002
	.4byte 0x0200002d
	.4byte Func_02000818
	.4byte 0x00000002
	.4byte 0x0200002e
	.4byte Func_02000818
	.4byte 0x00000002
	.4byte 0x0200002f
	.4byte Func_02000818
	.4byte 0x00000002
	.4byte 0x02000030
	.4byte Func_02000818
	.4byte 0x00000002
	.4byte 0x02000032
	.4byte Func_02000a80
	.4byte 0x00000002
	.4byte 0x02000033
	.4byte Func_02000a80
	.4byte 0x00000002
	.4byte 0x02000034
	.4byte Func_02000a80
	.4byte 0x00000002
	.4byte 0x02000035
	.4byte Func_02000a80
	.4byte 0x00000002
	.4byte 0x02000036
	.4byte Func_02000a80
	.4byte 0x00000002
	.4byte 0x02000037
	.4byte Func_02000a80
	.4byte 0x00000002
	.4byte 0x02000038
	.4byte Func_02000a80
	.4byte 0x00000002
	.4byte 0x02000039
	.4byte Func_02000a80
	.4byte 0x00000006
	.4byte 0xffff00c8
	.4byte Func_02000c94
	.4byte 0x00000006
	.4byte 0xffff00c9
	.4byte Func_02000da0
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_0200242c
Data_0200242c:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000002
	.4byte 0x09400005
	.4byte Func_02001108
	.4byte 0x00000002
	.4byte 0xffff001e
	.4byte Func_0200054c
	.4byte 0x00000002
	.4byte 0xffff001f
	.4byte Func_0200054c
	.4byte 0x00000002
	.4byte 0xffff0020
	.4byte Func_0200054c
	.4byte 0x00000002
	.4byte 0xffff0021
	.4byte Func_0200054c
	.4byte 0x00000002
	.4byte 0xffff0022
	.4byte Func_0200054c
	.4byte 0x00000002
	.4byte 0xffff0023
	.4byte Func_0200054c
	.4byte 0x00000002
	.4byte 0xffff0024
	.4byte Func_0200054c
	.4byte 0x00000002
	.4byte 0xffff0025
	.4byte Func_0200054c
	.4byte 0x00000002
	.4byte 0xffff0030
	.4byte Func_02000818
	.4byte 0x00000002
	.4byte 0xffff0029
	.4byte Func_0200043c
	.4byte 0x00009815
	.4byte 0xffff0009
	.4byte Func_02001480
	.4byte 0x00009815
	.4byte 0xffff000a
	.4byte Func_020014b4
	.4byte 0x00009815
	.4byte 0xffff000b
	.4byte Func_020014ec
	.4byte 0x00009815
	.4byte 0xffff000c
	.4byte Func_02001524
	.4byte 0x00009815
	.4byte 0xffff000d
	.4byte Func_0200155c
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_0200251c
Data_0200251c:
	.4byte 0x00000002
	.4byte 0xffff0001
	.4byte Func_020015cc
	.4byte 0x00000002
	.4byte 0xffff0002
	.4byte Func_020015ec
	.4byte 0x00000002
	.4byte 0xffff002d
	.4byte Func_02000818
	.4byte 0x00000002
	.4byte 0xffff002e
	.4byte Func_02000818
	.4byte 0x00000002
	.4byte 0xffff002f
	.4byte Func_02000818
	.4byte 0x00000002
	.4byte 0xffff0030
	.4byte Func_02000818
	.4byte 0x00009815
	.4byte 0xffff0009
	.4byte Func_02001594
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_0200257c
Data_0200257c:
	.2byte 0x0001
	.section .bss,"aw",%nobits
	.space 0x00000002
	.global Data_02002580
Data_02002580:
	.space 0x00000140
	.global Data_020026c0
Data_020026c0:
