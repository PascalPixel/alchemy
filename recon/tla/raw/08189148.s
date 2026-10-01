.syntax unified
	.thumb
	.global Func_08189148
	.thumb_func
Func_08189148:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #248
	str r0, [sp, #52]
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #96]
	movs r2, #0
	ldr r0, [r3, #92]
	str r1, [sp, #48]
	str r2, [sp, #40]
	str r2, [sp, #32]
	str r2, [sp, #28]
	mov r11, r0
	ldr r3, [r3, #100]
	movs r0, #0
	str r3, [sp, #24]
	bl BattleFx_BeginCanvasLayer
	ldr r3, .L_081891ac
	movs r2, #128
	lsls r2, r2, #19
	movs r1, #224
	adds r2, #82
	lsls r1, r1, #3
	strh r3, [r2]
	ldr r0, .L_081891b0
	movs r3, #1
	add r1, r11
	movs r2, #1
	bl Resource_LoadAndDecompress
	ldr r4, .L_081891b4
	movs r6, #234
	movs r5, #236
	movs r3, #1
	lsls r6, r6, #2
	lsls r5, r5, #5
	mov r7, r11
	mov r12, r4
	mov r8, r3
	mov lr, r5
	adds r4, r7, r6
	b .L_081891b8
	.2byte 0x0000
.L_081891ac:
	.4byte 0x00001010
.L_081891b0:
	.4byte 0x00000186
.L_081891b4:
	.4byte 0xfffff8f0
.L_081891b8:
	mov r1, r8
	mov r2, r12
	lsls r0, r1, #2
	adds r1, r4, r2
	mov r2, r11
	movs r5, #0
	add r2, lr
.L_081891c6:
	mov r7, r8
	ldrb r3, [r2]
	adds r2, #1
	cmp r7, #10
	ble .L_081891dc
	subs r3, r3, r0
	adds r3, #40
	cmp r3, #0
	bge .L_081891da
	movs r3, #0
.L_081891da:
	strb r3, [r1]
.L_081891dc:
	adds r5, #1
	adds r1, #1
	cmp r5, r6
	bne .L_081891c6
	movs r1, #1
	movs r0, #234
	add r8, r1
	lsls r0, r0, #2
	mov r2, r8
	adds r4, r4, r0
	cmp r2, #20
	bne .L_081891b8
	ldr r1, [sp, #24]
	ldr r0, .L_08189404
	movs r2, #0
	movs r3, #0
	bl Resource_LoadAndDecompress
	movs r1, #134
	lsls r1, r1, #7
	ldr r0, .L_08189408
	add r1, r11
	movs r2, #1
	movs r3, #1
	bl Resource_LoadAndDecompress
	movs r3, #0
	ldr r0, .L_0818940c
	ldr r1, .L_08189410
	movs r2, #1
	bl Resource_LoadAndDecompress
	ldr r4, [sp, #52]
	ldr r3, [r4, #24]
	cmp r3, #1
	bne .L_08189238
	ldr r0, .L_08189414
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_08189418
	lsls r0, r0, #19
	movs r2, #128
	mov lr, r3
	.2byte 0xf800
.L_08189238:
	ldr r5, [sp, #52]
	mov r7, sp
	adds r7, #56
	ldr r0, [r5, #4]
	adds r1, r7, #0
	str r7, [sp, #20]
	bl Func_08144aac
	movs r2, #239
	lsls r2, r2, #7
	add r2, r11
	movs r3, #2
	str r3, [r2]
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #132
	add r2, r11
	movs r3, #75
	movs r1, #200
	str r3, [r2]
	lsls r1, r1, #4
	ldr r0, .L_0818941c
	bl Scheduler_AddOrUpdateCallback
	movs r1, #36
	ldrsh r0, [r5, r1]
	bl GetBattleObjectSlotFar
	ldr r7, [r0]
	movs r2, #0
	mov r5, r11
	mov r8, r2
	movs r6, #63
	adds r5, #224
.L_0818927c:
	ldr r3, [r7, #8]
	ldr r2, .L_08189420
	str r3, [r5]
	movs r3, #0
	str r3, [r5, #4]
	ldr r3, [r7, #16]
	str r3, [r5, #8]
	ldr r4, [sp, #52]
	ldr r3, [r4, #24]
	ldrb r3, [r2, r3]
	cmp r8, r3
	blt .L_081892aa
	bl Random16
	ands r0, r6
	subs r0, #32
	lsls r0, r0, #14
	str r0, [r5, #12]
	bl Random16
	ands r0, r6
	lsls r0, r0, #12
	b .L_081892be
.L_081892aa:
	bl Random16
	ands r0, r6
	subs r0, #32
	lsls r0, r0, #14
	str r0, [r5, #12]
	bl Random16
	ands r0, r6
	lsls r0, r0, #13
.L_081892be:
	str r0, [r5, #16]
	bl Random16
	ands r0, r6
	subs r0, #32
	lsls r0, r0, #14
	str r0, [r5, #20]
	movs r3, #1
	add r8, r3
	mov r0, r8
	str r3, [r5, #24]
	adds r5, #28
	cmp r0, #56
	bne .L_0818927c
	ldr r3, .L_08189424
	movs r1, #0
	movs r2, #128
	mov r8, r1
	lsls r2, r2, #2
	subs r1, #1
.L_081892e6:
	movs r4, #1
	add r8, r4
	str r1, [r3]
	adds r3, #28
	cmp r8, r2
	bne .L_081892e6
	movs r5, #0
	mov r8, r5
	ldr r5, .L_08189428
	movs r6, #63
.L_081892fa:
	ldr r3, [r7, #8]
	str r3, [r5]
	movs r3, #0
	str r3, [r5, #4]
	ldr r3, [r7, #16]
	str r3, [r5, #8]
	bl Random16
	ands r0, r6
	subs r0, #32
	lsls r0, r0, #14
	str r0, [r5, #12]
	bl Random16
	ands r0, r6
	lsls r0, r0, #12
	str r0, [r5, #16]
	bl Random16
	ands r0, r6
	subs r0, #32
	lsls r0, r0, #14
	str r0, [r5, #20]
	bl Random16
	movs r3, #15
	ands r3, r0
	movs r0, #1
	add r8, r0
	adds r3, #24
	mov r1, r8
	str r3, [r5, #24]
	adds r5, #28
	cmp r1, #128
	bne .L_081892fa
	ldr r6, .L_0818942c
	movs r2, #0
	mov r8, r2
	mov r5, r11
.L_08189348:
	ldr r4, [sp, #52]
	ldr r3, [r4, #4]
	cmp r3, #1
	bne .L_08189362
	adds r0, r6, #0
	bl Trig_Sin
	lsls r3, r0, #1
	adds r3, r3, r0
	lsls r3, r3, #3
	asrs r3, r3, #16
	adds r3, #88
	b .L_08189374
.L_08189362:
	adds r0, r6, #0
	bl Trig_Sin
	lsls r3, r0, #1
	adds r3, r3, r0
	lsls r3, r3, #3
	negs r3, r3
	asrs r3, r3, #16
	adds r3, #16
.L_08189374:
	str r3, [r5]
	adds r0, r6, #0
	bl Trig_Cos
	lsls r0, r0, #4
	asrs r0, r0, #16
	adds r0, #40
	str r0, [r5, #4]
	mov r7, r8
	movs r1, #1
	lsls r3, r7, #1
	movs r0, #128
	add r8, r1
	negs r3, r3
	lsls r0, r0, #5
	mov r2, r8
	str r3, [r5, #24]
	adds r6, r6, r0
	adds r5, #28
	cmp r2, #8
	bne .L_08189348
	ldr r0, .L_08189430
	bl Resource_GetTableEntry
	movs r3, #0
	str r0, [sp, #36]
	str r3, [sp, #44]
.L_081893aa:
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #48]
	ldr r4, [sp, #44]
	str r3, [sp, #16]
	cmp r4, #83
	bne .L_081893ce
	ldr r5, [sp, #52]
	ldr r3, [r5, #24]
	cmp r3, #0
	bne .L_081893c8
	movs r0, #134
	bl Func_081180e8
	b .L_081893ce
.L_081893c8:
	movs r0, #145
	bl Func_081180e8
.L_081893ce:
	ldr r7, [sp, #44]
	cmp r7, #0
	bne .L_081893da
	movs r0, #136
	bl Audio_PlayCue
.L_081893da:
	ldr r0, [sp, #44]
	cmp r0, #50
	bne .L_081893e6
	movs r0, #136
	bl Audio_PlayCue
.L_081893e6:
	ldr r1, [sp, #52]
	ldr r3, [r1, #4]
	cmp r3, #0
	bne .L_08189438
	ldr r2, [sp, #44]
	cmp r2, #63
	bgt .L_0818944c
	ldr r4, [sp, #16]
	ldr r5, .L_08189434
	ldrh r3, [r4, #54]
	adds r7, r4, #0
	adds r3, r3, r5
	strh r3, [r7, #54]
	b .L_0818944c
	.2byte 0x0000
.L_08189404:
	.4byte 0x00000134
.L_08189408:
	.4byte 0x00000178
.L_0818940c:
	.4byte 0x0000013e
.L_08189410:
	.4byte gMapCellBuffer
.L_08189414:
	.4byte 0x00000148
.L_08189418:
	.4byte IwramCopyWords
.L_0818941c:
	.4byte Func_08143000
.L_08189420:
	.4byte Data_08199d5c
.L_08189424:
	.4byte Data_02014018
.L_08189428:
	.4byte Data_02014000
.L_0818942c:
	.4byte 0xffffc000
.L_08189430:
	.4byte 0x00000196
.L_08189434:
	.4byte 0xffffff00
.L_08189438:
	ldr r0, [sp, #44]
	cmp r0, #63
	bgt .L_0818944c
	ldr r1, [sp, #16]
	movs r2, #128
	ldrh r3, [r1, #54]
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r4, r1, #0
	strh r3, [r4, #54]
.L_0818944c:
	movs r0, #0
	movs r1, #0
	movs r2, #0
	movs r3, #100
	bl BattlePres_SetupTransitionSceneFar
	ldr r5, [sp, #44]
	cmp r5, #17
	bgt .L_081894c2
	adds r0, r5, #0
	movs r1, #3
	bl __divsi3
	ldr r3, .L_081896fc
	adds r5, r0, #0
	ldr r0, .L_08189700
	lsls r7, r5, #1
	ldrh r1, [r0, r7]
	ldrb r0, [r3, r5]
	ldr r4, .L_08189704
	movs r2, #134
	lsls r2, r2, #7
	mov r10, r2
	lsrs r3, r0, #1
	movs r2, #56
	subs r2, r2, r3
	ldr r6, .L_08189708
	ldrb r3, [r4, r5]
	str r0, [sp, #0]
	add r1, r11
	ldrb r0, [r6, r5]
	add r1, r10
	mov r8, r4
	adds r3, #60
	str r0, [sp, #4]
	ldr r4, [sp, #56]
	ldr r0, [sp, #48]
	mov lr, r4
	.2byte 0xf800
	ldr r0, .L_08189700
	ldr r2, .L_081896fc
	ldrh r1, [r0, r7]
	ldrb r0, [r2, r5]
	mov r4, r8
	lsrs r3, r0, #1
	movs r2, #64
	subs r2, r2, r3
	ldrb r3, [r4, r5]
	str r0, [sp, #0]
	add r1, r11
	ldrb r0, [r6, r5]
	ldr r5, [sp, #20]
	str r0, [sp, #4]
	add r1, r10
	adds r3, #60
	ldr r4, [r5, #4]
	ldr r0, [sp, #48]
	mov lr, r4
	.2byte 0xf800
.L_081894c2:
	ldr r7, [sp, #52]
	ldr r3, [r7, #24]
	cmp r3, #1
	bne .L_081895c0
	ldr r0, [sp, #44]
	cmp r0, #0
	bne .L_08189532
	ldr r7, .L_0818970c
	movs r1, #0
	mov r8, r1
.L_081894d6:
	movs r3, #240
	lsls r3, r3, #14
	str r3, [r7]
	movs r3, #216
	lsls r3, r3, #15
	str r3, [r7, #4]
	bl Random16
	movs r6, #254
	lsls r6, r6, #7
	adds r6, #255
	movs r2, #128
	lsls r2, r2, #7
	ands r6, r0
	adds r6, r6, r2
	bl Random16
	movs r5, #255
	ands r5, r0
	adds r0, r6, #0
	bl Trig_Sin
	adds r5, #127
	adds r3, r5, #0
	muls r3, r0
	asrs r3, r3, #7
	str r3, [r7, #12]
	adds r0, r6, #0
	bl Trig_Cos
	adds r3, r5, #0
	muls r3, r0
	asrs r3, r3, #5
	str r3, [r7, #16]
	bl Random16
	movs r3, #15
	ands r3, r0
	adds r3, #24
	str r3, [r7, #24]
	movs r3, #1
	add r8, r3
	mov r4, r8
	adds r7, #28
	cmp r4, #128
	bne .L_081894d6
.L_08189532:
	ldr r6, .L_0818970c
	movs r5, #0
	mov r8, r5
.L_08189538:
	ldr r0, [r6, #24]
	cmp r0, #0
	blt .L_081895b4
	mov r3, r8
	cmp r3, #0
	bge .L_08189546
	adds r3, #7
.L_08189546:
	ldr r7, [sp, #44]
	asrs r3, r3, #3
	cmp r7, r3
	blt .L_081895b4
	asrs r0, r0, #4
	adds r0, #1
	ldr r2, .L_08189710
	lsls r5, r0, #1
	subs r3, r5, #2
	mov r1, r8
	movs r4, #1
	ands r4, r1
	ldrh r1, [r2, r3]
	ldr r2, [sp, #24]
	lsls r4, r4, #2
	adds r1, r2, r1
	movs r3, #2
	ldrsh r2, [r6, r3]
	lsrs r3, r0, #31
	adds r3, r0, r3
	asrs r3, r3, #1
	subs r2, r2, r3
	movs r7, #6
	ldrsh r3, [r6, r7]
	str r0, [sp, #0]
	subs r3, r3, r0
	str r5, [sp, #4]
	ldr r0, [sp, #20]
	ldr r4, [r4, r0]
	ldr r0, [sp, #48]
	mov lr, r4
	.2byte 0xf800
	movs r2, #128
	movs r1, #62
	lsls r2, r2, #7
	adds r0, r6, #0
	bl BattleFxKernels_IntegrateVector2
	movs r3, #3
	mov r1, r8
	ands r3, r1
	ldr r2, [r6, #4]
	adds r3, #108
	lsls r3, r3, #16
	cmp r2, r3
	ble .L_081895ae
	ldr r3, [r6, #16]
	negs r3, r3
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	str r3, [r6, #16]
.L_081895ae:
	ldr r3, [r6, #24]
	subs r3, #1
	str r3, [r6, #24]
.L_081895b4:
	movs r2, #1
	add r8, r2
	mov r3, r8
	adds r6, #28
	cmp r3, #64
	bne .L_08189538
.L_081895c0:
	ldr r4, [sp, #44]
	subs r4, #18
	str r4, [sp, #12]
	cmp r4, #40
	bhi .L_0818960e
	ldr r5, [sp, #44]
	cmp r5, #18
	bne .L_081895f4
	ldr r7, [sp, #36]
	movs r3, #0
	ldrsb r3, [r7, r3]
	ldrb r2, [r7, #1]
	lsls r3, r3, #8
	adds r3, r3, r2
	str r3, [sp, #32]
	adds r0, r7, #0
	movs r3, #2
	ldrsb r3, [r7, r3]
	ldrb r2, [r7, #3]
	lsls r3, r3, #8
	adds r3, r3, r2
	adds r3, #16
	adds r0, #4
	str r3, [sp, #28]
	str r0, [sp, #36]
	b .L_0818960e
.L_081895f4:
	ldr r1, [sp, #36]
	ldr r2, [sp, #32]
	movs r3, #0
	ldrsb r3, [r1, r3]
	ldr r4, [sp, #28]
	adds r2, r2, r3
	str r2, [sp, #32]
	movs r3, #1
	ldrsb r3, [r1, r3]
	adds r1, #2
	adds r4, r4, r3
	str r4, [sp, #28]
	str r1, [sp, #36]
.L_0818960e:
	ldr r5, [sp, #44]
	subs r5, #78
	mov r9, r5
	cmp r5, #40
	bhi .L_08189642
	ldr r1, [sp, #52]
	add r5, sp, #76
	movs r7, #36
	ldrsh r0, [r1, r7]
	adds r1, r5, #0
	bl Func_0815e21c
	ldr r2, [sp, #44]
	cmp r2, #78
	bne .L_0818963c
	ldr r3, [r5]
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	str r3, [sp, #32]
	movs r3, #48
	str r3, [sp, #28]
	b .L_08189642
.L_0818963c:
	ldr r4, [sp, #28]
	subs r4, #16
	str r4, [sp, #28]
.L_08189642:
	movs r7, #24
	movs r0, #39
	mov r10, r7
	movs r5, #19
	movs r7, #156
	mov r8, r0
.L_0818964e:
	ldr r1, [sp, #44]
	adds r3, r5, #0
	adds r3, #18
	cmp r1, r3
	ble .L_081896b6
	adds r3, #65
	cmp r1, r3
	bgt .L_081896b6
	lsls r0, r5, #3
	adds r3, r0, #0
	add r2, sp, #88
	subs r3, #8
	ldr r3, [r2, r3]
	str r3, [r2, r0]
	subs r3, r0, #4
	ldr r6, [r2, r3]
	str r6, [r2, r7]
	cmp r5, #10
	ble .L_0818969a
	movs r3, #234
	lsls r3, r3, #2
	adds r1, r5, #0
	muls r1, r3
	ldr r2, [r2, r0]
	ldr r3, .L_08189714
	mov r4, r10
	mov r0, r8
	add r1, r11
	adds r1, r1, r3
	str r4, [sp, #0]
	str r0, [sp, #4]
	subs r2, #12
	ldr r4, [sp, #56]
	ldr r0, [sp, #48]
	adds r3, r6, #0
	mov lr, r4
	.2byte 0xf800
	b .L_081896b6
.L_0818969a:
	ldr r2, [r2, r0]
	mov r1, r10
	str r1, [sp, #0]
	movs r1, #236
	mov r3, r8
	lsls r1, r1, #5
	str r3, [sp, #4]
	subs r2, #12
	ldr r4, [sp, #56]
	ldr r0, [sp, #48]
	add r1, r11
	adds r3, r6, #0
	mov lr, r4
	.2byte 0xf800
.L_081896b6:
	subs r5, #1
	subs r7, #8
	cmp r5, #0
	bne .L_0818964e
	bl Func_08014de4
	ldr r1, [sp, #16]
	ldr r0, [sp, #16]
	adds r1, #12
	bl Graphics_PrepareTransferInIwramWork
	ldr r4, [sp, #12]
	cmp r4, #65
	bls .L_081896d4
	b .L_0818981e
.L_081896d4:
	mov r5, r9
	cmp r5, #40
	bhi .L_081896e2
	ldr r7, [sp, #32]
	add r5, sp, #64
	str r7, [r5]
	b .L_08189726
.L_081896e2:
	ldr r0, [sp, #52]
	ldr r3, [r0, #4]
	cmp r3, #1
	bne .L_08189718
	ldr r1, [sp, #32]
	movs r3, #64
	lsrs r2, r1, #31
	adds r2, r1, r2
	asrs r2, r2, #1
	add r5, sp, #64
	subs r3, r3, r2
	b .L_08189724
	.2byte 0x0000
.L_081896fc:
	.4byte Data_08197467
.L_08189700:
	.4byte Data_0819747a
.L_08189704:
	.4byte Data_08197473
.L_08189708:
	.4byte Data_0819746d
.L_0818970c:
	.4byte Data_02014e00
.L_08189710:
	.4byte Data_08197410
.L_08189714:
	.4byte 0xfffff8f0
.L_08189718:
	ldr r2, [sp, #32]
	add r5, sp, #64
	lsrs r3, r2, #31
	adds r3, r2, r3
	asrs r3, r3, #1
	adds r3, #64
.L_08189724:
	str r3, [r5]
.L_08189726:
	ldr r4, [sp, #28]
	movs r3, #60
	subs r3, r3, r4
	str r3, [r5, #4]
	add r4, sp, #88
	ldr r2, [r4, #4]
	subs r3, r3, r2
	subs r3, #24
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r0, r3, #1
	cmp r0, #2
	ble .L_08189742
	movs r0, #2
.L_08189742:
	movs r7, #2
	negs r7, r7
	cmp r0, r7
	bge .L_0818974e
	movs r0, #2
	negs r0, r0
.L_0818974e:
	ldr r1, [sp, #40]
	adds r1, r1, r0
	str r1, [sp, #40]
	cmp r1, #8
	ble .L_0818975c
	movs r2, #8
	str r2, [sp, #40]
.L_0818975c:
	ldr r3, [sp, #40]
	movs r7, #8
	negs r7, r7
	cmp r3, r7
	bge .L_0818976c
	movs r0, #8
	negs r0, r0
	str r0, [sp, #40]
.L_0818976c:
	ldr r3, [sp, #40]
	cmp r3, #0
	bge .L_08189774
	adds r3, #3
.L_08189774:
	ldr r2, [r5]
	asrs r3, r3, #2
	str r2, [r4]
	adds r0, r3, #2
	ldr r3, [r5, #4]
	subs r2, #12
	adds r1, r3, #0
	subs r1, #20
	str r1, [r4, #4]
	lsls r1, r0, #3
	adds r1, r1, r0
	lsls r1, r1, #7
	movs r0, #24
	movs r4, #224
	lsls r4, r4, #3
	str r0, [sp, #0]
	add r1, r11
	movs r0, #48
	adds r1, r1, r4
	str r0, [sp, #4]
	subs r3, #22
	ldr r4, [sp, #56]
	ldr r0, [sp, #48]
	mov lr, r4
	.2byte 0xf800
	ldr r0, [sp, #44]
	movs r7, #0
	lsls r0, r0, #2
	mov r8, r7
	mov r10, r0
	adds r7, r5, #0
.L_081897b2:
	mov r3, r10
	add r3, r8
	movs r2, #127
	movs r1, #128
	lsls r1, r1, #1
	ands r3, r2
	adds r3, r3, r1
	lsls r6, r3, #3
	ldr r2, .L_08189ae4
	subs r6, r6, r3
	ldr r3, [r7]
	lsls r6, r6, #2
	adds r6, r6, r2
	lsls r3, r3, #16
	str r3, [r6]
	ldr r3, [r7, #4]
	movs r5, #255
	lsls r3, r3, #16
	str r3, [r6, #4]
	bl Random16
	adds r2, r0, #0
	str r2, [sp, #8]
	bl Random16
	ldr r2, [sp, #8]
	ands r5, r0
	adds r0, r2, #0
	bl Trig_Sin
	adds r5, #127
	adds r3, r5, #0
	muls r3, r0
	ldr r2, [sp, #8]
	asrs r3, r3, #7
	str r3, [r6, #12]
	adds r0, r2, #0
	bl Trig_Cos
	adds r3, r5, #0
	muls r3, r0
	asrs r3, r3, #5
	str r3, [r6, #16]
	bl Random16
	movs r3, #15
	ands r3, r0
	adds r3, #24
	str r3, [r6, #24]
	movs r3, #1
	add r8, r3
	mov r4, r8
	cmp r4, #4
	bne .L_081897b2
.L_0818981e:
	ldr r5, [sp, #44]
	cmp r5, #82
	bgt .L_08189826
	b .L_0818997a
.L_08189826:
	movs r7, #0
	mov r8, r7
	mov r7, r11
	adds r7, #224
.L_0818982e:
	ldr r3, [r7, #4]
	cmp r3, #0
	blt .L_081898fa
	add r6, sp, #64
	adds r0, r7, #0
	adds r1, r6, #0
	bl Func_0815e1ec
	ldr r3, [r6]
	asrs r3, r3, #1
	str r3, [r6]
	ldr r3, [r6, #8]
	cmp r3, #159
	bgt .L_0818984e
	movs r3, #160
	str r3, [r6, #8]
.L_0818984e:
	movs r2, #136
	lsls r2, r2, #2
	adds r2, #255
	cmp r3, r2
	ble .L_0818985c
	str r2, [r6, #8]
	adds r3, r2, #0
.L_0818985c:
	adds r2, r3, #0
	subs r2, #160
	cmp r2, #0
	bge .L_08189866
	adds r2, #63
.L_08189866:
	ldr r0, [sp, #52]
	movs r3, #9
	asrs r2, r2, #6
	subs r5, r3, r2
	ldr r4, [r0, #24]
	ldr r3, .L_08189ae8
	ldrb r3, [r3, r4]
	cmp r8, r3
	blt .L_081898ca
	cmp r4, #0
	bne .L_08189886
	ldr r3, [r7, #24]
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r1, r3, #1
	b .L_08189890
.L_08189886:
	ldr r1, [r7, #24]
	cmp r1, #0
	bge .L_0818988e
	adds r1, #3
.L_0818988e:
	asrs r1, r1, #2
.L_08189890:
	cmp r1, #5
	bgt .L_081898fa
	ldr r2, .L_08189aec
	movs r0, #32
	lsls r1, r1, #11
	ldr r3, [r6, #4]
	adds r1, r1, r2
	ldr r2, [r6]
	str r0, [sp, #0]
	movs r0, #64
	str r0, [sp, #4]
	ldr r5, [sp, #20]
	lsls r4, r4, #2
	subs r2, #16
	subs r3, #40
	ldr r4, [r4, r5]
	ldr r0, [sp, #48]
	mov lr, r4
	.2byte 0xf800
	ldr r3, [r7, #24]
	movs r2, #128
	adds r3, #1
	str r3, [r7, #24]
	adds r0, r7, #0
	movs r1, #62
	lsls r2, r2, #6
	bl BattleFxKernels_IntegrateVector3
	b .L_081898fa
.L_081898ca:
	ldr r2, .L_08189af0
	lsls r0, r5, #1
	subs r3, r0, #2
	ldrh r1, [r2, r3]
	ldr r2, [sp, #24]
	lsrs r3, r5, #31
	adds r1, r2, r1
	ldr r2, [r6]
	adds r3, r5, r3
	asrs r3, r3, #1
	subs r2, r2, r3
	ldr r3, [r6, #4]
	ldr r4, [sp, #56]
	str r0, [sp, #4]
	subs r3, r3, r5
	str r5, [sp, #0]
	ldr r0, [sp, #48]
	mov lr, r4
	.2byte 0xf800
	adds r0, r7, #0
	movs r1, #64
	ldr r2, .L_08189af4
	bl BattleFxKernels_IntegrateVector3
.L_081898fa:
	movs r3, #1
	add r8, r3
	mov r4, r8
	adds r7, #28
	cmp r4, #56
	bne .L_0818982e
	ldr r5, [sp, #52]
	ldr r3, [r5, #24]
	cmp r3, #1
	bne .L_081899e6
	ldr r6, .L_08189ae4
	movs r7, #0
	mov r8, r7
	add r7, sp, #64
.L_08189916:
	ldr r5, [r6, #24]
	cmp r5, #0
	blt .L_0818996e
	adds r1, r7, #0
	adds r0, r6, #0
	bl Func_0815e1ec
	ldr r2, [r7]
	mov r1, r8
	asrs r5, r5, #3
	movs r0, #1
	adds r5, #1
	ands r0, r1
	ldr r1, .L_08189af0
	lsls r4, r5, #1
	asrs r2, r2, #1
	str r2, [r7]
	subs r3, r4, #2
	ldrh r1, [r1, r3]
	ldr r3, [sp, #24]
	lsls r0, r0, #2
	adds r1, r3, r1
	lsrs r3, r5, #31
	adds r3, r5, r3
	asrs r3, r3, #1
	subs r2, r2, r3
	ldr r3, [r7, #4]
	str r5, [sp, #0]
	subs r3, r3, r5
	str r4, [sp, #4]
	ldr r5, [sp, #20]
	ldr r4, [r0, r5]
	ldr r0, [sp, #48]
	mov lr, r4
	.2byte 0xf800
	movs r2, #128
	adds r0, r6, #0
	movs r1, #62
	lsls r2, r2, #6
	bl BattleFxKernels_IntegrateVector3
	ldr r3, [r6, #24]
	subs r3, #1
	str r3, [r6, #24]
.L_0818996e:
	movs r0, #1
	add r8, r0
	mov r1, r8
	adds r6, #28
	cmp r1, #128
	bne .L_08189916
.L_0818997a:
	ldr r2, [sp, #52]
	ldr r3, [r2, #24]
	cmp r3, #1
	bne .L_081899e6
	ldr r4, .L_08189af0
	ldr r6, .L_08189af8
	movs r3, #0
	mov r8, r3
	mov r10, r4
.L_0818998c:
	ldr r0, [r6, #24]
	cmp r0, #0
	blt .L_081899da
	asrs r0, r0, #4
	adds r0, #1
	mov r5, r8
	movs r4, #1
	ands r4, r5
	lsls r5, r0, #1
	subs r3, r5, #2
	mov r7, r10
	ldrh r1, [r7, r3]
	ldr r2, [sp, #24]
	lsls r4, r4, #2
	adds r1, r2, r1
	movs r3, #2
	ldrsh r2, [r6, r3]
	lsrs r3, r0, #31
	adds r3, r0, r3
	asrs r3, r3, #1
	subs r2, r2, r3
	movs r7, #6
	ldrsh r3, [r6, r7]
	str r0, [sp, #0]
	subs r3, r3, r0
	str r5, [sp, #4]
	ldr r0, [sp, #20]
	ldr r4, [r4, r0]
	ldr r0, [sp, #48]
	mov lr, r4
	.2byte 0xf800
	adds r0, r6, #0
	movs r1, #60
	ldr r2, .L_08189afc
	bl BattleFxKernels_IntegrateVector2
	ldr r3, [r6, #24]
	subs r3, #1
	str r3, [r6, #24]
.L_081899da:
	movs r1, #1
	add r8, r1
	mov r2, r8
	adds r6, #28
	cmp r2, #128
	bne .L_0818998c
.L_081899e6:
	ldr r3, [sp, #44]
	cmp r3, #83
	bne .L_08189a14
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #168
	movs r2, #8
	add r3, r11
	str r2, [r3]
	ldr r5, [sp, #52]
	movs r1, #7
	movs r4, #36
	ldrsh r0, [r5, r4]
	movs r3, #0
	str r2, [sp, #0]
	movs r2, #5
	bl Func_0814cd48
	movs r7, #36
	ldrsh r0, [r5, r7]
	movs r1, #1
	bl Func_08118088
.L_08189a14:
	ldr r0, [sp, #44]
	cmp r0, #50
	bne .L_08189a3a
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #168
	add r2, r11
	movs r3, #12
	str r3, [r2]
	ldr r2, [sp, #52]
	movs r3, #8
	movs r1, #36
	ldrsh r0, [r2, r1]
	str r3, [sp, #0]
	movs r1, #7
	movs r2, #5
	movs r3, #0
	bl Func_0814cd48
.L_08189a3a:
	ldr r3, [sp, #44]
	cmp r3, #49
	ble .L_08189a96
	movs r4, #0
	mov r8, r4
	mov r6, r11
.L_08189a46:
	ldr r3, [r6, #24]
	cmp r3, #11
	bhi .L_08189a86
	lsrs r4, r3, #31
	ldr r5, .L_08189b00
	ldr r0, .L_08189b04
	adds r4, r3, r4
	asrs r4, r4, #1
	lsls r3, r4, #1
	ldrh r1, [r5, r3]
	ldrb r5, [r0, r4]
	ldr r2, [r6]
	lsrs r3, r5, #1
	subs r2, r2, r3
	ldr r3, .L_08189b08
	movs r7, #134
	ldrb r0, [r3, r4]
	ldr r3, [r6, #4]
	str r5, [sp, #0]
	adds r3, r3, r0
	ldr r0, .L_08189b0c
	ldr r5, [sp, #20]
	ldrb r0, [r0, r4]
	add r1, r11
	str r0, [sp, #4]
	lsls r7, r7, #7
	adds r1, r1, r7
	ldr r4, [r5, #4]
	ldr r0, [sp, #48]
	mov lr, r4
	.2byte 0xf800
	ldr r3, [r6, #24]
.L_08189a86:
	movs r7, #1
	add r8, r7
	adds r3, #1
	mov r0, r8
	str r3, [r6, #24]
	adds r6, #28
	cmp r0, #8
	bne .L_08189a46
.L_08189a96:
	movs r1, #8
	movs r0, #8
	bl Func_08158ce0
	bl Func_081434f8
	movs r2, #240
	lsls r2, r2, #7
	adds r2, #232
	add r2, r11
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r1, [sp, #44]
	adds r1, #1
	str r1, [sp, #44]
	cmp r1, #150
	beq .L_08189ac0
	b .L_081893aa
.L_08189ac0:
	ldr r0, .L_08189b10
	bl Scheduler_RemoveCallback
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	bl Func_08143bb8
	add sp, #248
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_08189ae4:
	.4byte Data_02014000
.L_08189ae8:
	.4byte Data_08199d5c
.L_08189aec:
	.4byte gMapCellBuffer
.L_08189af0:
	.4byte Data_08197410
.L_08189af4:
	.4byte 0xffffe000
.L_08189af8:
	.4byte Data_02015c00
.L_08189afc:
	.4byte 0xffffc000
.L_08189b00:
	.4byte Data_0819747a
.L_08189b04:
	.4byte Data_08197467
.L_08189b08:
	.4byte Data_08197473
.L_08189b0c:
	.4byte Data_0819746d
.L_08189b10:
	.4byte Func_08143000
