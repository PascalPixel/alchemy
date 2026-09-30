.syntax unified
	.thumb
	.section .text.x02008038,"ax",%progbits
	.global Func_02000038
	.thumb_func
Func_02000038:
	ldr r0, .L_0200803c
	bx lr
.L_0200803c:
	.4byte Data_020021d0
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
	.4byte Data_02002200
	.section .text.x0200804c,"ax",%progbits
	.global Func_0200004c
	.thumb_func
Func_0200004c:
	ldr r0, .L_02008050
	bx lr
.L_02008050:
	.4byte Data_02002230
	.section .text.x02008054,"ax",%progbits
	.global Func_02000054
	.thumb_func
Func_02000054:
	push {lr}
	ldr r3, .L_02008078
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #18
	adds r3, r3, r2
	ldrb r3, [r3]
	cmp r3, #10
	bne .L_02008076
	movs r0, #136
	movs r1, #136
	lsls r0, r0, #18
	lsls r1, r1, #17
	movs r2, #0
	movs r3, #255
	bl Func_02001c90
.L_02008076:
	pop {pc}
.L_02008078:
	.4byte gPartyState
	.section .text.x0200807c,"ax",%progbits
	.global Func_0200007c
	.thumb_func
Func_0200007c:
	push {lr}
	movs r0, #136
	movs r1, #136
	lsls r1, r1, #17
	movs r2, #0
	movs r3, #6
	lsls r0, r0, #18
	bl Func_02001c90
	pop {pc}
	.section .text.x02008090,"ax",%progbits
	.global Func_02000090
	.thumb_func
