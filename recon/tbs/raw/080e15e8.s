.syntax unified
	.thumb
	.global BattleFx_InitializeMode12
	.thumb_func
BattleFx_InitializeMode12:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, .L_080e1664
	ldr r1, [r3]
	sub sp, #336
	str r1, [sp, #132]
	adds r2, r3, #0
	subs r2, #20
	ldr r2, [r2]
	str r2, [sp, #128]
	adds r2, r3, #0
	subs r2, #16
	ldr r2, [r2]
	str r2, [sp, #124]
	subs r3, #12
	ldr r2, [sp, #128]
	ldr r4, .L_080e1668
	ldr r3, [r3]
	str r3, [sp, #120]
	adds r3, r2, r4
	str r0, [r3]
	movs r0, #128
	lsls r0, r0, #6
	bl BattleFx_BeginCanvasLayer
	ldr r2, .L_080e166c
	ldr r3, .L_080e1660
	ldr r5, .L_080e1670
	strh r3, [r2]
	ldr r1, [sp, #128]
	adds r0, r5, #0
	movs r2, #1
	movs r3, #1
	bl Resource_LoadAndDecompress
	adds r0, r5, #0
	ldr r1, [sp, #128]
	movs r2, #1
	movs r3, #1
	bl Resource_LoadAndDecompress
	movs r2, #192
	ldr r5, [sp, #128]
	lsls r2, r2, #5
	adds r1, r5, r2
	ldr r0, .L_080e1674
	movs r2, #0
	movs r3, #0
	bl Resource_LoadAndDecompress
	ldr r0, .L_080e1678
	movs r3, #0
	ldr r1, [sp, #120]
	movs r2, #0
	b .L_080e167c
.L_080e1660:
	.4byte 0x00000100
.L_080e1664:
	.4byte gTransitionWork
.L_080e1668:
	.4byte 0x00007828
.L_080e166c:
	.4byte 0x04000020
.L_080e1670:
	.4byte 0x000000bc
.L_080e1674:
	.4byte 0x00000075
.L_080e1678:
	.4byte 0x00000073
.L_080e167c:
	bl Resource_LoadAndDecompress
	ldr r5, .L_080e170c
	movs r3, #0
	mov r9, r3
	movs r4, #0
	movs r0, #64
.L_080e168a:
	ldr r2, [sp, #128]
	movs r1, #0
	adds r3, r4, r2
	ldr r2, .L_080e1710
	mov r8, r1
	ldr r1, [sp, #120]
	mov r12, r0
	adds r3, r3, r2
.L_080e169a:
	ldrb r2, [r1]
	adds r1, #1
	cmp r2, r12
	ble .L_080e16a4
	mov r2, r12
.L_080e16a4:
	cmp r2, #0
	bge .L_080e16aa
	movs r2, #0
.L_080e16aa:
	strb r2, [r3]
	movs r2, #1
	add r8, r2
	adds r3, #1
	cmp r8, r5
	bne .L_080e169a
	add r9, r2
	mov r3, r9
	add r4, r8
	subs r0, #7
	cmp r3, #8
	bne .L_080e168a
	bl BattlePres_ConfigureEffectDisplay
	ldr r2, .L_080e1704
	movs r3, #160
	lsls r3, r3, #19
	strh r2, [r3]
	adds r3, #2
	strh r2, [r3]
	ldr r2, .L_080e1714
	ldr r3, .L_080e1708
	strh r3, [r2]
	ldr r4, [sp, #128]
	ldr r5, .L_080e1718
	ldr r0, .L_080e171c
	adds r3, r4, r5
	movs r6, #0
	ldr r2, .L_080e1720
	str r6, [r3]
	movs r1, #2
	adds r3, r4, r0
	str r1, [r3]
	adds r5, #12
	adds r3, r4, r2
	movs r2, #1
	str r2, [r3]
	adds r3, r4, r5
	str r6, [r3]
	ldr r0, [sp, #132]
	movs r5, #144
	lsls r5, r5, #3
	str r2, [r0, #16]
	b .L_080e1724
	.2byte 0x0000
.L_080e1704:
	.4byte 0x00000000
.L_080e1708:
	.4byte 0x00002784
.L_080e170c:
	.4byte 0x00000302
.L_080e1710:
	.4byte 0x00002710
.L_080e1714:
	.4byte 0x0400000c
.L_080e1718:
	.4byte 0x00007790
.L_080e171c:
	.4byte 0x00007794
.L_080e1720:
	.4byte 0x00007798
.L_080e1724:
	mov r10, r1
	ldr r0, .L_080e17ac
	adds r1, r5, #0
	bl Scheduler_AddOrUpdateCallback
	adds r1, r5, #0
	ldr r0, .L_080e17b0
	bl Scheduler_AddOrUpdateCallback
	movs r2, #239
	ldr r1, [sp, #128]
	lsls r2, r2, #7
	adds r1, r1, r2
	str r6, [r1]
	mov r8, r1
	movs r0, #0
	movs r1, #0
	bl BattleEffect_WipeCanvas
	ldr r1, .L_080e17b4
	movs r0, #1
	movs r2, #0
	bl BattleBackground_LoadFar
	ldr r2, .L_080e17b8
	movs r3, #240
	str r3, [r2, #16]
	movs r0, #0
	movs r1, #1
	bl BattleEffect_WipeCanvas
	ldr r6, .L_080e17bc
	ldr r5, .L_080e179c
	movs r3, #3
	strh r5, [r6]
	movs r1, #7
	movs r2, #7
	movs r0, #46
	str r3, [sp, #0]
	bl BattleEffect_LoadWork
	ldr r3, .L_080e17c0
	adds r3, #184
	ldr r3, [r3]
	movs r2, #128
	str r3, [sp, #108]
	ldr r3, .L_080e17a0
	lsls r2, r2, #19
	strh r3, [r2]
	ldr r3, .L_080e17a4
	adds r2, #82
	strh r5, [r6]
	strh r3, [r2]
	ldr r3, .L_080e17a8
	subs r2, #2
	strh r3, [r2]
	mov r4, r8
	mov r3, r10
	str r3, [r4]
	b .L_080e17c4
.L_080e179c:
	.4byte 0x00000080
.L_080e17a0:
	.4byte 0x00007741
.L_080e17a4:
	.4byte 0x0000100f
.L_080e17a8:
	.4byte 0x00003f44
.L_080e17ac:
	.4byte BattleFx_AdvanceScrollOnInterval
.L_080e17b0:
	.4byte BattlePresentation_ProcessPendingGraphicsTransfer
.L_080e17b4:
	.4byte 0x0000003d
.L_080e17b8:
	.4byte gProjection
.L_080e17bc:
	.4byte 0x04000020
.L_080e17c0:
	.4byte gWorkSlot
.L_080e17c4:
	ldr r5, [sp, #128]
	ldr r0, .L_080e1834
	movs r3, #75
	adds r2, r5, r0
	str r3, [r2]
	movs r2, #152
	lsls r2, r2, #1
	add r2, sp
	adds r5, r2, #0
	str r2, [sp, #48]
	ldr r2, .L_080e1838
	movs r1, #0
	add r2, sp
	mov r9, r1
	movs r6, #63
	adds r0, r5, #0
	movs r1, #3
	adds r4, r2, #0
.L_080e17e8:
	strb r1, [r0]
	strb r1, [r4]
	ldrb r3, [r0]
	subs r4, #1
	cmp r3, #63
	bls .L_080e17f6
	strb r6, [r5]
.L_080e17f6:
	ldrb r3, [r2]
	cmp r3, #63
	bls .L_080e17fe
	strb r6, [r2]
.L_080e17fe:
	movs r3, #1
	add r9, r3
	mov r3, r9
	adds r5, #1
	adds r0, #1
	subs r2, #1
	adds r1, #8
	cmp r3, #16
	bne .L_080e17e8
	ldr r2, .L_080e183c
	ldr r3, .L_080e1830
	mov r5, sp
	ldr r0, .L_080e1840
	ldr r1, .L_080e1844
	movs r4, #0
	adds r5, #136
	strh r3, [r2]
	str r4, [sp, #116]
	str r5, [sp, #96]
	str r0, [sp, #28]
	str r4, [sp, #24]
	str r4, [sp, #20]
	str r1, [sp, #16]
	b .L_080e1cc2
	.2byte 0x0000
.L_080e1830:
	.4byte 0x00000784
.L_080e1834:
	.4byte 0x00007784
.L_080e1838:
	.4byte 0x0000014f
.L_080e183c:
	.4byte 0x0400000c
.L_080e1840:
	.4byte 0xfffff460
.L_080e1844:
	.4byte 0xfff26c00
.L_080e1848:
	ldr r2, .L_080e1a98
	ldr r0, [sp, #20]
	ldr r1, .L_080e1a9c
	movs r3, #0
	bl Graphics_UpdatePhasePalette
	ldr r2, [sp, #116]
	cmp r2, #150
	bne .L_080e1870
	ldr r3, [sp, #128]
	movs r4, #239
	lsls r4, r4, #7
	adds r2, r3, r4
	movs r3, #1
	str r3, [r2]
	ldr r5, [sp, #128]
	ldr r0, .L_080e1aa0
	ldr r3, .L_080e1aa4
	adds r2, r5, r0
	b .L_080e1882
.L_080e1870:
	ldr r1, [sp, #128]
	movs r2, #239
	lsls r2, r2, #7
	ldr r4, [sp, #104]
	ldr r5, .L_080e1aa0
	adds r3, r1, r2
	str r4, [r3]
	adds r2, r1, r5
	movs r3, #75
.L_080e1882:
	str r3, [r2]
	movs r0, #255
	movs r1, #192
	ldr r3, .L_080e1aa8
	lsls r1, r1, #8
	lsls r0, r0, #17
	bl _call_via_r3
	adds r1, r0, #0
	movs r0, #255
	lsls r0, r0, #17
	ldr r2, .L_080e1aac
	bl Camera_StoreSceneParameters
	bl Render_ResetTransformState
	ldr r0, [sp, #116]
	cmp r0, #128
	ble .L_080e1922
	adds r6, r0, #0
	subs r6, #128
	cmp r6, #22
	ble .L_080e18b2
	movs r6, #20
.L_080e18b2:
	add r0, sp, #148
	movs r3, #0
	str r3, [r0]
	negs r3, r6
	lsls r3, r3, #17
	str r3, [r0, #4]
	movs r3, #128
	lsls r3, r3, #18
	str r3, [r0, #8]
	asrs r3, r6, #2
	adds r3, #2
	str r3, [sp, #104]
	cmp r3, #8
	ble .L_080e18d2
	movs r1, #8
	str r1, [sp, #104]
.L_080e18d2:
	movs r5, #128
	bl SceneTransform_ApplyPosition
	lsls r5, r5, #8
	ldr r0, .L_080e1ab0
	bl SceneTransform_ApplyPitch
	adds r0, r5, #0
	bl SceneTransform_ApplyRoll
	adds r0, r5, #0
	bl SceneTransform_ApplyPitch
	lsls r0, r6, #12
	bl SceneTransform_ApplyYaw
	ldr r2, [sp, #116]
	cmp r2, #150
	ble .L_080e1908
	ldr r0, [sp, #96]
	ldr r3, [sp, #16]
	movs r4, #5
	str r3, [r0]
	str r3, [r0, #4]
	str r3, [r0, #8]
	str r4, [sp, #104]
	b .L_080e191c
.L_080e1908:
	lsls r3, r6, #1
	adds r3, r3, r6
	movs r2, #128
	ldr r0, [sp, #96]
	lsls r3, r3, #10
	lsls r2, r2, #9
	subs r2, r2, r3
	str r2, [r0]
	str r2, [r0, #4]
	str r2, [r0, #8]
.L_080e191c:
	bl SceneTransform_ApplyScale
	b .L_080e194a
.L_080e1922:
	add r0, sp, #148
	movs r3, #0
	str r3, [r0]
	str r3, [r0, #4]
	movs r3, #128
	lsls r3, r3, #17
	str r3, [r0, #8]
	bl SceneTransform_ApplyPosition
	ldr r0, .L_080e1ab0
	bl SceneTransform_ApplyPitch
	ldr r0, [sp, #116]
	lsls r5, r0, #8
	adds r0, r5, #0
	bl SceneTransform_ApplyRoll
	adds r0, r5, #0
	bl SceneTransform_ApplyPitch
.L_080e194a:
	ldr r1, [sp, #116]
	cmp r1, #149
	ble .L_080e1952
	b .L_080e1a64
.L_080e1952:
	mov r3, sp
	movs r2, #0
	adds r3, #160
	mov r9, r2
	str r3, [sp, #52]
	ldr r2, .L_080e1ab4
	add r6, sp, #292
	adds r5, r3, #0
	movs r7, #0
.L_080e1964:
	ldrsh r3, [r7, r2]
	subs r3, #96
	lsls r3, r3, #16
	str r3, [r6]
	movs r3, #0
	str r3, [r6, #4]
	adds r3, r7, #2
	ldrsh r3, [r3, r2]
	subs r3, #96
	lsls r3, r3, #16
	adds r1, r5, #0
	str r3, [r6, #8]
	adds r0, r6, #0
	str r2, [sp, #12]
	bl EffectPosition_ApplyBaseAndYOffset
	ldr r3, [r5]
	asrs r3, r3, #17
	adds r3, #64
	str r3, [r5]
	movs r1, #6
	ldrsh r3, [r5, r1]
	adds r3, #60
	str r3, [r5, #4]
	movs r3, #1
	add r9, r3
	mov r4, r9
	adds r5, #12
	adds r7, #4
	ldr r2, [sp, #12]
	cmp r4, #6
	bne .L_080e1964
	ldr r1, [sp, #24]
	movs r5, #0
	movs r0, #4
	str r0, [sp, #40]
	str r5, [sp, #36]
	str r1, [sp, #32]
	mov r9, r5
.L_080e19b2:
	ldr r2, [sp, #32]
	ldr r3, .L_080e1ab8
	adds r2, r2, r3
	mov r10, r2
	cmp r2, #48
	ble .L_080e19c2
	movs r4, #48
	mov r10, r4
.L_080e19c2:
	mov r5, r10
	cmp r5, #0
	blt .L_080e1a48
	movs r0, #0
	mov r8, r0
	cmp r5, #0
	beq .L_080e1a48
	ldr r1, [sp, #104]
	ldr r2, [sp, #40]
	ldr r3, [sp, #104]
	asrs r1, r1, #31
	lsls r3, r3, #1
	str r1, [sp, #44]
	ldr r5, [sp, #52]
	ldr r6, [sp, #36]
	ldr r7, .L_080e1abc
	str r2, [sp, #92]
	mov r11, r3
.L_080e19e6:
	adds r3, r6, #0
	adds r3, #12
	ldr r3, [r5, r3]
	ldr r4, [r5, r6]
	subs r3, r3, r4
	mov r0, r8
	muls r0, r3
	ldr r1, .L_080e1ac0
	movs r0, r0
	mov r12, pc
	bx r7
	adds r4, r4, r0
	adds r3, r6, #0
	ldr r0, [sp, #92]
	adds r3, #16
	ldr r2, [r5, r3]
	ldr r3, [r5, r0]
	subs r2, r2, r3
	mov r0, r8
	muls r0, r2
	ldr r1, .L_080e1ac0
	mov r12, pc
	bx r7
	adds r3, r3, r0
	mov r2, r11
	ldr r0, .L_080e1ac4
	subs r2, #2
	ldrh r1, [r0, r2]
	ldr r2, [sp, #120]
	ldr r0, [sp, #44]
	adds r1, r2, r1
	lsrs r2, r0, #31
	ldr r0, [sp, #104]
	adds r2, r0, r2
	asrs r2, r2, #1
	subs r4, r4, r2
	mov r2, r11
	subs r3, r3, r0
	str r0, [sp, #0]
	str r2, [sp, #4]
	ldr r0, [sp, #124]
	adds r2, r4, #0
	ldr r4, [sp, #108]
	bl _call_via_r4
	movs r0, #1
	add r8, r0
	cmp r8, r10
	bne .L_080e19e6
.L_080e1a48:
	ldr r1, [sp, #40]
	ldr r2, [sp, #36]
	ldr r3, [sp, #32]
	movs r4, #1
	add r9, r4
	adds r1, #24
	adds r2, #24
	subs r3, #48
	mov r5, r9
	str r1, [sp, #40]
	str r2, [sp, #36]
	str r3, [sp, #32]
	cmp r5, #3
	bne .L_080e19b2
.L_080e1a64:
	ldr r0, [sp, #116]
	cmp r0, #179
	ble .L_080e1a6c
	b .L_080e1b8e
.L_080e1a6c:
	movs r5, #0
	cmp r0, #155
	ble .L_080e1a76
	adds r5, r0, #0
	subs r5, #156
.L_080e1a76:
	cmp r5, #7
	ble .L_080e1a7c
	movs r5, #7
.L_080e1a7c:
	ldr r1, [sp, #116]
	cmp r1, #139
	bgt .L_080e1acc
	movs r3, #3
	movs r2, #7
	movs r0, #47
	movs r1, #7
	str r3, [sp, #0]
	bl BattleEffect_LoadWork
	ldr r2, .L_080e1ac8
	ldr r2, [r2]
	str r2, [sp, #88]
	b .L_080e1ae2
.L_080e1a98:
	.4byte 0x00005555
.L_080e1a9c:
	.4byte 0x0000aaab
.L_080e1aa0:
	.4byte 0x00007784
.L_080e1aa4:
	.4byte 0x1a1a1a1a
.L_080e1aa8:
	.4byte IwramRatioMulQ14
.L_080e1aac:
	.4byte 0x7fff0000
.L_080e1ab0:
	.4byte 0xfffff000
.L_080e1ab4:
	.4byte Data_080eda88 + 0x10
.L_080e1ab8:
	.4byte 0xffffff00
.L_080e1abc:
	.4byte IwramMulQ16ReturnIp
.L_080e1ac0:
	.4byte 0x00000555
.L_080e1ac4:
	.4byte ParticleStreams_CellOffsets
.L_080e1ac8:
	.4byte gTransitionWork + 0xc
.L_080e1acc:
	movs r3, #2
	str r3, [sp, #0]
	movs r0, #47
	movs r1, #7
	movs r2, #7
	movs r3, #3
	bl BattleEffect_LoadWork
	ldr r4, .L_080e1db0
	ldr r4, [r4]
	str r4, [sp, #88]
.L_080e1ae2:
	ldr r0, [sp, #100]
	mov r10, r0
	cmp r0, #128
	ble .L_080e1aee
	movs r1, #128
	mov r10, r1
.L_080e1aee:
	movs r2, #0
	mov r3, r10
	mov r9, r2
	cmp r3, #0
	beq .L_080e1b88
	lsls r3, r5, #1
	adds r3, r3, r5
	ldr r7, [sp, #104]
	lsls r3, r3, #7
	movs r4, #146
	adds r3, r3, r5
	adds r7, #1
	lsls r4, r4, #1
	lsls r3, r3, #1
	add r4, sp
	lsls r0, r7, #1
	str r3, [sp, #84]
	mov r8, r4
	add r6, sp, #160
	mov r11, r0
.L_080e1b16:
	mov r1, r9
	lsls r5, r1, #9
	adds r0, r5, #0
	bl Trig_Sin
	lsls r3, r0, #1
	adds r3, r3, r0
	mov r2, r8
	lsls r3, r3, #5
	str r3, [r2]
	adds r0, r5, #0
	bl Trig_Cos
	lsls r3, r0, #1
	adds r3, r3, r0
	lsls r3, r3, #5
	negs r3, r3
	mov r4, r8
	str r3, [r4, #8]
	adds r1, r6, #0
	mov r0, r8
	bl EffectPosition_ApplyBaseAndYOffset
	ldr r2, [r6]
	movs r5, #6
	ldrsh r3, [r6, r5]
	asrs r2, r2, #17
	ldr r0, .L_080e1db4
	adds r2, #64
	adds r3, #60
	mov r1, r11
	str r2, [r6]
	str r3, [r6, #4]
	subs r1, #2
	ldrh r1, [r0, r1]
	ldr r0, [sp, #84]
	ldr r4, [sp, #128]
	adds r1, r0, r1
	lsrs r0, r7, #31
	ldr r5, .L_080e1db8
	adds r0, r7, r0
	adds r1, r4, r1
	asrs r0, r0, #1
	adds r1, r1, r5
	subs r2, r2, r0
	movs r5, #1
	mov r0, r11
	str r0, [sp, #4]
	subs r3, r3, r7
	str r7, [sp, #0]
	ldr r0, [sp, #124]
	ldr r4, [sp, #88]
	add r9, r5
	bl _call_via_r4
	cmp r9, r10
	bne .L_080e1b16
.L_080e1b88:
	movs r0, #47
	bl Runtime_ReleaseHeapBlock
.L_080e1b8e:
	ldr r3, [sp, #116]
	subs r3, #151
	cmp r3, #16
	bhi .L_080e1c90
	ldr r0, [sp, #28]
	ldr r1, [sp, #116]
	movs r4, #0
	mov r8, r0
	cmp r1, #151
	ble .L_080e1ba6
	adds r4, r1, #0
	subs r4, #152
.L_080e1ba6:
	cmp r4, #15
	ble .L_080e1bac
	movs r4, #15
.L_080e1bac:
	lsls r3, r4, #1
	ldr r5, [sp, #48]
	negs r3, r3
	adds r7, r3, #0
	adds r3, r4, r5
	movs r2, #1
	adds r6, r4, #0
	adds r3, #1
	mov r9, r2
	adds r7, #30
	adds r6, #49
	mov r10, r3
.L_080e1bc4:
	mov r0, r9
	adds r3, r4, r0
	cmp r3, #15
	bgt .L_080e1be6
	movs r2, #1
	movs r3, #16
	subs r3, r3, r0
	str r2, [sp, #4]
	str r4, [sp, #8]
	str r7, [sp, #0]
	ldr r0, [sp, #124]
	mov r1, r10
	adds r2, r6, #0
	ldr r5, [sp, #108]
	bl _call_via_r5
	ldr r4, [sp, #8]
.L_080e1be6:
	movs r0, #1
	add r9, r0
	mov r1, r9
	subs r7, #2
	adds r6, #1
	add r10, r0
	cmp r1, #10
	bne .L_080e1bc4
	movs r2, #0
	mov r3, r8
	mov r9, r2
	cmp r3, #0
	beq .L_080e1c32
	lsls r2, r4, #1
	movs r3, #32
	adds r6, r4, #0
	subs r3, r3, r2
	adds r6, #48
	mov r10, r3
	movs r7, #1
.L_080e1c0e:
	ldr r2, [sp, #48]
	mov r3, r9
	mov r5, r10
	adds r1, r2, r4
	str r5, [sp, #0]
	str r4, [sp, #8]
	adds r3, #16
	str r7, [sp, #4]
	ldr r0, [sp, #124]
	adds r2, r6, #0
	ldr r5, [sp, #108]
	bl _call_via_r5
	movs r0, #1
	add r9, r0
	ldr r4, [sp, #8]
	cmp r9, r8
	bne .L_080e1c0e
.L_080e1c32:
	movs r1, #2
	str r1, [sp, #0]
	movs r2, #7
	movs r1, #7
	movs r3, #3
	movs r0, #47
	bl BattleEffect_LoadWork
	ldr r2, [sp, #28]
	movs r5, #96
	movs r6, #32
	str r5, [sp, #4]
	str r6, [sp, #0]
	ldr r3, .L_080e1db0
	subs r2, #56
	mov r8, r2
	ldr r1, [sp, #128]
	ldr r4, [r3]
	ldr r0, [sp, #124]
	movs r2, #32
	mov r3, r8
	bl _call_via_r4
	movs r0, #47
	bl Runtime_ReleaseHeapBlock
	movs r4, #2
	movs r1, #7
	movs r2, #7
	movs r3, #7
	movs r0, #47
	str r4, [sp, #0]
	bl BattleEffect_LoadWork
	str r5, [sp, #4]
	str r6, [sp, #0]
	ldr r5, .L_080e1db0
	ldr r0, [sp, #124]
	ldr r4, [r5]
	ldr r1, [sp, #128]
	movs r2, #64
	mov r3, r8
	bl _call_via_r4
	movs r0, #47
	bl Runtime_ReleaseHeapBlock
.L_080e1c90:
	ldr r0, [sp, #128]
	ldr r1, .L_080e1dbc
	movs r3, #1
	adds r2, r0, r1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	movs r0, #192
	ldr r2, [sp, #28]
	ldr r3, [sp, #24]
	ldr r4, [sp, #20]
	ldr r5, [sp, #16]
	ldr r1, [sp, #116]
	lsls r0, r0, #5
	adds r2, #20
	adds r3, #4
	adds r4, #2
	adds r5, r5, r0
	adds r1, #1
	str r2, [sp, #28]
	str r3, [sp, #24]
	str r4, [sp, #20]
	str r5, [sp, #16]
	str r1, [sp, #116]
.L_080e1cc2:
	ldr r2, [sp, #116]
	cmp r2, #170
	beq .L_080e1d00
	ldr r4, [sp, #20]
	movs r3, #2
	str r3, [sp, #104]
	str r4, [sp, #100]
	cmp r2, #16
	bne .L_080e1cda
	movs r0, #140
	bl AudioCommand_PlayFar
.L_080e1cda:
	ldr r5, [sp, #116]
	cmp r5, #132
	bne .L_080e1ce6
	movs r0, #131
	bl AudioCommand_PlayFar
.L_080e1ce6:
	ldr r0, [sp, #116]
	cmp r0, #151
	bne .L_080e1cf2
	movs r0, #145
	bl AudioCommand_PlayFar
.L_080e1cf2:
	ldr r3, .L_080e1dc0
	ldr r3, [r3]
	movs r2, #3
	ands r3, r2
	cmp r3, #0
	bne .L_080e1d00
	b .L_080e1848
.L_080e1d00:
	ldr r1, [sp, #132]
	movs r3, #0
	str r3, [r1, #16]
	ldr r0, .L_080e1dc4
	bl Scheduler_RemoveCallback
	bl BattleEffect_SetupBlendedDisplay
	ldr r4, .L_080e1dc8
	ldr r2, [sp, #128]
	adds r3, r2, r4
	ldr r0, [r3]
	bl BattleFx_SelectLivingTargets
	ldr r1, .L_080e1dcc
	movs r0, #9
	movs r2, #1
	bl BattleFx_SpawnObjects
	ldr r0, .L_080e1dd0
	ldr r1, .L_080e1dd4
	movs r2, #1
	movs r3, #0
	bl Resource_LoadAndDecompress
	ldr r0, .L_080e1dd8
	ldr r1, [sp, #128]
	movs r2, #1
	movs r3, #1
	bl Resource_LoadAndDecompress
	movs r2, #192
	ldr r5, [sp, #128]
	lsls r2, r2, #7
	adds r1, r5, r2
	movs r3, #0
	movs r2, #1
	ldr r0, .L_080e1ddc
	bl Resource_LoadAndDecompress
	ldr r0, .L_080e1de0
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_080e1de4
	movs r2, #128
	lsls r0, r0, #19
	bl _call_via_r3
	movs r2, #0
	ldr r1, [sp, #120]
	movs r3, #0
	ldr r0, .L_080e1de8
	bl Resource_LoadAndDecompress
	ldr r2, .L_080e1dec
	ldr r3, .L_080e1dac
	movs r4, #128
	strh r3, [r2]
	movs r3, #160
	movs r5, #0
	lsls r3, r3, #15
	lsls r4, r4, #15
	str r3, [sp, #80]
	str r4, [sp, #76]
	str r5, [sp, #72]
	str r5, [sp, #68]
	ldr r2, .L_080e1df0
	ldr r3, .L_080e1df4
	mov r9, r5
	movs r1, #0
.L_080e1d90:
	movs r0, #1
	add r9, r0
	str r1, [r3]
	adds r3, #28
	cmp r9, r2
	bne .L_080e1d90
	ldr r2, [sp, #128]
	movs r3, #225
	movs r1, #0
	lsls r3, r3, #7
	mov r9, r1
	adds r5, r2, r3
	b .L_080e1df8
	.2byte 0x0000
.L_080e1dac:
	.4byte 0x00001010
.L_080e1db0:
	.4byte gTransitionWork + 0xc
.L_080e1db4:
	.4byte ParticleStreams_CellOffsets
.L_080e1db8:
	.4byte 0x00002710
.L_080e1dbc:
	.4byte 0x00007824
.L_080e1dc0:
	.4byte gKeysRepeat
.L_080e1dc4:
	.4byte BattleFx_AdvanceScrollOnInterval
.L_080e1dc8:
	.4byte 0x00007828
.L_080e1dcc:
	.4byte 0x00000173
.L_080e1dd0:
	.4byte 0x000000ce
.L_080e1dd4:
	.4byte gMapCellBuffer
.L_080e1dd8:
	.4byte 0x000000d1
.L_080e1ddc:
	.4byte 0x00000066
.L_080e1de0:
	.4byte 0x000000cf
.L_080e1de4:
	.4byte IwramCopyWords
.L_080e1de8:
	.4byte 0x00000074
.L_080e1dec:
	.4byte 0x04000052
.L_080e1df0:
	.4byte 0x0000038e
.L_080e1df4:
	.4byte gMapCellBuffer + 0xc70
.L_080e1df8:
	bl Random16
	movs r3, #31
	ands r3, r0
	subs r3, #16
	str r3, [r5]
	bl Random16
	movs r3, #63
	ands r3, r0
	str r3, [r5, #4]
	bl Random16
	movs r3, #15
	movs r4, #1
	ands r3, r0
	add r9, r4
	negs r3, r3
	mov r0, r9
	str r3, [r5, #24]
	adds r5, #28
	cmp r0, #16
	bne .L_080e1df8
	ldr r1, [sp, #128]
	movs r3, #239
	lsls r3, r3, #7
	ldr r4, .L_080e21b0
	adds r2, r1, r3
	movs r3, #2
	str r3, [r2]
	adds r2, r1, r4
	movs r3, #50
	movs r0, #145
	str r3, [r2]
	bl AudioCommand_PlayFar
	movs r1, #192
	ldr r0, [sp, #128]
	lsls r1, r1, #7
	movs r5, #0
	adds r1, r0, r1
	str r5, [sp, #116]
	str r1, [sp, #56]
.L_080e1e4e:
	ldr r2, [sp, #116]
	cmp r2, #20
	bne .L_080e1e60
	ldr r0, .L_080e21b4
	ldr r1, [sp, #128]
	movs r2, #1
	movs r3, #0
	bl Resource_LoadAndDecompress
.L_080e1e60:
	ldr r3, [sp, #116]
	subs r3, #80
	str r3, [sp, #64]
	cmp r3, #59
	bhi .L_080e1e7a
	ldr r4, [sp, #116]
	movs r3, #7
	ands r3, r4
	cmp r3, #0
	bne .L_080e1e7a
	movs r0, #134
	bl AudioCommand_PlayFar
.L_080e1e7a:
	ldr r5, [sp, #116]
	cmp r5, #140
	bne .L_080e1e86
	movs r0, #134
	bl BattleEventRuntime_BeginPhaseFar
.L_080e1e86:
	movs r0, #0
	movs r1, #8
	mov r8, r0
	mov r11, r1
.L_080e1e8e:
	ldr r4, [sp, #116]
	mov r3, r8
	lsls r2, r3, #7
	cmp r4, r11
	blt .L_080e1f78
	adds r3, r2, #0
	adds r3, #17
	cmp r4, r3
	bge .L_080e1f78
	adds r5, r2, #0
	subs r3, #8
	adds r5, #12
	cmp r4, r3
	blt .L_080e1ec4
	cmp r4, r5
	bge .L_080e1eca
	movs r3, #48
	movs r0, #112
	str r3, [sp, #0]
	str r0, [sp, #4]
	ldr r1, [sp, #128]
	ldr r0, [sp, #124]
	movs r2, #36
	movs r3, #0
	ldr r4, [sp, #108]
	bl _call_via_r4
.L_080e1ec4:
	ldr r0, [sp, #116]
	cmp r0, r5
	blt .L_080e1ef0
.L_080e1eca:
	mov r3, r11
	ldr r1, [sp, #116]
	adds r3, #8
	cmp r1, r3
	bge .L_080e1ef0
	movs r3, #48
	str r3, [sp, #0]
	movs r3, #112
	ldr r2, [sp, #128]
	str r3, [sp, #4]
	movs r3, #168
	lsls r3, r3, #5
	adds r1, r2, r3
	ldr r0, [sp, #124]
	movs r2, #36
	movs r3, #0
	ldr r4, [sp, #108]
	bl _call_via_r4
.L_080e1ef0:
	mov r3, r11
	ldr r5, [sp, #116]
	adds r3, #2
	cmp r5, r3
	bne .L_080e1f78
	movs r0, #0
	ldr r7, .L_080e21b8
	mov r10, r0
	mov r9, r0
.L_080e1f02:
	ldr r3, [r7, #24]
	cmp r3, #0
	bne .L_080e1f62
	bl Random16
	ldr r6, .L_080e21bc
	ands r6, r0
	bl Random16
	ldr r5, .L_080e21c0
	ldr r1, .L_080e21c4
	movs r2, #60
	lsls r3, r2, #16
	movs r4, #112
	ands r5, r0
	adds r5, r5, r1
	str r3, [r7]
	lsls r3, r4, #16
	str r3, [r7, #4]
	adds r0, r5, #0
	bl Trig_Sin
	adds r6, #32
	adds r3, r6, #0
	muls r3, r0
	asrs r3, r3, #7
	str r3, [r7, #12]
	adds r0, r5, #0
	bl Trig_Cos
	adds r3, r6, #0
	muls r3, r0
	lsls r3, r3, #1
	negs r3, r3
	asrs r3, r3, #7
	str r3, [r7, #16]
	bl Random16
	movs r3, #7
	ands r3, r0
	movs r5, #1
	movs r0, #128
	adds r3, #32
	add r10, r5
	lsls r0, r0, #2
	str r3, [r7, #24]
	cmp r10, r0
	beq .L_080e1f6e
.L_080e1f62:
	movs r1, #1
	ldr r2, .L_080e21c8
	add r9, r1
	adds r7, #28
	cmp r9, r2
	bne .L_080e1f02
.L_080e1f6e:
	ldr r3, [sp, #128]
	ldr r4, .L_080e21cc
	adds r2, r3, r4
	movs r3, #8
	str r3, [r2]
.L_080e1f78:
	movs r0, #1
	add r8, r0
	movs r5, #128
	mov r1, r8
	add r11, r5
	cmp r1, #1
	bne .L_080e1e8e
	ldr r2, [sp, #116]
	cmp r2, #10
	bne .L_080e1fcc
	movs r3, #0
	mov r9, r3
	ldr r4, [sp, #128]
	ldr r3, .L_080e21d0
	ldr r3, [r4, r3]
	ldr r3, [r3, #20]
	cmp r3, #0
	beq .L_080e1fcc
	ldr r0, .L_080e21d0
	movs r6, #36
	adds r5, r4, r0
.L_080e1fa2:
	ldr r3, [r5]
	ldrsh r0, [r3, r6]
	movs r3, #8
	str r3, [sp, #0]
	movs r2, #5
	mov r3, r9
	movs r1, #7
	bl ObjectGroup_UpdateMembers
	ldr r3, [r5]
	movs r1, #4
	ldrsh r0, [r3, r6]
	bl BattleMotion_ApplyVariantMotionFar
	movs r3, #1
	add r9, r3
	ldr r3, [r5]
	ldr r3, [r3, #20]
	adds r6, #2
	cmp r9, r3
	bne .L_080e1fa2
.L_080e1fcc:
	ldr r4, [sp, #64]
	cmp r4, #63
	bls .L_080e1fd4
	b .L_080e22e0
.L_080e1fd4:
	movs r5, #0
	ldr r6, .L_080e21d4
	mov r9, r5
	movs r5, #80
.L_080e1fdc:
	ldr r0, [sp, #116]
	cmp r0, r5
	bne .L_080e1fee
	movs r1, #128
	ldr r0, [sp, #124]
	lsls r1, r1, #7
	ldr r2, .L_080e21d8
	bl _call_via_r6
.L_080e1fee:
	movs r1, #1
	add r9, r1
	mov r2, r9
	adds r5, #8
	cmp r2, #6
	bne .L_080e1fdc
	movs r3, #0
	movs r4, #80
	mov r9, r3
	mov r11, r4
.L_080e2002:
	ldr r0, [sp, #116]
	mov r5, r9
	lsls r3, r5, #1
	cmp r0, r11
	bge .L_080e200e
	b .L_080e2134
.L_080e200e:
	adds r3, #82
	cmp r0, r3
	bge .L_080e206c
	bl Random16
	movs r1, #3
	adds r6, r0, #0
	ands r6, r1
	bl Random16
	movs r2, #3
	adds r5, r0, #0
	movs r3, #2
	str r3, [sp, #0]
	ands r5, r2
	movs r3, #3
	movs r2, #7
	movs r1, #7
	movs r0, #47
	bl BattleEffect_LoadWork
	movs r1, #3
	mov r0, r9
	bl __modsi3
	lsls r1, r0, #2
	adds r1, r1, r0
	movs r3, #72
	lsls r1, r1, #4
	ldr r4, [sp, #128]
	str r3, [sp, #0]
	str r3, [sp, #4]
	adds r1, r1, r0
	ldr r0, .L_080e21dc
	subs r6, #3
	adds r5, #32
	lsls r1, r1, #6
	adds r1, r4, r1
	adds r2, r6, #0
	ldr r4, [r0]
	adds r3, r5, #0
	ldr r0, [sp, #124]
	bl _call_via_r4
	movs r0, #47
	bl Runtime_ReleaseHeapBlock
.L_080e206c:
	ldr r1, [sp, #116]
	cmp r1, r11
	bne .L_080e2134
	movs r2, #0
	ldr r7, .L_080e21b8
	mov r10, r2
	mov r8, r2
.L_080e207a:
	ldr r3, [r7, #24]
	cmp r3, #0
	bne .L_080e20d6
	bl Random16
	ldr r6, .L_080e21bc
	ands r6, r0
	bl Random16
	ldr r3, .L_080e21e0
	adds r5, r0, #0
	ands r5, r3
	movs r3, #128
	lsls r3, r3, #13
	str r3, [r7]
	movs r3, #160
	lsls r3, r3, #15
	str r3, [r7, #4]
	adds r0, r5, #0
	bl Trig_Sin
	adds r6, #32
	adds r3, r6, #0
	muls r3, r0
	asrs r3, r3, #7
	str r3, [r7, #12]
	adds r0, r5, #0
	bl Trig_Cos
	adds r3, r6, #0
	muls r3, r0
	lsls r3, r3, #1
	negs r3, r3
	asrs r3, r3, #7
	str r3, [r7, #16]
	bl Random16
	movs r3, #7
	ands r3, r0
	adds r3, #32
	str r3, [r7, #24]
	movs r3, #1
	add r10, r3
	mov r4, r10
	cmp r4, #16
	beq .L_080e20e2
.L_080e20d6:
	movs r5, #1
	ldr r0, .L_080e21c8
	add r8, r5
	adds r7, #28
	cmp r8, r0
	bne .L_080e207a
.L_080e20e2:
	ldr r1, [sp, #128]
	ldr r3, .L_080e21cc
	ldr r5, .L_080e21d0
	adds r2, r1, r3
	movs r3, #8
	str r3, [r2]
	adds r2, r1, r5
	ldr r3, [r2]
	ldr r3, [r3, #20]
	movs r4, #0
	mov r8, r4
	cmp r3, #0
	beq .L_080e2134
	ldr r7, .L_080e21e4
	adds r5, r2, #0
	movs r6, #36
.L_080e2102:
	bl Random16
	ldr r3, [r5]
	movs r1, #3
	ands r0, r1
	ldrb r1, [r7, r0]
	ldrsh r0, [r3, r6]
	movs r3, #4
	str r3, [sp, #0]
	movs r2, #5
	mov r3, r8
	bl ObjectGroup_UpdateMembers
	ldr r3, [r5]
	movs r1, #4
	ldrsh r0, [r3, r6]
	bl BattleMotion_ApplyVariantMotionFar
	ldr r3, [r5]
	movs r0, #1
	ldr r3, [r3, #20]
	add r8, r0
	adds r6, #2
	cmp r8, r3
	bne .L_080e2102
.L_080e2134:
	movs r2, #1
	add r9, r2
	movs r1, #2
	mov r3, r9
	add r11, r1
	cmp r3, #30
	beq .L_080e2144
	b .L_080e2002
.L_080e2144:
	bl Random16
	movs r6, #7
	ands r6, r0
	bl Random16
	ldr r3, .L_080e21e0
	adds r5, r0, #0
	ands r5, r3
	adds r0, r5, #0
	bl Trig_Sin
	adds r6, #8
	adds r4, r6, #0
	muls r4, r0
	asrs r4, r4, #16
	mov r8, r4
	adds r0, r5, #0
	adds r4, #72
	str r4, [sp, #60]
	bl Trig_Cos
	adds r3, r6, #0
	muls r3, r0
	movs r2, #32
	asrs r3, r3, #16
	subs r2, r2, r3
	mov r11, r2
	movs r5, #2
	movs r1, #7
	movs r2, #7
	movs r3, #3
	movs r0, #47
	str r5, [sp, #0]
	bl BattleEffect_LoadWork
	movs r1, #24
	movs r5, #24
	movs r6, #12
	negs r1, r1
	str r5, [sp, #4]
	str r6, [sp, #0]
	ldr r2, .L_080e21dc
	movs r0, #60
	add r1, r11
	add r8, r0
	mov r10, r1
	ldr r0, [sp, #124]
	ldr r1, [sp, #56]
	ldr r4, [r2]
	mov r3, r10
	mov r2, r8
	b .L_080e21e8
	.2byte 0x0000
.L_080e21b0:
	.4byte 0x00007784
.L_080e21b4:
	.4byte 0x00000062
.L_080e21b8:
	.4byte gMapCellBuffer + 0xc58
.L_080e21bc:
	.4byte 0x000003ff
.L_080e21c0:
	.4byte 0x00007fff
.L_080e21c4:
	.4byte 0xffffc000
.L_080e21c8:
	.4byte 0x0000038e
.L_080e21cc:
	.4byte 0x000077a8
.L_080e21d0:
	.4byte 0x00007828
.L_080e21d4:
	.4byte IwramFillWords
.L_080e21d8:
	.4byte 0x10101010
.L_080e21dc:
	.4byte gTransitionWork + 0xc
.L_080e21e0:
	.4byte 0x0000ffff
.L_080e21e4:
	.4byte ParticleReveal_CellSourceOffsets + 0x42
.L_080e21e8:
	bl _call_via_r4
	movs r0, #47
	bl Runtime_ReleaseHeapBlock
	movs r3, #2
	str r3, [sp, #0]
	movs r1, #7
	movs r2, #7
	movs r3, #7
	movs r0, #47
	bl BattleEffect_LoadWork
	ldr r0, .L_080e24fc
	str r5, [sp, #4]
	str r6, [sp, #0]
	ldr r1, [sp, #56]
	ldr r2, [sp, #60]
	ldr r4, [r0]
	mov r3, r10
	ldr r0, [sp, #124]
	bl _call_via_r4
	movs r0, #47
	bl Runtime_ReleaseHeapBlock
	movs r1, #2
	str r1, [sp, #0]
	movs r2, #7
	movs r1, #7
	movs r3, #11
	movs r0, #47
	bl BattleEffect_LoadWork
	ldr r2, .L_080e24fc
	str r5, [sp, #4]
	str r6, [sp, #0]
	ldr r1, [sp, #56]
	ldr r4, [r2]
	ldr r0, [sp, #124]
	mov r2, r8
	mov r3, r11
	bl _call_via_r4
	movs r0, #47
	bl Runtime_ReleaseHeapBlock
	movs r3, #2
	str r3, [sp, #0]
	movs r1, #7
	movs r2, #7
	movs r3, #15
	movs r0, #47
	bl BattleEffect_LoadWork
	str r5, [sp, #4]
	str r6, [sp, #0]
	ldr r5, .L_080e24fc
	ldr r0, [sp, #124]
	ldr r4, [r5]
	ldr r1, [sp, #56]
	ldr r2, [sp, #60]
	mov r3, r11
	bl _call_via_r4
	movs r0, #47
	bl Runtime_ReleaseHeapBlock
	ldr r7, .L_080e2500
	movs r0, #0
	mov r8, r0
	mov r9, r0
.L_080e2278:
	ldr r3, [r7, #24]
	cmp r3, #0
	bne .L_080e22d4
	bl Random16
	movs r5, #63
	ands r5, r0
	bl Random16
	ldr r3, .L_080e2504
	ldr r1, [sp, #60]
	adds r6, r0, #0
	mov r2, r11
	ands r6, r3
	lsls r3, r1, #16
	str r3, [r7]
	lsls r3, r2, #16
	str r3, [r7, #4]
	adds r0, r6, #0
	bl Trig_Sin
	adds r5, #64
	adds r3, r5, #0
	muls r3, r0
	asrs r3, r3, #7
	str r3, [r7, #12]
	adds r0, r6, #0
	bl Trig_Cos
	adds r3, r5, #0
	muls r3, r0
	lsls r3, r3, #1
	negs r3, r3
	asrs r3, r3, #7
	str r3, [r7, #16]
	bl Random16
	movs r3, #7
	ands r3, r0
	adds r3, #16
	str r3, [r7, #24]
	movs r3, #1
	add r8, r3
	mov r4, r8
	cmp r4, #4
	beq .L_080e22e0
.L_080e22d4:
	movs r5, #1
	ldr r0, .L_080e2508
	add r9, r5
	adds r7, #28
	cmp r9, r0
	bne .L_080e2278
.L_080e22e0:
	movs r1, #2
	str r1, [sp, #0]
	movs r2, #7
	movs r3, #15
	movs r0, #47
	movs r1, #7
	bl BattleEffect_LoadWork
	ldr r2, .L_080e24fc
	ldr r2, [r2]
	movs r3, #0
	ldr r5, .L_080e2500
	mov r8, r2
	mov r9, r3
.L_080e22fc:
	ldr r3, [r5, #24]
	cmp r3, #0
	ble .L_080e2364
	subs r3, #1
	str r3, [r5, #24]
	adds r0, r5, #0
	movs r1, #60
	movs r2, #0
	bl EffectStep_AdvanceWithGravity2D
	movs r4, #240
	ldr r3, [r5, #4]
	lsls r4, r4, #15
	cmp r3, r4
	ble .L_080e2328
	ldr r3, [r5, #16]
	negs r3, r3
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	str r3, [r5, #16]
	b .L_080e2364
.L_080e2328:
	ldr r2, [r5]
	ldr r0, .L_080e250c
	cmp r2, r0
	bhi .L_080e2364
	cmp r3, #0
	blt .L_080e2364
	ldr r0, [r5, #24]
	asrs r6, r2, #16
	asrs r7, r3, #16
	cmp r0, #0
	bge .L_080e2340
	adds r0, #15
.L_080e2340:
	asrs r0, r0, #4
	adds r0, #1
	lsls r4, r0, #1
	ldr r2, .L_080e2510
	subs r3, r4, #2
	ldrh r1, [r2, r3]
	ldr r2, [sp, #120]
	adds r1, r2, r1
	lsrs r2, r0, #31
	adds r2, r0, r2
	asrs r2, r2, #1
	subs r3, r7, r0
	str r0, [sp, #0]
	subs r2, r6, r2
	str r4, [sp, #4]
	ldr r0, [sp, #124]
	bl _call_via_r8
.L_080e2364:
	movs r3, #1
	ldr r4, .L_080e2508
	add r9, r3
	adds r5, #28
	cmp r9, r4
	bne .L_080e22fc
	movs r0, #47
	bl Runtime_ReleaseHeapBlock
	ldr r5, [sp, #116]
	cmp r5, #15
	ble .L_080e23cc
	cmp r5, #32
	bne .L_080e238a
	movs r0, #128
	ldr r1, .L_080e2514
	lsls r0, r0, #11
	str r0, [sp, #72]
	str r1, [sp, #68]
.L_080e238a:
	ldr r2, [sp, #116]
	cmp r2, #31
	ble .L_080e23c2
	ldr r4, [sp, #72]
	ldr r3, [sp, #80]
	adds r3, r3, r4
	ldr r5, [sp, #76]
	str r3, [sp, #80]
	ldr r0, [sp, #68]
	lsls r3, r4, #4
	subs r3, r3, r4
	adds r5, r5, r0
	lsls r3, r3, #2
	str r5, [sp, #76]
	cmp r3, #0
	bge .L_080e23ac
	adds r3, #63
.L_080e23ac:
	ldr r1, [sp, #68]
	asrs r3, r3, #6
	str r3, [sp, #72]
	lsls r3, r1, #4
	subs r3, r3, r1
	lsls r3, r3, #2
	cmp r3, #0
	bge .L_080e23be
	adds r3, #63
.L_080e23be:
	asrs r3, r3, #6
	str r3, [sp, #68]
.L_080e23c2:
	movs r0, #0
	ldr r1, [sp, #80]
	ldr r2, [sp, #76]
	bl BattleFx_PlaceFormationObjects
.L_080e23cc:
	ldr r3, [sp, #116]
	subs r3, #16
	cmp r3, #127
	bhi .L_080e247c
	ldr r3, [sp, #80]
	ldr r4, .L_080e2518
	ldr r5, .L_080e251c
	movs r2, #0
	asrs r3, r3, #17
	mov r9, r2
	mov r11, r3
	movs r7, #3
	mov r10, r4
	mov r8, r5
.L_080e23e8:
	mov r4, r9
	ands r4, r7
	str r4, [sp, #8]
	bl Random16
	ldr r3, .L_080e2504
	adds r5, r0, #0
	ands r5, r3
	adds r0, r5, #0
	bl Trig_Sin
	ldr r4, [sp, #8]
	adds r6, r0, #0
	mov r0, r10
	ldrb r3, [r0, r4]
	lsls r6, r6, #3
	asrs r6, r6, #16
	lsrs r3, r3, #1
	adds r0, r5, #0
	add r6, r11
	subs r6, r6, r3
	bl Trig_Cos
	ldr r4, [sp, #8]
	mov r1, r8
	lsls r5, r0, #2
	ldrb r3, [r1, r4]
	adds r5, r5, r0
	lsls r5, r5, #3
	lsrs r3, r3, #1
	asrs r5, r5, #16
	subs r5, r5, r3
	bl Random16
	ldr r3, .L_080e2520
	ands r0, r7
	ldrb r2, [r3, r0]
	adds r3, r7, #0
	orrs r3, r2
	movs r2, #2
	str r2, [sp, #0]
	movs r1, #7
	movs r2, #7
	movs r0, #47
	bl BattleEffect_LoadWork
	ldr r4, [sp, #8]
	ldr r2, .L_080e2524
	lsls r3, r4, #1
	ldrh r1, [r2, r3]
	ldr r3, .L_080e2528
	mov r0, r10
	adds r1, r1, r3
	ldrb r3, [r0, r4]
	str r3, [sp, #0]
	mov r2, r8
	ldrb r3, [r2, r4]
	str r3, [sp, #4]
	ldr r3, .L_080e24fc
	adds r5, #56
	ldr r4, [r3]
	ldr r0, [sp, #124]
	adds r3, r5, #0
	adds r2, r6, #0
	bl _call_via_r4
	movs r0, #47
	bl Runtime_ReleaseHeapBlock
	movs r4, #1
	add r9, r4
	mov r5, r9
	cmp r5, #3
	bne .L_080e23e8
.L_080e247c:
	ldr r0, [sp, #116]
	cmp r0, #31
	bgt .L_080e248c
	movs r0, #8
	movs r1, #16
	bl Camera_ApplyShake
	b .L_080e2494
.L_080e248c:
	movs r0, #4
	movs r1, #4
	bl Camera_ApplyShake
.L_080e2494:
	bl ObjectGroup_TickMemberTimers
	ldr r3, .L_080e252c
	ldr r1, [sp, #128]
	adds r2, r1, r3
	movs r3, #1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r4, [sp, #116]
	adds r4, #1
	str r4, [sp, #116]
	cmp r4, #192
	beq .L_080e24b4
	b .L_080e1e4e
.L_080e24b4:
	movs r0, #46
	bl Runtime_ReleaseHeapBlock
	ldr r0, .L_080e2530
	bl Scheduler_RemoveCallback
	movs r0, #0
	ldr r1, [sp, #80]
	ldr r2, [sp, #76]
	bl BattleEffect_RunImpactBurst
	ldr r1, .L_080e2534
	ldr r0, [sp, #128]
	movs r5, #0
	mov r9, r5
	adds r5, r0, r1
.L_080e24d4:
	ldmia r5!, {r0}
	bl ResourceObject_ReleaseFar
	movs r2, #1
	add r9, r2
	mov r3, r9
	cmp r3, #9
	bne .L_080e24d4
	bl BattleFx_EndCanvasLayer
	add sp, #336
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
.L_080e24fc:
	.4byte gTransitionWork + 0xc
.L_080e2500:
	.4byte gMapCellBuffer + 0xc58
.L_080e2504:
	.4byte 0x0000ffff
.L_080e2508:
	.4byte 0x0000038e
.L_080e250c:
	.4byte 0x007effff
.L_080e2510:
	.4byte ParticleStreams_CellOffsets
.L_080e2514:
	.4byte 0xffffc000
.L_080e2518:
	.4byte BattleFx_GlintCellWidths
.L_080e251c:
	.4byte BattleFx_GlintCellHeights
.L_080e2520:
	.4byte ParticleReveal_CellSourceOffsets + 0x46
.L_080e2524:
	.4byte BattleFx_GlintCellOffsets
.L_080e2528:
	.4byte gMapCellBuffer
.L_080e252c:
	.4byte 0x00007824
.L_080e2530:
	.4byte BattlePresentation_ProcessPendingGraphicsTransfer
.L_080e2534:
	.4byte 0x000077d8
