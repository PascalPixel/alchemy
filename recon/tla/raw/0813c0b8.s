.syntax unified
	.thumb
	.global Func_0813c0b8
	.thumb_func
Func_0813c0b8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r5, #192
	lsls r5, r5, #18
	mov r10, r0
	ldr r0, [r5, #92]
	sub sp, #36
	str r0, [sp, #32]
	movs r0, #128
	ldr r1, [r5, #96]
	lsls r0, r0, #6
	adds r0, #1
	str r1, [sp, #28]
	bl BattleFx_BeginCanvasLayer
	ldr r3, .L_0813c11c
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #32
	strh r3, [r2]
	ldr r2, [sp, #32]
	movs r3, #208
	lsls r3, r3, #4
	adds r3, #4
	adds r1, r2, r3
	ldr r0, .L_0813c124
	movs r2, #1
	movs r3, #1
	bl Resource_LoadAndDecompress
	ldr r4, [sp, #32]
	movs r7, #224
	lsls r7, r7, #3
	adds r1, r4, r7
	ldr r0, .L_0813c128
	movs r2, #0
	movs r3, #0
	bl Resource_LoadAndDecompress
	bl Func_0813ba50
	ldr r3, .L_0813c120
	movs r2, #128
	lsls r2, r2, #19
	b .L_0813c12c
.L_0813c11c:
	.4byte 0x00000100
.L_0813c120:
	.4byte 0x00003f44
.L_0813c124:
	.4byte 0x00000190
.L_0813c128:
	.4byte 0x00000137
.L_0813c12c:
	adds r2, #80
	strh r3, [r2]
	ldr r3, .L_0813c160
	subs r2, #8
	strh r3, [r2]
	movs r1, #18
	movs r0, #104
	bl Func_081963ec
	ldr r0, [r5, #104]
	movs r1, #2
	str r0, [sp, #16]
	movs r0, #188
	bl Func_081963ec
	adds r5, #188
	ldr r5, [r5]
	ldr r3, .L_0813c164
	str r5, [sp, #20]
	movs r1, #0
	movs r2, #128
	mov r8, r1
	lsls r2, r2, #2
	subs r1, #1
	b .L_0813c168
	.2byte 0x0000
.L_0813c160:
	.4byte 0x00003337
.L_0813c164:
	.4byte Data_02010158
.L_0813c168:
	movs r4, #1
	add r8, r4
	str r1, [r3]
	adds r3, #28
	cmp r8, r2
	bne .L_0813c168
	movs r5, #0
	mov r8, r5
	ldr r6, .L_0813c218
	ldr r5, [sp, #32]
.L_0813c17c:
	bl Random16
	mov r7, r10
	movs r3, #63
	ands r0, r3
	ldr r3, [r7, #24]
	lsls r3, r3, #2
	adds r3, #2
	ldrb r3, [r6, r3]
	mov r1, r8
	muls r1, r3
	adds r3, r1, #0
	adds r3, #16
	negs r2, r3
	ldr r3, [r7, #4]
	cmp r3, #1
	bne .L_0813c1ac
	lsrs r3, r2, #31
	adds r3, r2, r3
	asrs r3, r3, #1
	adds r3, r0, r3
	adds r0, r3, #0
	subs r0, #48
	b .L_0813c1b8
.L_0813c1ac:
	lsrs r3, r2, #31
	adds r3, r2, r3
	asrs r3, r3, #1
	subs r3, r0, r3
	adds r0, r3, #0
	adds r0, #72
.L_0813c1b8:
	lsls r3, r0, #3
	str r3, [r5]
	lsls r3, r2, #3
	str r3, [r5, #4]
	movs r2, #1
	movs r3, #1
	negs r3, r3
	add r8, r2
	str r3, [r5, #24]
	mov r3, r8
	adds r5, #28
	cmp r3, #64
	bne .L_0813c17c
	mov r4, r10
	ldr r3, [r4, #4]
	cmp r3, #0
	bne .L_0813c238
	movs r5, #0
	mov r8, r5
	ldr r1, .L_0813c21c
	ldr r5, .L_0813c20c
	ldr r4, .L_0813c210
	ldr r0, .L_0813c214
.L_0813c1e6:
	mov r3, r8
	subs r3, #8
	cmp r3, #95
	bhi .L_0813c202
	mov r7, r8
	lsrs r3, r7, #31
	add r3, r8
	asrs r3, r3, #1
	subs r2, r5, r3
	lsls r2, r2, #8
	subs r3, r4, r3
	orrs r2, r3
	strh r2, [r1]
	b .L_0813c224
.L_0813c202:
	mov r2, r8
	cmp r2, #135
	bgt .L_0813c220
	strh r0, [r1]
	b .L_0813c224
.L_0813c20c:
	.4byte 0x00000034
.L_0813c210:
	.4byte 0x000000b4
.L_0813c214:
	.4byte 0x00000080
.L_0813c218:
	.4byte Data_08197527
.L_0813c21c:
	.4byte gMapCellBuffer
.L_0813c220:
	ldr r3, .L_0813c234
	strh r3, [r1]
.L_0813c224:
	movs r3, #1
	add r8, r3
	mov r7, r8
	adds r1, #2
	cmp r7, #160
	bne .L_0813c1e6
	b .L_0813c286
	.2byte 0x0000
.L_0813c234:
	.4byte 0x00000100
.L_0813c238:
	movs r0, #0
	mov r8, r0
	ldr r4, .L_0813c26c
	ldr r0, .L_0813c270
	ldr r1, .L_0813c274
.L_0813c242:
	mov r3, r8
	subs r3, #8
	cmp r3, #95
	bhi .L_0813c260
	mov r2, r8
	lsrs r3, r2, #31
	add r3, r8
	asrs r3, r3, #1
	adds r2, r3, #0
	adds r2, #60
	lsls r2, r2, #8
	adds r3, #188
	orrs r2, r3
	strh r2, [r1]
	b .L_0813c27a
.L_0813c260:
	mov r3, r8
	cmp r3, #135
	bgt .L_0813c278
	strh r4, [r1]
	b .L_0813c27a
	.2byte 0x0000
.L_0813c26c:
	.4byte 0x000070f0
.L_0813c270:
	.4byte 0x00000100
.L_0813c274:
	.4byte gMapCellBuffer
.L_0813c278:
	strh r0, [r1]
.L_0813c27a:
	movs r5, #1
	add r8, r5
	mov r7, r8
	adds r1, #2
	cmp r7, #160
	bne .L_0813c242
.L_0813c286:
	movs r1, #200
	lsls r1, r1, #4
	ldr r0, .L_0813c594
	bl Scheduler_AddOrUpdateCallback
	mov r0, r10
	ldr r1, [r0, #24]
	cmp r1, #0
	bne .L_0813c2b2
	ldr r2, [sp, #32]
	movs r4, #239
	lsls r4, r4, #7
	adds r3, r2, r4
	movs r2, #1
	str r2, [r3]
	ldr r5, [sp, #32]
	movs r7, #238
	lsls r7, r7, #7
	adds r7, #132
	adds r3, r5, r7
	str r1, [r3]
	b .L_0813c2e6
.L_0813c2b2:
	cmp r1, #1
	bne .L_0813c2ce
	ldr r0, [sp, #32]
	movs r1, #239
	lsls r1, r1, #7
	adds r2, r0, r1
	movs r3, #2
	str r3, [r2]
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #132
	adds r2, r0, r3
	movs r3, #50
	b .L_0813c2e4
.L_0813c2ce:
	ldr r4, [sp, #32]
	movs r5, #239
	movs r7, #238
	lsls r5, r5, #7
	lsls r7, r7, #7
	adds r2, r4, r5
	movs r3, #2
	adds r7, #132
	str r3, [r2]
	adds r2, r4, r7
	movs r3, #75
.L_0813c2e4:
	str r3, [r2]
.L_0813c2e6:
	movs r1, #200
	lsls r1, r1, #4
	ldr r0, .L_0813c598
	bl Scheduler_AddOrUpdateCallback
	movs r0, #0
	str r0, [sp, #24]
	mov r2, r10
	ldr r1, [r2, #24]
	ldr r5, .L_0813c59c
	adds r2, r1, #0
	lsls r3, r2, #2
	adds r3, #3
	ldrb r3, [r5, r3]
	cmp r3, #0
	bne .L_0813c308
	b .L_0813c564
.L_0813c308:
	lsls r3, r2, #2
	adds r3, #3
	ldrb r3, [r5, r3]
	ldr r4, [sp, #24]
	subs r3, #64
	cmp r4, r3
	bne .L_0813c320
	movs r0, #133
	mov r7, r10
	bl Func_081180e8
	ldr r1, [r7, #24]
.L_0813c320:
	lsls r3, r1, #2
	ldrb r3, [r5, r3]
	movs r0, #0
	mov r8, r0
	cmp r3, #0
	bne .L_0813c32e
	b .L_0813c4d0
.L_0813c32e:
	ldr r6, [sp, #32]
.L_0813c330:
	ldr r2, [r6]
	cmp r2, #0
	bge .L_0813c338
	adds r2, #7
.L_0813c338:
	ldr r3, [r6, #4]
	asrs r4, r2, #3
	cmp r3, #0
	bge .L_0813c342
	adds r3, #7
.L_0813c342:
	asrs r5, r3, #3
	ldr r3, [r6, #24]
	movs r1, #1
	negs r1, r1
	adds r2, r3, #0
	cmp r3, r1
	beq .L_0813c352
	b .L_0813c470
.L_0813c352:
	movs r3, #24
	ldr r2, [sp, #32]
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r3, #208
	lsls r3, r3, #4
	adds r3, #4
	adds r1, r2, r3
	str r4, [sp, #8]
	adds r2, r4, #0
	adds r3, r5, #0
	ldr r0, [sp, #28]
	ldr r7, [sp, #16]
	mov lr, r7
	.2byte 0xf800
	movs r0, #192
	ldr r3, [r6, #4]
	lsls r0, r0, #1
	adds r0, #255
	ldr r4, [sp, #8]
	cmp r3, r0
	bgt .L_0813c39a
	mov r1, r10
	ldr r3, [r1, #4]
	cmp r3, #0
	bne .L_0813c38c
	ldr r3, [r6]
	subs r3, #32
	b .L_0813c390
.L_0813c38c:
	ldr r3, [r6]
	adds r3, #32
.L_0813c390:
	str r3, [r6]
	ldr r3, [r6, #4]
	adds r3, #64
	str r3, [r6, #4]
	b .L_0813c4ba
.L_0813c39a:
	movs r3, #0
	str r3, [r6, #24]
	mov r3, r10
	ldr r2, [r3, #24]
	ldr r1, .L_0813c59c
	lsls r3, r2, #2
	adds r3, #1
	ldrb r3, [r1, r3]
	movs r7, #0
	cmp r3, #0
	beq .L_0813c434
	adds r4, #12
	str r4, [sp, #12]
	lsls r5, r5, #16
	movs r4, #255
	mov r11, r5
	mov r9, r4
.L_0813c3bc:
	lsls r3, r2, #2
	adds r3, #1
	ldrb r3, [r1, r3]
	ldr r0, .L_0813c5a0
	mov r2, r8
	muls r2, r3
	adds r2, r2, r7
	ldr r1, [sp, #12]
	lsls r3, r2, #3
	subs r3, r3, r2
	lsls r3, r3, #2
	adds r5, r3, r0
	mov r2, r11
	lsls r3, r1, #16
	str r3, [r5]
	str r2, [r5, #4]
	bl Random16
	mov r3, r9
	ands r0, r3
	subs r0, #128
	lsls r0, r0, #9
	str r0, [r5, #12]
	mov r4, r10
	ldr r3, [r4, #24]
	cmp r3, #2
	bne .L_0813c408
	bl Random16
	movs r3, #128
	lsls r3, r3, #1
	adds r3, #255
	ands r3, r0
	ldr r0, .L_0813c5a4
	adds r3, r3, r0
	lsls r3, r3, #10
	str r3, [r5, #16]
	b .L_0813c416
.L_0813c408:
	bl Random16
	mov r1, r9
	ands r0, r1
	subs r0, #255
	lsls r0, r0, #10
	str r0, [r5, #16]
.L_0813c416:
	bl Random16
	movs r3, #15
	ands r3, r0
	adds r3, #16
	str r3, [r5, #24]
	mov r3, r10
	ldr r2, [r3, #24]
	ldr r1, .L_0813c59c
	lsls r3, r2, #2
	adds r3, #1
	ldrb r3, [r1, r3]
	adds r7, #1
	cmp r7, r3
	bne .L_0813c3bc
.L_0813c434:
	movs r3, #3
	mov r4, r8
	ands r3, r4
	cmp r3, #0
	bne .L_0813c444
	movs r0, #132
	bl Audio_PlayCue
.L_0813c444:
	mov r5, r10
	ldr r3, [r5, #20]
	movs r7, #0
	cmp r3, #0
	beq .L_0813c4ba
	movs r5, #36
.L_0813c450:
	mov r1, r10
	movs r3, #2
	ldrsh r0, [r5, r1]
	str r3, [sp, #0]
	movs r1, #7
	adds r3, r7, #0
	movs r2, #5
	bl Func_0814cd48
	mov r4, r10
	ldr r3, [r4, #20]
	adds r7, #1
	adds r5, #2
	cmp r7, r3
	bne .L_0813c450
	b .L_0813c4ba
.L_0813c470:
	cmp r2, #3
	bhi .L_0813c48c
	ldr r7, [sp, #32]
	movs r2, #240
	lsls r2, r2, #4
	movs r3, #24
	adds r2, #68
	str r3, [sp, #0]
	str r3, [sp, #4]
	adds r1, r7, r2
	adds r3, r5, #0
	adds r2, r4, #0
	ldr r0, [sp, #28]
	b .L_0813c4aa
.L_0813c48c:
	cmp r2, #7
	bgt .L_0813c4b2
	adds r3, r5, #0
	movs r7, #136
	ldr r5, [sp, #32]
	lsls r7, r7, #5
	movs r1, #42
	adds r2, r4, #0
	adds r7, #132
	subs r3, #9
	str r1, [sp, #0]
	str r1, [sp, #4]
	subs r2, #9
	ldr r0, [sp, #28]
	adds r1, r5, r7
.L_0813c4aa:
	ldr r4, [sp, #16]
	mov lr, r4
	.2byte 0xf800
	ldr r3, [r6, #24]
.L_0813c4b2:
	cmp r3, #14
	bgt .L_0813c4ba
	adds r3, #1
	str r3, [r6, #24]
.L_0813c4ba:
	mov r7, r10
	ldr r3, [r7, #24]
	ldr r0, .L_0813c59c
	lsls r3, r3, #2
	ldrb r3, [r0, r3]
	movs r5, #1
	add r8, r5
	adds r6, #28
	cmp r8, r3
	beq .L_0813c4d0
	b .L_0813c330
.L_0813c4d0:
	ldr r6, .L_0813c5a8
	ldr r5, .L_0813c5a0
	movs r1, #0
	mov r8, r1
.L_0813c4d8:
	ldr r0, [r5, #24]
	movs r2, #1
	negs r2, r2
	cmp r0, r2
	beq .L_0813c524
	adds r4, r0, #1
	cmp r4, #6
	ble .L_0813c4ea
	movs r4, #6
.L_0813c4ea:
	lsls r0, r4, #1
	subs r3, r0, #2
	ldrh r1, [r6, r3]
	ldr r3, [sp, #32]
	movs r7, #224
	adds r1, r3, r1
	lsls r7, r7, #3
	movs r3, #2
	ldrsh r2, [r5, r3]
	adds r1, r1, r7
	movs r7, #6
	ldrsh r3, [r5, r7]
	subs r2, r2, r4
	subs r3, r3, r4
	str r0, [sp, #0]
	str r0, [sp, #4]
	ldr r4, [sp, #20]
	ldr r0, [sp, #28]
	mov lr, r4
	.2byte 0xf800
	movs r2, #128
	adds r0, r5, #0
	movs r1, #60
	lsls r2, r2, #6
	bl BattleFxKernels_IntegrateVector2
	ldr r3, [r5, #24]
	subs r3, #1
	str r3, [r5, #24]
.L_0813c524:
	movs r7, #1
	movs r0, #128
	add r8, r7
	lsls r0, r0, #2
	adds r5, #28
	cmp r8, r0
	bne .L_0813c4d8
	bl Func_081434f8
	movs r3, #240
	ldr r1, [sp, #32]
	lsls r3, r3, #7
	adds r3, #232
	adds r2, r1, r3
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r4, [sp, #24]
	mov r7, r10
	adds r4, #1
	str r4, [sp, #24]
	ldr r2, [r7, #24]
	ldr r5, .L_0813c59c
	lsls r3, r2, #2
	adds r3, #3
	ldrb r3, [r5, r3]
	adds r1, r2, #0
	cmp r4, r3
	beq .L_0813c564
	b .L_0813c308
.L_0813c564:
	ldr r0, .L_0813c594
	bl Scheduler_RemoveCallback
	ldr r0, .L_0813c598
	bl Scheduler_RemoveCallback
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	bl Func_08143bb8
	bl Func_0813ba50
	add sp, #36
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0813c594:
	.4byte Func_0813bba0
.L_0813c598:
	.4byte Func_08143000
.L_0813c59c:
	.4byte Data_08197527
.L_0813c5a0:
	.4byte Data_02010140
.L_0813c5a4:
	.4byte 0xfffffe80
.L_0813c5a8:
	.4byte Data_08197424
