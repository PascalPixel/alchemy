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
	bl Func_02005174
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
	bl Func_02005164
	ldr r2, .L_02008274
	mov r3, r8
	ands r3, r5
	lsls r3, r3, #2
	ldr r1, [r2, r3]
	adds r0, r6, #0
	mov r10, r3
	bl Func_0200516c
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
	bl Func_02005244
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
	bl Func_02005164
	ldr r1, [r7, #28]
	adds r0, r6, #0
	bl Func_0200516c
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
	.4byte Data_02005520
.L_02008278:
	.4byte Func_02000080
.L_0200827c:
	.4byte 0xffff0000
	.section .text.x02008280,"ax",%progbits
	.global Func_02000280
	.thumb_func
Func_02000280:
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	movs r0, #160
	lsls r0, r0, #1
	adds r1, r3, r0
	subs r0, #56
	adds r2, r3, r0
	ldr r3, [r2, #16]
	str r3, [r1, #16]
	ldr r3, [r2, #20]
	str r3, [r1, #20]
	ldrh r3, [r2, #40]
	strh r3, [r1, #40]
	ldrh r3, [r2, #42]
	strh r3, [r1, #42]
	bx lr
	.2byte 0x0000
	.section .text.x020082a4,"ax",%progbits
	.global Func_020002a4
	.thumb_func
Func_020002a4:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, .L_02008380
	movs r2, #4
	negs r2, r2
	ands r3, r2
	ldr r3, [r3, #4]
	movs r2, #192
	lsls r2, r2, #18
	mov r11, r3
	ldr r3, [r2, #32]
	movs r0, #160
	lsls r0, r0, #1
	adds r1, r3, r0
	ldr r3, [r2, #108]
	movs r2, #192
	lsls r2, r2, #4
	adds r2, #164
	adds r3, r3, r2
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	ldr r5, .L_02008384
	cmp r3, #0
	bne .L_02008374
	str r3, [r1, #28]
	ldr r3, .L_02008388
	movs r6, #0
	ldrb r2, [r3]
	movs r3, #6
	ldrsh r0, [r1, r3]
	movs r3, #2
	ldrsh r4, [r1, r3]
	movs r3, #204
	lsls r3, r3, #6
	muls r3, r0
	lsls r2, r2, #10
	movs r1, #153
	adds r3, r3, r2
	lsls r1, r1, #5
	mov r10, r3
	adds r3, r0, #0
	muls r3, r1
	mov r9, r4
	subs r7, r3, r2
	cmp r6, #160
	beq .L_02008340
.L_0200830c:
	mov r0, r10
	mov lr, r11
	.2byte 0xf800
	mov r8, r0
	adds r0, r7, #0
	mov lr, r11
	.2byte 0xf800
	mov r4, r8
	lsls r2, r0, #3
	lsls r3, r4, #1
	subs r2, r2, r0
	add r3, r8
	adds r3, r3, r2
	asrs r3, r3, #19
	movs r0, #204
	movs r2, #153
	add r3, r9
	lsls r0, r0, #6
	lsls r2, r2, #5
	adds r6, #1
	strh r3, [r5]
	add r10, r0
	adds r5, #2
	adds r7, r7, r2
	cmp r6, #160
	bne .L_0200830c
.L_02008340:
	ldr r5, .L_02008384
	movs r1, #128
	ldrh r3, [r5]
	lsls r1, r1, #19
	adds r1, #24
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
	adds r0, r5, #2
	ldrh r2, [r3, #10]
	ldr r2, .L_0200838c
	stmia r3!, {r0, r1, r2}
	subs r3, #12
.L_02008374:
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_02008380:
	.4byte Math_Sine
.L_02008384:
	.4byte gOverlayArea + 0x6340
.L_02008388:
	.4byte Data_0300122c
.L_0200838c:
	.4byte 0xa2600001
	.section .text.x02008390,"ax",%progbits
	.global Func_02000390
	.thumb_func
Func_02000390:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #128
	ldr r3, [r3]
	ldr r7, .L_020083d4
	adds r0, r3, #0
	movs r5, #0
	adds r0, #48
	movs r4, #4
	movs r6, #0
.L_020083a6:
	movs r3, #160
	lsls r3, r3, #19
	lsls r1, r5, #1
	adds r3, #48
	adds r1, r1, r3
	subs r3, r4, #2
	ldrh r2, [r7, r4]
	ldrh r3, [r7, r3]
	lsls r2, r2, #10
	lsls r3, r3, #5
	orrs r2, r3
	ldrh r3, [r7, r6]
	adds r5, #1
	orrs r2, r3
	strh r2, [r1]
	adds r4, #6
	ldrh r3, [r1]
	adds r6, #6
	strh r3, [r0]
	adds r0, #2
	cmp r5, #7
	bls .L_020083a6
	pop {r5, r6, r7, pc}
.L_020083d4:
	.4byte Data_020054ee
	.section .text.x020083d8,"ax",%progbits
	.global Func_020003d8
	.thumb_func
Func_020003d8:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #128
	ldr r3, [r3]
	ldr r7, .L_0200841c
	adds r0, r3, #0
	movs r5, #0
	adds r0, #194
	movs r4, #4
	movs r6, #0
.L_020083ee:
	movs r3, #160
	lsls r3, r3, #19
	lsls r1, r5, #1
	adds r3, #194
	adds r1, r1, r3
	subs r3, r4, #2
	ldrh r2, [r7, r4]
	ldrh r3, [r7, r3]
	lsls r2, r2, #10
	lsls r3, r3, #5
	orrs r2, r3
	ldrh r3, [r7, r6]
	adds r5, #1
	orrs r2, r3
	strh r2, [r1]
	adds r4, #6
	ldrh r3, [r1]
	adds r6, #6
	strh r3, [r0]
	adds r0, #2
	cmp r5, #8
	bls .L_020083ee
	pop {r5, r6, r7, pc}
.L_0200841c:
	.4byte Data_020054b8
	.section .text.x02008420,"ax",%progbits
	.global Func_02000420
	.thumb_func
Func_02000420:
	push {lr}
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #0
	pop {pc}
	.section .text.x0200842c,"ax",%progbits
	.global Func_0200042c
	.thumb_func
Func_0200042c:
	ldr r0, .L_02008430
	bx lr
.L_02008430:
	.4byte Data_02005574
	.section .text.x02008434,"ax",%progbits
	.global Func_02000434
	.thumb_func
Func_02000434:
	movs r0, #0
	bx lr
	.section .text.x02008438,"ax",%progbits
	.global Func_02000438
	.thumb_func
Func_02000438:
	ldr r0, .L_0200843c
	bx lr
.L_0200843c:
	.4byte Data_020055a4
	.section .text.x02008440,"ax",%progbits
	.global Func_02000440
	.thumb_func
Func_02000440:
	push {lr}
	ldr r3, .L_02008498
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_0200849c
	cmp r2, r3
	bne .L_0200846a
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #27
	bl Func_0200514c
	cmp r0, #0
	beq .L_02008466
	ldr r0, .L_020084a0
	b .L_02008494
.L_02008466:
	ldr r0, .L_020084a4
	b .L_02008494
.L_0200846a:
	ldr r3, .L_020084a8
	cmp r2, r3
	bne .L_02008474
	ldr r0, .L_020084ac
	b .L_02008494
.L_02008474:
	ldr r3, .L_020084b0
	cmp r2, r3
	bne .L_0200847e
	ldr r0, .L_020084b4
	b .L_02008494
.L_0200847e:
	ldr r3, .L_020084b8
	cmp r2, r3
	bne .L_02008488
	ldr r0, .L_020084bc
	b .L_02008494
.L_02008488:
	ldr r3, .L_020084c0
	cmp r2, r3
	bne .L_02008492
	ldr r0, .L_020084c4
	b .L_02008494
.L_02008492:
	ldr r0, .L_020084c8
.L_02008494:
	pop {pc}
	.2byte 0x0000
.L_02008498:
	.4byte gPartyState
.L_0200849c:
	.4byte 0x0000003b
.L_020084a0:
	.4byte Data_02005874
.L_020084a4:
	.4byte Data_020056f4
.L_020084a8:
	.4byte 0x0000003c
.L_020084ac:
	.4byte Data_02005b5c
.L_020084b0:
	.4byte 0x0000003d
.L_020084b4:
	.4byte Data_020059dc
.L_020084b8:
	.4byte 0x0000003e
.L_020084bc:
	.4byte Data_02005a54
.L_020084c0:
	.4byte 0x0000003f
.L_020084c4:
	.4byte Data_02005c1c
.L_020084c8:
	.4byte Data_020056dc
	.section .text.x020084cc,"ax",%progbits
	.global Func_020004cc
	.thumb_func
Func_020004cc:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r7, [r3, #108]
	bl Func_020051ac
	movs r0, #0
	bl Func_02005324
	movs r5, #8
.L_020084e0:
	adds r0, r5, #0
	bl Object_GetById
	cmp r0, #0
	beq .L_020084f2
	adds r2, r0, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
.L_020084f2:
	adds r5, #1
	cmp r5, #63
	bls .L_020084e0
	movs r2, #170
	lsls r2, r2, #1
	adds r3, r7, r2
	movs r2, #0
	ldrsh r6, [r3, r2]
	movs r0, #158
	bl Func_02005394
	subs r6, #1
	ldr r0, .L_02008578
	lsls r4, r6, #3
	adds r3, r4, #4
	ldrh r1, [r0, r3]
	adds r3, r3, r0
	ldrh r2, [r3, #2]
	ldr r0, [r0, r4]
	bl Func_0200517c
	ldr r5, .L_0200857c
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	ldr r0, [r5]
	movs r1, #2
	bl Object_SetModeById
	movs r1, #128
	movs r2, #128
	ldr r0, [r5]
	lsls r1, r1, #8
	lsls r2, r2, #7
	bl ObjectMotion_SetSpeedParameters
	ldr r0, [r5]
	cmp r6, #5
	bne .L_0200854c
	movs r2, #4
	movs r1, #0
	negs r2, r2
	bl ObjectMotion_OffsetPositionAndResetMotion
	b .L_02008556
.L_0200854c:
	movs r2, #4
	movs r1, #2
	negs r2, r2
	bl ObjectMotion_SnapHeadingAndOffset
.L_02008556:
	movs r0, #6
	bl Battle_WaitMode0
	movs r2, #170
	lsls r2, r2, #1
	adds r3, r7, r2
	movs r2, #0
	ldrsh r0, [r3, r2]
	bl Func_020052c4
	bl Event_ClearStatus1c6
	bl Event_WaitValue1c8Frames
	bl Func_020051b4
	pop {r5, r6, r7, pc}
.L_02008578:
	.4byte Data_02005de0
.L_0200857c:
	.4byte gPartyState
	.section .text.x02008580,"ax",%progbits
	.global Func_02000580
	.thumb_func
Func_02000580:
	push {r5, r6, lr}
	ldr r5, .L_020085c8
	adds r6, r0, #0
	adds r0, r5, #0
	bl Func_0200524c
	movs r1, #0
	adds r0, r6, #0
	bl Func_02005254
	bl Func_02005384
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_020085b0
	movs r0, #10
	bl Battle_WaitMode0
	adds r0, r5, #1
	bl Func_0200524c
	b .L_020085bc
.L_020085b0:
	movs r0, #20
	bl Battle_WaitMode0
	adds r0, r5, #2
	bl Func_0200524c
.L_020085bc:
	adds r0, r6, #0
	movs r1, #0
	bl Func_02005264
	pop {r5, r6, pc}
	.2byte 0x0000
.L_020085c8:
	.4byte 0x00001974
	.section .text.x020085cc,"ax",%progbits
	.global Func_020005cc
	.thumb_func
Func_020005cc:
	push {r5, r6, lr}
	ldr r5, .L_02008614
	adds r6, r0, #0
	adds r0, r5, #0
	bl Func_0200524c
	movs r1, #0
	adds r0, r6, #0
	bl Func_02005254
	bl Func_02005384
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_020085fc
	movs r0, #10
	bl Battle_WaitMode0
	adds r0, r5, #1
	bl Func_0200524c
	b .L_02008608
.L_020085fc:
	movs r0, #20
	bl Battle_WaitMode0
	adds r0, r5, #2
	bl Func_0200524c
.L_02008608:
	adds r0, r6, #0
	movs r1, #0
	bl Func_02005264
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02008614:
	.4byte 0x0000197c
	.section .text.x02008618,"ax",%progbits
	.global Func_02000618
	.thumb_func
Func_02000618:
	push {r5, r6, lr}
	ldr r5, .L_02008660
	adds r6, r0, #0
	adds r0, r5, #0
	bl Func_0200524c
	movs r1, #0
	adds r0, r6, #0
	bl Func_02005254
	bl Func_02005384
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_02008648
	movs r0, #10
	bl Battle_WaitMode0
	adds r0, r5, #1
	bl Func_0200524c
	b .L_02008654
.L_02008648:
	movs r0, #20
	bl Battle_WaitMode0
	adds r0, r5, #2
	bl Func_0200524c
.L_02008654:
	adds r0, r6, #0
	movs r1, #0
	bl Func_02005264
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02008660:
	.4byte 0x000019fd
	.section .text.x02008664,"ax",%progbits
	.global Func_02000664
	.thumb_func
Func_02000664:
	push {r5, lr}
	adds r5, r0, #0
	bl Func_020051ac
	movs r0, #0
	bl Func_02005324
	ldr r0, .L_02008694
	bl Func_0200524c
	adds r0, r5, #0
	movs r1, #0
	movs r2, #10
	bl Func_0200525c
	adds r0, r5, #0
	movs r1, #0
	movs r2, #10
	bl Func_0200525c
	bl Func_020051b4
	pop {r5, pc}
	.2byte 0x0000
.L_02008694:
	.4byte 0x000019fb
	.section .text.x02008698,"ax",%progbits
	.global Func_02000698
	.thumb_func
Func_02000698:
	push {r5, lr}
	adds r5, r0, #0
	bl Func_020051ac
	movs r0, #0
	bl Func_02005324
	movs r0, #192
	lsls r0, r0, #2
	bl Func_0200514c
	ldr r3, .L_020086e8
	cmp r0, #0
	beq .L_020086bc
	adds r0, r3, #0
	bl Func_0200524c
	b .L_020086d6
.L_020086bc:
	adds r0, r3, #0
	bl Func_0200524c
	adds r0, r5, #0
	movs r1, #0
	movs r2, #10
	bl Func_0200525c
	adds r0, r5, #0
	movs r1, #0
	movs r2, #10
	bl Func_0200525c
.L_020086d6:
	adds r0, r5, #0
	movs r1, #0
	movs r2, #10
	bl Func_0200525c
	bl Func_020051b4
	pop {r5, pc}
	.2byte 0x0000
.L_020086e8:
	.4byte 0x00001978
	.section .text.x020086ec,"ax",%progbits
	.global Func_020006ec
	.thumb_func
Func_020006ec:
	push {lr}
	bl Func_020051ac
	movs r0, #0
	bl Func_02005324
	movs r0, #13
	bl ObjectMotion_EnableActionAndResetMotion
	movs r1, #1
	movs r0, #13
	bl Object_SetModeById
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #13
	movs r1, #4
	movs r2, #0
	bl Func_0200523c
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #1
	bl Func_0200514c
	cmp r0, #0
	beq .L_0200872c
	ldr r0, .L_02008798
	bl Func_0200524c
	b .L_02008774
.L_0200872c:
	ldr r0, .L_0200879c
	bl Func_0200524c
	movs r0, #13
	movs r1, #0
	movs r2, #10
	bl Func_0200525c
	movs r1, #2
	adds r1, #255
	movs r2, #15
	movs r0, #4
	bl Func_0200528c
	movs r1, #131
	lsls r1, r1, #1
	movs r2, #10
	movs r0, #13
	bl Func_0200528c
	movs r0, #13
	movs r1, #0
	movs r2, #5
	bl Func_0200525c
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #15
	movs r0, #13
	bl Func_0200528c
	movs r0, #13
	movs r1, #0
	movs r2, #5
	bl Func_0200525c
.L_02008774:
	movs r2, #10
	movs r1, #0
	movs r0, #13
	bl Func_0200525c
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #1
	bl Func_02005154
	movs r0, #13
	movs r1, #2
	bl ObjectMotion_EnableActionAndSetCallback
	bl Func_020051b4
	pop {pc}
	.2byte 0x0000
.L_02008798:
	.4byte 0x00001a04
.L_0200879c:
	.4byte 0x00001a01
	.section .text.x020087a0,"ax",%progbits
	.global Func_020007a0
	.thumb_func
Func_020007a0:
	push {r5, lr}
	mov r5, r9
	push {r5}
	sub sp, #4
	mov r3, r9
	adds r5, r0, #0
	movs r1, #1
	movs r0, #144
	str r3, [sp, #0]
	bl Func_02005304
	movs r1, #0
	adds r0, r5, #0
	bl Func_0200530c
	movs r0, #1
	bl Func_020052fc
	bl Func_02005314
	bl Func_0200531c
	add sp, #4
	pop {r3}
	mov r9, r3
	pop {r5, pc}
	.section .text.x020087d4,"ax",%progbits
	.global Func_020007d4
	.thumb_func
Func_020007d4:
	push {r5, lr}
	mov r5, r9
	push {r5}
	sub sp, #4
	mov r3, r9
	adds r5, r0, #0
	movs r1, #1
	movs r0, #144
	str r3, [sp, #0]
	bl Func_02005304
	movs r1, #0
	adds r0, r5, #0
	bl Func_0200530c
	movs r0, #3
	bl Func_020052fc
	bl Func_02005314
	bl Func_0200531c
	add sp, #4
	pop {r3}
	mov r9, r3
	pop {r5, pc}
	.section .text.x02008808,"ax",%progbits
	.global Func_02000808
	.thumb_func
Func_02000808:
	push {r5, r6, lr}
	mov r6, r9
	push {r6}
	movs r0, #200
	lsls r0, r0, #2
	bl Func_0200514c
	adds r6, r0, #0
	cmp r6, #0
	beq .L_0200881e
	b .L_02008950
.L_0200881e:
	movs r0, #200
	lsls r0, r0, #2
	bl Func_02005154
	bl Func_020051ac
	movs r0, #0
	bl Func_02005324
	movs r0, #22
	bl Object_GetById
	movs r5, #128
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	lsls r5, r5, #7
	movs r1, #180
	movs r2, #199
	movs r0, #22
	lsls r1, r1, #17
	lsls r2, r2, #16
	adds r3, r5, #0
	bl Func_02005214
	movs r1, #1
	movs r2, #220
	negs r1, r1
	lsls r2, r2, #16
	movs r3, #1
	ldr r0, .L_02008958
	bl Func_020052ac
	bl Func_020052b4
	movs r0, #22
	mov r9, sp
	bl Func_020007a0
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #180
	movs r2, #199
	adds r3, r5, #0
	movs r0, #21
	lsls r1, r1, #17
	lsls r2, r2, #16
	bl Func_02005214
	movs r2, #16
	movs r1, #0
	movs r0, #21
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #22
	mov r9, sp
	bl Func_020007d4
	ldr r0, .L_0200895c
	bl Func_0200524c
	movs r1, #160
	movs r0, #21
	lsls r1, r1, #7
	bl Func_02005274
	movs r1, #2
	movs r0, #21
	bl Motion_SetVarCbAndRefresh
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #224
	movs r0, #4
	lsls r1, r1, #8
	movs r2, #0
	bl Func_0200526c
	movs r0, #21
	movs r1, #0
	movs r2, #10
	bl Func_0200525c
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #50
	movs r0, #21
	bl Func_0200528c
	movs r0, #21
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	strb r3, [r0]
	adds r1, r5, #0
	movs r2, #0
	movs r0, #21
	bl Func_0200526c
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #22
	mov r9, sp
	bl Func_020007a0
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #16
	movs r1, #0
	negs r2, r2
	movs r0, #21
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #21
	bl Object_GetById
	movs r2, #16
	adds r0, #85
	strb r6, [r0]
	movs r1, #0
	movs r0, #21
	negs r2, r2
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #0
	movs r2, #0
	movs r0, #21
	bl Func_0200520c
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #22
	mov r9, sp
	bl Func_020007d4
	movs r0, #60
	bl Battle_WaitMode0
	movs r0, #22
	movs r1, #0
	movs r2, #0
	bl Func_0200520c
	bl Func_020051b4
.L_02008950:
	pop {r3}
	mov r9, r3
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02008958:
	.4byte 0x01670000
.L_0200895c:
	.4byte 0x00001984
	.section .text.x02008960,"ax",%progbits
	.global Func_02000960
	.thumb_func
Func_02000960:
	push {r5, r6, lr}
	movs r5, #192
	lsls r5, r5, #18
	movs r0, #123
	ldr r6, [r5, #108]
	bl Func_02005394
	ldr r3, [r5, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #93
	str r2, [r3]
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r2, r2
	negs r1, r1
	movs r3, #0
	negs r0, r0
	bl Func_020052ac
	movs r0, #34
	adds r0, #255
	bl Func_02005154
	movs r3, #170
	lsls r3, r3, #1
	adds r6, r6, r3
	movs r2, #0
	ldrsh r0, [r6, r2]
	bl Func_020052c4
	pop {r5, r6, pc}
	.section .text.x020089a4,"ax",%progbits
	.global Func_020009a4
	.thumb_func
Func_020009a4:
	push {lr}
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #27
	bl Func_0200514c
	cmp r0, #0
	bne .L_020089c8
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #30
	bl Func_0200514c
	cmp r0, #0
	bne .L_020089da
	bl Func_02002694
	b .L_020089da
.L_020089c8:
	movs r0, #130
	lsls r0, r0, #4
	adds r0, #255
	bl Func_0200514c
	cmp r0, #0
	bne .L_020089da
	bl Func_02003878
.L_020089da:
	pop {pc}
	.section .text.x020089dc,"ax",%progbits
	.global Func_020009dc
	.thumb_func
Func_020009dc:
	push {lr}
	ldr r3, .L_02008a34
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008a38
	cmp r2, r3
	bne .L_02008a06
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #27
	bl Func_0200514c
	cmp r0, #0
	beq .L_02008a02
	ldr r0, .L_02008a3c
	b .L_02008a30
.L_02008a02:
	ldr r0, .L_02008a40
	b .L_02008a30
.L_02008a06:
	ldr r3, .L_02008a44
	cmp r2, r3
	bne .L_02008a10
	ldr r0, .L_02008a48
	b .L_02008a30
.L_02008a10:
	ldr r3, .L_02008a4c
	cmp r2, r3
	bne .L_02008a1a
	ldr r0, .L_02008a50
	b .L_02008a30
.L_02008a1a:
	ldr r3, .L_02008a54
	cmp r2, r3
	bne .L_02008a24
	ldr r0, .L_02008a58
	b .L_02008a30
.L_02008a24:
	ldr r3, .L_02008a5c
	cmp r2, r3
	bne .L_02008a2e
	ldr r0, .L_02008a60
	b .L_02008a30
.L_02008a2e:
	ldr r0, .L_02008a64
.L_02008a30:
	pop {pc}
	.2byte 0x0000
.L_02008a34:
	.4byte gPartyState
.L_02008a38:
	.4byte 0x0000003b
.L_02008a3c:
	.4byte Data_02005f78
.L_02008a40:
	.4byte Data_02005e34
.L_02008a44:
	.4byte 0x0000003c
.L_02008a48:
	.4byte Data_020060d4
.L_02008a4c:
	.4byte 0x0000003d
.L_02008a50:
	.4byte Data_0200611c
.L_02008a54:
	.4byte 0x0000003e
.L_02008a58:
	.4byte Data_020061d0
.L_02008a5c:
	.4byte 0x0000003f
.L_02008a60:
	.4byte Data_02006278
.L_02008a64:
	.4byte Data_02005e10
	.section .text.x02008a68,"ax",%progbits
	.global Func_02000a68
	.thumb_func
Func_02000a68:
	push {lr}
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #27
	bl Func_0200514c
	cmp r0, #0
	bne .L_02008aa2
	movs r0, #196
	lsls r0, r0, #2
	bl Func_0200514c
	cmp r0, #0
	beq .L_02008aa2
	ldr r3, .L_02008aa4
	ldr r2, [r3]
	adds r2, #1
	str r2, [r3]
	movs r3, #192
	lsls r3, r3, #2
	adds r3, #255
	ands r3, r2
	cmp r3, #0
	bne .L_02008aa2
	movs r0, #135
	lsls r0, r0, #1
	adds r0, #255
	bl Func_02005394
.L_02008aa2:
	pop {pc}
.L_02008aa4:
	.4byte Data_0200632c
	.section .text.x02008aa8,"ax",%progbits
	.global Func_02000aa8
	.thumb_func
Func_02000aa8:
	push {r5, r6, r7, lr}
	ldr r6, .L_02008bc4
	movs r2, #240
	lsls r2, r2, #1
	adds r1, r6, r2
	movs r3, #0
	ldrsh r2, [r1, r3]
	ldr r3, .L_02008bc8
	cmp r2, r3
	bne .L_02008ad0
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r1, #214
	lsls r1, r1, #1
	movs r2, #129
	adds r3, r3, r1
	lsls r2, r2, #2
	str r2, [r3]
	b .L_02008bc0
.L_02008ad0:
	ldr r3, .L_02008bcc
	cmp r2, r3
	bne .L_02008b5e
	movs r0, #13
	movs r1, #1
	bl Func_02005284
	movs r0, #14
	movs r1, #1
	bl Func_02005284
	movs r0, #130
	lsls r0, r0, #4
	adds r0, #255
	bl Func_0200514c
	cmp r0, #0
	beq .L_02008b06
	movs r3, #128
	movs r1, #164
	movs r2, #162
	lsls r3, r3, #7
	movs r0, #10
	lsls r1, r1, #17
	lsls r2, r2, #16
	bl Func_02005214
.L_02008b06:
	movs r5, #192
	lsls r5, r5, #18
	ldr r3, [r5, #108]
	movs r2, #214
	lsls r2, r2, #1
	movs r7, #129
	adds r3, r3, r2
	lsls r7, r7, #2
	str r7, [r3]
	movs r0, #0
	bl Func_020052f4
	movs r1, #241
	lsls r1, r1, #1
	adds r3, r6, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #6
	bne .L_02008bc0
	ldr r3, [r5, #108]
	movs r2, #133
	subs r1, #54
	lsls r2, r2, #1
	adds r3, r3, r1
	adds r2, #255
	movs r0, #144
	str r2, [r3]
	lsls r0, r0, #1
	bl Func_0200515c
	movs r0, #34
	adds r0, #255
	bl Func_0200515c
	ldr r3, [r5, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	str r7, [r3]
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	b .L_02008bc0
.L_02008b5e:
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	str r2, [r3]
	movs r3, #0
	ldrsh r2, [r1, r3]
	ldr r3, .L_02008bd0
	cmp r2, r3
	bne .L_02008b88
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #25
	bl Func_02005154
	bl Func_02000be4
	b .L_02008bc0
.L_02008b88:
	ldr r3, .L_02008bd4
	cmp r2, r3
	bne .L_02008b94
	bl Func_02000d30
	b .L_02008bc0
.L_02008b94:
	ldr r3, .L_02008bd8
	cmp r2, r3
	bne .L_02008bc0
	bl Func_02000e00
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #25
	bl Func_0200514c
	cmp r0, #0
	beq .L_02008bc0
	ldr r3, .L_02008bdc
	movs r1, #242
	lsls r1, r1, #1
	adds r2, r6, r1
	strh r3, [r2]
	movs r3, #243
	lsls r3, r3, #1
	adds r2, r6, r3
	movs r3, #2
	strh r3, [r2]
.L_02008bc0:
	movs r0, #0
	pop {r5, r6, r7, pc}
.L_02008bc4:
	.4byte gPartyState
.L_02008bc8:
	.4byte 0x0000003e
.L_02008bcc:
	.4byte 0x0000003f
.L_02008bd0:
	.4byte 0x0000003b
.L_02008bd4:
	.4byte 0x0000003c
.L_02008bd8:
	.4byte 0x0000003d
.L_02008bdc:
	.4byte 0x00000042
	.section .text.x02008be0,"ax",%progbits
	.global Func_02000be0
	.thumb_func
Func_02000be0:
	movs r0, #0
	bx lr
	.section .text.x02008be4,"ax",%progbits
	.global Func_02000be4
	.thumb_func
Func_02000be4:
	push {r5, r6, lr}
	movs r1, #144
	ldr r0, .L_02008c80
	lsls r1, r1, #3
	sub sp, #8
	bl Func_0200512c
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	movs r0, #144
	adds r3, r3, r2
	lsls r0, r0, #4
	movs r2, #0
	str r2, [r3]
	adds r0, #27
	bl Func_0200514c
	cmp r0, #0
	beq .L_02008c1c
	movs r0, #196
	lsls r0, r0, #2
	bl Func_0200514c
	cmp r0, #0
	beq .L_02008c84
.L_02008c1c:
	movs r0, #196
	lsls r0, r0, #2
	bl Func_02005154
	movs r0, #128
	lsls r0, r0, #9
	adds r0, #7
	movs r1, #1
	bl Func_020052cc
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #17
	bl Func_0200514c
	cmp r0, #0
	beq .L_02008c4a
	movs r0, #128
	lsls r0, r0, #9
	adds r0, #3
	movs r1, #2
	bl Func_020052cc
.L_02008c4a:
	movs r0, #1
	bl Func_020052d4
	bl Event_WaitValue1c8Frames
	bl Func_0200538c
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #128
	ldr r1, [r3]
	movs r2, #160
	ldr r3, .L_02008c7c
	lsls r2, r2, #19
	adds r2, #60
	strh r3, [r2]
	movs r0, #128
	ldrh r3, [r2]
	lsls r0, r0, #9
	strh r3, [r1, #60]
	movs r1, #1
	bl Func_020052cc
	b .L_02008ce6
	.2byte 0x0000
.L_02008c7c:
	.4byte 0x00007fff
.L_02008c80:
	.4byte Func_02000a68
.L_02008c84:
	movs r5, #1
	movs r0, #10
	movs r1, #32
	movs r2, #10
	movs r3, #10
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl Func_02005184
	movs r0, #12
	movs r1, #32
	movs r2, #25
	movs r3, #22
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl Func_02005184
	movs r0, #11
	movs r1, #33
	movs r2, #8
	movs r3, #22
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl Func_02005184
	movs r0, #74
	movs r1, #32
	movs r2, #74
	movs r3, #10
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl Func_02005184
	movs r0, #76
	movs r1, #32
	movs r2, #89
	movs r3, #22
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl Func_02005184
	movs r0, #75
	movs r1, #33
	movs r2, #72
	movs r3, #22
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl Func_02005184
.L_02008ce6:
	ldr r3, .L_02008d2c
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #12
	bne .L_02008d26
	movs r5, #192
	lsls r5, r5, #18
	ldr r2, [r5, #108]
	movs r3, #133
	movs r6, #214
	lsls r3, r3, #1
	adds r3, #255
	lsls r6, r6, #1
	movs r0, #144
	str r3, [r2, r6]
	lsls r0, r0, #1
	bl Func_0200515c
	movs r0, #34
	adds r0, #255
	bl Func_0200515c
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	ldr r2, [r5, #108]
	movs r3, #0
	str r3, [r2, r6]
.L_02008d26:
	add sp, #8
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02008d2c:
	.4byte gPartyState
	.section .text.x02008d30,"ax",%progbits
	.global Func_02000d30
	.thumb_func
Func_02000d30:
	push {r5, lr}
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_02008df0
	sub sp, #8
	bl Func_0200512c
	bl Func_0200535c
	movs r0, #0
	movs r1, #10
	movs r2, #11
	bl Func_02005364
	movs r0, #196
	lsls r0, r0, #2
	bl Func_0200514c
	cmp r0, #0
	beq .L_02008da6
	ldr r3, .L_02008df4
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #12
	bne .L_02008d72
	ldr r0, .L_02008df8
	movs r1, #1
	bl Func_020052cc
	b .L_02008d7e
.L_02008d72:
	movs r0, #128
	lsls r0, r0, #9
	adds r0, #7
	movs r1, #1
	bl Func_020052cc
.L_02008d7e:
	movs r0, #1
	bl Func_020052d4
	bl Event_WaitValue1c8Frames
	bl Func_0200538c
	bl Func_020003d8
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #1
	bl Func_020052cc
	movs r1, #200
	ldr r0, .L_02008dfc
	lsls r1, r1, #4
	bl Func_0200512c
	b .L_02008dea
.L_02008da6:
	movs r5, #5
	movs r0, #20
	movs r1, #7
	movs r2, #9
	movs r3, #7
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl Func_02005184
	movs r0, #20
	movs r1, #71
	movs r2, #9
	movs r3, #71
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl Func_02005184
	movs r0, #84
	movs r1, #7
	movs r2, #73
	movs r3, #7
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl Func_02005184
	bl Func_02000390
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #1
	bl Func_020052cc
	bl Func_02000280
.L_02008dea:
	add sp, #8
	pop {r5, pc}
	.2byte 0x0000
.L_02008df0:
	.4byte Func_02000a68
.L_02008df4:
	.4byte gPartyState
.L_02008df8:
	.4byte 0x002048c9
.L_02008dfc:
	.4byte Func_020002a4
	.section .text.x02008e00,"ax",%progbits
	.global Func_02000e00
	.thumb_func
Func_02000e00:
	push {lr}
	movs r1, #144
	ldr r0, .L_02008e50
	lsls r1, r1, #3
	bl Func_0200512c
	movs r0, #196
	lsls r0, r0, #2
	bl Func_0200514c
	cmp r0, #0
	beq .L_02008e3e
	movs r0, #198
	lsls r0, r0, #2
	bl Func_0200514c
	cmp r0, #0
	beq .L_02008e32
	movs r0, #128
	lsls r0, r0, #9
	adds r0, #3
	movs r1, #1
	bl Func_020052cc
	b .L_02008e3e
.L_02008e32:
	movs r0, #128
	lsls r0, r0, #9
	adds r0, #7
	movs r1, #1
	bl Func_020052cc
.L_02008e3e:
	ldr r0, .L_02008e54
	bl Func_0200536c
	movs r0, #1
	bl Func_020052d4
	bl Event_WaitValue1c8Frames
	pop {pc}
.L_02008e50:
	.4byte Func_02000a68
.L_02008e54:
	.4byte Data_02005450
	.section .text.x02008e58,"ax",%progbits
	.global Func_02000e58
	.thumb_func
Func_02000e58:
	push {lr}
	ldr r0, .L_02008e64
	bl Func_02005374
	pop {pc}
	.2byte 0x0000
.L_02008e64:
	.4byte Data_02005450
	.section .text.x02008e68,"ax",%progbits
	.global Func_02000e68
	.thumb_func
Func_02000e68:
	push {lr}
	ldr r0, .L_02008e74
	bl Func_02005374
	pop {pc}
	.2byte 0x0000
.L_02008e74:
	.4byte Data_02005450
	.section .text.x02008e78,"ax",%progbits
	.global Func_02000e78
	.thumb_func
Func_02000e78:
	push {r5, r6, r7, lr}
	adds r5, r1, #0
	adds r2, r5, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	adds r7, r0, #0
	movs r6, #16
.L_02008e88:
	ldr r3, [r5, #12]
	ldr r2, .L_02008eac
	movs r0, #1
	adds r3, r3, r2
	str r3, [r5, #12]
	subs r6, #1
	bl Battle_WaitMode0
	cmp r6, #0
	bgt .L_02008e88
	movs r0, #136
	bl Func_02005394
	adds r0, r7, #0
	bl Func_020012a8
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008eac:
	.4byte 0xfffe0000
	.section .text.x02008eb0,"ax",%progbits
	.global Func_02000eb0
	.thumb_func
Func_02000eb0:
	push {r5, r6, r7, lr}
	movs r0, #8
	bl Object_GetById
	adds r5, r0, #0
	ldr r3, [r5, #8]
	movs r0, #129
	asrs r7, r3, #20
	ldr r3, [r5, #16]
	lsls r0, r0, #1
	adds r0, #255
	asrs r6, r3, #20
	bl Func_0200514c
	cmp r0, #0
	bne .L_02008eea
	cmp r7, #10
	bne .L_02008eea
	cmp r6, #29
	bne .L_02008eea
	movs r0, #8
	adds r1, r5, #0
	bl Func_02000e78
	movs r0, #129
	lsls r0, r0, #1
	adds r0, #255
	bl Func_02005154
.L_02008eea:
	pop {r5, r6, r7, pc}
	.section .text.x02008eec,"ax",%progbits
	.global Func_02000eec
	.thumb_func
Func_02000eec:
	push {r5, r6, r7, lr}
	movs r0, #9
	bl Object_GetById
	adds r5, r0, #0
	ldr r3, [r5, #8]
	movs r0, #128
	asrs r7, r3, #20
	ldr r3, [r5, #16]
	lsls r0, r0, #2
	adds r0, #2
	asrs r6, r3, #20
	bl Func_0200514c
	cmp r0, #0
	bne .L_02008f26
	cmp r7, #15
	bne .L_02008f26
	cmp r6, #28
	bne .L_02008f26
	movs r0, #9
	adds r1, r5, #0
	bl Func_02000e78
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #2
	bl Func_02005154
.L_02008f26:
	pop {r5, r6, r7, pc}
	.section .text.x02008f28,"ax",%progbits
	.global Func_02000f28
	.thumb_func
Func_02000f28:
	push {lr}
	movs r0, #10
	bl Object_GetById
	movs r0, #130
	lsls r0, r0, #1
	adds r0, #255
	bl Func_0200514c
	pop {pc}
	.section .text.x02008f3c,"ax",%progbits
	.global Func_02000f3c
	.thumb_func
Func_02000f3c:
	push {r5, r6, r7, lr}
	movs r0, #11
	bl Object_GetById
	adds r5, r0, #0
	ldr r3, [r5, #8]
	movs r0, #129
	asrs r7, r3, #20
	ldr r3, [r5, #16]
	lsls r0, r0, #2
	asrs r6, r3, #20
	bl Func_0200514c
	cmp r0, #0
	bne .L_02008f72
	cmp r7, #15
	bne .L_02008f72
	cmp r6, #25
	bne .L_02008f72
	movs r0, #11
	adds r1, r5, #0
	bl Func_02000e78
	movs r0, #129
	lsls r0, r0, #2
	bl Func_02005154
.L_02008f72:
	pop {r5, r6, r7, pc}
	.section .text.x02008f74,"ax",%progbits
	.global Func_02000f74
	.thumb_func
Func_02000f74:
	push {lr}
	bl Func_0200537c
	bl Func_02000eb0
	pop {pc}
	.section .text.x02008f80,"ax",%progbits
	.global Func_02000f80
	.thumb_func
Func_02000f80:
	push {lr}
	bl Func_0200537c
	bl Func_02000eec
	pop {pc}
	.section .text.x02008f8c,"ax",%progbits
	.global Func_02000f8c
	.thumb_func
Func_02000f8c:
	push {lr}
	bl Func_0200537c
	bl Func_02000f28
	pop {pc}
	.section .text.x02008f98,"ax",%progbits
	.global Func_02000f98
	.thumb_func
Func_02000f98:
	push {lr}
	bl Func_0200537c
	bl Func_02000f3c
	pop {pc}
	.section .text.x02008fa4,"ax",%progbits
	.global Func_02000fa4
	.thumb_func
Func_02000fa4:
	push {lr}
	bl Func_0200537c
	bl Func_02000eb0
	bl Func_02000eec
	bl Func_02000f28
	bl Func_02000f3c
	pop {pc}
	.section .text.x02008fbc,"ax",%progbits
	.global Func_02000fbc
	.thumb_func
Func_02000fbc:
	push {lr}
	sub sp, #8
	movs r3, #1
	movs r2, #2
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #98
	movs r1, #8
	movs r2, #86
	movs r3, #8
	bl Func_02005184
	movs r3, #22
	movs r2, #11
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #34
	movs r1, #9
	movs r2, #1
	movs r3, #1
	bl Func_0200518c
	add sp, #8
	pop {pc}
	.section .text.x02008fec,"ax",%progbits
	.global Func_02000fec
	.thumb_func
Func_02000fec:
	push {lr}
	sub sp, #8
	movs r3, #1
	movs r2, #3
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #25
	movs r1, #47
	movs r2, #11
	movs r3, #33
	bl Func_02005184
	movs r3, #11
	movs r2, #33
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #25
	movs r1, #47
	movs r2, #1
	movs r3, #1
	bl Func_0200518c
	add sp, #8
	pop {pc}
	.section .text.x0200901c,"ax",%progbits
	.global Func_0200101c
	.thumb_func
Func_0200101c:
	push {r5, lr}
	sub sp, #8
	movs r5, #3
	movs r0, #44
	movs r1, #5
	movs r2, #19
	movs r3, #5
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl Func_02005184
	movs r3, #2
	str r3, [sp, #4]
	movs r0, #108
	movs r1, #5
	movs r2, #83
	movs r3, #5
	str r5, [sp, #0]
	bl Func_02005184
	movs r3, #19
	movs r2, #5
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #44
	movs r1, #5
	movs r2, #3
	movs r3, #3
	bl Func_0200518c
	add sp, #8
	pop {r5, pc}
	.section .text.x0200905c,"ax",%progbits
	.global Func_0200105c
	.thumb_func
Func_0200105c:
	push {lr}
	sub sp, #8
	movs r3, #1
	movs r2, #2
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #102
	movs r1, #8
	movs r2, #86
	movs r3, #8
	bl Func_02005184
	movs r3, #22
	movs r2, #11
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #22
	movs r1, #10
	movs r2, #1
	movs r3, #1
	bl Func_0200518c
	add sp, #8
	pop {pc}
	.section .text.x0200908c,"ax",%progbits
	.global Func_0200108c
	.thumb_func
Func_0200108c:
	push {lr}
	movs r0, #196
	lsls r0, r0, #2
	bl Func_0200514c
	pop {pc}
	.section .text.x02009098,"ax",%progbits
	.global Func_02001098
	.thumb_func
Func_02001098:
	push {lr}
	movs r0, #196
	lsls r0, r0, #2
	bl Func_0200514c
	cmp r0, #0
	beq .L_020090d2
	movs r0, #198
	lsls r0, r0, #2
	bl Func_0200514c
	cmp r0, #0
	beq .L_020090c0
	movs r0, #128
	lsls r0, r0, #9
	adds r0, #3
	movs r1, #1
	bl Func_020052cc
	b .L_020090cc
.L_020090c0:
	movs r0, #128
	lsls r0, r0, #9
	adds r0, #7
	movs r1, #1
	bl Func_020052cc
.L_020090cc:
	movs r0, #16
	bl Func_020052d4
.L_020090d2:
	pop {pc}
	.section .text.x020090d4,"ax",%progbits
	.global Func_020010d4
	.thumb_func
Func_020010d4:
	push {lr}
	sub sp, #8
	movs r3, #1
	movs r2, #3
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #8
	movs r1, #30
	movs r2, #11
	movs r3, #33
	bl Func_02005184
	movs r3, #11
	movs r2, #33
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #8
	movs r1, #30
	movs r2, #1
	movs r3, #1
	bl Func_0200518c
	add sp, #8
	pop {pc}
	.section .text.x02009104,"ax",%progbits
	.global Func_02001104
	.thumb_func
Func_02001104:
	push {r5, lr}
	sub sp, #8
	movs r5, #3
	movs r0, #39
	movs r1, #5
	movs r2, #19
	movs r3, #5
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl Func_02005184
	movs r3, #2
	str r3, [sp, #4]
	movs r0, #103
	movs r1, #5
	movs r2, #83
	movs r3, #5
	str r5, [sp, #0]
	bl Func_02005184
	movs r3, #19
	movs r2, #5
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #39
	movs r1, #5
	movs r2, #3
	movs r3, #3
	bl Func_0200518c
	add sp, #8
	pop {r5, pc}
	.section .text.x02009144,"ax",%progbits
	.global Func_02001144
	.thumb_func
Func_02001144:
	push {lr}
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #27
	bl Func_0200514c
	cmp r0, #0
	bne .L_020091c2
	movs r0, #132
	lsls r0, r0, #2
	bl Func_02005154
	ldr r3, .L_020091c4
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #1
	bne .L_0200918a
	movs r0, #128
	lsls r0, r0, #9
	adds r0, #3
	movs r1, #1
	bl Func_020052cc
	movs r0, #196
	lsls r0, r0, #2
	bl Func_02005154
	movs r0, #198
	lsls r0, r0, #2
	bl Func_02005154
	b .L_020091a4
.L_0200918a:
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #1
	bl Func_020052cc
	movs r0, #196
	lsls r0, r0, #2
	bl Func_0200515c
	movs r0, #198
	lsls r0, r0, #2
	bl Func_0200515c
.L_020091a4:
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #192
	lsls r2, r2, #4
	adds r2, #164
	adds r3, r3, r2
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	bne .L_020091c2
	movs r0, #120
	bl Func_020052d4
.L_020091c2:
	pop {pc}
.L_020091c4:
	.4byte gPartyState
	.section .text.x020091c8,"ax",%progbits
	.global Func_020011c8
	.thumb_func
Func_020011c8:
	push {lr}
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #27
	bl Func_0200514c
	cmp r0, #0
	bne .L_0200924a
	movs r0, #137
	lsls r0, r0, #1
	adds r0, #255
	bl Func_02005154
	ldr r3, .L_0200924c
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #2
	bne .L_02009210
	movs r0, #128
	lsls r0, r0, #9
	adds r0, #3
	movs r1, #1
	bl Func_020052cc
	movs r0, #196
	lsls r0, r0, #2
	bl Func_02005154
	movs r0, #198
	lsls r0, r0, #2
	bl Func_02005154
	b .L_0200922c
.L_02009210:
	movs r0, #128
	lsls r0, r0, #9
	adds r0, #7
	movs r1, #1
	bl Func_020052cc
	movs r0, #196
	lsls r0, r0, #2
	bl Func_02005154
	movs r0, #198
	lsls r0, r0, #2
	bl Func_0200515c
.L_0200922c:
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #192
	lsls r2, r2, #4
	adds r2, #164
	adds r3, r3, r2
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	bne .L_0200924a
	movs r0, #120
	bl Func_020052d4
.L_0200924a:
	pop {pc}
.L_0200924c:
	.4byte gPartyState
	.section .text.x02009250,"ax",%progbits
	.global Func_02001250
	.thumb_func
Func_02001250:
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
	bge .L_02009280
	adds r3, #15
.L_02009280:
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
	.section .text.x020092a8,"ax",%progbits
	.global Func_020012a8
	.thumb_func
Func_020012a8:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #68
	bl Object_GetById
	ldr r3, .L_02009328
	add r2, sp, #16
	str r3, [r2, #36]
	movs r3, #0
	adds r7, r0, #0
	mov r9, r2
	mov r10, r3
.L_020092c6:
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
	bls .L_020092c6
	add sp, #68
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009328:
	.4byte Func_02001250
	.section .text.x0200932c,"ax",%progbits
	.global Func_0200132c
	.thumb_func
Func_0200132c:
	push {lr}
	movs r1, #10
	movs r2, #11
	movs r0, #1
	bl Func_02005354
	movs r0, #130
	lsls r0, r0, #1
	adds r0, #255
	bl Func_02005154
	pop {pc}
	.section .text.x02009344,"ax",%progbits
	.global Func_02001344
	.thumb_func
Func_02001344:
	push {lr}
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #27
	bl Func_0200514c
	cmp r0, #0
	bne .L_0200935e
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #30
	bl Func_0200514c
.L_0200935e:
	pop {pc}
	.section .text.x02009360,"ax",%progbits
	.global Func_02001360
	.thumb_func
Func_02001360:
	push {r5, r6, r7, lr}
	adds r4, r0, #0
	adds r6, r2, #0
	adds r5, r1, #0
	lsls r3, r3, #16
	movs r0, #244
	asrs r7, r3, #16
	lsls r0, r0, #1
	adds r3, r6, #0
	adds r1, r4, #0
	adds r2, r5, #0
	bl Func_02005174
	adds r6, r0, #0
	cmp r6, #0
	beq .L_020093a4
	movs r1, #1
	ldr r5, [r6, #80]
	bl Func_02005164
	ldr r1, .L_020093ac
	adds r0, r6, #0
	bl Func_0200516c
	adds r2, r6, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	ldr r3, [sp, #16]
	ldr r1, .L_020093a8
	adds r2, #9
	strh r3, [r2]
	strb r1, [r5, #26]
	strh r7, [r5, #18]
.L_020093a4:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_020093a8:
	.4byte 0x00000000
.L_020093ac:
	.4byte Data_02006330
	.section .text.x020093b0,"ax",%progbits
	.global Func_020013b0
	.thumb_func
Func_020013b0:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	sub sp, #4
	bl Object_GetById
	adds r5, r0, #0
	ldr r0, [r5, #8]
	movs r3, #128
	lsls r3, r3, #12
	ldr r1, [r5, #12]
	adds r0, r0, r3
	movs r3, #224
	lsls r3, r3, #13
	mov r8, r3
	movs r3, #128
	ldr r2, [r5, #16]
	add r1, r8
	lsls r3, r3, #5
	movs r6, #20
	str r6, [sp, #0]
	bl Func_02001360
	movs r0, #151
	bl Func_02005394
	movs r0, #20
	bl Battle_WaitMode0
	ldr r0, [r5, #8]
	ldr r3, .L_02009414
	ldr r1, [r5, #12]
	adds r0, r0, r3
	movs r3, #240
	ldr r2, [r5, #16]
	add r1, r8
	lsls r3, r3, #8
	str r6, [sp, #0]
	bl Func_02001360
	movs r0, #151
	bl Func_02005394
	movs r0, #20
	bl Battle_WaitMode0
	add sp, #4
	pop {r3}
	mov r8, r3
	pop {r5, r6, pc}
.L_02009414:
	.4byte 0xfff80000
	.section .text.x02009418,"ax",%progbits
	.global Func_02001418
	.thumb_func
Func_02001418:
	push {r5, lr}
	adds r5, r0, #0
	movs r1, #1
	movs r0, #144
	sub sp, #8
	bl Func_02005304
	adds r0, r5, #0
	movs r1, #0
	bl Func_0200530c
	movs r0, #1
	bl Func_020052fc
	movs r3, #19
	movs r2, #5
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #44
	movs r1, #5
	movs r2, #3
	movs r3, #3
	bl Func_02005194
	bl Func_02005314
	bl Func_0200531c
	add sp, #8
	pop {r5, pc}
	.section .text.x02009454,"ax",%progbits
	.global Func_02001454
	.thumb_func
Func_02001454:
	push {r5, lr}
	adds r5, r0, #0
	movs r1, #1
	movs r0, #144
	sub sp, #8
	bl Func_02005304
	adds r0, r5, #0
	movs r1, #0
	bl Func_0200530c
	movs r0, #3
	bl Func_020052fc
	movs r3, #19
	movs r2, #5
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #39
	movs r1, #5
	movs r2, #3
	movs r3, #3
	bl Func_02005194
	bl Func_02005314
	bl Func_0200531c
	add sp, #8
	pop {r5, pc}
	.section .text.x02009490,"ax",%progbits
	.global Func_02001490
	.thumb_func
Func_02001490:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #28
	sub sp, #12
	bl Func_02005154
	bl Func_020051ac
	movs r0, #0
	bl Func_02005324
	movs r1, #192
	movs r2, #128
	lsls r1, r1, #15
	lsls r2, r2, #12
	movs r0, #8
	bl Func_0200520c
	ldr r0, .L_0200981c
	bl Func_0200524c
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #40
	movs r0, #4
	bl Func_0200528c
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #4
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r2, #152
	movs r0, #4
	movs r1, #152
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r2, #16
	movs r3, #176
	movs r0, #9
	movs r1, #32
	negs r2, r2
	lsls r3, r3, #8
	bl Func_02005334
	movs r3, #192
	movs r0, #5
	movs r1, #0
	movs r2, #0
	lsls r3, r3, #8
	bl Func_02005334
	movs r3, #176
	movs r0, #6
	movs r1, #16
	movs r2, #0
	lsls r3, r3, #8
	bl Func_02005334
	movs r0, #7
	bl Func_0200514c
	cmp r0, #0
	beq .L_0200953c
	movs r3, #192
	movs r0, #7
	movs r1, #8
	movs r2, #16
	lsls r3, r3, #8
	bl Func_02005334
.L_0200953c:
	movs r1, #16
	movs r2, #16
	movs r0, #4
	negs r1, r1
	negs r2, r2
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #4
	bl Func_0200526c
	movs r0, #9
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r0, #4
	movs r1, #0
	movs r2, #0
	bl Func_0200526c
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl Func_0200526c
	movs r1, #128
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #0
	bl Func_0200526c
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #9
	bl Func_0200526c
	movs r0, #25
	bl Battle_WaitMode0
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #2
	adds r1, #255
	movs r2, #0
	movs r0, #6
	bl Func_0200528c
	movs r1, #2
	movs r0, #9
	adds r1, #255
	movs r2, #0
	bl Func_0200528c
	movs r0, #7
	bl Func_0200514c
	cmp r0, #0
	beq .L_020095d6
	movs r1, #2
	movs r0, #7
	adds r1, #255
	movs r2, #0
	bl Func_0200528c
.L_020095d6:
	movs r1, #2
	adds r1, #255
	movs r2, #50
	movs r0, #4
	bl Func_0200528c
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r0, #7
	bl Func_0200514c
	cmp r0, #0
	beq .L_020095fe
	movs r0, #7
	movs r1, #2
	bl ObjectMotion_SetVariantCallback
.L_020095fe:
	movs r0, #6
	movs r1, #2
	bl ObjectMotion_SetVariantCallback
	movs r0, #9
	movs r1, #2
	bl ObjectMotion_SetVariantCallback
	movs r0, #4
	movs r1, #2
	bl ObjectMotion_SetVariantCallback
	movs r1, #2
	movs r0, #5
	bl Motion_SetVarCbAndRefresh
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #192
	movs r0, #4
	lsls r1, r1, #8
	movs r2, #0
	bl Func_0200526c
	movs r1, #192
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl Func_0200526c
	movs r1, #192
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #0
	bl Func_0200526c
	movs r1, #176
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #9
	bl Func_0200526c
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #144
	movs r1, #1
	movs r2, #128
	negs r1, r1
	lsls r2, r2, #16
	movs r3, #1
	lsls r0, r0, #16
	bl Func_020052ac
	bl Func_020052b4
	movs r0, #8
	bl Object_GetById
	movs r1, #2
	mov r10, r0
	movs r0, #8
	bl Object_SetModeById
	movs r6, #0
.L_02009682:
	lsls r7, r6, #13
	adds r0, r7, #0
	bl Math_Sine
	ldr r3, .L_02009820
	mov r5, sp
	muls r3, r6
	cmp r3, #0
	bge .L_02009696
	adds r3, #7
.L_02009696:
	lsls r2, r0, #1
	movs r1, #208
	lsls r1, r1, #15
	adds r2, r2, r0
	asrs r3, r3, #3
	lsls r2, r2, #2
	adds r3, r3, r1
	subs r3, r3, r2
	str r3, [r5]
	adds r0, r7, #0
	bl Math_Sine
	movs r3, #176
	lsls r3, r3, #15
	adds r2, r6, #0
	muls r2, r3
	cmp r2, #0
	bge .L_020096bc
	adds r2, #7
.L_020096bc:
	movs r3, #128
	lsls r3, r3, #14
	asrs r2, r2, #3
	adds r2, r2, r3
	lsls r3, r0, #3
	subs r3, r3, r0
	subs r2, r2, r3
	movs r3, #0
	str r2, [r5, #8]
	str r3, [r5, #4]
	bl Random16Far
	lsls r3, r0, #2
	adds r3, r3, r0
	lsls r3, r3, #1
	movs r2, #200
	lsrs r3, r3, #16
	lsls r2, r2, #5
	adds r3, #4
	adds r2, #153
	adds r1, r3, #0
	muls r1, r2
	movs r2, #192
	lsls r2, r2, #4
	adds r2, #204
	muls r2, r3
	movs r0, #8
	bl ObjectMotion_SetSpeedParameters
	ldr r1, [r5]
	cmp r1, #0
	bge .L_02009704
	movs r2, #255
	lsls r2, r2, #8
	adds r2, #255
	adds r1, r1, r2
.L_02009704:
	ldr r2, [r5, #8]
	asrs r1, r1, #16
	cmp r2, #0
	bge .L_02009714
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	adds r2, r2, r3
.L_02009714:
	asrs r2, r2, #16
	movs r0, #8
	adds r6, #1
	bl ObjectMotion_SetPositionAndReset
	cmp r6, #7
	ble .L_02009682
	movs r1, #1
	movs r0, #8
	bl Object_SetModeById
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #8
	movs r1, #2
	bl Motion_SetVarCbAndRefresh
	ldr r1, .L_02009824
	movs r0, #8
	bl ObjectMotion_EnableActionAndSetCallback
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r1, #226
	lsls r1, r1, #1
	adds r2, r2, r1
	ldrh r3, [r2]
	movs r1, #0
	subs r3, #1
	strh r3, [r2]
	movs r0, #8
	movs r2, #0
	bl Func_0200525c
	movs r0, #8
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #6
	movs r0, #8
	bl Func_0200526c
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #8
	movs r1, #1
	bl Func_0200529c
	movs r1, #2
	movs r0, #8
	bl Object_SetModeById
	mov r2, r10
	ldr r2, [r2, #8]
	mov r3, r10
	mov r8, r2
	movs r1, #128
	movs r2, #184
	ldr r7, [r3, #16]
	lsls r1, r1, #16
	lsls r2, r2, #16
	mov r9, r1
	mov r10, r2
	movs r6, #0
.L_020097a2:
	lsls r0, r6, #13
	bl Math_Sine
	mov r1, r9
	mov r2, r8
	subs r3, r1, r2
	muls r3, r6
	cmp r3, #0
	bge .L_020097b6
	adds r3, #7
.L_020097b6:
	asrs r3, r3, #3
	add r3, r8
	lsls r2, r0, #2
	subs r3, r3, r2
	mov r1, r10
	str r3, [r5]
	subs r3, r1, r7
	muls r3, r6
	cmp r3, #0
	bge .L_020097cc
	adds r3, #7
.L_020097cc:
	asrs r3, r3, #3
	adds r3, r7, r3
	str r3, [r5, #8]
	movs r3, #0
	str r3, [r5, #4]
	bl Random16Far
	lsls r3, r0, #2
	adds r3, r3, r0
	lsls r3, r3, #1
	movs r2, #200
	lsrs r3, r3, #16
	lsls r2, r2, #5
	adds r3, #4
	adds r2, #153
	adds r1, r3, #0
	muls r1, r2
	movs r2, #192
	lsls r2, r2, #4
	adds r2, #204
	muls r2, r3
	movs r0, #8
	bl ObjectMotion_SetSpeedParameters
	ldr r1, [r5]
	cmp r1, #0
	bge .L_0200980a
	movs r2, #255
	lsls r2, r2, #8
	adds r2, #255
	adds r1, r1, r2
.L_0200980a:
	ldr r2, [r5, #8]
	asrs r1, r1, #16
	cmp r2, #0
	bge .L_02009828
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	adds r2, r2, r3
	b .L_02009828
.L_0200981c:
	.4byte 0x00001952
.L_02009820:
	.4byte 0xfff00000
.L_02009824:
	.4byte Data_0200545c
.L_02009828:
	asrs r2, r2, #16
	movs r0, #8
	adds r6, #1
	bl ObjectMotion_SetPositionAndReset
	cmp r6, #7
	ble .L_020097a2
	movs r0, #8
	movs r1, #1
	bl Object_SetModeById
	movs r1, #192
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #8
	bl Func_0200526c
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #0
	movs r2, #0
	movs r0, #8
	bl Func_0200526c
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r2, #0
	movs r0, #6
	movs r1, #0
	bl Func_0200525c
	movs r1, #2
	movs r0, #8
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #192
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #8
	bl Func_0200526c
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #50
	movs r0, #8
	bl Func_0200528c
	movs r1, #153
	movs r2, #152
	lsls r1, r1, #8
	lsls r2, r2, #7
	adds r1, #153
	adds r2, #204
	movs r0, #8
	bl ObjectMotion_SetSpeedParameters
	movs r0, #8
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	strb r3, [r0]
	movs r1, #128
	movs r2, #148
	movs r0, #8
	bl ObjectMotion_SetPositionAndReset
	movs r0, #1
	bl Battle_WaitMode0
	movs r0, #8
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	movs r1, #160
	movs r2, #160
	strb r3, [r0]
	lsls r1, r1, #10
	movs r0, #8
	lsls r2, r2, #9
	bl ObjectMotion_SetSpeedParameters
	movs r1, #0
	movs r2, #4
	movs r0, #8
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #1
	movs r1, #1
	movs r2, #1
	movs r3, #0
	negs r0, r0
	negs r1, r1
	negs r2, r2
	bl Func_020052ac
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #8
	bl Func_0200526c
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #8
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #8
	bl Func_02005294
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #8
	bl Func_0200526c
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #8
	ldr r1, .L_02009cc8
	ldr r2, .L_02009ccc
	bl ObjectMotion_SetSpeedParameters
	movs r1, #24
	movs r2, #8
	movs r0, #8
	negs r1, r1
	negs r2, r2
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #8
	movs r2, #100
	negs r1, r1
	negs r2, r2
	movs r0, #8
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl Func_0200520c
	movs r0, #168
	movs r1, #1
	movs r2, #140
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #17
	lsls r0, r0, #16
	bl Func_020052ac
	bl Func_020052b4
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #4
	bl Func_0200528c
	movs r1, #129
	movs r0, #5
	lsls r1, r1, #1
	movs r2, #0
	bl Func_0200528c
	movs r0, #7
	bl Func_0200514c
	cmp r0, #0
	beq .L_020099ce
	movs r1, #129
	movs r0, #7
	lsls r1, r1, #1
	movs r2, #0
	bl Func_0200528c
.L_020099ce:
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #40
	movs r0, #6
	bl Func_0200528c
	movs r0, #9
	movs r1, #6
	movs r2, #15
	bl ObjectMotion_Launch
	movs r0, #9
	movs r1, #6
	movs r2, #23
	bl ObjectMotion_Launch
	movs r0, #9
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #224
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl Func_0200526c
	movs r1, #224
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #0
	bl Func_0200526c
	movs r0, #4
	movs r1, #0
	movs r2, #0
	bl Func_0200526c
	movs r0, #7
	bl Func_0200514c
	cmp r0, #0
	beq .L_02009a44
	movs r1, #208
	movs r0, #7
	lsls r1, r1, #8
	movs r2, #0
	bl Func_0200526c
.L_02009a44:
	movs r0, #25
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #5
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #160
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #9
	bl Func_0200526c
	movs r0, #25
	bl Battle_WaitMode0
	movs r2, #0
	movs r0, #9
	movs r1, #0
	bl Func_0200525c
	movs r1, #2
	movs r0, #6
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #6
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #9
	bl Func_0200528c
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r0, #7
	bl Func_0200514c
	cmp r0, #0
	beq .L_02009ac6
	movs r1, #2
	movs r0, #7
	adds r1, #255
	movs r2, #0
	bl Func_0200528c
.L_02009ac6:
	movs r1, #2
	adds r1, #255
	movs r2, #0
	movs r0, #6
	bl Func_0200528c
	movs r1, #2
	adds r1, #255
	movs r2, #0
	movs r0, #5
	bl Func_0200528c
	movs r1, #2
	movs r2, #50
	adds r1, #255
	movs r0, #4
	bl Func_0200528c
	movs r1, #2
	movs r0, #5
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r0, #9
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #0
	movs r0, #9
	movs r1, #0
	bl Func_0200525c
	movs r1, #2
	movs r0, #6
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #6
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #9
	bl Func_0200528c
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #2
	adds r1, #255
	movs r2, #50
	movs r0, #6
	bl Func_0200528c
	movs r0, #9
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #0
	movs r0, #9
	movs r1, #0
	bl Func_0200525c
	movs r1, #2
	movs r0, #9
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #0
	movs r0, #9
	movs r1, #0
	bl Func_0200525c
	movs r0, #5
	movs r1, #3
	bl Object_SetModeById
	movs r0, #6
	movs r1, #3
	bl Object_SetModeById
	movs r0, #7
	bl Func_0200514c
	cmp r0, #0
	beq .L_02009bb6
	movs r0, #7
	movs r1, #3
	bl Object_SetModeById
.L_02009bb6:
	movs r1, #3
	movs r0, #4
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #10
	adds r1, #255
	movs r2, #30
	movs r0, #9
	bl Func_0200528c
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r0, #9
	movs r1, #4
	bl Object_SetModeById
	movs r0, #33
	bl Battle_WaitMode0
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #8
	movs r2, #8
	movs r0, #9
	negs r1, r1
	negs r2, r2
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #192
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #0
	bl Func_0200526c
	movs r1, #10
	movs r2, #6
	movs r0, #9
	negs r1, r1
	negs r2, r2
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #192
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl Func_0200526c
	movs r1, #14
	movs r2, #4
	movs r0, #9
	negs r1, r1
	negs r2, r2
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #224
	movs r0, #4
	lsls r1, r1, #8
	movs r2, #0
	bl Func_0200526c
	movs r1, #160
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #0
	bl Func_0200526c
	movs r0, #7
	bl Func_0200514c
	cmp r0, #0
	beq .L_02009c68
	movs r1, #192
	movs r0, #7
	lsls r1, r1, #8
	movs r2, #0
	bl Func_0200526c
.L_02009c68:
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #9
	bl Func_0200526c
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #9
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #160
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl Func_0200526c
	movs r0, #7
	bl Func_0200514c
	cmp r0, #0
	beq .L_02009cd0
	movs r1, #176
	movs r0, #7
	lsls r1, r1, #8
	movs r2, #0
	bl Func_0200526c
	b .L_02009cd0
	.2byte 0x0000
.L_02009cc8:
	.4byte 0x00026666
.L_02009ccc:
	.4byte 0x00013333
.L_02009cd0:
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	movs r1, #0
	adds r0, #5
	bl Func_02005254
	movs r1, #128
	movs r0, #4
	lsls r1, r1, #6
	movs r2, #0
	bl Func_0200526c
	movs r0, #4
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_02009d58
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #9
	bl Motion_SetVarCbAndRefresh
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #160
	movs r0, #9
	lsls r1, r1, #7
	movs r2, #0
	bl Func_0200526c
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #7
	bl Func_0200514c
	cmp r0, #0
	beq .L_02009d36
	movs r1, #192
	movs r0, #7
	lsls r1, r1, #8
	movs r2, #0
	bl Func_0200526c
.L_02009d36:
	movs r0, #128
	lsls r0, r0, #6
	movs r1, #0
	movs r2, #0
	adds r0, #9
	bl Func_0200525c
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r1, #226
	lsls r1, r1, #1
	adds r2, r2, r1
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_02009db4
.L_02009d58:
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #9
	bl Motion_SetVarCbAndRefresh
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #160
	movs r0, #9
	lsls r1, r1, #7
	movs r2, #0
	bl Func_0200526c
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #7
	bl Func_0200514c
	cmp r0, #0
	beq .L_02009d94
	movs r1, #192
	movs r0, #7
	lsls r1, r1, #8
	movs r2, #0
	bl Func_0200526c
.L_02009d94:
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #128
	adds r3, #1
	lsls r0, r0, #6
	strh r3, [r2]
	adds r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
.L_02009db4:
	movs r0, #9
	movs r1, #6
	movs r2, #15
	bl ObjectMotion_Launch
	movs r0, #9
	movs r1, #6
	movs r2, #23
	bl ObjectMotion_Launch
	movs r0, #128
	lsls r0, r0, #6
	movs r2, #0
	adds r0, #9
	movs r1, #0
	bl Func_0200525c
	movs r1, #2
	movs r0, #5
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #5
	bl Func_0200526c
	movs r0, #15
	bl Battle_WaitMode0
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #224
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #4
	bl Func_0200526c
	movs r0, #25
	bl Battle_WaitMode0
	movs r0, #9
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #8
	bl Battle_WaitMode0
	movs r0, #9
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #8
	bl Battle_WaitMode0
	movs r0, #9
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #2
	adds r1, #255
	movs r2, #30
	movs r0, #6
	bl Func_0200528c
	movs r0, #128
	lsls r0, r0, #6
	movs r2, #0
	adds r0, #6
	movs r1, #0
	bl Func_0200525c
	movs r0, #9
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #9
	bl Func_02005294
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	movs r1, #0
	movs r2, #0
	adds r0, #9
	bl Func_0200525c
	movs r0, #6
	bl Func_020013b0
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	movs r2, #0
	adds r0, #6
	movs r1, #0
	bl Func_0200525c
	movs r1, #2
	movs r0, #9
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #7
	lsls r2, r2, #6
	adds r1, #102
	adds r2, #51
	movs r0, #9
	bl ObjectMotion_SetSpeedParameters
	movs r0, #9
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	movs r2, #16
	strb r3, [r0]
	movs r1, #0
	negs r2, r2
	movs r0, #9
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #1
	bl Battle_WaitMode0
	movs r0, #9
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	strb r3, [r0]
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #9
	bl Func_02005294
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #160
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #5
	bl Func_0200526c
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #192
	movs r0, #6
	lsls r1, r1, #7
	movs r2, #0
	bl Func_0200526c
	movs r1, #128
	movs r0, #4
	lsls r1, r1, #6
	movs r2, #0
	bl Func_0200526c
	movs r0, #7
	bl Func_0200514c
	cmp r0, #0
	beq .L_02009f68
	movs r1, #176
	movs r0, #7
	lsls r1, r1, #8
	movs r2, #0
	bl Func_0200526c
.L_02009f68:
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #6
	movs r1, #3
	bl Object_SetModeById
	movs r0, #7
	bl Func_0200514c
	cmp r0, #0
	beq .L_02009f88
	movs r0, #7
	movs r1, #3
	bl Object_SetModeById
.L_02009f88:
	movs r1, #3
	movs r0, #4
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #35
	bl Battle_WaitMode0
	movs r0, #7
	bl Func_0200514c
	cmp r0, #0
	beq .L_02009fb2
	movs r1, #192
	movs r0, #7
	lsls r1, r1, #8
	movs r2, #0
	bl Func_0200526c
.L_02009fb2:
	movs r1, #192
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl Func_0200526c
	movs r1, #160
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #0
	bl Func_0200526c
	movs r1, #224
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #4
	bl Func_0200526c
	movs r0, #25
	bl Battle_WaitMode0
	movs r1, #10
	adds r1, #255
	movs r2, #30
	movs r0, #9
	bl Func_0200528c
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #128
	movs r2, #128
	movs r0, #9
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r1, #0
	movs r2, #16
	movs r0, #9
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #4
	adds r1, #255
	movs r2, #30
	movs r0, #9
	bl Func_0200528c
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #4
	bl Func_0200528c
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #1
	movs r2, #0
	bl Func_0200528c
	movs r0, #7
	bl Func_0200514c
	cmp r0, #0
	beq .L_0200a054
	movs r1, #128
	movs r0, #7
	lsls r1, r1, #1
	movs r2, #0
	bl Func_0200528c
.L_0200a054:
	movs r1, #128
	movs r2, #50
	lsls r1, r1, #1
	movs r0, #6
	bl Func_0200528c
	movs r1, #2
	movs r0, #9
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #128
	movs r2, #128
	movs r0, #9
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl ObjectMotion_SetSpeedParameters
	movs r1, #16
	movs r0, #9
	negs r1, r1
	movs r2, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #160
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl Func_0200526c
	movs r1, #160
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #0
	bl Func_0200526c
	movs r1, #192
	movs r0, #4
	lsls r1, r1, #8
	movs r2, #0
	bl Func_0200526c
	movs r1, #128
	movs r0, #9
	lsls r1, r1, #7
	movs r2, #0
	bl Func_0200526c
	movs r0, #7
	bl Func_0200514c
	cmp r0, #0
	beq .L_0200a0dc
	movs r1, #160
	movs r0, #7
	lsls r1, r1, #8
	movs r2, #0
	bl Func_0200526c
.L_0200a0dc:
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #40
	movs r0, #4
	bl Func_0200528c
	movs r0, #9
	movs r1, #4
	bl Object_SetModeById
	movs r0, #33
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r0, #9
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r0, #9
	movs r1, #6
	movs r2, #15
	bl ObjectMotion_Launch
	movs r0, #9
	movs r1, #6
	movs r2, #23
	bl ObjectMotion_Launch
	movs r0, #7
	bl Func_0200514c
	cmp r0, #0
	beq .L_0200a150
	movs r1, #129
	movs r0, #7
	lsls r1, r1, #1
	movs r2, #0
	bl Func_0200528c
.L_0200a150:
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #6
	bl Func_0200528c
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #4
	bl Func_0200528c
	movs r1, #129
	movs r2, #50
	lsls r1, r1, #1
	movs r0, #5
	bl Func_0200528c
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #6
	movs r0, #4
	bl Func_02005274
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #6
	movs r1, #3
	bl Object_SetModeById
	movs r0, #4
	movs r1, #3
	bl Object_SetModeById
	movs r0, #7
	bl Func_0200514c
	cmp r0, #0
	beq .L_0200a1ac
	movs r0, #7
	movs r1, #3
	bl Object_SetModeById
.L_0200a1ac:
	movs r1, #3
	movs r0, #5
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #30
	bl Battle_WaitMode0
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #9
	ldr r1, .L_0200a2ec
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r0, #9
	movs r1, #2
	bl Object_SetModeById
	ldr r3, .L_0200a2f0
	movs r1, #133
	lsls r1, r1, #2
	adds r5, r3, r1
	ldr r0, [r5]
	bl Object_GetById
	cmp r0, #0
	beq .L_0200a1f6
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #9
	bl ObjectMotion_ResetAndSetPosition
.L_0200a1f6:
	movs r0, #9
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_0200520c
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #5
	ldr r1, .L_0200a2ec
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r0, #5
	movs r1, #2
	bl Object_SetModeById
	ldr r0, [r5]
	bl Object_GetById
	cmp r0, #0
	beq .L_0200a234
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #5
	bl ObjectMotion_ResetAndSetPosition
.L_0200a234:
	movs r0, #5
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl Func_0200520c
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #6
	ldr r1, .L_0200a2ec
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r0, #6
	movs r1, #2
	bl Object_SetModeById
	ldr r0, [r5]
	bl Object_GetById
	cmp r0, #0
	beq .L_0200a272
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #6
	bl ObjectMotion_ResetAndSetPosition
.L_0200a272:
	movs r0, #6
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #6
	movs r1, #0
	movs r2, #0
	bl Func_0200520c
	movs r0, #7
	bl Func_0200514c
	cmp r0, #0
	beq .L_0200a2ca
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #7
	ldr r1, .L_0200a2ec
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r0, #7
	movs r1, #2
	bl Object_SetModeById
	ldr r0, [r5]
	bl Object_GetById
	cmp r0, #0
	beq .L_0200a2ba
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #7
	bl ObjectMotion_ResetAndSetPosition
.L_0200a2ba:
	movs r0, #7
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #7
	movs r1, #0
	movs r2, #0
	bl Func_0200520c
.L_0200a2ca:
	ldr r3, .L_0200a2f0
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	ldr r0, [r3]
	movs r1, #1
	bl Func_0200529c
	bl Func_020051b4
	add sp, #12
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200a2ec:
	.4byte 0x00013333
.L_0200a2f0:
	.4byte gPartyState
	.section .text.x0200a2f4,"ax",%progbits
	.global Func_020022f4
	.thumb_func
Func_020022f4:
	push {r5, lr}
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #29
	bl Func_02005154
	bl Func_020051ac
	movs r0, #0
	bl Func_02005324
	ldr r0, .L_0200a684
	bl Func_0200524c
	movs r1, #144
	movs r2, #188
	movs r0, #8
	lsls r1, r1, #16
	lsls r2, r2, #18
	bl Func_0200520c
	movs r1, #140
	movs r2, #194
	movs r0, #4
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_SetPositionAndReset
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #4
	bl Func_0200526c
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r3, #176
	lsls r3, r3, #8
	movs r1, #16
	movs r2, #0
	movs r0, #9
	bl Func_02005334
	movs r0, #9
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #128
	movs r0, #9
	lsls r1, r1, #8
	movs r2, #0
	bl Func_0200526c
	movs r2, #0
	movs r1, #0
	movs r0, #4
	bl Func_0200526c
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #9
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	movs r0, #9
	lsls r1, r1, #1
	bl Func_02005294
	movs r0, #4
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #4
	bl Func_02005294
	movs r0, #45
	bl Battle_WaitMode0
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #176
	movs r0, #9
	lsls r1, r1, #8
	movs r2, #0
	bl Func_0200526c
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #4
	bl Func_0200526c
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #140
	movs r1, #1
	movs r2, #188
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #18
	lsls r0, r0, #17
	bl Func_020052ac
	bl Func_020052b4
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #144
	movs r2, #180
	movs r0, #8
	lsls r1, r1, #16
	lsls r2, r2, #18
	bl Func_0200520c
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #8
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r2, #180
	movs r1, #168
	lsls r2, r2, #2
	movs r0, #8
	bl ObjectMotion_SetPositionAndReset
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #128
	movs r2, #128
	movs r0, #8
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl ObjectMotion_SetSpeedParameters
	movs r0, #8
	movs r1, #6
	movs r2, #0
	bl ObjectMotion_Launch
	movs r2, #180
	movs r1, #200
	lsls r2, r2, #2
	movs r0, #8
	bl ObjectMotion_SetPositionAndReset
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #8
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r2, #178
	movs r0, #8
	movs r1, #216
	lsls r2, r2, #2
	bl ObjectMotion_SetPositionAndReset
	movs r1, #160
	movs r2, #178
	movs r0, #8
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_SetPositionAndReset
	movs r2, #0
	movs r0, #9
	movs r1, #0
	bl Func_0200525c
	movs r1, #2
	movs r0, #8
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #153
	movs r2, #152
	lsls r1, r1, #8
	lsls r2, r2, #7
	adds r1, #153
	adds r2, #204
	movs r0, #8
	bl ObjectMotion_SetSpeedParameters
	movs r0, #8
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	movs r1, #148
	movs r2, #178
	strb r3, [r0]
	lsls r1, r1, #1
	lsls r2, r2, #2
	movs r0, #8
	bl ObjectMotion_SetPositionAndReset
	movs r0, #1
	bl Battle_WaitMode0
	movs r0, #8
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	strb r3, [r0]
	movs r0, #15
	bl Battle_WaitMode0
	movs r1, #144
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #8
	bl Func_0200526c
	movs r0, #25
	bl Battle_WaitMode0
	movs r1, #240
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #8
	bl Func_0200526c
	movs r0, #25
	bl Battle_WaitMode0
	movs r1, #144
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #8
	bl Func_0200526c
	movs r0, #25
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #8
	bl Func_0200526c
	movs r0, #25
	bl Battle_WaitMode0
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #45
	movs r0, #8
	bl Func_0200528c
	movs r0, #8
	movs r1, #6
	movs r2, #15
	bl ObjectMotion_Launch
	movs r0, #8
	movs r1, #6
	movs r2, #23
	bl ObjectMotion_Launch
	movs r0, #8
	ldr r1, .L_0200a688
	ldr r2, .L_0200a68c
	bl ObjectMotion_SetSpeedParameters
	movs r2, #178
	movs r1, #200
	lsls r2, r2, #2
	movs r0, #8
	bl ObjectMotion_SetPositionAndReset
	movs r0, #3
	bl Battle_WaitMode0
	movs r1, #128
	movs r2, #128
	movs r0, #8
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl ObjectMotion_SetSpeedParameters
	movs r0, #8
	movs r1, #6
	movs r2, #0
	bl ObjectMotion_Launch
	movs r2, #178
	movs r1, #168
	lsls r2, r2, #2
	movs r0, #8
	bl ObjectMotion_SetPositionAndReset
	movs r0, #3
	bl Battle_WaitMode0
	movs r0, #8
	ldr r1, .L_0200a688
	ldr r2, .L_0200a68c
	bl ObjectMotion_SetSpeedParameters
	movs r2, #178
	movs r0, #8
	movs r1, #144
	lsls r2, r2, #2
	bl ObjectMotion_SetPositionAndReset
	movs r2, #0
	movs r1, #0
	movs r0, #8
	bl Func_0200520c
	movs r0, #30
	bl Battle_WaitMode0
	ldr r5, .L_0200a690
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	ldr r0, [r5]
	movs r1, #1
	bl Func_0200529c
	bl Func_020052b4
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #9
	bl Func_0200528c
	movs r2, #0
	movs r0, #9
	movs r1, #0
	bl Func_0200525c
	movs r1, #2
	movs r0, #9
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #9
	bl Func_0200526c
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #0
	movs r2, #0
	movs r0, #4
	bl Func_0200526c
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #9
	movs r1, #6
	movs r2, #15
	bl ObjectMotion_Launch
	movs r0, #9
	movs r1, #6
	movs r2, #23
	bl ObjectMotion_Launch
	movs r0, #4
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #9
	ldr r1, .L_0200a68c
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r0, #9
	movs r1, #2
	bl Object_SetModeById
	ldr r0, [r5]
	bl Object_GetById
	cmp r0, #0
	beq .L_0200a66c
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #9
	bl ObjectMotion_ResetAndSetPosition
.L_0200a66c:
	movs r0, #9
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_0200520c
	bl Func_020051b4
	pop {r5, pc}
	.2byte 0x0000
.L_0200a684:
	.4byte 0x000019ab
.L_0200a688:
	.4byte 0x00026666
.L_0200a68c:
	.4byte 0x00013333
.L_0200a690:
	.4byte gPartyState
	.section .text.x0200a694,"ax",%progbits
	.global Func_02002694
	.thumb_func
Func_02002694:
	push {r5, lr}
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #30
	bl Func_02005154
	bl Func_020051ac
	movs r0, #0
	bl Func_02005324
	ldr r0, .L_0200a814
	bl Func_0200524c
	movs r1, #164
	movs r2, #160
	movs r0, #10
	lsls r1, r1, #17
	lsls r2, r2, #14
	bl Func_0200520c
	movs r1, #164
	movs r2, #160
	movs r0, #8
	lsls r1, r1, #17
	lsls r2, r2, #14
	bl Func_0200520c
	movs r1, #128
	movs r0, #10
	lsls r1, r1, #7
	movs r2, #0
	bl Func_0200526c
	movs r1, #128
	movs r0, #8
	lsls r1, r1, #7
	movs r2, #0
	bl Func_0200526c
	movs r1, #160
	movs r0, #4
	lsls r1, r1, #1
	movs r2, #224
	bl ObjectMotion_SetPositionAndReset
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #4
	bl Func_0200526c
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #16
	movs r2, #8
	movs r3, #192
	negs r2, r2
	lsls r3, r3, #8
	negs r1, r1
	movs r0, #9
	bl Func_02005334
	movs r0, #9
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #9
	lsls r1, r1, #6
	bl Func_020052a4
	movs r0, #164
	movs r1, #1
	movs r2, #152
	movs r3, #1
	lsls r0, r0, #17
	negs r1, r1
	lsls r2, r2, #16
	bl Func_020052ac
	movs r1, #160
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #9
	bl Func_0200526c
	movs r0, #25
	bl Battle_WaitMode0
	movs r1, #224
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #9
	bl Func_0200526c
	movs r0, #25
	bl Battle_WaitMode0
	movs r1, #160
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #9
	bl Func_0200526c
	movs r0, #25
	bl Battle_WaitMode0
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #9
	bl Func_0200526c
	movs r0, #25
	bl Battle_WaitMode0
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #160
	movs r0, #4
	lsls r1, r1, #8
	movs r2, #0
	bl Func_0200526c
	movs r1, #128
	movs r2, #0
	movs r0, #9
	lsls r1, r1, #6
	bl Func_0200526c
	movs r0, #4
	movs r1, #1
	bl Func_020052bc
	bl Func_020052b4
	movs r0, #192
	movs r1, #192
	lsls r0, r0, #9
	lsls r1, r1, #6
	bl Func_020052a4
	movs r1, #0
	movs r0, #9
	bl Func_02005254
	movs r0, #4
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_0200a818
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #10
	adds r1, #255
	movs r2, #30
	movs r0, #9
	bl Func_0200528c
	movs r2, #0
	movs r0, #9
	movs r1, #0
	bl Func_0200525c
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_0200a846
	.2byte 0x0000
.L_0200a814:
	.4byte 0x000019b0
.L_0200a818:
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #10
	adds r1, #255
	movs r2, #30
	movs r0, #9
	bl Func_0200528c
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
	movs r2, #0
	bl Func_0200525c
.L_0200a846:
	movs r2, #0
	movs r0, #8
	movs r1, #0
	bl Func_0200525c
	movs r1, #2
	movs r0, #4
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #192
	movs r0, #4
	lsls r1, r1, #8
	movs r2, #0
	bl Func_0200526c
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #9
	bl Func_0200526c
	movs r0, #20
	bl Battle_WaitMode0
	movs r3, #192
	movs r0, #5
	movs r1, #16
	movs r2, #0
	lsls r3, r3, #8
	bl Func_02005334
	movs r3, #192
	movs r0, #6
	movs r1, #32
	movs r2, #0
	lsls r3, r3, #8
	bl Func_02005334
	movs r0, #7
	bl Func_0200514c
	cmp r0, #0
	beq .L_0200a8b2
	movs r2, #8
	movs r3, #160
	movs r0, #7
	movs r1, #48
	negs r2, r2
	lsls r3, r3, #8
	bl Func_02005334
.L_0200a8b2:
	movs r0, #6
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #5
	bl Func_0200528c
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #160
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #6
	bl Func_0200526c
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #224
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #6
	bl Func_0200526c
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #160
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #6
	bl Func_0200526c
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #6
	bl Func_0200526c
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #6
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #2
	adds r1, #255
	movs r2, #30
	movs r0, #9
	bl Func_0200528c
	movs r1, #160
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #9
	bl Func_0200526c
	movs r0, #25
	bl Battle_WaitMode0
	movs r1, #224
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #9
	bl Func_0200526c
	movs r0, #25
	bl Battle_WaitMode0
	movs r1, #160
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #9
	bl Func_0200526c
	movs r0, #25
	bl Battle_WaitMode0
	movs r1, #224
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #9
	bl Func_0200526c
	movs r0, #25
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #9
	bl Func_0200526c
	movs r0, #25
	bl Battle_WaitMode0
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #128
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #0
	bl Func_0200526c
	movs r1, #0
	movs r2, #0
	movs r0, #5
	bl Func_0200526c
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #160
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #0
	bl Func_0200526c
	movs r1, #160
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #5
	bl Func_0200526c
	movs r0, #25
	bl Battle_WaitMode0
	movs r0, #6
	movs r1, #3
	bl Object_SetModeById
	movs r0, #5
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #192
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl Func_0200526c
	movs r1, #192
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #0
	bl Func_0200526c
	movs r1, #192
	movs r0, #4
	lsls r1, r1, #8
	movs r2, #0
	bl Func_0200526c
	movs r1, #224
	movs r0, #9
	lsls r1, r1, #8
	movs r2, #0
	bl Func_0200526c
	movs r0, #164
	movs r1, #1
	movs r2, #144
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #16
	lsls r0, r0, #17
	bl Func_020052ac
	bl Func_020052b4
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #164
	movs r2, #136
	lsls r2, r2, #16
	lsls r1, r1, #17
	movs r0, #11
	bl Func_0200520c
	movs r0, #11
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #11
	bl Func_02001418
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #179
	movs r2, #178
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #8
	adds r1, #51
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r1, #179
	movs r2, #178
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #10
	adds r1, #51
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r1, #164
	movs r2, #128
	lsls r1, r1, #17
	lsls r2, r2, #16
	movs r0, #10
	bl Func_0200520c
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #10
	movs r1, #0
	movs r2, #16
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #164
	movs r2, #128
	lsls r1, r1, #17
	lsls r2, r2, #16
	movs r0, #8
	bl Func_0200520c
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #8
	movs r1, #0
	movs r2, #16
	bl ObjectMotion_OffsetPositionAndReset
	movs r1, #0
	movs r2, #16
	movs r0, #10
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #11
	bl Func_02001454
	movs r2, #0
	movs r0, #11
	movs r1, #0
	bl Func_0200520c
	movs r1, #1
	movs r0, #10
	bl Func_0200529c
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #8
	movs r1, #0
	movs r2, #24
	bl ObjectMotion_OffsetPositionAndReset
	movs r0, #10
	movs r1, #0
	movs r2, #24
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #8
	movs r1, #16
	movs r2, #8
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #8
	bl Func_0200526c
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #164
	movs r1, #1
	movs r2, #208
	movs r3, #1
	lsls r2, r2, #16
	negs r1, r1
	lsls r0, r0, #17
	bl Func_020052ac
	bl Func_020052b4
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #8
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #0
	movs r0, #8
	movs r1, #0
	bl Func_0200525c
	movs r0, #9
	movs r1, #2
	bl ObjectMotion_SetVariantCallback
	movs r0, #6
	movs r1, #2
	bl ObjectMotion_SetVariantCallback
	movs r0, #5
	movs r1, #2
	bl ObjectMotion_SetVariantCallback
	movs r0, #7
	bl Func_0200514c
	cmp r0, #0
	beq .L_0200ab98
	movs r0, #7
	movs r1, #2
	bl ObjectMotion_SetVariantCallback
.L_0200ab98:
	movs r1, #2
	movs r0, #4
	bl Motion_SetVarCbAndRefresh
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #10
	movs r1, #4
	bl Object_SetModeById
	movs r0, #33
	bl Battle_WaitMode0
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #6
	bl Func_0200528c
	movs r2, #0
	movs r0, #6
	movs r1, #0
	bl Func_0200525c
	movs r0, #8
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #8
	bl Func_02005294
	movs r0, #40
	bl Battle_WaitMode0
	movs r2, #0
	movs r0, #8
	movs r1, #0
	bl Func_0200525c
	movs r1, #2
	movs r0, #9
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r0, #8
	movs r1, #6
	movs r2, #15
	bl ObjectMotion_Launch
	movs r0, #8
	movs r1, #6
	movs r2, #23
	bl ObjectMotion_Launch
	movs r1, #192
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #8
	bl Func_0200526c
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #10
	movs r1, #4
	bl Object_SetModeById
	movs r0, #33
	bl Battle_WaitMode0
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #2
	adds r1, #255
	movs r2, #0
	movs r0, #4
	bl Func_0200528c
	movs r1, #2
	adds r1, #255
	movs r2, #0
	movs r0, #5
	bl Func_0200528c
	movs r1, #2
	movs r0, #6
	adds r1, #255
	movs r2, #0
	bl Func_0200528c
	movs r0, #7
	bl Func_0200514c
	cmp r0, #0
	beq .L_0200ac86
	movs r1, #2
	movs r0, #7
	adds r1, #255
	movs r2, #0
	bl Func_0200528c
.L_0200ac86:
	movs r1, #2
	movs r2, #60
	adds r1, #255
	movs r0, #9
	bl Func_0200528c
	movs r1, #2
	movs r0, #10
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #0
	movs r0, #10
	bl Func_02005254
	movs r0, #4
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	beq .L_0200ace6
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #10
	movs r1, #4
	bl Object_SetModeById
	movs r0, #33
	bl Battle_WaitMode0
	movs r0, #10
	movs r2, #0
	movs r1, #0
	bl Func_0200525c
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_0200ad16
.L_0200ace6:
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #10
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #10
	adds r3, #1
	strh r3, [r2]
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
.L_0200ad16:
	movs r1, #2
	movs r0, #8
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #0
	movs r2, #0
	movs r0, #10
	bl Func_0200526c
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #10
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #2
	adds r1, #255
	movs r2, #30
	movs r0, #5
	bl Func_0200528c
	movs r2, #0
	movs r0, #5
	movs r1, #0
	bl Func_0200525c
	movs r1, #2
	movs r0, #10
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #128
	movs r0, #8
	lsls r1, r1, #7
	movs r2, #0
	bl Func_0200526c
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #10
	bl Func_0200526c
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #131
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #6
	bl Func_0200528c
	movs r1, #160
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #6
	bl Func_0200526c
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #6
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r0, #10
	movs r1, #4
	bl Object_SetModeById
	movs r0, #33
	bl Battle_WaitMode0
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #2
	adds r1, #255
	movs r2, #30
	movs r0, #10
	bl Func_0200528c
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #2
	adds r1, #255
	movs r2, #0
	movs r0, #4
	bl Func_0200528c
	movs r1, #2
	adds r1, #255
	movs r2, #0
	movs r0, #5
	bl Func_0200528c
	movs r1, #2
	movs r0, #6
	adds r1, #255
	movs r2, #0
	bl Func_0200528c
	movs r0, #7
	bl Func_0200514c
	cmp r0, #0
	beq .L_0200ae32
	movs r1, #2
	movs r0, #7
	adds r1, #255
	movs r2, #0
	bl Func_0200528c
.L_0200ae32:
	movs r1, #2
	adds r1, #255
	movs r2, #60
	movs r0, #9
	bl Func_0200528c
	movs r0, #10
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #10
	movs r1, #0
	bl Func_02005254
	movs r1, #128
	movs r0, #9
	lsls r1, r1, #6
	movs r2, #0
	bl Func_0200526c
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl Func_0200526c
	movs r1, #128
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #0
	bl Func_0200526c
	movs r0, #7
	bl Func_0200514c
	cmp r0, #0
	beq .L_0200ae8e
	movs r1, #128
	movs r0, #7
	lsls r1, r1, #8
	movs r2, #0
	bl Func_0200526c
.L_0200ae8e:
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #4
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	beq .L_0200aed2
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #5
	bl Motion_SetVarCbAndRefresh
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #0
	movs r0, #5
	movs r1, #0
	bl Func_0200525c
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_0200af08
.L_0200aed2:
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #5
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #20
	bl Battle_WaitMode0
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #5
	adds r3, #1
	strh r3, [r2]
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
.L_0200af08:
	movs r1, #224
	movs r0, #10
	lsls r1, r1, #8
	movs r2, #0
	bl Func_0200526c
	movs r1, #192
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #8
	bl Func_0200526c
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #224
	movs r0, #9
	lsls r1, r1, #8
	movs r2, #0
	bl Func_0200526c
	movs r1, #192
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl Func_0200526c
	movs r1, #192
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #0
	bl Func_0200526c
	movs r0, #7
	bl Func_0200514c
	cmp r0, #0
	beq .L_0200af6a
	movs r1, #160
	movs r0, #7
	lsls r1, r1, #8
	movs r2, #0
	bl Func_0200526c
.L_0200af6a:
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #6
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #6
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #10
	bl Func_0200526c
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r0, #9
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #10
	bl Func_0200528c
	movs r2, #0
	movs r0, #10
	movs r1, #0
	bl Func_0200525c
	movs r0, #4
	movs r1, #3
	bl Object_SetModeById
	movs r0, #5
	movs r1, #3
	bl Object_SetModeById
	movs r0, #6
	movs r1, #3
	bl Object_SetModeById
	movs r0, #7
	bl Func_0200514c
	cmp r0, #0
	beq .L_0200affc
	movs r0, #7
	movs r1, #3
	bl Object_SetModeById
.L_0200affc:
	movs r1, #3
	movs r0, #9
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #9
	bl Func_0200526c
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #224
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #9
	bl Func_0200526c
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #10
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #8
	bl Func_0200528c
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #224
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #10
	bl Func_0200526c
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #5
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #192
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #10
	bl Func_0200526c
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #10
	adds r1, #255
	movs r2, #30
	movs r0, #6
	bl Func_0200528c
	movs r0, #6
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #8
	bl Func_0200526c
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #8
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #0
	movs r2, #0
	movs r0, #9
	bl Func_0200526c
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #224
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #9
	bl Func_0200526c
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #10
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #5
	bl Func_0200528c
	movs r2, #0
	movs r0, #5
	movs r1, #0
	bl Func_0200525c
	movs r1, #2
	movs r0, #10
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r0, #9
	movs r1, #4
	bl Object_SetModeById
	movs r0, #33
	bl Battle_WaitMode0
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #8
	bl Func_0200526c
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #0
	movs r2, #16
	movs r0, #8
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #8
	movs r1, #6
	movs r2, #15
	bl ObjectMotion_Launch
	movs r0, #8
	movs r1, #6
	movs r2, #23
	bl ObjectMotion_Launch
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #6
	bl Func_0200528c
	movs r0, #6
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #160
	movs r2, #160
	movs r0, #10
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl ObjectMotion_SetSpeedParameters
	movs r1, #160
	movs r2, #160
	movs r0, #8
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl ObjectMotion_SetSpeedParameters
	movs r1, #0
	movs r2, #0
	movs r0, #10
	bl Func_0200526c
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #10
	movs r1, #8
	movs r2, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #4
	movs r2, #0
	movs r0, #8
	bl ObjectMotion_Launch
	movs r0, #10
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r5, #254
	adds r3, r5, #0
	ands r3, r2
	movs r1, #8
	strb r3, [r0]
	negs r1, r1
	movs r2, #0
	movs r0, #10
	bl ObjectMotion_OffsetPositionAndReset
	movs r0, #8
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	movs r1, #16
	ands r5, r3
	movs r2, #0
	strb r5, [r0]
	movs r0, #8
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #1
	bl Battle_WaitMode0
	movs r0, #8
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	strb r3, [r0]
	movs r1, #5
	movs r0, #8
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #166
	movs r2, #166
	lsls r1, r1, #9
	lsls r2, r2, #8
	movs r0, #8
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #179
	movs r2, #178
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #10
	adds r1, #51
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r0, #10
	movs r1, #4
	bl Object_SetModeById
	movs r0, #33
	bl Battle_WaitMode0
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #129
	movs r2, #50
	lsls r1, r1, #1
	movs r0, #8
	bl Func_0200528c
	movs r0, #8
	movs r1, #1
	bl Object_SetModeById
	movs r0, #8
	movs r1, #6
	movs r2, #0
	bl ObjectMotion_Launch
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #8
	bl Func_0200526c
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #129
	movs r0, #8
	lsls r1, r1, #1
	bl Func_02005294
	movs r0, #8
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #15
	bl Battle_WaitMode0
	movs r1, #129
	movs r0, #8
	lsls r1, r1, #1
	bl Func_02005294
	movs r0, #8
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #10
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #2
	adds r1, #255
	movs r2, #30
	movs r0, #8
	bl Func_0200528c
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r0, #10
	movs r1, #4
	bl Object_SetModeById
	movs r0, #33
	bl Battle_WaitMode0
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #160
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #8
	bl Func_0200526c
	movs r0, #45
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #8
	bl Func_0200526c
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r0, #10
	movs r1, #4
	bl Object_SetModeById
	movs r0, #33
	bl Battle_WaitMode0
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #160
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #10
	bl Func_0200526c
	movs r0, #45
	bl Battle_WaitMode0
	movs r1, #0
	movs r2, #0
	movs r0, #10
	bl Func_0200526c
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r0, #8
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #8
	bl Func_0200526c
	movs r0, #35
	bl Battle_WaitMode0
	movs r0, #8
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #0
	movs r0, #8
	movs r1, #0
	bl Func_0200525c
	movs r0, #4
	movs r1, #3
	bl Object_SetModeById
	movs r0, #5
	movs r1, #3
	bl Object_SetModeById
	movs r0, #6
	movs r1, #3
	bl Object_SetModeById
	movs r0, #9
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #4
	movs r1, #8
	bl Object_LinkObjectAndSetCallback
	movs r0, #9
	movs r1, #8
	bl Object_LinkObjectAndSetCallback
	movs r0, #5
	movs r1, #8
	bl Object_LinkObjectAndSetCallback
	movs r0, #6
	movs r1, #8
	bl Object_LinkObjectAndSetCallback
	movs r0, #10
	movs r1, #8
	bl Object_LinkObjectAndSetCallback
	movs r0, #7
	bl Func_0200514c
	cmp r0, #0
	beq .L_0200b46e
	movs r0, #7
	movs r1, #8
	bl Object_LinkObjectAndSetCallback
.L_0200b46e:
	movs r1, #188
	movs r0, #8
	lsls r1, r1, #1
	movs r2, #208
	bl ObjectMotion_SetPositionAndReset
	movs r0, #8
	movs r1, #0
	movs r2, #16
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #16
	movs r0, #8
	negs r1, r1
	movs r2, #24
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #32
	movs r0, #8
	negs r1, r1
	movs r2, #16
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #0
	movs r2, #48
	movs r0, #8
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #4
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #9
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #5
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #6
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #10
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #7
	bl Func_0200514c
	cmp r0, #0
	beq .L_0200b4da
	movs r0, #7
	bl ObjectMotion_EnableActionAndResetMotion
.L_0200b4da:
	movs r2, #0
	movs r0, #8
	movs r1, #0
	bl Func_0200520c
	movs r1, #2
	movs r0, #10
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #224
	movs r0, #9
	lsls r1, r1, #8
	movs r2, #0
	bl Func_0200526c
	movs r1, #192
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl Func_0200526c
	movs r1, #192
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #0
	bl Func_0200526c
	movs r1, #192
	movs r0, #4
	lsls r1, r1, #8
	movs r2, #0
	bl Func_0200526c
	movs r0, #7
	bl Func_0200514c
	cmp r0, #0
	beq .L_0200b542
	movs r1, #160
	movs r0, #7
	lsls r1, r1, #8
	movs r2, #0
	bl Func_0200526c
.L_0200b542:
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #10
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #9
	bl Func_0200528c
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #10
	bl Func_0200528c
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #2
	adds r1, #255
	movs r2, #30
	movs r0, #6
	bl Func_0200528c
	movs r0, #6
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #10
	adds r1, #255
	movs r2, #30
	movs r0, #10
	bl Func_0200528c
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #9
	bl Func_0200528c
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r0, #10
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #5
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #5
	bl Func_0200526c
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r0, #10
	movs r1, #4
	bl Object_SetModeById
	movs r0, #33
	bl Battle_WaitMode0
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #2
	adds r1, #255
	movs r2, #50
	movs r0, #5
	bl Func_0200528c
	movs r1, #176
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #5
	bl Func_0200526c
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #9
	bl Func_0200526c
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r0, #10
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #224
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #9
	bl Func_0200526c
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #4
	movs r1, #3
	bl Object_SetModeById
	movs r0, #5
	movs r1, #3
	bl Object_SetModeById
	movs r0, #6
	movs r1, #3
	bl Object_SetModeById
	movs r0, #9
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #128
	movs r0, #9
	lsls r1, r1, #6
	movs r2, #0
	bl Func_0200526c
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl Func_0200526c
	movs r1, #128
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #0
	bl Func_0200526c
	movs r0, #7
	bl Func_0200514c
	cmp r0, #0
	beq .L_0200b6f4
	movs r1, #128
	movs r0, #7
	lsls r1, r1, #8
	movs r2, #0
	bl Func_0200526c
.L_0200b6f4:
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #5
	movs r1, #3
	bl Object_SetModeById
	movs r0, #6
	movs r1, #3
	bl Object_SetModeById
	movs r0, #7
	bl Func_0200514c
	cmp r0, #0
	beq .L_0200b71c
	movs r0, #7
	movs r1, #3
	bl Object_SetModeById
.L_0200b71c:
	movs r1, #3
	movs r0, #9
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #4
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #153
	lsls r2, r2, #8
	adds r2, #153
	movs r0, #9
	ldr r1, .L_0200b870
	bl ObjectMotion_SetSpeedParameters
	movs r0, #9
	movs r1, #2
	bl Object_SetModeById
	ldr r3, .L_0200b874
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r3, r2
	ldr r0, [r5]
	bl Object_GetById
	cmp r0, #0
	beq .L_0200b77a
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #9
	bl ObjectMotion_ResetAndSetPosition
.L_0200b77a:
	movs r0, #9
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_0200520c
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #5
	ldr r1, .L_0200b870
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r0, #5
	movs r1, #2
	bl Object_SetModeById
	ldr r0, [r5]
	bl Object_GetById
	cmp r0, #0
	beq .L_0200b7b8
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #5
	bl ObjectMotion_ResetAndSetPosition
.L_0200b7b8:
	movs r0, #5
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl Func_0200520c
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #6
	ldr r1, .L_0200b870
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r0, #6
	movs r1, #2
	bl Object_SetModeById
	ldr r0, [r5]
	bl Object_GetById
	cmp r0, #0
	beq .L_0200b7f6
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #6
	bl ObjectMotion_ResetAndSetPosition
.L_0200b7f6:
	movs r0, #6
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #6
	movs r1, #0
	movs r2, #0
	bl Func_0200520c
	movs r0, #7
	bl Func_0200514c
	cmp r0, #0
	beq .L_0200b84e
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #7
	ldr r1, .L_0200b870
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r0, #7
	movs r1, #2
	bl Object_SetModeById
	ldr r0, [r5]
	bl Object_GetById
	cmp r0, #0
	beq .L_0200b83e
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #7
	bl ObjectMotion_ResetAndSetPosition
.L_0200b83e:
	movs r0, #7
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #7
	movs r1, #0
	movs r2, #0
	bl Func_0200520c
.L_0200b84e:
	ldr r3, .L_0200b874
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	movs r1, #1
	bl Func_0200529c
	bl Func_02001344
	movs r0, #11
	bl Func_020052c4
	bl Func_020051b4
	pop {r5, pc}
	.2byte 0x0000
.L_0200b870:
	.4byte 0x00013333
.L_0200b874:
	.4byte gPartyState
	.section .text.x0200b878,"ax",%progbits
	.global Func_02003878
	.thumb_func
Func_02003878:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r0, #130
	lsls r0, r0, #4
	adds r0, #255
	sub sp, #28
	bl Func_02005154
	bl Func_020051ac
	movs r0, #0
	bl Func_02005324
	ldr r0, .L_0200ba88
	bl Func_0200524c
	movs r1, #164
	movs r2, #160
	movs r0, #10
	lsls r1, r1, #17
	lsls r2, r2, #14
	bl Func_0200520c
	movs r1, #128
	movs r0, #10
	lsls r1, r1, #7
	movs r2, #0
	bl Func_0200526c
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #4
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #160
	movs r0, #4
	lsls r1, r1, #1
	movs r2, #224
	bl ObjectMotion_SetPositionAndReset
	movs r1, #16
	movs r3, #192
	movs r0, #9
	negs r1, r1
	movs r2, #0
	lsls r3, r3, #8
	bl Func_02005334
	movs r3, #192
	movs r0, #5
	movs r1, #16
	movs r2, #0
	lsls r3, r3, #8
	bl Func_02005334
	movs r3, #192
	movs r0, #6
	movs r1, #32
	movs r2, #0
	lsls r3, r3, #8
	bl Func_02005334
	movs r0, #7
	bl Func_0200514c
	cmp r0, #0
	beq .L_0200b920
	movs r3, #192
	movs r0, #7
	movs r1, #48
	movs r2, #0
	lsls r3, r3, #8
	bl Func_02005334
.L_0200b920:
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #4
	bl Func_0200526c
	movs r0, #6
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #164
	movs r1, #1
	movs r2, #144
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #16
	lsls r0, r0, #17
	bl Func_020052ac
	bl Func_020052b4
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #164
	movs r2, #136
	lsls r2, r2, #16
	lsls r1, r1, #17
	movs r0, #11
	bl Func_0200520c
	movs r0, #11
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #11
	bl Func_02001418
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #179
	movs r2, #178
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #10
	adds r1, #51
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r1, #164
	movs r2, #128
	lsls r1, r1, #17
	lsls r2, r2, #16
	movs r0, #10
	bl Func_0200520c
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #0
	movs r2, #16
	movs r0, #10
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #11
	bl Func_02001454
	movs r2, #0
	movs r0, #11
	movs r1, #0
	bl Func_0200520c
	movs r1, #1
	movs r0, #10
	bl Func_0200529c
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #10
	movs r1, #0
	movs r2, #48
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #224
	movs r0, #9
	lsls r1, r1, #8
	movs r2, #0
	bl Func_0200526c
	movs r1, #192
	movs r0, #4
	lsls r1, r1, #8
	movs r2, #0
	bl Func_0200526c
	movs r1, #192
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl Func_0200526c
	movs r1, #160
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #0
	bl Func_0200526c
	movs r0, #7
	bl Func_0200514c
	cmp r0, #0
	beq .L_0200ba1e
	movs r1, #160
	movs r0, #7
	lsls r1, r1, #8
	movs r2, #0
	bl Func_0200526c
.L_0200ba1e:
	movs r0, #164
	movs r1, #1
	movs r2, #184
	negs r1, r1
	lsls r2, r2, #16
	movs r3, #1
	lsls r0, r0, #17
	bl Func_020052ac
	bl Func_020052b4
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #10
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #192
	lsls r0, r0, #7
	movs r2, #0
	adds r0, #10
	movs r1, #0
	bl Func_0200525c
	movs r0, #4
	movs r1, #3
	bl Object_SetModeById
	movs r0, #5
	movs r1, #3
	bl Object_SetModeById
	movs r0, #6
	movs r1, #3
	bl Object_SetModeById
	movs r0, #7
	bl Func_0200514c
	cmp r0, #0
	beq .L_0200ba8c
	movs r0, #7
	movs r1, #3
	bl Object_SetModeById
	b .L_0200ba8c
.L_0200ba88:
	.4byte 0x00001a2e
.L_0200ba8c:
	movs r1, #3
	movs r0, #9
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #16
	negs r2, r2
	movs r1, #0
	movs r0, #9
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #9
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #8
	movs r0, #10
	negs r1, r1
	movs r2, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #160
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #10
	bl Func_0200526c
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #192
	lsls r0, r0, #7
	adds r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #9
	bl Func_0200528c
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #8
	adds r1, #255
	movs r2, #30
	movs r0, #10
	bl Func_0200528c
	movs r0, #192
	lsls r0, r0, #7
	adds r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #10
	adds r1, #255
	movs r2, #30
	movs r0, #9
	bl Func_0200528c
	movs r2, #0
	movs r0, #9
	movs r1, #0
	bl Func_0200525c
	movs r1, #2
	movs r0, #10
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #192
	lsls r0, r0, #7
	adds r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #2
	adds r1, #255
	movs r2, #30
	movs r0, #9
	bl Func_0200528c
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r0, #10
	movs r1, #16
	movs r2, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #160
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #10
	bl Func_0200526c
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #192
	lsls r0, r0, #7
	adds r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #20
	movs r0, #5
	bl Func_0200528c
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #128
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #10
	bl Func_0200526c
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #192
	lsls r0, r0, #7
	movs r2, #0
	adds r0, #10
	movs r1, #0
	bl Func_0200525c
	movs r1, #2
	movs r0, #6
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #6
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #8
	movs r0, #10
	negs r1, r1
	movs r2, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #160
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #10
	bl Func_0200526c
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #192
	lsls r0, r0, #7
	adds r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r0, #4
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #35
	bl Battle_WaitMode0
	movs r1, #8
	movs r0, #10
	negs r1, r1
	movs r2, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #160
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #10
	bl Func_0200526c
	movs r0, #25
	bl Battle_WaitMode0
	movs r0, #10
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #192
	lsls r0, r0, #7
	adds r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #9
	bl Func_0200528c
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r0, #10
	movs r1, #4
	bl Object_SetModeById
	movs r0, #33
	bl Battle_WaitMode0
	movs r0, #192
	lsls r0, r0, #7
	adds r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #4
	adds r1, #255
	movs r2, #45
	movs r0, #9
	bl Func_0200528c
	movs r0, #9
	movs r1, #6
	movs r2, #15
	bl ObjectMotion_Launch
	movs r0, #9
	movs r1, #6
	movs r2, #23
	bl ObjectMotion_Launch
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #10
	bl Func_0200528c
	movs r0, #192
	lsls r0, r0, #7
	movs r2, #0
	adds r0, #10
	movs r1, #0
	bl Func_0200525c
	movs r1, #2
	movs r0, #9
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #10
	bl Func_0200526c
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #192
	lsls r0, r0, #7
	adds r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r2, #12
	negs r2, r2
	movs r1, #4
	movs r0, #10
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #10
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r2, #12
	movs r1, #4
	negs r2, r2
	movs r0, #10
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #10
	movs r1, #4
	bl Object_SetModeById
	movs r0, #33
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #10
	bl Func_0200526c
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	movs r2, #0
	adds r0, #10
	movs r1, #0
	bl Func_0200525c
	movs r0, #4
	movs r1, #3
	bl Object_SetModeById
	movs r0, #5
	movs r1, #3
	bl Object_SetModeById
	movs r0, #6
	movs r1, #3
	bl Object_SetModeById
	movs r0, #9
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #10
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	movs r2, #0
	adds r0, #10
	movs r1, #0
	bl Func_0200525c
	movs r1, #2
	movs r0, #10
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	movs r2, #0
	adds r0, #10
	movs r1, #0
	bl Func_0200525c
	movs r1, #2
	movs r0, #9
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #6
	adds r1, #255
	movs r2, #30
	movs r0, #10
	bl Func_0200528c
	movs r0, #128
	lsls r0, r0, #6
	movs r2, #0
	adds r0, #10
	movs r1, #0
	bl Func_0200525c
	movs r1, #2
	movs r0, #10
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #2
	adds r1, #255
	movs r2, #30
	movs r0, #6
	bl Func_0200528c
	movs r0, #6
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #192
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #10
	bl Func_0200526c
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #10
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #10
	bl Func_0200528c
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r0, #9
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #160
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #10
	bl Func_0200526c
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #10
	movs r1, #4
	bl Object_SetModeById
	movs r0, #33
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #128
	movs r0, #9
	lsls r1, r1, #6
	movs r2, #0
	bl Func_0200526c
	movs r1, #160
	movs r0, #4
	lsls r1, r1, #8
	movs r2, #0
	bl Func_0200526c
	movs r1, #128
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #0
	bl Func_0200526c
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl Func_0200526c
	movs r0, #7
	bl Func_0200514c
	cmp r0, #0
	beq .L_0200bf32
	movs r1, #128
	movs r0, #7
	lsls r1, r1, #8
	movs r2, #0
	bl Func_0200526c
.L_0200bf32:
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #10
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r0, #7
	bl Func_0200514c
	cmp r0, #0
	beq .L_0200bf6a
	movs r1, #160
	movs r0, #7
	lsls r1, r1, #8
	movs r2, #0
	bl Func_0200526c
.L_0200bf6a:
	movs r1, #224
	movs r0, #9
	lsls r1, r1, #8
	movs r2, #0
	bl Func_0200526c
	movs r1, #192
	movs r0, #4
	lsls r1, r1, #8
	movs r2, #0
	bl Func_0200526c
	movs r1, #192
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl Func_0200526c
	movs r1, #160
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #6
	bl Func_0200526c
	movs r0, #25
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #9
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #2
	adds r1, #255
	movs r2, #30
	movs r0, #10
	bl Func_0200528c
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r0, #9
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r0, #10
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #10
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #10
	bl Func_0200528c
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r0, #9
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #128
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #9
	bl Func_0200526c
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #9
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #0
	movs r0, #9
	movs r1, #0
	bl Func_0200525c
	movs r1, #2
	movs r0, #10
	bl Motion_SetVarCbAndRefresh
	movs r0, #15
	bl Battle_WaitMode0
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #10
	bl Func_0200526c
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #10
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	movs r2, #0
	adds r0, #10
	movs r1, #0
	bl Func_0200525c
	movs r0, #4
	movs r1, #3
	bl Object_SetModeById
	movs r0, #5
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #224
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #9
	bl Func_0200526c
	movs r0, #25
	bl Battle_WaitMode0
	movs r0, #9
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #0
	movs r0, #9
	movs r1, #0
	bl Func_0200525c
	movs r0, #10
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #10
	bl Func_02005294
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #160
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #10
	bl Func_0200526c
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r0, #9
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #10
	bl Func_0200528c
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #0
	movs r2, #0
	movs r0, #9
	bl Func_0200526c
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #16
	movs r2, #0
	movs r0, #9
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #9
	movs r1, #16
	movs r2, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #224
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #4
	bl Func_0200526c
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #9
	movs r1, #16
	movs r2, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #192
	movs r0, #10
	lsls r1, r1, #6
	movs r2, #0
	bl Func_0200526c
	movs r1, #224
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl Func_0200526c
	movs r1, #192
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #0
	bl Func_0200526c
	movs r0, #7
	bl Func_0200514c
	cmp r0, #0
	beq .L_0200c216
	movs r1, #192
	movs r0, #7
	lsls r1, r1, #8
	movs r2, #0
	bl Func_0200526c
.L_0200c216:
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #9
	movs r1, #16
	movs r2, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #224
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #6
	bl Func_0200526c
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #172
	movs r1, #1
	movs r2, #208
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #16
	lsls r0, r0, #17
	bl Func_020052ac
	bl Func_020052b4
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #10
	ldr r1, .L_0200c430
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r0, #10
	movs r1, #16
	movs r2, #16
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #179
	movs r2, #178
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #10
	adds r1, #51
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r0, #9
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #7
	bl Func_0200514c
	cmp r0, #0
	beq .L_0200c2bc
	movs r1, #160
	movs r0, #7
	lsls r1, r1, #8
	movs r2, #0
	bl Func_0200526c
.L_0200c2bc:
	movs r1, #224
	movs r0, #4
	lsls r1, r1, #8
	movs r2, #0
	bl Func_0200526c
	movs r1, #192
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl Func_0200526c
	movs r1, #192
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #0
	bl Func_0200526c
	movs r1, #160
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #9
	bl Func_0200526c
	movs r0, #30
	bl Battle_WaitMode0
	movs r2, #0
	movs r0, #9
	movs r1, #0
	bl Func_0200525c
	movs r1, #2
	movs r0, #10
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #0
	movs r0, #10
	movs r1, #0
	bl Func_0200525c
	movs r1, #2
	movs r0, #5
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #0
	movs r0, #5
	movs r1, #0
	bl Func_0200525c
	movs r1, #2
	movs r0, #10
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #160
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #10
	bl Func_0200526c
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r0, #9
	movs r1, #4
	bl Object_SetModeById
	movs r0, #33
	bl Battle_WaitMode0
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #128
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #10
	bl Func_0200526c
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r0, #9
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #131
	lsls r1, r1, #1
	movs r2, #40
	movs r0, #6
	bl Func_0200528c
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #6
	bl Func_0200526c
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #0
	movs r0, #6
	bl Func_02005254
	movs r1, #160
	movs r0, #9
	lsls r1, r1, #7
	movs r2, #0
	bl Func_0200526c
	movs r0, #4
	movs r1, #0
	movs r2, #0
	bl Func_0200526c
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl Func_0200526c
	movs r0, #7
	bl Func_0200514c
	cmp r0, #0
	beq .L_0200c400
	movs r1, #128
	movs r0, #7
	lsls r1, r1, #8
	movs r2, #0
	bl Func_0200526c
.L_0200c400:
	movs r0, #4
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_0200c434
	movs r0, #25
	bl Battle_WaitMode0
	movs r2, #0
	movs r0, #6
	movs r1, #0
	bl Func_0200525c
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_0200c456
.L_0200c430:
	.4byte 0x00013333
.L_0200c434:
	movs r0, #40
	bl Battle_WaitMode0
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #6
	adds r3, #1
	strh r3, [r2]
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
.L_0200c456:
	movs r1, #2
	adds r1, #255
	movs r2, #0
	movs r0, #9
	bl Func_0200528c
	movs r1, #2
	movs r0, #5
	adds r1, #255
	movs r2, #0
	bl Func_0200528c
	movs r0, #7
	bl Func_0200514c
	cmp r0, #0
	beq .L_0200c484
	movs r1, #2
	movs r0, #7
	adds r1, #255
	movs r2, #0
	bl Func_0200528c
.L_0200c484:
	movs r1, #2
	adds r1, #255
	movs r2, #50
	movs r0, #10
	bl Func_0200528c
	movs r1, #160
	movs r0, #9
	lsls r1, r1, #7
	movs r2, #0
	bl Func_0200526c
	movs r1, #224
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #6
	bl Func_0200526c
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #6
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #2
	adds r1, #255
	movs r2, #30
	movs r0, #9
	bl Func_0200528c
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #8
	adds r1, #255
	movs r2, #30
	movs r0, #6
	bl Func_0200528c
	movs r0, #6
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r0, #10
	movs r1, #4
	bl Object_SetModeById
	movs r0, #33
	bl Battle_WaitMode0
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #6
	bl Func_0200526c
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #4
	adds r1, #255
	movs r2, #30
	movs r0, #6
	bl Func_0200528c
	movs r0, #6
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r0, #6
	movs r1, #4
	bl Object_SetModeById
	movs r0, #33
	bl Battle_WaitMode0
	movs r0, #6
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #6
	adds r1, #255
	movs r2, #0
	movs r0, #9
	bl Func_0200528c
	movs r1, #6
	movs r2, #50
	adds r1, #255
	movs r0, #10
	bl Func_0200528c
	movs r1, #2
	movs r0, #6
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #0
	movs r0, #6
	movs r1, #0
	bl Func_0200525c
	movs r0, #4
	movs r1, #3
	bl Object_SetModeById
	movs r0, #5
	movs r1, #3
	bl Object_SetModeById
	movs r0, #10
	movs r1, #3
	bl Object_SetModeById
	movs r0, #7
	bl Func_0200514c
	cmp r0, #0
	beq .L_0200c596
	movs r0, #7
	movs r1, #3
	bl Object_SetModeById
.L_0200c596:
	movs r1, #3
	movs r0, #9
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #6
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #0
	movs r0, #6
	movs r1, #0
	bl Func_0200525c
	movs r0, #10
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	movs r0, #10
	lsls r1, r1, #1
	bl Func_02005294
	movs r0, #9
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #9
	bl Func_02005294
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #224
	movs r0, #4
	lsls r1, r1, #8
	movs r2, #0
	bl Func_0200526c
	movs r1, #224
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl Func_0200526c
	movs r1, #160
	movs r0, #9
	lsls r1, r1, #8
	movs r2, #0
	bl Func_0200526c
	movs r1, #128
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #10
	bl Func_0200526c
	movs r0, #25
	bl Battle_WaitMode0
	movs r0, #9
	movs r1, #6
	movs r2, #15
	bl ObjectMotion_Launch
	movs r2, #23
	movs r1, #6
	movs r0, #9
	bl ObjectMotion_Launch
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #9
	movs r1, #3
	bl Object_SetModeById
	movs r0, #10
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #10
	bl Battle_WaitMode0
	movs r3, #5
	movs r2, #17
	movs r1, #14
	movs r0, #20
	movs r4, #9
	str r3, [sp, #0]
	str r2, [sp, #8]
	str r1, [sp, #12]
	str r0, [sp, #16]
	movs r5, #0
	movs r0, #10
	movs r1, #9
	movs r2, #2
	movs r3, #9
	str r4, [sp, #4]
	str r4, [sp, #20]
	str r5, [sp, #24]
	bl Func_0200527c
	movs r0, #7
	bl Func_0200514c
	cmp r0, #0
	beq .L_0200c694
	movs r1, #160
	movs r0, #7
	lsls r1, r1, #8
	movs r2, #0
	bl Func_0200526c
.L_0200c694:
	movs r0, #10
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #10
	bl Func_02005294
	movs r0, #40
	bl Battle_WaitMode0
	movs r2, #0
	movs r0, #10
	movs r1, #0
	bl Func_0200525c
	movs r0, #9
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #9
	bl Func_02005294
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #10
	bl Func_0200528c
	movs r2, #0
	movs r0, #10
	movs r1, #0
	bl Func_0200525c
	movs r1, #2
	movs r0, #10
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r0, #10
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r0, #6
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #0
	movs r0, #6
	movs r1, #0
	bl Func_0200525c
	movs r1, #2
	movs r0, #9
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #192
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #9
	bl Func_0200526c
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #160
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #9
	bl Func_0200526c
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r0, #10
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #18
	movs r0, #9
	negs r1, r1
	movs r2, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #192
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl Func_0200526c
	movs r1, #160
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #9
	bl Func_0200526c
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #9
	bl Func_0200526c
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #160
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #10
	bl Func_0200526c
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #10
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #5
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #7
	bl Func_0200514c
	cmp r0, #0
	beq .L_0200c880
	movs r1, #128
	movs r0, #9
	lsls r1, r1, #6
	movs r2, #0
	bl Func_0200526c
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #160
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #10
	bl Func_0200526c
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #10
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #7
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #10
	bl Battle_WaitMode0
	b .L_0200c892
.L_0200c880:
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
.L_0200c892:
	movs r1, #42
	movs r0, #9
	negs r1, r1
	movs r2, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #192
	movs r0, #4
	lsls r1, r1, #8
	movs r2, #0
	bl Func_0200526c
	movs r1, #192
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #9
	bl Func_0200526c
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #9
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #224
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #9
	bl Func_0200526c
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #16
	movs r0, #10
	negs r1, r1
	movs r2, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #160
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #0
	bl Func_0200526c
	movs r1, #160
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #10
	bl Func_0200526c
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #10
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #4
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #25
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r0, #9
	movs r1, #4
	bl Object_SetModeById
	movs r0, #33
	bl Battle_WaitMode0
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #10
	bl Func_0200526c
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #10
	bl Func_020013b0
	movs r0, #10
	bl Func_020013b0
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #0
	movs r1, #16
	movs r0, #10
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #10
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #10
	bl Func_02005294
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #10
	bl Func_0200526c
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #16
	negs r1, r1
	movs r2, #0
	movs r0, #10
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #16
	negs r1, r1
	movs r2, #0
	movs r0, #10
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #10
	movs r1, #4
	bl Object_SetModeById
	movs r0, #33
	bl Battle_WaitMode0
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_0200526c
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #16
	movs r2, #0
	movs r0, #10
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #10
	bl Func_0200526c
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #10
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #10
	bl Func_02005294
	movs r0, #40
	bl Battle_WaitMode0
	movs r2, #12
	movs r1, #0
	negs r2, r2
	movs r0, #10
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #12
	movs r1, #0
	negs r2, r2
	movs r0, #10
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #131
	lsls r1, r1, #1
	movs r2, #40
	movs r0, #10
	bl Func_0200528c
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #10
	bl Func_0200526c
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #0
	movs r0, #10
	movs r1, #0
	bl Func_0200525c
	movs r1, #2
	movs r0, #10
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #10
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #5
	bl Battle_WaitMode0
	movs r0, #12
	bl Object_GetById
	adds r5, r0, #0
	ldr r2, [r5, #80]
	movs r3, #204
	lsls r3, r3, #7
	adds r3, #102
	str r3, [r2, #12]
	movs r1, #1
	movs r0, #12
	mov r8, r2
	bl Func_02005284
	movs r0, #12
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r1, #164
	movs r2, #160
	movs r0, #12
	lsls r1, r1, #17
	lsls r2, r2, #16
	bl Func_0200520c
	movs r0, #12
	movs r1, #9
	movs r2, #0
	bl ObjectMotion_Launch
	movs r7, #0
	movs r6, #20
.L_0200caf6:
	adds r0, r7, #0
	movs r1, #20
	bl Engine_MathDivide
	movs r3, #204
	lsls r3, r3, #7
	adds r3, #102
	adds r0, r0, r3
	mov r2, r8
	str r0, [r2, #12]
	movs r2, #128
	ldrh r3, [r5, #6]
	lsls r2, r2, #5
	adds r3, r3, r2
	strh r3, [r5, #6]
	movs r0, #1
	bl WaitFrames
	movs r3, #153
	lsls r3, r3, #8
	adds r3, #153
	subs r6, #1
	adds r7, r7, r3
	cmp r6, #0
	bge .L_0200caf6
	movs r3, #128
	lsls r3, r3, #7
	strh r3, [r5, #6]
	movs r2, #8
	movs r1, #0
	movs r0, #12
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #12
	bl Object_GetById
	movs r1, #1
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #5
	bl Battle_WaitMode0
	movs r1, #0
	movs r2, #8
	movs r0, #12
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #5
	bl Battle_WaitMode0
	movs r1, #0
	movs r2, #8
	movs r0, #12
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #5
	bl Battle_WaitMode0
	movs r1, #0
	movs r2, #8
	movs r0, #12
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #5
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #5
	bl Func_0200528c
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #5
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #2
	adds r1, #255
	movs r2, #30
	movs r0, #10
	bl Func_0200528c
	movs r1, #192
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #10
	bl Func_0200526c
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r0, #10
	movs r1, #0
	movs r2, #24
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #192
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #10
	bl Func_0200526c
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #160
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #10
	bl Func_0200526c
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	movs r1, #0
	adds r0, #10
	bl Func_02005254
	movs r1, #192
	movs r0, #9
	lsls r1, r1, #6
	movs r2, #0
	bl Func_0200526c
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl Func_0200526c
	movs r1, #128
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #0
	bl Func_0200526c
	movs r0, #7
	bl Func_0200514c
	cmp r0, #0
	beq .L_0200cc42
	movs r1, #128
	movs r0, #7
	lsls r1, r1, #8
	movs r2, #0
	bl Func_0200526c
.L_0200cc42:
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #4
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_0200cc7c
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	movs r2, #0
	adds r0, #10
	movs r1, #0
	bl Func_0200525c
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_0200cca2
.L_0200cc7c:
	movs r0, #40
	bl Battle_WaitMode0
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #128
	adds r3, #1
	lsls r0, r0, #6
	strh r3, [r2]
	adds r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
.L_0200cca2:
	movs r1, #192
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl Func_0200526c
	movs r1, #224
	movs r0, #9
	lsls r1, r1, #8
	movs r2, #0
	bl Func_0200526c
	movs r1, #160
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #0
	bl Func_0200526c
	movs r0, #7
	bl Func_0200514c
	cmp r0, #0
	beq .L_0200ccdc
	movs r1, #160
	movs r0, #7
	lsls r1, r1, #8
	movs r2, #0
	bl Func_0200526c
.L_0200ccdc:
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #10
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	ldr r1, .L_0200d0d0
	ldr r2, .L_0200d0d4
	movs r0, #10
	bl ObjectMotion_SetSpeedParameters
	movs r0, #10
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	movs r1, #160
	strb r3, [r0]
	lsls r1, r1, #7
	movs r0, #10
	movs r2, #0
	bl Func_0200526c
	movs r0, #10
	movs r1, #0
	movs r2, #8
	bl ObjectMotion_CommitPositionAndActivate
	movs r2, #8
	movs r1, #0
	negs r2, r2
	movs r0, #10
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #1
	bl Battle_WaitMode0
	movs r0, #10
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	strb r3, [r0]
	movs r0, #1
	bl Battle_WaitMode0
	movs r1, #179
	movs r2, #178
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #10
	adds r1, #51
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r1, #160
	movs r0, #10
	lsls r1, r1, #7
	movs r2, #0
	bl Func_0200526c
	movs r1, #192
	movs r0, #9
	lsls r1, r1, #6
	movs r2, #0
	bl Func_0200526c
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl Func_0200526c
	movs r0, #7
	bl Func_0200514c
	cmp r0, #0
	beq .L_0200cda2
	movs r1, #128
	movs r0, #7
	lsls r1, r1, #8
	movs r2, #0
	bl Func_0200526c
.L_0200cda2:
	movs r1, #128
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #0
	bl Func_0200526c
	movs r0, #12
	movs r1, #3
	movs r2, #9
	bl Func_0200532c
	movs r0, #1
	movs r1, #1
	movs r2, #1
	movs r3, #0
	negs r2, r2
	negs r0, r0
	negs r1, r1
	bl Func_020052ac
	movs r1, #1
	movs r0, #4
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #192
	movs r0, #4
	lsls r1, r1, #8
	movs r2, #0
	bl Func_0200526c
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #10
	bl Func_0200526c
	movs r0, #20
	bl Battle_WaitMode0
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r1, #132
	adds r3, #1
	strh r3, [r2]
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #6
	bl Func_0200528c
	movs r0, #6
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #0
	movs r2, #0
	movs r0, #5
	bl Func_0200526c
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #5
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #224
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #9
	bl Func_0200526c
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #160
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #0
	bl Func_0200526c
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #5
	bl Func_0200526c
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #10
	movs r1, #4
	bl Object_SetModeById
	movs r0, #33
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r0, #9
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #10
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #5
	bl Func_0200528c
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #5
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r1, #192
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #10
	bl Func_0200526c
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #10
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #4
	movs r1, #3
	bl Object_SetModeById
	movs r0, #5
	movs r1, #3
	bl Object_SetModeById
	movs r0, #6
	movs r1, #3
	bl Object_SetModeById
	movs r0, #7
	bl Func_0200514c
	cmp r0, #0
	beq .L_0200cf2a
	movs r0, #7
	movs r1, #3
	bl Object_SetModeById
.L_0200cf2a:
	movs r1, #3
	movs r0, #9
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #9
	movs r1, #12
	movs r2, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #160
	movs r0, #10
	lsls r1, r1, #7
	movs r2, #0
	bl Func_0200526c
	movs r1, #128
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #0
	bl Func_0200526c
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl Func_0200526c
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #9
	bl Func_0200526c
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #9
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_0200525c
	movs r0, #4
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #7
	bl Func_0200514c
	cmp r0, #0
	beq .L_0200cfbc
	movs r0, #7
	movs r1, #3
	bl Object_SetModeById
.L_0200cfbc:
	movs r0, #5
	movs r1, #3
	bl Object_SetModeById
	movs r0, #6
	movs r1, #3
	bl Object_SetModeById
	movs r0, #4
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #153
	lsls r2, r2, #8
	adds r2, #153
	movs r0, #9
	ldr r1, .L_0200d0d4
	bl ObjectMotion_SetSpeedParameters
	movs r0, #9
	movs r1, #2
	bl Object_SetModeById
	ldr r3, .L_0200d0d8
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r3, r2
	ldr r0, [r5]
	bl Object_GetById
	cmp r0, #0
	beq .L_0200d016
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #9
	bl ObjectMotion_ResetAndSetPosition
.L_0200d016:
	movs r0, #9
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_0200520c
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #5
	ldr r1, .L_0200d0d4
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r0, #5
	movs r1, #2
	bl Object_SetModeById
	ldr r0, [r5]
	bl Object_GetById
	cmp r0, #0
	beq .L_0200d054
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #5
	bl ObjectMotion_ResetAndSetPosition
.L_0200d054:
	movs r0, #5
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl Func_0200520c
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #6
	ldr r1, .L_0200d0d4
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r0, #6
	movs r1, #2
	bl Object_SetModeById
	ldr r0, [r5]
	bl Object_GetById
	cmp r0, #0
	beq .L_0200d092
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #6
	bl ObjectMotion_ResetAndSetPosition
.L_0200d092:
	movs r0, #6
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #6
	movs r1, #0
	movs r2, #0
	bl Func_0200520c
	movs r0, #7
	bl Func_0200514c
	cmp r0, #0
	beq .L_0200d0fa
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #7
	ldr r1, .L_0200d0d4
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r0, #7
	movs r1, #2
	bl Object_SetModeById
	ldr r0, [r5]
	bl Object_GetById
	cmp r0, #0
	beq .L_0200d0ea
	b .L_0200d0dc
	.2byte 0x0000
.L_0200d0d0:
	.4byte 0x00026666
.L_0200d0d4:
	.4byte 0x00013333
.L_0200d0d8:
	.4byte gPartyState
.L_0200d0dc:
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #7
	bl ObjectMotion_ResetAndSetPosition
.L_0200d0ea:
	movs r0, #7
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #7
	movs r1, #0
	movs r2, #0
	bl Func_0200520c
.L_0200d0fa:
	ldr r3, .L_0200d118
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	movs r1, #1
	bl Func_0200529c
	bl Func_020051b4
	add sp, #28
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200d118:
	.4byte gPartyState
	.section .rodata.x0200d39c,"a",%progbits
.L_0200d39c:
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
.L_0200d3d8:
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
.L_0200d414:
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
	.global Data_02005450
Data_02005450:
	.4byte 0x00090008
	.4byte 0x000b000a
	.4byte 0x0000ffff
	.global Data_0200545c
Data_0200545c:
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00005000
	.4byte 0x00000000
	.4byte 0x0000000c
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00003000
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00003000
	.4byte 0x00000000
	.4byte 0x00000007
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00005000
	.4byte 0x00000000
	.4byte 0x00000022
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0x00000000
	.global Data_020054b8
Data_020054b8:
	.4byte 0x001f001f
	.4byte 0x001d001f
	.4byte 0x001e001d
	.4byte 0x001c001b
	.4byte 0x0019001d
	.4byte 0x001c001b
	.4byte 0x00190017
	.4byte 0x0015001b
	.4byte 0x001a0018
	.4byte 0x0012000f
	.4byte 0x000a0013
	.4byte 0x000d000c
	.4byte 0x00060005
	.2byte 0x0006
	.global Data_020054ee
Data_020054ee:
	.2byte 0x0018
	.4byte 0x001e001c
	.4byte 0x00180013
	.4byte 0x000f001b
	.4byte 0x00190014
	.4byte 0x0010000b
	.4byte 0x00070017
	.4byte 0x0015000c
	.4byte 0x00080003
	.4byte 0x001b0013
	.4byte 0x001b001b
	.4byte 0x00180010
	.4byte 0x0000001d
	.global Data_02005520
Data_02005520:
	.4byte .L_0200d39c
	.4byte .L_0200d3d8
	.4byte .L_0200d414
.L_0200d52c:
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000000
	.4byte 0x00000017
	.4byte 0x00000006
	.4byte 0xfffc0000
	.4byte 0x00000017
	.4byte 0x00000005
	.4byte 0x00060000
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x0000cccc
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x0000cccc
	.4byte 0x0000002e
	.4byte Func_02000420
	.4byte 0x00000011
	.global Data_02005574
Data_02005574:
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
	.global Data_020055a4
Data_020055a4:
	.4byte 0x0000003b
	.4byte 0x10101040
	.4byte 0xffffffff
	.4byte 0x10204041
	.4byte 0xffffffff
	.4byte 0x10303040
	.4byte 0xffffffff
	.4byte 0x10404040
	.4byte 0xffffffff
	.4byte 0x10505040
	.4byte 0xffffffff
	.4byte 0x10602042
	.4byte 0xffffffff
	.4byte 0x1070403e
	.4byte 0xffffffff
	.4byte 0x1080103e
	.4byte 0xffffffff
	.4byte 0x1090b03e
	.4byte 0xffffffff
	.4byte 0x10a0203c
	.4byte 0xffffffff
	.4byte 0x10b0303c
	.4byte 0xffffffff
	.4byte 0x10c0103f
	.4byte 0xffffffff
	.4byte 0x0000003c
	.4byte 0x1010203d
	.4byte 0xffffffff
	.4byte 0x1020a03b
	.4byte 0xffffffff
	.4byte 0x1030b03b
	.4byte 0xffffffff
	.4byte 0x0000003d
	.4byte 0x1010f002
	.4byte 0xffffffff
	.4byte 0x1020103c
	.4byte 0xffffffff
	.4byte 0x0000003e
	.4byte 0x1010803b
	.4byte 0xffffffff
	.4byte 0x1020303e
	.4byte 0xffffffff
	.4byte 0x1030203e
	.4byte 0xffffffff
	.4byte 0x1040703b
	.4byte 0xffffffff
	.4byte 0x10503041
	.4byte 0xffffffff
	.4byte 0x10605041
	.4byte 0xffffffff
	.4byte 0x10706040
	.4byte 0xffffffff
	.4byte 0x10807040
	.4byte 0xffffffff
	.4byte 0x1090a03e
	.4byte 0xffffffff
	.4byte 0x10a0903e
	.4byte 0xffffffff
	.4byte 0x10b0903b
	.4byte 0xffffffff
	.4byte 0x0000003f
	.4byte 0x1010c03b
	.4byte 0xffffffff
	.4byte 0x1020403f
	.4byte 0xffffffff
	.4byte 0x1030503f
	.4byte 0xffffffff
	.4byte 0x1040203f
	.4byte 0xffffffff
	.4byte 0x1050303f
	.4byte 0xffffffff
	.4byte 0x1060703f
	.4byte 0xffffffff
	.4byte 0x1070603f
	.4byte 0xffffffff
	.4byte 0x10b63040
	.4byte 0xffffffff
	.4byte 0x000001ff
	.global Data_020056dc
Data_020056dc:
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020056f4
Data_020056f4:
	.4byte 0xffff0083
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x01900000
	.4byte 0x00010000
	.4byte 0xffff0083
	.4byte 0x00000001
	.4byte 0x01a00000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00014000
	.4byte 0xffff0083
	.4byte 0x00000001
	.4byte 0x00c00000
	.4byte 0x00000000
	.4byte 0x01300000
	.4byte 0x00010000
	.4byte 0xffff0083
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x00018000
	.4byte 0xffff0083
	.4byte 0x00000001
	.4byte 0x01700000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00014000
	.4byte 0xffff0130
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x0002c000
	.4byte 0xffff0130
	.4byte 0x00000001
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x0102c000
	.4byte 0xffff0130
	.4byte 0x00000001
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x0002c000
	.4byte 0xffff0130
	.4byte 0x00000001
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x0002c000
	.4byte 0xffff0130
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x0002c000
	.4byte 0xffff0130
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x0002c000
	.4byte 0xffff0130
	.4byte 0x00000001
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x0002c000
	.4byte 0xffff0130
	.4byte 0x00000001
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x0102c000
	.4byte 0xffff002a
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0002c000
	.4byte 0xffff01e9
	.4byte 0x00000001
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
	.global Data_02005874
Data_02005874:
	.4byte 0xffff00c0
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x01900000
	.4byte 0x00010000
	.4byte 0xffff00be
	.4byte 0x00000001
	.4byte 0x01a00000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00014000
	.4byte 0xffff00b9
	.4byte 0x00000001
	.4byte 0x00c00000
	.4byte 0x00000000
	.4byte 0x01300000
	.4byte 0x00010000
	.4byte 0xffff00c4
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x00018000
	.4byte 0xffff00bc
	.4byte 0x00000001
	.4byte 0x01700000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00014000
	.4byte 0xffff00bf
	.4byte 0x00000002
	.4byte 0x01800000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00014000
	.4byte 0xffff0130
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x0002c000
	.4byte 0xffff0130
	.4byte 0x00000001
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x0102c000
	.4byte 0xffff0130
	.4byte 0x00000001
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x0002c000
	.4byte 0xffff0130
	.4byte 0x00000001
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x0002c000
	.4byte 0xffff0130
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x0002c000
	.4byte 0xffff0130
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x0002c000
	.4byte 0xffff0130
	.4byte 0x00000001
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x0002c000
	.4byte 0xffff0130
	.4byte 0x00000001
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x0102c000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020059dc
Data_020059dc:
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x01d80000
	.4byte 0x0002c000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x0002c000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x0002c000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x0002c000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02005a54
Data_02005a54:
	.4byte 0xffff0132
	.4byte .L_0200d52c
	.4byte 0x00800000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x0002c000
	.4byte 0xffff0132
	.4byte .L_0200d52c
	.4byte 0x00d00000
	.4byte 0x00000000
	.4byte 0x00380000
	.4byte 0x0002c000
	.4byte 0xffff0132
	.4byte .L_0200d52c
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x0002c000
	.4byte 0xffff0132
	.4byte .L_0200d52c
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x0002c000
	.4byte 0xffff0132
	.4byte .L_0200d52c
	.4byte 0x01600000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x0002c000
	.4byte 0xffff0132
	.4byte .L_0200d52c
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x0002c000
	.4byte 0xffff0132
	.4byte .L_0200d52c
	.4byte 0x00600000
	.4byte 0x00000000
	.4byte 0x01e80000
	.4byte 0x0002c000
	.4byte 0xffff0132
	.4byte .L_0200d52c
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x01b80000
	.4byte 0x0002c000
	.4byte 0xffff0132
	.4byte .L_0200d52c
	.4byte 0x01c80000
	.4byte 0x00000000
	.4byte 0x01600000
	.4byte 0x0002c000
	.4byte 0xffff0132
	.4byte .L_0200d52c
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x0002c000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02005b5c
Data_02005b5c:
	.4byte 0xffff0016
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
	.4byte 0x0002c000
	.4byte 0xffff0005
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0002c000
	.4byte 0xffff0006
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0002c000
	.4byte 0xffff0007
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0002c000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x0002c000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x0102c000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02005c1c
Data_02005c1c:
	.4byte 0xffff0016
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
	.4byte 0x0002c000
	.4byte 0xffff002a
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0001c000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0002c000
	.4byte 0x007500f6
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
	.4byte 0x0002c000
	.4byte 0xffff0006
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0002c000
	.4byte 0xffff0007
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0002c000
	.4byte 0xffff0132
	.4byte .L_0200d52c
	.4byte 0x00400000
	.4byte 0x00000000
	.4byte 0x02320000
	.4byte 0x0002c000
	.4byte 0xffff0132
	.4byte .L_0200d52c
	.4byte 0x01720000
	.4byte 0x00000000
	.4byte 0x00380000
	.4byte 0x0002c000
	.4byte 0xffff0132
	.4byte .L_0200d52c
	.4byte 0x01000000
	.4byte 0x00000000
	.4byte 0x00500000
	.4byte 0x0002c000
	.4byte 0xffff0132
	.4byte .L_0200d52c
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x01600000
	.4byte 0x0102c000
	.4byte 0xffff0132
	.4byte .L_0200d52c
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x01700000
	.4byte 0x0102c000
	.4byte 0xffff0132
	.4byte .L_0200d52c
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x02720000
	.4byte 0x0102c000
	.4byte 0xffff0132
	.4byte .L_0200d52c
	.4byte 0x01500000
	.4byte 0x00000000
	.4byte 0x03100000
	.4byte 0x0102c000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
.L_0200dd9c:
	.4byte 0x00200052
	.4byte 0x00020001
	.4byte 0x00500006
	.4byte 0x00010020
	.4byte 0x00060002
	.2byte 0xffff
.L_0200ddb2:
	.2byte 0x0052
	.4byte 0x00010023
	.4byte 0x00060002
	.4byte 0x00230050
	.4byte 0x00020001
	.4byte 0xffff0006
.L_0200ddc8:
	.4byte 0x0020004e
	.4byte 0x00020002
	.4byte 0x004e0006
	.4byte 0x00020023
	.4byte 0x00060002
	.4byte 0x0000ffff
	.global Data_02005de0
Data_02005de0:
	.4byte .L_0200dd9c
	.4byte 0x0015004e
	.4byte .L_0200dd9c
	.4byte 0x00000000
	.4byte .L_0200dd9c
	.4byte 0x000a0048
	.4byte .L_0200ddb2
	.4byte 0x0016004a
	.4byte .L_0200dd9c
	.4byte 0x00170055
	.4byte .L_0200ddc8
	.4byte 0x000d004e
	.global Data_02005e10
Data_02005e10:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02005e34
Data_02005e34:
	.4byte 0x0000c602
	.4byte 0xffff0001
	.4byte Func_020004cc
	.4byte 0x0000c401
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x0000c602
	.4byte 0xffff0003
	.4byte Func_020004cc
	.4byte 0x0000c602
	.4byte 0xffff0004
	.4byte Func_020004cc
	.4byte 0x0000c602
	.4byte 0xffff0005
	.4byte Func_020004cc
	.4byte 0x0000c602
	.4byte 0xffff0006
	.4byte Func_020004cc
	.4byte 0x00000031
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x0000c401
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
	.4byte 0x0000c402
	.4byte 0xffff000c
	.4byte Func_02000960
	.4byte 0x00000002
	.4byte 0x091e0014
	.4byte Func_02000808
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte Func_02000580
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00001977
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte Func_02000698
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x0000197b
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte Func_020005cc
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x0000197f
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00001980
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00001981
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00001982
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00001983
	.4byte 0x50009085
	.4byte 0xffff0000
	.4byte Func_02000fbc
	.4byte 0x40009085
	.4byte 0xffff0000
	.4byte Func_0200105c
	.4byte 0x000001f3
	.4byte 0xffff00c8
	.4byte 0x0040303f
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02005f78
Data_02005f78:
	.4byte 0x0000c602
	.4byte 0xffff0001
	.4byte Func_020004cc
	.4byte 0x0000c401
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x0000c602
	.4byte 0xffff0003
	.4byte Func_020004cc
	.4byte 0x0000c602
	.4byte 0xffff0004
	.4byte Func_020004cc
	.4byte 0x0000c602
	.4byte 0xffff0005
	.4byte Func_020004cc
	.4byte 0x0000c602
	.4byte 0xffff0006
	.4byte Func_020004cc
	.4byte 0x00000031
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x0000c401
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
	.4byte 0x0000c402
	.4byte 0xffff000c
	.4byte Func_02000960
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x000019f9
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x000019fa
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte Func_02000664
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte Func_02000618
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x00001a00
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte Func_020006ec
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001a05
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00001a06
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00001a07
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00001a08
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00001a09
	.4byte 0x00008d15
	.4byte 0x0301040d
	.4byte Func_020006ec
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00001a0a
	.4byte 0x50009085
	.4byte 0xffff0000
	.4byte Func_02000fbc
	.4byte 0x40009085
	.4byte 0xffff0000
	.4byte Func_0200105c
	.4byte 0x000001f3
	.4byte 0xffff00c8
	.4byte 0x0040303f
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020060d4
Data_020060d4:
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
	.4byte 0x091c001e
	.4byte Func_02001490
	.4byte 0x40009085
	.4byte 0xffff0000
	.4byte Func_0200108c
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_0200611c
Data_0200611c:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000002
	.4byte 0x02100014
	.4byte Func_02001144
	.4byte 0x00000002
	.4byte 0x02110015
	.4byte Func_020011c8
	.4byte 0x10008c15
	.4byte Data_02010002 + 0x6
	.4byte Func_02000e58
	.4byte 0x00008c15
	.4byte Data_02010002 + 0x6
	.4byte Func_02000f74
	.4byte 0x10008c15
	.4byte Data_02020004 + 0x5
	.4byte Func_02000e58
	.4byte 0x00008c15
	.4byte Data_02020004 + 0x5
	.4byte Func_02000f80
	.4byte 0x10008c15
	.4byte Data_02030000 + 0xa
	.4byte Func_02000e58
	.4byte 0x00008c15
	.4byte Data_02030000 + 0xa
	.4byte Func_02000f8c
	.4byte 0x10008c15
	.4byte 0x0204000b
	.4byte Func_02000e58
	.4byte 0x00008c15
	.4byte 0x0204000b
	.4byte Func_02000f98
	.4byte 0x00000008
	.4byte gMapCellBuffer
	.4byte Func_02000e68
	.4byte 0x00000009
	.4byte gMapCellBuffer
	.4byte Func_02000fa4
	.4byte 0x40009085
	.4byte 0xffff0000
	.4byte Func_02001098
	.global Data_020061d0
Data_020061d0:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000031
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000021
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000021
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000021
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000021
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000021
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000021
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000031
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000021
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x50009085
	.4byte 0xffff0000
	.4byte Func_02000fec
	.4byte 0x40009085
	.4byte 0xffff0000
	.4byte Func_020010d4
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02006278
Data_02006278:
	.4byte 0x00000021
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
	.4byte 0x0000c402
	.4byte 0xffff0006
	.4byte Func_02000960
	.4byte 0x00000021
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000021
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000002
	.4byte 0x091d001e
	.4byte Func_020022f4
	.4byte 0x00000002
	.4byte 0xffff001f
	.4byte Func_020009a4
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x00001a8d
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00001a8e
	.4byte 0x50009085
	.4byte 0xffff0000
	.4byte Func_0200101c
	.4byte 0x40009085
	.4byte 0xffff0000
	.4byte Func_02001104
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_0200632c
Data_0200632c:
	.4byte 0x00000000
	.global Data_02006330
Data_02006330:
	.4byte 0x00000026
