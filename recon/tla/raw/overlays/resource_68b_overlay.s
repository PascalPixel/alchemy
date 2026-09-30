.syntax unified
	.thumb
	.section .text.x02008038,"ax",%progbits
	.global Func_02000038
	.thumb_func
Func_02000038:
	push {r5, lr}
	ldmia r0!, {r5}
	ldmia r1!, {r3}
	ldmia r0!, {r4}
	subs r5, r5, r3
	ldmia r1!, {r3}
	asrs r5, r5, #16
	ldr r2, [r1]
	subs r4, r4, r3
	ldr r3, [r0]
	asrs r4, r4, #16
	subs r3, r3, r2
	asrs r3, r3, #16
	adds r0, r5, #0
	muls r0, r5
	adds r2, r4, #0
	muls r2, r4
	adds r1, r3, #0
	muls r1, r3
	adds r0, r0, r2
	adds r3, r1, #0
	adds r0, r0, r3
	ldr r3, .L_0200806c
	mov lr, r3
	.2byte 0xf800
	pop {r5, pc}
.L_0200806c:
	.4byte IwramFillWords + 0x74
	.section .text.x02008070,"ax",%progbits
	.global Func_02000070
	.thumb_func
Func_02000070:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	mov r8, r0
	adds r6, r1, #0
	adds r7, r6, #0
	mov r5, r8
	adds r7, #8
	adds r5, #8
	mov r10, r2
	adds r0, r7, #0
	movs r2, #0
	adds r1, r5, #0
	mov r9, r2
	bl Func_02000038
	cmp r0, r10
	bge .L_020080d8
	mov r2, r8
	ldr r3, [r2, #16]
	ldr r0, [r6, #16]
	ldr r1, [r7]
	subs r0, r0, r3
	ldr r3, [r5]
	movs r5, #128
	subs r1, r1, r3
	bl ArcTan2
	lsls r0, r0, #16
	lsrs r0, r0, #16
	ldr r3, .L_020080e4
	lsls r5, r5, #5
	adds r1, r0, r5
	mov r5, r8
	ldrh r2, [r5, #6]
	adds r4, r0, r3
	movs r3, #240
	lsls r3, r3, #8
	ands r4, r3
	ands r1, r3
	ands r0, r3
	ands r3, r2
	cmp r0, r3
	beq .L_020080d4
	cmp r1, r3
	beq .L_020080d4
	cmp r4, r3
	bne .L_020080d8
.L_020080d4:
	movs r2, #1
	mov r9, r2
.L_020080d8:
	mov r0, r9
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_020080e4:
	.4byte 0xfffff000
	.section .text.x020080e8,"ax",%progbits
	.global Func_020000e8
	.thumb_func
Func_020000e8:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #108]
	ldr r3, .L_0200819c
	movs r2, #133
	lsls r2, r2, #2
	adds r7, r3, r2
	adds r6, r0, #0
	ldr r0, [r7]
	bl Object_GetById
	bl Func_02001d54
	adds r1, r0, #0
	adds r0, r6, #0
	bl Func_02001ba4
	movs r2, #179
	lsls r2, r2, #1
	adds r3, r5, r2
	ldrh r2, [r3]
	movs r3, #192
	lsls r3, r3, #2
	adds r3, #255
	adds r5, r6, #0
	ands r3, r2
	adds r5, #91
	cmp r3, #141
	bne .L_0200813e
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #224
	ldr r3, [r3]
	movs r2, #224
	lsls r2, r2, #3
	adds r2, #19
	adds r3, r3, r2
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	bne .L_0200818c
.L_0200813e:
	adds r3, r6, #0
	adds r3, #100
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #1
	beq .L_0200818c
	ldr r0, [r7]
	bl Object_GetById
	adds r1, r0, #0
	adds r0, r6, #0
	adds r0, #8
	adds r1, #8
	bl Func_02000038
	cmp r0, #11
	ble .L_02008182
	ldr r1, [r6, #104]
	adds r0, r6, #0
	movs r2, #18
	bl Func_02000070
	cmp r0, #0
	bne .L_0200818c
	ldr r0, [r7]
	bl Object_GetById
	movs r2, #26
	adds r1, r0, #0
	adds r0, r6, #0
	bl Func_02000070
	cmp r0, #0
	bne .L_0200818c
.L_02008182:
	movs r3, #0
	adds r0, r6, #0
	strb r3, [r5]
	movs r1, #2
	b .L_02008194
.L_0200818c:
	movs r3, #1
	adds r0, r6, #0
	strb r3, [r5]
	movs r1, #1
.L_02008194:
	bl Func_02001b7c
	movs r0, #0
	pop {r5, r6, r7, pc}
.L_0200819c:
	.4byte gPartyState
	.section .text.x020081a0,"ax",%progbits
	.global Func_020001a0
	.thumb_func
Func_020001a0:
	ldr r0, .L_020081a4
	bx lr
.L_020081a4:
	.4byte Data_02001e84
	.section .text.x020081a8,"ax",%progbits
	.global Func_020001a8
	.thumb_func
Func_020001a8:
	movs r0, #0
	bx lr
	.section .text.x020081ac,"ax",%progbits
	.global Func_020001ac
	.thumb_func
Func_020001ac:
	ldr r0, .L_020081b0
	bx lr
.L_020081b0:
	.4byte Data_02001eb4
	.section .text.x020081b4,"ax",%progbits
	.global Func_020001b4
	.thumb_func
Func_020001b4:
	ldr r0, .L_020081b8
	bx lr
.L_020081b8:
	.4byte Data_02001f14
	.section .text.x020081bc,"ax",%progbits
	.global Func_020001bc
	.thumb_func
Func_020001bc:
	push {lr}
	movs r1, #244
	movs r2, #157
	lsls r2, r2, #19
	lsls r1, r1, #17
	movs r0, #12
	bl Func_02001c2c
	movs r0, #12
	bl Object_GetById
	movs r1, #157
	bl Animation_SetIndexAndInitObjects
	movs r0, #12
	movs r1, #3
	bl Object_SetModeById
	movs r1, #128
	movs r0, #12
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	pop {pc}
	.2byte 0x0000
	.section .text.x020081f0,"ax",%progbits
	.global Func_020001f0
	.thumb_func
Func_020001f0:
	push {r5, r6, r7, lr}
	adds r5, r0, #0
	adds r7, r1, #0
	adds r6, r2, #0
	bl Func_02001bcc
	movs r0, #0
	bl Func_02001d04
	movs r0, #158
	bl Func_02001d7c
	ldrh r1, [r5, #4]
	ldrh r2, [r5, #6]
	ldr r0, [r5]
	bl Func_02001b84
	ldr r5, .L_02008274
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	ldr r0, [r5]
	bl Object_GetById
	movs r3, #2
	adds r0, #85
	strb r3, [r0]
	movs r1, #128
	movs r2, #128
	ldr r0, [r5]
	lsls r1, r1, #8
	lsls r2, r2, #7
	bl ObjectMotion_SetSpeedParameters
	ldr r0, [r5]
	movs r1, #2
	bl Object_SetModeById
	ldr r0, [r5]
	cmp r6, #0
	bne .L_0200824e
	movs r2, #8
	movs r1, #2
	negs r2, r2
	bl ObjectMotion_SnapHeadingAndOffset
	b .L_02008258
.L_0200824e:
	movs r2, #8
	movs r1, #0
	negs r2, r2
	bl ObjectMotion_OffsetPositionAndResetMotion
.L_02008258:
	movs r0, #10
	bl Battle_WaitMode0
	adds r0, r7, #0
	bl Func_02001cd4
	bl Event_ClearStatus1c6
	bl Event_WaitValue1c8Frames
	bl Func_02001bd4
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008274:
	.4byte gPartyState
	.section .text.x02008278,"ax",%progbits
	.global Func_02000278
	.thumb_func
Func_02000278:
	push {r5, r6, r7, lr}
	adds r5, r0, #0
	ldr r6, .L_020082e8
	movs r7, #0
	cmp r5, #3
	bne .L_020082a4
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #106
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020082a4
	ldr r0, .L_020082ec
	bl Func_02001c6c
	movs r0, #1
	negs r0, r0
	movs r1, #0
	bl Func_02001c84
	b .L_020082e6
.L_020082a4:
	cmp r5, #1
	bne .L_020082ac
	ldr r6, .L_020082e8
	movs r7, #0
.L_020082ac:
	cmp r5, #2
	bne .L_020082b4
	ldr r6, .L_020082f0
	movs r7, #0
.L_020082b4:
	cmp r5, #3
	bne .L_020082bc
	ldr r6, .L_020082f4
	movs r7, #1
.L_020082bc:
	cmp r5, #12
	bne .L_020082c4
	ldr r6, .L_020082f8
	movs r7, #0
.L_020082c4:
	cmp r5, #11
	bne .L_020082cc
	ldr r6, .L_020082fc
	movs r7, #1
.L_020082cc:
	cmp r5, #10
	bne .L_020082d4
	ldr r6, .L_02008300
	movs r7, #1
.L_020082d4:
	cmp r5, #16
	bne .L_020082dc
	ldr r6, .L_02008304
	movs r7, #0
.L_020082dc:
	adds r0, r6, #0
	adds r1, r5, #0
	adds r2, r7, #0
	bl Func_020001f0
.L_020082e6:
	pop {r5, r6, r7, pc}
.L_020082e8:
	.4byte Data_020021bc
.L_020082ec:
	.4byte 0x00002880
.L_020082f0:
	.4byte Data_020021c4
.L_020082f4:
	.4byte Data_020021cc
.L_020082f8:
	.4byte Data_020021d4
.L_020082fc:
	.4byte Data_020021dc
.L_02008300:
	.4byte Data_020021e4
.L_02008304:
	.4byte Data_020021ec
	.section .text.x02008308,"ax",%progbits
	.global Func_02000308
	.thumb_func
Func_02000308:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	movs r0, #18
	ldr r5, [r3, #108]
	bl Object_GetById
	adds r6, r0, #0
	movs r3, #6
	ldrsh r2, [r6, r3]
	adds r7, r6, #0
	movs r3, #1
	adds r7, #100
	strh r3, [r7]
	mov r8, r2
	bl Func_02001bcc
	movs r0, #0
	bl Func_02001d04
	movs r2, #179
	lsls r2, r2, #1
	adds r3, r5, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	bne .L_0200835c
	ldr r0, .L_0200838c
	bl Func_02001c6c
	ldr r3, .L_02008390
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r1, [r3]
	movs r0, #18
	movs r2, #2
	bl Object_LinkPair
	b .L_02008362
.L_0200835c:
	ldr r0, .L_02008394
	bl Func_02001c6c
.L_02008362:
	movs r0, #18
	movs r1, #0
	bl Object_SetModeById
	movs r0, #18
	movs r1, #0
	movs r2, #10
	bl Func_02001c7c
	bl Func_02001bd4
	movs r3, #0
	strh r3, [r7]
	mov r3, r8
	strh r3, [r6, #6]
	movs r0, #1
	bl WaitFrames
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_0200838c:
	.4byte 0x00002735
.L_02008390:
	.4byte gPartyState
.L_02008394:
	.4byte 0x00002742
	.section .text.x02008398,"ax",%progbits
	.global Func_02000398
	.thumb_func
Func_02000398:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	movs r0, #19
	ldr r5, [r3, #108]
	bl Object_GetById
	adds r6, r0, #0
	movs r3, #6
	ldrsh r2, [r6, r3]
	adds r7, r6, #0
	movs r3, #1
	adds r7, #100
	strh r3, [r7]
	mov r8, r2
	bl Func_02001bcc
	movs r0, #0
	bl Func_02001d04
	movs r2, #179
	lsls r2, r2, #1
	adds r3, r5, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	bne .L_020083ec
	ldr r0, .L_0200841c
	bl Func_02001c6c
	ldr r3, .L_02008420
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r1, [r3]
	movs r0, #19
	movs r2, #2
	bl Object_LinkPair
	b .L_020083f2
.L_020083ec:
	ldr r0, .L_02008424
	bl Func_02001c6c
.L_020083f2:
	movs r0, #19
	movs r1, #0
	bl Object_SetModeById
	movs r0, #19
	movs r1, #0
	movs r2, #10
	bl Func_02001c7c
	bl Func_02001bd4
	movs r3, #0
	strh r3, [r7]
	mov r3, r8
	strh r3, [r6, #6]
	movs r0, #1
	bl WaitFrames
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_0200841c:
	.4byte 0x00002736
.L_02008420:
	.4byte gPartyState
.L_02008424:
	.4byte 0x00002743
	.section .text.x02008428,"ax",%progbits
	.global Func_02000428
	.thumb_func
Func_02000428:
	push {lr}
	sub sp, #12
	movs r3, #39
	movs r2, #71
	movs r1, #2
	str r3, [sp, #0]
	str r2, [sp, #4]
	str r1, [sp, #8]
	movs r0, #37
	movs r1, #72
	movs r2, #2
	movs r3, #2
	bl Func_02001d44
	add sp, #12
	pop {pc}
	.section .text.x02008448,"ax",%progbits
	.global Func_02000448
	.thumb_func
Func_02000448:
	push {lr}
	sub sp, #12
	movs r3, #49
	movs r2, #71
	movs r1, #2
	str r3, [sp, #0]
	str r2, [sp, #4]
	str r1, [sp, #8]
	movs r0, #59
	movs r1, #64
	movs r2, #4
	movs r3, #3
	bl Func_02001d44
	add sp, #12
	pop {pc}
	.section .text.x02008468,"ax",%progbits
	.global Func_02000468
	.thumb_func
Func_02000468:
	push {lr}
	sub sp, #12
	movs r3, #53
	movs r2, #69
	movs r1, #2
	str r3, [sp, #0]
	str r2, [sp, #4]
	str r1, [sp, #8]
	movs r0, #52
	movs r1, #69
	movs r2, #1
	movs r3, #1
	bl Func_02001d44
	add sp, #12
	pop {pc}
	.section .text.x02008488,"ax",%progbits
	.global Func_02000488
	.thumb_func
Func_02000488:
	push {lr}
	sub sp, #12
	movs r3, #52
	movs r2, #64
	movs r1, #2
	str r3, [sp, #0]
	str r2, [sp, #4]
	str r1, [sp, #8]
	movs r0, #53
	movs r1, #64
	movs r2, #1
	movs r3, #1
	bl Func_02001d44
	add sp, #12
	pop {pc}
	.section .text.x020084a8,"ax",%progbits
	.global Func_020004a8
	.thumb_func
Func_020004a8:
	push {r5, r6, lr}
	ldr r3, .L_02008534
	movs r2, #133
	lsls r2, r2, #2
	adds r6, r3, r2
	ldr r0, [r6]
	sub sp, #12
	bl Object_GetById
	movs r3, #49
	movs r2, #71
	movs r1, #2
	str r3, [sp, #0]
	str r2, [sp, #4]
	str r1, [sp, #8]
	adds r5, r0, #0
	movs r1, #64
	movs r0, #59
	movs r2, #4
	movs r3, #3
	bl Func_02001d44
	movs r0, #240
	lsls r0, r0, #4
	adds r0, #187
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200852e
	movs r1, #202
	movs r2, #147
	movs r0, #69
	lsls r1, r1, #18
	lsls r2, r2, #19
	bl Func_02001c2c
	ldr r3, [r5, #8]
	asrs r3, r3, #20
	cmp r3, #50
	bne .L_0200852e
	ldr r3, [r5, #16]
	asrs r3, r3, #20
	cmp r3, #73
	bne .L_0200852e
	bl Func_02001bcc
	movs r0, #0
	bl Func_02001d04
	movs r1, #129
	ldr r0, [r6]
	lsls r1, r1, #1
	bl Func_02001cac
	ldr r0, [r6]
	movs r1, #16
	movs r2, #0
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r3, #192
	lsls r3, r3, #11
	str r3, [r5, #40]
	ldr r0, [r6]
	bl ObjectMotion_CommitCurrentPositionAndActivate
	bl Func_02001bd4
.L_0200852e:
	add sp, #12
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02008534:
	.4byte gPartyState
	.section .text.x02008538,"ax",%progbits
	.global Func_02000538
	.thumb_func
Func_02000538:
	push {lr}
	bl Func_02001d74
	ldr r0, .L_02008558
	movs r1, #2
	bl Func_02001ccc
	ldr r3, .L_0200855c
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #194
	adds r3, r3, r2
	movs r2, #2
	strb r2, [r3]
	pop {pc}
	.2byte 0x0000
.L_02008558:
	.4byte 0x000000ca
.L_0200855c:
	.4byte gPartyState
	.section .text.x02008560,"ax",%progbits
	.global Func_02000560
	.thumb_func
Func_02000560:
	push {r5, r6, r7, lr}
	movs r0, #144
	lsls r0, r0, #4
	movs r5, #214
	movs r7, #254
	lsls r5, r5, #18
	lsls r7, r7, #18
	adds r0, #99
	bl GameFlag_SetBit
	adds r1, r7, #0
	adds r0, r5, #0
	movs r2, #0
	bl Func_02001d5c
	movs r6, #64
	adds r1, r7, #0
	adds r3, r0, #0
	movs r7, #129
	orrs r3, r6
	lsls r7, r7, #19
	adds r0, r5, #0
	movs r2, #0
	bl Func_02001d64
	adds r1, r7, #0
	adds r0, r5, #0
	movs r2, #0
	bl Func_02001d5c
	adds r3, r0, #0
	orrs r3, r6
	adds r0, r5, #0
	adds r1, r7, #0
	movs r2, #0
	bl Func_02001d64
	pop {r5, r6, r7, pc}
	.section .text.x020085ac,"ax",%progbits
	.global Func_020005ac
	.thumb_func
Func_020005ac:
	push {lr}
	movs r2, #192
	movs r1, #64
	lsls r2, r2, #2
	bl Func_02001cf4
	pop {pc}
	.2byte 0x0000
	.section .text.x020085bc,"ax",%progbits
	.global Func_020005bc
	.thumb_func
Func_020005bc:
	push {lr}
	movs r2, #192
	lsls r2, r2, #2
	movs r1, #65
	adds r2, #1
	bl Func_02001cf4
	pop {pc}
	.section .text.x020085cc,"ax",%progbits
	.global Func_020005cc
	.thumb_func
Func_020005cc:
	push {lr}
	movs r2, #192
	lsls r2, r2, #2
	movs r1, #66
	adds r2, #2
	bl Func_02001cf4
	pop {pc}
	.section .text.x020085dc,"ax",%progbits
	.global Func_020005dc
	.thumb_func
Func_020005dc:
	push {lr}
	movs r2, #129
	lsls r2, r2, #2
	movs r1, #67
	adds r2, #255
	bl Func_02001cf4
	pop {pc}
	.section .text.x020085ec,"ax",%progbits
	.global Func_020005ec
	.thumb_func
Func_020005ec:
	push {lr}
	movs r2, #193
	movs r1, #68
	lsls r2, r2, #2
	bl Func_02001cf4
	pop {pc}
	.2byte 0x0000
	.section .text.x020085fc,"ax",%progbits
	.global Func_020005fc
	.thumb_func
Func_020005fc:
	push {lr}
	movs r2, #192
	lsls r2, r2, #2
	movs r1, #70
	adds r2, #6
	bl Func_02001cf4
	pop {pc}
	.section .text.x0200860c,"ax",%progbits
	.global Func_0200060c
	.thumb_func
Func_0200060c:
	push {lr}
	movs r2, #130
	lsls r2, r2, #2
	movs r1, #71
	adds r2, #255
	bl Func_02001cf4
	pop {pc}
	.section .text.x0200861c,"ax",%progbits
	.global Func_0200061c
	.thumb_func
Func_0200061c:
	push {lr}
	movs r2, #194
	movs r1, #72
	lsls r2, r2, #2
	bl Func_02001cf4
	pop {pc}
	.2byte 0x0000
	.section .text.x0200862c,"ax",%progbits
	.global Func_0200062c
	.thumb_func
Func_0200062c:
	push {r5, lr}
	ldr r3, .L_020088d8
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	ldr r0, [r3]
	bl Object_GetById
	ldr r3, [r0, #12]
	movs r2, #192
	lsls r2, r2, #15
	cmp r3, r2
	bgt .L_02008654
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #105
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008666
.L_02008654:
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r1, #173
	lsls r1, r1, #1
	adds r2, r3, r1
	movs r3, #1
	strh r3, [r2]
	b .L_020088d4
.L_02008666:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #102
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008682
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #104
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008694
.L_02008682:
	ldr r0, .L_020088dc
	bl Func_02001c6c
	movs r0, #1
	negs r0, r0
	movs r1, #0
	bl Func_02001c84
	b .L_020088d4
.L_02008694:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #104
	bl GameFlag_SetBit
	bl Func_02001bcc
	movs r0, #0
	bl Func_02001d04
	ldr r0, .L_020088e0
	bl Func_02001c6c
	movs r1, #218
	movs r2, #220
	lsls r2, r2, #18
	lsls r1, r1, #18
	movs r0, #12
	bl Func_02001c2c
	movs r0, #12
	bl Object_GetById
	movs r1, #43
	bl Animation_SetIndexAndInitObjects
	movs r0, #12
	movs r1, #3
	bl Object_SetModeById
	movs r1, #160
	movs r0, #12
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #12
	movs r1, #0
	movs r2, #10
	bl Func_02001c7c
	movs r0, #218
	movs r1, #1
	movs r2, #194
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #18
	lsls r0, r0, #18
	bl Motion_CamBounds
	bl Func_02001cc4
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #10
	adds r1, #255
	movs r2, #30
	movs r0, #12
	bl Func_02001ca4
	movs r2, #10
	movs r0, #12
	movs r1, #0
	bl Func_02001c7c
	movs r0, #12
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #12
	movs r1, #0
	movs r2, #10
	bl Func_02001c7c
	movs r0, #218
	movs r1, #1
	movs r2, #214
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #18
	lsls r0, r0, #18
	bl Motion_CamBounds
	bl Func_02001cc4
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #12
	movs r1, #0
	movs r2, #10
	bl Func_02001c7c
	movs r0, #218
	movs r1, #1
	movs r2, #232
	lsls r2, r2, #18
	lsls r0, r0, #18
	negs r1, r1
	movs r3, #1
	bl Motion_CamBounds
	bl Func_02001cc4
	movs r0, #133
	bl Func_02001bbc
	movs r2, #1
	adds r5, r0, #0
	negs r2, r2
	cmp r5, r2
	beq .L_02008778
	b .L_020088cc
.L_02008778:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #98
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008788
	b .L_020088cc
.L_02008788:
	ldr r0, .L_020088e4
	bl Func_02001c6c
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #214
	movs r2, #238
	lsls r2, r2, #2
	movs r0, #4
	lsls r1, r1, #2
	bl ObjectMotion_SetPositionAndReset
	movs r1, #192
	lsls r1, r1, #8
	movs r0, #4
	bl Func_02001c94
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #6
	adds r1, #255
	movs r2, #60
	movs r0, #4
	bl Func_02001ca4
	movs r0, #218
	movs r2, #198
	movs r3, #1
	adds r1, r5, #0
	lsls r2, r2, #18
	lsls r0, r0, #18
	bl Motion_CamBounds
	bl Func_02001cc4
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #2
	adds r1, #255
	movs r2, #30
	movs r0, #12
	bl Func_02001ca4
	movs r0, #12
	movs r1, #0
	movs r2, #10
	bl Func_02001c7c
	movs r0, #218
	movs r2, #214
	movs r3, #1
	lsls r0, r0, #18
	adds r1, r5, #0
	lsls r2, r2, #18
	bl Motion_CamBounds
	movs r0, #12
	movs r1, #4
	movs r2, #0
	bl ObjectMotion_Launch
	movs r1, #218
	movs r2, #228
	lsls r1, r1, #2
	lsls r2, r2, #2
	movs r0, #12
	bl ObjectMotion_SetPositionAndReset
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #128
	movs r2, #128
	movs r0, #12
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r1, #32
	movs r2, #0
	movs r0, #12
	negs r1, r1
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #192
	movs r0, #12
	lsls r1, r1, #6
	bl Func_02001c94
	movs r0, #212
	movs r2, #226
	movs r3, #1
	adds r1, r5, #0
	lsls r2, r2, #18
	lsls r0, r0, #18
	bl Motion_CamBounds
	bl Func_02001cc4
	movs r0, #15
	bl Battle_WaitMode0
	movs r0, #12
	movs r1, #0
	movs r2, #5
	bl Func_02001c7c
	movs r1, #210
	movs r2, #230
	movs r0, #28
	lsls r1, r1, #18
	lsls r2, r2, #18
	bl Func_02001c2c
	movs r1, #210
	movs r2, #234
	movs r0, #27
	lsls r1, r1, #18
	lsls r2, r2, #18
	bl Func_02001c2c
	movs r1, #28
	movs r2, #27
	movs r0, #1
	bl Func_02001d2c
	movs r1, #28
	movs r2, #27
	movs r0, #1
	bl Func_02001d34
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #12
	movs r1, #0
	movs r2, #5
	bl Func_02001c7c
	movs r2, #0
	movs r0, #12
	movs r1, #32
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #4
	movs r1, #1
	bl Object_AttachWorkTargetToObject
	movs r2, #32
	movs r0, #12
	movs r1, #0
	negs r2, r2
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #135
	lsls r0, r0, #4
	adds r0, #255
	bl GameFlag_SetBit
.L_020088cc:
	bl Func_020001bc
	bl Func_02001bd4
.L_020088d4:
	pop {r5, pc}
	.2byte 0x0000
.L_020088d8:
	.4byte gPartyState
.L_020088dc:
	.4byte 0x00002796
.L_020088e0:
	.4byte 0x00002797
.L_020088e4:
	.4byte 0x0000289d
	.section .text.x020088e8,"ax",%progbits
	.global Func_020008e8
	.thumb_func
Func_020008e8:
	push {lr}
	ldr r3, .L_0200892c
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	ldr r0, [r3]
	bl Object_GetById
	ldr r3, [r0, #12]
	movs r2, #180
	lsls r2, r2, #15
	cmp r3, r2
	bge .L_02008918
	ldr r0, .L_02008930
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
	movs r0, #126
	bl Func_02001d7c
	movs r0, #1
	bl Func_02001bb4
	b .L_02008928
.L_02008918:
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r1, #173
	lsls r1, r1, #1
	adds r2, r3, r1
	movs r3, #1
	strh r3, [r2]
.L_02008928:
	pop {pc}
	.2byte 0x0000
.L_0200892c:
	.4byte gPartyState
.L_02008930:
	.4byte 0x000028a2
	.section .text.x02008934,"ax",%progbits
	.global Func_02000934
	.thumb_func
Func_02000934:
	push {r5, lr}
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #8
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008976
	ldr r3, .L_02008a54
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r1, [r3]
	movs r2, #0
	movs r0, #30
	bl ObjectMotion_SetAngleToward
	movs r0, #30
	bl Object_RefreshSelectorById
	ldr r0, .L_02008a58
	bl Func_02001c6c
	movs r0, #30
	movs r1, #0
	bl Func_02001c84
	movs r1, #208
	movs r0, #30
	lsls r1, r1, #8
	bl Func_02001c94
	b .L_02008a52
.L_02008976:
	bl Func_02001bcc
	movs r0, #0
	bl Func_02001d04
	ldr r0, .L_02008a5c
	bl Func_02001c6c
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #30
	bl Func_02001ca4
	ldr r3, .L_02008a54
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r3, r2
	ldr r1, [r5]
	movs r2, #0
	movs r0, #30
	bl ObjectMotion_SetAngleToward
	movs r0, #30
	bl Object_RefreshSelectorById
	movs r0, #5
	bl Battle_WaitMode0
	movs r1, #0
	movs r0, #30
	bl UiText_OpenMessageAtObject
	movs r0, #4
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	beq .L_020089d6
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #30
	movs r1, #0
	movs r2, #0
	bl Func_02001c7c
	b .L_02008a44
.L_020089d6:
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #30
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #30
	adds r3, #1
	strh r3, [r2]
	movs r1, #0
	movs r2, #5
	bl Func_02001c7c
	movs r1, #232
	movs r2, #130
	lsls r2, r2, #3
	ldr r0, [r5]
	lsls r1, r1, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #192
	lsls r1, r1, #8
	ldr r0, [r5]
	bl Func_02001c94
	movs r0, #5
	bl Battle_WaitMode0
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #30
	ldr r1, .L_02008a60
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r1, #220
	movs r2, #131
	movs r0, #30
	lsls r1, r1, #1
	lsls r2, r2, #3
	bl ObjectMotion_SetPositionAndReset
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #8
	bl GameFlag_SetBit
.L_02008a44:
	movs r1, #208
	movs r0, #30
	lsls r1, r1, #8
	bl Func_02001c94
	bl Func_02001bd4
.L_02008a52:
	pop {r5, pc}
.L_02008a54:
	.4byte gPartyState
.L_02008a58:
	.4byte 0x000028ac
.L_02008a5c:
	.4byte 0x000028a9
.L_02008a60:
	.4byte 0x00013333
	.section .text.x02008a64,"ax",%progbits
	.global Func_02000a64
	.thumb_func
Func_02000a64:
	ldr r0, .L_02008a68
	bx lr
.L_02008a68:
	.4byte Data_020021f4
	.section .text.x02008a6c,"ax",%progbits
	.global Func_02000a6c
	.thumb_func
Func_02000a6c:
	push {lr}
	ldr r3, .L_02008acc
	movs r2, #3
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	bne .L_02008aca
	movs r3, #160
	lsls r3, r3, #19
	adds r3, #252
	ldrh r3, [r3]
	movs r1, #14
	lsls r3, r3, #16
	asrs r0, r3, #16
.L_02008a88:
	movs r4, #160
	lsls r4, r4, #19
	lsls r3, r1, #1
	adds r4, #224
	adds r2, r3, r4
	subs r4, #2
	adds r3, r3, r4
	ldrh r3, [r3]
	subs r1, #1
	strh r3, [r2]
	cmp r1, #1
	bne .L_02008a88
	movs r3, #160
	lsls r3, r3, #19
	adds r3, #226
	strh r0, [r3]
	adds r3, #58
	ldrh r3, [r3]
	movs r1, #14
	lsls r3, r3, #16
	asrs r0, r3, #16
.L_02008ab2:
	ldr r4, .L_02008ad0
	lsls r3, r1, #1
	adds r2, r3, r4
	subs r4, #2
	adds r3, r3, r4
	ldrh r3, [r3]
	subs r1, #1
	strh r3, [r2]
	cmp r1, #1
	bne .L_02008ab2
	ldr r3, .L_02008ad4
	strh r0, [r3]
.L_02008aca:
	pop {pc}
.L_02008acc:
	.4byte Data_0300122c
.L_02008ad0:
	.4byte 0x05000100
.L_02008ad4:
	.4byte 0x05000102
	.section .text.x02008ad8,"ax",%progbits
	.global Func_02000ad8
	.thumb_func
Func_02000ad8:
	ldr r3, .L_02008b1c
	ldrb r3, [r3]
	lsls r0, r3, #2
	adds r0, r0, r3
	ldr r3, .L_02008b20
	lsls r0, r0, #6
	adds r0, r0, r3
	movs r2, #0
	ldrsh r3, [r0, r2]
	movs r1, #128
	lsls r1, r1, #19
	adds r1, #24
	strh r3, [r1]
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #176
	ldrh r4, [r3, #10]
	movs r2, #197
	lsls r2, r2, #8
	adds r2, #255
	ands r2, r4
	strh r2, [r3, #10]
	movs r2, #254
	ldrh r4, [r3, #10]
	lsls r2, r2, #7
	adds r2, #255
	ands r2, r4
	strh r2, [r3, #10]
	adds r0, #2
	ldrh r2, [r3, #10]
	ldr r2, .L_02008b24
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	bx lr
.L_02008b1c:
	.4byte gOverlayArea + 0x2590
.L_02008b20:
	.4byte gOverlayArea + 0x25a0
.L_02008b24:
	.4byte 0xa2600001
	.section .text.x02008b28,"ax",%progbits
	.global Func_02000b28
	.thumb_func
Func_02000b28:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	movs r1, #160
	lsls r1, r1, #1
	adds r1, r1, r3
	ldr r3, .L_02008bb0
	sub sp, #8
	ldrb r2, [r3]
	movs r3, #1
	eors r3, r2
	lsls r2, r3, #2
	adds r2, r2, r3
	ldr r3, .L_02008bb4
	lsls r2, r2, #6
	adds r2, r2, r3
	add r7, sp, #4
	movs r3, #0
	str r3, [r7]
	mov r10, r1
	mov r8, r2
.L_02008b5a:
	ldr r3, .L_02008bb8
	ldr r0, [r7]
	ldrb r3, [r3]
	mov r1, r10
	adds r0, r0, r3
	movs r2, #6
	ldrsh r3, [r1, r2]
	adds r0, r0, r3
	lsls r0, r0, #9
	bl Math_Sine
	str r0, [sp, #0]
	mov r3, r10
	movs r2, #2
	ldrsh r5, [r3, r2]
	ldr r6, [r7]
	asrs r0, r0, #15
	adds r5, r5, r0
	movs r1, #3
	adds r0, r6, #0
	bl Engine_MathRemainder
	adds r5, r5, r0
	mov r1, r8
	subs r5, #1
	movs r2, #2
	adds r6, #1
	strh r5, [r1]
	add r8, r2
	str r6, [r7]
	cmp r6, #160
	bne .L_02008b5a
	ldr r3, .L_02008bb0
	movs r1, #1
	ldrb r2, [r3]
	add sp, #8
	eors r2, r1
	strb r2, [r3]
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008bb0:
	.4byte gOverlayArea + 0x2590
.L_02008bb4:
	.4byte gOverlayArea + 0x25a0
.L_02008bb8:
	.4byte Data_0300122c
	.section .text.x02008bbc,"ax",%progbits
	.global Func_02000bbc
	.thumb_func
Func_02000bbc:
	push {lr}
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_02008bd4
	bl Scheduler_AddOrUpdateCallback
	movs r1, #200
	lsls r1, r1, #4
	ldr r0, .L_02008bd8
	bl Scheduler_AddOrUpdateCallback
	pop {pc}
.L_02008bd4:
	.4byte Func_02000b28
.L_02008bd8:
	.4byte Func_02000ad8
	.section .text.x02008bdc,"ax",%progbits
	.global Func_02000bdc
	.thumb_func
Func_02000bdc:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r1, #200
	adds r2, #85
	str r2, [r3]
	lsls r1, r1, #4
	ldr r0, .L_02008e64
	sub sp, #8
	bl Scheduler_AddOrUpdateCallback
	movs r0, #0
	bl Func_02001cfc
	movs r0, #11
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #9
	movs r1, #3
	bl ObjectMotion_SetActionVariant
	movs r1, #3
	movs r0, #29
	bl ObjectMotion_SetActionVariant
	movs r0, #21
	bl Object_GetById
	ldr r3, .L_02008e68
	movs r1, #3
	str r3, [r0, #108]
	movs r0, #22
	bl Object_SetModeById
	movs r1, #160
	movs r2, #238
	lsls r2, r2, #18
	movs r0, #18
	lsls r1, r1, #14
	bl Func_02001c2c
	ldr r1, .L_02008e6c
	movs r0, #18
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #18
	bl Object_GetById
	adds r6, r0, #0
	movs r0, #19
	bl Object_GetById
	ldr r5, .L_02008e70
	movs r1, #176
	movs r2, #226
	lsls r2, r2, #18
	str r0, [r6, #104]
	str r5, [r6, #108]
	movs r0, #19
	lsls r1, r1, #15
	bl Func_02001c2c
	ldr r1, .L_02008e74
	movs r0, #19
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #19
	bl Object_GetById
	adds r6, r0, #0
	movs r0, #18
	bl Object_GetById
	str r0, [r6, #104]
	movs r0, #144
	lsls r0, r0, #4
	str r5, [r6, #108]
	adds r0, #8
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008c9c
	movs r1, #220
	movs r2, #131
	movs r0, #30
	lsls r1, r1, #17
	lsls r2, r2, #19
	bl Func_02001c2c
.L_02008c9c:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #100
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008cd4
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #105
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008cd4
	movs r1, #228
	movs r2, #130
	movs r0, #24
	lsls r1, r1, #17
	lsls r2, r2, #18
	bl Func_02001c2c
	movs r1, #236
	movs r2, #130
	movs r0, #25
	lsls r1, r1, #17
	lsls r2, r2, #18
	bl Func_02001c2c
.L_02008cd4:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #105
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008cee
	movs r0, #26
	movs r1, #0
	movs r2, #0
	bl Func_02001c2c
	b .L_02008cf8
.L_02008cee:
	movs r0, #26
	bl Object_GetById
	ldr r3, .L_02008e68
	str r3, [r0, #108]
.L_02008cf8:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #102
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008d2a
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #105
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008d2a
	movs r1, #224
	movs r2, #254
	movs r0, #7
	lsls r1, r1, #14
	lsls r2, r2, #18
	bl Func_02001c2c
	movs r0, #7
	movs r1, #9
	bl Object_SetModeById
.L_02008d2a:
	bl Func_02001d24
	movs r1, #144
	lsls r1, r1, #4
	adds r1, #98
	movs r2, #8
	movs r3, #9
	movs r0, #0
	bl Func_02001d3c
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
	movs r0, #135
	lsls r0, r0, #4
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008da6
	movs r0, #28
	movs r1, #2
	bl ObjectMotion_SetActionVariant
	movs r0, #27
	movs r1, #2
	bl ObjectMotion_SetActionVariant
	movs r1, #210
	movs r2, #230
	movs r0, #28
	lsls r1, r1, #18
	lsls r2, r2, #18
	bl Func_02001c2c
	movs r1, #210
	movs r2, #234
	movs r0, #27
	lsls r1, r1, #18
	lsls r2, r2, #18
	bl Func_02001c2c
	movs r1, #135
	lsls r1, r1, #4
	movs r0, #1
	adds r1, #255
	movs r2, #28
	movs r3, #27
	bl Func_02001d3c
.L_02008da6:
	movs r0, #28
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #99
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008dcc
	movs r0, #10
	movs r1, #1
	bl Func_02001d4c
	bl Func_02000560
.L_02008dcc:
	movs r0, #129
	lsls r0, r0, #2
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008de2
	movs r0, #67
	movs r1, #0
	bl Object_SetWideSprite
.L_02008de2:
	movs r0, #193
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008df6
	movs r0, #68
	movs r1, #0
	bl Object_SetWideSprite
.L_02008df6:
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #6
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008e0c
	movs r0, #70
	movs r1, #0
	bl Object_SetWideSprite
.L_02008e0c:
	movs r0, #130
	lsls r0, r0, #2
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008e22
	movs r0, #71
	movs r1, #0
	bl Object_SetWideSprite
.L_02008e22:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #102
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008e44
	movs r3, #54
	movs r2, #45
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #3
	movs r1, #1
	movs r2, #1
	movs r3, #1
	bl Func_02001b8c
.L_02008e44:
	movs r0, #31
	bl Object_GetById
	adds r6, r0, #0
	adds r2, r6, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	movs r3, #128
	lsls r3, r3, #15
	movs r0, #0
	str r3, [r6, #20]
	str r3, [r6, #12]
	add sp, #8
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02008e64:
	.4byte Func_02000a6c
.L_02008e68:
	.4byte Func_02001b3c
.L_02008e6c:
	.4byte Data_02001d84
.L_02008e70:
	.4byte Func_020000e8
.L_02008e74:
	.4byte Data_02001e04
	.section .text.x02008e78,"ax",%progbits
	.global Func_02000e78
	.thumb_func
Func_02000e78:
	movs r0, #0
	bx lr
	.section .text.x02008e7c,"ax",%progbits
	.global Func_02000e7c
	.thumb_func
Func_02000e7c:
	push {r5, r6, r7, lr}
	adds r5, r0, #0
	bl Func_02001bcc
	movs r0, #0
	bl Func_02001d04
	ldr r0, .L_02009090
	bl Func_02001c6c
	movs r0, #12
	bl Object_GetById
	movs r1, #56
	bl Animation_SetIndexAndInitObjects
	cmp r5, #21
	bne .L_02008ed4
	movs r1, #188
	movs r2, #242
	movs r0, #4
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_SetPositionAndReset
	movs r0, #4
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r6, #128
	movs r2, #16
	movs r0, #12
	movs r1, #16
	negs r2, r2
	movs r3, #0
	lsls r6, r6, #8
	movs r7, #160
	bl Func_02001d0c
	movs r5, #0
	adds r6, #10
	lsls r7, r7, #7
	b .L_02008f0c
.L_02008ed4:
	movs r1, #138
	movs r2, #242
	movs r0, #4
	lsls r1, r1, #2
	lsls r2, r2, #2
	bl ObjectMotion_SetPositionAndReset
	movs r1, #128
	movs r0, #4
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #16
	movs r2, #16
	movs r3, #128
	movs r0, #12
	negs r1, r1
	negs r2, r2
	lsls r3, r3, #8
	bl Func_02001d0c
	movs r5, #128
	movs r6, #10
	movs r7, #192
	lsls r5, r5, #8
	negs r6, r6
	lsls r7, r7, #6
.L_02008f0c:
	movs r0, #12
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #4
	movs r1, #12
	bl Object_LinkObjectAndSetCallback
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #8
	movs r0, #12
	lsls r1, r1, #9
	bl ObjectMotion_SetSpeedParameters
	movs r0, #12
	movs r1, #1
	bl Object_AttachWorkTargetToObject
	movs r1, #232
	movs r2, #238
	lsls r2, r2, #2
	movs r0, #12
	lsls r1, r1, #1
	bl ObjectMotion_SetPositionAndReset
	adds r1, r5, #0
	movs r0, #12
	bl Func_02001c94
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #12
	bl Func_02001ca4
	movs r2, #5
	movs r0, #12
	movs r1, #0
	bl Func_02001c7c
	adds r1, r6, #0
	movs r0, #12
	bl Func_02001c94
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #12
	bl Func_02001ca4
	movs r0, #12
	movs r1, #0
	movs r2, #5
	bl Func_02001c7c
	movs r1, #232
	movs r2, #196
	lsls r1, r1, #1
	lsls r2, r2, #2
	movs r0, #12
	bl ObjectMotion_SetPositionAndReset
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #1
	movs r1, #1
	movs r2, #1
	movs r3, #0
	negs r0, r0
	negs r1, r1
	negs r2, r2
	bl Motion_CamBounds
	movs r0, #12
	movs r1, #6
	movs r2, #15
	bl ObjectMotion_Launch
	movs r2, #23
	movs r1, #6
	movs r0, #12
	bl ObjectMotion_Launch
	movs r0, #10
	bl Battle_WaitMode0
	adds r1, r7, #0
	movs r0, #12
	bl Func_02001c94
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #5
	movs r1, #0
	movs r0, #12
	bl Func_02001c7c
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #4
	movs r1, #1
	bl ObjectMotion_SetActionVariant
	movs r1, #232
	movs r2, #214
	movs r0, #4
	lsls r1, r1, #17
	lsls r2, r2, #18
	bl Func_02001c2c
	movs r1, #232
	movs r2, #199
	lsls r2, r2, #2
	lsls r1, r1, #1
	movs r0, #4
	bl ObjectMotion_SetPositionAndReset
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #12
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #153
	lsls r2, r2, #8
	adds r2, #153
	movs r0, #12
	ldr r1, .L_02009094
	bl ObjectMotion_SetSpeedParameters
	movs r0, #12
	movs r1, #2
	bl Object_SetModeById
	ldr r3, .L_02009098
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	cmp r0, #0
	beq .L_02009052
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #12
	bl ObjectMotion_ResetAndSetPosition
.L_02009052:
	movs r0, #12
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r1, #0
	movs r2, #0
	movs r0, #12
	bl Func_02001c2c
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #4
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	movs r1, #232
	movs r2, #194
	strb r3, [r0]
	lsls r1, r1, #1
	movs r0, #4
	lsls r2, r2, #2
	bl ObjectMotion_SetPositionAndReset
	bl Func_020001bc
	bl Func_02001bd4
	pop {r5, r6, r7, pc}
.L_02009090:
	.4byte 0x00002896
.L_02009094:
	.4byte 0x00013333
.L_02009098:
	.4byte gPartyState
	.section .text.x0200909c,"ax",%progbits
	.global Func_0200109c
	.thumb_func
Func_0200109c:
	push {lr}
	bl Func_02001bcc
	movs r0, #0
	bl Func_02001d04
	ldr r0, .L_02009154
	bl Func_02001c6c
	movs r0, #12
	bl Object_GetById
	movs r1, #56
	bl Animation_SetIndexAndInitObjects
	movs r1, #128
	movs r0, #4
	lsls r1, r1, #7
	bl Func_02001c94
	movs r0, #12
	movs r1, #1
	bl ObjectMotion_SetActionVariant
	movs r3, #208
	lsls r3, r3, #8
	movs r1, #0
	movs r2, #16
	movs r0, #12
	bl Func_02001d0c
	movs r0, #12
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #5
	bl Battle_WaitMode0
	movs r0, #12
	movs r1, #0
	movs r2, #5
	bl Func_02001c7c
	movs r2, #153
	lsls r2, r2, #8
	adds r2, #153
	movs r0, #12
	ldr r1, .L_02009158
	bl ObjectMotion_SetSpeedParameters
	movs r0, #12
	movs r1, #2
	bl Object_SetModeById
	ldr r3, .L_0200915c
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	cmp r0, #0
	beq .L_02009126
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #12
	bl ObjectMotion_ResetAndSetPosition
.L_02009126:
	movs r0, #12
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r1, #0
	movs r2, #0
	movs r0, #12
	bl Func_02001c2c
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #16
	movs r0, #4
	movs r1, #0
	negs r2, r2
	bl ObjectMotion_CommitPositionAndActivate
	bl Func_020001bc
	bl Func_02001bd4
	pop {pc}
	.2byte 0x0000
.L_02009154:
	.4byte 0x00002899
.L_02009158:
	.4byte 0x00013333
.L_0200915c:
	.4byte gPartyState
	.section .text.x02009160,"ax",%progbits
	.global Func_02001160
	.thumb_func
Func_02001160:
	push {r5, lr}
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #100
	bl GameFlag_SetBit
	bl Func_02001bcc
	movs r0, #0
	bl Func_02001d04
	ldr r0, .L_02009574
	bl Func_02001c6c
	movs r1, #128
	movs r2, #128
	movs r0, #24
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #8
	lsls r1, r1, #9
	movs r0, #25
	bl ObjectMotion_SetSpeedParameters
	movs r0, #12
	bl Object_GetById
	movs r1, #56
	bl Animation_SetIndexAndInitObjects
	movs r1, #236
	movs r2, #142
	movs r0, #4
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_SetPositionAndReset
	movs r1, #16
	movs r3, #208
	lsls r3, r3, #8
	movs r0, #12
	negs r1, r1
	movs r2, #0
	bl Func_02001d0c
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #4
	bl ObjectMotion_ArmCallback
	movs r0, #12
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #232
	movs r1, #1
	movs r2, #136
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #18
	lsls r0, r0, #17
	bl Motion_CamBounds
	bl Func_02001cc4
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #12
	movs r1, #6
	movs r2, #15
	bl ObjectMotion_Launch
	movs r0, #12
	movs r1, #6
	movs r2, #23
	bl ObjectMotion_Launch
	movs r0, #12
	movs r1, #0
	movs r2, #10
	bl Func_02001c7c
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #40
	movs r0, #24
	bl Func_02001ca4
	movs r0, #24
	movs r1, #16
	movs r2, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #192
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #24
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #24
	movs r1, #0
	movs r2, #10
	bl Func_02001c7c
	movs r0, #12
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #4
	bl ObjectMotion_ArmCallback
	movs r0, #45
	bl Battle_WaitMode0
	movs r1, #208
	movs r0, #12
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #4
	bl ObjectMotion_ArmCallback
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #16
	movs r0, #25
	negs r1, r1
	movs r2, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #25
	movs r1, #0
	movs r2, #10
	bl Func_02001c7c
	movs r1, #232
	movs r2, #138
	movs r0, #25
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_SetPositionAndReset
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #6
	movs r0, #25
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #4
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #4
	bl Func_02001cac
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #25
	movs r1, #2
	bl ObjectMotion_SetActionVariant
	movs r0, #25
	movs r1, #16
	movs r2, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #160
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #25
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #4
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #4
	bl Func_02001cac
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #8
	movs r0, #25
	negs r1, r1
	movs r2, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #6
	movs r0, #25
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #25
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #236
	movs r2, #130
	movs r0, #25
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_SetPositionAndReset
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #25
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #25
	movs r1, #0
	bl Func_02001c7c
	movs r0, #4
	movs r1, #3
	bl Object_SetModeById
	movs r1, #3
	movs r0, #12
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #0
	movs r2, #0
	movs r0, #24
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #2
	adds r1, #255
	movs r2, #30
	movs r0, #24
	bl Func_02001ca4
	movs r0, #24
	movs r1, #0
	movs r2, #10
	bl Func_02001c7c
	movs r1, #8
	movs r2, #16
	movs r3, #192
	lsls r3, r3, #8
	negs r2, r2
	negs r1, r1
	movs r0, #7
	bl Func_02001d0c
	movs r0, #7
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #7
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #7
	movs r1, #0
	bl Func_02001c7c
	movs r0, #24
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	movs r0, #24
	lsls r1, r1, #1
	bl Func_02001cac
	movs r0, #25
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #25
	bl Func_02001cac
	movs r0, #45
	bl Battle_WaitMode0
	movs r1, #192
	movs r0, #24
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #25
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #7
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #7
	movs r1, #0
	movs r2, #10
	bl Func_02001c7c
	movs r0, #24
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #25
	bl ObjectMotion_ArmCallback
	movs r0, #35
	bl Battle_WaitMode0
	movs r1, #160
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #25
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #25
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #25
	movs r1, #0
	movs r2, #10
	bl Func_02001c7c
	movs r1, #2
	adds r1, #255
	movs r2, #30
	movs r0, #7
	bl Func_02001ca4
	movs r0, #7
	movs r1, #0
	movs r2, #10
	bl Func_02001c7c
	movs r1, #192
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #24
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #24
	movs r1, #0
	movs r2, #10
	bl Func_02001c7c
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #40
	movs r0, #7
	bl Func_02001ca4
	movs r0, #7
	movs r1, #0
	movs r2, #10
	bl Func_02001c7c
	movs r1, #2
	adds r1, #255
	movs r2, #30
	movs r0, #12
	bl Func_02001ca4
	movs r2, #10
	movs r0, #12
	movs r1, #0
	bl Func_02001c7c
	movs r1, #2
	movs r0, #7
	bl Motion_SetVarCbAndRefresh
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #7
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #7
	movs r1, #0
	movs r2, #10
	bl Func_02001c7c
	movs r1, #128
	movs r2, #60
	lsls r1, r1, #1
	movs r0, #12
	bl Func_02001ca4
	movs r0, #4
	movs r1, #12
	bl Object_LinkObjectAndSetCallback
	movs r0, #7
	movs r1, #12
	bl Object_LinkObjectAndSetCallback
	movs r0, #24
	movs r1, #12
	bl Object_LinkObjectAndSetCallback
	movs r0, #25
	movs r1, #12
	bl Object_LinkObjectAndSetCallback
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #8
	movs r0, #12
	lsls r1, r1, #9
	bl ObjectMotion_SetSpeedParameters
	movs r0, #12
	movs r1, #1
	bl Object_AttachWorkTargetToObject
	movs r1, #212
	movs r2, #150
	lsls r1, r1, #1
	lsls r2, r2, #2
	movs r0, #12
	bl ObjectMotion_SetPositionAndReset
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #200
	movs r1, #1
	movs r2, #162
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #18
	lsls r0, r0, #17
	bl Motion_CamBounds
	bl Func_02001cc4
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #192
	lsls r1, r1, #6
	b .L_02009578
	.2byte 0x0000
.L_02009574:
	.4byte 0x00002708
.L_02009578:
	movs r2, #0
	movs r0, #12
	bl ObjectMotion_ArmCallback
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #160
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #12
	bl ObjectMotion_ArmCallback
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #192
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #12
	bl ObjectMotion_ArmCallback
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #160
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #12
	bl ObjectMotion_ArmCallback
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #12
	movs r1, #1
	bl Object_AttachWorkTargetToObject
	movs r1, #216
	movs r2, #134
	lsls r1, r1, #1
	lsls r2, r2, #2
	movs r0, #12
	bl ObjectMotion_SetPositionAndReset
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #176
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #12
	bl ObjectMotion_ArmCallback
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #208
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #12
	bl ObjectMotion_ArmCallback
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #176
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #12
	bl ObjectMotion_ArmCallback
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #208
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #12
	bl ObjectMotion_ArmCallback
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #220
	movs r1, #1
	movs r2, #144
	negs r1, r1
	lsls r2, r2, #17
	movs r3, #1
	lsls r0, r0, #17
	bl Motion_CamBounds
	bl Func_02001cc4
	movs r0, #60
	bl Battle_WaitMode0
	movs r0, #220
	movs r1, #1
	movs r2, #132
	movs r3, #1
	lsls r2, r2, #18
	negs r1, r1
	lsls r0, r0, #17
	bl Motion_CamBounds
	bl Func_02001cc4
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #12
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #12
	movs r1, #1
	bl Object_AttachWorkTargetToObject
	movs r1, #228
	movs r2, #142
	movs r0, #12
	lsls r1, r1, #1
	lsls r2, r2, #2
	bl ObjectMotion_SetPositionAndReset
	movs r0, #232
	movs r1, #1
	movs r2, #136
	movs r3, #1
	lsls r0, r0, #17
	negs r1, r1
	lsls r2, r2, #18
	bl Motion_CamBounds
	bl Func_02001cc4
	movs r1, #128
	movs r0, #4
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #7
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #24
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	movs r0, #25
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #208
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #12
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #12
	movs r1, #0
	movs r2, #10
	bl Func_02001c7c
	movs r1, #179
	movs r2, #178
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #7
	adds r1, #51
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r0, #7
	movs r1, #8
	movs r2, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #7
	bl ObjectMotion_ArmCallback
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #4
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #0
	movs r0, #7
	bl UiText_OpenMessageAtObject
	movs r0, #4
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_0200975e
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #7
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r2, #10
	movs r0, #7
	movs r1, #0
	bl Func_02001c7c
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_0200978e
.L_0200975e:
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #7
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #7
	adds r3, #1
	strh r3, [r2]
	movs r1, #0
	movs r2, #10
	bl Func_02001c7c
.L_0200978e:
	movs r0, #4
	movs r1, #7
	bl Object_LinkObjectAndSetCallback
	movs r0, #12
	movs r1, #7
	bl Object_LinkObjectAndSetCallback
	movs r0, #24
	movs r1, #7
	bl Object_LinkObjectAndSetCallback
	movs r0, #25
	movs r1, #7
	bl Object_LinkObjectAndSetCallback
	movs r0, #7
	movs r1, #2
	bl ObjectMotion_SetActionVariant
	movs r0, #7
	movs r1, #32
	movs r2, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #192
	movs r0, #24
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #25
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #7
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #7
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r2, #10
	movs r0, #7
	movs r1, #0
	bl Func_02001c7c
	movs r1, #2
	movs r0, #7
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #7
	movs r1, #0
	movs r2, #10
	bl Func_02001c7c
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #40
	movs r0, #7
	bl Func_02001ca4
	movs r0, #7
	movs r1, #0
	movs r2, #10
	bl Func_02001c7c
	movs r1, #192
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #7
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #7
	movs r1, #0
	movs r2, #10
	bl Func_02001c7c
	movs r1, #128
	movs r0, #4
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #0
	movs r2, #0
	movs r0, #12
	bl ObjectMotion_ArmCallback
	movs r0, #50
	bl Battle_WaitMode0
	movs r1, #224
	movs r0, #4
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r2, #0
	movs r1, #0
	movs r0, #12
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #12
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #12
	movs r1, #0
	movs r2, #10
	bl Func_02001c7c
	movs r1, #10
	adds r1, #255
	movs r2, #30
	movs r0, #7
	bl Func_02001ca4
	movs r0, #7
	movs r1, #0
	movs r2, #10
	bl Func_02001c7c
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #12
	bl Func_02001ca4
	movs r0, #12
	movs r1, #0
	movs r2, #10
	bl Func_02001c7c
	movs r0, #12
	movs r1, #6
	movs r2, #15
	bl ObjectMotion_Launch
	movs r2, #23
	movs r1, #6
	movs r0, #12
	bl ObjectMotion_Launch
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #7
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #7
	movs r1, #0
	bl Func_02001c7c
	movs r1, #2
	movs r0, #24
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #24
	movs r1, #0
	bl Func_02001c7c
	movs r1, #2
	movs r0, #25
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #25
	movs r1, #0
	movs r2, #10
	bl Func_02001c7c
	movs r1, #128
	movs r0, #4
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #0
	movs r2, #0
	movs r0, #12
	bl ObjectMotion_ArmCallback
	movs r0, #50
	bl Battle_WaitMode0
	movs r1, #160
	movs r0, #25
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #4
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #208
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #12
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #2
	adds r1, #255
	movs r2, #0
	movs r0, #12
	bl Func_02001ca4
	movs r1, #2
	adds r1, #255
	movs r2, #40
	movs r0, #4
	bl Func_02001ca4
	movs r2, #10
	movs r0, #12
	movs r1, #0
	bl Func_02001c7c
	movs r0, #7
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r2, #10
	movs r0, #7
	movs r1, #0
	bl Func_02001c7c
	movs r1, #3
	movs r0, #24
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #24
	movs r1, #0
	bl Func_02001c7c
	movs r1, #2
	movs r0, #25
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #25
	movs r1, #0
	movs r2, #10
	bl Func_02001c7c
	movs r1, #10
	adds r1, #255
	movs r2, #45
	movs r0, #7
	bl Func_02001ca4
	movs r1, #16
	movs r0, #7
	negs r1, r1
	movs r2, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #192
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #7
	bl ObjectMotion_ArmCallback
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #7
	movs r1, #0
	bl Func_02001c7c
	movs r1, #3
	movs r0, #24
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #24
	movs r1, #0
	bl Func_02001c7c
	movs r0, #25
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #25
	movs r1, #0
	movs r2, #10
	bl Func_02001c7c
	movs r1, #16
	movs r0, #7
	negs r1, r1
	movs r2, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #7
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #7
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #7
	movs r1, #0
	movs r2, #10
	bl Func_02001c7c
	movs r1, #128
	movs r0, #4
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r2, #0
	movs r1, #0
	movs r0, #12
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #12
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #192
	movs r0, #4
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r2, #153
	lsls r2, r2, #8
	adds r2, #153
	movs r0, #7
	ldr r1, .L_02009b34
	bl ObjectMotion_SetSpeedParameters
	movs r0, #7
	movs r1, #2
	bl Object_SetModeById
	ldr r3, .L_02009b38
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r3, r2
	ldr r0, [r5]
	bl Object_GetById
	cmp r0, #0
	beq .L_02009adc
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #7
	bl ObjectMotion_ResetAndSetPosition
.L_02009adc:
	movs r0, #7
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #7
	movs r1, #0
	movs r2, #0
	bl Func_02001c2c
	movs r2, #153
	lsls r2, r2, #8
	movs r0, #12
	ldr r1, .L_02009b34
	adds r2, #153
	bl ObjectMotion_SetSpeedParameters
	movs r0, #12
	movs r1, #2
	bl Object_SetModeById
	ldr r0, [r5]
	bl Object_GetById
	cmp r0, #0
	beq .L_02009b1a
	movs r2, #10
	ldrsh r1, [r0, r2]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #12
	bl ObjectMotion_ResetAndSetPosition
.L_02009b1a:
	movs r0, #12
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #12
	movs r1, #0
	movs r2, #0
	bl Func_02001c2c
	bl Func_020001bc
	bl Func_02001bd4
	pop {r5, pc}
.L_02009b34:
	.4byte 0x00013333
.L_02009b38:
	.4byte gPartyState
	.section .text.x02009b3c,"ax",%progbits
	.global Func_02001b3c
	.thumb_func
Func_02001b3c:
	push {lr}
	bl Func_02001d6c
	pop {pc}
	.section .rodata.x02009d84,"a",%progbits
	.global Data_02001d84
Data_02001d84:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00010000
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00010000
	.4byte 0x80010000
	.4byte 0x00000004
	.4byte 0x00280000
	.4byte 0x00000000
	.4byte 0x03b80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00280000
	.4byte 0x00000000
	.4byte 0x03880000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x03880000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x03b80000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0xc0010000
	.global Data_02001e04
Data_02001e04:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00010000
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00010000
	.4byte 0x80010000
	.4byte 0x00000004
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x03880000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x03b80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00280000
	.4byte 0x00000000
	.4byte 0x03b80000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00280000
	.4byte 0x00000000
	.4byte 0x03880000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0xc0010000
	.global Data_02001e84
Data_02001e84:
	.4byte 0xffff0000
	.4byte 0x000001cc
	.4byte 0x40000226
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02001eb4
Data_02001eb4:
	.4byte 0x000000ca
	.4byte 0x101010cb
	.4byte 0xffffffff
	.4byte 0x102020cb
	.4byte 0xffffffff
	.4byte 0x103030cb
	.4byte 0xffffffff
	.4byte 0x104030c9
	.4byte 0xffffffff
	.4byte 0x105010cd
	.4byte 0xffffffff
	.4byte 0x10c0c0cb
	.4byte 0xffffffff
	.4byte 0x10f0f0cb
	.4byte 0xffffffff
	.4byte 0x110100cb
	.4byte 0xffffffff
	.4byte 0x10b010cc
	.4byte 0xffffffff
	.4byte 0x10a0a0cc
	.4byte 0xffffffff
	.4byte 0x111110cc
	.4byte 0xffffffff
	.4byte 0x000001ff
	.global Data_02001f14
Data_02001f14:
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x03b80000
	.4byte 0x00000000
	.4byte 0x03980000
	.4byte 0x00024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x03880000
	.4byte 0x00000000
	.4byte 0x03980000
	.4byte 0x01024000
	.4byte 0xffff0174
	.4byte 0x00000001
	.4byte 0x03580000
	.4byte 0x00000000
	.4byte 0x03f80000
	.4byte 0x00024000
	.4byte 0xffff01c2
	.4byte 0x00000001
	.4byte 0x00800000
	.4byte 0x00000000
	.4byte 0x03700000
	.4byte 0x00024000
	.4byte 0xffff009d
	.4byte 0x00000001
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x04e80000
	.4byte 0x00018000
	.4byte 0xffff009c
	.4byte 0x00000001
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x04100000
	.4byte 0x00014000
	.4byte 0xffff00a0
	.4byte 0x00000001
	.4byte 0x02380000
	.4byte 0x00000000
	.4byte 0x04280000
	.4byte 0x00014000
	.4byte 0xffff00a5
	.4byte 0x00000001
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x03580000
	.4byte 0x00015000
	.4byte 0xffff00a4
	.4byte 0x00000001
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x03980000
	.4byte 0x00018000
	.4byte 0xffff00a6
	.4byte 0x00000001
	.4byte 0x02580000
	.4byte 0x00000000
	.4byte 0x03980000
	.4byte 0x00015000
	.4byte 0xffff00a7
	.4byte 0x00000001
	.4byte 0x00280000
	.4byte 0x00000000
	.4byte 0x03b80000
	.4byte 0x0002c000
	.4byte 0xffff00a8
	.4byte 0x00000001
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x03880000
	.4byte 0x00024000
	.4byte 0xffff00d8
	.4byte 0x00000002
	.4byte 0x00500000
	.4byte 0x00000000
	.4byte 0x04500000
	.4byte 0x00004000
	.4byte 0xffff00bd
	.4byte 0x00000002
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x02c80000
	.4byte 0x00004000
	.4byte 0xffff0050
	.4byte 0x00000001
	.4byte 0x03480000
	.4byte 0x00000000
	.4byte 0x04380000
	.4byte 0x00020000
	.4byte 0xffff00cb
	.4byte 0x00000002
	.4byte 0x03600000
	.4byte 0x00000000
	.4byte 0x03d80000
	.4byte 0x00004000
	.4byte 0xffff00e7
	.4byte 0x00000001
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x02080000
	.4byte 0x00013000
	.4byte 0xffff00e7
	.4byte 0x00000001
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x02080000
	.4byte 0x00015000
	.4byte 0xffff00a6
	.4byte 0x00000002
	.4byte 0x02280000
	.4byte 0x00000000
	.4byte 0x01b80000
	.4byte 0x00004000
	.4byte 0xffff0007
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x0002c000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff009d
	.4byte 0x00000001
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x03f00000
	.4byte 0x00018000
	.4byte 0xffff00a0
	.4byte 0x00000001
	.4byte 0x01d00000
	.4byte 0x00000000
	.4byte 0x03f80000
	.4byte 0x0002d000
	.4byte 0xffff01e9
	.4byte 0x00000007
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x03d80000
	.4byte 0x00028000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
.L_0200a184:
	.4byte 0x00010002
	.4byte 0x00010001
	.4byte 0x00020006
	.4byte 0x00010002
	.4byte 0x00060001
	.2byte 0xffff
.L_0200a19a:
	.2byte 0x0000
	.4byte 0x00020001
	.4byte 0x00060001
	.4byte 0x00020000
	.4byte 0x00010002
	.4byte 0xffff0006
.L_0200a1b0:
	.4byte 0x00010003
	.4byte 0x00010001
	.4byte 0xffff0000
	.global Data_020021bc
Data_020021bc:
	.4byte .L_0200a184
	.4byte 0x002f0016
	.global Data_020021c4
Data_020021c4:
	.4byte .L_0200a184
	.4byte 0x002f0023
	.global Data_020021cc
Data_020021cc:
	.4byte .L_0200a19a
	.4byte 0x00280009
	.global Data_020021d4
Data_020021d4:
	.4byte .L_0200a1b0
	.4byte 0x002d0036
	.global Data_020021dc
Data_020021dc:
	.4byte .L_0200a19a
	.4byte 0x0014001c
	.global Data_020021e4
Data_020021e4:
	.4byte .L_0200a19a
	.4byte 0x000d001c
	.global Data_020021ec
Data_020021ec:
	.4byte .L_0200a184
	.4byte 0x00320036
	.global Data_020021f4
Data_020021f4:
	.4byte 0x0000c602
	.4byte 0xffff0001
	.4byte Func_02000278
	.4byte 0x0000c602
	.4byte 0xffff0002
	.4byte Func_02000278
	.4byte 0x0000c602
	.4byte 0xffff0003
	.4byte Func_02000278
	.4byte 0x00000031
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x0000c401
	.4byte 0x19080005
	.4byte 0x00000005
	.4byte 0x0000c602
	.4byte 0x1966000c
	.4byte Func_02000278
	.4byte 0x00000001
	.4byte 0xffff000f
	.4byte 0x0000000f
	.4byte 0x0000c602
	.4byte 0x19690010
	.4byte Func_02000278
	.4byte 0x0000c602
	.4byte 0xffff000b
	.4byte Func_02000278
	.4byte 0x0000c602
	.4byte 0xffff000a
	.4byte Func_02000278
	.4byte 0x00000001
	.4byte 0xffff0011
	.4byte 0x00000011
	.4byte 0x00000002
	.4byte 0x09640014
	.4byte Func_02001160
	.4byte 0x00000002
	.4byte 0x09640015
	.4byte Func_02000e7c
	.4byte 0x00000002
	.4byte 0x09640016
	.4byte Func_02000e7c
	.4byte 0x00000002
	.4byte 0x09640017
	.4byte Func_0200109c
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x0000272f
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x00002730
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x00002731
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x00002732
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x00002733
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x00002734
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte Func_02000308
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte Func_02000398
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte 0x00002737
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte 0x00002738
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte 0x00002739
	.4byte 0x00000000
	.4byte 0xffff0017
	.4byte 0x0000273a
	.4byte 0x00000000
	.4byte 0xffff0007
	.4byte 0x0000273b
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x0000273c
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x0000273d
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x0000273e
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x0000273f
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x00002740
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x00002741
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte Func_02000308
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte Func_02000398
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte 0x00002744
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte 0x00002745
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte 0x00002746
	.4byte 0x00008d15
	.4byte 0xffff0017
	.4byte 0x00002747
	.4byte 0x00008d15
	.4byte 0xffff0007
	.4byte 0x00002748
	.4byte 0x00000000
	.4byte 0x196a0018
	.4byte 0x0000286a
	.4byte 0x00000000
	.4byte 0x196a0019
	.4byte 0x0000286b
	.4byte 0x00008d15
	.4byte 0x196a0018
	.4byte 0x0000286c
	.4byte 0x00008d15
	.4byte 0x196a0019
	.4byte 0x0000286d
	.4byte 0x00000000
	.4byte 0x19690018
	.4byte 0x000027f2
	.4byte 0x00000000
	.4byte 0x19690019
	.4byte 0x000027f3
	.4byte 0x00008d15
	.4byte 0x19690018
	.4byte 0x000027f4
	.4byte 0x00008d15
	.4byte 0x19690019
	.4byte 0x000027f5
	.4byte 0x00000000
	.4byte 0xffff0018
	.4byte 0x0000272b
	.4byte 0x00000000
	.4byte 0xffff0019
	.4byte 0x0000272c
	.4byte 0x00008d15
	.4byte 0xffff0018
	.4byte 0x0000272d
	.4byte 0x00008d15
	.4byte 0xffff0019
	.4byte 0x0000272e
	.4byte 0x00000000
	.4byte 0x196a001a
	.4byte 0x00002873
	.4byte 0x00008d15
	.4byte 0x196a001a
	.4byte 0x00002879
	.4byte 0x00000000
	.4byte 0xffff001a
	.4byte 0x000027fb
	.4byte 0x00008d15
	.4byte 0xffff001a
	.4byte 0x00002801
	.4byte 0x0000c400
	.4byte 0xffff000b
	.4byte 0x00002782
	.4byte 0x00000000
	.4byte 0xffff001d
	.4byte 0x000028a0
	.4byte 0x00008d15
	.4byte 0xffff001d
	.4byte 0x000028a1
	.4byte 0x00000000
	.4byte 0xffff001e
	.4byte Func_02000934
	.4byte 0x00008d15
	.4byte 0xffff001e
	.4byte 0x000028ad
	.4byte 0x00000000
	.4byte 0xffff001f
	.4byte Func_020008e8
	.4byte 0x00000003
	.4byte 0xffff0019
	.4byte Func_020008e8
	.4byte 0x00000003
	.4byte 0xffff0028
	.4byte Func_0200062c
	.4byte 0x00008515
	.4byte 0x09620008
	.4byte 0x00000000
	.4byte 0x00000c15
	.4byte 0x0963000a
	.4byte Func_02000560
	.4byte 0x50008905
	.4byte 0xffff001e
	.4byte Func_02000428
	.4byte 0x50008905
	.4byte 0xffff001f
	.4byte Func_02000448
	.4byte 0x50008905
	.4byte 0xffff0020
	.4byte Func_02000468
	.4byte 0x50008905
	.4byte 0xffff0021
	.4byte Func_02000488
	.4byte 0x50008905
	.4byte 0xffff0026
	.4byte Func_020004a8
	.4byte 0x50008805
	.4byte 0x03030068
	.4byte Func_020005dc
	.4byte 0x50008805
	.4byte 0x03040069
	.4byte Func_020005ec
	.4byte 0x50008805
	.4byte 0x0306006b
	.4byte Func_020005fc
	.4byte 0x50008805
	.4byte 0x0307006c
	.4byte Func_0200060c
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
