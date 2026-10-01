.syntax unified
	.thumb
	.global Region_080ddde0
	.thumb_func
Region_080ddde0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r5, .L_080de08c
	adds r3, r5, #0
	ldmia r3!, {r1}
	ldr r3, [r3]
	sub sp, #64
	str r3, [sp, #48]
	ldr r3, .L_080de090
	mov r11, r1
	ldr r2, [r5, #8]
	add r3, r11
	str r2, [sp, #28]
	str r0, [r3]
	movs r0, #1
	bl BattleFx_BeginCanvasLayer
	movs r3, #2
	str r3, [sp, #0]
	movs r1, #7
	movs r2, #7
	movs r3, #3
	movs r0, #46
	bl Unnamed_080ed408
	ldr r5, [r5, #28]
	ldr r0, .L_080de094
	mov r1, r11
	movs r2, #1
	movs r3, #0
	str r5, [sp, #32]
	bl Resource_LoadAndDecompress
	ldr r1, .L_080de098
	ldr r0, .L_080de09c
	add r1, r11
	movs r2, #1
	movs r3, #1
	bl Resource_LoadAndDecompress
	movs r2, #0
	ldr r1, [sp, #28]
	movs r3, #0
	ldr r0, .L_080de0a0
	bl Resource_LoadAndDecompress
	movs r3, #0
	mov r10, r3
	movs r2, #128
	ldr r3, .L_080de0a4
	movs r1, #0
	lsls r2, r2, #3
.L_080dde52:
	movs r4, #1
	add r10, r4
	str r1, [r3]
	adds r3, #28
	cmp r10, r2
	bne .L_080dde52
	ldr r3, .L_080de0a8
	movs r0, #0
	movs r2, #1
	mov r10, r0
	negs r2, r2
	add r3, r11
.L_080dde6a:
	movs r1, #1
	add r10, r1
	mov r4, r10
	str r2, [r3]
	adds r3, #28
	cmp r4, #64
	bne .L_080dde6a
	movs r2, #239
	lsls r2, r2, #7
	add r2, r11
	movs r3, #2
	str r3, [r2]
	ldr r2, .L_080de0ac
	movs r3, #75
	add r2, r11
	movs r1, #144
	str r3, [r2]
	lsls r1, r1, #3
	ldr r0, .L_080de0b0
	bl Scheduler_AddOrUpdateCallback
	movs r0, #138
	bl AudioCommand_PlayFar
	movs r0, #0
	ldr r3, .L_080de090
	str r0, [sp, #40]
	add r3, r11
	ldr r3, [r3]
	ldr r3, [r3, #20]
	movs r1, #40
	lsls r3, r3, #3
	negs r1, r1
	cmp r3, r1
	bne .L_080ddeb2
	b .L_080de2a0
.L_080ddeb2:
	ldr r2, .L_080de090
	add r2, r11
	str r2, [sp, #20]
.L_080ddeb8:
	ldr r3, [sp, #40]
	cmp r3, #24
	bne .L_080ddec4
	movs r0, #133
	bl BattleEventRuntime_BeginPhaseFar
.L_080ddec4:
	movs r4, #0
	str r4, [sp, #44]
	ldr r0, [sp, #20]
	ldr r3, [r0]
	ldr r3, [r3, #20]
	cmp r3, #0
	beq .L_080ddefc
	ldr r5, .L_080de0b4
.L_080dded4:
	ldr r1, [sp, #44]
	ldr r2, [sp, #40]
	lsls r3, r1, #3
	cmp r2, r3
	bne .L_080ddeea
	movs r1, #128
	ldr r0, [sp, #48]
	lsls r1, r1, #7
	ldr r2, .L_080de0b8
	bl _call_via_r5
.L_080ddeea:
	ldr r3, [sp, #44]
	adds r3, #1
	str r3, [sp, #44]
	ldr r4, [sp, #20]
	ldr r3, [r4]
	ldr r0, [sp, #44]
	ldr r3, [r3, #20]
	cmp r0, r3
	bne .L_080dded4
.L_080ddefc:
	movs r1, #0
	str r1, [sp, #44]
	ldr r2, .L_080de090
	mov r4, r11
	ldr r3, [r4, r2]
	ldr r3, [r3, #20]
	cmp r3, #0
	bne .L_080ddf0e
	b .L_080de1f0
.L_080ddf0e:
	mov r0, sp
	adds r0, #52
	movs r1, #36
	movs r3, #0
	str r0, [sp, #24]
	str r1, [sp, #16]
	str r3, [sp, #12]
.L_080ddf1c:
	mov r0, r11
	adds r5, r0, r2
	ldr r3, [r5]
	ldr r1, [sp, #16]
	ldr r4, [sp, #44]
	ldrsh r0, [r3, r1]
	lsls r4, r4, #3
	ldr r1, [sp, #24]
	mov r8, r4
	bl EffectPosition_ApplyAlternateStepAndYOffset
	ldr r4, [sp, #24]
	ldr r3, [r4]
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	str r3, [r4]
	ldr r0, [sp, #40]
	mov r3, r8
	adds r3, #1
	cmp r0, r3
	bne .L_080ddf50
	ldr r2, .L_080de0bc
	movs r3, #4
	add r2, r11
	str r3, [r2]
.L_080ddf50:
	mov r3, r8
	ldr r1, [sp, #40]
	adds r3, #4
	cmp r1, r3
	bne .L_080ddf7a
	ldr r3, [r5]
	ldr r2, [sp, #16]
	ldrsh r0, [r3, r2]
	movs r3, #6
	str r3, [sp, #0]
	movs r1, #7
	movs r2, #5
	ldr r3, [sp, #44]
	bl ObjectGroup_UpdateMembers
	ldr r3, [r5]
	ldr r1, [sp, #16]
	ldrsh r0, [r3, r1]
	movs r1, #6
	bl BattleMotion_ApplyVariantMotionFar
.L_080ddf7a:
	movs r3, #2
	ldr r4, [sp, #40]
	add r3, r8
	mov r9, r3
	cmp r4, r8
	bge .L_080ddf88
	b .L_080de0da
.L_080ddf88:
	mov r3, r8
	adds r3, #16
	cmp r4, r3
	blt .L_080ddf92
	b .L_080de0d4
.L_080ddf92:
	mov r0, r8
	subs r3, r4, r0
	lsls r6, r3, #6
	cmp r6, #104
	ble .L_080ddf9e
	movs r6, #104
.L_080ddf9e:
	ldr r3, [r5]
	ldr r3, [r3, #24]
	ldr r7, .L_080de0c0
	lsls r3, r3, #2
	adds r3, #3
	adds r2, r7, #0
	ldrb r3, [r2, r3]
	movs r1, #0
	mov r10, r1
	cmp r3, #0
	beq .L_080de004
	ldr r3, [sp, #44]
	ldr r4, [sp, #40]
	mov r9, r7
	adds r5, r3, r4
.L_080ddfbc:
	mov r0, r10
	adds r3, r5, r0
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	movs r2, #3
	ands r3, r2
	lsls r2, r3, #1
	adds r2, r2, r3
	lsls r1, r2, #4
	subs r1, r1, r2
	lsls r1, r1, #6
	ldr r2, .L_080de098
	add r1, r11
	adds r1, r1, r2
	ldr r2, [sp, #52]
	movs r3, #24
	subs r2, #12
	str r3, [sp, #0]
	str r6, [sp, #4]
	movs r3, #0
	ldr r0, [sp, #48]
	ldr r4, [sp, #32]
	bl _call_via_r4
	ldr r1, [sp, #20]
	ldr r3, [r1]
	ldr r3, [r3, #24]
	lsls r3, r3, #2
	adds r3, #3
	mov r2, r9
	movs r0, #1
	ldrb r3, [r2, r3]
	add r10, r0
	cmp r10, r3
	bne .L_080ddfbc
.L_080de004:
	movs r3, #2
	add r3, r8
	ldr r4, [sp, #40]
	mov r9, r3
	cmp r4, r9
	bne .L_080de0da
	ldr r1, [sp, #20]
	ldr r3, [r1]
	ldr r3, [r3, #24]
	lsls r3, r3, #2
	ldrb r3, [r7, r3]
	movs r0, #0
	mov r10, r0
	cmp r3, #0
	beq .L_080de0da
	ldr r2, [sp, #12]
	ldr r3, .L_080de0c4
	adds r7, r2, r3
.L_080de028:
	bl Random16
	ldr r6, .L_080de0c8
	ands r6, r0
	bl Random16
	ldr r3, [sp, #52]
	ldr r5, .L_080de0cc
	ldr r4, .L_080de0d0
	lsls r3, r3, #16
	str r3, [r7]
	ands r5, r0
	movs r3, #208
	adds r5, r5, r4
	lsls r3, r3, #15
	str r3, [r7, #4]
	adds r0, r5, #0
	bl Trig_Sin
	adds r6, #64
	adds r3, r6, #0
	muls r3, r0
	asrs r3, r3, #5
	str r3, [r7, #12]
	adds r0, r5, #0
	bl Trig_Cos
	adds r3, r6, #0
	muls r3, r0
	negs r3, r3
	asrs r3, r3, #6
	str r3, [r7, #16]
	bl Random16
	movs r3, #7
	ands r3, r0
	adds r3, #32
	str r3, [r7, #24]
	ldr r1, [sp, #20]
	ldr r3, [r1]
	ldr r3, [r3, #24]
	ldr r2, .L_080de0c0
	lsls r3, r3, #2
	movs r0, #1
	ldrb r3, [r2, r3]
	add r10, r0
	adds r7, #28
	cmp r10, r3
	bne .L_080de028
	b .L_080de0da
.L_080de08c:
	.4byte gBattleFxWork
.L_080de090:
	.4byte 0x00007828
.L_080de094:
	.4byte 0x000000ce
.L_080de098:
	.4byte 0x00000c56
.L_080de09c:
	.4byte 0x000000c4
.L_080de0a0:
	.4byte 0x00000073
.L_080de0a4:
	.4byte gMapCellBuffer + 0x18
.L_080de0a8:
	.4byte 0x00007098
.L_080de0ac:
	.4byte 0x00007784
.L_080de0b0:
	.4byte BattlePresentation_ProcessPendingGraphicsTransfer
.L_080de0b4:
	.4byte IwramFillWords
.L_080de0b8:
	.4byte 0x10101010
.L_080de0bc:
	.4byte 0x000077a8
.L_080de0c0:
	.4byte Data_080eebd6
.L_080de0c4:
	.4byte gMapCellBuffer
.L_080de0c8:
	.4byte 0x000001ff
.L_080de0cc:
	.4byte 0x00007fff
.L_080de0d0:
	.4byte 0xffffc000
.L_080de0d4:
	movs r3, #2
	add r3, r8
	mov r9, r3
.L_080de0da:
	ldr r4, [sp, #40]
	cmp r4, r9
	blt .L_080de1ca
	mov r3, r8
	adds r3, #24
	cmp r4, r3
	bge .L_080de1ca
	ldr r1, [sp, #20]
	ldr r3, [r1]
	ldr r3, [r3, #24]
	ldr r2, .L_080de2c4
	lsls r3, r3, #2
	adds r3, #1
	ldrb r3, [r2, r3]
	movs r0, #0
	mov r10, r0
	cmp r3, #0
	beq .L_080de1ca
	ldr r7, .L_080de2c8
	movs r3, #3
	mov r9, r3
	add r7, r11
.L_080de106:
	mov r4, r10
	mov r0, r9
	ands r4, r0
	str r4, [sp, #8]
	bl Random16
	ldr r3, [r7]
	ldr r3, [r3, #24]
	ldr r1, .L_080de2c4
	lsls r3, r3, #2
	adds r3, #2
	ldrb r5, [r1, r3]
	adds r1, r5, #0
	bl __umodsi3
	ldr r2, [sp, #24]
	ldr r2, [r2, #4]
	mov r8, r2
	mov r3, r8
	ldr r4, [sp, #8]
	subs r3, r3, r0
	subs r5, r5, r0
	ldr r0, .L_080de2cc
	mov r8, r3
	ldrb r3, [r0, r4]
	mov r1, r8
	lsrs r3, r3, #1
	subs r1, r1, r3
	movs r2, #8
	mov r8, r1
	add r8, r2
	bl Random16
	adds r5, #1
	ldr r3, [sp, #24]
	adds r1, r5, #0
	ldr r6, [r3]
	bl __umodsi3
	ldr r4, [sp, #8]
	adds r6, r6, r0
	ldr r0, .L_080de2d0
	lsrs r3, r5, #31
	adds r5, r5, r3
	ldrb r3, [r0, r4]
	asrs r5, r5, #1
	lsrs r3, r3, #1
	subs r6, r6, r5
	subs r6, r6, r3
	bl Random16
	ldr r3, .L_080de2d4
	mov r1, r9
	ands r0, r1
	ldrb r2, [r3, r0]
	mov r3, r9
	orrs r3, r2
	ldr r2, [r7]
	ldr r1, .L_080de2d8
	ldr r2, [r2, #24]
	ldrb r2, [r1, r2]
	movs r0, #47
	str r2, [sp, #0]
	movs r1, #7
	movs r2, #7
	bl Unnamed_080ed408
	ldr r4, [sp, #8]
	ldr r2, .L_080de2dc
	lsls r3, r4, #1
	ldrh r1, [r2, r3]
	ldr r2, .L_080de2d0
	ldrb r3, [r2, r4]
	ldr r0, .L_080de2cc
	str r3, [sp, #0]
	ldrb r3, [r0, r4]
	ldr r2, .L_080de2e0
	str r3, [sp, #4]
	add r1, r11
	ldr r4, [r2]
	mov r3, r8
	ldr r0, [sp, #48]
	adds r2, r6, #0
	bl _call_via_r4
	movs r0, #47
	bl Runtime_ReleaseHeapBlock
	movs r3, #1
	add r10, r3
	ldr r3, [r7]
	ldr r3, [r3, #24]
	ldr r4, .L_080de2c4
	lsls r3, r3, #2
	adds r3, #1
	ldrb r3, [r4, r3]
	cmp r10, r3
	bne .L_080de106
.L_080de1ca:
	ldr r0, [sp, #16]
	ldr r1, [sp, #12]
	ldr r3, [sp, #44]
	movs r2, #224
	lsls r2, r2, #4
	adds r0, #2
	adds r1, r1, r2
	adds r3, #1
	str r0, [sp, #16]
	str r3, [sp, #44]
	str r1, [sp, #12]
	ldr r2, .L_080de2c8
	mov r4, r11
	ldr r3, [r4, r2]
	ldr r0, [sp, #44]
	ldr r3, [r3, #20]
	cmp r0, r3
	beq .L_080de1f0
	b .L_080ddf1c
.L_080de1f0:
	movs r1, #0
	ldr r6, .L_080de2e4
	mov r10, r1
.L_080de1f6:
	ldr r3, [r6, #24]
	cmp r3, #0
	ble .L_080de262
	subs r3, #1
	movs r2, #128
	str r3, [r6, #24]
	lsls r2, r2, #5
	adds r0, r6, #0
	movs r1, #60
	bl EffectStep_AdvanceWithGravity2D
	movs r2, #208
	ldr r3, [r6, #4]
	lsls r2, r2, #15
	cmp r3, r2
	ble .L_080de224
	ldr r3, [r6, #16]
	negs r3, r3
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	str r3, [r6, #16]
	b .L_080de262
.L_080de224:
	ldr r2, [r6]
	ldr r4, .L_080de2e8
	cmp r2, r4
	bhi .L_080de262
	cmp r3, #0
	blt .L_080de262
	ldr r4, [r6, #24]
	cmp r4, #0
	bge .L_080de238
	adds r4, #15
.L_080de238:
	asrs r4, r4, #4
	adds r4, #1
	lsls r5, r4, #1
	ldr r0, .L_080de2ec
	subs r1, r5, #2
	ldrh r1, [r0, r1]
	ldr r0, [sp, #28]
	adds r1, r0, r1
	lsrs r0, r4, #31
	adds r0, r4, r0
	asrs r0, r0, #1
	asrs r2, r2, #16
	asrs r3, r3, #16
	subs r2, r2, r0
	subs r3, r3, r4
	str r4, [sp, #0]
	str r5, [sp, #4]
	ldr r0, [sp, #48]
	ldr r4, [sp, #32]
	bl _call_via_r4
.L_080de262:
	movs r0, #1
	movs r1, #128
	add r10, r0
	lsls r1, r1, #3
	adds r6, #28
	cmp r10, r1
	bne .L_080de1f6
	movs r0, #2
	movs r1, #8
	bl Camera_ApplyShake
	bl ObjectGroup_TickMemberTimers
	ldr r2, .L_080de2f0
	movs r3, #1
	add r2, r11
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r2, [sp, #40]
	adds r2, #1
	str r2, [sp, #40]
	ldr r4, [sp, #20]
	ldr r3, [r4]
	ldr r3, [r3, #20]
	lsls r3, r3, #3
	adds r3, #40
	cmp r2, r3
	beq .L_080de2a0
	b .L_080ddeb8
.L_080de2a0:
	ldr r0, .L_080de2f4
	bl Scheduler_RemoveCallback
	movs r0, #46
	bl Runtime_ReleaseHeapBlock
	bl BattleFx_EndCanvasLayer
	add sp, #64
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
.L_080de2c4:
	.4byte Data_080eebd6
.L_080de2c8:
	.4byte 0x00007828
.L_080de2cc:
	.4byte BattleFx_GlintCellHeights
.L_080de2d0:
	.4byte BattleFx_GlintCellWidths
.L_080de2d4:
	.4byte Data_080eebe2
.L_080de2d8:
	.4byte Data_080eebe6
.L_080de2dc:
	.4byte BattleFx_GlintCellOffsets
.L_080de2e0:
	.4byte gTransitionWork + 0xc
.L_080de2e4:
	.4byte gMapCellBuffer
.L_080de2e8:
	.4byte 0x007effff
.L_080de2ec:
	.4byte ParticleStreams_CellOffsets
.L_080de2f0:
	.4byte 0x00007824
.L_080de2f4:
	.4byte BattlePresentation_ProcessPendingGraphicsTransfer
