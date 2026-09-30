.syntax unified
	.thumb
	.global Unnamed_080e698c
	.thumb_func
Unnamed_080e698c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r2, .L_080e69fc
	adds r3, r2, #0
	ldmia r3!, {r1}
	ldr r6, .L_080e6a00
	ldr r3, [r3]
	sub sp, #36
	mov r9, r1
	str r3, [sp, #12]
	add r6, r9
	str r0, [r6]
	movs r0, #0
	ldr r5, [r2, #8]
	bl BattleFx_BeginCanvasLayer
	ldr r2, .L_080e6a04
	ldr r3, .L_080e69f8
	ldr r0, .L_080e6a08
	strh r3, [r2]
	adds r1, r5, #0
	movs r2, #0
	movs r3, #0
	bl Resource_LoadAndDecompress
	ldr r0, .L_080e6a0c
	mov r1, r9
	movs r2, #1
	movs r3, #1
	bl Resource_LoadAndDecompress
	movs r1, #250
	lsls r1, r1, #6
	movs r2, #1
	ldr r0, .L_080e6a10
	add r1, r9
	movs r3, #0
	bl Resource_LoadAndDecompress
	ldr r3, [r6]
	add r1, sp, #16
	ldr r0, [r3, #4]
	bl BattleFx_FetchRectangleBlitters
	movs r2, #239
	lsls r2, r2, #7
	add r2, r9
	b .L_080e6a14
	.2byte 0x0000
.L_080e69f8:
	.4byte 0x00000100
.L_080e69fc:
	.4byte gBattleFxWork
.L_080e6a00:
	.4byte 0x00007828
.L_080e6a04:
	.4byte 0x04000020
.L_080e6a08:
	.4byte 0x00000073
.L_080e6a0c:
	.4byte 0x00000061
.L_080e6a10:
	.4byte 0x0000006d
.L_080e6a14:
	movs r3, #2
	str r3, [r2]
	ldr r2, .L_080e6cf8
	movs r3, #50
	add r2, r9
	movs r1, #144
	str r3, [r2]
	lsls r1, r1, #3
	ldr r0, .L_080e6cfc
	bl Scheduler_AddOrUpdateCallback
	movs r2, #0
	mov r8, r2
	ldr r3, .L_080e6d00
	movs r2, #128
	movs r1, #0
	lsls r2, r2, #3
.L_080e6a36:
	movs r4, #1
	add r8, r4
	str r1, [r3]
	adds r3, #28
	cmp r8, r2
	bne .L_080e6a36
	ldr r5, .L_080e6d04
	add r5, r9
	ldr r3, [r5]
	ldr r0, [r3, #8]
	bl GetBattleObjectSlotFar
	ldr r3, [r5]
	ldr r6, [r0]
	movs r1, #36
	ldrsh r0, [r3, r1]
	bl GetBattleObjectSlotFar
	ldr r0, [r0]
	ldr r2, [r6, #8]
	ldr r3, [r0, #8]
	subs r3, r3, r2
	mov r8, r0
	lsls r0, r3, #2
	adds r0, r0, r3
	movs r1, #100
	lsls r0, r0, #4
	mov r10, r2
	bl FixedPoint_Ratio
	mov r4, r8
	ldr r3, [r4, #16]
	adds r5, r0, #0
	ldr r0, [r6, #16]
	subs r3, r3, r0
	mov r8, r0
	lsls r0, r3, #2
	adds r0, r0, r3
	movs r1, #100
	lsls r0, r0, #4
	bl FixedPoint_Ratio
	add r10, r5
	add r8, r0
	asrs r5, r5, #8
	asrs r0, r0, #8
	adds r2, r0, #0
	muls r2, r0
	adds r3, r5, #0
	muls r3, r5
	adds r3, r3, r2
	adds r0, r3, #0
	ldr r2, .L_080e6d08
	bl _call_via_r2
	movs r1, #20
	lsls r0, r0, #8
	bl FixedPoint_Ratio
	adds r3, r6, #0
	movs r2, #1
	adds r3, #88
	str r0, [r6, #52]
	str r0, [r6, #48]
	strb r2, [r3]
	movs r3, #224
	lsls r3, r3, #11
	str r3, [r6, #40]
	ldr r3, .L_080e6d0c
	str r3, [r6, #72]
	adds r3, r6, #0
	movs r1, #0
	adds r3, #90
	str r1, [r6, #68]
	adds r0, r6, #0
	strb r2, [r3]
	bl Object_ResetMotion
	mov r1, r10
	movs r2, #0
	mov r3, r8
	adds r0, r6, #0
	bl Object_SetMoveTargetFar
	movs r1, #2
	adds r0, r6, #0
	bl Object_SetMode
	ldr r3, .L_080e6d04
	movs r1, #0
	add r2, sp, #24
	add r3, r9
	str r1, [sp, #8]
	mov r11, r2
	mov r10, r3
.L_080e6af4:
	mov r4, r10
	ldr r3, [r4]
	mov r1, r11
	ldr r0, [r3, #8]
	bl EffectPosition_ApplyAlternateStepAndYOffset
	mov r6, r11
	ldr r2, [r6]
	movs r3, #80
	subs r3, r3, r2
	ldr r1, .L_080e6d10
	lsls r3, r3, #8
	str r3, [r1]
	ldr r2, [sp, #8]
	subs r2, #8
	cmp r2, #15
	bhi .L_080e6b8e
	lsrs r3, r2, #31
	adds r3, r2, r3
	asrs r5, r3, #1
	cmp r5, #6
	ble .L_080e6b22
	movs r5, #6
.L_080e6b22:
	mov r0, r10
	ldr r3, [r0]
	ldr r3, [r3, #4]
	cmp r3, #0
	bne .L_080e6b5c
	ldr r2, .L_080e6d14
	lsls r3, r5, #1
	ldrh r1, [r2, r3]
	ldr r3, .L_080e6d18
	ldrsb r2, [r3, r5]
	ldr r3, .L_080e6d1c
	mov r4, r11
	ldr r0, [r4, #4]
	ldrsb r3, [r3, r5]
	adds r3, r3, r0
	ldr r0, .L_080e6d20
	ldrb r0, [r0, r5]
	str r0, [sp, #0]
	ldr r0, .L_080e6d24
	ldrb r0, [r0, r5]
	add r1, r9
	str r0, [sp, #4]
	adds r2, #30
	subs r3, #60
	ldr r4, [sp, #16]
	ldr r0, [sp, #12]
	bl _call_via_r4
	b .L_080e6b8e
.L_080e6b5c:
	ldr r2, .L_080e6d14
	lsls r3, r5, #1
	ldrh r1, [r2, r3]
	ldr r3, .L_080e6d18
	ldrsb r2, [r3, r5]
	ldr r3, .L_080e6d20
	ldrb r4, [r3, r5]
	ldr r3, .L_080e6d1c
	mov r6, r11
	ldr r0, [r6, #4]
	ldrsb r3, [r3, r5]
	str r4, [sp, #0]
	adds r3, r3, r0
	ldr r0, .L_080e6d24
	negs r2, r2
	ldrb r0, [r0, r5]
	subs r2, r2, r4
	str r0, [sp, #4]
	add r1, r9
	adds r2, #108
	subs r3, #60
	ldr r4, [sp, #16]
	ldr r0, [sp, #12]
	bl _call_via_r4
.L_080e6b8e:
	ldr r0, [sp, #8]
	cmp r0, #18
	bne .L_080e6c26
	movs r0, #134
	bl BattleEventRuntime_BeginPhaseFar
	mov r1, r10
	ldr r3, [r1]
	movs r2, #36
	ldrsh r0, [r3, r2]
	movs r3, #8
	str r3, [sp, #0]
	movs r2, #5
	movs r1, #7
	movs r3, #0
	bl ObjectGroup_UpdateMembers
	mov r4, r10
	ldr r3, [r4]
	movs r1, #6
	movs r6, #36
	ldrsh r0, [r3, r6]
	bl BattleMotion_ApplyVariantMotionFar
	ldr r2, .L_080e6d28
	movs r3, #4
	add r2, r9
	movs r0, #0
	str r3, [r2]
	ldr r7, .L_080e6d2c
	mov r8, r0
.L_080e6bcc:
	bl Random16
	movs r5, #63
	movs r1, #128
	lsls r1, r1, #1
	ands r5, r0
	adds r5, r5, r1
	bl Random16
	ldr r3, .L_080e6d30
	adds r6, r0, #0
	ands r6, r3
	movs r3, #128
	lsls r3, r3, #15
	str r3, [r7]
	movs r3, #160
	lsls r3, r3, #15
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
	negs r3, r3
	asrs r3, r3, #6
	str r3, [r7, #16]
	bl Random16
	movs r3, #15
	ands r3, r0
	movs r2, #1
	adds r3, #16
	add r8, r2
	str r3, [r7, #24]
	mov r3, r8
	adds r7, #28
	cmp r3, #16
	bne .L_080e6bcc
.L_080e6c26:
	movs r4, #0
	ldr r5, .L_080e6d2c
	mov r8, r4
.L_080e6c2c:
	ldr r3, [r5, #24]
	cmp r3, #0
	ble .L_080e6c9c
	subs r3, #1
	str r3, [r5, #24]
	adds r0, r5, #0
	movs r1, #60
	movs r2, #0
	bl EffectStep_AdvanceWithGravity2D
	movs r6, #208
	ldr r3, [r5, #4]
	lsls r6, r6, #15
	cmp r3, r6
	ble .L_080e6c58
	ldr r3, [r5, #16]
	negs r3, r3
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	str r3, [r5, #16]
	b .L_080e6c9c
.L_080e6c58:
	ldr r2, [r5]
	ldr r0, .L_080e6d34
	cmp r2, r0
	bhi .L_080e6c9c
	cmp r3, #0
	blt .L_080e6c9c
	ldr r0, [sp, #8]
	add r0, r8
	asrs r6, r2, #16
	asrs r7, r3, #16
	cmp r0, #0
	bge .L_080e6c72
	adds r0, #3
.L_080e6c72:
	movs r1, #6
	asrs r0, r0, #2
	bl Math_Mod
	adds r1, r0, #0
	lsls r1, r1, #8
	movs r2, #250
	lsls r2, r2, #6
	add r1, r9
	movs r0, #16
	adds r1, r1, r2
	adds r3, r7, #0
	adds r2, r6, #0
	str r0, [sp, #0]
	str r0, [sp, #4]
	subs r2, #8
	subs r3, #8
	ldr r4, [sp, #16]
	ldr r0, [sp, #12]
	bl _call_via_r4
.L_080e6c9c:
	movs r3, #1
	add r8, r3
	mov r4, r8
	adds r5, #28
	cmp r4, #128
	bne .L_080e6c2c
	movs r0, #8
	movs r1, #8
	bl Camera_ApplyShake
	bl ObjectGroup_TickMemberTimers
	ldr r2, .L_080e6d38
	movs r3, #1
	add r2, r9
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r6, [sp, #8]
	adds r6, #1
	str r6, [sp, #8]
	cmp r6, #70
	beq .L_080e6cce
	b .L_080e6af4
.L_080e6cce:
	ldr r0, .L_080e6cfc
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
	.2byte 0x0000
.L_080e6cf8:
	.4byte 0x00007784
.L_080e6cfc:
	.4byte BattlePresentation_ProcessPendingGraphicsTransfer
.L_080e6d00:
	.4byte gMapCellBuffer + 0x18
.L_080e6d04:
	.4byte 0x00007828
.L_080e6d08:
	.4byte IwramSqrt
.L_080e6d0c:
	.4byte 0x0000deb8
.L_080e6d10:
	.4byte 0x04000028
.L_080e6d14:
	.4byte Data_080eee02
.L_080e6d18:
	.4byte Data_080eee10
.L_080e6d1c:
	.4byte Data_080eee17
.L_080e6d20:
	.4byte Data_080eedf4
.L_080e6d24:
	.4byte Data_080eedfb
.L_080e6d28:
	.4byte 0x000077a8
.L_080e6d2c:
	.4byte gMapCellBuffer
.L_080e6d30:
	.4byte 0x0000ffff
.L_080e6d34:
	.4byte 0x007effff
.L_080e6d38:
	.4byte 0x00007824
