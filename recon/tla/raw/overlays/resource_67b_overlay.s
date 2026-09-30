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
	push {r5, r6, lr}
	adds r4, r0, #0
	adds r5, r1, #0
	adds r6, r2, #0
	adds r0, r3, #0
	adds r2, r5, #0
	adds r1, r4, #0
	adds r3, r6, #0
	bl Func_02002920
	adds r5, r0, #0
	cmp r5, #0
	beq .L_020080ca
	movs r1, #0
	bl Func_02000038
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
	bl Func_020029e8
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
	bl Func_02002920
	adds r5, r0, #0
	cmp r5, #0
	beq .L_0200811e
	movs r1, #1
	bl Func_02000038
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
	bl Func_020029e8
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
	bl Func_02002920
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
	bl Func_02002910
	ldr r2, .L_02008330
	mov r3, r10
	ands r3, r5
	lsls r3, r3, #2
	ldr r1, [r2, r3]
	adds r0, r6, #0
	mov r11, r3
	bl Func_02002918
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
	bl Func_02000038
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
	bl Func_020029e8
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
	bl Func_02000038
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
	bl Func_02002910
	ldr r1, [r7, #28]
	adds r0, r6, #0
	bl Func_02002918
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
	.4byte Data_02002df4
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
	.4byte Data_02002e00
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
	.4byte 0x0000009e
.L_02008370:
	.4byte Data_02002e30
.L_02008374:
	.4byte 0x0000009f
.L_02008378:
	.4byte Data_02002e60
	.section .text.x0200837c,"ax",%progbits
	.global Func_0200037c
	.thumb_func
Func_0200037c:
	ldr r0, .L_02008380
	bx lr
.L_02008380:
	.4byte Data_02002eb0
	.section .text.x02008384,"ax",%progbits
	.global Func_02000384
	.thumb_func
Func_02000384:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r2, #128
	adds r7, r0, #0
	lsls r2, r2, #1
	movs r0, #0
	sub sp, #24
	mov r10, r0
	mov r9, r2
.L_0200839c:
	movs r3, #1
	add r10, r3
	mov r5, r10
	cmp r5, #3
	bgt .L_02008436
	ldr r3, [r7, #8]
	add r0, sp, #12
	str r3, [r0]
	ldr r3, [r7, #12]
	mov r8, r0
	str r3, [r0, #4]
	ldr r3, [r7, #16]
	str r3, [r0, #8]
	bl Random16Far
	movs r1, #128
	ldr r3, .L_02008464
	lsls r1, r1, #12
	mov lr, r3
	.2byte 0xf800
	movs r2, #128
	lsls r2, r2, #11
	adds r6, r0, #0
	adds r6, r6, r2
	bl Random16Far
	adds r5, r0, #0
	bl Random16Far
	ldrh r1, [r7, #6]
	lsrs r5, r5, #2
	adds r1, r1, r5
	lsrs r0, r0, #2
	subs r1, r1, r0
	mov r2, r8
	adds r0, r6, #0
	mov r5, r8
	bl Vector_AddPolarOffsetFar
	ldr r3, [r5]
	adds r1, r3, #0
	cmp r3, #0
	bge .L_020083fa
	movs r0, #255
	lsls r0, r0, #8
	adds r0, #255
	adds r3, r3, r0
.L_020083fa:
	adds r2, r7, #0
	adds r2, #100
	movs r5, #0
	ldrsh r2, [r2, r5]
	asrs r3, r3, #16
	subs r0, r3, r2
	mov r3, r8
	ldr r2, [r3, #8]
	adds r4, r2, #0
	cmp r2, #0
	bge .L_02008418
	movs r5, #255
	lsls r5, r5, #8
	adds r5, #255
	adds r2, r2, r5
.L_02008418:
	adds r3, r7, #0
	adds r3, #102
	movs r5, #0
	ldrsh r3, [r3, r5]
	asrs r2, r2, #16
	subs r2, r2, r3
	adds r3, r0, #0
	muls r3, r0
	adds r0, r2, #0
	muls r0, r2
	adds r2, r0, #0
	adds r3, r3, r2
	cmp r3, r9
	ble .L_0200844a
	b .L_0200839c
.L_02008436:
	ldrh r3, [r7, #6]
	movs r2, #128
	lsls r2, r2, #8
	adds r3, r3, r2
	adds r2, r7, #0
	strh r3, [r7, #6]
	adds r2, #94
	movs r3, #1
	strh r3, [r2]
	b .L_02008456
.L_0200844a:
	mov r3, r8
	ldr r2, [r3, #4]
	adds r0, r7, #0
	adds r3, r4, #0
	bl Func_02002930
.L_02008456:
	movs r0, #0
	add sp, #24
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_02008464:
	.4byte IwramMulQ16
	.section .text.x02008468,"ax",%progbits
	.global Func_02000468
	.thumb_func
Func_02000468:
	ldrh r3, [r0, #6]
	ldr r2, [r0, #80]
	movs r1, #128
	lsls r1, r1, #7
	adds r3, r3, r1
	strh r3, [r2, #18]
	bx lr
	.2byte 0x0000
	.section .text.x02008478,"ax",%progbits
	.global Func_02000478
	.thumb_func
Func_02000478:
	push {r5, lr}
	movs r1, #0
	adds r5, r0, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r5, #48]
	movs r3, #192
	lsls r3, r3, #4
	adds r3, #204
	adds r2, r5, #0
	str r3, [r5, #52]
	adds r2, #89
	movs r3, #0
	strb r3, [r2]
	ldr r3, .L_020084a0
	movs r0, #0
	str r3, [r5, #108]
	pop {r5, pc}
.L_020084a0:
	.4byte Func_02000468
	.section .text.x020084a4,"ax",%progbits
	.global Func_020004a4
	.thumb_func
Func_020004a4:
	push {lr}
	ldr r3, .L_020084cc
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_020084d0
	cmp r2, r3
	bne .L_020084bc
	ldr r0, .L_020084d4
	b .L_020084c8
.L_020084bc:
	ldr r3, .L_020084d8
	cmp r2, r3
	bne .L_020084c6
	ldr r0, .L_020084dc
	b .L_020084c8
.L_020084c6:
	ldr r0, .L_020084e0
.L_020084c8:
	pop {pc}
	.2byte 0x0000
.L_020084cc:
	.4byte gPartyState
.L_020084d0:
	.4byte 0x0000009e
.L_020084d4:
	.4byte Data_02002f04
.L_020084d8:
	.4byte 0x0000009f
.L_020084dc:
	.4byte Data_0200312c
.L_020084e0:
	.4byte Data_02002eec
	.section .text.x020084e4,"ax",%progbits
	.global Func_020004e4
	.thumb_func
Func_020004e4:
	push {r5, lr}
	adds r5, r0, #0
	ldr r1, [r5, #8]
	ldr r2, [r5, #16]
	movs r0, #0
	bl Map_GetTerrainHeight
	ldr r3, [r5, #12]
	cmp r0, r3
	ble .L_02008502
	movs r2, #128
	lsls r2, r2, #10
	adds r3, r3, r2
	str r3, [r5, #12]
	b .L_0200850a
.L_02008502:
	ldr r3, [r5, #16]
	ldr r2, .L_02008528
	adds r3, r3, r2
	str r3, [r5, #16]
.L_0200850a:
	ldr r3, [r5, #12]
	asrs r3, r3, #20
	cmp r3, #23
	bne .L_02008518
	adds r0, r5, #0
	bl Func_02002928
.L_02008518:
	ldr r3, [r5, #16]
	asrs r3, r3, #20
	cmp r3, #48
	bne .L_02008526
	adds r0, r5, #0
	bl Func_02002928
.L_02008526:
	pop {r5, pc}
.L_02008528:
	.4byte 0xfffe0000
	.section .text.x0200852c,"ax",%progbits
	.global Func_0200052c
	.thumb_func
Func_0200052c:
	push {r5, lr}
	adds r5, r0, #0
	bl Random16Far
	movs r3, #128
	lsls r3, r3, #5
	cmp r0, r3
	bcs .L_02008574
	bl Random16Far
	ldr r1, [r5, #8]
	lsls r3, r0, #1
	adds r3, r3, r0
	lsls r3, r3, #4
	adds r1, r1, r3
	ldr r3, .L_02008578
	movs r0, #128
	lsls r0, r0, #2
	adds r1, r1, r3
	ldr r2, [r5, #12]
	ldr r3, [r5, #16]
	adds r0, #162
	bl Func_02002920
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02008574
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	ldr r3, .L_0200857c
	adds r2, r5, #0
	str r3, [r5, #108]
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
.L_02008574:
	pop {r5, pc}
	.2byte 0x0000
.L_02008578:
	.4byte 0xffe80000
.L_0200857c:
	.4byte Func_020004e4
	.section .text.x02008580,"ax",%progbits
	.global Func_02000580
	.thumb_func
Func_02000580:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r0, .L_020085e0
	sub sp, #20
	movs r1, #0
	mov r10, r0
	movs r2, #2
	str r1, [sp, #16]
	add r2, r10
	mov r8, r2
.L_0200859e:
	mov r4, r8
	ldrh r3, [r4, #16]
	ldr r0, .L_020085d4
	mov r6, r8
	ands r0, r3
	bl Func_020028d8
	ldrh r3, [r6, #16]
	ldr r2, .L_020085d8
	adds r4, r0, #0
	ands r3, r2
	cmp r3, #0
	beq .L_020085bc
	movs r3, #1
	eors r4, r3
.L_020085bc:
	mov r1, r8
	movs r0, #18
	ldrsh r3, [r1, r0]
	cmp r3, r4
	beq .L_0200860a
	movs r2, #28
	ldrsh r3, [r1, r2]
	ldr r2, .L_020085dc
	lsls r3, r3, #1
	mov r6, r8
	b .L_020085e4
	.2byte 0x0000
.L_020085d4:
	.4byte 0x00000fff
.L_020085d8:
	.4byte 0x00001000
.L_020085dc:
	.4byte 0x00000000
.L_020085e0:
	.4byte gOverlayArea + 0x3450
.L_020085e4:
	adds r3, #20
	strh r2, [r6, r3]
	movs r2, #0
	movs r0, #28
	ldrsh r3, [r6, r0]
	movs r0, #128
	lsls r3, r3, #1
	adds r3, #24
	strh r4, [r1, r3]
	lsls r0, r0, #9
	ldrh r3, [r1, #28]
	adds r3, #1
	strh r3, [r6, #28]
	lsls r3, r3, #16
	cmp r3, r0
	ble .L_02008606
	strh r2, [r1, #28]
.L_02008606:
	mov r2, r8
	strh r4, [r2, #18]
.L_0200860a:
	movs r4, #24
	str r4, [sp, #8]
	movs r3, #20
	movs r6, #1
	mov r9, r3
	mov r11, r6
.L_02008616:
	mov r0, r8
	str r0, [sp, #12]
	mov r3, r9
	mov r1, r9
	ldrsh r7, [r0, r3]
	movs r4, #14
	ldrsh r3, [r0, r4]
	ldrh r1, [r0, r1]
	lsls r3, r3, #2
	mov r12, r1
	cmp r7, r3
	bge .L_02008706
	mov r6, r10
	ldrh r4, [r6, #8]
	mov r2, r8
	adds r3, r7, #0
	ldrh r0, [r6]
	ldrh r1, [r2]
	mov lr, r4
	ldrh r5, [r6, #10]
	cmp r7, #0
	bge .L_02008644
	adds r3, r7, #3
.L_02008644:
	asrs r7, r3, #2
	ldr r3, [sp, #8]
	mov r6, r8
	ldrsh r4, [r6, r3]
	movs r3, #1
	adds r6, r4, #0
	mov r2, r12
	eors r6, r3
	ands r3, r2
	cmp r3, #0
	beq .L_020086aa
	mov r3, lr
	lsls r2, r3, #16
	lsls r3, r5, #16
	lsls r0, r0, #16
	lsls r5, r4, #1
	lsls r1, r1, #16
	asrs r3, r3, #16
	adds r5, r5, r4
	asrs r2, r2, #16
	adds r3, r3, r7
	asrs r0, r0, #16
	asrs r1, r1, #16
	adds r0, r5, r0
	adds r1, r1, r7
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r2, #3
	movs r3, #1
	bl Func_02002968
	mov r6, r10
	movs r4, #4
	ldrsh r2, [r6, r4]
	movs r0, #6
	ldrsh r1, [r6, r0]
	movs r3, #12
	ldrsh r0, [r6, r3]
	movs r4, #14
	ldrsh r3, [r6, r4]
	adds r5, r5, r2
	adds r3, r3, r7
	str r0, [sp, #0]
	str r3, [sp, #4]
	adds r1, r1, r7
	adds r0, r5, #0
	movs r2, #3
	movs r3, #1
	bl Func_02002958
	b .L_020086f8
.L_020086aa:
	mov r3, lr
	lsls r2, r3, #16
	lsls r3, r5, #16
	lsls r0, r0, #16
	lsls r5, r6, #1
	lsls r1, r1, #16
	asrs r3, r3, #16
	adds r5, r5, r6
	asrs r2, r2, #16
	adds r3, r3, r7
	asrs r0, r0, #16
	asrs r1, r1, #16
	adds r0, r5, r0
	adds r1, r1, r7
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r2, #3
	movs r3, #1
	bl Func_02002968
	mov r6, r10
	movs r4, #4
	ldrsh r2, [r6, r4]
	movs r0, #6
	ldrsh r1, [r6, r0]
	movs r3, #12
	ldrsh r0, [r6, r3]
	movs r4, #14
	ldrsh r3, [r6, r4]
	adds r5, r5, r2
	adds r3, r3, r7
	str r0, [sp, #0]
	str r3, [sp, #4]
	adds r1, r1, r7
	adds r0, r5, #0
	movs r2, #3
	movs r3, #1
	bl Func_02002958
.L_020086f8:
	ldr r6, [sp, #12]
	mov r0, r9
	ldrh r3, [r6, r0]
	adds r1, r6, #0
	adds r3, #1
	mov r2, r9
	strh r3, [r1, r2]
.L_02008706:
	ldr r4, [sp, #8]
	movs r6, #1
	negs r6, r6
	add r11, r6
	movs r3, #2
	adds r4, #2
	mov r0, r11
	add r9, r3
	str r4, [sp, #8]
	cmp r0, #0
	blt .L_0200871e
	b .L_02008616
.L_0200871e:
	ldr r1, [sp, #16]
	movs r2, #32
	adds r1, #1
	str r1, [sp, #16]
	add r8, r2
	add r10, r2
	cmp r1, #4
	bgt .L_02008730
	b .L_0200859e
.L_02008730:
	add sp, #20
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x02008740,"ax",%progbits
	.global Func_02000740
	.thumb_func
Func_02000740:
	push {lr}
	movs r0, #130
	lsls r0, r0, #1
	bl Func_020028d8
	cmp r0, #0
	bne .L_02008832
	ldr r3, .L_02008834
	ldr r3, [r3]
	cmp r3, #149
	bgt .L_02008776
	movs r0, #200
	lsls r0, r0, #2
	bl Func_020028d8
	cmp r0, #0
	beq .L_0200876c
	movs r0, #128
	lsls r0, r0, #2
	bl Func_020028e0
	b .L_0200877e
.L_0200876c:
	movs r0, #128
	lsls r0, r0, #2
	bl Func_020028e8
	b .L_0200877e
.L_02008776:
	movs r0, #128
	lsls r0, r0, #2
	bl Func_020028e0
.L_0200877e:
	ldr r2, .L_02008834
	ldr r3, [r2]
	adds r3, #1
	str r3, [r2]
	cmp r3, #239
	ble .L_0200878e
	movs r3, #0
	str r3, [r2]
.L_0200878e:
	ldr r3, .L_02008838
	ldr r3, [r3]
	cmp r3, #149
	bgt .L_020087bc
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #33
	bl Func_020028d8
	cmp r0, #0
	beq .L_020087b0
	movs r0, #129
	lsls r0, r0, #1
	adds r0, #255
	bl Func_020028e0
	b .L_020087c6
.L_020087b0:
	movs r0, #129
	lsls r0, r0, #1
	adds r0, #255
	bl Func_020028e8
	b .L_020087c6
.L_020087bc:
	movs r0, #129
	lsls r0, r0, #1
	adds r0, #255
	bl Func_020028e0
.L_020087c6:
	ldr r2, .L_02008838
	ldr r3, [r2]
	adds r3, #1
	str r3, [r2]
	cmp r3, #239
	ble .L_020087d6
	movs r3, #0
	str r3, [r2]
.L_020087d6:
	ldr r3, .L_0200883c
	ldr r3, [r3]
	cmp r3, #149
	bgt .L_020087ea
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #2
	bl Func_020028e8
	b .L_020087f4
.L_020087ea:
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #2
	bl Func_020028e0
.L_020087f4:
	ldr r2, .L_0200883c
	ldr r3, [r2]
	adds r3, #1
	str r3, [r2]
	cmp r3, #239
	ble .L_02008804
	movs r3, #0
	str r3, [r2]
.L_02008804:
	ldr r3, .L_02008840
	ldr r3, [r3]
	cmp r3, #149
	bgt .L_02008818
	movs r0, #130
	lsls r0, r0, #1
	adds r0, #255
	bl Func_020028e8
	b .L_02008822
.L_02008818:
	movs r0, #130
	lsls r0, r0, #1
	adds r0, #255
	bl Func_020028e0
.L_02008822:
	ldr r2, .L_02008840
	ldr r3, [r2]
	adds r3, #1
	str r3, [r2]
	cmp r3, #239
	ble .L_02008832
	movs r3, #0
	str r3, [r2]
.L_02008832:
	pop {pc}
.L_02008834:
	.4byte Data_02003204
.L_02008838:
	.4byte Data_02003208
.L_0200883c:
	.4byte Data_0200320c
.L_02008840:
	.4byte Data_02003210
	.section .text.x02008844,"ax",%progbits
	.global Func_02000844
	.thumb_func
Func_02000844:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r5, .L_020088b0
	movs r3, #64
	movs r2, #70
	mov r10, r2
	strh r3, [r5]
	movs r3, #76
	strh r3, [r5, #4]
	mov r3, r10
	strh r3, [r5, #8]
	movs r3, #6
	strh r3, [r5, #12]
	movs r4, #10
	ldr r3, .L_020088a4
	mov r11, r4
	movs r4, #128
	movs r6, #74
	mov r2, r11
	lsls r4, r4, #2
	movs r0, #128
	strh r6, [r5, #2]
	strh r6, [r5, #6]
	strh r2, [r5, #10]
	strh r6, [r5, #14]
	strh r3, [r5, #16]
	strh r4, [r5, #18]
	lsls r0, r0, #2
	sub sp, #8
	bl Func_020028d8
	movs r3, #156
	ldr r7, .L_020088a8
	ldr r2, .L_020088ac
	lsls r3, r3, #6
	adds r3, #15
	mov r8, r3
	strh r2, [r5, #30]
	eors r0, r7
	mov r2, r8
	mov r4, r8
	strh r0, [r5, #20]
	b .L_020088b4
.L_020088a4:
	.4byte 0x00000024
.L_020088a8:
	.4byte 0x00000001
.L_020088ac:
	.4byte 0x00000000
.L_020088b0:
	.4byte gOverlayArea + 0x3450
.L_020088b4:
	strh r4, [r5, #24]
	strh r2, [r5, #22]
	mov r3, r10
	adds r5, #32
	strh r3, [r5]
	strh r6, [r5, #2]
	strh r6, [r5, #6]
	strh r6, [r5, #14]
	movs r3, #82
	ldr r6, .L_02008900
	movs r0, #129
	strh r3, [r5, #4]
	lsls r0, r0, #1
	movs r3, #78
	mov r4, r11
	strh r3, [r5, #8]
	adds r0, #255
	movs r3, #14
	strh r4, [r5, #10]
	strh r3, [r5, #12]
	strh r6, [r5, #16]
	strh r0, [r5, #18]
	bl Func_020028d8
	ldr r2, .L_02008904
	eors r0, r7
	mov r3, r8
	mov r4, r8
	strh r0, [r5, #20]
	strh r2, [r5, #30]
	strh r3, [r5, #24]
	strh r4, [r5, #22]
	movs r3, #102
	adds r5, #32
	movs r2, #26
	strh r3, [r5, #4]
	b .L_02008908
	.2byte 0x0000
.L_02008900:
	.4byte 0x00000024
.L_02008904:
	.4byte 0x00000000
.L_02008908:
	mov r9, r2
	ldr r6, .L_02008944
	movs r3, #103
	strh r3, [r5, #8]
	ldr r4, .L_02008944
	movs r2, #15
	mov r3, r9
	movs r0, #128
	mov r11, r2
	strh r3, [r5, #10]
	lsls r0, r0, #2
	movs r3, #39
	strh r6, [r5]
	strh r3, [r5, #12]
	movs r6, #93
	mov r3, r11
	adds r0, #2
	strh r6, [r5, #2]
	strh r6, [r5, #6]
	strh r4, [r5, #14]
	strh r3, [r5, #16]
	strh r0, [r5, #18]
	bl Func_020028d8
	ldr r4, .L_02008948
	eors r0, r7
	mov r2, r8
	mov r3, r8
	strh r0, [r5, #20]
	b .L_0200894c
.L_02008944:
	.4byte 0x0000005a
.L_02008948:
	.4byte 0x00000000
.L_0200894c:
	strh r4, [r5, #30]
	strh r2, [r5, #24]
	strh r3, [r5, #22]
	movs r4, #96
	adds r5, #32
	movs r3, #108
	mov r10, r4
	strh r3, [r5, #4]
	ldr r4, .L_02008998
	movs r3, #107
	movs r0, #130
	strh r3, [r5, #8]
	lsls r0, r0, #1
	mov r3, r9
	mov r2, r10
	strh r6, [r5, #2]
	strh r6, [r5, #6]
	strh r3, [r5, #10]
	mov r6, r11
	movs r3, #43
	adds r0, #255
	strh r2, [r5]
	strh r3, [r5, #12]
	strh r4, [r5, #14]
	strh r6, [r5, #16]
	strh r0, [r5, #18]
	bl Func_020028d8
	ldr r2, .L_0200899c
	mov r4, r8
	eors r0, r7
	mov r3, r8
	strh r0, [r5, #20]
	strh r2, [r5, #30]
	strh r3, [r5, #24]
	strh r4, [r5, #22]
	movs r3, #116
	b .L_020089a0
.L_02008998:
	.4byte 0x0000005a
.L_0200899c:
	.4byte 0x00000000
.L_020089a0:
	adds r5, #32
	strh r3, [r5]
	movs r0, #208
	movs r3, #122
	movs r2, #95
	strh r3, [r5, #4]
	lsls r0, r0, #5
	movs r3, #32
	strh r2, [r5, #2]
	strh r2, [r5, #6]
	mov r6, r10
	strh r3, [r5, #10]
	strh r3, [r5, #12]
	mov r2, r10
	movs r3, #13
	adds r0, #107
	strh r6, [r5, #8]
	strh r3, [r5, #16]
	strh r2, [r5, #14]
	strh r0, [r5, #18]
	bl Func_020028d8
	ldr r3, .L_020089f4
	eors r0, r7
	strh r0, [r5, #20]
	mov r4, r8
	mov r6, r8
	movs r0, #200
	strh r3, [r5, #30]
	strh r4, [r5, #24]
	strh r6, [r5, #22]
	lsls r0, r0, #2
	bl Func_020028d8
	cmp r0, #0
	beq .L_020089f8
	movs r0, #128
	lsls r0, r0, #2
	bl Func_020028e0
	b .L_02008a00
	.2byte 0x0000
.L_020089f4:
	.4byte 0x00000000
.L_020089f8:
	movs r0, #128
	lsls r0, r0, #2
	bl Func_020028e8
.L_02008a00:
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #33
	bl Func_020028d8
	cmp r0, #0
	beq .L_02008a1a
	movs r0, #129
	lsls r0, r0, #1
	adds r0, #255
	bl Func_020028e0
	b .L_02008a24
.L_02008a1a:
	movs r0, #129
	lsls r0, r0, #1
	adds r0, #255
	bl Func_020028e8
.L_02008a24:
	movs r0, #128
	lsls r0, r0, #2
	bl Func_020028d8
	cmp r0, #0
	beq .L_02008a80
	ldr r5, .L_02008b00
	movs r2, #0
	ldrsh r0, [r5, r2]
	movs r6, #8
	ldrsh r2, [r5, r6]
	movs r3, #2
	ldrsh r1, [r5, r3]
	movs r4, #16
	ldrsh r3, [r5, r4]
	movs r6, #10
	ldrsh r4, [r5, r6]
	adds r0, #3
	str r2, [sp, #0]
	movs r2, #3
	str r4, [sp, #4]
	bl Func_02002968
	movs r2, #4
	ldrsh r0, [r5, r2]
	movs r6, #12
	ldrsh r2, [r5, r6]
	movs r3, #6
	ldrsh r1, [r5, r3]
	movs r4, #16
	ldrsh r3, [r5, r4]
	movs r6, #14
	ldrsh r4, [r5, r6]
	adds r0, #3
	str r2, [sp, #0]
	movs r2, #3
	str r4, [sp, #4]
	bl Func_02002958
	movs r3, #156
	lsls r3, r3, #6
	adds r3, #15
	strh r3, [r5, #24]
	strh r3, [r5, #22]
	movs r3, #1
	strh r3, [r5, #20]
.L_02008a80:
	movs r0, #129
	lsls r0, r0, #1
	adds r0, #255
	bl Func_020028d8
	cmp r0, #0
	beq .L_02008ade
	ldr r5, .L_02008b04
	movs r2, #0
	ldrsh r0, [r5, r2]
	movs r6, #8
	ldrsh r2, [r5, r6]
	movs r3, #2
	ldrsh r1, [r5, r3]
	movs r4, #16
	ldrsh r3, [r5, r4]
	movs r6, #10
	ldrsh r4, [r5, r6]
	adds r0, #3
	str r2, [sp, #0]
	movs r2, #3
	str r4, [sp, #4]
	bl Func_02002968
	movs r2, #4
	ldrsh r0, [r5, r2]
	movs r6, #12
	ldrsh r2, [r5, r6]
	movs r3, #6
	ldrsh r1, [r5, r3]
	movs r4, #16
	ldrsh r3, [r5, r4]
	movs r6, #14
	ldrsh r4, [r5, r6]
	adds r0, #3
	str r2, [sp, #0]
	movs r2, #3
	str r4, [sp, #4]
	bl Func_02002958
	movs r3, #156
	lsls r3, r3, #6
	adds r3, #15
	strh r3, [r5, #24]
	strh r3, [r5, #22]
	movs r3, #1
	strh r3, [r5, #20]
.L_02008ade:
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_02008b08
	bl Func_02002870
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_02008b0c
	bl Func_02002870
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_02008b00:
	.4byte gOverlayArea + 0x3450
.L_02008b04:
	.4byte gOverlayArea + 0x3470
.L_02008b08:
	.4byte Func_02000740
.L_02008b0c:
	.4byte Func_02000580
	.section .text.x02008b10,"ax",%progbits
	.global Func_02000b10
	.thumb_func
Func_02000b10:
	push {lr}
	ldr r3, .L_02008b30
	movs r2, #2
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	beq .L_02008b26
	movs r1, #7
	bl Animation_ApplyChildValues
	b .L_02008b2c
.L_02008b26:
	movs r1, #0
	bl Animation_ApplyChildValues
.L_02008b2c:
	pop {pc}
	.2byte 0x0000
.L_02008b30:
	.4byte Data_0300122c
	.section .text.x02008b34,"ax",%progbits
	.global Func_02000b34
	.thumb_func
Func_02000b34:
	push {lr}
	ldr r3, .L_02008b54
	movs r2, #2
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	beq .L_02008b4a
	movs r1, #6
	bl Animation_ApplyChildValues
	b .L_02008b50
.L_02008b4a:
	movs r1, #0
	bl Animation_ApplyChildValues
.L_02008b50:
	pop {pc}
	.2byte 0x0000
.L_02008b54:
	.4byte Data_0300122c
	.section .text.x02008b58,"ax",%progbits
	.global Func_02000b58
	.thumb_func
Func_02000b58:
	push {r5, lr}
	movs r1, #0
	adds r5, r0, #0
	bl Animation_ApplyChildValues
	movs r3, #0
	str r3, [r5, #108]
	pop {r5, pc}
	.section .text.x02008b68,"ax",%progbits
	.global Func_02000b68
	.thumb_func
Func_02000b68:
	push {r5, r6, r7, lr}
	adds r0, r1, #0
	sub sp, #8
	bl Object_GetById
	adds r7, r0, #0
	bl Func_02002998
	movs r0, #0
	bl Func_02002a48
	ldr r3, .L_02008cd0
	movs r0, #30
	str r3, [r7, #108]
	bl WaitFrames
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #104
	bl Func_020028d8
	cmp r0, #0
	bne .L_02008b9e
	movs r0, #26
	bl Func_02002834
	b .L_02008ba4
.L_02008b9e:
	movs r0, #0
	bl Func_02002834
.L_02008ba4:
	movs r5, #3
.L_02008ba6:
	bl Random16Far
	movs r6, #254
	lsls r6, r6, #7
	adds r6, #255
	movs r1, #0
	ands r0, r6
	bl Func_02002a20
	movs r0, #15
	bl Func_02002a28
	movs r0, #10
	bl WaitFrames
	movs r0, #128
	movs r1, #0
	lsls r0, r0, #9
	bl Func_02002a20
	movs r0, #10
	bl Func_02002a28
	subs r5, #1
	movs r0, #10
	bl WaitFrames
	cmp r5, #0
	bge .L_02008ba6
	movs r1, #0
	adds r0, r6, #0
	bl Func_02002a20
	movs r0, #20
	bl Func_02002a28
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #104
	bl Func_020028d8
	cmp r0, #0
	bne .L_02008c58
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #104
	bl Func_020028e0
	movs r3, #51
	movs r2, #82
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #64
	movs r1, #64
	movs r2, #1
	movs r3, #2
	bl Func_02002958
	movs r3, #115
	movs r2, #18
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #2
	movs r1, #64
	movs r2, #1
	movs r0, #66
	bl Func_02002958
	movs r0, #5
	bl Func_02002948
	movs r0, #6
	bl Func_02002948
	movs r0, #7
	bl Func_02002940
	movs r0, #8
	bl Func_02002940
	ldr r3, .L_02008cd4
	movs r0, #26
	str r3, [r7, #108]
	bl Func_020025a8
	b .L_02008cac
.L_02008c58:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #104
	bl Func_020028e8
	movs r3, #51
	movs r2, #82
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #65
	movs r1, #64
	movs r2, #1
	movs r3, #2
	bl Func_02002958
	movs r3, #115
	movs r2, #18
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #2
	movs r1, #64
	movs r2, #1
	movs r0, #67
	bl Func_02002958
	movs r0, #7
	bl Func_02002948
	movs r0, #8
	bl Func_02002948
	movs r0, #5
	bl Func_02002940
	movs r0, #6
	bl Func_02002940
	ldr r3, .L_02008cd4
	movs r0, #0
	str r3, [r7, #108]
	bl Func_020025a8
.L_02008cac:
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #128
	movs r1, #0
	lsls r0, r0, #9
	bl Func_02002a20
	movs r0, #30
	bl Func_02002a28
	movs r0, #15
	bl Battle_WaitMode0
	bl Func_020029a0
	add sp, #8
	pop {r5, r6, r7, pc}
.L_02008cd0:
	.4byte Func_02000b10
.L_02008cd4:
	.4byte Func_02000b58
	.section .text.x02008cd8,"ax",%progbits
	.global Func_02000cd8
	.thumb_func
Func_02000cd8:
	push {r5, r6, lr}
	cmp r1, #24
	bne .L_02008d36
	movs r0, #24
	bl Object_GetById
	ldr r3, [r0, #8]
	asrs r6, r3, #20
	ldr r3, [r0, #16]
	movs r0, #196
	lsls r0, r0, #2
	adds r1, r6, #0
	asrs r5, r3, #20
	bl GameFlag_SetByte
	movs r0, #198
	lsls r0, r0, #2
	adds r1, r5, #0
	bl GameFlag_SetByte
	movs r0, #200
	lsls r0, r0, #2
	bl Func_020028e8
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #33
	bl Func_020028e8
	cmp r6, #7
	bne .L_02008d22
	cmp r5, #35
	bne .L_02008d22
	movs r0, #200
	lsls r0, r0, #2
	bl Func_020028e0
.L_02008d22:
	cmp r6, #15
	bne .L_02008d62
	cmp r5, #35
	bne .L_02008d62
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #33
	bl Func_020028e0
	b .L_02008d62
.L_02008d36:
	cmp r1, #25
	bne .L_02008d62
	movs r0, #25
	bl Object_GetById
	adds r5, r0, #0
	ldr r3, [r5, #8]
	asrs r3, r3, #20
	cmp r3, #32
	bne .L_02008d62
	movs r0, #30
	bl WaitFrames
	ldr r3, [r5, #16]
	movs r2, #128
	lsls r2, r2, #9
	adds r3, r3, r2
	movs r0, #149
	str r3, [r5, #16]
	lsls r0, r0, #4
	bl Func_020028e0
.L_02008d62:
	pop {r5, r6, pc}
	.section .text.x02008d64,"ax",%progbits
	.global Func_02000d64
	.thumb_func
Func_02000d64:
	push {r5, r6, r7, lr}
	adds r7, r0, #0
	adds r6, r1, #0
	movs r5, #60
.L_02008d6c:
	cmp r5, #0
	beq .L_02008d7e
	movs r0, #1
	bl WaitFrames
	ldr r3, [r7, #12]
	subs r5, #1
	cmp r3, r6
	bgt .L_02008d6c
.L_02008d7e:
	pop {r5, r6, r7, pc}
	.section .text.x02008d80,"ax",%progbits
	.global Func_02000d80
	.thumb_func
Func_02000d80:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #104
	sub sp, #12
	bl Func_020028d8
	cmp r0, #0
	bne .L_02008da0
	b .L_02008f5a
.L_02008da0:
	ldr r0, .L_02008f6c
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #133
	mov r10, r0
	lsls r3, r3, #2
	add r3, r10
	movs r1, #230
	ldr r3, [r3]
	lsls r1, r1, #1
	adds r2, r2, r1
	ldr r2, [r2]
	mov r8, r3
	mov r0, r8
	mov r9, r2
	bl Object_GetById
	adds r6, r0, #0
	ldr r0, .L_02008f70
	ldr r1, .L_02008f74
	movs r2, #0
	ldrsh r3, [r0, r2]
	mov r11, r1
	cmp r3, r11
	bne .L_02008ddc
	ldr r3, [r6, #20]
	cmp r3, #0
	bne .L_02008ddc
	b .L_02008f5e
.L_02008ddc:
	ldr r3, [r6, #8]
	mov r7, sp
	str r3, [r7]
	movs r2, #128
	ldr r3, [r6, #12]
	lsls r2, r2, #10
	str r3, [r7, #4]
	adds r0, r6, #0
	ldr r3, [r6, #16]
	adds r1, r7, #0
	adds r3, r3, r2
	str r3, [r7, #8]
	bl Func_02002970
	ldr r3, .L_02008f78
	movs r2, #4
	ldr r3, [r3]
	adds r5, r0, #0
	ands r3, r2
	cmp r3, #0
	bne .L_02008e0c
	adds r0, r6, #0
	bl Func_02001d30
.L_02008e0c:
	cmp r5, #0
	bge .L_02008e5c
	movs r1, #129
	mov r0, r8
	lsls r1, r1, #1
	bl Func_020029f8
	ldr r3, [r6, #16]
	ldr r0, .L_02008f7c
	ldr r1, [r6, #8]
	adds r3, r3, r0
	ldr r2, [r6, #12]
	adds r0, r6, #0
	bl Func_02002930
	adds r0, r6, #0
	movs r1, #7
	bl Func_02002910
	adds r0, r6, #0
	bl Func_02002938
.L_02008e38:
	movs r0, #1
	bl WaitFrames
	ldr r2, [r6, #12]
	ldr r3, [r6, #20]
	cmp r2, r3
	bne .L_02008e38
	adds r0, r6, #0
	bl Func_02001d30
	adds r0, r6, #0
	movs r1, #6
	bl Func_02002910
	movs r0, #3
	bl WaitFrames
	b .L_02008f5e
.L_02008e5c:
	ldr r3, [r6, #8]
	ldr r1, .L_02008f7c
	str r3, [r7]
	adds r0, r6, #0
	ldr r3, [r6, #12]
	str r3, [r7, #4]
	ldr r3, [r6, #16]
	adds r3, r3, r1
	str r3, [r7, #8]
	adds r1, r7, #0
	bl Func_02002970
	adds r5, r0, #0
	cmp r5, #0
	ble .L_02008f06
	ldr r0, .L_02008f70
	movs r2, #0
	ldrsh r3, [r0, r2]
	cmp r3, r11
	bne .L_02008f5e
	bl Func_02002998
	movs r0, #0
	bl Func_02002a48
	adds r2, r6, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	movs r1, #0
	adds r0, r6, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #18
	add r2, r10
	movs r3, #2
	strb r3, [r2]
	adds r0, r6, #0
	bl Func_02001d30
	movs r0, #5
	bl Battle_WaitMode0
	movs r5, #59
.L_02008eb8:
	ldr r3, [r6, #12]
	movs r1, #128
	lsls r1, r1, #10
	adds r3, r3, r1
	str r3, [r6, #12]
	ldrh r3, [r6, #6]
	movs r2, #128
	lsls r2, r2, #5
	adds r3, r3, r2
	strh r3, [r6, #6]
	bl Random16Far
	movs r3, #240
	lsls r3, r3, #4
	adds r3, #255
	cmp r0, r3
	bhi .L_02008ee0
	adds r0, r6, #0
	bl Func_02001d30
.L_02008ee0:
	movs r0, #1
	subs r5, #1
	bl WaitFrames
	cmp r5, #0
	bge .L_02008eb8
	ldr r3, .L_02008f6c
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #18
	adds r3, r3, r0
	movs r2, #0
	strb r2, [r3]
	bl Func_020029a0
	movs r0, #6
	bl Func_02002a18
	b .L_02008f5e
.L_02008f06:
	ldr r3, [r6, #8]
	ldr r1, .L_02008f80
	ldr r2, .L_02008f84
	adds r3, r3, r1
	str r3, [r7]
	adds r0, r6, #0
	ldr r3, [r6, #12]
	adds r1, r7, #0
	str r3, [r7, #4]
	ldr r3, [r6, #16]
	adds r3, r3, r2
	str r3, [r7, #8]
	bl Func_02002970
	adds r5, r0, #0
	cmp r5, #0
	bgt .L_02008f5e
	ldr r3, [r6, #8]
	ldr r0, .L_02008f84
	adds r1, r7, #0
	adds r3, r3, r0
	str r3, [r7]
	ldr r3, [r6, #12]
	str r3, [r7, #4]
	ldr r3, [r6, #16]
	adds r3, r3, r0
	str r3, [r7, #8]
	adds r0, r6, #0
	bl Func_02002970
	adds r5, r0, #0
	cmp r5, #0
	bgt .L_02008f5e
	mov r1, r9
	ldr r3, [r1, #16]
	ldr r2, .L_02008f88
	adds r3, r3, r2
	str r3, [r1, #16]
	ldr r3, [r6, #16]
	adds r3, r3, r2
	str r3, [r6, #16]
	b .L_02008f5e
.L_02008f5a:
	bl Func_02001e00
.L_02008f5e:
	add sp, #12
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_02008f6c:
	.4byte gPartyState
.L_02008f70:
	.4byte Data_0200024c + 0x1d4
.L_02008f74:
	.4byte 0x0000009e
.L_02008f78:
	.4byte Data_0300122c
.L_02008f7c:
	.4byte 0xfff80000
.L_02008f80:
	.4byte 0x0005b333
.L_02008f84:
	.4byte 0xfffa4ccd
.L_02008f88:
	.4byte 0xfffe0000
	.section .text.x02008f8c,"ax",%progbits
	.global Func_02000f8c
	.thumb_func
Func_02000f8c:
	push {r5, r6, r7, lr}
	ldr r5, .L_02009008
	movs r3, #192
	movs r2, #133
	lsls r3, r3, #18
	lsls r2, r2, #2
	ldr r6, [r3, #32]
	ldr r7, [r3, #108]
	adds r3, r5, r2
	ldr r0, [r3]
	bl Object_GetById
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #18
	adds r5, r5, r3
	ldrb r3, [r5]
	ldr r2, [r0, #8]
	ldr r4, [r0, #16]
	ldr r1, [r0, #12]
	cmp r3, #2
	bne .L_02008fe4
	asrs r0, r2, #20
	ldr r2, .L_0200900c
	subs r3, r4, r1
	adds r3, r3, r2
	movs r2, #184
	lsls r2, r2, #1
	asrs r1, r3, #20
	adds r3, r6, r2
	ldr r2, [r3]
	lsls r3, r1, #7
	adds r3, r3, r0
	lsls r3, r3, #2
	adds r2, r2, r3
	ldrb r3, [r2, #3]
	cmp r3, #0
	beq .L_02009004
	movs r3, #181
	lsls r3, r3, #1
	adds r2, r7, r3
	movs r3, #44
	strh r3, [r2]
	b .L_02009004
.L_02008fe4:
	asrs r0, r2, #20
	movs r2, #184
	subs r3, r4, r1
	lsls r2, r2, #1
	asrs r1, r3, #20
	adds r3, r6, r2
	ldr r2, [r3]
	lsls r3, r1, #7
	adds r3, r3, r0
	lsls r3, r3, #2
	adds r2, r2, r3
	ldrb r3, [r2, #3]
	cmp r3, #0
	beq .L_02009004
	bl Func_02001e00
.L_02009004:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009008:
	.4byte gPartyState
.L_0200900c:
	.4byte 0xfff40000
	.section .text.x02009010,"ax",%progbits
	.global Func_02001010
	.thumb_func
Func_02001010:
	push {r5, r6, lr}
	sub sp, #8
	movs r3, #33
	movs r2, #41
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #62
	movs r1, #41
	movs r2, #1
	movs r3, #2
	bl Func_02002960
	movs r6, #96
	movs r5, #32
	movs r0, #125
	movs r1, #32
	movs r2, #3
	movs r3, #13
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl Func_02002968
	movs r2, #3
	movs r3, #13
	movs r1, #96
	movs r0, #61
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_02002968
	movs r0, #12
	bl Object_GetById
	movs r1, #5
	bl Func_020029e8
	movs r0, #12
	bl Func_02001f60
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #107
	bl Func_020028e0
	add sp, #8
	pop {r5, r6, pc}
	.section .text.x0200906c,"ax",%progbits
	.global Func_0200106c
	.thumb_func
Func_0200106c:
	push {r5, r6, r7, lr}
	ldr r5, .L_020090f0
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
	ldr r0, [r5]
	bl Object_GetById
	adds r6, r0, #0
	ldr r7, [r6, #72]
	bl Func_02002998
	movs r0, #0
	bl Func_02002a48
	movs r1, #2
	ldr r0, [r5]
	adds r1, #255
	bl Func_020029f8
	adds r0, r6, #0
	movs r1, #40
	bl Func_02002910
	adds r2, r6, #0
	adds r2, #85
	movs r3, #3
	movs r5, #0
	strb r3, [r2]
	b .L_020090aa
.L_020090a8:
	adds r5, #1
.L_020090aa:
	cmp r5, #179
	bgt .L_020090bc
	movs r0, #1
	bl WaitFrames
	ldr r2, [r6, #12]
	ldr r3, [r6, #20]
	cmp r2, r3
	bgt .L_020090a8
.L_020090bc:
	ldr r5, .L_020090f0
	str r7, [r6, #72]
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r5, r2
	ldr r0, [r3]
	movs r1, #0
	bl Func_020029f8
	adds r0, r6, #0
	movs r1, #1
	bl ObjectDispatch_SetSingleChildField26
	adds r0, r6, #0
	movs r1, #1
	bl Func_02002910
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #18
	adds r5, r5, r3
	movs r3, #0
	strb r3, [r5]
	bl Func_020029a0
	pop {r5, r6, r7, pc}
.L_020090f0:
	.4byte gPartyState
	.section .text.x020090f4,"ax",%progbits
	.global Func_020010f4
	.thumb_func
Func_020010f4:
	push {lr}
	adds r0, r1, #0
	sub sp, #8
	bl Object_GetById
	adds r2, r0, #0
	movs r3, #0
	adds r2, #35
	adds r0, #85
	strb r3, [r2]
	strb r3, [r0]
	movs r2, #32
	movs r3, #44
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #32
	movs r2, #1
	movs r3, #1
	movs r0, #42
	bl Func_02002960
	movs r0, #145
	lsls r0, r0, #1
	adds r0, #255
	bl Func_020028e0
	add sp, #8
	pop {pc}
	.section .text.x0200912c,"ax",%progbits
	.global Func_0200112c
	.thumb_func
Func_0200112c:
	push {lr}
	adds r0, r1, #0
	sub sp, #8
	bl Object_GetById
	adds r2, r0, #0
	movs r3, #0
	adds r2, #35
	adds r0, #85
	strb r3, [r2]
	strb r3, [r0]
	movs r2, #32
	movs r3, #46
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #32
	movs r2, #1
	movs r3, #1
	movs r0, #48
	bl Func_02002960
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #34
	bl Func_020028e0
	add sp, #8
	pop {pc}
	.section .text.x02009164,"ax",%progbits
	.global Func_02001164
	.thumb_func
Func_02001164:
	push {lr}
	adds r0, r1, #0
	sub sp, #8
	bl Object_GetById
	adds r2, r0, #0
	movs r3, #0
	adds r2, #35
	adds r0, #85
	strb r3, [r2]
	strb r3, [r0]
	movs r2, #44
	movs r3, #46
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #44
	movs r2, #1
	movs r3, #1
	movs r0, #44
	bl Func_02002960
	movs r0, #146
	lsls r0, r0, #1
	adds r0, #255
	bl Func_020028e0
	add sp, #8
	pop {pc}
	.section .text.x0200919c,"ax",%progbits
	.global Func_0200119c
	.thumb_func
Func_0200119c:
	push {r5, r6, lr}
	adds r0, r1, #0
	sub sp, #8
	bl Object_GetById
	adds r5, r0, #0
	ldr r3, [r5, #8]
	asrs r6, r3, #20
	cmp r6, #53
	bne .L_020091dc
	movs r0, #30
	bl WaitFrames
	ldr r3, [r5, #16]
	movs r2, #128
	lsls r2, r2, #9
	movs r0, #144
	adds r3, r3, r2
	lsls r0, r0, #4
	str r3, [r5, #16]
	adds r0, #81
	bl Func_020028e0
	movs r3, #52
	str r3, [sp, #4]
	movs r0, #54
	movs r1, #58
	movs r2, #1
	movs r3, #1
	str r6, [sp, #0]
	bl Func_02002960
.L_020091dc:
	add sp, #8
	pop {r5, r6, pc}
	.section .text.x020091e0,"ax",%progbits
	.global Func_020011e0
	.thumb_func
Func_020011e0:
	push {r5, r6, lr}
	adds r0, r1, #0
	sub sp, #8
	bl Object_GetById
	adds r5, r0, #0
	ldr r3, [r5, #8]
	asrs r6, r3, #20
	cmp r6, #53
	bne .L_02009220
	movs r0, #30
	bl WaitFrames
	ldr r3, [r5, #16]
	movs r2, #128
	lsls r2, r2, #9
	movs r0, #144
	adds r3, r3, r2
	lsls r0, r0, #4
	str r3, [r5, #16]
	adds r0, #82
	bl Func_020028e0
	movs r3, #58
	str r3, [sp, #4]
	movs r0, #54
	movs r1, #58
	movs r2, #1
	movs r3, #1
	str r6, [sp, #0]
	bl Func_02002960
.L_02009220:
	add sp, #8
	pop {r5, r6, pc}
	.section .text.x02009224,"ax",%progbits
	.global Func_02001224
	.thumb_func
Func_02001224:
	push {r5, r6, r7, lr}
	adds r0, r1, #0
	sub sp, #8
	bl Object_GetById
	adds r7, r0, #0
	bl Func_02002998
	movs r0, #0
	bl Func_02002a48
	ldr r3, .L_020093a8
	movs r0, #30
	str r3, [r7, #108]
	bl WaitFrames
	movs r0, #8
	bl Func_02002834
	movs r0, #220
	bl Func_02002a70
	movs r5, #3
.L_02009252:
	bl Random16Far
	movs r6, #254
	lsls r6, r6, #7
	adds r6, #255
	movs r1, #0
	ands r0, r6
	bl Func_02002a20
	movs r0, #15
	bl Func_02002a28
	movs r0, #10
	bl WaitFrames
	movs r0, #128
	movs r1, #0
	lsls r0, r0, #9
	bl Func_02002a20
	movs r0, #10
	bl Func_02002a28
	subs r5, #1
	movs r0, #10
	bl WaitFrames
	cmp r5, #0
	bge .L_02009252
	movs r1, #0
	adds r0, r6, #0
	bl Func_02002a20
	movs r0, #20
	bl Func_02002a28
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #105
	bl Func_020028e0
	movs r3, #48
	str r3, [sp, #4]
	movs r5, #16
	movs r0, #15
	movs r1, #48
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	bl Func_02002960
	movs r3, #50
	str r3, [sp, #4]
	movs r1, #50
	movs r2, #1
	movs r0, #17
	movs r3, #1
	str r5, [sp, #0]
	bl Func_02002960
	ldr r3, .L_020093ac
	movs r0, #8
	str r3, [r7, #108]
	bl Func_020025a8
	movs r3, #94
	str r3, [sp, #4]
	movs r5, #14
	movs r0, #64
	movs r1, #68
	movs r2, #5
	movs r3, #13
	str r5, [sp, #0]
	bl Func_02002968
	movs r3, #34
	str r3, [sp, #4]
	movs r2, #5
	movs r3, #8
	movs r1, #68
	movs r0, #64
	str r5, [sp, #0]
	bl Func_02002960
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #128
	movs r1, #0
	lsls r0, r0, #9
	bl Func_02002a20
	movs r0, #30
	bl Func_02002a28
	movs r0, #15
	bl Battle_WaitMode0
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #9
	lsls r1, r1, #6
	bl Func_02002a00
	movs r0, #248
	movs r2, #190
	movs r3, #1
	lsls r0, r0, #16
	ldr r1, .L_020093b0
	lsls r2, r2, #18
	bl Func_02002a08
	bl Func_02002a10
	movs r1, #132
	movs r2, #198
	lsls r2, r2, #18
	lsls r1, r1, #17
	movs r0, #15
	bl Func_020029d8
	movs r0, #15
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #15
	bl Object_GetById
	movs r3, #0
	adds r0, #89
	strb r3, [r0]
	movs r0, #15
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #2
	orrs r3, r2
	strb r3, [r0]
	movs r0, #15
	bl Object_GetById
	ldr r3, .L_020093b4
	movs r5, #5
	str r3, [r0, #108]
	movs r0, #30
	bl Battle_WaitMode0
.L_02009384:
	movs r0, #184
	lsls r0, r0, #1
	adds r0, #255
	bl Func_02002a70
	subs r5, #1
	movs r0, #10
	bl WaitFrames
	cmp r5, #0
	bge .L_02009384
	movs r0, #30
	bl Battle_WaitMode0
	bl Func_020029a0
	add sp, #8
	pop {r5, r6, r7, pc}
.L_020093a8:
	.4byte Func_02000b10
.L_020093ac:
	.4byte Func_02000b58
.L_020093b0:
	.4byte 0xffe00000
.L_020093b4:
	.4byte Func_02001d98
	.section .text.x020093b8,"ax",%progbits
	.global Func_020013b8
	.thumb_func
Func_020013b8:
	push {lr}
	ldr r3, .L_020093e0
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_020093e4
	cmp r2, r3
	bne .L_020093d0
	ldr r0, .L_020093e8
	b .L_020093dc
.L_020093d0:
	ldr r3, .L_020093ec
	cmp r2, r3
	bne .L_020093da
	ldr r0, .L_020093f0
	b .L_020093dc
.L_020093da:
	ldr r0, .L_020093f4
.L_020093dc:
	pop {pc}
	.2byte 0x0000
.L_020093e0:
	.4byte gPartyState
.L_020093e4:
	.4byte 0x0000009e
.L_020093e8:
	.4byte Data_02003220
.L_020093ec:
	.4byte 0x0000009f
.L_020093f0:
	.4byte Data_020032d4
.L_020093f4:
	.4byte Data_02003214
	.section .text.x020093f8,"ax",%progbits
	.global Func_020013f8
	.thumb_func
Func_020013f8:
	push {r5, lr}
	bl Func_02002a68
	bl Object_GetById
	adds r5, r0, #0
	cmp r5, #0
	beq .L_0200945a
	adds r3, r5, #0
	adds r3, #84
	ldrb r2, [r3]
	movs r3, #15
	ands r3, r2
	cmp r3, #1
	bne .L_0200945a
	ldr r3, .L_0200945c
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #18
	adds r3, r3, r2
	ldrb r2, [r3]
	movs r1, #2
	ldr r4, [r5, #80]
	eors r2, r1
	negs r3, r2
	orrs r3, r2
	ldrb r0, [r4, #9]
	lsrs r3, r3, #31
	movs r2, #13
	subs r1, r1, r3
	negs r2, r2
	movs r3, #3
	ands r1, r3
	adds r3, r2, #0
	lsls r1, r1, #2
	ands r3, r0
	orrs r3, r1
	strb r3, [r4, #9]
	adds r4, #37
	ldrb r3, [r4]
	ands r2, r3
	orrs r2, r1
	adds r1, r5, #0
	adds r1, #35
	strb r2, [r4]
	ldrb r2, [r1]
	movs r3, #254
	ands r3, r2
	strb r3, [r1]
.L_0200945a:
	pop {r5, pc}
.L_0200945c:
	.4byte gPartyState
	.section .text.x02009460,"ax",%progbits
	.global Func_02001460
	.thumb_func
Func_02001460:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r5, .L_02009530
	movs r1, #133
	lsls r1, r1, #2
	adds r5, r5, r1
	ldr r0, [r5]
	bl Object_GetById
	adds r6, r0, #0
	adds r7, r6, #0
	adds r7, #85
	ldrb r2, [r7]
	movs r0, #153
	lsls r0, r0, #2
	mov r8, r2
	bl Func_02002a70
	bl Func_02002998
	movs r0, #0
	bl Func_02002a48
	bl Event_SetStatus1c6
	adds r0, r6, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r3, #0
	strb r3, [r7]
	ldr r3, .L_02009534
	movs r1, #128
	str r3, [r6, #12]
	movs r2, #128
	ldr r0, [r5]
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl ObjectMotion_SetSpeedParameters
	movs r5, #15
.L_020094b4:
	ldr r3, [r6, #12]
	movs r1, #128
	lsls r1, r1, #9
	adds r3, r3, r1
	str r3, [r6, #12]
	ldrh r3, [r6, #6]
	movs r2, #128
	lsls r2, r2, #6
	adds r3, r3, r2
	strh r3, [r6, #6]
	movs r0, #1
	subs r5, #1
	bl WaitFrames
	cmp r5, #0
	bge .L_020094b4
	adds r0, r6, #0
	bl Func_02001d30
	mov r3, r8
	strb r3, [r7]
	movs r3, #128
	lsls r3, r3, #12
	str r3, [r6, #40]
	adds r0, r6, #0
	movs r1, #1
	bl ObjectDispatch_SetSingleChildField26
	ldr r3, .L_02009538
	movs r2, #64
	ldr r3, [r3]
	ands r3, r2
	ldr r2, .L_02009530
	cmp r3, #0
	beq .L_02009510
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r2, r1
	movs r1, #132
	movs r2, #190
	ldr r0, [r3]
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_SetPositionAndCommit
	b .L_02009524
.L_02009510:
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r2, r1
	movs r1, #132
	movs r2, #206
	ldr r0, [r3]
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_SetPositionAndCommit
.L_02009524:
	bl Func_020029a0
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009530:
	.4byte gPartyState
.L_02009534:
	.4byte 0xfff00000
.L_02009538:
	.4byte gInput
	.section .text.x0200953c,"ax",%progbits
	.global Func_0200153c
	.thumb_func
Func_0200153c:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r5, .L_020095e0
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
	ldr r0, [r5]
	bl Object_GetById
	adds r6, r0, #0
	adds r7, r6, #0
	adds r7, #85
	ldrb r3, [r7]
	mov r8, r3
	bl Func_02002998
	movs r0, #0
	bl Func_02002a48
	bl Event_SetStatus1c6
	adds r0, r6, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r3, #0
	strb r3, [r7]
	ldr r3, .L_020095e4
	movs r1, #128
	str r3, [r6, #12]
	movs r2, #128
	ldr r0, [r5]
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl ObjectMotion_SetSpeedParameters
	movs r5, #23
.L_02009588:
	ldr r3, [r6, #12]
	movs r2, #128
	lsls r2, r2, #10
	adds r3, r3, r2
	str r3, [r6, #12]
	ldrh r3, [r6, #6]
	movs r2, #128
	lsls r2, r2, #6
	adds r3, r3, r2
	strh r3, [r6, #6]
	movs r0, #1
	subs r5, #1
	bl WaitFrames
	cmp r5, #0
	bge .L_02009588
	adds r0, r6, #0
	bl Func_02001d30
	mov r3, r8
	strb r3, [r7]
	movs r3, #192
	lsls r3, r3, #12
	str r3, [r6, #40]
	adds r0, r6, #0
	movs r1, #1
	bl ObjectDispatch_SetSingleChildField26
	ldr r3, .L_020095e0
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	movs r1, #170
	movs r2, #228
	ldr r0, [r3]
	lsls r1, r1, #2
	lsls r2, r2, #2
	bl ObjectMotion_SetPositionAndCommit
	bl Func_020029a0
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_020095e0:
	.4byte gPartyState
.L_020095e4:
	.4byte 0xffc00000
	.section .text.x020095e8,"ax",%progbits
	.global Func_020015e8
	.thumb_func
Func_020015e8:
	push {r5, r6, lr}
	adds r1, r0, #0
	adds r1, #100
	ldrh r3, [r1]
	movs r2, #128
	lsls r2, r2, #6
	adds r3, r3, r2
	ldr r6, [r0, #80]
	strh r3, [r1]
	adds r5, r0, #0
	adds r5, #102
	ldrh r3, [r5]
	movs r0, #128
	adds r3, #32
	strh r3, [r5]
	movs r2, #128
	lsls r3, r3, #16
	lsls r0, r0, #18
	lsls r2, r2, #2
	cmp r3, r0
	ble .L_02009614
	strh r2, [r5]
.L_02009614:
	movs r2, #0
	ldrsh r0, [r1, r2]
	bl Math_Sine
	movs r3, #0
	ldrsh r1, [r5, r3]
	ldr r3, .L_0200962c
	mov lr, r3
	.2byte 0xf800
	strh r0, [r6, #18]
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200962c:
	.4byte IwramMulQ16
	.section .text.x02009630,"ax",%progbits
	.global Func_02001630
	.thumb_func
Func_02001630:
	push {r5, r6, lr}
	adds r2, r0, #0
	adds r2, #100
	ldrh r3, [r2]
	movs r1, #128
	lsls r1, r1, #6
	adds r3, r3, r1
	ldr r6, [r0, #80]
	strh r3, [r2]
	adds r5, r0, #0
	adds r5, #102
	ldrh r3, [r5]
	subs r3, #16
	strh r3, [r5]
	lsls r3, r3, #16
	cmp r3, #0
	bge .L_02009656
	movs r3, #0
	strh r3, [r5]
.L_02009656:
	movs r3, #0
	ldrsh r0, [r2, r3]
	bl Math_Sine
	movs r3, #0
	ldrsh r1, [r5, r3]
	ldr r3, .L_0200966c
	mov lr, r3
	.2byte 0xf800
	strh r0, [r6, #18]
	pop {r5, r6, pc}
.L_0200966c:
	.4byte IwramMulQ16
	.section .text.x02009670,"ax",%progbits
	.global Func_02001670
	.thumb_func
Func_02001670:
	push {r5, lr}
	adds r3, r0, #0
	ldr r5, [r3, #80]
	adds r3, #100
	ldrh r0, [r3]
	movs r2, #128
	lsls r2, r2, #1
	adds r0, r0, r2
	strh r0, [r3]
	lsls r0, r0, #16
	asrs r0, r0, #16
	bl Math_Sine
	cmp r0, #0
	bge .L_02009690
	adds r0, #63
.L_02009690:
	movs r2, #192
	asrs r3, r0, #6
	lsls r2, r2, #3
	adds r3, r3, r2
	strh r3, [r5, #18]
	pop {r5, pc}
	.section .text.x0200969c,"ax",%progbits
	.global Func_0200169c
	.thumb_func
Func_0200169c:
	push {r5, r6, lr}
	sub sp, #8
	bl Func_02002998
	movs r0, #0
	bl Func_02002a48
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r0, r0
	negs r1, r1
	negs r2, r2
	movs r3, #0
	bl Func_02002a08
	ldr r3, .L_020097e4
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	movs r1, #0
	movs r2, #0
	bl Func_020029d8
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	movs r0, #184
	bl Func_02002a70
	movs r6, #17
.L_020096de:
	adds r0, r6, #0
	bl Object_GetById
	movs r3, #128
	adds r5, r0, #0
	lsls r3, r3, #7
	str r3, [r5, #48]
	movs r3, #128
	lsls r3, r3, #3
	str r3, [r5, #52]
	ldr r3, .L_020097e8
	adds r6, #1
	str r3, [r5, #108]
	bl Random16Far
	adds r3, r5, #0
	adds r3, #100
	movs r2, #0
	adds r5, #102
	strh r0, [r3]
	strh r2, [r5]
	cmp r6, #21
	ble .L_020096de
	movs r0, #60
	bl Battle_WaitMode0
	movs r3, #53
	str r3, [sp, #4]
	movs r5, #87
	movs r3, #4
	movs r0, #64
	movs r1, #69
	movs r2, #11
	str r5, [sp, #0]
	bl Func_02002958
	movs r1, #196
	movs r2, #206
	movs r0, #17
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	movs r1, #212
	movs r2, #206
	movs r0, #18
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	movs r1, #228
	movs r2, #206
	movs r0, #19
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	movs r1, #244
	movs r2, #206
	movs r0, #20
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_ResetAndSetPosition
	movs r1, #130
	movs r2, #206
	lsls r1, r1, #2
	lsls r2, r2, #2
	movs r0, #21
	bl ObjectMotion_ResetAndSetPosition
	movs r0, #240
	bl Battle_WaitMode0
	movs r3, #50
	str r3, [sp, #4]
	movs r0, #64
	movs r1, #66
	movs r2, #11
	movs r3, #7
	str r5, [sp, #0]
	bl Func_02002958
	movs r6, #17
.L_02009786:
	adds r0, r6, #0
	bl Object_GetById
	ldr r3, .L_020097ec
	adds r6, #1
	str r3, [r0, #108]
	cmp r6, #21
	ble .L_02009786
	movs r0, #149
	lsls r0, r0, #1
	bl Func_02002a70
	movs r0, #60
	bl Battle_WaitMode0
	movs r0, #21
	bl Object_GetById
	ldr r3, .L_020097f0
	str r3, [r0, #108]
	movs r3, #0
	adds r0, #100
	strh r3, [r0]
	bl Event_ClearStatus1c6
	bl Event_WaitValue1c8Frames
	movs r0, #10
	adds r0, #255
	bl Func_020028e0
	movs r0, #48
	adds r0, #255
	bl Func_020028e8
	movs r0, #77
	bl Func_02002a18
	bl Func_020029a0
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #85
	bl Func_020028e0
	add sp, #8
	pop {r5, r6, pc}
.L_020097e4:
	.4byte gPartyState
.L_020097e8:
	.4byte Func_020015e8
.L_020097ec:
	.4byte Func_02001630
.L_020097f0:
	.4byte Func_02001670
	.section .text.x020097f4,"ax",%progbits
	.global Func_020017f4
	.thumb_func
Func_020017f4:
	push {r5, r6, r7, lr}
	ldr r7, .L_02009acc
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r7, r1
	movs r2, #0
	ldrsh r6, [r3, r2]
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #32]
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	ldr r5, .L_02009ad0
	adds r2, #88
	str r2, [r3]
	adds r2, #92
	adds r3, r7, r2
	strh r5, [r3]
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #98
	adds r2, r7, r3
	movs r3, #1
	strh r3, [r2]
	ldrb r2, [r1, #23]
	subs r3, #14
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	strb r3, [r1, #23]
	movs r0, #170
	sub sp, #8
	bl Func_02002a58
	cmp r6, r5
	beq .L_02009842
	b .L_02009ae0
.L_02009842:
	movs r0, #8
	bl Object_GetById
	movs r1, #4
	bl Func_020029e8
	movs r0, #9
	bl Object_GetById
	movs r1, #4
	bl Func_020029e8
	movs r0, #10
	bl Object_GetById
	movs r1, #4
	bl Func_020029e8
	movs r0, #11
	bl Object_GetById
	movs r1, #4
	bl Func_020029e8
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #107
	bl Func_020028d8
	cmp r0, #0
	beq .L_0200988e
	movs r0, #12
	bl Object_GetById
	movs r1, #5
	bl Func_020029e8
	b .L_0200989a
.L_0200988e:
	movs r0, #12
	bl Object_GetById
	movs r1, #6
	bl Func_020029e8
.L_0200989a:
	movs r0, #8
	bl Func_02001f60
	movs r0, #9
	bl Func_02001f60
	movs r0, #10
	bl Func_02001f60
	movs r0, #11
	bl Func_02001f60
	movs r0, #13
	bl Func_02001f60
	movs r0, #14
	bl Func_02001f60
	movs r5, #15
.L_020098c0:
	adds r0, r5, #0
	adds r5, #1
	bl Func_02001f60
	cmp r5, #23
	ble .L_020098c0
	movs r5, #8
.L_020098ce:
	adds r0, r5, #0
	movs r1, #3
	adds r5, #1
	bl Func_020029f0
	cmp r5, #14
	ble .L_020098ce
	movs r5, #15
.L_020098de:
	adds r0, r5, #0
	movs r1, #1
	adds r5, #1
	bl Func_020029f0
	cmp r5, #23
	ble .L_020098de
	movs r0, #15
	bl Object_GetById
	movs r3, #2
	adds r0, #92
	strb r3, [r0]
	movs r1, #3
	movs r0, #15
	bl Object_SetModeById
	movs r5, #16
.L_02009902:
	adds r0, r5, #0
	movs r1, #1
	bl Object_SetModeById
	adds r0, r5, #0
	movs r1, #0
	adds r5, #1
	bl Object_SetModeById
	cmp r5, #23
	ble .L_02009902
	movs r0, #149
	lsls r0, r0, #4
	bl Func_020028d8
	cmp r0, #0
	beq .L_02009932
	movs r1, #130
	movs r2, #146
	movs r0, #25
	lsls r1, r1, #18
	lsls r2, r2, #18
	bl Func_020029d8
.L_02009932:
	movs r0, #196
	lsls r0, r0, #2
	bl GameFlag_GetByte
	adds r5, r0, #0
	movs r0, #198
	lsls r0, r0, #2
	bl GameFlag_GetByte
	cmp r0, #0
	beq .L_0200995a
	movs r3, #128
	lsls r3, r3, #12
	lsls r2, r0, #20
	lsls r1, r5, #20
	adds r1, r1, r3
	adds r2, r2, r3
	movs r0, #24
	bl Func_020029d8
.L_0200995a:
	bl Func_02002a68
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #32
	orrs r3, r2
	movs r1, #144
	strb r3, [r0]
	lsls r1, r1, #3
	ldr r0, .L_02009ad4
	bl Func_02002870
	movs r0, #28
	movs r1, #1
	bl Func_020029f0
	movs r1, #1
	movs r0, #24
	bl Func_020029f0
	bl Func_02002550
	bl Func_0200276c
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #104
	bl Func_020028d8
	cmp r0, #0
	beq .L_020099ca
	movs r0, #26
	bl Func_020025a8
	movs r0, #26
	bl Func_02002834
	movs r0, #5
	bl Func_02002948
	movs r0, #6
	bl Func_02002948
	movs r0, #7
	bl Func_02002940
	movs r0, #8
	bl Func_02002940
	movs r0, #26
	bl Object_GetById
	ldr r3, .L_02009ad8
	str r3, [r0, #108]
.L_020099ca:
	movs r1, #1
	movs r0, #26
	bl Func_020029f0
	ldr r3, .L_02009acc
	movs r1, #241
	lsls r1, r1, #1
	adds r7, r3, r1
	movs r2, #0
	ldrsh r3, [r7, r2]
	cmp r3, #5
	bne .L_020099e6
	bl Func_0200169c
.L_020099e6:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #85
	bl Func_020028d8
	cmp r0, #0
	beq .L_02009a6c
	movs r1, #196
	movs r2, #206
	movs r0, #17
	lsls r1, r1, #17
	lsls r2, r2, #18
	bl Func_020029d8
	movs r1, #212
	movs r2, #206
	movs r0, #18
	lsls r1, r1, #17
	lsls r2, r2, #18
	bl Func_020029d8
	movs r1, #228
	movs r2, #206
	movs r0, #19
	lsls r1, r1, #17
	lsls r2, r2, #18
	bl Func_020029d8
	movs r1, #244
	movs r2, #206
	movs r0, #20
	lsls r1, r1, #17
	lsls r2, r2, #18
	bl Func_020029d8
	movs r1, #130
	movs r2, #206
	movs r0, #21
	lsls r1, r1, #18
	lsls r2, r2, #18
	bl Func_020029d8
	movs r3, #24
	movs r2, #53
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #64
	movs r1, #66
	movs r2, #9
	movs r3, #3
	bl Func_02002960
	movs r3, #87
	movs r2, #50
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #7
	movs r0, #64
	movs r1, #66
	movs r2, #11
	bl Func_02002958
	movs r0, #21
	bl Object_GetById
	ldr r3, .L_02009adc
	str r3, [r0, #108]
.L_02009a6c:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #107
	bl Func_020028d8
	cmp r0, #0
	beq .L_02009ab8
	movs r3, #33
	movs r2, #41
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #62
	movs r1, #41
	movs r2, #1
	movs r3, #2
	bl Func_02002960
	movs r6, #96
	movs r5, #32
	movs r0, #116
	movs r1, #95
	movs r2, #3
	movs r3, #13
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl Func_02002968
	movs r0, #122
	movs r1, #95
	movs r2, #3
	movs r3, #13
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_02002968
	movs r0, #12
	bl Func_02001f60
.L_02009ab8:
	bl Func_02000844
	movs r1, #0
	ldrsh r3, [r7, r1]
	cmp r3, #4
	beq .L_02009ac6
	b .L_02009d20
.L_02009ac6:
	bl Func_02002158
	b .L_02009d20
.L_02009acc:
	.4byte gPartyState
.L_02009ad0:
	.4byte 0x0000009e
.L_02009ad4:
	.4byte Func_020013f8
.L_02009ad8:
	.4byte Func_02000b58
.L_02009adc:
	.4byte Func_02001670
.L_02009ae0:
	ldr r3, .L_02009c28
	cmp r6, r3
	beq .L_02009ae8
	b .L_02009d20
.L_02009ae8:
	movs r0, #14
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #145
	lsls r0, r0, #1
	adds r0, #255
	bl Func_020028d8
	cmp r0, #0
	beq .L_02009b16
	movs r3, #44
	movs r2, #32
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #42
	movs r1, #32
	movs r2, #1
	movs r3, #1
	bl Func_02002960
.L_02009b16:
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #34
	bl Func_020028d8
	cmp r0, #0
	beq .L_02009b38
	movs r3, #46
	movs r2, #32
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #48
	movs r1, #32
	movs r2, #1
	movs r3, #1
	bl Func_02002960
.L_02009b38:
	movs r0, #146
	lsls r0, r0, #1
	adds r0, #255
	bl Func_020028d8
	cmp r0, #0
	beq .L_02009b5a
	movs r3, #46
	movs r2, #44
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #44
	movs r1, #44
	movs r2, #1
	movs r3, #1
	bl Func_02002960
.L_02009b5a:
	movs r0, #12
	movs r1, #2
	bl Object_SetModeById
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #81
	bl Func_020028d8
	cmp r0, #0
	beq .L_02009b92
	movs r1, #214
	movs r2, #210
	movs r0, #12
	lsls r1, r1, #18
	lsls r2, r2, #18
	bl Func_020029d8
	movs r3, #53
	movs r2, #52
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #54
	movs r1, #58
	movs r2, #1
	movs r3, #1
	bl Func_02002960
.L_02009b92:
	movs r0, #13
	movs r1, #2
	bl Object_SetModeById
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #82
	bl Func_020028d8
	cmp r0, #0
	beq .L_02009bca
	movs r1, #214
	movs r2, #234
	movs r0, #13
	lsls r1, r1, #18
	lsls r2, r2, #18
	bl Func_020029d8
	movs r3, #53
	movs r2, #58
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #54
	movs r1, #58
	movs r2, #1
	movs r3, #1
	bl Func_02002960
.L_02009bca:
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r7, r2
	movs r1, #0
	ldrsh r2, [r3, r1]
	cmp r2, #5
	beq .L_02009bdc
	cmp r2, #7
	bne .L_02009c2c
.L_02009bdc:
	bl Func_02002550
	bl Func_0200276c
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #104
	bl Func_020028d8
	cmp r0, #0
	beq .L_02009c00
	movs r0, #14
	bl Func_020025a8
	movs r0, #14
	bl Func_02002834
	b .L_02009c18
.L_02009c00:
	movs r0, #7
	bl Func_02002948
	movs r0, #8
	bl Func_02002948
	movs r0, #5
	bl Func_02002940
	movs r0, #6
	bl Func_02002940
.L_02009c18:
	movs r0, #14
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	b .L_02009cf4
	.2byte 0x0000
.L_02009c28:
	.4byte 0x0000009f
.L_02009c2c:
	bl Func_02002550
	bl Func_0200276c
	movs r0, #8
	bl Object_GetById
	ldr r3, .L_02009cbc
	str r3, [r0, #108]
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #105
	bl Func_020028d8
	cmp r0, #0
	beq .L_02009cf4
	movs r0, #8
	bl Func_020025a8
	movs r0, #8
	bl Func_02002834
	movs r1, #132
	movs r2, #198
	lsls r2, r2, #18
	lsls r1, r1, #17
	movs r0, #15
	bl Func_020029d8
	movs r0, #15
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #15
	bl Object_GetById
	ldr r3, .L_02009cb8
	adds r0, #89
	strb r3, [r0]
	movs r0, #15
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #2
	orrs r3, r2
	strb r3, [r0]
	movs r0, #15
	bl Object_GetById
	ldr r3, .L_02009cc0
	movs r5, #16
	str r3, [r0, #108]
	movs r3, #48
	str r3, [sp, #4]
	movs r0, #15
	movs r1, #48
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	bl Func_02002960
	movs r3, #50
	str r3, [sp, #4]
	movs r0, #17
	movs r1, #50
	b .L_02009cc4
	.2byte 0x0000
.L_02009cb8:
	.4byte 0x00000000
.L_02009cbc:
	.4byte Func_02000b58
.L_02009cc0:
	.4byte Func_02001d98
.L_02009cc4:
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	bl Func_02002960
	movs r3, #94
	movs r5, #14
	str r3, [sp, #4]
	movs r0, #64
	movs r1, #68
	movs r2, #5
	movs r3, #13
	str r5, [sp, #0]
	bl Func_02002968
	movs r3, #34
	str r3, [sp, #4]
	movs r0, #64
	movs r1, #68
	movs r2, #5
	movs r3, #8
	str r5, [sp, #0]
	bl Func_02002960
.L_02009cf4:
	movs r0, #10
	adds r0, #255
	bl Func_020028d8
	cmp r0, #0
	bne .L_02009d20
	ldr r3, .L_02009d28
	movs r2, #241
	lsls r2, r2, #1
	adds r5, r3, r2
	movs r1, #0
	ldrsh r3, [r5, r1]
	cmp r3, #1
	bne .L_02009d14
	bl Func_02001460
.L_02009d14:
	movs r2, #0
	ldrsh r3, [r5, r2]
	cmp r3, #6
	bne .L_02009d20
	bl Func_0200153c
.L_02009d20:
	movs r0, #0
	add sp, #8
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009d28:
	.4byte gPartyState
	.section .text.x02009d2c,"ax",%progbits
	.global Func_02001d2c
	.thumb_func
Func_02001d2c:
	movs r0, #0
	bx lr
	.section .text.x02009d30,"ax",%progbits
	.global Func_02001d30
	.thumb_func
Func_02001d30:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r6, r0, #0
	ldr r2, [r6, #12]
	ldr r3, [r6, #16]
	movs r0, #128
	lsls r0, r0, #11
	adds r2, r2, r0
	adds r3, r3, r0
	ldr r1, [r6, #8]
	movs r0, #14
	bl Func_02002920
	ldr r2, [r6, #80]
	adds r5, r0, #0
	mov r8, r2
	cmp r5, #0
	beq .L_02009d8c
	ldr r3, [r6, #20]
	ldr r7, [r5, #80]
	str r3, [r5, #20]
	ldr r1, .L_02009d94
	bl Func_02002918
	adds r3, r5, #0
	adds r3, #85
	movs r5, #0
	strb r5, [r3]
	cmp r7, #0
	beq .L_02009d8c
	movs r1, #1
	adds r0, r7, #0
	bl Animation_ApplyChildArgument
	strb r5, [r7, #26]
	mov r2, r8
	ldrb r3, [r2, #9]
	ldrb r1, [r7, #9]
	movs r2, #12
	ands r2, r3
	movs r3, #13
	negs r3, r3
	ands r3, r1
	orrs r3, r2
	strb r3, [r7, #9]
.L_02009d8c:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009d94:
	.4byte Data_02002c44
	.section .text.x02009d98,"ax",%progbits
	.global Func_02001d98
	.thumb_func
Func_02001d98:
	push {r5, r6, r7, lr}
	ldr r3, .L_02009df4
	ldr r7, [r3]
	movs r3, #15
	ands r7, r3
	cmp r7, #0
	bne .L_02009df0
	ldr r1, [r0, #8]
	ldr r2, [r0, #12]
	ldr r3, [r0, #16]
	movs r0, #14
	adds r0, #255
	bl Func_02002920
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02009df0
	ldr r1, .L_02009df8
	ldr r6, [r5, #80]
	bl Func_02002918
	adds r3, r5, #0
	adds r3, #85
	strb r7, [r3]
	ldr r3, .L_02009dfc
	adds r2, r5, #0
	str r3, [r5, #12]
	adds r2, #34
	movs r3, #1
	strb r3, [r2]
	cmp r6, #0
	beq .L_02009df0
	adds r0, r6, #0
	movs r1, #2
	bl Animation_ApplyChildArgument
	ldrb r3, [r6, #9]
	movs r2, #13
	negs r2, r2
	ands r2, r3
	movs r3, #8
	orrs r2, r3
	strb r7, [r6, #26]
	strb r2, [r6, #9]
.L_02009df0:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009df4:
	.4byte Data_0300122c
.L_02009df8:
	.4byte Data_02002c50
.L_02009dfc:
	.4byte 0xfff88000
	.section .text.x02009e00,"ax",%progbits
	.global Func_02001e00
	.thumb_func
Func_02001e00:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r6, .L_02009f4c
	movs r3, #192
	lsls r3, r3, #18
	movs r0, #133
	ldr r2, [r3, #108]
	lsls r0, r0, #2
	adds r3, r6, r0
	movs r1, #230
	ldr r3, [r3]
	lsls r1, r1, #1
	adds r2, r2, r1
	ldr r2, [r2]
	mov r8, r3
	mov r0, r8
	mov r10, r2
	sub sp, #12
	bl Object_GetById
	movs r2, #240
	lsls r2, r2, #1
	adds r3, r6, r2
	adds r5, r0, #0
	movs r0, #0
	ldrsh r2, [r3, r0]
	ldr r3, .L_02009f50
	cmp r2, r3
	bne .L_02009e44
	ldr r3, [r5, #20]
	cmp r3, #0
	beq .L_02009f40
.L_02009e44:
	ldr r3, [r5, #8]
	mov r7, sp
	str r3, [r7]
	movs r1, #128
	ldr r3, [r5, #12]
	lsls r1, r1, #10
	str r3, [r7, #4]
	adds r0, r5, #0
	ldr r3, [r5, #16]
	adds r3, r3, r1
	str r3, [r7, #8]
	adds r1, r7, #0
	bl Func_02002970
	ldr r3, .L_02009f54
	movs r2, #4
	ldr r3, [r3]
	adds r6, r0, #0
	ands r3, r2
	cmp r3, #0
	bne .L_02009e74
	adds r0, r5, #0
	bl Func_02001d30
.L_02009e74:
	cmp r6, #0
	bge .L_02009ec6
	movs r1, #129
	mov r0, r8
	lsls r1, r1, #1
	bl Func_020029f8
	ldr r3, [r5, #16]
	movs r0, #128
	lsls r0, r0, #12
	adds r3, r3, r0
	ldr r1, [r5, #8]
	ldr r2, [r5, #12]
	adds r0, r5, #0
	bl Func_02002930
	adds r0, r5, #0
	movs r1, #49
	bl Func_02002910
	adds r0, r5, #0
	bl Func_02002938
.L_02009ea2:
	movs r0, #1
	bl WaitFrames
	ldr r2, [r5, #12]
	ldr r3, [r5, #20]
	cmp r2, r3
	bne .L_02009ea2
	adds r0, r5, #0
	bl Func_02001d30
	adds r0, r5, #0
	movs r1, #49
	bl Func_02002910
	movs r0, #3
	bl WaitFrames
	b .L_02009f40
.L_02009ec6:
	ldr r3, [r5, #8]
	movs r1, #128
	str r3, [r7]
	lsls r1, r1, #12
	ldr r3, [r5, #12]
	adds r0, r5, #0
	str r3, [r7, #4]
	ldr r3, [r5, #16]
	adds r3, r3, r1
	str r3, [r7, #8]
	adds r1, r7, #0
	bl Func_02002970
	adds r6, r0, #0
	cmp r6, #0
	bgt .L_02009f40
	ldr r3, [r5, #8]
	ldr r2, .L_02009f58
	adds r0, r5, #0
	adds r3, r3, r2
	str r3, [r7]
	adds r1, r7, #0
	ldr r3, [r5, #12]
	str r3, [r7, #4]
	ldr r3, [r5, #16]
	adds r3, r3, r2
	str r3, [r7, #8]
	bl Func_02002970
	adds r6, r0, #0
	cmp r6, #0
	bgt .L_02009f40
	ldr r3, [r5, #8]
	ldr r2, .L_02009f5c
	ldr r0, .L_02009f58
	adds r3, r3, r2
	str r3, [r7]
	adds r1, r7, #0
	ldr r3, [r5, #12]
	str r3, [r7, #4]
	ldr r3, [r5, #16]
	adds r3, r3, r0
	str r3, [r7, #8]
	adds r0, r5, #0
	bl Func_02002970
	adds r6, r0, #0
	cmp r6, #0
	bgt .L_02009f40
	mov r1, r10
	ldr r3, [r1, #16]
	movs r2, #128
	lsls r2, r2, #10
	adds r3, r3, r2
	str r3, [r1, #16]
	ldr r3, [r5, #16]
	adds r3, r3, r2
	str r3, [r5, #16]
	movs r3, #128
	lsls r3, r3, #7
	strh r3, [r5, #6]
.L_02009f40:
	add sp, #12
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009f4c:
	.4byte gPartyState
.L_02009f50:
	.4byte 0x0000009e
.L_02009f54:
	.4byte Data_0300122c
.L_02009f58:
	.4byte 0x0005b333
.L_02009f5c:
	.4byte 0xfffa4ccd
	.section .text.x02009f60,"ax",%progbits
	.global Func_02001f60
	.thumb_func
Func_02001f60:
	push {r5, lr}
	bl Object_GetById
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02009f8c
	movs r1, #126
	adds r1, #255
	ldr r0, [r5, #80]
	bl ResourceMetadata_Register
	movs r3, #0
	strb r3, [r0, #5]
	strb r3, [r0, #6]
	movs r1, #0
	adds r0, r5, #0
	bl Func_02002910
	adds r0, r5, #0
	movs r1, #2
	bl Func_02002910
.L_02009f8c:
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x02009f90,"ax",%progbits
	.global Func_02001f90
	.thumb_func
Func_02001f90:
	push {r5, r6, r7, lr}
	ldr r5, .L_0200a028
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
	adds r7, r0, #0
	ldr r0, [r5]
	bl Object_GetById
	adds r6, r0, #0
	bl Func_02002998
	movs r0, #0
	bl Func_02002a48
	ldr r0, [r5]
	movs r1, #1
	bl Func_020029f0
	adds r2, r6, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	adds r1, r6, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #32
	orrs r3, r2
	strb r3, [r1]
	movs r0, #215
	bl Func_02002a70
	adds r0, r6, #0
	movs r1, #18
	bl Func_02002910
	movs r0, #153
	lsls r0, r0, #2
	bl Func_02002a70
	movs r5, #0
.L_02009fe2:
	cmp r5, #30
	bne .L_02009fea
	bl Event_ClearStatus1c6
.L_02009fea:
	ldr r3, [r6, #12]
	ldr r2, .L_0200a02c
	adds r3, r3, r2
	str r3, [r6, #12]
	ldrh r3, [r6, #6]
	movs r2, #128
	lsls r2, r2, #5
	adds r3, r3, r2
	strh r3, [r6, #6]
	movs r3, #7
	ands r3, r5
	cmp r3, #0
	bne .L_0200a00e
	movs r0, #15
	bl Object_GetById
	bl Func_02001d30
.L_0200a00e:
	movs r0, #1
	adds r5, #1
	bl WaitFrames
	cmp r5, #59
	ble .L_02009fe2
	bl Func_020029a0
	adds r0, r7, #0
	bl Func_02002a18
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200a028:
	.4byte gPartyState
.L_0200a02c:
	.4byte 0xffffc000
	.section .text.x0200a030,"ax",%progbits
	.global Func_02002030
	.thumb_func
Func_02002030:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r3, .L_0200a0b8
	sub sp, #56
	ldr r2, [r3]
	mov r8, r3
	movs r3, #1
	ands r3, r2
	adds r7, r0, #0
	cmp r3, #0
	beq .L_0200a0ac
	movs r3, #7
	add r6, sp, #16
	str r3, [r6, #4]
	movs r3, #2
	ands r3, r2
	cmp r3, #0
	bne .L_0200a05a
	movs r3, #5
	str r3, [r6, #4]
.L_0200a05a:
	movs r3, #204
	lsls r3, r3, #8
	adds r3, #204
	movs r5, #0
	str r3, [r6, #8]
	str r3, [r6, #12]
	str r5, [r6]
	bl Random16Far
	lsls r0, r0, #3
	lsrs r0, r0, #16
	lsls r4, r0, #1
	adds r4, r4, r0
	lsls r3, r4, #4
	adds r4, r4, r3
	lsls r3, r4, #8
	adds r4, r4, r3
	mov r3, r8
	ldr r2, [r3]
	movs r3, #15
	ldr r0, [r7, #8]
	ands r2, r3
	movs r3, #8
	subs r3, r3, r2
	ldr r1, [r7, #12]
	lsls r3, r3, #16
	adds r0, r0, r3
	movs r3, #208
	lsls r3, r3, #13
	adds r1, r1, r3
	movs r3, #176
	lsls r3, r3, #12
	ldr r2, [r7, #16]
	negs r4, r4
	str r3, [sp, #8]
	movs r3, #0
	str r4, [sp, #0]
	str r5, [sp, #4]
	str r6, [sp, #12]
	bl Func_0200015c
.L_0200a0ac:
	movs r0, #0
	add sp, #56
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200a0b8:
	.4byte Data_0300122c
	.section .text.x0200a0bc,"ax",%progbits
	.global Func_020020bc
	.thumb_func
Func_020020bc:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r8
	push {r5, r6}
	ldr r5, .L_0200a150
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
	mov r10, r0
	ldr r0, [r5]
	bl Object_GetById
	adds r6, r0, #0
	bl Func_02002998
	movs r0, #0
	bl Func_02002a48
	movs r0, #228
	bl Func_02002a70
	ldr r3, .L_0200a154
	movs r2, #0
	str r3, [r6, #108]
	mov r8, r2
	adds r3, r6, #0
	mov r2, r8
	adds r3, #85
	strb r2, [r3]
	movs r3, #204
	lsls r3, r3, #6
	adds r3, #51
	str r3, [r6, #48]
	movs r1, #2
	ldr r0, [r5]
	bl Object_SetModeById
	movs r2, #8
	negs r2, r2
	movs r1, #0
	ldr r0, [r5]
	bl ObjectMotion_OffsetPositionAndResetMotion
	ldr r0, [r5]
	bl ObjectMotion_CommitCurrentPositionAndActivate
	ldr r0, [r5]
	bl Object_GetById
	movs r1, #9
	bl Func_020029e8
	ldr r0, [r5]
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	mov r3, r8
	str r3, [r6, #108]
	bl Event_ClearStatus1c6
	bl Event_WaitValue1c8Frames
	mov r0, r10
	bl Func_02002a18
	bl Func_020029a0
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200a150:
	.4byte gPartyState
.L_0200a154:
	.4byte Func_02002030
	.section .text.x0200a158,"ax",%progbits
	.global Func_02002158
	.thumb_func
Func_02002158:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r3, .L_0200a244
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r3, r2
	ldr r0, [r5]
	bl Object_GetById
	adds r6, r0, #0
	movs r0, #10
	adds r0, #255
	bl Func_020028d8
	adds r7, r0, #0
	cmp r7, #0
	bne .L_0200a23e
	bl Func_02002998
	movs r0, #0
	bl Func_02002a48
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r2, r2
	negs r0, r0
	negs r1, r1
	movs r3, #0
	bl Func_02002a08
	movs r3, #85
	adds r3, r3, r6
	strb r7, [r3]
	mov r8, r3
	movs r2, #10
	ldrsh r1, [r6, r2]
	movs r3, #18
	ldrsh r2, [r6, r3]
	ldr r3, .L_0200a248
	lsls r2, r2, #16
	adds r2, r2, r3
	lsls r1, r1, #16
	ldr r0, [r5]
	bl Func_020029d8
	ldr r0, [r5]
	bl Object_GetById
	movs r1, #9
	bl Func_020029e8
	ldr r0, [r5]
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	bl Event_SetStatus1c6
	movs r0, #228
	bl Func_02002a70
	ldr r3, .L_0200a24c
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #7
	lsls r2, r2, #6
	str r3, [r6, #108]
	ldr r0, [r5]
	adds r1, #102
	adds r2, #51
	bl ObjectMotion_SetSpeedParameters
	movs r2, #8
	movs r1, #0
	ldr r0, [r5]
	bl ObjectMotion_CommitPositionAndActivate
	ldr r0, [r5]
	bl Object_GetById
	movs r1, #0
	bl Func_020029e8
	ldr r0, [r5]
	bl Object_GetById
	movs r1, #1
	bl ObjectDispatch_SetSingleChildField26
	ldr r1, [r6, #80]
	movs r3, #13
	ldrb r2, [r1, #9]
	negs r3, r3
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	strb r3, [r1, #9]
	movs r2, #10
	ldr r0, [r5]
	movs r1, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r3, #3
	mov r2, r8
	strb r3, [r2]
	str r7, [r6, #108]
	bl Func_02002a60
	bl Event_WaitValue1c8Frames
	bl Func_020029a0
.L_0200a23e:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_0200a244:
	.4byte gPartyState
.L_0200a248:
	.4byte 0xfff00000
.L_0200a24c:
	.4byte Func_02002030
	.section .text.x0200a250,"ax",%progbits
	.global Func_02002250
	.thumb_func
Func_02002250:
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
	ldr r3, .L_0200a3ac
	adds r2, r1, #0
	adds r2, #228
	ldr r0, [r2]
	ldr r2, [r2, #4]
	ands r0, r3
	ands r2, r3
	ldr r3, [r1]
	ldr r7, .L_0200a3b0
	ldr r3, [r3, #4]
	mov r8, r2
	mov r9, r3
	ldrh r3, [r7, #4]
	ldr r2, .L_0200a3b4
	lsls r3, r3, #2
	adds r3, r3, r2
	ldrh r3, [r3, #2]
	mov r10, r0
	lsrs r3, r3, #5
	mov r11, r3
	bl Random16Far
	movs r3, #128
	lsls r3, r3, #5
	cmp r0, r3
	bcs .L_0200a2de
	ldr r6, [r7]
	cmp r6, #0
	beq .L_0200a2de
	ldrh r2, [r7, #6]
	lsls r3, r2, #3
	subs r3, r3, r2
	lsls r3, r3, #2
	adds r3, r3, r7
	adds r5, r3, #0
	bl Random16Far
	ldr r3, [r6, #8]
	lsls r2, r0, #1
	adds r2, r2, r0
	ldr r0, .L_0200a3b8
	lsls r2, r2, #4
	adds r3, r3, r2
	adds r5, #12
	adds r3, r3, r0
	str r3, [r5]
	movs r2, #0
	ldr r3, [r6, #12]
	movs r1, #252
	str r3, [r5, #4]
	lsls r1, r1, #14
	ldr r3, [r6, #16]
	str r2, [r5, #12]
	str r3, [r5, #8]
	ldrh r3, [r7, #6]
	adds r3, #1
	strh r3, [r7, #6]
	lsls r3, r3, #16
	cmp r3, r1
	bls .L_0200a2de
	strh r2, [r7, #6]
.L_0200a2de:
	adds r5, r7, #0
	adds r5, #12
	movs r6, #63
.L_0200a2e4:
	ldr r1, [r5]
	cmp r1, #0
	beq .L_0200a396
	ldr r2, [r5, #8]
	movs r0, #0
	bl Map_GetTerrainHeight
	ldr r3, [r5, #4]
	cmp r0, r3
	ble .L_0200a302
	movs r2, #128
	lsls r2, r2, #10
	adds r3, r3, r2
	str r3, [r5, #4]
	b .L_0200a30a
.L_0200a302:
	ldr r3, [r5, #8]
	ldr r0, .L_0200a3bc
	adds r3, r3, r0
	str r3, [r5, #8]
.L_0200a30a:
	ldr r3, [r5, #4]
	asrs r3, r3, #20
	cmp r3, #23
	bne .L_0200a316
	movs r3, #0
	str r3, [r5]
.L_0200a316:
	ldr r3, [r5, #8]
	asrs r3, r3, #20
	cmp r3, #48
	bne .L_0200a322
	movs r3, #0
	str r3, [r5]
.L_0200a322:
	ldr r3, [r5, #12]
	mov r1, r10
	adds r3, #1
	str r3, [r5, #12]
	ldr r3, [r5]
	mov r0, r8
	subs r4, r3, r1
	ldr r1, [r5, #8]
	ldr r3, [r5, #4]
	mov r2, r9
	subs r1, r1, r0
	subs r1, r1, r2
	subs r3, r3, r2
	subs r2, r1, r3
	adds r3, r3, r1
	asrs r3, r3, #16
	asrs r0, r4, #16
	adds r1, r3, #0
	movs r3, #167
	subs r4, r0, #4
	asrs r2, r2, #16
	adds r0, #11
	lsls r3, r3, #1
	subs r2, #4
	adds r1, #58
	cmp r0, r3
	bhi .L_0200a396
	movs r0, #16
	negs r0, r0
	cmp r2, r0
	ble .L_0200a396
	cmp r2, #239
	bgt .L_0200a396
	adds r3, #177
	ands r4, r3
	movs r3, #255
	ands r2, r3
	movs r3, #0
	str r3, [r5, #16]
	lsls r3, r4, #16
	orrs r2, r3
	movs r3, #128
	lsls r3, r3, #6
	orrs r2, r3
	ldr r3, [r5, #12]
	str r2, [r5, #20]
	movs r2, #3
	ands r3, r2
	lsls r3, r3, #1
	movs r2, #128
	add r3, r11
	lsls r2, r2, #3
	orrs r3, r2
	adds r0, r5, #0
	str r3, [r5, #24]
	adds r0, #16
	bl Func_020028d0
.L_0200a396:
	subs r6, #1
	adds r5, #28
	cmp r6, #0
	bge .L_0200a2e4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200a3ac:
	.4byte 0xffff0000
.L_0200a3b0:
	.4byte gOverlayArea + 0x34f0
.L_0200a3b4:
	.4byte ResourceTableEntries
.L_0200a3b8:
	.4byte 0xffe80000
.L_0200a3bc:
	.4byte 0xfffe0000
	.section .text.x0200a3c0,"ax",%progbits
	.global Func_020023c0
	.thumb_func
Func_020023c0:
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
	ldr r3, .L_0200a520
	adds r2, r1, #0
	adds r2, #228
	ldr r0, [r2]
	ldr r2, [r2, #4]
	ands r0, r3
	ands r2, r3
	ldr r3, [r1]
	ldr r7, .L_0200a524
	ldr r3, [r3, #4]
	mov r8, r2
	mov r9, r3
	ldrh r3, [r7, #4]
	ldr r2, .L_0200a528
	lsls r3, r3, #2
	adds r3, r3, r2
	ldrh r3, [r3, #2]
	mov r10, r0
	lsrs r3, r3, #5
	mov r11, r3
	bl Random16Far
	movs r3, #128
	lsls r3, r3, #5
	cmp r0, r3
	bcs .L_0200a44e
	ldr r6, [r7]
	cmp r6, #0
	beq .L_0200a44e
	ldrh r2, [r7, #6]
	lsls r3, r2, #3
	subs r3, r3, r2
	lsls r3, r3, #2
	adds r3, r3, r7
	adds r5, r3, #0
	bl Random16Far
	ldr r3, [r6, #8]
	lsls r2, r0, #1
	adds r2, r2, r0
	ldr r0, .L_0200a52c
	lsls r2, r2, #3
	adds r3, r3, r2
	adds r5, #12
	adds r3, r3, r0
	str r3, [r5]
	movs r2, #0
	ldr r3, [r6, #12]
	movs r1, #252
	str r3, [r5, #4]
	lsls r1, r1, #14
	ldr r3, [r6, #16]
	str r2, [r5, #12]
	str r3, [r5, #8]
	ldrh r3, [r7, #6]
	adds r3, #1
	strh r3, [r7, #6]
	lsls r3, r3, #16
	cmp r3, r1
	bls .L_0200a44e
	strh r2, [r7, #6]
.L_0200a44e:
	adds r5, r7, #0
	adds r5, #12
	movs r6, #63
.L_0200a454:
	ldr r1, [r5]
	cmp r1, #0
	beq .L_0200a50a
	ldr r2, [r5, #8]
	movs r0, #0
	bl Map_GetTerrainHeight
	ldr r3, [r5, #4]
	cmp r0, r3
	bge .L_0200a470
	ldr r2, .L_0200a530
	adds r3, r3, r2
	str r3, [r5, #4]
	b .L_0200a47a
.L_0200a470:
	ldr r3, [r5, #8]
	movs r0, #128
	lsls r0, r0, #10
	adds r3, r3, r0
	str r3, [r5, #8]
.L_0200a47a:
	ldr r3, [r5, #4]
	movs r1, #4
	asrs r3, r3, #20
	negs r1, r1
	cmp r3, r1
	bne .L_0200a48a
	movs r3, #0
	str r3, [r5]
.L_0200a48a:
	ldr r3, [r5, #8]
	asrs r3, r3, #20
	cmp r3, #50
	bne .L_0200a496
	movs r3, #0
	str r3, [r5]
.L_0200a496:
	ldr r3, [r5, #12]
	mov r2, r10
	adds r3, #1
	str r3, [r5, #12]
	ldr r3, [r5]
	ldr r1, [r5, #8]
	subs r4, r3, r2
	ldr r3, [r5, #4]
	mov r2, r8
	mov r0, r9
	subs r1, r1, r2
	subs r1, r1, r0
	subs r3, r3, r0
	subs r2, r1, r3
	adds r3, r3, r1
	asrs r3, r3, #16
	asrs r0, r4, #16
	adds r1, r3, #0
	movs r3, #167
	subs r4, r0, #4
	asrs r2, r2, #16
	adds r0, #11
	lsls r3, r3, #1
	subs r2, #4
	adds r1, #58
	cmp r0, r3
	bhi .L_0200a50a
	movs r0, #16
	negs r0, r0
	cmp r2, r0
	ble .L_0200a50a
	cmp r2, #239
	bgt .L_0200a50a
	adds r3, #177
	ands r4, r3
	movs r3, #255
	ands r2, r3
	movs r3, #0
	str r3, [r5, #16]
	lsls r3, r4, #16
	orrs r2, r3
	movs r3, #128
	lsls r3, r3, #6
	orrs r2, r3
	ldr r3, [r5, #12]
	str r2, [r5, #20]
	movs r2, #3
	ands r3, r2
	lsls r3, r3, #1
	movs r2, #128
	add r3, r11
	lsls r2, r2, #3
	orrs r3, r2
	adds r0, r5, #0
	str r3, [r5, #24]
	adds r0, #16
	bl Func_020028d0
.L_0200a50a:
	subs r6, #1
	adds r5, #28
	cmp r6, #0
	bge .L_0200a454
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200a520:
	.4byte 0xffff0000
.L_0200a524:
	.4byte gOverlayArea + 0x34f0
.L_0200a528:
	.4byte ResourceTableEntries
.L_0200a52c:
	.4byte 0xfff40000
.L_0200a530:
	.4byte 0xfffe0000
	.section .text.x0200a534,"ax",%progbits
	.global Func_02002534
	.thumb_func
Func_02002534:
	push {lr}
	ldr r3, .L_0200a54c
	movs r2, #8
	ldrsh r3, [r3, r2]
	cmp r3, #0
	ble .L_0200a546
	bl Func_02002250
	b .L_0200a54a
.L_0200a546:
	bl Func_020023c0
.L_0200a54a:
	pop {pc}
.L_0200a54c:
	.4byte gOverlayArea + 0x34f0
	.section .text.x0200a550,"ax",%progbits
	.global Func_02002550
	.thumb_func
Func_02002550:
	push {r5, r6, lr}
	ldr r6, .L_0200a598
	movs r1, #224
	lsls r1, r1, #3
	ldr r3, .L_0200a59c
	adds r1, #12
	adds r0, r6, #0
	mov lr, r3
	.2byte 0xf800
	movs r0, #128
	lsls r0, r0, #1
	bl Runtime_BumpAllocate
	adds r5, r0, #0
	adds r1, r5, #0
	ldr r0, .L_0200a5a0
	bl Func_020028b8
	bl Resource_FindFreeEntry
	strh r0, [r6, #4]
	movs r1, #128
	lsls r1, r1, #1
	adds r2, r5, #0
	ldrh r0, [r6, #4]
	bl VramBlock_LoadCached
	adds r0, r5, #0
	bl Sys_Free
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_0200a5a4
	bl Func_02002870
	pop {r5, r6, pc}
.L_0200a598:
	.4byte gOverlayArea + 0x34f0
.L_0200a59c:
	.4byte IwramClearWords
.L_0200a5a0:
	.4byte Data_02002c74
.L_0200a5a4:
	.4byte Func_02002534
	.section .text.x0200a5a8,"ax",%progbits
	.global Func_020025a8
	.thumb_func
Func_020025a8:
	push {r5, r6, lr}
	adds r5, r0, #0
	ldr r6, .L_0200a5d0
	cmp r5, #0
	bne .L_0200a5b6
	str r5, [r6]
	b .L_0200a5ce
.L_0200a5b6:
	adds r0, r5, #0
	bl Object_GetById
	str r0, [r6]
	cmp r5, #8
	bne .L_0200a5ca
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	b .L_0200a5cc
.L_0200a5ca:
	movs r3, #1
.L_0200a5cc:
	strh r3, [r6, #8]
.L_0200a5ce:
	pop {r5, r6, pc}
.L_0200a5d0:
	.4byte gOverlayArea + 0x34f0
	.section .text.x0200a5d4,"ax",%progbits
	.global Func_020025d4
	.thumb_func
Func_020025d4:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r2, #192
	lsls r2, r2, #18
	ldr r3, [r2, #32]
	ldr r2, [r2, #116]
	adds r3, #228
	ldr r1, [r3]
	ldr r3, [r3, #4]
	mov r8, r2
	mov r5, r8
	movs r2, #0
	adds r5, #8
	mov r11, r1
	mov r9, r3
	mov r10, r2
.L_0200a5fe:
	ldrh r3, [r5, #28]
	movs r1, #255
	lsls r1, r1, #8
	adds r1, #255
	adds r3, r3, r1
	adds r2, r1, #0
	ands r2, r3
	strh r3, [r5, #28]
	cmp r2, r1
	bne .L_0200a614
	b .L_0200a74a
.L_0200a614:
	movs r0, #179
	lsls r0, r0, #1
	bl Func_020028d8
	cmp r0, #0
	beq .L_0200a626
	ldrh r3, [r5, #28]
	adds r3, #1
	strh r3, [r5, #28]
.L_0200a626:
	ldrh r2, [r5, #28]
	mov r1, r11
	lsls r3, r2, #2
	adds r3, r3, r2
	ldr r2, .L_0200a6d4
	lsls r3, r3, #1
	adds r4, r3, r2
	ldr r3, [r5, #12]
	subs r2, r3, r1
	cmp r2, #0
	bge .L_0200a644
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	adds r2, r2, r3
.L_0200a644:
	movs r1, #0
	ldrsh r3, [r4, r1]
	asrs r2, r2, #16
	adds r7, r2, r3
	ldr r2, [r5, #16]
	ldr r3, [r5, #20]
	adds r4, #2
	subs r3, r3, r2
	mov r2, r9
	subs r3, r3, r2
	cmp r3, #0
	bge .L_0200a664
	movs r1, #255
	lsls r1, r1, #8
	adds r1, #255
	adds r3, r3, r1
.L_0200a664:
	movs r1, #0
	ldrsh r2, [r4, r1]
	asrs r3, r3, #16
	adds r6, r3, r2
	adds r3, r7, #0
	adds r3, #16
	adds r4, #2
	cmp r3, #255
	bhi .L_0200a6f4
	movs r2, #32
	negs r2, r2
	cmp r6, r2
	blt .L_0200a6f4
	cmp r6, #159
	bgt .L_0200a6f4
	ldrb r3, [r5, #9]
	movs r1, #13
	negs r1, r1
	adds r2, r1, #0
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	strb r3, [r5, #9]
	ldr r3, .L_0200a6c4
	ldr r2, .L_0200a6c8
	ands r7, r3
	ldrh r3, [r5, #6]
	strb r6, [r5, #4]
	ands r3, r2
	orrs r3, r7
	strh r3, [r5, #6]
	mov r2, r8
	ldrh r3, [r4]
	ldr r1, [r2, #4]
	ldr r2, .L_0200a6cc
	adds r1, r1, r3
	ldr r3, .L_0200a6d0
	adds r4, #2
	ands r1, r3
	ldrh r3, [r5, #8]
	ldrb r0, [r5, #5]
	ands r3, r2
	orrs r3, r1
	strh r3, [r5, #8]
	movs r2, #63
	ldrb r1, [r4]
	adds r3, r2, #0
	b .L_0200a6d8
.L_0200a6c4:
	.4byte 0x000001ff
.L_0200a6c8:
	.4byte 0xfffffe00
.L_0200a6cc:
	.4byte 0xfffffc00
.L_0200a6d0:
	.4byte 0x000003ff
.L_0200a6d4:
	.4byte Data_02002cd4
.L_0200a6d8:
	lsls r1, r1, #6
	ands r3, r0
	orrs r3, r1
	strb r3, [r5, #5]
	ldrb r1, [r5, #7]
	ldrb r3, [r4, #2]
	ands r2, r1
	lsls r3, r3, #6
	orrs r2, r3
	strb r2, [r5, #7]
	adds r0, r5, #0
	movs r1, #240
	bl Func_020028d0
.L_0200a6f4:
	ldrh r3, [r5, #28]
	cmp r3, #0
	bne .L_0200a74a
	movs r3, #128
	lsls r3, r3, #4
	adds r3, #8
	add r3, r8
	ldr r6, [r3]
	cmp r6, #0
	beq .L_0200a740
	bl Random16Far
	ldr r3, [r6]
	lsls r2, r0, #4
	ldr r1, .L_0200a764
	subs r2, r2, r0
	lsls r2, r2, #4
	adds r3, r3, r2
	adds r7, r3, r1
	bl Random16Far
	ldr r3, [r6, #8]
	lsls r2, r0, #2
	adds r2, r2, r0
	lsls r2, r2, #5
	adds r3, r3, r2
	ldr r2, .L_0200a768
	str r7, [r5, #12]
	adds r6, r3, r2
	str r6, [r5, #20]
	movs r0, #0
	adds r1, r7, #0
	adds r2, r6, #0
	bl Map_GetTerrainHeight
	movs r3, #16
	str r0, [r5, #16]
	b .L_0200a748
.L_0200a740:
	movs r3, #16
	str r6, [r5, #12]
	str r6, [r5, #20]
	str r6, [r5, #16]
.L_0200a748:
	strh r3, [r5, #28]
.L_0200a74a:
	movs r3, #1
	add r10, r3
	mov r1, r10
	adds r5, #32
	cmp r1, #63
	bhi .L_0200a758
	b .L_0200a5fe
.L_0200a758:
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0200a764:
	.4byte 0xff880000
.L_0200a768:
	.4byte 0xffb00000
	.section .text.x0200a76c,"ax",%progbits
	.global Func_0200276c
	.thumb_func
Func_0200276c:
	push {r5, r6, r7, lr}
	movs r1, #128
	lsls r1, r1, #4
	adds r1, #20
	movs r0, #116
	sub sp, #8
	bl Runtime_AllocateBlock
	movs r3, #128
	adds r5, r0, #0
	movs r0, #0
	str r0, [sp, #0]
	adds r7, r5, #0
	add r0, sp, #4
	movs r1, #0
	lsls r3, r3, #19
	str r1, [r0]
	adds r7, #8
	adds r3, #212
	adds r1, r5, #0
	ldr r2, .L_0200a820
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r1, #128
	lsls r1, r1, #3
	movs r0, #56
	bl Runtime_AllocateBlock
	adds r6, r0, #0
	adds r1, r6, #0
	ldr r0, .L_0200a824
	bl Func_020028b0
	bl Resource_FindFreeEntry
	movs r1, #192
	str r0, [r5]
	lsls r1, r1, #2
	adds r2, r6, #0
	bl VramBlock_LoadCached
	str r0, [r5, #4]
	movs r0, #56
	bl Runtime_ReleaseHeapBlock
	movs r3, #128
	lsls r3, r3, #4
	ldr r0, [sp, #0]
	adds r3, #8
	adds r5, r5, r3
	str r0, [r5]
	movs r5, #0
.L_0200a7d4:
	movs r2, #0
	adds r3, r7, #0
	str r7, [sp, #0]
	stmia r3!, {r2}
	adds r1, r3, #0
	ldr r3, .L_0200a828
	stmia r1!, {r3}
	movs r3, #180
	adds r0, r1, #0
	lsls r3, r3, #8
	str r0, [sp, #0]
	str r3, [r1]
	movs r0, #0
	str r2, [r7, #12]
	str r2, [r7, #20]
	movs r1, #0
	bl Map_GetTerrainHeight
	ldr r2, .L_0200a81c
	adds r3, r5, #0
	ands r3, r2
	lsls r0, r0, #16
	adds r3, #1
	adds r5, #1
	str r0, [r7, #16]
	strh r3, [r7, #28]
	adds r7, #32
	cmp r5, #63
	bls .L_0200a7d4
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_0200a82c
	bl Func_02002870
	add sp, #8
	b .L_0200a830
.L_0200a81c:
	.4byte 0x0000000f
.L_0200a820:
	.4byte 0x85000205
.L_0200a824:
	.4byte Data_02003388
.L_0200a828:
	.4byte 0x40000400
.L_0200a82c:
	.4byte Func_020025d4
.L_0200a830:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x0200a834,"ax",%progbits
	.global Func_02002834
	.thumb_func
Func_02002834:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #116]
	adds r6, r0, #0
	bl Object_GetById
	movs r3, #128
	lsls r3, r3, #4
	adds r3, #8
	adds r5, r5, r3
	movs r3, #0
	str r3, [r5]
	cmp r6, #0
	beq .L_0200a85c
	cmp r0, #0
	beq .L_0200a85c
	adds r3, r0, #0
	adds r3, #8
	str r3, [r5]
.L_0200a85c:
	pop {r5, r6, pc}
	.2byte 0x0000
	.section .rodata.x0200aa78,"a",%progbits
.L_0200aa78:
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
.L_0200aab4:
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
.L_0200aaf0:
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
.L_0200ab2c:
	.4byte 0x0000002e
	.4byte Func_02000478
	.4byte 0x80010000
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x0000002e
	.4byte Func_02000384
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000078
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x0000002e
	.4byte Func_02000384
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x0000002e
	.4byte Func_02000384
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x000000b4
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0xc0010000
	.4byte 0x00000011
.L_0200abb4:
	.4byte 0x0000002e
	.4byte Func_02000478
	.4byte 0x80010000
	.4byte 0x00000000
	.4byte 0x0000003c
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x0000002e
	.4byte Func_02000384
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000078
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x0000002e
	.4byte Func_02000384
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0000000f
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x0000002e
	.4byte Func_02000384
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x000000b4
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0xc0010000
	.4byte 0x00000011
	.global Data_02002c44
Data_02002c44:
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000026
	.global Data_02002c50
Data_02002c50:
	.4byte 0x00000017
	.4byte 0x00000006
	.4byte 0x00004000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000001e
	.4byte 0x00000000
	.4byte 0x00000026
	.global Data_02002c74
Data_02002c74:
	.4byte 0x7e080100
	.4byte 0x82e00afd
	.4byte 0xd35d1389
	.4byte 0x855a6d2b
	.4byte 0xfda62705
	.4byte 0xc4ed21f0
	.4byte 0xa9eea465
	.4byte 0xa82af7e3
	.4byte 0xa5e3d158
	.4byte 0xfc79e5e9
	.4byte 0xfe9e5e82
	.4byte 0x3ccb0e0c
	.4byte 0xdcbf3bae
	.4byte 0x5eaee6e3
	.4byte 0x80a3af17
	.4byte 0x5e3e0a7c
	.4byte 0x100fcf2f
	.4byte 0x4401362e
	.4byte 0x5cfd9b8f
	.4byte 0x71f039f8
	.4byte 0xe6075e04
	.4byte 0xf3783dc2
	.4byte 0x3ec75e86
	.4byte 0x0007c083
	.global Data_02002cd4
Data_02002cd4:
	.4byte 0xfff4fff8
	.4byte 0x00000014
	.4byte 0xfff80001
	.4byte 0x0014fff4
	.4byte 0x00010000
	.4byte 0xfff4fff8
	.4byte 0x00000014
	.4byte 0xfff80001
	.4byte 0x0010fff4
	.4byte 0x00010000
	.4byte 0xfff4fff8
	.4byte 0x00000010
	.4byte 0xfff80001
	.4byte 0x000cfff4
	.4byte 0x00010000
	.4byte 0xfff4fff8
	.4byte 0x00000008
	.4byte 0x00000001
	.4byte 0x0000ffe0
	.4byte 0x00020002
	.4byte 0xffd00008
	.4byte 0x00020000
	.4byte 0x00100002
	.4byte 0x0000ffc0
	.4byte 0x00020002
	.4byte 0xffb00018
	.4byte 0x00020000
	.4byte 0x00200002
	.4byte 0x0000ffa0
	.4byte 0x00020002
	.4byte 0xff900028
	.4byte 0x00020000
	.4byte 0x00300002
	.4byte 0x0000ff80
	.4byte 0x00020002
	.4byte 0xff700038
	.4byte 0x00020000
	.4byte 0x00400002
	.4byte 0x0000ff60
	.4byte 0x00020002
	.4byte 0x000cfffe
	.4byte 0x000cfffc
	.4byte 0x000cfffa
	.4byte 0x0008fff8
	.4byte 0x0008fff6
	.4byte 0x0008fff4
	.4byte 0x0008fff2
	.4byte 0x0008fff0
	.4byte 0x0004ffed
	.4byte 0x0004ffeb
	.4byte 0x0004ffe8
	.4byte 0x0004ffe5
	.4byte 0x0004ffe2
	.4byte 0x0004ffdf
	.4byte 0x0004ffdc
	.4byte 0x0004ffd8
	.4byte 0x0004ffd4
	.4byte 0x0000ffd0
	.4byte 0x0000ffcc
	.4byte 0x0000ffc8
	.4byte 0x0000ffc4
	.4byte 0x0000ffc0
	.4byte 0x0000ffbc
	.4byte 0x0000ffb8
	.4byte 0x0000ffb4
	.4byte 0x0000ffb0
	.4byte 0x0000ffab
	.4byte 0x0000ffa6
	.4byte 0x0000ffa1
	.4byte 0x0000ff9c
	.4byte 0x0000ff92
	.4byte 0x0000ff88
	.global Data_02002df4
Data_02002df4:
	.4byte .L_0200aa78
	.4byte .L_0200aab4
	.4byte .L_0200aaf0
	.global Data_02002e00
Data_02002e00:
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
	.global Data_02002e30
Data_02002e30:
	.4byte 0x01a800b0
	.4byte 0x00c00220
	.4byte 0x023001b8
	.4byte 0x0002ffff
	.4byte 0x01a80110
	.4byte 0x01200220
	.4byte 0x023001b8
	.4byte 0x0003ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02002e60
Data_02002e60:
	.4byte 0xffc80030
	.4byte 0x004003c0
	.4byte 0x03d0ffd8
	.4byte 0x0002ffff
	.4byte 0xffc80090
	.4byte 0x00a003c0
	.4byte 0x03d0ffd8
	.4byte 0x0003ffff
	.4byte 0xff9400ec
	.4byte 0x010403bc
	.4byte 0x03d4ffac
	.4byte 0x0007ffff
	.4byte 0xff94010c
	.4byte 0x012403bc
	.4byte 0x03d4ffac
	.4byte 0x0007ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02002eb0
Data_02002eb0:
	.4byte 0x0000009e
	.4byte 0x00125002
	.4byte 0x0020209f
	.4byte 0x0030309f
	.4byte 0x004010a3
	.4byte 0x0060609f
	.4byte 0x04d0109a
	.4byte 0x0000009f
	.4byte 0x0010d0a5
	.4byte 0x0020209e
	.4byte 0x0030309e
	.4byte 0x0040509f
	.4byte 0x0050409f
	.4byte 0x0070709e
	.4byte 0x000001ff
	.global Data_02002eec
Data_02002eec:
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02002f04
Data_02002f04:
	.4byte 0xffff017c
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x02270000
	.4byte 0x00024000
	.4byte 0xffff017c
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x02270000
	.4byte 0x00024000
	.4byte 0xffff017c
	.4byte 0x00000001
	.4byte 0x02880000
	.4byte 0x00000000
	.4byte 0x02670000
	.4byte 0x00024000
	.4byte 0xffff017c
	.4byte 0x00000001
	.4byte 0x02c80000
	.4byte 0x00000000
	.4byte 0x02670000
	.4byte 0x00024000
	.4byte 0xffff017c
	.4byte 0x00000001
	.4byte 0x02180000
	.4byte 0x00000000
	.4byte 0x02870000
	.4byte 0x00024000
	.4byte 0xffff017c
	.4byte 0x00000001
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x02870000
	.4byte 0x00024000
	.4byte 0xffff017c
	.4byte 0x00000001
	.4byte 0x00380000
	.4byte 0x00000000
	.4byte 0x02a70000
	.4byte 0x00024000
	.4byte 0xffff017c
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x03370000
	.4byte 0x00024000
	.4byte 0xffff017c
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x039f0000
	.4byte 0x01024000
	.4byte 0xffff017c
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x036f0000
	.4byte 0x01024000
	.4byte 0xffff017c
	.4byte 0x00000001
	.4byte 0x01a80000
	.4byte 0x00000000
	.4byte 0x035f0000
	.4byte 0x01024000
	.4byte 0xffff017c
	.4byte 0x00000001
	.4byte 0x01c80000
	.4byte 0x00000000
	.4byte 0x034f0000
	.4byte 0x01024000
	.4byte 0xffff017c
	.4byte 0x00000001
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x035f0000
	.4byte 0x01024000
	.4byte 0xffff017c
	.4byte 0x00000001
	.4byte 0x02080000
	.4byte 0x00000000
	.4byte 0x036f0000
	.4byte 0x01024000
	.4byte 0xffff017c
	.4byte 0x00000001
	.4byte 0x02c80000
	.4byte 0x00000000
	.4byte 0x034f0000
	.4byte 0x01024000
	.4byte 0xffff017c
	.4byte 0x00000001
	.4byte 0x03480000
	.4byte 0x00000000
	.4byte 0x03bf0000
	.4byte 0x01024000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x02280000
	.4byte 0x00024000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x02180000
	.4byte 0x00000000
	.4byte 0x02480000
	.4byte 0x00024000
	.4byte 0xffff017f
	.4byte 0x00000001
	.4byte 0x03480000
	.4byte 0x00000000
	.4byte 0x02580000
	.4byte 0x00024000
	.4byte 0xffff0191
	.4byte .L_0200ab2c
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x03c00000
	.4byte 0x00024000
	.4byte 0xffff0191
	.4byte .L_0200ab2c
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x03880000
	.4byte 0x00024000
	.4byte 0xffff0191
	.4byte .L_0200abb4
	.4byte 0x00900000
	.4byte 0x00000000
	.4byte 0x03900000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_0200312c
Data_0200312c:
	.4byte 0xffff017f
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x02480000
	.4byte 0x00024000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x02c80000
	.4byte 0x00000000
	.4byte 0x02080000
	.4byte 0x00024000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x02e80000
	.4byte 0x00000000
	.4byte 0x02080000
	.4byte 0x00024000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x02e80000
	.4byte 0x00000000
	.4byte 0x02c80000
	.4byte 0x00024000
	.4byte 0xffff0124
	.4byte 0x00000001
	.4byte 0x03480000
	.4byte 0x00000000
	.4byte 0x03480000
	.4byte 0x00024000
	.4byte 0xffff0124
	.4byte 0x00000001
	.4byte 0x03380000
	.4byte 0x00000000
	.4byte 0x03a80000
	.4byte 0x00024000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x02c80000
	.4byte 0x00000000
	.4byte 0x03a80000
	.4byte 0x00024000
	.4byte 0xffff01e9
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
	.global Data_02003204
Data_02003204:
	.4byte 0x00000000
	.global Data_02003208
Data_02003208:
	.4byte 0x00000000
	.global Data_0200320c
Data_0200320c:
	.4byte 0x00000000
	.global Data_02003210
Data_02003210:
	.4byte 0x00000000
	.global Data_02003214
Data_02003214:
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02003220
Data_02003220:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x0000c402
	.4byte 0xffff0004
	.4byte Func_020020bc
	.4byte 0x00000002
	.4byte 0x0320001e
	.4byte Func_02000f8c
	.4byte 0x00000002
	.4byte 0x0321001f
	.4byte Func_02000f8c
	.4byte 0x00000002
	.4byte 0xffff0020
	.4byte Func_02000f8c
	.4byte 0x00000002
	.4byte 0xffff0021
	.4byte Func_02000f8c
	.4byte 0x00000002
	.4byte 0xffff0028
	.4byte Func_02001e00
	.4byte 0x00000002
	.4byte 0xffff0029
	.4byte Func_02000d80
	.4byte 0x00000006
	.4byte 0xffff002c
	.4byte Func_0200106c
	.4byte 0x00002115
	.4byte 0x0a6b000c
	.4byte Func_02001010
	.4byte 0x00002115
	.4byte 0x0a68001a
	.4byte Func_02000b68
	.4byte 0x00008c15
	.4byte 0xffff0018
	.4byte Func_02000cd8
	.4byte 0x00008c15
	.4byte 0x09500019
	.4byte Func_02000cd8
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte Func_02000cd8
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020032d4
Data_020032d4:
	.4byte 0x00000002
	.4byte 0xffff0001
	.4byte Func_02001f90
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
	.4byte 0x00000002
	.4byte 0xffff0028
	.4byte Func_02001e00
	.4byte 0x00000002
	.4byte 0xffff0029
	.4byte Func_02000d80
	.4byte 0x00001815
	.4byte 0x02210009
	.4byte Func_020010f4
	.4byte 0x00001815
	.4byte 0x0222000a
	.4byte Func_0200112c
	.4byte 0x00001815
	.4byte 0x0223000b
	.4byte Func_02001164
	.4byte 0x00008c15
	.4byte 0x0951000c
	.4byte Func_0200119c
	.4byte 0x00008c15
	.4byte 0x0952000d
	.4byte Func_020011e0
	.4byte 0x00000009
	.4byte 0x09520000
	.4byte Func_020011e0
	.4byte 0x00002115
	.4byte 0x0a690008
	.4byte Func_02001224
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02003388
Data_02003388:
	.4byte 0x01000057
	.4byte 0x04061011
	.4byte 0x0e040501
	.4byte 0x0f01000f
	.4byte 0x0406205d
	.4byte 0x0e040502
	.4byte 0x3001030f
	.4byte 0x03750406
	.4byte 0x0f0e0405
	.4byte 0x400f0100
	.4byte 0x05040406
	.4byte 0x0f0ef504
	.4byte 0x031e0100
	.4byte 0x0397049a
	.4byte 0x06300702
	.4byte 0x0109f54b
	.4byte 0x1d03f30a
	.4byte 0x0a400a06
	.4byte 0x100240ad
	.4byte Func_08023f9c + 0x4c0
	.4byte 0x021f0843
	.4byte 0x2049044e
	.4byte 0x03b92205
	.4byte 0xd700341d
	.4byte 0x106b1612
	.4byte Data_02020202 + 0x105f
	.4byte 0x30555e02
	.4byte 0x02245a02
	.4byte 0x561644bf
	.4byte 0x531f0833
	.4byte 0x0f46160f
	.4byte 0x00237919
	.4byte 0x0016f610
	.4byte 0x22621527
	.4byte 0x63010302
	.4byte 0x04302010
	.4byte 0x03ea11f8
	.4byte 0x0b1302ef
	.4byte 0x7b031205
	.4byte 0x30901411
	.4byte 0x12d303b5
	.4byte 0x01001806
	.4byte 0x8f261134
	.4byte 0xdbc31820
	.4byte 0x3c14bb04
	.4byte 0x161f0401
	.4byte 0x712701b5
	.4byte 0x01701d02
	.4byte 0x07012c05
	.2byte 0x0000
