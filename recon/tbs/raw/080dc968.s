.syntax unified
	.thumb
	.global BattleEffect_RunStagedParticles
	.thumb_func
BattleEffect_RunStagedParticles:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r7, .L_080dc9f0
	ldr r1, [r7]
	sub sp, #120
	str r1, [sp, #56]
	adds r3, r7, #0
	subs r3, #16
	ldr r3, [r3]
	str r3, [sp, #52]
	adds r3, r7, #0
	subs r3, #20
	ldr r3, [r3]
	mov r11, r3
	adds r3, r7, #0
	subs r3, #12
	ldr r3, [r3]
	str r3, [sp, #36]
	ldr r3, .L_080dc9f4
	ldr r5, .L_080dc9f8
	ldrh r3, [r3, #4]
	add r5, r11
	str r3, [sp, #32]
	str r0, [r5]
	movs r0, #128
	lsls r0, r0, #6
	bl BattleFx_BeginCanvasLayer
	ldr r2, .L_080dc9fc
	ldr r3, .L_080dc9e8
	mov r9, r2
	mov r0, r9
	strh r3, [r0]
	bl BattlePres_ConfigureEffectDisplay
	movs r1, #160
	lsls r1, r1, #19
	ldr r2, .L_080dc9ec
	mov r8, r1
	mov r3, r8
	strh r2, [r3]
	ldr r3, .L_080dca00
	strh r2, [r3]
	movs r3, #239
	lsls r3, r3, #7
	add r3, r11
	movs r6, #0
	movs r1, #144
	str r6, [r3]
	lsls r1, r1, #3
	ldr r0, .L_080dca04
	mov r10, r1
	bl Scheduler_AddOrUpdateCallback
	movs r1, #0
	movs r0, #0
	bl BattleEffect_WipeCanvas
	b .L_080dca08
.L_080dc9e8:
	.4byte 0x00000100
.L_080dc9ec:
	.4byte 0x00000000
.L_080dc9f0:
	.4byte gTransitionWork
.L_080dc9f4:
	.4byte gBgScroll
.L_080dc9f8:
	.4byte 0x00007828
.L_080dc9fc:
	.4byte 0x04000020
.L_080dca00:
	.4byte 0x05000002
.L_080dca04:
	.4byte BattlePresentation_ProcessPendingGraphicsTransfer
.L_080dca08:
	ldr r0, [r5]
	bl BattleFx_SelectLivingTargets
	movs r1, #185
	lsls r1, r1, #1
	movs r0, #9
	movs r2, #1
	bl BattleFx_SpawnObjects
	mov r1, r11
	movs r2, #1
	movs r3, #1
	ldr r0, .L_080dcb00
	bl Resource_LoadAndDecompress
	ldr r0, .L_080dcb04
	bl Resource_GetTableEntry
	ldr r3, .L_080dcb08
	adds r1, r0, #0
	movs r2, #128
	mov r0, r8
	bl _call_via_r3
	movs r2, #0
	ldr r1, [sp, #36]
	movs r3, #0
	ldr r0, .L_080dcb0c
	bl Resource_LoadAndDecompress
	ldr r0, .L_080dcb10
	bl Resource_GetTableEntry
	movs r3, #2
	str r0, [sp, #48]
	str r3, [sp, #0]
	movs r1, #7
	movs r2, #7
	movs r3, #3
	movs r0, #46
	bl Unnamed_080ed408
	movs r3, #3
	movs r1, #7
	movs r2, #7
	movs r0, #47
	str r3, [sp, #0]
	bl Unnamed_080ed408
	ldr r3, [r7, #8]
	str r3, [sp, #60]
	ldr r3, [r7, #12]
	mov r2, sp
	adds r2, #60
	str r2, [sp, #16]
	str r3, [r2, #4]
	ldr r2, .L_080dcb14
	movs r3, #240
	str r3, [r2, #16]
	movs r0, #1
	bl WaitFrames
	ldr r1, .L_080dcb18
	movs r0, #1
	movs r2, #0
	bl BattleBackground_LoadFar
	ldr r3, .L_080dcb1c
	ldr r2, .L_080dcb20
	add r3, r11
	str r6, [r3]
	add r2, r11
	movs r3, #4
	str r3, [r2]
	ldr r2, .L_080dcb24
	subs r3, #5
	add r2, r11
	str r3, [r2]
	ldr r3, .L_080dcb28
	add r3, r11
	str r6, [r3]
	mov r1, r10
	ldr r0, .L_080dcb2c
	bl Scheduler_AddOrUpdateCallback
	ldr r0, [sp, #56]
	movs r3, #1
	str r3, [r0, #16]
	movs r1, #1
	movs r0, #0
	bl BattleEffect_WipeCanvas
	ldr r3, .L_080dcaf0
	movs r2, #128
	lsls r2, r2, #19
	strh r3, [r2]
	ldr r3, .L_080dcaf4
	mov r1, r9
	strh r3, [r1]
	ldr r3, .L_080dcaf8
	adds r2, #82
	strh r3, [r2]
	ldr r3, .L_080dcafc
	subs r2, #2
	strh r3, [r2]
	movs r7, #225
	movs r2, #0
	ldr r3, .L_080dcb30
	lsls r7, r7, #7
	str r2, [sp, #28]
	str r2, [sp, #24]
	str r2, [sp, #44]
	mov r8, r3
	add r7, r11
	b .L_080dcb34
	.2byte 0x0000
.L_080dcaf0:
	.4byte 0x00007741
.L_080dcaf4:
	.4byte 0x00000080
.L_080dcaf8:
	.4byte 0x00001010
.L_080dcafc:
	.4byte 0x00003f44
.L_080dcb00:
	.4byte 0x0000006a
.L_080dcb04:
	.4byte 0x000000a0
.L_080dcb08:
	.4byte IwramCopyWords
.L_080dcb0c:
	.4byte 0x00000073
.L_080dcb10:
	.4byte 0x000000d2
.L_080dcb14:
	.4byte gProjection
.L_080dcb18:
	.4byte 0x0000003b
.L_080dcb1c:
	.4byte 0x00007790
.L_080dcb20:
	.4byte 0x00007794
.L_080dcb24:
	.4byte 0x00007798
.L_080dcb28:
	.4byte 0x0000779c
.L_080dcb2c:
	.4byte BattleFx_AdvanceScrollOnInterval
.L_080dcb30:
	.4byte 0x0000ffff
.L_080dcb34:
	bl Random16
	movs r1, #96
	bl __umodsi3
	adds r0, #12
	lsls r0, r0, #16
	str r0, [r7]
	bl Random16
	movs r3, #63
	ands r3, r0
	adds r3, #32
	lsls r3, r3, #16
	str r3, [r7, #4]
	movs r3, #0
	str r3, [r7, #12]
	str r3, [r7, #16]
	str r3, [r7, #24]
	ldr r0, [sp, #44]
	lsls r2, r0, #1
	adds r2, r2, r0
	mov r9, r3
	lsls r3, r2, #1
	adds r3, r3, r2
	ldr r1, .L_080dcc00
	lsls r3, r3, #7
	adds r6, r3, r1
	lsls r3, r2, #3
	subs r3, r3, r2
	ldr r2, .L_080dcc04
	lsls r3, r3, #5
	adds r5, r3, r2
.L_080dcb76:
	bl Random16
	movs r3, #15
	ands r3, r0
	adds r3, #48
	str r3, [r5]
	bl Render_ResetTransformState
	bl Random16
	mov r3, r8
	ands r0, r3
	bl SceneTransform_ApplyRoll
	bl Random16
	mov r1, r8
	ands r0, r1
	bl SceneTransform_ApplyPitch
	bl Random16
	mov r2, r8
	ands r0, r2
	bl SceneTransform_ApplyYaw
	adds r0, r6, #0
	bl Graphics_SaveTransferWork
	movs r3, #1
	add r9, r3
	mov r0, r9
	adds r5, #28
	adds r6, #48
	cmp r0, #24
	bne .L_080dcb76
	ldr r1, [sp, #44]
	adds r1, #1
	adds r7, #28
	str r1, [sp, #44]
	cmp r1, #16
	bne .L_080dcb34
	movs r2, #239
	lsls r2, r2, #7
	add r2, r11
	movs r3, #2
	str r3, [r2]
	ldr r2, .L_080dcc08
	movs r3, #50
	add r2, r11
	str r3, [r2]
	ldr r2, .L_080dcc0c
	ldr r3, .L_080dcbfc
	strh r3, [r2]
	movs r2, #0
	str r2, [sp, #40]
	ldr r3, .L_080dcc10
	ldr r3, [r3]
	movs r2, #3
	ands r3, r2
	cmp r3, #0
	beq .L_080dcbf4
	b .L_080dcfba
.L_080dcbf4:
	mov r3, sp
	adds r3, #108
	str r3, [sp, #12]
	b .L_080dcc14
.L_080dcbfc:
	.4byte 0x00000784
.L_080dcc00:
	.4byte gMapCellBuffer + 0x3800
.L_080dcc04:
	.4byte gMapCellBuffer
.L_080dcc08:
	.4byte 0x00007784
.L_080dcc0c:
	.4byte 0x0400000c
.L_080dcc10:
	.4byte gKeysRepeat
.L_080dcc14:
	ldr r0, [sp, #40]
	cmp r0, #209
	bgt .L_080dccc2
	cmp r0, #0
	bne .L_080dcc3e
	ldr r1, [sp, #48]
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
	adds r1, #4
	str r3, [sp, #24]
	str r1, [sp, #48]
	b .L_080dcc58
.L_080dcc3e:
	ldr r2, [sp, #48]
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
	str r2, [sp, #48]
.L_080dcc58:
	add r2, sp, #80
	movs r3, #0
	str r3, [r2, #12]
	movs r3, #255
	lsls r3, r3, #16
	str r3, [r2, #4]
	ldr r0, [sp, #24]
	movs r3, #0
	str r3, [sp, #20]
	mov r9, r3
	movs r3, #128
	mov r10, r2
	lsls r3, r3, #15
	lsls r2, r0, #16
	subs r7, r3, r2
.L_080dcc76:
	ldr r1, [sp, #28]
	movs r2, #160
	lsls r3, r1, #16
	lsls r2, r2, #15
	mov r0, r9
	adds r5, r3, r2
	ldr r1, .L_080dcec8
	lsls r3, r0, #2
	add r3, r11
	movs r4, #0
	mov r8, r7
	adds r6, r3, r1
.L_080dcc8e:
	mov r2, r10
	mov r3, r8
	str r5, [r2]
	str r3, [r2, #8]
	ldmia r6!, {r0}
	mov r1, r10
	ldr r2, .L_080dcecc
	movs r3, #0
	str r4, [sp, #8]
	bl Object_ApplyProjectedPlacementFar
	ldr r4, [sp, #8]
	movs r0, #128
	lsls r0, r0, #14
	adds r4, #1
	adds r5, r5, r0
	cmp r4, #3
	bne .L_080dcc8e
	ldr r2, [sp, #20]
	movs r1, #3
	adds r2, #1
	add r9, r1
	adds r7, r7, r0
	str r2, [sp, #20]
	cmp r2, #3
	bne .L_080dcc76
.L_080dccc2:
	ldr r3, [sp, #12]
	movs r1, #0
	str r1, [r3, #4]
	str r1, [r3, #8]
	ldr r0, [sp, #40]
	cmp r0, #48
	bne .L_080dccde
	ldr r3, .L_080dced0
	movs r2, #24
	add r3, r11
	str r2, [r3]
	ldr r3, .L_080dced4
	add r3, r11
	str r1, [r3]
.L_080dccde:
	movs r1, #0
	str r1, [sp, #44]
.L_080dcce2:
	ldr r2, [sp, #44]
	lsls r6, r2, #3
	adds r7, r6, #0
	ldr r3, [sp, #40]
	adds r7, #64
	cmp r3, r7
	bge .L_080dccf2
	b .L_080dcf88
.L_080dccf2:
	subs r3, r6, r2
	lsls r3, r3, #2
	movs r0, #225
	add r3, r11
	lsls r0, r0, #7
	adds r5, r3, r0
	movs r2, #2
	ldrsh r1, [r5, r2]
	movs r0, #6
	ldrsh r3, [r5, r0]
	mov r8, r1
	mov r10, r3
	ldr r1, [sp, #40]
	adds r3, r6, #0
	adds r3, #84
	cmp r1, r3
	bne .L_080dcd1a
	movs r0, #212
	bl AudioCommand_PlayFar
.L_080dcd1a:
	adds r3, r6, #0
	ldr r2, [sp, #40]
	adds r3, #85
	cmp r2, r3
	blt .L_080dcd8e
	ldr r1, [r5, #12]
	ldr r3, [r5]
	adds r3, r3, r1
	ldr r2, [r5, #16]
	str r3, [r5]
	ldr r3, [r5, #4]
	adds r3, r3, r2
	str r3, [r5, #4]
	ldr r3, .L_080dced8
	movs r0, #128
	adds r1, r1, r3
	lsls r0, r0, #10
	str r1, [r5, #12]
	adds r2, r2, r0
	ldr r1, .L_080dcedc
	str r2, [r5, #16]
	movs r0, #16
	movs r5, #21
	mov r2, r8
	mov r3, r10
	str r0, [sp, #0]
	ldr r4, [sp, #60]
	add r1, r11
	adds r2, #4
	subs r3, #40
	str r5, [sp, #4]
	ldr r0, [sp, #52]
	bl _call_via_r4
	movs r0, #29
	ldr r1, .L_080dcee0
	str r0, [sp, #0]
	mov r2, r8
	movs r0, #35
	mov r3, r10
	add r1, r11
	subs r2, #16
	subs r3, #19
	str r0, [sp, #4]
	ldr r4, [sp, #60]
	ldr r0, [sp, #52]
	bl _call_via_r4
	ldr r1, .L_080dcee4
	movs r0, #24
	mov r2, r8
	mov r3, r10
	str r0, [sp, #4]
	add r1, r11
	subs r2, #20
	adds r3, #16
	str r5, [sp, #0]
	b .L_080dcebc
.L_080dcd8e:
	adds r3, r6, #0
	ldr r1, [sp, #40]
	adds r3, #80
	cmp r1, r3
	bge .L_080dcd9a
	b .L_080dcf08
.L_080dcd9a:
	subs r3, r1, r7
	subs r3, #16
	cmp r3, #4
	bls .L_080dcda4
	b .L_080dcf88
.L_080dcda4:
	ldr r2, .L_080dcee8
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
.L_080dcdac:
	.4byte .L_080dcdc0
	.4byte .L_080dcddc
	.4byte .L_080dcdf4
	.4byte .L_080dce26
	.4byte .L_080dce74
.L_080dcdc0:
	movs r1, #14
	mov r2, r8
	mov r3, r10
	str r1, [sp, #0]
	movs r1, #28
	str r1, [sp, #4]
	subs r2, #7
	subs r3, #14
	ldr r4, [sp, #60]
	ldr r0, [sp, #52]
	mov r1, r11
	bl _call_via_r4
	b .L_080dcf88
.L_080dcddc:
	movs r0, #23
	movs r1, #196
	lsls r1, r1, #1
	mov r2, r8
	mov r3, r10
	str r0, [sp, #0]
	movs r0, #44
	str r0, [sp, #4]
	add r1, r11
	subs r2, #11
	subs r3, #22
	b .L_080dcebc
.L_080dcdf4:
	movs r0, #20
	ldr r1, .L_080dceec
	str r0, [sp, #0]
	mov r2, r8
	movs r0, #30
	mov r3, r10
	add r1, r11
	subs r2, #4
	subs r3, #31
	str r0, [sp, #4]
	ldr r4, [sp, #60]
	ldr r0, [sp, #52]
	bl _call_via_r4
	movs r0, #22
	ldr r1, .L_080dcef0
	mov r2, r8
	mov r3, r10
	str r0, [sp, #0]
	movs r0, #33
	str r0, [sp, #4]
	add r1, r11
	subs r2, #16
	subs r3, #1
	b .L_080dcebc
.L_080dce26:
	movs r0, #18
	ldr r1, .L_080dcef4
	str r0, [sp, #0]
	mov r2, r8
	movs r0, #27
	mov r3, r10
	str r0, [sp, #4]
	ldr r4, [sp, #60]
	add r1, r11
	adds r2, #1
	subs r3, #38
	ldr r0, [sp, #52]
	bl _call_via_r4
	movs r1, #201
	movs r0, #22
	lsls r1, r1, #4
	mov r2, r8
	mov r3, r10
	add r1, r11
	subs r2, #11
	subs r3, #11
	str r0, [sp, #0]
	str r0, [sp, #4]
	ldr r4, [sp, #60]
	ldr r0, [sp, #52]
	bl _call_via_r4
	movs r0, #19
	ldr r1, .L_080dcef8
	mov r2, r8
	mov r3, r10
	str r0, [sp, #0]
	movs r0, #28
	str r0, [sp, #4]
	add r1, r11
	subs r2, #19
	adds r3, #11
	b .L_080dcebc
.L_080dce74:
	ldr r1, .L_080dcefc
	movs r5, #23
	movs r0, #16
	mov r2, r8
	mov r3, r10
	str r0, [sp, #0]
	str r5, [sp, #4]
	ldr r4, [sp, #60]
	add r1, r11
	adds r2, #4
	subs r3, #40
	ldr r0, [sp, #52]
	bl _call_via_r4
	ldr r1, .L_080dcf00
	str r5, [sp, #0]
	mov r2, r8
	movs r5, #28
	mov r3, r10
	add r1, r11
	subs r2, #10
	subs r3, #17
	ldr r4, [sp, #60]
	str r5, [sp, #4]
	ldr r0, [sp, #52]
	bl _call_via_r4
	ldr r1, .L_080dcf04
	movs r0, #20
	mov r2, r8
	mov r3, r10
	str r0, [sp, #0]
	add r1, r11
	subs r2, #20
	adds r3, #11
	str r5, [sp, #4]
.L_080dcebc:
	ldr r4, [sp, #60]
	ldr r0, [sp, #52]
	bl _call_via_r4
	b .L_080dcf88
	.2byte 0x0000
.L_080dcec8:
	.4byte 0x000077d8
.L_080dcecc:
	.4byte Data_080eeafa + 0x46
.L_080dced0:
	.4byte 0x000077b4
.L_080dced4:
	.4byte 0x000077b8
.L_080dced8:
	.4byte 0xffff0000
.L_080dcedc:
	.4byte 0x000016ac
.L_080dcee0:
	.4byte 0x000017fc
.L_080dcee4:
	.4byte 0x00001bf3
.L_080dcee8:
	.4byte .L_080dcdac
.L_080dceec:
	.4byte 0x0000057c
.L_080dcef0:
	.4byte 0x000007d4
.L_080dcef4:
	.4byte 0x00000aaa
.L_080dcef8:
	.4byte 0x00000e74
.L_080dcefc:
	.4byte 0x00001088
.L_080dcf00:
	.4byte 0x000011f8
.L_080dcf04:
	.4byte 0x0000147c
.L_080dcf08:
	ldr r3, [sp, #44]
	movs r2, #0
	mov r9, r2
	lsls r2, r3, #1
	adds r3, r2, r3
	lsls r2, r3, #1
	adds r2, r2, r3
	ldr r0, .L_080dd02c
	lsls r2, r2, #7
	adds r7, r2, r0
	lsls r2, r3, #3
	subs r2, r2, r3
	ldr r1, .L_080dd030
	lsls r2, r2, #5
	add r6, sp, #96
	adds r5, r2, r1
.L_080dcf28:
	ldr r3, [r5]
	cmp r3, #0
	ble .L_080dcf7a
	adds r0, r7, #0
	bl Graphics_LoadTransferWork
	ldr r3, [r5]
	ldr r2, [sp, #12]
	str r3, [r2]
	adds r1, r6, #0
	ldr r0, [sp, #12]
	bl EffectPosition_ApplyBaseAndYOffset
	ldr r3, [r6]
	asrs r3, r3, #1
	add r3, r8
	str r3, [r6]
	ldr r3, [r6, #4]
	add r3, r10
	adds r3, #16
	str r3, [r6, #4]
	ldr r3, [r5]
	subs r3, #4
	str r3, [r5]
	ldr r3, .L_080dd034
	ldrh r1, [r3, #8]
	ldr r3, [sp, #36]
	movs r0, #5
	ldr r2, [r6]
	adds r1, r3, r1
	ldr r3, [r6, #4]
	str r0, [sp, #0]
	movs r0, #10
	str r0, [sp, #4]
	ldr r0, [sp, #16]
	subs r2, #2
	ldr r4, [r0, #4]
	subs r3, #5
	ldr r0, [sp, #52]
	bl _call_via_r4
.L_080dcf7a:
	movs r1, #1
	add r9, r1
	mov r2, r9
	adds r7, #48
	adds r5, #28
	cmp r2, #24
	bne .L_080dcf28
.L_080dcf88:
	ldr r3, [sp, #44]
	adds r3, #1
	str r3, [sp, #44]
	cmp r3, #16
	beq .L_080dcf94
	b .L_080dcce2
.L_080dcf94:
	ldr r2, .L_080dd038
	movs r3, #1
	add r2, r11
	movs r0, #1
	str r3, [r2]
	bl WaitFrames
	ldr r0, [sp, #40]
	adds r0, #1
	str r0, [sp, #40]
	cmp r0, #220
	beq .L_080dcfba
	ldr r3, .L_080dd03c
	ldr r3, [r3]
	movs r2, #3
	ands r3, r2
	cmp r3, #0
	bne .L_080dcfba
	b .L_080dcc14
.L_080dcfba:
	ldr r0, .L_080dd040
	bl Scheduler_RemoveCallback
	ldr r1, [sp, #56]
	movs r3, #0
	add r2, sp, #32
	str r3, [r1, #16]
	ldrh r2, [r2]
	ldr r3, .L_080dd044
	strh r2, [r3, #4]
	bl BattleEffect_SetupBlendedDisplay
	ldr r5, .L_080dd048
	movs r3, #0
	str r3, [sp, #44]
	add r5, r11
.L_080dcfda:
	ldmia r5!, {r0}
	bl ResourceObject_ReleaseFar
	ldr r0, [sp, #44]
	adds r0, #1
	str r0, [sp, #44]
	cmp r0, #9
	bne .L_080dcfda
	ldr r2, .L_080dd04c
	ldr r3, .L_080dd024
	strh r3, [r2]
	ldr r3, .L_080dd028
	subs r2, #32
	strh r3, [r2]
	ldr r1, .L_080dd030
	ldr r0, .L_080dd050
	movs r2, #1
	movs r3, #0
	bl Resource_LoadAndDecompress
	movs r5, #225
	movs r1, #0
	lsls r5, r5, #7
	str r1, [sp, #44]
	add r6, sp, #68
	add r5, r11
	movs r7, #31
.L_080dd010:
	ldr r0, [sp, #44]
	movs r1, #6
	bl __modsi3
	ldr r3, .L_080dd054
	add r3, r11
	ldr r2, [r3]
	ldr r3, [r2, #20]
	b .L_080dd058
	.2byte 0x0000
.L_080dd024:
	.4byte 0x00000080
.L_080dd028:
	.4byte 0x00007741
.L_080dd02c:
	.4byte gMapCellBuffer + 0x3800
.L_080dd030:
	.4byte gMapCellBuffer
.L_080dd034:
	.4byte ParticleStreams_CellOffsets
.L_080dd038:
	.4byte 0x00007824
.L_080dd03c:
	.4byte gKeysRepeat
.L_080dd040:
	.4byte BattleFx_AdvanceScrollOnInterval
.L_080dd044:
	.4byte gBgScroll
.L_080dd048:
	.4byte 0x000077d8
.L_080dd04c:
	.4byte 0x04000020
.L_080dd050:
	.4byte 0x000000b4
.L_080dd054:
	.4byte 0x00007828
.L_080dd058:
	cmp r0, r3
	bge .L_080dd08c
	lsls r3, r0, #1
	adds r3, #36
	ldrsh r0, [r2, r3]
	adds r1, r6, #0
	bl EffectPosition_ApplyStepAndYOffset
	bl Random16
	ands r0, r7
	adds r0, #40
	negs r0, r0
	str r0, [r5, #4]
	ldr r1, [r6]
	lsrs r3, r1, #31
	adds r1, r1, r3
	movs r3, #80
	subs r3, r3, r0
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r1, r1, #1
	asrs r3, r3, #1
	adds r1, r1, r3
	str r1, [r5]
	b .L_080dd0a4
.L_080dd08c:
	bl Random16
	movs r3, #63
	ands r3, r0
	adds r3, #80
	str r3, [r5]
	bl Random16
	ands r0, r7
	adds r0, #40
	negs r0, r0
	str r0, [r5, #4]
.L_080dd0a4:
	movs r3, #1
	negs r3, r3
	str r3, [r5, #24]
	ldr r2, [sp, #44]
	adds r2, #1
	adds r5, #28
	str r2, [sp, #44]
	cmp r2, #32
	bne .L_080dd010
	movs r3, #0
	str r3, [sp, #40]
.L_080dd0ba:
	movs r0, #0
	movs r7, #225
	lsls r7, r7, #7
	str r0, [sp, #44]
	add r7, r11
.L_080dd0c4:
	ldr r1, [sp, #44]
	ldr r2, [sp, #40]
	lsls r3, r1, #1
	cmp r2, r3
	bge .L_080dd0d4
	cmp r2, #40
	bgt .L_080dd0d4
	b .L_080dd220
.L_080dd0d4:
	ldr r3, [r7, #24]
	cmp r3, #0
	blt .L_080dd168
	cmp r3, #23
	bgt .L_080dd162
	adds r6, r3, #0
	cmp r3, #0
	bge .L_080dd0e6
	adds r6, r3, #3
.L_080dd0e6:
	ldr r3, [sp, #44]
	asrs r6, r6, #2
	ldr r2, .L_080dd27c
	movs r4, #1
	ands r4, r3
	lsls r3, r6, #1
	ldrh r1, [r2, r3]
	ldr r3, .L_080dd280
	ldrb r5, [r3, r6]
	ldr r2, [r7]
	lsrs r3, r5, #1
	ldr r0, .L_080dd284
	subs r2, r2, r3
	ldr r3, .L_080dd288
	adds r1, r1, r0
	ldrb r0, [r3, r6]
	ldr r3, [r7, #4]
	str r5, [sp, #0]
	adds r3, r3, r0
	ldr r0, .L_080dd28c
	ldrb r0, [r0, r6]
	str r0, [sp, #4]
	ldr r0, [sp, #16]
	lsls r4, r4, #2
	subs r3, #40
	ldr r4, [r4, r0]
	subs r2, #8
	ldr r0, [sp, #52]
	bl _call_via_r4
	ldr r3, [r7, #24]
	cmp r3, #11
	bgt .L_080dd162
	movs r1, #16
	ldr r2, [r7]
	ldr r3, [r7, #4]
	str r1, [sp, #0]
	movs r1, #21
	str r1, [sp, #4]
	ldr r1, .L_080dd290
	ldr r4, [sp, #60]
	adds r2, #4
	subs r3, #40
	ldr r0, [sp, #52]
	add r1, r11
	bl _call_via_r4
	movs r1, #29
	ldr r2, [r7]
	ldr r3, [r7, #4]
	str r1, [sp, #0]
	movs r1, #35
	str r1, [sp, #4]
	ldr r1, .L_080dd294
	subs r3, #19
	subs r2, #16
	ldr r4, [sp, #60]
	ldr r0, [sp, #52]
	add r1, r11
	bl _call_via_r4
	ldr r3, [r7, #24]
.L_080dd162:
	adds r3, #1
	str r3, [r7, #24]
	b .L_080dd220
.L_080dd168:
	ldr r1, [r7, #4]
	movs r5, #24
	cmp r1, #56
	ble .L_080dd176
	subs r3, r5, r1
	adds r5, r3, #0
	adds r5, #56
.L_080dd176:
	adds r3, r1, #0
	movs r1, #16
	ldr r2, [r7]
	str r1, [sp, #0]
	ldr r1, .L_080dd290
	movs r6, #21
	adds r2, #4
	subs r3, #40
	ldr r4, [sp, #60]
	add r1, r11
	str r6, [sp, #4]
	ldr r0, [sp, #52]
	bl _call_via_r4
	movs r1, #29
	ldr r2, [r7]
	ldr r3, [r7, #4]
	str r1, [sp, #0]
	movs r1, #35
	str r1, [sp, #4]
	ldr r1, .L_080dd294
	subs r2, #16
	subs r3, #19
	ldr r4, [sp, #60]
	ldr r0, [sp, #52]
	add r1, r11
	bl _call_via_r4
	cmp r5, #0
	ble .L_080dd1ca
	ldr r2, [r7]
	ldr r3, [r7, #4]
	ldr r1, .L_080dd298
	subs r2, #20
	adds r3, #16
	str r6, [sp, #0]
	str r5, [sp, #4]
	ldr r4, [sp, #60]
	ldr r0, [sp, #52]
	add r1, r11
	bl _call_via_r4
.L_080dd1ca:
	ldr r3, [r7]
	subs r3, #6
	str r3, [r7]
	ldr r3, [r7, #4]
	adds r3, #12
	str r3, [r7, #4]
	cmp r3, #79
	ble .L_080dd220
	ldr r2, .L_080dd29c
	movs r3, #0
	add r2, r11
	str r3, [r7, #24]
	movs r3, #2
	str r3, [r2]
	movs r0, #134
	bl AudioCommand_PlayFar
	movs r1, #6
	ldr r0, [sp, #44]
	bl __modsi3
	ldr r6, .L_080dd2a0
	add r6, r11
	ldr r2, [r6]
	ldr r3, [r2, #20]
	adds r4, r0, #0
	cmp r4, r3
	bge .L_080dd220
	lsls r5, r4, #1
	adds r5, #36
	movs r3, #8
	ldrsh r0, [r2, r5]
	str r3, [sp, #0]
	movs r1, #7
	movs r2, #5
	adds r3, r4, #0
	bl ObjectGroup_UpdateMembers
	ldr r3, [r6]
	movs r1, #1
	ldrsh r0, [r3, r5]
	bl BattleMotion_ApplyVariantMotionFar
.L_080dd220:
	ldr r3, [sp, #44]
	adds r3, #1
	adds r7, #28
	str r3, [sp, #44]
	cmp r3, #24
	beq .L_080dd22e
	b .L_080dd0c4
.L_080dd22e:
	movs r0, #4
	movs r1, #8
	bl Camera_ApplyShake
	bl ObjectGroup_TickMemberTimers
	ldr r2, .L_080dd2a4
	movs r3, #1
	add r2, r11
	movs r0, #1
	str r3, [r2]
	bl WaitFrames
	ldr r0, [sp, #40]
	adds r0, #1
	str r0, [sp, #40]
	cmp r0, #88
	beq .L_080dd254
	b .L_080dd0ba
.L_080dd254:
	ldr r0, .L_080dd2a8
	bl Scheduler_RemoveCallback
	movs r0, #47
	bl Runtime_ReleaseHeapBlock
	movs r0, #46
	bl Runtime_ReleaseHeapBlock
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
.L_080dd27c:
	.4byte PuffArc_CellSourceOffsets
.L_080dd280:
	.4byte PuffArc_CellWidths
.L_080dd284:
	.4byte gMapCellBuffer
.L_080dd288:
	.4byte PuffArc_CellBiasY
.L_080dd28c:
	.4byte PuffArc_CellHeights
.L_080dd290:
	.4byte 0x000016ac
.L_080dd294:
	.4byte 0x000017fc
.L_080dd298:
	.4byte 0x00001bf3
.L_080dd29c:
	.4byte 0x000077a8
.L_080dd2a0:
	.4byte 0x00007828
.L_080dd2a4:
	.4byte 0x00007824
.L_080dd2a8:
	.4byte BattlePresentation_ProcessPendingGraphicsTransfer
