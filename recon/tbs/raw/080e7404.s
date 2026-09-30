.syntax unified
	.thumb
	.global BattleEffect_RunParticleStreams
	.thumb_func
BattleEffect_RunParticleStreams:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #284
	str r1, [sp, #76]
	ldr r3, .L_080e7478
	mov r2, sp
	ldr r1, [r3]
	adds r2, #148
	str r2, [sp, #60]
	str r1, [sp, #72]
	subs r3, #4
	ldr r3, [r3]
	ldr r4, .L_080e747c
	str r3, [r2]
	adds r3, r3, r4
	str r0, [r3]
	movs r0, #128
	lsls r0, r0, #6
	bl BattleFx_BeginCanvasLayer
	ldr r2, .L_080e7480
	ldr r3, .L_080e7474
	strh r3, [r2]
	ldr r0, [sp, #76]
	cmp r0, #1
	bne .L_080e74b8
	ldr r1, [sp, #60]
	ldr r2, .L_080e747c
	ldr r3, [r1]
	adds r3, r3, r2
	ldr r3, [r3]
	ldr r0, [r3, #8]
	bl GetBattleObjectSlotFar
	movs r3, #160
	ldr r2, [r0]
	lsls r3, r3, #12
	str r3, [r2, #40]
	ldr r3, .L_080e7484
	str r3, [r2, #72]
	ldr r4, [sp, #60]
	ldr r0, .L_080e747c
	ldr r3, [r4]
	adds r3, r3, r0
	ldr r3, [r3]
	movs r5, #1
	ldr r0, [r3, #8]
	negs r5, r5
	movs r3, #0
	b .L_080e7488
	.2byte 0x0000
.L_080e7474:
	.4byte 0x00000100
.L_080e7478:
	.4byte Data_03001ef0
.L_080e747c:
	.4byte 0x00007828
.L_080e7480:
	.4byte 0x04000020
.L_080e7484:
	.4byte 0x000091eb
.L_080e7488:
	str r3, [sp, #0]
	adds r1, r5, #0
	movs r2, #2
	adds r3, r5, #0
	bl ObjectGroup_UpdateMembers
	movs r0, #145
	bl Func_080f9010
	ldr r1, [sp, #60]
	ldr r2, .L_080e74b4
	ldr r3, [r1]
	adds r3, r3, r2
	ldr r3, [r3]
	ldr r4, [sp, #76]
	ldr r3, [r3, #4]
	str r4, [sp, #64]
	cmp r3, #1
	beq .L_080e74be
	str r5, [sp, #64]
	b .L_080e74be
	.2byte 0x0000
.L_080e74b4:
	.4byte 0x00007828
.L_080e74b8:
	movs r0, #1
	negs r0, r0
	str r0, [sp, #64]
.L_080e74be:
	bl BattlePres_ConfigureEffectDisplay
	ldr r2, .L_080e74fc
	movs r3, #160
	lsls r3, r3, #19
	strh r2, [r3]
	adds r3, #2
	strh r2, [r3]
	ldr r1, [sp, #60]
	movs r2, #239
	ldr r3, [r1]
	ldr r5, .L_080e7500
	lsls r2, r2, #7
	adds r3, r3, r2
	movs r1, #144
	movs r2, #0
	str r2, [r3]
	lsls r1, r1, #3
	adds r0, r5, #0
	bl Scheduler_AddOrUpdateCallback
	movs r0, #0
	movs r1, #0
	bl BattleEffect_WipeCanvas
	adds r0, r5, #0
	bl Scheduler_RemoveCallback
	ldr r3, [sp, #76]
	b .L_080e7504
	.2byte 0x0000
.L_080e74fc:
	.4byte 0x00000000
.L_080e7500:
	.4byte BattlePresentation_ProcessPendingGraphicsTransfer
.L_080e7504:
	cmp r3, #1
	bne .L_080e755c
	movs r4, #0
	ldr r5, .L_080e7550
	ldr r6, .L_080e7554
	mov r8, r4
.L_080e7510:
	adds r0, r6, #0
	bl GetBattleEffectObject
	ldr r1, [sp, #60]
	ldr r3, [r1]
	str r0, [r3, r5]
	cmp r0, #0
	beq .L_080e753c
	adds r2, r0, #0
	adds r2, #38
	movs r3, #0
	strb r3, [r2]
	movs r1, #2
	bl Object_InitializeMode
	ldr r2, [sp, #60]
	ldr r3, [r2]
	ldr r1, [r3, r5]
	ldrb r3, [r1, #9]
	movs r2, #12
	orrs r3, r2
	strb r3, [r1, #9]
.L_080e753c:
	movs r4, #1
	ldr r3, .L_080e7558
	add r8, r4
	mov r0, r8
	adds r5, #4
	adds r6, r6, r3
	cmp r0, #2
	bne .L_080e7510
	b .L_080e7566
	.2byte 0x0000
.L_080e7550:
	.4byte 0x000077d8
.L_080e7554:
	.4byte 0x000001e3
.L_080e7558:
	.4byte 0x00002001
.L_080e755c:
	ldr r1, .L_080e75ec
	movs r0, #1
	movs r2, #3
	bl BattleFx_SpawnObjects
.L_080e7566:
	ldr r2, [sp, #60]
	movs r3, #1
	ldr r1, [r2]
	ldr r0, .L_080e75f0
	movs r2, #1
	bl Resource_LoadAndDecompress
	ldr r3, [sp, #76]
	cmp r3, #1
	bne .L_080e758e
	ldr r0, .L_080e75f4
	bl Resource_GetTableEntry
	adds r1, r0, #0
	movs r0, #160
	ldr r3, .L_080e75f8
	lsls r0, r0, #19
	movs r2, #128
	bl _call_via_r3
.L_080e758e:
	movs r4, #140
	lsls r4, r4, #1
	add r4, sp
	ldr r3, .L_080e75fc
	mov r9, r4
	str r3, [r4]
	mov r0, r9
	ldr r3, .L_080e7600
	ldr r1, .L_080e7604
	ldr r2, .L_080e7608
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r2, #240
	ldr r3, .L_080e75f8
	lsls r2, r2, #7
	ldr r0, .L_080e760c
	bl _call_via_r3
	movs r0, #1
	bl WaitFrames
	ldr r2, .L_080e7610
	ldr r3, .L_080e75dc
	strh r3, [r2]
	ldr r3, .L_080e75e0
	subs r2, #48
	strh r3, [r2]
	ldr r3, .L_080e75e4
	subs r2, #22
	strh r3, [r2]
	ldr r3, .L_080e75e8
	adds r2, #2
	ldr r1, .L_080e7614
	movs r0, #0
	strh r3, [r2]
	mov r8, r0
	movs r7, #15
	mov r10, r1
	b .L_080e7618
.L_080e75dc:
	.4byte 0x00000000
.L_080e75e0:
	.4byte 0x00000100
.L_080e75e4:
	.4byte 0x00001f80
.L_080e75e8:
	.4byte 0x00002787
.L_080e75ec:
	.4byte 0x0000017d
.L_080e75f0:
	.4byte 0x000000c1
.L_080e75f4:
	.4byte 0x000000c4
.L_080e75f8:
	.4byte IwramCopyWords
.L_080e75fc:
	.4byte 0x01010101
.L_080e7600:
	.4byte 0x040000d4
.L_080e7604:
	.4byte gMapCellBuffer
.L_080e7608:
	.4byte 0x85002000
.L_080e760c:
	.4byte 0x06008000
.L_080e7610:
	.4byte 0x04000050
.L_080e7614:
	.4byte 0x05000100
.L_080e7618:
	bl Random16
	adds r6, r0, #0
	bl Random16
	adds r5, r0, #0
	bl Random16
	ands r5, r7
	ands r0, r7
	adds r5, #16
	adds r0, #16
	ands r6, r7
	lsls r0, r0, #10
	lsls r5, r5, #5
	adds r6, #16
	orrs r0, r5
	movs r4, #1
	orrs r0, r6
	mov r2, r10
	add r8, r4
	strh r0, [r2]
	movs r3, #2
	mov r0, r8
	add r10, r3
	cmp r0, #63
	bne .L_080e7618
	mov r1, r9
	movs r3, #0
	str r3, [r1]
	mov r0, r9
	ldr r3, .L_080e780c
	ldr r1, [sp, #72]
	ldr r2, .L_080e7810
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r2, #0
	movs r3, #127
	mov r8, r2
	mov r10, r3
	movs r7, #7
.L_080e766a:
	bl Random16
	mov r4, r10
	adds r6, r0, #0
	ands r6, r4
	bl Random16
	adds r5, r0, #0
	mov r0, r10
	ands r5, r0
	bl Random16
	movs r3, #63
	ands r3, r0
	adds r1, r3, #0
	adds r1, #64
	adds r3, r5, #0
	cmp r5, #0
	bge .L_080e7692
	adds r3, r5, #7
.L_080e7692:
	asrs r3, r3, #3
	adds r2, r6, #0
	cmp r6, #0
	bge .L_080e769c
	adds r2, r6, #7
.L_080e769c:
	asrs r2, r2, #3
	lsls r3, r3, #4
	adds r3, r3, r2
	ands r5, r7
	lsls r3, r3, #3
	adds r3, r3, r5
	ands r6, r7
	lsls r3, r3, #3
	ldr r2, [sp, #72]
	adds r3, r3, r6
	strb r1, [r2, r3]
	movs r4, #128
	movs r3, #1
	add r8, r3
	lsls r4, r4, #1
	cmp r8, r4
	bne .L_080e766a
	movs r2, #128
	ldr r1, [sp, #72]
	ldr r3, .L_080e7814
	lsls r2, r2, #7
	ldr r0, .L_080e7818
	bl _call_via_r3
	ldr r2, .L_080e781c
	movs r3, #240
	str r3, [r2, #16]
	ldr r0, [sp, #60]
	ldr r1, .L_080e7820
	ldr r3, [r0]
	adds r3, r3, r1
	ldr r0, [r3]
	bl BattleFx_SelectLivingTargets
	ldr r3, [sp, #60]
	ldr r4, .L_080e7824
	ldr r2, [r3]
	ldr r0, .L_080e7828
	adds r3, r2, r4
	movs r1, #0
	str r1, [r3]
	subs r4, #64
	adds r3, r2, r0
	str r1, [r3]
	subs r0, #64
	adds r3, r2, r4
	str r1, [r3]
	adds r1, r2, r0
	movs r3, #2
	str r3, [r1]
	ldr r4, [sp, #64]
	ldr r3, .L_080e782c
	adds r0, #8
	adds r1, r2, r3
	lsls r3, r4, #7
	adds r2, r2, r0
	str r3, [r1]
	mov r1, r8
	str r1, [r2]
	ldr r0, .L_080e7830
	ldr r1, .L_080e7834
	bl Scheduler_AddOrUpdateCallback
	movs r1, #144
	ldr r0, .L_080e7838
	lsls r1, r1, #3
	bl Scheduler_AddOrUpdateCallback
	add r2, sp, #152
	mov r10, r2
	movs r7, #63
	mov r5, r10
	add r6, sp, #280
.L_080e772e:
	bl Random16
	ands r0, r7
	strb r0, [r5]
	adds r5, #1
	cmp r5, r6
	bne .L_080e772e
	movs r3, #1
	movs r6, #0
	mov r8, r3
	movs r5, #0
.L_080e7744:
	mov r4, r8
	lsrs r3, r4, #31
	add r3, r8
	asrs r3, r3, #1
	movs r0, #4
	adds r6, r6, r3
	add r8, r0
	cmp r5, r6
	beq .L_080e77b4
	movs r1, #127
	movs r2, #0
	mov r7, r10
	movs r4, #7
	mov lr, r1
	mov r12, r2
.L_080e7762:
	movs r0, #0
.L_080e7764:
	mov r1, lr
	adds r3, r0, #0
	ands r3, r1
	ldrb r3, [r7, r3]
	subs r1, r5, r3
	cmp r1, #0
	blt .L_080e77a4
	cmp r1, #127
	bgt .L_080e77a4
	adds r2, r1, #0
	cmp r1, #0
	bge .L_080e777e
	adds r2, r1, #7
.L_080e777e:
	asrs r2, r2, #3
	adds r3, r0, #0
	cmp r0, #0
	bge .L_080e7788
	adds r3, r0, #7
.L_080e7788:
	asrs r3, r3, #3
	lsls r2, r2, #5
	adds r2, r2, r3
	ands r1, r4
	lsls r2, r2, #3
	adds r2, r2, r1
	adds r3, r0, #0
	ands r3, r4
	lsls r2, r2, #3
	adds r2, r2, r3
	ldr r3, .L_080e783c
	mov r1, r12
	adds r2, r2, r3
	strb r1, [r2]
.L_080e77a4:
	movs r2, #128
	adds r0, #1
	lsls r2, r2, #1
	cmp r0, r2
	bne .L_080e7764
	adds r5, #1
	cmp r5, r6
	bne .L_080e7762
.L_080e77b4:
	ldr r4, [sp, #60]
	ldr r0, .L_080e7840
	ldr r3, [r4]
	movs r2, #1
	adds r3, r3, r0
	str r2, [r3]
	movs r0, #1
	bl WaitFrames
	cmp r6, #191
	ble .L_080e7744
	ldr r2, .L_080e7844
	ldr r3, .L_080e7804
	strh r3, [r2]
	ldr r3, .L_080e7808
	adds r2, #2
	strh r3, [r2]
	ldr r2, .L_080e7848
	ldrh r1, [r2, #4]
	str r1, [sp, #56]
	ldrh r3, [r2, #6]
	ldr r5, .L_080e784c
	str r3, [sp, #52]
	ldr r4, [r5]
	movs r3, #0
	str r4, [sp, #48]
	strh r3, [r2, #4]
	movs r3, #32
	strh r3, [r2, #6]
	movs r3, #2
	str r3, [sp, #0]
	movs r1, #8
	movs r2, #7
	movs r3, #3
	movs r0, #46
	bl Unnamed_080ed408
	ldr r5, [r5, #8]
	ldr r0, [sp, #60]
	b .L_080e7850
.L_080e7804:
	.4byte 0x00003f42
.L_080e7808:
	.4byte 0x00001010
.L_080e780c:
	.4byte 0x040000d4
.L_080e7810:
	.4byte 0x85001000
.L_080e7814:
	.4byte IwramCopyWords
.L_080e7818:
	.4byte 0x06004000
.L_080e781c:
	.4byte gProjection
.L_080e7820:
	.4byte 0x00007828
.L_080e7824:
	.4byte 0x000077d0
.L_080e7828:
	.4byte 0x000077d4
.L_080e782c:
	.4byte 0x00007798
.L_080e7830:
	.4byte Camera_AdvanceBg2Reference
.L_080e7834:
	.4byte 0x000004ff
.L_080e7838:
	.4byte BattleFx_FlushPendingGraphicsTransfer
.L_080e783c:
	.4byte gMapCellBuffer
.L_080e7840:
	.4byte 0x00007824
.L_080e7844:
	.4byte 0x04000050
.L_080e7848:
	.4byte gBgScroll
.L_080e784c:
	.4byte gTransitionWork
.L_080e7850:
	str r5, [sp, #68]
	movs r3, #239
	ldr r2, [r0]
	lsls r3, r3, #7
	adds r1, r2, r3
	ldr r4, .L_080e798c
	movs r3, #3
	str r3, [r1]
	ldr r3, .L_080e7990
	adds r2, r2, r4
	str r3, [r2]
	ldr r1, .L_080e7994
	ldr r0, .L_080e7998
	bl Scheduler_AddOrUpdateCallback
	ldr r1, [sp, #60]
	ldr r4, .L_080e799c
	ldr r3, [r1]
	movs r0, #0
	movs r2, #1
	mov r8, r0
	negs r2, r2
	adds r3, r3, r4
.L_080e787e:
	movs r0, #1
	add r8, r0
	mov r1, r8
	str r2, [r3]
	adds r3, #28
	cmp r1, #64
	bne .L_080e787e
	ldr r2, [sp, #48]
	movs r3, #1
	str r3, [r2, #16]
	ldr r4, [sp, #60]
	ldr r0, .L_080e79a0
	ldr r3, [r4]
	movs r2, #0
	adds r3, r3, r0
	str r2, [r3]
	mov r11, r2
	mov r1, sp
	mov r2, sp
	adds r1, #132
	adds r2, #88
	movs r3, #0
	str r1, [sp, #24]
	str r2, [sp, #44]
	str r4, [sp, #40]
	str r3, [sp, #16]
.L_080e78b2:
	ldr r4, [sp, #60]
	ldr r1, .L_080e79a0
	ldr r0, [r4]
	adds r3, r0, r1
	ldr r2, [r3]
	adds r3, r2, #0
	cmp r3, #0
	bge .L_080e78c4
	adds r3, #3
.L_080e78c4:
	asrs r4, r3, #2
	movs r2, #252
	ldr r3, [sp, #76]
	lsls r2, r2, #5
	adds r5, r0, r2
	cmp r3, #1
	bne .L_080e78e6
	ldr r3, .L_080e79a4
	ldr r3, [r3]
	movs r2, #3
	ands r3, r2
	cmp r3, #0
	beq .L_080e78fa
	mov r0, r11
	cmp r0, #16
	ble .L_080e78fa
	b .L_080e7cba
.L_080e78e6:
	ldr r3, .L_080e79a4
	ldr r3, [r3]
	movs r2, #3
	ands r3, r2
	cmp r3, #0
	beq .L_080e78fa
	mov r1, r11
	cmp r1, #4
	ble .L_080e78fa
	b .L_080e7cba
.L_080e78fa:
	mov r2, r11
	cmp r2, #0
	bne .L_080e790a
	movs r0, #141
	str r4, [sp, #8]
	bl Func_080f9010
	ldr r4, [sp, #8]
.L_080e790a:
	movs r3, #0
	mov r8, r3
.L_080e790e:
	movs r0, #1
	add r8, r0
	mov r1, r8
	strh r3, [r5]
	adds r5, #2
	cmp r1, #15
	bne .L_080e790e
.L_080e791c:
	mov r1, r8
	subs r1, #16
	adds r3, r1, #0
	cmp r1, #0
	bge .L_080e792a
	mov r3, r8
	subs r3, #13
.L_080e792a:
	asrs r3, r3, #2
	adds r2, r3, r4
	adds r3, r2, #0
	adds r1, r2, #0
	subs r3, #32
	subs r1, #80
	cmp r3, #0
	bge .L_080e793c
	movs r3, #0
.L_080e793c:
	cmp r3, #31
	ble .L_080e7942
	movs r3, #31
.L_080e7942:
	cmp r1, #0
	bge .L_080e7948
	movs r1, #0
.L_080e7948:
	cmp r1, #31
	ble .L_080e794e
	movs r1, #31
.L_080e794e:
	lsls r2, r1, #5
	lsls r3, r3, #10
	orrs r3, r2
	asrs r2, r1, #1
	orrs r3, r2
	movs r2, #1
	add r8, r2
	strh r3, [r5]
	mov r3, r8
	adds r5, #2
	cmp r3, #135
	bne .L_080e791c
	ldr r3, .L_080e7988
.L_080e7968:
	movs r4, #1
	add r8, r4
	mov r0, r8
	strh r3, [r5]
	adds r5, #2
	cmp r0, #160
	bne .L_080e7968
	ldr r1, [sp, #64]
	cmp r1, #1
	bne .L_080e79a8
	mov r3, r11
	cmp r3, #0
	bge .L_080e7984
	adds r3, #3
.L_080e7984:
	asrs r7, r3, #2
	b .L_080e79b6
.L_080e7988:
	.4byte 0x00000000
.L_080e798c:
	.4byte 0x00007784
.L_080e7990:
	.4byte Data_02020202
.L_080e7994:
	.4byte 0x000004fe
.L_080e7998:
	.4byte BattleFx_ArmPaletteHBlankDma
.L_080e799c:
	.4byte 0x00007098
.L_080e79a0:
	.4byte 0x0000778c
.L_080e79a4:
	.4byte gKeysRepeat
.L_080e79a8:
	mov r2, r11
	cmp r2, #0
	bge .L_080e79b0
	adds r2, #3
.L_080e79b0:
	asrs r2, r2, #2
	movs r3, #64
	subs r7, r3, r2
.L_080e79b6:
	movs r2, #96
	mov r3, r11
	subs r3, r2, r3
	ldr r4, [sp, #24]
	mov r10, r3
	movs r3, #0
	str r3, [r4, #12]
	movs r3, #255
	lsls r3, r3, #16
	str r3, [r4, #4]
	ldr r0, [sp, #76]
	cmp r0, #1
	bne .L_080e7a1e
	ldr r1, [sp, #16]
	movs r2, #160
	ldr r3, [sp, #44]
	lsls r2, r2, #8
	adds r6, r1, r2
	str r6, [sp, #88]
	movs r4, #160
	str r6, [r3, #4]
	ldr r0, [sp, #24]
	lsls r4, r4, #15
	lsls r3, r7, #16
	adds r3, r3, r4
	str r3, [r0]
	mov r1, r10
	movs r3, #64
	subs r3, r3, r1
	lsls r3, r3, #16
	str r3, [r0, #8]
	ldr r2, [sp, #40]
	ldr r4, .L_080e7bdc
	ldr r3, [r2]
	adds r3, r3, r4
	ldr r0, [r3]
	ldr r1, [sp, #24]
	ldr r2, [sp, #44]
	movs r3, #0
	bl Object_ApplyProjectedPlacementFar
	ldr r0, [sp, #40]
	ldr r1, .L_080e7be0
	ldr r3, [r0]
	adds r3, r3, r1
	ldr r0, [r3]
	ldr r1, [sp, #24]
	ldr r2, [sp, #44]
	movs r3, #0
	bl Object_ApplyProjectedPlacementFar
	b .L_080e7a54
.L_080e7a1e:
	ldr r3, [sp, #16]
	movs r4, #128
	ldr r0, [sp, #44]
	lsls r4, r4, #9
	adds r6, r3, r4
	str r6, [sp, #88]
	movs r1, #192
	str r6, [r0, #4]
	lsls r1, r1, #15
	ldr r4, [sp, #24]
	lsls r3, r7, #16
	adds r3, r3, r1
	mov r0, r10
	str r3, [r4]
	subs r3, r2, r0
	lsls r3, r3, #16
	str r3, [r4, #8]
	ldr r1, [sp, #60]
	ldr r2, .L_080e7bdc
	ldr r3, [r1]
	adds r3, r3, r2
	ldr r0, [r3]
	ldr r1, [sp, #24]
	ldr r2, [sp, #44]
	movs r3, #0
	bl Object_ApplyProjectedPlacementFar
.L_080e7a54:
	movs r3, #0
	mov r4, r10
	mov r8, r3
	movs r3, #32
	subs r4, r3, r4
	mov r10, r4
	movs r2, #0
.L_080e7a62:
	ldr r0, [sp, #60]
	ldr r3, [r0]
	movs r1, #225
	adds r3, r3, r2
	lsls r1, r1, #7
	adds r5, r3, r1
	movs r4, #1
	ldr r3, [r5, #24]
	negs r4, r4
	cmp r3, r4
	bne .L_080e7ad2
	bl Random16
	ldr r3, .L_080e7be4
	ands r3, r0
	movs r0, #128
	lsls r0, r0, #7
	adds r1, r3, r0
	movs r3, #0
	str r3, [r5, #24]
	adds r0, r1, #0
	str r1, [sp, #12]
	bl Trig_Sin
	adds r3, r7, #0
	adds r3, #96
	lsls r2, r3, #16
	lsls r3, r0, #4
	subs r3, r3, r0
	lsls r3, r3, #1
	ldr r1, [sp, #12]
	cmp r3, #0
	bge .L_080e7aa8
	ldr r4, .L_080e7be8
	adds r3, r3, r4
.L_080e7aa8:
	asrs r3, r3, #16
	muls r3, r6
	adds r3, r2, r3
	str r3, [r5]
	adds r0, r1, #0
	bl Func_0800231c
	lsls r3, r0, #4
	subs r3, r3, r0
	mov r1, r10
	lsls r3, r3, #1
	lsls r2, r1, #16
	cmp r3, #0
	bge .L_080e7ac8
	ldr r4, .L_080e7be8
	adds r3, r3, r4
.L_080e7ac8:
	asrs r3, r3, #16
	muls r3, r6
	subs r3, r2, r3
	str r3, [r5, #4]
	b .L_080e7ade
.L_080e7ad2:
	movs r0, #1
	add r8, r0
	mov r1, r8
	adds r2, #28
	cmp r1, #32
	bne .L_080e7a62
.L_080e7ade:
	add r5, sp, #96
	movs r3, #0
	str r3, [r5]
	str r3, [r5, #4]
	movs r3, #128
	lsls r3, r3, #18
	str r3, [r5, #8]
	bl Render_ResetTransformState
	adds r0, r5, #0
	bl SceneTransform_ApplyPosition
	movs r0, #128
	lsls r0, r0, #4
	bl SceneTransform_ApplyRoll
	ldr r0, [sp, #16]
	bl SceneTransform_ApplyYaw
	movs r2, #0
	ldr r7, .L_080e7bec
	mov r8, r2
	add r6, sp, #120
	add r5, sp, #108
.L_080e7b0e:
	ldrh r3, [r7]
	lsls r3, r3, #16
	asrs r2, r3, #16
	lsrs r3, r3, #31
	adds r2, r2, r3
	movs r4, #2
	ldrsh r3, [r7, r4]
	add r3, r11
	lsls r3, r3, #16
	str r3, [r6, #4]
	ldrh r3, [r7, #4]
	asrs r2, r2, #1
	lsls r2, r2, #16
	lsls r3, r3, #16
	str r2, [r6]
	asrs r2, r3, #16
	lsrs r3, r3, #31
	adds r2, r2, r3
	asrs r2, r2, #1
	lsls r2, r2, #16
	adds r1, r5, #0
	str r2, [r6, #8]
	adds r0, r6, #0
	bl EffectPosition_ApplyBaseAndYOffset
	movs r0, #2
	ldrsh r2, [r5, r0]
	adds r3, r2, #0
	adds r3, #128
	str r3, [r5]
	movs r1, #6
	ldrsh r3, [r5, r1]
	adds r1, r3, #0
	adds r1, #60
	str r1, [r5, #4]
	ldr r4, [sp, #60]
	movs r0, #250
	ldr r1, [r4]
	lsls r0, r0, #5
	adds r1, r1, r0
	movs r0, #8
	str r0, [sp, #0]
	str r0, [sp, #4]
	adds r2, #124
	adds r3, #56
	ldr r0, .L_080e7bf0
	ldr r4, [sp, #68]
	bl _call_via_r4
	movs r0, #1
	add r8, r0
	mov r1, r8
	adds r7, #6
	cmp r1, #7
	bne .L_080e7b0e
	ldr r2, [sp, #64]
	cmp r2, #1
	bne .L_080e7b92
	mov r3, r11
	cmp r3, #0
	bge .L_080e7b8a
	adds r3, #3
.L_080e7b8a:
	asrs r3, r3, #2
	adds r7, r3, #0
	subs r7, #16
	b .L_080e7ba0
.L_080e7b92:
	mov r2, r11
	cmp r2, #0
	bge .L_080e7b9a
	adds r2, #3
.L_080e7b9a:
	asrs r2, r2, #2
	movs r3, #16
	subs r7, r3, r2
.L_080e7ba0:
	movs r3, #96
	negs r3, r3
	add r3, r11
	movs r4, #0
	ldr r5, .L_080e7bf4
	mov r10, r3
	mov r8, r4
.L_080e7bae:
	movs r0, #2
	ldrsh r3, [r5, r0]
	add r3, r10
	cmp r3, #93
	bgt .L_080e7bf8
	ldr r2, [sp, #60]
	ldr r1, [r2]
	movs r0, #0
	ldrsh r2, [r5, r0]
	movs r4, #228
	movs r0, #24
	lsls r4, r4, #5
	adds r2, r2, r7
	adds r1, r1, r4
	str r0, [sp, #0]
	str r0, [sp, #4]
	subs r2, #12
	subs r3, #12
	ldr r0, .L_080e7bf0
	ldr r4, [sp, #68]
	bl _call_via_r4
	b .L_080e7c10
.L_080e7bdc:
	.4byte 0x000077d8
.L_080e7be0:
	.4byte 0x000077dc
.L_080e7be4:
	.4byte 0x00007fff
.L_080e7be8:
	.4byte 0x0000ffff
.L_080e7bec:
	.4byte Data_080eee76
.L_080e7bf0:
	.4byte gMapCellBuffer
.L_080e7bf4:
	.4byte Data_080eeea0
.L_080e7bf8:
	cmp r3, #95
	bgt .L_080e7c10
	movs r1, #0
	ldrsh r0, [r5, r1]
	add r2, sp, #284
	adds r0, r0, r7
	mov r9, r2
	lsls r0, r0, #16
	lsls r1, r3, #16
	movs r2, #1
	bl Func_080e7338
.L_080e7c10:
	movs r3, #1
	add r8, r3
	mov r4, r8
	adds r5, #4
	cmp r4, #7
	bne .L_080e7bae
	ldr r1, [sp, #64]
	lsls r3, r1, #2
	movs r0, #0
	adds r3, r3, r1
	mov r8, r0
	lsls r7, r3, #14
	movs r6, #0
.L_080e7c2a:
	ldr r3, [sp, #60]
	ldr r2, [r3]
	movs r4, #225
	adds r3, r2, r6
	lsls r4, r4, #7
	adds r5, r3, r4
	ldr r1, [r5, #24]
	cmp r1, #0
	blt .L_080e7c78
	lsls r1, r1, #10
	movs r4, #6
	ldrsh r3, [r5, r4]
	adds r1, r2, r1
	movs r0, #2
	ldrsh r2, [r5, r0]
	movs r0, #32
	subs r3, #16
	str r0, [sp, #0]
	str r0, [sp, #4]
	subs r2, #16
	ldr r0, .L_080e7d38
	ldr r4, [sp, #68]
	bl _call_via_r4
	ldr r3, [r5]
	subs r3, r3, r7
	str r3, [r5]
	ldr r0, .L_080e7d3c
	ldr r3, [r5, #4]
	adds r3, r3, r0
	str r3, [r5, #4]
	ldr r3, [r5, #24]
	adds r3, #1
	str r3, [r5, #24]
	cmp r3, #6
	bne .L_080e7c78
	movs r3, #1
	negs r3, r3
	str r3, [r5, #24]
.L_080e7c78:
	movs r1, #1
	add r8, r1
	mov r2, r8
	adds r6, #28
	cmp r2, #32
	bne .L_080e7c2a
	ldr r4, [sp, #40]
	ldr r0, .L_080e7d40
	ldr r3, [r4]
	movs r2, #1
	adds r3, r3, r0
	str r2, [r3]
	movs r0, #1
	bl WaitFrames
	movs r2, #128
	ldr r1, [sp, #16]
	lsls r2, r2, #1
	adds r1, r1, r2
	str r1, [sp, #16]
	ldr r4, [sp, #40]
	ldr r0, .L_080e7d44
	ldr r2, [r4]
	movs r3, #1
	adds r2, r2, r0
	add r11, r3
	ldr r3, [r2]
	mov r1, r11
	adds r3, #1
	str r3, [r2]
	cmp r1, #192
	beq .L_080e7cba
	b .L_080e78b2
.L_080e7cba:
	movs r0, #1
	bl WaitFrames
	ldr r2, [sp, #48]
	movs r5, #0
	str r5, [r2, #16]
	ldr r0, .L_080e7d48
	bl Scheduler_RemoveCallback
	ldr r0, .L_080e7d4c
	bl Scheduler_RemoveCallback
	ldr r0, .L_080e7d50
	bl Scheduler_RemoveCallback
	add r4, sp, #56
	add r0, sp, #52
	ldr r3, .L_080e7d54
	ldrh r4, [r4]
	ldrh r0, [r0]
	strh r4, [r3, #4]
	strh r0, [r3, #6]
	movs r0, #46
	bl Runtime_ReleaseHeapBlock
	bl BattleEffect_SetupBlendedDisplay
	ldr r2, .L_080e7d58
	ldr r3, .L_080e7d2c
	strh r3, [r2]
	ldr r3, .L_080e7d5c
	str r5, [r3]
	ldr r3, .L_080e7d60
	adds r2, #12
	str r3, [r2]
	ldr r3, .L_080e7d30
	adds r2, #38
	strh r3, [r2]
	ldr r3, .L_080e7d34
	subs r2, #70
	strh r3, [r2]
	movs r3, #2
	str r3, [sp, #0]
	movs r1, #7
	movs r2, #7
	movs r3, #3
	movs r0, #46
	bl Unnamed_080ed408
	ldr r3, .L_080e7d64
	adds r3, #184
	ldr r3, [r3]
	ldr r2, [sp, #60]
	str r3, [sp, #68]
	ldr r0, .L_080e7d68
	ldr r1, [r2]
	b .L_080e7d6c
.L_080e7d2c:
	.4byte 0x00000080
.L_080e7d30:
	.4byte 0x00001010
.L_080e7d34:
	.4byte 0x00002784
.L_080e7d38:
	.4byte gMapCellBuffer
.L_080e7d3c:
	.4byte 0xfffb0000
.L_080e7d40:
	.4byte 0x00007824
.L_080e7d44:
	.4byte 0x0000778c
.L_080e7d48:
	.4byte Camera_AdvanceBg2Reference
.L_080e7d4c:
	.4byte BattleFx_ArmPaletteHBlankDma
.L_080e7d50:
	.4byte BattleFx_FlushPendingGraphicsTransfer
.L_080e7d54:
	.4byte gBgScroll
.L_080e7d58:
	.4byte 0x04000020
.L_080e7d5c:
	.4byte 0x04000028
.L_080e7d60:
	.4byte 0xfffff000
.L_080e7d64:
	.4byte Data_03001e50
.L_080e7d68:
	.4byte 0x000000c0
.L_080e7d6c:
	movs r3, #0
	movs r2, #1
	bl Resource_LoadAndDecompress
	movs r3, #0
	mov r8, r3
	movs r7, #127
	movs r6, #0
.L_080e7d7c:
	ldr r4, [sp, #60]
	ldr r5, [r4]
	movs r0, #225
	adds r5, r5, r6
	lsls r0, r0, #7
	adds r5, r5, r0
	bl Random16
	ands r0, r7
	str r0, [r5]
	bl Random16
	movs r1, #1
	ands r0, r7
	add r8, r1
	adds r0, #127
	mov r2, r8
	str r0, [r5, #4]
	adds r6, #28
	cmp r2, #32
	bne .L_080e7d7c
	movs r3, #0
	ldr r5, .L_080e80dc
	mov r8, r3
	movs r6, #0
	movs r7, #255
.L_080e7db0:
	str r6, [r5]
	str r6, [r5, #4]
	str r6, [r5, #8]
	bl Random16
	ands r0, r7
	subs r0, #127
	lsls r0, r0, #12
	str r0, [r5, #12]
	bl Random16
	ands r0, r7
	lsls r0, r0, #11
	str r0, [r5, #16]
	bl Random16
	ands r0, r7
	subs r0, #127
	movs r4, #1
	lsls r0, r0, #12
	add r8, r4
	str r0, [r5, #20]
	mov r0, r8
	str r6, [r5, #24]
	adds r5, #28
	cmp r0, #128
	bne .L_080e7db0
	movs r1, #0
	ldr r5, .L_080e80e0
	mov r8, r1
	movs r6, #0
	movs r7, #255
.L_080e7df0:
	str r6, [r5]
	str r6, [r5, #4]
	str r6, [r5, #8]
	bl Random16
	ands r0, r7
	subs r0, #128
	lsls r0, r0, #13
	str r0, [r5, #12]
	bl Random16
	ands r0, r7
	lsls r0, r0, #11
	str r0, [r5, #16]
	bl Random16
	ands r0, r7
	subs r0, #128
	movs r2, #1
	movs r3, #128
	lsls r0, r0, #13
	add r8, r2
	lsls r3, r3, #2
	str r0, [r5, #20]
	str r6, [r5, #24]
	adds r5, #28
	cmp r8, r3
	bne .L_080e7df0
	ldr r4, [sp, #60]
	movs r0, #239
	ldr r2, [r4]
	lsls r0, r0, #7
	adds r1, r2, r0
	movs r3, #1
	str r3, [r1]
	ldr r1, .L_080e80e4
	ldr r3, .L_080e80e8
	adds r2, r2, r1
	movs r1, #144
	str r3, [r2]
	ldr r0, .L_080e80ec
	lsls r1, r1, #3
	bl Scheduler_AddOrUpdateCallback
	movs r0, #232
	ldr r3, [sp, #60]
	mov r4, sp
	adds r4, #80
	lsls r0, r0, #9
	movs r2, #0
	str r3, [sp, #32]
	str r4, [sp, #28]
	str r0, [sp, #20]
	mov r11, r2
.L_080e7e5c:
	ldr r3, .L_080e80f0
	mov r1, r11
	subs r1, #16
	ldr r5, [r3]
	str r1, [sp, #36]
	cmp r1, #19
	ble .L_080e7e74
	movs r0, #2
	movs r1, #2
	movs r2, #2
	bl Palette_BrightenBgEntries
.L_080e7e74:
	mov r2, r11
	cmp r2, #0
	bne .L_080e7e80
	movs r0, #156
	bl Func_080f9010
.L_080e7e80:
	mov r3, r11
	cmp r3, #40
	bne .L_080e7e8c
	movs r0, #145
	bl Func_080f9010
.L_080e7e8c:
	mov r4, r11
	cmp r4, #48
	bne .L_080e7ebe
	ldr r0, [sp, #76]
	cmp r0, #1
	bne .L_080e7eb8
	ldr r1, [sp, #32]
	ldr r2, .L_080e80f4
	ldr r3, [r1]
	adds r3, r3, r2
	ldr r0, [r3]
	bl ResourceObject_ReleaseFar
	ldr r4, [sp, #32]
	ldr r0, .L_080e80f8
	ldr r3, [r4]
	adds r3, r3, r0
	ldr r0, [r3]
	bl ResourceObject_ReleaseFar
	bl Func_080b5118
.L_080e7eb8:
	movs r0, #134
	bl BattleEventRuntime_BeginPhaseFar
.L_080e7ebe:
	bl Render_ResetTransformState
	adds r1, r5, #0
	adds r1, #12
	adds r0, r5, #0
	bl Graphics_PrepareTransferInIwramWork
	ldr r7, .L_080e80e0
	movs r1, #0
	movs r2, #63
	mov r8, r1
	mov r10, r2
.L_080e7ed6:
	ldr r3, [r7, #4]
	cmp r3, #0
	blt .L_080e7f96
	add r6, sp, #96
	adds r0, r7, #0
	adds r1, r6, #0
	bl EffectPosition_ApplyBaseAndYOffset
	ldr r3, [r6]
	ldr r2, [r6, #8]
	asrs r3, r3, #1
	str r3, [r6]
	cmp r2, #159
	bgt .L_080e7ef8
	movs r3, #160
	str r3, [r6, #8]
	movs r2, #160
.L_080e7ef8:
	ldr r3, .L_080e80fc
	cmp r2, r3
	ble .L_080e7f02
	str r3, [r6, #8]
	adds r2, r3, #0
.L_080e7f02:
	adds r3, r2, #0
	subs r3, #160
	cmp r3, #0
	bge .L_080e7f0c
	adds r3, #63
.L_080e7f0c:
	asrs r3, r3, #6
	movs r0, #9
	subs r0, r0, r3
	ldr r2, .L_080e8100
	lsls r5, r0, #1
	subs r3, r5, #2
	ldrh r4, [r2, r3]
	movs r3, #1
	mov r2, r8
	ands r2, r3
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #7
	adds r3, r3, r2
	ldr r2, [sp, #60]
	lsls r3, r3, #1
	ldr r1, [r2]
	adds r4, r4, r3
	movs r3, #200
	adds r1, r1, r4
	lsls r3, r3, #6
	adds r1, r1, r3
	lsrs r3, r0, #31
	ldr r2, [r6]
	adds r3, r0, r3
	asrs r3, r3, #1
	subs r2, r2, r3
	ldr r3, [r6, #4]
	ldr r4, [sp, #68]
	subs r3, r3, r0
	str r0, [sp, #0]
	str r5, [sp, #4]
	ldr r0, [sp, #72]
	bl _call_via_r4
	ldr r2, .L_080e8104
	adds r0, r7, #0
	movs r1, #64
	bl EffectStep_AdvanceWithGravity3D
	movs r2, #160
	ldr r3, [r7, #4]
	lsls r2, r2, #13
	cmp r3, r2
	bgt .L_080e7f96
	movs r3, #0
	str r3, [r7]
	str r3, [r7, #8]
	str r2, [r7, #4]
	bl Random16
	mov r1, r10
	ands r0, r1
	subs r0, #32
	lsls r0, r0, #15
	str r0, [r7, #12]
	bl Random16
	mov r2, r10
	ands r0, r2
	lsls r0, r0, #13
	str r0, [r7, #16]
	bl Random16
	mov r3, r10
	ands r0, r3
	subs r0, #32
	lsls r0, r0, #15
	str r0, [r7, #20]
.L_080e7f96:
	movs r4, #1
	add r8, r4
	mov r0, r8
	adds r7, #28
	cmp r0, #64
	bne .L_080e7ed6
	movs r1, #0
	mov r8, r1
	mov r10, r1
.L_080e7fa8:
	ldr r2, [sp, #60]
	movs r5, #7
	ldr r1, [r2]
	mov r2, r8
	ands r5, r2
	mov r4, r10
	adds r3, r1, r4
	movs r0, #225
	adds r4, r5, #3
	ldr r2, .L_080e8100
	lsls r6, r4, #1
	lsls r0, r0, #7
	adds r7, r3, r0
	subs r3, r6, #2
	ldrh r0, [r2, r3]
	movs r3, #1
	mov r2, r8
	ands r2, r3
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #7
	adds r3, r3, r2
	lsls r3, r3, #1
	ldr r2, [r7]
	adds r0, r0, r3
	lsrs r3, r4, #1
	adds r1, r1, r0
	subs r2, r2, r3
	movs r0, #200
	ldr r3, [r7, #4]
	lsls r0, r0, #6
	subs r3, r3, r4
	adds r1, r1, r0
	str r4, [sp, #0]
	str r6, [sp, #4]
	ldr r0, [sp, #72]
	ldr r4, [sp, #68]
	bl _call_via_r4
	ldr r3, [r7, #4]
	movs r0, #10
	subs r3, r3, r5
	subs r3, #8
	negs r0, r0
	str r3, [r7, #4]
	cmp r3, r0
	bge .L_080e800a
	movs r3, #128
	str r3, [r7, #4]
.L_080e800a:
	movs r2, #1
	add r8, r2
	movs r1, #28
	mov r3, r8
	add r10, r1
	cmp r3, #64
	bne .L_080e7fa8
	movs r4, #0
	movs r0, #255
	ldr r7, .L_080e80dc
	mov r8, r4
	mov r10, r4
	mov r9, r0
.L_080e8024:
	movs r1, #3
	mov r0, r8
	bl FixedPoint_Ratio
	ldr r1, [sp, #36]
	cmp r0, r1
	bge .L_080e80bc
	ldr r3, [r7, #4]
	cmp r3, #0
	blt .L_080e80bc
	add r5, sp, #96
	adds r0, r7, #0
	adds r1, r5, #0
	bl EffectPosition_ApplyBaseAndYOffset
	ldr r3, [r5]
	asrs r6, r3, #1
	str r6, [r5]
	ldr r2, [r7, #24]
	cmp r2, #13
	bhi .L_080e807a
	lsrs r3, r2, #31
	adds r3, r2, r3
	ldr r4, [sp, #60]
	ldr r2, .L_080e8108
	asrs r3, r3, #1
	lsls r3, r3, #1
	ldrh r2, [r2, r3]
	ldr r1, [r4]
	adds r1, r1, r2
	ldr r2, .L_080e810c
	ldrh r4, [r2, r3]
	ldr r3, [r5, #4]
	lsrs r0, r4, #1
	subs r2, r6, r0
	subs r3, r3, r0
	str r4, [sp, #0]
	str r4, [sp, #4]
	ldr r0, [sp, #72]
	ldr r4, [sp, #68]
	bl _call_via_r4
	ldr r2, [r7, #24]
.L_080e807a:
	adds r3, r2, #1
	str r3, [r7, #24]
	cmp r3, #14
	bne .L_080e80b2
	movs r3, #160
	lsls r3, r3, #13
	mov r0, r10
	str r3, [r7, #4]
	str r0, [r7]
	bl Random16
	mov r1, r9
	ands r0, r1
	subs r0, #127
	lsls r0, r0, #16
	mov r2, r10
	str r0, [r7, #8]
	str r2, [r7, #12]
	bl Random16
	mov r3, r9
	ands r0, r3
	mov r4, r10
	lsls r0, r0, #11
	str r0, [r7, #16]
	str r4, [r7, #20]
	str r4, [r7, #24]
	b .L_080e80bc
.L_080e80b2:
	adds r0, r7, #0
	movs r1, #64
	movs r2, #1
	bl EffectStep_AdvanceWithGravity3D
.L_080e80bc:
	movs r0, #1
	add r8, r0
	mov r1, r8
	adds r7, #28
	cmp r1, #64
	bne .L_080e8024
	ldr r2, [sp, #64]
	cmp r2, #1
	bne .L_080e8110
	mov r4, r11
	lsrs r3, r4, #31
	add r3, r11
	asrs r3, r3, #1
	adds r1, r3, #0
	adds r1, #24
	b .L_080e811c
.L_080e80dc:
	.4byte gMapCellBuffer
.L_080e80e0:
	.4byte Data_02010e00
.L_080e80e4:
	.4byte 0x00007784
.L_080e80e8:
	.4byte 0x10101010
.L_080e80ec:
	.4byte BattlePresentation_ProcessPendingGraphicsTransfer
.L_080e80f0:
	.4byte gCameraWork
.L_080e80f4:
	.4byte 0x000077d8
.L_080e80f8:
	.4byte 0x000077dc
.L_080e80fc:
	.4byte 0x0000031f
.L_080e8100:
	.4byte ParticleStreams_CellOffsets
.L_080e8104:
	.4byte 0xffffe000
.L_080e8108:
	.4byte Data_080eeebc
.L_080e810c:
	.4byte Data_080eeeca
.L_080e8110:
	mov r0, r11
	lsrs r3, r0, #31
	add r3, r11
	asrs r3, r3, #1
	movs r2, #56
	subs r1, r2, r3
.L_080e811c:
	mov r3, r11
	lsls r2, r3, #1
	mov r4, r11
	movs r3, #64
	subs r0, r3, r2
	lsls r3, r4, #8
	movs r4, #128
	lsls r4, r4, #10
	adds r2, r3, r4
	ldr r4, [sp, #24]
	movs r3, #0
	str r3, [r4, #12]
	movs r3, #255
	lsls r3, r3, #16
	str r3, [r4, #4]
	ldr r3, [sp, #76]
	cmp r3, #1
	bne .L_080e8186
	ldr r4, [sp, #20]
	ldr r2, [sp, #28]
	str r4, [sp, #80]
	str r4, [r2, #4]
	movs r4, #192
	lsls r3, r1, #16
	lsls r4, r4, #15
	ldr r1, [sp, #24]
	adds r3, r3, r4
	str r3, [r1]
	movs r3, #96
	subs r3, r3, r0
	lsls r3, r3, #16
	str r3, [r1, #8]
	ldr r2, [sp, #32]
	ldr r4, .L_080e8228
	ldr r3, [r2]
	adds r3, r3, r4
	ldr r0, [r3]
	ldr r1, [sp, #24]
	ldr r2, [sp, #28]
	movs r3, #0
	bl Object_ApplyProjectedPlacementFar
	ldr r0, [sp, #32]
	ldr r1, .L_080e822c
	ldr r3, [r0]
	adds r3, r3, r1
	ldr r0, [r3]
	ldr r1, [sp, #24]
	ldr r2, [sp, #28]
	movs r3, #0
	bl Object_ApplyProjectedPlacementFar
	b .L_080e81b4
.L_080e8186:
	ldr r3, [sp, #28]
	str r2, [sp, #80]
	movs r4, #192
	str r2, [r3, #4]
	lsls r4, r4, #15
	lsls r3, r1, #16
	ldr r1, [sp, #24]
	adds r3, r3, r4
	str r3, [r1]
	movs r3, #96
	subs r3, r3, r0
	lsls r3, r3, #16
	str r3, [r1, #8]
	ldr r2, [sp, #60]
	ldr r4, .L_080e8228
	ldr r3, [r2]
	adds r3, r3, r4
	ldr r0, [r3]
	ldr r1, [sp, #24]
	ldr r2, [sp, #28]
	movs r3, #0
	bl Object_ApplyProjectedPlacementFar
.L_080e81b4:
	ldr r0, [sp, #32]
	ldr r1, .L_080e8230
	ldr r3, [r0]
	movs r2, #1
	adds r3, r3, r1
	str r2, [r3]
	movs r0, #8
	movs r1, #8
	bl Camera_ApplyShake
	ldr r4, [sp, #32]
	ldr r0, .L_080e8234
	ldr r3, [r4]
	movs r1, #1
	adds r3, r3, r0
	str r1, [r3]
	movs r0, #1
	bl WaitFrames
	movs r3, #128
	ldr r2, [sp, #20]
	movs r4, #1
	lsls r3, r3, #1
	add r11, r4
	adds r2, r2, r3
	mov r0, r11
	str r2, [sp, #20]
	cmp r0, #54
	beq .L_080e81f0
	b .L_080e7e5c
.L_080e81f0:
	ldr r0, .L_080e8238
	bl Scheduler_RemoveCallback
	movs r0, #46
	bl Runtime_ReleaseHeapBlock
	ldr r1, [sp, #76]
	cmp r1, #0
	bne .L_080e8210
	ldr r2, [sp, #60]
	ldr r4, .L_080e8228
	ldr r3, [r2]
	adds r3, r3, r4
	ldr r0, [r3]
	bl ResourceObject_ReleaseFar
.L_080e8210:
	bl BattleFx_EndCanvasLayer
	add sp, #284
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
.L_080e8228:
	.4byte 0x000077d8
.L_080e822c:
	.4byte 0x000077dc
.L_080e8230:
	.4byte 0x000077a8
.L_080e8234:
	.4byte 0x00007824
.L_080e8238:
	.4byte BattlePresentation_ProcessPendingGraphicsTransfer
