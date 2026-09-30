.syntax unified
	.thumb
	.global Func_0816c274
	.thumb_func
Func_0816c274:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #104
	str r0, [sp, #56]
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #92]
	movs r2, #184
	str r1, [sp, #52]
	lsls r2, r2, #6
	ldr r3, [r3, #96]
	adds r2, #16
	adds r2, r1, r2
	movs r1, #128
	str r3, [sp, #48]
	str r2, [sp, #36]
	ldr r3, .L_0816c36c
	ldr r0, .L_0816c370
	lsls r1, r1, #8
	mov lr, r3
	.2byte 0xf800
	movs r3, #0
	movs r7, #0
	mov r8, r3
	movs r6, #2
	movs r5, #0
.L_0816c2b2:
	ldr r4, .L_0816c370
	movs r2, #128
	adds r1, r6, #0
	adds r0, r7, r4
	lsls r2, r2, #9
	bl Func_0815b434
	adds r3, r5, #3
	muls r3, r6
	ldr r1, [sp, #36]
	lsrs r2, r3, #31
	adds r3, r3, r2
	movs r2, #1
	asrs r3, r3, #1
	add r8, r2
	strh r7, [r5, r1]
	adds r7, r7, r3
	mov r3, r8
	adds r6, #2
	adds r5, #2
	cmp r3, #16
	bne .L_0816c2b2
	ldr r4, [sp, #52]
	movs r1, #224
	lsls r1, r1, #3
	adds r5, r4, r1
	ldr r0, .L_0816c374
	adds r1, r5, #0
	movs r2, #0
	movs r3, #0
	bl Func_08157cf4
	ldr r1, .L_0816c378
	movs r2, #64
	movs r3, #64
	adds r0, r5, #0
	bl Func_0816ae40
	movs r0, #0
	bl Func_081435e0
	ldr r2, [sp, #56]
	movs r3, #1
	ldr r1, [r2, #4]
	adds r0, r2, #0
	eors r1, r3
	lsls r1, r1, #4
	movs r3, #32
	orrs r1, r3
	mov r3, sp
	adds r3, #92
	str r3, [sp, #32]
	ldr r2, [sp, #32]
	add r3, sp, #80
	bl Func_0815585c
	movs r2, #216
	ldr r4, [sp, #52]
	lsls r2, r2, #7
	adds r2, #192
	adds r1, r4, r2
	movs r3, #0
	movs r2, #1
	ldr r0, .L_0816c37c
	bl Func_08157cf4
	ldr r0, .L_0816c380
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_0816c384
	movs r2, #128
	lsls r0, r0, #19
	mov lr, r3
	.2byte 0xf800
	ldr r3, .L_0816c368
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	ldr r4, [sp, #56]
	ldr r3, [r4, #4]
	cmp r3, #1
	bne .L_0816c388
	movs r0, #104
	movs r1, #3
	bl Func_081963ec
	b .L_0816c390
	.2byte 0x0000
.L_0816c368:
	.4byte 0x0000100e
.L_0816c36c:
	.4byte IwramClearWords
.L_0816c370:
	.4byte gMapCellBuffer
.L_0816c374:
	.4byte 0x000000fb
.L_0816c378:
	.4byte Data_02014000
.L_0816c37c:
	.4byte 0x00000157
.L_0816c380:
	.4byte 0x00000184
.L_0816c384:
	.4byte IwramCopyWords
.L_0816c388:
	movs r0, #104
	movs r1, #7
	bl Func_081963ec
.L_0816c390:
	ldr r3, [sp, #52]
	movs r5, #192
	lsls r5, r5, #18
	movs r4, #239
	lsls r4, r4, #7
	ldr r1, [r5, #104]
	adds r2, r3, r4
	movs r3, #2
	str r3, [r2]
	str r1, [sp, #40]
	movs r3, #238
	ldr r1, [sp, #52]
	lsls r3, r3, #7
	adds r3, #132
	adds r2, r1, r3
	movs r3, #75
	movs r1, #200
	str r3, [r2]
	lsls r1, r1, #4
	ldr r0, .L_0816c47c
	bl Func_080145a8
	ldr r5, [r5, #48]
	ldr r1, [sp, #56]
	str r5, [sp, #28]
	movs r4, #36
	ldrsh r0, [r1, r4]
	bl GetBattleObjectSlotFar
	ldr r3, [sp, #28]
	ldr r0, [r0]
	mov r4, sp
	adds r3, #12
	adds r4, #60
	str r0, [sp, #24]
	str r3, [sp, #16]
	str r4, [sp, #20]
	movs r2, #0
	mov r9, r2
.L_0816c3de:
	mov r1, r9
	cmp r1, #0
	bne .L_0816c45a
	ldr r7, [sp, #52]
	movs r2, #0
	mov r8, r2
.L_0816c3ea:
	bl Random16
	adds r6, r0, #0
	bl Random16
	movs r5, #63
	ands r5, r0
	adds r0, r6, #0
	bl Trig_Sin
	ldr r4, [sp, #24]
	adds r5, #80
	adds r2, r5, #0
	muls r2, r0
	ldr r3, [r4, #8]
	asrs r2, r2, #1
	adds r3, r3, r2
	str r3, [r7]
	movs r3, #128
	lsls r3, r3, #12
	str r3, [r7, #4]
	adds r0, r6, #0
	bl Trig_Cos
	adds r3, r5, #0
	muls r3, r0
	asrs r3, r3, #1
	str r3, [r7, #8]
	bl Random16
	str r0, [r7, #12]
	bl Random16
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	movs r1, #128
	ands r3, r0
	lsls r1, r1, #11
	adds r3, r3, r1
	str r3, [r7, #16]
	bl Random16
	movs r3, #7
	ands r3, r0
	adds r3, #4
	str r3, [r7, #20]
	movs r2, #1
	movs r3, #1
	negs r3, r3
	add r8, r2
	str r3, [r7, #24]
	mov r3, r8
	adds r7, #28
	cmp r3, #32
	bne .L_0816c3ea
.L_0816c45a:
	mov r4, r9
	cmp r4, #95
	ble .L_0816c480
	ldr r3, .L_0816c474
	lsls r2, r4, #1
	subs r3, r3, r2
	ldr r2, .L_0816c478
	movs r1, #128
	lsls r1, r1, #19
	adds r1, #82
	orrs r3, r2
	strh r3, [r1]
	b .L_0816c480
.L_0816c474:
	.4byte 0x000000ce
.L_0816c478:
	.4byte 0x00001000
.L_0816c47c:
	.4byte Func_08143000
.L_0816c480:
	bl Func_08014de4
	ldr r1, [sp, #16]
	ldr r0, [sp, #28]
	bl Func_080156e8
	mov r1, r9
	cmp r1, #19
	bgt .L_0816c508
	lsls r5, r1, #11
	adds r0, r5, #0
	bl Trig_Sin
	ldr r2, [sp, #32]
	lsls r0, r0, #3
	ldr r3, [r2]
	asrs r0, r0, #16
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	adds r0, r0, r3
	subs r0, #10
	mov r10, r0
	adds r0, r5, #0
	bl Trig_Cos
	ldr r4, [sp, #32]
	lsls r0, r0, #2
	ldr r3, [r4, #4]
	asrs r0, r0, #16
	adds r0, r0, r3
	mov r1, r9
	adds r5, r0, #0
	lsls r3, r1, #1
	add r3, r9
	subs r5, #24
	ldr r2, [sp, #52]
	adds r5, r5, r3
	movs r3, #216
	lsls r3, r3, #7
	movs r4, #20
	adds r3, #192
	adds r7, r2, r3
	str r4, [sp, #0]
	mov r8, r4
	movs r4, #40
	str r4, [sp, #4]
	adds r1, r7, #0
	str r4, [sp, #8]
	ldr r6, [sp, #40]
	ldr r0, [sp, #48]
	mov r2, r10
	adds r3, r5, #0
	mov lr, r6
	.2byte 0xf800
	mov r1, r9
	ldr r4, [sp, #8]
	cmp r1, #3
	bgt .L_0816c508
	mov r2, r8
	str r2, [sp, #0]
	str r4, [sp, #4]
	ldr r0, [sp, #48]
	adds r1, r7, #0
	mov r2, r10
	adds r3, r5, #0
	mov lr, r6
	.2byte 0xf800
.L_0816c508:
	mov r3, r9
	cmp r3, #47
	ble .L_0816c5b4
	ldr r5, [sp, #52]
	movs r4, #0
	movs r1, #7
	mov r8, r4
	mov r11, r1
.L_0816c518:
	mov r2, r8
	lsls r3, r2, #1
	adds r3, #48
	cmp r9, r3
	blt .L_0816c5a8
	add r6, sp, #68
	adds r1, r6, #0
	adds r0, r5, #0
	bl Func_0815e1ec
	ldr r3, [r6]
	asrs r3, r3, #1
	str r3, [r6]
	ldr r0, [r5, #12]
	ldr r7, [r5, #20]
	bl Trig_Sin
	adds r3, r7, #0
	muls r3, r0
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #17
	adds r7, r7, r3
	ldr r3, [r5, #12]
	movs r4, #128
	lsls r4, r4, #3
	lsls r7, r7, #1
	adds r3, r3, r4
	subs r2, r7, #2
	str r3, [r5, #12]
	cmp r2, #61
	bhi .L_0816c5a0
	lsrs r3, r2, #31
	adds r3, r2, r3
	asrs r3, r3, #1
	mov r10, r3
	ldr r3, [r5, #24]
	ldr r2, [r6]
	ldr r6, [r6, #4]
	cmp r3, #0
	bne .L_0816c58a
	str r2, [sp, #12]
	bl Random16
	ldr r2, [sp, #12]
	mov r1, r11
	ands r0, r1
	adds r0, r2, r0
	subs r2, r0, #4
	str r2, [sp, #12]
	bl Random16
	mov r3, r11
	ands r0, r3
	ldr r2, [sp, #12]
	adds r0, r6, r0
	subs r6, r0, #4
.L_0816c58a:
	ldr r1, [sp, #36]
	mov r4, r10
	lsls r3, r4, #1
	ldrsh r0, [r3, r1]
	ldr r1, .L_0816c6d4
	adds r3, r7, #0
	adds r0, r0, r1
	adds r1, r2, #0
	adds r2, r6, #0
	bl Func_0818caa8
.L_0816c5a0:
	ldr r3, [r5, #4]
	ldr r2, [r5, #16]
	adds r3, r3, r2
	str r3, [r5, #4]
.L_0816c5a8:
	movs r2, #1
	add r8, r2
	mov r3, r8
	adds r5, #28
	cmp r3, #32
	bne .L_0816c518
.L_0816c5b4:
	ldr r1, [sp, #56]
	movs r4, #0
	ldr r3, [r1, #20]
	mov r8, r4
	cmp r3, #0
	beq .L_0816c5f2
	movs r6, #36
	movs r5, #64
.L_0816c5c4:
	cmp r9, r5
	bne .L_0816c5e6
	movs r0, #126
	bl Audio_PlayCue
	ldr r2, [sp, #56]
	movs r1, #7
	ldrsh r0, [r6, r2]
	movs r3, #24
	movs r2, #1
	str r3, [sp, #0]
	negs r2, r2
	mov r3, r8
	bl Func_0814cd48
	ldr r4, [sp, #56]
	ldr r3, [r4, #20]
.L_0816c5e6:
	movs r1, #1
	add r8, r1
	adds r6, #2
	adds r5, #4
	cmp r8, r3
	bne .L_0816c5c4
.L_0816c5f2:
	bl Func_08014de4
	ldr r2, .L_0816c6d8
	movs r3, #104
	str r3, [r2, #16]
	movs r0, #32
	bl Runtime_BumpAllocateAlternatePool
	adds r7, r0, #0
	movs r0, #1
	bl Func_081969f8
	movs r3, #6
	adds r6, r0, #0
	str r3, [r6]
	ldr r3, .L_0816c6dc
	ldr r1, [sp, #20]
	movs r2, #7
	str r3, [r6, #8]
	add r3, sp, #60
	str r1, [r6, #16]
	str r7, [r6, #12]
	str r3, [sp, #20]
	strb r2, [r3]
	ldr r3, .L_0816c6e0
	strb r2, [r1, #1]
	mov r2, r9
	movs r0, #0
	str r3, [r1, #4]
	cmp r2, #15
	ble .L_0816c67c
	ldr r4, .L_0816c6e4
	ldr r1, .L_0816c6e8
	lsls r3, r2, #12
	adds r5, r3, r4
	cmp r5, r1
	ble .L_0816c63e
	ldr r5, .L_0816c6e8
.L_0816c63e:
	str r0, [r6, #20]
	ldr r0, .L_0816c6ec
	bl Func_08015024
	ldr r2, [sp, #32]
	movs r1, #160
	ldr r0, [r2]
	lsls r1, r1, #14
	lsrs r3, r0, #31
	adds r0, r0, r3
	asrs r0, r0, #1
	subs r0, #64
	movs r2, #0
	lsls r0, r0, #16
	bl Func_08015160
	asrs r0, r5, #1
	bl Func_0801521c
	mov r3, r9
	lsls r0, r3, #9
	bl Func_08015068
	ldr r0, .L_0816c6f0
	adds r1, r7, #0
	movs r2, #4
	bl Func_08196958
	adds r0, r6, #0
	bl Func_08196a7c
.L_0816c67c:
	adds r0, r6, #0
	bl Sys_Free
	adds r0, r7, #0
	bl Sys_Free
	ldr r2, .L_0816c6d8
	movs r3, #120
	str r3, [r2, #12]
	str r3, [r2, #16]
	bl Func_081434f8
	movs r1, #240
	ldr r4, [sp, #52]
	lsls r1, r1, #7
	adds r1, #232
	adds r2, r4, r1
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	movs r2, #1
	add r9, r2
	mov r3, r9
	cmp r3, #103
	beq .L_0816c6b4
	b .L_0816c3de
.L_0816c6b4:
	ldr r0, .L_0816c6f4
	bl Func_08014644
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	bl Func_08143bb8
	add sp, #104
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0816c6d4:
	.4byte gMapCellBuffer
.L_0816c6d8:
	.4byte gCameraSceneParameters
.L_0816c6dc:
	.4byte Data_08199364
.L_0816c6e0:
	.4byte Data_02014000
.L_0816c6e4:
	.4byte 0xffff0000
.L_0816c6e8:
	.4byte 0x0001bd50
.L_0816c6ec:
	.4byte 0xfffff448
.L_0816c6f0:
	.4byte Data_08199210
.L_0816c6f4:
	.4byte Func_08143000
