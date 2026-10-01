.syntax unified
	.thumb
	.global Func_0817c27c
	.thumb_func
Func_0817c27c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #40
	str r0, [sp, #24]
	movs r5, #192
	lsls r5, r5, #18
	ldr r0, [r5, #92]
	ldr r1, [r5, #96]
	ldr r2, [r5, #100]
	mov r9, r0
	movs r0, #1
	mov r11, r1
	str r2, [sp, #8]
	bl BattleFx_BeginCanvasLayer
	ldr r4, [sp, #24]
	ldr r3, [r4, #4]
	cmp r3, #0
	bne .L_0817c2b2
	movs r0, #104
	movs r1, #19
	b .L_0817c2b6
.L_0817c2b2:
	movs r0, #104
	movs r1, #23
.L_0817c2b6:
	bl Func_081963ec
	ldr r5, [r5, #104]
	str r5, [sp, #12]
	ldr r1, [sp, #8]
	ldr r0, .L_0817c614
	movs r2, #0
	movs r3, #0
	bl Resource_LoadAndDecompress
	movs r1, #224
	lsls r1, r1, #3
	add r1, r9
	movs r2, #0
	movs r3, #0
	ldr r0, .L_0817c618
	bl Resource_LoadAndDecompress
	ldr r0, .L_0817c61c
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_0817c620
	movs r2, #128
	lsls r0, r0, #19
	mov lr, r3
	.2byte 0xf800
	movs r2, #239
	lsls r2, r2, #7
	add r2, r9
	movs r3, #2
	str r3, [r2]
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #132
	add r2, r9
	movs r3, #50
	movs r1, #200
	str r3, [r2]
	ldr r0, .L_0817c624
	lsls r1, r1, #4
	bl Scheduler_AddOrUpdateCallback
	movs r6, #0
	movs r0, #28
	str r6, [sp, #20]
	add r0, sp
	mov r8, r0
.L_0817c318:
	ldr r1, [sp, #24]
	ldr r0, [r1, #8]
	mov r1, r8
	bl Func_0815e21c
	ldr r2, [sp, #24]
	ldr r3, [r2, #4]
	cmp r3, #0
	bne .L_0817c400
	ldr r0, [sp, #20]
	movs r1, #6
	bl Math_Div
	cmp r0, #4
	bls .L_0817c338
	b .L_0817c4d2
.L_0817c338:
	ldr r2, .L_0817c628
	lsls r3, r0, #2
	ldr r3, [r3, r2]
	mov pc, r3
.L_0817c340:
	.4byte .L_0817c354
	.4byte .L_0817c37e
	.4byte .L_0817c39e
	.4byte .L_0817c3c0
	.4byte .L_0817c3e0
.L_0817c354:
	mov r3, r8
	ldr r2, [r3]
	mov r4, r8
	lsrs r3, r2, #31
	adds r2, r2, r3
	ldr r3, [r4, #4]
	movs r0, #48
	movs r1, #224
	lsls r1, r1, #3
	asrs r2, r2, #1
	str r0, [sp, #0]
	movs r0, #96
	str r0, [sp, #4]
	add r1, r9
	subs r2, #48
	subs r3, #120
	mov r0, r11
	ldr r6, [sp, #12]
	mov lr, r6
	.2byte 0xf800
	b .L_0817c4d2
.L_0817c37e:
	mov r0, r8
	ldr r2, [r0]
	movs r1, #200
	lsrs r3, r2, #31
	adds r2, r2, r3
	ldr r3, [r0, #4]
	movs r0, #38
	lsls r1, r1, #5
	asrs r2, r2, #1
	str r0, [sp, #0]
	movs r0, #92
	str r0, [sp, #4]
	add r1, r9
	subs r2, #38
	subs r3, #120
	b .L_0817c4a2
.L_0817c39e:
	mov r6, r8
	ldr r2, [r6]
	movs r1, #152
	lsrs r3, r2, #31
	adds r2, r2, r3
	ldr r3, [r6, #4]
	movs r0, #30
	lsls r1, r1, #6
	adds r1, #168
	asrs r2, r2, #1
	str r0, [sp, #0]
	movs r0, #76
	str r0, [sp, #4]
	add r1, r9
	subs r2, #38
	subs r3, #100
	b .L_0817c4a2
.L_0817c3c0:
	mov r6, r8
	ldr r2, [r6]
	movs r1, #188
	lsrs r3, r2, #31
	adds r2, r2, r3
	ldr r3, [r6, #4]
	movs r0, #56
	lsls r1, r1, #6
	adds r1, #144
	asrs r2, r2, #1
	str r0, [sp, #0]
	movs r0, #96
	str r0, [sp, #4]
	add r1, r9
	subs r2, #64
	b .L_0817c4a0
.L_0817c3e0:
	mov r6, r8
	ldr r2, [r6]
	movs r1, #136
	lsrs r3, r2, #31
	adds r2, r2, r3
	ldr r3, [r6, #4]
	movs r0, #56
	lsls r1, r1, #7
	adds r1, #144
	asrs r2, r2, #1
	str r0, [sp, #0]
	movs r0, #88
	str r0, [sp, #4]
	add r1, r9
	subs r2, #64
	b .L_0817c4a0
.L_0817c400:
	ldr r0, [sp, #20]
	movs r1, #6
	bl Math_Div
	cmp r0, #4
	bhi .L_0817c4d2
	ldr r2, .L_0817c62c
	lsls r3, r0, #2
	ldr r3, [r3, r2]
	mov pc, r3
.L_0817c414:
	.4byte .L_0817c428
	.4byte .L_0817c446
	.4byte .L_0817c464
	.4byte .L_0817c484
	.4byte .L_0817c4ac
.L_0817c428:
	mov r6, r8
	ldr r2, [r6]
	movs r0, #48
	lsrs r3, r2, #31
	adds r2, r2, r3
	ldr r3, [r6, #4]
	movs r1, #224
	lsls r1, r1, #3
	str r0, [sp, #0]
	movs r0, #96
	str r0, [sp, #4]
	add r1, r9
	asrs r2, r2, #1
	subs r3, #120
	b .L_0817c4a2
.L_0817c446:
	mov r6, r8
	ldr r2, [r6]
	movs r0, #38
	lsrs r3, r2, #31
	adds r2, r2, r3
	ldr r3, [r6, #4]
	movs r1, #200
	lsls r1, r1, #5
	str r0, [sp, #0]
	movs r0, #92
	str r0, [sp, #4]
	add r1, r9
	asrs r2, r2, #1
	subs r3, #120
	b .L_0817c4a2
.L_0817c464:
	mov r6, r8
	ldr r2, [r6]
	movs r1, #152
	lsrs r3, r2, #31
	adds r2, r2, r3
	ldr r3, [r6, #4]
	movs r0, #30
	lsls r1, r1, #6
	adds r1, #168
	str r0, [sp, #0]
	movs r0, #76
	str r0, [sp, #4]
	add r1, r9
	asrs r2, r2, #1
	subs r3, #100
	b .L_0817c4a2
.L_0817c484:
	mov r6, r8
	ldr r2, [r6]
	movs r1, #188
	lsrs r3, r2, #31
	adds r2, r2, r3
	ldr r3, [r6, #4]
	movs r0, #56
	lsls r1, r1, #6
	adds r1, #144
	str r0, [sp, #0]
	movs r0, #96
	str r0, [sp, #4]
	add r1, r9
	asrs r2, r2, #1
.L_0817c4a0:
	subs r3, #88
.L_0817c4a2:
	mov r0, r11
	ldr r4, [sp, #12]
	mov lr, r4
	.2byte 0xf800
	b .L_0817c4d2
.L_0817c4ac:
	mov r6, r8
	ldr r2, [r6]
	movs r1, #136
	lsrs r3, r2, #31
	adds r2, r2, r3
	ldr r3, [r6, #4]
	movs r0, #56
	lsls r1, r1, #7
	adds r1, #144
	str r0, [sp, #0]
	movs r0, #88
	str r0, [sp, #4]
	add r1, r9
	asrs r2, r2, #1
	subs r3, #88
	mov r0, r11
	ldr r4, [sp, #12]
	mov lr, r4
	.2byte 0xf800
.L_0817c4d2:
	ldr r6, [sp, #20]
	cmp r6, #18
	bne .L_0817c5bc
	ldr r2, [sp, #24]
	ldr r7, .L_0817c630
	movs r1, #36
	ldrsh r0, [r2, r1]
	movs r1, #1
	bl Func_08118088
	ldr r4, [sp, #24]
	movs r2, #5
	movs r3, #36
	ldrsh r0, [r4, r3]
	movs r3, #16
	str r3, [sp, #0]
	movs r1, #7
	movs r3, #0
	bl Func_0814cd48
	movs r0, #134
	bl Func_081180e8
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #168
	add r2, r9
	movs r3, #8
	str r3, [r2]
	movs r6, #0
	mov r10, r6
.L_0817c510:
	ldr r0, [sp, #24]
	ldr r3, [r0, #4]
	cmp r3, #0
	bne .L_0817c526
	mov r1, r8
	ldr r3, [r1]
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	subs r3, #48
	b .L_0817c532
.L_0817c526:
	mov r2, r8
	ldr r3, [r2]
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	adds r3, #48
.L_0817c532:
	lsls r3, r3, #16
	str r3, [r7]
	bl Random16
	mov r4, r8
	ldr r3, [r4, #4]
	movs r2, #7
	ands r2, r0
	subs r3, r3, r2
	lsls r3, r3, #16
	str r3, [r7, #4]
	bl Random16
	movs r3, #254
	lsls r3, r3, #7
	adds r3, #255
	ands r3, r0
	movs r0, #128
	lsls r0, r0, #7
	adds r6, r3, r0
	bl Random16
	ldr r1, [sp, #24]
	movs r3, #255
	adds r5, r0, #0
	ands r5, r3
	ldr r3, [r1, #4]
	cmp r3, #0
	bne .L_0817c57e
	adds r0, r6, #0
	bl Trig_Sin
	adds r3, r5, #0
	muls r3, r0
	ldr r2, .L_0817c634
	asrs r3, r3, #4
	adds r3, r3, r2
	b .L_0817c590
.L_0817c57e:
	adds r0, r6, #0
	bl Trig_Sin
	adds r3, r5, #0
	muls r3, r0
	movs r4, #128
	asrs r3, r3, #4
	lsls r4, r4, #12
	adds r3, r3, r4
.L_0817c590:
	str r3, [r7, #12]
	adds r0, r6, #0
	bl Trig_Cos
	adds r3, r5, #0
	adds r3, #64
	muls r3, r0
	asrs r3, r3, #5
	str r3, [r7, #16]
	bl Random16
	movs r3, #15
	ands r3, r0
	movs r6, #1
	movs r0, #128
	adds r3, #16
	add r10, r6
	lsls r0, r0, #1
	str r3, [r7, #24]
	adds r7, #28
	cmp r10, r0
	bne .L_0817c510
.L_0817c5bc:
	ldr r1, [sp, #20]
	cmp r1, #17
	ble .L_0817c660
	ldr r5, .L_0817c630
	movs r2, #0
	mov r10, r2
.L_0817c5c8:
	ldr r0, [r5, #24]
	cmp r0, #0
	blt .L_0817c652
	asrs r0, r0, #3
	adds r0, #1
	ldr r2, .L_0817c638
	lsls r4, r0, #1
	subs r3, r4, #2
	ldrh r1, [r2, r3]
	ldr r3, [sp, #8]
	movs r6, #2
	ldrsh r2, [r5, r6]
	adds r1, r3, r1
	lsrs r3, r0, #31
	adds r3, r0, r3
	asrs r3, r3, #1
	subs r2, r2, r3
	movs r6, #6
	ldrsh r3, [r5, r6]
	str r0, [sp, #0]
	subs r3, r3, r0
	str r4, [sp, #4]
	mov r0, r11
	ldr r4, [sp, #12]
	mov lr, r4
	.2byte 0xf800
	mov r6, r10
	movs r3, #1
	ands r3, r6
	cmp r3, #0
	beq .L_0817c640
	adds r0, r5, #0
	movs r1, #60
	ldr r2, .L_0817c63c
	bl BattleFxKernels_IntegrateVector2
	b .L_0817c64c
	.2byte 0x0000
.L_0817c614:
	.4byte 0x00000134
.L_0817c618:
	.4byte 0x000000f3
.L_0817c61c:
	.4byte 0x00000149
.L_0817c620:
	.4byte IwramCopyWords
.L_0817c624:
	.4byte Func_08143000
.L_0817c628:
	.4byte .L_0817c340
.L_0817c62c:
	.4byte .L_0817c414
.L_0817c630:
	.4byte gMapCellBuffer
.L_0817c634:
	.4byte 0xfff80000
.L_0817c638:
	.4byte Data_08197410
.L_0817c63c:
	.4byte 0xffffe000
.L_0817c640:
	movs r2, #128
	adds r0, r5, #0
	movs r1, #60
	lsls r2, r2, #5
	bl BattleFxKernels_IntegrateVector2
.L_0817c64c:
	ldr r3, [r5, #24]
	subs r3, #1
	str r3, [r5, #24]
.L_0817c652:
	movs r0, #1
	movs r1, #128
	add r10, r0
	lsls r1, r1, #1
	adds r5, #28
	cmp r10, r1
	bne .L_0817c5c8
.L_0817c660:
	movs r0, #8
	movs r1, #8
	bl Func_08158ce0
	bl Func_081434f8
	movs r2, #240
	lsls r2, r2, #7
	adds r2, #232
	add r2, r9
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r2, [sp, #20]
	adds r2, #1
	str r2, [sp, #20]
	cmp r2, #52
	beq .L_0817c68a
	b .L_0817c318
.L_0817c68a:
	ldr r0, .L_0817c6a8
	bl Scheduler_RemoveCallback
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	bl Func_08143bb8
	add sp, #40
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0817c6a8:
	.4byte Func_08143000
