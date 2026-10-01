.syntax unified
	.thumb
	.global Unnamed_080d0ad4
	.thumb_func
Unnamed_080d0ad4:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r1, .L_080d0b4c
	mov r8, r1
	mov r3, r8
	ldmia r3!, {r2}
	sub sp, #120
	str r2, [sp, #68]
	ldr r3, [r3]
	str r3, [sp, #64]
	mov r3, r8
	ldr r4, .L_080d0b50
	ldr r6, [r3, #8]
	subs r3, #108
	ldr r3, [r3]
	adds r5, r2, r4
	str r3, [sp, #44]
	str r0, [r5]
	movs r0, #0
	bl BattleFx_BeginCanvasLayer
	ldr r2, .L_080d0b54
	ldr r3, .L_080d0b48
	ldr r0, .L_080d0b58
	strh r3, [r2]
	bl Resource_GetTableEntry
	adds r7, r0, #0
	movs r0, #160
	ldr r3, .L_080d0b5c
	movs r2, #128
	adds r1, r7, #0
	lsls r0, r0, #19
	bl _call_via_r3
	adds r7, #128
	ldr r1, [sp, #68]
	adds r0, r7, #0
	bl Resource_DecodeType01
	ldr r0, .L_080d0b60
	bl Resource_GetTableEntry
	adds r1, r6, #0
	bl Resource_DecodeType01
	ldr r0, .L_080d0b64
	bl Resource_GetTableEntry
	movs r3, #128
	ldr r2, [sp, #68]
	lsls r3, r3, #5
	b .L_080d0b68
.L_080d0b48:
	.4byte 0x00000100
.L_080d0b4c:
	.4byte gBattleFxWork
.L_080d0b50:
	.4byte 0x00007828
.L_080d0b54:
	.4byte 0x04000020
.L_080d0b58:
	.4byte 0x00000079
.L_080d0b5c:
	.4byte IwramCopyWords
.L_080d0b60:
	.4byte 0x00000073
.L_080d0b64:
	.4byte 0x00000076
.L_080d0b68:
	adds r1, r2, r3
	adds r7, r0, #0
	bl Resource_DecodeType01
	movs r1, #239
	ldr r4, [sp, #68]
	lsls r1, r1, #7
	adds r2, r4, r1
	movs r3, #3
	str r3, [r2]
	ldr r3, .L_080d0ea8
	adds r2, r4, r3
	ldr r3, .L_080d0eac
	movs r1, #144
	str r3, [r2]
	lsls r1, r1, #3
	ldr r0, .L_080d0eb0
	bl Scheduler_AddOrUpdateCallback
	ldr r3, [r5]
	mov r1, sp
	movs r4, #36
	ldrsh r0, [r3, r4]
	adds r1, #108
	str r1, [sp, #36]
	bl EffectPosition_ApplyStepAndYOffset
	ldr r3, [sp, #36]
	ldr r2, [r3]
	movs r3, #64
	subs r3, r3, r2
	ldr r2, .L_080d0eb4
	str r3, [sp, #40]
	lsls r3, r3, #8
	str r3, [r2]
	movs r0, #142
	bl AudioCommand_PlayFar
	movs r4, #0
	str r4, [sp, #60]
	ldr r3, [r5]
	ldr r2, [r3, #20]
	lsls r3, r2, #2
	adds r3, r3, r2
	movs r5, #72
	lsls r3, r3, #2
	negs r5, r5
	cmp r3, r5
	bne .L_080d0bcc
	b .L_080d0e8a
.L_080d0bcc:
	ldr r1, [sp, #60]
	cmp r1, #64
	bne .L_080d0bd8
	movs r0, #0
	bl BattleEventRuntime_BeginPhaseFar
.L_080d0bd8:
	ldr r2, [sp, #60]
	cmp r2, #46
	bne .L_080d0bf4
	ldr r4, [sp, #68]
	ldr r5, .L_080d0eb8
	adds r3, r4, r5
	ldr r3, [r3]
	movs r2, #36
	ldrsh r1, [r3, r2]
	ldr r0, [r3, #8]
	movs r2, #16
	movs r3, #0
	bl BattleMotion_ApproachTargetFar
.L_080d0bf4:
	ldr r0, [sp, #60]
	ldr r1, .L_080d0ebc
	ldr r2, .L_080d0ec0
	movs r3, #0
	bl Graphics_UpdatePhasePalette
	movs r5, #2
	movs r1, #7
	movs r2, #7
	movs r3, #3
	movs r0, #46
	str r5, [sp, #0]
	bl Unnamed_080ed408
	ldr r3, .L_080d0ec4
	adds r3, #184
	ldr r3, [r3]
	movs r0, #47
	str r3, [sp, #52]
	movs r1, #7
	movs r3, #7
	movs r2, #7
	str r5, [sp, #0]
	bl Unnamed_080ed408
	ldr r3, .L_080d0ec8
	ldr r4, [sp, #60]
	ldr r3, [r3]
	str r3, [sp, #56]
	cmp r4, #16
	ble .L_080d0c48
	movs r3, #15
	ands r3, r4
	cmp r3, #0
	bne .L_080d0c48
	ldr r5, [sp, #68]
	ldr r1, .L_080d0ea8
	adds r2, r5, r1
	ldr r3, [r2]
	ldr r4, .L_080d0ecc
	adds r3, r3, r4
	str r3, [r2]
.L_080d0c48:
	ldr r1, [sp, #60]
	lsls r3, r1, #1
	adds r3, r3, r1
	movs r5, #0
	lsls r3, r3, #9
	str r5, [sp, #48]
	str r5, [sp, #16]
	str r3, [sp, #12]
	mov r11, r1
.L_080d0c5a:
	ldr r2, [sp, #68]
	ldr r4, .L_080d0eb8
	ldr r5, [sp, #48]
	adds r3, r2, r4
	ldr r2, [r3]
	lsls r3, r5, #1
	adds r3, #36
	ldrsh r0, [r2, r3]
	bl GetBattleObjectSlotFar
	mov r2, r11
	ldr r6, [r0]
	cmp r2, #95
	bls .L_080d0c78
	b .L_080d0e30
.L_080d0c78:
	bl Render_ResetTransformState
	ldr r0, [sp, #44]
	adds r1, r0, #0
	adds r1, #12
	bl Graphics_PrepareTransferInIwramWork
	ldr r3, [r6, #8]
	add r5, sp, #96
	str r3, [r5]
	ldr r3, [r6, #12]
	str r3, [r5, #4]
	ldr r3, [r6, #16]
	str r3, [r5, #8]
	add r3, sp, #84
	mov r10, r3
	mov r1, r10
	adds r0, r5, #0
	bl EffectPosition_ApplyBaseAndYOffset
	ldr r4, [sp, #36]
	ldr r1, [sp, #40]
	ldr r3, [r4]
	mov r2, r10
	adds r3, r3, r1
	str r3, [r2]
	ldr r3, [r2, #4]
	subs r3, #24
	str r3, [r2, #4]
	mov r3, r11
	cmp r3, #67
	ble .L_080d0cba
	b .L_080d0df6
.L_080d0cba:
	ldr r5, [sp, #16]
	lsls r2, r5, #3
	subs r2, r2, r5
	ldr r1, [sp, #68]
	movs r4, #0
	lsls r2, r2, #2
	adds r2, r2, r1
	movs r3, #168
	str r4, [sp, #20]
	ldr r1, [sp, #12]
	movs r4, #225
	lsls r3, r3, #10
	lsls r4, r4, #7
	subs r1, r3, r1
	adds r5, r2, r4
	movs r3, #64
	mov r2, r11
	subs r3, r3, r2
	lsls r3, r3, #9
	movs r7, #0
	add r6, sp, #72
	mov r8, r1
	mov r9, r3
.L_080d0ce8:
	bl Render_ResetTransformState
	mov r3, r11
	cmp r3, #63
	bgt .L_080d0d0c
	mov r4, r8
	str r4, [r6]
	str r4, [r6, #4]
	str r4, [r6, #8]
	adds r0, r6, #0
	bl SceneTransform_ApplyScale
	mov r0, r9
	bl SceneTransform_ApplyRoll
	mov r0, r9
	bl SceneTransform_ApplyYaw
.L_080d0d0c:
	ldr r0, [sp, #20]
	bl SceneTransform_ApplyRoll
	add r2, sp, #96
	adds r1, r2, #0
	ldr r0, .L_080d0ed0
	bl EffectPosition_ApplyBaseAndYOffset
	mov r4, r10
	ldr r3, [r4]
	ldr r2, [sp, #96]
	adds r2, r2, r3
	str r2, [r5, #12]
	ldr r3, [sp, #100]
	ldr r2, [r4, #4]
	adds r3, r3, r2
	adds r3, #16
	str r3, [r5, #16]
	ldr r1, [sp, #20]
	ldr r2, .L_080d0ec0
	adds r7, #1
	adds r1, r1, r2
	str r1, [sp, #20]
	adds r5, #28
	cmp r7, #3
	bne .L_080d0ce8
	ldr r3, [sp, #16]
	str r3, [sp, #28]
	movs r7, #0
.L_080d0d46:
	ldr r4, [sp, #28]
	adds r2, r7, r4
	lsls r3, r2, #3
	subs r3, r3, r2
	ldr r5, [sp, #68]
	lsls r3, r3, #2
	movs r1, #225
	lsls r1, r1, #7
	adds r7, #1
	adds r3, r5, r3
	adds r3, r3, r1
	adds r0, r7, #0
	movs r1, #3
	str r3, [sp, #32]
	str r7, [sp, #24]
	bl __modsi3
	ldr r2, [sp, #28]
	adds r0, r0, r2
	lsls r3, r0, #3
	subs r3, r3, r0
	lsls r3, r3, #2
	movs r4, #225
	adds r3, r5, r3
	lsls r4, r4, #7
	adds r4, r4, r3
	mov r2, r11
	mov r9, r4
	cmp r2, #0
	bge .L_080d0d84
	adds r2, #15
.L_080d0d84:
	asrs r2, r2, #4
	movs r3, #5
	subs r4, r3, r2
	movs r5, #0
	mov r8, r5
	lsls r7, r4, #1
.L_080d0d90:
	ldr r2, [sp, #32]
	mov r1, r9
	ldr r6, [r2, #12]
	ldr r3, [r1, #12]
	subs r3, r3, r6
	mov r0, r8
	muls r0, r3
	movs r1, #24
	str r4, [sp, #8]
	bl __divsi3
	ldr r1, [sp, #32]
	mov r5, r9
	ldr r3, [r5, #16]
	ldr r5, [r1, #16]
	subs r3, r3, r5
	adds r6, r6, r0
	movs r1, #24
	mov r0, r8
	muls r0, r3
	bl __divsi3
	ldr r2, .L_080d0ed4
	subs r3, r7, #2
	ldrh r1, [r2, r3]
	ldr r4, [sp, #8]
	ldr r3, [sp, #68]
	adds r5, r5, r0
	movs r2, #128
	subs r5, r5, r4
	subs r6, r6, r4
	adds r1, r3, r1
	lsls r2, r2, #5
	adds r1, r1, r2
	adds r3, r5, #0
	adds r2, r6, #0
	str r7, [sp, #0]
	str r7, [sp, #4]
	ldr r0, [sp, #64]
	ldr r5, [sp, #52]
	bl _call_via_r5
	movs r1, #1
	add r8, r1
	mov r2, r8
	ldr r4, [sp, #8]
	cmp r2, #24
	bne .L_080d0d90
	ldr r7, [sp, #24]
	cmp r7, #3
	bne .L_080d0d46
.L_080d0df6:
	mov r3, r11
	cmp r3, #63
	ble .L_080d0e30
	mov r4, r10
	ldr r2, [r4]
	ldr r3, [r4, #4]
	movs r5, #24
	str r5, [sp, #0]
	movs r5, #48
	subs r2, #24
	subs r3, #24
	str r5, [sp, #4]
	ldr r1, [sp, #68]
	ldr r4, [sp, #52]
	ldr r0, [sp, #64]
	bl _call_via_r4
	mov r1, r10
	ldr r3, [r1, #4]
	movs r4, #24
	ldr r2, [r1]
	subs r3, #24
	str r5, [sp, #4]
	str r4, [sp, #0]
	ldr r0, [sp, #64]
	ldr r1, [sp, #68]
	ldr r5, [sp, #56]
	bl _call_via_r5
.L_080d0e30:
	ldr r1, [sp, #16]
	ldr r2, [sp, #12]
	ldr r5, [sp, #48]
	ldr r3, .L_080d0ed8
	movs r4, #8
	adds r1, #32
	adds r2, r2, r3
	negs r4, r4
	adds r5, #1
	str r1, [sp, #16]
	str r2, [sp, #12]
	add r11, r4
	str r5, [sp, #48]
	cmp r5, #1
	beq .L_080d0e50
	b .L_080d0c5a
.L_080d0e50:
	movs r0, #47
	bl Runtime_ReleaseHeapBlock
	movs r0, #46
	bl Runtime_ReleaseHeapBlock
	ldr r2, .L_080d0edc
	ldr r1, [sp, #68]
	adds r3, r1, r2
	str r5, [r3]
	movs r0, #1
	bl WaitFrames
	ldr r3, [sp, #60]
	ldr r4, [sp, #68]
	adds r3, #1
	ldr r5, .L_080d0eb8
	str r3, [sp, #60]
	adds r3, r4, r5
	ldr r3, [r3]
	ldr r2, [r3, #20]
	lsls r3, r2, #2
	adds r3, r3, r2
	lsls r3, r3, #2
	ldr r1, [sp, #60]
	adds r3, #72
	cmp r1, r3
	beq .L_080d0e8a
	b .L_080d0bcc
.L_080d0e8a:
	ldr r0, .L_080d0eb0
	bl Scheduler_RemoveCallback
	bl BattleFx_EndCanvasLayer
	add sp, #120
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
.L_080d0ea8:
	.4byte 0x00007784
.L_080d0eac:
	.4byte 0x04040404
.L_080d0eb0:
	.4byte BattlePresentation_ProcessPendingGraphicsTransfer
.L_080d0eb4:
	.4byte 0x04000028
.L_080d0eb8:
	.4byte 0x00007828
.L_080d0ebc:
	.4byte 0x0000aaab
.L_080d0ec0:
	.4byte 0x00005555
.L_080d0ec4:
	.4byte gWorkSlot
.L_080d0ec8:
	.4byte gTransitionWork + 0xc
.L_080d0ecc:
	.4byte 0x01010101
.L_080d0ed0:
	.4byte Data_080ee128 + 0xc
.L_080d0ed4:
	.4byte BattleFx6_FlareCells
.L_080d0ed8:
	.4byte 0xffffd000
.L_080d0edc:
	.4byte 0x00007824
