.syntax unified
	.thumb
	.global Func_080d05fc
	.thumb_func
Func_080d05fc:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r6, .L_080d0694
	adds r3, r6, #0
	ldmia r3!, {r1}
	sub sp, #132
	str r1, [sp, #64]
	ldr r3, [r3]
	str r3, [sp, #60]
	adds r3, r6, #0
	subs r3, #108
	ldr r3, [r3]
	ldr r2, [r6, #8]
	str r3, [sp, #48]
	ldr r3, .L_080d0698
	adds r5, r0, #0
	adds r7, r1, r3
	str r5, [r7]
	movs r0, #1
	mov r8, r2
	bl BattleFx_BeginCanvasLayer
	ldr r2, [r7]
	ldr r3, [r2, #28]
	cmp r3, #1
	bne .L_080d064e
	add r3, sp, #80
	ldr r2, [r2, #4]
	str r3, [sp, #0]
	add r3, sp, #76
	str r3, [sp, #4]
	adds r0, r5, #0
	movs r1, #3
	movs r3, #0
	bl BattleFx_PrepareCanvasEffect
.L_080d064e:
	ldr r2, .L_080d069c
	ldr r3, .L_080d0690
	ldr r0, .L_080d06a0
	strh r3, [r2]
	bl Resource_GetTableEntry
	adds r5, r0, #0
	movs r0, #160
	ldr r3, .L_080d06a4
	movs r2, #128
	adds r1, r5, #0
	lsls r0, r0, #19
	bl _call_via_r3
	adds r5, #128
	ldr r1, [sp, #64]
	adds r0, r5, #0
	bl Resource_DecodeType01
	ldr r0, .L_080d06a8
	bl Resource_GetTableEntry
	mov r1, r8
	bl Resource_DecodeType01
	ldr r0, .L_080d06ac
	bl Resource_GetTableEntry
	movs r3, #128
	ldr r2, [sp, #64]
	lsls r3, r3, #5
	b .L_080d06b0
	.2byte 0x0000
.L_080d0690:
	.4byte 0x00000100
.L_080d0694:
	.4byte gBattleFxWork
.L_080d0698:
	.4byte 0x00007828
.L_080d069c:
	.4byte 0x04000020
.L_080d06a0:
	.4byte 0x00000079
.L_080d06a4:
	.4byte IwramCopyWords
.L_080d06a8:
	.4byte 0x00000073
.L_080d06ac:
	.4byte 0x00000076
.L_080d06b0:
	adds r1, r2, r3
	bl Resource_DecodeType01
	ldr r0, .L_080d0a70
	bl Resource_GetTableEntry
	movs r3, #128
	ldr r2, [sp, #64]
	adds r5, r0, #0
	lsls r3, r3, #6
	adds r5, #128
	adds r1, r2, r3
	adds r0, r5, #0
	bl Resource_DecodeType01
	movs r3, #239
	ldr r1, [sp, #64]
	lsls r3, r3, #7
	adds r2, r1, r3
	movs r3, #3
	str r3, [r2]
	ldr r3, .L_080d0a74
	adds r2, r1, r3
	ldr r3, .L_080d0a78
	movs r1, #144
	str r3, [r2]
	lsls r1, r1, #3
	ldr r0, .L_080d0a7c
	bl Scheduler_AddOrUpdateCallback
	ldr r3, [r7]
	mov r2, sp
	adds r2, #120
	movs r1, #36
	ldrsh r0, [r3, r1]
	adds r1, r2, #0
	str r2, [sp, #40]
	bl EffectPosition_ApplyStepAndYOffset
	ldr r3, [sp, #40]
	ldr r2, [r3]
	movs r3, #64
	subs r3, r3, r2
	ldr r2, .L_080d0a80
	str r3, [sp, #44]
	lsls r3, r3, #8
	str r3, [r2]
	movs r0, #142
	bl AudioCommand_PlayFar
	movs r1, #0
	str r1, [sp, #56]
	ldr r3, [r7]
	ldr r2, [r3, #20]
	lsls r3, r2, #2
	adds r3, r3, r2
	movs r2, #72
	lsls r3, r3, #2
	negs r2, r2
	cmp r3, r2
	bne .L_080d072c
	b .L_080d0ab2
.L_080d072c:
	mov r3, sp
	adds r3, #68
	str r3, [sp, #20]
.L_080d0732:
	ldr r1, [sp, #56]
	cmp r1, #64
	bne .L_080d073e
	movs r0, #0
	bl BattleEventRuntime_BeginPhaseFar
.L_080d073e:
	ldr r2, .L_080d0a84
	movs r3, #0
	ldr r0, [sp, #56]
	ldr r1, .L_080d0a88
	bl Graphics_UpdatePhasePalette
	ldr r3, .L_080d0a8c
	ldr r2, [sp, #64]
	adds r7, r2, r3
	ldr r3, [r7]
	ldr r3, [r3, #28]
	cmp r3, #1
	bne .L_080d07e2
	ldr r1, [sp, #56]
	lsls r5, r1, #11
	adds r0, r5, #0
	bl Trig_Sin
	lsls r3, r0, #2
	adds r3, r3, r0
	ldr r2, [sp, #80]
	lsls r3, r3, #2
	asrs r3, r3, #16
	adds r3, r3, r2
	ldr r2, [sp, #44]
	adds r3, r3, r2
	subs r3, #20
	adds r0, r5, #0
	mov r8, r3
	bl Trig_Cos
	ldr r3, [sp, #76]
	lsls r0, r0, #2
	asrs r0, r0, #16
	adds r0, r0, r3
	ldr r3, [r7]
	adds r6, r0, #0
	ldr r1, [sp, #20]
	ldr r0, [r3, #4]
	bl BattleFx_FetchRectangleBlitters
	ldr r3, [sp, #56]
	subs r6, #24
	cmp r3, #32
	ble .L_080d07a0
	lsls r3, r3, #1
	subs r3, r6, r3
	adds r6, r3, #0
	adds r6, #64
.L_080d07a0:
	ldr r1, [sp, #64]
	movs r2, #128
	lsls r2, r2, #6
	adds r7, r1, r2
	movs r5, #40
	adds r3, r6, #0
	str r5, [sp, #0]
	str r5, [sp, #4]
	ldr r4, [sp, #68]
	ldr r0, [sp, #60]
	adds r1, r7, #0
	mov r2, r8
	bl _call_via_r4
	ldr r3, [sp, #56]
	cmp r3, #3
	bgt .L_080d07d6
	ldr r1, [sp, #20]
	str r5, [sp, #0]
	str r5, [sp, #4]
	ldr r0, [sp, #60]
	ldr r4, [r1, #4]
	mov r2, r8
	adds r1, r7, #0
	adds r3, r6, #0
	bl _call_via_r4
.L_080d07d6:
	movs r0, #47
	bl Runtime_ReleaseHeapBlock
	movs r0, #46
	bl Runtime_ReleaseHeapBlock
.L_080d07e2:
	movs r5, #2
	movs r1, #7
	movs r2, #7
	movs r3, #3
	movs r0, #46
	str r5, [sp, #0]
	bl Unnamed_080ed408
	ldr r3, .L_080d0a90
	adds r3, #184
	ldr r3, [r3]
	movs r1, #7
	str r3, [sp, #68]
	movs r2, #7
	movs r3, #7
	movs r0, #47
	str r5, [sp, #0]
	bl Unnamed_080ed408
	ldr r2, .L_080d0a94
	ldr r1, [sp, #20]
	ldr r3, [r2]
	str r3, [r1, #4]
	ldr r2, [sp, #56]
	cmp r2, #16
	ble .L_080d082c
	movs r3, #15
	ands r3, r2
	cmp r3, #0
	bne .L_080d082c
	ldr r3, [sp, #64]
	ldr r1, .L_080d0a74
	adds r2, r3, r1
	ldr r3, [r2]
	ldr r1, .L_080d0a98
	adds r3, r3, r1
	str r3, [r2]
.L_080d082c:
	ldr r1, [sp, #56]
	lsls r3, r1, #1
	adds r3, r3, r1
	movs r2, #0
	lsls r3, r3, #9
	str r2, [sp, #52]
	str r2, [sp, #12]
	str r3, [sp, #8]
	mov r11, r1
.L_080d083e:
	ldr r2, [sp, #64]
	ldr r1, .L_080d0a8c
	adds r3, r2, r1
	ldr r1, [sp, #52]
	ldr r2, [r3]
	lsls r3, r1, #1
	adds r3, #36
	ldrsh r0, [r2, r3]
	bl GetBattleObjectSlotFar
	mov r2, r11
	ldr r6, [r0]
	cmp r2, #95
	bls .L_080d085c
	b .L_080d0a18
.L_080d085c:
	bl Render_ResetTransformState
	ldr r0, [sp, #48]
	adds r1, r0, #0
	adds r1, #12
	bl Graphics_PrepareTransferInIwramWork
	ldr r3, [r6, #8]
	add r5, sp, #108
	str r3, [r5]
	ldr r3, [r6, #12]
	str r3, [r5, #4]
	ldr r3, [r6, #16]
	str r3, [r5, #8]
	add r3, sp, #96
	mov r9, r3
	mov r1, r9
	adds r0, r5, #0
	bl EffectPosition_ApplyBaseAndYOffset
	ldr r1, [sp, #40]
	ldr r2, [sp, #44]
	ldr r3, [r1]
	mov r1, r9
	adds r3, r3, r2
	str r3, [r1]
	ldr r3, [r1, #4]
	mov r2, r11
	subs r3, #24
	str r3, [r1, #4]
	cmp r2, #67
	ble .L_080d089e
	b .L_080d09dc
.L_080d089e:
	movs r1, #0
	str r1, [sp, #16]
	ldr r1, [sp, #12]
	lsls r2, r1, #3
	subs r2, r2, r1
	ldr r1, [sp, #64]
	lsls r2, r2, #2
	adds r2, r2, r1
	movs r1, #225
	lsls r1, r1, #7
	adds r5, r2, r1
	movs r3, #168
	ldr r2, [sp, #8]
	lsls r3, r3, #10
	subs r2, r3, r2
	mov r1, r11
	movs r3, #64
	subs r3, r3, r1
	lsls r3, r3, #9
	movs r7, #0
	add r6, sp, #84
	mov r8, r2
	mov r10, r3
.L_080d08cc:
	bl Render_ResetTransformState
	mov r2, r11
	cmp r2, #63
	bgt .L_080d08f0
	mov r3, r8
	str r3, [r6]
	str r3, [r6, #4]
	str r3, [r6, #8]
	adds r0, r6, #0
	bl SceneTransform_ApplyScale
	mov r0, r10
	bl SceneTransform_ApplyRoll
	mov r0, r10
	bl SceneTransform_ApplyYaw
.L_080d08f0:
	ldr r0, [sp, #16]
	bl SceneTransform_ApplyRoll
	add r2, sp, #108
	adds r1, r2, #0
	ldr r0, .L_080d0a9c
	bl EffectPosition_ApplyBaseAndYOffset
	mov r1, r9
	ldr r3, [r1]
	ldr r2, [sp, #108]
	adds r2, r2, r3
	str r2, [r5, #12]
	ldr r3, [sp, #112]
	ldr r2, [r1, #4]
	adds r3, r3, r2
	adds r3, #16
	str r3, [r5, #16]
	ldr r2, [sp, #16]
	ldr r3, .L_080d0a84
	adds r7, #1
	adds r2, r2, r3
	str r2, [sp, #16]
	adds r5, #28
	cmp r7, #3
	bne .L_080d08cc
	ldr r1, [sp, #12]
	str r1, [sp, #32]
	movs r7, #0
.L_080d092a:
	ldr r3, [sp, #32]
	adds r2, r7, r3
	lsls r3, r2, #3
	ldr r1, [sp, #64]
	subs r3, r3, r2
	lsls r3, r3, #2
	movs r2, #225
	lsls r2, r2, #7
	adds r3, r1, r3
	adds r7, #1
	adds r3, r3, r2
	movs r1, #3
	adds r0, r7, #0
	str r3, [sp, #36]
	str r7, [sp, #24]
	bl __modsi3
	ldr r3, [sp, #32]
	adds r0, r0, r3
	lsls r3, r0, #3
	subs r3, r3, r0
	ldr r1, [sp, #64]
	lsls r3, r3, #2
	movs r2, #225
	lsls r2, r2, #7
	adds r3, r1, r3
	adds r3, r3, r2
	mov r2, r11
	str r3, [sp, #28]
	cmp r2, #0
	bge .L_080d096a
	adds r2, #15
.L_080d096a:
	asrs r2, r2, #4
	movs r3, #5
	subs r3, r3, r2
	mov r8, r3
	mov r1, r8
	movs r3, #0
	mov r10, r3
	lsls r7, r1, #1
.L_080d097a:
	ldr r2, [sp, #28]
	ldr r1, [sp, #36]
	ldr r3, [r2, #12]
	ldr r6, [r1, #12]
	subs r3, r3, r6
	mov r0, r10
	muls r0, r3
	movs r1, #24
	bl __divsi3
	ldr r2, [sp, #28]
	ldr r1, [sp, #36]
	ldr r3, [r2, #16]
	ldr r5, [r1, #16]
	subs r3, r3, r5
	adds r6, r6, r0
	movs r1, #24
	mov r0, r10
	muls r0, r3
	bl __divsi3
	ldr r2, .L_080d0aa0
	subs r3, r7, #2
	ldrh r1, [r2, r3]
	ldr r3, [sp, #64]
	adds r5, r5, r0
	adds r1, r3, r1
	movs r2, #128
	mov r3, r8
	lsls r2, r2, #5
	subs r6, r6, r3
	subs r5, r5, r3
	adds r1, r1, r2
	str r7, [sp, #0]
	adds r2, r6, #0
	str r7, [sp, #4]
	ldr r4, [sp, #68]
	ldr r0, [sp, #60]
	adds r3, r5, #0
	bl _call_via_r4
	movs r1, #1
	add r10, r1
	mov r2, r10
	cmp r2, #24
	bne .L_080d097a
	ldr r7, [sp, #24]
	cmp r7, #3
	bne .L_080d092a
.L_080d09dc:
	mov r3, r11
	cmp r3, #63
	ble .L_080d0a18
	mov r1, r9
	ldr r2, [r1]
	ldr r3, [r1, #4]
	movs r5, #48
	movs r1, #24
	subs r2, #24
	subs r3, #24
	str r1, [sp, #0]
	ldr r4, [sp, #68]
	ldr r1, [sp, #64]
	str r5, [sp, #4]
	ldr r0, [sp, #60]
	bl _call_via_r4
	mov r3, r9
	movs r1, #24
	ldr r2, [r3]
	ldr r3, [r3, #4]
	str r1, [sp, #0]
	str r5, [sp, #4]
	ldr r1, [sp, #20]
	subs r3, #24
	ldr r4, [r1, #4]
	ldr r0, [sp, #60]
	ldr r1, [sp, #64]
	bl _call_via_r4
.L_080d0a18:
	ldr r3, [sp, #8]
	ldr r1, .L_080d0aa4
	ldr r2, [sp, #12]
	adds r3, r3, r1
	adds r2, #32
	str r3, [sp, #8]
	ldr r3, [sp, #52]
	str r2, [sp, #12]
	movs r2, #8
	negs r2, r2
	adds r3, #1
	add r11, r2
	str r3, [sp, #52]
	cmp r3, #1
	beq .L_080d0a38
	b .L_080d083e
.L_080d0a38:
	movs r0, #47
	bl Runtime_ReleaseHeapBlock
	movs r0, #46
	bl Runtime_ReleaseHeapBlock
	ldr r2, .L_080d0aa8
	ldr r1, [sp, #64]
	adds r3, r1, r2
	ldr r1, [sp, #52]
	movs r0, #1
	str r1, [r3]
	bl WaitFrames
	ldr r2, [sp, #56]
	adds r2, #1
	str r2, [sp, #56]
	ldr r1, [sp, #64]
	ldr r2, .L_080d0a8c
	adds r3, r1, r2
	ldr r3, [r3]
	ldr r2, [r3, #20]
	lsls r3, r2, #2
	adds r3, r3, r2
	lsls r3, r3, #2
	ldr r1, [sp, #56]
	adds r3, #72
	b .L_080d0aac
.L_080d0a70:
	.4byte 0x0000008f
.L_080d0a74:
	.4byte 0x00007784
.L_080d0a78:
	.4byte 0x04040404
.L_080d0a7c:
	.4byte BattlePresentation_ProcessPendingGraphicsTransfer
.L_080d0a80:
	.4byte 0x04000028
.L_080d0a84:
	.4byte 0x00005555
.L_080d0a88:
	.4byte 0x0000aaab
.L_080d0a8c:
	.4byte 0x00007828
.L_080d0a90:
	.4byte gWorkSlot
.L_080d0a94:
	.4byte gTransitionWork + 0xc
.L_080d0a98:
	.4byte 0x01010101
.L_080d0a9c:
	.4byte Data_080ee128
.L_080d0aa0:
	.4byte BattleFx6_FlareCells
.L_080d0aa4:
	.4byte 0xffffd000
.L_080d0aa8:
	.4byte 0x00007824
.L_080d0aac:
	cmp r1, r3
	beq .L_080d0ab2
	b .L_080d0732
.L_080d0ab2:
	ldr r0, .L_080d0ad0
	bl Scheduler_RemoveCallback
	bl BattleFx_EndCanvasLayer
	add sp, #132
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
.L_080d0ad0:
	.4byte BattlePresentation_ProcessPendingGraphicsTransfer
