.syntax unified
	.thumb
	.global BattleFx_RunMemberBurst
	.thumb_func
BattleFx_RunMemberBurst:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #84
	ldr r2, .L_080ced70
	str r1, [sp, #60]
	adds r3, r2, #0
	ldmia r3!, {r1}
	ldr r3, [r3]
	str r3, [sp, #56]
	subs r2, #108
	ldr r3, .L_080ced74
	ldr r2, [r2]
	mov r10, r1
	add r3, r10
	str r2, [sp, #44]
	str r0, [r3]
	movs r0, #1
	bl BattleFx_BeginCanvasLayer
	ldr r0, .L_080ced78
	bl Resource_GetTableEntry
	mov r1, r10
	bl Resource_DecodeType01
	ldr r2, [sp, #60]
	cmp r2, #0
	bne .L_080ceb9a
	ldr r0, .L_080ced7c
	b .L_080ceba6
.L_080ceb9a:
	ldr r3, [sp, #60]
	cmp r3, #1
	bne .L_080ceba4
	ldr r0, .L_080ced80
	b .L_080ceba6
.L_080ceba4:
	ldr r0, .L_080ced84
.L_080ceba6:
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	movs r2, #128
	ldr r3, .L_080ced88
	lsls r0, r0, #19
	bl _call_via_r3
	ldr r3, .L_080ced74
	add r3, r10
	ldr r3, [r3]
	add r1, sp, #64
	ldr r0, [r3, #4]
	bl BattleFx_FetchRectangleBlitters
	movs r1, #1
	movs r2, #128
	ldr r3, .L_080ced8c
	movs r7, #0
	negs r1, r1
	lsls r2, r2, #3
.L_080cebd2:
	adds r7, #1
	str r1, [r3]
	adds r3, #28
	cmp r7, r2
	bne .L_080cebd2
	movs r0, #0
	str r0, [sp, #48]
	ldr r2, .L_080ced74
	mov r1, r10
	ldr r3, [r1, r2]
	ldr r3, [r3, #20]
	cmp r3, #0
	beq .L_080cec6c
	movs r3, #255
	mov r9, r3
	mov r11, r0
.L_080cebf2:
	mov r0, r10
	adds r5, r0, r2
	ldr r3, [r5]
	ldr r0, [r3, #8]
	bl GetBattleObjectSlotFar
	ldr r3, [r5]
	ldr r6, [r0]
	ldr r0, [r3, #8]
	bl Func_080b5070
	ldr r5, .L_080ced90
	mov r8, r0
	movs r7, #0
	add r5, r11
.L_080cec10:
	ldr r3, [r6, #8]
	mov r1, r8
	str r1, [r5, #4]
	str r3, [r5]
	ldr r3, [r6, #16]
	str r3, [r5, #8]
	bl Random16
	mov r2, r9
	ands r0, r2
	subs r0, #128
	lsls r0, r0, #10
	str r0, [r5, #12]
	bl Random16
	mov r3, r9
	ands r0, r3
	subs r0, #128
	lsls r0, r0, #10
	str r0, [r5, #16]
	bl Random16
	mov r1, r9
	ands r0, r1
	subs r0, #128
	lsls r0, r0, #10
	movs r3, #0
	adds r7, #1
	str r0, [r5, #20]
	str r3, [r5, #24]
	adds r5, #28
	cmp r7, #128
	bne .L_080cec10
	ldr r3, [sp, #48]
	movs r2, #224
	lsls r2, r2, #4
	adds r3, #1
	str r3, [sp, #48]
	add r11, r2
	ldr r2, .L_080ced74
	mov r0, r10
	ldr r3, [r0, r2]
	ldr r1, [sp, #48]
	ldr r3, [r3, #20]
	cmp r1, r3
	bne .L_080cebf2
.L_080cec6c:
	movs r5, #144
	lsls r5, r5, #3
	adds r1, r5, #0
	ldr r0, .L_080ced94
	bl Scheduler_AddOrUpdateCallback
	movs r2, #239
	lsls r2, r2, #7
	add r2, r10
	movs r3, #2
	str r3, [r2]
	ldr r2, .L_080ced98
	movs r3, #75
	add r2, r10
	str r3, [r2]
	adds r1, r5, #0
	ldr r0, .L_080ced9c
	bl Scheduler_AddOrUpdateCallback
	movs r0, #146
	bl Func_080f9010
	ldr r3, [sp, #60]
	movs r2, #0
	lsls r3, r3, #1
	str r2, [sp, #52]
	str r3, [sp, #28]
	ldr r2, .L_080ceda0
	adds r3, #1
	ldrb r1, [r2, r3]
	ldr r3, .L_080ced74
	add r3, r10
	ldr r3, [r3]
	ldr r2, [r3, #20]
	lsls r3, r2, #2
	adds r3, r3, r2
	lsls r3, r3, #2
	cmn r1, r3
	bne .L_080cecbc
	b .L_080cef14
.L_080cecbc:
	ldr r0, [sp, #44]
	adds r0, #12
	str r0, [sp, #24]
.L_080cecc2:
	ldr r1, [sp, #52]
	cmp r1, #80
	bne .L_080cecdc
	ldr r2, [sp, #60]
	cmp r2, #0
	bne .L_080cecd6
	movs r0, #134
	bl BattleEventRuntime_BeginPhaseFar
	b .L_080cecdc
.L_080cecd6:
	movs r0, #133
	bl BattleEventRuntime_BeginPhaseFar
.L_080cecdc:
	bl Render_ResetTransformState
	ldr r0, [sp, #44]
	ldr r1, [sp, #24]
	bl Graphics_PrepareTransferInIwramWork
	movs r6, #211
	ldr r0, [sp, #52]
	lsls r6, r6, #7
	movs r3, #128
	add r6, r10
	movs r7, #0
	lsls r3, r3, #13
	lsls r5, r0, #10
.L_080cecf8:
	adds r0, r5, #0
	str r3, [sp, #8]
	bl Trig_Sin
	ldr r3, [sp, #8]
	lsls r0, r0, #4
	subs r0, r3, r0
	movs r1, #128
	asrs r0, r0, #10
	lsls r1, r1, #3
	adds r7, #1
	stmia r6!, {r0}
	adds r5, r5, r1
	cmp r7, #160
	bne .L_080cecf8
	movs r2, #0
	str r2, [sp, #48]
	ldr r2, .L_080ced74
	mov r0, r10
	ldr r3, [r0, r2]
	ldr r3, [r3, #20]
	cmp r3, #0
	bne .L_080ced28
	b .L_080ceedc
.L_080ced28:
	movs r1, #0
	movs r3, #36
	str r1, [sp, #20]
	str r3, [sp, #16]
	str r1, [sp, #12]
	mov r11, r1
.L_080ced34:
	mov r0, r10
	adds r5, r0, r2
	ldr r1, [sp, #16]
	ldr r3, [r5]
	ldrsh r0, [r3, r1]
	bl GetBattleObjectSlotFar
	ldr r3, [r5]
	ldr r0, [r0]
	ldr r1, [sp, #16]
	mov r9, r0
	ldrsh r0, [r3, r1]
	bl Func_080b5070
	lsrs r3, r0, #31
	adds r0, r0, r3
	asrs r0, r0, #1
	str r0, [sp, #40]
	mov r3, r11
	ldr r0, [sp, #52]
	adds r3, #71
	cmp r0, r3
	bne .L_080cedaa
	ldr r1, [sp, #60]
	cmp r1, #0
	bne .L_080ceda4
	movs r0, #134
	bl Func_080f9010
	b .L_080cedaa
.L_080ced70:
	.4byte gBattleFxWork
.L_080ced74:
	.4byte 0x00007828
.L_080ced78:
	.4byte 0x00000069
.L_080ced7c:
	.4byte 0x000000bb
.L_080ced80:
	.4byte 0x0000008d
.L_080ced84:
	.4byte 0x00000091
.L_080ced88:
	.4byte IwramCopyWords
.L_080ced8c:
	.4byte Data_02010018
.L_080ced90:
	.4byte gMapCellBuffer
.L_080ced94:
	.4byte BattleFx_ArmBg2AffineHBlankDma
.L_080ced98:
	.4byte 0x00007784
.L_080ced9c:
	.4byte BattlePresentation_ProcessPendingGraphicsTransfer
.L_080ceda0:
	.4byte Data_080ee090
.L_080ceda4:
	movs r0, #133
	bl Func_080f9010
.L_080cedaa:
	mov r3, r11
	ldr r2, [sp, #52]
	adds r3, #70
	cmp r2, r3
	bne .L_080cedcc
	ldr r3, .L_080cef44
	add r3, r10
	ldr r3, [r3]
	ldr r1, [sp, #16]
	ldrsh r0, [r3, r1]
	movs r3, #26
	str r3, [sp, #0]
	movs r1, #7
	movs r2, #5
	ldr r3, [sp, #48]
	bl ObjectGroup_UpdateMembers
.L_080cedcc:
	ldr r3, [sp, #52]
	cmp r3, r11
	ble .L_080ceeac
	ldr r2, .L_080cef48
	ldr r0, [sp, #28]
	ldrb r3, [r2, r0]
	movs r7, #0
	cmp r3, #0
	beq .L_080ceeac
	ldr r1, [sp, #20]
	add r3, sp, #72
	str r1, [sp, #36]
	mov r8, r3
	mov r0, r11
	ldr r1, [sp, #12]
	ldr r3, .L_080cef4c
	str r0, [sp, #32]
	adds r6, r1, r3
.L_080cedf0:
	ldr r0, [sp, #36]
	lsls r3, r0, #1
	adds r3, r3, r7
	ldr r1, [sp, #52]
	lsls r3, r3, #1
	cmp r1, r3
	ble .L_080ceea0
	ldr r3, [r6, #24]
	cmp r3, #0
	blt .L_080ceea0
	mov r1, r8
	adds r0, r6, #0
	bl EffectPosition_ApplyBaseAndYOffset
	mov r2, r8
	ldr r5, [r2]
	asrs r5, r5, #1
	str r5, [r2]
	movs r1, #3
	adds r0, r7, #0
	bl Func_080022fc
	lsls r1, r0, #2
	adds r1, r1, r0
	mov r0, r8
	ldr r3, [r0, #4]
	movs r2, #20
	str r2, [sp, #0]
	lsls r1, r1, #7
	movs r2, #32
	subs r5, #10
	subs r3, #16
	str r2, [sp, #4]
	add r1, r10
	ldr r4, [sp, #64]
	ldr r0, [sp, #56]
	adds r2, r5, #0
	bl _call_via_r4
	movs r1, #62
	movs r2, #0
	adds r0, r6, #0
	bl EffectStep_AdvanceWithGravity3D
	ldr r1, [sp, #32]
	ldr r2, [sp, #52]
	adds r3, r1, r7
	adds r3, #30
	cmp r2, r3
	ble .L_080cee9e
	mov r3, r9
	ldr r0, [r3, #8]
	ldr r3, [r6]
	mov r2, r9
	ldr r1, [r2, #12]
	subs r0, r0, r3
	ldr r3, [sp, #40]
	adds r1, r1, r3
	ldr r3, [r6, #4]
	ldr r2, [r2, #16]
	subs r1, r1, r3
	ldr r3, [r6, #8]
	subs r2, r2, r3
	ldr r3, [r6, #12]
	asrs r0, r0, #9
	adds r3, r3, r0
	str r3, [r6, #12]
	ldr r3, [r6, #16]
	asrs r1, r1, #9
	adds r3, r3, r1
	ldr r1, .L_080cef50
	str r3, [r6, #16]
	ldr r3, [r6, #20]
	asrs r2, r2, #9
	adds r0, r0, r1
	ldr r1, .L_080cef54
	adds r3, r3, r2
	str r3, [r6, #20]
	cmp r0, r1
	bhi .L_080cee9e
	ldr r0, .L_080cef50
	adds r3, r2, r0
	cmp r3, r1
	bhi .L_080cee9e
	movs r3, #1
	negs r3, r3
	str r3, [r6, #24]
.L_080cee9e:
	ldr r2, .L_080cef48
.L_080ceea0:
	ldr r1, [sp, #28]
	ldrb r3, [r2, r1]
	adds r7, #1
	adds r6, #28
	cmp r7, r3
	bne .L_080cedf0
.L_080ceeac:
	ldr r2, [sp, #20]
	ldr r3, [sp, #16]
	adds r2, #5
	adds r3, #2
	str r2, [sp, #20]
	str r3, [sp, #16]
	ldr r1, [sp, #12]
	ldr r3, [sp, #48]
	movs r2, #224
	lsls r2, r2, #4
	adds r1, r1, r2
	adds r3, #1
	str r1, [sp, #12]
	str r3, [sp, #48]
	movs r0, #20
	ldr r2, .L_080cef44
	add r11, r0
	mov r0, r10
	ldr r3, [r0, r2]
	ldr r1, [sp, #48]
	ldr r3, [r3, #20]
	cmp r1, r3
	beq .L_080ceedc
	b .L_080ced34
.L_080ceedc:
	bl ObjectGroup_TickMemberTimers
	ldr r2, .L_080cef58
	movs r3, #1
	add r2, r10
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r2, [sp, #52]
	ldr r3, [sp, #28]
	adds r2, #1
	str r2, [sp, #52]
	ldr r2, .L_080cef48
	adds r3, #1
	ldrb r1, [r2, r3]
	ldr r3, .L_080cef44
	add r3, r10
	ldr r3, [r3]
	ldr r2, [r3, #20]
	lsls r3, r2, #2
	adds r3, r3, r2
	lsls r3, r3, #2
	adds r1, r1, r3
	ldr r3, [sp, #52]
	cmp r3, r1
	beq .L_080cef14
	b .L_080cecc2
.L_080cef14:
	ldr r0, .L_080cef5c
	bl Scheduler_RemoveCallback
	movs r0, #47
	bl Runtime_ReleaseHeapBlock
	movs r0, #46
	bl Runtime_ReleaseHeapBlock
	ldr r0, .L_080cef60
	bl Scheduler_RemoveCallback
	bl BattleFx_EndCanvasLayer
	add sp, #84
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
.L_080cef44:
	.4byte 0x00007828
.L_080cef48:
	.4byte Data_080ee090
.L_080cef4c:
	.4byte gMapCellBuffer
.L_080cef50:
	.4byte 0x00000fff
.L_080cef54:
	.4byte 0x00001ffe
.L_080cef58:
	.4byte 0x00007824
.L_080cef5c:
	.4byte BattlePresentation_ProcessPendingGraphicsTransfer
.L_080cef60:
	.4byte BattleFx_ArmBg2AffineHBlankDma
