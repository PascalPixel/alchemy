.syntax unified
	.thumb
	.global Func_0817d174
	.thumb_func
Func_0817d174:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #76
	str r0, [sp, #52]
	movs r3, #192
	lsls r3, r3, #18
	ldr r0, [r3, #96]
	movs r5, #0
	str r0, [sp, #48]
	movs r0, #1
	ldr r1, [r3, #92]
	mov r9, r5
	str r1, [sp, #44]
	ldr r3, [r3, #100]
	str r3, [sp, #36]
	bl BattleFx_BeginCanvasLayer
	ldr r3, .L_0817d1cc
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	add r1, sp, #56
	movs r0, #0
	bl Func_08144aac
	ldr r2, [sp, #44]
	movs r3, #224
	lsls r3, r3, #3
	adds r1, r2, r3
	ldr r0, .L_0817d1d0
	movs r2, #1
	movs r3, #1
	bl Resource_LoadAndDecompress
	ldr r7, [sp, #44]
	str r5, [sp, #24]
	mov r10, r7
	b .L_0817d1d4
.L_0817d1cc:
	.4byte 0x00001010
.L_0817d1d0:
	.4byte 0x00000126
.L_0817d1d4:
	movs r0, #200
	ldr r3, [sp, #24]
	lsls r0, r0, #5
	add r0, r10
	movs r1, #141
	add r3, r9
	str r0, [sp, #8]
	movs r5, #0
	lsls r1, r1, #3
	lsls r3, r3, #3
	mov r8, r1
	mov lr, r5
	mov r11, r3
.L_0817d1ee:
	ldr r7, [sp, #8]
	mov r2, lr
	adds r3, r2, r5
	lsls r3, r3, #3
	adds r3, r3, r7
	movs r0, #23
	adds r0, r0, r3
	adds r4, r3, #0
	mov r3, r11
	ldr r1, [sp, #44]
	add r3, lr
	adds r2, r7, #0
	adds r3, r3, r5
	lsls r3, r3, #3
	add r2, r8
	movs r7, #224
	mov r12, r0
	adds r3, r3, r1
	adds r0, r2, #0
	lsls r7, r7, #3
	movs r6, #0
	adds r0, #23
	adds r1, r3, r7
.L_0817d21c:
	ldrb r3, [r1, #1]
	mov r7, r12
	strb r3, [r4]
	adds r6, #1
	ldrb r3, [r1]
	adds r4, #1
	strb r3, [r7]
	movs r3, #1
	negs r3, r3
	add r12, r3
	ldrb r3, [r1, #1]
	strb r3, [r2]
	adds r2, #1
	ldrb r3, [r1]
	adds r1, #2
	strb r3, [r0]
	subs r0, #1
	cmp r6, #12
	bne .L_0817d21c
	movs r7, #24
	negs r7, r7
	movs r0, #2
	adds r5, #1
	add r8, r7
	add lr, r0
	cmp r5, #24
	bne .L_0817d1ee
	ldr r1, [sp, #24]
	movs r3, #1
	movs r2, #144
	add r9, r3
	adds r1, #8
	lsls r2, r2, #3
	mov r5, r9
	str r1, [sp, #24]
	add r10, r2
	cmp r5, #2
	bne .L_0817d1d4
	ldr r7, [sp, #44]
	movs r2, #142
	lsls r2, r2, #7
	adds r1, r7, r2
	ldr r0, .L_0817d5fc
	movs r2, #1
	movs r3, #0
	bl Resource_LoadAndDecompress
	movs r2, #0
	ldr r1, [sp, #36]
	movs r3, #0
	ldr r0, .L_0817d600
	bl Resource_LoadAndDecompress
	ldr r0, .L_0817d604
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_0817d608
	movs r2, #128
	lsls r0, r0, #19
	mov lr, r3
	.2byte 0xf800
	movs r5, #239
	lsls r5, r5, #7
	adds r3, r7, r5
	mov r7, r9
	str r7, [r3]
	ldr r0, [sp, #44]
	movs r1, #238
	lsls r1, r1, #7
	adds r1, #132
	adds r2, r0, r1
	movs r3, #50
	movs r1, #200
	str r3, [r2]
	ldr r0, .L_0817d60c
	lsls r1, r1, #4
	bl Scheduler_AddOrUpdateCallback
	movs r2, #0
	str r2, [sp, #40]
.L_0817d2c0:
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #48]
	mov r10, r3
	ldr r3, [sp, #40]
	cmp r3, #0
	bne .L_0817d378
	ldr r6, [sp, #44]
	movs r7, #255
	movs r5, #0
	mov r8, r7
	mov r9, r5
	movs r7, #0
.L_0817d2da:
	ldr r1, [sp, #52]
	ldr r0, [r1, #8]
	bl GetBattleObjectSlotFar
	ldr r2, [sp, #52]
	ldr r5, [r0]
	ldr r0, [r2, #8]
	bl Battle_GetObjectTableValueFar
	lsrs r3, r0, #31
	adds r0, r0, r3
	ldr r3, [r5, #8]
	asrs r0, r0, #1
	str r3, [r6]
	ldr r3, [r5, #12]
	adds r3, r3, r0
	str r3, [r6, #4]
	ldr r3, [r5, #16]
	str r3, [r6, #8]
	bl Random16
	mov r3, r8
	ands r0, r3
	ldr r3, [r6]
	subs r0, #128
	lsls r0, r0, #8
	str r0, [r6, #12]
	cmp r3, #0
	ble .L_0817d318
	negs r3, r0
	str r3, [r6, #12]
.L_0817d318:
	str r7, [r6, #12]
	bl Random16
	movs r1, #200
	lsls r1, r1, #1
	bl Math_ModU
	subs r0, #128
	lsls r0, r0, #9
	str r0, [r6, #16]
	bl Random16
	mov r5, r8
	ands r0, r5
	subs r0, #128
	lsls r0, r0, #10
	str r0, [r6, #20]
	movs r0, #1
	add r9, r0
	mov r1, r9
	str r7, [r6, #24]
	adds r6, #28
	cmp r1, #6
	bne .L_0817d2da
	ldr r3, [sp, #44]
	movs r2, #0
	mov r9, r2
	adds r3, #192
	subs r2, #1
.L_0817d352:
	movs r5, #1
	add r9, r5
	mov r7, r9
	str r2, [r3]
	adds r3, #28
	cmp r7, #48
	bne .L_0817d352
	ldr r3, .L_0817d610
	movs r0, #0
	movs r2, #1
	mov r9, r0
	negs r2, r2
.L_0817d36a:
	movs r1, #1
	add r9, r1
	mov r5, r9
	str r2, [r3]
	adds r3, #28
	cmp r5, #192
	bne .L_0817d36a
.L_0817d378:
	bl Func_08014de4
	mov r1, r10
	adds r1, #12
	mov r0, r10
	bl Graphics_PrepareTransferInIwramWork
	movs r1, #0
	ldr r0, [sp, #44]
	str r1, [sp, #20]
	movs r7, #0
	mov r9, r7
	mov r8, r0
	mov r11, r1
.L_0817d394:
	ldr r2, [sp, #52]
	mov r0, r9
	ldr r1, [r2, #20]
	bl Math_Mod
	lsls r3, r0, #1
	str r3, [sp, #28]
	ldr r7, [sp, #52]
	str r0, [sp, #32]
	adds r5, r3, #0
	adds r5, #36
	ldrsh r0, [r7, r5]
	bl GetBattleObjectSlotFar
	ldr r2, [sp, #52]
	ldr r7, [r0]
	ldrsh r0, [r2, r5]
	bl Battle_GetObjectTableValueFar
	mov r5, r8
	lsrs r3, r0, #31
	adds r0, r0, r3
	ldr r3, [r5, #24]
	asrs r0, r0, #1
	mov r10, r0
	cmp r3, #0
	bge .L_0817d3cc
	b .L_0817d574
.L_0817d3cc:
	ldr r5, [sp, #40]
	add r6, sp, #64
	add r5, r9
	lsrs r3, r5, #31
	adds r5, r5, r3
	adds r1, r6, #0
	movs r3, #1
	asrs r5, r5, #1
	mov r0, r8
	ands r5, r3
	bl Func_0815e1ec
	ldr r2, [r6]
	lsls r1, r5, #3
	asrs r2, r2, #1
	str r2, [r6]
	ldr r0, [sp, #44]
	adds r1, r1, r5
	lsls r1, r1, #7
	movs r3, #200
	adds r1, r0, r1
	lsls r3, r3, #5
	adds r1, r1, r3
	ldr r3, [r6, #4]
	movs r0, #24
	str r0, [sp, #0]
	movs r0, #48
	subs r3, #24
	str r0, [sp, #4]
	subs r2, #12
	ldr r4, [sp, #56]
	ldr r0, [sp, #48]
	mov lr, r4
	.2byte 0xf800
	mov r0, r8
	movs r1, #63
	movs r2, #0
	bl BattleFxKernels_IntegrateVector3
	ldr r5, [sp, #40]
	mov r3, r11
	adds r3, #10
	cmp r5, r3
	bgt .L_0817d426
	b .L_0817d574
.L_0817d426:
	ldr r2, [r7, #12]
	mov r1, r8
	ldr r3, [r1, #4]
	mov r5, r8
	add r2, r10
	ldr r4, [r1]
	subs r2, r2, r3
	ldr r1, [r7, #16]
	ldr r3, [r5, #8]
	ldr r0, [r7, #8]
	subs r1, r1, r3
	ldr r3, [r5, #12]
	subs r0, r0, r4
	asrs r0, r0, #10
	adds r3, r3, r0
	str r3, [r5, #12]
	ldr r3, [r5, #16]
	asrs r2, r2, #10
	adds r3, r3, r2
	str r3, [r5, #16]
	ldr r3, [r5, #20]
	asrs r1, r1, #10
	adds r3, r3, r1
	str r3, [r5, #20]
	cmp r4, #0
	bge .L_0817d45c
	negs r4, r4
.L_0817d45c:
	ldr r0, [r7, #8]
	cmp r0, #0
	bge .L_0817d464
	negs r0, r0
.L_0817d464:
	cmp r4, r0
	bgt .L_0817d46a
	b .L_0817d574
.L_0817d46a:
	mov r0, r11
	lsls r3, r0, #3
	ldr r2, [sp, #44]
	subs r3, r3, r0
	lsls r3, r3, #2
	adds r3, r3, r2
	adds r5, r3, #0
	movs r1, #0
	adds r7, r6, #0
	adds r5, #168
.L_0817d47e:
	ldr r3, [r7]
	str r1, [sp, #16]
	lsls r3, r3, #16
	str r3, [r5]
	ldr r3, [r7, #4]
	lsls r3, r3, #16
	str r3, [r5, #4]
	bl Random16
	movs r3, #127
	ands r3, r0
	subs r3, #64
	lsls r3, r3, #12
	str r3, [r5, #12]
	bl Random16
	movs r2, #255
	ands r2, r0
	movs r3, #192
	ldr r1, [sp, #16]
	subs r3, r3, r2
	lsls r3, r3, #10
	str r3, [r5, #16]
	adds r1, #1
	movs r3, #0
	str r3, [r5, #24]
	adds r5, #28
	cmp r1, #4
	bne .L_0817d47e
	ldr r3, [sp, #20]
	ldr r5, .L_0817d614
	movs r1, #0
	mov r10, r6
	adds r7, r3, r5
.L_0817d4c2:
	mov r0, r10
	ldr r3, [r0]
	str r1, [sp, #16]
	lsls r3, r3, #16
	str r3, [r7]
	movs r6, #254
	ldr r3, [r0, #4]
	lsls r6, r6, #7
	lsls r3, r3, #16
	str r3, [r7, #4]
	bl Random16
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
	asrs r3, r3, #5
	str r3, [r7, #12]
	adds r0, r6, #0
	bl Trig_Cos
	adds r3, r5, #0
	muls r3, r0
	asrs r3, r3, #5
	str r3, [r7, #16]
	bl Random16
	ldr r1, [sp, #16]
	movs r3, #15
	ands r3, r0
	adds r3, #16
	adds r1, #1
	str r3, [r7, #24]
	adds r7, #28
	cmp r1, #32
	bne .L_0817d4c2
	movs r0, #134
	bl Func_081180e8
	movs r0, #145
	bl Audio_PlayCue
	ldr r5, [sp, #28]
	ldr r3, [sp, #52]
	adds r5, #36
	mov r1, r9
	ldrsh r0, [r3, r5]
	lsls r3, r1, #2
	add r3, r9
	adds r3, #110
	str r3, [sp, #4]
	movs r2, #128
	movs r3, #128
	lsls r2, r2, #10
	lsls r3, r3, #12
	movs r1, #1
	str r2, [sp, #0]
	bl Func_0815f000
	ldr r2, [sp, #52]
	movs r1, #7
	ldrsh r0, [r2, r5]
	movs r3, #16
	str r3, [sp, #0]
	movs r2, #5
	ldr r3, [sp, #32]
	bl Func_0814cd48
	movs r7, #238
	ldr r5, [sp, #44]
	lsls r7, r7, #7
	adds r7, #168
	movs r3, #4
	adds r2, r5, r7
	str r3, [r2]
	mov r0, r8
	subs r3, #5
	str r3, [r0, #24]
.L_0817d574:
	ldr r3, [sp, #20]
	movs r5, #224
	movs r7, #1
	lsls r5, r5, #2
	add r9, r7
	movs r1, #28
	movs r2, #8
	adds r3, r3, r5
	mov r0, r9
	add r8, r1
	add r11, r2
	str r3, [sp, #20]
	cmp r0, #6
	beq .L_0817d592
	b .L_0817d394
.L_0817d592:
	ldr r5, [sp, #44]
	movs r1, #0
	mov r9, r1
	adds r5, #168
.L_0817d59a:
	ldr r1, [r5, #24]
	cmp r1, #23
	bhi .L_0817d5e4
	cmp r1, #1
	ble .L_0817d5d4
	cmp r1, #0
	bge .L_0817d5aa
	adds r1, #3
.L_0817d5aa:
	ldr r2, [sp, #44]
	asrs r1, r1, #2
	lsls r1, r1, #11
	movs r3, #142
	adds r1, r2, r1
	lsls r3, r3, #7
	movs r7, #2
	ldrsh r2, [r5, r7]
	adds r1, r1, r3
	movs r0, #6
	ldrsh r3, [r5, r0]
	movs r0, #32
	str r0, [sp, #0]
	movs r0, #64
	str r0, [sp, #4]
	subs r2, #16
	subs r3, #32
	ldr r4, [sp, #56]
	ldr r0, [sp, #48]
	mov lr, r4
	.2byte 0xf800
.L_0817d5d4:
	adds r0, r5, #0
	movs r1, #60
	ldr r2, .L_0817d618
	bl BattleFxKernels_IntegrateVector2
	ldr r3, [r5, #24]
	adds r3, #1
	str r3, [r5, #24]
.L_0817d5e4:
	movs r1, #1
	add r9, r1
	mov r2, r9
	adds r5, #28
	cmp r2, #48
	bne .L_0817d59a
	ldr r6, .L_0817d61c
	ldr r5, .L_0817d614
	movs r3, #0
	mov r9, r3
	b .L_0817d620
	.2byte 0x0000
.L_0817d5fc:
	.4byte 0x0000013e
.L_0817d600:
	.4byte 0x00000134
.L_0817d604:
	.4byte 0x00000148
.L_0817d608:
	.4byte IwramCopyWords
.L_0817d60c:
	.4byte Func_08143000
.L_0817d610:
	.4byte Data_02010018
.L_0817d614:
	.4byte gMapCellBuffer
.L_0817d618:
	.4byte 0xfffff000
.L_0817d61c:
	.4byte Data_08197410
.L_0817d620:
	ldr r0, [r5, #24]
	cmp r0, #0
	blt .L_0817d664
	asrs r0, r0, #3
	adds r0, #1
	lsls r4, r0, #1
	subs r3, r4, #2
	ldrh r1, [r6, r3]
	ldr r7, [sp, #36]
	movs r3, #2
	ldrsh r2, [r5, r3]
	lsrs r3, r0, #31
	adds r3, r0, r3
	asrs r3, r3, #1
	adds r1, r7, r1
	subs r2, r2, r3
	movs r7, #6
	ldrsh r3, [r5, r7]
	str r0, [sp, #0]
	subs r3, r3, r0
	str r4, [sp, #4]
	ldr r0, [sp, #48]
	ldr r4, [sp, #56]
	mov lr, r4
	.2byte 0xf800
	movs r2, #128
	adds r0, r5, #0
	movs r1, #60
	lsls r2, r2, #5
	bl BattleFxKernels_IntegrateVector2
	ldr r3, [r5, #24]
	subs r3, #1
	str r3, [r5, #24]
.L_0817d664:
	movs r0, #1
	add r9, r0
	mov r1, r9
	adds r5, #28
	cmp r1, #192
	bne .L_0817d620
	movs r0, #8
	movs r1, #8
	bl Func_08158ce0
	bl Func_081434f8
	movs r5, #240
	ldr r3, [sp, #44]
	lsls r5, r5, #7
	adds r5, #232
	adds r2, r3, r5
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r7, [sp, #40]
	adds r7, #1
	str r7, [sp, #40]
	cmp r7, #144
	beq .L_0817d69c
	b .L_0817d2c0
.L_0817d69c:
	ldr r0, .L_0817d6c0
	bl Scheduler_RemoveCallback
	movs r0, #188
	bl Runtime_ReleaseHeapBlock
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	bl Func_08143bb8
	add sp, #76
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0817d6c0:
	.4byte Func_08143000
