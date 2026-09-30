.syntax unified
	.thumb
	.global Unnamed_080e01e4
	.thumb_func
Unnamed_080e01e4:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r2, .L_080e0254
	adds r3, r2, #0
	ldmia r3!, {r1}
	ldr r3, [r3]
	sub sp, #40
	str r3, [sp, #28]
	ldr r3, .L_080e0258
	mov r9, r1
	ldr r2, [r2, #8]
	add r3, r9
	str r2, [sp, #24]
	str r0, [r3]
	movs r0, #0
	bl BattleFx_BeginCanvasLayer
	ldr r3, .L_080e0250
	ldr r2, .L_080e025c
	strh r3, [r2]
	mov r2, sp
	adds r2, #32
	adds r1, r2, #0
	movs r0, #0
	str r2, [sp, #20]
	bl BattleFx_FetchRectangleBlitters
	ldr r0, .L_080e0260
	ldr r1, [sp, #24]
	movs r2, #0
	movs r3, #0
	bl Resource_LoadAndDecompress
	ldr r0, .L_080e0264
	mov r1, r9
	movs r2, #1
	movs r3, #1
	bl Resource_LoadAndDecompress
	movs r1, #200
	lsls r1, r1, #2
	ldr r0, .L_080e0268
	add r1, r9
	movs r2, #1
	movs r3, #0
	bl Resource_LoadAndDecompress
	b .L_080e026c
	.2byte 0x0000
.L_080e0250:
	.4byte 0x00001010
.L_080e0254:
	.4byte gBattleFxWork
.L_080e0258:
	.4byte 0x00007828
.L_080e025c:
	.4byte 0x04000052
.L_080e0260:
	.4byte 0x00000073
.L_080e0264:
	.4byte 0x00000090
.L_080e0268:
	.4byte 0x00000089
.L_080e026c:
	movs r2, #239
	lsls r2, r2, #7
	add r2, r9
	movs r3, #2
	str r3, [r2]
	ldr r2, .L_080e04f8
	movs r3, #75
	add r2, r9
	movs r1, #144
	str r3, [r2]
	ldr r0, .L_080e04fc
	lsls r1, r1, #3
	movs r5, #225
	bl Scheduler_AddOrUpdateCallback
	lsls r5, r5, #7
	movs r3, #0
	mov r10, r3
	movs r6, #63
	add r5, r9
.L_080e0294:
	bl Random16
	ands r0, r6
	adds r0, #64
	str r0, [r5]
	bl Random16
	movs r7, #1
	ands r0, r6
	subs r0, #80
	add r10, r7
	str r0, [r5, #4]
	mov r0, r10
	adds r5, #28
	cmp r0, #32
	bne .L_080e0294
	movs r1, #0
	movs r2, #128
	ldr r3, .L_080e0500
	mov r10, r1
	lsls r2, r2, #2
	subs r1, #1
.L_080e02c0:
	movs r7, #1
	add r10, r7
	str r1, [r3]
	adds r3, #28
	cmp r10, r2
	bne .L_080e02c0
	movs r0, #171
	bl Func_080f9010
	movs r1, #128
	lsls r1, r1, #8
	movs r0, #0
	str r1, [sp, #16]
	mov r11, r0
.L_080e02dc:
	mov r2, r11
	cmp r2, #56
	bne .L_080e02e8
	movs r0, #133
	bl BattleEventRuntime_BeginPhaseFar
.L_080e02e8:
	mov r3, r11
	cmp r3, #95
	bgt .L_080e0326
	ldr r0, [sp, #16]
	bl Trig_Sin
	mov r7, r11
	lsls r3, r7, #1
	movs r5, #64
	subs r5, r5, r3
	adds r6, r5, #0
	muls r6, r0
	ldr r0, [sp, #16]
	bl Func_0800231c
	adds r3, r5, #0
	muls r3, r0
	movs r2, #20
	asrs r6, r6, #17
	asrs r3, r3, #16
	adds r6, #86
	str r2, [sp, #0]
	movs r2, #40
	str r2, [sp, #4]
	adds r3, #28
	ldr r4, [sp, #32]
	ldr r0, [sp, #28]
	mov r1, r9
	adds r2, r6, #0
	bl _call_via_r4
.L_080e0326:
	movs r1, #225
	movs r0, #0
	lsls r1, r1, #7
	add r1, r9
	str r0, [sp, #12]
	mov r10, r0
	mov r8, r1
.L_080e0334:
	mov r2, r10
	lsls r3, r2, #2
	adds r3, #8
	cmp r11, r3
	blt .L_080e0422
	mov r7, r8
	ldr r3, [r7, #4]
	cmp r3, #95
	bgt .L_080e0422
	movs r1, #40
	ldr r2, [r7]
	str r1, [sp, #0]
	movs r1, #64
	str r1, [sp, #4]
	movs r1, #200
	lsls r1, r1, #2
	subs r3, #32
	subs r2, #20
	ldr r4, [sp, #32]
	ldr r0, [sp, #28]
	add r1, r9
	bl _call_via_r4
	ldr r3, [r7]
	subs r3, #6
	str r3, [r7]
	ldr r3, [r7, #4]
	adds r3, #12
	str r3, [r7, #4]
	cmp r3, #95
	ble .L_080e0422
	ldr r0, [sp, #12]
	ldr r1, .L_080e0504
	movs r4, #0
	adds r7, r0, r1
.L_080e037a:
	str r4, [sp, #8]
	bl Random16
	ldr r3, .L_080e0508
	adds r6, r0, #0
	ands r6, r3
	bl Random16
	ldr r5, .L_080e050c
	ands r5, r0
	mov r0, r8
	ldr r3, [r0]
	lsls r3, r3, #16
	str r3, [r7]
	ldr r3, [r0, #4]
	movs r2, #128
	lsls r3, r3, #16
	lsls r2, r2, #1
	str r3, [r7, #4]
	adds r0, r6, #0
	adds r5, r5, r2
	bl Trig_Sin
	adds r3, r5, #0
	muls r3, r0
	asrs r3, r3, #7
	str r3, [r7, #12]
	adds r0, r6, #0
	bl Func_0800231c
	adds r3, r5, #0
	muls r3, r0
	asrs r3, r3, #6
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
	bne .L_080e037a
	movs r0, #133
	bl Func_080f9010
	ldr r2, .L_080e0510
	movs r3, #4
	add r2, r9
	str r3, [r2]
	ldr r3, .L_080e0514
	mov r1, r9
	ldr r3, [r1, r3]
	ldr r3, [r3, #20]
	movs r4, #0
	cmp r3, #0
	beq .L_080e0422
	ldr r5, .L_080e0514
	movs r6, #36
	add r5, r9
.L_080e03f6:
	ldr r3, [r5]
	ldrsh r0, [r3, r6]
	movs r3, #6
	str r3, [sp, #0]
	movs r1, #7
	adds r3, r4, #0
	movs r2, #5
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
	bne .L_080e03f6
.L_080e0422:
	ldr r1, [sp, #12]
	movs r2, #224
	movs r3, #1
	lsls r2, r2, #2
	add r10, r3
	movs r0, #28
	adds r1, r1, r2
	mov r7, r10
	add r8, r0
	str r1, [sp, #12]
	cmp r7, #8
	beq .L_080e043c
	b .L_080e0334
.L_080e043c:
	movs r0, #0
	ldr r5, .L_080e0504
	ldr r6, .L_080e0518
	mov r10, r0
.L_080e0444:
	movs r1, #1
	ldr r0, [r5, #24]
	negs r1, r1
	cmp r0, r1
	beq .L_080e0494
	cmp r0, #0
	bge .L_080e0454
	adds r0, #15
.L_080e0454:
	asrs r0, r0, #4
	adds r0, #1
	lsls r4, r0, #1
	subs r3, r4, #2
	ldrh r1, [r6, r3]
	ldr r2, [sp, #24]
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
	ldr r0, [sp, #20]
	ldr r4, [r0, #4]
	ldr r0, [sp, #28]
	bl _call_via_r4
	movs r2, #128
	adds r0, r5, #0
	movs r1, #62
	lsls r2, r2, #6
	bl EffectStep_AdvanceWithGravity2D
	ldr r3, [r5, #24]
	subs r3, #1
	str r3, [r5, #24]
.L_080e0494:
	movs r1, #1
	movs r2, #128
	add r10, r1
	lsls r2, r2, #2
	adds r5, #28
	cmp r10, r2
	bne .L_080e0444
	movs r1, #4
	movs r0, #4
	bl Camera_ApplyShake
	bl ObjectGroup_TickMemberTimers
	ldr r2, .L_080e051c
	movs r3, #1
	add r2, r9
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r7, .L_080e0520
	ldr r3, [sp, #16]
	movs r0, #1
	add r11, r0
	adds r3, r3, r7
	mov r1, r11
	str r3, [sp, #16]
	cmp r1, #96
	beq .L_080e04d0
	b .L_080e02dc
.L_080e04d0:
	ldr r0, .L_080e04fc
	bl Scheduler_RemoveCallback
	movs r0, #47
	bl Runtime_ReleaseHeapBlock
	movs r0, #46
	bl Runtime_ReleaseHeapBlock
	bl BattleFx_EndCanvasLayer
	add sp, #40
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
.L_080e04f8:
	.4byte 0x00007784
.L_080e04fc:
	.4byte BattlePresentation_ProcessPendingGraphicsTransfer
.L_080e0500:
	.4byte Data_02010018
.L_080e0504:
	.4byte gMapCellBuffer
.L_080e0508:
	.4byte 0x0000ffff
.L_080e050c:
	.4byte 0x000001ff
.L_080e0510:
	.4byte 0x000077a8
.L_080e0514:
	.4byte 0x00007828
.L_080e0518:
	.4byte ParticleStreams_CellOffsets
.L_080e051c:
	.4byte 0x00007824
.L_080e0520:
	.4byte 0xfffff800