Func_02000090:
	push {r5, r6, r7, lr}
	ldr r3, .L_020080e4
	adds r7, r0, #0
	ldr r6, [r3]
	cmp r6, #0
	bne .L_020080e2
	movs r0, #8
	bl Object_GetById
	adds r5, r0, #0
	cmp r7, #30
	bne .L_020080c0
	movs r1, #148
	movs r2, #150
	movs r0, #8
	lsls r1, r1, #17
	lsls r2, r2, #18
	bl Func_02001c10
	movs r3, #128
	lsls r3, r3, #8
	str r6, [r5, #12]
	strh r3, [r5, #6]
	b .L_020080d6
.L_020080c0:
	cmp r7, #31
	bne .L_020080d6
	movs r1, #136
	movs r2, #170
	movs r0, #8
	lsls r1, r1, #16
	lsls r2, r2, #18
	bl Func_02001c10
	str r6, [r5, #12]
	strh r6, [r5, #6]
.L_020080d6:
	adds r0, r5, #0
	bl Func_02000910
	ldr r2, .L_020080e4
	movs r3, #1
	str r3, [r2]
.L_020080e2:
	pop {r5, r6, r7, pc}
.L_020080e4:
	.4byte gOverlayArea + 0x2618
	.section .text.x020080e8,"ax",%progbits
	.global Func_020000e8
	.thumb_func
Func_020000e8:
	push {r5, r6, r7, lr}
	ldr r3, .L_02008184
	adds r6, r0, #0
	ldr r7, [r3]
	cmp r7, #0
	bne .L_02008180
	ldr r3, .L_02008188
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	ldr r3, [r0, #12]
	cmp r3, #0
	ble .L_02008180
	movs r0, #8
	bl Object_GetById
	adds r5, r0, #0
	cmp r6, #50
	bne .L_02008120
	movs r1, #244
	movs r2, #140
	movs r0, #8
	lsls r1, r1, #17
	lsls r2, r2, #17
	b .L_0200812e
.L_02008120:
	cmp r6, #51
	bne .L_02008140
	movs r1, #236
	movs r2, #184
	movs r0, #8
	lsls r1, r1, #17
	lsls r2, r2, #16
.L_0200812e:
	bl Func_02001c10
	movs r3, #128
	lsls r3, r3, #14
	str r3, [r5, #12]
	movs r3, #128
	lsls r3, r3, #8
	strh r3, [r5, #6]
	b .L_0200816a
.L_02008140:
	cmp r6, #52
	bne .L_02008150
	movs r1, #154
	movs r2, #140
	movs r0, #8
	lsls r1, r1, #18
	lsls r2, r2, #17
	b .L_0200815e
.L_02008150:
	cmp r6, #53
	bne .L_0200816a
	movs r1, #158
	movs r2, #184
	movs r0, #8
	lsls r1, r1, #18
	lsls r2, r2, #16
.L_0200815e:
	bl Func_02001c10
	movs r3, #128
	lsls r3, r3, #14
	str r3, [r5, #12]
	strh r7, [r5, #6]
.L_0200816a:
	adds r0, r5, #0
	bl Func_02000910
	cmp r0, #0
	beq .L_0200817a
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r0, #48]
.L_0200817a:
	ldr r2, .L_02008184
	movs r3, #1
	str r3, [r2]
.L_02008180:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008184:
	.4byte gOverlayArea + 0x2618
.L_02008188:
	.4byte gPartyState
	.section .text.x0200818c,"ax",%progbits
	.global Func_0200018c
	.thumb_func
Func_0200018c:
	push {r5, lr}
	ldr r3, .L_020081b8
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	adds r5, r0, #0
	ldr r0, [r3]
	bl Object_GetById
	ldr r3, [r0, #12]
	cmp r3, #0
	bne .L_020081b4
	adds r0, r5, #0
	bl Func_02001978
	cmp r0, #0
	beq .L_020081b4
	movs r0, #13
	bl Func_02001c50
.L_020081b4:
	pop {r5, pc}
	.2byte 0x0000
.L_020081b8:
	.4byte gPartyState
	.section .text.x020081bc,"ax",%progbits
	.global Func_020001bc
	.thumb_func
Func_020001bc:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r3, .L_02008224
	movs r2, #133
	lsls r2, r2, #2
	adds r7, r3, r2
	mov r8, r0
	ldr r0, [r7]
	bl Object_GetById
	ldr r3, [r0, #12]
	cmp r3, #0
	bne .L_0200821e
	ldr r0, [r7]
	bl Object_GetById
	ldr r3, [r0, #8]
	asrs r6, r3, #20
	ldr r3, [r0, #16]
	asrs r5, r3, #20
	cmp r6, #18
	bne .L_020081f6
	cmp r5, #34
	bne .L_020081f6
	ldr r0, [r7]
	movs r1, #3
	bl ObjectMotion_SetActionVariant
.L_020081f6:
	cmp r6, #8
	bne .L_0200820e
	cmp r5, #39
	bne .L_0200820e
	ldr r3, .L_02008224
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	movs r1, #3
	bl ObjectMotion_SetActionVariant
.L_0200820e:
	mov r0, r8
	bl Func_02001978
	cmp r0, #0
	beq .L_0200821e
	movs r0, #14
	bl Func_02001c50
.L_0200821e:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_02008224:
	.4byte gPartyState
	.section .text.x02008228,"ax",%progbits
	.global Func_02000228
	.thumb_func
Func_02000228:
	push {r5, r6, lr}
	ldr r3, .L_02008284
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	adds r5, r0, #0
	ldr r0, [r3]
	bl Object_GetById
	ldr r3, [r0, #12]
	cmp r3, #0
	bgt .L_02008248
	adds r0, r5, #0
	bl Func_0200018c
	b .L_02008280
.L_02008248:
	ldr r3, [r0, #8]
	asrs r6, r3, #20
	ldr r3, [r0, #16]
	asrs r5, r3, #20
	cmp r6, #25
	bne .L_02008268
	cmp r5, #17
	bne .L_0200825e
	movs r0, #50
	bl Func_020000e8
.L_0200825e:
	cmp r5, #11
	bne .L_02008268
	movs r0, #51
	bl Func_020000e8
.L_02008268:
	cmp r6, #43
	bne .L_02008280
	cmp r5, #17
	bne .L_02008276
	movs r0, #52
	bl Func_020000e8
.L_02008276:
	cmp r5, #11
	bne .L_02008280
	movs r0, #53
	bl Func_020000e8
.L_02008280:
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02008284:
	.4byte gPartyState
	.section .text.x02008288,"ax",%progbits
	.global Func_02000288
	.thumb_func
Func_02000288:
	ldr r0, .L_0200828c
	bx lr
.L_0200828c:
	.4byte Data_020024a0
	.section .text.x02008290,"ax",%progbits
	.global Func_02000290
	.thumb_func
Func_02000290:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r1, #214
	lsls r1, r1, #1
	movs r6, #129
	adds r3, r3, r1
	lsls r6, r6, #2
	movs r1, #3
	str r6, [r3]
	movs r0, #64
	sub sp, #4
	bl ObjectMotion_SetActionVariant
	ldr r3, .L_02008350
	movs r5, #0
	str r5, [r3]
	movs r0, #8
	bl Object_GetById
	ldr r3, .L_02008354
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	ldrh r2, [r3]
	movs r1, #128
	subs r3, r2, #6
	adds r0, #85
	lsls r3, r3, #16
	lsls r1, r1, #9
	strb r5, [r0]
	cmp r3, r1
	bhi .L_020082f4
	movs r3, #1
	movs r1, #160
	movs r2, #224
	lsls r1, r1, #2
	lsls r2, r2, #2
	str r3, [sp, #0]
	movs r0, #33
	movs r3, #48
	bl Func_0200130c
	movs r0, #0
	movs r1, #0
	adds r2, r6, #0
	bl Func_02001a28
	b .L_02008348
.L_020082f4:
	lsls r3, r2, #16
	asrs r3, r3, #16
	cmp r3, #3
	beq .L_02008304
	cmp r3, #5
	beq .L_02008304
	cmp r3, #13
	bne .L_02008328
.L_02008304:
	movs r3, #1
	movs r1, #160
	movs r2, #224
	lsls r1, r1, #2
	lsls r2, r2, #2
	str r3, [sp, #0]
	movs r0, #43
	movs r3, #49
	bl Func_0200130c
	movs r0, #21
	movs r2, #129
	negs r0, r0
	lsls r2, r2, #2
	movs r1, #32
	bl Func_02001a28
	b .L_02008348
.L_02008328:
	movs r1, #160
	movs r2, #224
	lsls r1, r1, #2
	lsls r2, r2, #2
	movs r0, #43
	movs r3, #0
	str r5, [sp, #0]
	bl Func_0200130c
	movs r0, #2
	movs r1, #27
	negs r0, r0
	negs r1, r1
	adds r2, r6, #0
	bl Func_02001a28
.L_02008348:
	movs r0, #0
	add sp, #4
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02008350:
	.4byte gOverlayArea + 0x2618
.L_02008354:
	.4byte gPartyState
	.section .text.x02008358,"ax",%progbits
	.global Func_02000358
	.thumb_func
Func_02000358:
	push {lr}
	movs r0, #0
	bl Func_0200036c
	movs r0, #0
	bl Func_02000460
	movs r0, #0
	pop {pc}
	.2byte 0x0000
	.section .text.x0200836c,"ax",%progbits
	.global Func_0200036c
	.thumb_func
Func_0200036c:
	push {lr}
	movs r0, #20
	adds r0, #255
	bl GameFlag_SetBit
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #37
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008394
	movs r0, #98
	adds r0, #255
	bl GameFlag_SetBit
	movs r0, #162
	lsls r0, r0, #1
	bl GameFlag_SetBit
.L_02008394:
	pop {pc}
	.2byte 0x0000
	.section .text.x02008398,"ax",%progbits
	.global Func_02000398
	.thumb_func
Func_02000398:
	ldr r3, .L_020083a0
	str r0, [r3]
	bx lr
	.2byte 0x0000
.L_020083a0:
	.4byte Data_020025f4
	.section .text.x020083a4,"ax",%progbits
	.global Func_020003a4
	.thumb_func
Func_020003a4:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r3, .L_02008450
	sub sp, #32
	ldr r0, [r3]
	cmp r0, #0
	bge .L_020083b6
	adds r0, #3
.L_020083b6:
	asrs r0, r0, #2
	movs r1, #5
	bl Engine_MathRemainder
	ldr r3, .L_02008454
	mov r8, r0
	ldr r3, [r3]
	cmp r3, #0
	beq .L_0200840a
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
	beq .L_020083ea
	movs r3, #192
	lsls r3, r3, #2
	adds r3, #255
	ands r3, r1
	cmp r3, #153
	bne .L_02008446
.L_020083ea:
	movs r1, #192
	lsls r1, r1, #4
	adds r1, #164
	adds r3, r2, r1
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	bne .L_02008446
	movs r0, #175
	lsls r0, r0, #1
	adds r3, r2, r0
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	bne .L_02008446
.L_0200840a:
	movs r5, #0
	movs r6, #4
.L_0200840e:
	mov r2, r8
	adds r0, r2, r5
	movs r1, #5
	mov r7, sp
	bl Engine_MathRemainder
	ldr r3, .L_02008458
	lsls r0, r0, #1
	ldrh r3, [r3, r6]
	adds r5, #1
	strh r3, [r7, r0]
	adds r6, #2
	cmp r5, #4
	ble .L_0200840e
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	adds r0, r7, #0
	ldr r1, .L_0200845c
	adds r2, #2
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r2, .L_02008450
	ldr r3, [r2]
	adds r3, #1
	str r3, [r2]
.L_02008446:
	add sp, #32
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008450:
	.4byte Data_020025f0
.L_02008454:
	.4byte Data_020025f4
.L_02008458:
	.4byte gOverlayArea + 0x261c
.L_0200845c:
	.4byte 0x05000184
	.section .text.x02008460,"ax",%progbits
	.global Func_02000460
	.thumb_func
Func_02000460:
	push {r5, r6, lr}
	ldr r2, .L_020084bc
	movs r3, #1
	adds r6, r0, #0
	str r3, [r2]
	cmp r6, #2
	beq .L_02008484
	ldr r1, .L_020084c0
	movs r2, #32
	ldr r0, .L_020084c4
	ldr r5, .L_020084c8
	mov lr, r5
	.2byte 0xf800
	ldr r0, .L_020084cc
	ldr r1, .L_020084d0
	movs r2, #32
	mov lr, r5
	.2byte 0xf800
.L_02008484:
	movs r0, #160
	lsls r0, r0, #4
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008494
	cmp r6, #1
	bne .L_020084a6
.L_02008494:
	ldr r3, .L_020084d4
	movs r2, #0
	movs r1, #144
	str r2, [r3]
	ldr r0, .L_020084d8
	lsls r1, r1, #3
	bl Scheduler_AddOrUpdateCallback
	b .L_020084ba
.L_020084a6:
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	ldr r0, .L_020084dc
	ldr r1, .L_020084e0
	adds r2, #2
	stmia r3!, {r0, r1, r2}
	subs r3, #12
.L_020084ba:
	pop {r5, r6, pc}
.L_020084bc:
	.4byte Data_020025f4
.L_020084c0:
	.4byte 0x05000180
.L_020084c4:
	.4byte gOverlayArea + 0x261c
.L_020084c8:
	.4byte IwramCopyWords
.L_020084cc:
	.4byte gOverlayArea + 0x263c
.L_020084d0:
	.4byte 0x050001a0
.L_020084d4:
	.4byte Data_020025f0
.L_020084d8:
	.4byte Func_020003a4
.L_020084dc:
	.4byte gOverlayArea + 0x2640
.L_020084e0:
	.4byte 0x05000184
	.section .text.x020084e4,"ax",%progbits
	.global Func_020004e4
	.thumb_func
Func_020004e4:
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
	bl Func_02001bd8
	ldr r0, [r5, #8]
	ldr r1, [r5, #16]
	movs r2, #0
	movs r3, #128
	bl Func_02001c98
	pop {r5, pc}
	.section .text.x02008538,"ax",%progbits
	.global Func_02000538
	.thumb_func
Func_02000538:
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
	ldr r3, .L_02008644
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	adds r6, r0, #0
	ldr r7, [r6, #104]
	bl Func_02001be8
	movs r0, #0
	bl Func_02001c70
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
	beq .L_020085d8
.L_02008592:
	ldr r3, [r7, #8]
	ldr r2, .L_02008648
	str r3, [r6, #8]
	ldr r3, [r7, #12]
	adds r3, r3, r5
	str r3, [r6, #12]
	ldr r3, [r7, #16]
	str r3, [r6, #16]
	cmp r5, r2
	bgt .L_020085ae
	movs r3, #200
	lsls r3, r3, #5
	adds r3, #153
	adds r5, r5, r3
.L_020085ae:
	ldr r3, .L_0200864c
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
	bne .L_02008592
.L_020085d8:
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
	ldr r0, .L_02008640
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
	b .L_02008650
.L_02008640:
	.4byte 0x00000001
.L_02008644:
	.4byte gPartyState
.L_02008648:
	.4byte 0x0003ffff
.L_0200864c:
	.4byte Data_0300122c
.L_02008650:
	bl Motion_CamBounds
	bl Func_02001c48
	movs r3, #128
	adds r7, r0, #0
	lsls r3, r3, #12
	str r3, [r7, #48]
	movs r3, #128
	ldr r5, .L_02008698
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
	bl Func_02001b98
	ldr r3, [r6, #20]
	movs r2, #128
	lsls r2, r2, #13
	adds r3, r3, r2
	ldr r2, [r6, #12]
	movs r5, #0
	cmp r2, r3
	ble .L_020086b6
	b .L_0200869c
	.2byte 0x0000
.L_02008698:
	.4byte 0x00000000
.L_0200869c:
	movs r0, #1
	adds r5, #1
	bl WaitFrames
	cmp r5, #59
	bgt .L_020086b6
	ldr r3, [r6, #20]
	movs r2, #128
	lsls r2, r2, #13
	adds r3, r3, r2
	ldr r2, [r6, #12]
	cmp r2, r3
	bgt .L_0200869c
.L_020086b6:
	movs r0, #127
	bl Func_02001ca0
	ldr r3, [r6, #40]
	movs r5, #0
	cmp r3, #0
	beq .L_020086d6
.L_020086c4:
	movs r0, #1
	adds r5, #1
	bl WaitFrames
	cmp r5, #59
	bgt .L_020086d6
	ldr r3, [r6, #40]
	cmp r3, #0
	bne .L_020086c4
.L_020086d6:
	adds r0, r7, #0
	bl Func_02001ba0
	ldr r5, .L_02008720
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
	bl Func_02001bf0
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_02008720:
	.4byte gPartyState
	.section .text.x02008724,"ax",%progbits
	.global Func_02000724
	.thumb_func
Func_02000724:
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
	ldr r3, .L_0200874c
	mov lr, r3
	.2byte 0xf800
	pop {pc}
	.2byte 0x0000
.L_0200874c:
	.4byte IwramFillWords + 0x74
	.section .text.x02008750,"ax",%progbits
	.global Func_02000750
	.thumb_func
Func_02000750:
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
	ldr r3, .L_020087b8
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
	bne .L_020087ae
	movs r3, #173
	lsls r3, r3, #1
	add r3, r10
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	bne .L_020087ae
	movs r3, #175
	lsls r3, r3, #1
	add r3, r10
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	bne .L_020087ae
	movs r3, #180
	lsls r3, r3, #1
	add r3, r10
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	beq .L_020087bc
.L_020087ae:
	adds r0, r6, #0
	movs r1, #0
	bl ObjectDispatch_ApplyValueToChildren
	b .L_020088fa
.L_020087b8:
	.4byte gPartyState
.L_020087bc:
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
	bne .L_020087dc
	movs r0, #231
	bl Func_02001ca0
.L_020087dc:
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
	bl Func_02001c88
	cmp r0, #255
	beq .L_020088de
	ldr r3, [r6, #8]
	mov r5, sp
	str r3, [r5]
	adds r0, r5, #0
	ldr r3, [r6, #12]
	str r3, [r5, #4]
	ldr r3, [r6, #16]
	str r3, [r5, #8]
	bl Func_02001c78
	ldr r5, [r5]
	movs r3, #136
	lsls r3, r3, #17
	cmp r5, r3
	bgt .L_020088de
	ldr r2, .L_020088cc
	cmp r5, r2
	blt .L_020088de
	movs r3, #98
	adds r3, r3, r6
	mov r9, r3
	ldrb r3, [r3]
	cmp r3, #0
	bne .L_020088a4
	ldr r2, [r7, #12]
	ldr r3, [r6, #12]
	subs r5, r2, r3
	cmp r5, #0
	bge .L_0200883c
	subs r5, r3, r2
.L_0200883c:
	adds r0, r7, #0
	adds r1, r6, #0
	movs r2, #0
	adds r0, #8
	adds r1, #8
	mov r8, r2
	bl Func_02000724
	cmp r0, #12
	bgt .L_0200885c
	movs r3, #192
	lsls r3, r3, #12
	cmp r5, r3
	bge .L_0200885c
	movs r2, #1
	mov r8, r2
.L_0200885c:
	mov r3, r8
	cmp r3, #0
	beq .L_020088a4
	movs r0, #130
	lsls r0, r0, #1
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020088a4
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
	ldr r3, .L_020088d0
	movs r2, #128
	ldr r0, .L_020088c8
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
.L_020088a4:
	ldrh r0, [r6, #6]
	bl Math_Cosine
	ldr r1, [r6, #48]
	ldr r5, .L_020088d4
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
	b .L_020088d8
.L_020088c8:
	.4byte 0x00000000
.L_020088cc:
	.4byte 0xffe00000
.L_020088d0:
	.4byte gPartyState
.L_020088d4:
	.4byte IwramMulQ16
.L_020088d8:
	adds r3, r3, r0
	str r3, [r6, #16]
	b .L_020088fa
.L_020088de:
	adds r3, r6, #0
	adds r3, #99
	movs r5, #0
	strb r5, [r3]
	ldr r1, .L_02008908
	adds r0, r6, #0
	str r5, [r6, #108]
	bl Func_02001b78
	movs r0, #228
	bl Func_02001ca0
	ldr r3, .L_0200890c
	str r5, [r3]
.L_020088fa:
	add sp, #12
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008908:
	.4byte Data_020025f8
.L_0200890c:
	.4byte gOverlayArea + 0x2618
	.section .text.x02008910,"ax",%progbits
	.global Func_02000910
	.thumb_func
Func_02000910:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r5, r0, #0
	movs r0, #222
	sub sp, #68
	bl Func_02001ca0
	ldrh r0, [r5, #6]
	bl Math_Cosine
	adds r1, r0, #0
	movs r0, #128
	ldr r6, .L_020089c4
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
	bl Func_02001b80
	movs r1, #2
	adds r7, r0, #0
	bl Func_02001b68
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
	ldr r2, .L_020089c0
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
	ldr r3, .L_020089c8
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
	ldr r3, .L_020089cc
	ldr r1, [r5, #12]
	adds r2, r2, r3
	movs r3, #192
	lsls r3, r3, #10
	str r3, [sp, #8]
	b .L_020089d0
.L_020089c0:
	.4byte 0x00000000
.L_020089c4:
	.4byte IwramMulQ16
.L_020089c8:
	.4byte Func_02000750
.L_020089cc:
	.4byte 0xfffa0000
.L_020089d0:
	movs r3, #0
	str r6, [sp, #0]
	str r6, [sp, #4]
	str r4, [sp, #12]
	bl Func_02001018
	adds r0, r7, #0
	add sp, #68
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x020089e8,"ax",%progbits
	.global Func_020009e8
	.thumb_func
Func_020009e8:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, .L_02008a84
	sub sp, #56
	ldr r7, [r3]
	movs r3, #7
	ands r7, r3
	mov r10, r0
	cmp r7, #0
	bne .L_02008a76
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
	bl Func_02001018
.L_02008a76:
	movs r0, #0
	add sp, #56
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008a84:
	.4byte Data_0300122c
	.section .text.x02008a88,"ax",%progbits
	.global Func_02000a88
	.thumb_func
Func_02000a88:
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
	bl Func_02001c10
	movs r1, #128
	movs r2, #248
	movs r0, #23
	lsls r1, r1, #18
	lsls r2, r2, #16
	bl Func_02001c10
	movs r1, #236
	movs r2, #128
	movs r0, #9
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_02001c10
	movs r1, #138
	movs r2, #128
	lsls r2, r2, #17
	movs r0, #10
	lsls r1, r1, #18
	bl Func_02001c10
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
	beq .L_02008b64
	ldr r3, [r6, #12]
	ldr r2, .L_02008bfc
	movs r0, #9
	adds r3, r3, r2
	str r3, [r6, #12]
	ldr r2, .L_02008c00
	ldr r3, [r5, #12]
	adds r3, r3, r2
	str r3, [r5, #12]
	ldr r2, .L_02008c04
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
.L_02008b64:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #11
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008bae
	ldr r3, [r7, #12]
	ldr r2, .L_02008bfc
	movs r0, #10
	adds r3, r3, r2
	str r3, [r7, #12]
	ldr r2, .L_02008c00
	ldr r3, [r5, #12]
	adds r3, r3, r2
	str r3, [r5, #12]
	ldr r2, .L_02008c08
	ldr r3, [r5, #8]
	adds r3, r3, r2
	str r3, [r5, #8]
	mov r2, r8
	ldrh r3, [r2, #18]
	ldr r2, .L_02008c0c
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
.L_02008bae:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #10
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008bee
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #11
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008bee
	ldr r3, [r6, #12]
	ldr r2, .L_02008c10
	adds r3, r3, r2
	str r3, [r6, #12]
	ldr r3, [r7, #12]
	adds r3, r3, r2
	str r3, [r7, #12]
	ldr r2, .L_02008c14
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
.L_02008bee:
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008bfc:
	.4byte 0x00066640
.L_02008c00:
	.4byte 0x0001eb80
.L_02008c04:
	.4byte 0xfffd70c0
.L_02008c08:
	.4byte 0x00028f40
.L_02008c0c:
	.4byte 0xfffff800
.L_02008c10:
	.4byte 0x00199900
.L_02008c14:
	.4byte 0x001b8480
	.section .text.x02008c18,"ax",%progbits
	.global Func_02000c18
	.thumb_func
Func_02000c18:
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
	ldr r3, .L_02008dac
	adds r2, r1, #0
	adds r2, #228
	ldr r0, [r2]
	ldr r2, [r2, #4]
	ands r0, r3
	ands r2, r3
	ldr r3, .L_02008db0
	mov r10, r0
	movs r0, #0
	ldrsh r3, [r3, r0]
	mov r8, r2
	ldr r2, .L_02008db4
	lsls r3, r3, #2
	adds r3, r3, r2
	ldrh r3, [r3, #2]
	sub sp, #8
	lsrs r3, r3, #5
	str r3, [sp, #4]
	ldr r6, .L_02008db8
	ldr r3, [r1]
	movs r1, #0
	ldr r3, [r3, #4]
	mov r9, r1
	str r3, [sp, #0]
	ldr r3, .L_02008dbc
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r9, r3
	blt .L_02008c6a
	b .L_02008d9e
.L_02008c6a:
	ldr r2, .L_02008dc0
	mov r0, r9
	lsls r3, r0, #2
	ldr r5, [r2, r3]
	cmp r5, #0
	bne .L_02008c78
	b .L_02008d8e
.L_02008c78:
	ldr r3, [r5, #8]
	cmp r3, #0
	bne .L_02008c80
	b .L_02008d8e
.L_02008c80:
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
	ldr r3, .L_02008dc4
	movs r1, #0
	ldrsh r2, [r3, r1]
	adds r3, r5, #0
	mov r12, r2
	asrs r1, r0, #16
	mov r0, r12
	adds r3, #100
	asrs r2, r4, #16
	cmp r0, #0
	bne .L_02008cf6
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
	bhi .L_02008d8e
	movs r2, #16
	negs r2, r2
	cmp r4, r2
	ble .L_02008d8e
	cmp r4, #239
	bgt .L_02008d8e
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
	ldr r3, .L_02008dc8
	b .L_02008d32
.L_02008cf6:
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
	bhi .L_02008d8e
	movs r2, #64
	negs r2, r2
	cmp r4, r2
	ble .L_02008d8e
	cmp r4, #175
	bgt .L_02008d8e
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
	ldr r3, .L_02008dcc
.L_02008d32:
	movs r2, #128
	orrs r4, r3
	stmia r1!, {r4}
	ldr r0, [sp, #4]
	lsls r3, r7, #3
	adds r3, r0, r3
	lsls r2, r2, #4
	orrs r3, r2
	str r3, [r1]
	ldr r3, .L_02008dd0
	movs r0, #1
	ldrh r2, [r3]
	movs r1, #0
	ldrsh r3, [r3, r1]
	negs r0, r0
	cmp r3, r0
	bne .L_02008d70
	adds r0, r5, #0
	bl Func_02001c80
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
	b .L_02008d84
.L_02008d70:
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
.L_02008d84:
	adds r0, r6, #0
	mov r1, r11
	bl Func_02001b40
	adds r6, #12
.L_02008d8e:
	ldr r3, .L_02008dbc
	movs r1, #1
	movs r2, #0
	ldrsh r3, [r3, r2]
	add r9, r1
	cmp r9, r3
	bge .L_02008d9e
	b .L_02008c6a
.L_02008d9e:
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_02008dac:
	.4byte 0xffff0000
.L_02008db0:
	.4byte gOverlayArea + 0x265c
.L_02008db4:
	.4byte ResourceTableEntries
.L_02008db8:
	.4byte gOverlayArea + 0x26a0
.L_02008dbc:
	.4byte gOverlayArea + 0x265e
.L_02008dc0:
	.4byte gOverlayArea + 0x2660
.L_02008dc4:
	.4byte gOverlayArea + 0x2760
.L_02008dc8:
	.4byte 0x40002000
.L_02008dcc:
	.4byte 0xc000a000
.L_02008dd0:
	.4byte gOverlayArea + 0x2762
	.section .text.x02008dd4,"ax",%progbits
	.global Func_02000dd4
	.thumb_func
Func_02000dd4:
	push {r5, r6, lr}
	movs r0, #192
	lsls r0, r0, #4
	bl Runtime_BumpAllocate
	ldr r3, .L_02008e34
	adds r6, r0, #0
	movs r1, #64
	ldr r0, .L_02008e38
	mov lr, r3
	.2byte 0xf800
	adds r1, r6, #0
	ldr r0, .L_02008e3c
	bl Func_02001b28
	ldr r5, .L_02008e40
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
	ldr r0, .L_02008e44
	bl Scheduler_AddOrUpdateCallback
	ldr r3, .L_02008e48
	ldr r2, .L_02008e2c
	strh r2, [r3]
	ldr r3, .L_02008e4c
	strh r2, [r3]
	ldr r2, .L_02008e50
	ldr r3, .L_02008e30
	strh r3, [r2]
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02008e2c:
	.4byte 0x00000000
.L_02008e30:
	.4byte 0xffffffff
.L_02008e34:
	.4byte IwramClearWords
.L_02008e38:
	.4byte gOverlayArea + 0x2660
.L_02008e3c:
	.4byte Data_02001ca8
.L_02008e40:
	.4byte gOverlayArea + 0x265c
.L_02008e44:
	.4byte Func_02000c18
.L_02008e48:
	.4byte gOverlayArea + 0x265e
.L_02008e4c:
	.4byte gOverlayArea + 0x2760
.L_02008e50:
	.4byte gOverlayArea + 0x2762
	.section .text.x02008e54,"ax",%progbits
	.global Func_02000e54
	.thumb_func
Func_02000e54:
	push {r5, r6, lr}
	movs r0, #192
	lsls r0, r0, #4
	bl Runtime_BumpAllocate
	ldr r3, .L_02008eb4
	adds r6, r0, #0
	movs r1, #64
	ldr r0, .L_02008eb8
	mov lr, r3
	.2byte 0xf800
	adds r1, r6, #0
	ldr r0, .L_02008ebc
	bl Func_02001b28
	ldr r5, .L_02008ec0
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
	ldr r0, .L_02008ec4
	bl Scheduler_AddOrUpdateCallback
	ldr r3, .L_02008ec8
	ldr r2, .L_02008eac
	strh r2, [r3]
	ldr r3, .L_02008ecc
	strh r2, [r3]
	ldr r2, .L_02008ed0
	ldr r3, .L_02008eb0
	strh r3, [r2]
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02008eac:
	.4byte 0x00000000
.L_02008eb0:
	.4byte 0xffffffff
.L_02008eb4:
	.4byte IwramClearWords
.L_02008eb8:
	.4byte gOverlayArea + 0x2660
.L_02008ebc:
	.4byte Data_02001e0a + 0x1
.L_02008ec0:
	.4byte gOverlayArea + 0x265c
.L_02008ec4:
	.4byte Func_02000c18
.L_02008ec8:
	.4byte gOverlayArea + 0x265e
.L_02008ecc:
	.4byte gOverlayArea + 0x2760
.L_02008ed0:
	.4byte gOverlayArea + 0x2762
	.section .text.x02008ed4,"ax",%progbits
	.global Func_02000ed4
	.thumb_func
Func_02000ed4:
	push {r5, r6, lr}
	movs r0, #128
	lsls r0, r0, #4
	bl Runtime_BumpAllocate
	ldr r3, .L_02008f38
	adds r6, r0, #0
	movs r1, #64
	ldr r0, .L_02008f3c
	mov lr, r3
	.2byte 0xf800
	adds r1, r6, #0
	ldr r0, .L_02008f40
	bl Func_02001b28
	ldr r5, .L_02008f44
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
	ldr r0, .L_02008f48
	bl Scheduler_AddOrUpdateCallback
	ldr r2, .L_02008f4c
	ldr r3, .L_02008f2c
	strh r3, [r2]
	ldr r2, .L_02008f50
	ldr r3, .L_02008f30
	strh r3, [r2]
	ldr r2, .L_02008f54
	ldr r3, .L_02008f34
	strh r3, [r2]
	b .L_02008f58
.L_02008f2c:
	.4byte 0x00000000
.L_02008f30:
	.4byte 0x00000001
.L_02008f34:
	.4byte 0xffffffff
.L_02008f38:
	.4byte IwramClearWords
.L_02008f3c:
	.4byte gOverlayArea + 0x2660
.L_02008f40:
	.4byte Data_0200203a
.L_02008f44:
	.4byte gOverlayArea + 0x265c
.L_02008f48:
	.4byte Func_02000c18
.L_02008f4c:
	.4byte gOverlayArea + 0x265e
.L_02008f50:
	.4byte gOverlayArea + 0x2760
.L_02008f54:
	.4byte gOverlayArea + 0x2762
.L_02008f58:
	pop {r5, r6, pc}
	.2byte 0x0000
	.section .text.x02008f5c,"ax",%progbits
	.global Func_02000f5c
	.thumb_func
Func_02000f5c:
	push {r5, lr}
	adds r5, r1, #0
	bl Object_GetById
	adds r4, r0, #0
	cmp r4, #0
	beq .L_02008f82
	adds r3, r4, #0
	adds r3, #100
	strh r5, [r3]
	ldr r1, .L_02008f84
	ldr r0, .L_02008f88
	ldrh r2, [r1]
	movs r5, #0
	ldrsh r3, [r1, r5]
	adds r2, #1
	lsls r3, r3, #2
	str r4, [r0, r3]
	strh r2, [r1]
.L_02008f82:
	pop {r5, pc}
.L_02008f84:
	.4byte gOverlayArea + 0x265e
.L_02008f88:
	.4byte gOverlayArea + 0x2660
	.section .text.x02008f8c,"ax",%progbits
	.global Func_02000f8c
	.thumb_func
Func_02000f8c:
	ldr r3, .L_02008f94
	strh r0, [r3]
	bx lr
	.2byte 0x0000
.L_02008f94:
	.4byte gOverlayArea + 0x2762
	.section .text.x02008fde,"ax",%progbits
	.2byte 0x0000
	.section .text.x02008fe0,"ax",%progbits
	.global Func_02000fe0
	.thumb_func
Func_02000fe0:
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
	.section .text.x02009018,"ax",%progbits
	.global Func_02001018
	.thumb_func
Func_02001018:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	mov r11, r3
	ldr r3, .L_020091d0
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
	beq .L_02009060
	cmp r7, #0
	beq .L_02009060
	movs r2, #24
	ldrsh r0, [r7, r2]
	adds r1, r5, #0
	adds r2, r6, #0
	b .L_02009068
.L_02009060:
	movs r0, #30
	adds r2, r6, #0
	adds r0, #255
	adds r1, r5, #0
.L_02009068:
	mov r3, r10
	bl Func_02001b80
	adds r6, r0, #0
	cmp r6, #0
	bne .L_02009076
	b .L_020091c2
.L_02009076:
	ldr r3, [r6, #80]
	mov r1, r8
	movs r5, #15
	adds r1, #1
	ands r1, r5
	adds r0, r6, #0
	str r3, [sp, #0]
	bl Func_02001b68
	ldr r2, .L_020091d4
	mov r3, r8
	ands r3, r5
	lsls r3, r3, #2
	ldr r1, [r2, r3]
	adds r0, r6, #0
	mov r10, r3
	bl Func_02001b78
	adds r3, r6, #0
	movs r5, #0
	adds r3, #85
	strb r5, [r3]
	adds r0, r6, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	ldr r3, .L_020091d8
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
	ldr r3, .L_020091dc
	mov r1, r8
	ands r3, r1
	cmp r3, #0
	beq .L_020091c2
	cmp r7, #0
	beq .L_020091c2
	movs r3, #128
	lsls r3, r3, #9
	ands r3, r1
	cmp r3, #0
	beq .L_020090f8
	ldr r1, [r7, #4]
	adds r0, r6, #0
	bl Object_SetPartAttribute
.L_020090f8:
	movs r3, #128
	lsls r3, r3, #10
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_02009118
	adds r1, r6, #0
	adds r1, #35
	ldrb r3, [r1]
	movs r2, #254
	ands r2, r3
	strb r2, [r1]
	ldr r1, [r7]
	adds r0, r6, #0
	bl Object_SetSpritePriority
.L_02009118:
	movs r2, #128
	lsls r2, r2, #12
	mov r3, r8
	ands r2, r3
	cmp r2, #0
	beq .L_0200912c
	ldr r3, [r7, #8]
	str r3, [r6, #24]
	ldr r3, [r7, #12]
	str r3, [r6, #28]
.L_0200912c:
	movs r3, #128
	lsls r3, r3, #11
	mov r1, r8
	ands r3, r1
	cmp r3, #0
	beq .L_02009172
	ldr r3, .L_020091d4
	mov r1, r10
	ldr r5, [r3, r1]
	ldr r3, [r7, #16]
	ldr r1, [r5, #12]
	cmp r2, #0
	beq .L_0200915a
	ldr r0, [r6, #24]
	subs r0, r3, r0
	bl Engine_MathDivide
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, [r6, #28]
	ldr r1, [r5, #12]
	subs r0, r0, r3
	b .L_0200916c
.L_0200915a:
	ldr r2, .L_020091dc
	adds r0, r3, r2
	bl Engine_MathDivide
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, .L_020091dc
	ldr r1, [r5, #12]
	adds r0, r0, r3
.L_0200916c:
	bl Engine_MathDivide
	str r0, [r6, #52]
.L_02009172:
	movs r3, #128
	lsls r3, r3, #14
	mov r1, r8
	ands r3, r1
	cmp r3, #0
	beq .L_0200918e
	adds r0, r6, #0
	movs r1, #1
	bl Func_02001b68
	ldr r1, [r7, #28]
	adds r0, r6, #0
	bl Func_02001b78
.L_0200918e:
	movs r3, #128
	lsls r3, r3, #15
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_020091a0
	ldrh r3, [r7, #32]
	ldr r1, [sp, #0]
	strh r3, [r1, #18]
.L_020091a0:
	movs r3, #128
	lsls r3, r3, #16
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_020091b2
	ldrh r3, [r7, #34]
	mov r1, r9
	strh r3, [r1]
.L_020091b2:
	movs r3, #128
	lsls r3, r3, #17
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_020091c2
	ldr r3, [r7, #36]
	str r3, [r6, #108]
.L_020091c2:
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_020091d0:
	.4byte gPartyState
.L_020091d4:
	.4byte Data_0200260c
.L_020091d8:
	.4byte Func_02000fe0
.L_020091dc:
	.4byte 0xffff0000
	.section .text.x020091e0,"ax",%progbits
	.global Func_020011e0
	.thumb_func
Func_020011e0:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r4, .L_020092f8
	movs r1, #1
	movs r0, #12
	ldrsh r3, [r4, r0]
	negs r1, r1
	sub sp, #4
	cmp r3, r1
	beq .L_020092ec
	lsls r3, r3, #3
	adds r3, r3, r4
	adds r3, #32
	mov r8, r3
	ldr r3, .L_020092fc
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
	bne .L_0200922c
	ldr r3, [r0, #16]
	movs r5, #2
	ldrsh r2, [r1, r5]
	asrs r3, r3, #20
	cmp r3, r2
	beq .L_02009234
.L_0200922c:
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	strh r3, [r4, #12]
.L_02009234:
	movs r0, #12
	ldrsh r3, [r4, r0]
	movs r2, #1
	negs r2, r2
	ldr r1, .L_02009300
	cmp r3, r2
	beq .L_020092ec
	movs r5, #14
	ldrsh r3, [r4, r5]
	cmp r3, #0
	beq .L_020092ec
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
	ldr r2, .L_02009304
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
	bhi .L_020092ec
	movs r0, #15
	negs r0, r0
	cmp r2, r0
	blt .L_020092ec
	cmp r2, #239
	bgt .L_020092ec
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
	ldr r3, .L_02009308
	adds r0, r4, #0
	orrs r2, r3
	movs r3, #128
	str r2, [r4, #24]
	lsls r3, r3, #3
	mov r2, r10
	orrs r2, r3
	str r2, [r4, #28]
	adds r0, #20
	bl Func_02001b40
.L_020092ec:
	add sp, #4
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_020092f8:
	.4byte gOverlayArea + 0x2764
.L_020092fc:
	.4byte gPartyState
.L_02009300:
	.4byte 0xffff0000
.L_02009304:
	.4byte ResourceTableEntries
.L_02009308:
	.4byte 0x80008800
	.section .text.x0200930c,"ax",%progbits
	.global Func_0200130c
	.thumb_func
Func_0200130c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #48
	str r0, [sp, #44]
	ldr r0, .L_0200953c
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
	ldr r3, .L_02009540
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
	bge .L_02009490
.L_020093c0:
	ldr r1, [sp, #12]
	ldr r2, [sp, #36]
	ldr r5, [sp, #24]
	lsls r3, r1, #9
	adds r2, r2, r3
	movs r3, #0
	mov r11, r2
	str r3, [sp, #16]
	cmp r3, r5
	bge .L_02009484
.L_020093d4:
	mov r0, r11
	ldrb r5, [r0, #2]
	cmp r5, #0
	beq .L_02009474
	ldr r1, [sp, #44]
	cmp r5, r1
	bcc .L_02009474
	adds r1, #1
	mov r10, r1
	cmp r5, r10
	bhi .L_02009474
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
	bne .L_02009428
	cmp r5, r10
	bne .L_02009466
	mov r3, r9
	movs r2, #4
	ldrsh r0, [r3, r2]
	bl GameFlag_SetBit
	b .L_02009466
.L_02009428:
	mov r1, r9
	movs r5, #4
	ldrsh r0, [r1, r5]
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009466
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
	bl Func_02001bb0
.L_02009466:
	mov r0, r8
	ldrh r3, [r0, #10]
	mov r1, r8
	adds r3, #1
	strh r3, [r1, #10]
	movs r5, #8
	add r9, r5
.L_02009474:
	ldr r2, [sp, #16]
	ldr r5, [sp, #24]
	adds r2, #1
	movs r3, #4
	str r2, [sp, #16]
	add r11, r3
	cmp r2, r5
	blt .L_020093d4
.L_02009484:
	ldr r0, [sp, #12]
	ldr r1, [sp, #20]
	adds r0, #1
	str r0, [sp, #12]
	cmp r0, r1
	blt .L_020093c0
.L_02009490:
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020094e8
	ldr r3, .L_02009544
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
	bge .L_020094e8
.L_020094c2:
	mov r0, r9
	movs r5, #0
	ldrsh r3, [r0, r5]
	cmp r3, r4
	bne .L_020094d8
	movs r5, #2
	ldrsh r3, [r0, r5]
	cmp r3, r1
	bne .L_020094d8
	mov r0, r8
	strh r2, [r0, #12]
.L_020094d8:
	movs r3, #8
	mov r0, r8
	add r9, r3
	movs r5, #10
	ldrsh r3, [r0, r5]
	adds r2, #1
	cmp r2, r3
	blt .L_020094c2
.L_020094e8:
	movs r0, #128
	lsls r0, r0, #1
	bl Runtime_BumpAllocate
	adds r5, r0, #0
	adds r1, r5, #0
	movs r2, #63
.L_020094f6:
	ldr r3, .L_02009548
	subs r2, #1
	stmia r1!, {r3}
	cmp r2, #0
	bge .L_020094f6
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
	ldr r0, .L_0200954c
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
.L_0200953c:
	.4byte gOverlayArea + 0x2764
.L_02009540:
	.4byte IwramClearWords
.L_02009544:
	.4byte gPartyState
.L_02009548:
	.4byte 0x11111111
.L_0200954c:
	.4byte Func_020011e0
	.section .text.x02009550,"ax",%progbits
	.global Func_02001550
	.thumb_func
Func_02001550:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r8
	push {r5, r6}
	ldr r1, .L_020095d0
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
	bl Func_02001c48
	ldr r2, .L_020095d4
	ldr r3, [r0, #12]
	adds r3, r3, r2
	str r3, [r0, #12]
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, pc}
	.2byte 0x0000
.L_020095d0:
	.4byte gPartyState
.L_020095d4:
	.4byte 0xfff80000
	.section .text.x020095d8,"ax",%progbits
	.global Func_020015d8
	.thumb_func
Func_020015d8:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	ldr r3, .L_02009644
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
	bl Func_02001550
	movs r0, #161
	bl Func_02001ca0
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
	bl Func_02001bb0
	movs r0, #12
	bl Battle_WaitMode0
	add sp, #8
	pop {r3}
	mov r8, r3
	pop {r5, r6, pc}
.L_02009644:
	.4byte gPartyState
	.section .text.x02009648,"ax",%progbits
	.global Func_02001648
	.thumb_func
Func_02001648:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r1, .L_020096f8
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
	bl Func_02001550
	movs r0, #229
	bl Func_02001ca0
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
	bl Func_02001bb0
	movs r0, #12
	bl Battle_WaitMode0
	movs r3, #128
	ldr r2, .L_020096f0
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
	ldr r2, .L_020096f4
	movs r7, #0
	orrs r3, r2
	strh r3, [r1]
	mov r2, r10
	b .L_020096fc
	.2byte 0x0000
.L_020096f0:
	.4byte 0x00000000
.L_020096f4:
	.4byte 0x00008000
.L_020096f8:
	.4byte gPartyState
.L_020096fc:
	movs r3, #1
	mov r1, r8
	strh r3, [r1, #14]
	ldr r0, [r2]
	movs r1, #28
	bl Object_SetModeById
	movs r0, #16
	bl Battle_WaitMode0
.L_02009710:
	cmp r7, #5
	bne .L_0200971a
	movs r0, #204
	bl Func_02001ca0
.L_0200971a:
	ldr r3, [r6, #24]
	ldr r1, .L_02009778
	ldr r2, .L_0200977c
	adds r3, r3, r1
	str r3, [r6, #24]
	ldr r3, [r6, #28]
	ldr r1, .L_02009780
	adds r3, r3, r2
	str r3, [r6, #28]
	ldr r3, [r6, #12]
	movs r0, #1
	adds r3, r3, r1
	str r3, [r6, #12]
	adds r7, #1
	bl WaitFrames
	cmp r7, #39
	ble .L_02009710
	ldr r3, .L_02009784
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
.L_02009778:
	.4byte 0xfffffc00
.L_0200977c:
	.4byte 0xfffffd00
.L_02009780:
	.4byte 0xffff6667
.L_02009784:
	.4byte gPartyState
	.section .text.x02009788,"ax",%progbits
	.global Func_02001788
	.thumb_func
Func_02001788:
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
	bge .L_020097b8
	adds r3, #15
.L_020097b8:
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
	.section .text.x020097e0,"ax",%progbits
	.global Func_020017e0
	.thumb_func
Func_020017e0:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, .L_02009964
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	sub sp, #68
	bl Object_GetById
	adds r7, r0, #0
	bl Func_02001be8
	movs r0, #0
	bl Func_02001c70
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r1, r1
	negs r2, r2
	movs r3, #0
	negs r0, r0
	bl Motion_CamBounds
	bl Func_02001b90
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
	bl Func_02001ca0
	movs r3, #3
	strb r3, [r5]
	movs r0, #24
	bl Battle_WaitMode0
	add r2, sp, #28
	movs r3, #7
	str r3, [r2, #4]
	ldr r3, .L_02009968
	mov r8, r2
	str r3, [r2, #36]
	movs r3, #163
	lsls r3, r3, #8
	adds r3, #215
	str r3, [r2, #8]
	str r3, [r2, #12]
	movs r3, #0
	mov r10, r3
.L_0200987a:
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
	ldr r3, .L_0200996c
	adds r2, r2, r3
	str r2, [r6]
	bl Random16Far
	lsls r3, r0, #1
	ldr r5, [r6, #8]
	adds r3, r3, r0
	ldr r4, .L_02009970
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
	ldr r4, .L_02009974
	str r5, [sp, #4]
	str r4, [sp, #8]
	mov r4, r8
	str r4, [sp, #12]
	bl Func_02001018
	movs r2, #1
	add r10, r2
	mov r3, r10
	cmp r3, #16
	bls .L_0200987a
	movs r0, #188
	bl Func_02001ca0
	ldr r5, .L_02009964
	movs r4, #133
	lsls r4, r4, #2
	adds r5, r5, r4
	movs r1, #2
	ldr r0, [r5]
	adds r1, #255
	bl Func_02001c30
	ldr r0, [r5]
	movs r1, #49
	bl Object_SetModeById
	movs r0, #160
	movs r1, #160
	movs r2, #128
	lsls r0, r0, #11
	lsls r1, r1, #11
	lsls r2, r2, #9
	bl Func_02001bc0
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	adds r2, #102
	negs r0, r0
	negs r1, r1
	bl Func_02001bc0
	bl Func_02001bc8
	movs r1, #128
	ldr r0, [r5]
	lsls r1, r1, #1
	bl Func_02001c30
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
	bl Func_02001bf0
	add sp, #68
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_02009964:
	.4byte gPartyState
.L_02009968:
	.4byte Func_02001788
.L_0200996c:
	.4byte 0xffffa000
.L_02009970:
	.4byte 0xffffd000
.L_02009974:
	.4byte 0x01090001
	.section .text.x02009978,"ax",%progbits
	.global Func_02001978
	.thumb_func
Func_02001978:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, .L_02009a20
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	ldr r0, [r3]
	bl Object_GetById
	ldr r3, [r0, #8]
	ldr r6, .L_02009a24
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
	bge .L_02009a14
.L_020099ac:
	movs r1, #0
	ldrsh r3, [r5, r1]
	cmp r3, r8
	bne .L_02009a08
	movs r1, #2
	ldrsh r3, [r5, r1]
	cmp r3, r10
	bne .L_02009a08
	movs r2, #4
	ldrsh r0, [r5, r2]
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020099dc
	adds r0, r6, #0
	adds r1, r5, #0
	bl Func_020015d8
	movs r3, #4
	ldrsh r0, [r5, r3]
	bl GameFlag_SetBit
	strh r7, [r6, #12]
	b .L_02009a14
.L_020099dc:
	movs r1, #12
	ldrsh r3, [r6, r1]
	cmp r7, r3
	beq .L_02009a14
	adds r0, r6, #0
	adds r1, r5, #0
	strh r7, [r6, #12]
	bl Func_02001648
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
	b .L_02009a16
.L_02009a08:
	lsls r3, r2, #16
	adds r7, #1
	asrs r3, r3, #16
	adds r5, #8
	cmp r7, r3
	blt .L_020099ac
.L_02009a14:
	movs r0, #0
.L_02009a16:
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009a20:
	.4byte gPartyState
.L_02009a24:
	.4byte gOverlayArea + 0x2764
	.section .text.x02009a28,"ax",%progbits
	.global Func_02001a28
	.thumb_func
Func_02001a28:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #4
	ldr r3, .L_02009ad8
	str r2, [sp, #0]
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	mov r9, r0
	ldr r0, [r3]
	mov r11, r1
	bl Object_GetById
	movs r3, #192
	ldr r5, .L_02009adc
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
	bne .L_02009a76
	cmp r0, #0
	beq .L_02009aca
.L_02009a76:
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
	bl Func_02001b90
	bl Func_020017e0
	movs r2, #192
	lsls r2, r2, #18
	ldr r3, [r2, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	ldr r2, [sp, #0]
	str r2, [r3]
.L_02009aca:
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_02009ad8:
	.4byte gPartyState
.L_02009adc:
	.4byte gOverlayArea + 0x2764
	.section .rodata.x02009ca8,"a",%progbits
	.global Data_02001ca8
Data_02001ca8:
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
	.global Data_02001e0a
Data_02001e0a:
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
	.global Data_0200203a
Data_0200203a:
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
.L_0200a11c:
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
.L_0200a158:
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
.L_0200a194:
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
	.global Data_020021d0
Data_020021d0:
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
	.global Data_02002200
Data_02002200:
	.4byte 0x00000106
	.4byte 0x00101104
	.4byte 0x00203106
	.4byte 0x00302106
	.4byte 0x00404108
	.4byte 0x00506106
	.4byte 0x00605106
	.4byte 0x00708106
	.4byte 0x00807106
	.4byte 0x00d0d106
	.4byte 0x00e0e106
	.4byte 0x000001ff
	.global Data_02002230
Data_02002230:
	.4byte 0xffff01e9
	.4byte 0x0000000b
	.4byte 0xfff80000
	.4byte 0x00000000
	.4byte 0xfff80000
	.4byte 0x00028000
	.4byte 0xffff013d
	.4byte 0x0000000b
	.4byte 0x01a80000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00024000
	.4byte 0xffff013d
	.4byte 0x0000000b
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x01024000
	.4byte 0xffff013d
	.4byte 0x0000000b
	.4byte 0x02980000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x01024000
	.4byte 0xffff013d
	.4byte 0x0000000b
	.4byte 0x02a80000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x01024000
	.4byte 0xffff013d
	.4byte 0x0000000b
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x01024000
	.4byte 0xffff013d
	.4byte 0x0000000b
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x01024000
	.4byte 0xffff013d
	.4byte 0x0000000b
	.4byte 0x01a80000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x01024000
	.4byte 0xffff013d
	.4byte 0x0000000b
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x01024000
	.4byte 0xffff013d
	.4byte 0x0000000b
	.4byte 0x01c80000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x01024000
	.4byte 0xffff013d
	.4byte 0x0000000b
	.4byte 0x01d80000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x01024000
	.4byte 0xffff013d
	.4byte 0x0000000b
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x01024000
	.4byte 0xffff013d
	.4byte 0x0000000b
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x01024000
	.4byte 0xffff013d
	.4byte 0x0000000b
	.4byte 0x02080000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x01024000
	.4byte 0xffff013d
	.4byte 0x0000000b
	.4byte 0x02180000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x01024000
	.4byte 0xffff013d
	.4byte 0x0000000b
	.4byte 0x02280000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x01024000
	.4byte 0xffff013d
	.4byte 0x0000000b
	.4byte 0x02380000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x01024000
	.4byte 0xffff013d
	.4byte 0x0000000b
	.4byte 0x02480000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x01024000
	.4byte 0xffff013d
	.4byte 0x0000000b
	.4byte 0x02580000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x01024000
	.4byte 0xffff013d
	.4byte 0x0000000b
	.4byte 0x02680000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x01024000
	.4byte 0xffff013d
	.4byte 0x0000000b
	.4byte 0x02780000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x01024000
	.4byte 0xffff013d
	.4byte 0x0000000b
	.4byte 0x02880000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x01024000
	.4byte 0xffff013d
	.4byte 0x0000000b
	.4byte 0x02980000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x01024000
	.4byte 0xffff013d
	.4byte 0x0000000b
	.4byte 0x02a80000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x01024000
	.4byte 0xffff013d
	.4byte 0x0000000b
	.4byte 0x02b80000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x01024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020024a0
Data_020024a0:
	.4byte 0x00000031
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000021
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000031
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
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
	.4byte 0x00000031
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
	.4byte 0x00000002
	.4byte 0xffff001e
	.4byte Func_02000090
	.4byte 0x00000002
	.4byte 0xffff001f
	.4byte Func_02000090
	.4byte 0x00000002
	.4byte 0xffff0032
	.4byte Func_020000e8
	.4byte 0x00000002
	.4byte 0xffff0033
	.4byte Func_020000e8
	.4byte 0x00000002
	.4byte 0xffff0034
	.4byte Func_020000e8
	.4byte 0x00000002
	.4byte 0xffff0035
	.4byte Func_020000e8
	.4byte 0x00000006
	.4byte 0xffff00c8
	.4byte Func_02000538
	.4byte 0x00000002
	.4byte 0xffff0021
	.4byte Func_0200018c
	.4byte 0x00000002
	.4byte 0xffff0022
	.4byte Func_02000228
	.4byte 0x00000002
	.4byte 0xffff0023
	.4byte Func_0200018c
	.4byte 0x00000002
	.4byte 0xffff002b
	.4byte Func_020001bc
	.4byte 0x00000002
	.4byte 0xffff002c
	.4byte Func_020001bc
	.4byte 0x00000002
	.4byte 0xffff002d
	.4byte Func_020001bc
	.4byte 0x00009985
	.4byte 0xffff0000
	.4byte Func_02000054
	.4byte 0x20009985
	.4byte 0xffff0000
	.4byte Func_0200007c
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020025f0
Data_020025f0:
	.4byte 0xffffffff
	.global Data_020025f4
Data_020025f4:
	.4byte 0x00000001
	.global Data_020025f8
Data_020025f8:
	.4byte 0x00000027
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x0000000c
	.4byte 0x00000026
	.global Data_0200260c
Data_0200260c:
	.4byte .L_0200a11c
	.4byte .L_0200a158
	.4byte .L_0200a194
