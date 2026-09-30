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
	bl Func_02002de4
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
	bl Func_02002e84
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
	bl Func_02002de4
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
	bl Func_02002e84
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
	bl Func_02002de4
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
	bl Func_02002dd4
	ldr r2, .L_02008330
	mov r3, r10
	ands r3, r5
	lsls r3, r3, #2
	ldr r1, [r2, r3]
	adds r0, r6, #0
	mov r11, r3
	bl Func_02002ddc
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
	bl Func_02002e84
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
	bl Func_02002dd4
	ldr r1, [r7, #28]
	adds r0, r6, #0
	bl Func_02002ddc
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
	.4byte Data_02003040
.L_02008334:
	.4byte Func_02000124
.L_02008338:
	.4byte 0xffff0000
	.section .text.x0200833c,"ax",%progbits
	.global Func_0200033c
	.thumb_func
Func_0200033c:
	ldr r0, .L_02008340
	bx lr
.L_02008340:
	.4byte Data_0200304c
	.section .text.x02008344,"ax",%progbits
	.global Func_02000344
	.thumb_func
Func_02000344:
	push {lr}
	ldr r3, .L_02008368
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_0200836c
	cmp r2, r3
	bne .L_0200835c
	ldr r0, .L_02008370
	b .L_02008366
.L_0200835c:
	ldr r3, .L_02008374
	movs r0, #0
	cmp r2, r3
	bne .L_02008366
	ldr r0, .L_02008378
.L_02008366:
	pop {pc}
.L_02008368:
	.4byte gPartyState
.L_0200836c:
	.4byte 0x000000c1
.L_02008370:
	.4byte Data_0200307c
.L_02008374:
	.4byte 0x000000c4
.L_02008378:
	.4byte Data_0200309c
	.section .text.x0200837c,"ax",%progbits
	.global Func_0200037c
	.thumb_func
Func_0200037c:
	ldr r0, .L_02008380
	bx lr
.L_02008380:
	.4byte Data_020030bc
	.section .text.x02008384,"ax",%progbits
	.global Func_02000384
	.thumb_func
Func_02000384:
	push {r5, lr}
	ldr r5, .L_020083bc
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	ldr r0, [r5]
	movs r1, #2
	bl Func_02002e8c
	movs r2, #128
	movs r1, #6
	lsls r2, r2, #5
	ldr r0, [r5]
	bl Func_02002e7c
	movs r0, #155
	lsls r0, r0, #1
	bl Func_02002eec
	movs r0, #145
	lsls r0, r0, #1
	bl Func_02002dbc
	movs r0, #13
	bl Func_02002ebc
	pop {r5, pc}
	.2byte 0x0000
.L_020083bc:
	.4byte gPartyState
	.section .text.x020083c0,"ax",%progbits
	.global Func_020003c0
	.thumb_func
Func_020003c0:
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
	bge .L_020083f0
	adds r3, #15
.L_020083f0:
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
	.section .text.x02008418,"ax",%progbits
	.global Func_02000418
	.thumb_func
Func_02000418:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, .L_020085ac
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	sub sp, #68
	bl Object_GetById
	adds r7, r0, #0
	bl Func_02002e44
	movs r0, #0
	bl Func_02002edc
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r1, r1
	negs r2, r2
	movs r3, #0
	negs r0, r0
	bl Func_02002eac
	bl Func_02002dec
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
	bl Func_02002eec
	movs r3, #3
	strb r3, [r5]
	movs r0, #24
	bl Battle_WaitMode0
	add r2, sp, #28
	movs r3, #7
	str r3, [r2, #4]
	ldr r3, .L_020085b0
	mov r8, r2
	str r3, [r2, #36]
	movs r3, #163
	lsls r3, r3, #8
	adds r3, #215
	str r3, [r2, #8]
	str r3, [r2, #12]
	movs r3, #0
	mov r10, r3
.L_020084b2:
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
	ldr r3, .L_020085b4
	adds r2, r2, r3
	str r2, [r6]
	bl Random16Far
	lsls r3, r0, #1
	ldr r5, [r6, #8]
	adds r3, r3, r0
	ldr r4, .L_020085b8
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
	ldr r4, .L_020085bc
	str r5, [sp, #4]
	str r4, [sp, #8]
	mov r4, r8
	str r4, [sp, #12]
	bl Func_0200015c
	movs r2, #1
	add r10, r2
	mov r3, r10
	cmp r3, #16
	bls .L_020084b2
	movs r0, #188
	bl Func_02002eec
	ldr r5, .L_020085ac
	movs r4, #133
	lsls r4, r4, #2
	adds r5, r5, r4
	movs r1, #2
	ldr r0, [r5]
	adds r1, #255
	bl Func_02002e9c
	ldr r0, [r5]
	movs r1, #49
	bl Object_SetModeById
	movs r0, #160
	movs r1, #160
	movs r2, #128
	lsls r0, r0, #11
	lsls r1, r1, #11
	lsls r2, r2, #9
	bl Func_02002e24
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	adds r2, #102
	negs r0, r0
	negs r1, r1
	bl Func_02002e24
	bl Func_02002e2c
	movs r1, #128
	ldr r0, [r5]
	lsls r1, r1, #1
	bl Func_02002e9c
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
	bl Func_02002e4c
	add sp, #68
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_020085ac:
	.4byte gPartyState
.L_020085b0:
	.4byte Func_020003c0
.L_020085b4:
	.4byte 0xffffa000
.L_020085b8:
	.4byte 0xffffd000
.L_020085bc:
	.4byte 0x01090001
	.section .text.x020085c0,"ax",%progbits
	.global Func_020005c0
	.thumb_func
Func_020005c0:
	push {r5, lr}
	movs r1, #0
	adds r5, r0, #0
	bl ObjectDispatch_SetSingleChildField26
	ldr r1, [r5, #80]
	movs r3, #13
	ldrb r2, [r1, #9]
	negs r3, r3
	ands r3, r2
	movs r2, #8
	orrs r3, r2
	strb r3, [r1, #9]
	adds r1, r5, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #254
	ands r3, r2
	strb r3, [r1]
	adds r3, r5, #0
	movs r2, #0
	adds r3, #85
	strb r2, [r3]
	ldr r2, .L_020085fc
	ldr r3, [r5, #12]
	movs r0, #0
	adds r3, r3, r2
	str r3, [r5, #12]
	pop {r5, pc}
	.2byte 0x0000
.L_020085fc:
	.4byte 0xfffc0000
	.section .text.x02008600,"ax",%progbits
	.global Func_02000600
	.thumb_func
Func_02000600:
	push {lr}
	ldr r3, .L_02008658
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_0200865c
	cmp r2, r3
	bne .L_02008618
	ldr r0, .L_02008660
	b .L_02008656
.L_02008618:
	ldr r3, .L_02008664
	cmp r2, r3
	bne .L_02008622
	ldr r0, .L_02008668
	b .L_02008656
.L_02008622:
	ldr r3, .L_0200866c
	cmp r2, r3
	bne .L_0200862c
	ldr r0, .L_02008670
	b .L_02008656
.L_0200862c:
	ldr r3, .L_02008674
	cmp r2, r3
	bne .L_02008636
	ldr r0, .L_02008678
	b .L_02008656
.L_02008636:
	ldr r3, .L_0200867c
	cmp r2, r3
	bne .L_02008640
	ldr r0, .L_02008680
	b .L_02008656
.L_02008640:
	ldr r3, .L_02008684
	cmp r2, r3
	bne .L_0200864a
	ldr r0, .L_02008688
	b .L_02008656
.L_0200864a:
	ldr r3, .L_0200868c
	cmp r2, r3
	bne .L_02008654
	ldr r0, .L_02008690
	b .L_02008656
.L_02008654:
	ldr r0, .L_02008694
.L_02008656:
	pop {pc}
.L_02008658:
	.4byte gPartyState
.L_0200865c:
	.4byte 0x000000bd
.L_02008660:
	.4byte Data_020032b8
.L_02008664:
	.4byte 0x000000be
.L_02008668:
	.4byte Data_020034f8
.L_0200866c:
	.4byte 0x000000bf
.L_02008670:
	.4byte Data_02003570
.L_02008674:
	.4byte 0x000000c0
.L_02008678:
	.4byte Data_020035d0
.L_0200867c:
	.4byte 0x000000c1
.L_02008680:
	.4byte Data_02003708
.L_02008684:
	.4byte 0x000000c2
.L_02008688:
	.4byte Data_02003750
.L_0200868c:
	.4byte 0x000000c4
.L_02008690:
	.4byte Data_02003768
.L_02008694:
	.4byte Data_020032a0
	.section .text.x02008698,"ax",%progbits
	.global Func_02000698
	.thumb_func
Func_02000698:
	push {lr}
	ldr r3, .L_020086b0
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	movs r3, #2
	adds r0, #34
	strb r3, [r0]
	pop {pc}
.L_020086b0:
	.4byte gPartyState
	.section .text.x020086b4,"ax",%progbits
	.global Func_020006b4
	.thumb_func
Func_020006b4:
	push {lr}
	ldr r3, .L_020086cc
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	movs r3, #0
	adds r0, #34
	strb r3, [r0]
	pop {pc}
.L_020086cc:
	.4byte gPartyState
	.section .text.x020086d0,"ax",%progbits
	.global Func_020006d0
	.thumb_func
Func_020006d0:
	push {r5, r6, lr}
	ldr r3, .L_0200883c
	adds r6, r1, #0
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008840
	sub sp, #8
	cmp r2, r3
	bne .L_02008714
	cmp r6, #8
	bne .L_02008714
	movs r3, #4
	movs r2, #69
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #4
	movs r1, #68
	movs r2, #1
	movs r3, #1
	bl Func_02002dfc
	movs r3, #3
	movs r2, #70
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #3
	movs r1, #68
	movs r2, #1
	movs r3, #1
	bl Func_02002dfc
.L_02008714:
	ldr r3, .L_0200883c
	movs r2, #240
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008844
	cmp r2, r3
	bne .L_02008782
	cmp r6, #19
	bne .L_02008752
	movs r3, #38
	movs r2, #72
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #35
	movs r1, #72
	movs r2, #1
	movs r3, #1
	bl Func_02002dfc
	movs r3, #37
	movs r2, #73
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #36
	movs r1, #72
	movs r2, #1
	movs r3, #1
	bl Func_02002dfc
.L_02008752:
	cmp r6, #8
	bne .L_0200876a
	movs r3, #3
	movs r2, #113
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #2
	movs r1, #113
	movs r2, #1
	movs r3, #1
	bl Func_02002dfc
.L_0200876a:
	cmp r6, #18
	bne .L_02008782
	movs r3, #40
	movs r2, #93
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #39
	movs r1, #93
	movs r2, #1
	movs r3, #1
	bl Func_02002dfc
.L_02008782:
	ldr r3, .L_0200883c
	movs r2, #240
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008848
	cmp r2, r3
	bne .L_020087c0
	cmp r6, #8
	bne .L_020087c0
	movs r3, #32
	movs r2, #70
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #32
	movs r1, #69
	movs r2, #1
	movs r3, #1
	bl Func_02002dfc
	movs r3, #33
	movs r2, #71
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #33
	movs r1, #69
	movs r2, #1
	movs r3, #1
	bl Func_02002dfc
.L_020087c0:
	ldr r3, .L_0200883c
	movs r2, #240
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_0200884c
	cmp r2, r3
	bne .L_02008836
	cmp r6, #8
	bne .L_02008810
	movs r3, #17
	str r3, [sp, #0]
	movs r5, #83
	movs r0, #17
	movs r1, #85
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl Func_02002dfc
	movs r3, #18
	movs r2, #82
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #19
	movs r1, #84
	movs r2, #1
	movs r3, #1
	bl Func_02002dfc
	movs r3, #19
	str r3, [sp, #0]
	movs r0, #19
	movs r1, #84
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl Func_02002dfc
.L_02008810:
	cmp r6, #9
	bne .L_02008836
	movs r0, #131
	lsls r0, r0, #1
	adds r0, #255
	bl Func_02002db4
	cmp r0, #0
	beq .L_02008836
	movs r3, #25
	movs r2, #98
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #35
	movs r1, #98
	movs r2, #3
	movs r3, #5
	bl Func_02002df4
.L_02008836:
	add sp, #8
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200883c:
	.4byte gPartyState
.L_02008840:
	.4byte 0x000000bd
.L_02008844:
	.4byte 0x000000c0
.L_02008848:
	.4byte 0x000000bf
.L_0200884c:
	.4byte 0x000000c1
	.section .text.x02008850,"ax",%progbits
	.global Func_02000850
	.thumb_func
Func_02000850:
	push {r5, r6, r7, lr}
	adds r5, r1, #0
	adds r0, r5, #0
	sub sp, #8
	bl Object_GetById
	adds r7, r0, #0
	cmp r5, #8
	bne .L_02008924
	ldr r3, .L_02008a54
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008a58
	cmp r2, r3
	bne .L_020088c2
	ldr r3, [r7, #8]
	asrs r3, r3, #20
	cmp r3, #4
	bne .L_0200889a
	movs r3, #2
	movs r2, #69
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #4
	movs r1, #68
	movs r2, #1
	movs r3, #1
	bl Func_02002dfc
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #198
	bl Func_02002dbc
.L_0200889a:
	movs r3, #4
	movs r2, #69
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #3
	movs r1, #68
	movs r2, #1
	movs r3, #1
	bl Func_02002dfc
	movs r3, #3
	movs r2, #70
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #3
	movs r1, #69
	movs r2, #1
	movs r3, #1
	bl Func_02002dfc
.L_020088c2:
	cmp r5, #8
	bne .L_02008924
	ldr r3, .L_02008a54
	movs r2, #240
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008a5c
	cmp r2, r3
	bne .L_02008924
	movs r3, #71
	str r3, [sp, #4]
	movs r0, #32
	movs r1, #71
	movs r2, #1
	movs r3, #1
	movs r6, #33
	str r6, [sp, #0]
	bl Func_02002dfc
	movs r3, #32
	movs r2, #70
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #1
	movs r0, #31
	movs r1, #70
	movs r2, #1
	bl Func_02002dfc
	ldr r3, [r7, #8]
	asrs r3, r3, #20
	cmp r3, #32
	bne .L_02008924
	movs r3, #69
	str r3, [sp, #4]
	movs r0, #32
	movs r1, #69
	movs r2, #1
	movs r3, #1
	str r6, [sp, #0]
	bl Func_02002dfc
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #199
	bl Func_02002dbc
.L_02008924:
	ldr r3, .L_02008a54
	movs r2, #240
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008a60
	cmp r2, r3
	bne .L_020089da
	cmp r5, #8
	bne .L_02008960
	movs r3, #3
	movs r2, #113
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #1
	movs r0, #3
	movs r1, #114
	movs r2, #1
	bl Func_02002dfc
	ldr r3, [r7, #8]
	asrs r3, r3, #20
	cmp r3, #2
	bne .L_02008960
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #200
	bl Func_02002dbc
.L_02008960:
	cmp r5, #18
	bne .L_0200898a
	movs r3, #40
	movs r2, #93
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #1
	movs r0, #40
	movs r1, #94
	movs r2, #1
	bl Func_02002dfc
	ldr r3, [r7, #8]
	asrs r3, r3, #20
	cmp r3, #39
	bne .L_0200898a
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #201
	bl Func_02002dbc
.L_0200898a:
	cmp r5, #19
	bne .L_020089da
	movs r3, #38
	movs r2, #72
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #36
	movs r1, #72
	movs r2, #1
	movs r3, #1
	bl Func_02002dfc
	movs r3, #73
	str r3, [sp, #4]
	movs r6, #37
	movs r3, #1
	movs r0, #37
	movs r1, #74
	movs r2, #1
	str r6, [sp, #0]
	bl Func_02002dfc
	ldr r3, [r7, #8]
	asrs r3, r3, #20
	cmp r3, #38
	bne .L_020089da
	movs r3, #71
	str r3, [sp, #4]
	movs r0, #37
	movs r1, #70
	movs r2, #1
	movs r3, #1
	str r6, [sp, #0]
	bl Func_02002dfc
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #202
	bl Func_02002dbc
.L_020089da:
	cmp r5, #8
	bne .L_02008a4e
	ldr r3, .L_02008a54
	movs r2, #240
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008a64
	cmp r2, r3
	bne .L_02008a4e
	movs r3, #17
	str r3, [sp, #0]
	movs r5, #83
	movs r0, #16
	movs r1, #83
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl Func_02002dfc
	movs r3, #82
	str r3, [sp, #4]
	movs r0, #19
	movs r1, #82
	movs r2, #1
	movs r3, #1
	movs r6, #18
	str r6, [sp, #0]
	bl Func_02002dfc
	movs r3, #19
	str r3, [sp, #0]
	movs r0, #19
	movs r3, #1
	movs r1, #82
	movs r2, #1
	str r5, [sp, #4]
	bl Func_02002dfc
	ldr r3, [r7, #8]
	asrs r3, r3, #20
	cmp r3, #17
	bne .L_02008a4e
	movs r3, #84
	str r3, [sp, #4]
	movs r0, #16
	movs r1, #84
	movs r2, #1
	movs r3, #1
	str r6, [sp, #0]
	bl Func_02002dfc
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #203
	bl Func_02002dbc
.L_02008a4e:
	add sp, #8
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008a54:
	.4byte gPartyState
.L_02008a58:
	.4byte 0x000000bd
.L_02008a5c:
	.4byte 0x000000bf
.L_02008a60:
	.4byte 0x000000c0
.L_02008a64:
	.4byte 0x000000c1
	.section .text.x02008a68,"ax",%progbits
	.global Func_02000a68
	.thumb_func
Func_02000a68:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r5, r1, #0
	adds r0, r5, #0
	bl Object_GetById
	adds r6, r0, #0
	ldr r3, [r6, #16]
	movs r2, #60
	asrs r3, r3, #20
	mov r8, r2
	cmp r3, #31
	bne .L_02008b26
	adds r2, r6, #0
	adds r2, #85
	movs r3, #3
	strb r3, [r2]
	movs r1, #3
	bl Func_02002e34
	movs r0, #204
	bl Func_02002eec
	movs r7, #236
	lsls r7, r7, #14
	b .L_02008aa8
.L_02008a9e:
	ldr r2, .L_02008b2c
	adds r3, r7, #0
	asrs r3, r3, #16
	adds r7, r7, r2
	mov r8, r3
.L_02008aa8:
	mov r3, r8
	cmp r3, #0
	beq .L_02008aba
	movs r0, #1
	bl WaitFrames
	ldr r3, [r6, #40]
	cmp r3, #0
	bne .L_02008a9e
.L_02008aba:
	adds r0, r5, #0
	movs r1, #0
	movs r2, #0
	bl Func_02002e6c
	cmp r5, #10
	bne .L_02008ad2
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #210
	bl Func_02002dbc
.L_02008ad2:
	cmp r5, #11
	bne .L_02008ae0
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #211
	bl Func_02002dbc
.L_02008ae0:
	cmp r5, #12
	bne .L_02008aee
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #212
	bl Func_02002dbc
.L_02008aee:
	cmp r5, #13
	bne .L_02008afc
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #213
	bl Func_02002dbc
.L_02008afc:
	cmp r5, #14
	bne .L_02008b0a
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #214
	bl Func_02002dbc
.L_02008b0a:
	cmp r5, #15
	bne .L_02008b18
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #215
	bl Func_02002dbc
.L_02008b18:
	cmp r5, #16
	bne .L_02008b26
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #216
	bl Func_02002dbc
.L_02008b26:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_02008b2c:
	.4byte 0xffff0000
	.section .text.x02008b30,"ax",%progbits
	.global Func_02000b30
	.thumb_func
Func_02000b30:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r7, r1, #0
	bl Object_GetById
	movs r2, #85
	adds r5, r0, #0
	adds r2, r2, r5
	movs r3, #3
	strb r3, [r2]
	movs r6, #60
	mov r8, r2
.L_02008b4a:
	cmp r6, #0
	beq .L_02008b5c
	movs r0, #1
	bl WaitFrames
	ldr r3, [r5, #40]
	subs r6, #1
	cmp r3, #0
	bne .L_02008b4a
.L_02008b5c:
	cmp r7, #0
	beq .L_02008b66
	adds r0, r7, #0
	bl Func_02002eec
.L_02008b66:
	movs r0, #10
	bl WaitFrames
	movs r3, #0
	mov r2, r8
	strb r3, [r2]
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.section .text.x02008b78,"ax",%progbits
	.global Func_02000b78
	.thumb_func
Func_02000b78:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #68
	str r2, [sp, #20]
	movs r2, #28
	str r3, [sp, #16]
	add r2, sp
	movs r3, #1
	str r1, [sp, #24]
	str r3, [r2, #4]
	movs r3, #192
	lsls r3, r3, #8
	str r3, [r2, #8]
	str r3, [r2, #12]
	movs r3, #0
	mov r11, r0
	mov r9, r3
	mov r8, r2
	cmp r9, r11
	bge .L_02008c16
	ldr r2, [sp, #100]
	mov r10, r2
.L_02008bae:
	ldr r3, [sp, #104]
	cmp r3, #0
	ble .L_02008c04
	ldr r2, [sp, #20]
	adds r6, r3, #0
	lsls r7, r2, #16
.L_02008bba:
	bl Random16Far
	movs r5, #15
	ands r5, r0
	bl Random16Far
	ldr r3, [sp, #16]
	movs r1, #31
	ands r1, r0
	adds r1, r3, r1
	movs r3, #0
	str r3, [sp, #0]
	movs r3, #128
	lsls r3, r3, #1
	str r3, [sp, #4]
	movs r3, #128
	subs r5, #8
	lsls r3, r3, #12
	adds r3, #1
	lsls r5, r5, #16
	adds r5, r7, r5
	str r3, [sp, #8]
	mov r2, r8
	mov r3, r10
	str r2, [sp, #12]
	lsls r1, r1, #16
	lsls r2, r3, #16
	adds r0, r5, #0
	movs r3, #0
	bl Func_0200015c
	movs r2, #128
	lsls r2, r2, #13
	subs r6, #1
	adds r7, r7, r2
	cmp r6, #0
	bne .L_02008bba
.L_02008c04:
	ldr r0, [sp, #24]
	bl Battle_WaitMode0
	movs r2, #1
	movs r3, #2
	add r9, r2
	add r10, r3
	cmp r9, r11
	blt .L_02008bae
.L_02008c16:
	add sp, #68
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.section .text.x02008c24,"ax",%progbits
	.global Func_02000c24
	.thumb_func
Func_02000c24:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	adds r5, r1, #0
	adds r0, r5, #0
	sub sp, #8
	bl Object_GetById
	ldr r3, [r0, #8]
	asrs r7, r3, #20
	ldr r3, [r0, #16]
	asrs r6, r3, #20
	cmp r5, #9
	bne .L_02008c5e
	cmp r7, #12
	bne .L_02008c5e
	cmp r6, #45
	bne .L_02008c5e
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #217
	bl Func_02002dbc
	movs r0, #9
	movs r1, #181
	bl Func_02000b30
.L_02008c5e:
	cmp r5, #10
	bne .L_02008c7c
	cmp r7, #14
	bne .L_02008c7c
	cmp r6, #45
	bne .L_02008c7c
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #218
	bl Func_02002dbc
	movs r0, #10
	movs r1, #181
	bl Func_02000b30
.L_02008c7c:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #218
	bl Func_02002db4
	cmp r0, #0
	bne .L_02008c8c
	b .L_02008e50
.L_02008c8c:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #217
	bl Func_02002db4
	cmp r0, #0
	bne .L_02008c9c
	b .L_02008e50
.L_02008c9c:
	bl Func_02002e44
	movs r0, #0
	bl Func_02002edc
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r0, r0, #10
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl Func_02002e24
	movs r0, #216
	movs r1, #1
	negs r1, r1
	ldr r2, .L_02008e5c
	movs r3, #1
	lsls r0, r0, #16
	bl Func_02002eac
	movs r0, #15
	bl Battle_WaitMode0
	movs r0, #202
	bl Func_02002eec
	movs r5, #12
	movs r6, #111
	movs r1, #91
	movs r2, #3
	movs r3, #1
	movs r0, #12
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_02002e04
	movs r0, #60
	bl Battle_WaitMode0
	movs r0, #160
	lsls r0, r0, #1
	adds r0, #255
	bl Func_02002eec
	movs r3, #47
	str r3, [sp, #4]
	mov r9, r3
	movs r0, #16
	movs r1, #27
	movs r2, #3
	movs r3, #2
	str r5, [sp, #0]
	bl Func_02002df4
	movs r0, #16
	movs r1, #91
	movs r2, #3
	movs r3, #2
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_02002df4
	movs r3, #40
	negs r3, r3
	mov r10, r3
	movs r3, #186
	lsls r3, r3, #2
	str r3, [sp, #0]
	movs r3, #3
	str r3, [sp, #4]
	mov r8, r3
	movs r0, #3
	movs r1, #15
	movs r2, #200
	mov r3, r10
	bl Func_02000b78
	mov r3, r9
	str r3, [sp, #4]
	movs r0, #16
	movs r1, #27
	movs r2, #3
	movs r3, #3
	str r5, [sp, #0]
	bl Func_02002df4
	movs r0, #16
	movs r1, #91
	movs r2, #3
	movs r3, #3
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_02002df4
	movs r3, #190
	lsls r3, r3, #2
	str r3, [sp, #0]
	mov r3, r8
	str r3, [sp, #4]
	movs r0, #3
	movs r1, #10
	movs r2, #200
	mov r3, r10
	bl Func_02000b78
	mov r3, r9
	str r3, [sp, #4]
	movs r0, #16
	movs r1, #27
	movs r2, #3
	movs r3, #4
	str r5, [sp, #0]
	bl Func_02002df4
	movs r0, #16
	movs r1, #91
	movs r2, #3
	movs r3, #4
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_02002df4
	movs r3, #194
	lsls r3, r3, #2
	str r3, [sp, #0]
	mov r3, r8
	str r3, [sp, #4]
	movs r0, #3
	movs r1, #10
	movs r2, #200
	mov r3, r10
	bl Func_02000b78
	mov r3, r9
	str r3, [sp, #4]
	movs r0, #16
	movs r1, #27
	movs r2, #3
	movs r3, #5
	str r5, [sp, #0]
	bl Func_02002df4
	movs r0, #16
	movs r1, #91
	movs r2, #3
	movs r3, #5
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_02002df4
	movs r3, #198
	lsls r3, r3, #2
	str r3, [sp, #0]
	mov r3, r8
	str r3, [sp, #4]
	movs r0, #3
	movs r1, #10
	movs r2, #200
	mov r3, r10
	bl Func_02000b78
	mov r3, r9
	str r3, [sp, #4]
	movs r0, #16
	movs r1, #27
	movs r2, #3
	movs r3, #6
	str r5, [sp, #0]
	bl Func_02002df4
	movs r3, #6
	movs r0, #16
	movs r1, #91
	movs r2, #3
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_02002df4
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	negs r0, r0
	negs r1, r1
	adds r2, #102
	bl Func_02002e24
	movs r3, #202
	lsls r3, r3, #2
	str r3, [sp, #0]
	mov r3, r8
	str r3, [sp, #4]
	movs r0, #3
	movs r1, #10
	movs r2, #200
	mov r3, r10
	bl Func_02000b78
	mov r3, r9
	str r3, [sp, #4]
	movs r0, #16
	movs r1, #27
	movs r2, #3
	movs r3, #13
	str r5, [sp, #0]
	bl Func_02002df4
	movs r0, #16
	movs r1, #91
	movs r2, #3
	movs r3, #13
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_02002df4
	bl Func_02002e4c
.L_02008e50:
	add sp, #8
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_02008e5c:
	.4byte 0x03020000
	.section .text.x02008e60,"ax",%progbits
	.global Func_02000e60
	.thumb_func
Func_02000e60:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	adds r5, r1, #0
	adds r0, r5, #0
	sub sp, #8
	bl Object_GetById
	ldr r3, [r0, #8]
	asrs r7, r3, #20
	ldr r3, [r0, #16]
	asrs r6, r3, #20
	cmp r5, #8
	beq .L_02008e80
	b .L_02009040
.L_02008e80:
	cmp r7, #12
	beq .L_02008e86
	b .L_02009040
.L_02008e86:
	cmp r6, #5
	beq .L_02008e8c
	b .L_02009040
.L_02008e8c:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #220
	bl Func_02002dbc
	movs r1, #181
	movs r0, #8
	bl Func_02000b30
	bl Func_02002e44
	movs r0, #0
	bl Func_02002edc
	movs r0, #15
	bl Battle_WaitMode0
	movs r0, #202
	bl Func_02002eec
	movs r3, #71
	str r3, [sp, #4]
	movs r0, #29
	movs r1, #70
	movs r2, #3
	movs r3, #1
	str r7, [sp, #0]
	bl Func_02002df4
	movs r0, #216
	movs r1, #1
	movs r2, #240
	movs r3, #1
	lsls r0, r0, #16
	negs r1, r1
	lsls r2, r2, #15
	bl Func_02002eac
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #10
	lsls r2, r2, #9
	lsls r0, r0, #10
	bl Func_02002e24
	movs r0, #60
	bl Battle_WaitMode0
	movs r0, #160
	lsls r0, r0, #1
	adds r0, #255
	bl Func_02002eec
	movs r3, #7
	str r3, [sp, #4]
	movs r5, #10
	mov r10, r3
	movs r0, #35
	movs r1, #6
	movs r2, #7
	movs r3, #2
	str r5, [sp, #0]
	bl Func_02002df4
	movs r3, #71
	str r3, [sp, #4]
	movs r0, #35
	movs r1, #70
	movs r2, #7
	movs r3, #2
	str r5, [sp, #0]
	bl Func_02002df4
	movs r3, #40
	negs r3, r3
	mov r8, r3
	movs r3, #124
	str r3, [sp, #0]
	movs r3, #3
	str r3, [sp, #4]
	movs r0, #6
	movs r1, #10
	movs r2, #200
	mov r3, r8
	bl Func_02000b78
	mov r3, r10
	str r3, [sp, #4]
	movs r0, #35
	movs r1, #6
	movs r2, #7
	movs r3, #3
	str r5, [sp, #0]
	bl Func_02002df4
	movs r3, #71
	str r3, [sp, #4]
	movs r0, #35
	movs r1, #70
	movs r2, #7
	movs r3, #3
	str r5, [sp, #0]
	bl Func_02002df4
	movs r3, #140
	str r3, [sp, #0]
	movs r0, #3
	movs r1, #10
	movs r2, #184
	mov r3, r8
	str r6, [sp, #4]
	bl Func_02000b78
	mov r3, r10
	str r3, [sp, #4]
	movs r0, #35
	movs r1, #6
	movs r2, #7
	movs r3, #4
	str r5, [sp, #0]
	bl Func_02002df4
	movs r3, #71
	str r3, [sp, #4]
	movs r0, #35
	movs r1, #70
	movs r2, #7
	movs r3, #4
	str r5, [sp, #0]
	bl Func_02002df4
	movs r3, #156
	str r3, [sp, #0]
	movs r0, #3
	movs r1, #10
	movs r2, #184
	mov r3, r8
	str r6, [sp, #4]
	bl Func_02000b78
	mov r3, r10
	str r3, [sp, #4]
	movs r0, #35
	movs r1, #6
	movs r2, #7
	movs r3, #5
	str r5, [sp, #0]
	bl Func_02002df4
	movs r3, #71
	str r3, [sp, #4]
	movs r0, #35
	movs r1, #70
	movs r2, #7
	movs r3, #5
	str r5, [sp, #0]
	bl Func_02002df4
	movs r3, #172
	str r3, [sp, #0]
	movs r0, #3
	mov r3, r8
	movs r1, #10
	movs r2, #184
	str r6, [sp, #4]
	bl Func_02000b78
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	negs r0, r0
	negs r1, r1
	adds r2, #102
	bl Func_02002e24
	mov r3, r10
	str r3, [sp, #4]
	movs r0, #35
	movs r1, #6
	movs r2, #7
	movs r3, #6
	str r5, [sp, #0]
	bl Func_02002df4
	movs r3, #71
	str r3, [sp, #4]
	movs r1, #70
	movs r2, #7
	movs r3, #6
	movs r0, #35
	str r5, [sp, #0]
	bl Func_02002df4
	movs r0, #15
	bl Battle_WaitMode0
	mov r3, r10
	str r3, [sp, #4]
	movs r0, #35
	movs r1, #6
	movs r2, #7
	movs r3, #16
	str r5, [sp, #0]
	bl Func_02002df4
	movs r3, #71
	str r3, [sp, #4]
	movs r0, #35
	movs r1, #70
	movs r2, #7
	movs r3, #16
	str r5, [sp, #0]
	bl Func_02002df4
	bl Func_02002e4c
.L_02009040:
	add sp, #8
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x0200904c,"ax",%progbits
	.global Func_0200104c
	.thumb_func
Func_0200104c:
	push {r5, r6, lr}
	adds r5, r1, #0
	adds r0, r5, #0
	bl Object_GetById
	adds r6, r0, #0
	ldr r3, [r6, #8]
	asrs r2, r3, #20
	ldr r3, [r6, #16]
	asrs r3, r3, #20
	cmp r5, #10
	bne .L_020090e6
	cmp r2, #15
	bne .L_020090e6
	cmp r3, #24
	bne .L_020090e6
	movs r0, #10
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	ldr r5, .L_020090e8
	ldr r3, [r6, #12]
	movs r0, #1
	adds r3, r3, r5
	str r3, [r6, #12]
	bl Battle_WaitMode0
	ldr r3, [r6, #12]
	movs r0, #1
	adds r3, r3, r5
	str r3, [r6, #12]
	bl Battle_WaitMode0
	ldr r3, [r6, #12]
	ldr r2, .L_020090ec
	movs r0, #1
	adds r3, r3, r2
	str r3, [r6, #12]
	bl Battle_WaitMode0
	ldr r2, .L_020090f0
	ldr r3, [r6, #12]
	movs r0, #1
	adds r3, r3, r2
	str r3, [r6, #12]
	bl Battle_WaitMode0
	movs r0, #9
	movs r1, #3
	bl Object_SetModeById
	movs r2, #0
	movs r1, #0
	movs r0, #10
	bl Func_02002e6c
	movs r0, #188
	bl Func_02002eec
	movs r0, #45
	bl Battle_WaitMode0
	movs r0, #9
	movs r1, #4
	bl Object_SetModeById
	movs r0, #141
	lsls r0, r0, #2
	bl Func_02002eec
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #221
	bl Func_02002dbc
.L_020090e6:
	pop {r5, r6, pc}
.L_020090e8:
	.4byte 0xfffe0000
.L_020090ec:
	.4byte 0xfffc0000
.L_020090f0:
	.4byte 0xfff80000
	.section .text.x020090f4,"ax",%progbits
	.global Func_020010f4
	.thumb_func
Func_020010f4:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r3, .L_020091b8
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	ldr r0, [r3]
	sub sp, #12
	bl Object_GetById
	movs r3, #192
	lsls r3, r3, #18
	ldr r7, [r3, #108]
	movs r2, #230
	lsls r2, r2, #1
	ldr r5, [r3, #32]
	adds r3, r7, r2
	ldr r3, [r3]
	adds r6, r0, #0
	movs r0, #130
	lsls r0, r0, #1
	mov r8, r3
	bl Func_02002db4
	cmp r0, #0
	bne .L_020091b0
	cmp r5, #0
	bne .L_02009134
	ldr r1, .L_020091bc
	adds r0, r1, #0
	b .L_02009144
.L_02009134:
	movs r1, #156
	lsls r1, r1, #1
	movs r2, #212
	adds r3, r5, r1
	lsls r2, r2, #1
	ldr r0, [r3]
	adds r3, r5, r2
	ldr r1, [r3]
.L_02009144:
	ldr r3, [r6, #16]
	ldr r4, [r6, #8]
	asrs r3, r3, #20
	lsls r3, r3, #7
	asrs r2, r4, #20
	adds r2, r2, r3
	lsls r2, r2, #2
	adds r0, r0, r2
	adds r1, r1, r2
	ldrb r3, [r0, #3]
	ldrb r1, [r1, #2]
	movs r2, #4
	ands r2, r3
	movs r3, #232
	ands r3, r1
	orrs r2, r3
	cmp r2, #236
	bne .L_020091b0
	ldr r3, .L_020091b8
	movs r1, #128
	lsls r1, r1, #2
	adds r1, #18
	adds r3, r3, r1
	ldrb r3, [r3]
	cmp r3, #5
	beq .L_020091b0
	mov r1, sp
	str r4, [r1]
	movs r2, #128
	ldr r3, [r6, #12]
	lsls r2, r2, #12
	str r3, [r1, #4]
	adds r0, r6, #0
	ldr r3, [r6, #16]
	adds r3, r3, r2
	str r3, [r1, #8]
	bl Func_02002e0c
	cmp r0, #0
	bge .L_0200919e
	movs r3, #170
	lsls r3, r3, #1
	adds r2, r7, r3
	movs r3, #66
	strh r3, [r2]
.L_0200919e:
	ldr r3, [r6, #16]
	movs r2, #128
	lsls r2, r2, #8
	adds r3, r3, r2
	str r3, [r6, #16]
	mov r1, r8
	ldr r3, [r1, #16]
	adds r3, r3, r2
	str r3, [r1, #16]
.L_020091b0:
	add sp, #12
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_020091b8:
	.4byte gPartyState
.L_020091bc:
	.4byte gMapCellBuffer
	.section .text.x020091c0,"ax",%progbits
	.global Func_020011c0
	.thumb_func
Func_020011c0:
	push {r5, r6, lr}
	ldr r5, .L_02009248
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	ldr r0, [r5]
	bl Object_GetById
	adds r6, r0, #0
	bl Func_02002e44
	movs r0, #0
	bl Func_02002edc
	ldr r0, [r5]
	movs r1, #0
	bl Object_SetModeById
	movs r1, #129
	ldr r0, [r5]
	lsls r1, r1, #1
	bl Func_02002e9c
	adds r2, r6, #0
	movs r3, #0
	adds r6, #90
	adds r2, #85
	strb r3, [r2]
	strb r3, [r6]
	movs r1, #0
	ldr r0, [r5]
	movs r2, #8
	bl ObjectMotion_OffsetPositionAndResetMotion
	ldr r0, [r5]
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	ldr r0, [r5]
	bl Func_02002e94
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r2, r2
	movs r3, #0
	negs r1, r1
	negs r0, r0
	bl Func_02002eac
	movs r0, #204
	bl Func_02002eec
	ldr r0, [r5]
	movs r1, #188
	bl Func_02000b30
	movs r3, #1
	strb r3, [r6]
	bl Func_02002e4c
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02009248:
	.4byte gPartyState
	.section .text.x0200924c,"ax",%progbits
	.global Func_0200124c
	.thumb_func
Func_0200124c:
	push {lr}
	adds r3, r1, #0
	subs r3, #9
	cmp r3, #1
	bhi .L_0200925c
	bl Func_02000c24
	b .L_02009260
.L_0200925c:
	bl Func_02000850
.L_02009260:
	pop {pc}
	.2byte 0x0000
	.section .text.x02009264,"ax",%progbits
	.global Func_02001264
	.thumb_func
Func_02001264:
	push {r5, r6, lr}
	ldr r3, .L_020092b4
	movs r2, #240
	lsls r2, r2, #1
	adds r6, r3, r2
	movs r3, #0
	ldrsh r2, [r6, r3]
	ldr r3, .L_020092b8
	adds r5, r1, #0
	cmp r2, r3
	bne .L_02009286
	movs r2, #136
	lsls r2, r2, #4
	adds r2, #255
	adds r0, r5, r2
	bl Func_02002dbc
.L_02009286:
	movs r3, #0
	ldrsh r2, [r6, r3]
	ldr r3, .L_020092bc
	cmp r2, r3
	bne .L_0200929c
	movs r2, #144
	lsls r2, r2, #4
	adds r2, #149
	adds r0, r5, r2
	bl Func_02002dbc
.L_0200929c:
	movs r3, #0
	ldrsh r2, [r6, r3]
	ldr r3, .L_020092c0
	cmp r2, r3
	bne .L_020092b2
	movs r2, #144
	lsls r2, r2, #4
	adds r2, #150
	adds r0, r5, r2
	bl Func_02002dbc
.L_020092b2:
	pop {r5, r6, pc}
.L_020092b4:
	.4byte gPartyState
.L_020092b8:
	.4byte 0x000000bd
.L_020092bc:
	.4byte 0x000000be
.L_020092c0:
	.4byte 0x000000c0
	.section .text.x020092c4,"ax",%progbits
	.global Func_020012c4
	.thumb_func
Func_020012c4:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	sub sp, #4
	mov r8, r3
	mov r2, r8
	adds r2, #228
	ldr r7, [r2]
	ldr r3, .L_02009408
	ldr r6, [r2, #4]
	mov r1, r8
	ands r7, r3
	ands r6, r3
	ldr r3, [r1]
	ldr r0, .L_0200940c
	ldr r3, [r3, #4]
	ldr r2, .L_02009410
	str r3, [sp, #0]
	mov r10, r0
	movs r1, #2
	ldrsh r3, [r0, r1]
	movs r0, #131
	lsls r3, r3, #2
	adds r3, r3, r2
	ldrh r3, [r3, #2]
	lsls r0, r0, #1
	lsrs r3, r3, #5
	mov r5, r10
	adds r0, #255
	mov r11, r3
	adds r5, #16
	bl Func_02002db4
	cmp r0, #0
	bne .L_020093e8
	mov r2, r10
	ldr r3, [r2, #4]
	movs r0, #128
	lsls r0, r0, #14
	cmp r3, r0
	bge .L_020093e8
	movs r1, #0
	mov r9, r1
.L_02009328:
	ldr r3, [r5, #12]
	ldr r1, [r5]
	ldr r0, [r5, #8]
	ldr r4, [r5, #4]
	cmp r3, #4
	bhi .L_02009388
	ldr r2, .L_02009414
	lsls r3, r3, #2
	ldr r2, [r3, r2]
	mov r3, r10
	mov r12, r2
	ldr r2, [r3, #4]
	mov pc, r12
	.2byte 0x0000
.L_02009344:
	.4byte .L_02009358
	.4byte .L_02009366
	.4byte .L_0200936c
	.4byte .L_02009372
	.4byte .L_0200937a
.L_02009358:
	movs r3, #128
	lsls r3, r3, #13
	subs r1, r1, r2
.L_0200935e:
	adds r0, r0, r2
	cmp r2, r3
	ble .L_02009388
	b .L_02009386
.L_02009366:
	subs r1, r1, r2
	adds r0, r0, r2
	b .L_02009388
.L_0200936c:
	adds r1, r1, r2
	adds r0, r0, r2
	b .L_02009388
.L_02009372:
	movs r3, #128
	lsls r3, r3, #13
	adds r1, r1, r2
	b .L_0200935e
.L_0200937a:
	movs r3, #128
	lsls r3, r3, #13
	cmp r2, r3
	bge .L_02009386
	adds r0, r0, r2
	b .L_02009388
.L_02009386:
	ldr r1, .L_02009418
.L_02009388:
	subs r1, r1, r7
	subs r3, r4, r6
	subs r2, r3, r0
	asrs r3, r1, #16
	movs r0, #167
	adds r1, r3, #0
	asrs r2, r2, #16
	adds r3, #7
	lsls r0, r0, #1
	subs r1, #8
	subs r2, #8
	cmp r3, r0
	bhi .L_020093dc
	movs r3, #15
	negs r3, r3
	cmp r2, r3
	blt .L_020093dc
	cmp r2, #239
	bgt .L_020093dc
	movs r3, #128
	lsls r3, r3, #1
	adds r3, #255
	ands r1, r3
	movs r3, #255
	ands r2, r3
	movs r3, #0
	str r3, [r5, #16]
	lsls r3, r1, #16
	orrs r2, r3
	ldr r3, .L_0200941c
	mov r0, r11
	orrs r2, r3
	movs r3, #128
	lsls r3, r3, #3
	orrs r3, r0
	adds r0, r5, #0
	str r2, [r5, #20]
	str r3, [r5, #24]
	adds r0, #16
	movs r1, #255
	bl Func_02002dac
.L_020093dc:
	movs r1, #1
	add r9, r1
	mov r2, r9
	adds r5, #28
	cmp r2, #7
	ble .L_02009328
.L_020093e8:
	mov r0, r10
	ldr r2, [r0, #12]
	ldr r1, [r0, #4]
	movs r3, #160
	lsls r3, r3, #1
	add r3, r8
	adds r2, r2, r1
	str r2, [r3, #12]
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009408:
	.4byte 0xffff0000
.L_0200940c:
	.4byte gOverlayArea + 0x4030
.L_02009410:
	.4byte ResourceTableEntries
.L_02009414:
	.4byte .L_02009344
.L_02009418:
	.4byte 0xfff00000
.L_0200941c:
	.4byte 0x40000800
	.section .text.x02009420,"ax",%progbits
	.global Func_02001420
	.thumb_func
Func_02001420:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r0, #128
	sub sp, #4
	ldr r7, .L_020094d4
	bl Runtime_BumpAllocate
	adds r6, r0, #0
	str r6, [sp, #0]
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	movs r0, #160
	lsls r0, r0, #1
	adds r0, r0, r3
	mov r8, r0
	movs r0, #10
	adds r5, r7, #0
	adds r0, #255
	adds r5, #16
	bl Func_02002db4
	cmp r0, #0
	bne .L_0200945c
	ldr r3, .L_020094d8
	adds r0, r7, #0
	movs r1, #240
	mov lr, r3
	.2byte 0xf800
.L_0200945c:
	movs r2, #31
.L_0200945e:
	ldr r0, [sp, #0]
	ldr r3, .L_020094dc
	subs r2, #1
	stmia r0!, {r3}
	adds r1, r0, #0
	str r1, [sp, #0]
	cmp r2, #0
	bge .L_0200945e
	bl Resource_FindFreeEntry
	strh r0, [r7, #2]
	lsls r0, r0, #16
	movs r1, #128
	adds r2, r6, #0
	asrs r0, r0, #16
	bl VramBlock_LoadCached
	adds r0, r6, #0
	bl Sys_Free
	mov r1, r8
	ldr r3, [r1, #8]
	str r3, [r7, #8]
	ldr r3, [r1, #12]
	str r3, [r7, #12]
	movs r2, #209
	movs r3, #128
	lsls r2, r2, #5
	lsls r3, r3, #19
	adds r2, #255
	adds r3, #74
	strh r2, [r3]
	movs r1, #128
	lsls r1, r1, #19
	ldrh r3, [r1]
	ldr r2, .L_020094d0
	ldr r0, .L_020094e0
	orrs r3, r2
	strh r3, [r1]
	movs r1, #144
	lsls r1, r1, #3
	bl Func_02002d64
	ldr r3, .L_020094e4
	movs r2, #241
	lsls r2, r2, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #128
	subs r3, #1
	lsls r3, r3, #16
	lsls r0, r0, #9
	mov r12, r2
	cmp r3, r0
	bhi .L_0200953e
	b .L_020094e8
	.2byte 0x0000
.L_020094d0:
	.4byte 0x00008000
.L_020094d4:
	.4byte gOverlayArea + 0x4030
.L_020094d8:
	.4byte IwramClearWords
.L_020094dc:
	.4byte 0x11111111
.L_020094e0:
	.4byte Func_020012c4
.L_020094e4:
	.4byte gPartyState
.L_020094e8:
	movs r3, #160
	lsls r3, r3, #14
	movs r2, #240
	str r3, [r5]
	movs r1, #0
	lsls r2, r2, #15
	movs r0, #1
	movs r3, #224
	str r0, [r5, #12]
	str r1, [r5, #8]
	str r2, [r5, #4]
	lsls r3, r3, #14
	adds r5, #28
	str r3, [r5]
	movs r3, #216
	str r0, [r5, #12]
	str r1, [r5, #8]
	str r2, [r5, #4]
	lsls r3, r3, #16
	adds r5, #28
	str r3, [r5]
	movs r0, #2
	movs r3, #232
	str r1, [r5, #8]
	str r2, [r5, #4]
	str r0, [r5, #12]
	lsls r3, r3, #16
	adds r5, #28
	str r3, [r5]
	movs r3, #184
	str r1, [r5, #8]
	str r2, [r5, #4]
	str r0, [r5, #12]
	lsls r3, r3, #16
	adds r5, #28
	str r3, [r5]
	movs r3, #176
	lsls r3, r3, #15
	str r3, [r5, #4]
	movs r3, #4
	str r1, [r5, #8]
	str r3, [r5, #12]
	adds r5, #28
.L_0200953e:
	mov r1, r12
	ldrh r3, [r1]
	movs r2, #128
	subs r3, #3
	lsls r3, r3, #16
	lsls r2, r2, #9
	cmp r3, r2
	bhi .L_020095a4
	movs r3, #160
	lsls r3, r3, #14
	movs r1, #220
	str r3, [r5]
	movs r2, #0
	lsls r1, r1, #17
	movs r0, #1
	movs r3, #224
	str r1, [r5, #4]
	str r2, [r5, #8]
	str r0, [r5, #12]
	lsls r3, r3, #14
	adds r5, #28
	str r3, [r5]
	movs r3, #216
	str r1, [r5, #4]
	str r2, [r5, #8]
	str r0, [r5, #12]
	lsls r3, r3, #16
	adds r5, #28
	movs r1, #204
	str r3, [r5]
	lsls r1, r1, #17
	movs r3, #232
	str r2, [r5, #8]
	str r1, [r5, #4]
	str r2, [r5, #12]
	lsls r3, r3, #16
	adds r5, #28
	str r3, [r5]
	movs r3, #144
	str r2, [r5, #8]
	str r1, [r5, #4]
	str r0, [r5, #12]
	lsls r3, r3, #15
	adds r5, #28
	str r3, [r5]
	movs r3, #188
	lsls r3, r3, #17
	str r3, [r5, #4]
	movs r3, #4
	str r2, [r5, #8]
	str r3, [r5, #12]
.L_020095a4:
	add sp, #4
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.section .text.x020095ac,"ax",%progbits
	.global Func_020015ac
	.thumb_func
Func_020015ac:
	push {r5, lr}
	movs r3, #192
	movs r0, #130
	lsls r3, r3, #18
	lsls r0, r0, #1
	ldr r5, [r3, #108]
	bl Func_02002db4
	cmp r0, #0
	bne .L_020095f8
	ldr r2, .L_020095fc
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	ldr r2, .L_02009600
	lsls r3, r3, #16
	movs r1, #0
	ldrsh r2, [r2, r1]
	asrs r3, r3, #16
	cmp r3, r2
	blt .L_020095f8
	ldr r0, .L_02009604
	bl Scheduler_RemoveCallbackFar
	ldr r3, .L_02009608
	ldr r3, [r3]
	cmp r3, #1
	bne .L_020095ee
	movs r3, #181
	lsls r3, r3, #1
	adds r2, r5, r3
	movs r3, #68
	b .L_020095f6
.L_020095ee:
	movs r1, #181
	lsls r1, r1, #1
	adds r2, r5, r1
	movs r3, #69
.L_020095f6:
	strh r3, [r2]
.L_020095f8:
	pop {r5, pc}
	.2byte 0x0000
.L_020095fc:
	.4byte Data_02003782
.L_02009600:
	.4byte Data_02003784
.L_02009604:
	.4byte Func_020015ac
.L_02009608:
	.4byte Data_0200378c
	.section .text.x0200960c,"ax",%progbits
	.global Func_0200160c
	.thumb_func
Func_0200160c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r5, .L_02009980
	movs r0, #133
	lsls r0, r0, #2
	adds r5, r5, r0
	ldr r0, [r5]
	sub sp, #20
	bl Object_GetById
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	ldr r7, .L_02009984
	adds r6, r0, #0
	movs r1, #0
	ldr r0, [r5]
	mov r9, r3
	bl Object_SetModeById
	ldr r3, [r7, #4]
	movs r1, #85
	asrs r3, r3, #19
	adds r1, r1, r6
	mov r8, r3
	movs r3, #0
	strb r3, [r1]
	mov r10, r1
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #11
	lsls r2, r2, #9
	lsls r0, r0, #11
	bl Func_02002e24
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #139
	lsls r0, r0, #2
	bl Func_02002eec
	movs r3, #66
	str r3, [sp, #4]
	movs r5, #7
	movs r0, #22
	movs r1, #66
	movs r2, #3
	movs r3, #1
	str r5, [sp, #0]
	bl Func_02002df4
	movs r3, #1
	str r3, [sp, #4]
	movs r2, #3
	movs r0, #22
	movs r1, #1
	movs r3, #5
	str r5, [sp, #0]
	bl Func_02002df4
	movs r2, #0
	str r2, [sp, #16]
.L_02009696:
	ldr r3, .L_02009988
	movs r4, #0
	ldrsh r3, [r3, r4]
	cmp r3, #0
	ble .L_020096be
	movs r5, #128
	lsls r5, r5, #9
.L_020096a4:
	movs r0, #1
	bl Battle_WaitMode0
	ldr r2, .L_02009988
	adds r3, r5, #0
	movs r1, #0
	ldrsh r2, [r2, r1]
	movs r0, #128
	lsls r0, r0, #9
	asrs r3, r3, #16
	adds r5, r5, r0
	cmp r3, r2
	blt .L_020096a4
.L_020096be:
	ldr r3, .L_0200998c
	ldr r2, [r7, #4]
	ldr r0, [r3]
	movs r3, #212
	adds r2, r2, r0
	str r2, [r7, #4]
	lsls r3, r3, #1
	add r3, r9
	ldr r2, [r3]
	ldr r3, [r6, #8]
	cmp r3, #0
	bge .L_020096da
	ldr r4, .L_02009990
	adds r3, r3, r4
.L_020096da:
	asrs r1, r3, #20
	ldr r3, [r6, #16]
	cmp r3, #0
	bge .L_020096e6
	ldr r4, .L_02009990
	adds r3, r3, r4
.L_020096e6:
	asrs r3, r3, #20
	lsls r3, r3, #7
	adds r3, r1, r3
	lsls r3, r3, #2
	adds r2, r2, r3
	ldrb r3, [r2, #2]
	cmp r3, #232
	bne .L_020096fe
	ldr r3, [r6, #12]
	adds r3, r3, r0
	str r3, [r6, #12]
	str r3, [r6, #20]
.L_020096fe:
	movs r0, #1
	bl Battle_WaitMode0
	mov r0, r8
	cmp r0, #2
	bne .L_02009748
	ldr r1, [sp, #16]
	cmp r1, #0
	bne .L_02009748
	movs r3, #69
	movs r5, #11
	str r3, [sp, #4]
	movs r0, #11
	movs r1, #70
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	bl Func_02002df4
	movs r3, #68
	str r3, [sp, #4]
	movs r0, #10
	movs r1, #68
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	bl Func_02002e04
	movs r3, #5
	str r3, [sp, #4]
	movs r0, #10
	movs r1, #5
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	bl Func_02002dfc
.L_02009748:
	mov r2, r8
	cmp r2, #1
	bne .L_02009768
	ldr r3, [sp, #16]
	cmp r3, #15
	bne .L_02009768
	movs r3, #11
	movs r2, #68
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #18
	movs r1, #73
	movs r2, #1
	movs r3, #1
	bl Func_02002e04
.L_02009768:
	ldr r3, [sp, #16]
	adds r3, #1
	lsls r3, r3, #16
	asrs r3, r3, #16
	str r3, [sp, #16]
	cmp r3, #15
	ble .L_02009696
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	negs r0, r0
	negs r1, r1
	adds r2, #102
	bl Func_02002e24
	movs r0, #5
	bl Battle_WaitMode0
	movs r3, #3
	mov r4, r10
	strb r3, [r4]
	ldr r3, [r7, #4]
	movs r0, #128
	lsls r0, r0, #14
	cmp r3, r0
	bge .L_020097b0
	ldr r3, .L_02009994
	movs r2, #0
	movs r1, #144
	strh r2, [r3]
	ldr r0, .L_02009998
	lsls r1, r1, #3
	bl Func_02002d64
	b .L_020097d4
.L_020097b0:
	movs r3, #68
	movs r2, #5
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #69
	movs r1, #6
	movs r2, #1
	movs r3, #2
	bl Func_02002e04
	movs r0, #129
	lsls r0, r0, #2
	bl Func_02002dbc
	movs r0, #126
	adds r0, #255
	bl Func_02002dc4
.L_020097d4:
	movs r3, #66
	str r3, [sp, #4]
	movs r5, #7
	movs r0, #18
	movs r1, #66
	movs r2, #3
	movs r3, #1
	str r5, [sp, #0]
	bl Func_02002df4
	movs r3, #1
	str r3, [sp, #4]
	movs r2, #3
	movs r3, #5
	movs r0, #18
	movs r1, #1
	str r5, [sp, #0]
	bl Func_02002df4
	ldr r3, [r7, #4]
	ldr r2, .L_0200999c
	asrs r3, r3, #19
	str r3, [sp, #16]
	ldrb r3, [r2]
	cmp r3, #99
	beq .L_020098b4
	movs r1, #4
	mov r11, r1
	movs r7, #0
.L_0200980e:
	ldrb r3, [r2, r7]
	ldr r4, [sp, #16]
	adds r1, r2, #0
	cmp r4, r3
	blt .L_020098a6
	adds r3, r7, r2
	ldrb r3, [r3, #2]
	movs r0, #0
	mov r8, r0
	cmp r8, r3
	bge .L_020098a6
.L_02009824:
	str r7, [sp, #12]
	adds r3, r7, r1
	ldrb r3, [r3, #1]
	movs r4, #0
	mov r9, r1
	cmp r4, r3
	bge .L_02009890
	movs r1, #128
	lsls r1, r1, #9
	mov r10, r1
.L_02009838:
	ldr r2, [sp, #16]
	mov r3, r9
	adds r6, r7, r3
	mov r0, r9
	mov r1, r11
	movs r5, #14
	ldrb r3, [r6, #3]
	subs r5, r5, r2
	ldrb r2, [r0, r1]
	adds r3, r3, r4
	add r2, r8
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #21
	adds r1, r5, #0
	movs r2, #1
	movs r3, #1
	str r4, [sp, #8]
	bl Func_02002dfc
	mov r0, r9
	mov r1, r11
	ldrb r3, [r0, r1]
	ldrb r2, [r6, #3]
	ldr r4, [sp, #8]
	add r3, r8
	adds r2, r2, r4
	adds r3, #64
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r2, #1
	movs r3, #1
	movs r0, #25
	adds r1, r5, #0
	bl Func_02002dfc
	mov r3, r10
	asrs r4, r3, #16
	ldrb r3, [r6, #1]
	movs r2, #128
	lsls r2, r2, #9
	add r10, r2
	cmp r4, r3
	blt .L_02009838
.L_02009890:
	mov r3, r8
	adds r3, #1
	ldr r1, .L_0200999c
	ldr r4, [sp, #12]
	lsls r3, r3, #16
	asrs r3, r3, #16
	mov r8, r3
	adds r3, r4, r1
	ldrb r3, [r3, #2]
	cmp r8, r3
	blt .L_02009824
.L_020098a6:
	ldr r2, .L_0200999c
	adds r7, #8
	ldrb r3, [r2, r7]
	movs r0, #8
	add r11, r0
	cmp r3, #99
	bne .L_0200980e
.L_020098b4:
	ldr r1, [sp, #16]
	movs r5, #14
	subs r5, r5, r1
	movs r3, #7
	str r3, [sp, #4]
	movs r2, #2
	mov r8, r3
	movs r0, #19
	adds r1, r5, #0
	movs r3, #1
	str r2, [sp, #0]
	mov r11, r2
	bl Func_02002dfc
	mov r0, r8
	str r0, [sp, #4]
	movs r4, #13
	movs r0, #22
	adds r1, r5, #0
	movs r2, #2
	movs r3, #1
	str r4, [sp, #0]
	mov r9, r4
	bl Func_02002dfc
	mov r1, r8
	str r1, [sp, #0]
	movs r7, #5
	movs r0, #18
	adds r1, r5, #0
	movs r2, #1
	movs r3, #1
	str r7, [sp, #4]
	bl Func_02002dfc
	movs r2, #9
	str r2, [sp, #0]
	mov r10, r2
	movs r0, #24
	adds r1, r5, #0
	movs r2, #1
	movs r3, #1
	str r7, [sp, #4]
	bl Func_02002dfc
	mov r3, r11
	str r3, [sp, #0]
	movs r6, #71
	movs r0, #19
	adds r1, r5, #0
	movs r2, #2
	movs r3, #1
	str r6, [sp, #4]
	bl Func_02002dfc
	mov r4, r9
	movs r0, #22
	adds r1, r5, #0
	movs r2, #2
	movs r3, #1
	str r6, [sp, #4]
	str r4, [sp, #0]
	bl Func_02002dfc
	mov r0, r8
	str r0, [sp, #0]
	movs r6, #69
	movs r0, #26
	adds r1, r5, #0
	movs r2, #1
	movs r3, #1
	str r6, [sp, #4]
	bl Func_02002dfc
	mov r1, r10
	str r1, [sp, #0]
	movs r2, #1
	movs r0, #27
	adds r1, r5, #0
	movs r3, #1
	str r6, [sp, #4]
	bl Func_02002dfc
	ldr r2, [sp, #16]
	cmp r2, #2
	bne .L_02009972
	movs r3, #11
	str r3, [sp, #0]
	movs r0, #18
	movs r1, #8
	movs r2, #1
	movs r3, #1
	str r7, [sp, #4]
	bl Func_02002dfc
.L_02009972:
	add sp, #20
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_02009980:
	.4byte gPartyState
.L_02009984:
	.4byte gOverlayArea + 0x4030
.L_02009988:
	.4byte Data_02003780
.L_0200998c:
	.4byte Data_02003788
.L_02009990:
	.4byte 0x000fffff
.L_02009994:
	.4byte Data_02003782
.L_02009998:
	.4byte Func_020015ac
.L_0200999c:
	.4byte Data_02002fa8
	.section .text.x020099a0,"ax",%progbits
	.global Func_020019a0
	.thumb_func
Func_020019a0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r5, .L_02009d14
	movs r0, #133
	lsls r0, r0, #2
	adds r5, r5, r0
	ldr r0, [r5]
	sub sp, #20
	bl Object_GetById
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	ldr r7, .L_02009d18
	adds r6, r0, #0
	movs r1, #0
	ldr r0, [r5]
	mov r9, r3
	bl Object_SetModeById
	ldr r3, [r7, #4]
	movs r1, #85
	asrs r3, r3, #19
	adds r1, r1, r6
	mov r8, r3
	movs r3, #0
	strb r3, [r1]
	mov r10, r1
	movs r0, #160
	movs r1, #160
	movs r2, #128
	lsls r1, r1, #11
	lsls r2, r2, #9
	lsls r0, r0, #11
	bl Func_02002e24
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #139
	lsls r0, r0, #2
	bl Func_02002eec
	movs r3, #84
	str r3, [sp, #4]
	movs r5, #7
	movs r0, #22
	movs r1, #66
	movs r2, #3
	movs r3, #1
	str r5, [sp, #0]
	bl Func_02002df4
	movs r3, #19
	str r3, [sp, #4]
	movs r2, #3
	movs r0, #22
	movs r1, #1
	movs r3, #5
	str r5, [sp, #0]
	bl Func_02002df4
	movs r2, #0
	str r2, [sp, #16]
.L_02009a2a:
	ldr r3, .L_02009d1c
	movs r4, #0
	ldrsh r3, [r3, r4]
	cmp r3, #0
	ble .L_02009a52
	movs r5, #128
	lsls r5, r5, #9
.L_02009a38:
	movs r0, #1
	bl Battle_WaitMode0
	ldr r2, .L_02009d1c
	adds r3, r5, #0
	movs r1, #0
	ldrsh r2, [r2, r1]
	movs r0, #128
	lsls r0, r0, #9
	asrs r3, r3, #16
	adds r5, r5, r0
	cmp r3, r2
	blt .L_02009a38
.L_02009a52:
	ldr r3, .L_02009d20
	ldr r2, [r7, #4]
	ldr r0, [r3]
	movs r3, #212
	adds r2, r2, r0
	str r2, [r7, #4]
	lsls r3, r3, #1
	add r3, r9
	ldr r2, [r3]
	ldr r3, [r6, #8]
	cmp r3, #0
	bge .L_02009a6e
	ldr r4, .L_02009d24
	adds r3, r3, r4
.L_02009a6e:
	asrs r1, r3, #20
	ldr r3, [r6, #16]
	cmp r3, #0
	bge .L_02009a7a
	ldr r4, .L_02009d24
	adds r3, r3, r4
.L_02009a7a:
	asrs r3, r3, #20
	lsls r3, r3, #7
	adds r3, r1, r3
	lsls r3, r3, #2
	adds r2, r2, r3
	ldrb r3, [r2, #2]
	cmp r3, #232
	bne .L_02009a92
	ldr r3, [r6, #12]
	adds r3, r3, r0
	str r3, [r6, #12]
	str r3, [r6, #20]
.L_02009a92:
	movs r0, #1
	bl Battle_WaitMode0
	mov r0, r8
	cmp r0, #2
	bne .L_02009ade
	ldr r1, [sp, #16]
	cmp r1, #0
	bne .L_02009ade
	movs r3, #11
	movs r2, #69
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #11
	movs r1, #70
	movs r2, #1
	movs r3, #1
	bl Func_02002df4
	movs r3, #86
	movs r5, #4
	str r3, [sp, #4]
	movs r0, #10
	movs r1, #68
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	bl Func_02002e04
	movs r3, #23
	str r3, [sp, #4]
	movs r0, #4
	movs r1, #24
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	bl Func_02002dfc
.L_02009ade:
	mov r2, r8
	cmp r2, #1
	bne .L_02009afe
	ldr r3, [sp, #16]
	cmp r3, #15
	bne .L_02009afe
	movs r3, #4
	movs r2, #86
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #18
	movs r1, #73
	movs r2, #1
	movs r3, #1
	bl Func_02002e04
.L_02009afe:
	ldr r3, [sp, #16]
	adds r3, #1
	lsls r3, r3, #16
	asrs r3, r3, #16
	str r3, [sp, #16]
	cmp r3, #15
	ble .L_02009a2a
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	negs r0, r0
	negs r1, r1
	adds r2, #102
	bl Func_02002e24
	movs r0, #5
	bl Battle_WaitMode0
	movs r3, #3
	mov r4, r10
	strb r3, [r4]
	ldr r3, [r7, #4]
	movs r0, #128
	lsls r0, r0, #14
	cmp r3, r0
	bge .L_02009b46
	ldr r3, .L_02009d28
	movs r2, #0
	movs r1, #144
	strh r2, [r3]
	ldr r0, .L_02009d2c
	lsls r1, r1, #3
	bl Func_02002d64
	b .L_02009b6a
.L_02009b46:
	movs r3, #68
	movs r2, #23
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #69
	movs r1, #24
	movs r2, #1
	movs r3, #4
	bl Func_02002e04
	movs r0, #129
	lsls r0, r0, #2
	bl Func_02002dbc
	movs r0, #126
	adds r0, #255
	bl Func_02002dc4
.L_02009b6a:
	movs r3, #84
	str r3, [sp, #4]
	movs r5, #7
	movs r0, #18
	movs r1, #66
	movs r2, #3
	movs r3, #1
	str r5, [sp, #0]
	bl Func_02002df4
	movs r3, #19
	str r3, [sp, #4]
	movs r2, #3
	movs r3, #5
	movs r0, #18
	movs r1, #1
	str r5, [sp, #0]
	bl Func_02002df4
	ldr r3, [r7, #4]
	ldr r2, .L_02009d30
	asrs r3, r3, #19
	str r3, [sp, #16]
	ldrb r3, [r2]
	cmp r3, #99
	beq .L_02009c4a
	movs r1, #4
	mov r11, r1
	movs r7, #0
.L_02009ba4:
	ldrb r3, [r2, r7]
	ldr r4, [sp, #16]
	adds r1, r2, #0
	cmp r4, r3
	blt .L_02009c3c
	adds r3, r7, r2
	ldrb r3, [r3, #2]
	movs r0, #0
	mov r8, r0
	cmp r8, r3
	bge .L_02009c3c
.L_02009bba:
	str r7, [sp, #12]
	adds r3, r7, r1
	ldrb r3, [r3, #1]
	movs r4, #0
	mov r9, r1
	cmp r4, r3
	bge .L_02009c26
	movs r1, #128
	lsls r1, r1, #9
	mov r10, r1
.L_02009bce:
	ldr r2, [sp, #16]
	mov r3, r9
	adds r6, r7, r3
	mov r0, r9
	mov r1, r11
	movs r5, #14
	ldrb r3, [r6, #3]
	subs r5, r5, r2
	ldrb r2, [r0, r1]
	adds r3, r3, r4
	add r2, r8
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #21
	adds r1, r5, #0
	movs r2, #1
	movs r3, #1
	str r4, [sp, #8]
	bl Func_02002dfc
	mov r0, r9
	mov r1, r11
	ldrb r3, [r0, r1]
	ldrb r2, [r6, #3]
	ldr r4, [sp, #8]
	add r3, r8
	adds r2, r2, r4
	adds r3, #64
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r2, #1
	movs r3, #1
	movs r0, #25
	adds r1, r5, #0
	bl Func_02002dfc
	mov r3, r10
	asrs r4, r3, #16
	ldrb r3, [r6, #1]
	movs r2, #128
	lsls r2, r2, #9
	add r10, r2
	cmp r4, r3
	blt .L_02009bce
.L_02009c26:
	mov r3, r8
	adds r3, #1
	ldr r1, .L_02009d30
	ldr r4, [sp, #12]
	lsls r3, r3, #16
	asrs r3, r3, #16
	mov r8, r3
	adds r3, r4, r1
	ldrb r3, [r3, #2]
	cmp r8, r3
	blt .L_02009bba
.L_02009c3c:
	ldr r2, .L_02009d30
	adds r7, #8
	ldrb r3, [r2, r7]
	movs r0, #8
	add r11, r0
	cmp r3, #99
	bne .L_02009ba4
.L_02009c4a:
	ldr r1, [sp, #16]
	movs r5, #14
	subs r5, r5, r1
	movs r3, #27
	str r3, [sp, #4]
	movs r2, #2
	movs r0, #19
	adds r1, r5, #0
	movs r3, #1
	str r2, [sp, #0]
	mov r9, r2
	bl Func_02002dfc
	movs r3, #25
	str r3, [sp, #4]
	movs r6, #13
	movs r0, #19
	adds r1, r5, #0
	movs r2, #2
	movs r3, #1
	str r6, [sp, #0]
	bl Func_02002dfc
	movs r3, #7
	str r3, [sp, #0]
	mov r10, r3
	movs r7, #23
	movs r0, #18
	adds r1, r5, #0
	movs r2, #1
	movs r3, #1
	str r7, [sp, #4]
	bl Func_02002dfc
	movs r4, #9
	movs r0, #24
	adds r1, r5, #0
	movs r2, #1
	movs r3, #1
	mov r8, r4
	str r4, [sp, #0]
	str r7, [sp, #4]
	bl Func_02002dfc
	movs r3, #91
	mov r0, r9
	str r0, [sp, #0]
	str r3, [sp, #4]
	movs r0, #19
	adds r1, r5, #0
	movs r2, #2
	movs r3, #1
	bl Func_02002dfc
	movs r3, #89
	str r3, [sp, #4]
	movs r0, #19
	adds r1, r5, #0
	movs r2, #2
	movs r3, #1
	str r6, [sp, #0]
	bl Func_02002dfc
	mov r1, r10
	str r1, [sp, #0]
	movs r6, #87
	movs r0, #26
	adds r1, r5, #0
	movs r2, #1
	movs r3, #1
	str r6, [sp, #4]
	bl Func_02002dfc
	mov r2, r8
	str r2, [sp, #0]
	movs r3, #1
	movs r0, #27
	adds r1, r5, #0
	movs r2, #1
	str r6, [sp, #4]
	bl Func_02002dfc
	ldr r3, [sp, #16]
	cmp r3, #2
	bne .L_02009d06
	movs r3, #4
	str r3, [sp, #0]
	movs r0, #18
	movs r1, #7
	movs r2, #1
	movs r3, #1
	str r7, [sp, #4]
	bl Func_02002dfc
.L_02009d06:
	add sp, #20
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_02009d14:
	.4byte gPartyState
.L_02009d18:
	.4byte gOverlayArea + 0x4030
.L_02009d1c:
	.4byte Data_02003780
.L_02009d20:
	.4byte Data_02003788
.L_02009d24:
	.4byte 0x000fffff
.L_02009d28:
	.4byte Data_02003782
.L_02009d2c:
	.4byte Func_020015ac
.L_02009d30:
	.4byte Data_02002ff0
	.section .text.x02009d34,"ax",%progbits
	.global Func_02001d34
	.thumb_func
Func_02001d34:
	push {r5, r6, lr}
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #2
	sub sp, #8
	bl Func_02002db4
	adds r6, r0, #0
	cmp r6, #0
	bne .L_02009dd8
	movs r0, #0
	bl Func_02002edc
	movs r0, #146
	lsls r0, r0, #2
	bl Func_02002eec
	movs r0, #15
	bl Battle_WaitMode0
	movs r0, #202
	bl Func_02002eec
	movs r3, #66
	str r3, [sp, #4]
	movs r5, #7
	movs r3, #1
	movs r0, #18
	movs r1, #66
	movs r2, #3
	str r5, [sp, #0]
	bl Func_02002df4
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #10
	lsls r2, r2, #9
	lsls r0, r0, #10
	bl Func_02002e24
	movs r0, #15
	bl Battle_WaitMode0
	ldr r3, .L_02009dc4
	ldr r2, .L_02009dc8
	strh r5, [r3]
	ldr r3, .L_02009dcc
	movs r0, #126
	strh r6, [r3]
	ldr r3, .L_02009dc0
	adds r0, #255
	strh r3, [r2]
	ldr r2, .L_02009dd0
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r2]
	ldr r2, .L_02009dd4
	movs r3, #1
	str r3, [r2]
	bl Func_02002dbc
	bl Func_0200160c
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #2
	bl Func_02002dbc
	b .L_02009dd8
.L_02009dc0:
	.4byte 0x000000c8
.L_02009dc4:
	.4byte Data_02003780
.L_02009dc8:
	.4byte Data_02003784
.L_02009dcc:
	.4byte Data_02003782
.L_02009dd0:
	.4byte Data_02003788
.L_02009dd4:
	.4byte Data_0200378c
.L_02009dd8:
	add sp, #8
	pop {r5, r6, pc}
	.section .text.x02009ddc,"ax",%progbits
	.global Func_02001ddc
	.thumb_func
Func_02001ddc:
	push {r5, r6, lr}
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #226
	sub sp, #8
	bl Func_02002db4
	cmp r0, #0
	bne .L_02009e50
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #226
	bl Func_02002dbc
	movs r0, #146
	lsls r0, r0, #2
	bl Func_02002eec
	bl Func_02002e44
	movs r0, #0
	bl Func_02002edc
	movs r5, #14
	movs r1, #70
	movs r2, #1
	movs r3, #3
	movs r6, #65
	movs r0, #26
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_02002df4
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #181
	bl Func_02002eec
	movs r0, #26
	movs r1, #67
	movs r2, #1
	movs r3, #3
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_02002df4
	movs r3, #5
	str r3, [sp, #4]
	movs r0, #15
	movs r1, #5
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	bl Func_02002dfc
	bl Func_02002e4c
.L_02009e50:
	add sp, #8
	pop {r5, r6, pc}
	.section .text.x02009e54,"ax",%progbits
	.global Func_02001e54
	.thumb_func
Func_02001e54:
	push {r5, lr}
	movs r0, #130
	lsls r0, r0, #1
	adds r0, #255
	sub sp, #8
	bl Func_02002db4
	adds r5, r0, #0
	cmp r5, #0
	bne .L_02009f00
	movs r0, #0
	bl Func_02002edc
	movs r0, #146
	lsls r0, r0, #2
	bl Func_02002eec
	movs r0, #15
	bl Battle_WaitMode0
	movs r0, #202
	bl Func_02002eec
	movs r3, #7
	movs r2, #84
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #1
	movs r0, #18
	movs r1, #66
	movs r2, #3
	bl Func_02002df4
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #10
	lsls r2, r2, #9
	lsls r0, r0, #10
	bl Func_02002e24
	movs r0, #15
	bl Battle_WaitMode0
	ldr r2, .L_02009eec
	ldr r3, .L_02009ee4
	movs r0, #126
	strh r3, [r2]
	ldr r3, .L_02009ef0
	ldr r2, .L_02009ef4
	strh r5, [r3]
	ldr r3, .L_02009ee8
	adds r0, #255
	strh r3, [r2]
	ldr r2, .L_02009ef8
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r2]
	ldr r2, .L_02009efc
	movs r3, #2
	str r3, [r2]
	bl Func_02002dbc
	bl Func_020019a0
	movs r0, #130
	lsls r0, r0, #1
	adds r0, #255
	bl Func_02002dbc
	b .L_02009f00
	.2byte 0x0000
.L_02009ee4:
	.4byte 0x00000003
.L_02009ee8:
	.4byte 0x00000064
.L_02009eec:
	.4byte Data_02003780
.L_02009ef0:
	.4byte Data_02003782
.L_02009ef4:
	.4byte Data_02003784
.L_02009ef8:
	.4byte Data_02003788
.L_02009efc:
	.4byte Data_0200378c
.L_02009f00:
	add sp, #8
	pop {r5, pc}
	.section .text.x02009f04,"ax",%progbits
	.global Func_02001f04
	.thumb_func
Func_02001f04:
	push {r5, r6, lr}
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #227
	sub sp, #8
	bl Func_02002db4
	cmp r0, #0
	bne .L_02009f78
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #227
	bl Func_02002dbc
	movs r0, #146
	lsls r0, r0, #2
	bl Func_02002eec
	bl Func_02002e44
	movs r0, #0
	bl Func_02002edc
	movs r5, #2
	movs r1, #70
	movs r2, #1
	movs r3, #3
	movs r6, #83
	movs r0, #26
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_02002df4
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #181
	bl Func_02002eec
	movs r0, #26
	movs r1, #67
	movs r2, #1
	movs r3, #3
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_02002df4
	movs r3, #23
	str r3, [sp, #4]
	movs r0, #1
	movs r1, #23
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	bl Func_02002dfc
	bl Func_02002e4c
.L_02009f78:
	add sp, #8
	pop {r5, r6, pc}
	.section .text.x02009f7c,"ax",%progbits
	.global Func_02001f7c
	.thumb_func
Func_02001f7c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r5, #192
	lsls r5, r5, #18
	ldr r3, [r5, #108]
	movs r1, #230
	lsls r1, r1, #1
	adds r3, r3, r1
	ldr r3, [r3]
	movs r0, #9
	sub sp, #8
	mov r10, r3
	bl Object_GetById
	ldr r2, .L_0200a0ec
	movs r3, #133
	mov r8, r2
	lsls r3, r3, #2
	add r3, r8
	adds r7, r0, #0
	ldr r0, [r3]
	bl Object_GetById
	adds r6, r0, #0
	movs r0, #130
	lsls r0, r0, #1
	ldr r5, [r5, #108]
	bl Func_02002db4
	cmp r0, #0
	beq .L_02009fce
	movs r1, #170
	lsls r1, r1, #1
	adds r3, r5, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #40
	beq .L_02009fce
	b .L_0200a0e0
.L_02009fce:
	ldr r1, [r7, #16]
	movs r3, #204
	lsls r3, r3, #8
	adds r3, #204
	adds r0, r1, r3
	str r0, [r7, #16]
	ldr r3, [r6, #8]
	asrs r2, r3, #20
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #18
	add r3, r8
	ldrb r3, [r3]
	cmp r3, #5
	beq .L_0200a02a
	cmp r2, #12
	ble .L_0200a02a
	cmp r2, #15
	bgt .L_0200a02a
	ldr r3, .L_0200a0f0
	adds r2, r1, r3
	ldr r3, [r6, #16]
	cmp r2, r3
	blt .L_0200a010
	cmp r3, r0
	blt .L_0200a010
	str r2, [r6, #16]
	ldr r3, [r7, #16]
	movs r1, #192
	lsls r1, r1, #13
	adds r3, r3, r1
	mov r2, r10
	str r3, [r2, #16]
.L_0200a010:
	ldr r3, [r7, #16]
	ldr r2, [r6, #16]
	cmp r3, r2
	blt .L_0200a02a
	ldr r1, .L_0200a0f4
	adds r3, r3, r1
	cmp r2, r3
	blt .L_0200a02a
	str r3, [r6, #16]
	ldr r3, [r7, #16]
	mov r2, r10
	adds r3, r3, r1
	str r3, [r2, #16]
.L_0200a02a:
	ldr r3, [r7, #16]
	movs r1, #128
	asrs r5, r3, #20
	adds r3, r5, #0
	subs r3, #31
	lsls r3, r3, #16
	lsls r1, r1, #11
	cmp r3, r1
	bhi .L_0200a064
	movs r0, #163
	lsls r0, r0, #1
	bl Func_02002db4
	cmp r0, #0
	bne .L_0200a064
	movs r3, #12
	movs r2, #96
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #35
	movs r1, #83
	movs r2, #5
	movs r3, #4
	bl Func_02002dfc
	movs r0, #163
	lsls r0, r0, #1
	bl Func_02002dbc
.L_0200a064:
	cmp r5, #37
	bne .L_0200a090
	movs r0, #163
	lsls r0, r0, #1
	bl Func_02002db4
	cmp r0, #0
	beq .L_0200a090
	movs r3, #12
	movs r2, #96
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #40
	movs r1, #83
	movs r2, #5
	movs r3, #4
	bl Func_02002dfc
	movs r0, #163
	lsls r0, r0, #1
	bl Func_02002dc4
.L_0200a090:
	cmp r5, #41
	ble .L_0200a0e0
	ldr r0, .L_0200a0f8
	bl Scheduler_RemoveCallbackFar
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	negs r0, r0
	negs r1, r1
	adds r2, #102
	bl Func_02002e24
	movs r3, #13
	movs r2, #41
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #48
	movs r2, #3
	movs r3, #2
	movs r0, #41
	bl Func_02002dfc
	movs r0, #129
	lsls r0, r0, #1
	adds r0, #255
	bl Func_02002dbc
	movs r0, #126
	adds r0, #255
	bl Func_02002dc4
	movs r0, #98
	adds r0, #255
	bl Func_02002dc4
	movs r0, #208
	bl Func_02002eec
.L_0200a0e0:
	add sp, #8
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200a0ec:
	.4byte gPartyState
.L_0200a0f0:
	.4byte 0x0018cccc
.L_0200a0f4:
	.4byte 0xfff00000
.L_0200a0f8:
	.4byte Func_02001f7c
	.section .text.x0200a0fc,"ax",%progbits
	.global Func_020020fc
	.thumb_func
Func_020020fc:
	push {r5, r6, r7, lr}
	movs r0, #9
	sub sp, #8
	bl Object_GetById
	ldr r3, [r0, #16]
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #221
	asrs r6, r3, #20
	bl Func_02002db4
	cmp r0, #0
	beq .L_0200a1c6
	cmp r6, #24
	bne .L_0200a1c6
	ldr r7, .L_0200a1cc
	adds r0, r7, #0
	bl Func_02002dcc
	movs r2, #1
	adds r5, r0, #0
	negs r2, r2
	cmp r5, r2
	bne .L_0200a1c6
	bl Func_02002e44
	movs r0, #0
	bl Func_02002edc
	movs r0, #207
	bl Func_02002eec
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #10
	lsls r2, r2, #9
	lsls r0, r0, #10
	bl Func_02002e24
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #232
	movs r2, #196
	lsls r2, r2, #17
	movs r3, #1
	lsls r0, r0, #16
	adds r1, r5, #0
	bl Func_02002eac
	bl Func_02002eb4
	movs r1, #144
	lsls r1, r1, #3
	adds r0, r7, #0
	bl Func_02002d64
	movs r0, #40
	bl Battle_WaitMode0
	ldr r3, .L_0200a1d0
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	movs r1, #1
	bl Func_02002ea4
	bl Func_02002eb4
	movs r3, #13
	str r3, [sp, #0]
	movs r0, #13
	movs r1, #30
	movs r2, #3
	movs r3, #2
	str r6, [sp, #4]
	bl Func_02002dfc
	movs r3, #14
	movs r2, #88
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #87
	movs r2, #1
	movs r3, #1
	movs r0, #14
	bl Func_02002dfc
	movs r0, #126
	adds r0, #255
	bl Func_02002dbc
	movs r0, #98
	adds r0, #255
	bl Func_02002dbc
	bl Func_02002e4c
.L_0200a1c6:
	add sp, #8
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200a1cc:
	.4byte Func_02001f7c
.L_0200a1d0:
	.4byte gPartyState
	.section .text.x0200a1d4,"ax",%progbits
	.global Func_020021d4
	.thumb_func
Func_020021d4:
	push {r5, r6, lr}
	movs r0, #9
	sub sp, #8
	bl Object_GetById
	ldr r3, [r0, #16]
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #221
	asrs r6, r3, #20
	bl Func_02002db4
	cmp r0, #0
	beq .L_0200a258
	cmp r6, #24
	bne .L_0200a258
	ldr r5, .L_0200a25c
	adds r0, r5, #0
	bl Func_02002dcc
	movs r3, #1
	negs r3, r3
	cmp r0, r3
	bne .L_0200a258
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #9
	lsls r0, r0, #10
	lsls r1, r1, #10
	bl Func_02002e24
	movs r1, #144
	lsls r1, r1, #3
	adds r0, r5, #0
	bl Func_02002d64
	movs r3, #13
	str r3, [sp, #0]
	movs r0, #13
	movs r1, #30
	movs r2, #3
	movs r3, #2
	str r6, [sp, #4]
	bl Func_02002dfc
	movs r3, #14
	movs r2, #88
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #87
	movs r2, #1
	movs r3, #1
	movs r0, #14
	bl Func_02002dfc
	movs r0, #126
	adds r0, #255
	bl Func_02002dbc
	movs r0, #98
	adds r0, #255
	bl Func_02002dbc
	bl Func_02002e4c
.L_0200a258:
	add sp, #8
	pop {r5, r6, pc}
.L_0200a25c:
	.4byte Func_02001f7c
	.section .text.x0200a260,"ax",%progbits
	.global Func_02002260
	.thumb_func
Func_02002260:
	push {lr}
	movs r0, #9
	bl Object_GetById
	ldr r3, [r0, #8]
	asrs r3, r3, #20
	cmp r3, #26
	bne .L_0200a2a4
	ldr r3, [r0, #16]
	asrs r3, r3, #20
	cmp r3, #35
	bne .L_0200a2a4
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #219
	bl Func_02002dbc
	movs r0, #204
	bl Func_02002eec
	movs r0, #9
	movs r1, #0
	bl Func_02000b30
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_02002e6c
	bl Func_02002ee4
	movs r0, #20
	bl Func_02002ebc
.L_0200a2a4:
	pop {pc}
	.2byte 0x0000
	.section .text.x0200a2a8,"ax",%progbits
	.global Func_020022a8
	.thumb_func
Func_020022a8:
	push {r5, lr}
	movs r0, #131
	lsls r0, r0, #1
	adds r0, #255
	sub sp, #8
	bl Func_02002dbc
	movs r3, #34
	str r3, [sp, #4]
	movs r5, #25
	movs r0, #35
	movs r1, #34
	movs r2, #3
	movs r3, #5
	str r5, [sp, #0]
	bl Func_02002df4
	movs r3, #98
	str r3, [sp, #4]
	movs r0, #35
	movs r1, #98
	movs r2, #3
	movs r3, #3
	str r5, [sp, #0]
	bl Func_02002df4
	add sp, #8
	pop {r5, pc}
	.section .text.x0200a2e0,"ax",%progbits
	.global Func_020022e0
	.thumb_func
Func_020022e0:
	push {r5, lr}
	movs r0, #131
	lsls r0, r0, #1
	adds r0, #255
	sub sp, #8
	bl Func_02002dc4
	movs r3, #34
	str r3, [sp, #4]
	movs r5, #25
	movs r0, #40
	movs r1, #34
	movs r2, #3
	movs r3, #5
	str r5, [sp, #0]
	bl Func_02002df4
	movs r3, #98
	str r3, [sp, #4]
	movs r0, #40
	movs r1, #98
	movs r2, #3
	movs r3, #3
	str r5, [sp, #0]
	bl Func_02002df4
	add sp, #8
	pop {r5, pc}
	.section .text.x0200a318,"ax",%progbits
	.global Func_02002318
	.thumb_func
Func_02002318:
	push {lr}
	movs r0, #131
	lsls r0, r0, #1
	adds r0, #255
	bl Func_02002dbc
	pop {pc}
	.2byte 0x0000
	.section .text.x0200a328,"ax",%progbits
	.global Func_02002328
	.thumb_func
Func_02002328:
	push {lr}
	movs r0, #131
	lsls r0, r0, #1
	adds r0, #255
	bl Func_02002dc4
	pop {pc}
	.2byte 0x0000
	.section .text.x0200a338,"ax",%progbits
	.global Func_02002338
	.thumb_func
Func_02002338:
	push {lr}
	ldr r3, .L_0200a390
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_0200a394
	cmp r2, r3
	bne .L_0200a350
	ldr r0, .L_0200a398
	b .L_0200a38e
.L_0200a350:
	ldr r3, .L_0200a39c
	cmp r2, r3
	bne .L_0200a35a
	ldr r0, .L_0200a3a0
	b .L_0200a38e
.L_0200a35a:
	ldr r3, .L_0200a3a4
	cmp r2, r3
	bne .L_0200a364
	ldr r0, .L_0200a3a8
	b .L_0200a38e
.L_0200a364:
	ldr r3, .L_0200a3ac
	cmp r2, r3
	bne .L_0200a36e
	ldr r0, .L_0200a3b0
	b .L_0200a38e
.L_0200a36e:
	ldr r3, .L_0200a3b4
	cmp r2, r3
	bne .L_0200a378
	ldr r0, .L_0200a3b8
	b .L_0200a38e
.L_0200a378:
	ldr r3, .L_0200a3bc
	cmp r2, r3
	bne .L_0200a382
	ldr r0, .L_0200a3c0
	b .L_0200a38e
.L_0200a382:
	ldr r3, .L_0200a3c4
	cmp r2, r3
	bne .L_0200a38c
	ldr r0, .L_0200a3c8
	b .L_0200a38e
.L_0200a38c:
	ldr r0, .L_0200a3cc
.L_0200a38e:
	pop {pc}
.L_0200a390:
	.4byte gPartyState
.L_0200a394:
	.4byte 0x000000bd
.L_0200a398:
	.4byte Data_020037b4
.L_0200a39c:
	.4byte 0x000000be
.L_0200a3a0:
	.4byte Data_02003a78
.L_0200a3a4:
	.4byte 0x000000bf
.L_0200a3a8:
	.4byte Data_02003b98
.L_0200a3ac:
	.4byte 0x000000c0
.L_0200a3b0:
	.4byte Data_02003c94
.L_0200a3b4:
	.4byte 0x000000c1
.L_0200a3b8:
	.4byte Data_02003e38
.L_0200a3bc:
	.4byte 0x000000c2
.L_0200a3c0:
	.4byte Data_02003f4c
.L_0200a3c4:
	.4byte 0x000000c4
.L_0200a3c8:
	.4byte Data_02004018
.L_0200a3cc:
	.4byte Data_02003790
	.section .text.x0200a3d0,"ax",%progbits
	.global Func_020023d0
	.thumb_func
Func_020023d0:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r3, .L_0200a434
	movs r6, #7
	ldr r7, [r3]
	sub sp, #56
	ands r7, r6
	mov r8, r0
	cmp r7, #0
	bne .L_0200a42a
	bl Random16Far
	movs r5, #15
	ands r5, r0
	bl Random16Far
	movs r3, #209
	lsls r3, r3, #1
	ands r0, r6
	adds r3, #255
	add r6, sp, #16
	strh r3, [r6, #24]
	mov r3, r8
	ldr r4, [r3, #8]
	ldr r1, [r3, #12]
	ldr r2, [r3, #16]
	movs r3, #128
	lsls r3, r3, #8
	subs r5, #8
	lsls r5, r5, #16
	subs r0, #8
	str r3, [sp, #0]
	movs r3, #128
	lsls r0, r0, #16
	lsls r3, r3, #13
	adds r4, r4, r5
	adds r1, r1, r0
	str r3, [sp, #8]
	adds r0, r4, #0
	movs r3, #0
	str r7, [sp, #4]
	str r6, [sp, #12]
	bl Func_0200015c
.L_0200a42a:
	add sp, #56
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200a434:
	.4byte Data_0300122c
	.section .text.x0200a438,"ax",%progbits
	.global Func_02002438
	.thumb_func
Func_02002438:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r7, .L_0200a74c
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r7, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_0200a750
	sub sp, #8
	cmp r2, r3
	beq .L_0200a456
	b .L_0200a686
.L_0200a456:
	movs r0, #0
	bl Func_02002ed4
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #210
	bl Func_02002db4
	cmp r0, #0
	beq .L_0200a474
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_02002e6c
.L_0200a474:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #211
	bl Func_02002db4
	cmp r0, #0
	beq .L_0200a48c
	movs r0, #11
	movs r1, #0
	movs r2, #0
	bl Func_02002e6c
.L_0200a48c:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #212
	bl Func_02002db4
	cmp r0, #0
	beq .L_0200a4a4
	movs r0, #12
	movs r1, #0
	movs r2, #0
	bl Func_02002e6c
.L_0200a4a4:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #213
	bl Func_02002db4
	cmp r0, #0
	beq .L_0200a4bc
	movs r0, #13
	movs r1, #0
	movs r2, #0
	bl Func_02002e6c
.L_0200a4bc:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #214
	bl Func_02002db4
	cmp r0, #0
	beq .L_0200a4d4
	movs r0, #14
	movs r1, #0
	movs r2, #0
	bl Func_02002e6c
.L_0200a4d4:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #215
	bl Func_02002db4
	cmp r0, #0
	beq .L_0200a4ec
	movs r0, #15
	movs r1, #0
	movs r2, #0
	bl Func_02002e6c
.L_0200a4ec:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #216
	bl Func_02002db4
	cmp r0, #0
	beq .L_0200a504
	movs r0, #16
	movs r1, #0
	movs r2, #0
	bl Func_02002e6c
.L_0200a504:
	movs r0, #153
	lsls r0, r0, #4
	bl Func_02002db4
	cmp r0, #0
	beq .L_0200a51a
	movs r0, #17
	movs r1, #0
	movs r2, #0
	bl Func_02002e6c
.L_0200a51a:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #145
	bl Func_02002db4
	cmp r0, #0
	beq .L_0200a532
	movs r0, #18
	movs r1, #0
	movs r2, #0
	bl Func_02002e6c
.L_0200a532:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #146
	bl Func_02002db4
	cmp r0, #0
	beq .L_0200a54a
	movs r0, #19
	movs r1, #0
	movs r2, #0
	bl Func_02002e6c
.L_0200a54a:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #147
	bl Func_02002db4
	cmp r0, #0
	beq .L_0200a562
	movs r0, #20
	movs r1, #0
	movs r2, #0
	bl Func_02002e6c
.L_0200a562:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #148
	bl Func_02002db4
	cmp r0, #0
	beq .L_0200a57a
	movs r0, #21
	movs r1, #0
	movs r2, #0
	bl Func_02002e6c
.L_0200a57a:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #149
	bl Func_02002db4
	cmp r0, #0
	beq .L_0200a592
	movs r0, #22
	movs r1, #0
	movs r2, #0
	bl Func_02002e6c
.L_0200a592:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #150
	bl Func_02002db4
	cmp r0, #0
	beq .L_0200a5aa
	movs r0, #23
	movs r1, #0
	movs r2, #0
	bl Func_02002e6c
.L_0200a5aa:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #151
	bl Func_02002db4
	cmp r0, #0
	beq .L_0200a5c2
	movs r0, #24
	movs r1, #0
	movs r2, #0
	bl Func_02002e6c
.L_0200a5c2:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #152
	bl Func_02002db4
	cmp r0, #0
	beq .L_0200a5da
	movs r0, #25
	movs r1, #0
	movs r2, #0
	bl Func_02002e6c
.L_0200a5da:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #153
	bl Func_02002db4
	cmp r0, #0
	beq .L_0200a5f2
	movs r0, #26
	movs r1, #0
	movs r2, #0
	bl Func_02002e6c
.L_0200a5f2:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #154
	bl Func_02002db4
	cmp r0, #0
	beq .L_0200a60a
	movs r0, #27
	movs r1, #0
	movs r2, #0
	bl Func_02002e6c
.L_0200a60a:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #155
	bl Func_02002db4
	cmp r0, #0
	beq .L_0200a622
	movs r0, #28
	movs r1, #0
	movs r2, #0
	bl Func_02002e6c
.L_0200a622:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #156
	bl Func_02002db4
	cmp r0, #0
	beq .L_0200a63a
	movs r0, #29
	movs r1, #0
	movs r2, #0
	bl Func_02002e6c
.L_0200a63a:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #157
	bl Func_02002db4
	cmp r0, #0
	beq .L_0200a652
	movs r0, #30
	movs r1, #0
	movs r2, #0
	bl Func_02002e6c
.L_0200a652:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #198
	bl Func_02002db4
	cmp r0, #0
	bne .L_0200a662
	b .L_0200ad2a
.L_0200a662:
	movs r1, #144
	movs r2, #176
	movs r0, #8
	lsls r1, r1, #15
	lsls r2, r2, #15
	bl Func_02002e6c
	movs r3, #2
	movs r2, #69
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #4
	movs r1, #68
	movs r2, #1
	movs r3, #1
	bl Func_02002dfc
	b .L_0200ad2a
.L_0200a686:
	ldr r3, .L_0200a754
	cmp r2, r3
	bne .L_0200a758
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #158
	bl Func_02002db4
	cmp r0, #0
	beq .L_0200a6a4
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_02002e6c
.L_0200a6a4:
	movs r0, #138
	lsls r0, r0, #4
	adds r0, #255
	bl Func_02002db4
	cmp r0, #0
	beq .L_0200a6bc
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_02002e6c
.L_0200a6bc:
	movs r0, #154
	lsls r0, r0, #4
	bl Func_02002db4
	cmp r0, #0
	beq .L_0200a6d2
	movs r0, #11
	movs r1, #0
	movs r2, #0
	bl Func_02002e6c
.L_0200a6d2:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #220
	bl Func_02002db4
	cmp r0, #0
	bne .L_0200a6e2
	b .L_0200ad2a
.L_0200a6e2:
	movs r0, #8
	bl Object_GetById
	adds r5, r0, #0
	adds r2, r5, #0
	movs r3, #3
	adds r2, #85
	strb r3, [r2]
	movs r1, #200
	movs r2, #176
	movs r0, #8
	lsls r1, r1, #16
	lsls r2, r2, #15
	bl Func_02002e6c
	movs r3, #7
	str r3, [sp, #4]
	movs r5, #10
	movs r0, #35
	movs r1, #6
	movs r2, #7
	movs r3, #16
	str r5, [sp, #0]
	bl Func_02002df4
	movs r3, #71
	str r3, [sp, #4]
	movs r1, #70
	movs r2, #7
	movs r3, #16
	movs r0, #35
	str r5, [sp, #0]
	bl Func_02002df4
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r7, r2
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #6
	beq .L_0200a73e
	cmp r3, #17
	beq .L_0200a73e
	cmp r3, #15
	beq .L_0200a73e
	b .L_0200ad2a
.L_0200a73e:
	movs r0, #160
	lsls r0, r0, #1
	adds r0, #255
	bl Func_02002eec
	b .L_0200ad2a
	.2byte 0x0000
.L_0200a74c:
	.4byte gPartyState
.L_0200a750:
	.4byte 0x000000bd
.L_0200a754:
	.4byte 0x000000be
.L_0200a758:
	ldr r3, .L_0200aadc
	cmp r2, r3
	bne .L_0200a7fa
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #199
	bl Func_02002db4
	cmp r0, #0
	beq .L_0200a78e
	movs r1, #130
	movs r2, #208
	movs r0, #8
	lsls r1, r1, #18
	lsls r2, r2, #15
	bl Func_02002e6c
	movs r3, #33
	movs r2, #69
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #32
	movs r1, #69
	movs r2, #1
	movs r3, #1
	bl Func_02002dfc
.L_0200a78e:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #221
	bl Func_02002db4
	cmp r0, #0
	beq .L_0200a7ae
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_02002e6c
	movs r0, #9
	movs r1, #4
	bl Object_SetModeById
.L_0200a7ae:
	movs r0, #129
	lsls r0, r0, #1
	adds r0, #255
	bl Func_02002db4
	cmp r0, #0
	bne .L_0200a7be
	b .L_0200ad2a
.L_0200a7be:
	movs r3, #24
	str r3, [sp, #4]
	movs r5, #13
	movs r0, #13
	movs r1, #30
	movs r2, #3
	movs r3, #2
	str r5, [sp, #0]
	bl Func_02002dfc
	movs r3, #14
	movs r2, #88
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #14
	movs r1, #87
	movs r2, #1
	movs r3, #1
	bl Func_02002dfc
	movs r3, #41
	str r3, [sp, #4]
	movs r0, #41
	movs r1, #48
	movs r2, #3
	movs r3, #2
	str r5, [sp, #0]
	bl Func_02002dfc
	b .L_0200ad2a
.L_0200a7fa:
	ldr r3, .L_0200aae0
	cmp r2, r3
	beq .L_0200a802
	b .L_0200aae8
.L_0200a802:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #161
	bl Func_02002db4
	cmp r0, #0
	beq .L_0200a81a
	movs r0, #11
	movs r1, #0
	movs r2, #0
	bl Func_02002e6c
.L_0200a81a:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #162
	bl Func_02002db4
	cmp r0, #0
	beq .L_0200a832
	movs r0, #12
	movs r1, #0
	movs r2, #0
	bl Func_02002e6c
.L_0200a832:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #163
	bl Func_02002db4
	cmp r0, #0
	beq .L_0200a84a
	movs r0, #13
	movs r1, #0
	movs r2, #0
	bl Func_02002e6c
.L_0200a84a:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #164
	bl Func_02002db4
	cmp r0, #0
	beq .L_0200a862
	movs r0, #14
	movs r1, #0
	movs r2, #0
	bl Func_02002e6c
.L_0200a862:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #165
	bl Func_02002db4
	cmp r0, #0
	beq .L_0200a87a
	movs r0, #15
	movs r1, #0
	movs r2, #0
	bl Func_02002e6c
.L_0200a87a:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #166
	bl Func_02002db4
	cmp r0, #0
	beq .L_0200a892
	movs r0, #16
	movs r1, #0
	movs r2, #0
	bl Func_02002e6c
.L_0200a892:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #167
	bl Func_02002db4
	cmp r0, #0
	beq .L_0200a8aa
	movs r0, #17
	movs r1, #0
	movs r2, #0
	bl Func_02002e6c
.L_0200a8aa:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #200
	bl Func_02002db4
	cmp r0, #0
	beq .L_0200a8c6
	movs r1, #160
	movs r2, #194
	movs r0, #8
	lsls r1, r1, #14
	lsls r2, r2, #18
	bl Func_02002e6c
.L_0200a8c6:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #201
	bl Func_02002db4
	cmp r0, #0
	beq .L_0200a8e2
	movs r1, #158
	movs r2, #228
	movs r0, #18
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl Func_02002e6c
.L_0200a8e2:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #202
	bl Func_02002db4
	cmp r0, #0
	beq .L_0200a912
	movs r1, #154
	movs r2, #136
	movs r0, #19
	lsls r1, r1, #18
	lsls r2, r2, #16
	bl Func_02002e6c
	movs r3, #37
	movs r2, #71
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #37
	movs r1, #70
	movs r2, #1
	movs r3, #1
	bl Func_02002df4
.L_0200a912:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #217
	bl Func_02002db4
	cmp r0, #0
	beq .L_0200a93e
	movs r0, #9
	bl Object_GetById
	adds r5, r0, #0
	adds r2, r5, #0
	adds r2, #85
	movs r3, #3
	strb r3, [r2]
	movs r1, #200
	movs r2, #182
	movs r0, #9
	lsls r1, r1, #16
	lsls r2, r2, #18
	bl Func_02002e6c
.L_0200a93e:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #218
	bl Func_02002db4
	cmp r0, #0
	beq .L_0200a96c
	movs r0, #10
	bl Object_GetById
	movs r1, #232
	movs r2, #182
	adds r5, r0, #0
	lsls r2, r2, #18
	movs r0, #10
	lsls r1, r1, #16
	bl Func_02002e6c
	adds r2, r5, #0
	adds r2, #85
	movs r3, #3
	strb r3, [r2]
	b .L_0200a994
.L_0200a96c:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #219
	bl Func_02002db4
	cmp r0, #0
	beq .L_0200a994
	movs r0, #10
	adds r0, #255
	bl Func_02002db4
	cmp r0, #0
	bne .L_0200a994
	movs r1, #164
	movs r2, #190
	movs r0, #10
	lsls r1, r1, #17
	lsls r2, r2, #18
	bl Func_02002e6c
.L_0200a994:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #218
	bl Func_02002db4
	cmp r0, #0
	beq .L_0200a9f6
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #217
	bl Func_02002db4
	cmp r0, #0
	beq .L_0200a9f6
	ldr r3, .L_0200aae4
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	ldrh r3, [r3]
	movs r1, #128
	subs r3, #13
	lsls r3, r3, #16
	lsls r1, r1, #11
	cmp r3, r1
	bhi .L_0200a9d0
	movs r0, #160
	lsls r0, r0, #1
	adds r0, #255
	bl Func_02002eec
.L_0200a9d0:
	movs r3, #47
	movs r5, #12
	str r3, [sp, #4]
	movs r0, #16
	movs r1, #27
	movs r2, #3
	movs r3, #13
	str r5, [sp, #0]
	bl Func_02002df4
	movs r3, #111
	str r3, [sp, #4]
	movs r0, #16
	movs r1, #91
	movs r2, #3
	movs r3, #13
	str r5, [sp, #0]
	bl Func_02002df4
.L_0200a9f6:
	movs r0, #10
	adds r0, #255
	bl Func_02002db4
	cmp r0, #0
	bne .L_0200aa16
	ldr r3, .L_0200aae4
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #17
	bne .L_0200aa16
	bl Func_02000418
.L_0200aa16:
	ldr r5, .L_0200aae4
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r5, r2
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #20
	bne .L_0200aac2
	bl Func_02002e44
	movs r0, #0
	bl Func_02002edc
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r0, r0
	negs r1, r1
	negs r2, r2
	movs r3, #0
	bl Func_02002eac
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r5, r2
	ldr r0, [r3]
	movs r1, #0
	movs r2, #0
	bl Func_02002e6c
	movs r1, #0
	movs r2, #0
	movs r0, #10
	bl Func_02002e6c
	bl Event_SetStatus1c6
	movs r0, #15
	bl Battle_WaitMode0
	movs r0, #10
	bl Object_GetById
	movs r1, #164
	movs r2, #190
	adds r5, r0, #0
	lsls r1, r1, #17
	lsls r2, r2, #18
	movs r0, #10
	bl Func_02002e6c
	ldr r3, [r5, #12]
	movs r1, #128
	lsls r1, r1, #16
	adds r3, r3, r1
	adds r2, r5, #0
	str r3, [r5, #12]
	adds r2, #85
	movs r3, #3
	movs r6, #60
	strb r3, [r2]
	b .L_0200aa94
.L_0200aa92:
	subs r6, #1
.L_0200aa94:
	cmp r6, #0
	beq .L_0200aaa8
	movs r0, #1
	bl WaitFrames
	ldr r3, [r5, #40]
	movs r2, #1
	negs r2, r2
	cmp r3, r2
	ble .L_0200aa92
.L_0200aaa8:
	movs r0, #188
	bl Func_02002eec
	movs r0, #60
	bl Battle_WaitMode0
	movs r0, #10
	adds r0, #255
	bl Func_02002dbc
	movs r0, #20
	bl Func_02002ebc
.L_0200aac2:
	ldr r3, .L_0200aae4
	movs r1, #241
	lsls r1, r1, #1
	adds r3, r3, r1
	ldrh r3, [r3]
	movs r2, #192
	subs r3, #13
	lsls r3, r3, #16
	lsls r2, r2, #10
	cmp r3, r2
	bls .L_0200aada
	b .L_0200ad2a
.L_0200aada:
	b .L_0200ab54
.L_0200aadc:
	.4byte 0x000000bf
.L_0200aae0:
	.4byte 0x000000c0
.L_0200aae4:
	.4byte gPartyState
.L_0200aae8:
	ldr r3, .L_0200ad38
	cmp r2, r3
	bne .L_0200ab60
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #219
	bl Func_02002db4
	cmp r0, #0
	beq .L_0200ab06
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_02002e6c
.L_0200ab06:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #203
	bl Func_02002db4
	cmp r0, #0
	beq .L_0200ab36
	movs r1, #140
	movs r2, #156
	movs r0, #8
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_02002e6c
	movs r3, #18
	movs r2, #84
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #16
	movs r1, #84
	movs r2, #1
	movs r3, #1
	bl Func_02002dfc
.L_0200ab36:
	movs r1, #241
	lsls r1, r1, #1
	adds r3, r7, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #1
	beq .L_0200ab4a
	cmp r3, #12
	beq .L_0200ab4a
	b .L_0200ad2a
.L_0200ab4a:
	movs r0, #160
	lsls r0, r0, #1
	adds r0, #255
	bl Func_02002eec
.L_0200ab54:
	movs r1, #144
	ldr r0, .L_0200ad3c
	lsls r1, r1, #3
	bl Func_02002d64
	b .L_0200ad2a
.L_0200ab60:
	ldr r3, .L_0200ad40
	cmp r2, r3
	beq .L_0200ab68
	b .L_0200ad04
.L_0200ab68:
	movs r3, #241
	lsls r3, r3, #1
	adds r3, r3, r7
	mov r10, r3
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #4
	bgt .L_0200ab7c
	bl Func_02001420
.L_0200ab7c:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #226
	bl Func_02002db4
	cmp r0, #0
	beq .L_0200abb0
	movs r3, #65
	movs r5, #14
	str r3, [sp, #4]
	movs r0, #26
	movs r1, #67
	movs r2, #1
	movs r3, #3
	str r5, [sp, #0]
	bl Func_02002df4
	movs r3, #5
	str r3, [sp, #4]
	movs r0, #15
	movs r1, #5
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	bl Func_02002dfc
.L_0200abb0:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #227
	bl Func_02002db4
	cmp r0, #0
	beq .L_0200abe4
	movs r3, #83
	movs r5, #2
	str r3, [sp, #4]
	movs r0, #26
	movs r1, #67
	movs r2, #1
	movs r3, #3
	str r5, [sp, #0]
	bl Func_02002df4
	movs r3, #23
	str r3, [sp, #4]
	movs r0, #1
	movs r1, #23
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	bl Func_02002dfc
.L_0200abe4:
	movs r0, #10
	adds r0, #255
	bl Func_02002db4
	cmp r0, #0
	bne .L_0200abf2
	b .L_0200ad2a
.L_0200abf2:
	movs r0, #129
	lsls r0, r0, #2
	bl Func_02002db4
	cmp r0, #0
	bne .L_0200ac00
	b .L_0200ad2a
.L_0200ac00:
	ldr r3, .L_0200ad44
	movs r2, #128
	lsls r2, r2, #14
	str r2, [r3, #4]
	mov r1, r10
	ldrh r3, [r1]
	mov r8, r2
	subs r3, #1
	movs r2, #128
	lsls r3, r3, #16
	lsls r2, r2, #9
	cmp r3, r2
	bhi .L_0200ac76
	movs r5, #4
	movs r6, #5
	movs r0, #25
	movs r1, #22
	movs r2, #9
	movs r3, #2
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_02002dfc
	movs r3, #69
	str r3, [sp, #4]
	movs r0, #25
	movs r1, #86
	movs r2, #9
	movs r3, #2
	str r5, [sp, #0]
	bl Func_02002dfc
	movs r5, #1
	movs r3, #7
	movs r0, #22
	movs r1, #24
	movs r2, #15
	str r3, [sp, #4]
	str r5, [sp, #0]
	bl Func_02002dfc
	movs r3, #71
	str r3, [sp, #4]
	movs r0, #22
	movs r1, #88
	movs r2, #15
	movs r3, #7
	str r5, [sp, #0]
	bl Func_02002dfc
	movs r3, #68
	str r3, [sp, #0]
	movs r0, #69
	movs r1, #6
	movs r2, #1
	movs r3, #2
	str r6, [sp, #4]
	bl Func_02002e04
.L_0200ac76:
	mov r1, r10
	ldrh r3, [r1]
	movs r2, #128
	subs r3, #3
	lsls r3, r3, #16
	lsls r2, r2, #9
	cmp r3, r2
	bhi .L_0200ace4
	movs r5, #4
	movs r6, #23
	movs r0, #25
	movs r1, #22
	movs r2, #9
	movs r3, #2
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_02002dfc
	movs r3, #87
	str r3, [sp, #4]
	movs r0, #25
	movs r1, #86
	movs r2, #9
	movs r3, #2
	str r5, [sp, #0]
	bl Func_02002dfc
	movs r3, #25
	str r3, [sp, #4]
	movs r5, #1
	movs r0, #22
	movs r1, #24
	movs r2, #15
	movs r3, #7
	str r5, [sp, #0]
	bl Func_02002dfc
	movs r3, #89
	str r3, [sp, #4]
	movs r0, #22
	movs r1, #88
	movs r2, #15
	movs r3, #7
	str r5, [sp, #0]
	bl Func_02002dfc
	movs r3, #68
	str r3, [sp, #0]
	movs r0, #69
	movs r1, #24
	movs r2, #1
	movs r3, #4
	str r6, [sp, #4]
	bl Func_02002e04
.L_0200ace4:
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r7, r1
	ldr r0, [r3]
	bl Object_GetById
	mov r2, r8
	adds r5, r0, #0
	str r2, [r5, #20]
	str r2, [r5, #12]
	movs r0, #1
	bl Battle_WaitMode0
	bl Func_02002dec
	b .L_0200ad2a
.L_0200ad04:
	ldr r3, .L_0200ad48
	cmp r2, r3
	bne .L_0200ad2a
	movs r0, #64
	bl Object_GetById
	adds r5, r0, #0
	cmp r5, #0
	beq .L_0200ad2a
	adds r2, r5, #0
	movs r3, #0
	adds r2, #85
	strb r3, [r2]
	movs r3, #144
	lsls r3, r3, #15
	str r3, [r5, #20]
	str r3, [r5, #12]
	ldr r3, .L_0200ad4c
	str r3, [r5, #108]
.L_0200ad2a:
	movs r0, #0
	add sp, #8
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200ad38:
	.4byte 0x000000c1
.L_0200ad3c:
	.4byte Func_020010f4
.L_0200ad40:
	.4byte 0x000000c2
.L_0200ad44:
	.4byte gOverlayArea + 0x4030
.L_0200ad48:
	.4byte 0x000000c4
.L_0200ad4c:
	.4byte Func_020023d0
	.section .text.x0200ad50,"ax",%progbits
	.global Func_02002d50
	.thumb_func
Func_02002d50:
	movs r0, #0
	bx lr
	.section .rodata.x0200aef4,"a",%progbits
.L_0200aef4:
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
.L_0200af30:
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
.L_0200af6c:
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
	.global Data_02002fa8
Data_02002fa8:
	.4byte 0x01040f00
	.4byte 0x00000008
	.4byte 0x04030300
	.4byte 0x00000005
	.4byte 0x07020600
	.4byte 0x00000006
	.4byte 0x0a010100
	.4byte 0x00000005
	.4byte 0x0c010100
	.4byte 0x00000005
	.4byte 0x0b010102
	.4byte 0x00000005
	.4byte 0x01010f02
	.4byte 0x0000000c
	.4byte 0x01010f04
	.4byte 0x0000000d
	.4byte 0x00000063
	.4byte 0x00000000
	.global Data_02002ff0
Data_02002ff0:
	.4byte 0x01020f00
	.4byte 0x0000001c
	.4byte 0x04020c00
	.4byte 0x0000001a
	.4byte 0x04020800
	.4byte 0x00000018
	.4byte 0x05010200
	.4byte 0x00000017
	.4byte 0x0a010200
	.4byte 0x00000017
	.4byte 0x0f010100
	.4byte 0x00000019
	.4byte 0x04010102
	.4byte 0x00000017
	.4byte 0x01010f02
	.4byte 0x0000001e
	.4byte 0x01010f04
	.4byte 0x0000001f
	.4byte 0x00000063
	.4byte 0x00000000
	.global Data_02003040
Data_02003040:
	.4byte .L_0200aef4
	.4byte .L_0200af30
	.4byte .L_0200af6c
	.global Data_0200304c
Data_0200304c:
	.4byte 0xffff0000
	.4byte 0x00000088
	.4byte 0x80000078
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_0200307c
Data_0200307c:
	.4byte 0x00ee0333
	.4byte 0x033b0354
	.4byte 0x035c00f6
	.4byte 0x000cffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_0200309c
Data_0200309c:
	.4byte 0xffe600b4
	.4byte 0x00bc00a4
	.4byte 0x00acffee
	.4byte 0x0001ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020030bc
Data_020030bc:
	.4byte 0x000000c3
	.4byte 0x00129002
	.4byte 0x002010bd
	.4byte 0x000000bd
	.4byte 0x001020c3
	.4byte 0x0020b0bd
	.4byte 0x003050bd
	.4byte 0x004060bd
	.4byte 0x005030bd
	.4byte 0x006040bd
	.4byte 0x007010be
	.4byte 0x0080c0bd
	.4byte 0x0090a0bd
	.4byte 0x00a090bd
	.4byte 0x00b020bd
	.4byte 0x00c080bd
	.4byte 0x00d140bd
	.4byte 0x00e150bd
	.4byte 0x00f160bd
	.4byte 0x010170c2
	.4byte 0x011180bd
	.4byte 0x012190bd
	.4byte 0x0131a0c1
	.4byte 0x0140d0bd
	.4byte 0x0150e0bd
	.4byte 0x0160f0bd
	.4byte 0x018110bd
	.4byte 0x019120bd
	.4byte 0x01b1e0bd
	.4byte 0x01d1f0bd
	.4byte 0x01e1b0bd
	.4byte 0x01f1d0bd
	.4byte gOamBuckets + 0xae
	.4byte 0x021020be
	.4byte 0x000000be
	.4byte 0x001070bd
	.4byte 0x002210bd
	.4byte 0x003200bd
	.4byte 0x0041c0c2
	.4byte 0x006090bf
	.4byte 0x007040bf
	.4byte 0x00a030bf
	.4byte 0x00b0c0be
	.4byte 0x00c0b0be
	.4byte 0x00e0f0be
	.4byte 0x00f0e0be
	.4byte 0x010110be
	.4byte 0x011100be
	.4byte 0x0120d0c0
	.4byte 0x0130a0c0
	.4byte 0x000000bf
	.4byte 0x001010c0
	.4byte 0x002090c0
	.4byte 0x0030a0be
	.4byte 0x004070be
	.4byte 0x005080bf
	.4byte 0x0060a0bf
	.4byte 0x007010c2
	.4byte 0x008050bf
	.4byte 0x009060be
	.4byte 0x00a060bf
	.4byte 0x00b050c0
	.4byte 0x00c060c0
	.4byte 0x000000c0
	.4byte 0x001010bf
	.4byte 0x002080c1
	.4byte 0x0030e0c0
	.4byte 0x004070c1
	.4byte 0x0050b0bf
	.4byte 0x0060c0bf
	.4byte 0x007040c2
	.4byte 0x0080b0c0
	.4byte 0x009020bf
	.4byte 0x00a130be
	.4byte 0x00b080c0
	.4byte 0x00c030c2
	.4byte 0x00d120be
	.4byte 0x00e030c0
	.4byte 0x00f040c1
	.4byte 0x010030c1
	.4byte 0x014040c1
	.4byte 0x000000c1
	.4byte 0x001020c1
	.4byte 0x002010c1
	.4byte 0x003100c0
	.4byte 0x0040f0c0
	.4byte 0x005060c1
	.4byte 0x006050c1
	.4byte 0x007040c0
	.4byte 0x008020c0
	.4byte 0x009020c2
	.4byte 0x00a0b0c1
	.4byte 0x00b0a0c1
	.4byte 0x00c010c4
	.4byte 0x00d110c0
	.4byte 0x014140c0
	.4byte 0x01a130bd
	.4byte 0x000000c2
	.4byte 0x001070bf
	.4byte 0x002090c1
	.4byte 0x0030c0c0
	.4byte 0x004070c0
	.4byte 0x017100bd
	.4byte 0x01c040be
	.4byte 0x000000c4
	.4byte 0x0010c0c1
	.4byte 0x000001ff
.L_0200b268:
	.4byte 0x00000016
	.4byte 0x0000000b
	.4byte 0x00018000
	.4byte 0x0000002e
	.4byte Func_020005c0
	.4byte 0x00000011
.L_0200b280:
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000000
	.4byte 0x00000011
.L_0200b290:
	.4byte 0x00000016
	.4byte 0x0000001e
	.4byte 0x00000000
	.4byte 0x00000011
	.global Data_020032a0
Data_020032a0:
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020032b8
Data_020032b8:
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x00380000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00024000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00024000
	.4byte 0xffff0188
	.4byte 0x00000001
	.4byte 0x00380000
	.4byte 0x00000000
	.4byte 0x01e80000
	.4byte 0x00024000
	.4byte 0xffff0188
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x01e80000
	.4byte 0x00024000
	.4byte 0xffff0188
	.4byte 0x00000001
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x01e80000
	.4byte 0x00024000
	.4byte 0xffff0188
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0188
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x01e80000
	.4byte 0x00024000
	.4byte 0xffff0188
	.4byte 0x00000001
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x01e80000
	.4byte 0x00024000
	.4byte 0xffff0188
	.4byte 0x00000001
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x01e80000
	.4byte 0x00024000
	.4byte 0xffff0133
	.4byte .L_0200b268
	.4byte 0x01c80000
	.4byte 0x00000000
	.4byte 0x03480000
	.4byte 0x00024000
	.4byte 0xffff0133
	.4byte .L_0200b268
	.4byte 0x02b80000
	.4byte 0x00000000
	.4byte 0x03480000
	.4byte 0x00024000
	.4byte 0xffff0133
	.4byte .L_0200b268
	.4byte 0x02180000
	.4byte 0x00000000
	.4byte 0x02d80000
	.4byte 0x00024000
	.4byte 0xffff0133
	.4byte .L_0200b268
	.4byte 0x02380000
	.4byte 0x00000000
	.4byte 0x02d80000
	.4byte 0x00024000
	.4byte 0xffff0133
	.4byte .L_0200b268
	.4byte 0x02580000
	.4byte 0x00000000
	.4byte 0x02d80000
	.4byte 0x00024000
	.4byte 0xffff0133
	.4byte .L_0200b268
	.4byte 0x02880000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00024000
	.4byte 0xffff0133
	.4byte .L_0200b268
	.4byte 0x02480000
	.4byte 0x00000000
	.4byte 0x00380000
	.4byte 0x00024000
	.4byte 0xffff0133
	.4byte .L_0200b268
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x03980000
	.4byte 0x00024000
	.4byte 0xffff0133
	.4byte .L_0200b268
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x03980000
	.4byte 0x00024000
	.4byte 0xffff0133
	.4byte .L_0200b268
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x02e80000
	.4byte 0x00024000
	.4byte 0xffff0133
	.4byte .L_0200b268
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x02e80000
	.4byte 0x00024000
	.4byte 0xffff0133
	.4byte .L_0200b268
	.4byte 0x03480000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00024000
	.4byte 0xffff0133
	.4byte .L_0200b268
	.4byte 0x03a80000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00024000
	.4byte 0xffff0133
	.4byte .L_0200b268
	.4byte 0x03580000
	.4byte 0x00000000
	.4byte 0x02680000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020034f8
Data_020034f8:
	.4byte 0xffff0187
	.4byte .L_0200b280
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00024000
	.4byte 0xffff0133
	.4byte .L_0200b268
	.4byte 0x01d80000
	.4byte 0x00000000
	.4byte 0x03080000
	.4byte 0x00024000
	.4byte 0xffff0133
	.4byte .L_0200b268
	.4byte 0x02080000
	.4byte 0x00000000
	.4byte 0x03080000
	.4byte 0x00024000
	.4byte 0xffff0133
	.4byte .L_0200b268
	.4byte 0x02580000
	.4byte 0x00000000
	.4byte 0x03080000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02003570
Data_02003570:
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x02180000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00024000
	.4byte 0xffff0186
	.4byte .L_0200b290
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00024000
	.4byte 0xffff0189
	.4byte .L_0200b280
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020035d0
Data_020035d0:
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x00380000
	.4byte 0x00000000
	.4byte 0x03080000
	.4byte 0x00024000
	.4byte 0xffff0187
	.4byte .L_0200b280
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x02f80000
	.4byte 0x00024000
	.4byte 0xffff0189
	.4byte .L_0200b280
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0133
	.4byte .L_0200b268
	.4byte 0x01a80000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00024000
	.4byte 0xffff0133
	.4byte .L_0200b268
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00024000
	.4byte 0xffff0133
	.4byte .L_0200b268
	.4byte 0x02c80000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00024000
	.4byte 0xffff0133
	.4byte .L_0200b268
	.4byte 0x03180000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00024000
	.4byte 0xffff0133
	.4byte .L_0200b268
	.4byte 0x02880000
	.4byte 0x00000000
	.4byte 0x03980000
	.4byte 0x00024000
	.4byte 0xffff0133
	.4byte .L_0200b268
	.4byte 0x02d80000
	.4byte 0x00000000
	.4byte 0x03980000
	.4byte 0x00024000
	.4byte 0xffff0133
	.4byte .L_0200b268
	.4byte 0x03280000
	.4byte 0x00000000
	.4byte 0x03980000
	.4byte 0x00024000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x02880000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x00024000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x02580000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02003708
Data_02003708:
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00024000
	.4byte 0xffff0189
	.4byte .L_0200b280
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x02480000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02003750
Data_02003750:
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02003768
Data_02003768:
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02003780
Data_02003780:
	.2byte 0x000a
	.global Data_02003782
Data_02003782:
	.2byte 0x0000
	.global Data_02003784
Data_02003784:
	.4byte 0x00000000
	.global Data_02003788
Data_02003788:
	.4byte 0x00008000
	.global Data_0200378c
Data_0200378c:
	.4byte 0x00000000
	.global Data_02003790
Data_02003790:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020037b4
Data_020037b4:
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
	.4byte 0x00000021
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
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00000001
	.4byte 0xffff000c
	.4byte 0x0000000c
	.4byte 0x00000001
	.4byte 0xffff000d
	.4byte 0x0000000d
	.4byte 0x00000001
	.4byte 0xffff000e
	.4byte 0x0000000e
	.4byte 0x00000001
	.4byte 0xffff000f
	.4byte 0x0000000f
	.4byte 0x00000001
	.4byte 0xffff0010
	.4byte 0x00000010
	.4byte 0x00000001
	.4byte 0xffff0011
	.4byte 0x00000011
	.4byte 0x00000001
	.4byte 0xffff0012
	.4byte 0x00000012
	.4byte 0x00000001
	.4byte 0xffff0013
	.4byte 0x00000013
	.4byte 0x00000001
	.4byte 0xffff0014
	.4byte 0x00000014
	.4byte 0x00000001
	.4byte 0xffff0015
	.4byte 0x00000015
	.4byte 0x00000001
	.4byte 0xffff0016
	.4byte 0x00000016
	.4byte 0x00000001
	.4byte 0xffff0017
	.4byte 0x00000017
	.4byte 0x00000001
	.4byte 0xffff0018
	.4byte 0x00000018
	.4byte 0x00000001
	.4byte 0xffff0019
	.4byte 0x00000019
	.4byte 0x00000001
	.4byte 0xffff001a
	.4byte 0x0000001a
	.4byte 0x00000001
	.4byte 0xffff001b
	.4byte 0x0000001b
	.4byte 0x00000021
	.4byte 0xffff001c
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0xffff001d
	.4byte 0x0000001d
	.4byte 0x00000001
	.4byte 0xffff001e
	.4byte 0x0000001e
	.4byte 0x00000001
	.4byte 0xffff001f
	.4byte 0x0000001f
	.4byte 0x00000021
	.4byte 0xffff0020
	.4byte 0x00000020
	.4byte 0x00000021
	.4byte 0xffff0021
	.4byte 0x00000021
	.4byte 0x00008c15
	.4byte 0x09c60008
	.4byte Func_02000850
	.4byte 0x10008c15
	.4byte 0x09c60008
	.4byte Func_020006d0
	.4byte 0x00008c15
	.4byte 0x09d2000a
	.4byte Func_02000a68
	.4byte 0x00008c15
	.4byte 0x09d3000b
	.4byte Func_02000a68
	.4byte 0x00008c15
	.4byte 0x09d4000c
	.4byte Func_02000a68
	.4byte 0x00008c15
	.4byte 0x09d5000d
	.4byte Func_02000a68
	.4byte 0x00008c15
	.4byte 0x09d6000e
	.4byte Func_02000a68
	.4byte 0x00008c15
	.4byte 0x09d7000f
	.4byte Func_02000a68
	.4byte 0x00008c15
	.4byte 0x09d80010
	.4byte Func_02000a68
	.4byte 0x00004e15
	.4byte 0xffff0011
	.4byte Func_02001264
	.4byte 0x00004e15
	.4byte 0xffff0012
	.4byte Func_02001264
	.4byte 0x00004e15
	.4byte 0xffff0013
	.4byte Func_02001264
	.4byte 0x00004e15
	.4byte 0xffff0014
	.4byte Func_02001264
	.4byte 0x00004e15
	.4byte 0xffff0015
	.4byte Func_02001264
	.4byte 0x00004e15
	.4byte 0xffff0016
	.4byte Func_02001264
	.4byte 0x00004e15
	.4byte 0xffff0017
	.4byte Func_02001264
	.4byte 0x00004e15
	.4byte 0xffff0018
	.4byte Func_02001264
	.4byte 0x00004e15
	.4byte 0xffff0019
	.4byte Func_02001264
	.4byte 0x00004e15
	.4byte 0xffff001a
	.4byte Func_02001264
	.4byte 0x00004e15
	.4byte 0xffff001b
	.4byte Func_02001264
	.4byte 0x00004e15
	.4byte 0xffff001c
	.4byte Func_02001264
	.4byte 0x00004e15
	.4byte 0xffff001d
	.4byte Func_02001264
	.4byte 0x00004e15
	.4byte 0xffff001e
	.4byte Func_02001264
	.4byte 0x00008b85
	.4byte 0xffff0000
	.4byte Func_02000698
	.4byte 0x40008b85
	.4byte 0xffff0000
	.4byte Func_020006b4
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02003a78
Data_02003a78:
	.4byte 0x00000031
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000031
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000031
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000031
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000021
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000001
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00000001
	.4byte 0xffff000c
	.4byte 0x0000000c
	.4byte 0x00000021
	.4byte 0xffff000d
	.4byte 0x0000000d
	.4byte 0x00000001
	.4byte 0xffff000e
	.4byte 0x0000000e
	.4byte 0x00000001
	.4byte 0xffff000f
	.4byte 0x0000000f
	.4byte 0x00000001
	.4byte 0xffff0010
	.4byte 0x00000010
	.4byte 0x00000001
	.4byte 0xffff0011
	.4byte 0x00000011
	.4byte 0x00000001
	.4byte 0xffff0012
	.4byte 0x00000012
	.4byte 0x00000021
	.4byte 0xffff0013
	.4byte 0x00000013
	.4byte 0x00008c15
	.4byte 0x09dc0008
	.4byte Func_02000e60
	.4byte 0x00000009
	.4byte 0x09dc0008
	.4byte Func_02000e60
	.4byte 0x00004e15
	.4byte 0xffff0009
	.4byte Func_02001264
	.4byte 0x00004e15
	.4byte 0xffff000a
	.4byte Func_02001264
	.4byte 0x00004e15
	.4byte 0xffff000b
	.4byte Func_02001264
	.4byte 0x00008b85
	.4byte 0xffff0000
	.4byte Func_02000698
	.4byte 0x40008b85
	.4byte 0xffff0000
	.4byte Func_020006b4
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02003b98
Data_02003b98:
	.4byte 0x00000021
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000021
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
	.4byte 0x00000031
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000001
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00000001
	.4byte 0xffff000c
	.4byte 0x0000000c
	.4byte 0x00000002
	.4byte 0xffff0028
	.4byte Func_020020fc
	.4byte 0x00000002
	.4byte 0xffff0029
	.4byte Func_020021d4
	.4byte 0x10008c15
	.4byte 0x09c70008
	.4byte Func_020006d0
	.4byte 0x00008c15
	.4byte 0x09c70008
	.4byte Func_02000850
	.4byte 0x00008c15
	.4byte 0x09dd000a
	.4byte Func_0200104c
	.4byte 0x00000009
	.4byte 0x09dd000a
	.4byte Func_0200104c
	.4byte 0x00008b85
	.4byte 0xffff0000
	.4byte Func_02000698
	.4byte 0x40008b85
	.4byte 0xffff0000
	.4byte Func_020006b4
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02003c94
Data_02003c94:
	.4byte 0x00000031
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
	.4byte 0x00000031
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000031
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00000001
	.4byte 0xffff000c
	.4byte 0x0000000c
	.4byte 0x00000001
	.4byte 0xffff000d
	.4byte 0x0000000d
	.4byte 0x00000001
	.4byte 0xffff000e
	.4byte 0x0000000e
	.4byte 0x00000021
	.4byte 0xffff000f
	.4byte 0x0000000f
	.4byte 0x00000021
	.4byte 0xffff0010
	.4byte 0x00000010
	.4byte 0x00004e15
	.4byte 0xffff000b
	.4byte Func_02001264
	.4byte 0x00004e15
	.4byte 0xffff000c
	.4byte Func_02001264
	.4byte 0x00004e15
	.4byte 0xffff000d
	.4byte Func_02001264
	.4byte 0x00004e15
	.4byte 0xffff000e
	.4byte Func_02001264
	.4byte 0x00004e15
	.4byte 0xffff000f
	.4byte Func_02001264
	.4byte 0x00004e15
	.4byte 0xffff0010
	.4byte Func_02001264
	.4byte 0x00004e15
	.4byte 0xffff0011
	.4byte Func_02001264
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte Func_0200124c
	.4byte 0x00008c15
	.4byte 0xffff0009
	.4byte Func_02000c24
	.4byte 0x00008c15
	.4byte 0xffff000a
	.4byte Func_02000c24
	.4byte 0x10008c15
	.4byte 0x09c80008
	.4byte Func_020006d0
	.4byte 0x00008c15
	.4byte 0x09c80008
	.4byte Func_02000850
	.4byte 0x10008c15
	.4byte 0x09c90012
	.4byte Func_020006d0
	.4byte 0x00008c15
	.4byte 0x09c90012
	.4byte Func_02000850
	.4byte 0x10008c15
	.4byte 0x09ca0013
	.4byte Func_020006d0
	.4byte 0x00008c15
	.4byte 0x09ca0013
	.4byte Func_02000850
	.4byte 0x00008b85
	.4byte 0xffff0000
	.4byte Func_02000698
	.4byte 0x40008b85
	.4byte 0xffff0000
	.4byte Func_020006b4
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02003e38
Data_02003e38:
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
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000031
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000031
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000031
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000001
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00004602
	.4byte 0xffff000d
	.4byte Func_02000384
	.4byte 0x00000001
	.4byte 0xffff001a
	.4byte 0x0000001a
	.4byte 0x00000002
	.4byte 0xffff0042
	.4byte Func_020011c0
	.4byte 0x10008c15
	.4byte 0x09cb0008
	.4byte Func_020006d0
	.4byte 0x00008c15
	.4byte 0x09cb0008
	.4byte Func_02000850
	.4byte 0x10008c15
	.4byte 0x09db0009
	.4byte Func_020006d0
	.4byte 0x00008c15
	.4byte 0x09db0009
	.4byte Func_02002260
	.4byte 0x50009085
	.4byte 0xffff0000
	.4byte Func_020022a8
	.4byte 0x40009085
	.4byte 0xffff0000
	.4byte Func_020022e0
	.4byte 0x00008b85
	.4byte 0xffff0000
	.4byte Func_02000698
	.4byte 0x40008b85
	.4byte 0xffff0000
	.4byte Func_020006b4
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02003f4c
Data_02003f4c:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000021
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0017
	.4byte 0x00000017
	.4byte 0x00000021
	.4byte 0xffff001c
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte Data_02020004 + 0x24
	.4byte Func_02001d34
	.4byte 0x00000002
	.4byte 0xffff0029
	.4byte Func_02001ddc
	.4byte 0x00000002
	.4byte Data_02030000 + 0x2a
	.4byte Func_02001e54
	.4byte 0x00000002
	.4byte 0xffff002b
	.4byte Func_02001f04
	.4byte 0x00000006
	.4byte 0xffff0044
	.4byte Func_0200160c
	.4byte 0x00000006
	.4byte 0xffff0045
	.4byte Func_020019a0
	.4byte 0x00008b85
	.4byte 0xffff0000
	.4byte Func_02000698
	.4byte 0x40008b85
	.4byte 0xffff0000
	.4byte Func_020006b4
	.4byte 0x50009085
	.4byte 0xffff0000
	.4byte Func_02002318
	.4byte 0x40009085
	.4byte 0xffff0000
	.4byte Func_02002328
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02004018
Data_02004018:
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
