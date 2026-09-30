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
	bl Func_02001ff8
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
	bl Func_02001fe0
	ldr r2, .L_02008274
	mov r3, r8
	ands r3, r5
	lsls r3, r3, #2
	ldr r1, [r2, r3]
	adds r0, r6, #0
	mov r10, r3
	bl Func_02001ff0
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
	bl Engine_MathDivide
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, [r6, #28]
	ldr r1, [r5, #12]
	subs r0, r0, r3
	b .L_0200820c
.L_020081fa:
	ldr r2, .L_0200827c
	adds r0, r3, r2
	bl Engine_MathDivide
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, .L_0200827c
	ldr r1, [r5, #12]
	adds r0, r0, r3
.L_0200820c:
	bl Engine_MathDivide
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
	bl Func_02001fe0
	ldr r1, [r7, #28]
	adds r0, r6, #0
	bl Func_02001ff0
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
	.4byte Data_020022d8
.L_02008278:
	.4byte Func_02000080
.L_0200827c:
	.4byte 0xffff0000
	.section .text.x02008280,"ax",%progbits
	.global Func_02000280
	.thumb_func
Func_02000280:
	push {lr}
	movs r0, #10
	movs r1, #1
	movs r2, #14
	bl Func_02002158
	pop {pc}
	.2byte 0x0000
	.section .text.x02008290,"ax",%progbits
	.global Func_02000290
	.thumb_func
Func_02000290:
	push {lr}
	movs r0, #11
	movs r1, #2
	movs r2, #14
	bl Func_02002158
	pop {pc}
	.2byte 0x0000
	.section .text.x020082a0,"ax",%progbits
	.global Func_020002a0
	.thumb_func
Func_020002a0:
	push {lr}
	movs r0, #23
	movs r1, #3
	movs r2, #19
	bl Func_02002158
	pop {pc}
	.2byte 0x0000
	.section .text.x020082b0,"ax",%progbits
	.global Func_020002b0
	.thumb_func
Func_020002b0:
	push {lr}
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #0
	pop {pc}
	.section .text.x020082bc,"ax",%progbits
	.global Func_020002bc
	.thumb_func
Func_020002bc:
	push {r5, r6, lr}
	adds r2, r0, #0
	adds r5, r2, #0
	adds r5, #98
	ldrb r3, [r5]
	movs r0, #63
	adds r1, r2, #0
	ands r0, r3
	adds r1, #85
	movs r3, #3
	ldr r6, [r2, #80]
	strb r3, [r1]
	cmp r0, #0
	bne .L_020082de
	movs r3, #192
	lsls r3, r3, #10
	str r3, [r2, #40]
.L_020082de:
	cmp r0, #16
	bne .L_020082e8
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r2, #40]
.L_020082e8:
	cmp r0, #24
	bne .L_020082f2
	movs r3, #192
	lsls r3, r3, #10
	str r3, [r2, #40]
.L_020082f2:
	lsls r0, r0, #12
	bl Math_Cosine
	cmp r0, #0
	bge .L_020082fe
	adds r0, #63
.L_020082fe:
	asrs r3, r0, #6
	strh r3, [r6, #18]
	ldrb r3, [r5]
	movs r0, #1
	adds r3, #1
	strb r3, [r5]
	negs r0, r0
	pop {r5, r6, pc}
	.2byte 0x0000
	.section .text.x02008318,"ax",%progbits
	.global Func_02000318
	.thumb_func
Func_02000318:
	push {lr}
	ldr r3, .L_0200833c
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008340
	cmp r2, r3
	bne .L_02008330
	ldr r0, .L_02008344
	b .L_0200833a
.L_02008330:
	ldr r3, .L_02008348
	movs r0, #0
	cmp r2, r3
	bne .L_0200833a
	ldr r0, .L_0200834c
.L_0200833a:
	pop {pc}
.L_0200833c:
	.4byte gPartyState
.L_02008340:
	.4byte 0x000000d9
.L_02008344:
	.4byte Data_02002380
.L_02008348:
	.4byte 0x000000d7
.L_0200834c:
	.4byte Data_020023a0
	.section .text.x02008358,"ax",%progbits
	.global Func_02000358
	.thumb_func
Func_02000358:
	push {lr}
	ldr r3, .L_020083bc
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_020083c0
	cmp r2, r3
	bne .L_02008370
	ldr r0, .L_020083c4
	b .L_020083b8
.L_02008370:
	ldr r3, .L_020083c8
	cmp r2, r3
	bne .L_0200837a
	ldr r0, .L_020083cc
	b .L_020083b8
.L_0200837a:
	ldr r3, .L_020083d0
	cmp r2, r3
	bne .L_02008384
	ldr r0, .L_020083d4
	b .L_020083b8
.L_02008384:
	ldr r3, .L_020083d8
	cmp r2, r3
	bne .L_0200838e
	ldr r0, .L_020083dc
	b .L_020083b8
.L_0200838e:
	ldr r3, .L_020083e0
	cmp r2, r3
	bne .L_02008398
	ldr r0, .L_020083e4
	b .L_020083b8
.L_02008398:
	ldr r3, .L_020083e8
	cmp r2, r3
	bne .L_020083a2
	ldr r0, .L_020083ec
	b .L_020083b8
.L_020083a2:
	ldr r3, .L_020083f0
	cmp r2, r3
	bne .L_020083ac
	ldr r0, .L_020083f4
	b .L_020083b8
.L_020083ac:
	ldr r3, .L_020083f8
	cmp r2, r3
	bne .L_020083b6
	ldr r0, .L_020083fc
	b .L_020083b8
.L_020083b6:
	ldr r0, .L_02008400
.L_020083b8:
	pop {pc}
	.2byte 0x0000
.L_020083bc:
	.4byte gPartyState
.L_020083c0:
	.4byte 0x000000d5
.L_020083c4:
	.4byte Data_020024dc
.L_020083c8:
	.4byte 0x000000d6
.L_020083cc:
	.4byte Data_02002524
.L_020083d0:
	.4byte 0x000000d8
.L_020083d4:
	.4byte Data_02002554
.L_020083d8:
	.4byte 0x000000d9
.L_020083dc:
	.4byte Data_020025cc
.L_020083e0:
	.4byte 0x000000db
.L_020083e4:
	.4byte Data_020027dc
.L_020083e8:
	.4byte 0x000000dc
.L_020083ec:
	.4byte Data_02002854
.L_020083f0:
	.4byte 0x000000dd
.L_020083f4:
	.4byte Data_02002614
.L_020083f8:
	.4byte 0x000000de
.L_020083fc:
	.4byte Data_020027ac
.L_02008400:
	.4byte Data_020024ac
	.section .text.x02008404,"ax",%progbits
	.global Func_02000404
	.thumb_func
Func_02000404:
	push {lr}
	bl Func_02002068
	movs r0, #0
	bl Func_02002150
	movs r2, #0
	movs r1, #0
	movs r0, #11
	bl Func_020020c0
	movs r0, #240
	lsls r0, r0, #4
	adds r0, #93
	bl GameFlag_SetBit
	movs r0, #193
	movs r1, #3
	bl Func_02002130
	movs r1, #0
	movs r0, #193
	bl PartyInventory_GiveItem
	bl Func_02002070
	pop {pc}
	.2byte 0x0000
	.section .text.x0200843c,"ax",%progbits
	.global Func_0200043c
	.thumb_func
Func_0200043c:
	push {r5, r6, lr}
	ldr r5, .L_02008484
	adds r6, r0, #0
	adds r0, r5, #0
	bl Func_020020e0
	movs r1, #0
	adds r0, r6, #0
	bl UiText_OpenMessageAtObject
	bl Func_020021a8
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_0200846c
	movs r0, #10
	bl Battle_WaitMode0
	adds r0, r5, #1
	bl Func_020020e0
	b .L_02008478
.L_0200846c:
	movs r0, #20
	bl Battle_WaitMode0
	adds r0, r5, #2
	bl Func_020020e0
.L_02008478:
	adds r0, r6, #0
	movs r1, #0
	bl Func_020020f0
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02008484:
	.4byte 0x000028ae
	.section .text.x02008488,"ax",%progbits
	.global Func_02000488
	.thumb_func
Func_02000488:
	push {lr}
	sub sp, #12
	movs r3, #5
	movs r2, #43
	movs r1, #1
	str r3, [sp, #0]
	str r2, [sp, #4]
	str r1, [sp, #8]
	movs r0, #5
	movs r1, #70
	movs r2, #9
	movs r3, #7
	bl Func_020021a0
	add sp, #12
	pop {pc}
	.section .text.x020084a8,"ax",%progbits
	.global Func_020004a8
	.thumb_func
Func_020004a8:
	push {r5, r6, lr}
	ldr r3, .L_02008528
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r3, r2
	ldr r0, [r5]
	sub sp, #12
	bl Object_GetById
	ldr r3, [r0, #8]
	ldr r2, [r0, #16]
	asrs r3, r3, #20
	asrs r2, r2, #20
	cmp r3, #5
	bne .L_020084e0
	cmp r2, #17
	bne .L_020084e0
	ldr r0, [r5]
	movs r1, #2
	movs r2, #0
	bl ObjectMotion_Launch
	movs r2, #148
	ldr r0, [r5]
	movs r1, #88
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndCommit
.L_020084e0:
	movs r3, #43
	str r3, [sp, #4]
	movs r5, #1
	movs r6, #5
	movs r0, #5
	movs r1, #70
	movs r2, #9
	movs r3, #7
	str r6, [sp, #0]
	str r5, [sp, #8]
	bl Func_020021a0
	movs r0, #4
	movs r1, #25
	movs r2, #5
	movs r3, #17
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl Func_02002018
	movs r3, #17
	str r3, [sp, #4]
	movs r1, #25
	movs r2, #1
	movs r3, #1
	movs r0, #4
	str r6, [sp, #0]
	bl Func_02002030
	movs r0, #166
	lsls r0, r0, #4
	bl GameFlag_SetBit
	add sp, #12
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02008528:
	.4byte gPartyState
	.section .text.x0200852c,"ax",%progbits
	.global Func_0200052c
	.thumb_func
Func_0200052c:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #170
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r1, #192
	movs r2, #0
	ldrsh r5, [r3, r2]
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #4
	sub sp, #8
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
	bl Func_02002018
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
	movs r0, #4
	movs r1, #13
	bl Object_SetModeById
	movs r2, #16
	movs r1, #0
	movs r0, #4
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r0, #4
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r1, #10
	movs r0, #4
	bl Object_SetModeById
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #123
	bl Func_020021b0
	adds r0, r5, #0
	bl Func_02002110
	bl Func_02002070
	add sp, #8
	pop {r5, pc}
	.section .text.x020085cc,"ax",%progbits
	.global Func_020005cc
	.thumb_func
Func_020005cc:
	push {lr}
	bl Func_02002068
	movs r0, #0
	bl Func_02002150
	movs r2, #14
	movs r0, #4
	movs r1, #0
	negs r2, r2
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #4
	movs r1, #16
	movs r2, #0
	bl ObjectMotion_CommitPositionAndActivate
	bl Func_0200052c
	pop {pc}
	.section .text.x020085f4,"ax",%progbits
	.global Func_020005f4
	.thumb_func
Func_020005f4:
	push {lr}
	bl Func_02002068
	movs r0, #0
	bl Func_02002150
	movs r1, #2
	movs r2, #6
	movs r0, #4
	negs r1, r1
	negs r2, r2
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #14
	movs r2, #8
	movs r0, #4
	negs r1, r1
	negs r2, r2
	bl ObjectMotion_CommitPositionAndActivate
	bl Func_0200052c
	pop {pc}
	.2byte 0x0000
	.section .text.x02008624,"ax",%progbits
	.global Func_02000624
	.thumb_func
Func_02000624:
	push {lr}
	bl Func_02002068
	movs r0, #0
	bl Func_02002150
	movs r1, #192
	movs r0, #4
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	bl Func_0200052c
	pop {pc}
	.2byte 0x0000
	.section .text.x02008644,"ax",%progbits
	.global Func_02000644
	.thumb_func
Func_02000644:
	push {lr}
	sub sp, #8
	movs r3, #3
	movs r2, #15
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #3
	movs r2, #1
	movs r3, #1
	movs r0, #0
	bl Func_02002030
	movs r0, #140
	lsls r0, r0, #2
	bl GameFlag_SetBit
	add sp, #8
	pop {pc}
	.section .text.x02008668,"ax",%progbits
	.global Func_02000668
	.thumb_func
Func_02000668:
	push {lr}
	ldr r3, .L_020086cc
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_020086d0
	cmp r2, r3
	bne .L_02008680
	ldr r0, .L_020086d4
	b .L_020086c8
.L_02008680:
	ldr r3, .L_020086d8
	cmp r2, r3
	bne .L_0200868a
	ldr r0, .L_020086dc
	b .L_020086c8
.L_0200868a:
	ldr r3, .L_020086e0
	cmp r2, r3
	bne .L_02008694
	ldr r0, .L_020086e4
	b .L_020086c8
.L_02008694:
	ldr r3, .L_020086e8
	cmp r2, r3
	bne .L_0200869e
	ldr r0, .L_020086ec
	b .L_020086c8
.L_0200869e:
	ldr r3, .L_020086f0
	cmp r2, r3
	bne .L_020086a8
	ldr r0, .L_020086f4
	b .L_020086c8
.L_020086a8:
	ldr r3, .L_020086f8
	cmp r2, r3
	bne .L_020086b2
	ldr r0, .L_020086fc
	b .L_020086c8
.L_020086b2:
	ldr r3, .L_02008700
	cmp r2, r3
	bne .L_020086bc
	ldr r0, .L_02008704
	b .L_020086c8
.L_020086bc:
	ldr r3, .L_02008708
	cmp r2, r3
	bne .L_020086c6
	ldr r0, .L_0200870c
	b .L_020086c8
.L_020086c6:
	ldr r0, .L_02008710
.L_020086c8:
	pop {pc}
	.2byte 0x0000
.L_020086cc:
	.4byte gPartyState
.L_020086d0:
	.4byte 0x000000d5
.L_020086d4:
	.4byte Data_020028f0
.L_020086d8:
	.4byte 0x000000d6
.L_020086dc:
	.4byte Data_02002974
.L_020086e0:
	.4byte 0x000000d8
.L_020086e4:
	.4byte Data_020029a4
.L_020086e8:
	.4byte 0x000000d9
.L_020086ec:
	.4byte Data_02002a70
.L_020086f0:
	.4byte 0x000000db
.L_020086f4:
	.4byte Data_02002ad0
.L_020086f8:
	.4byte 0x000000dc
.L_020086fc:
	.4byte Data_02002b3c
.L_02008700:
	.4byte 0x000000dd
.L_02008704:
	.4byte Data_02002b90
.L_02008708:
	.4byte 0x000000de
.L_0200870c:
	.4byte Data_02002c2c
.L_02008710:
	.4byte Data_0200289c
	.section .text.x02008714,"ax",%progbits
	.global Func_02000714
	.thumb_func
Func_02000714:
	push {lr}
	adds r1, r0, #0
	adds r1, #100
	movs r3, #0
	ldrsh r2, [r1, r3]
	ldr r3, [r0, #8]
	lsls r2, r2, #8
	adds r3, r3, r2
	str r3, [r0, #8]
	ldr r3, [r0, #12]
	movs r2, #128
	lsls r2, r2, #8
	adds r3, r3, r2
	str r3, [r0, #12]
	movs r2, #160
	ldr r3, [r0, #24]
	lsls r2, r2, #3
	adds r2, #30
	adds r3, r3, r2
	str r3, [r0, #24]
	ldr r3, [r0, #28]
	adds r3, r3, r2
	str r3, [r0, #28]
	ldrh r3, [r1]
	adds r3, #2
	strh r3, [r1]
	ldr r3, [r0, #104]
	subs r3, #1
	str r3, [r0, #104]
	cmp r3, #0
	bne .L_02008756
	bl Func_02002000
.L_02008756:
	pop {pc}
	.section .text.x02008758,"ax",%progbits
	.global Func_02000758
	.thumb_func
Func_02000758:
	push {r5, lr}
	adds r4, r0, #0
	adds r5, r1, #0
	movs r0, #30
	adds r3, r2, #0
	adds r0, #255
	adds r2, r5, #0
	adds r1, r4, #0
	bl Func_02001ff8
	adds r5, r0, #0
	cmp r5, #0
	beq .L_020087d0
	adds r2, r5, #0
	adds r2, #92
	movs r3, #2
	strb r3, [r2]
	adds r3, r5, #0
	movs r1, #0
	adds r3, #85
	strb r1, [r3]
	adds r3, #15
	strh r1, [r3]
	adds r1, r5, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #254
	ands r3, r2
	strb r3, [r1]
	movs r1, #7
	bl Object_SetPartAttribute
	ldr r1, [r5, #80]
	movs r3, #13
	ldrb r2, [r1, #9]
	negs r3, r3
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	strb r3, [r1, #9]
	adds r0, r5, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r5, #24]
	str r3, [r5, #28]
	movs r3, #60
	str r3, [r5, #104]
	ldr r3, .L_020087d4
	adds r0, r5, #0
	movs r1, #5
	str r3, [r5, #108]
	bl Func_02001fe0
	adds r0, r5, #0
	movs r1, #1
	bl Animation_SetStateFlags
.L_020087d0:
	pop {r5, pc}
	.2byte 0x0000
.L_020087d4:
	.4byte Func_02000714
	.section .text.x020087d8,"ax",%progbits
	.global Func_020007d8
	.thumb_func
Func_020007d8:
	push {lr}
	ldr r3, .L_020087f8
	movs r2, #63
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	bne .L_020087f6
	movs r0, #248
	movs r1, #192
	movs r2, #128
	lsls r0, r0, #16
	lsls r1, r1, #14
	lsls r2, r2, #17
	bl Func_02000758
.L_020087f6:
	pop {pc}
.L_020087f8:
	.4byte Data_0300122c
	.section .text.x020087fc,"ax",%progbits
	.global Func_020007fc
	.thumb_func
Func_020007fc:
	push {r5, r6, r7, lr}
	adds r6, r0, #0
	ldr r0, [r6, #48]
	ldr r7, [r6, #80]
	bl Math_Sine
	lsls r5, r0, #1
	cmp r5, #0
	ble .L_02008810
	negs r5, r5
.L_02008810:
	ldr r0, [r6, #48]
	bl Math_Cosine
	ldr r3, [r6, #56]
	lsls r0, r0, #1
	adds r3, r3, r0
	str r3, [r6, #8]
	ldr r0, [r6, #48]
	ldr r3, [r6, #60]
	movs r2, #128
	lsls r2, r2, #8
	adds r3, r3, r5
	adds r0, r0, r2
	str r3, [r6, #12]
	bl Math_Cosine
	cmp r0, #0
	bge .L_02008836
	adds r0, #7
.L_02008836:
	asrs r3, r0, #3
	strh r3, [r7, #18]
	bl Random16Far
	adds r5, r0, #0
	bl Random16Far
	lsls r5, r5, #9
	ldr r3, [r6, #48]
	lsls r0, r0, #9
	lsrs r0, r0, #16
	lsrs r5, r5, #16
	adds r5, r5, r0
	movs r2, #128
	adds r3, r3, r5
	lsls r2, r2, #3
	adds r3, r3, r2
	str r3, [r6, #48]
	movs r0, #0
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x02008860,"ax",%progbits
	.global Func_02000860
	.thumb_func
Func_02000860:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	ldr r3, .L_020088a4
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	ldr r3, .L_020088a8
	subs r2, #2
	movs r1, #144
	strh r3, [r2]
	lsls r1, r1, #3
	ldr r0, .L_020088ac
	bl Scheduler_AddOrUpdateCallback
	bl Func_02002170
	bl Func_02002170
	movs r1, #130
	lsls r1, r1, #1
	movs r0, #1
	adds r1, #255
	movs r2, #8
	movs r3, #9
	bl Func_02002180
	movs r0, #240
	lsls r0, r0, #4
	adds r0, #93
	b .L_020088b0
.L_020088a4:
	.4byte 0x00000c08
.L_020088a8:
	.4byte 0x00003f10
.L_020088ac:
	.4byte Func_020007d8
.L_020088b0:
	bl GameFlag_Test
	mov r8, r0
	cmp r0, #0
	beq .L_020088c6
	movs r0, #11
	movs r1, #0
	movs r2, #0
	bl Func_020020c0
	b .L_0200897a
.L_020088c6:
	movs r0, #11
	bl Object_GetById
	adds r7, r0, #0
	ldr r6, [r7, #80]
	movs r2, #13
	ldrb r3, [r6, #9]
	negs r2, r2
	ldrb r1, [r6, #5]
	ands r2, r3
	movs r3, #4
	orrs r2, r3
	movs r3, #33
	negs r3, r3
	ands r3, r1
	strb r3, [r6, #5]
	movs r3, #15
	ands r2, r3
	strb r2, [r6, #9]
	mov r2, r8
	strb r2, [r6, #27]
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r3, #92
	adds r3, r3, r7
	mov r2, r8
	strb r2, [r3]
	mov r10, r3
	adds r3, r7, #0
	adds r3, #85
	movs r0, #10
	strb r2, [r3]
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200891c
	ldr r3, [r7, #12]
	movs r2, #128
	lsls r2, r2, #13
	adds r3, r3, r2
	str r3, [r7, #12]
.L_0200891c:
	adds r1, r7, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #254
	ands r3, r2
	movs r2, #1
	strb r3, [r1]
	mov r9, r2
	adds r3, r7, #0
	adds r3, #97
	mov r2, r9
	movs r1, #193
	strb r2, [r3]
	lsls r1, r1, #3
	movs r0, #68
	bl Runtime_AllocateHeapBlockFar
	adds r5, r0, #0
	movs r0, #193
	bl Func_02002058
	movs r3, #128
	lsls r3, r3, #3
	adds r5, r5, r3
	adds r2, r5, #0
	movs r1, #128
	ldrb r0, [r6, #16]
	bl VramBlock_LoadCached
	movs r0, #68
	bl Runtime_ReleaseHeapBlock
	ldr r3, [r7, #8]
	mov r2, r8
	str r3, [r7, #56]
	ldr r3, [r7, #12]
	str r2, [r7, #48]
	str r3, [r7, #60]
	mov r2, r10
	mov r3, r9
	strb r3, [r2]
	ldr r3, .L_02008984
	mov r2, r8
	str r3, [r7, #108]
	adds r3, r7, #0
	adds r3, #86
	strb r2, [r3]
.L_0200897a:
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_02008984:
	.4byte Func_020007fc
	.section .text.x02008988,"ax",%progbits
	.global Func_02000988
	.thumb_func
Func_02000988:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r1, #214
	lsls r1, r1, #1
	movs r2, #128
	adds r3, r3, r1
	lsls r2, r2, #1
	str r2, [r3]
	ldr r3, .L_02008af8
	adds r2, #224
	adds r5, r3, r2
	movs r3, #0
	ldrsh r2, [r5, r3]
	ldr r3, .L_02008afc
	sub sp, #8
	cmp r2, r3
	bne .L_020089b2
	bl Func_02000860
.L_020089b2:
	movs r1, #0
	ldrsh r2, [r5, r1]
	ldr r3, .L_02008b00
	cmp r2, r3
	bne .L_02008a66
	movs r0, #0
	bl Func_02002140
	ldr r0, .L_02008b04
	bl Func_02002188
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #6
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020089e0
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_020020c0
.L_020089e0:
	movs r0, #10
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #2
	orrs r3, r2
	strb r3, [r0]
	movs r1, #2
	movs r0, #18
	bl Object_SetModeById
	movs r0, #19
	movs r1, #2
	bl Object_SetModeById
	movs r0, #21
	movs r1, #2
	bl Object_SetModeById
	movs r0, #131
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008a1e
	movs r0, #10
	movs r1, #2
	bl Object_SetModeById
.L_02008a1e:
	bl Func_02000e04
	movs r0, #129
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008a66
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_020020c0
	movs r1, #184
	movs r2, #184
	lsls r1, r1, #16
	lsls r2, r2, #16
	movs r0, #21
	bl Func_020020c0
	movs r0, #21
	bl Object_GetById
	movs r3, #0
	adds r0, #85
	strb r3, [r0]
	movs r2, #12
	movs r3, #11
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #9
	movs r1, #11
	movs r2, #1
	movs r3, #1
	bl Func_02002030
.L_02008a66:
	ldr r3, .L_02008af8
	movs r2, #240
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008b08
	cmp r2, r3
	bne .L_02008aca
	movs r0, #140
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008aa0
	movs r3, #3
	movs r2, #15
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #3
	movs r2, #1
	movs r3, #1
	bl Func_02002030
	movs r0, #9
	movs r1, #4
	bl Object_SetModeById
.L_02008aa0:
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008ac4
	movs r0, #136
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008ac4
	movs r0, #136
	lsls r0, r0, #2
	bl GameFlag_ClearBit
	bl Func_0200125c
.L_02008ac4:
	ldr r0, .L_02008b0c
	bl Func_02002188
.L_02008aca:
	ldr r3, .L_02008af8
	movs r2, #240
	lsls r2, r2, #1
	adds r5, r3, r2
	movs r3, #0
	ldrsh r2, [r5, r3]
	ldr r3, .L_02008b10
	cmp r2, r3
	bne .L_02008ae2
	movs r0, #0
	bl Func_02002140
.L_02008ae2:
	movs r1, #0
	ldrsh r2, [r5, r1]
	ldr r3, .L_02008b14
	cmp r2, r3
	bne .L_02008af2
	movs r0, #0
	bl Func_02002140
.L_02008af2:
	movs r0, #0
	add sp, #8
	pop {r5, pc}
.L_02008af8:
	.4byte gPartyState
.L_02008afc:
	.4byte 0x000000db
.L_02008b00:
	.4byte 0x000000dd
.L_02008b04:
	.4byte Data_0200226c
.L_02008b08:
	.4byte 0x000000d8
.L_02008b0c:
	.4byte Data_02002272
.L_02008b10:
	.4byte 0x000000d7
.L_02008b14:
	.4byte 0x000000da
	.section .text.x02008b18,"ax",%progbits
	.global Func_02000b18
	.thumb_func
Func_02000b18:
	push {r5, lr}
	ldr r3, .L_02008c10
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008c14
	sub sp, #8
	cmp r2, r3
	bne .L_02008bb6
	movs r0, #131
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008b72
	movs r3, #11
	movs r2, #12
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #9
	movs r1, #11
	movs r2, #1
	movs r3, #1
	bl Func_02002030
	movs r5, #1
	movs r0, #12
	movs r1, #32
	movs r2, #11
	movs r3, #15
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl Func_02002018
	movs r0, #77
	movs r1, #15
	movs r2, #75
	movs r3, #15
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl Func_02002018
.L_02008b72:
	movs r0, #129
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008bb6
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_020020c0
	movs r1, #184
	movs r2, #184
	lsls r1, r1, #16
	lsls r2, r2, #16
	movs r0, #21
	bl Func_020020c0
	movs r0, #21
	bl Object_GetById
	movs r3, #0
	adds r0, #85
	strb r3, [r0]
	movs r2, #12
	movs r3, #11
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #9
	movs r1, #11
	movs r2, #1
	movs r3, #1
	bl Func_02002030
.L_02008bb6:
	ldr r3, .L_02008c10
	movs r2, #240
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008c18
	cmp r2, r3
	bne .L_02008c0a
	movs r0, #166
	lsls r0, r0, #4
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008c0a
	movs r5, #1
	movs r0, #5
	movs r1, #46
	movs r2, #5
	movs r3, #47
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl Func_02002018
	movs r0, #4
	movs r1, #25
	movs r2, #5
	movs r3, #17
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl Func_02002018
	movs r3, #5
	movs r2, #17
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #4
	movs r1, #25
	movs r2, #1
	movs r3, #1
	bl Func_02002030
.L_02008c0a:
	movs r0, #0
	add sp, #8
	pop {r5, pc}
.L_02008c10:
	.4byte gPartyState
.L_02008c14:
	.4byte 0x000000dd
.L_02008c18:
	.4byte 0x000000d5
	.section .text.x02008c1c,"ax",%progbits
	.global Func_02000c1c
	.thumb_func
Func_02000c1c:
	push {lr}
	movs r0, #129
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
	pop {pc}
	.2byte 0x0000
	.section .text.x02008c2c,"ax",%progbits
	.global Func_02000c2c
	.thumb_func
Func_02000c2c:
	push {lr}
	movs r0, #129
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_ClearBit
	pop {pc}
	.2byte 0x0000
	.section .text.x02008c3c,"ax",%progbits
	.global Func_02000c3c
	.thumb_func
Func_02000c3c:
	push {lr}
	bl Func_02001770
	pop {pc}
	.section .text.x02008c44,"ax",%progbits
	.global Func_02000c44
	.thumb_func
Func_02000c44:
	push {r5, lr}
	sub sp, #8
	bl Func_02002068
	movs r0, #0
	bl Func_02002150
	movs r5, #8
.L_02008c54:
	adds r0, r5, #0
	bl Object_GetById
	cmp r0, #0
	beq .L_02008c66
	adds r2, r0, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
.L_02008c66:
	adds r5, #1
	cmp r5, #63
	bls .L_02008c54
	ldr r3, .L_02008cf4
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008cf8
	cmp r2, r3
	bne .L_02008c94
	movs r3, #1
	movs r2, #2
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #33
	movs r1, #30
	movs r2, #23
	movs r3, #14
	bl Func_02002018
	b .L_02008ca8
.L_02008c94:
	movs r3, #1
	movs r2, #2
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #27
	movs r1, #29
	movs r2, #40
	movs r3, #16
	bl Func_02002018
.L_02008ca8:
	movs r0, #158
	bl Func_020021b0
	ldr r5, .L_02008cf4
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
	ldr r0, [r5]
	movs r1, #2
	bl Object_SetModeById
	movs r1, #128
	movs r2, #128
	ldr r0, [r5]
	lsls r1, r1, #8
	lsls r2, r2, #7
	bl ObjectMotion_SetSpeedParameters
	movs r2, #4
	movs r1, #2
	negs r2, r2
	ldr r0, [r5]
	bl ObjectMotion_SnapHeadingAndOffset
	movs r0, #6
	bl Battle_WaitMode0
	movs r0, #2
	bl Func_02002110
	bl Event_ClearStatus1c6
	bl Event_WaitValue1c8Frames
	bl Func_02002070
	add sp, #8
	pop {r5, pc}
.L_02008cf4:
	.4byte gPartyState
.L_02008cf8:
	.4byte 0x000000d8
	.section .text.x02008cfc,"ax",%progbits
	.global Func_02000cfc
	.thumb_func
Func_02000cfc:
	push {lr}
	movs r1, #8
	movs r2, #9
	movs r0, #1
	bl Func_02002178
	movs r0, #130
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
	pop {pc}
	.section .text.x02008d14,"ax",%progbits
	.global Func_02000d14
	.thumb_func
Func_02000d14:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r1, #179
	lsls r1, r1, #1
	adds r3, r2, r1
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	bne .L_02008de4
	movs r1, #192
	lsls r1, r1, #4
	adds r1, #164
	adds r3, r2, r1
	movs r6, #0
	ldrsb r6, [r3, r6]
	cmp r6, #0
	bne .L_02008de4
	ldr r7, .L_02008dec
	movs r2, #1
	ldr r3, [r7]
	negs r2, r2
	cmp r3, r2
	bne .L_02008d6a
	ldr r5, .L_02008df0
	ldr r3, .L_02008df4
	movs r1, #160
	lsls r1, r1, #19
	mov r8, r3
	adds r1, #96
	movs r2, #32
	adds r0, r5, #0
	mov lr, r8
	.2byte 0xf800
	ldr r0, .L_02008df8
	adds r1, r5, #0
	movs r2, #32
	mov lr, r8
	.2byte 0xf800
	str r6, [r7]
.L_02008d6a:
	ldr r3, [r7]
	cmp r3, #0
	bge .L_02008d72
	adds r3, #7
.L_02008d72:
	asrs r2, r3, #3
	adds r3, r2, #0
	cmp r2, #0
	bge .L_02008d7c
	adds r3, r2, #3
.L_02008d7c:
	asrs r1, r3, #2
	ldr r6, .L_02008df8
	ldr r5, .L_02008df0
	lsls r3, r1, #2
	subs r1, r2, r3
	movs r0, #0
	movs r4, #24
.L_02008d8a:
	adds r2, r1, r0
	adds r3, r2, #0
	cmp r2, #0
	bge .L_02008d94
	adds r3, r2, #3
.L_02008d94:
	asrs r3, r3, #2
	lsls r3, r3, #2
	subs r3, r2, r3
	ldrh r2, [r5, r4]
	lsls r3, r3, #1
	adds r3, #24
	adds r0, #1
	strh r2, [r6, r3]
	adds r4, #2
	cmp r0, #3
	ble .L_02008d8a
	ldr r1, .L_02008dfc
	ldr r0, .L_02008e00
	ldrh r3, [r0]
	adds r4, r3, #0
	strh r0, [r0]
	ldrh r2, [r1]
	cmp r2, #31
	bgt .L_02008dda
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	adds r3, r3, r1
	adds r3, #4
	adds r2, #1
	stmia r3!, {r6}
	strh r2, [r1]
	movs r2, #160
	lsls r2, r2, #19
	adds r2, #96
	stmia r3!, {r2}
	movs r2, #132
	lsls r2, r2, #24
	adds r2, #8
	str r2, [r3]
.L_02008dda:
	strh r4, [r0]
	ldr r2, .L_02008dec
	ldr r3, [r2]
	adds r3, #1
	str r3, [r2]
.L_02008de4:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008dec:
	.4byte Data_02002c5c
.L_02008df0:
	.4byte gOverlayArea + 0x2c60
.L_02008df4:
	.4byte IwramCopyWords
.L_02008df8:
	.4byte gOverlayArea + 0x2c80
.L_02008dfc:
	.4byte Data_020038e0
.L_02008e00:
	.4byte 0x04000208
	.section .text.x02008e04,"ax",%progbits
	.global Func_02000e04
	.thumb_func
Func_02000e04:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r1, #144
	adds r2, #86
	str r2, [r3]
	lsls r1, r1, #3
	ldr r0, .L_02008e2c
	bl Scheduler_AddOrUpdateCallback
	movs r0, #170
	bl Func_02002168
	bl Func_02000ed0
	pop {pc}
.L_02008e2c:
	.4byte Func_02000d14
	.section .text.x02008e30,"ax",%progbits
	.global Func_02000e30
	.thumb_func
Func_02000e30:
	push {lr}
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #6
	ldrh r2, [r3]
	ldr r3, .L_02008e5c
	ldr r3, [r3]
	cmp r2, r3
	bge .L_02008e64
	ldr r3, .L_02008e60
	ldrh r2, [r3]
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #20
	strh r2, [r3]
	movs r2, #128
	ldr r3, .L_02008e58
	lsls r2, r2, #19
	adds r2, #80
	b .L_02008e7e
.L_02008e58:
	.4byte 0x00000000
.L_02008e5c:
	.4byte gOverlayArea + 0x2ce0
.L_02008e60:
	.4byte gOverlayArea + 0x2ce4
.L_02008e64:
	ldr r3, .L_02008e8c
	ldrh r2, [r3]
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #20
	strh r2, [r3]
	movs r2, #128
	ldr r3, .L_02008e84
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	ldr r3, .L_02008e88
	subs r2, #2
.L_02008e7e:
	strh r3, [r2]
	pop {pc}
	.2byte 0x0000
.L_02008e84:
	.4byte 0x0000100c
.L_02008e88:
	.4byte 0x00003f42
.L_02008e8c:
	.4byte gOverlayArea + 0x2ce6
	.section .text.x02008e90,"ax",%progbits
	.global Func_02000e90
	.thumb_func
Func_02000e90:
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #32]
	movs r1, #188
	lsls r1, r1, #1
	adds r2, r2, r1
	movs r3, #6
	ldrsh r1, [r2, r3]
	ldr r0, .L_02008ec0
	movs r3, #181
	lsls r3, r3, #3
	subs r3, r3, r1
	str r3, [r0]
	ldr r3, .L_02008ec4
	movs r1, #2
	ldrsh r2, [r2, r1]
	ldr r1, .L_02008ec8
	strh r2, [r3]
	ldr r3, .L_02008ecc
	ldr r3, [r3]
	lsrs r3, r3, #2
	subs r2, r2, r3
	strh r2, [r1]
	bx lr
.L_02008ec0:
	.4byte gOverlayArea + 0x2ce0
.L_02008ec4:
	.4byte gOverlayArea + 0x2ce4
.L_02008ec8:
	.4byte gOverlayArea + 0x2ce6
.L_02008ecc:
	.4byte Data_0300122c
	.section .text.x02008ed0,"ax",%progbits
	.global Func_02000ed0
	.thumb_func
Func_02000ed0:
	push {lr}
	ldr r2, .L_02008ee8
	movs r0, #1
	movs r1, #0
	bl Func_02001fa8
	movs r1, #200
	lsls r1, r1, #4
	ldr r0, .L_02008eec
	bl Scheduler_AddOrUpdateCallback
	pop {pc}
.L_02008ee8:
	.4byte Func_02000e30
.L_02008eec:
	.4byte Func_02000e90
	.section .text.x02008ef0,"ax",%progbits
	.global Func_02000ef0
	.thumb_func
Func_02000ef0:
	push {lr}
	ldr r0, .L_02008efc
	bl Func_02002190
	pop {pc}
	.2byte 0x0000
.L_02008efc:
	.4byte Data_0200226c
	.section .text.x02008f00,"ax",%progbits
	.global Func_02000f00
	.thumb_func
Func_02000f00:
	push {lr}
	ldr r3, .L_02008f24
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #2
	orrs r3, r2
	strb r3, [r0]
	ldr r0, .L_02008f28
	bl Func_02002190
	pop {pc}
	.2byte 0x0000
.L_02008f24:
	.4byte gPartyState
.L_02008f28:
	.4byte Data_0200226c
	.section .text.x02008f2c,"ax",%progbits
	.global Func_02000f2c
	.thumb_func
Func_02000f2c:
	push {r5, r6, r7, lr}
	movs r0, #9
	sub sp, #8
	bl Object_GetById
	adds r5, r0, #0
	ldr r3, [r5, #8]
	movs r0, #129
	asrs r7, r3, #20
	ldr r3, [r5, #16]
	lsls r0, r0, #2
	asrs r6, r3, #20
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008fc4
	cmp r7, #11
	bne .L_02008fc4
	cmp r6, #12
	bne .L_02008fc4
	adds r3, r5, #0
	adds r3, #85
	strb r0, [r3]
	movs r6, #10
.L_02008f5c:
	ldr r3, [r5, #12]
	ldr r2, .L_02008fc8
	movs r0, #1
	adds r3, r3, r2
	str r3, [r5, #12]
	subs r6, #1
	bl Battle_WaitMode0
	cmp r6, #0
	bgt .L_02008f5c
	movs r0, #9
	bl Func_0200156c
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_020020c0
	movs r1, #184
	movs r2, #184
	lsls r1, r1, #16
	lsls r2, r2, #16
	movs r0, #21
	bl Func_020020c0
	movs r0, #21
	bl Object_GetById
	movs r3, #0
	adds r0, #85
	strb r3, [r0]
	movs r0, #21
	bl Object_GetById
	ldr r2, .L_02008fcc
	ldr r3, [r0, #12]
	movs r1, #11
	adds r3, r3, r2
	str r3, [r0, #12]
	movs r2, #12
	movs r3, #11
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #9
	movs r2, #1
	movs r3, #1
	bl Func_02002030
	movs r0, #129
	lsls r0, r0, #2
	bl GameFlag_SetBit
.L_02008fc4:
	add sp, #8
	pop {r5, r6, r7, pc}
.L_02008fc8:
	.4byte 0xfffe0000
.L_02008fcc:
	.4byte 0xffc00000
	.section .text.x02008fd0,"ax",%progbits
	.global Func_02000fd0
	.thumb_func
Func_02000fd0:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r0, #10
	sub sp, #8
	bl Object_GetById
	adds r7, r0, #0
	ldr r3, [r7, #8]
	asrs r6, r3, #20
	ldr r3, [r7, #16]
	asrs r5, r3, #20
	cmp r6, #11
	beq .L_02008fee
	b .L_0200915c
.L_02008fee:
	cmp r5, #9
	beq .L_02008ff4
	b .L_0200915c
.L_02008ff4:
	adds r2, r7, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	movs r6, #8
.L_02008ffe:
	movs r0, #1
	bl Battle_WaitMode0
	ldr r3, [r7, #12]
	ldr r2, .L_020091ac
	subs r6, #1
	adds r3, r3, r2
	str r3, [r7, #12]
	cmp r6, #0
	bgt .L_02008ffe
	movs r0, #10
	bl Func_0200156c
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #192
	movs r1, #1
	movs r2, #224
	movs r3, #1
	lsls r0, r0, #16
	negs r1, r1
	lsls r2, r2, #16
	bl Motion_CamBounds
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #10
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #0
	movs r2, #8
	movs r0, #10
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r0, #10
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #10
	bl Battle_WaitMode0
	ldr r3, .L_020091b0
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	movs r1, #128
	ldr r0, [r3]
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #10
	ldr r1, .L_020091b4
	ldr r2, .L_020091b8
	bl ObjectMotion_SetSpeedParameters
	movs r1, #0
	movs r2, #80
	movs r0, #10
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r0, #10
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #10
	bl Func_0200156c
	movs r0, #129
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020090f4
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #10
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #0
	movs r2, #80
	movs r0, #10
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r0, #10
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r1, #160
	movs r2, #160
	movs r0, #10
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl ObjectMotion_SetSpeedParameters
	movs r1, #0
	movs r2, #80
	movs r0, #10
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r0, #5
	bl Battle_WaitMode0
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_020020c0
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #6
	bl GameFlag_SetBit
	b .L_02009156
.L_020090f4:
	movs r5, #1
	movs r0, #12
	movs r1, #32
	movs r2, #11
	movs r3, #15
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl Func_02002018
	movs r3, #15
	movs r0, #77
	movs r1, #15
	movs r2, #75
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl Func_02002018
	movs r1, #0
	movs r2, #12
	movs r0, #10
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r0, #10
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #20
	bl Battle_WaitMode0
	movs r3, #11
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r2, #1
	movs r3, #1
	movs r0, #9
	movs r1, #11
	bl Func_02002030
	movs r0, #10
	movs r1, #2
	bl Object_SetModeById
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #131
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
.L_02009156:
	bl Func_02002070
	b .L_020091a4
.L_0200915c:
	movs r0, #132
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	mov r8, r0
	cmp r0, #0
	bne .L_020091a4
	cmp r6, #16
	bne .L_020091a4
	cmp r5, #12
	bne .L_020091a4
	str r5, [sp, #4]
	movs r1, #12
	movs r2, #1
	movs r0, #17
	movs r3, #1
	adds r5, r7, #0
	str r6, [sp, #0]
	adds r5, #85
	bl Func_02002030
	movs r3, #3
	strb r3, [r5]
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #132
	mov r3, r8
	lsls r0, r0, #1
	strb r3, [r5]
	adds r0, #255
	bl GameFlag_SetBit
	bl Func_02002070
.L_020091a4:
	add sp, #8
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_020091ac:
	.4byte 0xfffe0000
.L_020091b0:
	.4byte gPartyState
.L_020091b4:
	.4byte 0x00026666
.L_020091b8:
	.4byte 0x00013333
	.section .text.x020091bc,"ax",%progbits
	.global Func_020011bc
	.thumb_func
Func_020011bc:
	push {lr}
	bl Func_02002198
	bl Func_02000f2c
	pop {pc}
	.section .text.x020091c8,"ax",%progbits
	.global Func_020011c8
	.thumb_func
Func_020011c8:
	push {lr}
	bl Func_02002198
	bl Func_02000fd0
	pop {pc}
	.section .text.x020091d4,"ax",%progbits
	.global Func_020011d4
	.thumb_func
Func_020011d4:
	push {lr}
	bl Func_02002198
	ldr r3, .L_020091fc
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	adds r3, #254
	strb r3, [r0]
	bl Func_02000f2c
	bl Func_02000fd0
	pop {pc}
	.2byte 0x0000
.L_020091fc:
	.4byte gPartyState
	.section .text.x02009200,"ax",%progbits
	.global Func_02001200
	.thumb_func
Func_02001200:
	push {r5, lr}
	sub sp, #8
	movs r3, #0
	str r3, [sp, #4]
	movs r5, #64
	movs r0, #11
	movs r1, #73
	movs r2, #10
	movs r3, #10
	str r5, [sp, #0]
	bl Func_02002030
	movs r3, #10
	movs r0, #42
	movs r1, #73
	movs r2, #10
	str r5, [sp, #0]
	str r3, [sp, #4]
	bl Func_02002030
	add sp, #8
	pop {r5, pc}
	.section .text.x0200922c,"ax",%progbits
	.global Func_0200122c
	.thumb_func
Func_0200122c:
	push {r5, lr}
	sub sp, #8
	movs r3, #11
	str r3, [sp, #0]
	movs r5, #73
	movs r0, #64
	movs r1, #0
	movs r2, #10
	movs r3, #10
	str r5, [sp, #4]
	bl Func_02002030
	movs r3, #42
	str r3, [sp, #0]
	movs r0, #64
	movs r1, #10
	movs r2, #10
	movs r3, #10
	str r5, [sp, #4]
	bl Func_02002030
	add sp, #8
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x0200925c,"ax",%progbits
	.global Func_0200125c
	.thumb_func
Func_0200125c:
	push {r5, lr}
	movs r0, #145
	lsls r0, r0, #1
	adds r0, #255
	sub sp, #8
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020092bc
	movs r0, #136
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020092bc
	movs r3, #50
	str r3, [sp, #4]
	movs r5, #64
	movs r0, #11
	movs r1, #73
	movs r2, #10
	movs r3, #10
	str r5, [sp, #0]
	bl Func_02002030
	movs r3, #60
	str r3, [sp, #4]
	movs r0, #42
	movs r1, #73
	movs r2, #10
	movs r3, #10
	str r5, [sp, #0]
	bl Func_02002030
	movs r3, #11
	movs r2, #75
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #42
	movs r1, #11
	movs r2, #10
	movs r3, #4
	bl Func_02002030
	movs r0, #136
	lsls r0, r0, #2
	bl GameFlag_SetBit
.L_020092bc:
	add sp, #8
	pop {r5, pc}
	.section .text.x020092c0,"ax",%progbits
	.global Func_020012c0
	.thumb_func
Func_020012c0:
	push {r5, lr}
	movs r0, #136
	lsls r0, r0, #2
	sub sp, #8
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020092fe
	movs r3, #11
	str r3, [sp, #0]
	movs r5, #73
	movs r0, #64
	movs r1, #50
	movs r2, #10
	movs r3, #10
	str r5, [sp, #4]
	bl Func_02002030
	movs r3, #42
	str r3, [sp, #0]
	movs r0, #64
	movs r1, #60
	movs r2, #10
	movs r3, #10
	str r5, [sp, #4]
	bl Func_02002030
	movs r0, #136
	lsls r0, r0, #2
	bl GameFlag_ClearBit
.L_020092fe:
	add sp, #8
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x02009304,"ax",%progbits
	.global Func_02001304
	.thumb_func
Func_02001304:
	push {r5, r6, lr}
	movs r0, #10
	sub sp, #8
	bl Object_GetById
	ldr r3, [r0, #8]
	asrs r6, r3, #20
	ldr r3, [r0, #16]
	asrs r5, r3, #20
	bl Func_02001200
	ldr r0, .L_02009388
	bl Func_02002190
	cmp r6, #12
	bne .L_02009336
	cmp r5, #15
	bne .L_02009336
	movs r3, #42
	movs r2, #14
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #42
	movs r1, #3
	b .L_02009352
.L_02009336:
	cmp r6, #13
	bne .L_0200933e
	cmp r5, #15
	beq .L_02009346
.L_0200933e:
	cmp r6, #16
	bne .L_02009370
	cmp r5, #15
	bne .L_02009370
.L_02009346:
	movs r3, #42
	movs r2, #14
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #42
	movs r1, #0
.L_02009352:
	movs r2, #7
	movs r3, #3
	bl Func_02002030
	movs r3, #11
	movs r2, #73
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #42
	movs r1, #9
	movs r2, #10
	movs r3, #10
	bl Func_02002030
	b .L_02009384
.L_02009370:
	movs r3, #11
	movs r2, #73
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #42
	movs r1, #9
	movs r2, #10
	movs r3, #10
	bl Func_02002030
.L_02009384:
	add sp, #8
	pop {r5, r6, pc}
.L_02009388:
	.4byte Data_02002272
	.section .text.x0200938c,"ax",%progbits
	.global Func_0200138c
	.thumb_func
Func_0200138c:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #8
	bl Func_02002068
	movs r0, #0
	bl Func_02002150
	movs r0, #10
	movs r1, #1
	bl Object_AttachWorkTargetToObject
	movs r0, #10
	bl Object_GetById
	adds r7, r0, #0
	ldr r3, [r7, #8]
	asrs r6, r3, #20
	ldr r3, [r7, #16]
	asrs r5, r3, #20
	cmp r6, #12
	bne .L_020093ee
	cmp r5, #15
	bne .L_020093ee
	movs r0, #145
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
	adds r2, r7, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	movs r5, #32
.L_020093d2:
	movs r0, #1
	bl Battle_WaitMode0
	ldr r3, [r7, #12]
	ldr r2, .L_02009554
	subs r5, #1
	adds r3, r3, r2
	str r3, [r7, #12]
	cmp r5, #0
	bgt .L_020093d2
	movs r0, #10
	bl Func_02001620
	b .L_020094aa
.L_020093ee:
	movs r0, #131
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02009442
	cmp r6, #16
	bne .L_02009442
	cmp r5, #15
	bne .L_02009442
	adds r3, r7, #0
	adds r3, #85
	strb r0, [r3]
	movs r0, #131
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
	movs r0, #145
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
	movs r5, #48
.L_02009420:
	movs r0, #1
	bl Battle_WaitMode0
	ldr r3, [r7, #12]
	ldr r2, .L_02009554
	subs r5, #1
	adds r3, r3, r2
	str r3, [r7, #12]
	cmp r5, #0
	bgt .L_02009420
	movs r0, #10
	bl Func_02001620
	movs r0, #30
	bl Battle_WaitMode0
	b .L_02009548
.L_02009442:
	cmp r6, #19
	bne .L_020094b2
	cmp r5, #15
	bne .L_020094b2
	movs r0, #18
	movs r1, #16
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	str r6, [sp, #0]
	bl Func_02002030
	adds r2, r7, #0
	movs r0, #145
	adds r2, #85
	movs r3, #0
	lsls r0, r0, #1
	strb r3, [r2]
	adds r0, #255
	bl GameFlag_SetBit
	movs r5, #32
.L_0200946e:
	movs r0, #1
	bl Battle_WaitMode0
	ldr r3, [r7, #12]
	ldr r2, .L_02009554
	subs r5, #1
	adds r3, r3, r2
	str r3, [r7, #12]
	cmp r5, #0
	bgt .L_0200946e
	movs r0, #10
	bl Func_02001620
	movs r0, #102
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020094aa
	movs r0, #11
	movs r1, #0
	movs r2, #0
	bl Func_020020c0
	movs r0, #10
	ldr r1, .L_02009558
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #20
	bl Battle_WaitMode0
.L_020094aa:
	movs r0, #20
	bl Battle_WaitMode0
	b .L_02009548
.L_020094b2:
	movs r0, #129
	lsls r0, r0, #2
	bl GameFlag_Test
	mov r8, r0
	cmp r0, #0
	bne .L_0200951e
	cmp r6, #13
	bne .L_0200951e
	cmp r5, #15
	bne .L_0200951e
	movs r3, #12
	str r3, [sp, #0]
	movs r0, #1
	movs r1, #3
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl Func_02002030
	movs r2, #1
	movs r3, #1
	movs r0, #1
	movs r1, #3
	str r5, [sp, #4]
	str r6, [sp, #0]
	bl Func_02002030
	adds r3, r7, #0
	adds r3, #85
	mov r2, r8
	strb r2, [r3]
	movs r5, #16
.L_020094f4:
	movs r0, #1
	bl Battle_WaitMode0
	ldr r3, [r7, #12]
	ldr r2, .L_02009554
	subs r5, #1
	adds r3, r3, r2
	str r3, [r7, #12]
	cmp r5, #0
	bgt .L_020094f4
	movs r0, #10
	bl Func_02001620
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #129
	lsls r0, r0, #2
	bl GameFlag_SetBit
	b .L_02009548
.L_0200951e:
	cmp r6, #14
	bne .L_02009548
	cmp r5, #15
	bne .L_02009548
	movs r3, #12
	str r3, [sp, #0]
	movs r0, #1
	movs r1, #3
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl Func_02002030
	movs r0, #1
	movs r1, #3
	movs r2, #1
	movs r3, #1
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl Func_02002030
.L_02009548:
	bl Func_02002070
	add sp, #8
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_02009554:
	.4byte 0xfffe0000
.L_02009558:
	.4byte Data_0200232c
	.section .text.x0200955c,"ax",%progbits
	.global Func_0200155c
	.thumb_func
Func_0200155c:
	push {lr}
	bl Func_02002198
	bl Func_0200122c
	bl Func_0200138c
	pop {pc}
	.section .text.x0200956c,"ax",%progbits
	.global Func_0200156c
	.thumb_func
Func_0200156c:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	sub sp, #68
	bl Object_GetById
	movs r3, #0
	mov r8, r3
	movs r3, #22
	add r5, sp, #16
	adds r3, #255
	strh r3, [r5, #24]
	movs r3, #204
	lsls r3, r3, #8
	adds r3, #204
	str r3, [r5, #8]
	str r3, [r5, #12]
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r5, #16]
	str r3, [r5, #20]
	ldr r3, .L_020095c4
	adds r6, r0, #0
	movs r0, #154
	str r3, [r5, #28]
	bl Func_020021b0
	mov r3, r8
	ldr r0, [r6, #8]
	ldr r1, [r6, #12]
	ldr r2, [r6, #16]
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r3, #224
	lsls r3, r3, #13
	str r3, [sp, #8]
	movs r3, #0
	str r5, [sp, #12]
	bl Func_020000b8
	add sp, #68
	pop {r3}
	mov r8, r3
	pop {r5, r6, pc}
.L_020095c4:
	.4byte Data_02002310
	.section .text.x020095c8,"ax",%progbits
	.global Func_020015c8
	.thumb_func
Func_020015c8:
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
	bge .L_020095f8
	adds r3, #15
.L_020095f8:
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
	.section .text.x02009620,"ax",%progbits
	.global Func_02001620
	.thumb_func
Func_02001620:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #68
	bl Object_GetById
	ldr r3, .L_020096a0
	add r2, sp, #16
	str r3, [r2, #36]
	movs r3, #0
	adds r7, r0, #0
	mov r9, r2
	mov r10, r3
.L_0200963e:
	mov r2, r10
	lsls r6, r2, #12
	adds r0, r6, #0
	bl Math_Cosine
	add r5, sp, #56
	movs r3, #0
	str r0, [r5]
	adds r0, r6, #0
	str r3, [r5, #4]
	bl Math_Sine
	ldr r6, [r5]
	mov r8, r0
	str r0, [r5, #8]
	movs r1, #3
	adds r0, r6, #0
	bl Engine_MathDivide
	ldr r3, [r5, #4]
	adds r6, r6, r0
	str r6, [r5]
	ldr r2, [r7, #16]
	ldr r0, [r7, #8]
	ldr r1, [r7, #12]
	str r3, [sp, #0]
	mov r3, r8
	str r3, [sp, #4]
	movs r3, #128
	lsls r3, r3, #17
	adds r3, #1
	str r3, [sp, #8]
	mov r3, r9
	str r3, [sp, #12]
	adds r3, r6, #0
	bl Func_020000b8
	movs r2, #2
	add r10, r2
	mov r3, r10
	cmp r3, #16
	bls .L_0200963e
	add sp, #68
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_020096a0:
	.4byte Func_020015c8
	.section .text.x020096a4,"ax",%progbits
	.global Func_020016a4
	.thumb_func
Func_020016a4:
	push {lr}
	movs r0, #130
	lsls r0, r0, #2
	bl GameFlag_SetBit
	pop {pc}
	.section .text.x020096b0,"ax",%progbits
	.global Func_020016b0
	.thumb_func
Func_020016b0:
	push {r5, lr}
	adds r5, r0, #0
	adds r2, r5, #0
	adds r2, #100
	movs r0, #0
	ldrsh r1, [r2, r0]
	ldrh r3, [r2]
	cmp r1, #0
	beq .L_020096c8
	subs r3, #1
	strh r3, [r2]
	b .L_0200972e
.L_020096c8:
	adds r3, r5, #0
	adds r3, #90
	movs r0, #131
	strb r1, [r3]
	lsls r0, r0, #1
	bl GameFlag_Test
	movs r3, #1
	negs r3, r3
	cmp r0, #0
	bne .L_020096ee
	ldr r3, .L_02009730
	movs r2, #15
	ldr r3, [r3]
	ldr r1, .L_02009734
	lsrs r3, r3, #4
	ands r3, r2
	lsls r3, r3, #1
	ldrsh r3, [r1, r3]
.L_020096ee:
	movs r0, #1
	negs r0, r0
	cmp r3, r0
	bne .L_02009700
	adds r0, r5, #0
	movs r1, #9
	bl Func_02001fe0
	b .L_0200972e
.L_02009700:
	ldrh r1, [r5, #6]
	movs r2, #128
	subs r3, r3, r1
	lsls r3, r3, #16
	asrs r3, r3, #16
	lsls r2, r2, #5
	cmp r3, r2
	ble .L_02009712
	adds r3, r2, #0
.L_02009712:
	ldr r2, .L_02009738
	cmp r3, r2
	bge .L_0200971a
	adds r3, r2, #0
.L_0200971a:
	adds r3, r1, r3
	adds r0, r5, #0
	movs r1, #2
	strh r3, [r5, #6]
	bl Func_02001fe0
	adds r0, r5, #0
	movs r1, #48
	bl ObjectDispatch_ApplyValueToChildren
.L_0200972e:
	pop {r5, pc}
.L_02009730:
	.4byte gInput
.L_02009734:
	.4byte Data_02002276
.L_02009738:
	.4byte 0xfffff000
	.section .text.x0200973c,"ax",%progbits
	.global Func_0200173c
	.thumb_func
Func_0200173c:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #192
	lsls r2, r2, #4
	adds r2, #162
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	beq .L_0200976c
	movs r0, #131
	lsls r0, r0, #1
	bl GameFlag_SetBit
	bl Func_02002118
	bl Func_02002148
	movs r0, #131
	lsls r0, r0, #1
	bl GameFlag_ClearBit
.L_0200976c:
	pop {pc}
	.2byte 0x0000
	.section .text.x02009770,"ax",%progbits
	.global Func_02001770
	.thumb_func
Func_02001770:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, .L_02009830
	movs r0, #133
	lsls r0, r0, #2
	adds r3, r3, r0
	ldr r0, [r3]
	sub sp, #40
	bl ObjectTable_Get
	adds r7, r0, #0
.L_02009790:
	bl Func_0200173c
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r7, #48]
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r7, #52]
	movs r3, #128
	lsls r3, r3, #24
	str r3, [r7, #56]
	str r3, [r7, #64]
	movs r3, #0
	str r3, [r7, #36]
	str r3, [r7, #44]
	ldr r1, .L_02009834
	ldr r3, [r7, #8]
	add r2, sp, #28
	mov r10, r2
	movs r2, #128
	lsls r2, r2, #12
	ands r3, r1
	adds r3, r3, r2
	mov r0, r10
	str r3, [sp, #12]
	str r3, [r0]
	ldr r3, [r7, #12]
	str r3, [r0, #4]
	ldr r3, [r7, #16]
	ands r3, r1
	adds r3, r3, r2
	str r3, [sp, #8]
	str r3, [r0, #8]
	ldr r2, [sp, #12]
	str r3, [sp, #16]
	adds r3, r7, #0
	adds r3, #34
	str r2, [sp, #20]
	str r3, [sp, #4]
	adds r1, r2, #0
	ldrb r0, [r3]
	ldr r2, [sp, #16]
	bl Func_02002028
	str r0, [sp, #24]
	ldr r2, [sp, #20]
	ldr r1, [r7, #8]
	ldr r0, [sp, #16]
	ldr r3, .L_02009838
	ldr r6, [r7, #16]
	subs r1, r2, r1
	subs r6, r0, r6
	mov r8, r3
	adds r0, r1, #0
	mov lr, r8
	.2byte 0xf800
	adds r1, r6, #0
	adds r5, r0, #0
	adds r0, r6, #0
	mov lr, r8
	.2byte 0xf800
	adds r5, r5, r0
	movs r0, #128
	lsls r0, r0, #11
	cmp r5, r0
	bge .L_02009848
	ldr r3, .L_0200983c
	movs r2, #15
	ldr r3, [r3]
	ldr r1, .L_02009840
	lsrs r3, r3, #4
	ands r3, r2
	lsls r3, r3, #1
	ldrsh r2, [r1, r3]
	mov r9, r2
	lsls r3, r2, #16
	ldr r2, .L_02009844
	cmp r3, r2
	bne .L_02009874
	b .L_02009a0a
.L_02009830:
	.4byte gPartyState
.L_02009834:
	.4byte 0xfff00000
.L_02009838:
	.4byte IwramMulQ16
.L_0200983c:
	.4byte gInput
.L_02009840:
	.4byte Data_020022b6
.L_02009844:
	.4byte 0xffff0000
.L_02009848:
	ldr r3, [sp, #16]
	ldr r2, [sp, #20]
	ldr r0, [r7, #16]
	ldr r1, [r7, #8]
	subs r0, r3, r0
	subs r1, r2, r1
	bl ArcTan2
	lsls r0, r0, #16
	asrs r0, r0, #16
	movs r3, #128
	ldr r2, .L_02009870
	mov r9, r0
	lsls r3, r3, #6
	add r3, r9
	ands r3, r2
	lsls r3, r3, #16
	asrs r3, r3, #16
	mov r9, r3
	b .L_02009874
.L_02009870:
	.4byte 0xffffc000
.L_02009874:
	movs r0, #128
	add r2, sp, #28
	lsls r0, r0, #13
	mov r1, r9
	bl Vector_AddPolarOffsetFar
	ldr r3, [sp, #4]
	mov r2, r10
	ldr r1, [r2]
	ldrb r0, [r3]
	ldr r2, [r2, #8]
	bl Func_02002028
	mov r11, r0
	cmp r0, #255
	beq .L_020098f6
	ldr r3, [sp, #4]
	mov r2, r10
	ldrb r0, [r3]
	ldr r1, [r2]
	ldr r2, [r2, #8]
	bl Map_GetTerrainHeight
	ldr r3, [r7, #12]
	subs r0, r0, r3
	movs r3, #128
	lsls r3, r3, #12
	cmp r0, r3
	bgt .L_020098f6
	ldr r0, [sp, #12]
	mov r2, r10
	str r0, [r2]
	ldr r3, [sp, #8]
	adds r0, r7, #0
	str r3, [r2, #8]
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r7, #48]
	movs r3, #200
	lsls r3, r3, #5
	adds r3, #153
	adds r2, r7, #0
	str r3, [r7, #52]
	adds r2, #100
	movs r3, #0
	strh r3, [r2]
	ldr r1, [sp, #12]
	ldr r3, [sp, #8]
	ldr r2, [r7, #12]
	bl Func_02002008
	adds r0, r7, #0
	movs r1, #2
	bl Func_02001fe0
	adds r0, r7, #0
	movs r1, #48
	bl ObjectDispatch_ApplyValueToChildren
	adds r0, r7, #0
	bl Func_02002010
	ldr r3, .L_02009a18
	str r3, [r7, #108]
	b .L_020099a0
.L_020098f6:
	movs r3, #0
	mov r0, r9
	strh r0, [r7, #6]
	str r3, [r7, #36]
	str r3, [r7, #44]
	ldr r2, [sp, #12]
	str r2, [r7, #8]
	ldr r3, [sp, #8]
	str r3, [r7, #16]
	b .L_020099ec
.L_0200990a:
	ldr r2, [sp, #4]
	ldr r1, [r6]
	ldrb r0, [r2]
	ldr r2, [r6, #8]
	bl Map_GetTerrainHeight
	ldr r3, [r7, #12]
	subs r0, r0, r3
	movs r3, #128
	lsls r3, r3, #12
	cmp r0, r3
	bgt .L_020099c0
	ldrh r3, [r7, #32]
	movs r2, #89
	subs r3, #2
	str r3, [sp, #0]
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #20]
	movs r0, #0
	adds r2, r2, r5
	mov r10, r0
	mov r8, r2
.L_02009938:
	ldr r3, [r5]
	cmp r3, #0
	beq .L_02009962
	mov r3, r8
	ldrb r2, [r3]
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	beq .L_02009962
	cmp r5, r7
	beq .L_02009962
	ldrh r3, [r5, #32]
	adds r0, r5, #0
	adds r0, #8
	subs r3, #2
	ldr r1, [sp, #0]
	add r2, sp, #28
	bl Func_02002050
	cmp r0, #0
	bge .L_020099c0
.L_02009962:
	movs r0, #1
	add r10, r0
	movs r2, #128
	mov r3, r10
	add r8, r2
	adds r5, #128
	cmp r3, #63
	ble .L_02009938
	ldr r0, [r6]
	movs r3, #128
	str r0, [sp, #20]
	lsls r3, r3, #10
	ldr r2, [r6, #8]
	adds r0, r7, #0
	str r2, [sp, #16]
	str r3, [r7, #48]
	movs r3, #200
	lsls r3, r3, #5
	adds r3, #153
	str r3, [r7, #52]
	ldr r3, [r6, #8]
	ldr r1, [r6]
	ldr r2, [r6, #4]
	bl Func_02002008
	adds r0, r7, #0
	bl Func_02002010
	ldr r3, [sp, #24]
	cmp r11, r3
	bne .L_020099e6
.L_020099a0:
	movs r0, #128
	add r2, sp, #28
	lsls r0, r0, #13
	mov r1, r9
	bl Vector_AddPolarOffsetFar
	ldr r2, [sp, #4]
	add r6, sp, #28
	ldrb r0, [r2]
	ldr r1, [r6]
	ldr r2, [r6, #8]
	bl Func_02002028
	mov r11, r0
	cmp r0, #255
	bne .L_0200990a
.L_020099c0:
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r7, #48]
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r7, #52]
	ldr r2, [r7, #12]
	adds r0, r7, #0
	ldr r1, [sp, #20]
	ldr r3, [sp, #16]
	bl Func_02002008
	adds r0, r7, #0
	bl Func_02002010
	movs r0, #2
	bl WaitFrames
	b .L_02009790
.L_020099e6:
	movs r0, #10
	bl WaitFrames
.L_020099ec:
	movs r3, #0
	str r3, [r7, #108]
	adds r1, r7, #0
	adds r1, #90
	ldrb r2, [r1]
	movs r3, #1
	orrs r3, r2
	strb r3, [r1]
	movs r3, #128
	lsls r3, r3, #7
	str r3, [r7, #52]
	adds r0, r7, #0
	movs r1, #1
	bl Func_02001fe0
.L_02009a0a:
	add sp, #40
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_02009a18:
	.4byte Func_020016b0
	.section .text.x02009a1c,"ax",%progbits
	.global Func_02001a1c
	.thumb_func
Func_02001a1c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, .L_02009a7c
	movs r0, #133
	lsls r0, r0, #2
	adds r3, r3, r0
	ldr r0, [r3]
	sub sp, #40
	bl ObjectTable_Get
	ldr r2, .L_02009a80
	ldr r3, .L_02009a78
	adds r7, r0, #0
	strh r3, [r2]
.L_02009a42:
	bl Func_0200173c
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r7, #48]
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r7, #52]
	movs r3, #128
	lsls r3, r3, #24
	str r3, [r7, #56]
	str r3, [r7, #64]
	movs r3, #0
	str r3, [r7, #36]
	str r3, [r7, #44]
	ldr r1, .L_02009a84
	ldr r3, [r7, #8]
	add r2, sp, #28
	mov r10, r2
	movs r2, #128
	lsls r2, r2, #12
	ands r3, r1
	adds r3, r3, r2
	mov r0, r10
	str r3, [sp, #12]
	str r3, [r0]
	b .L_02009a88
.L_02009a78:
	.4byte 0x00000000
.L_02009a7c:
	.4byte gPartyState
.L_02009a80:
	.4byte gOverlayArea + 0x2ce8
.L_02009a84:
	.4byte 0xfff00000
.L_02009a88:
	ldr r3, [r7, #12]
	str r3, [r0, #4]
	ldr r3, [r7, #16]
	ands r3, r1
	adds r3, r3, r2
	str r3, [sp, #8]
	str r3, [r0, #8]
	ldr r2, [sp, #12]
	str r3, [sp, #16]
	adds r3, r7, #0
	adds r3, #34
	str r2, [sp, #20]
	str r3, [sp, #4]
	adds r1, r2, #0
	ldrb r0, [r3]
	ldr r2, [sp, #16]
	bl Func_02002028
	str r0, [sp, #24]
	ldr r2, [sp, #20]
	ldr r1, [r7, #8]
	ldr r0, [sp, #16]
	ldr r3, .L_02009af4
	ldr r6, [r7, #16]
	subs r1, r2, r1
	subs r6, r0, r6
	mov r8, r3
	adds r0, r1, #0
	mov lr, r8
	.2byte 0xf800
	adds r1, r6, #0
	adds r5, r0, #0
	adds r0, r6, #0
	mov lr, r8
	.2byte 0xf800
	adds r5, r5, r0
	movs r0, #128
	lsls r0, r0, #11
	cmp r5, r0
	bge .L_02009b04
	ldr r3, .L_02009af8
	movs r2, #15
	ldr r3, [r3]
	ldr r1, .L_02009afc
	lsrs r3, r3, #4
	ands r3, r2
	lsls r3, r3, #1
	ldrsh r2, [r1, r3]
	mov r9, r2
	lsls r3, r2, #16
	ldr r2, .L_02009b00
	cmp r3, r2
	bne .L_02009b30
	b .L_02009cfa
.L_02009af4:
	.4byte IwramMulQ16
.L_02009af8:
	.4byte gInput
.L_02009afc:
	.4byte Data_020022b6
.L_02009b00:
	.4byte 0xffff0000
.L_02009b04:
	ldr r3, [sp, #16]
	ldr r2, [sp, #20]
	ldr r0, [r7, #16]
	ldr r1, [r7, #8]
	subs r0, r3, r0
	subs r1, r2, r1
	bl ArcTan2
	lsls r0, r0, #16
	asrs r0, r0, #16
	movs r3, #128
	ldr r2, .L_02009b2c
	mov r9, r0
	lsls r3, r3, #6
	add r3, r9
	ands r3, r2
	lsls r3, r3, #16
	asrs r3, r3, #16
	mov r9, r3
	b .L_02009b30
.L_02009b2c:
	.4byte 0xffffc000
.L_02009b30:
	movs r0, #128
	add r2, sp, #28
	lsls r0, r0, #13
	mov r1, r9
	bl Vector_AddPolarOffsetFar
	ldr r3, [sp, #4]
	mov r2, r10
	ldr r1, [r2]
	ldrb r0, [r3]
	ldr r2, [r2, #8]
	bl Func_02002028
	mov r11, r0
	cmp r0, #255
	beq .L_02009baa
	ldr r3, [sp, #4]
	mov r2, r10
	ldrb r0, [r3]
	ldr r1, [r2]
	ldr r2, [r2, #8]
	bl Map_GetTerrainHeight
	ldr r3, [r7, #12]
	subs r0, r0, r3
	movs r3, #128
	lsls r3, r3, #12
	cmp r0, r3
	bgt .L_02009baa
	ldr r0, [sp, #12]
	mov r2, r10
	str r0, [r2]
	ldr r3, [sp, #8]
	adds r0, r7, #0
	str r3, [r2, #8]
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r7, #48]
	movs r3, #200
	lsls r3, r3, #5
	adds r3, #153
	adds r2, r7, #0
	str r3, [r7, #52]
	adds r2, #100
	movs r3, #0
	strh r3, [r2]
	ldr r1, [sp, #12]
	ldr r2, [r7, #12]
	ldr r3, [sp, #8]
	bl Func_02002008
	adds r0, r7, #0
	movs r1, #2
	bl Func_02001fe0
	adds r0, r7, #0
	movs r1, #48
	bl ObjectDispatch_ApplyValueToChildren
	movs r5, #0
	b .L_02009bd2
.L_02009baa:
	movs r3, #0
	mov r0, r9
	strh r0, [r7, #6]
	str r3, [r7, #36]
	str r3, [r7, #44]
	ldr r2, [sp, #12]
	str r2, [r7, #8]
	ldr r3, [sp, #8]
	str r3, [r7, #16]
	b .L_02009cfa
.L_02009bbe:
	ldr r3, .L_02009d28
	ldrh r3, [r3]
	lsls r3, r3, #16
	cmp r3, #0
	beq .L_02009bca
	b .L_02009cfa
.L_02009bca:
	movs r0, #1
	bl WaitFrames
	adds r5, #1
.L_02009bd2:
	cmp r5, #179
	bgt .L_02009be0
	adds r0, r7, #0
	bl Func_02002048
	cmp r0, #0
	beq .L_02009bbe
.L_02009be0:
	ldr r3, .L_02009d2c
	str r3, [r7, #108]
	b .L_02009cae
.L_02009be6:
	ldr r2, [sp, #4]
	ldr r1, [r6]
	ldrb r0, [r2]
	ldr r2, [r6, #8]
	bl Map_GetTerrainHeight
	ldr r3, [r7, #12]
	subs r0, r0, r3
	movs r3, #128
	lsls r3, r3, #12
	cmp r0, r3
	bgt .L_02009cce
	ldr r3, .L_02009d28
	ldrh r3, [r3]
	lsls r3, r3, #16
	cmp r3, #0
	bne .L_02009cfa
	ldrh r3, [r7, #32]
	movs r2, #89
	subs r3, #2
	str r3, [sp, #0]
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #20]
	movs r0, #0
	adds r2, r2, r5
	mov r10, r0
	mov r8, r2
.L_02009c1e:
	ldr r3, [r5]
	cmp r3, #0
	beq .L_02009c48
	mov r3, r8
	ldrb r2, [r3]
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	beq .L_02009c48
	cmp r5, r7
	beq .L_02009c48
	ldrh r3, [r5, #32]
	adds r0, r5, #0
	adds r0, #8
	subs r3, #2
	ldr r1, [sp, #0]
	add r2, sp, #28
	bl Func_02002050
	cmp r0, #0
	bge .L_02009cce
.L_02009c48:
	movs r0, #1
	add r10, r0
	movs r2, #128
	mov r3, r10
	add r8, r2
	adds r5, #128
	cmp r3, #63
	ble .L_02009c1e
	ldr r0, [r6]
	movs r3, #128
	str r0, [sp, #20]
	lsls r3, r3, #10
	ldr r2, [r6, #8]
	adds r0, r7, #0
	str r2, [sp, #16]
	str r3, [r7, #48]
	movs r3, #200
	lsls r3, r3, #5
	adds r3, #153
	str r3, [r7, #52]
	movs r5, #0
	ldr r1, [r6]
	ldr r2, [r6, #4]
	ldr r3, [r6, #8]
	bl Func_02002008
	b .L_02009c86
.L_02009c7e:
	movs r0, #1
	bl WaitFrames
	adds r5, #1
.L_02009c86:
	cmp r5, #179
	bgt .L_02009c9e
	adds r0, r7, #0
	bl Func_02002048
	cmp r0, #0
	bne .L_02009c9e
	ldr r3, .L_02009d28
	ldrh r3, [r3]
	lsls r3, r3, #16
	cmp r3, #0
	beq .L_02009c7e
.L_02009c9e:
	ldr r3, .L_02009d28
	ldrh r3, [r3]
	lsls r3, r3, #16
	cmp r3, #0
	bne .L_02009cfa
	ldr r3, [sp, #24]
	cmp r11, r3
	bne .L_02009cf4
.L_02009cae:
	movs r0, #128
	add r2, sp, #28
	lsls r0, r0, #13
	mov r1, r9
	bl Vector_AddPolarOffsetFar
	ldr r2, [sp, #4]
	add r6, sp, #28
	ldrb r0, [r2]
	ldr r1, [r6]
	ldr r2, [r6, #8]
	bl Func_02002028
	mov r11, r0
	cmp r0, #255
	bne .L_02009be6
.L_02009cce:
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r7, #48]
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r7, #52]
	ldr r2, [r7, #12]
	adds r0, r7, #0
	ldr r1, [sp, #20]
	ldr r3, [sp, #16]
	bl Func_02002008
	adds r0, r7, #0
	bl Func_02002010
	movs r0, #2
	bl WaitFrames
	b .L_02009a42
.L_02009cf4:
	movs r0, #10
	bl WaitFrames
.L_02009cfa:
	movs r3, #0
	str r3, [r7, #108]
	adds r1, r7, #0
	adds r1, #90
	ldrb r2, [r1]
	movs r3, #1
	orrs r3, r2
	strb r3, [r1]
	movs r3, #128
	lsls r3, r3, #7
	str r3, [r7, #52]
	adds r0, r7, #0
	movs r1, #1
	bl Func_02001fe0
	add sp, #40
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009d28:
	.4byte gOverlayArea + 0x2ce8
.L_02009d2c:
	.4byte Func_020016b0
	.section .text.x02009d30,"ax",%progbits
	.global Func_02001d30
	.thumb_func
Func_02001d30:
	ldr r3, .L_02009d38
	strh r0, [r3]
	bx lr
	.2byte 0x0000
.L_02009d38:
	.4byte gOverlayArea + 0x2ce8
	.section .text.x02009d3c,"ax",%progbits
	.global Func_02001d3c
	.thumb_func
Func_02001d3c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, .L_02009da8
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	ldr r0, [r3]
	sub sp, #32
	bl ObjectTable_Get
	adds r5, r0, #0
	ldrh r3, [r5, #6]
	movs r2, #128
	lsls r2, r2, #6
	adds r3, r3, r2
	ldr r2, .L_02009da4
	ands r3, r2
	lsls r3, r3, #16
	asrs r3, r3, #16
	str r3, [sp, #16]
.L_02009d6e:
	bl Func_0200173c
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r5, #48]
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r5, #52]
	movs r3, #128
	lsls r3, r3, #24
	str r3, [r5, #56]
	str r3, [r5, #64]
	movs r3, #0
	str r3, [r5, #36]
	str r3, [r5, #44]
	ldr r2, .L_02009dac
	ldr r3, [r5, #8]
	movs r1, #128
	lsls r1, r1, #12
	ands r3, r2
	mov r9, r1
	add r6, sp, #20
	add r3, r9
	str r3, [r6]
	mov r8, r3
	b .L_02009db0
	.2byte 0x0000
.L_02009da4:
	.4byte 0xffffc000
.L_02009da8:
	.4byte gPartyState
.L_02009dac:
	.4byte 0xfff00000
.L_02009db0:
	ldr r3, [r5, #12]
	str r3, [r6, #4]
	ldr r3, [r5, #16]
	ands r3, r2
	adds r7, r3, r1
	mov r2, r8
	str r7, [r6, #8]
	str r2, [sp, #8]
	str r7, [sp, #4]
	movs r3, #34
	adds r3, r3, r5
	ldrb r0, [r3]
	adds r1, r2, #0
	adds r2, r7, #0
	mov r11, r3
	bl Func_02002028
	str r0, [sp, #12]
	movs r0, #128
	ldr r1, [sp, #16]
	lsls r0, r0, #13
	adds r2, r6, #0
	bl Vector_AddPolarOffsetFar
	mov r1, r11
	ldrb r0, [r1]
	ldr r2, [r6, #8]
	ldr r1, [r6]
	bl Func_02002028
	mov r10, r0
	cmp r0, #255
	beq .L_02009e44
	mov r2, r11
	ldrb r0, [r2]
	ldr r1, [r6]
	ldr r2, [r6, #8]
	bl Map_GetTerrainHeight
	ldr r3, [r5, #12]
	subs r0, r0, r3
	cmp r0, r9
	bgt .L_02009e44
	ldr r3, [sp, #8]
	ldr r2, .L_02009e3c
	str r3, [r6]
	ldr r1, [sp, #4]
	movs r3, #128
	lsls r3, r3, #10
	str r1, [r6, #8]
	str r3, [r5, #48]
	movs r3, #200
	lsls r3, r3, #5
	adds r3, #153
	str r3, [r5, #52]
	adds r3, r5, #0
	adds r3, #100
	strh r2, [r3]
	adds r0, r5, #0
	movs r1, #2
	bl Func_02001fe0
	adds r0, r5, #0
	movs r1, #48
	bl ObjectDispatch_ApplyValueToChildren
	ldr r3, .L_02009e40
	str r3, [r5, #108]
	b .L_02009eee
	.2byte 0x0000
.L_02009e3c:
	.4byte 0x00000000
.L_02009e40:
	.4byte Func_020016b0
.L_02009e44:
	add r1, sp, #16
	ldrh r1, [r1]
	movs r3, #0
	mov r2, r8
	strh r1, [r5, #6]
	str r3, [r5, #36]
	str r3, [r5, #44]
	str r2, [r5, #8]
	str r7, [r5, #16]
	b .L_02009f3a
.L_02009e58:
	mov r3, r11
	ldrb r0, [r3]
	ldr r1, [r7]
	ldr r2, [r7, #8]
	bl Map_GetTerrainHeight
	ldr r3, [r5, #12]
	movs r1, #128
	subs r0, r0, r3
	lsls r1, r1, #12
	cmp r0, r1
	bgt .L_02009f0e
	ldrh r3, [r5, #32]
	movs r2, #0
	subs r3, #2
	str r3, [sp, #0]
	movs r3, #192
	lsls r3, r3, #18
	ldr r6, [r3, #20]
	movs r3, #89
	adds r3, r3, r6
	mov r9, r2
	mov r8, r3
.L_02009e86:
	ldr r3, [r6]
	cmp r3, #0
	beq .L_02009eb0
	mov r1, r8
	ldrb r2, [r1]
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	beq .L_02009eb0
	cmp r6, r5
	beq .L_02009eb0
	ldrh r3, [r6, #32]
	adds r0, r6, #0
	adds r0, #8
	subs r3, #2
	ldr r1, [sp, #0]
	add r2, sp, #20
	bl Func_02002050
	cmp r0, #0
	bge .L_02009f0e
.L_02009eb0:
	movs r2, #1
	add r9, r2
	movs r3, #128
	mov r1, r9
	add r8, r3
	adds r6, #128
	cmp r1, #63
	ble .L_02009e86
	ldr r2, [r7]
	adds r0, r5, #0
	str r2, [sp, #8]
	ldr r3, [r7, #8]
	str r3, [sp, #4]
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r5, #48]
	movs r3, #200
	lsls r3, r3, #5
	adds r3, #153
	str r3, [r5, #52]
	ldr r1, [r7]
	ldr r2, [r7, #4]
	ldr r3, [r7, #8]
	bl Func_02002008
	adds r0, r5, #0
	bl Func_02002010
	ldr r1, [sp, #12]
	cmp r10, r1
	bne .L_02009f34
.L_02009eee:
	movs r0, #128
	ldr r1, [sp, #16]
	add r2, sp, #20
	lsls r0, r0, #13
	bl Vector_AddPolarOffsetFar
	mov r2, r11
	add r7, sp, #20
	ldrb r0, [r2]
	ldr r1, [r7]
	ldr r2, [r7, #8]
	bl Func_02002028
	mov r10, r0
	cmp r0, #255
	bne .L_02009e58
.L_02009f0e:
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r5, #48]
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r5, #52]
	ldr r2, [r5, #12]
	adds r0, r5, #0
	ldr r1, [sp, #8]
	ldr r3, [sp, #4]
	bl Func_02002008
	adds r0, r5, #0
	bl Func_02002010
	movs r0, #2
	bl WaitFrames
	b .L_02009d6e
.L_02009f34:
	movs r0, #10
	bl WaitFrames
.L_02009f3a:
	movs r3, #0
	str r3, [r5, #108]
	adds r1, r5, #0
	adds r1, #90
	ldrb r2, [r1]
	movs r3, #1
	orrs r3, r2
	strb r3, [r1]
	movs r3, #128
	lsls r3, r3, #7
	str r3, [r5, #52]
	adds r0, r5, #0
	movs r1, #1
	bl Func_02001fe0
	add sp, #32
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .rodata.x0200a1b8,"a",%progbits
.L_0200a1b8:
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
.L_0200a1f4:
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
.L_0200a230:
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
	.global Data_0200226c
Data_0200226c:
	.4byte 0x000a0009
	.2byte 0xffff
	.global Data_02002272
Data_02002272:
	.2byte 0x000a
	.2byte 0xffff
	.global Data_02002276
Data_02002276:
	.2byte 0xffff
	.4byte 0x80000000
	.4byte 0xc000ffff
	.4byte 0xa000e000
	.4byte 0x4000c000
	.4byte 0x60002000
	.4byte 0xffff4000
	.4byte 0x80000000
	.4byte 0xffffffff
	.4byte 0x80000000
	.4byte 0xc000ffff
	.4byte 0xffffffff
	.4byte 0x4000c000
	.4byte 0xffffffff
	.4byte 0xffff4000
	.4byte 0x80000000
	.2byte 0xffff
	.global Data_020022b6
Data_020022b6:
	.2byte 0xffff
	.4byte 0x80000000
	.4byte 0xc000ffff
	.4byte 0x80000000
	.4byte 0x4000c000
	.4byte 0x80000000
	.4byte 0xffff4000
	.4byte 0x80000000
	.4byte 0x0000ffff
	.global Data_020022d8
Data_020022d8:
	.4byte .L_0200a1b8
	.4byte .L_0200a1f4
	.4byte .L_0200a230
.L_0200a2e4:
	.4byte 0x00000027
	.4byte 0x00000003
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000000
	.4byte 0x00000017
	.4byte 0x00000006
	.4byte 0xfff00000
	.4byte 0x0000002e
	.4byte Func_020002b0
	.4byte 0x00000011
	.global Data_02002310
Data_02002310:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000011
.L_0200a31c:
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000000
	.4byte 0x00000011
	.global Data_0200232c
Data_0200232c:
	.4byte 0x0000002e
	.4byte Func_020002bc
	.4byte 0x00000011
	.global gSceneEntrances
gSceneEntrances:
	.4byte 0xffff0000
	.4byte 0x000000d8
	.4byte 0x400000d8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0063
	.4byte 0x00000140
	.4byte 0xc000033e
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02002380
Data_02002380:
	.4byte 0x002e02a0
	.4byte 0x02b00190
	.4byte 0x01a0003e
	.4byte 0x000effff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020023a0
Data_020023a0:
	.4byte 0x000c00d4
	.4byte 0x00dc0084
	.4byte 0x008c0014
	.4byte 0x0001ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gSceneExits
gSceneExits:
	.4byte 0x000000d5
	.4byte 0x10130002
	.4byte 0xffffffff
	.4byte 0x1020b0d6
	.4byte 0xffffffff
	.4byte 0x103010d7
	.4byte 0xffffffff
	.4byte 0x104010d7
	.4byte 0xffffffff
	.4byte 0x000000d6
	.4byte 0x101020d5
	.4byte 0xffffffff
	.4byte 0x000000d7
	.4byte 0x101030d5
	.4byte 0xffffffff
	.4byte 0x102040d8
	.4byte 0xffffffff
	.4byte 0x000000d8
	.4byte 0x10131002
	.4byte 0xffffffff
	.4byte 0x1020b0d9
	.4byte 0xffffffff
	.4byte 0x1030e0d9
	.4byte 0xffffffff
	.4byte 0x1040e0d9
	.4byte 0xffffffff
	.4byte 0x1050e0d9
	.4byte 0xffffffff
	.4byte 0x106020da
	.4byte 0xffffffff
	.4byte 0x000000da
	.4byte 0x102040d8
	.4byte 0xffffffff
	.4byte 0x000000d9
	.4byte 0x10b020d8
	.4byte 0xffffffff
	.4byte 0x10c0d0d9
	.4byte 0xffffffff
	.4byte 0x10d0c0d9
	.4byte 0xffffffff
	.4byte 0x10e030d8
	.4byte 0xffffffff
	.4byte 0x000000db
	.4byte 0x10132002
	.4byte 0xffffffff
	.4byte 0x102010dc
	.4byte 0xffffffff
	.4byte 0x000000dc
	.4byte 0x101020db
	.4byte 0xffffffff
	.4byte 0x000000dd
	.4byte 0x1012f002
	.4byte 0xffffffff
	.4byte 0x1020b0de
	.4byte 0xffffffff
	.4byte 0x000000de
	.4byte 0x101020dd
	.4byte 0xffffffff
	.4byte 0x000001ff
	.global Data_020024ac
Data_020024ac:
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x03280000
	.4byte 0x0000c000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020024dc
Data_020024dc:
	.4byte 0xffff0073
	.4byte 0x00000002
	.4byte 0x00c00000
	.4byte 0x00000000
	.4byte 0x00f00000
	.4byte 0x0001c000
	.4byte 0xffff006a
	.4byte 0x00000002
	.4byte 0x01500000
	.4byte 0x00000000
	.4byte 0x00f00000
	.4byte 0x0001c000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02002524
Data_02002524:
	.4byte 0xffff0074
	.4byte 0x00000003
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x00014000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02002554
Data_02002554:
	.4byte 0xffff0072
	.4byte 0x00000002
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x0001c000
	.4byte 0xffff012b
	.4byte 0x00000007
	.4byte 0x00380000
	.4byte 0x00000000
	.4byte 0x00f00000
	.4byte 0x0002c000
	.4byte 0xffff011f
	.4byte .L_0200a31c
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x0002c000
	.4byte 0x006600f5
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00008000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020025cc
Data_020025cc:
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x02980000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00014000
	.4byte 0xffff006a
	.4byte 0x00000003
	.4byte 0x02b80000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x00014000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02002614
Data_02002614:
	.4byte 0xffff005d
	.4byte 0x00000001
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x00b00000
	.4byte 0x00014000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00014000
	.4byte 0xffff0120
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00014000
	.4byte 0xffff0112
	.4byte .L_0200a2e4
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x00480000
	.4byte 0x00014000
	.4byte 0xffff0112
	.4byte .L_0200a2e4
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x01014000
	.4byte 0xffff0112
	.4byte .L_0200a2e4
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x01014000
	.4byte 0xffff0112
	.4byte .L_0200a2e4
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x01014000
	.4byte 0xffff0112
	.4byte .L_0200a2e4
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x01014000
	.4byte 0xffff0112
	.4byte .L_0200a2e4
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x01014000
	.4byte 0xffff0112
	.4byte .L_0200a2e4
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x01014000
	.4byte 0xffff0112
	.4byte .L_0200a2e4
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00014000
	.4byte 0xffff0112
	.4byte .L_0200a2e4
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x01014000
	.4byte 0xffff0112
	.4byte .L_0200a2e4
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00014000
	.4byte 0xffff0112
	.4byte .L_0200a2e4
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00014000
	.4byte 0xffff0149
	.4byte 0x00000001
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x005c0000
	.4byte 0x00014000
	.4byte 0x007f00f6
	.4byte 0x00000001
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00008000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020027ac
Data_020027ac:
	.4byte 0xffff005d
	.4byte 0x00000001
	.4byte 0x00600000
	.4byte 0x00000000
	.4byte 0x03300000
	.4byte 0x00014000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020027dc
Data_020027dc:
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x0002c000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x0002c000
	.4byte 0x005200f4
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00008000
	.4byte 0xffff01e9
	.4byte 0x00000007
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x01500000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02002854
Data_02002854:
	.4byte 0xffff004b
	.4byte 0x00000001
	.4byte 0x00b00000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00014000
	.4byte 0xffff004c
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x0001c000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_0200289c
Data_0200289c:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x00000009
	.4byte 0x00000001
	.4byte 0xffff000c
	.4byte 0x00000009
	.4byte 0x00000001
	.4byte 0xffff000d
	.4byte 0x00000009
	.4byte 0x00000001
	.4byte 0xffff000e
	.4byte 0x00000009
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020028f0
Data_020028f0:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00004602
	.4byte 0xffff0003
	.4byte Func_02000624
	.4byte 0x00000602
	.4byte 0xffff0004
	.4byte Func_020005cc
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte Func_0200043c
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x000028b2
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x000028b3
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x000028b5
	.4byte 0x50008905
	.4byte 0xffff000a
	.4byte Func_02000488
	.4byte 0x50008905
	.4byte 0xffff000b
	.4byte Func_020004a8
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02002974
Data_02002974:
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x000028b1
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x000028b4
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020029a4
Data_020029a4:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x0000c602
	.4byte 0xffff0002
	.4byte Func_02000c44
	.4byte 0x00004602
	.4byte 0xffff0003
	.4byte Func_02000624
	.4byte 0x00000602
	.4byte 0xffff0004
	.4byte Func_020005cc
	.4byte 0x00008602
	.4byte 0xffff0005
	.4byte Func_020005f4
	.4byte 0x0000c401
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000002
	.4byte 0x02200022
	.4byte Func_0200125c
	.4byte 0x00000002
	.4byte 0x12200023
	.4byte Func_020012c0
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x000028b6
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x000028b9
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte Func_02000290
	.4byte 0x10008c15
	.4byte 0xffff000a
	.4byte Func_02001304
	.4byte 0x00008c15
	.4byte 0xffff000a
	.4byte Func_0200155c
	.4byte 0x00000008
	.4byte 0xffff0000
	.4byte Func_02001304
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte Func_0200155c
	.4byte 0x00000c15
	.4byte 0x02300009
	.4byte Func_02000644
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02002a70
Data_02002a70:
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00000021
	.4byte 0xffff000c
	.4byte 0x0000000c
	.4byte 0x00000031
	.4byte 0xffff000d
	.4byte 0x0000000d
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x000028b7
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x000028b8
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x000028ba
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x000028bb
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02002ad0
Data_02002ad0:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x0000c602
	.4byte 0xffff0002
	.4byte Func_02000c44
	.4byte 0x00000002
	.4byte 0xffff000a
	.4byte Func_02000c1c
	.4byte 0x00000002
	.4byte 0xffff000b
	.4byte Func_02000c2c
	.4byte 0x00000002
	.4byte 0x02010014
	.4byte Func_02000c3c
	.4byte 0x00008515
	.4byte 0x02030008
	.4byte Func_02000cfc
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte Func_02000280
	.4byte 0x00009415
	.4byte 0x0f5d000b
	.4byte Func_02000404
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02002b3c
Data_02002b3c:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x000028bc
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x000028bd
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x000028be
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x000028bf
	.4byte 0x00000173
	.4byte 0xffff00c8
	.4byte 0x0040305e
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02002b90
Data_02002b90:
	.4byte 0x0000c401
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x0000c401
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x000028c1
	.4byte 0x00000000
	.4byte 0xffff0017
	.4byte Func_020002a0
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x000028c3
	.4byte 0x10008c15
	.4byte 0x02040009
	.4byte Func_02000ef0
	.4byte 0x00008c15
	.4byte 0x02040009
	.4byte Func_020011bc
	.4byte 0x10008c15
	.4byte 0x0205000a
	.4byte Func_02000ef0
	.4byte 0x00008c15
	.4byte 0x0205000a
	.4byte Func_020011c8
	.4byte 0x00000008
	.4byte 0xffff0000
	.4byte Func_02000f00
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte Func_020011d4
	.4byte 0x00008f15
	.4byte 0x02080016
	.4byte Func_020016a4
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02002c2c
Data_02002c2c:
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x000028c0
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x000028c2
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02002c5c
Data_02002c5c:
	.4byte 0xffffffff
