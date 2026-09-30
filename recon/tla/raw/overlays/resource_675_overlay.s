.syntax unified
	.thumb
	.section .text.x02008038,"ax",%progbits
	.global Func_02000038
	.thumb_func
Func_02000038:
	push {r5, r6, lr}
	adds r4, r1, #0
	movs r1, #192
	lsls r1, r1, #18
	adds r1, #128
	ldr r6, [r1]
	ldr r1, .L_020080dc
	adds r5, r0, #0
	str r5, [r1]
	ldr r1, .L_020080e0
	str r4, [r1]
	ldr r1, .L_020080e4
	str r2, [r1]
	ldr r2, .L_020080e8
	str r3, [r2]
	movs r2, #255
	ldrh r3, [r5]
	b .L_02008082
.L_0200805c:
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
.L_02008082:
	lsls r2, r2, #8
	adds r2, #255
	cmp r3, r2
	beq .L_02008090
	ldrh r3, [r4]
	cmp r3, r2
	bne .L_0200805c
.L_02008090:
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
	ldr r0, .L_020080ec
	adds r2, #112
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #0
	bl Func_020012ec
	ldr r3, .L_020080f0
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #194
	adds r3, r3, r2
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #2
	bne .L_020080da
	bl Func_02000328
.L_020080da:
	pop {r5, r6, pc}
.L_020080dc:
	.4byte gOverlayArea + 0x1a24
.L_020080e0:
	.4byte gOverlayArea + 0x1a28
.L_020080e4:
	.4byte gOverlayArea + 0x1a2c
.L_020080e8:
	.4byte gOverlayArea + 0x1a18
.L_020080ec:
	.4byte 0x05000200
.L_020080f0:
	.4byte gPartyState
	.section .text.x020080f4,"ax",%progbits
	.global Func_020000f4
	.thumb_func
Func_020000f4:
	push {r5, lr}
	ldr r2, .L_02008138
	ldr r3, .L_02008128
	ldr r5, .L_0200813c
	strh r3, [r2]
	ldr r3, .L_02008140
	ldr r0, [r3]
	bl Func_020001d4
	ldr r2, .L_02008144
	ldr r3, .L_0200812c
	strh r0, [r5]
	strh r3, [r2]
	ldr r2, .L_02008148
	ldr r3, .L_02008130
	movs r1, #144
	strh r3, [r2]
	ldr r2, .L_0200814c
	ldr r3, .L_02008134
	lsls r1, r1, #3
	strh r3, [r2]
	ldr r0, .L_02008150
	bl Func_020011e4
	b .L_02008154
	.2byte 0x0000
.L_02008128:
	.4byte 0x00000000
.L_0200812c:
	.4byte 0x0000000f
.L_02008130:
	.4byte 0x00000010
.L_02008134:
	.4byte 0x00000001
.L_02008138:
	.4byte gOverlayArea + 0x1a34
.L_0200813c:
	.4byte gOverlayArea + 0x1a30
.L_02008140:
	.4byte gOverlayArea + 0x1a24
.L_02008144:
	.4byte gOverlayArea + 0x1a20
.L_02008148:
	.4byte gOverlayArea + 0x1a1c
.L_0200814c:
	.4byte gOverlayArea + 0x1a14
.L_02008150:
	.4byte Func_020001f8
.L_02008154:
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x02008158,"ax",%progbits
	.global Func_02000158
	.thumb_func
Func_02000158:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	subs r2, #172
	str r2, [r3]
	ldr r0, .L_02008174
	bl Scheduler_RemoveCallbackFar
	pop {pc}
	.2byte 0x0000
.L_02008174:
	.4byte Func_020001f8
	.section .text.x02008178,"ax",%progbits
	.global Func_02000178
	.thumb_func
Func_02000178:
	push {r5, lr}
	ldr r2, .L_020081b4
	ldr r3, .L_020081a8
	ldr r5, .L_020081b8
	strh r3, [r2]
	ldr r3, .L_020081bc
	ldr r0, [r3]
	bl Func_020001d4
	ldr r2, .L_020081ac
	ldr r3, .L_020081c0
	strh r0, [r5]
	strh r2, [r3]
	ldr r3, .L_020081c4
	movs r1, #144
	strh r2, [r3]
	ldr r2, .L_020081c8
	ldr r3, .L_020081b0
	lsls r1, r1, #3
	strh r3, [r2]
	ldr r0, .L_020081cc
	bl Func_020011e4
	b .L_020081d0
.L_020081a8:
	.4byte 0x00000000
.L_020081ac:
	.4byte 0x00000002
.L_020081b0:
	.4byte 0x00000001
.L_020081b4:
	.4byte gOverlayArea + 0x1a34
.L_020081b8:
	.4byte gOverlayArea + 0x1a30
.L_020081bc:
	.4byte gOverlayArea + 0x1a24
.L_020081c0:
	.4byte gOverlayArea + 0x1a20
.L_020081c4:
	.4byte gOverlayArea + 0x1a1c
.L_020081c8:
	.4byte gOverlayArea + 0x1a14
.L_020081cc:
	.4byte Func_020001f8
.L_020081d0:
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x020081d4,"ax",%progbits
	.global Func_020001d4
	.thumb_func
Func_020001d4:
	push {lr}
	ldr r1, .L_020081ec
	ldrh r3, [r0]
	movs r2, #0
	cmp r3, r1
	beq .L_020081f0
.L_020081e0:
	adds r0, #2
	ldrh r3, [r0]
	adds r2, #1
	cmp r3, r1
	bne .L_020081e0
	b .L_020081f0
.L_020081ec:
	.4byte 0x0000ffff
.L_020081f0:
	subs r2, #1
	adds r0, r2, #0
	pop {pc}
	.2byte 0x0000
	.section .text.x020081f8,"ax",%progbits
	.global Func_020001f8
	.thumb_func
Func_020001f8:
	push {r5, r6, r7, lr}
	ldr r1, .L_020082a8
	movs r4, #0
	ldrh r2, [r1]
	adds r3, r2, #0
	cmp r3, #0
	beq .L_02008232
	movs r0, #255
	lsls r0, r0, #8
	adds r0, #255
	adds r3, r2, r0
	strh r3, [r1]
	lsls r3, r3, #16
	cmp r3, #0
	bne .L_02008232
	ldr r0, .L_020082ac
	movs r4, #1
	ldrh r2, [r0]
	strh r2, [r1]
	movs r1, #128
	lsls r3, r2, #16
	lsls r1, r1, #10
	cmp r3, r1
	bls .L_02008232
	movs r1, #255
	lsls r1, r1, #8
	adds r1, #255
	adds r3, r2, r1
	strh r3, [r0]
.L_02008232:
	cmp r4, #0
	bne .L_02008238
	b .L_02008324
.L_02008238:
	ldr r3, .L_020082b0
	ldr r6, .L_020082b4
	ldr r1, [r3]
	ldrh r3, [r6]
	movs r5, #0
	cmp r5, r3
	bcs .L_02008286
	ldr r3, .L_020082b8
	ldr r2, .L_020082bc
	ldr r7, [r3]
	mov lr, r2
	mov r12, r6
.L_02008250:
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
	bcc .L_02008250
.L_02008286:
	ldr r3, .L_020082b4
	movs r0, #160
	ldrh r1, [r3]
	ldr r3, .L_020082c0
	lsls r2, r1, #1
	ldr r3, [r3]
	lsls r0, r0, #19
	ldrh r3, [r2, r3]
	adds r2, r2, r1
	lsls r3, r3, #1
	adds r4, r3, r0
	ldr r3, .L_020082c4
	ldrh r3, [r3]
	cmp r3, #0
	beq .L_020082cc
	ldr r3, .L_020082c8
	b .L_020082ce
