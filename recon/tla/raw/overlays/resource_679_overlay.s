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
	bl Func_02001c7c
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
	bl Func_02001c7c
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
	bl Func_02001c7c
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
	bl Func_02001c6c
	ldr r2, .L_02008330
	mov r3, r10
	ands r3, r5
	lsls r3, r3, #2
	ldr r1, [r2, r3]
	adds r0, r6, #0
	mov r11, r3
	bl Func_02001c74
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
	bl Func_02001c6c
	ldr r1, [r7, #28]
	adds r0, r6, #0
	bl Func_02001c74
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
	.4byte Data_02002038
.L_02008334:
	.4byte Func_02000124
.L_02008338:
	.4byte 0xffff0000
	.section .text.x0200833c,"ax",%progbits
	.global Func_0200033c
	.thumb_func
Func_0200033c:
	push {lr}
	movs r0, #10
	movs r1, #3
	movs r2, #11
	bl Func_02001dd4
	pop {pc}
	.2byte 0x0000
	.section .text.x02008360,"ax",%progbits
	.global Func_02000360
	.thumb_func
Func_02000360:
	push {lr}
	ldr r3, .L_02008390
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008394
	cmp r2, r3
	bne .L_02008378
	ldr r0, .L_02008398
	b .L_0200838e
.L_02008378:
	ldr r3, .L_0200839c
	cmp r2, r3
	bne .L_02008382
	ldr r0, .L_020083a0
	b .L_0200838e
.L_02008382:
	ldr r3, .L_020083a4
	cmp r2, r3
	bne .L_0200838c
	ldr r0, .L_020083a8
	b .L_0200838e
.L_0200838c:
	ldr r0, .L_020083ac
.L_0200838e:
	pop {pc}
.L_02008390:
	.4byte gPartyState
.L_02008394:
	.4byte 0x0000009a
.L_02008398:
	.4byte Data_02002218
.L_0200839c:
	.4byte 0x0000009c
.L_020083a0:
	.4byte Data_02002320
.L_020083a4:
	.4byte 0x0000009b
.L_020083a8:
	.4byte Data_02002458
.L_020083ac:
	.4byte Data_02002200
	.section .text.x020083b0,"ax",%progbits
	.global Func_020003b0
	.thumb_func
Func_020003b0:
	push {lr}
	ldr r3, .L_020083c8
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	movs r3, #2
	adds r0, #34
	strb r3, [r0]
	pop {pc}
.L_020083c8:
	.4byte gPartyState
	.section .text.x020083cc,"ax",%progbits
	.global Func_020003cc
	.thumb_func
Func_020003cc:
	push {lr}
	ldr r3, .L_020083e4
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	movs r3, #0
	adds r0, #34
	strb r3, [r0]
	pop {pc}
.L_020083e4:
	.4byte gPartyState
	.section .text.x020083e8,"ax",%progbits
	.global Func_020003e8
	.thumb_func
Func_020003e8:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r1, #179
	lsls r1, r1, #1
	adds r3, r2, r1
	movs r5, #0
	ldrsh r3, [r3, r5]
	cmp r3, #0
	beq .L_02008404
	b .L_0200851a
.L_02008404:
	movs r1, #192
	lsls r1, r1, #4
	adds r1, #164
	adds r3, r2, r1
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	mov r8, r3
	cmp r3, #0
	beq .L_0200841a
	b .L_0200851a
.L_0200841a:
	ldr r7, .L_02008520
	movs r2, #1
	ldr r3, [r7]
	negs r2, r2
	cmp r3, r2
	bne .L_02008460
	ldr r6, .L_02008524
	movs r1, #160
	lsls r1, r1, #19
	ldr r5, .L_02008528
	adds r0, r6, #0
	adds r1, #96
	movs r2, #32
	mov lr, r5
	.2byte 0xf800
	adds r1, r6, #0
	movs r2, #32
	ldr r0, .L_0200852c
	mov lr, r5
	.2byte 0xf800
	ldr r6, .L_02008530
	movs r1, #160
	lsls r1, r1, #19
	adds r1, #128
	movs r2, #32
	adds r0, r6, #0
	mov lr, r5
	.2byte 0xf800
	ldr r0, .L_02008534
	adds r1, r6, #0
	movs r2, #32
	mov lr, r5
	.2byte 0xf800
	mov r3, r8
	str r3, [r7]
.L_02008460:
	ldr r3, [r7]
	cmp r3, #0
	bge .L_02008468
	adds r3, #7
.L_02008468:
	asrs r2, r3, #3
	adds r3, r2, #0
	cmp r2, #0
	bge .L_02008472
	adds r3, r2, #3
.L_02008472:
	asrs r1, r3, #2
	lsls r3, r1, #2
	subs r1, r2, r3
	ldr r5, .L_02008534
	ldr r2, .L_02008530
	ldr r7, .L_0200852c
	ldr r6, .L_02008524
	movs r4, #0
	mov r12, r5
	mov r8, r2
	movs r0, #24
.L_02008488:
	adds r2, r1, r4
	adds r3, r2, #0
	cmp r2, #0
	bge .L_02008492
	adds r3, r2, #3
.L_02008492:
	asrs r3, r3, #2
	lsls r3, r3, #2
	subs r3, r2, r3
	ldrh r2, [r6, r0]
	lsls r3, r3, #1
	adds r3, #24
	strh r2, [r7, r3]
	mov r5, r8
	ldrh r5, [r5, r0]
	mov r2, r12
	adds r4, #1
	strh r5, [r2, r3]
	adds r0, #2
	cmp r4, #3
	ble .L_02008488
	ldr r0, .L_02008538
	ldr r1, .L_0200853c
	ldrh r3, [r1]
	adds r4, r3, #0
	strh r1, [r1]
	ldrh r2, [r0]
	cmp r2, #31
	bgt .L_020084e0
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	adds r3, r3, r0
	adds r3, #4
	adds r2, #1
	stmia r3!, {r7}
	strh r2, [r0]
	movs r2, #160
	lsls r2, r2, #19
	adds r2, #96
	stmia r3!, {r2}
	movs r2, #132
	lsls r2, r2, #24
	adds r2, #8
	str r2, [r3]
.L_020084e0:
	strh r4, [r1]
	ldrh r3, [r1]
	adds r4, r3, #0
	strh r1, [r1]
	ldrh r2, [r0]
	cmp r2, #31
	bgt .L_02008510
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	adds r2, #1
	adds r3, r3, r0
	adds r3, #4
	strh r2, [r0]
	mov r2, r12
	stmia r3!, {r2}
	movs r2, #160
	lsls r2, r2, #19
	adds r2, #128
	stmia r3!, {r2}
	movs r2, #132
	lsls r2, r2, #24
	adds r2, #8
	str r2, [r3]
.L_02008510:
	strh r4, [r1]
	ldr r2, .L_02008520
	ldr r3, [r2]
	adds r3, #1
	str r3, [r2]
.L_0200851a:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_02008520:
	.4byte Data_020024c4
.L_02008524:
	.4byte Data_02002958
.L_02008528:
	.4byte IwramCopyWords
.L_0200852c:
	.4byte Data_02002978
.L_02008530:
	.4byte Data_02002998
.L_02008534:
	.4byte Data_020029b8
.L_02008538:
	.4byte gIoWriteQueue
.L_0200853c:
	.4byte 0x04000208
	.section .text.x02008540,"ax",%progbits
	.global Func_02000540
	.thumb_func
Func_02000540:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r1, #179
	lsls r1, r1, #1
	adds r3, r2, r1
	movs r5, #0
	ldrsh r3, [r3, r5]
	cmp r3, #0
	beq .L_0200855c
	b .L_0200866a
.L_0200855c:
	movs r1, #192
	lsls r1, r1, #4
	adds r1, #164
	adds r3, r2, r1
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	mov r8, r3
	cmp r3, #0
	beq .L_02008572
	b .L_0200866a
.L_02008572:
	ldr r7, .L_02008670
	movs r2, #1
	ldr r3, [r7]
	negs r2, r2
	cmp r3, r2
	bne .L_020085b4
	ldr r6, .L_02008674
	movs r1, #160
	lsls r1, r1, #19
	ldr r5, .L_02008678
	adds r0, r6, #0
	adds r1, #96
	movs r2, #32
	mov lr, r5
	.2byte 0xf800
	adds r1, r6, #0
	movs r2, #32
	ldr r0, .L_0200867c
	mov lr, r5
	.2byte 0xf800
	ldr r6, .L_02008680
	ldr r1, .L_02008684
	movs r2, #32
	adds r0, r6, #0
	mov lr, r5
	.2byte 0xf800
	ldr r0, .L_02008688
	adds r1, r6, #0
	movs r2, #32
	mov lr, r5
	.2byte 0xf800
	mov r3, r8
	str r3, [r7]
.L_020085b4:
	ldr r3, [r7]
	cmp r3, #0
	bge .L_020085bc
	adds r3, #7
.L_020085bc:
	asrs r2, r3, #3
	adds r3, r2, #0
	cmp r2, #0
	bge .L_020085c6
	adds r3, r2, #3
.L_020085c6:
	asrs r1, r3, #2
	lsls r3, r1, #2
	subs r1, r2, r3
	ldr r5, .L_02008688
	ldr r2, .L_02008680
	ldr r7, .L_0200867c
	ldr r6, .L_02008674
	movs r4, #0
	mov r12, r5
	mov r8, r2
	movs r0, #24
.L_020085dc:
	adds r2, r1, r4
	adds r3, r2, #0
	cmp r2, #0
	bge .L_020085e6
	adds r3, r2, #3
.L_020085e6:
	asrs r3, r3, #2
	lsls r3, r3, #2
	subs r3, r2, r3
	ldrh r2, [r6, r0]
	lsls r3, r3, #1
	adds r3, #24
	strh r2, [r7, r3]
	mov r5, r8
	ldrh r5, [r5, r0]
	mov r2, r12
	adds r4, #1
	strh r5, [r2, r3]
	adds r0, #2
	cmp r4, #3
	ble .L_020085dc
	ldr r0, .L_0200868c
	ldr r1, .L_02008690
	ldrh r3, [r1]
	adds r4, r3, #0
	strh r1, [r1]
	ldrh r2, [r0]
	cmp r2, #31
	bgt .L_02008634
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	adds r3, r3, r0
	adds r3, #4
	adds r2, #1
	stmia r3!, {r7}
	strh r2, [r0]
	movs r2, #160
	lsls r2, r2, #19
	adds r2, #96
	stmia r3!, {r2}
	movs r2, #132
	lsls r2, r2, #24
	adds r2, #8
	str r2, [r3]
.L_02008634:
	strh r4, [r1]
	ldrh r3, [r1]
	adds r4, r3, #0
	strh r1, [r1]
	ldrh r2, [r0]
	cmp r2, #31
	bgt .L_02008660
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	adds r2, #1
	adds r3, r3, r0
	adds r3, #4
	strh r2, [r0]
	mov r2, r12
	stmia r3!, {r2}
	ldr r2, .L_02008684
	stmia r3!, {r2}
	movs r2, #132
	lsls r2, r2, #24
	adds r2, #8
	str r2, [r3]
.L_02008660:
	strh r4, [r1]
	ldr r2, .L_02008670
	ldr r3, [r2]
	adds r3, #1
	str r3, [r2]
.L_0200866a:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_02008670:
	.4byte Data_020024c8
.L_02008674:
	.4byte Data_020029d8
.L_02008678:
	.4byte IwramCopyWords
.L_0200867c:
	.4byte Data_020029f8
.L_02008680:
	.4byte Data_02002a18
.L_02008684:
	.4byte 0x050001a0
.L_02008688:
	.4byte Data_02002a38
.L_0200868c:
	.4byte gIoWriteQueue
.L_02008690:
	.4byte 0x04000208
	.section .text.x02008694,"ax",%progbits
	.global Func_02000694
	.thumb_func
Func_02000694:
	push {lr}
	ldr r0, .L_020086a4
	bl Func_02001bf4
	movs r0, #3
	bl WaitFrames
	pop {pc}
.L_020086a4:
	.4byte Func_02000540
	.section .text.x020086a8,"ax",%progbits
	.global Func_020006a8
	.thumb_func
Func_020006a8:
	push {lr}
	ldr r0, .L_020086b8
	bl Func_02001bfc
	movs r0, #3
	bl WaitFrames
	pop {pc}
.L_020086b8:
	.4byte Func_02000540
	.section .text.x020086bc,"ax",%progbits
	.global Func_020006bc
	.thumb_func
Func_020006bc:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r5, .L_02008754
	movs r2, #1
	ldr r3, [r5]
	negs r2, r2
	cmp r3, r2
	bne .L_020086e2
	movs r1, #160
	lsls r1, r1, #19
	ldr r3, .L_02008758
	ldr r0, .L_0200875c
	adds r1, #128
	movs r2, #32
	mov lr, r3
	.2byte 0xf800
	movs r3, #0
	str r3, [r5]
.L_020086e2:
	ldr r0, [r5]
	movs r1, #15
	lsrs r3, r0, #31
	adds r0, r0, r3
	asrs r0, r0, #1
	bl __modsi3
	movs r5, #0
	adds r7, r0, #0
	movs r6, #2
.L_020086f6:
	ldr r3, .L_02008760
	adds r0, r7, r5
	movs r1, #15
	mov r8, r3
	bl __modsi3
	ldr r3, .L_0200875c
	lsls r0, r0, #1
	ldrh r3, [r3, r6]
	adds r0, #2
	mov r2, r8
	adds r5, #1
	strh r3, [r2, r0]
	adds r6, #2
	cmp r5, #14
	ble .L_020086f6
	ldr r1, .L_02008764
	ldr r0, .L_02008768
	ldrh r3, [r0]
	adds r4, r3, #0
	strh r0, [r0]
	ldrh r2, [r1]
	cmp r2, #31
	bgt .L_02008744
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	adds r2, #1
	adds r3, r3, r1
	adds r3, #4
	strh r2, [r1]
	mov r2, r8
	stmia r3!, {r2}
	ldr r2, .L_0200876c
	stmia r3!, {r2}
	movs r2, #132
	lsls r2, r2, #24
	adds r2, #8
	str r2, [r3]
.L_02008744:
	strh r4, [r0]
	ldr r2, .L_02008754
	ldr r3, [r2]
	adds r3, #1
	str r3, [r2]
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_02008754:
	.4byte Data_020024cc
.L_02008758:
	.4byte IwramCopyWords
.L_0200875c:
	.4byte Data_02002a58
.L_02008760:
	.4byte Data_02002a78
.L_02008764:
	.4byte gIoWriteQueue
.L_02008768:
	.4byte 0x04000208
.L_0200876c:
	.4byte 0x050003c0
	.section .text.x02008770,"ax",%progbits
	.global Func_02000770
	.thumb_func
Func_02000770:
	push {r5, lr}
	movs r1, #0
	adds r5, r0, #0
	bl Animation_ApplyChildValues
	movs r3, #0
	str r3, [r5, #108]
	pop {r5, pc}
	.section .text.x02008780,"ax",%progbits
	.global Func_02000780
	.thumb_func
Func_02000780:
	push {lr}
	ldr r3, .L_020087a0
	movs r2, #2
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	beq .L_02008796
	movs r1, #7
	bl Animation_ApplyChildValues
	b .L_0200879c
.L_02008796:
	movs r1, #0
	bl Animation_ApplyChildValues
.L_0200879c:
	pop {pc}
	.2byte 0x0000
.L_020087a0:
	.4byte Data_0300122c
	.section .text.x020087a4,"ax",%progbits
	.global Func_020007a4
	.thumb_func
Func_020007a4:
	push {r5, lr}
	adds r0, r1, #0
	bl Object_GetById
	adds r5, r0, #0
	bl Func_02001cc4
	movs r0, #0
	bl Func_02001dcc
	ldr r3, .L_02008960
	movs r0, #78
	str r3, [r5, #108]
	bl Func_02001e2c
	movs r0, #30
	bl WaitFrames
	movs r0, #21
	bl Func_02001e2c
	bl Func_02001ad8
	movs r0, #17
	bl Func_02001ba0
	movs r0, #170
	bl Func_02001dec
	movs r5, #3
.L_020087e0:
	bl Random16Far
	adds r3, r0, #0
	movs r0, #254
	lsls r0, r0, #7
	adds r0, #255
	ands r0, r3
	movs r1, #0
	bl Func_02001da4
	movs r0, #15
	bl Func_02001dac
	movs r0, #10
	bl WaitFrames
	movs r0, #128
	movs r1, #0
	lsls r0, r0, #9
	bl Func_02001da4
	movs r0, #10
	bl Func_02001dac
	subs r5, #1
	movs r0, #10
	bl WaitFrames
	cmp r5, #0
	bge .L_020087e0
	bl Func_02001de4
	ldr r0, .L_02008964
	ldr r1, .L_02008968
	ldrh r3, [r1]
	adds r4, r3, #0
	strh r1, [r1]
	ldrh r2, [r0]
	cmp r2, #31
	bgt .L_02008852
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	adds r2, #1
	adds r3, r3, r0
	strh r2, [r0]
	movs r2, #240
	adds r3, #4
	lsls r2, r2, #4
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #80
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #10
	str r2, [r3]
.L_02008852:
	strh r4, [r1]
	ldrh r3, [r1]
	adds r4, r3, #0
	strh r1, [r1]
	ldrh r2, [r0]
	cmp r2, #31
	bgt .L_02008882
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
.L_02008882:
	strh r4, [r1]
	movs r0, #1
	bl WaitFrames
	ldr r0, .L_0200896c
	bl Scheduler_RemoveCallbackFar
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_02008970
	bl Scheduler_AddOrUpdateCallback
	bl Func_020017bc
	movs r0, #33
	movs r1, #0
	movs r2, #0
	bl Func_020017f8
	movs r5, #1
.L_020088aa:
	ldr r1, .L_02008964
	ldr r0, .L_02008968
	ldrh r3, [r0]
	adds r4, r3, #0
	strh r0, [r0]
	ldrh r3, [r1]
	cmp r3, #31
	bgt .L_020088de
	lsls r2, r3, #1
	adds r2, r2, r3
	adds r3, #1
	lsls r2, r2, #2
	strh r3, [r1]
	movs r3, #128
	adds r2, r2, r1
	lsls r3, r3, #5
	adds r2, #4
	orrs r3, r5
	stmia r2!, {r3}
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #82
	stmia r2!, {r3}
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r2]
.L_020088de:
	strh r4, [r0]
	movs r0, #4
	adds r5, #1
	bl WaitFrames
	cmp r5, #6
	ble .L_020088aa
	movs r0, #134
	lsls r0, r0, #4
	adds r0, #255
	bl GameFlag_SetBit
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #94
	bl GameFlag_SetBit
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #9
	lsls r1, r1, #6
	bl Func_02001d7c
	movs r0, #180
	movs r1, #1
	movs r2, #164
	movs r3, #1
	lsls r0, r0, #17
	negs r1, r1
	lsls r2, r2, #17
	bl Motion_CamBounds
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #20
	movs r0, #16
	bl Func_02001d6c
	bl Func_02001d8c
	movs r0, #220
	movs r1, #1
	movs r2, #188
	lsls r0, r0, #17
	negs r1, r1
	lsls r2, r2, #17
	movs r3, #1
	bl Motion_CamBounds
	bl Func_02001d8c
	movs r0, #236
	movs r1, #1
	movs r2, #204
	lsls r0, r0, #17
	negs r1, r1
	lsls r2, r2, #17
	movs r3, #1
	bl Motion_CamBounds
	movs r0, #77
	bl Func_02001d94
	pop {r5, pc}
	.2byte 0x0000
.L_02008960:
	.4byte Func_02000780
.L_02008964:
	.4byte gIoWriteQueue
.L_02008968:
	.4byte 0x04000208
.L_0200896c:
	.4byte Func_02000540
.L_02008970:
	.4byte Func_020006bc
	.section .text.x02008974,"ax",%progbits
	.global Func_02000974
	.thumb_func
Func_02000974:
	push {lr}
	bl Func_02001cc4
	movs r0, #0
	bl Func_02001dcc
	movs r0, #17
	bl Object_GetById
	ldr r3, .L_02008a3c
	str r3, [r0, #108]
	ldr r0, .L_02008a40
	bl Func_02001d44
	ldr r3, .L_02008a44
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	movs r1, #16
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	movs r2, #0
	movs r0, #16
	movs r1, #0
	bl ObjectMotion_ArmCallback
	movs r0, #16
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r1, #128
	movs r2, #0
	movs r0, #16
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r0, #16
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r1, #224
	movs r2, #0
	movs r0, #16
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r0, #16
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r1, #129
	movs r2, #20
	lsls r1, r1, #1
	movs r0, #16
	bl Func_02001d6c
	movs r0, #16
	movs r1, #0
	bl Func_02001d54
	movs r2, #25
	movs r0, #16
	movs r1, #2
	bl ObjectMotion_Launch
	movs r0, #16
	movs r1, #0
	bl Func_02001d54
	movs r0, #16
	movs r1, #1
	bl Motion_SetVarCbAndRefresh
	movs r0, #16
	movs r1, #0
	bl Func_02001d54
	movs r0, #16
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #16
	movs r1, #0
	bl Func_02001d54
	bl Func_02001ccc
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #94
	bl GameFlag_ClearBit
	pop {pc}
	.2byte 0x0000
.L_02008a3c:
	.4byte Func_02000770
.L_02008a40:
	.4byte 0x0000227b
.L_02008a44:
	.4byte gPartyState
	.section .text.x02008a48,"ax",%progbits
	.global Func_02000a48
	.thumb_func
Func_02000a48:
	push {lr}
	sub sp, #12
	movs r3, #68
	movs r2, #4
	movs r1, #1
	str r3, [sp, #0]
	str r2, [sp, #4]
	str r1, [sp, #8]
	movs r0, #68
	movs r1, #38
	movs r2, #39
	movs r3, #24
	bl Func_02001e0c
	add sp, #12
	pop {pc}
	.section .text.x02008a68,"ax",%progbits
	.global Func_02000a68
	.thumb_func
Func_02000a68:
	push {r5, r6, lr}
	ldr r3, .L_02008af4
	movs r2, #133
	lsls r2, r2, #2
	adds r6, r3, r2
	ldr r0, [r6]
	sub sp, #12
	bl Object_GetById
	movs r3, #68
	movs r2, #4
	movs r1, #1
	str r3, [sp, #0]
	str r2, [sp, #4]
	str r1, [sp, #8]
	adds r5, r0, #0
	movs r1, #38
	movs r0, #68
	movs r2, #38
	movs r3, #21
	bl Func_02001e0c
	movs r0, #240
	lsls r0, r0, #4
	adds r0, #155
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008aee
	movs r1, #188
	movs r2, #188
	movs r0, #65
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_02001d04
	ldr r3, [r5, #8]
	asrs r3, r3, #20
	cmp r3, #23
	bne .L_02008aee
	ldr r3, [r5, #16]
	asrs r3, r3, #20
	cmp r3, #23
	bne .L_02008aee
	bl Func_02001cc4
	movs r0, #0
	bl Func_02001dcc
	movs r1, #129
	ldr r0, [r6]
	lsls r1, r1, #1
	bl Func_02001d74
	ldr r0, [r6]
	movs r1, #16
	movs r2, #0
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r3, #192
	lsls r3, r3, #11
	str r3, [r5, #40]
	ldr r0, [r6]
	bl ObjectMotion_CommitCurrentPositionAndActivate
	bl Func_02001ccc
.L_02008aee:
	add sp, #12
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02008af4:
	.4byte gPartyState
	.section .text.x02008af8,"ax",%progbits
	.global Func_02000af8
	.thumb_func
Func_02000af8:
	push {r5, r6, lr}
	ldr r3, .L_02008b84
	movs r2, #133
	lsls r2, r2, #2
	adds r6, r3, r2
	ldr r0, [r6]
	sub sp, #12
	bl Object_GetById
	movs r3, #68
	movs r2, #4
	movs r1, #1
	str r3, [sp, #0]
	str r2, [sp, #4]
	str r1, [sp, #8]
	adds r5, r0, #0
	movs r1, #38
	movs r0, #68
	movs r2, #38
	movs r3, #21
	bl Func_02001e0c
	movs r0, #240
	lsls r0, r0, #4
	adds r0, #154
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008b7e
	movs r1, #150
	movs r2, #204
	movs r0, #64
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl Func_02001d04
	ldr r3, [r5, #8]
	asrs r3, r3, #20
	cmp r3, #37
	bne .L_02008b7e
	ldr r3, [r5, #16]
	asrs r3, r3, #20
	cmp r3, #25
	bne .L_02008b7e
	bl Func_02001cc4
	movs r0, #0
	bl Func_02001dcc
	movs r1, #129
	ldr r0, [r6]
	lsls r1, r1, #1
	bl Func_02001d74
	ldr r0, [r6]
	movs r1, #16
	movs r2, #0
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r3, #192
	lsls r3, r3, #11
	str r3, [r5, #40]
	ldr r0, [r6]
	bl ObjectMotion_CommitCurrentPositionAndActivate
	bl Func_02001ccc
.L_02008b7e:
	add sp, #12
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02008b84:
	.4byte gPartyState
	.section .text.x02008b88,"ax",%progbits
	.global Func_02000b88
	.thumb_func
Func_02000b88:
	push {lr}
	sub sp, #12
	movs r3, #67
	movs r2, #8
	movs r1, #1
	str r3, [sp, #0]
	str r2, [sp, #4]
	str r1, [sp, #8]
	movs r0, #67
	movs r1, #38
	movs r2, #11
	movs r3, #6
	bl Func_02001e0c
	add sp, #12
	pop {pc}
	.section .text.x02008ba8,"ax",%progbits
	.global Func_02000ba8
	.thumb_func
Func_02000ba8:
	push {r5, r6, lr}
	ldr r3, .L_02008c34
	movs r2, #133
	lsls r2, r2, #2
	adds r6, r3, r2
	ldr r0, [r6]
	sub sp, #12
	bl Object_GetById
	movs r3, #67
	movs r2, #8
	movs r1, #1
	str r3, [sp, #0]
	str r2, [sp, #4]
	str r1, [sp, #8]
	adds r5, r0, #0
	movs r1, #38
	movs r0, #67
	movs r2, #11
	movs r3, #6
	bl Func_02001e0c
	movs r0, #240
	lsls r0, r0, #4
	adds r0, #158
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008c2e
	movs r1, #200
	movs r2, #152
	movs r0, #64
	lsls r1, r1, #16
	lsls r2, r2, #16
	bl Func_02001d04
	ldr r3, [r5, #8]
	asrs r3, r3, #20
	cmp r3, #12
	bne .L_02008c2e
	ldr r3, [r5, #16]
	asrs r3, r3, #20
	cmp r3, #9
	bne .L_02008c2e
	bl Func_02001cc4
	movs r0, #0
	bl Func_02001dcc
	movs r1, #129
	ldr r0, [r6]
	lsls r1, r1, #1
	bl Func_02001d74
	ldr r0, [r6]
	movs r1, #16
	movs r2, #0
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r3, #192
	lsls r3, r3, #11
	str r3, [r5, #40]
	ldr r0, [r6]
	bl ObjectMotion_CommitCurrentPositionAndActivate
	bl Func_02001ccc
.L_02008c2e:
	add sp, #12
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02008c34:
	.4byte gPartyState
	.section .text.x02008c38,"ax",%progbits
	.global Func_02000c38
	.thumb_func
Func_02000c38:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r6, [r3, #108]
	bl Func_02001cc4
	movs r0, #0
	bl Func_02001dcc
	ldr r5, .L_02008cb0
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #7
	ldr r0, [r5]
	lsls r1, r1, #8
	bl ObjectMotion_SetSpeedParameters
	movs r1, #2
	ldr r0, [r5]
	bl ObjectMotion_SetActionVariant
	ldr r0, [r5]
	bl Object_GetById
	movs r3, #0
	adds r0, #85
	strb r3, [r0]
	movs r0, #123
	bl Func_02001e2c
	ldr r0, [r5]
	movs r1, #2
	bl Object_SetModeById
	movs r2, #6
	movs r1, #2
	negs r2, r2
	ldr r0, [r5]
	bl ObjectMotion_SnapHeadingAndOffset
	movs r0, #10
	bl Battle_WaitMode0
	movs r3, #170
	lsls r3, r3, #1
	adds r6, r6, r3
	movs r3, #0
	ldrsh r0, [r6, r3]
	bl Func_02001d94
	bl Event_ClearStatus1c6
	bl Event_WaitValue1c8Frames
	bl Func_02001ccc
	pop {r5, r6, pc}
.L_02008cb0:
	.4byte gPartyState
	.section .text.x02008cb4,"ax",%progbits
	.global Func_02000cb4
	.thumb_func
Func_02000cb4:
	push {r5, r6, lr}
	ldr r3, .L_02008d0c
	movs r2, #133
	lsls r2, r2, #2
	adds r6, r3, r2
	ldr r0, [r6]
	bl Object_GetById
	ldrh r5, [r0, #6]
	movs r3, #128
	lsls r3, r3, #6
	adds r5, r5, r3
	ldr r3, .L_02008d08
	ands r5, r3
	lsls r5, r5, #16
	asrs r5, r5, #16
	bl Func_02001cc4
	lsls r5, r5, #16
	movs r0, #0
	bl Func_02001dcc
	cmp r5, #0
	beq .L_02008d18
	movs r2, #0
	ldr r1, [r6]
	movs r0, #9
	bl ObjectMotion_SetAngleToward
	ldr r0, .L_02008d10
	bl Func_02001d44
	movs r0, #9
	movs r1, #0
	bl Func_02001d54
	movs r1, #128
	movs r0, #9
	lsls r1, r1, #6
	movs r2, #0
	b .L_02008d14
	.2byte 0x0000
.L_02008d08:
	.4byte 0xffffc000
.L_02008d0c:
	.4byte gPartyState
.L_02008d10:
	.4byte 0x00002280
.L_02008d14:
	bl ObjectMotion_ArmCallback
.L_02008d18:
	bl Func_02001ccc
	movs r0, #0
	pop {r5, r6, pc}
	.section .text.x02008d20,"ax",%progbits
	.global Func_02000d20
	.thumb_func
Func_02000d20:
	push {r5, lr}
	ldr r3, .L_02008d64
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	ldrh r5, [r0, #6]
	movs r3, #128
	lsls r3, r3, #6
	adds r5, r5, r3
	ldr r3, .L_02008d60
	ands r5, r3
	lsls r5, r5, #16
	asrs r5, r5, #16
	bl Func_02001cc4
	lsls r5, r5, #16
	movs r0, #0
	bl Func_02001dcc
	cmp r5, #0
	beq .L_02008d6c
	ldr r0, .L_02008d68
	bl Func_02001d44
	movs r0, #9
	movs r1, #0
	bl Func_02001d54
	b .L_02008d6c
.L_02008d60:
	.4byte 0xffffc000
.L_02008d64:
	.4byte gPartyState
.L_02008d68:
	.4byte 0x00002282
.L_02008d6c:
	bl Func_02001ccc
	movs r0, #0
	pop {r5, pc}
	.section .text.x02008d74,"ax",%progbits
	.global Func_02000d74
	.thumb_func
Func_02000d74:
	push {lr}
	ldr r3, .L_02008d8c
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	movs r1, #1
	movs r2, #5
	bl Func_02001d2c
	pop {pc}
	.2byte 0x0000
.L_02008d8c:
	.4byte gPartyState
	.section .text.x02008d90,"ax",%progbits
	.global Func_02000d90
	.thumb_func
Func_02000d90:
	push {lr}
	ldr r3, .L_02008dc0
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008dc4
	cmp r2, r3
	bne .L_02008da8
	ldr r0, .L_02008dc8
	b .L_02008dbe
.L_02008da8:
	ldr r3, .L_02008dcc
	cmp r2, r3
	bne .L_02008db2
	ldr r0, .L_02008dd0
	b .L_02008dbe
.L_02008db2:
	ldr r3, .L_02008dd4
	cmp r2, r3
	bne .L_02008dbc
	ldr r0, .L_02008dd8
	b .L_02008dbe
.L_02008dbc:
	ldr r0, .L_02008ddc
.L_02008dbe:
	pop {pc}
.L_02008dc0:
	.4byte gPartyState
.L_02008dc4:
	.4byte 0x0000009a
.L_02008dc8:
	.4byte Data_020024d0
.L_02008dcc:
	.4byte 0x0000009c
.L_02008dd0:
	.4byte Data_02002698
.L_02008dd4:
	.4byte 0x0000009b
.L_02008dd8:
	.4byte Data_0200280c
.L_02008ddc:
	.4byte Data_020024b8
	.section .text.x02008de0,"ax",%progbits
	.global Func_02000de0
	.thumb_func
Func_02000de0:
	push {r5, lr}
	ldr r3, .L_02008e18
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	adds r5, r0, #0
	ldr r0, [r3]
	bl Object_GetById
	ldrh r3, [r0, #6]
	movs r2, #128
	lsls r2, r2, #6
	adds r3, r3, r2
	ldr r2, .L_02008e14
	ands r3, r2
	movs r2, #192
	lsls r3, r3, #16
	lsls r2, r2, #24
	cmp r3, r2
	bne .L_02008e1c
	movs r0, #8
	adds r1, r5, #0
	bl Func_02001e24
	b .L_02008e38
	.2byte 0x0000
.L_02008e14:
	.4byte 0xffffc000
.L_02008e18:
	.4byte gPartyState
.L_02008e1c:
	bl Func_02001cc4
	movs r0, #0
	bl Func_02001dcc
	ldr r0, .L_02008e3c
	bl Func_02001d44
	adds r0, r5, #0
	movs r1, #0
	bl Func_02001d54
	bl Func_02001ccc
.L_02008e38:
	pop {r5, pc}
	.2byte 0x0000
.L_02008e3c:
	.4byte 0x00002291
	.section .text.x02008e40,"ax",%progbits
	.global Func_02000e40
	.thumb_func
Func_02000e40:
	push {r5, r6, lr}
	adds r6, r0, #0
	bl Func_02001cc4
	movs r0, #0
	bl Func_02001dcc
	ldr r5, .L_02008e94
	adds r0, r5, #0
	bl Func_02001d44
	movs r1, #0
	adds r0, r6, #0
	bl UiText_OpenMessageAtObject
	bl Func_02001e14
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_02008e7a
	movs r0, #10
	bl Battle_WaitMode0
	adds r0, r5, #1
	bl Func_02001d44
	b .L_02008e86
.L_02008e7a:
	movs r0, #20
	bl Battle_WaitMode0
	adds r0, r5, #2
	bl Func_02001d44
.L_02008e86:
	adds r0, r6, #0
	movs r1, #0
	bl Func_02001d54
	bl Func_02001ccc
	pop {r5, r6, pc}
.L_02008e94:
	.4byte 0x00002292
	.section .text.x02008e98,"ax",%progbits
	.global Func_02000e98
	.thumb_func
Func_02000e98:
	push {r5, lr}
	ldr r3, .L_02008ed0
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	adds r5, r0, #0
	ldr r0, [r3]
	bl Object_GetById
	ldrh r3, [r0, #6]
	movs r2, #128
	lsls r2, r2, #6
	adds r3, r3, r2
	ldr r2, .L_02008ecc
	ands r3, r2
	movs r2, #192
	lsls r3, r3, #16
	lsls r2, r2, #24
	cmp r3, r2
	bne .L_02008ed4
	movs r0, #23
	adds r1, r5, #0
	bl Func_02001e1c
	b .L_02008ef0
	.2byte 0x0000
.L_02008ecc:
	.4byte 0xffffc000
.L_02008ed0:
	.4byte gPartyState
.L_02008ed4:
	bl Func_02001cc4
	movs r0, #0
	bl Func_02001dcc
	ldr r0, .L_02008ef4
	bl Func_02001d44
	adds r0, r5, #0
	movs r1, #0
	bl Func_02001d54
	bl Func_02001ccc
.L_02008ef0:
	pop {r5, pc}
	.2byte 0x0000
.L_02008ef4:
	.4byte 0x0000228f
	.section .text.x02008ef8,"ax",%progbits
	.global Func_02000ef8
	.thumb_func
Func_02000ef8:
	push {lr}
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #6
	ldrh r2, [r3]
	ldr r3, .L_02008f24
	ldr r3, [r3]
	cmp r2, r3
	bge .L_02008f2c
	ldr r3, .L_02008f28
	ldrh r2, [r3]
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #20
	strh r2, [r3]
	movs r2, #128
	ldr r3, .L_02008f20
	lsls r2, r2, #19
	adds r2, #80
	b .L_02008f46
.L_02008f20:
	.4byte 0x00000000
.L_02008f24:
	.4byte Data_02002a98
.L_02008f28:
	.4byte Data_02002a9c
.L_02008f2c:
	ldr r3, .L_02008f54
	ldrh r2, [r3]
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #20
	strh r2, [r3]
	movs r2, #128
	ldr r3, .L_02008f4c
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	ldr r3, .L_02008f50
	subs r2, #2
.L_02008f46:
	strh r3, [r2]
	pop {pc}
	.2byte 0x0000
.L_02008f4c:
	.4byte 0x0000100c
.L_02008f50:
	.4byte 0x00003f42
.L_02008f54:
	.4byte Data_02002a9e
	.section .text.x02008f58,"ax",%progbits
	.global Func_02000f58
	.thumb_func
Func_02000f58:
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #32]
	movs r1, #188
	lsls r1, r1, #1
	adds r2, r2, r1
	movs r3, #6
	ldrsh r1, [r2, r3]
	ldr r0, .L_02008f88
	movs r3, #181
	lsls r3, r3, #3
	subs r3, r3, r1
	str r3, [r0]
	ldr r3, .L_02008f8c
	movs r1, #2
	ldrsh r2, [r2, r1]
	ldr r1, .L_02008f90
	strh r2, [r3]
	ldr r3, .L_02008f94
	ldr r3, [r3]
	lsrs r3, r3, #2
	subs r2, r2, r3
	strh r2, [r1]
	bx lr
.L_02008f88:
	.4byte Data_02002a98
.L_02008f8c:
	.4byte Data_02002a9c
.L_02008f90:
	.4byte Data_02002a9e
.L_02008f94:
	.4byte Data_0300122c
	.section .text.x02008f98,"ax",%progbits
	.global Func_02000f98
	.thumb_func
Func_02000f98:
	push {lr}
	ldr r2, .L_02008fb0
	movs r0, #1
	movs r1, #0
	bl Func_02001c0c
	movs r1, #200
	lsls r1, r1, #4
	ldr r0, .L_02008fb4
	bl Scheduler_AddOrUpdateCallback
	pop {pc}
.L_02008fb0:
	.4byte Func_02000ef8
.L_02008fb4:
	.4byte Func_02000f58
	.section .text.x02008fb8,"ax",%progbits
	.global Func_02000fb8
	.thumb_func
Func_02000fb8:
	push {lr}
	movs r0, #192
	lsls r0, r0, #18
	ldr r1, [r0, #32]
	movs r3, #13
	ldrb r2, [r1, #23]
	negs r3, r3
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	strb r3, [r1, #23]
	ldr r3, [r0, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r0, r3, r2
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #2
	ldr r1, .L_0200903c
	str r3, [r0]
	movs r4, #240
	lsls r4, r4, #1
	adds r3, r1, r4
	movs r4, #0
	ldrsh r2, [r3, r4]
	ldr r3, .L_02009040
	cmp r2, r3
	bne .L_02008ff6
	bl Func_02001054
	b .L_02009038
.L_02008ff6:
	ldr r3, .L_02009044
	cmp r2, r3
	bne .L_0200901a
	movs r1, #144
	ldr r0, .L_02009048
	lsls r1, r1, #3
	bl Scheduler_AddOrUpdateCallback
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #94
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009038
	bl Func_02000974
	b .L_02009038
.L_0200901a:
	ldr r3, .L_0200904c
	cmp r2, r3
	bne .L_02009038
	movs r3, #128
	lsls r3, r3, #1
	str r3, [r0]
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r1, r2
	movs r4, #0
	ldrsh r3, [r3, r4]
	cmp r3, #6
	bne .L_02009038
	bl Func_020010b4
.L_02009038:
	movs r0, #0
	pop {pc}
.L_0200903c:
	.4byte gPartyState
.L_02009040:
	.4byte 0x0000009b
.L_02009044:
	.4byte 0x0000009a
.L_02009048:
	.4byte Func_02000540
.L_0200904c:
	.4byte 0x0000009c
	.section .text.x02009054,"ax",%progbits
	.global Func_02001054
	.thumb_func
Func_02001054:
	push {r5, lr}
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_020090ac
	bl Scheduler_AddOrUpdateCallback
	bl Func_02001dfc
	movs r1, #128
	movs r2, #8
	movs r3, #9
	lsls r1, r1, #2
	movs r0, #0
	bl Func_02001e04
	movs r0, #170
	bl Func_02001dec
	bl Func_02000f98
	ldr r3, .L_020090b0
	movs r2, #241
	lsls r2, r2, #1
	adds r5, r3, r2
	movs r2, #0
	ldrsh r3, [r5, r2]
	cmp r3, #3
	bne .L_02009090
	bl Func_02001500
.L_02009090:
	movs r2, #0
	ldrsh r3, [r5, r2]
	cmp r3, #4
	bne .L_0200909c
	bl Func_02001500
.L_0200909c:
	movs r2, #0
	ldrsh r3, [r5, r2]
	cmp r3, #5
	bne .L_020090a8
	bl Func_02001500
.L_020090a8:
	pop {r5, pc}
	.2byte 0x0000
.L_020090ac:
	.4byte Func_020003e8
.L_020090b0:
	.4byte gPartyState
	.section .text.x020090b4,"ax",%progbits
	.global Func_020010b4
	.thumb_func
Func_020010b4:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r0, #48
	adds r2, #93
	str r2, [r3]
	adds r0, #255
	bl GameFlag_ClearBit
	pop {pc}
	.section .text.x020090d0,"ax",%progbits
	.global Func_020010d0
	.thumb_func
Func_020010d0:
	push {lr}
	bl Func_02001d9c
	pop {pc}
	.section .text.x020090d8,"ax",%progbits
	.global Func_020010d8
	.thumb_func
Func_020010d8:
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
	bl Func_02001c7c
	ldr r2, [r6, #80]
	adds r5, r0, #0
	mov r8, r2
	cmp r5, #0
	beq .L_02009134
	ldr r3, [r6, #20]
	ldr r7, [r5, #80]
	str r3, [r5, #20]
	ldr r1, .L_0200913c
	bl Func_02001c74
	adds r3, r5, #0
	adds r3, #85
	movs r5, #0
	strb r5, [r3]
	cmp r7, #0
	beq .L_02009134
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
.L_02009134:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200913c:
	.4byte Data_02001ee8
	.section .text.x02009140,"ax",%progbits
	.global Func_02001140
	.thumb_func
Func_02001140:
	push {r5, r6, r7, lr}
	ldr r3, .L_0200919c
	ldr r7, [r3]
	movs r3, #15
	ands r7, r3
	cmp r7, #0
	bne .L_02009198
	ldr r1, [r0, #8]
	ldr r2, [r0, #12]
	ldr r3, [r0, #16]
	movs r0, #14
	adds r0, #255
	bl Func_02001c7c
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02009198
	ldr r1, .L_020091a0
	ldr r6, [r5, #80]
	bl Func_02001c74
	adds r3, r5, #0
	adds r3, #85
	strb r7, [r3]
	ldr r3, .L_020091a4
	adds r2, r5, #0
	str r3, [r5, #12]
	adds r2, #34
	movs r3, #1
	strb r3, [r2]
	cmp r6, #0
	beq .L_02009198
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
.L_02009198:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200919c:
	.4byte Data_0300122c
.L_020091a0:
	.4byte Data_02001ef4
.L_020091a4:
	.4byte 0xfff88000
	.section .text.x020091a8,"ax",%progbits
	.global Func_020011a8
	.thumb_func
Func_020011a8:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r6, .L_020092f4
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
	ldr r3, .L_020092f8
	cmp r2, r3
	bne .L_020091ec
	ldr r3, [r5, #20]
	cmp r3, #0
	beq .L_020092e8
.L_020091ec:
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
	bl Func_02001c9c
	ldr r3, .L_020092fc
	movs r2, #4
	ldr r3, [r3]
	adds r6, r0, #0
	ands r3, r2
	cmp r3, #0
	bne .L_0200921c
	adds r0, r5, #0
	bl Func_020010d8
.L_0200921c:
	cmp r6, #0
	bge .L_0200926e
	movs r1, #129
	mov r0, r8
	lsls r1, r1, #1
	bl Func_02001d74
	ldr r3, [r5, #16]
	movs r0, #128
	lsls r0, r0, #12
	adds r3, r3, r0
	ldr r1, [r5, #8]
	ldr r2, [r5, #12]
	adds r0, r5, #0
	bl Func_02001c84
	adds r0, r5, #0
	movs r1, #49
	bl Func_02001c6c
	adds r0, r5, #0
	bl Func_02001c8c
.L_0200924a:
	movs r0, #1
	bl WaitFrames
	ldr r2, [r5, #12]
	ldr r3, [r5, #20]
	cmp r2, r3
	bne .L_0200924a
	adds r0, r5, #0
	bl Func_020010d8
	adds r0, r5, #0
	movs r1, #49
	bl Func_02001c6c
	movs r0, #3
	bl WaitFrames
	b .L_020092e8
.L_0200926e:
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
	bl Func_02001c9c
	adds r6, r0, #0
	cmp r6, #0
	bgt .L_020092e8
	ldr r3, [r5, #8]
	ldr r2, .L_02009300
	adds r0, r5, #0
	adds r3, r3, r2
	str r3, [r7]
	adds r1, r7, #0
	ldr r3, [r5, #12]
	str r3, [r7, #4]
	ldr r3, [r5, #16]
	adds r3, r3, r2
	str r3, [r7, #8]
	bl Func_02001c9c
	adds r6, r0, #0
	cmp r6, #0
	bgt .L_020092e8
	ldr r3, [r5, #8]
	ldr r2, .L_02009304
	ldr r0, .L_02009300
	adds r3, r3, r2
	str r3, [r7]
	adds r1, r7, #0
	ldr r3, [r5, #12]
	str r3, [r7, #4]
	ldr r3, [r5, #16]
	adds r3, r3, r0
	str r3, [r7, #8]
	adds r0, r5, #0
	bl Func_02001c9c
	adds r6, r0, #0
	cmp r6, #0
	bgt .L_020092e8
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
.L_020092e8:
	add sp, #12
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_020092f4:
	.4byte gPartyState
.L_020092f8:
	.4byte 0x0000009e
.L_020092fc:
	.4byte Data_0300122c
.L_02009300:
	.4byte 0x0005b333
.L_02009304:
	.4byte 0xfffa4ccd
	.section .text.x02009308,"ax",%progbits
	.global Func_02001308
	.thumb_func
Func_02001308:
	push {r5, lr}
	bl Object_GetById
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02009334
	movs r1, #126
	adds r1, #255
	ldr r0, [r5, #80]
	bl ResourceMetadata_Register
	movs r3, #0
	strb r3, [r0, #5]
	strb r3, [r0, #6]
	movs r1, #0
	adds r0, r5, #0
	bl Func_02001c6c
	adds r0, r5, #0
	movs r1, #2
	bl Func_02001c6c
.L_02009334:
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x02009338,"ax",%progbits
	.global Func_02001338
	.thumb_func
Func_02001338:
	push {r5, r6, r7, lr}
	ldr r5, .L_020093d0
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
	adds r7, r0, #0
	ldr r0, [r5]
	bl Object_GetById
	adds r6, r0, #0
	bl Func_02001cc4
	movs r0, #0
	bl Func_02001dcc
	ldr r0, [r5]
	movs r1, #1
	bl ObjectMotion_SetActionVariant
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
	bl Func_02001e2c
	adds r0, r6, #0
	movs r1, #18
	bl Func_02001c6c
	movs r0, #153
	lsls r0, r0, #2
	bl Func_02001e2c
	movs r5, #0
.L_0200938a:
	cmp r5, #30
	bne .L_02009392
	bl Event_ClearStatus1c6
.L_02009392:
	ldr r3, [r6, #12]
	ldr r2, .L_020093d4
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
	bne .L_020093b6
	movs r0, #15
	bl Object_GetById
	bl Func_020010d8
.L_020093b6:
	movs r0, #1
	adds r5, #1
	bl WaitFrames
	cmp r5, #59
	ble .L_0200938a
	bl Func_02001ccc
	adds r0, r7, #0
	bl Func_02001d94
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_020093d0:
	.4byte gPartyState
.L_020093d4:
	.4byte 0xffffc000
	.section .text.x020093d8,"ax",%progbits
	.global Func_020013d8
	.thumb_func
Func_020013d8:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r3, .L_02009460
	sub sp, #56
	ldr r2, [r3]
	mov r8, r3
	movs r3, #1
	ands r3, r2
	adds r7, r0, #0
	cmp r3, #0
	beq .L_02009454
	movs r3, #7
	add r6, sp, #16
	str r3, [r6, #4]
	movs r3, #2
	ands r3, r2
	cmp r3, #0
	bne .L_02009402
	movs r3, #5
	str r3, [r6, #4]
.L_02009402:
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
.L_02009454:
	movs r0, #0
	add sp, #56
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009460:
	.4byte Data_0300122c
	.section .text.x02009464,"ax",%progbits
	.global Func_02001464
	.thumb_func
Func_02001464:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r8
	push {r5, r6}
	ldr r5, .L_020094f8
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
	mov r10, r0
	ldr r0, [r5]
	bl Object_GetById
	adds r6, r0, #0
	bl Func_02001cc4
	movs r0, #0
	bl Func_02001dcc
	movs r0, #228
	bl Func_02001e2c
	ldr r3, .L_020094fc
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
	bl Object_SetPartAttribute
	ldr r0, [r5]
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	mov r3, r8
	str r3, [r6, #108]
	bl Event_ClearStatus1c6
	bl Event_WaitValue1c8Frames
	mov r0, r10
	bl Func_02001d94
	bl Func_02001ccc
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, pc}
	.2byte 0x0000
.L_020094f8:
	.4byte gPartyState
.L_020094fc:
	.4byte Func_020013d8
	.section .text.x02009500,"ax",%progbits
	.global Func_02001500
	.thumb_func
Func_02001500:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r3, .L_020095ec
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r3, r2
	ldr r0, [r5]
	bl Object_GetById
	adds r6, r0, #0
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	adds r7, r0, #0
	cmp r7, #0
	bne .L_020095e6
	bl Func_02001cc4
	movs r0, #0
	bl Func_02001dcc
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r2, r2
	negs r0, r0
	negs r1, r1
	movs r3, #0
	bl Motion_CamBounds
	movs r3, #85
	adds r3, r3, r6
	strb r7, [r3]
	mov r8, r3
	movs r2, #10
	ldrsh r1, [r6, r2]
	movs r3, #18
	ldrsh r2, [r6, r3]
	ldr r3, .L_020095f0
	lsls r2, r2, #16
	adds r2, r2, r3
	lsls r1, r1, #16
	ldr r0, [r5]
	bl Func_02001d04
	ldr r0, [r5]
	bl Object_GetById
	movs r1, #9
	bl Object_SetPartAttribute
	ldr r0, [r5]
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	bl Event_SetStatus1c6
	movs r0, #228
	bl Func_02001e2c
	ldr r3, .L_020095f4
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
	bl Object_SetPartAttribute
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
	bl Func_02001df4
	bl Event_WaitValue1c8Frames
	bl Func_02001ccc
.L_020095e6:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_020095ec:
	.4byte gPartyState
.L_020095f0:
	.4byte 0xfff00000
.L_020095f4:
	.4byte Func_020013d8
	.section .text.x020095f8,"ax",%progbits
	.global Func_020015f8
	.thumb_func
Func_020015f8:
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
	ldr r3, .L_02009774
	adds r2, r1, #0
	adds r2, #228
	ldr r0, [r2]
	ldr r2, [r2, #4]
	ands r0, r3
	ands r2, r3
	ldr r3, [r1]
	sub sp, #4
	ldr r3, [r3, #4]
	ldr r5, .L_02009778
	str r3, [sp, #0]
	mov r8, r2
	ldrh r3, [r5]
	ldr r2, .L_0200977c
	lsls r3, r3, #2
	adds r3, r3, r2
	ldrh r2, [r3, #2]
	ldrh r4, [r3, #2]
	lsrs r2, r2, #5
	mov r11, r2
	ldr r2, .L_02009780
	movs r3, #128
	adds r1, r4, r2
	movs r2, #132
	lsls r3, r3, #19
	lsls r2, r2, #24
	mov r9, r0
	adds r3, #212
	ldr r0, .L_02009784
	adds r2, #16
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r2, .L_02009788
	ldr r0, .L_0200978c
	adds r1, r4, r2
	movs r2, #132
	lsls r2, r2, #24
	adds r2, #16
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r2, .L_02009790
	ldr r0, .L_02009794
	adds r1, r4, r2
	movs r2, #132
	lsls r2, r2, #24
	adds r2, #16
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r2, .L_02009798
	ldr r0, .L_0200979c
	adds r1, r4, r2
	movs r2, #132
	lsls r2, r2, #24
	adds r2, #16
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r2, .L_020097a0
	ldr r0, .L_020097a4
	adds r1, r4, r2
	movs r2, #132
	lsls r2, r2, #24
	adds r2, #16
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r2, .L_020097a8
	ldr r0, .L_020097ac
	adds r1, r4, r2
	movs r2, #132
	lsls r2, r2, #24
	adds r2, #16
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r2, .L_020097b0
	ldr r0, .L_020097a4
	adds r1, r4, r2
	movs r2, #132
	lsls r2, r2, #24
	adds r2, #16
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r2, .L_020097b4
	ldr r0, .L_020097ac
	adds r1, r4, r2
	movs r2, #132
	lsls r2, r2, #24
	adds r2, #16
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r3, #127
	adds r5, #4
	mov r10, r3
.L_020096c4:
	ldrh r0, [r5, #18]
	cmp r0, #0
	beq .L_02009758
	ldr r3, [r5]
	mov r1, r9
	subs r4, r3, r1
	ldr r3, [r5, #8]
	ldr r2, [r5, #4]
	mov r1, r8
	subs r3, r3, r1
	subs r1, r3, r2
	ldr r3, [r5, #12]
	cmp r2, r3
	bne .L_020096ea
	movs r2, #236
	lsls r2, r2, #8
	mov r12, r2
	cmp r0, #2
	bne .L_020096f0
.L_020096ea:
	movs r3, #232
	lsls r3, r3, #8
	mov r12, r3
.L_020096f0:
	movs r0, #0
	asrs r3, r4, #16
	asrs r2, r1, #16
	mov lr, r0
	movs r0, #167
	adds r4, r3, #0
	adds r1, r2, #0
	adds r3, #7
	lsls r0, r0, #1
	subs r4, #8
	subs r1, #8
	cmp r3, r0
	bhi .L_02009758
	adds r3, r2, #0
	movs r2, #143
	adds r3, #39
	lsls r2, r2, #1
	cmp r3, r2
	bhi .L_02009758
	movs r3, #128
	lsls r3, r3, #1
	adds r3, #255
	ands r4, r3
	movs r3, #255
	ands r1, r3
	ldrh r3, [r5, #16]
	adds r7, r5, #0
	adds r7, #20
	adds r2, r7, #0
	lsls r6, r3, #2
	movs r0, #0
	cmp r3, #2
	bne .L_02009736
	movs r0, #128
	lsls r0, r0, #21
.L_02009736:
	mov r3, lr
	str r3, [r2]
	lsls r3, r4, #16
	orrs r1, r3
	ldr r3, .L_020097b8
	orrs r1, r0
	orrs r1, r3
	mov r0, r11
	adds r3, r0, r6
	str r1, [r5, #24]
	mov r1, r12
	orrs r1, r3
	str r1, [r5, #28]
	adds r0, r7, #0
	movs r1, #0
	bl Func_02001c3c
.L_02009758:
	movs r2, #1
	negs r2, r2
	add r10, r2
	mov r3, r10
	adds r5, #32
	cmp r3, #0
	bge .L_020096c4
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_02009774:
	.4byte 0xffff0000
.L_02009778:
	.4byte Data_02002aa0
.L_0200977c:
	.4byte ResourceTableEntries
.L_02009780:
	.4byte 0x06010000
.L_02009784:
	.4byte 0x0600f000
.L_02009788:
	.4byte 0x06010040
.L_0200978c:
	.4byte 0x0600f400
.L_02009790:
	.4byte 0x06010080
.L_02009794:
	.4byte 0x0600f040
.L_02009798:
	.4byte 0x060100c0
.L_0200979c:
	.4byte 0x0600f440
.L_020097a0:
	.4byte 0x06010100
.L_020097a4:
	.4byte 0x0600f080
.L_020097a8:
	.4byte 0x06010140
.L_020097ac:
	.4byte 0x0600f480
.L_020097b0:
	.4byte 0x06010180
.L_020097b4:
	.4byte 0x060101c0
.L_020097b8:
	.4byte 0x40000400
	.section .text.x020097bc,"ax",%progbits
	.global Func_020017bc
	.thumb_func
Func_020017bc:
	push {r5, lr}
	ldr r5, .L_020097ec
	movs r1, #128
	lsls r1, r1, #5
	ldr r3, .L_020097f0
	adds r1, #4
	adds r0, r5, #0
	mov lr, r3
	.2byte 0xf800
	bl Resource_FindFreeEntry
	strh r0, [r5]
	movs r1, #128
	ldrh r0, [r5]
	lsls r1, r1, #2
	movs r2, #0
	bl VramBlock_LoadCached
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_020097f4
	bl Scheduler_AddOrUpdateCallback
	pop {r5, pc}
.L_020097ec:
	.4byte Data_02002aa0
.L_020097f0:
	.4byte IwramClearWords
.L_020097f4:
	.4byte Func_020015f8
	.section .text.x020097f8,"ax",%progbits
	.global Func_020017f8
	.thumb_func
Func_020017f8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #28
	str r1, [sp, #24]
	str r2, [sp, #20]
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	movs r1, #132
	mov r12, r3
	movs r3, #160
	lsls r3, r3, #1
	add r3, r12
	ldr r3, [r3, #48]
	lsls r1, r1, #1
	mov r8, r3
	mov r3, r12
	add r1, r12
	adds r3, #236
	ldr r2, [r1, #8]
	mov r10, r0
	ldr r0, [r3]
	adds r3, #4
	adds r2, r2, r0
	str r2, [sp, #16]
	ldr r2, [r3]
	ldr r1, [r1, #12]
	adds r3, #4
	adds r1, r1, r2
	str r1, [sp, #12]
	ldr r1, .L_02009938
	ldr r3, [r3]
	mov r9, r1
	subs r3, r3, r0
	asrs r3, r3, #20
	str r3, [sp, #8]
	mov r3, r12
	adds r3, #248
	ldr r3, [r3]
	asrs r0, r0, #20
	subs r3, r3, r2
	asrs r3, r3, #20
	str r3, [sp, #4]
	asrs r2, r2, #20
	ldrh r3, [r1, #2]
	lsls r2, r2, #7
	adds r2, r2, r0
	str r2, [sp, #0]
	ldr r2, [sp, #24]
	ldr r1, .L_0200993c
	lsls r3, r3, #5
	add r3, r9
	adds r5, r3, #4
	lsls r3, r2, #8
	str r3, [r1]
	ldr r1, [sp, #0]
	movs r2, #0
	lsls r3, r1, #2
	add r8, r3
	ldr r3, [sp, #4]
	mov lr, r2
	cmp lr, r3
	bge .L_02009928
.L_02009880:
	mov r1, lr
	lsls r1, r1, #16
	lsrs r3, r1, #7
	mov r2, r8
	adds r6, r2, r3
	ldr r3, [sp, #8]
	movs r7, #0
	mov r11, r1
	cmp r7, r3
	bge .L_02009912
.L_02009894:
	ldrb r4, [r6, #2]
	cmp r4, #0
	beq .L_020098fe
	cmp r4, r10
	bcc .L_020098fe
	mov r3, r10
	adds r3, #4
	cmp r4, r3
	bcs .L_020098fe
	ldr r1, [sp, #16]
	lsls r0, r7, #16
	lsrs r0, r0, #16
	lsls r3, r0, #20
	movs r2, #128
	adds r3, r3, r1
	lsls r2, r2, #12
	adds r3, r3, r2
	str r3, [r5]
	ldr r2, [sp, #12]
	mov r3, r11
	lsrs r1, r3, #16
	lsls r3, r1, #20
	adds r3, r3, r2
	movs r2, #128
	lsls r2, r2, #12
	adds r3, r3, r2
	str r3, [r5, #8]
	ldr r2, [sp, #20]
	lsls r1, r1, #7
	lsls r3, r2, #19
	str r3, [r5, #12]
	ldr r2, [sp, #24]
	adds r0, r0, r1
	lsls r3, r2, #19
	mov r2, r10
	str r3, [r5, #4]
	subs r3, r4, r2
	strh r3, [r5, #16]
	movs r3, #1
	strh r3, [r5, #18]
	mov r2, r9
	ldrh r3, [r2, #2]
	adds r5, #32
	adds r3, #1
	strh r3, [r2, #2]
	movs r3, #158
	lsls r3, r3, #1
	add r3, r12
	ldr r2, [r3]
	ldr r3, [sp, #0]
	adds r2, r2, r3
	movs r3, #120
	strb r3, [r2, r0]
.L_020098fe:
	movs r1, #128
	lsls r3, r7, #16
	lsls r1, r1, #9
	ldr r2, [sp, #8]
	adds r3, r3, r1
	asrs r7, r3, #16
	lsrs r3, r3, #16
	adds r6, #4
	cmp r3, r2
	blt .L_02009894
.L_02009912:
	mov r1, lr
	movs r2, #128
	lsls r3, r1, #16
	lsls r2, r2, #9
	adds r3, r3, r2
	ldr r2, [sp, #4]
	asrs r1, r3, #16
	lsrs r3, r3, #16
	mov lr, r1
	cmp r3, r2
	blt .L_02009880
.L_02009928:
	add sp, #28
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009938:
	.4byte Data_02002aa0
.L_0200993c:
	.4byte Data_0202c001 + 0x1df
	.section .text.x02009940,"ax",%progbits
	.global Func_02001940
	.thumb_func
Func_02001940:
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
.L_0200996a:
	ldrh r3, [r5, #28]
	movs r1, #255
	lsls r1, r1, #8
	adds r1, #255
	adds r3, r3, r1
	adds r2, r1, #0
	ands r2, r3
	strh r3, [r5, #28]
	cmp r2, r1
	bne .L_02009980
	b .L_02009ab6
.L_02009980:
	movs r0, #179
	lsls r0, r0, #1
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009992
	ldrh r3, [r5, #28]
	adds r3, #1
	strh r3, [r5, #28]
.L_02009992:
	ldrh r2, [r5, #28]
	mov r1, r11
	lsls r3, r2, #2
	adds r3, r3, r2
	ldr r2, .L_02009a40
	lsls r3, r3, #1
	adds r4, r3, r2
	ldr r3, [r5, #12]
	subs r2, r3, r1
	cmp r2, #0
	bge .L_020099b0
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	adds r2, r2, r3
.L_020099b0:
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
	bge .L_020099d0
	movs r1, #255
	lsls r1, r1, #8
	adds r1, #255
	adds r3, r3, r1
.L_020099d0:
	movs r1, #0
	ldrsh r2, [r4, r1]
	asrs r3, r3, #16
	adds r6, r3, r2
	adds r3, r7, #0
	adds r3, #16
	adds r4, #2
	cmp r3, #255
	bhi .L_02009a60
	movs r2, #32
	negs r2, r2
	cmp r6, r2
	blt .L_02009a60
	cmp r6, #159
	bgt .L_02009a60
	ldrb r3, [r5, #9]
	movs r1, #13
	negs r1, r1
	adds r2, r1, #0
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	strb r3, [r5, #9]
	ldr r3, .L_02009a30
	ldr r2, .L_02009a34
	ands r7, r3
	ldrh r3, [r5, #6]
	strb r6, [r5, #4]
	ands r3, r2
	orrs r3, r7
	strh r3, [r5, #6]
	mov r2, r8
	ldrh r3, [r4]
	ldr r1, [r2, #4]
	ldr r2, .L_02009a38
	adds r1, r1, r3
	ldr r3, .L_02009a3c
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
	b .L_02009a44
.L_02009a30:
	.4byte 0x000001ff
.L_02009a34:
	.4byte 0xfffffe00
.L_02009a38:
	.4byte 0xfffffc00
.L_02009a3c:
	.4byte 0x000003ff
.L_02009a40:
	.4byte Data_02001f18
.L_02009a44:
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
	bl Func_02001c3c
.L_02009a60:
	ldrh r3, [r5, #28]
	cmp r3, #0
	bne .L_02009ab6
	movs r3, #128
	lsls r3, r3, #4
	adds r3, #8
	add r3, r8
	ldr r6, [r3]
	cmp r6, #0
	beq .L_02009aac
	bl Random16Far
	ldr r3, [r6]
	lsls r2, r0, #4
	ldr r1, .L_02009ad0
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
	ldr r2, .L_02009ad4
	str r7, [r5, #12]
	adds r6, r3, r2
	str r6, [r5, #20]
	movs r0, #0
	adds r1, r7, #0
	adds r2, r6, #0
	bl Map_GetTerrainHeight
	movs r3, #16
	str r0, [r5, #16]
	b .L_02009ab4
.L_02009aac:
	movs r3, #16
	str r6, [r5, #12]
	str r6, [r5, #20]
	str r6, [r5, #16]
.L_02009ab4:
	strh r3, [r5, #28]
.L_02009ab6:
	movs r3, #1
	add r10, r3
	mov r1, r10
	adds r5, #32
	cmp r1, #63
	bhi .L_02009ac4
	b .L_0200996a
.L_02009ac4:
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_02009ad0:
	.4byte 0xff880000
.L_02009ad4:
	.4byte 0xffb00000
	.section .text.x02009ad8,"ax",%progbits
	.global Func_02001ad8
	.thumb_func
Func_02001ad8:
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
	ldr r2, .L_02009b8c
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r1, #128
	lsls r1, r1, #3
	movs r0, #56
	bl Runtime_AllocateBlock
	adds r6, r0, #0
	adds r1, r6, #0
	ldr r0, .L_02009b90
	bl Func_02001c24
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
.L_02009b40:
	movs r2, #0
	adds r3, r7, #0
	str r7, [sp, #0]
	stmia r3!, {r2}
	adds r1, r3, #0
	ldr r3, .L_02009b94
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
	ldr r2, .L_02009b88
	adds r3, r5, #0
	ands r3, r2
	lsls r0, r0, #16
	adds r3, #1
	adds r5, #1
	str r0, [r7, #16]
	strh r3, [r7, #28]
	adds r7, #32
	cmp r5, #63
	bls .L_02009b40
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_02009b98
	bl Scheduler_AddOrUpdateCallback
	add sp, #8
	b .L_02009b9c
.L_02009b88:
	.4byte 0x0000000f
.L_02009b8c:
	.4byte 0x85000205
.L_02009b90:
	.4byte Data_02002890
.L_02009b94:
	.4byte 0x40000400
.L_02009b98:
	.4byte Func_02001940
.L_02009b9c:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x02009ba0,"ax",%progbits
	.global Func_02001ba0
	.thumb_func
Func_02001ba0:
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
	beq .L_02009bc8
	cmp r0, #0
	beq .L_02009bc8
	adds r3, r0, #0
	adds r3, #8
	str r3, [r5]
.L_02009bc8:
	pop {r5, r6, pc}
	.2byte 0x0000
	.section .rodata.x02009e34,"a",%progbits
.L_02009e34:
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
.L_02009e70:
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
.L_02009eac:
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
	.global Data_02001ee8
Data_02001ee8:
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000026
	.global Data_02001ef4
Data_02001ef4:
	.4byte 0x00000017
	.4byte 0x00000006
	.4byte 0x00004000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000001e
	.4byte 0x00000000
	.4byte 0x00000026
	.global Data_02001f18
Data_02001f18:
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
	.global Data_02002038
Data_02002038:
	.4byte .L_02009e34
	.4byte .L_02009e70
	.4byte .L_02009eac
.L_0200a044:
	.4byte 0x00000016
	.4byte 0x0000001e
	.4byte 0x00000081
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00003333
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00009999
	.4byte 0x80010000
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0000003c
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000000
	.4byte 0x0000003c
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0000003c
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000000
	.4byte 0x0000003c
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0xc0010000
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
	.4byte 0x0000009a
	.4byte 0x10124002
	.4byte 0xffffffff
	.4byte 0x1020109b
	.4byte 0xffffffff
	.4byte 0x1030209b
	.4byte 0xffffffff
	.4byte 0x1040109c
	.4byte 0xffffffff
	.4byte 0x1050209c
	.4byte 0xffffffff
	.4byte 0x1060309c
	.4byte 0xffffffff
	.4byte 0x1070409c
	.4byte 0xffffffff
	.4byte 0x1080509c
	.4byte 0xffffffff
	.4byte 0x1090503a
	.4byte 0xffffffff
	.4byte 0x04d4e002
	.4byte 0x0000009c
	.4byte 0x1010409a
	.4byte 0xffffffff
	.4byte 0x1020509a
	.4byte 0xffffffff
	.4byte 0x1030609a
	.4byte 0xffffffff
	.4byte 0x1040709a
	.4byte 0xffffffff
	.4byte 0x1050809a
	.4byte 0xffffffff
	.4byte 0x10602087
	.4byte 0xffffffff
	.4byte 0x0000009b
	.4byte 0x1010209a
	.4byte 0xffffffff
	.4byte 0x1020309a
	.4byte 0xffffffff
	.4byte 0x1030309d
	.4byte 0xffffffff
	.4byte 0x1040409d
	.4byte 0xffffffff
	.4byte 0x1050509d
	.4byte 0xffffffff
	.4byte 0x000001ff
	.global Data_02002200
Data_02002200:
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02002218
Data_02002218:
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x00380000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x0001a000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x01c80000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x0003e000
	.4byte 0xffff0072
	.4byte 0x00000001
	.4byte 0x02380000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x0001e000
	.4byte 0xffff0080
	.4byte 0x00000002
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x0000c000
	.4byte 0xffff0070
	.4byte 0x00000001
	.4byte 0x02680000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00012000
	.4byte 0xffff0071
	.4byte 0x00000002
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x0001e000
	.4byte 0xffff0088
	.4byte 0x00000003
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x00004000
	.4byte 0xffff008a
	.4byte 0x00000003
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x00002000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x0001c000
	.4byte 0xffff017f
	.4byte 0x00000001
	.4byte 0x01600000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02002320
Data_02002320:
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00010000
	.4byte 0xffff0088
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00022000
	.4byte 0xffff0072
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00012000
	.4byte 0xffff0080
	.4byte .L_0200a044
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00010000
	.4byte 0xffff0088
	.4byte 0x00000001
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00014000
	.4byte 0xffff008a
	.4byte 0x00000002
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x0001e000
	.4byte 0xffff0070
	.4byte 0x00000003
	.4byte 0x02500000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00012000
	.4byte 0xffff0071
	.4byte 0x00000001
	.4byte 0x02280000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00014000
	.4byte 0xffff006e
	.4byte 0x00000001
	.4byte 0x01a00000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00014000
	.4byte 0xffff0086
	.4byte 0x00000001
	.4byte 0x00900000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00014000
	.4byte 0xffff0087
	.4byte 0x00000001
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x00016000
	.4byte 0xffff0070
	.4byte 0x00000003
	.4byte 0x00800000
	.4byte 0x00000000
	.4byte 0x026c0000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02002458
Data_02002458:
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x0002c000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x0002c000
	.4byte 0x007700f6
	.4byte 0x00000001
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00008000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020024b8
Data_020024b8:
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020024c4
Data_020024c4:
	.4byte 0xffffffff
	.global Data_020024c8
Data_020024c8:
	.4byte 0xffffffff
	.global Data_020024cc
Data_020024cc:
	.4byte 0xffffffff
	.global Data_020024d0
Data_020024d0:
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
	.4byte 0xffff0004
	.4byte Func_02000c38
	.4byte 0x00000002
	.4byte 0xffff0005
	.4byte Func_02000c38
	.4byte 0x00000002
	.4byte 0xffff0006
	.4byte Func_02000c38
	.4byte 0x00000002
	.4byte 0xffff0007
	.4byte Func_02000c38
	.4byte 0x00000002
	.4byte 0xffff0008
	.4byte Func_02000c38
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x0000c602
	.4byte 0xffff0021
	.4byte Func_020010d0
	.4byte 0x0000c602
	.4byte 0xffff0015
	.4byte Func_020010d0
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00002269
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x0000226a
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x0000226b
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x0000226c
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x0000226d
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x0000226e
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x0000226f
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x00002270
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x00002271
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00002272
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00002273
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00002274
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00002275
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00002276
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00002277
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x00002278
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x00002279
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x0000227a
	.4byte 0x00002115
	.4byte 0x095f0011
	.4byte Func_020007a4
	.4byte 0x00008b85
	.4byte 0xffff0000
	.4byte Func_020003b0
	.4byte 0x40008b85
	.4byte 0xffff0000
	.4byte Func_020003cc
	.4byte 0x50009085
	.4byte 0xffff0000
	.4byte Func_02000694
	.4byte 0x40009085
	.4byte 0xffff0000
	.4byte Func_020006a8
	.4byte 0x50008905
	.4byte 0xffff0015
	.4byte Func_02000a48
	.4byte 0x50008905
	.4byte 0xffff0016
	.4byte Func_02000a68
	.4byte 0x50008905
	.4byte 0xffff0017
	.4byte Func_02000af8
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02002698
Data_02002698:
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
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x0000227f
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte Func_02000cb4
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x00002283
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x00002284
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x00002285
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x00002286
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x0000228b
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x0000228c
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte Func_02000e98
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte Func_02000de0
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte Func_02000e40
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00002281
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte Func_02000d20
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00002287
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00002288
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00002289
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x0000228a
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x0000228d
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x0000228e
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x00002290
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x00002295
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x00002296
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x000021ae
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x000021b0
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_0200280c
Data_0200280c:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000002
	.4byte 0xffff0003
	.4byte Func_02001464
	.4byte 0x00000002
	.4byte 0xffff0004
	.4byte Func_02001464
	.4byte 0x00000002
	.4byte 0xffff0005
	.4byte Func_02001464
	.4byte 0x00004602
	.4byte 0xffff0028
	.4byte Func_02000d74
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte Func_0200033c
	.4byte 0x00008515
	.4byte 0x02000008
	.4byte 0x00000000
	.4byte 0x50008905
	.4byte 0xffff0015
	.4byte Func_02000b88
	.4byte 0x50008905
	.4byte 0xffff0016
	.4byte Func_02000ba8
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02002890
Data_02002890:
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
	.4byte 0x0802445d
	.4byte 0x021f0843
	.4byte 0x2049044e
	.4byte 0x03b92205
	.4byte 0xd700341d
	.4byte 0x106b1612
	.4byte 0x02021261
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
	.section .bss,"aw",%nobits
	.space 0x00000002
	.global Data_02002958
Data_02002958:
	.space 0x00000020
	.global Data_02002978
Data_02002978:
	.space 0x00000020
	.global Data_02002998
Data_02002998:
	.space 0x00000020
	.global Data_020029b8
Data_020029b8:
	.space 0x00000020
	.global Data_020029d8
Data_020029d8:
	.space 0x00000020
	.global Data_020029f8
Data_020029f8:
	.space 0x00000020
	.global Data_02002a18
Data_02002a18:
	.space 0x00000020
	.global Data_02002a38
Data_02002a38:
	.space 0x00000020
	.global Data_02002a58
Data_02002a58:
	.space 0x00000020
	.global Data_02002a78
Data_02002a78:
	.space 0x00000020
	.global Data_02002a98
Data_02002a98:
	.space 0x00000004
	.global Data_02002a9c
Data_02002a9c:
	.space 0x00000002
	.global Data_02002a9e
Data_02002a9e:
	.space 0x00000002
	.global Data_02002aa0
Data_02002aa0:
