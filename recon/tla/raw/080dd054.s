.syntax unified
	.thumb
	.global Func_080dd054
	.thumb_func
Func_080dd054:
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
	sub sp, #68
	str r3, [sp, #40]
	movs r5, #128
	ldr r0, [r3, #16]
	ldr r1, [r3, #20]
	ldrh r3, [r3]
	lsls r5, r5, #8
	mov r10, r1
	adds r3, r3, r5
	str r3, [sp, #28]
	mov r3, r10
	movs r2, #0
	adds r3, #34
	str r2, [sp, #24]
	str r3, [sp, #16]
	mov r11, r0
	ldrb r4, [r3]
	mov r0, r10
	str r4, [sp, #20]
	str r2, [sp, #8]
	cmp r0, #0
	bne .L_080dd098
	b .L_080dd414
.L_080dd098:
	bl BattleEffect_InitializeSharedScene
	mov r2, r11
	mov r1, r10
	str r1, [r2, #104]
	mov r0, r11
	ldr r1, .L_080dd424
	bl Object_SetCallback
	mov r0, r11
	bl BattleFx_StartItemBreak
	mov r9, r0
	cmp r0, #0
	bne .L_080dd0bc
	bl BattleFx_PrepareBufferInterpolation
	b .L_080dd414
.L_080dd0bc:
	ldr r4, [sp, #40]
	movs r3, #30
	ldrsh r0, [r4, r3]
	bl Func_080ce31c
	ldr r4, [sp, #40]
	adds r2, r0, #0
	movs r0, #160
	lsls r0, r0, #23
	movs r3, #30
	ldrsh r1, [r4, r3]
	adds r0, #5
	bl Func_080ce458
	str r0, [sp, #12]
	mov r0, r10
	ldr r3, [r0, #8]
	add r6, sp, #56
	str r3, [r6]
	mov r1, r9
	ldr r3, [r0, #12]
	str r0, [r1, #104]
	movs r0, #128
	lsls r0, r0, #13
	adds r3, r3, r0
	str r3, [r6, #4]
	mov r2, r10
	ldr r3, [r2, #16]
	adds r2, r6, #0
	str r3, [r6, #8]
	ldr r1, [sp, #28]
	bl Func_0801489c
	ldr r1, [r6]
	ldr r2, [r6, #4]
	ldr r3, [r6, #8]
	mov r0, r9
	bl Object_SetPosition
	mov r0, r9
	bl BattleFx_SnapScaleToFull
	movs r3, #128
	mov r4, r9
	lsls r3, r3, #11
	str r3, [r4, #48]
	str r5, [r4, #52]
	movs r3, #4
	adds r4, #85
	str r4, [sp, #4]
	strb r3, [r4]
	ldr r3, .L_080dd428
	mov r0, r10
	str r3, [r0, #108]
	movs r3, #204
	lsls r3, r3, #7
	adds r3, #102
	str r3, [r0, #48]
	movs r3, #204
	add r1, sp, #24
	lsls r3, r3, #6
	adds r3, #51
	ldrb r1, [r1]
	str r3, [r0, #52]
	mov r3, r10
	adds r3, #90
	strb r1, [r3]
	ldr r2, [sp, #16]
	movs r3, #2
	strb r3, [r2]
	ldr r3, [sp, #40]
	adds r3, #33
	str r3, [sp, #0]
	b .L_080dd34c
.L_080dd150:
	movs r4, #255
	lsls r4, r4, #8
	adds r4, #255
	cmp r7, r4
	bne .L_080dd196
	mov r0, r10
	ldr r3, [r0, #8]
	mov r1, r10
	str r3, [r6]
	adds r2, r6, #0
	ldr r3, [r0, #12]
	movs r0, #128
	lsls r0, r0, #13
	adds r3, r3, r0
	str r3, [r6, #4]
	ldr r3, [r1, #16]
	str r3, [r6, #8]
	ldr r1, [sp, #28]
	bl Func_0801489c
	ldr r2, [r6, #4]
	ldr r1, [r6]
	ldr r3, [r6, #8]
	mov r0, r9
	bl Object_SetPosition
	mov r0, r9
	movs r1, #1
	bl Object_SetMode
	mov r2, r9
	str r5, [r2, #36]
	str r5, [r2, #40]
	str r5, [r2, #44]
	b .L_080dd34c
.L_080dd196:
	mov r4, r10
	ldr r3, [r4, #8]
	movs r5, #128
	str r3, [r6]
	lsls r5, r5, #13
	ldr r3, [r4, #12]
	adds r0, r5, #0
	adds r3, r3, r5
	str r3, [r6, #4]
	adds r2, r6, #0
	ldr r3, [r4, #16]
	str r3, [r6, #8]
	ldr r1, [sp, #28]
	bl Func_0801489c
	movs r0, #128
	lsls r0, r0, #10
	adds r1, r7, #0
	adds r2, r6, #0
	bl Func_0801489c
	ldr r1, [r6]
	ldr r2, [r6, #4]
	ldr r3, [r6, #8]
	mov r0, r9
	bl Object_SetPosition
	mov r0, r9
	bl Object_CommitPosition
	mov r0, r10
	ldr r3, [r0, #8]
	adds r1, r7, #0
	str r3, [r6]
	adds r2, r6, #0
	ldr r3, [r0, #12]
	str r3, [r6, #4]
	ldr r3, [r0, #16]
	adds r0, r5, #0
	str r3, [r6, #8]
	bl Func_0801489c
	mov r2, r10
	ldr r3, [r2, #8]
	add r1, sp, #44
	str r3, [r1]
	mov r8, r1
	ldr r3, [r2, #12]
	movs r0, #128
	str r3, [r1, #4]
	lsls r0, r0, #14
	ldr r3, [r2, #16]
	mov r2, r8
	str r3, [r1, #8]
	adds r1, r7, #0
	bl Func_0801489c
	mov r0, r10
	adds r1, r6, #0
	bl Func_08020210
	cmp r0, #0
	bgt .L_080dd266
	mov r0, r10
	adds r1, r6, #0
	bl Func_080202e8
	cmp r0, #0
	beq .L_080dd286
	cmp r0, r11
	bne .L_080dd266
	mov r3, r11
	ldr r2, .L_080dd42c
	ldr r0, [r3, #8]
	ldr r4, [r3, #16]
	ldr r3, [r6]
	ands r0, r2
	ands r3, r2
	ands r4, r2
	cmp r0, r3
	bne .L_080dd240
	ldr r3, [r6, #8]
	ands r3, r2
	cmp r4, r3
	beq .L_080dd266
.L_080dd240:
	mov r2, r8
	ldr r1, [r2]
	ldr r5, .L_080dd42c
	adds r3, r1, #0
	ands r3, r5
	cmp r0, r3
	bne .L_080dd286
	ldr r2, [r2, #8]
	adds r3, r2, #0
	ands r3, r5
	cmp r4, r3
	bne .L_080dd286
	mov r3, r11
	adds r3, #34
	ldrb r0, [r3]
	bl Func_080202f0
	cmp r0, #0
	beq .L_080dd282
.L_080dd266:
	mov r0, r9
	movs r1, #4
	bl Object_SetMode
	ldr r3, .L_080dd430
	movs r2, #15
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	bne .L_080dd34c
	movs r0, #114
	bl Audio_PlayCue
	b .L_080dd34c
.L_080dd282:
	movs r3, #1
	str r3, [sp, #24]
.L_080dd286:
	movs r0, #175
	bl Audio_PlayCue
	movs r4, #1
	str r4, [sp, #8]
	ldr r4, [sp, #28]
	ldr r0, [r6]
	subs r3, r4, r7
	str r0, [sp, #36]
	ldr r2, .L_080dd434
	ldr r1, [r6, #8]
	lsls r3, r3, #16
	str r1, [sp, #32]
	lsrs r3, r3, #30
	ldrb r1, [r2, r3]
	mov r0, r9
	bl Object_SetMode
	movs r0, #15
	bl WaitFrames
	movs r0, #204
	mov r3, r10
	lsls r0, r0, #6
	movs r5, #0
	adds r0, #51
	mov r1, r10
	adds r3, #91
	strb r5, [r3]
	str r0, [r1, #48]
	str r0, [r1, #52]
	mov r0, r10
	ldr r1, [r6]
	ldr r2, [r6, #4]
	ldr r3, [r6, #8]
	bl Object_SetPosition
	ldr r2, [sp, #4]
	movs r3, #204
	strb r5, [r2]
	lsls r3, r3, #6
	movs r5, #128
	adds r3, #51
	mov r4, r9
	lsls r5, r5, #13
	str r3, [r4, #48]
	str r3, [r4, #52]
	adds r0, r5, #0
	adds r1, r7, #0
	adds r2, r6, #0
	bl Func_0801489c
	ldr r2, [r6, #4]
	mov r0, r9
	ldr r1, [r6]
	adds r2, r2, r5
	ldr r3, [r6, #8]
	bl Object_SetPosition
	ldr r0, [sp, #24]
	cmp r0, #1
	bne .L_080dd330
	ldr r2, [sp, #40]
	movs r1, #24
	ldrsh r0, [r2, r1]
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	strb r3, [r0]
	movs r3, #204
	lsls r3, r3, #6
	adds r3, #51
	mov r4, r11
	str r3, [r4, #48]
	str r3, [r4, #52]
	mov r0, r8
	ldr r1, [r0]
	ldr r2, [r0, #4]
	ldr r3, [r0, #8]
	mov r0, r11
	bl Object_SetPosition
.L_080dd330:
	mov r0, r10
	bl Object_CommitPosition
	ldr r1, [sp, #12]
	cmp r1, #0
	bne .L_080dd396
	ldr r2, [sp, #36]
	mov r3, r10
	str r2, [r3, #8]
	ldr r4, [sp, #32]
	str r1, [r3, #36]
	str r4, [r3, #16]
	str r1, [r3, #44]
	b .L_080dd396
.L_080dd34c:
	movs r0, #1
	bl WaitFrames
	ldr r0, [sp, #0]
	movs r3, #0
	ldrsb r3, [r0, r3]
	cmp r3, #0
	bne .L_080dd36e
	ldr r0, .L_080dd438
	movs r3, #129
	ldr r2, [r0, #4]
	lsls r3, r3, #2
	adds r3, #255
	ands r2, r3
	cmp r2, #0
	bne .L_080dd396
	b .L_080dd370
.L_080dd36e:
	ldr r0, .L_080dd438
.L_080dd370:
	ldr r0, [r0]
	bl Func_080dcf54
	ldr r1, [sp, #40]
	lsls r0, r0, #16
	ldr r5, [r1, #36]
	lsrs r7, r0, #16
	cmp r5, #0
	bne .L_080dd384
	b .L_080dd150
.L_080dd384:
	mov lr, r5
	.2byte 0xf800
	movs r2, #255
	lsls r2, r2, #8
	adds r7, r0, #0
	adds r2, #255
	cmp r7, r2
	beq .L_080dd396
	b .L_080dd196
.L_080dd396:
	add r3, sp, #20
	ldr r4, [sp, #16]
	ldrb r3, [r3]
	mov r0, r10
	strb r3, [r4]
	ldr r3, [sp, #40]
	adds r3, #64
	ldrb r1, [r3]
	bl Animation_ApplyChildValuesFar
	ldr r4, [sp, #40]
	mov r0, r10
	ldr r1, [r4, #60]
	bl Object_SetCallback
	ldr r0, [sp, #40]
	mov r1, r10
	ldr r3, [r0, #56]
	str r3, [r1, #108]
	ldr r2, [sp, #8]
	cmp r2, #0
	beq .L_080dd3ec
	mov r4, r9
	movs r3, #0
	str r3, [r4, #108]
	mov r0, r9
	movs r1, #4
	bl Object_SetMode
	mov r0, r9
	ldr r2, [r0, #12]
	ldr r1, [r0, #8]
	ldr r3, [r0, #16]
	bl Object_SetPositionAndResetMotionFar
	ldr r3, [sp, #40]
	ldr r0, [sp, #12]
	movs r2, #24
	ldrsh r1, [r3, r2]
	movs r4, #26
	ldrsh r2, [r3, r4]
	bl Func_080ceafc
.L_080dd3ec:
	bl Func_080dc7cc
	ldr r0, [sp, #24]
	cmp r0, #1
	bne .L_080dd40a
	ldr r2, [sp, #40]
	movs r1, #24
	ldrsh r0, [r2, r1]
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	movs r2, #1
	orrs r3, r2
	strb r3, [r0]
.L_080dd40a:
	bl BattleFx_PrepareBufferInterpolation
	mov r0, r9
	bl UpdateRisingParticleBurst
.L_080dd414:
	add sp, #68
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080dd424:
	.4byte Data_080f0e60
.L_080dd428:
	.4byte Func_080db91c
.L_080dd42c:
	.4byte 0xfff00000
.L_080dd430:
	.4byte Data_0300122c
.L_080dd434:
	.4byte Data_080f0ee4
.L_080dd438:
	.4byte gInput
