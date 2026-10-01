.syntax unified
	.thumb
	.global Func_0816f080
	.thumb_func
Func_0816f080:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #68
	str r0, [sp, #44]
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #96]
	ldr r0, [r3, #92]
	str r2, [sp, #40]
	mov r10, r0
	ldr r3, [r3, #100]
	movs r0, #1
	str r3, [sp, #24]
	bl BattleFx_BeginCanvasLayer
	ldr r3, .L_0816f0cc
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	strh r3, [r2]
	ldr r3, .L_0816f0d0
	subs r2, #2
	strh r3, [r2]
	ldr r4, [sp, #44]
	ldr r3, [r4, #4]
	cmp r3, #0
	bne .L_0816f0d4
	movs r0, #104
	movs r1, #19
	bl Func_081963ec
	b .L_0816f0dc
	.2byte 0x0000
.L_0816f0cc:
	.4byte 0x00001010
.L_0816f0d0:
	.4byte 0x00000000
.L_0816f0d4:
	movs r0, #104
	movs r1, #23
	bl Func_081963ec
.L_0816f0dc:
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #104]
	movs r1, #239
	movs r2, #238
	lsls r1, r1, #7
	lsls r2, r2, #7
	str r3, [sp, #32]
	add r1, r10
	movs r3, #2
	adds r2, #132
	str r3, [r1]
	add r2, r10
	movs r3, #50
	str r3, [r2]
	movs r3, #1
	str r3, [r1]
	movs r3, #0
	movs r1, #200
	str r3, [r2]
	lsls r1, r1, #4
	ldr r0, .L_0816f494
	bl Scheduler_AddOrUpdateCallback
	ldr r6, [sp, #44]
	movs r5, #160
	ldr r0, [r6, #8]
	bl GetBattleObjectSlotFar
	ldr r0, [r0]
	ldr r3, .L_0816f498
	str r0, [sp, #20]
	movs r0, #160
	lsls r0, r0, #19
	movs r1, #128
	mov lr, r3
	.2byte 0xf800
	lsls r5, r5, #19
	adds r5, #2
	movs r6, #0
.L_0816f12c:
	adds r0, r6, #0
	movs r1, #3
	bl Math_Div
	adds r6, #1
	adds r0, #8
	strh r0, [r5]
	adds r5, #2
	cmp r6, #63
	bne .L_0816f12c
	movs r0, #16
	mov r11, r0
	movs r0, #184
	lsls r0, r0, #5
	movs r1, #128
	ldr r3, .L_0816f498
	lsls r1, r1, #4
	add r0, r10
	mov lr, r3
	.2byte 0xf800
	movs r0, #224
	lsls r0, r0, #3
	movs r2, #128
	lsls r2, r2, #9
	add r0, r10
	movs r1, #16
	bl Func_0815b434
	movs r2, #0
	mov r8, r2
	mov lr, r2
.L_0816f16a:
	movs r3, #0
	movs r4, #8
	mov r12, r3
	cmp r4, #0
	beq .L_0816f202
	mov r6, lr
	lsrs r3, r6, #31
	add r3, lr
	asrs r3, r3, #1
	mov r9, r3
	mov r3, r11
	movs r0, #176
	add r3, lr
	lsls r0, r0, #5
	lsls r3, r3, #1
	mov r6, r8
	adds r0, #254
	add r3, r10
	movs r1, #184
	adds r7, r3, r0
	lsls r3, r6, #5
	lsls r1, r1, #5
	add r3, r10
	mov r2, r11
	adds r5, r3, r1
	movs r3, #32
	lsls r4, r2, #1
	subs r3, r3, r6
	movs r2, #184
	lsls r3, r3, #5
	lsls r2, r2, #5
	subs r3, #32
	adds r2, #1
	adds r2, r3, r2
	adds r6, r2, #0
	movs r2, #176
	lsls r2, r2, #5
	adds r2, #255
	adds r2, r3, r2
	adds r1, r3, r1
	adds r3, r3, r0
	str r2, [sp, #8]
	add r3, r10
	add r2, r10
	adds r2, r4, r2
	add r6, r10
	add r1, r10
	adds r4, r4, r3
.L_0816f1ca:
	mov r3, r9
	movs r0, #224
	lsls r0, r0, #3
	add r3, r12
	adds r3, r3, r0
	mov r0, r10
	ldrb r3, [r0, r3]
	cmp r3, #0
	beq .L_0816f1ec
	strb r3, [r5]
	strb r3, [r5, #1]
	strb r3, [r7]
	strb r3, [r7, #1]
	strb r3, [r1]
	strb r3, [r6]
	strb r3, [r4]
	strb r3, [r2]
.L_0816f1ec:
	movs r3, #1
	add r12, r3
	mov r0, r12
	subs r2, #2
	subs r4, #2
	subs r7, #2
	adds r5, #2
	adds r6, #2
	adds r1, #2
	cmp r0, #8
	bne .L_0816f1ca
.L_0816f202:
	movs r2, #1
	add r8, r2
	add lr, r11
	cmp r8, r11
	bne .L_0816f16a
	ldr r1, [sp, #24]
	ldr r0, .L_0816f49c
	movs r2, #0
	movs r3, #0
	bl Resource_LoadAndDecompress
	movs r1, #142
	lsls r1, r1, #7
	movs r3, #0
	ldr r0, .L_0816f4a0
	add r1, r10
	movs r2, #0
	bl Resource_LoadAndDecompress
	mov r4, sp
	movs r3, #0
	adds r4, #48
	str r3, [sp, #28]
	str r4, [sp, #16]
.L_0816f232:
	ldr r6, [sp, #28]
	cmp r6, #0
	bne .L_0816f2b0
	ldr r3, [sp, #44]
	add r7, sp, #56
	adds r1, r7, #0
	movs r2, #36
	ldrsh r0, [r3, r2]
	bl Func_0815e21c
	movs r6, #0
	movs r1, #0
	mov r5, r10
.L_0816f24c:
	str r1, [sp, #12]
	bl Random16
	ldr r3, [r7]
	ldr r1, [sp, #12]
	lsrs r2, r3, #31
	adds r3, r3, r2
	movs r2, #15
	ands r2, r0
	asrs r3, r3, #1
	adds r3, r3, r2
	subs r3, #8
	lsls r3, r3, #16
	str r3, [r5]
	movs r3, #8
	negs r3, r3
	str r3, [r5, #4]
	str r1, [r5, #12]
	str r1, [r5, #16]
	bl Random16
	movs r3, #3
	ands r3, r0
	adds r3, #4
	ldr r1, [sp, #12]
	str r3, [r5, #8]
	lsls r2, r6, #4
	movs r3, #160
	subs r3, r3, r2
	adds r6, #1
	str r3, [r5, #20]
	str r1, [r5, #24]
	adds r5, #28
	cmp r6, #16
	bne .L_0816f24c
	ldr r3, [r7]
	movs r1, #192
	lsrs r2, r3, #31
	lsls r1, r1, #3
	adds r3, r3, r2
	adds r1, #228
	asrs r3, r3, #1
	add r1, r10
	lsls r3, r3, #16
	str r3, [r1]
	movs r3, #224
	lsls r3, r3, #15
	movs r4, #0
	str r3, [r1, #4]
	str r4, [r1, #24]
.L_0816f2b0:
	ldr r6, [sp, #28]
	cmp r6, #63
	bgt .L_0816f33e
	movs r6, #0
	mov r5, r10
.L_0816f2ba:
	ldr r0, [sp, #28]
	lsls r3, r6, #3
	cmp r0, r3
	ble .L_0816f336
	ldr r7, [r5, #24]
	cmp r7, #0
	bne .L_0816f336
	ldr r0, [r5, #8]
	ldr r2, .L_0816f4a4
	lsls r4, r0, #1
	subs r3, r4, #2
	ldrh r1, [r2, r3]
	ldr r2, [sp, #24]
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
	ldr r0, [sp, #40]
	ldr r4, [sp, #32]
	mov lr, r4
	.2byte 0xf800
	movs r2, #128
	adds r0, r5, #0
	movs r1, #64
	lsls r2, r2, #8
	bl BattleFxKernels_IntegrateVector2
	movs r0, #6
	ldrsh r3, [r5, r0]
	cmp r3, #111
	ble .L_0816f336
	movs r3, #1
	str r3, [r5, #24]
	bl Random16
	movs r3, #31
	ands r3, r0
	subs r3, #16
	lsls r3, r3, #12
	str r3, [r5, #12]
	ldr r2, .L_0816f4a8
	adds r3, r6, #0
	cmp r6, #0
	bge .L_0816f328
	adds r3, r6, #3
.L_0816f328:
	asrs r3, r3, #2
	lsls r3, r3, #2
	subs r3, r6, r3
	ldrsb r3, [r2, r3]
	str r7, [r5, #16]
	lsls r3, r3, #13
	str r3, [r5, #12]
.L_0816f336:
	adds r6, #1
	adds r5, #28
	cmp r6, #6
	bne .L_0816f2ba
.L_0816f33e:
	movs r0, #32
	bl Runtime_BumpAllocateAlternatePool
	mov r11, r0
	movs r0, #1
	bl Func_081969f8
	ldr r2, .L_0816f4ac
	ldr r3, [sp, #48]
	adds r7, r0, #0
	ands r3, r2
	movs r2, #5
	orrs r3, r2
	ldr r2, .L_0816f4b0
	movs r4, #0
	ands r3, r2
	movs r2, #160
	lsls r2, r2, #3
	orrs r3, r2
	str r3, [sp, #48]
	ldr r2, [sp, #16]
	movs r3, #184
	lsls r3, r3, #5
	add r3, r10
	str r3, [r2, #4]
	movs r3, #6
	str r3, [r7]
	ldr r3, .L_0816f4b4
	movs r0, #192
	str r3, [r7, #8]
	mov r3, r11
	lsls r0, r0, #3
	str r2, [r7, #16]
	str r3, [r7, #12]
	str r4, [r7, #20]
	adds r0, #228
	movs r6, #0
	add r0, r10
	mov r1, r10
.L_0816f38c:
	ldr r3, [r1, #24]
	cmp r3, #0
	ble .L_0816f3a4
	ldr r2, [r1, #20]
	ldr r3, [r0, #24]
	adds r3, r3, r2
	str r3, [r0, #24]
	ldr r3, [r1, #20]
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	str r3, [r1, #20]
.L_0816f3a4:
	adds r6, #1
	adds r1, #28
	cmp r6, #8
	bne .L_0816f38c
	movs r6, #192
	lsls r6, r6, #3
	adds r6, #228
	add r6, r10
	ldr r5, [r6, #24]
	cmp r5, #0
	ble .L_0816f408
	movs r1, #206
	lsls r0, r5, #13
	lsls r1, r1, #2
	bl Math_Div
	mov r8, r0
	ldr r0, [sp, #28]
	cmp r0, #63
	ble .L_0816f3d2
	adds r3, r5, #0
	subs r3, #24
	str r3, [r6, #24]
.L_0816f3d2:
	bl Func_08014de4
	ldr r3, .L_0816f4b8
	ldr r0, [r6]
	ldr r1, [r6, #4]
	adds r0, r0, r3
	adds r1, r1, r3
	movs r2, #0
	bl Func_08015160
	mov r3, r8
	lsls r2, r3, #1
	adds r0, r2, #0
	mov r1, r8
	bl Func_080151e4
	ldr r0, .L_0816f4bc
	bl SceneTransform_ApplyPitch
	ldr r0, .L_0816f4c0
	mov r1, r11
	movs r2, #4
	bl Func_08196958
	adds r0, r7, #0
	bl Func_08196a7c
.L_0816f408:
	ldr r4, [sp, #16]
	add r6, sp, #48
	movs r3, #5
	strb r3, [r4]
	str r6, [sp, #16]
	strb r3, [r6, #1]
	movs r3, #7
	str r3, [r7]
	ldr r3, .L_0816f4b4
	movs r0, #194
	lsls r0, r0, #7
	str r3, [r7, #8]
	adds r0, #168
	mov r8, r6
	mov r9, r0
	movs r6, #0
	mov r5, r10
.L_0816f42a:
	ldr r2, [sp, #28]
	lsls r3, r6, #2
	adds r3, #64
	cmp r2, r3
	ble .L_0816f4d0
	movs r3, #0
	str r3, [r7, #20]
	lsrs r3, r6, #31
	adds r3, r6, r3
	asrs r3, r3, #1
	lsls r3, r3, #1
	subs r3, r6, r3
	lsls r3, r3, #10
	movs r4, #142
	lsls r4, r4, #7
	add r3, r10
	adds r3, r3, r4
	mov r0, r8
	str r3, [r0, #4]
	bl Func_08014de4
	ldr r3, .L_0816f4c4
	ldr r0, [r5]
	ldr r2, .L_0816f4b8
	ldr r1, [r5, #4]
	adds r0, r0, r2
	adds r1, r1, r3
	movs r2, #0
	bl Func_08015160
	mov r4, r9
	asrs r2, r4, #1
	adds r0, r2, #0
	mov r1, r9
	bl Func_080151e4
	ldr r0, .L_0816f4c8
	bl SceneTransform_ApplyPitch
	mov r1, r11
	movs r2, #4
	ldr r0, .L_0816f4c0
	bl Func_08196958
	adds r0, r7, #0
	bl Func_08196a7c
	adds r0, r5, #0
	movs r1, #60
	ldr r2, .L_0816f4cc
	bl BattleFxKernels_IntegrateVector2
	b .L_0816f4d0
.L_0816f494:
	.4byte Func_08143000
.L_0816f498:
	.4byte IwramClearWords
.L_0816f49c:
	.4byte 0x00000134
.L_0816f4a0:
	.4byte 0x000000d8
.L_0816f4a4:
	.4byte Data_08197410
.L_0816f4a8:
	.4byte Data_08198bea
.L_0816f4ac:
	.4byte 0xffffff00
.L_0816f4b0:
	.4byte 0xffff00ff
.L_0816f4b4:
	.4byte Data_08199244
.L_0816f4b8:
	.4byte 0xffc00000
.L_0816f4bc:
	.4byte 0xffffe000
.L_0816f4c0:
	.4byte Data_08199210
.L_0816f4c4:
	.4byte 0xffd00000
.L_0816f4c8:
	.4byte 0xffffc000
.L_0816f4cc:
	.4byte 0xffff8000
.L_0816f4d0:
	adds r6, #1
	adds r5, #28
	cmp r6, #6
	bne .L_0816f42a
	adds r0, r7, #0
	bl Sys_Free
	mov r0, r11
	bl Sys_Free
	ldr r6, [sp, #28]
	cmp r6, #64
	bne .L_0816f4f0
	movs r0, #147
	bl Audio_PlayCue
.L_0816f4f0:
	movs r6, #0
	movs r7, #2
	movs r5, #80
.L_0816f4f6:
	ldr r0, [sp, #28]
	cmp r0, r5
	bne .L_0816f536
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #168
	add r3, r10
	str r7, [r3]
	ldr r3, [sp, #44]
	movs r1, #0
	movs r2, #36
	ldrsh r0, [r3, r2]
	bl Func_08118088
	ldr r2, [sp, #44]
	movs r1, #7
	movs r4, #36
	ldrsh r0, [r2, r4]
	movs r3, #0
	movs r2, #5
	str r7, [sp, #0]
	bl Func_0814cd48
	cmp r6, #0
	bne .L_0816f530
	movs r0, #134
	bl Func_081180e8
	b .L_0816f536
.L_0816f530:
	movs r0, #133
	bl Audio_PlayCue
.L_0816f536:
	adds r6, #1
	adds r5, #4
	cmp r6, #6
	bne .L_0816f4f6
	movs r0, #4
	movs r1, #4
	bl Func_08158ce0
	bl Func_081434f8
	movs r2, #240
	lsls r2, r2, #7
	adds r2, #232
	movs r3, #1
	add r2, r10
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r3, [sp, #28]
	adds r3, #1
	str r3, [sp, #28]
	cmp r3, #120
	beq .L_0816f568
	b .L_0816f232
.L_0816f568:
	ldr r0, [sp, #20]
	movs r1, #16
	bl ObjectDispatch_ApplyValueToChildrenFar
	movs r0, #104
	bl Runtime_ReleaseHeapBlock
	ldr r0, .L_0816f590
	bl Scheduler_RemoveCallback
	bl Func_08143bb8
	add sp, #68
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0816f590:
	.4byte Func_08143000
