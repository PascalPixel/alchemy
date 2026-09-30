.syntax unified
	.thumb
	.global BattleFx_RunCastingImpact
	.thumb_func
BattleFx_RunCastingImpact:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #184
	ldr r3, .L_080e4804
	str r1, [sp, #96]
	str r0, [sp, #100]
	adds r2, r3, #0
	ldmia r2!, {r0}
	str r0, [sp, #92]
	ldr r2, [r2]
	str r2, [sp, #88]
	adds r2, r3, #0
	subs r2, #108
	ldr r2, [r2]
	str r2, [sp, #80]
	ldr r1, .L_080e4808
	ldr r3, [r3, #8]
	ldr r2, [sp, #100]
	str r3, [sp, #76]
	adds r3, r0, r1
	str r2, [r3]
	ldr r3, [sp, #96]
	cmp r3, #11
	beq .L_080e47fa
	cmp r3, #8
	beq .L_080e47fa
	cmp r3, #32
	bne .L_080e480c
.L_080e47fa:
	movs r0, #0
	bl BattleFx_BeginTiledCanvas
	b .L_080e4812
	.2byte 0x0000
.L_080e4804:
	.4byte gBattleFxWork
.L_080e4808:
	.4byte 0x00007828
.L_080e480c:
	movs r0, #0
	bl BattleFx_BeginCanvasLayer
.L_080e4812:
	ldr r2, .L_080e4858
	ldr r3, .L_080e4854
	strh r3, [r2]
	ldr r1, [sp, #76]
	ldr r0, .L_080e485c
	movs r2, #0
	movs r3, #0
	bl Resource_LoadAndDecompress
	ldr r0, .L_080e4860
	ldr r1, [sp, #92]
	movs r2, #1
	movs r3, #0
	bl Resource_LoadAndDecompress
	ldr r0, .L_080e4864
	ldr r1, .L_080e4868
	movs r2, #1
	movs r3, #0
	bl Resource_LoadAndDecompress
	movs r0, #162
	ldr r5, [sp, #92]
	lsls r0, r0, #7
	movs r3, #144
	adds r1, r5, r0
	lsls r3, r3, #1
	ldr r0, .L_080e4868
	movs r2, #40
	bl Graphics_PackTileRows
	b .L_080e486c
	.2byte 0x0000
.L_080e4854:
	.4byte 0x00001010
.L_080e4858:
	.4byte 0x04000052
.L_080e485c:
	.4byte 0x00000073
.L_080e4860:
	.4byte 0x00000096
.L_080e4864:
	.4byte 0x00000099
.L_080e4868:
	.4byte gMapCellBuffer
.L_080e486c:
	ldr r1, [sp, #96]
	cmp r1, #5
	beq .L_080e4876
	cmp r1, #23
	bne .L_080e487a
.L_080e4876:
	ldr r0, .L_080e4afc
	b .L_080e48f0
.L_080e487a:
	ldr r2, [sp, #96]
	cmp r2, #12
	bne .L_080e4884
	ldr r0, .L_080e4b00
	b .L_080e48f0
.L_080e4884:
	ldr r3, [sp, #96]
	cmp r3, #6
	beq .L_080e488e
	cmp r3, #27
	bne .L_080e48a0
.L_080e488e:
	ldr r0, .L_080e4b04
	ldr r1, .L_080e4b08
	movs r2, #1
	movs r3, #0
	bl Resource_LoadAndDecompress
	ldr r0, .L_080e4b0c
	ldr r1, .L_080e4b10
	b .L_080e48f2
.L_080e48a0:
	ldr r5, [sp, #96]
	cmp r5, #31
	bne .L_080e48aa
	ldr r0, .L_080e4b14
	b .L_080e48b2
.L_080e48aa:
	ldr r0, [sp, #96]
	cmp r0, #8
	bne .L_080e48be
	ldr r0, .L_080e4b18
.L_080e48b2:
	ldr r1, .L_080e4b08
	movs r2, #1
	movs r3, #1
	bl Resource_LoadAndDecompress
	b .L_080e4912
.L_080e48be:
	ldr r1, [sp, #96]
	cmp r1, #14
	bne .L_080e48c8
	ldr r0, .L_080e4b1c
	b .L_080e48f0
.L_080e48c8:
	ldr r2, [sp, #96]
	cmp r2, #30
	bne .L_080e48d2
	ldr r0, .L_080e4b04
	b .L_080e48f0
.L_080e48d2:
	ldr r3, [sp, #96]
	cmp r3, #16
	bne .L_080e48dc
	ldr r0, .L_080e4b20
	b .L_080e48f0
.L_080e48dc:
	ldr r5, [sp, #96]
	cmp r5, #20
	bne .L_080e48e6
	ldr r0, .L_080e4b24
	b .L_080e48f0
.L_080e48e6:
	ldr r3, [sp, #96]
	subs r3, #33
	cmp r3, #1
	bhi .L_080e48fc
	ldr r0, .L_080e4b28
.L_080e48f0:
	ldr r1, .L_080e4b08
.L_080e48f2:
	movs r2, #1
	movs r3, #0
	bl Resource_LoadAndDecompress
	b .L_080e4912
.L_080e48fc:
	ldr r0, [sp, #96]
	cmp r0, #11
	beq .L_080e4912
	cmp r0, #32
	beq .L_080e4912
	ldr r0, .L_080e4b2c
	ldr r1, .L_080e4b08
	movs r2, #1
	movs r3, #0
	bl Resource_LoadAndDecompress
.L_080e4912:
	ldr r1, [sp, #96]
	cmp r1, #100
	bls .L_080e491a
	b .L_080e4ade
.L_080e491a:
	ldr r2, .L_080e4b30
	lsls r3, r1, #2
	ldr r3, [r3, r2]
	mov pc, r3
	.2byte 0x0000
.L_080e4924:
	.4byte .L_080e4ab8
	.4byte .L_080e4ac4
	.4byte .L_080e4abc
	.4byte .L_080e4ac0
	.4byte .L_080e4ab8
	.4byte .L_080e4ac0
	.4byte .L_080e4ac4
	.4byte .L_080e4ab8
	.4byte .L_080e4ab8
	.4byte .L_080e4ab8
	.4byte .L_080e4ab8
	.4byte .L_080e4ab8
	.4byte .L_080e4ab8
	.4byte .L_080e4ab8
	.4byte .L_080e4abc
	.4byte .L_080e4abc
	.4byte .L_080e4abc
	.4byte .L_080e4abc
	.4byte .L_080e4abc
	.4byte .L_080e4abc
	.4byte .L_080e4ac0
	.4byte .L_080e4ac0
	.4byte .L_080e4ac0
	.4byte .L_080e4ac0
	.4byte .L_080e4ac0
	.4byte .L_080e4ac0
	.4byte .L_080e4ac4
	.4byte .L_080e4ac4
	.4byte .L_080e4ac4
	.4byte .L_080e4ac4
	.4byte .L_080e4ac4
	.4byte .L_080e4ac4
	.4byte .L_080e4ac4
	.4byte .L_080e4ab8
	.4byte .L_080e4ac0
	.4byte .L_080e4ac0
	.4byte .L_080e4ade
	.4byte .L_080e4ade
	.4byte .L_080e4ade
	.4byte .L_080e4ade
	.4byte .L_080e4ade
	.4byte .L_080e4ade
	.4byte .L_080e4ade
	.4byte .L_080e4ade
	.4byte .L_080e4ade
	.4byte .L_080e4ade
	.4byte .L_080e4ade
	.4byte .L_080e4ade
	.4byte .L_080e4ade
	.4byte .L_080e4ade
	.4byte .L_080e4ade
	.4byte .L_080e4ade
	.4byte .L_080e4ade
	.4byte .L_080e4ade
	.4byte .L_080e4ade
	.4byte .L_080e4ade
	.4byte .L_080e4ade
	.4byte .L_080e4ade
	.4byte .L_080e4ade
	.4byte .L_080e4ade
	.4byte .L_080e4ade
	.4byte .L_080e4ade
	.4byte .L_080e4ade
	.4byte .L_080e4ade
	.4byte .L_080e4ade
	.4byte .L_080e4ade
	.4byte .L_080e4ade
	.4byte .L_080e4ade
	.4byte .L_080e4ade
	.4byte .L_080e4ade
	.4byte .L_080e4ade
	.4byte .L_080e4ade
	.4byte .L_080e4ade
	.4byte .L_080e4ade
	.4byte .L_080e4ade
	.4byte .L_080e4ade
	.4byte .L_080e4ade
	.4byte .L_080e4ade
	.4byte .L_080e4ade
	.4byte .L_080e4ade
	.4byte .L_080e4ade
	.4byte .L_080e4ade
	.4byte .L_080e4ade
	.4byte .L_080e4ade
	.4byte .L_080e4ade
	.4byte .L_080e4ade
	.4byte .L_080e4ade
	.4byte .L_080e4ade
	.4byte .L_080e4ade
	.4byte .L_080e4ade
	.4byte .L_080e4ade
	.4byte .L_080e4ade
	.4byte .L_080e4ade
	.4byte .L_080e4ade
	.4byte .L_080e4ade
	.4byte .L_080e4ade
	.4byte .L_080e4ade
	.4byte .L_080e4ade
	.4byte .L_080e4ade
	.4byte .L_080e4ade
	.4byte .L_080e4ad2
.L_080e4ab8:
	ldr r0, .L_080e4b34
	b .L_080e4ac6
.L_080e4abc:
	ldr r0, .L_080e4b38
	b .L_080e4ac6
.L_080e4ac0:
	ldr r0, .L_080e4b3c
	b .L_080e4ac6
.L_080e4ac4:
	ldr r0, .L_080e4b40
.L_080e4ac6:
	ldr r1, .L_080e4b44
	movs r2, #1
	movs r3, #1
	bl Resource_LoadAndDecompress
	b .L_080e4ade
.L_080e4ad2:
	ldr r0, .L_080e4b38
	ldr r1, .L_080e4b44
	movs r2, #1
	movs r3, #1
	bl Resource_LoadAndDecompress
.L_080e4ade:
	ldr r3, [sp, #92]
	movs r5, #239
	lsls r5, r5, #7
	adds r2, r3, r5
	movs r3, #2
	str r3, [r2]
	ldr r0, [sp, #96]
	cmp r0, #12
	bne .L_080e4b4c
	ldr r1, [sp, #92]
	ldr r3, .L_080e4b48
	adds r2, r1, r3
	movs r3, #75
	b .L_080e4b54
	.2byte 0x0000
.L_080e4afc:
	.4byte 0x0000007d
.L_080e4b00:
	.4byte 0x000000a9
.L_080e4b04:
	.4byte 0x000000ce
.L_080e4b08:
	.4byte gMapCellBuffer
.L_080e4b0c:
	.4byte 0x000000c4
.L_080e4b10:
	.4byte Data_02010c56
.L_080e4b14:
	.4byte 0x00000079
.L_080e4b18:
	.4byte 0x000000c3
.L_080e4b1c:
	.4byte 0x0000006f
.L_080e4b20:
	.4byte 0x000000b8
.L_080e4b24:
	.4byte 0x000000b4
.L_080e4b28:
	.4byte 0x00000053
.L_080e4b2c:
	.4byte 0x0000009e
.L_080e4b30:
	.4byte .L_080e4924
.L_080e4b34:
	.4byte 0x00000094
.L_080e4b38:
	.4byte 0x00000092
.L_080e4b3c:
	.4byte 0x0000008e
.L_080e4b40:
	.4byte 0x00000090
.L_080e4b44:
	.4byte Data_02013c56
.L_080e4b48:
	.4byte 0x00007784
.L_080e4b4c:
	ldr r5, [sp, #92]
	ldr r0, .L_080e4de0
	movs r3, #50
	adds r2, r5, r0
.L_080e4b54:
	str r3, [r2]
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_080e4de4
	bl Scheduler_AddOrUpdateCallback
	ldr r2, .L_080e4de8
	ldr r1, [sp, #92]
	adds r5, r1, r2
	ldr r3, [r5]
	mov r2, sp
	adds r2, #160
	movs r1, #36
	ldrsh r0, [r3, r1]
	adds r1, r2, #0
	str r2, [sp, #60]
	bl EffectPosition_ApplyStepAndYOffset
	ldr r3, [r5]
	ldr r0, [r3, #8]
	mov r3, sp
	adds r3, #172
	adds r1, r3, #0
	str r3, [sp, #56]
	bl EffectPosition_ApplyStepAndYOffset
	ldr r3, [r5]
	mov r1, sp
	ldr r0, [r3, #4]
	adds r1, #104
	str r1, [sp, #52]
	bl BattleFx_FetchRectangleBlitters
	ldr r0, .L_080e4dec
	ldr r3, [sp, #92]
	adds r2, r3, r0
	movs r3, #24
	str r3, [r2]
	ldr r1, [sp, #92]
	ldr r3, .L_080e4df0
	adds r2, r1, r3
	movs r3, #0
	str r3, [r2]
	ldr r3, [r5]
	ldr r0, [r3, #8]
	bl GetBattleObjectSlotFar
	movs r1, #225
	ldr r7, [r0]
	ldr r0, [sp, #92]
	movs r5, #0
	lsls r1, r1, #7
	ldr r6, .L_080e4df4
	mov r10, r5
	mov r8, r5
	adds r5, r0, r1
.L_080e4bc4:
	bl Random16
	movs r3, #63
	ands r3, r0
	adds r3, #32
	mov r2, r8
	str r3, [r5]
	str r2, [r5, #4]
	str r2, [r5, #8]
	bl Random16
	ands r0, r6
	str r0, [r5, #12]
	bl Random16
	ands r0, r6
	str r0, [r5, #16]
	bl Random16
	movs r3, #1
	ands r0, r6
	add r10, r3
	str r0, [r5, #20]
	mov r0, r10
	adds r5, #28
	cmp r0, #64
	bne .L_080e4bc4
	adds r0, r7, #0
	movs r1, #0
	bl ObjectDispatch_ApplyValueToChildrenFar
	mov r1, sp
	adds r1, #148
	str r1, [sp, #8]
	ldr r3, [r7, #8]
	str r3, [r1]
	ldr r3, [r7, #12]
	movs r2, #160
	lsls r2, r2, #15
	adds r3, r3, r2
	str r3, [r1, #4]
	ldr r3, [r7, #16]
	str r3, [r1, #8]
	ldr r3, [r7, #36]
	str r3, [sp, #48]
	ldr r5, [r7, #40]
	str r5, [sp, #44]
	ldr r0, [r7, #44]
	str r0, [sp, #40]
	ldr r1, [r7, #52]
	str r1, [sp, #36]
	ldr r2, [r7, #72]
	movs r3, #0
	str r2, [sp, #32]
	str r3, [r7, #36]
	str r3, [r7, #40]
	str r3, [r7, #44]
	str r3, [r7, #52]
	str r3, [r7, #72]
	ldr r5, [sp, #92]
	ldr r0, .L_080e4de8
	adds r3, r5, r0
	ldr r3, [r3]
	ldr r1, [sp, #56]
	ldr r0, [r3, #8]
	bl EffectPosition_ApplyStepAndYOffset
	ldr r1, [sp, #56]
	ldr r3, [r1]
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	str r3, [r1]
	movs r0, #212
	bl Func_080f9010
	movs r2, #0
	add r3, sp, #124
	str r2, [sp, #84]
	mov r8, r3
.L_080e4c64:
	ldr r0, [sp, #92]
	movs r1, #225
	movs r5, #0
	lsls r1, r1, #7
	mov r9, r5
	mov r10, r5
	adds r6, r0, r1
.L_080e4c72:
	ldr r3, [r6]
	cmp r3, #0
	blt .L_080e4d34
	mov r3, r10
	cmp r3, #0
	bge .L_080e4c80
	adds r3, #3
.L_080e4c80:
	ldr r2, [sp, #84]
	asrs r3, r3, #2
	cmp r2, r3
	blt .L_080e4d30
	movs r3, #5
	mov r11, r3
	bl Render_ResetTransformState
	ldr r0, [r6, #20]
	bl SceneTransform_ApplyRoll
	ldr r0, [r6, #12]
	bl SceneTransform_ApplyPitch
	ldr r0, [r6, #16]
	bl SceneTransform_ApplyYaw
	add r5, sp, #124
	adds r1, r5, #0
	adds r0, r6, #0
	bl EffectPosition_ApplyBaseAndYOffset
	ldr r3, [r5]
	ldr r0, [sp, #56]
	lsrs r2, r3, #31
	adds r3, r3, r2
	ldr r2, [r0]
	asrs r3, r3, #1
	adds r3, r3, r2
	str r3, [r5]
	ldr r1, [sp, #96]
	cmp r1, #7
	bgt .L_080e4ccc
	ldr r3, [r5, #4]
	ldr r2, [r0, #4]
	adds r3, r3, r2
	subs r3, #8
	b .L_080e4ce8
.L_080e4ccc:
	ldr r2, [sp, #96]
	cmp r2, #35
	bne .L_080e4cde
	ldr r0, [sp, #56]
	ldr r3, [r5, #4]
	ldr r2, [r0, #4]
	adds r3, r3, r2
	adds r3, #44
	b .L_080e4ce8
.L_080e4cde:
	ldr r1, [sp, #56]
	ldr r3, [r5, #4]
	ldr r2, [r1, #4]
	adds r3, r3, r2
	adds r3, #12
.L_080e4ce8:
	str r3, [r5, #4]
	movs r2, #60
	ldr r3, [r5, #8]
	negs r2, r2
	cmp r3, r2
	bge .L_080e4cf8
	str r2, [r5, #8]
	adds r3, r2, #0
.L_080e4cf8:
	cmp r3, #60
	ble .L_080e4d00
	movs r3, #60
	str r3, [r5, #8]
.L_080e4d00:
	adds r3, #60
	str r3, [r5, #8]
	ldr r2, .L_080e4df8
	movs r3, #10
	subs r3, #2
	ldrh r1, [r2, r3]
	ldr r2, [sp, #76]
	ldr r3, [r5, #4]
	adds r1, r2, r1
	movs r0, #10
	ldr r2, [r5]
	mov r5, r11
	str r5, [sp, #0]
	str r0, [sp, #4]
	ldr r5, [sp, #52]
	subs r3, #5
	subs r2, #2
	ldr r4, [r5, #4]
	ldr r0, [sp, #88]
	bl _call_via_r4
	ldr r3, [r6]
	subs r3, #4
	str r3, [r6]
.L_080e4d30:
	movs r0, #1
	add r9, r0
.L_080e4d34:
	movs r1, #1
	add r10, r1
	mov r2, r10
	adds r6, #28
	cmp r2, #64
	bne .L_080e4c72
	ldr r3, [sp, #96]
	cmp r3, #7
	bgt .L_080e4d8a
	mov r5, r9
	cmp r5, #63
	bgt .L_080e4d8a
	bl Render_ResetTransformState
	ldr r0, [sp, #80]
	adds r1, r0, #0
	adds r1, #12
	bl Graphics_PrepareTransferInIwramWork
	add r1, sp, #148
	adds r0, r1, #0
	mov r1, r8
	bl EffectPosition_ApplyBaseAndYOffset
	mov r3, r8
	ldr r2, [r3]
	lsrs r3, r2, #31
	adds r2, r2, r3
	mov r5, r8
	ldr r3, [r5, #4]
	asrs r2, r2, #1
	movs r1, #20
	str r2, [r5]
	str r1, [sp, #0]
	movs r1, #40
	str r1, [sp, #4]
	subs r2, #10
	subs r3, #4
	ldr r4, [sp, #104]
	ldr r0, [sp, #88]
	ldr r1, .L_080e4dfc
	bl _call_via_r4
.L_080e4d8a:
	ldr r0, [sp, #92]
	ldr r1, .L_080e4e00
	movs r3, #1
	adds r2, r0, r1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r2, [sp, #84]
	adds r2, #1
	str r2, [sp, #84]
	cmp r2, #32
	beq .L_080e4da6
	b .L_080e4c64
.L_080e4da6:
	ldr r3, [sp, #96]
	cmp r3, #11
	bne .L_080e4e0c
	ldr r3, .L_080e4ddc
	ldr r2, .L_080e4e04
	strh r3, [r2]
	ldr r5, [sp, #92]
	ldr r0, .L_080e4de8
	adds r3, r5, r0
	ldr r3, [r3]
	ldr r3, [r3, #4]
	cmp r3, #0
	bne .L_080e4dcc
	ldr r1, [sp, #60]
	ldr r5, [sp, #84]
	ldr r3, [r1]
	subs r3, r5, r3
	adds r2, #8
	b .L_080e4e54
.L_080e4dcc:
	ldr r0, [sp, #60]
	ldr r2, [r0]
	movs r3, #96
	ldr r1, .L_080e4e08
	subs r3, r3, r2
	lsls r3, r3, #8
	str r3, [r1]
	b .L_080e4e58
.L_080e4ddc:
	.4byte 0x00000100
.L_080e4de0:
	.4byte 0x00007784
.L_080e4de4:
	.4byte BattlePresentation_ProcessPendingGraphicsTransfer
.L_080e4de8:
	.4byte 0x00007828
.L_080e4dec:
	.4byte 0x000077b4
.L_080e4df0:
	.4byte 0x000077b8
.L_080e4df4:
	.4byte 0x0000ffff
.L_080e4df8:
	.4byte ParticleStreams_CellOffsets
.L_080e4dfc:
	.4byte Data_02013c56
.L_080e4e00:
	.4byte 0x00007824
.L_080e4e04:
	.4byte 0x04000020
.L_080e4e08:
	.4byte 0x04000028
.L_080e4e0c:
	ldr r1, [sp, #96]
	cmp r1, #32
	bne .L_080e4e58
	ldr r2, .L_080e4e38
	ldr r3, .L_080e4e34
	strh r3, [r2]
	ldr r2, [sp, #92]
	ldr r5, .L_080e4e3c
	adds r3, r2, r5
	ldr r3, [r3]
	ldr r3, [r3, #4]
	cmp r3, #0
	bne .L_080e4e44
	ldr r0, .L_080e4e40
	movs r1, #192
	lsls r1, r1, #12
	str r0, [sp, #72]
	str r1, [sp, #68]
	b .L_080e4e4e
	.2byte 0x0000
.L_080e4e34:
	.4byte 0x00000100
.L_080e4e38:
	.4byte 0x04000020
.L_080e4e3c:
	.4byte 0x00007828
.L_080e4e40:
	.4byte 0xff800000
.L_080e4e44:
	movs r2, #128
	ldr r3, .L_080e4ea0
	lsls r2, r2, #12
	str r2, [sp, #72]
	str r3, [sp, #68]
.L_080e4e4e:
	ldr r5, [sp, #72]
	ldr r2, .L_080e4ea4
	asrs r3, r5, #16
.L_080e4e54:
	lsls r3, r3, #8
	str r3, [r2]
.L_080e4e58:
	ldr r0, [sp, #96]
	cmp r0, #8
	bne .L_080e4ec0
	ldr r2, .L_080e4ea8
	ldr r3, .L_080e4e9c
	strh r3, [r2]
	ldr r3, [sp, #60]
	ldr r2, [r3]
	movs r3, #64
	ldr r1, .L_080e4ea4
	subs r3, r3, r2
	lsls r3, r3, #8
	str r3, [r1]
	ldr r5, [sp, #92]
	movs r0, #239
	lsls r0, r0, #7
	ldr r1, .L_080e4eac
	adds r2, r5, r0
	movs r3, #1
	str r3, [r2]
	movs r6, #0
	adds r3, r5, r1
	movs r1, #128
	str r6, [r3]
	ldr r5, .L_080e4eb0
	lsls r1, r1, #7
	ldr r0, .L_080e4eb4
	bl _call_via_r5
	movs r1, #128
	ldr r0, [sp, #88]
	lsls r1, r1, #7
	b .L_080e4eb8
	.2byte 0x0000
.L_080e4e9c:
	.4byte 0x00000100
.L_080e4ea0:
	.4byte 0xfff40000
.L_080e4ea4:
	.4byte 0x04000028
.L_080e4ea8:
	.4byte 0x04000020
.L_080e4eac:
	.4byte 0x00007784
.L_080e4eb0:
	.4byte IwramClearWords
.L_080e4eb4:
	.4byte 0x06004000
.L_080e4eb8:
	bl _call_via_r5
	ldr r3, .L_080e4ee8
	strh r6, [r3]
.L_080e4ec0:
	ldr r2, [sp, #96]
	cmp r2, #31
	bne .L_080e4f02
	ldr r3, .L_080e4ee4
	ldr r2, .L_080e4eec
	strh r3, [r2]
	ldr r5, [sp, #92]
	ldr r0, .L_080e4ef0
	adds r3, r5, r0
	ldr r3, [r3]
	ldr r3, [r3, #4]
	cmp r3, #0
	bne .L_080e4ef4
	ldr r3, [sp, #60]
	ldr r2, [r3]
	movs r3, #32
	b .L_080e4efa
	.2byte 0x0000
.L_080e4ee4:
	.4byte 0x00000100
.L_080e4ee8:
	.4byte 0x04000050
.L_080e4eec:
	.4byte 0x04000020
.L_080e4ef0:
	.4byte 0x00007828
.L_080e4ef4:
	ldr r5, [sp, #60]
	ldr r2, [r5]
	movs r3, #96
.L_080e4efa:
	ldr r1, .L_080e50bc
	subs r3, r3, r2
	lsls r3, r3, #8
	str r3, [r1]
.L_080e4f02:
	ldr r0, [sp, #96]
	cmp r0, #15
	beq .L_080e4f14
	cmp r0, #17
	beq .L_080e4f14
	cmp r0, #24
	beq .L_080e4f14
	cmp r0, #26
	bne .L_080e4f86
.L_080e4f14:
	movs r1, #128
	ldr r5, .L_080e50c0
	lsls r1, r1, #7
	ldr r0, .L_080e50c4
	bl _call_via_r5
	movs r1, #128
	ldr r0, [sp, #88]
	lsls r1, r1, #7
	bl _call_via_r5
	ldr r2, .L_080e50c8
	ldr r1, [sp, #92]
	adds r3, r1, r2
	ldr r2, [r3]
	movs r3, #0
	str r3, [r2, #28]
	ldr r0, .L_080e50cc
	bl Scheduler_RemoveCallback
	ldr r0, .L_080e50d0
	bl Scheduler_RemoveCallback
	movs r0, #47
	bl Runtime_ReleaseHeapBlock
	movs r0, #46
	bl Runtime_ReleaseHeapBlock
	adds r0, r7, #0
	movs r1, #3
	bl Func_08009080
	ldr r3, [sp, #96]
	cmp r3, #15
	bne .L_080e4f64
	ldr r0, [sp, #100]
	movs r1, #9
	bl BattleFx_RunProjectileVolley
.L_080e4f64:
	ldr r5, [sp, #96]
	cmp r5, #24
	bne .L_080e4f70
	ldr r0, [sp, #100]
	bl BattleFx_RenderMode5
.L_080e4f70:
	ldr r0, [sp, #96]
	cmp r0, #26
	beq .L_080e4f7a
	bl .L_080e65f8
.L_080e4f7a:
	ldr r0, [sp, #100]
	movs r1, #8
	bl BattleFx_RunProjectileVolley
	bl .L_080e65f8
.L_080e4f86:
	adds r0, r7, #0
	movs r1, #16
	bl ObjectDispatch_ApplyValueToChildrenFar
	ldr r1, [sp, #48]
	str r1, [r7, #36]
	ldr r2, [sp, #44]
	str r2, [r7, #40]
	ldr r3, [sp, #40]
	str r3, [r7, #44]
	ldr r5, [sp, #36]
	str r5, [r7, #52]
	ldr r0, [sp, #32]
	str r0, [r7, #72]
	ldr r1, [sp, #96]
	cmp r1, #35
	bne .L_080e4ff4
	movs r1, #128
	ldr r5, .L_080e50c0
	lsls r1, r1, #7
	ldr r0, .L_080e50c4
	bl _call_via_r5
	movs r1, #128
	lsls r1, r1, #7
	ldr r0, [sp, #88]
	bl _call_via_r5
	ldr r5, .L_080e50c8
	ldr r2, [sp, #92]
	adds r3, r2, r5
	ldr r2, [r3]
	movs r3, #0
	str r3, [r2, #28]
	ldr r0, .L_080e50cc
	bl Scheduler_RemoveCallback
	ldr r0, .L_080e50d0
	bl Scheduler_RemoveCallback
	movs r0, #47
	bl Runtime_ReleaseHeapBlock
	movs r0, #46
	bl Runtime_ReleaseHeapBlock
	ldr r0, [sp, #100]
	movs r3, #3
	str r3, [r0, #24]
	ldr r0, [sp, #100]
	movs r1, #2
	bl BattleFx_RunSparkGroups
	bl .L_080e65f8
.L_080e4ff4:
	ldr r1, [sp, #92]
	ldr r2, .L_080e50c8
	adds r3, r1, r2
	ldr r3, [r3]
	movs r5, #36
	ldrsh r0, [r3, r5]
	bl GetBattleObjectSlotFar
	ldr r0, [r0]
	str r0, [sp, #28]
	mov r0, sp
	adds r0, #136
	ldr r1, [sp, #28]
	ldr r2, [sp, #8]
	str r0, [sp, #24]
	ldr r3, [r2]
	ldr r0, [r1, #8]
	movs r1, #6
	subs r0, r0, r3
	bl FixedPoint_Ratio
	ldr r3, [sp, #24]
	str r0, [r3]
	ldr r5, [sp, #28]
	ldr r1, [sp, #8]
	ldr r0, [r5, #12]
	ldr r3, [r1, #4]
	movs r2, #240
	lsls r2, r2, #13
	subs r0, r0, r3
	adds r0, r0, r2
	movs r1, #6
	bl FixedPoint_Ratio
	ldr r3, [sp, #24]
	str r0, [r3, #4]
	ldr r0, [r5, #16]
	ldr r5, [sp, #8]
	ldr r3, [r5, #8]
	movs r1, #6
	subs r0, r0, r3
	bl FixedPoint_Ratio
	ldr r1, [sp, #24]
	str r0, [r1, #8]
	ldr r5, [sp, #92]
	ldr r0, .L_080e50d4
	movs r2, #0
	mov r10, r2
	adds r3, r5, r0
.L_080e5058:
	movs r1, #1
	add r10, r1
	mov r5, r10
	str r2, [r3]
	adds r3, #28
	cmp r5, #64
	bne .L_080e5058
	ldr r0, [sp, #96]
	cmp r0, #14
	beq .L_080e5114
	ldr r1, [sp, #92]
	ldr r2, .L_080e50c8
	adds r3, r1, r2
	ldr r3, [r3]
	movs r5, #36
	ldrsh r0, [r3, r5]
	bl Func_080b5070
	lsrs r3, r0, #31
	adds r0, r0, r3
	ldr r1, [sp, #92]
	movs r2, #225
	asrs r7, r0, #1
	lsls r2, r2, #7
	movs r0, #0
	mov r10, r0
	movs r6, #255
	adds r5, r1, r2
.L_080e5090:
	ldr r0, [sp, #28]
	ldr r3, [r0, #8]
	str r7, [r5, #4]
	str r3, [r5]
	ldr r3, [r0, #16]
	str r3, [r5, #8]
	ldr r1, [sp, #96]
	cmp r1, #31
	bne .L_080e50d8
	bl Random16
	ands r0, r6
	subs r0, #127
	lsls r0, r0, #12
	str r0, [r5, #12]
	bl Random16
	ands r0, r6
	subs r0, #64
	lsls r0, r0, #10
	b .L_080e50ee
	.2byte 0x0000
.L_080e50bc:
	.4byte 0x04000028
.L_080e50c0:
	.4byte IwramClearWords
.L_080e50c4:
	.4byte 0x06004000
.L_080e50c8:
	.4byte 0x00007828
.L_080e50cc:
	.4byte Palette_StepFadeTransfer
.L_080e50d0:
	.4byte BattlePresentation_ProcessPendingGraphicsTransfer
.L_080e50d4:
	.4byte 0x00007098
.L_080e50d8:
	bl Random16
	ands r0, r6
	subs r0, #127
	lsls r0, r0, #12
	str r0, [r5, #12]
	bl Random16
	ands r0, r6
	subs r0, #64
	lsls r0, r0, #12
.L_080e50ee:
	str r0, [r5, #16]
	bl Random16
	ands r0, r6
	subs r0, #127
	lsls r0, r0, #12
	str r0, [r5, #20]
	mov r2, r10
	lsrs r3, r2, #31
	add r3, r10
	asrs r3, r3, #1
	adds r3, #32
	str r3, [r5, #24]
	movs r3, #1
	add r10, r3
	mov r0, r10
	adds r5, #28
	cmp r0, #32
	bne .L_080e5090
.L_080e5114:
	ldr r1, [sp, #96]
	cmp r1, #11
	bne .L_080e5138
	ldr r1, [sp, #92]
	ldr r0, .L_080e5164
	movs r2, #1
	movs r3, #1
	bl Resource_LoadAndDecompress
	movs r2, #1
	movs r3, #0
	ldr r0, .L_080e5168
	ldr r1, .L_080e516c
	bl Resource_LoadAndDecompress
	ldr r2, .L_080e5170
	ldr r3, .L_080e5160
	strh r3, [r2]
.L_080e5138:
	ldr r2, [sp, #96]
	cmp r2, #32
	bne .L_080e517c
	ldr r1, [sp, #92]
	ldr r0, .L_080e5174
	movs r2, #1
	movs r3, #1
	bl Resource_LoadAndDecompress
	movs r2, #1
	movs r3, #0
	ldr r0, .L_080e5178
	ldr r1, .L_080e516c
	bl Resource_LoadAndDecompress
	ldr r2, .L_080e5170
	ldr r3, .L_080e5160
	strh r3, [r2]
	b .L_080e517c
	.2byte 0x0000
.L_080e5160:
	.4byte 0x00000e10
.L_080e5164:
	.4byte 0x000000ab
.L_080e5168:
	.4byte 0x000000ac
.L_080e516c:
	.4byte gMapCellBuffer
.L_080e5170:
	.4byte 0x04000052
.L_080e5174:
	.4byte 0x000000ad
.L_080e5178:
	.4byte 0x000000ae
.L_080e517c:
	ldr r3, [sp, #96]
	cmp r3, #7
	beq .L_080e523e
	cmp r3, #13
	beq .L_080e523e
	cmp r3, #18
	beq .L_080e523e
	cmp r3, #11
	beq .L_080e523e
	cmp r3, #32
	beq .L_080e523e
	cmp r3, #19
	beq .L_080e523e
	movs r7, #0
	cmp r3, #12
	beq .L_080e51a0
	movs r7, #160
	lsls r7, r7, #13
.L_080e51a0:
	movs r5, #0
	mov r10, r5
	ldr r5, .L_080e54cc
	movs r6, #255
.L_080e51a8:
	ldr r0, [sp, #28]
	ldr r3, [r0, #8]
	str r7, [r5, #4]
	str r3, [r5]
	ldr r3, [r0, #16]
	str r3, [r5, #8]
	ldr r1, [sp, #96]
	cmp r1, #5
	beq .L_080e51be
	cmp r1, #23
	bne .L_080e51e0
.L_080e51be:
	bl Random16
	ands r0, r6
	subs r0, #127
	lsls r0, r0, #11
	str r0, [r5, #12]
	bl Random16
	ands r0, r6
	lsls r0, r0, #11
	str r0, [r5, #16]
	bl Random16
	ands r0, r6
	subs r0, #127
	lsls r0, r0, #11
	b .L_080e522c
.L_080e51e0:
	ldr r2, [sp, #96]
	cmp r2, #25
	bne .L_080e520a
	bl Random16
	ands r0, r6
	subs r0, #127
	lsls r0, r0, #11
	str r0, [r5, #12]
	bl Random16
	movs r3, #127
	ands r3, r0
	lsls r3, r3, #10
	str r3, [r5, #16]
	bl Random16
	ands r0, r6
	subs r0, #127
	lsls r0, r0, #11
	b .L_080e522c
.L_080e520a:
	bl Random16
	ands r0, r6
	subs r0, #127
	lsls r0, r0, #10
	str r0, [r5, #12]
	bl Random16
	movs r3, #127
	ands r3, r0
	lsls r3, r3, #10
	str r3, [r5, #16]
	bl Random16
	ands r0, r6
	subs r0, #127
	lsls r0, r0, #10
.L_080e522c:
	str r0, [r5, #20]
	movs r3, #0
	str r3, [r5, #24]
	movs r3, #1
	add r10, r3
	mov r0, r10
	adds r5, #28
	cmp r0, #64
	bne .L_080e51a8
.L_080e523e:
	ldr r1, [sp, #96]
	subs r1, #2
	str r1, [sp, #20]
	cmp r1, #1
	bls .L_080e525a
	ldr r2, [sp, #96]
	cmp r2, #12
	beq .L_080e525a
	cmp r2, #22
	beq .L_080e525a
	cmp r2, #29
	beq .L_080e525a
	cmp r2, #28
	bne .L_080e5264
.L_080e525a:
	movs r1, #144
	ldr r0, .L_080e54d0
	lsls r1, r1, #3
	bl Scheduler_AddOrUpdateCallback
.L_080e5264:
	ldr r3, [sp, #96]
	subs r3, #4
	str r3, [sp, #16]
	cmp r3, #2
	bls .L_080e5288
	ldr r5, [sp, #96]
	cmp r5, #23
	beq .L_080e5288
	cmp r5, #30
	beq .L_080e5288
	cmp r5, #27
	beq .L_080e5288
	cmp r5, #33
	beq .L_080e5288
	cmp r5, #34
	beq .L_080e5288
	cmp r5, #100
	bne .L_080e528e
.L_080e5288:
	movs r0, #32
	str r0, [sp, #64]
	b .L_080e52ea
.L_080e528e:
	ldr r1, [sp, #96]
	cmp r1, #3
	bls .L_080e52b4
	cmp r1, #8
	beq .L_080e52b4
	cmp r1, #9
	beq .L_080e52b4
	cmp r1, #10
	beq .L_080e52b4
	cmp r1, #22
	beq .L_080e52b4
	cmp r1, #25
	beq .L_080e52b4
	cmp r1, #29
	beq .L_080e52b4
	cmp r1, #31
	beq .L_080e52b4
	cmp r1, #14
	bne .L_080e52ba
.L_080e52b4:
	movs r2, #48
	str r2, [sp, #64]
	b .L_080e52ea
.L_080e52ba:
	ldr r5, [sp, #96]
	movs r3, #20
	str r3, [sp, #64]
	cmp r5, #21
	beq .L_080e52ea
	ldr r0, [sp, #96]
	cmp r0, #11
	beq .L_080e52d2
	cmp r0, #32
	beq .L_080e52d2
	cmp r0, #20
	bne .L_080e52d8
.L_080e52d2:
	movs r1, #40
	str r1, [sp, #64]
	b .L_080e52ea
.L_080e52d8:
	ldr r2, [sp, #96]
	cmp r2, #28
	beq .L_080e52e6
	movs r3, #80
	str r3, [sp, #64]
	cmp r2, #12
	bne .L_080e52ea
.L_080e52e6:
	movs r5, #64
	str r5, [sp, #64]
.L_080e52ea:
	ldr r1, [sp, #64]
	movs r0, #0
	str r0, [sp, #84]
	cmp r1, #0
	bne .L_080e52f8
	bl .L_080e657c
.L_080e52f8:
	ldr r2, [sp, #96]
	cmp r2, #11
	beq .L_080e5334
	cmp r2, #32
	beq .L_080e5334
	ldr r3, [sp, #92]
	movs r5, #211
	ldr r1, [sp, #84]
	lsls r5, r5, #7
	movs r0, #0
	movs r7, #128
	adds r6, r3, r5
	mov r10, r0
	lsls r7, r7, #11
	lsls r5, r1, #12
.L_080e5316:
	adds r0, r5, #0
	bl Trig_Sin
	lsls r0, r0, #2
	subs r0, r7, r0
	movs r3, #1
	asrs r0, r0, #10
	movs r2, #128
	add r10, r3
	stmia r6!, {r0}
	lsls r2, r2, #4
	mov r0, r10
	adds r5, r5, r2
	cmp r0, #160
	bne .L_080e5316
.L_080e5334:
	ldr r1, [sp, #84]
	cmp r1, #2
	bgt .L_080e535c
	ldr r2, [sp, #92]
	ldr r5, .L_080e54d4
	adds r3, r2, r5
	ldr r3, [r3]
	ldr r1, [sp, #56]
	ldr r0, [r3, #8]
	bl EffectPosition_ApplyStepAndYOffset
	ldr r0, [sp, #56]
	ldr r3, [r0]
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	str r3, [r0]
	ldr r3, [r0, #4]
	adds r3, #16
	str r3, [r0, #4]
.L_080e535c:
	ldr r1, [sp, #96]
	cmp r1, #11
	beq .L_080e53e2
	cmp r1, #8
	beq .L_080e53e2
	cmp r1, #32
	beq .L_080e53e2
	cmp r1, #33
	beq .L_080e53ea
	cmp r1, #34
	beq .L_080e53e2
	ldr r2, [sp, #84]
	cmp r2, #11
	bgt .L_080e53e2
	ldr r5, [sp, #92]
	ldr r0, .L_080e54d4
	adds r3, r5, r0
	ldr r3, [r3]
	ldr r3, [r3, #4]
	cmp r3, #0
	bne .L_080e53b4
	lsrs r3, r2, #31
	adds r3, r2, r3
	asrs r3, r3, #1
	lsls r1, r3, #3
	subs r1, r1, r3
	lsls r1, r1, #2
	subs r1, r1, r3
	ldr r3, [sp, #56]
	movs r0, #48
	ldr r2, [r3]
	ldr r3, [r3, #4]
	lsls r1, r1, #7
	str r0, [sp, #0]
	movs r0, #72
	str r0, [sp, #4]
	adds r1, r5, r1
	subs r2, #32
	subs r3, #40
	ldr r4, [sp, #104]
	ldr r0, [sp, #88]
	bl _call_via_r4
	b .L_080e53e2
.L_080e53b4:
	ldr r5, [sp, #84]
	lsrs r3, r5, #31
	adds r3, r5, r3
	asrs r3, r3, #1
	lsls r1, r3, #3
	subs r1, r1, r3
	lsls r1, r1, #2
	subs r1, r1, r3
	ldr r0, [sp, #92]
	ldr r3, [sp, #56]
	lsls r1, r1, #7
	ldr r2, [r3]
	adds r1, r0, r1
	ldr r3, [r3, #4]
	movs r0, #48
	str r0, [sp, #0]
	movs r0, #72
	str r0, [sp, #4]
	subs r3, #40
	ldr r4, [sp, #104]
	ldr r0, [sp, #88]
	bl _call_via_r4
.L_080e53e2:
	ldr r5, [sp, #96]
	cmp r5, #33
	bls .L_080e53ea
	b .L_080e551a
.L_080e53ea:
	ldr r0, [sp, #96]
	ldr r2, .L_080e54d8
	lsls r3, r0, #2
	ldr r3, [r3, r2]
	mov pc, r3
.L_080e53f4:
	.4byte .L_080e549c
	.4byte .L_080e54c4
	.4byte .L_080e54bc
	.4byte .L_080e5504
	.4byte .L_080e551a
	.4byte .L_080e5514
	.4byte .L_080e551a
	.4byte .L_080e551a
	.4byte .L_080e5494
	.4byte .L_080e550c
	.4byte .L_080e549c
	.4byte .L_080e551a
	.4byte .L_080e54a4
	.4byte .L_080e54a4
	.4byte .L_080e5484
	.4byte .L_080e551a
	.4byte .L_080e551a
	.4byte .L_080e551a
	.4byte .L_080e54ac
	.4byte .L_080e54b4
	.4byte .L_080e5504
	.4byte .L_080e551a
	.4byte .L_080e5504
	.4byte .L_080e5514
	.4byte .L_080e551a
	.4byte .L_080e54a4
	.4byte .L_080e551a
	.4byte .L_080e551a
	.4byte .L_080e54c4
	.4byte .L_080e54bc
	.4byte .L_080e551a
	.4byte .L_080e548c
	.4byte .L_080e551a
	.4byte .L_080e547c
.L_080e547c:
	ldr r0, .L_080e54dc
	bl BattleFx_StepPaletteToResource
	b .L_080e551a
.L_080e5484:
	ldr r0, .L_080e54e0
	bl BattleFx_StepPaletteToResource
	b .L_080e551a
.L_080e548c:
	ldr r0, .L_080e54e4
	bl BattleFx_StepPaletteToResource
	b .L_080e551a
.L_080e5494:
	ldr r0, .L_080e54e8
	bl BattleFx_StepPaletteToResource
	b .L_080e551a
.L_080e549c:
	ldr r0, .L_080e54ec
	bl BattleFx_StepPaletteToResource
	b .L_080e551a
.L_080e54a4:
	ldr r0, .L_080e54f0
	bl BattleFx_StepPaletteToResource
	b .L_080e551a
.L_080e54ac:
	ldr r0, .L_080e54f4
	bl BattleFx_StepPaletteToResource
	b .L_080e551a
.L_080e54b4:
	ldr r0, .L_080e54f8
	bl BattleFx_StepPaletteToResource
	b .L_080e551a
.L_080e54bc:
	ldr r0, .L_080e54fc
	bl BattleFx_StepPaletteToResource
	b .L_080e551a
.L_080e54c4:
	ldr r0, .L_080e5500
	bl BattleFx_StepPaletteToResource
	b .L_080e551a
.L_080e54cc:
	.4byte Data_02014000
.L_080e54d0:
	.4byte BattleFx_ArmBg2AffineHBlankDma
.L_080e54d4:
	.4byte 0x00007828
.L_080e54d8:
	.4byte .L_080e53f4
.L_080e54dc:
	.4byte 0x00000053
.L_080e54e0:
	.4byte 0x0000006f
.L_080e54e4:
	.4byte 0x00000079
.L_080e54e8:
	.4byte 0x000000c3
.L_080e54ec:
	.4byte 0x0000008d
.L_080e54f0:
	.4byte 0x000000bb
.L_080e54f4:
	.4byte 0x000000b9
.L_080e54f8:
	.4byte 0x000000c0
.L_080e54fc:
	.4byte 0x000000a4
.L_080e5500:
	.4byte 0x000000a3
.L_080e5504:
	ldr r0, .L_080e5790
	bl BattleFx_StepPaletteToResource
	b .L_080e551a
.L_080e550c:
	ldr r0, .L_080e5794
	bl BattleFx_StepPaletteToResource
	b .L_080e551a
.L_080e5514:
	ldr r0, .L_080e5798
	bl BattleFx_StepPaletteToResource
.L_080e551a:
	ldr r1, [sp, #96]
	cmp r1, #11
	beq .L_080e5608
	cmp r1, #8
	beq .L_080e5608
	cmp r1, #32
	beq .L_080e5608
	ldr r2, [sp, #84]
	subs r2, #4
	cmp r2, #11
	bhi .L_080e556a
	lsrs r3, r2, #31
	adds r3, r2, r3
	asrs r3, r3, #1
	lsls r1, r3, #4
	subs r1, r1, r3
	ldr r2, [sp, #92]
	ldr r5, [sp, #60]
	lsls r1, r1, #6
	adds r1, r2, r1
	movs r3, #162
	ldr r2, [r5]
	lsls r3, r3, #7
	ldr r0, [sp, #56]
	adds r1, r1, r3
	lsrs r3, r2, #31
	adds r2, r2, r3
	ldr r3, [r0, #4]
	movs r0, #20
	str r0, [sp, #0]
	movs r0, #48
	str r0, [sp, #4]
	ldr r5, [sp, #52]
	asrs r2, r2, #1
	subs r2, #8
	subs r3, #24
	ldr r4, [r5, #4]
	ldr r0, [sp, #88]
	bl _call_via_r4
.L_080e556a:
	bl Render_ResetTransformState
	ldr r1, [sp, #80]
	ldr r0, [sp, #80]
	adds r1, #12
	bl Graphics_PrepareTransferInIwramWork
	ldr r0, [sp, #84]
	cmp r0, #3
	ble .L_080e5608
	movs r2, #112
	movs r1, #0
	add r2, sp
	mov r10, r1
	mov r9, r2
.L_080e5588:
	mov r5, r10
	lsrs r3, r5, #31
	add r3, r10
	asrs r7, r3, #1
	lsls r3, r7, #3
	subs r3, r3, r7
	ldr r0, [sp, #92]
	lsls r3, r3, #2
	movs r1, #225
	adds r3, r0, r3
	lsls r1, r1, #7
	adds r1, r1, r3
	ldr r5, [r1, #24]
	mov r8, r1
	cmp r5, #0
	ble .L_080e55fe
	mov r6, r9
	adds r1, r6, #0
	mov r0, r8
	bl EffectPosition_ApplyBaseAndYOffset
	ldr r2, [r6]
	asrs r5, r5, #4
	lsrs r3, r2, #31
	adds r5, #1
	adds r2, r2, r3
	lsls r4, r5, #1
	asrs r2, r2, #1
	ldr r1, .L_080e579c
	str r2, [r6]
	subs r3, r4, #2
	ldrh r1, [r1, r3]
	ldr r3, [sp, #76]
	adds r1, r3, r1
	lsrs r3, r5, #31
	adds r3, r5, r3
	asrs r3, r3, #1
	subs r2, r2, r3
	ldr r3, [r6, #4]
	movs r0, #1
	subs r3, r3, r5
	str r5, [sp, #0]
	str r4, [sp, #4]
	ldr r5, [sp, #52]
	ands r0, r7
	lsls r0, r0, #2
	ldr r4, [r0, r5]
	ldr r0, [sp, #88]
	bl _call_via_r4
	mov r0, r8
	movs r1, #60
	ldr r2, .L_080e57a0
	bl EffectStep_AdvanceWithGravity3D
	mov r0, r8
	ldr r3, [r0, #24]
	subs r3, #1
	str r3, [r0, #24]
.L_080e55fe:
	movs r1, #1
	add r10, r1
	mov r2, r10
	cmp r2, #128
	bne .L_080e5588
.L_080e5608:
	ldr r3, [sp, #96]
	cmp r3, #7
	beq .L_080e561c
	cmp r3, #13
	beq .L_080e561c
	cmp r3, #18
	beq .L_080e561c
	cmp r3, #19
	beq .L_080e561c
	b .L_080e57b4
.L_080e561c:
	ldr r5, [sp, #84]
	cmp r5, #50
	bne .L_080e563c
	ldr r0, [sp, #92]
	ldr r1, .L_080e57a4
	adds r3, r0, r1
	ldr r3, [r3]
	ldr r0, [r3, #8]
	movs r3, #1
	movs r2, #0
	negs r3, r3
	str r2, [sp, #0]
	movs r1, #7
	adds r2, r3, #0
	bl ObjectGroup_UpdateMembers
.L_080e563c:
	ldr r2, [sp, #84]
	cmp r2, #79
	bne .L_080e565c
	ldr r0, .L_080e57a4
	ldr r5, [sp, #92]
	adds r3, r5, r0
	ldr r3, [r3]
	ldr r0, [r3, #8]
	movs r3, #1
	movs r2, #0
	negs r3, r3
	str r2, [sp, #0]
	movs r1, #0
	adds r2, r3, #0
	bl ObjectGroup_UpdateMembers
.L_080e565c:
	ldr r1, [sp, #84]
	cmp r1, #12
	bne .L_080e56ae
	movs r2, #0
	ldr r5, .L_080e57a8
	mov r10, r2
	movs r6, #255
.L_080e566a:
	ldr r0, [sp, #28]
	ldr r3, [r0, #8]
	str r3, [r5]
	movs r3, #160
	lsls r3, r3, #13
	str r3, [r5, #4]
	ldr r3, [r0, #16]
	str r3, [r5, #8]
	bl Random16
	ands r0, r6
	subs r0, #128
	lsls r0, r0, #10
	str r0, [r5, #12]
	bl Random16
	ands r0, r6
	subs r0, #128
	lsls r0, r0, #10
	str r0, [r5, #16]
	bl Random16
	movs r1, #1
	ands r0, r6
	subs r0, #128
	add r10, r1
	lsls r0, r0, #10
	movs r3, #0
	mov r2, r10
	str r0, [r5, #20]
	str r3, [r5, #24]
	adds r5, #28
	cmp r2, #64
	bne .L_080e566a
.L_080e56ae:
	ldr r3, [sp, #84]
	cmp r3, #11
	bgt .L_080e56b8
	bl .L_080e640e
.L_080e56b8:
	ldr r0, [sp, #92]
	ldr r1, .L_080e57a4
	adds r5, r0, r1
	ldr r3, [r5]
	ldr r0, [r3, #8]
	bl GetBattleObjectSlotFar
	ldr r3, [r5]
	ldr r0, [r0]
	mov r8, r0
	ldr r0, [r3, #8]
	bl Func_080b5070
	lsrs r3, r0, #31
	adds r0, r0, r3
	movs r3, #112
	asrs r0, r0, #1
	movs r2, #0
	add r3, sp
	ldr r7, .L_080e57a8
	mov r11, r0
	mov r10, r2
	mov r9, r3
.L_080e56e6:
	ldr r3, [r7, #24]
	cmp r3, #0
	blt .L_080e577e
	mov r6, r9
	mov r0, r10
	movs r5, #1
	ands r5, r0
	adds r1, r6, #0
	adds r0, r7, #0
	bl EffectPosition_ApplyBaseAndYOffset
	ldr r2, [r6]
	adds r5, #6
	lsls r0, r5, #1
	asrs r2, r2, #1
	ldr r1, .L_080e579c
	str r2, [r6]
	subs r3, r0, #2
	ldrh r1, [r1, r3]
	ldr r3, [sp, #76]
	adds r1, r3, r1
	lsrs r3, r5, #1
	subs r2, r2, r3
	ldr r3, [r6, #4]
	ldr r4, [sp, #104]
	subs r3, r3, r5
	str r5, [sp, #0]
	str r0, [sp, #4]
	ldr r0, [sp, #88]
	bl _call_via_r4
	adds r0, r7, #0
	movs r1, #62
	movs r2, #0
	bl EffectStep_AdvanceWithGravity3D
	mov r3, r10
	ldr r5, [sp, #84]
	adds r3, #22
	cmp r5, r3
	ble .L_080e577e
	mov r1, r8
	ldr r0, [r1, #8]
	ldr r3, [r7]
	ldr r1, [r1, #12]
	subs r0, r0, r3
	ldr r3, [r7, #4]
	mov r2, r8
	add r1, r11
	subs r1, r1, r3
	ldr r3, [r2, #16]
	ldr r2, [r7, #8]
	subs r3, r3, r2
	asrs r4, r3, #8
	ldr r3, [r7, #12]
	asrs r0, r0, #8
	adds r3, r3, r0
	str r3, [r7, #12]
	ldr r3, [r7, #16]
	asrs r1, r1, #8
	adds r3, r3, r1
	str r3, [r7, #16]
	ldr r3, [r7, #20]
	adds r3, r3, r4
	str r3, [r7, #20]
	ldr r3, .L_080e57ac
	ldr r2, .L_080e57b0
	adds r0, r0, r3
	cmp r0, r2
	bhi .L_080e577e
	adds r3, r4, r3
	cmp r3, r2
	bhi .L_080e577e
	movs r3, #1
	negs r3, r3
	str r3, [r7, #24]
.L_080e577e:
	movs r5, #1
	add r10, r5
	mov r0, r10
	adds r7, #28
	cmp r0, #32
	bne .L_080e56e6
	bl .L_080e640e
	.2byte 0x0000
.L_080e5790:
	.4byte 0x000000b4
.L_080e5794:
	.4byte 0x000000a0
.L_080e5798:
	.4byte 0x0000007d
.L_080e579c:
	.4byte ParticleStreams_CellOffsets
.L_080e57a0:
	.4byte 0xfffff000
.L_080e57a4:
	.4byte 0x00007828
.L_080e57a8:
	.4byte Data_02014000
.L_080e57ac:
	.4byte 0x00000fff
.L_080e57b0:
	.4byte 0x00001ffe
.L_080e57b4:
	ldr r1, [sp, #96]
	cmp r1, #21
	bne .L_080e57be
	bl .L_080e640e
.L_080e57be:
	cmp r1, #6
	beq .L_080e57c8
	cmp r1, #27
	beq .L_080e57c8
	b .L_080e58f2
.L_080e57c8:
	ldr r3, [sp, #84]
	subs r3, #6
	cmp r3, #13
	bhi .L_080e5818
	movs r2, #0
	ldr r5, [sp, #84]
	mov r10, r2
.L_080e57d6:
	lsrs r3, r5, #31
	adds r3, r5, r3
	movs r2, #3
	asrs r3, r3, #1
	ands r3, r2
	lsls r2, r3, #1
	adds r2, r2, r3
	ldr r0, [sp, #60]
	lsls r1, r2, #4
	subs r1, r1, r2
	ldr r3, .L_080e5acc
	ldr r2, [r0]
	lsls r1, r1, #6
	adds r1, r1, r3
	lsrs r3, r2, #31
	adds r2, r2, r3
	movs r3, #24
	str r3, [sp, #0]
	asrs r2, r2, #1
	movs r3, #104
	subs r2, #8
	str r3, [sp, #4]
	ldr r4, [sp, #104]
	ldr r0, [sp, #88]
	movs r3, #0
	bl _call_via_r4
	movs r1, #1
	add r10, r1
	mov r2, r10
	adds r5, #3
	cmp r2, #2
	bne .L_080e57d6
.L_080e5818:
	ldr r3, [sp, #84]
	subs r3, #8
	cmp r3, #15
	bls .L_080e5824
	bl .L_080e640e
.L_080e5824:
	ldr r5, .L_080e5ad0
	ldr r0, .L_080e5ad4
	movs r3, #0
	mov r10, r3
	movs r7, #3
	mov r11, r5
	mov r9, r0
.L_080e5832:
	mov r8, r10
	mov r1, r8
	ands r1, r7
	mov r8, r1
	bl Random16
	ldr r3, .L_080e5ad8
	adds r5, r0, #0
	ands r5, r3
	adds r0, r5, #0
	bl Trig_Sin
	ldr r2, [sp, #60]
	ldr r3, [r2]
	adds r6, r0, #0
	lsrs r2, r3, #31
	adds r3, r3, r2
	lsls r6, r6, #3
	mov r1, r8
	asrs r3, r3, #1
	mov r0, r11
	asrs r6, r6, #16
	adds r6, r6, r3
	ldrb r3, [r0, r1]
	adds r0, r5, #0
	lsrs r3, r3, #1
	subs r6, r6, r3
	bl Func_0800231c
	mov r2, r9
	adds r5, r0, #0
	mov r0, r8
	ldrb r3, [r2, r0]
	lsls r5, r5, #5
	lsrs r3, r3, #1
	movs r0, #47
	asrs r5, r5, #16
	subs r5, r5, r3
	bl Runtime_ReleaseHeapBlock
	movs r0, #46
	bl Runtime_ReleaseHeapBlock
	bl Random16
	ldr r3, .L_080e5adc
	ands r0, r7
	ldrb r2, [r3, r0]
	adds r3, r7, #0
	orrs r3, r2
	movs r2, #2
	str r2, [sp, #0]
	movs r1, #7
	movs r2, #7
	movs r0, #47
	bl Unnamed_080ed408
	ldr r2, .L_080e5ae0
	mov r1, r8
	lsls r3, r1, #1
	ldrh r1, [r2, r3]
	ldr r2, .L_080e5ae4
	mov r0, r11
	adds r1, r1, r2
	mov r2, r8
	ldrb r3, [r0, r2]
	str r3, [sp, #0]
	mov r0, r9
	ldrb r3, [r0, r2]
	ldr r2, .L_080e5ae8
	str r3, [sp, #4]
	adds r5, #56
	ldr r4, [r2]
	adds r3, r5, #0
	adds r2, r6, #0
	ldr r0, [sp, #88]
	bl _call_via_r4
	movs r0, #47
	bl Runtime_ReleaseHeapBlock
	ldr r0, .L_080e5aec
	ldr r5, [sp, #92]
	adds r3, r5, r0
	ldr r3, [r3]
	ldr r1, [sp, #52]
	ldr r0, [r3, #4]
	bl BattleFx_FetchRectangleBlitters
	movs r1, #1
	add r10, r1
	mov r2, r10
	cmp r2, #3
	bne .L_080e5832
	bl .L_080e640e
.L_080e58f2:
	ldr r3, [sp, #96]
	cmp r3, #14
	beq .L_080e58fa
	b .L_080e5a2e
.L_080e58fa:
	movs r0, #47
	bl Runtime_ReleaseHeapBlock
	movs r0, #46
	bl Runtime_ReleaseHeapBlock
	ldr r5, [sp, #84]
	cmp r5, #23
	bls .L_080e590e
	b .L_080e5ab6
.L_080e590e:
	ldr r0, [sp, #60]
	ldr r3, [r0]
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	str r3, [sp, #12]
	lsls r3, r5, #5
	subs r3, #232
	mov r11, r3
	lsls r3, r5, #4
	adds r7, r3, #0
	mov r1, r11
	subs r7, #48
	cmp r1, #0
	ble .L_080e5930
	movs r2, #0
	mov r11, r2
.L_080e5930:
	cmp r7, #104
	ble .L_080e593a
.L_080e5934:
	subs r7, #104
	cmp r7, #104
	bgt .L_080e5934
.L_080e593a:
	movs r3, #2
	str r3, [sp, #0]
	movs r1, #7
	movs r2, #7
	movs r3, #3
	movs r0, #47
	bl Unnamed_080ed408
	ldr r3, .L_080e5af0
	ldr r0, [sp, #12]
	mov r8, r3
	movs r5, #188
	mov r1, r11
	add r8, r5
	adds r1, r1, r7
	movs r6, #17
	movs r5, #104
	str r5, [sp, #4]
	str r6, [sp, #0]
	mov r9, r1
	subs r0, #8
	mov r10, r0
	mov r2, r8
	mov r3, r9
	ldr r0, [sp, #88]
	ldr r4, [r2]
	subs r3, #104
	ldr r1, .L_080e5ae4
	mov r2, r10
	bl _call_via_r4
	subs r5, r5, r7
	str r5, [sp, #4]
	str r6, [sp, #0]
	mov r3, r8
	ldr r4, [r3]
	ldr r0, [sp, #88]
	ldr r1, .L_080e5ae4
	mov r2, r10
	mov r3, r9
	bl _call_via_r4
	movs r0, #34
	str r0, [sp, #0]
	movs r0, #65
	str r0, [sp, #4]
	ldr r2, [sp, #12]
	mov r3, r11
	mov r5, r8
	ldr r1, .L_080e5af4
	subs r2, #17
	adds r3, #47
	ldr r4, [r5]
	ldr r0, [sp, #88]
	bl _call_via_r4
	movs r0, #47
	bl Runtime_ReleaseHeapBlock
	ldr r0, [sp, #84]
	cmp r0, #8
	bne .L_080e59be
	ldr r1, [sp, #92]
	ldr r2, .L_080e5af8
	adds r3, r1, r2
	str r0, [r3]
.L_080e59be:
	ldr r3, [sp, #84]
	cmp r3, #1
	ble .L_080e5ab6
	ldr r0, [sp, #92]
	movs r1, #225
	movs r5, #0
	lsls r1, r1, #7
	mov r10, r5
	movs r7, #0
	movs r6, #255
	adds r5, r0, r1
.L_080e59d4:
	ldr r3, [r5, #24]
	cmp r3, #0
	bne .L_080e5a20
	ldr r2, [sp, #28]
	ldr r3, [r2, #8]
	str r3, [r5]
	movs r3, #160
	lsls r3, r3, #13
	str r3, [r5, #4]
	ldr r3, [r2, #16]
	str r3, [r5, #8]
	bl Random16
	ands r0, r6
	subs r0, #127
	lsls r0, r0, #12
	str r0, [r5, #12]
	bl Random16
	ands r0, r6
	subs r0, #64
	lsls r0, r0, #10
	str r0, [r5, #16]
	bl Random16
	ands r0, r6
	subs r0, #127
	lsls r0, r0, #12
	str r0, [r5, #20]
	mov r0, r10
	lsrs r3, r0, #31
	add r3, r10
	asrs r3, r3, #1
	adds r3, #32
	adds r7, #1
	str r3, [r5, #24]
	cmp r7, #4
	beq .L_080e5ab6
.L_080e5a20:
	movs r1, #1
	add r10, r1
	mov r2, r10
	adds r5, #28
	cmp r2, #64
	bne .L_080e59d4
	b .L_080e5ab6
.L_080e5a2e:
	ldr r1, [sp, #96]
	cmp r1, #31
	bne .L_080e5afc
	movs r0, #47
	bl Runtime_ReleaseHeapBlock
	movs r0, #46
	bl Runtime_ReleaseHeapBlock
	ldr r3, [sp, #84]
	subs r3, #4
	cmp r3, #19
	bhi .L_080e5ab6
	ldr r2, [sp, #60]
	ldr r5, [r2]
	lsrs r3, r5, #31
	movs r0, #2
	adds r5, r5, r3
	movs r3, #48
	str r0, [sp, #0]
	mov r9, r3
	mov r10, r0
	movs r1, #7
	movs r2, #7
	movs r3, #3
	movs r0, #47
	bl Unnamed_080ed408
	mov r3, r9
	movs r1, #24
	ldr r6, .L_080e5af0
	str r1, [sp, #0]
	str r3, [sp, #4]
	asrs r5, r5, #1
	adds r6, #188
	adds r2, r5, #0
	ldr r4, [r6]
	ldr r0, [sp, #88]
	subs r2, #24
	mov r8, r1
	ldr r1, .L_080e5ae4
	bl _call_via_r4
	movs r0, #47
	bl Runtime_ReleaseHeapBlock
	mov r0, r10
	str r0, [sp, #0]
	movs r1, #7
	movs r2, #7
	movs r3, #7
	movs r0, #47
	bl Unnamed_080ed408
	mov r1, r8
	mov r2, r9
	str r1, [sp, #0]
	str r2, [sp, #4]
	ldr r0, [sp, #88]
	ldr r4, [r6]
	ldr r1, .L_080e5ae4
	adds r2, r5, #0
	movs r3, #48
	bl _call_via_r4
	movs r0, #47
	bl Runtime_ReleaseHeapBlock
.L_080e5ab6:
	ldr r0, .L_080e5aec
	ldr r5, [sp, #92]
	adds r3, r5, r0
	ldr r3, [r3]
	ldr r1, [sp, #52]
	ldr r0, [r3, #4]
	bl BattleFx_FetchRectangleBlitters
	bl .L_080e640e
	.2byte 0x0000
.L_080e5acc:
	.4byte Data_02010c56
.L_080e5ad0:
	.4byte BattleFx_GlintCellWidths
.L_080e5ad4:
	.4byte BattleFx_GlintCellHeights
.L_080e5ad8:
	.4byte 0x0000ffff
.L_080e5adc:
	.4byte CastingImpact_GlintDrawFlags
.L_080e5ae0:
	.4byte BattleFx_GlintCellOffsets
.L_080e5ae4:
	.4byte gMapCellBuffer
.L_080e5ae8:
	.4byte Data_03001f0c
.L_080e5aec:
	.4byte 0x00007828
.L_080e5af0:
	.4byte Data_03001e50
.L_080e5af4:
	.4byte Data_020106e8
.L_080e5af8:
	.4byte 0x000077a8
.L_080e5afc:
	ldr r1, [sp, #96]
	cmp r1, #30
	bne .L_080e5bac
	ldr r2, [sp, #84]
	cmp r2, #15
	ble .L_080e5b16
	ldr r5, [sp, #84]
	ldr r2, .L_080e5b48
	ldr r1, .L_080e5b4c
	ldr r3, .L_080e5b50
	subs r2, r2, r5
	orrs r2, r1
	strh r2, [r3]
.L_080e5b16:
	ldr r0, [sp, #84]
	cmp r0, #5
	bgt .L_080e5b20
	bl .L_080e640e
.L_080e5b20:
	ldr r1, [sp, #60]
	ldr r2, [sp, #84]
	ldr r6, [r1]
	lsrs r0, r0, #31
	adds r0, r2, r0
	lsrs r3, r6, #31
	movs r1, #3
	asrs r0, r0, #1
	adds r6, r6, r3
	bl Func_080022fc
	lsls r5, r0, #2
	adds r5, r5, r0
	lsls r3, r5, #9
	asrs r6, r6, #1
	ldr r1, .L_080e5b54
	subs r6, #20
	mov r9, r3
	b .L_080e5b58
	.2byte 0x0000
.L_080e5b48:
	.4byte 0x00000020
.L_080e5b4c:
	.4byte 0x00001000
.L_080e5b50:
	.4byte 0x04000052
.L_080e5b54:
	.4byte Data_02010c56
.L_080e5b58:
	movs r0, #40
	movs r2, #32
	add r1, r9
	mov r10, r0
	str r0, [sp, #0]
	mov r8, r2
	str r2, [sp, #4]
	ldr r4, [sp, #104]
	ldr r0, [sp, #88]
	adds r2, r6, #0
	movs r3, #16
	bl _call_via_r4
	ldr r3, .L_080e5df0
	lsls r5, r5, #8
	adds r5, r5, r3
	mov r0, r10
	mov r1, r8
	str r0, [sp, #0]
	str r1, [sp, #4]
	ldr r4, [sp, #104]
	adds r1, r5, #0
	adds r2, r6, #0
	movs r3, #48
	ldr r0, [sp, #88]
	bl _call_via_r4
	ldr r2, .L_080e5df4
	mov r3, r10
	add r9, r2
	mov r5, r8
	str r3, [sp, #0]
	str r5, [sp, #4]
	ldr r4, [sp, #104]
	ldr r0, [sp, #88]
	mov r1, r9
	adds r2, r6, #0
	movs r3, #80
	bl _call_via_r4
	bl .L_080e640e
.L_080e5bac:
	ldr r0, [sp, #96]
	cmp r0, #5
	beq .L_080e5bb6
	cmp r0, #23
	bne .L_080e5c32
.L_080e5bb6:
	movs r2, #112
	movs r1, #0
	add r2, sp
	ldr r7, .L_080e5df8
	mov r10, r1
	mov r8, r2
.L_080e5bc2:
	mov r5, r10
	lsrs r3, r5, #31
	add r3, r10
	asrs r3, r3, #1
	ldr r0, [sp, #84]
	adds r3, #4
	cmp r0, r3
	blt .L_080e5c22
	ldr r3, [r7, #24]
	cmp r3, #11
	bgt .L_080e5c22
	mov r6, r8
	lsrs r5, r3, #31
	adds r1, r6, #0
	adds r0, r7, #0
	adds r5, r3, r5
	bl EffectPosition_ApplyBaseAndYOffset
	ldr r2, [r6]
	ldr r1, .L_080e5dfc
	lsrs r3, r2, #31
	asrs r5, r5, #1
	adds r2, r2, r3
	lsls r5, r5, #11
	ldr r3, [r6, #4]
	asrs r2, r2, #1
	adds r5, r5, r1
	movs r1, #32
	str r2, [r6]
	str r1, [sp, #0]
	movs r1, #64
	subs r3, #32
	str r1, [sp, #4]
	subs r2, #16
	ldr r4, [sp, #104]
	ldr r0, [sp, #88]
	adds r1, r5, #0
	bl _call_via_r4
	movs r2, #128
	adds r0, r7, #0
	movs r1, #60
	lsls r2, r2, #5
	bl EffectStep_AdvanceWithGravity3D
	ldr r3, [r7, #24]
	adds r3, #1
	str r3, [r7, #24]
.L_080e5c22:
	movs r2, #1
	add r10, r2
	mov r3, r10
	adds r7, #28
	cmp r3, #16
	bne .L_080e5bc2
	bl .L_080e640e
.L_080e5c32:
	ldr r5, [sp, #96]
	cmp r5, #4
	bne .L_080e5c3a
	b .L_080e640e
.L_080e5c3a:
	cmp r5, #11
	beq .L_080e5c40
	b .L_080e5e28
.L_080e5c40:
	ldr r0, [sp, #84]
	lsls r5, r0, #9
	adds r0, r5, #0
	bl Trig_Sin
	adds r0, r5, #0
	bl Func_0800231c
	ldr r2, [sp, #60]
	lsls r0, r0, #2
	movs r1, #6
	ldrsh r3, [r2, r1]
	asrs r0, r0, #16
	adds r3, r3, r0
	adds r5, r3, #0
	ldr r3, [sp, #84]
	adds r5, #16
	cmp r3, #3
	bgt .L_080e5c92
	ldr r0, [sp, #92]
	ldr r2, .L_080e5e00
	adds r3, r0, r2
	ldr r3, [r3]
	ldr r2, [r3, #4]
	ldr r1, .L_080e5e04
	lsls r3, r2, #3
	subs r3, r3, r2
	ldrb r2, [r1, r3]
	ldr r3, .L_080e5e08
	movs r1, #57
	ldrb r3, [r3]
	str r1, [sp, #0]
	movs r1, #98
	str r1, [sp, #4]
	adds r3, r5, r3
	ldr r4, [sp, #104]
	ldr r0, [sp, #88]
	ldr r1, [sp, #92]
	bl _call_via_r4
	b .L_080e640e
.L_080e5c92:
	ldr r3, [sp, #84]
	cmp r3, #7
	bgt .L_080e5cc2
	ldr r0, [sp, #92]
	ldr r2, .L_080e5e00
	adds r3, r0, r2
	ldr r3, [r3]
	ldr r2, [r3, #4]
	ldr r1, .L_080e5e04
	lsls r3, r2, #3
	subs r3, r3, r2
	ldrb r2, [r1, r3]
	ldr r3, .L_080e5e08
	movs r1, #57
	ldrb r3, [r3]
	str r1, [sp, #0]
	movs r1, #98
	str r1, [sp, #4]
	adds r3, r5, r3
	ldr r4, [sp, #104]
	ldr r0, [sp, #88]
	ldr r1, [sp, #92]
	bl _call_via_r4
.L_080e5cc2:
	ldr r3, [sp, #92]
	ldr r0, .L_080e5e0c
	adds r1, r3, r0
	ldr r0, .L_080e5e00
	adds r6, r3, r0
	ldr r3, [r6]
	ldr r2, .L_080e5e04
	mov r8, r2
	ldr r2, [r3, #4]
	lsls r3, r2, #3
	subs r3, r3, r2
	ldr r7, .L_080e5e08
	adds r3, #1
	mov r0, r8
	ldrb r2, [r0, r3]
	ldrb r3, [r7, #1]
	movs r0, #99
	str r0, [sp, #0]
	movs r0, #69
	adds r3, r5, r3
	str r0, [sp, #4]
	ldr r4, [sp, #104]
	ldr r0, [sp, #88]
	bl _call_via_r4
	ldr r3, [sp, #84]
	subs r3, #4
	cmp r3, #1
	bhi .L_080e5d0a
	movs r1, #128
	ldr r3, .L_080e5e10
	ldr r0, [sp, #88]
	lsls r1, r1, #7
	ldr r2, .L_080e5e14
	bl _call_via_r3
.L_080e5d0a:
	ldr r3, [sp, #84]
	subs r3, #6
	cmp r3, #1
	bhi .L_080e5d3a
	ldr r2, [sp, #92]
	ldr r3, .L_080e5e18
	adds r1, r2, r3
	ldr r3, [r6]
	ldr r2, [r3, #4]
	lsls r3, r2, #3
	subs r3, r3, r2
	adds r3, #2
	mov r0, r8
	ldrb r2, [r0, r3]
	ldrb r3, [r7, #2]
	movs r0, #128
	str r0, [sp, #0]
	movs r0, #91
	str r0, [sp, #4]
	adds r3, r5, r3
	ldr r4, [sp, #104]
	ldr r0, [sp, #88]
	bl _call_via_r4
.L_080e5d3a:
	ldr r3, [sp, #84]
	subs r3, #8
	cmp r3, #1
	bhi .L_080e5d66
	ldr r3, [r6]
	ldr r2, [r3, #4]
	lsls r3, r2, #3
	subs r3, r3, r2
	adds r3, #3
	mov r1, r8
	ldrb r2, [r1, r3]
	ldrb r3, [r7, #3]
	movs r1, #128
	str r1, [sp, #0]
	movs r1, #91
	str r1, [sp, #4]
	adds r3, r5, r3
	ldr r4, [sp, #104]
	ldr r0, [sp, #88]
	ldr r1, .L_080e5dfc
	bl _call_via_r4
.L_080e5d66:
	ldr r3, [sp, #84]
	subs r3, #10
	cmp r3, #1
	bhi .L_080e5d92
	ldr r3, [r6]
	ldr r2, [r3, #4]
	lsls r3, r2, #3
	subs r3, r3, r2
	adds r3, #4
	mov r0, r8
	ldrb r2, [r0, r3]
	ldrb r3, [r7, #4]
	movs r0, #128
	str r0, [sp, #0]
	movs r0, #59
	str r0, [sp, #4]
	ldr r1, .L_080e5e1c
	adds r3, r5, r3
	ldr r4, [sp, #104]
	ldr r0, [sp, #88]
	bl _call_via_r4
.L_080e5d92:
	ldr r3, [sp, #84]
	subs r3, #12
	cmp r3, #1
	bhi .L_080e5dbe
	ldr r3, [r6]
	ldr r2, [r3, #4]
	lsls r3, r2, #3
	subs r3, r3, r2
	adds r3, #5
	mov r0, r8
	ldrb r2, [r0, r3]
	ldrb r3, [r7, #5]
	movs r0, #122
	str r0, [sp, #0]
	movs r0, #29
	str r0, [sp, #4]
	ldr r1, .L_080e5e20
	adds r3, r5, r3
	ldr r4, [sp, #104]
	ldr r0, [sp, #88]
	bl _call_via_r4
.L_080e5dbe:
	ldr r3, [sp, #84]
	subs r3, #14
	cmp r3, #1
	bls .L_080e5dc8
	b .L_080e640e
.L_080e5dc8:
	ldr r3, [r6]
	ldr r2, [r3, #4]
	lsls r3, r2, #3
	subs r3, r3, r2
	adds r3, #6
	mov r0, r8
	ldrb r2, [r0, r3]
	ldrb r3, [r7, #6]
	movs r0, #76
	str r0, [sp, #0]
	movs r0, #25
	str r0, [sp, #4]
	ldr r1, .L_080e5e24
	adds r3, r5, r3
	ldr r4, [sp, #104]
	ldr r0, [sp, #88]
	bl _call_via_r4
	b .L_080e640e
	.2byte 0x0000
.L_080e5df0:
	.4byte Data_02012a56
.L_080e5df4:
	.4byte Data_02011156
.L_080e5df8:
	.4byte Data_02014000
.L_080e5dfc:
	.4byte gMapCellBuffer
.L_080e5e00:
	.4byte 0x00007828
.L_080e5e04:
	.4byte CastingImpact_ImageX
.L_080e5e08:
	.4byte CastingImpact_ImageY
.L_080e5e0c:
	.4byte 0x000015d2
.L_080e5e10:
	.4byte IwramFillWords
.L_080e5e14:
	.4byte 0x3f3f3f3f
.L_080e5e18:
	.4byte 0x00003081
.L_080e5e1c:
	.4byte Data_02012d80
.L_080e5e20:
	.4byte Data_02014b00
.L_080e5e24:
	.4byte Data_020158d2
.L_080e5e28:
	ldr r1, [sp, #96]
	cmp r1, #32
	beq .L_080e5e30
	b .L_080e600a
.L_080e5e30:
	ldr r2, [sp, #72]
	ldr r3, [sp, #68]
	ldr r5, [sp, #84]
	adds r2, r2, r3
	str r2, [sp, #72]
	cmp r5, #6
	ble .L_080e5e50
	ldr r0, [sp, #68]
	lsls r3, r3, #1
	adds r3, r3, r0
	lsls r3, r3, #4
	cmp r3, #0
	bge .L_080e5e4c
	adds r3, #63
.L_080e5e4c:
	asrs r3, r3, #6
	str r3, [sp, #68]
.L_080e5e50:
	ldr r1, [sp, #72]
	ldr r2, .L_080e5ea0
	asrs r3, r1, #16
	lsls r3, r3, #8
	str r3, [r2]
	ldr r1, [sp, #84]
	subs r1, #16
	cmp r1, #15
	bhi .L_080e5e6e
	ldr r2, .L_080e5e98
	subs r2, r2, r1
	ldr r1, .L_080e5e9c
	ldr r3, .L_080e5ea4
	orrs r2, r1
	strh r2, [r3]
.L_080e5e6e:
	ldr r3, [sp, #84]
	subs r3, #4
	cmp r3, #1
	bhi .L_080e5e84
	movs r1, #128
	ldr r3, .L_080e5ea8
	ldr r0, [sp, #88]
	lsls r1, r1, #7
	ldr r2, .L_080e5eac
	bl _call_via_r3
.L_080e5e84:
	ldr r2, [sp, #84]
	cmp r2, #3
	bgt .L_080e5ee2
	ldr r5, [sp, #92]
	ldr r0, .L_080e5eb0
	adds r3, r5, r0
	ldr r3, [r3]
	ldr r3, [r3, #4]
	b .L_080e5eb4
	.2byte 0x0000
.L_080e5e98:
	.4byte 0x00000010
.L_080e5e9c:
	.4byte 0x00001000
.L_080e5ea0:
	.4byte 0x04000028
.L_080e5ea4:
	.4byte 0x04000052
.L_080e5ea8:
	.4byte IwramFillWords
.L_080e5eac:
	.4byte 0x3f3f3f3f
.L_080e5eb0:
	.4byte 0x00007828
.L_080e5eb4:
	cmp r3, #1
	bne .L_080e5eca
	movs r3, #80
	str r3, [sp, #0]
	movs r3, #104
	str r3, [sp, #4]
	ldr r4, [sp, #104]
	ldr r0, [sp, #88]
	adds r1, r5, #0
	movs r2, #0
	b .L_080e5eda
.L_080e5eca:
	movs r3, #80
	str r3, [sp, #0]
	movs r3, #104
	str r3, [sp, #4]
	ldr r4, [sp, #104]
	ldr r0, [sp, #88]
	ldr r1, [sp, #92]
	movs r2, #48
.L_080e5eda:
	movs r3, #24
	bl _call_via_r4
	b .L_080e640e
.L_080e5ee2:
	ldr r1, [sp, #84]
	cmp r1, #7
	bgt .L_080e5f24
	ldr r2, [sp, #92]
	ldr r5, .L_080e6254
	adds r3, r2, r5
	ldr r3, [r3]
	ldr r3, [r3, #4]
	cmp r3, #1
	bne .L_080e5f0e
	movs r3, #80
	str r3, [sp, #0]
	movs r3, #104
	str r3, [sp, #4]
	adds r1, r2, #0
	ldr r4, [sp, #104]
	ldr r0, [sp, #88]
	movs r2, #0
	movs r3, #24
	bl _call_via_r4
	b .L_080e5f24
.L_080e5f0e:
	movs r3, #80
	str r3, [sp, #0]
	movs r3, #104
	str r3, [sp, #4]
	ldr r4, [sp, #104]
	ldr r0, [sp, #88]
	ldr r1, [sp, #92]
	movs r2, #48
	movs r3, #24
	bl _call_via_r4
.L_080e5f24:
	ldr r0, [sp, #92]
	ldr r1, .L_080e6254
	adds r3, r0, r1
	ldr r3, [r3]
	ldr r3, [r3, #4]
	cmp r3, #1
	bne .L_080e5f4e
	movs r3, #80
	movs r2, #240
	lsls r2, r2, #5
	str r3, [sp, #0]
	movs r3, #104
	adds r1, r0, r2
	str r3, [sp, #4]
	ldr r4, [sp, #104]
	ldr r0, [sp, #88]
	movs r2, #16
	movs r3, #16
	bl _call_via_r4
	b .L_080e5f6a
.L_080e5f4e:
	ldr r3, [sp, #92]
	movs r5, #240
	lsls r5, r5, #5
	adds r1, r3, r5
	movs r3, #80
	str r3, [sp, #0]
	movs r3, #104
	str r3, [sp, #4]
	ldr r4, [sp, #104]
	ldr r0, [sp, #88]
	movs r2, #32
	movs r3, #16
	bl _call_via_r4
.L_080e5f6a:
	ldr r3, [sp, #84]
	subs r3, #6
	cmp r3, #1
	bhi .L_080e5f8e
	ldr r0, [sp, #92]
	movs r3, #128
	movs r2, #250
	lsls r2, r2, #6
	str r3, [sp, #0]
	movs r3, #91
	adds r1, r0, r2
	str r3, [sp, #4]
	ldr r4, [sp, #104]
	ldr r0, [sp, #88]
	movs r2, #0
	movs r3, #16
	bl _call_via_r4
.L_080e5f8e:
	ldr r3, [sp, #84]
	subs r3, #8
	cmp r3, #1
	bhi .L_080e5fac
	movs r3, #128
	str r3, [sp, #0]
	movs r3, #91
	str r3, [sp, #4]
	ldr r4, [sp, #104]
	ldr r0, [sp, #88]
	ldr r1, .L_080e6258
	movs r2, #0
	movs r3, #16
	bl _call_via_r4
.L_080e5fac:
	ldr r3, [sp, #84]
	subs r3, #10
	cmp r3, #1
	bhi .L_080e5fca
	movs r3, #128
	str r3, [sp, #0]
	movs r3, #59
	str r3, [sp, #4]
	ldr r1, .L_080e625c
	ldr r4, [sp, #104]
	ldr r0, [sp, #88]
	movs r2, #0
	movs r3, #16
	bl _call_via_r4
.L_080e5fca:
	ldr r3, [sp, #84]
	subs r3, #12
	cmp r3, #1
	bhi .L_080e5fe8
	movs r3, #128
	str r3, [sp, #0]
	movs r3, #29
	str r3, [sp, #4]
	ldr r1, .L_080e6260
	ldr r4, [sp, #104]
	ldr r0, [sp, #88]
	movs r2, #0
	movs r3, #16
	bl _call_via_r4
.L_080e5fe8:
	ldr r3, [sp, #84]
	subs r3, #14
	cmp r3, #1
	bls .L_080e5ff2
	b .L_080e640e
.L_080e5ff2:
	movs r3, #128
	str r3, [sp, #0]
	movs r3, #26
	str r3, [sp, #4]
	ldr r1, .L_080e6264
	ldr r4, [sp, #104]
	ldr r0, [sp, #88]
	movs r2, #0
	movs r3, #16
	bl _call_via_r4
	b .L_080e640e
.L_080e600a:
	ldr r3, [sp, #96]
	cmp r3, #20
	bne .L_080e60ba
	movs r5, #0
	mov r10, r5
.L_080e6014:
	mov r3, r10
	ldr r0, [sp, #84]
	mov r5, r10
	adds r3, #6
	adds r5, #1
	cmp r0, r3
	blt .L_080e60b2
	adds r3, #12
	cmp r0, r3
	bge .L_080e60ae
	mov r1, r10
	subs r3, r0, r1
	subs r3, #6
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r4, r3, #1
	ldr r3, [sp, #60]
	ldr r2, [r3]
	ldr r7, .L_080e6268
	lsrs r3, r2, #31
	adds r2, r2, r3
	ldrb r3, [r7, r4]
	asrs r2, r2, #1
	lsrs r3, r3, #1
	subs r6, r2, r3
	movs r3, #1
	ands r3, r1
	cmp r3, #0
	beq .L_080e605c
	lsrs r3, r5, #31
	adds r3, r5, r3
	asrs r3, r3, #1
	lsls r2, r3, #1
	adds r2, r2, r3
	adds r6, r6, r2
	b .L_080e606c
.L_080e605c:
	mov r5, r10
	adds r5, #1
	lsrs r3, r5, #31
	adds r3, r5, r3
	asrs r3, r3, #1
	lsls r2, r3, #1
	adds r2, r2, r3
	subs r6, r6, r2
.L_080e606c:
	mov r1, r10
	movs r0, #1
	cmp r1, #0
	beq .L_080e6084
	mov r3, r10
	subs r3, #1
	movs r2, #3
	ands r3, r2
	movs r0, #0
	cmp r3, #1
	ble .L_080e6084
	movs r0, #1
.L_080e6084:
	ldr r2, .L_080e626c
	lsls r3, r4, #1
	ldrh r1, [r2, r3]
	ldr r2, .L_080e6258
	ldr r3, .L_080e6270
	adds r1, r1, r2
	ldrb r2, [r7, r4]
	ldrb r3, [r3, r4]
	str r2, [sp, #0]
	ldr r2, .L_080e6274
	ldrb r2, [r2, r4]
	str r2, [sp, #4]
	ldr r2, [sp, #52]
	lsls r0, r0, #2
	ldr r4, [r0, r2]
	adds r3, #48
	ldr r0, [sp, #88]
	adds r2, r6, #0
	bl _call_via_r4
	b .L_080e60b2
.L_080e60ae:
	mov r5, r10
	adds r5, #1
.L_080e60b2:
	mov r10, r5
	cmp r5, #12
	bne .L_080e6014
	b .L_080e640e
.L_080e60ba:
	ldr r3, [sp, #96]
	cmp r3, #16
	beq .L_080e60c2
	b .L_080e61d4
.L_080e60c2:
	ldr r5, [sp, #84]
	cmp r5, #0
	bne .L_080e610c
	movs r0, #0
	ldr r6, .L_080e6278
	ldr r5, .L_080e627c
	mov r10, r0
	movs r7, #0
.L_080e60d2:
	bl Random16
	movs r3, #127
	ands r3, r0
	adds r3, #32
	str r3, [r5]
	str r7, [r5, #4]
	str r7, [r5, #8]
	bl Random16
	ands r0, r6
	str r0, [r5, #12]
	bl Random16
	ands r0, r6
	str r0, [r5, #16]
	bl Random16
	movs r1, #1
	add r10, r1
	ands r0, r6
	mov r2, r10
	str r0, [r5, #20]
	adds r5, #28
	cmp r2, #64
	bne .L_080e60d2
	ldr r2, .L_080e6280
	movs r3, #159
	str r3, [r2, #4]
.L_080e610c:
	movs r5, #112
	ldr r0, [sp, #60]
	movs r3, #0
	add r5, sp
	ldr r7, .L_080e627c
	mov r10, r3
	mov r9, r5
	mov r8, r0
.L_080e611c:
	ldr r3, [r7]
	cmp r3, #0
	blt .L_080e61c6
	mov r1, r10
	lsrs r3, r1, #31
	add r3, r10
	ldr r2, [sp, #84]
	asrs r3, r3, #1
	cmp r2, r3
	blt .L_080e61c6
	movs r5, #3
	ands r5, r1
	bl Render_ResetTransformState
	ldr r0, [r7, #12]
	bl SceneTransform_ApplyPitch
	mov r6, r9
	ldr r0, [r7, #16]
	bl SceneTransform_ApplyYaw
	adds r1, r6, #0
	adds r0, r7, #0
	bl EffectPosition_ApplyBaseAndYOffset
	ldr r2, [r6]
	mov r0, r8
	lsrs r3, r2, #31
	adds r2, r2, r3
	ldr r3, [r0]
	lsrs r1, r3, #31
	adds r3, r3, r1
	asrs r3, r3, #1
	asrs r2, r2, #1
	adds r2, r2, r3
	str r2, [r6]
	ldr r3, [r6, #4]
	ldr r1, [r0, #4]
	adds r3, r3, r1
	adds r1, r3, #0
	adds r1, #32
	str r1, [r6, #4]
	ldr r1, .L_080e6284
	movs r0, #8
	lsls r5, r5, #1
	ldrh r1, [r1, r5]
	str r0, [sp, #0]
	str r0, [sp, #4]
	ldr r5, .L_080e6258
	ldr r0, [sp, #52]
	adds r3, #28
	ldr r4, [r0, #4]
	adds r1, r1, r5
	subs r2, #4
	ldr r0, [sp, #88]
	bl _call_via_r4
	ldr r3, [r7]
	subs r3, #6
	str r3, [r7]
	cmp r3, #0
	bge .L_080e61c6
	movs r3, #7
	mov r1, r10
	ands r3, r1
	cmp r3, #0
	beq .L_080e61a6
	cmp r1, #63
	bne .L_080e61c6
.L_080e61a6:
	movs r0, #133
	bl Func_080f9010
	ldr r5, .L_080e6254
	ldr r2, [sp, #92]
	adds r3, r2, r5
	ldr r3, [r3]
	movs r1, #36
	ldrsh r0, [r3, r1]
	movs r3, #4
	str r3, [sp, #0]
	movs r1, #7
	movs r2, #5
	movs r3, #0
	bl ObjectGroup_UpdateMembers
.L_080e61c6:
	movs r2, #1
	add r10, r2
	mov r3, r10
	adds r7, #28
	cmp r3, #64
	bne .L_080e611c
	b .L_080e640e
.L_080e61d4:
	ldr r5, [sp, #96]
	cmp r5, #8
	bne .L_080e6218
	ldr r3, [sp, #84]
	subs r3, #5
	cmp r3, #44
	bls .L_080e61e4
	b .L_080e640e
.L_080e61e4:
	ldr r0, [sp, #84]
	cmp r0, #25
	ble .L_080e61f2
	lsls r2, r0, #2
	movs r3, #196
	subs r1, r3, r2
	b .L_080e61fa
.L_080e61f2:
	ldr r1, [sp, #84]
	lsls r3, r1, #4
	adds r1, r3, #0
	subs r1, #64
.L_080e61fa:
	cmp r1, #96
	ble .L_080e6200
	movs r1, #96
.L_080e6200:
	movs r2, #32
	movs r3, #104
	subs r3, r3, r1
	str r2, [sp, #0]
	str r1, [sp, #4]
	ldr r4, [sp, #104]
	ldr r0, [sp, #88]
	ldr r1, .L_080e6258
	movs r2, #48
	bl _call_via_r4
	b .L_080e640e
.L_080e6218:
	ldr r3, [sp, #96]
	subs r3, #33
	cmp r3, #1
	bhi .L_080e62cc
	ldr r2, [sp, #84]
	cmp r2, #5
	ble .L_080e6228
	b .L_080e640e
.L_080e6228:
	ldr r5, [sp, #92]
	ldr r0, .L_080e6254
	adds r3, r5, r0
	ldr r3, [r3]
	ldr r3, [r3, #4]
	cmp r3, #0
	bne .L_080e6288
	ldr r2, [sp, #60]
	ldr r1, [r2]
	ldr r5, [sp, #84]
	lsrs r3, r1, #31
	adds r1, r1, r3
	movs r3, #6
	subs r3, r3, r5
	lsls r2, r3, #1
	adds r2, r2, r3
	ldr r0, [sp, #60]
	lsls r3, r2, #1
	asrs r1, r1, #1
	adds r1, r1, r3
	b .L_080e62a2
	.2byte 0x0000
.L_080e6254:
	.4byte 0x00007828
.L_080e6258:
	.4byte gMapCellBuffer
.L_080e625c:
	.4byte Data_02012d80
.L_080e6260:
	.4byte Data_02014b00
.L_080e6264:
	.4byte Data_02015980
.L_080e6268:
	.4byte PuffArc_CellWidths
.L_080e626c:
	.4byte PuffArc_CellSourceOffsets
.L_080e6270:
	.4byte PuffArc_CellBiasY
.L_080e6274:
	.4byte PuffArc_CellHeights
.L_080e6278:
	.4byte 0x0000ffff
.L_080e627c:
	.4byte Data_02014000
.L_080e6280:
	.4byte Data_020146e4
.L_080e6284:
	.4byte CastingImpact_OrbitCells
.L_080e6288:
	ldr r2, [sp, #60]
	ldr r1, [r2]
	ldr r5, [sp, #84]
	lsrs r3, r1, #31
	adds r1, r1, r3
	movs r3, #6
	subs r3, r3, r5
	lsls r2, r3, #1
	adds r2, r2, r3
	ldr r0, [sp, #60]
	lsls r3, r2, #1
	asrs r1, r1, #1
	subs r1, r1, r3
.L_080e62a2:
	ldr r3, [r0, #4]
	lsls r2, r2, #2
	subs r3, r3, r2
	adds r3, #24
	adds r2, r1, #0
	movs r1, #32
	str r1, [sp, #0]
	movs r1, #64
	str r1, [sp, #4]
	ldr r1, [sp, #52]
	subs r2, #16
	ldr r4, [r1, #4]
	subs r3, #32
	ldr r0, [sp, #88]
	ldr r1, .L_080e62c8
	bl _call_via_r4
	b .L_080e640e
	.2byte 0x0000
.L_080e62c8:
	.4byte gMapCellBuffer
.L_080e62cc:
	ldr r2, [sp, #96]
	cmp r2, #12
	bne .L_080e6374
	ldr r3, [sp, #84]
	cmp r3, #47
	ble .L_080e62e6
	ldr r5, [sp, #84]
	ldr r2, .L_080e62f8
	ldr r1, .L_080e62fc
	ldr r3, .L_080e6300
	subs r2, r2, r5
	orrs r2, r1
	strh r2, [r3]
.L_080e62e6:
	movs r0, #0
	movs r1, #3
	movs r2, #1
	ldr r7, .L_080e6304
	mov r10, r0
	mov r9, r1
	add r6, sp, #112
	mov r8, r2
	b .L_080e6308
.L_080e62f8:
	.4byte 0x00000040
.L_080e62fc:
	.4byte 0x00001000
.L_080e6300:
	.4byte 0x04000052
.L_080e6304:
	.4byte Data_02014000
.L_080e6308:
	mov r0, r10
	movs r1, #3
	bl Func_080022fc
	adds r1, r6, #0
	adds r5, r0, #0
	adds r0, r7, #0
	bl EffectPosition_ApplyBaseAndYOffset
	ldr r2, [r6]
	lsrs r3, r2, #31
	adds r2, r2, r3
	asrs r2, r2, #1
	str r2, [r6]
	movs r0, #24
	mov r4, r10
	mov r3, r8
	lsls r1, r5, #3
	ands r4, r3
	adds r1, r1, r5
	ldr r3, [r6, #4]
	ldr r5, .L_080e660c
	str r0, [sp, #0]
	str r0, [sp, #4]
	ldr r0, [sp, #52]
	lsls r4, r4, #2
	lsls r1, r1, #6
	ldr r4, [r4, r0]
	adds r1, r1, r5
	subs r2, #12
	subs r3, #12
	ldr r0, [sp, #88]
	bl _call_via_r4
	mov r3, r10
	mov r1, r9
	ands r3, r1
	adds r3, #11
	mov r2, r8
	lsls r2, r3
	adds r0, r7, #0
	movs r1, #60
	bl EffectStep_AdvanceWithGravity3D
	ldr r3, [r7, #24]
	movs r2, #1
	adds r3, #1
	add r10, r2
	str r3, [r7, #24]
	mov r3, r10
	adds r7, #28
	cmp r3, #16
	bne .L_080e6308
	b .L_080e640e
.L_080e6374:
	ldr r5, [sp, #96]
	cmp r5, #100
	beq .L_080e640e
	movs r1, #112
	movs r0, #0
	add r1, sp
	ldr r7, .L_080e6610
	mov r10, r0
	mov r8, r1
.L_080e6386:
	mov r3, r10
	ldr r2, [sp, #84]
	adds r3, #4
	cmp r2, r3
	blt .L_080e6402
	ldr r5, [r7, #24]
	cmp r5, #23
	bgt .L_080e6402
	cmp r5, #0
	bge .L_080e639c
	adds r5, #3
.L_080e639c:
	mov r6, r8
	adds r1, r6, #0
	adds r0, r7, #0
	bl EffectPosition_ApplyBaseAndYOffset
	ldr r2, [r6]
	lsrs r3, r2, #31
	adds r2, r2, r3
	asrs r5, r5, #2
	asrs r2, r2, #1
	mov r3, r10
	movs r0, #24
	movs r4, #1
	str r2, [r6]
	ands r4, r3
	lsls r1, r5, #3
	ldr r3, [r6, #4]
	str r0, [sp, #0]
	movs r0, #48
	str r0, [sp, #4]
	adds r1, r1, r5
	ldr r0, [sp, #52]
	ldr r5, .L_080e660c
	lsls r4, r4, #2
	lsls r1, r1, #7
	adds r1, r1, r5
	ldr r4, [r4, r0]
	subs r2, #12
	subs r3, #24
	ldr r0, [sp, #88]
	bl _call_via_r4
	ldr r1, [sp, #96]
	cmp r1, #25
	bne .L_080e63f0
	movs r2, #128
	adds r0, r7, #0
	movs r1, #60
	lsls r2, r2, #3
	bl EffectStep_AdvanceWithGravity3D
	b .L_080e63fc
.L_080e63f0:
	movs r2, #128
	adds r0, r7, #0
	movs r1, #60
	lsls r2, r2, #5
	bl EffectStep_AdvanceWithGravity3D
.L_080e63fc:
	ldr r3, [r7, #24]
	adds r3, #1
	str r3, [r7, #24]
.L_080e6402:
	movs r2, #1
	add r10, r2
	mov r3, r10
	adds r7, #28
	cmp r3, #16
	bne .L_080e6386
.L_080e640e:
	ldr r5, [sp, #96]
	cmp r5, #7
	bgt .L_080e6464
	ldr r0, [sp, #84]
	cmp r0, #5
	bgt .L_080e6464
	add r5, sp, #112
	adds r1, r5, #0
	ldr r0, [sp, #8]
	bl EffectPosition_ApplyBaseAndYOffset
	ldr r2, [r5]
	lsrs r3, r2, #31
	adds r2, r2, r3
	asrs r2, r2, #1
	movs r0, #20
	str r2, [r5]
	ldr r3, [r5, #4]
	str r0, [sp, #0]
	movs r0, #40
	str r0, [sp, #4]
	ldr r5, [sp, #52]
	ldr r1, .L_080e6614
	subs r2, #10
	subs r3, #4
	ldr r4, [r5, #4]
	ldr r0, [sp, #88]
	bl _call_via_r4
	ldr r0, [sp, #8]
	ldr r1, [sp, #24]
	ldr r3, [r0]
	ldr r2, [r1]
	adds r3, r3, r2
	str r3, [r0]
	ldr r3, [r0, #4]
	ldr r2, [r1, #4]
	adds r3, r3, r2
	str r3, [r0, #4]
	ldr r3, [r0, #8]
	ldr r2, [r1, #8]
	adds r3, r3, r2
	str r3, [r0, #8]
.L_080e6464:
	ldr r2, [sp, #84]
	cmp r2, #3
	bne .L_080e6472
	movs r0, #1
	negs r0, r0
	bl BattleEventRuntime_BeginPhaseFar
.L_080e6472:
	ldr r3, [sp, #84]
	cmp r3, #4
	bne .L_080e647e
	movs r0, #134
	bl Func_080f9010
.L_080e647e:
	ldr r5, [sp, #84]
	cmp r5, #6
	bne .L_080e6530
	ldr r0, [sp, #16]
	cmp r0, #1
	bls .L_080e64a8
	ldr r1, [sp, #96]
	cmp r1, #7
	beq .L_080e64a8
	cmp r1, #13
	beq .L_080e64a8
	cmp r1, #18
	beq .L_080e64a8
	cmp r1, #19
	beq .L_080e64a8
	cmp r1, #23
	beq .L_080e64a8
	cmp r1, #34
	beq .L_080e64a8
	cmp r1, #100
	bne .L_080e64c2
.L_080e64a8:
	ldr r2, [sp, #92]
	ldr r5, .L_080e6618
	adds r3, r2, r5
	ldr r3, [r3]
	movs r1, #36
	ldrsh r0, [r3, r1]
	movs r1, #4
	bl BattleMotion_ApplyVariantMotionFar
	ldr r3, [sp, #92]
	subs r5, #128
	adds r2, r3, r5
	b .L_080e650c
.L_080e64c2:
	ldr r0, [sp, #96]
	cmp r0, #20
	beq .L_080e64d0
	cmp r0, #14
	beq .L_080e64d0
	cmp r0, #33
	bne .L_080e64ec
.L_080e64d0:
	ldr r1, [sp, #92]
	ldr r2, .L_080e6618
	adds r3, r1, r2
	ldr r3, [r3]
	movs r1, #1
	movs r5, #36
	ldrsh r0, [r3, r5]
	bl BattleMotion_ApplyVariantMotionFar
	ldr r1, .L_080e661c
	ldr r0, [sp, #92]
	movs r3, #2
	adds r2, r0, r1
	b .L_080e650e
.L_080e64ec:
	ldr r2, [sp, #96]
	cmp r2, #30
	beq .L_080e64f6
	cmp r2, #8
	bne .L_080e6510
.L_080e64f6:
	ldr r5, [sp, #92]
	ldr r0, .L_080e6618
	adds r3, r5, r0
	ldr r3, [r3]
	movs r1, #36
	ldrsh r0, [r3, r1]
	movs r1, #3
	bl BattleMotion_ApplyVariantMotionFar
	ldr r3, .L_080e661c
	adds r2, r5, r3
.L_080e650c:
	movs r3, #8
.L_080e650e:
	str r3, [r2]
.L_080e6510:
	ldr r5, [sp, #84]
	cmp r5, #6
	bne .L_080e6530
	ldr r0, [sp, #92]
	ldr r1, .L_080e6618
	adds r3, r0, r1
	ldr r3, [r3]
	movs r2, #36
	ldrsh r0, [r3, r2]
	movs r3, #4
	str r3, [sp, #0]
	movs r1, #7
	movs r2, #5
	movs r3, #0
	bl ObjectGroup_UpdateMembers
.L_080e6530:
	ldr r3, [sp, #84]
	cmp r3, #14
	bne .L_080e6550
	ldr r0, .L_080e6618
	ldr r5, [sp, #92]
	adds r3, r5, r0
	ldr r3, [r3]
	movs r1, #36
	ldrsh r0, [r3, r1]
	movs r3, #4
	str r3, [sp, #0]
	movs r1, #7
	movs r2, #5
	movs r3, #0
	bl ObjectGroup_UpdateMembers
.L_080e6550:
	movs r1, #8
	movs r0, #8
	bl Camera_ApplyShake
	bl ObjectGroup_TickMemberTimers
	ldr r5, .L_080e6620
	ldr r3, [sp, #92]
	adds r2, r3, r5
	movs r3, #1
	movs r0, #1
	str r3, [r2]
	bl WaitFrames
	ldr r0, [sp, #84]
	ldr r1, [sp, #64]
	adds r0, #1
	str r0, [sp, #84]
	cmp r0, r1
	beq .L_080e657c
	bl .L_080e52f8
.L_080e657c:
	ldr r2, [sp, #96]
	cmp r2, #21
	bne .L_080e65c4
	movs r1, #128
	ldr r5, .L_080e6624
	lsls r1, r1, #7
	ldr r0, .L_080e6628
	bl _call_via_r5
	movs r1, #128
	ldr r0, [sp, #88]
	lsls r1, r1, #7
	bl _call_via_r5
	ldr r0, .L_080e6618
	ldr r5, [sp, #92]
	adds r3, r5, r0
	ldr r2, [r3]
	movs r3, #0
	str r3, [r2, #28]
	ldr r0, .L_080e662c
	bl Scheduler_RemoveCallback
	ldr r0, .L_080e6630
	bl Scheduler_RemoveCallback
	movs r0, #47
	bl Runtime_ReleaseHeapBlock
	movs r0, #46
	bl Runtime_ReleaseHeapBlock
	ldr r0, [sp, #100]
	bl BattleFx_RunPaletteRampMode1
	b .L_080e65f8
.L_080e65c4:
	ldr r1, [sp, #20]
	cmp r1, #1
	bls .L_080e65dc
	ldr r2, [sp, #96]
	cmp r2, #12
	beq .L_080e65dc
	cmp r2, #22
	beq .L_080e65dc
	cmp r2, #28
	beq .L_080e65dc
	cmp r2, #29
	bne .L_080e65e2
.L_080e65dc:
	ldr r0, .L_080e6634
	bl Scheduler_RemoveCallback
.L_080e65e2:
	ldr r0, .L_080e6630
	bl Scheduler_RemoveCallback
	movs r0, #47
	bl Runtime_ReleaseHeapBlock
	movs r0, #46
	bl Runtime_ReleaseHeapBlock
	bl BattleFx_EndCanvasLayer
.L_080e65f8:
	add sp, #184
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
.L_080e660c:
	.4byte gMapCellBuffer
.L_080e6610:
	.4byte Data_02014000
.L_080e6614:
	.4byte Data_02013c56
.L_080e6618:
	.4byte 0x00007828
.L_080e661c:
	.4byte 0x000077a8
.L_080e6620:
	.4byte 0x00007824
.L_080e6624:
	.4byte IwramClearWords
.L_080e6628:
	.4byte 0x06004000
.L_080e662c:
	.4byte Palette_StepFadeTransfer
.L_080e6630:
	.4byte BattlePresentation_ProcessPendingGraphicsTransfer
.L_080e6634:
	.4byte BattleFx_ArmBg2AffineHBlankDma
