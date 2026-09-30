.syntax unified
	.thumb
	.global Battle_CollectPartyCommands
	.thumb_func
Battle_CollectPartyCommands:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #100
	str r0, [sp, #88]
	movs r0, #128
	lsls r0, r0, #1
	str r0, [sp, #68]
	movs r0, #128
	str r1, [sp, #84]
	lsls r0, r0, #3
	movs r1, #0
	str r2, [sp, #80]
	str r1, [sp, #60]
	str r1, [sp, #56]
	bl Resource_LoadIntoFreeSlot
	str r0, [sp, #52]
	movs r0, #128
	lsls r0, r0, #2
	bl Resource_LoadIntoFreeSlot
	movs r1, #130
	str r0, [sp, #48]
	lsls r1, r1, #1
	movs r0, #57
	bl Runtime_AllocateBlock
	mov r2, sp
	adds r2, #92
	str r2, [sp, #36]
	str r0, [r2]
	movs r1, #0
	ldr r0, [sp, #52]
	bl Resource_LoadIndexedIntoBuffer
	ldr r0, .L_080271f0
	bl Graphics_ExpandVramTilesByColorTable
	ldr r0, .L_080271f4
	bl Link_DrawShiftedTilePair
	bl Menu_BuildLocalizedPatternTiles
	ldr r3, .L_080271f8
	ldr r2, [sp, #36]
	ldr r0, [r3]
	ldr r3, [r2]
	movs r1, #128
	lsls r1, r1, #24
	adds r3, #228
	movs r2, #7
.L_08027184:
	subs r2, #1
	stmia r3!, {r1}
	cmp r2, #0
	bge .L_08027184
	ldr r2, [sp, #36]
	ldr r3, [r2]
	movs r1, #0
	adds r3, #36
	movs r2, #2
.L_08027196:
	subs r2, #1
	strb r1, [r3]
	adds r3, #1
	cmp r2, #0
	bge .L_08027196
	ldr r2, [sp, #36]
	ldr r1, [r2]
	movs r2, #1
	movs r3, #0
	negs r2, r2
	str r3, [r1, #40]
	str r3, [r1, #44]
	str r3, [r1, #60]
	str r3, [r1, #64]
	str r3, [r1, #80]
	str r3, [r1, #72]
	str r3, [r1, #68]
	str r2, [r1, #76]
	adds r3, r0, #0
	adds r3, #68
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_0802729e
	movs r3, #1
	ldr r2, .L_080271fc
	str r3, [r1, #80]
	ldr r3, .L_080271e4
	strh r3, [r2, #8]
	ldr r3, .L_080271e8
	strh r3, [r2, #10]
	strh r3, [r2, #12]
	ldr r3, .L_080271ec
	movs r5, #0
	strh r3, [r2, #14]
	adds r7, r0, #0
	adds r6, r0, #0
	adds r7, #80
	adds r6, #82
	b .L_0802727a
.L_080271e4:
	.4byte 0x00000056
.L_080271e8:
	.4byte 0x00000053
.L_080271ec:
	.4byte 0x00000054
.L_080271f0:
	.4byte 0x06006000
.L_080271f4:
	.4byte 0x06006680
.L_080271f8:
	.4byte gBattleWork
.L_080271fc:
	.4byte gSerialTransfer + 0x4
.L_08027200:
	ldr r3, .L_080273b4
	ldrh r2, [r3]
	movs r3, #3
	ands r3, r2
	cmp r3, #3
	beq .L_08027220
	adds r5, #1
	cmp r5, #24
	ble .L_08027274
	movs r3, #1
	negs r3, r3
	str r3, [sp, #80]
	ldr r1, [sp, #36]
	ldr r3, [r1]
	str r0, [r3, #80]
	b .L_0802729e
.L_08027220:
	ldrh r2, [r1, #8]
	adds r3, r2, #0
	movs r5, #0
	cmp r3, #86
	bne .L_0802723c
	ldrh r3, [r1, #10]
	cmp r3, #83
	bne .L_0802723c
	ldrh r3, [r1, #12]
	cmp r3, #83
	bne .L_0802723c
	ldrh r3, [r1, #14]
	cmp r3, #84
	beq .L_0802729e
.L_0802723c:
	adds r3, r2, #0
	cmp r3, #69
	bne .L_08027254
	ldrh r3, [r1, #10]
	cmp r3, #68
	bne .L_08027254
	ldrh r3, [r1, #12]
	cmp r3, #86
	bne .L_08027254
	ldrh r3, [r1, #14]
	cmp r3, #83
	beq .L_0802729e
.L_08027254:
	ldrh r3, [r1]
	cmp r3, #69
	bne .L_0802726c
	ldrh r3, [r1, #2]
	cmp r3, #88
	bne .L_0802726c
	ldrh r3, [r1, #4]
	cmp r3, #69
	bne .L_0802726c
	ldrh r3, [r1, #6]
	cmp r3, #67
	beq .L_08027274
.L_0802726c:
	movs r2, #1
	negs r2, r2
	str r2, [sp, #80]
	b .L_08027296
.L_08027274:
	movs r0, #1
	bl WaitFrames
.L_0802727a:
	ldrb r2, [r7]
	movs r3, #1
	eors r3, r2
	lsls r2, r3, #1
	adds r2, r2, r3
	ldrb r0, [r6]
	ldr r3, .L_080273b8
	lsls r2, r2, #3
	adds r1, r2, r3
	cmp r0, #0
	beq .L_08027200
	movs r1, #1
	negs r1, r1
	str r1, [sp, #80]
.L_08027296:
	ldr r0, [sp, #36]
	ldr r3, [r0]
	movs r2, #0
	str r2, [r3, #80]
.L_0802729e:
	movs r1, #200
	ldr r0, .L_080273bc
	lsls r1, r1, #4
	bl Scheduler_AddOrUpdateCallback
.L_080272a8:
	add r1, sp, #100
	mov r9, r1
	bl Battle_DrawPartyPanelsWithEmptyList
	ldr r2, [sp, #36]
	ldr r0, [r2]
	adds r3, r0, #0
	adds r3, #38
	movs r1, #0
	strb r1, [r3]
	adds r2, r0, #0
	movs r3, #1
	negs r3, r3
	adds r2, #224
	str r3, [r2]
	adds r3, r0, #0
	adds r3, #216
	movs r0, #183
	str r1, [r3]
	lsls r0, r0, #1
	bl GameFlag_IsSet
	cmp r0, #0
	beq .L_080272ea
	ldr r3, [sp, #36]
	ldr r2, [r3]
	adds r1, r2, #0
	movs r3, #1
	adds r1, #216
	str r3, [r1]
	adds r2, #220
	movs r3, #60
	str r3, [r2]
.L_080272ea:
	ldr r0, [sp, #80]
	cmp r0, #0
	ble .L_080272fa
	movs r0, #0
	bl Ui_RunSelectionScreen
	adds r6, r0, #0
	b .L_080272fc
.L_080272fa:
	movs r6, #14
.L_080272fc:
	cmp r6, #7
	bne .L_08027340
	movs r0, #12
	bl Runtime_BumpAllocate
	ldr r3, .L_080273c0
	ldrb r3, [r3]
	adds r6, r0, #0
	cmp r3, #0
	beq .L_0802731e
	ldr r3, .L_080273c4
	ldr r2, [r3]
	movs r3, #8
	ands r2, r3
	movs r0, #2
	cmp r2, #0
	bne .L_08027320
.L_0802731e:
	movs r0, #1
.L_08027320:
	adds r1, r6, #0
	bl BattleParty_ListActorIdsFar
	adds r5, r0, #0
	movs r0, #1
	bl WaitFrames
	ldrh r2, [r6]
	adds r0, r6, #0
	adds r1, r5, #0
	bl Ui_RunOwnerStatusScreen
	adds r0, r6, #0
	bl Runtime_BumpFree
	b .L_080272a8
.L_08027340:
	cmp r6, #4
	bne .L_08027376
	bl UiText_ShowLocalizedMessageAndWait
	cmp r0, #0
	bne .L_080272a8
	ldr r2, [sp, #88]
	movs r1, #1
	str r2, [sp, #76]
	str r1, [sp, #80]
	ldr r1, [sp, #84]
	ldrh r3, [r1]
	strh r3, [r2]
	ldr r1, [sp, #76]
	ldr r3, .L_080273c8
	strh r3, [r1, #4]
	ldr r2, [sp, #76]
	movs r3, #99
	strh r3, [r2, #6]
	ldr r3, [sp, #76]
	strh r0, [r3, #8]
	ldr r0, [sp, #76]
	movs r3, #128
	lsls r3, r3, #1
	strh r3, [r0, #10]
	bl .L_08028020
.L_08027376:
	cmp r6, #14
	beq .L_0802737e
	bl .L_08028020
.L_0802737e:
	movs r0, #154
	bl AudioCommand_PlayFar
	ldr r2, [sp, #80]
	movs r1, #0
	str r1, [sp, #44]
	cmp r1, r2
	blt .L_08027392
	bl .L_08028020
.L_08027392:
	mov r3, sp
	adds r3, #96
	str r1, [sp, #28]
	str r1, [sp, #24]
	str r1, [sp, #32]
	str r3, [sp, #20]
.L_0802739e:
	ldr r0, [sp, #44]
	cmp r0, #0
	bne .L_080273cc
	ldr r2, [sp, #36]
	ldr r1, [r2]
	movs r0, #0
	adds r1, #84
	bl BattlePlacement_CountValidEntriesFar
	b .L_080273e6
	.2byte 0x0000
.L_080273b4:
	.4byte gLinkStatus
.L_080273b8:
	.4byte gLinkPeerSignatures
.L_080273bc:
	.4byte UpdateLinkSessionCountdown
.L_080273c0:
	.4byte gDebugMode
.L_080273c4:
	.4byte gKeysHeld
.L_080273c8:
	.4byte 0x00007ffe
.L_080273cc:
	ldr r0, [sp, #36]
	ldr r1, [sp, #28]
	ldr r3, [r0]
	adds r3, r1, r3
	adds r2, r3, #0
	adds r2, #80
	movs r1, #3
.L_080273da:
	ldrb r3, [r2]
	subs r1, #1
	strb r3, [r2, #4]
	adds r2, #1
	cmp r1, #0
	bge .L_080273da
.L_080273e6:
	ldr r3, [sp, #24]
	ldr r2, [sp, #88]
	adds r2, r2, r3
	str r2, [sp, #76]
	ldr r0, [sp, #32]
	ldr r1, [sp, #84]
	ldrh r0, [r0, r1]
	str r0, [sp, #64]
	bl Owner_GetStateFar
	ldr r3, .L_08027464
	str r0, [sp, #72]
	ldr r5, [r3]
	ldr r2, [sp, #64]
	adds r3, r5, #0
	adds r3, #224
	str r2, [r3]
	movs r2, #0
	ldr r3, .L_08027468
	str r2, [r5, #64]
	adds r5, #24
	str r3, [r5, #4]
	str r2, [r5, #8]
	ldr r0, [sp, #72]
	movs r1, #148
	lsls r1, r1, #1
	adds r3, r0, r1
	ldrb r0, [r3]
	ldr r1, [sp, #48]
	bl Ui_LoadEntryForKind
	ldr r3, .L_08027460
	ldrh r2, [r5, #8]
	ands r0, r3
	ldr r3, .L_0802746c
	ands r3, r2
	orrs r3, r0
	strh r3, [r5, #8]
	ldrh r2, [r5, #6]
	ldr r3, .L_08027470
	ands r3, r2
	strh r3, [r5, #6]
	ldrb r2, [r5, #9]
	movs r3, #128
	strb r3, [r5, #4]
	movs r3, #15
	ands r3, r2
	movs r2, #224
	orrs r3, r2
	strb r3, [r5, #9]
	ldr r3, [sp, #36]
	ldr r2, [r3]
	movs r3, #1
	adds r2, #38
	strb r3, [r2]
.L_08027454:
	bl UiWindow_MarkVisibleTileAttributes
	ldr r5, .L_08027464
	ldr r1, [r5]
	b .L_08027474
	.2byte 0x0000
.L_08027460:
	.4byte 0x000003ff
.L_08027464:
	.4byte gLinkCountdownWork
.L_08027468:
	.4byte 0x80000400
.L_0802746c:
	.4byte 0xfffffc00
.L_08027470:
	.4byte 0xfffffe00
.L_08027474:
	adds r3, r1, #0
	adds r3, #36
	movs r7, #0
	strb r7, [r3]
	ldr r2, [sp, #28]
	movs r3, #128
	adds r2, #228
	lsls r3, r3, #24
	str r3, [r1, r2]
	add r1, sp, #64
	ldr r2, [sp, #20]
	ldrh r1, [r1]
	strh r1, [r2]
	ldr r2, [sp, #20]
	movs r3, #255
	ldr r0, .L_080274c4
	strh r3, [r2, #2]
	movs r1, #1
	mov r8, r0
	ldr r0, [sp, #20]
	bl BattlePres_SetActorModesFar
	ldr r0, [sp, #20]
	bl BattleLayout_HighlightPartyPanels
	movs r0, #1
	bl WaitFrames
	movs r0, #1
	bl Ui_RunSelectionScreen
	adds r6, r0, #0
	movs r0, #1
	bl WaitFrames
	movs r3, #2
	negs r3, r3
	cmp r6, r3
	bne .L_08027506
	b .L_080274c8
.L_080274c4:
	.4byte 0x00000000
.L_080274c8:
	movs r0, #12
	bl Runtime_BumpAllocate
	adds r5, r0, #0
	adds r1, r5, #0
	movs r0, #1
	bl BattleParty_ListActorIdsFar
	adds r6, r0, #0
	ldr r0, [sp, #36]
	ldr r3, [r0]
	mov r1, r8
	adds r3, #38
	strb r1, [r3]
	movs r0, #1
	bl WaitFrames
	adds r0, r5, #0
	ldr r2, [sp, #64]
	adds r1, r6, #0
	bl Ui_RunOwnerStatusScreen
	ldr r2, [sp, #36]
	ldr r3, [r2]
	movs r2, #1
	adds r3, #38
	strb r2, [r3]
	adds r0, r5, #0
	bl Runtime_BumpFree
	b .L_0802739e
.L_08027506:
	ldr r0, [sp, #20]
	movs r1, #0
	bl BattlePres_SetActorModesFar
	movs r3, #1
	negs r3, r3
	cmp r6, r3
	bne .L_08027532
	ldr r0, [sp, #44]
	cmp r0, #0
	bne .L_08027520
	bl .L_08028014
.L_08027520:
	subs r0, #1
	lsls r1, r0, #2
	lsls r2, r0, #4
	lsls r3, r0, #1
	str r0, [sp, #44]
	str r1, [sp, #28]
	str r2, [sp, #24]
	str r3, [sp, #32]
	b .L_0802739e
.L_08027532:
	ldr r5, [r5]
	ldr r3, [r5, #76]
	cmp r3, #0
	bne .L_0802753c
	movs r6, #3
.L_0802753c:
	ldr r3, .L_08027570
	str r7, [r5, #8]
	str r3, [r5, #4]
	ldr r0, [sp, #52]
	adds r1, r6, #0
	bl Resource_LoadIndexedIntoBuffer
	ldr r3, .L_08027568
	ldrh r2, [r5, #8]
	ands r0, r3
	ldr r3, .L_08027574
	ands r3, r2
	orrs r3, r0
	strh r3, [r5, #8]
	cmp r6, #15
	bne .L_0802757c
	ldrh r3, [r5, #6]
	ldr r2, .L_08027578
	ands r2, r3
	ldr r3, .L_0802756c
	b .L_08027584
	.2byte 0x0000
.L_08027568:
	.4byte 0x000003ff
.L_0802756c:
	.4byte 0x00000080
.L_08027570:
	.4byte 0x80002400
.L_08027574:
	.4byte 0xfffffc00
.L_08027578:
	.4byte 0xfffffe00
.L_0802757c:
	ldrh r3, [r5, #6]
	ldr r2, .L_080275ac
	ands r2, r3
	ldr r3, .L_080275a8
.L_08027584:
	orrs r2, r3
	strh r2, [r5, #6]
	movs r3, #136
	strb r3, [r5, #4]
	ldr r0, [sp, #36]
	ldr r2, [r0]
	movs r3, #1
	adds r2, #36
	strb r3, [r2]
	cmp r6, #16
	bls .L_0802759e
	bl .L_08027f82
.L_0802759e:
	ldr r2, .L_080275b0
	lsls r3, r6, #2
	ldr r3, [r3, r2]
	b .L_080275b4
	.2byte 0x0000
.L_080275a8:
	.4byte 0x00000060
.L_080275ac:
	.4byte 0xfffffe00
.L_080275b0:
	.4byte .L_080275b8
.L_080275b4:
	mov pc, r3
	.2byte 0x0000
.L_080275b8:
	.4byte .L_080275fc
	.4byte .L_08027670
	.4byte .L_08027d6a
	.4byte .L_08027f7e
	.4byte .L_08027f82
	.4byte .L_08027f82
	.4byte .L_08027f82
	.4byte .L_08027f82
	.4byte .L_08027f82
	.4byte .L_08027f82
	.4byte .L_08027f82
	.4byte .L_08027f82
	.4byte .L_08027f82
	.4byte .L_08027f82
	.4byte .L_08027f82
	.4byte .L_08027b5c
	.4byte .L_080278e4
.L_080275fc:
	movs r3, #6
	str r3, [sp, #0]
	movs r1, #17
	movs r2, #11
	movs r3, #3
	movs r0, #11
	bl UiWindow_Create
	mov r11, r0
	mov r1, r11
	ldr r0, .L_08027658
	movs r2, #16
	movs r3, #0
	bl UiText_DrawCharacterAtOffset
	ldr r2, [sp, #36]
	ldr r1, [r2]
	ldr r3, .L_0802765c
	ldrh r2, [r1, #6]
	ands r3, r2
	ldr r2, .L_08027654
	orrs r3, r2
	strh r3, [r1, #6]
	movs r0, #112
	bl AudioCommand_PlayFar
	movs r3, #0
	ldr r0, [sp, #64]
	movs r1, #1
	movs r2, #1
	bl BattleTarget_RunSelection
	movs r1, #1
	adds r6, r0, #0
	mov r0, r11
	bl UiWork_Finalize
	movs r3, #1
	negs r3, r3
	cmp r6, r3
	bne .L_08027650
	b .L_08027454
.L_08027650:
	b .L_08027660
	.2byte 0x0000
.L_08027654:
	.4byte 0x00000040
.L_08027658:
	.4byte 0x0000001f
.L_0802765c:
	.4byte 0xfffffe00
.L_08027660:
	ldr r1, [sp, #76]
	movs r0, #0
	movs r3, #1
	str r0, [sp, #60]
	str r6, [sp, #68]
	strh r3, [r1, #12]
	bl .L_08027f82
.L_08027670:
	movs r0, #112
	bl AudioCommand_PlayFar
	movs r6, #0
	ldr r3, .L_080276c0
	ldr r3, [r3]
	str r6, [r3, #52]
	str r6, [r3, #48]
	str r6, [r3, #56]
.L_08027682:
	ldr r2, [sp, #36]
	ldr r1, [r2]
	movs r3, #150
	adds r3, r3, r1
	ldrh r2, [r1, #6]
	mov r8, r3
	ldr r3, .L_080276c4
	ands r3, r2
	ldr r2, .L_080276bc
	orrs r3, r2
	strh r3, [r1, #6]
	ldr r2, [sp, #72]
	movs r3, #88
	ldrh r3, [r2, r3]
	ldr r2, .L_080276c8
	movs r0, #116
	adds r5, r2, #0
	adds r0, r0, r1
	ands r5, r3
	mov r10, r0
	movs r4, #0
	movs r1, #0
	cmp r5, #0
	beq .L_08027700
	ldr r7, [sp, #72]
	mov r6, r8
	adds r7, #88
	mov r9, r2
	b .L_080276cc
.L_080276bc:
	.4byte 0x00000030
.L_080276c0:
	.4byte gLinkCountdownWork
.L_080276c4:
	.4byte 0xfffffe00
.L_080276c8:
	.4byte 0x00003fff
.L_080276cc:
	adds r0, r5, #0
	str r1, [sp, #16]
	str r4, [sp, #4]
	bl Ability_GetData
	ldrb r2, [r0, #1]
	movs r3, #128
	ands r3, r2
	ldr r1, [sp, #16]
	ldr r4, [sp, #4]
	cmp r3, #0
	beq .L_080276ee
	mov r3, r10
	strb r1, [r3, r4]
	strh r5, [r6]
	adds r4, #1
	adds r6, #2
.L_080276ee:
	adds r1, #1
	cmp r1, #32
	beq .L_08027700
	adds r7, #4
	ldrh r3, [r7]
	mov r5, r9
	ands r5, r3
	cmp r5, #0
	bne .L_080276cc
.L_08027700:
	movs r3, #0
	mov r0, r10
	strb r3, [r0, r4]
	ldr r3, .L_08027740
	lsls r2, r4, #1
	mov r1, r8
	strh r3, [r2, r1]
	ldr r0, [sp, #64]
	adds r2, r4, #0
	bl BattleMenu_RunActionSelection
	movs r2, #1
	adds r6, r0, #0
	negs r2, r2
	cmp r6, r2
	bne .L_08027722
	b .L_08027454
.L_08027722:
	mov r0, r10
	ldrb r3, [r0, r6]
	ldr r1, [sp, #72]
	lsls r3, r3, #2
	adds r3, #88
	ldrh r3, [r1, r3]
	ldr r2, .L_08027744
	ands r2, r3
	adds r0, r2, #0
	str r2, [sp, #56]
	bl Ability_GetData
	adds r6, r0, #0
	ldr r0, [sp, #36]
	b .L_08027748
.L_08027740:
	.4byte 0x00000000
.L_08027744:
	.4byte 0x00003fff
.L_08027748:
	ldrb r3, [r6, #8]
	ldr r5, [r0]
	movs r0, #128
	mov r8, r3
	bl Resource_LoadIntoFreeSlot
	ldr r3, .L_080277b4
	ldr r7, [r3]
	movs r3, #6
	str r3, [sp, #0]
	mov r10, r0
	movs r1, #17
	movs r2, #18
	movs r3, #3
	movs r0, #8
	bl UiWindow_Create
	ldr r1, [sp, #36]
	mov r11, r0
	ldr r0, [r1]
	ldr r1, .L_080277b8
	ldrh r2, [r0, #6]
	adds r3, r1, #0
	ands r3, r2
	ldr r2, .L_080277ac
	orrs r3, r2
	strh r3, [r0, #6]
	ldr r3, .L_080277bc
	adds r5, #12
	str r3, [r5, #4]
	movs r3, #0
	str r3, [r5, #8]
	mov r3, r11
	ldrh r2, [r3, #12]
	ldr r3, .L_080277b0
	lsls r2, r2, #3
	adds r2, #8
	ands r2, r3
	ldrh r3, [r5, #6]
	mov r0, r11
	ands r1, r3
	ldrh r3, [r0, #14]
	lsls r3, r3, #3
	orrs r1, r2
	adds r3, #4
	strh r1, [r5, #6]
	strb r3, [r5, #4]
	mov r1, r10
	b .L_080277c0
	.2byte 0x0000
.L_080277ac:
	.4byte 0x00000028
.L_080277b0:
	.4byte 0x000001ff
.L_080277b4:
	.4byte gWindowWork
.L_080277b8:
	.4byte 0xfffffe00
.L_080277bc:
	.4byte 0x40000400
.L_080277c0:
	ldr r0, [sp, #56]
	bl Resource_LoadIndexedEntryToBuffer
	ldr r3, .L_080277fc
	ldrh r2, [r5, #8]
	ands r0, r3
	ldr r3, .L_08027800
	ands r3, r2
	orrs r3, r0
	strh r3, [r5, #8]
	ldr r1, [sp, #36]
	ldr r2, [r1]
	movs r3, #1
	adds r2, #37
	strb r3, [r2]
	ldr r3, .L_08027804
	adds r2, r7, r3
	movs r3, #5
	strb r3, [r2]
	ldr r1, [sp, #72]
	ldrb r2, [r6, #9]
	movs r0, #58
	ldrsh r3, [r1, r0]
	cmp r2, r3
	ble .L_08027808
	movs r0, #2
	bl UiWork_SetParamNibble
	b .L_0802781a
	.2byte 0x0000
.L_080277fc:
	.4byte 0x000003ff
.L_08027800:
	.4byte 0xfffffc00
.L_08027804:
	.4byte 0x00000ea7
.L_08027808:
	ldr r2, [sp, #72]
	ldr r0, .L_080278cc
	adds r3, r2, r0
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_0802781a
	movs r0, #9
	bl UiWork_SetParamNibble
.L_0802781a:
	ldr r1, [sp, #56]
	ldr r0, .L_080278d0
	movs r2, #16
	adds r0, r1, r0
	movs r3, #0
	mov r1, r11
	bl UiText_DrawCharacterAtOffset
	movs r5, #0
	ldrb r0, [r6, #9]
	movs r1, #2
	mov r2, r11
	movs r3, #104
	str r5, [sp, #0]
	bl UiText_DrawNumberAtOffset
	ldr r3, .L_080278d4
	adds r2, r7, r3
	movs r3, #15
	strb r3, [r2]
	movs r0, #15
	bl UiWork_SetParamNibble
	ldr r1, .L_080278d8
	mov r0, r11
	movs r2, #11
	movs r3, #0
	str r5, [sp, #0]
	bl UiWindow_SetTilemapEntry
	movs r3, #0
	ldr r1, .L_080278dc
	mov r0, r11
	movs r2, #12
	str r5, [sp, #0]
	bl UiWindow_SetTilemapEntry
	ldrb r3, [r6, #2]
	cmp r3, #4
	beq .L_0802787c
	ldr r0, .L_080278e0
	adds r1, r3, #0
	adds r1, r1, r0
	movs r2, #15
	mov r0, r11
	movs r3, #0
	str r5, [sp, #0]
	bl UiWindow_SetTilemapEntry
.L_0802787c:
	ldr r2, [sp, #76]
	mov r1, r8
	strh r1, [r2, #12]
	movs r0, #112
	bl AudioCommand_PlayFar
	adds r0, r6, #0
	bl Battle_ClassifyEntryKind
	ldrb r1, [r6]
	adds r3, r0, #0
	mov r2, r8
	ldr r0, [sp, #64]
	bl BattleTarget_RunSelection
	adds r6, r0, #0
	ldr r0, [sp, #36]
	ldr r3, [r0]
	ldr r5, .L_080278c8
	adds r3, #37
	strb r5, [r3]
	mov r0, r10
	bl Resource_ResetEntry
	movs r1, #1
	mov r0, r11
	bl UiWork_Finalize
	movs r1, #1
	negs r1, r1
	cmp r6, r1
	bne .L_080278be
	b .L_08027682
.L_080278be:
	movs r2, #1
	str r2, [sp, #60]
.L_080278c2:
	str r6, [sp, #68]
	b .L_08027f82
	.2byte 0x0000
.L_080278c8:
	.4byte 0x00000000
.L_080278cc:
	.4byte 0x0000013d
.L_080278d0:
	.4byte 0x00000333
.L_080278d4:
	.4byte 0x00000ea7
.L_080278d8:
	.4byte 0x0000f01f
.L_080278dc:
	.4byte 0x0000f01e
.L_080278e0:
	.4byte 0x00005001
.L_080278e4:
	movs r0, #112
	bl AudioCommand_PlayFar
	ldr r3, .L_08027938
	ldr r2, [r3]
	movs r3, #0
	str r3, [r2, #52]
	str r3, [r2, #48]
	str r3, [r2, #56]
.L_080278f6:
	ldr r3, [sp, #36]
	ldr r1, [r3]
	ldr r3, .L_0802793c
	ldrh r2, [r1, #6]
	ands r3, r2
	ldr r2, .L_08027934
	orrs r3, r2
	strh r3, [r1, #6]
	ldr r0, [sp, #20]
	bl BattleLayout_HighlightPartyPanels
	ldr r0, [sp, #36]
	ldr r1, [sp, #28]
	ldr r2, [r0]
	adds r2, r2, r1
	adds r2, #84
	movs r1, #0
	movs r0, #0
	bl Func_08024934
	adds r6, r0, #0
	ldr r0, [sp, #20]
	bl BattleLayout_HighlightPartyPanels
	movs r2, #1
	negs r2, r2
	cmp r6, r2
	bne .L_08027930
	b .L_08027454
.L_08027930:
	b .L_08027940
	.2byte 0x0000
.L_08027934:
	.4byte 0x00000050
.L_08027938:
	.4byte gLinkCountdownWork
.L_0802793c:
	.4byte 0xfffffe00
.L_08027940:
	movs r3, #6
	movs r0, #1
	str r3, [sp, #60]
	str r6, [sp, #56]
	bl WaitFrames
	ldr r0, [sp, #36]
	ldr r3, [r0]
	adds r0, r6, #0
	adds r3, #12
	mov r8, r3
	bl SummonDefinition_Get
	mov r9, r0
	ldrh r0, [r0]
	bl Ability_GetData
	mov r10, r0
	movs r0, #128
	bl Resource_LoadIntoFreeSlot
	ldr r1, [sp, #60]
	str r0, [sp, #40]
	str r1, [sp, #0]
	movs r2, #17
	movs r1, #17
	movs r3, #3
	movs r0, #10
	bl UiWindow_Create
	ldr r2, [sp, #36]
	ldr r4, [sp, #28]
	ldr r3, [r2]
	mov r1, r9
	adds r1, #4
	adds r4, #84
	ldrb r2, [r1]
	ldrb r3, [r3, r4]
	mov r11, r0
	movs r7, #0
	cmp r2, r3
	bhi .L_080279ae
	ldr r5, [sp, #36]
	adds r0, r1, #0
	adds r1, r4, #0
.L_0802799a:
	adds r7, #1
	adds r1, #1
	cmp r7, #3
	bgt .L_080279ae
	ldr r3, [r5]
	adds r0, #1
	ldrb r2, [r0]
	ldrb r3, [r3, r1]
	cmp r2, r3
	bls .L_0802799a
.L_080279ae:
	movs r3, #4
	eors r3, r7
	negs r2, r3
	orrs r2, r3
	ldr r3, [sp, #36]
	ldr r0, [r3]
	ldr r1, .L_08027a00
	lsrs r4, r2, #31
	ldrh r2, [r0, #6]
	adds r3, r1, #0
	ands r3, r2
	ldr r2, .L_080279f8
	orrs r3, r2
	strh r3, [r0, #6]
	ldr r3, .L_08027a04
	mov r0, r8
	movs r5, #1
	str r3, [r0, #4]
	movs r3, #0
	subs r4, r5, r4
	str r3, [r0, #8]
	mov r3, r11
	ldrh r2, [r3, #12]
	ldr r3, .L_080279fc
	lsls r2, r2, #3
	adds r2, #8
	ands r2, r3
	ldrh r3, [r0, #6]
	ands r1, r3
	orrs r1, r2
	strh r1, [r0, #6]
	mov r1, r11
	ldrh r3, [r1, #14]
	lsls r3, r3, #3
	mov r2, r9
	b .L_08027a08
	.2byte 0x0000
.L_080279f8:
	.4byte 0x00000038
.L_080279fc:
	.4byte 0x000001ff
.L_08027a00:
	.4byte 0xfffffe00
.L_08027a04:
	.4byte 0x40000400
.L_08027a08:
	adds r3, #4
	strb r3, [r0, #4]
	ldrh r3, [r2]
	ldr r0, .L_08027a54
	ldr r1, [sp, #40]
	ands r0, r3
	str r4, [sp, #4]
	bl Resource_LoadIndexedEntryToBuffer
	ldr r3, .L_08027a50
	ands r0, r3
	mov r3, r8
	ldrh r2, [r3, #8]
	ldr r3, .L_08027a58
	ands r3, r2
	orrs r3, r0
	mov r0, r8
	strh r3, [r0, #8]
	ldr r1, [sp, #36]
	ldr r3, [r1]
	ldr r4, [sp, #4]
	adds r3, #37
	strb r5, [r3]
	cmp r4, #0
	bne .L_08027a42
	movs r0, #2
	bl UiWork_SetParamNibble
	ldr r4, [sp, #4]
.L_08027a42:
	adds r0, r6, #0
	str r4, [sp, #4]
	bl SummonDefinition_Get
	ldr r3, .L_08027a5c
	ldrh r0, [r0]
	b .L_08027a60
.L_08027a50:
	.4byte 0x000003ff
.L_08027a54:
	.4byte 0x00003fff
.L_08027a58:
	.4byte 0xfffffc00
.L_08027a5c:
	.4byte 0x00000333
.L_08027a60:
	movs r2, #16
	adds r0, r0, r3
	mov r1, r11
	movs r3, #0
	bl UiText_DrawCharacterAtOffset
	movs r2, #0
	lsls r3, r2, #1
	mov r6, r9
	adds r5, r3, #0
	ldr r4, [sp, #4]
	movs r7, #0
	mov r8, r2
	adds r6, #4
	adds r5, #13
.L_08027a7e:
	ldrb r3, [r6]
	cmp r3, #0
	beq .L_08027aae
	ldr r3, .L_08027ac4
	mov r0, r8
	adds r1, r7, r3
	str r0, [sp, #0]
	adds r2, r5, #0
	mov r0, r11
	movs r3, #0
	str r4, [sp, #4]
	bl UiWindow_SetTilemapEntry
	ldrb r1, [r6]
	mov r3, r8
	adds r2, r5, #1
	str r3, [sp, #0]
	adds r1, #48
	mov r0, r11
	movs r3, #0
	bl UiWindow_PutGlyph
	ldr r4, [sp, #4]
	adds r5, #2
.L_08027aae:
	adds r7, #1
	adds r6, #1
	cmp r7, #3
	ble .L_08027a7e
	cmp r4, #0
	beq .L_08027ac8
	movs r0, #112
	bl AudioCommand_PlayFar
	b .L_08027ace
	.2byte 0x0000
.L_08027ac4:
	.4byte 0x00005001
.L_08027ac8:
	movs r0, #114
	bl AudioCommand_PlayFar
.L_08027ace:
	mov r0, r10
	bl Battle_ClassifyEntryKind
	adds r3, r0, #0
	mov r0, r10
	ldrb r1, [r0]
	ldrb r2, [r0, #8]
	ldr r0, [sp, #64]
	bl BattleTarget_RunSelection
	mov r1, r10
	ldrb r3, [r1, #8]
	adds r6, r0, #0
	ldr r0, [sp, #76]
	strh r3, [r0, #12]
	ldr r1, [sp, #36]
	ldr r3, [r1]
	ldr r2, .L_08027b10
	adds r3, #37
	strb r2, [r3]
	ldr r0, [sp, #40]
	bl Resource_ResetEntry
	mov r0, r11
	movs r1, #1
	bl UiWork_Finalize
	movs r2, #1
	negs r2, r2
	cmp r6, r2
	bne .L_08027b0e
	b .L_080278f6
.L_08027b0e:
	b .L_08027b14
.L_08027b10:
	.4byte 0x00000000
.L_08027b14:
	ldr r3, [sp, #36]
	ldr r2, [sp, #28]
	movs r0, #0
	mov lr, r0
	adds r2, #84
	mov r0, r9
	ldr r1, [r3]
	adds r0, #4
	adds r5, r2, #0
	mov r12, r3
	ldrb r4, [r0]
	ldrb r3, [r1, r5]
	movs r7, #0
	cmp r4, r3
	bls .L_08027b38
	mov r2, lr
	strb r2, [r1, r5]
	b .L_080278c2
.L_08027b38:
	subs r3, r3, r4
	adds r7, #1
	strb r3, [r1, r5]
	adds r0, #1
	adds r2, #1
	cmp r7, #3
	ble .L_08027b48
	b .L_080278c2
.L_08027b48:
	mov r3, r12
	ldr r1, [r3]
	adds r5, r2, #0
	ldrb r4, [r0]
	ldrb r3, [r1, r5]
	cmp r4, r3
	bls .L_08027b38
	mov r0, lr
	strb r0, [r1, r5]
	b .L_080278c2
.L_08027b5c:
	movs r0, #112
	bl AudioCommand_PlayFar
	ldr r3, .L_08027bb8
	ldr r2, [r3]
	movs r3, #0
	str r3, [r2, #52]
	str r3, [r2, #48]
	str r3, [r2, #56]
.L_08027b6e:
	ldr r2, [sp, #36]
	ldr r0, .L_08027bbc
	ldr r1, [r2]
	mov r10, r0
	ldrh r3, [r1, #6]
	mov r2, r10
	ands r2, r3
	ldr r3, .L_08027bb4
	orrs r2, r3
	strh r2, [r1, #6]
	ldr r7, .L_08027bb8
	ldr r5, [sp, #28]
	ldr r2, [r7]
	movs r3, #128
	lsls r3, r3, #24
	adds r5, #228
	str r3, [r2, r5]
	movs r1, #1
	ldr r0, [sp, #64]
	bl Battle_SelectAbility
	movs r1, #0
	adds r6, r0, #0
	movs r0, #1
	mov r8, r1
	ldr r3, [sp, #76]
	negs r0, r0
	mov r2, r8
	mov r9, r0
	strh r2, [r3, #12]
	cmp r6, r9
	bne .L_08027bb2
	bl .L_08027454
.L_08027bb2:
	b .L_08027bc0
.L_08027bb4:
	.4byte 0x00000090
.L_08027bb8:
	.4byte gLinkCountdownWork
.L_08027bbc:
	.4byte 0xfffffe00
.L_08027bc0:
	movs r1, #5
	str r1, [sp, #60]
	str r6, [sp, #56]
	ldr r3, [r7]
	str r6, [r3, r5]
	ldr r2, [sp, #56]
	movs r3, #15
	asrs r7, r2, #8
	movs r4, #255
	ands r4, r2
	ands r7, r3
	adds r2, r4, #0
	ldr r0, [sp, #64]
	adds r1, r7, #0
	str r4, [sp, #4]
	bl Djinn_IsActiveFar
	adds r5, r0, #0
	ldr r4, [sp, #4]
	cmp r5, #0
	beq .L_08027cd4
	adds r1, r4, #0
	adds r0, r7, #0
	bl Djinn_GetDefinitionHeaderFar
	bl Ability_GetData
	movs r3, #6
	adds r5, r0, #0
	ldrb r6, [r5, #8]
	movs r1, #17
	str r3, [sp, #0]
	movs r2, #10
	movs r3, #3
	movs r0, #11
	bl UiWindow_Create
	ldr r3, [sp, #36]
	ldr r1, [r3]
	ldrh r2, [r1, #6]
	mov r3, r10
	ands r3, r2
	ldr r2, .L_08027c54
	mov r11, r0
	ldr r0, .L_08027c58
	orrs r3, r2
	strh r3, [r1, #6]
	mov r2, r8
	adds r1, r7, r0
	movs r3, #0
	mov r0, r11
	str r2, [sp, #0]
	bl UiWindow_SetTilemapEntry
	lsls r0, r7, #2
	ldr r4, [sp, #4]
	adds r0, r0, r7
	ldr r3, .L_08027c5c
	lsls r0, r0, #2
	adds r0, r0, r4
	mov r1, r11
	movs r2, #16
	adds r0, r0, r3
	movs r3, #0
	bl UiText_DrawCharacterAtOffset
	ldr r3, [sp, #76]
	movs r0, #1
	strh r6, [r3, #12]
	bl WaitFrames
	movs r0, #112
	b .L_08027c60
	.2byte 0x0000
.L_08027c54:
	.4byte 0x00000040
.L_08027c58:
	.4byte 0x00005001
.L_08027c5c:
	.4byte 0x0000045f
.L_08027c60:
	bl AudioCommand_PlayFar
	adds r0, r5, #0
	bl Battle_ClassifyEntryKind
	adds r2, r6, #0
	adds r3, r0, #0
	ldrb r1, [r5]
	ldr r0, [sp, #64]
	bl BattleTarget_RunSelection
	adds r6, r0, #0
	ldr r0, [sp, #36]
	ldr r3, [r0]
	adds r3, #216
	ldr r3, [r3]
	cmp r3, #0
	beq .L_08027cbc
	ldr r0, .L_08027cd0
	movs r1, #15
	movs r2, #8
	bl UiText_ShowMessageAndWaitComplete
	adds r5, r0, #0
	b .L_08027c98
.L_08027c92:
	movs r0, #1
	bl WaitFrames
.L_08027c98:
	bl UiWork_IsComplete
	cmp r0, #0
	beq .L_08027c92
	movs r1, #1
	adds r0, r5, #0
	bl UiWork_Finalize
	ldr r2, [sp, #36]
	ldr r1, [r2]
	adds r2, r1, #0
	adds r2, #216
	ldr r3, [r2]
	adds r3, #1
	str r3, [r2]
	adds r1, #220
	movs r3, #45
	str r3, [r1]
.L_08027cbc:
	mov r0, r11
	movs r1, #1
	bl UiWork_Finalize
	movs r3, #1
	negs r3, r3
	cmp r6, r3
	bne .L_08027cce
	b .L_08027b6e
.L_08027cce:
	b .L_080278c2
.L_08027cd0:
	.4byte 0x00000c4e
.L_08027cd4:
	movs r3, #6
	str r3, [sp, #0]
	movs r1, #17
	movs r2, #10
	movs r3, #3
	movs r0, #11
	str r4, [sp, #4]
	bl UiWindow_Create
	mov r11, r0
	ldr r0, [sp, #36]
	ldr r1, [r0]
	ldrh r2, [r1, #6]
	mov r3, r10
	ands r3, r2
	ldr r2, .L_08027d30
	orrs r3, r2
	strh r3, [r1, #6]
	movs r0, #2
	bl UiWork_SetParamNibble
	ldr r2, .L_08027d34
	mov r0, r11
	adds r1, r7, r2
	movs r3, #0
	movs r2, #0
	str r5, [sp, #0]
	bl UiWindow_SetTilemapEntry
	lsls r0, r7, #2
	ldr r4, [sp, #4]
	adds r0, r0, r7
	ldr r3, .L_08027d38
	lsls r0, r0, #2
	adds r0, r0, r4
	mov r1, r11
	movs r2, #16
	adds r0, r0, r3
	movs r3, #0
	bl UiText_DrawCharacterAtOffset
	movs r0, #15
	bl UiWork_SetParamNibble
	ldr r0, [sp, #76]
	b .L_08027d3c
.L_08027d30:
	.4byte 0x00000040
.L_08027d34:
	.4byte 0x00005001
.L_08027d38:
	.4byte 0x0000045f
.L_08027d3c:
	movs r3, #1
	strh r3, [r0, #12]
	movs r0, #1
	bl WaitFrames
	movs r0, #112
	bl AudioCommand_PlayFar
	movs r1, #4
	ldr r0, [sp, #64]
	movs r2, #0
	movs r3, #7
	bl BattleTarget_RunSelection
	movs r1, #1
	adds r6, r0, #0
	mov r0, r11
	bl UiWork_Finalize
	cmp r6, r9
	bne .L_08027d68
	b .L_08027b6e
.L_08027d68:
	b .L_080278c2
.L_08027d6a:
	movs r0, #112
	bl AudioCommand_PlayFar
	movs r6, #0
	ldr r3, .L_08027db4
	ldr r3, [r3]
	str r6, [r3, #52]
	str r6, [r3, #48]
	str r6, [r3, #56]
.L_08027d7c:
	ldr r2, [sp, #36]
	ldr r1, [r2]
	ldr r3, .L_08027db8
	ldrh r2, [r1, #6]
	ands r3, r2
	ldr r2, .L_08027db0
	orrs r3, r2
	strh r3, [r1, #6]
	movs r3, #116
	adds r3, r3, r1
	adds r1, #150
	mov r10, r1
	ldr r1, [sp, #72]
	mov r9, r3
	movs r3, #216
	ldrh r5, [r1, r3]
	movs r0, #0
	mov r8, r0
	movs r4, #0
	cmp r5, #0
	beq .L_08027df0
	adds r3, r1, #0
	adds r3, #216
	mov r7, r9
	mov r6, r10
	b .L_08027dbc
.L_08027db0:
	.4byte 0x00000060
.L_08027db4:
	.4byte gLinkCountdownWork
.L_08027db8:
	.4byte 0xfffffe00
.L_08027dbc:
	adds r0, r5, #0
	str r3, [sp, #8]
	str r4, [sp, #4]
	bl Item_Get
	adds r1, r5, #0
	ldr r0, [sp, #64]
	bl Item_ClassifyUseAbility
	ldr r3, [sp, #8]
	ldr r4, [sp, #4]
	cmp r0, #0
	bne .L_08027de2
	movs r2, #1
	strh r5, [r6]
	add r8, r2
	strb r4, [r7]
	adds r6, #2
	adds r7, #1
.L_08027de2:
	adds r4, #1
	cmp r4, #15
	beq .L_08027df0
	adds r3, #2
	ldrh r5, [r3]
	cmp r5, #0
	bne .L_08027dbc
.L_08027df0:
	ldr r0, [sp, #72]
	movs r3, #216
	ldrh r5, [r0, r3]
	movs r4, #0
	cmp r5, #0
	beq .L_08027e40
	mov r1, r8
	adds r2, r0, #0
	mov r7, r8
	lsls r3, r1, #1
	mov r0, r10
	adds r2, #216
	add r7, r9
	adds r6, r3, r0
.L_08027e0c:
	adds r0, r5, #0
	str r2, [sp, #12]
	str r4, [sp, #4]
	bl Item_Get
	adds r1, r5, #0
	ldr r0, [sp, #64]
	bl Item_ClassifyUseAbility
	ldr r2, [sp, #12]
	ldr r4, [sp, #4]
	cmp r0, #0
	beq .L_08027e32
	movs r1, #1
	strh r5, [r6]
	add r8, r1
	strb r4, [r7]
	adds r6, #2
	adds r7, #1
.L_08027e32:
	adds r4, #1
	cmp r4, #15
	beq .L_08027e40
	adds r2, #2
	ldrh r5, [r2]
	cmp r5, #0
	bne .L_08027e0c
.L_08027e40:
	mov r2, r8
	ldr r1, .L_08027e64
	lsls r3, r2, #1
	mov r0, r10
	strh r1, [r3, r0]
	ldr r0, [sp, #64]
	mov r1, r10
	bl ItemList_SelectEntry
	movs r7, #1
	adds r6, r0, #0
	negs r7, r7
	cmp r6, r7
	bne .L_08027e60
	bl .L_08027454
.L_08027e60:
	b .L_08027e68
	.2byte 0x0000
.L_08027e64:
	.4byte 0x00000000
.L_08027e68:
	mov r2, r9
	ldrb r6, [r2, r6]
	ldr r3, [sp, #72]
	str r6, [sp, #56]
	lsls r6, r6, #1
	adds r6, #216
	ldrh r0, [r3, r6]
	bl Item_Get
	ldrh r0, [r0, #40]
	bl Ability_GetData
	mov r8, r0
	ldrb r0, [r0, #8]
	ldr r1, [sp, #36]
	mov r10, r0
	movs r0, #128
	ldr r5, [r1]
	bl Resource_LoadIntoFreeSlot
	movs r3, #6
	str r3, [sp, #0]
	mov r9, r0
	movs r1, #17
	movs r2, #15
	movs r3, #3
	movs r0, #9
	bl UiWindow_Create
	ldr r2, [sp, #36]
	mov r11, r0
	ldr r0, [r2]
	ldr r1, .L_08027ee8
	ldrh r2, [r0, #6]
	adds r3, r1, #0
	ands r3, r2
	ldr r2, .L_08027ee0
	orrs r3, r2
	strh r3, [r0, #6]
	ldr r3, .L_08027eec
	adds r5, #12
	str r3, [r5, #4]
	movs r3, #0
	str r3, [r5, #8]
	mov r0, r11
	ldrh r3, [r0, #12]
	ldr r4, .L_08027ee4
	ldrh r2, [r5, #6]
	lsls r3, r3, #3
	adds r3, #8
	ands r3, r4
	ands r1, r2
	orrs r1, r3
	ldrh r3, [r0, #14]
	lsls r3, r3, #3
	adds r3, #4
	strb r3, [r5, #4]
	strh r1, [r5, #6]
	ldr r1, [sp, #72]
	b .L_08027ef0
.L_08027ee0:
	.4byte 0x00000030
.L_08027ee4:
	.4byte 0x000001ff
.L_08027ee8:
	.4byte 0xfffffe00
.L_08027eec:
	.4byte 0x40000400
.L_08027ef0:
	ldrh r0, [r1, r6]
	mov r1, r9
	str r4, [sp, #4]
	bl Resource_LoadKind26EntryToBuffer
	ldr r3, .L_08027f34
	ldrh r2, [r5, #8]
	ands r0, r3
	ldr r3, .L_08027f38
	ands r3, r2
	orrs r3, r0
	strh r3, [r5, #8]
	ldr r3, [sp, #36]
	ldr r2, [r3]
	movs r3, #1
	adds r2, #37
	strb r3, [r2]
	ldr r1, [sp, #72]
	ldr r4, [sp, #4]
	ldrh r0, [r1, r6]
	ldr r3, .L_08027f3c
	ands r0, r4
	mov r1, r11
	adds r0, r0, r3
	movs r2, #24
	movs r3, #0
	bl UiText_DrawCharacterAtOffset
	ldr r3, [sp, #76]
	mov r2, r10
	strh r2, [r3, #12]
	movs r0, #112
	b .L_08027f40
	.2byte 0x0000
.L_08027f34:
	.4byte 0x000003ff
.L_08027f38:
	.4byte 0xfffffc00
.L_08027f3c:
	.4byte 0x00000182
.L_08027f40:
	bl AudioCommand_PlayFar
	mov r0, r8
	bl Battle_ClassifyEntryKind
	adds r3, r0, #0
	mov r0, r8
	ldrb r1, [r0]
	mov r2, r10
	ldr r0, [sp, #64]
	bl BattleTarget_RunSelection
	ldr r1, [sp, #36]
	ldr r3, [r1]
	movs r2, #0
	adds r3, #37
	adds r6, r0, #0
	strb r2, [r3]
	mov r0, r9
	bl Resource_ResetEntry
	mov r0, r11
	movs r1, #1
	bl UiWork_Finalize
	cmp r6, r7
	bne .L_08027f78
	b .L_08027d7c
.L_08027f78:
	movs r3, #2
	str r3, [sp, #60]
	b .L_080278c2
.L_08027f7e:
	movs r0, #3
	str r0, [sp, #60]
.L_08027f82:
	movs r0, #110
	bl AudioCommand_PlayFar
	add r1, sp, #64
	ldrh r1, [r1]
	ldr r2, [sp, #76]
	strh r1, [r2]
	ldr r5, [sp, #72]
	adds r5, #64
	ldrh r6, [r5]
	cmp r6, #0
	beq .L_08027fa6
	bl Random16
	ldrh r3, [r5]
	muls r3, r0
	lsrs r3, r3, #20
	adds r6, r6, r3
.L_08027fa6:
	ldr r2, [sp, #76]
	strh r6, [r2, #4]
	ldr r3, [sp, #44]
	cmp r3, #0
	beq .L_08027fce
	ldr r1, [sp, #84]
	ldr r0, [sp, #32]
	adds r3, r0, r1
	subs r2, r3, #2
	ldrh r1, [r3]
	ldrh r3, [r2]
	cmp r1, r3
	bne .L_08027fce
	lsls r2, r6, #16
	asrs r3, r2, #16
	lsrs r2, r2, #31
	adds r3, r3, r2
	ldr r2, [sp, #76]
	asrs r3, r3, #1
	strh r3, [r2, #4]
.L_08027fce:
	ldr r1, [sp, #76]
	movs r0, #4
	ldrsh r3, [r1, r0]
	cmp r3, #0
	bge .L_08027fe0
	movs r3, #250
	lsls r3, r3, #3
	adds r2, r1, #0
	strh r3, [r2, #4]
.L_08027fe0:
	add r3, sp, #60
	ldrh r3, [r3]
	ldr r0, [sp, #76]
	strh r3, [r0, #6]
	add r0, sp, #56
	ldrh r0, [r0]
	ldr r1, [sp, #76]
	strh r0, [r1, #8]
	add r1, sp, #68
	ldrh r1, [r1]
	ldr r2, [sp, #76]
	strh r1, [r2, #10]
	ldr r2, [sp, #44]
	adds r2, #1
	lsls r3, r2, #2
	str r3, [sp, #28]
	ldr r3, [sp, #80]
	lsls r0, r2, #4
	lsls r1, r2, #1
	str r2, [sp, #44]
	str r0, [sp, #24]
	str r1, [sp, #32]
	cmp r2, r3
	bge .L_08028014
	bl .L_0802739e
.L_08028014:
	ldr r0, [sp, #44]
	ldr r1, [sp, #80]
	cmp r0, r1
	bge .L_08028020
	bl .L_080272a8
.L_08028020:
	ldr r2, [sp, #36]
	ldr r0, [r2]
	ldr r3, [r0, #80]
	cmp r3, #0
	beq .L_0802803c
	ldr r2, .L_08028070
	ldr r3, .L_08028060
	strh r3, [r2, #8]
	ldr r3, .L_08028064
	strh r3, [r2, #10]
	ldr r3, .L_08028068
	strh r3, [r2, #12]
	ldr r3, .L_0802806c
	strh r3, [r2, #14]
.L_0802803c:
	ldr r0, [r0, #68]
	cmp r0, #0
	beq .L_08028048
	movs r1, #1
	bl UiWork_Finalize
.L_08028048:
	ldr r0, [sp, #48]
	bl Resource_ResetEntry
	ldr r0, [sp, #52]
	bl Resource_ResetEntry
	ldr r0, .L_08028074
	bl Scheduler_RemoveCallback
	ldr r3, [sp, #36]
	ldr r2, [r3]
	b .L_08028078
.L_08028060:
	.4byte 0x00000045
.L_08028064:
	.4byte 0x00000044
.L_08028068:
	.4byte 0x00000056
.L_0802806c:
	.4byte 0x00000053
.L_08028070:
	.4byte gSerialTransfer + 0x4
.L_08028074:
	.4byte UpdateLinkSessionCountdown
.L_08028078:
	ldr r3, [r2, #80]
	cmp r3, #0
	beq .L_08028166
	ldr r3, .L_08028188
	ldr r5, [r3]
	ldr r3, [r2, #68]
	movs r6, #0
	cmp r3, #0
	bne .L_080280b8
	adds r7, r5, #0
	adds r7, #82
	ldrb r3, [r7]
	cmp r3, #0
	bne .L_080280bc
	movs r3, #42
	str r3, [sp, #0]
	movs r2, #30
	movs r1, #16
	movs r3, #4
	movs r0, #0
	bl UiWindow_Create
	ldr r1, [sp, #36]
	ldr r3, [r1]
	str r0, [r3, #68]
	bl Ui_FillVramBlockPattern
	add r2, sp, #100
	mov r9, r2
	bl UiText_DrawLocalizedResource80d
	b .L_080280c4
.L_080280b8:
	adds r7, r5, #0
	adds r7, #82
.L_080280bc:
	ldr r3, [sp, #36]
	ldr r2, [r3]
	movs r3, #0
	str r3, [r2, #68]
.L_080280c4:
	adds r5, #80
	ldrb r2, [r5]
	movs r3, #1
	eors r3, r2
	lsls r2, r3, #1
	adds r2, r2, r3
	ldr r3, .L_0802818c
	lsls r2, r2, #3
	adds r1, r2, r3
	ldrb r3, [r7]
	cmp r3, #0
	beq .L_080280e4
	movs r0, #1
	negs r0, r0
	str r0, [sp, #80]
	b .L_08028156
.L_080280e4:
	ldr r3, .L_08028190
	ldrh r2, [r3]
	movs r3, #3
	ands r3, r2
	cmp r3, #3
	beq .L_080280fe
	adds r6, #1
	cmp r6, #24
	ble .L_08028134
	movs r1, #1
	negs r1, r1
	str r1, [sp, #80]
	b .L_08028156
.L_080280fe:
	ldrh r2, [r1, #8]
	adds r3, r2, #0
	movs r6, #0
	cmp r3, #69
	bne .L_0802811a
	ldrh r3, [r1, #10]
	cmp r3, #68
	bne .L_0802811a
	ldrh r3, [r1, #12]
	cmp r3, #86
	bne .L_0802811a
	ldrh r3, [r1, #14]
	cmp r3, #83
	beq .L_08028156
.L_0802811a:
	adds r3, r2, #0
	cmp r3, #86
	bne .L_08028132
	ldrh r3, [r1, #10]
	cmp r3, #83
	bne .L_08028132
	ldrh r3, [r1, #12]
	cmp r3, #83
	bne .L_08028132
	ldrh r3, [r1, #14]
	cmp r3, #84
	beq .L_08028134
.L_08028132:
	movs r6, #1
.L_08028134:
	movs r0, #1
	bl WaitFrames
	ldrb r2, [r5]
	movs r3, #1
	eors r3, r2
	lsls r2, r3, #1
	adds r2, r2, r3
	ldr r3, .L_0802818c
	lsls r2, r2, #3
	adds r1, r2, r3
	ldrb r3, [r7]
	cmp r3, #0
	beq .L_080280e4
	movs r2, #1
	negs r2, r2
	str r2, [sp, #80]
.L_08028156:
	ldr r0, [sp, #36]
	ldr r3, [r0]
	ldr r0, [r3, #68]
	cmp r0, #0
	beq .L_08028166
	movs r1, #1
	bl UiWork_Finalize
.L_08028166:
	movs r0, #0
	bl Camera_ConfigureSceneFar
	movs r0, #57
	bl Runtime_ReleaseHeapBlock
	ldr r0, [sp, #80]
	add sp, #100
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r1}
	bx r1
	.2byte 0x0000
.L_08028188:
	.4byte gBattleWork
.L_0802818c:
	.4byte gLinkPeerSignatures
.L_08028190:
	.4byte gLinkStatus
