.syntax unified
	.thumb
	.global Unnamed_080e89ec
	.thumb_func
Unnamed_080e89ec:
	.global BattleEffect_RunDualParticleStream
BattleEffect_RunDualParticleStream:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r2, .L_080e8a58
	adds r3, r2, #0
	ldmia r3!, {r1}
	ldr r3, [r3]
	sub sp, #228
	str r3, [sp, #44]
	movs r3, #0
	str r3, [sp, #36]
	str r3, [sp, #28]
	str r3, [sp, #24]
	ldr r3, .L_080e8a5c
	mov r9, r1
	ldr r2, [r2, #8]
	add r3, r9
	str r2, [sp, #20]
	str r0, [r3]
	movs r0, #0
	bl BattleFx_BeginCanvasLayer
	ldr r2, .L_080e8a60
	ldr r3, .L_080e8a54
	ldr r0, .L_080e8a64
	strh r3, [r2]
	mov r1, r9
	movs r2, #1
	movs r3, #1
	bl Resource_LoadAndDecompress
	movs r5, #234
	ldr r0, .L_080e8a68
	movs r1, #180
	lsls r5, r5, #2
	lsls r1, r1, #5
	mov r2, r9
	movs r7, #1
	mov lr, r0
	mov r12, r1
	adds r4, r2, r5
.L_080e8a46:
	mov r3, lr
	mov r2, r9
	movs r6, #0
	lsls r0, r7, #2
	adds r1, r4, r3
	add r2, r12
	b .L_080e8a6c
.L_080e8a54:
	.4byte 0x00001010
.L_080e8a58:
	.4byte gBattleFxWork
.L_080e8a5c:
	.4byte 0x00007828
.L_080e8a60:
	.4byte 0x04000052
.L_080e8a64:
	.4byte 0x000000c2
.L_080e8a68:
	.4byte 0xfffff1f0
.L_080e8a6c:
	ldrb r3, [r2]
	adds r2, #1
	cmp r7, #10
	ble .L_080e8a80
	subs r3, r3, r0
	adds r3, #40
	cmp r3, #0
	bge .L_080e8a7e
	movs r3, #0
.L_080e8a7e:
	strb r3, [r1]
.L_080e8a80:
	adds r6, #1
	adds r1, #1
	cmp r6, r5
	bne .L_080e8a6c
	movs r0, #234
	lsls r0, r0, #2
	adds r7, #1
	adds r4, r4, r0
	cmp r7, #20
	bne .L_080e8a46
	ldr r1, [sp, #20]
	ldr r0, .L_080e8de0
	movs r2, #0
	movs r3, #0
	bl Resource_LoadAndDecompress
	movs r1, #240
	lsls r1, r1, #6
	ldr r0, .L_080e8de4
	add r1, r9
	movs r2, #1
	movs r3, #1
	bl Resource_LoadAndDecompress
	movs r2, #1
	ldr r0, .L_080e8de8
	ldr r1, .L_080e8dec
	movs r3, #0
	bl Resource_LoadAndDecompress
	ldr r3, .L_080e8df0
	add r3, r9
	ldr r3, [r3]
	mov r1, sp
	ldr r0, [r3, #4]
	adds r1, #48
	str r1, [sp, #16]
	bl BattleFx_FetchRectangleBlitters
	movs r2, #239
	lsls r2, r2, #7
	add r2, r9
	movs r3, #2
	str r3, [r2]
	ldr r2, .L_080e8df4
	movs r3, #75
	add r2, r9
	movs r1, #144
	str r3, [r2]
	ldr r0, .L_080e8df8
	lsls r1, r1, #3
	bl Scheduler_AddOrUpdateCallback
	ldr r5, .L_080e8dfc
	movs r6, #0
	movs r7, #63
	add r5, r9
.L_080e8af2:
	ldr r3, .L_080e8df0
	add r3, r9
	ldr r3, [r3]
	ldr r3, [r3, #4]
	cmp r3, #0
	bne .L_080e8b02
	ldr r3, .L_080e8e00
	b .L_080e8b06
.L_080e8b02:
	movs r3, #224
	lsls r3, r3, #14
.L_080e8b06:
	str r3, [r5]
	movs r3, #0
	str r3, [r5, #4]
	str r3, [r5, #8]
	bl Random16
	ands r0, r7
	subs r0, #32
	lsls r0, r0, #14
	str r0, [r5, #12]
	bl Random16
	ands r0, r7
	lsls r0, r0, #13
	str r0, [r5, #16]
	bl Random16
	ands r0, r7
	subs r0, #32
	lsls r0, r0, #14
	movs r3, #1
	adds r6, #1
	str r0, [r5, #20]
	str r3, [r5, #24]
	adds r5, #28
	cmp r6, #40
	bne .L_080e8af2
	ldr r5, .L_080e8e04
	movs r6, #0
	mov r8, r6
	movs r7, #63
	add r5, r9
.L_080e8b46:
	ldr r3, .L_080e8df0
	add r3, r9
	ldr r3, [r3]
	ldr r3, [r3, #4]
	cmp r3, #0
	bne .L_080e8b56
	ldr r3, .L_080e8e00
	b .L_080e8b5a
.L_080e8b56:
	movs r3, #224
	lsls r3, r3, #14
.L_080e8b5a:
	str r3, [r5]
	movs r3, #160
	lsls r3, r3, #13
	mov r2, r8
	str r3, [r5, #4]
	str r2, [r5, #8]
	bl Random16
	ands r0, r7
	subs r0, #32
	lsls r0, r0, #14
	str r0, [r5, #12]
	bl Random16
	ands r0, r7
	lsls r0, r0, #12
	str r0, [r5, #16]
	bl Random16
	ands r0, r7
	subs r0, #32
	lsls r0, r0, #14
	mov r3, r8
	adds r6, #1
	str r0, [r5, #20]
	str r3, [r5, #24]
	adds r5, #28
	cmp r6, #16
	bne .L_080e8b46
	movs r5, #225
	lsls r5, r5, #7
	ldr r6, .L_080e8e08
	movs r7, #0
	add r5, r9
.L_080e8b9e:
	ldr r3, .L_080e8df0
	add r3, r9
	ldr r3, [r3]
	ldr r3, [r3, #4]
	cmp r3, #1
	bne .L_080e8bbc
	adds r0, r6, #0
	bl Trig_Sin
	lsls r3, r0, #1
	adds r3, r3, r0
	lsls r3, r3, #3
	asrs r3, r3, #16
	adds r3, #88
	b .L_080e8bce
.L_080e8bbc:
	adds r0, r6, #0
	bl Trig_Sin
	lsls r3, r0, #1
	adds r3, r3, r0
	lsls r3, r3, #3
	negs r3, r3
	asrs r3, r3, #16
	adds r3, #16
.L_080e8bce:
	str r3, [r5]
	adds r0, r6, #0
	bl Trig_Cos
	lsls r0, r0, #4
	asrs r0, r0, #16
	adds r0, #40
	str r0, [r5, #4]
	lsls r3, r7, #1
	movs r0, #128
	negs r3, r3
	lsls r0, r0, #5
	adds r7, #1
	str r3, [r5, #24]
	adds r6, r6, r0
	adds r5, #28
	cmp r7, #8
	bne .L_080e8b9e
	ldr r0, .L_080e8e0c
	bl Resource_GetTableEntry
	ldr r2, .L_080e8df0
	movs r1, #0
	add r2, r9
	str r0, [sp, #32]
	str r1, [sp, #40]
	str r2, [sp, #12]
.L_080e8c04:
	ldr r3, .L_080e8e10
	ldr r3, [r3]
	mov r11, r3
	ldr r3, [sp, #40]
	cmp r3, #83
	bne .L_080e8c16
	movs r0, #134
	bl BattleEventRuntime_BeginPhaseFar
.L_080e8c16:
	ldr r0, [sp, #40]
	cmp r0, #0
	bne .L_080e8c22
	movs r0, #136
	bl AudioCommand_PlayFar
.L_080e8c22:
	ldr r1, [sp, #40]
	cmp r1, #50
	bne .L_080e8c2e
	movs r0, #136
	bl AudioCommand_PlayFar
.L_080e8c2e:
	ldr r2, [sp, #12]
	ldr r3, [r2]
	ldr r3, [r3, #4]
	cmp r3, #0
	bne .L_080e8c4a
	ldr r3, [sp, #40]
	cmp r3, #63
	bgt .L_080e8c5e
	mov r0, r11
	ldrh r3, [r0, #54]
	ldr r1, .L_080e8e14
	mov r2, r11
	adds r3, r3, r1
	b .L_080e8c5c
.L_080e8c4a:
	ldr r3, [sp, #40]
	cmp r3, #63
	bgt .L_080e8c5e
	mov r0, r11
	ldrh r3, [r0, #54]
	movs r1, #128
	lsls r1, r1, #1
	adds r3, r3, r1
	mov r2, r11
.L_080e8c5c:
	strh r3, [r2, #54]
.L_080e8c5e:
	movs r3, #100
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl BattlePres_SetupTransitionSceneFar
	ldr r3, [sp, #40]
	cmp r3, #17
	bgt .L_080e8cce
	adds r0, r3, #0
	movs r1, #3
	bl __divsi3
	ldr r2, .L_080e8e18
	adds r5, r0, #0
	lsls r0, r5, #1
	ldrh r1, [r2, r0]
	mov r10, r0
	movs r3, #240
	ldr r0, .L_080e8e1c
	lsls r3, r3, #6
	mov r8, r3
	ldrb r3, [r0, r5]
	ldr r0, .L_080e8e20
	ldrb r2, [r0, r5]
	ldr r6, .L_080e8e24
	str r2, [sp, #0]
	ldrb r2, [r6, r5]
	add r1, r9
	add r1, r8
	adds r3, #60
	str r2, [sp, #4]
	ldr r4, [sp, #48]
	movs r2, #48
	ldr r0, [sp, #44]
	bl _call_via_r4
	ldr r2, .L_080e8e18
	ldr r0, .L_080e8e1c
	mov r3, r10
	ldrh r1, [r2, r3]
	ldrb r3, [r0, r5]
	ldr r0, .L_080e8e20
	ldrb r2, [r0, r5]
	str r2, [sp, #0]
	ldrb r2, [r6, r5]
	str r2, [sp, #4]
	ldr r2, [sp, #16]
	add r1, r9
	ldr r4, [r2, #4]
	add r1, r8
	adds r3, #60
	ldr r0, [sp, #44]
	movs r2, #56
	bl _call_via_r4
.L_080e8cce:
	ldr r3, [sp, #40]
	subs r3, #18
	str r3, [sp, #8]
	cmp r3, #40
	bhi .L_080e8d1a
	ldr r0, [sp, #40]
	cmp r0, #18
	bne .L_080e8d00
	ldr r1, [sp, #32]
	movs r3, #0
	ldrsb r3, [r1, r3]
	ldrb r2, [r1, #1]
	lsls r3, r3, #8
	adds r3, r3, r2
	str r3, [sp, #28]
	movs r3, #2
	ldrsb r3, [r1, r3]
	ldrb r2, [r1, #3]
	lsls r3, r3, #8
	adds r3, r3, r2
	adds r3, #16
	adds r1, #4
	str r3, [sp, #24]
	str r1, [sp, #32]
	b .L_080e8d1a
.L_080e8d00:
	ldr r2, [sp, #32]
	ldr r0, [sp, #28]
	movs r3, #0
	ldrsb r3, [r2, r3]
	adds r0, r0, r3
	str r0, [sp, #28]
	ldr r1, [sp, #24]
	movs r3, #1
	ldrsb r3, [r2, r3]
	adds r2, #2
	adds r1, r1, r3
	str r1, [sp, #24]
	str r2, [sp, #32]
.L_080e8d1a:
	ldr r3, [sp, #40]
	subs r3, #78
	cmp r3, #40
	bhi .L_080e8d3a
	ldr r2, [sp, #40]
	cmp r2, #78
	bne .L_080e8d34
	movs r3, #56
	negs r3, r3
	movs r0, #48
	str r3, [sp, #28]
	str r0, [sp, #24]
	b .L_080e8d3a
.L_080e8d34:
	ldr r1, [sp, #24]
	subs r1, #16
	str r1, [sp, #24]
.L_080e8d3a:
	movs r2, #24
	movs r3, #39
	movs r6, #19
	mov r10, r2
	mov r8, r3
	movs r7, #156
.L_080e8d46:
	adds r3, r6, #0
	ldr r0, [sp, #40]
	adds r3, #18
	cmp r0, r3
	ble .L_080e8daa
	adds r3, #65
	cmp r0, r3
	bgt .L_080e8daa
	lsls r0, r6, #3
	adds r3, r0, #0
	add r2, sp, #68
	subs r3, #8
	ldr r3, [r2, r3]
	str r3, [r2, r0]
	subs r3, r0, #4
	ldr r5, [r2, r3]
	str r5, [r2, r7]
	cmp r6, #10
	ble .L_080e8d90
	movs r3, #234
	lsls r3, r3, #2
	adds r1, r6, #0
	muls r1, r3
	ldr r3, .L_080e8e28
	add r1, r9
	ldr r2, [r2, r0]
	adds r1, r1, r3
	mov r0, r10
	mov r3, r8
	str r0, [sp, #0]
	str r3, [sp, #4]
	ldr r4, [sp, #48]
	ldr r0, [sp, #44]
	adds r3, r5, #0
	bl _call_via_r4
	b .L_080e8daa
.L_080e8d90:
	mov r1, r8
	ldr r2, [r2, r0]
	str r1, [sp, #4]
	movs r1, #180
	mov r0, r10
	lsls r1, r1, #5
	str r0, [sp, #0]
	ldr r4, [sp, #48]
	ldr r0, [sp, #44]
	add r1, r9
	adds r3, r5, #0
	bl _call_via_r4
.L_080e8daa:
	subs r6, #1
	subs r7, #8
	cmp r6, #0
	bne .L_080e8d46
	bl Render_ResetTransformState
	mov r1, r11
	adds r1, #12
	mov r0, r11
	bl Graphics_PrepareTransferInIwramWork
	ldr r2, [sp, #8]
	cmp r2, #65
	bhi .L_080e8eb4
	ldr r0, [sp, #12]
	ldr r3, [r0]
	ldr r3, [r3, #4]
	cmp r3, #1
	bne .L_080e8e2c
	ldr r3, [sp, #28]
	lsrs r2, r3, #31
	adds r2, r3, r2
	asrs r2, r2, #1
	movs r3, #64
	add r1, sp, #56
	subs r3, r3, r2
	b .L_080e8e38
.L_080e8de0:
	.4byte 0x00000073
.L_080e8de4:
	.4byte 0x000000b4
.L_080e8de8:
	.4byte 0x0000007d
.L_080e8dec:
	.4byte gMapCellBuffer
.L_080e8df0:
	.4byte 0x00007828
.L_080e8df4:
	.4byte 0x00007784
.L_080e8df8:
	.4byte BattlePresentation_ProcessPendingGraphicsTransfer
.L_080e8dfc:
	.4byte 0x00007160
.L_080e8e00:
	.4byte 0xffc80000
.L_080e8e04:
	.4byte 0x000075c0
.L_080e8e08:
	.4byte 0xffffc000
.L_080e8e0c:
	.4byte 0x000000d3
.L_080e8e10:
	.4byte gCameraWork
.L_080e8e14:
	.4byte 0xffffff00
.L_080e8e18:
	.4byte PuffArc_CellSourceOffsets
.L_080e8e1c:
	.4byte PuffArc_CellBiasY
.L_080e8e20:
	.4byte PuffArc_CellWidths
.L_080e8e24:
	.4byte PuffArc_CellHeights
.L_080e8e28:
	.4byte 0xfffff1f0
.L_080e8e2c:
	ldr r0, [sp, #28]
	lsrs r3, r0, #31
	adds r3, r0, r3
	asrs r3, r3, #1
	add r1, sp, #56
	adds r3, #64
.L_080e8e38:
	str r3, [r1]
	ldr r2, [sp, #24]
	movs r3, #60
	subs r3, r3, r2
	str r3, [r1, #4]
	add r4, sp, #68
	ldr r2, [r4, #4]
	subs r3, r3, r2
	subs r3, #24
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r0, r3, #1
	cmp r0, #2
	ble .L_080e8e56
	movs r0, #2
.L_080e8e56:
	movs r3, #2
	negs r3, r3
	cmp r0, r3
	bge .L_080e8e62
	movs r0, #2
	negs r0, r0
.L_080e8e62:
	ldr r2, [sp, #36]
	adds r2, r2, r0
	str r2, [sp, #36]
	cmp r2, #8
	ble .L_080e8e70
	movs r3, #8
	str r3, [sp, #36]
.L_080e8e70:
	movs r2, #8
	ldr r0, [sp, #36]
	negs r2, r2
	cmp r0, r2
	bge .L_080e8e7c
	str r2, [sp, #36]
.L_080e8e7c:
	ldr r3, [sp, #36]
	cmp r3, #0
	bge .L_080e8e84
	adds r3, #3
.L_080e8e84:
	ldr r2, [r1]
	asrs r3, r3, #2
	adds r0, r3, #2
	adds r3, r2, #0
	subs r3, #12
	str r3, [r4]
	ldr r3, [r1, #4]
	adds r1, r3, #0
	subs r1, #20
	str r1, [r4, #4]
	lsls r1, r0, #3
	adds r1, r1, r0
	movs r0, #24
	lsls r1, r1, #7
	str r0, [sp, #0]
	movs r0, #48
	str r0, [sp, #4]
	add r1, r9
	subs r2, #18
	subs r3, #22
	ldr r4, [sp, #48]
	ldr r0, [sp, #44]
	bl _call_via_r4
.L_080e8eb4:
	ldr r3, [sp, #40]
	cmp r3, #83
	bne .L_080e8ee4
	ldr r3, .L_080e9078
	movs r2, #8
	add r3, r9
	str r2, [r3]
	ldr r0, [sp, #12]
	ldr r3, [r0]
	movs r1, #36
	ldrsh r0, [r3, r1]
	movs r1, #7
	str r2, [sp, #0]
	movs r3, #0
	movs r2, #5
	bl ObjectGroup_UpdateMembers
	ldr r2, [sp, #12]
	ldr r3, [r2]
	movs r1, #36
	ldrsh r0, [r3, r1]
	movs r1, #1
	bl BattleMotion_ApplyVariantMotionFar
.L_080e8ee4:
	ldr r2, [sp, #40]
	cmp r2, #83
	ble .L_080e8fac
	ldr r6, .L_080e907c
	movs r7, #0
	add r6, r9
.L_080e8ef0:
	ldr r3, [r6, #4]
	cmp r3, #0
	blt .L_080e8fa4
	add r5, sp, #56
	adds r0, r6, #0
	adds r1, r5, #0
	bl EffectPosition_ApplyBaseAndYOffset
	ldr r3, [r5]
	asrs r3, r3, #1
	str r3, [r5]
	ldr r3, [r5, #8]
	cmp r3, #159
	bgt .L_080e8f10
	movs r3, #160
	str r3, [r5, #8]
.L_080e8f10:
	ldr r2, .L_080e9080
	cmp r3, r2
	ble .L_080e8f1a
	str r2, [r5, #8]
	adds r3, r2, #0
.L_080e8f1a:
	adds r2, r3, #0
	subs r2, #160
	cmp r2, #0
	bge .L_080e8f24
	adds r2, #63
.L_080e8f24:
	asrs r2, r2, #6
	movs r3, #9
	subs r4, r3, r2
	cmp r7, #47
	ble .L_080e8f60
	ldr r3, [r6, #24]
	cmp r3, #11
	bgt .L_080e8f86
	lsrs r1, r3, #31
	adds r1, r3, r1
	asrs r1, r1, #1
	ldr r3, .L_080e9084
	lsls r1, r1, #11
	ldr r2, [r5]
	adds r1, r1, r3
	movs r0, #32
	ldr r3, [r5, #4]
	str r0, [sp, #0]
	movs r0, #64
	subs r3, #32
	str r0, [sp, #4]
	subs r2, #16
	ldr r4, [sp, #48]
	ldr r0, [sp, #44]
	bl _call_via_r4
	ldr r3, [r6, #24]
	adds r3, #1
	str r3, [r6, #24]
	b .L_080e8f86
.L_080e8f60:
	lsls r0, r4, #1
	ldr r2, .L_080e9088
	subs r3, r0, #2
	ldrh r1, [r2, r3]
	ldr r2, [sp, #20]
	lsrs r3, r4, #31
	adds r3, r4, r3
	adds r1, r2, r1
	ldr r2, [r5]
	asrs r3, r3, #1
	subs r2, r2, r3
	ldr r3, [r5, #4]
	str r4, [sp, #0]
	subs r3, r3, r4
	str r0, [sp, #4]
	ldr r4, [sp, #48]
	ldr r0, [sp, #44]
	bl _call_via_r4
.L_080e8f86:
	ldr r3, [r6]
	ldr r2, [r6, #12]
	adds r3, r3, r2
	ldr r1, [r6, #16]
	str r3, [r6]
	ldr r3, [r6, #4]
	adds r3, r3, r1
	str r3, [r6, #4]
	ldr r2, [r6, #20]
	ldr r3, [r6, #8]
	adds r3, r3, r2
	str r3, [r6, #8]
	ldr r3, .L_080e908c
	adds r1, r1, r3
	str r1, [r6, #16]
.L_080e8fa4:
	adds r7, #1
	adds r6, #28
	cmp r7, #56
	bne .L_080e8ef0
.L_080e8fac:
	ldr r0, [sp, #40]
	cmp r0, #50
	bne .L_080e8fd0
	ldr r2, .L_080e9078
	movs r3, #12
	add r2, r9
	str r3, [r2]
	ldr r1, [sp, #12]
	ldr r3, [r1]
	movs r2, #36
	ldrsh r0, [r3, r2]
	movs r3, #8
	str r3, [sp, #0]
	movs r1, #7
	movs r2, #5
	movs r3, #0
	bl ObjectGroup_UpdateMembers
.L_080e8fd0:
	ldr r3, [sp, #40]
	cmp r3, #49
	ble .L_080e902a
	movs r6, #225
	lsls r6, r6, #7
	movs r7, #0
	add r6, r9
.L_080e8fde:
	ldr r3, [r6, #24]
	cmp r3, #11
	bhi .L_080e901e
	lsrs r4, r3, #31
	adds r4, r3, r4
	asrs r4, r4, #1
	ldr r0, .L_080e9090
	lsls r3, r4, #1
	ldrh r1, [r0, r3]
	ldr r3, .L_080e9094
	movs r2, #240
	ldrb r5, [r3, r4]
	lsls r2, r2, #6
	add r1, r9
	adds r1, r1, r2
	ldr r2, [r6]
	lsrs r3, r5, #1
	subs r2, r2, r3
	ldr r3, .L_080e9098
	ldrb r0, [r3, r4]
	ldr r3, [r6, #4]
	str r5, [sp, #0]
	adds r3, r3, r0
	ldr r0, .L_080e909c
	ldrb r0, [r0, r4]
	str r0, [sp, #4]
	ldr r0, [sp, #16]
	ldr r4, [r0, #4]
	ldr r0, [sp, #44]
	bl _call_via_r4
	ldr r3, [r6, #24]
.L_080e901e:
	adds r3, #1
	adds r7, #1
	str r3, [r6, #24]
	adds r6, #28
	cmp r7, #8
	bne .L_080e8fde
.L_080e902a:
	movs r1, #8
	movs r0, #8
	bl Camera_ApplyShake
	bl ObjectGroup_TickMemberTimers
	ldr r2, .L_080e90a0
	movs r3, #1
	add r2, r9
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r1, [sp, #40]
	adds r1, #1
	str r1, [sp, #40]
	cmp r1, #150
	beq .L_080e9050
	b .L_080e8c04
.L_080e9050:
	ldr r0, .L_080e90a4
	bl Scheduler_RemoveCallback
	movs r0, #47
	bl Runtime_ReleaseHeapBlock
	movs r0, #46
	bl Runtime_ReleaseHeapBlock
	bl BattleFx_EndCanvasLayer
	add sp, #228
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
.L_080e9078:
	.4byte 0x000077a8
.L_080e907c:
	.4byte 0x00007160
.L_080e9080:
	.4byte 0x0000031f
.L_080e9084:
	.4byte gMapCellBuffer
.L_080e9088:
	.4byte ParticleStreams_CellOffsets
.L_080e908c:
	.4byte 0xffffe000
.L_080e9090:
	.4byte PuffArc_CellSourceOffsets
.L_080e9094:
	.4byte PuffArc_CellWidths
.L_080e9098:
	.4byte PuffArc_CellBiasY
.L_080e909c:
	.4byte PuffArc_CellHeights
.L_080e90a0:
	.4byte 0x00007824
.L_080e90a4:
	.4byte BattlePresentation_ProcessPendingGraphicsTransfer
