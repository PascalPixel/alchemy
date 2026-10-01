.syntax unified
	.thumb
	.global Func_0814e744
	.thumb_func
Func_0814e744:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r5, #192
	lsls r5, r5, #18
	mov r11, r0
	ldr r0, [r5, #92]
	sub sp, #60
	str r0, [sp, #32]
	movs r0, #1
	ldr r1, [r5, #96]
	str r1, [sp, #28]
	ldr r2, [r5, #100]
	str r2, [sp, #16]
	ldr r6, [r5, #48]
	bl BattleFx_BeginCanvasLayer
	movs r2, #0
	ldr r1, [sp, #16]
	movs r3, #0
	ldr r0, .L_0814ea1c
	bl Resource_LoadAndDecompress
	ldr r0, .L_0814ea20
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_0814ea24
	movs r2, #128
	lsls r0, r0, #19
	mov lr, r3
	.2byte 0xf800
	movs r1, #19
	movs r0, #104
	bl Func_081963ec
	ldr r5, [r5, #104]
	movs r3, #0
	mov r8, r3
	str r5, [sp, #24]
	ldr r3, .L_0814ea28
	movs r1, #1
	movs r2, #128
	negs r1, r1
	lsls r2, r2, #3
.L_0814e7a8:
	movs r4, #1
	add r8, r4
	str r1, [r3]
	adds r3, #28
	cmp r8, r2
	bne .L_0814e7a8
	bl Func_08014de4
	adds r1, r6, #0
	adds r1, #12
	adds r0, r6, #0
	bl Graphics_PrepareTransferInIwramWork
	movs r0, #0
	str r0, [sp, #20]
	mov r1, r11
	ldr r3, [r1, #20]
	cmp r3, #0
	beq .L_0814e8b4
	movs r3, #36
	movs r2, #36
	movs r4, #48
	str r3, [sp, #12]
	str r0, [sp, #8]
	add r2, sp
	add r4, sp
	mov r10, r2
	mov r9, r4
.L_0814e7e0:
	ldr r1, [sp, #12]
	mov r3, r11
	ldrsh r0, [r1, r3]
	bl GetBattleObjectSlotFar
	ldr r4, [sp, #12]
	mov r2, r11
	ldr r5, [r0]
	ldrsh r0, [r4, r2]
	bl Battle_GetObjectTableValueFar
	lsrs r3, r0, #31
	adds r0, r0, r3
	ldr r3, [r5, #8]
	mov r4, r9
	asrs r0, r0, #1
	str r0, [r4, #4]
	str r3, [r4]
	mov r1, r10
	ldr r3, [r5, #16]
	mov r0, r9
	str r3, [r4, #8]
	bl Func_0815e1ec
	mov r0, r10
	ldr r3, [r0]
	movs r1, #0
	asrs r3, r3, #1
	str r3, [r0]
	ldr r2, [sp, #8]
	ldr r3, .L_0814ea2c
	mov r8, r1
	adds r7, r2, r3
.L_0814e822:
	bl Random16
	movs r4, #255
	adds r6, r0, #0
	ands r6, r4
	bl Random16
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	adds r5, r0, #0
	ands r5, r3
	adds r0, r5, #0
	bl Trig_Sin
	adds r2, r6, #0
	muls r2, r0
	mov r0, r10
	ldr r3, [r0]
	asrs r2, r2, #7
	lsls r3, r3, #16
	adds r2, r2, r3
	str r2, [r7]
	adds r0, r5, #0
	bl Trig_Cos
	mov r1, r10
	ldr r3, [r1, #4]
	adds r2, r6, #0
	muls r2, r0
	lsls r3, r3, #16
	asrs r2, r2, #3
	adds r2, r2, r3
	str r2, [r7, #4]
	bl Random16
	movs r2, #255
	ands r0, r2
	movs r3, #128
	subs r3, r3, r0
	lsls r3, r3, #9
	str r3, [r7, #12]
	bl Random16
	movs r3, #255
	ands r0, r3
	negs r0, r0
	subs r0, #128
	movs r4, #1
	lsls r0, r0, #10
	add r8, r4
	str r0, [r7, #16]
	movs r3, #0
	mov r0, r8
	str r3, [r7, #24]
	adds r7, #28
	cmp r0, #128
	bne .L_0814e822
	ldr r1, [sp, #12]
	ldr r2, [sp, #8]
	ldr r4, [sp, #20]
	movs r3, #224
	lsls r3, r3, #4
	adds r2, r2, r3
	adds r1, #2
	adds r4, #1
	str r1, [sp, #12]
	str r2, [sp, #8]
	str r4, [sp, #20]
	mov r0, r11
	ldr r3, [r0, #20]
	cmp r4, r3
	bne .L_0814e7e0
.L_0814e8b4:
	ldr r1, [sp, #32]
	movs r3, #239
	movs r4, #238
	lsls r3, r3, #7
	lsls r4, r4, #7
	adds r2, r1, r3
	adds r4, #132
	movs r3, #2
	str r3, [r2]
	adds r2, r1, r4
	movs r3, #50
	movs r1, #200
	str r3, [r2]
	lsls r1, r1, #4
	ldr r0, .L_0814ea30
	bl Scheduler_AddOrUpdateCallback
	mov r1, r11
	ldr r2, [r1, #20]
	movs r4, #56
	lsls r3, r2, #2
	adds r3, r3, r2
	movs r0, #0
	lsls r3, r3, #2
	negs r4, r4
	mov r9, r0
	cmp r3, r4
	bne .L_0814e8ee
	b .L_0814e9fc
.L_0814e8ee:
	mov r0, r9
	cmp r0, #32
	bne .L_0814e8fe
	movs r0, #0
	bl Func_081180e8
	mov r1, r11
	ldr r2, [r1, #20]
.L_0814e8fe:
	movs r3, #0
	str r3, [sp, #20]
	cmp r2, #0
	beq .L_0814e9cc
	movs r4, #36
	mov r10, r4
	movs r6, #0
	movs r7, #0
.L_0814e90e:
	cmp r9, r6
	bne .L_0814e92e
	movs r0, #143
	bl Audio_PlayCue
	mov r1, r10
	mov r3, r11
	ldrsh r0, [r1, r3]
	movs r3, #20
	movs r2, #1
	str r3, [sp, #0]
	movs r1, #7
	negs r2, r2
	ldr r3, [sp, #20]
	bl Func_0814cd48
.L_0814e92e:
	cmp r9, r6
	ble .L_0814e9b2
	ldr r0, .L_0814ea2c
	movs r4, #0
	mov r8, r4
	adds r5, r7, r0
.L_0814e93a:
	ldr r3, [r5, #24]
	cmp r3, #0
	blt .L_0814e9a6
	movs r1, #3
	mov r0, r8
	bl __modsi3
	ldr r2, .L_0814ea34
	adds r0, #1
	lsls r4, r0, #1
	subs r3, r4, #2
	ldrh r1, [r2, r3]
	ldr r2, [sp, #16]
	adds r1, r2, r1
	movs r3, #2
	ldrsh r2, [r5, r3]
	lsrs r3, r0, #31
	adds r3, r0, r3
	asrs r3, r3, #1
	subs r2, r2, r3
	mov lr, r2
	movs r3, #6
	ldrsh r2, [r5, r3]
	str r0, [sp, #0]
	subs r3, r2, r0
	str r4, [sp, #4]
	mov r2, lr
	ldr r0, [sp, #28]
	ldr r4, [sp, #24]
	mov lr, r4
	.2byte 0xf800
	ldr r2, .L_0814ea38
	movs r0, #3
	mov r3, r8
	ands r3, r0
	lsls r3, r3, #2
	ldr r2, [r2, r3]
	adds r0, r5, #0
	movs r1, #62
	bl BattleFxKernels_IntegrateVector2
	ldr r3, [r5, #24]
	adds r3, #1
	str r3, [r5, #24]
	ldr r3, [r5, #16]
	cmp r3, #0
	ble .L_0814e9a6
	movs r1, #6
	ldrsh r3, [r5, r1]
	cmp r3, #112
	ble .L_0814e9a6
	movs r3, #1
	negs r3, r3
	str r3, [r5, #24]
.L_0814e9a6:
	movs r2, #1
	add r8, r2
	mov r3, r8
	adds r5, #28
	cmp r3, #128
	bne .L_0814e93a
.L_0814e9b2:
	ldr r1, [sp, #20]
	mov r2, r11
	adds r1, #1
	str r1, [sp, #20]
	movs r0, #224
	ldr r3, [r2, #20]
	movs r4, #2
	lsls r0, r0, #4
	add r10, r4
	adds r6, #20
	adds r7, r7, r0
	cmp r1, r3
	bne .L_0814e90e
.L_0814e9cc:
	bl Func_081434f8
	movs r4, #240
	ldr r3, [sp, #32]
	lsls r4, r4, #7
	adds r4, #232
	adds r2, r3, r4
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	mov r1, r11
	ldr r3, [r1, #20]
	movs r0, #1
	adds r2, r3, #0
	lsls r3, r2, #2
	adds r3, r3, r2
	lsls r3, r3, #2
	add r9, r0
	adds r3, #56
	cmp r9, r3
	beq .L_0814e9fc
	b .L_0814e8ee
.L_0814e9fc:
	ldr r0, .L_0814ea30
	bl Scheduler_RemoveCallback
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	bl Func_08143bb8
	add sp, #60
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0814ea1c:
	.4byte 0x00000134
.L_0814ea20:
	.4byte 0x0000017d
.L_0814ea24:
	.4byte IwramCopyWords
.L_0814ea28:
	.4byte Data_02010018
.L_0814ea2c:
	.4byte gMapCellBuffer
.L_0814ea30:
	.4byte Func_08143000
.L_0814ea34:
	.4byte Data_08197410
.L_0814ea38:
	.4byte Data_081982e4
