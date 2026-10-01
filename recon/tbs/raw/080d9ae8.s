.syntax unified
	.thumb
	.global RunPaletteRampEffect
	.thumb_func
RunPaletteRampEffect:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #100
	str r1, [sp, #52]
	ldr r3, .L_080d9b30
	ldmia r3!, {r1}
	str r1, [sp, #48]
	ldr r2, .L_080d9b34
	ldr r3, [r3]
	str r3, [sp, #44]
	adds r3, r1, r2
	str r0, [r3]
	movs r0, #0
	bl BattleFx_BeginCanvasLayer
	ldr r3, .L_080d9b2c
	ldr r2, .L_080d9b38
	strh r3, [r2]
	ldr r3, [sp, #52]
	cmp r3, #0
	bne .L_080d9b40
	ldr r0, .L_080d9b3c
	ldr r1, [sp, #48]
	movs r2, #1
	movs r3, #1
	bl Resource_LoadAndDecompress
	b .L_080d9b4c
	.2byte 0x0000
.L_080d9b2c:
	.4byte 0x00000100
.L_080d9b30:
	.4byte gBattleFxWork
.L_080d9b34:
	.4byte 0x00007828
.L_080d9b38:
	.4byte 0x04000020
.L_080d9b3c:
	.4byte 0x0000009c
.L_080d9b40:
	ldr r0, .L_080d9dec
	ldr r1, [sp, #48]
	movs r2, #1
	movs r3, #1
	bl Resource_LoadAndDecompress
.L_080d9b4c:
	ldr r5, [sp, #52]
	cmp r5, #0
	beq .L_080d9b5c
	ldr r0, [sp, #52]
	cmp r0, #1
	bne .L_080d9b5c
	ldr r0, .L_080d9df0
	b .L_080d9b5e
.L_080d9b5c:
	ldr r0, .L_080d9df4
.L_080d9b5e:
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_080d9df8
	movs r2, #128
	lsls r0, r0, #19
	bl _call_via_r3
	movs r3, #150
	ldr r2, [sp, #48]
	lsls r3, r3, #6
	adds r1, r2, r3
	ldr r0, .L_080d9dfc
	movs r2, #0
	movs r3, #0
	bl Resource_LoadAndDecompress
	movs r0, #174
	movs r5, #1
	lsls r0, r0, #2
	mov r9, r5
	mov lr, r0
	movs r4, #57
.L_080d9b8e:
	ldr r3, [sp, #48]
	movs r2, #150
	movs r1, #0
	lsls r2, r2, #6
	str r1, [sp, #36]
	adds r1, r3, r2
	adds r3, r0, r3
	mov r12, r4
	adds r3, r3, r2
.L_080d9ba0:
	ldrb r2, [r1]
	adds r1, #1
	cmp r2, r12
	ble .L_080d9baa
	mov r2, r12
.L_080d9baa:
	cmp r2, #0
	bge .L_080d9bb0
	movs r2, #0
.L_080d9bb0:
	strb r2, [r3]
	ldr r5, [sp, #36]
	adds r5, #1
	adds r3, #1
	str r5, [sp, #36]
	cmp r5, lr
	bne .L_080d9ba0
	movs r2, #1
	movs r1, #174
	add r9, r2
	lsls r1, r1, #2
	mov r3, r9
	adds r0, r0, r1
	subs r4, #7
	cmp r3, #8
	bne .L_080d9b8e
	ldr r5, [sp, #48]
	ldr r0, .L_080d9e00
	adds r3, r5, r0
	ldr r3, [r3]
	ldr r3, [r3, #4]
	cmp r3, #1
	bne .L_080d9bec
	ldr r2, .L_080d9e04
	ldr r3, .L_080d9e08
	movs r1, #112
	negs r1, r1
	str r3, [r2]
	str r1, [sp, #32]
	b .L_080d9bf4
.L_080d9bec:
	ldr r2, .L_080d9e04
	movs r3, #0
	str r3, [r2]
	str r3, [sp, #32]
.L_080d9bf4:
	movs r2, #0
	ldr r5, .L_080d9e0c
	mov r9, r2
	movs r7, #192
.L_080d9bfc:
	bl Random16
	ldr r3, .L_080d9e10
	adds r6, r0, #0
	ands r6, r3
	movs r3, #0
	str r3, [r5]
	ldr r3, [sp, #52]
	cmp r3, #0
	bne .L_080d9c42
	movs r2, #31
	mov r0, r9
	ands r2, r0
	cmp r2, #0
	bge .L_080d9c1c
	adds r2, #3
.L_080d9c1c:
	asrs r2, r2, #2
	lsls r3, r2, #1
	adds r3, r3, r2
	ldr r1, .L_080d9e14
	lsls r3, r3, #17
	adds r3, r3, r1
	str r3, [r5, #4]
	mov r3, r9
	cmp r3, #0
	bge .L_080d9c32
	adds r3, #3
.L_080d9c32:
	asrs r3, r3, #2
	lsls r3, r3, #2
	mov r2, r9
	subs r3, r2, r3
	ldr r0, .L_080d9e18
	lsls r3, r3, #17
	adds r3, r3, r0
	b .L_080d9c72
.L_080d9c42:
	movs r2, #31
	mov r1, r9
	ands r2, r1
	cmp r2, #0
	bge .L_080d9c4e
	adds r2, #3
.L_080d9c4e:
	asrs r2, r2, #2
	lsls r3, r2, #1
	adds r3, r3, r2
	ldr r2, .L_080d9e14
	lsls r3, r3, #17
	adds r3, r3, r2
	str r3, [r5, #4]
	mov r3, r9
	cmp r3, #0
	bge .L_080d9c64
	adds r3, #3
.L_080d9c64:
	asrs r3, r3, #2
	lsls r3, r3, #2
	mov r0, r9
	subs r3, r0, r3
	ldr r1, .L_080d9e1c
	lsls r3, r3, #19
	adds r3, r3, r1
.L_080d9c72:
	str r3, [r5, #8]
	ldr r2, [sp, #48]
	ldr r0, .L_080d9e00
	adds r3, r2, r0
	ldr r3, [r3]
	ldr r3, [r3, #4]
	cmp r3, #1
	bne .L_080d9c88
	movs r3, #128
	lsls r3, r3, #10
	b .L_080d9c8a
.L_080d9c88:
	ldr r3, .L_080d9e18
.L_080d9c8a:
	str r3, [r5, #12]
	adds r0, r6, #0
	bl Trig_Cos
	adds r3, r7, #0
	muls r3, r0
	movs r1, #128
	lsls r1, r1, #9
	asrs r3, r3, #6
	adds r3, r3, r1
	str r3, [r5, #16]
	adds r0, r6, #0
	bl Trig_Sin
	adds r3, r7, #0
	muls r3, r0
	asrs r3, r3, #6
	str r3, [r5, #20]
	bl Random16
	movs r3, #255
	ands r3, r0
	str r3, [r5, #24]
	movs r2, #1
	movs r3, #128
	add r9, r2
	lsls r3, r3, #2
	adds r5, #28
	cmp r9, r3
	bne .L_080d9bfc
	ldr r0, [sp, #48]
	ldr r1, .L_080d9e00
	adds r5, r0, r1
	ldr r3, [r5]
	mov r2, sp
	adds r2, #56
	ldr r0, [r3, #4]
	adds r1, r2, #0
	str r2, [sp, #28]
	bl BattleFx_FetchRectangleBlitters
	movs r0, #239
	ldr r3, [sp, #48]
	lsls r0, r0, #7
	adds r2, r3, r0
	movs r3, #2
	str r3, [r2]
	ldr r1, [sp, #48]
	ldr r3, .L_080d9e20
	adds r2, r1, r3
	movs r3, #50
	movs r1, #144
	str r3, [r2]
	lsls r1, r1, #3
	ldr r0, .L_080d9e24
	bl Scheduler_AddOrUpdateCallback
	movs r0, #0
	str r0, [sp, #40]
	ldr r3, [r5]
	ldr r3, [r3, #20]
	movs r1, #64
	lsls r3, r3, #2
	negs r1, r1
	cmp r3, r1
	bne .L_080d9d10
	b .L_080d9f82
.L_080d9d10:
	ldr r3, .L_080d9e28
	ldr r2, [sp, #40]
	ldr r3, [r3]
	str r3, [sp, #24]
	cmp r2, #72
	bne .L_080d9d22
	movs r0, #0
	bl BattleEventRuntime_BeginPhaseFar
.L_080d9d22:
	movs r3, #0
	str r3, [sp, #36]
	ldr r2, .L_080d9e00
	ldr r5, [sp, #48]
	ldr r3, [r5, r2]
	ldr r3, [r3, #20]
	cmp r3, #0
	bne .L_080d9d34
	b .L_080d9f58
.L_080d9d34:
	ldr r0, [sp, #40]
	ldr r3, [sp, #24]
	subs r0, #24
	ldr r1, [sp, #40]
	adds r3, #12
	movs r5, #0
	str r0, [sp, #12]
	str r3, [sp, #20]
	str r5, [sp, #8]
	mov r8, r1
.L_080d9d48:
	ldr r0, [sp, #48]
	ldr r1, [sp, #36]
	ldr r2, [r0, r2]
	lsls r3, r1, #1
	adds r3, #36
	ldrsh r0, [r2, r3]
	bl GetBattleObjectSlotFar
	ldr r6, [r0]
	mov r0, r8
	cmp r0, #0
	bgt .L_080d9d62
	b .L_080d9f2c
.L_080d9d62:
	bl Render_ResetTransformState
	ldr r0, [sp, #24]
	ldr r1, [sp, #20]
	bl Graphics_PrepareTransferInIwramWork
	ldr r3, [r6, #8]
	add r5, sp, #64
	str r3, [r5]
	movs r3, #160
	lsls r3, r3, #13
	str r3, [r5, #4]
	ldr r3, [r6, #16]
	str r3, [r5, #8]
	bl Render_ResetTransformState
	ldr r0, [sp, #24]
	ldr r1, [sp, #20]
	bl Graphics_PrepareTransferInIwramWork
	adds r0, r5, #0
	bl SceneTransform_ApplyPosition
	movs r3, #0
	add r0, sp, #88
	add r5, sp, #76
	str r3, [r0]
	str r3, [r0, #4]
	str r3, [r0, #8]
	adds r1, r5, #0
	bl EffectPosition_ApplyBaseAndYOffset
	ldr r3, [r5]
	ldr r1, [sp, #32]
	ldr r2, [r5, #4]
	adds r6, r3, r1
	ldr r3, [sp, #52]
	mov r10, r2
	cmp r3, #0
	bne .L_080d9e2c
	mov r0, r8
	cmp r0, #26
	bgt .L_080d9e68
	cmp r0, #0
	bge .L_080d9dbe
	adds r0, #3
.L_080d9dbe:
	movs r1, #7
	asrs r0, r0, #2
	bl __modsi3
	lsls r1, r0, #4
	subs r1, r1, r0
	ldr r2, [sp, #48]
	movs r0, #24
	lsls r1, r1, #6
	adds r1, r2, r1
	mov r3, r10
	adds r2, r6, #0
	str r0, [sp, #0]
	movs r0, #40
	str r0, [sp, #4]
	subs r2, #12
	subs r3, #20
	ldr r4, [sp, #56]
	ldr r0, [sp, #44]
	bl _call_via_r4
	b .L_080d9e68
	.2byte 0x0000
.L_080d9dec:
	.4byte 0x0000009b
.L_080d9df0:
	.4byte 0x000000b7
.L_080d9df4:
	.4byte 0x000000bb
.L_080d9df8:
	.4byte IwramCopyWords
.L_080d9dfc:
	.4byte 0x0000009d
.L_080d9e00:
	.4byte 0x00007828
.L_080d9e04:
	.4byte 0x04000028
.L_080d9e08:
	.4byte 0xffff9000
.L_080d9e0c:
	.4byte gMapCellBuffer
.L_080d9e10:
	.4byte 0x0000ffff
.L_080d9e14:
	.4byte 0xfff60000
.L_080d9e18:
	.4byte 0xfffe0000
.L_080d9e1c:
	.4byte 0xfff00000
.L_080d9e20:
	.4byte 0x00007784
.L_080d9e24:
	.4byte BattlePresentation_ProcessPendingGraphicsTransfer
.L_080d9e28:
	.4byte gCameraWork
.L_080d9e2c:
	mov r3, r8
	cmp r3, #23
	bgt .L_080d9e68
	mov r0, r8
	cmp r3, #0
	bge .L_080d9e3a
	adds r0, #3
.L_080d9e3a:
	movs r1, #6
	asrs r0, r0, #2
	bl __modsi3
	lsls r1, r0, #1
	adds r1, r1, r0
	lsls r1, r1, #3
	adds r1, r1, r0
	ldr r0, [sp, #48]
	lsls r1, r1, #6
	adds r1, r0, r1
	movs r0, #40
	str r0, [sp, #0]
	str r0, [sp, #4]
	ldr r0, [sp, #28]
	adds r2, r6, #0
	mov r3, r10
	ldr r4, [r0, #4]
	subs r2, #20
	subs r3, #20
	ldr r0, [sp, #44]
	bl _call_via_r4
.L_080d9e68:
	mov r1, r8
	cmp r1, #24
	bne .L_080d9e74
	movs r0, #143
	bl AudioCommand_PlayFar
.L_080d9e74:
	ldr r2, [sp, #12]
	cmp r2, #36
	bhi .L_080d9f2c
	mov r3, r8
	movs r1, #0
	cmp r3, #28
	ble .L_080d9e94
	adds r3, r2, #0
	cmp r2, #0
	bge .L_080d9e8c
	mov r3, r8
	subs r3, #21
.L_080d9e8c:
	asrs r1, r3, #2
	cmp r1, #7
	ble .L_080d9e94
	movs r1, #7
.L_080d9e94:
	movs r3, #174
	lsls r3, r3, #2
	adds r2, r1, #0
	muls r2, r3
	mov r11, r5
	ldr r3, [sp, #8]
	ldr r5, .L_080d9fac
	movs r0, #0
	str r2, [sp, #16]
	mov r9, r0
	adds r7, r3, r5
.L_080d9eaa:
	mov r3, r9
	cmp r3, #0
	bge .L_080d9eb2
	adds r3, #3
.L_080d9eb2:
	asrs r3, r3, #2
	mov r0, r9
	lsls r3, r3, #2
	subs r3, r0, r3
	lsls r2, r3, #1
	adds r6, r2, r3
	ldr r3, [r7, #24]
	mov r1, r8
	adds r0, r3, r1
	cmp r0, #0
	bge .L_080d9eca
	adds r0, #7
.L_080d9eca:
	movs r1, #3
	asrs r0, r0, #3
	bl __modsi3
	mov r1, r11
	adds r5, r0, #0
	adds r0, r7, #0
	bl EffectPosition_ApplyBaseAndYOffset
	mov r2, r11
	ldr r3, [r2]
	ldr r0, [sp, #32]
	ldr r1, [r2, #4]
	adds r5, r6, r5
	ldr r2, .L_080d9fb0
	adds r6, r3, r0
	lsls r3, r5, #1
	mov r10, r1
	ldrh r1, [r2, r3]
	ldr r2, [sp, #16]
	ldr r3, [sp, #48]
	adds r1, r2, r1
	adds r1, r3, r1
	ldr r3, .L_080d9fb4
	ldrb r3, [r3, r5]
	str r3, [sp, #0]
	ldr r3, .L_080d9fb8
	movs r0, #150
	ldrb r3, [r3, r5]
	lsls r0, r0, #6
	adds r1, r1, r0
	str r3, [sp, #4]
	ldr r4, [sp, #56]
	ldr r0, [sp, #44]
	adds r2, r6, #0
	mov r3, r10
	bl _call_via_r4
	adds r0, r7, #0
	movs r1, #60
	movs r2, #0
	bl EffectStep_AdvanceWithGravity3D
	movs r1, #1
	add r9, r1
	mov r2, r9
	adds r7, #28
	cmp r2, #24
	bne .L_080d9eaa
.L_080d9f2c:
	ldr r3, [sp, #12]
	ldr r0, [sp, #8]
	ldr r2, [sp, #36]
	movs r1, #224
	movs r5, #4
	lsls r1, r1, #2
	subs r3, #4
	negs r5, r5
	adds r0, r0, r1
	adds r2, #1
	str r2, [sp, #36]
	str r0, [sp, #8]
	str r3, [sp, #12]
	add r8, r5
	ldr r2, .L_080d9fbc
	ldr r5, [sp, #48]
	ldr r3, [r5, r2]
	ldr r0, [sp, #36]
	ldr r3, [r3, #20]
	cmp r0, r3
	beq .L_080d9f58
	b .L_080d9d48
.L_080d9f58:
	ldr r1, [sp, #48]
	ldr r3, .L_080d9fc0
	adds r2, r1, r3
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r5, [sp, #40]
	ldr r0, [sp, #48]
	adds r5, #1
	ldr r1, .L_080d9fbc
	str r5, [sp, #40]
	adds r3, r0, r1
	ldr r3, [r3]
	ldr r3, [r3, #20]
	lsls r3, r3, #2
	adds r3, #64
	cmp r5, r3
	beq .L_080d9f82
	b .L_080d9d10
.L_080d9f82:
	ldr r0, .L_080d9fc4
	bl Scheduler_RemoveCallback
	movs r0, #47
	bl Runtime_ReleaseHeapBlock
	movs r0, #46
	bl Runtime_ReleaseHeapBlock
	bl BattleFx_EndCanvasLayer
	add sp, #100
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
.L_080d9fac:
	.4byte gMapCellBuffer
.L_080d9fb0:
	.4byte Data_080eea08
.L_080d9fb4:
	.4byte Data_080eea20
.L_080d9fb8:
	.4byte Data_080eea2c
.L_080d9fbc:
	.4byte 0x00007828
.L_080d9fc0:
	.4byte 0x00007824
.L_080d9fc4:
	.4byte BattlePresentation_ProcessPendingGraphicsTransfer
