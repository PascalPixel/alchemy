.syntax unified
	.thumb
	.global Unnamed_080d3c80
	.thumb_func
Unnamed_080d3c80:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, .L_080d3ce8
	ldmia r3!, {r1}
	ldr r5, .L_080d3cec
	mov r9, r1
	ldr r3, [r3]
	sub sp, #40
	add r5, r9
	str r3, [sp, #28]
	str r0, [r5]
	movs r0, #0
	bl BattleFx_BeginCanvasLayer
	ldr r2, .L_080d3cf0
	ldr r3, .L_080d3ce4
	ldr r0, .L_080d3cf4
	strh r3, [r2]
	mov r1, r9
	movs r2, #1
	movs r3, #1
	bl Resource_LoadAndDecompress
	ldr r3, [r5]
	mov r2, sp
	adds r2, #32
	ldr r0, [r3, #4]
	adds r1, r2, #0
	str r2, [sp, #20]
	bl BattleFx_FetchRectangleBlitters
	movs r2, #239
	lsls r2, r2, #7
	add r2, r9
	movs r3, #2
	str r3, [r2]
	ldr r2, .L_080d3cf8
	movs r3, #50
	add r2, r9
	movs r1, #144
	str r3, [r2]
	ldr r0, .L_080d3cfc
	lsls r1, r1, #3
	b .L_080d3d00
	.2byte 0x0000
.L_080d3ce4:
	.4byte 0x00001010
.L_080d3ce8:
	.4byte gBattleFxWork
.L_080d3cec:
	.4byte 0x00007828
.L_080d3cf0:
	.4byte 0x04000052
.L_080d3cf4:
	.4byte 0x000000cf
.L_080d3cf8:
	.4byte 0x00007784
.L_080d3cfc:
	.4byte BattlePresentation_ProcessPendingGraphicsTransfer
.L_080d3d00:
	bl Scheduler_AddOrUpdateCallback
	ldr r4, .L_080d3f20
	str r4, [sp, #12]
	movs r3, #0
	mov r10, r3
	ldr r3, [r5]
	ldr r3, [r3, #24]
	lsls r3, r3, #1
	ldrb r3, [r4, r3]
	cmp r3, #0
	beq .L_080d3d98
	movs r5, #31
	mov r11, r5
	ldr r7, .L_080d3f24
	movs r5, #225
	lsls r5, r5, #7
	movs r0, #0
	add r5, r9
	mov r8, r0
	add r7, r9
.L_080d3d2a:
	bl Random16
	ldr r3, .L_080d3f28
	str r3, [r5, #4]
	ldr r3, [r7]
	ldr r3, [r3, #4]
	cmp r3, #1
	bne .L_080d3d50
	bl Random16
	mov r1, r11
	ands r0, r1
	adds r0, #80
	lsls r6, r0, #16
	bl Random16
	movs r3, #63
	ands r3, r0
	b .L_080d3d66
.L_080d3d50:
	bl Random16
	mov r2, r11
	ands r0, r2
	adds r0, #8
	lsls r6, r0, #16
	bl Random16
	movs r3, #63
	ands r3, r0
	negs r3, r3
.L_080d3d66:
	lsls r3, r3, #12
	str r3, [r5, #12]
	ldr r2, [r5, #12]
	lsls r3, r2, #3
	adds r3, r3, r2
	lsls r3, r3, #1
	subs r3, r6, r3
	str r3, [r5]
	movs r3, #0
	str r3, [r5, #16]
	str r3, [r5, #8]
	mov r3, r8
	str r3, [r5, #24]
	ldr r3, [r7]
	ldr r3, [r3, #24]
	ldr r1, .L_080d3f20
	lsls r3, r3, #1
	movs r0, #1
	ldrb r3, [r1, r3]
	movs r4, #8
	add r10, r0
	adds r5, #28
	add r8, r4
	cmp r10, r3
	bne .L_080d3d2a
.L_080d3d98:
	movs r2, #0
	str r2, [sp, #24]
	ldr r2, .L_080d3f24
	mov r4, r9
	ldr r3, [r4, r2]
	ldr r3, [r3, #24]
	ldr r5, [sp, #12]
	lsls r3, r3, #1
	adds r3, #1
	ldrb r3, [r5, r3]
	cmp r3, #0
	bne .L_080d3db2
	b .L_080d40a6
.L_080d3db2:
	ldr r0, .L_080d3f24
	add r0, r9
	str r0, [sp, #16]
.L_080d3db8:
	mov r1, r9
	ldr r3, [r1, r2]
	ldr r3, [r3, #24]
	cmp r3, #2
	bne .L_080d3e02
	ldr r2, [sp, #24]
	cmp r2, #103
	bgt .L_080d3e02
	ldr r3, .L_080d3f2c
	ldr r1, [r3]
	ldr r3, [sp, #24]
	movs r2, #192
	cmp r3, #95
	ble .L_080d3de2
	ldr r4, [sp, #24]
	lsls r3, r4, #1
	adds r3, r3, r4
	movs r2, #156
	lsls r3, r3, #3
	lsls r2, r2, #4
	subs r2, r2, r3
.L_080d3de2:
	ldr r5, [sp, #16]
	ldr r3, [r5]
	ldr r3, [r3, #4]
	cmp r3, #0
	bne .L_080d3df8
	ldrh r3, [r1, #54]
	ldr r0, .L_080d3f20
	subs r3, r3, r2
	strh r3, [r1, #54]
	str r0, [sp, #12]
	b .L_080d3e02
.L_080d3df8:
	ldrh r3, [r1, #54]
	adds r3, r3, r2
	strh r3, [r1, #54]
	ldr r1, .L_080d3f20
	str r1, [sp, #12]
.L_080d3e02:
	ldr r5, .L_080d3f24
	add r5, r9
	ldr r2, [r5]
	ldr r3, [r2, #24]
	ldr r4, [sp, #12]
	lsls r3, r3, #1
	adds r3, #1
	ldrb r3, [r4, r3]
	ldr r0, [sp, #24]
	subs r3, #80
	cmp r0, r3
	bne .L_080d3e22
	movs r0, #134
	bl BattleEventRuntime_BeginPhaseFar
	ldr r2, [r5]
.L_080d3e22:
	ldr r3, [r2, #24]
	ldr r1, [sp, #12]
	lsls r3, r3, #1
	adds r3, #1
	ldrb r3, [r1, r3]
	ldr r4, [sp, #24]
	subs r3, #8
	cmp r4, r3
	bne .L_080d3e48
	movs r2, #239
	lsls r2, r2, #7
	add r2, r9
	movs r3, #3
	str r3, [r2]
	ldr r2, .L_080d3f30
	ldr r3, .L_080d3f34
	add r2, r9
	str r3, [r2]
	ldr r2, [r5]
.L_080d3e48:
	ldr r3, [r2, #24]
	ldr r5, [sp, #12]
	lsls r2, r3, #1
	adds r3, r2, #1
	ldrb r3, [r5, r3]
	ldr r0, [sp, #24]
	subs r3, #8
	cmp r0, r3
	ble .L_080d3e5c
	b .L_080d4060
.L_080d3e5c:
	ldrb r3, [r5, r2]
	movs r1, #0
	mov r11, r1
	cmp r3, #0
	bne .L_080d3e68
	b .L_080d4060
.L_080d3e68:
	movs r2, #225
	lsls r2, r2, #7
	add r2, r9
	mov r8, r2
.L_080d3e70:
	mov r4, r8
	ldr r3, [r4, #8]
	cmp r3, #1
	bne .L_080d3f48
	mov r0, r11
	lsls r2, r0, #4
	lsls r3, r0, #7
	subs r3, r3, r2
	ldr r1, .L_080d3f38
	movs r5, #0
	lsls r3, r3, #2
	mov r10, r5
	adds r7, r3, r1
.L_080d3e8a:
	movs r1, #5
	mov r0, r10
	bl Math_Mod
	lsls r5, r0, #1
	adds r5, r5, r0
	movs r1, #96
	ldr r0, [r7, #24]
	bl FixedPoint_Ratio
	movs r1, #3
	bl Math_Mod
	movs r2, #4
	mov r3, r10
	adds r6, r7, #0
	adds r4, r5, r0
	mov r12, r2
	cmp r3, #2
	ble .L_080d3eb6
	movs r5, #0
	mov r12, r5
.L_080d3eb6:
	ldr r2, .L_080d3f3c
	lsls r3, r4, #2
	ldr r1, [r2, r3]
	movs r3, #2
	ldrsh r2, [r6, r3]
	ldr r3, .L_080d3f40
	ldrb r5, [r3, r4]
	movs r0, #128
	lsls r0, r0, #4
	lsrs r3, r5, #1
	add r1, r9
	adds r1, r1, r0
	subs r2, r2, r3
	movs r0, #6
	ldrsh r3, [r6, r0]
	ldr r0, .L_080d3f44
	ldrb r4, [r0, r4]
	lsrs r0, r4, #1
	subs r3, r3, r0
	str r5, [sp, #0]
	ldr r0, [sp, #20]
	str r4, [sp, #4]
	mov r5, r12
	ldr r4, [r5, r0]
	ldr r0, [sp, #28]
	bl _call_via_r4
	movs r2, #128
	lsls r2, r2, #6
	adds r0, r6, #0
	movs r1, #64
	bl EffectStep_AdvanceWithGravity2D
	ldr r3, [r6, #24]
	ldr r2, [r6, #8]
	adds r3, r3, r2
	str r3, [r6, #24]
	cmp r2, #1
	ble .L_080d3f12
	ldr r1, [sp, #24]
	movs r3, #1
	ands r3, r1
	cmp r3, #0
	beq .L_080d3f12
	subs r3, r2, #1
	str r3, [r6, #8]
.L_080d3f12:
	movs r2, #1
	add r10, r2
	mov r3, r10
	adds r7, #28
	cmp r3, #16
	bne .L_080d3e8a
	b .L_080d4046
.L_080d3f20:
	.4byte Data_080ee1f5
.L_080d3f24:
	.4byte 0x00007828
.L_080d3f28:
	.4byte 0xffc00000
.L_080d3f2c:
	.4byte gCameraWork
.L_080d3f30:
	.4byte 0x00007784
.L_080d3f34:
	.4byte 0x06060606
.L_080d3f38:
	.4byte gMapCellBuffer
.L_080d3f3c:
	.4byte Data_080ee214
.L_080d3f40:
	.4byte Data_080ee1fb
.L_080d3f44:
	.4byte Data_080ee207
.L_080d3f48:
	mov r4, r8
	ldr r3, [r4, #24]
	ldr r5, [sp, #24]
	cmp r5, r3
	blt .L_080d4046
	movs r1, #6
	ldrsh r3, [r4, r1]
	movs r1, #2
	ldrsh r2, [r4, r1]
	movs r1, #32
	str r1, [sp, #0]
	movs r5, #1
	movs r1, #64
	mov r6, r11
	str r1, [sp, #4]
	ands r6, r5
	ldr r1, [sp, #20]
	lsls r0, r6, #2
	ldr r4, [r0, r1]
	subs r2, #16
	ldr r0, [sp, #28]
	mov r1, r9
	bl _call_via_r4
	movs r2, #128
	lsls r2, r2, #9
	mov r0, r8
	movs r1, #64
	bl EffectStep_AdvanceWithGravity2D
	mov r2, r8
	ldr r3, [r2, #4]
	movs r2, #224
	lsls r2, r2, #14
	cmp r3, r2
	ble .L_080d4046
	mov r3, r8
	str r5, [r3, #8]
	mov r5, r11
	str r2, [r3, #4]
	lsls r2, r5, #4
	lsls r3, r5, #7
	subs r3, r3, r2
	ldr r0, .L_080d40d0
	movs r4, #0
	lsls r3, r3, #2
	ldr r7, .L_080d40d4
	mov r10, r4
	movs r1, #127
	adds r5, r3, r0
.L_080d3fac:
	ldrb r3, [r7]
	mov r4, r8
	ldr r2, [r4]
	subs r3, #40
	lsls r3, r3, #16
	adds r3, r3, r2
	str r3, [r5]
	ldrb r3, [r7, #1]
	lsls r3, r3, #16
	str r3, [r5, #4]
	str r1, [sp, #8]
	bl Random16
	ldr r1, [sp, #8]
	ands r0, r1
	subs r0, #64
	lsls r0, r0, #11
	str r0, [r5, #12]
	bl Random16
	ldr r1, [sp, #8]
	ands r0, r1
	negs r0, r0
	lsls r3, r0, #11
	str r3, [r5, #16]
	cmp r6, #0
	beq .L_080d3fec
	ldr r3, [r5, #12]
	lsls r3, r3, #1
	str r3, [r5, #12]
	lsls r3, r0, #12
	str r3, [r5, #16]
.L_080d3fec:
	movs r0, #1
	movs r3, #32
	add r10, r0
	str r3, [r5, #8]
	mov r2, r10
	movs r3, #0
	str r3, [r5, #24]
	adds r7, #2
	adds r5, #28
	cmp r2, #16
	bne .L_080d3fac
	ldr r2, .L_080d40d8
	movs r3, #8
	add r2, r9
	str r3, [r2]
	movs r0, #144
	bl AudioCommand_PlayFar
	movs r3, #0
	mov r10, r3
	ldr r3, .L_080d40dc
	mov r4, r9
	ldr r3, [r4, r3]
	ldr r3, [r3, #20]
	cmp r3, #0
	beq .L_080d4046
	ldr r5, .L_080d40dc
	movs r6, #36
	add r5, r9
.L_080d4026:
	ldr r3, [r5]
	ldrsh r0, [r3, r6]
	movs r3, #4
	str r3, [sp, #0]
	movs r2, #5
	mov r3, r10
	movs r1, #7
	bl ObjectGroup_UpdateMembers
	ldr r3, [r5]
	movs r2, #1
	ldr r3, [r3, #20]
	add r10, r2
	adds r6, #2
	cmp r10, r3
	bne .L_080d4026
.L_080d4046:
	ldr r5, [sp, #16]
	movs r3, #28
	add r8, r3
	ldr r3, [r5]
	ldr r3, [r3, #24]
	ldr r0, .L_080d40e0
	lsls r3, r3, #1
	movs r4, #1
	ldrb r3, [r0, r3]
	add r11, r4
	cmp r11, r3
	beq .L_080d4060
	b .L_080d3e70
.L_080d4060:
	ldr r1, [sp, #16]
	ldr r3, [r1]
	ldr r1, [r3, #24]
	lsls r0, r1, #1
	lsls r1, r1, #2
	adds r0, #4
	adds r1, #8
	bl Camera_ApplyShake
	bl ObjectGroup_TickMemberTimers
	ldr r2, .L_080d40e4
	movs r3, #1
	add r2, r9
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r2, [sp, #24]
	ldr r3, .L_080d40e0
	adds r2, #1
	str r2, [sp, #24]
	str r3, [sp, #12]
	ldr r4, [sp, #16]
	ldr r3, [r4]
	ldr r3, [r3, #24]
	ldr r5, .L_080d40e0
	lsls r3, r3, #1
	adds r3, #1
	ldrb r3, [r5, r3]
	ldr r0, [sp, #24]
	ldr r2, .L_080d40dc
	cmp r0, r3
	beq .L_080d40a6
	b .L_080d3db8
.L_080d40a6:
	ldr r0, .L_080d40e8
	bl Scheduler_RemoveCallback
	movs r0, #47
	bl Runtime_ReleaseHeapBlock
	movs r0, #46
	bl Runtime_ReleaseHeapBlock
	bl BattleFx_EndCanvasLayer
	add sp, #40
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
.L_080d40d0:
	.4byte gMapCellBuffer
.L_080d40d4:
	.4byte Data_080ee1d3
.L_080d40d8:
	.4byte 0x000077a8
.L_080d40dc:
	.4byte 0x00007828
.L_080d40e0:
	.4byte Data_080ee1f5
.L_080d40e4:
	.4byte 0x00007824
.L_080d40e8:
	.4byte BattlePresentation_ProcessPendingGraphicsTransfer
