@ not-yet-c: own-ROM disassembly; source-shaped scalar draft remains unmatched.
	.syntax unified
	.thumb
	.text
	.balign 4
	.global BattleFx_RunTwoResource
	.type BattleFx_RunTwoResource, %function
	.thumb_func
BattleFx_RunTwoResource:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r6, .Lpool_048
	adds r3, r6, #0
	ldmia r3!, {r7}
	ldr r2, .Lpool_04c
	ldr r3, [r3]
	adds r5, r7, r2
	str r0, [r5]
	movs r0, #0
	mov r9, r3
	sub sp, #32
	mov r10, r1
	bl BattleFx_BeginCanvasLayer
	ldr r3, .Lpool_044
	ldr r2, .Lpool_050
	strh r3, [r2]
	ldr r3, [r5]
	ldr r3, [r3, #4]
	cmp r3, #1
	bne .Lbranch_054
	str r3, [sp, #0]
	movs r0, #46
	movs r1, #7
	movs r2, #7
	movs r3, #3
	b .Lbranch_060
	.balign 4, 0
.Lpool_044:
	.4byte 0x00000100
.Lpool_048:
	.4byte gBattleFxWork
.Lpool_04c:
	.4byte 0x00007828
.Lpool_050:
	.4byte 0x04000020
.Lbranch_054:
	movs r3, #1
	str r3, [sp, #0]
	movs r0, #46
	movs r1, #7
	movs r2, #7
	movs r3, #7
.Lbranch_060:
	bl BattleEffect_LoadWork
	ldr r6, [r6, #28]
	str r6, [sp, #12]
	ldr r0, .Lpool_240
	adds r1, r7, #0
	movs r2, #1
	movs r3, #1
	bl Resource_LoadAndDecompress
	movs r3, #0
	ldr r0, .Lpool_244
	ldr r1, .Lpool_248
	movs r2, #1
	bl Resource_LoadAndDecompress
	mov r3, r10
	cmp r3, #0
	bne .Lbranch_09a
	ldr r0, .Lpool_24c
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .Lpool_250
	lsls r0, r0, #19
	movs r2, #128
	bl _call_via_r3
.Lbranch_09a:
	movs r4, #239
	lsls r4, r4, #7
	ldr r0, .Lpool_254
	adds r2, r7, r4
	movs r3, #2
	str r3, [r2]
	movs r1, #144
	adds r2, r7, r0
	movs r3, #75
	str r3, [r2]
	lsls r1, r1, #3
	ldr r0, .Lpool_258
	bl Scheduler_AddOrUpdateCallback
	ldr r2, .Lpool_25c
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
	bne .Lbranch_0e4
	ldr r2, [r6]
	movs r3, #16
	b .Lbranch_0e8
.Lbranch_0e4:
	ldr r2, [r6]
	movs r3, #112
.Lbranch_0e8:
	ldr r1, .Lpool_260
	subs r3, r3, r2
	lsls r3, r3, #8
	str r3, [r1]
	movs r3, #74
	mov r4, r10
	str r3, [sp, #8]
	cmp r4, #1
	beq .Lbranch_0fe
	movs r0, #48
	str r0, [sp, #8]
.Lbranch_0fe:
	ldr r2, [sp, #8]
	movs r5, #0
	cmp r2, #0
	bne .Lbranch_108
	b .Lbranch_21c
.Lbranch_108:
	ldr r3, .Lpool_25c
	ldr r4, .Lpool_264
	adds r6, r7, r3
	mov r11, r4
.Lbranch_110:
	adds r3, r5, #0
	cmp r5, #0
	bge .Lbranch_118
	adds r3, r5, #3
.Lbranch_118:
	asrs r4, r3, #2
	cmp r4, #5
	bgt .Lbranch_18a
	cmp r4, #3
	bgt .Lbranch_156
	lsls r3, r4, #1
	mov r0, r11
	ldrh r1, [r0, r3]
	ldr r3, [r6]
	ldr r2, [r3, #4]
	lsls r3, r2, #1
	adds r3, r3, r2
	ldr r0, .Lpool_268
	lsls r3, r3, #1
	adds r3, r4, r3
	ldrb r2, [r0, r3]
	ldr r0, .Lpool_26c
	ldr r3, .Lpool_270
	ldrb r0, [r0, r4]
	ldrsb r3, [r3, r4]
	str r0, [sp, #0]
	ldr r0, .Lpool_274
	ldrb r0, [r0, r4]
	adds r1, r7, r1
	str r0, [sp, #4]
	adds r3, #32
	mov r0, r9
	ldr r4, [sp, #12]
	bl _call_via_r4
	b .Lbranch_18a
.Lbranch_156:
	lsls r3, r4, #1
	mov r0, r11
	ldrh r1, [r0, r3]
	ldr r2, .Lpool_248
	ldr r3, [r6]
	adds r1, r1, r2
	ldr r2, [r3, #4]
	lsls r3, r2, #1
	adds r3, r3, r2
	ldr r0, .Lpool_268
	lsls r3, r3, #1
	adds r3, r4, r3
	ldrb r2, [r0, r3]
	ldr r0, .Lpool_26c
	ldr r3, .Lpool_270
	ldrb r0, [r0, r4]
	ldrsb r3, [r3, r4]
	str r0, [sp, #0]
	ldr r0, .Lpool_274
	ldrb r0, [r0, r4]
	adds r3, #32
	str r0, [sp, #4]
	ldr r4, [sp, #12]
	mov r0, r9
	bl _call_via_r4
.Lbranch_18a:
	cmp r5, #8
	bne .Lbranch_1ca
	mov r0, r10
	cmp r0, #0
	bne .Lbranch_1a8
	movs r0, #133
	bl BattleEventRuntime_BeginPhaseFar
	ldr r3, [r6]
	movs r1, #1
	movs r2, #36
	ldrsh r0, [r3, r2]
	bl BattleMotion_ApplyVariantMotionFar
	b .Lbranch_1c2
.Lbranch_1a8:
	movs r0, #134
	bl Audio_PlayCue
	ldr r3, [r6]
	movs r4, #36
	ldrsh r0, [r3, r4]
	movs r3, #4
	str r3, [sp, #0]
	movs r1, #7
	movs r2, #5
	movs r3, #0
	bl ObjectGroup_UpdateMembers
.Lbranch_1c2:
	ldr r0, .Lpool_278
	movs r3, #8
	adds r2, r7, r0
	str r3, [r2]
.Lbranch_1ca:
	mov r2, r10
	cmp r2, #1
	bne .Lbranch_1f8
	cmp r5, #13
	bne .Lbranch_1e6
	movs r3, #192
	mov r4, r8
	lsls r3, r3, #12
	str r3, [r4, #40]
	ldr r3, .Lpool_27c
	str r3, [r4, #72]
	movs r3, #128
	lsls r3, r3, #7
	str r3, [r4, #68]
.Lbranch_1e6:
	cmp r5, #65
	bne .Lbranch_1f8
	ldr r0, .Lpool_278
	movs r3, #4
	adds r2, r7, r0
	str r3, [r2]
	movs r0, #134
	bl BattleEventRuntime_BeginPhaseFar
.Lbranch_1f8:
	movs r0, #8
	movs r1, #8
	bl Camera_ApplyShake
	bl ObjectGroup_TickMemberTimers
	ldr r3, .Lpool_280
	adds r2, r7, r3
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r4, [sp, #8]
	adds r5, #1
	cmp r5, r4
	beq .Lbranch_21c
	b .Lbranch_110
.Lbranch_21c:
	ldr r0, .Lpool_258
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
	.balign 4, 0
.Lpool_240:
	.4byte ResourceId_MagentaSwirlSheet
.Lpool_244:
	.4byte ResourceId_MagentaTailSheet
.Lpool_248:
	.4byte gMapCellBuffer
.Lpool_24c:
	.4byte ResourceId_LimePalette
.Lpool_250:
	.4byte IwramCopyWords
.Lpool_254:
	.4byte 0x00007784
.Lpool_258:
	.4byte BattlePresentation_ProcessPendingGraphicsTransfer
.Lpool_25c:
	.4byte 0x00007828
.Lpool_260:
	.4byte 0x04000028
.Lpool_264:
	.4byte TwoResource_CellSourceOffsets
.Lpool_268:
	.4byte TwoResource_CellX
.Lpool_26c:
	.4byte TwoResource_CellWidths
.Lpool_270:
	.4byte TwoResource_CellBiasY
.Lpool_274:
	.4byte TwoResource_CellHeights
.Lpool_278:
	.4byte 0x000077a8
.Lpool_27c:
	.4byte 0x00007851
.Lpool_280:
	.4byte 0x00007824
	.size BattleFx_RunTwoResource, .-BattleFx_RunTwoResource
