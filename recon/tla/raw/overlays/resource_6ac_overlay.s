.syntax unified
	.thumb
	.section .text.x0200804c,"ax",%progbits
	.global Func_0200004c
	.thumb_func
Func_0200004c:
	push {lr}
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #0
	pop {pc}
	.section .text.x02008058,"ax",%progbits
	.global Func_02000058
	.thumb_func
Func_02000058:
	push {r5, r6, lr}
	adds r6, r1, #0
	bl Object_GetById
	adds r5, r0, #0
	adds r2, r5, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	movs r3, #224
	lsls r3, r3, #12
	str r3, [r5, #12]
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	adds r0, r5, #0
	adds r1, r6, #0
	bl Func_02003118
	pop {r5, r6, pc}
	.section .text.x02008080,"ax",%progbits
	.global Func_02000080
	.thumb_func
Func_02000080:
	push {lr}
	ldr r3, .L_020080a8
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_020080ac
	cmp r2, r3
	bne .L_02008098
	ldr r0, .L_020080b0
	b .L_020080a4
.L_02008098:
	ldr r3, .L_020080b4
	cmp r2, r3
	bne .L_020080a2
	ldr r0, .L_020080b8
	b .L_020080a4
.L_020080a2:
	ldr r0, .L_020080bc
.L_020080a4:
	pop {pc}
	.2byte 0x0000
.L_020080a8:
	.4byte gPartyState
.L_020080ac:
	.4byte 0x00000128
.L_020080b0:
	.4byte Data_02003508
.L_020080b4:
	.4byte 0x0000011f
.L_020080b8:
	.4byte Data_02003598
.L_020080bc:
	.4byte Data_020034f0
	.section .text.x020080c0,"ax",%progbits
	.global Func_020000c0
	.thumb_func
Func_020000c0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, .L_020082a0
	movs r1, #241
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	movs r3, #192
	lsls r3, r3, #18
	mov r11, r2
	ldr r2, [r3, #108]
	movs r1, #179
	lsls r1, r1, #1
	adds r3, r2, r1
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	beq .L_020080f2
	b .L_02008292
.L_020080f2:
	movs r1, #192
	lsls r1, r1, #4
	adds r1, #164
	adds r3, r2, r1
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	mov r10, r3
	cmp r3, #0
	beq .L_02008108
	b .L_02008292
.L_02008108:
	ldr r7, .L_020082a4
	movs r2, #1
	ldr r3, [r7]
	negs r2, r2
	cmp r3, r2
	bne .L_02008140
	ldr r1, .L_020082a8
	movs r2, #32
	ldr r3, .L_020082ac
	ldr r0, .L_020082b0
	mov lr, r3
	.2byte 0xf800
	ldr r1, .L_020082b4
	movs r2, #64
	mov r8, r1
	ldr r3, .L_020082ac
	ldr r1, .L_020082b8
	mov r0, r8
	mov lr, r3
	.2byte 0xf800
	mov r1, r8
	ldr r0, .L_020082bc
	movs r2, #64
	ldr r3, .L_020082ac
	mov lr, r3
	.2byte 0xf800
	mov r1, r10
	str r1, [r7]
.L_02008140:
	ldr r0, [r7]
	cmp r0, #0
	bge .L_02008148
	adds r0, #7
.L_02008148:
	movs r1, #6
	asrs r0, r0, #3
	bl __modsi3
	movs r2, #32
	mov r9, r0
	ldr r1, .L_020082b0
	ldr r0, .L_020082bc
	ldr r3, .L_020082ac
	mov lr, r3
	.2byte 0xf800
	mov r2, r11
	cmp r2, #1
	bne .L_0200819e
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #50
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200819e
	movs r3, #20
	movs r7, #0
	mov r8, r3
.L_02008178:
	ldr r1, .L_020082b4
	mov r2, r9
	adds r0, r2, r7
	mov r10, r1
	movs r1, #6
	bl __modsi3
	lsls r0, r0, #1
	mov r1, r10
	adds r0, #20
	ldrh r3, [r1, r0]
	ldr r2, .L_020082bc
	mov r1, r8
	strh r3, [r2, r1]
	adds r7, #1
	movs r2, #2
	add r8, r2
	cmp r7, #5
	ble .L_02008178
.L_0200819e:
	mov r3, r11
	cmp r3, #2
	bne .L_020081de
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #51
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020081de
	movs r1, #20
	movs r7, #0
	mov r8, r1
.L_020081b8:
	ldr r2, .L_020082b4
	mov r3, r9
	adds r0, r3, r7
	movs r1, #6
	mov r10, r2
	bl __modsi3
	lsls r0, r0, #1
	mov r1, r10
	adds r0, #40
	ldrh r3, [r1, r0]
	ldr r2, .L_020082bc
	mov r1, r8
	strh r3, [r2, r1]
	adds r7, #1
	movs r2, #2
	add r8, r2
	cmp r7, #5
	ble .L_020081b8
.L_020081de:
	mov r3, r11
	cmp r3, #3
	bne .L_0200821c
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #52
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200821c
	movs r7, #0
	movs r5, #20
.L_020081f6:
	ldr r1, .L_020082bc
	ldr r2, .L_020082b4
	mov r3, r9
	adds r0, r3, r7
	mov r10, r1
	movs r1, #6
	mov r8, r2
	bl __modsi3
	lsls r0, r0, #1
	adds r0, #52
	mov r1, r8
	ldrh r3, [r1, r0]
	mov r2, r10
	adds r7, #1
	strh r3, [r2, r5]
	adds r5, #2
	cmp r7, #5
	ble .L_020081f6
.L_0200821c:
	mov r3, r11
	cmp r3, #4
	bne .L_0200825a
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #53
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200825a
	movs r7, #0
	movs r6, #20
.L_02008234:
	ldr r1, .L_020082bc
	ldr r2, .L_020082b4
	mov r3, r9
	adds r0, r3, r7
	mov r10, r1
	movs r1, #6
	mov r8, r2
	bl __modsi3
	lsls r0, r0, #1
	adds r0, #8
	mov r1, r8
	ldrh r3, [r1, r0]
	mov r2, r10
	adds r7, #1
	strh r3, [r2, r6]
	adds r6, #2
	cmp r7, #5
	ble .L_02008234
.L_0200825a:
	ldr r1, .L_020082c0
	ldr r0, .L_020082c4
	ldrh r3, [r0]
	adds r4, r3, #0
	strh r0, [r0]
	ldrh r2, [r1]
	cmp r2, #31
	bgt .L_02008288
	lsls r3, r2, #1
	adds r3, r3, r2
	adds r2, #1
	lsls r3, r3, #2
	strh r2, [r1]
	ldr r2, .L_020082bc
	adds r3, r3, r1
	adds r3, #4
	stmia r3!, {r2}
	ldr r2, .L_020082a8
	stmia r3!, {r2}
	movs r2, #132
	lsls r2, r2, #24
	adds r2, #8
	str r2, [r3]
.L_02008288:
	strh r4, [r0]
	ldr r2, .L_020082a4
	ldr r3, [r2]
	adds r3, #1
	str r3, [r2]
.L_02008292:
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_020082a0:
	.4byte gPartyState
.L_020082a4:
	.4byte Data_020036c4
.L_020082a8:
	.4byte 0x050001a0
.L_020082ac:
	.4byte IwramCopyWords
.L_020082b0:
	.4byte Data_020039bc
.L_020082b4:
	.4byte Data_0200393c
.L_020082b8:
	.4byte 0x05000100
.L_020082bc:
	.4byte Data_0200397c
.L_020082c0:
	.4byte gIoWriteQueue
.L_020082c4:
	.4byte 0x04000208
	.section .text.x020082c8,"ax",%progbits
	.global Func_020002c8
	.thumb_func
Func_020002c8:
	push {lr}
	movs r0, #123
	bl Func_020032b8
	ldr r3, .L_020082e4
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r0, [r3, r2]
	bl Func_02003238
	pop {pc}
	.2byte 0x0000
.L_020082e4:
	.4byte gPartyState
	.section .text.x020082e8,"ax",%progbits
	.global Func_020002e8
	.thumb_func
Func_020002e8:
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #208
	lsls r2, r2, #4
	adds r2, #56
	adds r3, r3, r2
	movs r2, #1
	strb r2, [r3]
	bx lr
	.section .text.x020082fc,"ax",%progbits
	.global Func_020002fc
	.thumb_func
Func_020002fc:
	push {lr}
	movs r0, #10
	movs r1, #1
	bl Func_02003230
	bl Func_02003228
	pop {pc}
	.section .text.x0200830c,"ax",%progbits
	.global Func_0200030c
	.thumb_func
Func_0200030c:
	push {lr}
	bl Func_020031e0
	movs r0, #0
	bl Func_02003280
	movs r0, #254
	lsls r0, r0, #7
	movs r1, #0
	adds r0, #255
	bl Func_02003248
	movs r0, #30
	bl Func_02003258
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #190
	bl Func_020032b8
	ldr r3, .L_020083c0
	movs r1, #241
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #2
	beq .L_02008366
	cmp r3, #2
	bgt .L_02008350
	cmp r3, #1
	beq .L_0200835a
	b .L_02008388
.L_02008350:
	cmp r3, #3
	beq .L_02008372
	cmp r3, #4
	beq .L_0200837e
	b .L_02008388
.L_0200835a:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #50
	bl GameFlag_SetBit
	b .L_02008388
.L_02008366:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #51
	bl GameFlag_SetBit
	b .L_02008388
.L_02008372:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #52
	bl GameFlag_SetBit
	b .L_02008388
.L_0200837e:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #53
	bl GameFlag_SetBit
.L_02008388:
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r1, #179
	lsls r1, r1, #1
	adds r2, r3, r1
	movs r3, #0
	strh r3, [r2]
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #128
	movs r1, #0
	lsls r0, r0, #9
	bl Func_02003248
	movs r0, #30
	bl Func_02003258
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #10
	bl Battle_WaitMode0
	bl Func_020031e8
	pop {pc}
.L_020083c0:
	.4byte gPartyState
	.section .text.x020083c4,"ax",%progbits
	.global Func_020003c4
	.thumb_func
Func_020003c4:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r0, #179
	lsls r0, r0, #1
	adds r3, r2, r0
	movs r1, #0
	ldrsh r3, [r3, r1]
	sub sp, #4
	cmp r3, #0
	beq .L_020083e2
	b .L_0200852e
.L_020083e2:
	movs r0, #192
	lsls r0, r0, #4
	adds r0, #164
	adds r3, r2, r0
	movs r5, #0
	ldrsb r5, [r3, r5]
	cmp r5, #0
	beq .L_020083f4
	b .L_0200852e
.L_020083f4:
	ldr r7, .L_02008538
	movs r1, #1
	ldr r3, [r7]
	negs r1, r1
	cmp r3, r1
	bne .L_0200842c
	movs r1, #160
	lsls r1, r1, #19
	adds r1, #64
	movs r2, #32
	ldr r3, .L_0200853c
	ldr r0, .L_02008540
	mov lr, r3
	.2byte 0xf800
	ldr r6, .L_02008544
	ldr r1, .L_02008548
	movs r2, #64
	ldr r3, .L_0200853c
	adds r0, r6, #0
	mov lr, r3
	.2byte 0xf800
	ldr r0, .L_0200854c
	adds r1, r6, #0
	movs r2, #64
	ldr r3, .L_0200853c
	mov lr, r3
	.2byte 0xf800
	str r5, [r7]
.L_0200842c:
	ldr r0, [r7]
	cmp r0, #0
	bge .L_02008434
	adds r0, #7
.L_02008434:
	asrs r0, r0, #3
	movs r1, #6
	bl __modsi3
	movs r7, #8
	mov r8, r0
	movs r4, #0
.L_02008442:
	mov r1, r8
	adds r0, r1, r4
	movs r1, #6
	str r4, [sp, #0]
	bl __modsi3
	ldr r2, .L_02008544
	lsls r1, r0, #1
	adds r3, r1, #0
	adds r3, #8
	ldrh r3, [r2, r3]
	ldr r6, .L_0200854c
	ldr r0, .L_02008544
	strh r3, [r6, r7]
	adds r3, r1, #0
	adds r3, #20
	ldrh r3, [r0, r3]
	adds r2, r7, #0
	adds r2, #12
	strh r3, [r6, r2]
	adds r3, r1, #0
	adds r3, #40
	ldrh r3, [r0, r3]
	adds r2, #20
	strh r3, [r6, r2]
	adds r3, r1, #0
	adds r3, #52
	ldr r4, [sp, #0]
	ldrh r3, [r0, r3]
	adds r2, #12
	adds r4, #1
	strh r3, [r6, r2]
	adds r7, #2
	cmp r4, #5
	ble .L_02008442
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #50
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020084a4
	adds r0, r6, #0
	adds r0, #20
	ldr r1, .L_02008550
	ldr r3, .L_0200853c
	movs r2, #12
	mov lr, r3
	.2byte 0xf800
.L_020084a4:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #51
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020084c0
	adds r0, r6, #0
	adds r0, #40
	ldr r1, .L_02008550
	ldr r3, .L_0200853c
	movs r2, #12
	mov lr, r3
	.2byte 0xf800
.L_020084c0:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #52
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020084dc
	adds r0, r6, #0
	adds r0, #52
	ldr r1, .L_02008550
	ldr r3, .L_0200853c
	movs r2, #12
	mov lr, r3
	.2byte 0xf800
.L_020084dc:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #53
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020084f8
	adds r0, r6, #0
	adds r0, #8
	ldr r1, .L_02008550
	ldr r3, .L_0200853c
	movs r2, #12
	mov lr, r3
	.2byte 0xf800
.L_020084f8:
	ldr r1, .L_02008554
	ldr r0, .L_02008558
	ldrh r3, [r0]
	adds r4, r3, #0
	strh r0, [r0]
	ldrh r2, [r1]
	cmp r2, #31
	bgt .L_02008524
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	adds r3, r3, r1
	adds r3, #4
	adds r2, #1
	stmia r3!, {r6}
	strh r2, [r1]
	ldr r2, .L_02008548
	stmia r3!, {r2}
	movs r2, #132
	lsls r2, r2, #24
	adds r2, #16
	str r2, [r3]
.L_02008524:
	strh r4, [r0]
	ldr r2, .L_02008538
	ldr r3, [r2]
	adds r3, #1
	str r3, [r2]
.L_0200852e:
	add sp, #4
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008538:
	.4byte Data_02003704
.L_0200853c:
	.4byte IwramCopyWords
.L_02008540:
	.4byte Data_02003a5c
.L_02008544:
	.4byte Data_020039dc
.L_02008548:
	.4byte 0x05000100
.L_0200854c:
	.4byte Data_02003a1c
.L_02008550:
	.4byte Data_0200ba70
.L_02008554:
	.4byte gIoWriteQueue
.L_02008558:
	.4byte 0x04000208
	.section .text.x0200855c,"ax",%progbits
	.global Func_0200055c
	.thumb_func
Func_0200055c:
	push {r5, r6, lr}
	ldr r5, .L_020086b4
	ldr r3, .L_020086b8
	movs r1, #0
	ldrsh r2, [r5, r1]
	movs r1, #0
	ldrsh r3, [r3, r1]
	sub sp, #8
	cmp r2, r3
	bne .L_02008572
	b .L_020086b0
.L_02008572:
	subs r1, r3, r2
	cmp r1, #0
	bge .L_0200857e
	movs r1, #1
	negs r1, r1
	b .L_02008580
.L_0200857e:
	movs r1, #1
.L_02008580:
	ldrh r3, [r5]
	ldr r0, .L_020086bc
	adds r1, r3, r1
	strh r1, [r5]
	ldr r4, .L_020086c0
	ldrh r3, [r4]
	adds r6, r3, #0
	strh r4, [r4]
	ldrh r3, [r0]
	cmp r3, #31
	bgt .L_020085c0
	lsls r2, r3, #1
	adds r2, r2, r3
	lsls r1, r1, #16
	adds r3, #1
	asrs r1, r1, #16
	strh r3, [r0]
	movs r3, #16
	lsls r2, r2, #2
	subs r3, r3, r1
	adds r2, r2, r0
	lsls r3, r3, #8
	adds r2, #4
	orrs r3, r1
	stmia r2!, {r3}
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #82
	stmia r2!, {r3}
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r2]
.L_020085c0:
	strh r6, [r4]
	movs r2, #0
	ldrsh r3, [r5, r2]
	cmp r3, #0
	bne .L_02008638
	ldr r5, .L_020086c4
	movs r1, #0
	ldrsh r3, [r5, r1]
	cmp r3, #0
	bne .L_020085e8
	movs r3, #17
	movs r2, #8
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #34
	movs r1, #8
	movs r2, #3
	movs r3, #3
	bl Func_02003170
.L_020085e8:
	movs r2, #0
	ldrsh r3, [r5, r2]
	cmp r3, #1
	bne .L_02008602
	movs r3, #8
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #30
	movs r1, #8
	movs r2, #3
	movs r3, #3
	bl Func_02003170
.L_02008602:
	movs r1, #0
	ldrsh r3, [r5, r1]
	cmp r3, #2
	bne .L_0200861e
	movs r3, #17
	movs r2, #8
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #34
	movs r1, #8
	movs r2, #3
	movs r3, #3
	bl Func_02003170
.L_0200861e:
	movs r2, #0
	ldrsh r3, [r5, r2]
	cmp r3, #3
	bne .L_02008638
	movs r3, #8
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #30
	movs r1, #8
	movs r2, #3
	movs r3, #3
	bl Func_02003170
.L_02008638:
	ldr r3, .L_020086b4
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #16
	bne .L_020086b0
	ldr r5, .L_020086c4
	movs r2, #0
	ldrsh r3, [r5, r2]
	cmp r3, #0
	bne .L_02008660
	movs r3, #17
	movs r2, #8
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #34
	movs r1, #0
	movs r2, #3
	movs r3, #3
	bl Func_02003170
.L_02008660:
	movs r1, #0
	ldrsh r3, [r5, r1]
	cmp r3, #1
	bne .L_0200867a
	movs r3, #8
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #30
	movs r1, #4
	movs r2, #3
	movs r3, #3
	bl Func_02003170
.L_0200867a:
	movs r2, #0
	ldrsh r3, [r5, r2]
	cmp r3, #2
	bne .L_02008696
	movs r3, #17
	movs r2, #8
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #34
	movs r1, #4
	movs r2, #3
	movs r3, #3
	bl Func_02003170
.L_02008696:
	movs r1, #0
	ldrsh r3, [r5, r1]
	cmp r3, #3
	bne .L_020086b0
	movs r3, #8
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #30
	movs r1, #0
	movs r2, #3
	movs r3, #3
	bl Func_02003170
.L_020086b0:
	add sp, #8
	pop {r5, r6, pc}
.L_020086b4:
	.4byte Data_02003a7c
.L_020086b8:
	.4byte Data_02003a7e
.L_020086bc:
	.4byte gIoWriteQueue
.L_020086c0:
	.4byte 0x04000208
.L_020086c4:
	.4byte Data_02003a80
	.section .text.x020086c8,"ax",%progbits
	.global Func_020006c8
	.thumb_func
Func_020006c8:
	push {r5, r6, r7, lr}
	ldr r7, .L_02008708
	adds r5, r0, #0
	movs r3, #0
	ldrsh r2, [r7, r3]
	sub sp, #8
	cmp r5, r2
	bne .L_020086da
	b .L_02008806
.L_020086da:
	adds r3, r5, #0
	eors r3, r2
	movs r6, #1
	ands r3, r6
	cmp r3, #0
	beq .L_020086ee
	movs r0, #140
	lsls r0, r0, #2
	bl Func_020032b8
.L_020086ee:
	adds r3, r5, #0
	ands r3, r6
	adds r6, r5, #0
	strh r5, [r7]
	ldr r2, .L_0200870c
	subs r6, #20
	cmp r3, #0
	beq .L_02008710
	ldr r3, .L_02008704
	b .L_02008712
	.2byte 0x0000
.L_02008704:
	.4byte 0x00000000
.L_02008708:
	.4byte Data_02003a82
.L_0200870c:
	.4byte Data_02003a7e
.L_02008710:
	ldr r3, .L_02008734
.L_02008712:
	strh r3, [r2]
	ldr r5, .L_02008738
	movs r2, #0
	ldrsh r3, [r5, r2]
	cmp r3, #0
	bne .L_0200873c
	movs r3, #17
	movs r2, #67
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #122
	movs r2, #3
	movs r3, #3
	bl Func_02003178
	b .L_0200873c
.L_02008734:
	.4byte 0x00000010
.L_02008738:
	.4byte Data_02003a80
.L_0200873c:
	movs r2, #0
	ldrsh r3, [r5, r2]
	cmp r3, #1
	bne .L_02008758
	movs r3, #8
	movs r2, #72
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #122
	movs r2, #3
	movs r3, #3
	bl Func_02003178
.L_02008758:
	movs r2, #0
	ldrsh r3, [r5, r2]
	cmp r3, #2
	bne .L_02008774
	movs r3, #17
	movs r2, #72
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #122
	movs r2, #3
	movs r3, #3
	bl Func_02003178
.L_02008774:
	movs r2, #0
	ldrsh r3, [r5, r2]
	cmp r3, #3
	bne .L_02008790
	movs r3, #8
	movs r2, #67
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #122
	movs r2, #3
	movs r3, #3
	bl Func_02003178
.L_02008790:
	lsrs r3, r6, #31
	adds r3, r6, r3
	asrs r3, r3, #1
	strh r3, [r5]
	lsls r3, r3, #16
	cmp r3, #0
	bne .L_020087b2
	movs r3, #17
	movs r2, #67
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #125
	movs r2, #3
	movs r3, #3
	bl Func_02003178
.L_020087b2:
	movs r2, #0
	ldrsh r3, [r5, r2]
	cmp r3, #1
	bne .L_020087ce
	movs r3, #8
	movs r2, #72
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #125
	movs r2, #3
	movs r3, #3
	bl Func_02003178
.L_020087ce:
	movs r2, #0
	ldrsh r3, [r5, r2]
	cmp r3, #2
	bne .L_020087ea
	movs r3, #17
	movs r2, #72
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #125
	movs r2, #3
	movs r3, #3
	bl Func_02003178
.L_020087ea:
	movs r2, #0
	ldrsh r3, [r5, r2]
	cmp r3, #3
	bne .L_02008806
	movs r3, #8
	movs r2, #67
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #125
	movs r2, #3
	movs r3, #3
	bl Func_02003178
.L_02008806:
	add sp, #8
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x0200880c,"ax",%progbits
	.global Func_0200080c
	.thumb_func
Func_0200080c:
	push {r5, r6, lr}
	mov r6, r11
	mov r5, r10
	push {r5, r6}
	mov r6, r9
	mov r5, r8
	push {r5, r6}
	movs r3, #17
	mov r9, r3
	movs r3, #67
	sub sp, #8
	mov r11, r3
	mov r3, r9
	str r3, [sp, #0]
	mov r3, r11
	str r3, [sp, #4]
	movs r0, #0
	movs r1, #125
	movs r2, #3
	movs r3, #3
	bl Func_02003178
	movs r3, #8
	mov r8, r3
	movs r3, #72
	mov r10, r3
	mov r3, r8
	str r3, [sp, #0]
	mov r3, r10
	str r3, [sp, #4]
	movs r0, #0
	movs r1, #125
	movs r2, #3
	movs r3, #3
	bl Func_02003178
	mov r3, r9
	str r3, [sp, #0]
	mov r3, r10
	str r3, [sp, #4]
	movs r0, #0
	movs r1, #125
	movs r2, #3
	movs r3, #3
	bl Func_02003178
	mov r3, r8
	str r3, [sp, #0]
	mov r3, r11
	str r3, [sp, #4]
	movs r1, #125
	movs r2, #3
	movs r3, #3
	movs r0, #0
	bl Func_02003178
	movs r0, #1
	bl WaitFrames
	ldr r6, .L_020088c0
	ldr r5, .L_020088c8
	movs r0, #12
	strh r6, [r5]
	bl WaitFrames
	mov r3, r8
	strh r3, [r5]
	movs r0, #4
	bl WaitFrames
	strh r6, [r5]
	movs r0, #4
	bl WaitFrames
	mov r3, r8
	strh r3, [r5]
	movs r0, #4
	bl WaitFrames
	strh r6, [r5]
	movs r0, #4
	bl WaitFrames
	ldr r3, .L_020088c4
	movs r0, #12
	strh r3, [r5]
	bl WaitFrames
	b .L_020088cc
	.2byte 0x0000
.L_020088c0:
	.4byte 0x0000000c
.L_020088c4:
	.4byte 0x00000000
.L_020088c8:
	.4byte Data_02003a7e
.L_020088cc:
	mov r3, r9
	str r3, [sp, #0]
	mov r3, r11
	str r3, [sp, #4]
	movs r0, #0
	movs r1, #122
	movs r2, #3
	movs r3, #3
	bl Func_02003178
	mov r3, r8
	str r3, [sp, #0]
	mov r3, r10
	str r3, [sp, #4]
	movs r0, #0
	movs r1, #122
	movs r2, #3
	movs r3, #3
	bl Func_02003178
	mov r3, r9
	str r3, [sp, #0]
	mov r3, r10
	str r3, [sp, #4]
	movs r0, #0
	movs r1, #122
	movs r2, #3
	movs r3, #3
	bl Func_02003178
	mov r3, r8
	str r3, [sp, #0]
	mov r3, r11
	str r3, [sp, #4]
	movs r1, #122
	movs r2, #3
	movs r3, #3
	movs r0, #0
	bl Func_02003178
	movs r0, #1
	bl WaitFrames
	add sp, #8
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r3}
	mov r11, r3
	pop {r5, r6, pc}
	.2byte 0x0000
	.section .text.x02008934,"ax",%progbits
	.global Func_02000934
	.thumb_func
Func_02000934:
	push {r5, r6, lr}
	adds r5, r0, #0
	movs r6, #1
	sub sp, #8
	negs r6, r6
	bl Func_02003158
	ldr r1, .L_0200897c
	cmp r5, #8
	bne .L_0200894a
	movs r6, #20
.L_0200894a:
	cmp r5, #10
	bne .L_02008950
	movs r6, #22
.L_02008950:
	cmp r5, #12
	bne .L_02008956
	movs r6, #24
.L_02008956:
	cmp r5, #14
	bne .L_0200895c
	movs r6, #26
.L_0200895c:
	movs r2, #1
	negs r2, r2
	cmp r6, r2
	bne .L_0200898c
	ldr r2, .L_02008978
	ldr r3, .L_02008980
	strh r2, [r1]
	strh r2, [r3]
	ldr r3, .L_02008984
	strh r6, [r3]
	ldr r3, .L_02008988
	strh r6, [r3]
	b .L_02008a2c
	.2byte 0x0000
.L_02008978:
	.4byte 0x00000000
.L_0200897c:
	.4byte Data_02003a7c
.L_02008980:
	.4byte Data_02003a7e
.L_02008984:
	.4byte Data_02003a80
.L_02008988:
	.4byte Data_02003a82
.L_0200898c:
	ldr r3, .L_020089c4
	ldr r2, .L_020089cc
	strh r3, [r1]
	ldr r3, .L_020089c8
	ldr r5, .L_020089d0
	strh r3, [r2]
	adds r3, r6, #0
	subs r3, #20
	lsrs r2, r3, #31
	adds r3, r3, r2
	ldr r2, .L_020089d4
	asrs r3, r3, #1
	strh r3, [r5]
	strh r6, [r2]
	cmp r3, #0
	bne .L_020089d8
	movs r3, #17
	movs r2, #67
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #125
	movs r2, #3
	movs r3, #3
	bl Func_02003178
	b .L_020089d8
	.2byte 0x0000
.L_020089c4:
	.4byte 0x0000000f
.L_020089c8:
	.4byte 0x00000010
.L_020089cc:
	.4byte Data_02003a7e
.L_020089d0:
	.4byte Data_02003a80
.L_020089d4:
	.4byte Data_02003a82
.L_020089d8:
	movs r6, #0
	ldrsh r3, [r5, r6]
	cmp r3, #1
	bne .L_020089f4
	movs r3, #8
	movs r2, #72
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #125
	movs r2, #3
	movs r3, #3
	bl Func_02003178
.L_020089f4:
	movs r2, #0
	ldrsh r3, [r5, r2]
	cmp r3, #2
	bne .L_02008a10
	movs r3, #17
	movs r2, #72
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #125
	movs r2, #3
	movs r3, #3
	bl Func_02003178
.L_02008a10:
	movs r6, #0
	ldrsh r3, [r5, r6]
	cmp r3, #3
	bne .L_02008a2c
	movs r3, #8
	movs r2, #67
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #125
	movs r2, #3
	movs r3, #3
	bl Func_02003178
.L_02008a2c:
	ldr r4, .L_02008aac
	ldr r0, .L_02008ab0
	ldrh r3, [r0]
	adds r5, r3, #0
	strh r0, [r0]
	ldrh r3, [r4]
	cmp r3, #31
	bgt .L_02008a68
	lsls r2, r3, #1
	adds r2, r2, r3
	adds r3, #1
	strh r3, [r4]
	ldr r3, .L_02008ab4
	lsls r2, r2, #2
	movs r6, #0
	ldrsh r1, [r3, r6]
	movs r3, #16
	subs r3, r3, r1
	lsls r3, r3, #8
	adds r2, r2, r4
	adds r2, #4
	orrs r1, r3
	stmia r2!, {r1}
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #82
	stmia r2!, {r3}
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r2]
.L_02008a68:
	strh r5, [r0]
	ldrh r3, [r0]
	adds r1, r3, #0
	strh r0, [r0]
	ldrh r2, [r4]
	cmp r2, #31
	bgt .L_02008a9a
	lsls r3, r2, #1
	adds r3, r3, r2
	adds r2, #1
	lsls r3, r3, #2
	strh r2, [r4]
	movs r2, #252
	adds r3, r3, r4
	lsls r2, r2, #6
	adds r3, #4
	adds r2, #66
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #80
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #10
	str r2, [r3]
.L_02008a9a:
	strh r1, [r0]
	ldr r0, .L_02008ab8
	movs r1, #144
	lsls r1, r1, #3
	bl Scheduler_AddOrUpdateCallback
	add sp, #8
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02008aac:
	.4byte gIoWriteQueue
.L_02008ab0:
	.4byte 0x04000208
.L_02008ab4:
	.4byte Data_02003a7c
.L_02008ab8:
	.4byte Func_0200055c
	.section .text.x02008abc,"ax",%progbits
	.global Func_02000abc
	.thumb_func
Func_02000abc:
	push {lr}
	bl Func_02001608
	bl Func_020032a8
	ldr r3, .L_02008aec
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #194
	adds r3, r3, r2
	movs r2, #2
	strb r2, [r3]
	movs r0, #1
	bl Func_02003238
	movs r0, #40
	bl Battle_WaitMode0
	bl Func_0200166c
	ldr r0, .L_02008af0
	bl Scheduler_RemoveCallbackFar
	pop {pc}
.L_02008aec:
	.4byte gPartyState
.L_02008af0:
	.4byte Func_0200055c
	.section .text.x02008af4,"ax",%progbits
	.global Func_02000af4
	.thumb_func
Func_02000af4:
	push {lr}
	ldr r3, .L_02008b38
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008b3c
	cmp r2, r3
	bne .L_02008b0c
	ldr r0, .L_02008b40
	b .L_02008b34
.L_02008b0c:
	ldr r3, .L_02008b44
	cmp r2, r3
	bne .L_02008b16
	ldr r0, .L_02008b48
	b .L_02008b34
.L_02008b16:
	ldr r3, .L_02008b4c
	cmp r2, r3
	beq .L_02008b2e
	ldr r3, .L_02008b50
	cmp r2, r3
	beq .L_02008b2e
	ldr r3, .L_02008b54
	cmp r2, r3
	beq .L_02008b2e
	ldr r3, .L_02008b58
	cmp r2, r3
	bne .L_02008b32
.L_02008b2e:
	ldr r0, .L_02008b5c
	b .L_02008b34
.L_02008b32:
	ldr r0, .L_02008b60
.L_02008b34:
	pop {pc}
	.2byte 0x0000
.L_02008b38:
	.4byte gPartyState
.L_02008b3c:
	.4byte 0x00000128
.L_02008b40:
	.4byte Data_020036c8
.L_02008b44:
	.4byte 0x0000011f
.L_02008b48:
	.4byte Data_02003708
.L_02008b4c:
	.4byte 0x00000120
.L_02008b50:
	.4byte 0x00000121
.L_02008b54:
	.4byte 0x00000122
.L_02008b58:
	.4byte 0x00000123
.L_02008b5c:
	.4byte Data_02003858
.L_02008b60:
	.4byte Data_020036b8
	.section .text.x02008b64,"ax",%progbits
	.global Func_02000b64
	.thumb_func
Func_02000b64:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r2, .L_02008cdc
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r2, r1
	movs r1, #0
	ldrsh r6, [r3, r1]
	movs r1, #241
	lsls r1, r1, #1
	movs r7, #192
	adds r3, r2, r1
	lsls r7, r7, #18
	movs r2, #0
	ldrsh r5, [r3, r2]
	ldr r3, [r7, #108]
	subs r1, #54
	movs r2, #128
	adds r3, r3, r1
	lsls r2, r2, #1
	movs r0, #137
	str r2, [r3]
	lsls r0, r0, #1
	sub sp, #8
	bl GameFlag_SetBit
	ldr r3, .L_02008ce0
	ldr r0, .L_02008ce4
	ldr r1, .L_02008ce8
	ldr r2, .L_02008cec
	bl Func_0200154c
	ldr r3, .L_02008cf0
	cmp r6, r3
	bne .L_02008bb0
	b .L_02008e76
.L_02008bb0:
	ldr r3, .L_02008cf4
	cmp r6, r3
	beq .L_02008bb8
	b .L_02008e76
.L_02008bb8:
	movs r0, #0
	bl Func_02003278
	ldr r3, [r7, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #88
	str r2, [r3]
	subs r3, r5, #1
	cmp r3, #1
	bls .L_02008be2
	cmp r5, #8
	beq .L_02008be2
	cmp r5, #10
	beq .L_02008be2
	cmp r5, #12
	beq .L_02008be2
	cmp r5, #14
	beq .L_02008be2
	b .L_02008e76
.L_02008be2:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #49
	bl GameFlag_Test
	adds r6, r0, #0
	cmp r6, #0
	beq .L_02008bf4
	b .L_02008e76
.L_02008bf4:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #50
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008c04
	b .L_02008e76
.L_02008c04:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #51
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008c14
	b .L_02008e76
.L_02008c14:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #52
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008c24
	b .L_02008e76
.L_02008c24:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #53
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008c34
	b .L_02008e76
.L_02008c34:
	bl Func_020031e0
	movs r0, #0
	bl Func_02003280
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	movs r0, #78
	bl Func_020032b8
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #10
	lsls r1, r1, #7
	bl Func_02003218
	movs r0, #224
	movs r1, #128
	movs r2, #216
	lsls r2, r2, #16
	movs r3, #1
	lsls r1, r1, #15
	lsls r0, r0, #16
	bl Motion_CamBounds
	bl Func_02003228
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #0
	bl Func_02003250
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #0
	adds r0, #3
	bl Func_02003248
	movs r0, #20
	bl Func_02003258
	movs r0, #20
	bl WaitFrames
	ldr r5, .L_02008cf8
	ldr r3, .L_02008cd8
	movs r0, #1
	strh r3, [r5]
	bl WaitFrames
	movs r2, #4
	ldr r0, .L_02008cfc
	movs r1, #0
	bl Func_020031c0
	movs r0, #128
	movs r1, #0
	lsls r0, r0, #9
	bl Func_02003248
	movs r0, #20
	bl Func_02003258
	movs r0, #20
	bl WaitFrames
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #9
	lsls r1, r1, #6
	bl Func_02003218
	movs r0, #224
	movs r1, #128
	movs r2, #216
	b .L_02008d00
	.2byte 0x0000
.L_02008cd8:
	.4byte 0x00007fff
.L_02008cdc:
	.4byte gPartyState
.L_02008ce0:
	.4byte Data_020033f4
.L_02008ce4:
	.4byte Data_0200338c
.L_02008ce8:
	.4byte Data_0200339c
.L_02008cec:
	.4byte Data_020033c8
.L_02008cf0:
	.4byte 0x00000128
.L_02008cf4:
	.4byte 0x0000011f
.L_02008cf8:
	.4byte 0x0500021e
.L_02008cfc:
	.4byte 0x00002d8b
.L_02008d00:
	lsls r1, r1, #13
	lsls r2, r2, #16
	movs r3, #1
	lsls r0, r0, #16
	bl Motion_CamBounds
	bl Func_02003228
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #16
	bl Object_GetById
	adds r2, r0, #0
	adds r3, r2, #0
	adds r3, #85
	strb r6, [r3]
	movs r3, #128
	adds r1, r2, #0
	lsls r3, r3, #13
	str r3, [r2, #12]
	str r3, [r2, #20]
	adds r1, #98
	movs r3, #18
	strb r3, [r1]
	movs r3, #128
	lsls r3, r3, #7
	adds r1, #4
	strh r3, [r1]
	str r6, [r2, #104]
	bl Func_02002e6c
	movs r0, #40
	bl WaitFrames
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r0, r0, #10
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl Func_02003198
	movs r3, #13
	str r3, [sp, #0]
	str r3, [sp, #4]
	mov r8, r3
	movs r0, #216
	movs r1, #216
	movs r2, #65
	movs r3, #65
	bl Func_02001324
	mov r1, r8
	str r1, [sp, #4]
	movs r6, #14
	movs r0, #232
	movs r1, #216
	movs r2, #66
	movs r3, #65
	str r6, [sp, #0]
	bl Func_02001324
	movs r0, #232
	movs r1, #232
	movs r2, #66
	movs r3, #66
	str r6, [sp, #0]
	str r6, [sp, #4]
	bl Func_02001324
	mov r2, r8
	str r2, [sp, #0]
	movs r5, #12
	movs r0, #216
	movs r1, #200
	movs r2, #65
	movs r3, #64
	str r5, [sp, #4]
	bl Func_02001324
	movs r3, #15
	str r3, [sp, #0]
	mov r10, r3
	movs r0, #248
	movs r1, #200
	movs r2, #67
	movs r3, #64
	str r5, [sp, #4]
	bl Func_02001324
	mov r1, r10
	str r1, [sp, #0]
	movs r0, #248
	movs r1, #232
	movs r2, #67
	movs r3, #66
	str r6, [sp, #4]
	bl Func_02001324
	mov r2, r8
	str r2, [sp, #0]
	movs r0, #216
	movs r1, #232
	movs r2, #65
	movs r3, #66
	str r6, [sp, #4]
	bl Func_02001324
	mov r3, r8
	str r3, [sp, #4]
	movs r0, #200
	movs r1, #216
	movs r2, #64
	movs r3, #65
	str r5, [sp, #0]
	bl Func_02001324
	movs r0, #232
	movs r1, #200
	movs r2, #66
	movs r3, #64
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl Func_02001324
	mov r1, r10
	mov r2, r8
	str r1, [sp, #0]
	str r2, [sp, #4]
	movs r0, #248
	movs r1, #216
	movs r2, #67
	movs r3, #65
	bl Func_02001324
	movs r0, #200
	movs r1, #232
	movs r2, #64
	movs r3, #66
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_02001324
	movs r3, #64
	movs r0, #200
	movs r1, #200
	movs r2, #64
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl Func_02001324
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	negs r0, r0
	negs r1, r1
	adds r2, #102
	bl Func_02003198
	movs r1, #12
	movs r2, #4
	movs r3, #3
	movs r0, #30
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl Func_02003170
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #49
	bl GameFlag_SetBit
	movs r0, #80
	bl Func_020032b8
	bl AudioCommand_WaitForCompletion
	bl Func_02003290
	bl Func_020031e8
.L_02008e76:
	movs r0, #0
	add sp, #8
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x02008e84,"ax",%progbits
	.global Func_02000e84
	.thumb_func
Func_02000e84:
	push {r5, r6, lr}
	ldr r2, .L_020090b8
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r2, r1
	movs r1, #0
	ldrsh r5, [r3, r1]
	movs r1, #241
	lsls r1, r1, #1
	adds r3, r2, r1
	movs r2, #0
	ldrsh r6, [r3, r2]
	ldr r3, .L_020090bc
	sub sp, #12
	cmp r5, r3
	beq .L_02008ebc
	ldr r3, .L_020090c0
	cmp r5, r3
	beq .L_02008ebc
	ldr r3, .L_020090c4
	cmp r5, r3
	beq .L_02008ebc
	ldr r3, .L_020090c8
	cmp r5, r3
	beq .L_02008ebc
	ldr r3, .L_020090cc
	cmp r5, r3
	bne .L_02008ee2
.L_02008ebc:
	movs r0, #1
	bl Func_020032b0
	movs r3, #128
	movs r2, #128
	lsls r3, r3, #8
	lsls r2, r2, #7
	movs r1, #128
	str r3, [sp, #4]
	str r2, [sp, #8]
	movs r3, #128
	movs r2, #128
	lsls r1, r1, #9
	movs r0, #1
	lsls r2, r2, #11
	lsls r3, r3, #10
	str r1, [sp, #0]
	bl Func_02003270
.L_02008ee2:
	ldr r3, .L_020090bc
	cmp r5, r3
	bne .L_02008fa6
	cmp r6, #2
	beq .L_02008f22
	cmp r6, #2
	bgt .L_02008ef6
	cmp r6, #1
	beq .L_02008f00
	b .L_02008f86
.L_02008ef6:
	cmp r6, #3
	beq .L_02008f44
	cmp r6, #4
	beq .L_02008f66
	b .L_02008f86
.L_02008f00:
	movs r0, #8
	bl Object_GetById
	movs r1, #1
	bl Object_SetPartAttribute
	movs r1, #4
	movs r0, #9
	bl Func_02000058
	movs r0, #4
	bl Func_02003150
	movs r0, #5
	bl Func_02003150
	b .L_02008f86
.L_02008f22:
	movs r0, #8
	bl Object_GetById
	movs r1, #3
	bl Object_SetPartAttribute
	movs r1, #5
	movs r0, #9
	bl Func_02000058
	movs r0, #6
	bl Func_02003150
	movs r0, #7
	bl Func_02003150
	b .L_02008f86
.L_02008f44:
	movs r0, #8
	bl Object_GetById
	movs r1, #0
	bl Object_SetPartAttribute
	movs r1, #2
	movs r0, #9
	bl Func_02000058
	movs r0, #0
	bl Func_02003150
	movs r0, #1
	bl Func_02003150
	b .L_02008f86
.L_02008f66:
	movs r0, #8
	bl Object_GetById
	movs r1, #4
	bl Object_SetPartAttribute
	movs r1, #3
	movs r0, #9
	bl Func_02000058
	movs r0, #2
	bl Func_02003150
	movs r0, #3
	bl Func_02003150
.L_02008f86:
	movs r0, #10
	bl Object_GetById
	adds r2, r0, #0
	movs r3, #0
	adds r2, #85
	strb r3, [r2]
	movs r3, #208
	lsls r3, r3, #15
	movs r1, #144
	str r3, [r0, #16]
	lsls r1, r1, #3
	ldr r0, .L_020090d0
	bl Scheduler_AddOrUpdateCallback
	b .L_02009088
.L_02008fa6:
	ldr r3, .L_020090d4
	cmp r5, r3
	bne .L_02009088
	subs r3, r6, #1
	cmp r3, #1
	bls .L_02008fc2
	cmp r6, #8
	beq .L_02008fc2
	cmp r6, #10
	beq .L_02008fc2
	cmp r6, #12
	beq .L_02008fc2
	cmp r6, #14
	bne .L_02009088
.L_02008fc2:
	bl Func_02002118
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #49
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008ff8
	movs r5, #12
	movs r0, #30
	movs r1, #12
	movs r2, #4
	movs r3, #3
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl Func_02003170
	movs r3, #76
	str r3, [sp, #0]
	movs r0, #64
	movs r1, #64
	movs r2, #4
	movs r3, #3
	str r5, [sp, #4]
	bl Func_02003178
.L_02008ff8:
	adds r0, r6, #0
	bl Func_02000934
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_020090d8
	bl Scheduler_AddOrUpdateCallback
	movs r1, #4
	movs r0, #12
	bl Func_02000058
	movs r1, #5
	movs r0, #13
	bl Func_02000058
	movs r1, #2
	movs r0, #14
	bl Func_02000058
	movs r0, #15
	movs r1, #3
	bl Func_02000058
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #50
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02009040
	movs r0, #12
	movs r1, #0
	movs r2, #0
	bl Func_020031f8
.L_02009040:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #51
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02009058
	movs r0, #13
	movs r1, #0
	movs r2, #0
	bl Func_020031f8
.L_02009058:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #52
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02009070
	movs r0, #14
	movs r1, #0
	movs r2, #0
	bl Func_020031f8
.L_02009070:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #53
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02009088
	movs r0, #15
	movs r1, #0
	movs r2, #0
	bl Func_020031f8
.L_02009088:
	movs r0, #163
	lsls r0, r0, #4
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020090b0
	ldr r3, .L_020090b8
	movs r1, #253
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r2, #0
	ldrsh r0, [r3, r2]
	ldr r3, .L_020090dc
	ldr r2, .L_020090e0
	movs r1, #160
	subs r3, r3, r2
	adds r0, r0, r3
	lsls r1, r1, #19
	bl Func_020031b8
.L_020090b0:
	movs r0, #0
	add sp, #12
	pop {r5, r6, pc}
	.2byte 0x0000
.L_020090b8:
	.4byte gPartyState
.L_020090bc:
	.4byte 0x00000128
.L_020090c0:
	.4byte 0x00000120
.L_020090c4:
	.4byte 0x00000121
.L_020090c8:
	.4byte 0x00000122
.L_020090cc:
	.4byte 0x00000123
.L_020090d0:
	.4byte Func_020000c0
.L_020090d4:
	.4byte 0x0000011f
.L_020090d8:
	.4byte Func_020003c4
.L_020090dc:
	.4byte 0x00000121
.L_020090e0:
	.4byte 0x0000010e
	.section .text.x020090e4,"ax",%progbits
	.global Func_020010e4
	.thumb_func
Func_020010e4:
	push {r5, lr}
	bl Func_020031e0
	movs r0, #0
	bl Func_02003280
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #0
	bl Func_02003250
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #0
	adds r0, #3
	bl Func_02003248
	movs r0, #20
	bl Func_02003258
	movs r0, #20
	bl WaitFrames
	ldr r5, .L_02009148
	ldr r3, .L_02009144
	movs r0, #1
	strh r3, [r5]
	bl WaitFrames
	movs r2, #0
	ldr r0, .L_0200914c
	movs r1, #0
	bl Func_020031c0
	movs r0, #128
	movs r1, #0
	lsls r0, r0, #9
	bl Func_02003248
	movs r0, #20
	bl Func_02003258
	movs r0, #20
	bl WaitFrames
	bl Func_020031e8
	b .L_02009150
.L_02009144:
	.4byte 0x00007fff
.L_02009148:
	.4byte 0x0500021e
.L_0200914c:
	.4byte 0x00002d87
.L_02009150:
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x02009154,"ax",%progbits
	.global Func_02001154
	.thumb_func
Func_02001154:
	push {r5, lr}
	bl Func_020031e0
	movs r0, #0
	bl Func_02003280
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #0
	bl Func_02003250
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #0
	adds r0, #3
	bl Func_02003248
	movs r0, #20
	bl Func_02003258
	movs r0, #20
	bl WaitFrames
	ldr r5, .L_020091b8
	ldr r3, .L_020091b4
	movs r0, #1
	strh r3, [r5]
	bl WaitFrames
	movs r2, #0
	ldr r0, .L_020091bc
	movs r1, #0
	bl Func_020031c0
	movs r0, #128
	movs r1, #0
	lsls r0, r0, #9
	bl Func_02003248
	movs r0, #20
	bl Func_02003258
	movs r0, #20
	bl WaitFrames
	bl Func_020031e8
	b .L_020091c0
.L_020091b4:
	.4byte 0x00007fff
.L_020091b8:
	.4byte 0x0500021e
.L_020091bc:
	.4byte 0x00002d88
.L_020091c0:
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x020091c4,"ax",%progbits
	.global Func_020011c4
	.thumb_func
Func_020011c4:
	push {r5, lr}
	bl Func_020031e0
	movs r0, #0
	bl Func_02003280
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #0
	bl Func_02003250
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #0
	adds r0, #3
	bl Func_02003248
	movs r0, #20
	bl Func_02003258
	movs r0, #20
	bl WaitFrames
	ldr r5, .L_02009228
	ldr r3, .L_02009224
	movs r0, #1
	strh r3, [r5]
	bl WaitFrames
	movs r2, #0
	ldr r0, .L_0200922c
	movs r1, #0
	bl Func_020031c0
	movs r0, #128
	movs r1, #0
	lsls r0, r0, #9
	bl Func_02003248
	movs r0, #20
	bl Func_02003258
	movs r0, #20
	bl WaitFrames
	bl Func_020031e8
	b .L_02009230
.L_02009224:
	.4byte 0x00007fff
.L_02009228:
	.4byte 0x0500021e
.L_0200922c:
	.4byte 0x00002d89
.L_02009230:
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x02009234,"ax",%progbits
	.global Func_02001234
	.thumb_func
Func_02001234:
	push {r5, lr}
	bl Func_020031e0
	movs r0, #0
	bl Func_02003280
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #0
	bl Func_02003250
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #0
	adds r0, #3
	bl Func_02003248
	movs r0, #20
	bl Func_02003258
	movs r0, #20
	bl WaitFrames
	ldr r5, .L_02009298
	ldr r3, .L_02009294
	movs r0, #1
	strh r3, [r5]
	bl WaitFrames
	movs r2, #0
	ldr r0, .L_0200929c
	movs r1, #0
	bl Func_020031c0
	movs r0, #128
	movs r1, #0
	lsls r0, r0, #9
	bl Func_02003248
	movs r0, #20
	bl Func_02003258
	movs r0, #20
	bl WaitFrames
	bl Func_020031e8
	b .L_020092a0
.L_02009294:
	.4byte 0x00007fff
.L_02009298:
	.4byte 0x0500021e
.L_0200929c:
	.4byte 0x00002d8a
.L_020092a0:
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x020092a4,"ax",%progbits
	.global Func_020012a4
	.thumb_func
Func_020012a4:
	push {r5, r6, lr}
	adds r4, r0, #0
	adds r3, r1, #0
	lsls r4, r4, #16
	movs r0, #148
	adds r6, r2, #0
	lsls r3, r3, #16
	adds r0, #255
	adds r1, r4, #0
	movs r2, #0
	bl Func_02003128
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02009318
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r1, #3
	adds r0, r5, #0
	bl Func_02003118
	ldr r1, [r5, #80]
	cmp r6, #0
	beq .L_020092ee
	ldrb r2, [r1, #9]
	movs r3, #13
	negs r3, r3
	ands r3, r2
	movs r2, #8
	orrs r3, r2
	strb r3, [r1, #9]
	adds r0, r5, #0
	ldr r1, .L_0200931c
	bl Func_02003120
	b .L_02009318
.L_020092ee:
	ldrb r2, [r1, #9]
	movs r3, #13
	negs r3, r3
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	strb r3, [r1, #9]
	bl Random16Far
	lsls r0, r0, #3
	lsrs r0, r0, #16
	subs r0, #4
	movs r3, #128
	lsls r0, r0, #16
	lsls r3, r3, #11
	str r0, [r5, #36]
	str r3, [r5, #40]
	ldr r1, .L_02009320
	adds r0, r5, #0
	bl Func_02003120
.L_02009318:
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200931c:
	.4byte Data_0200387c
.L_02009320:
	.4byte Data_02003900
	.section .text.x02009324,"ax",%progbits
	.global Func_02001324
	.thumb_func
Func_02001324:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	mov r8, r0
	adds r7, r1, #0
	movs r0, #144
	sub sp, #8
	mov r10, r2
	mov r9, r3
	bl Func_020032b8
	adds r1, r7, #0
	adds r1, #12
	mov r0, r8
	movs r2, #1
	bl Func_020012a4
	movs r6, #0
.L_0200934c:
	bl Random16Far
	lsls r5, r0, #1
	adds r5, r5, r0
	bl Random16Far
	lsls r1, r0, #1
	adds r1, r1, r0
	lsls r5, r5, #2
	lsrs r5, r5, #16
	lsls r1, r1, #2
	add r5, r8
	lsrs r1, r1, #16
	subs r5, #6
	adds r1, r7, r1
	subs r1, #6
	adds r0, r5, #0
	movs r2, #0
	adds r6, #1
	bl Func_020012a4
	cmp r6, #2
	bls .L_0200934c
	movs r0, #4
	bl WaitFrames
	ldr r2, [sp, #36]
	movs r3, #1
	str r3, [sp, #0]
	str r3, [sp, #4]
	adds r2, #64
	ldr r3, [sp, #40]
	mov r0, r10
	mov r1, r9
	bl Func_02003148
	add sp, #8
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.section .text.x020093a0,"ax",%progbits
	.global Func_020013a0
	.thumb_func
Func_020013a0:
	push {r5, lr}
	movs r0, #222
	bl PartyInventory_FindOwner
	movs r3, #1
	negs r3, r3
	cmp r0, r3
	bne .L_0200945c
	movs r0, #147
	lsls r0, r0, #4
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020093c0
	b .L_02009548
.L_020093c0:
	bl Func_020031e0
	movs r0, #0
	bl Func_02003280
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #10
	lsls r1, r1, #7
	bl Func_02003218
	movs r0, #224
	movs r1, #128
	movs r2, #216
	lsls r2, r2, #16
	movs r3, #1
	lsls r1, r1, #15
	lsls r0, r0, #16
	bl Motion_CamBounds
	bl Func_02003228
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #0
	bl Func_02003250
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #0
	adds r0, #3
	bl Func_02003248
	movs r0, #20
	bl Func_02003258
	movs r0, #20
	bl WaitFrames
	ldr r5, .L_02009454
	ldr r3, .L_02009450
	movs r0, #1
	strh r3, [r5]
	bl WaitFrames
	movs r2, #4
	ldr r0, .L_02009458
	movs r1, #0
	bl Func_020031c0
	movs r0, #128
	movs r1, #0
	lsls r0, r0, #9
	bl Func_02003248
	movs r0, #20
	bl Func_02003258
	movs r0, #20
	bl WaitFrames
	movs r0, #147
	lsls r0, r0, #4
	adds r0, #255
	bl GameFlag_SetBit
	bl Func_020031e8
	b .L_02009548
.L_02009450:
	.4byte 0x00007fff
.L_02009454:
	.4byte 0x0500021e
.L_02009458:
	.4byte 0x00002d84
.L_0200945c:
	movs r0, #163
	lsls r0, r0, #4
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009548
	bl Func_020031e0
	movs r0, #0
	bl Func_02003280
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #10
	lsls r1, r1, #7
	bl Func_02003218
	movs r0, #224
	movs r1, #128
	movs r2, #216
	lsls r2, r2, #16
	movs r3, #1
	lsls r1, r1, #15
	lsls r0, r0, #16
	bl Motion_CamBounds
	bl Func_02003228
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #0
	bl Func_02003250
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #0
	adds r0, #3
	bl Func_02003248
	movs r0, #20
	bl Func_02003258
	movs r0, #20
	bl WaitFrames
	ldr r5, .L_02009500
	ldr r3, .L_020094fc
	movs r0, #1
	strh r3, [r5]
	bl WaitFrames
	ldr r5, .L_02009504
	movs r1, #0
	adds r0, r5, #0
	movs r2, #4
	bl Func_020031c0
	adds r5, #1
	movs r0, #10
	bl WaitFrames
	adds r0, r5, #0
	movs r1, #0
	movs r2, #3
	bl Func_020031c0
	movs r0, #224
	movs r1, #176
	movs r2, #216
	lsls r2, r2, #16
	movs r3, #1
	lsls r1, r1, #15
	lsls r0, r0, #16
	bl Motion_CamBounds
	b .L_02009508
	.2byte 0x0000
.L_020094fc:
	.4byte 0x00007fff
.L_02009500:
	.4byte 0x0500021e
.L_02009504:
	.4byte 0x00002d85
.L_02009508:
	bl Func_02003228
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #140
	lsls r0, r0, #2
	bl Func_020032b8
	bl Func_0200080c
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #128
	movs r1, #0
	lsls r0, r0, #9
	bl Func_02003248
	movs r0, #20
	bl Func_02003258
	movs r0, #40
	bl WaitFrames
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #46
	bl GameFlag_SetBit
	bl Func_020031e8
.L_02009548:
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x0200954c,"ax",%progbits
	.global Func_0200154c
	.thumb_func
Func_0200154c:
	push {r5, r6, lr}
	adds r4, r1, #0
	movs r1, #192
	lsls r1, r1, #18
	adds r1, #128
	ldr r6, [r1]
	ldr r1, .L_020095f0
	adds r5, r0, #0
	str r5, [r1]
	ldr r1, .L_020095f4
	str r4, [r1]
	ldr r1, .L_020095f8
	str r2, [r1]
	ldr r2, .L_020095fc
	str r3, [r2]
	movs r2, #255
	ldrh r3, [r5]
	b .L_02009596
.L_02009570:
	ldrh r0, [r4]
	adds r4, #2
	ldrh r2, [r4]
	adds r4, #2
	ldrh r1, [r5]
	ldrh r3, [r4]
	adds r5, #2
	adds r4, #2
	lsls r3, r3, #10
	lsls r2, r2, #5
	orrs r3, r2
	movs r2, #160
	lsls r2, r2, #19
	lsls r1, r1, #1
	orrs r3, r0
	adds r1, r1, r2
	strh r3, [r1]
	ldrh r3, [r5]
	movs r2, #255
.L_02009596:
	lsls r2, r2, #8
	adds r2, #255
	cmp r3, r2
	beq .L_020095a4
	ldrh r3, [r4]
	cmp r3, r2
	bne .L_02009570
.L_020095a4:
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	movs r0, #160
	lsls r2, r2, #24
	adds r3, #212
	lsls r0, r0, #19
	adds r1, r6, #0
	adds r2, #112
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r2, #224
	lsls r2, r2, #1
	adds r1, r6, r2
	movs r2, #132
	lsls r2, r2, #24
	ldr r0, .L_02009600
	adds r2, #112
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #0
	bl Func_02003248
	ldr r3, .L_02009604
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #194
	adds r3, r3, r2
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #2
	bne .L_020095ee
	bl Func_0200183c
.L_020095ee:
	pop {r5, r6, pc}
.L_020095f0:
	.4byte Data_02003928
.L_020095f4:
	.4byte Data_0200392c
.L_020095f8:
	.4byte Data_02003930
.L_020095fc:
	.4byte Data_0200391c
.L_02009600:
	.4byte 0x05000200
.L_02009604:
	.4byte gPartyState
	.section .text.x02009608,"ax",%progbits
	.global Func_02001608
	.thumb_func
Func_02001608:
	push {r5, lr}
	ldr r2, .L_0200964c
	ldr r3, .L_0200963c
	ldr r5, .L_02009650
	strh r3, [r2]
	ldr r3, .L_02009654
	ldr r0, [r3]
	bl Func_020016e8
	ldr r2, .L_02009658
	ldr r3, .L_02009640
	strh r0, [r5]
	strh r3, [r2]
	ldr r2, .L_0200965c
	ldr r3, .L_02009644
	movs r1, #144
	strh r3, [r2]
	ldr r2, .L_02009660
	ldr r3, .L_02009648
	lsls r1, r1, #3
	strh r3, [r2]
	ldr r0, .L_02009664
	bl Scheduler_AddOrUpdateCallback
	b .L_02009668
	.2byte 0x0000
.L_0200963c:
	.4byte 0x00000000
.L_02009640:
	.4byte 0x0000000f
.L_02009644:
	.4byte 0x00000010
.L_02009648:
	.4byte 0x00000001
.L_0200964c:
	.4byte Data_02003938
.L_02009650:
	.4byte Data_02003934
.L_02009654:
	.4byte Data_02003928
.L_02009658:
	.4byte Data_02003924
.L_0200965c:
	.4byte Data_02003920
.L_02009660:
	.4byte Data_02003918
.L_02009664:
	.4byte Func_0200170c
.L_02009668:
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x0200966c,"ax",%progbits
	.global Func_0200166c
	.thumb_func
Func_0200166c:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	subs r2, #172
	str r2, [r3]
	ldr r0, .L_02009688
	bl Scheduler_RemoveCallbackFar
	pop {pc}
	.2byte 0x0000
.L_02009688:
	.4byte Func_0200170c
	.section .text.x0200968c,"ax",%progbits
	.global Func_0200168c
	.thumb_func
Func_0200168c:
	push {r5, lr}
	ldr r2, .L_020096c8
	ldr r3, .L_020096bc
	ldr r5, .L_020096cc
	strh r3, [r2]
	ldr r3, .L_020096d0
	ldr r0, [r3]
	bl Func_020016e8
	ldr r2, .L_020096c0
	ldr r3, .L_020096d4
	strh r0, [r5]
	strh r2, [r3]
	ldr r3, .L_020096d8
	movs r1, #144
	strh r2, [r3]
	ldr r2, .L_020096dc
	ldr r3, .L_020096c4
	lsls r1, r1, #3
	strh r3, [r2]
	ldr r0, .L_020096e0
	bl Scheduler_AddOrUpdateCallback
	b .L_020096e4
.L_020096bc:
	.4byte 0x00000000
.L_020096c0:
	.4byte 0x00000002
.L_020096c4:
	.4byte 0x00000001
.L_020096c8:
	.4byte Data_02003938
.L_020096cc:
	.4byte Data_02003934
.L_020096d0:
	.4byte Data_02003928
.L_020096d4:
	.4byte Data_02003924
.L_020096d8:
	.4byte Data_02003920
.L_020096dc:
	.4byte Data_02003918
.L_020096e0:
	.4byte Func_0200170c
.L_020096e4:
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x020096e8,"ax",%progbits
	.global Func_020016e8
	.thumb_func
Func_020016e8:
	push {lr}
	ldr r1, .L_02009700
	ldrh r3, [r0]
	movs r2, #0
	cmp r3, r1
	beq .L_02009704
.L_020096f4:
	adds r0, #2
	ldrh r3, [r0]
	adds r2, #1
	cmp r3, r1
	bne .L_020096f4
	b .L_02009704
.L_02009700:
	.4byte 0x0000ffff
.L_02009704:
	subs r2, #1
	adds r0, r2, #0
	pop {pc}
	.2byte 0x0000
	.section .text.x0200970c,"ax",%progbits
	.global Func_0200170c
	.thumb_func
Func_0200170c:
	push {r5, r6, r7, lr}
	ldr r1, .L_020097bc
	movs r4, #0
	ldrh r2, [r1]
	adds r3, r2, #0
	cmp r3, #0
	beq .L_02009746
	movs r0, #255
	lsls r0, r0, #8
	adds r0, #255
	adds r3, r2, r0
	strh r3, [r1]
	lsls r3, r3, #16
	cmp r3, #0
	bne .L_02009746
	ldr r0, .L_020097c0
	movs r4, #1
	ldrh r2, [r0]
	strh r2, [r1]
	movs r1, #128
	lsls r3, r2, #16
	lsls r1, r1, #10
	cmp r3, r1
	bls .L_02009746
	movs r1, #255
	lsls r1, r1, #8
	adds r1, #255
	adds r3, r2, r1
	strh r3, [r0]
.L_02009746:
	cmp r4, #0
	bne .L_0200974c
	b .L_02009838
.L_0200974c:
	ldr r3, .L_020097c4
	ldr r6, .L_020097c8
	ldr r1, [r3]
	ldrh r3, [r6]
	movs r5, #0
	cmp r5, r3
	bcs .L_0200979a
	ldr r3, .L_020097cc
	ldr r2, .L_020097d0
	ldr r7, [r3]
	mov lr, r2
	mov r12, r6
.L_02009764:
	mov r3, lr
	ldrh r2, [r3]
	ldrh r3, [r6]
	movs r0, #160
	muls r3, r2
	adds r3, r3, r5
	lsls r3, r3, #1
	ldrh r3, [r3, r7]
	lsls r0, r0, #19
	lsls r3, r3, #1
	adds r4, r3, r0
	ldrh r0, [r1]
	adds r1, #2
	ldrh r2, [r1]
	adds r1, #2
	ldrh r3, [r1]
	adds r1, #2
	lsls r3, r3, #10
	lsls r2, r2, #5
	orrs r3, r2
	orrs r3, r0
	strh r3, [r4]
	adds r5, #1
	mov r2, r12
	ldrh r3, [r2]
	cmp r5, r3
	bcc .L_02009764
.L_0200979a:
	ldr r3, .L_020097c8
	movs r0, #160
	ldrh r1, [r3]
	ldr r3, .L_020097d4
	lsls r2, r1, #1
	ldr r3, [r3]
	lsls r0, r0, #19
	ldrh r3, [r2, r3]
	adds r2, r2, r1
	lsls r3, r3, #1
	adds r4, r3, r0
	ldr r3, .L_020097d8
	ldrh r3, [r3]
	cmp r3, #0
	beq .L_020097e0
	ldr r3, .L_020097dc
	b .L_020097e2
.L_020097bc:
	.4byte Data_02003920
.L_020097c0:
	.4byte Data_02003924
.L_020097c4:
	.4byte Data_02003930
.L_020097c8:
	.4byte Data_02003934
.L_020097cc:
	.4byte Data_0200391c
.L_020097d0:
	.4byte Data_02003938
.L_020097d4:
	.4byte Data_02003928
.L_020097d8:
	.4byte Data_02003918
.L_020097dc:
	.4byte Data_0200392c
.L_020097e0:
	ldr r3, .L_02009828
.L_020097e2:
	lsls r2, r2, #1
	ldr r3, [r3]
	adds r1, r3, r2
	ldrh r0, [r1]
	adds r1, #2
	ldrh r2, [r1]
	ldrh r3, [r1, #2]
	lsls r3, r3, #10
	lsls r2, r2, #5
	orrs r3, r2
	orrs r3, r0
	strh r3, [r4]
	ldr r1, .L_0200982c
	ldr r2, .L_02009820
	ldrh r3, [r1]
	eors r3, r2
	strh r3, [r1]
	ldr r1, .L_02009830
	ldr r2, .L_02009834
	ldrh r3, [r1]
	adds r3, #1
	strh r3, [r1]
	lsls r3, r3, #16
	ldrh r2, [r2]
	lsrs r3, r3, #16
	cmp r3, r2
	bne .L_02009838
	ldr r3, .L_02009824
	strh r3, [r1]
	b .L_02009838
	.2byte 0x0000
.L_02009820:
	.4byte 0x00000001
.L_02009824:
	.4byte 0x00000000
.L_02009828:
	.4byte Data_02003930
.L_0200982c:
	.4byte Data_02003918
.L_02009830:
	.4byte Data_02003938
.L_02009834:
	.4byte Data_02003934
.L_02009838:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x0200983c,"ax",%progbits
	.global Func_0200183c
	.thumb_func
Func_0200183c:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r8
	push {r5, r6}
	movs r6, #192
	lsls r6, r6, #18
	ldr r5, [r6, #108]
	movs r1, #214
	lsls r1, r1, #1
	mov r8, r1
	add r5, r8
	ldr r2, [r5]
	ldr r0, .L_020098bc
	movs r1, #1
	mov r10, r2
	bl Func_02003248
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #2
	bl Func_02003248
	ldr r2, [r6, #108]
	movs r3, #128
	lsls r3, r3, #1
	mov r1, r8
	str r3, [r2, r1]
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	bl Func_0200168c
	bl Func_020032a0
	movs r0, #40
	bl WaitFrames
	bl Func_0200166c
	movs r0, #128
	movs r1, #1
	lsls r0, r0, #9
	bl Func_02003248
	movs r0, #16
	bl Func_02003258
	movs r0, #16
	bl WaitFrames
	ldr r3, .L_020098c0
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #194
	adds r3, r3, r2
	movs r2, #0
	strb r2, [r3]
	mov r3, r10
	str r3, [r5]
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, pc}
.L_020098bc:
	.4byte 0x00202108
.L_020098c0:
	.4byte gPartyState
	.section .text.x020098c4,"ax",%progbits
	.global Func_020018c4
	.thumb_func
Func_020018c4:
	push {lr}
	ldr r4, [r0, #8]
	ldr r3, [r1, #8]
	subs r2, r4, r3
	cmp r2, #0
	blt .L_020098da
	movs r3, #128
	lsls r3, r3, #12
	cmp r2, r3
	blt .L_020098e4
	b .L_02009924
.L_020098da:
	movs r2, #128
	subs r3, r3, r4
	lsls r2, r2, #12
	cmp r3, r2
	bge .L_02009924
.L_020098e4:
	ldr r4, [r0, #12]
	ldr r3, [r1, #12]
	subs r2, r4, r3
	cmp r2, #0
	blt .L_020098f8
	movs r3, #128
	lsls r3, r3, #12
	cmp r2, r3
	blt .L_02009902
	b .L_02009924
.L_020098f8:
	movs r2, #128
	subs r3, r3, r4
	lsls r2, r2, #12
	cmp r3, r2
	bge .L_02009924
.L_02009902:
	ldr r0, [r0, #16]
	ldr r1, [r1, #16]
	subs r2, r0, r1
	cmp r2, #0
	blt .L_02009916
	movs r3, #128
	lsls r3, r3, #12
	cmp r2, r3
	blt .L_02009920
	b .L_02009924
.L_02009916:
	movs r2, #128
	subs r3, r1, r0
	lsls r2, r2, #12
	cmp r3, r2
	bge .L_02009924
.L_02009920:
	movs r0, #1
	b .L_02009926
.L_02009924:
	movs r0, #0
.L_02009926:
	pop {pc}
	.section .text.x02009928,"ax",%progbits
	.global Func_02001928
	.thumb_func
Func_02001928:
	push {lr}
	ldr r4, [r0, #8]
	ldr r3, [r1, #8]
	subs r2, r4, r3
	cmp r2, #0
	blt .L_0200993e
	movs r3, #192
	lsls r3, r3, #12
	cmp r2, r3
	blt .L_02009948
	b .L_0200997a
.L_0200993e:
	movs r2, #192
	subs r3, r3, r4
	lsls r2, r2, #12
	cmp r3, r2
	bge .L_0200997a
.L_02009948:
	ldr r2, [r1, #12]
	ldr r3, [r0, #12]
	subs r3, r3, r2
	ldr r2, .L_02009980
	adds r3, r3, r2
	ldr r2, .L_02009984
	cmp r3, r2
	bhi .L_0200997a
	ldr r0, [r0, #16]
	ldr r1, [r1, #16]
	subs r2, r0, r1
	cmp r2, #0
	blt .L_0200996c
	movs r3, #192
	lsls r3, r3, #12
	cmp r2, r3
	blt .L_02009976
	b .L_0200997a
.L_0200996c:
	movs r2, #192
	subs r3, r1, r0
	lsls r2, r2, #12
	cmp r3, r2
	bge .L_0200997a
.L_02009976:
	movs r0, #1
	b .L_0200997c
.L_0200997a:
	movs r0, #0
.L_0200997c:
	pop {pc}
	.2byte 0x0000
.L_02009980:
	.4byte 0x0007ffff
.L_02009984:
	.4byte 0x001ffffe
	.section .text.x02009988,"ax",%progbits
	.global Func_02001988
	.thumb_func
Func_02001988:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	ldr r3, .L_02009ae4
	ldr r2, .L_02009ae8
	mov r10, r3
	movs r3, #133
	lsls r3, r3, #2
	add r3, r10
	adds r6, r0, #0
	ldr r0, [r3]
	sub sp, #4
	mov r8, r2
	bl Object_GetById
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	ldr r5, [r6, #68]
	mov r9, r3
	ldr r3, [r6, #8]
	ldr r2, [r6, #72]
	adds r3, r3, r5
	str r3, [r6, #8]
	ldr r3, [r6, #12]
	ldr r7, [r6, #76]
	adds r3, r3, r2
	str r3, [r6, #12]
	ldr r3, [r6, #16]
	adds r4, r0, #0
	adds r3, r3, r7
	adds r0, r5, #0
	movs r1, #18
	str r3, [r6, #16]
	str r4, [sp, #0]
	bl __divsi3
	subs r5, r5, r0
	str r5, [r6, #68]
	adds r3, r7, #0
	ldr r4, [sp, #0]
	cmp r7, #0
	bge .L_020099e4
	adds r3, #15
.L_020099e4:
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
	ldr r2, [r4, #12]
	ldr r3, [r4, #20]
	cmp r2, r3
	bne .L_02009a76
	adds r0, r6, #0
	adds r1, r4, #0
	bl Func_020018c4
	cmp r0, #0
	beq .L_02009a76
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #18
	add r3, r10
	ldrb r2, [r3]
	cmp r2, #0
	bne .L_02009a76
	ldr r1, [r6, #76]
	cmp r1, #0
	beq .L_02009a4a
	mov r3, r8
	adds r3, #104
	strh r2, [r3]
	mov r2, r8
	adds r2, #106
	cmp r1, #0
	ble .L_02009a42
	movs r3, #1
	b .L_02009a48
.L_02009a42:
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
.L_02009a48:
	strh r3, [r2]
.L_02009a4a:
	adds r2, r6, #0
	adds r2, #35
	movs r3, #2
	strb r3, [r2]
	movs r1, #0
	ldr r2, [r6, #76]
	ldr r3, [r6, #16]
	str r1, [r6, #76]
	subs r3, r3, r2
	str r3, [r6, #16]
	ldr r3, [r6, #72]
	asrs r2, r2, #2
	adds r3, r3, r2
	str r3, [r6, #72]
	mov r2, r8
	movs r3, #1
	strh r3, [r2, #4]
	ldrh r2, [r2, #10]
	movs r3, #170
	lsls r3, r3, #1
	add r3, r9
	strh r2, [r3]
.L_02009a76:
	movs r3, #84
	mov r2, r8
	ldrh r0, [r2, r3]
	movs r7, #0
	cmp r0, #0
	beq .L_02009ad6
	mov r5, r8
	adds r5, #84
.L_02009a86:
	bl Object_GetById
	adds r4, r0, #0
	ldr r2, [r4, #12]
	ldr r3, [r4, #20]
	cmp r2, r3
	bne .L_02009ac8
	adds r0, r6, #0
	adds r1, r4, #0
	bl Func_02001928
	cmp r0, #0
	beq .L_02009ac8
	ldrh r1, [r5, #2]
	cmp r1, #0
	bne .L_02009ac2
	adds r2, r6, #0
	adds r2, #35
	movs r3, #2
	strb r3, [r2]
	ldr r2, [r6, #76]
	ldr r3, [r6, #16]
	str r1, [r6, #76]
	subs r3, r3, r2
	str r3, [r6, #16]
	ldr r3, [r6, #72]
	asrs r2, r2, #2
	adds r3, r3, r2
	str r3, [r6, #72]
	b .L_02009ac8
.L_02009ac2:
	movs r3, #1
	mov r2, r8
	strh r3, [r2, #6]
.L_02009ac8:
	adds r7, #1
	adds r5, #4
	cmp r7, #3
	bgt .L_02009ad6
	ldrh r0, [r5]
	cmp r0, #0
	bne .L_02009a86
.L_02009ad6:
	add sp, #4
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009ae4:
	.4byte gPartyState
.L_02009ae8:
	.4byte Data_020023c4 + 0x188
	.section .text.x02009aec,"ax",%progbits
	.global Func_02001aec
	.thumb_func
Func_02001aec:
	push {r5, r6, lr}
	adds r5, r0, #0
	adds r3, r5, #0
	movs r6, #0
	adds r3, #85
	strb r6, [r3]
	adds r3, #4
	strb r6, [r3]
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	ldr r1, .L_02009b18
	adds r0, r5, #0
	bl Func_02003120
	adds r3, r5, #0
	adds r3, #100
	strh r6, [r3]
	str r6, [r5, #48]
	str r6, [r5, #52]
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02009b18:
	.4byte Data_020032d4
	.section .text.x02009b1c,"ax",%progbits
	.global Func_02001b1c
	.thumb_func
Func_02001b1c:
	push {r5, r6, r7, lr}
	ldr r3, [r0, #8]
	adds r6, r1, #0
	adds r7, r2, #0
	ldr r1, [r0]
	ldr r2, [r0, #4]
	movs r0, #183
	lsls r0, r0, #1
	bl Func_02003128
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02009b5c
	movs r3, #0
	str r3, [r5, #68]
	str r3, [r5, #72]
	str r6, [r5, #76]
	movs r1, #2
	bl Func_02001aec
	ldr r3, .L_02009b60
	str r3, [r5, #108]
	cmp r7, #0
	beq .L_02009b54
	adds r2, r5, #0
	adds r2, #92
	movs r3, #2
	strb r3, [r2]
.L_02009b54:
	adds r0, r5, #0
	movs r1, #2
	bl Func_02003118
.L_02009b5c:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009b60:
	.4byte Func_02001988
	.section .text.x02009b64,"ax",%progbits
	.global Func_02001b64
	.thumb_func
Func_02001b64:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	ldr r3, .L_02009cbc
	ldr r2, .L_02009cc0
	mov r10, r3
	movs r3, #133
	lsls r3, r3, #2
	add r3, r10
	adds r6, r0, #0
	ldr r0, [r3]
	sub sp, #4
	mov r8, r2
	bl Object_GetById
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	ldr r5, [r6, #68]
	mov r9, r3
	ldr r3, [r6, #16]
	ldr r2, [r6, #72]
	adds r3, r3, r5
	str r3, [r6, #16]
	ldr r3, [r6, #12]
	ldr r7, [r6, #76]
	adds r3, r3, r2
	str r3, [r6, #12]
	ldr r3, [r6, #8]
	adds r4, r0, #0
	adds r3, r3, r7
	adds r0, r5, #0
	movs r1, #18
	str r3, [r6, #8]
	str r4, [sp, #0]
	bl __divsi3
	subs r5, r5, r0
	str r5, [r6, #68]
	adds r3, r7, #0
	ldr r4, [sp, #0]
	cmp r7, #0
	bge .L_02009bc0
	adds r3, #15
.L_02009bc0:
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
	ldr r2, [r4, #12]
	ldr r3, [r4, #20]
	cmp r2, r3
	bne .L_02009c50
	adds r0, r6, #0
	adds r1, r4, #0
	bl Func_020018c4
	cmp r0, #0
	beq .L_02009c50
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #18
	add r3, r10
	ldrb r3, [r3]
	cmp r3, #0
	bne .L_02009c50
	ldr r2, [r6, #76]
	cmp r2, #0
	beq .L_02009c24
	mov r1, r8
	adds r1, #106
	strh r3, [r1]
	subs r1, #2
	cmp r2, #0
	ble .L_02009c1c
	movs r3, #1
	b .L_02009c22
.L_02009c1c:
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
.L_02009c22:
	strh r3, [r1]
.L_02009c24:
	adds r2, r6, #0
	adds r2, #35
	movs r3, #2
	strb r3, [r2]
	movs r1, #0
	ldr r3, [r6, #16]
	ldr r2, [r6, #68]
	str r1, [r6, #76]
	subs r3, r3, r2
	str r3, [r6, #16]
	ldr r3, [r6, #72]
	asrs r2, r2, #2
	adds r3, r3, r2
	str r3, [r6, #72]
	mov r2, r8
	movs r3, #1
	strh r3, [r2, #4]
	ldrh r2, [r2, #10]
	movs r3, #170
	lsls r3, r3, #1
	add r3, r9
	strh r2, [r3]
.L_02009c50:
	movs r3, #84
	mov r2, r8
	ldrh r0, [r2, r3]
	movs r7, #0
	cmp r0, #0
	beq .L_02009cb0
	mov r5, r8
	adds r5, #84
.L_02009c60:
	bl Object_GetById
	adds r4, r0, #0
	ldr r2, [r4, #12]
	ldr r3, [r4, #20]
	cmp r2, r3
	bne .L_02009ca2
	adds r0, r6, #0
	adds r1, r4, #0
	bl Func_02001928
	cmp r0, #0
	beq .L_02009ca2
	ldrh r1, [r5, #2]
	cmp r1, #0
	bne .L_02009c9c
	adds r2, r6, #0
	adds r2, #35
	movs r3, #2
	strb r3, [r2]
	ldr r2, [r6, #76]
	ldr r3, [r6, #16]
	str r1, [r6, #76]
	subs r3, r3, r2
	str r3, [r6, #16]
	ldr r3, [r6, #72]
	asrs r2, r2, #2
	adds r3, r3, r2
	str r3, [r6, #72]
	b .L_02009ca2
.L_02009c9c:
	movs r3, #1
	mov r2, r8
	strh r3, [r2, #6]
.L_02009ca2:
	adds r7, #1
	adds r5, #4
	cmp r7, #3
	bgt .L_02009cb0
	ldrh r0, [r5]
	cmp r0, #0
	bne .L_02009c60
.L_02009cb0:
	add sp, #4
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_02009cbc:
	.4byte gPartyState
.L_02009cc0:
	.4byte Data_020023c4 + 0x188
	.section .text.x02009cc4,"ax",%progbits
	.global Func_02001cc4
	.thumb_func
Func_02001cc4:
	push {r5, r6, lr}
	adds r5, r0, #0
	adds r3, r5, #0
	movs r6, #0
	adds r3, #85
	strb r6, [r3]
	adds r3, #4
	strb r6, [r3]
	movs r3, #3
	ldr r0, [r5, #80]
	ands r1, r3
	ldrb r2, [r0, #9]
	movs r3, #13
	negs r3, r3
	ands r3, r2
	lsls r1, r1, #2
	orrs r3, r1
	strb r3, [r0, #9]
	movs r1, #0
	adds r0, r5, #0
	bl ObjectDispatch_SetSingleChildField26
	ldr r1, .L_02009d04
	adds r0, r5, #0
	bl Func_02003120
	adds r3, r5, #0
	adds r3, #100
	strh r6, [r3]
	str r6, [r5, #48]
	str r6, [r5, #52]
	pop {r5, r6, pc}
.L_02009d04:
	.4byte Data_020032d4
	.section .text.x02009d08,"ax",%progbits
	.global Func_02001d08
	.thumb_func
Func_02001d08:
	push {r5, r6, r7, lr}
	ldr r3, [r0, #8]
	adds r6, r1, #0
	adds r7, r2, #0
	ldr r1, [r0]
	ldr r2, [r0, #4]
	movs r0, #183
	lsls r0, r0, #1
	bl Func_02003128
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02009d4a
	movs r3, #0
	str r3, [r5, #68]
	str r3, [r5, #72]
	negs r3, r6
	str r3, [r5, #76]
	movs r1, #3
	bl Func_02001cc4
	ldr r3, .L_02009d4c
	str r3, [r5, #108]
	cmp r7, #0
	beq .L_02009d42
	adds r2, r5, #0
	adds r2, #92
	movs r3, #2
	strb r3, [r2]
.L_02009d42:
	adds r0, r5, #0
	movs r1, #2
	bl Func_02003118
.L_02009d4a:
	pop {r5, r6, r7, pc}
.L_02009d4c:
	.4byte Func_02001988
	.section .text.x02009d50,"ax",%progbits
	.global Func_02001d50
	.thumb_func
Func_02001d50:
	push {r5, r6, r7, lr}
	ldr r3, [r0, #8]
	adds r6, r1, #0
	adds r7, r2, #0
	ldr r1, [r0]
	ldr r2, [r0, #4]
	movs r0, #183
	lsls r0, r0, #1
	bl Func_02003128
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02009d90
	movs r3, #0
	str r3, [r5, #68]
	str r3, [r5, #72]
	str r6, [r5, #76]
	movs r1, #3
	bl Func_02001cc4
	ldr r3, .L_02009d94
	str r3, [r5, #108]
	cmp r7, #0
	beq .L_02009d88
	adds r2, r5, #0
	adds r2, #92
	movs r3, #2
	strb r3, [r2]
.L_02009d88:
	adds r0, r5, #0
	movs r1, #2
	bl Func_02003118
.L_02009d90:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009d94:
	.4byte Func_02001b64
	.section .text.x02009d98,"ax",%progbits
	.global Func_02001d98
	.thumb_func
Func_02001d98:
	push {r5, r6, r7, lr}
	ldr r3, [r0, #8]
	adds r6, r1, #0
	adds r7, r2, #0
	ldr r1, [r0]
	ldr r2, [r0, #4]
	movs r0, #183
	lsls r0, r0, #1
	bl Func_02003128
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02009dda
	movs r3, #0
	str r3, [r5, #68]
	str r3, [r5, #72]
	negs r3, r6
	str r3, [r5, #76]
	movs r1, #3
	bl Func_02001cc4
	ldr r3, .L_02009ddc
	str r3, [r5, #108]
	cmp r7, #0
	beq .L_02009dd2
	adds r2, r5, #0
	adds r2, #92
	movs r3, #2
	strb r3, [r2]
.L_02009dd2:
	adds r0, r5, #0
	movs r1, #2
	bl Func_02003118
.L_02009dda:
	pop {r5, r6, r7, pc}
.L_02009ddc:
	.4byte Func_02001b64
	.section .text.x02009de0,"ax",%progbits
	.global Func_02001de0
	.thumb_func
Func_02001de0:
	push {lr}
	ldr r4, [r0, #8]
	ldr r3, [r1, #8]
	subs r2, r4, r3
	cmp r2, #0
	blt .L_02009df6
	movs r3, #192
	lsls r3, r3, #12
	cmp r2, r3
	blt .L_02009e00
	b .L_02009e30
.L_02009df6:
	movs r2, #192
	subs r3, r3, r4
	lsls r2, r2, #12
	cmp r3, r2
	bge .L_02009e30
.L_02009e00:
	ldr r2, [r1, #12]
	ldr r3, [r0, #12]
	subs r3, r3, r2
	ldr r2, .L_02009e34
	subs r3, #1
	cmp r3, r2
	bhi .L_02009e30
	ldr r0, [r0, #16]
	ldr r1, [r1, #16]
	subs r2, r0, r1
	cmp r2, #0
	blt .L_02009e22
	movs r3, #128
	lsls r3, r3, #12
	cmp r2, r3
	blt .L_02009e2c
	b .L_02009e30
.L_02009e22:
	movs r2, #128
	subs r3, r1, r0
	lsls r2, r2, #12
	cmp r3, r2
	bge .L_02009e30
.L_02009e2c:
	movs r0, #1
	b .L_02009e32
.L_02009e30:
	movs r0, #0
.L_02009e32:
	pop {pc}
.L_02009e34:
	.4byte 0x000ffffe
	.section .text.x02009e38,"ax",%progbits
	.global Func_02001e38
	.thumb_func
Func_02001e38:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r3, .L_02009ed0
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	adds r6, r0, #0
	ldr r0, [r3]
	bl Object_GetById
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
	mov r8, r0
	adds r3, r3, r7
	adds r0, r5, #0
	movs r1, #18
	str r3, [r6, #16]
	bl __divsi3
	subs r5, r5, r0
	str r5, [r6, #68]
	adds r3, r7, #0
	cmp r7, #0
	bge .L_02009e7c
	adds r3, #15
.L_02009e7c:
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
	mov r3, r8
	ldr r2, [r3, #12]
	ldr r3, [r3, #20]
	cmp r2, r3
	bne .L_02009eca
	adds r0, r6, #0
	mov r1, r8
	bl Func_02001de0
	cmp r0, #0
	beq .L_02009eca
	ldr r2, [r6, #76]
	ldr r3, [r6, #16]
	subs r3, r3, r2
	str r3, [r6, #16]
	ldr r3, [r6, #72]
	asrs r2, r2, #2
	adds r3, r3, r2
	str r3, [r6, #72]
	movs r3, #0
	str r3, [r6, #76]
.L_02009eca:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_02009ed0:
	.4byte gPartyState
	.section .text.x02009ed4,"ax",%progbits
	.global Func_02001ed4
	.thumb_func
Func_02001ed4:
	push {r5, r6, lr}
	adds r5, r0, #0
	adds r3, r5, #0
	movs r6, #0
	adds r3, #85
	strb r6, [r3]
	adds r3, #4
	strb r6, [r3]
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	adds r0, r5, #0
	movs r1, #2
	bl Func_02003118
	adds r0, r5, #0
	ldr r1, .L_02009f10
	bl Func_02003120
	adds r0, r5, #0
	movs r1, #10
	bl Object_SetPartAttribute
	adds r3, r5, #0
	adds r3, #100
	strh r6, [r3]
	str r6, [r5, #48]
	str r6, [r5, #52]
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02009f10:
	.4byte Data_020032d4
	.section .text.x02009f14,"ax",%progbits
	.global Func_02001f14
	.thumb_func
Func_02001f14:
	push {r5, r6, lr}
	ldr r3, [r0, #8]
	ldr r2, [r0, #4]
	adds r6, r1, #0
	ldr r1, [r0]
	ldr r0, .L_02009f88
	adds r3, r3, r0
	movs r0, #30
	adds r0, #255
	bl Func_02003128
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02009f86
	bl Random16Far
	adds r3, r0, #0
	lsls r0, r3, #1
	adds r0, r0, r3
	lsls r0, r0, #12
	movs r3, #192
	lsls r3, r3, #6
	lsrs r0, r0, #16
	adds r0, r0, r3
	bl Math_Cosine
	str r0, [r5, #68]
	bl Random16Far
	lsls r0, r0, #16
	lsrs r0, r0, #16
	str r0, [r5, #72]
	bl Random16Far
	lsls r0, r0, #17
	lsrs r0, r0, #16
	adds r0, r0, r6
	str r0, [r5, #76]
	bl Random16Far
	ldr r3, .L_02009f8c
	lsls r0, r0, #16
	lsrs r0, r0, #16
	adds r0, r0, r3
	adds r3, r5, #0
	adds r3, #100
	strh r0, [r3]
	movs r1, #2
	adds r0, r5, #0
	bl Func_02001ed4
	ldr r3, .L_02009f90
	adds r0, r5, #0
	str r3, [r5, #108]
	movs r1, #1
	bl Animation_SetStateFlags
.L_02009f86:
	pop {r5, r6, pc}
.L_02009f88:
	.4byte 0xfffe0000
.L_02009f8c:
	.4byte 0xffff8000
.L_02009f90:
	.4byte Func_02001e38
	.section .text.x02009f94,"ax",%progbits
	.global Func_02001f94
	.thumb_func
Func_02001f94:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r0, .L_0200a110
	sub sp, #8
	mov r8, r0
	movs r0, #192
	lsls r0, r0, #18
	ldr r1, [r0, #32]
	mov r7, r8
	adds r2, r1, #0
	adds r2, #228
	ldr r3, [r2]
	ldr r2, [r2, #4]
	mov r9, r3
	ldr r3, .L_0200a114
	mov r4, r9
	ands r4, r3
	ands r2, r3
	ldr r3, [r1]
	mov r9, r4
	ldr r3, [r3, #4]
	adds r7, #20
	str r3, [sp, #4]
	mov r10, r2
	ldr r0, [r0, #108]
	str r0, [sp, #0]
	mov r0, r8
	movs r4, #6
	ldrsh r3, [r0, r4]
	cmp r3, #0
	beq .L_02009fee
	movs r1, #8
	ldrsh r3, [r0, r1]
	cmp r3, #0
	bne .L_02009fee
	ldr r3, [r0, #16]
	cmp r3, #0
	beq .L_02009fee
	mov lr, r3
	.2byte 0xf800
.L_02009fee:
	mov r2, r8
	ldrh r3, [r2, #6]
	mov r4, r8
	movs r2, #0
	mov r0, r8
	strh r3, [r4, #8]
	strh r2, [r0, #6]
	movs r1, #3
	mov r11, r1
.L_0200a000:
	movs r2, #0
	ldrsh r3, [r7, r2]
	cmp r3, #0
	beq .L_0200a0f0
	ldr r5, [r7, #8]
	cmp r5, #0
	beq .L_0200a0f0
	movs r0, #131
	lsls r0, r0, #1
	bl GameFlag_Test
	ldr r4, [sp, #0]
	movs r1, #179
	lsls r1, r1, #1
	adds r3, r4, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	beq .L_0200a02a
	movs r3, #1
	orrs r0, r3
.L_0200a02a:
	adds r6, r5, #0
	adds r6, #91
	strb r0, [r6]
	mov r0, r8
	movs r4, #14
	ldrsh r3, [r0, r4]
	cmp r3, #0
	beq .L_0200a044
	movs r0, #131
	lsls r0, r0, #1
	bl GameFlag_Test
	strb r0, [r6]
.L_0200a044:
	ldr r2, [r5, #8]
	mov r1, r9
	ldr r3, [r5, #16]
	subs r2, r2, r1
	ldr r1, [r5, #12]
	mov r4, r10
	subs r3, r3, r4
	subs r1, r3, r1
	movs r0, #6
	ldrsh r4, [r7, r0]
	asrs r3, r1, #16
	adds r1, r3, #0
	asrs r2, r2, #16
	subs r1, #8
	cmp r4, #0
	bne .L_0200a07a
	adds r3, r2, #7
	movs r2, #167
	lsls r2, r2, #1
	cmp r3, r2
	bhi .L_0200a0f0
	movs r3, #48
	negs r3, r3
	cmp r1, r3
	ble .L_0200a0f0
	cmp r1, #239
	bgt .L_0200a0f0
.L_0200a07a:
	movs r0, #2
	ldrsh r3, [r7, r0]
	ldrh r1, [r7, #2]
	cmp r3, #0
	bgt .L_0200a0ec
	ldrh r3, [r7, #4]
	movs r1, #240
	ands r1, r3
	cmp r1, #32
	beq .L_0200a0c0
	cmp r1, #32
	bgt .L_0200a09c
	cmp r1, #0
	beq .L_0200a0dc
	cmp r1, #16
	beq .L_0200a0ce
	b .L_0200a0e8
.L_0200a09c:
	cmp r1, #48
	beq .L_0200a0b2
	cmp r1, #128
	bne .L_0200a0e8
	ldr r0, [r7, #8]
	ldr r1, [r7, #12]
	adds r0, #8
	adds r2, r4, #0
	bl Func_02001f14
	b .L_0200a0e8
.L_0200a0b2:
	ldr r0, [r7, #8]
	ldr r1, [r7, #12]
	adds r0, #8
	adds r2, r4, #0
	bl Func_02001d08
	b .L_0200a0e8
.L_0200a0c0:
	ldr r0, [r7, #8]
	ldr r1, [r7, #12]
	adds r0, #8
	adds r2, r4, #0
	bl Func_02001d98
	b .L_0200a0e8
.L_0200a0ce:
	ldr r0, [r7, #8]
	ldr r1, [r7, #12]
	adds r0, #8
	adds r2, r4, #0
	bl Func_02001d50
	b .L_0200a0e8
.L_0200a0dc:
	ldr r0, [r7, #8]
	ldr r1, [r7, #12]
	adds r0, #8
	adds r2, r4, #0
	bl Func_02001b1c
.L_0200a0e8:
	movs r3, #8
	b .L_0200a0ee
.L_0200a0ec:
	subs r3, r1, #1
.L_0200a0ee:
	strh r3, [r7, #2]
.L_0200a0f0:
	movs r1, #1
	negs r1, r1
	add r11, r1
	mov r2, r11
	adds r7, #16
	cmp r2, #0
	blt .L_0200a100
	b .L_0200a000
.L_0200a100:
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200a110:
	.4byte Data_020023c4 + 0x188
.L_0200a114:
	.4byte 0xffff0000
	.section .text.x0200a118,"ax",%progbits
	.global Func_02002118
	.thumb_func
Func_02002118:
	push {r5, r6, lr}
	movs r0, #10
	adds r0, #255
	ldr r6, .L_0200a164
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a132
	ldr r3, .L_0200a168
	adds r0, r6, #0
	movs r1, #116
	mov lr, r3
	.2byte 0xf800
.L_0200a132:
	movs r0, #110
	movs r1, #1
	movs r2, #0
	movs r3, #0
	adds r0, #255
	bl Func_02003128
	adds r5, r0, #0
	adds r2, r5, #0
	adds r2, #92
	movs r3, #2
	strb r3, [r2]
	movs r1, #1
	bl Func_02003118
	ldr r1, [r5, #80]
	movs r2, #1
	ldrb r3, [r1, #16]
	str r5, [r6, #112]
	strh r3, [r6, #12]
	ldrb r3, [r1, #17]
	orrs r3, r2
	strb r3, [r1, #17]
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200a164:
	.4byte Data_020023c4 + 0x188
.L_0200a168:
	.4byte IwramClearWords
	.section .text.x0200a16c,"ax",%progbits
	.global Func_0200216c
	.thumb_func
Func_0200216c:
	push {r5, lr}
	ldr r5, .L_0200a17c
	ldr r0, [r5, #112]
	bl Func_02003130
	movs r3, #0
	str r3, [r5, #112]
	pop {r5, pc}
.L_0200a17c:
	.4byte Data_020023c4 + 0x188
	.section .text.x0200a180,"ax",%progbits
	.global Func_02002180
	.thumb_func
Func_02002180:
	ldr r3, .L_0200a188
	strh r0, [r3, #14]
	bx lr
	.2byte 0x0000
.L_0200a188:
	.4byte Data_020023c4 + 0x188
	.section .text.x0200a18c,"ax",%progbits
	.global Func_0200218c
	.thumb_func
Func_0200218c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	mov r8, r1
	ldr r1, .L_0200a2b8
	sub sp, #16
	adds r6, r0, #0
	movs r0, #10
	str r1, [sp, #4]
	adds r0, #255
	adds r1, #20
	str r2, [sp, #12]
	str r3, [sp, #8]
	mov r9, r1
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a270
	ldrh r3, [r6]
	movs r2, #0
	mov r11, r2
	mov r10, r3
	adds r6, #2
	cmp r3, #0
	ble .L_0200a22e
.L_0200a1c6:
	ldrh r7, [r6]
	movs r1, #15
	ands r1, r7
	movs r3, #240
	mov r0, r10
	str r1, [sp, #0]
	ands r7, r3
	bl Object_GetById
	adds r5, r0, #0
	adds r6, #2
	cmp r5, #0
	beq .L_0200a21a
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	adds r3, r5, #0
	movs r2, #128
	adds r3, #98
	movs r1, #1
	ands r2, r7
	strb r1, [r3]
	cmp r2, #0
	bne .L_0200a1fa
	subs r3, #9
	strb r2, [r3]
.L_0200a1fa:
	mov r2, r9
	mov r3, r9
	strh r1, [r2]
	mov r0, r10
	strh r7, [r3, #4]
	bl Object_GetById
	mov r1, r9
	str r0, [r1, #8]
	ldr r2, [sp, #0]
	lsls r3, r2, #16
	str r3, [r1, #12]
	mov r3, r11
	strh r3, [r1, #2]
	movs r2, #16
	add r9, r2
.L_0200a21a:
	movs r3, #1
	add r11, r3
	mov r1, r11
	cmp r1, #3
	bgt .L_0200a22e
	ldrh r2, [r6]
	adds r6, #2
	mov r10, r2
	cmp r2, #0
	bgt .L_0200a1c6
.L_0200a22e:
	mov r3, r8
	cmp r3, #0
	beq .L_0200a270
	movs r1, #0
	ldrh r2, [r3]
	mov r11, r1
	ldr r1, [sp, #4]
	movs r3, #2
	add r8, r3
	movs r3, #84
	strh r2, [r1, r3]
	cmp r2, #0
	ble .L_0200a270
	adds r2, r1, #0
	adds r2, #84
.L_0200a24c:
	mov r1, r8
	ldrh r3, [r1]
	movs r1, #1
	strh r3, [r2, #2]
	add r11, r1
	movs r3, #2
	add r8, r3
	mov r3, r11
	adds r2, #4
	cmp r3, #3
	bgt .L_0200a270
	mov r1, r8
	ldrh r3, [r1]
	movs r1, #2
	add r8, r1
	strh r3, [r2]
	cmp r3, #0
	bgt .L_0200a24c
.L_0200a270:
	ldr r2, [sp, #12]
	ldr r3, [sp, #4]
	add r1, sp, #8
	str r2, [r3, #16]
	ldrh r1, [r1]
	ldr r2, [sp, #4]
	strh r1, [r2, #10]
	movs r1, #128
	lsls r1, r1, #19
	adds r1, #80
	ldrh r3, [r1]
	cmp r3, #0
	bne .L_0200a2a0
	movs r3, #192
	movs r2, #128
	lsls r3, r3, #4
	lsls r2, r2, #19
	adds r3, #8
	adds r2, #82
	strh r3, [r2]
	movs r3, #252
	lsls r3, r3, #6
	adds r3, #16
	strh r3, [r1]
.L_0200a2a0:
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_0200a2bc
	bl Scheduler_AddOrUpdateCallback
	add sp, #16
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0200a2b8:
	.4byte Data_020023c4 + 0x188
.L_0200a2bc:
	.4byte Func_02001f94
	.section .text.x0200a2c0,"ax",%progbits
	.global Func_020022c0
	.thumb_func
Func_020022c0:
	ldr r3, .L_0200a2cc
	lsls r0, r0, #4
	adds r0, r0, r3
	movs r3, #20
	ldrsh r0, [r0, r3]
	bx lr
.L_0200a2cc:
	.4byte Data_020023c4 + 0x188
	.section .text.x0200a2d0,"ax",%progbits
	.global Func_020022d0
	.thumb_func
Func_020022d0:
	ldr r3, .L_0200a2dc
	lsls r0, r0, #4
	adds r0, r0, r3
	strh r1, [r0, #20]
	bx lr
	.2byte 0x0000
.L_0200a2dc:
	.4byte Data_020023c4 + 0x188
	.section .text.x0200a2e0,"ax",%progbits
	.global Func_020022e0
	.thumb_func
Func_020022e0:
	ldr r3, .L_0200a2e8
	ldr r0, [r3]
	bx lr
	.2byte 0x0000
.L_0200a2e8:
	.4byte Data_020023c4 + 0x188
	.section .text.x0200a2ec,"ax",%progbits
	.global Func_020022ec
	.thumb_func
Func_020022ec:
	push {r5, r6, lr}
	adds r6, r0, #0
	movs r0, #130
	lsls r0, r0, #1
	ldr r5, .L_0200a310
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a30a
	ldr r3, [r5]
	adds r3, #1
	str r3, [r5]
	cmp r3, r6
	blt .L_0200a30a
	str r0, [r5]
.L_0200a30a:
	ldr r0, [r5]
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200a310:
	.4byte Data_020023c4 + 0x188
	.section .text.x0200a314,"ax",%progbits
	.global Func_02002314
	.thumb_func
Func_02002314:
	ldr r3, .L_0200a330
	lsls r0, r0, #4
	adds r0, r0, r3
	movs r3, #15
	ands r3, r1
	adds r0, #20
	lsls r3, r3, #16
	str r3, [r0, #12]
	ldr r3, .L_0200a32c
	ands r1, r3
	strh r1, [r0, #4]
	bx lr
.L_0200a32c:
	.4byte 0x000000f0
.L_0200a330:
	.4byte Data_020023c4 + 0x188
	.section .text.x0200a334,"ax",%progbits
	.global Func_02002334
	.thumb_func
Func_02002334:
	ldr r3, .L_0200a340
	lsls r0, r0, #4
	adds r0, r0, r3
	strh r1, [r0, #26]
	bx lr
	.2byte 0x0000
.L_0200a340:
	.4byte Data_020023c4 + 0x188
	.section .text.x0200a344,"ax",%progbits
	.global Func_02002344
	.thumb_func
Func_02002344:
	ldr r3, .L_0200a350
	lsls r0, r0, #4
	adds r0, r0, r3
	movs r3, #24
	ldrsh r0, [r0, r3]
	bx lr
.L_0200a350:
	.4byte Data_020023c4 + 0x188
	.section .text.x0200a354,"ax",%progbits
	.global Func_02002354
	.thumb_func
Func_02002354:
	push {lr}
	ldr r2, .L_0200a364
	cmp r0, #3
	bhi .L_0200a362
	lsls r3, r0, #2
	adds r3, #84
	strh r1, [r2, r3]
.L_0200a362:
	pop {pc}
.L_0200a364:
	.4byte Data_020023c4 + 0x188
	.section .text.x0200a368,"ax",%progbits
	.global Func_02002368
	.thumb_func
Func_02002368:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	sub sp, #16
	ldr r6, [r3, #108]
	bl Func_02003298
	mov r8, r0
	bl Object_GetById
	bl Party_CountActiveOwners
	movs r5, #0
	adds r7, r0, #0
	cmp r5, r7
	bge .L_0200a3aa
.L_0200a38e:
	ldr r2, .L_0200a450
	movs r1, #134
	lsls r1, r1, #2
	adds r3, r5, r1
	ldrb r0, [r2, r3]
	bl Owner_GetState
	ldrh r3, [r0, #56]
	lsls r2, r5, #1
	mov r1, sp
	adds r5, #1
	strh r3, [r1, r2]
	cmp r5, r7
	blt .L_0200a38e
.L_0200a3aa:
	movs r0, #10
	negs r0, r0
	movs r1, #0
	bl Func_02003288
	movs r2, #182
	lsls r2, r2, #1
	movs r4, #183
	adds r3, r6, r2
	lsls r4, r4, #1
	movs r2, #0
	strh r2, [r3]
	movs r1, #129
	adds r3, r6, r4
	strh r2, [r3]
	mov r0, r8
	lsls r1, r1, #1
	movs r5, #0
	bl Func_02003210
	cmp r5, r7
	bge .L_0200a444
.L_0200a3d6:
	ldr r1, .L_0200a450
	movs r2, #134
	lsls r2, r2, #2
	adds r2, r2, r5
	ldrb r0, [r1, r2]
	mov r10, r1
	mov r8, r2
	bl Owner_GetState
	movs r4, #56
	ldrsh r3, [r0, r4]
	cmp r3, #0
	ble .L_0200a3fe
	movs r1, #183
	lsls r1, r1, #1
	adds r2, r6, r1
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_0200a43e
.L_0200a3fe:
	mov r3, sp
	lsls r2, r5, #1
	ldrsh r3, [r3, r2]
	cmp r3, #0
	beq .L_0200a43e
	movs r2, #182
	lsls r2, r2, #1
	adds r1, r6, r2
	ldrh r3, [r1]
	movs r4, #184
	adds r2, r3, #1
	lsls r3, r3, #16
	lsls r4, r4, #1
	asrs r3, r3, #15
	strh r2, [r1]
	adds r3, r3, r4
	mov r1, r10
	mov r4, r8
	ldrb r2, [r1, r4]
	movs r1, #181
	strh r2, [r6, r3]
	movs r3, #255
	lsls r1, r1, #1
	lsls r3, r3, #8
	adds r2, r6, r1
	adds r3, #255
	strh r3, [r2]
	movs r3, #50
	adds r3, #255
	adds r2, r0, r3
	movs r3, #0
	strb r3, [r2]
.L_0200a43e:
	adds r5, #1
	cmp r5, r7
	blt .L_0200a3d6
.L_0200a444:
	add sp, #16
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200a450:
	.4byte gPartyState
	.section .text.x0200a454,"ax",%progbits
	.global Func_02002454
	.thumb_func
Func_02002454:
	push {r5, r6, r7, lr}
	ldr r3, .L_0200a4b8
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	adds r6, r0, #0
	ldr r0, [r3]
	bl Object_GetById
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
	bge .L_0200a492
	adds r3, #15
.L_0200a492:
	asrs r3, r3, #4
	subs r3, r7, r3
	str r3, [r6, #76]
	ldr r2, [r6, #48]
	ldr r3, [r6, #24]
	movs r1, #128
	adds r3, r3, r2
	str r3, [r6, #24]
	ldr r2, [r6, #52]
	ldr r3, [r6, #28]
	lsls r1, r1, #5
	adds r3, r3, r2
	str r3, [r6, #28]
	ldr r2, [r6, #80]
	ldrh r3, [r2, #18]
	adds r3, r3, r1
	strh r3, [r2, #18]
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200a4b8:
	.4byte gPartyState
	.section .text.x0200a4bc,"ax",%progbits
	.global Func_020024bc
	.thumb_func
Func_020024bc:
	push {lr}
	ldr r3, .L_0200a4e0
	ldr r2, [r3]
	movs r3, #7
	ands r2, r3
	cmp r2, #0
	beq .L_0200a4d0
	cmp r2, #4
	beq .L_0200a4d8
	b .L_0200a4de
.L_0200a4d0:
	movs r1, #10
	bl Animation_ApplyChildValues
	b .L_0200a4de
.L_0200a4d8:
	movs r1, #0
	bl Animation_ApplyChildValues
.L_0200a4de:
	pop {pc}
.L_0200a4e0:
	.4byte gFrameCount
	.section .text.x0200a4e4,"ax",%progbits
	.global Func_020024e4
	.thumb_func
Func_020024e4:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r5, .L_0200a6d8
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #18
	adds r3, r5, r0
	ldrb r3, [r3]
	sub sp, #16
	cmp r3, #0
	beq .L_0200a506
	b .L_0200a6c4
.L_0200a506:
	movs r0, #10
	movs r1, #0
	negs r0, r0
	bl Func_02002368
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r5, r1
	ldr r0, [r3]
	bl Object_GetById
	ldr r2, .L_0200a6dc
	adds r6, r0, #0
	str r2, [sp, #0]
	bl Func_020031e0
	movs r0, #0
	bl Func_02003280
	ldr r3, .L_0200a6e0
	adds r0, r6, #0
	str r3, [r6, #108]
	movs r3, #128
	lsls r3, r3, #7
	strh r3, [r6, #6]
	movs r1, #49
	bl Func_02003118
.L_0200a53e:
	adds r3, r6, #0
	adds r3, #34
	ldrb r0, [r3]
	ldr r1, [r6, #8]
	ldr r2, [r6, #16]
	bl Func_02003168
	ldr r3, [sp, #0]
	ldr r1, [sp, #0]
	adds r3, #104
	adds r1, #106
	mov r9, r1
	mov r10, r3
	add r1, sp, #4
	cmp r0, #7
	bne .L_0200a5ba
	movs r2, #0
	ldrsh r3, [r3, r2]
	mov r1, r9
	lsls r3, r3, #17
	str r3, [r6, #36]
	movs r5, #0
	movs r0, #0
	ldrsh r3, [r1, r0]
	lsls r3, r3, #17
	str r3, [r6, #44]
	movs r3, #128
	lsls r3, r3, #5
	str r3, [r6, #52]
.L_0200a578:
	mov r0, r10
	movs r3, #0
	ldrsh r2, [r0, r3]
	ldr r3, [r6, #8]
	lsls r2, r2, #19
	add r1, sp, #4
	adds r3, r3, r2
	str r3, [r1]
	mov r0, r9
	ldr r3, [r6, #12]
	str r3, [r1, #4]
	movs r3, #0
	ldrsh r2, [r0, r3]
	ldr r3, [r6, #16]
	lsls r2, r2, #19
	adds r3, r3, r2
	str r3, [r1, #8]
	adds r0, r6, #0
	bl Func_02003180
	cmp r0, #0
	beq .L_0200a5ac
	movs r3, #0
	str r3, [r6, #36]
	str r3, [r6, #44]
	b .L_0200a6b8
.L_0200a5ac:
	movs r0, #1
	adds r5, #1
	bl WaitFrames
	cmp r5, #9
	ble .L_0200a578
	b .L_0200a6b8
.L_0200a5ba:
	mov r0, r10
	movs r3, #0
	ldrsh r2, [r0, r3]
	ldr r3, [r6, #8]
	lsls r2, r2, #19
	adds r3, r3, r2
	str r3, [r1]
	mov r0, r9
	ldr r3, [r6, #12]
	str r3, [r1, #4]
	movs r3, #0
	ldrsh r2, [r0, r3]
	ldr r3, [r6, #16]
	lsls r2, r2, #19
	adds r3, r3, r2
	str r3, [r1, #8]
	adds r0, r6, #0
	bl Func_02003180
	cmp r0, #0
	bgt .L_0200a6b8
	cmp r0, #0
	bge .L_0200a604
	movs r3, #128
	lsls r3, r3, #7
	strh r3, [r6, #6]
	ldr r3, .L_0200a6d8
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	movs r2, #1
	ldr r0, [r3]
	movs r1, #6
	negs r2, r2
	bl Func_02003200
	b .L_0200a6b8
.L_0200a604:
	ldrh r3, [r6, #32]
	movs r2, #0
	subs r3, #2
	mov r11, r3
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #20]
	mov r8, r2
	adds r7, r5, #0
	adds r7, #89
.L_0200a618:
	ldr r3, [r5]
	cmp r3, #0
	beq .L_0200a648
	ldrb r2, [r7]
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	beq .L_0200a648
	cmp r5, r6
	beq .L_0200a648
	ldrh r3, [r5, #32]
	adds r0, r5, #0
	adds r0, #8
	subs r3, #2
	mov r1, r11
	add r2, sp, #4
	bl Func_020031b0
	cmp r0, #0
	blt .L_0200a648
	movs r0, #1
	bl WaitFrames
	b .L_0200a6b8
.L_0200a648:
	movs r3, #1
	add r8, r3
	mov r0, r8
	adds r7, #128
	adds r5, #128
	cmp r0, #63
	ble .L_0200a618
	mov r2, r10
	movs r1, #0
	ldrsh r3, [r2, r1]
	ldr r2, [r6, #8]
	lsls r3, r3, #17
	adds r1, r2, r3
	str r1, [r6, #8]
	mov r2, r9
	movs r0, #0
	ldrsh r3, [r2, r0]
	ldr r2, [r6, #16]
	ldr r7, .L_0200a6e4
	lsls r3, r3, #17
	ldr r0, .L_0200a6e8
	adds r5, r2, r3
	adds r3, r1, #0
	ands r3, r7
	movs r4, #128
	adds r2, r3, r0
	lsls r4, r4, #9
	str r5, [r6, #16]
	cmp r2, r4
	ble .L_0200a686
	adds r2, r4, #0
.L_0200a686:
	ldr r0, .L_0200a6ec
	cmp r2, r0
	bge .L_0200a68e
	adds r2, r0, #0
.L_0200a68e:
	subs r3, r1, r2
	ldr r1, .L_0200a6e8
	str r3, [r6, #8]
	adds r3, r5, #0
	ands r3, r7
	adds r2, r3, r1
	cmp r2, r4
	ble .L_0200a6a0
	adds r2, r4, #0
.L_0200a6a0:
	cmp r2, r0
	bge .L_0200a6a6
	adds r2, r0, #0
.L_0200a6a6:
	subs r3, r5, r2
	str r3, [r6, #16]
	ldr r2, [sp, #0]
	movs r3, #0
	strh r3, [r2, #4]
	movs r0, #1
	bl WaitFrames
	b .L_0200a53e
.L_0200a6b8:
	movs r3, #0
	str r3, [r6, #108]
	adds r0, r6, #0
	movs r1, #0
	bl Animation_ApplyChildValues
.L_0200a6c4:
	ldr r0, [sp, #0]
	movs r3, #0
	strh r3, [r0, #4]
	add sp, #16
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0200a6d8:
	.4byte gPartyState
.L_0200a6dc:
	.4byte Data_020023c4 + 0x188
.L_0200a6e0:
	.4byte Func_020024bc
.L_0200a6e4:
	.4byte 0x000fffff
.L_0200a6e8:
	.4byte 0xfff80000
.L_0200a6ec:
	.4byte 0xffff0000
	.section .text.x0200a6f0,"ax",%progbits
	.global Func_020026f0
	.thumb_func
Func_020026f0:
	push {lr}
	ldr r3, .L_0200a704
	movs r2, #4
	ldrsh r3, [r3, r2]
	cmp r3, #0
	beq .L_0200a700
	bl Func_020024e4
.L_0200a700:
	pop {pc}
	.2byte 0x0000
.L_0200a704:
	.4byte Data_020023c4 + 0x188
	.section .text.x0200a708,"ax",%progbits
	.global Func_02002708
	.thumb_func
Func_02002708:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #108]
	movs r1, #217
	lsls r1, r1, #1
	adds r6, r5, r1
	ldrh r3, [r6]
	sub sp, #12
	cmp r3, #0
	bne .L_0200a734
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r5, r2
	adds r1, #2
	ldr r0, [r3]
	adds r3, r5, r1
	ldr r1, [r3]
	bl Func_02003240
	movs r3, #1
	strh r3, [r6]
.L_0200a734:
	movs r0, #131
	lsls r0, r0, #1
	bl GameFlag_ClearBit
	movs r2, #179
	lsls r2, r2, #1
	movs r1, #173
	adds r3, r5, r2
	lsls r1, r1, #1
	movs r2, #0
	strh r2, [r3]
	adds r3, r5, r1
	adds r1, #4
	strh r2, [r3]
	adds r3, r5, r1
	subs r1, #2
	strh r2, [r3]
	adds r3, r5, r1
	adds r1, #8
	strh r2, [r3]
	movs r0, #10
	adds r3, r5, r1
	strh r2, [r3]
	movs r1, #0
	negs r0, r0
	bl Func_02002368
	movs r0, #224
	movs r1, #224
	lsls r1, r1, #8
	lsls r0, r0, #11
	bl Func_02003218
	ldr r3, .L_0200a838
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	adds r6, r0, #0
	movs r0, #131
	lsls r0, r0, #1
	ldr r7, .L_0200a83c
	bl GameFlag_SetBit
	bl Func_020031e0
	movs r0, #0
	bl Func_02003280
	movs r0, #131
	lsls r0, r0, #1
	bl GameFlag_ClearBit
	ldr r3, .L_0200a840
	movs r1, #49
	str r3, [r6, #108]
	movs r3, #128
	lsls r3, r3, #7
	strh r3, [r6, #6]
	adds r0, r6, #0
	bl Func_02003118
	ldr r3, [r7, #108]
	adds r3, #100
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	bne .L_0200a820
.L_0200a7c0:
	ldr r1, [r6, #8]
	ldr r2, [r6, #16]
	movs r0, #0
	bl Func_02003168
	ldr r1, [r7, #108]
	cmp r0, #7
	beq .L_0200a7e0
	ldr r2, [r1, #112]
	ldr r3, [r6, #8]
	adds r3, r3, r2
	str r3, [r6, #8]
	ldr r3, [r6, #16]
	ldr r2, [r1, #120]
	adds r3, r3, r2
	b .L_0200a808
.L_0200a7e0:
	ldr r3, [r1, #112]
	cmp r3, #0
	beq .L_0200a7f4
	ldr r3, [r6, #8]
	ldr r2, .L_0200a844
	ands r3, r2
	movs r2, #128
	lsls r2, r2, #12
	adds r3, r3, r2
	str r3, [r6, #8]
.L_0200a7f4:
	ldr r3, [r7, #108]
	ldr r3, [r3, #120]
	cmp r3, #0
	beq .L_0200a80a
	ldr r3, [r6, #16]
	ldr r2, .L_0200a844
	movs r1, #128
	ands r3, r2
	lsls r1, r1, #12
	adds r3, r3, r1
.L_0200a808:
	str r3, [r6, #16]
.L_0200a80a:
	movs r3, #0
	strh r3, [r7, #4]
	movs r0, #1
	bl WaitFrames
	ldr r3, [r7, #108]
	adds r3, #100
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	beq .L_0200a7c0
.L_0200a820:
	movs r5, #0
	movs r0, #30
	bl Battle_WaitMode0
	adds r0, r6, #0
	str r5, [r6, #108]
	movs r1, #0
	bl Animation_ApplyChildValues
	strh r5, [r7, #4]
	add sp, #12
	pop {r5, r6, r7, pc}
.L_0200a838:
	.4byte gPartyState
.L_0200a83c:
	.4byte Data_020023c4 + 0x188
.L_0200a840:
	.4byte Func_020024bc
.L_0200a844:
	.4byte 0xfff00000
	.section .text.x0200a848,"ax",%progbits
	.global Func_02002848
	.thumb_func
Func_02002848:
	push {lr}
	ldr r3, .L_0200a860
	movs r2, #4
	ldrsh r3, [r3, r2]
	cmp r3, #0
	beq .L_0200a85c
	bl Func_02002708
	movs r0, #1
	b .L_0200a85e
.L_0200a85c:
	movs r0, #0
.L_0200a85e:
	pop {pc}
.L_0200a860:
	.4byte Data_020023c4 + 0x188
	.section .text.x0200a864,"ax",%progbits
	.global Func_02002864
	.thumb_func
Func_02002864:
	ldr r3, .L_0200a86c
	movs r2, #4
	ldrsh r0, [r3, r2]
	bx lr
.L_0200a86c:
	.4byte Data_020023c4 + 0x188
	.section .text.x0200a870,"ax",%progbits
	.global Func_02002870
	.thumb_func
Func_02002870:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r1, .L_0200a9d8
	sub sp, #4
	ldr r3, [r1, #112]
	mov r11, r0
	cmp r3, #0
	bne .L_0200a88c
	b .L_0200a9e4
.L_0200a88c:
	movs r2, #0
	str r2, [sp, #0]
.L_0200a890:
	bl Random16Far
	adds r5, r0, #0
	bl Random16Far
	mov r3, r11
	ldr r3, [r3, #8]
	lsls r5, r5, #4
	mov r8, r3
	add r8, r5
	lsls r0, r0, #4
	mov r1, r8
	subs r1, r1, r0
	mov r8, r1
	bl Random16Far
	adds r6, r0, #0
	bl Random16Far
	adds r5, r0, #0
	bl Random16Far
	mov r2, r11
	ldr r3, [r2, #16]
	ldr r2, [r2, #12]
	lsls r5, r5, #4
	lsls r6, r6, #3
	movs r1, #128
	lsls r0, r0, #4
	adds r6, r6, r2
	lsls r1, r1, #11
	adds r3, r3, r5
	subs r3, r3, r0
	adds r6, r6, r1
	movs r0, #234
	adds r0, #255
	mov r1, r8
	adds r2, r6, #0
	bl Func_02003128
	adds r7, r0, #0
	cmp r7, #0
	beq .L_0200a9c4
	bl Random16Far
	mov r10, r0
	bl Random16Far
	adds r6, r0, #0
	bl Random16Far
	adds r5, r0, #0
	bl Random16Far
	movs r2, #128
	lsls r2, r2, #8
	adds r5, r5, r0
	ldr r1, .L_0200a9dc
	adds r0, r7, #0
	mov r9, r2
	bl Func_02003120
	adds r0, r7, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	mov r1, r10
	movs r2, #128
	lsls r2, r2, #10
	lsls r3, r1, #2
	adds r3, r3, r2
	str r3, [r7, #40]
	mov r0, r10
	bl Math_Cosine
	ldr r3, .L_0200a9e0
	lsls r6, r6, #3
	mov r8, r3
	adds r1, r0, #0
	adds r0, r6, #0
	mov lr, r8
	.2byte 0xf800
	str r0, [r7, #44]
	mov r0, r10
	bl Math_Sine
	adds r1, r0, #0
	adds r0, r6, #0
	mov lr, r8
	.2byte 0xf800
	ldr r2, .L_0200a9d8
	movs r3, #168
	lsls r3, r3, #7
	adds r3, #122
	str r3, [r7, #72]
	lsrs r5, r5, #2
	ldr r3, [r2, #112]
	add r5, r9
	movs r6, #0
	mov r1, r9
	str r5, [r7, #24]
	str r5, [r7, #28]
	str r1, [r7, #68]
	ldr r5, [r7, #80]
	str r0, [r7, #36]
	str r6, [r7, #52]
	ldr r3, [r3, #80]
	ldrb r0, [r5, #16]
	mov r8, r3
	bl Resource_ResetEntry
	ldrb r3, [r5, #17]
	ldr r1, .L_0200a9d8
	movs r2, #1
	orrs r3, r2
	strb r3, [r5, #17]
	ldrh r3, [r1, #12]
	ldr r0, [r5, #40]
	strb r3, [r5, #16]
	bl ResourceMetadata_ClearRecord
	str r6, [r5, #40]
	strb r6, [r5, #27]
	mov r2, r8
	ldrb r3, [r2, #20]
	ldrb r0, [r5, #5]
	strb r3, [r5, #20]
	ldrb r3, [r2, #21]
	strb r3, [r5, #21]
	ldrb r1, [r2, #5]
	movs r2, #63
	adds r3, r2, #0
	lsrs r1, r1, #6
	lsls r1, r1, #6
	ands r3, r0
	orrs r3, r1
	strb r3, [r5, #5]
	mov r1, r8
	ldrb r3, [r1, #7]
	ldrb r1, [r5, #7]
	lsrs r3, r3, #6
	lsls r3, r3, #6
	ands r2, r1
	orrs r2, r3
	strb r2, [r5, #7]
	mov r3, r8
	ldrh r2, [r3, #8]
	ldr r1, .L_0200a9d4
	ldrh r3, [r5, #8]
	lsls r2, r2, #22
	lsrs r2, r2, #22
	ands r3, r1
	orrs r3, r2
	strh r3, [r5, #8]
.L_0200a9c4:
	ldr r1, [sp, #0]
	subs r1, #1
	str r1, [sp, #0]
	cmp r1, #0
	blt .L_0200a9d0
	b .L_0200a890
.L_0200a9d0:
	b .L_0200a9e4
	.2byte 0x0000
.L_0200a9d4:
	.4byte 0xfffffc00
.L_0200a9d8:
	.4byte Data_020023c4 + 0x188
.L_0200a9dc:
	.4byte Data_02003304
.L_0200a9e0:
	.4byte IwramMulQ16
.L_0200a9e4:
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x0200a9f4,"ax",%progbits
	.global Func_020029f4
	.thumb_func
Func_020029f4:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #16
	str r0, [sp, #12]
	str r1, [sp, #8]
	str r2, [sp, #4]
	str r3, [sp, #0]
	movs r3, #1
	mov r11, r3
.L_0200aa10:
	movs r0, #70
	adds r0, #255
	ldr r1, [sp, #12]
	ldr r2, [sp, #8]
	ldr r3, [sp, #4]
	bl Func_02003128
	adds r7, r0, #0
	cmp r7, #0
	beq .L_0200aab2
	bl Random16Far
	adds r5, r0, #0
	bl Random16Far
	ldr r3, [sp, #0]
	lsrs r5, r5, #4
	adds r5, r3, r5
	lsrs r0, r0, #4
	movs r3, #128
	subs r5, r5, r0
	lsls r3, r3, #7
	mov r10, r3
	adds r3, r5, #0
	add r3, r10
	mov r9, r3
	bl Random16Far
	mov r8, r0
	mov r3, r8
	lsls r3, r3, #3
	mov r8, r3
	bl Random16Far
	ldr r1, .L_0200aacc
	adds r6, r0, #0
	adds r0, r7, #0
	bl Func_02003120
	movs r1, #0
	adds r0, r7, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r3, #160
	lsls r3, r3, #9
	adds r5, r5, r3
	str r5, [r7, #40]
	mov r0, r9
	bl Math_Cosine
	ldr r5, .L_0200aad0
	adds r1, r0, #0
	mov r0, r8
	mov lr, r5
	.2byte 0xf800
	str r0, [r7, #44]
	mov r0, r9
	bl Math_Sine
	adds r1, r0, #0
	mov r0, r8
	mov lr, r5
	.2byte 0xf800
	movs r3, #0
	str r3, [r7, #52]
	movs r3, #168
	lsls r3, r3, #7
	adds r3, #122
	lsrs r6, r6, #1
	str r3, [r7, #72]
	movs r3, #128
	add r6, r10
	lsls r3, r3, #8
	str r0, [r7, #36]
	str r6, [r7, #24]
	str r6, [r7, #28]
	str r3, [r7, #68]
	adds r0, r7, #0
	movs r1, #1
	bl Animation_ApplyChildValues
.L_0200aab2:
	movs r3, #1
	negs r3, r3
	add r11, r3
	mov r3, r11
	cmp r3, #0
	bge .L_0200aa10
	add sp, #16
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0200aacc:
	.4byte Data_02003348
.L_0200aad0:
	.4byte IwramMulQ16
	.section .text.x0200aad4,"ax",%progbits
	.global Func_02002ad4
	.thumb_func
Func_02002ad4:
	push {r5, lr}
	adds r5, r0, #0
	movs r0, #130
	lsls r0, r0, #1
	bl GameFlag_Test
	adds r5, #91
	strb r0, [r5]
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x0200aae8,"ax",%progbits
	.global Func_02002ae8
	.thumb_func
Func_02002ae8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	adds r5, r0, #0
	movs r3, #192
	movs r0, #100
	lsls r3, r3, #18
	adds r0, r0, r5
	ldr r6, [r3, #108]
	movs r1, #0
	ldrsh r3, [r0, r1]
	sub sp, #56
	mov r8, r0
	cmp r3, #0
	beq .L_0200ab10
	b .L_0200ad9c
.L_0200ab10:
	movs r0, #131
	lsls r0, r0, #1
	bl GameFlag_Test
	movs r2, #179
	lsls r2, r2, #1
	adds r3, r6, r2
	movs r4, #0
	ldrsh r3, [r3, r4]
	cmp r3, #0
	beq .L_0200ab2a
	movs r3, #1
	orrs r0, r3
.L_0200ab2a:
	adds r3, r5, #0
	adds r3, #91
	strb r0, [r3]
	add r7, sp, #44
	ldr r3, [r5, #8]
	movs r0, #128
	str r3, [r7]
	lsls r0, r0, #12
	ldr r3, [r5, #12]
	adds r2, r7, #0
	str r3, [r7, #4]
	ldr r3, [r5, #16]
	str r3, [r7, #8]
	ldrh r1, [r5, #6]
	bl Vector_AddPolarOffsetFar
	ldr r1, [r7]
	ldr r2, [r7, #8]
	movs r0, #0
	bl Func_02003168
	cmp r0, #7
	bne .L_0200aba0
	movs r3, #128
	lsls r3, r3, #24
	str r3, [r5, #64]
	str r3, [r5, #60]
	str r3, [r5, #56]
	movs r3, #128
	lsls r3, r3, #7
	str r3, [r5, #52]
	mov r0, r8
	movs r3, #1
	strh r3, [r0]
	movs r0, #145
	bl Func_020032b8
	movs r0, #160
	lsls r0, r0, #11
	movs r2, #128
	adds r1, r0, #0
	lsls r2, r2, #9
	bl Func_02003198
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	negs r0, r0
	negs r1, r1
	adds r2, #102
	bl Func_02003198
	ldr r3, [r5, #104]
	cmp r3, #0
	beq .L_0200aba0
	adds r0, r5, #0
	mov lr, r3
	.2byte 0xf800
.L_0200aba0:
	movs r3, #192
	lsls r3, r3, #18
	ldr r0, [r3, #32]
	ldr r3, [r5, #8]
	ldr r1, [r5, #16]
	asrs r3, r3, #20
	str r3, [sp, #32]
	movs r3, #184
	lsls r3, r3, #1
	ldr r4, [sp, #32]
	asrs r1, r1, #20
	adds r2, r0, r3
	ldr r2, [r2]
	lsls r3, r1, #7
	adds r3, r4, r3
	lsls r3, r3, #2
	adds r2, r2, r3
	str r2, [sp, #28]
	movs r4, #212
	lsls r4, r4, #1
	adds r2, r0, r4
	ldr r2, [r2]
	subs r4, #92
	adds r2, r2, r3
	str r2, [sp, #24]
	movs r2, #164
	lsls r2, r2, #1
	adds r3, r0, r2
	ldr r3, [r3]
	asrs r3, r3, #20
	str r3, [sp, #20]
	adds r3, r0, r4
	adds r4, #52
	ldr r2, [r3]
	adds r3, r0, r4
	ldr r3, [r3]
	adds r4, #4
	asrs r3, r3, #20
	str r3, [sp, #16]
	adds r3, r0, r4
	ldr r3, [r3]
	movs r0, #1
	asrs r3, r3, #20
	adds r3, r1, r3
	subs r3, #2
	asrs r2, r2, #20
	negs r0, r0
	str r3, [sp, #8]
	str r0, [sp, #40]
	subs r3, r1, #1
	adds r1, r1, r2
	subs r1, #1
	mov r8, r3
	mov r11, r1
.L_0200ac0c:
	ldr r0, [sp, #32]
	ldr r1, [sp, #16]
	ldr r2, [sp, #20]
	movs r4, #1
	adds r3, r0, r1
	subs r3, #1
	negs r4, r4
	mov r9, r3
	str r4, [sp, #36]
	adds r3, r0, r2
	adds r6, r0, #0
	subs r3, #1
	subs r6, #1
	mov r10, r3
.L_0200ac28:
	ldr r4, [sp, #40]
	ldr r0, [sp, #36]
	lsls r3, r4, #7
	adds r3, r3, r0
	lsls r3, r3, #2
	ldr r1, [sp, #28]
	str r3, [sp, #12]
	adds r2, r3, r1
	ldrb r3, [r2, #2]
	cmp r3, #77
	bne .L_0200ac88
	movs r3, #0
	strb r3, [r2, #2]
	mov r2, r10
	mov r3, r11
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r0, #64
	movs r1, #64
	movs r2, #1
	movs r3, #1
	bl Func_02003178
	mov r4, r8
	movs r1, #64
	movs r2, #1
	movs r3, #1
	movs r0, #64
	str r4, [sp, #4]
	str r6, [sp, #0]
	bl Func_02003170
	movs r0, #159
	lsls r0, r0, #1
	adds r0, #255
	bl Func_020032b8
	mov r3, r8
	movs r4, #128
	lsls r4, r4, #12
	lsls r2, r3, #20
	lsls r0, r6, #20
	adds r0, r0, r4
	ldr r1, [r5, #12]
	ldrh r3, [r5, #6]
	adds r2, r2, r4
	bl Func_020029f4
.L_0200ac88:
	ldr r4, [sp, #12]
	ldr r0, [sp, #24]
	adds r2, r4, r0
	ldrb r3, [r2, #2]
	cmp r3, #77
	bne .L_0200acdc
	movs r3, #0
	strb r3, [r2, #2]
	ldr r2, [sp, #8]
	mov r1, r9
	str r1, [sp, #0]
	str r2, [sp, #4]
	movs r0, #64
	movs r1, #64
	movs r2, #1
	movs r3, #2
	bl Func_02003178
	mov r3, r8
	str r3, [sp, #4]
	movs r1, #64
	movs r2, #1
	movs r3, #1
	movs r0, #64
	str r6, [sp, #0]
	bl Func_02003170
	movs r0, #143
	lsls r0, r0, #2
	bl Func_020032b8
	mov r3, r8
	movs r4, #128
	lsls r4, r4, #12
	lsls r2, r3, #20
	lsls r0, r6, #20
	adds r0, r0, r4
	ldr r1, [r5, #12]
	ldrh r3, [r5, #6]
	adds r2, r2, r4
	bl Func_020029f4
.L_0200acdc:
	ldr r0, [sp, #36]
	movs r4, #1
	adds r0, #1
	add r9, r4
	adds r6, #1
	add r10, r4
	str r0, [sp, #36]
	cmp r0, #1
	ble .L_0200ac28
	ldr r1, [sp, #8]
	ldr r2, [sp, #40]
	adds r1, #1
	adds r2, #1
	str r1, [sp, #8]
	add r8, r4
	add r11, r4
	str r2, [sp, #40]
	cmp r2, #1
	ble .L_0200ac0c
	ldr r3, [r5, #24]
	movs r4, #128
	lsls r4, r4, #9
	cmp r3, r4
	bge .L_0200ad1a
	movs r2, #128
	lsls r2, r2, #5
	adds r3, r3, r2
	str r3, [r5, #24]
	ldr r3, [r5, #28]
	adds r3, r3, r2
	str r3, [r5, #28]
.L_0200ad1a:
	ldr r1, [r7]
	ldr r2, [r7, #4]
	ldr r3, [r7, #8]
	adds r0, r5, #0
	bl Func_02003140
	movs r3, #192
	lsls r3, r3, #18
	ldr r7, [r3, #108]
	ldr r3, .L_0200ade0
	movs r0, #133
	lsls r0, r0, #2
	adds r3, r3, r0
	ldr r0, [r3]
	ldr r6, .L_0200ade4
	bl Object_GetById
	ldr r1, [r5, #8]
	ldr r3, [r0, #8]
	subs r2, r1, r3
	cmp r2, #0
	blt .L_0200ad50
	movs r1, #160
	lsls r1, r1, #13
	cmp r2, r1
	blt .L_0200ad5a
	b .L_0200add0
.L_0200ad50:
	movs r2, #160
	subs r3, r3, r1
	lsls r2, r2, #13
	cmp r3, r2
	bge .L_0200add0
.L_0200ad5a:
	ldr r3, [r5, #12]
	ldr r2, [r0, #12]
	ldr r4, .L_0200ade8
	ldr r1, .L_0200adec
	subs r3, r3, r2
	adds r3, r3, r4
	cmp r3, r1
	bhi .L_0200add0
	ldr r3, [r5, #16]
	ldr r0, [r0, #16]
	subs r2, r3, r0
	cmp r2, #0
	blt .L_0200ad7e
	movs r3, #160
	lsls r3, r3, #13
	cmp r2, r3
	blt .L_0200ad88
	b .L_0200add0
.L_0200ad7e:
	movs r4, #160
	subs r3, r0, r3
	lsls r4, r4, #13
	cmp r3, r4
	bge .L_0200add0
.L_0200ad88:
	movs r3, #2
	strh r3, [r6, #4]
	ldrh r3, [r6, #10]
	movs r0, #170
	lsls r0, r0, #1
	adds r3, #2
	adds r2, r7, r0
	str r5, [r6, #108]
	strh r3, [r2]
	b .L_0200add0
.L_0200ad9c:
	cmp r3, #1
	bne .L_0200add0
	adds r3, r5, #0
	adds r3, #91
	movs r2, #0
	strb r2, [r3]
	ldr r3, [r5, #24]
	cmp r3, #0
	ble .L_0200adc2
	ldr r2, .L_0200adf0
	adds r0, r5, #0
	adds r3, r3, r2
	str r3, [r5, #24]
	ldr r3, [r5, #28]
	adds r3, r3, r2
	str r3, [r5, #28]
	bl Func_02002870
	b .L_0200add0
.L_0200adc2:
	str r2, [r5, #16]
	str r2, [r5, #12]
	str r2, [r5, #8]
	str r2, [r5, #44]
	str r2, [r5, #40]
	str r2, [r5, #36]
	str r2, [r5, #108]
.L_0200add0:
	add sp, #56
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200ade0:
	.4byte gPartyState
.L_0200ade4:
	.4byte Data_020023c4 + 0x188
.L_0200ade8:
	.4byte 0x0007ffff
.L_0200adec:
	.4byte 0x001ffffe
.L_0200adf0:
	.4byte 0xfffff000
	.section .text.x0200adf4,"ax",%progbits
	.global Func_02002df4
	.thumb_func
Func_02002df4:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r9
	push {r5, r6}
	mov r6, r8
	push {r6}
	ldr r6, [sp, #24]
	adds r5, r1, #0
	mov r9, r2
	mov r10, r3
	bl Object_GetById
	mov r8, r0
	adds r0, r5, #0
	bl Object_GetById
	adds r5, r0, #0
	mov r0, r8
	ldr r2, [r0, #12]
	ldr r3, [r0, #16]
	ldr r1, [r0, #8]
	adds r0, r5, #0
	bl Object_SetPositionAndResetMotion
	adds r0, r5, #0
	adds r1, r6, #0
	bl Func_02003120
	adds r0, r5, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	adds r3, r5, #0
	adds r3, #85
	movs r6, #0
	strb r6, [r3]
	adds r0, r5, #0
	movs r1, #1
	bl Func_02003118
	adds r3, r5, #0
	mov r2, r8
	adds r3, #100
	str r2, [r5, #104]
	mov r0, r10
	strh r6, [r3]
	adds r3, #2
	strh r0, [r3]
	mov r2, r9
	subs r3, #4
	strb r2, [r3]
	ldr r3, .L_0200ae68
	str r3, [r5, #108]
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, pc}
.L_0200ae68:
	.4byte Func_02002ad4
	.section .text.x0200ae6c,"ax",%progbits
	.global Func_02002e6c
	.thumb_func
Func_02002e6c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	mov r11, r0
	mov r3, r11
	adds r3, #98
	ldrb r0, [r3]
	sub sp, #12
	bl Object_GetById
	mov r3, r11
	adds r7, r0, #0
	adds r3, #102
	movs r2, #0
	ldrsh r6, [r3, r2]
	ldr r3, [r7, #80]
	mov r2, r11
	mov r9, r3
	ldr r3, [r2, #8]
	mov r5, sp
	str r3, [r5]
	movs r0, #128
	ldr r3, [r2, #12]
	ldr r2, .L_0200aefc
	adds r1, r6, #0
	adds r3, r3, r2
	str r3, [r5, #4]
	mov r2, r11
	ldr r3, [r2, #16]
	lsls r0, r0, #14
	adds r2, r5, #0
	str r3, [r5, #8]
	bl Vector_AddPolarOffsetFar
	ldr r3, .L_0200aef8
	movs r2, #0
	mov r10, r3
	adds r3, r7, #0
	mov r8, r2
	adds r3, #85
	mov r2, r10
	strh r6, [r7, #6]
	strb r2, [r3]
	adds r0, r7, #0
	ldr r1, [r5]
	ldr r2, [r5, #4]
	ldr r3, [r5, #8]
	bl Object_SetPositionAndResetMotion
	ldr r3, .L_0200af00
	mov r2, r10
	str r3, [r7, #108]
	adds r3, r7, #0
	adds r3, #90
	strb r2, [r3]
	movs r3, #128
	lsls r3, r3, #12
	str r3, [r7, #48]
	movs r3, #128
	lsls r3, r3, #6
	str r3, [r7, #52]
	ldr r3, .L_0200af04
	mov r2, r9
	adds r6, r6, r3
	b .L_0200af08
	.2byte 0x0000
.L_0200aef8:
	.4byte 0x00000000
.L_0200aefc:
	.4byte 0xfff40000
.L_0200af00:
	.4byte Func_02002ae8
.L_0200af04:
	.4byte 0xffffc000
.L_0200af08:
	mov r3, r8
	strh r6, [r2, #18]
	str r3, [r7, #24]
	str r3, [r7, #28]
	adds r3, r7, #0
	mov r2, r8
	adds r3, #100
	strh r2, [r3]
	mov r2, r11
	ldr r3, [r2, #104]
	movs r0, #104
	str r3, [r7, #104]
	bl Func_020032b8
	add sp, #12
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x0200af34,"ax",%progbits
	.global Func_02002f34
	.thumb_func
Func_02002f34:
	push {r5, r6, r7, lr}
	adds r5, r0, #0
	adds r7, r5, #0
	adds r7, #100
	movs r3, #0
	ldrsh r6, [r7, r3]
	cmp r6, #0
	bne .L_0200af6c
	ldr r1, [r5, #8]
	ldr r2, [r5, #16]
	movs r0, #0
	bl Map_GetTerrainHeight
	movs r3, #128
	lsls r3, r3, #13
	adds r0, r0, r3
	movs r3, #128
	lsls r3, r3, #9
	str r0, [r5, #12]
	movs r1, #1
	adds r0, r5, #0
	str r6, [r5, #24]
	str r6, [r5, #28]
	str r3, [r5, #48]
	str r3, [r5, #52]
	bl Func_020031a8
	b .L_0200afd4
.L_0200af6c:
	cmp r6, #30
	bgt .L_0200af84
	cmp r6, #30
	bne .L_0200afd4
	movs r0, #136
	bl Func_020032b8
	ldr r0, [r5, #104]
	movs r1, #2
	bl Func_02003118
	b .L_0200afd4
.L_0200af84:
	cmp r6, #60
	bgt .L_0200afac
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r5, #24]
	str r3, [r5, #28]
	ldrh r3, [r7]
	ldr r0, [r5, #104]
	lsls r3, r3, #16
	asrs r2, r3, #16
	lsrs r3, r3, #31
	adds r2, r2, r3
	ldr r3, .L_0200afd0
	asrs r2, r2, #1
	ands r2, r3
	lsls r1, r2, #3
	subs r1, r1, r2
	bl Animation_ApplyChildValues
	b .L_0200afd4
.L_0200afac:
	movs r1, #1
	ldr r0, [r5, #104]
	bl Func_02003118
	movs r0, #184
	bl Func_020032b8
	ldr r1, [r5, #8]
	ldr r2, [r5, #16]
	movs r0, #0
	bl Map_GetTerrainHeight
	movs r3, #0
	str r0, [r5, #12]
	str r3, [r5, #104]
	movs r0, #0
	b .L_0200afdc
	.2byte 0x0000
.L_0200afd0:
	.4byte 0x00000001
.L_0200afd4:
	ldrh r3, [r7]
	movs r0, #1
	adds r3, #1
	strh r3, [r7]
.L_0200afdc:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x0200afe0,"ax",%progbits
	.global Func_02002fe0
	.thumb_func
Func_02002fe0:
	push {r5, r6, r7, lr}
	adds r5, r0, #0
	adds r7, r5, #0
	adds r7, #100
	movs r3, #0
	ldrsh r6, [r7, r3]
	cmp r6, #0
	bne .L_0200b02a
	movs r0, #136
	bl Func_020032b8
	ldr r0, [r5, #104]
	movs r1, #2
	bl Func_02003118
	ldr r1, [r5, #8]
	ldr r2, [r5, #16]
	movs r0, #0
	bl Map_GetTerrainHeight
	movs r3, #128
	lsls r3, r3, #13
	adds r0, r0, r3
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r5, #48]
	movs r3, #128
	str r0, [r5, #12]
	lsls r3, r3, #8
	adds r0, r5, #0
	movs r1, #1
	str r6, [r5, #24]
	str r6, [r5, #28]
	str r3, [r5, #52]
	bl Func_020031a8
	b .L_0200b078
.L_0200b02a:
	cmp r6, #32
	bgt .L_0200b052
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r5, #24]
	str r3, [r5, #28]
	ldrh r3, [r7]
	ldr r0, [r5, #104]
	lsls r3, r3, #16
	asrs r2, r3, #16
	lsrs r3, r3, #31
	adds r2, r2, r3
	ldr r3, .L_0200b074
	asrs r2, r2, #1
	ands r2, r3
	lsls r1, r2, #3
	subs r1, r1, r2
	bl Animation_ApplyChildValues
	b .L_0200b078
.L_0200b052:
	movs r1, #1
	ldr r0, [r5, #104]
	bl Func_02003118
	movs r0, #184
	bl Func_020032b8
	ldr r1, [r5, #8]
	ldr r2, [r5, #16]
	movs r0, #0
	bl Map_GetTerrainHeight
	movs r3, #0
	str r0, [r5, #12]
	str r3, [r5, #104]
	movs r0, #0
	b .L_0200b080
.L_0200b074:
	.4byte 0x00000001
.L_0200b078:
	ldrh r3, [r7]
	movs r0, #1
	adds r3, #1
	strh r3, [r7]
.L_0200b080:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x0200b084,"ax",%progbits
	.global Func_02003084
	.thumb_func
Func_02003084:
	push {r5, lr}
	adds r5, r0, #0
	bl Func_02002e6c
	movs r3, #0
	str r3, [r5, #8]
	str r3, [r5, #12]
	str r3, [r5, #16]
	str r3, [r5, #36]
	str r3, [r5, #40]
	str r3, [r5, #44]
	movs r0, #0
	pop {r5, pc}
	.2byte 0x0000
	.section .rodata.x0200b2c8,"a",%progbits
.L_0200b2c8:
	.4byte 0x0000002e
	.4byte Func_0200004c
	.4byte 0x00000011
	.global Data_020032d4
Data_020032d4:
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x00000016
	.4byte 0x00000026
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.global Data_02003304
Data_02003304:
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x80010000
	.4byte 0x00000016
	.4byte 0x00000019
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000019
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000000a
	.4byte 0xc0010000
	.4byte 0x00000026
	.global Data_02003348
Data_02003348:
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x80010000
	.4byte 0x00000016
	.4byte 0x00000019
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000019
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000000a
	.4byte 0xc0010000
	.4byte 0x00000026
	.global Data_0200338c
Data_0200338c:
	.4byte 0x00ca00c9
	.4byte 0x00cc00cb
	.4byte 0x00ce00cd
	.4byte 0xffff00cf
	.global Data_0200339c
Data_0200339c:
	.4byte 0x00190017
	.4byte 0x00170016
	.4byte 0x00160019
	.4byte 0x00190017
	.4byte 0x00170016
	.4byte 0x00160019
	.4byte 0x00190017
	.4byte 0x00170016
	.4byte 0x00160019
	.4byte 0x00190017
	.4byte 0xffff0016
	.global Data_020033c8
Data_020033c8:
	.4byte 0x001b001e
	.4byte 0x0016001f
	.4byte 0x001f0015
	.4byte 0x00120014
	.4byte 0x000d001f
	.4byte 0x001b000b
	.4byte 0x0008000b
	.4byte 0x00080018
	.4byte 0x00150005
	.4byte 0x001f001f
	.4byte 0xffff001f
	.global Data_020033f4
Data_020033f4:
	.4byte 0x00c900ce
	.4byte 0x00cb00ca
	.4byte 0x00cd00cc
	.4byte 0x00ce00cd
	.4byte 0x00ca00c9
	.4byte 0x00cc00cb
	.4byte 0x00cd00cc
	.4byte 0x00c900ce
	.4byte 0x00cb00ca
	.4byte 0x00cc00cb
	.4byte 0x00ce00cd
	.4byte 0x00ca00c9
	.4byte 0x00cb00ca
	.4byte 0x00cd00cc
	.4byte 0x00c900ce
	.4byte 0x00ca00c9
	.4byte 0x00cc00cb
	.4byte 0x00ce00cd
	.global gSceneEntrances
gSceneEntrances:
	.4byte 0xffff0000
	.4byte 0x000000c0
	.4byte 0x800000c0
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
	.4byte 0x00000128
	.4byte 0x00105127
	.4byte 0x00208125
	.4byte 0x00304124
	.4byte 0x0040f126
	.4byte 0x0000011f
	.4byte 0x00101129
	.4byte 0x00202118
	.4byte 0x00301123
	.4byte 0x00401120
	.4byte 0x00501122
	.4byte 0x00601121
	.4byte 0x0070811f
	.4byte 0x0080711f
	.4byte 0x0090a11f
	.4byte 0x00a0911f
	.4byte 0x00b0c11f
	.4byte 0x00c0b11f
	.4byte 0x00d0e11f
	.4byte 0x00e0d11f
	.4byte 0x00000120
	.4byte 0x0010411f
	.4byte 0x00201125
	.4byte 0x00000121
	.4byte 0x0010611f
	.4byte 0x00201126
	.4byte 0x00000122
	.4byte 0x0010511f
	.4byte 0x00201124
	.4byte 0x00000123
	.4byte 0x0010311f
	.4byte 0x00201127
	.4byte 0x000001ff
	.global Data_020034f0
Data_020034f0:
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02003508
Data_02003508:
	.4byte 0xffff01a2
	.4byte .L_0200b2c8
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00020000
	.4byte 0xffff016d
	.4byte 0x00000007
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00024000
	.4byte 0xffff01e9
	.4byte .L_0200b2c8
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x00700000
	.4byte 0x00020000
	.4byte 0xffff019d
	.4byte 0x00000001
	.4byte 0x01d80000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00020000
	.4byte 0xffff019d
	.4byte 0x00000001
	.4byte 0x02180000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00028000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02003598
Data_02003598:
	.4byte 0xffff01a0
	.4byte 0x00000001
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00020000
	.4byte 0xffff01a0
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x01028000
	.4byte 0xffff01a0
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x01020000
	.4byte 0xffff01a0
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x01028000
	.4byte 0xffff016d
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00024000
	.4byte 0xffff016d
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00024000
	.4byte 0xffff016d
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00024000
	.4byte 0xffff016d
	.4byte 0x00000001
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00024000
	.4byte 0xffff01e9
	.4byte 0x00000007
	.4byte 0x00e00000
	.4byte 0x00000000
	.4byte 0x00900000
	.4byte 0x00024000
	.4byte 0xffff02a1
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff019c
	.4byte 0x00000007
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
	.global Data_020036b8
Data_020036b8:
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020036c4
Data_020036c4:
	.4byte 0xffffffff
	.global Data_020036c8
Data_020036c8:
	.4byte 0x00000002
	.4byte 0xffff0001
	.4byte Func_020002c8
	.4byte 0x10009a15
	.4byte 0xffff0008
	.4byte Func_020002e8
	.4byte 0x60009a15
	.4byte 0xffff000a
	.4byte Func_020002fc
	.4byte 0x20009a15
	.4byte 0xffff000a
	.4byte Func_0200030c
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02003704
Data_02003704:
	.4byte 0xffffffff
	.global Data_02003708
Data_02003708:
	.4byte 0x00000031
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
	.4byte 0x00000002
	.4byte 0x1a300014
	.4byte Func_020006c8
	.4byte 0x00000002
	.4byte 0x1a300015
	.4byte Func_020006c8
	.4byte 0x00000002
	.4byte 0x1a300016
	.4byte Func_020006c8
	.4byte 0x00000002
	.4byte 0x1a300017
	.4byte Func_020006c8
	.4byte 0x00000002
	.4byte 0x1a300018
	.4byte Func_020006c8
	.4byte 0x00000002
	.4byte 0x1a300019
	.4byte Func_020006c8
	.4byte 0x00000002
	.4byte 0x1a30001a
	.4byte Func_020006c8
	.4byte 0x00000002
	.4byte 0x1a30001b
	.4byte Func_020006c8
	.4byte 0x00009c05
	.4byte 0xffff0001
	.4byte Func_02000abc
	.4byte 0x00000003
	.4byte 0xffff001e
	.4byte Func_020010e4
	.4byte 0x00000003
	.4byte 0xffff001f
	.4byte Func_02001154
	.4byte 0x00000003
	.4byte 0xffff0020
	.4byte Func_020011c4
	.4byte 0x00000003
	.4byte 0xffff0021
	.4byte Func_02001234
	.4byte 0x00000002
	.4byte 0x0a2e0022
	.4byte Func_020013a0
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02003858
Data_02003858:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_0200387c
Data_0200387c:
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00003000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0x00003000
	.4byte 0x00000017
	.4byte 0x00000006
	.4byte 0xffffe800
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000008
	.4byte 0x00000000
	.4byte 0x80010000
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xffffd000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xffffd000
	.4byte 0x00000017
	.4byte 0x00000006
	.4byte 0x00001800
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000000c
	.4byte 0xc0010000
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.global Data_02003900
Data_02003900:
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.section .bss,"aw",%nobits
	.global Data_02003918
Data_02003918:
	.space 0x00000004
	.global Data_0200391c
Data_0200391c:
	.space 0x00000004
	.global Data_02003920
Data_02003920:
	.space 0x00000004
	.global Data_02003924
Data_02003924:
	.space 0x00000004
	.global Data_02003928
Data_02003928:
	.space 0x00000004
	.global Data_0200392c
Data_0200392c:
	.space 0x00000004
	.global Data_02003930
Data_02003930:
	.space 0x00000004
	.global Data_02003934
Data_02003934:
	.space 0x00000004
	.global Data_02003938
Data_02003938:
	.space 0x00000004
	.global Data_0200393c
Data_0200393c:
	.space 0x00000040
	.global Data_0200397c
Data_0200397c:
	.space 0x00000040
	.global Data_020039bc
Data_020039bc:
	.space 0x00000020
	.global Data_020039dc
Data_020039dc:
	.space 0x00000040
	.global Data_02003a1c
Data_02003a1c:
	.space 0x00000040
	.global Data_02003a5c
Data_02003a5c:
	.space 0x00000014
	.global Data_0200ba70
Data_0200ba70:
	.space 0x0000000c
	.global Data_02003a7c
Data_02003a7c:
	.space 0x00000002
	.global Data_02003a7e
Data_02003a7e:
	.space 0x00000002
	.global Data_02003a80
Data_02003a80:
	.space 0x00000002
	.global Data_02003a82
Data_02003a82:
