.syntax unified
	.thumb
	.global Func_0814a264
	.thumb_func
Func_0814a264:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #72
	str r0, [sp, #48]
	movs r3, #192
	lsls r3, r3, #18
	ldr r0, [r3, #92]
	str r0, [sp, #44]
	movs r0, #1
	ldr r1, [r3, #96]
	str r1, [sp, #40]
	ldr r3, [r3, #100]
	str r3, [sp, #28]
	bl BattleFx_BeginCanvasLayer
	ldr r3, .L_0814a2c8
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	ldr r2, [sp, #44]
	movs r3, #224
	lsls r3, r3, #3
	adds r1, r2, r3
	ldr r0, .L_0814a2cc
	movs r2, #1
	movs r3, #1
	bl Resource_LoadAndDecompress
	movs r3, #0
	ldr r0, .L_0814a2d0
	ldr r1, [sp, #28]
	movs r2, #0
	bl Resource_LoadAndDecompress
	ldr r5, [sp, #48]
	ldr r3, [r5, #24]
	cmp r3, #0
	bne .L_0814a2e6
	ldr r0, .L_0814a2d4
	bl Resource_GetTableEntry
	adds r1, r0, #0
	b .L_0814a2d8
	.2byte 0x0000
.L_0814a2c8:
	.4byte 0x00001010
.L_0814a2cc:
	.4byte 0x00000146
.L_0814a2d0:
	.4byte 0x00000134
.L_0814a2d4:
	.4byte 0x00000147
.L_0814a2d8:
	movs r0, #160
	ldr r3, .L_0814a5d4
	lsls r0, r0, #19
	movs r2, #128
	mov lr, r3
	.2byte 0xf800
	b .L_0814a2fe
.L_0814a2e6:
	cmp r3, #2
	bne .L_0814a2fe
	ldr r0, .L_0814a5d8
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_0814a5d4
	lsls r0, r0, #19
	movs r2, #128
	mov lr, r3
	.2byte 0xf800
.L_0814a2fe:
	movs r1, #19
	movs r0, #104
	bl Func_081963ec
	movs r5, #192
	lsls r5, r5, #18
	ldr r3, [r5, #104]
	movs r1, #23
	movs r0, #188
	str r3, [sp, #52]
	bl Func_081963ec
	adds r5, #188
	ldr r3, [r5]
	mov r7, sp
	adds r7, #52
	str r7, [sp, #24]
	str r3, [r7, #4]
	ldr r3, .L_0814a5dc
	movs r0, #0
	movs r2, #128
	mov r8, r0
	movs r1, #0
	lsls r2, r2, #3
.L_0814a32e:
	movs r5, #1
	add r8, r5
	str r1, [r3]
	adds r3, #28
	cmp r8, r2
	bne .L_0814a32e
	ldr r1, [sp, #48]
	movs r7, #36
	ldrsh r0, [r1, r7]
	bl GetBattleObjectSlotFar
	ldr r7, .L_0814a5e0
	ldr r6, [r0]
	ldr r5, [sp, #44]
	movs r2, #0
	mov r8, r2
.L_0814a34e:
	bl Random16
	movs r3, #15
	ands r3, r0
	adds r3, #72
	lsls r2, r3, #16
	movs r3, #0
	str r2, [r5]
	str r3, [r5, #4]
	ldr r0, [sp, #48]
	ldr r3, [r0, #24]
	lsls r3, r3, #2
	add r3, r8
	ldrsb r3, [r7, r3]
	lsls r3, r3, #16
	str r3, [r5, #8]
	ldr r3, [r6, #8]
	cmp r3, #0
	bge .L_0814a378
	negs r3, r2
	str r3, [r5]
.L_0814a378:
	movs r1, #1
	add r8, r1
	mov r2, r8
	adds r5, #28
	cmp r2, #4
	bne .L_0814a34e
	ldr r3, [sp, #44]
	movs r5, #239
	lsls r5, r5, #7
	adds r2, r3, r5
	movs r3, #2
	str r3, [r2]
	ldr r7, [sp, #44]
	movs r0, #238
	lsls r0, r0, #7
	adds r0, #132
	adds r2, r7, r0
	movs r3, #50
	movs r1, #200
	lsls r1, r1, #4
	str r3, [r2]
	ldr r0, .L_0814a5e4
	bl Scheduler_AddOrUpdateCallback
	movs r1, #0
	str r1, [sp, #36]
.L_0814a3ac:
	ldr r2, [sp, #48]
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #48]
	ldr r3, [r2, #24]
	cmp r3, #2
	bne .L_0814a3d2
	ldr r3, [sp, #36]
	cmp r3, #63
	bgt .L_0814a3d2
	ldr r3, [r2, #4]
	cmp r3, #0
	bne .L_0814a3cc
	ldrh r3, [r5, #54]
	adds r3, #192
	b .L_0814a3d0
.L_0814a3cc:
	ldrh r3, [r5, #54]
	subs r3, #192
.L_0814a3d0:
	strh r3, [r5, #54]
.L_0814a3d2:
	ldr r7, [sp, #36]
	cmp r7, #16
	bne .L_0814a3de
	movs r0, #134
	bl Func_081180e8
.L_0814a3de:
	bl Func_08014de4
	adds r1, r5, #0
	adds r0, r5, #0
	adds r1, #12
	bl Graphics_PrepareTransferInIwramWork
	ldr r0, [sp, #36]
	cmp r0, #63
	ble .L_0814a3f4
	b .L_0814a686
.L_0814a3f4:
	movs r1, #0
	str r1, [sp, #32]
	ldr r5, [sp, #48]
	ldr r3, .L_0814a5e8
	ldr r2, [r5, #24]
	ldrb r3, [r3, r2]
	cmp r3, #0
	bne .L_0814a406
	b .L_0814a686
.L_0814a406:
	ldr r3, [sp, #44]
	ldr r2, .L_0814a5ec
	movs r5, #224
	movs r1, #1
	lsls r5, r5, #3
	movs r7, #60
	ands r0, r1
	adds r5, r3, r5
	add r7, sp
	str r0, [sp, #16]
	str r2, [sp, #12]
	str r3, [sp, #8]
	str r5, [sp, #20]
	mov r9, r7
	mov r11, r9
.L_0814a424:
	mov r1, r9
	ldr r0, [sp, #8]
	bl Func_0815e1ec
	mov r7, r9
	ldr r3, [r7]
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	str r3, [r7]
	ldr r3, [r7, #4]
	subs r3, #8
	str r3, [r7, #4]
	ldr r0, [sp, #12]
	ldr r1, [sp, #36]
	ldrb r3, [r0]
	cmp r1, r3
	bne .L_0814a452
	movs r0, #145
	bl Audio_PlayCue
	ldr r2, [sp, #12]
	ldrb r3, [r2]
.L_0814a452:
	ldr r5, [sp, #36]
	adds r3, #4
	cmp r5, r3
	blt .L_0814a53c
	ldr r7, [sp, #32]
	lsls r0, r5, #4
	lsls r3, r7, #1
	adds r3, r3, r7
	lsls r3, r3, #3
	adds r3, r3, r7
	adds r0, r0, r3
	movs r1, #104
	bl __modsi3
	ldr r5, [sp, #32]
	mov r8, r0
	mov r3, r11
	movs r0, #1
	ldr r2, [r3]
	ldr r1, [sp, #24]
	ldr r3, [r3, #4]
	ands r5, r0
	movs r6, #34
	movs r0, #104
	str r0, [sp, #4]
	str r6, [sp, #0]
	mov r7, r8
	lsls r5, r5, #2
	adds r5, r5, r1
	subs r3, r3, r7
	ldr r1, [sp, #20]
	subs r2, #17
	subs r3, #104
	ldr r4, [r5]
	ldr r0, [sp, #40]
	mov lr, r4
	.2byte 0xf800
	mov r1, r11
	ldr r2, [r1]
	ldr r3, [r1, #4]
	str r6, [sp, #0]
	str r7, [sp, #4]
	subs r2, #17
	subs r3, r3, r7
	ldr r4, [r5]
	ldr r0, [sp, #40]
	ldr r1, [sp, #20]
	mov lr, r4
	.2byte 0xf800
	ldr r2, [sp, #16]
	cmp r2, #0
	beq .L_0814a4fc
	ldr r3, [sp, #44]
	movs r7, #160
	lsls r7, r7, #5
	mov r0, r11
	adds r7, #208
	ldr r2, [r0]
	adds r5, r3, r7
	ldr r3, [r0, #4]
	movs r7, #37
	movs r1, #20
	subs r2, #20
	subs r3, #24
	str r1, [sp, #0]
	str r7, [sp, #4]
	ldr r4, [sp, #52]
	adds r1, r5, #0
	ldr r0, [sp, #40]
	mov lr, r4
	.2byte 0xf800
	mov r0, r11
	ldr r3, [r0, #4]
	ldr r2, [r0]
	str r7, [sp, #4]
	ldr r7, [sp, #24]
	movs r1, #20
	str r1, [sp, #0]
	subs r3, #24
	ldr r4, [r7, #4]
	ldr r0, [sp, #40]
	adds r1, r5, #0
	mov lr, r4
	.2byte 0xf800
	b .L_0814a53c
.L_0814a4fc:
	ldr r0, [sp, #44]
	mov r3, r11
	movs r1, #184
	ldr r2, [r3]
	lsls r1, r1, #5
	ldr r3, [r3, #4]
	adds r1, #180
	adds r5, r0, r1
	movs r7, #20
	movs r0, #37
	subs r2, #20
	subs r3, #24
	str r7, [sp, #0]
	str r0, [sp, #4]
	ldr r4, [sp, #52]
	adds r1, r5, #0
	ldr r0, [sp, #40]
	mov lr, r4
	.2byte 0xf800
	mov r1, r11
	ldr r3, [r1, #4]
	ldr r2, [r1]
	str r7, [sp, #0]
	movs r7, #37
	ldr r0, [sp, #24]
	str r7, [sp, #4]
	subs r3, #24
	ldr r4, [r0, #4]
	adds r1, r5, #0
	ldr r0, [sp, #40]
	mov lr, r4
	.2byte 0xf800
.L_0814a53c:
	ldr r1, [sp, #12]
	ldr r5, [sp, #36]
	ldrb r3, [r1]
	ldr r2, .L_0814a5ec
	cmp r5, r3
	beq .L_0814a54e
	adds r3, #16
	cmp r5, r3
	blt .L_0814a61a
.L_0814a54e:
	movs r7, #0
	mov r10, r7
	ldr r7, .L_0814a5f0
	movs r0, #0
	mov r8, r0
.L_0814a558:
	ldr r3, [r7, #24]
	cmp r3, #0
	bne .L_0814a60a
	bl Random16
	movs r6, #192
	lsls r6, r6, #2
	adds r6, #255
	ands r6, r0
	bl Random16
	mov r2, r9
	ldr r3, [r2]
	movs r5, #254
	lsls r3, r3, #8
	str r3, [r7]
	lsls r5, r5, #7
	ldr r3, [r2, #4]
	ldr r1, .L_0814a5f4
	adds r5, #255
	ands r5, r0
	movs r0, #128
	lsls r0, r0, #5
	lsls r3, r3, #8
	adds r5, r5, r1
	adds r3, r3, r0
	str r3, [r7, #4]
	adds r0, r5, #0
	bl Trig_Sin
	adds r6, #32
	adds r3, r6, #0
	muls r3, r0
	asrs r3, r3, #15
	str r3, [r7, #8]
	adds r0, r5, #0
	bl Trig_Cos
	adds r3, r6, #0
	muls r3, r0
	lsls r3, r3, #1
	negs r3, r3
	asrs r3, r3, #15
	str r3, [r7, #16]
	ldr r2, [sp, #12]
	ldr r5, [sp, #36]
	ldrb r3, [r2]
	movs r1, #1
	add r10, r1
	cmp r5, r3
	bne .L_0814a5f8
	bl Random16
	movs r1, #7
	ands r0, r1
	adds r0, #48
	mov r2, r10
	str r0, [r7, #24]
	cmp r2, #200
	bne .L_0814a60a
	b .L_0814a618
	.2byte 0x0000
.L_0814a5d4:
	.4byte IwramCopyWords
.L_0814a5d8:
	.4byte 0x00000148
.L_0814a5dc:
	.4byte Data_02010018
.L_0814a5e0:
	.4byte Data_08197a23
.L_0814a5e4:
	.4byte Func_08143000
.L_0814a5e8:
	.4byte Data_08197a20
.L_0814a5ec:
	.4byte Data_08197a2f
.L_0814a5f0:
	.4byte gMapCellBuffer
.L_0814a5f4:
	.4byte 0xffffc000
.L_0814a5f8:
	bl Random16
	movs r3, #7
	ands r0, r3
	adds r0, #24
	mov r5, r10
	str r0, [r7, #24]
	cmp r5, #4
	beq .L_0814a618
.L_0814a60a:
	movs r0, #1
	movs r1, #128
	add r8, r0
	lsls r1, r1, #3
	adds r7, #28
	cmp r8, r1
	bne .L_0814a558
.L_0814a618:
	ldr r2, .L_0814a794
.L_0814a61a:
	ldr r5, [sp, #32]
	ldr r7, [sp, #36]
	ldrb r3, [r2, r5]
	cmp r7, r3
	bne .L_0814a666
	ldr r0, [sp, #44]
	movs r1, #238
	lsls r1, r1, #7
	adds r1, #168
	adds r3, r0, r1
	movs r2, #2
	str r2, [r3]
	ldr r5, [sp, #48]
	movs r2, #0
	ldr r3, [r5, #20]
	mov r8, r2
	cmp r3, #0
	beq .L_0814a666
	movs r5, #36
.L_0814a640:
	ldr r7, [sp, #48]
	movs r3, #8
	ldrsh r0, [r5, r7]
	movs r2, #5
	str r3, [sp, #0]
	movs r1, #10
	mov r3, r8
	bl Func_0814cd48
	ldrsh r0, [r5, r7]
	movs r1, #1
	bl Func_08118088
	movs r3, #1
	add r8, r3
	ldr r3, [r7, #20]
	adds r5, #2
	cmp r8, r3
	bne .L_0814a640
.L_0814a666:
	ldr r5, [sp, #12]
	ldr r7, [sp, #8]
	ldr r0, [sp, #32]
	adds r5, #1
	adds r7, #28
	adds r0, #1
	str r5, [sp, #12]
	str r7, [sp, #8]
	str r0, [sp, #32]
	ldr r1, [sp, #48]
	ldr r3, .L_0814a798
	ldr r2, [r1, #24]
	ldrb r3, [r3, r2]
	cmp r0, r3
	beq .L_0814a686
	b .L_0814a424
.L_0814a686:
	ldr r6, .L_0814a79c
	movs r2, #0
	mov r8, r2
.L_0814a68c:
	ldr r5, [r6, #24]
	cmp r5, #0
	ble .L_0814a734
	subs r3, r5, #1
	ldr r2, [r6, #8]
	str r3, [r6, #24]
	ldr r3, [r6]
	ldr r1, [r6, #16]
	adds r4, r3, r2
	ldr r3, [r6, #4]
	str r4, [r6]
	adds r0, r3, r1
	lsls r3, r2, #4
	subs r3, r3, r2
	lsls r3, r3, #2
	str r0, [r6, #4]
	cmp r3, #0
	bge .L_0814a6b2
	adds r3, #63
.L_0814a6b2:
	asrs r3, r3, #6
	str r3, [r6, #8]
	lsls r3, r1, #4
	subs r3, r3, r1
	lsls r3, r3, #2
	cmp r3, #0
	bge .L_0814a6c2
	adds r3, #63
.L_0814a6c2:
	asrs r3, r3, #6
	adds r2, r3, #0
	subs r2, #16
	str r2, [r6, #16]
	adds r3, r0, #0
	cmp r0, #0
	bge .L_0814a6d2
	adds r3, #255
.L_0814a6d2:
	asrs r3, r3, #8
	mov r12, r3
	cmp r3, #120
	ble .L_0814a6e6
	negs r3, r2
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	str r3, [r6, #16]
	b .L_0814a734
.L_0814a6e6:
	cmp r4, #0
	blt .L_0814a734
	asrs r7, r4, #8
	cmp r7, #126
	bgt .L_0814a734
	cmp r0, #0
	blt .L_0814a734
	adds r2, r5, #0
	subs r2, #17
	cmp r2, #0
	bge .L_0814a6fe
	adds r2, #7
.L_0814a6fe:
	asrs r5, r2, #3
	cmp r5, #0
	bgt .L_0814a706
	movs r5, #1
.L_0814a706:
	ldr r2, .L_0814a7a0
	lsls r4, r5, #1
	mov r3, r8
	movs r0, #1
	ands r0, r3
	subs r3, r4, #2
	ldrh r1, [r2, r3]
	ldr r2, [sp, #28]
	str r5, [sp, #0]
	adds r1, r2, r1
	lsrs r2, r5, #31
	adds r2, r5, r2
	asrs r2, r2, #1
	subs r2, r7, r2
	mov r7, r12
	subs r3, r7, r5
	str r4, [sp, #4]
	ldr r5, [sp, #24]
	lsls r0, r0, #2
	ldr r4, [r0, r5]
	ldr r0, [sp, #40]
	mov lr, r4
	.2byte 0xf800
.L_0814a734:
	movs r7, #1
	movs r0, #128
	add r8, r7
	lsls r0, r0, #3
	adds r6, #28
	cmp r8, r0
	bne .L_0814a68c
	movs r0, #16
	movs r1, #16
	bl Func_08158ce0
	bl Func_081434f8
	movs r3, #240
	ldr r1, [sp, #44]
	lsls r3, r3, #7
	adds r3, #232
	adds r2, r1, r3
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r5, [sp, #36]
	adds r5, #1
	str r5, [sp, #36]
	cmp r5, #96
	beq .L_0814a76e
	b .L_0814a3ac
.L_0814a76e:
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	ldr r0, .L_0814a7a4
	bl Scheduler_RemoveCallback
	bl Func_08143bb8
	add sp, #72
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0814a794:
	.4byte Data_08197a2f
.L_0814a798:
	.4byte Data_08197a20
.L_0814a79c:
	.4byte gMapCellBuffer
.L_0814a7a0:
	.4byte Data_08197410
.L_0814a7a4:
	.4byte Func_08143000
