.syntax unified
	.thumb
	.global BattleFx_RunTwoResource
	.thumb_func
BattleFx_RunTwoResource:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r6, .L_080ccc80
	adds r3, r6, #0
	ldmia r3!, {r7}
	ldr r2, .L_080ccc84
	ldr r3, [r3]
	adds r5, r7, r2
	str r0, [r5]
	movs r0, #0
	mov r9, r3
	sub sp, #32
	mov r10, r1
	bl BattleFx_BeginCanvasLayer
	ldr r3, .L_080ccc7c
	ldr r2, .L_080ccc88
	strh r3, [r2]
	ldr r3, [r5]
	ldr r3, [r3, #4]
	cmp r3, #1
	bne .L_080ccc8c
	str r3, [sp, #0]
	movs r0, #46
	movs r1, #7
	movs r2, #7
	movs r3, #3
	b .L_080ccc98
	.2byte 0x0000
.L_080ccc7c:
	.4byte 0x00000100
.L_080ccc80:
	.4byte gBattleFxWork
.L_080ccc84:
	.4byte 0x00007828
.L_080ccc88:
	.4byte 0x04000020
.L_080ccc8c:
	movs r3, #1
	str r3, [sp, #0]
	movs r0, #46
	movs r1, #7
	movs r2, #7
	movs r3, #7
.L_080ccc98:
	bl Unnamed_080ed408
	ldr r6, [r6, #28]
	str r6, [sp, #12]
	ldr r0, .L_080cce78
	adds r1, r7, #0
	movs r2, #1
	movs r3, #1
	bl Resource_LoadAndDecompress
	movs r3, #0
	ldr r0, .L_080cce7c
	ldr r1, .L_080cce80
	movs r2, #1
	bl Resource_LoadAndDecompress
	mov r3, r10
	cmp r3, #0
	bne .L_080cccd2
	ldr r0, .L_080cce84
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_080cce88
	lsls r0, r0, #19
	movs r2, #128
	bl _call_via_r3
.L_080cccd2:
	movs r4, #239
	lsls r4, r4, #7
	ldr r0, .L_080cce8c
	adds r2, r7, r4
	movs r3, #2
	str r3, [r2]
	movs r1, #144
	adds r2, r7, r0
	movs r3, #75
	str r3, [r2]
	lsls r1, r1, #3
	ldr r0, .L_080cce90
	bl Scheduler_AddOrUpdateCallback
	ldr r2, .L_080cce94
	adds r5, r7, r2
	ldr r3, [r5]
	movs r4, #36
	ldrsh r0, [r3, r4]
	bl GetBattleObjectSlotFar
	ldr r3, [r5]
	ldr r0, [r0]
	add r6, sp, #20
	mov r8, r0
	adds r1, r6, #0
	movs r2, #36
	ldrsh r0, [r3, r2]
	bl EffectPosition_ApplyStepAndYOffset
	ldr r3, [r5]
	ldr r3, [r3, #4]
	cmp r3, #0
	bne .L_080ccd1c
	ldr r2, [r6]
	movs r3, #16
	b .L_080ccd20
.L_080ccd1c:
	ldr r2, [r6]
	movs r3, #112
.L_080ccd20:
	ldr r1, .L_080cce98
	subs r3, r3, r2
	lsls r3, r3, #8
	str r3, [r1]
	movs r3, #74
	mov r4, r10
	str r3, [sp, #8]
	cmp r4, #1
	beq .L_080ccd36
	movs r0, #48
	str r0, [sp, #8]
.L_080ccd36:
	ldr r2, [sp, #8]
	movs r5, #0
	cmp r2, #0
	bne .L_080ccd40
	b .L_080cce54
.L_080ccd40:
	ldr r3, .L_080cce94
	ldr r4, .L_080cce9c
	adds r6, r7, r3
	mov r11, r4
.L_080ccd48:
	adds r3, r5, #0
	cmp r5, #0
	bge .L_080ccd50
	adds r3, r5, #3
.L_080ccd50:
	asrs r4, r3, #2
	cmp r4, #5
	bgt .L_080ccdc2
	cmp r4, #3
	bgt .L_080ccd8e
	lsls r3, r4, #1
	mov r0, r11
	ldrh r1, [r0, r3]
	ldr r3, [r6]
	ldr r2, [r3, #4]
	lsls r3, r2, #1
	adds r3, r3, r2
	ldr r0, .L_080ccea0
	lsls r3, r3, #1
	adds r3, r4, r3
	ldrb r2, [r0, r3]
	ldr r0, .L_080ccea4
	ldr r3, .L_080ccea8
	ldrb r0, [r0, r4]
	ldrsb r3, [r3, r4]
	str r0, [sp, #0]
	ldr r0, .L_080cceac
	ldrb r0, [r0, r4]
	adds r1, r7, r1
	str r0, [sp, #4]
	adds r3, #32
	mov r0, r9
	ldr r4, [sp, #12]
	bl _call_via_r4
	b .L_080ccdc2
.L_080ccd8e:
	lsls r3, r4, #1
	mov r0, r11
	ldrh r1, [r0, r3]
	ldr r2, .L_080cce80
	ldr r3, [r6]
	adds r1, r1, r2
	ldr r2, [r3, #4]
	lsls r3, r2, #1
	adds r3, r3, r2
	ldr r0, .L_080ccea0
	lsls r3, r3, #1
	adds r3, r4, r3
	ldrb r2, [r0, r3]
	ldr r0, .L_080ccea4
	ldr r3, .L_080ccea8
	ldrb r0, [r0, r4]
	ldrsb r3, [r3, r4]
	str r0, [sp, #0]
	ldr r0, .L_080cceac
	ldrb r0, [r0, r4]
	adds r3, #32
	str r0, [sp, #4]
	ldr r4, [sp, #12]
	mov r0, r9
	bl _call_via_r4
.L_080ccdc2:
	cmp r5, #8
	bne .L_080cce02
	mov r0, r10
	cmp r0, #0
	bne .L_080ccde0
	movs r0, #133
	bl BattleEventRuntime_BeginPhaseFar
	ldr r3, [r6]
	movs r1, #1
	movs r2, #36
	ldrsh r0, [r3, r2]
	bl BattleMotion_ApplyVariantMotionFar
	b .L_080ccdfa
.L_080ccde0:
	movs r0, #134
	bl AudioCommand_PlayFar
	ldr r3, [r6]
	movs r4, #36
	ldrsh r0, [r3, r4]
	movs r3, #4
	str r3, [sp, #0]
	movs r1, #7
	movs r2, #5
	movs r3, #0
	bl ObjectGroup_UpdateMembers
.L_080ccdfa:
	ldr r0, .L_080cceb0
	movs r3, #8
	adds r2, r7, r0
	str r3, [r2]
.L_080cce02:
	mov r2, r10
	cmp r2, #1
	bne .L_080cce30
	cmp r5, #13
	bne .L_080cce1e
	movs r3, #192
	mov r4, r8
	lsls r3, r3, #12
	str r3, [r4, #40]
	ldr r3, .L_080cceb4
	str r3, [r4, #72]
	movs r3, #128
	lsls r3, r3, #7
	str r3, [r4, #68]
.L_080cce1e:
	cmp r5, #65
	bne .L_080cce30
	ldr r0, .L_080cceb0
	movs r3, #4
	adds r2, r7, r0
	str r3, [r2]
	movs r0, #134
	bl BattleEventRuntime_BeginPhaseFar
.L_080cce30:
	movs r0, #8
	movs r1, #8
	bl Camera_ApplyShake
	bl ObjectGroup_TickMemberTimers
	ldr r3, .L_080cceb8
	adds r2, r7, r3
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r4, [sp, #8]
	adds r5, #1
	cmp r5, r4
	beq .L_080cce54
	b .L_080ccd48
.L_080cce54:
	ldr r0, .L_080cce90
	bl Scheduler_RemoveCallback
	movs r0, #46
	bl Runtime_ReleaseHeapBlock
	bl BattleFx_EndCanvasLayer
	add sp, #32
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
.L_080cce78:
	.4byte 0x00000071
.L_080cce7c:
	.4byte 0x00000072
.L_080cce80:
	.4byte gMapCellBuffer
.L_080cce84:
	.4byte 0x000000a0
.L_080cce88:
	.4byte IwramCopyWords
.L_080cce8c:
	.4byte 0x00007784
.L_080cce90:
	.4byte BattlePresentation_ProcessPendingGraphicsTransfer
.L_080cce94:
	.4byte 0x00007828
.L_080cce98:
	.4byte 0x04000028
.L_080cce9c:
	.4byte TwoResource_CellSourceOffsets
.L_080ccea0:
	.4byte TwoResource_CellX
.L_080ccea4:
	.4byte TwoResource_CellWidths
.L_080ccea8:
	.4byte TwoResource_CellBiasY
.L_080cceac:
	.4byte TwoResource_CellHeights
.L_080cceb0:
	.4byte 0x000077a8
.L_080cceb4:
	.4byte 0x00007851
.L_080cceb8:
	.4byte 0x00007824
