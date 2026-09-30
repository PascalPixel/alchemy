.syntax unified
	.thumb
	.section .text.x02008038,"ax",%progbits
	.global Func_02000038
	.thumb_func
Func_02000038:
	ldr r0, .L_0200803c
	bx lr
.L_0200803c:
	.4byte Data_02002074
	.section .text.x02008040,"ax",%progbits
	.global Func_02000040
	.thumb_func
Func_02000040:
	movs r0, #0
	bx lr
	.section .text.x02008044,"ax",%progbits
	.global Func_02000044
	.thumb_func
Func_02000044:
	ldr r0, .L_02008048
	bx lr
.L_02008048:
	.4byte Data_020020a4
	.section .text.x0200804c,"ax",%progbits
	.global Func_0200004c
	.thumb_func
Func_0200004c:
	ldr r0, .L_02008050
	bx lr
.L_02008050:
	.4byte Data_020020bc
	.section .text.x02008054,"ax",%progbits
	.global Func_02000054
	.thumb_func
Func_02000054:
	push {lr}
	sub sp, #12
	movs r3, #70
	movs r2, #19
	movs r1, #1
	str r3, [sp, #0]
	str r2, [sp, #4]
	str r1, [sp, #8]
	movs r0, #56
	movs r1, #0
	movs r2, #4
	movs r3, #2
	bl Func_02001b1c
	add sp, #12
	pop {pc}
	.section .text.x02008074,"ax",%progbits
	.global Func_02000074
	.thumb_func
Func_02000074:
	push {lr}
	sub sp, #12
	movs r3, #69
	movs r2, #22
	movs r1, #1
	str r3, [sp, #0]
	str r2, [sp, #4]
	str r1, [sp, #8]
	movs r0, #56
	movs r1, #0
	movs r2, #4
	movs r3, #2
	bl Func_02001b1c
	add sp, #12
	pop {pc}
	.section .text.x02008094,"ax",%progbits
	.global Func_02000094
	.thumb_func
Func_02000094:
	push {lr}
	sub sp, #12
	movs r3, #79
	movs r2, #25
	movs r1, #1
	str r3, [sp, #0]
	str r2, [sp, #4]
	str r1, [sp, #8]
	movs r0, #56
	movs r1, #0
	movs r2, #3
	movs r3, #3
	bl Func_02001b1c
	add sp, #12
	pop {pc}
	.section .text.x020080b4,"ax",%progbits
	.global Func_020000b4
	.thumb_func
Func_020000b4:
	push {lr}
	sub sp, #12
	movs r3, #85
	movs r2, #26
	movs r1, #1
	str r3, [sp, #0]
	str r2, [sp, #4]
	str r1, [sp, #8]
	movs r0, #56
	movs r1, #0
	movs r2, #5
	movs r3, #3
	bl Func_02001b1c
	add sp, #12
	pop {pc}
	.section .text.x020080d4,"ax",%progbits
	.global Func_020000d4
	.thumb_func
Func_020000d4:
	push {lr}
	sub sp, #12
	movs r3, #94
	movs r2, #26
	movs r1, #1
	str r3, [sp, #0]
	str r2, [sp, #4]
	str r1, [sp, #8]
	movs r0, #56
	movs r1, #0
	movs r2, #5
	movs r3, #3
	bl Func_02001b1c
	add sp, #12
	pop {pc}
	.section .text.x020080f4,"ax",%progbits
	.global Func_020000f4
	.thumb_func
Func_020000f4:
	push {r5, r6, lr}
	ldr r3, .L_02008180
	movs r2, #133
	lsls r2, r2, #2
	adds r6, r3, r2
	ldr r0, [r6]
	sub sp, #12
	bl Object_GetById
	movs r3, #79
	movs r2, #25
	movs r1, #1
	str r3, [sp, #0]
	str r2, [sp, #4]
	str r1, [sp, #8]
	adds r5, r0, #0
	movs r1, #0
	movs r0, #56
	movs r2, #3
	movs r3, #3
	bl Func_02001b1c
	movs r0, #240
	lsls r0, r0, #4
	adds r0, #226
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200817c
	movs r1, #140
	movs r2, #220
	movs r0, #64
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_02001ab4
	ldr r3, [r5, #8]
	asrs r3, r3, #20
	cmp r3, #17
	bne .L_0200817c
	ldr r3, [r5, #16]
	asrs r3, r3, #20
	cmp r3, #27
	bne .L_0200817c
	bl Func_02001a7c
	movs r0, #0
	bl Func_02001b0c
	movs r1, #129
	ldr r0, [r6]
	lsls r1, r1, #1
	bl Func_02001acc
	movs r1, #16
	ldr r0, [r6]
	negs r1, r1
	movs r2, #0
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r3, #192
	lsls r3, r3, #11
	str r3, [r5, #40]
	ldr r0, [r6]
	bl ObjectMotion_CommitCurrentPositionAndActivate
	bl Func_02001a84
.L_0200817c:
	add sp, #12
	pop {r5, r6, pc}
.L_02008180:
	.4byte gPartyState
	.section .text.x02008184,"ax",%progbits
	.global Func_02000184
	.thumb_func
Func_02000184:
	push {lr}
	movs r0, #4
	bl Func_02001aec
	pop {pc}
	.2byte 0x0000
	.section .text.x02008190,"ax",%progbits
	.global Func_02000190
	.thumb_func
Func_02000190:
	ldr r0, .L_02008194
	bx lr
.L_02008194:
	.4byte Data_02002104
	.section .text.x02008198,"ax",%progbits
	.global Func_02000198
	.thumb_func
Func_02000198:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r0, #214
	lsls r0, r0, #1
	movs r2, #128
	adds r3, r3, r0
	lsls r2, r2, #1
	str r2, [r3]
	subs r0, #163
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020081d0
	ldr r2, .L_020081ec
	movs r1, #241
	lsls r1, r1, #1
	adds r3, r2, r1
	movs r0, #0
	ldrsh r3, [r3, r0]
	cmp r3, #4
	bne .L_020081d0
	adds r1, #50
	adds r3, r2, r1
	ldr r0, [r3]
	bl Func_02001b3c
.L_020081d0:
	ldr r1, .L_020081ec
	ldr r3, .L_020081f0
	movs r0, #242
	lsls r0, r0, #1
	adds r2, r1, r0
	strh r3, [r2]
	movs r3, #243
	lsls r3, r3, #1
	adds r2, r1, r3
	movs r3, #6
	strh r3, [r2]
	movs r0, #0
	pop {pc}
	.2byte 0x0000
.L_020081ec:
	.4byte gPartyState
.L_020081f0:
	.4byte 0x000000ed
	.section .text.x020081f4,"ax",%progbits
	.global Func_020001f4
	.thumb_func
Func_020001f4:
	push {lr}
	movs r0, #0
	bl Func_02000200
	movs r0, #0
	pop {pc}
	.section .text.x02008200,"ax",%progbits
	.global Func_02000200
	.thumb_func
Func_02000200:
	push {lr}
	movs r0, #20
	adds r0, #255
	bl GameFlag_SetBit
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #37
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008228
	movs r0, #98
	adds r0, #255
	bl GameFlag_SetBit
	movs r0, #162
	lsls r0, r0, #1
	bl GameFlag_SetBit
.L_02008228:
	pop {pc}
	.2byte 0x0000
	.section .text.x0200822c,"ax",%progbits
	.global Func_0200022c
	.thumb_func
Func_0200022c:
	ldr r3, .L_02008234
	str r0, [r3]
	bx lr
	.2byte 0x0000
.L_02008234:
	.4byte Data_02002198
	.section .text.x02008238,"ax",%progbits
	.global Func_02000238
	.thumb_func
Func_02000238:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r3, .L_020082e4
	sub sp, #32
	ldr r0, [r3]
	cmp r0, #0
	bge .L_0200824a
	adds r0, #3
.L_0200824a:
	asrs r0, r0, #2
	movs r1, #5
	bl Engine_MathRemainder
	ldr r3, .L_020082e8
	mov r8, r0
	ldr r3, [r3]
	cmp r3, #0
	beq .L_0200829e
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r0, #179
	lsls r0, r0, #1
	adds r3, r2, r0
	ldrh r1, [r3]
	movs r0, #0
	ldrsh r3, [r3, r0]
	cmp r3, #0
	beq .L_0200827e
	movs r3, #192
	lsls r3, r3, #2
	adds r3, #255
	ands r3, r1
	cmp r3, #153
	bne .L_020082da
.L_0200827e:
	movs r1, #192
	lsls r1, r1, #4
	adds r1, #164
	adds r3, r2, r1
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	bne .L_020082da
	movs r0, #175
	lsls r0, r0, #1
	adds r3, r2, r0
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	bne .L_020082da
.L_0200829e:
	movs r5, #0
	movs r6, #4
.L_020082a2:
	mov r2, r8
	adds r0, r2, r5
	movs r1, #5
	mov r7, sp
	bl Engine_MathRemainder
	ldr r3, .L_020082ec
	lsls r0, r0, #1
	ldrh r3, [r3, r6]
	adds r5, #1
	strh r3, [r7, r0]
	adds r6, #2
	cmp r5, #4
	ble .L_020082a2
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	adds r0, r7, #0
	ldr r1, .L_020082f0
	adds r2, #2
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r2, .L_020082e4
	ldr r3, [r2]
	adds r3, #1
	str r3, [r2]
.L_020082da:
	add sp, #32
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_020082e4:
	.4byte Data_02002194
.L_020082e8:
	.4byte Data_02002198
.L_020082ec:
	.4byte gOverlayArea + 0x21c0
.L_020082f0:
	.4byte 0x05000184
	.section .text.x020082f4,"ax",%progbits
	.global Func_020002f4
	.thumb_func
Func_020002f4:
	push {r5, r6, lr}
	ldr r2, .L_02008350
	movs r3, #1
	adds r6, r0, #0
	str r3, [r2]
	cmp r6, #2
	beq .L_02008318
	ldr r1, .L_02008354
	movs r2, #32
	ldr r0, .L_02008358
	ldr r5, .L_0200835c
	mov lr, r5
	.2byte 0xf800
	ldr r0, .L_02008360
	ldr r1, .L_02008364
	movs r2, #32
	mov lr, r5
	.2byte 0xf800
.L_02008318:
	movs r0, #160
	lsls r0, r0, #4
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008328
	cmp r6, #1
	bne .L_0200833a
.L_02008328:
	ldr r3, .L_02008368
	movs r2, #0
	movs r1, #144
	str r2, [r3]
	ldr r0, .L_0200836c
	lsls r1, r1, #3
	bl Scheduler_AddOrUpdateCallback
	b .L_0200834e
.L_0200833a:
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	ldr r0, .L_02008370
	ldr r1, .L_02008374
	adds r2, #2
	stmia r3!, {r0, r1, r2}
	subs r3, #12
.L_0200834e:
	pop {r5, r6, pc}
.L_02008350:
	.4byte Data_02002198
.L_02008354:
	.4byte 0x05000180
.L_02008358:
	.4byte gOverlayArea + 0x21c0
.L_0200835c:
	.4byte IwramCopyWords
.L_02008360:
	.4byte gOverlayArea + 0x21e0
.L_02008364:
	.4byte 0x050001a0
.L_02008368:
	.4byte Data_02002194
.L_0200836c:
	.4byte Func_02000238
.L_02008370:
	.4byte gOverlayArea + 0x21e4
.L_02008374:
	.4byte 0x05000184
	.section .text.x02008378,"ax",%progbits
	.global Func_02000378
	.thumb_func
Func_02000378:
	push {r5, lr}
	bl Object_GetById
	adds r5, r0, #0
	adds r1, r5, #0
	adds r1, #85
	movs r3, #4
	strb r3, [r1]
	movs r2, #0
	ldr r3, [r5, #20]
	str r2, [r5, #68]
	movs r2, #128
	lsls r2, r2, #14
	adds r3, r3, r2
	str r3, [r5, #12]
	subs r1, #50
	ldrb r2, [r1]
	movs r3, #128
	orrs r3, r2
	strb r3, [r1]
	adds r3, r5, #0
	adds r3, #34
	ldr r1, [r5, #8]
	ldr r2, [r5, #16]
	ldrb r0, [r3]
	bl Map_GetTerrainHeight
	adds r3, r0, #0
	asrs r3, r3, #19
	ldr r0, [r5, #8]
	ldr r1, [r5, #16]
	adds r3, #6
	movs r2, #0
	bl Func_02001a6c
	ldr r0, [r5, #8]
	ldr r1, [r5, #16]
	movs r2, #0
	movs r3, #128
	bl Func_02001b34
	pop {r5, pc}
	.section .text.x020083cc,"ax",%progbits
	.global Func_020003cc
	.thumb_func
Func_020003cc:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #133
	mov r10, r3
	ldr r3, .L_020084d8
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	adds r6, r0, #0
	ldr r7, [r6, #104]
	bl Func_02001a7c
	movs r0, #0
	bl Func_02001b0c
	movs r3, #208
	lsls r3, r3, #4
	adds r3, #70
	add r3, r10
	movs r5, #0
	strh r5, [r3]
	movs r3, #85
	adds r3, r3, r6
	mov r9, r3
	mov r2, r9
	movs r3, #4
	strb r3, [r2]
	adds r0, r6, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r3, #99
	adds r3, r3, r7
	mov r8, r3
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_0200846c
.L_02008426:
	ldr r3, [r7, #8]
	ldr r2, .L_020084dc
	str r3, [r6, #8]
	ldr r3, [r7, #12]
	adds r3, r3, r5
	str r3, [r6, #12]
	ldr r3, [r7, #16]
	str r3, [r6, #16]
	cmp r5, r2
	bgt .L_02008442
	movs r3, #200
	lsls r3, r3, #5
	adds r3, #153
	adds r5, r5, r3
.L_02008442:
	ldr r3, .L_020084e0
	adds r1, r6, #0
	ldr r2, [r3]
	ldrb r3, [r3]
	adds r1, #35
	lsls r3, r3, #12
	strh r3, [r6, #6]
	movs r3, #1
	ands r2, r3
	movs r3, #2
	lsls r3, r2
	ldrb r2, [r1]
	movs r0, #1
	eors r3, r2
	strb r3, [r1]
	bl WaitFrames
	mov r2, r8
	ldrb r3, [r2]
	cmp r3, #0
	bne .L_02008426
.L_0200846c:
	movs r3, #208
	lsls r3, r3, #4
	adds r3, #68
	movs r2, #1
	add r3, r10
	strh r2, [r3]
	movs r3, #208
	lsls r3, r3, #4
	adds r3, #70
	add r3, r10
	strh r2, [r3]
	ldr r3, [r7, #8]
	ldrh r1, [r7, #6]
	subs r2, #3
	asrs r3, r3, #19
	ands r3, r2
	asrs r1, r1, #13
	adds r3, r3, r1
	subs r3, #1
	lsls r3, r3, #19
	str r3, [r6, #8]
	ldr r3, [r7, #16]
	ldr r0, .L_020084d4
	asrs r3, r3, #19
	ands r3, r2
	movs r2, #2
	ands r1, r2
	subs r3, r3, r1
	adds r3, #1
	lsls r3, r3, #19
	str r3, [r6, #16]
	movs r3, #192
	lsls r3, r3, #11
	str r3, [r6, #40]
	adds r3, r6, #0
	adds r3, #35
	strb r0, [r3]
	mov r2, r9
	movs r3, #3
	strb r3, [r2]
	adds r0, r6, #0
	movs r1, #1
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r1, r1
	negs r2, r2
	movs r3, #0
	negs r0, r0
	b .L_020084e4
.L_020084d4:
	.4byte 0x00000001
.L_020084d8:
	.4byte gPartyState
.L_020084dc:
	.4byte 0x0003ffff
.L_020084e0:
	.4byte Data_0300122c
.L_020084e4:
	bl Motion_CamBounds
	bl Func_02001ae4
	movs r3, #128
	adds r7, r0, #0
	lsls r3, r3, #12
	str r3, [r7, #48]
	movs r3, #128
	ldr r5, .L_0200852c
	lsls r3, r3, #9
	str r3, [r7, #52]
	adds r3, r7, #0
	adds r3, #85
	strb r5, [r3]
	movs r0, #0
	ldr r1, [r6, #8]
	ldr r2, [r6, #16]
	bl Map_GetTerrainHeight
	ldr r3, [r6, #16]
	adds r2, r0, #0
	ldr r1, [r6, #8]
	adds r0, r7, #0
	bl Func_02001a2c
	ldr r3, [r6, #20]
	movs r2, #128
	lsls r2, r2, #13
	adds r3, r3, r2
	ldr r2, [r6, #12]
	movs r5, #0
	cmp r2, r3
	ble .L_0200854a
	b .L_02008530
	.2byte 0x0000
.L_0200852c:
	.4byte 0x00000000
.L_02008530:
	movs r0, #1
	adds r5, #1
	bl WaitFrames
	cmp r5, #59
	bgt .L_0200854a
	ldr r3, [r6, #20]
	movs r2, #128
	lsls r2, r2, #13
	adds r3, r3, r2
	ldr r2, [r6, #12]
	cmp r2, r3
	bgt .L_02008530
.L_0200854a:
	movs r0, #127
	bl Func_02001b44
	ldr r3, [r6, #40]
	movs r5, #0
	cmp r3, #0
	beq .L_0200856a
.L_02008558:
	movs r0, #1
	adds r5, #1
	bl WaitFrames
	cmp r5, #59
	bgt .L_0200856a
	ldr r3, [r6, #40]
	cmp r3, #0
	bne .L_02008558
.L_0200856a:
	adds r0, r7, #0
	bl Func_02001a34
	ldr r5, .L_020085b4
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	ldr r0, [r5]
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	strb r3, [r0]
	movs r1, #1
	ldr r0, [r5]
	bl Object_AttachWorkTargetToObject
	movs r2, #208
	lsls r2, r2, #4
	adds r2, #70
	add r2, r10
	movs r3, #1
	strh r3, [r2]
	movs r3, #170
	lsls r3, r3, #1
	movs r6, #0
	add r3, r10
	strh r6, [r3]
	bl Func_02001a84
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_020085b4:
	.4byte gPartyState
	.section .text.x020085b8,"ax",%progbits
	.global Func_020005b8
	.thumb_func
Func_020005b8:
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
	ldr r3, .L_020085e0
	mov lr, r3
	.2byte 0xf800
	pop {pc}
	.2byte 0x0000
.L_020085e0:
	.4byte IwramFillWords + 0x74
	.section .text.x020085e4,"ax",%progbits
	.global Func_020005e4
	.thumb_func
Func_020005e4:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #133
	mov r10, r3
	ldr r3, .L_0200864c
	lsls r2, r2, #2
	adds r3, r3, r2
	adds r6, r0, #0
	ldr r0, [r3]
	sub sp, #12
	bl Object_GetById
	movs r3, #179
	lsls r3, r3, #1
	add r3, r10
	movs r2, #0
	ldrsh r3, [r3, r2]
	adds r7, r0, #0
	cmp r3, #0
	bne .L_02008642
	movs r3, #173
	lsls r3, r3, #1
	add r3, r10
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	bne .L_02008642
	movs r3, #175
	lsls r3, r3, #1
	add r3, r10
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	bne .L_02008642
	movs r3, #180
	lsls r3, r3, #1
	add r3, r10
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	beq .L_02008650
.L_02008642:
	adds r0, r6, #0
	movs r1, #0
	bl ObjectDispatch_ApplyValueToChildren
	b .L_0200878e
.L_0200864c:
	.4byte gPartyState
.L_02008650:
	adds r0, r6, #0
	movs r1, #16
	bl ObjectDispatch_ApplyValueToChildren
	adds r3, r6, #0
	adds r3, #100
	ldrh r2, [r3]
	adds r2, #1
	strh r2, [r3]
	movs r3, #31
	ands r3, r2
	cmp r3, #31
	bne .L_02008670
	movs r0, #231
	bl Func_02001b44
.L_02008670:
	ldr r3, [r7, #80]
	ldr r0, [r6, #80]
	ldrb r3, [r3, #9]
	ldrb r1, [r0, #9]
	movs r2, #12
	ands r2, r3
	movs r3, #13
	negs r3, r3
	ands r3, r1
	orrs r3, r2
	strb r3, [r0, #9]
	movs r2, #2
	ldr r0, [r6, #8]
	ldr r1, [r6, #16]
	bl Func_02001b2c
	cmp r0, #255
	beq .L_02008772
	ldr r3, [r6, #8]
	mov r5, sp
	str r3, [r5]
	adds r0, r5, #0
	ldr r3, [r6, #12]
	str r3, [r5, #4]
	ldr r3, [r6, #16]
	str r3, [r5, #8]
	bl Func_02001b14
	ldr r5, [r5]
	movs r3, #136
	lsls r3, r3, #17
	cmp r5, r3
	bgt .L_02008772
	ldr r2, .L_02008760
	cmp r5, r2
	blt .L_02008772
	movs r3, #98
	adds r3, r3, r6
	mov r9, r3
	ldrb r3, [r3]
	cmp r3, #0
	bne .L_02008738
	ldr r2, [r7, #12]
	ldr r3, [r6, #12]
	subs r5, r2, r3
	cmp r5, #0
	bge .L_020086d0
	subs r5, r3, r2
.L_020086d0:
	adds r0, r7, #0
	adds r1, r6, #0
	movs r2, #0
	adds r0, #8
	adds r1, #8
	mov r8, r2
	bl Func_020005b8
	cmp r0, #12
	bgt .L_020086f0
	movs r3, #192
	lsls r3, r3, #12
	cmp r5, r3
	bge .L_020086f0
	movs r2, #1
	mov r8, r2
.L_020086f0:
	mov r3, r8
	cmp r3, #0
	beq .L_02008738
	movs r0, #130
	lsls r0, r0, #1
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008738
	ldrh r3, [r6, #6]
	str r6, [r7, #104]
	strh r3, [r7, #6]
	adds r1, r7, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #254
	ands r3, r2
	movs r2, #181
	lsls r2, r2, #1
	strb r3, [r1]
	add r2, r10
	movs r3, #200
	strh r3, [r2]
	ldr r3, .L_02008764
	movs r2, #128
	ldr r0, .L_0200875c
	lsls r2, r2, #2
	adds r2, #18
	adds r3, r3, r2
	strb r0, [r3]
	mov r2, r9
	movs r3, #1
	strb r3, [r2]
	adds r2, r6, #0
	adds r2, #99
	strb r3, [r2]
.L_02008738:
	ldrh r0, [r6, #6]
	bl Math_Cosine
	ldr r1, [r6, #48]
	ldr r5, .L_02008768
	mov lr, r5
	.2byte 0xf800
	ldr r3, [r6, #8]
	adds r3, r3, r0
	ldrh r0, [r6, #6]
	str r3, [r6, #8]
	bl Math_Sine
	ldr r1, [r6, #48]
	mov lr, r5
	.2byte 0xf800
	ldr r3, [r6, #16]
	b .L_0200876c
.L_0200875c:
	.4byte 0x00000000
.L_02008760:
	.4byte 0xffe00000
.L_02008764:
	.4byte gPartyState
.L_02008768:
	.4byte IwramMulQ16
.L_0200876c:
	adds r3, r3, r0
	str r3, [r6, #16]
	b .L_0200878e
.L_02008772:
	adds r3, r6, #0
	adds r3, #99
	movs r5, #0
	strb r5, [r3]
	ldr r1, .L_0200879c
	adds r0, r6, #0
	str r5, [r6, #108]
	bl Func_02001a0c
	movs r0, #228
	bl Func_02001b44
	ldr r3, .L_020087a0
	str r5, [r3]
.L_0200878e:
	add sp, #12
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200879c:
	.4byte Data_0200219c
.L_020087a0:
	.4byte gOverlayArea + 0x21bc
	.section .text.x020087a4,"ax",%progbits
	.global Func_020007a4
	.thumb_func
Func_020007a4:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r5, r0, #0
	movs r0, #222
	sub sp, #68
	bl Func_02001b44
	ldrh r0, [r5, #6]
	bl Math_Cosine
	adds r1, r0, #0
	movs r0, #128
	ldr r6, .L_02008858
	lsls r0, r0, #12
	mov lr, r6
	.2byte 0xf800
	ldr r3, [r5, #8]
	add r2, sp, #56
	adds r3, r3, r0
	str r3, [r2]
	mov r8, r2
	ldrh r0, [r5, #6]
	bl Math_Sine
	adds r1, r0, #0
	movs r0, #128
	lsls r0, r0, #12
	mov lr, r6
	.2byte 0xf800
	ldr r3, [r5, #16]
	mov r2, r8
	adds r3, r3, r0
	str r3, [r2, #8]
	movs r0, #140
	ldr r1, [r2]
	lsls r0, r0, #1
	ldr r2, [r5, #12]
	bl Func_02001a14
	movs r1, #2
	adds r7, r0, #0
	bl Func_020019fc
	adds r0, r7, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	adds r3, r7, #0
	movs r6, #0
	adds r3, #85
	strb r6, [r3]
	adds r1, r7, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #254
	ands r3, r2
	strb r3, [r1]
	ldr r2, .L_02008854
	ldrh r3, [r5, #6]
	add r4, sp, #16
	strh r3, [r7, #6]
	adds r3, r7, #0
	adds r3, #100
	strh r6, [r3]
	subs r3, #2
	strb r2, [r3]
	adds r3, #1
	strb r2, [r3]
	ldr r3, .L_0200885c
	str r3, [r7, #108]
	movs r3, #192
	lsls r3, r3, #9
	str r3, [r7, #48]
	movs r3, #1
	str r3, [r4]
	movs r3, #7
	str r3, [r4, #4]
	mov r3, r8
	ldr r0, [r3]
	ldr r2, [r3, #8]
	ldr r3, .L_02008860
	ldr r1, [r5, #12]
	adds r2, r2, r3
	movs r3, #192
	lsls r3, r3, #10
	str r3, [sp, #8]
	b .L_02008864
.L_02008854:
	.4byte 0x00000000
.L_02008858:
	.4byte IwramMulQ16
.L_0200885c:
	.4byte Func_020005e4
.L_02008860:
	.4byte 0xfffa0000
.L_02008864:
	movs r3, #0
	str r6, [sp, #0]
	str r6, [sp, #4]
	str r4, [sp, #12]
	bl Func_02000eac
	adds r0, r7, #0
	add sp, #68
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x0200887c,"ax",%progbits
	.global Func_0200087c
	.thumb_func
Func_0200087c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, .L_02008918
	sub sp, #56
	ldr r7, [r3]
	movs r3, #7
	ands r7, r3
	mov r10, r0
	cmp r7, #0
	bne .L_0200890a
	add r6, sp, #16
	movs r3, #3
	str r3, [r6]
	movs r3, #179
	lsls r3, r3, #8
	adds r3, #51
	str r3, [r6, #8]
	str r3, [r6, #12]
	movs r3, #14
	str r3, [r6, #4]
	bl Random16Far
	mov r2, r10
	lsls r3, r0, #3
	ldr r2, [r2, #8]
	adds r3, r3, r0
	lsrs r3, r3, #16
	subs r3, #4
	lsls r3, r3, #16
	mov r8, r2
	add r8, r3
	bl Random16Far
	lsls r3, r0, #2
	adds r3, r3, r0
	lsls r3, r3, #2
	lsrs r3, r3, #16
	movs r2, #32
	subs r2, r2, r3
	mov r3, r10
	ldr r5, [r3, #12]
	lsls r2, r2, #16
	adds r5, r5, r2
	bl Random16Far
	adds r3, r0, #0
	lsls r0, r3, #2
	adds r0, r0, r3
	lsrs r0, r0, #16
	movs r2, #160
	lsls r2, r2, #11
	lsls r0, r0, #16
	adds r0, r0, r2
	movs r1, #10
	bl Engine_MathDivide
	mov r3, r10
	ldr r2, [r3, #16]
	movs r3, #176
	lsls r3, r3, #12
	str r0, [sp, #0]
	str r3, [sp, #8]
	mov r0, r8
	adds r1, r5, #0
	movs r3, #0
	str r7, [sp, #4]
	str r6, [sp, #12]
	bl Func_02000eac
.L_0200890a:
	movs r0, #0
	add sp, #56
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008918:
	.4byte Data_0300122c
	.section .text.x0200891c,"ax",%progbits
	.global Func_0200091c
	.thumb_func
Func_0200091c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r0, #9
	bl Object_GetById
	adds r6, r0, #0
	movs r0, #10
	bl Object_GetById
	adds r7, r0, #0
	movs r0, #23
	bl Object_GetById
	adds r5, r0, #0
	ldr r2, [r5, #80]
	movs r1, #128
	mov r8, r2
	movs r2, #248
	movs r0, #24
	lsls r1, r1, #18
	lsls r2, r2, #16
	bl Func_02001ab4
	movs r1, #128
	movs r2, #248
	movs r0, #23
	lsls r1, r1, #18
	lsls r2, r2, #16
	bl Func_02001ab4
	movs r1, #236
	movs r2, #128
	movs r0, #9
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_02001ab4
	movs r1, #138
	movs r2, #128
	lsls r2, r2, #17
	movs r0, #10
	lsls r1, r1, #18
	bl Func_02001ab4
	movs r0, #24
	bl Object_GetById
	movs r3, #85
	movs r2, #0
	adds r3, r3, r5
	str r2, [r0, #24]
	strb r2, [r3]
	mov r11, r3
	ldr r3, [r5, #20]
	movs r0, #160
	str r3, [r5, #12]
	movs r3, #85
	adds r3, r3, r6
	strb r2, [r3]
	mov r9, r3
	ldr r3, [r6, #20]
	lsls r0, r0, #4
	str r3, [r6, #12]
	movs r3, #85
	adds r3, r3, r7
	strb r2, [r3]
	mov r10, r3
	ldr r3, [r7, #20]
	adds r0, #10
	str r3, [r7, #12]
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020089f8
	ldr r3, [r6, #12]
	ldr r2, .L_02008a90
	movs r0, #9
	adds r3, r3, r2
	str r3, [r6, #12]
	ldr r2, .L_02008a94
	ldr r3, [r5, #12]
	adds r3, r3, r2
	str r3, [r5, #12]
	ldr r2, .L_02008a98
	ldr r3, [r5, #8]
	adds r3, r3, r2
	str r3, [r5, #8]
	mov r2, r8
	ldrh r3, [r2, #18]
	movs r2, #128
	lsls r2, r2, #4
	adds r3, r3, r2
	mov r2, r8
	strh r3, [r2, #18]
	bl Object_GetById
	movs r1, #4
	bl Animation_ApplyChildValues
	movs r0, #11
	bl Object_GetById
	movs r1, #4
	bl Animation_ApplyChildValues
.L_020089f8:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #11
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008a42
	ldr r3, [r7, #12]
	ldr r2, .L_02008a90
	movs r0, #10
	adds r3, r3, r2
	str r3, [r7, #12]
	ldr r2, .L_02008a94
	ldr r3, [r5, #12]
	adds r3, r3, r2
	str r3, [r5, #12]
	ldr r2, .L_02008a9c
	ldr r3, [r5, #8]
	adds r3, r3, r2
	str r3, [r5, #8]
	mov r2, r8
	ldrh r3, [r2, #18]
	ldr r2, .L_02008aa0
	adds r3, r3, r2
	mov r2, r8
	strh r3, [r2, #18]
	bl Object_GetById
	movs r1, #4
	bl Animation_ApplyChildValues
	movs r0, #12
	bl Object_GetById
	movs r1, #4
	bl Animation_ApplyChildValues
.L_02008a42:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #10
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008a82
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #11
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008a82
	ldr r3, [r6, #12]
	ldr r2, .L_02008aa4
	adds r3, r3, r2
	str r3, [r6, #12]
	ldr r3, [r7, #12]
	adds r3, r3, r2
	str r3, [r7, #12]
	ldr r2, .L_02008aa8
	ldr r3, [r5, #12]
	adds r3, r3, r2
	str r3, [r5, #12]
	mov r2, r9
	movs r3, #4
	strb r3, [r2]
	mov r2, r10
	strb r3, [r2]
	mov r2, r11
	strb r3, [r2]
.L_02008a82:
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008a90:
	.4byte 0x00066640
.L_02008a94:
	.4byte 0x0001eb80
.L_02008a98:
	.4byte 0xfffd70c0
.L_02008a9c:
	.4byte 0x00028f40
.L_02008aa0:
	.4byte 0xfffff800
.L_02008aa4:
	.4byte 0x00199900
.L_02008aa8:
	.4byte 0x001b8480
	.section .text.x02008aac,"ax",%progbits
	.global Func_02000aac
	.thumb_func
Func_02000aac:
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
	ldr r3, .L_02008c40
	adds r2, r1, #0
	adds r2, #228
	ldr r0, [r2]
	ldr r2, [r2, #4]
	ands r0, r3
	ands r2, r3
	ldr r3, .L_02008c44
	mov r10, r0
	movs r0, #0
	ldrsh r3, [r3, r0]
	mov r8, r2
	ldr r2, .L_02008c48
	lsls r3, r3, #2
	adds r3, r3, r2
	ldrh r3, [r3, #2]
	sub sp, #8
	lsrs r3, r3, #5
	str r3, [sp, #4]
	ldr r6, .L_02008c4c
	ldr r3, [r1]
	movs r1, #0
	ldr r3, [r3, #4]
	mov r9, r1
	str r3, [sp, #0]
	ldr r3, .L_02008c50
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r9, r3
	blt .L_02008afe
	b .L_02008c32
.L_02008afe:
	ldr r2, .L_02008c54
	mov r0, r9
	lsls r3, r0, #2
	ldr r5, [r2, r3]
	cmp r5, #0
	bne .L_02008b0c
	b .L_02008c22
.L_02008b0c:
	ldr r3, [r5, #8]
	cmp r3, #0
	bne .L_02008b14
	b .L_02008c22
.L_02008b14:
	mov r1, r10
	subs r0, r3, r1
	ldr r2, [sp, #0]
	ldr r3, [r5, #12]
	movs r1, #128
	subs r3, r3, r2
	ldr r2, [r5, #16]
	lsls r1, r1, #12
	adds r3, r3, r1
	mov r1, r8
	subs r2, r2, r1
	ldr r1, [sp, #0]
	subs r2, r2, r1
	subs r4, r2, r3
	adds r3, r3, r2
	asrs r3, r3, #16
	adds r3, #58
	mov r11, r3
	ldr r3, .L_02008c58
	movs r1, #0
	ldrsh r2, [r3, r1]
	adds r3, r5, #0
	mov r12, r2
	asrs r1, r0, #16
	mov r0, r12
	adds r3, #100
	asrs r2, r4, #16
	cmp r0, #0
	bne .L_02008b8a
	movs r0, #0
	ldrsh r7, [r3, r0]
	adds r0, r1, #0
	adds r3, r1, #7
	movs r1, #167
	adds r4, r2, #0
	lsls r1, r1, #1
	subs r0, #8
	subs r4, #16
	cmp r3, r1
	bhi .L_02008c22
	movs r2, #16
	negs r2, r2
	cmp r4, r2
	ble .L_02008c22
	cmp r4, #239
	bgt .L_02008c22
	movs r3, #128
	lsls r3, r3, #1
	adds r3, #255
	ands r0, r3
	movs r3, #255
	adds r1, r6, #0
	ands r4, r3
	mov r3, r12
	stmia r1!, {r3}
	lsls r3, r0, #16
	orrs r4, r3
	ldr r3, .L_02008c5c
	b .L_02008bc6
.L_02008b8a:
	movs r0, #0
	ldrsh r7, [r3, r0]
	adds r0, r1, #0
	adds r3, r1, #0
	movs r1, #175
	adds r4, r2, #0
	adds r3, #23
	lsls r1, r1, #1
	subs r0, #8
	subs r4, #64
	cmp r3, r1
	bhi .L_02008c22
	movs r2, #64
	negs r2, r2
	cmp r4, r2
	ble .L_02008c22
	cmp r4, #175
	bgt .L_02008c22
	movs r3, #128
	lsls r3, r3, #1
	adds r3, #255
	ands r0, r3
	movs r3, #255
	adds r1, r6, #0
	ands r4, r3
	movs r3, #0
	stmia r1!, {r3}
	lsls r3, r0, #16
	orrs r4, r3
	ldr r3, .L_02008c60
.L_02008bc6:
	movs r2, #128
	orrs r4, r3
	stmia r1!, {r4}
	ldr r0, [sp, #4]
	lsls r3, r7, #3
	adds r3, r0, r3
	lsls r2, r2, #4
	orrs r3, r2
	str r3, [r1]
	ldr r3, .L_02008c64
	movs r0, #1
	ldrh r2, [r3]
	movs r1, #0
	ldrsh r3, [r3, r1]
	negs r0, r0
	cmp r3, r0
	bne .L_02008c04
	adds r0, r5, #0
	bl Func_02001b24
	movs r3, #3
	ands r0, r3
	movs r1, #13
	ldrb r3, [r6, #9]
	negs r1, r1
	adds r2, r1, #0
	lsls r0, r0, #2
	ands r3, r2
	orrs r3, r0
	strb r3, [r6, #9]
	b .L_02008c18
.L_02008c04:
	movs r3, #3
	ands r3, r2
	movs r0, #13
	ldrb r2, [r6, #9]
	negs r0, r0
	adds r1, r0, #0
	lsls r3, r3, #2
	ands r2, r1
	orrs r2, r3
	strb r2, [r6, #9]
.L_02008c18:
	adds r0, r6, #0
	mov r1, r11
	bl Func_020019d4
	adds r6, #12
.L_02008c22:
	ldr r3, .L_02008c50
	movs r1, #1
	movs r2, #0
	ldrsh r3, [r3, r2]
	add r9, r1
	cmp r9, r3
	bge .L_02008c32
	b .L_02008afe
.L_02008c32:
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_02008c40:
	.4byte 0xffff0000
.L_02008c44:
	.4byte gOverlayArea + 0x2200
.L_02008c48:
	.4byte ResourceTableEntries
.L_02008c4c:
	.4byte gOverlayArea + 0x2244
.L_02008c50:
	.4byte gOverlayArea + 0x2202
.L_02008c54:
	.4byte gOverlayArea + 0x2204
.L_02008c58:
	.4byte gOverlayArea + 0x2304
.L_02008c5c:
	.4byte 0x40002000
.L_02008c60:
	.4byte 0xc000a000
.L_02008c64:
	.4byte gOverlayArea + 0x2306
	.section .text.x02008c68,"ax",%progbits
	.global Func_02000c68
	.thumb_func
Func_02000c68:
	push {r5, r6, lr}
	movs r0, #192
	lsls r0, r0, #4
	bl Runtime_BumpAllocate
	ldr r3, .L_02008cc8
	adds r6, r0, #0
	movs r1, #64
	ldr r0, .L_02008ccc
	mov lr, r3
	.2byte 0xf800
	adds r1, r6, #0
	ldr r0, .L_02008cd0
	bl Func_020019bc
	ldr r5, .L_02008cd4
	bl Resource_FindFreeEntry
	movs r1, #192
	strh r0, [r5]
	lsls r0, r0, #16
	adds r2, r6, #0
	lsls r1, r1, #4
	asrs r0, r0, #16
	bl VramBlock_LoadCached
	adds r0, r6, #0
	bl Sys_Free
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_02008cd8
	bl Scheduler_AddOrUpdateCallback
	ldr r3, .L_02008cdc
	ldr r2, .L_02008cc0
	strh r2, [r3]
	ldr r3, .L_02008ce0
	strh r2, [r3]
	ldr r2, .L_02008ce4
	ldr r3, .L_02008cc4
	strh r3, [r2]
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02008cc0:
	.4byte 0x00000000
.L_02008cc4:
	.4byte 0xffffffff
.L_02008cc8:
	.4byte IwramClearWords
.L_02008ccc:
	.4byte gOverlayArea + 0x2204
.L_02008cd0:
	.4byte Data_02001b4c
.L_02008cd4:
	.4byte gOverlayArea + 0x2200
.L_02008cd8:
	.4byte Func_02000aac
.L_02008cdc:
	.4byte gOverlayArea + 0x2202
.L_02008ce0:
	.4byte gOverlayArea + 0x2304
.L_02008ce4:
	.4byte gOverlayArea + 0x2306
	.section .text.x02008ce8,"ax",%progbits
	.global Func_02000ce8
	.thumb_func
Func_02000ce8:
	push {r5, r6, lr}
	movs r0, #192
	lsls r0, r0, #4
	bl Runtime_BumpAllocate
	ldr r3, .L_02008d48
	adds r6, r0, #0
	movs r1, #64
	ldr r0, .L_02008d4c
	mov lr, r3
	.2byte 0xf800
	adds r1, r6, #0
	ldr r0, .L_02008d50
	bl Func_020019bc
	ldr r5, .L_02008d54
	bl Resource_FindFreeEntry
	movs r1, #128
	strh r0, [r5]
	lsls r0, r0, #16
	adds r2, r6, #0
	lsls r1, r1, #4
	asrs r0, r0, #16
	bl VramBlock_LoadCached
	adds r0, r6, #0
	bl Sys_Free
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_02008d58
	bl Scheduler_AddOrUpdateCallback
	ldr r3, .L_02008d5c
	ldr r2, .L_02008d40
	strh r2, [r3]
	ldr r3, .L_02008d60
	strh r2, [r3]
	ldr r2, .L_02008d64
	ldr r3, .L_02008d44
	strh r3, [r2]
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02008d40:
	.4byte 0x00000000
.L_02008d44:
	.4byte 0xffffffff
.L_02008d48:
	.4byte IwramClearWords
.L_02008d4c:
	.4byte gOverlayArea + 0x2204
.L_02008d50:
	.4byte Data_02001cae + 0x1
.L_02008d54:
	.4byte gOverlayArea + 0x2200
.L_02008d58:
	.4byte Func_02000aac
.L_02008d5c:
	.4byte gOverlayArea + 0x2202
.L_02008d60:
	.4byte gOverlayArea + 0x2304
.L_02008d64:
	.4byte gOverlayArea + 0x2306
	.section .text.x02008d68,"ax",%progbits
	.global Func_02000d68
	.thumb_func
Func_02000d68:
	push {r5, r6, lr}
	movs r0, #128
	lsls r0, r0, #4
	bl Runtime_BumpAllocate
	ldr r3, .L_02008dcc
	adds r6, r0, #0
	movs r1, #64
	ldr r0, .L_02008dd0
	mov lr, r3
	.2byte 0xf800
	adds r1, r6, #0
	ldr r0, .L_02008dd4
	bl Func_020019bc
	ldr r5, .L_02008dd8
	bl Resource_FindFreeEntry
	movs r1, #128
	strh r0, [r5]
	lsls r0, r0, #16
	adds r2, r6, #0
	lsls r1, r1, #4
	asrs r0, r0, #16
	bl VramBlock_LoadCached
	adds r0, r6, #0
	bl Sys_Free
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_02008ddc
	bl Scheduler_AddOrUpdateCallback
	ldr r2, .L_02008de0
	ldr r3, .L_02008dc0
	strh r3, [r2]
	ldr r2, .L_02008de4
	ldr r3, .L_02008dc4
	strh r3, [r2]
	ldr r2, .L_02008de8
	ldr r3, .L_02008dc8
	strh r3, [r2]
	b .L_02008dec
.L_02008dc0:
	.4byte 0x00000000
.L_02008dc4:
	.4byte 0x00000001
.L_02008dc8:
	.4byte 0xffffffff
.L_02008dcc:
	.4byte IwramClearWords
.L_02008dd0:
	.4byte gOverlayArea + 0x2204
.L_02008dd4:
	.4byte Data_02001ede
.L_02008dd8:
	.4byte gOverlayArea + 0x2200
.L_02008ddc:
	.4byte Func_02000aac
.L_02008de0:
	.4byte gOverlayArea + 0x2202
.L_02008de4:
	.4byte gOverlayArea + 0x2304
.L_02008de8:
	.4byte gOverlayArea + 0x2306
.L_02008dec:
	pop {r5, r6, pc}
	.2byte 0x0000
	.section .text.x02008df0,"ax",%progbits
	.global Func_02000df0
	.thumb_func
Func_02000df0:
	push {r5, lr}
	adds r5, r1, #0
	bl Object_GetById
	adds r4, r0, #0
	cmp r4, #0
	beq .L_02008e16
	adds r3, r4, #0
	adds r3, #100
	strh r5, [r3]
	ldr r1, .L_02008e18
	ldr r0, .L_02008e1c
	ldrh r2, [r1]
	movs r5, #0
	ldrsh r3, [r1, r5]
	adds r2, #1
	lsls r3, r3, #2
	str r4, [r0, r3]
	strh r2, [r1]
.L_02008e16:
	pop {r5, pc}
.L_02008e18:
	.4byte gOverlayArea + 0x2202
.L_02008e1c:
	.4byte gOverlayArea + 0x2204
	.section .text.x02008e20,"ax",%progbits
	.global Func_02000e20
	.thumb_func
Func_02000e20:
	ldr r3, .L_02008e28
	strh r0, [r3]
	bx lr
	.2byte 0x0000
.L_02008e28:
	.4byte gOverlayArea + 0x2306
	.section .text.x02008e72,"ax",%progbits
	.2byte 0x0000
	.section .text.x02008e74,"ax",%progbits
	.global Func_02000e74
	.thumb_func
Func_02000e74:
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
	.section .text.x02008eac,"ax",%progbits
	.global Func_02000eac
	.thumb_func
Func_02000eac:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	mov r11, r3
	ldr r3, .L_02009064
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
	beq .L_02008ef4
	cmp r7, #0
	beq .L_02008ef4
	movs r2, #24
	ldrsh r0, [r7, r2]
	adds r1, r5, #0
	adds r2, r6, #0
	b .L_02008efc
.L_02008ef4:
	movs r0, #30
	adds r2, r6, #0
	adds r0, #255
	adds r1, r5, #0
.L_02008efc:
	mov r3, r10
	bl Func_02001a14
	adds r6, r0, #0
	cmp r6, #0
	bne .L_02008f0a
	b .L_02009056
.L_02008f0a:
	ldr r3, [r6, #80]
	mov r1, r8
	movs r5, #15
	adds r1, #1
	ands r1, r5
	adds r0, r6, #0
	str r3, [sp, #0]
	bl Func_020019fc
	ldr r2, .L_02009068
	mov r3, r8
	ands r3, r5
	lsls r3, r3, #2
	ldr r1, [r2, r3]
	adds r0, r6, #0
	mov r10, r3
	bl Func_02001a0c
	adds r3, r6, #0
	movs r5, #0
	adds r3, #85
	strb r5, [r3]
	adds r0, r6, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	ldr r3, .L_0200906c
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
	ldr r3, .L_02009070
	mov r1, r8
	ands r3, r1
	cmp r3, #0
	beq .L_02009056
	cmp r7, #0
	beq .L_02009056
	movs r3, #128
	lsls r3, r3, #9
	ands r3, r1
	cmp r3, #0
	beq .L_02008f8c
	ldr r1, [r7, #4]
	adds r0, r6, #0
	bl Object_SetPartAttribute
.L_02008f8c:
	movs r3, #128
	lsls r3, r3, #10
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_02008fac
	adds r1, r6, #0
	adds r1, #35
	ldrb r3, [r1]
	movs r2, #254
	ands r2, r3
	strb r2, [r1]
	ldr r1, [r7]
	adds r0, r6, #0
	bl Object_SetSpritePriority
.L_02008fac:
	movs r2, #128
	lsls r2, r2, #12
	mov r3, r8
	ands r2, r3
	cmp r2, #0
	beq .L_02008fc0
	ldr r3, [r7, #8]
	str r3, [r6, #24]
	ldr r3, [r7, #12]
	str r3, [r6, #28]
.L_02008fc0:
	movs r3, #128
	lsls r3, r3, #11
	mov r1, r8
	ands r3, r1
	cmp r3, #0
	beq .L_02009006
	ldr r3, .L_02009068
	mov r1, r10
	ldr r5, [r3, r1]
	ldr r3, [r7, #16]
	ldr r1, [r5, #12]
	cmp r2, #0
	beq .L_02008fee
	ldr r0, [r6, #24]
	subs r0, r3, r0
	bl Engine_MathDivide
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, [r6, #28]
	ldr r1, [r5, #12]
	subs r0, r0, r3
	b .L_02009000
.L_02008fee:
	ldr r2, .L_02009070
	adds r0, r3, r2
	bl Engine_MathDivide
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, .L_02009070
	ldr r1, [r5, #12]
	adds r0, r0, r3
.L_02009000:
	bl Engine_MathDivide
	str r0, [r6, #52]
.L_02009006:
	movs r3, #128
	lsls r3, r3, #14
	mov r1, r8
	ands r3, r1
	cmp r3, #0
	beq .L_02009022
	adds r0, r6, #0
	movs r1, #1
	bl Func_020019fc
	ldr r1, [r7, #28]
	adds r0, r6, #0
	bl Func_02001a0c
.L_02009022:
	movs r3, #128
	lsls r3, r3, #15
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_02009034
	ldrh r3, [r7, #32]
	ldr r1, [sp, #0]
	strh r3, [r1, #18]
.L_02009034:
	movs r3, #128
	lsls r3, r3, #16
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_02009046
	ldrh r3, [r7, #34]
	mov r1, r9
	strh r3, [r1]
.L_02009046:
	movs r3, #128
	lsls r3, r3, #17
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_02009056
	ldr r3, [r7, #36]
	str r3, [r6, #108]
.L_02009056:
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_02009064:
	.4byte gPartyState
.L_02009068:
	.4byte Data_020021b0
.L_0200906c:
	.4byte Func_02000e74
.L_02009070:
	.4byte 0xffff0000
	.section .text.x02009074,"ax",%progbits
	.global Func_02001074
	.thumb_func
Func_02001074:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r4, .L_0200918c
	movs r1, #1
	movs r0, #12
	ldrsh r3, [r4, r0]
	negs r1, r1
	sub sp, #4
	cmp r3, r1
	beq .L_02009180
	lsls r3, r3, #3
	adds r3, r3, r4
	adds r3, #32
	mov r8, r3
	ldr r3, .L_02009190
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	str r4, [sp, #0]
	bl Object_GetById
	mov r1, r8
	ldr r3, [r0, #8]
	movs r5, #0
	ldrsh r2, [r1, r5]
	asrs r3, r3, #20
	ldr r4, [sp, #0]
	cmp r3, r2
	bne .L_020090c0
	ldr r3, [r0, #16]
	movs r5, #2
	ldrsh r2, [r1, r5]
	asrs r3, r3, #20
	cmp r3, r2
	beq .L_020090c8
.L_020090c0:
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	strh r3, [r4, #12]
.L_020090c8:
	movs r0, #12
	ldrsh r3, [r4, r0]
	movs r2, #1
	negs r2, r2
	ldr r1, .L_02009194
	cmp r3, r2
	beq .L_02009180
	movs r5, #14
	ldrsh r3, [r4, r5]
	cmp r3, #0
	beq .L_02009180
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #32]
	str r4, [sp, #0]
	adds r3, r2, #0
	adds r3, #228
	ldr r0, [r3]
	ldr r5, [r3, #4]
	ldr r3, [r2]
	ands r0, r1
	ands r5, r1
	ldr r6, [r3, #4]
	movs r1, #16
	ldrsh r3, [r4, r1]
	ldr r2, .L_02009198
	lsls r3, r3, #2
	adds r3, r3, r2
	ldrh r3, [r3, #2]
	lsrs r3, r3, #5
	mov r10, r3
	mov r3, r8
	movs r2, #0
	ldrsh r1, [r3, r2]
	lsls r1, r1, #20
	subs r7, r1, r0
	movs r0, #2
	ldrsh r2, [r3, r0]
	movs r0, #0
	lsls r2, r2, #20
	bl Map_GetTerrainHeight
	mov r2, r8
	movs r1, #2
	ldrsh r3, [r2, r1]
	subs r0, r0, r6
	lsls r3, r3, #20
	subs r3, r3, r5
	subs r3, r3, r6
	subs r2, r3, r0
	asrs r7, r7, #16
	adds r0, r0, r3
	asrs r0, r0, #16
	adds r3, r7, #0
	movs r5, #167
	asrs r2, r2, #16
	adds r1, r0, #0
	adds r3, #15
	lsls r5, r5, #1
	adds r2, #14
	adds r1, #58
	ldr r4, [sp, #0]
	cmp r3, r5
	bhi .L_02009180
	movs r0, #15
	negs r0, r0
	cmp r2, r0
	blt .L_02009180
	cmp r2, #239
	bgt .L_02009180
	movs r3, #128
	lsls r3, r3, #1
	adds r3, #255
	ands r7, r3
	movs r3, #255
	ands r2, r3
	movs r3, #0
	str r3, [r4, #20]
	lsls r3, r7, #16
	orrs r2, r3
	ldr r3, .L_0200919c
	adds r0, r4, #0
	orrs r2, r3
	movs r3, #128
	str r2, [r4, #24]
	lsls r3, r3, #3
	mov r2, r10
	orrs r2, r3
	str r2, [r4, #28]
	adds r0, #20
	bl Func_020019d4
.L_02009180:
	add sp, #4
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200918c:
	.4byte gOverlayArea + 0x2308
.L_02009190:
	.4byte gPartyState
.L_02009194:
	.4byte 0xffff0000
.L_02009198:
	.4byte ResourceTableEntries
.L_0200919c:
	.4byte 0x80008800
	.section .text.x020091a0,"ax",%progbits
	.global Func_020011a0
	.thumb_func
Func_020011a0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #48
	str r0, [sp, #44]
	ldr r0, .L_020093d0
	str r1, [sp, #40]
	mov r8, r0
	movs r1, #32
	add r1, r8
	mov r9, r1
	mov r12, r9
	adds r5, r2, #0
	mov r2, r12
	adds r6, r3, #0
	str r2, [sp, #8]
	ldr r3, .L_020093d4
	movs r1, #4
	ldr r7, [sp, #80]
	mov lr, r3
	.2byte 0xf800
	add r0, sp, #44
	ldrh r0, [r0]
	mov r1, r8
	strh r0, [r1, #4]
	add r1, sp, #40
	ldrh r1, [r1]
	mov r3, r8
	strh r1, [r3]
	strh r5, [r3, #2]
	movs r3, #255
	lsls r3, r3, #8
	mov r5, r8
	mov r0, r8
	adds r3, #255
	mov r1, r8
	strh r6, [r5, #6]
	movs r2, #0
	strh r7, [r0, #8]
	strh r3, [r1, #12]
	mov r3, r8
	strh r2, [r3, #10]
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	movs r2, #132
	mov r12, r3
	lsls r2, r2, #1
	mov r1, r12
	add r2, r12
	adds r1, #236
	ldr r0, [r1]
	ldr r3, [r2, #8]
	ldr r5, [r2, #48]
	adds r3, r3, r0
	asrs r3, r3, #20
	str r3, [sp, #32]
	adds r1, #4
	ldr r3, [r2, #12]
	ldr r2, [r1]
	adds r3, r3, r2
	asrs r3, r3, #20
	str r3, [sp, #28]
	mov r3, r12
	adds r3, #244
	ldr r3, [r3]
	subs r3, r3, r0
	asrs r3, r3, #20
	str r3, [sp, #24]
	mov r3, r12
	adds r3, #248
	ldr r3, [r3]
	asrs r0, r0, #20
	subs r3, r3, r2
	asrs r2, r2, #20
	lsls r2, r2, #7
	adds r2, r2, r0
	lsls r2, r2, #2
	asrs r3, r3, #20
	adds r5, r5, r2
	movs r0, #0
	str r3, [sp, #20]
	str r5, [sp, #36]
	str r0, [sp, #12]
	cmp r0, r3
	bge .L_02009324
.L_02009254:
	ldr r1, [sp, #12]
	ldr r2, [sp, #36]
	ldr r5, [sp, #24]
	lsls r3, r1, #9
	adds r2, r2, r3
	movs r3, #0
	mov r11, r2
	str r3, [sp, #16]
	cmp r3, r5
	bge .L_02009318
.L_02009268:
	mov r0, r11
	ldrb r5, [r0, #2]
	cmp r5, #0
	beq .L_02009308
	ldr r1, [sp, #44]
	cmp r5, r1
	bcc .L_02009308
	adds r1, #1
	mov r10, r1
	cmp r5, r10
	bhi .L_02009308
	ldr r2, [sp, #16]
	ldr r3, [sp, #32]
	mov r0, r9
	adds r7, r2, r3
	strh r7, [r0]
	ldr r1, [sp, #12]
	ldr r2, [sp, #28]
	add r0, sp, #40
	ldrh r0, [r0]
	adds r6, r1, r2
	mov r3, r9
	mov r1, r9
	strh r6, [r3, #2]
	strh r0, [r1, #4]
	ldr r1, [sp, #40]
	movs r0, #10
	adds r1, #1
	adds r0, #255
	str r1, [sp, #40]
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020092bc
	cmp r5, r10
	bne .L_020092fa
	mov r3, r9
	movs r2, #4
	ldrsh r0, [r3, r2]
	bl GameFlag_SetBit
	b .L_020092fa
.L_020092bc:
	mov r1, r9
	movs r5, #4
	ldrsh r0, [r1, r5]
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020092fa
	mov r2, r8
	ldrh r4, [r2, #6]
	ldrh r5, [r2, #8]
	movs r3, #8
	ldrsh r1, [r2, r3]
	movs r3, #6
	ldrsh r0, [r2, r3]
	movs r2, #64
	adds r3, r2, #0
	ands r3, r4
	ands r2, r5
	lsls r3, r3, #16
	lsls r2, r2, #16
	asrs r3, r3, #16
	asrs r2, r2, #16
	orrs r7, r3
	orrs r6, r2
	adds r1, #1
	movs r2, #1
	movs r3, #1
	str r7, [sp, #0]
	str r6, [sp, #4]
	bl Func_02001a44
.L_020092fa:
	mov r0, r8
	ldrh r3, [r0, #10]
	mov r1, r8
	adds r3, #1
	strh r3, [r1, #10]
	movs r5, #8
	add r9, r5
.L_02009308:
	ldr r2, [sp, #16]
	ldr r5, [sp, #24]
	adds r2, #1
	movs r3, #4
	str r2, [sp, #16]
	add r11, r3
	cmp r2, r5
	blt .L_02009268
.L_02009318:
	ldr r0, [sp, #12]
	ldr r1, [sp, #20]
	adds r0, #1
	str r0, [sp, #12]
	cmp r0, r1
	blt .L_02009254
.L_02009324:
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200937c
	ldr r3, .L_020093d8
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	ldr r3, [r0, #8]
	movs r2, #0
	asrs r4, r3, #20
	ldr r3, [r0, #16]
	mov r0, r8
	asrs r1, r3, #20
	ldr r3, [sp, #8]
	mov r9, r3
	movs r5, #10
	ldrsh r3, [r0, r5]
	cmp r2, r3
	bge .L_0200937c
.L_02009356:
	mov r0, r9
	movs r5, #0
	ldrsh r3, [r0, r5]
	cmp r3, r4
	bne .L_0200936c
	movs r5, #2
	ldrsh r3, [r0, r5]
	cmp r3, r1
	bne .L_0200936c
	mov r0, r8
	strh r2, [r0, #12]
.L_0200936c:
	movs r3, #8
	mov r0, r8
	add r9, r3
	movs r5, #10
	ldrsh r3, [r0, r5]
	adds r2, #1
	cmp r2, r3
	blt .L_02009356
.L_0200937c:
	movs r0, #128
	lsls r0, r0, #1
	bl Runtime_BumpAllocate
	adds r5, r0, #0
	adds r1, r5, #0
	movs r2, #63
.L_0200938a:
	ldr r3, .L_020093dc
	subs r2, #1
	stmia r1!, {r3}
	cmp r2, #0
	bge .L_0200938a
	bl Resource_FindFreeEntry
	mov r1, r8
	strh r0, [r1, #16]
	lsls r0, r0, #16
	movs r1, #128
	adds r2, r5, #0
	lsls r1, r1, #1
	asrs r0, r0, #16
	bl VramBlock_LoadCached
	adds r0, r5, #0
	bl Sys_Free
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_020093e0
	bl Scheduler_AddOrUpdateCallback
	mov r3, r8
	movs r2, #10
	ldrsh r0, [r3, r2]
	add sp, #48
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_020093d0:
	.4byte gOverlayArea + 0x2308
.L_020093d4:
	.4byte IwramClearWords
.L_020093d8:
	.4byte gPartyState
.L_020093dc:
	.4byte 0x11111111
.L_020093e0:
	.4byte Func_02001074
	.section .text.x020093e4,"ax",%progbits
	.global Func_020013e4
	.thumb_func
Func_020013e4:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r8
	push {r5, r6}
	ldr r1, .L_02009464
	movs r2, #133
	mov r8, r1
	lsls r2, r2, #2
	add r8, r2
	mov r3, r8
	ldr r0, [r3]
	bl Object_GetById
	mov r1, r8
	ldr r5, [r0, #8]
	ldr r6, [r0, #16]
	mov r10, r0
	movs r2, #128
	ldr r0, [r1]
	movs r1, #128
	lsls r1, r1, #11
	lsls r2, r2, #10
	bl ObjectMotion_SetSpeedParameters
	asrs r5, r5, #20
	mov r2, r8
	asrs r6, r6, #20
	ldr r0, [r2]
	lsls r1, r5, #4
	lsls r2, r6, #4
	adds r1, #8
	adds r2, #8
	bl ObjectMotion_SetPositionAndCommit
	movs r0, #1
	bl WaitFrames
	movs r3, #128
	lsls r3, r3, #12
	lsls r5, r5, #20
	lsls r6, r6, #20
	adds r5, r5, r3
	mov r1, r10
	adds r6, r6, r3
	ldr r2, [r1, #12]
	adds r3, r6, #0
	adds r1, r5, #0
	mov r0, r10
	bl Object_SetPositionAndResetMotion
	movs r0, #4
	bl Battle_WaitMode0
	bl Func_02001ae4
	ldr r2, .L_02009468
	ldr r3, [r0, #12]
	adds r3, r3, r2
	str r3, [r0, #12]
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02009464:
	.4byte gPartyState
.L_02009468:
	.4byte 0xfff80000
	.section .text.x0200946c,"ax",%progbits
	.global Func_0200146c
	.thumb_func
Func_0200146c:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	ldr r3, .L_020094d8
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	mov r8, r0
	ldr r0, [r3]
	sub sp, #8
	bl Object_GetById
	mov r2, r8
	ldrh r1, [r2, #6]
	movs r2, #64
	ldr r6, [r0, #8]
	adds r3, r2, #0
	ands r3, r1
	lsls r3, r3, #16
	mov r1, r8
	asrs r3, r3, #16
	asrs r6, r6, #20
	orrs r6, r3
	ldrh r3, [r1, #8]
	ldr r5, [r0, #16]
	ands r2, r3
	lsls r2, r2, #16
	asrs r2, r2, #16
	asrs r5, r5, #20
	orrs r5, r2
	bl Func_020013e4
	movs r0, #161
	bl Func_02001b44
	mov r3, r8
	movs r2, #8
	ldrsh r1, [r3, r2]
	movs r2, #6
	ldrsh r0, [r3, r2]
	adds r1, #1
	movs r2, #1
	movs r3, #1
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl Func_02001a44
	movs r0, #12
	bl Battle_WaitMode0
	add sp, #8
	pop {r3}
	mov r8, r3
	pop {r5, r6, pc}
.L_020094d8:
	.4byte gPartyState
	.section .text.x020094dc,"ax",%progbits
	.global Func_020014dc
	.thumb_func
Func_020014dc:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r1, .L_0200958c
	movs r2, #133
	lsls r2, r2, #2
	adds r1, r1, r2
	mov r8, r0
	ldr r0, [r1]
	sub sp, #8
	mov r10, r1
	bl Object_GetById
	adds r6, r0, #0
	ldr r3, [r6, #8]
	movs r2, #64
	asrs r7, r3, #20
	mov r3, r8
	ldrh r1, [r3, #6]
	adds r3, r2, #0
	ands r3, r1
	lsls r3, r3, #16
	mov r1, r8
	asrs r3, r3, #16
	orrs r7, r3
	ldrh r3, [r1, #8]
	ldr r5, [r6, #16]
	ands r2, r3
	lsls r2, r2, #16
	asrs r2, r2, #16
	asrs r5, r5, #20
	orrs r5, r2
	bl Func_020013e4
	movs r0, #229
	bl Func_02001b44
	mov r3, r8
	movs r2, #8
	ldrsh r1, [r3, r2]
	movs r2, #6
	ldrsh r0, [r3, r2]
	adds r1, #2
	movs r2, #1
	movs r3, #1
	str r7, [sp, #0]
	str r5, [sp, #4]
	bl Func_02001a44
	movs r0, #12
	bl Battle_WaitMode0
	movs r3, #128
	ldr r2, .L_02009584
	lsls r3, r3, #7
	strh r3, [r6, #6]
	adds r3, r6, #0
	adds r3, #85
	strb r2, [r3]
	mov r3, r10
	ldr r0, [r3]
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r2, #226
	movs r3, #128
	lsls r2, r2, #4
	lsls r3, r3, #19
	adds r2, #255
	adds r3, #74
	strh r2, [r3]
	movs r1, #128
	lsls r1, r1, #19
	ldrh r3, [r1]
	ldr r2, .L_02009588
	movs r7, #0
	orrs r3, r2
	strh r3, [r1]
	mov r2, r10
	b .L_02009590
	.2byte 0x0000
.L_02009584:
	.4byte 0x00000000
.L_02009588:
	.4byte 0x00008000
.L_0200958c:
	.4byte gPartyState
.L_02009590:
	movs r3, #1
	mov r1, r8
	strh r3, [r1, #14]
	ldr r0, [r2]
	movs r1, #28
	bl Object_SetModeById
	movs r0, #16
	bl Battle_WaitMode0
.L_020095a4:
	cmp r7, #5
	bne .L_020095ae
	movs r0, #204
	bl Func_02001b44
.L_020095ae:
	ldr r3, [r6, #24]
	ldr r1, .L_0200960c
	ldr r2, .L_02009610
	adds r3, r3, r1
	str r3, [r6, #24]
	ldr r3, [r6, #28]
	ldr r1, .L_02009614
	adds r3, r3, r2
	str r3, [r6, #28]
	ldr r3, [r6, #12]
	movs r0, #1
	adds r3, r3, r1
	str r3, [r6, #12]
	adds r7, #1
	bl WaitFrames
	cmp r7, #39
	ble .L_020095a4
	ldr r3, .L_02009618
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	movs r3, #0
	adds r0, #84
	strb r3, [r0]
	mov r1, r8
	strh r3, [r1, #14]
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #93
	str r2, [r3]
	bl Event_ClearStatus1c6
	bl Event_WaitValue1c8Frames
	add sp, #8
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_0200960c:
	.4byte 0xfffffc00
.L_02009610:
	.4byte 0xfffffd00
.L_02009614:
	.4byte 0xffff6667
.L_02009618:
	.4byte gPartyState
	.section .text.x0200961c,"ax",%progbits
	.global Func_0200161c
	.thumb_func
Func_0200161c:
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
	bge .L_0200964c
	adds r3, #15
.L_0200964c:
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
	.section .text.x02009674,"ax",%progbits
	.global Func_02001674
	.thumb_func
Func_02001674:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, .L_020097f8
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	sub sp, #68
	bl Object_GetById
	adds r7, r0, #0
	bl Func_02001a7c
	movs r0, #0
	bl Func_02001b0c
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r1, r1
	negs r2, r2
	movs r3, #0
	negs r0, r0
	bl Motion_CamBounds
	bl Func_02001a24
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
	bl Func_02001b44
	movs r3, #3
	strb r3, [r5]
	movs r0, #24
	bl Battle_WaitMode0
	add r2, sp, #28
	movs r3, #7
	str r3, [r2, #4]
	ldr r3, .L_020097fc
	mov r8, r2
	str r3, [r2, #36]
	movs r3, #163
	lsls r3, r3, #8
	adds r3, #215
	str r3, [r2, #8]
	str r3, [r2, #12]
	movs r3, #0
	mov r10, r3
.L_0200970e:
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
	ldr r3, .L_02009800
	adds r2, r2, r3
	str r2, [r6]
	bl Random16Far
	lsls r3, r0, #1
	ldr r5, [r6, #8]
	adds r3, r3, r0
	ldr r4, .L_02009804
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
	ldr r4, .L_02009808
	str r5, [sp, #4]
	str r4, [sp, #8]
	mov r4, r8
	str r4, [sp, #12]
	bl Func_02000eac
	movs r2, #1
	add r10, r2
	mov r3, r10
	cmp r3, #16
	bls .L_0200970e
	movs r0, #188
	bl Func_02001b44
	ldr r5, .L_020097f8
	movs r4, #133
	lsls r4, r4, #2
	adds r5, r5, r4
	movs r1, #2
	ldr r0, [r5]
	adds r1, #255
	bl Func_02001acc
	ldr r0, [r5]
	movs r1, #49
	bl Object_SetModeById
	movs r0, #160
	movs r1, #160
	movs r2, #128
	lsls r0, r0, #11
	lsls r1, r1, #11
	lsls r2, r2, #9
	bl Func_02001a54
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	adds r2, #102
	negs r0, r0
	negs r1, r1
	bl Func_02001a54
	bl Func_02001a5c
	movs r1, #128
	ldr r0, [r5]
	lsls r1, r1, #1
	bl Func_02001acc
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
	bl Func_02001a84
	add sp, #68
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_020097f8:
	.4byte gPartyState
.L_020097fc:
	.4byte Func_0200161c
.L_02009800:
	.4byte 0xffffa000
.L_02009804:
	.4byte 0xffffd000
.L_02009808:
	.4byte 0x01090001
	.section .text.x0200980c,"ax",%progbits
	.global Func_0200180c
	.thumb_func
Func_0200180c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, .L_020098b4
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	ldr r0, [r3]
	bl Object_GetById
	ldr r3, [r0, #8]
	ldr r6, .L_020098b8
	asrs r3, r3, #20
	mov r8, r3
	ldr r3, [r0, #16]
	adds r5, r6, #0
	asrs r3, r3, #20
	mov r10, r3
	movs r1, #10
	ldrsh r3, [r6, r1]
	movs r7, #0
	adds r5, #32
	ldrh r2, [r6, #10]
	cmp r7, r3
	bge .L_020098a8
.L_02009840:
	movs r1, #0
	ldrsh r3, [r5, r1]
	cmp r3, r8
	bne .L_0200989c
	movs r1, #2
	ldrsh r3, [r5, r1]
	cmp r3, r10
	bne .L_0200989c
	movs r2, #4
	ldrsh r0, [r5, r2]
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02009870
	adds r0, r6, #0
	adds r1, r5, #0
	bl Func_0200146c
	movs r3, #4
	ldrsh r0, [r5, r3]
	bl GameFlag_SetBit
	strh r7, [r6, #12]
	b .L_020098a8
.L_02009870:
	movs r1, #12
	ldrsh r3, [r6, r1]
	cmp r7, r3
	beq .L_020098a8
	adds r0, r6, #0
	adds r1, r5, #0
	strh r7, [r6, #12]
	bl Func_020014dc
	movs r2, #2
	ldrsh r0, [r6, r2]
	mov r1, r8
	bl GameFlag_SetByte
	movs r3, #2
	ldrsh r0, [r6, r3]
	mov r1, r10
	adds r0, #8
	bl GameFlag_SetByte
	movs r0, #1
	b .L_020098aa
.L_0200989c:
	lsls r3, r2, #16
	adds r7, #1
	asrs r3, r3, #16
	adds r5, #8
	cmp r7, r3
	blt .L_02009840
.L_020098a8:
	movs r0, #0
.L_020098aa:
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_020098b4:
	.4byte gPartyState
.L_020098b8:
	.4byte gOverlayArea + 0x2308
	.section .text.x020098bc,"ax",%progbits
	.global Func_020018bc
	.thumb_func
Func_020018bc:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #4
	ldr r3, .L_0200996c
	str r2, [sp, #0]
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	mov r9, r0
	ldr r0, [r3]
	mov r11, r1
	bl Object_GetById
	movs r3, #192
	ldr r5, .L_02009970
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	adds r6, r0, #0
	movs r2, #2
	ldrsh r0, [r5, r2]
	mov r10, r3
	bl GameFlag_GetByte
	adds r7, r0, #0
	movs r3, #2
	ldrsh r0, [r5, r3]
	adds r0, #8
	bl GameFlag_GetByte
	mov r8, r0
	cmp r7, #0
	bne .L_0200990a
	cmp r0, #0
	beq .L_0200995e
.L_0200990a:
	movs r2, #2
	ldrsh r0, [r5, r2]
	movs r1, #0
	bl GameFlag_SetByte
	movs r3, #2
	ldrsh r0, [r5, r3]
	movs r1, #0
	adds r0, #8
	bl GameFlag_SetByte
	mov r3, r9
	adds r2, r7, r3
	mov r3, r8
	movs r1, #128
	add r3, r11
	lsls r1, r1, #12
	lsls r3, r3, #20
	adds r3, r3, r1
	str r3, [r6, #16]
	movs r3, #230
	lsls r3, r3, #1
	add r3, r10
	lsls r2, r2, #20
	adds r2, r2, r1
	ldr r1, [r3]
	str r2, [r6, #8]
	str r2, [r1, #8]
	ldr r3, [r6, #16]
	str r3, [r1, #16]
	bl Func_02001a24
	bl Func_02001674
	movs r2, #192
	lsls r2, r2, #18
	ldr r3, [r2, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	ldr r2, [sp, #0]
	str r2, [r3]
.L_0200995e:
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0200996c:
	.4byte gPartyState
.L_02009970:
	.4byte gOverlayArea + 0x2308
	.section .rodata.x02009b4c,"a",%progbits
	.global Data_02001b4c
Data_02001b4c:
	.4byte 0x06345d01
	.4byte Runtime_ReciprocalTable + 0x1259
	.4byte 0x2f010026
	.4byte 0x5f1f7000
	.4byte 0x667b0906
	.4byte 0x01040901
	.4byte 0x0e287800
	.4byte 0x56053b3f
	.4byte 0x5f6f080e
	.4byte 0x08100800
	.4byte 0xa1076601
	.4byte 0x001d0800
	.4byte Text_MessageContexts + 0x10b38
	.4byte 0x660015bb
	.4byte 0x0e020016
	.4byte 0x66177000
	.4byte 0x3b02080d
	.4byte Data_02001024 + 0xbb
	.4byte 0x66128010
	.4byte 0x01037a01
	.4byte 0x04277910
	.4byte 0xfb44013d
	.4byte Data_0200752c + 0x2d4
	.4byte 0x20590414
	.4byte 0x57052700
	.4byte 0x50002066
	.4byte 0x20ff0016
	.4byte 0x00162a00
	.4byte 0x0016002e
	.4byte 0x162a0020
	.4byte 0x4e002000
	.4byte 0x7f320040
	.4byte 0x04aa023b
	.4byte 0x02500006
	.4byte 0xe7169207
	.4byte 0x0010e103
	.4byte 0x0301ff0d
	.4byte 0x0c01e709
	.4byte 0x5906dd03
	.4byte 0x210a6118
	.4byte 0xfd340030
	.4byte 0x0403a425
	.4byte 0x5f290630
	.4byte 0x02490100
	.4byte 0xf8063b58
	.4byte 0x3b5704af
	.4byte 0x163bb406
	.4byte Resource_DecodeHalfwordLzCode + 0x12
	.4byte 0x283ea00f
	.4byte 0x070820ff
	.4byte 0x2e050010
	.4byte 0x057807f8
	.4byte 0x0a980488
	.4byte 0xdd660ba8
	.4byte 0x07080030
	.4byte 0x38205f22
	.4byte 0x33481706
	.4byte 0x00603b58
	.4byte 0x783ffe78
	.4byte Text_MessageContexts + 0x15ce8
	.4byte 0x1f017900
	.4byte 0x0a782700
	.4byte 0x2aff5f11
	.4byte 0x701f07a8
	.4byte 0xef273000
	.4byte 0x00505704
	.4byte 0x4c002037
	.4byte 0xf7000040
	.4byte 0x0040002a
	.4byte 0x1100206e
	.4byte 0x403b0046
	.4byte 0x00200000
	.4byte 0x2a00402c
	.4byte 0x244819f7
	.4byte 0x2d004000
	.4byte 0x043b0803
	.4byte 0x05830722
	.4byte 0x1817b705
	.4byte 0x0028003b
	.4byte 0x66015920
	.4byte 0x00072260
	.4byte 0x6805041a
	.4byte 0x3f1b01ff
	.4byte 0x06150458
	.4byte 0x40182a2c
	.4byte 0x8f000900
	.4byte 0xe0130302
	.4byte 0x0030cd0c
	.2byte 0x0002
	.global Data_02001cae
Data_02001cae:
	.2byte 0x0000
	.4byte 0xac862b05
	.4byte 0xaf643138
	.4byte 0x426905df
	.4byte 0xabd8ac4e
	.4byte 0xbe810f20
	.4byte 0x7015130e
	.4byte 0x58aaf4f3
	.4byte 0x1c57fed1
	.4byte 0xba72f5fa
	.4byte 0x15f3c78f
	.4byte 0x2c4bddfc
	.4byte 0x8c405c2e
	.4byte 0x5a901050
	.4byte 0x669c3d02
	.4byte 0x813f657c
	.4byte 0xc48af4a0
	.4byte 0x70f27d1e
	.4byte 0xdfcf8f1f
	.4byte 0x0be667d7
	.4byte 0x54be3c30
	.4byte 0xa442f8f4
	.4byte 0xeb17cfc7
	.4byte 0x058022f4
	.4byte 0xc3a1d0c0
	.4byte 0xf05c0a2b
	.4byte 0xbcc4f23e
	.4byte 0xe8ae7760
	.4byte 0x38745730
	.4byte 0xe6259c57
	.4byte 0x0e73008e
	.4byte 0x075f2408
	.4byte 0x1297a050
	.4byte 0x8f028687
	.4byte 0xebc03222
	.4byte 0x3e706883
	.4byte 0x8034e810
	.4byte 0x161c479c
	.4byte 0x9c090038
	.4byte 0x0a75ee0f
	.4byte 0xbf6039f8
	.4byte 0xf613af4e
	.4byte Tileset_Set112TilesA + 0x21e
	.4byte 0x3df8973f
	.4byte 0xf81f363e
	.4byte 0x63e27cd8
	.4byte 0x9df381f3
	.4byte 0xf7e793f3
	.4byte 0xd0e1c9fa
	.4byte 0xf258ce1d
	.4byte 0x3873c780
	.4byte 0xb93be3cc
	.4byte 0x2f3be3cc
	.4byte 0x6118fcc1
	.4byte 0x980e30ca
	.4byte 0x7cef9c33
	.4byte 0x29f1be0c
	.4byte 0xc7c786f0
	.4byte 0x5edecbe6
	.4byte 0x9239f837
	.4byte 0xebc2b9d8
	.4byte 0xd2f88ad8
	.4byte 0xa9bcc731
	.4byte 0x71e99cc7
	.4byte 0x5f547988
	.4byte 0x27049ac1
	.4byte 0xd7ae264f
	.4byte 0xc532a694
	.4byte 0x12fa7ae5
	.4byte 0xc43ef833
	.4byte 0x7d1e54f6
	.4byte 0x97fdb2f9
	.4byte 0xc6a7c3dd
	.4byte 0x3415957d
	.4byte 0x1f1897ad
	.4byte 0xac141210
	.4byte 0x047b4ae7
	.4byte 0x9eb43c03
	.4byte 0xcd180907
	.4byte 0x000079ea
	.4byte 0xf6450423
	.4byte 0xa7d80d99
	.4byte 0x78059b49
	.4byte 0x467a0580
	.4byte 0xb85c205b
	.4byte 0xd050008c
	.4byte 0x500b31ab
	.4byte 0x7833d893
	.4byte 0x8307f302
	.4byte 0x3a9c0502
	.4byte 0x3831102c
	.4byte 0x306a301c
	.4byte 0x3831101d
	.4byte 0x3026301c
	.4byte 0x38f2201a
	.4byte 0x0f2f2e80
	.4byte 0xc0861624
	.4byte 0x1f1ec23c
	.4byte 0x04f95397
	.4byte 0xc067c07c
	.4byte 0xc701f244
	.4byte 0x644c7e42
	.4byte 0x28ae605c
	.4byte 0xa2b8f289
	.4byte Battle_PurpleCaveBackdrop + 0x3607
	.4byte 0xb7fbd5ce
	.4byte 0x8db92233
	.4byte 0xa5310573
	.4byte 0x8e792a73
	.4byte 0x80804174
	.4byte 0x783318f3
	.4byte 0x3905033e
	.4byte 0x7c0bd78f
	.4byte 0x71e9ce5c
	.4byte 0xebc2b80d
	.4byte 0xfe12e605
	.4byte 0x385d04be
	.4byte 0xc06a9047
	.4byte 0x83a09e03
	.4byte 0x1efc23d7
	.4byte 0xf02b869f
	.4byte 0x3002f9f9
	.4byte 0x3f0df1e0
	.4byte 0x793f904f
	.4byte 0xc78f423c
	.4byte 0x3e067c3b
	.4byte 0x3e29ebe7
	.4byte 0xf7f39f16
	.4byte 0x33967f4d
	.4byte 0x780c9067
	.4byte 0x6ce3c57c
	.4byte 0x37ce61c6
	.4byte 0x37cc0e8c
	.4byte 0xcfc6d8fc
	.4byte 0xfcf83ef7
	.4byte 0xe07f9f03
	.4byte 0xfe7c0ff3
	.4byte 0xf03fcf81
	.4byte 0xff3e07f9
	.4byte 0xf816c7c0
	.2byte 0x0000
	.global Data_02001ede
Data_02001ede:
	.2byte 0x0100
	.4byte 0xcf81fe7c
	.4byte 0x07f9f03f
	.4byte 0x6080ff3e
	.4byte 0xead9f7b4
	.4byte 0x1e7cf8d3
	.4byte 0xca9f3dc1
	.4byte 0x8bf7f3e3
	.4byte 0x5b2fc50f
	.4byte 0xf00e1c7d
	.4byte 0xe7c5f8b1
	.4byte 0x4a1f1bef
	.4byte 0x8bf0f3e0
	.4byte 0x3e3dc58f
	.4byte 0xa1f0df7f
	.4byte 0x0a7dbf05
	.4byte 0xf8d3e16e
	.4byte 0x829f7b46
	.4byte 0xe7b8e75e
	.4byte 0x828df1f3
	.4byte 0x5ad8295e
	.4byte 0x3e060bd0
	.4byte 0x49a8e07d
	.4byte 0x6264f270
	.4byte 0x32a6829c
	.4byte 0x127b65c5
	.4byte 0xf6fcf033
	.4byte 0x54f6c43c
	.4byte 0xb2f97d1e
	.4byte 0xc3dd97fd
	.4byte 0x957dc6a7
	.4byte 0x8914e015
	.4byte 0x012101f1
	.4byte 0x166b2b8f
	.4byte 0xf00c11e4
	.4byte 0x241e7ad0
	.4byte 0xe7ab3460
	.4byte 0x108c0001
	.4byte 0x3667d914
	.4byte 0xde0f9f60
	.4byte 0x1b4b2a37
	.4byte 0x2c23c02c
	.4byte 0x02da33d0
	.4byte 0x0465c6e1
	.4byte 0x8d5e8282
	.4byte 0xc49a8459
	.4byte 0x9813c19e
	.4byte 0x2854187f
	.4byte 0x8161d4e0
	.4byte 0x80e1c388
	.4byte 0x80e98751
	.4byte 0x80e1c388
	.4byte 0x00d18131
	.4byte 0x7401c791
	.4byte 0xb1207979
	.4byte 0x7c37dd8f
	.4byte 0x00000000
.L_02009fc0:
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
.L_02009ffc:
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
.L_0200a038:
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
	.global Data_02002074
Data_02002074:
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
	.global Data_020020a4
Data_020020a4:
	.4byte 0x000000fb
	.4byte 0x00137002
	.4byte 0x002020fe
	.4byte 0x003030fe
	.4byte 0x004040fc
	.4byte 0x000001ff
	.global Data_020020bc
Data_020020bc:
	.4byte 0xffff0155
	.4byte 0x00000001
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00026000
	.4byte 0xffff0155
	.4byte 0x00000001
	.4byte 0x01d80000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x01022000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02002104
Data_02002104:
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
	.4byte 0x50008905
	.4byte 0xffff001e
	.4byte Func_02000054
	.4byte 0x50008905
	.4byte 0xffff001f
	.4byte Func_02000074
	.4byte 0x50008905
	.4byte 0xffff0020
	.4byte Func_02000094
	.4byte 0x50008905
	.4byte 0xffff0021
	.4byte Func_020000b4
	.4byte 0x50008905
	.4byte 0xffff0022
	.4byte Func_020000d4
	.4byte 0x50008905
	.4byte 0xffff0023
	.4byte Func_02000184
	.4byte 0x50008905
	.4byte 0xffff0024
	.4byte Func_020000f4
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02002194
Data_02002194:
	.4byte 0xffffffff
	.global Data_02002198
Data_02002198:
	.4byte 0x00000001
	.global Data_0200219c
Data_0200219c:
	.4byte 0x00000027
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x0000000c
	.4byte 0x00000026
	.global Data_020021b0
Data_020021b0:
	.4byte .L_02009fc0
	.4byte .L_02009ffc
	.4byte .L_0200a038