.L_020082a8:
	.4byte gOverlayArea + 0x1a1c
.L_020082ac:
	.4byte gOverlayArea + 0x1a20
.L_020082b0:
	.4byte gOverlayArea + 0x1a2c
.L_020082b4:
	.4byte gOverlayArea + 0x1a30
.L_020082b8:
	.4byte gOverlayArea + 0x1a18
.L_020082bc:
	.4byte gOverlayArea + 0x1a34
.L_020082c0:
	.4byte gOverlayArea + 0x1a24
.L_020082c4:
	.4byte gOverlayArea + 0x1a14
.L_020082c8:
	.4byte gOverlayArea + 0x1a28
.L_020082cc:
	ldr r3, .L_02008314
.L_020082ce:
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
	ldr r1, .L_02008318
	ldr r2, .L_0200830c
	ldrh r3, [r1]
	eors r3, r2
	strh r3, [r1]
	ldr r1, .L_0200831c
	ldr r2, .L_02008320
	ldrh r3, [r1]
	adds r3, #1
	strh r3, [r1]
	lsls r3, r3, #16
	ldrh r2, [r2]
	lsrs r3, r3, #16
	cmp r3, r2
	bne .L_02008324
	ldr r3, .L_02008310
	strh r3, [r1]
	b .L_02008324
	.2byte 0x0000
.L_0200830c:
	.4byte 0x00000001
.L_02008310:
	.4byte 0x00000000
.L_02008314:
	.4byte gOverlayArea + 0x1a2c
.L_02008318:
	.4byte gOverlayArea + 0x1a14
.L_0200831c:
	.4byte gOverlayArea + 0x1a34
.L_02008320:
	.4byte gOverlayArea + 0x1a30
.L_02008324:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x02008328,"ax",%progbits
	.global Func_02000328
	.thumb_func
