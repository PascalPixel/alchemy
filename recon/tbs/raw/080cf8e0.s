.syntax unified
	.thumb
	.global BattleFx_RunSevenMode
	.thumb_func
BattleFx_RunSevenMode:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #80
	ldr r2, .L_080cf958
	str r0, [sp, #56]
	adds r3, r2, #0
	ldmia r3!, {r0}
	ldr r3, [r3]
	str r3, [sp, #52]
	subs r2, #108
	ldr r3, .L_080cf95c
	mov r11, r0
	ldr r2, [r2]
	mov r8, r1
	ldr r1, [sp, #56]
	add r3, r11
	str r2, [sp, #36]
	movs r0, #1
	str r1, [r3]
	bl BattleFx_BeginCanvasLayer
	ldr r3, .L_080cf954
	ldr r2, .L_080cf960
	ldr r7, .L_080cf964
	strh r3, [r2]
	adds r0, r7, #0
	bl Resource_GetTableEntry
	adds r6, r0, #0
	movs r0, #160
	movs r2, #128
	adds r1, r6, #0
	ldr r5, .L_080cf968
	adds r6, #128
	lsls r0, r0, #19
	bl _call_via_r5
	mov r1, r11
	adds r0, r6, #0
	bl Resource_DecodeType01
	ldr r0, .L_080cf96c
	bl Resource_GetTableEntry
	adds r6, r0, #0
	movs r0, #160
	adds r1, r6, #0
	movs r2, #128
	lsls r0, r0, #19
	bl _call_via_r5
	b .L_080cf970
	.2byte 0x0000
.L_080cf954:
	.4byte 0x00001010
.L_080cf958:
	.4byte gBattleFxWork
.L_080cf95c:
	.4byte 0x00007828
.L_080cf960:
	.4byte 0x04000052
.L_080cf964:
	.4byte 0x000000bf
.L_080cf968:
	.4byte IwramCopyWords
.L_080cf96c:
	.4byte 0x0000009e
.L_080cf970:
	movs r1, #250
	adds r6, #128
	lsls r1, r1, #6
	add r1, r11
	adds r0, r6, #0
	bl Resource_DecodeType01
	mov r2, r8
	cmp r2, #0
	bne .L_080cf988
	ldr r0, .L_080cfc50
	b .L_080cf9b8
.L_080cf988:
	mov r3, r8
	cmp r3, #1
	bne .L_080cf992
	ldr r0, .L_080cfc54
	b .L_080cf9b8
.L_080cf992:
	mov r4, r8
	cmp r4, #2
	bne .L_080cf99c
	ldr r0, .L_080cfc58
	b .L_080cf9b8
.L_080cf99c:
	mov r0, r8
	cmp r0, #3
	beq .L_080cf9b6
	mov r1, r8
	cmp r1, #4
	bne .L_080cf9ac
	adds r0, r7, #0
	b .L_080cf9b8
.L_080cf9ac:
	mov r2, r8
	cmp r2, #6
	bne .L_080cf9b6
	ldr r0, .L_080cfc5c
	b .L_080cf9b8
.L_080cf9b6:
	ldr r0, .L_080cfc60
.L_080cf9b8:
	bl Resource_GetTableEntry
	adds r6, r0, #0
	movs r0, #160
	ldr r3, .L_080cfc64
	adds r1, r6, #0
	movs r2, #128
	lsls r0, r0, #19
	bl _call_via_r3
	movs r3, #0
	mov r10, r3
	movs r1, #1
	movs r2, #128
	ldr r3, .L_080cfc68
	negs r1, r1
	lsls r2, r2, #3
.L_080cf9da:
	movs r4, #1
	add r10, r4
	str r1, [r3]
	adds r3, #28
	cmp r10, r2
	bne .L_080cf9da
	ldr r5, .L_080cfc6c
	add r5, r11
	ldr r3, [r5]
	ldr r0, [r3, #8]
	bl GetBattleObjectSlotFar
	ldr r6, [r0]
	movs r0, #0
	str r0, [sp, #44]
	ldr r3, [r5]
	ldr r3, [r3, #20]
	movs r4, #128
	lsls r4, r4, #11
	cmp r3, #0
	beq .L_080cfa70
	ldr r5, .L_080cfc6c
	mov r9, r0
	add r5, r11
	movs r7, #0
.L_080cfa0c:
	ldr r1, [sp, #44]
	ldr r2, [r5]
	lsls r3, r1, #1
	adds r3, #36
	ldrsh r0, [r2, r3]
	str r4, [sp, #8]
	bl GetBattleObjectSlotFar
	ldr r3, .L_080cfc70
	movs r2, #0
	ldr r0, [r0]
	ldr r4, [sp, #8]
	mov r10, r2
	adds r1, r7, r3
.L_080cfa28:
	ldr r3, [r6, #8]
	str r4, [r1, #4]
	str r3, [r1]
	ldr r3, [r6, #16]
	str r3, [r1, #8]
	ldr r2, [r6, #8]
	ldr r3, [r0, #8]
	subs r3, r3, r2
	movs r2, #128
	asrs r3, r3, #4
	lsls r2, r2, #11
	str r3, [r1, #12]
	str r2, [r1, #16]
	ldr r2, [r6, #16]
	ldr r3, [r0, #16]
	subs r3, r3, r2
	asrs r3, r3, #4
	movs r2, #1
	str r3, [r1, #20]
	add r10, r2
	mov r3, r9
	str r3, [r1, #24]
	mov r3, r10
	adds r1, #28
	cmp r3, #16
	bne .L_080cfa28
	ldr r1, [sp, #44]
	adds r1, #1
	str r1, [sp, #44]
	ldr r3, [r5]
	movs r0, #224
	ldr r3, [r3, #20]
	lsls r0, r0, #1
	adds r7, r7, r0
	cmp r1, r3
	bne .L_080cfa0c
.L_080cfa70:
	movs r2, #0
	mov r10, r2
	movs r1, #1
	movs r2, #128
	ldr r3, .L_080cfc74
	negs r1, r1
	lsls r2, r2, #1
.L_080cfa7e:
	movs r4, #1
	add r10, r4
	str r1, [r3]
	adds r3, #28
	cmp r10, r2
	bne .L_080cfa7e
	ldr r0, [sp, #56]
	ldr r3, [r0, #4]
	cmp r3, #0
	bne .L_080cfae4
	movs r1, #7
	movs r3, #3
	movs r6, #2
	movs r0, #46
	movs r2, #7
	str r6, [sp, #0]
	bl BattleEffect_LoadWork
	ldr r5, .L_080cfc78
	adds r3, r5, #0
	adds r3, #184
	ldr r3, [r3]
	mov r1, r8
	lsls r1, r1, #1
	str r3, [sp, #60]
	str r1, [sp, #28]
	ldr r3, .L_080cfc7c
	ldrsb r3, [r3, r1]
	cmp r3, #0
	bne .L_080cfac6
	movs r3, #3
	movs r2, #7
	movs r0, #47
	movs r1, #7
	str r3, [sp, #0]
	b .L_080cfb34
.L_080cfac6:
	movs r3, #7
	movs r0, #47
	movs r1, #7
	movs r2, #7
	str r6, [sp, #0]
	bl BattleEffect_LoadWork
	adds r3, r5, #0
	adds r3, #188
	ldr r3, [r3]
	mov r4, sp
	adds r4, #60
	str r4, [sp, #32]
	str r3, [r4, #4]
	b .L_080cfb46
.L_080cfae4:
	movs r3, #7
	movs r6, #2
	movs r0, #46
	movs r1, #7
	movs r2, #7
	str r6, [sp, #0]
	bl BattleEffect_LoadWork
	ldr r5, .L_080cfc78
	adds r3, r5, #0
	adds r3, #184
	ldr r3, [r3]
	mov r0, r8
	lsls r0, r0, #1
	str r3, [sp, #60]
	str r0, [sp, #28]
	ldr r3, .L_080cfc7c
	ldrsb r3, [r3, r0]
	cmp r3, #0
	bne .L_080cfb2a
	movs r3, #3
	movs r1, #7
	movs r0, #47
	movs r2, #7
	str r3, [sp, #0]
	bl BattleEffect_LoadWork
	adds r3, r5, #0
	adds r3, #188
	ldr r3, [r3]
	mov r1, sp
	adds r1, #60
	str r1, [sp, #32]
	str r3, [r1, #4]
	b .L_080cfb46
.L_080cfb2a:
	movs r2, #7
	movs r3, #3
	movs r0, #47
	movs r1, #7
	str r6, [sp, #0]
.L_080cfb34:
	bl BattleEffect_LoadWork
	adds r3, r5, #0
	adds r3, #188
	ldr r3, [r3]
	mov r2, sp
	adds r2, #60
	str r2, [sp, #32]
	str r3, [r2, #4]
.L_080cfb46:
	movs r2, #239
	lsls r2, r2, #7
	add r2, r11
	movs r3, #2
	str r3, [r2]
	ldr r2, .L_080cfc80
	movs r3, #75
	add r2, r11
	movs r1, #144
	str r3, [r2]
	ldr r0, .L_080cfc84
	lsls r1, r1, #3
	bl Scheduler_AddOrUpdateCallback
	ldr r3, .L_080cfc7c
	ldr r4, [sp, #28]
	ldrsb r3, [r3, r4]
	cmp r3, #0
	bne .L_080cfb7a
	ldr r3, .L_080cfc6c
	add r3, r11
	ldr r3, [r3]
	ldr r3, [r3, #20]
	lsls r3, r3, #3
	adds r3, #72
	b .L_080cfb86
.L_080cfb7a:
	ldr r3, .L_080cfc6c
	add r3, r11
	ldr r3, [r3]
	ldr r3, [r3, #20]
	lsls r3, r3, #3
	adds r3, #56
.L_080cfb86:
	str r3, [sp, #40]
	movs r0, #103
	bl AudioCommand_PlayFar
	ldr r1, [sp, #40]
	movs r0, #0
	str r0, [sp, #48]
	cmp r1, #0
	bne .L_080cfb9a
	b .L_080cfea6
.L_080cfb9a:
	ldr r2, [sp, #36]
	adds r2, #12
	str r2, [sp, #20]
.L_080cfba0:
	bl Render_ResetTransformState
	ldr r0, [sp, #36]
	ldr r1, [sp, #20]
	bl Graphics_PrepareTransferInIwramWork
	movs r3, #0
	str r3, [sp, #44]
	ldr r3, .L_080cfc6c
	add r3, r11
	ldr r3, [r3]
	ldr r3, [r3, #20]
	cmp r3, #0
	bne .L_080cfbbe
	b .L_080cfd88
.L_080cfbbe:
	ldr r4, .L_080cfc70
	movs r0, #0
	str r4, [sp, #16]
	str r0, [sp, #12]
.L_080cfbc6:
	ldr r1, [sp, #48]
	ldr r2, [sp, #12]
	cmp r1, r2
	bge .L_080cfbd0
	b .L_080cfd64
.L_080cfbd0:
	adds r3, r2, #0
	adds r3, #17
	ldr r7, [sp, #16]
	cmp r1, r3
	bne .L_080cfbfc
	ldr r3, .L_080cfc6c
	ldr r4, [sp, #44]
	add r3, r11
	ldr r2, [r3]
	lsls r3, r4, #1
	adds r3, #36
	ldrsh r0, [r2, r3]
	movs r3, #16
	str r3, [sp, #0]
	movs r1, #7
	movs r2, #5
	adds r3, r4, #0
	bl ObjectGroup_UpdateMembers
	movs r0, #133
	bl BattleEventRuntime_BeginPhaseFar
.L_080cfbfc:
	ldr r2, [sp, #16]
	ldr r3, [r2, #24]
	cmp r3, #0
	bge .L_080cfc06
	b .L_080cfd64
.L_080cfc06:
	ldr r3, [sp, #48]
	ldr r4, [sp, #12]
	movs r1, #3
	subs r0, r3, r4
	bl __divsi3
	adds r5, r0, #0
	cmp r5, #9
	ble .L_080cfc1a
	movs r5, #9
.L_080cfc1a:
	add r6, sp, #68
	ldr r0, [sp, #16]
	adds r1, r6, #0
	bl EffectPosition_ApplyBaseAndYOffset
	ldr r3, [r6]
	asrs r2, r3, #1
	str r2, [r6]
	cmp r5, #4
	ble .L_080cfc88
	lsls r1, r5, #1
	ldr r3, [r6, #4]
	movs r0, #32
	adds r1, r1, r5
	lsls r1, r1, #8
	str r0, [sp, #0]
	movs r0, #24
	str r0, [sp, #4]
	add r1, r11
	subs r2, #16
	subs r3, #12
	ldr r4, [sp, #60]
	ldr r0, [sp, #52]
	bl _call_via_r4
	b .L_080cfca6
	.2byte 0x0000
.L_080cfc50:
	.4byte 0x0000009f
.L_080cfc54:
	.4byte 0x00000059
.L_080cfc58:
	.4byte 0x000000a0
.L_080cfc5c:
	.4byte 0x0000008d
.L_080cfc60:
	.4byte 0x00000077
.L_080cfc64:
	.4byte IwramCopyWords
.L_080cfc68:
	.4byte gMapCellBuffer + 0x18
.L_080cfc6c:
	.4byte 0x00007828
.L_080cfc70:
	.4byte gMapCellBuffer
.L_080cfc74:
	.4byte gMapCellBuffer + 0x1c18
.L_080cfc78:
	.4byte gWorkSlot
.L_080cfc7c:
	.4byte Data_080ee0b6
.L_080cfc80:
	.4byte 0x00007784
.L_080cfc84:
	.4byte BattlePresentation_ProcessPendingGraphicsTransfer
.L_080cfc88:
	lsls r1, r5, #1
	ldr r3, [r6, #4]
	movs r0, #24
	adds r1, r1, r5
	lsls r1, r1, #8
	str r0, [sp, #0]
	movs r0, #32
	str r0, [sp, #4]
	add r1, r11
	subs r2, #12
	subs r3, #16
	ldr r4, [sp, #60]
	ldr r0, [sp, #52]
	bl _call_via_r4
.L_080cfca6:
	ldr r3, [r7, #24]
	cmp r3, #0
	bne .L_080cfcb6
	adds r0, r7, #0
	movs r1, #63
	ldr r2, .L_080cfed0
	bl EffectStep_AdvanceWithGravity3D
.L_080cfcb6:
	ldr r3, [r7, #4]
	cmp r3, #0
	bge .L_080cfd64
	movs r3, #0
	str r3, [r7, #4]
	movs r3, #1
	str r3, [r7, #24]
	ldr r0, .L_080cfed4
	ldr r1, [sp, #28]
	ldrsb r3, [r0, r1]
	movs r2, #4
	mov r9, r2
	cmp r3, #0
	beq .L_080cfcd6
	movs r3, #16
	mov r9, r3
.L_080cfcd6:
	movs r4, #0
	mov r0, r9
	mov r10, r4
	cmp r0, #0
	beq .L_080cfd64
	ldr r1, [sp, #44]
	lsls r1, r1, #2
	movs r2, #63
	str r1, [sp, #24]
	mov r8, r2
.L_080cfcea:
	ldr r3, [sp, #24]
	lsls r2, r3, #3
	add r2, r10
	lsls r3, r2, #3
	subs r3, r3, r2
	ldr r4, .L_080cfed8
	lsls r3, r3, #2
	adds r5, r3, r4
	ldr r3, [r7]
	str r3, [r5]
	ldr r3, [r7, #4]
	str r3, [r5, #4]
	ldr r3, [r7, #8]
	str r3, [r5, #8]
	ldr r0, .L_080cfed4
	ldr r1, [sp, #28]
	ldrsb r6, [r0, r1]
	cmp r6, #0
	bne .L_080cfd2e
	bl Random16
	mov r2, r8
	ands r0, r2
	subs r0, #32
	lsls r0, r0, #11
	str r0, [r5, #12]
	str r6, [r5, #16]
	bl Random16
	mov r3, r8
	ands r0, r3
	subs r0, #32
	lsls r0, r0, #11
	b .L_080cfd56
.L_080cfd2e:
	bl Random16
	mov r4, r8
	ands r0, r4
	subs r0, #32
	lsls r0, r0, #13
	str r0, [r5, #12]
	bl Random16
	movs r3, #31
	ands r3, r0
	adds r3, #32
	lsls r3, r3, #12
	str r3, [r5, #16]
	bl Random16
	mov r1, r8
	ands r0, r1
	subs r0, #32
	lsls r0, r0, #13
.L_080cfd56:
	str r0, [r5, #20]
	movs r2, #1
	movs r3, #0
	add r10, r2
	str r3, [r5, #24]
	cmp r10, r9
	bne .L_080cfcea
.L_080cfd64:
	ldr r3, [sp, #16]
	movs r4, #224
	ldr r0, [sp, #12]
	ldr r1, [sp, #44]
	lsls r4, r4, #1
	adds r3, r3, r4
	str r3, [sp, #16]
	adds r0, #8
	adds r1, #1
	ldr r3, .L_080cfedc
	str r0, [sp, #12]
	str r1, [sp, #44]
	add r3, r11
	ldr r3, [r3]
	ldr r3, [r3, #20]
	cmp r1, r3
	beq .L_080cfd88
	b .L_080cfbc6
.L_080cfd88:
	ldr r3, .L_080cfed8
	ldr r4, .L_080cfed4
	movs r2, #0
	mov r10, r2
	mov r8, r3
	mov r9, r4
.L_080cfd94:
	mov r0, r8
	ldr r3, [r0, #24]
	cmp r3, #44
	bhi .L_080cfe6e
	ldr r3, [r0, #4]
	cmp r3, #0
	blt .L_080cfe6e
	add r6, sp, #68
	adds r1, r6, #0
	bl EffectPosition_ApplyBaseAndYOffset
	ldr r3, [r6]
	asrs r2, r3, #1
	str r2, [r6]
	ldr r4, [sp, #28]
	mov r1, r9
	ldrsb r3, [r1, r4]
	cmp r3, #0
	bne .L_080cfdee
	mov r0, r8
	ldr r3, [r0, #24]
	cmp r3, #0
	bge .L_080cfdc4
	adds r3, #7
.L_080cfdc4:
	asrs r3, r3, #3
	lsls r1, r3, #3
	adds r1, r1, r3
	lsls r1, r1, #7
	movs r3, #250
	lsls r3, r3, #6
	movs r0, #24
	add r1, r11
	adds r1, r1, r3
	ldr r3, [r6, #4]
	str r0, [sp, #0]
	movs r0, #48
	str r0, [sp, #4]
	ldr r0, [sp, #32]
	subs r2, #12
	ldr r4, [r0, #4]
	subs r3, #24
	ldr r0, [sp, #52]
	bl _call_via_r4
	b .L_080cfe44
.L_080cfdee:
	mov r1, r8
	ldr r0, [r1, #24]
	movs r1, #5
	bl __divsi3
	movs r1, #1
	mov r3, r10
	ands r3, r1
	cmp r3, #0
	beq .L_080cfe04
	adds r0, #9
.L_080cfe04:
	ldr r3, [sp, #56]
	mov r4, r8
	ldr r2, [r3, #4]
	ldr r3, [r4, #12]
	cmp r3, #0
	ble .L_080cfe12
	eors r2, r1
.L_080cfe12:
	lsls r7, r2, #2
	ldr r2, .L_080cfee0
	lsls r3, r0, #1
	ldrh r1, [r2, r3]
	ldr r3, .L_080cfee4
	movs r2, #240
	ldrb r5, [r3, r0]
	lsls r2, r2, #5
	add r1, r11
	adds r1, r1, r2
	ldr r2, [r6]
	lsrs r3, r5, #1
	subs r2, r2, r3
	ldr r3, .L_080cfee8
	ldrb r4, [r3, r0]
	ldr r3, [r6, #4]
	lsrs r0, r4, #1
	subs r3, r3, r0
	ldr r0, [sp, #32]
	str r5, [sp, #0]
	str r4, [sp, #4]
	ldr r4, [r7, r0]
	ldr r0, [sp, #52]
	bl _call_via_r4
.L_080cfe44:
	ldr r2, [sp, #28]
	mov r1, r9
	ldrsb r3, [r1, r2]
	cmp r3, #0
	bne .L_080cfe5c
	movs r2, #128
	mov r0, r8
	movs r1, #62
	lsls r2, r2, #4
	bl EffectStep_AdvanceWithGravity3D
	b .L_080cfe66
.L_080cfe5c:
	mov r0, r8
	movs r1, #62
	ldr r2, .L_080cfed0
	bl EffectStep_AdvanceWithGravity3D
.L_080cfe66:
	mov r4, r8
	ldr r3, [r4, #24]
	adds r3, #1
	str r3, [r4, #24]
.L_080cfe6e:
	movs r1, #1
	movs r2, #128
	movs r0, #28
	add r10, r1
	lsls r2, r2, #1
	add r8, r0
	cmp r10, r2
	bne .L_080cfd94
	movs r0, #2
	movs r1, #2
	bl Camera_ApplyShake
	bl ObjectGroup_TickMemberTimers
	ldr r2, .L_080cfeec
	movs r3, #1
	add r2, r11
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r3, [sp, #48]
	ldr r4, [sp, #40]
	adds r3, #1
	str r3, [sp, #48]
	cmp r3, r4
	beq .L_080cfea6
	b .L_080cfba0
.L_080cfea6:
	ldr r0, .L_080cfef0
	bl Scheduler_RemoveCallback
	movs r0, #47
	bl Runtime_ReleaseHeapBlock
	movs r0, #46
	bl Runtime_ReleaseHeapBlock
	bl BattleFx_EndCanvasLayer
	add sp, #80
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
.L_080cfed0:
	.4byte 0xffff8000
.L_080cfed4:
	.4byte Data_080ee0b6
.L_080cfed8:
	.4byte gMapCellBuffer + 0x1c00
.L_080cfedc:
	.4byte 0x00007828
.L_080cfee0:
	.4byte Data_080ee0e8
.L_080cfee4:
	.4byte Data_080ee0c4
.L_080cfee8:
	.4byte Data_080ee0d6
.L_080cfeec:
	.4byte 0x00007824
.L_080cfef0:
	.4byte BattlePresentation_ProcessPendingGraphicsTransfer
