.syntax unified
	.thumb
	.global BattleEffect_RunPaletteParticles
	.thumb_func
BattleEffect_RunPaletteParticles:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #72
	str r1, [sp, #56]
	ldr r5, .L_080d24e0
	ldr r1, [r5]
	adds r3, r5, #0
	str r1, [sp, #52]
	subs r3, #112
	ldr r3, [r3]
	str r3, [sp, #48]
	subs r3, r5, #4
	ldr r3, [r3]
	ldr r7, .L_080d24e4
	mov r10, r3
	ldr r2, [r5, #4]
	add r7, r10
	str r2, [sp, #32]
	str r0, [r7]
	movs r0, #0
	bl BattleFx_BeginCanvasLayer
	ldr r2, .L_080d24e8
	ldr r3, .L_080d24d8
	strh r3, [r2]
	ldr r3, .L_080d24dc
	subs r2, #70
	strh r3, [r2]
	movs r3, #2
	str r3, [sp, #0]
	movs r1, #7
	movs r2, #7
	mov r8, r3
	movs r0, #46
	movs r3, #3
	bl BattleEffect_LoadWork
	ldr r4, [r5, #24]
	movs r3, #3
	movs r1, #7
	movs r2, #7
	movs r0, #47
	str r4, [sp, #40]
	str r3, [sp, #0]
	bl BattleEffect_LoadWork
	ldr r5, [r5, #28]
	ldr r0, .L_080d24ec
	str r5, [sp, #44]
	bl Resource_GetTableEntry
	adds r5, r0, #0
	b .L_080d24f0
.L_080d24d8:
	.4byte 0x00001010
.L_080d24dc:
	.4byte 0x00000784
.L_080d24e0:
	.4byte gBattleFxWork + 0x4
.L_080d24e4:
	.4byte 0x00007828
.L_080d24e8:
	.4byte 0x04000052
.L_080d24ec:
	.4byte 0x0000007d
.L_080d24f0:
	movs r0, #160
	movs r2, #128
	adds r1, r5, #0
	ldr r6, .L_080d2848
	adds r5, #128
	lsls r0, r0, #19
	bl _call_via_r6
	mov r1, r10
	adds r0, r5, #0
	bl Resource_DecodeType01
	ldr r0, .L_080d284c
	bl Resource_GetTableEntry
	movs r1, #192
	adds r5, r0, #0
	adds r5, #128
	lsls r1, r1, #6
	add r1, r10
	adds r0, r5, #0
	bl Resource_DecodeType01
	ldr r0, .L_080d2850
	bl Resource_GetTableEntry
	ldr r1, [sp, #32]
	bl Resource_DecodeType01
	ldr r0, [sp, #56]
	cmp r0, #1
	bne .L_080d2542
	ldr r0, .L_080d2854
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	lsls r0, r0, #19
	movs r2, #128
	bl _call_via_r6
.L_080d2542:
	movs r3, #239
	lsls r3, r3, #7
	ldr r2, .L_080d2858
	add r3, r10
	mov r1, r8
	str r1, [r3]
	add r2, r10
	movs r3, #75
	movs r1, #144
	str r3, [r2]
	ldr r0, .L_080d285c
	lsls r1, r1, #3
	bl Scheduler_AddOrUpdateCallback
	ldr r3, [sp, #56]
	movs r2, #1
	str r2, [sp, #20]
	cmp r3, #0
	beq .L_080d257a
	ldr r3, [r7]
	movs r4, #1
	ldr r3, [r3, #4]
	negs r4, r4
	str r4, [sp, #20]
	cmp r3, #1
	beq .L_080d257a
	movs r6, #1
	str r6, [sp, #20]
.L_080d257a:
	ldr r0, [sp, #56]
	cmp r0, #1
	bne .L_080d25ae
	ldr r5, .L_080d2860
	add r5, r10
	ldr r3, [r5]
	add r6, sp, #60
	ldr r0, [r3, #8]
	adds r1, r6, #0
	bl EffectPosition_ApplyStepAndYOffset
	ldr r3, [r6]
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	str r3, [r6]
	movs r3, #66
	str r3, [r6, #4]
	ldr r3, [r5]
	ldr r3, [r3, #4]
	cmp r3, #1
	bne .L_080d25aa
	movs r3, #76
	b .L_080d25ac
.L_080d25aa:
	movs r3, #44
.L_080d25ac:
	str r3, [r6]
.L_080d25ae:
	ldr r2, .L_080d2864
	movs r1, #212
	lsls r1, r1, #16
	ldr r3, .L_080d2868
	str r2, [sp, #28]
	str r1, [sp, #24]
	movs r2, #1
	movs r7, #0
	negs r2, r2
	add r3, r10
.L_080d25c2:
	adds r7, #1
	str r2, [r3]
	adds r3, #28
	cmp r7, #64
	bne .L_080d25c2
	ldr r5, .L_080d286c
	movs r7, #0
	movs r6, #127
	add r5, r10
.L_080d25d4:
	ldr r3, [sp, #20]
	cmp r3, #1
	bne .L_080d25e4
	bl Random16
	ands r0, r6
	adds r0, #128
	b .L_080d25ec
.L_080d25e4:
	bl Random16
	ands r0, r6
	subs r0, #128
.L_080d25ec:
	str r0, [r5]
	bl Random16
	movs r3, #7
	ands r3, r0
	subs r3, #72
	str r3, [r5, #4]
	bl Random16
	movs r3, #31
	ands r3, r0
	negs r3, r3
	adds r7, #1
	str r3, [r5, #24]
	adds r5, #28
	cmp r7, #16
	bne .L_080d25d4
	movs r1, #1
	movs r2, #128
	ldr r3, .L_080d2870
	movs r7, #0
	negs r1, r1
	lsls r2, r2, #2
.L_080d261a:
	adds r7, #1
	str r1, [r3]
	adds r3, #28
	cmp r7, r2
	bne .L_080d261a
	ldr r4, [sp, #56]
	cmp r4, #0
	bne .L_080d2644
	ldr r3, .L_080d2860
	add r3, r10
	ldr r0, [r3]
	bl BattleFx_SelectLivingTargets
	movs r0, #1
	bl WaitFrames
	ldr r1, .L_080d2874
	movs r0, #8
	movs r2, #2
	bl BattleFx_SpawnObjects
.L_080d2644:
	ldr r0, [sp, #48]
	movs r6, #0
	adds r0, #12
	str r6, [sp, #36]
	str r0, [sp, #12]
.L_080d264e:
	ldr r3, .L_080d2878
	ldr r3, [r3]
	movs r2, #3
	ands r3, r2
	cmp r3, #0
	beq .L_080d26da
	ldr r1, [sp, #36]
	cmp r1, #48
	ble .L_080d26da
	cmp r1, #159
	bgt .L_080d26da
	ldr r2, [sp, #56]
	cmp r2, #0
	bne .L_080d269a
	ldr r3, .L_080d287c
	add r3, r10
	ldr r0, [r3]
	movs r1, #8
	bl Object_InitializeMode
	ldr r3, .L_080d2880
	add r3, r10
	ldr r0, [r3]
	movs r1, #9
	bl Object_InitializeMode
	ldr r3, .L_080d2884
	add r3, r10
	ldr r0, [r3]
	movs r1, #10
	bl Object_InitializeMode
	ldr r3, .L_080d2888
	add r3, r10
	ldr r0, [r3]
	movs r1, #11
	bl Object_InitializeMode
.L_080d269a:
	ldr r3, .L_080d2860
	mov r4, r10
	ldr r3, [r4, r3]
	ldr r3, [r3, #20]
	movs r7, #0
	cmp r3, #0
	beq .L_080d26d6
	ldr r5, .L_080d2860
	movs r6, #36
	add r5, r10
.L_080d26ae:
	ldr r3, [r5]
	ldrsh r0, [r3, r6]
	movs r3, #0
	str r3, [sp, #0]
	movs r2, #5
	movs r1, #10
	subs r3, #1
	bl ObjectGroup_UpdateMembers
	ldr r3, [r5]
	movs r1, #4
	ldrsh r0, [r3, r6]
	bl BattleMotion_ApplyVariantMotionFar
	ldr r3, [r5]
	ldr r3, [r3, #20]
	adds r7, #1
	adds r6, #2
	cmp r7, r3
	bne .L_080d26ae
.L_080d26d6:
	movs r3, #160
	str r3, [sp, #36]
.L_080d26da:
	bl Render_ResetTransformState
	ldr r0, [sp, #48]
	ldr r1, [sp, #12]
	bl Graphics_PrepareTransferInIwramWork
	ldr r4, [sp, #36]
	cmp r4, #178
	bne .L_080d26f2
	movs r0, #134
	bl BattleEventRuntime_BeginPhaseFar
.L_080d26f2:
	ldr r6, [sp, #36]
	cmp r6, #128
	bne .L_080d2700
	ldr r2, .L_080d2858
	movs r3, #50
	add r2, r10
	str r3, [r2]
.L_080d2700:
	ldr r0, [sp, #36]
	cmp r0, #176
	bne .L_080d2730
	movs r2, #239
	lsls r2, r2, #7
	add r2, r10
	movs r3, #3
	str r3, [r2]
	ldr r2, .L_080d2858
	ldr r3, .L_080d288c
	add r2, r10
	str r3, [r2]
	ldr r0, .L_080d284c
	bl Resource_GetTableEntry
	adds r5, r0, #0
	movs r0, #160
	ldr r3, .L_080d2848
	lsls r0, r0, #19
	adds r1, r5, #0
	movs r2, #128
	bl _call_via_r3
	b .L_080d275e
.L_080d2730:
	ldr r3, [sp, #36]
	subs r3, #160
	cmp r3, #15
	bhi .L_080d275e
	movs r3, #239
	lsls r3, r3, #7
	add r3, r10
	movs r2, #1
	str r2, [r3]
	ldr r2, .L_080d2858
	ldr r3, .L_080d2890
	add r2, r10
	str r3, [r2]
	ldr r1, [sp, #36]
	cmp r1, #173
	ble .L_080d2754
	ldr r3, .L_080d2894
	str r3, [r2]
.L_080d2754:
	movs r0, #2
	movs r1, #2
	movs r2, #2
	bl Palette_BrightenBgEntries
.L_080d275e:
	ldr r3, [sp, #36]
	subs r3, #33
	cmp r3, #142
	bhi .L_080d27e6
	ldr r4, [sp, #36]
	movs r2, #0
	movs r3, #1
	mov r8, r2
	str r3, [sp, #16]
	cmp r4, #103
	ble .L_080d2778
	movs r6, #8
	str r6, [sp, #16]
.L_080d2778:
	ldr r1, .L_080d2898
	movs r0, #127
	ldr r6, .L_080d289c
	movs r7, #0
	mov r11, r0
	mov r9, r1
.L_080d2784:
	movs r2, #1
	ldr r3, [r6, #24]
	negs r2, r2
	cmp r3, r2
	bne .L_080d27da
	bl Random16
	movs r3, #255
	ands r3, r0
	subs r3, #32
	lsls r3, r3, #16
	str r3, [r6]
	movs r3, #224
	lsls r3, r3, #15
	str r3, [r6, #4]
	bl Random16
	movs r5, #3
	mov r4, r9
	mov r3, r11
	ands r5, r7
	ands r0, r3
	ldrb r3, [r4, r5]
	adds r0, r0, r3
	lsls r0, r0, #9
	str r0, [r6, #12]
	bl Random16
	mov r2, r9
	ldrb r3, [r2, r5]
	mov r1, r11
	ands r0, r1
	adds r0, r0, r3
	negs r0, r0
	movs r3, #0
	lsls r0, r0, #11
	str r3, [r6, #24]
	str r0, [r6, #16]
	movs r3, #1
	ldr r4, [sp, #16]
	add r8, r3
	cmp r8, r4
	beq .L_080d27e6
.L_080d27da:
	movs r0, #128
	adds r7, #1
	lsls r0, r0, #2
	adds r6, #28
	cmp r7, r0
	bne .L_080d2784
.L_080d27e6:
	ldr r3, [sp, #36]
	subs r3, #41
	cmp r3, #86
	bls .L_080d27f0
	b .L_080d28f8
.L_080d27f0:
	ldr r2, [sp, #36]
	movs r3, #1
	movs r1, #0
	ands r3, r2
	mov r11, r1
	cmp r3, #0
	beq .L_080d28f8
	movs r3, #60
	ldr r5, .L_080d28a0
	add r3, sp
	movs r7, #0
	mov r9, r3
	add r5, r10
.L_080d280a:
	movs r4, #1
	ldr r3, [r5, #24]
	negs r4, r4
	cmp r3, r4
	bne .L_080d28f0
	bl Random16
	movs r3, #255
	ands r3, r0
	adds r3, #128
	mov r8, r3
	bl Random16
	ldr r3, .L_080d28a4
	ldr r1, [sp, #56]
	ands r3, r0
	ldr r0, .L_080d28a8
	adds r6, r3, r0
	cmp r1, #0
	bne .L_080d28ac
	bl Random16
	movs r2, #7
	ands r0, r2
	adds r0, #78
	movs r3, #140
	lsls r0, r0, #16
	lsls r3, r3, #15
	str r0, [r5]
	b .L_080d28c4
	.2byte 0x0000
.L_080d2848:
	.4byte IwramCopyWords
.L_080d284c:
	.4byte 0x000000b4
.L_080d2850:
	.4byte 0x00000073
.L_080d2854:
	.4byte 0x000000c4
.L_080d2858:
	.4byte 0x00007784
.L_080d285c:
	.4byte BattlePresentation_ProcessPendingGraphicsTransfer
.L_080d2860:
	.4byte 0x00007828
.L_080d2864:
	.4byte 0xffc40000
.L_080d2868:
	.4byte 0x00007098
.L_080d286c:
	.4byte 0x00007320
.L_080d2870:
	.4byte gMapCellBuffer + 0x18
.L_080d2874:
	.4byte 0x00000179
.L_080d2878:
	.4byte gKeysRepeat
.L_080d287c:
	.4byte 0x000077d8
.L_080d2880:
	.4byte 0x000077dc
.L_080d2884:
	.4byte 0x000077e4
.L_080d2888:
	.4byte 0x000077e8
.L_080d288c:
	.4byte gMapBlocks + 0x202
.L_080d2890:
	.4byte 0x10101010
.L_080d2894:
	.4byte 0x3f3f3f3f
.L_080d2898:
	.4byte Data_080ee17e + 0x6
.L_080d289c:
	.4byte gMapCellBuffer
.L_080d28a0:
	.4byte 0x000074e0
.L_080d28a4:
	.4byte 0x00001fff
.L_080d28a8:
	.4byte 0x00004e20
.L_080d28ac:
	bl Random16
	movs r3, #7
	mov r4, r9
	ands r0, r3
	ldr r3, [r4]
	adds r0, r0, r3
	subs r0, #8
	lsls r0, r0, #16
	str r0, [r5]
	ldr r3, [r4, #4]
	lsls r3, r3, #16
.L_080d28c4:
	str r3, [r5, #4]
	adds r0, r6, #0
	bl Trig_Sin
	mov r3, r8
	muls r3, r0
	asrs r3, r3, #9
	str r3, [r5, #12]
	adds r0, r6, #0
	bl Trig_Cos
	mov r3, r8
	muls r3, r0
	movs r6, #1
	asrs r3, r3, #9
	add r11, r6
	str r3, [r5, #16]
	mov r0, r11
	movs r3, #0
	str r3, [r5, #24]
	cmp r0, #1
	beq .L_080d28f8
.L_080d28f0:
	adds r7, #1
	adds r5, #28
	cmp r7, #24
	bne .L_080d280a
.L_080d28f8:
	ldr r1, [sp, #36]
	cmp r1, #48
	bne .L_080d2904
	movs r0, #141
	bl AudioCommand_PlayFar
.L_080d2904:
	ldr r2, [sp, #36]
	cmp r2, #128
	bne .L_080d2910
	movs r0, #145
	bl AudioCommand_PlayFar
.L_080d2910:
	ldr r3, [sp, #36]
	subs r3, #129
	cmp r3, #46
	bhi .L_080d299e
	movs r4, #60
	movs r5, #225
	movs r3, #0
	add r4, sp
	lsls r5, r5, #7
	mov r11, r3
	movs r7, #0
	mov r9, r4
	add r5, r10
.L_080d292a:
	movs r6, #1
	ldr r3, [r5, #24]
	negs r6, r6
	cmp r3, r6
	bne .L_080d2996
	bl Random16
	movs r3, #255
	ands r3, r0
	adds r3, #128
	mov r8, r3
	bl Random16
	ldr r3, .L_080d2c5c
	ldr r1, [sp, #56]
	ands r3, r0
	ldr r0, .L_080d2c60
	adds r6, r3, r0
	cmp r1, #0
	bne .L_080d295e
	movs r3, #136
	lsls r3, r3, #15
	str r3, [r5]
	movs r3, #128
	lsls r3, r3, #15
	b .L_080d296a
.L_080d295e:
	mov r2, r9
	ldr r3, [r2]
	lsls r3, r3, #16
	str r3, [r5]
	ldr r3, [r2, #4]
	lsls r3, r3, #16
.L_080d296a:
	str r3, [r5, #4]
	adds r0, r6, #0
	bl Trig_Sin
	mov r3, r8
	muls r3, r0
	asrs r3, r3, #6
	str r3, [r5, #12]
	adds r0, r6, #0
	bl Trig_Cos
	mov r3, r8
	muls r3, r0
	asrs r3, r3, #6
	str r3, [r5, #16]
	movs r3, #0
	str r3, [r5, #24]
	movs r3, #1
	add r11, r3
	mov r4, r11
	cmp r4, #1
	beq .L_080d299e
.L_080d2996:
	adds r7, #1
	adds r5, #28
	cmp r7, #24
	bne .L_080d292a
.L_080d299e:
	ldr r6, [sp, #36]
	cmp r6, #175
	bgt .L_080d2a02
	movs r5, #225
	lsls r5, r5, #7
	movs r7, #0
	add r5, r10
.L_080d29ac:
	ldr r1, [r5, #24]
	cmp r1, #0
	blt .L_080d29fa
	movs r0, #2
	ldrsh r2, [r5, r0]
	movs r4, #6
	ldrsh r3, [r5, r4]
	movs r0, #32
	asrs r1, r1, #2
	str r0, [sp, #0]
	lsls r1, r1, #11
	movs r0, #64
	subs r2, #16
	subs r3, #32
	str r0, [sp, #4]
	add r1, r10
	ldr r0, [sp, #52]
	ldr r6, [sp, #40]
	bl _call_via_r6
	ldr r3, [r5, #12]
	ldr r0, [sp, #20]
	adds r2, r0, #0
	muls r2, r3
	ldr r3, [r5]
	adds r3, r3, r2
	str r3, [r5]
	ldr r2, [r5, #16]
	ldr r3, [r5, #4]
	adds r3, r3, r2
	str r3, [r5, #4]
	ldr r3, [r5, #24]
	adds r3, #1
	str r3, [r5, #24]
	cmp r3, #24
	bne .L_080d29fa
	movs r3, #1
	negs r3, r3
	str r3, [r5, #24]
.L_080d29fa:
	adds r7, #1
	adds r5, #28
	cmp r7, #24
	bne .L_080d29ac
.L_080d2a02:
	ldr r5, .L_080d2c64
	movs r7, #0
	movs r6, #1
	add r5, r10
.L_080d2a0a:
	ldr r3, [r5, #24]
	cmp r3, #0
	blt .L_080d2a56
	movs r4, #6
	ldrsh r3, [r5, r4]
	movs r1, #2
	ldrsh r2, [r5, r1]
	movs r1, #2
	subs r3, #1
	str r1, [sp, #4]
	str r6, [sp, #0]
	ldr r1, .L_080d2c68
	ldr r0, [sp, #52]
	ldr r4, [sp, #44]
	bl _call_via_r4
	ldr r3, [r5, #12]
	ldr r0, [sp, #20]
	adds r2, r0, #0
	muls r2, r3
	ldr r3, [r5]
	adds r3, r3, r2
	str r3, [r5]
	ldr r2, [r5, #16]
	ldr r3, [r5, #4]
	adds r3, r3, r2
	str r3, [r5, #4]
	ldr r1, .L_080d2c6c
	ldr r3, [r5, #24]
	adds r2, r2, r1
	adds r3, #1
	str r2, [r5, #16]
	str r3, [r5, #24]
	cmp r3, #48
	bne .L_080d2a56
	movs r3, #1
	negs r3, r3
	str r3, [r5, #24]
.L_080d2a56:
	adds r7, #1
	adds r5, #28
	cmp r7, #24
	bne .L_080d2a0a
	ldr r2, [sp, #36]
	cmp r2, #175
	bgt .L_080d2af6
	ldr r5, .L_080d2c70
	movs r7, #0
.L_080d2a68:
	ldr r3, [r5, #24]
	cmp r3, #0
	blt .L_080d2aea
	ldr r2, .L_080d2c74
	movs r3, #3
	ands r3, r7
	ldrb r0, [r2, r3]
	ldr r2, .L_080d2c78
	lsls r4, r0, #1
	subs r3, r4, #2
	ldrh r1, [r2, r3]
	ldr r3, [sp, #32]
	movs r6, #2
	ldrsh r2, [r5, r6]
	adds r1, r3, r1
	lsrs r3, r0, #1
	subs r2, r2, r3
	movs r6, #6
	ldrsh r3, [r5, r6]
	str r0, [sp, #0]
	subs r3, r3, r0
	str r4, [sp, #4]
	ldr r0, [sp, #52]
	ldr r4, [sp, #40]
	bl _call_via_r4
	ldr r1, [r5, #12]
	ldr r6, [sp, #20]
	adds r2, r6, #0
	muls r2, r1
	ldr r3, [r5]
	adds r3, r3, r2
	str r3, [r5]
	ldr r2, [r5, #16]
	ldr r3, [r5, #4]
	adds r3, r3, r2
	str r3, [r5, #4]
	ldr r0, [sp, #36]
	cmp r0, #128
	ble .L_080d2ad0
	movs r3, #1
	ands r3, r7
	cmp r3, #0
	beq .L_080d2ac8
	ldr r2, .L_080d2c7c
	adds r3, r1, r2
	str r3, [r5, #12]
	b .L_080d2ad6
.L_080d2ac8:
	ldr r4, .L_080d2c80
	adds r3, r1, r4
	str r3, [r5, #12]
	b .L_080d2ad6
.L_080d2ad0:
	ldr r6, .L_080d2c6c
	adds r3, r2, r6
	str r3, [r5, #16]
.L_080d2ad6:
	ldr r3, [r5, #24]
	movs r0, #128
	adds r3, #1
	lsls r0, r0, #1
	str r3, [r5, #24]
	cmp r3, r0
	bne .L_080d2aea
	movs r3, #1
	negs r3, r3
	str r3, [r5, #24]
.L_080d2aea:
	movs r1, #128
	adds r7, #1
	lsls r1, r1, #2
	adds r5, #28
	cmp r7, r1
	bne .L_080d2a68
.L_080d2af6:
	ldr r2, [sp, #36]
	cmp r2, #128
	bne .L_080d2b04
	ldr r2, .L_080d2c84
	movs r3, #48
	add r2, r10
	str r3, [r2]
.L_080d2b04:
	ldr r3, [sp, #56]
	cmp r3, #0
	bne .L_080d2b18
	ldr r4, [sp, #36]
	cmp r4, #48
	bne .L_080d2b18
	ldr r2, .L_080d2c84
	movs r3, #8
	add r2, r10
	str r3, [r2]
.L_080d2b18:
	ldr r3, [sp, #36]
	subs r3, #40
	cmp r3, #7
	bhi .L_080d2b32
	ldr r6, [sp, #24]
	ldr r1, [sp, #28]
	ldr r0, .L_080d2c88
	movs r2, #128
	lsls r2, r2, #13
	adds r0, r6, r0
	adds r2, r1, r2
	str r0, [sp, #24]
	str r2, [sp, #28]
.L_080d2b32:
	ldr r3, [sp, #56]
	cmp r3, #0
	bne .L_080d2bae
	ldr r4, [sp, #36]
	cmp r4, #128
	bne .L_080d2b6e
	ldr r3, .L_080d2c8c
	add r3, r10
	ldr r0, [r3]
	movs r1, #8
	bl Object_InitializeMode
	ldr r3, .L_080d2c90
	add r3, r10
	ldr r0, [r3]
	movs r1, #9
	bl Object_InitializeMode
	ldr r3, .L_080d2c94
	add r3, r10
	ldr r0, [r3]
	movs r1, #10
	bl Object_InitializeMode
	ldr r3, .L_080d2c98
	add r3, r10
	ldr r0, [r3]
	movs r1, #11
	bl Object_InitializeMode
.L_080d2b6e:
	ldr r6, [sp, #36]
	cmp r6, #176
	bne .L_080d2ba4
	ldr r3, .L_080d2c8c
	add r3, r10
	ldr r0, [r3]
	movs r1, #0
	bl Object_InitializeMode
	ldr r3, .L_080d2c90
	add r3, r10
	ldr r0, [r3]
	movs r1, #1
	bl Object_InitializeMode
	ldr r3, .L_080d2c94
	add r3, r10
	ldr r0, [r3]
	movs r1, #3
	bl Object_InitializeMode
	ldr r3, .L_080d2c98
	add r3, r10
	ldr r0, [r3]
	movs r1, #4
	bl Object_InitializeMode
.L_080d2ba4:
	movs r0, #3
	ldr r1, [sp, #24]
	ldr r2, [sp, #28]
	bl BattleFx_PlaceFormationObjects
.L_080d2bae:
	ldr r0, [sp, #36]
	cmp r0, #138
	bne .L_080d2bf0
	ldr r3, .L_080d2c9c
	mov r1, r10
	ldr r3, [r1, r3]
	ldr r3, [r3, #20]
	movs r7, #0
	cmp r3, #0
	beq .L_080d2bf0
	ldr r5, .L_080d2c9c
	movs r6, #36
	add r5, r10
.L_080d2bc8:
	ldr r3, [r5]
	ldrsh r0, [r3, r6]
	movs r3, #0
	str r3, [sp, #0]
	movs r1, #10
	movs r2, #5
	subs r3, #1
	bl ObjectGroup_UpdateMembers
	ldr r3, [r5]
	movs r1, #4
	ldrsh r0, [r3, r6]
	bl BattleMotion_ApplyVariantMotionFar
	ldr r3, [r5]
	ldr r3, [r3, #20]
	adds r7, #1
	adds r6, #2
	cmp r7, r3
	bne .L_080d2bc8
.L_080d2bf0:
	ldr r6, [sp, #36]
	cmp r6, #175
	ble .L_080d2bf8
	b .L_080d2d16
.L_080d2bf8:
	ldr r5, .L_080d2ca0
	movs r0, #0
	movs r1, #20
	movs r2, #5
	movs r7, #0
	mov r11, r0
	mov r8, r1
	mov r9, r2
	add r5, r10
.L_080d2c0a:
	ldr r6, [r5, #4]
	cmp r6, #55
	ble .L_080d2cb4
	ldr r3, [r5, #24]
	cmp r3, #11
	bhi .L_080d2c4c
	lsrs r4, r3, #31
	adds r4, r3, r4
	asrs r4, r4, #1
	ldr r2, .L_080d2ca4
	lsls r3, r4, #1
	ldrh r1, [r2, r3]
	movs r3, #192
	lsls r3, r3, #6
	add r1, r10
	adds r1, r1, r3
	ldr r3, .L_080d2ca8
	ldrb r0, [r3, r4]
	ldr r2, [r5]
	lsrs r3, r0, #1
	subs r2, r2, r3
	ldr r3, .L_080d2cac
	ldrb r3, [r3, r4]
	str r0, [sp, #0]
	ldr r0, .L_080d2cb0
	ldrb r0, [r0, r4]
	adds r3, r6, r3
	str r0, [sp, #4]
	ldr r4, [sp, #40]
	ldr r0, [sp, #52]
	bl _call_via_r4
	ldr r3, [r5, #24]
.L_080d2c4c:
	adds r3, #1
	str r3, [r5, #24]
	cmp r3, #12
	bne .L_080d2d0c
	mov r6, r11
	str r6, [r5, #24]
	b .L_080d2d0c
	.2byte 0x0000
.L_080d2c5c:
	.4byte 0x00001fff
.L_080d2c60:
	.4byte 0xffffb1e0
.L_080d2c64:
	.4byte 0x000074e0
.L_080d2c68:
	.4byte Data_080ee17e + 0xa
.L_080d2c6c:
	.4byte 0xfffffc00
.L_080d2c70:
	.4byte gMapCellBuffer
.L_080d2c74:
	.4byte Data_080ee17e + 0xc
.L_080d2c78:
	.4byte ParticleStreams_CellOffsets
.L_080d2c7c:
	.4byte 0xffff8000
.L_080d2c80:
	.4byte 0xffffe000
.L_080d2c84:
	.4byte 0x000077a8
.L_080d2c88:
	.4byte 0xfff80000
.L_080d2c8c:
	.4byte 0x000077d8
.L_080d2c90:
	.4byte 0x000077dc
.L_080d2c94:
	.4byte 0x000077e4
.L_080d2c98:
	.4byte 0x000077e8
.L_080d2c9c:
	.4byte 0x00007828
.L_080d2ca0:
	.4byte 0x00007320
.L_080d2ca4:
	.4byte Data_080ee17e + 0x22
.L_080d2ca8:
	.4byte Data_080ee17e + 0x10
.L_080d2cac:
	.4byte Data_080ee17e + 0x1c
.L_080d2cb0:
	.4byte Data_080ee17e + 0x16
.L_080d2cb4:
	ldr r3, [r5, #24]
	cmp r3, #0
	bne .L_080d2d08
	ldr r0, [sp, #20]
	lsls r3, r0, #1
	ldr r2, [r5]
	adds r3, r3, r0
	lsls r3, r3, #1
	subs r2, r2, r3
	adds r3, r6, #6
	str r2, [r5]
	str r3, [r5, #4]
	ldr r1, [sp, #36]
	movs r4, #10
	cmp r1, #47
	bgt .L_080d2ce2
	cmp r3, #55
	ble .L_080d2ce2
	movs r0, #136
	str r4, [sp, #8]
	bl AudioCommand_PlayFar
	ldr r4, [sp, #8]
.L_080d2ce2:
	ldr r2, .L_080d2d88
	mov r3, r8
	subs r3, #2
	ldrh r1, [r2, r3]
	ldr r2, [sp, #32]
	adds r1, r2, r1
	ldr r2, [r5]
	mov r3, r9
	subs r2, r2, r3
	ldr r3, [r5, #4]
	str r4, [sp, #0]
	mov r4, r8
	adds r3, #30
	str r4, [sp, #4]
	ldr r0, [sp, #52]
	ldr r6, [sp, #40]
	bl _call_via_r6
	b .L_080d2d0c
.L_080d2d08:
	adds r3, #1
	str r3, [r5, #24]
.L_080d2d0c:
	adds r7, #1
	adds r5, #28
	cmp r7, #16
	beq .L_080d2d16
	b .L_080d2c0a
.L_080d2d16:
	movs r0, #8
	movs r1, #8
	bl Camera_ApplyShake
	bl ObjectGroup_TickMemberTimers
	ldr r2, .L_080d2d8c
	movs r3, #1
	add r2, r10
	movs r0, #1
	str r3, [r2]
	bl WaitFrames
	ldr r0, [sp, #36]
	adds r0, #1
	str r0, [sp, #36]
	cmp r0, #208
	beq .L_080d2d3c
	b .L_080d264e
.L_080d2d3c:
	ldr r0, .L_080d2d90
	bl Scheduler_RemoveCallback
	movs r0, #47
	bl Runtime_ReleaseHeapBlock
	movs r0, #46
	bl Runtime_ReleaseHeapBlock
	ldr r1, [sp, #56]
	cmp r1, #0
	bne .L_080d2d70
	movs r0, #3
	ldr r1, [sp, #24]
	ldr r2, [sp, #28]
	bl BattleEffect_RunImpactBurst
	ldr r5, .L_080d2d94
	movs r7, #0
	add r5, r10
.L_080d2d64:
	ldmia r5!, {r0}
	adds r7, #1
	bl ResourceObject_ReleaseFar
	cmp r7, #8
	bne .L_080d2d64
.L_080d2d70:
	bl BattleFx_EndCanvasLayer
	add sp, #72
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
.L_080d2d88:
	.4byte ParticleStreams_CellOffsets
.L_080d2d8c:
	.4byte 0x00007824
.L_080d2d90:
	.4byte BattlePresentation_ProcessPendingGraphicsTransfer
.L_080d2d94:
	.4byte 0x000077d8
