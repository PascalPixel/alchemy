.syntax unified
	.thumb
	.global Func_080de21c
	.thumb_func
Func_080de21c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #224
	ldr r3, [r3]
	sub sp, #40
	ldr r0, [r3, #20]
	mov r9, r3
	str r0, [sp, #8]
	bl BattleEffect_InitializeSharedScene
	movs r0, #130
	bl Audio_PlayCue
	add r1, sp, #12
	mov r5, r9
	mov r10, r1
	movs r2, #11
	adds r5, #80
	mov r6, r10
	mov r8, r2
.L_080de252:
	mov r3, r9
	ldr r2, [r3, #16]
	movs r4, #128
	ldr r3, [r2, #8]
	lsls r4, r4, #13
	str r3, [r6]
	adds r0, r6, #0
	ldr r3, [r2, #12]
	adds r3, r3, r4
	str r3, [r6, #4]
	ldr r3, [r2, #16]
	str r3, [r6, #8]
	bl Camera_WorldToScreen
	movs r1, #168
	ldr r2, [r6]
	ldr r3, [r6, #8]
	adds r0, r5, #0
	lsls r1, r1, #2
	bl Func_080ebec8
	adds r0, r5, #0
	ldr r1, .L_080de2f4
	bl Func_080ebeb4
	adds r0, r5, #0
	movs r1, #7
	bl Func_080ebea8
	ldr r0, [r5]
	movs r1, #9
	bl Animation_ApplyChildValuesToRecordFar
	movs r3, #179
	lsls r3, r3, #8
	adds r3, #51
	str r3, [r5, #44]
	str r3, [r5, #40]
	movs r0, #2
	bl WaitFrames
	movs r0, #1
	negs r0, r0
	add r8, r0
	mov r1, r8
	adds r5, #72
	cmp r1, #0
	bge .L_080de252
	mov r3, r9
	ldr r2, [r3, #16]
	mov r4, r10
	ldr r3, [r2, #8]
	movs r0, #128
	str r3, [r4]
	lsls r0, r0, #13
	ldr r3, [r2, #12]
	adds r3, r3, r0
	str r3, [r4, #4]
	movs r0, #128
	ldr r3, [r2, #16]
	mov r2, r9
	str r3, [r4, #8]
	lsls r0, r0, #12
	ldrh r1, [r2]
	mov r2, r10
	bl Vector_AddPolarOffset
	mov r3, r10
	movs r0, #139
	ldr r1, [r3]
	ldr r2, [r3, #4]
	lsls r0, r0, #1
	ldr r3, [r3, #8]
	bl Object_Spawn
	adds r6, r0, #0
	cmp r6, #0
	bne .L_080de2f8
	bl BattleFx_PrepareBufferInterpolation
	b .L_080de53a
.L_080de2f4:
	.4byte Func_080de060
.L_080de2f8:
	movs r3, #128
	lsls r3, r3, #7
	str r3, [r6, #28]
	str r3, [r6, #24]
	mov r4, r9
	ldrh r3, [r4]
	ldr r2, .L_080de334
	strh r3, [r6, #6]
	movs r3, #128
	lsls r3, r3, #11
	str r3, [r6, #48]
	str r3, [r6, #52]
	adds r3, r6, #0
	adds r3, #85
	strb r2, [r3]
	adds r0, r6, #0
	movs r1, #5
	bl Object_SetMode
	adds r0, r6, #0
	movs r1, #3
	bl Animation_ApplyChildValuesFar
	ldr r3, [r6, #24]
	movs r0, #128
	lsls r0, r0, #9
	cmp r3, r0
	bge .L_080de354
	b .L_080de338
	.2byte 0x0000
.L_080de334:
	.4byte 0x00000000
.L_080de338:
	movs r1, #160
	lsls r1, r1, #3
	adds r3, r3, r1
	str r3, [r6, #28]
	str r3, [r6, #24]
	movs r0, #1
	bl WaitFrames
	movs r2, #255
	ldr r3, [r6, #24]
	lsls r2, r2, #8
	adds r2, #255
	cmp r3, r2
	ble .L_080de338
.L_080de354:
	movs r0, #3
	bl WaitFrames
	mov r0, sp
	adds r0, #24
	str r0, [sp, #4]
	movs r3, #0
	movs r4, #2
	mov r11, r3
	mov r8, r4
	add r7, sp, #32
.L_080de36a:
	movs r0, #139
	ldr r1, [r6, #8]
	ldr r2, [r6, #12]
	ldr r3, [r6, #16]
	lsls r0, r0, #1
	bl Object_Spawn
	adds r5, r0, #0
	str r0, [r7]
	subs r7, #4
	cmp r5, #0
	beq .L_080de3b8
	movs r3, #240
	lsls r3, r3, #8
	str r3, [r5, #28]
	str r3, [r5, #24]
	mov r1, r9
	ldrh r3, [r1]
	movs r2, #0
	strh r3, [r5, #6]
	movs r3, #128
	lsls r3, r3, #11
	str r3, [r5, #48]
	str r3, [r5, #52]
	adds r3, r5, #0
	adds r3, #85
	strb r2, [r3]
	movs r1, #5
	bl Object_SetMode
	adds r0, r5, #0
	movs r1, #2
	bl Animation_ApplyChildValuesFar
	mov r1, r11
	ldr r0, [r5, #80]
	bl Func_080dc0d8
	mov r11, r0
.L_080de3b8:
	movs r3, #1
	negs r3, r3
	add r8, r3
	mov r4, r8
	cmp r4, #0
	bge .L_080de36a
	mov r3, r9
	mov r0, r11
	adds r3, #32
	ldrb r0, [r0, #16]
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	mov r11, r0
	cmp r3, #0
	beq .L_080de3fe
	mov r1, r9
	ldr r2, [r1, #16]
	mov r4, r10
	ldr r3, [r2, #8]
	movs r0, #128
	str r3, [r4]
	lsls r0, r0, #13
	ldr r3, [r2, #12]
	adds r3, r3, r0
	str r3, [r4, #4]
	movs r0, #224
	ldr r3, [r2, #16]
	lsls r0, r0, #14
	str r3, [r4, #8]
	mov r2, r10
	ldrh r1, [r1]
	bl Vector_AddPolarOffset
	b .L_080de414
.L_080de3fe:
	mov r1, r9
	ldr r3, [r1, #4]
	mov r2, r10
	str r3, [r2]
	movs r4, #128
	ldr r3, [r1, #8]
	lsls r4, r4, #13
	adds r3, r3, r4
	str r3, [r2, #4]
	ldr r3, [r1, #12]
	str r3, [r2, #8]
.L_080de414:
	mov r0, r10
	ldr r2, [r0, #4]
	ldr r1, [r0]
	ldr r3, [r0, #8]
	adds r0, r6, #0
	bl Object_SetPosition
	ldr r1, .L_080de548
	adds r0, r6, #0
	bl Object_SetCallback
	ldr r1, [sp, #4]
	movs r2, #2
	str r1, [sp, #0]
	mov r7, r10
	mov r8, r2
.L_080de434:
	ldr r4, [sp, #0]
	ldmia r4!, {r5}
	adds r3, r4, #0
	str r3, [sp, #0]
	cmp r5, #0
	beq .L_080de45a
	movs r0, #3
	bl WaitFrames
	ldr r1, [r7]
	adds r0, r5, #0
	ldr r2, [r7, #4]
	ldr r3, [r7, #8]
	bl Object_SetPosition
	adds r0, r5, #0
	ldr r1, .L_080de54c
	bl Object_SetCallback
.L_080de45a:
	movs r0, #1
	negs r0, r0
	add r8, r0
	mov r1, r8
	cmp r1, #0
	bge .L_080de434
	ldr r3, [r6]
	movs r2, #0
	mov r8, r2
	cmp r3, #0
	beq .L_080de486
.L_080de470:
	movs r0, #1
	bl WaitFrames
	movs r3, #1
	add r8, r3
	mov r4, r8
	cmp r4, #59
	bgt .L_080de486
	ldr r3, [r6]
	cmp r3, #0
	bne .L_080de470
.L_080de486:
	ldr r0, [sp, #8]
	cmp r0, #0
	beq .L_080de4fa
	mov r3, r9
	adds r3, #53
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	bne .L_080de4fa
	mov r3, r9
	adds r3, #52
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	beq .L_080de4ae
	movs r3, #128
	lsls r3, r3, #12
	str r3, [r0, #40]
.L_080de4ae:
	ldr r1, [sp, #8]
	mov r2, r10
	ldr r3, [r1, #8]
	movs r0, #128
	str r3, [r2]
	lsls r0, r0, #13
	ldr r3, [r1, #12]
	str r3, [r2, #4]
	ldr r3, [r1, #16]
	str r3, [r2, #8]
	mov r3, r9
	ldrh r1, [r3]
	bl Vector_AddPolarOffset
	mov r1, r10
	ldr r0, [sp, #8]
	bl Func_08020210
	cmp r0, #0
	bne .L_080de4fa
	ldr r0, [sp, #8]
	mov r1, r10
	bl Func_08020298
	cmp r0, #0
	bne .L_080de4fa
	ldr r4, [sp, #8]
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r4, #52]
	str r3, [r4, #48]
	mov r0, r10
	ldr r1, [r0]
	ldr r2, [r0, #4]
	ldr r3, [r0, #8]
	ldr r0, [sp, #8]
	bl Object_SetPosition
.L_080de4fa:
	movs r0, #4
	bl Func_080ce31c
	adds r2, r0, #0
	movs r0, #160
	lsls r0, r0, #23
	adds r0, #5
	movs r1, #4
	bl Func_080ce458
	cmp r0, #0
	beq .L_080de520
	mov r3, r9
	movs r2, #24
	ldrsh r1, [r3, r2]
	movs r4, #26
	ldrsh r2, [r3, r4]
	bl Func_080ceafc
.L_080de520:
	movs r0, #10
	bl WaitFrames
	bl BattleFx_PrepareBufferInterpolation
	movs r0, #20
	bl WaitFrames
	mov r0, r11
	cmp r0, #96
	beq .L_080de53a
	bl Resource_ResetEntry
.L_080de53a:
	add sp, #40
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_080de548:
	.4byte Data_080f0ef8
.L_080de54c:
	.4byte Data_080f0e58
