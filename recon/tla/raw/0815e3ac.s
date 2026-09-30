.syntax unified
	.thumb
	.global Func_0815e3ac
	.thumb_func
Func_0815e3ac:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #100
	str r0, [sp, #52]
	movs r3, #192
	lsls r3, r3, #18
	ldr r0, [r3, #92]
	ldr r5, [sp, #52]
	str r0, [sp, #48]
	ldr r1, [r3, #96]
	str r1, [sp, #44]
	ldr r2, [r3, #100]
	str r2, [sp, #36]
	ldr r3, [r3, #48]
	str r3, [sp, #32]
	ldr r3, [sp, #52]
	ldr r3, [r3]
	str r3, [sp, #28]
	ldr r0, [r5, #8]
	bl Owner_GetState
	str r0, [sp, #24]
	movs r0, #1
	bl WaitFrames
	bl Func_0813ba50
	bl Func_08143d80
	ldr r3, .L_0815e424
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #10
	strh r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r6, [sp, #28]
	subs r6, #5
	str r6, [sp, #20]
	cmp r6, #1
	bhi .L_0815e43a
	ldr r3, [r5, #4]
	cmp r3, #0
	bne .L_0815e428
	movs r1, #11
	movs r0, #104
	bl Func_081963ec
	movs r0, #188
	movs r1, #27
	bl Func_081963ec
	b .L_0815e464
	.2byte 0x0000
.L_0815e424:
	.4byte 0x00001f80
.L_0815e428:
	movs r1, #15
	movs r0, #104
	bl Func_081963ec
	movs r0, #188
	movs r1, #31
	bl Func_081963ec
	b .L_0815e464
.L_0815e43a:
	ldr r0, [sp, #52]
	ldr r3, [r0, #4]
	cmp r3, #0
	bne .L_0815e454
	movs r1, #3
	movs r0, #104
	bl Func_081963ec
	movs r0, #188
	movs r1, #19
	bl Func_081963ec
	b .L_0815e464
.L_0815e454:
	movs r1, #7
	movs r0, #104
	bl Func_081963ec
	movs r0, #188
	movs r1, #23
	bl Func_081963ec
.L_0815e464:
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #104]
	adds r3, #188
	str r2, [sp, #56]
	mov r1, sp
	ldr r3, [r3]
	adds r1, #56
	str r1, [sp, #12]
	movs r0, #1
	str r3, [r1, #4]
	bl WaitFrames
	ldr r2, [sp, #28]
	cmp r2, #4
	bne .L_0815e490
	ldr r3, [sp, #48]
	movs r5, #224
	lsls r5, r5, #3
	adds r1, r3, r5
	ldr r0, .L_0815e58c
	b .L_0815e4de
.L_0815e490:
	ldr r6, [sp, #28]
	cmp r6, #3
	bne .L_0815e4aa
	ldr r2, [sp, #48]
	movs r3, #224
	lsls r3, r3, #3
	adds r1, r2, r3
	ldr r0, .L_0815e590
	movs r2, #0
	movs r3, #0
	bl Func_08157cf4
	b .L_0815e4fa
.L_0815e4aa:
	ldr r5, [sp, #28]
	cmp r5, #6
	bhi .L_0815e4fa
	ldr r2, .L_0815e594
	lsls r3, r5, #2
	ldr r3, [r3, r2]
	mov pc, r3
.L_0815e4b8:
	.4byte .L_0815e4d4
	.4byte .L_0815e4d4
	.4byte .L_0815e4e8
	.4byte .L_0815e4fa
	.4byte .L_0815e4fa
	.4byte .L_0815e4d4
	.4byte .L_0815e4d4
.L_0815e4d4:
	ldr r6, [sp, #48]
	movs r2, #224
	lsls r2, r2, #3
	adds r1, r6, r2
	ldr r0, .L_0815e598
.L_0815e4de:
	movs r2, #1
	movs r3, #1
	bl Func_08157cf4
	b .L_0815e4fa
.L_0815e4e8:
	ldr r3, [sp, #48]
	movs r5, #224
	lsls r5, r5, #3
	adds r1, r3, r5
	ldr r0, .L_0815e59c
	movs r2, #1
	movs r3, #1
	bl Func_08157cf4
.L_0815e4fa:
	ldr r6, [sp, #52]
	ldr r3, [r6, #8]
	cmp r3, #7
	ble .L_0815e506
	ldr r0, .L_0815e5a0
	b .L_0815e508
.L_0815e506:
	ldr r0, .L_0815e5a4
.L_0815e508:
	bl Resource_GetTableEntry
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	movs r1, #160
	lsls r2, r2, #24
	adds r3, #212
	lsls r1, r1, #19
	adds r2, #32
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r0, #1
	bl WaitFrames
	ldr r0, .L_0815e5a8
	ldr r1, [sp, #36]
	movs r2, #0
	movs r3, #0
	bl Func_08157cf4
	ldr r0, .L_0815e5ac
	ldr r1, .L_0815e5b0
	movs r2, #1
	movs r3, #0
	bl Func_08157cf4
	ldr r0, [sp, #48]
	movs r1, #239
	lsls r1, r1, #7
	adds r2, r0, r1
	movs r3, #2
	str r3, [r2]
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #132
	adds r2, r0, r3
	movs r1, #200
	movs r3, #50
	str r3, [r2]
	lsls r1, r1, #4
	ldr r0, .L_0815e5b4
	bl Scheduler_AddOrUpdateCallback
	ldr r3, .L_0815e588
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #10
	strh r3, [r2]
	ldr r6, [sp, #52]
	mov r1, sp
	movs r5, #36
	ldrsh r0, [r6, r5]
	adds r1, #76
	str r1, [sp, #16]
	bl Func_0815e1fc
	ldr r3, [r6, #4]
	cmp r3, #0
	bne .L_0815e5b8
	ldr r3, [sp, #16]
	ldr r2, [r3]
	movs r3, #96
	b .L_0815e5be
.L_0815e588:
	.4byte 0x00001f81
.L_0815e58c:
	.4byte 0x0000012c
.L_0815e590:
	.4byte 0x00000189
.L_0815e594:
	.4byte .L_0815e4b8
.L_0815e598:
	.4byte 0x00000179
.L_0815e59c:
	.4byte 0x0000017a
.L_0815e5a0:
	.4byte 0x00000151
.L_0815e5a4:
	.4byte 0x0000010c
.L_0815e5a8:
	.4byte 0x00000137
.L_0815e5ac:
	.4byte 0x0000015c
.L_0815e5b0:
	.4byte gMapCellBuffer
.L_0815e5b4:
	.4byte Func_08143000
.L_0815e5b8:
	ldr r5, [sp, #16]
	movs r3, #32
	ldr r2, [r5]
.L_0815e5be:
	subs r3, r3, r2
	str r3, [sp, #40]
	ldr r6, [sp, #40]
	cmp r6, #0
	ble .L_0815e5cc
	movs r0, #0
	str r0, [sp, #40]
.L_0815e5cc:
	ldr r1, [sp, #40]
	movs r3, #128
	negs r3, r3
	cmp r1, r3
	bge .L_0815e5d8
	str r3, [sp, #40]
.L_0815e5d8:
	ldr r2, [sp, #16]
	ldr r5, [sp, #40]
	ldr r3, [r2]
	add r6, sp, #40
	adds r3, r3, r5
	str r3, [r2]
	ldrh r6, [r6]
	ldr r2, .L_0815e878
	movs r3, #80
	strh r3, [r2, #6]
	strh r6, [r2, #4]
	movs r0, #1
	bl WaitFrames
	ldr r2, [sp, #52]
	movs r7, #255
	movs r1, #36
	ldrsh r0, [r2, r1]
	bl GetBattleObjectSlotFar
	ldr r5, [sp, #52]
	ldr r6, [r0]
	movs r3, #36
	ldrsh r0, [r5, r3]
	bl Func_08118070
	lsrs r3, r0, #31
	adds r0, r0, r3
	asrs r0, r0, #1
	ldr r5, [sp, #48]
	mov r8, r0
	movs r0, #0
	mov r10, r0
.L_0815e61a:
	ldr r3, [r6, #8]
	str r3, [r5]
	ldr r3, [r6, #12]
	add r3, r8
	str r3, [r5, #4]
	ldr r3, [r6, #16]
	str r3, [r5, #8]
	bl Random16
	ands r0, r7
	lsls r0, r0, #10
	str r0, [r5, #12]
	bl Random16
	ands r0, r7
	subs r0, #32
	lsls r0, r0, #10
	str r0, [r5, #16]
	bl Random16
	ldr r3, [r5]
	ands r0, r7
	subs r0, #127
	lsls r0, r0, #10
	str r0, [r5, #20]
	cmp r3, #0
	ble .L_0815e656
	ldr r3, [r5, #12]
	negs r3, r3
	str r3, [r5, #12]
.L_0815e656:
	ldr r3, [r5, #12]
	movs r1, #1
	negs r3, r3
	str r3, [r5, #12]
	mov r3, r10
	add r10, r1
	adds r3, #16
	mov r2, r10
	str r3, [r5, #24]
	adds r5, #28
	cmp r2, #64
	bne .L_0815e61a
	ldr r6, [sp, #32]
	movs r5, #88
	adds r6, #12
	str r6, [sp, #8]
	movs r3, #0
	add r5, sp
	mov r9, r3
	mov r11, r5
.L_0815e67e:
	mov r0, r9
	cmp r0, #5
	bne .L_0815e6a4
	ldr r1, [sp, #24]
	movs r2, #165
	lsls r2, r2, #1
	adds r3, r1, r2
	ldrh r0, [r3]
	bl Func_08118058
	cmp r0, #0
	beq .L_0815e69e
	movs r0, #134
	bl Func_081180e8
	b .L_0815e6a4
.L_0815e69e:
	movs r0, #133
	bl Func_081180e8
.L_0815e6a4:
	mov r3, r9
	cmp r3, #4
	bne .L_0815e704
	ldr r5, [sp, #28]
	cmp r5, #7
	bne .L_0815e6be
	ldr r1, [sp, #52]
	movs r6, #36
	ldrsh r0, [r1, r6]
	movs r1, #4
	bl Func_08118088
	b .L_0815e704
.L_0815e6be:
	ldr r2, [sp, #20]
	cmp r2, #1
	bhi .L_0815e6d2
	ldr r5, [sp, #52]
	movs r1, #1
	movs r3, #36
	ldrsh r0, [r5, r3]
	bl Func_08118088
	b .L_0815e704
.L_0815e6d2:
	ldr r6, [sp, #28]
	cmp r6, #8
	bne .L_0815e6f8
	ldr r2, [sp, #52]
	movs r3, #192
	lsls r3, r3, #10
	movs r1, #36
	ldrsh r0, [r2, r1]
	str r3, [sp, #0]
	movs r3, #150
	str r3, [sp, #4]
	movs r2, #128
	movs r3, #128
	movs r1, #1
	lsls r2, r2, #9
	lsls r3, r3, #11
	bl Func_0815f000
	b .L_0815e704
.L_0815e6f8:
	ldr r5, [sp, #52]
	movs r1, #0
	movs r3, #36
	ldrsh r0, [r5, r3]
	bl Func_08118088
.L_0815e704:
	ldr r6, [sp, #52]
	add r2, sp, #88
	ldr r0, [r6, #8]
	adds r1, r2, #0
	bl Func_0815e20c
	ldr r3, [sp, #92]
	ldr r5, [sp, #28]
	adds r3, #16
	str r3, [sp, #92]
	cmp r5, #6
	bne .L_0815e722
	adds r3, #16
	mov r6, r11
	str r3, [r6, #4]
.L_0815e722:
	ldr r0, [sp, #28]
	cmp r0, #4
	bne .L_0815e79c
	mov r1, r9
	cmp r1, #11
	ble .L_0815e730
	b .L_0815e8ca
.L_0815e730:
	ldr r2, [sp, #52]
	ldr r3, [r2, #4]
	cmp r3, #0
	bne .L_0815e76a
	lsrs r2, r1, #31
	add r2, r9
	asrs r2, r2, #1
	movs r3, #5
	subs r3, r3, r2
	lsls r1, r3, #1
	adds r1, r1, r3
	ldr r3, [sp, #48]
	mov r6, r11
	ldr r2, [r6]
	ldr r0, [sp, #40]
	lsls r1, r1, #8
	adds r1, r3, r1
	movs r5, #224
	ldr r3, [r6, #4]
	lsls r5, r5, #3
	adds r2, r2, r0
	adds r1, r1, r5
	movs r0, #16
	movs r5, #48
	str r0, [sp, #4]
	subs r2, #48
	subs r3, #8
	str r5, [sp, #0]
	b .L_0815e86c
.L_0815e76a:
	mov r6, r9
	lsrs r2, r6, #31
	add r2, r9
	asrs r2, r2, #1
	movs r3, #5
	subs r3, r3, r2
	lsls r1, r3, #1
	ldr r0, [sp, #48]
	adds r1, r1, r3
	lsls r1, r1, #8
	movs r2, #224
	adds r1, r0, r1
	lsls r2, r2, #3
	mov r3, r11
	adds r1, r1, r2
	ldr r5, [sp, #40]
	ldr r2, [r3]
	ldr r3, [r3, #4]
	movs r0, #16
	movs r6, #48
	str r0, [sp, #4]
	adds r2, r2, r5
	subs r3, #8
	str r6, [sp, #0]
	b .L_0815e86c
.L_0815e79c:
	ldr r0, [sp, #28]
	cmp r0, #2
	bls .L_0815e7aa
	cmp r0, #5
	beq .L_0815e7aa
	cmp r0, #6
	bne .L_0815e81a
.L_0815e7aa:
	mov r1, r9
	cmp r1, #11
	ble .L_0815e7b2
	b .L_0815e8ca
.L_0815e7b2:
	ldr r2, [sp, #52]
	ldr r3, [r2, #4]
	cmp r3, #0
	bne .L_0815e7ea
	lsrs r3, r1, #31
	add r3, r9
	asrs r3, r3, #1
	lsls r1, r3, #3
	subs r1, r1, r3
	lsls r1, r1, #2
	subs r1, r1, r3
	ldr r3, [sp, #48]
	lsls r1, r1, #7
	ldr r2, [sp, #88]
	ldr r6, [sp, #40]
	adds r1, r3, r1
	ldr r3, [sp, #92]
	movs r0, #48
	movs r5, #224
	lsls r5, r5, #3
	adds r2, r2, r6
	str r0, [sp, #0]
	movs r0, #72
	str r0, [sp, #4]
	adds r1, r1, r5
	subs r2, #48
	subs r3, #40
	b .L_0815e86c
.L_0815e7ea:
	mov r1, r9
	lsrs r3, r1, #31
	add r3, r9
	asrs r3, r3, #1
	lsls r1, r3, #3
	subs r1, r1, r3
	ldr r2, [sp, #48]
	lsls r1, r1, #2
	subs r1, r1, r3
	lsls r1, r1, #7
	movs r3, #224
	adds r1, r2, r1
	lsls r3, r3, #3
	adds r1, r1, r3
	ldr r2, [sp, #88]
	ldr r5, [sp, #40]
	ldr r3, [sp, #92]
	movs r0, #72
	movs r6, #48
	str r0, [sp, #4]
	adds r2, r2, r5
	subs r3, #40
	str r6, [sp, #0]
	b .L_0815e86c
.L_0815e81a:
	ldr r0, [sp, #28]
	cmp r0, #3
	bne .L_0815e8ca
	mov r1, r9
	cmp r1, #17
	bgt .L_0815e8ca
	mov r0, r9
	movs r1, #3
	bl Math_Div
	ldr r2, [sp, #52]
	adds r6, r0, #0
	ldr r3, [r2, #4]
	cmp r3, #0
	bne .L_0815e88c
	ldr r2, .L_0815e87c
	lsls r3, r6, #1
	ldrh r1, [r2, r3]
	ldr r3, [sp, #48]
	mov r0, r11
	adds r1, r3, r1
	ldr r3, .L_0815e880
	ldr r2, [r0]
	ldrb r3, [r3, r6]
	movs r5, #224
	adds r2, r2, r3
	ldr r3, [sp, #40]
	lsls r5, r5, #3
	adds r2, r2, r3
	ldr r3, .L_0815e884
	adds r1, r1, r5
	ldrb r4, [r3, r6]
	mov r5, r11
	ldr r3, [r5, #4]
	lsrs r0, r4, #1
	subs r3, r3, r0
	ldr r0, .L_0815e888
	subs r2, #58
	ldrb r0, [r0, r6]
	str r4, [sp, #4]
	str r0, [sp, #0]
.L_0815e86c:
	ldr r4, [sp, #56]
	ldr r0, [sp, #44]
	mov lr, r4
	.2byte 0xf800
	b .L_0815e8ca
	.2byte 0x0000
.L_0815e878:
	.4byte Data_03001120
.L_0815e87c:
	.4byte Data_0819886c
.L_0815e880:
	.4byte Data_08198878
.L_0815e884:
	.4byte Data_08198866
.L_0815e888:
	.4byte Data_08198860
.L_0815e88c:
	ldr r2, .L_0815e9d0
	lsls r3, r6, #1
	ldrh r1, [r2, r3]
	ldr r0, [sp, #48]
	ldr r3, .L_0815e9d4
	movs r2, #224
	adds r1, r0, r1
	lsls r2, r2, #3
	mov r5, r11
	ldrb r3, [r3, r6]
	adds r1, r1, r2
	ldr r2, [r5]
	ldr r0, [sp, #40]
	subs r2, r2, r3
	ldr r3, .L_0815e9d8
	adds r2, r2, r0
	ldrb r5, [r3, r6]
	ldr r3, .L_0815e9dc
	subs r2, r2, r5
	ldrb r4, [r3, r6]
	mov r6, r11
	ldr r3, [r6, #4]
	lsrs r0, r4, #1
	subs r3, r3, r0
	str r4, [sp, #4]
	adds r2, #58
	str r5, [sp, #0]
	ldr r4, [sp, #56]
	ldr r0, [sp, #44]
	mov lr, r4
	.2byte 0xf800
.L_0815e8ca:
	mov r5, r9
	subs r5, #4
	cmp r5, #11
	bhi .L_0815e8fc
	lsrs r3, r5, #31
	adds r3, r5, r3
	asrs r3, r3, #1
	lsls r1, r3, #4
	subs r1, r1, r3
	ldr r3, [sp, #16]
	ldr r0, .L_0815e9e0
	ldr r2, [r3]
	ldr r3, [r3, #4]
	lsls r1, r1, #7
	adds r1, r1, r0
	movs r6, #48
	movs r0, #40
	str r0, [sp, #0]
	subs r2, #16
	subs r3, #24
	str r6, [sp, #4]
	ldr r4, [sp, #56]
	ldr r0, [sp, #44]
	mov lr, r4
	.2byte 0xf800
.L_0815e8fc:
	bl Func_08014de4
	ldr r0, [sp, #32]
	ldr r1, [sp, #8]
	bl Func_080156e8
	cmp r5, #27
	bhi .L_0815e986
	movs r1, #64
	movs r0, #0
	add r1, sp
	mov r10, r0
	mov r8, r1
.L_0815e916:
	mov r2, r10
	lsrs r3, r2, #31
	add r3, r10
	asrs r6, r3, #1
	ldr r5, [sp, #48]
	lsls r3, r6, #3
	subs r3, r3, r6
	lsls r3, r3, #2
	adds r7, r5, r3
	ldr r5, [r7, #24]
	cmp r5, #0
	ble .L_0815e97c
	mov r1, r8
	adds r0, r7, #0
	bl Func_08015778
	mov r0, r8
	ldr r2, [r0]
	ldr r1, [sp, #40]
	asrs r5, r5, #3
	adds r5, #2
	adds r2, r2, r1
	ldr r1, .L_0815e9e4
	lsls r4, r5, #1
	str r2, [r0]
	subs r3, r4, #2
	ldrh r1, [r1, r3]
	ldr r3, [sp, #36]
	movs r0, #1
	ands r0, r6
	mov r6, r8
	adds r1, r3, r1
	ldr r3, [r6, #4]
	subs r2, r2, r5
	subs r3, r3, r5
	str r4, [sp, #0]
	str r4, [sp, #4]
	ldr r5, [sp, #12]
	lsls r0, r0, #2
	ldr r4, [r0, r5]
	ldr r0, [sp, #44]
	mov lr, r4
	.2byte 0xf800
	adds r0, r7, #0
	movs r1, #60
	ldr r2, .L_0815e9e8
	bl BattleFxKernels_IntegrateVector3
	ldr r3, [r7, #24]
	subs r3, #1
	str r3, [r7, #24]
.L_0815e97c:
	movs r6, #1
	add r10, r6
	mov r0, r10
	cmp r0, #64
	bne .L_0815e916
.L_0815e986:
	ldr r1, [sp, #48]
	movs r3, #240
	lsls r3, r3, #7
	adds r3, #232
	movs r5, #1
	adds r2, r1, r3
	add r9, r5
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	mov r6, r9
	bl WaitFrames
	cmp r6, #32
	beq .L_0815e9a6
	b .L_0815e67e
.L_0815e9a6:
	ldr r0, .L_0815e9ec
	bl Scheduler_RemoveCallback
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	ldr r3, .L_0815e9f0
	mov r0, r9
	strh r0, [r3, #6]
	bl Func_08143d04
	add sp, #100
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0815e9d0:
	.4byte Data_0819886c
.L_0815e9d4:
	.4byte Data_08198878
.L_0815e9d8:
	.4byte Data_08198860
.L_0815e9dc:
	.4byte Data_08198866
.L_0815e9e0:
	.4byte gMapCellBuffer
.L_0815e9e4:
	.4byte Data_08197424
.L_0815e9e8:
	.4byte 0xfffffc00
.L_0815e9ec:
	.4byte Func_08143000
.L_0815e9f0:
	.4byte Data_03001120
