.syntax unified
	.thumb
	.global Unnamed_080e2538
	.thumb_func
Unnamed_080e2538:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r5, .L_080e25a8
	adds r3, r5, #0
	ldmia r3!, {r1}
	sub sp, #56
	str r1, [sp, #28]
	ldr r2, .L_080e25ac
	ldr r3, [r3]
	adds r1, r1, r2
	str r3, [sp, #24]
	str r0, [r1]
	movs r0, #1
	mov r8, r1
	bl BattleFx_BeginCanvasLayer
	ldr r2, .L_080e25b0
	ldr r3, .L_080e25a0
	strh r3, [r2]
	ldr r3, .L_080e25a4
	adds r2, #48
	strh r3, [r2]
	ldr r1, [sp, #28]
	ldr r0, .L_080e25b4
	movs r2, #1
	movs r3, #1
	bl Resource_LoadAndDecompress
	movs r3, #1
	str r3, [sp, #0]
	movs r1, #7
	movs r2, #7
	mov r10, r3
	movs r0, #46
	movs r3, #3
	bl Unnamed_080ed408
	ldr r5, [r5, #28]
	str r5, [sp, #20]
	mov r4, r8
	ldr r3, [r4]
	add r6, sp, #44
	movs r1, #36
	ldrsh r0, [r3, r1]
	adds r1, r6, #0
	b .L_080e25b8
	.2byte 0x0000
.L_080e25a0:
	.4byte 0x00000100
.L_080e25a4:
	.4byte 0x00000000
.L_080e25a8:
	.4byte gBattleFxWork
.L_080e25ac:
	.4byte 0x00007828
.L_080e25b0:
	.4byte 0x04000020
.L_080e25b4:
	.4byte 0x0000008a
.L_080e25b8:
	bl EffectPosition_ApplyStepAndYOffset
	mov r3, r8
	ldr r2, [r3]
	ldr r3, [r2, #20]
	lsls r3, r3, #1
	add r5, sp, #32
	adds r3, #34
	ldrsh r0, [r2, r3]
	adds r1, r5, #0
	bl EffectPosition_ApplyStepAndYOffset
	ldr r1, [r6]
	ldr r3, [r5]
	subs r3, r3, r1
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	adds r1, r1, r3
	movs r3, #64
	ldr r2, .L_080e28b4
	subs r3, r3, r1
	lsls r3, r3, #8
	str r1, [r6]
	str r3, [r2]
	ldr r0, [sp, #28]
	movs r1, #239
	lsls r1, r1, #7
	adds r3, r0, r1
	mov r2, r10
	str r2, [r3]
	ldr r3, .L_080e28b8
	movs r1, #144
	adds r2, r0, r3
	movs r3, #0
	str r3, [r2]
	ldr r0, .L_080e28bc
	lsls r1, r1, #3
	bl Scheduler_AddOrUpdateCallback
	mov r4, r8
	ldr r3, [r4]
	ldr r2, .L_080e28c0
	ldr r3, [r3, #24]
	ldrb r2, [r2, r3]
	movs r0, #0
	str r2, [sp, #16]
	mov r9, r0
	cmp r2, #0
	beq .L_080e26b2
	ldr r4, [sp, #28]
	movs r0, #225
	lsls r0, r0, #7
	ldr r1, .L_080e28c4
	movs r2, #0
	adds r3, r4, r0
.L_080e2628:
	str r1, [r3, #4]
	str r2, [r3, #16]
	movs r4, #1
	ldr r0, [sp, #16]
	add r9, r4
	adds r3, #28
	cmp r9, r0
	bne .L_080e2628
	movs r1, #0
	mov r9, r1
	cmp r0, #0
	beq .L_080e26b2
	ldr r2, .L_080e28c8
	movs r7, #0
	mov r10, r2
.L_080e2646:
	movs r3, #0
	mov r8, r3
	movs r3, #140
	mov r4, r9
	muls r4, r3
	ldr r0, .L_080e28cc
	adds r3, r4, #0
	adds r3, r7, r3
	mov r6, r10
	adds r5, r3, r0
.L_080e265a:
	ldr r2, .L_080e28d0
	mov r1, r9
	ldrsb r2, [r2, r1]
	ldrb r3, [r6]
	adds r3, r3, r2
	lsls r3, r3, #16
	str r3, [r5]
	ldrb r3, [r6, #1]
	lsls r3, r3, #16
	str r3, [r5, #4]
	bl Random16
	movs r1, #96
	bl IwramUnsignedRemainderEntry
	subs r0, #48
	lsls r0, r0, #10
	str r0, [r5, #12]
	bl Random16
	movs r3, #127
	ands r3, r0
	adds r3, #32
	negs r3, r3
	lsls r3, r3, #11
	str r3, [r5, #16]
	movs r2, #1
	movs r3, #32
	str r3, [r5, #8]
	add r8, r2
	movs r3, #0
	str r3, [r5, #24]
	mov r3, r8
	adds r6, #2
	adds r5, #28
	cmp r3, #21
	bne .L_080e265a
	movs r4, #224
	ldr r0, [sp, #16]
	lsls r4, r4, #1
	add r9, r2
	adds r7, r7, r4
	cmp r9, r0
	bne .L_080e2646
.L_080e26b2:
	ldr r2, [sp, #16]
	movs r1, #0
	subs r2, #1
	mov r11, r1
	str r2, [sp, #12]
	ldr r1, .L_080e28d4
	movs r4, #80
	ldrb r3, [r1, r2]
	negs r4, r4
	cmp r3, r4
	bne .L_080e26ca
	b .L_080e2890
.L_080e26ca:
	ldrb r3, [r1, r2]
	adds r3, #48
	cmp r11, r3
	bne .L_080e26d8
	movs r0, #132
	bl BattleEventRuntime_BeginPhaseFar
.L_080e26d8:
	ldr r1, [sp, #16]
	movs r0, #0
	mov r9, r0
	cmp r1, #0
	bne .L_080e26e4
	b .L_080e2862
.L_080e26e4:
	ldr r2, [sp, #28]
	movs r3, #225
	lsls r3, r3, #7
	adds r2, r2, r3
	mov r10, r2
.L_080e26ee:
	ldr r5, .L_080e28d4
	mov r4, r9
	ldrb r3, [r5, r4]
	adds r2, r3, #0
	adds r2, #18
	cmp r11, r2
	bne .L_080e2710
	movs r0, #134
	bl Func_080f9010
	ldr r1, .L_080e28d8
	ldr r0, [sp, #28]
	movs r3, #4
	adds r2, r0, r1
	str r3, [r2]
	mov r2, r9
	ldrb r3, [r5, r2]
.L_080e2710:
	adds r3, #18
	cmp r11, r3
	blt .L_080e27c0
	mov r4, r9
	lsls r4, r4, #2
	movs r3, #0
	str r4, [sp, #8]
	mov r8, r3
.L_080e2720:
	ldr r3, [sp, #8]
	add r3, r9
	lsls r3, r3, #2
	add r3, r9
	add r3, r8
	lsls r2, r3, #3
	subs r2, r2, r3
	ldr r0, .L_080e28cc
	lsls r2, r2, #2
	adds r7, r2, r0
	movs r1, #5
	mov r0, r8
	bl Func_080022fc
	lsls r5, r0, #1
	adds r5, r5, r0
	movs r1, #96
	ldr r0, [r7, #24]
	bl FixedPoint_Ratio
	movs r1, #3
	bl Func_080022fc
	ldr r2, .L_080e28dc
	adds r5, r5, r0
	lsls r3, r5, #1
	ldrh r1, [r2, r3]
	ldr r2, [sp, #28]
	ldr r3, .L_080e28e0
	adds r1, r2, r1
	adds r1, r1, r3
	ldr r3, .L_080e28e4
	ldrb r6, [r3, r5]
	movs r4, #2
	ldrsh r2, [r7, r4]
	lsrs r3, r6, #1
	subs r2, r2, r3
	movs r0, #6
	ldrsh r3, [r7, r0]
	ldr r0, .L_080e28e8
	ldrb r4, [r0, r5]
	lsrs r0, r4, #1
	subs r3, r3, r0
	str r4, [sp, #4]
	str r6, [sp, #0]
	ldr r0, [sp, #24]
	ldr r4, [sp, #20]
	bl _call_via_r4
	movs r2, #128
	lsls r2, r2, #7
	adds r0, r7, #0
	movs r1, #64
	bl EffectStep_AdvanceWithGravity2D
	ldr r3, [r7, #24]
	ldr r2, [r7, #8]
	adds r3, r3, r2
	str r3, [r7, #24]
	cmp r2, #1
	ble .L_080e27a8
	movs r3, #1
	mov r0, r11
	ands r3, r0
	cmp r3, #0
	beq .L_080e27a8
	subs r3, r2, #1
	str r3, [r7, #8]
.L_080e27a8:
	movs r1, #1
	add r8, r1
	mov r2, r8
	cmp r2, #21
	bne .L_080e2720
	ldr r5, .L_080e28d4
	mov r4, r9
	ldrb r3, [r5, r4]
	adds r3, #18
	cmp r11, r3
	bge .L_080e2812
	b .L_080e27c2
.L_080e27c0:
	ldr r5, .L_080e28d4
.L_080e27c2:
	mov r0, r9
	ldrb r3, [r5, r0]
	cmp r11, r3
	blt .L_080e27e8
	ldr r3, .L_080e28d0
	mov r4, r10
	ldrsb r2, [r3, r0]
	movs r1, #6
	ldrsh r3, [r4, r1]
	movs r1, #34
	str r1, [sp, #0]
	movs r1, #62
	str r1, [sp, #4]
	adds r2, #47
	ldr r0, [sp, #24]
	ldr r1, [sp, #28]
	ldr r4, [sp, #20]
	bl _call_via_r4
.L_080e27e8:
	mov r0, r10
	ldr r3, [r0, #4]
	ldr r2, [r0, #16]
	mov r1, r9
	adds r3, r3, r2
	str r3, [r0, #4]
	ldrb r3, [r5, r1]
	cmp r11, r3
	ble .L_080e2802
	movs r4, #128
	lsls r4, r4, #9
	adds r3, r2, r4
	str r3, [r0, #16]
.L_080e2802:
	mov r0, r10
	movs r2, #200
	ldr r3, [r0, #4]
	lsls r2, r2, #14
	cmp r3, r2
	ble .L_080e2810
	str r2, [r0, #4]
.L_080e2810:
	ldr r5, .L_080e28d4
.L_080e2812:
	mov r1, r9
	ldrb r3, [r5, r1]
	adds r3, #18
	cmp r11, r3
	bne .L_080e2852
	ldr r3, .L_080e28ec
	ldr r4, [sp, #28]
	ldr r3, [r4, r3]
	ldr r3, [r3, #20]
	movs r2, #0
	mov r8, r2
	cmp r3, #0
	beq .L_080e2852
	ldr r0, .L_080e28ec
	movs r6, #36
	adds r5, r4, r0
.L_080e2832:
	ldr r3, [r5]
	ldrsh r0, [r3, r6]
	movs r3, #8
	str r3, [sp, #0]
	movs r2, #5
	mov r3, r8
	movs r1, #7
	bl ObjectGroup_UpdateMembers
	ldr r3, [r5]
	movs r2, #1
	ldr r3, [r3, #20]
	add r8, r2
	adds r6, #2
	cmp r8, r3
	bne .L_080e2832
.L_080e2852:
	movs r4, #1
	ldr r0, [sp, #16]
	movs r3, #28
	add r9, r4
	add r10, r3
	cmp r9, r0
	beq .L_080e2862
	b .L_080e26ee
.L_080e2862:
	movs r0, #2
	movs r1, #4
	bl Camera_ApplyShake
	bl ObjectGroup_TickMemberTimers
	ldr r3, .L_080e28f0
	ldr r1, [sp, #28]
	adds r2, r1, r3
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r1, .L_080e28d4
	ldr r2, [sp, #12]
	ldrb r3, [r1, r2]
	movs r4, #1
	add r11, r4
	adds r3, #80
	cmp r11, r3
	beq .L_080e2890
	b .L_080e26ca
.L_080e2890:
	ldr r0, .L_080e28bc
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
.L_080e28b4:
	.4byte 0x04000028
.L_080e28b8:
	.4byte 0x00007784
.L_080e28bc:
	.4byte BattlePresentation_ProcessPendingGraphicsTransfer
.L_080e28c0:
	.4byte Data_080eecfc
.L_080e28c4:
	.4byte 0xffc00000
.L_080e28c8:
	.4byte Data_080eecb2
.L_080e28cc:
	.4byte gMapCellBuffer
.L_080e28d0:
	.4byte Data_080eecf2
.L_080e28d4:
	.4byte Data_080eecf7
.L_080e28d8:
	.4byte 0x000077a8
.L_080e28dc:
	.4byte Data_080eed1e
.L_080e28e0:
	.4byte 0x0000083c
.L_080e28e4:
	.4byte Data_080eecff
.L_080e28e8:
	.4byte Data_080eed0e
.L_080e28ec:
	.4byte 0x00007828
.L_080e28f0:
	.4byte 0x00007824
