.syntax unified
	.thumb
	.global Func_080dfe2c
	.thumb_func
Func_080dfe2c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r2, .L_080e01a4
	adds r3, r2, #0
	ldmia r3!, {r1}
	ldr r3, [r3]
	sub sp, #36
	str r3, [sp, #24]
	ldr r3, .L_080e01a8
	mov r9, r1
	ldr r2, [r2, #8]
	add r3, r9
	str r2, [sp, #16]
	str r0, [r3]
	movs r0, #0
	bl BattleFx_BeginCanvasLayer
	mov r2, sp
	adds r2, #28
	adds r1, r2, #0
	movs r0, #0
	str r2, [sp, #12]
	bl BattleFx_FetchRectangleBlitters
	ldr r5, .L_080e01ac
	ldr r1, [sp, #16]
	ldr r0, .L_080e01b0
	movs r2, #0
	movs r3, #0
	bl Resource_LoadAndDecompress
	ldr r0, .L_080e01b4
	mov r1, r9
	movs r2, #1
	movs r3, #0
	bl Resource_LoadAndDecompress
	adds r1, r5, #0
	ldr r0, .L_080e01b8
	movs r2, #1
	movs r3, #1
	bl Resource_LoadAndDecompress
	movs r1, #170
	lsls r1, r1, #2
	adds r0, r5, #0
	add r1, r9
	movs r2, #17
	movs r3, #104
	bl Graphics_TransposeCopy
	movs r3, #221
	lsls r3, r3, #3
	movs r1, #153
	adds r5, r5, r3
	lsls r1, r1, #4
	add r1, r9
	adds r0, r5, #0
	movs r2, #34
	movs r3, #65
	bl Graphics_TransposeCopy
	movs r2, #239
	lsls r2, r2, #7
	add r2, r9
	movs r3, #2
	str r3, [r2]
	ldr r2, .L_080e01bc
	movs r3, #75
	add r2, r9
	movs r1, #144
	str r3, [r2]
	ldr r0, .L_080e01c0
	lsls r1, r1, #3
	bl Scheduler_AddOrUpdateCallback
	ldr r3, .L_080e01c4
	movs r7, #0
	movs r2, #1
	mov r10, r7
	negs r2, r2
	add r3, r9
.L_080dfeda:
	movs r0, #1
	add r10, r0
	mov r1, r10
	str r2, [r3]
	adds r3, #28
	cmp r1, #8
	bne .L_080dfeda
	movs r2, #0
	mov r10, r2
	movs r1, #1
	movs r2, #128
	ldr r3, .L_080e01c8
	negs r1, r1
	lsls r2, r2, #2
.L_080dfef6:
	movs r7, #1
	add r10, r7
	str r1, [r3]
	adds r3, #28
	cmp r10, r2
	bne .L_080dfef6
	movs r0, #162
	bl AudioCommand_PlayFar
	movs r0, #0
	str r0, [sp, #20]
.L_080dff0c:
	ldr r1, [sp, #20]
	cmp r1, #56
	bne .L_080dff18
	movs r0, #133
	bl BattleEventRuntime_BeginPhaseFar
.L_080dff18:
	movs r3, #225
	lsls r3, r3, #7
	movs r2, #0
	add r3, r9
	mov r10, r2
	mov r8, r3
	mov r11, r2
.L_080dff26:
	mov r7, r8
	movs r0, #1
	ldr r3, [r7, #24]
	negs r0, r0
	cmp r3, r0
	beq .L_080dffce
	movs r1, #65
	ldr r2, [r7]
	ldr r3, [r7, #4]
	str r1, [sp, #0]
	movs r1, #34
	str r1, [sp, #4]
	movs r1, #153
	lsls r1, r1, #4
	subs r3, #17
	subs r2, #16
	ldr r4, [sp, #28]
	ldr r0, [sp, #24]
	add r1, r9
	bl _call_via_r4
	ldr r3, [r7]
	subs r3, #12
	str r3, [r7]
	ldr r3, [r7, #24]
	adds r3, #1
	str r3, [r7, #24]
	cmp r3, #5
	bne .L_080dffce
	movs r0, #133
	bl AudioCommand_PlayFar
	ldr r2, .L_080e01cc
	movs r3, #4
	add r2, r9
	ldr r7, .L_080e01ac
	str r3, [r2]
	movs r4, #0
	add r7, r11
.L_080dff74:
	str r4, [sp, #8]
	bl Random16
	ldr r3, .L_080e01d0
	adds r6, r0, #0
	ands r6, r3
	bl Random16
	mov r2, r8
	ldr r3, [r2]
	lsls r3, r3, #16
	str r3, [r7]
	ldr r3, [r2, #4]
	ldr r5, .L_080e01d4
	lsls r3, r3, #16
	movs r1, #128
	lsls r1, r1, #1
	str r3, [r7, #4]
	ands r5, r0
	adds r0, r6, #0
	adds r5, r5, r1
	bl Trig_Sin
	adds r3, r5, #0
	muls r3, r0
	asrs r3, r3, #8
	str r3, [r7, #12]
	adds r0, r6, #0
	bl Trig_Cos
	adds r3, r5, #0
	muls r3, r0
	asrs r3, r3, #7
	str r3, [r7, #16]
	bl Random16
	movs r3, #15
	ldr r4, [sp, #8]
	ands r3, r0
	adds r3, #32
	adds r4, #1
	str r3, [r7, #24]
	adds r7, #28
	cmp r4, #32
	bne .L_080dff74
.L_080dffce:
	movs r0, #1
	movs r7, #224
	add r10, r0
	movs r3, #28
	lsls r7, r7, #2
	mov r1, r10
	add r8, r3
	add r11, r7
	cmp r1, #5
	bne .L_080dff26
	ldr r2, [sp, #20]
	cmp r2, #95
	ble .L_080dffea
	b .L_080e00f0
.L_080dffea:
	lsls r6, r2, #11
	adds r0, r6, #0
	bl Trig_Sin
	ldr r7, [sp, #20]
	movs r5, #64
	lsls r3, r7, #1
	subs r5, r5, r3
	adds r3, r5, #0
	muls r3, r0
	movs r0, #96
	asrs r7, r3, #17
	adds r0, r0, r7
	mov r8, r0
	adds r0, r6, #0
	bl Trig_Cos
	adds r3, r5, #0
	muls r3, r0
	movs r1, #60
	asrs r3, r3, #16
	adds r1, r1, r3
	mov r11, r1
	movs r1, #20
	str r1, [sp, #0]
	movs r1, #34
	str r1, [sp, #4]
	ldr r0, [sp, #12]
	adds r2, r7, #0
	adds r2, #86
	adds r3, #43
	ldr r4, [r0, #4]
	mov r1, r9
	ldr r0, [sp, #24]
	bl _call_via_r4
	ldr r0, .L_080e01d8
	movs r1, #0
	ldrb r3, [r0, r1]
	ldr r2, [sp, #20]
	mov r10, r1
	cmp r2, r3
	bne .L_080e0064
	movs r1, #225
	lsls r1, r1, #7
	add r1, r9
	movs r2, #1
	ldr r3, [r1, #24]
	negs r2, r2
	cmp r3, r2
	bne .L_080e0064
	adds r3, r7, #0
	adds r3, #88
	str r3, [r1]
	mov r7, r11
	mov r3, r8
	mov r0, r10
	str r3, [r1, #12]
	str r7, [r1, #4]
	str r0, [r1, #24]
	b .L_080e00f0
.L_080e0064:
	mov r1, r10
	ldrb r3, [r0, r1]
	ldr r2, [sp, #20]
	adds r3, #6
	cmp r2, r3
	bne .L_080e00b0
	ldr r3, .L_080e01a8
	mov r7, r9
	ldr r3, [r7, r3]
	ldr r3, [r3, #20]
	movs r4, #0
	cmp r3, #0
	beq .L_080e00b0
	ldr r5, .L_080e01a8
	movs r6, #36
	add r5, r9
.L_080e0084:
	ldr r3, [r5]
	ldrsh r0, [r3, r6]
	movs r3, #6
	str r3, [sp, #0]
	movs r2, #5
	adds r3, r4, #0
	movs r1, #7
	str r4, [sp, #8]
	bl ObjectGroup_UpdateMembers
	ldr r3, [r5]
	movs r1, #6
	ldrsh r0, [r3, r6]
	bl BattleMotion_ApplyVariantMotionFar
	ldr r3, [r5]
	ldr r4, [sp, #8]
	ldr r3, [r3, #20]
	adds r4, #1
	adds r6, #2
	cmp r4, r3
	bne .L_080e0084
.L_080e00b0:
	movs r3, #1
	add r10, r3
	mov r7, r10
	cmp r7, #5
	beq .L_080e00f0
	ldr r3, .L_080e01d8
	adds r0, r3, #0
	ldrb r3, [r0, r7]
	ldr r1, [sp, #20]
	cmp r1, r3
	bne .L_080e0064
	lsls r3, r7, #3
	subs r3, r3, r7
	lsls r3, r3, #2
	movs r7, #225
	add r3, r9
	lsls r7, r7, #7
	adds r2, r3, r7
	movs r1, #1
	ldr r3, [r2, #24]
	negs r1, r1
	cmp r3, r1
	bne .L_080e0064
	mov r3, r8
	subs r3, #8
	str r3, [r2]
	mov r3, r8
	str r3, [r2, #12]
	mov r7, r11
	movs r3, #0
	str r7, [r2, #4]
	str r3, [r2, #24]
.L_080e00f0:
	movs r0, #0
	ldr r5, .L_080e01ac
	ldr r6, .L_080e01dc
	mov r10, r0
.L_080e00f8:
	movs r1, #1
	ldr r0, [r5, #24]
	negs r1, r1
	cmp r0, r1
	beq .L_080e0148
	cmp r0, #0
	bge .L_080e0108
	adds r0, #15
.L_080e0108:
	asrs r0, r0, #4
	adds r0, #2
	lsls r4, r0, #1
	subs r3, r4, #2
	ldrh r1, [r6, r3]
	ldr r2, [sp, #16]
	adds r1, r2, r1
	movs r3, #2
	ldrsh r2, [r5, r3]
	lsrs r3, r0, #31
	adds r3, r0, r3
	asrs r3, r3, #1
	subs r2, r2, r3
	movs r7, #6
	ldrsh r3, [r5, r7]
	str r0, [sp, #0]
	subs r3, r3, r0
	str r4, [sp, #4]
	ldr r0, [sp, #12]
	ldr r4, [r0, #4]
	ldr r0, [sp, #24]
	bl _call_via_r4
	movs r2, #128
	adds r0, r5, #0
	movs r1, #62
	lsls r2, r2, #6
	bl EffectStep_AdvanceWithGravity2D
	ldr r3, [r5, #24]
	subs r3, #1
	str r3, [r5, #24]
.L_080e0148:
	movs r1, #1
	movs r2, #128
	add r10, r1
	lsls r2, r2, #1
	adds r5, #28
	cmp r10, r2
	bne .L_080e00f8
	movs r0, #4
	movs r1, #4
	bl Camera_ApplyShake
	bl ObjectGroup_TickMemberTimers
	ldr r2, .L_080e01e0
	movs r3, #1
	add r2, r9
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r3, [sp, #20]
	adds r3, #1
	str r3, [sp, #20]
	cmp r3, #96
	beq .L_080e017c
	b .L_080dff0c
.L_080e017c:
	ldr r0, .L_080e01c0
	bl Scheduler_RemoveCallback
	movs r0, #47
	bl Runtime_ReleaseHeapBlock
	movs r0, #46
	bl Runtime_ReleaseHeapBlock
	bl BattleFx_EndCanvasLayer
	add sp, #36
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
.L_080e01a4:
	.4byte gBattleFxWork
.L_080e01a8:
	.4byte 0x00007828
.L_080e01ac:
	.4byte gMapCellBuffer
.L_080e01b0:
	.4byte 0x00000073
.L_080e01b4:
	.4byte 0x00000092
.L_080e01b8:
	.4byte 0x0000006f
.L_080e01bc:
	.4byte 0x00007784
.L_080e01c0:
	.4byte BattlePresentation_ProcessPendingGraphicsTransfer
.L_080e01c4:
	.4byte 0x00007098
.L_080e01c8:
	.4byte gMapCellBuffer + 0x18
.L_080e01cc:
	.4byte 0x000077a8
.L_080e01d0:
	.4byte 0x0000ffff
.L_080e01d4:
	.4byte 0x000001ff
.L_080e01d8:
	.4byte Data_080eec5a
.L_080e01dc:
	.4byte ParticleStreams_CellOffsets
.L_080e01e0:
	.4byte 0x00007824
