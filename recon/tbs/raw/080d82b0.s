.syntax unified
	.thumb
	.global Unnamed_080d82b0
	.thumb_func
Unnamed_080d82b0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r5, .L_080d859c
	adds r3, r5, #0
	ldmia r3!, {r1}
	sub sp, #56
	str r1, [sp, #28]
	ldr r3, [r3]
	str r3, [sp, #24]
	ldr r2, [r5, #8]
	ldr r4, .L_080d85a0
	str r2, [sp, #16]
	adds r3, r5, #0
	subs r3, #108
	ldr r6, [r3]
	adds r3, r1, r4
	str r0, [r3]
	movs r0, #1
	bl BattleFx_BeginCanvasLayer
	movs r2, #0
	ldr r1, [sp, #16]
	movs r3, #0
	ldr r0, .L_080d85a4
	bl Resource_LoadAndDecompress
	ldr r0, .L_080d85a8
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_080d85ac
	movs r2, #128
	lsls r0, r0, #19
	bl _call_via_r3
	movs r3, #2
	str r3, [sp, #0]
	movs r1, #7
	movs r2, #7
	movs r3, #3
	movs r0, #46
	bl Unnamed_080ed408
	ldr r5, [r5, #28]
	movs r0, #0
	movs r1, #1
	movs r2, #128
	str r5, [sp, #20]
	ldr r3, .L_080d85b0
	mov r8, r0
	negs r1, r1
	lsls r2, r2, #3
.L_080d8324:
	movs r4, #1
	add r8, r4
	str r1, [r3]
	adds r3, #28
	cmp r8, r2
	bne .L_080d8324
	bl Render_ResetTransformState
	adds r1, r6, #0
	adds r1, #12
	adds r0, r6, #0
	bl Graphics_PrepareTransferInIwramWork
	ldr r2, .L_080d85a0
	ldr r1, [sp, #28]
	ldr r3, [r1, r2]
	ldr r3, [r3, #20]
	movs r0, #0
	mov r11, r0
	cmp r3, #0
	beq .L_080d8436
	movs r3, #44
	movs r4, #32
	movs r0, #36
	movs r1, #0
	add r3, sp
	add r4, sp
	str r0, [sp, #12]
	str r1, [sp, #8]
	mov r9, r3
	mov r10, r4
.L_080d8362:
	ldr r3, [sp, #28]
	adds r5, r3, r2
	ldr r4, [sp, #12]
	ldr r3, [r5]
	ldrsh r0, [r3, r4]
	bl GetBattleObjectSlotFar
	ldr r3, [r5]
	ldr r2, [sp, #12]
	ldr r6, [r0]
	ldrsh r0, [r3, r2]
	bl Func_080b5070
	lsrs r3, r0, #31
	adds r0, r0, r3
	ldr r3, [r6, #8]
	mov r1, r9
	asrs r0, r0, #1
	str r0, [r1, #4]
	str r3, [r1]
	ldr r3, [r6, #16]
	mov r0, r9
	str r3, [r1, #8]
	mov r1, r10
	bl EffectPosition_ApplyBaseAndYOffset
	mov r2, r10
	ldr r3, [r2]
	asrs r3, r3, #1
	str r3, [r2]
	ldr r4, [sp, #8]
	ldr r0, .L_080d85b4
	movs r3, #0
	mov r8, r3
	adds r7, r4, r0
.L_080d83a8:
	bl Random16
	movs r1, #255
	adds r6, r0, #0
	ands r6, r1
	bl Random16
	ldr r3, .L_080d85b8
	adds r5, r0, #0
	ands r5, r3
	adds r0, r5, #0
	bl Trig_Sin
	adds r2, r6, #0
	muls r2, r0
	mov r4, r10
	ldr r3, [r4]
	asrs r2, r2, #7
	lsls r3, r3, #16
	adds r2, r2, r3
	str r2, [r7]
	adds r0, r5, #0
	bl Func_0800231c
	adds r2, r6, #0
	muls r2, r0
	mov r0, r10
	ldr r3, [r0, #4]
	asrs r2, r2, #3
	lsls r3, r3, #16
	adds r2, r2, r3
	str r2, [r7, #4]
	bl Random16
	movs r1, #255
	ands r0, r1
	movs r3, #128
	subs r3, r3, r0
	lsls r3, r3, #9
	str r3, [r7, #12]
	bl Random16
	movs r2, #255
	movs r3, #0
	ands r0, r2
	str r3, [r7, #24]
	negs r0, r0
	movs r3, #1
	subs r0, #128
	add r8, r3
	lsls r0, r0, #10
	mov r4, r8
	str r0, [r7, #16]
	adds r7, #28
	cmp r4, #128
	bne .L_080d83a8
	ldr r0, [sp, #12]
	ldr r1, [sp, #8]
	movs r2, #224
	lsls r2, r2, #4
	adds r1, r1, r2
	adds r0, #2
	str r0, [sp, #12]
	str r1, [sp, #8]
	ldr r2, .L_080d85a0
	ldr r4, [sp, #28]
	add r11, r3
	ldr r3, [r4, r2]
	ldr r3, [r3, #20]
	cmp r11, r3
	bne .L_080d8362
.L_080d8436:
	ldr r0, [sp, #28]
	movs r1, #239
	lsls r1, r1, #7
	adds r2, r0, r1
	movs r3, #2
	str r3, [r2]
	ldr r3, .L_080d85bc
	movs r1, #144
	adds r2, r0, r3
	movs r3, #50
	str r3, [r2]
	lsls r1, r1, #3
	ldr r0, .L_080d85c0
	bl Scheduler_AddOrUpdateCallback
	ldr r1, .L_080d85a0
	ldr r0, [sp, #28]
	adds r3, r0, r1
	ldr r3, [r3]
	ldr r2, [r3, #20]
	lsls r3, r2, #2
	adds r3, r3, r2
	movs r2, #56
	movs r4, #0
	lsls r3, r3, #2
	negs r2, r2
	mov r9, r4
	cmp r3, r2
	bne .L_080d8472
	b .L_080d8578
.L_080d8472:
	adds r6, r0, r1
.L_080d8474:
	mov r3, r9
	cmp r3, #32
	bne .L_080d8480
	movs r0, #0
	bl BattleEventRuntime_BeginPhaseFar
.L_080d8480:
	ldr r3, [r6]
	ldr r3, [r3, #20]
	movs r4, #0
	mov r11, r4
	cmp r3, #0
	beq .L_080d854e
	movs r0, #0
	movs r7, #0
	mov r10, r0
.L_080d8492:
	cmp r9, r7
	bne .L_080d84b6
	movs r0, #143
	bl Func_080f9010
	mov r1, r11
	ldr r2, [r6]
	lsls r3, r1, #1
	adds r3, #36
	ldrsh r0, [r2, r3]
	movs r3, #20
	movs r2, #1
	str r3, [sp, #0]
	movs r1, #7
	negs r2, r2
	mov r3, r11
	bl ObjectGroup_UpdateMembers
.L_080d84b6:
	cmp r9, r7
	ble .L_080d853a
	ldr r5, .L_080d85b4
	movs r0, #0
	mov r8, r0
	add r5, r10
.L_080d84c2:
	ldr r3, [r5, #24]
	cmp r3, #0
	blt .L_080d852e
	movs r1, #3
	mov r0, r8
	bl Func_080022fc
	adds r0, #1
	lsls r4, r0, #1
	ldr r2, .L_080d85c4
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
	ldr r0, [sp, #24]
	ldr r4, [sp, #20]
	bl _call_via_r4
	movs r0, #3
	mov r3, r8
	ldr r2, .L_080d85c8
	ands r3, r0
	lsls r3, r3, #2
	ldr r2, [r2, r3]
	adds r0, r5, #0
	movs r1, #62
	bl EffectStep_AdvanceWithGravity2D
	ldr r3, [r5, #24]
	adds r3, #1
	str r3, [r5, #24]
	ldr r3, [r5, #16]
	cmp r3, #0
	ble .L_080d852e
	movs r1, #6
	ldrsh r3, [r5, r1]
	cmp r3, #112
	ble .L_080d852e
	movs r3, #1
	negs r3, r3
	str r3, [r5, #24]
.L_080d852e:
	movs r2, #1
	add r8, r2
	mov r3, r8
	adds r5, #28
	cmp r3, #128
	bne .L_080d84c2
.L_080d853a:
	ldr r3, [r6]
	movs r4, #224
	movs r0, #1
	ldr r3, [r3, #20]
	lsls r4, r4, #4
	add r11, r0
	adds r7, #20
	add r10, r4
	cmp r11, r3
	bne .L_080d8492
.L_080d854e:
	bl ObjectGroup_TickMemberTimers
	ldr r3, .L_080d85cc
	ldr r1, [sp, #28]
	adds r2, r1, r3
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r3, [r6]
	ldr r2, [r3, #20]
	lsls r3, r2, #2
	adds r3, r3, r2
	movs r4, #1
	lsls r3, r3, #2
	add r9, r4
	adds r3, #56
	cmp r9, r3
	beq .L_080d8578
	b .L_080d8474
.L_080d8578:
	ldr r0, .L_080d85c0
	bl Scheduler_RemoveCallback
	movs r0, #46
	bl Runtime_ReleaseHeapBlock
	bl BattleFx_EndCanvasLayer
	add sp, #56
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
.L_080d859c:
	.4byte gBattleFxWork
.L_080d85a0:
	.4byte 0x00007828
.L_080d85a4:
	.4byte 0x00000073
.L_080d85a8:
	.4byte 0x000000b9
.L_080d85ac:
	.4byte IwramCopyWords
.L_080d85b0:
	.4byte Data_02010018
.L_080d85b4:
	.4byte gMapCellBuffer
.L_080d85b8:
	.4byte 0x0000ffff
.L_080d85bc:
	.4byte 0x00007784
.L_080d85c0:
	.4byte BattlePresentation_ProcessPendingGraphicsTransfer
.L_080d85c4:
	.4byte ParticleStreams_CellOffsets
.L_080d85c8:
	.4byte Data_080ee9f8
.L_080d85cc:
	.4byte 0x00007824
