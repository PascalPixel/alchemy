.syntax unified
	.thumb
	.global Func_080e1040
	.thumb_func
Func_080e1040:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r2, .L_080e10ac
	adds r3, r2, #0
	adds r6, r0, #0
	ldmia r3!, {r0}
	ldr r3, [r3]
	sub sp, #72
	str r3, [sp, #48]
	subs r2, #108
	ldr r5, .L_080e10b0
	ldr r2, [r2]
	mov r9, r0
	add r5, r9
	str r2, [sp, #28]
	movs r0, #0
	str r6, [r5]
	bl BattleFx_BeginCanvasLayer
	ldr r3, [r5]
	ldr r2, [r3, #4]
	add r3, sp, #56
	str r3, [sp, #0]
	add r3, sp, #52
	str r3, [sp, #4]
	adds r0, r6, #0
	movs r3, #2
	movs r1, #0
	bl BattleFx_PrepareCanvasEffect
	ldr r3, .L_080e10a8
	ldr r2, .L_080e10b4
	strh r3, [r2]
	ldr r3, [r5]
	ldr r3, [r3, #4]
	cmp r3, #1
	bne .L_080e10b8
	movs r3, #2
	str r3, [sp, #0]
	movs r0, #46
	movs r1, #7
	movs r2, #7
	movs r3, #7
	bl Unnamed_080ed408
	b .L_080e10c8
	.2byte 0x0000
.L_080e10a8:
	.4byte 0x00001010
.L_080e10ac:
	.4byte gBattleFxWork
.L_080e10b0:
	.4byte 0x00007828
.L_080e10b4:
	.4byte 0x04000052
.L_080e10b8:
	movs r3, #2
	str r3, [sp, #0]
	movs r0, #46
	movs r1, #7
	movs r2, #7
	movs r3, #3
	bl Unnamed_080ed408
.L_080e10c8:
	ldr r3, .L_080e13a0
	adds r3, #184
	ldr r3, [r3]
	ldr r0, .L_080e13a4
	str r3, [sp, #32]
	mov r1, r9
	movs r2, #1
	movs r3, #0
	bl Resource_LoadAndDecompress
	ldr r1, .L_080e13a8
	ldr r0, .L_080e13ac
	add r1, r9
	movs r2, #1
	movs r3, #1
	bl Resource_LoadAndDecompress
	movs r2, #239
	lsls r2, r2, #7
	add r2, r9
	movs r3, #2
	str r3, [r2]
	ldr r2, .L_080e13b0
	ldr r5, .L_080e13b4
	add r2, r9
	movs r3, #75
	movs r1, #144
	str r3, [r2]
	lsls r1, r1, #3
	add r5, r9
	ldr r0, .L_080e13b8
	bl Scheduler_AddOrUpdateCallback
	ldr r3, [r5]
	ldr r0, [r3, #8]
	bl GetBattleObjectSlotFar
	ldr r3, [r5]
	ldr r6, [r0]
	movs r1, #36
	ldrsh r0, [r3, r1]
	bl GetBattleObjectSlotFar
	ldr r0, [r0]
	movs r2, #0
	movs r5, #225
	lsls r5, r5, #7
	str r0, [sp, #24]
	str r2, [sp, #44]
	movs r7, #0
	add r5, r9
.L_080e112e:
	ldr r3, [r6, #8]
	str r3, [r5]
	movs r3, #132
	lsls r3, r3, #15
	str r3, [r5, #4]
	ldr r3, [r6, #16]
	str r3, [r5, #8]
	asrs r3, r7, #5
	str r3, [r5, #12]
	bl Random16
	movs r3, #127
	ands r3, r0
	subs r3, #64
	lsls r3, r3, #16
	asrs r3, r3, #6
	str r3, [r5, #16]
	bl Random16
	movs r3, #255
	ands r3, r0
	subs r3, #127
	lsls r3, r3, #16
	asrs r3, r3, #5
	str r3, [r5, #20]
	ldr r3, [r5]
	cmp r3, #0
	ble .L_080e116c
	ldr r3, [r5, #12]
	negs r3, r3
	str r3, [r5, #12]
.L_080e116c:
	movs r3, #1
	str r3, [r5, #24]
	ldr r4, [sp, #44]
	movs r3, #160
	lsls r3, r3, #15
	adds r4, #1
	adds r7, r7, r3
	adds r5, #28
	str r4, [sp, #44]
	cmp r4, #8
	bne .L_080e112e
	ldr r0, [sp, #28]
	movs r5, #0
	adds r0, #12
	str r5, [sp, #40]
	str r0, [sp, #16]
.L_080e118c:
	ldr r1, [sp, #40]
	cmp r1, #16
	ble .L_080e1198
	ldr r0, .L_080e13a4
	bl BattleFx_StepPaletteToResource
.L_080e1198:
	ldr r6, .L_080e13b4
	add r6, r9
	ldr r3, [r6]
	ldr r3, [r3, #28]
	cmp r3, #1
	bne .L_080e1248
	ldr r2, [sp, #40]
	lsls r5, r2, #11
	adds r0, r5, #0
	bl Trig_Sin
	ldr r3, [sp, #56]
	negs r0, r0
	lsrs r2, r3, #31
	adds r3, r3, r2
	lsls r0, r0, #2
	asrs r3, r3, #1
	asrs r0, r0, #16
	adds r0, r0, r3
	adds r7, r0, #0
	adds r0, r5, #0
	bl Trig_Cos
	ldr r3, [sp, #52]
	lsls r0, r0, #1
	asrs r0, r0, #16
	adds r0, r0, r3
	ldr r3, [sp, #40]
	adds r5, r0, #0
	subs r7, #10
	subs r5, #22
	cmp r3, #16
	ble .L_080e11e2
	lsls r3, r3, #1
	subs r3, r5, r3
	adds r5, r3, #0
	adds r5, #32
.L_080e11e2:
	ldr r3, [r6]
	ldr r3, [r3, #4]
	cmp r3, #1
	bne .L_080e11fc
	movs r3, #3
	str r3, [sp, #0]
	movs r0, #47
	movs r1, #7
	movs r2, #7
	movs r3, #7
	bl Unnamed_080ed408
	b .L_080e120a
.L_080e11fc:
	movs r3, #3
	movs r0, #47
	movs r1, #7
	movs r2, #7
	str r3, [sp, #0]
	bl Unnamed_080ed408
.L_080e120a:
	ldr r4, [sp, #40]
	cmp r4, #3
	bgt .L_080e122a
	movs r3, #20
	str r3, [sp, #0]
	movs r3, #40
	str r3, [sp, #4]
	ldr r0, .L_080e13bc
	ldr r1, .L_080e13a8
	ldr r4, [r0]
	add r1, r9
	ldr r0, [sp, #48]
	adds r2, r7, #0
	adds r3, r5, #0
	bl _call_via_r4
.L_080e122a:
	movs r0, #47
	bl Runtime_ReleaseHeapBlock
	movs r3, #20
	ldr r1, .L_080e13a8
	str r3, [sp, #0]
	movs r3, #40
	str r3, [sp, #4]
	ldr r0, [sp, #48]
	add r1, r9
	adds r2, r7, #0
	adds r3, r5, #0
	ldr r4, [sp, #32]
	bl _call_via_r4
.L_080e1248:
	ldr r5, [sp, #40]
	movs r3, #1
	ands r3, r5
	cmp r3, #0
	bne .L_080e1284
	movs r0, #0
	movs r5, #232
	lsls r5, r5, #7
	str r0, [sp, #44]
	ldr r6, .L_080e13c0
	add r5, r9
.L_080e125e:
	bl Random16
	movs r1, #6
	bl IwramUnsignedRemainderEntry
	adds r0, #3
	str r0, [r5, #12]
	bl Random16
	movs r3, #3
	ands r3, r0
	ldrb r3, [r6, r3]
	str r3, [r5, #16]
	ldr r1, [sp, #44]
	adds r1, #1
	adds r5, #28
	str r1, [sp, #44]
	cmp r1, #32
	bne .L_080e125e
.L_080e1284:
	bl Render_ResetTransformState
	ldr r0, [sp, #28]
	ldr r1, [sp, #16]
	bl Graphics_PrepareTransferInIwramWork
	movs r6, #225
	movs r2, #0
	mov r3, r9
	lsls r6, r6, #7
	str r2, [sp, #44]
	str r2, [sp, #12]
	str r3, [sp, #8]
	add r6, r9
.L_080e12a0:
	ldr r3, [r6, #24]
	cmp r3, #1
	beq .L_080e12a8
	b .L_080e14da
.L_080e12a8:
	ldr r4, [sp, #12]
	ldr r5, [sp, #40]
	str r4, [sp, #20]
	cmp r5, r4
	bgt .L_080e12b4
	b .L_080e142a
.L_080e12b4:
	add r5, sp, #60
	adds r1, r5, #0
	adds r0, r6, #0
	bl EffectPosition_ApplyBaseAndYOffset
	ldr r3, [r5]
	asrs r3, r3, #1
	str r3, [r5]
	subs r3, #12
	mov r10, r3
	ldr r3, [r5, #4]
	subs r3, #24
	mov r8, r3
	movs r3, #24
	str r3, [sp, #0]
	movs r3, #48
	str r3, [sp, #4]
	ldr r5, [sp, #32]
	mov r3, r8
	ldr r0, [sp, #48]
	mov r1, r9
	mov r2, r10
	bl _call_via_r5
	ldr r0, [sp, #40]
	movs r3, #3
	ands r3, r0
	cmp r3, #1
	bgt .L_080e1314
	ldr r3, .L_080e13c4
	ldr r4, .L_080e13c8
	ldrh r1, [r3, #2]
	ldr r3, .L_080e13cc
	ldrb r2, [r3, #1]
	ldrb r3, [r4, #1]
	ldr r4, .L_080e13d0
	ldrb r0, [r4, #1]
	str r0, [sp, #0]
	ldr r0, .L_080e13d4
	ldrb r0, [r0, #1]
	add r1, r9
	str r0, [sp, #4]
	add r2, r10
	add r3, r8
	ldr r0, [sp, #48]
	bl _call_via_r5
	b .L_080e1338
.L_080e1314:
	ldr r3, .L_080e13c4
	ldr r0, .L_080e13c8
	ldrh r1, [r3, #4]
	ldr r4, .L_080e13d0
	ldr r3, .L_080e13cc
	ldrb r2, [r3, #2]
	ldrb r3, [r0, #2]
	ldrb r0, [r4, #2]
	str r0, [sp, #0]
	ldr r0, .L_080e13d4
	ldrb r0, [r0, #2]
	add r1, r9
	str r0, [sp, #4]
	add r2, r10
	add r3, r8
	ldr r0, [sp, #48]
	bl _call_via_r5
.L_080e1338:
	ldr r0, [sp, #8]
	movs r1, #232
	movs r5, #0
	lsls r1, r1, #7
	mov r11, r5
	adds r7, r0, r1
.L_080e1344:
	movs r2, #2
	ldr r3, [r7, #16]
	movs r1, #7
	str r2, [sp, #0]
	movs r0, #47
	movs r2, #7
	bl Unnamed_080ed408
	ldr r2, .L_080e13bc
	ldr r1, [r7, #16]
	ldr r2, [r2]
	movs r3, #4
	ands r3, r1
	str r2, [sp, #36]
	cmp r3, #0
	beq .L_080e1378
	ldr r0, [r7, #12]
	ldr r4, .L_080e13d0
	ldrb r3, [r4, r0]
	mov r5, r10
	subs r3, r5, r3
	ldr r5, .L_080e13cc
	ldrb r2, [r5, r0]
	subs r3, r3, r2
	adds r3, #24
	b .L_080e1382
.L_080e1378:
	ldr r0, [r7, #12]
	ldr r2, .L_080e13cc
	ldrb r3, [r2, r0]
	ldr r4, .L_080e13d0
	add r3, r10
.L_080e1382:
	mov r12, r3
	movs r3, #8
	ands r3, r1
	cmp r3, #0
	beq .L_080e13d8
	ldr r5, .L_080e13d4
	ldrb r3, [r5, r0]
	ldr r5, .L_080e13c8
	mov r1, r8
	ldrb r2, [r5, r0]
	subs r3, r1, r3
	subs r3, r3, r2
	adds r5, r3, #0
	adds r5, #48
	b .L_080e13e0
.L_080e13a0:
	.4byte gWorkSlot
.L_080e13a4:
	.4byte 0x000000a7
.L_080e13a8:
	.4byte 0x000065c0
.L_080e13ac:
	.4byte 0x00000094
.L_080e13b0:
	.4byte 0x00007784
.L_080e13b4:
	.4byte 0x00007828
.L_080e13b8:
	.4byte BattlePresentation_ProcessPendingGraphicsTransfer
.L_080e13bc:
	.4byte gTransitionWork + 0xc
.L_080e13c0:
	.4byte ParticleReveal_CellSourceOffsets + 0x8
.L_080e13c4:
	.4byte ParticleReveal_CellSourceOffsets + 0x1e
.L_080e13c8:
	.4byte ParticleReveal_CellSourceOffsets + 0x39
.L_080e13cc:
	.4byte ParticleReveal_CellSourceOffsets + 0x30
.L_080e13d0:
	.4byte ParticleReveal_CellSourceOffsets + 0xc
.L_080e13d4:
	.4byte ParticleReveal_CellSourceOffsets + 0x15
.L_080e13d8:
	ldr r1, .L_080e153c
	ldrb r3, [r1, r0]
	mov r2, r8
	adds r5, r2, r3
.L_080e13e0:
	ldr r2, .L_080e1540
	lsls r3, r0, #1
	ldrh r1, [r2, r3]
	ldrb r3, [r4, r0]
	str r3, [sp, #0]
	ldr r3, [r7, #12]
	ldr r4, .L_080e1544
	ldrb r3, [r4, r3]
	add r1, r9
	str r3, [sp, #4]
	ldr r0, [sp, #48]
	adds r3, r5, #0
	mov r2, r12
	ldr r5, [sp, #36]
	bl _call_via_r5
	movs r0, #47
	bl Runtime_ReleaseHeapBlock
	movs r0, #1
	add r11, r0
	mov r1, r11
	adds r7, #28
	cmp r1, #4
	bne .L_080e1344
	ldr r3, [r6]
	ldr r2, [r6, #12]
	adds r3, r3, r2
	str r3, [r6]
	ldr r2, [r6, #16]
	ldr r3, [r6, #4]
	adds r3, r3, r2
	str r3, [r6, #4]
	ldr r2, [r6, #20]
	ldr r3, [r6, #8]
	adds r3, r3, r2
	str r3, [r6, #8]
.L_080e142a:
	ldr r3, [sp, #12]
	ldr r2, [sp, #40]
	adds r3, #16
	cmp r2, r3
	ble .L_080e14da
	ldr r4, [sp, #24]
	ldr r2, [r6]
	ldr r3, [r4, #8]
	subs r3, r3, r2
	ldr r2, [r6, #12]
	asrs r3, r3, #8
	adds r1, r2, r3
	ldr r2, [r6, #4]
	movs r3, #160
	lsls r3, r3, #13
	subs r3, r3, r2
	ldr r2, [r6, #16]
	asrs r3, r3, #8
	adds r0, r2, r3
	str r1, [r6, #12]
	str r0, [r6, #16]
	ldr r2, [r6, #8]
	ldr r3, [r4, #16]
	subs r3, r3, r2
	ldr r2, [r6, #20]
	asrs r3, r3, #8
	adds r4, r2, r3
	str r4, [r6, #20]
	ldr r3, [sp, #20]
	ldr r5, [sp, #40]
	adds r3, #85
	cmp r5, r3
	bge .L_080e149c
	lsls r3, r1, #4
	subs r3, r3, r1
	lsls r2, r3, #2
	cmp r2, #0
	bge .L_080e1478
	adds r2, #63
.L_080e1478:
	asrs r3, r2, #6
	str r3, [r6, #12]
	lsls r3, r0, #4
	subs r3, r3, r0
	lsls r2, r3, #2
	cmp r2, #0
	bge .L_080e1488
	adds r2, #63
.L_080e1488:
	asrs r3, r2, #6
	str r3, [r6, #16]
	lsls r3, r4, #4
	subs r3, r3, r4
	lsls r2, r3, #2
	cmp r2, #0
	bge .L_080e1498
	adds r2, #63
.L_080e1498:
	asrs r3, r2, #6
	str r3, [r6, #20]
.L_080e149c:
	ldr r3, [r6, #4]
	ldr r0, .L_080e1548
	cmp r3, r0
	bgt .L_080e14da
	ldr r2, .L_080e154c
	movs r3, #8
	add r2, r9
	str r3, [r2]
	movs r3, #0
	str r3, [r6, #24]
	movs r0, #134
	bl AudioCommand_PlayFar
	ldr r5, .L_080e1550
	add r5, r9
	ldr r3, [r5]
	movs r1, #36
	ldrsh r0, [r3, r1]
	movs r3, #4
	str r3, [sp, #0]
	movs r1, #7
	movs r2, #5
	movs r3, #0
	bl ObjectGroup_UpdateMembers
	ldr r3, [r5]
	movs r1, #4
	movs r2, #36
	ldrsh r0, [r3, r2]
	bl BattleMotion_ApplyVariantMotionFar
.L_080e14da:
	ldr r3, [sp, #12]
	ldr r4, [sp, #8]
	ldr r5, [sp, #44]
	adds r3, #2
	adds r4, #112
	adds r5, #1
	str r3, [sp, #12]
	adds r6, #28
	str r4, [sp, #8]
	str r5, [sp, #44]
	cmp r5, #6
	beq .L_080e14f4
	b .L_080e12a0
.L_080e14f4:
	movs r0, #16
	movs r1, #16
	bl Camera_ApplyShake
	bl ObjectGroup_TickMemberTimers
	ldr r2, .L_080e1554
	movs r3, #1
	add r2, r9
	movs r0, #1
	str r3, [r2]
	bl WaitFrames
	ldr r0, [sp, #40]
	adds r0, #1
	str r0, [sp, #40]
	cmp r0, #96
	beq .L_080e151a
	b .L_080e118c
.L_080e151a:
	ldr r0, .L_080e1558
	bl Scheduler_RemoveCallback
	movs r0, #46
	bl Runtime_ReleaseHeapBlock
	bl BattleFx_EndCanvasLayer
	add sp, #72
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
.L_080e153c:
	.4byte ParticleReveal_CellSourceOffsets + 0x39
.L_080e1540:
	.4byte ParticleReveal_CellSourceOffsets + 0x1e
.L_080e1544:
	.4byte ParticleReveal_CellSourceOffsets + 0x15
.L_080e1548:
	.4byte 0x0013ffff
.L_080e154c:
	.4byte 0x000077a8
.L_080e1550:
	.4byte 0x00007828
.L_080e1554:
	.4byte 0x00007824
.L_080e1558:
	.4byte BattlePresentation_ProcessPendingGraphicsTransfer