Func_02000328:
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
	ldr r0, .L_020083a8
	movs r1, #1
	mov r10, r2
	bl Func_020012ec
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #2
	bl Func_020012ec
	ldr r2, [r6, #108]
	movs r3, #128
	lsls r3, r3, #1
	mov r1, r8
	str r3, [r2, r1]
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	bl Func_02000178
	bl Func_0200132c
	movs r0, #40
	bl WaitFrames
	bl Func_02000158
	movs r0, #128
	movs r1, #1
	lsls r0, r0, #9
	bl Func_020012ec
	movs r0, #16
	bl Func_020012fc
	movs r0, #16
	bl WaitFrames
	ldr r3, .L_020083ac
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
.L_020083a8:
	.4byte 0x00202108
.L_020083ac:
	.4byte gPartyState
	.section .text.x020083b0,"ax",%progbits
	.global Func_020003b0
	.thumb_func
Func_020003b0:
	push {lr}
	movs r0, #15
	movs r1, #12
	bl Func_020012dc
	pop {pc}
	.section .text.x020083bc,"ax",%progbits
	.global Func_020003bc
	.thumb_func
Func_020003bc:
	push {lr}
	movs r0, #9
	movs r1, #37
	bl Func_020012dc
	pop {pc}
	.section .text.x020083c8,"ax",%progbits
	.global Func_020003c8
	.thumb_func
Func_020003c8:
	push {lr}
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #0
	pop {pc}
	.section .text.x020083d4,"ax",%progbits
	.global Func_020003d4
	.thumb_func
Func_020003d4:
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
	cmp r0, #31
	bgt .L_0200840a
	cmp r0, #0
	bne .L_020083fa
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r2, #40]
.L_020083fa:
	lsls r0, r0, #12
	bl Math_Cosine
	cmp r0, #0
	bge .L_02008406
	adds r0, #127
.L_02008406:
	asrs r3, r0, #7
	strh r3, [r6, #18]
.L_0200840a:
	ldrb r3, [r5]
	movs r0, #1
	adds r3, #1
	strb r3, [r5]
	negs r0, r0
	pop {r5, r6, pc}
	.2byte 0x0000
	.section .text.x02008418,"ax",%progbits
	.global Func_02000418
	.thumb_func
Func_02000418:
	ldr r0, .L_0200841c
	bx lr
.L_0200841c:
	.4byte Data_02001460
	.section .text.x02008420,"ax",%progbits
	.global Func_02000420
	.thumb_func
Func_02000420:
	movs r0, #0
	bx lr
	.section .text.x02008424,"ax",%progbits
	.global Func_02000424
	.thumb_func
Func_02000424:
	ldr r0, .L_02008428
	bx lr
.L_02008428:
	.4byte Data_02001490
	.section .text.x0200842c,"ax",%progbits
	.global Func_0200042c
	.thumb_func
Func_0200042c:
	push {lr}
	ldr r3, .L_02008454
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008458
	cmp r2, r3
	bne .L_02008444
	ldr r0, .L_0200845c
	b .L_02008450
.L_02008444:
	ldr r3, .L_02008460
	cmp r2, r3
	bne .L_0200844e
	ldr r0, .L_02008464
	b .L_02008450
.L_0200844e:
	ldr r0, .L_02008468
.L_02008450:
	pop {pc}
	.2byte 0x0000
.L_02008454:
	.4byte gPartyState
.L_02008458:
	.4byte 0x00000088
.L_0200845c:
	.4byte Data_02001668
.L_02008460:
	.4byte 0x00000089
.L_02008464:
	.4byte Data_02001740
.L_02008468:
	.4byte Data_02001548
	.section .text.x0200846c,"ax",%progbits
	.global Func_0200046c
	.thumb_func
Func_0200046c:
	push {lr}
	ldr r3, .L_02008494
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008498
	cmp r2, r3
	bne .L_02008484
	ldr r0, .L_0200849c
	b .L_02008490
.L_02008484:
	ldr r3, .L_020084a0
	cmp r2, r3
	bne .L_0200848e
	ldr r0, .L_020084a4
	b .L_02008490
.L_0200848e:
	ldr r0, .L_020084a8
.L_02008490:
	pop {pc}
	.2byte 0x0000
.L_02008494:
	.4byte gPartyState
.L_02008498:
	.4byte 0x00000089
.L_0200849c:
	.4byte Data_02001914
.L_020084a0:
	.4byte 0x0000008a
.L_020084a4:
	.4byte Data_020019b0
.L_020084a8:
	.4byte Data_02001788
	.section .text.x020084ac,"ax",%progbits
	.global Func_020004ac
	.thumb_func
Func_020004ac:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r5, r5, r2
	ldr r6, [r5]
	bl Func_02000178
	bl Func_0200132c
	movs r0, #40
	bl WaitFrames
	bl Func_02000158
	movs r0, #128
	movs r1, #1
	lsls r0, r0, #9
	bl Func_020012ec
	movs r0, #16
	bl Func_020012fc
	movs r0, #16
	bl WaitFrames
	ldr r3, .L_02008504
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #194
	adds r1, r3, r2
	movs r2, #0
	strb r2, [r1]
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	movs r1, #0
	bl Func_020012bc
	str r6, [r5]
	pop {r5, r6, pc}
.L_02008504:
	.4byte gPartyState
	.section .text.x02008508,"ax",%progbits
	.global Func_02000508
	.thumb_func
Func_02000508:
	push {r5, lr}
	bl Func_020000f4
	bl Func_02001334
	ldr r5, .L_02008540
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #194
	adds r2, r5, r3
	movs r3, #1
	strb r3, [r2]
	movs r0, #40
	bl Battle_WaitMode0
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	movs r1, #232
	movs r2, #136
	ldr r0, [r5]
	lsls r1, r1, #16
	lsls r2, r2, #16
	bl Func_0200129c
	bl Func_020004ac
	pop {r5, pc}
.L_02008540:
	.4byte gPartyState
	.section .text.x02008544,"ax",%progbits
	.global Func_02000544
	.thumb_func
Func_02000544:
	push {r5, lr}
	bl Func_020000f4
	bl Func_02001334
	ldr r5, .L_0200857c
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #194
	adds r2, r5, r3
	movs r3, #1
	strb r3, [r2]
	movs r0, #40
	bl Battle_WaitMode0
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	movs r1, #240
	movs r2, #240
	ldr r0, [r5]
	lsls r1, r1, #16
	lsls r2, r2, #17
	bl Func_0200129c
	bl Func_020004ac
	pop {r5, pc}
.L_0200857c:
	.4byte gPartyState
	.section .text.x02008580,"ax",%progbits
	.global Func_02000580
	.thumb_func
Func_02000580:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #128
	mov r12, r0
	ldr r0, [r3]
	movs r3, #224
	lsls r3, r3, #4
	movs r4, #224
	adds r1, r0, r3
	mov lr, r4
	movs r7, #0
.L_0200859c:
	ldrh r3, [r0]
	movs r5, #31
	lsrs r2, r3, #10
	ldr r6, .L_020085f8
	ands r2, r5
	movs r4, #248
	lsls r2, r2, #10
	lsls r4, r4, #7
	adds r0, #2
	mov r8, r6
	cmp r2, r4
	bls .L_020085b6
	adds r2, r4, #0
.L_020085b6:
	strh r2, [r1]
	lsrs r2, r3, #5
	ands r2, r5
	mov r6, r12
	muls r6, r2
	adds r2, r6, #0
	mov r6, r8
	ands r2, r6
	adds r1, #2
	cmp r2, r4
	bls .L_020085ce
	adds r2, r4, #0
.L_020085ce:
	strh r2, [r1]
	adds r2, r5, #0
	ands r2, r3
	mov r3, r12
	muls r3, r2
	mov r6, r8
	adds r2, r3, #0
	ands r2, r6
	adds r1, #2
	cmp r2, r4
	bls .L_020085e6
	adds r2, r4, #0
.L_020085e6:
	adds r7, #1
	strh r2, [r1]
	adds r1, #2
	cmp r7, lr
	bcc .L_0200859c
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_020085f8:
	.4byte 0xfffffc00
	.section .text.x020085fc,"ax",%progbits
	.global Func_020005fc
	.thumb_func
Func_020005fc:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r6, [r3, #108]
	bl Func_02001254
	movs r0, #0
	bl Func_02001324
	movs r5, #8
.L_02008610:
	adds r0, r5, #0
	bl Object_GetById
	cmp r0, #0
	beq .L_02008622
	adds r2, r0, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
.L_02008622:
	adds r5, #1
	cmp r5, #63
	bls .L_02008610
	movs r3, #170
	lsls r3, r3, #1
	adds r6, r6, r3
	movs r3, #0
	ldrsh r5, [r6, r3]
	movs r0, #158
	subs r5, #1
	bl Func_02001344
	lsrs r5, r5, #1
	ldr r0, .L_02008694
	lsls r5, r5, #3
	adds r3, r5, #4
	ldrh r1, [r0, r3]
	adds r3, r3, r0
	ldrh r2, [r3, #2]
	ldr r0, [r0, r5]
	bl Func_0200122c
	ldr r5, .L_02008698
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
	movs r2, #4
	movs r1, #0
	negs r2, r2
	ldr r0, [r5]
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r0, #6
	bl Battle_WaitMode0
	movs r3, #0
	ldrsh r0, [r6, r3]
	bl Func_020012cc
	bl Event_ClearStatus1c6
	bl Event_WaitValue1c8Frames
	bl Func_0200125c
	pop {r5, r6, pc}
.L_02008694:
	.4byte Data_020019ec
.L_02008698:
	.4byte gPartyState
	.section .text.x0200869c,"ax",%progbits
	.global Func_0200069c
	.thumb_func
Func_0200069c:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r1, #214
	movs r2, #133
	lsls r1, r1, #1
	lsls r2, r2, #1
	adds r3, r3, r1
	adds r2, #255
	str r2, [r3]
	movs r0, #0
	bl Func_0200131c
	ldr r5, .L_020087b4
	movs r2, #240
	lsls r2, r2, #1
	adds r3, r5, r2
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_020087b8
	cmp r2, r3
	bne .L_020086dc
	bl Func_020008c8
	ldr r0, .L_020087bc
	ldr r1, .L_020087c0
	ldr r2, .L_020087c4
	ldr r3, .L_020087c8
	bl Func_02000038
	b .L_02008754
.L_020086dc:
	ldr r3, .L_020087cc
	cmp r2, r3
	bne .L_02008712
	movs r0, #9
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r5, r2
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #2
	bne .L_02008754
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl Func_0200129c
	movs r0, #141
	lsls r0, r0, #4
	adds r0, #255
	bl Func_02001204
	b .L_02008754
.L_02008712:
	ldr r3, .L_020087d0
	cmp r2, r3
	bne .L_02008766
	movs r0, #85
	bl Func_020011fc
	cmp r0, #0
	bne .L_0200872c
	movs r0, #165
	bl Func_020011fc
	cmp r0, #0
	beq .L_02008736
.L_0200872c:
	movs r0, #140
	lsls r0, r0, #2
	bl Func_02001204
	b .L_02008754
.L_02008736:
	movs r0, #140
	lsls r0, r0, #2
	bl Func_020011fc
	cmp r0, #0
	bne .L_02008754
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_0200129c
	ldr r1, .L_020087d4
	movs r0, #8
	bl ObjectMotion_EnableActionAndSetCallback
.L_02008754:
	ldr r3, .L_020087b4
	movs r2, #240
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_020087d0
	cmp r2, r3
	beq .L_0200877e
.L_02008766:
	ldr r3, .L_020087b4
	movs r2, #240
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_020087cc
	cmp r2, r3
	bne .L_020087b0
	ldr r3, .L_020087d0
	cmp r2, r3
	bne .L_02008794
.L_0200877e:
	ldr r3, .L_020087b4
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r1, #0
	ldrsh r3, [r3, r1]
	subs r3, #3
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	b .L_02008796
.L_02008794:
	movs r3, #3
.L_02008796:
	movs r0, #128
	lsls r3, r3, #7
	lsls r0, r0, #3
	subs r0, r0, r3
	bl Func_02000580
	bl Func_0200133c
	movs r0, #1
	bl Func_020012fc
	bl Event_WaitValue1c8Frames
.L_020087b0:
	movs r0, #0
	pop {r5, pc}
.L_020087b4:
	.4byte gPartyState
.L_020087b8:
	.4byte 0x00000088
.L_020087bc:
	.4byte Data_0200134c
.L_020087c0:
	.4byte Data_0200135c
.L_020087c4:
	.4byte Data_02001388
.L_020087c8:
	.4byte Data_020013b4
.L_020087cc:
	.4byte 0x0000008a
.L_020087d0:
	.4byte 0x00000089
.L_020087d4:
	.4byte Data_02001454
	.section .text.x020087d8,"ax",%progbits
	.global Func_020007d8
	.thumb_func
Func_020007d8:
	movs r0, #0
	bx lr
	.section .text.x020087dc,"ax",%progbits
	.global Func_020007dc
	.thumb_func
Func_020007dc:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	sub sp, #8
	adds r6, r0, #0
	mov r8, r1
	mov r10, r2
	adds r7, r3, #0
	bl Object_GetById
	adds r5, r0, #0
	cmp r5, #0
	beq .L_0200883c
	ldr r0, [sp, #40]
	bl Func_020011fc
	cmp r0, #0
	beq .L_0200880e
	ldr r2, [sp, #32]
	lsls r1, r7, #16
	lsls r2, r2, #16
	adds r0, r6, #0
	bl Func_0200129c
.L_0200880e:
	adds r0, r6, #0
	movs r1, #3
	bl Func_020012c4
	adds r1, r5, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #2
	orrs r3, r2
	strb r3, [r1]
	mov r0, r8
	ldr r3, [r5, #8]
	ldr r2, [r5, #16]
	asrs r3, r3, #20
	asrs r2, r2, #20
	subs r3, #1
	str r3, [sp, #0]
	str r2, [sp, #4]
	mov r1, r10
	movs r2, #3
	movs r3, #1
	bl Func_02001234
.L_0200883c:
	add sp, #8
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x02008848,"ax",%progbits
	.global Func_02000848
	.thumb_func
Func_02000848:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #8
	adds r5, r0, #0
	adds r7, r1, #0
	mov r8, r2
	adds r6, r3, #0
	bl Object_GetById
	cmp r0, #0
	beq .L_020088aa
	adds r0, r5, #0
	movs r1, #3
	bl Func_020012c4
	ldr r0, [sp, #36]
	bl Func_020011fc
	cmp r0, #0
	beq .L_0200887e
	ldr r2, [sp, #28]
	lsls r1, r6, #16
	lsls r2, r2, #16
	adds r0, r5, #0
	bl Func_0200129c
.L_0200887e:
	adds r0, r5, #0
	bl Object_GetById
	adds r1, r0, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #2
	orrs r3, r2
	strb r3, [r1]
	mov r1, r8
	ldr r3, [r0, #16]
	ldr r2, [r0, #8]
	asrs r3, r3, #20
	asrs r2, r2, #20
	subs r3, #1
	str r2, [sp, #0]
	str r3, [sp, #4]
	adds r0, r7, #0
	movs r2, #1
	movs r3, #3
	bl Func_02001234
.L_020088aa:
	add sp, #8
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x020088b4,"ax",%progbits
	.global Func_020008b4
	.thumb_func
Func_020008b4:
	push {r5, lr}
	adds r5, r0, #0
	movs r1, #1
	bl Object_SetModeById
	adds r0, r5, #0
	movs r1, #2
	bl Object_SetModeById
	pop {r5, pc}
	.section .text.x020088c8,"ax",%progbits
	.global Func_020008c8
	.thumb_func
Func_020008c8:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	movs r3, #252
	sub sp, #12
	lsls r3, r3, #1
	str r3, [sp, #0]
	movs r3, #20
	str r3, [sp, #4]
	movs r3, #160
	lsls r3, r3, #2
	str r3, [sp, #8]
	movs r0, #8
	movs r1, #27
	movs r2, #0
	movs r3, #136
	bl Func_02000848
	movs r3, #232
	str r3, [sp, #0]
	movs r3, #21
	str r3, [sp, #4]
	movs r3, #193
	lsls r3, r3, #1
	adds r3, #255
	str r3, [sp, #8]
	movs r0, #9
	movs r1, #28
	movs r2, #0
	movs r3, #136
	bl Func_02000848
	movs r3, #214
	lsls r3, r3, #2
	mov r8, r3
	movs r3, #22
	str r3, [sp, #4]
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #130
	str r3, [sp, #8]
	movs r5, #152
	mov r3, r8
	movs r0, #10
	movs r1, #29
	movs r2, #0
	str r5, [sp, #0]
	bl Func_02000848
	movs r3, #23
	str r3, [sp, #4]
	movs r3, #194
	movs r6, #198
	lsls r3, r3, #1
	lsls r6, r6, #2
	adds r3, #255
	str r3, [sp, #8]
	movs r0, #11
	adds r3, r6, #0
	movs r1, #30
	movs r2, #0
	str r5, [sp, #0]
	bl Func_02000848
	movs r3, #24
	str r3, [sp, #4]
	movs r3, #161
	lsls r3, r3, #2
	str r3, [sp, #8]
	adds r5, #224
	mov r3, r8
	movs r0, #12
	movs r1, #31
	movs r2, #0
	str r5, [sp, #0]
	bl Func_02000848
	movs r3, #25
	str r3, [sp, #4]
	movs r3, #195
	lsls r3, r3, #1
	adds r3, #255
	str r3, [sp, #8]
	movs r0, #13
	adds r3, r6, #0
	movs r1, #32
	movs r2, #0
	str r5, [sp, #0]
	bl Func_02000848
	movs r2, #120
	str r2, [sp, #0]
	movs r2, #26
	str r2, [sp, #4]
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #134
	movs r3, #206
	str r2, [sp, #8]
	lsls r3, r3, #2
	movs r0, #14
	movs r1, #33
	movs r2, #0
	bl Func_020007dc
	add sp, #12
	pop {r3}
	mov r8, r3
	pop {r5, r6, pc}
	.2byte 0x0000
	.section .text.x020089a4,"ax",%progbits
	.global Func_020009a4
	.thumb_func
Func_020009a4:
	push {lr}
	movs r0, #8
	bl Func_020008b4
	pop {pc}
	.2byte 0x0000
	.section .text.x020089b0,"ax",%progbits
	.global Func_020009b0
	.thumb_func
Func_020009b0:
	push {lr}
	movs r0, #9
	bl Func_020008b4
	pop {pc}
	.2byte 0x0000
	.section .text.x020089bc,"ax",%progbits
	.global Func_020009bc
	.thumb_func
Func_020009bc:
	push {lr}
	movs r0, #10
	bl Func_020008b4
	pop {pc}
	.2byte 0x0000
	.section .text.x020089c8,"ax",%progbits
	.global Func_020009c8
	.thumb_func
Func_020009c8:
	push {lr}
	movs r0, #11
	bl Func_020008b4
	pop {pc}
	.2byte 0x0000
	.section .text.x020089d4,"ax",%progbits
	.global Func_020009d4
	.thumb_func
Func_020009d4:
	push {lr}
	movs r0, #12
	bl Func_020008b4
	pop {pc}
	.2byte 0x0000
	.section .text.x020089e0,"ax",%progbits
	.global Func_020009e0
	.thumb_func
Func_020009e0:
	push {lr}
	movs r0, #13
	bl Func_020008b4
	pop {pc}
	.2byte 0x0000
	.section .text.x020089ec,"ax",%progbits
	.global Func_020009ec
	.thumb_func
Func_020009ec:
	push {lr}
	movs r0, #14
	bl Func_020008b4
	pop {pc}
	.2byte 0x0000
	.section .text.x020089f8,"ax",%progbits
	.global Func_020009f8
	.thumb_func
Func_020009f8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #8
	ldr r3, .L_02008ae4
	str r1, [sp, #4]
	str r2, [sp, #0]
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	adds r5, r0, #0
	ldr r0, [r3]
	bl Object_GetById
	adds r6, r0, #0
	adds r0, r5, #0
	bl Object_GetById
	adds r7, r0, #0
	bl Func_02001254
	movs r0, #0
	bl Func_02001324
	ldr r3, [sp, #4]
	ldr r2, .L_02008ae8
	lsls r3, r3, #16
	mov r11, r3
	ldr r3, [r6, #8]
	movs r5, #128
	add r3, r11
	lsls r5, r5, #12
	ands r3, r2
	adds r1, r3, r5
	ldr r3, [sp, #0]
	mov r10, r2
	lsls r3, r3, #16
	mov r9, r3
	ldr r3, [r6, #16]
	adds r0, r6, #0
	add r3, r9
	ands r3, r2
	movs r2, #128
	lsls r2, r2, #9
	str r2, [r6, #48]
	movs r2, #128
	lsls r2, r2, #8
	adds r3, r3, r5
	mov r8, r2
	str r2, [r6, #52]
	ldr r2, [r6, #12]
	bl Func_0200121c
	adds r0, r6, #0
	movs r1, #27
	bl Func_02001214
	ldr r3, [r7, #8]
	mov r2, r10
	add r3, r11
	ands r3, r2
	adds r1, r3, r5
	ldr r3, [r7, #16]
	adds r0, r7, #0
	add r3, r9
	ands r3, r2
	movs r2, #128
	lsls r2, r2, #9
	str r2, [r7, #48]
	mov r2, r8
	adds r3, r3, r5
	str r2, [r7, #52]
	ldr r2, [r7, #12]
	bl Func_0200121c
	ldr r3, [sp, #4]
	cmp r3, #0
	blt .L_02008aa2
	ldr r2, [sp, #0]
	cmp r2, #0
	bge .L_02008aac
.L_02008aa2:
	adds r0, r7, #0
	movs r1, #4
	bl Func_02001214
	b .L_02008ab4
.L_02008aac:
	adds r0, r7, #0
	movs r1, #3
	bl Func_02001214
.L_02008ab4:
	movs r0, #226
	bl Func_02001344
	adds r0, r6, #0
	bl Func_02001224
	movs r1, #2
	adds r0, r7, #0
	bl Func_02001214
	movs r0, #149
	lsls r0, r0, #1
	bl Func_02001344
	bl Func_0200125c
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008ae4:
	.4byte gPartyState
.L_02008ae8:
	.4byte 0xfff00000
	.section .text.x02008aec,"ax",%progbits
	.global Func_02000aec
	.thumb_func
Func_02000aec:
	push {r5, r6, lr}
	movs r0, #8
	sub sp, #8
	bl Object_GetById
	ldr r6, [r0, #8]
	movs r0, #8
	bl Object_GetById
	movs r1, #32
	movs r2, #0
	ldr r5, [r0, #16]
	movs r0, #8
	bl Func_020009f8
	movs r0, #2
	bl WaitFrames
	movs r0, #8
	bl Object_GetById
	ldr r3, [r0, #8]
	asrs r5, r5, #20
	subs r5, #1
	asrs r6, r6, #20
	asrs r3, r3, #20
	str r3, [sp, #0]
	adds r0, r6, #0
	adds r1, r5, #0
	movs r2, #1
	movs r3, #3
	str r5, [sp, #4]
	bl Func_02001234
	movs r1, #0
	movs r2, #1
	movs r3, #3
	movs r0, #0
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl Func_02001234
	movs r0, #160
	lsls r0, r0, #2
	bl Func_0200120c
	add sp, #8
	pop {r5, r6, pc}
	.section .text.x02008b4c,"ax",%progbits
	.global Func_02000b4c
	.thumb_func
Func_02000b4c:
	push {r5, r6, lr}
	movs r0, #8
	sub sp, #8
	bl Object_GetById
	ldr r6, [r0, #8]
	movs r0, #8
	bl Object_GetById
	movs r1, #32
	negs r1, r1
	movs r2, #0
	ldr r5, [r0, #16]
	movs r0, #8
	bl Func_020009f8
	movs r0, #2
	bl WaitFrames
	movs r0, #8
	bl Object_GetById
	ldr r3, [r0, #8]
	asrs r5, r5, #20
	subs r5, #1
	asrs r6, r6, #20
	asrs r3, r3, #20
	str r3, [sp, #0]
	adds r0, r6, #0
	adds r1, r5, #0
	movs r2, #1
	movs r3, #3
	str r5, [sp, #4]
	bl Func_02001234
	movs r1, #0
	movs r2, #1
	movs r3, #3
	movs r0, #0
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl Func_02001234
	movs r0, #160
	lsls r0, r0, #2
	bl Func_02001204
	add sp, #8
	pop {r5, r6, pc}
	.2byte 0x0000
	.section .text.x02008bb0,"ax",%progbits
	.global Func_02000bb0
	.thumb_func
Func_02000bb0:
	push {r5, r6, lr}
	movs r0, #9
	sub sp, #8
	bl Object_GetById
	ldr r6, [r0, #8]
	movs r0, #9
	bl Object_GetById
	movs r1, #112
	ldr r5, [r0, #16]
	movs r2, #0
	movs r0, #9
	bl Func_020009f8
	movs r1, #96
	movs r2, #0
	movs r0, #9
	bl Func_020009f8
	movs r0, #2
	bl WaitFrames
	movs r0, #9
	bl Object_GetById
	ldr r3, [r0, #8]
	asrs r5, r5, #20
	subs r5, #1
	asrs r6, r6, #20
	asrs r3, r3, #20
	str r3, [sp, #0]
	adds r0, r6, #0
	adds r1, r5, #0
	movs r2, #1
	movs r3, #3
	str r5, [sp, #4]
	bl Func_02001234
	movs r1, #0
	movs r2, #1
	movs r3, #3
	movs r0, #0
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl Func_02001234
	movs r0, #193
	lsls r0, r0, #1
	adds r0, #255
	bl Func_0200120c
	add sp, #8
	pop {r5, r6, pc}
	.section .text.x02008c1c,"ax",%progbits
	.global Func_02000c1c
	.thumb_func
Func_02000c1c:
	push {r5, r6, lr}
	movs r0, #9
	sub sp, #8
	bl Object_GetById
	ldr r6, [r0, #8]
	movs r0, #9
	bl Object_GetById
	movs r1, #112
	ldr r5, [r0, #16]
	negs r1, r1
	movs r0, #9
	movs r2, #0
	bl Func_020009f8
	movs r1, #96
	negs r1, r1
	movs r2, #0
	movs r0, #9
	bl Func_020009f8
	movs r0, #2
	bl WaitFrames
	movs r0, #9
	bl Object_GetById
	ldr r3, [r0, #8]
	asrs r5, r5, #20
	subs r5, #1
	asrs r6, r6, #20
	asrs r3, r3, #20
	str r3, [sp, #0]
	adds r0, r6, #0
	adds r1, r5, #0
	movs r2, #1
	movs r3, #3
	str r5, [sp, #4]
	bl Func_02001234
	movs r1, #0
	movs r2, #1
	movs r3, #3
	movs r0, #0
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl Func_02001234
	movs r0, #193
	lsls r0, r0, #1
	adds r0, #255
	bl Func_02001204
	add sp, #8
	pop {r5, r6, pc}
	.section .text.x02008c8c,"ax",%progbits
	.global Func_02000c8c
	.thumb_func
Func_02000c8c:
	push {r5, r6, lr}
	movs r0, #10
	sub sp, #8
	bl Object_GetById
	ldr r6, [r0, #8]
	movs r0, #10
	bl Object_GetById
	movs r1, #80
	movs r2, #0
	ldr r5, [r0, #16]
	movs r0, #10
	bl Func_020009f8
	movs r0, #2
	bl WaitFrames
	movs r0, #10
	bl Object_GetById
	ldr r3, [r0, #8]
	asrs r5, r5, #20
	subs r5, #1
	asrs r6, r6, #20
	asrs r3, r3, #20
	str r3, [sp, #0]
	adds r0, r6, #0
	adds r1, r5, #0
	movs r2, #1
	movs r3, #3
	str r5, [sp, #4]
	bl Func_02001234
	movs r1, #0
	movs r2, #1
	movs r3, #3
	movs r0, #0
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl Func_02001234
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #130
	bl Func_02001204
	add sp, #8
	pop {r5, r6, pc}
	.2byte 0x0000
	.section .text.x02008cf0,"ax",%progbits
	.global Func_02000cf0
	.thumb_func
Func_02000cf0:
	push {r5, r6, lr}
	movs r0, #10
	sub sp, #8
	bl Object_GetById
	ldr r6, [r0, #8]
	movs r0, #10
	bl Object_GetById
	movs r1, #80
	negs r1, r1
	movs r2, #0
	ldr r5, [r0, #16]
	movs r0, #10
	bl Func_020009f8
	movs r0, #2
	bl WaitFrames
	movs r0, #10
	bl Object_GetById
	ldr r3, [r0, #8]
	asrs r5, r5, #20
	subs r5, #1
	asrs r6, r6, #20
	asrs r3, r3, #20
	str r3, [sp, #0]
	adds r0, r6, #0
	adds r1, r5, #0
	movs r2, #1
	movs r3, #3
	str r5, [sp, #4]
	bl Func_02001234
	movs r1, #0
	movs r2, #1
	movs r3, #3
	movs r0, #0
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl Func_02001234
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #130
	bl Func_0200120c
	add sp, #8
	pop {r5, r6, pc}
	.section .text.x02008d54,"ax",%progbits
	.global Func_02000d54
	.thumb_func
Func_02000d54:
	push {r5, r6, lr}
	movs r0, #11
	sub sp, #8
	bl Object_GetById
	ldr r6, [r0, #8]
	movs r0, #11
	bl Object_GetById
	movs r1, #80
	movs r2, #0
	ldr r5, [r0, #16]
	movs r0, #11
	bl Func_020009f8
	movs r0, #2
	bl WaitFrames
	movs r0, #11
	bl Object_GetById
	ldr r3, [r0, #8]
	asrs r5, r5, #20
	subs r5, #1
	asrs r6, r6, #20
	asrs r3, r3, #20
	str r3, [sp, #0]
	adds r0, r6, #0
	adds r1, r5, #0
	movs r2, #1
	movs r3, #3
	str r5, [sp, #4]
	bl Func_02001234
	movs r1, #0
	movs r2, #1
	movs r3, #3
	movs r0, #0
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl Func_02001234
	movs r0, #194
	lsls r0, r0, #1
	adds r0, #255
	bl Func_0200120c
	add sp, #8
	pop {r5, r6, pc}
	.2byte 0x0000
	.section .text.x02008db8,"ax",%progbits
	.global Func_02000db8
	.thumb_func
Func_02000db8:
	push {r5, r6, lr}
	movs r0, #11
	sub sp, #8
	bl Object_GetById
	ldr r6, [r0, #8]
	movs r0, #11
	bl Object_GetById
	movs r1, #80
	negs r1, r1
	movs r2, #0
	ldr r5, [r0, #16]
	movs r0, #11
	bl Func_020009f8
	movs r0, #2
	bl WaitFrames
	movs r0, #11
	bl Object_GetById
	ldr r3, [r0, #8]
	asrs r5, r5, #20
	subs r5, #1
	asrs r6, r6, #20
	asrs r3, r3, #20
	str r3, [sp, #0]
	adds r0, r6, #0
	adds r1, r5, #0
	movs r2, #1
	movs r3, #3
	str r5, [sp, #4]
	bl Func_02001234
	movs r1, #0
	movs r2, #1
	movs r3, #3
	movs r0, #0
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl Func_02001234
	movs r0, #194
	lsls r0, r0, #1
	adds r0, #255
	bl Func_0200120c
	add sp, #8
	pop {r5, r6, pc}
	.section .text.x02008e1c,"ax",%progbits
	.global Func_02000e1c
	.thumb_func
Func_02000e1c:
	push {r5, r6, lr}
	movs r0, #12
	sub sp, #8
	bl Object_GetById
	ldr r6, [r0, #8]
	movs r0, #12
	bl Object_GetById
	movs r1, #80
	movs r2, #0
	ldr r5, [r0, #16]
	movs r0, #12
	bl Func_020009f8
	movs r0, #2
	bl WaitFrames
	movs r0, #12
	bl Object_GetById
	ldr r3, [r0, #8]
	asrs r5, r5, #20
	subs r5, #1
	asrs r6, r6, #20
	asrs r3, r3, #20
	str r3, [sp, #0]
	adds r0, r6, #0
	adds r1, r5, #0
	movs r2, #1
	movs r3, #3
	str r5, [sp, #4]
	bl Func_02001234
	movs r1, #0
	movs r2, #1
	movs r3, #3
	movs r0, #0
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl Func_02001234
	movs r0, #161
	lsls r0, r0, #2
	bl Func_02001204
	add sp, #8
	pop {r5, r6, pc}
	.section .text.x02008e7c,"ax",%progbits
	.global Func_02000e7c
	.thumb_func
Func_02000e7c:
	push {r5, r6, lr}
	movs r0, #12
	sub sp, #8
	bl Object_GetById
	ldr r6, [r0, #8]
	movs r0, #12
	bl Object_GetById
	movs r1, #80
	negs r1, r1
	movs r2, #0
	ldr r5, [r0, #16]
	movs r0, #12
	bl Func_020009f8
	movs r0, #2
	bl WaitFrames
	movs r0, #12
	bl Object_GetById
	ldr r3, [r0, #8]
	asrs r5, r5, #20
	subs r5, #1
	asrs r6, r6, #20
	asrs r3, r3, #20
	str r3, [sp, #0]
	adds r0, r6, #0
	adds r1, r5, #0
	movs r2, #1
	movs r3, #3
	str r5, [sp, #4]
	bl Func_02001234
	movs r1, #0
	movs r2, #1
	movs r3, #3
	movs r0, #0
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl Func_02001234
	movs r0, #161
	lsls r0, r0, #2
	bl Func_0200120c
	add sp, #8
	pop {r5, r6, pc}
	.2byte 0x0000
	.section .text.x02008ee0,"ax",%progbits
	.global Func_02000ee0
	.thumb_func
Func_02000ee0:
	push {r5, r6, lr}
	movs r0, #13
	sub sp, #8
	bl Object_GetById
	ldr r6, [r0, #8]
	movs r0, #13
	bl Object_GetById
	movs r1, #80
	movs r2, #0
	ldr r5, [r0, #16]
	movs r0, #13
	bl Func_020009f8
	movs r0, #2
	bl WaitFrames
	movs r0, #13
	bl Object_GetById
	ldr r3, [r0, #8]
	asrs r5, r5, #20
	subs r5, #1
	asrs r6, r6, #20
	asrs r3, r3, #20
	str r3, [sp, #0]
	adds r0, r6, #0
	adds r1, r5, #0
	movs r2, #1
	movs r3, #3
	str r5, [sp, #4]
	bl Func_02001234
	movs r1, #0
	movs r2, #1
	movs r3, #3
	movs r0, #0
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl Func_02001234
	movs r0, #195
	lsls r0, r0, #1
	adds r0, #255
	bl Func_0200120c
	add sp, #8
	pop {r5, r6, pc}
	.2byte 0x0000
	.section .text.x02008f44,"ax",%progbits
	.global Func_02000f44
	.thumb_func
Func_02000f44:
	push {r5, r6, lr}
	movs r0, #13
	sub sp, #8
	bl Object_GetById
	ldr r6, [r0, #8]
	movs r0, #13
	bl Object_GetById
	movs r1, #80
	negs r1, r1
	movs r2, #0
	ldr r5, [r0, #16]
	movs r0, #13
	bl Func_020009f8
	movs r0, #2
	bl WaitFrames
	movs r0, #13
	bl Object_GetById
	ldr r3, [r0, #8]
	asrs r5, r5, #20
	subs r5, #1
	asrs r6, r6, #20
	asrs r3, r3, #20
	str r3, [sp, #0]
	adds r0, r6, #0
	adds r1, r5, #0
	movs r2, #1
	movs r3, #3
	str r5, [sp, #4]
	bl Func_02001234
	movs r1, #0
	movs r2, #1
	movs r3, #3
	movs r0, #0
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl Func_02001234
	movs r0, #195
	lsls r0, r0, #1
	adds r0, #255
	bl Func_02001204
	add sp, #8
	pop {r5, r6, pc}
	.section .text.x02008fa8,"ax",%progbits
	.global Func_02000fa8
	.thumb_func
Func_02000fa8:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	movs r0, #14
	sub sp, #8
	bl Object_GetById
	ldr r5, [r0, #8]
	movs r0, #14
	bl Object_GetById
	ldr r0, [r0, #16]
	movs r6, #112
	mov r8, r0
	negs r6, r6
	mov r3, r8
	asrs r3, r3, #20
	adds r2, r6, #0
	movs r0, #14
	movs r1, #0
	mov r8, r3
	bl Func_020009f8
	adds r2, r6, #0
	movs r0, #14
	movs r1, #0
	bl Func_020009f8
	movs r2, #128
	negs r2, r2
	movs r1, #0
	movs r0, #14
	bl Func_020009f8
	movs r0, #2
	bl WaitFrames
	movs r0, #14
	bl Object_GetById
	ldr r3, [r0, #16]
	asrs r5, r5, #20
	subs r5, #1
	asrs r3, r3, #20
	str r3, [sp, #4]
	adds r0, r5, #0
	mov r1, r8
	movs r2, #3
	movs r3, #1
	str r5, [sp, #0]
	bl Func_02001234
	mov r3, r8
	str r3, [sp, #4]
	movs r1, #0
	movs r2, #3
	movs r3, #1
	movs r0, #0
	str r5, [sp, #0]
	bl Func_02001234
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #134
	bl Func_02001204
	add sp, #8
	pop {r3}
	mov r8, r3
	pop {r5, r6, pc}
	.section .text.x02009034,"ax",%progbits
	.global Func_02001034
	.thumb_func
Func_02001034:
	push {r5, r6, lr}
	movs r0, #14
	sub sp, #8
	bl Object_GetById
	ldr r5, [r0, #8]
	movs r0, #14
	bl Object_GetById
	movs r1, #0
	ldr r6, [r0, #16]
	movs r2, #112
	movs r0, #14
	bl Func_020009f8
	movs r0, #14
	movs r1, #0
	movs r2, #112
	bl Func_020009f8
	movs r1, #0
	movs r2, #128
	movs r0, #14
	bl Func_020009f8
	movs r0, #2
	bl WaitFrames
	movs r0, #14
	bl Object_GetById
	ldr r3, [r0, #16]
	asrs r5, r5, #20
	subs r5, #1
	asrs r6, r6, #20
	asrs r3, r3, #20
	str r3, [sp, #4]
	adds r0, r5, #0
	adds r1, r6, #0
	movs r2, #3
	movs r3, #1
	str r5, [sp, #0]
	bl Func_02001234
	movs r1, #0
	movs r2, #3
	movs r3, #1
	movs r0, #0
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_02001234
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #134
	bl Func_0200120c
	add sp, #8
	pop {r5, r6, pc}
	.2byte 0x0000
	.section .text.x020090ac,"ax",%progbits
	.global Func_020010ac
	.thumb_func
Func_020010ac:
	push {r5, lr}
	bl Func_02001254
	movs r0, #0
	bl Func_02001324
	movs r1, #2
	movs r0, #8
	bl ObjectMotion_SetVariantCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #0
	bl Func_020012f4
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #1
	adds r0, #3
	bl Func_020012ec
	movs r0, #60
	bl Func_020012fc
	movs r0, #60
	bl Battle_WaitMode0
	movs r2, #2
	ldr r0, .L_02009150
	movs r1, #0
	bl Func_02001244
	movs r0, #128
	movs r1, #0
	lsls r0, r0, #9
	bl Func_020012ec
	movs r0, #60
	bl Func_020012fc
	movs r0, #60
	bl Battle_WaitMode0
	ldr r5, .L_02009154
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r5, r2
	ldr r0, [r3]
	bl Object_GetById
	cmp r0, #0
	beq .L_02009128
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #8
	bl ObjectMotion_ResetAndSetPosition
.L_02009128:
	movs r0, #20
	bl Battle_WaitMode0
	ldr r0, .L_02009158
	movs r1, #2
	bl Func_020012e4
	movs r3, #166
	lsls r3, r3, #1
	adds r3, #255
	adds r2, r5, r3
	movs r3, #2
	strb r3, [r2]
	movs r0, #103
	movs r1, #2
	bl Func_020012d4
	bl Func_0200125c
	pop {r5, pc}
.L_02009150:
	.4byte 0x000030aa
.L_02009154:
	.4byte gPartyState
.L_02009158:
	.4byte 0x0000008a
	.section .text.x0200915c,"ax",%progbits
	.global Func_0200115c
	.thumb_func
Func_0200115c:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	movs r0, #140
	lsls r0, r0, #2
	bl Func_020011fc
	cmp r0, #0
	bne .L_020091d2
	movs r0, #9
	bl Object_GetById
	ldr r5, .L_020091d8
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	adds r6, r0, #0
	ldr r0, [r5]
	bl Object_GetById
	mov r8, r0
	movs r0, #140
	lsls r0, r0, #2
	bl Func_02001204
	movs r1, #148
	movs r2, #160
	movs r0, #9
	lsls r1, r1, #17
	lsls r2, r2, #16
	bl Func_0200129c
	movs r3, #192
	lsls r3, r3, #9
	str r3, [r6, #48]
	movs r3, #240
	lsls r3, r3, #12
	str r3, [r6, #40]
	mov r3, r8
	ldr r2, [r3, #16]
	movs r1, #172
	asrs r2, r2, #19
	lsls r2, r2, #3
	adds r2, #24
	lsls r1, r1, #1
	movs r0, #9
	bl ObjectMotion_SetPositionAndCommit
	movs r0, #8
	bl ObjectMotion_EnableActionAndResetMotion
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r6, #48]
	movs r1, #9
	ldr r0, [r5]
	movs r2, #0
	bl Object_LinkPair
.L_020091d2:
	pop {r3}
	mov r8, r3
	pop {r5, r6, pc}
.L_020091d8:
	.4byte gPartyState
	.section .rodata.x0200934c,"a",%progbits
	.global Data_0200134c
Data_0200134c:
	.4byte 0x000a0009
	.4byte 0x000c000b
	.4byte 0x000e000d
	.4byte 0xffff000f
	.global Data_0200135c
Data_0200135c:
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
	.global Data_02001388
Data_02001388:
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
	.global Data_020013b4
Data_020013b4:
	.4byte 0x0009000e
	.4byte 0x000b000a
	.4byte 0x000d000c
	.4byte 0x000e000d
	.4byte 0x000a0009
	.4byte 0x000c000b
	.4byte 0x000d000c
	.4byte 0x0009000e
	.4byte 0x000b000a
	.4byte 0x000c000b
	.4byte 0x000e000d
	.4byte 0x000a0009
	.4byte 0x000b000a
	.4byte 0x000d000c
	.4byte 0x0009000e
	.4byte 0x000a0009
	.4byte 0x000c000b
	.4byte 0x000e000d
.L_020093fc:
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x00014ccc
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00014ccc
	.4byte 0x00000011
.L_02009418:
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000000
	.4byte 0x00000017
	.4byte 0x00000006
	.4byte 0xfffc0000
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x0000cccc
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x0000cccc
	.4byte 0x0000002e
	.4byte Func_020003c8
	.4byte 0x00000011
	.global Data_02001454
Data_02001454:
	.4byte 0x0000002e
	.4byte Func_020003d4
	.4byte 0x00000011
	.global Data_02001460
Data_02001460:
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
	.global Data_02001490
Data_02001490:
	.4byte 0x00000088
	.4byte 0x10105087
	.4byte 0xffffffff
	.4byte 0x10203088
	.4byte 0xffffffff
	.4byte 0x10302088
	.4byte 0xffffffff
	.4byte 0x10405088
	.4byte 0xffffffff
	.4byte 0x10504088
	.4byte 0xffffffff
	.4byte 0x10607088
	.4byte 0xffffffff
	.4byte 0x10706088
	.4byte 0xffffffff
	.4byte 0x10801089
	.4byte 0xffffffff
	.4byte 0x1090a088
	.4byte 0xffffffff
	.4byte 0x10a09088
	.4byte 0xffffffff
	.4byte 0x00000089
	.4byte 0x10108088
	.4byte 0xffffffff
	.4byte 0x10203089
	.4byte 0xffffffff
	.4byte 0x10302089
	.4byte 0xffffffff
	.4byte 0x10405089
	.4byte 0xffffffff
	.4byte 0x10504089
	.4byte 0xffffffff
	.4byte 0x10607089
	.4byte 0xffffffff
	.4byte 0x10706089
	.4byte 0xffffffff
	.4byte 0x10809089
	.4byte 0xffffffff
	.4byte 0x10908089
	.4byte 0xffffffff
	.4byte 0x10a0108a
	.4byte 0xffffffff
	.4byte 0x0000008a
	.4byte 0x1010a089
	.4byte 0xffffffff
	.4byte 0x000001ff
	.global Data_02001548
Data_02001548:
	.4byte 0x09cf00aa
	.4byte .L_020093fc
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00024000
	.4byte 0xffff0132
	.4byte .L_02009418
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00900000
	.4byte 0x0002c000
	.4byte 0xffff0132
	.4byte .L_02009418
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x00900000
	.4byte 0x0102c000
	.4byte 0xffff0132
	.4byte .L_02009418
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x00b00000
	.4byte 0x0102c000
	.4byte 0xffff0132
	.4byte .L_02009418
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x00b00000
	.4byte 0x0102c000
	.4byte 0xffff0132
	.4byte .L_02009418
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00e00000
	.4byte 0x0102c000
	.4byte 0xffff0132
	.4byte .L_02009418
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x00e00000
	.4byte 0x0102c000
	.4byte 0xffff0132
	.4byte .L_02009418
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x01000000
	.4byte 0x0102c000
	.4byte 0xffff0132
	.4byte .L_02009418
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x01000000
	.4byte 0x0102c000
	.4byte 0xffff0132
	.4byte .L_02009418
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x01300000
	.4byte 0x0102c000
	.4byte 0xffff0132
	.4byte .L_02009418
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x01300000
	.4byte 0x0102c000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02001668
Data_02001668:
	.4byte 0xffff012a
	.4byte 0x00000007
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x01f80000
	.4byte 0x00024000
	.4byte 0xffff012a
	.4byte 0x00000007
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00024000
	.4byte 0xffff012a
	.4byte 0x00000007
	.4byte 0x03080000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00024000
	.4byte 0xffff012a
	.4byte 0x00000007
	.4byte 0x03680000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00024000
	.4byte 0xffff012a
	.4byte 0x00000007
	.4byte 0x03080000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x00024000
	.4byte 0xffff012a
	.4byte 0x00000007
	.4byte 0x03680000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x00024000
	.4byte 0xffff0129
	.4byte 0x00000007
	.4byte 0x03380000
	.4byte 0x00000000
	.4byte 0x01d80000
	.4byte 0x00024000
	.4byte 0x003c00f3
	.4byte 0x00000001
	.4byte 0x03380000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02001740
Data_02001740:
	.4byte 0xffff00fc
	.4byte 0x00000001
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00024000
	.4byte 0x005500f4
	.4byte 0x00000001
	.4byte 0x01500000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02001788
Data_02001788:
	.4byte 0x00000021
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000031
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000021
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
	.4byte 0x00000602
	.4byte 0xffff0014
	.4byte Func_02000aec
	.4byte 0x00008602
	.4byte 0xffff0014
	.4byte Func_02000b4c
	.4byte 0x00000602
	.4byte 0xffff0015
	.4byte Func_02000bb0
	.4byte 0x00008602
	.4byte 0xffff0015
	.4byte Func_02000c1c
	.4byte 0x00000602
	.4byte 0xffff0016
	.4byte Func_02000c8c
	.4byte 0x00008602
	.4byte 0xffff0016
	.4byte Func_02000cf0
	.4byte 0x00000602
	.4byte 0xffff0017
	.4byte Func_02000d54
	.4byte 0x00008602
	.4byte 0xffff0017
	.4byte Func_02000db8
	.4byte 0x00000602
	.4byte 0xffff0018
	.4byte Func_02000e1c
	.4byte 0x00008602
	.4byte 0xffff0018
	.4byte Func_02000e7c
	.4byte 0x00000602
	.4byte 0xffff0019
	.4byte Func_02000ee0
	.4byte 0x00008602
	.4byte 0xffff0019
	.4byte Func_02000f44
	.4byte 0x0000c602
	.4byte 0xffff001a
	.4byte Func_02000fa8
	.4byte 0x00004602
	.4byte 0xffff001a
	.4byte Func_02001034
	.4byte 0x00000002
	.4byte 0xffff0014
	.4byte Func_020009a4
	.4byte 0x00000002
	.4byte 0xffff0015
	.4byte Func_020009b0
	.4byte 0x00000002
	.4byte 0xffff0016
	.4byte Func_020009bc
	.4byte 0x00000002
	.4byte 0xffff0017
	.4byte Func_020009c8
	.4byte 0x00000002
	.4byte 0xffff0018
	.4byte Func_020009d4
	.4byte 0x00000002
	.4byte 0xffff0019
	.4byte Func_020009e0
	.4byte 0x00000002
	.4byte 0xffff001a
	.4byte Func_020009ec
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte Func_020003b0
	.4byte 0x00009c05
	.4byte 0xffff0032
	.4byte Func_02000508
	.4byte 0x00009c05
	.4byte 0xffff0033
	.4byte Func_02000544
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02001914
Data_02001914:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x0000c602
	.4byte 0xffff0002
	.4byte Func_020005fc
	.4byte 0x0000c602
	.4byte 0xffff0004
	.4byte Func_020005fc
	.4byte 0x0000c602
	.4byte 0xffff0006
	.4byte Func_020005fc
	.4byte 0x0000c602
	.4byte 0xffff0008
	.4byte Func_020005fc
	.4byte 0x0000c602
	.4byte 0xffff000a
	.4byte Func_020005fc
	.4byte 0x00008715
	.4byte 0x02300008
	.4byte Func_0200115c
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte Func_020003bc
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020019b0
Data_020019b0:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x09cf000a
	.4byte Func_020010ac
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
.L_020099d4:
	.4byte 0x00040048
	.4byte 0x00020002
	.4byte 0x00490002
	.4byte 0x00020004
	.4byte 0x00020002
	.4byte 0x0000ffff
	.global Data_020019ec
Data_020019ec:
	.4byte .L_020099d4
	.4byte 0x00040048
	.4byte .L_020099d4
	.4byte 0x00040054
	.4byte .L_020099d4
	.4byte 0x00040060
	.4byte .L_020099d4
	.4byte 0x0004006b
	.4byte .L_020099d4
	.4byte 0x00040076
