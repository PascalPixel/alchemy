.syntax unified
	.thumb
	.global Unnamed_080cb7f8
	.thumb_func
Unnamed_080cb7f8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	adds r6, r0, #0
	ldr r1, .L_080cb888
	movs r0, #39
	sub sp, #76
	bl Runtime_AllocateHeapBlock
	movs r1, #128
	mov r9, r0
	lsls r1, r1, #7
	movs r0, #40
	bl Runtime_AllocateHeapBlock
	ldr r1, .L_080cb88c
	str r0, [sp, #36]
	movs r0, #41
	bl Runtime_AllocateHeapBlock
	ldr r3, .L_080cb890
	str r0, [sp, #24]
	ldr r5, .L_080cb894
	ldr r3, [r3]
	add r5, r9
	str r3, [sp, #20]
	movs r0, #0
	str r6, [r5]
	bl BattleFx_BeginCanvasLayer
	ldr r2, .L_080cb898
	movs r3, #24
	add r2, r9
	str r3, [r2]
	ldr r2, .L_080cb89c
	movs r3, #0
	add r2, r9
	str r3, [r2]
	ldr r2, .L_080cb8a0
	ldr r3, .L_080cb880
	ldr r6, .L_080cb8a4
	strh r3, [r2]
	ldr r3, .L_080cb884
	subs r2, #50
	strh r3, [r2]
	adds r0, r6, #0
	mov r1, r9
	movs r2, #1
	movs r3, #0
	bl Resource_LoadAndDecompress
	movs r3, #0
	ldr r0, .L_080cb8a8
	ldr r1, [sp, #24]
	movs r2, #0
	bl Resource_LoadAndDecompress
	ldr r3, [r5]
	ldr r3, [r3]
	cmp r3, #1
	beq .L_080cb8bc
	cmp r3, #1
	bgt .L_080cb8b2
	b .L_080cb8ac
.L_080cb880:
	.4byte 0x0000100c
.L_080cb884:
	.4byte 0x00000100
.L_080cb888:
	.4byte 0x0000782c
.L_080cb88c:
	.4byte 0x0000060e
.L_080cb890:
	.4byte gCameraWork
.L_080cb894:
	.4byte 0x00007828
.L_080cb898:
	.4byte 0x000077b4
.L_080cb89c:
	.4byte 0x000077b8
.L_080cb8a0:
	.4byte 0x04000052
.L_080cb8a4:
	.4byte 0x00000057
.L_080cb8a8:
	.4byte 0x00000076
.L_080cb8ac:
	cmp r3, #0
	beq .L_080cb8b8
	b .L_080cb8cc
.L_080cb8b2:
	cmp r3, #2
	beq .L_080cb8c0
	b .L_080cb8cc
.L_080cb8b8:
	ldr r0, .L_080cb8c4
	b .L_080cb8ce
.L_080cb8bc:
	adds r0, r6, #0
	b .L_080cb8ce
.L_080cb8c0:
	ldr r0, .L_080cb8c8
	b .L_080cb8ce
.L_080cb8c4:
	.4byte 0x00000048
.L_080cb8c8:
	.4byte 0x00000047
.L_080cb8cc:
	ldr r0, .L_080cb9dc
.L_080cb8ce:
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_080cb9e0
	lsls r0, r0, #19
	movs r2, #128
	bl _call_via_r3
	ldr r5, .L_080cb9e4
	movs r1, #0
	mov r8, r1
.L_080cb8e6:
	movs r3, #0
	str r3, [r5, #4]
	bl Random16
	ldr r3, .L_080cb9e8
	ands r3, r0
	str r3, [r5]
	bl Random16
	ldr r3, .L_080cb9ec
	mov r4, r8
	lsls r2, r4, #1
	ands r3, r0
	movs r1, #1
	adds r3, r3, r2
	add r8, r1
	str r3, [r5, #8]
	mov r2, r8
	negs r3, r4
	str r3, [r5, #24]
	adds r5, #28
	cmp r2, #128
	bne .L_080cb8e6
	movs r2, #239
	lsls r2, r2, #7
	add r2, r9
	movs r3, #2
	str r3, [r2]
	ldr r2, .L_080cb9f0
	movs r3, #75
	add r2, r9
	movs r1, #144
	str r3, [r2]
	lsls r1, r1, #3
	ldr r0, .L_080cb9f4
	bl Scheduler_AddOrUpdateCallback
	movs r3, #3
	movs r1, #7
	movs r2, #7
	movs r0, #46
	str r3, [sp, #0]
	bl Unnamed_080ed408
	ldr r3, .L_080cb9f8
	ldr r1, .L_080cb9fc
	adds r3, #184
	add r1, r9
	ldr r3, [r3]
	ldr r2, [r1]
	str r3, [sp, #28]
	ldr r3, [r2, #24]
	adds r3, #1
	str r3, [r2, #24]
	cmp r3, #0
	bgt .L_080cb95a
	movs r3, #1
	str r3, [r2, #24]
.L_080cb95a:
	ldr r2, [r1]
	ldr r3, [r2, #24]
	cmp r3, #4
	ble .L_080cb966
	movs r3, #4
	str r3, [r2, #24]
.L_080cb966:
	movs r0, #212
	bl AudioCommand_PlayFar
	ldr r2, .L_080cb9fc
	ldr r1, [sp, #20]
	mov r4, sp
	adds r4, #64
	adds r1, #12
	add r2, r9
	movs r3, #0
	str r4, [sp, #8]
	str r1, [sp, #12]
	str r2, [sp, #16]
	mov r10, r3
.L_080cb982:
	ldr r4, [sp, #16]
	ldr r3, [r4]
	ldr r1, [sp, #8]
	ldr r0, [r3, #8]
	bl EffectPosition_ApplyAlternateStepAndYOffset
	ldr r1, [sp, #8]
	ldr r3, [r1]
	movs r4, #64
	ldr r2, .L_080cba00
	subs r3, r4, r3
	lsls r3, r3, #8
	mov r1, r10
	str r3, [r2]
	cmp r1, #49
	ble .L_080cb9b2
	mov r3, r10
	lsls r2, r3, #1
	ldr r3, .L_080cb9d4
	subs r3, r3, r2
	ldr r2, .L_080cb9d8
	ldr r1, .L_080cba04
	orrs r3, r2
	strh r3, [r1]
.L_080cb9b2:
	mov r4, r10
	cmp r4, #16
	bne .L_080cba08
	ldr r1, [sp, #16]
	ldr r3, [r1]
	movs r2, #36
	ldrsh r0, [r3, r2]
	movs r3, #20
	movs r2, #1
	str r3, [sp, #0]
	movs r1, #7
	negs r2, r2
	movs r3, #0
	bl ObjectGroup_UpdateMembers
	b .L_080cba08
	.2byte 0x0000
.L_080cb9d4:
	.4byte 0x00000070
.L_080cb9d8:
	.4byte 0x00001000
.L_080cb9dc:
	.4byte 0x00000046
.L_080cb9e0:
	.4byte IwramCopyWords
.L_080cb9e4:
	.4byte gMapCellBuffer
.L_080cb9e8:
	.4byte 0x0000ffff
.L_080cb9ec:
	.4byte 0x000001ff
.L_080cb9f0:
	.4byte 0x00007784
.L_080cb9f4:
	.4byte BattlePresentation_ProcessPendingGraphicsTransfer
.L_080cb9f8:
	.4byte gWorkSlot
.L_080cb9fc:
	.4byte 0x00007828
.L_080cba00:
	.4byte 0x04000028
.L_080cba04:
	.4byte 0x04000052
.L_080cba08:
	mov r3, r10
	cmp r3, #55
	bgt .L_080cbaf4
	lsrs r3, r3, #31
	add r3, r10
	asrs r3, r3, #1
	mov r8, r3
	mov r0, r8
	cmp r0, #0
	bge .L_080cba1e
	adds r0, #3
.L_080cba1e:
	asrs r7, r0, #2
	mov r11, r7
	mov r4, r11
	lsls r3, r4, #2
	mov r1, r8
	movs r2, #2
	subs r7, r1, r3
	str r2, [sp, #0]
	movs r1, #7
	movs r2, #7
	movs r3, #3
	movs r0, #47
	bl Unnamed_080ed408
	ldr r3, .L_080cbbf8
	ldr r4, [sp, #8]
	lsls r1, r7, #4
	ldr r6, [r3]
	adds r1, r1, r7
	ldr r3, [r4, #4]
	movs r2, #17
	movs r4, #64
	lsls r1, r1, #6
	str r2, [sp, #0]
	str r6, [sp, #32]
	add r1, r9
	subs r3, #64
	str r4, [sp, #4]
	ldr r0, [sp, #36]
	movs r2, #47
	bl _call_via_r6
	mov r0, r10
	cmp r0, #0
	bge .L_080cba66
	adds r0, #3
.L_080cba66:
	movs r1, #3
	asrs r0, r0, #2
	bl __modsi3
	adds r7, r0, #0
	lsls r5, r7, #7
	ldr r2, [sp, #8]
	adds r5, r5, r7
	lsls r5, r5, #3
	movs r1, #136
	ldr r3, [r2, #4]
	lsls r1, r1, #5
	add r5, r9
	movs r4, #24
	adds r5, r5, r1
	movs r1, #43
	str r4, [sp, #0]
	str r1, [sp, #4]
	ldr r0, [sp, #36]
	subs r3, #36
	adds r1, r5, #0
	movs r2, #40
	bl _call_via_r6
	movs r0, #47
	bl Runtime_ReleaseHeapBlock
	movs r2, #2
	str r2, [sp, #0]
	movs r1, #7
	movs r2, #7
	movs r3, #7
	movs r0, #47
	bl Unnamed_080ed408
	ldr r3, .L_080cbbf8
	mov r4, r11
	ldr r6, [r3]
	mov r1, r8
	lsls r3, r4, #2
	ldr r2, [sp, #8]
	subs r7, r1, r3
	lsls r1, r7, #4
	ldr r3, [r2, #4]
	adds r1, r1, r7
	movs r4, #17
	movs r2, #64
	lsls r1, r1, #6
	str r4, [sp, #0]
	str r2, [sp, #4]
	str r6, [sp, #32]
	add r1, r9
	subs r3, #64
	ldr r0, [sp, #36]
	bl _call_via_r6
	ldr r4, [sp, #8]
	ldr r3, [r4, #4]
	movs r1, #24
	movs r2, #43
	str r1, [sp, #0]
	str r2, [sp, #4]
	subs r3, #36
	ldr r0, [sp, #36]
	adds r1, r5, #0
	movs r2, #64
	bl _call_via_r6
	movs r0, #47
	bl Runtime_ReleaseHeapBlock
.L_080cbaf4:
	ldr r4, [sp, #16]
	ldr r3, [r4]
	ldr r0, [r3, #8]
	bl GetBattleObjectSlotFar
	bl Render_ResetTransformState
	ldr r1, [sp, #12]
	ldr r0, [sp, #20]
	bl Graphics_PrepareTransferInIwramWork
	ldr r5, .L_080cbbfc
	movs r1, #0
	mov r8, r1
	add r7, sp, #40
	add r6, sp, #52
.L_080cbb14:
	ldr r3, [r5, #24]
	cmp r3, #0
	blt .L_080cbb96
	ldr r0, [r5]
	bl Trig_Sin
	ldr r3, [r5, #8]
	muls r3, r0
	asrs r3, r3, #4
	str r3, [r7]
	ldr r0, [r5]
	bl Trig_Cos
	ldr r3, [r5, #8]
	muls r3, r0
	asrs r3, r3, #4
	negs r3, r3
	str r3, [r7, #8]
	ldr r3, [r5, #4]
	str r3, [r7, #4]
	movs r2, #128
	ldr r3, [r5]
	lsls r2, r2, #3
	adds r3, r3, r2
	str r3, [r5]
	movs r4, #160
	ldr r3, [r5, #4]
	lsls r4, r4, #11
	adds r3, r3, r4
	str r3, [r5, #4]
	ldr r3, [r5, #8]
	adds r3, #64
	str r3, [r5, #8]
	adds r1, r6, #0
	adds r0, r7, #0
	bl EffectPosition_ApplyBaseAndYOffset
	ldr r2, [r6]
	lsrs r3, r2, #31
	adds r2, r2, r3
	asrs r2, r2, #1
	str r2, [r6]
	ldr r4, [sp, #16]
	ldr r3, [r4]
	mov r1, r8
	ldr r3, [r3, #24]
	movs r0, #1
	ands r0, r1
	adds r0, r0, r3
	lsls r4, r0, #1
	ldr r1, .L_080cbc00
	subs r3, r4, #2
	ldrh r1, [r1, r3]
	ldr r3, [sp, #24]
	adds r1, r3, r1
	ldr r3, [r6, #4]
	subs r2, r2, r0
	subs r3, r3, r0
	str r4, [sp, #0]
	str r4, [sp, #4]
	ldr r0, [sp, #36]
	ldr r4, [sp, #28]
	bl _call_via_r4
	ldr r3, [r5, #24]
.L_080cbb96:
	movs r1, #1
	add r8, r1
	adds r3, #1
	mov r2, r8
	str r3, [r5, #24]
	adds r5, #28
	cmp r2, #32
	bne .L_080cbb14
	bl ObjectGroup_TickMemberTimers
	ldr r2, .L_080cbc04
	movs r3, #1
	add r2, r9
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	movs r3, #1
	add r10, r3
	mov r4, r10
	cmp r4, #56
	beq .L_080cbbc4
	b .L_080cb982
.L_080cbbc4:
	movs r0, #46
	bl Runtime_ReleaseHeapBlock
	ldr r0, .L_080cbc08
	bl Scheduler_RemoveCallback
	bl BattleFx_EndCanvasLayer
	movs r0, #41
	bl Runtime_ReleaseHeapBlock
	movs r0, #40
	bl Runtime_ReleaseHeapBlock
	movs r0, #39
	bl Runtime_ReleaseHeapBlock
	add sp, #76
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
.L_080cbbf8:
	.4byte gTransitionWork + 0xc
.L_080cbbfc:
	.4byte gMapCellBuffer
.L_080cbc00:
	.4byte BattleFx6_FlareCells
.L_080cbc04:
	.4byte 0x00007824
.L_080cbc08:
	.4byte BattlePresentation_ProcessPendingGraphicsTransfer
