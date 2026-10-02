.syntax unified
	.thumb
	.global Func_0814b350
	.thumb_func
Func_0814b350:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #168
	str r0, [sp, #72]
	movs r3, #192
	lsls r3, r3, #18
	ldr r0, [r3, #92]
	ldr r7, .L_0814b594
	str r0, [sp, #68]
	movs r0, #0
	ldr r1, [r3, #96]
	str r1, [sp, #64]
	ldr r3, [r3, #100]
	str r3, [sp, #56]
	bl BattleFx_BeginCanvasLayer
	ldr r2, [sp, #68]
	movs r3, #224
	lsls r3, r3, #3
	adds r1, r2, r3
	ldr r0, .L_0814b598
	movs r2, #1
	movs r3, #1
	bl Resource_LoadAndDecompress
	movs r2, #0
	ldr r1, [sp, #56]
	ldr r0, .L_0814b59c
	movs r3, #0
	bl Resource_LoadAndDecompress
	ldr r5, [sp, #72]
	mov r6, sp
	ldr r0, [r5, #4]
	movs r3, #1
	adds r6, #76
	eors r0, r3
	adds r1, r6, #0
	str r6, [sp, #52]
	bl Func_08144aac
	ldr r0, [r5, #8]
	bl GetBattleObjectSlotFar
	ldr r0, [r0]
	mov r9, r0
	movs r1, #36
	ldrsh r0, [r5, r1]
	bl GetBattleObjectSlotFar
	ldr r0, [r0]
	movs r2, #0
	str r0, [sp, #48]
	mov r10, r2
	mov r8, r2
.L_0814b3c8:
	bl Random16
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	adds r6, r0, #0
	ands r6, r3
	bl Random16
	mov r3, r8
	str r3, [r7]
	movs r5, #255
	ands r5, r0
	bl Random16
	movs r3, #31
	ands r3, r0
	adds r3, #20
	lsls r3, r3, #16
	mov r0, r8
	str r3, [r7, #4]
	str r0, [r7, #8]
	adds r0, r6, #0
	bl Trig_Sin
	adds r5, #128
	adds r3, r5, #0
	muls r3, r0
	mov r1, r8
	asrs r3, r3, #5
	str r3, [r7, #12]
	str r1, [r7, #16]
	adds r0, r6, #0
	bl Trig_Cos
	adds r3, r5, #0
	muls r3, r0
	asrs r3, r3, #5
	str r3, [r7, #20]
	movs r3, #1
	add r10, r3
	mov r2, r8
	mov r5, r10
	str r2, [r7, #24]
	adds r7, #28
	cmp r5, #64
	bne .L_0814b3c8
	ldr r6, [sp, #68]
	movs r0, #239
	movs r1, #238
	lsls r0, r0, #7
	lsls r1, r1, #7
	adds r2, r6, r0
	movs r3, #2
	adds r1, #132
	str r3, [r2]
	adds r2, r6, r1
	movs r3, #75
	movs r1, #200
	str r3, [r2]
	lsls r1, r1, #4
	ldr r0, .L_0814b5a0
	bl Scheduler_AddOrUpdateCallback
	mov r5, r9
	ldr r3, [r5, #8]
	add r2, sp, #156
	mov r11, r2
	str r3, [r2]
	mov r6, r11
	movs r2, #0
	str r2, [r6, #4]
	mov r0, sp
	ldr r3, [r5, #16]
	adds r0, #144
	str r3, [r6, #8]
	ldr r1, [sp, #48]
	str r0, [sp, #44]
	ldr r3, [sp, #44]
	ldr r0, [r1, #8]
	mov r6, sp
	str r0, [r3]
	ldr r5, [sp, #44]
	movs r3, #180
	adds r6, #132
	lsls r3, r3, #15
	str r2, [r5, #8]
	str r3, [r5, #4]
	str r6, [sp, #40]
	mov r1, r11
	ldr r3, [r1]
	movs r1, #40
	subs r0, r0, r3
	bl __divsi3
	str r0, [r6]
	mov r2, r11
	ldr r3, [r2, #4]
	ldr r0, [r5, #4]
	movs r1, #40
	subs r0, r0, r3
	bl __divsi3
	str r0, [r6, #4]
	movs r1, #40
	ldr r0, [r5, #8]
	mov r5, r11
	ldr r3, [r5, #8]
	subs r0, r0, r3
	bl __divsi3
	str r0, [r6, #8]
	movs r6, #0
	mov r9, r6
.L_0814b4ac:
	movs r3, #192
	lsls r3, r3, #18
	mov r0, r9
	ldr r5, [r3, #48]
	cmp r0, #8
	bne .L_0814b4be
	movs r0, #212
	bl Audio_PlayCue
.L_0814b4be:
	mov r1, r9
	cmp r1, #80
	bne .L_0814b4ca
	movs r0, #142
	bl Audio_PlayCue
.L_0814b4ca:
	bl Func_08014de4
	adds r1, r5, #0
	adds r1, #12
	adds r0, r5, #0
	bl Graphics_PrepareTransferInIwramWork
	mov r3, r9
	subs r3, #30
	cmp r3, #39
	bhi .L_0814b4fe
	ldr r5, [sp, #40]
	mov r2, r11
	ldr r3, [r2]
	ldr r2, [r5]
	mov r6, r11
	adds r3, r3, r2
	str r3, [r6]
	ldr r3, [r6, #4]
	ldr r2, [r5, #4]
	adds r3, r3, r2
	str r3, [r6, #4]
	ldr r3, [r6, #8]
	ldr r2, [r5, #8]
	adds r3, r3, r2
	str r3, [r6, #8]
.L_0814b4fe:
	mov r0, r11
	bl SceneTransform_ApplyPosition
	mov r0, r9
	cmp r0, #0
	bne .L_0814b520
	ldr r1, [sp, #72]
	mov r2, r9
	ldr r0, [r1, #8]
	movs r3, #1
	str r2, [sp, #0]
	movs r2, #1
	movs r1, #7
	negs r2, r2
	negs r3, r3
	bl Func_0814cd48
.L_0814b520:
	mov r3, r9
	cmp r3, #24
	bne .L_0814b53c
	ldr r5, [sp, #72]
	movs r2, #1
	movs r3, #1
	ldr r0, [r5, #8]
	movs r6, #0
	movs r1, #0
	negs r2, r2
	negs r3, r3
	str r6, [sp, #0]
	bl Func_0814cd48
.L_0814b53c:
	mov r1, r9
	negs r1, r1
	mov r2, r9
	str r1, [sp, #36]
	ldr r6, .L_0814b594
	movs r0, #0
	lsls r2, r2, #8
	mov r10, r0
	lsls r7, r1, #8
	mov r8, r2
.L_0814b550:
	cmp r9, r10
	ble .L_0814b64a
	ldr r3, [r6, #24]
	cmp r3, #0
	bne .L_0814b64a
	bl Graphics_SaveTransferWorkOnce
	mov r5, r10
	movs r3, #3
	ands r3, r5
	cmp r3, #1
	beq .L_0814b584
	cmp r3, #1
	bgt .L_0814b572
	cmp r3, #0
	beq .L_0814b57c
	b .L_0814b5b0
.L_0814b572:
	cmp r3, #2
	beq .L_0814b58c
	cmp r3, #3
	beq .L_0814b5a4
	b .L_0814b5b0
.L_0814b57c:
	mov r0, r8
	bl Func_08015068
	b .L_0814b5b0
.L_0814b584:
	adds r0, r7, #0
	bl SceneTransform_ApplyPitch
	b .L_0814b5b0
.L_0814b58c:
	adds r0, r7, #0
	bl Func_080150e4
	b .L_0814b5b0
.L_0814b594:
	.4byte gMapCellBuffer
.L_0814b598:
	.4byte 0x00000155
.L_0814b59c:
	.4byte 0x00000134
.L_0814b5a0:
	.4byte Func_08143000
.L_0814b5a4:
	adds r0, r7, #0
	bl SceneTransform_ApplyPitch
	adds r0, r7, #0
	bl Func_080150e4
.L_0814b5b0:
	add r5, sp, #108
	adds r1, r5, #0
	adds r0, r6, #0
	bl Func_0815e1ec
	ldr r3, [r5]
	asrs r3, r3, #1
	str r3, [r5]
	bl Func_08014ea8
	ldr r2, [r5, #8]
	cmp r2, #249
	bgt .L_0814b5d0
	movs r3, #250
	str r3, [r5, #8]
	movs r2, #250
.L_0814b5d0:
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #122
	cmp r2, r3
	ble .L_0814b5de
	str r3, [r5, #8]
	adds r2, r3, #0
.L_0814b5de:
	adds r3, r2, #0
	subs r3, #250
	cmp r3, #0
	bge .L_0814b5e8
	adds r3, #63
.L_0814b5e8:
	asrs r3, r3, #6
	movs r0, #9
	subs r0, r0, r3
	ldr r2, .L_0814b99c
	lsls r4, r0, #1
	subs r3, r4, #2
	ldrh r1, [r2, r3]
	ldr r2, [sp, #56]
	lsrs r3, r0, #31
	adds r1, r2, r1
	ldr r2, [r5]
	adds r3, r0, r3
	asrs r3, r3, #1
	subs r2, r2, r3
	ldr r3, [r5, #4]
	str r0, [sp, #0]
	subs r3, r3, r0
	str r4, [sp, #4]
	ldr r0, [sp, #64]
	ldr r4, [sp, #76]
	mov lr, r4
	.2byte 0xf800
	adds r0, r6, #0
	movs r1, #60
	movs r2, #0
	bl BattleFxKernels_IntegrateVector3
	mov r3, r10
	adds r3, #30
	cmp r9, r3
	ble .L_0814b64a
	ldr r2, [r6]
	ldr r3, [r6, #12]
	negs r2, r2
	asrs r2, r2, #8
	ldr r1, [r6, #4]
	adds r3, r3, r2
	str r3, [r6, #12]
	ldr r3, [r6, #16]
	negs r1, r1
	asrs r1, r1, #8
	ldr r0, [r6, #8]
	adds r3, r3, r1
	str r3, [r6, #16]
	ldr r3, [r6, #20]
	negs r0, r0
	asrs r0, r0, #8
	adds r3, r3, r0
	str r3, [r6, #20]
.L_0814b64a:
	ldr r5, [sp, #36]
	movs r1, #1
	lsls r3, r5, #5
	mov r0, r9
	add r10, r1
	adds r7, r7, r3
	mov r2, r10
	lsls r3, r0, #5
	add r8, r3
	adds r6, #28
	cmp r2, #32
	beq .L_0814b664
	b .L_0814b550
.L_0814b664:
	cmp r0, #82
	ble .L_0814b6ac
	add r6, sp, #120
	movs r3, #0
	str r3, [r6]
	lsls r0, r0, #10
	bl Trig_Sin
	movs r5, #0
	str r5, [r6, #8]
	lsls r0, r0, #2
	add r5, sp, #108
	str r0, [r6, #4]
	adds r1, r5, #0
	adds r0, r6, #0
	bl Func_0815e1ec
	ldr r2, [r5]
	movs r1, #20
	asrs r2, r2, #1
	str r2, [r5]
	ldr r3, [r5, #4]
	str r1, [sp, #0]
	movs r1, #34
	str r1, [sp, #4]
	ldr r6, [sp, #52]
	ldr r5, [sp, #68]
	ldr r4, [r6, #4]
	movs r6, #224
	lsls r6, r6, #3
	subs r2, #10
	subs r3, #17
	ldr r0, [sp, #64]
	adds r1, r5, r6
	mov lr, r4
	.2byte 0xf800
.L_0814b6ac:
	ldr r0, [sp, #68]
	movs r1, #240
	lsls r1, r1, #7
	adds r1, #232
	adds r2, r0, r1
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	movs r2, #1
	add r9, r2
	mov r3, r9
	cmp r3, #98
	beq .L_0814b6cc
	b .L_0814b4ac
.L_0814b6cc:
	movs r5, #0
	mov r10, r5
	ldr r5, .L_0814b9a0
	movs r6, #255
.L_0814b6d4:
	bl Random16
	ands r0, r6
	subs r0, #127
	lsls r0, r0, #15
	str r0, [r5]
	bl Random16
	movs r3, #127
	ands r3, r0
	adds r3, #64
	lsls r3, r3, #15
	str r3, [r5, #4]
	bl Random16
	ands r0, r6
	subs r0, #127
	lsls r0, r0, #15
	str r0, [r5, #8]
	movs r1, #128
	movs r0, #1
	movs r3, #0
	add r10, r0
	lsls r1, r1, #2
	str r3, [r5, #24]
	adds r5, #28
	cmp r10, r1
	bne .L_0814b6d4
	movs r2, #0
	ldr r0, .L_0814b9a4
	ldr r1, [sp, #56]
	bl Resource_LoadAndDecompress
	ldr r5, [sp, #72]
	movs r6, #72
	ldr r3, [r5, #20]
	movs r2, #0
	lsls r3, r3, #3
	negs r6, r6
	mov r9, r2
	cmp r3, r6
	bne .L_0814b72a
	b .L_0814b982
.L_0814b72a:
	ldr r2, [sp, #44]
	mov r0, sp
	mov r1, sp
	adds r0, #96
	adds r1, #84
	str r0, [sp, #16]
	str r1, [sp, #20]
	str r2, [sp, #24]
.L_0814b73a:
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #48]
	str r3, [sp, #32]
	bl Func_08014de4
	ldr r3, [sp, #32]
	ldr r0, [sp, #32]
	adds r3, #12
	adds r1, r3, #0
	str r3, [sp, #28]
	bl Graphics_PrepareTransferInIwramWork
	ldr r5, [sp, #72]
	ldr r3, [r5, #20]
	lsls r3, r3, #3
	adds r3, #40
	cmp r9, r3
	blt .L_0814b76c
	ldr r6, [sp, #24]
	movs r0, #128
	ldr r3, [r6, #4]
	lsls r0, r0, #11
	adds r3, r3, r0
	str r3, [r6, #4]
.L_0814b76c:
	ldr r1, [sp, #24]
	ldr r2, [sp, #16]
	ldr r3, [r1]
	str r3, [r2]
	ldr r3, [r1, #4]
	str r3, [r2, #4]
	mov r3, r9
	lsls r0, r3, #11
	bl Trig_Sin
	ldr r5, [sp, #24]
	lsls r2, r0, #2
	ldr r3, [r5, #8]
	ldr r6, [sp, #16]
	adds r2, r2, r0
	lsls r2, r2, #3
	adds r3, r3, r2
	str r3, [r6, #8]
	add r2, sp, #84
	adds r1, r2, #0
	ldr r0, [sp, #16]
	bl Func_0815e1ec
	ldr r2, [sp, #84]
	ldr r3, [sp, #88]
	ldr r5, [sp, #68]
	movs r1, #20
	movs r6, #224
	asrs r2, r2, #1
	str r1, [sp, #0]
	lsls r6, r6, #3
	movs r1, #34
	str r2, [sp, #84]
	subs r3, #17
	str r1, [sp, #4]
	subs r2, #10
	adds r1, r5, r6
	ldr r4, [sp, #76]
	ldr r0, [sp, #64]
	mov lr, r4
	.2byte 0xf800
	movs r0, #0
	str r0, [sp, #60]
	ldr r1, [sp, #72]
	ldr r3, [r1, #20]
	cmp r3, #0
	bne .L_0814b7cc
	b .L_0814b95c
.L_0814b7cc:
	movs r2, #36
	str r2, [sp, #12]
	str r0, [sp, #8]
.L_0814b7d2:
	ldr r3, [sp, #12]
	ldr r6, [sp, #72]
	ldrsh r0, [r3, r6]
	bl GetBattleObjectSlotFar
	ldr r5, [r0]
	ldr r0, [sp, #60]
	lsls r0, r0, #3
	mov r8, r0
	bl Func_08014de4
	ldr r0, [sp, #32]
	ldr r1, [sp, #28]
	bl Graphics_PrepareTransferInIwramWork
	ldr r3, [r5, #8]
	mov r1, r11
	str r3, [r1]
	movs r3, #160
	lsls r3, r3, #14
	str r3, [r1, #4]
	mov r0, r11
	ldr r3, [r5, #16]
	str r3, [r1, #8]
	bl SceneTransform_ApplyPosition
	mov r3, r8
	adds r3, #30
	cmp r9, r3
	bne .L_0814b814
	movs r0, #126
	bl Audio_PlayCue
.L_0814b814:
	mov r3, r8
	adds r3, #40
	cmp r9, r3
	bne .L_0814b832
	ldr r2, [sp, #12]
	ldr r5, [sp, #72]
	movs r1, #7
	ldrsh r0, [r2, r5]
	movs r3, #0
	movs r2, #1
	str r3, [sp, #0]
	negs r2, r2
	subs r3, #1
	bl Func_0814cd48
.L_0814b832:
	mov r3, r8
	adds r3, #64
	cmp r9, r3
	bne .L_0814b850
	ldr r2, [sp, #72]
	ldr r6, [sp, #12]
	movs r3, #0
	ldrsh r0, [r6, r2]
	movs r2, #1
	str r3, [sp, #0]
	movs r1, #0
	negs r2, r2
	subs r3, #1
	bl Func_0814cd48
.L_0814b850:
	cmp r9, r8
	ble .L_0814b93c
	mov r3, r9
	mov r5, r8
	subs r0, r3, r5
	lsls r0, r0, #9
	bl Func_08015068
	ldr r1, .L_0814b9a0
	ldr r0, [sp, #8]
	movs r6, #0
	mov r10, r6
	adds r6, r0, r1
.L_0814b86a:
	mov r2, r10
	lsrs r3, r2, #31
	add r3, r10
	asrs r3, r3, #1
	add r3, r8
	cmp r9, r3
	ble .L_0814b930
	ldr r3, [r6]
	asrs r3, r3, #8
	adds r0, r3, #0
	muls r0, r3
	ldr r3, [r6, #4]
	asrs r3, r3, #8
	adds r2, r3, #0
	muls r2, r3
	ldr r3, [r6, #8]
	adds r0, r0, r2
	asrs r3, r3, #8
	adds r5, r3, #0
	muls r5, r3
	adds r3, r5, #0
	adds r0, r0, r3
	ldr r3, .L_0814b9a8
	mov lr, r3
	.2byte 0xf800
	asrs r7, r0, #9
	cmp r7, #0
	beq .L_0814b930
	add r2, sp, #84
	adds r0, r6, #0
	adds r1, r2, #0
	bl Func_0815e1ec
	ldr r3, [sp, #84]
	movs r5, #58
	asrs r3, r3, #1
	str r3, [sp, #84]
	ldr r3, [sp, #92]
	adds r5, #255
	cmp r3, r5
	bgt .L_0814b8c4
	ldr r0, [sp, #20]
	movs r3, #157
	lsls r3, r3, #1
	str r3, [r0, #8]
.L_0814b8c4:
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #122
	cmp r3, r2
	ble .L_0814b8d2
	ldr r1, [sp, #20]
	str r2, [r1, #8]
.L_0814b8d2:
	mov r3, r10
	lsls r0, r3, #2
	movs r1, #9
	add r0, r9
	bl __modsi3
	ldr r2, .L_0814b9ac
	lsls r3, r0, #1
	ldrh r1, [r2, r3]
	ldr r3, .L_0814b9b0
	ldr r5, [sp, #56]
	ldrb r0, [r3, r0]
	ldr r3, [sp, #20]
	adds r1, r5, r1
	ldr r2, [r3]
	ldr r5, [sp, #52]
	ldr r3, [r3, #4]
	str r0, [sp, #0]
	str r0, [sp, #4]
	lsrs r4, r0, #1
	subs r2, r2, r4
	subs r3, r3, r4
	ldr r0, [sp, #64]
	ldr r4, [r5, #4]
	mov lr, r4
	.2byte 0xf800
	ldr r5, [r6]
	adds r1, r7, #0
	adds r0, r5, #0
	bl __divsi3
	subs r5, r5, r0
	str r5, [r6]
	ldr r5, [r6, #4]
	adds r1, r7, #0
	adds r0, r5, #0
	bl __divsi3
	subs r5, r5, r0
	str r5, [r6, #4]
	ldr r5, [r6, #8]
	adds r1, r7, #0
	adds r0, r5, #0
	bl __divsi3
	subs r5, r5, r0
	str r5, [r6, #8]
.L_0814b930:
	movs r0, #1
	add r10, r0
	mov r1, r10
	adds r6, #28
	cmp r1, #64
	bne .L_0814b86a
.L_0814b93c:
	ldr r2, [sp, #12]
	ldr r3, [sp, #8]
	ldr r6, [sp, #60]
	movs r5, #224
	lsls r5, r5, #3
	adds r3, r3, r5
	adds r2, #2
	adds r6, #1
	str r2, [sp, #12]
	str r3, [sp, #8]
	str r6, [sp, #60]
	ldr r0, [sp, #72]
	ldr r3, [r0, #20]
	cmp r6, r3
	beq .L_0814b95c
	b .L_0814b7d2
.L_0814b95c:
	ldr r1, [sp, #68]
	movs r3, #240
	lsls r3, r3, #7
	adds r3, #232
	adds r2, r1, r3
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r6, [sp, #72]
	movs r5, #1
	ldr r3, [r6, #20]
	add r9, r5
	lsls r3, r3, #3
	adds r3, #72
	cmp r9, r3
	beq .L_0814b982
	b .L_0814b73a
.L_0814b982:
	ldr r0, .L_0814b9b4
	bl Scheduler_RemoveCallback
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	bl Func_08143bb8
	add sp, #168
	b .L_0814b9b8
.L_0814b99c:
	.4byte Data_08197410
.L_0814b9a0:
	.4byte gMapCellBuffer
.L_0814b9a4:
	.4byte 0x0000017e
.L_0814b9a8:
	.4byte IwramFillWords + 0x74
.L_0814b9ac:
	.4byte Data_0819744c
.L_0814b9b0:
	.4byte Data_0819745e
.L_0814b9b4:
	.4byte Func_08143000
.L_0814b9b8:
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
