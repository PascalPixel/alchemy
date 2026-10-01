.syntax unified
	.thumb
	.global Func_080e0564
	.thumb_func
Func_080e0564:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r5, .L_080e05d4
	adds r3, r5, #0
	ldmia r3!, {r1}
	ldr r3, [r3]
	sub sp, #40
	str r3, [sp, #36]
	ldr r3, .L_080e05d8
	mov r9, r1
	ldr r2, [r5, #8]
	add r3, r9
	str r2, [sp, #20]
	str r0, [r3]
	movs r0, #0
	bl BattleFx_BeginCanvasLayer
	ldr r2, .L_080e05dc
	ldr r3, .L_080e05d0
	movs r6, #2
	strh r3, [r2]
	movs r1, #7
	movs r2, #7
	movs r3, #11
	movs r0, #46
	str r6, [sp, #0]
	bl Unnamed_080ed408
	movs r3, #3
	movs r1, #7
	movs r2, #7
	movs r0, #47
	str r3, [sp, #0]
	bl Unnamed_080ed408
	ldr r3, [r5, #28]
	ldr r5, [r5, #32]
	str r3, [sp, #24]
	ldr r1, [sp, #20]
	ldr r0, .L_080e05e0
	movs r2, #0
	movs r3, #0
	str r5, [sp, #28]
	bl Resource_LoadAndDecompress
	ldr r0, .L_080e05e4
	mov r1, r9
	movs r2, #1
	b .L_080e05e8
.L_080e05d0:
	.4byte 0x00001010
.L_080e05d4:
	.4byte gBattleFxWork
.L_080e05d8:
	.4byte 0x00007828
.L_080e05dc:
	.4byte 0x04000052
.L_080e05e0:
	.4byte 0x00000073
.L_080e05e4:
	.4byte 0x00000094
.L_080e05e8:
	movs r3, #1
	bl Resource_LoadAndDecompress
	movs r1, #190
	lsls r1, r1, #2
	ldr r0, .L_080e0890
	add r1, r9
	movs r2, #1
	movs r3, #0
	bl Resource_LoadAndDecompress
	movs r3, #239
	lsls r3, r3, #7
	ldr r2, .L_080e0894
	add r3, r9
	str r6, [r3]
	add r2, r9
	movs r3, #75
	movs r1, #144
	str r3, [r2]
	ldr r0, .L_080e0898
	lsls r1, r1, #3
	movs r5, #225
	bl Scheduler_AddOrUpdateCallback
	lsls r5, r5, #7
	movs r4, #0
	mov r10, r4
	movs r7, #63
	add r5, r9
	movs r6, #104
.L_080e0626:
	bl Random16
	ands r0, r7
	str r0, [r5]
	movs r0, #1
	add r10, r0
	mov r1, r10
	str r6, [r5, #4]
	adds r5, #28
	cmp r1, #32
	bne .L_080e0626
	movs r2, #0
	mov r10, r2
	movs r1, #1
	movs r2, #128
	ldr r3, .L_080e089c
	negs r1, r1
	lsls r2, r2, #2
.L_080e064a:
	movs r4, #1
	add r10, r4
	str r1, [r3]
	adds r3, #28
	cmp r10, r2
	bne .L_080e064a
	movs r0, #141
	bl AudioCommand_PlayFar
	movs r0, #128
	movs r7, #0
	lsls r0, r0, #8
	str r7, [sp, #32]
	str r0, [sp, #16]
.L_080e0666:
	ldr r1, [sp, #32]
	cmp r1, #79
	bgt .L_080e06a4
	ldr r0, [sp, #16]
	bl Trig_Sin
	lsls r5, r0, #1
	adds r5, r5, r0
	ldr r0, [sp, #16]
	bl Trig_Cos
	ldr r3, [sp, #32]
	lsls r2, r3, #1
	movs r3, #64
	subs r3, r3, r2
	muls r3, r0
	lsls r5, r5, #3
	movs r2, #20
	asrs r5, r5, #16
	asrs r3, r3, #16
	adds r5, #22
	str r2, [sp, #0]
	movs r2, #38
	str r2, [sp, #4]
	adds r3, #29
	ldr r0, [sp, #36]
	mov r1, r9
	adds r2, r5, #0
	ldr r4, [sp, #28]
	bl _call_via_r4
.L_080e06a4:
	ldr r7, [sp, #32]
	cmp r7, #56
	bne .L_080e06b0
	movs r0, #133
	bl BattleEventRuntime_BeginPhaseFar
.L_080e06b0:
	movs r2, #225
	movs r0, #0
	lsls r2, r2, #7
	movs r1, #16
	add r2, r9
	str r0, [sp, #12]
	mov r10, r0
	mov r11, r1
	mov r8, r2
.L_080e06c2:
	ldr r3, [sp, #32]
	cmp r3, r11
	blt .L_080e07b6
	mov r4, r8
	movs r1, #34
	ldr r2, [r4]
	ldr r3, [r4, #4]
	str r1, [sp, #0]
	movs r1, #65
	str r1, [sp, #4]
	movs r1, #158
	lsls r1, r1, #4
	subs r2, #17
	subs r3, #32
	ldr r0, [sp, #36]
	add r1, r9
	ldr r7, [sp, #24]
	bl _call_via_r7
	ldr r0, [sp, #32]
	cmp r0, r11
	bne .L_080e07ae
	ldr r1, [sp, #12]
	ldr r2, .L_080e08a0
	movs r4, #0
	adds r7, r1, r2
.L_080e06f6:
	str r4, [sp, #8]
	bl Random16
	ldr r6, .L_080e08a4
	movs r3, #128
	lsls r3, r3, #7
	ands r6, r0
	adds r6, r6, r3
	bl Random16
	mov r1, r8
	ldr r3, [r1]
	lsls r3, r3, #16
	str r3, [r7]
	ldr r5, .L_080e08a8
	ldr r3, [r1, #4]
	ands r5, r0
	adds r3, #16
	movs r0, #128
	lsls r3, r3, #16
	lsls r0, r0, #1
	adds r5, r5, r0
	str r3, [r7, #4]
	adds r0, r6, #0
	bl Trig_Sin
	adds r3, r5, #0
	muls r3, r0
	asrs r3, r3, #7
	str r3, [r7, #12]
	adds r0, r6, #0
	bl Trig_Cos
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
	cmp r4, #16
	bne .L_080e06f6
	movs r3, #1
	mov r2, r10
	ands r3, r2
	cmp r3, #0
	beq .L_080e0766
	movs r0, #133
	bl AudioCommand_PlayFar
.L_080e0766:
	ldr r2, .L_080e08ac
	movs r3, #4
	add r2, r9
	str r3, [r2]
	ldr r3, .L_080e08b0
	mov r7, r9
	ldr r3, [r7, r3]
	ldr r3, [r3, #20]
	movs r4, #0
	cmp r3, #0
	beq .L_080e07ae
	ldr r5, .L_080e08b0
	movs r6, #36
	add r5, r9
.L_080e0782:
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
	bne .L_080e0782
.L_080e07ae:
	mov r4, r8
	ldr r3, [r4, #4]
	subs r3, #12
	str r3, [r4, #4]
.L_080e07b6:
	ldr r1, [sp, #12]
	movs r2, #224
	movs r3, #1
	lsls r2, r2, #2
	add r10, r3
	movs r7, #4
	movs r0, #28
	adds r1, r1, r2
	mov r4, r10
	add r11, r7
	add r8, r0
	str r1, [sp, #12]
	cmp r4, #10
	beq .L_080e07d4
	b .L_080e06c2
.L_080e07d4:
	movs r7, #0
	ldr r5, .L_080e08a0
	ldr r6, .L_080e08b4
	mov r10, r7
.L_080e07dc:
	movs r1, #1
	ldr r0, [r5, #24]
	negs r1, r1
	cmp r0, r1
	beq .L_080e082a
	cmp r0, #0
	bge .L_080e07ec
	adds r0, #15
.L_080e07ec:
	asrs r0, r0, #4
	adds r0, #2
	lsls r4, r0, #1
	subs r3, r4, #2
	ldrh r1, [r6, r3]
	ldr r2, [sp, #20]
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
	ldr r0, [sp, #36]
	ldr r4, [sp, #28]
	bl _call_via_r4
	movs r2, #128
	adds r0, r5, #0
	movs r1, #62
	lsls r2, r2, #6
	bl EffectStep_AdvanceWithGravity2D
	ldr r3, [r5, #24]
	subs r3, #1
	str r3, [r5, #24]
.L_080e082a:
	movs r7, #1
	movs r0, #128
	add r10, r7
	lsls r0, r0, #2
	adds r5, #28
	cmp r10, r0
	bne .L_080e07dc
	movs r1, #4
	movs r0, #4
	bl Camera_ApplyShake
	bl ObjectGroup_TickMemberTimers
	ldr r2, .L_080e08b8
	movs r3, #1
	add r2, r9
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r2, .L_080e08bc
	ldr r1, [sp, #16]
	ldr r3, [sp, #32]
	adds r1, r1, r2
	adds r3, #1
	str r1, [sp, #16]
	str r3, [sp, #32]
	cmp r3, #96
	beq .L_080e0866
	b .L_080e0666
.L_080e0866:
	ldr r0, .L_080e0898
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
	.2byte 0x0000
.L_080e0890:
	.4byte 0x0000006f
.L_080e0894:
	.4byte 0x00007784
.L_080e0898:
	.4byte BattlePresentation_ProcessPendingGraphicsTransfer
.L_080e089c:
	.4byte gMapCellBuffer + 0x18
.L_080e08a0:
	.4byte gMapCellBuffer
.L_080e08a4:
	.4byte 0x00007fff
.L_080e08a8:
	.4byte 0x000001ff
.L_080e08ac:
	.4byte 0x000077a8
.L_080e08b0:
	.4byte 0x00007828
.L_080e08b4:
	.4byte ParticleStreams_CellOffsets
.L_080e08b8:
	.4byte 0x00007824
.L_080e08bc:
	.4byte 0xfffff800
