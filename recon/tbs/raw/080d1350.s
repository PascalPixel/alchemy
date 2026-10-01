.syntax unified
	.thumb
	.global Unnamed_080d1350
	.thumb_func
Unnamed_080d1350:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r6, .L_080d16d4
	adds r3, r6, #0
	ldmia r3!, {r1}
	sub sp, #64
	str r1, [sp, #48]
	ldr r3, [r3]
	str r3, [sp, #44]
	ldr r2, [r6, #8]
	adds r3, r6, #0
	str r2, [sp, #24]
	subs r3, #108
	ldr r3, [r3]
	str r3, [sp, #20]
	ldr r3, .L_080d16d8
	adds r1, r1, r3
	str r0, [r1]
	movs r0, #1
	mov r8, r1
	bl BattleFx_BeginCanvasLayer
	ldr r0, .L_080d16dc
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_080d16e0
	movs r2, #128
	lsls r0, r0, #19
	bl _call_via_r3
	ldr r0, .L_080d16e4
	bl Resource_GetTableEntry
	ldr r1, [sp, #24]
	bl Resource_DecodeType01
	movs r5, #2
	movs r1, #7
	movs r2, #7
	movs r3, #3
	movs r0, #46
	str r5, [sp, #0]
	bl BattleEffect_LoadWork
	movs r1, #239
	ldr r0, [sp, #48]
	lsls r1, r1, #7
	adds r3, r0, r1
	str r5, [r3]
	ldr r3, .L_080d16e8
	ldr r6, [r6, #28]
	adds r2, r0, r3
	movs r1, #144
	movs r3, #50
	str r3, [r2]
	lsls r1, r1, #3
	mov r5, r8
	ldr r0, .L_080d16ec
	str r6, [sp, #32]
	bl Scheduler_AddOrUpdateCallback
	ldr r3, [r5]
	ldr r0, [r3, #8]
	bl GetBattleObjectSlotFar
	ldr r3, [r5]
	ldr r0, [r0]
	mov r10, r0
	movs r1, #36
	ldrsh r0, [r3, r1]
	bl GetBattleObjectSlotFar
	movs r5, #225
	movs r2, #0
	ldr r3, [sp, #48]
	ldr r7, [r0]
	lsls r5, r5, #7
	str r2, [sp, #28]
	adds r6, r3, r5
.L_080d13fc:
	mov r1, r10
	ldr r3, [r1, #8]
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	ldr r0, [sp, #28]
	str r3, [r6]
	movs r2, #240
	ldr r3, [r1, #12]
	lsls r2, r2, #15
	adds r3, r3, r2
	str r3, [r6, #4]
	ldr r3, [r1, #16]
	str r3, [r6, #8]
	lsls r5, r0, #3
	bl Random16
	movs r3, #127
	ands r3, r0
	subs r3, #64
	ldr r0, [r7, #8]
	lsls r3, r3, #16
	adds r0, r0, r3
	ldr r3, [r6]
	movs r1, #12
	subs r0, r0, r3
	bl __divsi3
	str r0, [r6, #12]
	ldr r0, [r7, #12]
	ldr r3, [r6, #4]
	subs r0, r0, r3
	movs r3, #160
	lsls r3, r3, #13
	adds r0, r0, r3
	movs r1, #12
	bl __divsi3
	str r0, [r6, #16]
	ldr r3, [r6, #8]
	ldr r0, [r7, #16]
	movs r1, #12
	subs r0, r0, r3
	bl __divsi3
	str r0, [r6, #20]
	bl Random16
	movs r3, #15
	ands r3, r0
	adds r3, r3, r5
	str r3, [r6, #24]
	ldr r5, [sp, #28]
	adds r5, #1
	adds r6, #28
	str r5, [sp, #28]
	cmp r5, #8
	bne .L_080d13fc
	movs r0, #0
	str r0, [sp, #40]
.L_080d1474:
	ldr r1, .L_080d16f0
	ldr r0, [sp, #40]
	ldr r2, .L_080d16f4
	movs r3, #0
	bl Graphics_UpdatePhasePalette
	ldr r1, [sp, #40]
	cmp r1, #96
	bne .L_080d148c
	movs r0, #134
	bl BattleEventRuntime_BeginPhaseFar
.L_080d148c:
	movs r2, #0
	ldr r3, [sp, #48]
	movs r5, #225
	lsls r5, r5, #7
	str r2, [sp, #28]
	str r2, [sp, #16]
	adds r7, r3, r5
.L_080d149a:
	ldr r3, [r7, #24]
	ldr r0, [sp, #40]
	cmp r0, r3
	bge .L_080d14a4
	b .L_080d166e
.L_080d14a4:
	bl Render_ResetTransformState
	ldr r0, [sp, #20]
	adds r1, r0, #0
	adds r1, #12
	bl Graphics_PrepareTransferInIwramWork
	add r5, sp, #52
	adds r0, r7, #0
	adds r1, r5, #0
	bl EffectPosition_ApplyBaseAndYOffset
	ldr r3, [r5]
	asrs r3, r3, #1
	str r3, [r5]
	adds r3, #8
	cmp r3, #135
	bls .L_080d14ca
	b .L_080d15ee
.L_080d14ca:
	ldr r3, [r5, #4]
	cmp r3, #127
	ble .L_080d14d2
	b .L_080d15ee
.L_080d14d2:
	movs r1, #8
	negs r1, r1
	cmp r3, r1
	bge .L_080d14dc
	b .L_080d15ee
.L_080d14dc:
	ldr r3, [sp, #28]
	lsls r2, r3, #2
	adds r2, r2, r3
	lsls r3, r2, #3
	mov r10, r5
	subs r3, r3, r2
	ldr r5, .L_080d16f8
	ldr r0, .L_080d16fc
	lsls r3, r3, #3
	movs r4, #0
	mov r8, r5
	adds r1, r3, r0
.L_080d14f4:
	ldr r3, .L_080d1700
	adds r6, r4, #0
	muls r6, r3
	ldr r2, [sp, #40]
	ldr r0, [r7, #24]
	subs r0, r2, r0
	lsls r0, r0, #11
	subs r0, r6, r0
	str r1, [sp, #12]
	str r4, [sp, #8]
	bl Trig_Sin
	ldr r4, [sp, #8]
	movs r5, #1
	ands r5, r4
	mov r2, r8
	ldrb r3, [r2, r5]
	adds r2, r3, #0
	muls r2, r0
	mov r0, r10
	lsrs r3, r2, #31
	adds r2, r2, r3
	ldr r3, [r0]
	ldr r1, [sp, #12]
	asrs r2, r2, #17
	adds r3, r3, r2
	str r3, [r1, #12]
	ldr r2, [sp, #40]
	ldr r3, [r7, #24]
	subs r3, r2, r3
	lsls r3, r3, #11
	subs r6, r6, r3
	adds r0, r6, #0
	bl Trig_Cos
	mov r2, r8
	ldrb r3, [r2, r5]
	adds r2, r3, #0
	muls r2, r0
	mov r5, r10
	ldr r3, [r5, #4]
	ldr r4, [sp, #8]
	ldr r1, [sp, #12]
	asrs r2, r2, #16
	subs r3, r3, r2
	adds r4, #1
	str r3, [r1, #16]
	adds r1, #28
	cmp r4, #10
	bne .L_080d14f4
	ldr r0, [sp, #16]
	movs r4, #0
	mov r11, r0
.L_080d155e:
	mov r1, r11
	adds r2, r4, r1
	lsls r3, r2, #3
	adds r4, #1
	subs r3, r3, r2
	ldr r2, .L_080d16fc
	lsls r3, r3, #2
	mov r9, r4
	adds r2, r2, r3
	mov r0, r9
	movs r1, #10
	mov r10, r2
	bl __modsi3
	add r0, r11
	lsls r3, r0, #3
	subs r3, r3, r0
	ldr r5, .L_080d16fc
	lsls r3, r3, #2
	adds r5, r5, r3
	mov r8, r5
	movs r4, #0
.L_080d158a:
	mov r0, r8
	mov r1, r10
	ldr r6, [r1, #12]
	ldr r3, [r0, #12]
	subs r3, r3, r6
	adds r0, r4, #0
	muls r0, r3
	movs r1, #12
	str r4, [sp, #8]
	bl __divsi3
	mov r2, r8
	adds r6, r6, r0
	mov r0, r10
	ldr r3, [r2, #16]
	ldr r5, [r0, #16]
	ldr r4, [sp, #8]
	subs r3, r3, r5
	adds r0, r4, #0
	muls r0, r3
	movs r1, #12
	bl __divsi3
	ldr r2, .L_080d1704
	movs r3, #4
	subs r3, #2
	ldrh r1, [r2, r3]
	ldr r3, [sp, #24]
	adds r5, r5, r0
	movs r0, #1
	adds r1, r3, r1
	subs r6, r6, r0
	subs r5, #2
	movs r2, #2
	movs r3, #4
	str r2, [sp, #0]
	str r3, [sp, #4]
	ldr r0, [sp, #44]
	adds r3, r5, #0
	adds r2, r6, #0
	ldr r5, [sp, #32]
	bl _call_via_r5
	ldr r4, [sp, #8]
	adds r4, #1
	cmp r4, #12
	bne .L_080d158a
	mov r4, r9
	cmp r4, #10
	bne .L_080d155e
.L_080d15ee:
	ldr r3, [r7, #4]
	ldr r0, .L_080d1708
	cmp r3, r0
	bgt .L_080d1656
	ldr r3, [r7, #16]
	negs r3, r3
	str r3, [r7, #16]
	ldr r3, [r7, #12]
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	str r3, [r7, #12]
	ldr r3, [r7, #20]
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	str r3, [r7, #20]
	ldr r1, [sp, #48]
	ldr r3, .L_080d170c
	adds r2, r1, r3
	movs r3, #4
	str r3, [r2]
	movs r0, #134
	bl AudioCommand_PlayFar
	ldr r3, .L_080d16d8
	ldr r5, [sp, #48]
	ldr r3, [r5, r3]
	ldr r3, [r3, #20]
	movs r4, #0
	cmp r3, #0
	beq .L_080d1656
	ldr r0, .L_080d16d8
	movs r6, #36
	adds r5, r5, r0
.L_080d1634:
	ldr r3, [r5]
	ldrsh r0, [r3, r6]
	movs r3, #8
	str r3, [sp, #0]
	movs r1, #7
	adds r3, r4, #0
	movs r2, #5
	str r4, [sp, #8]
	bl ObjectGroup_UpdateMembers
	ldr r3, [r5]
	ldr r4, [sp, #8]
	ldr r3, [r3, #20]
	adds r4, #1
	adds r6, #2
	cmp r4, r3
	bne .L_080d1634
.L_080d1656:
	ldr r3, [r7]
	ldr r2, [r7, #12]
	adds r3, r3, r2
	str r3, [r7]
	ldr r2, [r7, #16]
	ldr r3, [r7, #4]
	adds r3, r3, r2
	str r3, [r7, #4]
	ldr r2, [r7, #20]
	ldr r3, [r7, #8]
	adds r3, r3, r2
	str r3, [r7, #8]
.L_080d166e:
	ldr r2, [sp, #16]
	ldr r3, [sp, #28]
	adds r2, #10
	adds r3, #1
	str r2, [sp, #16]
	adds r7, #28
	str r3, [sp, #28]
	cmp r3, #8
	beq .L_080d1682
	b .L_080d149a
.L_080d1682:
	movs r1, #4
	movs r0, #4
	bl Camera_ApplyShake
	bl ObjectGroup_TickMemberTimers
	ldr r0, .L_080d1710
	ldr r5, [sp, #48]
	movs r3, #1
	adds r2, r5, r0
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r1, [sp, #40]
	adds r1, #1
	str r1, [sp, #40]
	cmp r1, #128
	beq .L_080d16aa
	b .L_080d1474
.L_080d16aa:
	ldr r0, .L_080d16ec
	bl Scheduler_RemoveCallback
	movs r0, #47
	bl Runtime_ReleaseHeapBlock
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
.L_080d16d4:
	.4byte gBattleFxWork
.L_080d16d8:
	.4byte 0x00007828
.L_080d16dc:
	.4byte 0x00000079
.L_080d16e0:
	.4byte IwramCopyWords
.L_080d16e4:
	.4byte 0x00000073
.L_080d16e8:
	.4byte 0x00007784
.L_080d16ec:
	.4byte BattlePresentation_ProcessPendingGraphicsTransfer
.L_080d16f0:
	.4byte 0x0000aaab
.L_080d16f4:
	.4byte 0x00005555
.L_080d16f8:
	.4byte Data_080ee158
.L_080d16fc:
	.4byte gMapCellBuffer
.L_080d1700:
	.4byte 0x0000199a
.L_080d1704:
	.4byte ParticleStreams_CellOffsets
.L_080d1708:
	.4byte 0x001dffff
.L_080d170c:
	.4byte 0x000077a8
.L_080d1710:
	.4byte 0x00007824
