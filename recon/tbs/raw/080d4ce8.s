.syntax unified
	.thumb
	.global Region_080d4ce8
	.thumb_func
Region_080d4ce8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r2, .L_080d4d54
	adds r3, r2, #0
	ldmia r3!, {r1}
	ldr r3, [r3]
	sub sp, #64
	str r3, [sp, #40]
	ldr r5, .L_080d4d58
	mov r11, r1
	ldr r2, [r2, #8]
	add r5, r11
	str r2, [sp, #28]
	str r0, [r5]
	movs r0, #1
	bl BattleFx_BeginCanvasLayer
	ldr r2, .L_080d4d5c
	ldr r3, .L_080d4d50
	ldr r0, .L_080d4d60
	strh r3, [r2]
	mov r1, r11
	movs r2, #1
	movs r3, #1
	bl Resource_LoadAndDecompress
	movs r3, #0
	ldr r0, .L_080d4d64
	ldr r1, [sp, #28]
	movs r2, #0
	bl Resource_LoadAndDecompress
	ldr r3, [r5]
	ldr r3, [r3, #24]
	cmp r3, #0
	bne .L_080d4d72
	ldr r0, .L_080d4d68
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_080d4d6c
	lsls r0, r0, #19
	movs r2, #128
	bl _call_via_r3
	b .L_080d4d70
.L_080d4d50:
	.4byte 0x00001010
.L_080d4d54:
	.4byte gBattleFxWork
.L_080d4d58:
	.4byte 0x00007828
.L_080d4d5c:
	.4byte 0x04000052
.L_080d4d60:
	.4byte 0x00000085
.L_080d4d64:
	.4byte 0x00000073
.L_080d4d68:
	.4byte 0x00000086
.L_080d4d6c:
	.4byte IwramCopyWords
.L_080d4d70:
	b .L_080d4d8a
.L_080d4d72:
	cmp r3, #2
	bne .L_080d4d8a
	ldr r0, .L_080d5054
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_080d5058
	lsls r0, r0, #19
	movs r2, #128
	bl _call_via_r3
.L_080d4d8a:
	movs r6, #2
	movs r1, #7
	movs r2, #7
	movs r3, #3
	movs r0, #46
	str r6, [sp, #0]
	bl Unnamed_080ed408
	ldr r5, .L_080d505c
	adds r3, r5, #0
	adds r3, #184
	ldr r3, [r3]
	movs r1, #7
	str r3, [sp, #44]
	movs r2, #7
	movs r3, #7
	movs r0, #47
	str r6, [sp, #0]
	bl Unnamed_080ed408
	adds r5, #188
	ldr r3, [r5]
	mov r2, sp
	adds r2, #44
	str r2, [sp, #24]
	str r3, [r2, #4]
	movs r3, #0
	mov r8, r3
	movs r2, #128
	ldr r3, .L_080d5060
	movs r1, #0
	lsls r2, r2, #3
.L_080d4dca:
	movs r5, #1
	add r8, r5
	str r1, [r3]
	adds r3, #28
	cmp r8, r2
	bne .L_080d4dca
	ldr r3, .L_080d5064
	add r3, r11
	ldr r3, [r3]
	movs r7, #36
	ldrsh r0, [r3, r7]
	bl GetBattleObjectSlotFar
	movs r5, #225
	ldr r6, [r0]
	lsls r5, r5, #7
	movs r0, #0
	ldr r7, .L_080d5068
	mov r8, r0
	add r5, r11
.L_080d4df2:
	bl Random16
	movs r3, #15
	ands r3, r0
	adds r3, #72
	lsls r2, r3, #16
	movs r3, #0
	str r3, [r5, #4]
	ldr r3, .L_080d5064
	add r3, r11
	str r2, [r5]
	ldr r3, [r3]
	ldr r3, [r3, #24]
	lsls r3, r3, #2
	add r3, r8
	ldrsb r3, [r7, r3]
	lsls r3, r3, #16
	str r3, [r5, #8]
	ldr r3, [r6, #8]
	cmp r3, #0
	bge .L_080d4e20
	negs r3, r2
	str r3, [r5]
.L_080d4e20:
	movs r1, #1
	add r8, r1
	mov r2, r8
	adds r5, #28
	cmp r2, #4
	bne .L_080d4df2
	movs r2, #239
	lsls r2, r2, #7
	add r2, r11
	movs r3, #2
	str r3, [r2]
	ldr r2, .L_080d506c
	movs r3, #50
	add r2, r11
	movs r1, #144
	str r3, [r2]
	ldr r0, .L_080d5070
	lsls r1, r1, #3
	bl Scheduler_AddOrUpdateCallback
	movs r3, #0
	str r3, [sp, #36]
.L_080d4e4c:
	ldr r3, .L_080d5074
	ldr r5, [r3]
	ldr r3, .L_080d5064
	add r3, r11
	ldr r2, [r3]
	ldr r3, [r2, #24]
	cmp r3, #2
	bne .L_080d4e74
	ldr r7, [sp, #36]
	cmp r7, #63
	bgt .L_080d4e74
	ldr r3, [r2, #4]
	cmp r3, #0
	bne .L_080d4e6e
	ldrh r3, [r5, #54]
	adds r3, #192
	b .L_080d4e72
.L_080d4e6e:
	ldrh r3, [r5, #54]
	subs r3, #192
.L_080d4e72:
	strh r3, [r5, #54]
.L_080d4e74:
	ldr r0, [sp, #36]
	cmp r0, #16
	bne .L_080d4e80
	movs r0, #134
	bl BattleEventRuntime_BeginPhaseFar
.L_080d4e80:
	bl Render_ResetTransformState
	adds r1, r5, #0
	adds r1, #12
	adds r0, r5, #0
	bl Graphics_PrepareTransferInIwramWork
	ldr r1, [sp, #36]
	cmp r1, #63
	ble .L_080d4e96
	b .L_080d512e
.L_080d4e96:
	movs r2, #0
	ldr r3, .L_080d5064
	str r2, [sp, #32]
	add r3, r11
	ldr r3, [r3]
	ldr r2, .L_080d5078
	ldr r3, [r3, #24]
	ldrb r3, [r2, r3]
	cmp r3, #0
	bne .L_080d4eac
	b .L_080d512e
.L_080d4eac:
	adds r5, r1, #0
	movs r2, #221
	movs r1, #225
	movs r7, #1
	ldr r0, .L_080d507c
	lsls r1, r1, #7
	lsls r2, r2, #4
	movs r3, #52
	ands r5, r7
	add r1, r11
	add r2, r11
	add r3, sp
	str r5, [sp, #20]
	str r0, [sp, #12]
	str r1, [sp, #8]
	str r2, [sp, #16]
	mov r9, r3
.L_080d4ece:
	ldr r0, [sp, #8]
	mov r1, r9
	bl EffectPosition_ApplyBaseAndYOffset
	mov r5, r9
	ldr r3, [r5]
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	str r3, [r5]
	ldr r3, [r5, #4]
	subs r3, #8
	str r3, [r5, #4]
	ldr r7, [sp, #12]
	ldr r0, [sp, #36]
	ldrb r3, [r7]
	cmp r0, r3
	bne .L_080d4efa
	movs r0, #145
	bl AudioCommand_PlayFar
	ldrb r3, [r7]
.L_080d4efa:
	ldr r1, [sp, #36]
	adds r3, #4
	cmp r1, r3
	blt .L_080d4fc6
	ldr r2, [sp, #32]
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #3
	adds r3, r3, r2
	lsls r0, r1, #4
	adds r0, r0, r3
	movs r1, #104
	bl Math_Mod
	ldr r5, [sp, #32]
	movs r3, #1
	ands r5, r3
	ldr r3, [sp, #56]
	ldr r7, [sp, #24]
	mov r8, r0
	subs r3, r3, r0
	movs r6, #34
	movs r0, #104
	str r0, [sp, #4]
	ldr r2, [sp, #52]
	str r6, [sp, #0]
	lsls r5, r5, #2
	adds r5, r5, r7
	subs r2, #17
	subs r3, #104
	ldr r4, [r5]
	ldr r0, [sp, #40]
	mov r1, r11
	bl _call_via_r4
	mov r1, r8
	str r1, [sp, #4]
	ldr r2, [sp, #52]
	ldr r3, [sp, #56]
	str r6, [sp, #0]
	subs r2, #17
	subs r3, r3, r1
	ldr r4, [r5]
	ldr r0, [sp, #40]
	mov r1, r11
	bl _call_via_r4
	ldr r2, [sp, #20]
	cmp r2, #0
	beq .L_080d4f90
	ldr r2, [sp, #52]
	ldr r3, [sp, #56]
	movs r5, #20
	movs r7, #37
	subs r2, #20
	subs r3, #24
	ldr r4, [sp, #44]
	ldr r1, [sp, #16]
	str r5, [sp, #0]
	str r7, [sp, #4]
	ldr r0, [sp, #40]
	bl _call_via_r4
	ldr r0, [sp, #24]
	ldr r3, [sp, #56]
	str r5, [sp, #0]
	str r7, [sp, #4]
	ldr r2, [sp, #52]
	ldr r4, [r0, #4]
	subs r3, #24
	ldr r0, [sp, #40]
	ldr r1, [sp, #16]
	bl _call_via_r4
	b .L_080d4fc6
.L_080d4f90:
	ldr r5, .L_080d5080
	ldr r2, [sp, #52]
	ldr r3, [sp, #56]
	add r5, r11
	movs r1, #20
	movs r7, #37
	subs r2, #20
	subs r3, #24
	str r1, [sp, #0]
	ldr r4, [sp, #44]
	adds r1, r5, #0
	str r7, [sp, #4]
	ldr r0, [sp, #40]
	bl _call_via_r4
	movs r0, #20
	str r0, [sp, #0]
	ldr r1, [sp, #24]
	ldr r3, [sp, #56]
	str r7, [sp, #4]
	ldr r2, [sp, #52]
	ldr r4, [r1, #4]
	subs r3, #24
	ldr r0, [sp, #40]
	adds r1, r5, #0
	bl _call_via_r4
.L_080d4fc6:
	ldr r5, [sp, #12]
	ldr r7, [sp, #36]
	ldrb r3, [r5]
	ldr r2, .L_080d507c
	cmp r7, r3
	beq .L_080d4fd8
	adds r3, #16
	cmp r7, r3
	blt .L_080d50b6
.L_080d4fd8:
	movs r0, #0
	ldr r7, .L_080d5084
	mov r10, r0
	mov r8, r0
.L_080d4fe0:
	ldr r3, [r7, #24]
	cmp r3, #0
	bne .L_080d50a6
	bl Random16
	ldr r6, .L_080d5088
	ands r6, r0
	bl Random16
	mov r2, r9
	ldr r3, [r2]
	lsls r3, r3, #8
	str r3, [r7]
	ldr r5, .L_080d508c
	ldr r3, [r2, #4]
	ldr r1, .L_080d5090
	ands r5, r0
	movs r0, #128
	lsls r0, r0, #5
	lsls r3, r3, #8
	adds r5, r5, r1
	adds r3, r3, r0
	str r3, [r7, #4]
	adds r0, r5, #0
	bl Trig_Sin
	adds r6, #32
	adds r3, r6, #0
	muls r3, r0
	asrs r3, r3, #15
	str r3, [r7, #8]
	adds r0, r5, #0
	bl Trig_Cos
	adds r3, r6, #0
	muls r3, r0
	lsls r3, r3, #1
	negs r3, r3
	asrs r3, r3, #15
	str r3, [r7, #16]
	ldr r2, [sp, #12]
	ldr r5, [sp, #36]
	ldrb r3, [r2]
	movs r1, #1
	add r10, r1
	cmp r5, r3
	bne .L_080d5094
	bl Random16
	movs r1, #7
	ands r0, r1
	adds r0, #48
	mov r2, r10
	str r0, [r7, #24]
	cmp r2, #200
	bne .L_080d50a6
	b .L_080d50b4
	.2byte 0x0000
.L_080d5054:
	.4byte 0x00000087
.L_080d5058:
	.4byte IwramCopyWords
.L_080d505c:
	.4byte gWorkSlot
.L_080d5060:
	.4byte gMapCellBuffer + 0x18
.L_080d5064:
	.4byte 0x00007828
.L_080d5068:
	.4byte Data_080ee29d
.L_080d506c:
	.4byte 0x00007784
.L_080d5070:
	.4byte BattlePresentation_ProcessPendingGraphicsTransfer
.L_080d5074:
	.4byte gCameraWork
.L_080d5078:
	.4byte Data_080ee29a
.L_080d507c:
	.4byte Data_080ee2a9
.L_080d5080:
	.4byte 0x000010b4
.L_080d5084:
	.4byte gMapCellBuffer
.L_080d5088:
	.4byte 0x000003ff
.L_080d508c:
	.4byte 0x00007fff
.L_080d5090:
	.4byte 0xffffc000
.L_080d5094:
	bl Random16
	movs r3, #7
	ands r0, r3
	adds r0, #24
	mov r5, r10
	str r0, [r7, #24]
	cmp r5, #4
	beq .L_080d50b4
.L_080d50a6:
	movs r0, #1
	movs r1, #128
	add r8, r0
	lsls r1, r1, #3
	adds r7, #28
	cmp r8, r1
	bne .L_080d4fe0
.L_080d50b4:
	ldr r2, .L_080d5238
.L_080d50b6:
	ldr r5, [sp, #32]
	ldr r7, [sp, #36]
	ldrb r3, [r2, r5]
	cmp r7, r3
	bne .L_080d5108
	ldr r2, .L_080d523c
	movs r3, #2
	add r2, r11
	str r3, [r2]
	ldr r3, .L_080d5240
	mov r1, r11
	ldr r3, [r1, r3]
	ldr r3, [r3, #20]
	movs r0, #0
	mov r8, r0
	cmp r3, #0
	beq .L_080d5108
	ldr r5, .L_080d5240
	movs r6, #36
	add r5, r11
.L_080d50de:
	ldr r3, [r5]
	ldrsh r0, [r3, r6]
	movs r3, #8
	str r3, [sp, #0]
	movs r1, #10
	mov r3, r8
	movs r2, #5
	bl ObjectGroup_UpdateMembers
	ldr r3, [r5]
	movs r1, #1
	ldrsh r0, [r3, r6]
	bl BattleMotion_ApplyVariantMotionFar
	ldr r3, [r5]
	movs r0, #1
	ldr r3, [r3, #20]
	add r8, r0
	adds r6, #2
	cmp r8, r3
	bne .L_080d50de
.L_080d5108:
	ldr r3, [sp, #32]
	ldr r1, [sp, #12]
	ldr r2, [sp, #8]
	adds r3, #1
	str r3, [sp, #32]
	adds r2, #28
	adds r1, #1
	ldr r3, .L_080d5240
	str r2, [sp, #8]
	str r1, [sp, #12]
	add r3, r11
	ldr r3, [r3]
	ldr r2, .L_080d5244
	ldr r3, [r3, #24]
	ldr r5, [sp, #32]
	ldrb r3, [r2, r3]
	cmp r5, r3
	beq .L_080d512e
	b .L_080d4ece
.L_080d512e:
	movs r7, #0
	ldr r6, .L_080d5248
	mov r8, r7
.L_080d5134:
	ldr r5, [r6, #24]
	cmp r5, #0
	ble .L_080d51dc
	subs r3, r5, #1
	ldr r2, [r6, #8]
	str r3, [r6, #24]
	ldr r3, [r6]
	ldr r1, [r6, #16]
	adds r4, r3, r2
	ldr r3, [r6, #4]
	adds r0, r3, r1
	lsls r3, r2, #4
	subs r3, r3, r2
	lsls r3, r3, #2
	str r4, [r6]
	str r0, [r6, #4]
	cmp r3, #0
	bge .L_080d515a
	adds r3, #63
.L_080d515a:
	asrs r3, r3, #6
	str r3, [r6, #8]
	lsls r3, r1, #4
	subs r3, r3, r1
	lsls r3, r3, #2
	cmp r3, #0
	bge .L_080d516a
	adds r3, #63
.L_080d516a:
	asrs r3, r3, #6
	adds r2, r3, #0
	subs r2, #16
	str r2, [r6, #16]
	adds r3, r0, #0
	cmp r0, #0
	bge .L_080d517a
	adds r3, #255
.L_080d517a:
	asrs r3, r3, #8
	mov r12, r3
	cmp r3, #120
	ble .L_080d518e
	negs r3, r2
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	str r3, [r6, #16]
	b .L_080d51dc
.L_080d518e:
	cmp r4, #0
	blt .L_080d51dc
	asrs r7, r4, #8
	cmp r7, #126
	bgt .L_080d51dc
	cmp r0, #0
	blt .L_080d51dc
	adds r2, r5, #0
	subs r2, #17
	cmp r2, #0
	bge .L_080d51a6
	adds r2, #7
.L_080d51a6:
	asrs r5, r2, #3
	cmp r5, #0
	bgt .L_080d51ae
	movs r5, #1
.L_080d51ae:
	lsls r4, r5, #1
	ldr r2, .L_080d524c
	subs r3, r4, #2
	mov r1, r8
	movs r0, #1
	ands r0, r1
	ldrh r1, [r2, r3]
	ldr r2, [sp, #28]
	adds r1, r2, r1
	lsrs r2, r5, #31
	adds r2, r5, r2
	asrs r2, r2, #1
	subs r2, r7, r2
	mov r7, r12
	str r5, [sp, #0]
	subs r3, r7, r5
	str r4, [sp, #4]
	ldr r5, [sp, #24]
	lsls r0, r0, #2
	ldr r4, [r0, r5]
	ldr r0, [sp, #40]
	bl _call_via_r4
.L_080d51dc:
	movs r7, #1
	movs r0, #128
	add r8, r7
	lsls r0, r0, #3
	adds r6, #28
	cmp r8, r0
	bne .L_080d5134
	movs r1, #16
	movs r0, #16
	bl Camera_ApplyShake
	bl ObjectGroup_TickMemberTimers
	ldr r2, .L_080d5250
	movs r3, #1
	add r2, r11
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r1, [sp, #36]
	adds r1, #1
	str r1, [sp, #36]
	cmp r1, #96
	beq .L_080d5210
	b .L_080d4e4c
.L_080d5210:
	movs r0, #47
	bl Runtime_ReleaseHeapBlock
	movs r0, #46
	bl Runtime_ReleaseHeapBlock
	ldr r0, .L_080d5254
	bl Scheduler_RemoveCallback
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
.L_080d5238:
	.4byte Data_080ee2a9
.L_080d523c:
	.4byte 0x000077a8
.L_080d5240:
	.4byte 0x00007828
.L_080d5244:
	.4byte Data_080ee29a
.L_080d5248:
	.4byte gMapCellBuffer
.L_080d524c:
	.4byte ParticleStreams_CellOffsets
.L_080d5250:
	.4byte 0x00007824
.L_080d5254:
	.4byte BattlePresentation_ProcessPendingGraphicsTransfer
