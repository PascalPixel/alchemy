.syntax unified
	.thumb
	.global Unnamed_080d85d0
	.thumb_func
Unnamed_080d85d0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r2, .L_080d8908
	adds r3, r2, #0
	ldmia r3!, {r1}
	ldr r3, [r3]
	sub sp, #64
	str r3, [sp, #48]
	ldr r3, [r2, #8]
	str r3, [sp, #40]
	subs r2, #108
	ldr r2, [r2]
	str r2, [sp, #36]
	ldr r3, [r0, #24]
	negs r5, r3
	orrs r5, r3
	ldr r3, .L_080d890c
	mov r9, r1
	add r3, r9
	str r0, [r3]
	movs r0, #1
	bl BattleFx_BeginCanvasLayer
	lsrs r5, r5, #31
	ldr r0, .L_080d8910
	ldr r1, [sp, #40]
	movs r2, #0
	movs r3, #0
	bl Resource_LoadAndDecompress
	cmp r5, #0
	bne .L_080d861e
	ldr r0, .L_080d8914
	b .L_080d8620
.L_080d861e:
	ldr r0, .L_080d8918
.L_080d8620:
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_080d891c
	movs r2, #128
	lsls r0, r0, #19
	bl _call_via_r3
	movs r1, #1
	movs r2, #128
	ldr r3, .L_080d8920
	movs r7, #0
	negs r1, r1
	lsls r2, r2, #3
.L_080d863e:
	adds r7, #1
	str r1, [r3]
	adds r3, #28
	cmp r7, r2
	bne .L_080d863e
	ldr r2, .L_080d890c
	mov r0, r9
	ldr r3, [r0, r2]
	ldr r3, [r3, #20]
	movs r4, #0
	mov r8, r4
	cmp r3, #0
	beq .L_080d86f0
	movs r3, #36
	movs r1, #255
	str r3, [sp, #24]
	str r4, [sp, #16]
	mov r11, r1
.L_080d8662:
	mov r4, r9
	adds r5, r4, r2
	ldr r3, [r5]
	ldr r1, [sp, #24]
	ldrsh r0, [r3, r1]
	bl GetBattleObjectSlotFar
	ldr r3, [r5]
	ldr r4, [sp, #24]
	ldr r6, [r0]
	ldrsh r0, [r3, r4]
	bl Battle_GetObjectTableValueFar
	lsrs r3, r0, #31
	adds r0, r0, r3
	ldr r2, [sp, #16]
	ldr r3, .L_080d8924
	asrs r0, r0, #1
	mov r10, r0
	movs r7, #0
	adds r5, r2, r3
.L_080d868c:
	ldr r3, [r6, #8]
	str r3, [r5]
	ldr r3, [r6, #12]
	add r3, r10
	str r3, [r5, #4]
	ldr r3, [r6, #16]
	str r3, [r5, #8]
	bl Random16
	mov r4, r11
	ands r0, r4
	subs r0, #128
	lsls r0, r0, #10
	str r0, [r5, #12]
	bl Random16
	mov r1, r11
	ands r0, r1
	subs r0, #128
	lsls r0, r0, #10
	str r0, [r5, #16]
	bl Random16
	mov r2, r11
	ands r0, r2
	subs r0, #128
	lsls r0, r0, #10
	movs r3, #0
	adds r7, #1
	str r0, [r5, #20]
	str r3, [r5, #24]
	adds r5, #28
	cmp r7, #128
	bne .L_080d868c
	ldr r3, [sp, #24]
	ldr r4, [sp, #16]
	movs r0, #224
	lsls r0, r0, #4
	adds r3, #2
	adds r4, r4, r0
	str r4, [sp, #16]
	str r3, [sp, #24]
	ldr r2, .L_080d890c
	mov r4, r9
	ldr r3, [r4, r2]
	movs r1, #1
	ldr r3, [r3, #20]
	add r8, r1
	cmp r8, r3
	bne .L_080d8662
.L_080d86f0:
	movs r3, #2
	str r3, [sp, #0]
	movs r1, #7
	movs r2, #7
	movs r3, #3
	movs r0, #46
	bl Unnamed_080ed408
	ldr r3, .L_080d8928
	adds r3, #184
	ldr r3, [r3]
	movs r2, #239
	lsls r2, r2, #7
	str r3, [sp, #44]
	add r2, r9
	movs r3, #3
	str r3, [r2]
	ldr r2, .L_080d892c
	ldr r3, .L_080d8930
	add r2, r9
	movs r1, #144
	str r3, [r2]
	lsls r1, r1, #3
	ldr r0, .L_080d8934
	bl Scheduler_AddOrUpdateCallback
	movs r0, #142
	bl AudioCommand_PlayFar
	ldr r1, .L_080d890c
	mov r2, r9
	ldr r3, [r2, r1]
	ldr r2, [r3, #20]
	lsls r3, r2, #2
	adds r3, r3, r2
	movs r4, #72
	movs r0, #0
	lsls r3, r3, #2
	negs r4, r4
	mov r11, r0
	cmp r3, r4
	bne .L_080d8746
	b .L_080d88e4
.L_080d8746:
	ldr r0, [sp, #36]
	adds r0, #12
	str r0, [sp, #28]
.L_080d874c:
	mov r2, r9
	adds r5, r2, r1
	ldr r3, [r5]
	ldr r0, [r3, #8]
	bl GetBattleObjectSlotFar
	ldr r3, [r5]
	ldr r0, [r0]
	mov r10, r0
	ldr r0, [r3, #8]
	bl Battle_GetObjectTableValueFar
	lsrs r3, r0, #31
	adds r0, r0, r3
	asrs r0, r0, #1
	mov r3, r11
	str r0, [sp, #32]
	cmp r3, #64
	bne .L_080d8778
	movs r0, #133
	bl BattleEventRuntime_BeginPhaseFar
.L_080d8778:
	bl Render_ResetTransformState
	ldr r0, [sp, #36]
	ldr r1, [sp, #28]
	bl Graphics_PrepareTransferInIwramWork
	mov r4, r11
	cmp r4, #40
	bne .L_080d879e
	ldr r3, [r5]
	movs r2, #1
	ldr r0, [r3, #8]
	movs r3, #0
	str r3, [sp, #0]
	movs r1, #7
	negs r2, r2
	subs r3, #1
	bl ObjectGroup_UpdateMembers
.L_080d879e:
	ldr r1, [r5]
	ldr r2, [r1, #20]
	lsls r3, r2, #2
	adds r3, r3, r2
	lsls r3, r3, #2
	adds r3, #52
	cmp r11, r3
	bne .L_080d87c2
	movs r3, #0
	movs r2, #1
	ldr r0, [r1, #8]
	str r3, [sp, #0]
	movs r1, #0
	negs r2, r2
	subs r3, #1
	bl ObjectGroup_UpdateMembers
	ldr r1, [r5]
.L_080d87c2:
	ldr r3, [r1, #20]
	movs r0, #0
	mov r8, r0
	cmp r3, #0
	beq .L_080d88b8
	str r0, [sp, #8]
	str r0, [sp, #20]
.L_080d87d0:
	ldr r1, [sp, #8]
	cmp r11, r1
	bne .L_080d87f2
	ldr r3, .L_080d890c
	mov r4, r8
	add r3, r9
	ldr r2, [r3]
	lsls r3, r4, #1
	adds r3, #36
	ldrsh r0, [r2, r3]
	movs r3, #42
	str r3, [sp, #0]
	movs r1, #7
	movs r2, #5
	mov r3, r8
	bl ObjectGroup_UpdateMembers
.L_080d87f2:
	ldr r2, [sp, #8]
	cmp r11, r2
	ble .L_080d8898
	ldr r3, [sp, #20]
	ldr r4, .L_080d8924
	movs r7, #0
	add r6, sp, #52
	adds r5, r3, r4
.L_080d8802:
	ldr r3, [r5, #24]
	cmp r3, #0
	blt .L_080d8890
	adds r1, r6, #0
	adds r0, r5, #0
	bl EffectPosition_ApplyBaseAndYOffset
	ldr r2, [r6]
	ldr r3, .L_080d8938
	asrs r2, r2, #1
	str r2, [r6]
	ldrh r1, [r3, #10]
	ldr r0, [sp, #40]
	ldr r3, [r6, #4]
	adds r1, r0, r1
	movs r4, #6
	movs r0, #12
	subs r3, #6
	str r4, [sp, #0]
	str r0, [sp, #4]
	subs r2, #3
	ldr r0, [sp, #48]
	ldr r4, [sp, #44]
	bl _call_via_r4
	adds r0, r5, #0
	movs r1, #62
	movs r2, #0
	bl EffectStep_AdvanceWithGravity3D
	ldr r0, [sp, #8]
	adds r3, r0, r7
	adds r3, #10
	cmp r11, r3
	ble .L_080d8890
	mov r1, r10
	ldr r0, [r1, #8]
	ldr r3, [r5]
	ldr r2, [sp, #32]
	ldr r1, [r1, #12]
	subs r0, r0, r3
	ldr r3, [r5, #4]
	adds r1, r1, r2
	subs r1, r1, r3
	mov r3, r10
	ldr r2, [r3, #16]
	ldr r3, [r5, #8]
	subs r2, r2, r3
	ldr r3, [r5, #12]
	asrs r0, r0, #8
	adds r3, r3, r0
	str r3, [r5, #12]
	ldr r3, [r5, #16]
	asrs r1, r1, #8
	adds r3, r3, r1
	str r3, [r5, #16]
	ldr r4, .L_080d893c
	ldr r3, [r5, #20]
	asrs r2, r2, #8
	ldr r1, .L_080d8940
	adds r3, r3, r2
	adds r0, r0, r4
	str r3, [r5, #20]
	cmp r0, r1
	bhi .L_080d8890
	adds r3, r2, r4
	cmp r3, r1
	bhi .L_080d8890
	movs r0, #1
	negs r0, r0
	str r0, [r5, #24]
.L_080d8890:
	adds r7, #1
	adds r5, #28
	cmp r7, #32
	bne .L_080d8802
.L_080d8898:
	ldr r1, [sp, #8]
	ldr r2, [sp, #20]
	movs r3, #224
	lsls r3, r3, #4
	adds r2, r2, r3
	adds r1, #20
	ldr r3, .L_080d890c
	str r1, [sp, #8]
	str r2, [sp, #20]
	add r3, r9
	ldr r3, [r3]
	movs r4, #1
	ldr r3, [r3, #20]
	add r8, r4
	cmp r8, r3
	bne .L_080d87d0
.L_080d88b8:
	bl ObjectGroup_TickMemberTimers
	ldr r2, .L_080d8944
	movs r3, #1
	add r2, r9
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r1, .L_080d890c
	mov r2, r9
	ldr r3, [r2, r1]
	ldr r2, [r3, #20]
	lsls r3, r2, #2
	adds r3, r3, r2
	movs r0, #1
	lsls r3, r3, #2
	add r11, r0
	adds r3, #72
	cmp r11, r3
	beq .L_080d88e4
	b .L_080d874c
.L_080d88e4:
	ldr r0, .L_080d8934
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
.L_080d8908:
	.4byte gBattleFxWork
.L_080d890c:
	.4byte 0x00007828
.L_080d8910:
	.4byte 0x00000073
.L_080d8914:
	.4byte 0x000000b9
.L_080d8918:
	.4byte 0x000000c0
.L_080d891c:
	.4byte IwramCopyWords
.L_080d8920:
	.4byte gMapCellBuffer + 0x18
.L_080d8924:
	.4byte gMapCellBuffer
.L_080d8928:
	.4byte gWorkSlot
.L_080d892c:
	.4byte 0x00007784
.L_080d8930:
	.4byte 0x04040404
.L_080d8934:
	.4byte BattlePresentation_ProcessPendingGraphicsTransfer
.L_080d8938:
	.4byte ParticleStreams_CellOffsets
.L_080d893c:
	.4byte 0x00000fff
.L_080d8940:
	.4byte 0x00001ffe
.L_080d8944:
	.4byte 0x00007824
