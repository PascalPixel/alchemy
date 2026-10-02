.syntax unified
	.thumb
	.global BattleTarget_RunSelection
	.thumb_func
BattleTarget_RunSelection:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #324
	str r2, [sp, #84]
	str r3, [sp, #80]
	ldr r5, .L_080261a8
	mov r10, r0
	ldr r0, [r5]
	ldr r2, .L_080261ac
	str r0, [sp, #76]
	movs r0, #128
	mov r8, r1
	lsls r0, r0, #1
	movs r1, #0
	str r1, [sp, #64]
	str r2, [sp, #56]
	bl Resource_LoadIntoFreeSlot
	ldr r4, [sp, #84]
	movs r3, #0
	str r0, [sp, #52]
	mov r9, r3
	cmp r4, #0
	bne .L_080260be
	movs r6, #1
	str r6, [sp, #84]
.L_080260be:
	mov r7, r8
	cmp r7, #2
	beq .L_080260c8
	cmp r7, #4
	bne .L_080260d4
.L_080260c8:
	adds r3, r5, #0
	adds r3, #192
	ldr r2, [r3]
	movs r3, #2
	negs r3, r3
	b .L_080260dc
.L_080260d4:
	adds r3, r5, #0
	adds r3, #192
	ldr r2, [r3]
	movs r3, #16
.L_080260dc:
	str r3, [r2, #40]
	mov r0, sp
	adds r0, #212
	mov r3, sp
	str r0, [sp, #36]
	movs r2, #0
	movs r7, #5
	adds r3, #234
.L_080260ec:
	subs r7, #1
	strb r2, [r3]
	subs r3, #4
	cmp r7, #0
	bge .L_080260ec
	movs r1, #1
	negs r1, r1
	mov r2, r8
	str r1, [sp, #68]
	cmp r2, #2
	bne .L_0802613e
	ldr r4, [sp, #76]
	movs r3, #88
	ldrsh r3, [r4, r3]
	movs r7, #0
	cmp r3, #255
	beq .L_08026194
	movs r6, #154
	lsls r6, r6, #1
	ldr r0, [sp, #64]
	add r6, sp
	adds r2, r4, #0
	lsls r3, r0, #1
	str r6, [sp, #28]
	adds r2, #88
	adds r1, r3, r6
.L_08026120:
	ldrh r3, [r2]
	strh r3, [r1]
	ldr r3, [sp, #64]
	adds r7, #1
	adds r3, #1
	adds r1, #2
	str r3, [sp, #64]
	adds r2, #2
	cmp r7, #5
	bgt .L_080261b8
	movs r4, #0
	ldrsh r3, [r2, r4]
	cmp r3, #255
	bne .L_08026120
	b .L_080261b8
.L_0802613e:
	mov r5, r8
	cmp r5, #4
	bne .L_08026158
	movs r6, #154
	lsls r6, r6, #1
	add r6, sp
	mov r7, r10
	adds r0, r6, #0
	movs r1, #1
	str r6, [sp, #28]
	strh r7, [r0]
	str r1, [sp, #64]
	b .L_080261b8
.L_08026158:
	ldr r1, [sp, #76]
	movs r3, #100
	adds r1, #2
	ldrsh r3, [r1, r3]
	movs r7, #0
	cmp r3, #255
	beq .L_080261b0
	movs r3, #154
	lsls r3, r3, #1
	add r3, sp
	str r3, [sp, #28]
	ldr r4, [sp, #64]
	ldr r5, [sp, #28]
	lsls r3, r4, #1
	movs r0, #100
	adds r2, r3, r5
.L_08026178:
	ldrh r3, [r1, r0]
	strh r3, [r2]
	ldr r6, [sp, #64]
	adds r7, #1
	adds r6, #1
	adds r2, #2
	str r6, [sp, #64]
	adds r0, #2
	cmp r7, #5
	bgt .L_080261b8
	ldrsh r3, [r1, r0]
	cmp r3, #255
	bne .L_08026178
	b .L_080261b8
.L_08026194:
	movs r5, #154
	lsls r5, r5, #1
	add r5, sp
	str r5, [sp, #28]
	b .L_080261b8
.L_0802619e:
	ldr r6, [sp, #28]
	mov r7, r11
	ldrh r6, [r6, r7]
	mov r10, r6
	b .L_080262b6
.L_080261a8:
	.4byte gBattleWork
.L_080261ac:
	.4byte 0x0000ffff
.L_080261b0:
	movs r0, #154
	lsls r0, r0, #1
	add r0, sp
	str r0, [sp, #28]
.L_080261b8:
	ldr r1, [sp, #64]
	ldr r3, .L_080261e8
	ldr r4, [sp, #28]
	lsls r2, r1, #1
	mov r5, r8
	strh r3, [r4, r2]
	str r1, [sp, #60]
	cmp r5, #2
	beq .L_080261cc
	b .L_080262e0
.L_080261cc:
	ldr r6, [sp, #84]
	cmp r6, #255
	beq .L_080262b6
	ldr r7, [sp, #80]
	cmp r7, #0
	beq .L_080262b6
	movs r5, #0
	movs r7, #0
	cmp r5, r1
	bge .L_080262b6
	ldr r4, .L_080261ec
	movs r6, #0
	b .L_080261f0
	.2byte 0x0000
.L_080261e8:
	.4byte 0x000000ff
.L_080261ec:
	.4byte 0x0000ffff
.L_080261f0:
	ldr r0, [sp, #28]
	ldrh r3, [r6, r0]
	mov r11, r6
	cmp r3, #254
	beq .L_080262ac
	adds r0, r3, #0
	str r4, [sp, #8]
	bl Owner_GetStateFar
	ldr r2, [sp, #80]
	adds r1, r0, #0
	ldr r4, [sp, #8]
	cmp r2, #4
	beq .L_08026240
	cmp r2, #4
	bhi .L_08026216
	cmp r2, #3
	beq .L_08026234
	b .L_080262a6
.L_08026216:
	ldr r3, [sp, #80]
	cmp r3, #5
	beq .L_08026222
	cmp r3, #6
	beq .L_08026264
	b .L_080262a6
.L_08026222:
	movs r0, #56
	ldrsh r3, [r1, r0]
	cmp r3, #0
	bne .L_080262a6
	ldr r1, [sp, #28]
	ldrh r1, [r6, r1]
	movs r7, #1
	mov r10, r1
	b .L_080262a6
.L_08026234:
	ldr r2, .L_0802637c
	adds r3, r1, r2
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	b .L_080262a0
.L_08026240:
	movs r0, #156
	lsls r0, r0, #1
	adds r3, r1, r0
	ldr r3, [r3]
	ldr r2, .L_08026380
	ands r3, r2
	cmp r3, #0
	bne .L_080262a4
	movs r2, #158
	lsls r2, r2, #1
	adds r3, r1, r2
	ldrh r3, [r3]
	ands r3, r4
	cmp r3, #0
	bne .L_080262a4
	adds r0, #9
	adds r3, r1, r0
	b .L_0802629e
.L_08026264:
	movs r2, #156
	lsls r2, r2, #1
	adds r3, r1, r2
	ldr r3, [r3]
	ldr r2, .L_08026380
	ands r3, r2
	cmp r3, #0
	bne .L_080262a4
	movs r0, #158
	lsls r0, r0, #1
	adds r3, r1, r0
	ldrh r3, [r3]
	ands r3, r4
	cmp r3, #0
	bne .L_080262a4
	ldr r2, .L_08026384
	adds r3, r1, r2
	ldrb r3, [r3]
	cmp r3, #0
	bne .L_080262a4
	subs r0, #11
	adds r3, r1, r0
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	bne .L_080262a4
	subs r2, #1
	adds r3, r1, r2
.L_0802629e:
	ldrb r3, [r3]
.L_080262a0:
	cmp r3, #0
	beq .L_080262a6
.L_080262a4:
	movs r7, #1
.L_080262a6:
	cmp r7, #0
	beq .L_080262ac
	b .L_0802619e
.L_080262ac:
	ldr r3, [sp, #64]
	adds r5, #1
	adds r6, #2
	cmp r5, r3
	blt .L_080261f0
.L_080262b6:
	ldr r4, [sp, #64]
	movs r5, #0
	cmp r5, r4
	bge .L_080262d8
	ldr r6, [sp, #28]
	ldrh r3, [r6]
	cmp r3, r10
	beq .L_080262d8
	adds r2, r6, #0
.L_080262c8:
	ldr r7, [sp, #64]
	adds r5, #1
	cmp r5, r7
	bge .L_080262d8
	adds r2, #2
	ldrh r3, [r2]
	cmp r3, r10
	bne .L_080262c8
.L_080262d8:
	ldr r0, [sp, #64]
	cmp r5, r0
	beq .L_080262e0
	str r5, [sp, #68]
.L_080262e0:
	ldr r1, [sp, #68]
	cmp r1, #0
	bge .L_08026308
	ldr r3, [sp, #64]
	subs r3, #1
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	str r3, [sp, #68]
	b .L_08026308
.L_080262f4:
	ldr r2, [sp, #68]
	ldr r4, [sp, #64]
	adds r3, r2, r4
	subs r3, #1
	adds r0, r3, #0
	adds r1, r4, #0
	str r3, [sp, #68]
	bl __modsi3
	str r0, [sp, #68]
.L_08026308:
	ldr r5, [sp, #68]
	lsls r5, r5, #1
	str r5, [sp, #24]
	ldr r6, [sp, #28]
	ldrh r3, [r6, r5]
	cmp r3, #254
	beq .L_080262f4
	movs r0, #182
	lsls r0, r0, #1
	bl GameFlag_IsSet
	cmp r0, #0
	beq .L_08026336
	mov r7, r8
	cmp r7, #1
	bne .L_08026336
	ldrh r0, [r6, r5]
	bl Owner_GetStateFar
	movs r1, #56
	ldrsh r3, [r0, r1]
	cmp r3, #0
	beq .L_080262f4
.L_08026336:
	mov r2, r8
	cmp r2, #2
	beq .L_08026354
	add r5, sp, #200
	mov r0, r10
	adds r1, r5, #0
	bl BattleMotion_ProjectConditionalPositionFar
	ldr r4, [sp, #36]
	movs r3, #8
	strb r3, [r4, #2]
	ldr r3, [r5]
	strb r3, [r4]
	movs r3, #128
	strb r3, [r4, #1]
.L_08026354:
	movs r3, #74
	str r3, [sp, #0]
	movs r0, #0
	movs r1, #12
	movs r2, #30
	movs r3, #4
	bl UiWindow_Create
	movs r6, #152
	mov r5, sp
	mov r7, sp
	adds r5, #236
	add r6, sp
	adds r7, #88
	str r0, [sp, #72]
	str r5, [sp, #32]
	mov r11, r6
	str r7, [sp, #20]
	b .L_0802638e
	.2byte 0x0000
.L_0802637c:
	.4byte 0x00000131
.L_08026380:
	.4byte 0xff0000ff
.L_08026384:
	.4byte 0x00000141
.L_08026388:
	ldr r0, [sp, #68]
	lsls r0, r0, #1
	str r0, [sp, #24]
.L_0802638e:
	movs r1, #0
	str r1, [sp, #48]
	ldr r3, [sp, #24]
	ldr r2, [sp, #28]
	mov r1, r11
	ldrh r0, [r2, r3]
	bl BattleMotion_ProjectConditionalPositionFar
	ldr r3, .L_080263e8
	ldr r4, [sp, #32]
	str r3, [r4, #4]
	ldr r5, [sp, #48]
	str r5, [r4, #8]
	ldr r5, .L_080263ec
	ldr r1, [r5]
	movs r3, #31
	lsrs r1, r1, #2
	ands r1, r3
	ldr r3, .L_080263f0
	lsls r1, r1, #8
	adds r1, r1, r3
	ldr r0, [sp, #52]
	bl Resource_GetBuffer
	ldr r3, .L_080263e4
	ldr r6, [sp, #32]
	ands r0, r3
	ldrh r2, [r6, #8]
	ldr r3, .L_080263f4
	ands r3, r2
	orrs r3, r0
	ldr r0, [r5]
	adds r7, r6, #0
	strh r3, [r7, #8]
	lsls r0, r0, #12
	bl Trig_Sin
	cmp r0, #0
	bge .L_080263fc
	ldr r1, .L_080263f8
	adds r0, r0, r1
	b .L_080263fc
	.2byte 0x0000
.L_080263e4:
	.4byte 0x000003ff
.L_080263e8:
	.4byte 0x40002000
.L_080263ec:
	.4byte gFrameCount
.L_080263f0:
	.4byte Menu_AnimatedCursorTiles
.L_080263f4:
	.4byte 0xfffffc00
.L_080263f8:
	.4byte 0x00007fff
.L_080263fc:
	mov r4, r11
	ldr r3, [r4, #4]
	asrs r2, r0, #15
	adds r0, r3, r2
	str r0, [r4, #4]
	ldr r5, [sp, #36]
	movs r1, #1
	ldrb r2, [r5, #2]
	adds r3, r1, #0
	ands r3, r2
	cmp r3, #0
	beq .L_0802644e
	ldr r4, [r4]
	ldrb r3, [r5]
	adds r3, r4, r3
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r1, r3, #1
	ldrb r3, [r5, #1]
	adds r3, r0, r3
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r0, r3, #1
	subs r3, r4, r1
	cmp r3, #0
	blt .L_08026436
	cmp r3, #7
	ble .L_0802643c
	b .L_08026440
.L_08026436:
	subs r3, r1, r4
	cmp r3, #7
	bgt .L_08026440
.L_0802643c:
	movs r6, #1
	str r6, [sp, #48]
.L_08026440:
	mov r7, r11
	str r1, [r7]
	ldr r2, [sp, #36]
	str r0, [r7, #4]
	strb r1, [r2]
	strb r0, [r2, #1]
	b .L_08026486
.L_0802644e:
	movs r4, #192
	lsls r3, r2, #24
	lsls r4, r4, #18
	cmp r3, r4
	bhi .L_08026468
	mov r5, r11
	ldr r6, [sp, #36]
	ldr r3, [r5]
	str r0, [r5, #4]
	strb r3, [r6]
	strb r0, [r6, #1]
	strb r1, [r6, #2]
	b .L_08026486
.L_08026468:
	ldr r7, [sp, #36]
	ldrb r3, [r7]
	mov r0, r11
	str r3, [r0]
	ldrb r3, [r7, #1]
	str r3, [r0, #4]
	adds r3, r2, #0
	adds r3, #252
	movs r2, #192
	strb r3, [r7, #2]
	lsls r2, r2, #18
	lsls r3, r3, #24
	cmp r3, r2
	bhi .L_08026486
	strb r1, [r7, #2]
.L_08026486:
	mov r3, r11
	ldr r2, [r3]
	ldr r4, [sp, #32]
	ldr r3, .L_080264c0
	subs r2, #8
	ldrh r1, [r4, #6]
	ands r2, r3
	ldr r3, .L_080264c4
	ands r3, r1
	orrs r3, r2
	adds r5, r4, #0
	mov r6, r11
	strh r3, [r5, #6]
	ldr r3, [r6, #4]
	subs r3, #16
	strb r3, [r5, #4]
	ldr r0, [sp, #32]
	movs r1, #240
	bl Runtime_PushSlotEntry
	ldr r7, [sp, #84]
	cmp r7, #255
	bne .L_080264d8
	ldr r2, .L_080264c8
	ldr r3, [sp, #88]
	ands r3, r2
	movs r2, #128
	lsls r2, r2, #1
	b .L_080264cc
.L_080264c0:
	.4byte 0x000001ff
.L_080264c4:
	.4byte 0xfffffe00
.L_080264c8:
	.4byte 0xffff0000
.L_080264cc:
	orrs r3, r2
	ldr r2, .L_080265ec
	ands r3, r2
	movs r2, #128
	lsls r2, r2, #17
	b .L_080264ea
.L_080264d8:
	ldr r2, .L_080265f0
	ldr r3, [sp, #88]
	ands r3, r2
	movs r2, #176
	orrs r3, r2
	ldr r2, .L_080265ec
	ands r3, r2
	movs r2, #176
	lsls r2, r2, #16
.L_080264ea:
	orrs r3, r2
	str r3, [sp, #88]
	ldr r0, [sp, #20]
	ldr r3, .L_080265f0
	ldr r2, [r0, #4]
	ands r2, r3
	str r2, [r0, #4]
	ldr r0, [sp, #20]
	bl AffineMatrix_BuildForEffect
	ldr r1, [sp, #56]
	movs r3, #1
	ands r3, r1
	str r0, [sp, #44]
	cmp r3, #0
	bne .L_0802650c
	b .L_08026b96
.L_0802650c:
	movs r2, #0
	str r2, [sp, #64]
	ldr r1, [sp, #36]
	movs r0, #253
	movs r7, #5
.L_08026516:
	ldrb r2, [r1, #2]
	adds r3, r0, #0
	ands r3, r2
	subs r7, #1
	strb r3, [r1, #2]
	adds r1, #4
	cmp r7, #0
	bge .L_08026516
	ldr r3, [sp, #84]
	movs r7, #0
	cmp r7, r3
	bcs .L_080265f4
	ldr r6, [sp, #64]
	add r4, sp, #172
	add r0, sp, #324
	ldr r1, [sp, #64]
	adds r3, r6, r0
	mov r10, r4
	ldr r5, [sp, #28]
	ldr r4, [sp, #36]
	adds r6, r3, #0
	mov r2, r10
	lsls r3, r1, #1
	mov lr, r5
	adds r0, r4, #0
	adds r5, r3, r2
	movs r3, #254
	subs r6, #160
	mov r8, r3
	adds r0, #24
.L_08026552:
	ldr r1, [sp, #68]
	ldr r2, [sp, #60]
	adds r3, r1, r7
	cmp r3, r2
	bge .L_08026592
	lsls r3, r3, #1
	mov r1, lr
	ldrh r2, [r1, r3]
	adds r3, r2, #0
	cmp r3, #254
	beq .L_08026592
	ldrb r3, [r4, #2]
	strh r2, [r5]
	movs r2, #2
	orrs r2, r3
	movs r3, #0
	orrs r2, r3
	movs r3, #3
	ldrsb r3, [r4, r3]
	strb r2, [r4, #2]
	cmp r3, r7
	beq .L_08026586
	mov r1, r8
	ands r2, r1
	strb r2, [r4, #2]
	strb r7, [r4, #3]
.L_08026586:
	strb r7, [r6]
	ldr r2, [sp, #64]
	adds r2, #1
	str r2, [sp, #64]
	adds r6, #1
	adds r5, #2
.L_08026592:
	cmp r7, #0
	beq .L_080265de
	ldr r1, [sp, #68]
	subs r3, r1, r7
	cmp r3, #0
	blt .L_080265de
	lsls r3, r3, #1
	mov r1, lr
	ldrh r2, [r1, r3]
	adds r3, r2, #0
	cmp r3, #254
	beq .L_080265de
	strh r2, [r5]
	movs r2, #6
	ldrb r3, [r0, #2]
	subs r2, r2, r7
	mov r12, r2
	movs r2, #2
	orrs r2, r3
	movs r3, #0
	orrs r2, r3
	movs r3, #3
	ldrsb r3, [r0, r3]
	negs r1, r7
	strb r2, [r0, #2]
	cmp r3, r1
	beq .L_080265d0
	mov r3, r8
	ands r2, r3
	strb r2, [r0, #2]
	strb r1, [r0, #3]
.L_080265d0:
	mov r1, r12
	strb r1, [r6]
	ldr r2, [sp, #64]
	adds r2, #1
	str r2, [sp, #64]
	adds r6, #1
	adds r5, #2
.L_080265de:
	ldr r3, [sp, #84]
	adds r7, #1
	adds r4, #4
	subs r0, #4
	cmp r7, r3
	bcc .L_08026552
	b .L_080265f8
.L_080265ec:
	.4byte 0x0000ffff
.L_080265f0:
	.4byte 0xffff0000
.L_080265f4:
	add r4, sp, #172
	mov r10, r4
.L_080265f8:
	ldr r1, [sp, #36]
	movs r4, #2
	movs r0, #6
	movs r7, #5
.L_08026600:
	ldrb r2, [r1, #2]
	adds r3, r4, #0
	ands r3, r2
	cmp r3, #0
	bne .L_0802660c
	strb r0, [r1, #3]
.L_0802660c:
	subs r7, #1
	adds r1, #4
	cmp r7, #0
	bge .L_08026600
	ldr r5, [sp, #64]
	ldr r2, .L_08026644
	lsls r3, r5, #1
	mov r6, r10
	strh r2, [r6, r3]
	mov r0, r10
	movs r1, #1
	bl BattlePres_SetActorModesFar
	ldr r7, [sp, #28]
	ldr r0, [sp, #24]
	ldrh r3, [r7, r0]
	cmp r3, #7
	bls .L_08026632
	b .L_08026a84
.L_08026632:
	ldr r1, [sp, #84]
	cmp r1, #255
	bne .L_0802663a
	b .L_08026b8c
.L_0802663a:
	ldr r2, [sp, #80]
	cmp r2, #0
	bne .L_08026642
	b .L_08026b8c
.L_08026642:
	b .L_08026648
.L_08026644:
	.4byte 0x000000ff
.L_08026648:
	adds r0, r3, #0
	bl Owner_GetStateFar
	ldr r3, [sp, #24]
	adds r6, r0, #0
	mov r1, r11
	ldrh r0, [r7, r3]
	bl BattleMotion_ProjectConditionalPositionFar
	mov r4, r9
	cmp r4, #0
	beq .L_08026668
	mov r0, r9
	movs r1, #1
	bl UiWork_Finalize
.L_08026668:
	ldr r3, [sp, #80]
	subs r3, #1
	cmp r3, #6
	bls .L_08026672
	b .L_08026b8c
.L_08026672:
	ldr r2, .L_080268dc
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
	.2byte 0x0000
.L_0802667c:
	.4byte .L_080266c8
	.4byte .L_0802671e
	.4byte .L_080267b4
	.4byte .L_080267f8
	.4byte .L_08026780
	.4byte .L_0802691c
	.4byte .L_08026698
.L_08026698:
	mov r5, r11
	ldr r3, [r5]
	cmp r3, #0
	bge .L_080266a2
	adds r3, #7
.L_080266a2:
	asrs r3, r3, #3
	subs r0, r3, #4
	adds r3, #4
	cmp r3, #29
	ble .L_080266ae
	movs r0, #22
.L_080266ae:
	movs r3, #6
	str r3, [sp, #0]
	movs r1, #8
	movs r2, #9
	movs r3, #3
	bl UiWindow_Create
	mov r9, r0
	movs r0, #2
	bl UiWork_SetParamNibble
	ldr r0, .L_080268e0
	b .L_08026a72
.L_080266c8:
	mov r7, r11
	ldr r3, [r7]
	cmp r3, #0
	bge .L_080266d2
	adds r3, #7
.L_080266d2:
	asrs r3, r3, #3
	subs r0, r3, #7
	adds r3, #6
	cmp r3, #29
	ble .L_080266de
	movs r0, #17
.L_080266de:
	movs r3, #6
	str r3, [sp, #0]
	movs r1, #8
	movs r2, #13
	movs r3, #3
	bl UiWindow_Create
	mov r9, r0
	mov r1, r9
	ldr r0, .L_080268e4
	movs r2, #0
	movs r3, #0
	bl UiText_DrawStringAtOffset
	movs r5, #0
	movs r1, #56
	ldrsh r0, [r6, r1]
	mov r2, r9
	movs r1, #4
	movs r3, #16
	str r5, [sp, #0]
	bl UiText_DrawNumberInWindow
	movs r2, #48
	ldr r0, .L_080268e8
	mov r1, r9
	movs r3, #0
	bl UiText_DrawStringAtOffset
	movs r2, #52
	ldrsh r0, [r6, r2]
	b .L_08026772
.L_0802671e:
	mov r4, r11
	ldr r3, [r4]
	cmp r3, #0
	bge .L_08026728
	adds r3, #7
.L_08026728:
	asrs r3, r3, #3
	subs r0, r3, #7
	adds r3, #6
	cmp r3, #29
	ble .L_08026734
	movs r0, #17
.L_08026734:
	movs r3, #6
	str r3, [sp, #0]
	movs r1, #8
	movs r2, #13
	movs r3, #3
	bl UiWindow_Create
	mov r9, r0
	mov r1, r9
	ldr r0, .L_080268ec
	movs r2, #0
	movs r3, #0
	bl UiText_DrawStringAtOffset
	movs r5, #58
	ldrsh r0, [r6, r5]
	movs r1, #4
	movs r5, #0
	mov r2, r9
	movs r3, #16
	str r5, [sp, #0]
	bl UiText_DrawNumberInWindow
	ldr r0, .L_080268e8
	mov r1, r9
	movs r2, #48
	movs r3, #0
	bl UiText_DrawStringAtOffset
	movs r7, #54
	ldrsh r0, [r6, r7]
.L_08026772:
	movs r1, #4
	mov r2, r9
	movs r3, #56
	str r5, [sp, #0]
	bl UiText_DrawNumberInWindow
	b .L_08026b8c
.L_08026780:
	mov r0, r11
	ldr r3, [r0]
	cmp r3, #0
	bge .L_0802678a
	adds r3, #7
.L_0802678a:
	asrs r3, r3, #3
	subs r0, r3, #7
	adds r3, #5
	cmp r3, #29
	ble .L_08026796
	movs r0, #18
.L_08026796:
	movs r3, #6
	str r3, [sp, #0]
	movs r1, #8
	movs r3, #3
	movs r2, #12
	bl UiWindow_Create
	movs r1, #56
	ldrsh r3, [r6, r1]
	mov r9, r0
	cmp r3, #0
	beq .L_080267b0
	b .L_08026a6a
.L_080267b0:
	ldr r0, .L_080268f0
	b .L_080267ec
.L_080267b4:
	mov r2, r11
	ldr r3, [r2]
	cmp r3, #0
	bge .L_080267be
	adds r3, #7
.L_080267be:
	asrs r3, r3, #3
	subs r0, r3, #7
	adds r3, #5
	cmp r3, #29
	ble .L_080267ca
	movs r0, #18
.L_080267ca:
	movs r3, #6
	str r3, [sp, #0]
	movs r1, #8
	movs r3, #3
	movs r2, #12
	bl UiWindow_Create
	ldr r4, .L_080268f4
	adds r3, r6, r4
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	mov r9, r0
	cmp r3, #0
	bne .L_080267ea
	b .L_08026a6a
.L_080267ea:
	ldr r0, .L_080268f8
.L_080267ec:
	mov r1, r9
	movs r2, #0
	movs r3, #0
	bl UiText_DrawCharacterAtOffset
	b .L_08026b8c
.L_080267f8:
	movs r0, #156
	lsls r0, r0, #1
	adds r7, r6, r0
	ldrb r3, [r7]
	movs r5, #0
	cmp r3, #0
	beq .L_08026808
	movs r5, #1
.L_08026808:
	ldr r1, .L_080268fc
	adds r1, r1, r6
	ldrb r3, [r1]
	mov r8, r1
	cmp r3, #0
	beq .L_08026816
	adds r5, #1
.L_08026816:
	movs r2, #158
	lsls r2, r2, #1
	adds r2, r2, r6
	ldrb r3, [r2]
	mov r10, r2
	cmp r3, #0
	beq .L_08026826
	adds r5, #1
.L_08026826:
	ldr r3, .L_08026900
	adds r3, r6, r3
	str r3, [sp, #40]
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_08026834
	adds r5, #1
.L_08026834:
	ldr r4, .L_08026904
	adds r6, r6, r4
	ldrb r3, [r6]
	cmp r3, #0
	beq .L_08026840
	adds r5, #1
.L_08026840:
	cmp r5, #0
	bne .L_08026846
	movs r5, #1
.L_08026846:
	movs r3, #9
	subs r1, r3, r5
	cmp r1, #3
	bgt .L_08026850
	movs r1, #4
.L_08026850:
	mov r0, r11
	ldr r3, [r0]
	cmp r3, #0
	bge .L_0802685a
	adds r3, #7
.L_0802685a:
	asrs r3, r3, #3
	subs r0, r3, #7
	adds r3, #9
	cmp r3, #29
	ble .L_08026866
	movs r0, #14
.L_08026866:
	movs r2, #6
	adds r3, r5, #2
	str r2, [sp, #0]
	movs r2, #16
	bl UiWindow_Create
	ldrb r3, [r7]
	mov r9, r0
	movs r5, #0
	cmp r3, #0
	beq .L_0802688a
	ldr r0, .L_08026908
	mov r1, r9
	movs r2, #0
	movs r3, #0
	bl UiText_DrawCharacterAtOffset
	movs r5, #1
.L_0802688a:
	mov r1, r8
	ldrb r3, [r1]
	cmp r3, #0
	beq .L_080268a0
	lsls r3, r5, #3
	ldr r0, .L_0802690c
	mov r1, r9
	movs r2, #0
	bl UiText_DrawCharacterAtOffset
	adds r5, #1
.L_080268a0:
	mov r2, r10
	ldrb r3, [r2]
	cmp r3, #0
	beq .L_080268b6
	lsls r3, r5, #3
	ldr r0, .L_08026910
	mov r1, r9
	movs r2, #0
	bl UiText_DrawCharacterAtOffset
	adds r5, #1
.L_080268b6:
	ldr r4, [sp, #40]
	ldrb r3, [r4]
	cmp r3, #0
	beq .L_080268cc
	lsls r3, r5, #3
	ldr r0, .L_08026914
	mov r1, r9
	movs r2, #0
	bl UiText_DrawCharacterAtOffset
	adds r5, #1
.L_080268cc:
	ldrb r3, [r6]
	cmp r3, #0
	bne .L_080268d4
	b .L_08026a64
.L_080268d4:
	lsls r3, r5, #3
	ldr r0, .L_08026918
	b .L_08026a5a
	.2byte 0x0000
.L_080268dc:
	.4byte .L_0802667c
.L_080268e0:
	.4byte 0x000008ac
.L_080268e4:
	.4byte OwnerStatus_HpString
.L_080268e8:
	.4byte OwnerStatus_SlashString
.L_080268ec:
	.4byte OwnerStatus_PpString
.L_080268f0:
	.4byte 0x000008ab
.L_080268f4:
	.4byte 0x00000131
.L_080268f8:
	.4byte 0x000008a4
.L_080268fc:
	.4byte 0x0000013b
.L_08026900:
	.4byte 0x0000013d
.L_08026904:
	.4byte 0x00000141
.L_08026908:
	.4byte 0x000008a5
.L_0802690c:
	.4byte 0x000008a6
.L_08026910:
	.4byte 0x000008a7
.L_08026914:
	.4byte 0x000008a8
.L_08026918:
	.4byte 0x000008a9
.L_0802691c:
	ldr r7, .L_08026b08
	adds r3, r6, r7
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	movs r5, #0
	cmp r3, #0
	beq .L_0802692e
	movs r5, #1
.L_0802692e:
	movs r0, #156
	lsls r0, r0, #1
	adds r3, r6, r0
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_0802693c
	adds r5, #1
.L_0802693c:
	ldr r1, .L_08026b0c
	adds r3, r6, r1
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_08026948
	adds r5, #1
.L_08026948:
	movs r2, #158
	lsls r2, r2, #1
	adds r3, r6, r2
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_08026956
	adds r5, #1
.L_08026956:
	ldr r4, .L_08026b10
	adds r3, r6, r4
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_08026962
	adds r5, #1
.L_08026962:
	ldr r7, .L_08026b14
	adds r3, r6, r7
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_0802696e
	adds r5, #1
.L_0802696e:
	movs r0, #160
	lsls r0, r0, #1
	adds r3, r6, r0
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_0802697c
	adds r5, #1
.L_0802697c:
	cmp r5, #0
	bne .L_08026982
	movs r5, #1
.L_08026982:
	movs r3, #9
	subs r1, r3, r5
	cmp r1, #3
	bgt .L_0802698c
	movs r1, #4
.L_0802698c:
	mov r2, r11
	ldr r3, [r2]
	cmp r3, #0
	bge .L_08026996
	adds r3, #7
.L_08026996:
	asrs r3, r3, #3
	subs r0, r3, #7
	adds r3, #9
	cmp r3, #29
	ble .L_080269a2
	movs r0, #14
.L_080269a2:
	movs r2, #6
	adds r3, r5, #2
	str r2, [sp, #0]
	movs r2, #16
	bl UiWindow_Create
	ldr r4, .L_08026b08
	adds r3, r6, r4
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	mov r9, r0
	movs r5, #0
	cmp r3, #0
	beq .L_080269ce
	ldr r0, .L_08026b18
	mov r1, r9
	movs r2, #0
	movs r3, #0
	bl UiText_DrawCharacterAtOffset
	movs r5, #1
.L_080269ce:
	movs r7, #156
	lsls r7, r7, #1
	adds r3, r6, r7
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_080269e8
	lsls r3, r5, #3
	ldr r0, .L_08026b1c
	mov r1, r9
	movs r2, #0
	bl UiText_DrawCharacterAtOffset
	adds r5, #1
.L_080269e8:
	ldr r0, .L_08026b0c
	adds r3, r6, r0
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_08026a00
	lsls r3, r5, #3
	ldr r0, .L_08026b20
	mov r1, r9
	movs r2, #0
	bl UiText_DrawCharacterAtOffset
	adds r5, #1
.L_08026a00:
	movs r1, #158
	lsls r1, r1, #1
	adds r3, r6, r1
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_08026a1a
	lsls r3, r5, #3
	ldr r0, .L_08026b24
	mov r1, r9
	movs r2, #0
	bl UiText_DrawCharacterAtOffset
	adds r5, #1
.L_08026a1a:
	ldr r2, .L_08026b10
	adds r3, r6, r2
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_08026a32
	lsls r3, r5, #3
	ldr r0, .L_08026b28
	mov r1, r9
	movs r2, #0
	bl UiText_DrawCharacterAtOffset
	adds r5, #1
.L_08026a32:
	ldr r4, .L_08026b14
	adds r3, r6, r4
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_08026a4a
	lsls r3, r5, #3
	ldr r0, .L_08026b2c
	mov r1, r9
	movs r2, #0
	bl UiText_DrawCharacterAtOffset
	adds r5, #1
.L_08026a4a:
	movs r7, #160
	lsls r7, r7, #1
	adds r3, r6, r7
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_08026a64
	lsls r3, r5, #3
	ldr r0, .L_08026b30
.L_08026a5a:
	mov r1, r9
	movs r2, #0
	bl UiText_DrawCharacterAtOffset
	adds r5, #1
.L_08026a64:
	cmp r5, #0
	beq .L_08026a6a
	b .L_08026b8c
.L_08026a6a:
	movs r0, #2
	bl UiWork_SetParamNibble
	ldr r0, .L_08026b34
.L_08026a72:
	mov r1, r9
	movs r2, #0
	movs r3, #0
	bl UiText_DrawCharacterAtOffset
	movs r0, #15
	bl UiWork_SetParamNibble
	b .L_08026b8c
.L_08026a84:
	ldr r0, [sp, #84]
	cmp r0, #255
	bne .L_08026a8c
	b .L_08026b8c
.L_08026a8c:
	ldr r1, [sp, #28]
	ldr r2, [sp, #24]
	ldrh r0, [r1, r2]
	bl Owner_GetStateFar
	ldr r3, [sp, #28]
	ldr r4, [sp, #24]
	add r5, sp, #108
	mov r8, r0
	adds r1, r5, #0
	ldrh r0, [r3, r4]
	bl BattleMotion_ProjectConditionalPositionFar
	ldr r3, .L_08026b38
	ldr r0, [r3]
	lsls r0, r0, #12
	bl Trig_Sin
	cmp r0, #0
	bge .L_08026ab8
	ldr r6, .L_08026b3c
	adds r0, r0, r6
.L_08026ab8:
	ldr r2, [r5, #4]
	asrs r3, r0, #15
	adds r2, r2, r3
	movs r3, #148
	str r2, [r5, #4]
	lsls r3, r3, #1
	add r3, r8
	ldrb r3, [r3]
	cmp r3, #125
	beq .L_08026ad6
	cmp r3, #122
	beq .L_08026ad6
	movs r7, #0
	add r6, sp, #120
	b .L_08026af2
.L_08026ad6:
	movs r3, #148
	lsls r3, r3, #1
	add r3, r8
	ldrb r3, [r3]
	ldr r0, .L_08026b40
	cmp r3, #125
	bne .L_08026ae6
	adds r0, #1
.L_08026ae6:
	add r6, sp, #120
	adds r1, r6, #0
	movs r2, #14
	bl UiText_CopyMessageString
	b .L_08026b4a
.L_08026af2:
	cmp r7, #13
	bgt .L_08026b44
	mov r0, r8
	ldrb r3, [r0, r7]
	lsls r2, r7, #1
	strh r3, [r6, r2]
	adds r7, #1
	cmp r3, #0
	bne .L_08026af2
	b .L_08026b46
	.2byte 0x0000
.L_08026b08:
	.4byte 0x00000131
.L_08026b0c:
	.4byte 0x0000013b
.L_08026b10:
	.4byte 0x0000013d
.L_08026b14:
	.4byte 0x00000141
.L_08026b18:
	.4byte 0x000008a4
.L_08026b1c:
	.4byte 0x000008a5
.L_08026b20:
	.4byte 0x000008a6
.L_08026b24:
	.4byte 0x000008a7
.L_08026b28:
	.4byte 0x000008a8
.L_08026b2c:
	.4byte 0x000008a9
.L_08026b30:
	.4byte 0x000008aa
.L_08026b34:
	.4byte 0x000008a3
.L_08026b38:
	.4byte gFrameCount
.L_08026b3c:
	.4byte 0x00007fff
.L_08026b40:
	.4byte 0x0000080e
.L_08026b44:
	lsls r2, r7, #1
.L_08026b46:
	ldr r3, .L_08026b78
	strh r3, [r6, r2]
.L_08026b4a:
	adds r0, r6, #0
	bl UiText_GetWideStringWidth
	lsrs r2, r0, #31
	ldr r3, [r5]
	adds r2, r0, r2
	asrs r2, r2, #1
	subs r3, r3, r2
	subs r3, #8
	str r3, [r5]
	adds r3, r3, r0
	cmp r3, #224
	ble .L_08026b6a
	movs r3, #224
	subs r3, r3, r0
	str r3, [r5]
.L_08026b6a:
	ldr r3, [r5]
	cmp r3, #0
	bge .L_08026b7c
	movs r3, #0
	str r3, [r5]
	b .L_08026b7c
	.2byte 0x0000
.L_08026b78:
	.4byte 0x00000000
.L_08026b7c:
	bl Ui_ClearVramBlock
	ldr r2, [r5]
	adds r0, r6, #0
	ldr r1, [sp, #72]
	movs r3, #4
	bl UiText_RenderWideStringAtOffset
.L_08026b8c:
	ldr r1, [sp, #56]
	movs r3, #2
	negs r3, r3
	ands r1, r3
	str r1, [sp, #56]
.L_08026b96:
	ldr r2, [sp, #48]
	cmp r2, #0
	bne .L_08026b9e
	b .L_08026cdc
.L_08026b9e:
	ldr r3, [sp, #64]
	movs r7, #1
	cmp r7, r3
	blt .L_08026ba8
	b .L_08026cdc
.L_08026ba8:
	mov r5, sp
	adds r5, #164
	movs r6, #96
	movs r4, #172
	str r5, [sp, #16]
	add r6, sp
	ldr r5, [sp, #32]
	movs r0, #2
	add r4, sp
	mov r8, r6
	str r0, [sp, #12]
	mov r10, r4
	adds r5, #12
	mov r4, r8
.L_08026bc4:
	ldr r1, [sp, #16]
	ldrb r3, [r1, r7]
	ldr r2, [sp, #36]
	lsls r3, r3, #2
	adds r3, r2, r3
	str r3, [sp, #4]
	ldr r3, [sp, #12]
	mov r6, r10
	adds r1, r4, #0
	ldrh r0, [r3, r6]
	str r4, [sp, #8]
	bl BattleMotion_ProjectConditionalPositionFar
	ldr r3, .L_08026c94
	ldr r0, [r3]
	lsls r0, r0, #12
	bl Trig_Sin
	ldr r4, [sp, #8]
	cmp r0, #0
	bge .L_08026bf2
	ldr r1, .L_08026c98
	adds r0, r0, r1
.L_08026bf2:
	ldr r3, [r4, #4]
	asrs r2, r0, #15
	adds r3, r3, r2
	str r3, [r4, #4]
	ldr r2, [sp, #32]
	mov lr, r5
	mov r12, r2
	mov r3, lr
	mov r6, r12
	ldmia r6!, {r0, r1, r2}
	stmia r3!, {r0, r1, r2}
	ldr r3, [sp, #4]
	movs r1, #1
	ldrb r2, [r3, #2]
	adds r3, r1, #0
	ands r3, r2
	cmp r3, #0
	beq .L_08026c3a
	ldr r6, [sp, #4]
	ldr r1, [r4]
	ldrb r3, [r6]
	adds r1, r1, r3
	lsrs r3, r1, #31
	ldrb r2, [r6, #1]
	adds r1, r1, r3
	ldr r3, [r4, #4]
	adds r3, r3, r2
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r1, r1, #1
	asrs r3, r3, #1
	str r1, [r4]
	strb r1, [r6]
	str r3, [r4, #4]
	strb r3, [r6, #1]
	b .L_08026c50
.L_08026c3a:
	ldrh r3, [r5, #6]
	ldrb r2, [r5, #4]
	ldr r0, [sp, #4]
	lsls r3, r3, #23
	lsrs r3, r3, #23
	adds r2, #8
	strb r1, [r0, #2]
	str r3, [r4]
	strb r3, [r0]
	str r2, [r4, #4]
	strb r2, [r0, #1]
.L_08026c50:
	ldrb r2, [r5, #5]
	movs r1, #13
	negs r1, r1
	adds r3, r1, #0
	adds r0, r2, #0
	mov r2, r8
	ands r0, r3
	ldr r1, [r2]
	movs r3, #4
	orrs r0, r3
	ldr r3, .L_08026c8c
	subs r1, #8
	ands r1, r3
	ldr r2, .L_08026c90
	ldrh r3, [r5, #6]
	ands r3, r2
	orrs r3, r1
	mov r6, r8
	strh r3, [r5, #6]
	ldr r3, [r6, #4]
	subs r3, #12
	strb r0, [r5, #5]
	strb r3, [r5, #4]
	ldr r1, [sp, #84]
	cmp r1, #255
	bne .L_08026c9c
	movs r2, #4
	negs r2, r2
	ands r0, r2
	b .L_08026ca6
.L_08026c8c:
	.4byte 0x000001ff
.L_08026c90:
	.4byte 0xfffffe00
.L_08026c94:
	.4byte gFrameCount
.L_08026c98:
	.4byte 0x00007fff
.L_08026c9c:
	movs r3, #4
	negs r3, r3
	ands r0, r3
	movs r3, #1
	orrs r0, r3
.L_08026ca6:
	strb r0, [r5, #5]
	ldr r2, [sp, #44]
	movs r3, #31
	movs r6, #63
	ands r2, r3
	negs r6, r6
	ldrb r3, [r5, #7]
	adds r1, r6, #0
	lsls r2, r2, #1
	ands r3, r1
	orrs r3, r2
	adds r0, r5, #0
	strb r3, [r5, #7]
	movs r1, #240
	str r4, [sp, #8]
	bl Runtime_PushSlotEntry
	ldr r0, [sp, #12]
	ldr r1, [sp, #64]
	adds r0, #2
	adds r7, #1
	adds r5, #12
	str r0, [sp, #12]
	ldr r4, [sp, #8]
	cmp r7, r1
	bge .L_08026cdc
	b .L_08026bc4
.L_08026cdc:
	ldr r3, .L_08026e74
	ldr r6, [r3]
	ldr r3, .L_08026e78
	ldr r5, [r3]
	ldr r3, .L_08026e7c
	ldr r2, [r3]
	adds r3, r2, #0
	adds r3, #216
	ldr r3, [r3]
	cmp r3, #0
	beq .L_08026d0c
	adds r2, #220
	ldr r3, [r2]
	movs r5, #0
	movs r6, #0
	cmp r3, #0
	bne .L_08026d08
	movs r3, #60
	str r3, [r2]
	movs r5, #1
	movs r6, #1
	b .L_08026d0c
.L_08026d08:
	subs r3, #1
	str r3, [r2]
.L_08026d0c:
	movs r3, #1
	ands r3, r6
	cmp r3, #0
	beq .L_08026d8e
	ldr r2, [sp, #28]
	ldr r3, [sp, #24]
	movs r5, #0
	ldrh r4, [r2, r3]
	ldr r7, [sp, #76]
	str r5, [sp, #56]
	movs r3, #88
	ldrsh r3, [r7, r3]
	movs r0, #1
	negs r0, r0
	movs r1, #0
	cmp r3, #255
	beq .L_08026d54
	cmp r3, r4
	bne .L_08026d38
	movs r0, #128
	lsls r0, r0, #1
	b .L_08026d54
.L_08026d38:
	adds r1, #1
	cmp r1, #5
	bgt .L_08026d54
	lsls r3, r1, #1
	ldr r5, [sp, #76]
	adds r3, #88
	ldrsh r3, [r5, r3]
	cmp r3, #255
	beq .L_08026d54
	cmp r3, r4
	bne .L_08026d38
	movs r0, #128
	lsls r0, r0, #1
	orrs r0, r1
.L_08026d54:
	cmp r0, #0
	bge .L_08026d8a
	ldr r2, [sp, #76]
	adds r2, #102
	movs r7, #0
	ldrsh r3, [r2, r7]
	movs r5, #192
	movs r1, #0
	lsls r5, r5, #1
	cmp r3, #255
	beq .L_08026d8a
	cmp r3, r4
	bne .L_08026d72
	adds r0, r5, #0
	b .L_08026d8a
.L_08026d72:
	adds r1, #1
	adds r2, #2
	cmp r1, #5
	bgt .L_08026d8a
	movs r7, #0
	ldrsh r3, [r2, r7]
	cmp r3, #255
	beq .L_08026d8a
	cmp r3, r4
	bne .L_08026d72
	adds r0, r5, #0
	orrs r0, r1
.L_08026d8a:
	str r0, [sp, #68]
	b .L_08026df6
.L_08026d8e:
	ldr r0, [sp, #84]
	cmp r0, #255
	beq .L_08026df6
	movs r3, #144
	ands r3, r5
	cmp r3, #0
	beq .L_08026dc4
	movs r0, #111
	bl AudioCommand_PlayFar
.L_08026da2:
	ldr r1, [sp, #68]
	adds r1, #1
	str r1, [sp, #68]
	adds r0, r1, #0
	ldr r1, [sp, #60]
	bl __modsi3
	str r0, [sp, #68]
	ldr r4, [sp, #28]
	lsls r2, r0, #1
	ldrh r3, [r4, r2]
	cmp r3, #254
	beq .L_08026da2
	ldr r7, [sp, #56]
	movs r3, #1
	orrs r7, r3
	str r7, [sp, #56]
.L_08026dc4:
	movs r3, #96
	ands r3, r5
	cmp r3, #0
	beq .L_08026df6
	movs r0, #111
	bl AudioCommand_PlayFar
.L_08026dd2:
	ldr r0, [sp, #68]
	ldr r1, [sp, #60]
	adds r3, r0, r1
	subs r3, #1
	adds r0, r3, #0
	str r3, [sp, #68]
	bl __modsi3
	str r0, [sp, #68]
	ldr r2, [sp, #28]
	lsls r3, r0, #1
	ldrh r3, [r2, r3]
	cmp r3, #254
	beq .L_08026dd2
	ldr r4, [sp, #56]
	movs r3, #1
	orrs r4, r3
	str r4, [sp, #56]
.L_08026df6:
	ldr r3, .L_08026e7c
	ldr r3, [r3]
	ldr r3, [r3, #76]
	cmp r3, #0
	beq .L_08026e08
	movs r3, #2
	ands r3, r6
	cmp r3, #0
	beq .L_08026e16
.L_08026e08:
	movs r0, #113
	bl AudioCommand_PlayFar
	movs r5, #1
	negs r5, r5
	str r5, [sp, #68]
	b .L_08026e26
.L_08026e16:
	movs r0, #1
	bl WaitFrames
	ldr r6, [sp, #56]
	cmp r6, #0
	beq .L_08026e26
	bl .L_08026388
.L_08026e26:
	movs r0, #1
	bl WaitFrames
	mov r7, r9
	ldr r0, [sp, #52]
	bl Resource_ResetEntry
	cmp r7, #0
	beq .L_08026e40
	mov r0, r9
	movs r1, #1
	bl UiWork_Finalize
.L_08026e40:
	ldr r0, [sp, #72]
	movs r1, #1
	bl UiWork_Finalize
	movs r1, #0
	ldr r0, [sp, #28]
	bl BattlePres_SetActorModesFar
	ldr r3, .L_08026e7c
	ldr r2, [r3]
	movs r3, #0
	str r3, [r2, #40]
	movs r0, #1
	bl WaitFrames
	ldr r0, [sp, #68]
	add sp, #324
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r1}
	bx r1
	.2byte 0x0000
.L_08026e74:
	.4byte gKeyState
.L_08026e78:
	.4byte gKeysRepeat
.L_08026e7c:
	.4byte gLinkCountdownWork
