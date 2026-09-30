.syntax unified
	.thumb
	.section .text.x02008038,"ax",%progbits
	.global Func_02000038
	.thumb_func
Func_02000038:
	push {r5, r6, r7, lr}
	ldr r2, .L_02008098
	movs r1, #128
	movs r3, #192
	lsls r1, r1, #2
	lsls r3, r3, #18
	adds r1, #18
	ldr r6, [r3, #108]
	adds r3, r2, r1
	ldrb r3, [r3]
	cmp r3, #1
	bne .L_02008094
	subs r1, #122
	movs r3, #150
	lsls r3, r3, #2
	adds r5, r6, r1
	adds r7, r2, r3
	ldr r3, [r5]
	movs r1, #10
	lsls r0, r3, #3
	adds r0, r0, r3
	bl Engine_MathDivide
	ldr r3, [r7]
	cmp r3, r0
	blt .L_02008094
	bl Random16Far
	movs r3, #128
	lsls r3, r3, #8
	cmp r0, r3
	bcs .L_02008090
	movs r0, #128
	lsls r0, r0, #4
	movs r1, #8
	adds r0, #10
	bl Func_02004d50
	movs r1, #202
	lsls r1, r1, #1
	adds r2, r6, r1
	movs r3, #0
	str r3, [r2]
	b .L_02008094
.L_02008090:
	ldr r3, [r5]
	str r3, [r7]
.L_02008094:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008098:
	.4byte gPartyState
	.section .text.x0200809c,"ax",%progbits
	.global Func_0200009c
	.thumb_func
Func_0200009c:
	push {r5, r6, r7, lr}
	ldr r2, .L_020080fc
	movs r1, #128
	movs r3, #192
	lsls r1, r1, #2
	lsls r3, r3, #18
	adds r1, #18
	ldr r6, [r3, #108]
	adds r3, r2, r1
	ldrb r3, [r3]
	cmp r3, #1
	bne .L_020080f8
	subs r1, #122
	movs r3, #150
	lsls r3, r3, #2
	adds r5, r6, r1
	adds r7, r2, r3
	ldr r3, [r5]
	movs r1, #10
	lsls r0, r3, #3
	adds r0, r0, r3
	bl Engine_MathDivide
	ldr r3, [r7]
	cmp r3, r0
	blt .L_020080f8
	bl Random16Far
	movs r3, #128
	lsls r3, r3, #8
	cmp r0, r3
	bcs .L_020080f4
	movs r0, #128
	lsls r0, r0, #4
	movs r1, #28
	adds r0, #11
	bl Func_02004d50
	movs r1, #202
	lsls r1, r1, #1
	adds r2, r6, r1
	movs r3, #0
	str r3, [r2]
	b .L_020080f8
.L_020080f4:
	ldr r3, [r5]
	str r3, [r7]
.L_020080f8:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_020080fc:
	.4byte gPartyState
	.section .text.x02008100,"ax",%progbits
	.global Func_02000100
	.thumb_func
Func_02000100:
	push {r5, r6, r7, lr}
	ldr r2, .L_02008160
	movs r1, #128
	movs r3, #192
	lsls r1, r1, #2
	lsls r3, r3, #18
	adds r1, #18
	ldr r6, [r3, #108]
	adds r3, r2, r1
	ldrb r3, [r3]
	cmp r3, #1
	bne .L_0200815c
	subs r1, #122
	movs r3, #150
	lsls r3, r3, #2
	adds r5, r6, r1
	adds r7, r2, r3
	ldr r3, [r5]
	movs r1, #10
	lsls r0, r3, #3
	adds r0, r0, r3
	bl Engine_MathDivide
	ldr r3, [r7]
	cmp r3, r0
	blt .L_0200815c
	bl Random16Far
	movs r3, #128
	lsls r3, r3, #8
	cmp r0, r3
	bcs .L_02008158
	movs r0, #128
	lsls r0, r0, #4
	movs r1, #31
	adds r0, #12
	bl Func_02004d50
	movs r1, #202
	lsls r1, r1, #1
	adds r2, r6, r1
	movs r3, #0
	str r3, [r2]
	b .L_0200815c
.L_02008158:
	ldr r3, [r5]
	str r3, [r7]
.L_0200815c:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008160:
	.4byte gPartyState
	.section .text.x02008164,"ax",%progbits
	.global Func_02000164
	.thumb_func
Func_02000164:
	push {r5, r6, r7, lr}
	ldr r2, .L_020081c4
	movs r1, #128
	movs r3, #192
	lsls r1, r1, #2
	lsls r3, r3, #18
	adds r1, #18
	ldr r6, [r3, #108]
	adds r3, r2, r1
	ldrb r3, [r3]
	cmp r3, #1
	bne .L_020081c0
	subs r1, #122
	movs r3, #150
	lsls r3, r3, #2
	adds r5, r6, r1
	adds r7, r2, r3
	ldr r3, [r5]
	movs r1, #10
	lsls r0, r3, #3
	adds r0, r0, r3
	bl Engine_MathDivide
	ldr r3, [r7]
	cmp r3, r0
	blt .L_020081c0
	bl Random16Far
	movs r3, #128
	lsls r3, r3, #8
	cmp r0, r3
	bcs .L_020081bc
	movs r0, #128
	lsls r0, r0, #4
	movs r1, #72
	adds r0, #13
	bl Func_02004d50
	movs r1, #202
	lsls r1, r1, #1
	adds r2, r6, r1
	movs r3, #0
	str r3, [r2]
	b .L_020081c0
.L_020081bc:
	ldr r3, [r5]
	str r3, [r7]
.L_020081c0:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_020081c4:
	.4byte gPartyState
	.section .text.x020081c8,"ax",%progbits
	.global Func_020001c8
	.thumb_func
Func_020001c8:
	push {r5, r6, r7, lr}
	ldr r2, .L_02008228
	movs r1, #128
	movs r3, #192
	lsls r1, r1, #2
	lsls r3, r3, #18
	adds r1, #18
	ldr r6, [r3, #108]
	adds r3, r2, r1
	ldrb r3, [r3]
	cmp r3, #1
	bne .L_02008224
	subs r1, #122
	movs r3, #150
	lsls r3, r3, #2
	adds r5, r6, r1
	adds r7, r2, r3
	ldr r3, [r5]
	movs r1, #10
	lsls r0, r3, #3
	adds r0, r0, r3
	bl Engine_MathDivide
	ldr r3, [r7]
	cmp r3, r0
	blt .L_02008224
	bl Random16Far
	movs r3, #128
	lsls r3, r3, #8
	cmp r0, r3
	bcs .L_02008220
	movs r0, #128
	lsls r0, r0, #4
	movs r1, #13
	adds r0, #14
	bl Func_02004d50
	movs r1, #202
	lsls r1, r1, #1
	adds r2, r6, r1
	movs r3, #0
	str r3, [r2]
	b .L_02008224
.L_02008220:
	ldr r3, [r5]
	str r3, [r7]
.L_02008224:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008228:
	.4byte gPartyState
	.section .text.x0200822c,"ax",%progbits
	.global Func_0200022c
	.thumb_func
Func_0200022c:
	push {r5, r6, r7, lr}
	ldr r2, .L_0200828c
	movs r1, #128
	movs r3, #192
	lsls r1, r1, #2
	lsls r3, r3, #18
	adds r1, #18
	ldr r6, [r3, #108]
	adds r3, r2, r1
	ldrb r3, [r3]
	cmp r3, #1
	bne .L_02008288
	subs r1, #122
	movs r3, #150
	lsls r3, r3, #2
	adds r5, r6, r1
	adds r7, r2, r3
	ldr r3, [r5]
	movs r1, #10
	lsls r0, r3, #3
	adds r0, r0, r3
	bl Engine_MathDivide
	ldr r3, [r7]
	cmp r3, r0
	blt .L_02008288
	bl Random16Far
	movs r3, #128
	lsls r3, r3, #8
	cmp r0, r3
	bcs .L_02008284
	movs r0, #226
	lsls r0, r0, #3
	movs r1, #53
	adds r0, #255
	bl Func_02004d50
	movs r1, #202
	lsls r1, r1, #1
	adds r2, r6, r1
	movs r3, #0
	str r3, [r2]
	b .L_02008288
.L_02008284:
	ldr r3, [r5]
	str r3, [r7]
.L_02008288:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200828c:
	.4byte gPartyState
	.section .text.x02008290,"ax",%progbits
	.global Func_02000290
	.thumb_func
Func_02000290:
	ldr r0, .L_02008294
	bx lr
.L_02008294:
	.4byte Data_02005990
	.section .text.x02008298,"ax",%progbits
	.global Func_02000298
	.thumb_func
Func_02000298:
	movs r0, #0
	bx lr
	.section .text.x0200829c,"ax",%progbits
	.global Func_0200029c
	.thumb_func
Func_0200029c:
	ldr r0, .L_020082a0
	bx lr
.L_020082a0:
	.4byte Data_020061d0
	.section .text.x020082a4,"ax",%progbits
	.global Func_020002a4
	.thumb_func
Func_020002a4:
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	movs r2, #143
	lsls r2, r2, #1
	adds r3, r3, r2
	ldr r1, [r0, #80]
	ldrh r3, [r3]
	ldr r2, .L_020082c0
	strh r3, [r1, #18]
	strb r2, [r1, #26]
	movs r0, #1
	bx lr
	.2byte 0x0000
.L_020082c0:
	.4byte 0x00000000
	.section .text.x020082c4,"ax",%progbits
	.global Func_020002c4
	.thumb_func
Func_020002c4:
	push {r5, r6, lr}
	ldr r3, .L_0200833c
	movs r2, #2
	ldr r3, [r3]
	adds r6, r0, #0
	ands r3, r2
	cmp r3, #0
	beq .L_020082dc
	movs r1, #10
	bl Animation_ApplyChildValues
	b .L_020082e4
.L_020082dc:
	adds r0, r6, #0
	movs r1, #7
	bl Animation_ApplyChildValues
.L_020082e4:
	adds r3, r6, #0
	adds r3, #102
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	bne .L_0200833a
	ldr r3, .L_02008340
	adds r5, r6, #0
	str r3, [r6, #8]
	adds r5, #100
	movs r3, #0
	ldrsh r0, [r5, r3]
	lsls r0, r0, #3
	bl Math_Sine
	movs r1, #128
	ldr r3, .L_02008344
	lsls r1, r1, #11
	mov lr, r3
	.2byte 0xf800
	ldr r3, .L_02008348
	movs r4, #128
	lsls r4, r4, #13
	adds r0, r0, r4
	str r0, [r6, #12]
	str r3, [r6, #16]
	adds r0, r4, #0
	movs r2, #0
	ldrsh r1, [r5, r2]
	adds r2, r6, #0
	adds r2, #8
	bl Vector_AddPolarOffsetFar
	ldrh r3, [r5]
	movs r2, #128
	lsls r2, r2, #7
	adds r3, r3, r2
	strh r3, [r6, #6]
	movs r2, #128
	ldrh r3, [r5]
	lsls r2, r2, #3
	adds r3, r3, r2
	strh r3, [r5]
.L_0200833a:
	pop {r5, r6, pc}
.L_0200833c:
	.4byte Data_0300122c
.L_02008340:
	.4byte 0x27880000
.L_02008344:
	.4byte IwramMulQ16
.L_02008348:
	.4byte 0x20a40000
	.section .text.x0200834c,"ax",%progbits
	.global Func_0200034c
	.thumb_func
Func_0200034c:
	push {r5, lr}
	adds r5, r0, #0
	movs r0, #55
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008380
	movs r0, #183
	lsls r0, r0, #1
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008380
	ldr r3, .L_02008384
	str r3, [r5, #108]
	adds r3, r5, #0
	adds r3, #85
	strb r0, [r3]
	adds r3, #15
	strh r0, [r3]
	adds r3, #2
	strh r0, [r3]
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r5, #24]
	str r3, [r5, #28]
.L_02008380:
	movs r0, #0
	pop {r5, pc}
.L_02008384:
	.4byte Func_020002c4
	.section .text.x02008388,"ax",%progbits
	.global Func_02000388
	.thumb_func
Func_02000388:
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	movs r2, #143
	lsls r2, r2, #1
	adds r3, r3, r2
	ldr r1, [r0, #80]
	ldrh r3, [r3]
	ldr r2, .L_020083a4
	strh r3, [r1, #18]
	strb r2, [r1, #26]
	movs r0, #1
	bx lr
	.2byte 0x0000
.L_020083a4:
	.4byte 0x00000000
	.section .text.x020083a8,"ax",%progbits
	.global Func_020003a8
	.thumb_func
Func_020003a8:
	push {lr}
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #0
	pop {pc}
	.section .text.x020083b4,"ax",%progbits
	.global Func_020003b4
	.thumb_func
Func_020003b4:
	push {r5, lr}
	adds r5, r0, #0
	movs r0, #128
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020083c8
	movs r0, #1
	b .L_020083f0
.L_020083c8:
	adds r0, r5, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	ldr r1, [r5, #8]
	ldr r0, .L_020083f4
	movs r3, #128
	lsls r3, r3, #5
	str r3, [r5, #52]
	str r3, [r5, #48]
	ldr r3, [r5, #16]
	adds r1, r1, r0
	movs r0, #128
	lsls r0, r0, #12
	adds r3, r3, r0
	ldr r2, [r5, #12]
	adds r0, r5, #0
	bl Func_02004b50
	movs r0, #0
.L_020083f0:
	pop {r5, pc}
	.2byte 0x0000
.L_020083f4:
	.4byte 0xffe20000
	.section .text.x020083f8,"ax",%progbits
	.global Func_020003f8
	.thumb_func
Func_020003f8:
	push {lr}
	ldr r3, .L_02008440
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #68
	beq .L_02008426
	cmp r3, #68
	bgt .L_02008418
	cmp r3, #64
	beq .L_0200841e
	cmp r3, #65
	beq .L_02008422
	b .L_0200843c
.L_02008418:
	cmp r3, #77
	beq .L_02008438
	b .L_0200843c
.L_0200841e:
	ldr r0, .L_02008444
	b .L_0200843e
.L_02008422:
	ldr r0, .L_02008448
	b .L_0200843e
.L_02008426:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #222
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008438
	ldr r0, .L_0200844c
	b .L_0200843e
.L_02008438:
	ldr r0, .L_02008450
	b .L_0200843e
.L_0200843c:
	ldr r0, .L_02008454
.L_0200843e:
	pop {pc}
.L_02008440:
	.4byte gPartyState
.L_02008444:
	.4byte Data_02006500
.L_02008448:
	.4byte Data_02006598
.L_0200844c:
	.4byte Data_02006700
.L_02008450:
	.4byte Data_02006790
.L_02008454:
	.4byte Data_02006338
	.section .text.x02008458,"ax",%progbits
	.global Func_02000458
	.thumb_func
Func_02000458:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #108]
	movs r1, #128
	ldr r3, .L_02008504
	lsls r1, r1, #2
	adds r1, #18
	adds r3, r3, r1
	movs r2, #8
	movs r0, #72
	strb r2, [r3]
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200848e
	bl Func_02004da8
	movs r0, #153
	movs r1, #4
	bl Func_02004ba8
	ldr r0, .L_02008508
	movs r1, #4
	bl UiText_ShowPositionedMessageAndWait
.L_0200848e:
	bl Func_02004e10
	bl Object_GetById
	cmp r0, #0
	beq .L_0200849e
	bl Func_02004b28
.L_0200849e:
	bl Func_02003bf0
	movs r0, #72
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020084ba
	movs r3, #208
	lsls r3, r3, #4
	adds r3, #59
	adds r2, r5, r3
	movs r3, #0
	b .L_020084c4
.L_020084ba:
	movs r1, #208
	lsls r1, r1, #4
	adds r1, #59
	adds r2, r5, r1
	movs r3, #1
.L_020084c4:
	strb r3, [r2]
	movs r0, #72
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020084da
	bl UiWork_FinalizePendingCore
	bl Func_02004db0
.L_020084da:
	bl Func_020049bc
	movs r3, #173
	lsls r3, r3, #1
	movs r1, #175
	adds r2, r5, r3
	lsls r1, r1, #1
	movs r3, #0
	strh r3, [r2]
	adds r2, r5, r1
	subs r1, #2
	strh r3, [r2]
	adds r2, r5, r1
	adds r1, #8
	strh r3, [r2]
	adds r2, r5, r1
	adds r1, #2
	strh r3, [r2]
	adds r2, r5, r1
	strh r3, [r2]
	pop {r5, pc}
.L_02008504:
	.4byte gPartyState
.L_02008508:
	.4byte 0x00000dc2
	.section .text.x0200850c,"ax",%progbits
	.global Func_0200050c
	.thumb_func
Func_0200050c:
	push {r5, r6, r7, lr}
	ldr r3, .L_02008570
	movs r1, #128
	lsls r1, r1, #2
	adds r1, #18
	adds r3, r3, r1
	movs r2, #7
	strb r2, [r3]
	bl Func_02003c3c
	bl Func_02004a1c
	movs r0, #8
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	bl Party_CountActiveOwners
	movs r6, #0
	adds r7, r0, #0
	movs r5, #0
	cmp r6, r7
	bge .L_02008558
.L_0200853e:
	ldr r2, .L_02008570
	movs r1, #134
	lsls r1, r1, #2
	adds r3, r5, r1
	ldrb r0, [r2, r3]
	bl Owner_GetState
	movs r2, #58
	ldrsh r3, [r0, r2]
	adds r5, #1
	adds r6, r6, r3
	cmp r5, r7
	blt .L_0200853e
.L_02008558:
	cmp r6, #0
	bne .L_0200856c
	ldr r3, .L_02008570
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	ldr r0, [r3]
	movs r1, #1
	bl Owner_AdjustSecondValue
.L_0200856c:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008570:
	.4byte gPartyState
	.section .text.x02008574,"ax",%progbits
	.global Func_02000574
	.thumb_func
Func_02000574:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #222
	mov r9, r3
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008596
	b .L_020086c2
.L_02008596:
	ldr r1, .L_020086cc
	movs r3, #133
	mov r8, r1
	lsls r3, r3, #2
	add r3, r8
	ldr r0, [r3]
	mov r10, r3
	bl Func_02004d68
	mov r1, r10
	adds r5, r0, #0
	ldr r0, [r1]
	bl Object_GetById
	adds r6, r0, #0
	movs r0, #8
	bl Object_GetById
	adds r7, r0, #0
	ldr r3, [r7, #8]
	ldr r1, [r6, #8]
	ldr r2, [r7, #16]
	subs r1, r1, r3
	ldr r3, [r6, #16]
	asrs r1, r1, #16
	subs r3, r3, r2
	asrs r3, r3, #16
	adds r2, r1, #0
	muls r2, r1
	adds r1, r3, #0
	muls r1, r3
	adds r3, r1, #0
	adds r2, r2, r3
	cmp r5, #8
	beq .L_020085e0
	cmp r2, #255
	bgt .L_020086c2
.L_020085e0:
	bl Func_02004c20
	movs r0, #0
	bl Func_02004db8
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #18
	add r2, r8
	movs r3, #7
	strb r3, [r2]
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #118
	add r2, r8
	movs r3, #1
	movs r0, #187
	strh r3, [r2]
	lsls r0, r0, #1
	bl GameFlag_SetBit
	ldr r3, [r7, #8]
	movs r2, #158
	lsls r2, r2, #2
	add r2, r8
	str r3, [r2]
	ldr r3, [r7, #16]
	movs r2, #159
	lsls r2, r2, #2
	add r2, r8
	str r3, [r2]
	movs r2, #160
	ldrh r3, [r7, #6]
	lsls r2, r2, #2
	add r2, r8
	str r3, [r2]
	movs r0, #78
	bl Func_02004e30
	movs r1, #6
	adds r0, r6, #0
	bl Func_02004b18
	movs r0, #6
	bl WaitFrames
	movs r0, #152
	bl Func_02004e30
	adds r0, r6, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r1, #7
	adds r0, r6, #0
	bl Func_02004b18
	movs r3, #192
	lsls r3, r3, #10
	str r3, [r6, #48]
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r6, #52]
	movs r3, #128
	lsls r3, r3, #11
	str r3, [r6, #40]
	str r3, [r6, #20]
	adds r0, r6, #0
	bl Func_02004b48
	ldr r3, [r7, #16]
	movs r2, #0
	ldr r1, [r7, #8]
	adds r0, r6, #0
	bl Func_02004b50
	adds r0, r6, #0
	bl Func_02004b58
	movs r1, #6
	adds r0, r6, #0
	bl Func_02004b18
	movs r5, #0
	movs r0, #6
	bl WaitFrames
	str r5, [r6, #20]
	mov r3, r10
	ldr r0, [r3]
	movs r1, #0
	movs r2, #0
	bl Func_02004c88
	bl Func_02004dc8
	bl Func_02004dc0
	bl Func_02004c28
	movs r3, #174
	lsls r3, r3, #1
	add r3, r9
	strh r5, [r3]
	movs r3, #170
	lsls r3, r3, #1
	add r3, r9
	movs r0, #128
	strh r5, [r3]
	lsls r0, r0, #9
	movs r1, #30
	bl Func_02004df8
.L_020086c2:
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_020086cc:
	.4byte gPartyState
	.section .text.x020086d0,"ax",%progbits
	.global Func_020006d0
	.thumb_func
Func_020006d0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, .L_02008784
	movs r0, #133
	lsls r0, r0, #2
	adds r3, r3, r0
	ldr r0, [r3]
	sub sp, #16
	bl Object_GetById
	mov r10, r0
	movs r0, #8
	bl Object_GetById
	movs r3, #192
	adds r6, r0, #0
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	adds r5, r6, #0
	adds r5, #8
	adds r0, r5, #0
	str r3, [sp, #0]
	bl GetWorldMapCollision
	ldr r3, [r5]
	add r7, sp, #4
	str r3, [r7]
	movs r1, #0
	ldr r3, [r6, #12]
	mov r8, r0
	str r3, [r7, #4]
	movs r0, #192
	ldr r3, [r6, #16]
	adds r2, r7, #0
	str r3, [r7, #8]
	mov r11, r1
	lsls r0, r0, #13
	ldrh r1, [r6, #6]
	bl Vector_AddPolarOffsetFar
	ldr r3, [r7]
	movs r2, #0
	mov r9, r2
	cmp r3, #0
	bge .L_02008738
	ldr r0, .L_02008788
	adds r3, r3, r0
.L_02008738:
	asrs r2, r3, #21
	ldr r3, [r7, #8]
	movs r1, #31
	ands r2, r1
	cmp r3, #0
	bge .L_02008748
	ldr r0, .L_02008788
	adds r3, r3, r0
.L_02008748:
	asrs r3, r3, #21
	ands r3, r1
	lsls r3, r3, #5
	ldr r1, .L_0200878c
	adds r3, r2, r3
	lsls r3, r3, #2
	adds r2, r3, r1
	mov r3, r8
	cmp r3, #12
	ble .L_02008762
	movs r0, #1
	mov r9, r0
	b .L_02008802
.L_02008762:
	mov r3, r8
	subs r3, #1
	cmp r3, #2
	bhi .L_0200876e
	movs r1, #1
	mov r11, r1
.L_0200876e:
	ldrb r2, [r2, #3]
	movs r3, #64
	ands r3, r2
	cmp r3, #0
	bne .L_02008790
	mov r2, r11
	cmp r2, #0
	beq .L_02008790
	movs r3, #1
	b .L_02008800
	.2byte 0x0000
.L_02008784:
	.4byte gPartyState
.L_02008788:
	.4byte 0x001fffff
.L_0200878c:
	.4byte gMapBlocks
.L_02008790:
	ldr r3, [r7]
	movs r5, #192
	lsls r5, r5, #11
	adds r3, r3, r5
	str r3, [r7]
	adds r1, r7, #0
	adds r0, r6, #0
	bl Func_02004b88
	ldr r2, .L_02008874
	ldr r3, [r7]
	mov r8, r2
	mov r1, r9
	orrs r1, r0
	add r3, r8
	mov r9, r1
	str r3, [r7]
	adds r1, r7, #0
	adds r0, r6, #0
	bl Func_02004b88
	mov r3, r9
	orrs r3, r0
	mov r9, r3
	ldr r3, [r7]
	adds r1, r7, #0
	adds r3, r3, r5
	str r3, [r7]
	ldr r3, [r7, #4]
	adds r0, r6, #0
	adds r3, r3, r5
	str r3, [r7, #4]
	bl Func_02004b88
	ldr r3, [r7, #4]
	mov r1, r9
	orrs r1, r0
	add r3, r8
	mov r9, r1
	str r3, [r7, #4]
	adds r1, r7, #0
	adds r0, r6, #0
	bl Func_02004b88
	ldr r3, [r7, #4]
	mov r2, r9
	adds r3, r3, r5
	orrs r2, r0
	str r3, [r7, #4]
	adds r0, r6, #0
	adds r1, r7, #0
	mov r9, r2
	bl Func_02004b88
	mov r3, r9
	orrs r3, r0
.L_02008800:
	mov r9, r3
.L_02008802:
	mov r0, r9
	cmp r0, #0
	beq .L_0200880a
	b .L_0200890a
.L_0200880a:
	movs r0, #78
	bl Func_02004e30
	bl Func_02004c20
	movs r0, #0
	bl Func_02004db8
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #10
	bl Func_02004df8
	ldr r5, .L_02008878
	movs r1, #128
	lsls r1, r1, #2
	adds r1, #18
	adds r2, r5, r1
	movs r3, #1
	strb r3, [r2]
	ldr r0, .L_02008870
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #118
	adds r3, r5, r2
	mov r1, r9
	mov r11, r0
	movs r0, #187
	strh r1, [r3]
	lsls r0, r0, #1
	bl GameFlag_ClearBit
	movs r3, #158
	lsls r3, r3, #2
	adds r2, r5, r3
	ldr r3, [r6, #8]
	movs r0, #159
	str r3, [r2]
	lsls r0, r0, #2
	ldr r3, [r6, #16]
	adds r2, r5, r0
	str r3, [r2]
	movs r1, #160
	ldrh r3, [r6, #6]
	lsls r1, r1, #2
	adds r5, r5, r1
	str r3, [r5]
	mov r7, r10
	ldr r3, [r6, #8]
	b .L_0200887c
	.2byte 0x0000
.L_02008870:
	.4byte 0x00000000
.L_02008874:
	.4byte 0xfff40000
.L_02008878:
	.4byte gPartyState
.L_0200887c:
	movs r5, #128
	mov r0, r10
	adds r7, #85
	lsls r5, r5, #11
	ldrb r2, [r7]
	str r3, [r0, #8]
	str r5, [r0, #12]
	mov r1, r11
	ldr r3, [r6, #16]
	mov r8, r2
	str r3, [r0, #16]
	strb r1, [r7]
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r1, #6
	mov r0, r10
	bl Func_02004b18
	movs r0, #6
	bl WaitFrames
	movs r0, #152
	bl Func_02004e30
	movs r1, #7
	mov r0, r10
	bl Func_02004b18
	movs r3, #192
	mov r2, r10
	lsls r3, r3, #10
	str r3, [r2, #48]
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r2, #52]
	mov r3, r8
	str r5, [r2, #40]
	mov r0, r10
	strb r3, [r7]
	bl Func_02004b48
	add r3, sp, #4
	movs r2, #0
	ldr r1, [r3]
	mov r0, r10
	ldr r3, [r3, #8]
	bl Func_02004b50
	mov r0, r10
	bl Func_02004b58
	movs r1, #6
	mov r0, r10
	bl Func_02004b18
	movs r0, #6
	bl WaitFrames
	bl Func_02004c28
	bl Func_02004dc8
	bl Func_02004dc0
	ldr r0, [sp, #0]
	movs r1, #174
	lsls r1, r1, #1
	adds r3, r0, r1
	mov r2, r9
	strh r2, [r3]
.L_0200890a:
	add sp, #16
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.section .text.x02008918,"ax",%progbits
	.global Func_02000918
	.thumb_func
Func_02000918:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	movs r0, #8
	ldr r7, [r3, #108]
	bl Object_GetById
	adds r6, r0, #0
	bl Func_02004c20
	movs r0, #0
	bl Func_02004db8
	ldr r5, .L_02008990
	movs r1, #128
	lsls r1, r1, #2
	adds r1, #18
	adds r3, r5, r1
	ldrb r3, [r3]
	cmp r3, #8
	bne .L_02008946
	bl Func_0200050c
.L_02008946:
	bl Event_ClearStatus1c6
	bl Event_WaitValue1c8Frames
	movs r3, #158
	lsls r3, r3, #2
	adds r2, r5, r3
	ldr r3, [r6, #8]
	movs r1, #159
	str r3, [r2]
	lsls r1, r1, #2
	ldr r3, [r6, #16]
	adds r2, r5, r1
	str r3, [r2]
	movs r2, #160
	lsls r2, r2, #2
	subs r1, #6
	adds r3, r5, r2
	movs r2, #0
	str r2, [r3]
	movs r0, #187
	adds r3, r5, r1
	strh r2, [r3]
	lsls r0, r0, #1
	bl GameFlag_ClearBit
	movs r0, #123
	bl Func_02004e30
	movs r2, #170
	lsls r2, r2, #1
	adds r3, r7, r2
	movs r1, #0
	ldrsh r0, [r3, r1]
	bl Func_02004d40
	pop {r5, r6, r7, pc}
.L_02008990:
	.4byte gPartyState
	.section .text.x02008994,"ax",%progbits
	.global Func_02000994
	.thumb_func
Func_02000994:
	push {r5, r6, r7, lr}
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #33
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020089a6
	b .L_02008bac
.L_020089a6:
	movs r0, #157
	lsls r0, r0, #4
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020089b4
	b .L_02008bac
.L_020089b4:
	ldr r3, .L_02008bd4
	movs r2, #133
	lsls r2, r2, #2
	adds r7, r3, r2
	ldr r0, [r7]
	bl Object_GetById
	ldr r6, [r0, #8]
	ldr r5, [r0, #16]
	bl Func_02004c20
	movs r0, #0
	bl Func_02004db8
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #179
	bl GameFlag_Test
	asrs r6, r6, #20
	asrs r5, r5, #20
	lsls r6, r6, #4
	lsls r5, r5, #4
	cmp r0, #0
	beq .L_02008a8c
	movs r0, #179
	lsls r0, r0, #9
	adds r0, #102
	movs r1, #6
	bl Func_02004df8
	ldr r1, [r7]
	movs r0, #5
	bl Func_02004c98
	movs r0, #1
	bl WaitFrames
	adds r1, r6, #0
	adds r2, r5, #0
	adds r1, #24
	subs r2, #12
	movs r0, #5
	bl ObjectMotion_SetPositionAndReset
	movs r1, #192
	movs r2, #0
	movs r0, #5
	lsls r1, r1, #7
	bl ObjectMotion_ArmCallback
	movs r1, #224
	lsls r1, r1, #8
	ldr r0, [r7]
	bl Func_02004d00
	bl Func_02004da8
	ldr r0, .L_02008bd8
	bl Func_02004cd8
	movs r0, #5
	movs r1, #0
	bl Func_02004cf0
	bl Func_02004db0
	movs r0, #5
	movs r1, #2
	bl Object_SetModeById
	ldr r0, [r7]
	bl Object_GetById
	cmp r0, #0
	beq .L_02008a5a
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #5
	bl ObjectMotion_ResetAndSetPosition
.L_02008a5a:
	movs r0, #5
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r1, #0
	movs r2, #0
	movs r0, #5
	bl Func_02004c88
	movs r0, #1
	bl WaitFrames
	movs r1, #224
	lsls r1, r1, #5
	movs r2, #128
	ldr r0, [r7]
	adds r1, #144
	lsls r2, r2, #6
	bl ObjectMotion_SetPositionAndReset
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #6
	bl Func_02004df8
	b .L_02008ba0
.L_02008a8c:
	movs r0, #179
	lsls r0, r0, #9
	adds r0, #102
	movs r1, #6
	bl Func_02004df8
	ldr r1, [r7]
	movs r0, #5
	bl Func_02004c98
	movs r0, #1
	bl WaitFrames
	subs r5, #12
	adds r1, r6, #0
	adds r1, #24
	movs r0, #5
	adds r2, r5, #0
	bl ObjectMotion_SetPositionAndReset
	movs r1, #192
	movs r2, #0
	movs r0, #5
	lsls r1, r1, #7
	bl ObjectMotion_ArmCallback
	movs r1, #224
	lsls r1, r1, #8
	ldr r0, [r7]
	bl Func_02004d00
	bl Func_02004da8
	ldr r0, .L_02008bdc
	bl Func_02004cd8
	movs r0, #5
	movs r1, #0
	bl Func_02004cf0
	bl Func_02004db0
	movs r1, #128
	movs r2, #0
	movs r0, #5
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r1, #129
	ldr r0, [r7]
	lsls r1, r1, #1
	bl Func_02004d10
	adds r1, r6, #0
	adds r2, r5, #0
	ldr r0, [r7]
	adds r1, #12
	bl ObjectMotion_SetPositionAndReset
	ldr r0, [r7]
	movs r1, #0
	bl Func_02004d00
	movs r0, #5
	movs r1, #4
	bl Object_SetModeById
	bl Func_02004da8
	movs r0, #5
	movs r1, #0
	bl Func_02004cf0
	movs r1, #10
	movs r2, #0
	adds r1, #255
	movs r0, #5
	bl Func_02004d08
	movs r0, #5
	movs r1, #0
	bl Func_02004cf0
	movs r0, #5
	movs r1, #3
	bl Object_SetModeById
	movs r0, #5
	movs r1, #0
	bl Func_02004cf0
	bl Func_02004db0
	movs r0, #5
	movs r1, #2
	bl Object_SetModeById
	ldr r0, [r7]
	bl Object_GetById
	cmp r0, #0
	beq .L_02008b66
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #5
	bl ObjectMotion_ResetAndSetPosition
.L_02008b66:
	movs r0, #5
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r1, #0
	movs r2, #0
	movs r0, #5
	bl Func_02004c88
	movs r0, #1
	bl WaitFrames
	movs r1, #224
	lsls r1, r1, #5
	movs r2, #128
	adds r1, #144
	ldr r0, [r7]
	lsls r2, r2, #6
	bl ObjectMotion_SetPositionAndReset
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #179
	bl GameFlag_SetBit
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #6
	bl Func_02004df8
.L_02008ba0:
	movs r0, #8
	bl WaitFrames
	bl Func_02004c28
	b .L_02008bd0
.L_02008bac:
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #108]
	bl Func_02004c20
	movs r0, #0
	bl Func_02004db8
	movs r0, #123
	bl Func_02004e30
	movs r2, #170
	lsls r2, r2, #1
	adds r5, r5, r2
	movs r3, #0
	ldrsh r0, [r5, r3]
	bl Func_02004d40
.L_02008bd0:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008bd4:
	.4byte gPartyState
.L_02008bd8:
	.4byte 0x00002b54
.L_02008bdc:
	.4byte 0x00002b50
	.section .text.x02008be0,"ax",%progbits
	.global Func_02000be0
	.thumb_func
Func_02000be0:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	sub sp, #12
	bl Random16Far
	lsls r0, r0, #4
	lsrs r5, r0, #16
	bl Func_02004c20
	movs r0, #0
	bl Func_02004db8
	ldr r0, .L_02008d0c
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
	cmp r5, #0
	bne .L_02008cba
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #79
	mov r8, r3
	mov r10, r2
	bl Func_02004e10
	bl Object_GetById
	ldr r3, .L_02008d10
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	adds r6, r0, #0
	ldr r0, [r3]
	bl Owner_GetState
	adds r5, r0, #0
	bl Random16Far
	ldrb r3, [r5, #15]
	movs r2, #128
	muls r3, r0
	lsls r2, r2, #8
	lsls r3, r3, #1
	lsrs r3, r3, #16
	adds r2, #1
	ldr r0, .L_02008d14
	movs r1, #1
	adds r7, r3, r2
	bl UiText_ShowPositionedMessageAndWait
	ldr r3, [r6, #8]
	mov r5, sp
	str r3, [r5]
	movs r0, #128
	ldr r3, [r6, #12]
	lsls r0, r0, #13
	str r3, [r5, #4]
	adds r2, r5, #0
	ldr r3, [r6, #16]
	str r3, [r5, #8]
	ldrh r1, [r6, #6]
	bl Vector_AddPolarOffsetFar
	movs r0, #79
	bl Object_GetById
	adds r6, r0, #0
	ldr r1, [r5]
	cmp r6, #0
	bne .L_02008c90
	movs r0, #234
	ldr r2, [r5, #4]
	ldr r3, [r5, #8]
	adds r0, #255
	bl Func_02004b30
	adds r6, r0, #0
	cmp r6, #0
	beq .L_02008d00
	mov r2, r10
	lsls r3, r2, #2
	adds r3, #20
	mov r2, r8
	str r6, [r2, r3]
	b .L_02008c9a
.L_02008c90:
	str r1, [r6, #8]
	ldr r3, [r5, #4]
	str r3, [r6, #12]
	ldr r3, [r5, #8]
	str r3, [r6, #16]
.L_02008c9a:
	movs r0, #79
	adds r1, r7, #0
	bl Func_02004e18
	movs r3, #208
	lsls r3, r3, #4
	adds r3, #52
	add r3, r8
	movs r0, #128
	strh r7, [r3]
	lsls r0, r0, #5
	mov r3, r10
	orrs r0, r3
	bl Func_02004d70
	b .L_02008cfc
.L_02008cba:
	cmp r5, #1
	bne .L_02008cf0
	movs r0, #192
	movs r1, #160
	movs r2, #128
	lsls r0, r0, #10
	lsls r1, r1, #11
	lsls r2, r2, #9
	bl Func_02004b78
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	negs r1, r1
	negs r0, r0
	adds r2, #102
	bl Func_02004b78
	movs r0, #30
	bl Battle_WaitMode0
	ldr r0, .L_02008d18
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
	b .L_02008cfc
.L_02008cf0:
	ldr r3, .L_02008d18
	lsrs r0, r5, #1
	adds r0, r0, r3
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
.L_02008cfc:
	bl Func_02004c28
.L_02008d00:
	add sp, #12
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008d0c:
	.4byte 0x00000de4
.L_02008d10:
	.4byte gPartyState
.L_02008d14:
	.4byte 0x00000de6
.L_02008d18:
	.4byte 0x00000de7
	.section .text.x02008d1c,"ax",%progbits
	.global Func_02000d1c
	.thumb_func
Func_02000d1c:
	push {lr}
	bl Func_02004c20
	movs r0, #0
	bl Func_02004db8
	movs r1, #204
	movs r2, #1
	movs r0, #19
	lsls r1, r1, #1
	negs r2, r2
	bl Func_02004c38
	cmp r0, #0
	bne .L_02008d44
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #119
	bl GameFlag_ClearBit
.L_02008d44:
	bl Func_02004c28
	pop {pc}
	.2byte 0x0000
	.section .text.x02008d4c,"ax",%progbits
	.global Func_02000d4c
	.thumb_func
Func_02000d4c:
	push {lr}
	movs r1, #204
	lsls r1, r1, #1
	movs r0, #19
	bl Func_02004e18
	movs r0, #19
	bl Object_GetById
	ldr r1, [r0, #80]
	movs r2, #1
	ldrb r3, [r1, #17]
	movs r0, #19
	orrs r3, r2
	strb r3, [r1, #17]
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r3, #128
	lsls r3, r3, #7
	movs r0, #19
	ldr r1, .L_02008d90
	ldr r2, .L_02008d94
	bl Func_02004c90
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #119
	bl GameFlag_SetBit
	pop {pc}
	.2byte 0x0000
.L_02008d90:
	.4byte 0x27b80000
.L_02008d94:
	.4byte 0x25b00000
	.section .text.x02008d98,"ax",%progbits
	.global Func_02000d98
	.thumb_func
Func_02000d98:
	push {lr}
	movs r1, #63
	movs r2, #62
	bl Func_02003654
	pop {pc}
	.section .text.x02008da4,"ax",%progbits
	.global Func_02000da4
	.thumb_func
Func_02000da4:
	push {lr}
	movs r1, #34
	movs r2, #33
	bl Func_02003654
	pop {pc}
	.section .text.x02008db0,"ax",%progbits
	.global Func_02000db0
	.thumb_func
Func_02000db0:
	push {lr}
	movs r1, #3
	movs r2, #2
	bl Func_02003654
	pop {pc}
	.section .text.x02008dbc,"ax",%progbits
	.global Func_02000dbc
	.thumb_func
Func_02000dbc:
	ldr r0, .L_02008dc0
	bx lr
.L_02008dc0:
	.4byte Data_020067c0
	.section .text.x02008dc4,"ax",%progbits
	.global Func_02000dc4
	.thumb_func
Func_02000dc4:
	push {r5, r6, r7, lr}
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008e24
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #222
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008e24
	ldr r5, .L_02009188
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #118
	adds r3, r5, r2
	movs r2, #0
	ldrsh r6, [r3, r2]
	cmp r6, #0
	bne .L_02008e36
	movs r3, #158
	lsls r3, r3, #2
	adds r7, r5, r3
	ldr r3, [r7]
	cmp r3, #0
	beq .L_02008e24
	movs r0, #8
	bl Object_GetById
	ldr r3, [r7]
	movs r2, #159
	str r3, [r0, #8]
	lsls r2, r2, #2
	adds r3, r5, r2
	ldr r3, [r3]
	adds r2, #4
	str r3, [r0, #16]
	adds r3, r5, r2
	ldr r3, [r3]
	strh r3, [r0, #6]
	adds r3, r0, #0
	adds r3, #100
	adds r0, #102
	strh r6, [r3]
	strh r6, [r0]
.L_02008e24:
	ldr r3, .L_02009188
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #118
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	beq .L_02008e40
.L_02008e36:
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #1
	bl Func_02004df8
.L_02008e40:
	movs r0, #162
	lsls r0, r0, #1
	bl GameFlag_SetBit
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #108]
	movs r3, #214
	lsls r3, r3, #1
	adds r2, r1, r3
	movs r3, #128
	lsls r3, r3, #3
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
	bl Func_02004d20
	movs r0, #48
	adds r0, #255
	bl GameFlag_ClearBit
	bl Func_02003960
	movs r0, #129
	lsls r0, r0, #4
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008ea0
	movs r2, #168
	movs r3, #144
	movs r0, #0
	movs r1, #240
	lsls r2, r2, #1
	lsls r3, r3, #1
	bl Func_02004b60
.L_02008ea0:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #33
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008eca
	movs r3, #128
	movs r0, #0
	movs r1, #224
	movs r2, #208
	lsls r3, r3, #1
	bl Func_02004b60
	movs r3, #128
	movs r0, #16
	movs r1, #224
	movs r2, #224
	lsls r3, r3, #1
	bl Func_02004b60
.L_02008eca:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #119
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008edc
	bl Func_02000d4c
.L_02008edc:
	ldr r1, .L_02009188
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r1, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	subs r3, #2
	cmp r3, #97
	bls .L_02008ef0
	b .L_020091ee
.L_02008ef0:
	ldr r2, .L_0200918c
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
.L_02008ef8:
	.4byte .L_0200910c
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_0200909c
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_02009080
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020090ca
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020090fa
	.4byte .L_02009100
	.4byte .L_020091ee
	.4byte .L_02009124
	.4byte .L_0200912a
	.4byte .L_02009130
	.4byte .L_02009106
	.4byte .L_020091ee
	.4byte .L_02009136
	.4byte .L_0200909c
	.4byte .L_0200909c
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020090be
	.4byte .L_020090c4
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091ee
	.4byte .L_020091a4
	.4byte .L_020091de
.L_02009080:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #39
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02009090
	b .L_020091ee
.L_02009090:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #171
	bl GameFlag_SetBit
	b .L_020091ee
.L_0200909c:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #118
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020090ac
	b .L_020091ee
.L_020090ac:
	ldr r3, .L_02009188
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r0, [r3, r2]
	bl Func_020012bc
	b .L_020091ee
.L_020090be:
	bl Func_020029a4
	b .L_020091ee
.L_020090c4:
	bl Func_02003490
	b .L_020091ee
.L_020090ca:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #10
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020090da
	b .L_020091ee
.L_020090da:
	movs r0, #150
	lsls r0, r0, #4
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020090ea
	b .L_020091ee
.L_020090ea:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #10
	bl GameFlag_SetBit
	bl Func_02001c5c
	b .L_020091ee
.L_020090fa:
	bl Func_02002698
	b .L_020091ee
.L_02009100:
	bl Func_02002ab0
	b .L_020091ee
.L_02009106:
	bl Func_02004c00
	b .L_020091ee
.L_0200910c:
	movs r0, #55
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020091ee
	ldr r1, .L_02009190
	movs r0, #9
	bl ObjectMotion_EnableActionAndSetCallback
	bl Func_02001e34
	b .L_020091ee
.L_02009124:
	bl Func_02002e00
	b .L_020091ee
.L_0200912a:
	bl Func_02002e78
	b .L_020091ee
.L_02009130:
	bl Func_0200310c
	b .L_020091ee
.L_02009136:
	movs r0, #2
	bl Func_02003d44
	movs r0, #7
	bl Object_GetById
	movs r1, #15
	bl Object_SetPartAttribute
	ldr r2, .L_02009194
	ldr r1, .L_02009198
	movs r0, #7
	bl Func_02004c88
	movs r0, #1
	bl WaitFrames
	ldr r0, .L_0200919c
	bl Func_02004cd8
	movs r1, #0
	movs r0, #7
	bl Func_02004cf0
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #7
	movs r1, #0
	bl Func_02004cf0
	bl Event_ClearStatus1c6
	bl Event_WaitValue1c8Frames
	ldr r0, .L_020091a0
	movs r1, #9
	bl Func_02004d38
	b .L_020091ee
	.2byte 0x0000
.L_02009188:
	.4byte gPartyState
.L_0200918c:
	.4byte .L_02008ef8
.L_02009190:
	.4byte Data_0200632c
.L_02009194:
	.4byte 0x1c840000
.L_02009198:
	.4byte 0x1c6d0000
.L_0200919c:
	.4byte 0x00002b4e
.L_020091a0:
	.4byte 0x00000109
.L_020091a4:
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #118
	movs r0, #128
	adds r3, r1, r2
	lsls r0, r0, #4
	movs r2, #1
	strh r2, [r3]
	adds r0, #222
	bl GameFlag_SetBit
	movs r0, #252
	lsls r0, r0, #3
	adds r0, #255
	bl GameFlag_SetBit
	movs r0, #100
	adds r0, #255
	bl GameFlag_SetBit
	movs r0, #30
	adds r0, #255
	bl GameFlag_SetBit
	ldr r0, .L_020091f4
	movs r1, #61
	bl Func_02004d38
	b .L_020091ee
.L_020091de:
	movs r0, #100
	adds r0, #255
	bl GameFlag_SetBit
	movs r0, #30
	adds r0, #255
	bl GameFlag_SetBit
.L_020091ee:
	movs r0, #0
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_020091f4:
	.4byte 0x00000002
	.section .text.x020091f8,"ax",%progbits
	.global Func_020011f8
	.thumb_func
Func_020011f8:
	movs r0, #0
	bx lr
	.section .text.x020091fc,"ax",%progbits
	.global Func_020011fc
	.thumb_func
Func_020011fc:
	push {r5, lr}
	bl Func_02004db0
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #6
	bl Func_02004df8
	bl Func_02004e00
	bl Func_02004da8
	movs r1, #2
	movs r0, #9
	bl Motion_SetVarCbAndRefresh
	movs r0, #118
	adds r0, #255
	bl GameFlag_Test
	ldr r3, .L_020092ac
	ldr r5, .L_020092b0
	subs r5, r5, r3
	muls r0, r5
	ldr r3, .L_020092b4
	adds r0, r0, r3
	bl Func_02004cd8
	movs r1, #0
	movs r0, #9
	bl Func_02004cf0
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #111
	bl Func_02004e30
	movs r1, #2
	movs r0, #0
	bl Menu_AnimateSelectionToEntry
	movs r0, #112
	adds r0, #255
	bl GameFlag_ClearBit
	movs r0, #114
	adds r0, #255
	bl GameFlag_ClearBit
	bl Func_02004e28
	movs r2, #30
	movs r1, #4
	movs r0, #9
	bl ObjectMotion_Launch
	movs r0, #118
	adds r0, #255
	bl GameFlag_Test
	ldr r3, .L_020092b8
	muls r0, r5
	adds r0, r0, r3
	bl Func_02004cd8
	movs r1, #0
	movs r0, #9
	bl Func_02004cf0
	movs r0, #112
	adds r0, #255
	bl GameFlag_ClearBit
	movs r0, #114
	adds r0, #255
	bl GameFlag_SetBit
	bl Func_02004e28
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #9
	movs r1, #5
	bl Func_02004d48
	pop {r5, pc}
.L_020092ac:
	.4byte 0x00002fd2
.L_020092b0:
	.4byte 0x00003007
.L_020092b4:
	.4byte 0x00002ffb
.L_020092b8:
	.4byte 0x00002ffc
	.section .text.x020092bc,"ax",%progbits
	.global Func_020012bc
	.thumb_func
Func_020012bc:
	push {r5, r6, lr}
	adds r5, r0, #0
	bl Func_02004c20
	movs r0, #0
	bl Func_02004db8
	cmp r5, #9
	beq .L_020092d0
	b .L_020095c0
.L_020092d0:
	movs r6, #192
	lsls r6, r6, #8
	adds r3, r6, #0
	ldr r1, .L_020095ac
	ldr r2, .L_020095b0
	movs r0, #20
	bl Func_02004c90
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	bl Func_02004da8
	ldr r0, .L_020095b4
	bl Func_02004cd8
	movs r2, #5
	movs r0, #20
	movs r1, #0
	bl Func_02004ce8
	bl Func_02004db0
	movs r0, #198
	lsls r0, r0, #9
	adds r0, #204
	movs r1, #16
	bl Func_02004df8
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #4
	bl Func_02004d08
	movs r0, #4
	movs r1, #0
	movs r2, #32
	bl ObjectMotion_CommitPositionAndActivate
	adds r1, r6, #0
	movs r2, #0
	movs r0, #4
	bl ObjectMotion_ArmCallback
	bl Func_02004e00
	movs r0, #4
	bl ObjectMotion_CommitCurrentPositionAndActivate
	bl Func_02004da8
	adds r3, r6, #0
	movs r2, #0
	movs r1, #16
	movs r0, #23
	bl Func_02004dd8
	movs r0, #23
	bl Object_RefreshSelectorById
	adds r1, r6, #0
	movs r0, #23
	bl Func_02004d00
	movs r5, #128
	movs r0, #10
	bl Battle_WaitMode0
	lsls r5, r5, #7
	movs r2, #150
	adds r3, r5, #0
	movs r0, #20
	ldr r1, .L_020095ac
	lsls r2, r2, #22
	bl Func_02004c90
	movs r2, #150
	adds r3, r5, #0
	movs r0, #21
	ldr r1, .L_020095ac
	lsls r2, r2, #22
	bl Func_02004c90
	movs r2, #150
	adds r3, r5, #0
	movs r0, #22
	ldr r1, .L_020095ac
	lsls r2, r2, #22
	bl Func_02004c90
	movs r0, #20
	movs r1, #0
	movs r2, #16
	bl ObjectMotion_OffsetPositionAndReset
	movs r1, #16
	movs r0, #21
	negs r1, r1
	movs r2, #16
	bl ObjectMotion_OffsetPositionAndReset
	movs r0, #22
	movs r1, #16
	movs r2, #16
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #192
	movs r0, #20
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #21
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #22
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #23
	bl Func_02004d08
	movs r0, #20
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #21
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #22
	bl ObjectMotion_CommitCurrentPositionAndActivate
	bl Func_02004da8
	movs r2, #5
	movs r0, #23
	movs r1, #0
	bl Func_02004ce8
	movs r1, #2
	movs r0, #20
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl Battle_WaitMode0
	movs r0, #20
	movs r1, #0
	movs r2, #5
	bl Func_02004ce8
	movs r2, #0
	movs r1, #23
	movs r0, #4
	bl Object_LinkPair
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #21
	movs r1, #2
	bl Motion_SetVarCbAndRefresh
	adds r1, r6, #0
	movs r0, #4
	movs r2, #0
	bl ObjectMotion_ArmCallback
	adds r1, r6, #0
	movs r2, #0
	movs r0, #23
	bl ObjectMotion_ArmCallback
	movs r0, #5
	bl Battle_WaitMode0
	movs r0, #21
	movs r1, #0
	movs r2, #5
	bl Func_02004ce8
	movs r1, #10
	adds r1, #255
	movs r2, #30
	movs r0, #23
	bl Func_02004d08
	movs r0, #23
	movs r1, #0
	movs r2, #5
	bl Func_02004ce8
	movs r1, #8
	adds r1, #255
	movs r2, #30
	movs r0, #22
	bl Func_02004d08
	movs r0, #22
	movs r1, #0
	movs r2, #5
	bl Func_02004ce8
	movs r1, #2
	adds r1, #255
	movs r2, #30
	movs r0, #23
	bl Func_02004d08
	movs r0, #23
	movs r1, #0
	movs r2, #5
	bl Func_02004ce8
	movs r1, #8
	adds r1, #255
	movs r2, #30
	movs r0, #20
	bl Func_02004d08
	movs r0, #20
	movs r1, #0
	movs r2, #5
	bl Func_02004ce8
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #23
	bl Func_02004d08
	movs r0, #23
	movs r1, #0
	movs r2, #5
	bl Func_02004ce8
	movs r0, #21
	movs r1, #0
	movs r2, #5
	bl Func_02004ce8
	movs r1, #23
	movs r2, #0
	movs r0, #4
	bl Object_LinkPair
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #23
	movs r1, #0
	movs r2, #5
	bl Func_02004ce8
	movs r1, #8
	adds r1, #255
	movs r2, #30
	movs r0, #22
	bl Func_02004d08
	adds r1, r6, #0
	movs r0, #4
	movs r2, #0
	bl ObjectMotion_ArmCallback
	adds r1, r6, #0
	movs r0, #23
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #22
	movs r1, #0
	movs r2, #5
	bl Func_02004ce8
	movs r2, #8
	movs r0, #4
	movs r1, #8
	negs r2, r2
	bl ObjectMotion_OffsetPositionAndReset
	movs r1, #8
	negs r1, r1
	movs r2, #8
	movs r0, #23
	bl ObjectMotion_OffsetPositionAndReset
	movs r0, #4
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #23
	bl ObjectMotion_CommitCurrentPositionAndActivate
	adds r1, r6, #0
	movs r0, #4
	movs r2, #0
	bl ObjectMotion_ArmCallback
	adds r1, r6, #0
	movs r0, #23
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #129
	movs r2, #30
	lsls r1, r1, #1
	movs r0, #4
	bl Func_02004d08
	movs r1, #3
	movs r0, #20
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #20
	movs r1, #0
	movs r2, #5
	bl Func_02004ce8
	ldr r3, .L_020095b8
	movs r1, #166
	lsls r1, r1, #1
	adds r1, #255
	adds r3, r3, r1
	movs r2, #2
	strb r2, [r3]
	ldr r5, .L_020095bc
	movs r1, #73
	adds r0, r5, #0
	bl Party_SetFields1eeAnd1f0
	adds r0, r5, #0
	movs r1, #74
	bl Party_SetFields1f2And1f4
	bl Func_02004db0
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r0, r0
	negs r1, r1
	negs r2, r2
	movs r3, #0
	bl Motion_CamBounds
	movs r0, #102
	movs r1, #3
	bl Func_02004d48
	b .L_02009c54
	.2byte 0x0000
.L_020095ac:
	.4byte 0x27b80000
.L_020095b0:
	.4byte 0x23280000
.L_020095b4:
	.4byte 0x00002fa5
.L_020095b8:
	.4byte gPartyState
.L_020095bc:
	.4byte 0x00000002
.L_020095c0:
	cmp r5, #74
	beq .L_020095c6
	b .L_0200976c
.L_020095c6:
	movs r0, #160
	lsls r0, r0, #4
	movs r5, #128
	lsls r5, r5, #7
	adds r0, #118
	bl GameFlag_ClearBit
	movs r0, #20
	ldr r1, .L_020099c0
	ldr r2, .L_020099c4
	adds r3, r5, #0
	bl Func_02004c90
	movs r0, #21
	ldr r1, .L_020099c8
	ldr r2, .L_020099c4
	adds r3, r5, #0
	bl Func_02004c90
	movs r0, #22
	ldr r1, .L_020099cc
	ldr r2, .L_020099c4
	adds r3, r5, #0
	bl Func_02004c90
	movs r3, #224
	movs r2, #151
	lsls r3, r3, #8
	movs r0, #23
	ldr r1, .L_020099c8
	lsls r2, r2, #22
	bl Func_02004c90
	movs r3, #160
	lsls r3, r3, #8
	ldr r2, .L_020099d0
	movs r0, #4
	ldr r1, .L_020099c0
	bl Func_02004c90
	movs r0, #4
	movs r1, #0
	bl Object_AttachWorkTargetToObject
	movs r0, #198
	lsls r0, r0, #9
	adds r0, #204
	movs r1, #1
	bl Func_02004df8
	movs r1, #19
	movs r0, #4
	bl Object_SetModeById
	movs r0, #4
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	bl Func_02004da8
	ldr r0, .L_020099d4
	bl Func_02004cd8
	movs r1, #2
	movs r0, #4
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl Battle_WaitMode0
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #20
	bl Func_02004d08
	movs r2, #5
	movs r0, #20
	movs r1, #0
	bl Func_02004ce8
	adds r1, r5, #0
	movs r0, #21
	bl Func_02004d00
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #5
	movs r0, #21
	movs r1, #0
	bl Func_02004ce8
	adds r1, r5, #0
	movs r0, #22
	bl Func_02004d00
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #22
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl Battle_WaitMode0
	movs r2, #5
	movs r0, #22
	movs r1, #0
	bl Func_02004ce8
	movs r1, #3
	movs r0, #21
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	adds r1, r5, #0
	movs r0, #20
	bl Func_02004d00
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #20
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl Battle_WaitMode0
	movs r2, #5
	movs r0, #20
	movs r1, #0
	bl Func_02004ce8
	movs r1, #3
	movs r0, #20
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #5
	movs r0, #20
	movs r1, #0
	bl Func_02004ce8
	movs r1, #3
	movs r0, #21
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #21
	movs r1, #0
	movs r2, #5
	bl Func_02004ce8
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #22
	bl Func_02004d08
	movs r2, #5
	movs r0, #22
	movs r1, #0
	bl Func_02004ce8
	movs r1, #3
	movs r0, #20
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #40
	bl Battle_WaitMode0
	ldr r2, .L_020099d8
	movs r1, #242
	lsls r1, r1, #1
	adds r3, r2, r1
	movs r1, #0
	ldrsh r0, [r3, r1]
	movs r1, #243
	lsls r1, r1, #1
	adds r3, r2, r1
	movs r2, #0
	ldrsh r1, [r3, r2]
	bl Func_02004d38
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r1, #214
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r2, #0
	str r2, [r3]
	b .L_02009c54
.L_0200976c:
	movs r0, #160
	lsls r0, r0, #4
	movs r6, #128
	lsls r6, r6, #7
	adds r0, #118
	bl GameFlag_ClearBit
	movs r0, #20
	ldr r1, .L_020099c0
	ldr r2, .L_020099dc
	adds r3, r6, #0
	bl Func_02004c90
	movs r5, #192
	movs r0, #21
	ldr r1, .L_020099c8
	ldr r2, .L_020099dc
	adds r3, r6, #0
	bl Func_02004c90
	lsls r5, r5, #8
	movs r0, #22
	ldr r1, .L_020099cc
	ldr r2, .L_020099dc
	adds r3, r6, #0
	bl Func_02004c90
	movs r0, #23
	ldr r1, .L_020099e0
	ldr r2, .L_020099e4
	adds r3, r5, #0
	bl Func_02004c90
	movs r1, #159
	adds r3, r5, #0
	ldr r2, .L_020099e4
	movs r0, #4
	lsls r1, r1, #22
	bl Func_02004c90
	movs r0, #4
	movs r1, #0
	bl Object_AttachWorkTargetToObject
	movs r0, #192
	movs r1, #1
	lsls r0, r0, #9
	bl Func_02004df8
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	bl Func_02004da8
	ldr r0, .L_020099e8
	bl Func_02004cd8
	movs r1, #2
	movs r0, #20
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl Battle_WaitMode0
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #20
	bl Func_02004d08
	movs r0, #20
	movs r1, #0
	movs r2, #5
	bl Func_02004ce8
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #21
	bl Func_02004d08
	movs r0, #21
	movs r1, #0
	movs r2, #5
	bl Func_02004ce8
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #22
	bl Func_02004d08
	movs r2, #5
	movs r0, #22
	movs r1, #0
	bl Func_02004ce8
	movs r1, #2
	movs r0, #20
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl Battle_WaitMode0
	movs r2, #5
	movs r0, #20
	movs r1, #0
	bl Func_02004ce8
	movs r1, #0
	movs r0, #21
	bl Func_02004d00
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #2
	adds r1, #255
	movs r2, #30
	movs r0, #21
	bl Func_02004d08
	movs r2, #5
	movs r0, #21
	movs r1, #0
	bl Func_02004ce8
	movs r1, #128
	movs r0, #20
	lsls r1, r1, #8
	bl Func_02004d00
	movs r1, #3
	movs r0, #20
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #5
	movs r0, #20
	movs r1, #0
	bl Func_02004ce8
	movs r1, #128
	lsls r1, r1, #8
	movs r0, #22
	bl Func_02004d00
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #2
	adds r1, #255
	movs r2, #30
	movs r0, #22
	bl Func_02004d08
	movs r0, #22
	movs r1, #0
	movs r2, #5
	bl Func_02004ce8
	movs r1, #2
	adds r1, #255
	movs r2, #30
	movs r0, #23
	bl Func_02004d08
	movs r0, #20
	adds r1, r6, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #21
	adds r1, r6, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #22
	adds r1, r6, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r2, #5
	movs r0, #23
	movs r1, #0
	bl Func_02004ce8
	movs r0, #20
	movs r1, #3
	bl Object_SetModeById
	movs r0, #21
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #22
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #10
	adds r1, #255
	movs r2, #30
	movs r0, #23
	bl Func_02004d08
	movs r0, #23
	movs r1, #0
	movs r2, #5
	bl Func_02004ce8
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #20
	bl Func_02004d08
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #21
	bl Func_02004d08
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #22
	bl Func_02004d08
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #20
	movs r1, #21
	movs r2, #0
	bl Object_LinkPair
	movs r1, #128
	movs r0, #22
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #6
	adds r1, #255
	movs r2, #30
	movs r0, #22
	bl Func_02004d08
	movs r1, #129
	movs r2, #40
	lsls r1, r1, #1
	movs r0, #20
	bl Func_02004d08
	adds r1, r6, #0
	movs r0, #20
	bl Func_02004d00
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #0
	movs r0, #20
	bl UiText_OpenMessageAtObject
	movs r0, #4
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_020099ec
	movs r1, #10
	adds r1, #255
	movs r2, #30
	movs r0, #20
	bl Func_02004d08
	movs r2, #5
	movs r0, #20
	movs r1, #0
	bl Func_02004ce8
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_02009a14
.L_020099c0:
	.4byte 0x27b80000
.L_020099c4:
	.4byte 0x25900000
.L_020099c8:
	.4byte 0x27a80000
.L_020099cc:
	.4byte 0x27c80000
.L_020099d0:
	.4byte 0x25b00000
.L_020099d4:
	.4byte 0x00002fb2
.L_020099d8:
	.4byte gPartyState
.L_020099dc:
	.4byte 0x25a00000
.L_020099e0:
	.4byte 0x27b00000
.L_020099e4:
	.4byte 0x25d00000
.L_020099e8:
	.4byte 0x00002fb9
.L_020099ec:
	movs r1, #10
	adds r1, #255
	movs r2, #30
	movs r0, #20
	bl Func_02004d08
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r1, #226
	lsls r1, r1, #1
	adds r2, r2, r1
	ldrh r3, [r2]
	movs r0, #20
	adds r3, #1
	strh r3, [r2]
	movs r1, #0
	movs r2, #5
	bl Func_02004ce8
.L_02009a14:
	movs r1, #23
	movs r2, #0
	movs r0, #4
	bl Object_LinkPair
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #192
	movs r0, #4
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #23
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #10
	adds r1, #255
	movs r2, #30
	movs r0, #23
	bl Func_02004d08
	movs r2, #5
	movs r0, #23
	movs r1, #0
	bl Func_02004ce8
	movs r0, #21
	movs r1, #2
	bl Motion_SetVarCbAndRefresh
	movs r1, #192
	movs r0, #21
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #22
	bl ObjectMotion_ArmCallback
	movs r0, #5
	bl Battle_WaitMode0
	movs r2, #5
	movs r0, #21
	movs r1, #0
	bl Func_02004ce8
	movs r0, #22
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r2, #5
	movs r0, #22
	movs r1, #0
	bl Func_02004ce8
	movs r1, #2
	movs r0, #20
	bl Motion_SetVarCbAndRefresh
	bl Func_02000d4c
	movs r0, #5
	bl Battle_WaitMode0
	movs r1, #10
	adds r1, #255
	movs r2, #30
	movs r0, #20
	bl Func_02004d08
	movs r0, #20
	movs r1, #0
	movs r2, #5
	bl Func_02004ce8
	movs r2, #0
	movs r1, #21
	movs r0, #20
	bl Object_LinkPair
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #21
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #20
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #0
	movs r1, #22
	movs r0, #20
	bl Object_LinkPair
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #22
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #20
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #20
	movs r1, #2
	bl Motion_SetVarCbAndRefresh
	movs r1, #128
	movs r0, #21
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r2, #0
	movs r0, #22
	lsls r1, r1, #7
	bl ObjectMotion_ArmCallback
	movs r1, #128
	lsls r1, r1, #7
	movs r0, #20
	bl Func_02004d00
	movs r0, #5
	bl Battle_WaitMode0
	movs r0, #20
	movs r1, #0
	movs r2, #5
	bl Func_02004ce8
	movs r1, #32
	movs r0, #20
	negs r1, r1
	movs r2, #0
	bl ObjectMotion_OffsetPositionAndReset
	movs r1, #32
	movs r0, #21
	negs r1, r1
	movs r2, #0
	bl ObjectMotion_OffsetPositionAndReset
	movs r1, #32
	movs r0, #22
	negs r1, r1
	movs r2, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #64
	movs r2, #64
	movs r0, #20
	negs r1, r1
	negs r2, r2
	bl ObjectMotion_OffsetPositionAndReset
	movs r1, #64
	movs r2, #64
	movs r0, #21
	negs r1, r1
	negs r2, r2
	bl ObjectMotion_OffsetPositionAndReset
	movs r1, #64
	movs r2, #64
	movs r0, #22
	negs r1, r1
	negs r2, r2
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #20
	movs r1, #0
	movs r2, #0
	bl Func_02004c88
	movs r0, #21
	movs r1, #0
	movs r2, #0
	bl Func_02004c88
	movs r2, #0
	movs r1, #0
	movs r0, #22
	bl Func_02004c88
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #7
	movs r0, #20
	bl Func_02004d00
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #23
	movs r1, #0
	bl Func_02004d00
	movs r1, #128
	movs r0, #4
	lsls r1, r1, #8
	bl Func_02004d00
	movs r1, #3
	movs r0, #23
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #5
	movs r0, #23
	movs r1, #0
	bl Func_02004ce8
	movs r0, #4
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #23
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #23
	ldr r1, .L_02009c58
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r0, #23
	movs r1, #2
	bl Object_SetModeById
	movs r0, #4
	bl Object_GetById
	cmp r0, #0
	beq .L_02009c32
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #23
	bl ObjectMotion_ResetAndSetPosition
.L_02009c32:
	movs r0, #23
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #23
	movs r1, #0
	movs r2, #0
	bl Func_02004c88
	bl Func_02004db0
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #15
	bl Func_02004df8
	bl Func_02004c28
.L_02009c54:
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02009c58:
	.4byte 0x00013333
	.section .text.x02009c5c,"ax",%progbits
	.global Func_02001c5c
	.thumb_func
Func_02001c5c:
	push {r5, lr}
	bl Func_02004c20
	movs r0, #0
	bl Func_02004db8
	movs r0, #16
	bl Object_GetById
	movs r5, #2
	adds r0, #85
	strb r5, [r0]
	movs r0, #17
	bl Object_GetById
	adds r0, #85
	strb r5, [r0]
	movs r0, #18
	bl Object_GetById
	adds r0, #85
	strb r5, [r0]
	movs r5, #224
	lsls r5, r5, #8
	adds r3, r5, #0
	movs r0, #16
	ldr r1, .L_02009e0c
	ldr r2, .L_02009e10
	bl Func_02004c90
	adds r3, r5, #0
	movs r0, #17
	ldr r1, .L_02009e14
	ldr r2, .L_02009e18
	bl Func_02004c90
	adds r3, r5, #0
	movs r0, #18
	ldr r1, .L_02009e14
	ldr r2, .L_02009e1c
	bl Func_02004c90
	bl Event_SetStatus1c6
	movs r1, #204
	movs r2, #200
	lsls r1, r1, #6
	lsls r2, r2, #5
	movs r0, #16
	adds r1, #51
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r1, #204
	movs r2, #200
	lsls r1, r1, #6
	lsls r2, r2, #5
	movs r0, #17
	adds r1, #51
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r1, #204
	movs r2, #200
	lsls r1, r1, #6
	lsls r2, r2, #5
	movs r0, #18
	adds r1, #51
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r0, #16
	movs r1, #10
	movs r2, #0
	bl ObjectMotion_OffsetPositionAndReset
	movs r0, #17
	movs r1, #10
	movs r2, #0
	bl ObjectMotion_OffsetPositionAndReset
	movs r2, #0
	movs r0, #18
	movs r1, #10
	bl ObjectMotion_OffsetPositionAndReset
	bl Event_WaitValue1c8Frames
	movs r1, #6
	ldr r0, .L_02009e20
	bl Func_02004df8
	bl Func_02004e00
	bl Func_02004da8
	movs r0, #16
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #16
	bl Object_SetModeById
	ldr r0, .L_02009e24
	bl Func_02004cd8
	movs r1, #208
	movs r0, #17
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #208
	movs r2, #0
	movs r0, #18
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r0, #16
	movs r1, #0
	bl Func_02004cf0
	movs r1, #3
	movs r0, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #16
	movs r1, #0
	bl Func_02004cf0
	movs r0, #16
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #4
	bl Object_SetModeById
	movs r0, #60
	bl Battle_WaitMode0
	movs r0, #4
	movs r1, #18
	bl Object_LinkObjectAndSetCallback
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #7
	lsls r2, r2, #6
	movs r0, #16
	adds r1, #102
	adds r2, #51
	bl ObjectMotion_SetSpeedParameters
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #7
	lsls r2, r2, #6
	movs r0, #17
	adds r1, #102
	adds r2, #51
	bl ObjectMotion_SetSpeedParameters
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #7
	lsls r2, r2, #6
	adds r2, #51
	movs r0, #18
	adds r1, #102
	bl ObjectMotion_SetSpeedParameters
	ldr r1, .L_02009e28
	movs r0, #16
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #40
	bl Battle_WaitMode0
	ldr r1, .L_02009e2c
	movs r0, #17
	bl ObjectMotion_EnableActionAndSetCallback
	ldr r1, .L_02009e30
	movs r0, #18
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #16
	bl Object_RefreshSelectorById
	movs r0, #17
	bl Object_RefreshSelectorById
	movs r0, #18
	bl Object_RefreshSelectorById
	bl Func_02004db0
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #6
	bl Func_02004df8
	bl Func_02004c28
	pop {r5, pc}
	.2byte 0x0000
.L_02009e0c:
	.4byte 0x22d10000
.L_02009e10:
	.4byte 0x16dd0000
.L_02009e14:
	.4byte 0x22e10000
.L_02009e18:
	.4byte 0x16ed0000
.L_02009e1c:
	.4byte 0x16fd0000
.L_02009e20:
	.4byte 0x00013333
.L_02009e24:
	.4byte 0x00002cd8
.L_02009e28:
	.4byte Data_02004e38
.L_02009e2c:
	.4byte Data_02004e58
.L_02009e30:
	.4byte Data_02004e88
	.section .text.x02009e34,"ax",%progbits
	.global Func_02001e34
	.thumb_func
Func_02001e34:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r0, #9
	sub sp, #4
	bl Object_GetById
	ldr r3, .L_0200a08c
	movs r1, #133
	lsls r1, r1, #2
	adds r5, r3, r1
	adds r6, r0, #0
	ldr r0, [r5]
	bl Object_GetById
	ldr r2, .L_0200a090
	ldr r3, [r0, #8]
	ldr r1, .L_0200a094
	adds r3, r3, r2
	lsrs r2, r3, #31
	adds r3, r3, r2
	mov r9, r1
	asrs r3, r3, #1
	add r3, r9
	str r3, [sp, #0]
	ldr r2, .L_0200a098
	ldr r3, [r0, #16]
	ldr r1, .L_0200a09c
	adds r3, r3, r2
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	mov r8, r1
	movs r0, #183
	add r3, r8
	lsls r0, r0, #1
	mov r11, r3
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009e90
	b .L_0200a368
.L_02009e90:
	ldr r3, .L_0200a0a0
	movs r2, #64
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	beq .L_02009ea4
	movs r0, #118
	adds r0, #255
	bl GameFlag_SetBit
.L_02009ea4:
	bl Func_02004c20
	movs r0, #0
	bl Func_02004db8
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	movs r0, #0
	bl Party_RemoveActiveOwner
	movs r0, #1
	bl Party_RemoveActiveOwner
	movs r0, #2
	bl Party_RemoveActiveOwner
	movs r0, #3
	bl Party_RemoveActiveOwner
	movs r0, #4
	bl Party_RemoveActiveOwner
	movs r0, #5
	bl Party_RemoveActiveOwner
	movs r0, #6
	bl Party_RemoveActiveOwner
	movs r0, #7
	bl Party_RemoveActiveOwner
	movs r0, #4
	bl Party_AddActiveOwner
	movs r0, #5
	bl Party_AddActiveOwner
	movs r0, #6
	bl Party_AddActiveOwner
	movs r0, #1
	ldr r7, [r5]
	bl Func_02004bf8
	movs r0, #183
	lsls r0, r0, #1
	bl GameFlag_SetBit
	bl Func_02004c20
	movs r0, #0
	bl Func_02004db8
	movs r1, #0
	movs r2, #7
	adds r0, r7, #0
	bl Djinn_AddToOwner
	movs r1, #0
	movs r2, #7
	adds r0, r7, #0
	bl Trade_AddOffer
	bl Func_02004da8
	movs r1, #9
	movs r2, #0
	adds r0, r7, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #2
	adds r1, #255
	movs r2, #60
	adds r0, r7, #0
	bl Func_02004d08
	adds r2, r6, #0
	movs r3, #1
	adds r2, #102
	strh r3, [r2]
	adds r1, r7, #0
	movs r2, #0
	movs r0, #9
	bl ObjectMotion_SetAngleToward
	movs r0, #16
	bl WaitFrames
	movs r0, #118
	adds r0, #255
	bl GameFlag_Test
	ldr r2, .L_0200a0a4
	ldr r3, .L_0200a0a8
	movs r5, #15
	subs r3, r3, r2
	muls r0, r3
	ldr r3, .L_0200a0ac
	adds r0, r0, r3
	bl Func_02004cd8
	movs r0, #9
	movs r1, #0
	bl Func_02004cf0
	bl Func_02004db0
	ldr r0, .L_0200a0b0
	movs r1, #6
	bl Func_02004df8
	bl Func_02004e00
	bl Func_02004da8
	movs r2, #85
	adds r2, r2, r6
	movs r3, #2
	strb r3, [r2]
	movs r3, #128
	lsls r3, r3, #7
	str r3, [r6, #72]
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r6, #48]
	str r3, [r6, #52]
	movs r3, #0
	str r3, [r6, #40]
	str r3, [r6, #20]
	mov r10, r2
	adds r0, r6, #0
	mov r1, r9
	movs r2, #0
	mov r3, r8
	bl Func_02004b50
.L_02009fbe:
	ldr r3, [r6, #24]
	movs r2, #128
	lsls r2, r2, #4
	adds r3, r3, r2
	str r3, [r6, #24]
	ldr r3, [r6, #28]
	movs r0, #1
	adds r3, r3, r2
	str r3, [r6, #28]
	subs r5, #1
	bl WaitFrames
	cmp r5, #0
	bge .L_02009fbe
	movs r0, #9
	adds r1, r7, #0
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r2, #0
	movs r1, #9
	adds r0, r7, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #16
	bl WaitFrames
	movs r3, #0
	str r3, [r6, #108]
	adds r0, r6, #0
	movs r1, #0
	bl Animation_ApplyChildValues
	movs r3, #128
	lsls r3, r3, #9
	movs r1, #0
	str r3, [r6, #72]
	movs r0, #9
	bl UiText_OpenMessageAtObject
	adds r0, r7, #0
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #1
	bne .L_0200a0b8
	movs r0, #9
	movs r1, #2
	movs r2, #20
	bl ObjectMotion_Launch
	movs r1, #2
	movs r2, #20
	movs r0, #9
	bl ObjectMotion_Launch
	movs r0, #118
	adds r0, #255
	bl GameFlag_Test
	ldr r2, .L_0200a0a8
	ldr r3, .L_0200a0a4
	ldr r1, .L_0200a0b4
	subs r5, r2, r3
	muls r0, r5
	mov r8, r1
	add r0, r8
	bl Func_02004cd8
	movs r1, #0
	movs r0, #9
	bl UiText_OpenMessageAtObject
	adds r0, r7, #0
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #1
	bne .L_0200a06c
	movs r0, #118
	adds r0, #255
	bl GameFlag_Test
	muls r0, r5
	mov r3, r8
	adds r3, #1
	b .L_0200a07a
.L_0200a06c:
	movs r0, #118
	adds r0, #255
	bl GameFlag_Test
	muls r0, r5
	mov r3, r8
	adds r3, #2
.L_0200a07a:
	adds r0, r0, r3
	bl Func_02004cd8
	movs r0, #9
	movs r1, #0
	bl Func_02004cf0
	b .L_0200a0d8
	.2byte 0x0000
.L_0200a08c:
	.4byte gPartyState
.L_0200a090:
	.4byte 0xd8780000
.L_0200a094:
	.4byte 0x27880000
.L_0200a098:
	.4byte 0xdf5c0000
.L_0200a09c:
	.4byte 0x20a40000
.L_0200a0a0:
	.4byte gInput
.L_0200a0a4:
	.4byte 0x00002fd2
.L_0200a0a8:
	.4byte 0x00003007
.L_0200a0ac:
	.4byte 0x00002fe1
.L_0200a0b0:
	.4byte 0x00013333
.L_0200a0b4:
	.4byte 0x00002fe3
.L_0200a0b8:
	movs r0, #118
	adds r0, #255
	bl GameFlag_Test
	ldr r3, .L_0200a348
	ldr r2, .L_0200a34c
	subs r3, r3, r2
	muls r0, r3
	ldr r3, .L_0200a350
	adds r0, r0, r3
	bl Func_02004cd8
	movs r0, #9
	movs r1, #0
	bl Func_02004cf0
.L_0200a0d8:
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #30
	adds r0, r7, #0
	bl Func_02004d08
	movs r0, #9
	movs r1, #2
	movs r2, #20
	bl ObjectMotion_Launch
	movs r1, #2
	movs r2, #20
	movs r0, #9
	bl ObjectMotion_Launch
	movs r0, #118
	adds r0, #255
	bl GameFlag_Test
	ldr r2, .L_0200a34c
	ldr r3, .L_0200a348
	movs r5, #15
	subs r3, r3, r2
	muls r0, r3
	ldr r3, .L_0200a354
	adds r0, r0, r3
	bl Func_02004cd8
	movs r0, #9
	movs r1, #0
	bl Func_02004cf0
	movs r1, #129
	movs r2, #30
	lsls r1, r1, #1
	adds r0, r7, #0
	bl Func_02004d08
	movs r0, #9
	movs r1, #0
	bl Func_02004cf0
	movs r1, #128
	movs r2, #30
	lsls r1, r1, #1
	adds r0, r7, #0
	bl Func_02004d08
	movs r0, #9
	movs r1, #0
	bl Func_02004cf0
	adds r0, r7, #0
	movs r1, #2
	bl Motion_SetVarCbAndRefresh
	movs r0, #9
	movs r1, #0
	bl Func_02004cf0
	movs r2, #30
	movs r0, #9
	movs r1, #2
	bl ObjectMotion_Launch
	movs r0, #9
	movs r1, #0
	bl Func_02004cf0
	movs r3, #0
	mov r2, r10
	strb r3, [r2]
	movs r2, #128
	adds r0, r6, #0
	ldr r1, [sp, #0]
	lsls r2, r2, #13
	mov r3, r11
	bl Func_02004b50
.L_0200a178:
	ldrh r3, [r6, #6]
	movs r1, #128
	lsls r1, r1, #5
	adds r3, r3, r1
	strh r3, [r6, #6]
	movs r0, #1
	subs r5, #1
	bl WaitFrames
	cmp r5, #0
	bge .L_0200a178
	adds r0, r7, #0
	movs r1, #1
	bl Object_SetModeById
	movs r1, #0
	movs r0, #9
	bl Func_02004cf0
	movs r2, #0
	movs r3, #2
	mov r1, r10
	strb r3, [r1]
	str r2, [r6, #40]
	str r2, [r6, #20]
	movs r5, #7
.L_0200a1ac:
	ldrh r3, [r6, #6]
	movs r2, #128
	lsls r2, r2, #5
	adds r3, r3, r2
	strh r3, [r6, #6]
	movs r0, #1
	subs r5, #1
	bl WaitFrames
	cmp r5, #0
	bge .L_0200a1ac
	adds r0, r7, #0
	movs r1, #22
	bl Object_SetModeById
	movs r0, #9
	movs r1, #0
	bl Func_02004cf0
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #9
	bl Func_02004d08
	movs r2, #0
	movs r0, #9
	adds r1, r7, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #9
	movs r1, #2
	bl Motion_SetVarCbAndRefresh
	movs r0, #9
	movs r1, #0
	bl Func_02004cf0
	movs r0, #9
	movs r1, #2
	movs r2, #30
	bl ObjectMotion_Launch
	movs r0, #9
	movs r1, #0
	bl UiText_OpenMessageAtObject
	movs r6, #0
.L_0200a20c:
	adds r0, r7, #0
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	adds r5, r0, #0
	cmp r6, #7
	bne .L_0200a254
	cmp r5, #0
	bne .L_0200a292
	movs r0, #9
	movs r1, #2
	movs r2, #20
	bl ObjectMotion_Launch
	movs r1, #2
	movs r2, #20
	movs r0, #9
	bl ObjectMotion_Launch
	movs r0, #118
	adds r0, #255
	bl GameFlag_Test
	ldr r3, .L_0200a348
	ldr r2, .L_0200a34c
	subs r3, r3, r2
	muls r0, r3
	ldr r3, .L_0200a358
	adds r0, r0, r3
	bl Func_02004cd8
	movs r0, #9
	movs r1, #0
	bl Func_02004cf0
	b .L_0200a2ce
.L_0200a254:
	cmp r5, #1
	bne .L_0200a292
	movs r0, #9
	movs r1, #2
	movs r2, #20
	bl ObjectMotion_Launch
	movs r1, #2
	movs r2, #20
	movs r0, #9
	bl ObjectMotion_Launch
	movs r0, #118
	adds r0, #255
	bl GameFlag_Test
	ldr r2, .L_0200a34c
	ldr r3, .L_0200a348
	subs r3, r3, r2
	muls r0, r3
	ldr r3, .L_0200a35c
	adds r0, r6, r0
	adds r0, r0, r3
	bl Func_02004cd8
	movs r0, #9
	movs r1, #0
	bl UiText_OpenMessageAtObject
	adds r6, #1
	b .L_0200a20c
.L_0200a292:
	adds r0, r7, #0
	movs r1, #22
	bl Object_SetModeById
	movs r0, #9
	movs r1, #2
	movs r2, #20
	bl ObjectMotion_Launch
	movs r1, #4
	movs r2, #20
	movs r0, #9
	bl ObjectMotion_Launch
	movs r0, #118
	adds r0, #255
	bl GameFlag_Test
	ldr r3, .L_0200a348
	ldr r2, .L_0200a34c
	subs r3, r3, r2
	muls r0, r3
	ldr r3, .L_0200a360
	adds r0, r0, r3
	bl Func_02004cd8
	movs r0, #9
	movs r1, #0
	bl Func_02004cf0
.L_0200a2ce:
	movs r0, #52
	movs r1, #4
	adds r0, #255
	bl Func_02004ba8
	movs r0, #81
	bl Func_02004e30
	movs r0, #118
	adds r0, #255
	bl GameFlag_Test
	ldr r3, .L_0200a34c
	ldr r6, .L_0200a348
	ldr r5, .L_0200a364
	subs r6, r6, r3
	muls r0, r6
	movs r1, #3
	adds r0, r0, r5
	bl UiText_ShowPositionedMessageAndWait
	movs r0, #118
	adds r0, #255
	bl GameFlag_Test
	muls r0, r6
	adds r5, #1
	adds r0, r0, r5
	bl Func_02004cd8
	movs r0, #9
	movs r1, #2
	movs r2, #20
	bl ObjectMotion_Launch
	movs r1, #0
	movs r0, #9
	bl UiText_OpenMessageAtObject
	adds r0, r7, #0
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #1
	bne .L_0200a32a
	b .L_0200a4c8
.L_0200a32a:
	movs r0, #118
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a33e
	movs r0, #17
	bl Func_02004e30
	b .L_0200a4be
.L_0200a33e:
	movs r0, #35
	bl Func_02004e30
	b .L_0200a4be
	.2byte 0x0000
.L_0200a348:
	.4byte 0x00003007
.L_0200a34c:
	.4byte 0x00002fd2
.L_0200a350:
	.4byte 0x00002fe6
.L_0200a354:
	.4byte 0x00002fe7
.L_0200a358:
	.4byte 0x00002ff7
.L_0200a35c:
	.4byte 0x00002ff0
.L_0200a360:
	.4byte 0x00002ff8
.L_0200a364:
	.4byte 0x00002ff9
.L_0200a368:
	bl Func_02004c20
	movs r0, #0
	bl Func_02004db8
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	movs r0, #9
	adds r1, r7, #0
	bl Func_02004c98
	movs r3, #160
	lsls r3, r3, #12
	str r3, [r6, #40]
	ldr r1, [sp, #0]
	mov r3, r11
	movs r2, #0
	adds r0, r6, #0
	bl Func_02004b50
	movs r0, #30
	bl Battle_WaitMode0
	bl Func_02004da8
	movs r0, #9
	adds r1, r7, #0
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r2, #0
	adds r0, r7, #0
	movs r1, #9
	bl ObjectMotion_SetAngleToward
	movs r1, #22
	adds r0, r7, #0
	bl Object_SetModeById
	movs r0, #118
	adds r0, #255
	bl GameFlag_Test
	ldr r2, .L_0200a57c
	ldr r3, .L_0200a580
	subs r5, r2, r3
	muls r0, r5
	ldr r3, .L_0200a584
	adds r0, r0, r3
	bl Func_02004cd8
	movs r0, #9
	movs r1, #2
	movs r2, #20
	bl ObjectMotion_Launch
	movs r2, #20
	movs r0, #9
	movs r1, #2
	bl ObjectMotion_Launch
	movs r0, #9
	movs r1, #0
	bl Func_02004cf0
	movs r0, #9
	movs r1, #2
	bl Motion_SetVarCbAndRefresh
	movs r1, #0
	movs r0, #9
	bl Func_02004cf0
	movs r0, #111
	bl Func_02004e30
	movs r1, #2
	movs r0, #0
	bl Menu_AnimateSelectionToEntry
	movs r0, #112
	adds r0, #255
	bl GameFlag_SetBit
	movs r0, #114
	adds r0, #255
	bl GameFlag_ClearBit
	bl Func_02004e28
	movs r0, #118
	adds r0, #255
	bl GameFlag_Test
	ldr r3, .L_0200a588
	muls r0, r5
	adds r0, r0, r3
	bl Func_02004cd8
	mov r3, r8
	movs r2, #0
	mov r1, r9
	adds r0, r6, #0
	bl Func_02004b50
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #9
	movs r1, #0
	bl Func_02004cf0
	movs r2, #0
	movs r0, #9
	adds r1, r7, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #9
	movs r1, #0
	bl Func_02004cf0
	movs r1, #0
	movs r0, #9
	bl UiText_OpenMessageAtObject
	adds r0, r7, #0
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #1
	bne .L_0200a4c8
	adds r0, r7, #0
	movs r1, #22
	bl Object_SetModeById
	movs r1, #2
	movs r0, #9
	bl Motion_SetVarCbAndRefresh
	movs r0, #118
	adds r0, #255
	bl GameFlag_Test
	ldr r3, .L_0200a58c
	muls r0, r5
	adds r0, r0, r3
	bl Func_02004cd8
	movs r1, #0
	movs r0, #9
	bl UiText_OpenMessageAtObject
	adds r0, r7, #0
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #1
	beq .L_0200a4c8
	movs r0, #9
	movs r1, #0
	bl Func_02004cf0
	ldr r3, [sp, #0]
	movs r0, #9
	asrs r1, r3, #16
	mov r3, r11
	asrs r2, r3, #16
	bl ObjectMotion_SetPositionAndReset
.L_0200a4be:
	bl Func_020011fc
	bl Func_02004db0
	b .L_0200a56c
.L_0200a4c8:
	movs r1, #22
	adds r0, r7, #0
	bl Object_SetModeById
	movs r0, #118
	adds r0, #255
	bl GameFlag_Test
	ldr r2, .L_0200a580
	ldr r3, .L_0200a57c
	subs r3, r3, r2
	muls r0, r3
	ldr r3, .L_0200a590
	adds r0, r0, r3
	bl Func_02004cd8
	movs r0, #9
	movs r1, #2
	movs r2, #20
	bl ObjectMotion_Launch
	movs r2, #20
	movs r0, #9
	movs r1, #2
	bl ObjectMotion_Launch
	movs r0, #9
	movs r1, #0
	bl Func_02004cf0
	movs r1, #128
	movs r2, #30
	lsls r1, r1, #1
	movs r0, #9
	bl Func_02004d08
	movs r1, #0
	movs r0, #9
	bl Func_02004cf0
	movs r0, #112
	adds r0, #255
	bl GameFlag_SetBit
	movs r0, #114
	adds r0, #255
	bl GameFlag_SetBit
	bl Func_02004e28
	movs r2, #20
	movs r0, #9
	movs r1, #2
	bl ObjectMotion_Launch
	movs r0, #9
	movs r1, #0
	bl Func_02004cf0
	bl Func_02004db0
	movs r1, #0
	movs r2, #7
	movs r0, #9
	bl Func_02004dd0
	bl Func_02004dc0
	bl Func_02004c28
	movs r0, #183
	lsls r0, r0, #1
	bl GameFlag_ClearBit
	movs r0, #112
	adds r0, #255
	bl GameFlag_ClearBit
	movs r0, #114
	adds r0, #255
	bl GameFlag_ClearBit
.L_0200a56c:
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200a57c:
	.4byte 0x00003007
.L_0200a580:
	.4byte 0x00002fd2
.L_0200a584:
	.4byte 0x00002ffd
.L_0200a588:
	.4byte 0x00002fff
.L_0200a58c:
	.4byte 0x00003002
.L_0200a590:
	.4byte 0x00003004
	.section .text.x0200a594,"ax",%progbits
	.global Func_02002594
	.thumb_func
Func_02002594:
	push {r5, r6, lr}
	bl Random16Far
	lsls r5, r0, #2
	adds r5, r5, r0
	bl Random16Far
	lsls r5, r5, #3
	lsls r3, r0, #4
	lsrs r5, r5, #16
	movs r2, #247
	subs r3, r3, r0
	lsls r2, r2, #19
	lsls r5, r5, #16
	adds r5, r5, r2
	lsls r3, r3, #1
	ldr r2, .L_0200a60c
	lsrs r3, r3, #16
	lsls r3, r3, #16
	movs r0, #30
	adds r3, r3, r2
	adds r0, #255
	adds r1, r5, #0
	movs r2, #0
	bl Func_02004b30
	adds r6, r0, #0
	cmp r6, #0
	beq .L_0200a608
	ldr r5, [r6, #80]
	bl Random16Far
	ldr r3, .L_0200a610
	lsls r0, r0, #15
	ldrb r2, [r5, #9]
	lsrs r0, r0, #16
	adds r0, r0, r3
	movs r3, #13
	negs r3, r3
	ands r3, r2
	movs r2, #8
	orrs r3, r2
	strb r3, [r5, #9]
	adds r3, r6, #0
	movs r1, #0
	adds r3, #85
	strb r1, [r5, #26]
	strb r1, [r3]
	str r0, [r6, #24]
	str r0, [r6, #28]
	movs r1, #1
	adds r0, r6, #0
	bl Func_02004b18
	ldr r1, .L_0200a614
	adds r0, r6, #0
	bl Func_02004b20
.L_0200a608:
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200a60c:
	.4byte 0x1c5c0000
.L_0200a610:
	.4byte 0x00013333
.L_0200a614:
	.4byte Data_02006bc8
	.section .text.x0200a618,"ax",%progbits
	.global Func_02002618
	.thumb_func
Func_02002618:
	push {r5, r6, r7, lr}
	adds r7, r0, #0
	adds r5, r1, #0
	adds r6, r2, #0
	bl Object_GetById
	cmp r0, #0
	beq .L_0200a63e
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	adds r1, r1, r5
	adds r2, r2, r6
	lsls r1, r1, #16
	lsls r2, r2, #16
	adds r0, r7, #0
	bl Func_02004c88
.L_0200a63e:
	pop {r5, r6, r7, pc}
	.section .text.x0200a640,"ax",%progbits
	.global Func_02002640
	.thumb_func
Func_02002640:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r8
	push {r5, r6}
	adds r5, r2, #0
	adds r6, r3, #0
	movs r2, #48
	movs r3, #224
	mov r10, r1
	mov r8, r0
	bl Func_02004b68
	adds r1, r5, #0
	adds r2, r6, #0
	movs r0, #5
	bl Func_02002618
	adds r1, r5, #0
	adds r2, r6, #0
	movs r0, #8
	bl Func_02002618
	adds r1, r5, #0
	adds r2, r6, #0
	movs r0, #9
	bl Func_02002618
	adds r1, r5, #0
	adds r2, r6, #0
	movs r0, #10
	bl Func_02002618
	movs r3, #16
	add r8, r3
	mov r0, r8
	mov r1, r10
	movs r2, #64
	movs r3, #224
	bl Func_02004b68
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, pc}
	.section .text.x0200a698,"ax",%progbits
	.global Func_02002698
	.thumb_func
Func_02002698:
	push {lr}
	movs r0, #1
	bl Func_02003d44
	ldr r0, .L_0200a8b4
	bl Func_02004cd8
	movs r0, #8
	movs r1, #0
	bl Func_02004cf0
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #9
	lsls r1, r1, #11
	lsls r0, r0, #11
	bl Func_02004b78
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #0
	movs r0, #5
	bl Func_02004cf0
	bl Func_02004db0
	movs r0, #145
	bl Func_02004e30
	movs r0, #254
	lsls r0, r0, #7
	adds r0, #255
	movs r1, #0
	bl Func_02004d80
	movs r0, #128
	movs r1, #0
	lsls r0, r0, #9
	bl Func_02004d78
	movs r0, #60
	bl Func_02004d88
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_0200a8b8
	bl Scheduler_AddOrUpdateCallback
	movs r0, #160
	movs r1, #160
	movs r2, #128
	lsls r1, r1, #11
	lsls r2, r2, #9
	lsls r0, r0, #11
	bl Func_02004b78
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #224
	movs r1, #192
	movs r2, #128
	lsls r1, r1, #11
	lsls r2, r2, #9
	lsls r0, r0, #11
	bl Func_02004b78
	movs r0, #30
	bl Battle_WaitMode0
	bl Func_02004da8
	movs r0, #192
	movs r1, #192
	movs r2, #128
	lsls r2, r2, #9
	lsls r0, r0, #10
	lsls r1, r1, #10
	bl Func_02004b78
	movs r0, #9
	movs r1, #0
	bl Func_02004cf0
	bl Func_02004db0
	movs r0, #224
	movs r1, #224
	movs r2, #128
	lsls r0, r0, #11
	lsls r1, r1, #11
	lsls r2, r2, #9
	bl Func_02004b78
	movs r1, #1
	ldr r2, .L_0200a8bc
	movs r3, #1
	ldr r0, .L_0200a8c0
	negs r1, r1
	bl Motion_CamBounds
	movs r0, #153
	lsls r0, r0, #8
	movs r1, #60
	adds r0, #153
	bl Func_02004df8
	bl Func_02004d30
	movs r0, #30
	bl Battle_WaitMode0
	bl Func_02004da8
	movs r0, #5
	movs r1, #0
	bl Func_02004cf0
	movs r1, #0
	movs r0, #8
	bl Func_02004cf0
	bl Func_02004db0
	movs r0, #145
	bl Func_02004e30
	movs r2, #0
	movs r3, #0
	movs r0, #0
	movs r1, #128
	bl Func_02002640
	movs r0, #5
	movs r1, #19
	bl Object_SetModeById
	movs r0, #8
	movs r1, #5
	bl Object_SetModeById
	movs r1, #7
	movs r0, #9
	bl Object_SetModeById
	movs r0, #4
	bl WaitFrames
	movs r0, #145
	bl Func_02004e30
	movs r1, #128
	movs r2, #32
	movs r3, #0
	movs r0, #32
	bl Func_02002640
	movs r0, #4
	bl WaitFrames
	movs r1, #128
	movs r2, #32
	movs r3, #0
	movs r0, #64
	bl Func_02002640
	movs r0, #4
	bl WaitFrames
	movs r1, #1
	movs r3, #1
	ldr r0, .L_0200a8c4
	negs r1, r1
	ldr r2, .L_0200a8bc
	bl Motion_CamBounds
	bl Func_02004d30
	bl Func_02004da8
	movs r0, #192
	movs r1, #192
	movs r2, #128
	lsls r2, r2, #9
	lsls r0, r0, #10
	lsls r1, r1, #10
	bl Func_02004b78
	movs r0, #9
	movs r1, #0
	bl Func_02004cf0
	movs r0, #5
	movs r1, #0
	bl Func_02004cf0
	bl Func_02004db0
	movs r0, #192
	movs r1, #192
	movs r2, #128
	lsls r0, r0, #10
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl Func_02004b78
	movs r1, #128
	movs r2, #32
	movs r3, #0
	movs r0, #96
	bl Func_02002640
	movs r0, #20
	bl WaitFrames
	movs r1, #160
	movs r2, #32
	movs r3, #0
	movs r0, #0
	bl Func_02002640
	movs r0, #20
	bl WaitFrames
	movs r0, #32
	movs r1, #160
	movs r2, #32
	movs r3, #0
	bl Func_02002640
	movs r1, #1
	ldr r2, .L_0200a8bc
	movs r3, #1
	ldr r0, .L_0200a8c8
	negs r1, r1
	bl Motion_CamBounds
	bl Func_02004d30
	bl Func_02004da8
	movs r0, #8
	movs r1, #0
	bl Func_02004cf0
	movs r1, #0
	movs r0, #5
	bl Func_02004cf0
	movs r0, #78
	bl Func_02004e30
	movs r0, #195
	lsls r0, r0, #1
	bl Func_02004e30
	bl Event_ClearStatus1c6
	bl Event_WaitValue1c8Frames
	movs r0, #64
	bl Func_02004d40
	movs r0, #120
	bl Battle_WaitMode0
	pop {pc}
	.2byte 0x0000
.L_0200a8b4:
	.4byte 0x0000163c
.L_0200a8b8:
	.4byte Func_02002594
.L_0200a8bc:
	.4byte 0x1d810000
.L_0200a8c0:
	.4byte 0x07c40000
.L_0200a8c4:
	.4byte 0x08040000
.L_0200a8c8:
	.4byte 0x08640000
	.section .text.x0200a8cc,"ax",%progbits
	.global Func_020028cc
	.thumb_func
Func_020028cc:
	push {r5, r6, r7, lr}
	ldr r3, .L_0200a994
	movs r2, #15
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	bne .L_0200a990
	movs r0, #8
	bl Object_GetById
	adds r6, r0, #0
	ldr r1, [r6, #8]
	ldr r0, .L_0200a998
	ldr r3, [r6, #16]
	adds r1, r1, r0
	ldr r0, .L_0200a99c
	ldr r2, [r6, #12]
	adds r3, r3, r0
	movs r0, #30
	adds r0, #255
	bl Func_02004b30
	adds r5, r0, #0
	cmp r5, #0
	beq .L_0200a990
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r5, #24]
	str r3, [r5, #28]
	ldr r7, [r5, #80]
	bl Random16Far
	lsls r0, r0, #1
	lsrs r0, r0, #16
	cmp r0, #0
	beq .L_0200a930
	bl Random16Far
	lsls r2, r0, #1
	adds r2, r2, r0
	lsls r2, r2, #4
	ldr r3, [r5, #8]
	lsrs r2, r2, #16
	lsls r2, r2, #16
	asrs r1, r2, #1
	subs r3, r3, r1
	str r3, [r5, #8]
	ldr r3, [r5, #16]
	subs r3, r3, r2
	b .L_0200a946
.L_0200a930:
	bl Random16Far
	ldr r3, [r5, #8]
	lsls r0, r0, #5
	lsrs r0, r0, #16
	lsls r0, r0, #16
	adds r3, r3, r0
	str r3, [r5, #8]
	ldr r3, [r5, #16]
	asrs r0, r0, #1
	adds r3, r3, r0
.L_0200a946:
	str r3, [r5, #16]
	movs r3, #0
	strb r3, [r7, #26]
	ldrb r1, [r7, #9]
	ldr r3, [r6, #80]
	movs r2, #12
	ldrb r3, [r3, #9]
	adds r0, r5, #0
	ands r2, r3
	movs r3, #13
	negs r3, r3
	ands r3, r1
	adds r1, r5, #0
	adds r1, #35
	orrs r3, r2
	ldrb r2, [r1]
	strb r3, [r7, #9]
	movs r3, #2
	orrs r3, r2
	strb r3, [r1]
	adds r3, r6, #0
	adds r3, #85
	ldrb r3, [r3]
	adds r2, r5, #0
	adds r2, #85
	strb r3, [r2]
	movs r1, #9
	bl Object_SetPartAttribute
	adds r0, r5, #0
	movs r1, #2
	bl Func_02004b18
	ldr r1, .L_0200a9a0
	adds r0, r5, #0
	bl Func_02004b20
.L_0200a990:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200a994:
	.4byte Data_0300122c
.L_0200a998:
	.4byte 0xffe00000
.L_0200a99c:
	.4byte 0xfff00000
.L_0200a9a0:
	.4byte Data_02004f10
	.section .text.x0200a9a4,"ax",%progbits
	.global Func_020029a4
	.thumb_func
Func_020029a4:
	push {r5, r6, lr}
	movs r0, #8
	bl Object_GetById
	adds r6, r0, #0
	movs r0, #60
	bl Battle_WaitMode0
	bl Func_02004c20
	movs r0, #0
	bl Func_02004db8
	movs r0, #153
	lsls r0, r0, #8
	adds r0, #153
	movs r1, #1
	bl Func_02004df8
	ldr r3, .L_0200aa9c
	movs r1, #1
	str r3, [r6, #24]
	str r3, [r6, #28]
	movs r0, #8
	bl Object_AttachWorkTargetToObject
	movs r0, #1
	bl WaitFrames
	ldr r5, .L_0200aaa0
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
	ldr r0, [r5]
	bl Object_GetById
	movs r1, #15
	bl Object_SetPartAttribute
	ldr r0, [r5]
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #8
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #7
	lsls r2, r2, #6
	adds r2, #51
	movs r0, #8
	adds r1, #102
	bl ObjectMotion_SetSpeedParameters
	adds r6, #100
	movs r3, #0
	strh r3, [r6]
	ldr r1, .L_0200aaa4
	movs r0, #8
	bl ObjectMotion_EnableActionAndSetCallback
	movs r5, #192
	movs r1, #144
	lsls r5, r5, #18
	lsls r1, r1, #3
	ldr r0, .L_0200aaa8
	bl Scheduler_AddOrUpdateCallback
	ldr r3, [r5, #108]
	movs r2, #214
	lsls r2, r2, #1
	movs r0, #128
	adds r3, r3, r2
	lsls r0, r0, #9
	subs r2, #172
	movs r1, #1
	str r2, [r3]
	adds r0, #3
	bl Func_02004d78
	ldr r2, [r5, #108]
	movs r6, #218
	movs r3, #32
	lsls r6, r6, #1
	str r3, [r2, r6]
	bl Event_SetStatus1c6
	movs r0, #120
	bl Battle_WaitMode0
	movs r0, #179
	lsls r0, r0, #9
	movs r1, #150
	lsls r1, r1, #1
	adds r0, #102
	bl Func_02004df8
	movs r0, #135
	lsls r0, r0, #1
	bl Battle_WaitMode0
	ldr r2, [r5, #108]
	movs r3, #16
	str r3, [r2, r6]
	ldr r3, .L_0200aa98
	movs r2, #160
	lsls r2, r2, #19
	strh r3, [r2]
	bl Event_ClearStatus1c6
	bl Event_WaitValue1c8Frames
	movs r0, #77
	bl Func_02004d40
	b .L_0200aaac
.L_0200aa98:
	.4byte 0x00007fff
.L_0200aa9c:
	.4byte 0x00013333
.L_0200aaa0:
	.4byte gPartyState
.L_0200aaa4:
	.4byte Data_02004eb8
.L_0200aaa8:
	.4byte Func_020028cc
.L_0200aaac:
	pop {r5, r6, pc}
	.2byte 0x0000
	.section .text.x0200aab0,"ax",%progbits
	.global Func_02002ab0
	.thumb_func
Func_02002ab0:
	push {r5, r6, r7, lr}
	movs r0, #10
	bl Object_GetById
	movs r3, #192
	lsls r3, r3, #18
	adds r5, r0, #0
	ldr r7, [r3, #32]
	bl Func_02004c20
	movs r0, #0
	bl Func_02004db8
	movs r2, #152
	movs r0, #64
	movs r1, #176
	lsls r2, r2, #1
	movs r3, #240
	bl Func_02004b68
	movs r2, #160
	movs r0, #80
	movs r1, #176
	lsls r2, r2, #1
	movs r3, #240
	bl Func_02004b68
	ldr r3, .L_0200adac
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r2, r2
	str r3, [r5, #24]
	str r3, [r5, #28]
	negs r1, r1
	movs r3, #0
	negs r0, r0
	bl Motion_CamBounds
	movs r0, #1
	bl WaitFrames
	ldr r6, .L_0200adb0
	movs r1, #133
	lsls r1, r1, #2
	adds r6, r6, r1
	ldr r0, [r6]
	bl Object_GetById
	movs r1, #15
	bl Object_SetPartAttribute
	ldr r0, [r6]
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #10
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #7
	lsls r2, r2, #6
	movs r0, #10
	adds r1, #102
	adds r2, #51
	bl ObjectMotion_SetSpeedParameters
	adds r2, r5, #0
	adds r2, #100
	movs r3, #0
	strh r3, [r2]
	movs r3, #128
	lsls r3, r3, #6
	ldr r2, [r5, #12]
	adds r0, r5, #0
	ldr r1, .L_0200adb4
	str r3, [r5, #52]
	str r3, [r5, #48]
	ldr r3, .L_0200adb8
	bl Func_02004b50
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	bl Func_02004da8
	ldr r0, .L_0200adbc
	bl Func_02004cd8
	movs r0, #5
	movs r1, #0
	bl Func_02004cf0
	movs r0, #8
	movs r1, #0
	bl Func_02004cf0
	ldr r2, [r5, #12]
	ldr r3, .L_0200adc0
	adds r0, r5, #0
	ldr r1, .L_0200adc4
	bl Func_02004b50
	movs r0, #6
	movs r1, #0
	bl Func_02004cf0
	movs r0, #5
	movs r1, #0
	bl Func_02004cf0
	movs r0, #6
	movs r1, #0
	bl Func_02004cf0
	movs r0, #8
	movs r1, #0
	bl Func_02004cf0
	movs r0, #6
	movs r1, #0
	bl Func_02004cf0
	movs r1, #0
	movs r0, #5
	bl Func_02004cf0
	movs r0, #78
	bl Func_02004e30
	movs r0, #162
	bl Func_02004e30
	ldr r0, [r6]
	movs r1, #0
	bl Func_02004cf0
	movs r1, #0
	movs r0, #8
	bl Func_02004cf0
	movs r0, #61
	bl Func_02004e30
	bl Func_02004db0
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #11
	lsls r1, r1, #8
	bl Func_02004d20
	movs r1, #1
	ldr r0, .L_0200adc8
	negs r1, r1
	ldr r2, .L_0200adcc
	movs r3, #1
	bl Motion_CamBounds
	movs r5, #31
.L_0200abfe:
	movs r3, #143
	lsls r3, r3, #1
	adds r2, r7, r3
	ldrh r3, [r2]
	movs r1, #255
	lsls r1, r1, #8
	adds r1, #128
	adds r3, r3, r1
	strh r3, [r2]
	movs r3, #142
	lsls r3, r3, #1
	adds r2, r7, r3
	ldrh r3, [r2]
	movs r0, #10
	adds r3, #128
	strh r3, [r2]
	bl Object_GetById
	ldr r1, .L_0200add0
	ldr r3, [r0, #28]
	subs r5, #1
	adds r3, r3, r1
	str r3, [r0, #28]
	movs r0, #1
	bl WaitFrames
	cmp r5, #0
	bge .L_0200abfe
	bl Func_02004da8
	movs r0, #128
	lsls r0, r0, #2
	bl GameFlag_SetBit
	movs r0, #5
	movs r1, #0
	bl Func_02004cf0
	movs r0, #6
	movs r1, #0
	bl Func_02004cf0
	movs r0, #5
	movs r1, #0
	bl Func_02004cf0
	movs r0, #6
	movs r1, #0
	bl Func_02004cf0
	movs r0, #8
	movs r1, #0
	bl Func_02004cf0
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #9
	lsls r0, r0, #9
	lsls r1, r1, #10
	bl Func_02004b78
	bl Func_02004db0
	movs r0, #153
	lsls r0, r0, #8
	adds r0, #153
	movs r1, #1
	bl Func_02004df8
	movs r3, #143
	lsls r3, r3, #1
	adds r2, r7, r3
	movs r1, #142
	movs r3, #0
	strh r3, [r2]
	lsls r1, r1, #1
	movs r3, #224
	adds r2, r7, r1
	lsls r3, r3, #8
	strh r3, [r2]
	movs r0, #1
	bl WaitFrames
	movs r1, #1
	movs r3, #0
	ldr r0, .L_0200add4
	negs r1, r1
	ldr r2, .L_0200add8
	bl Motion_CamBounds
	bl Func_02004b40
	movs r2, #238
	movs r0, #8
	ldr r1, .L_0200addc
	lsls r2, r2, #21
	bl Func_02004c88
	movs r0, #9
	ldr r1, .L_0200ade0
	ldr r2, .L_0200ade4
	bl Func_02004c88
	movs r0, #4
	ldr r1, .L_0200ade8
	ldr r2, .L_0200adec
	bl Func_02004c88
	movs r0, #5
	ldr r1, .L_0200adf0
	ldr r2, .L_0200adf4
	bl Func_02004c88
	ldr r1, .L_0200adf8
	movs r0, #6
	ldr r2, .L_0200adfc
	bl Func_02004c88
	ldr r5, .L_0200adb0
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
	ldr r0, [r5]
	bl Object_GetById
	movs r1, #0
	bl Object_SetPartAttribute
	ldr r0, [r5]
	bl Object_GetById
	movs r1, #1
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #1
	bl WaitFrames
	bl Func_02004da8
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #5
	bl Func_02004d10
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #5
	movs r1, #0
	bl Func_02004cf0
	movs r0, #9
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #9
	movs r1, #0
	bl Func_02004cf0
	movs r0, #6
	movs r1, #6
	movs r2, #15
	bl ObjectMotion_Launch
	movs r0, #6
	movs r1, #6
	movs r2, #23
	bl ObjectMotion_Launch
	movs r2, #0
	movs r0, #6
	movs r1, #4
	bl ObjectMotion_SetAngleToward
	movs r0, #6
	movs r1, #0
	bl Func_02004cf0
	movs r1, #2
	movs r0, #8
	adds r1, #255
	bl Func_02004d10
	movs r0, #8
	movs r1, #0
	bl Func_02004cf0
	movs r1, #128
	movs r2, #128
	movs r0, #8
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r1, #224
	movs r2, #240
	lsls r1, r1, #4
	lsls r2, r2, #5
	adds r1, #24
	adds r2, #15
	movs r0, #8
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r0, #163
	bl Func_02004e30
	movs r0, #0
	bl Func_020042e0
	movs r0, #65
	bl Func_02004d40
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200adac:
	.4byte 0x00013333
.L_0200adb0:
	.4byte gPartyState
.L_0200adb4:
	.4byte 0x27ff0000
.L_0200adb8:
	.4byte 0x1ecc0000
.L_0200adbc:
	.4byte 0x0000168b
.L_0200adc0:
	.4byte 0x1ebc0000
.L_0200adc4:
	.4byte 0x280f0000
.L_0200adc8:
	.4byte 0x283f0000
.L_0200adcc:
	.4byte 0x1e8c0000
.L_0200add0:
	.4byte 0xfffffd00
.L_0200add4:
	.4byte 0x0e470000
.L_0200add8:
	.4byte 0x1dd00000
.L_0200addc:
	.4byte 0x0e6a0000
.L_0200ade0:
	.4byte 0x0e3e0000
.L_0200ade4:
	.4byte 0x1dff0000
.L_0200ade8:
	.4byte 0x0e550000
.L_0200adec:
	.4byte 0x1df00000
.L_0200adf0:
	.4byte 0x0e490000
.L_0200adf4:
	.4byte 0x1dc50000
.L_0200adf8:
	.4byte 0x0e770000
.L_0200adfc:
	.4byte 0x1de30000
	.section .text.x0200ae00,"ax",%progbits
	.global Func_02002e00
	.thumb_func
Func_02002e00:
	push {r5, lr}
	ldr r5, .L_0200ae64
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	ldr r0, [r5]
	bl Object_GetById
	movs r3, #0
	str r3, [r0, #24]
	ldr r0, [r5]
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #11
	lsls r1, r1, #8
	bl Func_02004d20
	movs r1, #1
	movs r3, #1
	ldr r0, .L_0200ae68
	negs r1, r1
	ldr r2, .L_0200ae6c
	bl Motion_CamBounds
	b .L_0200ae50
.L_0200ae4a:
	movs r0, #1
	bl WaitFrames
.L_0200ae50:
	ldr r3, .L_0200ae70
	ldr r3, [r3]
	cmp r3, #0
	beq .L_0200ae4a
	ldr r0, .L_0200ae74
	movs r1, #80
	bl Func_02004d38
	pop {r5, pc}
	.2byte 0x0000
.L_0200ae64:
	.4byte gPartyState
.L_0200ae68:
	.4byte 0x2e720000
.L_0200ae6c:
	.4byte 0x276a0000
.L_0200ae70:
	.4byte gInput
.L_0200ae74:
	.4byte 0x00000061
	.section .text.x0200ae78,"ax",%progbits
	.global Func_02002e78
	.thumb_func
Func_02002e78:
	push {r5, r6, r7, lr}
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #222
	bl GameFlag_Test
	adds r6, r0, #0
	cmp r6, #0
	beq .L_0200ae8c
	b .L_0200b102
.L_0200ae8c:
	bl Func_02004c20
	movs r0, #0
	bl Func_02004db8
	ldr r5, .L_0200b104
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	ldr r0, [r5]
	bl Object_GetById
	ldr r7, [r0, #24]
	ldr r0, [r5]
	bl Object_GetById
	str r6, [r0, #24]
	ldr r0, [r5]
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	bl Func_02004da8
	ldr r0, .L_0200b108
	bl Func_02004cd8
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #7
	movs r1, #0
	movs r2, #10
	bl Func_02004ce8
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #8
	bl Func_02004d08
	movs r0, #4
	movs r1, #0
	movs r2, #10
	bl Func_02004ce8
	movs r0, #7
	movs r1, #0
	movs r2, #10
	bl Func_02004ce8
	movs r1, #2
	adds r1, #255
	movs r2, #30
	movs r0, #8
	bl Func_02004d08
	movs r0, #6
	movs r1, #0
	movs r2, #10
	bl Func_02004ce8
	movs r0, #9
	movs r1, #0
	movs r2, #10
	bl Func_02004ce8
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #8
	bl Func_02004d08
	movs r0, #5
	movs r1, #0
	movs r2, #10
	bl Func_02004ce8
	movs r1, #0
	movs r0, #7
	bl UiText_OpenMessageAtObject
	movs r0, #4
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_0200af70
	movs r1, #10
	adds r1, #255
	movs r2, #30
	movs r0, #8
	bl Func_02004d08
	movs r2, #10
	movs r0, #9
	movs r1, #0
	bl Func_02004ce8
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_0200af98
.L_0200af70:
	movs r1, #10
	adds r1, #255
	movs r2, #30
	movs r0, #8
	bl Func_02004d08
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #9
	adds r3, #1
	strh r3, [r2]
	movs r1, #0
	movs r2, #10
	bl Func_02004ce8
.L_0200af98:
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #8
	bl Func_02004d08
	movs r0, #7
	movs r1, #0
	movs r2, #10
	bl Func_02004ce8
	movs r0, #9
	movs r1, #0
	movs r2, #10
	bl Func_02004ce8
	movs r1, #2
	adds r1, #255
	movs r2, #30
	movs r0, #8
	bl Func_02004d08
	movs r0, #6
	movs r1, #0
	movs r2, #10
	bl Func_02004ce8
	movs r0, #9
	movs r1, #0
	movs r2, #10
	bl Func_02004ce8
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #8
	bl Func_02004d08
	movs r0, #5
	movs r1, #0
	movs r2, #10
	bl Func_02004ce8
	movs r0, #9
	movs r1, #0
	movs r2, #10
	bl Func_02004ce8
	movs r0, #6
	movs r1, #0
	movs r2, #10
	bl Func_02004ce8
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #8
	bl Func_02004d08
	movs r0, #5
	movs r1, #0
	movs r2, #10
	bl Func_02004ce8
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #8
	bl Func_02004d08
	movs r0, #6
	movs r1, #0
	movs r2, #10
	bl Func_02004ce8
	movs r0, #5
	movs r1, #0
	movs r2, #10
	bl Func_02004ce8
	movs r0, #9
	movs r1, #0
	movs r2, #10
	bl Func_02004ce8
	movs r1, #2
	adds r1, #255
	movs r2, #30
	movs r0, #8
	bl Func_02004d08
	movs r0, #7
	movs r1, #0
	movs r2, #10
	bl Func_02004ce8
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #8
	bl Func_02004d08
	movs r0, #9
	movs r1, #0
	movs r2, #10
	bl Func_02004ce8
	movs r0, #6
	movs r1, #0
	movs r2, #10
	bl Func_02004ce8
	movs r0, #5
	movs r1, #0
	movs r2, #10
	bl Func_02004ce8
	movs r0, #7
	movs r1, #0
	movs r2, #10
	bl Func_02004ce8
	movs r0, #9
	movs r1, #0
	movs r2, #10
	bl Func_02004ce8
	ldr r5, .L_0200b104
	movs r3, #133
	lsls r3, r3, #2
	adds r6, r5, r3
	movs r1, #0
	ldr r0, [r6]
	movs r2, #0
	bl Func_02004c88
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #18
	adds r2, r5, r3
	movs r3, #7
	strb r3, [r2]
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #118
	adds r5, r5, r3
	movs r3, #1
	strh r3, [r5]
	bl Func_02004db0
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #222
	bl GameFlag_SetBit
	movs r0, #7
	movs r1, #0
	movs r2, #0
	bl Func_02004c88
	movs r0, #6
	movs r1, #0
	movs r2, #0
	bl Func_02004c88
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_02004c88
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl Func_02004c88
	ldr r0, [r6]
	bl Object_GetById
	str r7, [r0, #24]
	bl Func_02004c28
.L_0200b102:
	pop {r5, r6, r7, pc}
.L_0200b104:
	.4byte gPartyState
.L_0200b108:
	.4byte 0x0000216b
	.section .text.x0200b10c,"ax",%progbits
	.global Func_0200310c
	.thumb_func
Func_0200310c:
	push {lr}
	bl Func_02004c20
	movs r0, #0
	bl Func_02004db8
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	movs r1, #164
	movs r2, #140
	lsls r1, r1, #6
	lsls r2, r2, #6
	movs r0, #8
	adds r1, #224
	adds r2, #115
	bl ObjectMotion_SetPositionAndCommit
	bl Event_ClearStatus1c6
	bl Event_WaitValue1c8Frames
	movs r0, #66
	bl Func_02004d40
	pop {pc}
	.2byte 0x0000
	.section .text.x0200b144,"ax",%progbits
	.global Func_02003144
	.thumb_func
Func_02003144:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r0, #252
	lsls r0, r0, #3
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200b15a
	b .L_0200b40a
.L_0200b15a:
	bl Func_02004c20
	movs r0, #0
	bl Func_02004db8
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #120
	bl GameFlag_SetBit
	movs r1, #216
	movs r2, #132
	lsls r1, r1, #5
	lsls r2, r2, #6
	adds r1, #246
	movs r0, #8
	adds r2, #84
	bl ObjectMotion_SetPositionAndCommit
	bl Func_0200050c
	movs r0, #8
	bl Object_GetById
	ldr r1, [r0, #8]
	cmp r1, #0
	bge .L_0200b198
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	adds r1, r1, r3
.L_0200b198:
	asrs r1, r1, #16
	movs r0, #8
	mov r8, r1
	bl Object_GetById
	ldr r2, [r0, #16]
	cmp r2, #0
	bge .L_0200b1b0
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	adds r2, r2, r3
.L_0200b1b0:
	asrs r7, r2, #16
	movs r6, #0
.L_0200b1b4:
	adds r0, r6, #0
	bl Object_GetById
	adds r5, r6, #1
	ldr r1, .L_0200b410
	lsls r2, r6, #2
	cmp r0, #0
	beq .L_0200b1ea
	str r6, [r1, r2]
	adds r0, r6, #0
	bl Object_GetById
	movs r3, #0
	str r3, [r0, #24]
	adds r0, r6, #0
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	mov r3, r8
	adds r0, r6, #0
	lsls r1, r3, #16
	lsls r2, r7, #16
	bl Func_02004c88
	b .L_0200b1f0
.L_0200b1ea:
	movs r3, #1
	negs r3, r3
	str r3, [r1, r2]
.L_0200b1f0:
	adds r6, r5, #0
	cmp r6, #7
	ble .L_0200b1b4
	movs r0, #23
	bl Object_GetById
	movs r3, #0
	str r3, [r0, #24]
	movs r0, #23
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	mov r3, r8
	lsls r1, r3, #16
	lsls r2, r7, #16
	movs r0, #23
	bl Func_02004c88
	bl Func_02004da8
	ldr r0, .L_0200b414
	bl Func_02004cd8
	movs r0, #195
	lsls r0, r0, #1
	bl Func_02004e30
	movs r0, #2
	movs r1, #0
	bl Func_02004cf0
	movs r1, #129
	movs r2, #40
	lsls r1, r1, #1
	movs r0, #8
	bl Func_02004d08
	movs r0, #1
	movs r1, #0
	bl Func_02004cf0
	movs r0, #3
	movs r1, #0
	bl Func_02004cf0
	movs r1, #0
	movs r0, #0
	bl Func_02004cf0
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #5
	movs r1, #0
	bl Func_02004cf0
	movs r0, #6
	movs r1, #0
	bl Func_02004cf0
	movs r1, #0
	movs r0, #23
	bl Func_02004cf0
	movs r0, #78
	bl Func_02004e30
	movs r0, #50
	bl Battle_WaitMode0
	movs r0, #6
	movs r1, #0
	bl Func_02004cf0
	movs r1, #0
	movs r0, #5
	bl Func_02004cf0
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #38
	bl Func_02004e30
	movs r1, #0
	movs r0, #6
	bl Func_02004cf0
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #0
	movs r0, #0
	bl Func_02004cf0
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #6
	movs r1, #0
	bl Func_02004cf0
	movs r1, #0
	movs r0, #7
	bl Func_02004cf0
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #0
	movs r0, #3
	bl Func_02004cf0
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #0
	movs r0, #2
	bl Func_02004cf0
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #1
	movs r1, #0
	bl Func_02004cf0
	movs r0, #6
	movs r1, #0
	bl Func_02004cf0
	movs r0, #1
	movs r1, #0
	bl Func_02004cf0
	movs r0, #6
	movs r1, #0
	bl Func_02004cf0
	movs r0, #1
	movs r1, #0
	bl Func_02004cf0
	movs r0, #5
	movs r1, #0
	bl Func_02004cf0
	movs r0, #3
	movs r1, #0
	bl Func_02004cf0
	movs r1, #0
	movs r0, #1
	bl Func_02004cf0
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #23
	movs r1, #0
	bl Func_02004cf0
	movs r0, #6
	movs r1, #0
	bl Func_02004cf0
	movs r1, #0
	movs r0, #23
	bl Func_02004cf0
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #0
	movs r0, #23
	bl Func_02004cf0
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #0
	movs r0, #23
	bl Func_02004cf0
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #23
	movs r1, #0
	bl Func_02004cf0
	movs r1, #0
	movs r0, #0
	bl Func_02004cf0
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #23
	movs r1, #0
	bl Func_02004cf0
	movs r0, #6
	movs r1, #0
	bl Func_02004cf0
	movs r1, #0
	movs r0, #23
	bl Func_02004cf0
	movs r0, #60
	bl Battle_WaitMode0
	movs r1, #0
	movs r0, #6
	bl Func_02004cf0
	movs r0, #78
	bl Func_02004e30
	movs r1, #0
	movs r0, #23
	bl Func_02004cf0
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #23
	movs r1, #0
	bl Func_02004cf0
	bl Func_02004dc0
	bl Func_02004db0
	movs r6, #0
.L_0200b3cc:
	ldr r7, .L_0200b410
	lsls r5, r6, #2
	ldr r0, [r7, r5]
	cmp r0, #0
	blt .L_0200b3f6
	bl Object_GetById
	movs r1, #1
	bl ObjectDispatch_SetSingleChildField26
	ldr r0, [r7, r5]
	bl Object_GetById
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r0, #24]
	ldr r0, [r7, r5]
	movs r1, #0
	movs r2, #0
	bl Func_02004c88
.L_0200b3f6:
	adds r6, #1
	cmp r6, #7
	ble .L_0200b3cc
	movs r0, #23
	movs r1, #0
	movs r2, #0
	bl Func_02004c88
	bl Func_02004c28
.L_0200b40a:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_0200b410:
	.4byte gOverlayArea + 0x6be8
.L_0200b414:
	.4byte 0x00002c61
	.section .text.x0200b418,"ax",%progbits
	.global Func_02003418
	.thumb_func
Func_02003418:
	push {r5, lr}
	adds r5, r0, #0
	bl Random16Far
	movs r3, #128
	lsls r3, r3, #7
	cmp r0, r3
	bcs .L_0200b438
	bl Random16Far
	adds r1, r0, #0
	lsls r1, r1, #3
	lsrs r1, r1, #16
	adds r0, r5, #0
	bl Animation_ApplyChildValues
.L_0200b438:
	movs r0, #0
	pop {r5, pc}
	.section .text.x0200b43c,"ax",%progbits
	.global Func_0200343c
	.thumb_func
Func_0200343c:
	push {r5, r6, lr}
	ldr r3, .L_0200b488
	ldr r6, [r3]
	movs r3, #7
	ands r6, r3
	cmp r6, #0
	bne .L_0200b484
	ldr r1, [r0, #8]
	ldr r2, [r0, #12]
	ldr r3, [r0, #16]
	movs r0, #209
	lsls r0, r0, #1
	adds r0, #255
	bl Func_02004b30
	adds r5, r0, #0
	cmp r5, #0
	beq .L_0200b484
	ldr r1, .L_0200b48c
	bl Func_02004b20
	adds r0, r5, #0
	movs r1, #1
	bl Func_02004b18
	adds r0, r5, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	adds r3, r5, #0
	adds r3, #85
	strb r6, [r3]
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r5, #24]
	str r3, [r5, #28]
.L_0200b484:
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200b488:
	.4byte Data_0300122c
.L_0200b48c:
	.4byte Data_02004f70
	.section .text.x0200b490,"ax",%progbits
	.global Func_02003490
	.thumb_func
Func_02003490:
	push {r5, lr}
	bl Func_02004c20
	movs r0, #0
	bl Func_02004db8
	ldr r5, .L_0200b5d8
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	ldr r0, [r5]
	bl Object_GetById
	movs r1, #15
	bl Object_SetPartAttribute
	ldr r0, [r5]
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	ldr r0, [r5]
	bl Object_GetById
	ldr r3, .L_0200b5dc
	str r3, [r0, #108]
	ldr r0, [r5]
	bl Object_GetById
	movs r3, #128
	lsls r3, r3, #6
	str r3, [r0, #52]
	ldr r0, [r5]
	bl Object_GetById
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r0, #48]
	movs r0, #5
	bl WaitFrames
	movs r0, #192
	lsls r0, r0, #8
	movs r1, #60
	bl Func_02004df8
	bl Event_SetStatus1c6
	movs r1, #224
	movs r2, #132
	lsls r1, r1, #6
	lsls r2, r2, #6
	ldr r0, [r5]
	adds r1, #201
	adds r2, #129
	bl ObjectMotion_SetPositionAndCommit
	movs r1, #224
	movs r2, #132
	lsls r1, r1, #6
	lsls r2, r2, #6
	ldr r0, [r5]
	adds r1, #129
	adds r2, #214
	bl ObjectMotion_SetPositionAndCommit
	movs r1, #224
	movs r2, #136
	lsls r1, r1, #6
	lsls r2, r2, #6
	ldr r0, [r5]
	adds r1, #116
	adds r2, #62
	bl ObjectMotion_SetPositionAndCommit
	movs r1, #224
	movs r2, #136
	lsls r1, r1, #6
	lsls r2, r2, #6
	ldr r0, [r5]
	adds r1, #51
	adds r2, #124
	bl ObjectMotion_SetPositionAndCommit
	movs r1, #220
	movs r2, #136
	lsls r1, r1, #6
	lsls r2, r2, #6
	ldr r0, [r5]
	adds r1, #251
	adds r2, #184
	bl ObjectMotion_SetPositionAndCommit
	movs r1, #220
	movs r2, #136
	lsls r1, r1, #6
	lsls r2, r2, #6
	ldr r0, [r5]
	adds r1, #204
	adds r2, #200
	bl ObjectMotion_SetPositionAndCommit
	movs r1, #220
	movs r2, #136
	lsls r1, r1, #6
	lsls r2, r2, #6
	ldr r0, [r5]
	adds r1, #175
	adds r2, #166
	bl ObjectMotion_SetPositionAndCommit
	movs r1, #220
	movs r2, #134
	lsls r1, r1, #6
	lsls r2, r2, #6
	ldr r0, [r5]
	adds r1, #175
	adds r2, #255
	bl ObjectMotion_SetPositionAndCommit
	movs r1, #220
	movs r2, #136
	lsls r1, r1, #6
	lsls r2, r2, #6
	ldr r0, [r5]
	adds r2, #68
	adds r1, #175
	bl ObjectMotion_SetPositionAndCommit
	movs r0, #160
	lsls r0, r0, #9
	movs r1, #120
	bl Func_02004df8
	movs r1, #220
	movs r2, #136
	lsls r1, r1, #6
	lsls r2, r2, #6
	ldr r0, [r5]
	adds r1, #195
	adds r2, #32
	bl ObjectMotion_SetPositionAndCommit
	movs r1, #220
	movs r2, #136
	lsls r1, r1, #6
	lsls r2, r2, #6
	ldr r0, [r5]
	adds r1, #177
	adds r2, #16
	bl ObjectMotion_SetPositionAndCommit
	bl Event_ClearStatus1c6
	bl Event_WaitValue1c8Frames
	bl Func_02004c28
	movs r0, #78
	bl Func_02004d40
	pop {r5, pc}
	.2byte 0x0000
.L_0200b5d8:
	.4byte gPartyState
.L_0200b5dc:
	.4byte Func_0200343c
	.section .text.x0200b5e0,"ax",%progbits
	.global Func_020035e0
	.thumb_func
Func_020035e0:
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	movs r2, #143
	lsls r2, r2, #1
	adds r3, r3, r2
	ldr r1, [r0, #80]
	ldrh r3, [r3]
	ldr r2, .L_0200b5fc
	strh r3, [r1, #18]
	strb r2, [r1, #26]
	movs r0, #1
	bx lr
	.2byte 0x0000
.L_0200b5fc:
	.4byte 0x00000000
	.section .text.x0200b600,"ax",%progbits
	.global Func_02003600
	.thumb_func
Func_02003600:
	push {r5, r6, r7, lr}
	adds r6, r2, #0
	ldr r2, .L_0200b648
	ldr r3, .L_0200b64c
	lsls r0, r0, #2
	adds r0, r0, r2
	ldr r5, [r3, r0]
	ldr r3, .L_0200b650
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
	bge .L_0200b636
	movs r2, #172
	lsls r2, r2, #1
	adds r3, r1, r2
	strh r7, [r3]
	b .L_0200b63e
.L_0200b636:
	movs r2, #172
	lsls r2, r2, #1
	adds r3, r1, r2
	strh r6, [r3]
.L_0200b63e:
	movs r0, #123
	bl Func_02004e30
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200b648:
	.4byte 0xfffffe70
.L_0200b64c:
	.4byte gOverlayArea + 0x6c08
.L_0200b650:
	.4byte gPartyState
	.section .text.x0200b654,"ax",%progbits
	.global Func_02003654
	.thumb_func
Func_02003654:
	push {r5, r6, r7, lr}
	adds r6, r2, #0
	ldr r2, .L_0200b69c
	ldr r3, .L_0200b6a0
	lsls r0, r0, #2
	adds r0, r0, r2
	ldr r5, [r3, r0]
	ldr r3, .L_0200b6a4
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
	bge .L_0200b68a
	movs r2, #172
	lsls r2, r2, #1
	adds r3, r1, r2
	strh r7, [r3]
	b .L_0200b692
.L_0200b68a:
	movs r2, #172
	lsls r2, r2, #1
	adds r3, r1, r2
	strh r6, [r3]
.L_0200b692:
	movs r0, #123
	bl Func_02004e30
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200b69c:
	.4byte 0xfffffe70
.L_0200b6a0:
	.4byte gOverlayArea + 0x6c08
.L_0200b6a4:
	.4byte gPartyState
	.section .text.x0200b6a8,"ax",%progbits
	.global Func_020036a8
	.thumb_func
Func_020036a8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #24
	bl Func_02004e08
	mov r8, r0
	bl Func_02004e10
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
	ldr r4, .L_0200b8c4
	cmp r3, r5
	bgt .L_0200b71e
	ldr r3, .L_0200b8c8
	movs r5, #160
	lsls r5, r5, #16
	adds r3, r1, r3
	adds r5, r1, r5
	ldr r1, .L_0200b8cc
	str r3, [sp, #12]
	movs r3, #200
	lsls r3, r3, #16
	adds r1, r0, r1
	adds r3, r0, r3
	str r5, [sp, #8]
	str r1, [sp, #4]
	str r3, [sp, #0]
	b .L_0200b73a
.L_0200b71e:
	ldr r5, .L_0200b8d0
	movs r3, #240
	adds r5, r1, r5
	str r5, [sp, #12]
	lsls r3, r3, #16
	ldr r5, .L_0200b8d4
	adds r3, r1, r3
	movs r1, #150
	lsls r1, r1, #17
	adds r5, r0, r5
	adds r1, r0, r1
	str r3, [sp, #8]
	str r5, [sp, #4]
	str r1, [sp, #0]
.L_0200b73a:
	movs r3, #0
	str r3, [sp, #16]
	adds r3, r2, #0
	mov r11, r4
	cmp r3, #0
	bne .L_0200b748
	b .L_0200b8b6
.L_0200b748:
	mov r5, r8
	ldrb r3, [r5, #1]
	cmp r3, #15
	bne .L_0200b752
	b .L_0200b89a
.L_0200b752:
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
	bgt .L_0200b76c
	b .L_0200b88a
.L_0200b76c:
	ldr r2, [sp, #8]
	cmp r5, r2
	blt .L_0200b774
	b .L_0200b88a
.L_0200b774:
	ldr r3, [sp, #4]
	cmp r7, r3
	bgt .L_0200b77c
	b .L_0200b88a
.L_0200b77c:
	ldr r0, [sp, #0]
	cmp r7, r0
	blt .L_0200b784
	b .L_0200b88a
.L_0200b784:
	mov r2, r8
	movs r1, #10
	ldrsh r0, [r2, r1]
	movs r3, #1
	negs r3, r3
	movs r2, #1
	cmp r0, r3
	beq .L_0200b7b4
	movs r3, #128
	lsls r3, r3, #5
	ands r3, r0
	cmp r3, #0
	beq .L_0200b7a6
	bl GameFlag_Test
	adds r2, r0, #0
	b .L_0200b7b4
.L_0200b7a6:
	bl GameFlag_Test
	negs r3, r0
	orrs r3, r0
	lsrs r3, r3, #31
	movs r2, #1
	subs r2, r2, r3
.L_0200b7b4:
	cmp r2, #0
	beq .L_0200b88a
	cmp r6, #0
	bne .L_0200b808
	mov r2, r8
	movs r1, #2
	ldrsh r0, [r2, r1]
	adds r3, r7, #0
	adds r1, r5, #0
	movs r2, #0
	bl Func_02004b30
	adds r6, r0, #0
	cmp r6, #0
	beq .L_0200b89a
	ldr r1, .L_0200b8d8
	bl Func_02004b20
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
	bl Func_02004b18
	ldr r5, [r6, #80]
	movs r1, #192
	ldr r0, [r5, #12]
	ldr r3, .L_0200b8dc
	lsls r1, r1, #8
	mov lr, r3
	.2byte 0xf800
	str r0, [r5, #12]
	mov r0, r11
	str r6, [r0]
.L_0200b808:
	ldr r3, .L_0200b8e0
	movs r1, #128
	lsls r1, r1, #2
	adds r1, #118
	adds r3, r3, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	bne .L_0200b89a
	ldr r3, [r6, #80]
	mov r5, r10
	ldr r0, [r3, #12]
	ldr r2, [r6, #8]
	ldr r3, [r5, #8]
	subs r1, r2, r3
	cmp r1, #0
	bge .L_0200b82c
	subs r1, r3, r2
.L_0200b82c:
	ldrh r3, [r6, #32]
	muls r3, r0
	ldr r0, [sp, #20]
	adds r2, r0, r3
	mov r3, r10
	ldr r0, [r6, #16]
	ldr r4, [r3, #16]
	subs r3, r0, r4
	cmp r3, #0
	blt .L_0200b848
	adds r3, r1, r3
	cmp r3, r2
	blt .L_0200b850
	b .L_0200b89a
.L_0200b848:
	subs r3, r4, r0
	adds r3, r1, r3
	cmp r3, r2
	bge .L_0200b89a
.L_0200b850:
	movs r0, #130
	lsls r0, r0, #1
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200b89a
	ldr r3, .L_0200b8e0
	movs r5, #128
	lsls r5, r5, #2
	adds r5, #18
	adds r3, r3, r5
	ldrb r3, [r3]
	cmp r3, #9
	bne .L_0200b87c
	movs r2, #179
	movs r3, #128
	lsls r2, r2, #1
	lsls r3, r3, #6
	add r2, r9
	adds r3, #139
	strh r3, [r2]
	b .L_0200b89a
.L_0200b87c:
	ldr r2, [sp, #16]
	movs r3, #170
	lsls r3, r3, #1
	adds r2, #100
	add r3, r9
	strh r2, [r3]
	b .L_0200b89a
.L_0200b88a:
	cmp r6, #0
	beq .L_0200b89a
	adds r0, r6, #0
	bl Func_02004b38
	movs r3, #0
	mov r0, r11
	str r3, [r0]
.L_0200b89a:
	ldr r1, [sp, #16]
	movs r2, #4
	adds r1, #1
	movs r3, #20
	str r1, [sp, #16]
	add r11, r2
	add r8, r3
	cmp r1, #127
	bhi .L_0200b8b6
	mov r5, r8
	ldrb r3, [r5]
	cmp r3, #0
	beq .L_0200b8b6
	b .L_0200b748
.L_0200b8b6:
	add sp, #24
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0200b8c4:
	.4byte gOverlayArea + 0x6c08
.L_0200b8c8:
	.4byte 0xff600000
.L_0200b8cc:
	.4byte 0xfed40000
.L_0200b8d0:
	.4byte 0xff100000
.L_0200b8d4:
	.4byte 0xfe3e0000
.L_0200b8d8:
	.4byte Data_02006bd4
.L_0200b8dc:
	.4byte IwramMulQ16
.L_0200b8e0:
	.4byte gPartyState
	.section .text.x0200b8e4,"ax",%progbits
	.global Func_020038e4
	.thumb_func
Func_020038e4:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #24]
	ldrh r3, [r3, #4]
	cmp r3, #0
	bne .L_0200b956
	ldr r0, .L_0200b958
	ldr r1, .L_0200b95c
	ldrh r3, [r1]
	adds r4, r3, #0
	strh r1, [r1]
	ldrh r2, [r0]
	cmp r2, #31
	bgt .L_0200b926
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
.L_0200b926:
	strh r4, [r1]
	ldrh r3, [r1]
	adds r4, r3, #0
	strh r1, [r1]
	ldrh r2, [r0]
	cmp r2, #31
	bgt .L_0200b954
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
.L_0200b954:
	strh r4, [r1]
.L_0200b956:
	pop {pc}
.L_0200b958:
	.4byte Data_020038e0
.L_0200b95c:
	.4byte 0x04000208
	.section .text.x0200b960,"ax",%progbits
	.global Func_02003960
	.thumb_func
Func_02003960:
	push {lr}
	movs r1, #128
	ldr r3, .L_0200b97c
	lsls r1, r1, #2
	ldr r0, .L_0200b980
	mov lr, r3
	.2byte 0xf800
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_0200b984
	bl Scheduler_AddOrUpdateCallback
	pop {pc}
	.2byte 0x0000
.L_0200b97c:
	.4byte IwramClearWords
.L_0200b980:
	.4byte gOverlayArea + 0x6c08
.L_0200b984:
	.4byte Func_020036a8
	.section .text.x0200b988,"ax",%progbits
	.global Func_02003988
	.thumb_func
Func_02003988:
	push {lr}
	cmp r0, #0
	bge .L_0200b992
	ldr r2, .L_0200b9a4
	adds r0, r0, r2
.L_0200b992:
	asrs r3, r0, #20
	cmp r1, #0
	bge .L_0200b99c
	ldr r2, .L_0200b9a4
	adds r1, r1, r2
.L_0200b99c:
	asrs r0, r1, #20
	lsls r0, r0, #7
	adds r0, r3, r0
	pop {pc}
.L_0200b9a4:
	.4byte 0x000fffff
	.section .text.x0200b9a8,"ax",%progbits
	.global Func_020039a8
	.thumb_func
Func_020039a8:
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
	ble .L_0200b9c4
	movs r0, #0
	b .L_0200b9ec
.L_0200b9c4:
	lsls r0, r0, #10
	bl Math_Sine
	movs r1, #160
	ldr r3, .L_0200b9f0
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
.L_0200b9ec:
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200b9f0:
	.4byte IwramMulQ16
	.section .text.x0200b9f4,"ax",%progbits
	.global Func_020039f4
	.thumb_func
Func_020039f4:
	push {r5, r6, r7, lr}
	adds r7, r0, #0
	movs r1, #192
	ldr r3, .L_0200ba7c
	lsls r1, r1, #9
	ldr r0, [r7, #24]
	mov lr, r3
	.2byte 0xf800
	movs r0, #16
	ldr r1, [r7, #8]
	ldr r2, [r7, #12]
	ldr r3, [r7, #16]
	adds r0, #255
	bl Func_02004b30
	adds r5, r0, #0
	cmp r5, #0
	beq .L_0200ba50
	ldr r3, [r7, #20]
	ldr r1, .L_0200ba80
	str r3, [r5, #20]
	ldr r6, [r5, #80]
	bl Func_02004b20
	adds r3, r5, #0
	adds r3, #85
	movs r2, #0
	strb r2, [r3]
	adds r3, #15
	strh r2, [r3]
	str r7, [r5, #104]
	cmp r6, #0
	beq .L_0200ba50
	adds r0, r6, #0
	movs r1, #2
	bl Animation_ApplyChildArgument
	ldr r3, .L_0200ba78
	ldrb r2, [r6, #9]
	strb r3, [r6, #26]
	movs r3, #13
	negs r3, r3
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	strb r3, [r6, #9]
.L_0200ba50:
	movs r0, #16
	ldr r1, [r7, #8]
	ldr r2, [r7, #12]
	ldr r3, [r7, #16]
	adds r0, #255
	bl Func_02004b30
	adds r5, r0, #0
	cmp r5, #0
	beq .L_0200baa4
	ldr r3, [r7, #20]
	ldr r1, .L_0200ba80
	str r3, [r5, #20]
	ldr r6, [r5, #80]
	bl Func_02004b20
	adds r3, r5, #0
	movs r2, #0
	adds r3, #85
	b .L_0200ba84
.L_0200ba78:
	.4byte 0x00000000
.L_0200ba7c:
	.4byte IwramMulQ16
.L_0200ba80:
	.4byte Data_02004f90
.L_0200ba84:
	strb r2, [r3]
	adds r3, #15
	strh r2, [r3]
	adds r2, r5, #0
	adds r2, #34
	movs r3, #1
	str r7, [r5, #104]
	strb r3, [r2]
	cmp r6, #0
	beq .L_0200baa4
	adds r0, r6, #0
	movs r1, #1
	bl Animation_ApplyChildArgument
	ldr r3, .L_0200baa8
	strb r3, [r6, #26]
.L_0200baa4:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200baa8:
	.4byte 0x00000000
	.section .text.x0200baac,"ax",%progbits
	.global Func_02003aac
	.thumb_func
Func_02003aac:
	push {r5, r6, r7, lr}
	adds r7, r0, #0
	movs r1, #192
	lsls r1, r1, #9
	ldr r3, .L_0200bb30
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
	bl Func_02004b30
	adds r5, r0, #0
	cmp r5, #0
	beq .L_0200bb2c
	ldr r3, [r7, #20]
	ldr r6, [r5, #80]
	str r3, [r5, #20]
	ldr r1, .L_0200bb34
	bl Func_02004b20
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
	beq .L_0200bb2c
	movs r3, #0
	strb r3, [r6, #26]
.L_0200bb2c:
	add sp, #12
	pop {r5, r6, r7, pc}
.L_0200bb30:
	.4byte IwramMulQ16
.L_0200bb34:
	.4byte Data_02004f9c
	.section .text.x0200bb38,"ax",%progbits
	.global Func_02003b38
	.thumb_func
Func_02003b38:
	push {r5, lr}
	ldr r3, .L_0200bb6c
	movs r2, #2
	ldr r3, [r3]
	adds r5, r0, #0
	ands r3, r2
	cmp r3, #0
	beq .L_0200bb50
	movs r1, #7
	bl Animation_ApplyChildValues
	b .L_0200bb58
.L_0200bb50:
	adds r0, r5, #0
	movs r1, #0
	bl Animation_ApplyChildValues
.L_0200bb58:
	ldr r3, .L_0200bb6c
	movs r2, #15
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	bne .L_0200bb6a
	adds r0, r5, #0
	bl Func_020039f4
.L_0200bb6a:
	pop {r5, pc}
.L_0200bb6c:
	.4byte Data_0300122c
	.section .text.x0200bb70,"ax",%progbits
	.global Func_02003b70
	.thumb_func
Func_02003b70:
	push {r5, r6, r7, lr}
	ldr r7, .L_0200bbac
	movs r2, #1
	ldr r3, [r7]
	adds r6, r0, #0
	ands r3, r2
	cmp r3, #0
	beq .L_0200bb98
	ldr r5, .L_0200bbb0
	ldr r2, .L_0200bbb4
	ldr r3, [r5]
	lsls r3, r3, #2
	ldr r1, [r2, r3]
	bl Animation_ApplyChildValues
	ldr r3, [r5]
	movs r2, #3
	adds r3, #1
	ands r3, r2
	str r3, [r5]
.L_0200bb98:
	ldr r3, [r7]
	movs r2, #3
	ands r3, r2
	cmp r3, #0
	bne .L_0200bba8
	adds r0, r6, #0
	bl Func_02003aac
.L_0200bba8:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200bbac:
	.4byte Data_0300122c
.L_0200bbb0:
	.4byte Data_02006be4
.L_0200bbb4:
	.4byte Data_02004fc8
	.section .text.x0200bbb8,"ax",%progbits
	.global Func_02003bb8
	.thumb_func
Func_02003bb8:
	push {lr}
	cmp r1, #0
	bne .L_0200bbd0
	str r1, [r0, #108]
	movs r1, #0
	bl Animation_ApplyChildValues
	movs r0, #195
	lsls r0, r0, #1
	bl Func_02004e30
	b .L_0200bbe6
.L_0200bbd0:
	cmp r1, #1
	bne .L_0200bbda
	ldr r3, .L_0200bbe8
	str r3, [r0, #108]
	b .L_0200bbe6
.L_0200bbda:
	ldr r3, .L_0200bbec
	str r3, [r0, #108]
	movs r0, #36
	adds r0, #255
	bl Func_02004e30
.L_0200bbe6:
	pop {pc}
.L_0200bbe8:
	.4byte Func_02003b38
.L_0200bbec:
	.4byte Func_02003b70
	.section .text.x0200bbf0,"ax",%progbits
	.global Func_02003bf0
	.thumb_func
Func_02003bf0:
	push {r5, r6, lr}
	bl Func_02004e10
	bl Object_GetById
	adds r5, r0, #0
	movs r0, #212
	bl Func_02004e30
	adds r0, r5, #0
	movs r1, #1
	bl Func_02003bb8
	movs r6, #0
	b .L_0200bc16
.L_0200bc0e:
	movs r0, #1
	bl WaitFrames
	adds r6, #1
.L_0200bc16:
	cmp r6, #19
	bgt .L_0200bc32
	adds r0, r5, #0
	adds r0, #8
	bl Func_02004b90
	adds r1, r0, #0
	cmp r1, #0
	beq .L_0200bc0e
	movs r0, #2
	bl Func_02004e20
	cmp r0, #0
	beq .L_0200bc0e
.L_0200bc32:
	adds r0, r5, #0
	movs r1, #2
	bl Func_02003bb8
	pop {r5, r6, pc}
	.section .text.x0200bc3c,"ax",%progbits
	.global Func_02003c3c
	.thumb_func
Func_02003c3c:
	push {lr}
	bl Func_02004e10
	bl Object_GetById
	movs r1, #0
	bl Func_02003bb8
	pop {pc}
	.2byte 0x0000
	.section .text.x0200bc50,"ax",%progbits
	.global Func_02003c50
	.thumb_func
Func_02003c50:
	push {lr}
	movs r0, #128
	movs r1, #252
	lsls r0, r0, #19
	lsls r1, r1, #6
	adds r0, #80
	adds r1, #65
	bl QueueIoWriteDelay2
	ldr r3, .L_0200bc88
	movs r2, #2
	ldr r3, [r3]
	ands r3, r2
	ldr r2, .L_0200bc8c
	cmp r3, #0
	beq .L_0200bc90
	ldrh r3, [r2]
	ldr r1, .L_0200bc84
	movs r0, #128
	lsls r0, r0, #19
	orrs r1, r3
	adds r0, #82
	bl QueueIoWriteDelay2
	b .L_0200bca0
	.2byte 0x0000
.L_0200bc84:
	.4byte 0x0000000c
.L_0200bc88:
	.4byte Data_0300122c
.L_0200bc8c:
	.4byte gOverlayArea + 0x6e08
.L_0200bc90:
	ldrh r3, [r2]
	ldr r1, .L_0200bca4
	movs r0, #128
	lsls r0, r0, #19
	orrs r1, r3
	adds r0, #82
	bl QueueIoWriteDelay2
.L_0200bca0:
	pop {pc}
	.2byte 0x0000
.L_0200bca4:
	.4byte 0x00000010
	.section .text.x0200bca8,"ax",%progbits
	.global Func_02003ca8
	.thumb_func
Func_02003ca8:
	push {r5, r6, lr}
	ldr r3, .L_0200bd38
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	movs r3, #10
	ldrsh r5, [r0, r3]
	ldr r3, .L_0200bd3c
	movs r2, #18
	ldrsh r6, [r0, r2]
	movs r1, #3
	ldr r0, [r3]
	bl Engine_MathModulo
	cmp r0, #0
	bne .L_0200bd34
	bl Random16Far
	lsls r0, r0, #2
	lsrs r0, r0, #16
	cmp r0, #1
	beq .L_0200bcf8
	cmp r0, #1
	bcc .L_0200bce8
	cmp r0, #2
	beq .L_0200bd08
	cmp r0, #3
	beq .L_0200bd20
	b .L_0200bd34
.L_0200bce8:
	ldr r3, .L_0200bd40
	lsls r0, r5, #16
	adds r0, r0, r3
	movs r3, #128
	lsls r3, r3, #9
	lsls r2, r6, #16
	movs r1, #1
	b .L_0200bd14
.L_0200bcf8:
	ldr r3, .L_0200bd40
	movs r2, #128
	lsls r2, r2, #9
	lsls r0, r5, #16
	adds r0, r0, r2
	movs r1, #1
	lsls r2, r6, #16
	b .L_0200bd14
.L_0200bd08:
	movs r3, #128
	lsls r3, r3, #9
	lsls r0, r5, #16
	lsls r2, r6, #16
	movs r1, #1
	adds r0, r0, r3
.L_0200bd14:
	adds r2, r2, r3
	negs r1, r1
	movs r3, #1
	bl Motion_CamBounds
	b .L_0200bd34
.L_0200bd20:
	ldr r3, .L_0200bd40
	lsls r0, r5, #16
	lsls r2, r6, #16
	movs r1, #1
	adds r0, r0, r3
	adds r2, r2, r3
	negs r1, r1
	movs r3, #1
	bl Motion_CamBounds
.L_0200bd34:
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200bd38:
	.4byte gPartyState
.L_0200bd3c:
	.4byte Data_0300122c
.L_0200bd40:
	.4byte 0xffff0000
	.section .text.x0200bd44,"ax",%progbits
	.global Func_02003d44
	.thumb_func
Func_02003d44:
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
	ldr r3, .L_0200be88
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
	bne .L_0200bdb4
	movs r0, #128
	movs r1, #192
	movs r2, #128
	lsls r0, r0, #10
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl Func_02004b78
	movs r0, #141
	bl Func_02004e30
.L_0200bdb4:
	bl Func_02004da8
	movs r0, #128
	lsls r0, r0, #7
	bl Runtime_BumpAllocateAlternatePool
	adds r7, r0, #0
	adds r1, r7, #0
	ldr r0, .L_0200be8c
	bl Func_02004aa8
	movs r2, #128
	lsls r2, r2, #5
	adds r1, r7, r2
	ldr r0, .L_0200be90
	bl Func_02004aa8
	ldr r6, .L_0200be94
	ldr r5, .L_0200be98
	ldrh r3, [r5]
	adds r0, r3, #0
	strh r5, [r5]
	ldrh r2, [r6]
	cmp r2, #31
	bgt .L_0200be0a
	lsls r3, r2, #1
	adds r3, r3, r2
	mov r1, r10
	adds r2, #1
	strh r2, [r6]
	lsls r2, r1, #5
	ldr r1, .L_0200be9c
	lsls r3, r3, #2
	adds r3, r3, r6
	adds r3, #4
	adds r2, r2, r1
	stmia r3!, {r2}
	ldr r2, .L_0200bea0
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #24
	adds r2, #16
	str r2, [r3]
.L_0200be0a:
	strh r0, [r5]
	ldrh r3, [r5]
	adds r1, r3, #0
	strh r5, [r5]
	ldrh r2, [r6]
	cmp r2, #31
	bgt .L_0200be30
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	adds r3, r3, r6
	adds r3, #4
	adds r2, #1
	stmia r3!, {r7}
	strh r2, [r6]
	ldr r2, .L_0200bea4
	stmia r3!, {r2}
	ldr r2, .L_0200bea8
	str r2, [r3]
.L_0200be30:
	strh r1, [r5]
	ldr r0, .L_0200beac
	movs r1, #144
	lsls r1, r1, #3
	bl Scheduler_AddOrUpdateCallback
	bl Func_02004c20
	movs r0, #0
	bl Func_02004db8
	movs r0, #246
	bl Func_02004e30
	ldr r2, .L_0200beb0
	ldr r3, .L_0200be84
	mov r8, r2
	mov r0, r8
	strh r3, [r0]
	ldrh r3, [r5]
	adds r1, r3, #0
	strh r5, [r5]
	ldrh r2, [r6]
	cmp r2, #31
	bgt .L_0200bebc
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
	ldr r2, .L_0200beb4
	stmia r3!, {r2}
	ldr r2, .L_0200beb8
	str r2, [r3]
	b .L_0200bebc
	.2byte 0x0000
.L_0200be84:
	.4byte 0x00000e00
.L_0200be88:
	.4byte gPartyState
.L_0200be8c:
	.4byte Data_02005312
.L_0200be90:
	.4byte Data_02005058
.L_0200be94:
	.4byte Data_020038e0
.L_0200be98:
	.4byte 0x04000208
.L_0200be9c:
	.4byte Data_02004fd8
.L_0200bea0:
	.4byte 0x050001c0
.L_0200bea4:
	.4byte 0x06001000
.L_0200bea8:
	.4byte 0x84000400
.L_0200beac:
	.4byte Func_02003c50
.L_0200beb0:
	.4byte gOverlayArea + 0x6e08
.L_0200beb4:
	.4byte 0x06002000
.L_0200beb8:
	.4byte 0x84000140
.L_0200bebc:
	strh r1, [r5]
	movs r0, #2
	bl Battle_WaitMode0
	ldr r3, .L_0200bef8
	mov r1, r8
	strh r3, [r1]
	ldrh r3, [r5]
	adds r1, r3, #0
	strh r5, [r5]
	ldrh r2, [r6]
	cmp r2, #31
	bgt .L_0200bf04
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
	ldr r2, .L_0200befc
	stmia r3!, {r2}
	ldr r2, .L_0200bf00
	str r2, [r3]
	b .L_0200bf04
	.2byte 0x0000
.L_0200bef8:
	.4byte 0x00000d00
.L_0200befc:
	.4byte 0x06002000
.L_0200bf00:
	.4byte 0x84000140
.L_0200bf04:
	strh r1, [r5]
	movs r0, #2
	bl Battle_WaitMode0
	ldr r3, .L_0200bf40
	mov r1, r8
	strh r3, [r1]
	ldrh r3, [r5]
	adds r1, r3, #0
	strh r5, [r5]
	ldrh r2, [r6]
	cmp r2, #31
	bgt .L_0200bf4c
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
	ldr r2, .L_0200bf44
	stmia r3!, {r2}
	ldr r2, .L_0200bf48
	str r2, [r3]
	b .L_0200bf4c
	.2byte 0x0000
.L_0200bf40:
	.4byte 0x00000c00
.L_0200bf44:
	.4byte 0x06002000
.L_0200bf48:
	.4byte 0x84000140
.L_0200bf4c:
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
	bgt .L_0200bf86
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
	ldr r2, .L_0200c040
	stmia r3!, {r2}
	ldr r2, .L_0200c044
	str r2, [r3]
.L_0200bf86:
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
	bgt .L_0200bfc4
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
	ldr r2, .L_0200c040
	stmia r3!, {r2}
	ldr r2, .L_0200c044
	str r2, [r3]
.L_0200bfc4:
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
	bgt .L_0200c002
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
	ldr r2, .L_0200c040
	stmia r3!, {r2}
	ldr r2, .L_0200c044
	str r2, [r3]
.L_0200c002:
	strh r1, [r5]
	movs r0, #2
	bl Battle_WaitMode0
	ldr r3, .L_0200c03c
	mov r1, r8
	strh r3, [r1]
	ldrh r3, [r5]
	adds r1, r3, #0
	strh r5, [r5]
	ldrh r2, [r6]
	cmp r2, #31
	bgt .L_0200c048
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
	ldr r2, .L_0200c040
	stmia r3!, {r2}
	ldr r2, .L_0200c044
	str r2, [r3]
	b .L_0200c048
.L_0200c03c:
	.4byte 0x00000800
.L_0200c040:
	.4byte 0x06002000
.L_0200c044:
	.4byte 0x84000140
.L_0200c048:
	strh r1, [r5]
	movs r0, #140
	bl Battle_WaitMode0
	ldrh r3, [r5]
	adds r1, r3, #0
	strh r5, [r5]
	ldrh r2, [r6]
	cmp r2, #31
	bgt .L_0200c07a
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
	ldr r2, .L_0200c1c8
	stmia r3!, {r2}
	ldr r2, .L_0200c1cc
	str r2, [r3]
.L_0200c07a:
	strh r1, [r5]
	movs r0, #4
	bl Battle_WaitMode0
	ldrh r3, [r5]
	adds r1, r3, #0
	strh r5, [r5]
	ldrh r2, [r6]
	cmp r2, #31
	bgt .L_0200c0ac
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
	ldr r2, .L_0200c1c8
	stmia r3!, {r2}
	ldr r2, .L_0200c1cc
	str r2, [r3]
.L_0200c0ac:
	strh r1, [r5]
	movs r0, #4
	bl Battle_WaitMode0
	ldrh r3, [r5]
	adds r1, r3, #0
	strh r5, [r5]
	ldrh r2, [r6]
	cmp r2, #31
	bgt .L_0200c0de
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
	ldr r2, .L_0200c1c8
	stmia r3!, {r2}
	ldr r2, .L_0200c1cc
	str r2, [r3]
.L_0200c0de:
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
	bgt .L_0200c116
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
	ldr r2, .L_0200c1c8
	stmia r3!, {r2}
	ldr r2, .L_0200c1cc
	str r2, [r3]
.L_0200c116:
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
	bgt .L_0200c14e
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
	ldr r2, .L_0200c1c8
	stmia r3!, {r2}
	ldr r2, .L_0200c1cc
	str r2, [r3]
.L_0200c14e:
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
	bgt .L_0200c188
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
	ldr r2, .L_0200c1c8
	stmia r3!, {r2}
	ldr r2, .L_0200c1cc
	str r2, [r3]
.L_0200c188:
	strh r1, [r5]
	movs r0, #4
	bl Battle_WaitMode0
	ldr r3, .L_0200c1c4
	mov r1, r8
	strh r3, [r1]
	ldrh r3, [r5]
	adds r1, r3, #0
	strh r5, [r5]
	ldrh r2, [r6]
	cmp r2, #31
	bgt .L_0200c1d0
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
	ldr r2, .L_0200c1c8
	stmia r3!, {r2}
	ldr r2, .L_0200c1cc
	str r2, [r3]
	b .L_0200c1d0
	.2byte 0x0000
.L_0200c1c4:
	.4byte 0x00000c00
.L_0200c1c8:
	.4byte 0x06002000
.L_0200c1cc:
	.4byte 0x84000140
.L_0200c1d0:
	strh r1, [r5]
	mov r1, r10
	cmp r1, #1
	bne .L_0200c25c
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #10
	lsls r2, r2, #9
	lsls r0, r0, #10
	bl Func_02004b78
	movs r0, #141
	bl Func_02004e30
	ldr r3, .L_0200c228
	mov r2, r8
	strh r3, [r2]
	movs r0, #4
	bl Battle_WaitMode0
	ldr r3, .L_0200c22c
	mov r0, r8
	strh r3, [r0]
	movs r0, #4
	bl Battle_WaitMode0
	ldr r3, .L_0200c230
	mov r1, r8
	strh r3, [r1]
	movs r0, #4
	bl Battle_WaitMode0
	ldr r3, .L_0200c234
	mov r2, r8
	strh r3, [r2]
	movs r0, #45
	bl Battle_WaitMode0
	ldr r0, .L_0200c238
	bl Scheduler_RemoveCallbackFar
	b .L_0200c23c
	.2byte 0x0000
.L_0200c228:
	.4byte 0x00000d00
.L_0200c22c:
	.4byte 0x00000e00
.L_0200c230:
	.4byte 0x00000f00
.L_0200c234:
	.4byte 0x00001000
.L_0200c238:
	.4byte Func_02003c50
.L_0200c23c:
	bl Func_02004ac8
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
	b .L_0200c2c6
.L_0200c25c:
	ldr r3, .L_0200c294
	mov r0, r8
	strh r3, [r0]
	movs r0, #4
	bl Battle_WaitMode0
	ldr r3, .L_0200c298
	mov r1, r8
	strh r3, [r1]
	movs r0, #4
	bl Battle_WaitMode0
	ldr r3, .L_0200c29c
	mov r2, r8
	strh r3, [r2]
	movs r0, #4
	bl Battle_WaitMode0
	ldr r3, .L_0200c2a0
	mov r0, r8
	strh r3, [r0]
	movs r0, #45
	bl Battle_WaitMode0
	ldr r0, .L_0200c2a4
	bl Scheduler_RemoveCallbackFar
	b .L_0200c2a8
.L_0200c294:
	.4byte 0x00000d00
.L_0200c298:
	.4byte 0x00000e00
.L_0200c29c:
	.4byte 0x00000f00
.L_0200c2a0:
	.4byte 0x00001000
.L_0200c2a4:
	.4byte Func_02003c50
.L_0200c2a8:
	bl Func_02004ac8
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
.L_0200c2c6:
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
	.section .text.x0200c2e0,"ax",%progbits
	.global Func_020042e0
	.thumb_func
Func_020042e0:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r0, #128
	lsls r0, r0, #7
	bl Runtime_BumpAllocateAlternatePool
	adds r7, r0, #0
	adds r1, r7, #0
	ldr r0, .L_0200c4d4
	bl Func_02004aa8
	movs r1, #128
	lsls r1, r1, #6
	adds r1, r1, r7
	ldr r0, .L_0200c4d8
	mov r8, r1
	bl Func_02004aa8
	ldr r6, .L_0200c4dc
	ldr r5, .L_0200c4e0
	ldrh r3, [r5]
	adds r1, r3, #0
	strh r5, [r5]
	ldrh r2, [r6]
	cmp r2, #31
	bgt .L_0200c334
	lsls r3, r2, #1
	adds r3, r3, r2
	adds r2, #1
	lsls r3, r3, #2
	strh r2, [r6]
	ldr r2, .L_0200c4e4
	adds r3, r3, r6
	adds r3, #4
	stmia r3!, {r2}
	ldr r2, .L_0200c4e8
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #24
	adds r2, #16
	str r2, [r3]
.L_0200c334:
	strh r1, [r5]
	ldrh r3, [r5]
	adds r1, r3, #0
	strh r5, [r5]
	ldrh r2, [r6]
	cmp r2, #31
	bgt .L_0200c35c
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	adds r3, r3, r6
	adds r3, #4
	adds r2, #1
	stmia r3!, {r7}
	strh r2, [r6]
	movs r2, #192
	lsls r2, r2, #19
	stmia r3!, {r2}
	ldr r2, .L_0200c4ec
	str r2, [r3]
.L_0200c35c:
	strh r1, [r5]
	ldrh r3, [r5]
	adds r1, r3, #0
	strh r5, [r5]
	ldrh r2, [r6]
	cmp r2, #31
	bgt .L_0200c384
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	adds r2, #1
	adds r3, r3, r6
	adds r3, #4
	strh r2, [r6]
	mov r2, r8
	stmia r3!, {r2}
	ldr r2, .L_0200c4f0
	stmia r3!, {r2}
	ldr r2, .L_0200c4f4
	str r2, [r3]
.L_0200c384:
	strh r1, [r5]
	movs r0, #128
	movs r1, #252
	lsls r0, r0, #19
	lsls r1, r1, #6
	adds r0, #80
	adds r1, #65
	bl QueueIoWriteDelay2
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #19
	lsls r1, r1, #5
	adds r0, #82
	adds r1, #16
	bl QueueIoWriteDelay2
	ldr r2, .L_0200c4f8
	movs r3, #0
	strh r3, [r2]
	movs r3, #176
	lsls r3, r3, #1
	strh r3, [r2, #2]
	ldrh r3, [r5]
	adds r1, r3, #0
	strh r5, [r5]
	ldrh r2, [r6]
	cmp r2, #31
	bgt .L_0200c3e0
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	adds r2, #1
	adds r3, r3, r6
	strh r2, [r6]
	movs r2, #194
	adds r3, #4
	lsls r2, r2, #8
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #8
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #10
	str r2, [r3]
.L_0200c3e0:
	strh r1, [r5]
	movs r7, #180
	movs r6, #153
	lsls r7, r7, #6
	lsls r6, r6, #1
	adds r7, #96
	adds r6, #255
	movs r5, #95
.L_0200c3f0:
	lsls r3, r6, #4
	cmp r3, #0
	bge .L_0200c3f8
	adds r3, #127
.L_0200c3f8:
	asrs r2, r3, #7
	negs r3, r7
	cmp r3, #0
	bge .L_0200c402
	adds r3, #127
.L_0200c402:
	movs r1, #176
	lsls r1, r1, #1
	asrs r3, r3, #7
	adds r3, r3, r1
	ldr r1, .L_0200c4f8
	movs r0, #1
	mov r8, r1
	strh r2, [r1]
	mov r2, r8
	strh r3, [r2, #2]
	bl WaitFrames
	movs r3, #176
	lsls r3, r3, #1
	subs r5, #1
	adds r7, r7, r3
	adds r6, #17
	cmp r5, #0
	bge .L_0200c3f0
	movs r0, #164
	movs r5, #192
	bl Func_02004e30
	lsls r5, r5, #18
	movs r0, #120
	bl WaitFrames
	ldr r3, [r5, #108]
	movs r1, #214
	lsls r1, r1, #1
	movs r2, #128
	movs r0, #254
	adds r3, r3, r1
	lsls r2, r2, #1
	lsls r0, r0, #7
	str r2, [r3]
	movs r1, #0
	adds r0, #255
	bl Func_02004d78
	movs r0, #1
	bl Func_02004d88
	movs r0, #128
	lsls r0, r0, #19
	movs r1, #128
	lsls r1, r1, #5
	adds r0, #82
	bl QueueIoWriteDelay2
	movs r0, #1
	bl WaitFrames
	movs r1, #0
	movs r0, #0
	bl Func_02004d78
	movs r0, #60
	bl Func_02004d88
	movs r0, #78
	bl Func_02004e30
	movs r0, #60
	bl WaitFrames
	movs r0, #195
	lsls r0, r0, #1
	bl Func_02004e30
	movs r0, #120
	bl WaitFrames
	ldr r2, [r5, #24]
	movs r3, #1
	strh r3, [r2, #4]
	movs r0, #1
	bl WaitFrames
	bl Func_02004ad8
	bl Func_02004ac8
	ldr r3, .L_0200c4fc
	movs r1, #147
	lsls r1, r1, #1
	adds r1, #255
	adds r2, r3, r1
	ldrb r0, [r2]
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #38
	adds r3, r3, r2
	ldrb r1, [r3]
	bl Func_02004bb8
	movs r6, #0
	mov r3, r8
	mov r1, r8
	strh r6, [r3]
	strh r6, [r1, #2]
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200c4d4:
	.4byte Data_020057d0
.L_0200c4d8:
	.4byte Data_0200556e
.L_0200c4dc:
	.4byte Data_020038e0
.L_0200c4e0:
	.4byte 0x04000208
.L_0200c4e4:
	.4byte Data_0200554e
.L_0200c4e8:
	.4byte 0x050001c0
.L_0200c4ec:
	.4byte 0x84000400
.L_0200c4f0:
	.4byte 0x06001000
.L_0200c4f4:
	.4byte 0x84000800
.L_0200c4f8:
	.4byte Data_03001120
.L_0200c4fc:
	.4byte gPartyState
	.section .text.x0200c500,"ax",%progbits
	.global Func_02004500
	.thumb_func
Func_02004500:
	push {r5, lr}
	ldr r3, .L_0200c534
	movs r2, #2
	ldr r3, [r3]
	adds r5, r0, #0
	ands r3, r2
	cmp r3, #0
	beq .L_0200c518
	movs r1, #7
	bl Animation_ApplyChildValues
	b .L_0200c520
.L_0200c518:
	adds r0, r5, #0
	movs r1, #0
	bl Animation_ApplyChildValues
.L_0200c520:
	ldr r3, .L_0200c534
	movs r2, #15
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	bne .L_0200c532
	adds r0, r5, #0
	bl Func_02004634
.L_0200c532:
	pop {r5, pc}
.L_0200c534:
	.4byte Data_0300122c
	.section .text.x0200c538,"ax",%progbits
	.global Func_02004538
	.thumb_func
Func_02004538:
	push {r5, r6, lr}
	ldr r6, .L_0200c56c
	adds r5, r0, #0
	ldr r0, [r6]
	movs r3, #1
	ands r3, r0
	cmp r3, #0
	beq .L_0200c558
	movs r1, #6
	lsrs r0, r0, #1
	bl Engine_MathModulo
	adds r1, r0, #0
	adds r0, r5, #0
	bl Animation_ApplyChildValues
.L_0200c558:
	ldr r3, [r6]
	movs r2, #15
	ands r3, r2
	cmp r3, #0
	bne .L_0200c568
	adds r0, r5, #0
	bl Func_02004634
.L_0200c568:
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200c56c:
	.4byte Data_0300122c
	.section .text.x0200c570,"ax",%progbits
	.global Func_02004570
	.thumb_func
Func_02004570:
	push {r5, lr}
	ldr r3, .L_0200c594
	adds r5, r0, #0
	ldr r0, [r3]
	movs r3, #1
	ands r3, r0
	cmp r3, #0
	beq .L_0200c590
	movs r1, #6
	lsrs r0, r0, #1
	bl Engine_MathModulo
	adds r1, r0, #0
	adds r0, r5, #0
	bl Animation_ApplyChildValues
.L_0200c590:
	pop {r5, pc}
	.2byte 0x0000
.L_0200c594:
	.4byte Data_0300122c
	.section .text.x0200c598,"ax",%progbits
	.global Func_02004598
	.thumb_func
Func_02004598:
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
	ble .L_0200c5b8
	adds r0, r5, #0
	bl Func_02004b38
	b .L_0200c5e2
.L_0200c5b8:
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
.L_0200c5e2:
	pop {r5, r6, pc}
	.section .text.x0200c5e4,"ax",%progbits
	.global Func_020045e4
	.thumb_func
Func_020045e4:
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
	ble .L_0200c604
	adds r0, r5, #0
	bl Func_02004b38
	b .L_0200c630
.L_0200c604:
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
.L_0200c630:
	pop {r5, r6, pc}
	.2byte 0x0000
	.section .text.x0200c634,"ax",%progbits
	.global Func_02004634
	.thumb_func
Func_02004634:
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
.L_0200c64e:
	movs r0, #16
	ldr r1, [r6, #8]
	ldr r3, [r6, #16]
	ldr r2, [r6, #12]
	adds r0, #255
	bl Func_02004b30
	mov r8, sp
	lsls r3, r7, #2
	mov r1, r8
	str r0, [r3, r1]
	cmp r0, #0
	beq .L_0200c6f8
	ldr r3, [r6, #20]
	ldr r5, [r0, #80]
	str r3, [r0, #20]
	ldr r1, .L_0200c688
	adds r3, r0, #0
	adds r3, #85
	movs r2, #0
	strb r2, [r3]
	adds r3, #15
	strh r2, [r3]
	mov r10, r1
	str r6, [r0, #104]
	cmp r5, #0
	beq .L_0200c6f8
	b .L_0200c68c
	.2byte 0x0000
.L_0200c688:
	.4byte 0x00000000
.L_0200c68c:
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
	ldr r1, .L_0200c6e8
	orrs r3, r2
	strb r3, [r5, #17]
	ldrb r3, [r5, #16]
	ldr r2, .L_0200c6ec
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
	b .L_0200c6f0
	.2byte 0x0000
.L_0200c6e8:
	.4byte 0xfffffc00
.L_0200c6ec:
	.4byte ResourceTableEntries
.L_0200c6f0:
	ldr r3, [r5, #40]
	strb r2, [r5, #7]
	mov r2, r10
	strb r2, [r3, #22]
.L_0200c6f8:
	adds r7, #1
	cmp r7, #1
	ble .L_0200c64e
	.global Data_020046fe
Data_020046fe:
	ldr r2, [sp, #0]
	ldr r3, .L_0200c73c
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
	ldr r3, .L_0200c740
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
.L_0200c73c:
	.4byte Func_020045e4
.L_0200c740:
	.4byte Func_02004598
	.section .text.x0200c744,"ax",%progbits
	.global Func_02004744
	.thumb_func
Func_02004744:
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
	beq .L_0200c75e
	movs r7, #100
.L_0200c75e:
	cmp r6, #15
	bgt .L_0200c770
	ldr r3, .L_0200c8b0
	movs r2, #15
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #1
	bhi .L_0200c770
	movs r7, #200
.L_0200c770:
	cmp r6, #7
	bgt .L_0200c782
	ldr r3, .L_0200c8b0
	movs r2, #7
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #1
	bhi .L_0200c782
	movs r7, #200
.L_0200c782:
	ldr r1, .L_0200c8b4
	adds r2, r7, r6
	movs r0, #0
	ldrsh r3, [r1, r0]
	cmp r2, r3
	bne .L_0200c790
	b .L_0200c8ae
.L_0200c790:
	ldr r5, .L_0200c8b8
	strh r2, [r1]
	movs r1, #128
	ldr r3, .L_0200c8bc
	adds r0, r5, #0
	lsls r1, r1, #1
	mov lr, r3
	.2byte 0xf800
	ldr r3, .L_0200c8c0
	cmp r7, #0
	bne .L_0200c7b2
	ldr r1, .L_0200c8c4
	adds r0, r5, #0
	movs r2, #128
	mov lr, r3
	.2byte 0xf800
	b .L_0200c89e
.L_0200c7b2:
	cmp r7, #100
	bne .L_0200c7c2
	ldr r1, .L_0200c8c8
	adds r0, r5, #0
	movs r2, #128
	mov lr, r3
	.2byte 0xf800
	b .L_0200c7cc
.L_0200c7c2:
	ldr r1, .L_0200c8c4
	adds r0, r5, #0
	movs r2, #128
	mov lr, r3
	.2byte 0xf800
.L_0200c7cc:
	movs r2, #0
	adds r5, r6, #0
	mov r12, r2
	cmp r5, #7
	ble .L_0200c7e8
	ldr r3, .L_0200c8c0
	ldr r0, .L_0200c8b8
	ldr r1, .L_0200c8cc
	movs r2, #32
	mov lr, r3
	.2byte 0xf800
	movs r3, #8
	mov r12, r3
	subs r5, #8
.L_0200c7e8:
	cmp r6, #15
	ble .L_0200c800
	ldr r0, .L_0200c8d0
	ldr r1, .L_0200c8d4
	ldr r3, .L_0200c8c0
	movs r2, #32
	mov lr, r3
	.2byte 0xf800
	adds r5, r6, #0
	movs r0, #16
	mov r12, r0
	subs r5, #16
.L_0200c800:
	cmp r6, #23
	ble .L_0200c818
	movs r2, #32
	ldr r0, .L_0200c8d8
	ldr r1, .L_0200c8dc
	ldr r3, .L_0200c8c0
	mov lr, r3
	.2byte 0xf800
	adds r5, r6, #0
	movs r2, #24
	mov r12, r2
	subs r5, #24
.L_0200c818:
	cmp r6, #30
	ble .L_0200c82e
	ldr r3, .L_0200c8c0
	ldr r0, .L_0200c8e0
	ldr r1, .L_0200c8e4
	movs r2, #32
	mov lr, r3
	.2byte 0xf800
	movs r3, #0
	mov r12, r3
	movs r5, #0
.L_0200c82e:
	cmp r5, #0
	beq .L_0200c89e
	movs r7, #0
	cmp r7, r5
	bge .L_0200c89e
.L_0200c838:
	mov r3, r12
	cmp r3, #0
	bge .L_0200c840
	adds r3, #7
.L_0200c840:
	asrs r3, r3, #3
	lsls r1, r3, #5
	lsrs r3, r7, #31
	adds r3, r7, r3
	asrs r0, r3, #1
	ldr r2, .L_0200c8e8
	ldr r3, .L_0200c8ec
	adds r2, r1, r2
	adds r3, r1, r3
	adds r6, r2, r0
	adds r0, r3, r0
	movs r3, #1
	ands r3, r7
	movs r4, #2
	cmp r3, #0
	bne .L_0200c87e
	movs r4, #1
.L_0200c862:
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
	ble .L_0200c862
	b .L_0200c898
.L_0200c87e:
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
	bge .L_0200c87e
.L_0200c898:
	adds r7, #1
	cmp r7, r5
	blt .L_0200c838
.L_0200c89e:
	ldr r3, .L_0200c8f0
	movs r1, #128
	movs r2, #0
	ldrsh r0, [r3, r2]
	lsls r1, r1, #1
	ldr r2, .L_0200c8b8
	bl VramBlock_LoadCached
.L_0200c8ae:
	pop {r5, r6, r7, pc}
.L_0200c8b0:
	.4byte Data_0300122c
.L_0200c8b4:
	.4byte gOverlayArea + 0x6e0c
.L_0200c8b8:
	.4byte gOverlayArea + 0x6e20
.L_0200c8bc:
	.4byte IwramClearWords
.L_0200c8c0:
	.4byte IwramCopyWords
.L_0200c8c4:
	.4byte gOverlayArea + 0x7020
.L_0200c8c8:
	.4byte gOverlayArea + 0x6fa0
.L_0200c8cc:
	.4byte gOverlayArea + 0x6f20
.L_0200c8d0:
	.4byte gOverlayArea + 0x6e40
.L_0200c8d4:
	.4byte gOverlayArea + 0x6f40
.L_0200c8d8:
	.4byte gOverlayArea + 0x6e60
.L_0200c8dc:
	.4byte gOverlayArea + 0x6f60
.L_0200c8e0:
	.4byte gOverlayArea + 0x6e80
.L_0200c8e4:
	.4byte gOverlayArea + 0x6f80
.L_0200c8e8:
	.4byte gOverlayArea + 0x6f24
.L_0200c8ec:
	.4byte gOverlayArea + 0x6e24
.L_0200c8f0:
	.4byte gOverlayArea + 0x6e0e
	.section .text.x0200c8f4,"ax",%progbits
	.global Func_020048f4
	.thumb_func
Func_020048f4:
	push {r5, r6, r7, lr}
	ldr r0, .L_0200c990
	sub sp, #16
	str r0, [sp, #0]
	ldr r3, .L_0200c994
	ldr r2, .L_0200c998
	movs r1, #0
	ldrsh r3, [r3, r1]
	lsls r3, r3, #2
	adds r3, r3, r2
	ldrh r3, [r3, #2]
	lsrs r6, r3, #5
	bl Func_02004e10
	bl Object_GetById
	ldr r3, .L_0200c99c
	movs r2, #3
	ldr r3, [r3]
	adds r7, r0, #0
	ands r3, r2
	cmp r3, #0
	beq .L_0200c950
	movs r0, #131
	lsls r0, r0, #1
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200c950
	ldr r3, [r7, #12]
	movs r2, #128
	lsls r2, r2, #11
	cmp r3, r2
	ble .L_0200c950
	ldr r5, .L_0200c9a0
	ldrh r3, [r5]
	subs r3, #1
	strh r3, [r5]
	lsls r3, r3, #16
	cmp r3, #0
	bgt .L_0200c950
	movs r0, #1
	bl Func_02004c08
	ldr r3, .L_0200c98c
	strh r3, [r5]
.L_0200c950:
	bl Func_02004c10
	bl Func_02004744
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
	ldr r5, .L_0200c9a4
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
	b .L_0200c9a8
	.2byte 0x0000
.L_0200c98c:
	.4byte 0x0000000a
.L_0200c990:
	.4byte gOverlayArea + 0x6e14
.L_0200c994:
	.4byte gOverlayArea + 0x6e0e
.L_0200c998:
	.4byte ResourceTableEntries
.L_0200c99c:
	.4byte gInput
.L_0200c9a0:
	.4byte gOverlayArea + 0x6e10
.L_0200c9a4:
	.4byte 0x80004000
.L_0200c9a8:
	str r1, [sp, #0]
	str r6, [r0]
	movs r1, #255
	ldr r0, .L_0200c9b8
	bl Func_02004ad0
	add sp, #16
	pop {r5, r6, r7, pc}
.L_0200c9b8:
	.4byte gOverlayArea + 0x6e14
	.section .text.x0200c9bc,"ax",%progbits
	.global Func_020049bc
	.thumb_func
Func_020049bc:
	push {r5, lr}
	ldr r1, .L_0200ca00
	ldr r0, .L_0200ca04
	bl Func_02004aa8
	ldr r5, .L_0200ca08
	bl Resource_FindFreeEntry
	movs r1, #128
	strh r0, [r5]
	lsls r0, r0, #16
	lsls r1, r1, #1
	movs r2, #0
	asrs r0, r0, #16
	bl VramBlock_LoadCached
	ldr r2, .L_0200ca0c
	ldr r3, .L_0200c9f8
	movs r1, #128
	strh r3, [r2]
	ldr r2, .L_0200ca10
	ldr r3, .L_0200c9fc
	lsls r1, r1, #3
	strh r3, [r2]
	adds r1, #118
	ldr r0, .L_0200ca14
	bl Scheduler_AddOrUpdateCallback
	b .L_0200ca18
	.2byte 0x0000
.L_0200c9f8:
	.4byte 0xffffffff
.L_0200c9fc:
	.4byte 0x0000000a
.L_0200ca00:
	.4byte gOverlayArea + 0x6f20
.L_0200ca04:
	.4byte Data_02005920
.L_0200ca08:
	.4byte gOverlayArea + 0x6e0e
.L_0200ca0c:
	.4byte gOverlayArea + 0x6e0c
.L_0200ca10:
	.4byte gOverlayArea + 0x6e10
.L_0200ca14:
	.4byte Func_020048f4
.L_0200ca18:
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x0200ca1c,"ax",%progbits
	.global Func_02004a1c
	.thumb_func
Func_02004a1c:
	push {lr}
	ldr r1, .L_0200ca44
	ldr r0, .L_0200ca48
	bl Func_02004aa8
	ldr r3, .L_0200ca4c
	movs r2, #0
	ldrsh r0, [r3, r2]
	bl Resource_ResetEntry
	ldr r2, .L_0200ca50
	ldr r3, .L_0200ca40
	ldr r0, .L_0200ca54
	strh r3, [r2]
	bl Scheduler_RemoveCallbackFar
	pop {pc}
	.2byte 0x0000
.L_0200ca40:
	.4byte 0xffffffff
.L_0200ca44:
	.4byte gOverlayArea + 0x6f20
.L_0200ca48:
	.4byte Data_02005920
.L_0200ca4c:
	.4byte gOverlayArea + 0x6e0e
.L_0200ca50:
	.4byte gOverlayArea + 0x6e0c
.L_0200ca54:
	.4byte Func_020048f4
	.section .rodata.x0200ce38,"a",%progbits
	.global Data_02004e38
Data_02004e38:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000030
	.4byte 0x23010000
	.4byte 0x16cd0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00000011
	.global Data_02004e58
Data_02004e58:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000030
	.4byte 0x22f10000
	.4byte 0x16dd0000
	.4byte 0x00000001
	.4byte 0x00000030
	.4byte 0x23010000
	.4byte 0x16cd0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00000011
	.global Data_02004e88
Data_02004e88:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000030
	.4byte 0x22f10000
	.4byte 0x16dd0000
	.4byte 0x00000001
	.4byte 0x00000030
	.4byte 0x23010000
	.4byte 0x16cd0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00000011
	.global Data_02004eb8
Data_02004eb8:
	.4byte 0x00000005
	.4byte 0x00180000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000000a
	.4byte 0x00000000
	.4byte 0x00000016
	.4byte 0x00000023
	.4byte 0x00000001
	.4byte 0x00000005
	.4byte 0x00180000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000005
	.4byte 0x00180000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.global Data_02004f10
Data_02004f10:
	.4byte 0x00000017
	.4byte 0x00000005
	.4byte 0xffffe667
	.4byte 0x00000017
	.4byte 0x00000007
	.4byte 0x00001999
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00000e00
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffffe80
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000030
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.global Data_02004f70
Data_02004f70:
	.4byte 0x0000002e
	.4byte Func_02003418
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000078
	.4byte 0x00000000
	.4byte 0x00000026
	.global Data_02004f90
Data_02004f90:
	.4byte 0x0000002e
	.4byte Func_020039a8
	.4byte 0x00000026
	.global Data_02004f9c
Data_02004f9c:
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
	.global Data_02004fc8
Data_02004fc8:
	.4byte 0x00000000
	.4byte 0x00000009
	.4byte 0x00000000
	.4byte 0x00000006
	.global Data_02004fd8
Data_02004fd8:
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
	.global Data_02005058
Data_02005058:
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
	.4byte Data_02019600 + 0x1ce0
	.4byte 0xb0e4b1e4
	.4byte 0x09254000
	.4byte 0xb2e8b1e8
	.4byte 0xb1ec0201
	.4byte 0x00840a01
	.4byte 0xa1e825c0
	.4byte Data_02019600 + 0xce8
	.4byte 0x01cfa1ec
	.4byte 0x2740100a
	.4byte Data_02018000 + 0x12e8
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
	.4byte Data_02019600 + 0x27e8
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
	.4byte Data_02018000 + 0xf7f
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
	.4byte Data_02019600 + 0x14e8
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
	.4byte Data_02018000 + 0x13e0
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
	.4byte Sound_Wave55 + 0x39fc
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
	.4byte Sound_Wave56 + 0x2664
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
	.4byte gOverlayArea + 0x7f02
	.4byte 0xff0200ff
	.4byte 0x80100200
	.2byte 0x0000
	.global Data_02005312
Data_02005312:
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
	.global Data_0200554e
Data_0200554e:
	.2byte 0x01a0
	.4byte Resource_Data012 + 0x120000
	.4byte 0x18c61084
	.4byte 0x294a2108
	.4byte 0x3def318c
	.4byte 0x4e734631
	.4byte 0x63185ad6
	.4byte 0x77bd6b5a
	.2byte 0x7fff
	.global Data_0200556e
Data_0200556e:
	.2byte 0x0001
	.4byte 0xe009e008
	.4byte 0xe00be00a
	.4byte 0x0de00c00
	.4byte 0x0fe00ee0
	.4byte 0xe01000e0
	.4byte 0xe012e011
	.4byte 0x1410e013
	.4byte 0x040015e0
	.4byte 0x29e02814
	.4byte 0xe02a00e0
	.4byte 0xe02ce02b
	.4byte 0x2e00e02d
	.4byte 0x30e02fe0
	.4byte 0x01e031e0
	.4byte 0xe033e032
	.4byte 0x0035e034
	.4byte 0x06081404
	.4byte 0x00e007e0
	.4byte 0xe0262b84
	.4byte 0x84008827
	.4byte 0x05e0042c
	.4byte 0x242c8400
	.4byte 0x008825e0
	.4byte 0xe0022c84
	.4byte 0x2c840003
	.4byte 0xbd23e022
	.4byte 0x002c8400
	.4byte 0x84000202
	.4byte 0x0040032b
	.4byte 0x04002b84
	.4byte 0x00016202
	.4byte 0x3e032884
	.4byte 0x0021e020
	.4byte 0xff002884
	.4byte 0x8400020a
	.4byte 0x00400b23
	.4byte 0x400b2384
	.4byte 0x0b230810
	.4byte 0x2308103e
	.4byte 0x00800fff
	.4byte 0x800f1f84
	.4byte 0x0d1f8400
	.4byte 0x21081080
	.4byte 0x00033e00
	.4byte 0x00ff1b84
	.4byte 0x84000580
	.4byte 0x073e0019
	.4byte 0x00178400
	.4byte 0x84000980
	.4byte 0x0b3e0015
	.4byte 0xff138400
	.4byte 0x000f8000
	.4byte 0x80000f84
	.4byte 0x0f84000f
	.4byte 0x100d8000
	.4byte 0x3e001108
	.4byte 0x0b840013
	.4byte 0x158000ff
	.4byte 0x00098400
	.4byte 0x8400173e
	.4byte 0x1b800007
	.4byte 0x00038400
	.4byte 0x84001b80
	.4byte 0xe870ff03
	.4byte 0x07040017
	.4byte 0x0017e870
	.4byte 0x80000704
	.4byte 0xff8000ff
	.4byte 0x00ff8000
	.4byte 0x00ffff80
	.4byte 0x6cd0af80
	.4byte 0x0304001b
	.4byte 0x001b6cd0
	.4byte 0xf0d00304
	.4byte 0xd0040f1f
	.4byte 0x0fff1ff0
	.4byte 0x2374e004
	.4byte 0x74e0040b
	.4byte 0xe0040b23
	.4byte 0x040727f8
	.4byte 0x6f1df0d0
	.4byte 0x00140014
	.4byte 0x341df0d0
	.4byte 0xd0001400
	.4byte 0x80001df0
	.4byte 0x19808001
	.4byte 0x0528d0ff
	.4byte 0x8b234000
	.4byte 0x23400084
	.4byte 0x4000840b
	.4byte 0x00840727
	.4byte 0x03ff2b40
	.4byte 0x27400084
	.4byte 0x40004407
	.4byte 0xff02002b
	.4byte 0x00ff0200
	.4byte gOverlayArea + 0x7f02
	.4byte Data_020046fe + 0x1
	.4byte 0xe30200ff
	.4byte 0x8009e008
	.4byte 0x80801a84
	.4byte 0xe03f2801
	.4byte 0x1a848029
	.4byte 0x83018080
	.4byte 0x19840044
	.4byte 0x83018000
	.4byte 0x8400ff44
	.4byte 0x01800019
	.4byte 0x8400c887
	.4byte 0x0d420f09
	.4byte 0x10c88780
	.4byte 0x09ff1d08
	.4byte 0x104c9b04
	.4byte 0x04051d8c
	.4byte 0x84004c9b
	.4byte 0x00800323
	.4byte 0x346f2984
	.4byte 0x84008004
	.4byte 0x54a01429
	.4byte 0x1b102004
	.4byte 0x200354a0
	.4byte 0x3fff1b10
	.4byte 0x1f102010
	.4byte 0x8400400f
	.4byte 0x0b5cb01f
	.4byte 0xb0130810
	.4byte 0x84000b5c
	.4byte 0x8000ff13
	.4byte 0x1b840003
	.4byte 0x00038000
	.4byte 0x64c01b84
	.4byte 0x0b840013
	.4byte 0x001364c0
	.4byte 0x00ff0b84
	.4byte 0x84000b80
	.4byte 0x0b800013
	.4byte 0xc0138400
	.4byte 0x84000fe4
	.4byte 0x0f60c00f
	.4byte 0xff0f8400
	.4byte 0x0f1ff0d0
	.4byte 0x1ff0d084
	.4byte 0xac60840f
	.4byte 0x0384001b
	.4byte 0x001b4000
	.4byte 0xe0ff0384
	.4byte 0x08101b70
	.4byte 0x1becd003
	.4byte 0x70030810
	.4byte 0x840b23b4
	.4byte 0x0b234000
	.4byte 0x00008084
	.global Data_020057d0
Data_020057d0:
	.4byte 0xae7c2300
	.4byte 0x4a2290c1
	.4byte 0xa3e4588a
	.4byte 0x94108bd8
	.4byte 0xa01be1d1
	.4byte 0x58a96041
	.4byte 0x8671c264
	.4byte 0x338e90e0
	.4byte 0x86587465
	.4byte 0xc48a1d06
	.4byte 0x22510742
	.4byte 0x523a5582
	.4byte 0x1962521d
	.4byte 0x1228741a
	.4byte 0xab441d0b
	.4byte 0xc66b2d56
	.4byte 0x2a59aab1
	.4byte 0xa0b0ca45
	.4byte 0x10742448
	.4byte 0xcd668d99
	.4byte 0x68e3bdde
	.4byte 0x474883a7
	.4byte 0x2810343b
	.4byte 0x162450e5
	.4byte 0x46230fa4
	.4byte 0x883b0044
	.4byte 0x3c748838
	.4byte 0x60990344
	.4byte 0x99e62287
	.4byte 0xea623933
	.4byte 0x5311d220
	.4byte 0x02c40b87
	.4byte 0xb53a5816
	.4byte 0xb004556a
	.4byte 0x48838883
	.4byte 0x809154c7
	.4byte 0xc2c07605
	.4byte 0x0977bacc
	.4byte Monster_OrcLordSprites + 0xebe
	.4byte 0xe220ec05
	.4byte 0x580b1120
	.4byte 0x733183a0
	.4byte 0x3acb8e66
	.4byte 0xd65c7488
	.4byte 0x320833e1
	.4byte 0x1bbddee7
	.4byte 0xe220ec01
	.4byte 0x5b91d220
	.4byte 0x90446647
	.4byte 0xef6e160b
	.4byte 0x0edd9dde
	.4byte 0x1d220e22
	.4byte 0x16ec75db
	.4byte 0xa160580b
	.4byte 0x0b10234f
	.4byte 0x7fd16058
	.4byte 0x75fdee82
	.4byte 0xb220e228
	.4byte 0x3f7f4751
	.4byte 0x59f7a807
	.4byte 0x0e012204
	.4byte 0x3ee471f0
	.4byte 0xcf8020c1
	.4byte 0x07f9f03f
	.4byte 0x07c0a33e
	.4byte 0xbb9f7beb
	.4byte 0x91d2a5ff
	.4byte 0x7fe2c7c8
	.4byte 0xfe89963a
	.4byte 0x829f3f30
	.4byte 0xfcf0fa01
	.4byte 0xcf9fd9cf
	.4byte 0x7ec4ffc7
	.4byte 0x06ccfece
	.4byte 0xdaf9ff9c
	.4byte 0xffc24f9f
	.4byte 0x123e7904
	.4byte 0xf851f403
	.4byte 0xf9fd9f9f
	.4byte 0x27cffcec
	.4byte 0x5a3e7f62
	.4byte 0x43f9f3ff
	.4byte 0xf03fcf99
	.4byte 0x003e0521
	.global Data_02005920
Data_02005920:
	.4byte 0x11128100
	.4byte 0x904fffa4
	.4byte 0xd25c1bc8
	.4byte 0x412ef611
	.4byte 0xa7606f00
	.4byte 0x02dfdf7a
	.4byte 0x3fa23ef5
	.4byte 0x3e4ffa81
	.4byte 0xfaad3e81
	.4byte 0xf9bf7e0b
	.4byte 0x904cce26
	.4byte 0x1d8108a4
	.4byte 0xf55f0a3e
	.4byte 0xc0bb5599
	.4byte 0xa7cbe147
	.4byte 0x9f5027f2
	.4byte 0x3b217549
	.4byte 0x52be147c
	.4byte 0x3c48208c
	.4byte 0x0e452098
	.4byte 0x2f851f02
	.4byte 0x0fd423f5
	.4byte 0x47c39f56
	.4byte 0xe54f8be1
	.4byte 0x907ea04f
	.4byte 0x33ea41fa
	.4byte 0xf17c18f9
	.4byte 0x00000001
	.global Data_02005990
Data_02005990:
	.4byte 0xffff0000
	.4byte 0x0000267a
	.4byte 0x40002614
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00002800
	.4byte 0x80001f26
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
	.4byte 0x0000281e
	.4byte 0x40002532
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000b
	.4byte 0x0000281f
	.4byte 0xc00024d1
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
	.4byte 0x000028c8
	.4byte 0x000024b7
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
	.4byte 0x40002601
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
	.4byte 0x0000241c
	.4byte 0x80002538
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0016
	.4byte 0x00002470
	.4byte 0x00002538
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
	.4byte 0x000023ef
	.4byte 0x400020da
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
	.4byte 0x0000327b
	.4byte 0xc00025d1
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
	.4byte 0x00003188
	.4byte 0x80002839
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
	.4byte 0x00003440
	.4byte 0x800017b4
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
	.4byte 0x00002a97
	.4byte 0x4000188d
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0029
	.4byte 0x00002bc1
	.4byte 0x400017d9
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff002a
	.4byte 0x000030ad
	.4byte 0x40001d8e
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
	.4byte 0x000032d2
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
	.4byte 0x0000228f
	.4byte 0x40001215
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
	.4byte 0x00001c6d
	.4byte 0x40001ca2
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
	.4byte 0x00001def
	.4byte 0x4000159f
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
	.4byte 0x40000e78
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
	.4byte 0x00002360
	.4byte 0x400004e2
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff003f
	.4byte 0x00002360
	.4byte 0xc0000497
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
	.4byte 0x000027ff
	.4byte 0x00001ecc
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
	.4byte 0x00001c58
	.4byte 0x40002085
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
	.4byte 0xffff0049
	.4byte 0x000027b8
	.4byte 0x4000259e
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff004a
	.4byte 0x000027b8
	.4byte 0x4000259e
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff004d
	.4byte 0x00002f40
	.4byte 0x00002085
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff004e
	.4byte 0x000038de
	.4byte 0x00002109
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
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020061d0
Data_020061d0:
	.4byte 0x00000002
	.4byte 0x0404d002
	.4byte 0x04102009
	.4byte 0x0420a02a
	.4byte 0x04d0c009
	.4byte 0x04e0509e
	.4byte 0x00104009
	.4byte 0x1020800a
	.4byte 0x00000873
	.4byte 0x1030900a
	.4byte 0x00000873
	.4byte 0x0020800d
	.4byte 0x0030900d
	.4byte 0x00401015
	.4byte 0x00503017
	.4byte 0x00601022
	.4byte 0x00706024
	.4byte 0x0080202b
	.4byte 0x0090b02c
	.4byte 0x00a01035
	.4byte 0x00b02035
	.4byte 0x00c01036
	.4byte 0x00d02036
	.4byte 0x00e06037
	.4byte 0x00f0103d
	.4byte 0x01001052
	.4byte 0x01101043
	.4byte 0x0120204a
	.4byte 0x0130204b
	.4byte 0x0140905f
	.4byte 0x01501066
	.4byte 0x01602066
	.4byte 0x01706069
	.4byte 0x0190106c
	.4byte 0x01a0106b
	.4byte 0x11b01071
	.4byte 0x000008ff
	.4byte 0x11b0106e
	.4byte 0xffffffff
	.4byte 0x01c01083
	.4byte 0x01d01084
	.4byte 0x01e01085
	.4byte 0x01f0a086
	.4byte Data_02001024 + 0x63
	.4byte 0x0210a08b
	.4byte 0x0220b08b
	.4byte 0x0230108f
	.4byte 0x0240109a
	.4byte 0x0250109e
	.4byte 0x026010ab
	.4byte 0x027050ae
	.4byte 0x028090b6
	.4byte 0x029010c3
	.4byte 0x02a010c5
	.4byte 0x02b050c6
	.4byte 0x02c030c6
	.4byte 0x02d010c7
	.4byte 0x02e010ce
	.4byte 0x02f010dd
	.4byte 0x030010d5
	.4byte 0x031010d8
	.4byte 0x032010db
	.4byte 0x033010df
	.4byte 0x034010ea
	.4byte 0x035020eb
	.4byte 0x036010ec
	.4byte 0x037010fb
	.4byte 0x038010f0
	.4byte 0x0390a0f0
	.4byte 0x03a010f2
	.4byte 0x03b0710a
	.4byte 0x03c0110c
	.4byte 0x03d0410c
	.4byte 0x03e0110d
	.4byte 0x03f0310f
	.4byte 0x05201114
	.4byte 0x05301113
	.4byte 0x05401117
	.4byte 0x05501067
	.4byte 0x05602067
	.4byte 0x0570112a
	.4byte 0x05805115
	.4byte 0x05901116
	.4byte 0x000001ff
.L_0200e320:
	.4byte 0x0000002e
	.4byte Func_020002a4
	.4byte 0x00000011
	.global Data_0200632c
Data_0200632c:
	.4byte 0x0000002e
	.4byte Func_0200034c
	.4byte 0x00000011
	.global Data_02006338
Data_02006338:
	.4byte 0xffff0008
	.4byte .L_0200e320
	.4byte 0x29900000
	.4byte 0x00000000
	.4byte 0x23730000
	.4byte 0x00020000
	.4byte 0x003700f3
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0x003800f3
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0x004c00f4
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0x004f00f4
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0x007800f6
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0x003d00f3
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0x006500f5
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff0005
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
	.4byte 0xffff00dd
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0002a000
	.4byte 0xffff009a
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0xffff00dd
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00028000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0002c000
	.4byte 0x1a7600f0
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0002c000
	.4byte 0x1a7600f1
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0002c000
	.4byte 0x1a7600f1
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0002c000
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
	.global Data_02006500
Data_02006500:
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0x07b00000
	.4byte 0x00000000
	.4byte 0x1d400000
	.4byte 0x0002c000
	.4byte 0xffff0005
	.4byte 0x00000001
	.4byte 0x07900000
	.4byte 0x00000000
	.4byte 0x1d300000
	.4byte 0x0002c000
	.4byte 0xffff0039
	.4byte 0x00000001
	.4byte 0x07700000
	.4byte 0x00000000
	.4byte 0x1d400000
	.4byte 0x0002c000
	.4byte 0xffff0008
	.4byte .L_0200e320
	.4byte 0x079a0000
	.4byte 0x00000000
	.4byte 0x1d850000
	.4byte 0x00020000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
.L_0200e578:
	.4byte 0x0000002e
	.4byte Func_02000388
	.4byte 0x00000011
.L_0200e584:
	.4byte 0x0000002e
	.4byte Func_020003a8
	.4byte 0x0000002e
	.4byte Func_020003b4
	.4byte 0x00000011
	.global Data_02006598
Data_02006598:
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0x2af80000
	.4byte 0x00000000
	.4byte 0x1d4c0000
	.4byte 0x00020000
	.4byte 0xffff0005
	.4byte 0x00000001
	.4byte 0x27100000
	.4byte 0x00000000
	.4byte 0x1d4c0000
	.4byte 0x00020000
	.4byte 0xffff0039
	.4byte 0x00000001
	.4byte 0x27100000
	.4byte 0x00000000
	.4byte 0x1f400000
	.4byte 0x00020000
	.4byte 0xffff0006
	.4byte 0x00000001
	.4byte 0x2af80000
	.4byte 0x00000000
	.4byte 0x1f400000
	.4byte 0x00020000
	.4byte 0xffff01b1
	.4byte .L_0200e578
	.4byte 0x27df0000
	.4byte 0x00000000
	.4byte 0x1edc0000
	.4byte 0x00024000
	.4byte 0xffff0115
	.4byte .L_0200e584
	.4byte 0x291c0000
	.4byte 0x00000000
	.4byte 0x1d1a0000
	.4byte 0x00024000
	.4byte 0xffff0115
	.4byte .L_0200e584
	.4byte 0x29240000
	.4byte 0x00000000
	.4byte 0x1d380000
	.4byte 0x01024000
	.4byte 0xffff0115
	.4byte .L_0200e584
	.4byte 0x292c0000
	.4byte 0x00000000
	.4byte 0x1d560000
	.4byte 0x01024000
	.4byte 0xffff0115
	.4byte .L_0200e584
	.4byte 0x29340000
	.4byte 0x00000000
	.4byte 0x1d740000
	.4byte 0x01024000
	.4byte 0xffff0115
	.4byte .L_0200e584
	.4byte 0x293c0000
	.4byte 0x00000000
	.4byte 0x1d920000
	.4byte 0x01024000
	.4byte 0xffff0115
	.4byte .L_0200e584
	.4byte 0x29440000
	.4byte 0x00000000
	.4byte 0x1db00000
	.4byte 0x01024000
	.4byte 0xffff0115
	.4byte .L_0200e584
	.4byte 0x294c0000
	.4byte 0x00000000
	.4byte 0x1dce0000
	.4byte 0x01024000
	.4byte 0xffff0115
	.4byte .L_0200e584
	.4byte 0x29540000
	.4byte 0x00000000
	.4byte 0x1dec0000
	.4byte 0x01024000
	.4byte 0xffff0008
	.4byte .L_0200e320
	.4byte 0x0dbb0000
	.4byte 0x00000000
	.4byte 0x1dc50000
	.4byte 0x00020000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02006700
Data_02006700:
	.4byte 0xffff0009
	.4byte .L_0200e320
	.4byte 0x29900000
	.4byte 0x00000000
	.4byte 0x23730000
	.4byte 0x00020000
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0x29360000
	.4byte 0x00000000
	.4byte 0x21ea0000
	.4byte 0x00024000
	.4byte 0xffff0005
	.4byte 0x00000001
	.4byte 0x29360000
	.4byte 0x00000000
	.4byte 0x24420000
	.4byte 0x00024000
	.4byte 0xffff0007
	.4byte 0x00000001
	.4byte 0x2a620000
	.4byte 0x00000000
	.4byte 0x21ea0000
	.4byte 0x00024000
	.4byte 0xffff0006
	.4byte 0x00000001
	.4byte 0x2a620000
	.4byte 0x00000000
	.4byte 0x24420000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02006790
Data_02006790:
	.4byte 0xffff01b1
	.4byte .L_0200e578
	.4byte 0x2f400000
	.4byte 0x00000000
	.4byte 0x20850000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020067c0
Data_020067c0:
	.4byte 0x00000006
	.4byte 0xffff00fb
	.4byte Func_02000574
	.4byte 0x00000006
	.4byte 0xffff00fc
	.4byte Func_020006d0
	.4byte 0x00000006
	.4byte 0xffff00fd
	.4byte Func_02000458
	.4byte 0x00000006
	.4byte 0xffff00fe
	.4byte Func_0200050c
	.4byte 0x00000002
	.4byte 0x0038005c
	.4byte Func_02000038
	.4byte 0x00000002
	.4byte 0x004c005d
	.4byte Func_0200009c
	.4byte 0x00000002
	.4byte 0x004f005e
	.4byte Func_02000100
	.4byte 0x00000002
	.4byte 0x0078005f
	.4byte Func_02000164
	.4byte 0x00000002
	.4byte 0x003d0060
	.4byte Func_020001c8
	.4byte 0x00000002
	.4byte 0x00650061
	.4byte Func_0200022c
	.4byte 0x00000003
	.4byte 0xffff0463
	.4byte Func_02000be0
	.4byte 0x00000002
	.4byte 0xffff0064
	.4byte Func_02000db0
	.4byte 0x00000001
	.4byte 0xffff0065
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0066
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0067
	.4byte 0x00000006
	.4byte 0x00000001
	.4byte 0xffff0068
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte Tileset_Set96TilesA + 0x62d
	.4byte 0x00000008
	.4byte 0x00000001
	.4byte 0xffff006a
	.4byte 0x00000009
	.4byte 0x00000001
	.4byte 0xffff006b
	.4byte 0x0000000c
	.4byte 0x00000001
	.4byte 0xffff006c
	.4byte 0x0000000d
	.4byte 0x00000001
	.4byte 0xffff006d
	.4byte 0x0000000e
	.4byte 0x00000001
	.4byte 0xffff006e
	.4byte 0x0000000f
	.4byte 0x00000001
	.4byte 0xffff006f
	.4byte 0x00000010
	.4byte 0x00000001
	.4byte 0xffff0070
	.4byte 0x00000013
	.4byte 0x00000001
	.4byte 0xffff0071
	.4byte 0x00000014
	.4byte 0x00000001
	.4byte 0xffff0072
	.4byte 0x00000015
	.4byte 0x00000001
	.4byte 0xffff0073
	.4byte 0x00000016
	.4byte 0x00000001
	.4byte 0xffff0074
	.4byte 0x00000017
	.4byte 0x00000001
	.4byte 0xffff0075
	.4byte 0x00000019
	.4byte 0x00000001
	.4byte 0xffff0076
	.4byte 0x0000001a
	.4byte 0x00000001
	.4byte 0xffff0077
	.4byte 0x0000001b
	.4byte 0x00000001
	.4byte 0xffff0078
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0xffff0079
	.4byte 0x0000001d
	.4byte 0x00000001
	.4byte 0xffff007a
	.4byte 0x0000001e
	.4byte 0x00000001
	.4byte 0xffff007b
	.4byte 0x0000001f
	.4byte 0x00000001
	.4byte 0xffff007c
	.4byte 0x00000020
	.4byte 0x00000002
	.4byte 0xffff007d
	.4byte Func_02000da4
	.4byte 0x00000001
	.4byte 0xffff007e
	.4byte 0x00000023
	.4byte 0x00000001
	.4byte 0xffff007f
	.4byte 0x00000024
	.4byte 0x00000001
	.4byte 0xffff0080
	.4byte 0x00000025
	.4byte 0x00000001
	.4byte 0xffff0081
	.4byte 0x00000026
	.4byte 0x00000001
	.4byte 0xffff0082
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0xffff0083
	.4byte 0x00000028
	.4byte 0x00000001
	.4byte 0xffff0084
	.4byte 0x00000029
	.4byte 0x00000001
	.4byte 0xffff0085
	.4byte 0x0000002a
	.4byte 0x00000001
	.4byte 0xffff0086
	.4byte 0x0000002b
	.4byte 0x00000001
	.4byte 0xffff0088
	.4byte 0x0000002d
	.4byte 0x00000001
	.4byte 0xffff0089
	.4byte 0x0000002e
	.4byte 0x00000001
	.4byte 0xffff008a
	.4byte 0x0000002f
	.4byte 0x00000001
	.4byte 0xffff008b
	.4byte 0x00000030
	.4byte 0x00000001
	.4byte 0xffff008c
	.4byte 0x00000031
	.4byte 0x00000001
	.4byte 0xffff008d
	.4byte 0x00000032
	.4byte 0x00000001
	.4byte 0xffff008e
	.4byte 0x00000033
	.4byte 0x00000001
	.4byte 0xffff008f
	.4byte 0x00000036
	.4byte 0x00000001
	.4byte 0xffff0090
	.4byte 0x00000037
	.4byte 0x00000001
	.4byte 0xffff0091
	.4byte 0x00000038
	.4byte 0x00000001
	.4byte 0xffff0092
	.4byte 0x00000039
	.4byte 0x00000001
	.4byte 0xffff0093
	.4byte 0x0000003a
	.4byte 0x00000001
	.4byte 0xffff0094
	.4byte 0x0000003b
	.4byte 0x00000002
	.4byte 0xffff0096
	.4byte Func_02000d98
	.4byte 0x00000001
	.4byte 0xffff0097
	.4byte 0x00000052
	.4byte 0x00000001
	.4byte 0xffff0098
	.4byte 0x00000053
	.4byte 0x00000001
	.4byte 0xffff0099
	.4byte 0x00000054
	.4byte 0x00000001
	.4byte 0xffff009a
	.4byte 0x00000054
	.4byte 0x00000001
	.4byte 0xffff009b
	.4byte 0x00000057
	.4byte 0x00000001
	.4byte 0xffff009c
	.4byte 0x00000058
	.4byte 0x00000001
	.4byte 0xffff009d
	.4byte 0x00000059
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000003
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
	.4byte 0xffff0011
	.4byte 0x00000011
	.4byte 0x00000001
	.4byte 0xffff0012
	.4byte 0x00000012
	.4byte 0x00000001
	.4byte 0xffff002a
	.4byte 0x0000002a
	.4byte 0x00000001
	.4byte 0xffff002b
	.4byte 0x0000002b
	.4byte 0x00000001
	.4byte 0xffff002c
	.4byte 0x0000002c
	.4byte 0x00000001
	.4byte 0xffff002d
	.4byte 0x0000002d
	.4byte 0x00000002
	.4byte 0xffff0034
	.4byte Func_02000918
	.4byte 0x00000002
	.4byte 0xffff0035
	.4byte Func_02000994
	.4byte 0x00000002
	.4byte 0x0a780028
	.4byte Func_02003144
	.4byte 0x00000001
	.4byte 0xffff003c
	.4byte 0x0000003c
	.4byte 0x00000001
	.4byte 0xffff003d
	.4byte 0x0000003d
	.4byte 0x00000001
	.4byte 0xffff0055
	.4byte 0x00000055
	.4byte 0x00000001
	.4byte 0xffff0056
	.4byte 0x00000056
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte Func_02000d1c
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02006bc8
Data_02006bc8:
	.4byte 0x00000000
	.4byte 0x00000010
	.4byte 0x00000026
	.global Data_02006bd4
Data_02006bd4:
	.4byte 0x0000002e
	.4byte Func_020035e0
	.4byte 0x00000011
	.4byte 0x00000000
	.global Data_02006be4
Data_02006be4:
	.4byte 0x00000000
