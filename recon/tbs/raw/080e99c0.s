.syntax unified
	.thumb
	.global Unnamed_080e99c0
	.thumb_func
Unnamed_080e99c0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r6, .L_080e9a30
	adds r3, r6, #0
	ldmia r3!, {r1}
	sub sp, #52
	str r1, [sp, #36]
	ldr r3, [r3]
	str r3, [sp, #32]
	ldr r3, .L_080e9a34
	ldr r2, [r6, #8]
	adds r5, r1, r3
	str r2, [sp, #20]
	str r0, [r5]
	movs r0, #1
	bl BattleFx_BeginCanvasLayer
	ldr r2, .L_080e9a38
	ldr r3, .L_080e9a2c
	strh r3, [r2]
	ldr r3, [r5]
	add r5, sp, #40
	movs r4, #36
	ldrsh r0, [r3, r4]
	adds r1, r5, #0
	bl EffectPosition_ApplyAlternateStepAndYOffset
	ldr r3, [r5]
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	str r3, [sp, #16]
	movs r5, #2
	movs r1, #7
	movs r2, #7
	movs r3, #3
	movs r0, #46
	str r5, [sp, #0]
	bl Unnamed_080ed408
	ldr r0, [r6, #28]
	movs r3, #1
	str r0, [sp, #24]
	str r3, [sp, #0]
	movs r1, #7
	movs r2, #7
	movs r3, #3
	movs r0, #47
	b .L_080e9a3c
.L_080e9a2c:
	.4byte 0x00001010
.L_080e9a30:
	.4byte gBattleFxWork
.L_080e9a34:
	.4byte 0x00007828
.L_080e9a38:
	.4byte 0x04000052
.L_080e9a3c:
	bl Unnamed_080ed408
	ldr r3, .L_080e9c38
	ldr r2, [sp, #36]
	ldr r6, [r6, #32]
	adds r1, r2, r3
	ldr r0, .L_080e9c3c
	movs r2, #1
	movs r3, #1
	str r6, [sp, #28]
	bl Resource_LoadAndDecompress
	ldr r0, .L_080e9c40
	ldr r1, [sp, #36]
	movs r2, #1
	movs r3, #0
	bl Resource_LoadAndDecompress
	movs r6, #221
	ldr r4, [sp, #36]
	lsls r6, r6, #4
	adds r1, r4, r6
	ldr r0, .L_080e9c44
	movs r2, #1
	movs r3, #0
	bl Resource_LoadAndDecompress
	ldr r0, .L_080e9c48
	ldr r1, [sp, #20]
	movs r2, #0
	movs r3, #0
	bl Resource_LoadAndDecompress
	movs r1, #239
	ldr r0, [sp, #36]
	lsls r1, r1, #7
	adds r3, r0, r1
	str r5, [r3]
	ldr r3, .L_080e9c4c
	movs r1, #144
	adds r2, r0, r3
	movs r3, #75
	str r3, [r2]
	ldr r0, .L_080e9c50
	lsls r1, r1, #3
	bl Scheduler_AddOrUpdateCallback
	movs r6, #225
	ldr r5, [sp, #36]
	movs r4, #0
	lsls r6, r6, #7
	mov r8, r4
	adds r7, r5, r6
.L_080e9aa6:
	mov r0, r8
	lsls r6, r0, #1
	bl Random16
	ldr r3, .L_080e9c54
	adds r5, r0, #0
	ands r5, r3
	adds r0, r5, #0
	bl Trig_Sin
	adds r3, r6, #0
	muls r3, r0
	adds r0, r5, #0
	str r3, [r7]
	bl Trig_Cos
	adds r3, r6, #0
	muls r3, r0
	mov r1, r8
	negs r3, r3
	str r3, [r7, #4]
	lsrs r3, r1, #31
	add r3, r8
	asrs r3, r3, #1
	movs r2, #1
	adds r3, #25
	add r8, r2
	str r3, [r7, #24]
	mov r3, r8
	adds r7, #28
	cmp r3, #32
	bne .L_080e9aa6
	movs r4, #0
	movs r1, #1
	movs r2, #171
	ldr r3, .L_080e9c58
	mov r8, r4
	negs r1, r1
	lsls r2, r2, #2
.L_080e9af4:
	movs r5, #1
	add r8, r5
	str r1, [r3]
	adds r3, #28
	cmp r8, r2
	bne .L_080e9af4
	ldr r0, [sp, #16]
	lsls r0, r0, #16
	movs r6, #0
	str r0, [sp, #8]
	ldr r7, .L_080e9c5c
	mov r8, r6
.L_080e9b0c:
	bl Random16
	ldr r6, .L_080e9c60
	ands r6, r0
	bl Random16
	ldr r3, .L_080e9c54
	adds r5, r0, #0
	ldr r1, [sp, #8]
	ands r5, r3
	movs r3, #176
	lsls r3, r3, #15
	str r1, [r7]
	str r3, [r7, #4]
	adds r0, r5, #0
	bl Trig_Sin
	adds r6, #32
	adds r3, r6, #0
	muls r3, r0
	asrs r3, r3, #5
	str r3, [r7, #12]
	adds r0, r5, #0
	bl Trig_Cos
	adds r3, r6, #0
	muls r3, r0
	negs r3, r3
	asrs r3, r3, #6
	str r3, [r7, #16]
	bl Random16
	movs r3, #7
	ands r3, r0
	adds r3, #32
	str r3, [r7, #24]
	movs r2, #1
	movs r3, #170
	add r8, r2
	lsls r3, r3, #1
	adds r7, #28
	cmp r8, r3
	bne .L_080e9b0c
	movs r4, #0
	mov r11, r4
.L_080e9b66:
	mov r3, r11
	subs r3, #25
	cmp r3, #22
	bhi .L_080e9b74
	ldr r0, .L_080e9c64
	bl BattleFx_StepPaletteToResource
.L_080e9b74:
	mov r5, r11
	cmp r5, #56
	ble .L_080e9b80
	ldr r0, .L_080e9c68
	bl BattleFx_StepPaletteToResource
.L_080e9b80:
	mov r6, r11
	cmp r6, #8
	bne .L_080e9b8e
	ldr r0, [sp, #36]
	ldr r1, .L_080e9c6c
	adds r3, r0, r1
	str r6, [r3]
.L_080e9b8e:
	mov r2, r11
	cmp r2, #48
	bne .L_080e9b9e
	ldr r3, [sp, #36]
	ldr r4, .L_080e9c6c
	adds r2, r3, r4
	movs r3, #8
	str r3, [r2]
.L_080e9b9e:
	mov r5, r11
	cmp r5, #60
	bne .L_080e9bae
	ldr r6, [sp, #36]
	ldr r0, .L_080e9c6c
	movs r3, #16
	adds r2, r6, r0
	str r3, [r2]
.L_080e9bae:
	mov r1, r11
	cmp r1, #4
	bne .L_080e9bba
	movs r0, #212
	bl AudioCommand_PlayFar
.L_080e9bba:
	mov r2, r11
	cmp r2, #32
	bne .L_080e9bc6
	movs r0, #164
	bl AudioCommand_PlayFar
.L_080e9bc6:
	mov r3, r11
	cmp r3, #60
	bne .L_080e9bd8
	movs r0, #145
	bl AudioCommand_PlayFar
	movs r0, #134
	bl BattleEventRuntime_BeginPhaseFar
.L_080e9bd8:
	mov r4, r11
	cmp r4, #55
	ble .L_080e9c86
	ldr r6, .L_080e9c70
	ldr r0, [sp, #36]
	movs r1, #225
	movs r5, #0
	lsls r1, r1, #7
	mov r8, r5
	mov r10, r6
	adds r5, r0, r1
.L_080e9bee:
	ldr r0, [sp, #16]
	movs r4, #2
	ldrsh r3, [r5, r4]
	adds r6, r3, r0
	ldr r0, [r5, #24]
	movs r2, #6
	ldrsh r7, [r5, r2]
	cmp r0, #17
	bhi .L_080e9c30
	movs r1, #3
	bl FixedPoint_Ratio
	mov r2, r10
	ldrb r1, [r2, r0]
	ldr r3, [sp, #36]
	movs r0, #32
	lsls r1, r1, #11
	movs r4, #221
	adds r1, r3, r1
	lsls r4, r4, #4
	adds r2, r6, #0
	str r0, [sp, #0]
	adds r3, r7, #0
	movs r0, #64
	str r0, [sp, #4]
	adds r1, r1, r4
	subs r2, #16
	adds r3, #48
	ldr r0, [sp, #32]
	ldr r6, [sp, #24]
	bl _call_via_r6
	ldr r0, [r5, #24]
.L_080e9c30:
	cmp r0, #0
	ble .L_080e9c74
	subs r3, r0, #1
	b .L_080e9c78
.L_080e9c38:
	.4byte 0x00004e20
.L_080e9c3c:
	.4byte 0x00000056
.L_080e9c40:
	.4byte 0x00000085
.L_080e9c44:
	.4byte 0x0000007d
.L_080e9c48:
	.4byte 0x00000073
.L_080e9c4c:
	.4byte 0x00007784
.L_080e9c50:
	.4byte BattlePresentation_ProcessPendingGraphicsTransfer
.L_080e9c54:
	.4byte 0x0000ffff
.L_080e9c58:
	.4byte gMapCellBuffer + 0x18
.L_080e9c5c:
	.4byte gMapCellBuffer + 0x4ad0
.L_080e9c60:
	.4byte 0x000001ff
.L_080e9c64:
	.4byte 0x000000c0
.L_080e9c68:
	.4byte 0x000000c4
.L_080e9c6c:
	.4byte 0x000077a8
.L_080e9c70:
	.4byte Data_080eef12
.L_080e9c74:
	movs r3, #1
	negs r3, r3
.L_080e9c78:
	str r3, [r5, #24]
	movs r0, #1
	add r8, r0
	mov r1, r8
	adds r5, #28
	cmp r1, #16
	bne .L_080e9bee
.L_080e9c86:
	mov r2, r11
	cmp r2, #28
	bne .L_080e9d0a
	movs r3, #0
	movs r4, #63
	ldr r7, .L_080e9f8c
	mov r8, r3
	mov r10, r4
.L_080e9c96:
	movs r5, #1
	ldr r3, [r7, #24]
	negs r5, r5
	cmp r3, r5
	bne .L_080e9cfc
	bl Random16
	adds r6, r0, #0
	mov r0, r10
	ands r6, r0
	bl Random16
	ldr r3, .L_080e9f90
	adds r5, r0, #0
	ands r5, r3
	adds r0, r5, #0
	bl Trig_Sin
	adds r3, r6, #0
	muls r3, r0
	ldr r1, [sp, #8]
	asrs r3, r3, #3
	adds r3, r3, r1
	str r3, [r7]
	adds r0, r5, #0
	bl Trig_Cos
	adds r3, r6, #0
	muls r3, r0
	movs r2, #192
	lsls r2, r2, #15
	asrs r3, r3, #2
	adds r3, r3, r2
	str r3, [r7, #4]
	bl Random16
	mov r3, r10
	ands r0, r3
	subs r0, #32
	lsls r0, r0, #14
	str r0, [r7, #12]
	bl Random16
	mov r4, r10
	ands r0, r4
	negs r0, r0
	subs r0, #8
	lsls r0, r0, #13
	movs r3, #0
	str r0, [r7, #16]
	str r3, [r7, #24]
.L_080e9cfc:
	movs r5, #1
	movs r6, #128
	add r8, r5
	lsls r6, r6, #1
	adds r7, #28
	cmp r8, r6
	bne .L_080e9c96
.L_080e9d0a:
	mov r0, r11
	subs r0, #32
	str r0, [sp, #12]
	cmp r0, #31
	bhi .L_080e9dec
	movs r1, #0
	movs r2, #63
	ldr r7, .L_080e9f8c
	mov r9, r1
	mov r8, r1
	mov r10, r2
.L_080e9d20:
	movs r4, #1
	ldr r3, [r7, #24]
	negs r4, r4
	cmp r3, r4
	bne .L_080e9d90
	bl Random16
	adds r6, r0, #0
	bl Random16
	mov r5, r10
	ldr r3, .L_080e9f90
	ands r6, r5
	adds r5, r0, #0
	ands r5, r3
	adds r0, r5, #0
	bl Trig_Sin
	adds r3, r6, #0
	muls r3, r0
	ldr r0, [sp, #8]
	asrs r3, r3, #3
	adds r3, r3, r0
	str r3, [r7]
	adds r0, r5, #0
	bl Trig_Cos
	adds r3, r6, #0
	muls r3, r0
	movs r1, #192
	lsls r1, r1, #15
	asrs r3, r3, #2
	adds r3, r3, r1
	str r3, [r7, #4]
	bl Random16
	mov r2, r10
	ands r0, r2
	subs r0, #32
	lsls r0, r0, #14
	str r0, [r7, #12]
	bl Random16
	mov r3, r10
	ands r0, r3
	negs r0, r0
	movs r4, #1
	subs r0, #8
	add r9, r4
	lsls r0, r0, #13
	movs r3, #0
	mov r5, r9
	str r0, [r7, #16]
	str r3, [r7, #24]
	cmp r5, #16
	beq .L_080e9d9e
.L_080e9d90:
	movs r6, #1
	movs r0, #171
	add r8, r6
	lsls r0, r0, #2
	adds r7, #28
	cmp r8, r0
	bne .L_080e9d20
.L_080e9d9e:
	ldr r1, [sp, #12]
	cmp r1, #31
	bhi .L_080e9dec
	mov r2, r11
	ldr r3, [sp, #16]
	ldr r1, .L_080e9f94
	lsls r0, r2, #4
	movs r4, #34
	subs r3, #17
	adds r0, r0, r1
	movs r1, #104
	mov r10, r3
	mov r8, r4
	ldr r5, [sp, #24]
	bl Math_Mod
	mov r9, r5
	mov r2, r8
	adds r5, r0, #0
	movs r6, #104
	movs r3, #4
	subs r3, r3, r5
	str r2, [sp, #0]
	ldr r1, [sp, #36]
	mov r2, r10
	str r6, [sp, #4]
	ldr r0, [sp, #32]
	bl _call_via_r9
	movs r3, #108
	mov r4, r8
	subs r3, r3, r5
	str r4, [sp, #0]
	str r5, [sp, #4]
	ldr r0, [sp, #32]
	ldr r1, [sp, #36]
	mov r2, r10
	bl _call_via_r9
.L_080e9dec:
	mov r5, r11
	cmp r5, #71
	bgt .L_080e9ee8
	movs r6, #0
	ldr r5, .L_080e9f8c
	mov r8, r6
.L_080e9df8:
	ldr r3, [r5, #24]
	cmp r3, #0
	blt .L_080e9eda
	mov r0, r8
	movs r1, #3
	bl Math_Mod
	ldr r3, [r5, #16]
	adds r4, r0, #2
	cmp r3, #0
	ble .L_080e9e10
	adds r4, #2
.L_080e9e10:
	mov r0, r11
	cmp r0, #68
	ble .L_080e9e1c
	cmp r4, #5
	bgt .L_080e9e1c
	movs r4, #6
.L_080e9e1c:
	mov r1, r11
	cmp r1, #70
	ble .L_080e9e28
	cmp r4, #6
	bgt .L_080e9e28
	movs r4, #7
.L_080e9e28:
	mov r2, r11
	cmp r2, #72
	ble .L_080e9e34
	cmp r4, #7
	bgt .L_080e9e34
	movs r4, #8
.L_080e9e34:
	mov r3, r11
	cmp r3, #74
	ble .L_080e9e40
	cmp r4, #8
	bgt .L_080e9e40
	movs r4, #9
.L_080e9e40:
	mov r6, r11
	cmp r6, #76
	ble .L_080e9e48
	movs r4, #10
.L_080e9e48:
	lsls r0, r4, #1
	ldr r2, .L_080e9f98
	subs r3, r0, #2
	ldrh r1, [r2, r3]
	ldr r2, [sp, #20]
	adds r1, r2, r1
	movs r3, #2
	ldrsh r2, [r5, r3]
	lsrs r3, r4, #31
	adds r3, r4, r3
	asrs r3, r3, #1
	subs r2, r2, r3
	movs r6, #6
	ldrsh r3, [r5, r6]
	str r4, [sp, #0]
	subs r3, r3, r4
	str r0, [sp, #4]
	ldr r4, [sp, #24]
	ldr r0, [sp, #32]
	bl _call_via_r4
	ldr r3, [r5]
	ldr r2, [r5, #12]
	adds r3, r3, r2
	str r3, [r5]
	ldr r1, [r5, #16]
	ldr r3, [r5, #4]
	mov r6, r11
	adds r3, r3, r1
	str r3, [r5, #4]
	cmp r6, #80
	ble .L_080e9e8e
	ldr r0, .L_080e9f9c
	adds r3, r1, r0
	b .L_080e9e9c
.L_080e9e8e:
	movs r2, #3
	mov r4, r8
	ldr r3, .L_080e9fa0
	ands r2, r4
	lsls r2, r2, #2
	ldr r3, [r3, r2]
	adds r3, r1, r3
.L_080e9e9c:
	str r3, [r5, #16]
	ldr r2, [r5, #12]
	lsls r3, r2, #5
	subs r3, r3, r2
	lsls r3, r3, #1
	cmp r3, #0
	bge .L_080e9eac
	adds r3, #63
.L_080e9eac:
	ldr r2, [r5, #16]
	asrs r3, r3, #6
	str r3, [r5, #12]
	lsls r3, r2, #5
	subs r3, r3, r2
	lsls r2, r3, #1
	cmp r2, #0
	bge .L_080e9ebe
	adds r2, #63
.L_080e9ebe:
	ldr r3, [r5, #24]
	asrs r2, r2, #6
	adds r3, #1
	str r2, [r5, #16]
	str r3, [r5, #24]
	cmp r2, #0
	ble .L_080e9eda
	movs r6, #6
	ldrsh r3, [r5, r6]
	cmp r3, #108
	ble .L_080e9eda
	movs r3, #1
	negs r3, r3
	str r3, [r5, #24]
.L_080e9eda:
	movs r0, #1
	movs r1, #171
	add r8, r0
	lsls r1, r1, #1
	adds r5, #28
	cmp r8, r1
	bne .L_080e9df8
.L_080e9ee8:
	mov r2, r11
	cmp r2, #95
	bgt .L_080e9f4e
	ldr r2, [sp, #16]
	mov r3, r11
	subs r2, #18
	movs r1, #120
	cmp r3, #60
	ble .L_080e9f02
	ldr r5, .L_080e9fa4
	lsls r3, r3, #3
	adds r4, r3, r5
	b .L_080e9f26
.L_080e9f02:
	mov r6, r11
	cmp r6, #32
	ble .L_080e9f16
	ldr r0, [sp, #12]
	lsrs r3, r0, #31
	adds r3, r0, r3
	asrs r3, r3, #1
	adds r4, r3, #0
	adds r4, #16
	b .L_080e9f26
.L_080e9f16:
	mov r3, r11
	cmp r3, #9
	bgt .L_080e9f24
	lsls r3, r3, #4
	adds r4, r3, #0
	subs r4, #128
	b .L_080e9f26
.L_080e9f24:
	movs r4, #16
.L_080e9f26:
	adds r3, r4, #0
	adds r3, #120
	cmp r3, #108
	ble .L_080e9f34
	subs r3, r1, r4
	adds r1, r3, #0
	subs r1, #12
.L_080e9f34:
	cmp r1, #0
	ble .L_080e9f4e
	ldr r5, [sp, #36]
	ldr r6, .L_080e9fa8
	movs r3, #36
	str r3, [sp, #0]
	str r1, [sp, #4]
	adds r3, r4, #0
	ldr r0, [sp, #32]
	adds r1, r5, r6
	ldr r4, [sp, #28]
	bl _call_via_r4
.L_080e9f4e:
	mov r5, r11
	cmp r5, #59
	ble .L_080e9ffc
	movs r6, #0
	ldr r7, .L_080e9fac
	mov r8, r6
.L_080e9f5a:
	ldr r3, [r7, #24]
	cmp r3, #0
	ble .L_080e9fee
	movs r2, #128
	adds r0, r7, #0
	movs r1, #64
	lsls r2, r2, #6
	bl EffectStep_AdvanceWithGravity2D
	ldr r3, [r7, #24]
	movs r1, #216
	ldr r6, [r7, #4]
	subs r0, r3, #1
	lsls r1, r1, #15
	str r0, [r7, #24]
	cmp r6, r1
	ble .L_080e9fb0
	ldr r3, [r7, #16]
	negs r3, r3
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	str r3, [r7, #16]
	b .L_080e9fee
	.2byte 0x0000
.L_080e9f8c:
	.4byte gMapCellBuffer
.L_080e9f90:
	.4byte 0x0000ffff
.L_080e9f94:
	.4byte 0xffffff00
.L_080e9f98:
	.4byte ParticleStreams_CellOffsets
.L_080e9f9c:
	.4byte 0xffff8000
.L_080e9fa0:
	.4byte Data_080eef18
.L_080e9fa4:
	.4byte 0xfffffe3e
.L_080e9fa8:
	.4byte 0x00004e20
.L_080e9fac:
	.4byte gMapCellBuffer + 0x4ad0
.L_080e9fb0:
	ldr r5, [r7]
	ldr r2, .L_080ea0bc
	cmp r5, r2
	bhi .L_080e9fee
	cmp r6, #0
	blt .L_080e9fee
	movs r1, #5
	bl FixedPoint_Ratio
	adds r0, #1
	lsls r4, r0, #1
	ldr r2, .L_080ea0c0
	subs r3, r4, #2
	ldrh r1, [r2, r3]
	ldr r3, [sp, #20]
	adds r1, r3, r1
	lsrs r3, r0, #31
	adds r3, r0, r3
	asrs r3, r3, #1
	asrs r5, r5, #16
	asrs r6, r6, #16
	subs r5, r5, r3
	subs r6, r6, r0
	str r0, [sp, #0]
	str r4, [sp, #4]
	ldr r0, [sp, #32]
	adds r2, r5, #0
	adds r3, r6, #0
	ldr r4, [sp, #24]
	bl _call_via_r4
.L_080e9fee:
	movs r5, #1
	movs r6, #170
	add r8, r5
	lsls r6, r6, #1
	adds r7, #28
	cmp r8, r6
	bne .L_080e9f5a
.L_080e9ffc:
	mov r0, r11
	cmp r0, #68
	bne .L_080ea042
	ldr r3, .L_080ea0c4
	ldr r2, [sp, #36]
	ldr r3, [r2, r3]
	ldr r3, [r3, #20]
	movs r1, #0
	mov r8, r1
	cmp r3, #0
	beq .L_080ea042
	ldr r3, .L_080ea0c4
	movs r6, #36
	adds r5, r2, r3
.L_080ea018:
	ldr r3, [r5]
	ldrsh r0, [r3, r6]
	movs r3, #16
	str r3, [sp, #0]
	movs r1, #7
	movs r2, #5
	mov r3, r8
	bl ObjectGroup_UpdateMembers
	ldr r3, [r5]
	ldrsh r0, [r3, r6]
	movs r1, #7
	bl BattleMotion_ApplyVariantMotionFar
	ldr r3, [r5]
	movs r2, #1
	ldr r3, [r3, #20]
	add r8, r2
	adds r6, #2
	cmp r8, r3
	bne .L_080ea018
.L_080ea042:
	mov r3, r11
	cmp r3, #9
	bne .L_080ea056
	movs r1, #128
	ldr r3, .L_080ea0c8
	ldr r0, [sp, #32]
	lsls r1, r1, #7
	ldr r2, .L_080ea0cc
	bl _call_via_r3
.L_080ea056:
	mov r4, r11
	cmp r4, #60
	bne .L_080ea06a
	movs r1, #128
	ldr r3, .L_080ea0c8
	ldr r0, [sp, #32]
	lsls r1, r1, #7
	ldr r2, .L_080ea0cc
	bl _call_via_r3
.L_080ea06a:
	movs r1, #16
	movs r0, #16
	bl Camera_ApplyShake
	bl ObjectGroup_TickMemberTimers
	ldr r6, .L_080ea0d0
	ldr r5, [sp, #36]
	movs r3, #1
	adds r2, r5, r6
	movs r0, #1
	str r3, [r2]
	bl WaitFrames
	movs r0, #1
	add r11, r0
	mov r1, r11
	cmp r1, #102
	beq .L_080ea092
	b .L_080e9b66
.L_080ea092:
	ldr r0, .L_080ea0d4
	bl Scheduler_RemoveCallback
	movs r0, #47
	bl Runtime_ReleaseHeapBlock
	movs r0, #46
	bl Runtime_ReleaseHeapBlock
	bl BattleFx_EndCanvasLayer
	add sp, #52
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
.L_080ea0bc:
	.4byte 0x007effff
.L_080ea0c0:
	.4byte ParticleStreams_CellOffsets
.L_080ea0c4:
	.4byte 0x00007828
.L_080ea0c8:
	.4byte IwramFillWords
.L_080ea0cc:
	.4byte 0x3f3f3f3f
.L_080ea0d0:
	.4byte 0x00007824
.L_080ea0d4:
	.4byte BattlePresentation_ProcessPendingGraphicsTransfer
