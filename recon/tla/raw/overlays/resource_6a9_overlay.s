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
	bl Func_02001084
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
	bl Func_02001084
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
	bl Func_02001084
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
	bl Func_02001074
	ldr r2, .L_02008330
	mov r3, r10
	ands r3, r5
	lsls r3, r3, #2
	ldr r1, [r2, r3]
	adds r0, r6, #0
	mov r11, r3
	bl Func_0200107c
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
	bl Func_02001074
	ldr r1, [r7, #28]
	adds r0, r6, #0
	bl Func_0200107c
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
	.4byte Data_02001228
.L_02008334:
	.4byte Func_02000124
.L_02008338:
	.4byte 0xffff0000
	.section .text.x02008344,"ax",%progbits
	.global Func_02000344
	.thumb_func
Func_02000344:
	push {lr}
	ldr r3, .L_02008360
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008364
	movs r0, #0
	cmp r2, r3
	bne .L_0200835c
	ldr r0, .L_02008368
.L_0200835c:
	pop {pc}
	.2byte 0x0000
.L_02008360:
	.4byte gPartyState
.L_02008364:
	.4byte 0x00000114
.L_02008368:
	.4byte Data_02001264
	.section .text.x02008374,"ax",%progbits
	.global Func_02000374
	.thumb_func
Func_02000374:
	push {lr}
	ldr r3, .L_0200839c
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_020083a0
	cmp r2, r3
	bne .L_0200838c
	ldr r0, .L_020083a4
	b .L_02008398
.L_0200838c:
	ldr r3, .L_020083a8
	cmp r2, r3
	bne .L_02008396
	ldr r0, .L_020083ac
	b .L_02008398
.L_02008396:
	ldr r0, .L_020083b0
.L_02008398:
	pop {pc}
	.2byte 0x0000
.L_0200839c:
	.4byte gPartyState
.L_020083a0:
	.4byte 0x00000113
.L_020083a4:
	.4byte Data_02001358
.L_020083a8:
	.4byte 0x00000116
.L_020083ac:
	.4byte Data_020013b8
.L_020083b0:
	.4byte Data_02001328
	.section .text.x020083b4,"ax",%progbits
	.global Func_020003b4
	.thumb_func
Func_020003b4:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r4, [r3, #32]
	cmp r4, #0
	bne .L_020083c4
	movs r0, #0
	b .L_020083ea
.L_020083c4:
	cmp r0, #2
	bhi .L_020083d8
	lsls r3, r0, #3
	subs r3, r3, r0
	movs r0, #156
	lsls r3, r3, #3
	lsls r0, r0, #1
	adds r3, r3, r0
	ldr r4, [r4, r3]
	b .L_020083da
.L_020083d8:
	ldr r4, .L_020083ec
.L_020083da:
	lsls r3, r2, #7
	adds r3, r1, r3
	lsls r3, r3, #2
	adds r4, r4, r3
	ldrb r0, [r4, #2]
	ldrb r3, [r4, #3]
	lsls r0, r0, #8
	orrs r0, r3
.L_020083ea:
	pop {pc}
.L_020083ec:
	.4byte gMapCellBuffer
	.section .text.x020083f0,"ax",%progbits
	.global Func_020003f0
	.thumb_func
Func_020003f0:
	push {r5, lr}
	adds r5, r1, #0
	adds r1, r3, #0
	movs r3, #192
	lsls r3, r3, #18
	ldr r4, [r3, #32]
	cmp r4, #0
	bne .L_02008404
	movs r0, #0
	b .L_02008430
.L_02008404:
	cmp r0, #2
	bhi .L_02008418
	lsls r3, r0, #3
	subs r3, r3, r0
	movs r0, #156
	lsls r3, r3, #3
	lsls r0, r0, #1
	adds r3, r3, r0
	ldr r4, [r4, r3]
	b .L_0200841a
.L_02008418:
	ldr r4, .L_02008434
.L_0200841a:
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
.L_02008430:
	pop {r5, pc}
	.2byte 0x0000
.L_02008434:
	.4byte gMapCellBuffer
	.section .text.x02008438,"ax",%progbits
	.global Func_02000438
	.thumb_func
Func_02000438:
	push {r5, r6, lr}
	adds r6, r0, #0
	movs r5, #60
.L_0200843e:
	cmp r5, #0
	beq .L_02008450
	movs r0, #1
	bl WaitFrames
	ldr r3, [r6, #40]
	subs r5, #1
	cmp r3, #0
	bne .L_0200843e
.L_02008450:
	pop {r5, r6, pc}
	.2byte 0x0000
	.section .text.x02008454,"ax",%progbits
	.global Func_02000454
	.thumb_func
Func_02000454:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r3, .L_02008538
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	mov r8, r0
	ldr r0, [r3]
	bl Object_GetById
	mov r2, r8
	ldrh r3, [r2]
	movs r2, #255
	lsls r2, r2, #8
	adds r2, #255
	adds r7, r0, #0
	cmp r3, r2
	beq .L_020084f4
.L_0200847a:
	mov r3, r8
	ldrh r0, [r3]
	bl Object_GetById
	adds r5, r0, #0
	adds r2, r5, #0
	movs r3, #0
	adds r2, #85
	strb r3, [r2]
	adds r2, #4
	strb r3, [r2]
	movs r1, #0
	str r3, [r5, #68]
	bl ObjectDispatch_SetSingleChildField26
	ldr r1, [r5, #8]
	ldr r2, [r5, #16]
	movs r0, #0
	bl Map_GetTerrainHeight
	adds r6, r0, #0
	asrs r6, r6, #19
	adds r6, #2
	ldr r0, [r5, #8]
	ldr r1, [r5, #16]
	movs r2, #0
	adds r3, r6, #0
	bl Func_020010cc
	adds r3, r6, #0
	ldr r0, [r5, #8]
	ldr r1, [r5, #16]
	movs r2, #2
	bl Func_020010cc
	ldr r1, [r5, #8]
	ldr r2, [r5, #16]
	asrs r1, r1, #20
	asrs r2, r2, #20
	movs r0, #0
	bl Func_020003b4
	ldr r1, [r5, #8]
	ldr r2, [r5, #16]
	movs r3, #128
	orrs r3, r0
	str r0, [r5, #76]
	asrs r2, r2, #20
	asrs r1, r1, #20
	movs r0, #0
	bl Func_020003f0
	movs r2, #2
	add r8, r2
	mov r2, r8
	ldrh r3, [r2]
	movs r2, #255
	lsls r2, r2, #8
	adds r2, #255
	cmp r3, r2
	bne .L_0200847a
.L_020084f4:
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008532
	movs r0, #1
	bl WaitFrames
	adds r3, r7, #0
	adds r3, #34
	ldrb r0, [r3]
	ldr r1, [r7, #8]
	ldr r2, [r7, #16]
	bl Map_GetTerrainHeight
	movs r3, #0
	str r3, [r7, #40]
	movs r3, #128
	lsls r3, r3, #24
	str r3, [r7, #60]
	ldr r3, .L_02008538
	str r0, [r7, #20]
	str r0, [r7, #12]
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	movs r1, #0
	bl Object_AttachWorkTargetToObject
.L_02008532:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_02008538:
	.4byte gPartyState
	.section .text.x0200853c,"ax",%progbits
	.global Func_0200053c
	.thumb_func
Func_0200053c:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #192
	lsls r2, r2, #4
	adds r2, #188
	adds r3, r3, r2
	ldr r6, [r3]
	adds r5, r0, #0
	movs r2, #14
	ldrsh r3, [r6, r2]
	lsls r5, r5, #3
	cmp r3, #31
	bgt .L_02008568
	movs r0, #138
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020085ae
.L_02008568:
	movs r1, #142
	movs r2, #134
	lsls r1, r1, #18
	lsls r2, r2, #18
	movs r0, #2
	bl Map_GetTerrainHeight
	asrs r0, r0, #19
	adds r3, r0, r5
	movs r1, #134
	movs r0, #142
	lsls r0, r0, #18
	lsls r1, r1, #18
	movs r2, #2
	bl Func_020010cc
	movs r1, #146
	movs r2, #138
	lsls r1, r1, #18
	lsls r2, r2, #18
	movs r0, #2
	bl Map_GetTerrainHeight
	asrs r0, r0, #19
	adds r3, r0, r5
	movs r1, #138
	movs r0, #146
	lsls r0, r0, #18
	lsls r1, r1, #18
	movs r2, #2
	bl Func_020010cc
	cmp r5, #8
	bne .L_02008638
	b .L_0200862c
.L_020085ae:
	movs r2, #18
	ldrsh r3, [r6, r2]
	movs r2, #134
	lsls r2, r2, #2
	cmp r3, r2
	beq .L_020085c8
	movs r0, #138
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008644
.L_020085c8:
	movs r1, #134
	movs r2, #138
	lsls r1, r1, #18
	lsls r2, r2, #18
	movs r0, #2
	bl Map_GetTerrainHeight
	asrs r0, r0, #19
	adds r3, r0, r5
	movs r1, #138
	movs r0, #134
	lsls r0, r0, #18
	lsls r1, r1, #18
	movs r2, #2
	bl Func_020010cc
	movs r1, #138
	movs r2, #138
	lsls r1, r1, #18
	lsls r2, r2, #18
	movs r0, #2
	bl Map_GetTerrainHeight
	asrs r0, r0, #19
	adds r3, r0, r5
	movs r1, #138
	movs r0, #138
	lsls r0, r0, #18
	lsls r1, r1, #18
	movs r2, #2
	bl Func_020010cc
	movs r1, #142
	movs r2, #138
	lsls r1, r1, #18
	lsls r2, r2, #18
	movs r0, #2
	bl Map_GetTerrainHeight
	asrs r0, r0, #19
	adds r3, r0, r5
	movs r1, #138
	movs r0, #142
	lsls r0, r0, #18
	lsls r1, r1, #18
	movs r2, #2
	bl Func_020010cc
	cmp r5, #6
	bne .L_02008638
.L_0200862c:
	movs r0, #138
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
	b .L_020086a4
.L_02008638:
	movs r0, #138
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_ClearBit
	b .L_020086a4
.L_02008644:
	movs r1, #134
	movs r2, #134
	lsls r1, r1, #18
	lsls r2, r2, #18
	movs r0, #2
	bl Map_GetTerrainHeight
	asrs r0, r0, #19
	adds r3, r0, r5
	movs r1, #134
	movs r0, #134
	lsls r0, r0, #18
	lsls r1, r1, #18
	movs r2, #2
	bl Func_020010cc
	movs r1, #138
	movs r2, #134
	lsls r1, r1, #18
	lsls r2, r2, #18
	movs r0, #2
	bl Map_GetTerrainHeight
	asrs r0, r0, #19
	adds r3, r0, r5
	movs r1, #134
	movs r0, #138
	lsls r0, r0, #18
	lsls r1, r1, #18
	movs r2, #2
	bl Func_020010cc
	movs r1, #142
	movs r2, #134
	lsls r1, r1, #18
	lsls r2, r2, #18
	movs r0, #2
	bl Map_GetTerrainHeight
	asrs r0, r0, #19
	adds r3, r0, r5
	movs r1, #134
	movs r0, #142
	lsls r0, r0, #18
	lsls r1, r1, #18
	movs r2, #2
	bl Func_020010cc
.L_020086a4:
	pop {r5, r6, pc}
	.2byte 0x0000
	.section .text.x020086a8,"ax",%progbits
	.global Func_020006a8
	.thumb_func
Func_020006a8:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r7, [r3, #108]
	adds r3, #224
	ldr r2, [r3]
	movs r3, #192
	ldr r0, .L_020087e4
	lsls r3, r3, #4
	adds r3, #188
	movs r6, #0
	adds r1, r7, r3
	sub sp, #12
	mov r8, r0
	str r6, [r1]
	cmp r2, #0
	beq .L_020086f2
	movs r0, #179
	lsls r0, r0, #1
	adds r3, r7, r0
	movs r0, #0
	ldrsh r3, [r3, r0]
	cmp r3, #0
	beq .L_020086f2
	ldr r3, [r2, #20]
	str r3, [r1]
	adds r6, r3, #0
	movs r1, #28
	ldrsh r3, [r2, r1]
	cmp r3, #147
	beq .L_020087a2
	movs r0, #1
	bl Func_0200053c
	b .L_020087a2
.L_020086f2:
	ldr r3, .L_020087e8
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	ldrh r1, [r0, #6]
	movs r3, #128
	lsls r3, r3, #6
	adds r1, r1, r3
	movs r3, #192
	lsls r3, r3, #8
	ands r1, r3
	ldr r3, [r0, #8]
	mov r5, sp
	str r3, [r5]
	adds r2, r5, #0
	ldr r3, [r0, #12]
	str r3, [r5, #4]
	ldr r3, [r0, #16]
	movs r0, #128
	str r3, [r5, #8]
	lsls r0, r0, #13
	bl Vector_AddPolarOffsetFar
	mov r0, r8
	movs r1, #255
	ldrh r3, [r0]
	lsls r1, r1, #8
	adds r1, #255
	cmp r3, r1
	beq .L_0200878e
.L_02008734:
	mov r2, r8
	ldrh r0, [r2]
	bl Object_GetById
	adds r6, r0, #0
	ldr r2, [r5]
	ldr r3, [r6, #8]
	asrs r2, r2, #20
	asrs r3, r3, #20
	cmp r2, r3
	bne .L_0200877a
	ldr r3, [r5, #4]
	cmp r3, #0
	bge .L_02008758
	movs r0, #255
	lsls r0, r0, #8
	adds r0, #255
	adds r3, r3, r0
.L_02008758:
	asrs r2, r3, #16
	ldr r3, [r6, #12]
	cmp r3, #0
	bge .L_02008768
	movs r1, #255
	lsls r1, r1, #8
	adds r1, #255
	adds r3, r3, r1
.L_02008768:
	asrs r3, r3, #16
	cmp r2, r3
	bne .L_0200877a
	ldr r2, [r5, #8]
	ldr r3, [r6, #16]
	asrs r2, r2, #20
	asrs r3, r3, #20
	cmp r2, r3
	beq .L_0200878e
.L_0200877a:
	movs r2, #2
	add r8, r2
	mov r0, r8
	movs r1, #255
	ldrh r3, [r0]
	lsls r1, r1, #8
	adds r1, #255
	movs r6, #0
	cmp r3, r1
	bne .L_02008734
.L_0200878e:
	cmp r6, #0
	beq .L_020087da
	movs r2, #192
	lsls r2, r2, #4
	adds r2, #188
	adds r3, r7, r2
	str r6, [r3]
	movs r0, #1
	bl Func_0200053c
.L_020087a2:
	ldr r1, [r6, #8]
	ldr r2, [r6, #16]
	movs r0, #0
	bl Map_GetTerrainHeight
	adds r5, r0, #0
	asrs r5, r5, #19
	subs r5, #2
	ldr r0, [r6, #8]
	ldr r1, [r6, #16]
	movs r2, #0
	adds r3, r5, #0
	bl Func_020010cc
	ldr r0, [r6, #8]
	ldr r1, [r6, #16]
	movs r2, #2
	adds r3, r5, #0
	bl Func_020010cc
	ldr r1, [r6, #8]
	ldr r2, [r6, #16]
	asrs r1, r1, #20
	asrs r2, r2, #20
	ldr r3, [r6, #76]
	movs r0, #0
	bl Func_020003f0
.L_020087da:
	add sp, #12
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_020087e4:
	.4byte Data_02001220
.L_020087e8:
	.4byte gPartyState
	.section .text.x020087ec,"ax",%progbits
	.global Func_020007ec
	.thumb_func
Func_020007ec:
	push {r5, r6, lr}
	adds r5, r0, #0
	ldr r4, [r5, #68]
	ldr r3, [r5, #8]
	ldr r2, [r5, #72]
	adds r3, r3, r4
	str r3, [r5, #8]
	ldr r3, [r5, #12]
	ldr r6, [r5, #76]
	adds r3, r3, r2
	str r3, [r5, #12]
	ldr r3, [r5, #16]
	sub sp, #4
	adds r3, r3, r6
	str r3, [r5, #16]
	adds r0, r4, #0
	movs r1, #12
	str r4, [sp, #0]
	bl Engine_MathDivide
	ldr r4, [sp, #0]
	movs r1, #10
	subs r4, r4, r0
	str r4, [r5, #68]
	adds r0, r6, #0
	bl Engine_MathDivide
	ldr r3, [r5, #24]
	ldr r2, [r5, #48]
	subs r6, r6, r0
	adds r3, r3, r2
	str r3, [r5, #24]
	ldr r2, [r5, #52]
	ldr r3, [r5, #28]
	str r6, [r5, #76]
	adds r3, r3, r2
	str r3, [r5, #28]
	ldr r1, [r5, #80]
	adds r5, #100
	ldrh r3, [r1, #18]
	ldrh r2, [r5]
	add sp, #4
	adds r3, r3, r2
	strh r3, [r1, #18]
	pop {r5, r6, pc}
	.2byte 0x0000
	.section .text.x02008848,"ax",%progbits
	.global Func_02000848
	.thumb_func
Func_02000848:
	push {r5, lr}
	ldr r3, .L_02008878
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	adds r5, r0, #0
	ldr r0, [r3]
	bl Object_GetById
	ldr r3, [r5, #16]
	ldr r2, [r0, #16]
	adds r0, #35
	cmp r2, r3
	ble .L_0200886c
	ldrb r2, [r0]
	movs r3, #4
	orrs r3, r2
	b .L_02008872
.L_0200886c:
	ldrb r2, [r0]
	movs r3, #249
	ands r3, r2
.L_02008872:
	strb r3, [r0]
	pop {r5, pc}
	.2byte 0x0000
.L_02008878:
	.4byte gPartyState
	.section .text.x0200887c,"ax",%progbits
	.global Func_0200087c
	.thumb_func
Func_0200087c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r0, #192
	adds r3, #224
	ldr r1, [r3]
	lsls r0, r0, #4
	adds r0, #188
	adds r3, r2, r0
	sub sp, #68
	ldr r7, [r3]
	cmp r1, #0
	beq .L_020088c4
	movs r4, #179
	lsls r4, r4, #1
	adds r3, r2, r4
	movs r0, #0
	ldrsh r3, [r3, r0]
	cmp r3, #0
	beq .L_020088c4
	movs r2, #28
	ldrsh r3, [r1, r2]
	cmp r3, #147
	beq .L_020088cc
	movs r0, #1
	negs r0, r0
	bl Func_0200053c
	b .L_020088cc
.L_020088c4:
	movs r0, #1
	negs r0, r0
	bl Func_0200053c
.L_020088cc:
	ldr r1, [r7, #8]
	ldr r2, [r7, #16]
	movs r0, #0
	bl Map_GetTerrainHeight
	ldr r3, [r7, #12]
	cmp r0, r3
	bne .L_020088de
	b .L_02008a60
.L_020088de:
	movs r3, #34
	adds r3, r3, r7
	movs r5, #2
	movs r0, #85
	strb r5, [r3]
	movs r4, #0
	adds r0, r0, r7
	mov r11, r3
	movs r3, #3
	strb r3, [r0]
	mov r9, r0
	str r4, [r7, #68]
	movs r0, #1
	mov r8, r4
	bl WaitFrames
	ldr r1, [r7, #8]
	ldr r2, [r7, #16]
	movs r0, #2
	bl Func_020010a4
	cmp r0, #0
	beq .L_0200897a
	adds r1, r7, #0
	adds r1, #35
	ldrb r3, [r1]
	movs r2, #2
	orrs r3, r2
	strb r3, [r1]
	movs r2, #128
	ldr r3, .L_02008958
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	movs r6, #128
	ldr r3, .L_0200895c
	ldr r2, [r7, #16]
	lsls r6, r6, #19
	adds r4, r0, #2
	adds r6, #80
	strh r3, [r6]
	lsls r3, r4, #20
	adds r2, r2, r3
	movs r3, #156
	lsls r3, r3, #1
	movs r1, #0
	ldr r0, [r7, #8]
	bl Func_02000080
	adds r5, r0, #0
	movs r0, #204
	bl Func_02001164
	adds r0, r7, #0
	bl Func_02000438
	adds r0, r7, #0
	bl Func_02000438
	b .L_02008960
	.2byte 0x0000
.L_02008958:
	.4byte 0x00001000
.L_0200895c:
	.4byte 0x00003f10
.L_02008960:
	mov r2, r8
	str r2, [r7, #16]
	str r2, [r7, #12]
	str r2, [r7, #8]
	adds r0, r5, #0
	bl Func_0200108c
	movs r0, #1
	bl WaitFrames
	mov r3, r8
	strh r3, [r6]
	b .L_02008a1e
.L_0200897a:
	adds r0, r7, #0
	bl Func_02000438
	adds r0, r7, #0
	bl Func_02000438
	movs r0, #188
	bl Func_02001164
	add r4, sp, #16
	movs r3, #7
	str r3, [r4, #4]
	ldr r3, .L_02008ab8
	str r5, [r4]
	str r3, [r4, #36]
	movs r3, #204
	lsls r3, r3, #8
	adds r3, #204
	str r3, [r4, #8]
	str r3, [r4, #12]
	movs r0, #0
	mov r8, r4
	mov r10, r0
.L_020089a8:
	mov r2, r10
	lsls r5, r2, #12
	adds r0, r5, #0
	bl Math_Cosine
	movs r3, #0
	add r6, sp, #56
	lsls r0, r0, #1
	str r0, [r6]
	str r3, [r6, #4]
	adds r0, r5, #0
	bl Math_Sine
	ldr r3, [r6]
	lsls r0, r0, #1
	asrs r2, r3, #1
	adds r3, r3, r2
	str r3, [r6]
	str r0, [r6, #8]
	bl Random16Far
	lsls r3, r0, #1
	ldr r2, [r6]
	adds r3, r3, r0
	lsls r3, r3, #14
	lsrs r3, r3, #16
	adds r2, r2, r3
	ldr r3, .L_02008abc
	adds r2, r2, r3
	str r2, [r6]
	bl Random16Far
	lsls r3, r0, #1
	ldr r5, [r6, #8]
	adds r3, r3, r0
	ldr r4, .L_02008ac0
	lsls r3, r3, #13
	lsrs r3, r3, #16
	adds r5, r5, r3
	adds r5, r5, r4
	ldr r4, [r6, #4]
	str r5, [r6, #8]
	ldr r0, [r7, #8]
	ldr r2, [r7, #16]
	ldr r1, [r7, #12]
	ldr r3, [r6]
	str r4, [sp, #0]
	ldr r4, .L_02008ac4
	str r5, [sp, #4]
	str r4, [sp, #8]
	mov r4, r8
	str r4, [sp, #12]
	bl Func_0200015c
	movs r0, #1
	add r10, r0
	mov r2, r10
	cmp r2, #16
	bls .L_020089a8
.L_02008a1e:
	movs r3, #0
	mov r0, r11
	mov r4, r9
	strb r3, [r4]
	strb r3, [r0]
	ldr r3, [r7, #8]
	add r0, sp, #56
	str r3, [r0]
	ldr r3, [r7, #12]
	ldr r2, .L_02008ac8
	movs r1, #1
	adds r3, r3, r2
	str r3, [r0, #4]
	ldr r3, [r7, #16]
	str r3, [r0, #8]
	bl Func_0200115c
	cmp r0, #0
	beq .L_02008a60
	ldr r3, .L_02008acc
	movs r2, #1
	str r3, [r7, #108]
	adds r3, r7, #0
	adds r3, #98
	strb r2, [r3]
	adds r3, r0, #0
	movs r0, #128
	adds r3, #98
	lsls r0, r0, #2
	strb r2, [r3]
	adds r0, #18
	bl GameFlag_SetBit
.L_02008a60:
	ldr r1, [r7, #8]
	ldr r2, [r7, #16]
	movs r0, #0
	bl Map_GetTerrainHeight
	adds r5, r0, #0
	asrs r5, r5, #19
	adds r5, #2
	ldr r0, [r7, #8]
	ldr r1, [r7, #16]
	adds r3, r5, #0
	movs r2, #0
	bl Func_020010cc
	adds r3, r5, #0
	ldr r0, [r7, #8]
	ldr r1, [r7, #16]
	movs r2, #2
	bl Func_020010cc
	ldr r1, [r7, #8]
	ldr r2, [r7, #16]
	asrs r1, r1, #20
	asrs r2, r2, #20
	movs r0, #0
	bl Func_020003b4
	ldr r1, [r7, #8]
	ldr r2, [r7, #16]
	movs r3, #128
	str r0, [r7, #76]
	orrs r3, r0
	asrs r1, r1, #20
	asrs r2, r2, #20
	movs r0, #0
	bl Func_020003f0
	add sp, #68
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_02008ab8:
	.4byte Func_020007ec
.L_02008abc:
	.4byte 0xffffa000
.L_02008ac0:
	.4byte 0xffffd000
.L_02008ac4:
	.4byte 0x010b0000
.L_02008ac8:
	.4byte 0xfff00000
.L_02008acc:
	.4byte Func_02000848
	.section .text.x02008ad0,"ax",%progbits
	.global Func_02000ad0
	.thumb_func
Func_02000ad0:
	push {lr}
	ldr r3, .L_02008b00
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008b04
	cmp r2, r3
	bne .L_02008ae8
	ldr r0, .L_02008b08
	b .L_02008afe
.L_02008ae8:
	ldr r3, .L_02008b0c
	cmp r2, r3
	bne .L_02008af2
	ldr r0, .L_02008b10
	b .L_02008afe
.L_02008af2:
	ldr r3, .L_02008b14
	cmp r2, r3
	bne .L_02008afc
	ldr r0, .L_02008b18
	b .L_02008afe
.L_02008afc:
	ldr r0, .L_02008b1c
.L_02008afe:
	pop {pc}
.L_02008b00:
	.4byte gPartyState
.L_02008b04:
	.4byte 0x00000113
.L_02008b08:
	.4byte Data_0200149c
.L_02008b0c:
	.4byte 0x00000115
.L_02008b10:
	.4byte Data_020014d8
.L_02008b14:
	.4byte 0x00000116
.L_02008b18:
	.4byte Data_020014fc
.L_02008b1c:
	.4byte Data_02001430
	.section .text.x02008b20,"ax",%progbits
	.global Func_02000b20
	.thumb_func
Func_02000b20:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #88
	str r2, [r3]
	ldr r3, .L_02008b58
	subs r2, #36
	adds r5, r3, r2
	movs r3, #0
	ldrsh r2, [r5, r3]
	ldr r3, .L_02008b5c
	cmp r2, r3
	bne .L_02008b46
	bl Func_02000ba8
.L_02008b46:
	movs r3, #0
	ldrsh r2, [r5, r3]
	ldr r3, .L_02008b60
	cmp r2, r3
	bne .L_02008b54
	bl Func_02000f98
.L_02008b54:
	movs r0, #0
	pop {r5, pc}
.L_02008b58:
	.4byte gPartyState
.L_02008b5c:
	.4byte 0x00000113
.L_02008b60:
	.4byte 0x00000116
	.section .text.x02008b64,"ax",%progbits
	.global Func_02000b64
	.thumb_func
Func_02000b64:
	push {lr}
	ldr r3, .L_02008b98
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008b9c
	cmp r2, r3
	bne .L_02008b7e
	bl Func_02000d10
	b .L_02008b94
.L_02008b7e:
	ldr r3, .L_02008ba0
	cmp r2, r3
	bne .L_02008b8a
	bl Func_02000ea0
	b .L_02008b94
.L_02008b8a:
	ldr r3, .L_02008ba4
	cmp r2, r3
	bne .L_02008b94
	bl Func_02000f50
.L_02008b94:
	movs r0, #0
	pop {pc}
.L_02008b98:
	.4byte gPartyState
.L_02008b9c:
	.4byte 0x00000114
.L_02008ba0:
	.4byte 0x00000115
.L_02008ba4:
	.4byte 0x00000116
	.section .text.x02008ba8,"ax",%progbits
	.global Func_02000ba8
	.thumb_func
Func_02000ba8:
	push {r5, lr}
	movs r0, #4
	sub sp, #8
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #32
	orrs r3, r2
	strb r3, [r0]
	bl Func_0200114c
	movs r1, #128
	lsls r1, r1, #2
	movs r2, #8
	movs r3, #9
	movs r0, #0
	bl Func_02001154
	movs r0, #10
	bl Object_GetById
	adds r3, r0, #0
	movs r5, #0
	adds r3, #85
	strb r5, [r3]
	movs r2, #10
	ldr r3, [r0, #8]
	movs r1, #10
	asrs r3, r3, #20
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #16
	movs r2, #1
	movs r3, #1
	bl Func_020010ac
	add sp, #8
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x02008bf8,"ax",%progbits
	.global Func_02000bf8
	.thumb_func
Func_02000bf8:
	push {r5, r6, lr}
	adds r0, r1, #0
	sub sp, #8
	bl Object_GetById
	adds r6, r0, #0
	adds r2, r6, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	movs r3, #13
	str r3, [sp, #0]
	movs r5, #10
	movs r0, #13
	movs r1, #12
	movs r2, #2
	movs r3, #1
	str r5, [sp, #4]
	bl Func_020010ac
	ldr r3, [r6, #8]
	movs r0, #16
	asrs r3, r3, #20
	str r3, [sp, #0]
	movs r1, #10
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl Func_020010ac
	add sp, #8
	pop {r5, r6, pc}
	.section .text.x02008c38,"ax",%progbits
	.global Func_02000c38
	.thumb_func
Func_02000c38:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r5, r0, #0
	adds r7, r5, #0
	adds r7, #85
	ldrb r0, [r7]
	adds r6, r1, #0
	mov r8, r0
	adds r0, r5, #0
	bl Func_020010b4
	cmp r0, #0
	bne .L_02008cce
	bl Func_020010dc
	movs r0, #0
	bl Func_0200113c
	movs r1, #6
	adds r0, r5, #0
	bl Func_02001074
	movs r0, #6
	bl WaitFrames
	movs r0, #152
	bl Func_02001164
	adds r0, r5, #0
	movs r1, #7
	bl Func_02001074
	movs r3, #192
	lsls r3, r3, #10
	str r3, [r5, #48]
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r5, #52]
	movs r3, #128
	lsls r3, r3, #11
	str r3, [r5, #40]
	ldrb r2, [r7]
	movs r3, #126
	ands r3, r2
	adds r0, r5, #0
	strb r3, [r7]
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	ldr r3, .L_02008cd8
	movs r2, #2
	ldrsh r1, [r6, r2]
	movs r0, #10
	ldrsh r2, [r6, r0]
	movs r0, #133
	lsls r0, r0, #2
	adds r3, r3, r0
	ldr r0, [r3]
	bl ObjectMotion_SetPositionAndCommit
	adds r0, r5, #0
	movs r1, #6
	bl Func_02001074
	adds r0, r5, #0
	movs r1, #1
	bl ObjectDispatch_SetSingleChildField26
	mov r2, r8
	strb r2, [r7]
	bl Func_020010e4
	movs r0, #1
	b .L_02008cd0
.L_02008cce:
	movs r0, #0
.L_02008cd0:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008cd8:
	.4byte gPartyState
	.section .text.x02008cdc,"ax",%progbits
	.global Func_02000cdc
	.thumb_func
Func_02000cdc:
	push {lr}
	ldr r3, .L_02008d0c
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	sub sp, #12
	bl Object_GetById
	ldr r3, [r0, #8]
	movs r2, #128
	lsls r2, r2, #14
	mov r1, sp
	adds r3, r3, r2
	str r3, [r1]
	ldr r3, [r0, #12]
	str r3, [r1, #4]
	ldr r3, [r0, #16]
	str r3, [r1, #8]
	bl Func_02000c38
	add sp, #12
	pop {pc}
	.2byte 0x0000
.L_02008d0c:
	.4byte gPartyState
	.section .text.x02008d10,"ax",%progbits
	.global Func_02000d10
	.thumb_func
Func_02000d10:
	push {lr}
	movs r0, #139
	lsls r0, r0, #4
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008d24
	bl Func_02000d28
.L_02008d24:
	pop {pc}
	.2byte 0x0000
	.section .text.x02008d28,"ax",%progbits
	.global Func_02000d28
	.thumb_func
Func_02000d28:
	push {lr}
	sub sp, #8
	movs r3, #3
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #35
	movs r1, #12
	movs r2, #18
	movs r3, #12
	bl Func_02001094
	movs r3, #18
	movs r2, #12
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #35
	movs r1, #12
	movs r2, #3
	movs r3, #3
	bl Func_020010ac
	movs r3, #1
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #84
	movs r1, #13
	movs r2, #83
	movs r3, #13
	bl Func_02001094
	add sp, #8
	pop {pc}
	.section .text.x02008d68,"ax",%progbits
	.global Func_02000d68
	.thumb_func
Func_02000d68:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #170
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r5, [r3, r2]
	sub sp, #8
	cmp r5, #2
	bne .L_02008d90
	movs r1, #144
	movs r2, #224
	movs r0, #8
	lsls r1, r1, #16
	lsls r2, r2, #16
	bl Func_0200110c
	b .L_02008d9e
.L_02008d90:
	movs r1, #224
	movs r2, #224
	movs r0, #8
	lsls r1, r1, #16
	lsls r2, r2, #16
	bl Func_0200110c
.L_02008d9e:
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #4
	bl ObjectMotion_ArmCallback
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #4
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	strb r3, [r0]
	movs r2, #1
	movs r3, #2
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #14
	movs r0, #96
	movs r1, #14
	movs r2, #72
	bl Func_02001094
	movs r2, #4
	movs r1, #0
	movs r0, #4
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r0, #4
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #4
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r2, #16
	movs r1, #0
	movs r0, #4
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r0, #4
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r1, #13
	movs r0, #4
	bl Object_SetModeById
	movs r0, #123
	bl Func_02001164
	movs r0, #14
	bl Battle_WaitMode0
	adds r0, r5, #0
	bl Func_02001134
	add sp, #8
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x02008e20,"ax",%progbits
	.global Func_02000e20
	.thumb_func
Func_02000e20:
	push {lr}
	movs r2, #14
	movs r0, #4
	movs r1, #0
	negs r2, r2
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #16
	movs r2, #0
	movs r0, #4
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #10
	bl Battle_WaitMode0
	bl Func_02000d68
	pop {pc}
	.section .text.x02008e44,"ax",%progbits
	.global Func_02000e44
	.thumb_func
Func_02000e44:
	push {lr}
	movs r2, #14
	movs r0, #4
	movs r1, #0
	negs r2, r2
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #16
	negs r1, r1
	movs r2, #0
	movs r0, #4
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #10
	bl Battle_WaitMode0
	bl Func_02000d68
	pop {pc}
	.2byte 0x0000
	.section .text.x02008e6c,"ax",%progbits
	.global Func_02000e6c
	.thumb_func
Func_02000e6c:
	push {lr}
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #4
	bl ObjectMotion_ArmCallback
	movs r0, #10
	bl Battle_WaitMode0
	bl Func_02000d68
	pop {pc}
	.2byte 0x0000
	.section .text.x02008e88,"ax",%progbits
	.global Func_02000e88
	.thumb_func
Func_02000e88:
	push {lr}
	cmp r0, #1
	bne .L_02008e9c
	bl Func_02000d28
	movs r0, #139
	lsls r0, r0, #4
	adds r0, #255
	bl GameFlag_SetBit
.L_02008e9c:
	pop {pc}
	.2byte 0x0000
	.section .text.x02008ea0,"ax",%progbits
	.global Func_02000ea0
	.thumb_func
Func_02000ea0:
	push {lr}
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #23
	sub sp, #8
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008eee
	movs r3, #48
	movs r2, #47
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #31
	movs r1, #43
	movs r2, #9
	movs r3, #12
	bl Func_020010ac
	movs r3, #9
	movs r2, #13
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #30
	movs r1, #110
	movs r2, #48
	movs r3, #110
	bl Func_02001094
	movs r3, #52
	movs r2, #111
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #52
	movs r1, #110
	movs r2, #1
	movs r3, #1
	bl Func_020010ac
.L_02008eee:
	add sp, #8
	pop {pc}
	.2byte 0x0000
	.section .text.x02008ef4,"ax",%progbits
	.global Func_02000ef4
	.thumb_func
Func_02000ef4:
	push {lr}
	sub sp, #8
	bl Func_020010dc
	movs r0, #0
	bl Func_0200113c
	movs r3, #48
	movs r2, #47
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #31
	movs r1, #43
	movs r2, #9
	movs r3, #11
	bl Func_020010ac
	movs r3, #9
	movs r2, #13
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #30
	movs r1, #110
	movs r2, #48
	movs r3, #110
	bl Func_02001094
	movs r3, #52
	movs r2, #111
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #110
	movs r2, #1
	movs r3, #1
	movs r0, #52
	bl Func_020010ac
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #23
	bl GameFlag_SetBit
	bl Func_020010e4
	add sp, #8
	pop {pc}
	.section .text.x02008f50,"ax",%progbits
	.global Func_02000f50
	.thumb_func
Func_02000f50:
	push {r5, lr}
	ldr r3, .L_02008f94
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	ldrh r3, [r3]
	movs r2, #128
	subs r3, #5
	lsls r3, r3, #16
	lsls r2, r2, #9
	sub sp, #8
	cmp r3, r2
	bhi .L_02008f90
	movs r3, #28
	movs r5, #5
	str r3, [sp, #0]
	movs r0, #28
	movs r1, #0
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl Func_020010ac
	movs r3, #36
	str r3, [sp, #0]
	movs r0, #36
	movs r1, #0
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl Func_020010ac
.L_02008f90:
	add sp, #8
	pop {r5, pc}
.L_02008f94:
	.4byte gPartyState
	.section .text.x02008f98,"ax",%progbits
	.global Func_02000f98
	.thumb_func
Func_02000f98:
	push {r5, lr}
	ldr r0, .L_02008fe0
	bl Func_02000454
	movs r0, #8
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r5, #32
	orrs r3, r5
	strb r3, [r0]
	movs r0, #9
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	orrs r5, r3
	strb r5, [r0]
	movs r0, #10
	bl Object_GetById
	adds r0, #89
	ldrb r3, [r0]
	movs r5, #8
	orrs r3, r5
	strb r3, [r0]
	movs r0, #11
	bl Object_GetById
	adds r0, #89
	ldrb r3, [r0]
	orrs r5, r3
	strb r5, [r0]
	pop {r5, pc}
	.2byte 0x0000
.L_02008fe0:
	.4byte Data_02001220
	.section .text.x02008fe4,"ax",%progbits
	.global Func_02000fe4
	.thumb_func
Func_02000fe4:
	push {lr}
	sub sp, #8
	bl Func_020010dc
	movs r0, #0
	bl Func_0200113c
	movs r0, #158
	bl Func_02001164
	movs r0, #10
	bl Battle_WaitMode0
	movs r3, #1
	movs r2, #2
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #94
	movs r2, #6
	movs r3, #94
	movs r0, #7
	bl Func_02001094
	movs r0, #16
	bl Battle_WaitMode0
	movs r0, #123
	bl Func_02001164
	movs r0, #8
	bl Func_02001134
	bl Func_020010e4
	add sp, #8
	pop {pc}
	.section .rodata.x0200916c,"a",%progbits
.L_0200916c:
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
.L_020091a8:
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
.L_020091e4:
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
	.global Data_02001220
Data_02001220:
	.4byte 0x00090008
	.4byte 0x0000ffff
	.global Data_02001228
Data_02001228:
	.4byte .L_0200916c
	.4byte .L_020091a8
	.4byte .L_020091e4
	.global gSceneEntrances
gSceneEntrances:
	.4byte 0xffff0000
	.4byte 0x000001d3
	.4byte 0x40000099
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02001264
Data_02001264:
	.4byte 0x001c0084
	.4byte 0x008c02b4
	.4byte 0x02bc0024
	.4byte 0x0004ffff
	.4byte 0x001c0124
	.4byte 0x012c02b4
	.4byte 0x02bc0024
	.4byte 0x0005ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gSceneExits
gSceneExits:
	.4byte 0x00000114
	.4byte 0x10152002
	.4byte 0xffffffff
	.4byte 0x10204114
	.4byte 0xffffffff
	.4byte 0x10305114
	.4byte 0xffffffff
	.4byte 0x10402114
	.4byte 0xffffffff
	.4byte 0x10503114
	.4byte 0xffffffff
	.4byte 0x00000113
	.4byte 0x10153002
	.4byte 0xffffffff
	.4byte 0x00000115
	.4byte 0x10558002
	.4byte 0xffffffff
	.4byte 0x00000116
	.4byte 0x10159002
	.4byte 0xffffffff
	.4byte 0x10203116
	.4byte 0xffffffff
	.4byte 0x10302116
	.4byte 0xffffffff
	.4byte 0x10405116
	.4byte 0xffffffff
	.4byte 0x10504116
	.4byte 0xffffffff
	.4byte 0x10607116
	.4byte 0xffffffff
	.4byte 0x10706116
	.4byte 0xffffffff
	.4byte 0x10809116
	.4byte 0xffffffff
	.4byte 0x10908116
	.4byte 0xffffffff
	.4byte 0x000001ff
	.global Data_02001328
Data_02001328:
	.4byte 0xffff01e9
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
	.global Data_02001358
Data_02001358:
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x0002c000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x0002c000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x0002c000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020013b8
Data_020013b8:
	.4byte 0xffff013b
	.4byte 0x00000001
	.4byte 0x02180000
	.4byte 0x00000000
	.4byte 0x02280000
	.4byte 0x0002c000
	.4byte 0xffff013b
	.4byte 0x00000001
	.4byte 0x02580000
	.4byte 0x00000000
	.4byte 0x02180000
	.4byte 0x0002c000
	.4byte 0xffff0182
	.4byte 0x00000001
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x02000000
	.4byte 0x0002c000
	.4byte 0xffff0182
	.4byte 0x00000001
	.4byte 0x02080000
	.4byte 0x00000000
	.4byte 0x02000000
	.4byte 0x0002c000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02001430
Data_02001430:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00004602
	.4byte 0xffff0002
	.4byte Func_02000e6c
	.4byte 0x00008602
	.4byte 0xffff0002
	.4byte Func_02000e44
	.4byte 0x00000602
	.4byte 0xffff0002
	.4byte Func_02000e20
	.4byte 0x00004602
	.4byte 0xffff0003
	.4byte Func_02000e6c
	.4byte 0x00008602
	.4byte 0xffff0003
	.4byte Func_02000e44
	.4byte 0x00000602
	.4byte 0xffff0003
	.4byte Func_02000e20
	.4byte 0x50008805
	.4byte 0x09af000a
	.4byte Func_02000e88
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_0200149c
Data_0200149c:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000602
	.4byte 0xffff000a
	.4byte Func_02000cdc
	.4byte 0x00008515
	.4byte 0x02000008
	.4byte 0x00000000
	.4byte 0x00008c15
	.4byte 0xffff000a
	.4byte Func_02000bf8
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020014d8
Data_020014d8:
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x50008a05
	.4byte 0xffff003c
	.4byte Func_02000ef4
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020014fc
Data_020014fc:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000031
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000021
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000031
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000021
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000031
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000021
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x0000c602
	.4byte 0xffff0008
	.4byte Func_02000fe4
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000008
	.4byte 0x02120000
	.4byte Func_020006a8
	.4byte 0x00000009
	.4byte 0x02120000
	.4byte Func_0200087c
	.4byte 0x10008c15
	.4byte 0x02120008
	.4byte Func_020006a8
	.4byte 0x00008c15
	.4byte 0x02120008
	.4byte Func_0200087c
	.4byte 0x10008c15
	.4byte 0x02120009
	.4byte Func_020006a8
	.4byte 0x00008c15
	.4byte 0x02120009
	.4byte Func_0200087c
	.4byte 0x10009315
	.4byte 0x02120008
	.4byte Func_020006a8
	.4byte 0x00009315
	.4byte 0x02120008
	.4byte Func_0200087c
	.4byte 0x10009315
	.4byte 0x02120009
	.4byte Func_020006a8
	.4byte 0x00009315
	.4byte 0x02120009
	.4byte Func_0200087c
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
