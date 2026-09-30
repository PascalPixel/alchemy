.syntax unified
	.thumb
	.global Battle_ResolveTargetAction
	.thumb_func
Battle_ResolveTargetAction:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #112
	str r0, [sp, #92]
	movs r0, #0
	str r1, [sp, #88]
	str r0, [sp, #68]
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #36]
	str r0, [sp, #56]
	str r0, [sp, #48]
	str r0, [sp, #44]
	str r0, [sp, #32]
	str r0, [sp, #24]
	movs r0, #166
	lsls r0, r0, #1
	str r3, [sp, #64]
	bl Runtime_BumpAllocate
	movs r1, #156
	lsls r1, r1, #1
	str r0, [sp, #20]
	cmp r11, r1
	beq .L_08120498
	movs r2, #158
	lsls r2, r2, #1
	cmp r11, r2
	bne .L_081204a0
.L_08120498:
	ldr r3, [sp, #92]
	ldrb r3, [r3, #2]
	str r3, [sp, #76]
	b .L_081204a6
.L_081204a0:
	ldr r4, [sp, #92]
	ldrb r4, [r4]
	str r4, [sp, #76]
.L_081204a6:
	ldr r1, [sp, #92]
	ldr r6, [sp, #92]
	ldr r1, [r1, #80]
	ldr r5, [sp, #88]
	ldr r0, [sp, #92]
	ldr r3, [sp, #88]
	adds r6, #3
	ldrb r5, [r6, r5]
	ldr r0, [r0, #76]
	str r1, [sp, #72]
	adds r3, #28
	ldrsb r3, [r6, r3]
	mov r10, r5
	str r3, [sp, #52]
	ldr r5, [sp, #92]
	ldr r3, [sp, #88]
	adds r5, #1
	adds r3, #44
	ldrsb r3, [r5, r3]
	mov r11, r0
	str r3, [sp, #36]
	bl BattleAction_Get
	str r0, [sp, #84]
	ldr r0, [sp, #76]
	bl Owner_GetState
	str r0, [sp, #80]
	mov r0, r10
	bl Owner_GetState
	movs r2, #166
	mov r8, r0
	ldr r3, .L_081207bc
	lsls r2, r2, #1
	ldr r0, [sp, #20]
	mov r1, r8
	mov lr, r3
	.2byte 0xf800
	movs r3, #165
	lsls r3, r3, #1
	add r3, r8
	ldr r2, [sp, #92]
	ldrh r3, [r3]
	adds r2, #74
	str r2, [sp, #4]
	cmp r3, #101
	blt .L_081205a0
	cmp r3, #103
	ble .L_0812053a
	cmp r3, #221
	bne .L_08120582
	adds r3, r2, #0
	movs r4, #0
	ldrsh r3, [r3, r4]
	cmp r3, #1
	bne .L_081205a0
	ldr r3, [sp, #76]
	mov r0, r10
	eors r3, r0
	lsrs r3, r3, #7
	cmp r3, #0
	beq .L_081205a0
	movs r0, #11
	mov r1, r10
	bl BattleEv_Push
	movs r0, #0
	mov r1, r10
	bl BattleEv_Push
	ldr r1, .L_081207c0
	bl .L_08122326
.L_0812053a:
	movs r1, #159
	lsls r1, r1, #1
	adds r1, #255
	cmp r11, r1
	beq .L_0812058a
	ldr r2, [sp, #64]
	movs r4, #128
	lsls r4, r4, #4
	adds r4, #104
	adds r3, r2, r4
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_08120592
	ldr r3, [sp, #76]
	mov r0, r10
	eors r3, r0
	lsrs r3, r3, #7
	cmp r3, #0
	beq .L_0812059a
	movs r0, #11
	mov r1, r10
	bl BattleEv_Push
	movs r0, #0
	mov r1, r10
	bl BattleEv_Push
	ldr r1, .L_081207c4
	movs r0, #4
	bl BattleEv_Push
	ldr r1, [sp, #92]
	adds r1, #74
	str r1, [sp, #4]
	bl .L_081223ba
.L_08120582:
	ldr r2, [sp, #92]
	adds r2, #74
	str r2, [sp, #4]
	b .L_081205a0
.L_0812058a:
	ldr r3, [sp, #92]
	adds r3, #74
	str r3, [sp, #4]
	b .L_081205a0
.L_08120592:
	ldr r4, [sp, #92]
	adds r4, #74
	str r4, [sp, #4]
	b .L_081205a0
.L_0812059a:
	ldr r0, [sp, #92]
	adds r0, #74
	str r0, [sp, #4]
.L_081205a0:
	movs r3, #44
	adds r3, #255
	add r3, r8
	ldrb r3, [r3]
	cmp r3, #4
	bne .L_081205cc
	ldr r3, [sp, #76]
	mov r1, r10
	eors r3, r1
	lsrs r3, r3, #7
	cmp r3, #0
	beq .L_081205cc
	movs r0, #11
	bl BattleEv_Push
	movs r0, #0
	mov r1, r10
	bl BattleEv_Push
	ldr r1, .L_081207c8
	bl .L_08122326
.L_081205cc:
	ldr r2, [sp, #84]
	ldrb r3, [r2, #8]
	cmp r3, #255
	beq .L_081205e4
	ldr r3, [sp, #88]
	adds r3, #16
	ldrsb r3, [r5, r3]
	str r3, [sp, #60]
	cmp r3, #0
	bge .L_081205e8
	negs r3, r3
	b .L_081205e6
.L_081205e4:
	movs r3, #0
.L_081205e6:
	str r3, [sp, #60]
.L_081205e8:
	ldr r4, [sp, #72]
	cmp r4, #4
	beq .L_08120646
	lsls r3, r4, #2
	mov r4, r8
	add r3, r8
	adds r4, #36
	movs r5, #38
	ldrsh r0, [r3, r5]
	movs r2, #2
	ldrsh r3, [r4, r2]
	movs r1, #0
	cmp r0, r3
	blt .L_08120616
	adds r2, r4, #0
.L_08120606:
	adds r1, #1
	adds r2, #4
	cmp r1, #3
	bgt .L_08120616
	movs r5, #2
	ldrsh r3, [r2, r5]
	cmp r0, r3
	bge .L_08120606
.L_08120616:
	cmp r1, #4
	bne .L_08120620
	movs r1, #1
	negs r1, r1
	str r1, [sp, #24]
.L_08120620:
	movs r2, #2
	ldrsh r3, [r4, r2]
	movs r1, #0
	cmp r0, r3
	bgt .L_0812063e
	mov r2, r8
	adds r2, #36
.L_0812062e:
	adds r1, #1
	adds r2, #4
	cmp r1, #3
	bgt .L_0812063e
	movs r4, #2
	ldrsh r3, [r2, r4]
	cmp r0, r3
	ble .L_0812062e
.L_0812063e:
	cmp r1, #4
	bne .L_08120646
	movs r5, #1
	str r5, [sp, #24]
.L_08120646:
	ldr r0, [sp, #92]
	ldr r2, [r0, #80]
	cmp r2, #3
	bhi .L_08120664
	ldr r4, [sp, #4]
	movs r1, #0
	ldrsh r3, [r4, r1]
	cmp r3, #2
	beq .L_08120664
	ldr r5, [sp, #80]
	lsls r3, r2, #2
	adds r3, #72
	ldrsh r5, [r5, r3]
	str r5, [sp, #16]
	b .L_08120668
.L_08120664:
	movs r1, #100
	str r1, [sp, #16]
.L_08120668:
	ldr r5, [sp, #4]
	movs r4, #0
	ldrsh r3, [r5, r4]
	cmp r3, #5
	bne .L_081206ac
	cmp r2, #3
	bhi .L_081206ac
	ldr r0, [sp, #24]
	cmp r0, #0
	ble .L_081206ac
	lsls r3, r2, #2
	adds r3, #72
	add r3, r8
	movs r1, #2
	ldrsh r5, [r3, r1]
	ldr r2, [sp, #16]
	movs r3, #200
	subs r5, r2, r5
	lsls r3, r3, #1
	adds r3, #255
	adds r5, #30
	muls r5, r3
	bl BattleRandom16Far
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	ands r0, r3
	cmp r5, r0
	ble .L_081206ac
	movs r0, #13
	movs r1, #5
	bl BattleEv_Push
.L_081206ac:
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #174
	cmp r11, r3
	beq .L_081206be
	movs r4, #102
	adds r4, #255
	cmp r11, r4
	bne .L_081206c8
.L_081206be:
	mov r0, r10
	movs r1, #0
	movs r2, #0
	bl Battle_ApplyActionExtras
.L_081206c8:
	ldr r3, .L_081207cc
	add r3, r11
	cmp r3, #1
	bhi .L_081206da
	mov r0, r10
	movs r1, #1
	movs r2, #0
	bl Battle_ApplyActionExtras
.L_081206da:
	movs r5, #182
	lsls r5, r5, #2
	cmp r11, r5
	bne .L_081206ec
	mov r0, r10
	movs r1, #1
	movs r2, #1
	bl Battle_ApplyActionExtras
.L_081206ec:
	ldr r0, [sp, #84]
	movs r1, #15
	ldrb r3, [r0, #1]
	movs r2, #1
	ands r1, r3
	ldr r3, [sp, #88]
	str r1, [sp, #28]
	adds r3, #56
	ldrsb r0, [r6, r3]
	negs r2, r2
	cmp r0, r2
	bne .L_0812071a
	ldr r2, .L_081207d0
	ldr r5, [sp, #60]
	ldr r4, [sp, #84]
	ldrb r2, [r2, r5]
	ldrb r3, [r4, #3]
	ldr r0, [sp, #76]
	str r2, [sp, #0]
	mov r1, r10
	ldr r2, [sp, #72]
	bl Battle_HitCheck
.L_0812071a:
	str r0, [sp, #40]
	ldr r0, [sp, #84]
	movs r2, #128
	ldrb r1, [r0, #3]
	lsls r2, r2, #17
	adds r3, r1, #0
	adds r3, #206
	lsls r3, r3, #24
	cmp r3, r2
	bls .L_0812073a
	adds r3, r1, #0
	cmp r3, #86
	beq .L_0812073a
	cmp r3, #87
	beq .L_0812073a
	b .L_08120964
.L_0812073a:
	ldr r3, [sp, #80]
	movs r4, #165
	lsls r4, r4, #1
	adds r6, r3, r4
	ldrh r5, [r6]
	bl Summon_FindSlot
	mov r9, r0
	ldr r0, [sp, #84]
	movs r7, #1
	ldrb r3, [r0, #3]
	negs r7, r7
	cmp r3, #51
	bne .L_08120762
	ldr r1, [sp, #64]
	ldr r0, [r1]
	bl BattleFormation_SelectRandomAvailableMember
	adds r5, r0, #0
	b .L_081207d8
.L_08120762:
	cmp r3, #86
	bne .L_08120780
	ldrh r3, [r6]
	cmp r3, #164
	bne .L_0812077c
	bl BattleRandom16Far
	movs r3, #3
	movs r2, #189
	ands r3, r0
	lsls r2, r2, #1
	adds r5, r3, r2
	b .L_081207d8
.L_0812077c:
	movs r5, #81
	b .L_081207d8
.L_08120780:
	cmp r3, #87
	bne .L_081207d8
	ldr r4, [sp, #64]
	movs r0, #160
	lsls r0, r0, #3
	adds r0, #107
	adds r3, r4, r0
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	beq .L_081207d4
	movs r1, #160
	lsls r1, r1, #3
	adds r1, #106
	adds r3, r4, r1
	movs r2, #0
	ldrsb r2, [r3, r2]
	movs r4, #160
	ldr r0, [sp, #64]
	lsls r4, r4, #3
	lsls r3, r2, #1
	adds r4, #100
	subs r1, #2
	adds r3, r3, r4
	adds r2, r2, r1
	ldrh r5, [r0, r3]
	ldrb r7, [r0, r2]
	b .L_081207d8
	.2byte 0x0000
.L_081207bc:
	.4byte IwramCopyWords
.L_081207c0:
	.4byte 0x00000cb5
.L_081207c4:
	.4byte 0x00000cb4
.L_081207c8:
	.4byte 0x00000cab
.L_081207cc:
	.4byte 0xfffffd30
.L_081207d0:
	.4byte HitFalloff
.L_081207d4:
	movs r2, #0
	str r2, [sp, #40]
.L_081207d8:
	ldr r3, [sp, #40]
	cmp r3, #0
	bne .L_081207e0
	b .L_08120942
.L_081207e0:
	adds r0, r5, #0
	bl Summon_ClassValid
	cmp r0, #0
	bne .L_081207ec
	b .L_08120942
.L_081207ec:
	mov r4, r9
	cmp r4, #0
	bge .L_081207f4
	b .L_08120942
.L_081207f4:
	movs r0, #1
	negs r0, r0
	cmp r7, r0
	bne .L_08120816
	adds r0, r5, #0
	movs r1, #1
	bl Summon_TakeCharge
	movs r3, #128
	adds r7, r0, #0
	lsls r3, r3, #8
	ands r3, r7
	cmp r3, #0
	beq .L_08120816
	adds r0, r5, #0
	bl Summon_ResetCharge
.L_08120816:
	movs r2, #254
	lsls r2, r2, #7
	adds r2, #255
	adds r1, r5, #0
	ands r2, r7
	mov r0, r9
	bl BattleUnit_AssignFar
	ldr r1, [sp, #84]
	ldrb r3, [r1, #3]
	cmp r3, #87
	bne .L_0812085a
	ldr r3, [sp, #64]
	movs r4, #160
	lsls r4, r4, #3
	adds r4, #107
	adds r2, r3, r4
	ldrb r3, [r2]
	movs r0, #160
	subs r3, #1
	strb r3, [r2]
	ldr r5, [sp, #64]
	lsls r0, r0, #3
	adds r0, #106
	adds r1, r5, r0
	movs r2, #0
	ldrsb r2, [r1, r2]
	adds r2, #1
	lsrs r3, r2, #31
	adds r3, r2, r3
	asrs r3, r3, #1
	lsls r3, r3, #1
	subs r2, r2, r3
	strb r2, [r1]
.L_0812085a:
	ldr r1, [sp, #80]
	movs r2, #165
	lsls r2, r2, #1
	adds r3, r1, r2
	ldrh r3, [r3]
	cmp r3, #164
	bne .L_08120874
	ldr r0, [sp, #64]
	mov r1, r9
	adds r0, #102
	bl BattleParty_InsertUnitCentered
	b .L_081208b8
.L_08120874:
	ldr r1, [sp, #64]
	movs r2, #100
	adds r1, #2
	ldrsh r3, [r1, r2]
	movs r5, #0
	movs r6, #0
	adds r7, r1, #0
	movs r0, #100
	movs r4, #0
	cmp r3, #254
	bne .L_0812089c
	mov r5, r9
	strh r5, [r1, r2]
	b .L_081208b8
.L_08120890:
	mov r3, r9
	strh r3, [r1, r0]
	adds r3, r6, #0
	adds r3, #102
	strh r2, [r1, r3]
	b .L_081208b8
.L_0812089c:
	ldrsh r2, [r0, r7]
	cmp r2, #255
	beq .L_08120890
	adds r5, #1
	adds r0, #2
	adds r4, #2
	cmp r5, #5
	bgt .L_081208b8
	ldrsh r3, [r0, r1]
	adds r6, r4, #0
	cmp r3, #254
	bne .L_0812089c
	mov r3, r9
	strh r3, [r0, r1]
.L_081208b8:
	bl Summon_Refresh
	mov r0, r9
	bl GetBattleObjectSlot
	ldr r2, [r0, #12]
	cmp r2, #0
	bge .L_081208d0
	movs r4, #255
	lsls r4, r4, #8
	adds r4, #255
	adds r2, r2, r4
.L_081208d0:
	ldr r3, [r0, #16]
	asrs r2, r2, #16
	cmp r3, #0
	bge .L_081208e0
	movs r5, #255
	lsls r5, r5, #8
	adds r5, #255
	adds r3, r3, r5
.L_081208e0:
	asrs r3, r3, #16
	mov r1, r9
	bl BattlePresentation_SpawnActorObject
	bl BattleActor_CommitPlacement
	add r5, sp, #96
	adds r0, r5, #0
	bl BattleParty_ListPresentEnemies
	cmp r0, #0
	ble .L_0812090a
	adds r6, r5, #0
	adds r5, r0, #0
.L_081208fc:
	ldrh r0, [r6]
	subs r5, #1
	adds r6, #2
	bl Actor_ResetMotionAtAnchor
	cmp r5, #0
	bne .L_081208fc
.L_0812090a:
	movs r0, #0
	mov r1, r9
	bl BattleEv_Push
	ldr r0, [sp, #84]
	ldrb r3, [r0, #3]
	cmp r3, #87
	bne .L_0812091e
	ldr r1, .L_08120bc8
	b .L_0812094c
.L_0812091e:
	movs r3, #248
	adds r3, #255
	cmp r11, r3
	beq .L_08120934
	ldr r1, .L_08120bcc
	movs r0, #4
	bl BattleEv_Push
	ldr r4, [sp, #84]
	ldrb r1, [r4, #3]
	b .L_08120964
.L_08120934:
	ldr r1, .L_08120bd0
	movs r0, #4
	bl BattleEv_Push
	ldr r5, [sp, #84]
	ldrb r1, [r5, #3]
	b .L_08120964
.L_08120942:
	movs r0, #248
	adds r0, #255
	cmp r11, r0
	bne .L_08120958
	ldr r1, .L_08120bd4
.L_0812094c:
	movs r0, #4
	bl BattleEv_Push
	ldr r2, [sp, #84]
	ldrb r1, [r2, #3]
	b .L_08120964
.L_08120958:
	ldr r1, .L_08120bd8
	movs r0, #4
	bl BattleEv_Push
	ldr r3, [sp, #84]
	ldrb r1, [r3, #3]
.L_08120964:
	ldr r4, [sp, #40]
	cmp r4, #0
	beq .L_08120a10
	adds r3, r1, #0
	cmp r3, #53
	beq .L_08120974
	cmp r3, #83
	bne .L_081209a8
.L_08120974:
	movs r5, #0
	str r5, [sp, #40]
	ldr r0, [sp, #64]
	movs r3, #187
	lsls r3, r3, #2
	ldrsh r3, [r0, r3]
	movs r2, #0
	cmp r3, r10
	bne .L_0812098c
	movs r5, #1
	str r5, [sp, #40]
	b .L_08120a10
.L_0812098c:
	adds r2, #1
	cmp r2, #19
	bhi .L_08120a10
	movs r0, #187
	ldr r4, [sp, #64]
	lsls r3, r2, #4
	lsls r0, r0, #2
	adds r3, r3, r0
	ldrsh r3, [r4, r3]
	cmp r3, r10
	bne .L_0812098c
	movs r0, #1
	str r0, [sp, #40]
	b .L_08120a10
.L_081209a8:
	adds r3, r1, #0
	cmp r3, #35
	bne .L_081209b4
	movs r2, #1
	str r2, [sp, #56]
	b .L_08120a10
.L_081209b4:
	cmp r3, #34
	bne .L_081209be
	movs r3, #1
	str r3, [sp, #44]
	b .L_08120a10
.L_081209be:
	cmp r3, #27
	bne .L_081209c8
	movs r4, #1
	str r4, [sp, #32]
	b .L_08120a10
.L_081209c8:
	cmp r3, #55
	bne .L_081209e4
	ldr r0, [sp, #80]
	movs r5, #56
	ldrsh r3, [r0, r5]
	cmp r3, #0
	beq .L_08120a10
	ldr r1, [sp, #76]
	movs r0, #12
	bl BattleEv_Push
	ldr r2, [sp, #84]
	ldrb r1, [r2, #3]
	b .L_08120a10
.L_081209e4:
	cmp r3, #32
	bne .L_081209fe
	mov r5, r8
	movs r4, #58
	ldrsh r3, [r5, r4]
	cmp r3, #0
	beq .L_081209f8
	movs r0, #10
	str r0, [sp, #28]
	b .L_08120a10
.L_081209f8:
	movs r2, #0
	str r2, [sp, #40]
	b .L_08120a10
.L_081209fe:
	cmp r3, #90
	bne .L_08120a08
	movs r3, #2
	str r3, [sp, #56]
	b .L_08120a10
.L_08120a08:
	cmp r3, #91
	bne .L_08120a10
	movs r4, #2
	str r4, [sp, #56]
.L_08120a10:
	adds r3, r1, #0
	cmp r3, #75
	bne .L_08120a20
	ldr r5, [sp, #88]
	cmp r5, #0
	beq .L_08120a20
	bl .L_08121480
.L_08120a20:
	ldr r0, [sp, #32]
	cmp r0, #0
	beq .L_08120a2a
	bl .L_08121480
.L_08120a2a:
	mov r2, r8
	movs r1, #56
	ldrsh r3, [r2, r1]
	cmp r3, #0
	bne .L_08120a44
	ldr r3, [sp, #84]
	ldrb r0, [r3, #3]
	bl BattleFx_IsReviveFar
	cmp r0, #0
	bne .L_08120a44
	bl .L_08121480
.L_08120a44:
	ldr r3, [sp, #28]
	adds r3, #1
	cmp r3, #12
	bls .L_08120a50
	bl .L_08121480
.L_08120a50:
	ldr r2, .L_08120bdc
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
.L_08120a58:
	.4byte .L_08120e74
	.4byte .L_08121480
	.4byte .L_08120dce
	.4byte .L_08121304
	.4byte .L_08120a8c
	.4byte .L_08120a8c
	.4byte .L_08120f70
	.4byte .L_08120f70
	.4byte .L_08121480
	.4byte .L_08120f70
	.4byte .L_08121480
	.4byte .L_08120ce2
	.4byte .L_08121278
.L_08120a8c:
	movs r3, #44
	adds r3, #255
	add r3, r8
	ldrb r3, [r3]
	cmp r3, #4
	bne .L_08120a9c
	bl .L_08121480
.L_08120a9c:
	mov r4, r8
	movs r5, #56
	ldrsh r4, [r4, r5]
	mov r5, r8
	ldrh r5, [r5, #62]
	ldr r0, [sp, #56]
	mov r9, r4
	str r5, [sp, #12]
	cmp r0, #0
	beq .L_08120ac0
	lsrs r3, r5, #31
	adds r3, r5, r3
	asrs r3, r3, #1
	str r3, [sp, #12]
	cmp r0, #2
	bne .L_08120ac0
	movs r1, #0
	str r1, [sp, #12]
.L_08120ac0:
	movs r2, #1
	str r2, [sp, #8]
.L_08120ac4:
	ldr r3, [sp, #72]
	cmp r3, #4
	beq .L_08120ada
	lsls r3, r3, #2
	adds r3, #72
	add r3, r8
	movs r4, #2
	ldrsh r3, [r3, r4]
	ldr r5, [sp, #16]
	subs r3, r5, r3
	str r3, [sp, #68]
.L_08120ada:
	ldr r0, [sp, #8]
	cmp r0, #0
	bne .L_08120ae4
	movs r1, #0
	str r1, [sp, #68]
.L_08120ae4:
	ldr r2, [sp, #84]
	ldr r3, [sp, #28]
	ldrh r7, [r2, #10]
	cmp r3, #4
	bne .L_08120b06
	ldr r4, [sp, #80]
	ldr r1, [sp, #12]
	ldrh r0, [r4, #60]
	movs r2, #0
	ldr r3, [sp, #68]
	bl Battle_CalcAttack
	movs r1, #10
	muls r0, r7
	bl __divsi3
	b .L_08120b42
.L_08120b06:
	ldr r0, [sp, #80]
	movs r6, #156
	lsls r6, r6, #1
	ldrh r5, [r0, #60]
	cmp r11, r6
	beq .L_08120b1a
	movs r1, #158
	lsls r1, r1, #1
	cmp r11, r1
	bne .L_08120b36
.L_08120b1a:
	ldr r2, [sp, #92]
	ldrb r0, [r2]
	bl Owner_GetState
	ldrh r5, [r0, #60]
	cmp r11, r6
	bne .L_08120b36
	ldr r3, [sp, #92]
	ldrb r0, [r3, #2]
	bl Owner_GetState
	ldrh r3, [r0, #60]
	lsrs r3, r3, #2
	adds r5, r5, r3
.L_08120b36:
	adds r0, r5, #0
	ldr r1, [sp, #12]
	adds r2, r7, #0
	ldr r3, [sp, #68]
	bl Battle_CalcAttack
.L_08120b42:
	adds r5, r0, #0
	ldr r4, [sp, #52]
	ldr r0, [sp, #36]
	muls r5, r4
	cmp r0, #0
	beq .L_08120b9a
	cmp r0, #1
	bne .L_08120b60
	lsls r3, r5, #2
	adds r0, r3, r5
	cmp r0, #0
	bge .L_08120b5c
	adds r0, #3
.L_08120b5c:
	asrs r5, r0, #2
	b .L_08120b6a
.L_08120b60:
	lsls r3, r5, #1
	adds r3, r3, r5
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r5, r3, #1
.L_08120b6a:
	mov r1, r8
	ldrb r0, [r1, #15]
	movs r1, #5
	bl Math_DivU
	lsls r0, r0, #24
	ldr r2, [sp, #8]
	lsrs r0, r0, #24
	adds r0, r5, r0
	adds r5, r0, #6
	cmp r2, #0
	bne .L_08120b9a
	movs r1, #0
	movs r0, #6
	bl BattleEv_Push
	mov r3, r10
	ldr r1, .L_08120be0
	cmp r3, #7
	bhi .L_08120b94
	adds r1, #1
.L_08120b94:
	movs r0, #5
	bl BattleEv_Push
.L_08120b9a:
	bl BattleRandom16Far
	movs r3, #3
	ands r3, r0
	adds r5, r5, r3
	movs r3, #44
	adds r3, #255
	add r3, r8
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_08120bee
	cmp r3, #1
	bne .L_08120bbc
	lsrs r3, r5, #31
	adds r3, r5, r3
	asrs r5, r3, #1
	b .L_08120bee
.L_08120bbc:
	cmp r3, #2
	bne .L_08120be4
	lsls r0, r5, #1
	movs r1, #5
	b .L_08120be8
	.2byte 0x0000
.L_08120bc8:
	.4byte 0x00000d64
.L_08120bcc:
	.4byte 0x00000d56
.L_08120bd0:
	.4byte 0x00000d54
.L_08120bd4:
	.4byte 0x00000d55
.L_08120bd8:
	.4byte 0x00000d57
.L_08120bdc:
	.4byte .L_08120a58
.L_08120be0:
	.4byte 0x00000c6e
.L_08120be4:
	adds r0, r5, #0
	movs r1, #10
.L_08120be8:
	bl __divsi3
	adds r5, r0, #0
.L_08120bee:
	cmp r5, #0
	bgt .L_08120bf4
	movs r5, #1
.L_08120bf4:
	ldr r4, [sp, #44]
	cmp r4, #0
	beq .L_08120c0a
	mov r3, r9
	subs r3, #1
	cmp r5, r3
	bge .L_08120c0a
	adds r5, r3, #0
	cmp r5, #0
	bgt .L_08120c0a
	movs r5, #1
.L_08120c0a:
	movs r0, #183
	lsls r0, r0, #1
	bl GameFlag_Test
	cmp r0, #0
	beq .L_08120c28
	ldr r1, [sp, #4]
	movs r0, #0
	ldrsh r3, [r1, r0]
	cmp r3, #5
	bne .L_08120c28
	cmp r9, r5
	bgt .L_08120c28
	mov r5, r9
	subs r5, #1
.L_08120c28:
	ldr r2, [sp, #8]
	adds r2, #1
	str r2, [sp, #8]
	cmp r2, #1
	bgt .L_08120c34
	b .L_08120ac4
.L_08120c34:
	mov r3, r9
	subs r3, r3, r5
	movs r0, #8
	mov r1, r10
	mov r9, r3
	bl BattleEv_Push
	movs r0, #0
	mov r1, r10
	bl BattleEv_Push
	movs r0, #1
	adds r1, r5, #0
	bl BattleEv_Push
	mov r4, r10
	cmp r4, #7
	bhi .L_08120c60
	ldr r3, .L_08120f3c
	ldr r5, [sp, #24]
	adds r1, r5, r3
	b .L_08120c66
.L_08120c60:
	ldr r3, .L_08120f40
	ldr r0, [sp, #24]
	adds r1, r0, r3
.L_08120c66:
	movs r0, #4
	bl BattleEv_Push
	mov r1, r9
	cmp r1, #0
	bgt .L_08120cc6
	mov r0, r10
	bl BattleUnit_KeepsOneHp
	cmp r0, #0
	beq .L_08120c80
	movs r2, #1
	mov r9, r2
.L_08120c80:
	mov r3, r9
	cmp r3, #0
	bgt .L_08120cc6
	movs r4, #0
	movs r0, #9
	mov r1, r10
	mov r9, r4
	bl BattleEv_Push
	movs r3, #165
	lsls r3, r3, #1
	add r3, r8
	ldrh r3, [r3]
	movs r5, #114
	adds r5, #255
	cmp r3, r5
	beq .L_08120cce
	movs r0, #118
	adds r0, #255
	cmp r3, r0
	beq .L_08120cce
	mov r1, r10
	movs r0, #0
	bl BattleEv_Push
	mov r1, r10
	cmp r1, #7
	bhi .L_08120cbc
	ldr r1, .L_08120f44
	b .L_08120cbe
.L_08120cbc:
	ldr r1, .L_08120f48
.L_08120cbe:
	movs r0, #4
	bl BattleEv_Push
	b .L_08120cce
.L_08120cc6:
	movs r0, #11
	mov r1, r10
	bl BattleEv_Push
.L_08120cce:
	mov r4, r8
	movs r2, #56
	ldrsh r3, [r4, r2]
	mov r5, r9
	mov r0, r9
	subs r5, r3, r5
	mov r1, r8
	str r5, [sp, #48]
	strh r0, [r1, #56]
	b .L_081212fc
.L_08120ce2:
	movs r2, #44
	adds r2, #255
	add r2, r8
	ldrb r3, [r2]
	mov r9, r2
	cmp r3, #4
	bne .L_08120cf2
	b .L_08121480
.L_08120cf2:
	ldr r4, [sp, #84]
	ldrh r3, [r4, #10]
	cmp r3, #0
	bne .L_08120cfc
	b .L_08121480
.L_08120cfc:
	ldr r1, [sp, #72]
	mov r0, r8
	movs r5, #58
	ldrsh r6, [r0, r5]
	cmp r1, #4
	beq .L_08120d18
	lsls r3, r1, #2
	adds r3, #72
	add r3, r8
	movs r2, #2
	ldrsh r3, [r3, r2]
	ldr r4, [sp, #16]
	subs r3, r4, r3
	str r3, [sp, #68]
.L_08120d18:
	ldr r5, [sp, #84]
	movs r2, #128
	ldrh r7, [r5, #10]
	ldr r1, [sp, #68]
	lsls r2, r2, #1
	adds r0, r7, #0
	bl Battle_CalcPower
	adds r5, r0, #0
	ldr r0, [sp, #60]
	ldr r2, .L_08120f4c
	lsls r3, r0, #2
	ldr r3, [r2, r3]
	movs r1, #100
	adds r0, r3, #0
	muls r0, r5
	bl __divsi3
	mov r2, r9
	ldr r1, [sp, #52]
	ldrb r3, [r2]
	adds r5, r0, #0
	muls r5, r1
	cmp r3, #0
	beq .L_08120d6a
	cmp r3, #1
	bne .L_08120d56
	lsrs r3, r5, #31
	adds r3, r5, r3
	asrs r5, r3, #1
	b .L_08120d6a
.L_08120d56:
	cmp r3, #2
	bne .L_08120d60
	lsls r0, r5, #1
	movs r1, #5
	b .L_08120d64
.L_08120d60:
	adds r0, r5, #0
	movs r1, #10
.L_08120d64:
	bl __divsi3
	adds r5, r0, #0
.L_08120d6a:
	ldr r4, [sp, #84]
	ldrb r3, [r4, #3]
	cmp r3, #32
	bne .L_08120d78
	cmp r5, r6
	ble .L_08120d7e
	adds r5, r6, #0
.L_08120d78:
	cmp r5, r6
	ble .L_08120d7e
	adds r5, r6, #0
.L_08120d7e:
	movs r0, #8
	mov r1, r10
	bl BattleEv_Push
	movs r0, #1
	adds r1, r5, #0
	bl BattleEv_Push
	movs r0, #0
	mov r1, r10
	bl BattleEv_Push
	mov r0, r10
	cmp r0, #7
	bhi .L_08120da0
	ldr r1, .L_08120f50
	b .L_08120da2
.L_08120da0:
	ldr r1, .L_08120f54
.L_08120da2:
	movs r0, #4
	subs r6, r6, r5
	bl BattleEv_Push
	cmp r6, #0
	bgt .L_08120db0
	movs r6, #0
.L_08120db0:
	movs r0, #11
	mov r1, r10
	bl BattleEv_Push
	mov r2, r8
	movs r1, #58
	ldrsh r3, [r2, r1]
	mov r0, r10
	subs r3, r3, r6
	str r3, [sp, #48]
	mov r3, r8
	strh r6, [r3, #58]
	bl Owner_RecalculateRatiosFar
	b .L_08121480
.L_08120dce:
	ldr r4, [sp, #84]
	ldrh r3, [r4, #10]
	cmp r3, #0
	bne .L_08120dd8
	b .L_08121480
.L_08120dd8:
	mov r0, r8
	movs r5, #56
	ldrsh r6, [r0, r5]
	cmp r6, #0
	bne .L_08120de4
	b .L_08121480
.L_08120de4:
	ldr r2, [sp, #72]
	adds r7, r3, #0
	ldr r1, [sp, #16]
	cmp r2, #4
	bne .L_08120df0
	movs r1, #100
.L_08120df0:
	movs r2, #128
	lsls r2, r2, #1
	adds r0, r7, #0
	bl Battle_CalcRestore
	ldr r4, [sp, #60]
	ldr r2, .L_08120f58
	lsls r3, r4, #2
	ldr r3, [r2, r3]
	adds r5, r0, #0
	adds r0, r3, #0
	muls r0, r5
	movs r1, #100
	bl __divsi3
	adds r5, r0, #0
	ldr r0, [sp, #52]
	muls r5, r0
	bl BattleRandom16Far
	movs r3, #3
	ands r3, r0
	mov r2, r8
	adds r5, r5, r3
	movs r1, #52
	ldrsh r3, [r2, r1]
	adds r6, r6, r5
	cmp r6, r3
	ble .L_08120e32
	adds r6, r3, #0
	movs r4, #56
	ldrsh r3, [r2, r4]
	subs r5, r6, r3
.L_08120e32:
	movs r0, #0
	mov r1, r10
	bl BattleEv_Push
	mov r1, r8
	movs r0, #52
	ldrsh r3, [r1, r0]
	cmp r6, r3
	bne .L_08120e4e
	ldr r1, .L_08120f5c
	movs r0, #4
	bl BattleEv_Push
	b .L_08120e5e
.L_08120e4e:
	movs r0, #1
	adds r1, r5, #0
	bl BattleEv_Push
	ldr r1, .L_08120f60
	movs r0, #4
	bl BattleEv_Push
.L_08120e5e:
	mov r4, r8
	movs r2, #56
	ldrsh r3, [r4, r2]
	mov r5, r8
	subs r3, r3, r6
	str r3, [sp, #48]
	mov r0, r10
	strh r6, [r5, #56]
	bl Owner_RecalculateRatiosFar
	b .L_08121480
.L_08120e74:
	movs r0, #44
	adds r0, #255
	add r0, r8
	ldrb r3, [r0]
	mov r9, r0
	cmp r3, #4
	bne .L_08120e84
	b .L_08121480
.L_08120e84:
	ldr r1, [sp, #84]
	ldrh r3, [r1, #10]
	cmp r3, #0
	bne .L_08120e8e
	b .L_08121480
.L_08120e8e:
	ldr r4, [sp, #72]
	mov r3, r8
	movs r2, #58
	ldrsh r6, [r3, r2]
	cmp r4, #4
	beq .L_08120eaa
	lsls r3, r4, #2
	adds r3, #72
	add r3, r8
	movs r5, #2
	ldrsh r3, [r3, r5]
	ldr r0, [sp, #16]
	subs r3, r0, r3
	str r3, [sp, #68]
.L_08120eaa:
	ldr r1, [sp, #84]
	movs r2, #128
	ldrh r7, [r1, #10]
	lsls r2, r2, #1
	ldr r1, [sp, #68]
	adds r0, r7, #0
	bl Battle_CalcPower
	ldr r4, [sp, #60]
	ldr r2, .L_08120f64
	lsls r3, r4, #2
	ldr r3, [r2, r3]
	adds r5, r0, #0
	adds r0, r3, #0
	muls r0, r5
	movs r1, #100
	bl __divsi3
	mov r1, r9
	adds r5, r0, #0
	ldrb r3, [r1]
	ldr r0, [sp, #52]
	muls r5, r0
	cmp r3, #0
	beq .L_08120efc
	cmp r3, #1
	bne .L_08120ee8
	lsrs r3, r5, #31
	adds r3, r5, r3
	asrs r5, r3, #1
	b .L_08120efc
.L_08120ee8:
	cmp r3, #2
	bne .L_08120ef2
	lsls r0, r5, #1
	movs r1, #5
	b .L_08120ef6
.L_08120ef2:
	adds r0, r5, #0
	movs r1, #10
.L_08120ef6:
	bl __divsi3
	adds r5, r0, #0
.L_08120efc:
	movs r0, #8
	mov r1, r10
	bl BattleEv_Push
	movs r0, #1
	adds r1, r5, #0
	bl BattleEv_Push
	movs r0, #0
	mov r1, r10
	bl BattleEv_Push
	mov r2, r10
	cmp r2, #7
	bhi .L_08120f1e
	ldr r1, .L_08120f68
	b .L_08120f20
.L_08120f1e:
	ldr r1, .L_08120f6c
.L_08120f20:
	movs r0, #4
	subs r6, r6, r5
	bl BattleEv_Push
	cmp r6, #0
	bgt .L_08120f2e
	movs r6, #0
.L_08120f2e:
	movs r0, #11
	mov r1, r10
	bl BattleEv_Push
	mov r3, r8
	strh r6, [r3, #58]
	b .L_081212fc
.L_08120f3c:
	.4byte 0x00000c80
.L_08120f40:
	.4byte 0x00000c7d
.L_08120f44:
	.4byte 0x00000c71
.L_08120f48:
	.4byte 0x00000c70
.L_08120f4c:
	.4byte PpLossFalloff
.L_08120f50:
	.4byte 0x00000c76
.L_08120f54:
	.4byte 0x00000c75
.L_08120f58:
	.4byte HpHealFalloff
.L_08120f5c:
	.4byte 0x00000c6c
.L_08120f60:
	.4byte 0x00000c69
.L_08120f64:
	.4byte PpDmgFalloff
.L_08120f68:
	.4byte 0x00000c73
.L_08120f6c:
	.4byte 0x00000c72
.L_08120f70:
	movs r3, #44
	adds r3, #255
	add r3, r8
	ldrb r3, [r3]
	cmp r3, #4
	bne .L_08120f7e
	b .L_08121480
.L_08120f7e:
	ldr r4, [sp, #84]
	ldrh r3, [r4, #10]
	cmp r3, #0
	bne .L_08120f88
	b .L_08121480
.L_08120f88:
	mov r0, r8
	movs r5, #56
	ldrsh r6, [r0, r5]
	movs r1, #1
	mov r9, r1
.L_08120f92:
	ldr r2, [sp, #72]
	cmp r2, #4
	beq .L_08120fa8
	lsls r3, r2, #2
	adds r3, #72
	add r3, r8
	movs r4, #2
	ldrsh r3, [r3, r4]
	ldr r5, [sp, #16]
	subs r3, r5, r3
	str r3, [sp, #68]
.L_08120fa8:
	mov r0, r9
	cmp r0, #0
	bne .L_08120fb2
	movs r1, #0
	str r1, [sp, #68]
.L_08120fb2:
	ldr r2, [sp, #84]
	movs r3, #214
	lsls r3, r3, #1
	adds r3, #255
	ldrh r7, [r2, #10]
	cmp r11, r3
	beq .L_08120fe2
	movs r4, #209
	lsls r4, r4, #1
	adds r4, #255
	cmp r11, r4
	beq .L_08120fe2
	movs r5, #181
	lsls r5, r5, #2
	cmp r11, r5
	beq .L_08120fe2
	ldr r1, [sp, #4]
	movs r0, #0
	ldrsh r3, [r1, r0]
	cmp r3, #6
	beq .L_08120fe2
	cmp r3, #10
	beq .L_08120fe2
	b .L_08121126
.L_08120fe2:
	movs r3, #199
	lsls r3, r3, #1
	movs r2, #0
	cmp r11, r3
	beq .L_081210ec
	cmp r11, r3
	bgt .L_08121062
	subs r3, #10
	cmp r11, r3
	beq .L_081210dc
	cmp r11, r3
	bgt .L_0812102c
	subs r3, #5
	cmp r11, r3
	beq .L_081210e0
	cmp r11, r3
	bgt .L_08121016
	subs r3, #2
	cmp r11, r3
	beq .L_081210e8
	cmp r11, r3
	bgt .L_081210ec
	subs r3, #1
	cmp r11, r3
	beq .L_081210dc
	b .L_08121106
.L_08121016:
	movs r3, #130
	adds r3, #255
	cmp r11, r3
	beq .L_081210f4
	cmp r11, r3
	blt .L_081210e8
	movs r4, #193
	lsls r4, r4, #1
	cmp r11, r4
	beq .L_08121100
	b .L_08121106
.L_0812102c:
	movs r3, #196
	lsls r3, r3, #1
	cmp r11, r3
	beq .L_081210ec
	cmp r11, r3
	bgt .L_08121044
	subs r3, #2
	cmp r11, r3
	beq .L_081210ec
	cmp r11, r3
	bgt .L_081210e0
	b .L_081210e8
.L_08121044:
	movs r3, #197
	lsls r3, r3, #1
	cmp r11, r3
	beq .L_081210f8
	cmp r11, r3
	blt .L_08121106
	movs r5, #198
	lsls r5, r5, #1
	cmp r11, r5
	beq .L_081210dc
	movs r0, #142
	adds r0, #255
	cmp r11, r0
	beq .L_081210e8
	b .L_08121106
.L_08121062:
	movs r3, #152
	adds r3, #255
	cmp r11, r3
	beq .L_081210e8
	cmp r11, r3
	bgt .L_0812109c
	subs r3, #5
	cmp r11, r3
	beq .L_081210f0
	cmp r11, r3
	bgt .L_08121086
	subs r3, #2
	cmp r11, r3
	beq .L_081210e8
	movs r2, #12
	cmp r11, r3
	bgt .L_08121106
	b .L_081210e0
.L_08121086:
	movs r3, #202
	lsls r3, r3, #1
	cmp r11, r3
	beq .L_08121104
	cmp r11, r3
	blt .L_081210f4
	movs r1, #203
	lsls r1, r1, #1
	cmp r11, r1
	beq .L_081210dc
	b .L_08121106
.L_0812109c:
	movs r3, #156
	adds r3, #255
	cmp r11, r3
	beq .L_081210f4
	cmp r11, r3
	bgt .L_081210b0
	subs r3, #2
	cmp r11, r3
	beq .L_081210e0
	b .L_081210ec
.L_081210b0:
	movs r3, #209
	lsls r3, r3, #1
	adds r3, #255
	cmp r11, r3
	beq .L_08121100
	cmp r11, r3
	bgt .L_081210c8
	movs r3, #206
	lsls r3, r3, #1
	cmp r11, r3
	beq .L_081210fc
	b .L_08121106
.L_081210c8:
	movs r4, #214
	lsls r4, r4, #1
	adds r4, #255
	cmp r11, r4
	beq .L_081210e4
	movs r5, #181
	lsls r5, r5, #2
	cmp r11, r5
	beq .L_08121104
	b .L_08121106
.L_081210dc:
	movs r2, #3
	b .L_08121106
.L_081210e0:
	movs r2, #12
	b .L_08121106
.L_081210e4:
	movs r2, #35
	b .L_08121106
.L_081210e8:
	movs r2, #6
	b .L_08121106
.L_081210ec:
	movs r2, #9
	b .L_08121106
.L_081210f0:
	movs r2, #7
	b .L_08121106
.L_081210f4:
	movs r2, #15
	b .L_08121106
.L_081210f8:
	movs r2, #21
	b .L_08121106
.L_081210fc:
	movs r2, #24
	b .L_08121106
.L_08121100:
	movs r2, #30
	b .L_08121106
.L_08121104:
	movs r2, #40
.L_08121106:
	mov r3, r8
	movs r4, #156
	movs r1, #52
	ldrsh r0, [r3, r1]
	lsls r4, r4, #6
	adds r4, #16
	cmp r0, r4
	ble .L_0812111c
	movs r0, #156
	lsls r0, r0, #6
	adds r0, #16
.L_0812111c:
	muls r0, r2
	movs r1, #100
	bl __divsi3
	adds r7, r7, r0
.L_08121126:
	movs r2, #128
	adds r0, r7, #0
	ldr r1, [sp, #68]
	lsls r2, r2, #1
	bl Battle_CalcPower
	adds r5, r0, #0
	ldr r3, [sp, #28]
	ldr r0, [sp, #52]
	muls r5, r0
	cmp r3, #6
	beq .L_0812115e
	cmp r3, #6
	bgt .L_08121148
	cmp r3, #5
	beq .L_0812114e
	b .L_08121172
.L_08121148:
	cmp r3, #8
	beq .L_08121156
	b .L_08121172
.L_0812114e:
	ldr r1, [sp, #60]
	ldr r2, .L_08121430
	lsls r3, r1, #2
	b .L_08121164
.L_08121156:
	ldr r4, [sp, #60]
	ldr r2, .L_08121434
	lsls r3, r4, #2
	b .L_08121164
.L_0812115e:
	ldr r0, [sp, #60]
	ldr r2, .L_08121438
	lsls r3, r0, #2
.L_08121164:
	ldr r3, [r2, r3]
	movs r1, #100
	adds r0, r3, #0
	muls r0, r5
	bl __divsi3
	adds r5, r0, #0
.L_08121172:
	bl BattleRandom16Far
	movs r3, #3
	ands r3, r0
	adds r5, r5, r3
	movs r3, #44
	adds r3, #255
	add r3, r8
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_081211a8
	cmp r3, #1
	bne .L_08121194
	lsrs r3, r5, #31
	adds r3, r5, r3
	asrs r5, r3, #1
	b .L_081211a8
.L_08121194:
	cmp r3, #2
	bne .L_0812119e
	lsls r0, r5, #1
	movs r1, #5
	b .L_081211a2
.L_0812119e:
	adds r0, r5, #0
	movs r1, #10
.L_081211a2:
	bl __divsi3
	adds r5, r0, #0
.L_081211a8:
	ldr r3, .L_0812143c
	movs r1, #166
	lsls r1, r1, #1
	adds r1, #255
	adds r3, r3, r1
	ldrb r3, [r3]
	cmp r3, #6
	beq .L_081211ce
	movs r0, #183
	lsls r0, r0, #1
	bl GameFlag_Test
	cmp r0, #0
	beq .L_081211d4
	ldr r4, [sp, #4]
	movs r2, #0
	ldrsh r3, [r4, r2]
	cmp r3, #6
	bne .L_081211d4
.L_081211ce:
	cmp r6, r5
	ble .L_081211d4
	adds r5, r6, #0
.L_081211d4:
	movs r0, #1
	add r9, r0
	mov r1, r9
	cmp r1, #1
	bgt .L_081211e0
	b .L_08120f92
.L_081211e0:
	movs r0, #8
	mov r1, r10
	bl BattleEv_Push
	movs r0, #1
	adds r1, r5, #0
	bl BattleEv_Push
	movs r0, #0
	mov r1, r10
	bl BattleEv_Push
	mov r2, r10
	cmp r2, #7
	bhi .L_08121206
	ldr r3, .L_08121440
	ldr r4, [sp, #24]
	adds r1, r4, r3
	b .L_0812120c
.L_08121206:
	ldr r3, .L_08121444
	ldr r0, [sp, #24]
	adds r1, r0, r3
.L_0812120c:
	movs r0, #4
	subs r6, r6, r5
	bl BattleEv_Push
	cmp r6, #0
	bgt .L_08121224
	mov r0, r10
	bl BattleUnit_KeepsOneHp
	cmp r0, #0
	beq .L_08121224
	movs r6, #1
.L_08121224:
	ldr r3, .L_0812143c
	movs r1, #166
	lsls r1, r1, #1
	adds r1, #255
	adds r3, r3, r1
	ldrb r3, [r3]
	cmp r3, #6
	bne .L_08121236
	movs r6, #0
.L_08121236:
	cmp r6, #0
	bgt .L_08121260
	movs r0, #9
	mov r1, r10
	bl BattleEv_Push
	movs r0, #0
	mov r1, r10
	bl BattleEv_Push
	mov r2, r10
	movs r6, #0
	cmp r2, #7
	bhi .L_08121256
	ldr r1, .L_08121448
	b .L_08121258
.L_08121256:
	ldr r1, .L_0812144c
.L_08121258:
	movs r0, #4
	bl BattleEv_Push
	b .L_08121268
.L_08121260:
	movs r0, #11
	mov r1, r10
	bl BattleEv_Push
.L_08121268:
	mov r5, r8
	movs r4, #56
	ldrsh r3, [r5, r4]
	mov r0, r8
	subs r3, r3, r6
	str r3, [sp, #48]
	strh r6, [r0, #56]
	b .L_081212fc
.L_08121278:
	ldr r1, [sp, #84]
	ldrh r3, [r1, #10]
	cmp r3, #0
	bne .L_08121282
	b .L_08121480
.L_08121282:
	ldr r4, [sp, #72]
	mov r3, r8
	ldrh r7, [r1, #10]
	movs r2, #58
	ldrsh r6, [r3, r2]
	ldr r1, [sp, #16]
	cmp r4, #4
	bne .L_08121294
	movs r1, #100
.L_08121294:
	movs r2, #128
	lsls r2, r2, #1
	adds r0, r7, #0
	bl Battle_CalcRestore
	adds r5, r0, #0
	ldr r0, [sp, #60]
	ldr r2, .L_08121450
	lsls r3, r0, #2
	ldr r3, [r2, r3]
	movs r1, #100
	adds r0, r3, #0
	muls r0, r5
	bl __divsi3
	ldr r1, [sp, #52]
	adds r5, r0, #0
	muls r5, r1
	mov r4, r8
	movs r2, #54
	ldrsh r3, [r4, r2]
	adds r6, r6, r5
	cmp r6, r3
	ble .L_081212cc
	adds r6, r3, #0
	movs r5, #58
	ldrsh r3, [r4, r5]
	subs r5, r6, r3
.L_081212cc:
	movs r0, #0
	mov r1, r10
	bl BattleEv_Push
	mov r1, r8
	movs r0, #54
	ldrsh r3, [r1, r0]
	cmp r6, r3
	bne .L_081212e8
	ldr r1, .L_08121454
	movs r0, #4
	bl BattleEv_Push
	b .L_081212f8
.L_081212e8:
	movs r0, #1
	adds r1, r5, #0
	bl BattleEv_Push
	ldr r1, .L_08121458
	movs r0, #4
	bl BattleEv_Push
.L_081212f8:
	mov r2, r8
	strh r6, [r2, #58]
.L_081212fc:
	mov r0, r10
	bl Owner_RecalculateRatiosFar
	b .L_08121480
.L_08121304:
	movs r3, #44
	adds r3, #255
	add r3, r8
	mov r9, r3
	ldrb r3, [r3]
	cmp r3, #4
	bne .L_08121314
	b .L_08121480
.L_08121314:
	ldr r4, [sp, #40]
	cmp r4, #0
	bne .L_0812131c
	b .L_08121468
.L_0812131c:
	ldr r5, [sp, #84]
	ldrh r3, [r5, #10]
	cmp r3, #0
	bne .L_08121326
	b .L_08121480
.L_08121326:
	ldr r2, [sp, #72]
	mov r1, r8
	movs r0, #56
	ldrsh r6, [r1, r0]
	cmp r2, #4
	beq .L_08121342
	lsls r3, r2, #2
	adds r3, #72
	add r3, r8
	movs r4, #2
	ldrsh r3, [r3, r4]
	ldr r5, [sp, #16]
	subs r3, r5, r3
	str r3, [sp, #68]
.L_08121342:
	ldr r0, [sp, #84]
	movs r2, #128
	ldrh r7, [r0, #10]
	ldr r1, [sp, #68]
	lsls r2, r2, #1
	adds r0, r7, #0
	bl Battle_CalcPower
	ldr r4, [sp, #60]
	ldr r1, [sp, #52]
	ldr r2, .L_0812145c
	adds r5, r0, #0
	lsls r3, r4, #2
	muls r5, r1
	ldr r3, [r2, r3]
	movs r1, #100
	adds r0, r3, #0
	muls r0, r5
	bl __divsi3
	adds r5, r0, #0
	mov r0, r9
	ldrb r3, [r0]
	cmp r3, #0
	beq .L_08121394
	cmp r3, #1
	bne .L_08121380
	lsrs r3, r5, #31
	adds r3, r5, r3
	asrs r5, r3, #1
	b .L_08121394
.L_08121380:
	cmp r3, #2
	bne .L_0812138a
	lsls r0, r5, #1
	movs r1, #5
	b .L_0812138e
.L_0812138a:
	adds r0, r5, #0
	movs r1, #10
.L_0812138e:
	bl __divsi3
	adds r5, r0, #0
.L_08121394:
	movs r0, #8
	mov r1, r10
	bl BattleEv_Push
	movs r0, #1
	adds r1, r5, #0
	bl BattleEv_Push
	mov r1, r10
	movs r0, #0
	bl BattleEv_Push
	mov r1, r10
	cmp r1, #7
	bhi .L_081213b6
	ldr r1, .L_08121460
	b .L_081213b8
.L_081213b6:
	ldr r1, .L_08121464
.L_081213b8:
	movs r0, #4
	subs r6, r6, r5
	bl BattleEv_Push
	cmp r6, #0
	bgt .L_08121412
	mov r0, r10
	bl BattleUnit_KeepsOneHp
	cmp r0, #0
	beq .L_081213d0
	movs r6, #1
.L_081213d0:
	cmp r6, #0
	bgt .L_08121412
	movs r0, #9
	mov r1, r10
	bl BattleEv_Push
	movs r3, #165
	lsls r3, r3, #1
	add r3, r8
	ldrh r3, [r3]
	movs r2, #114
	adds r2, #255
	movs r6, #0
	cmp r3, r2
	beq .L_0812141a
	movs r4, #118
	adds r4, #255
	cmp r3, r4
	beq .L_0812141a
	movs r0, #0
	mov r1, r10
	mov r5, r10
	bl BattleEv_Push
	cmp r5, #7
	bhi .L_08121408
	ldr r1, .L_08121448
	b .L_0812140a
.L_08121408:
	ldr r1, .L_0812144c
.L_0812140a:
	movs r0, #4
	bl BattleEv_Push
	b .L_0812141a
.L_08121412:
	movs r0, #11
	mov r1, r10
	bl BattleEv_Push
.L_0812141a:
	mov r1, r8
	movs r0, #56
	ldrsh r3, [r1, r0]
	mov r2, r8
	subs r3, r3, r6
	str r3, [sp, #48]
	mov r0, r10
	strh r6, [r2, #56]
	bl Owner_RecalculateRatiosFar
	b .L_08121480
.L_08121430:
	.4byte HpDmgFalloff5
.L_08121434:
	.4byte HpDmgFalloff8
.L_08121438:
	.4byte HpDmgFalloff6
.L_0812143c:
	.4byte gPartyState
.L_08121440:
	.4byte 0x00000c80
.L_08121444:
	.4byte 0x00000c7d
.L_08121448:
	.4byte 0x00000c71
.L_0812144c:
	.4byte 0x00000c70
.L_08121450:
	.4byte PpHealFalloff
.L_08121454:
	.4byte 0x00000c6d
.L_08121458:
	.4byte 0x00000c6a
.L_0812145c:
	.4byte HpDmgFalloff
.L_08121460:
	.4byte 0x00000c73
.L_08121464:
	.4byte 0x00000c72
.L_08121468:
	movs r0, #11
	mov r1, r10
	bl BattleEv_Push
	movs r0, #0
	mov r1, r10
	bl BattleEv_Push
	ldr r1, .L_08121820
	movs r0, #4
	bl BattleEv_Push
.L_08121480:
	ldr r3, [sp, #64]
	movs r4, #128
	lsls r4, r4, #4
	adds r4, #104
	adds r5, r3, r4
	ldrb r3, [r5]
	cmp r3, #0
	beq .L_081214ae
	movs r0, #159
	lsls r0, r0, #1
	adds r0, #255
	cmp r11, r0
	bne .L_081214ae
	movs r0, #0
	mov r1, r10
	bl BattleEv_Push
	ldr r1, .L_08121824
	movs r0, #4
	bl BattleEv_Push
	movs r3, #0
	strb r3, [r5]
.L_081214ae:
	mov r1, r10
	movs r0, #0
	bl BattleEv_Push
	ldr r1, [sp, #84]
	ldrb r3, [r1, #3]
	cmp r3, #75
	bne .L_081214c8
	ldr r2, [sp, #88]
	cmp r2, #0
	bne .L_081214c8
	bl .L_081223ba
.L_081214c8:
	ldr r3, [sp, #84]
	ldrb r0, [r3, #3]
	bl BattleFx_IsReviveFar
	cmp r0, #0
	bne .L_081214ee
	mov r5, r8
	movs r4, #56
	ldrsh r3, [r5, r4]
	cmp r3, #0
	bne .L_081214ee
	ldr r1, [sp, #84]
	ldrb r0, [r1, #3]
	bl BattleFx_CanAffectDefeatedUnit
	cmp r0, #0
	bne .L_081214ee
	bl .L_081223ba
.L_081214ee:
	ldr r2, [sp, #40]
	cmp r2, #0
	bne .L_081214f8
	bl .L_081223ba
.L_081214f8:
	ldr r4, [sp, #84]
	ldrb r3, [r4, #3]
	subs r3, #3
	cmp r3, #85
	bls .L_08121506
	bl .L_081223ba
.L_08121506:
	ldr r2, .L_08121828
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
	.2byte 0x0000
.L_08121510:
	.4byte .L_08121d34
	.4byte .L_08121756
	.4byte .L_08121c98
	.4byte .L_08121ada
	.4byte .L_08121a88
	.4byte .L_08121a36
	.4byte .L_081219e4
	.4byte .L_08121c48
	.4byte .L_08121bd0
	.4byte .L_08121b7e
	.4byte .L_08121b2c
	.4byte .L_08121e66
	.4byte .L_08121e08
	.4byte .L_08121dac
	.4byte .L_08121d50
	.4byte .L_08121ec4
	.4byte .L_08121ee2
	.4byte .L_08121f00
	.4byte .L_08121f12
	.4byte .L_08121f24
	.4byte .L_08121f36
	.4byte .L_08121f48
	.4byte .L_08121f5a
	.4byte .L_0812223e
	.4byte .L_08121fa2
	.4byte .L_0812224c
	.4byte .L_08122030
	.4byte .L_08122042
	.4byte .L_08122054
	.4byte .L_081220ba
	.4byte .L_081221b8
	.4byte .L_081223ba
	.4byte .L_081223ba
	.4byte .L_081223ba
	.4byte .L_081223ba
	.4byte .L_081223ba
	.4byte .L_081223ba
	.4byte .L_081223ba
	.4byte .L_081223ba
	.4byte .L_081223ba
	.4byte .L_081223ba
	.4byte .L_081223ba
	.4byte .L_08122322
	.4byte .L_081222c4
	.4byte .L_081222f2
	.4byte .L_081223ba
	.4byte .L_081223ba
	.4byte .L_081223ba
	.4byte .L_081223ba
	.4byte .L_081223ba
	.4byte .L_081222b8
	.4byte .L_08122290
	.4byte .L_081223ba
	.4byte .L_08121cb6
	.4byte .L_08121ce4
	.4byte .L_081219b0
	.4byte .L_0812197c
	.4byte .L_08122054
	.4byte .L_08121848
	.4byte .L_08121848
	.4byte .L_081218f2
	.4byte .L_08121668
	.4byte .L_081223ba
	.4byte .L_0812227e
	.4byte .L_08121f7e
	.4byte .L_081223ba
	.4byte .L_08122112
	.4byte .L_08121848
	.4byte .L_08121848
	.4byte .L_081222da
	.4byte .L_08121d0a
	.4byte .L_081223a8
	.4byte .L_08121f00
	.4byte .L_08121848
	.4byte .L_081218f2
	.4byte .L_081218f2
	.4byte .L_0812230a
	.4byte .L_0812224c
	.4byte .L_0812232e
	.4byte .L_0812233e
	.4byte .L_081222b8
	.4byte .L_0812216c
	.4byte .L_08121f36
	.4byte .L_081223ba
	.4byte .L_081223ba
	.4byte .L_081222f2
.L_08121668:
	movs r2, #156
	lsls r2, r2, #1
	add r2, r8
	ldrb r3, [r2]
	cmp r3, #0
	beq .L_08121688
	movs r3, #0
	ldr r1, .L_0812182c
	movs r0, #4
	strb r3, [r2]
	bl BattleEv_Push
	movs r0, #0
	mov r1, r10
	bl BattleEv_Push
.L_08121688:
	movs r2, #60
	adds r2, #255
	add r2, r8
	ldrb r3, [r2]
	cmp r3, #0
	beq .L_081216b0
	movs r3, #0
	strb r3, [r2]
	movs r0, #7
	movs r1, #0
	bl BattleEv_Push
	movs r0, #0
	mov r1, r10
	bl BattleEv_Push
	ldr r1, .L_08121830
	movs r0, #4
	bl BattleEv_Push
.L_081216b0:
	movs r2, #158
	lsls r2, r2, #1
	add r2, r8
	ldrb r3, [r2]
	cmp r3, #0
	beq .L_081216d0
	movs r3, #0
	ldr r1, .L_08121834
	movs r0, #4
	strb r3, [r2]
	bl BattleEv_Push
	movs r0, #0
	mov r1, r10
	bl BattleEv_Push
.L_081216d0:
	movs r2, #62
	adds r2, #255
	add r2, r8
	ldrb r3, [r2]
	cmp r3, #0
	beq .L_081216f0
	movs r3, #0
	ldr r1, .L_08121838
	movs r0, #4
	strb r3, [r2]
	bl BattleEv_Push
	movs r0, #0
	mov r1, r10
	bl BattleEv_Push
.L_081216f0:
	movs r2, #66
	adds r2, #255
	add r2, r8
	ldrb r3, [r2]
	cmp r3, #0
	beq .L_08121710
	movs r3, #0
	ldr r1, .L_0812183c
	movs r0, #4
	strb r3, [r2]
	bl BattleEv_Push
	movs r0, #0
	mov r1, r10
	bl BattleEv_Push
.L_08121710:
	movs r2, #160
	lsls r2, r2, #1
	add r2, r8
	ldrb r3, [r2]
	cmp r3, #0
	beq .L_08121730
	movs r3, #0
	ldr r1, .L_08121840
	movs r0, #4
	strb r3, [r2]
	bl BattleEv_Push
	movs r0, #0
	mov r1, r10
	bl BattleEv_Push
.L_08121730:
	movs r5, #50
	adds r5, #255
	add r5, r8
	movs r3, #0
	ldrsb r3, [r5, r3]
	cmp r3, #0
	beq .L_0812174a
	ldr r1, .L_08121844
	movs r0, #4
	bl BattleEv_Push
	movs r3, #0
	strb r3, [r5]
.L_0812174a:
	movs r0, #7
	movs r1, #0
	bl BattleEv_Push
	bl .L_081223ba
.L_08121756:
	movs r2, #156
	lsls r2, r2, #1
	add r2, r8
	ldrb r3, [r2]
	cmp r3, #0
	beq .L_0812177e
	movs r3, #0
	strb r3, [r2]
	movs r0, #7
	movs r1, #0
	bl BattleEv_Push
	movs r0, #0
	mov r1, r10
	bl BattleEv_Push
	ldr r1, .L_0812182c
	movs r0, #4
	bl BattleEv_Push
.L_0812177e:
	movs r2, #60
	adds r2, #255
	add r2, r8
	ldrb r3, [r2]
	cmp r3, #0
	beq .L_081217a6
	movs r3, #0
	strb r3, [r2]
	movs r0, #7
	movs r1, #0
	bl BattleEv_Push
	movs r0, #0
	mov r1, r10
	bl BattleEv_Push
	ldr r1, .L_08121830
	movs r0, #4
	bl BattleEv_Push
.L_081217a6:
	movs r2, #158
	lsls r2, r2, #1
	add r2, r8
	ldrb r3, [r2]
	cmp r3, #0
	beq .L_081217ce
	movs r3, #0
	strb r3, [r2]
	movs r0, #7
	movs r1, #0
	bl BattleEv_Push
	movs r0, #0
	mov r1, r10
	bl BattleEv_Push
	ldr r1, .L_08121834
	movs r0, #4
	bl BattleEv_Push
.L_081217ce:
	movs r2, #62
	adds r2, #255
	add r2, r8
	ldrb r3, [r2]
	cmp r3, #0
	beq .L_081217f6
	movs r3, #0
	strb r3, [r2]
	movs r0, #7
	movs r1, #0
	bl BattleEv_Push
	movs r0, #0
	mov r1, r10
	bl BattleEv_Push
	ldr r1, .L_08121838
	movs r0, #4
	bl BattleEv_Push
.L_081217f6:
	movs r2, #66
	adds r2, #255
	add r2, r8
	ldrb r3, [r2]
	cmp r3, #0
	bne .L_08121806
	bl .L_081223ba
.L_08121806:
	movs r3, #0
	strb r3, [r2]
	movs r0, #7
	movs r1, #0
	bl BattleEv_Push
	movs r0, #0
	mov r1, r10
	bl BattleEv_Push
	ldr r1, .L_0812183c
	bl .L_08122326
.L_08121820:
	.4byte 0x00000cab
.L_08121824:
	.4byte 0x00000ca7
.L_08121828:
	.4byte .L_08121510
.L_0812182c:
	.4byte 0x00000ceb
.L_08121830:
	.4byte 0x00000ced
.L_08121834:
	.4byte 0x00000ce3
.L_08121838:
	.4byte 0x00000cec
.L_0812183c:
	.4byte 0x00000cf4
.L_08121840:
	.4byte 0x00000cef
.L_08121844:
	.4byte 0x00000ce4
.L_08121848:
	ldr r1, [sp, #84]
	mov r5, r8
	ldrb r2, [r1, #3]
	ldrh r7, [r5, #56]
	movs r0, #56
	ldrsh r5, [r5, r0]
	cmp r2, #76
	bne .L_08121866
	mov r2, r8
	movs r3, #52
	ldrsh r0, [r2, r3]
	movs r1, #5
	lsls r0, r0, #1
	ldrh r6, [r2, #52]
	b .L_081218ac
.L_08121866:
	cmp r2, #71
	bne .L_0812187a
	mov r4, r8
	movs r0, #52
	ldrsh r3, [r4, r0]
	movs r1, #10
	lsls r0, r3, #3
	subs r0, r0, r3
	ldrh r6, [r4, #52]
	b .L_081218ac
.L_0812187a:
	cmp r2, #70
	bne .L_08121890
	mov r1, r8
	ldrh r6, [r1, #52]
	lsls r3, r6, #16
	asrs r2, r3, #16
	lsrs r3, r3, #31
	adds r2, r2, r3
	asrs r2, r2, #1
	adds r5, r5, r2
	b .L_081218b2
.L_08121890:
	mov r3, r8
	ldrh r6, [r3, #52]
	movs r4, #52
	ldrsh r3, [r3, r4]
	cmp r2, #61
	bne .L_081218a4
	lsls r0, r3, #4
	subs r0, r0, r3
	lsls r0, r0, #2
	b .L_081218aa
.L_081218a4:
	lsls r0, r3, #4
	subs r0, r0, r3
	lsls r0, r0, #1
.L_081218aa:
	movs r1, #100
.L_081218ac:
	bl __divsi3
	adds r5, r5, r0
.L_081218b2:
	lsls r3, r6, #16
	asrs r2, r3, #16
	cmp r5, r2
	ble .L_081218bc
	adds r5, r2, #0
.L_081218bc:
	lsls r3, r7, #16
	asrs r3, r3, #16
	subs r1, r5, r3
	cmp r1, #0
	bne .L_081218d0
	ldr r0, [sp, #28]
	cmp r0, #1
	beq .L_081218d0
	bl .L_081223ba
.L_081218d0:
	cmp r5, r2
	bne .L_081218de
	ldr r1, .L_08121c20
	movs r0, #4
	bl BattleEv_Push
	b .L_081218ec
.L_081218de:
	movs r0, #1
	bl BattleEv_Push
	ldr r1, .L_08121c24
	movs r0, #4
	bl BattleEv_Push
.L_081218ec:
	mov r1, r8
	strh r5, [r1, #56]
	b .L_08121fe4
.L_081218f2:
	ldr r4, [sp, #84]
	mov r2, r8
	movs r3, #58
	ldrsh r5, [r2, r3]
	ldrb r3, [r4, #3]
	ldrh r7, [r2, #58]
	cmp r3, #77
	bne .L_08121914
	movs r1, #54
	ldrsh r0, [r2, r1]
	movs r1, #10
	ldrh r6, [r2, #54]
	bl __divsi3
	lsls r0, r0, #16
	asrs r0, r0, #16
	b .L_0812193a
.L_08121914:
	cmp r3, #78
	bne .L_08121928
	mov r2, r8
	movs r4, #54
	ldrsh r3, [r2, r4]
	movs r1, #10
	lsls r0, r3, #1
	adds r0, r0, r3
	ldrh r6, [r2, #54]
	b .L_08121936
.L_08121928:
	mov r0, r8
	movs r1, #54
	ldrsh r3, [r0, r1]
	ldrh r6, [r0, #54]
	lsls r0, r3, #3
	subs r0, r0, r3
	movs r1, #100
.L_08121936:
	bl __divsi3
.L_0812193a:
	adds r5, r5, r0
	lsls r3, r6, #16
	asrs r2, r3, #16
	cmp r5, r2
	ble .L_08121946
	adds r5, r2, #0
.L_08121946:
	lsls r3, r7, #16
	asrs r3, r3, #16
	subs r1, r5, r3
	cmp r1, #0
	bne .L_0812195a
	ldr r3, [sp, #28]
	cmp r3, #11
	beq .L_0812195a
	bl .L_081223ba
.L_0812195a:
	cmp r5, r2
	bne .L_08121968
	ldr r1, .L_08121c28
	movs r0, #4
	bl BattleEv_Push
	b .L_08121976
.L_08121968:
	movs r0, #1
	bl BattleEv_Push
	ldr r1, .L_08121c2c
	movs r0, #4
	bl BattleEv_Push
.L_08121976:
	mov r4, r8
	strh r5, [r4, #58]
	b .L_08121fe4
.L_0812197c:
	movs r2, #72
	adds r2, #255
	add r2, r8
	movs r3, #8
	strb r3, [r2]
	movs r2, #163
	lsls r2, r2, #1
	add r2, r8
	movs r3, #5
	strb r3, [r2]
	mov r0, r10
	bl BattleUnit_Recalculate
	mov r3, r8
	adds r3, #64
	ldrh r1, [r3]
	ldr r3, [sp, #20]
	movs r0, #1
	adds r3, #64
	ldrh r3, [r3]
	subs r1, r1, r3
	bl BattleEv_Push
	ldr r1, .L_08121c30
	bl .L_08122326
.L_081219b0:
	movs r2, #72
	adds r2, #255
	add r2, r8
	movs r3, #252
	strb r3, [r2]
	movs r2, #163
	lsls r2, r2, #1
	add r2, r8
	movs r3, #5
	strb r3, [r2]
	mov r0, r10
	bl BattleUnit_Recalculate
	ldr r3, [sp, #20]
	movs r0, #1
	adds r3, #64
	ldrh r1, [r3]
	mov r3, r8
	adds r3, #64
	ldrh r3, [r3]
	subs r1, r1, r3
	bl BattleEv_Push
	ldr r1, .L_08121c34
	bl .L_08122326
.L_081219e4:
	movs r2, #52
	adds r2, #255
	add r2, r8
	ldrb r3, [r2]
	movs r5, #4
	subs r3, #1
	strb r3, [r2]
	lsls r3, r3, #24
	asrs r3, r3, #24
	negs r5, r5
	cmp r3, r5
	bge .L_08121a00
	movs r3, #252
	strb r3, [r2]
.L_08121a00:
	movs r3, #0
	ldrsb r3, [r2, r3]
	cmp r3, #4
	ble .L_08121a0c
	movs r3, #4
	strb r3, [r2]
.L_08121a0c:
	mov r0, r10
	bl BattleUnit_Recalculate
	ldr r0, [sp, #20]
	mov r2, r8
	ldrh r3, [r2, #60]
	ldrh r1, [r0, #60]
	movs r0, #1
	subs r1, r1, r3
	bl BattleEv_Push
	ldr r1, .L_08121c38
	movs r0, #4
	bl BattleEv_Push
	movs r2, #153
	lsls r2, r2, #1
	add r2, r8
	movs r3, #7
	bl .L_081223b8
.L_08121a36:
	movs r2, #52
	adds r2, #255
	add r2, r8
	ldrb r3, [r2]
	movs r4, #4
	subs r3, #2
	strb r3, [r2]
	lsls r3, r3, #24
	asrs r3, r3, #24
	negs r4, r4
	cmp r3, r4
	bge .L_08121a52
	movs r3, #252
	strb r3, [r2]
.L_08121a52:
	movs r3, #0
	ldrsb r3, [r2, r3]
	cmp r3, #4
	ble .L_08121a5e
	movs r3, #4
	strb r3, [r2]
.L_08121a5e:
	mov r0, r10
	bl BattleUnit_Recalculate
	ldr r5, [sp, #20]
	mov r0, r8
	ldrh r3, [r0, #60]
	ldrh r1, [r5, #60]
	movs r0, #1
	subs r1, r1, r3
	bl BattleEv_Push
	ldr r1, .L_08121c38
	movs r0, #4
	bl BattleEv_Push
	movs r2, #153
	lsls r2, r2, #1
	add r2, r8
	movs r3, #7
	bl .L_081223b8
.L_08121a88:
	movs r2, #52
	adds r2, #255
	add r2, r8
	ldrb r3, [r2]
	movs r1, #4
	adds r3, #1
	strb r3, [r2]
	lsls r3, r3, #24
	asrs r3, r3, #24
	negs r1, r1
	cmp r3, r1
	bge .L_08121aa4
	movs r3, #252
	strb r3, [r2]
.L_08121aa4:
	movs r3, #0
	ldrsb r3, [r2, r3]
	cmp r3, #4
	ble .L_08121ab0
	movs r3, #4
	strb r3, [r2]
.L_08121ab0:
	mov r0, r10
	bl BattleUnit_Recalculate
	ldr r4, [sp, #20]
	mov r2, r8
	ldrh r1, [r2, #60]
	ldrh r3, [r4, #60]
	movs r0, #1
	subs r1, r1, r3
	bl BattleEv_Push
	ldr r1, .L_08121c3c
	movs r0, #4
	bl BattleEv_Push
	movs r2, #153
	lsls r2, r2, #1
	add r2, r8
	movs r3, #7
	bl .L_081223b8
.L_08121ada:
	movs r2, #52
	adds r2, #255
	add r2, r8
	ldrb r3, [r2]
	movs r5, #4
	adds r3, #2
	strb r3, [r2]
	lsls r3, r3, #24
	asrs r3, r3, #24
	negs r5, r5
	cmp r3, r5
	bge .L_08121af6
	movs r3, #252
	strb r3, [r2]
.L_08121af6:
	movs r3, #0
	ldrsb r3, [r2, r3]
	cmp r3, #4
	ble .L_08121b02
	movs r3, #4
	strb r3, [r2]
.L_08121b02:
	mov r0, r10
	bl BattleUnit_Recalculate
	ldr r2, [sp, #20]
	mov r0, r8
	ldrh r3, [r2, #60]
	ldrh r1, [r0, #60]
	movs r0, #1
	subs r1, r1, r3
	bl BattleEv_Push
	ldr r1, .L_08121c3c
	movs r0, #4
	bl BattleEv_Push
	movs r2, #153
	lsls r2, r2, #1
	add r2, r8
	movs r3, #7
	bl .L_081223b8
.L_08121b2c:
	movs r2, #54
	adds r2, #255
	add r2, r8
	ldrb r3, [r2]
	movs r4, #4
	subs r3, #1
	strb r3, [r2]
	lsls r3, r3, #24
	asrs r3, r3, #24
	negs r4, r4
	cmp r3, r4
	bge .L_08121b48
	movs r3, #252
	strb r3, [r2]
.L_08121b48:
	movs r3, #0
	ldrsb r3, [r2, r3]
	cmp r3, #4
	ble .L_08121b54
	movs r3, #4
	strb r3, [r2]
.L_08121b54:
	mov r0, r10
	bl BattleUnit_Recalculate
	ldr r5, [sp, #20]
	mov r0, r8
	ldrh r3, [r0, #62]
	ldrh r1, [r5, #62]
	movs r0, #1
	subs r1, r1, r3
	bl BattleEv_Push
	ldr r1, .L_08121c40
	movs r0, #4
	bl BattleEv_Push
	movs r2, #154
	lsls r2, r2, #1
	add r2, r8
	movs r3, #7
	bl .L_081223b8
.L_08121b7e:
	movs r2, #54
	adds r2, #255
	add r2, r8
	ldrb r3, [r2]
	movs r1, #4
	subs r3, #2
	strb r3, [r2]
	lsls r3, r3, #24
	asrs r3, r3, #24
	negs r1, r1
	cmp r3, r1
	bge .L_08121b9a
	movs r3, #252
	strb r3, [r2]
.L_08121b9a:
	movs r3, #0
	ldrsb r3, [r2, r3]
	cmp r3, #4
	ble .L_08121ba6
	movs r3, #4
	strb r3, [r2]
.L_08121ba6:
	mov r0, r10
	bl BattleUnit_Recalculate
	ldr r2, [sp, #20]
	mov r4, r8
	ldrh r1, [r2, #62]
	ldrh r3, [r4, #62]
	movs r0, #1
	subs r1, r1, r3
	bl BattleEv_Push
	ldr r1, .L_08121c40
	movs r0, #4
	bl BattleEv_Push
	movs r2, #154
	lsls r2, r2, #1
	add r2, r8
	movs r3, #7
	bl .L_081223b8
.L_08121bd0:
	movs r2, #54
	adds r2, #255
	add r2, r8
	ldrb r3, [r2]
	movs r5, #4
	adds r3, #1
	strb r3, [r2]
	lsls r3, r3, #24
	asrs r3, r3, #24
	negs r5, r5
	cmp r3, r5
	bge .L_08121bec
	movs r3, #252
	strb r3, [r2]
.L_08121bec:
	movs r3, #0
	ldrsb r3, [r2, r3]
	cmp r3, #4
	ble .L_08121bf8
	movs r3, #4
	strb r3, [r2]
.L_08121bf8:
	mov r0, r10
	bl BattleUnit_Recalculate
	ldr r2, [sp, #20]
	mov r0, r8
	ldrh r3, [r2, #62]
	ldrh r1, [r0, #62]
	movs r0, #1
	subs r1, r1, r3
	bl BattleEv_Push
	ldr r1, .L_08121c44
	movs r0, #4
	bl BattleEv_Push
	movs r2, #154
	lsls r2, r2, #1
	add r2, r8
	movs r3, #7
	b .L_081223b8
.L_08121c20:
	.4byte 0x00000c6c
.L_08121c24:
	.4byte 0x00000c69
.L_08121c28:
	.4byte 0x00000c6d
.L_08121c2c:
	.4byte 0x00000c6a
.L_08121c30:
	.4byte 0x00000cd3
.L_08121c34:
	.4byte 0x00000cd4
.L_08121c38:
	.4byte 0x00000cbc
.L_08121c3c:
	.4byte 0x00000cbd
.L_08121c40:
	.4byte 0x00000cbe
.L_08121c44:
	.4byte 0x00000cbf
.L_08121c48:
	movs r2, #54
	adds r2, #255
	add r2, r8
	ldrb r3, [r2]
	movs r4, #4
	adds r3, #2
	strb r3, [r2]
	lsls r3, r3, #24
	asrs r3, r3, #24
	negs r4, r4
	cmp r3, r4
	bge .L_08121c64
	movs r3, #252
	strb r3, [r2]
.L_08121c64:
	movs r3, #0
	ldrsb r3, [r2, r3]
	cmp r3, #4
	ble .L_08121c70
	movs r3, #4
	strb r3, [r2]
.L_08121c70:
	mov r0, r10
	bl BattleUnit_Recalculate
	ldr r0, [sp, #20]
	mov r5, r8
	ldrh r3, [r0, #62]
	ldrh r1, [r5, #62]
	movs r0, #1
	subs r1, r1, r3
	bl BattleEv_Push
	ldr r1, .L_08121fec
	movs r0, #4
	bl BattleEv_Push
	movs r2, #154
	lsls r2, r2, #1
	add r2, r8
	movs r3, #7
	b .L_081223b8
.L_08121c98:
	mov r2, r8
	movs r1, #56
	ldrsh r3, [r2, r1]
	cmp r3, #0
	beq .L_08121ca4
	b .L_081223ba
.L_08121ca4:
	movs r0, #4
	ldr r1, .L_08121ff0
	bl BattleEv_Push
	mov r4, r8
	ldrh r3, [r4, #52]
	mov r5, r8
	strh r3, [r5, #56]
	b .L_08121fe4
.L_08121cb6:
	mov r1, r8
	movs r0, #56
	ldrsh r3, [r1, r0]
	cmp r3, #0
	beq .L_08121cc2
	b .L_081223ba
.L_08121cc2:
	movs r0, #4
	ldr r1, .L_08121ff0
	bl BattleEv_Push
	mov r2, r8
	ldrh r3, [r2, #52]
	mov r0, r10
	lsls r3, r3, #16
	asrs r2, r3, #16
	lsrs r3, r3, #31
	adds r2, r2, r3
	asrs r2, r2, #1
	mov r3, r8
	strh r2, [r3, #56]
	bl Owner_RecalculateRatiosFar
	b .L_081223ba
.L_08121ce4:
	mov r5, r8
	movs r4, #56
	ldrsh r3, [r5, r4]
	cmp r3, #0
	beq .L_08121cf0
	b .L_081223ba
.L_08121cf0:
	ldr r1, .L_08121ff0
	movs r0, #4
	bl BattleEv_Push
	movs r1, #52
	ldrsh r0, [r5, r1]
	movs r1, #10
	lsls r0, r0, #3
	bl __divsi3
	mov r2, r8
	strh r0, [r2, #56]
	b .L_08121fe4
.L_08121d0a:
	mov r5, r8
	movs r4, #56
	ldrsh r3, [r5, r4]
	cmp r3, #0
	beq .L_08121d16
	b .L_081223ba
.L_08121d16:
	movs r0, #4
	ldr r1, .L_08121ff0
	bl BattleEv_Push
	movs r0, #52
	ldrsh r3, [r5, r0]
	movs r1, #10
	lsls r0, r3, #1
	adds r0, r0, r3
	lsls r0, r0, #1
	bl __divsi3
	mov r1, r8
	strh r0, [r1, #56]
	b .L_08121fe4
.L_08121d34:
	movs r5, #50
	adds r5, #255
	add r5, r8
	movs r3, #0
	ldrsb r3, [r5, r3]
	cmp r3, #0
	beq .L_08121d4a
	ldr r1, .L_08121ff4
	movs r0, #4
	bl BattleEv_Push
.L_08121d4a:
	movs r3, #0
	strb r3, [r5]
	b .L_081223ba
.L_08121d50:
	movs r2, #56
	adds r2, #255
	add r2, r8
	ldrb r3, [r2]
	movs r4, #4
	subs r3, #1
	strb r3, [r2]
	lsls r3, r3, #24
	asrs r3, r3, #24
	negs r4, r4
	cmp r3, r4
	bge .L_08121d6c
	movs r3, #252
	strb r3, [r2]
.L_08121d6c:
	movs r3, #0
	ldrsb r3, [r2, r3]
	ldrb r1, [r2]
	cmp r3, #4
	ble .L_08121d7c
	movs r3, #4
	strb r3, [r2]
	movs r1, #4
.L_08121d7c:
	ldr r5, [sp, #20]
	movs r0, #56
	adds r0, #255
	adds r3, r5, r0
	movs r2, #0
	ldrsb r2, [r3, r2]
	lsls r3, r1, #24
	asrs r3, r3, #24
	subs r2, r2, r3
	lsls r1, r2, #2
	adds r1, r1, r2
	lsls r1, r1, #2
	movs r0, #1
	bl BattleEv_Push
	ldr r1, .L_08121ff8
	movs r0, #4
	bl BattleEv_Push
	movs r2, #155
	lsls r2, r2, #1
	add r2, r8
	movs r3, #7
	b .L_081223b8
.L_08121dac:
	movs r2, #56
	adds r2, #255
	add r2, r8
	ldrb r3, [r2]
	movs r1, #4
	subs r3, #2
	strb r3, [r2]
	lsls r3, r3, #24
	asrs r3, r3, #24
	negs r1, r1
	cmp r3, r1
	bge .L_08121dc8
	movs r3, #252
	strb r3, [r2]
.L_08121dc8:
	movs r3, #0
	ldrsb r3, [r2, r3]
	ldrb r1, [r2]
	cmp r3, #4
	ble .L_08121dd8
	movs r3, #4
	strb r3, [r2]
	movs r1, #4
.L_08121dd8:
	ldr r2, [sp, #20]
	movs r4, #56
	adds r4, #255
	adds r3, r2, r4
	movs r2, #0
	ldrsb r2, [r3, r2]
	lsls r3, r1, #24
	asrs r3, r3, #24
	subs r2, r2, r3
	lsls r1, r2, #2
	adds r1, r1, r2
	lsls r1, r1, #2
	movs r0, #1
	bl BattleEv_Push
	ldr r1, .L_08121ff8
	movs r0, #4
	bl BattleEv_Push
	movs r2, #155
	lsls r2, r2, #1
	add r2, r8
	movs r3, #7
	b .L_081223b8
.L_08121e08:
	movs r2, #56
	adds r2, #255
	add r2, r8
	ldrb r3, [r2]
	movs r5, #4
	adds r3, #1
	strb r3, [r2]
	lsls r3, r3, #24
	asrs r3, r3, #24
	negs r5, r5
	cmp r3, r5
	bge .L_08121e24
	movs r3, #252
	strb r3, [r2]
.L_08121e24:
	movs r3, #0
	ldrsb r3, [r2, r3]
	ldrb r1, [r2]
	cmp r3, #4
	ble .L_08121e34
	movs r3, #4
	strb r3, [r2]
	movs r1, #4
.L_08121e34:
	ldr r0, [sp, #20]
	lsls r3, r1, #24
	movs r1, #56
	adds r1, #255
	adds r2, r0, r1
	ldrb r2, [r2]
	lsls r2, r2, #24
	asrs r2, r2, #24
	asrs r3, r3, #24
	subs r3, r3, r2
	lsls r1, r3, #2
	adds r1, r1, r3
	lsls r1, r1, #2
	movs r0, #1
	bl BattleEv_Push
	ldr r1, .L_08121ffc
	movs r0, #4
	bl BattleEv_Push
	movs r2, #155
	lsls r2, r2, #1
	add r2, r8
	movs r3, #7
	b .L_081223b8
.L_08121e66:
	movs r2, #56
	adds r2, #255
	add r2, r8
	ldrb r3, [r2]
	movs r4, #4
	adds r3, #2
	strb r3, [r2]
	lsls r3, r3, #24
	asrs r3, r3, #24
	negs r4, r4
	cmp r3, r4
	bge .L_08121e82
	movs r3, #252
	strb r3, [r2]
.L_08121e82:
	movs r3, #0
	ldrsb r3, [r2, r3]
	ldrb r1, [r2]
	cmp r3, #4
	ble .L_08121e92
	movs r3, #4
	strb r3, [r2]
	movs r1, #4
.L_08121e92:
	ldr r5, [sp, #20]
	movs r0, #56
	adds r0, #255
	adds r2, r5, r0
	ldrb r2, [r2]
	lsls r2, r2, #24
	asrs r2, r2, #24
	lsls r3, r1, #24
	asrs r3, r3, #24
	subs r3, r3, r2
	lsls r1, r3, #2
	adds r1, r1, r3
	lsls r1, r1, #2
	movs r0, #1
	bl BattleEv_Push
	ldr r1, .L_08121ffc
	movs r0, #4
	bl BattleEv_Push
	movs r2, #155
	lsls r2, r2, #1
	add r2, r8
	movs r3, #7
	b .L_081223b8
.L_08121ec4:
	movs r5, #50
	adds r5, #255
	add r5, r8
	movs r3, #0
	ldrsb r3, [r5, r3]
	cmp r3, #0
	beq .L_08121ed4
	b .L_081223ba
.L_08121ed4:
	ldr r1, .L_08122000
	movs r0, #4
	bl BattleEv_Push
	movs r3, #1
	strb r3, [r5]
	b .L_081223ba
.L_08121ee2:
	movs r5, #50
	adds r5, #255
	add r5, r8
	movs r3, #0
	ldrsb r3, [r5, r3]
	cmp r3, #1
	ble .L_08121ef2
	b .L_081223ba
.L_08121ef2:
	ldr r1, .L_08122004
	movs r0, #4
	bl BattleEv_Push
	movs r3, #2
	strb r3, [r5]
	b .L_081223ba
.L_08121f00:
	ldr r1, .L_08122008
	movs r0, #4
	bl BattleEv_Push
	movs r2, #156
	lsls r2, r2, #1
	add r2, r8
	movs r3, #7
	b .L_081223b8
.L_08121f12:
	ldr r1, .L_0812200c
	movs r0, #4
	bl BattleEv_Push
	movs r2, #58
	adds r2, #255
	add r2, r8
	movs r3, #7
	b .L_081223b8
.L_08121f24:
	ldr r1, .L_08122010
	movs r0, #4
	bl BattleEv_Push
	movs r2, #157
	lsls r2, r2, #1
	add r2, r8
	movs r3, #7
	b .L_081223b8
.L_08121f36:
	ldr r1, .L_08122014
	movs r0, #4
	bl BattleEv_Push
	movs r2, #60
	adds r2, #255
	add r2, r8
	movs r3, #7
	b .L_081223b8
.L_08121f48:
	ldr r1, .L_08122018
	movs r0, #4
	bl BattleEv_Push
	movs r2, #158
	lsls r2, r2, #1
	add r2, r8
	movs r3, #7
	b .L_081223b8
.L_08121f5a:
	mov r1, r10
	cmp r1, #7
	bhi .L_08121f6a
	ldr r1, .L_0812201c
	movs r0, #4
	bl BattleEv_Push
	b .L_08121f72
.L_08121f6a:
	ldr r1, .L_08122020
	movs r0, #4
	bl BattleEv_Push
.L_08121f72:
	movs r1, #62
	adds r1, #255
	add r1, r8
	ldrb r2, [r1]
	movs r3, #7
	b .L_081222b2
.L_08121f7e:
	mov r2, r10
	cmp r2, #7
	bhi .L_08121f8e
	ldr r1, .L_0812201c
	movs r0, #4
	bl BattleEv_Push
	b .L_08121f96
.L_08121f8e:
	ldr r1, .L_08122020
	movs r0, #4
	bl BattleEv_Push
.L_08121f96:
	movs r1, #62
	adds r1, #255
	add r1, r8
	ldrb r2, [r1]
	movs r3, #16
	b .L_081222b2
.L_08121fa2:
	mov r0, r10
	bl BattleUnit_KeepsOneHp
	cmp r0, #0
	beq .L_08121fae
	b .L_081223ba
.L_08121fae:
	movs r0, #9
	mov r1, r10
	bl BattleEv_Push
	movs r3, #149
	lsls r3, r3, #1
	add r3, r8
	ldrb r3, [r3]
	cmp r3, #2
	bne .L_08121fc6
	ldr r1, .L_08122024
	b .L_08121fce
.L_08121fc6:
	mov r3, r11
	cmp r3, #219
	bne .L_08121fd6
	ldr r1, .L_08122028
.L_08121fce:
	movs r0, #4
	bl BattleEv_Push
	b .L_08121fde
.L_08121fd6:
	ldr r1, .L_0812202c
	movs r0, #4
	bl BattleEv_Push
.L_08121fde:
	movs r3, #0
	mov r4, r8
	strh r3, [r4, #56]
.L_08121fe4:
	mov r0, r10
	bl Owner_RecalculateRatiosFar
	b .L_081223ba
.L_08121fec:
	.4byte 0x00000cbf
.L_08121ff0:
	.4byte 0x00000cc0
.L_08121ff4:
	.4byte 0x00000ce4
.L_08121ff8:
	.4byte 0x00000cc1
.L_08121ffc:
	.4byte 0x00000cc2
.L_08122000:
	.4byte 0x00000cc3
.L_08122004:
	.4byte 0x00000cd0
.L_08122008:
	.4byte 0x00000cc4
.L_0812200c:
	.4byte 0x00000cc5
.L_08122010:
	.4byte 0x00000cc6
.L_08122014:
	.4byte 0x00000cc7
.L_08122018:
	.4byte 0x00000cc8
.L_0812201c:
	.4byte 0x00000cc9
.L_08122020:
	.4byte 0x00000cd2
.L_08122024:
	.4byte 0x00000ca4
.L_08122028:
	.4byte 0x00000ca5
.L_0812202c:
	.4byte 0x00000ca1
.L_08122030:
	ldr r1, .L_08122350
	movs r0, #4
	bl BattleEv_Push
	movs r2, #159
	lsls r2, r2, #1
	add r2, r8
	movs r3, #5
	b .L_081223b8
.L_08122042:
	ldr r1, .L_08122354
	movs r0, #4
	bl BattleEv_Push
	movs r2, #64
	adds r2, #255
	add r2, r8
	movs r3, #7
	b .L_081223b8
.L_08122054:
	ldr r1, [sp, #80]
	ldr r4, [sp, #84]
	movs r0, #56
	ldrsh r5, [r1, r0]
	ldrb r3, [r4, #3]
	adds r2, r5, #0
	ldr r7, [sp, #48]
	cmp r3, #60
	bne .L_0812206e
	adds r0, r7, #0
	lsrs r3, r0, #31
	adds r3, r0, r3
	asrs r7, r3, #1
.L_0812206e:
	ldr r4, [sp, #80]
	adds r6, r7, #0
	movs r1, #52
	ldrsh r3, [r4, r1]
	adds r5, r5, r6
	cmp r5, r3
	ble .L_08122080
	adds r5, r3, #0
	subs r6, r5, r2
.L_08122080:
	movs r0, #7
	movs r1, #0
	bl BattleEv_Push
	movs r0, #0
	ldr r1, [sp, #76]
	bl BattleEv_Push
	ldr r1, [sp, #80]
	movs r0, #52
	ldrsh r3, [r1, r0]
	cmp r5, r3
	bne .L_081220a4
	ldr r1, .L_08122358
	movs r0, #4
	bl BattleEv_Push
	b .L_081220b4
.L_081220a4:
	movs r0, #1
	adds r1, r6, #0
	bl BattleEv_Push
	ldr r1, .L_0812235c
	movs r0, #4
	bl BattleEv_Push
.L_081220b4:
	ldr r2, [sp, #80]
	strh r5, [r2, #56]
	b .L_0812210a
.L_081220ba:
	ldr r4, [sp, #80]
	ldr r6, [sp, #48]
	movs r3, #58
	ldrsh r5, [r4, r3]
	movs r0, #54
	ldrsh r3, [r4, r0]
	adds r2, r5, #0
	adds r5, r5, r6
	cmp r5, r3
	ble .L_081220d2
	adds r5, r3, #0
	subs r6, r5, r2
.L_081220d2:
	movs r0, #7
	movs r1, #0
	bl BattleEv_Push
	movs r0, #0
	ldr r1, [sp, #76]
	bl BattleEv_Push
	ldr r2, [sp, #80]
	movs r1, #54
	ldrsh r3, [r2, r1]
	cmp r5, r3
	bne .L_081220f6
	ldr r1, .L_08122360
	movs r0, #4
	bl BattleEv_Push
	b .L_08122106
.L_081220f6:
	movs r0, #1
	adds r1, r6, #0
	bl BattleEv_Push
	ldr r1, .L_08122364
	movs r0, #4
	bl BattleEv_Push
.L_08122106:
	ldr r3, [sp, #80]
	strh r5, [r3, #58]
.L_0812210a:
	ldr r0, [sp, #76]
	bl Owner_RecalculateRatiosFar
	b .L_081223ba
.L_08122112:
	ldr r0, [sp, #48]
	movs r1, #10
	bl __divsi3
	adds r5, r0, #0
	mov r0, r8
	movs r4, #58
	ldrsh r3, [r0, r4]
	cmp r3, r5
	bge .L_08122128
	adds r5, r3, #0
.L_08122128:
	ldr r3, [sp, #80]
	ldr r0, [sp, #80]
	movs r2, #58
	ldrsh r1, [r3, r2]
	movs r4, #54
	ldrsh r2, [r0, r4]
	adds r3, r1, r5
	cmp r3, r2
	ble .L_0812213c
	subs r5, r2, r1
.L_0812213c:
	cmp r5, #0
	bne .L_08122142
	b .L_081223ba
.L_08122142:
	adds r1, r5, #0
	movs r0, #1
	bl BattleEv_Push
	mov r1, r10
	cmp r1, #7
	bhi .L_0812215a
	ldr r1, .L_08122368
	movs r0, #4
	bl BattleEv_Push
	b .L_08122162
.L_0812215a:
	ldr r1, .L_0812236c
	movs r0, #4
	bl BattleEv_Push
.L_08122162:
	ldr r0, [sp, #76]
	adds r1, r5, #0
	bl Owner_AdjustSecondValueFar
	b .L_081223ba
.L_0812216c:
	mov r3, r8
	movs r2, #54
	ldrsh r0, [r3, r2]
	movs r1, #10
	bl __divsi3
	lsls r0, r0, #16
	asrs r5, r0, #16
	mov r0, r8
	movs r4, #58
	ldrsh r3, [r0, r4]
	cmp r3, r5
	bge .L_08122188
	adds r5, r3, #0
.L_08122188:
	cmp r5, #0
	bne .L_0812218e
	b .L_081223ba
.L_0812218e:
	adds r1, r5, #0
	movs r0, #1
	bl BattleEv_Push
	mov r1, r10
	cmp r1, #7
	bhi .L_081221a6
	ldr r1, .L_08122370
	movs r0, #4
	bl BattleEv_Push
	b .L_081221ae
.L_081221a6:
	ldr r1, .L_08122374
	movs r0, #4
	bl BattleEv_Push
.L_081221ae:
	negs r1, r5
	mov r0, r10
	bl Owner_AdjustSecondValueFar
	b .L_081223ba
.L_081221b8:
	movs r2, #52
	adds r2, #255
	add r2, r8
	movs r3, #0
	ldrsb r3, [r2, r3]
	cmp r3, #0
	ble .L_081221d2
	movs r3, #0
	strb r3, [r2]
	movs r2, #153
	lsls r2, r2, #1
	add r2, r8
	strb r3, [r2]
.L_081221d2:
	movs r1, #54
	adds r1, #255
	add r1, r8
	movs r3, #0
	ldrsb r3, [r1, r3]
	cmp r3, #0
	ble .L_081221ec
	movs r3, #154
	lsls r3, r3, #1
	movs r2, #0
	add r3, r8
	strb r2, [r1]
	strb r2, [r3]
.L_081221ec:
	movs r2, #56
	adds r2, #255
	add r2, r8
	movs r3, #0
	ldrsb r3, [r2, r3]
	cmp r3, #0
	ble .L_08122206
	movs r3, #0
	strb r3, [r2]
	movs r2, #155
	lsls r2, r2, #1
	add r2, r8
	strb r3, [r2]
.L_08122206:
	movs r2, #72
	adds r2, #255
	add r2, r8
	movs r3, #0
	ldrsb r3, [r2, r3]
	cmp r3, #0
	ble .L_08122218
	movs r3, #0
	strb r3, [r2]
.L_08122218:
	movs r3, #150
	lsls r3, r3, #1
	movs r2, #0
	add r3, r8
	strb r2, [r3]
	movs r3, #46
	adds r3, #255
	add r3, r8
	strb r2, [r3]
	movs r3, #151
	lsls r3, r3, #1
	add r3, r8
	strb r2, [r3]
	movs r3, #48
	adds r3, #255
	add r3, r8
	strb r2, [r3]
	ldr r1, .L_08122378
	b .L_08122326
.L_0812223e:
	ldr r1, .L_0812237c
	movs r0, #4
	bl BattleEv_Push
	movs r2, #160
	lsls r2, r2, #1
	b .L_081223b4
.L_0812224c:
	movs r5, #66
	adds r5, #255
	add r5, r8
	ldrb r3, [r5]
	adds r2, r3, #0
	cmp r2, #0
	bne .L_08122268
	ldr r1, .L_08122380
	movs r0, #4
	bl BattleEv_Push
	movs r3, #7
	strb r3, [r5]
	b .L_081223ba
.L_08122268:
	cmp r2, #1
	bhi .L_0812226e
	b .L_081223ba
.L_0812226e:
	adds r3, #255
	strb r3, [r5]
	movs r0, #1
	ldrb r1, [r5]
	bl BattleEv_Push
	ldr r1, .L_08122384
	b .L_08122326
.L_0812227e:
	ldr r1, .L_08122388
	movs r0, #4
	bl BattleEv_Push
	movs r2, #162
	lsls r2, r2, #1
	add r2, r8
	movs r3, #2
	b .L_081223b8
.L_08122290:
	ldr r1, .L_0812238c
	movs r0, #4
	bl BattleEv_Push
	movs r2, #164
	lsls r2, r2, #1
	add r2, r8
	movs r3, #1
	strb r3, [r2]
	mov r2, r10
	cmp r2, #7
	bls .L_081222aa
	b .L_081223ba
.L_081222aa:
	ldr r1, [sp, #64]
	movs r2, #2
	adds r1, #67
	ldrb r3, [r1]
.L_081222b2:
	orrs r3, r2
	strb r3, [r1]
	b .L_081223ba
.L_081222b8:
	ldr r1, .L_08122390
	movs r0, #4
	bl BattleEv_Push
	movs r2, #70
	b .L_081223b2
.L_081222c4:
	ldr r1, .L_08122394
	movs r0, #4
	bl BattleEv_Push
	movs r2, #44
	adds r2, #255
	add r2, r8
	ldrb r3, [r2]
	cmp r3, #0
	bne .L_081223ba
	b .L_081223b6
.L_081222da:
	ldr r1, .L_08122394
	movs r0, #4
	bl BattleEv_Push
	movs r2, #44
	adds r2, #255
	add r2, r8
	ldrb r3, [r2]
	cmp r3, #1
	bhi .L_081223ba
	movs r3, #2
	b .L_081223b8
.L_081222f2:
	ldr r1, .L_08122398
	movs r0, #4
	bl BattleEv_Push
	movs r2, #44
	adds r2, #255
	add r2, r8
	ldrb r3, [r2]
	cmp r3, #2
	bhi .L_081223ba
	movs r3, #3
	b .L_081223b8
.L_0812230a:
	ldr r1, .L_0812239c
	movs r0, #4
	bl BattleEv_Push
	movs r2, #44
	adds r2, #255
	add r2, r8
	ldrb r3, [r2]
	cmp r3, #3
	bhi .L_081223ba
	movs r3, #4
	b .L_081223b8
.L_08122322:
	movs r1, #1
	negs r1, r1
.L_08122326:
	movs r0, #4
	bl BattleEv_Push
	b .L_081223ba
.L_0812232e:
	ldr r1, .L_081223a0
	movs r0, #4
	bl BattleEv_Push
	ldr r2, [sp, #64]
	movs r3, #1
	adds r2, #71
	b .L_081223b8
.L_0812233e:
	ldr r1, .L_081223a4
	movs r0, #4
	bl BattleEv_Push
	movs r0, #15
	mov r1, r10
	bl BattleEv_Push
	b .L_081223ba
.L_08122350:
	.4byte 0x00000ccb
.L_08122354:
	.4byte 0x00000ccc
.L_08122358:
	.4byte 0x00000c6c
.L_0812235c:
	.4byte 0x00000c69
.L_08122360:
	.4byte 0x00000c6d
.L_08122364:
	.4byte 0x00000c6a
.L_08122368:
	.4byte 0x00000cb9
.L_0812236c:
	.4byte 0x00000cb8
.L_08122370:
	.4byte 0x00000cbb
.L_08122374:
	.4byte 0x00000cba
.L_08122378:
	.4byte 0x00000cf6
.L_0812237c:
	.4byte 0x00000cce
.L_08122380:
	.4byte 0x00000ccf
.L_08122384:
	.4byte 0x00000cd1
.L_08122388:
	.4byte 0x00000cd9
.L_0812238c:
	.4byte 0x00000cda
.L_08122390:
	.4byte 0x00000cdb
.L_08122394:
	.4byte 0x00000cdd
.L_08122398:
	.4byte 0x00000cde
.L_0812239c:
	.4byte 0x00000cdf
.L_081223a0:
	.4byte 0x00000c8f
.L_081223a4:
	.4byte 0x00000ca6
.L_081223a8:
	ldr r1, .L_081224b8
	movs r0, #4
	bl BattleEv_Push
	movs r2, #68
.L_081223b2:
	adds r2, #255
.L_081223b4:
	add r2, r8
.L_081223b6:
	movs r3, #1
.L_081223b8:
	strb r3, [r2]
.L_081223ba:
	movs r0, #7
	movs r1, #0
	bl BattleEv_Push
	ldr r5, [sp, #4]
	movs r4, #0
	ldrsh r3, [r5, r4]
	cmp r3, #9
	beq .L_08122408
	movs r3, #68
	adds r3, #255
	add r3, r8
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	beq .L_08122408
	ldr r0, [sp, #84]
	ldrb r3, [r0]
	cmp r3, #1
	bne .L_08122408
	mov r2, r8
	movs r1, #56
	ldrsh r3, [r2, r1]
	cmp r3, #0
	beq .L_08122446
	ldr r4, [sp, #64]
	movs r5, #128
	lsls r5, r5, #4
	adds r5, #88
	adds r3, r4, r5
	mov r0, r10
	str r0, [r3]
	movs r1, #128
	lsls r1, r1, #4
	ldr r2, [sp, #76]
	adds r1, #92
	adds r3, r4, r1
	str r2, [r3]
.L_08122408:
	mov r5, r8
	movs r4, #56
	ldrsh r3, [r5, r4]
	cmp r3, #0
	beq .L_08122446
	movs r5, #158
	lsls r5, r5, #1
	add r5, r8
	ldrb r3, [r5]
	cmp r3, #0
	beq .L_08122446
	cmp r3, #6
	bhi .L_08122446
	ldr r0, [sp, #48]
	cmp r0, #0
	ble .L_08122446
	bl BattleRandom16Far
	movs r3, #3
	ands r0, r3
	cmp r0, #0
	bne .L_08122446
	strb r0, [r5]
	mov r1, r10
	movs r0, #0
	bl BattleEv_Push
	ldr r1, .L_081224bc
	movs r0, #4
	bl BattleEv_Push
.L_08122446:
	ldr r0, [sp, #20]
	bl Sys_Free
	mov r0, r10
	bl BattleUnit_Recalculate
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #36]
	adds r3, #65
	ldrb r0, [r3]
	bl UiWindow_DrawPartyStatusContentsFar
	mov r2, r8
	movs r1, #56
	ldrsh r3, [r2, r1]
	cmp r3, #0
	beq .L_08122472
	movs r0, #11
	mov r1, r10
	bl BattleEv_Push
.L_08122472:
	movs r3, #148
	adds r3, #255
	cmp r11, r3
	beq .L_081224aa
	ldr r4, [sp, #80]
	movs r5, #160
	lsls r5, r5, #1
	adds r3, r4, r5
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_081224aa
	bl BattleRandom16Far
	movs r3, #3
	ands r0, r3
	cmp r0, #0
	bne .L_081224aa
	ldr r0, [sp, #48]
	cmp r0, #0
	ble .L_081224aa
	asrs r0, r0, #2
	cmp r0, #0
	bne .L_081224a2
	movs r0, #1
.L_081224a2:
	ldr r1, [sp, #92]
	ldr r3, [r1, #96]
	adds r3, r3, r0
	str r3, [r1, #96]
.L_081224aa:
	add sp, #112
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_081224b8:
	.4byte 0x00000ce0
.L_081224bc:
	.4byte 0x00000ce3
