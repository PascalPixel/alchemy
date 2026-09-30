.syntax unified
	.thumb
	.global Unnamed_080e94b8
	.thumb_func
Unnamed_080e94b8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r2, .L_080e9538
	adds r3, r2, #0
	adds r5, r0, #0
	ldmia r3!, {r0}
	ldr r3, [r3]
	sub sp, #52
	str r3, [sp, #40]
	ldr r2, [r2, #8]
	str r2, [sp, #32]
	mov r11, r0
	ldr r0, [r5, #8]
	bl GetBattleObjectSlotFar
	ldr r6, .L_080e953c
	ldr r0, [r0]
	movs r3, #1
	add r6, r11
	str r0, [sp, #28]
	str r3, [r5, #24]
	movs r0, #1
	str r5, [r6]
	bl BattleFx_BeginCanvasLayer
	ldr r2, .L_080e9540
	ldr r3, .L_080e9534
	strh r3, [r2]
	ldr r3, [r6]
	mov r1, sp
	ldr r0, [r3, #4]
	adds r1, #44
	str r1, [sp, #24]
	bl BattleFx_FetchRectangleBlitters
	movs r1, #2
	ldr r0, [sp, #28]
	bl Object_SetMode
	movs r1, #48
	ldr r0, [sp, #28]
	bl ObjectDispatch_ApplyValueToChildrenFar
	ldr r0, .L_080e9544
	mov r1, r11
	movs r2, #1
	movs r3, #1
	bl Resource_LoadAndDecompress
	movs r1, #128
	lsls r1, r1, #6
	ldr r0, .L_080e9548
	add r1, r11
	movs r2, #1
	movs r3, #0
	b .L_080e954c
	.2byte 0x0000
.L_080e9534:
	.4byte 0x00001010
.L_080e9538:
	.4byte gBattleFxWork
.L_080e953c:
	.4byte 0x00007828
.L_080e9540:
	.4byte 0x04000052
.L_080e9544:
	.4byte 0x00000055
.L_080e9548:
	.4byte 0x0000007d
.L_080e954c:
	bl Resource_LoadAndDecompress
	movs r2, #0
	ldr r0, .L_080e985c
	ldr r1, [sp, #32]
	movs r3, #0
	bl Resource_LoadAndDecompress
	movs r2, #0
	str r2, [sp, #36]
	mov r9, r2
	mov r8, r11
.L_080e9564:
	movs r7, #225
	movs r3, #0
	lsls r7, r7, #7
	mov r10, r3
	add r7, r8
.L_080e956e:
	mov r0, r10
	lsls r6, r0, #1
	bl Random16
	ldr r3, .L_080e9860
	adds r5, r0, #0
	ands r5, r3
	adds r0, r5, #0
	bl Trig_Sin
	adds r3, r6, #0
	muls r3, r0
	adds r0, r5, #0
	str r3, [r7]
	bl Trig_Cos
	adds r3, r6, #0
	muls r3, r0
	mov r1, r10
	negs r3, r3
	str r3, [r7, #4]
	lsrs r3, r1, #31
	add r3, r10
	asrs r3, r3, #1
	movs r2, #1
	adds r3, #25
	add r10, r2
	str r3, [r7, #24]
	mov r3, r10
	adds r7, #28
	cmp r3, #16
	bne .L_080e956e
	mov r1, r9
	lsls r3, r1, #3
	subs r3, r3, r1
	ldr r2, .L_080e9864
	movs r0, #0
	lsls r3, r3, #2
	mov r10, r0
	adds r7, r3, r2
.L_080e95be:
	bl Random16
	ldr r5, .L_080e9868
	ands r5, r0
	bl Random16
	ldr r3, .L_080e9860
	adds r6, r0, #0
	ands r6, r3
	ldr r3, .L_080e986c
	add r3, r11
	ldr r3, [r3]
	ldr r2, [r3, #4]
	ldr r0, [sp, #36]
	lsls r3, r2, #1
	adds r3, r3, r2
	ldr r1, .L_080e9870
	adds r3, r0, r3
	ldrb r3, [r1, r3]
	lsls r3, r3, #16
	str r3, [r7]
	movs r3, #176
	lsls r3, r3, #15
	str r3, [r7, #4]
	adds r0, r6, #0
	bl Trig_Sin
	adds r5, #32
	adds r3, r5, #0
	muls r3, r0
	asrs r3, r3, #6
	str r3, [r7, #12]
	adds r0, r6, #0
	bl Trig_Cos
	adds r3, r5, #0
	muls r3, r0
	lsls r3, r3, #1
	negs r3, r3
	asrs r3, r3, #6
	str r3, [r7, #16]
	bl Random16
	movs r3, #7
	ands r3, r0
	adds r3, #32
	str r3, [r7, #24]
	movs r2, #1
	movs r3, #170
	add r10, r2
	lsls r3, r3, #1
	adds r7, #28
	cmp r10, r3
	bne .L_080e95be
	ldr r1, [sp, #36]
	movs r0, #224
	lsls r0, r0, #1
	adds r1, #1
	add r9, r3
	add r8, r0
	str r1, [sp, #36]
	cmp r1, #3
	bne .L_080e9564
	movs r2, #239
	lsls r2, r2, #7
	add r2, r11
	movs r3, #2
	str r3, [r2]
	ldr r2, .L_080e9874
	movs r3, #75
	add r2, r11
	movs r1, #144
	str r3, [r2]
	ldr r0, .L_080e9878
	lsls r1, r1, #3
	bl Scheduler_AddOrUpdateCallback
	movs r2, #0
	mov r9, r2
.L_080e965c:
	mov r3, r9
	cmp r3, #4
	bne .L_080e9668
	movs r0, #212
	bl AudioCommand_PlayFar
.L_080e9668:
	mov r0, r9
	cmp r0, #8
	bne .L_080e9674
	ldr r3, .L_080e987c
	add r3, r11
	str r0, [r3]
.L_080e9674:
	mov r1, r9
	cmp r1, #18
	bne .L_080e9680
	movs r0, #145
	bl AudioCommand_PlayFar
.L_080e9680:
	mov r2, r9
	cmp r2, #40
	bne .L_080e968c
	movs r0, #134
	bl BattleEventRuntime_BeginPhaseFar
.L_080e968c:
	mov r3, r9
	cmp r3, #39
	bgt .L_080e9724
	ldr r3, .L_080e986c
	add r3, r11
	ldr r3, [r3]
	ldr r3, [r3, #4]
	movs r1, #128
	cmp r3, #1
	bne .L_080e96ce
	mov r0, r9
	cmp r0, #9
	bgt .L_080e96b8
	lsls r3, r0, #2
	add r3, r9
	lsls r3, r3, #1
	adds r2, r3, #0
	lsls r3, r0, #4
	adds r5, r3, #0
	subs r2, #8
	subs r5, #128
	b .L_080e96fe
.L_080e96b8:
	mov r2, r9
	cmp r2, #20
	ble .L_080e96ca
	mov r0, r9
	lsls r3, r0, #1
	adds r5, r3, #0
	adds r2, #62
	subs r5, #24
	b .L_080e96fe
.L_080e96ca:
	movs r2, #82
	b .L_080e96fc
.L_080e96ce:
	mov r2, r9
	cmp r2, #9
	bgt .L_080e96e6
	lsls r3, r2, #2
	add r3, r9
	lsls r3, r3, #1
	mov r0, r9
	subs r2, r1, r3
	lsls r3, r0, #4
	adds r5, r3, #0
	subs r5, #128
	b .L_080e96fe
.L_080e96e6:
	mov r2, r9
	cmp r2, #20
	ble .L_080e96fa
	movs r3, #58
	mov r0, r9
	subs r2, r3, r2
	lsls r3, r0, #1
	adds r5, r3, #0
	subs r5, #24
	b .L_080e96fe
.L_080e96fa:
	movs r2, #38
.L_080e96fc:
	movs r5, #16
.L_080e96fe:
	adds r3, r5, #0
	adds r3, #128
	cmp r3, #104
	ble .L_080e970c
	subs r3, r1, r5
	adds r1, r3, #0
	subs r1, #24
.L_080e970c:
	cmp r1, #0
	ble .L_080e9724
	movs r3, #64
	str r3, [sp, #0]
	str r1, [sp, #4]
	subs r2, #32
	ldr r4, [sp, #44]
	ldr r0, [sp, #40]
	mov r1, r11
	adds r3, r5, #0
	bl _call_via_r4
.L_080e9724:
	mov r1, r9
	cmp r1, #16
	ble .L_080e9730
	ldr r0, .L_080e9880
	bl BattleFx_StepPaletteToResource
.L_080e9730:
	movs r2, #0
	movs r3, #22
	movs r0, #16
	mov r1, r11
	str r2, [sp, #36]
	str r3, [sp, #20]
	str r2, [sp, #16]
	str r0, [sp, #12]
	str r1, [sp, #8]
.L_080e9742:
	ldr r2, [sp, #36]
	ldr r3, [sp, #12]
	lsls r1, r2, #3
	cmp r9, r3
	bne .L_080e9754
	ldr r2, .L_080e987c
	movs r3, #12
	add r2, r11
	str r3, [r2]
.L_080e9754:
	ldr r0, [sp, #12]
	cmp r9, r0
	blt .L_080e980c
	adds r3, r1, #0
	adds r3, #18
	cmp r9, r3
	bge .L_080e9790
	ldr r3, .L_080e986c
	add r3, r11
	ldr r3, [r3]
	ldr r2, [r3, #4]
	lsls r3, r2, #1
	adds r3, r3, r2
	ldr r2, [sp, #36]
	ldr r1, .L_080e9870
	adds r3, r2, r3
	ldrb r2, [r1, r3]
	movs r3, #32
	movs r1, #128
	str r3, [sp, #0]
	lsls r1, r1, #6
	movs r3, #64
	str r3, [sp, #4]
	subs r2, #16
	ldr r4, [sp, #44]
	ldr r0, [sp, #40]
	add r1, r11
	movs r3, #56
	bl _call_via_r4
.L_080e9790:
	ldr r0, .L_080e9884
	ldr r1, [sp, #8]
	movs r2, #225
	movs r3, #0
	lsls r2, r2, #7
	mov r10, r3
	mov r8, r0
	adds r5, r1, r2
.L_080e97a0:
	movs r3, #6
	ldrsh r7, [r5, r3]
	ldr r3, .L_080e986c
	add r3, r11
	ldr r3, [r3]
	ldr r2, [r3, #4]
	lsls r3, r2, #1
	adds r3, r3, r2
	ldr r2, [sp, #36]
	movs r0, #2
	ldrsh r1, [r5, r0]
	ldr r0, .L_080e9870
	adds r3, r2, r3
	ldrb r3, [r0, r3]
	ldr r0, [r5, #24]
	adds r6, r1, r3
	cmp r0, #17
	bhi .L_080e97f2
	movs r1, #3
	bl FixedPoint_Ratio
	mov r2, r8
	ldrb r1, [r2, r0]
	movs r3, #128
	lsls r1, r1, #11
	lsls r3, r3, #6
	movs r0, #32
	add r1, r11
	adds r1, r1, r3
	str r0, [sp, #0]
	adds r2, r6, #0
	movs r0, #64
	adds r3, r7, #0
	str r0, [sp, #4]
	subs r2, #16
	adds r3, #56
	ldr r4, [sp, #44]
	ldr r0, [sp, #40]
	bl _call_via_r4
	ldr r0, [r5, #24]
.L_080e97f2:
	cmp r0, #0
	ble .L_080e97fa
	subs r3, r0, #1
	b .L_080e97fe
.L_080e97fa:
	movs r3, #1
	negs r3, r3
.L_080e97fe:
	str r3, [r5, #24]
	movs r0, #1
	add r10, r0
	mov r1, r10
	adds r5, #28
	cmp r1, #12
	bne .L_080e97a0
.L_080e980c:
	ldr r3, [sp, #12]
	adds r3, #5
	cmp r9, r3
	ble .L_080e98e2
	movs r2, #0
	ldr r7, [sp, #16]
	mov r10, r2
.L_080e981a:
	lsls r3, r7, #4
	adds r3, r7, r3
	lsls r3, r3, #2
	add r3, r10
	lsls r2, r3, #3
	subs r2, r2, r3
	ldr r3, .L_080e9864
	lsls r2, r2, #2
	adds r6, r2, r3
	ldr r3, [r6, #24]
	cmp r3, #0
	ble .L_080e98d6
	movs r2, #128
	adds r0, r6, #0
	movs r1, #64
	lsls r2, r2, #5
	bl EffectStep_AdvanceWithGravity2D
	ldr r3, [r6, #24]
	movs r0, #216
	ldr r1, [r6, #4]
	subs r3, #1
	lsls r0, r0, #15
	str r3, [r6, #24]
	cmp r1, r0
	ble .L_080e9888
	ldr r3, [r6, #16]
	negs r3, r3
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	str r3, [r6, #16]
	b .L_080e98d6
.L_080e985c:
	.4byte 0x00000073
.L_080e9860:
	.4byte 0x0000ffff
.L_080e9864:
	.4byte gMapCellBuffer
.L_080e9868:
	.4byte 0x000001ff
.L_080e986c:
	.4byte 0x00007828
.L_080e9870:
	.4byte Data_080eeef8 + 0xe
.L_080e9874:
	.4byte 0x00007784
.L_080e9878:
	.4byte BattlePresentation_ProcessPendingGraphicsTransfer
.L_080e987c:
	.4byte 0x000077a8
.L_080e9880:
	.4byte 0x000000c0
.L_080e9884:
	.4byte Data_080eeef8 + 0x14
.L_080e9888:
	ldr r0, [r6]
	ldr r2, .L_080e99ac
	cmp r0, r2
	bhi .L_080e98d6
	cmp r1, #0
	blt .L_080e98d6
	asrs r1, r1, #16
	mov r8, r1
	asrs r6, r0, #16
	movs r1, #5
	adds r0, r3, #0
	bl FixedPoint_Ratio
	adds r0, #1
	lsls r5, r0, #1
	mov r3, r10
	ldr r2, .L_080e99b0
	movs r4, #1
	ands r4, r3
	subs r3, r5, #2
	ldrh r1, [r2, r3]
	lsrs r3, r0, #31
	adds r3, r0, r3
	asrs r3, r3, #1
	subs r6, r6, r3
	mov r3, r8
	ldr r2, [sp, #32]
	subs r3, r3, r0
	str r0, [sp, #0]
	str r5, [sp, #4]
	ldr r0, [sp, #24]
	lsls r4, r4, #2
	adds r1, r2, r1
	ldr r4, [r4, r0]
	adds r2, r6, #0
	ldr r0, [sp, #40]
	mov r8, r3
	bl _call_via_r4
.L_080e98d6:
	movs r1, #1
	movs r2, #128
	add r10, r1
	lsls r2, r2, #1
	cmp r10, r2
	bne .L_080e981a
.L_080e98e2:
	ldr r2, .L_080e99b4
	movs r3, #0
	mov r0, r11
	mov r10, r3
	ldr r3, [r0, r2]
	ldr r3, [r3, #20]
	cmp r3, #0
	beq .L_080e992e
	ldr r1, [sp, #20]
	movs r6, #36
	mov r8, r1
.L_080e98f8:
	cmp r9, r8
	bne .L_080e991c
	mov r3, r11
	adds r5, r3, r2
	ldr r3, [r5]
	ldrsh r0, [r3, r6]
	movs r3, #10
	str r3, [sp, #0]
	movs r1, #7
	movs r2, #5
	mov r3, r10
	bl ObjectGroup_UpdateMembers
	ldr r3, [r5]
	movs r1, #4
	ldrsh r0, [r3, r6]
	bl BattleMotion_ApplyVariantMotionFar
.L_080e991c:
	ldr r2, .L_080e99b4
	movs r3, #1
	mov r0, r11
	add r10, r3
	ldr r3, [r0, r2]
	ldr r3, [r3, #20]
	adds r6, #2
	cmp r10, r3
	bne .L_080e98f8
.L_080e992e:
	ldr r1, [sp, #20]
	ldr r2, [sp, #16]
	adds r1, #8
	adds r2, #5
	ldr r3, [sp, #12]
	ldr r0, [sp, #8]
	str r1, [sp, #20]
	str r2, [sp, #16]
	movs r1, #224
	ldr r2, [sp, #36]
	lsls r1, r1, #1
	adds r3, #8
	adds r0, r0, r1
	adds r2, #1
	str r3, [sp, #12]
	str r0, [sp, #8]
	str r2, [sp, #36]
	cmp r2, #2
	beq .L_080e9956
	b .L_080e9742
.L_080e9956:
	movs r0, #16
	movs r1, #16
	bl Camera_ApplyShake
	bl ObjectGroup_TickMemberTimers
	ldr r3, .L_080e99b8
	movs r5, #1
	add r3, r11
	str r5, [r3]
	movs r0, #1
	bl WaitFrames
	movs r3, #1
	add r9, r3
	mov r0, r9
	cmp r0, #80
	beq .L_080e997c
	b .L_080e965c
.L_080e997c:
	ldr r0, [sp, #28]
	movs r1, #16
	bl ObjectDispatch_ApplyValueToChildrenFar
	ldr r0, .L_080e99bc
	bl Scheduler_RemoveCallback
	movs r0, #47
	bl Runtime_ReleaseHeapBlock
	movs r0, #46
	bl Runtime_ReleaseHeapBlock
	bl BattleFx_EndCanvasLayer
	add sp, #52
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
.L_080e99ac:
	.4byte 0x007effff
.L_080e99b0:
	.4byte ParticleStreams_CellOffsets
.L_080e99b4:
	.4byte 0x00007828
.L_080e99b8:
	.4byte 0x00007824
.L_080e99bc:
	.4byte BattlePresentation_ProcessPendingGraphicsTransfer
